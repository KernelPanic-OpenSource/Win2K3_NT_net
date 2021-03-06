/*++

Copyright (c) 2001  Microsoft Corporation

Module Name:

    rmALG.cpp

Abstract:

    This module contains routines for the ALG Manager module's 
    private interface to be used only by the ALG.exe manager.

Author:

    JPDup		10-Nov-2000

Revision History:

--*/


#include "precomp.h"

#include <atlbase.h>

extern CComModule _Module;
#include <atlcom.h>

#include "Alg.h"
#include "NatPrivateAPI_Imp.h"

#include <MyTrace.h>

#include <Rtutils.h>




extern HANDLE                   AlgPortReservationHandle;   // see rmALG.CPP




/////////////////////////////////////////////////////////////////////////////
// CNat






//
// Standard destructor
//
CNat::~CNat(void)
{
    MYTRACE_ENTER("CNat::~CNat(void)");

    if ( m_hTranslatorHandle )
        NatShutdownTranslator(m_hTranslatorHandle);

    if ( m_pSidLocalService )
    {
        MYTRACE("Free m_pSidLocalService");
        LocalFree(m_pSidLocalService);
    }

    if ( m_pSidLocalSystem )
    {
        MYTRACE("Free m_pSidLocalSystem");
        LocalFree(m_pSidLocalSystem);
    }


}






bool 
GetSecuritySID(
    WELL_KNOWN_SID_TYPE WellKnownSidToCreate,
    PSID&               pSidToReturn
    )
/*++

Routine Description:

    Creates a SID from a specified well know SID

Arguments:

    WellKnownSidToCreate    - Specify the well know SID to create from
    pSidToReturn            - pointer that will be allocated and populated with the SID attributes

Return Value:

    bool                    - true is the SID was successfully created
                            - false one or more error occurred

Environment:

    The routine will allocate the side so the caller is responsible for doing a LocalFree on the returned pSidToReturn pointer

--*/
{
    MYTRACE_ENTER("CNat::GetSecuritySID()");


    //
    // If theasked SID is not already cached then create it
    //
    if ( NULL == pSidToReturn )
    {
        DWORD dwSizeOfSid = SECURITY_MAX_SID_SIZE;

        // Allocate enough memory for the largest possible SID.
        if ( !(pSidToReturn = LocalAlloc(LMEM_FIXED, dwSizeOfSid)) )
        {    
            MYTRACE_ERROR("LocalAlloc got get LocalService", GetLastError());

            return false;
        }

        BOOL bRet = CreateWellKnownSid(
            WellKnownSidToCreate,
            NULL,
            pSidToReturn,
            &dwSizeOfSid
            );

            
        if ( !bRet )
        {
            MYTRACE_ERROR("From CreateWellKnownSid(LocalService)", GetLastError());
            LocalFree(pSidToReturn);
            pSidToReturn = NULL;

            return false;
        }
    }

    return true;
}





bool
CNat::IsTokenPartOfWellKnowSid(
    HANDLE              pTokenToCheck,
    WELL_KNOWN_SID_TYPE WellKnownSidToCheckAgainst,
    PSID&               pSidToCache
    )
/*++

Routine Description:

    Helper function that create/gets a well know sid and verify that the supplied token is a member of that SID

Arguments:

    pTokenToCheck           - Token that will be inspected
    WellKnownSidToCreate    - Specify the well know SID to check against
    pSidToCache             - pointer to a SID that you will need to LocalFree when done

Return Value:

    bool                    - true if the supplied token is member of the WellKnowSID
                            - false one or more error occurred

Environment:

    The caller of the 

--*/
{
    MYTRACE_ENTER("IsTokenPartOfWellKnowSid()");

    BOOL bRet;

    m_AutoCS_SIDAllocation.Lock();

    bRet = GetSecuritySID(
        WellKnownSidToCheckAgainst,
        pSidToCache
        );

    m_AutoCS_SIDAllocation.Unlock();

    if ( !bRet )
    {
        MYTRACE("GetSecuritySID failed");
        return false;
    }

    BOOL bIsMember;

    bRet = CheckTokenMembership(
            pTokenToCheck,
            pSidToCache,
            &bIsMember
            );

    if ( !bRet )
    {
        MYTRACE_ERROR("Call to CheckTokenMembership failed", GetLastError());
        return false;
    }

    if ( FALSE == bIsMember )
    {
        MYTRACE("Token %d is not a member of SID %d", pTokenToCheck, pSidToCache);
        return false;
    }

    return true;
}


