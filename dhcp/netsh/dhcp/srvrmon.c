/*++

Copyright (c) 1998  Microsoft Corporation

Module Name:

    Routing\Netsh\dhcp\dhcpmon.c

Abstract:

    SRVR Command dispatcher.

Created by:

    Shubho Bhattacharya(a-sbhatt) on 11/14/98

--*/
#include "precomp.h"

#define MAX_FQDN_SIZE 256

//
// The DHCP manager's commands are broken into 2 sets
//      - The commands are split into "command groups"
//        i.e, commands grouped by the VERB where the VERB is ADD, DELETE,
//        SHOW or SET.  This is not for any technical reason - only for
//        staying with the semantics used in netsh with which it will be 
//                integrated
//      - The commands which are supported by the subcontext commands of 
//        the server. The subcontext supported by DHCP is Server.
//
// A command is described using a CMD_ENTRY structure. It requires the
// command token, the handler, a short help message token and an extended 
// help message token.  To make it easier to create we use the 
// CREATE_CMD_ENTRY macro. This, however puts restrictions on how the tokens
// are named.
//
// The command groups are simply arrays of the CMD_ENTRY structure.  The 
// top level commands are also grouped in a similar array.
//
// The info about a complete command group is put in a CMD_GROUP_ENTRY
// structure, all of which are put in an array.
//
 

//
// NOTE: Since we have only one entry per group, currently, we really didnt
// need command groups. This is done for later extensibility.
// To add a command entry to a group, simply add the command to the appropriate
// array
// To add a command group - create and array and add its info to the
// command group array
//

#define CONTEXT_DHCP            L"Dhcp"
#define CONTEXT_SERVER          L"Dhcp Server"
#define CONTEXT_SCOPE           L"Scope"
#define CONTEXT_MSCOPE          L"MScope"

extern HANDLE   g_hModule;
extern HANDLE   g_hParentModule;
extern HANDLE   g_hDhcpsapiModule;
extern BOOL     g_bCommit;
extern BOOL     g_hConnect;
extern BOOL     g_fServer = FALSE;
PWCHAR   g_pwszServer = NULL;


LPWSTR  g_CurrentSubContext = NULL;

ULONG   g_ulSrvrInitCount = 0;
DWORD   g_dwMajorVersion = 0;
DWORD   g_dwMinorVersion = 0;

BOOL   g_IsSubcontext = FALSE;

DHCPMON_SUBCONTEXT_TABLE_ENTRY  g_SrvrSubContextTable[] =
{
    {L"Scope", HLP_SRVR_CONTEXT_SCOPE, HLP_SRVR_CONTEXT_SCOPE_EX, ScopeMonitor},
    {L"MScope", HLP_SRVR_CONTEXT_MSCOPE, HLP_SRVR_CONTEXT_MSCOPE_EX, MScopeMonitor},
};



CMD_ENTRY  g_SrvrAddCmdTable[] = {
    CREATE_CMD_ENTRY(SRVR_ADD_CLASS, HandleSrvrAddClass),
    CREATE_CMD_ENTRY(SRVR_ADD_MSCOPE, HandleSrvrAddMscope),
    CREATE_CMD_ENTRY(SRVR_ADD_OPTIONDEF, HandleSrvrAddOptiondef),
    CREATE_CMD_ENTRY(SRVR_ADD_SCOPE, HandleSrvrAddScope),
};

CMD_ENTRY  g_SrvrDeleteCmdTable[] = {
    CREATE_CMD_ENTRY(SRVR_DELETE_CLASS, HandleSrvrDeleteClass),
    CREATE_CMD_ENTRY(SRVR_DELETE_DNSCREDENTIALS, HandleSrvrDeleteDnsCredentials),
    CREATE_CMD_ENTRY(SRVR_DELETE_MSCOPE, HandleSrvrDeleteMscope),
    CREATE_CMD_ENTRY(SRVR_DELETE_OPTIONDEF, HandleSrvrDeleteOptiondef),
    CREATE_CMD_ENTRY(SRVR_DELETE_OPTIONVALUE, HandleSrvrDeleteOptionvalue),
    CREATE_CMD_ENTRY(SRVR_DELETE_SCOPE, HandleSrvrDeleteScope),
    CREATE_CMD_ENTRY(SRVR_DELETE_SUPERSCOPE, HandleSrvrDeleteSuperscope),
};

