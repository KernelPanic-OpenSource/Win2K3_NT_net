/*
    File:   rpbk.c

    Implementation of api's that populate the router phonebook
    portions of MPR intformation structures.

    Paul Mayfield, 11/2/98

    The following describes the mapping from ras entry  to 
    router entry.

    // The following RAS options are not supported.  They will always
    // be cleared if set.
    //
    RASEO_UseCountryAndAreaCodes
    RASEO_TerminalBeforeDial        
    RASEO_TerminalAfterDial         
    RASEO_ModemLights               
    RASEO_UseLogonCredentials     
    RASEO_Custom                    
    RASEO_PreviewPhoneNumber        
    RASEO_PreviewUserPw             
    RASEO_PreviewDomain             
    RASEO_ShowDialingProgress       

    // The following ras types are not supported.  
    //
    RASET_Internet      // Reset to MPRET_Phone if set

    // The following protocol setting is not supported
    //
    RASNP_NetBEUI       // Always exclude

    //
    // RASENTRY Conversion
    //
    Items deleted:
        DWORD       dwSize;                     // version is MPR_INTERFACE_2
        WCHAR       szScript[ MAX_PATH ];       // Set to ""
        DWORD       dwReserved1;                // Set to 0
        DWORD       dwReserved2;                // Set to 0
        WCHAR       szAutodialDll[...];         // Set to ""
        WCHAR       szAutodialFunc[...];        // Set to ""
        WCHAR       szCustomDialDll[MAX_PATH];  // Set to ""
        DWORD       dwFramingProtocol;          // Set to PPP
        DWORD       dwCountryID;                // Set to 0
        DWORD       dwCountryCode;              // Set to 0
        WCHAR       szAreaCode[ RAS_MaxAreaCode + 1 ];  // Set to ""
        DWORD       dwFrameSize

    Items modified:
        DWORD dwAlternateOffset to PWCHAR pszAlternates
        
    	RASIPADDR   ipaddr	        to DWORD
      	RASIPADDR   ipaddrDns	    to DWORD
       	RASIPADDR   ipaddrDnsAlt	to DWORD
       	RASIPADDR   ipaddrWins	    to DWORD
       	RASIPADDR   ipaddrWinsAlt	to DWORD

    //
    // RASSUBENTRY Conversion
    //
    Items deleted:
        DWORD       dwSize;     // version is MPR_INTERFACE_DEVICE_0
        DWORD       dwfFlags;   // unused anyway

    Items modified:
        DWORD dwAlternateOffset to PWCHAR pszAlternates

*/

#include "dimsvcp.h"
#include <ras.h>
#include <dimsvc.h>     // Generated by MIDL
#include <mprapi.h>
#include <mprapip.h>
#include "rpbk.h"

//
// Definitions
//
#define MPRIO_UnsupportedOptions            \
        (                                   \
            RASEO_UseCountryAndAreaCodes  | \
            RASEO_TerminalBeforeDial      | \
            RASEO_TerminalAfterDial       | \
            RASEO_ModemLights             | \
            RASEO_UseLogonCredentials     | \
            RASEO_Custom                  | \
            RASEO_PreviewPhoneNumber      | \
            RASEO_PreviewUserPw           | \
            RASEO_PreviewDomain           | \
            RASEO_ShowDialingProgress       \
        )

//
// Strings
//
static const WCHAR  pszRouterPbkFmt[] = L"\\ras\\Router.pbk";

// 
// Structure tracks router entry information
//
typedef struct _RPBK_ENTRY_INFO
{
    PWCHAR pszPhonebookPath;

    DWORD dwEntrySize;
    LPRASENTRY pRasEntry;
    
    DWORD dwCustAuthDataSize;
    LPBYTE lpbCustAuthData;
    
} RPBK_ENTRY_INFO;

// 
// Structure tracks sub entry information
//
typedef struct _RPBK_SUBENTRY_INFO
{
    PWCHAR pszPhonebookPath;
    LPRASSUBENTRY pRasSubEntry;
    DWORD dwSize;
} RPBK_SUBENTRY_INFO;

//
// Common allocation
//
PVOID
RpbkAlloc(
    IN DWORD dwBytes,
    IN BOOL bZero)
{
    return LOCAL_ALLOC( (bZero) ? HEAP_ZERO_MEMORY : 0, dwBytes );
}

//
// Common free
//
VOID
RpbkFree(
    IN PVOID pvData)
{
    LOCAL_FREE( pvData );
}    

