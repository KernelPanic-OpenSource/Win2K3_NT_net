//+-------------------------------------------------------------------------
//
//  Microsoft Windows
//
//  Copyright (C) Microsoft Corporation, 1997 - 1999
//
//  File:       irxfer.cxx
//
//--------------------------------------------------------------------------

/*
 *  IRXFER.C
 *
 */

#include "precomp.h"
#include <malloc.h>

#include <devlist.h>

#include <irmon.h>
#include <irmonftp.h>

#ifndef  VOLATILE
#define  VOLATILE  volatile
#endif



//
struct SAVED_IRDA_DEVICE_LIST
{
    unsigned            MaxDevices;
    OBEX_DEVICE_LIST    Devices;
};

typedef struct _IRXFER_CONTROL {

    CRITICAL_SECTION    Lock;

    PVOID               IrmonContext;
    SET_SOUND_STATUS_FN SoundStatusCallback;


    PXFER_LIST          TransferList;

    FILE_TRANSFER      *IrTransfer1;
    FILE_TRANSFER      *IrTransfer2;

}   IRXFER_CONTROL, *PIRXFER_CONTROL;

IRXFER_CONTROL    GlobalControl;


//
// Handle for irmon.dll (see ..\irmon\irmon.c).
//
extern "C" HINSTANCE    ghInstance;

//
// Entry points for IrTran-P
//
#ifdef IRTRANP_ENABLED

#include <irtranpp.h>


static HANDLE g_hIrTranPThread = NULL;

#endif

//
// registry entries
//
const wchar_t * const WSZ_REG_KEY            = L"Control Panel\\Infrared";
const wchar_t * const WSZ_REG_KEY_GLOBAL     = L"Control Panel\\Infrared\\Global";
const wchar_t * const WSZ_REG_KEY_XFER       = L"Control Panel\\Infrared\\File Transfer";

#ifdef IRTRANP_ENABLED
const wchar_t * const WSZ_REG_KEY_IRTRANP    = L"Control Panel\\Infrared\\IrTranP";
#endif

const wchar_t * const WSZ_REG_SHOW_UI       = L"PopupApp";
const wchar_t * const WSZ_REG_ALLOW_RECEIVE = L"AllowSend";
const wchar_t * const WSZ_REG_SHOW_TRAY     = L"ShowTrayIcon";
const wchar_t * const WSZ_REG_PLAY_SOUND    = L"PlaySound";
const wchar_t * const WSZ_REG_RECEIVED_FILES_FOLDER = L"RecvdFilesLocation";

#ifdef IRTRANP_ENABLED
const wchar_t * const WSZ_REG_PICTURES_FOLDER   = L"RecvdFilesLocation";
const wchar_t * const WSZ_REG_DISABLE_IRTRANPV1 = L"DisableIrTranPv1";
const wchar_t * const WSZ_REG_DISABLE_IRCOMM    = L"DisableIrCOMM";
const wchar_t * const WSZ_REG_EXPLORE           = L"ExploreOnCompletion";
const wchar_t * const WSZ_REG_SAVEASUPF         = L"SaveAsUPF";

const wchar_t * const WSZ_DEFAULT_MYPICTURES    = L"C:\\My Pictures";
#endif


SET_TRAY_STATUS_FN   g_SetTrayStatus;

HKEY    g_hUserKey;
HKEY    g_hInfraredKey;
HKEY    g_hGlobalKey  ;
HKEY    g_hFileXferKey;

#ifdef IRTRANP_ENABLED
HKEY    g_hIrTranPXferKey = 0;
BOOL    g_fIrTranPv1Disabled = FALSE;
BOOL    g_fIrCOMMDisabled = TRUE;
BOOL    g_fExploreOnCompletion = TRUE;
BOOL    g_fSaveAsUPF = FALSE;
#endif


HANDLE  g_RegistryEvent;


MUTEX * g_Mutex;

struct SAVED_IRDA_DEVICE_LIST * g_DeviceList;

wchar_t g_ReceivedFilesFolder  [ MAX_PATH ];
wchar_t g_DuplicateFileTemplate[ MAX_PATH ];
wchar_t g_DesktopFolder        [ MAX_PATH ];

wchar_t g_UnknownDeviceName    [ 100 ];

#ifdef IRTRANP_ENABLED
wchar_t g_DefaultPicturesFolder[ MAX_PATH ];
wchar_t g_SpecifiedPicturesFolder[ MAX_PATH ];
#endif



HANDLE  g_RegistryWaitHandle = 0;

void
RegistryMonitorCallback(
                      PVOID   ignore,
                      BOOLEAN unknown
                      );

BOOL Uninit_RegistryCallback();



RPC_BINDING_HANDLE rpcBinding = 0;

BOOL fListen = TRUE;

BOOL g_fAllowReceives = FALSE;
BOOL g_fShowTrayIcon  = TRUE;
BOOL g_fShutdown      = FALSE;
BOOL g_fLogoff        = FALSE;
BOOL g_fRegistryWaitComplete = FALSE;



