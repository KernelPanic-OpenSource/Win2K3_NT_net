/*++

Copyright (c) 1997-2001  Microsoft Corporation

Module Name:

    offload.c

Abstract:

    This module contains the code that handles offload.

Author:

    ChunYe

Environment:

    Kernel mode

Revision History:

--*/


#include    "precomp.h"

#ifdef RUN_WPP
#include "offload.tmh"
#endif


VOID
IPSecFillHwAddSA(
    IN  PSA_TABLE_ENTRY pSA,
    IN  PUCHAR          Buf,
    IN  ULONG           Len
    )
/*++

Routine Description:

    Fills in the ADD_SA hw request from pSA

Arguments:

    pSA - the SA
    Buf - buffer to set info
    Len - length

Return Value:

    status of the operation

--*/
{
    POFFLOAD_IPSEC_ADD_SA           pAddSA = (POFFLOAD_IPSEC_ADD_SA)Buf;
    POFFLOAD_SECURITY_ASSOCIATION   pSAInfo;
    LONG    Index;
    ULONG   Offset = 0;

    pAddSA->NumSAs = (SHORT)pSA->sa_NumOps;
    pAddSA->SrcAddr = pSA->SA_SRC_ADDR;
    pAddSA->SrcMask = pSA->SA_SRC_MASK;
    pAddSA->DestAddr = pSA->SA_DEST_ADDR;
    pAddSA->DestMask = pSA->SA_DEST_MASK;
    pAddSA->Protocol = pSA->SA_PROTO;
    pAddSA->SrcPort = SA_SRC_PORT(pSA);
    pAddSA->DestPort = SA_DEST_PORT(pSA);

    if (pSA->sa_Flags & FLAGS_SA_OUTBOUND) {
        pAddSA->Flags |= OFFLOAD_OUTBOUND_SA;
    } else {
        pAddSA->Flags |= OFFLOAD_INBOUND_SA;
    }

    if (pSA->sa_Flags & FLAGS_SA_TUNNEL) {
        pAddSA->DestTunnelAddr = pSA->sa_TunnelAddr;
        pAddSA->SrcTunnelAddr = pSA->sa_SrcTunnelAddr;
    }

    for (Index = 0; Index < pSA->sa_NumOps; Index++) {
        pSAInfo = &pAddSA->SecAssoc[Index];

        pSAInfo->Operation = pSA->sa_Operation[Index];
        pSAInfo->SPI = pSA->sa_OtherSPIs[Index];
        pSAInfo->EXT_INT_ALGO = pSA->INT_ALGO(Index);
        pSAInfo->EXT_INT_KEYLEN = pSA->INT_KEYLEN(Index);
        pSAInfo->EXT_INT_ROUNDS = pSA->INT_ROUNDS(Index);

        pSAInfo->EXT_CONF_ALGO = pSA->CONF_ALGO(Index);
        pSAInfo->EXT_CONF_KEYLEN = pSA->CONF_KEYLEN(Index);
        pSAInfo->EXT_CONF_ROUNDS = pSA->CONF_ROUNDS(Index);

        //
        // now get the keys in
        //
        ASSERT(Len >= sizeof(OFFLOAD_IPSEC_ADD_SA) + pSA->INT_KEYLEN(Index) + pSA->CONF_KEYLEN(Index));

        RtlCopyMemory(  pAddSA->KeyMat + Offset,
                        pSA->CONF_KEY(Index),
                        pSA->CONF_KEYLEN(Index));

        RtlCopyMemory(  pAddSA->KeyMat + Offset + pSA->CONF_KEYLEN(Index),
                        pSA->INT_KEY(Index),
                        pSA->INT_KEYLEN(Index));

        Offset += pSA->INT_KEYLEN(Index) + pSA->CONF_KEYLEN(Index);
        pAddSA->KeyLen += Offset;

        IPSEC_DEBUG(LL_A, DBF_HW, ("pAddSA: %p, keylen: %lx, KeyMat: %p", pAddSA, pAddSA->KeyLen, pAddSA->KeyMat));
    }
}


NDIS_STATUS
IPSecPlumbHw(
    IN  PVOID           DestIF,
    IN  PVOID           Buf,
    IN  ULONG           Len,
    IN  NDIS_OID        Oid
    )
/*++

Routine Description:

    Plumbs the input outbound and its corresponding inbound SA
    into the hw accelerator.

Arguments:

    DestIF - the IP Interface
    Buf - buffer to set info
    Len - length

Return Value:

    status of the operation

--*/
{
#if DBG
    NTSTATUS    status;

    if (Oid == OID_TCP_TASK_IPSEC_ADD_SA) {
        IPSEC_INCREMENT(NumAddSA);
    }
    if (Oid == OID_TCP_TASK_IPSEC_DELETE_SA) {
        IPSEC_INCREMENT(NumDelSA);
    }
    if (Oid == OID_TCP_TASK_IPSEC_ADD_UDPESP_SA) {
        IPSEC_INCREMENT(NumAddEncapSA);
    }
    if (Oid == OID_TCP_TASK_IPSEC_DELETE_UDPESP_SA) {
        IPSEC_INCREMENT(NumDelEncapSA);
    }

    status = TCPIP_NDIS_REQUEST(DestIF,
                                NdisRequestSetInformation,
                                Oid,
                                Buf,
                                Len,
                                NULL);

    if (status == STATUS_SUCCESS) {
        if (Oid == OID_TCP_TASK_IPSEC_ADD_SA) {
            IPSEC_INCREMENT(NumAddSU);
        }
        if (Oid == OID_TCP_TASK_IPSEC_DELETE_SA) {
            IPSEC_INCREMENT(NumDelSU);
        }
        if (Oid == OID_TCP_TASK_IPSEC_ADD_UDPESP_SA) {
            IPSEC_INCREMENT(NumAddEncapSU);
        }
        if (Oid == OID_TCP_TASK_IPSEC_DELETE_UDPESP_SA) {
            IPSEC_INCREMENT(NumDelEncapSU);
        }
    } else {
        if (Oid == OID_TCP_TASK_IPSEC_ADD_SA) {
            IPSEC_INCREMENT(NumAddFA);
        }
        if (Oid == OID_TCP_TASK_IPSEC_DELETE_SA) {
            IPSEC_INCREMENT(NumDelFA);
        }
        if (Oid == OID_TCP_TASK_IPSEC_ADD_UDPESP_SA) {
            IPSEC_INCREMENT(NumAddEncapFA);
        }
        if (Oid == OID_TCP_TASK_IPSEC_DELETE_UDPESP_SA) {
            IPSEC_INCREMENT(NumDelEncapFA);
        }
    }

    return  status;
#else
    return TCPIP_NDIS_REQUEST(  DestIF,
                                NdisRequestSetInformation,
                                Oid,
                                Buf,
                                Len,
                                NULL);
#endif
}