//
// Cleans up the entry info blob
//
VOID
RpbkFreeEntryInfo(
    IN RPBK_ENTRY_INFO * pInfo)
{
    if (pInfo)
    {
        if (pInfo->pszPhonebookPath)
        {
            RpbkFreePhonebookPath(pInfo->pszPhonebookPath);
        }
        if (pInfo->pRasEntry)
        {
            RpbkFree(pInfo->pRasEntry);
        }
        if (pInfo->lpbCustAuthData)
        {
            RpbkFree(pInfo->lpbCustAuthData);
        }
        RpbkFree(pInfo);
    }
}

//
// Cleans up the sub entry info blob
//
VOID
RpbkFreeSubEntryInfo(
    IN RPBK_SUBENTRY_INFO * pInfo)
{
    if (pInfo)
    {
        if (pInfo->pszPhonebookPath)
        {
            RpbkFree(pInfo->pszPhonebookPath);
        }
        if (pInfo->pRasSubEntry)
        {
            RpbkFree(pInfo->pRasSubEntry);
        }
        RpbkFree(pInfo);
    }
}

//
// Returns a heap-allocated copy of the path to the router
// phonebook
//
DWORD 
RpbkGetPhonebookPath(
    OUT PWCHAR* ppszPath)
{
    WCHAR pszSystemPath[MAX_PATH];
    UINT uiLength = sizeof(pszSystemPath) / sizeof(WCHAR);
    DWORD dwRetSize = 0;
    PWCHAR pszRet = NULL;

    // Find the system directory
    //
    uiLength = GetSystemDirectoryW(pszSystemPath, uiLength);
    if (uiLength == 0)
    {
        return ERROR_NOT_ENOUGH_MEMORY;
    }

    // Allocate the return buffer
    //
    dwRetSize = ((uiLength + 1) * sizeof(WCHAR)) + sizeof(pszRouterPbkFmt);
    pszRet = (PWCHAR) RpbkAlloc(dwRetSize, FALSE);
    if (pszRet == NULL)
    {
        return ERROR_NOT_ENOUGH_MEMORY;
    }

    // Format the string
    //
    wcscpy(pszRet, pszSystemPath);
    wcscpy(pszRet + uiLength, pszRouterPbkFmt);

    *ppszPath = pszRet;
    
    return NO_ERROR;
}

//
// Cleans up after RpbkGetPhonebookPath
//
DWORD
RpbkFreePhonebookPath(
    IN PWCHAR pszPath)
{
    if ( pszPath )
    {
        RpbkFree(pszPath);
    }

    return NO_ERROR;
}

//
// Returns size in bytes of a multisz.
//
DWORD
RpbkGetMultiSzSize(
    IN LPWSTR lpwsMultSz
)
{
    LPWSTR lpwsPtr = lpwsMultSz;
    DWORD dwcbAlternates  = 0;
    DWORD dwCurCount;

    if ( lpwsMultSz == NULL )
    {
        return( 0 );
    }

    while( *lpwsPtr != L'\0' )
    {
        dwCurCount = ( wcslen( lpwsPtr ) + 1 );
        dwcbAlternates += dwCurCount;
        lpwsPtr += dwCurCount;
    }

    //
    // One more for the last NULL terminator
    //

    dwcbAlternates++;

    dwcbAlternates *= sizeof( WCHAR );

    return( dwcbAlternates );
}

//
// Copies a multi sz 
//
DWORD 
RpbkCopyMultiSz(
    IN LPWSTR lpwsDst,
    IN LPWSTR lpwsSrc)
{
    if (!lpwsDst || !lpwsSrc)
    {
        return ERROR_INVALID_PARAMETER;
    }

    while (lpwsSrc[0] || lpwsSrc[1])
    {
        *lpwsDst = *lpwsSrc;
        lpwsDst++;
        lpwsSrc++;
    }
    lpwsDst[0] = (WCHAR)0;
    lpwsDst[1] = (WCHAR)0;
    
    return NO_ERROR;
}

//
// Removes any unsupported options from the interface data
// provided.
//
DWORD
RpbkConformIfData(
    IN DWORD dwLevel, 
    IN LPBYTE pInterfaceData)
{
    MPR_INTERFACE_2* pIf2 = (MPR_INTERFACE_2*)pInterfaceData;

    // Clear the unsupported options
    //
    pIf2->dwfOptions &= ~MPRIO_UnsupportedOptions;

    // Make sure that netbios doesn't get reported
    //
    pIf2->dwfNetProtocols &= ~RASNP_NetBEUI;

    // Make sure that the type is not internet
    //
    if (pIf2->dwType == RASET_Internet)
    {
        pIf2->dwType = RASET_Phone;
    }

    return NO_ERROR;
}

