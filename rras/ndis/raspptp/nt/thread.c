//depot/Lab03_N/Net/rras/ndis/raspptp/nt/thread.c#6 - edit change 19457 (text)
/*******************************************************************
*
*   Copyright (c) 1998-1999 Microsoft Corporation
*
*    DESCRIPTION: THREAD.C - Thread handling routines, for NT
*                 Also implements work items.
*
*    AUTHOR: Stan Adermann (StanA)
*
*    DATE:10/20/1998
*
*******************************************************************/

#include "raspptp.h"
#include <stdarg.h>
#include <stdio.h>

extern struct PPTP_ADAPTER * pgAdapter;

//ULONG ProcCountTxQ[2] = {0, 0};
//ULONG ProcCountRxQ[2] = {0, 0};

HANDLE          hPassiveThread = NULL;
KEVENT          EventPassiveThread;
KEVENT          EventKillThread;
LIST_ENTRY      WorkItemList;
NDIS_SPIN_LOCK  GlobalThreadLock;

BOOLEAN ThreadingInitialized = FALSE;

NDIS_STATUS
ScheduleWorkItem(
    WORK_PROC         Callback,
    PVOID             Context,
    PVOID             InfoBuf,
    ULONG             InfoBufLen)
{
    PPPTP_WORK_ITEM pWorkItem;
    NDIS_STATUS Status = NDIS_STATUS_FAILURE;

    DEBUGMSG(DBG_FUNC, (DTEXT("+ScheduleWorkItem\n")));

    ASSERT(ThreadingInitialized);
    pWorkItem = MyMemAlloc(sizeof(PPTP_WORK_ITEM), TAG_WORK_ITEM);

    if (pWorkItem != NULL)
    {
        pWorkItem->Context      = Context;
        pWorkItem->pBuffer      = InfoBuf;
        pWorkItem->Length       = InfoBufLen;

        /*
        ** This interface was designed to use NdisScheduleWorkItem(), which
        ** would be good except that we're really only supposed to use that
        ** interface during startup and shutdown, due to the limited pool of
        ** threads available to service NdisScheduleWorkItem().  Therefore,
        ** instead of scheduling real work items, we simulate them, and use
        ** our own thread to process the calls.  This also makes it easy to
        ** expand the size of our own thread pool, if we wish.
        **
        ** Our version is slightly different from actual NDIS_WORK_ITEMs,
        ** because that is an NDIS 5.0 structure, and we want people to
        ** (at least temporarily) build this with NDIS 4.0 headers.
        */

        pWorkItem->Callback = Callback;

        /*
        ** Our worker thread checks this list for new jobs, whenever its event
        ** is signalled.
        */

        MyInterlockedInsertTailList(&WorkItemList,
                                    &pWorkItem->ListEntry,
                                    &GlobalThreadLock);

        // Wake up our thread.

        KeSetEvent(&EventPassiveThread, 0, FALSE);
        Status = NDIS_STATUS_SUCCESS;
    }
    else
    {
        gCounters.ulWorkItemFail++;
    
    }

    return Status;
}

STATIC VOID
MainPassiveLevelThread(
    IN OUT PVOID Context
    )
{
    NDIS_STATUS Status;
    NTSTATUS    NtStatus;
    PLIST_ENTRY pListEntry;
    PKEVENT EventList[2];

    DEBUGMSG(DBG_FUNC, (DTEXT("+MainPassiveLevelThread\n")));

    //KeSetPriorityThread(KeGetCurrentThread(), LOW_REALTIME_PRIORITY);

    EventList[0] = &EventPassiveThread;
    EventList[1] = &EventKillThread;

    for (;;)
    {
        //
        // The EventPassiveThread is an auto-clearing event, so
        // we don't need to reset the event.
        //

        NtStatus = KeWaitForMultipleObjects(2,
                                            EventList,
                                            WaitAny,
                                            Executive,
                                            KernelMode,
                                            FALSE,
                                            NULL,
                                            NULL);

        if (NtStatus==0) // The first event, for a work item, was signalled
        {
            while (pListEntry = MyInterlockedRemoveHeadList(&WorkItemList,
                                                            &GlobalThreadLock))
            {
                PPPTP_WORK_ITEM pWorkItem = CONTAINING_RECORD(pListEntry,
                                                              PPTP_WORK_ITEM,
                                                              ListEntry);

                ASSERT(KeGetCurrentIrql()<DISPATCH_LEVEL);
                pWorkItem->Callback(pWorkItem);
                ASSERT(KeGetCurrentIrql()<DISPATCH_LEVEL);
                FreeWorkItem(pWorkItem);
            }
        }
        else
        {
            // A kill event was received.

            DEBUGMSG(DBG_THREAD, (DTEXT("Thread: HALT %08x\n"), NtStatus));

            // Free any pending requests

            while (pListEntry = MyInterlockedRemoveHeadList(&WorkItemList,
                                                            &GlobalThreadLock))
            {
                PPPTP_WORK_ITEM pWorkItem = CONTAINING_RECORD(pListEntry,
                                                              PPTP_WORK_ITEM,
                                                              ListEntry);

                DEBUGMSG(DBG_WARN, (DTEXT("Releasing work item %08x\n"), pWorkItem));
                FreeWorkItem(pWorkItem);
            }

            hPassiveThread = NULL;
            DEBUGMSG(DBG_FUNC, (DTEXT("PsTerminateSystemThread MainPassiveLevelThread\n")));
            PsTerminateSystemThread(STATUS_SUCCESS);

            break;
        }
    }

    DEBUGMSG(DBG_FUNC, (DTEXT("-MainPassiveLevelThread\n")));
}


