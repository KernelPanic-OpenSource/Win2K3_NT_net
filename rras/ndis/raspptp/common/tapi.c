/*****************************************************************************
*
*   Copyright (c) 1998-1999 Microsoft Corporation
*
*   TAPI.C - TAPI handling functions
*
*   Author:     Stan Adermann (stana)
*
*   Created:    9/17/1998
*
*****************************************************************************/

#include "raspptp.h"

#include "tapi.tmh"

NDIS_STATUS
TapiAnswer(
    IN PPPTP_ADAPTER pAdapter,
    IN PNDIS_TAPI_ANSWER pRequest
    )
{
    PCALL_SESSION pCall = NULL;
    PCONTROL_TUNNEL pCtl;
    BOOLEAN LockHeld = FALSE;
    NDIS_STATUS Status = NDIS_STATUS_SUCCESS;

    DEBUGMSG(DBG_FUNC, (DTEXT("+TapiAnswer\n")));
    
    if ( pRequest == NULL || pAdapter == NULL )
    {
        gCounters.ulAnswerNullRequest++;
        
        DEBUGMSG(DBG_ERROR|DBG_FUNC, (DTEXT("-TapiAnswer NDIS_STATUS_TAPI_INVALPARAM\n")));
        WPLOG(LL_A, LM_TAPI, ("-NDIS_STATUS_TAPI_INVALPARAM"));
        return NDIS_STATUS_TAPI_INVALPARAM;
    }

    pCall = CallGetCall(pAdapter, pRequest->hdCall);
    
    // Verify the ID
    if (!pCall)
    {
        gCounters.ulAnswerNullCall++;
        
        DEBUGMSG(DBG_ERROR|DBG_FUNC, (DTEXT("-TapiAnswer NDIS_STATUS_TAPI_INVALCALLHANDLE\n")));
        WPLOG(LL_A, LM_TAPI, ("-NDIS_STATUS_TAPI_INVALCALLHANDLE"));
        return NDIS_STATUS_TAPI_INVALCALLHANDLE;
    }

    DBGTRACE('A');
    WPLOG(LL_I, LM_TAPI, ("Answer Cid %d", (ULONG)pCall->DeviceId));

    NdisAcquireSpinLock(&pCall->Lock);
    LockHeld = TRUE;
    
    pCtl = pCall->pCtl;

    if (!pCtl)
    {
        DBGTRACE('t');
        gCounters.ulAnswerNullControl++;
    
        DEBUGMSG(DBG_WARN, (DTEXT("TapiAnswer: No control tunnel Cid %d\n"), pCall->DeviceId));
        WPLOG(LL_A, LM_TAPI, ("No control tunnel Cid %d", (ULONG)pCall->DeviceId));
        Status = NDIS_STATUS_FAILURE;
    }
    else if (pCall->State==STATE_CALL_OFFERING)
    {
        USHORT NewCallId;
        PPTP_CALL_OUT_REPLY_PACKET *pReply = CtlAllocPacket(pCtl, CALL_OUT_REPLY);

        CallAssignSerialNumber(pCall);
        if(PptpClientSide)
        {
            NewCallId = (USHORT)((pCall->SerialNumber << CALL_ID_INDEX_BITS) + pCall->DeviceId);
            if (pCall->Packet.CallId == NewCallId)
            {
                // Don't allow a line to have the same CallId twice in a row.
                NewCallId += (1<<CALL_ID_INDEX_BITS);
            }
        }
        else
        {
            NewCallId = (USHORT)pCall->FullDeviceId;
        }
        pCall->Packet.CallId = NewCallId;

        if (!pReply)
        {
            DBGTRACE('r');
            gCounters.ulAnswerNullReply++;
            
            DEBUGMSG(DBG_WARN, (DTEXT("TapiAnswer: Failed to alloc a reply packet Cid %d\n"), pCall->DeviceId));
            WPLOG(LL_A, LM_TAPI, ("Failed to alloc a reply packet Cid %d", (ULONG)pCall->DeviceId));
                                                  
            Status = NDIS_STATUS_RESOURCES;
        }
        else
        {
            pCall->Close.Checklist &= ~(CALL_CLOSE_DROP|
                                        CALL_CLOSE_DROP_COMPLETE|
                                        CALL_CLOSE_CLEANUP_STATE);
                                        
            CallSetState(pCall, STATE_CALL_ESTABLISHED, 0, LockHeld);

            pCall->Speed = pCtl->Speed;
            // pCtl is safe to touch because it can't be released from the call
            // without the call spinlock.

            NdisReleaseSpinLock(&pCall->Lock);
            LockHeld = FALSE;

            pReply->PeerCallId = htons(pCall->Remote.CallId);
            pReply->CallId = htons(pCall->Packet.CallId);
            pReply->ResultCode = RESULT_CALL_OUT_CONNECTED;
            pReply->RecvWindowSize = PPTP_RECV_WINDOW;
            pReply->ConnectSpeed = pCall->Speed;
            pReply->ProcessingDelay = 0;
            pReply->PhysicalChannelId = 0;
            
            WPLOG(LL_M, LM_TUNNEL, ("SEND CALL_OUT_REPLY -> %!IPADDR! Cid %d, Pkt-Cid %d, Peer's Cid %d", 
                pCall->Remote.Address.Address[0].Address[0].in_addr, 
                (ULONG)pCall->DeviceId, pCall->Packet.CallId, pCall->Remote.CallId));
            
            Status = CtlSend(pCtl, pReply);
            if (Status!=NDIS_STATUS_SUCCESS)
            {
                DBGTRACE('s');
                gCounters.ulAnswerCtlSendFail++;
            
                DEBUGMSG(DBG_WARN, (DTEXT("TapiAnswer: Failed to send CALL_OUT_REPLY Cid %d\n"), pCall->DeviceId));
                WPLOG(LL_A, LM_TAPI, ("Failed to send CALL_OUT_REPLY Cid %d", (ULONG)pCall->DeviceId));
                
                CallSetState(pCall, STATE_CALL_CLEANUP, LINEDISCONNECTMODE_NORMAL, LockHeld);
                CallCleanup(pCall, LockHeld);
            }
        }
    }
    else if (pCall->State==STATE_CALL_PAC_OFFERING)
    {
        USHORT NewCallId;
        PPTP_CALL_OUT_REPLY_PACKET *pReply = CtlAllocPacket(pCtl, CALL_IN_REPLY);

        CallAssignSerialNumber(pCall);
        if(PptpClientSide)
        {
            NewCallId = (USHORT)((pCall->SerialNumber << CALL_ID_INDEX_BITS) + pCall->DeviceId);
            if (pCall->Packet.CallId == NewCallId)
            {
                // Don't allow a line to have the same CallId twice in a row.
                NewCallId += (1<<CALL_ID_INDEX_BITS);
            }
        }
        else
        {
            NewCallId = (USHORT)pCall->FullDeviceId;
        }
        pCall->Packet.CallId = NewCallId;

        if (!pReply)
        {
            DBGTRACE('r');
            gCounters.ulAnswerNullReply++;
            
            DEBUGMSG(DBG_WARN, (DTEXT("TapiAnswer: Failed to alloc a reply packet Cid %d\n"), pCall->DeviceId));
            WPLOG(LL_A, LM_TAPI, ("Failed to alloc a reply packet Cid %d", (ULONG)pCall->DeviceId));
                
            Status = NDIS_STATUS_RESOURCES;
        }
        else
        {
            pCall->Close.Checklist &= ~(CALL_CLOSE_DROP|
                                        CALL_CLOSE_DROP_COMPLETE|
                                        CALL_CLOSE_CLEANUP_STATE);
            CallSetState(pCall, STATE_CALL_PAC_WAIT, 0, LockHeld);
            
            NdisReleaseSpinLock(&pCall->Lock);
            LockHeld = FALSE;
            
            pReply->PeerCallId = htons(pCall->Remote.CallId);
            pReply->CallId = htons(pCall->Packet.CallId);
            pReply->ResultCode = RESULT_CALL_IN_CONNECTED;
            pReply->RecvWindowSize = PPTP_RECV_WINDOW;
            pReply->ProcessingDelay = 0;
            Status = CtlSend(pCtl, pReply);
            if (Status!=NDIS_STATUS_SUCCESS)
            {
                CallSetState(pCall, STATE_CALL_CLEANUP, LINEDISCONNECTMODE_NORMAL, LockHeld);
                CallCleanup(pCall, LockHeld);
            }
        }
    }
    else
    {
        DEBUGMSG(DBG_WARN, (DTEXT("Wrong state for TapiAnswer %d\n"), pCall->State));
        WPLOG(LL_A, LM_TAPI, ("Wrong state for TapiAnswer %d", pCall->State));
        Status = NDIS_STATUS_FAILURE;
    }

    if (LockHeld)
    {
        NdisReleaseSpinLock(&pCall->Lock);
    }
                
    DEBUGMSG(DBG_FUNC|DBG_ERR(Status), (DTEXT("-TapiAnswer %08x\n"), Status));
    return Status;
}