void __stdcall
ProcessRegistryChange();

unsigned long
RegistryMonitorThread(
                      PVOID ignore
                      );

DWORD ReadUserPreferences();




extern "C"
{
BOOL
ShowSendWindow();

BOOL
ShowPropertiesPage();



} // extern "c"

VOID
RemoveFromTransferList(
    FILE_TRANSFER*   Transfer
    )
{

    RemoveTransferFromList(GlobalControl.TransferList,Transfer);
    return;
}





BOOL
InitializeIrxfer(
    PVOID               IrmonContext,
    SET_TRAY_STATUS_FN  y,
    SET_SOUND_STATUS_FN SoundStatusCallback,
    PVOID              *IrxferContext
    )

{
    BOOL b;
    DWORD status = 0;


    g_DefaultPicturesFolder[0]='\0';
    g_DesktopFolder[0]='\0';

    ZeroMemory(&GlobalControl,sizeof(GlobalControl));

    GlobalControl.IrmonContext=IrmonContext;

    GlobalControl.SoundStatusCallback=SoundStatusCallback;

    InitializeCriticalSection(&GlobalControl.Lock);

    *IrxferContext=&GlobalControl;


    g_fShutdown = FALSE;

    g_SetTrayStatus  = y;


    //
    // Event logging. This will make sure that the system event
    // logging is configured in the registry:
    //
    status = EVENT_LOG::CheckConfiguration( WS_EVENT_SOURCE,
                                            WS_CATALOG_PATH,
                                            CATEGORY_COUNT,
                                            0x7 );
    #ifdef DBG_ERROR
    if (dwEventStatus)
        {
        DbgPrint("IrMon: EVENT_LOG::CheckConfiguration() failed: %d\n",
                 status );
        }
    #endif

    if (!LoadString( ghInstance,
                     IDS_DUPLICATE_FILE_PATTERN,
                     g_DuplicateFileTemplate,
                     sizeof(g_DuplicateFileTemplate) / sizeof(WCHAR)
                     ))
        {
#if DBG
        DbgPrint("InitializeIrxfer(): LoadString(1) failed\n");
#endif
        goto abend;
    }

    if (!LoadString( ghInstance,
                     IDS_UNKNOWN_DEVICE2,
                     g_UnknownDeviceName,
                     sizeof(g_UnknownDeviceName) / sizeof(WCHAR)
                     ))
        {
#if DBG
        DbgPrint("InitializeIrxfer(): LoadString(2) failed\n");
#endif

        goto abend;
    }

    GlobalControl.TransferList=CreateXferList();

    if (GlobalControl.TransferList == NULL) {

#if DBG
        DbgPrint("InitializeIrxfer(): CreateXferList() failed\n");
#endif

        goto abend;
    }


    g_Mutex = new MUTEX(&status);
    if (!g_Mutex || status)
        {
        goto abend;
        }

    if (FAILED( SHGetFolderPath( NULL,
                                 CSIDL_DESKTOPDIRECTORY,
                                 NULL,
                                 0,
                                 g_DesktopFolder)))
        {

#if DBG
        DbgPrint("InitializeIrxfer(): SHGetFolderPath(desktop) failed\n");
#endif

        goto abend;
    }

    g_RegistryEvent = CreateEvent( NULL,    // no security
                                   FALSE,    // auto-reset
                                   FALSE,   // initially FALSE
                                   NULL     // no name
                                   );
    if (!g_RegistryEvent) {

#if DBG
        DbgPrint("InitializeIrxfer(): CreateEvent() failed\n");
#endif


        goto abend;
    }

    g_hIrTranPThread=StartIrTranP();
    if (g_hIrTranPThread == NULL)
    {
#if DBG
        DbgPrint("InitializeIrxfer(): StartIrTranP() failed\n");
#endif

        goto abend;
    }


    if (ReadUserPreferences() != 0)
    {
        // force Uninit_RegistryCallback to properly terminate registry watcher thread.
        g_fShutdown = true;
        goto abend;
    }

    return TRUE;

abend:

    DWORD     dwEventStatus = 0;
    EVENT_LOG EventLog(WS_EVENT_SOURCE,&dwEventStatus);

    if (!dwEventStatus)
        {
        EventLog.ReportError(CAT_IRXFER, MC_IRXFER_SETUP_FAILED, GetLastError());
        }

    if (GlobalControl.TransferList != NULL) {

        DeleteXferList(GlobalControl.TransferList);
    }

    delete g_Mutex;


    if (g_RegistryEvent)
        {
        Uninit_RegistryCallback();
        }
    return FALSE;
}