NTSTATUS
IPSecSendOffload(
    IN  IPHeader UNALIGNED  *pIPHeader,
    IN  PNDIS_PACKET        Packet,
    IN  Interface           *DestIF,
    IN  PSA_TABLE_ENTRY     pSA,
    IN  PSA_TABLE_ENTRY     pNextSA,
    IN  PVOID               *ppSCContext,
    IN  BOOLEAN             *pfCryptoOnly
    )
{
    KIRQL                           kIrql;
    BOOLEAN                         fRefBumped = FALSE;
    NTSTATUS                        status = STATUS_UNSUCCESSFUL;
    PSA_TABLE_ENTRY                 pSaveSA = NULL;
    PIPSEC_SEND_COMPLETE_CONTEXT    pContext = NULL;
    PNDIS_IPSEC_PACKET_INFO         IPSecPktInfo = NULL;
    PNDIS_PACKET_EXTENSION          PktExt = NDIS_PACKET_EXTENSION_FROM_PACKET(Packet);
    BOOL                            bTptOverTunCheckDone = FALSE;


    IPSEC_DEBUG(LL_A, DBF_HW, ("IPSecSendOffload: DestIF: %p, DestIF->Flags: %lx", DestIF, DestIF->if_IPSecOffloadFlags));

    *pfCryptoOnly = FALSE;

    //
    // See if options are supported.
    //
    if (((pIPHeader->iph_verlen & (UCHAR)~IP_VER_FLAG) << 2) > sizeof(IPHeader) &&
        !(DestIF->if_IPSecOffloadFlags & IPSEC_OFFLOAD_V4_OPTIONS)) {
        status = STATUS_UNSUCCESSFUL;
        IPSEC_DEBUG(LL_A, DBF_HW, ("Options present - not offloading the packet. HdrLen %d",
                    ((pIPHeader->iph_verlen & (UCHAR)~IP_VER_FLAG) << 2)));
        return (status);
    }

    AcquireWriteLock(&g_ipsec.SADBLock, &kIrql);

    do {
        if ((pSA->sa_Flags & FLAGS_SA_HW_PLUMBED) &&
            pSA->sa_IPIF == DestIF) {

            if (*ppSCContext == NULL) {
                pContext = IPSecAllocateSendCompleteCtx(IPSEC_TAG_HW);

                if (!pContext) {
                    IPSEC_DEBUG(LL_A, DBF_HW, ("Failed to alloc. SendCtx"));
                    status = STATUS_INSUFFICIENT_RESOURCES;
                    *ppSCContext = NULL;
                    break;
                }

                IPSEC_INCREMENT(g_ipsec.NumSends);

                IPSecZeroMemory(pContext, sizeof(IPSEC_SEND_COMPLETE_CONTEXT));

#if DBG
                RtlCopyMemory(pContext->Signature, "ISC5", 4);
#endif
                *ppSCContext = pContext;
            } else {
                pContext = *ppSCContext;
            }

            if (IPSecPktInfo == NULL) {
                if (IPSecPktInfo = IPSecAllocatePktInfo(IPSEC_TAG_HW_PKTINFO)) {
                    IPSecZeroMemory(IPSecPktInfo, sizeof(NDIS_IPSEC_PACKET_INFO));

                    pContext->Flags |= SCF_PKTINFO;
                    pContext->PktInfo = IPSecPktInfo;
                } else {
                    IPSEC_DEBUG(LL_A, DBF_HW, ("Failed to alloc. PktInfo"));
                    status = STATUS_UNSUCCESSFUL;
                    break;
                }
            }

            PktExt->NdisPacketInfo[IpSecPacketInfo] = IPSecPktInfo;

            //
            // if this is the nextOperationSA
            //
            if (fRefBumped) {
                IPSEC_DEBUG(LL_A, DBF_HW, ("Offloading... pSA: %p, NextOffloadHandle %p", pSA, pSA->sa_OffloadHandle));
                IPSecPktInfo->Transmit.NextOffloadHandle = pSA->sa_OffloadHandle;
            } else {
                IPSEC_DEBUG(LL_A, DBF_HW, ("Offloading... pSA: %p, OffloadHandle %p", pSA, pSA->sa_OffloadHandle));
                IPSecPktInfo->Transmit.OffloadHandle = pSA->sa_OffloadHandle;
            }

            *pfCryptoOnly = TRUE;

            IPSEC_DEBUG(LL_A, DBF_HW, ("Using Hw for SA->handle: %p on IF: %p IPSecPktInfo: %p *pfCryptoOnly %d", pSA->sa_OffloadHandle, DestIF, IPSecPktInfo, *pfCryptoOnly));

            status = STATUS_SUCCESS;
        } else if (!(pSA->sa_Flags & FLAGS_SA_HW_PLUMB_FAILED) && !pSA->sa_IPIF) {
            PUCHAR  outBuf;
            ULONG   outLen;
            LONG    Index;

            pSA->sa_Flags |= FLAGS_SA_HW_PLUMB_FAILED;
		
	    /* DELETE 
	    if ((SA_UDP_ENCAP_TYPE_OTHER == pSA->sa_EncapType ) ||
                (pNextSA && (SA_UDP_ENCAP_TYPE_OTHER == pNextSA->sa_EncapType ))) {
                status = STATUS_UNSUCCESSFUL;
                break;
            }*/

            //
            // See if CryptoOnly mode is supported.
            //
            if (!(DestIF->if_IPSecOffloadFlags & IPSEC_OFFLOAD_CRYPTO_ONLY)) {
                status = STATUS_UNSUCCESSFUL;
                break;
            }

            //
            // No need to offload soft SAs.
            //
            if (pSA->sa_Operation[0] == None) {
                status = STATUS_UNSUCCESSFUL;
                break;
            }

            //
            // See if transport over tunnel mode is supported.
            //
            if (pNextSA && !bTptOverTunCheckDone) {
                bTptOverTunCheckDone = TRUE;
                ASSERT((pNextSA->sa_Flags & FLAGS_SA_TUNNEL));
                if (SA_UDP_ENCAP_TYPE_IKE ==  pNextSA->sa_EncapType ) {
                    ASSERT((SA_UDP_ENCAP_TYPE_IKE == pSA->sa_EncapType));
                    if (!(DestIF->if_IPSecOffloadFlags & IPSEC_OFFLOAD_TPT_OVER_TUNNEL_UDPESP_IKE)) {
                        status = STATUS_UNSUCCESSFUL;
                        break;
                    }
                }
                else if (SA_UDP_ENCAP_TYPE_OTHER == pNextSA->sa_EncapType){
                	ASSERT((SA_UDP_ENCAP_TYPE_OTHER==pSA->sa_EncapType));
                	if ( !(DestIF->if_IPSecOffloadFlags  & IPSEC_OFFLOAD_TPT_OVER_TUNNEL_UDPESP_OTHER)){
                		status=STATUS_UNSUCCESSFUL;
                		break;
                		}
                	}
                else {
                    if (SA_UDP_ENCAP_TYPE_IKE==pSA->sa_EncapType ) {
                        if (!(DestIF->if_IPSecOffloadFlags & IPSEC_OFFLOAD_TPT_UDPESP_OVER_PURE_TUNNEL_IKE)) {
                            status = STATUS_UNSUCCESSFUL;
                            break;
                        }
                    }
                    else
                    	if (SA_UDP_ENCAP_TYPE_OTHER == pSA->sa_EncapType ) {
                        if (!(DestIF->if_IPSecOffloadFlags & IPSEC_OFFLOAD_TPT_UDPESP_OVER_PURE_TUNNEL_OTHER)) {
                            status = STATUS_UNSUCCESSFUL;
                            break;
                        	}
                    	}
                    else {
                        if (!(DestIF->if_IPSecOffloadFlags & IPSEC_OFFLOAD_TPT_TUNNEL)) {
                            status = STATUS_UNSUCCESSFUL;
                            break;
                        }
                    }
                }
            }

            //
            // Tunnel required, but not supported, don't plumb.
            //
            if (pSA->sa_Flags & FLAGS_SA_TUNNEL) {
                if ((SA_UDP_ENCAP_TYPE_IKE == pSA->sa_EncapType) ||(SA_UDP_ENCAP_TYPE_OTHER == pSA->sa_EncapType)) {
                    ASSERT(IS_AH_SA(pSA) == 0);
                    ASSERT(IS_ESP_SA(pSA));
                    ASSERT(pSA->sa_NumOps == 1);
                    if ((!(DestIF->if_IPSecOffloadFlags & IPSEC_OFFLOAD_TUNNEL_UDPESP_IKE) && (SA_UDP_ENCAP_TYPE_IKE == pSA->sa_EncapType ))
                    	||(!(DestIF->if_IPSecOffloadFlags & IPSEC_OFFLOAD_TUNNEL_UDPESP_OTHER) && (SA_UDP_ENCAP_TYPE_OTHER == pSA->sa_EncapType)))
                    	{
                        status = STATUS_UNSUCCESSFUL;
                        break;
                    }
                }
                else {
                    if ((IS_AH_SA(pSA) &&
                        !(DestIF->if_IPSecOffloadFlags & IPSEC_OFFLOAD_AH_TUNNEL)) ||
                        (IS_ESP_SA(pSA) &&
                        !(DestIF->if_IPSecOffloadFlags & IPSEC_OFFLOAD_ESP_TUNNEL))) {
                        status = STATUS_UNSUCCESSFUL;
                        break;
                    }
                }
            }
            else {
                if ((SA_UDP_ENCAP_TYPE_IKE==pSA->sa_EncapType) ||(SA_UDP_ENCAP_TYPE_OTHER==pSA->sa_EncapType)) {
                    ASSERT(IS_AH_SA(pSA) == 0);
                    ASSERT(IS_ESP_SA(pSA));
                    ASSERT(pSA->sa_NumOps == 1);
                    if (((!(DestIF->if_IPSecOffloadFlags & IPSEC_OFFLOAD_TPT_UDPESP_IKE)) && (SA_UDP_ENCAP_TYPE_IKE==pSA->sa_EncapType))
                    	||((!(DestIF->if_IPSecOffloadFlags & IPSEC_OFFLOAD_TPT_UDPESP_OTHER)) && (SA_UDP_ENCAP_TYPE_OTHER==pSA->sa_EncapType))){
                        status = STATUS_UNSUCCESSFUL;
                        break;
                    }
                }
                else {
                    if ((IS_AH_SA(pSA) &&
                        !(DestIF->if_IPSecOffloadFlags & IPSEC_OFFLOAD_AH_TPT)) ||
                        (IS_ESP_SA(pSA) &&
                        !(DestIF->if_IPSecOffloadFlags & IPSEC_OFFLOAD_ESP_TPT))) {
                        status = STATUS_UNSUCCESSFUL;
                        break;
                    }
                }
            }

            //
            // AH + ESP required, but not supported, don't plumb.
            //
            if (pSA->sa_NumOps > 1 &&
                !(DestIF->if_IPSecOffloadFlags & IPSEC_OFFLOAD_AH_ESP)) {
                status = STATUS_UNSUCCESSFUL;
                break;
            }

            //
            // Check XMT capabilities.
            //
            if ((IS_AH_SA(pSA) &&
                !(DestIF->if_IPSecOffloadFlags & IPSEC_OFFLOAD_AH_XMT)) ||
                (IS_ESP_SA(pSA) &&
                !(DestIF->if_IPSecOffloadFlags & IPSEC_OFFLOAD_ESP_XMT))) {
                status = STATUS_UNSUCCESSFUL;
                break;
            }

            if ((SA_UDP_ENCAP_TYPE_IKE == pSA->sa_EncapType) ||(SA_UDP_ENCAP_TYPE_OTHER==pSA->sa_EncapType )) {
                outLen = sizeof(OFFLOAD_IPSEC_ADD_UDPESP_SA);
            }
            else {
                outLen = sizeof(OFFLOAD_IPSEC_ADD_SA);
            }

            for (Index = 0; Index < pSA->sa_NumOps; Index++) {
                //
                // Check offload capability bits with those in the SA.
                //
                if ((pSA->INT_ALGO(Index) == IPSEC_AH_MD5) &&
                    (!(DestIF->if_IPSecOffloadFlags & IPSEC_OFFLOAD_AH_MD5)) ||
                    ((pSA->INT_ALGO(Index) == IPSEC_AH_SHA) &&
                    (!(DestIF->if_IPSecOffloadFlags & IPSEC_OFFLOAD_AH_SHA_1))) ||
                    ((pSA->CONF_ALGO(Index) == IPSEC_ESP_NONE) &&
                    (!(DestIF->if_IPSecOffloadFlags & IPSEC_OFFLOAD_ESP_NONE))) ||
                    ((pSA->CONF_ALGO(Index) == IPSEC_ESP_DES) &&
                    (!(DestIF->if_IPSecOffloadFlags & IPSEC_OFFLOAD_ESP_DES))) ||
                    ((pSA->CONF_ALGO(Index) == IPSEC_ESP_3_DES) &&
                    (!(DestIF->if_IPSecOffloadFlags & IPSEC_OFFLOAD_ESP_3_DES)))) {
                    status = STATUS_UNSUCCESSFUL;
                    goto out;
                }

                outLen += pSA->INT_KEYLEN(Index) + pSA->CONF_KEYLEN(Index);
            }

            //
            // This SA can be offloaded.
            //
            pSA->sa_Flags |= FLAGS_SA_OFFLOADABLE;

            IPSEC_DEBUG(LL_A, DBF_HW, ("outLen: %lx", outLen));

            outBuf = IPSecAllocateMemory(outLen, IPSEC_TAG_HW_ADDSA);

            if (outBuf) {

                IPSecZeroMemory(outBuf, outLen);

                if ((SA_UDP_ENCAP_TYPE_IKE == pSA->sa_EncapType) || (SA_UDP_ENCAP_TYPE_OTHER == pSA->sa_EncapType )){
                    IPSecFillHwAddEncapSa(pSA, NULL, outBuf, outLen);
                }
                else {
                    IPSecFillHwAddSA(pSA, outBuf, outLen);
                }

                //
                // Bump the SA reference count to make sure they won't 
                // go away during the processing of the work item.
                //
                IPSecRefSA(pSA);

                //
                // Plumb the SA by scheduling a work item; the SA will
                // not be used for offload until plumbing succeeds.
                //
                IPSecBufferPlumbSA( DestIF,
                                    pSA,
                                    NULL,
                                    outBuf,
                                    outLen);

                //
                // Return failure here so the caller does it in software.
                //
                status = STATUS_UNSUCCESSFUL;
                break;
            } else {
                IPSEC_DEBUG(LL_A, DBF_HW, ("Memory: Failed to plumb outboundSA: %p on IF: %p", pSA, DestIF));
                status = STATUS_UNSUCCESSFUL;
                break;
            }
        } else {
            status = STATUS_UNSUCCESSFUL;
            break;
        }

        if (pNextSA && !fRefBumped) {
            IPSEC_DEBUG(LL_A, DBF_HW, ("RefBumped on SA: %p", pSA));
            pSaveSA = pSA;
            pSA = pNextSA;
            fRefBumped = TRUE;
        } else {
            break;
        }
    } while (TRUE);

out:
    if (status == STATUS_SUCCESS && (*pfCryptoOnly)) {
        ASSERT(pContext);
        ASSERT(pContext->Flags & SCF_PKTINFO);

        if (fRefBumped) {
            IPSecRefSA(pSaveSA);
            IPSecRefSA(pNextSA);
            IPSEC_INCREMENT(pSaveSA->sa_NumSends);
            IPSEC_INCREMENT(pNextSA->sa_NumSends);
            pContext->pSA = pSaveSA;
            pContext->pNextSA = pNextSA;
        } else {
            IPSecRefSA(pSA);
            IPSEC_INCREMENT(pSA->sa_NumSends);
            pContext->pSA = pSA;
        }
    } else {
        if (IPSecPktInfo) {
            ASSERT(pContext);
            ASSERT(pContext->Flags & SCF_PKTINFO);

            IPSecFreePktInfo(pContext->PktInfo);

            pContext->Flags &= ~SCF_PKTINFO;
            pContext->PktInfo = NULL;

            PktExt->NdisPacketInfo[IpSecPacketInfo] = NULL;
        }

        status = STATUS_UNSUCCESSFUL;
        *pfCryptoOnly = FALSE;
    }

    ReleaseWriteLock(&g_ipsec.SADBLock, kIrql);

    return  status;
}


