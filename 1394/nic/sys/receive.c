//
// Copyright (c) 1998-1999, Microsoft Corporation, all rights reserved
//
// receive.c
//
// IEEE1394 mini-port/call-manager driver
//
// Mini-port Receive routines
//
// 2/13/1998 ADube Created, 
//

#include <precomp.h>
#define MAX_NUM_SLIST_ENTRY 0x10
#define FRAGMENT_NUM_INVALID ((UINT)-1)
//-----------------------------------------------------------------------------
// Local prototypes (alphabetically)
//-----------------------------------------------------------------------------

VOID
nicAllocateAddressRangeCallback( 
    IN PNOTIFICATION_INFO NotificationInfo 
);

VOID
nicAllocateAddressRangeDebugSpew(
    IN PIRB pIrb 
    );

NDIS_STATUS
nicGetInitializedAddressFifoElement(
    IN     UINT BufferLength, 
    IN OUT PADDRESS_FIFO *ppElement 
    );

NDIS_STATUS
nicGetEmptyAddressFifoElement(
    IN PADDRESS_FIFO *ppElement
    );
    
VOID
nicReceivePacketWorkItem(
    PNDIS_WORK_ITEM pWorkItem,
    PVOID   pContext
    );

VOID
nicAllocateRemainingFifoWorkItem (
    PNDIS_WORK_ITEM pNdisWorkItem, 
    IN PVOID Context
    );

VOID
nicFifoAllocationScheme (
    PRECVFIFO_VCCB pRecvFIFOVc
);

ULONG           ReassemblyAllocated = 0;
extern ULONG           NdisBufferAllocated[NoMoreCodePaths];
extern ULONG           NdisBufferFreed[NoMoreCodePaths];

//-----------------------------------------------------------------------------
// prototype implementation (alphabetically)
//-----------------------------------------------------------------------------


NDIS_STATUS
nicAllocateAddressRange(
    IN PADAPTERCB pAdapter,
    IN PRECVFIFO_VCCB pRecvFIFOVc
    )

/*++

Routine Description:
    This function will use the AllocateAddressRange Bus Api
    To do this it must initialize an S-list with structures
    Allocate and Initialize an Irb and an Irp and call the nic
    nicSubmitIrp routine
    This funcion is used by VCs. This routine will complete synchronously

        
Arguments:
    pAdapter - provides the PDO on which the IRP is sent,
    pRecvFIFOVc - Recv Fifo Vc on which the address range is allocated


Return Value:
        Success - if all allocations and Irp succeeds.

--*/
{

    PIRB                    pIrb = NULL;
    PIRP                    pIrp = NULL; 
    PSLIST_HEADER           pSlistHead = NULL;
    UINT                    cnt = 0;   
    PDEVICE_OBJECT          pPdo = NULL;
    NDIS_STATUS             NdisStatus = NDIS_STATUS_FAILURE;
    UINT                    Length;
    ADDRESS_OFFSET          AddressOffset;
    UINT                    MaxNumSlistEntry = MAX_NUM_SLIST_ENTRY;
    BOOLEAN                 bRefCall = FALSE;
    STORE_CURRENT_IRQL;
    
        
    ASSERT (pRecvFIFOVc == pAdapter->pRecvFIFOVc);
    
    ASSERT (pRecvFIFOVc != NULL);

    TRACE( TL_T, TM_Recv, ( "==>nicAllocateAddressRange, pAdapter 8x, pRecvFIFOVc %x", pAdapter, pRecvFIFOVc ) );

    
    do
    {
        // Increment the Refcount on the VC, so we can gaurantee its presence
        //
        VC_ACQUIRE_LOCK (pRecvFIFOVc)   

        //
        // Add a reference to the pdo block. 
        // This reference is added to guarantee its presence
        // Removed in Free Address Range or at the end of the function
        //
        
        bRefCall =  nicReferenceCall ((PVCCB) pRecvFIFOVc, "nicAllocateAddressRange" ) ;

        VC_RELEASE_LOCK (pRecvFIFOVc);

        if ( bRefCall == FALSE )
        {
            //
            // This will only fail if the Vc is not activated
            //
            NdisStatus = NDIS_STATUS_FAILURE;
        
            break;
        }
        
        //
        //  Allocate an IRB                                                                                                                                
        //
        
        NdisStatus = nicGetIrb (&pIrb);
    
        if (NdisStatus != NDIS_STATUS_SUCCESS)
        {
            break;
        }
    
        ASSERT (pIrb != NULL);
        
        //
        // Initalize the IrB with the correct values
        // AllocateAddressRange
        //

        ASSERT (pRecvFIFOVc->Hdr.Nic1394MediaParams.Destination.AddressType == NIC1394AddressType_FIFO);
        
        AddressOffset.Off_High = pRecvFIFOVc->Hdr.Nic1394MediaParams.Destination.FifoAddress.Off_High;

        AddressOffset.Off_Low = pRecvFIFOVc->Hdr.Nic1394MediaParams.Destination.FifoAddress.Off_Low;

        ASSERT (pRecvFIFOVc->Hdr.MTU !=0 );
        
        Length = pRecvFIFOVc->Hdr.MTU;
                
        nicInitAllocateAddressIrb( pIrb,    
                                pAdapter,
                                0,
                                Length,
                                0,
                                ACCESS_FLAGS_TYPE_WRITE|ACCESS_FLAGS_TYPE_BROADCAST,
                                NOTIFY_FLAGS_AFTER_WRITE,
                                &AddressOffset,
                                pRecvFIFOVc);
            
        //
        // Allocate an Irp
        //

    
        NdisStatus = nicGetIrp (pAdapter->pNextDeviceObject, &pIrp);
                                                                                                                                                             
        if (NdisStatus != NDIS_STATUS_SUCCESS)
        {
            break;
        }

        ASSERT(pIrp != NULL);
    
        NdisStatus = nicSubmitIrp_LocalHostSynch(pAdapter,
                                               pIrp,
                                               pIrb );
                           

        //
        // Make this a synchronous call as this is during init
        //
        if (NdisStatus != NDIS_STATUS_SUCCESS)
        {
            TRACE( TL_A, TM_Recv, ( "nicAllocateAddressRange SUBMIT IRP FAILED NdisStatus %.8x", NdisStatus ) );

            break;
            
        }


        //
        // Check to see if the IoCallDriver succeeded
        //

        if(pIrp->IoStatus.Status == STATUS_SUCCESS)
        {
            NdisStatus = nicAllocateAddressRangeSucceeded (pIrb, pRecvFIFOVc);
            

            if (NdisStatus != NDIS_STATUS_SUCCESS)
            {
                break;
            }
        }
        else
        {
            ASSERT (pIrp->IoStatus.Status != STATUS_MORE_PROCESSING_REQUIRED);
            // else mark status as failure
            //
            // 
            //This means dereference will happen in this function
            NdisStatus = NDIS_STATUS_FAILURE;
            
        }
        //
        // we need to clean up the Irb and the Irp
        //

        
    } while (FALSE);

    //
    // Clean up -dereference the Call if things failed
    // If we successfully completed the Irp then all the references made above
    // will be dereferenced when the remote node goes away or the
    // Call is closed
    //
    // Deref the references that were made above. 
   
    VC_ACQUIRE_LOCK (pRecvFIFOVc);

    if (! NT_SUCCESS (NdisStatus))
    {
        if (bRefCall == TRUE)
        {
            nicDereferenceCall ( (PVCCB) pRecvFIFOVc , "nicAllocateAddressRange");

        }
        
    }
    
    VC_RELEASE_LOCK (pRecvFIFOVc);

    // We don't care about the status as we are just freeing locally allocated memory
    //
    if (pIrb != NULL)
    {
        nicFreeIrb (pIrb);
    }
    
    if (pIrp!= NULL)
    {
        nicFreeIrp (pIrp);
    }


    MATCH_IRQL

    TRACE( TL_T, TM_Recv, ( "<==nicAllocateAddressRange, pVc %.8x, Status %.8x ", pRecvFIFOVc, NdisStatus ) );

    return NdisStatus;

}
 



VOID
nicAllocateAddressRangeCallback( 
    IN PNOTIFICATION_INFO pNotificationInfo 
    )
/*++

Routine Description:
    This is the callback routine for the AllocateAddressRange that was done on a VC.
    We update statistics and then call the common Receive function.

Arguments:
    NotificationInfo - This structure contains the VC as context, the source of the packet
    and the length of the payload received by the ohci driver.

Return Value:


--*/
{
    PADAPTERCB pAdapter = (PADAPTERCB) pNotificationInfo->Context;
    PRECVFIFO_VCCB pRecvFIFOVc = pAdapter->pRecvFIFOVc;
    PNODE_ADDRESS pSenderNodeAddress = NULL;

    //
    // Debug spew for debugging
    //
    
    TRACE( TL_V, TM_Recv, ( "    Mdl is at %.8x",pNotificationInfo->Mdl ) );
       
    TRACE( TL_V, TM_Recv, ( "    ulLength is %.8x",pNotificationInfo->nLength) );
    
    TRACE( TL_V, TM_Recv, ( "    pNotificationInfo->RequestPacket %x, ", pNotificationInfo->RequestPacket) );

    TRACE( TL_V, TM_Recv, ( "    tLabel %x, ", ((PASYNC_PACKET)pNotificationInfo->RequestPacket)->AP_tLabel) );

    pSenderNodeAddress = & (((PASYNC_PACKET)pNotificationInfo->RequestPacket)->AP_Source_ID);

    TRACE( TL_V, TM_Recv, ( "    Senders' NodeAddress %x, ", pSenderNodeAddress->NA_Node_Number ) );

    TRACE (TL_V, TM_Reas,("tLabel %x    ", ((PASYNC_PACKET)pNotificationInfo->RequestPacket)->AP_tLabel));

    NdisInterlockedIncrement (&pRecvFIFOVc->NumIndicatedFifos);
    
    pNotificationInfo->Fifo->FifoList.Next = NULL;
    pNotificationInfo->Fifo->FifoMdl->Next = NULL;

    nicFifoAllocationScheme (pRecvFIFOVc);

    nicStatsRecordNumIndicatedFifos(pRecvFIFOVc->NumIndicatedFifos);

    nicReceiveCommonCallback (pNotificationInfo, 
                             (PVCCB)pRecvFIFOVc ,
                             AddressRange,
                             pNotificationInfo->Mdl );
}






NDIS_STATUS
nicAllocateAddressRangeSucceeded (
    IN PIRB pIrb,
    IN OUT PRECVFIFO_VCCB   pRecvFIFOVc
    )
/*++

Routine Description:

     This function updates all the Vc, PdoCb structures once the allocate address range Irb has succeeded
     If the Irp succeeds but the rempte node is going away then it will free the address range before
     returning
     The Irb is used to initialize the fields.  


Arguments:
     pIrb : The Irb that was used in the Irp that just succeeded
     pRecvFIFOVc: The RecvFifoVc that started the AllocateAddressRange


Return Value:
     Success: If the address returned is correct.

--*/    
{

    NDIS_STATUS             NdisStatus = NDIS_STATUS_FAILURE;
    BOOLEAN                 fPdoBeingRemoved = FALSE;
    NIC1394_FIFO_ADDRESS *pFifoAddress = NULL; 
    BOOLEAN                 fFirstAddressRangeOnVc = FALSE;

    //
    // These are pointers to the locations that the newly allocated address range needs to be copied to 
    //

    ADDRESS_RANGE           *pSrcAddressRange = &pIrb->u.AllocateAddressRange.p1394AddressRange[0];
    ADDRESS_RANGE           *pVcAddressRange = &pRecvFIFOVc->VcAddressRange;

    TRACE( TL_T, TM_Recv, ( "==>nicAllocateAddressRangeSucceeded pIrb %.8x, ", pIrb) );


    ASSERT (pIrb->u.AllocateAddressRange.AddressesReturned == 1);

    //
    // we expect this to be populated or 
    //
    ASSERT (pRecvFIFOVc != NULL);


    //
    // If both high and low are zero, the bus driver is doing something wrong, return  Failure
    //

    if (pSrcAddressRange->AR_Off_Low ==0 && pSrcAddressRange ->AR_Off_High == 0)
    {   
        // Some fun with DeMorgan's theorem
        ASSERT (pSrcAddressRange->AR_Off_Low!=0 || pSrcAddressRange ->AR_Off_High!=0);
        return NDIS_STATUS_FAILURE;
    }
    
    //
    // Copy the Address Ranges returned. For now just copy locally without allocating extra memory
    //

    pFifoAddress = &pRecvFIFOVc->Hdr.Nic1394MediaParams.Destination.FifoAddress;

    VC_ACQUIRE_LOCK (pRecvFIFOVc);

    do 
    {

        

        //
        // check to see if we need to update the Recv Fifo's structures. This needs to be done if the addresses are zeroes
        //
        if (pFifoAddress->Off_Low  == 0 && pFifoAddress->Off_High  == 0)
        {

            fFirstAddressRangeOnVc = TRUE;
            
            pFifoAddress->Off_Low  = pSrcAddressRange->AR_Off_Low;
            pFifoAddress->Off_High = pSrcAddressRange->AR_Off_High;


            pVcAddressRange->AR_Off_Low = pSrcAddressRange->AR_Off_Low;
            pVcAddressRange->AR_Off_High = pSrcAddressRange->AR_Off_High;
            pVcAddressRange->AR_Length = pSrcAddressRange->AR_Length;


        }
        else 
        {
            ASSERT (pFifoAddress->Off_Low == pSrcAddressRange->AR_Off_Low);
            ASSERT (pFifoAddress->Off_High == pSrcAddressRange->AR_Off_High);

        }

        pRecvFIFOVc->AddressesReturned = pIrb->u.AllocateAddressRange.AddressesReturned;
        pRecvFIFOVc->hAddressRange = pIrb->u.AllocateAddressRange.hAddressRange;

    
        //
        // If we reached this far, we have succeeded
        //
        NdisStatus = NDIS_STATUS_SUCCESS;   

    } while (FALSE);

    VC_RELEASE_LOCK (pRecvFIFOVc);
    TRACE( TL_T, TM_Recv, ( "   hAddressRange %x, NumReturned %x , Low %x , Hi %x, Length %x", 
                             pRecvFIFOVc->hAddressRange ,
                             pRecvFIFOVc->AddressesReturned,
                             pSrcAddressRange->AR_Off_Low,
                             pSrcAddressRange->AR_Off_High,
                             pSrcAddressRange->AR_Length) );




    TRACE( TL_T, TM_Recv, ( "<==nicAllocateAddressRangeSucceeded Status %.8x", NdisStatus ) );

    return NdisStatus;
}