DWORD
ReadUserPreferences()
{

    DbgLog(SEV_INFO, "ReadUserPreferences");


    //
    // Open all our keys.  If we can't open the user's key
    // or the key to watch for changes, we bail.
    //
    if (RegOpenKeyEx( HKEY_CURRENT_USER,
                      NULL,
                      0,
                      KEY_READ,
                      &g_hUserKey
                      ))
        {
        DbgLog1(SEV_WARNING, "RegOpenKey  failed %d",  GetLastError());
        return GetLastError();
        }

    DWORD Disposition;
    if (RegCreateKeyEx( g_hUserKey,
                        WSZ_REG_KEY,
                        0,                      // reserved MBZ
                        0,                      // class name
                        REG_OPTION_NON_VOLATILE,
                        KEY_READ,
                        0,                      // security attributes
                        &g_hInfraredKey,
                        &Disposition
                        ))
        {
        DbgLog2(SEV_WARNING, "RegCreateKey '%S' failed %d", WSZ_REG_KEY, GetLastError());
        return GetLastError();
        }

    #ifdef IRTRANP_ENABLED
    RegOpenKeyEx( g_hUserKey,
                  WSZ_REG_KEY_IRTRANP,
                  0,
                  KEY_READ,
                  &g_hIrTranPXferKey );
    #endif


    //
    // Start the registry snoop.
    //
    RegistryMonitorCallback(0, 0);
    
    BOOL            bResult;
    GlobalControl.IrTransfer1=InitializeSocket( SERVICE_NAME_1);
    if (GlobalControl.IrTransfer1 != NULL) {

        bResult=AddTransferToList(GlobalControl.TransferList,GlobalControl.IrTransfer1);

        if (!bResult) {

            GlobalControl.IrTransfer1->StopListening();
            GlobalControl.IrTransfer1=NULL;
        }
    }

    GlobalControl.IrTransfer2=InitializeSocket(SERVICE_NAME_2);
    if (GlobalControl.IrTransfer2 != NULL) {

        bResult=AddTransferToList(GlobalControl.TransferList,GlobalControl.IrTransfer2);

        if (!bResult) {

            GlobalControl.IrTransfer2->StopListening();
            GlobalControl.IrTransfer2=NULL;
        }
    }

    if (GlobalControl.IrTransfer1 == NULL && GlobalControl.IrTransfer2 == NULL)
    {
        DbgPrint("IrMon: ReadUserPreferences::Failed to init sockets\n");
        return -1;
    }
    
    return 0;
}

void
_UserLoggedOff( handle_t rpc,
                error_status_t __RPC_FAR * pStatus
                )
{
    error_status_t status = 0;

    //
    // Allow only a single notification at a time, to avoid odd race conditions.
    //
    CLAIM_MUTEX Lock( g_Mutex );

    DbgLog(SEV_INFO, "user logged off");

    g_fLogoff = TRUE;
    g_fAllowReceives = FALSE;

    if (GlobalControl.IrTransfer1 != NULL) {

        RemoveTransferFromList(GlobalControl.TransferList,GlobalControl.IrTransfer1);

        GlobalControl.IrTransfer1->StopListening();
        GlobalControl.IrTransfer1=NULL;
    }

    if (GlobalControl.IrTransfer2 != NULL) {

        RemoveTransferFromList(GlobalControl.TransferList,GlobalControl.IrTransfer2);

        GlobalControl.IrTransfer2->StopListening();
        GlobalControl.IrTransfer2=NULL;
    }



    //
    // Close down the registry watcher.
    //
    if (g_RegistryWaitHandle)
        {
        if (FALSE == UnregisterWait( g_RegistryWaitHandle ))
            {
            while (!*((BOOL volatile *)&g_fRegistryWaitComplete))
                {
                Sleep(1000);
                }
            }

        g_RegistryWaitHandle = 0;
        }

    g_fRegistryWaitComplete = FALSE;



    //
    // Close the registry handles.
    //
    RegCloseKey( g_hUserKey );
    g_hUserKey = 0;

    RegCloseKey( g_hFileXferKey );
    g_hFileXferKey = 0;

    #ifdef IRTRANP_ENABLED
    RegCloseKey( g_hIrTranPXferKey );
    g_hIrTranPXferKey = 0;
    #endif

    RegCloseKey( g_hInfraredKey );
    g_hInfraredKey = 0;

    RegCloseKey( g_hGlobalKey );
    g_hGlobalKey   = 0;

    *pStatus = 0;

    g_fLogoff = FALSE;
}


