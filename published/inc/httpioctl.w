/*++

Copyright (c) 1998-2002 Microsoft Corporation

Module Name:

    HttpIoctl.h

Abstract:

    Contains structures and declarations shared between
    HTTP.SYS and HTTPAPI.DLL. Users of the HTTP API should include
    HttpP.h and not this file.

--*/


#ifndef __HTTP_IOCTL_H__
#define __HTTP_IOCTL_H__

#include <httpp.h>

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus


//
// Names of the object directory, devices, driver, and service.
//

#define HTTP_DIRECTORY_NAME         L"\\Device\\Http"
#define HTTP_CONTROL_DEVICE_NAME    L"\\Device\\Http\\Control"
#define HTTP_APP_POOL_DEVICE_NAME   L"\\Device\\Http\\AppPool"
#define HTTP_FILTER_DEVICE_NAME     L"\\Device\\Http\\Filter"
#define HTTP_SERVER_DEVICE_NAME     L"\\Device\\Http\\Server"



//
#define HTTP_DRIVER_NAME            L"HTTP.SYS"
#define HTTP_SERVICE_NAME           L"HTTP"
#define HTTP_FILTER_SERVICE_NAME    L"HTTPFilter"


//
// The current interface version number. This version number must be
// updated after any significant changes to the interface (especially
// structure changes).
//

#define HTTP_INTERFACE_VERSION_MAJOR  0x0001
#define HTTP_INTERFACE_VERSION_MINOR  0x0001


//
// The name of the EA (Extended Attribute) passed to NtCreateFile(). This
// allows us to pass version information at the time the driver is opened,
// allowing HTTP.SYS to immediately fail open requests with invalid version
// numbers.
//
// N.B. The EA name (including the terminator) must be a multiple of eight
// to ensure natural alignment of the HTTP_OPEN_PACKET structure used as
// the EA value.
//

//                                       7654321076543210
#define HTTP_OPEN_PACKET_NAME           "UlOpenPacket000"
#define HTTP_OPEN_PACKET_NAME_LENGTH    (sizeof(HTTP_OPEN_PACKET_NAME) - 1)
C_ASSERT( ((HTTP_OPEN_PACKET_NAME_LENGTH + 1) & 7) == 0 );


//
// The following structure is used as the value for the EA named above.
//

typedef struct HTTP_OPEN_PACKET
{
    USHORT             MajorVersion;
    USHORT             MinorVersion;
    USHORT             ServerNameLength;
    PWCHAR             pServerName;
    USHORT             ProxyNameLength;
    PWCHAR             pProxyName;
    USHORT             TransportAddressLength;
    PTRANSPORT_ADDRESS pTransportAddress;

} HTTP_OPEN_PACKET, *PHTTP_OPEN_PACKET;

//
// Structures for IOCTL_HTTP_QUERY_CONTROL_CHANNEL and
// IOCTL_HTTP_SET_CONTROL_CHANNEL.
//

typedef struct _HTTP_CONTROL_CHANNEL_INFO
{
    HTTP_CONTROL_CHANNEL_INFORMATION_CLASS InformationClass;

} HTTP_CONTROL_CHANNEL_INFO, *PHTTP_CONTROL_CHANNEL_INFO;


//
// Structures for IOCTL_HTTP_CREATE_CONFIG_GROUP,
// IOCTL_HTTP_DELETE_CONFIG_GROUP, IOCTL_HTTP_QUERY_CONFIG_GROUP,
// and IOCTL_HTTP_SET_CONFIG_GROUP.
//

typedef struct _HTTP_CONFIG_GROUP_INFO
{
    HTTP_CONFIG_GROUP_ID ConfigGroupId;
    HTTP_CONFIG_GROUP_INFORMATION_CLASS InformationClass;

} HTTP_CONFIG_GROUP_INFO, *PHTTP_CONFIG_GROUP_INFO;


//
// Structures for IOCTL_HTTP_ADD_URL_TO_CONFIG_GROUP and
// IOCTL_HTTP_REMOVE_URL_FROM_CONFIG_GROUP.
//