bool 
CNat::IsClientAllowedToCallUs()
/*++

Routine Description:

    Verify that the current caller is part of the LocalService or LocalSystem group

Arguments:

    none


Return Value:

    bool            - true  = Part of one or the group
                    - false = access should be denied

Environment:

    The routine runs will allocate 2 sid and they must be freed when the CNat object is released
    see member variables m_pSidLocalService and m_pSidLocalSystem;

--*/
{
    MYTRACE_ENTER("CNat::IsClientAllowedToCallUs()");


    HRESULT hr;

    hr = CoImpersonateClient();

    if ( FAILED(hr) )
    {
        MYTRACE_ERROR("CoImpersonateClient() failed", hr);
        return false;
    }

    HANDLE  hClientToken = NULL;

    bool bClientIsAllowedAccess = false;

    if ( OpenThreadToken(GetCurrentThread(), TOKEN_QUERY, TRUE, &hClientToken) )
    {
        //
        // Try with LocalService, this is the normal expected scenario
        // 
        bClientIsAllowedAccess = IsTokenPartOfWellKnowSid(
            hClientToken,
            WinLocalServiceSid,
            m_pSidLocalService
            );

        if ( bClientIsAllowedAccess )
        {
            MYTRACE("Client is member of the LOCAL_SERVICE group");
        }
        else
        {
            //
            // For debugging purpose sometime you need to set the ALG service to LocalSystem
            // in this case we need to grant access also to LocalSystem account
            //
            bClientIsAllowedAccess = IsTokenPartOfWellKnowSid(
                hClientToken,
                WinLocalSystemSid,
                m_pSidLocalSystem
                );

            if ( bClientIsAllowedAccess )
            {
                MYTRACE("Client is member of the LOCAL_SYSTEM group");
            }
            else
            {
                MYTRACE("Client is NOT member of LOCAL_SERVICE or LOCAL_SYSTEM group");
            }
        }

        //
        // No need for this handle anymore
        //
        CloseHandle( hClientToken );
    }
    else
    {
        MYTRACE_ERROR("Could not OpenThreadToken", GetLastError());
    }



    //
    // Set security context back 
    //
    hr = CoRevertToSelf();

    if ( FAILED(hr) )
    {
        MYTRACE_ERROR("from CoRevertToSelf()", hr);
        return  false;;
    }

    return bClientIsAllowedAccess;

}

#define RETURN_IF_CLIENT_NOT_LOCAL_SERVICE  if ( !IsClientAllowedToCallUs() ) { return E_ACCESSDENIED; };