#ifdef IRTRANP_ENABLED
//--------------------------------------------------------------------
//  RegistryCheckDwordFlag()
//
//  Check to see if the specified registry key value has changed,
//  if so, then update the value and return TRUE.
//--------------------------------------------------------------------
BOOL RegistryCheckDwordFlag( IN  HKEY         hKey,
                             IN  const WCHAR *pwszValueName,
                             IN       BOOL   NotPresentValue,
                             OUT BOOL        *pbValue )
{
    BOOL  fChanged = FALSE;
    DWORD dwStatus = 0;
    DWORD dwRegDword;
    DWORD dwSize = sizeof(dwRegDword);

    dwStatus = RegQueryValueEx( hKey,
                                pwszValueName,
                                NULL,
                                NULL,
                                (UCHAR*)&dwRegDword,
                                &dwSize );

    if (dwStatus != ERROR_SUCCESS) {
        //
        //  failed, no change
        //
        DbgLog2(SEV_INFO,"REG: Key/Value: %S: Status: %d\n",
                pwszValueName, dwStatus );

        if (NotPresentValue != *pbValue) {
            //
            //  the not present value is no the same as the current, change and report a change
            //
            *pbValue=NotPresentValue;

            return TRUE;
        }
        //
        //  No change
        //
        return FALSE;

    } else {
        //
        //  got value
        //
        if ( (dwRegDword) && (*pbValue == FALSE)) {
            //
            // Value changed from FALSE to TRUE.
            //
            fChanged = TRUE;
            *pbValue = TRUE;

        } else  if ((!dwRegDword) && (*pbValue)) {
            //
            // Value changed from TRUE to FALSE.
            //
            fChanged = TRUE;
            *pbValue = FALSE;
        }

    }

    return fChanged;
}

//--------------------------------------------------------------------
//  IrTranPFlagChanged()
//
//  Check to see if the specified registry flag changed. If the value
//  changed, return TRUE, if not then return FALSE. Return the new value
//  in *pfDisabled.
//--------------------------------------------------------------------
BOOL IrTranPFlagChanged( IN const WCHAR *pwszDisabledValueName,
                         IN       BOOL   NotPresentValue,
                         IN OUT   BOOL  *pfDisabled )
    {
    DWORD dwStatus = 0;
    DWORD dwSize;
    DWORD dwRegDword;
    BOOL  fChanged = FALSE;
    BOOL  fDisabled = *pfDisabled;

    DbgLog3(SEV_INFO, "Process IrTranP flag Change %S, def=%d, current=%d",
        pwszDisabledValueName,
        NotPresentValue,
        *pfDisabled
        );

    if (!g_hIrTranPXferKey)
        {
        // By default, the IrTranP subkey does not exist at all until
        // user brings up Wireless Link CPL and makes changes to
        // to the Image Transfer page. Without trying to reopen the subkey
        // here, we may fail to pick up the changes if this is the first
        // time IrTranP subkey gets created.
        // The other issue that might rise(although rare) is when the subkey
        // is deleted for whatever reasons. We should reset everything to
        // default when the key is deleted. An easy but slow solution is to
        // reopen the subkey everytime this function is called. This approach
        // would ensures that the HKEY to the subkey is always fresh.
        // Since the case is rare and implementation is slow, lets forget
        // about this for the time being. If the entire Infrared subkey is
        // deleted or missing then we will return TRUE (the default is
        // enabled).
        //
        RegOpenKeyEx(g_hUserKey,
                     WSZ_REG_KEY_IRTRANP,
                     0,
                     KEY_READ,
                     &g_hIrTranPXferKey );
        }

    if (!g_hIrTranPXferKey) {

        DbgLog(SEV_INFO, "IrTranP: g_hIrTranPXferKey not opened");

        if (NotPresentValue != *pfDisabled) {
            //
            //  the not present value does not match the current setting, change it
            //
            *pfDisabled=NotPresentValue;
            return TRUE;
        }

        //
        //  it must match, so no change
        //
        return FALSE;

    }

    //
    // Is IrTranP over the specified channel to be disabled?
    //
    if (RegistryCheckDwordFlag(g_hIrTranPXferKey,
                               pwszDisabledValueName,
                               NotPresentValue,
                               &fDisabled))
        {
        #ifdef DBG_REGISTRY
        DbgPrint("DisableIrCOMM: Changed: %d\n",g_fIrCOMMDisabled);
        #endif
        }

    if (*pfDisabled != fDisabled)
        {
        *pfDisabled = fDisabled;
        fChanged = TRUE;
        }

    return fChanged;
    }

