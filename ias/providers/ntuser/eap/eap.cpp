///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) Microsoft Corporation
//
// SYNOPSIS
//
//   Defines the class EAP.
//
///////////////////////////////////////////////////////////////////////////////

#include <ias.h>
#include <iaslsa.h>
#include <iastlutl.h>

#include <eap.h>
#include <eapdnary.h>
#include <eapstate.h>
#include <eaptypes.h>
#include <vector>

//////////
// Define static members.
//////////
EAPTypes EAP::theTypes;

STDMETHODIMP EAP::Initialize()
{
   HRESULT hr;

   // Initialize the LSA API.
   DWORD error = IASLsaInitialize();
   if (error)
   {
      hr = HRESULT_FROM_WIN32(error);
      goto lsa_failed;
   }

   // Initialize the sessions.
   hr = EAPSession::initialize();
   if (FAILED(hr))
   {
      goto sessions_failed;
   }

   // Initialize the IAS <--> RAS translator.
   hr = EAPTranslator::initialize();
   if (FAILED(hr))
   {
      goto translator_failed;
   }

   // The rest can't fail.
   EAPState::initialize();
   theTypes.initialize();

   // Everything succeeded, so we're done.
   return S_OK;

translator_failed:
   EAPSession::finalize();

sessions_failed:
   IASLsaUninitialize();

lsa_failed:
   return hr;
}

STDMETHODIMP EAP::Shutdown()
{
   // Clear out any remaining sessions.
   sessions.clear();

   // Shutdown our sub-systems.
   theTypes.finalize();
   EAPTranslator::finalize();
   EAPSession::finalize();
   IASLsaUninitialize();

   return S_OK;
}