STDMETHODIMP 
CNat::CreateRedirect(
    IN  ULONG       Flags, 
    IN  UCHAR       Protocol, 

    IN  ULONG       DestinationAddress, 
    IN  USHORT      DestinationPort, 

    IN  ULONG       SourceAddress, 
    IN  USHORT      SourcePort, 

    IN  ULONG       NewDestinationAddress, 
    IN  USHORT      NewDestinationPort, 

    IN  ULONG       NewSourceAddress, 
    IN  USHORT      NewSourcePort, 

    IN  ULONG       RestrictAdapterIndex, 

    IN  DWORD_PTR   dwAlgProcessId,
    IN  HANDLE_PTR  hCreateEvent, 
    IN  HANDLE_PTR  hDeleteEvent
    )
{
/*++

Routine Description:

    Creates a Redirect PORT

Arguments:

    Flags                   - Specifies options for the redirect
    Protocol                - IP protocol of the session to be redirected

    DestinationAddress      - destination endpoint of the session to be redirected
    DestinationPort         - "

    SourceAddress           - source endpoint of the session to be redirected
    SourcePort              - "

    NewDestinationAddress   - replacement destination endpoint for the session
    NewDestinationPort      - "

    NewSourceAddress        - replacement source endpoint for the session
    NewSourcePort           - "

    RestrictAdapterIndex    - optionally specifies the adapter index that this redirect should be restricted to 

    hCreateEvent            - optionally specifies an event to be signalled when a session matches the redirect.

    hDeleteEvent            - optionally specifies an event to be signalled when a session is delete.


Return Value:

    HRESULT                 - S_OK for success or and HRESULT error

Environment:

    The routine runs in the context of the ALG Manager and cant only be invoke by the ALG.EXE

--*/

    MYTRACE_ENTER("CNat::CreateRedirect");

    RETURN_IF_CLIENT_NOT_LOCAL_SERVICE;

    MYTRACE("ProtocolPublic %d, ProtocolInternal %d", Protocol, ProtocolConvertToNT(Protocol));
    MYTRACE("Destination    %s:%d", MYTRACE_IP(DestinationAddress),      ntohs(DestinationPort));
    MYTRACE("Source         %s:%d", MYTRACE_IP(SourceAddress),           ntohs(SourcePort));
    MYTRACE("NewDestination %s:%d", MYTRACE_IP(NewDestinationAddress),   ntohs(NewDestinationPort));
    MYTRACE("NewSource      %s:%d", MYTRACE_IP(NewSourceAddress),        ntohs(NewSourcePort));

    HANDLE  hThisEventForCreate=NULL;
    HANDLE  hThisEventForDelete=NULL;


    //
    // Duplicate the requested Event handles
    //
    if ( dwAlgProcessId )
    {

        HANDLE hAlgProcess = OpenProcess(
            PROCESS_DUP_HANDLE,     // access flag
            false,                  // handle inheritance option
            (DWORD)dwAlgProcessId   // process identifier
            );

        if ( !hAlgProcess )
        {
            MYTRACE_ERROR("Could not open the Process ID of ALG.exe", 0);
            return HRESULT_FROM_WIN32(GetLastError());
        }



        if ( hCreateEvent )
        {
        
            //
            // a create event was requested 
            //
            if ( !DuplicateHandle(
                    hAlgProcess,
                    (HANDLE)hCreateEvent,
                    GetCurrentProcess(),
                    &hThisEventForCreate,
                    0,
                    FALSE,
                    DUPLICATE_SAME_ACCESS
                    )
                )
            {
                MYTRACE_ERROR("DuplicateHandle on the CREATE handle", 0);
                CloseHandle(hAlgProcess);
                return HRESULT_FROM_WIN32(GetLastError());
            }

            MYTRACE("New DuplicateHandle 'CREATE'=%d base on=%d", hThisEventForCreate, hCreateEvent);
        }
        else
        {
            MYTRACE("No event for Creation requested");
        }



        if ( hDeleteEvent )
        {
            //
            // a delete event was requested
            //
            if ( !DuplicateHandle(
                    hAlgProcess,
                    (HANDLE)hDeleteEvent,
                    GetCurrentProcess(),
                    &hThisEventForDelete,
                    0,
                    FALSE,
                    DUPLICATE_SAME_ACCESS
                    )
                )
            {
                MYTRACE_ERROR("DuplicateHandle on the DELETE handle", 0);

                if ( hThisEventForCreate )
	            CloseHandle(hThisEventForCreate);

                CloseHandle(hAlgProcess);

                return HRESULT_FROM_WIN32(GetLastError());
            }

            MYTRACE("New DuplicateHandle 'DELETE'=%d base on=%d", hThisEventForDelete, hDeleteEvent);

        }
        else
        {
            MYTRACE("No event for Delete requested");
        }

        CloseHandle(hAlgProcess);
    }
    else
    {
        MYTRACE("NO EVENT Requested");
    }


    ULONG Error = NatCreateRedirectEx(
        GetTranslatorHandle(),
        Flags,
        ProtocolConvertToNT(Protocol),

        DestinationAddress,
        DestinationPort,

        SourceAddress,      
        SourcePort,

        NewDestinationAddress,
        NewDestinationPort,

        NewSourceAddress,
        NewSourcePort,

        RestrictAdapterIndex,
        IPNATAPI_SET_EVENT_ON_COMPLETION, // Special constant to use Event vs. a callback to a CompletionRoutine
        (PVOID)hThisEventForDelete,       //HANDLE for DELETE sessions
        (HANDLE)hThisEventForCreate       //HANDLE                    NotifyEvent         OPTIONAL
        ); 

    if ( hThisEventForCreate )
        CloseHandle(hThisEventForCreate);

    if ( hThisEventForDelete )
        CloseHandle(hThisEventForDelete);

    if ( ERROR_SUCCESS != Error )
    {
        MYTRACE_ERROR("From NatCreateRedirectEx", Error);
        return HRESULT_FROM_WIN32(Error);
    }

    return S_OK;
}