//--------------------------------------------------------------------
//  ProcessIrTranPRegistryChanges()
//
//--------------------------------------------------------------------
void ProcessIrTranPRegistryChanges()
    {
    DWORD dwStatus = 0;
    DWORD dwSize;
    DWORD dwRegDword;

    DbgLog(SEV_INFO, "Process IrTranP Registry Change");

    //
    // Is IrTranP over IrCOMM to be disabled?
    //
    if (IrTranPFlagChanged(WSZ_REG_DISABLE_IRCOMM,TRUE,&g_fIrCOMMDisabled))
        {
        #ifdef DBG_REGISTRY
        DbgPrint("DisableIrCOMM: Changed: %d\n",g_fIrCOMMDisabled);
        #endif
        dwStatus = EnableDisableIrCOMM(g_hIrTranPThread,g_fIrCOMMDisabled);

        if ((g_fIrCOMMDisabled == FALSE) && (dwStatus))
            {
            #ifdef DBG_ERROR
            if (dwStatus)
                {
                DbgPrint("IrTranP: EnableDisableIrCOMM() failed: %d\n",dwStatus);
                }
            #endif

            IrTranPEnableIrCOMMFailed(g_hIrTranPThread,dwStatus);
            }
        }

    //
    // Is IrTranP over sockets (IrTranPv1) to be disabled?
    //
    if (IrTranPFlagChanged(WSZ_REG_DISABLE_IRTRANPV1,FALSE,&g_fIrTranPv1Disabled))
        {
        #ifdef DBG_REGISTRY
        DbgPrint("DisableIrTranPv1: Changed: %d\n",g_fIrTranPv1Disabled);
        #endif
        dwStatus = EnableDisableIrTranPv1(g_hIrTranPThread,g_fIrTranPv1Disabled);
        }

    //
    // Explore the new pictures automatically:
    //
    if (RegistryCheckDwordFlag(g_hIrTranPXferKey,
                               WSZ_REG_EXPLORE,
                               TRUE,
                               &g_fExploreOnCompletion))
        {
        #ifdef DBG_REGISTRY
        DbgPrint("ExploreOnCompletion: Changed: %d\n",g_fExploreOnCompletion);
        #endif
        }

    //
    // Save pictures as .UPF files instead of .JPEG files?
    //
    // NOTE: This is used for testing to create full .UPF files and
    // would not normally be set by the customer.
    //
    if (RegistryCheckDwordFlag(g_hIrTranPXferKey,
                               WSZ_REG_SAVEASUPF,
                               FALSE,
                               &g_fSaveAsUPF))
        {
        #ifdef DBG_REGISTRY
        DbgPrint("SaveAsUPF: Changed: %d\n",g_fSaveAsUPF);
        #endif
        }

    //
    // Where pictures are to be stored. If this value exists then it
    // is used instead of the default "My Documents\My Pictures\"
    // path.
    //
    dwSize = sizeof(g_SpecifiedPicturesFolder);
    dwStatus = RegQueryValueEx( g_hIrTranPXferKey,
                                WSZ_REG_PICTURES_FOLDER,
                                NULL,
                                NULL,
                                (UCHAR*)g_SpecifiedPicturesFolder,
                                &dwSize );
    if (dwStatus)
        {
        DbgLog1( SEV_INFO, "REG: Pictures Folder Status: %d\n", dwStatus );
        g_SpecifiedPicturesFolder[0] = 0;
        }
    else
        {
        DbgLog1( SEV_INFO, "REG: Pictures Folder %S\n",
                 g_SpecifiedPicturesFolder );
        #ifdef DBG_REGISTRY
        DbgPrint("RecvdFilesLocation: %S\n",g_SpecifiedPicturesFolder);
        #endif
        }
    }
#endif