NTSTATUS
IPSecRecvOffload(
    IN  IPHeader UNALIGNED  *pIPHeader,
    IN  Interface           *DestIF,
    IN  PSA_TABLE_ENTRY     pSA
    )
{
    KIRQL                           kIrql;
    NTSTATUS                        status = STATUS_UNSUCCESSFUL;
    PPARSER_IFENTRY                 pParserIfEntry = NULL;


    IPSEC_DEBUG(LL_A, DBF_HW, ("IPSecRecvOffload: DestIF: %p, DestIF->Flags: %lx", DestIF, DestIF->if_IPSecOffloadFlags));

    AcquireWriteLock(&g_ipsec.SADBLock, &kIrql);

    // Make sure SA is fully associated
    if (!pSA->sa_AssociatedSA) {
        status = STATUS_UNSUCCESSFUL;
        goto out;
    }

    if (!(pSA->sa_Flags & FLAGS_SA_HW_PLUMB_FAILED) &&
        !(pSA->sa_Flags & FLAGS_SA_HW_PLUMBED) &&
        !pSA->sa_IPIF) {
        PUCHAR  inBuf;
        ULONG   inLen;
        LONG    Index;

        pSA->sa_Flags |= FLAGS_SA_HW_PLUMB_FAILED;

	/* DELETE
	if (SA_UDP_ENCAP_TYPE_OTHER == pSA->sa_EncapType){
		status = STATUS_UNSUCCESSFUL;
            goto out;
        }
	*/	

        //
        // See if CryptoOnly mode is supported.
        //
        if (!(DestIF->if_IPSecOffloadFlags & IPSEC_OFFLOAD_CRYPTO_ONLY)) {
            status = STATUS_UNSUCCESSFUL;
            goto out;
        }

        //
        // No need to offload soft SAs.
        //
        if (pSA->sa_Operation[0] == None) {
            status = STATUS_UNSUCCESSFUL;
            goto out;
        }

        //
        // Tunnel required, but not supported, don't plumb.
        //
        if (pSA->sa_Flags & FLAGS_SA_TUNNEL) {
            if (SA_UDP_ENCAP_TYPE_IKE == pSA->sa_EncapType) {
                ASSERT(IS_AH_SA(pSA) == 0);
                ASSERT(IS_ESP_SA(pSA));
                ASSERT(pSA->sa_NumOps == 1);
                if (!(DestIF->if_IPSecOffloadFlags & IPSEC_OFFLOAD_TUNNEL_UDPESP_IKE)) {
                    status = STATUS_UNSUCCESSFUL;
                    goto out;
                }
            }
            else
            	if (SA_UDP_ENCAP_TYPE_OTHER== pSA->sa_EncapType) {
                ASSERT(IS_AH_SA(pSA) == 0);
                ASSERT(IS_ESP_SA(pSA));
                ASSERT(pSA->sa_NumOps == 1);
                if (!(DestIF->if_IPSecOffloadFlags & IPSEC_OFFLOAD_TUNNEL_UDPESP_OTHER )) {
                    status = STATUS_UNSUCCESSFUL;
                    goto out;
                }
            }
            else {
                if ((IS_AH_SA(pSA) &&
                    !(DestIF->if_IPSecOffloadFlags & IPSEC_OFFLOAD_AH_TUNNEL)) ||
                    (IS_ESP_SA(pSA) &&
                    !(DestIF->if_IPSecOffloadFlags & IPSEC_OFFLOAD_ESP_TUNNEL))) {
                    status = STATUS_UNSUCCESSFUL;
                    goto out;
                }
            }
        }
        else {
            if (SA_UDP_ENCAP_TYPE_IKE == pSA->sa_EncapType) {
                ASSERT(IS_AH_SA(pSA) == 0);
                ASSERT(IS_ESP_SA(pSA));
                ASSERT(pSA->sa_NumOps == 1);
                if (!(DestIF->if_IPSecOffloadFlags & IPSEC_OFFLOAD_TPT_UDPESP_IKE)) {
                    status = STATUS_UNSUCCESSFUL;
                    goto out;
                }
            }
            else
            	if (SA_UDP_ENCAP_TYPE_OTHER == pSA->sa_EncapType) {
                ASSERT(IS_AH_SA(pSA) == 0);
                ASSERT(IS_ESP_SA(pSA));
                ASSERT(pSA->sa_NumOps == 1);
                if (!(DestIF->if_IPSecOffloadFlags & IPSEC_OFFLOAD_TPT_UDPESP_OTHER)) {
                    status = STATUS_UNSUCCESSFUL;
                    goto out;
                }
            }
            else {
                if ((IS_AH_SA(pSA) &&
                    !(DestIF->if_IPSecOffloadFlags & IPSEC_OFFLOAD_AH_TPT)) ||
                    (IS_ESP_SA(pSA) &&
                    !(DestIF->if_IPSecOffloadFlags & IPSEC_OFFLOAD_ESP_TPT))) {
                    status = STATUS_UNSUCCESSFUL;
                    goto out;
                }
            }
        }

        //
        // AH + ESP required, but not supported, don't plumb.
        //
        if (pSA->sa_NumOps > 1 &&
            !(DestIF->if_IPSecOffloadFlags & IPSEC_OFFLOAD_AH_ESP)) {
            status = STATUS_UNSUCCESSFUL;
            goto out;
        }

        //
        // Check RCV capabilities.
        //
        if ((IS_AH_SA(pSA) &&
            !(DestIF->if_IPSecOffloadFlags & IPSEC_OFFLOAD_AH_RCV)) ||
            (IS_ESP_SA(pSA) &&
            !(DestIF->if_IPSecOffloadFlags & IPSEC_OFFLOAD_ESP_RCV))) {
            status = STATUS_UNSUCCESSFUL;
            goto out;
        }

        if ((SA_UDP_ENCAP_TYPE_IKE==pSA->sa_EncapType) || (SA_UDP_ENCAP_TYPE_OTHER==pSA->sa_EncapType)){
            inLen = sizeof(OFFLOAD_IPSEC_ADD_UDPESP_SA);
        }
        else {
            inLen = sizeof(OFFLOAD_IPSEC_ADD_SA);
        }

        for (Index = 0; Index < pSA->sa_NumOps; Index++) {
            //
            // Check offload capability bits with those in the SA
            //
            if ((pSA->INT_ALGO(Index) == IPSEC_AH_MD5) &&
                (!(DestIF->if_IPSecOffloadFlags & IPSEC_OFFLOAD_AH_MD5)) ||
                ((pSA->INT_ALGO(Index) == IPSEC_AH_SHA) &&
                (!(DestIF->if_IPSecOffloadFlags & IPSEC_OFFLOAD_AH_SHA_1))) ||
                ((pSA->CONF_ALGO(Index) == IPSEC_ESP_NONE) &&
                (!(DestIF->if_IPSecOffloadFlags & IPSEC_OFFLOAD_ESP_NONE))) ||
                ((pSA->CONF_ALGO(Index) == IPSEC_ESP_DES) &&
                (!(DestIF->if_IPSecOffloadFlags & IPSEC_OFFLOAD_ESP_DES))) ||
                ((pSA->CONF_ALGO(Index) == IPSEC_ESP_3_DES) &&
                (!(DestIF->if_IPSecOffloadFlags & IPSEC_OFFLOAD_ESP_3_DES)))) {
                status = STATUS_UNSUCCESSFUL;
                goto out;
            }

            inLen += pSA->INT_KEYLEN(Index) + pSA->CONF_KEYLEN(Index);
        }

        IPSEC_DEBUG(LL_A, DBF_HW, ("inLen: %lx", inLen));

        inBuf = IPSecAllocateMemory(inLen, IPSEC_TAG_HW_ADDSA);

        if (inBuf) {

            IPSecZeroMemory(inBuf, inLen);

            if ((SA_UDP_ENCAP_TYPE_IKE == pSA->sa_EncapType) || (SA_UDP_ENCAP_TYPE_OTHER==pSA->sa_EncapType)){
                status = GetParserEntry(pSA, DestIF, &pParserIfEntry);
                if (!NT_SUCCESS(status)) {
                    IPSecFreeMemory(inBuf);
                    status = STATUS_UNSUCCESSFUL;
                    goto out;
                }
                IPSecFillHwAddEncapSa(pSA, pParserIfEntry, inBuf, inLen);
            }
            else {
                IPSecFillHwAddSA(pSA, inBuf, inLen);
            }

            //
            // Bump the SA reference count to make sure they won't 
            // go away during the processing of the work item.
            //
            IPSecRefSA(pSA);

            //
            // Plumb the SA by scheduling a work item; the SA will
            // not be used for offload until plumbing succeeds.
            //
            status = IPSecBufferPlumbSA(DestIF,
                                        pSA,
                                        pParserIfEntry,
                                        inBuf,
                                        inLen);

            //
            // Return failure here so the caller does it in software.
            //
            status = STATUS_UNSUCCESSFUL;
            goto out;
        } else {
            IPSEC_DEBUG(LL_A, DBF_HW, ("Memory: Failed to plumb inboundSA: %p on IF: %p", pSA, DestIF));
            status = STATUS_UNSUCCESSFUL;
            goto out;
        }
    }

out:
    ReleaseWriteLock(&g_ipsec.SADBLock, kIrql);

    return  status;
}