//
//
//

STDMETHODIMP 
CNat::CancelRedirect(
    IN  UCHAR    Protocol, 

    IN  ULONG    DestinationAddress, 
    IN  USHORT   DestinationPort, 

    IN  ULONG    SourceAddress, 
    IN  USHORT   SourcePort, 

    IN  ULONG    NewDestinationAddress, 
    IN  USHORT   NewDestinationPort, 

    IN  ULONG    NewSourceAddress, 
    IN  USHORT   NewSourcePort
    )
/*++

Routine Description:

    Cancel a Redirect

Arguments:

    Protocol                - IP protocol of the session to be redirected eALG_TCP || eALG_UDP

    DestinationAddress      - destination endpoint of the session to be redirected
    DestinationPort         - "

    SourceAddress           - source endpoint of the session to be redirected
    SourcePort              - "

    NewDestinationAddress   - replacement destination endpoint for the session
    NewDestinationPort      - "

    NewSourceAddress        - replacement source endpoint for the session
    NewSourcePort           - "


Return Value:

    HRESULT                 - S_OK for success or and HRESULT error

Environment:

    The routine runs in the context of the ALG Manager and cant only be invoke by the ALG.EXE

--*/
{
    MYTRACE_ENTER("CNat::CancelRedirect");

    RETURN_IF_CLIENT_NOT_LOCAL_SERVICE

    MYTRACE("Protocol Public %d, Internal %d", Protocol, ProtocolConvertToNT(Protocol));
    MYTRACE("Destination    %s:%d", MYTRACE_IP(DestinationAddress),     ntohs(DestinationPort));
    MYTRACE("Source         %s:%d", MYTRACE_IP(SourceAddress),          ntohs(SourcePort));
    MYTRACE("NewDestination %s:%d", MYTRACE_IP(NewDestinationAddress),  ntohs(NewDestinationPort));
    MYTRACE("NewSource      %s:%d", MYTRACE_IP(NewSourceAddress),       ntohs(NewSourcePort));

    ULONG Error = NatCancelRedirect(
        GetTranslatorHandle(),
        ProtocolConvertToNT(Protocol),
        DestinationAddress,
        DestinationPort,
        SourceAddress,
        SourcePort,
        NewDestinationAddress,
        NewDestinationPort,
        NewSourceAddress,
        NewSourcePort
        );

    if ( ERROR_SUCCESS != Error )
    {
        MYTRACE_ERROR("From NatCancelRedirect", Error);
        return HRESULT_FROM_WIN32(Error);
    }

    return S_OK;
}






STDMETHODIMP 
CNat::CreateDynamicRedirect(
    IN  ULONG       Flags, 
    IN  ULONG       nAdapterIndex,
    IN  UCHAR       Protocol, 

    IN  ULONG       DestinationAddress, 
    IN  USHORT      DestinationPort, 

    IN  ULONG       SourceAddress, 
    IN  USHORT      SourcePort, 

    IN  ULONG       NewDestinationAddress, 
    IN  USHORT      NewDestinationPort, 

    IN  ULONG       NewSourceAddress,
    IN  USHORT      NewSourcePort,

    OUT HANDLE_PTR* pDynamicRedirectHandle
    )