VOID
nicFreeAddressFifo(
    IN PADDRESS_FIFO pAddressFifo,
    IN PRECVFIFO_VCCB pRecvFIFOVc 
    )
/*++

Routine Description:
    Takes a single AddressFifo element, frees it and dereferences the 
    VC on which it was allocated.

Arguments:
    pAddressFifo - The AddressFifo being freed.
    pRecvFIFOVc - VC on which the Address fifo was allocated.


Return Value:


--*/

{

    PVOID SystemAddress = NIC_GET_SYSTEM_ADDRESS_FOR_MDL (pAddressFifo->FifoMdl);
    UINT Length = MmGetMdlByteCount(pAddressFifo->FifoMdl);

    ASSERT (SystemAddress!=NULL);
    ASSERT (Length != 0);
    
    TRACE( TL_T, TM_Recv, ( "==>nicFreeAddressFifo") );

    if (SystemAddress != NULL)
    {
        nicFreeLocalBuffer(Length, SystemAddress);
    }
    
    nicFreeMdl (pAddressFifo->FifoMdl);

    FREE_NONPAGED((PVOID)pAddressFifo);

    //
    // Dereference the reference added when this AddressFifo was inserted into the list
    //

    nicDereferenceCall ((PVCCB)pRecvFIFOVc, "nicFreeAddressFifo");

    

    TRACE( TL_T, TM_Recv, ( "<==nicFreeAddressFifo") );

    return ;
}


VOID
nicFreeAllocateAddressRangeSList(
    IN PRECVFIFO_VCCB pRecvFIFOVc 
    )
/*++

Routine Description:
    
   This function should pop entries from the Slist
   Each entry is an Adress_fifo element containing an MDl
   The function should call nicFreeAddressFifo to free the Address FIFO element
     

Arguments:
    pRecvFIFOVc - RecvFIfoVc which has the list of Fifo Entries 
                  which need to be freed.

Return Value:


--*/
{

    PADDRESS_FIFO       pAddressFifoElement = NULL;
    SLIST_ENTRY   *pSingleListEntry = NULL;
    UINT                NumFreed = 0;

    STORE_CURRENT_IRQL;
    
    TRACE( TL_T, TM_Recv, ( "==>nicFreeAllocateAddressRangeSList, Num %.8x", 
                              ExQueryDepthSList (&pRecvFIFOVc->FifoSListHead) ) );

    while ( ExQueryDepthSList (&pRecvFIFOVc->FifoSListHead) != 0)
    {
        pSingleListEntry= ExInterlockedPopEntrySList ( &pRecvFIFOVc->FifoSListHead,
                                                  &pRecvFIFOVc->FifoSListSpinLock );

        //
        // This will dereference the call
        //
        pAddressFifoElement = CONTAINING_RECORD (pSingleListEntry, ADDRESS_FIFO, FifoList);

        ASSERT (pAddressFifoElement != NULL);

        //
        // This will dereference the Vc and free the address fifo
        //
        nicFreeAddressFifo ( pAddressFifoElement, 
                                          pRecvFIFOVc ); 
        NumFreed ++;                                          
    }

    VC_ACQUIRE_LOCK (pRecvFIFOVc);

    ASSERT ( ExQueryDepthSList (&pRecvFIFOVc->FifoSListHead) == 0);
    
    pRecvFIFOVc->FifoSListHead.Alignment = 0;
    pRecvFIFOVc->NumOfFifosInSlistInCloseCall = NumFreed;

    VC_RELEASE_LOCK (pRecvFIFOVc);


    MATCH_IRQL;

    TRACE( TL_T, TM_Recv, ( "<==nicFreeAllocateAddressRangeSList, NumFreed %x", NumFreed  ) );
    return ;

}






NDIS_STATUS
nicFillAllocateAddressRangeSList(
    PRECVFIFO_VCCB pRecvFIFOVc,
    UINT *Num )
/*++

Routine Description:
      Function inits the Slist that will be sent down with the 
      AllocateAddressRange Irb

      It is the responsibility of the caller to free the Allocated memory
    

Arguments:
      RecvFifoVc - VC to be linked with the Slist
      Num - Num  of AddressFifo Elements that are inserted into the SList

Return Value:

    Num  - Contains the number of Fifo elements that were inserted into the Slist
    Status - Success if all allocations succeeded.
    
--*/
    
     
{


    PADDRESS_FIFO pRecvFifoElement = NULL;
    NDIS_STATUS NdisStatus;
    UINT cnt = 0;
    BOOLEAN bRef = FALSE;

    TRACE( TL_T, TM_Recv, ( "==>nicFillAllocateAddressRangeSList" ) );

    ASSERT (pRecvFIFOVc != NULL);

    ASSERT (pRecvFIFOVc->Hdr.MTU != 0);


    do
    {
  
        NdisStatus = nicGetInitializedAddressFifoElement (pRecvFIFOVc->Hdr.MTU, 
                                                    &pRecvFifoElement);

        if (NdisStatus != NDIS_STATUS_SUCCESS)
        {
            break;
        }

        ASSERT (pRecvFifoElement  != NULL);
 
        ExInterlockedPushEntrySList ( &pRecvFIFOVc->FifoSListHead,
                                     (PSLIST_ENTRY)&pRecvFifoElement->FifoList,
                                     &pRecvFIFOVc->FifoSListSpinLock);

        //
        // Add this once for every Address Fifo element inserted 
        // Will be decremented by  a call to nicFreeAddressFifo
        //
        VC_ACQUIRE_LOCK (pRecvFIFOVc);

        bRef = nicReferenceCall ((PVCCB) pRecvFIFOVc, "nicFillAllocateAddressRangeSList");

        VC_RELEASE_LOCK (pRecvFIFOVc);
        
        if (bRef == FALSE)
        {
            NdisStatus = NDIS_STATUS_VC_NOT_ACTIVATED;
            break;
        }

        TRACE( TL_V, TM_Recv, ( "cnt %.8x, Num %.8x, ",cnt, *Num) );

    } while (++cnt < *Num);

    //
    // Need to handle failure cases and also return number allocated
    //
    *Num = cnt;

    
    if (NdisStatus != NDIS_STATUS_SUCCESS)
    {
        TRACE( TL_V, TM_Recv, ( "nicFillAllocateAddressRangeSList Failed, num allotted %.8x, MTU %,8x ",cnt ,pRecvFIFOVc->Hdr.MTU ) );

        nicFreeAllocateAddressRangeSList (pRecvFIFOVc);

        ASSERT (NdisStatus == NDIS_STATUS_SUCCESS);
    }

    TRACE( TL_T, TM_Recv, ( "==>nicFillAllocateAddressRangeSList Num %.8x, MTU %.8x",cnt,pRecvFIFOVc->Hdr.MTU ) );

    return NdisStatus;
}





NDIS_STATUS
nicGetInitializedAddressFifoElement(
    IN     UINT BufferLength, 
    IN OUT PADDRESS_FIFO *ppElement 
    )
/*++

Routine Description:
    
     This function return a single  AddressFifo element, 
     with an MDL pointing to locally owned allocated memory
     The size of the memory needs to be specified at MTU of
     the VC that this belongs to and is the BufferLength. 
    
     Get locally owned buffer, get address fifo , init MDL with
     local buffer. return the AddressFifo
    

Arguments:
    BufferLength - The length of the buffer that the Address_fifo contains, 
    *ppElement - output variable


Return Value:

    *ppElement - contains the allocated structure
    Status - On a failure, it contains the appropriate failure code.

--*/


{


    PVOID pLocalBuffer = NULL;
    NDIS_STATUS NdisStatus = NDIS_STATUS_FAILURE;

    TRACE( TL_T, TM_Recv, ( "==>nicGetAddressInitializedFifoElement" ) );
    
    ASSERT (BufferLength != 0);
    do
    {
        if (BufferLength == 0)
        {
            NdisStatus = NDIS_STATUS_FAILURE;
            TRACE( TL_A, TM_Recv, ( "BufferLength is 0" ) );

            break;
        }
        //
        // Get Locally owned memory for the data
        // 
        NdisStatus = nicGetLocalBuffer (BufferLength, &pLocalBuffer);
        
        if (NdisStatus != NDIS_STATUS_SUCCESS)
        {
            (*ppElement) = NULL;
            break;
        }

        //
        // Get Empty memory for the Address Fifo element
        //
        NdisStatus = nicGetEmptyAddressFifoElement (ppElement);

        if (NdisStatus != NDIS_STATUS_SUCCESS)
        {
            nicFreeLocalBuffer (BufferLength, 
                            pLocalBuffer);

            (*ppElement) = NULL;
            
            break;
        }

        //
        // Get an MDL and initialze the MDL with the buffer 
        // and initialize the fifo ,with MDL. 
        //
        NdisStatus = nicGetMdl ( BufferLength,
                   pLocalBuffer,
                   &((*ppElement)->FifoMdl));

        if (NdisStatus != NDIS_STATUS_SUCCESS || (*ppElement)->FifoMdl == NULL)
        {
            nicFreeLocalBuffer (BufferLength, 
                            pLocalBuffer);

            FREE_NONPAGED (*ppElement);

            (*ppElement) = NULL;

        }

    } while(FALSE);
    

    TRACE( TL_T, TM_Recv, ( "<==nicGetInitializedAddressFifoElement, Status %.8x, AddressFifo at %.8x, LocalBuffer at %.8x", 
                              NdisStatus, *ppElement,MmGetMdlVirtualAddress((*ppElement)->FifoMdl ) ) );
    
    return NdisStatus;

}


NDIS_STATUS
nicGetEmptyAddressFifoElement(
    IN PADDRESS_FIFO *ppElement
    )
/*++

Routine Description:

    Allocates and zeroes and empty Address_Fifo structure.
    
Arguments:
    ppElement - Output value

Return Value:
    On failure ppElement contains NULL and the appropriate Status 
    is returned
    

--*/
{


    NDIS_STATUS NdisStatus = NDIS_STATUS_FAILURE;
    ULONG Size = 0;
    TRACE( TL_T, TM_Recv, ( "==>nicGetEmptyAddressFifoElement" ) );

    Size = sizeof (ADDRESS_FIFO );
    
    *ppElement = ALLOC_NONPAGED (Size, MTAG_FIFO);

    if (*ppElement != NULL)
    {
        NdisZeroMemory (*ppElement, Size);
        
        NdisStatus = NDIS_STATUS_SUCCESS;
    }

    TRACE( TL_T, TM_Recv, ( "<==nicGetEmptyAddressFifoElement, Status % .8x, at %.8x",NdisStatus,*ppElement ) );
       

    return NdisStatus;


}



NDIS_STATUS
nicGetNdisBuffer(
    IN UINT Length,
    IN PVOID pLocalBuffer,
    IN OUT PNDIS_BUFFER *ppNdisBuffer 
    )
/*++

Routine Description:

    Given a buffer and Length, this function allocates an NdisBuffer (MDL) to point
    to that buffer

Arguments:
    Length - Length of the buffer,
    pLocalBuffer - pointer to the buffer,
    *ppNdisBuffer  output variable that contains the MDL


Return Value:
    Status - Appropriate Status code
    ppNDisBuffer - NDisBuffer if allocation succeeds.

--*/
{

    NDIS_STATUS NdisStatus = NDIS_STATUS_SUCCESS;
    ASSERT (pLocalBuffer != NULL);
    ASSERT (Length > 0);
    ASSERT (ppNdisBuffer != NULL);
    
    TRACE( TL_T, TM_Recv, ( "==>nicGetNdisBuffer Local Buffer %.8x, Length %.8x", pLocalBuffer, Length) );

    if ( Length > 0 &&
       pLocalBuffer != NULL &&
       ppNdisBuffer != NULL)
    {
        NdisAllocateBuffer( &NdisStatus,
                            ppNdisBuffer,
                            NULL,
                            pLocalBuffer,
                            Length );
    }
    else
    {
        nicIncrementMallocFailure();
        NdisStatus = NDIS_STATUS_FAILURE;
    }


    TRACE( TL_T, TM_Recv, ( "<==nicGetNdisBuffer  Buffer %x, NdisStatus %.8x", *ppNdisBuffer, NdisStatus  ) );
    return NdisStatus;

}





NDIS_STATUS
nicInitAllocateAddressIrb(
   IN PIRB                  pIrb,   
   IN PVOID                 pContext,   
   IN ULONG                 fulFlags,
   IN ULONG                 nLength,
   IN ULONG                 MaxSegmentSize,
   IN ULONG                 fulAccessType,
   IN ULONG                 fulNotificationOptions,
   IN PADDRESS_OFFSET       pOffset,
   IN PRECVFIFO_VCCB        pRecvFIFOVc
   )
