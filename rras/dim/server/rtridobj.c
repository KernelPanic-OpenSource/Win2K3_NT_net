/********************************************************************/
/**          Copyright(c) 1985-1997 Microsoft Corporation.         **/
/********************************************************************/

//***
//
// Filename:    rtridobj
//
// Description: Support routines to manipulate router information in the
//              router object
//
// History:     Feb 11,1998         NarenG              Created original version.
//

#include "dimsvcp.h"

#include <activeds.h>
#include <adsi.h>
#include <ntdsapi.h>
#include <dsgetdc.h>
#include <lmapibuf.h>
#define SECURITY_WIN32
#include <security.h>
#include <routprot.h>
#include <rtinfo.h>

#include <dimsvc.h>     // Generated by MIDL

#define ROUTER_IDENTITY_OBJECT_NAME    TEXT("CN=RouterIdentity")
#define ROUTER_OBJECT_ATTRIBUTE_NAME   TEXT("MsRRASAttribute")
#define ROUTER_LDAP_PREFIX             TEXT("LDAP://")
#define ROUTER_CN_COMMA                TEXT(",")
#define ROUTER_IDENTITY_CLASS          TEXT("RRASAdministrationConnectionPoint")

LPWSTR RouterObjectAttributeNames[] =
{
    ROUTER_OBJECT_ATTRIBUTE_NAME
};