typedef enum _HTTP_URL_OPERATOR_TYPE
{
    HttpUrlOperatorTypeReservation,
    HttpUrlOperatorTypeRegistration,
    HttpUrlOperatorTypeMax

} HTTP_URL_OPERATOR_TYPE, *PHTTP_URL_OPERATOR_TYPE;

typedef struct _HTTP_CONFIG_GROUP_URL_INFO
{
    HTTP_URL_OPERATOR_TYPE   UrlType;
    HTTP_CONFIG_GROUP_ID     ConfigGroupId;
    HTTP_URL_CONTEXT         UrlContext;
    PSECURITY_DESCRIPTOR     pSecurityDescriptor;
    ULONG                    SecurityDescriptorLength;
    UNICODE_STRING           FullyQualifiedUrl;

} HTTP_CONFIG_GROUP_URL_INFO, *PHTTP_CONFIG_GROUP_URL_INFO;


//
// Structures for IOCTL_HTTP_REMOVE_ALL_URLS_FROM_CONFIG_GROUP.
//

typedef struct _HTTP_REMOVE_ALL_URLS_INFO
{
    HTTP_CONFIG_GROUP_ID ConfigGroupId;

} HTTP_REMOVE_ALL_URLS_INFO, *PHTTP_REMOVE_ALL_URLS_INFO;


//
// Structures for IOCTL_HTTP_CREATE_APP_POOL.
//

typedef struct _HTTP_CREATE_APP_POOL_INFO
{
    UNICODE_STRING AppPoolName;

} HTTP_CREATE_APP_POOL_INFO, *PHTTP_CREATE_APP_POOL_INFO;


//
// Structures for IOCTL_HTTP_QUERY_APP_POOL_INFORMATION and
// IOCTL_HTTP_SET_APP_POOL_INFORMATION.
//

typedef struct _HTTP_APP_POOL_INFO
{
    HTTP_APP_POOL_INFORMATION_CLASS InformationClass;

} HTTP_APP_POOL_INFO, *PHTTP_APP_POOL_INFO;


//
// Structures for IOCTL_HTTP_RECEIVE_HTTP_REQUEST and
// IOCTL_HTTP_RECEIVE_ENTITY_BODY.
//

typedef struct _HTTP_RECEIVE_REQUEST_INFO
{
    HTTP_REQUEST_ID RequestId;
    ULONG Flags;

} HTTP_RECEIVE_REQUEST_INFO, *PHTTP_RECEIVE_REQUEST_INFO;


//
// Structures for IOCTL_HTTP_SEND_HTTP_RESPONSE.
//

typedef struct _HTTP_SEND_HTTP_RESPONSE_INFO
{
    PHTTP_RESPONSE        pHttpResponse;
    USHORT                EntityChunkCount;
    PHTTP_DATA_CHUNK      pEntityChunks;
    HTTP_CACHE_POLICY     CachePolicy;
    HTTP_REQUEST_ID       RequestId;
    ULONG                 Flags;
    PHTTP_LOG_FIELDS_DATA pLogData;

} HTTP_SEND_HTTP_RESPONSE_INFO, *PHTTP_SEND_HTTP_RESPONSE_INFO;


//
// Structures for IOCTL_HTTP_FLUSH_RESPONSE_CACHE.
//

typedef struct _HTTP_FLUSH_RESPONSE_CACHE_INFO
{
    UNICODE_STRING FullyQualifiedUrl;
    ULONG          Flags;

} HTTP_FLUSH_RESPONSE_CACHE_INFO, *PHTTP_FLUSH_RESPONSE_CACHE_INFO;


//
// Structures for IOCTL_HTTP_WAIT_FOR_DISCONNECT.
//

typedef struct _HTTP_WAIT_FOR_DISCONNECT_INFO
{
    HTTP_CONNECTION_ID ConnectionId;

} HTTP_WAIT_FOR_DISCONNECT_INFO, *PHTTP_WAIT_FOR_DISCONNECT_INFO;


//
// Structures for IOCTL_HTTP_FILTER_RECEIVE_CLIENT_CERT
//

