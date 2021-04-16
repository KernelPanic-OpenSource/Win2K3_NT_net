/*++

Copyright (c) 1994  Microsoft Corporation

Module Name:

    dhcpcli.h

Abstract:

    This file is the central include file for the DHCP client service.

Author:

    Manny Weiser  (mannyw)  20-Oct-1992

Environment:

    User Mode - Win32

Revision History:

    Madan Appiah (madana)  21-Oct-1993

--*/

#ifndef _DHCPCLI_H_
#define _DHCPCLI_H_

//
//  NT public header files
//
#include <nt.h>
#include <ntrtl.h>
#include <nturtl.h>

#include <windef.h>
#include <winbase.h>
#include <winsock2.h>
#include <crypt.h>
#include <ws2tcpip.h>
#include <mstcpip.h>

//
//  DHCP public header files
//

#include <dhcp.h>
#include <dhcplib.h>
#if !defined(VXD)
#include <dhcpcapi.h>
#endif

//
// C Runtime Lib.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

//
//  Local header files
//

#include <dhcpdef.h>
#include <debug.h>
#include <gmacros.h>

#ifdef VXD
#include <vxdmsg.h>
#else
#include <dhcpmsg.h>
#endif

#include <ipexport.h>
#ifndef VXD
#include <ntddip.h>
#endif

#include <proto.h>


#ifdef VXD
#include <vxdprocs.h>
#endif


//
// debug heap
//
#include <heapx.h>

#ifndef VXD
#ifdef DBG
#ifdef __DHCP_USE_DEBUG_HEAP__

#pragma message ( "*** DHCP Client will use debug heap ***" )

#define DhcpAllocateMemory(x) calloc(1,x)
#define DhcpFreeMemory(x)     free(x)

#endif
#endif
#endif

#ifdef CHICAGO
#define _WINNT_
#include <vmm.h>
#endif  // CHICAGO
//
// Macros for pageable code.
//
#define CTEMakePageable( _Page, _Routine )  \
    alloc_text(_Page,_Routine)

#ifdef CHICAGO
#define ALLOC_PRAGMA
#undef  INIT
#define INIT _ITEXT
#undef  PAGE
#define PAGE _PTEXT
#define PAGEDHCP _PTEXT
#endif // CHICAGO

#if     defined(CHICAGO) && defined(DEBUG)
//
// This is asserts when the pageable code is called at inappropriate time.
// Since in reality all our pageable code is dynamically locked, there is no
// need for this.
//
//#define CTEPagedCode() _Debug_Flags_Service(DFS_TEST_REENTER+DFS_TEST_BLOCK)
#define CTEPagedCode()
#else
#define CTEPagedCode()
#endif

#include <options.h>
#include <optreg.h>
#include <stack.h>

#endif //_DHCPCLI_H_