//**
//
// Call:        RouterIdentityObjectOpen
//
// Returns:     NO_ERROR         - Success
//              Non-zero returns - Failure
//
// Description: Given the machine name of the router, will return the handle
//              to the router's administration service point or router object.
//
DWORD
RouterIdentityObjectOpen(
    IN  LPWSTR      lpwszRouterName,
    IN  DWORD       dwRouterType,
    OUT HANDLE *    phObjectRouterIdentity
)
{
    DWORD                               dwRetCode;
    LPWSTR                              lpwszRouterIdentityObjectPath = NULL;
    LPWSTR                              lpwszComputerObjectPath       = NULL;
    DOMAIN_CONTROLLER_INFO *            pDomainControllerInfo         = NULL;
    HRESULT                             hResult  = HRESULT_FROM_WIN32(NO_ERROR);
    DWORD                               dwCharCount;

    do
    {
        dwRetCode = DsGetDcName( NULL,
                                 NULL,
                                 NULL,
                                 NULL,   
                                 DS_DIRECTORY_SERVICE_REQUIRED |
                                 DS_WRITABLE_REQUIRED,
                                 &pDomainControllerInfo );

        if ( dwRetCode != NO_ERROR )
        {
            TracePrintfExA( gblDIMConfigInfo.dwTraceId, 
                            TRACE_DIM, "No DS located, DsGetDcName()=%d",
                            dwRetCode );
            break;
        }

        if ( !( pDomainControllerInfo->Flags & DS_DS_FLAG ) )
        {
            TracePrintfExA( gblDIMConfigInfo.dwTraceId, 
                            TRACE_DIM, "No DS located");

            dwRetCode = ERROR_DOMAIN_CONTROLLER_NOT_FOUND;

            break;
        }

        //
        // Get the CN of the router object
        //

        dwCharCount = 200;

        lpwszComputerObjectPath = LOCAL_ALLOC(LPTR, dwCharCount*sizeof(WCHAR));
        if (lpwszComputerObjectPath == NULL)
        {
            TracePrintfExA( gblDIMConfigInfo.dwTraceId, 
                            TRACE_DIM, "Memory exhausted -- unable to continue");

            dwRetCode = ERROR_NOT_ENOUGH_MEMORY;

            break;
        }

        if ( !GetComputerObjectName( NameFullyQualifiedDN,
                                     lpwszComputerObjectPath,
                                     &dwCharCount ) )
        {
            //
            // We failed for some other reason
            //

            LPWSTR lpwsComputerObjectPathReAlloc = 
                            LOCAL_REALLOC( lpwszComputerObjectPath,
                                          (++dwCharCount)*sizeof(WCHAR) );

            if ( lpwsComputerObjectPathReAlloc == NULL )
            {
                dwRetCode = ERROR_NOT_ENOUGH_MEMORY;

                break;
            }

            lpwszComputerObjectPath = lpwsComputerObjectPathReAlloc;

            if ( !GetComputerObjectName( NameFullyQualifiedDN,
                                         lpwszComputerObjectPath,
                                         &dwCharCount ) )
            {
                dwRetCode = GetLastError();

                break;
            }
        }

        lpwszRouterIdentityObjectPath =
            LOCAL_ALLOC( LPTR,
                        sizeof( ROUTER_LDAP_PREFIX )            +
                        sizeof( ROUTER_IDENTITY_OBJECT_NAME )   +
                        sizeof( ROUTER_CN_COMMA )               +
                        ((wcslen( lpwszComputerObjectPath )+1)* sizeof(WCHAR)));

        if ( lpwszRouterIdentityObjectPath == NULL )
        {
            dwRetCode = ERROR_NOT_ENOUGH_MEMORY;
            break;
        }

        wcscpy( lpwszRouterIdentityObjectPath, ROUTER_LDAP_PREFIX );
        wcscat( lpwszRouterIdentityObjectPath, ROUTER_IDENTITY_OBJECT_NAME );
        wcscat( lpwszRouterIdentityObjectPath, ROUTER_CN_COMMA );
        wcscat( lpwszRouterIdentityObjectPath, lpwszComputerObjectPath );

        //
        // Try to open the router identity object
        //

        hResult =  ADSIOpenDSObject( lpwszRouterIdentityObjectPath,
                                     NULL,
                                     NULL,
                                     0,
                                     phObjectRouterIdentity );

        if ( hResult == HRESULT_FROM_WIN32( ERROR_DS_NO_SUCH_OBJECT ) )
        {
            HANDLE          hObjectComputer;
            ADS_ATTR_INFO   AttributeEntries[2];
            ADSVALUE        ObjectClassAttributeValue;
            ADSVALUE        msRRASAttributeValues[3];
            WCHAR           wchmsRRASAttributeValue1[50];
            WCHAR           wchmsRRASAttributeValue2[50];
            WCHAR           wchmsRRASAttributeValue3[50];
            DWORD           dwIndex = 0;

            //
            // If we failed because it doesn't exist, then create it
            //

            wcscpy( lpwszRouterIdentityObjectPath, ROUTER_LDAP_PREFIX );
            wcscat( lpwszRouterIdentityObjectPath, lpwszComputerObjectPath );

            hResult =  ADSIOpenDSObject(
                                     lpwszRouterIdentityObjectPath,
                                     NULL,
                                     NULL,
                                     0,
                                     &hObjectComputer );

            if ( FAILED( hResult ) )
            {
                dwRetCode = HRESULT_CODE( hResult );

                break;
            }

            //
            // Set up attributes for this object
            //

            ObjectClassAttributeValue.dwType = ADSTYPE_CASE_IGNORE_STRING;
            ObjectClassAttributeValue.CaseIgnoreString = ROUTER_IDENTITY_CLASS;

            AttributeEntries[0].pszAttrName     = TEXT("ObjectClass");
            AttributeEntries[0].dwControlCode   = ADS_ATTR_APPEND;
            AttributeEntries[0].dwADsType       = ADSTYPE_CASE_IGNORE_STRING;
            AttributeEntries[0].pADsValues      = &ObjectClassAttributeValue;
            AttributeEntries[0].dwNumValues     = 1;

            if ( dwRouterType & ROUTER_ROLE_RAS )
            {
                wsprintf( wchmsRRASAttributeValue1,
                          TEXT("%d:%d:%d"),
                          DIM_MS_VENDOR_ID,
                          6,
                          602 );

                msRRASAttributeValues[dwIndex].dwType =
                                                    ADSTYPE_CASE_IGNORE_STRING;
                msRRASAttributeValues[dwIndex].CaseIgnoreString =
                                                    wchmsRRASAttributeValue1;
                dwIndex++;
            }

            if ( dwRouterType & ROUTER_ROLE_LAN )
            {
                wsprintf( wchmsRRASAttributeValue2,
                          TEXT("%d:%d:%d"),
                          DIM_MS_VENDOR_ID,
                          6,
                          601 );

                msRRASAttributeValues[dwIndex].dwType =
                                                   ADSTYPE_CASE_IGNORE_STRING;
                msRRASAttributeValues[dwIndex].CaseIgnoreString =
                                                   wchmsRRASAttributeValue2;
                dwIndex++;
            }

            if ( dwRouterType & ROUTER_ROLE_WAN )
            {
                wsprintf( wchmsRRASAttributeValue3,
                          TEXT("%d:%d:%d"),
                          DIM_MS_VENDOR_ID,
                          6,
                          603 );

                msRRASAttributeValues[dwIndex].dwType =
                                                   ADSTYPE_CASE_IGNORE_STRING;
                msRRASAttributeValues[dwIndex].CaseIgnoreString =
                                                   wchmsRRASAttributeValue3;

                dwIndex++;
            }

            AttributeEntries[1].pszAttrName     = ROUTER_OBJECT_ATTRIBUTE_NAME;
            AttributeEntries[1].dwControlCode   = ADS_ATTR_APPEND;
            AttributeEntries[1].dwADsType       = ADSTYPE_CASE_IGNORE_STRING;
            AttributeEntries[1].pADsValues      = msRRASAttributeValues;
            AttributeEntries[1].dwNumValues     = dwIndex;

            hResult =  ADSICreateDSObject(
                                    hObjectComputer,
                                    ROUTER_IDENTITY_OBJECT_NAME,
                                    AttributeEntries,
                                    2 );

            ADSICloseDSObject( hObjectComputer );

            if ( FAILED( hResult ) )
            {
                dwRetCode = HRESULT_CODE( hResult );

                break;
            }

            wcscpy(lpwszRouterIdentityObjectPath, ROUTER_LDAP_PREFIX);
            wcscat(lpwszRouterIdentityObjectPath, ROUTER_IDENTITY_OBJECT_NAME);
            wcscat(lpwszRouterIdentityObjectPath, ROUTER_CN_COMMA );
            wcscat(lpwszRouterIdentityObjectPath, lpwszComputerObjectPath);

            //
            // Now open it to get the handle
            //

            hResult =  ADSIOpenDSObject(
                                     lpwszRouterIdentityObjectPath,
                                     NULL,
                                     NULL,
                                     0,
                                     phObjectRouterIdentity );
        }

        if ( FAILED( hResult ) )
        {
            dwRetCode = HRESULT_CODE( hResult );
        }
        else
        {
            dwRetCode = NO_ERROR;
        }

    } while( FALSE );

    if ( lpwszRouterIdentityObjectPath != NULL )
    {
        LOCAL_FREE( lpwszRouterIdentityObjectPath );
    }

    if ( lpwszComputerObjectPath != NULL )
    {
        LOCAL_FREE( lpwszComputerObjectPath );
    }

    if ( pDomainControllerInfo != NULL )
    {
        NetApiBufferFree( pDomainControllerInfo );
    }

    TracePrintfExA( gblDIMConfigInfo.dwTraceId, TRACE_DIM, 
                    "RouterIdentityObjectOpen returned %d", dwRetCode );

    return( dwRetCode );
}