/*++

Routine Description:
    
    Initializes the allocate adddress Irb with the 
    values passed to the function
    
    And adds constants for certain preknown values (e.g. callback, context)

    Spew as much debug as possible
    
   
Arguments:
    Are taken from the AllocateAddress Irb from 1394.h


Return Value:
    None

--*/
{

    NDIS_STATUS NdisStatus = NDIS_STATUS_SUCCESS;
    ASSERT (pRecvFIFOVc != NULL);

    TRACE( TL_T, TM_Recv, ( "==>nicInitAllocateAddressIrb" ) );

    pIrb->FunctionNumber = REQUEST_ALLOCATE_ADDRESS_RANGE;
    pIrb->Flags = 0;
    pIrb->u.AllocateAddressRange.Mdl = NULL;
    pIrb->u.AllocateAddressRange.fulFlags = 0;
    pIrb->u.AllocateAddressRange.nLength = nLength;
    pIrb->u.AllocateAddressRange.MaxSegmentSize = 0;
    pIrb->u.AllocateAddressRange.fulAccessType = fulAccessType;
    pIrb->u.AllocateAddressRange.fulNotificationOptions = NOTIFY_FLAGS_AFTER_WRITE;
    pIrb->u.AllocateAddressRange.Callback = nicAllocateAddressRangeCallback;
    pIrb->u.AllocateAddressRange.Context = pContext; // should be pAdapter

    
    pIrb->u.AllocateAddressRange.Required1394Offset.Off_High = pOffset->Off_High;
    pIrb->u.AllocateAddressRange.Required1394Offset.Off_Low = pOffset->Off_Low;

    pIrb->u.AllocateAddressRange.FifoSListHead = &pRecvFIFOVc->FifoSListHead;
    pIrb->u.AllocateAddressRange.FifoSpinLock = &pRecvFIFOVc->FifoSListSpinLock;
    
    pIrb->u.AllocateAddressRange.AddressesReturned = 0;
    pIrb->u.AllocateAddressRange.p1394AddressRange = &pRecvFIFOVc->VcAddressRange;

    



    TRACE(TL_V, TM_Recv, ("nLength = 0x%x\n", pIrb->u.AllocateAddressRange.nLength));
    TRACE(TL_V, TM_Recv, ("MaxSegmentSize = 0x%x\n", pIrb->u.AllocateAddressRange.MaxSegmentSize));
    TRACE(TL_V, TM_Recv, ("fulAccessType = 0x%x\n", pIrb->u.AllocateAddressRange.fulAccessType));
    TRACE(TL_V, TM_Recv, ("fulNotificationOptions = 0x%x\n", pIrb->u.AllocateAddressRange.fulNotificationOptions));
    TRACE(TL_V, TM_Recv, ("Callback = 0x%x\n", pIrb->u.AllocateAddressRange.Callback));
    TRACE(TL_V, TM_Recv, ("Context = 0x%x\n", pIrb->u.AllocateAddressRange.Context));
    TRACE(TL_V, TM_Recv, ("Required1394Offset->Off_High = 0x%x\n", pIrb->u.AllocateAddressRange.Required1394Offset.Off_High));
    TRACE(TL_V, TM_Recv, ("Required1394Offset->Off_Low = 0x%x\n", pIrb->u.AllocateAddressRange.Required1394Offset.Off_Low));
    TRACE(TL_V, TM_Recv, ("FifoSListHeader = 0x%x\n", pIrb->u.AllocateAddressRange.FifoSListHead));
    TRACE(TL_V, TM_Recv, ("FifoSListSpinLock = 0x%x\n", pIrb->u.AllocateAddressRange.FifoSpinLock));
    TRACE(TL_V, TM_Recv, ("AddressesReturned = 0x%x\n", pIrb->u.AllocateAddressRange.AddressesReturned));
    TRACE(TL_V, TM_Recv, ("p1394AddressRange = 0x%x\n", pIrb->u.AllocateAddressRange.p1394AddressRange));


    TRACE( TL_T, TM_Recv, ( "<==nicInitAllocateAddressIrb" ) );
       
    return NdisStatus;

}




VOID 
nicFifoReturnPacket (
    IN PVCCB pVc,
    IN PNDIS_PACKET pMyPacket
    )

/*++

Routine Description:
       For FIFO's, this will reinsert the buffer (MDL) into  the Fifo SList
       Checks to see if the VC is active and then return it ot the SList . 
       Free the FifoElement otherwise 

Arguments:
    pVc - VC on which the packet is being returned.
    pMyPacket - that contains the packet that contains the Fifo list


Return Value:


--*/
{
    PRECVFIFO_VCCB          pRecvFIFOVc  = (PRECVFIFO_VCCB) pVc; 
    PNDIS_BUFFER            pMyNdisBuffer;
    PADAPTERCB              pAdapter = pRecvFIFOVc->Hdr.pAF->pAdapter;
    BOOLEAN                 fVcActive = FALSE;
    PADDRESS_FIFO           pAddressFifo;
    PPKT_CONTEXT            pPktContext = (PPKT_CONTEXT)&pMyPacket->MiniportReserved;   
    
    
    TRACE( TL_T, TM_Recv, ( "==>nicFifoReturnPacket pVc %x, pPacket %x, pAdapter %x, ", 
                             pRecvFIFOVc, pMyPacket, pAdapter) );


    //
    // Either the reassembly structure has the indicated Fifo's or if no reassembly was done
    // then the PktContext has it.
    //
    pAddressFifo = pPktContext->AllocateAddressRange.pIndicatedFifo;    

    // 
    // Do not push it back in the list if the VC is about to close. 
    // However, we push it back in, if the VC has not been activated yet
    //

    
    nicReturnFifoChain ( pAddressFifo , pRecvFIFOVc) ;
    

    //
    // Now we have to free the packet and ndis buffers that we got in the 
    // Calback code
    //
    TRACE( TL_V, TM_Recv, ( " AllocateAddress Range - Free Packet and Free Buffer" ) );

    nicReturnNdisBufferChain  (pMyPacket->Private.Head, pVc);

    nicFreePacket(pMyPacket, &pRecvFIFOVc->PacketPool);
    

    TRACE( TL_T, TM_Recv, ( "<==nicFifoReturnPacket "  ) );


    return;

}

VOID
nicReturnNdisBufferChain (
    IN PNDIS_BUFFER pNdisBuffer ,
    IN PVCCB pVc
    )
/*++

Routine Description:

    This functions frees a list of NDIS Buffers
    
Arguments:

    pNdisBuffer - NdisBufferChain
    VC  - not used except for statistics.

Return Value:


--*/
{
    PNDIS_BUFFER pNext;
    BOOLEAN fIsFifo = (pVc->Hdr.VcType == NIC1394_RecvFIFO);

    if (pNdisBuffer == NULL)
    {
        ASSERT (pNdisBuffer != NULL);
        return;
    }


    while (pNdisBuffer != NULL)
    {
        pNext = pNdisBuffer->Next;

        NdisFreeBuffer(pNdisBuffer);

        nicDecRecvBuffer(fIsFifo);
        pNdisBuffer = pNext;
    } 

}









VOID
nicReturnFifoChain (
    IN PADDRESS_FIFO pAddressFifo,
    IN PRECVFIFO_VCCB pRecvFIFOVc
    )
/*++

Routine Description:
    This takes a chain of Address Fifos and returns it to the slist if 
    the VC is active or frees the AddressFifo if the VC is not active

Arguments:
    pAddressFifo - Address Fifo that needs to be returned,
    pRecvFIFOVc - The VC which owns the AddressFifo


Return Value:

    None
    
--*/
{
    
    TRACE( TL_T, TM_Recv, ( "==> nicReturnFifoChain pAddressFifo %x, pRecvFifoVc %x", pAddressFifo, pRecvFIFOVc) );


    VC_ACQUIRE_LOCK (pRecvFIFOVc);

    //
    // lets update the value again, before we insert the Address Fifo back in to the Slist
    // If there are any remote nodes present and the VC is active 
    // , then we should insert this back into the SList
    //
    

    if  ( VC_ACTIVE (pRecvFIFOVc) == TRUE )
    {     

        //
        // Return all the AddressFifo elements to the slist
        // Do this with the lock held so no one can change the
        // VC state from under us
        //

            
        while (pAddressFifo != NULL)
        {
            PADDRESS_FIFO pNextFifo = (PADDRESS_FIFO)(pAddressFifo->FifoList.Next);

            ExInterlockedPushEntrySList ( &pRecvFIFOVc->FifoSListHead,
                                          (PSLIST_ENTRY)&pAddressFifo ->FifoList,
                                          &pRecvFIFOVc->FifoSListSpinLock);
        
            TRACE( TL_V, TM_Recv, ( "  VcActive Address Fifo %x, Next Fifo %x",pAddressFifo , pNextFifo) );

            pAddressFifo = pNextFifo;

            NdisInterlockedDecrement (&pRecvFIFOVc->NumIndicatedFifos);


        }

        VC_RELEASE_LOCK (pRecvFIFOVc);

    }
    else  //VC_ACTIVE (pRecvFIFOVc) == TRUE 
    {

        VC_RELEASE_LOCK (pRecvFIFOVc);
        //
        // free all the Address Fifo after releasing the lock
        //
            
        while (pAddressFifo != NULL)
        {
            PADDRESS_FIFO pNextFifo = (PADDRESS_FIFO)(pAddressFifo->FifoList.Next);

            // 
            // Free the Mdl and Address Fifo structure and decrease the refcount
            // on the call. Do not touch the Vc after this
            //

            TRACE( TL_V, TM_Recv, ( "  Vc NOT Active  Address Fifo %x, Next Fifo %x",pAddressFifo , pNextFifo) );

            nicFreeAddressFifo(pAddressFifo ,
                               pRecvFIFOVc);
        
            NdisInterlockedDecrement (&pRecvFIFOVc->NumIndicatedFifos);

            pAddressFifo = pNextFifo;

        }


    }

    TRACE( TL_T, TM_Recv, ( "<== nicReturnFifoChain ") );

    return; 
}




VOID
nicInternalReturnPacket(
    IN  PVCCB                   pVc ,
    IN  PNDIS_PACKET            pPacket
    )
/*++

Routine Description:
    Finds out what type of Vc is being indicated and calls the appropriate VC return packets handler

Arguments:
    MiniportAdapterContext  - the pAdapter structure,
    pPacket - pPacket that the protocol returns


Return Value:


--*/
{
    PPKT_CONTEXT    pPktContext = (PPKT_CONTEXT)&pPacket->MiniportReserved;


    switch (pVc->Hdr.VcType)
    {
        case NIC1394_SendRecvChannel:
        case NIC1394_RecvChannel:
        {
            nicChannelReturnPacket (pVc, pPacket );
            break;
        }
        case NIC1394_RecvFIFO:
        {
    
            nicFifoReturnPacket ( pVc, pPacket);        
            break;
        }

        
        default :
        {
            
            ASSERT (0);
            break;
        }



    }
    

    return;


}

VOID
NicReturnPacket(
    IN  NDIS_HANDLE             MiniportAdapterContext,
    IN  PNDIS_PACKET            pPacket
    )
/*++

Routine Description:
    This is the return packets handler. 
    This functikon handles all the instrumentation to catch outstanding packets and 
    then calls the internal return packets function

Arguments:
    MiniportAdapterContext  - the pAdapter structure,
    pPacket - pPacket that the protocol returns


Return Value:


--*/
{

    PADAPTERCB      pAdapter = (PADAPTERCB) MiniportAdapterContext;
    PPKT_CONTEXT    pPktContext = (PPKT_CONTEXT)&pPacket->MiniportReserved;
    PINDICATE_RSVD  pIndicateRsvd  = NULL;
    PRSVD           pRsvd = NULL;

    //
    // The first parameter of the MiniportReserved will always contain the VC
    //

    PVCCB pVc = (PVCCB)pPktContext->AllocateAddressRange.pRecvFIFOVc;


    TRACE( TL_T, TM_Recv, ( "==> NicReturnPacket pPacket %x ", pPacket) );

    do
    {

        //
        // Mark the packet as returned
        //
        pRsvd =(PRSVD)(pPacket->ProtocolReserved);
        pIndicateRsvd = &pRsvd->IndicateRsvd;

        ASSERT (pIndicateRsvd->Tag == NIC1394_TAG_INDICATED);
        
        pIndicateRsvd->Tag =  NIC1394_TAG_RETURNED;

        nicInternalReturnPacket (pVc, pPacket);

    }while (FALSE);
    
    TRACE( TL_T, TM_Recv, ( " <== NicReturnPacket ") );

    return;
}



NDIS_STATUS
nicFindReassemblyStructure (
    IN PREMOTE_NODE pRemoteNode,
    IN USHORT Dgl,
    IN BUS_OPERATION BusOp,
    IN PVCCB pVc,
    OUT PNDIS1394_REASSEMBLY_STRUCTURE* ppReassembly
    )
