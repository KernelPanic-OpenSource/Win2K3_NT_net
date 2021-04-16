;/*++ BUILD Version: 0001    // Increment this if a change has global effects
;
;Copyright (c) 1992  Microsoft Corporation
;
;Module Name:
;
;    atkmsg.h
;
;Abstract:
;
;    Definitions for AppleTalk events.
;
;Author:
;
;    Nikhil Kamkolkar (nikhilk@microsoft.com)
;    Jameel Hyder (jameelh@microsoft.com)
;
;Revision History:
;
;Notes:
;
;    This file is generated by the MC tool from the atkmsg.mc file.
;
;--*/
;
;#ifndef _ATKMSG_
;#define _ATKMSG_
;

SeverityNames=(Success=0x0:STATUS_SEVERITY_SUCCESS
               Informational=0x1:STATUS_SEVERITY_INFORMATIONAL
               Warning=0x2:STATUS_SEVERITY_WARNING
               Error=0x3:STATUS_SEVERITY_ERROR
              )
	
;
;/////////////////////////////////////////////////////////////////////////
;// AppleTalk Events - Start with the DDP upwards and then the glue code
;//                    events
;//
;/////////////////////////////////////////////////////////////////////////
;

;
;// Don't use %1 for insertion strings -- the I/O system provides the
;// first string (device name).
;

;
;//
;// INITIALIZE/SHUTDOWN Error Codes
;//
;


MessageId=+1 Severity=Error SymbolicName=EVENT_ATALK_ROUTING_BIND_FAIL
Language=English
Not used
Unable to bind to a adapter when in routing configuration.
.


MessageId=+1 Severity=Error SymbolicName=EVENT_ARAP_BIND_FAIL
Language=English
Not used
Unable to bind the ARAP adapter
.


MessageId=+1 Severity=Error SymbolicName=EVENT_ARAP_NO_RESRC
Language=English
Not used
Unable to retrieve ARAP adapter info
.


MessageId=+1 Severity=Warning SymbolicName=EVENT_ATALK_INIT_NAMEREGISTERFAILED
Language=English
Unable to register name for node on adapter "%2". 
Either the name is already in use on the network or is longer 
than 32 characters long.
Change the name of the adapter to a valid one. 
Unbind and rebind Appletalk protocol on the adapter.
.


MessageId=+1 Severity=Informational SymbolicName=EVENT_ATALK_INIT_NAMEREGISTERED
Language=English
A name was successfully registered for this node via AppleTalk protocol on adapter "%2".
.


MessageId=+1 Severity=Error SymbolicName=EVENT_ATALK_INVALID_PORTNAME
Language=English
The adapter name specified was greater than the maximum length of 32 characters allowed for adapter "%2".
The adapter has been initialized with a default port name.
If you change the adapter name, unbind and rebind Appletalk protocol on the adapter.
.


MessageId=+1 Severity=Error SymbolicName=EVENT_ATALK_INVALID_NETRANGE
Language=English
Invalid network range specified for adapter "%2".
Appletalk was not initialized on the adapter.
Verify the values for the Network Range limits for the adapter for AppleTalk Routing are valid using the Routing and Remote Access UI.
If values are modified, rebind Appletalk protocol on the adapter.
.


MessageId=+1 Severity=Error SymbolicName=EVENT_ATALK_INITIAL_RANGEOVERLAP
Language=English
The network ranges specified for one or more adapters overlap with those of the
adapter "%2".
Appletalk was not initialized on the adapter.
Verify the values for the Network Range limits for the adapters for AppleTalk Routing are valid using the Routing and Remote Access UI.
If values are modified, rebind Appletalk protocol on the adapter.
.


MessageId=+1 Severity=Warning SymbolicName=EVENT_ATALK_PRAM_OUTOFSYNC
Language=English
PRAM values for the network range were not within the
network range specified and are being ignored for adapter "%2".
.

MessageId=+1 Severity=Error SymbolicName=EVENT_ATALK_INVALID_DEFZONE
Language=English
Default zone specified was invalid for adapter "%2".
Verify the value for the Default Zone for the adapter for AppleTalk Routing is valid using the Routing and Remote Access UI.
.

MessageId=+1 Severity=Error SymbolicName=EVENT_ATALK_NO_DEFZONE
Language=English
No default zone was specified for adapter "%2".
Verify the value for the Default Zone for the adapter for AppleTalk Routing is valid using the Routing and Remote Access UI.
.

MessageId=+1 Severity=Error SymbolicName=EVENT_ATALK_INVALID_DESIREDZONE
Language=English
Desired zone invalid or not specified.
The value was ignored and processing was continued.
.

MessageId=+1 Severity=Error SymbolicName=EVENT_ATALK_INVALID_ZONEINLIST
Language=English
An invalid zone name specified in the zone list for adapter "%2".
The error was ignored and processing was continued.
.