//**
//
// Call:        RouterIdentityObjectClose
//
// Returns:     NO_ERROR         - Success
//              Non-zero returns - Failure
//
// Description: Will close the router object.
//
VOID
RouterIdentityObjectClose(
    IN HANDLE hObjectRouterIdentity
)
{
    ADSICloseDSObject( hObjectRouterIdentity );
}

//**
//
// Call:        RouterIdentityObjectGetAttributes
//
// Returns:     NO_ERROR         - Success
//              Non-zero returns - Failure
//
// Description: Will retreive all the attributes of the give Router object
//
DWORD
RouterIdentityObjectGetAttributes(
    IN      HANDLE      hRouterIdentityObject,
    OUT     HANDLE *    phRouterIdentityAttributes

)
{
    ADS_ATTR_INFO *     pADSAttributes          = NULL;
    DWORD               dwNumAttributesReturned = 0;
    HRESULT             hResult;

    *phRouterIdentityAttributes = NULL;

    //
    // Get all the attributes in this object
    //

    hResult = ADSIGetObjectAttributes(
                    hRouterIdentityObject,
                    RouterObjectAttributeNames,
                    sizeof( RouterObjectAttributeNames ) / sizeof( LPWSTR ),
                    &pADSAttributes,
                    &dwNumAttributesReturned );

    if ( FAILED( hResult ) )
    {
        return( HRESULT_CODE( hResult ) );
    }

    if ( dwNumAttributesReturned > 0 )
    {
        *phRouterIdentityAttributes = (HANDLE)pADSAttributes;
    }
    else
    {
        TracePrintfExA( gblDIMConfigInfo.dwTraceId, 
                        TRACE_DIM, "No attributes in identity object" );
    }

    return( NO_ERROR );
}


//**
//
// Call:        RouterIdentityObjectIsValueSet
//
// Returns:     NO_ERROR         - Success
//              Non-zero returns - Failure
//
// Description: Will check to see if a give value exists for the attribute
//
BOOL
RouterIdentityObjectIsValueSet(
    IN HANDLE   hRouterIdentityAttributes,
    IN DWORD    dwVendorId,
    IN DWORD    dwType,
    IN DWORD    dwValue
)
{
    ADS_ATTR_INFO * pADSAttributes = (ADS_ATTR_INFO *)hRouterIdentityAttributes;
    DWORD           dwIndex;
    WCHAR           wchValue[100];
    CHAR            chValue[100];

    if ( pADSAttributes == NULL )
    {
        return( FALSE );   
    }

    if (_wcsicmp(pADSAttributes->pszAttrName, ROUTER_OBJECT_ATTRIBUTE_NAME)!=0)
    {
        return( FALSE );
    }

    wsprintf( wchValue, TEXT("%d:%d:%d"), dwVendorId, dwType, dwValue );
    sprintf( chValue, "%d:%d:%d", dwVendorId, dwType, dwValue );
    
    for( dwIndex = 0; dwIndex < pADSAttributes->dwNumValues; dwIndex ++ )
    {
        ADSVALUE * pADsValue = &(pADSAttributes->pADsValues[dwIndex]);

        switch (pADsValue->dwType) {

            case ADSTYPE_PROV_SPECIFIC:
            {
                ADS_PROV_SPECIFIC *pProviderSpecific;
                pProviderSpecific = &pADsValue->ProviderSpecific;
                if (strncmp( pProviderSpecific->lpValue, chValue, 
                    pProviderSpecific->dwLength) == 0 )
                {
                    return( TRUE );
                }
                break;
            }
            
            case ADSTYPE_CASE_IGNORE_STRING:
                if ( _wcsicmp( pADsValue->CaseIgnoreString, wchValue ) == 0 )
                {
                    return( TRUE );
                }
                break;

            default : //same as ADSTYPE_CASE_IGNORE_STRING
                if ( _wcsicmp( pADsValue->CaseIgnoreString, wchValue ) == 0 )
                {
                    return( TRUE );
                }
                break;
        }
    }

    return( FALSE );
}