/*++

Routine Description:

    Cancel a dynamic Redirect, by seting up a dynamic redirection any time a adapter is created the redirection will be
    applied to that new adapter.

Arguments:

    Flags                   - Specifies options for the redirect
    nAdapterIndex           - Index of the IP adapter (Same as the index found using the cmd line "ROUTE PRINT")
    Protocol                - IP protocol of the session to be redirected

    DestinationAddress      - destination endpoint of the session to be redirected
    DestinationPort         - "

    SourceAddress           - source endpoint of the session to be redirected
    SourcePort              - "

    NewDestinationAddress   - replacement destination endpoint for the session
    NewDestinationPort      - "

    NewSourceAddress        - replacement source endpoint for the session
    NewSourcePort           - "

    pDynamicRedirectHandle  - This routine will populate this field with the handle (Cookie) for the purpose of canceling
                              this DynamicRedirect

Return Value:

    HRESULT                 - S_OK for success or and HRESULT error

Environment:

    The routine runs in the context of the ALG Manager and cant only be invoke by the ALG.EXE
    and is use via the public api CreatePrimaryControlChannel (See ALG.EXE)

--*/
{
    MYTRACE_ENTER("CNat::CreateDynamicRedirect");

    RETURN_IF_CLIENT_NOT_LOCAL_SERVICE;


    ASSERT(pDynamicRedirectHandle!=NULL);

#if defined(DBG) || defined(_DEBUG)

    MYTRACE("Flags          %d", Flags);


    MYTRACE("Protocol Public %d Internal %d", Protocol, ProtocolConvertToNT(Protocol));

    if ( Flags & NatRedirectFlagNoTimeout )
        MYTRACE("    NatRedirectFlagNoTimeout");

    if ( Flags & NatRedirectFlagUnidirectional )
        MYTRACE("    NatRedirectFlagUnidirectional");

    if ( Flags & NatRedirectFlagRestrictSource )
        MYTRACE("    NatRedirectFlagRestrictSource");

    if ( Flags & NatRedirectFlagPortRedirect )
        MYTRACE("    NatRedirectFlagPortRedirect");

    if ( Flags & NatRedirectFlagReceiveOnly )
        MYTRACE("    NatRedirectFlagReceiveOnly");

    if ( Flags & NatRedirectFlagLoopback )
        MYTRACE("    NatRedirectFlagLoopback");

    if ( Flags & NatRedirectFlagSendOnly )
        MYTRACE("    NatRedirectFlagSendOnly");

    if ( Flags & NatRedirectFlagRestrictAdapter )
        MYTRACE("    NatRedirectFlagRestrictAdapter");

    if ( Flags & NatRedirectFlagSourceRedirect )
        MYTRACE("    NatRedirectFlagSourceRedirect");


    MYTRACE("AdapterIndex   %d", nAdapterIndex);
    
    in_addr tmpAddr;
    tmpAddr.s_addr = DestinationAddress;
    MYTRACE("Destination    %s:%d", inet_ntoa(tmpAddr),    ntohs(DestinationPort));
    tmpAddr.s_addr = SourceAddress;
    MYTRACE("Source         %s:%d", inet_ntoa(tmpAddr),    ntohs(SourcePort));
    tmpAddr.s_addr = NewDestinationAddress;
    MYTRACE("NewDestination %s:%d", inet_ntoa(tmpAddr),    ntohs(NewDestinationPort));
    tmpAddr.s_addr = NewSourceAddress;
    MYTRACE("NewSource      %s:%d", inet_ntoa(tmpAddr),    ntohs(NewSourcePort));
#endif





    MYTRACE("About to call NatCreateDynamicFullRedirect");

    ULONG nRestrictSourceAddress = 0;

    if ( NatRedirectFlagRestrictSource & Flags )
    {
        MYTRACE("NatRedirectFlagRestrictSource flags is set");
        nRestrictSourceAddress = SourceAddress;
        SourceAddress = 0;
    }

    ULONG Error = NatCreateDynamicFullRedirect(
        Flags|NatRedirectFlagLoopback,
        ProtocolConvertToNT(Protocol),

        DestinationAddress,
        DestinationPort,

        SourceAddress,
        SourcePort,

        NewDestinationAddress,
        NewDestinationPort,

        NewSourceAddress,
        NewSourcePort,

        nRestrictSourceAddress,         //ULONG RestrictSourceAddress OPTIONAL,
        nAdapterIndex,                  //ULONG RestrictAdapterIndex OPTIONAL,
        0,                              //MinimumBacklog OPTIONAL,
        (PHANDLE)pDynamicRedirectHandle
        );

    if ( ERROR_SUCCESS != Error )
    {
        MYTRACE_ERROR("Failed NatCreateDynamicFullRedirect", Error);
        return HRESULT_FROM_WIN32(Error);
    }

    //
    // Cache the DynamicRedirect cookie before we hand it out to the caller
    // this list of cache cookie will be used to validate when a caller ask 
    // us to Cancel a redirect
    //
    m_AutoCS_DynamicRedirect.Lock();
    BOOL bAddedToListOFOutstandingRedirectHandle = m_ListOfOutstandingRedirects.Add(*pDynamicRedirectHandle);
    m_AutoCS_DynamicRedirect.Unlock();

    if ( bAddedToListOFOutstandingRedirectHandle )
    {
        MYTRACE("Added %d to the list of outstanding redirects for a total of %d redirects", *pDynamicRedirectHandle, m_ListOfOutstandingRedirects.GetSize());
    }
    else
    {
        MYTRACE_ERROR("Could add to list of Outstanding Redirect handle", GetLastError());
    }


    MYTRACE("Call to NatCreateDynamicFullRedirect worked");

    return S_OK;;
}




