/*++

    Intel Corporation Proprietary Information
    Copyright (c) 1995 Intel Corporation

    This listing is supplied under the terms of a license agreement with
    Intel Corporation and may not be used, copied, nor disclosed except in
    accordance with the terms of that agreeement.

Module Name:

    nspinstl.cpp

Abstract:

    This module contains the entry points for name space provider
    installation/configuration.

Author:

    Dirk Brandewie (dirk@mink.intel.com) 04-December-1995

Notes:

    $Revision:   1.11  $

    $Modtime:   08 Mar 1996 15:46:16  $


Revision History:

    most-recent-revision-date email-name
        description

    04-Dec-1995 dirk@mink.intel.com
        Initial revision

--*/

#include "precomp.h"

// Structure used as context value for catalog enumerations
typedef struct
{
    GUID            ProviderId;
    PNSCATALOGENTRY CatalogItem;
} GUID_MATCH_CONTEXT, *PGUID_MATCH_CONTEXT;


BOOL
GuidMatcher(
    IN PVOID             PassBack,
    IN PNSCATALOGENTRY   CatalogItem
    )
#ifndef _WIN64_32
/*++

Routine Description:

    Enumeration proceedure for WSC calls. Finds catalog item
    with matching provider id

Arguments:

    PassBack - A context value passed to EunerateCatalogItems. It is really a
               pointer to a GUID_MATCH_CONTEXT struct.

    CatalogItem - A pointer to a catalog item to be inspected.

Return Value:

    TRUE if the Enumeration should continue else FALSE.

--*/
{
    PGUID_MATCH_CONTEXT Context;
    BOOL                ContinueEnumeration =TRUE;

    Context = (PGUID_MATCH_CONTEXT)PassBack;

    if (Context->ProviderId == *(CatalogItem->GetProviderId ())){
        Context->CatalogItem = CatalogItem;
        ContinueEnumeration = FALSE;
    } //if

    return(ContinueEnumeration);
}
#else
;
#endif