NDIS_STATUS
TapiClose(
    IN PPPTP_ADAPTER pAdapter,
    IN PNDIS_TAPI_CLOSE pRequest
    )
{
    PCALL_SESSION pCall;
    NDIS_STATUS Status = NDIS_STATUS_SUCCESS;

    DEBUGMSG(DBG_FUNC, (DTEXT("+TapiClose\n")));
    WPLOG(LL_I, LM_TAPI, ("enter"));

    if ( pRequest == NULL || pAdapter == NULL )
    {
        DEBUGMSG(DBG_ERROR|DBG_FUNC, (DTEXT("-TapiClose NDIS_STATUS_TAPI_INVALPARAM\n")));
        WPLOG(LL_A, LM_TAPI, ("-NDIS_STATUS_TAPI_INVALPARAM"));
        return NDIS_STATUS_TAPI_INVALPARAM;
    }

    if (!CallIsValidCall(pAdapter, TapiLineHandleToId(pRequest->hdLine)))
    {
        DEBUGMSG(DBG_ERROR|DBG_FUNC, (DTEXT("-TapiClose NDIS_STATUS_TAPI_INVALLINEHANDLE\n")));
        WPLOG(LL_A, LM_TAPI, ("-NDIS_STATUS_TAPI_INVALLINEHANDLE"));
        return NDIS_STATUS_TAPI_INVALLINEHANDLE;
    }

    NdisAcquireSpinLock(&pAdapter->Lock);
    pAdapter->Tapi.Open = FALSE;
    NdisReleaseSpinLock(&pAdapter->Lock);
    if (pAdapter->hCtdiListen)
    {
        // We have to pend this request until the listen endpoint is closed
        CtdiSetRequestPending(pAdapter->hCtdiListen);
        CtdiClose(pAdapter->hCtdiListen);
        pAdapter->hCtdiListen = NULL;
        Status = NDIS_STATUS_PENDING;
    }

    DEBUGMSG(DBG_FUNC, (DTEXT("-TapiClose\n")));
    WPLOG(LL_I, LM_TAPI, ("exit"));
    return Status;
}