STDMETHODIMP 
CNat::CancelDynamicRedirect(
    IN  HANDLE_PTR DynamicRedirectHandle
    )
/*++

Routine Description:

    This routine is called to cancel the given dynamic redirect.
    by calling the NatApi version of this function

Arguments:

    DynamicRedirectHandle   - the handle to the dynamic redirect to be cancelled

Return Value:

    HRESULT                 - S_OK for success or and HRESULT error

--*/

{
    MYTRACE_ENTER("CNat::CancelDynamicRedirect");

    RETURN_IF_CLIENT_NOT_LOCAL_SERVICE;


    //
    // Is the caller passing a valid DynamicRedirect handle 
    //
    m_AutoCS_DynamicRedirect.Lock();
    MYTRACE("Remove %d from the outstanding list of redirects, the before size is %d", DynamicRedirectHandle, m_ListOfOutstandingRedirects.GetSize());
    BOOL bFoundAndRemoved = m_ListOfOutstandingRedirects.Remove(DynamicRedirectHandle);
    m_AutoCS_DynamicRedirect.Unlock();

    if ( bFoundAndRemoved )
    {
        MYTRACE("Removed succesfull");
    }
    else
    {
        MYTRACE_ERROR("pDynamicRedirectHandle is not valid", GetLastError());
        return E_INVALIDARG;
    }


    //
    // We are good to go release this redirect
    //
    ULONG Error = NatCancelDynamicRedirect((PHANDLE)DynamicRedirectHandle);

    if ( ERROR_SUCCESS != Error )
    {
        MYTRACE_ERROR("Failed NatCancelDynamicRedirect", Error);
        return HRESULT_FROM_WIN32(Error);
    }

    return S_OK;
}




STDMETHODIMP 
CNat::GetBestSourceAddressForDestinationAddress(
    IN  ULONG       ulDestinationAddress, 
    IN  BOOL        fDemandDial, 
    OUT ULONG*      pulBestSrcAddress
    )
/*++

Routine Description:

    We create a temporary UDP socket, connect the socket to the
    actual client's IP address, extract the IP address to which
    the socket is implicitly bound by the TCP/IP driver, and
    discard the socket. This leaves us with the exact IP address
    that we need to use to contact the client.

Arguments:

    ulDestinationAddress, 
    fDemandDial, 
    pulBestSrcAddress


Return Value:

    HRESULT             - S_OK for success

Environment:

    ALG module will call this method to:

--*/

{
    MYTRACE_ENTER("CNat::GetBestSourceAddressForDestinationAddress");

    RETURN_IF_CLIENT_NOT_LOCAL_SERVICE;

    if ( !pulBestSrcAddress )
    {
        MYTRACE_ERROR("pulBestSrcAddress not supplied",0);
        return E_INVALIDARG;
    }


    SOCKADDR_IN SockAddr;

    SockAddr.sin_family         = AF_INET;
    SockAddr.sin_port           = 0;
    SockAddr.sin_addr.s_addr    = ulDestinationAddress;

    
    ULONG Length = sizeof(SockAddr);


    SOCKET UdpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if  (   INVALID_SOCKET == UdpSocket
        ||  SOCKET_ERROR   == connect(UdpSocket, (PSOCKADDR)&SockAddr, sizeof(SockAddr))
        ||  SOCKET_ERROR   == getsockname(UdpSocket, (PSOCKADDR)&SockAddr, (int*)&Length)
        )   
    {
        ULONG nError = WSAGetLastError();

        if ( nError == WSAEHOSTUNREACH )
        {
            if ( fDemandDial )
                nError = RasAutoDialSharedConnection(); 

            if ( ERROR_SUCCESS != nError ) 
            {
                MYTRACE_ERROR(" RasAutoDialSharedConnection failed [%d]", nError);

                if ( UdpSocket != INVALID_SOCKET ) 
                { 
                    closesocket(UdpSocket); 
                }

                return HRESULT_FROM_WIN32(nError);
            }
        } 
        else 
        {
            MYTRACE_ERROR("error %d routing endpoint %d using UDP", nError);

            if (UdpSocket != INVALID_SOCKET) 
            { 
                closesocket(UdpSocket); 
            }

            return HRESULT_FROM_WIN32(nError);
        }
    }

    *pulBestSrcAddress = SockAddr.sin_addr.s_addr;

    closesocket(UdpSocket);

    return S_OK;
}