INT WSAAPI
#ifndef _WIN64_32
WSCInstallNameSpace (
#else
WSCInstallNameSpace32 (
#endif
    IN  LPWSTR  lpszIdentifier,
    IN  LPWSTR  lpszPathName,
    IN  DWORD   dwNameSpace,
    IN  DWORD   dwVersion,
    IN  LPGUID  lpProviderId
    )
/*++

Routine Description:

    WSCInstallNameSpace() is used to install a name space provider. For
    providers that are able to support multiple names spaces, this function
    must be called once for every name space supported, and a unique provider
    ID must be supplied each time.

Arguments:

    lpszIdentifier - Display string for the provider.

    lpszPathname - Points to a path to the providers DLL image which
                   follows  the usual rules for path resolution. This path may
                   contain embedded environment strings (such as
                   %SystemRoot%). Such environment strings are expanded
                   whenever the WinSock 2 DLL needs to subsequently load
                   theprovider DLL on behalf of an application. After any
                   embedded environment strings are expanded, the WinSock 2 DLL
                   passes the resulting string into the LoadLibrary() function
                   to load the provider into memory.

    dwNameSpace - Specifies the name space supported by this provider.

    dwVersion - Specifies the version number of the provider.

    bStoresAllServiceClassInfo - Specifies that this provider supports the
                                 storage  of service class schema information
                                 for all service classes across all namespaces.
                                 The Winsock DLL will then use this provider to
                                 get all of its classinfo information rather
                                 than asking each individual provider.

    lpProviderId - A unique identifier for this provider.  This GUID should be
                   generated by UUIDGEN.EXE.

Return Value:

    The function should return NO_ERROR (0) if the routine succeeds.  It should
    return SOCKET_ERROR (-1) if the routine fails and it must set the
    appropriate error code using SetLastError().
--*/
{
    INT             ReturnCode;
    PNSCATALOG      Catalog =NULL;
    PNSCATALOGENTRY Item =NULL;
    HKEY            registry_root;

    registry_root = OpenWinSockRegistryRoot();
    if (NULL == registry_root) {
        DEBUGF(
            DBG_ERR,
            ("Opening registry root\n"));
        SetLastError(WSASYSCALLFAILURE);
        return(SOCKET_ERROR);
    }

    //
    // Check the current protocol catalog key. If it doesn't match
    // the expected value, blow away the old key and update the
    // stored value.
    //

    ValidateCurrentCatalogName(
        registry_root,
        WINSOCK_CURRENT_NAMESPACE_CATALOG_NAME,
        NSCATALOG::GetCurrentCatalogName()
        );

    TRY_START(guard_memalloc){
        GUID_MATCH_CONTEXT context;


        Catalog = new NSCATALOG;
        if (NULL == Catalog){
            ReturnCode = WSA_NOT_ENOUGH_MEMORY;
            TRY_THROW(guard_memalloc);
        } //item

        Item = new NSCATALOGENTRY;
        if (NULL == Item){
            ReturnCode = WSA_NOT_ENOUGH_MEMORY;
            TRY_THROW(guard_memalloc);
        } //if


        __try {
            context.ProviderId = *lpProviderId;
            ReturnCode = Item->InitializeFromValues(
                lpszPathName,
                lpszIdentifier,
                lpProviderId,
                dwNameSpace,
                dwVersion
                );
        }
        __except (WS2_EXCEPTION_FILTER()) {
            ReturnCode = WSAEFAULT;
            TRY_THROW(guard_memalloc);
        }

        if (ERROR_SUCCESS != ReturnCode){
            TRY_THROW(guard_memalloc);
        } //if

#ifndef _WIN64_32
        ReturnCode = Catalog->InitializeFromRegistry(
            registry_root,  // ParentKey
            NULL            // ChangeEvent
            );
#else
        ReturnCode = Catalog->InitializeFromRegistry32(
            registry_root
            );
#endif

        if (ERROR_SUCCESS != ReturnCode){
            TRY_THROW(guard_memalloc);
        } //if

        context.CatalogItem = NULL;
        Catalog->EnumerateCatalogItems(
            GuidMatcher,
            &context
            );

        if (context.CatalogItem != NULL){
            ReturnCode = WSAEINVAL;
            TRY_THROW(guard_memalloc);
        } //if

        Catalog->AppendCatalogItem(
            Item
            );
        Item = NULL;  // item deletion is now covered by catalog

        ReturnCode = Catalog->WriteToRegistry();
        if (ReturnCode!=ERROR_SUCCESS) {
            TRY_THROW (guard_memalloc);
        }
        delete Catalog;

    } TRY_CATCH(guard_memalloc){
        assert (ReturnCode!=ERROR_SUCCESS);

        if (Item){
            Item->Dereference ();
        } //if

        if (Catalog){
            delete Catalog;
        } //if

    } TRY_END(guard_memalloc);


    CloseWinSockRegistryRoot(registry_root);

    if (ReturnCode == ERROR_SUCCESS) {
        HANDLE hHelper;

        //
        // Alert all interested apps of change via the notification method
        //


        if (WahOpenNotificationHandleHelper( &hHelper )==ERROR_SUCCESS) {
            WahNotifyAllProcesses( hHelper );
            WahCloseNotificationHandleHelper( hHelper );
        }
        else {
            // This is non-fatal and catalog was updated anyway
        }

        return ERROR_SUCCESS;
    }
    else {
        SetLastError(ReturnCode);
        return SOCKET_ERROR;
    }
}





INT
WSAAPI
#ifndef _WIN64_32
WSCUnInstallNameSpace (
#else
WSCUnInstallNameSpace32 (
#endif
    IN  LPGUID  lpProviderId
    )
/*++

Routine Description:

    WSCUnInstallNameSpace() is used to uninstall the indicated name space
    provider.

Arguments:

    lpProviderId - The unique identifier for a provider.

Return Value:

    The function should return NO_ERROR (0) if the routine succeeds.  It should
    return SOCKET_ERROR (-1) if the routine fails and it must set the
    appropriate error code using SetLastError().
--*/
{
    INT                  ReturnCode;
    PNSCATALOG           Catalog =NULL;
    HKEY                 registry_root;

    //Open and Initialize a name space jprovider catalog
    registry_root = OpenWinSockRegistryRoot();
    if (NULL == registry_root) {
        DEBUGF(
            DBG_ERR,
            ("Opening registry root\n"));
        SetLastError(WSASYSCALLFAILURE);
        return(SOCKET_ERROR);
    }

    TRY_START(guard_memalloc){
        GUID_MATCH_CONTEXT     context;
        Catalog = new NSCATALOG;
        if (NULL == Catalog){
            ReturnCode = WSA_NOT_ENOUGH_MEMORY;
            TRY_THROW(guard_memalloc);
        } //item


#ifndef _WIN64_32
        ReturnCode = Catalog->InitializeFromRegistry(
                            registry_root,  // ParentKey
                            NULL            // ChangeEvent
                            );
#else
        ReturnCode = Catalog->InitializeFromRegistry32(
                            registry_root  // ParentKey
                            );
#endif
        if (ERROR_SUCCESS != ReturnCode){
            TRY_THROW(guard_memalloc);
        } //if

        __try {
            context.ProviderId = *lpProviderId;
        }
        __except (WS2_EXCEPTION_FILTER()) {
            ReturnCode = WSAEFAULT;
            TRY_THROW(guard_memalloc);
        }

        context.CatalogItem = NULL;
        Catalog->EnumerateCatalogItems(
            GuidMatcher,
            &context);

        if (context.CatalogItem!=NULL) {
            Catalog->RemoveCatalogItem (context.CatalogItem);
            context.CatalogItem->Dereference ();
        }
        else {
            ReturnCode = WSAEINVAL;
            TRY_THROW(guard_memalloc);
        }

        ReturnCode = Catalog->WriteToRegistry();

        if (ERROR_SUCCESS != ReturnCode){
            TRY_THROW(guard_memalloc);
        } //if

        delete Catalog;
    } TRY_CATCH(guard_memalloc){
        assert (ReturnCode!=ERROR_SUCCESS);
        if (Catalog){
            delete Catalog;
        } //if
    } TRY_END(guard_memalloc);

    CloseWinSockRegistryRoot(registry_root);

    if (ERROR_SUCCESS == ReturnCode){
        HANDLE hHelper;

        //
        // Alert all interested apps of change via the notification method
        //

        if (WahOpenNotificationHandleHelper( &hHelper) == ERROR_SUCCESS) {
            WahNotifyAllProcesses( hHelper );
            WahCloseNotificationHandleHelper( hHelper );
        }
        else {
            //
            // This in non-fatal and catalog was updated anyway.
            //
        }

        return ERROR_SUCCESS;
    } 
    else {
        SetLastError(ReturnCode);
        return SOCKET_ERROR;
    }
}




INT WSAAPI
#ifndef _WIN64_32
WSCEnableNSProvider (
#else
WSCEnableNSProvider32 (
#endif
    IN  LPGUID  lpProviderId,
    IN  BOOL    fEnable
    )
/*++

Routine Description:

    WSCEnableNSProvider() is used to change the state of a given name space
    provider.  This function is intended to be used by the control panel applet
    to change the state of the providers.  An ISV should not just blindly
    de-activate another ISV's provider in order to activate their own.  This
    choice should be left up to the user.description-of-function

Arguments:

    lpProviderId - The unique identifier for this provider.

    fEnable - If TRUE, the provider is set to the active state.  If FALSE, the
              provider is disabled and will not be available for query
              operations or service registration.

Return Value:

    The function should return NO_ERROR (0) if the routine succeeds.  It should
    return SOCKET_ERROR (-1) if the routine fails and it must set the
    appropriate error code using SetLastError().

--*/
{
    INT                  ReturnCode;
    PNSCATALOG           Catalog =NULL;
    HKEY                 registry_root;

    registry_root = OpenWinSockRegistryRoot();
    if (NULL == registry_root) {
        DEBUGF(
            DBG_ERR,
            ("Opening registry root\n"));
        SetLastError(WSASYSCALLFAILURE);
        return(SOCKET_ERROR);
    }

    TRY_START(guard_memalloc){
        GUID_MATCH_CONTEXT     context;

        Catalog = new NSCATALOG;
        if (NULL == Catalog){
            ReturnCode = WSA_NOT_ENOUGH_MEMORY;
            TRY_THROW(guard_memalloc);
        } //item


#ifndef _WIN64_32
        ReturnCode = Catalog->InitializeFromRegistry(
                        registry_root,  // ParentKey
                        NULL            // ChangeEvent
                        );
#else
        ReturnCode = Catalog->InitializeFromRegistry32(
                        registry_root   // ParentKey
                        );
#endif

        if (ERROR_SUCCESS != ReturnCode){
            TRY_THROW(guard_memalloc);
        } //if

        __try {
            context.ProviderId = *lpProviderId;
        }
        __except (WS2_EXCEPTION_FILTER()) {
            ReturnCode = WSAEFAULT;
            TRY_THROW(guard_memalloc);
        }

        context.CatalogItem = NULL;
        Catalog->EnumerateCatalogItems(
            GuidMatcher,
            &context);

        if (context.CatalogItem!=NULL) {
            context.CatalogItem->Enable (fEnable ? (BOOLEAN)TRUE : (BOOLEAN)FALSE);
        }
        else {
            ReturnCode = WSAEINVAL;
            TRY_THROW(guard_memalloc);
        }

        ReturnCode = Catalog->WriteToRegistry();
        if (ReturnCode != ERROR_SUCCESS) {
            TRY_THROW(guard_memalloc);
        }

        delete Catalog;

    } TRY_CATCH(guard_memalloc){

        assert (ReturnCode!=ERROR_SUCCESS);
        if (Catalog){
            delete Catalog;
        } //if
    } TRY_END(guard_memalloc);

    CloseWinSockRegistryRoot(registry_root);

    if (ERROR_SUCCESS == ReturnCode){
        HANDLE hHelper;

        //
        // Alert all interested apps of change via the notification method
        //

        if (WahOpenNotificationHandleHelper( &hHelper) == ERROR_SUCCESS) {
            WahNotifyAllProcesses( hHelper );
            WahCloseNotificationHandleHelper( hHelper );
        }
        else {
            //
            // This in non-fatal and catalog was updated anyway.
            //
        }

        return ERROR_SUCCESS;
    } 
    else {
        SetLastError(ReturnCode);
        return SOCKET_ERROR;
    }

}


// The  following  type is used to pass context back and forth to an enumerator
// iteration procedure when removing all items form the catalog
typedef struct {
    IN  DWORD               Count;      // Number of ids / size of item array
    IN  GUID                *ProviderIds;// Array of provider id's to match against
    OUT PNSCATALOGENTRY     *Items;     // Array of item pointers to return
    OUT INT                 ErrorCode;
} PROVIDER_SNAP_CONTEXT,  FAR * PPROVIDER_SNAP_CONTEXT;




BOOL
ProviderSnap(
    IN PVOID                PassBack,
    IN PNSCATALOGENTRY      CatalogEntry
    )
#ifndef _WIN64_32
/*++

Routine Description:
    Snaps all the catalog items in the current catalog

Arguments:

    PassBack     - Supplies  a reference to a PROVIDER_SNAP_CONTEXT structure.
                   Returns an array of items in the order specified by catalog
                   id array.

    CatalogEntry - Supplies  to  the client a reference to a PROTO_CATALOG_ITEM
                   structure with values for this item of the enumeration.  The
                   pointer  is  not guaranteed to be valid after this procedure
                   returns, so the client should copy data if required.

Return Value:

    If an item is found that could not be matched to catalog id in the array,
    the function returns FALSE to terminate the iteration,
    otherwise it returns TRUE.
--*/
{
    PPROVIDER_SNAP_CONTEXT      context;
    DWORD                       i;

    context = (PPROVIDER_SNAP_CONTEXT)PassBack;

    __try {
        for (i=0; i<context->Count; i++) {
            if (context->ProviderIds[i]
                    == *(CatalogEntry->GetProviderId ())) {
                assert (context->Items[i]==NULL);
                context->Items[i] = CatalogEntry;
                return TRUE;
            }
        }
    }
    __except (WS2_EXCEPTION_FILTER()) {
        context->ErrorCode = WSAEFAULT;
    }

    return FALSE;
}  // ProviderSnap
#else
;
#endif

int
WSPAPI
#ifndef _WIN64_32
WSCWriteNameSpaceOrder (
#else
WSCWriteNameSpaceOrder32 (
#endif
    IN LPGUID lpProviderId,
    IN DWORD dwNumberOfEntries
    )
/*++

Routine Description:

    Reorder existing WinSock2 name space providers.  The order of the service
    providers determines their priority in being selected for use.  The
    sporder.exe tool will show you the installed provider and their ordering,
    Alternately, WSAEnumNameSpaces(), in conjunction with this function,
    will allow you to write your own tool.

Arguments:

    lpwdCatalogEntryId  [in]
      An array of CatalogEntryId elements as found in the WSAPROTOCOL_INFO
      structure.  The order of the CatalogEntryId elements is the new
      priority ordering for the service providers.

    dwNumberOfEntries  [in]
      The number of elements in the lpwdCatalogEntryId array.


Return Value:

    ERROR_SUCCESS   - the service providers have been reordered.
    WSAEINVAL       - input parameters were bad, no action was taken.
    WSAEFAULT       - CatalogEnryId array is not fully contained within
                        process address space.
    WSATRY_AGAIN    - the routine is being called by another thread or process.
    any registry error code


Comments:

    Here are scenarios in which the WSCWriteProviderOrder function may fail:

      The dwNumberOfEntries is not equal to the number of registered service
      providers.

      The lpwdCatalogEntryId contains an invalid catalog ID.

      The lpwdCatalogEntryId does not contain all valid catalog IDs exactly
      1 time.

      The routine is not able to access the registry for some reason
      (e.g. inadequate user persmissions)

      Another process (or thread) is currently calling the routine.


--*/
{
    INT             errno_result;
    HKEY            registry_root;
    PNSCATALOGENTRY *items = NULL;
    DWORD           i;

    // object protected by "try" block
    PNSCATALOG           catalog = NULL;


    items = new PNSCATALOGENTRY[dwNumberOfEntries];
    if (items==NULL) {
        DEBUGF(
            DBG_ERR,
            ("Allocating items array\n"));
        return WSA_NOT_ENOUGH_MEMORY;
    }

    memset (items, 0, sizeof (PNSCATALOGENTRY)*dwNumberOfEntries);

    errno_result = ERROR_SUCCESS;

    TRY_START(guard_memalloc) {
        PROVIDER_SNAP_CONTEXT context;
        registry_root = OpenWinSockRegistryRoot();
        if (registry_root == NULL) {
            DEBUGF(
                DBG_ERR,
                ("Opening registry root\n"));
            errno_result = WSANO_RECOVERY;
            TRY_THROW(guard_memalloc);
        }

        catalog = new NSCATALOG();
        if (catalog == NULL) {
            errno_result = WSA_NOT_ENOUGH_MEMORY;
            TRY_THROW(guard_memalloc);
        }

#ifndef _WIN64_32
        errno_result = catalog->InitializeFromRegistry(
            registry_root,  // ParentKey
            NULL            // ChangeEvent
            );
#else
        errno_result = catalog->InitializeFromRegistry32(
            registry_root   // ParentKey
            );
#endif
        if (errno_result != ERROR_SUCCESS) {
            TRY_THROW(guard_memalloc);
        }

        context.Items = items;
        context.ProviderIds = lpProviderId;
        context.Count = dwNumberOfEntries;
        context.ErrorCode = ERROR_SUCCESS;

        catalog->EnumerateCatalogItems(
            ProviderSnap,         // Iteration
            & context               // PassBack
            );
        if (context.ErrorCode!=ERROR_SUCCESS) {
            errno_result = context.ErrorCode;
            TRY_THROW(guard_memalloc);
        }

        for (i=0; i<dwNumberOfEntries; i++) {
            if (context.Items[i]!=NULL) {
                //
                // Remove catalog item and add it in the end.
                //
                catalog->RemoveCatalogItem (context.Items[i]);
                catalog->AppendCatalogItem (context.Items[i]);
            }
            else {
                DEBUGF (DBG_ERR,
                    ("Checking item array against catalog, item: %ld.\n",
                    i));
                errno_result = WSAEINVAL;
                TRY_THROW(guard_memalloc);
            }
        } // for i

        errno_result = catalog->WriteToRegistry();
        if (errno_result!=ERROR_SUCCESS) {
            TRY_THROW(guard_memalloc);
        }

        delete catalog;
        CloseWinSockRegistryRoot(registry_root);

    } TRY_CATCH(guard_memalloc) {
        assert (errno_result != ERROR_SUCCESS);
        if (catalog != NULL) {
            delete catalog; // This destroys the items as well
        }
        
        if (registry_root!=NULL) {
            CloseWinSockRegistryRoot(registry_root);
        }
    } TRY_END(guard_memalloc);


    delete items;

    if (errno_result == ERROR_SUCCESS) {
        HANDLE hHelper;

        //
        // Alert all interested apps of change via the notification method
        //

        if (WahOpenNotificationHandleHelper( &hHelper) == ERROR_SUCCESS) {
            WahNotifyAllProcesses( hHelper );
            WahCloseNotificationHandleHelper( hHelper );
        }
        else {
            //
            // This in non-fatal and catalog was updated anyway.
            //
        }
    }

    return errno_result;
}