NDIS_STATUS
CallQueueTransmitPacket(
    PCALL_SESSION       pCall,
    PNDIS_WAN_PACKET    pWanPacket
    )
{
    NDIS_STATUS Status = NDIS_STATUS_PENDING;

    DEBUGMSG(DBG_FUNC, (DTEXT("+CallQueueTransmitPacket\n")));

    //++ProcCountTxQ[KeGetCurrentProcessorNumber()];

    if (!IS_CALL(pCall) || pCall->State!=STATE_CALL_ESTABLISHED)
    {
        // Drop the packet.
        Status = NDIS_STATUS_SUCCESS;
        goto cqtpDone;
    }

    NdisAcquireSpinLock(&pCall->Lock);

    InsertTailList(&pCall->TxPacketList, &pWanPacket->WanPacketQueue);

    if (!pCall->Transferring)
    {
        pCall->Transferring = TRUE;
        REFERENCE_OBJECT(pCall);
        NdisScheduleWorkItem(&pCall->SendWorkItem);
    }

    NdisReleaseSpinLock(&pCall->Lock);

cqtpDone:

    DEBUGMSG(DBG_FUNC, (DTEXT("-CallQueueTransmitPacket\n")));
    return Status;
}

NDIS_STATUS
CallQueueReceivePacket(
    PCALL_SESSION       pCall,
    PDGRAM_CONTEXT      pDgContext
    )
{
    NDIS_STATUS Status = NDIS_STATUS_SUCCESS;
    PGRE_HEADER pGre = pDgContext->pGreHeader;
    ULONG_PTR ProcNum = 0;
    PULONG pSequence;
    ULONG Sequence;

    DEBUGMSG(DBG_FUNC, (DTEXT("+CallQueueReceivePacket\n")));

    //++ProcCountRxQ[KeGetCurrentProcessorNumber()];

    if(!pGre->SequenceNumberPresent)
    {
        return NDIS_STATUS_FAILURE;
    }

    NdisAcquireSpinLock(&pCall->Lock);
    if (pCall->RxPacketsPending > 256 ||
        htons(pGre->KeyCallId)!=pCall->Packet.CallId ||
        pCall->State!=STATE_CALL_ESTABLISHED ||
        !IS_LINE_UP(pCall))
    {
        NdisReleaseSpinLock(&pCall->Lock);

        DEBUGMSG(DBG_PACKET|DBG_RX, (DTEXT("Rx: GRE CallId invalid or call in wrong state\n")));
        Status = NDIS_STATUS_FAILURE;
        goto cqrpDone;
    }

    // The packet has passed all of our tests.

    if (IsListEmpty(&pCall->RxPacketList))
    {
        InsertTailList(&pCall->RxPacketList, &pDgContext->ListEntry);
        pCall->RxPacketsPending++;
    }
    else
    {
        // There are packets already queued.  Put this one in order.
        pSequence = (PULONG)(pGre + 1);
        Sequence = htonl(*pSequence);

        // We don't check the sequence # anymore, just put it on the queue in order
        {

            // There are packets already queued.  Put this one in order.
            // The sequence # is the one we want soon to be the one we want.
            PLIST_ENTRY pListEntry;
            BOOLEAN OnList = FALSE;
    
            for (pListEntry = pCall->RxPacketList.Flink;
                 pListEntry != &pCall->RxPacketList;
                 pListEntry = pListEntry->Flink)
            {
                PDGRAM_CONTEXT pListDg = CONTAINING_RECORD(pListEntry,
                                                           DGRAM_CONTEXT,
                                                           ListEntry);
    
                if ((signed)htonl(GreSequence(pListDg->pGreHeader)) - (signed)Sequence > 0)
                {
                    // The one on the list is newer!  Put this one before it.
                    InsertTailList(&pListDg->ListEntry, &pDgContext->ListEntry);
                    pCall->RxPacketsPending++;
                    OnList = TRUE;
                    break;
                }
            }
            if (!OnList)
            {
                // There were no packets on this list with a greater sequence.
                // Put this at the end.
                InsertTailList(&pCall->RxPacketList, &pDgContext->ListEntry);
                pCall->RxPacketsPending++;
            }
        }
    }
    
    if (!pCall->Receiving)
    {
        pCall->Receiving = TRUE;
        REFERENCE_OBJECT(pCall);
        NdisScheduleWorkItem(&pCall->RecvWorkItem);

//        PptpQueueDpc(&pCall->ReceiveDpc);
    }

    NdisReleaseSpinLock(&pCall->Lock);

cqrpDone:
    DEBUGMSG(DBG_FUNC, (DTEXT("-CallQueueReceivePacket\n")));
    return Status;
}