NDIS_STATUS
TapiCloseCall(
    IN PPPTP_ADAPTER pAdapter,
    IN PNDIS_TAPI_CLOSE_CALL pRequest
    )
{
    PCALL_SESSION pCall;
    DEBUGMSG(DBG_FUNC, (DTEXT("+TapiCloseCall\n")));

    if ( pRequest == NULL || pAdapter == NULL )
    {
        gCounters.ulCloseCallNullRequest++;
    
        DEBUGMSG(DBG_ERROR|DBG_FUNC, (DTEXT("-TapiCloseCall NDIS_STATUS_TAPI_INVALPARAM\n")));
        WPLOG(LL_A, LM_TAPI, ("-NDIS_STATUS_TAPI_INVALPARAM"));
        return NDIS_STATUS_TAPI_INVALPARAM;
    }

    // Verify the ID
    pCall = CallGetCall(pAdapter, pRequest->hdCall);

    if (!pCall)
    {
        gCounters.ulCloseCallNullCall++;
    
        DEBUGMSG(DBG_FUNC|DBG_ERROR, (DTEXT("-TapiCloseCall NDIS_STATUS_TAPI_INVALCALLHANDLE\n")));
        WPLOG(LL_A, LM_TAPI, ("-NDIS_STATUS_TAPI_INVALCALLHANDLE"));
        return NDIS_STATUS_TAPI_INVALLINEHANDLE;
    }
    
    DBGTRACE('C');
    WPLOG(LL_M, LM_TAPI, ("Cid %d", (ULONG)pCall->DeviceId));
    
    NdisAcquireSpinLock(&pCall->Lock);
    pCall->Close.Checklist |= CALL_CLOSE_CLOSE_CALL;
    pCall->hTapiCall = 0;
    CallCleanup(pCall, LOCKED);
    NdisReleaseSpinLock(&pCall->Lock);

    DEBUGMSG(DBG_FUNC, (DTEXT("-TapiCloseCall\n")));
    return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
TapiDrop(
    IN PPPTP_ADAPTER pAdapter,
    IN PNDIS_TAPI_DROP pRequest
    )
{
    PCALL_SESSION pCall;
    PCONTROL_TUNNEL pCtl;
    NDIS_STATUS Status = NDIS_STATUS_SUCCESS;
    BOOLEAN CleanupNow = FALSE;

    DEBUGMSG(DBG_FUNC, (DTEXT("+TapiDrop\n")));
        
    if ( pRequest == NULL || pAdapter == NULL )
    {
        gCounters.ulDropNullRequest++;
    
        DEBUGMSG(DBG_ERROR|DBG_FUNC, (DTEXT("-TapiDrop NDIS_STATUS_TAPI_INVALPARAM\n")));
        WPLOG(LL_A, LM_TAPI, ("-NDIS_STATUS_TAPI_INVALPARAM"));
        return NDIS_STATUS_TAPI_INVALPARAM;
    }
    
    // Verify the ID
    pCall = CallGetCall(pAdapter, pRequest->hdCall);

    if (!pCall)
    {
        gCounters.ulDropNullCall++;
    
        DEBUGMSG(DBG_FUNC|DBG_ERROR, (DTEXT("-TapiDrop NDIS_STATUS_TAPI_INVALCALLHANDLE\n")));
        WPLOG(LL_A, LM_TAPI, ("-NDIS_STATUS_TAPI_INVALCALLHANDLE"));
        return NDIS_STATUS_TAPI_INVALCALLHANDLE;
    }

    DBGTRACE('D');
    WPLOG(LL_M, LM_TAPI, ("Drop pCall %p, Cid %d, pCtl %p",
        pCall, (ULONG)pCall->DeviceId, pCall->pCtl));
        
    NdisAcquireSpinLock(&pCall->Lock);

    //ASSERT(!(pCall->Close.Checklist&CALL_CLOSE_DROP));

    pCtl = pCall->pCtl;

    if (!pCtl)
    {
        DBGTRACE('t');
        gCounters.ulDropNullControl++;
    
        DEBUGMSG(DBG_WARN, (DTEXT("TapiDrop: No control tunnel Cid %d\n"), pCall->DeviceId));
        WPLOG(LL_I, LM_TAPI, ("No control tunnel Cid %d", (ULONG)pCall->DeviceId));
        Status = NDIS_STATUS_FAILURE;
    }
    else if (pCall->State==STATE_CALL_ESTABLISHED)
    {
        PPPTP_CALL_CLEAR_REQUEST_PACKET pPacket = CtlAllocPacket(pCtl, CALL_CLEAR_REQUEST);

        if (!pPacket)
        {
            DBGTRACE('p');
            gCounters.ulDropNullPacket++;
        
            DEBUGMSG(DBG_WARN, (DTEXT("TapiDrop: Failed to alloc CALL_CLEAR_REQUEST Cid %d\n"), pCall->DeviceId));
            WPLOG(LL_A, LM_TAPI, ("Failed to alloc CALL_CLEAR_REQUEST Cid %d", (ULONG)pCall->DeviceId));
            Status = NDIS_STATUS_RESOURCES;
        }
        else
        {
            CallSetState(pCall, STATE_CALL_WAIT_DISCONNECT, 0, LOCKED);
            pPacket->CallId = htons(pCall->Packet.CallId);

            NdisReleaseSpinLock(&pCall->Lock);
            
            WPLOG(LL_M, LM_TUNNEL, ("SEND CALL_CLEAR_REQUEST -> %!IPADDR! pCall %p, Cid %d, Pkt-Cid %d", 
                pCtl->Remote.Address.Address[0].Address[0].in_addr, 
                pCall, (ULONG)pCall->DeviceId, pCall->Packet.CallId));
            
            Status = CtlSend(pCtl, pPacket);
            if (Status==NDIS_STATUS_SUCCESS)
            {
                Status = NDIS_STATUS_PENDING;
            }
            else             
            {
                DBGTRACE('s');
                gCounters.ulDropCtlSendFail++;
                DEBUGMSG(DBG_WARN, (DTEXT("TapiDrop: Failed to send CALL_CLEAR_REQUEST Cid %d\n"), pCall->DeviceId));
                WPLOG(LL_A, LM_TAPI, ("Failed to send CALL_CLEAR_REQUEST Cid %d", (ULONG)pCall->DeviceId));
                
            }
            if (Status==NDIS_STATUS_PENDING)
            {
                NdisMSetTimer(&pCall->Close.Timer, PPTP_CLOSE_TIMEOUT);
            }
            NdisAcquireSpinLock(&pCall->Lock);
        }
    }
    else if (pCall->State!=STATE_CALL_CLEANUP)
    {
        CallSetState(pCall, STATE_CALL_CLEANUP, 0, LOCKED);
        CleanupNow = TRUE;
    }

    pCall->Close.Checklist |= CALL_CLOSE_DROP;
    if (Status==NDIS_STATUS_PENDING)
    {
        pCall->Close.Checklist &= ~CALL_CLOSE_DROP_COMPLETE;
    }
    else
    {
        pCall->Close.Checklist |= CALL_CLOSE_DROP_COMPLETE;
    }
    if (CleanupNow)
    {
        CallCleanup(pCall, LOCKED);
    }
    NdisReleaseSpinLock(&pCall->Lock);

    DEBUGMSG(DBG_FUNC|DBG_ERR(Status), (DTEXT("-TapiDrop %08x\n"), Status));
    return Status;
}

NDIS_STATUS
TapiGetAddressCaps(
    IN PPPTP_ADAPTER pAdapter,
    IN OUT PNDIS_TAPI_GET_ADDRESS_CAPS pRequest
    )
{
    BOOLEAN ValidCall;
    CHAR LineAddress[TAPI_MAX_LINE_ADDRESS_LENGTH];
    DEBUGMSG(DBG_FUNC, (DTEXT("+TapiGetAddressCaps\n")));

    if ( pRequest == NULL || pAdapter == NULL )
    {
        DEBUGMSG(DBG_ERROR|DBG_FUNC, (DTEXT("-TapiGetAddressCaps NDIS_STATUS_TAPI_INVALPARAM\n")));
        WPLOG(LL_A, LM_TAPI, ("-NDIS_STATUS_TAPI_INVALPARAM"));
        return NDIS_STATUS_TAPI_INVALPARAM;
    }

    ValidCall = CallIsValidCall(pAdapter, pRequest->ulDeviceID);

    if (!ValidCall)
    {
        DEBUGMSG(DBG_ERROR|DBG_FUNC, (DTEXT("-TapiGetAddressCaps NDIS_STATUS_TAPI_NODRIVER\n")));
        WPLOG(LL_A, LM_TAPI, ("-NDIS_STATUS_TAPI_NODRIVER"));
        return NDIS_STATUS_TAPI_NODRIVER;
    }

    if (pRequest->ulAddressID >= TAPI_ADDR_PER_LINE)
    {
        DEBUGMSG(DBG_FUNC|DBG_ERROR, (DTEXT("-TapiGetAddressCaps NDIS_STATUS_TAPI_INVALADDRESSID\n")));
        WPLOG(LL_A, LM_TAPI, ("-NDIS_STATUS_TAPI_INVALADDRESSID"));
        return NDIS_STATUS_TAPI_INVALADDRESSID;

    }

    if (pRequest->ulExtVersion!=0 &&
        pRequest->ulExtVersion!=TAPI_EXT_VERSION)
    {
        DEBUGMSG(DBG_FUNC|DBG_ERROR, (DTEXT("-TapiGetAddressCaps NDIS_STATUS_TAPI_INCOMPATIBLEEXTVERSION\n")));
        WPLOG(LL_A, LM_TAPI, ("-NDIS_STATUS_TAPI_INCOMPATIBLEEXTVERSION"));
        return NDIS_STATUS_TAPI_INCOMPATIBLEEXTVERSION;
    }

    pRequest->LineAddressCaps.ulDialToneModes     = LINEDIALTONEMODE_NORMAL;
    pRequest->LineAddressCaps.ulSpecialInfo       = LINESPECIALINFO_UNAVAIL;

    pRequest->LineAddressCaps.ulDisconnectModes   = LINEDISCONNECTMODE_NORMAL |
                                                    LINEDISCONNECTMODE_UNKNOWN |
                                                    LINEDISCONNECTMODE_BUSY |
                                                    LINEDISCONNECTMODE_NOANSWER;

    pRequest->LineAddressCaps.ulMaxNumActiveCalls = pAdapter->Info.Endpoints;
    pRequest->LineAddressCaps.ulMaxNumTransConf   = 1;
    pRequest->LineAddressCaps.ulAddrCapFlags      = LINEADDRCAPFLAGS_DIALED;

    pRequest->LineAddressCaps.ulCallFeatures      = LINECALLFEATURE_ACCEPT |
                                                    LINECALLFEATURE_ANSWER |
                                                    LINECALLFEATURE_COMPLETECALL |
                                                    LINECALLFEATURE_DIAL |
                                                    LINECALLFEATURE_DROP;

    pRequest->LineAddressCaps.ulLineDeviceID      = pRequest->ulDeviceID;
    pRequest->LineAddressCaps.ulAddressSharing    = LINEADDRESSSHARING_PRIVATE;
    pRequest->LineAddressCaps.ulAddressStates     = 0;

    // List of all possible call states.

    pRequest->LineAddressCaps.ulCallStates        = LINECALLSTATE_IDLE |
                                                    LINECALLSTATE_OFFERING |
                                                    LINECALLSTATE_DIALING |
                                                    LINECALLSTATE_PROCEEDING |
                                                    LINECALLSTATE_CONNECTED |
                                                    LINECALLSTATE_DISCONNECTED;

    OsGetTapiLineAddress(DeviceIdToIndex(pAdapter, pRequest->ulDeviceID),
                         LineAddress,
                         sizeof(LineAddress));

    pRequest->LineAddressCaps.ulNeededSize = sizeof(pRequest->LineAddressCaps) +
                                             strlen(LineAddress) + 1;

    if (pRequest->LineAddressCaps.ulTotalSize<pRequest->LineAddressCaps.ulNeededSize)
    {
        pRequest->LineAddressCaps.ulUsedSize = 0;
        DEBUGMSG(DBG_FUNC|DBG_ERROR, (DTEXT("-TapiGetAddressCaps NDIS_STATUS_INVALID_LENGTH\n")));
        WPLOG(LL_A, LM_TAPI, ("-NDIS_STATUS_INVALID_LENGTH"));
        return NDIS_STATUS_INVALID_LENGTH;
    }

    pRequest->LineAddressCaps.ulAddressSize = strlen(LineAddress) + 1;
    pRequest->LineAddressCaps.ulAddressOffset = sizeof(pRequest->LineAddressCaps);
    strcpy((PUCHAR)((&pRequest->LineAddressCaps) + 1), LineAddress);

    pRequest->LineAddressCaps.ulUsedSize = pRequest->LineAddressCaps.ulNeededSize;

    DEBUGMSG(DBG_FUNC, (DTEXT("-TapiGetAddressCaps\n")));
    return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
TapiGetAddressStatus(
    IN PPPTP_ADAPTER pAdapter,
    IN OUT PNDIS_TAPI_GET_ADDRESS_STATUS pRequest
    )
{
    PCALL_SESSION pCall = NULL;
    BOOLEAN fReady;

    DEBUGMSG(DBG_FUNC, (DTEXT("+TapiGetAddressStatus\n")));

    if ( pRequest == NULL || pAdapter == NULL )
    {
        DEBUGMSG(DBG_ERROR|DBG_FUNC, (DTEXT("-TapiGetAddressStatus NDIS_STATUS_TAPI_INVALPARAM\n")));
        WPLOG(LL_A, LM_TAPI, ("-NDIS_STATUS_TAPI_INVALPARAM"));
        return NDIS_STATUS_TAPI_INVALPARAM;
    }

    
    DEBUGMSG(DBG_TAPI, (DTEXT("TapiGetAddressStatus: hdLine=%Xh, ulAddressID=%d\n"),
               pRequest->hdLine, pRequest->ulAddressID));


    if (!CallIsValidCall(pAdapter, TapiLineHandleToId(pRequest->hdLine)))
    {
        DEBUGMSG(DBG_FUNC|DBG_ERROR, (DTEXT("-TapiGetAddressStatus NDIS_STATUS_TAPI_INVALCALLHANDLE\n")));
        WPLOG(LL_A, LM_TAPI, ("-NDIS_STATUS_TAPI_INVALCALLHANDLE"));
        return NDIS_STATUS_TAPI_INVALCALLHANDLE;
    }

    if (pRequest->ulAddressID >= TAPI_ADDR_PER_LINE)
    {
        DEBUGMSG(DBG_FUNC|DBG_ERROR, (DTEXT("-TapiGetAddressStatus NDIS_STATUS_TAPI_INVALADDRESSID\n")));
        WPLOG(LL_A, LM_TAPI, ("-NDIS_STATUS_TAPI_INVALADDRESSID"));
        return NDIS_STATUS_TAPI_INVALADDRESSID;
    }

    pRequest->LineAddressStatus.ulNeededSize =
    pRequest->LineAddressStatus.ulUsedSize = sizeof(pRequest->LineAddressStatus);

    pRequest->LineAddressStatus.ulNumInUse = 1;
    pRequest->LineAddressStatus.ulNumActiveCalls = 1;
    pRequest->LineAddressStatus.ulAddressFeatures = LINEADDRFEATURE_MAKECALL;
    pRequest->LineAddressStatus.ulNumRingsNoAnswer = 999;

    DEBUGMSG(DBG_FUNC, (DTEXT("-TapiGetAddressStatus\n")));
    return NDIS_STATUS_SUCCESS;

}


NDIS_STATUS
TapiGetCallInfo(
    IN PPPTP_ADAPTER pAdapter,
    IN OUT PNDIS_TAPI_GET_CALL_INFO pRequest,
    IN OUT PULONG pRequiredLength
    )
{
    PCALL_SESSION pCall;
    LINE_CALL_INFO *pLineCallInfo = NULL;
    ULONG CallerIdLength;

    DEBUGMSG(DBG_FUNC, (DTEXT("+TapiGetCallInfo\n")));

    if ( pRequest == NULL || pAdapter == NULL )
    {
        DEBUGMSG(DBG_ERROR|DBG_FUNC, (DTEXT("-TapiGetCallInfo NDIS_STATUS_TAPI_INVALPARAM\n")));
        WPLOG(LL_A, LM_TAPI, ("-NDIS_STATUS_TAPI_INVALPARAM"));
        return NDIS_STATUS_TAPI_INVALPARAM;
    }
    
    pLineCallInfo = &pRequest->LineCallInfo;

    // Verify the ID
    pCall = CallGetCall(pAdapter, pRequest->hdCall);

    if (!pCall)
    {
        DEBUGMSG(DBG_FUNC, (DTEXT("-TapiGetCallInfo NDIS_STATUS_TAPI_INVALCALLHANDLE\n")));
        WPLOG(LL_A, LM_TAPI, ("-NDIS_STATUS_TAPI_INVALCALLHANDLE"));
        return NDIS_STATUS_TAPI_INVALCALLHANDLE;
    }

    CallerIdLength = strlen(pCall->CallerId);
    if( CallerIdLength ){
        CallerIdLength += 1; // Add one for null terminator
    }
    if( pRequiredLength ){
        // Note: This returns NDIS struct size not LineCallInfo Size
        *pRequiredLength = sizeof(NDIS_TAPI_GET_CALL_INFO) + CallerIdLength;
        DEBUGMSG(DBG_FUNC, (DTEXT("-TapiGetCallInfo NDIS_STATUS_SUCCESS - Returning Length Only Len=0x%X\n"), *pRequiredLength));
        return NDIS_STATUS_SUCCESS;
    }

    pLineCallInfo->ulNeededSize = sizeof(pRequest->LineCallInfo) + CallerIdLength;

    if( pLineCallInfo->ulTotalSize < sizeof(pRequest->LineCallInfo) ){
        pLineCallInfo->ulUsedSize = 0;
        DEBUGMSG(DBG_FUNC|DBG_ERROR, (DTEXT("-TapiGetCallInfo NDIS_STATUS_INVALID_LENGTH\n")));
        WPLOG(LL_A, LM_TAPI, ("-NDIS_STATUS_INVALID_LENGTH"));
        return NDIS_STATUS_INVALID_LENGTH;
    }

    pLineCallInfo->ulUsedSize = sizeof(pRequest->LineCallInfo);

    pLineCallInfo->hLine = (ULONG)pCall->DeviceId;
    pLineCallInfo->ulLineDeviceID = (ULONG)pCall->DeviceId;
    pLineCallInfo->ulAddressID = TAPI_ADDRESSID;

    pLineCallInfo->ulBearerMode = LINEBEARERMODE_DATA;
    pLineCallInfo->ulRate = pCall->Speed;
    pLineCallInfo->ulMediaMode = pCall->MediaModeMask;

    pLineCallInfo->ulCallParamFlags = LINECALLPARAMFLAGS_IDLE;
    pLineCallInfo->ulCallStates = CALL_STATES_MASK;

    pLineCallInfo->ulCallerIDFlags = LINECALLPARTYID_UNAVAIL;
    pLineCallInfo->ulCallerIDSize = 0;
    pLineCallInfo->ulCalledIDOffset = 0;
    pLineCallInfo->ulCalledIDFlags = LINECALLPARTYID_UNAVAIL;
    pLineCallInfo->ulCalledIDSize = 0;

    if( CallerIdLength ){
        if (pLineCallInfo->ulTotalSize >= pLineCallInfo->ulNeededSize)
        {
            PUCHAR pCallerId = (PUCHAR)(pLineCallInfo + 1);

            strcpy(pCallerId, pCall->CallerId);
            if (pCall->Inbound)
            {
                pLineCallInfo->ulCallerIDFlags = LINECALLPARTYID_ADDRESS;
                pLineCallInfo->ulCallerIDSize = CallerIdLength;
                pLineCallInfo->ulCallerIDOffset = sizeof(LINE_CALL_INFO);
            }
            else
            {
                pLineCallInfo->ulCalledIDFlags = LINECALLPARTYID_ADDRESS;
                pLineCallInfo->ulCalledIDSize = CallerIdLength;
                pLineCallInfo->ulCalledIDOffset = sizeof(LINE_CALL_INFO);
            }

            pLineCallInfo->ulUsedSize = pLineCallInfo->ulNeededSize;

        }else{
            DEBUGMSG(DBG_FUNC|DBG_WARN, (DTEXT("-TapiGetCallInfo NDIS_STATUS_SUCCESS without CallerID string\n")));
            WPLOG(LL_I, LM_TAPI, ("-NDIS_STATUS_SUCCESS without CallerID string"));
            return NDIS_STATUS_SUCCESS;
        }
    }

    DEBUGMSG(DBG_FUNC, (DTEXT("-TapiGetCallInfo\n")));
    return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
TapiGetCallStatus(
    IN PPPTP_ADAPTER pAdapter,
    IN OUT PNDIS_TAPI_GET_CALL_STATUS pRequest
    )
{
    PCALL_SESSION pCall;

    DEBUGMSG(DBG_FUNC, (DTEXT("+TapiGetCallStatus\n")));

    if ( pRequest == NULL || pAdapter == NULL )
    {
        DEBUGMSG(DBG_ERROR|DBG_FUNC, (DTEXT("-TapiGetCallStatus NDIS_STATUS_TAPI_INVALPARAM\n")));
        WPLOG(LL_A, LM_TAPI, ("-NDIS_STATUS_TAPI_INVALPARAM"));
        return NDIS_STATUS_TAPI_INVALPARAM;
    }

    // Verify the ID
    pCall = CallGetCall(pAdapter, pRequest->hdCall);

    if (!pCall)
    {
        DEBUGMSG(DBG_FUNC|DBG_ERROR, (DTEXT("-TapiGetCallStatus NDIS_STATUS_TAPI_INVALCALLHANDLE\n")));
        WPLOG(LL_A, LM_TAPI, ("-NDIS_STATUS_TAPI_INVALCALLHANDLE"));
        return NDIS_STATUS_TAPI_INVALCALLHANDLE;
    }

    pRequest->LineCallStatus.ulNeededSize =
        pRequest->LineCallStatus.ulUsedSize = sizeof(LINE_CALL_STATUS);

    pRequest->LineCallStatus.ulCallFeatures = LINECALLFEATURE_ANSWER | LINECALLFEATURE_DROP;
    pRequest->LineCallStatus.ulCallPrivilege = LINECALLPRIVILEGE_OWNER;
    pRequest->LineCallStatus.ulCallState = CallGetLineCallState(pCall->State);

    DBG_X(DBG_TAPI, pRequest->LineCallStatus.ulCallState);

    switch (pRequest->LineCallStatus.ulCallState)
    {
        case LINECALLSTATE_DIALTONE:
            pRequest->LineCallStatus.ulCallStateMode = LINEDIALTONEMODE_NORMAL;
            break;
        case LINECALLSTATE_BUSY:
            pRequest->LineCallStatus.ulCallStateMode = LINEBUSYMODE_STATION;
            break;
        case LINECALLSTATE_DISCONNECTED:
            pRequest->LineCallStatus.ulCallStateMode = LINEDISCONNECTMODE_UNKNOWN;
            break;
        default:
            break;
    }
    DEBUGMSG(DBG_FUNC, (DTEXT("-TapiGetCallStatus\n")));
    return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
TapiGetDevCaps(
    IN PPPTP_ADAPTER pAdapter,
    IN OUT PNDIS_TAPI_GET_DEV_CAPS pRequest
    )
{
    BOOLEAN ValidCall;
    NDIS_STATUS Status;

    DEBUGMSG(DBG_FUNC, (DTEXT("+TapiGetDevCaps\n")));

    if ( pRequest == NULL || pAdapter == NULL )
    {
        DEBUGMSG(DBG_ERROR|DBG_FUNC, (DTEXT("-TapiGetDevCaps NDIS_STATUS_TAPI_INVALPARAM\n")));
        WPLOG(LL_A, LM_TAPI, ("-NDIS_STATUS_TAPI_INVALPARAM"));
        return NDIS_STATUS_TAPI_INVALPARAM;
    }

    // Verify the ID
    ValidCall = CallIsValidCall(pAdapter, pRequest->ulDeviceID);

    if (!ValidCall)
    {
        DEBUGMSG(DBG_ERROR|DBG_FUNC, (DTEXT("-TapiGetDevCaps NDIS_STATUS_TAPI_NODRIVER\n")));
        WPLOG(LL_A, LM_TAPI, ("-NDIS_STATUS_TAPI_NODRIVER"));
        return NDIS_STATUS_TAPI_NODRIVER;
    }

    pRequest->LineDevCaps.ulUsedSize = sizeof(pRequest->LineDevCaps);

    pRequest->LineDevCaps.ulAddressModes = LINEADDRESSMODE_ADDRESSID |
                                           LINEADDRESSMODE_DIALABLEADDR;
    pRequest->LineDevCaps.ulNumAddresses = 1;
    pRequest->LineDevCaps.ulBearerModes  = LINEBEARERMODE_DATA;

    pRequest->LineDevCaps.ulDevCapFlags  = LINEDEVCAPFLAGS_CLOSEDROP;
    pRequest->LineDevCaps.ulMaxNumActiveCalls = pAdapter->Info.Endpoints;
    pRequest->LineDevCaps.ulAnswerMode   = LINEANSWERMODE_DROP;
    pRequest->LineDevCaps.ulRingModes    = 1;

    pRequest->LineDevCaps.ulPermanentLineID = pRequest->ulDeviceID + 1;
    pRequest->LineDevCaps.ulMaxRate      = 0;
    pRequest->LineDevCaps.ulMediaModes   = LINEMEDIAMODE_DIGITALDATA;

    Status = OsSpecificTapiGetDevCaps( pRequest->ulDeviceID, pRequest );

    DEBUGMSG(DBG_FUNC, (DTEXT("-TapiGetDevCaps\n")));
    return Status;
}

NDIS_STATUS
TapiGetExtensionId(
    IN PPPTP_ADAPTER pAdapter,
    IN OUT PNDIS_TAPI_GET_EXTENSION_ID pRequest
    )
{
    BOOLEAN ValidCall;
    DEBUGMSG(DBG_FUNC, (DTEXT("+TapiGetExtensionId\n")));

    if ( pRequest == NULL || pAdapter == NULL )
    {
        DEBUGMSG(DBG_ERROR|DBG_FUNC, (DTEXT("-TapiGetExtensionId NDIS_STATUS_TAPI_INVALPARAM\n")));
        WPLOG(LL_A, LM_TAPI, ("-NDIS_STATUS_TAPI_INVALPARAM"));
        return NDIS_STATUS_TAPI_INVALPARAM;
    }

    // Verify the ID
    ValidCall = CallIsValidCall(pAdapter, pRequest->ulDeviceID);
    if (!ValidCall)
    {
        DEBUGMSG(DBG_ERROR|DBG_FUNC, (DTEXT("-TapiGetExtensionId NDIS_STATUS_TAPI_NODRIVER\n")));
        WPLOG(LL_A, LM_TAPI, ("-NDIS_STATUS_TAPI_NODRIVER"));
        return NDIS_STATUS_TAPI_NODRIVER;
    }

    // No extensions supported.
    pRequest->LineExtensionID.ulExtensionID0 = 0;
    pRequest->LineExtensionID.ulExtensionID1 = 0;
    pRequest->LineExtensionID.ulExtensionID2 = 0;
    pRequest->LineExtensionID.ulExtensionID3 = 0;

    DEBUGMSG(DBG_FUNC, (DTEXT("-TapiGetExtensionId\n")));
    return NDIS_STATUS_SUCCESS;
}

#define PPTP_DEVICE_TYPE_STR "PPTP"

NDIS_STATUS
TapiGetId(
    IN PPPTP_ADAPTER pAdapter,
    IN OUT PNDIS_TAPI_GET_ID pRequest
    )
{
    PCALL_SESSION pCall;
    ULONG_PTR DeviceID;
    BOOLEAN IsNdisClass;

    DEBUGMSG(DBG_FUNC, (DTEXT("+TapiGetId\n")));

    if ( pRequest == NULL || pAdapter == NULL )
    {
        DEBUGMSG(DBG_ERROR|DBG_FUNC, (DTEXT("-TapiGetId NDIS_STATUS_TAPI_INVALPARAM\n")));
        WPLOG(LL_A, LM_TAPI, ("-NDIS_STATUS_TAPI_INVALPARAM"));
        return NDIS_STATUS_TAPI_INVALPARAM;
    }

    // Make sure this is a tapi or ndis request.
    if (pRequest->ulDeviceClassSize == sizeof(NDIS_DEVICECLASS_NAME) &&
        _strnicmp((PCHAR) pRequest + pRequest->ulDeviceClassOffset,
                  NDIS_DEVICECLASS_NAME, pRequest->ulDeviceClassSize) == 0)
    {
        IsNdisClass = TRUE;
    }
    else if (pRequest->ulDeviceClassSize == sizeof(TAPI_DEVICECLASS_NAME) && 
        _strnicmp((PCHAR) pRequest + pRequest->ulDeviceClassOffset,
                  TAPI_DEVICECLASS_NAME, pRequest->ulDeviceClassSize) == 0)
    {
        IsNdisClass = FALSE;
    }
    else
    {
        DEBUGMSG(DBG_ERROR|DBG_FUNC, (DTEXT("-TapiGetId NDIS_STATUS_TAPI_INVALDEVICECLASS\n")));
        WPLOG(LL_A, LM_TAPI, ("-NDIS_STATUS_TAPI_INVALDEVICECLASS"));
        return NDIS_STATUS_TAPI_INVALDEVICECLASS;
    }
    

    DBG_D(DBG_TAPI, pRequest->ulSelect);

#if DBG    

    if(pRequest->ulDeviceClassSize != 0)
    {
        DBG_S(DBG_TAPI, (PCHAR) pRequest + pRequest->ulDeviceClassOffset);
    }
    
#endif    

    switch (pRequest->ulSelect) {
        case LINECALLSELECT_LINE:
            if (!CallIsValidCall(pAdapter, TapiLineHandleToId(pRequest->hdLine)))
            {
                DEBUGMSG(DBG_ERROR|DBG_FUNC, (DTEXT("-TapiGetId NDIS_STATUS_TAPI_INVALLINEHANDLE\n")));
                WPLOG(LL_A, LM_TAPI, ("-NDIS_STATUS_TAPI_INVALLINEHANDLE"));
                return NDIS_STATUS_TAPI_INVALLINEHANDLE;
            }
            DeviceID = 0;

            break;

        case LINECALLSELECT_ADDRESS:
            if (!CallIsValidCall(pAdapter, TapiLineHandleToId(pRequest->hdLine)))
            {
                DEBUGMSG(DBG_ERROR|DBG_FUNC, (DTEXT("-TapiGetId NDIS_STATUS_TAPI_INVALLINEHANDLE\n")));
                WPLOG(LL_A, LM_TAPI, ("-NDIS_STATUS_TAPI_INVALLINEHANDLE"));
                return NDIS_STATUS_TAPI_INVALLINEHANDLE;
            }
            if (pRequest->ulAddressID >= TAPI_ADDR_PER_LINE)
            {
                DEBUGMSG(DBG_ERROR|DBG_FUNC, (DTEXT("-TapiGetId NDIS_STATUS_TAPI_INVALADDRESSID\n")));
                WPLOG(LL_A, LM_TAPI, ("-NDIS_STATUS_TAPI_INVALADDRESSID"));
                return NDIS_STATUS_TAPI_INVALADDRESSID;
            }
            DeviceID = 0;
            break;

        case LINECALLSELECT_CALL:
            pCall = CallGetCall(pAdapter, pRequest->hdCall);

            if (pCall == NULL)
            {
                DEBUGMSG(DBG_ERROR|DBG_FUNC, (DTEXT("-TapiGetId NDIS_STATUS_TAPI_INVALLINEHANDLE\n")));
                WPLOG(LL_A, LM_TAPI, ("-NDIS_STATUS_TAPI_INVALLINEHANDLE"));
                return NDIS_STATUS_TAPI_INVALLINEHANDLE;
            }
            TapiLineUp(pCall);

            DeviceID = (ULONG_PTR)pCall->NdisLinkContext;

            break;

        default:
            DEBUGMSG(DBG_ERROR|DBG_FUNC, (DTEXT("-TapiGetId NDIS_STATUS_FAILURE ulSelect=%d\n"), pRequest->ulSelect));
            WPLOG(LL_A, LM_TAPI, ("-NDIS_STATUS_FAILURE ulSelect=%d", pRequest->ulSelect));
            return (NDIS_STATUS_FAILURE);

    }

    // Default to the minimum amount of data we will return.
    pRequest->DeviceID.ulUsedSize = sizeof(VAR_STRING);

    if( IsNdisClass ){
        // The format of the DeviceID for the "ndis" class is:
        struct _NDIS_CLASS {
            ULONG_PTR   hDevice;
            CHAR        szDeviceType[1];
        } *pNDISClass;

        pRequest->DeviceID.ulNeededSize =sizeof(VAR_STRING) + sizeof(PVOID) + 
            sizeof(DeviceID) + sizeof(PPTP_DEVICE_TYPE_STR);

        if (pRequest->DeviceID.ulTotalSize >= pRequest->DeviceID.ulNeededSize)
        {
            pRequest->DeviceID.ulUsedSize = pRequest->DeviceID.ulNeededSize;
            pRequest->DeviceID.ulStringFormat = STRINGFORMAT_BINARY;
            pRequest->DeviceID.ulStringSize   = sizeof(DeviceID) + sizeof(PPTP_DEVICE_TYPE_STR);

            pNDISClass = (struct _NDIS_CLASS *)
                ((PUCHAR)((&pRequest->DeviceID) + 1) + sizeof(PVOID));

            (ULONG_PTR)pNDISClass &=
                ~((ULONG_PTR)sizeof(PVOID) - 1);

            pRequest->DeviceID.ulStringOffset = (ULONG)
                ((PUCHAR)pNDISClass - (PUCHAR)(&pRequest->DeviceID));

            pNDISClass->hDevice = DeviceID;

            DBG_X(DBG_TAPI, pNDISClass->hDevice);
            NdisMoveMemory(pNDISClass->szDeviceType, PPTP_DEVICE_TYPE_STR, sizeof(PPTP_DEVICE_TYPE_STR));
        }

    }else{
        // Now we need to adjust the variable field to place the device ID.

        pRequest->DeviceID.ulNeededSize = sizeof(VAR_STRING) + sizeof(DeviceID);
        if (pRequest->DeviceID.ulTotalSize >= pRequest->DeviceID.ulNeededSize)
        {
            pRequest->DeviceID.ulUsedSize = pRequest->DeviceID.ulNeededSize;
            pRequest->DeviceID.ulStringFormat = STRINGFORMAT_BINARY;
            pRequest->DeviceID.ulStringSize   = sizeof(DeviceID);
            pRequest->DeviceID.ulStringOffset = sizeof(VAR_STRING);

            *(PULONG_PTR)((&pRequest->DeviceID) + 1) = DeviceID;
        }
    }

    //DEBUGMEM(DBG_TAPI, &pRequest->DeviceID, pRequest->DeviceID.ulUsedSize, 1);

    DEBUGMSG(DBG_FUNC, (DTEXT("-TapiGetId\n")));
    return NDIS_STATUS_SUCCESS;
}

VOID
TapiLineDown(
    PCALL_SESSION pCall
    )
{
    NDIS_MAC_LINE_DOWN LineDownInfo;
    DEBUGMSG(DBG_FUNC|DBG_CALL|DBG_TAPI, (DTEXT("+TapiLineDown %08x\n"), pCall->NdisLinkContext));

    if (pCall->NdisLinkContext)
    {
        LineDownInfo.NdisLinkContext = pCall->NdisLinkContext;

        WPLOG(LL_M, LM_TAPI, ("LINE_DOWN %!IPADDR! pCall %p, Cid %d\n",
            pCall->Remote.Address.Address[0].Address[0].in_addr,
            pCall, (ULONG)pCall->DeviceId));
        /*
        * Indicate the event to the WAN wrapper.
        */
        NdisMIndicateStatus(pCall->pAdapter->hMiniportAdapter,
                            NDIS_STATUS_WAN_LINE_DOWN,
                            &LineDownInfo,
                            sizeof(LineDownInfo));
        pCall->NdisLinkContext = NULL;
    }
    else
    {
        WPLOG(LL_A, LM_TAPI, ("LINE_DOWN %!IPADDR! pCall %p, Cid %d - No NdisLinkContext",
            pCall->Remote.Address.Address[0].Address[0].in_addr,
            pCall, (ULONG)pCall->DeviceId));
    }
    DEBUGMSG(DBG_FUNC, (DTEXT("-TapiLineDown\n")));
}

VOID
TapiLineUp(
    PCALL_SESSION pCall
    )
{
    NDIS_MAC_LINE_UP LineUpInfo;

    DEBUGMSG(DBG_FUNC|DBG_CALL|DBG_TAPI, (DTEXT("+TapiLineUp %08x\n"), pCall));

    NdisAcquireSpinLock(&pCall->Lock);
    pCall->Close.Checklist &= ~CALL_CLOSE_LINE_DOWN;
    NdisReleaseSpinLock(&pCall->Lock);
    /*
    * Initialize the LINE_UP event packet.
    */
    LineUpInfo.LinkSpeed = pCall->Speed / 100;
    LineUpInfo.Quality             = NdisWanErrorControl;
    LineUpInfo.SendWindow          = 0;

    // Jeff says Win98 needs the DeviceID as the connection wrapper ID, but
    // TonyBe says NT needs an absolutely unique ID, which hTapiCall is.
    // hTapiCall is probably more correct according to Tapi rules, but we make
    // allowances.

    LineUpInfo.ConnectionWrapperID = OS_CONNECTION_WRAPPER_ID;
    LineUpInfo.NdisLinkHandle      = (NDIS_HANDLE) DeviceIdToLinkHandle(pCall->FullDeviceId);
    LineUpInfo.NdisLinkContext     = pCall->NdisLinkContext;

    WPLOG(LL_M, LM_TAPI, ("LINE_UP %!IPADDR! pCall %p, Cid %d",
        pCall->Remote.Address.Address[0].Address[0].in_addr,
        pCall, (ULONG)pCall->DeviceId));
    /*
    * Indicate the event to the WAN wrapper.
    */
    NdisMIndicateStatus(pCall->pAdapter->hMiniportAdapter,
                        NDIS_STATUS_WAN_LINE_UP,
                        &LineUpInfo,
                        sizeof(LineUpInfo));

    NdisAcquireSpinLock(&pCall->Lock);
    pCall->NdisLinkContext = LineUpInfo.NdisLinkContext;
    NdisReleaseSpinLock(&pCall->Lock);
    DBG_X(DBG_TAPI, LineUpInfo.NdisLinkContext);

    DEBUGMSG(DBG_FUNC, (DTEXT("-TapiLineUp\n")));
}

typedef struct {
    TA_IP_ADDRESS   TargetAddress[MAX_TARGET_ADDRESSES];
    ULONG           NumAddresses;
    ULONG           CurrentAddress;
} MAKE_CALL_CONTEXT, *PMAKE_CALL_CONTEXT;

VOID
TapipParseIpAddressesAndCallerId(
    IN      PUCHAR  pAddressList,
    IN      ULONG   ListLength,
    IN      PCALL_SESSION pCall,
    IN OUT  PTA_IP_ADDRESS TargetAddress,
    IN OUT  PULONG  pulCountAddresses
    )
{
    BOOLEAN ValidAddress = FALSE;
    ULONG NumTargetAddresses = 0;
    PUCHAR pEndAddressList;
    ULONG MaxTargetAddresses = *pulCountAddresses;

    DEBUGMSG(DBG_FUNC, (DTEXT("+TapipParseAddresses\n")));

    // TAPI gives us an address of the following forms:
    //
    // [ IP Address ... IP Address ] [ Phone Number ]
    //
    // It begins with a list of addresses to try, and an optional phone number
    // if the remote device is an access concentrator.

    NdisZeroMemory(TargetAddress, sizeof(TA_IP_ADDRESS) * MaxTargetAddresses);
    *pulCountAddresses = 0;

    pEndAddressList = pAddressList + ListLength;
    
    if(ListLength < 2)
    {
        return;         // invalid length
    }
    
    // Make sure it's null terminated
    if(pAddressList[ListLength - 1] != '\0')
    {
        pAddressList[ListLength - 1] = '\0';
    }

    ValidAddress = TRUE;
    while (ValidAddress && pAddressList<pEndAddressList)
    {
        pAddressList = StringToIpAddress(pAddressList,
                                     &TargetAddress[NumTargetAddresses],
                                     &ValidAddress);

        if (ValidAddress)
        {
            if (++NumTargetAddresses==MaxTargetAddresses)
            {
                // We've hit the limit of what we'll take.  Throw the rest.
                while (ValidAddress && pAddressList<pEndAddressList)
                {
                    TA_IP_ADDRESS ThrowAway;
                    pAddressList = StringToIpAddress(pAddressList,
                                                     &ThrowAway,
                                                     &ValidAddress);
                }
            }
        }
    }
    
    // We've got the addresses.  Now nab the phone #
    {
        ULONG Digits = 0;
        while (pAddressList<pEndAddressList && *pAddressList && (*pAddressList==' ' || *pAddressList=='\t'))
        {
            pAddressList++;
        }
        while (pAddressList<pEndAddressList && *pAddressList &&
               Digits<MAX_PHONE_NUMBER_LENGTH-1)
        {
            pCall->CallerId[Digits++] = *pAddressList++;
        }
        pCall->CallerId[Digits] = '\0';
#if 0
        // This apparently breaks Alcatel
        if (!Digits)
        {
            IpAddressToString(htonl(pCall->Remote.Address.Address[0].Address[0].in_addr), pCall->CallerId);
        }
#endif
        DEBUGMSG(DBG_CALL, (DTEXT("Dialing %hs\n"), pCall->CallerId));
    }
    
    *pulCountAddresses = NumTargetAddresses;

    DEBUGMSG(DBG_FUNC, (DTEXT("-TapipParseAddresses\n")));
}

VOID
TapipMakeCallCallback(
    PPPTP_WORK_ITEM pWorkItem
    )
{
    PCALL_SESSION pCall;
    PPPTP_ADAPTER pAdapter = pWorkItem->Context;
    PNDIS_TAPI_MAKE_CALL pRequest = pWorkItem->pBuffer;
    NDIS_STATUS Status;

    DEBUGMSG(DBG_FUNC, (DTEXT("+TapipMakeCallCallback\n")));

    pCall = CallGetCall(pAdapter, pRequest->hdCall);

    if (!pCall)
    {
        DEBUGMSG(DBG_ERROR|DBG_FUNC, (DTEXT("-TapipMakeCallCallback NDIS_STATUS_TAPI_INUSE\n")));
        WPLOG(LL_A, LM_TAPI, ("-NDIS_STATUS_TAPI_INUSE"));
        NdisMQueryInformationComplete(pAdapter->hMiniportAdapter, NDIS_STATUS_TAPI_INUSE);
        return;
    }

    NdisAcquireSpinLock(&pCall->Lock);

    CallSetState(pCall, STATE_CALL_DIALING, 0, LOCKED);
    NdisMSetTimer(&pCall->DialTimer, 60*1000);
   
    pCall->Close.Checklist &= ~(CALL_CLOSE_DROP|CALL_CLOSE_DROP_COMPLETE|CALL_CLOSE_CLOSE_CALL);
    pCall->Close.Checklist &= ~(CALL_CLOSE_DISCONNECT|CALL_CLOSE_CLEANUP_STATE);

    NdisReleaseSpinLock(&pCall->Lock);

    Status = CtlConnectCall(pAdapter, pCall, &pCall->Remote.Address);
    NdisAcquireSpinLock(&pCall->Lock);
    if (Status==NDIS_STATUS_PENDING)
    {
        Status = NDIS_STATUS_SUCCESS;
    }
    else if (Status!=NDIS_STATUS_SUCCESS)
    {
        // We weren't successful, so we won't be getting these calls from tapi
        pCall->Close.Checklist |= (CALL_CLOSE_DROP|CALL_CLOSE_DROP_COMPLETE|CALL_CLOSE_CLOSE_CALL);
    }

    pCall->Remote.Address.Address[0].Address[0].sin_port = htons(PptpProtocolNumber);
    NdisReleaseSpinLock(&pCall->Lock);
    NdisMQueryInformationComplete(pAdapter->hMiniportAdapter, Status);

    DEBUGMSG(DBG_FUNC, (DTEXT("-TapipMakeCallCallback\n")));
}

NDIS_STATUS
TapiMakeCall(
    IN PPPTP_ADAPTER pAdapter,
    IN OUT PNDIS_TAPI_MAKE_CALL pRequest
    )
{
    PCALL_SESSION pCall;
    NDIS_STATUS Status;
    
    ULONG NumAddresses = 1;
    PUCHAR pAddressList = (PUCHAR)pRequest + pRequest->ulDestAddressOffset;

    DEBUGMSG(DBG_FUNC, (DTEXT("+TapiMakeCall\n")));

    if ( pRequest == NULL || pAdapter == NULL )
    {
        DEBUGMSG(DBG_ERROR|DBG_FUNC, (DTEXT("-TapiMakeCall NDIS_STATUS_TAPI_INVALPARAM\n")));
        WPLOG(LL_A, LM_TAPI, ("-NDIS_STATUS_TAPI_INVALPARAM"));
        return NDIS_STATUS_TAPI_INVALPARAM;
    }
    
    if(!pAdapter->Tapi.Open)
    {
        DEBUGMSG(DBG_ERROR|DBG_FUNC, (DTEXT("-TapiMakeCall NDIS_STATUS_TAPI_NODEVICE\n")));
        WPLOG(LL_A, LM_TAPI, ("-NDIS_STATUS_TAPI_NODEVICE"));
        return NDIS_STATUS_TAPI_NODEVICE;
    }

    if (!CallIsValidCall(pAdapter, TapiLineHandleToId(pRequest->hdLine)))
    {
        DEBUGMSG(DBG_ERROR|DBG_FUNC, (DTEXT("-TapiMakeCall NDIS_STATUS_TAPI_INVALLINEHANDLE\n")));
        WPLOG(LL_A, LM_TAPI, ("-NDIS_STATUS_TAPI_INVALLINEHANDLE"));
        return NDIS_STATUS_TAPI_INVALLINEHANDLE;
    }

    pCall = CallFindAndLock(pAdapter, STATE_CALL_IDLE, FIND_OUTGOING);

    if (!pCall)
    {
        DEBUGMSG(DBG_ERROR, (DTEXT("-TapiMakeCall NDIS_STATUS_TAPI_INUSE\n")));
        WPLOG(LL_A, LM_TAPI, ("-NDIS_STATUS_TAPI_INUSE"));
        return NDIS_STATUS_TAPI_INUSE;
    }

    if (pCall->State != STATE_CALL_IDLE)
    {
        NdisReleaseSpinLock(&pCall->Lock);
        DEBUGMSG(DBG_ERROR, (DTEXT("-TapiMakeCall NDIS_STATUS_TAPI_INUSE\n")));
        WPLOG(LL_A, LM_TAPI, ("-NDIS_STATUS_TAPI_INUSE"));
        return NDIS_STATUS_TAPI_INUSE;
    }

    // Save the handle tapi provides
    ASSERT(!pCall->hTapiCall);
    pCall->hTapiCall = pRequest->htCall;
    pCall->Inbound = FALSE;

    // our handle for tapi to this call is the device id
    pRequest->hdCall = pCall->FullDeviceId;
    
    WPLOG(LL_M, LM_TAPI, ("MAKECALL pCall %p, Cid %d, hTapiCall %Ix, hdCall %d",
        pCall, (ULONG)pCall->DeviceId, pCall->hTapiCall, (ULONG)pRequest->hdCall));    

    CallSetState(pCall, STATE_CALL_OFFHOOK, 0, LOCKED);
    pCall->PendingUse = FALSE;

    NdisReleaseSpinLock(&pCall->Lock);

    // We currently support one IP address only
    TapipParseIpAddressesAndCallerId(pAddressList,
                          pRequest->ulDestAddressSize,
                          pCall,
                          &pCall->Remote.Address,
                          &NumAddresses);
                          
    if(NumAddresses != 1)
    {
        DEBUGMSG(DBG_ERROR|DBG_FUNC, (DTEXT("-TapipMakeCall NDIS_STATUS_TAPI_INVALCALLPARAMS\n")));
        WPLOG(LL_A, LM_TAPI, ("-NDIS_STATUS_TAPI_INVALCALLPARAMS"));
        Status = NDIS_STATUS_TAPI_INVALCALLPARAMS;
    }
    else
    {
        // We got a valid IP address.
        // Schedule a work item so we can issue a CtlCreateCall at passive level.
        Status = ScheduleWorkItem(TapipMakeCallCallback,
                                  pAdapter,
                                  pRequest,
                                  0);
    }
    
    if (Status==NDIS_STATUS_SUCCESS)
    {
        // Keep the pRequest around until the callback.
        Status = NDIS_STATUS_PENDING;
    }
    else
    {
        CallSetState(pCall, STATE_CALL_CLEANUP, LINEDISCONNECTMODE_UNKNOWN, UNLOCKED);
        CallCleanup(pCall, UNLOCKED);
    }

    DEBUGMSG(DBG_FUNC, (DTEXT("-TapiMakeCall %08x\n"), Status));
    return Status;
}

NDIS_STATUS
TapiNegotiateExtVersion(
    IN PPPTP_ADAPTER pAdapter,
    IN OUT PNDIS_TAPI_NEGOTIATE_EXT_VERSION pRequest
    )
{
    DEBUGMSG(DBG_FUNC, (DTEXT("+TapiNegotiateExtVersion Low=%08x Hi=%08x\n"),
                        pRequest ? pRequest->ulLowVersion : 0, pRequest ? pRequest->ulHighVersion : 0));

    if ( pRequest == NULL || pAdapter == NULL )
    {
        DEBUGMSG(DBG_ERROR|DBG_FUNC, (DTEXT("-TapiNegotiateExtVersion NDIS_STATUS_TAPI_INVALPARAM\n")));
        WPLOG(LL_A, LM_TAPI, ("-TapiNegotiateExtVersion NDIS_STATUS_TAPI_INVALPARAM"));
        return NDIS_STATUS_TAPI_INVALPARAM;
    }

    if (pRequest->ulLowVersion < TAPI_EXT_VERSION ||
        pRequest->ulHighVersion > TAPI_EXT_VERSION)
    {
        DEBUGMSG(DBG_ERROR|DBG_FUNC, (DTEXT("-TapiNegotiateExtVersion NDIS_STATUS_TAPI_INCOMPATIBLEEXTVERSION\n")));
        WPLOG(LL_A, LM_TAPI, ("-TapiNegotiateExtVersion NDIS_STATUS_TAPI_INCOMPATIBLEEXTVERSION"));
        return NDIS_STATUS_TAPI_INCOMPATIBLEEXTVERSION;
    }

    DEBUGMSG(DBG_FUNC, (DTEXT("-TapiNegotiateExtVersion\n")));
    return NDIS_STATUS_SUCCESS;
}

VOID
TapipPassiveOpen(
    PPPTP_WORK_ITEM pWorkItem
    )
{
    PPPTP_ADAPTER pAdapter = pWorkItem->Context;
    DEBUGMSG(DBG_FUNC, (DTEXT("+TapipPassiveOpen\n")));

    // We only come here if PptpInitialized = FALSE;
    // It's a mechanism to defer opening tcp handles until tcp is
    // actually available, as it may not always be during startup/setup
    PptpInitialize(pAdapter);
    
    NdisMQueryInformationComplete(pAdapter->hMiniportAdapter, NDIS_STATUS_SUCCESS);
    DEBUGMSG(DBG_FUNC, (DTEXT("-TapipPassiveOpen\n")));
}

NDIS_STATUS
TapiOpen(
    IN PPPTP_ADAPTER pAdapter,
    IN OUT PNDIS_TAPI_OPEN pRequest
    )
{
    PCALL_SESSION pCall;
    DEBUGMSG(DBG_FUNC, (DTEXT("+TapiOpen\n")));

    if ( pRequest == NULL || pAdapter == NULL )
    {
        DEBUGMSG(DBG_ERROR|DBG_FUNC, (DTEXT("-TapiOpen NDIS_STATUS_TAPI_INVALPARAM\n")));
        WPLOG(LL_A, LM_TAPI, ("-NDIS_STATUS_TAPI_INVALPARAM"));
        return NDIS_STATUS_TAPI_INVALPARAM;
    }

    if (!CallIsValidCall(pAdapter, pRequest->ulDeviceID))
    {
        DEBUGMSG(DBG_ERROR|DBG_FUNC, (DTEXT("-TapiOpen NDIS_STATUS_TAPI_NODRIVER\n")));
        WPLOG(LL_A, LM_TAPI, ("-NDIS_STATUS_TAPI_NODRIVER"));
        return NDIS_STATUS_TAPI_NODRIVER;
    }

    NdisAcquireSpinLock(&pAdapter->Lock);

    if (pAdapter->Tapi.Open)
    {
        NdisReleaseSpinLock(&pAdapter->Lock);
        return NDIS_STATUS_TAPI_ALLOCATED;
    }

    pAdapter->Tapi.Open = TRUE;
    pAdapter->Tapi.hTapiLine = pRequest->htLine;

    pRequest->hdLine = TapiIdToLineHandle(pRequest->ulDeviceID);

    NdisReleaseSpinLock(&pAdapter->Lock);

    if (!PptpInitialized)
    {
        if (ScheduleWorkItem(TapipPassiveOpen, pAdapter, NULL, 0)==NDIS_STATUS_SUCCESS)
        {
            DEBUGMSG(DBG_FUNC, (DTEXT("-TapiOpen PENDING\n")));
            return NDIS_STATUS_PENDING;
        }
    }

    DEBUGMSG(DBG_FUNC, (DTEXT("-TapiOpen\n")));
    return NDIS_STATUS_SUCCESS;
}

VOID
TapipPassiveProviderInitialize(
    PPPTP_WORK_ITEM pWorkItem
    )
{
    PPPTP_ADAPTER pAdapter = pWorkItem->Context;

    DEBUGMSG(DBG_FUNC, (DTEXT("+TapipPassiveProviderInitialize\n")));

    // We initialize this here because TCP may not be available in MiniportInitialize
    // We don't uninit until MiniportHalt.  CtdiInitialize will just return quietly
    // if it's already initialized, so we can come through here several times
    // safely.

    PptpInitialize(pAdapter); // Ignore the return status.

    NdisMQueryInformationComplete(pAdapter->hMiniportAdapter, NDIS_STATUS_SUCCESS);

    DEBUGMSG(DBG_FUNC, (DTEXT("-TapipPassiveProviderInitialize\n")));
}

NDIS_STATUS
TapiProviderInitialize(
    IN PPPTP_ADAPTER pAdapter,
    IN OUT PNDIS_TAPI_PROVIDER_INITIALIZE pRequest
    )
{
    NDIS_STATUS Status = NDIS_STATUS_SUCCESS;

    DEBUGMSG(DBG_FUNC, (DTEXT("+TapiProviderInitialize\n")));

    if ( pRequest == NULL || pAdapter == NULL )
    {
        DEBUGMSG(DBG_ERROR|DBG_FUNC, (DTEXT("-TapiProviderInitialize NDIS_STATUS_TAPI_INVALPARAM\n")));
        WPLOG(LL_A, LM_TAPI, ("-NDIS_STATUS_TAPI_INVALPARAM"));
        return NDIS_STATUS_TAPI_INVALPARAM;
    }

    pAdapter->Tapi.DeviceIdBase = pRequest->ulDeviceIDBase;
    pRequest->ulNumLineDevs = 1;
    pRequest->ulProviderID = (ULONG_PTR)pAdapter;
    
    if (!PptpInitialized)
    {
        Status = ScheduleWorkItem(TapipPassiveProviderInitialize, pAdapter, NULL, 0);
        if (Status==NDIS_STATUS_SUCCESS)
        {
            // Keep the pRequest around until the callback.
            Status = NDIS_STATUS_PENDING;
        }
    }

    DEBUGMSG(DBG_FUNC, (DTEXT("-TapiProviderInitialize %08x\n"), Status));
    return Status;
}

VOID
TapipPassiveProviderShutdown(
    PPPTP_WORK_ITEM pWorkItem
    )
{
    PPPTP_ADAPTER pAdapter = pWorkItem->Context;
    ULONG i;
    DEBUGMSG(DBG_FUNC|DBG_TAPI, (DTEXT("+TapipPassiveProviderShutdown\n")));

    NdisAcquireSpinLock(&pAdapter->Lock);
    pAdapter->Tapi.Open = FALSE;
    NdisReleaseSpinLock(&pAdapter->Lock);

    for (i=0; i<pAdapter->Info.Endpoints; i++)
    {
//        PCALL_SESSION pCall = CallGetCall(pAdapter, i);
        PCALL_SESSION pCall = pAdapter->pCallArray[i];

        if (IS_CALL(pCall))
        {
            NdisAcquireSpinLock(&pCall->Lock);
            if (pCall->State>STATE_CALL_IDLE && pCall->State<=STATE_CALL_CLEANUP)
            {
                pCall->Open = FALSE;
                CallSetState(pCall, STATE_CALL_CLEANUP, LINEDISCONNECTMODE_NORMAL, LOCKED);
                // CALL_SESSION will be reused
                // CallDetachFromAdapter(pCall);
                pCall->Close.Checklist |= (CALL_CLOSE_DROP|CALL_CLOSE_DROP_COMPLETE|CALL_CLOSE_CLOSE_CALL);
                CallCleanup(pCall, LOCKED);
            }
            NdisReleaseSpinLock(&pCall->Lock);

        }
    }

    if (pAdapter->hCtdiDg)
    {
        CtdiClose(pAdapter->hCtdiDg);
        pAdapter->hCtdiDg = NULL;
    }

    if (pAdapter->hCtdiListen)
    {
        CtdiClose(pAdapter->hCtdiListen);
        pAdapter->hCtdiListen = NULL;
    }
    
    PptpInitialized = FALSE;

    NdisMSetInformationComplete(pAdapter->hMiniportAdapter, NDIS_STATUS_SUCCESS);

    DEBUGMSG(DBG_FUNC, (DTEXT("-TapipPassiveProviderShutdown\n")));
}


NDIS_STATUS
TapiProviderShutdown(
    IN PPPTP_ADAPTER pAdapter,
    IN OUT PNDIS_TAPI_PROVIDER_SHUTDOWN pRequest
    )
{
    NDIS_STATUS Status;

    DEBUGMSG(DBG_FUNC, (DTEXT("+TapiProviderShutdown\n")));

    if ( pRequest == NULL || pAdapter == NULL )
    {
        DEBUGMSG(DBG_ERROR|DBG_FUNC, (DTEXT("-TapiProviderShutdown NDIS_STATUS_TAPI_INVALPARAM\n")));
        WPLOG(LL_A, LM_TAPI, ("-NDIS_STATUS_TAPI_INVALPARAM"));
        return NDIS_STATUS_TAPI_INVALPARAM;
    }
    
    Status = ScheduleWorkItem(TapipPassiveProviderShutdown, pAdapter, NULL, 0);
    if (Status==NDIS_STATUS_SUCCESS)
    {
        Status = NDIS_STATUS_PENDING;
    }
    DEBUGMSG(DBG_FUNC|DBG_ERR(Status), (DTEXT("-TapiProviderShutdown %08x\n"), Status));
    return Status;
}

VOID
TapipSetDefaultMediaDetectionCallback(
    PPPTP_WORK_ITEM pWorkItem
    )
{
    PPPTP_ADAPTER pAdapter = pWorkItem->Context;
    PNDIS_TAPI_SET_DEFAULT_MEDIA_DETECTION pRequest = pWorkItem->pBuffer;
    NDIS_STATUS Status = NDIS_STATUS_SUCCESS;

    DEBUGMSG(DBG_FUNC, (DTEXT("+TapipSetDefaultMediaDetectionCallback\n")));

    if (pRequest->ulMediaModes&LINEMEDIAMODE_DIGITALDATA)
    {
        Status = CtlListen(pAdapter);
    }
    else if (pAdapter->hCtdiListen)
    {
        CtdiClose(pAdapter->hCtdiListen);
        pAdapter->hCtdiListen = NULL;
    }

    NdisMSetInformationComplete(pAdapter->hMiniportAdapter, Status);

    DEBUGMSG(DBG_FUNC, (DTEXT("-TapipSetDefaultMediaDetectionCallback\n")));
}

NDIS_STATUS
TapiSetDefaultMediaDetection(
    IN PPPTP_ADAPTER pAdapter,
    IN OUT PNDIS_TAPI_SET_DEFAULT_MEDIA_DETECTION pRequest
    )
{
    NDIS_STATUS Status = NDIS_STATUS_SUCCESS;
    DEBUGMSG(DBG_FUNC, (DTEXT("+TapiSetDefaultMediaDetection\n")));

    if ( pRequest == NULL || pAdapter == NULL )
    {
        DEBUGMSG(DBG_ERROR|DBG_FUNC, (DTEXT("-TapiSetDefaultMediaDetection NDIS_STATUS_TAPI_INVALPARAM\n")));
        WPLOG(LL_A, LM_TAPI, ("-TapiSetDefaultMediaDetection NDIS_STATUS_TAPI_INVALPARAM"));
        return NDIS_STATUS_TAPI_INVALPARAM;
    }

    // Verify the ID
    if (!CallIsValidCall(pAdapter, TapiLineHandleToId(pRequest->hdLine)))
    {
        DEBUGMSG(DBG_ERROR|DBG_FUNC, (DTEXT("-TapiSetDefaultMediaDetection NDIS_STATUS_TAPI_INVALLINEHANDLE\n")));
        WPLOG(LL_A, LM_TAPI, ("-TapiSetDefaultMediaDetection NDIS_STATUS_TAPI_INVALLINEHANDLE"));
        return NDIS_STATUS_TAPI_INVALLINEHANDLE;
    }

    // This is the OID to make us start accepting calls.

    DEBUGMSG(DBG_TAPI, (DTEXT("MediaModes: %x\n"), pRequest->ulMediaModes));

    // Schedule a work item so we can issue a CtlListen at passive level.
    Status = ScheduleWorkItem(TapipSetDefaultMediaDetectionCallback,
                              pAdapter,
                              pRequest,
                              0);
    if (Status==NDIS_STATUS_SUCCESS)
    {
        Status = NDIS_STATUS_PENDING;
    }

    DEBUGMSG(DBG_FUNC, (DTEXT("-TapiSetDefaultMediaDetection %08x\n"), Status));
    return NDIS_STATUS_PENDING;
}

NDIS_STATUS
TapiSetStatusMessages(
    IN PPPTP_ADAPTER pAdapter,
    IN OUT PNDIS_TAPI_SET_STATUS_MESSAGES pRequest
    )
{
    DEBUGMSG(DBG_FUNC, (DTEXT("+TapiSetStatusMessages\n")));

    if ( pRequest == NULL || pAdapter == NULL )
    {
        DEBUGMSG(DBG_ERROR|DBG_FUNC, (DTEXT("-TapiSetStatusMessages NDIS_STATUS_TAPI_INVALPARAM\n")));
        WPLOG(LL_A, LM_TAPI, ("-NDIS_STATUS_TAPI_INVALPARAM"));
        return NDIS_STATUS_TAPI_INVALPARAM;
    }

    if (!CallIsValidCall(pAdapter, TapiLineHandleToId(pRequest->hdLine)))
    {
        DEBUGMSG(DBG_ERROR|DBG_FUNC, (DTEXT("-TapiSetDefaultMediaDetection NDIS_STATUS_TAPI_INVALLINEHANDLE\n")));
        WPLOG(LL_A, LM_TAPI, ("-NDIS_STATUS_TAPI_INVALLINEHANDLE"));
        return NDIS_STATUS_TAPI_INVALLINEHANDLE;
    }

    pAdapter->Tapi.LineStateMask = pRequest->ulLineStates;

    DEBUGMSG(DBG_FUNC, (DTEXT("-TapiSetStatusMessages\n")));
    return NDIS_STATUS_SUCCESS;
}


