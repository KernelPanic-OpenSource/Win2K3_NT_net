/*++

Copyright (c) 2000-2002 Microsoft Corporation

Module Name:

    largemem.h

Abstract:

    The public definition of large memory allocator interfaces.

Author:

    George V. Reilly (GeorgeRe)    10-Nov-2000

Revision History:

--*/

#ifndef _LARGEMEM_H_
#define _LARGEMEM_H_


NTSTATUS
UlLargeMemInitialize(
    );

VOID
UlLargeMemTerminate(
    VOID
    );

ULONG
UlLargeMemUsagePercentage(
    VOID
    );

PMDL
UlLargeMemAllocate(
    IN ULONG Length
    );

VOID
UlLargeMemFree(
    IN PMDL pMdl
    );

BOOLEAN
UlLargeMemSetData(
    IN PMDL pMdl,
    IN PUCHAR pBuffer,
    IN ULONG Length,
    IN ULONG Offset
    );


// 2^20 = 1MB
#define MEGABYTE_SHIFT 20
C_ASSERT(PAGE_SHIFT < MEGABYTE_SHIFT);

#define PAGES_TO_MEGABYTES(P)  ((P) >> (MEGABYTE_SHIFT - PAGE_SHIFT))
#define MEGABYTES_TO_PAGES(M)  ((M) << (MEGABYTE_SHIFT - PAGE_SHIFT))
#define MEGABYTES_TO_BYTES(M)  ((M) << (MEGABYTE_SHIFT))


#endif // _LARGEMEM_H_