MessageId=+1 Severity=Error SymbolicName=EVENT_ATALK_ZONE_NOTINLIST
Language=English
The default/desired zone name specified is not in the zone list for adapter "%2".
Appletalk was not initialized on the adapter.
.


MessageId=+1 Severity=Error SymbolicName=EVENT_ATALK_NO_DEFAULTPORT
Language=English
No adapter is configured to be the default adapter.
Appletalk was not initialized on any adapter.
Services over AppleTalk e.g. Print Server, File Server etc. will not be 
functional unless a default adapter is specified.
.


MessageId=+1 Severity=Error SymbolicName=EVENT_ATALK_INVALID_DEFAULTPORT
Language=English
Not used
An invalid adapter is configured to be the default adapter "%2". 
Localtalk adapters cannot be a default adapter in a routing configuration.
Appletalk could not be started on the adapter.
.


MessageId=+1 Severity=Error SymbolicName=EVENT_ATALK_INIT_COULDNOTGETNODE
Language=English
Unable to acquire a node on adapter "%2".
Appletalk could not be started on the adapter.
.


MessageId=+1 Severity=Error SymbolicName=EVENT_ATALK_SEEDROUTER_NONETRANGE
Language=English
A seeding adapter had no network range specified for adapter "%2".
Appletalk could not be started on the adapter.
.


MessageId=+1 Severity=Error SymbolicName=EVENT_ATALK_SEEDROUTER_NOZONELIST
Language=English
A seeding adapter had no zone list specified on adapter "%2".
Appletalk could not be started on the adapter.
.


MessageId=+1 Severity=Error SymbolicName=EVENT_ATALK_SEEDROUTER_TOOMANYZONES
Language=English
Not used
A seeding adapter had too many zones specified on adapter "%2". The limit on zones is 255.
.


MessageId=+1 Severity=Error SymbolicName=EVENT_ATALK_NONEXTENDED_ZONEINFOINVALID
Language=English
Not used
Either the default zone was null or the zone list was empty on a LocalTalk
adapter "%2".
.


MessageId=+1 Severity=Error SymbolicName=EVENT_ATALK_EXTRAZONESINLIST
Language=English
Not used
There was more than one zone specified in the zone list for a LocalTalk adapter "%2".
.



MessageId=+1 Severity=Error SymbolicName=EVENT_ATALK_OPENATALKKEY
Language=English
Not used
Unable to open the AppleTalk key in the registry.
.


MessageId=+1 Severity=Error SymbolicName=EVENT_ATALK_INVALID_BINDINGS
Language=English
Not used
Started with no bindings. The protocol must be bound to at least one adapter.
.


;
;//
;//     ROUTER Error Codes
;//
;

MessageId=+1 Severity=Error SymbolicName=EVENT_ATALK_ROUTER_COULDNOTGETNODE
Language=English
Not used
Unable to start the router on adapter "%2" because a node could not be acquired.
.


MessageId=+1 Severity=Error SymbolicName=EVENT_ATALK_ROUTER_ZIPRTMPERROR
Language=English
Not used
Unable to start ZIP/RTMP processes on adapter "%2".
.

MessageId=+1 Severity=Error SymbolicName=EVENT_ATALK_ROUTER_RELEASENODE
Language=English
Not used
Unable to release a node acquired on adapter "%2".
.




;
;//
;//     AARP Error Codes
;//
;

MessageId=+1 Severity=Error SymbolicName=EVENT_ATALK_INVALIDAARPPACKET
Language=English
An invalid AARP packet was received on adapter "%2".
.


MessageId=+1 Severity=Error SymbolicName=EVENT_ATALK_AARP_SEND_FAIL
Language=English
An AARP packet could not be sent on adapter "%2".
.


MessageId=+1 Severity=Error SymbolicName=EVENT_ATALK_OPENZIPSOCKET
Language=English
The Zone Information Socket could not be opened on a new node being
acquired, and the acquired node is being released on adapter "%2".
.


MessageId=+1 Severity=Warning SymbolicName=EVENT_ATALK_STARTUPRANGENODE
Language=English
Releasing a node acquired on the adapter "%2" because the connected network
is not seeded by any router on the net
.


MessageId=+1 Severity=Warning SymbolicName=EVENT_ATALK_INITIALRANGENODE
Language=English
The node ID obtained during Appletalk initialization on adapter "%2" was assigned to another device.
Current node ID was released and a new node ID was acquired from the seed router in the seeded range.
.


MessageId=+1 Severity=Error SymbolicName=EVENT_ATALK_AMT_INVALIDSOURCE
Language=English
The Address Mapping Table for adapter "%2" was not updated because the incoming
packet contained an invalid source address
.


;
;//
;//     ZIP Error Codes
;//
;

MessageId=+1 Severity=Warning SymbolicName=EVENT_ATALK_ZIPSENDPACKET
Language=English
Not used
Unable to send a ZIP packet on adapter "%2".
.