/*++

Routine Description:

      Walk through all the reassembly operations on this remote node
      and see if one is present 
    
      If no reassembly is found, it will allocate and initialie a structure. 
      All within the context of the reassembly  lock  
    
     Arguments
     Return Value:
    
    

Arguments:

      pRemoteNode - Remote Node that is sending the fragments
      dgl  - identifier for reassembly packet

      Together they are unique for each reassembly operation

      BusOp - Isoch or Fifo
      pVc - on which the fragment has been indicated.
      ppReassembly - Output variable that is filled if the Reassembly is found.


Return Value:

    ppReassembly - Contains the allocated/found structure
    Status - appropriate failure code on failure.

--*/
{

    PNDIS1394_REASSEMBLY_STRUCTURE      pTempReassembly = NULL;
    PNDIS1394_REASSEMBLY_STRUCTURE      pReassembly = NULL;
    PLIST_ENTRY                         pReassemblyList = NULL;
    NDIS_STATUS                         NdisStatus = NDIS_STATUS_FAILURE;


    TRACE( TL_T, TM_Recv, ( "==>nicFindReassemblyStructure  pRemoteNode %x, dgl %x " , pRemoteNode , Dgl) );

    //
    // Acquire the reassebly lock . Only let go when either a reassembly structure is found or a new 
    // reassembly structure is inserted into the remote node's reassembly list
    //
    REMOTE_NODE_ACQUIRE_LOCK (pRemoteNode);
    REMOTE_NODE_REASSEMBLY_ACQUIRE_LOCK (pRemoteNode)

    pReassemblyList = pRemoteNode->ReassemblyList.Flink;

    //
    // Find the reassembly with the same dgl 
    // 
    
    while ( pReassemblyList != &pRemoteNode->ReassemblyList)
    {   
        pTempReassembly = CONTAINING_RECORD (pReassemblyList ,
                                               NDIS1394_REASSEMBLY_STRUCTURE,
                                               ReassemblyListEntry );
                                               
    
        TRACE( TL_V, TM_Recv, ( "Current Dgl %x, dgl %x " , pTempReassembly->Dgl , Dgl) );

        if (pTempReassembly->Dgl == Dgl)
        {
            pReassembly = pTempReassembly;
            break;
        }

        pReassemblyList  = pReassemblyList->Flink;
    }

    

    do 
    {
        //
        // If we have found a valid reassembly then return
        //

        if (pReassembly != NULL )
        {
            *ppReassembly = pReassembly ;

            NdisStatus = NDIS_STATUS_SUCCESS;
        }
        else
        {   
            //
            // If the number of outstanding reassemblies is excessive, do not 
            // allocate a new Reassembly strucutre. Drop the packet.
            //
            PADAPTERCB pAdapter = pRemoteNode->pAdapter;
            
            if (pAdapter->OutstandingReassemblies > NIC1394_MAX_REASSEMBLY_THRESHOLD)
            {
                NdisStatus = NDIS_STATUS_FAILURE;
                break;

            }
            
            //
            // We need to allocate and initialize  a reassembly structure
            // 
            NdisStatus = nicGetReassemblyStructure (&pReassembly);
            

            if (NdisStatus != NDIS_STATUS_SUCCESS)
            {

                BREAK (TM_Recv, ("    nicGetReassemblyStructure nicGetReassemblyStructure FAILED") ); 
            }

            NdisStatus = nicInitializeReassemblyStructure (pReassembly, 
                                                       Dgl, 
                                                       pRemoteNode, 
                                                       pVc, 
                                                       BusOp);

            if (NdisStatus != NDIS_STATUS_SUCCESS)
            {
            
                pReassembly = NULL;
                BREAK (TM_Recv, ("    nicFindReassemblyStructure nicInitializeReassemblyStructure FAILED" ) );
            }

        }
        
    } while (FALSE);

    if (NdisStatus == NDIS_STATUS_SUCCESS)
    {
        //
        // Increment the ref count. Ref Count will be freed when the fragment is inserted into 
        // the reassembly structure or the packet indicated up
        //
        nicReferenceReassembly ( pReassembly, "nicFindReassemblyStructure " );
        
    }
    
    REMOTE_NODE_REASSEMBLY_RELEASE_LOCK (pRemoteNode)
    REMOTE_NODE_RELEASE_LOCK (pRemoteNode);

    if (NdisStatus == NDIS_STATUS_SUCCESS)
    {
        //
        // Update output parameters
        //
        *ppReassembly = pReassembly;

    }
    TRACE( TL_T, TM_Recv, ( "<==nicFindReassemblyStructure NdisStatus %x, *ppReassembly %x" , NdisStatus ,*ppReassembly ) );

    return NdisStatus ;
}



NDIS_STATUS
nicGetReassemblyStructure ( 
    IN OUT PNDIS1394_REASSEMBLY_STRUCTURE* ppReassembly
    )
/*++

Routine Description:
      Just allocates a structure and returns
    
     Arguments
      
    
    
     Return Value:
      Success - if succeeded 
    
     Called with the lock held

Arguments:

      ppReassembly - to point to the newly allocated structure

Return Value:

      Success - if succeeded 
    
     Called with the lock held


--*/
{
    NDIS_STATUS NdisStatus = NDIS_STATUS_FAILURE;
    TRACE( TL_T, TM_Recv, ( "==>nicGetReassemblyStructure  ppReassembly %x", ppReassembly ) );

    *ppReassembly = ALLOC_NONPAGED (sizeof (NDIS1394_REASSEMBLY_STRUCTURE), MTAG_REASSEMBLY);

    if (*ppReassembly == NULL)
    {
        nicIncrementMallocFailure();
        NdisStatus = NDIS_STATUS_FAILURE;

    }
    else
    {
        NdisZeroMemory (*ppReassembly, sizeof (NDIS1394_REASSEMBLY_STRUCTURE) );
        NdisStatus = NDIS_STATUS_SUCCESS;
        (*ppReassembly)->Tag = MTAG_REASSEMBLY;
        ReassemblyAllocated++;  
    }

    

    TRACE( TL_T, TM_Recv, ( " <==nicGetReassemblyStructure  NdisStatus %x, pReassembly %x", NdisStatus, *ppReassembly) );
    return NdisStatus;
}



VOID
nicFreeReassemblyStructure ( 
    IN PNDIS1394_REASSEMBLY_STRUCTURE pReassembly
    )

/*++

Routine Description:
    Just Frees the structure and returns

Arguments:
    ppReassembly - to point to the newly allocated structure


Return Value:
    None    

--*/
{
    TRACE( TL_T, TM_Recv, ( "== nicFreeReassemblyStructure  ppReassembly %x", pReassembly ) );

    pReassembly->Tag = MTAG_FREED;

    NdisInterlockedDecrement (&ReassemblyAllocated);
    
    nicDereferenceReassembly (pReassembly, "nicFreeReassemblyStructure ");

    return;
}


NDIS_STATUS
nicInitializeReassemblyStructure (
    IN PNDIS1394_REASSEMBLY_STRUCTURE pReassembly,
    IN USHORT Dgl,
    IN PREMOTE_NODE pRemoteNode,
    IN PVCCB pVc,
    IN BUS_OPERATION ReceiveOp
    )
/*++

Routine Description:
    Goes in and assigns values to all the fields in the structure

Arguments:
      pReassembly = pReassembly structure all zeroed out,
      Dgl,- Datagram Label to be used in reassembly 
      pRemoteNode - pRemoteNode pointing to the sender
      ReceiveOp - ISoch or Fifo
      Vc - On which the packet came in.
    

Return Value:

    Success : - If remote node active and this has been inserted into the remote node's list
    Failure - If remote Node is not active   
    Called with the lock held
    
--*/
{
    BOOLEAN fRemoteNodeActive = FALSE;
    NDIS_STATUS NdisStatus = NDIS_STATUS_FAILURE;
    PADAPTERCB pAdapter = pVc->Hdr.pAF->pAdapter;
    
    TRACE( TL_T, TM_Recv, ( "==> nicInitializeReassemblyStructure pReassembly %x, ReceiveOp %x", pReassembly, ReceiveOp ) );

    TRACE( TL_T, TM_Recv, ( "     pRemoteNode %x, Dgl %x, pVc %x ", pReassembly, Dgl, pVc ) );

    //
    // Increment the reassembly count
    //
    nicReassemblyStarted(pAdapter);
    pAdapter->AdaptStats.TempStats.ulMaxOutstandingReassemblies = 
                    max(pAdapter->AdaptStats.TempStats.ulMaxOutstandingReassemblies,
                         pAdapter->AdaptStats.TempStats.ulNumOutstandingReassemblies);

    
    //
    // Dgl  - Datagram label. Unique for every reassembly structure gernerated by this local host
    //
    pReassembly->Dgl = Dgl;


    //
    // pRemoteNode  -> RemoteNode + Dgl are unique for each reassembly structure
    //
    pReassembly->pRemoteNode = pRemoteNode;
    
    
    //
    // ExpectedFragmentOffset is computed by the Last Fragment's Offset + 
    // length of fragment. Does not account for gaps in the reassembled packet.
    // 
    pReassembly->ExpectedFragmentOffset = 0;



    //
    // LastNdisBuffer that was appended to the packet 
    //
    pReassembly->pTailNdisBuffer = NULL;

    //
    // Packet that is being reassembled
    //
    pReassembly->pNdisPacket = NULL;


    pReassembly->Head.pAddressFifo = NULL;
    pReassembly->Tail.pAddressFifo = NULL;
    pReassembly->ReceiveOp = ReceiveOp;
    pReassembly->pVc = pVc;
    

    //
    // Reference the remote node. This will be derefernced when the packet is returned
    //

    fRemoteNodeActive = (REMOTE_NODE_ACTIVE (pRemoteNode));

    TRACE( TL_V, TM_Recv, ( "  nicInitializeReassemblyStructure fRemoteNodeActive %x", fRemoteNodeActive) );


    if (fRemoteNodeActive  == TRUE) 
    {
        //
        // REfcount made as the reassembly will happen on the remote node. 
        // REfcount released when the last fragment is complete
        //
        nicReferenceRemoteNode (pRemoteNode, InitializeReassemblyStructure);

        InsertTailList(&pRemoteNode->ReassemblyList, &pReassembly->ReassemblyListEntry);

        //
        // Reerence REassembly . Ref removed when this is removed from the Remote node list
        //
        nicReferenceReassembly (pReassembly, "nicInitializeReassembly" );
    }
    

    if (fRemoteNodeActive  == FALSE)
    {
        //
        // Temporary assert 
        //

        FREE_NONPAGED (pReassembly);
        
        NdisStatus = NDIS_STATUS_FAILURE;
    }
    else
    {
        NdisStatus = NDIS_STATUS_SUCCESS;
    }

    //
    // reference the reassembly for its creation. Dereferenced in the Indicate Packet Code path
    //
    nicReferenceReassembly (pReassembly, " nicInitializeReassemblyStructure ");

    TRACE( TL_T, TM_Recv, ( "<== nicInitializeReassemblyStructure NdisStatus %x, pReassembly%x ", NdisStatus,pReassembly ) );

    return NdisStatus;
}


VOID
nicAbortReassembly (
    IN PNDIS1394_REASSEMBLY_STRUCTURE pReassembly
    )
/*++

Routine Description:

    This thread can get called in one of two cases. 1) If the Remote node is going away
    and 2) if the Reassembly has timed out. In the former case, we have Remote Node lock and 
    that will stop any thread from accessing this list. In the latter case, the Reassembly struct
    has been removed from the Remote Node's Reassembly List while the RemoteNode lock was held. 
    This Reassembly structure now cannot be accessed by the Reassembly code or the RemoveRemoteNode code,
    no lock is held in the second case.
    
    This function will free all allocated NdisBuffers and return all AddressFifo 
    elements to the bus driver (or frees them if the VC is closing down).
    
Arguments

    pReasssembly - Reassembly structure that needs to be freed
        
Return Value:
      None

--*/
{

    PNDIS_BUFFER pNdisBuffer = NULL;
    PRECVFIFO_VCCB pRecvFIFOVc = NULL;
    PCHANNEL_VCCB pChannelVc  = NULL;
    PADAPTERCB pAdapter = pReassembly->pVc->Hdr.pAF->pAdapter;
    STORE_CURRENT_IRQL;

    
    
    TRACE( TL_T, TM_Recv, ( "==> nicAbortReassembly  pReassembly %x", pReassembly ) );

    
    //
    // Free all the ndis buffers and so forth
    //
    if (pReassembly != NULL)
    {   
        //
        // First Chain the reassembly array into a linked so our return functions can deal with it
        //
        nicChainReassembly (pReassembly);

        if (pReassembly->pHeadNdisBuffer != NULL)
        {
            nicReturnNdisBufferChain(pReassembly->pHeadNdisBuffer, pReassembly->pVc);         
        }

        switch (pReassembly->ReceiveOp)
        {
            case AddressRange:
            {
                pRecvFIFOVc = (PRECVFIFO_VCCB) pReassembly->pVc;


                //
                // Time to return all of our address fifos
                //
                nicReturnFifoChain (pReassembly->Head.pAddressFifo,
                                    pRecvFIFOVc
                                    );
                            
                pReassembly->Head.pAddressFifo = NULL;
                break;
            }

            case IsochReceive:
            {
                pChannelVc = (PCHANNEL_VCCB)pReassembly->pVc;

                nicReturnDescriptorChain ( pReassembly->Head.pIsochDescriptor,
                                       pChannelVc);
                                       
                pReassembly->Head.pIsochDescriptor = NULL;
                break;
            }


            default:
            {

                ASSERT (0);
            }


        }


    }
    else
    {
        ASSERT (0);
    }

    //
    // Now deref the reassembly and free it.
    //
    nicReassemblyAborted (pAdapter);
    nicFreeReassemblyStructure (pReassembly);

    TRACE( TL_T, TM_Recv, ( "<== nicAbortReassembly  pReassembly %x", pReassembly ) );
    MATCH_IRQL;
    return;
}



NDIS_STATUS
nicDoReassembly ( 
    IN PNIC_RECV_DATA_INFO pRcvInfo,
    OUT PNDIS1394_REASSEMBLY_STRUCTURE *ppReassembly,
    PBOOLEAN pfReassemblyComplete
    )