//--------------------------------------------------------------------
//  ProcessRegistryChange()
//
//--------------------------------------------------------------------
void __stdcall
ProcessRegistryChange()
{
    DWORD status = 0;

    DWORD size = sizeof(unsigned char);
    unsigned char boolVal = 0;

    DbgLog(SEV_INFO, "process registry change");

    if (!g_hGlobalKey)
        {
        status = RegOpenKeyEx( g_hUserKey,
                               WSZ_REG_KEY_GLOBAL,
                               0,
                               KEY_READ,
                               & g_hGlobalKey
                               );
        if (status)
            {
            DbgLog2(SEV_WARNING, "RegOpenKeyEx '%S' failed %d", WSZ_REG_KEY_GLOBAL, status);
            }
        }

    if (!g_hFileXferKey)
        {
        status = RegOpenKeyEx( g_hUserKey,
                               WSZ_REG_KEY_XFER,
                               0,
                               KEY_READ,
                               & g_hFileXferKey
                               );
        if (status)
            {
            DbgLog2(SEV_WARNING, "RegOpenKeyEx '%S' failed %d", WSZ_REG_KEY_XFER, status);
            }
        }


    //
    // Are other people allowed to send to me?
    //
    size = sizeof(unsigned char);
    status = RegQueryValueEx( g_hFileXferKey,
                              WSZ_REG_ALLOW_RECEIVE,
                              NULL,
                              NULL,
                              &boolVal,
                              &size
                              );
    if (!g_hUserKey)
        {
        g_fAllowReceives = FALSE;
        }
    else if (status || boolVal)
        {
        DbgLog1( SEV_INFO, "reg: allow receives key, status %d\n", status);
        g_fAllowReceives = TRUE;
        }
    else
        {
        g_fAllowReceives = FALSE;
        }

    size = sizeof( g_ReceivedFilesFolder );
    status = RegQueryValueEx( g_hFileXferKey,
                              WSZ_REG_RECEIVED_FILES_FOLDER,
                              NULL,
                              NULL,
                              (unsigned char *) g_ReceivedFilesFolder,
                              &size
                              );
    if (status) {

        StringCbCopy(g_ReceivedFilesFolder,sizeof(g_ReceivedFilesFolder), g_DesktopFolder );
        DbgLog2( SEV_INFO, "reg: received files folder status %d\nfolder %S", status, g_ReceivedFilesFolder);

    } else {
        //
        // normalize the path so that we can compare it to other normalized paths
        //
        WCHAR szNormalized[MAX_PATH];
        DWORD dwLen = GetFullPathName(g_ReceivedFilesFolder, MAX_PATH, szNormalized, NULL);

        if (dwLen != 0 && dwLen < MAX_PATH) {

            StringCbCopy(g_ReceivedFilesFolder,sizeof(g_ReceivedFilesFolder), szNormalized);

        } else {

            StringCbCopy( g_ReceivedFilesFolder,sizeof(g_ReceivedFilesFolder), g_DesktopFolder );
        }

        DbgLog1( SEV_INFO, "reg: received files folder %S\n", g_ReceivedFilesFolder);
    }

    //
    // always end path with a backslash '\'
    //
    if ( bNoTrailingSlash(g_ReceivedFilesFolder) ) {

        StringCbCat(g_ReceivedFilesFolder,sizeof(g_ReceivedFilesFolder), szBACKSLASH );
    }

    size = sizeof(unsigned char);
    status = RegQueryValueEx( g_hGlobalKey,
                              WSZ_REG_SHOW_TRAY,
                              NULL,
                              NULL,
                              &boolVal,
                              &size
                              );
    if (status || boolVal)
        {
        DbgLog1( SEV_INFO, "reg: show tray key, status %d\n", status);
        g_fShowTrayIcon = TRUE;
        }
    else
        {
        g_fShowTrayIcon = FALSE;
        }

    (*g_SetTrayStatus)( GlobalControl.IrmonContext, g_fShowTrayIcon );

    size = sizeof(unsigned char);
    status = RegQueryValueEx( g_hGlobalKey,
                              WSZ_REG_PLAY_SOUND,
                              NULL,
                              NULL,
                              &boolVal,
                              &size
                              );

    if ((status != ERROR_SUCCESS)) {

        DbgLog1( SEV_INFO, "reg: play sound key, status %d\n", status);
        boolVal=TRUE;

    }

    (*GlobalControl.SoundStatusCallback)(
        GlobalControl.IrmonContext,
        boolVal
        );

    //
    // Process changes specific to IrTran-P:
    //
    #ifdef IRTRANP_ENABLED
    ProcessIrTranPRegistryChanges();
    #endif

    //
    // Look for more changes.
    //
    status = RegNotifyChangeKeyValue( g_hInfraredKey,
                                      TRUE,              // watch child keys
                                      REG_NOTIFY_CHANGE_LAST_SET,
                                      g_RegistryEvent,
                                      TRUE               // async
                                      );
    if (status)
        {
        DWORD     dwEventStatus = 0;
        EVENT_LOG EventLog(WS_EVENT_SOURCE,&dwEventStatus);

        if (!dwEventStatus)
            {
            EventLog.ReportError(CAT_IRXFER, MC_IRXFER_REGISTER_NOTIFY_FAILED, status);
            }
        }
}



void
RegistryMonitorCallback(
                      PVOID   ignore,
                      BOOLEAN unknown
                      )
{
    DbgLog(SEV_INFO, "reg callback");

    if (g_fShutdown || g_fLogoff)
        {
        DbgLog(SEV_INFO, "reg callback: terminating");

        if (g_fShutdown)
            {
            CloseHandle( g_RegistryEvent );
            }

        g_fRegistryWaitComplete = TRUE;
        return;
        }

    ProcessRegistryChange();

    if (!RegisterWaitForSingleObject( &g_RegistryWaitHandle,
                                      g_RegistryEvent,
                                      RegistryMonitorCallback,
                                      0,
                                      INFINITE,
                                      WT_EXECUTEDEFAULT | WT_EXECUTEONLYONCE
                                      ))
        {
        g_RegistryWaitHandle = 0;

        DWORD     dwEventStatus = 0;
        EVENT_LOG EventLog(WS_EVENT_SOURCE,&dwEventStatus);

        if (!dwEventStatus)
            {
            EventLog.ReportError(CAT_IRXFER, MC_IRXFER_REGISTER_NOTIFY_FAILED, GetLastError());
            }
        }
}