MessageId=+1 Severity=Warning SymbolicName=EVENT_ATALK_ZIPSOCKETCLOSE
Language=English
Not used
Unable to close a socket opened for a ZIP request on adapter "%2".
.


;
;//
;//     NODE Error Codes
;//
;



MessageId=+1 Severity=Error SymbolicName=EVENT_ATALK_NODE_OPENSOCKETS
Language=English
Unable to open RTMP/NBP/EP sockets on a node created on adapter "%2".
Appletalk could not be started on the adapter.
.


MessageId=+1 Severity=Error SymbolicName=EVENT_ATALK_NODE_CLOSESOCKETS
Language=English
Not used
Unable to close sockets on a node on adapter "%2" and so the
node may not be released properly
.


MessageId=+1 Severity=Error SymbolicName=EVENT_ATALK_CANT_CREATE_DEVICE
Language=English
Unable to create the device "%2".  
AppleTalk protocol could not be started.
The error is in the data.
.

;
;//
;//     NDIS Level Error Codes
;//
;

MessageId=+1 Severity=Error SymbolicName=EVENT_ATALK_REGISTERPROTOCOL
Language=English
Unable to register with the NDIS wrapper.
Appletalk could not be started on the adapter.
.

MessageId=+1 Severity=Error SymbolicName=EVENT_ATALK_DEREGISTERPROTOCOL
Language=English
Unable to deregister with the NDIS wrapper.
Appletalk protocol may be in an unstable state on the adapter.
.

MessageId=+1 Severity=Error SymbolicName=EVENT_ATALK_OPENADAPTER
Language=English
Unable to open the adapter (bind to the mac) "%2".
Appletalk could not be started on the adapter.
.

MessageId=+1 Severity=Warning SymbolicName=EVENT_ATALK_CLOSEADAPTER
Language=English
Unable to close the adapter "%2".
Appletalk protocol may be in an unstable state on the adapter.
.

MessageId=+1 Severity=Error SymbolicName=EVENT_ATALK_NOTBOUNDTOMAC
Language=English
AppleTalk is not bound to the adapter "%2". 
Request for the adapter was ignored.
.

MessageId=+1 Severity=Warning SymbolicName=EVENT_ATALK_STATIONADDRESS
Language=English
Unable to get the station address for adapter "%2".
The error was ignored and processing was continued.
.

MessageId=+1 Severity=Warning SymbolicName=EVENT_ATALK_LOOKAHEADSIZE
Language=English
Unable to set the lookahead size for adapter "%2".
Appletalk could not be started on the adapter.
.


MessageId=+1 Severity=Warning SymbolicName=EVENT_ATALK_PACKETFILTER
Language=English
Unable to set the packet filter for adapter "%2".
Appletalk could not be started on the adapter.
.


MessageId=+1 Severity=Error SymbolicName=EVENT_ATALK_NDISREQUEST
Language=English
Unable to allocate resources from the NDIS wrapper.
System may be low on resources.
.

MessageId=+1 Severity=Error SymbolicName=EVENT_ATALK_NDISRESOURCES
Language=English
Unable to allocate resources from the NDIS wrapper.
Appletalk could not be started on the adapter.
System may be low on resources.
.

MessageId=+1 Severity=Warning SymbolicName=EVENT_ATALK_AARPPACKET
Language=English
Not used
An AARP packet was not completely available in the lookahead data from NDIS
on adapter "%2".
.

MessageId=+1 Severity=Warning SymbolicName=EVENT_ATALK_PACKETINVALID
Language=English
Packet received on adapter "%2" contained errors and is not being accepted.
.

;//
;//     GENERAL Error Codes
;//

MessageId=+1 Severity=Error SymbolicName=EVENT_ATALK_RESOURCES
Language=English
Unable to allocate resources.
.

MessageId=+1 Severity=Error SymbolicName=EVENT_ATALK_MEMORYRESOURCES
Language=English
Appletalk protocol stack was unable to allocate memory. 
System may be low on memory.
.

MessageId=+1 Severity=Error SymbolicName=EVENT_ATALK_INTERNALERROR
Language=English
Not used
A fatal internal error was detected in the AppleTalk stack.
.

MessageId=+1 Severity=Warning SymbolicName=EVENT_ATALK_RECEPTION
Language=English
Unable to start packet reception on the adapter "%2".
Appletalk could not be started on the adapter.
.

MessageId=+1 Severity=Warning SymbolicName=EVENT_ATALK_PACKETINVALID_NOPORT
Language=English
Not used
Packet received contained errors and is not being accepted.
.

MessageId=+1 Severity=Warning SymbolicName=EVENT_ATALK_NETNUMBERCONFLICT
Language=English
Network Number conflicts with an existing router.
The error was ignored and processing was continued.
.

MessageId=+1 Severity=Error SymbolicName=EVENT_ATALK_UPONMP
Language=English
Not used
A uniprocessor driver was loaded a multiprocessor system. The driver could not load.
.

;
;#endif // _ATKMSG
;