//**
//
// Call:        RouterIdentityObjectGetValue
//
// Returns:     NO_ERROR         - Success
//              Non-zero returns - Failure
//
// Description: Will get the specified indexed value from the router object
//
DWORD
RouterIdentityObjectGetValue(
    IN HANDLE   hRouterIdentityAttributes,
    IN DWORD    dwValueIndex,
    IN DWORD *  lpdwVendorId,
    IN DWORD *  lpdwType,
    IN DWORD *  lpdwValue
)
{
    ADS_ATTR_INFO * pADSAttributes = (ADS_ATTR_INFO *)hRouterIdentityAttributes;
    DWORD           dwIndex;
    ADSVALUE *      pADsValue;

    if ( pADSAttributes == NULL )
    {
        return( ERROR_DS_NO_ATTRIBUTE_OR_VALUE );   
    }

    if (_wcsicmp(pADSAttributes->pszAttrName, ROUTER_OBJECT_ATTRIBUTE_NAME)!=0)
    {
        return( ERROR_DS_NO_ATTRIBUTE_OR_VALUE );   
    }

    if ( dwValueIndex >= pADSAttributes->dwNumValues )
    {
        *lpdwVendorId   = (DWORD)-1;
        *lpdwType       = (DWORD)-1;
        *lpdwValue      = (DWORD)-1;

        return( NO_ERROR );
    }

    pADsValue = &(pADSAttributes->pADsValues[dwValueIndex]);

    switch (pADsValue->dwType) {

        case ADSTYPE_PROV_SPECIFIC:
        {
            ADS_PROV_SPECIFIC *pProviderSpecific;
            CHAR            chValue[100];
            pProviderSpecific = &pADsValue->ProviderSpecific;

            strncpy(chValue, pProviderSpecific->lpValue,
                pProviderSpecific->dwLength);
            chValue[pProviderSpecific->dwLength] = 0;
            
            if (scanf( chValue,
                         TEXT("%d:%d:%d"),
                         lpdwVendorId,
                         lpdwType,
                         lpdwValue ) == EOF)
            {
                return ERROR_DS_INVALID_ATTRIBUTE_SYNTAX;
            }          
            break;
        }
        default :
        {
            if (swscanf( pADsValue->CaseIgnoreString,
                         TEXT("%d:%d:%d"),
                         lpdwVendorId,
                         lpdwType,
                         lpdwValue ) == EOF)
            {
                return ERROR_DS_INVALID_ATTRIBUTE_SYNTAX;
            }
            
            break;
        }
    }
    
    return( NO_ERROR );
}

//**
//
// Call:        RouterIdentityObjectAddRemoveValue
//
// Returns:     NO_ERROR         - Success
//              Non-zero returns - Failure
//
// Description: Will add or remove a value from the multi-valued attribute
//
DWORD
RouterIdentityObjectAddRemoveValue(
    IN  HANDLE      hRouterIdentityObject,
    IN  DWORD       dwVendorId,
    IN  DWORD       dwType,
    IN  DWORD       dwValue,
    IN  BOOL        fAdd
)
{
    HRESULT         hResult;
    DWORD           dwNumAttributesModified;
    ADS_ATTR_INFO   AttributeEntry[1];
    WCHAR           wchValue[100];
    ADSVALUE        AttributeValue;

    wsprintf( wchValue, TEXT("%d:%d:%d"), dwVendorId, dwType, dwValue );

    AttributeValue.dwType           = ADSTYPE_CASE_IGNORE_STRING;
    AttributeValue.CaseIgnoreString = wchValue;

    AttributeEntry[0].pszAttrName   = ROUTER_OBJECT_ATTRIBUTE_NAME;
    AttributeEntry[0].dwControlCode = ( fAdd )
                                            ? ADS_ATTR_APPEND
                                            : ADS_ATTR_DELETE;
    AttributeEntry[0].dwADsType     = ADSTYPE_CASE_IGNORE_STRING;
    AttributeEntry[0].pADsValues    = &AttributeValue;
    AttributeEntry[0].dwNumValues   = 1;

    if ( fAdd )
    {
        TracePrintfExA( gblDIMConfigInfo.dwTraceId,
                        TRACE_DIM,
                        "Adding value %ws in the Router Identity Object",
                        wchValue );
    }
    else
    {
        TracePrintfExA( gblDIMConfigInfo.dwTraceId,
                        TRACE_DIM,
                        "Removing value %ws in the Router Identity Object",
                        wchValue );
    }

    hResult = ADSISetObjectAttributes( hRouterIdentityObject,
                                       AttributeEntry,
                                       1,
                                       &dwNumAttributesModified );
    if ( FAILED( hResult ) )
    {
        return( HRESULT_CODE( hResult ) );
    }

    return( NO_ERROR );
}