typedef struct _HTTP_FILTER_RECEIVE_CLIENT_CERT_INFO
{
    HTTP_CONNECTION_ID  ConnectionId;
    ULONG               Flags;

} HTTP_FILTER_RECEIVE_CLIENT_CERT_INFO, *PHTTP_FILTER_RECEIVE_CLIENT_CERT_INFO;


//
// Structure for IOCTL_HTTP_SEND_REQUEST (Input).
//

typedef struct  _HTTP_SEND_REQUEST_INPUT_INFO
{
    PHTTP_REQUEST_ID     pHttpRequestId;
    PHTTP_REQUEST        pHttpRequest;
    PULONG               pBytesTaken;
    ULONG                HttpRequestFlags;
    USHORT               RequestConfigCount;
    PHTTP_REQUEST_CONFIG pRequestConfig;

} HTTP_SEND_REQUEST_INPUT_INFO, *PHTTP_SEND_REQUEST_INPUT_INFO;


//
// Structure for IOCTL_HTTP_SEND_REQUEST_ENTITY_BODY_INFO.
//

typedef struct _HTTP_SEND_REQUEST_ENTITY_BODY_INFO
{
    HTTP_REQUEST_ID     RequestID;
    ULONG               Flags;
    USHORT              EntityChunkCount;
    PHTTP_DATA_CHUNK    pHttpEntityChunk;

} HTTP_SEND_REQUEST_ENTITY_BODY_INFO, *PHTTP_SEND_REQUEST_ENTITY_BODY_INFO;


//
// Structure for IOCTL_HTTP_RECEIVE_RESPONSE (Input). 
//

typedef struct _HTTP_RECEIVE_RESPONSE_INFO
{
    HTTP_REQUEST_ID RequestID;
    PULONG          pBytesTaken;
    ULONG           Flags;

} HTTP_RECEIVE_RESPONSE_INFO, *PHTTP_RECEIVE_RESPONSE_INFO;


//
// structure for IOCTL_HTTP_SET_SERVER_CONTEXT_INFORMATION & 
// IOCTL_HTTP_QUERY_SERVER_CONTEXT_INFORMATION.
//

typedef struct _HTTP_SERVER_CONTEXT_INFORMATION
{
    HTTP_SERVER_CONFIG_ID   ConfigID;
    PULONG                  pBytesTaken;
    PVOID                   pInputBuffer;
    ULONG                   InputBufferLength;

} HTTP_SERVER_CONTEXT_INFORMATION, *PHTTP_SERVER_CONTEXT_INFORMATION;


//
// Structures for IOCTL_HTTP_ADD_FRAGMENT_TO_CACHE.
//

typedef struct _HTTP_ADD_FRAGMENT_INFO
{
    UNICODE_STRING      FragmentName;
    HTTP_DATA_CHUNK     DataChunk;
    HTTP_CACHE_POLICY   CachePolicy;

} HTTP_ADD_FRAGMENT_INFO, *PHTTP_ADD_FRAGMENT_INFO;


//
// Structures for IOCTL_HTTP_READ_FRAGMENT_FROM_CACHE.
//

typedef struct _HTTP_READ_FRAGMENT_INFO
{
    UNICODE_STRING  FragmentName;
    HTTP_BYTE_RANGE ByteRange;

} HTTP_READ_FRAGMENT_INFO, *PHTTP_READ_FRAGMENT_INFO;


//
// HTTP IOCTL code definitions.
//
// N.B. To ensure the efficiency of the code generated by HTTP.SYS's
//      IOCTL dispatcher, these IOCTL codes should be contiguous
//      (no gaps).
//
// N.B. If new IOCTLs are added here, update the lookup table in devctrl.c.
//

#define _HTTP_CTL_CODE(request, method, access)             \
    CTL_CODE( FILE_DEVICE_NETWORK, request, method, access )

#define _HTTP_REQUEST(ioctl)                                \
   ((((ULONG)(ioctl)) >> 2) & 0x03FF)


//
// Server-side IOCTLs.
//