CMD_ENTRY g_SrvrRedoCmdTable[] = {
    CREATE_CMD_ENTRY(SRVR_REDO_AUTH, HandleSrvrRedoAuth),
    CREATE_CMD_ENTRY(SRVR_INITIATE_RECONCILE, HandleSrvrInitiateReconcile),
};

CMD_ENTRY g_SrvrExportCmdTable[] = {
    CREATE_CMD_ENTRY(SRVR_EXPORT, HandleSrvrExport),
};

CMD_ENTRY g_SrvrImportCmdTable[] = {
    CREATE_CMD_ENTRY(SRVR_IMPORT, HandleSrvrImport),
};

CMD_ENTRY g_SrvrSetCmdTable[] = {
    CREATE_CMD_ENTRY(SRVR_SET_AUDITLOG, HandleSrvrSetAuditlog),
    CREATE_CMD_ENTRY(SRVR_SET_BACKUPINTERVAL, HandleSrvrSetBackupinterval),
    CREATE_CMD_ENTRY(SRVR_SET_BACKUPPATH, HandleSrvrSetBackuppath),
    CREATE_CMD_ENTRY(SRVR_SET_DATABASECLEANUPINTERVAL, HandleSrvrSetDatabasecleanupinterval),
    CREATE_CMD_ENTRY(SRVR_SET_DATABASELOGGINGFLAG, HandleSrvrSetDatabaseloggingflag),
    CREATE_CMD_ENTRY(SRVR_SET_DATABASENAME, HandleSrvrSetDatabasename),
    CREATE_CMD_ENTRY(SRVR_SET_DATABASEPATH, HandleSrvrSetDatabasepath),
    CREATE_CMD_ENTRY(SRVR_SET_DATABASERESTOREFLAG, HandleSrvrSetDatabaserestoreflag),
    CREATE_CMD_ENTRY(SRVR_SET_DETECTCONFLICTRETRY, HandleSrvrSetDetectconflictretry),
    CREATE_CMD_ENTRY(SRVR_SET_DNSCREDENTIALS, HandleSrvrSetDnsCredentials),
    CREATE_CMD_ENTRY(SRVR_SET_DNSCONFIG, HandleSrvrSetDnsconfig),
    CREATE_CMD_ENTRY(SRVR_SET_OPTIONVALUE, HandleSrvrSetOptionvalue),
    CREATE_CMD_ENTRY(SRVR_SET_SERVER, HandleSrvrSetServer),
    CREATE_CMD_ENTRY(SRVR_SET_USERCLASS, HandleSrvrSetUserclass),
    CREATE_CMD_ENTRY(SRVR_SET_VENDORCLASS, HandleSrvrSetVendorclass),
};

CMD_ENTRY g_SrvrShowCmdTable[] = {
    CREATE_CMD_ENTRY(SRVR_SHOW_ALL, HandleSrvrShowAll),
    CREATE_CMD_ENTRY(SRVR_SHOW_AUDITLOG, HandleSrvrShowAuditlog),
    CREATE_CMD_ENTRY(SRVR_SHOW_BINDINGS, HandleSrvrShowBindings),
    CREATE_CMD_ENTRY(SRVR_SHOW_CLASS, HandleSrvrShowClass),
    CREATE_CMD_ENTRY(SRVR_SHOW_DETECTCONFLICTRETRY, HandleSrvrShowDetectconflictretry),
    CREATE_CMD_ENTRY(SRVR_SHOW_DNSCONFIG, HandleSrvrShowDnsconfig),
    CREATE_CMD_ENTRY(SRVR_SHOW_DNSCREDENTIALS, HandleSrvrShowDnsCredentials),
    CREATE_CMD_ENTRY(SRVR_SHOW_MIBINFO, HandleSrvrShowMibinfo),
    CREATE_CMD_ENTRY(SRVR_SHOW_MSCOPE, HandleSrvrShowMscope),
    CREATE_CMD_ENTRY(SRVR_SHOW_OPTIONDEF, HandleSrvrShowOptiondef),
    CREATE_CMD_ENTRY(SRVR_SHOW_OPTIONVALUE, HandleSrvrShowOptionvalue),
    CREATE_CMD_ENTRY(SRVR_SHOW_SCOPE, HandleSrvrShowScope),
    CREATE_CMD_ENTRY(SRVR_SHOW_SUPERSCOPE, HandleSrvrShowSuperScope),
    CREATE_CMD_ENTRY(SRVR_SHOW_SERVER, HandleSrvrShowServer),
    CREATE_CMD_ENTRY(SRVR_SHOW_SERVERCONFIG, HandleSrvrShowServerconfig),
    CREATE_CMD_ENTRY(SRVR_SHOW_SERVERSTATUS, HandleSrvrShowServerstatus),
    CREATE_CMD_ENTRY(SRVR_SHOW_USERCLASS, HandleSrvrShowUserclass),
    CREATE_CMD_ENTRY(SRVR_SHOW_VENDORCLASS, HandleSrvrShowVendorclass),
    CREATE_CMD_ENTRY(SRVR_SHOW_VERSION, HandleSrvrShowVersion),
};