NDIS_STATUS
InitThreading(
    NDIS_HANDLE hMiniportAdapter
    )
{
    NDIS_STATUS Status = NDIS_STATUS_SUCCESS;
    NTSTATUS NtStatus;

    UNREFERENCED_PARAMETER(hMiniportAdapter);

    DEBUGMSG(DBG_FUNC, (DTEXT("+InitializeThreading\n")));

    if (ThreadingInitialized)
    {
       ASSERT(!"Threading initialized twice");
       goto itDone;
    }

    NdisInitializeListHead(&WorkItemList);
    NdisAllocateSpinLock(&GlobalThreadLock);

    KeInitializeEvent(
                &EventPassiveThread,
                SynchronizationEvent, // auto-clearing event
                FALSE                 // event initially non-signalled
                );

    KeInitializeEvent(
                &EventKillThread,
                SynchronizationEvent, // auto-clearing event
                FALSE                 // event initially non-signalled
                );

    NtStatus = PsCreateSystemThread(&hPassiveThread,
                                    (ACCESS_MASK) 0L,
                                    NULL,
                                    NULL,
                                    NULL,
                                    MainPassiveLevelThread,
                                    NULL);
                                    
                                    
    if (NtStatus == STATUS_SUCCESS)
    {
        ThreadingInitialized = TRUE;
    }
    else
    {
        DEBUGMSG(DBG_ERROR, (DTEXT("PsCreateSystemThread failed. %08x\n"), Status));
        Status = NDIS_STATUS_FAILURE;

        goto itDone;
    }

itDone:
    DEBUGMSG(DBG_FUNC, (DTEXT("-InitializeThreading %08x\n"), Status));
    return Status;
}

VOID STATIC WaitForThreadToDieAndCloseIt(HANDLE hThread, PRKEVENT pKillEvent)
{
    PVOID pThread = NULL;
    NTSTATUS Status;
    
    DEBUGMSG(DBG_FUNC, (DTEXT("+WaitForThreadToDie\n")));

    if ( hThread != NULL && pKillEvent != NULL )
    {

        Status = ObReferenceObjectByHandle(hThread, 0, NULL, KernelMode, &pThread, NULL);
        if (Status==STATUS_SUCCESS)
        {
            KeSetEvent(pKillEvent, 0, FALSE);
    
            KeWaitForSingleObject(pThread, Executive, KernelMode, FALSE, NULL);
            ObDereferenceObject(pThread);
        }
        ZwClose(hThread);
    }
    
    DEBUGMSG(DBG_FUNC, (DTEXT("-WaitForThreadToDie\n")));
}

VOID
DeinitThreading()
{
    DEBUGMSG(DBG_FUNC, (DTEXT("+DeinitThreading\n")));
    
    ThreadingInitialized = FALSE;
    
    WaitForThreadToDieAndCloseIt( hPassiveThread, 
                                  &EventKillThread );

    DEBUGMSG(DBG_FUNC, (DTEXT("-DeinitThreading\n")));
}