STDMETHODIMP EAP::PutProperty(LONG Id, VARIANT* pValue)
{
   if (pValue == NULL) { return E_INVALIDARG; }

   switch (Id)
   {
      case PROPERTY_EAP_SESSION_TIMEOUT:
      {
         if (V_VT(pValue) != VT_I4) { return DISP_E_TYPEMISMATCH; }
         if (V_I4(pValue) <= 0) { return E_INVALIDARG; }

         IASTracePrintf("Setting EAP session timeout to %ld msec.", V_I4(pValue));

         sessions.setSessionTimeout(V_I4(pValue));
         break;
      }

      case PROPERTY_EAP_MAX_SESSIONS:
      {
         if (V_VT(pValue) != VT_I4) { return DISP_E_TYPEMISMATCH; }
         if (V_I4(pValue) <= 0) { return E_INVALIDARG; }

         IASTracePrintf("Setting max. EAP sessions to %ld.", V_I4(pValue));

         sessions.setMaxSessions(V_I4(pValue));
         break;
      }

      default:
      {
         return DISP_E_MEMBERNOTFOUND;
      }
   }

   return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
//
// METHOD
//
//    EAP::onSyncRequest
//
// DESCRIPTION
//
//    Processes a request. Note that this method does only enough work to
//    retrieve or create a session object. Once this has been accomplished
//    the main processing logic takes place inside EAPSession (q.v.).
//
///////////////////////////////////////////////////////////////////////////////
IASREQUESTSTATUS EAP::onSyncRequest(IRequest* pRequest) throw ()
{
   EAPSession* session = NULL;

   try
   {
      IASRequest request(pRequest);

      //////////
      // Does the request contain an EAP-Message?
      //////////

      DWORD attrID = RADIUS_ATTRIBUTE_EAP_MESSAGE;
      IASAttributeVectorWithBuffer<16> eapMessage;
      if (!eapMessage.load(request, 1, &attrID))
      {
         // If not, we're not interested.
         return IAS_REQUEST_STATUS_CONTINUE;
      }

      IASTraceString("NT-SAM EAP handler received request.");

      //////////
      // Concatenate the RADIUS EAP-Message attributes into a single packet.
      //////////

      IASAttributeVector::iterator it;
      DWORD pktlen = 0;
      for (it = eapMessage.begin(); it != eapMessage.end(); ++it)
      {
         pktlen += it->pAttribute->Value.OctetString.dwLength;
      }

      PBYTE p = (PBYTE)_alloca(pktlen);
      PPPP_EAP_PACKET recvPkt = (PPPP_EAP_PACKET)p;
      for (it = eapMessage.begin(); it != eapMessage.end(); ++it)
      {
         memcpy(p,
                it->pAttribute->Value.OctetString.lpValue,
                it->pAttribute->Value.OctetString.dwLength);
         p += it->pAttribute->Value.OctetString.dwLength;
      }

      //////////
      // Ensure that the packet is valid.
      //////////

      if (pktlen < 5 || IASExtractWORD(recvPkt->Length) != pktlen)
      {
         IASTraceString("Assembled EAP-Message has invalid length.");

         request.SetResponse(IAS_RESPONSE_DISCARD_PACKET,
                             IAS_MALFORMED_REQUEST);
         return IAS_REQUEST_STATUS_ABORT;
      }

      //////////
      // Get a session object to handle this request.
      //////////

      IASREQUESTSTATUS retval;
      IASAttribute state;
      if (state.load(request, RADIUS_ATTRIBUTE_STATE, IASTYPE_OCTET_STRING))
      {
         //////////
         // If the State attribute exists, this is an ongoing session.
         //////////

         EAPState& s = (EAPState&)(state->Value.OctetString);

         if (!s.isValid())
         {
            IASTraceString("State attribute is present, but unrecognized.");

            // We don't recognize this state attribute, so it must belong
            // to someone else.
            return IAS_REQUEST_STATUS_CONTINUE;
         }

         // Retrieve the object for this session ID.
         session = sessions.remove(s.getSessionID());

         if (!session)
         {
            IASTraceString("Session timed-out. Discarding packet.");

            // The session is already complete.
            request.SetResponse(IAS_RESPONSE_DISCARD_PACKET,
                                IAS_SESSION_TIMEOUT);
            return IAS_REQUEST_STATUS_ABORT;
         }

         IASTracePrintf("Successfully retrieved session state for user %S.",
                        session->getAccountName());

         retval = session->process(request, recvPkt);
      }
      else
      {
         IASTraceString("No State attribute present. Creating new session.");

         //////////
         // No state attribute, so this is a new session.
         // Does the request contain an NT4-Account-Name ?
         //////////

         IASAttribute identity;
         if (!identity.load(request,
                            IAS_ATTRIBUTE_NT4_ACCOUNT_NAME,
                            IASTYPE_STRING))
         {
            IASTraceString("SAM account name not found.");

            // We only handle SAM users.
            return IAS_REQUEST_STATUS_CONTINUE;
         }

         //////////
         // Find out which EAP provider to use.
         //////////

         // Load the EAP Types attributes into the vector.
         IASAttributeVectorWithBuffer<8> eapTypes;
         if (!eapTypes.load(request, IAS_ATTRIBUTE_NP_ALLOWED_EAP_TYPE))
         {
            IASTraceString("EAP not authorized for this user.");

            // Since we don't have an EAP-Type attribute, the user is not
            // allowed to use EAP.
            request.SetResponse(IAS_RESPONSE_ACCESS_REJECT,
                                IAS_INVALID_AUTH_TYPE);
            return IAS_REQUEST_STATUS_HANDLED;
         }

         //////////
         // Retrieve the provider for each EAP type.
         //////////

         // Fill the vector of providers
         std::vector<EAPType*> providers;
         providers.reserve(eapTypes.size());
         for (IASAttributeVector::iterator i = eapTypes.begin();
              i != eapTypes.end();
              ++i)
         {
            IASTracePrintf(
               "Allowed EAP type: %d",
               i->pAttribute->Value.Integer
               );

            EAPType* provider = theTypes[i->pAttribute->Value.Enumerator];
            if (!provider)
            {
               // We can't handle this EAP type. This is an internal error.
               // Lets try the other types.
               IASTraceString("Ignoring EAP type.");
            }
            else
            {
               providers.push_back(provider);
            }
         }

         // pass a vector of providers
         if (!providers.empty())
         {
            session = new EAPSession(identity, providers);
            _ASSERT(session);

            IASTracePrintf(
               "Successfully created new EAP session for user %S.",
               session->getAccountName()
               );

            retval = session->begin(request, recvPkt);
         }
         else
         {
            IASTraceString("No authorized EAP types could be loaded.");

            // Since we don't have an EAP-Type attribute, the user is not
            // allowed to use EAP.
            request.SetResponse(
                       IAS_RESPONSE_DISCARD_PACKET,
                       IAS_EAP_NEGOTIATION_FAILED
                       );
            return IAS_REQUEST_STATUS_ABORT;
         }
      }

      // Save it for later.
      sessions.insert(session);

      return retval;
   }
   catch (const _com_error& )
   {
      IASTraceExcept();
      pRequest->SetResponse(IAS_RESPONSE_DISCARD_PACKET, IAS_INTERNAL_ERROR);
   }
   catch (const std::bad_alloc&)
   {
      IASTraceExcept();
      pRequest->SetResponse(IAS_RESPONSE_DISCARD_PACKET, IAS_INTERNAL_ERROR);
   }

   // If we have any errors, we'll delete the session.
   delete session;

   return IAS_REQUEST_STATUS_ABORT;
}