NTSTATUS
IPSecDelHWSA(
    IN  PSA_TABLE_ENTRY pSA
    )
{
    NTSTATUS    status = STATUS_SUCCESS;
    OFFLOAD_IPSEC_DELETE_UDPESP_SA  OffldDelUdpEspSa;
    KIRQL                           kIrql = 0;

    ASSERT(pSA->sa_Flags & FLAGS_SA_HW_PLUMBED);

    pSA->sa_Flags &= ~FLAGS_SA_HW_PLUMBED;
    pSA->sa_Flags |= FLAGS_SA_HW_PLUMB_FAILED;

    if ((pSA->sa_Flags & FLAGS_SA_OUTBOUND) &&
        IPSEC_GET_VALUE(pSA->sa_NumSends) > 0) {
        pSA->sa_Flags |= FLAGS_SA_HW_DELETE_SA;
        return  STATUS_PENDING;
    }

    pSA->sa_Flags &= ~FLAGS_SA_HW_DELETE_SA;
    pSA->sa_Flags |= FLAGS_SA_HW_DELETE_QUEUED;

    ASSERT(pSA->sa_IPIF);

    if (pSA->sa_IPIF) {
    	/* DELETE
    	 ASSERT((SA_UDP_ENCAP_TYPE_OTHER!=pSA->sa_EncapType));
    	 */
        if ((SA_UDP_ENCAP_TYPE_IKE == pSA->sa_EncapType ) || (SA_UDP_ENCAP_TYPE_OTHER == pSA->sa_EncapType)){

            OffldDelUdpEspSa.OffloadHandle = pSA->sa_OffloadHandle;

            if (!(pSA->sa_Flags & FLAGS_SA_OUTBOUND)) {
                AcquireWriteLock(&g_ipsec.SADBLock, &kIrql);
                OffldDelUdpEspSa.EncapTypeEntryOffldHandle = UploadParserEntryAndGetHandle(pSA, pSA->sa_IPIF);
                ReleaseWriteLock(&g_ipsec.SADBLock, kIrql);
            } else {
                OffldDelUdpEspSa.EncapTypeEntryOffldHandle = NULL;
            }

            status = IPSecPlumbHw(  pSA->sa_IPIF,
                                    &OffldDelUdpEspSa,
                                    sizeof(OFFLOAD_IPSEC_DELETE_UDPESP_SA),
                                    OID_TCP_TASK_IPSEC_DELETE_UDPESP_SA);

        } else {
            status = IPSecPlumbHw(  pSA->sa_IPIF,
                                    &pSA->sa_OffloadHandle,
                                    sizeof(OFFLOAD_IPSEC_DELETE_SA),
                                    OID_TCP_TASK_IPSEC_DELETE_SA);
        }

        IPSEC_DEBUG(LL_A, DBF_HWAPI, ("DelHWSA %s: %p, handle: %p, status: %lx",
                    (pSA->sa_Flags & FLAGS_SA_OUTBOUND)? "outbound": "inbound",
                    pSA,
                    pSA->sa_OffloadHandle,
                    status));

        IPSEC_DEC_STATISTIC(dwNumOffloadedSAs);

        IPSecDerefSA(pSA);
    }

    return status;
}