//**
//
// Call:        RouterIdentityObjectFreeAttributes
//
// Returns:     NO_ERROR         - Success
//              Non-zero returns - Failure
//
// Description: Frees allocated set of attributes returned by
//              RouterIdentityObjectGetAttributes
//
VOID
RouterIdentityObjectFreeAttributes(
    IN HANDLE   hRouterIdentityAttributes
)
{
    if ( hRouterIdentityAttributes != NULL )
    {
        FreeADsMem( (ADS_ATTR_INFO *)hRouterIdentityAttributes );
    }
}

//**
//
// Call:        RouterIdentityObjectSetAttributes
//
// Returns:     NO_ERROR         - Success
//              Non-zero returns - Failure
//
// Description: Will gather all current configuration information and plumb it
//              into the router identity object in the DS.
//
//              Note: 
//                  This API first takes the lock on the interface table,
//                  then it takes the lock around the device table to get the
//                  installed device types. Hence this API MUST NOT be called
//                  while holding a lock around the interface table since this
//                  violates the design principal of first holding the 
//                  device lock before holding the interface lock.
//
DWORD
RouterIdentityObjectSetAttributes(
    IN HANDLE  hRouterIdentityObject
)
{
    DWORD                       dwRetCode;
    HANDLE                      hRouterIdentityAttributes;
    DWORD                       dwIndex = 0;
    ROUTER_IDENTITY_ATTRIBUTE   RIAttributes[DIM_MAX_IDENTITY_ATTRS];
    DWORD                       dwXportIndex;

    //
    // Obtain router identity information plumbed in the DS currently
    //

    dwRetCode = RouterIdentityObjectGetAttributes(
                                                hRouterIdentityObject,
                                                &hRouterIdentityAttributes );

    if ( dwRetCode != NO_ERROR )
    {
        return( dwRetCode );
    }

    //
    // Now get the current running configuration of the router
    //

    //
    // First, what is our role?
    //

    if ( gblDIMConfigInfo.dwRouterRole & ROUTER_ROLE_LAN )
    {
        RIAttributes[dwIndex].dwVendorId = DIM_MS_VENDOR_ID;
        RIAttributes[dwIndex].dwType     = 6;
        RIAttributes[dwIndex].dwValue    = 601;

        dwIndex++;
    }

    if ( gblDIMConfigInfo.dwRouterRole & ROUTER_ROLE_RAS )
    {
        RIAttributes[dwIndex].dwVendorId = DIM_MS_VENDOR_ID;
        RIAttributes[dwIndex].dwType     = 6;
        RIAttributes[dwIndex].dwValue    = 602;

        dwIndex++;
    }

    if ( gblDIMConfigInfo.dwRouterRole & ROUTER_ROLE_WAN )
    {
        RIAttributes[dwIndex].dwVendorId = DIM_MS_VENDOR_ID;
        RIAttributes[dwIndex].dwType     = 6;
        RIAttributes[dwIndex].dwValue    = 603;

        dwIndex++;
    }

    //
    // Check if a LAN interface exists
    //

    EnterCriticalSection( &(gblInterfaceTable.CriticalSection) );

    if ( IfObjectDoesLanInterfaceExist() )
    {
        RIAttributes[dwIndex].dwVendorId    = 311;
        RIAttributes[dwIndex].dwType        = 6;
        RIAttributes[dwIndex].dwValue       = 712;

        dwIndex++;
    }

    //
    // Get all IP routing protocols
    //

    if ( ( dwXportIndex = GetTransportIndex( PID_IP ) ) != (DWORD)-1 )
    {
        BYTE * pGlobalInfo      = NULL;
        DWORD  dwGlobalInfoSize = 0;

        dwRetCode =
                gblRouterManagers[dwXportIndex].DdmRouterIf.GetGlobalInfo(
                                            pGlobalInfo,
                                            &dwGlobalInfoSize );

        if ( dwRetCode == ERROR_INSUFFICIENT_BUFFER )
        {
            if ( dwGlobalInfoSize > 0 )
            {
                pGlobalInfo = LOCAL_ALLOC( LPTR, dwGlobalInfoSize );

                if ( pGlobalInfo != NULL )
                {
                    dwRetCode =
                    gblRouterManagers[dwXportIndex].DdmRouterIf.GetGlobalInfo(
                                            pGlobalInfo,
                                            &dwGlobalInfoSize );

                    if ( dwRetCode == NO_ERROR )
                    {
                        DWORD                   dwRoutingProtIndex;
                        RTR_INFO_BLOCK_HEADER * pInfoBlock =
                                        (RTR_INFO_BLOCK_HEADER *)(pGlobalInfo);

                        for ( dwRoutingProtIndex = 0;
                              dwRoutingProtIndex < pInfoBlock->TocEntriesCount;
                              dwRoutingProtIndex++ )
                        {
                            DWORD dwVendorId;

                            RIAttributes[dwIndex].dwType     
                            = TYPE_FROM_PROTO_ID(
                            pInfoBlock->TocEntry[dwRoutingProtIndex].InfoType );

                            // 
                            // Add unicast and multicast protocol ids
                            //
                            if ( ( RIAttributes[dwIndex].dwType 
                                                        == PROTO_TYPE_UCAST ) ||
                                 ( RIAttributes[dwIndex].dwType 
                                                        == PROTO_TYPE_MCAST ) )
                            {
                                DWORD dwProtoId;
                                
                                dwVendorId = VENDOR_FROM_PROTO_ID(

                                pInfoBlock->TocEntry[dwRoutingProtIndex].InfoType );

                                dwProtoId = PROTO_FROM_PROTO_ID(

                                pInfoBlock->TocEntry[dwRoutingProtIndex].InfoType );

                                //
                                // Nothing defined for dhcp relay agent
                                //
                                if ( dwProtoId == PROTO_IP_BOOTP )
                                {
                                    continue;
                                }
                                
                                if ( ( dwVendorId == PROTO_VENDOR_MS0 ) ||
                                     ( dwVendorId == PROTO_VENDOR_MS1 ) ||
                                     ( dwVendorId == PROTO_VENDOR_MS2 ) )
                                {
                                    RIAttributes[dwIndex].dwVendorId = 311;
                                }
                                else
                                {
                                    RIAttributes[dwIndex].dwVendorId = dwVendorId;
                                }

                                RIAttributes[dwIndex].dwValue    
                                = PROTO_FROM_PROTO_ID(
                                pInfoBlock->TocEntry[dwRoutingProtIndex].InfoType );

                                dwIndex++;
                            }

                            // 
                            // Add ms0 protocols
                            //
                            else if (RIAttributes[dwIndex].dwType == 
                                        PROTO_TYPE_MS0 )
                            {
                                DWORD dwProtoId;
                                
                                dwVendorId = VENDOR_FROM_PROTO_ID(

                                pInfoBlock->TocEntry[dwRoutingProtIndex].InfoType );

                                dwProtoId = PROTO_FROM_PROTO_ID(

                                pInfoBlock->TocEntry[dwRoutingProtIndex].InfoType );

                                // 
                                // Check for NAT
                                // Vendor= MS, TypeMajor= 6, TypeMinor= 604
                                //
                                if ( ( dwVendorId == PROTO_VENDOR_MS1 ) && 
                                     ( dwProtoId  == PROTO_IP_NAT)
                                   )
                                {
                                    RIAttributes[dwIndex].dwVendorId = 
                                        PROTO_VENDOR_MS1;
                                    RIAttributes[dwIndex].dwType     = 
                                        6;
                                    RIAttributes[dwIndex].dwValue    = 
                                        604;
                                        
                                    dwIndex++;
                                }
                            }
                        }
                    }

                    LOCAL_FREE( pGlobalInfo );
                }
            }
        }

        // As per amritanr, if you have the ip router installed,
        // then ip forwarding is always turned on.
        //
        // Ip Fwd'ing Enabled: Vendor= MS, TypeMajor= 6, TypeMinor= 501
        //
        RIAttributes[dwIndex].dwVendorId = PROTO_VENDOR_MS1;
        RIAttributes[dwIndex].dwType     = 6;
        RIAttributes[dwIndex].dwValue    = 501;
        dwIndex++;
    }

    //
    // Get all IPX routing protocols
    //

    if ( ( dwXportIndex = GetTransportIndex( PID_IPX ) ) != (DWORD)-1 )
    {
        // All nt5 ipx routers support rip and sap.  Go ahead 
        // and attributes for both.
        //

        // IPXRIP: Vendor= MS, TypeMajor= 5, TypeMinor= 1
        //
        RIAttributes[dwIndex].dwVendorId = PROTO_VENDOR_MS1;
        RIAttributes[dwIndex].dwType     = 5;
        RIAttributes[dwIndex].dwValue    = 1;
        dwIndex++;

        // IPXSAP: Vendor= MS, TypeMajor= 5, TypeMinor= 2
        //
        RIAttributes[dwIndex].dwVendorId = PROTO_VENDOR_MS1;
        RIAttributes[dwIndex].dwType     = 5;
        RIAttributes[dwIndex].dwValue    = 2;
        dwIndex++;

        // Ipx Fwd'ing Enabled: Vendor= MS, TypeMajor= 6, TypeMinor= 502
        //
        RIAttributes[dwIndex].dwVendorId = PROTO_VENDOR_MS1;
        RIAttributes[dwIndex].dwType     = 6;
        RIAttributes[dwIndex].dwValue    = 502;
        dwIndex++;
    }

    LeaveCriticalSection( &(gblInterfaceTable.CriticalSection) );

    RIAttributes[dwIndex].dwVendorId    = (DWORD)-1;
    RIAttributes[dwIndex].dwType        = (DWORD)-1;
    RIAttributes[dwIndex].dwValue       = (DWORD)-1;

    //
    // Get all the RAS server information
    //

    if ( gblDIMConfigInfo.dwRouterRole & ( ROUTER_ROLE_RAS | ROUTER_ROLE_WAN ) )
    {
        DWORD (*DDMGetIdentityAttributes)( ROUTER_IDENTITY_ATTRIBUTE * ) =
            (DWORD(*)( ROUTER_IDENTITY_ATTRIBUTE * ))
                                GetDDMEntryPoint("DDMGetIdentityAttributes");

        if(NULL != DDMGetIdentityAttributes)
        {
            dwRetCode = DDMGetIdentityAttributes( RIAttributes );
        }
        else
        {
            ASSERT(FALSE);
        }
    }

    //
    // Now obtain walk thru the current configration an make sure that
    // all of it is plumbed. If it is not then go ahead and set it.
    //

    for ( dwIndex = 0;
          RIAttributes[dwIndex].dwVendorId != (DWORD)-1;
          dwIndex++ )
    {
        //
        // If this attribute is not set, then set it
        //

        if ( !RouterIdentityObjectIsValueSet(
                                        hRouterIdentityAttributes,
                                        RIAttributes[dwIndex].dwVendorId,
                                        RIAttributes[dwIndex].dwType,
                                        RIAttributes[dwIndex].dwValue ) )
        {
            RouterIdentityObjectAddRemoveValue(
                                        hRouterIdentityObject,
                                        RIAttributes[dwIndex].dwVendorId,
                                        RIAttributes[dwIndex].dwType,
                                        RIAttributes[dwIndex].dwValue,
                                        TRUE );
        }
    }

    //
    // Now walk thru and remove attributes in the DS that are not in our
    // current configuration. We reconcile all attributes with
    // dwType values 0,1 or 5, and with dwVendorId of 311 (Microsoft) with
    // dwType value of 6.
    //

    for ( dwIndex = 0;; dwIndex++ )
    {
        DWORD dwVendorId;
        DWORD dwType;
        DWORD dwValue;
        DWORD dwCurrentValueIndex;
        BOOL  fInCurrentConfiguration = FALSE;

        dwRetCode = RouterIdentityObjectGetValue( hRouterIdentityAttributes,
                                                  dwIndex,
                                                  &dwVendorId,
                                                  &dwType,
                                                  &dwValue );

        if ( dwRetCode != NO_ERROR )
        {
            break;
        }

        //
        // We are done
        //

        if ( dwVendorId == (DWORD)-1 )
        {
            break;
        }

        //
        // Ignore these types
        //

        if ( ( dwType != 0 ) && ( dwType != 1 ) && ( dwType != 5 ) &&
             ( !( ( dwType == 6 ) && ( dwVendorId == DIM_MS_VENDOR_ID ) ) ) )
        {
            continue;
        }

        //
        // Now check to see it this attribute is a member of our current
        // configuration
        //


        for ( dwCurrentValueIndex = 0;
              RIAttributes[dwCurrentValueIndex].dwVendorId != (DWORD)-1;
              dwCurrentValueIndex++ )
        {
            if ( (RIAttributes[dwCurrentValueIndex].dwVendorId == dwVendorId)&&
                 (RIAttributes[dwCurrentValueIndex].dwType     == dwType )   &&
                 (RIAttributes[dwCurrentValueIndex].dwValue    == dwValue ) )
            {
                //
                // Attribute is part of current configuration
                //

                fInCurrentConfiguration = TRUE;
            }
        }

        if ( !fInCurrentConfiguration )
        {
            //
            // Remove this attribute from the DS since it is not in our
            // current configuration
            //


            dwRetCode = RouterIdentityObjectAddRemoveValue(
                                                        hRouterIdentityObject,
                                                        dwVendorId,
                                                        dwType,
                                                        dwValue,
                                                        FALSE );
            if ( dwRetCode != NO_ERROR )
            {
                break;
            }
        }
    }

    RouterIdentityObjectFreeAttributes( hRouterIdentityAttributes );

    return( dwRetCode );
}

