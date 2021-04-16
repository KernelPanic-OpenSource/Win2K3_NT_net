/*++ BUILD Version: 0001    // Increment this if a change has global effects

Copyright (c) 1994  Microsoft Corporation

Module Name:

    dhcpmsg.h

Abstract:

    Definitions for DHCP messages.

Author:

    Manny Weiser 8-Dec-1992

Revision History:

Notes:

    This file is generated by the MC tool from the dhcpmsg.mc file.

--*/

#ifndef _DHCPMESSAGE_
#define _DHCPMESSAGE_

//
//  1000 - 1099   Reserved for DHCP client events.
//
//
//  Values are 32 bit values layed out as follows:
//
//   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
//   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//  +---+-+-+-----------------------+-------------------------------+
//  |Sev|C|R|     Facility          |               Code            |
//  +---+-+-+-----------------------+-------------------------------+
//
//  where
//
//      Sev - is the severity code
//
//          00 - Success
//          01 - Informational
//          10 - Warning
//          11 - Error
//
//      C - is the Customer code flag
//
//      R - is a reserved bit
//
//      Facility - is the facility code
//
//      Code - is the facility's status code
//
//
// Define the facility codes
//


//
// Define the severity codes
//


//
// MessageId: EVENT_FAILED_TO_INITIALIZE
//
// MessageText:
//
//  Failed to initialize the card with network address %1.
//
#define EVENT_FAILED_TO_INITIALIZE       1000L

//
// MessageId: EVENT_LEASE_TERMINATED
//
// MessageText:
//
//  DHCP is unable to renew a lease for the card with network address %1.
//  The lease for IP address %2 has been terminated.
//
#define EVENT_LEASE_TERMINATED           1001L

//
// MessageId: EVENT_UNKNOWN_DHCP_OPTION
//
// MessageText:
//
//  DHCP received an unknown option %1 of length %2. The raw option data is
//  given below.
//
#define EVENT_UNKNOWN_DHCP_OPTION        1002L

//
// MessageId: EVENT_FAILED_TO_OBTAIN_LEASE
//
// MessageText:
//
//  DHCP failed to obtain a lease for the card with network address %1.
//  The following error occurred : %n%2
//
#define EVENT_FAILED_TO_OBTAIN_LEASE     1003L

//
// MessageId: EVENT_NACK_LEASE
//
// MessageText:
//
//  DHCP IP address lease %1 for the card with network address %2 has been
//  denied.
//
#define EVENT_NACK_LEASE                 1004L

//
// MessageId: EVENT_FAILED_TO_RENEW
//
// MessageText:
//
//  DHCP failed to renew a lease for the card with network address %1. The
//  following error occurred : %n%2
//
#define EVENT_FAILED_TO_RENEW            1005L

//
// MessageId: EVENT_DHCP_SHUTDOWN
//
// MessageText:
//
//  DHCP service is shutting down. The following error occurred : %n%1
//
#define EVENT_DHCP_SHUTDOWN              1006L

//
// MessageId: EVENT_FAILED_TO_OBTAIN_LEASE_MULTI_HOME
//
// MessageText:
//
//  DHCP failed to obtain a lease for the card with network address %1.
//  On multi-homed machine all subsequent adapters will also fail to obtain
//  lease. The following error occurred : %n%2
//
#define EVENT_FAILED_TO_OBTAIN_LEASE_MULTI_HOME 1007L

//
// MessageId: EVENT_ADDRESS_CONFLICT
//
// MessageText:
//
//  The IP address %1 for the card with network address %2 is already in use
//  on the network.  The DHCP client will attempt to obtain a new address.
//
#define EVENT_ADDRESS_CONFLICT           1008L

//
// MessageId: EVENT_IPAUTOCONFIGURATION_FAILED
//
// MessageText:
//
//  Windows NT was unable to automatically configure the IP parameters for the network interface identified by the physical address %1.  The following error occurred during configuration: %2.
//
#define EVENT_IPAUTOCONFIGURATION_FAILED 1009L