//
// Removes any unsupported options from the router entry
// provided.
//
DWORD
RpbkConformEntry(
    IN LPRASENTRY pEntry)
{
    // Clear the unsupported options
    //
    pEntry->dwfOptions &= ~MPRIO_UnsupportedOptions;

    // Make sure that netbios is not enabled
    //
    pEntry->dwfNetProtocols &= ~RASNP_NetBEUI;

    // Make sure that the type is not internet
    //
    if (pEntry->dwType == RASET_Internet)
    {
        pEntry->dwType = RASET_Phone;
    }

    // Default all other values of the entry that
    // can't be supplied through MPR_INTERFACE_* 
    // structures.
    pEntry->dwSize              = sizeof(RASENTRY);
    pEntry->dwReserved1         = 0;
    pEntry->dwReserved2         = 0;
    pEntry->dwFramingProtocol   = RASFP_Ppp;
    pEntry->dwFrameSize         = 0;
    pEntry->dwCountryID         = 0;
    pEntry->dwCountryCode       = 0;
    pEntry->szScript[0]         = L'\0';
    pEntry->szAutodialDll[0]    = L'\0';
    pEntry->szAutodialFunc[0]   = L'\0';
    pEntry->szCustomDialDll[0]  = L'\0';
    pEntry->szAreaCode[0]       = L'\0';

    return NO_ERROR;
}

//
// Reads in the router phonebook entry associated with
// the given interface
//
DWORD 
RpbkOpenEntry(
    IN  ROUTER_INTERFACE_OBJECT* pIfObject, 
    OUT PHANDLE                  phEntry )
{
    RPBK_ENTRY_INFO * pInfo = NULL;
    DWORD dwErr = NO_ERROR, dwSize;

    do {
        // Initialize
        *phEntry = NULL;
    
        // Allocate the control structure
        //
        pInfo = (RPBK_ENTRY_INFO*) RpbkAlloc(sizeof(RPBK_ENTRY_INFO), TRUE);
        if (pInfo == NULL)
        {
            dwErr = ERROR_NOT_ENOUGH_MEMORY;
            break;
        }

        // Get the phonebook path
        //
        dwErr = RpbkGetPhonebookPath(&(pInfo->pszPhonebookPath));
        if (dwErr != NO_ERROR)
        {
            break;
        }

        // Find out how big the ras entry needs to be
        //
        dwErr = RasGetEntryProperties(
                    pInfo->pszPhonebookPath,
                    pIfObject->lpwsInterfaceName,
                    NULL,
                    &(pInfo->dwEntrySize),
                    NULL,
                    NULL);
        if (dwErr != ERROR_BUFFER_TOO_SMALL)
        {
            break;
        }

        // Allocate the ras entry structure
        //
        pInfo->pRasEntry = (LPRASENTRY) RpbkAlloc(pInfo->dwEntrySize, TRUE);
        if (pInfo->pRasEntry == NULL)
        {
            dwErr = ERROR_NOT_ENOUGH_MEMORY;
            break;
        }

        // Read in the ras entry
        //
        pInfo->pRasEntry->dwSize = sizeof(RASENTRY);
        dwErr = RasGetEntryProperties(
                    pInfo->pszPhonebookPath,
                    pIfObject->lpwsInterfaceName,
                    pInfo->pRasEntry,
                    &(pInfo->dwEntrySize),
                    NULL,
                    NULL);
        if (dwErr != NO_ERROR)
        {
            break;
        }

        // Find out how big the custom auth data needs
        // to be
        dwErr = RasGetCustomAuthDataW (
                    pInfo->pszPhonebookPath,
                    pIfObject->lpwsInterfaceName,
                    NULL,
                    &(pInfo->dwCustAuthDataSize));
        if ( (dwErr != NO_ERROR) &&
             (dwErr != ERROR_BUFFER_TOO_SMALL)
           )
        {
            break;
        }

        dwErr = NO_ERROR;
        if ( pInfo->dwCustAuthDataSize ) 
        {
            // Allocate the custom auth data
            //
            pInfo->lpbCustAuthData = 
                RpbkAlloc(pInfo->dwCustAuthDataSize, TRUE);
                
            if (pInfo->lpbCustAuthData == NULL)
            {
                dwErr = ERROR_NOT_ENOUGH_MEMORY;
                break;
            }

            // Read in the ras entry
            //
            dwErr = RasGetCustomAuthDataW(
                        pInfo->pszPhonebookPath,
                        pIfObject->lpwsInterfaceName,
                        pInfo->lpbCustAuthData,
                        &(pInfo->dwCustAuthDataSize));
            if (dwErr != NO_ERROR)
            {
                break;
            }
        }

        // Assign the return value
        *phEntry = (HANDLE)pInfo;
        
    } while (FALSE);         

    // Cleanup
    {
        if (dwErr != NO_ERROR)
        {
            RpbkFreeEntryInfo(pInfo);
        }
    }
    
    return dwErr;
}