//**
//
// Call:        RouterIdentityObjectUpdateAttributes
//
// Returns:     NO_ERROR         - Success
//              Non-zero returns - Failure
//
// Description: Will be called to update the attributes currently set in the DS
//              or to set it if it was not able to be set originally
//
VOID
RouterIdentityObjectUpdateAttributes(
    IN PVOID    pParameter,
    IN BOOLEAN  fTimedOut
)
{
    DWORD dwRetCode        = NO_ERROR;
    BOOL  fCalledFromTimer = (BOOL)PtrToUlong(pParameter);

    //
    // Make sure service is in the running state
    //

    if ( gblDIMConfigInfo.ServiceStatus.dwCurrentState != SERVICE_RUNNING )
    {
        if(fCalledFromTimer)
        {
            //
            // Delete the timer if theres one queued before we
            // return from here otherwise DeleteTimerQ in
            // DimCleanup will hang.
            //
            RtlDeleteTimer(gblDIMConfigInfo.hTimerQ,
                           gblDIMConfigInfo.hTimer, 
                           NULL);
            gblDIMConfigInfo.hTimer = NULL;
        }
        
        return;
    }

    if ( fCalledFromTimer )
    {
        //
        // Always call DeleteTimer otherwise we will leak memory
        //

        RtlDeleteTimer( gblDIMConfigInfo.hTimerQ,
                        gblDIMConfigInfo.hTimer,
                        NULL );

        gblDIMConfigInfo.hTimer = NULL;                        

        //
        // Called from timer thread so we first try to obtain a handle to
        // the router idenitity object
        //

        dwRetCode = RouterIdentityObjectOpen(
                              NULL,
                              ( gblDIMConfigInfo.dwRouterRole ),
                              &(gblDIMConfigInfo.hObjectRouterIdentity) );

        if ( ( dwRetCode != NO_ERROR ) ||
             ( gblDIMConfigInfo.hObjectRouterIdentity == NULL ) ) 
        {
            //
            // Check to see if service is stopping and create a timer for this
            // only if it is in running state.
            //
            if(gblDIMConfigInfo.ServiceStatus.dwCurrentState == SERVICE_RUNNING)
            {
                //
                // Couldn't access DC, try again later for a max of once a day
                //

                if ( gblDIMConfigInfo.dwRouterIdentityDueTime < 24*60*60*1000 )
                {
                    gblDIMConfigInfo.dwRouterIdentityDueTime *= 2;
                }

                TracePrintfExA(
                           gblDIMConfigInfo.dwTraceId,
                           TRACE_DIM,
                           "Could not access DC, will set router attributes later");

                RtlCreateTimer( gblDIMConfigInfo.hTimerQ,
                                &(gblDIMConfigInfo.hTimer),
                                RouterIdentityObjectUpdateAttributes,
                                (PVOID)TRUE,
                                gblDIMConfigInfo.dwRouterIdentityDueTime,
                                0,
                                WT_EXECUTEDEFAULT );
            }                            

            return;
        }

        //
        // Otherwise we succeeded in opening the router identity object
        // and we set the identity information below.
        //
    }
    else
    {
        //
        // If we do not have a handle for the router identity object, then
        // either we are in the process of obtaining it or we are not
        // a member of the DS, so in both cases simply return
        //

        if ( gblDIMConfigInfo.hObjectRouterIdentity == NULL )
        {
            return;
        }
    }

    //
    // Can be called from different threads at the same time so we need
    // Critical section around this code so that we do not have 2 writers to 
    // the DS at the same time that trample on each other. ex could be called
    // by DDM thread as well as the timer thread.
    //

    EnterCriticalSection( &(gblDIMConfigInfo.CSRouterIdentity) );

    TracePrintfExA( gblDIMConfigInfo.dwTraceId,
                    TRACE_DIM,
                    "Setting router attributes in the identity object" );

    RouterIdentityObjectSetAttributes( gblDIMConfigInfo.hObjectRouterIdentity );

    LeaveCriticalSection( &(gblDIMConfigInfo.CSRouterIdentity) );
}

//**
//
// Call:        RouterIdentityObjectUpdateAttributesForDD
//
// Returns:     None
//
// Description:
//
VOID
RouterIdentityObjectUpdateAttributesForDDM(
    PVOID pParameter
)
{
    RouterIdentityObjectUpdateAttributes( (PVOID)NULL, FALSE );
}

//**
//
// Call:        RouterIdentityObjectUpdateDDMAttributes
//
// Returns:     None
//
// Description: Export this call to DDM. When DDM calls this call, it already
//              has taken a lock around it's device table and cannot make
//              the call to update attributes directly because the   
//              RouterIdentityObjectUpdateAttributes call takes the
//              lock around the RouterIdentity object leading to a deadlock.
//              Hence we execute this call asynchronously using a worker
//              thread.
//              
//
VOID
RouterIdentityObjectUpdateDDMAttributes(
    VOID
)
{
    RtlQueueWorkItem( RouterIdentityObjectUpdateAttributesForDDM,
                      NULL,
                      WT_EXECUTEDEFAULT );
}
