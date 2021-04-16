///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) Microsoft Corporation.
//
// SYNOPSIS
//
//    Declares the class Request.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef REQUEST_H
#define REQUEST_H

#include "resource.h"
#include <iaspolcy.h>
#include <sdoias.h>

///////////////////////////////////////////////////////////////////////////////
//
// CLASS
//
//    Request
//
///////////////////////////////////////////////////////////////////////////////
class ATL_NO_VTABLE Request :
   public CComObjectRootEx<CComMultiThreadModelNoCS>,
   public CComCoClass<Request, &__uuidof(Request)>,
   public IRequest,
   public IAttributesRaw,
   public IRequestState
{
public:

DECLARE_REGISTRY_RESOURCEID(IDR_Registry)

DECLARE_NOT_AGGREGATABLE(Request)

BEGIN_COM_MAP(Request)
   COM_INTERFACE_ENTRY_IID(__uuidof(IAttributesRaw), IAttributesRaw)
   COM_INTERFACE_ENTRY_IID(__uuidof(Request), Request)
   COM_INTERFACE_ENTRY_IID(__uuidof(IRequestState), IRequestState)
   COM_INTERFACE_ENTRY_IID(__uuidof(IRequest), IRequest)
END_COM_MAP()

   /////////
   // Methods used by the Pipeline for routing requests.
   /////////

   IASREQUEST getRequest() const throw ()
   { return request; }

   IASPROVIDER getProvider() const throw ()
   {
      PIASATTRIBUTE attr = findFirst(IAS_ATTRIBUTE_PROVIDER_TYPE);
      return attr ? (IASPROVIDER)attr->Value.Enumerator : IAS_PROVIDER_NONE;
   }


   IASRESPONSE getResponse() const throw ()
   { return response; }

   IASREASON getReason() const throw ()
   { return reason; }

   void pushState(ULONG64 state) throw ()
   { *topOfStack++ = state; }

   ULONG64 popState() throw ()
   { return *--topOfStack; }

   void pushSource(IRequestSource* newVal) throw ()
   { pushState((ULONG64)source); source = newVal; }

   void popSource() throw ()
   { source = (IRequestSource*)popState(); }

   IASREQUEST getRoutingType() const throw ()
   { return routing; }
   void setRoutingType(IASREQUEST newVal) throw ()
   { routing = newVal; }

   PIASATTRIBUTE findFirst(DWORD id) const throw ();

   static Request* narrow(IUnknown* pUnk) throw ();

   /////////
   // IRequest
   /////////

   STDMETHOD(get_Source)(IRequestSource** pVal);
   STDMETHOD(put_Source)(IRequestSource* newVal);
   STDMETHOD(get_Protocol)(IASPROTOCOL *pVal);
   STDMETHOD(put_Protocol)(IASPROTOCOL newVal);
   STDMETHOD(get_Request)(LONG *pVal);
   STDMETHOD(put_Request)(LONG newVal);
   STDMETHOD(get_Response)(LONG *pVal);
   STDMETHOD(get_Reason)(LONG *pVal);
   STDMETHOD(SetResponse)(IASRESPONSE eResponse, LONG lReason);
   STDMETHOD(ReturnToSource)(IASREQUESTSTATUS eStatus);

   /////////
   // IAttributesRaw
   /////////

   STDMETHOD(AddAttributes)(
                 DWORD dwPosCount,
                 PATTRIBUTEPOSITION pPositions
                 );
   STDMETHOD(RemoveAttributes)(
                 DWORD dwPosCount,
                 PATTRIBUTEPOSITION pPositions
                 );
   STDMETHOD(RemoveAttributesByType)(
                 DWORD dwAttrIDCount,
                 DWORD *lpdwAttrIDs
                 );
   STDMETHOD(GetAttributeCount)(
                 DWORD *lpdwCount
                 );
   STDMETHOD(GetAttributes)(
                 DWORD *lpdwPosCount,
                 PATTRIBUTEPOSITION pPositions,
                 DWORD dwAttrIDCount,
                 DWORD *lpdwAttrIDs
                 );
   STDMETHOD(InsertBefore)(
                PATTRIBUTEPOSITION newAttr,
                PATTRIBUTEPOSITION refAttr
                );

   /////////
   // IRequestState
   /////////

   STDMETHOD(Push)(
                 ULONG64 state
                 );
   STDMETHOD(Pop)(
                 ULONG64 *pState
                 );
   STDMETHOD(Top)(
                 ULONG64 *pState
                 );

protected:
   Request() throw ();
   ~Request() throw ();

private:
   // Returns the number of attributes in the request.
   size_t size() const throw ();

   // Resizes the attribute array as necessary to ensure it has room for at
   // least newCapacity elements. Returns true if successful, false otherwise.
   bool reserve(size_t newCapacity) throw ();

   // Returns the position of the specified attribute in the array or null if
   // the attribute doesn't exist.
   PIASATTRIBUTE* find(IASATTRIBUTE* key) const throw ();

   // Properties.
   IRequestSource* source;
   IASPROTOCOL protocol;
   IASREQUEST request;
   IASRESPONSE response;
   IASREASON reason;

   // More specific request type used for routing.
   IASREQUEST routing;

   // Attribute collection.
   PIASATTRIBUTE* begin;
   PIASATTRIBUTE* end;
   size_t capacity;

   // State stack.
   ULONG64 state[3];
   PULONG64 topOfStack;

   // Not implemented.
   Request(const Request&) throw ();
   Request& operator=(const Request&) throw ();
};

#endif // REQUEST_H