//
// Cleans up the data returned by RpbkOpen* functions
//
DWORD 
RpbkCloseEntry( 
    IN HANDLE hEntry )
{
    RpbkFreeEntryInfo((RPBK_ENTRY_INFO*)hEntry);
    
    return NO_ERROR;
}

//
// Writes out the router phonebook entry based on the 
// the given interface
//
DWORD
RpbkSetEntry( 
    IN  DWORD            dwLevel,
    IN  LPBYTE           pInterfaceData )
{
    MPRI_INTERFACE_2* pIf2   = (MPRI_INTERFACE_2*)pInterfaceData;
    LPRASENTRY       pEntry = NULL;
    PWCHAR           pszAlternates = NULL, pszPath = NULL;
    DWORD            dwErr = NO_ERROR, dwSize;
    LPWSTR           pszAltSrc = NULL;
    
    // Validate parameters
    if (!pIf2)
    {
        return ERROR_INVALID_PARAMETER;
    }

    // Allocate the ras entry structure
    //
    dwSize = sizeof(RASENTRY);
    if (pIf2->dwAlternatesOffset)
    {
        pszAltSrc = (LPWSTR)
            ((ULONG_PTR)(pIf2) + (ULONG_PTR)(pIf2->dwAlternatesOffset));
            
        dwSize += RpbkGetMultiSzSize(pszAltSrc);
                
    }
    pEntry = RpbkAlloc(dwSize, TRUE);
    if (pEntry == NULL)
    {
        return ERROR_NOT_ENOUGH_MEMORY;
    }

    // Assign all assignable fields
    //
    pEntry->dwfOptions                  = pIf2->dwfOptions;
    *((DWORD*)&(pEntry->ipaddr))        = pIf2->ipaddr;
    *((DWORD*)&(pEntry->ipaddrDns))     = pIf2->ipaddrDns;
    *((DWORD*)&(pEntry->ipaddrDnsAlt))  = pIf2->ipaddrDnsAlt;
    *((DWORD*)&(pEntry->ipaddrWins))    = pIf2->ipaddrWins;
    *((DWORD*)&(pEntry->ipaddrWinsAlt)) = pIf2->ipaddrWinsAlt;
    pEntry->dwfNetProtocols             = pIf2->dwfNetProtocols;
    pEntry->dwChannels                  = pIf2->dwChannels;
    pEntry->dwSubEntries                = pIf2->dwSubEntries;
    pEntry->dwDialMode                  = pIf2->dwDialMode;
    pEntry->dwDialExtraPercent          = pIf2->dwDialExtraPercent;
    pEntry->dwDialExtraSampleSeconds    = pIf2->dwDialExtraSampleSeconds;
    pEntry->dwHangUpExtraPercent        = pIf2->dwHangUpExtraPercent;
    pEntry->dwHangUpExtraSampleSeconds  = pIf2->dwHangUpExtraSampleSeconds;
    pEntry->dwIdleDisconnectSeconds     = pIf2->dwIdleDisconnectSeconds;
    pEntry->dwType                      = pIf2->dwType;
    pEntry->dwEncryptionType            = pIf2->dwEncryptionType;
    pEntry->dwCustomAuthKey             = pIf2->dwCustomAuthKey;
    pEntry->dwVpnStrategy               = pIf2->dwVpnStrategy;
    pEntry->guidId                      = pIf2->guidId;

    // Copy all copyable fields
    //
    wcscpy(pEntry->szLocalPhoneNumber,  pIf2->szLocalPhoneNumber);
    wcscpy(pEntry->szDeviceType,        pIf2->szDeviceType);
    wcscpy(pEntry->szDeviceName,        pIf2->szDeviceName);
    wcscpy(pEntry->szX25PadType,        pIf2->szX25PadType);
    wcscpy(pEntry->szX25Address,        pIf2->szX25Address);
    wcscpy(pEntry->szX25Facilities,     pIf2->szX25Facilities);
    wcscpy(pEntry->szX25UserData,       pIf2->szX25UserData);

    do 
    {
        // Copy the alternates list
        //
        if (pIf2->dwAlternatesOffset)
        {
            pEntry->dwAlternateOffset = sizeof(RASENTRY);
                 
            pszAlternates = (PWCHAR)
                ((ULONG_PTR)(pEntry) + 
                 (ULONG_PTR)(pEntry->dwAlternateOffset));
                 
            dwErr = RpbkCopyMultiSz(pszAlternates, pszAltSrc);
            if (dwErr != NO_ERROR)
            {
                break;
            }
        }
        else
        {
            pEntry->dwAlternateOffset = 0;
        }

        // Remove any unsupported options that may have made
        // it in. (shouldn't be any)
        //
        dwErr = RpbkConformEntry(pEntry);
        if (dwErr != NO_ERROR)
        {
            break;
        }

        // Map MPRIO_IpSecPreSharedKey to RASEO_PreSharedKey
        // 
        //
        if(pIf2->dwfOptions & MPRIO_IpSecPreSharedKey)
        {   
            pEntry->dwfOptions &= ~(MPRIO_IpSecPreSharedKey);
            pEntry->dwfOptions2 |= RASEO2_UsePreSharedKey;
        }

        // Discover the phonebook path
        dwErr = RpbkGetPhonebookPath(&pszPath);
        if (dwErr != NO_ERROR)
        {
            break;
        }

        // Save the entry
        dwErr = RasSetEntryPropertiesW(
                    pszPath,
                    pIf2->wszInterfaceName,
                    pEntry,
                    dwSize, 
                    NULL,
                    0);
        if (dwErr != NO_ERROR)
        {   
            break;
        }

        // Save the custom auth options
        //
        dwErr = RasSetCustomAuthDataW( 
                    pszPath,
                    pIf2->wszInterfaceName,
                    (LPBYTE)(pIf2 + 1),
                    pIf2->dwCustomAuthDataSize);
        if ( dwErr != NO_ERROR )                    
        {
            break;
        }
        
    } while (FALSE);        

    // Cleanup
    {
        if (pEntry)
        {
            RpbkFree(pEntry);
        }
        if (pszPath)
        {
            RpbkFree(pszPath);
        }
    }

    return dwErr;
}
    