STDMETHODIMP CNat::LookupAdapterPortMapping(
    IN  ULONG   ulAdapterIndex,
    IN  UCHAR   Protocol,
    IN  ULONG   ulDestinationAddress,
    IN  USHORT  usDestinationPort,
    OUT ULONG*  pulRemapAddress,
    OUT USHORT* pusRemapPort
    )
/*++

Routine Description:

    Call NAT port maping to ge the real destination for the port
    This ofcourse is the use has set some maping in the SharedConnection or Firewalled adapter on the Service Tab.

Arguments:

    ulAdapterIndex          - Index of the IP adapter of the session.

    Protocol                - eALG_PROTOCOL_UDP, eALG_PROTOCOL_TCP
    DestinationAddress      - the edge public adapter address
    DestinationPort         - the edge public adapter port

    RemapAddres             - The address where that the user itended this port to go to (Private computer on the private lan)
    SourcePort              - Should be the same as the DestinationPort for future it may be different.


Return Value:

    HRESULT - S_OK if it worked or E_FAIL if no maping was found

--*/
{
    MYTRACE_ENTER("LookupAdapterPortMapping");

    RETURN_IF_CLIENT_NOT_LOCAL_SERVICE;

    MYTRACE("AdapterIndex %d Protocol %d DestAddress %s:%d", ulAdapterIndex, ProtocolConvertToNT(Protocol), MYTRACE_IP(ulDestinationAddress), ntohs(usDestinationPort));

    IP_NAT_PORT_MAPPING PortMapping;

    ULONG Error = NatLookupPortMappingAdapter(
                    ulAdapterIndex,
                    ProtocolConvertToNT(Protocol),
                    ulDestinationAddress,
                    usDestinationPort,
                    &PortMapping
                    );
                        
    if ( Error ) 
    {
        MYTRACE_ERROR("from NatLookupPortMappingAdapter", Error);
        return HRESULT_FROM_WIN32(Error);
    }

    *pulRemapAddress   = PortMapping.PrivateAddress;
    *pusRemapPort      = PortMapping.PrivatePort;

    return S_OK;
}




STDMETHODIMP CNat::GetOriginalDestinationInformation(
    IN  UCHAR   Protocol,

    IN  ULONG   ulDestinationAddress,
    IN  USHORT  usDestinationPort,

    IN  ULONG   ulSourceAddress,
    IN  USHORT  usSourcePort,

    OUT ULONG*  pulOriginalDestinationAddress,
    OUT USHORT* pusOriginalDestinationPort,

    OUT ULONG*  pulAdapterIndex
    )
/*++

Routine Description:

    Determine the original destination endpoint of a session that is redirected to.

Arguments:


    DestinationAddress      - destination endpoint of the session to be redirected
    DestinationPort         - "

    SourceAddress           - source endpoint of the session to be redirected
    SourcePort              - "

    NewDestinationAddress   - replacement destination endpoint for the session
    NewDestinationPort      - "

    NewSourceAddress        - replacement source endpoint for the session
    NewSourcePort           - "

    pulOriginalDestinationAddress   - Returns the original address of the destination (Where the caller realy wanted to go)
    pusOriginalDestinationPort      - Returns the original port of the destination

    pulAdapterIndex                 - Index of the IP adapter of the session.

Return Value:

    HRESULT - S_OK if it worked or E_FAIL

--*/
{
    MYTRACE_ENTER("CNat::GetOriginalDestinationInformation");

    RETURN_IF_CLIENT_NOT_LOCAL_SERVICE;

    MYTRACE("Destination  %s:%d", MYTRACE_IP(ulDestinationAddress), ntohs(usDestinationPort));
    MYTRACE("Address      %s:%d", MYTRACE_IP(ulSourceAddress),      ntohs(usSourcePort));

    ASSERT(pulOriginalDestinationAddress!=NULL);
    ASSERT(pusOriginalDestinationPort!=NULL);
    ASSERT(pulAdapterIndex!=NULL);


    IP_NAT_SESSION_MAPPING_KEY_EX  Information;
    ULONG   ulSizeOfInformation = sizeof(IP_NAT_SESSION_MAPPING_KEY_EX);

    ULONG Error = NatLookupAndQueryInformationSessionMapping(
        GetTranslatorHandle(),
        ProtocolConvertToNT(Protocol),

        ulDestinationAddress,
        usDestinationPort,

        ulSourceAddress,
        usSourcePort,

        &Information,
        &ulSizeOfInformation,
        NatKeySessionMappingExInformation
        );


    if ( ERROR_SUCCESS != Error )
    {
        MYTRACE_ERROR("Call to NatLookupAndQueryInformationMapping", Error);
        return HRESULT_FROM_WIN32(Error);
    }

    MYTRACE("Original Index %d Address:Port %s:%d", Information.AdapterIndex, MYTRACE_IP(Information.DestinationAddress), ntohs(Information.DestinationPort));

    *pulOriginalDestinationAddress  = Information.DestinationAddress;
    *pusOriginalDestinationPort     = Information.DestinationPort;
    *pulAdapterIndex                = Information.AdapterIndex;

    return S_OK;

}