/*++

Routine Description:
       Does the reassembly work . 
       Allocates an ndisbuffer pointing to the data .
       Does In order or out of order reassembly
        
Arguments:
        pRcvInfo  - pRcv Information
        pReassembly reassmbly structure associated with this fragment
        pfReassemblyComplete - Is the REassembly complete
Return Value:
        Success - if this fragment was successfully associated with a reassembly structure

--*/
{
    NDIS_STATUS                                 NdisStatus = NDIS_STATUS_FAILURE;
    PNDIS_BUFFER                                pNdisBuffer = NULL;
    PNDIS1394_REASSEMBLY_STRUCTURE              pReassembly = NULL;
    BOOLEAN                                     fInOrder = FALSE;
    BOOLEAN                                     fNeedToReleaseReassemblyLock = FALSE;
    BOOLEAN                                     fReassemblyComplete = FALSE;
    PADAPTERCB                                  pAdapter = pRcvInfo->pRemoteNode->pAdapter;

    STORE_CURRENT_IRQL;
    
    TRACE( TL_T, TM_Recv, ( "==> nicDoReassembly ppReassembly %x pRcvInfo %x",
                           ppReassembly, pRcvInfo ) );

    
    do
    {
        //
        // Get an NdisBuffer pointing to the data 
        //
        NdisStatus = nicGetNdisBufferForReassembly( pRcvInfo, &pNdisBuffer);

        if (NdisStatus != NDIS_STATUS_SUCCESS)
        {
            //
            // If we break from here, the reassmbly will never get completed and the 
            // garbage collector will eventually free this.
            //
            pNdisBuffer = NULL;
            BREAK (TM_Send, ("nicDoReassembly  nicGetNdisBufferForReassembly FAILED" ) );

        }

                
        //
        // Either there is a reassembly currently going or one will be allocated and initialized
        //
        
        NdisStatus = nicFindReassemblyStructure (pRcvInfo->pRemoteNode, 
                                            pRcvInfo->Dgl, 
                                            pRcvInfo->RecvOp, 
                                            (PVCCB)pRcvInfo->pVc,
                                            &pReassembly);

        if (NdisStatus != NDIS_STATUS_SUCCESS)
        {
            pReassembly=NULL;
            BREAK (TM_Recv, ("    nicDoReassembly  nicFindReassemblyStructure FAILED")); 
        }
    

        //
        // Now we start doing the actual work . Acquire the 
        // reassembly lock so no one else can touch the reassembly
        //
        
        
        ASSERT (pReassembly != NULL);
        TRACE( TL_V, TM_Recv, ( " ExpectedFragmentOffset  %x FragmentHeader Offset %x, ",
                               pReassembly->ExpectedFragmentOffset , pRcvInfo->FragmentOffset) );

        //
        // Code expects that if the reassembly is not Null, then the lock is acquired. 
        //
        REASSEMBLY_ACQUIRE_LOCK (pReassembly);
        fNeedToReleaseReassemblyLock = TRUE;

        
        if (REASSEMBLY_ACTIVE (pReassembly) == FALSE)
        {
            //
            // Drop the reassembly, as this structure is about to be freed
            // 
            NdisStatus = NDIS_STATUS_FAILURE;
            break;

        }


        //
        // This is the new reassembly scheme, which uses a table and does out of order and inorder
        // reassembly
        //
        

        NdisStatus = nicInsertFragmentInReassembly (pReassembly,
                                              pRcvInfo);
                                      

        if (NdisStatus != NDIS_STATUS_SUCCESS)
        {
            //
            // Do not assert
            //
            TRACE (TL_V,  TM_Reas, ("nicDoReassembly nicInsertFragmentInReassembly  FAILED") );
            break;
        }


        fReassemblyComplete = pReassembly->fReassemblyComplete;


        
    } while (FALSE);

    //
    // Release the reassembly lock (if acquired)
    //
    if (fNeedToReleaseReassemblyLock == TRUE)
    {
        REASSEMBLY_RELEASE_LOCK (pReassembly);

        if (fReassemblyComplete  == TRUE)
        {
            //
            // Dereference the remote node as we are removing the reassembly from the remote node
            //
            nicDereferenceReassembly (pReassembly, "nicInsertFragmentInReassembly " );
            //
            // now dereference the remote node. ref was added when the reassembly was
            // inserted into the remote node's list
            //
            nicDereferenceRemoteNode(pReassembly->pRemoteNode, InsertFragmentInReassembly );   

            pReassembly->pRemoteNode = NULL;

        }
    }
    //
    // Clean up time. First handle the failure case. 
    // If reassembly is != NULL, then free the lock 
    // and free the reassembly structure
    //

    if (NdisStatus == NDIS_STATUS_SUCCESS)
    {
        *ppReassembly = pReassembly; 

        //
        // Queue a Reassembly timer, if reassembly was not complete
        //
        if (fReassemblyComplete == FALSE)
        {   
            nicQueueReassemblyTimer(pAdapter, FALSE);
        }
    }   

    if (NdisStatus != NDIS_STATUS_SUCCESS )
    {
        //
        // Free any locally allocated structures.
        //
        if (pNdisBuffer)
        {
            NdisFreeBuffer (pNdisBuffer);
        }
        //
        // Return NULL as output. The Reassembly structure is  
        // in the remote node's list. The timer routine will pick it up
        //

        //
        // Deref the ref made the REassembly was found/
        //
        if (pReassembly != NULL)
        {
             nicDereferenceReassembly (pReassembly, "nicDoReassembly - failure" );
        }
        
        *ppReassembly = pReassembly = NULL;

    }

    *pfReassemblyComplete = fReassemblyComplete;


    TRACE( TL_T, TM_Recv, ( "<== nicDoReassembly NdisStatus %x, , pReassembly %x, Complete %x", NdisStatus, *ppReassembly, *pfReassemblyComplete  ) );
    MATCH_IRQL;
    
    return NdisStatus;  

}





NDIS_STATUS
nicGetNdisBufferForReassembly(
    IN PNIC_RECV_DATA_INFO pRcvInfo,
    OUT PNDIS_BUFFER *ppNdisBuffer
    )
/*++

Routine Description:
Function Description:
    This function gets an Ndis Buffer that points to the start of the data
    that the Mdl points to. The Data starts from the point after the 
    Fragmentation Header
    
    If this is the First fragment, then 32 bytes of the fragment header are also
    copied to make room for the header that the ARP  module expects

Arguments
       pRcvInfo - Pointer to the Receive Tracking structure
       ppNdisBuffer - Output
    
Return Value:
      Success  - if the mem alloc succeeded, appropriate failure code otherwise
      NdisBuffer - Buffer pointing ot the data ,
    

--*/
{
    NDIS_STATUS                 NdisStatus = NDIS_STATUS_FAILURE;
    PVOID                       pStartValidData = NULL;
    ULONG                       ulValidDataLength = 0;
    PNDIS1394_FRAGMENT_HEADER   pNonByteSwappedFragmentHeader  = NULL;
    USHORT                      Dgl;
    PNDIS_BUFFER                pNdisBuffer = NULL;
    ULONG                       IsochPrefix = ISOCH_PREFIX_LENGTH;
    PPACKET_FORMAT              pIndicatedData = NULL;

    TRACE( TL_T, TM_Recv, ( "==> nicGetNdisBufferForReassembly ") );
    do
    {
        //
        // Get a pointer to the start of the data, ie. it should point past the encapsulation header
        //
        pStartValidData = (PVOID)((ULONG_PTR)pRcvInfo->pEncapHeader + sizeof(NDIS1394_FRAGMENT_HEADER));

        ulValidDataLength  = pRcvInfo->DataLength - sizeof (NDIS1394_FRAGMENT_HEADER);
        //
        // if this is the first fragment, then leave room for the Unfragmented header that will need 
        // to be added before sending it up to the IP module
        // 
        if (pRcvInfo->fFirstFragment == TRUE)
        {
            ULONG ExtraData = (sizeof(NDIS1394_FRAGMENT_HEADER) - sizeof (NDIS1394_UNFRAGMENTED_HEADER)) ;

            pStartValidData = (PVOID)((ULONG_PTR)pStartValidData - ExtraData);

            ulValidDataLength  += ExtraData ;
        }
        
        
        NdisStatus = nicGetNdisBuffer ( ulValidDataLength,   
                                   pStartValidData,
                                   &pNdisBuffer);

        if (NdisStatus != NDIS_STATUS_SUCCESS)
        {
            BREAK (TM_Recv, ( "nicGetNdisBufferForReassembly: nicGetNdisBuffer Failed" ) );
        }

        nicIncRecvBuffer(pRcvInfo->pVc->Hdr.VcType == NIC1394_RecvFIFO);

        *ppNdisBuffer = pNdisBuffer;
        pRcvInfo->pNdisBuffer = pNdisBuffer;
        pRcvInfo->pNdisBufferData = pStartValidData;
        
    }while (FALSE);



    TRACE( TL_T, TM_Recv, ( "<== nicGetNdisBufferForReassembly NdisStatus %x, *ppNdisbuffer %x, pStartValidData%x ,ulValidDataLength %x", 
                             NdisStatus, *ppNdisBuffer, pStartValidData, ulValidDataLength) );



    return NdisStatus;

}


VOID
nicAddUnfragmentedHeader (
    IN PNDIS1394_REASSEMBLY_STRUCTURE pReassembly,
    IN PVOID pEncapHeader
    )
/*++

Routine Description:
   Its purpose is to add the fragment header that arp expects.
   There is room in the Head NdisBuffer to do this
     
   We own the buffer, so we can manipulate the data
    

Arguments:
    pReassembty Structure - contains all the necessary reasembly info
    pEncapHeader - Pointer to where th Unfragmented header will be stored

Return Value:
    None

--*/


{
    
    PNDIS1394_UNFRAGMENTED_HEADER pHeader = NULL;
    ASSERT (sizeof(NDIS1394_UNFRAGMENTED_HEADER) == sizeof(ULONG));
    TRACE( TL_T, TM_Recv, ( "==> nicAddUnfragmentedHeader  %x, pEncapHeader %x", pReassembly, pEncapHeader) );


    pHeader = (PNDIS1394_UNFRAGMENTED_HEADER) pEncapHeader;

    //
    // Now we add the unfragmented header. first zero it, then add the approriate values
    //
    pHeader->HeaderUlong = 0;   
    pHeader->u.FH_lf = lf_Unfragmented;
    pHeader->u.FH_EtherType = pReassembly->EtherType;

    //
    // Convert the header  to network order and indicate it up.
    //
    pHeader->HeaderUlong = SWAPBYTES_ULONG (pHeader->HeaderUlong);

    
    
    TRACE( TL_T, TM_Recv, ( "<== nicAddUnfragmentedHeader  pReasembly %x, pHeader %x ", pReassembly, pHeader->HeaderUlong) );
    
    return;
}





VOID
nicAbortReassemblyList (
    PLIST_ENTRY pToBeFreedList
    )
/*++

Routine Description:
    Walks the list and calls nicAbortReassembly on each structure
       
    Does not do any lock or refcount work as all the  Reassembly structures are 
    off the Remote node and cannot be accessed by any other thread.
    

Arguments:
   pToBeFreedList - list of reassembly structures that are going to be freed


Return Value:
    None

--*/
{

    PLIST_ENTRY pReassemblyList  = ListNext (pToBeFreedList);
    PNDIS1394_REASSEMBLY_STRUCTURE pReassembly = NULL;

    TRACE( TL_T, TM_Recv, ( "==> nicAbortReassemblyList  pToBeFreedList %x",  pToBeFreedList));


    while (pReassemblyList  != pToBeFreedList)
    {
        pReassembly = CONTAINING_RECORD(pReassemblyList, 
                                        NDIS1394_REASSEMBLY_STRUCTURE, 
                                        ReassemblyListEntry);

        pReassemblyList = ListNext(pReassemblyList);

        TRACE( TL_T, TM_Recv, ( "       Aborting pReassembly %x",  pReassembly));
        
        nicAbortReassembly(pReassembly);
    }

}


VOID
nicFreeAllPendingReassemblyStructures(
    IN PADAPTERCB pAdapter
    )
/*++

Routine Description:
   When we are notified of a reset we need to go and invalidate all
   reassemblies
   
   This will always be called from the Reset code path . and will be at dispatch
   It will clear out all the remote node reaassembly and mark them as aborted.
   The Timer routine will then pick them up and free it

   Does not actually free anything. Just marks them as aborted
    

Arguments:
    Adapter - Adapter on which the Reset occurred.

Return Value:


--*/
{
    PLIST_ENTRY pRemoteNodeList = NULL;
    PREMOTE_NODE pRemoteNode = NULL;
    PLIST_ENTRY pReassemblyList = NULL;
    PNDIS1394_REASSEMBLY_STRUCTURE pReassembly = NULL;
    ULONG NumMarkedAborted = 0;
    
    TRACE( TL_T, TM_Recv, ( "==> nicFreeAllPendingReassemblyStructures "));

    
    pRemoteNodeList = ListNext(&pAdapter->PDOList);

    ADAPTER_ACQUIRE_LOCK (pAdapter);

    //
    // Walking through the remote nodes
    //
    while (pRemoteNodeList != &pAdapter->PDOList)
    {
        pRemoteNode = CONTAINING_RECORD(pRemoteNodeList, 
                                        REMOTE_NODE, 
                                        linkPdo);

        pRemoteNodeList = ListNext (pRemoteNodeList);
        

        //
        // Reference the remote node, so we can guarantee its presence
        //
        if (REMOTE_NODE_ACTIVE (pRemoteNode)== FALSE) 
        {
            //
            // The remote node is going away. Skip this remote node
            //
            continue;
        }
        if (nicReferenceRemoteNode (pRemoteNode, FreeAllPendingReassemblyStructures )== FALSE )
        {
            //
            // The remote node is going away. Skip this remote node
            //
            continue;
        }

        //
        // Now walking through all the reassembly structures on that remote node
        //
        REMOTE_NODE_REASSEMBLY_ACQUIRE_LOCK(pRemoteNode);
        
        pReassemblyList = ListNext (&pRemoteNode->ReassemblyList);

        while (pReassemblyList  != &pRemoteNode->ReassemblyList)
        {
            pReassembly = CONTAINING_RECORD (pReassemblyList, 
                                             NDIS1394_REASSEMBLY_STRUCTURE, 
                                             ReassemblyListEntry);

            pReassemblyList = ListNext(pReassemblyList);


            //
            // If the reassembly has not been touched since the last timer it needs to be freed.
            // Other threads can ask us to free the reassembly by setting the aborted flag
            //
            if (REASSEMBLY_TEST_FLAG (pReassembly, REASSEMBLY_ABORTED) == FALSE);
            {

                REASSEMBLY_SET_FLAG (pReassembly, REASSEMBLY_ABORTED);
            
                NdisInterlockedIncrement (&NumMarkedAborted);
            }
        }


        REMOTE_NODE_REASSEMBLY_RELEASE_LOCK(pRemoteNode);

        nicDereferenceRemoteNode (pRemoteNode, FreeAllPendingReassemblyStructures );   

    }

    
    ADAPTER_RELEASE_LOCK (pAdapter);
    


    TRACE( TL_T, TM_Recv, ( "<== nicFreeAllPendingReassemblyStructures NumMarkedAborted  %x"));


}