//
// Deletes the given entry from the phonebook
//
DWORD 
RpbkDeleteEntry( 
    IN PWCHAR pszInterfaceName )
{
    PWCHAR           pszPath = NULL;
    DWORD            dwErr = NO_ERROR;
    
    do
    {
        // Get the phonebook path
        //
        dwErr = RpbkGetPhonebookPath(&pszPath);
        if (dwErr != NO_ERROR)
        {
            break;
        }

        // Delete the entry
        //
        dwErr = RasDeleteEntry(pszPath, pszInterfaceName);
        if (dwErr != NO_ERROR)
        {
            break;
        }
        
    } while (FALSE);
    
    // Cleanup
    {
        if ( pszPath )
        {
            RpbkFree(pszPath);
        }
    }
    
    return dwErr;
}

//
// Calculates the required size of a buffer to hold interface information
// at the given level based on the given entry or sub entry.
//
DWORD
RpbkEntryToIfDataSize(
    IN  HANDLE  hEntry, 
    IN  DWORD   dwLevel,
    OUT LPDWORD lpdwcbSizeOfData )
{
    RPBK_ENTRY_INFO* pInfo = (RPBK_ENTRY_INFO*)hEntry;
    DWORD dwSize;
    PWCHAR pszAlternates = NULL;

    // Validate parameters
    if (!pInfo)
    {
        return ERROR_INVALID_PARAMETER;
    }

    // Initialize
    *lpdwcbSizeOfData = 0;

    // For level 2, the size is the size of the level 2 
    // structure plus the size of the alternate phone list   
    if (dwLevel != 2)
    {
        return ERROR_INVALID_LEVEL;
    }

    // Initailize the size to the base size
    //
    dwSize = sizeof(MPR_INTERFACE_2);

    //  Add on the size of the custom auth data
    //
    dwSize += pInfo->dwCustAuthDataSize;
    
    //  Add on the size of the alternates list
    //
    if (pInfo->pRasEntry->dwAlternateOffset)
    {
        pszAlternates = (PWCHAR)
            ((ULONG_PTR)(pInfo->pRasEntry) + 
             (ULONG_PTR)(pInfo->pRasEntry->dwAlternateOffset));
             
        dwSize += RpbkGetMultiSzSize(pszAlternates);
    }

    // Assign the return value
    //
    *lpdwcbSizeOfData = dwSize;
    
    return NO_ERROR;
}