#define HTTP_QUERY_CONTROL_CHANNEL                          0
#define HTTP_SET_CONTROL_CHANNEL                            1
#define HTTP_CREATE_CONFIG_GROUP                            2
#define HTTP_DELETE_CONFIG_GROUP                            3
#define HTTP_QUERY_CONFIG_GROUP                             4
#define HTTP_SET_CONFIG_GROUP                               5
#define HTTP_ADD_URL_TO_CONFIG_GROUP                        6
#define HTTP_REMOVE_URL_FROM_CONFIG_GROUP                   7
#define HTTP_REMOVE_ALL_URLS_FROM_CONFIG_GROUP              8
#define HTTP_QUERY_APP_POOL_INFORMATION                     9
#define HTTP_SET_APP_POOL_INFORMATION                       10
#define HTTP_SHUTDOWN_APP_POOL                              11
#define HTTP_RECEIVE_HTTP_REQUEST                           12
#define HTTP_RECEIVE_ENTITY_BODY                            13
#define HTTP_SEND_HTTP_RESPONSE                             14
#define HTTP_SEND_ENTITY_BODY                               15
#define HTTP_FLUSH_RESPONSE_CACHE                           16
#define HTTP_WAIT_FOR_DEMAND_START                          17
#define HTTP_WAIT_FOR_DISCONNECT                            18
#define HTTP_FILTER_ACCEPT                                  19
#define HTTP_FILTER_CLOSE                                   20
#define HTTP_FILTER_RAW_READ                                21
#define HTTP_FILTER_RAW_WRITE                               22
#define HTTP_FILTER_APP_READ                                23
#define HTTP_FILTER_APP_WRITE                               24
#define HTTP_FILTER_RECEIVE_CLIENT_CERT                     25
#define HTTP_SHUTDOWN_FILTER_CHANNEL                        26
#define HTTP_GET_COUNTERS                                   27
#define HTTP_ADD_FRAGMENT_TO_CACHE                          28
#define HTTP_READ_FRAGMENT_FROM_CACHE                       29

//
// Client-side IOCTLs.
//

#define HTTP_SEND_REQUEST                                   30
#define HTTP_SEND_REQUEST_ENTITY_BODY                       31
#define HTTP_RECEIVE_RESPONSE                               32
#define HTTP_QUERY_SERVER_CONTEXT_INFORMATION               33 
#define HTTP_SET_SERVER_CONTEXT_INFORMATION                 34
#define HTTP_CANCEL_REQUEST                                 35

#define HTTP_NUM_IOCTLS                                     36


#define IOCTL_HTTP_QUERY_CONTROL_CHANNEL                    \
    _HTTP_CTL_CODE( HTTP_QUERY_CONTROL_CHANNEL,             \
                    METHOD_OUT_DIRECT,                      \
                    FILE_READ_ACCESS )

#define IOCTL_HTTP_SET_CONTROL_CHANNEL                      \
    _HTTP_CTL_CODE( HTTP_SET_CONTROL_CHANNEL,               \
                    METHOD_IN_DIRECT,                       \
                    FILE_WRITE_ACCESS )

#define IOCTL_HTTP_CREATE_CONFIG_GROUP                      \
    _HTTP_CTL_CODE( HTTP_CREATE_CONFIG_GROUP,               \
                    METHOD_BUFFERED,                        \
                    FILE_WRITE_ACCESS )

#define IOCTL_HTTP_DELETE_CONFIG_GROUP                      \
    _HTTP_CTL_CODE( HTTP_DELETE_CONFIG_GROUP,               \
                    METHOD_BUFFERED,                        \
                    FILE_WRITE_ACCESS )

#define IOCTL_HTTP_QUERY_CONFIG_GROUP                       \
    _HTTP_CTL_CODE( HTTP_QUERY_CONFIG_GROUP,                \
                    METHOD_OUT_DIRECT,                      \
                    FILE_READ_ACCESS )

#define IOCTL_HTTP_SET_CONFIG_GROUP                         \
    _HTTP_CTL_CODE( HTTP_SET_CONFIG_GROUP,                  \
                    METHOD_IN_DIRECT,                       \
                    FILE_WRITE_ACCESS )

#define IOCTL_HTTP_ADD_URL_TO_CONFIG_GROUP                  \
    _HTTP_CTL_CODE( HTTP_ADD_URL_TO_CONFIG_GROUP,           \
                    METHOD_BUFFERED,                        \
                    FILE_WRITE_ACCESS )