ULONG
nicReferenceReassembly (
    IN PNDIS1394_REASSEMBLY_STRUCTURE pReassembly,
    PCHAR pString
    )
/*++

Routine Description:
    This is the return packets handler. 
    This functikon handles all the instrumentation to catch outstanding packets and 
    then calls the internal return packets function

Arguments:
    MiniportAdapterContext  - the pAdapter structure,
    pPacket - pPacket that the protocol returns


Return Value:


--*/
{   
    ULONG Ref;
    
    Ref = NdisInterlockedIncrement (&pReassembly->Ref);

    TRACE( TL_V, TM_Ref, ( "**nicReferenceReassembly  pReassembly %x, to %d, %s ", pReassembly, pReassembly->Ref, pString) );

    return Ref;


}









ULONG
nicDereferenceReassembly (
    IN PNDIS1394_REASSEMBLY_STRUCTURE pReassembly,
    PCHAR pString
    )
/*++

Routine Description:
    Dereference the Reassembly strucure

    In the case that the ref hits zero, the Reassembly structure is no longer
    in the Remote Node List so no other thread other than the caller has 
    access to this strcuture.
    
Arguments:
    pReassembly - pReassembly strucure to be dereferenced,
    PCHAR - Character string for debugging purposes.
   
Return Value:


--*/
{   
    ULONG Ref;
    
    Ref = NdisInterlockedDecrement (&pReassembly->Ref);

    TRACE( TL_V, TM_Ref, ( "**nicDereferenceReassembly  pReassembly %x, to %d, %s ", pReassembly, pReassembly->Ref, pString) );

    if ( Ref ==0 )
    {
        
        TRACE( TL_V, TM_Ref, ( "**FREEING pReassembly %x,  ", pReassembly) );
        FREE_NONPAGED (pReassembly);
    }

    return Ref;
}





VOID
nicIndicateNdisPacketToNdis (
    PNDIS_PACKET pPacket, 
    PVCCB pVc, 
    PADAPTERCB pAdapter
    )

/*++

Routine Description:
    This is to be used to indicate packets to NDIS . 
    Assumption - There will be only one packet in the array
    
Arguments:
    ppPacket  - Packet Array
    pVc -Vc on which the packet came in.
    Adapter - Adapter in which the packet came in


Return Value:
    None.

--*/
{
    NDIS_STATUS             NdisStatus = NDIS_STATUS_SUCCESS;
    PRSVD                   pRsvd = NULL;
    PNDIS_MINIPORT_TIMER    pRcvTimer= NULL;
    PINDICATE_RSVD          pIndicateRsvd = NULL;


    TRACE( TL_T, TM_Recv, ( "==> nicIndicateNdisPacketToNdis  pPacket %x, pVc %x, pAdapter %x ",
                            pPacket , pVc, pAdapter));

    TRACE (TL_V, TM_Reas, ("Indicating packet " ));                            

    
    do
    {
        NdisInterlockedIncrement (&pAdapter->AdaptStats.ulRcvOk);

        nicDumpPkt (pPacket,"Indicating Rcv ");

        
        ASSERT (pPacket != NULL);   

        //
        // Set up the Context for the indication
        //
        pRsvd =(PRSVD)(pPacket->ProtocolReserved);
        pIndicateRsvd = &pRsvd->IndicateRsvd;


        //
        // Update the tag increment counter and indicate rcv
        //

        pIndicateRsvd->Tag =  NIC1394_TAG_INDICATED;

        ASSERT (pPacket != NULL);

        nicIncrementRcvVcPktCount(pVc,pPacket);

        NdisMCoIndicateReceivePacket(pVc->Hdr.NdisVcHandle, &pPacket, 1);

        ASSERT (pAdapter->MiniportAdapterHandle != NULL);
        NdisMCoReceiveComplete(pAdapter->MiniportAdapterHandle);


    }while (FALSE);





    TRACE( TL_T, TM_Recv, ( "<==nicIndicateNdisPacketToNdis  %x"));



}



NDIS_STATUS
nicValidateRecvDataIsoch(
    IN  PMDL                pMdl,
    IN  PISOCH_DESCRIPTOR  pIsochDescriptor,
    IN  PVCCB               pVc,
    OUT PNIC_RECV_DATA_INFO pRcvInfo
    )
/*++

Routine Description:

    This function ensures that the length of the received packet is within reason.
    In the Isoch case, we do not know the exact number of bytes received, so we check 
    against the Mdl Length in the isoch Descriptor

    
Arguments:


Return Value:


--*/
{

    NDIS_STATUS NdisStatus = NDIS_STATUS_FAILURE;

    do
    {
        NODE_ADDRESS NodeAddress;
        PGASP_HEADER pGaspHeader;
        //
        // Isoch header is already byte swapped
        //
        pRcvInfo->DataLength = pRcvInfo->p1394Data->IsochReceiveFragmented.IsochHeader.IH_Data_Length;

        if (pRcvInfo->DataLength <= (UINT)FIELD_OFFSET(DATA_FORMAT,IsochReceiveFragmented.Data))
        {
            // Too small. Note that for simplicitly we check for the 
            // fragmented case.
            //
            NdisStatus = NDIS_STATUS_FAILURE;
            break;
        }           

        pRcvInfo->fGasp = TRUE;

        //
        // The total length of the data indicated by the bus driver
        //
        pRcvInfo->Length1394 = pRcvInfo->DataLength + sizeof (ISOCH_HEADER) + sizeof(ULONG); // Account for the prefix and isoch header

        //
        // Validate the Received length. Isoch Descriptors do not give us the actual length of the received packet, so
        // we rely on the MDL length.
        //
        if ((pRcvInfo->DataLength < sizeof(GASP_HEADER)) || pRcvInfo->DataLength > pIsochDescriptor->ulLength)
        {
            NdisStatus = NDIS_STATUS_FAILURE;
            break;
        }

        //
        // The valid data does not include the gasp header
        //
        pRcvInfo->DataLength -= sizeof (GASP_HEADER);


        pRcvInfo->NdisPktContext.pIsochContext = pIsochDescriptor;

        pRcvInfo->pPacketPool = &((PCHANNEL_VCCB) pVc)->PacketPool;

        //
        // Get the source Info out. 
        //
        //
        // pRcvInfo->p1394Data points to the start of the Mdl's VA that was indicated by the bus driver
        //
        pGaspHeader = &pRcvInfo->p1394Data->IsochReceiveFragmented.GaspHeader;

        //
        // Byte swap the Gasp Header in the actual data. we own the buffer, so we can byte swap it   
        //
        pGaspHeader->FirstQuadlet.GaspHeaderHigh = SWAPBYTES_ULONG(pGaspHeader->FirstQuadlet.GaspHeaderHigh);
        pGaspHeader->SecondQuadlet.GaspHeaderLow = SWAPBYTES_ULONG(pGaspHeader->SecondQuadlet.GaspHeaderLow);

        TRACE (TL_V, TM_Recv, (" Gasp Hi %x, Gasp Lo %x.",  
                                pGaspHeader->FirstQuadlet.GaspHeaderHigh,
                                pGaspHeader->SecondQuadlet.GaspHeaderLow ) );

        pRcvInfo->pGaspHeader = pGaspHeader;
        
        pRcvInfo->SourceID = pGaspHeader->FirstQuadlet.u1.GH_NodeAddress.NA_Node_Number;

        pRcvInfo->SourceID = pGaspHeader->FirstQuadlet.u1.GH_NodeAddress.NA_Node_Number;

        NdisStatus = NDIS_STATUS_SUCCESS;


    } while (FALSE);

    return NdisStatus;
}



NDIS_STATUS
nicValidateRecvDataFifo(
    IN  PMDL                pMdl,
    IN  PNOTIFICATION_INFO pFifoContext,
    IN  PVCCB               pVc,
    OUT PNIC_RECV_DATA_INFO pRcvInfo
    )
/*++

Routine Description:

    This routine verifies that the length is not too small
    This routine initializes the RecvDataInfo for the default (unfragmented case).
    If the data is unfragmented the main recv routine will then call the Fragmented version of this routine

    This initializes the length and StartData and fGasp fields of the struct only 

Arguments:
    pMdl - Mdl that was indicated up by the bus driver
    RecvOp - Is this part of isoch callback, or AddrRange Callback
    pIndicatedStruct - NotificationInfo or IsochDescriptor
    pRcvInfo - Recv Structure that will be updated

Return Value:
    Success  - if all the operations succeeded  

--*/
{

    NDIS_STATUS NdisStatus = NDIS_STATUS_FAILURE;
    do
    {
        pRcvInfo->DataLength = pFifoContext->nLength;
        
        if (pRcvInfo->DataLength <= (UINT)FIELD_OFFSET(DATA_FORMAT, AsyncWriteFragmented.Data))
        {
            // Too small. Note that for simplicitly we check for the 
            // fragmented case.
            //
            NdisStatus = NDIS_STATUS_FAILURE;
            break;
        }

        pRcvInfo->fGasp = FALSE;

        //
        //In Fifo receives the DataLength is equal to the total amount of data indicated by the bus driver
        //
        pRcvInfo->Length1394 = pRcvInfo->DataLength;            
        
        pRcvInfo->NdisPktContext.pFifoContext = pFifoContext ->Fifo;

        pRcvInfo->pPacketPool = &((PRECVFIFO_VCCB) pVc)->PacketPool;

        pRcvInfo->SourceID =  ((PASYNC_PACKET)pFifoContext->RequestPacket)->AP_Source_ID.NA_Node_Number;

        NdisStatus = NDIS_STATUS_SUCCESS;

    }while (FALSE);

    return NdisStatus;


}


NDIS_STATUS
nicValidateRecvData(
    IN  PMDL                pMdl,
    IN  BUS_OPERATION       RecvOp,
    IN  PVOID               pIndicatedStruct,
    IN  PVCCB               pVc,
    OUT PNIC_RECV_DATA_INFO pRcvInfo
    )
/*++

Routine Description:

    This routine verifies that the length is not too small
    This routine initializes the RecvDataInfo for the default (unfragmented case).
    If the data is unfragmented the main recv routine will then call the Fragmented version of this routine

    This initializes the length and StartData and fGasp fields of the struct only 
Arguments:
    pMdl - Mdl that was indicated up by the bus driver
    RecvOp - Is this part of isoch callback, or AddrRange Callback
    pIndicatedStruct - NotificationInfo or IsochDescriptor
    pRcvInfo - Recv Structure that will be updated

Return Value:
    Success  - if all the operations succeeded  

--*/
{
    NDIS_STATUS                         NdisStatus = NDIS_STATUS_FAILURE;
    PDATA_FORMAT                        pData = NULL;
    NDIS1394_UNFRAGMENTED_HEADER        EncapHeader;
    PNDIS1394_UNFRAGMENTED_HEADER   pEncapHeader = NULL;
    NDIS1394_FRAGMENT_LF                lf;
    ULONG                               UlongLf;


    NdisZeroMemory (pRcvInfo , sizeof (NIC_RECV_DATA_INFO) );

    TRACE( TL_T, TM_Recv, ( "==>nicValidateRecvData pMdl %x, RecvOp %x, pIndicatedStruct %x, pRcvInfo %x", 
                                pMdl, RecvOp , pIndicatedStruct, pRcvInfo));

    ASSERT (RecvOp ==  IsochReceive || RecvOp == AddressRange);

    pRcvInfo->RecvOp = RecvOp;
    pRcvInfo->pVc = pVc;
    
    do
    {   
        if (pMdl == NULL)
        {
            NdisStatus = NDIS_STATUS_FAILURE;
            BREAK (TM_Recv, ("nicValidateRecvData  , no Mdl present") );
        }

        pRcvInfo->p1394Data = (PPACKET_FORMAT)NIC_GET_SYSTEM_ADDRESS_FOR_MDL (pMdl);

        if (pRcvInfo->p1394Data  == NULL)
        {
            NdisStatus = NDIS_STATUS_RESOURCES;
            break;
        }

    
        //
        // Check minimum valid packet size . Checks whether the data length that was passed to us includes
        // at least the first byte of data
        //
        
        if (RecvOp == IsochReceive)
        {
            NdisStatus = nicValidateRecvDataIsoch (pMdl, 
                                                 (PISOCH_DESCRIPTOR)pIndicatedStruct,
                                                 pVc,
                                                 pRcvInfo
                                                 );

            if (NdisStatus != NDIS_STATUS_SUCCESS)           
            {
                // Validation failed . exit
                break;
            }           
            
            //
            // Get to the Encap header. Should be at the same position for Fragmented and nonfragmented 
            //
            pEncapHeader = &pRcvInfo->p1394Data->IsochReceiveNonFragmented.NonFragmentedHeader;

        }
        else
        {

            NdisStatus = nicValidateRecvDataFifo(pMdl,(PNOTIFICATION_INFO)pIndicatedStruct,pVc,pRcvInfo);

            if (NdisStatus != NDIS_STATUS_SUCCESS)           
            {
                // Failure
                break;
            }

            pEncapHeader  = &pRcvInfo->p1394Data->AsyncWriteNonFragmented.NonFragmentedHeader;


        }

        //
        // Byteswap Unfrag Header into a local variable
        //
        //EncapHeader.HeaderUlong = SwapBytesUlong (pEncapHeader->HeaderUlong);

        EncapHeader.HeaderUlong = SWAPBYTES_ULONG (pEncapHeader->HeaderUlong);

        EncapHeader.HeaderUlong  = pEncapHeader->HeaderUlong & 0x000000C0;
        EncapHeader.HeaderUlong  = EncapHeader.HeaderUlong >> 6;
        
        pRcvInfo->lf = EncapHeader.HeaderUlong ; 
    
        //
        // Update the lf
        //
    
        pRcvInfo->lf = EncapHeader.HeaderUlong;
        TRACE (TL_V, TM_Reas,("Header %x\n",pRcvInfo->lf ) );

        ASSERT (EncapHeader.HeaderUlong <= lf_InteriorFragment);


        if (pRcvInfo->lf != lf_Unfragmented)
        {
            pRcvInfo->fFragmented = TRUE;
        }
        else
        {
            pRcvInfo->fFragmented = FALSE;
        }

        if (pRcvInfo->DataLength > pVc->Hdr.MTU)
        {
            //
            // This cannot belong to us
            //
            NdisStatus = NDIS_STATUS_FAILURE;
            break;
        }
        NdisStatus = NDIS_STATUS_SUCCESS;


        pRcvInfo->pEncapHeader = (PVOID)pEncapHeader;
        
        //
        // Spew out all the information discovered
        //
        TRACE ( TL_V, TM_Recv, ( "lf %x, p1394Data %x, Length1394 %x, DataLength %x, pEncapHeader %x " , 
                                pRcvInfo->lf,
                                pRcvInfo->p1394Data, 
                                pRcvInfo->Length1394, 
                                pRcvInfo->DataLength, 
                                pRcvInfo->pEncapHeader ) );

        
    } while (FALSE);

    TRACE( TL_T, TM_Recv, ( "<==nicValidateRecvData %x", NdisStatus));
    return NdisStatus;
}