CMD_GROUP_ENTRY g_SrvrCmdGroups[] = 
{
    CREATE_CMD_GROUP_ENTRY(GROUP_ADD, g_SrvrAddCmdTable),
    CREATE_CMD_GROUP_ENTRY(GROUP_DELETE, g_SrvrDeleteCmdTable),
    CREATE_CMD_GROUP_ENTRY(GROUP_EXPORT, g_SrvrExportCmdTable),
    CREATE_CMD_GROUP_ENTRY(GROUP_IMPORT, g_SrvrImportCmdTable),
    CREATE_CMD_GROUP_ENTRY(GROUP_REDO, g_SrvrRedoCmdTable),
    CREATE_CMD_GROUP_ENTRY(GROUP_SET, g_SrvrSetCmdTable),
    CREATE_CMD_GROUP_ENTRY(GROUP_SHOW, g_SrvrShowCmdTable),
};


CMD_ENTRY g_SrvrCmds[] = 
{
    CREATE_CMD_ENTRY(SRVR_LIST, HandleSrvrList),
    CREATE_CMD_ENTRY(SRVR_DUMP, HandleSrvrDump),
    CREATE_CMD_ENTRY(SRVR_HELP1, HandleSrvrHelp),
    CREATE_CMD_ENTRY(SRVR_HELP2, HandleSrvrHelp),
    CREATE_CMD_ENTRY(SRVR_HELP3, HandleSrvrHelp),
    CREATE_CMD_ENTRY(SRVR_HELP4, HandleSrvrHelp),
};


ULONG g_ulSrvrNumSubContext = sizeof(g_SrvrSubContextTable)/sizeof(DHCPMON_SUBCONTEXT_TABLE_ENTRY);
ULONG g_ulSrvrNumTopCmds = sizeof(g_SrvrCmds)/sizeof(CMD_ENTRY);
ULONG g_ulSrvrNumGroups = sizeof(g_SrvrCmdGroups)/sizeof(CMD_GROUP_ENTRY);

DWORD
WINAPI
SrvrCommit(
    IN  DWORD   dwAction
)
{
    BOOL    bCommit, bFlush = FALSE;

    switch(dwAction)
    {
        case NETSH_COMMIT:
        {
            if(g_bCommit)
            {
                return NO_ERROR;
            }

            g_bCommit = TRUE;

            break;
        }

        case NETSH_UNCOMMIT:
        {
            g_bCommit = FALSE;

            return NO_ERROR;
        }

        case NETSH_SAVE:
        {
            if(g_bCommit)
            {
                return NO_ERROR;
            }

            break;
        }

        case NETSH_FLUSH:
        {
            //
            // Action is a flush. Srvr current state is commit, then
            // nothing to be done.
            //

            if(g_bCommit)
            {
                return NO_ERROR;
            }

            bFlush = TRUE;

            break;
        }

        default:
        {
            return NO_ERROR;
        }
    }

    //
    // Switched to commit mode. So set all valid info in the
    // strutures. Free memory and invalidate the info.
    //

    return NO_ERROR;
}