UCHAR TapiLineNameBuffer[64] =  TAPI_LINE_NAME_STRING;
ANSI_STRING TapiLineName = {
    sizeof(TAPI_LINE_NAME_STRING),
    sizeof(TapiLineNameBuffer),
    TapiLineNameBuffer
};
typedef UCHAR TAPI_CHAR_TYPE;

ANSI_STRING TapiLineAddrList = { 0, 0, NULL };

#define READ_NDIS_REG_ULONG(hConfig, Var, Value) \
    {                                                                                   \
        PNDIS_CONFIGURATION_PARAMETER RegValue;                                         \
        NDIS_STATUS Status;                                                             \
        NDIS_STRING String = NDIS_STRING_CONST(Value);                                  \
                                                                                        \
        NdisReadConfiguration(&Status,                                                  \
                              &RegValue,                                                \
                              hConfig,                                                  \
                              &String,                                                  \
                              NdisParameterInteger);                                    \
        if (Status==NDIS_STATUS_SUCCESS)                                                \
        {                                                                               \
            (Var) = RegValue->ParameterData.IntegerData;                                \
            DEBUGMSG(DBG_INIT, (DTEXT(#Var"==%d\n"), (Var)));                           \
        }                                                                               \
        else                                                                            \
        {                                                                               \
            DEBUGMSG(DBG_INIT, (DTEXT(Value", default==%d\n"), (Var)));\
        }                                                                               \
    }

#define READ_NDIS_REG_USHORT(hConfig, Var, Value) \
    {                                                                                   \
        PNDIS_CONFIGURATION_PARAMETER RegValue;                                         \
        NDIS_STATUS Status;                                                             \
        NDIS_STRING String = NDIS_STRING_CONST(Value);                                  \
                                                                                        \
        NdisReadConfiguration(&Status,                                                  \
                              &RegValue,                                                \
                              hConfig,                                                  \
                              &String,                                                  \
                              NdisParameterInteger);                                    \
        if (Status==NDIS_STATUS_SUCCESS)                                                \
        {                                                                               \
            (Var) = (USHORT)(RegValue->ParameterData.IntegerData&0xffff);               \
            DEBUGMSG(DBG_INIT, (DTEXT(#Var"==%d\n"), (Var)));                           \
        }                                                                               \
        else                                                                            \
        {                                                                               \
            DEBUGMSG(DBG_INIT, (DTEXT(Value", default==%d\n"), (Var)));\
        }                                                                               \
    }

#define READ_NDIS_REG_BOOL(hConfig, Var, Value) \
    {                                                                                   \
        PNDIS_CONFIGURATION_PARAMETER RegValue;                                         \
        NDIS_STATUS Status;                                                             \
        NDIS_STRING String = NDIS_STRING_CONST(Value);                                  \
                                                                                        \
        NdisReadConfiguration(&Status,                                                  \
                              &RegValue,                                                \
                              hConfig,                                                  \
                              &String,                                                  \
                              NdisParameterInteger);                                    \
        if (Status==NDIS_STATUS_SUCCESS)                                                \
        {                                                                               \
            (Var) = RegValue->ParameterData.IntegerData ? TRUE : FALSE;                 \
            DEBUGMSG(DBG_INIT, (DTEXT(#Var"==%d\n"), (Var)));                           \
        }                                                                               \
        else                                                                            \
        {                                                                               \
            DEBUGMSG(DBG_INIT, (DTEXT(Value", default==%d\n"), (Var)));\
        }                                                                               \
    }
    

ULONG ReadClientAddressAndMaskList(
    IN NDIS_HANDLE hConfig,
    IN PNDIS_STRING IpAddressesString,
    IN PNDIS_STRING IpMasksString,
    IN OUT PCLIENT_ADDRESS *pClientAddressList
    )
    //
    // Read the IP addresses and masks in multi string formats
    //
{
    NDIS_STATUS Status = NDIS_STATUS_FAILURE;
    PNDIS_CONFIGURATION_PARAMETER Value = NULL;
    ULONG ulNumIpAddresses = 0;
    PCLIENT_ADDRESS ClientList = NULL;
    
    *pClientAddressList = NULL;

    NdisReadConfiguration(&Status,  // Not required value
                          &Value,
                          hConfig,
                          IpAddressesString,
                          NdisParameterMultiString);
    if (Status==NDIS_STATUS_SUCCESS)
    {
    
        ULONG i, ulAddressIndex = 0;
        BOOLEAN InEntry, ValidAddress;
        PWCHAR AddressList = Value->ParameterData.StringData.Buffer;
        TA_IP_ADDRESS Address;

        // Loop and count the addresses, so we can allocate proper size to hold them.
        for (i=0, InEntry=FALSE; i<(Value->ParameterData.StringData.Length/sizeof(WCHAR))-1; i++)
        {
            if (!InEntry)
            {
                if (AddressList[i]!=L'\0')
                {
                    InEntry = TRUE;
                    StringToIpAddressW(&AddressList[i],
                                       &Address,
                                       &ValidAddress);
                    if (ValidAddress)
                    {
                        ulAddressIndex++;
                    }
                }
            }
            else
            {
                if (AddressList[i]==L'\0')
                {
                    InEntry = FALSE;
                }
            }
        }
        
        ulNumIpAddresses = ulAddressIndex;
        
        if (ulNumIpAddresses)
        {
            ClientList = MyMemAlloc(sizeof(CLIENT_ADDRESS)*ulNumIpAddresses, TAG_PPTP_ADDR_LIST);
            if (ClientList)
            {
                ulAddressIndex = 0;
                for (i=0, InEntry=FALSE; 
                    i<(Value->ParameterData.StringData.Length/sizeof(WCHAR))-1 && (ulAddressIndex < ulNumIpAddresses); 
                    i++)
                {
                    if (!InEntry)
                    {
                        if (AddressList[i]!=L'\0')
                        {
                            InEntry = TRUE;
                            StringToIpAddressW(&AddressList[i],
                                               &Address,
                                               &ValidAddress);
                            if (ValidAddress)
                            {
                                ClientList[ulAddressIndex].Address = Address.Address[0].Address[0].in_addr;
                                ClientList[ulAddressIndex].Mask = 0xFFFFFFFF;
                                ulAddressIndex++;
                            }
                        }
                    }
                    else
                    {
                        if (AddressList[i]==L'\0')
                        {
                            InEntry = FALSE;
                        }
                    }
                }
                
                NdisReadConfiguration(&Status,      // Not required value
                                      &Value,
                                      hConfig,
                                      IpMasksString,
                                      NdisParameterMultiString);
                if (Status==NDIS_STATUS_SUCCESS)
                {
                    AddressList = Value->ParameterData.StringData.Buffer;
                    ulAddressIndex = 0;
                    for (i=0, InEntry=FALSE;
                         i<(Value->ParameterData.StringData.Length/sizeof(WCHAR))-1 && (ulAddressIndex < ulNumIpAddresses);
                         i++)
                    {
                        if (!InEntry)
                        {
                            if (AddressList[i]!=L'\0')
                            {
                                InEntry = TRUE;
                                StringToIpAddressW(&AddressList[i],
                                                   &Address,
                                                   &ValidAddress);
                                if (ValidAddress)
                                {
                                    ClientList[ulAddressIndex].Mask = Address.Address[0].Address[0].in_addr;
                                    ulAddressIndex++;
                                }
                            }
                        }
                        else
                        {
                            if (AddressList[i]==L'\0')
                            {
                                InEntry = FALSE;
                            }
                        }
                    }
                }
                
                for (i=0; i<ulNumIpAddresses; i++)
                {
                    DEBUGMSG(DBG_INIT, (DTEXT("Client Address:%d.%d.%d.%d  Mask:%d.%d.%d.%d\n"),
                                        IPADDR(ClientList[i].Address), IPADDR(ClientList[i].Mask)));
                }
            }
            else
            {
                ulNumIpAddresses = 0;
            }
        }
    }
    
    if(ulNumIpAddresses)
    {
        *pClientAddressList = ClientList;
    }
    else
    {
        ASSERT(ClientList == NULL);
    }
    
    return ulNumIpAddresses;
}

    

VOID
OsReadConfig(
    NDIS_HANDLE hConfig
    )
{
    NDIS_STATUS Status = NDIS_STATUS_SUCCESS;
    PNDIS_CONFIGURATION_PARAMETER Value;
    
    NDIS_STRING TapiLineNameString = NDIS_STRING_CONST("TapiLineName");
#if VER_PRODUCTVERSION_W < 0x0500
    NDIS_STRING TapiLineAddrString = NDIS_STRING_CONST("AddressList");
#endif
    NDIS_STRING PeerClientIpAddressString = NDIS_STRING_CONST("ClientIpAddresses");
    NDIS_STRING PeerClientIpMaskString = NDIS_STRING_CONST("ClientIpMasks");
    
    NDIS_STRING TrustedIpAddressString = NDIS_STRING_CONST("TrustedIpAddresses");
    NDIS_STRING TrustedIpMaskString = NDIS_STRING_CONST("TrustedIpMasks");
                                                              
    DEBUGMSG(DBG_FUNC, (DTEXT("+OsReadConfig\n")));

    READ_NDIS_REG_USHORT(hConfig, PptpControlPort,                  "TcpPortNumber");
    READ_NDIS_REG_ULONG (hConfig, PptpWanEndpoints,                 "MaxWanEndpoints");
    READ_NDIS_REG_ULONG (hConfig, PptpBaseCallId,                   "BaseCallId");
    READ_NDIS_REG_ULONG (hConfig, PptpMaxTransmit,                  "MaxTransmit");
    READ_NDIS_REG_ULONG (hConfig, PptpEchoTimeout,                  "InactivityIdleSeconds");
    READ_NDIS_REG_BOOL  (hConfig, PptpEchoAlways,                   "AlwaysEcho");
    READ_NDIS_REG_ULONG (hConfig, CtdiTcpDisconnectTimeout,         "TcpDisconnectTimeout");
    READ_NDIS_REG_ULONG (hConfig, CtdiTcpConnectTimeout,            "TcpConnectTimeout");
    READ_NDIS_REG_BOOL  (hConfig, PptpAuthenticateIncomingCalls,    "AuthenticateIncomingCalls");
    READ_NDIS_REG_ULONG (hConfig, PptpTraceMask,                    "PptpTraceMask");
    READ_NDIS_REG_ULONG (hConfig, PptpValidateAddress,              "ValidateAddress");
    READ_NDIS_REG_ULONG (hConfig, PptpMaxTunnelsPerIpAddress,       "MaxTunnelsPerIpAddress");
    
    // Validate some reg values
    if(PptpWanEndpoints == 0)
    {
        PptpWanEndpoints = OS_DEFAULT_WAN_ENDPOINTS;
    }
    if(PptpMaxTunnelsPerIpAddress > PptpWanEndpoints)
    {
        PptpMaxTunnelsPerIpAddress = -1;
    }

    OS_RANGE_CHECK_ENDPOINTS(PptpWanEndpoints, PptpBaseCallId);
    OS_RANGE_CHECK_MAX_TRANSMIT(PptpMaxTransmit);

    // Read client IP addresses/masks to accept (old behavior)  
    if(PptpAuthenticateIncomingCalls)
    {
        DEBUGMSG(DBG_INIT, (DTEXT("Accept IP Addresses/Masks\n")));
        g_ulAcceptClientAddresses = ReadClientAddressAndMaskList(
                hConfig,
                &PeerClientIpAddressString,
                &PeerClientIpMaskString,
                &g_AcceptClientList);
    }
    
    // Read trusted client IP addresses/masks
    DEBUGMSG(DBG_INIT, (DTEXT("Trsuted IP Addresses/Masks\n")));
    g_ulTrustedClientAddresses = ReadClientAddressAndMaskList(
        hConfig,
        &TrustedIpAddressString,
        &TrustedIpMaskString,
        &g_TrustedClientList);
            
    NdisReadConfiguration(&Status,  // Not required value
                          &Value,
                          hConfig,
                          &TapiLineNameString,
                          NdisParameterString);
    if (Status==NDIS_STATUS_SUCCESS)
    {
        RtlUnicodeStringToAnsiString(&TapiLineName, &Value->ParameterData.StringData, FALSE);
        *(TAPI_CHAR_TYPE*)(TapiLineName.Buffer+TapiLineName.MaximumLength-sizeof(TAPI_CHAR_TYPE)) = (TAPI_CHAR_TYPE)0;
    }

#if VER_PRODUCTVERSION_W < 0x0500
    NdisReadConfiguration(&Status,  // Not required value
                          &Value,
                          hConfig,
                          &TapiLineAddrString,
                          NdisParameterMultiString);
    if (Status==NDIS_STATUS_SUCCESS)
    {
        RtlInitAnsiString( &TapiLineAddrList, NULL );
        if (RtlUnicodeStringToAnsiString(&TapiLineAddrList, &Value->ParameterData.StringData, TRUE)==NDIS_STATUS_SUCCESS)
        {
            // NT4 doesn't have the same registry value to tell us how many lines to publish.
            // We work around that by counting the number of address strings here

            PUCHAR p = TapiLineAddrList.Buffer;

            DEBUGMEM(DBG_TAPI, TapiLineAddrList.Buffer, TapiLineAddrList.Length, 1);
            PptpWanEndpoints = 0;
            if (p)
            {
                // Count the valid MULTI_SZ entries.
                while (*p++)
                {
                    PptpWanEndpoints++;
                    while (*p++);  // This also skips the first NULL
                }
            }
            DBG_D(DBG_INIT, PptpWanEndpoints);
        }

    }
#endif

    DEBUGMSG(DBG_FUNC, (DTEXT("-OsReadConfig\n")));
}

#if 0
VOID OsGetFullHostName(VOID)
{
    NTSTATUS status;
    OBJECT_ATTRIBUTES objattr;
    UNICODE_STRING uni;
    HANDLE hParams;
    KEY_VALUE_PARTIAL_INFORMATION* pNameValue;
    ULONG ulAllocSize, ulSize, ulIndex, ulDomainIndex;

    hParams = NULL;
    pNameValue = NULL;
    
    NdisZeroMemory(PptpHostName, MAX_HOSTNAME_LENGTH);

    do
    {
        // Get a handle to the TCPIP Parameters registry key.
        RtlInitUnicodeString(
            &uni,
            L"\\Registry\\Machine\\System\\CurrentControlSet\\Services\\Tcpip\\Parameters" );
        InitializeObjectAttributes(
            &objattr, &uni, OBJ_CASE_INSENSITIVE, NULL, NULL );

        status = ZwOpenKey(&hParams, KEY_QUERY_VALUE, &objattr);
        if (status != STATUS_SUCCESS)
        {
            break;
        }

        ulAllocSize = sizeof(KEY_VALUE_PARTIAL_INFORMATION) + MAX_HOSTNAME_LENGTH * 2;
        // Query the "Hostname" registry value.
        pNameValue = MyMemAlloc(ulAllocSize, TAG_REG);
        if (!pNameValue)
        {
            break;
        }

        RtlInitUnicodeString( &uni, L"Hostname");
        status = ZwQueryValueKey(
            hParams, &uni, KeyValuePartialInformation,
            pNameValue, ulAllocSize, &ulSize );
        if (status != STATUS_SUCCESS || pNameValue->Type != REG_SZ || pNameValue->DataLength < sizeof(WCHAR) * 2)
        {
            break;
        }
        
        // Convert host unicode to ASCII, put it in PptpHostName;
        ulSize = min(pNameValue->DataLength/2, MAX_HOSTNAME_LENGTH);
        for(ulIndex = 0; ulIndex < ulSize; ulIndex++)
        {
            PptpHostName[ulIndex] = pNameValue->Data[ulIndex*2];
        }
        
        // Query the "Domain" registry value.
        RtlInitUnicodeString( &uni, L"Domain");
        status = ZwQueryValueKey(
            hParams, &uni, KeyValuePartialInformation,
            pNameValue, ulAllocSize, &ulSize );

        // Convert domain unicode to ASCII, append it in PptpHostName;
        if (status == STATUS_SUCCESS
            && pNameValue->Type == REG_SZ
            && pNameValue->DataLength > sizeof(WCHAR)
            && ((WCHAR* )pNameValue->Data)[ 0 ] != L'\0')
        {
            PptpHostName[--ulIndex] = '.';
            ++ulIndex;
            
            for(ulDomainIndex = 0; 
                ulIndex < MAX_HOSTNAME_LENGTH && ulDomainIndex < pNameValue->DataLength/2; 
                ulIndex++, ulDomainIndex++)
            {
                PptpHostName[ulIndex] = pNameValue->Data[ulDomainIndex*2];
            }
        }
        
        PptpHostName[MAX_HOSTNAME_LENGTH -1] = '\0';
    }
    while (FALSE);

    if(hParams)
    {
        ZwClose(hParams);
    }

    if(pNameValue)
    {
        MyMemFree(pNameValue, sizeof(KEY_VALUE_PARTIAL_INFORMATION) + MAX_HOSTNAME_LENGTH * 2);
    }
}

#endif

VOID
OsGetTapiLineAddress(ULONG Index, PUCHAR s, ULONG Length)
{
    ASSERT(Length);

#if VER_PRODUCTVERSION_W < 0x0500
    PUCHAR pAddr = TapiLineAddrList.Buffer;

    *s = 0;

    if (pAddr)
    {
        UINT i;

        for (i=0; i<Index; i++)
        {
            if (!*pAddr)
            {
                // No string at index
                return;
            }
            while (*pAddr) pAddr++;
            pAddr++;
        }
        strncpy(s, pAddr, Length);
        s[Length-1] = 0;
    }
#else // VER_PRODUCTVERSION_W >= 0x0500
    strncpy(s, TAPI_LINE_ADDR_STRING, Length);
    s[Length-1] = 0;
#endif
}

NDIS_STATUS
OsSpecificTapiGetDevCaps(
    ULONG_PTR ulDeviceId,
    IN OUT PNDIS_TAPI_GET_DEV_CAPS pRequest
    )
{
    PUCHAR pTmp, pTmp2;
    ULONG_PTR Index;

    DEBUGMSG(DBG_FUNC, (DTEXT("+OsSpecificTapiGetDevCaps\n")));

    // Convert to our internal index
    ulDeviceId -= pgAdapter->Tapi.DeviceIdBase;

    pRequest->LineDevCaps.ulStringFormat = STRINGFORMAT_ASCII;


    // The *6 at the end adds enough space for " 9999"
    pRequest->LineDevCaps.ulNeededSize   = sizeof(pRequest->LineDevCaps) +
                                           sizeof(TAPI_PROVIDER_STRING) +
                                           TapiLineName.Length +
                                           sizeof(TAPI_CHAR_TYPE) * 6;

    if (pRequest->LineDevCaps.ulTotalSize<pRequest->LineDevCaps.ulNeededSize)
    {
        DEBUGMSG(DBG_FUNC|DBG_WARN, (DTEXT("-TapiGetDevCaps NDIS_STATUS_SUCCESS without PROVIDER or LINE_NAME strings\n")));
        return NDIS_STATUS_SUCCESS;
    }

    // Tack the provider string to the end of the LineDevCaps structure.

    pRequest->LineDevCaps.ulProviderInfoSize = sizeof(TAPI_PROVIDER_STRING);
    pRequest->LineDevCaps.ulProviderInfoOffset = sizeof(pRequest->LineDevCaps);

    pTmp = ((PUCHAR)&pRequest->LineDevCaps) + sizeof(pRequest->LineDevCaps);
    NdisMoveMemory(pTmp, TAPI_PROVIDER_STRING, sizeof(TAPI_PROVIDER_STRING));

    pTmp += sizeof(TAPI_PROVIDER_STRING);

    // Tack on the LineName after the provider string.

    pRequest->LineDevCaps.ulLineNameSize = TapiLineName.Length + sizeof(TAPI_CHAR_TYPE);
    pRequest->LineDevCaps.ulLineNameOffset = pRequest->LineDevCaps.ulProviderInfoOffset +
                                             pRequest->LineDevCaps.ulProviderInfoSize;
    NdisMoveMemory(pTmp, TapiLineName.Buffer, TapiLineName.Length+sizeof(TAPI_CHAR_TYPE));

    while (*pTmp) pTmp++; // Find the NULL

    *pTmp++ = ' ';
    pRequest->LineDevCaps.ulLineNameSize++;

    // Put a number at the end of the string.

    if (ulDeviceId==0)
    {
        *pTmp++ = '0';
        *pTmp++ = '\0';
        pRequest->LineDevCaps.ulLineNameSize += 2;
    }
    else
    {
        Index = ulDeviceId;
        ASSERT(Index<100000);
        if(Index >= 100000)
        {
            // Index value is usually small, but just make sure it's within boundary
            Index = 99999;
        }
        pTmp2 = pTmp;
        while (Index)
        {
            *pTmp2++ = (UCHAR)((Index%10) + '0');
            Index /= 10;
            pRequest->LineDevCaps.ulLineNameSize++;
        }
        *pTmp2-- = '\0'; // Null terminate and point to the last digit.
        pRequest->LineDevCaps.ulLineNameSize++;
        // We put the number in backwards, now reverse it.
        while (pTmp<pTmp2)
        {
            UCHAR t = *pTmp;
            *pTmp++ = *pTmp2;
            *pTmp2-- = t;
        }
    }

    pRequest->LineDevCaps.ulUsedSize     = pRequest->LineDevCaps.ulNeededSize;

    DEBUGMSG(DBG_FUNC, (DTEXT("-OsSpecificTapiGetDevCaps\n")));
    return NDIS_STATUS_SUCCESS;
}