VOID
nicInitRecvDataFragmented (
    IN  PMDL                pMdl,
    IN  BUS_OPERATION       RecvOp,
    IN  PVOID               pIndicatedStruct,
    OUT PNIC_RECV_DATA_INFO pRcvInfo
    )
/*++

Routine Description:
    The routine will extract from the packet all the information that is required for reassembly
    and store it in the pRcvInfo
    
Arguments:
    pMdl - Indicated Mdl
    RecvOp - IsochReceive ot AddressRange Callback
    pIndicatedStruct - IsochDesc or Address Fifo
    pRcvInfo - output structure

Return Value:
    None

--*/
{

    PNOTIFICATION_INFO                  pNotificationInfo = NULL;
    PGASP_HEADER                        pGaspHeader = NULL;
    PNDIS1394_FRAGMENT_HEADER           pEncapHeader = NULL; 

    
    TRACE( TL_T, TM_Recv, ( "==> nicInitRecvDataFragmented pMdl, %x, RecvOp %x, pIndicatedStruct %x, pRcvInfo %x", 
                                 pMdl, RecvOp, pIndicatedStruct, pRcvInfo));

    do
    {   
        pRcvInfo->pMdl  = pMdl;
        
        
        if (RecvOp == IsochReceive)
        {
            
            pRcvInfo->NdisPktContext.pIsochContext = (PISOCH_DESCRIPTOR) pIndicatedStruct;

        }
        else
        {
            pNotificationInfo = (PNOTIFICATION_INFO) pIndicatedStruct;
            
         
            pRcvInfo->NdisPktContext.pFifoContext = pNotificationInfo->Fifo;

        }

        
        //
        // Now byte swap the fragment header so it can be correctly interpreted
        //
        pEncapHeader = (PNDIS1394_FRAGMENT_HEADER )pRcvInfo->pEncapHeader;
        
        pRcvInfo->FragmentHeader.u.FH_High = SWAPBYTES_ULONG(pEncapHeader->u.FH_High);
        pRcvInfo->FragmentHeader.u1.FH_Low = SWAPBYTES_ULONG(pEncapHeader->u1.FH_Low);
        
        //
        // Now get the Dgl 
        //
        pRcvInfo->Dgl = (USHORT)pRcvInfo->FragmentHeader.u1.SecondQuadlet.FH_dgl;

        if (pRcvInfo->lf == lf_FirstFragment)
        {
            pRcvInfo->fFirstFragment = TRUE;
            pRcvInfo->EtherType  = pRcvInfo->FragmentHeader.u.FirstQuadlet_FirstFragment.FH_EtherType;
            pRcvInfo->FragmentOffset   = 0;

        }
        else
        {
            pRcvInfo->fFirstFragment = FALSE            ;
            pRcvInfo->FragmentOffset  = pRcvInfo->FragmentHeader.u.FirstQuadlet.FH_fragment_offset;

        }



        
        
        pRcvInfo->BufferSize = pRcvInfo->FragmentHeader.u.FirstQuadlet.FH_buffersize ;

        //
        // Spew out all the information that has been found
        //
        TRACE ( TL_V, TM_Recv, (" SourceId %x, FragHead Hi %x, FragHead Lo %x, Dgl %x, fFirstFragment %x",
                                pRcvInfo->SourceID,
                                pRcvInfo->FragmentHeader.u.FH_High, 
                                pRcvInfo->FragmentHeader.u1.FH_Low ,
                                pRcvInfo->Dgl,
                                pRcvInfo->fFirstFragment ) );

        TRACE ( TL_V, TM_Recv, ("  Fragment Offset %x, bufferSize %x", pRcvInfo->FragmentOffset, pRcvInfo->BufferSize));                                
        ASSERT (pRcvInfo->SourceID < 64);                           

    } while (FALSE);




    

    TRACE( TL_T, TM_Recv, ( "<==nicInitRecvDataFragmented " ));

}



NDIS_STATUS
nicInsertFragmentInReassembly (
    PNDIS1394_REASSEMBLY_STRUCTURE  pReassembly,
    PNIC_RECV_DATA_INFO pRcvInfo
    )
/*++

Routine Description:
 
    Checks for over laps and if valid then copies current fragment
    into the table

    This function does the validation for overlaps
  
Arguments:
    PNDIS1394_REASSEMBLY_STRUCTURE  pReassembly,
    PNDIS_BUFFER pNdisBuffer,
    PMDL pMdl,
    PVOID pIndicatedStructure,
    ULONG FragOffset,
    ULONG IPLength


Return Value:


--*/

{

    NDIS_STATUS     NdisStatus = NDIS_STATUS_SUCCESS; 
    BOOLEAN         fFragPositionFound = FALSE;
    ULONG           FragmentNum = 0;
    BOOLEAN         Completed = FALSE;
    PNDIS_BUFFER    pNdisBuffer = pRcvInfo->pNdisBuffer;
    PMDL            pMdl = pRcvInfo->pMdl;
    PVOID           pIndicatedStructure = pRcvInfo->NdisPktContext.pCommon;
    ULONG           FragOffset = pRcvInfo->FragmentOffset;
    ULONG           IPLength = pRcvInfo->DataLength - sizeof (NDIS1394_FRAGMENT_HEADER);
    

    TRACE( TL_T, TM_Recv, ( "==> nicInsertFragmentInReassembly " ));
    

    do
    {
        if (pReassembly->BufferSize != 0 && 
            FragOffset >= pReassembly->BufferSize )
        {
            NdisStatus = NDIS_STATUS_FAILURE;
            break;
        }

        //
        // First Find the correct entry in the frag table.
        //

        nicFindInsertionPosition (pReassembly, 
                                  FragOffset, 
                                  IPLength, 
                                  &FragmentNum);

        if (FragmentNum == FRAGMENT_NUM_INVALID ||
            FragmentNum > (MAX_ALLOWED_FRAGMENTS-1))
        {
            NdisStatus = NDIS_STATUS_FAILURE;
            break;
        }

        if (pReassembly->FragTable[FragmentNum].IPLength != 0)
        {
            //
            // we must copy the current fragments descriptors  in the table 
            // so as not to overwrite the table
            //
            LONG OffsetIndex =0;
            
            //
            // First lets check for overlaps. Do we overlap the last fragment.
            // At this point, FragmentNum contains the record for the 
            // next fragment in the reassembly
            //
            if (FragmentNum != 0)
            {
                ULONG PrevFragmentOffset = pReassembly->FragTable[FragmentNum-1].Offset ;
                ULONG PrevFramentLength = pReassembly->FragTable[FragmentNum-1].IPLength ;

                ULONG EndOfPrevFragment = PrevFramentLength + PrevFragmentOffset ;
                
                if (EndOfPrevFragment  >  FragOffset)
                {
                    NdisStatus = NDIS_STATUS_FAILURE;
                    break;
                }
    
            }

            //
            // Do we overlap the next fragment
            //
            if (FragmentNum < pReassembly->MaxOffsetTableIndex)
            {
                ULONG EndOfCurrentFragment = FragOffset + IPLength;
                ULONG StartOfNextFragment = pReassembly->FragTable[FragmentNum].Offset ;
                    

                if (EndOfCurrentFragment  > StartOfNextFragment)
                {
                    NdisStatus = NDIS_STATUS_FAILURE;
                    break;
            
                }
            }
            
            //
            // Now make room for this fragment
            //
            OffsetIndex = pReassembly->MaxOffsetTableIndex ; 

            if (OffsetIndex >= MAX_ALLOWED_FRAGMENTS)
            {
                NdisStatus = NDIS_STATUS_FAILURE;
                ASSERT(OffsetIndex > MAX_ALLOWED_FRAGMENTS) ;
                break;
            }
            
            //
            // Signed compare and move the records ahead by one
            //
            while (OffsetIndex >= (LONG)FragmentNum)
            {
                pReassembly->FragTable[OffsetIndex+1].Offset  =  pReassembly->FragTable[OffsetIndex].Offset ;
                pReassembly->FragTable[OffsetIndex+1].IPLength =  pReassembly->FragTable[OffsetIndex].IPLength;
                pReassembly->FragTable[OffsetIndex+1].pMdl =  pReassembly->FragTable[OffsetIndex].pMdl;
                pReassembly->FragTable[OffsetIndex+1].pNdisBuffer=  pReassembly->FragTable[OffsetIndex].pNdisBuffer;
                pReassembly->FragTable[OffsetIndex+1].pNdisBuffer=  pReassembly->FragTable[OffsetIndex].pNdisBuffer;
                pReassembly->FragTable[OffsetIndex+1].IndicatedStructure.pCommon = pReassembly->FragTable[OffsetIndex].IndicatedStructure.pCommon ;
                pReassembly->FragTable[OffsetIndex+1].FragHeader =  pReassembly->FragTable[OffsetIndex].FragHeader;
                
                OffsetIndex --;                     
            }

        }

        pNdisBuffer->Next = NULL;
        pMdl->Next = NULL;

        //
        // Copy the current fragment into the table
        //
        pReassembly->FragTable[FragmentNum].Offset = FragOffset;
        pReassembly->FragTable[FragmentNum].IPLength = IPLength;
        pReassembly->FragTable[FragmentNum].pNdisBuffer = pNdisBuffer;
        pReassembly->FragTable[FragmentNum].pMdl    = pMdl;
        pReassembly->FragTable[FragmentNum].FragHeader =  pRcvInfo->FragmentHeader;
        
        if (pReassembly->ReceiveOp == IsochReceive)
        {
            pReassembly->FragTable[FragmentNum].IndicatedStructure.pCommon = &((PISOCH_DESCRIPTOR)pIndicatedStructure)->DeviceReserved[IsochNext];
        }
        else
        {
            pReassembly->FragTable[FragmentNum].IndicatedStructure.pFifo = (PADDRESS_FIFO)pIndicatedStructure;
        }
        
        pReassembly->BytesRecvSoFar += IPLength;
        //
        // Now increment the Max offset
        //

        pReassembly->MaxOffsetTableIndex ++;
        
        if (pReassembly->BufferSize == 0)
        {       
            pReassembly->BufferSize = pRcvInfo->BufferSize;
        }

        //
        // Add the unfragmented header here as we have to extract the EtherType here
        //
        if (pRcvInfo->fFirstFragment == TRUE)
        {
                
            pReassembly->EtherType = (USHORT)pRcvInfo->EtherType;

            nicAddUnfragmentedHeader (pReassembly, pRcvInfo->pNdisBufferData );
            

        }

        //
        // According to the RFC, the buffersize of the reassembled packet
        // is 1 less than the number of bytes in the packet
        //
        if (pReassembly->BytesRecvSoFar  == pReassembly->BufferSize + 1)
        {

            nicChainReassembly (pReassembly);

            pReassembly->fReassemblyComplete = TRUE;

            RemoveEntryList (&pReassembly->ReassemblyListEntry);


        }
        


    } while (FALSE);
    

    TRACE( TL_T, TM_Recv, ( "<== nicInsertFragmentInReassembly Status %x, Complete ", NdisStatus , pReassembly->fReassemblyComplete  ));

    return NdisStatus;
}










VOID
nicFindInsertionPosition (
    PNDIS1394_REASSEMBLY_STRUCTURE  pReassembly, 
    ULONG FragOffset, 
    ULONG IPLength, 
    PULONG pFragmentNum
    )
