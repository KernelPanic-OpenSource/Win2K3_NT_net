/*++

Copyright (c) 2000-2002 Microsoft Corporation

Module Name:

    httpconnp.h

Abstract:

    This module contains private declarations for manipulation of
    HTTP_CONNECTION objects.

Author:

    Eric Stenson (ericsten)       01-Nov-2000

Revision History:

--*/

#ifndef __HTTPCONNP_H__
#define __HTTPCONNP_H__

//
// private types
//

#define ERROR_LOG_INFO_FOR_ZOMBIE_DROP       "Connection_Dropped"
#define ERROR_LOG_INFO_FOR_ZOMBIE_DROP_SIZE  \
            (sizeof(ERROR_LOG_INFO_FOR_ZOMBIE_DROP) - sizeof(CHAR))

//
// for binding connections to app pool processes
//

#define IS_VALID_PROC_BINDING(pObject) \
    HAS_VALID_SIGNATURE(pObject, UL_APOOL_PROC_BINDING_POOL_TAG)

typedef struct _UL_APOOL_PROC_BINDING
{
    ULONG                   Signature;      // UL_APOOL_PROC_BINDING_POOL_TAG
    LIST_ENTRY              BindingEntry;

    PUL_APP_POOL_OBJECT     pAppPool;
    PUL_APP_POOL_PROCESS    pProcess;
} UL_APOOL_PROC_BINDING, *PUL_APOOL_PROC_BINDING;


//
// Private prototypes.
//

VOID
UlpSetZombieTimer(
    VOID
    );

VOID
UlpTerminateZombieList(
    VOID
    );

NTSTATUS
UlpZombifyHttpConnection(
    IN PUL_HTTP_CONNECTION pHttpConnection
    );

VOID
UlpZombieTimerWorker(
    IN PUL_WORK_ITEM pWorkItem
    );

VOID
UlpRemoveZombieHttpConnection(
    IN PUL_HTTP_CONNECTION pHttpConnection
    );

VOID
UlpCleanupZombieHttpConnection(
    IN PUL_HTTP_CONNECTION pHttpConnection
    );


PUL_APOOL_PROC_BINDING
UlpCreateProcBinding(
    IN PUL_APP_POOL_OBJECT pAppPool,
    IN PUL_APP_POOL_PROCESS pProcess
    );

VOID
UlpFreeProcBinding(
    IN PUL_APOOL_PROC_BINDING pBinding
    );

PUL_APOOL_PROC_BINDING
UlpFindProcBinding(
    IN PUL_HTTP_CONNECTION pHttpConnection,
    IN PUL_APP_POOL_OBJECT pAppPool
    );


#endif // __HTTPCONNP_H__
