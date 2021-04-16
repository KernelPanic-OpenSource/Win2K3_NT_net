;/*++ BUILD Version: 0001    // Increment this if a change has global effects
;
;Copyright (c) 1994  Microsoft Corporation
;
;Module Name:
;
;    dhcpmsg.h
;
;Abstract:
;
;    Definitions for DHCP messages.
;
;Author:
;
;    Manny Weiser 8-Dec-1992
;
;Revision History:
;
;Notes:
;
;    This file is generated by the MC tool from the dhcpmsg.mc file.
;
;--*/
;
;#ifndef _DHCPMESSAGE_
;#define _DHCPMESSAGE_
;

;//
;//  1000 - 1099   Reserved for DHCP client events.
;//

MessageId=1000 SymbolicName=EVENT_LEASE_TERMINATED
Language=English
Your computer has lost the lease to its IP address %2 on the
Network Card with network address %1.
.

MessageId=+1 SymbolicName=EVENT_FAILED_TO_OBTAIN_LEASE
Language=English
Your computer was not assigned an address from the network (by the DHCP
Server) for the Network Card with network address %1.  The following error
occurred: %n%2.
Your computer will continue to try and obtain an address on its own from
the network address (DHCP) server.
.

MessageId=+1 SymbolicName=EVENT_NACK_LEASE
Language=English
The IP address lease %1 for the Network Card with network address %2 has been
denied by the DHCP server %3 (The DHCP Server sent a DHCPNACK message).
.

MessageId=+1 SymbolicName=EVENT_FAILED_TO_RENEW
Language=English
Your computer was not able to renew its address from the network (from the
DHCP Server) for the Network Card with network address %1.  The following
error occurred: %n%2.
Your computer will continue to try and obtain an address on its own from
the network address (DHCP) server.
.

MessageId=+1 SymbolicName=EVENT_DHCP_SHUTDOWN
Language=English
The DHCP Client service is shutting down. The following error occurred : %n%1
.

MessageId=+1 SymbolicName=EVENT_ADDRESS_CONFLICT
Language=English
Your computer has detected that the IP address %1 for the Network Card
with network address %2 is already in use on the network.
Your computer will automatically attempt to obtain a different address.
.

MessageID=+1 SymbolicName=EVENT_IPAUTOCONFIGURATION_FAILED
Language=English
Your computer was unable to automatically configure the IP parameters for
the Network Card with the network address %1.  The following error occurred
during configuration: %2.
.

MessageID=+1 SymbolicName=EVENT_IPAUTOCONFIGURATION_SUCCEEDED
Language=English
Your computer has automatically configured the IP address for the Network
Card with network address %1.  The IP address being used is %2.
.

MessageID=+1 SymbolicName=EVENT_COULD_NOT_INITIALISE_INTERFACE
Language=English
Your computer was unable to initialize a Network Interface attached
to the system. The error code is: %1.
.

MessageID=+1 SymbolicName=EVENT_NET_ERROR
Language=English
A network error occurred when trying to send a message. The error code is: %1.
.

;//
;// 1100 - 1199   Reserved for User messages
;//

;//
;// These are MASSIVELY changed for Nt 5.0 Beta2
;//

MessageId=1100 SymbolicName=MESSAGE_POPUP_TITLE
Language=English
Network%0
.

MessageId=+1 SymbolicName=MESSAGE_FAILED_TO_OBTAIN_LEASE
Language=English
Your computer was not assigned an address from the network. It
may require a network address for full access to the network.

It will continue to try and obtain an address on its own from the
network address server (DHCP).

Either your computer is not physically connected to the network or
the server that assigns addresses is not responding. You may check
your network cable connection to diagnose the problem.
.

MessageId=+2 SymbolicName=MESSAGE_SUCCESSFUL_LEASE
Language=English
Your computer was successfully assigned an address from the network,
and it can now connect to other computers.
.


;#endif // _DHCPMESSAGE_