#define IOCTL_HTTP_REMOVE_URL_FROM_CONFIG_GROUP             \
    _HTTP_CTL_CODE( HTTP_REMOVE_URL_FROM_CONFIG_GROUP,      \
                    METHOD_BUFFERED,                        \
                    FILE_WRITE_ACCESS )

#define IOCTL_HTTP_REMOVE_ALL_URLS_FROM_CONFIG_GROUP        \
    _HTTP_CTL_CODE( HTTP_REMOVE_ALL_URLS_FROM_CONFIG_GROUP, \
                    METHOD_BUFFERED,                        \
                    FILE_WRITE_ACCESS )

#define IOCTL_HTTP_QUERY_APP_POOL_INFORMATION               \
    _HTTP_CTL_CODE( HTTP_QUERY_APP_POOL_INFORMATION,        \
                    METHOD_OUT_DIRECT,                      \
                    FILE_READ_ACCESS )

#define IOCTL_HTTP_SET_APP_POOL_INFORMATION                 \
    _HTTP_CTL_CODE( HTTP_SET_APP_POOL_INFORMATION,          \
                    METHOD_IN_DIRECT,                       \
                    FILE_WRITE_ACCESS )

#define IOCTL_HTTP_SHUTDOWN_APP_POOL                        \
    _HTTP_CTL_CODE( HTTP_SHUTDOWN_APP_POOL,                 \
                    METHOD_BUFFERED,                        \
                    FILE_READ_ACCESS )

#define IOCTL_HTTP_RECEIVE_HTTP_REQUEST                     \
    _HTTP_CTL_CODE( HTTP_RECEIVE_HTTP_REQUEST,              \
                    METHOD_OUT_DIRECT,                      \
                    FILE_READ_ACCESS )

#define IOCTL_HTTP_RECEIVE_ENTITY_BODY                      \
    _HTTP_CTL_CODE( HTTP_RECEIVE_ENTITY_BODY,               \
                    METHOD_OUT_DIRECT,                      \
                    FILE_READ_ACCESS )

#define IOCTL_HTTP_SEND_HTTP_RESPONSE                       \
    _HTTP_CTL_CODE( HTTP_SEND_HTTP_RESPONSE,                \
                    METHOD_NEITHER,                         \
                    FILE_READ_ACCESS )

#define IOCTL_HTTP_SEND_ENTITY_BODY                         \
    _HTTP_CTL_CODE( HTTP_SEND_ENTITY_BODY,                  \
                    METHOD_NEITHER,                         \
                    FILE_READ_ACCESS )

#define IOCTL_HTTP_FLUSH_RESPONSE_CACHE                     \
    _HTTP_CTL_CODE( HTTP_FLUSH_RESPONSE_CACHE,              \
                    METHOD_BUFFERED,                        \
                    FILE_READ_ACCESS )

#define IOCTL_HTTP_WAIT_FOR_DEMAND_START                    \
    _HTTP_CTL_CODE( HTTP_WAIT_FOR_DEMAND_START,             \
                    METHOD_BUFFERED,                        \
                    FILE_WRITE_ACCESS )

#define IOCTL_HTTP_WAIT_FOR_DISCONNECT                      \
    _HTTP_CTL_CODE( HTTP_WAIT_FOR_DISCONNECT,               \
                    METHOD_BUFFERED,                        \
                    FILE_READ_ACCESS )

#define IOCTL_HTTP_FILTER_ACCEPT                            \
    _HTTP_CTL_CODE( HTTP_FILTER_ACCEPT,                     \
                    METHOD_OUT_DIRECT,                      \
                    FILE_READ_ACCESS )

#define IOCTL_HTTP_FILTER_CLOSE                             \
    _HTTP_CTL_CODE( HTTP_FILTER_CLOSE,                      \
                    METHOD_BUFFERED,                        \
                    FILE_READ_ACCESS )

#define IOCTL_HTTP_FILTER_RAW_READ                          \
    _HTTP_CTL_CODE( HTTP_FILTER_RAW_READ,                   \
                    METHOD_OUT_DIRECT,                      \
                    FILE_READ_ACCESS )