void
UpdateDiscoveredDevices(
    const OBEX_DEVICE_LIST *IrDevices
    )
{
    BOOL fLaunch = FALSE;
    error_status_t status;

    //
    // Update the saved device list.
    //
    SAVED_IRDA_DEVICE_LIST * list;

    g_Mutex->Enter();

    if ((g_DeviceList == NULL)
        ||
        (g_DeviceList->MaxDevices < IrDevices->DeviceCount)) {

        list = (SAVED_IRDA_DEVICE_LIST *)
               new char [ sizeof(SAVED_IRDA_DEVICE_LIST)
                          + (IrDevices->DeviceCount ) * sizeof(OBEX_DEVICE)
                          ];
        if (!list) {

            g_Mutex->Leave();
            return;
        }

        list->MaxDevices = 1+IrDevices->DeviceCount;

        if (g_DeviceList) {

            delete g_DeviceList;
        }

        g_DeviceList = list;

    } else {

        list = g_DeviceList;
    }

    //
    // Determine how many of the existing devices support our protocol.
    //
    list->Devices.DeviceCount=0;

    unsigned i;
    for (i=0; i < IrDevices->DeviceCount; i++) {

        list->Devices.DeviceList[list->Devices.DeviceCount]=IrDevices->DeviceList[i];

        list->Devices.DeviceCount++;

        fLaunch = fLaunch || list->Devices.DeviceList[i].DeviceSpecific.s.Irda.ObexSupport;

    }
    //
    // Inform the UI program of the new device list.  Don't create the process
    // unless at least one of the devices said it supports OBEX.
    //
    if (fLaunch) {

        g_Mutex->Leave();

        LaunchUi( NULL );

    } else {

        if (g_DeviceList->Devices.DeviceCount > 0) {

            _DeviceInRange( rpcBinding, &g_DeviceList->Devices, &status );

            g_Mutex->Leave();

            DbgLog1(SEV_INFO, "UpdateDiscoveredDevices: DeviceInRange returned %d", status);

        } else {

            g_Mutex->Leave();

            _NoDeviceInRange( rpcBinding, &status );
        }
    }
}


BOOL
UninitializeIrxfer(
    PVOID    IrxferContext
    )
{
    PIRXFER_CONTROL    Control=(PIRXFER_CONTROL)IrxferContext;
    error_status_t     LogoffStatus;

    g_fShutdown = TRUE;

    DbgLog1(SEV_INFO, "start shutdown: %d", GetTickCount());


    _UserLoggedOff(NULL,&LogoffStatus);

    if (Control->TransferList != NULL) {

        DeleteXferList(Control->TransferList);
    }


    _ShutdownUi( rpcBinding );

    BOOL fSuccess = TRUE;


    fSuccess &= Uninit_RegistryCallback();

    DbgLog1(SEV_INFO, "after registry: %d", GetTickCount());

    #ifdef IRTRANP_ENABLED
    StopIrTranP(g_hIrTranPThread);
    #endif


#ifdef LOGGING

    Log_Close();

#endif

    DeleteCriticalSection(&Control->Lock);

    return fSuccess;
}


BOOL
Uninit_RegistryCallback()
{
    BOOL fSuccess = TRUE;

    SetEvent( g_RegistryEvent );

    //
    // The handle will be deleted when the callback is completed.
    //
    while (WAIT_TIMEOUT == WaitForSingleObject( g_RegistryEvent, 0 ))
        {
        Sleep(1000);
        }

    return fSuccess;
}



void
_SendFiles(
    handle_t RpcHandle,
    COOKIE   ClientCookie,
    wchar_t  DirectoryName[],
    wchar_t  FileNameList[],
    long   ListLength,
    unsigned long DeviceId,
    OBEX_DEVICE_TYPE    DeviceType,
    error_status_t * pStatus,
    int * pLocation
    )
{
    FILE_TRANSFER * Transfer;

    DbgPrint("IRMON: SendFiles address=%08lx, type=%d\n",DeviceId,DeviceType);

    *pStatus = 0;
    *pLocation = (int)locStartup;

    if ( g_fShutdown )
        {
        *pStatus = ERROR_SHUTDOWN_IN_PROGRESS;
        return;
        }

    if (DirectoryName && *DirectoryName)
        {
        *pStatus = ERROR_ACCESS_DENIED;
        return;
        }

    //
    // Try NT-compatible connect.
    //
    Transfer = new FILE_TRANSFER;
    if (!Transfer || !Transfer->Xfer_Init( FileNameList, ListLength, dialNt5, DeviceType,TRUE,INVALID_SOCKET ))
        {
        delete Transfer;
        *pStatus = ERROR_NOT_ENOUGH_MEMORY;
        return;
        }

    Transfer->_rpcHandle = RpcHandle;
    Transfer->_cookie    = ClientCookie;

    Transfer->BeginSend( DeviceId, DeviceType, pStatus, (FAILURE_LOCATION*)pLocation );

    if (*pStatus == ERROR_INVALID_DATA && *pLocation == locConnect) {

        //
        // Try Win98-compatible connect.
        //
        Transfer = new FILE_TRANSFER;
        if (!Transfer || !Transfer->Xfer_Init( FileNameList, ListLength, dialWin95,DeviceType,TRUE,INVALID_SOCKET ))
            {
            delete Transfer;
            *pStatus = ERROR_NOT_ENOUGH_MEMORY;
            return;
            }

        Transfer->_rpcHandle = RpcHandle;
        Transfer->_cookie    = ClientCookie;

        Transfer->BeginSend( DeviceId, DeviceType, pStatus, (FAILURE_LOCATION*)pLocation );
    }

    if (*pStatus != ERROR_SUCCESS) {

        DWORD     dwEventStatus = 0;
        EVENT_LOG EventLog(WS_EVENT_SOURCE,&dwEventStatus);

        if (!dwEventStatus) {

            EventLog.ReportError(CAT_IRXFER, MC_IRXFER_CONNECT_FAILED, *pStatus);
        }
    } else {
        //
        //  it worked add it to the list
        //

        AddTransferToList(GlobalControl.TransferList,Transfer);

    }
}