//
// Populates the ras portion of the given interface info blob based on the 
// level and entry or subentry.
//
DWORD
RpbkEntryToIfData( 
    IN  HANDLE           hEntry, 
    IN  DWORD            dwLevel,
    OUT LPBYTE           pInterfaceData )
{
    RPBK_ENTRY_INFO* pInfo  = (RPBK_ENTRY_INFO*)hEntry;
    MPRI_INTERFACE_2* pIf2   = (MPRI_INTERFACE_2*)pInterfaceData;
    LPRASENTRY       pEntry = NULL;
    PWCHAR           pszSrc, pszDst;
    DWORD            dwErr = NO_ERROR, dwOffset = 0;
    
    // Validate parameters
    if (!pInfo || !pIf2)
    {
        return ERROR_INVALID_PARAMETER;
    }

    // Alias the ras entry
    //
    pEntry = pInfo->pRasEntry;

    // Assign all assignable fields
    //
    pIf2->dwfOptions                = pEntry->dwfOptions;
    pIf2->ipaddr                    = *((DWORD*)&(pEntry->ipaddr));
    pIf2->ipaddrDns                 = *((DWORD*)&(pEntry->ipaddrDns));
    pIf2->ipaddrDnsAlt              = *((DWORD*)&(pEntry->ipaddrDnsAlt));
    pIf2->ipaddrWins                = *((DWORD*)&(pEntry->ipaddrWins));
    pIf2->ipaddrWinsAlt             = *((DWORD*)&(pEntry->ipaddrWinsAlt));
    pIf2->dwfNetProtocols           = pEntry->dwfNetProtocols;
    pIf2->dwChannels                = pEntry->dwChannels;
    pIf2->dwSubEntries              = pEntry->dwSubEntries;
    pIf2->dwDialMode                = pEntry->dwDialMode;
    pIf2->dwDialExtraPercent        = pEntry->dwDialExtraPercent;
    pIf2->dwDialExtraSampleSeconds  = pEntry->dwDialExtraSampleSeconds;
    pIf2->dwHangUpExtraPercent      = pEntry->dwHangUpExtraPercent;
    pIf2->dwHangUpExtraSampleSeconds= pEntry->dwHangUpExtraSampleSeconds;
    pIf2->dwIdleDisconnectSeconds   = pEntry->dwIdleDisconnectSeconds;
    pIf2->dwType                    = pEntry->dwType;
    pIf2->dwEncryptionType          = pEntry->dwEncryptionType;
    pIf2->dwCustomAuthKey           = pEntry->dwCustomAuthKey;
    pIf2->dwVpnStrategy             = pEntry->dwVpnStrategy;
    pIf2->guidId                    = pEntry->guidId;

    // Copy all copyable fields
    //
    wcscpy(pIf2->szLocalPhoneNumber,  pEntry->szLocalPhoneNumber);
    wcscpy(pIf2->szDeviceType,        pEntry->szDeviceType);
    wcscpy(pIf2->szDeviceName,        pEntry->szDeviceName);
    wcscpy(pIf2->szX25PadType,        pEntry->szX25PadType);
    wcscpy(pIf2->szX25Address,        pEntry->szX25Address);
    wcscpy(pIf2->szX25Facilities,     pEntry->szX25Facilities);
    wcscpy(pIf2->szX25UserData,       pEntry->szX25UserData);

    // Append the custom auth data to the end of the
    // structure.
    //
    dwOffset = sizeof(MPRI_INTERFACE_2);
    if ( pInfo->dwCustAuthDataSize )
    {
        pIf2->dwCustomAuthDataSize = 
            pInfo->dwCustAuthDataSize;
        
        pIf2->dwCustomAuthDataOffset = TRUE;
        
        CopyMemory(
            pIf2 + 1, 
            pInfo->lpbCustAuthData, 
            pInfo->dwCustAuthDataSize);
    }

    // Append the alternates list
    //
    dwOffset += pInfo->dwCustAuthDataSize;
    if (pEntry->dwAlternateOffset)
    {
        pIf2->dwAlternatesOffset = TRUE;
            
        pszSrc = (PWCHAR)((ULONG_PTR)(pEntry) + 
             (ULONG_PTR)(pEntry->dwAlternateOffset));

        pszDst = (PWCHAR)((ULONG_PTR)(pIf2) + (ULONG_PTR)dwOffset);
             
        dwErr = RpbkCopyMultiSz(pszDst, pszSrc);
        if (dwErr != NO_ERROR)
        {
            return dwErr;
        }
    }
    else
    {
        pIf2->dwAlternatesOffset = 0;
    }

    // Remove any unsupported options that may have made
    // it in. (shouldn't be any)
    //
    dwErr = RpbkConformIfData(dwLevel, pInterfaceData);
    if (dwErr != NO_ERROR)
    {
        return dwErr;
    }

    if(pEntry->dwfOptions2 & RASEO2_UsePreSharedKey)
    {
        pIf2->dwfOptions |= MPRIO_IpSecPreSharedKey;
    }

    return NO_ERROR;
}