NTSTATUS
IPSecDelHWSAAtDpc(
    IN  PSA_TABLE_ENTRY pSA
    )
{
    ASSERT(pSA->sa_Flags & (FLAGS_SA_HW_PLUMBED | FLAGS_SA_HW_DELETE_SA));

    pSA->sa_Flags &= ~FLAGS_SA_HW_PLUMBED;
    pSA->sa_Flags |= FLAGS_SA_HW_PLUMB_FAILED;

    if ((pSA->sa_Flags & FLAGS_SA_OUTBOUND) &&
        IPSEC_GET_VALUE(pSA->sa_NumSends) > 0) {
        pSA->sa_Flags |= FLAGS_SA_HW_DELETE_SA;
        return  STATUS_PENDING;
    }

    pSA->sa_Flags &= ~FLAGS_SA_HW_DELETE_SA;
    pSA->sa_Flags |= FLAGS_SA_HW_DELETE_QUEUED;

    ASSERT(pSA->sa_IPIF);

    ExInitializeWorkItem(   &pSA->sa_QueueItem,
                            IPSecProcessDeleteSA,
                            (PVOID)pSA);

    ExQueueWorkItem(&pSA->sa_QueueItem, DelayedWorkQueue);
    
    IPSEC_INCREMENT(g_ipsec.NumWorkers);

    return STATUS_SUCCESS;
}