#define IOCTL_HTTP_FILTER_RAW_WRITE                         \
    _HTTP_CTL_CODE( HTTP_FILTER_RAW_WRITE,                  \
                    METHOD_IN_DIRECT,                       \
                    FILE_READ_ACCESS )

#define IOCTL_HTTP_FILTER_APP_READ                          \
    _HTTP_CTL_CODE( HTTP_FILTER_APP_READ,                   \
                    METHOD_OUT_DIRECT,                      \
                    FILE_READ_ACCESS )

#define IOCTL_HTTP_FILTER_APP_WRITE                         \
    _HTTP_CTL_CODE( HTTP_FILTER_APP_WRITE,                  \
                    METHOD_IN_DIRECT,                       \
                    FILE_READ_ACCESS )

#define IOCTL_HTTP_FILTER_RECEIVE_CLIENT_CERT               \
    _HTTP_CTL_CODE( HTTP_FILTER_RECEIVE_CLIENT_CERT,        \
                    METHOD_OUT_DIRECT,                      \
                    FILE_READ_ACCESS )

#define IOCTL_HTTP_SHUTDOWN_FILTER_CHANNEL                  \
    _HTTP_CTL_CODE( HTTP_SHUTDOWN_FILTER_CHANNEL,           \
                    METHOD_BUFFERED,                        \
                    FILE_READ_ACCESS )

#define IOCTL_HTTP_GET_COUNTERS                             \
    _HTTP_CTL_CODE( HTTP_GET_COUNTERS,                      \
                    METHOD_OUT_DIRECT,                      \
                    FILE_READ_ACCESS )

#define IOCTL_HTTP_ADD_FRAGMENT_TO_CACHE                    \
    _HTTP_CTL_CODE( HTTP_ADD_FRAGMENT_TO_CACHE,             \
                    METHOD_BUFFERED,                        \
                    FILE_READ_ACCESS )

#define IOCTL_HTTP_READ_FRAGMENT_FROM_CACHE                 \
    _HTTP_CTL_CODE( HTTP_READ_FRAGMENT_FROM_CACHE,          \
                    METHOD_NEITHER,                         \
                    FILE_READ_ACCESS )

#define IOCTL_HTTP_SEND_REQUEST                             \
    _HTTP_CTL_CODE( HTTP_SEND_REQUEST,                      \
                    METHOD_OUT_DIRECT,                      \
                    FILE_READ_ACCESS )

#define IOCTL_HTTP_SEND_REQUEST_ENTITY_BODY                 \
    _HTTP_CTL_CODE( HTTP_SEND_REQUEST_ENTITY_BODY,          \
                    METHOD_IN_DIRECT,                       \
                    FILE_READ_ACCESS )

#define IOCTL_HTTP_RECEIVE_RESPONSE                         \
    _HTTP_CTL_CODE( HTTP_RECEIVE_RESPONSE,                  \
                    METHOD_OUT_DIRECT,                      \
                    FILE_READ_ACCESS )

#define IOCTL_HTTP_QUERY_SERVER_CONTEXT_INFORMATION         \
    _HTTP_CTL_CODE( HTTP_QUERY_SERVER_CONTEXT_INFORMATION,  \
                    METHOD_OUT_DIRECT,                      \
                    FILE_READ_ACCESS | FILE_WRITE_ACCESS )

#define IOCTL_HTTP_SET_SERVER_CONTEXT_INFORMATION           \
    _HTTP_CTL_CODE( HTTP_SET_SERVER_CONTEXT_INFORMATION,    \
                    METHOD_IN_DIRECT,                       \
                    FILE_READ_ACCESS | FILE_WRITE_ACCESS )

#define IOCTL_HTTP_CANCEL_REQUEST                           \
    _HTTP_CTL_CODE( HTTP_CANCEL_REQUEST,                    \
                    METHOD_BUFFERED,                        \
                    FILE_READ_ACCESS | FILE_WRITE_ACCESS )


#ifdef __cplusplus
}   // extern "C"
#endif  // __cplusplus


#endif // __HTTP_IOCTL_H__