error_status_t
_CancelSend(
    /* [in] */ handle_t      binding,
    /* [in] */ COOKIE        cookie
               )
{
    FILE_TRANSFER * transfer = TransferFromCookie(GlobalControl.TransferList,cookie);

    if (!transfer) {

        return ERROR_FILE_NOT_FOUND;
    }

    transfer->Cancel();

    return 0;
}



error_status_t
_CancelReceive(
    /* [in] */ handle_t      binding,
    /* [in] */ COOKIE        cookie
               )
{
    FILE_TRANSFER * transfer = TransferFromCookie(GlobalControl.TransferList,cookie);

    if (!transfer) {

        return ERROR_FILE_NOT_FOUND;
    }

    transfer->Cancel();

    return 0;
}

BOOL
LaunchUi(
         wchar_t * cmdline
         )
{

    //
    // Give the UI the current device list.
    //
    error_status_t status = 0;

    g_Mutex->Enter();

    if (g_DeviceList)
        {
        _DeviceInRange( rpcBinding, &g_DeviceList->Devices, &status );
        }

    DbgLog1(SEV_INFO, "LaunchUi: DeviceInRange returned %d", status);

    g_Mutex->Leave();

    return TRUE;
}


BOOL GetReceivedFilesFolder( LPWSTR szDir, DWORD dwLen )
{
    if (g_ReceivedFilesFolder[0] == L'\0') {

        ProcessRegistryChange();
    }

    StringCchCopy(szDir, dwLen, g_ReceivedFilesFolder);


    return TRUE;
}

BOOL
ShowSendWindow()
{

    _PopupUI(NULL);

    return TRUE;
}

BOOL ShowPropertiesPage()
{
    _DisplaySettings(NULL);

    return TRUE;
}

extern "C"
{
LRESULT
IrxferHandlePowerMessage(
    HWND hWnd,
    UINT message,
    WPARAM wParam,
    LPARAM lParam
    );
}

BOOL IrxferQuerySuspend( LPARAM lParam );
void  IrxferResume();


LRESULT
IrxferHandlePowerMessage(
    HWND hwnd,
    UINT message,
    WPARAM wParam,
    LPARAM lParam
    )
{
    if (message == WM_QUERYENDSESSION)
        {
        DbgLog1(SEV_INFO, "POWER: Query logoff %d", lParam);
        return IrxferQuerySuspend( 1 );
        }

    ASSERT( message == WM_POWERBROADCAST );

    switch (wParam)
        {
        case PBT_APMQUERYSUSPEND:
            {
            DbgLog1(SEV_INFO, "POWER: Query Suspend %d", lParam);
            return ( IrxferQuerySuspend( lParam ) ? TRUE : BROADCAST_QUERY_DENY );
            }
        case PBT_APMQUERYSUSPENDFAILED:
            {
            DbgLog(SEV_INFO, "POWER: Suspend Failed");
            IrxferResume();
            break;
            }
        case PBT_APMRESUMESUSPEND:
            {
            DbgLog(SEV_INFO, "POWER: Resume Suspend");
            IrxferResume();
            break;
            }
        case PBT_APMRESUMECRITICAL:
            {
            DbgLog(SEV_INFO, "POWER: Resume Critical");
            //
            // Do nothing.  The Winsock layer will send failures
            // for reads and writes on any sockets active before
            // the shutdown.
            //
            break;
            }

        default:
            DbgLog2(SEV_INFO, "POWER: w=0x%x l=0x%x", wParam, lParam);
        }

    return DefWindowProc(hwnd, message, wParam, lParam);
}

BOOL
IrxferQuerySuspend( LPARAM lParam )
{


    if (!AreThereActiveTransfers(GlobalControl.TransferList)) {

        DbgLog(SEV_INFO, "    no transfers active");
        return TRUE;
    }

    if (lParam & 1)
        {
        //
        // The app is allowed to prompt the user if necessary.
        //
        boolean answer;
        DWORD status = _ShutdownRequested( rpcBinding, &answer );
        DbgLog2(SEV_INFO, "    status %d  answer %d", status, answer);
        if (status || answer == TRUE)
            {
            g_fShutdown = 1;
            return TRUE;
            }
        else
            {
            return FALSE;
            }
        }
    else
        {
        DbgLog(SEV_INFO, "    not allowed to prompt");
        g_fShutdown = 1;
        return TRUE;
        }
}

void
IrxferResume()
{
    g_fShutdown = FALSE;

    // update device list
}