//
// MessageId: EVENT_IPAUTOCONFIGURATION_SUCCEEDED
//
// MessageText:
//
//  Windows NT was unable to reach a DHCP server, has automatically configured the IP parameters identified by
//  the physical address %1.  The IP address being used is %2.
//
#define EVENT_IPAUTOCONFIGURATION_SUCCEEDED 1010L

//
// 1100 - 1199   Reserved for User messages
//
//
// MessageId: MESSAGE_FAILED_TO_INITIALIZE
//
// MessageText:
//
//  The DHCP client could not obtain an IP address.
//  
//  If you want to see DHCP messages in the future, choose YES. If you do not want to see DHCP messages choose NO.
//
#define MESSAGE_FAILED_TO_INITIALIZE     1100L

//
// MessageId: MESSAGE_LEASE_TERMINATED
//
// MessageText:
//
//  The DHCP client lost the lease of the IP Address %1. The network is brought down.
//  
//  If you want to see DHCP messages in the future, choose YES. Otherwise choose NO.
//
#define MESSAGE_LEASE_TERMINATED         1101L

//
// MessageId: MESSAGE_FAILED_TO_OBTAIN_LEASE
//
// MessageText:
//
//  The DHCP client could not obtain an IP address.
//  
//  If you want to see DHCP messages in the future, choose YES. Otherwise choose NO.
//
#define MESSAGE_FAILED_TO_OBTAIN_LEASE   1102L

//
// MessageId: MESSAGE_FAILED_TO_RENEW_LEASE
//
// MessageText:
//
//  The DHCP client could not renew the lease of the IP Address %1. Your lease is valid until %2. DHCP will try to renew the lease before it expires.
//  
//  If you want to see DHCP messages in the future, choose YES. Otherwise choose NO.
//
#define MESSAGE_FAILED_TO_RENEW_LEASE    1103L

//
// MessageId: MESSAGE_SUCCESSFUL_LEASE
//
// MessageText:
//
//  The DHCP client successfully leased the IP address %1. Your lease is valid until %2.
//  
//  If you want to see DHCP messages in the future, choose YES. Otherwise choose NO.
//
#define MESSAGE_SUCCESSFUL_LEASE         1104L

//
// MessageId: MESSAGE_SUCCESSFUL_RENEW
//
// MessageText:
//
//  The DHCP client successfully renewed the IP address %1. Your lease is valid until %2.
//  
//  If you want to see DHCP messages in the future, choose YES. Otherwise choose NO.
//
#define MESSAGE_SUCCESSFUL_RENEW         1105L

//
// MessageId: MESSAGE_POPUP_TITLE
//
// MessageText:
//
//  DHCP Client%0
//
#define MESSAGE_POPUP_TITLE              1106L

//
// MessageId: MESSAGE_ADDRESS_CONFLICT
//
// MessageText:
//
//  The DHCP client obtained an IP address that is already in use on the network.  The local
//  interface will be disabled until the DHCP client can obtain a valid address.
//  
//  If you want to see DHCP messages in the future, choose YES. Otherwise choose NO.
//
#define MESSAGE_ADDRESS_CONFLICT         1107L

//
// MessageId: MESSAGE_IPAUTOCONFIGURATION_FAILED
//
// MessageText:
//
//  Windows NT was unable to automatically configure the IP parameters for one or more network interfaces.
//  
//  If you want to see DHCP messages in the future, choose YES. Otherwise choose NO.
//
#define MESSAGE_IPAUTOCONFIGURATION_FAILED 1108L

//
// MessageId: MESSAGE_IPAUTOCONFIGURATION_SUCCEEDED
//
// MessageText:
//
//  Windows NT was unable to reach a DHCP server, and has automatically configured the IP address %1.
//  DHCP would automatically try to reach a DHCP server in the background.
//  
//  If you want to see DHCP messages in the future, choose YES. Otherwise choose NO.
//
#define MESSAGE_IPAUTOCONFIGURATION_SUCCEEDED 1109L

#endif // _DHCPMESSAGE_