NTSTATUS
IPSecBufferPlumbSA(
    IN  Interface       *DestIF,
    IN  PSA_TABLE_ENTRY pSA,
    PPARSER_IFENTRY     pParserIfEntry,
    IN  PUCHAR          Buf,
    IN  ULONG           Len
    )
{
    PIPSEC_PLUMB_SA pPlumbSA;

    pPlumbSA = IPSecAllocateMemory(sizeof(IPSEC_PLUMB_SA), IPSEC_TAG_HW_PLUMB);
    if (pPlumbSA == NULL) {
        IPSecFreeMemory(Buf);

        if (pSA) {
            IPSecDerefSA(pSA);
        }

        if (NULL != pParserIfEntry) {
            DerefParserEntry (pParserIfEntry);
        }
        return  STATUS_INSUFFICIENT_RESOURCES;
    }

    pPlumbSA->pSA = pSA;
    pPlumbSA->pParserIfEntry = pParserIfEntry;
    pPlumbSA->DestIF = DestIF;
    pPlumbSA->Buf = Buf;
    pPlumbSA->Len = Len;

    ExInitializeWorkItem(   &pPlumbSA->PlumbQueueItem,
                            IPSecProcessPlumbSA,
                            (PVOID)pPlumbSA);

    ExQueueWorkItem(&pPlumbSA->PlumbQueueItem, DelayedWorkQueue);
    
    IPSEC_INCREMENT(g_ipsec.NumWorkers);

    return STATUS_SUCCESS;
}


NTSTATUS
IPSecProcessPlumbSA(
    IN  PVOID   Context
    )
{
    PIPSEC_PLUMB_SA pPlumbSA = (PIPSEC_PLUMB_SA)Context;
    NTSTATUS        status = STATUS_SUCCESS;
    KIRQL           kIrql;
    Interface       *DestIF = pPlumbSA->DestIF;
    PSA_TABLE_ENTRY pSA = pPlumbSA->pSA;
    PUCHAR          Buf = pPlumbSA->Buf;
    ULONG           Len = pPlumbSA->Len;
    PPARSER_IFENTRY pParserIfEntry = pPlumbSA->pParserIfEntry;
    NDIS_OID        Oid = OID_TCP_TASK_IPSEC_ADD_SA;

    //
    // Plumb this SA into the hw if acceleration is enabled
    // on this card and it has not been plumbed already.
    //
    if (pSA) {
        IPSEC_DEBUG(LL_A, DBF_HW, ("About to plumb outbound"));
	/* Delete
        ASSERT(SA_UDP_ENCAP_TYPE_OTHER != pSA->sa_EncapType);
        */
        if ((SA_UDP_ENCAP_TYPE_IKE== pSA->sa_EncapType) || (SA_UDP_ENCAP_TYPE_OTHER== pSA->sa_EncapType)){
            Oid = OID_TCP_TASK_IPSEC_ADD_UDPESP_SA;
        }

        status = IPSecPlumbHw(  DestIF,
                                Buf,
                                Len,
                                Oid);

        IPSEC_DEBUG(LL_A, DBF_HWAPI, ("AddHWSA %s: %p, handle: %p, status: %lx",
                    (pSA->sa_Flags & FLAGS_SA_OUTBOUND)? "outbound": "inbound",
                    pSA,
                    ((POFFLOAD_IPSEC_ADD_SA)Buf)->OffloadHandle,
                    status));

        AcquireWriteLock(&g_ipsec.SADBLock, &kIrql);

        pSA->sa_IPIF = DestIF;

        if (status != STATUS_SUCCESS) {
            IPSEC_DEBUG(LL_A, DBF_HW, ("Failed to plumb SA: %p on IF: %p, status: %lx", pSA, DestIF, status));
            pSA->sa_Flags |= FLAGS_SA_HW_PLUMB_FAILED;
            IPSecDerefSA(pSA);
            if (NULL != pParserIfEntry) {
                DerefParserEntry(pParserIfEntry);
            }
        } else {
        	/* DELETE
            ASSERT(SA_UDP_ENCAP_TYPE_OTHER != pSA->sa_EncapType);
            */
            if ((SA_UDP_ENCAP_TYPE_IKE == pSA->sa_EncapType) || (SA_UDP_ENCAP_TYPE_OTHER == pSA->sa_EncapType)){

                pSA->sa_OffloadHandle = ((POFFLOAD_IPSEC_ADD_UDPESP_SA)Buf)->OffloadHandle;
                if (NULL != pParserIfEntry) {
                    pParserIfEntry->hParserIfOffload = ((POFFLOAD_IPSEC_ADD_UDPESP_SA)Buf)->EncapTypeEntryOffldHandle;
                    ASSERT (pParserIfEntry->hParserIfOffload != NULL);
                }

            } else {
                pSA->sa_OffloadHandle = ((POFFLOAD_IPSEC_ADD_SA)Buf)->OffloadHandle;
            }


            IPSEC_DEBUG(LL_A, DBF_HW, ("Success plumb SA: %p, pSA->sa_OffloadHandle %p", pSA, pSA->sa_OffloadHandle));

            pSA->sa_Flags |= FLAGS_SA_HW_PLUMBED;
            pSA->sa_Flags &= ~FLAGS_SA_HW_PLUMB_FAILED;

            IPSEC_INC_STATISTIC(dwNumOffloadedSAs);
        }

        if (status == STATUS_SUCCESS &&
            !(pSA->sa_State == STATE_SA_ACTIVE &&
             (pSA->sa_Flags & FLAGS_SA_ON_FILTER_LIST) &&
             pSA->sa_AssociatedSA)) {
            //
            // SA got deleted before we plumb, call DelHWSA now since
            // HW_PLUMBED wasn't set when the SA was deleted.
            //
            IPSecDelHWSAAtDpc(pSA);
        }

        ReleaseWriteLock(&g_ipsec.SADBLock, kIrql);
    }

    IPSecFreeMemory(Buf);
    IPSecFreeMemory(pPlumbSA);

    IPSEC_DECREMENT(g_ipsec.NumWorkers);

    return status;
}