// 
// Reads in the router phonebook sub entry associated with
// the given interface
//
DWORD 
RpbkOpenSubEntry(
    IN  ROUTER_INTERFACE_OBJECT* pIfObject, 
    IN  DWORD                    dwIndex,    
    OUT PHANDLE                  phSubEntry )
{
    RPBK_SUBENTRY_INFO * pInfo = NULL;
    DWORD dwErr = NO_ERROR, dwSize;

    do {
        // Initialize
        *phSubEntry = NULL;
    
        // Allocate the control structure
        //
        pInfo = (RPBK_SUBENTRY_INFO*) 
            RpbkAlloc(sizeof(RPBK_SUBENTRY_INFO), TRUE);
            
        if (pInfo == NULL)
        {
            dwErr = ERROR_NOT_ENOUGH_MEMORY;
            break;
        }

        // Get the phonebook path
        //
        dwErr = RpbkGetPhonebookPath(&(pInfo->pszPhonebookPath));
        if (dwErr != NO_ERROR)
        {
            break;
        }

        // Find out how big the ras entry needs to be
        //
        dwErr = RasGetSubEntryPropertiesW(
                    pInfo->pszPhonebookPath,
                    pIfObject->lpwsInterfaceName,
                    dwIndex,
                    NULL,
                    &(pInfo->dwSize),
                    NULL,
                    NULL);
        if (dwErr != ERROR_BUFFER_TOO_SMALL)
        {
            break;
        }

        // Allocate the ras entry structure
        //
        pInfo->pRasSubEntry = 
            (LPRASSUBENTRY) RpbkAlloc(pInfo->dwSize, TRUE);
            
        if (pInfo->pRasSubEntry == NULL)
        {
            dwErr = ERROR_NOT_ENOUGH_MEMORY;
            break;
        }

        // Read in the ras entry
        //
        pInfo->pRasSubEntry->dwSize = sizeof(RASSUBENTRY);
        dwErr = RasGetSubEntryProperties(
                    pInfo->pszPhonebookPath,
                    pIfObject->lpwsInterfaceName,
                    dwIndex,
                    pInfo->pRasSubEntry,
                    &(pInfo->dwSize),
                    NULL,
                    NULL);
        if (dwErr != NO_ERROR)
        {
            break;
        }

        // Assign the return value
        *phSubEntry = (HANDLE)pInfo;
        
    } while (FALSE);         

    // Cleanup
    {
        if (dwErr != NO_ERROR)
        {
            RpbkFreeSubEntryInfo(pInfo);
        }
    }
    
    return dwErr;
}

//
// Cleans up the data returned by RpbkOpen* functions
//
DWORD 
RpbkCloseSubEntry( 
    IN HANDLE hSubEntry )
{
    RpbkFreeSubEntryInfo((RPBK_SUBENTRY_INFO*)hSubEntry);
    
    return NO_ERROR;
}

DWORD
RpbkSetSubEntry( 
    IN  PWCHAR pszInterface,
    IN  DWORD  dwIndex,
    IN  DWORD  dwLevel,
    OUT LPBYTE pDevData )
{
    MPR_DEVICE_0*    pDev0  = (MPR_DEVICE_0*)pDevData;
    MPR_DEVICE_1*    pDev1  = (MPR_DEVICE_1*)pDevData;
    LPRASSUBENTRY    pSubEntry = NULL;
    PWCHAR           pszAlternates = NULL, pszPath = NULL;
    DWORD            dwErr = NO_ERROR, dwSize;
    
    // Validate parameters
    if (! pDev0)
    {
        return ERROR_INVALID_PARAMETER;
    }

    // Allocate the ras entry structure
    //
    dwSize = sizeof(RASSUBENTRY);
    if ((dwLevel == 1) && (pDev1->szAlternates))
    {
        dwSize += RpbkGetMultiSzSize(pDev1->szAlternates);
    }
    pSubEntry = RpbkAlloc(dwSize, TRUE);
    if (pSubEntry == NULL)
    {
        return ERROR_NOT_ENOUGH_MEMORY;
    }
    pSubEntry->dwSize = sizeof(RASSUBENTRY);

    do 
    {
        // Copy all copyable fields
        //
        if ( dwLevel == 0 )
        {
            wcscpy(pSubEntry->szDeviceType,  pDev0->szDeviceType);
            wcscpy(pSubEntry->szDeviceName,  pDev0->szDeviceName);
        }            
        
        if ( dwLevel == 1 )
        {
            wcscpy(pSubEntry->szDeviceType,  pDev1->szDeviceType);
            wcscpy(pSubEntry->szDeviceName,  pDev1->szDeviceName);
            wcscpy(pSubEntry->szLocalPhoneNumber,  pDev1->szLocalPhoneNumber);

            if (pDev1->szAlternates)
            {
                pSubEntry->dwAlternateOffset = sizeof(RASSUBENTRY);
                     
                pszAlternates = (PWCHAR)
                    ((ULONG_PTR)(pSubEntry) + 
                     (ULONG_PTR)(pSubEntry->dwAlternateOffset));
                     
                dwErr = RpbkCopyMultiSz(pszAlternates, pDev1->szAlternates);
                if (dwErr != NO_ERROR)
                {
                    break;
                }
            }
            else
            {
                pSubEntry->dwAlternateOffset = 0;
            }
        }

        // Discover the phonebook path
        dwErr = RpbkGetPhonebookPath(&pszPath);
        if (dwErr != NO_ERROR)
        {
            break;
        }

        // Save the entry
        dwErr = RasSetSubEntryPropertiesW(
                    pszPath,
                    pszInterface,
                    dwIndex,
                    pSubEntry,
                    dwSize, 
                    NULL,
                    0);
        if (dwErr != NO_ERROR)
        {   
            break;
        }

    } while (FALSE);        

    // Cleanup
    {
        if (pSubEntry)
        {
            RpbkFree(pSubEntry);
        }
        if (pszPath)
        {
            RpbkFree(pszPath);
        }
    }

    return dwErr;
}
    