DWORD
WINAPI
SrvrMonitor(
    IN      LPCWSTR     pwszMachine,
    IN OUT  LPWSTR     *ppwcArguments,
    IN      DWORD       dwArgCount,
    IN      DWORD       dwFlags,
    IN      LPCVOID     pvData,
    OUT     LPWSTR      pwcNewContext
    )
{
    DWORD                   dwError = NO_ERROR;
    DWORD                   dwIndex, i, j, k;
    DWORD                   dwIsServer = 0;
    DWORD                   SrvIndex = 0;
    BOOL                    bFound = FALSE;
    PFN_HANDLE_CMD          pfnHandler = NULL;
    PNS_CONTEXT_ENTRY_FN    pfnHelperEntryPt;
    PNS_CONTEXT_DUMP_FN     pfnHelperDumpPt;
    WCHAR                   wcSvrTemp[MAX_IP_STRING_LEN+1] = {L'\0'};
    BOOL                    fTemp = FALSE;                            
    DWORD                   dwNumMatched;
    DWORD                   dwCmdHelpToken = 0;
    DWORD                   dw = 0;

    if(dwArgCount is 1)
    {
        //Hmm! Hasn't passed any Server Info. Set the server to the
        // value pwsRouter
        WCHAR ServerInfo[1000] = L"\\\\";

        if( pwszMachine and
            wcslen(pwszMachine) > 2 and
            wcsncmp(pwszMachine, L"\\\\", 2) and
            IsIpAddress(pwszMachine) is FALSE )
        {
            wcscpy(&ServerInfo[2], pwszMachine );
            pwszMachine = ServerInfo;
        }

        if( FALSE is SetServerInfo(pwszMachine) )
        {
            return GetLastError();
        }
        else
        {
            g_fServer = TRUE;
            //wcscpy(pwcNewContext, L"dhcp server");
            return ERROR_CONTEXT_SWITCH;
        }
    }

    dwIndex = 1;

    //If it is a help for server command
    if( IsHelpToken(ppwcArguments[dwIndex]) is TRUE and
        g_fServer is FALSE )
    {
        DisplayMessage(g_hModule, HLP_DHCP_CONTEXT_SERVER_EX);
        dwError = NO_ERROR;
        goto CleanUp;
    }

    //Is it a servername or address?
    if( IsValidServer(ppwcArguments[dwIndex]) )
    {
        if( g_fServer is TRUE and
            dwArgCount > 2 )
        {
            wcscpy(wcSvrTemp, g_ServerIpAddressUnicodeString);
            fTemp = TRUE;
        }
        if( FALSE is SetServerInfo(ppwcArguments[dwIndex]) )
        {
            dwError = GetLastError();
            goto CleanUp;
        }

        pwcNewContext[wcslen(pwcNewContext)- wcslen(ppwcArguments[dwIndex]) -1 ] = L'\0';
        
        dw++;
        dwIndex++;
        SrvIndex = 1;
        dwIsServer++;
        g_fServer = TRUE;

    }
    else if( g_fServer is FALSE )
    {
        WCHAR ServerInfo[1000] = L"\\\\";

        if( pwszMachine and 
            wcslen(pwszMachine) > 2 and
            wcsncmp(pwszMachine, L"\\\\", 2) and
            IsIpAddress(pwszMachine) is FALSE )
        {
            wcscpy(&ServerInfo[2], pwszMachine );
            pwszMachine = ServerInfo;
        }
        
        if( FALSE is SetServerInfo(pwszMachine) )
        {
            dwError = GetLastError();
            goto CleanUp;
        }
        g_fServer = TRUE;
    }

    
    if( dwIndex >= dwArgCount )
    {            
        dwError = ERROR_CONTEXT_SWITCH;
        //wcscpy(pwcNewContext, L"dhcp server");
        goto CleanUp;
    }

    //Is it a top level(non Group command)?
    for(i=0; i<g_ulSrvrNumTopCmds; i++)
    {
        if(MatchToken(ppwcArguments[dwIndex],
                      g_SrvrCmds[i].pwszCmdToken))
        {
            bFound = TRUE;

            pfnHandler = g_SrvrCmds[i].pfnCmdHandler;

            dwCmdHelpToken = g_SrvrCmds[i].dwCmdHlpToken;
            
            dwIndex++;

            break;
        }
    }


    if(bFound)
    {
        if ((( dwArgCount - SrvIndex ) > 2 ) &&
            ( IsHelpToken(ppwcArguments[ dwIndex ])))
        {
            DisplayMessage(g_hModule, dwCmdHelpToken);

            dwError = NO_ERROR;
            goto CleanUp;
        }
        
        dwIndex++;

        dwError = (*pfnHandler)(pwszMachine, ppwcArguments+dw, dwIndex, dwArgCount-dw, dwFlags, pvData, &bFound);

        goto CleanUp;
    }
    //Is it meant for any of the subcontext?
    for(i = 0; i<g_ulSrvrNumSubContext; i++)
    {
        if( MatchToken(ppwcArguments[dwIndex], g_SrvrSubContextTable[i].pwszContext) )
        {
            bFound = TRUE;
            dwIndex++;
            pfnHelperEntryPt = g_SrvrSubContextTable[i].pfnEntryFn;
            DEBUG("Meant for subcontext under it");
            break;
        }
    }

    if( bFound )    //Subcontext
    {
        dwError = (pfnHelperEntryPt)(pwszMachine,
                                     ppwcArguments+1+dw,
                                     dwArgCount-1 -dw,
                                     dwFlags,
                                     pvData,
                                     pwcNewContext);
        if( dwError is ERROR_CONTEXT_SWITCH )
        {
            if( g_CurrentSubContext isnot NULL )
            {
                memset(g_CurrentSubContext, 0x00, (wcslen(g_CurrentSubContext)+1)*sizeof(WCHAR));
                DhcpFreeMemory(g_CurrentSubContext);
                g_CurrentSubContext = NULL;
            }

            g_CurrentSubContext = DhcpAllocateMemory((wcslen(g_SrvrSubContextTable[i].pwszContext)+1)*sizeof(WCHAR));
            if( g_CurrentSubContext is NULL )
            {
                dwError = ERROR_NOT_ENOUGH_MEMORY;
                goto CleanUp;
            }
            g_CurrentSubContext[0] = L'\0';
            wcscpy(g_CurrentSubContext, g_SrvrSubContextTable[i].pwszContext);
            g_IsSubcontext = TRUE;
        }

        goto CleanUp;
    }

    bFound = FALSE;

    //It is not a non Group Command. Then is it a config command for the manager?
    for(i = 0; (i < g_ulSrvrNumGroups) and !bFound; i++)
    {
        if(MatchToken(ppwcArguments[dwIndex],
                      g_SrvrCmdGroups[i].pwszCmdGroupToken))
        {
            //
            // Command matched entry i, so look at the table of sub commands 
            // for this command
            //
            if( dwArgCount > dwIndex+1 )
            {
                for (j = 0; j < g_SrvrCmdGroups[i].ulCmdGroupSize; j++)
                {
                    if (MatchCmdLine(ppwcArguments+dwIndex,
                                      dwArgCount - 1,
                                      g_SrvrCmdGroups[i].pCmdGroup[j].pwszCmdToken,
                                      &dwNumMatched))
                    {
                        bFound = TRUE;
                
                        pfnHandler = g_SrvrCmdGroups[i].pCmdGroup[j].pfnCmdHandler;
                
                        dwCmdHelpToken = g_SrvrCmdGroups[i].pCmdGroup[j].dwCmdHlpToken;

                        //
                        // break out of the for(j) loop
                        //
                        dwIndex+=dwNumMatched;
                        break;
                    }
                }
            }

            if(!bFound)
            {
                //
                // We matched the command group token but none of the
                // sub commands
                //

                DisplayMessage(g_hModule, 
                               EMSG_SRVR_INCOMPLETE_COMMAND);

                for (j = 0; j < g_SrvrCmdGroups[i].ulCmdGroupSize; j++)
                {
                    DisplayMessage(g_hModule, 
                             g_SrvrCmdGroups[i].pCmdGroup[j].dwShortCmdHelpToken);
                    
                                        DisplayMessage(g_hModule, MSG_DHCP_FORMAT_LINE);
                }

                dwError = ERROR_INVALID_PARAMETER;
                goto CleanUp;
            }
            else
            {
                //
                // quit the for(i)
                //

                break;
            }
        }
    }




    if (!bFound)
    {
        //
        // Command not found. 
        //
        if( _wcsicmp(ppwcArguments[dwIndex], L"..") is 0 )
        {
            if( g_IsSubcontext is FALSE )
            {
                if(g_pwszServer)
                {
                    DhcpFreeMemory(g_pwszServer);
                    g_pwszServer = NULL;
                }
                memset(g_ServerIpAddressUnicodeString, 0x00, (MAX_IP_STRING_LEN+1)*sizeof(WCHAR));
                g_fServer = FALSE;
            }
            else
            {
                g_IsSubcontext = FALSE;
            }
        }

        dwError = ERROR_CMD_NOT_FOUND;
        goto CleanUp;
    }

    //
    // See if it is a request for help.
    //

    dwNumMatched += dwIsServer;

    if (dwNumMatched < (dwArgCount - 1) &&
        IsHelpToken(ppwcArguments[dwNumMatched + 1]))
    {
        DisplayMessage(g_hModule, dwCmdHelpToken);

        dwError = NO_ERROR;
        goto CleanUp;
    }
    
    //
    // Call the parsing routine for the command
    //

    dwError = (*pfnHandler)(pwszMachine, ppwcArguments, dwIndex, dwArgCount - dwIndex, dwFlags, pvData, &bFound);

    if( dwError is ERROR_CONTEXT_SWITCH )
    {
        if( ( _wcsicmp(g_CurrentSubContext, CONTEXT_SCOPE) is 0 ) or
            ( _wcsicmp(g_CurrentSubContext, CONTEXT_MSCOPE) is 0 ) )
        {
            //pwcNewContext[0] = L'\0';
            //wcscpy(pwcNewContext, CONTEXT_SERVER);
            dwError = ERROR_CONTEXT_SWITCH;

        }
        else
        {
            dwError = NO_ERROR;
        }
        goto CleanUp;
    }
CleanUp:
    if( fTemp )
    {
        fTemp = SetServerInfo(wcSvrTemp);
    }
    return dwError;
}