NTSTATUS
IPSecProcessDeleteSA(
    IN  PVOID   Context
    )
{
    PSA_TABLE_ENTRY pSA = (PSA_TABLE_ENTRY)Context;
    NTSTATUS        status;
    OFFLOAD_IPSEC_DELETE_UDPESP_SA  OffldDelUdpEspSa;
    KIRQL                           kIrql = 0;

    ASSERT(IPSEC_GET_VALUE(pSA->sa_NumSends) == 0);
   /* DELETE
    ASSERT(SA_UDP_ENCAP_TYPE_OTHER != pSA->sa_EncapType);
    */

    if ((SA_UDP_ENCAP_TYPE_IKE == pSA->sa_EncapType) || (SA_UDP_ENCAP_TYPE_OTHER ==pSA->sa_EncapType)) {

        OffldDelUdpEspSa.OffloadHandle = pSA->sa_OffloadHandle;

        if (!(pSA->sa_Flags & FLAGS_SA_OUTBOUND)) {
            AcquireWriteLock(&g_ipsec.SADBLock, &kIrql);
            OffldDelUdpEspSa.EncapTypeEntryOffldHandle = UploadParserEntryAndGetHandle(pSA, pSA->sa_IPIF);
            ReleaseWriteLock(&g_ipsec.SADBLock, kIrql);
        } else {
            OffldDelUdpEspSa.EncapTypeEntryOffldHandle = NULL;
        }

        status = IPSecPlumbHw(  pSA->sa_IPIF,
                                &OffldDelUdpEspSa,
                                sizeof(OFFLOAD_IPSEC_DELETE_UDPESP_SA),
                                OID_TCP_TASK_IPSEC_DELETE_UDPESP_SA);

    } else {

        status = IPSecPlumbHw(  pSA->sa_IPIF,
                                &pSA->sa_OffloadHandle,
                                sizeof(OFFLOAD_IPSEC_DELETE_SA),
                                OID_TCP_TASK_IPSEC_DELETE_SA);

    }

    IPSEC_DEBUG(LL_A, DBF_HWAPI, ("ProcessDeleteSA %s: %p, handle: %p, status: %lx",
                (pSA->sa_Flags & FLAGS_SA_OUTBOUND)? "outbound": "inbound",
                pSA,
                pSA->sa_OffloadHandle,
                status));

    IPSEC_DEC_STATISTIC(dwNumOffloadedSAs);

    IPSecDerefSA(pSA);

    IPSEC_DECREMENT(g_ipsec.NumWorkers);

    return status;
}


NTSTATUS
IPSecNdisStatus(
    IN  PVOID       IPContext,
    IN  UINT        Status
    )
/*++

Routine Description:

    Notify Interface has a NDIS status change.

Arguments:

    IPContext   - This is the Interface notified of status change.

Return Value:

--*/
{
    IPSEC_DEBUG(LL_A, DBF_HWAPI, ("IPSecNdisStatus %lx called for interface %p", Status, IPContext));

    switch (Status) {
        case NDIS_STATUS_NETWORK_UNREACHABLE:
            IPSecDeleteIF(IPContext);
            break;

        case NDIS_STATUS_RESET_START:
            IPSecResetStart(IPContext);
            break;

        case NDIS_STATUS_RESET_END:
            IPSecResetEnd(IPContext);
            break;

        case NDIS_STATUS_INTERFACE_UP:
            IPSecWakeUp(IPContext);
            break;

        default:
            ASSERT(FALSE);
            break;
    }

    return  STATUS_SUCCESS;
}


VOID
IPSecDeleteIF(
    IN  PVOID       IPContext
    )
/*++

Routine Description:

    Notify Interface is deleted so need to clean up SA's that are offloaded
    on the deleted interface.

Arguments:

    IPContext   - This is the Interface being deleted.

Return Value:

--*/
{
    Interface       *DestIF = (Interface *)IPContext;
    PLIST_ENTRY     pFilterEntry;
    PLIST_ENTRY     pSAEntry;
    PFILTER         pFilter;
    PSA_TABLE_ENTRY pSA;
    KIRQL           kIrql;
    LONG            Index;
    LONG            SAIndex;

    IPSEC_DEBUG(LL_A, DBF_HWAPI, ("IPSecDeleteIF called for interface %p", DestIF));

    //
    // Go through all SA's and unmark the offload bits.
    //
    AcquireWriteLock(&g_ipsec.SADBLock, &kIrql);

    for (   Index = MIN_FILTER;
            Index <= MAX_FILTER;
            Index++) {

        for (   pFilterEntry = g_ipsec.FilterList[Index].Flink;
                pFilterEntry != &g_ipsec.FilterList[Index];
                pFilterEntry = pFilterEntry->Flink) {

            pFilter = CONTAINING_RECORD(pFilterEntry,
                                        FILTER,
                                        MaskedLinkage);

            for (   SAIndex = 0;
                    SAIndex < pFilter->SAChainSize;
                    SAIndex++) {

                for (   pSAEntry = pFilter->SAChain[SAIndex].Flink;
                        pSAEntry != &pFilter->SAChain[SAIndex];
                        pSAEntry = pSAEntry->Flink) {

                    pSA = CONTAINING_RECORD(pSAEntry,
                                            SA_TABLE_ENTRY,
                                            sa_FilterLinkage);

                    if ((pSA->sa_Flags & FLAGS_SA_HW_PLUMBED) &&
                        (DestIF == pSA->sa_IPIF)) {

                        pSA->sa_Flags &= ~FLAGS_SA_HW_PLUMBED;
                        pSA->sa_Flags |= FLAGS_SA_HW_PLUMB_FAILED;
                        pSA->sa_Flags &= ~FLAGS_SA_HW_DELETE_SA;

                        IPSEC_DEC_STATISTIC(dwNumOffloadedSAs);

#if DBG
                        NumReset++;
#endif

                        IPSecDerefSA(pSA);
                    }
                }
            }
        }
    }

    FlushParserEntriesForInterface(DestIF);

    ReleaseWriteLock(&g_ipsec.SADBLock, kIrql);
}


VOID
IPSecResetStart(
    IN  PVOID       IPContext
    )
/*++

Routine Description:

    Notify Interface is being reset.

Arguments:

    IPContext   - This is the Interface being reset.

Return Value:

--*/
{
    Interface       *DestIF = (Interface *)IPContext;
    PLIST_ENTRY     pFilterEntry;
    PLIST_ENTRY     pSAEntry;
    PFILTER         pFilter;
    PSA_TABLE_ENTRY pSA;
    KIRQL           kIrql;
    LONG            Index;
    LONG            SAIndex;

    IPSEC_DEBUG(LL_A, DBF_HWAPI, ("IPSecResetStart called for interface %p", DestIF));

    //
    // Go through all SA's and unmark the offload bits.
    //
    AcquireWriteLock(&g_ipsec.SADBLock, &kIrql);

    for (   Index = MIN_FILTER;
            Index <= MAX_FILTER;
            Index++) {

        for (   pFilterEntry = g_ipsec.FilterList[Index].Flink;
                pFilterEntry != &g_ipsec.FilterList[Index];
                pFilterEntry = pFilterEntry->Flink) {

            pFilter = CONTAINING_RECORD(pFilterEntry,
                                        FILTER,
                                        MaskedLinkage);

            for (   SAIndex = 0;
                    SAIndex < pFilter->SAChainSize;
                    SAIndex++) {

                for (   pSAEntry = pFilter->SAChain[SAIndex].Flink;
                        pSAEntry != &pFilter->SAChain[SAIndex];
                        pSAEntry = pSAEntry->Flink) {

                    pSA = CONTAINING_RECORD(pSAEntry,
                                            SA_TABLE_ENTRY,
                                            sa_FilterLinkage);

                    if ((pSA->sa_Flags & FLAGS_SA_HW_PLUMBED) &&
                        (DestIF == pSA->sa_IPIF)) {

                        pSA->sa_Flags &= ~FLAGS_SA_HW_PLUMBED;
                        pSA->sa_Flags |= FLAGS_SA_HW_PLUMB_FAILED;
                        pSA->sa_Flags &= ~FLAGS_SA_HW_DELETE_SA;
                        pSA->sa_Flags |= FLAGS_SA_HW_RESET;

                        IPSEC_DEC_STATISTIC(dwNumOffloadedSAs);

#if DBG
                        NumReset++;
#endif

                        IPSecDerefSA(pSA);
                    }
                }
            }
        }
    }

    IPSecNumResets++;

    FlushParserEntriesForInterface(DestIF);

    ReleaseWriteLock(&g_ipsec.SADBLock, kIrql);
}