STDMETHODIMP CNat::ReservePort(
    IN  USHORT      PortCount, 
    OUT PUSHORT     pReservedPortBase
    )
/*++

Routine Description:

    Call the into the NAP api to reserve the required port on behave of the ALG module.

Arguments:

    PortCount           -   Number of port to reserve
    pReservedPortBase   -   Starting number of the range of port reserved. example  ReserePort(3, &) would save 5000,5001,5002 and return 5000 as base

Return Value:

    HRESULT - S_OK if it worked or E_FAIL


Environment:

    Private interface between rmALG and ALG.EXE

    ALG expose a more simple interface to reserve at Port
    in turn it call this private interface that end up calling the more complex NatApi

--*/
{
    MYTRACE_ENTER("CNat::ReservePort");

    RETURN_IF_CLIENT_NOT_LOCAL_SERVICE;

    ASSERT(pReservedPortBase!=NULL);

    if ( !AlgPortReservationHandle )
        return E_FAIL;                  // AlgPortReservationHandle should already have been done


    ULONG Error = NatAcquirePortReservation(
        AlgPortReservationHandle,
        PortCount,
        pReservedPortBase
        );

    if ( ERROR_SUCCESS != Error )
    {
        MYTRACE_ERROR("from NatAcquirePortReservation", Error);
        return HRESULT_FROM_WIN32(Error);
    }

    MYTRACE("PortBase %d count %d", *pReservedPortBase, PortCount);

    return S_OK;
}





STDMETHODIMP CNat::ReleasePort(
    IN  USHORT  ReservedPortBase, 
    IN  USHORT  PortCount
    )
/*++

Routine Description:

    Private interface between rmALG and ALG.EXE

    ALG expose a more simple interface to reserve at Port
    in turn it call this private interface that end up calling the more complex NatApi

    This routine will call the Nat api to release the previously reserved ports

Arguments:

    PortCount           -   Number of port to reserve
    pReservedPortBase   -   Starting number of the range of port reserved. example  ReserePort(3, &) would save 5000,5001,5002 and return 5000 as base

Return Value:

    HRESULT - S_OK if it worked or E_FAIL

Environment:

    Private interface between rmALG and ALG.EXE

    ALG expose a more simple interface to reserve at Port
    in turn it call this private interface that end up calling the more complex NatApi

--*/
{

    MYTRACE_ENTER("CNat::ReleasePort");    

    RETURN_IF_CLIENT_NOT_LOCAL_SERVICE;

    if ( !AlgPortReservationHandle )
        return E_FAIL;                  // AlgPortReservationHandle should already have been done

    ULONG Error = NatReleasePortReservation(
        AlgPortReservationHandle,
        ReservedPortBase,
        PortCount
        );

    if ( ERROR_SUCCESS != Error )
    {
        MYTRACE_ERROR("from NatReleasePortReservation", Error);
        return HRESULT_FROM_WIN32(Error);
    }

    MYTRACE("PortBase=%d, Count=%d", ntohs(ReservedPortBase), PortCount);

    return S_OK;
}