DWORD
WINAPI
SrvrUnInit(
    IN  DWORD   dwReserved
    )
{
    if(InterlockedDecrement(&g_ulSrvrInitCount) isnot 0)
    {
        return NO_ERROR;
    }

    return NO_ERROR;
}

BOOL
SetServerInfo(
    IN  LPCWSTR  pwszServerInfo
)
{

    PHOSTENT pHost;
    INT      retval;
    SOCKADDR_IN addr;
    DWORD    Len, Error;

    // Get the FQDN into g_pwszServer
    if ( NULL != g_pwszServer ) {
        DhcpFreeMemory( g_pwszServer );
        g_pwszServer = NULL;
    }


    // skip the leading backslashes if any
    if (( NULL != pwszServerInfo ) && 
        ( pwszServerInfo[ 0 ] == L'\\') &&
        ( pwszServerInfo[ 1 ] == L'\\')) {
        pwszServerInfo = &pwszServerInfo[ 2 ];
    }
    pHost = UnicodeGetHostByName( pwszServerInfo, &g_pwszServer );
    if ( NULL == pHost ) {
        return FALSE;
    }

    // copy the server info into the global variables

    addr.sin_family = AF_INET;
    addr.sin_addr.S_un.S_addr = *(( u_long * )(pHost->h_addr_list) [ 0 ]);
    addr.sin_port = 0;

    LocalFree( pHost );

    Len = sizeof( g_ServerIpAddressUnicodeString );
    retval = WSAAddressToString(( LPSOCKADDR ) &addr, sizeof( addr ), NULL,
                                g_ServerIpAddressUnicodeString, &Len );

    if ( 0 != retval ) {
        return FALSE;
    }

    // If the server does not have an FQDN, use the provided name
    if ( NULL == g_pwszServer ) {
	Len = wcslen( pwszServerInfo ) + 1;
	Len *= sizeof( WCHAR );
	g_pwszServer = DhcpAllocateMemory( Len );
	
	if ( NULL == g_pwszServer ) {
	    return FALSE;
	}
	wcscpy( g_pwszServer, pwszServerInfo );
    } // if 
    

    // Get the server version
    Error = DhcpGetVersion( g_ServerIpAddressUnicodeString, &g_dwMajorVersion, &g_dwMinorVersion );
    if ( ERROR_SUCCESS != Error ) {
	SetLastError( Error );
	DisplayMessage( g_hModule, EMSG_SRVR_UNKNOWN_VERSION,
			g_ServerIpAddressUnicodeString );
	return FALSE;
    }
    return TRUE;
} // SetServerInfo()