VOID
IPSecResetEnd(
    IN  PVOID       IPContext
    )
/*++

Routine Description:

    Notify Interface reset is completed.

Arguments:

    IPContext   - This is the Interface being reset.

Return Value:

--*/
{
    Interface       *DestIF = (Interface *)IPContext;
    PLIST_ENTRY     pFilterEntry;
    PLIST_ENTRY     pSAEntry;
    PFILTER         pFilter;
    PSA_TABLE_ENTRY pSA;
    KIRQL           kIrql;
    LONG            Index;
    LONG            SAIndex;

    IPSEC_DEBUG(LL_A, DBF_HWAPI, ("IPSecResetEnd called for interface %p", DestIF));

    //
    // Go through all SA's and unmark the offload bits.
    //
    AcquireWriteLock(&g_ipsec.SADBLock, &kIrql);

    for (   Index = MIN_FILTER;
            Index <= MAX_FILTER;
            Index++) {

        for (   pFilterEntry = g_ipsec.FilterList[Index].Flink;
                pFilterEntry != &g_ipsec.FilterList[Index];
                pFilterEntry = pFilterEntry->Flink) {

            pFilter = CONTAINING_RECORD(pFilterEntry,
                                        FILTER,
                                        MaskedLinkage);

            for (   SAIndex = 0;
                    SAIndex < pFilter->SAChainSize;
                    SAIndex++) {

                for (   pSAEntry = pFilter->SAChain[SAIndex].Flink;
                        pSAEntry != &pFilter->SAChain[SAIndex];
                        pSAEntry = pSAEntry->Flink) {

                    pSA = CONTAINING_RECORD(pSAEntry,
                                            SA_TABLE_ENTRY,
                                            sa_FilterLinkage);

                    if ((pSA->sa_Flags & FLAGS_SA_HW_PLUMB_FAILED) &&
                        !(pSA->sa_Flags & FLAGS_SA_HW_DELETE_QUEUED) &&
                        (DestIF == pSA->sa_IPIF)) {

                        pSA->sa_Flags &= ~FLAGS_SA_HW_PLUMB_FAILED;
                        pSA->sa_IPIF = NULL;
                    }
                }
            }
        }
    }

    ReleaseWriteLock(&g_ipsec.SADBLock, kIrql);
}


VOID
IPSecWakeUp(
    IN  PVOID       IPContext
    )
/*++

Routine Description:

    Notify Interface has waken up from hibernate.

Arguments:

    IPContext   - This is the Interface that wakes up.

Return Value:

--*/
{
    Interface       *DestIF = (Interface *)IPContext;
    PLIST_ENTRY     pFilterEntry;
    PLIST_ENTRY     pSAEntry;
    PFILTER         pFilter;
    PSA_TABLE_ENTRY pSA;
    KIRQL           kIrql;
    LONG            Index;
    LONG            SAIndex;

    IPSEC_DEBUG(LL_A, DBF_HWAPI, ("IPSecWakeUp called for interface %p", DestIF));

    //
    // Go through all SA's and unmark the offload bits.
    //
    AcquireWriteLock(&g_ipsec.SADBLock, &kIrql);

    for (   Index = MIN_FILTER;
            Index <= MAX_FILTER;
            Index++) {

        for (   pFilterEntry = g_ipsec.FilterList[Index].Flink;
                pFilterEntry != &g_ipsec.FilterList[Index];
                pFilterEntry = pFilterEntry->Flink) {

            pFilter = CONTAINING_RECORD(pFilterEntry,
                                        FILTER,
                                        MaskedLinkage);

            for (   SAIndex = 0;
                    SAIndex < pFilter->SAChainSize;
                    SAIndex++) {

                for (   pSAEntry = pFilter->SAChain[SAIndex].Flink;
                        pSAEntry != &pFilter->SAChain[SAIndex];
                        pSAEntry = pSAEntry->Flink) {

                    pSA = CONTAINING_RECORD(pSAEntry,
                                            SA_TABLE_ENTRY,
                                            sa_FilterLinkage);

                    if ((pSA->sa_Flags & FLAGS_SA_HW_PLUMBED) &&
                        (DestIF == pSA->sa_IPIF)) {

                        pSA->sa_Flags &= ~FLAGS_SA_HW_PLUMBED;
                        pSA->sa_IPIF = NULL;
                        pSA->sa_Flags |= FLAGS_SA_HIBERNATED;

                        IPSEC_DEC_STATISTIC(dwNumOffloadedSAs);

                        IPSecDerefSA(pSA);
                    }
                }
            }
        }
    }

    FlushParserEntriesForInterface(DestIF);

    ReleaseWriteLock(&g_ipsec.SADBLock, kIrql);
}


VOID
IPSecBufferOffloadEvent(
    IN  IPHeader UNALIGNED      *pIPH,
    IN  PNDIS_IPSEC_PACKET_INFO IPSecPktInfo
    )
/*++

Routine Description:

    Log an event for offload failures.

Arguments:

    pIPH            - The IP header of the problem packet.
    IPSecPktInfo    - The per-packet IPSec offload info.

Return Value:

    None

--*/
{
    switch (IPSecPktInfo->Receive.CryptoStatus) {
        case CRYPTO_TRANSPORT_AH_AUTH_FAILED:
            IPSEC_INC_STATISTIC(dwNumPacketsNotAuthenticated);
            IPSecBufferEvent(   pIPH->iph_src,
                                EVENT_IPSEC_AUTH_FAILURE,
                                3,
                                TRUE);
            break;

        case CRYPTO_TRANSPORT_ESP_AUTH_FAILED:
            IPSEC_INC_STATISTIC(dwNumPacketsNotAuthenticated);
            IPSecBufferEvent(   pIPH->iph_src,
                                EVENT_IPSEC_AUTH_FAILURE,
                                4,
                                TRUE);
            break;

        case CRYPTO_TUNNEL_AH_AUTH_FAILED:
            IPSEC_INC_STATISTIC(dwNumPacketsNotAuthenticated);
            IPSecBufferEvent(   pIPH->iph_src,
                                EVENT_IPSEC_AUTH_FAILURE,
                                5,
                                TRUE);
            break;

        case CRYPTO_TUNNEL_ESP_AUTH_FAILED:
            IPSEC_INC_STATISTIC(dwNumPacketsNotAuthenticated);
            IPSecBufferEvent(   pIPH->iph_src,
                                EVENT_IPSEC_AUTH_FAILURE,
                                6,
                                TRUE);
            break;

        case CRYPTO_INVALID_PACKET_SYNTAX:
            IPSecBufferEvent(   pIPH->iph_src,
                                EVENT_IPSEC_BAD_PACKET_SYNTAX,
                                1,
                                TRUE);
            break;

        case CRYPTO_INVALID_PROTOCOL:
            IPSecBufferEvent(   pIPH->iph_src,
                                EVENT_IPSEC_BAD_PROTOCOL_RECEIVED,
                                3,
                                TRUE);
            break;

        case CRYPTO_GENERIC_ERROR:
        default:
            IPSecBufferEvent(   pIPH->iph_src,
                                EVENT_IPSEC_GENERIC_FAILURE,
                                1,
                                TRUE);
            break;
    }
}