DWORD
RpbkSubEntryToDevDataSize(
    IN  HANDLE  hSubEntry, 
    IN  DWORD   dwLevel,
    OUT LPDWORD lpdwcbSizeOfData )
{
    RPBK_SUBENTRY_INFO* pInfo = (RPBK_SUBENTRY_INFO*)hSubEntry;
    DWORD dwSize = 0;
    PWCHAR pszAlternates = NULL;

    // Validate parameters
    if (pInfo == NULL)
    {
        return ERROR_INVALID_PARAMETER;
    }

    // Initialize
    *lpdwcbSizeOfData = 0;

    // Initailize the size 
    //
    if ( dwLevel == 0 )
    {
        dwSize = sizeof(MPR_DEVICE_0);
    }
    else
    {
        dwSize = sizeof(MPR_DEVICE_1);
        
        //  Add on the size of the alternates list
        //
        if (pInfo->pRasSubEntry->dwAlternateOffset)
        {
            pszAlternates = (PWCHAR)
                ((ULONG_PTR)(pInfo->pRasSubEntry) + 
                 (ULONG_PTR)(pInfo->pRasSubEntry->dwAlternateOffset));
                 
            dwSize += RpbkGetMultiSzSize(pszAlternates);
        }
    }

    // Assign the return value
    //
    *lpdwcbSizeOfData = dwSize;
    
    return NO_ERROR;
}
    
DWORD
RpbkSubEntryToDevData( 
    IN  HANDLE  hSubEntry, 
    IN  DWORD   dwLevel,
    OUT LPBYTE  pDeviceData )
{
    RPBK_SUBENTRY_INFO* pInfo  = (RPBK_SUBENTRY_INFO*)hSubEntry;
    MPR_DEVICE_0*    pDev0  = (MPR_DEVICE_0*)pDeviceData;
    MPR_DEVICE_1*    pDev1  = (MPR_DEVICE_1*)pDeviceData;
    LPRASSUBENTRY    pSubEntry = NULL;
    PWCHAR           pszAlternates = NULL;
    DWORD            dwErr = NO_ERROR, dwOffset = 0;
    
    // Validate parameters
    if (!pInfo || !pDev0)
    {
        return ERROR_INVALID_PARAMETER;
    }

    // Alias the ras entry
    //
    pSubEntry = pInfo->pRasSubEntry;

    // Copy all copyable fields
    //
    if ( dwLevel == 0 )
    {
        wcscpy(pDev0->szDeviceType,       pSubEntry->szDeviceType);
        wcscpy(pDev0->szDeviceName,       pSubEntry->szDeviceName);
    }        
    
    else if ( dwLevel == 1 )
    {
        wcscpy(pDev1->szDeviceType,       pSubEntry->szDeviceType);
        wcscpy(pDev1->szDeviceName,       pSubEntry->szDeviceName);
        wcscpy(pDev1->szLocalPhoneNumber,  pSubEntry->szLocalPhoneNumber);
        
        // Append the custom auth data to the end of the
        // structure.
        //
        dwOffset += sizeof(MPR_DEVICE_1);
        if (pSubEntry->dwAlternateOffset)
        {
            pDev1->szAlternates = (PWCHAR)
                ((ULONG_PTR)(pDev1) + dwOffset);
                
            pszAlternates = (PWCHAR)
                ((ULONG_PTR)(pSubEntry) + 
                 (ULONG_PTR)(pSubEntry->dwAlternateOffset));
                 
            dwErr = RpbkCopyMultiSz(pDev1->szAlternates, pszAlternates);
            if (dwErr != NO_ERROR)
            {
                return dwErr;
            }
        }
        else
        {
            pDev1->szAlternates = NULL;
        }
    }
    
    return NO_ERROR;
}