/*++

Routine Description:
 
    This functions figures out where should the new fragment be inserted into 
    our tracking array. If this is our first fragment, 0 is returned. If the offset 
    is greater than the last fragment, the next available position in the array is 
    returned.

    If the new framgent is somewhere in the middle, than the position is based on
    the offset of the newly arrived packet in relation to the already arrived fragment's 
    offset
  
Arguments:
    pReassembly,  - Our tracking structure
    FragOffset - The Offset of the new fragment
    IPLength,  - The length of the new fragment
    pFragmentNum - the output variable


Return Value:


--*/

{

    ULONG FragmentNum = 0;
    

    do
    {
        //
        // First Do quick checks for Inorder reassembly
        //

        //
        // Is it the first arrived fragment
        //
        if (pReassembly->MaxOffsetTableIndex == 0 || 
            FragOffset < pReassembly->FragTable[0].Offset +pReassembly->FragTable[0].IPLength  )
        {
            FragmentNum = 0;    
            break;
        }

        //
        // Do we need to insert it in the last position
        //
        if ((pReassembly->FragTable[pReassembly->MaxOffsetTableIndex-1].Offset +
            pReassembly->FragTable[pReassembly->MaxOffsetTableIndex-1].IPLength ) <=
            FragOffset)
        {

            FragmentNum = pReassembly->MaxOffsetTableIndex; 

            break;
        }

        //
        // Now walk the table and try to find the correct offset
        // We know there is atleast one entry and the current fragment
        // goes is not the last entry
        //
        while ( FragmentNum != pReassembly->MaxOffsetTableIndex)
        {
            if (FragOffset < pReassembly->FragTable[FragmentNum].Offset)
            {
                //
                //We have found the Correct position
                //
                break;
            }

            FragmentNum++;

            

        }
        ASSERT (FragmentNum != pReassembly->MaxOffsetTableIndex); 

        FragmentNum = FRAGMENT_NUM_INVALID;        


    } while (FALSE);


        
    *pFragmentNum = FragmentNum;

    
}




VOID 
nicChainReassembly (
    IN PNDIS1394_REASSEMBLY_STRUCTURE  pReassembly
    )
/*++

Routine Description:
 Chains the mdl, ndis buffers and indicated structures
 This can be called from abort on the reasssembly complete code path

Arguments:
    pReassembly

Return Value:


--*/

{

    ULONG i = 0;


    //
    // first chain all fragments save the last one
    //
    while (i< pReassembly->MaxOffsetTableIndex-1)
    {
        PFRAGMENT_DESCRIPTOR pCurr = & pReassembly->FragTable[i];
        PFRAGMENT_DESCRIPTOR pNext = & pReassembly->FragTable[i+1];
        
        ASSERT (pNext->IPLength != 0);

        pCurr->pMdl->Next = pNext->pMdl;
        pCurr->pNdisBuffer->Next = pNext->pNdisBuffer;
        pCurr->IndicatedStructure.pListEntry->Next = pNext->IndicatedStructure.pListEntry;

        i++;
    }


    //
    // Clear the next pointers for the last descriptor
    //
    {
        PFRAGMENT_DESCRIPTOR pLast = & pReassembly->FragTable[pReassembly->MaxOffsetTableIndex-1];
        pLast->pMdl->Next = NULL;
        pLast->pNdisBuffer->Next = NULL;
        pLast->IndicatedStructure.pListEntry->Next = NULL;

    }   

    pReassembly->pHeadNdisBuffer = pReassembly->FragTable[0].pNdisBuffer;
    pReassembly->pHeadMdl = pReassembly->FragTable[0].pMdl;

    if (pReassembly->ReceiveOp == IsochReceive)
    {
        //
        // The pointer currently has the Next field. But the Head expects that start of an IsochDescriptor
        //
        pReassembly->Head.pCommon = CONTAINING_RECORD (pReassembly->FragTable[0].IndicatedStructure.pCommon,
                                                        ISOCH_DESCRIPTOR,
                                                        DeviceReserved[IsochNext] );

    }
    else
    {
        pReassembly->Head.pCommon = pReassembly->FragTable[0].IndicatedStructure.pCommon;
    }

    

}



NDIS_STATUS
nicInitSerializedReassemblyStruct(
    PADAPTERCB pAdapter
    )
/*++

Routine Description:
  Initialize the Reassembly serialization structure

Arguments:
  padapter

Return Value:
 Success

--*/
{


    NdisZeroMemory (&pAdapter->Reassembly, sizeof(pAdapter->Reassembly));
    InitializeListHead(&pAdapter->Reassembly.Queue); // Not be Used

    NdisInitializeEvent (&pAdapter->Reassembly.CompleteEvent.NdisEvent);
    pAdapter->Reassembly.CompleteEvent.EventCode = Nic1394EventCode_InvalidEventCode;

    NdisMInitializeTimer (&pAdapter->Reassembly.Timer,
                         pAdapter->MiniportAdapterHandle,
                         ReassemblyTimerFunction ,
                         pAdapter);



    return NDIS_STATUS_SUCCESS;

}


VOID
nicDeInitSerializedReassmblyStruct(
    PADAPTERCB pAdapter
    )
/*++

Routine Description:
 Deinits the Reassembly routine routine

  if the timer is set, it waits the timer out.
 As all the reassemblies will be marked as aborted in nicFreeAllPendingReassemblies (below)
 it queues a timer one last time to go in and delete all the reassembly structures.
 
Arguments:


Return Value:


--*/
{
 

    do
    {
        
        BOOLEAN bTimerAlreadySet = FALSE;
        //
        // If this adapter is halting, then mark all reassemblies as aborted
        //
        nicFreeAllPendingReassemblyStructures(pAdapter);


        //
        // First wait for any pending timer to fire.
        //
        ADAPTER_ACQUIRE_LOCK (pAdapter);
        bTimerAlreadySet = pAdapter->Reassembly.bTimerAlreadySet ;               
        if (bTimerAlreadySet == TRUE)
        {
            //
            // if the (bTimerAlreadySet==TRUE ), it means we can clear/init the event. 
            // Because the TimerAlreadySet is cleared and the Event is always set within
            // the same Acquire-Release Spinlock
            //
            NdisResetEvent (&pAdapter->Reassembly.CompleteEvent.NdisEvent);
            pAdapter->Reassembly.CompleteEvent.EventCode = Nic1394EventCode_InvalidEventCode;
        
            
        }
        ADAPTER_RELEASE_LOCK(pAdapter);


        if (bTimerAlreadySet == TRUE)
        {
            NdisWaitEvent (&pAdapter->Reassembly.CompleteEvent.NdisEvent,WAIT_INFINITE); 
        }

        //
        // Reset the event , to prepare for the next wait.
        //
        pAdapter->Reassembly.CompleteEvent.EventCode = Nic1394EventCode_InvalidEventCode;

        NdisResetEvent (&pAdapter->Reassembly.CompleteEvent.NdisEvent);


        //
        // Now enqueue the timer one last time to free all pending reassemblies.
        // and Stop any further reassembly timers 
        //

        nicQueueReassemblyTimer (pAdapter,TRUE); 


        //
        // Wait for the last timer to fire.
        //

        bTimerAlreadySet = pAdapter->Reassembly.bTimerAlreadySet ;               

        //
        // Only do the wait, if nicQueueReassembly Timer actually queued a reassembly timer
        //
        if (bTimerAlreadySet == TRUE)
        {   
            NdisWaitEvent (&pAdapter->Reassembly.CompleteEvent.NdisEvent,WAIT_INFINITE); 
        }



    } while (FALSE);
    
        
 

}



NDIS_STATUS
nicQueueReassemblyTimer(
    PADAPTERCB pAdapter,
    BOOLEAN fIsLastTimer
    )
/*++

Routine Description:

  Queues a timer to be fired in one second. 
  If there is already a timer active it quietly exists

Arguments:
  Self explanatory

Return Value:


--*/
    
{
    NDIS_STATUS Status = NDIS_STATUS_FAILURE;
    BOOLEAN fSetTimer = FALSE;

    do
    {
    

        ADAPTER_ACQUIRE_LOCK (pAdapter);



        //
        // If the timer is not set, then this thread must set it
        //


        if (pAdapter->Reassembly.bTimerAlreadySet == FALSE && // timer is not set
            pAdapter->Reassembly.PktsInQueue > 0 &&   // there are packets to be reassembled
            ADAPTER_TEST_FLAG (pAdapter,fADAPTER_NoMoreReassembly) == FALSE ) // the adapter is not halting
        {
            fSetTimer = TRUE;
            pAdapter->Reassembly.bTimerAlreadySet = TRUE;
        }

        if (fIsLastTimer == TRUE)
        {
            //
            // Stop any further reassembly timers 
            //

            ADAPTER_SET_FLAG (pAdapter, fADAPTER_NoMoreReassembly);
        }


        ADAPTER_RELEASE_LOCK (pAdapter);

        //
        // Now queue the timer
        //
        
        if (fSetTimer == TRUE)
        {
            //
            //  Set the timer
            //
                         

            
            TRACE( TL_V, TM_Recv, ( "   Set Timer "));

            NdisMSetTimer ( &pAdapter->Reassembly.Timer, 2000);

    
        }


        Status = NDIS_STATUS_SUCCESS;

    } while (FALSE);

    ASSERT (Status == NDIS_STATUS_SUCCESS);
    return Status;
}


VOID
nicFifoAllocationScheme (
    PRECVFIFO_VCCB pRecvFIFOVc
)
/*++

Routine Description:
 If there are less than 20 fifo allocated, it starts a workitem to 
 allocate a lot more fifos

Arguments:
   

Return Value:


--*/
{

    BOOLEAN fQueueWorkItemInThisThread = FALSE;
    PNIC_WORK_ITEM pFifoWorkItem = NULL;

    do
    {
        if (pRecvFIFOVc->NumAllocatedFifos != NUM_RECV_FIFO_FIRST_PHASE)
        {
            break ; 
        }

        if (pRecvFIFOVc->FifoWorkItemInProgress == TRUE)        
        {
            break;
        } 
        
        pFifoWorkItem  = ALLOC_NONPAGED (sizeof(NIC_WORK_ITEM), MTAG_WORKITEM); 

        if (pFifoWorkItem == NULL)
        {
            break;
        }

        VC_ACQUIRE_LOCK(pRecvFIFOVc);

        if (VC_ACTIVE (pRecvFIFOVc)  && 
            pRecvFIFOVc->FifoWorkItemInProgress == FALSE)        
        {
            fQueueWorkItemInThisThread = TRUE;


            pRecvFIFOVc->FifoWorkItemInProgress = TRUE;

            // Add reference to the VC . Derefed in the WorkItem
            //
            nicReferenceCall((VCCB*)pRecvFIFOVc, "Queueing miniport Work Item\n");
        }

        VC_RELEASE_LOCK (pRecvFIFOVc);

        if (fQueueWorkItemInThisThread  == FALSE)
        {
            break;
        }

        //
        // Queue the workitem
        //
        NdisInitializeWorkItem ( &pFifoWorkItem->NdisWorkItem, 
                                 (NDIS_PROC) nicAllocateRemainingFifoWorkItem,
                                 (PVOID) pRecvFIFOVc);

        NdisScheduleWorkItem (&pFifoWorkItem->NdisWorkItem);

    } while (FALSE);

    if (fQueueWorkItemInThisThread  == FALSE  && 
        pFifoWorkItem != NULL)
    {
        FREE_NONPAGED (pFifoWorkItem);
    }

    
}



VOID
nicAllocateRemainingFifoWorkItem (
    PNDIS_WORK_ITEM pNdisWorkItem, 
    IN PVOID Context
    )
/*++

Routine Description:
    This follows a simple algorithm. It simply allocates fifos 
    until we reach our expected number of 100

Arguments:
   

Return Value:


--*/
{
    PRECVFIFO_VCCB pRecvFIFOVc = NULL;
    BOOLEAN fIsVcActive = FALSE;
    NDIS_STATUS NdisStatus = NDIS_STATUS_FAILURE;
    

    pRecvFIFOVc = (PRECVFIFO_VCCB) (Context);
    
    fIsVcActive = VC_ACTIVE(pRecvFIFOVc);

    do
    {
        PADDRESS_FIFO pRecvFifoElement = NULL;

        fIsVcActive = VC_ACTIVE(pRecvFIFOVc);

        if (fIsVcActive == FALSE)
        {
            break;
        }

        NdisStatus = nicGetInitializedAddressFifoElement (pRecvFIFOVc->Hdr.MTU, 
                                                    &pRecvFifoElement);

        if (NdisStatus != NDIS_STATUS_SUCCESS)
        {
            break;
        }

        ASSERT (pRecvFifoElement  != NULL);
         
        ExInterlockedPushEntrySList ( &pRecvFIFOVc->FifoSListHead,
                                     (PSLIST_ENTRY)&pRecvFifoElement->FifoList,
                                     &pRecvFIFOVc->FifoSListSpinLock);

        //
        // Add this once for every Address Fifo element inserted 
        // Will be decremented by  a call to nicFreeAddressFifo
        //
        VC_ACQUIRE_LOCK (pRecvFIFOVc);

        nicReferenceCall ((PVCCB) pRecvFIFOVc, "nicWorkItemFileSList");

        pRecvFIFOVc->NumAllocatedFifos++;

        VC_RELEASE_LOCK (pRecvFIFOVc);
      

    } while (pRecvFIFOVc->NumAllocatedFifos < NUM_RECV_FIFO_BUFFERS);

    pRecvFIFOVc->FifoWorkItemInProgress = FALSE;

    nicDereferenceCall ((PVCCB)pRecvFIFOVc,"nicAllocateRemainingFifoWorkItem" );

    FREE_NONPAGED(pNdisWorkItem);
    
}

