/*   
Copyright 2006 - 2011 Intel Corporation

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

char const * UPnPXmlLocation = "/upnp.xml";

#if defined(WIN32) || defined(_WIN32_WCE)
#	ifndef MICROSTACK_NO_STDAFX
#		include "stdafx.h"
#	endif
char* UPnPPLATFORM = "WINDOWS";
#elif defined(__SYMBIAN32__)
char* UPnPPLATFORM = "SYMBIAN";
#else
char* UPnPPLATFORM = "POSIX";
#endif

#if defined(WIN32)
#define snprintf sprintf_s
#define _CRTDBG_MAP_ALLOC
#endif

#if defined(WINSOCK2)
#	include <winsock2.h>
#	include <ws2tcpip.h>
#elif defined(WINSOCK1)
#	include <winsock.h>
#	include <wininet.h>
#endif

#include "ILibParsers.h"
#include "UPnPMicroStack.h"
#include "ILibWebServer.h"
#include "ILibWebClient.h"
#include "ILibAsyncSocket.h"
#include "ILibAsyncUDPSocket.h"

#if defined(WIN32) && !defined(_WIN32_WCE)
#include <crtdbg.h>
#endif

#define UPNP_SSDP_TTL 4
#define UPNP_HTTP_MAXSOCKETS 5
#define UPNP_MAX_SSDP_HEADER_SIZE 4096
#define UPNP_PORT 1900
#define UPNP_MCASTv4_GROUP "239.255.255.250"
#define UPNP_MCASTv6_GROUP "FF05:0:0:0:0:0:0:C" // Site local
#define UPNP_MCASTv6_GROUPB "[FF05:0:0:0:0:0:0:C]"
#define UPNP_MCASTv6_LINK_GROUP "FF02:0:0:0:0:0:0:C" // Link local
#define UPNP_MCASTv6_LINK_GROUPB "[FF02:0:0:0:0:0:0:C]"
#define UPnP_MAX_SUBSCRIPTION_TIMEOUT 7200
#define UPnPMIN(a,b) (((a)<(b))?(a):(b))

#define LVL3DEBUG(x)
#define INET_SOCKADDR_LENGTH(x) ((x==AF_INET6?sizeof(struct sockaddr_in6):sizeof(struct sockaddr_in)))

#if defined(WIN32)
#pragma warning( push, 3 ) // warning C4310: cast truncates constant value
#endif
struct UPnP_StateVariableTable_AVTransport UPnP_StateVariableTable_AVTransport_Impl = 
{
	{
		(char)0x3A,(char)0x3C,(char)0x73,(char)0x74,(char)0x61,(char)0x74,(char)0x65,(char)0x56,(char)0x61,(char)0x72
		,(char)0x69,(char)0x61,(char)0x62,(char)0x6C,(char)0x65,(char)0x20,(char)0x73,(char)0x65,(char)0x6E,(char)0x64
		,(char)0x45,(char)0x76,(char)0x65,(char)0x6E,(char)0x74,(char)0x73,(char)0x3D,(char)0x22,(char)0x6E,(char)0x6F
		,(char)0x22,(char)0x3E,(char)0x3C,(char)0x6E,(char)0x61,(char)0x6D,(char)0x65,(char)0x3E,(char)0x41,(char)0x5F
		,(char)0x41,(char)0x52,(char)0x47,(char)0x5F,(char)0x54,(char)0x59,(char)0x50,(char)0x45,(char)0x5F,(char)0x53
		,(char)0x65,(char)0x65,(char)0x6B,(char)0x4D,(char)0x6F,(char)0x64,(char)0x65,(char)0x3C,(char)0x2F,(char)0x85
		,(char)0x06,(char)0x12,(char)0x3C,(char)0x64,(char)0x61,(char)0x74,(char)0x61,(char)0x54,(char)0x79,(char)0x70
		,(char)0x65,(char)0x3E,(char)0x73,(char)0x74,(char)0x72,(char)0x69,(char)0x6E,(char)0x67,(char)0x3C,(char)0x2F
		,(char)0x49,(char)0x04,(char)0x11,(char)0x3C,(char)0x61,(char)0x6C,(char)0x6C,(char)0x6F,(char)0x77,(char)0x65
		,(char)0x64,(char)0x56,(char)0x61,(char)0x6C,(char)0x75,(char)0x65,(char)0x4C,(char)0x69,(char)0x73,(char)0x74
		,(char)0x8E,(char)0x04,(char)0x0C,(char)0x3E,(char)0x41,(char)0x42,(char)0x53,(char)0x5F,(char)0x43,(char)0x4F
		,(char)0x55,(char)0x4E,(char)0x54,(char)0x3C,(char)0x2F,(char)0x0D,(char)0x06,(char)0x00,(char)0x8E,(char)0x09
		,(char)0x08,(char)0x54,(char)0x52,(char)0x41,(char)0x43,(char)0x4B,(char)0x5F,(char)0x4E,(char)0x52,(char)0x5D
		,(char)0x09,(char)0x08,(char)0x52,(char)0x45,(char)0x4C,(char)0x5F,(char)0x54,(char)0x49,(char)0x4D,(char)0x45
		,(char)0x90,(char)0x12,(char)0x01,(char)0x2F,(char)0xD2,(char)0x20,(char)0x01,(char)0x2F,(char)0x0D,(char)0x3C
		,(char)0x01,(char)0x3E,(char)0xE5,(char)0x3F,(char)0x12,(char)0x4E,(char)0x65,(char)0x78,(char)0x74,(char)0x41
		,(char)0x56,(char)0x54,(char)0x72,(char)0x61,(char)0x6E,(char)0x73,(char)0x70,(char)0x6F,(char)0x72,(char)0x74
		,(char)0x55,(char)0x52,(char)0x49,(char)0xA3,(char)0x3F,(char)0x00,(char)0x74,(char)0x1A,(char)0x15,(char)0x50
		,(char)0x6C,(char)0x61,(char)0x79,(char)0x62,(char)0x61,(char)0x63,(char)0x6B,(char)0x53,(char)0x74,(char)0x6F
		,(char)0x72,(char)0x61,(char)0x67,(char)0x65,(char)0x4D,(char)0x65,(char)0x64,(char)0x69,(char)0x75,(char)0x6D
		,(char)0xBF,(char)0x5A,(char)0x0A,(char)0x75,(char)0x65,(char)0x3E,(char)0x55,(char)0x4E,(char)0x4B,(char)0x4E
		,(char)0x4F,(char)0x57,(char)0x4E,(char)0x1D,(char)0x5A,(char)0x05,(char)0x43,(char)0x44,(char)0x2D,(char)0x44
		,(char)0x41,(char)0x9D,(char)0x62,(char)0x09,(char)0x44,(char)0x56,(char)0x44,(char)0x2D,(char)0x56,(char)0x49
		,(char)0x44,(char)0x45,(char)0x4F,(char)0x1D,(char)0x6C,(char)0x03,(char)0x48,(char)0x44,(char)0x44,(char)0x1D
		,(char)0x74,(char)0x07,(char)0x4E,(char)0x45,(char)0x54,(char)0x57,(char)0x4F,(char)0x52,(char)0x4B,(char)0xBF
		,(char)0x6A,(char)0x00,(char)0x58,(char)0xAA,(char)0x06,(char)0x52,(char)0x65,(char)0x63,(char)0x6F,(char)0x72
		,(char)0x64,(char)0xFF,(char)0x4F,(char)0x00,(char)0x50,(char)0xAA,(char)0x0F,(char)0x4E,(char)0x4F,(char)0x54
		,(char)0x5F,(char)0x49,(char)0x4D,(char)0x50,(char)0x4C,(char)0x45,(char)0x4D,(char)0x45,(char)0x4E,(char)0x54
		,(char)0x45,(char)0x44,(char)0x7F,(char)0x99,(char)0x00,(char)0x23,(char)0xD9,(char)0x0A,(char)0x49,(char)0x6E
		,(char)0x73,(char)0x74,(char)0x61,(char)0x6E,(char)0x63,(char)0x65,(char)0x49,(char)0x44,(char)0x91,(char)0xD9
		,(char)0x03,(char)0x75,(char)0x69,(char)0x34,(char)0x7F,(char)0x99,(char)0x0D,(char)0x3E,(char)0x43,(char)0x75
		,(char)0x72,(char)0x72,(char)0x65,(char)0x6E,(char)0x74,(char)0x54,(char)0x72,(char)0x61,(char)0x63,(char)0x6B
		,(char)0x20,(char)0x18,(char)0x00,(char)0xCC,(char)0xF0,(char)0x12,(char)0x52,(char)0x61,(char)0x6E,(char)0x67
		,(char)0x65,(char)0x3E,(char)0x3C,(char)0x6D,(char)0x69,(char)0x6E,(char)0x69,(char)0x6D,(char)0x75,(char)0x6D
		,(char)0x3E,(char)0x30,(char)0x3C,(char)0x2F,(char)0xC8,(char)0x02,(char)0x04,(char)0x3C,(char)0x6D,(char)0x61
		,(char)0x78,(char)0x05,(char)0x05,(char)0x07,(char)0x36,(char)0x35,(char)0x35,(char)0x33,(char)0x35,(char)0x3C
		,(char)0x2F,(char)0xC8,(char)0x03,(char)0x09,(char)0x3C,(char)0x73,(char)0x74,(char)0x65,(char)0x70,(char)0x3E
		,(char)0x31,(char)0x3C,(char)0x2F,(char)0x05,(char)0x02,(char)0x02,(char)0x3C,(char)0x2F,(char)0x93,(char)0x13
		,(char)0x00,(char)0x7F,(char)0x30,(char)0x09,(char)0x6B,(char)0x44,(char)0x75,(char)0x72,(char)0x61,(char)0x74
		,(char)0x69,(char)0x6F,(char)0x6E,(char)0x7F,(char)0xE4,(char)0x00,(char)0x59,(char)0xE4,(char)0x07,(char)0x6F
		,(char)0x73,(char)0x73,(char)0x69,(char)0x62,(char)0x6C,(char)0x65,(char)0x53,(char)0xE6,(char)0x01,(char)0x61
		,(char)0x3F,(char)0xE6,(char)0x00,(char)0x3F,(char)0xE6,(char)0x00,(char)0x3F,(char)0xE6,(char)0x00,(char)0x3F
		,(char)0xE6,(char)0x00,(char)0x37,(char)0x9D,(char)0x00,(char)0xC4,(char)0x51,(char)0x04,(char)0x4D,(char)0x6F
		,(char)0x64,(char)0x65,(char)0x3F,(char)0xE5,(char)0x00,(char)0x05,(char)0xE5,(char)0x04,(char)0x52,(char)0x4D
		,(char)0x41,(char)0x4C,(char)0x9D,(char)0x4E,(char)0x07,(char)0x53,(char)0x48,(char)0x55,(char)0x46,(char)0x46
		,(char)0x4C,(char)0x45,(char)0x9D,(char)0x57,(char)0x0A,(char)0x52,(char)0x45,(char)0x50,(char)0x45,(char)0x41
		,(char)0x54,(char)0x5F,(char)0x41,(char)0x4C,(char)0x4C,(char)0xA3,(char)0xF5,(char)0x07,(char)0x64,(char)0x65
		,(char)0x66,(char)0x61,(char)0x75,(char)0x6C,(char)0x74,(char)0x4E,(char)0x20,(char)0x00,(char)0x4D,(char)0x05
		,(char)0x00,(char)0x75,(char)0xFE,(char)0x0F,(char)0x54,(char)0x72,(char)0x61,(char)0x6E,(char)0x73,(char)0x70
		,(char)0x6F,(char)0x72,(char)0x74,(char)0x53,(char)0x74,(char)0x61,(char)0x74,(char)0x75,(char)0x73,(char)0xFF
		,(char)0x95,(char)0x05,(char)0x75,(char)0x65,(char)0x3E,(char)0x4F,(char)0x4B,(char)0x9D,(char)0x94,(char)0x0E
		,(char)0x45,(char)0x52,(char)0x52,(char)0x4F,(char)0x52,(char)0x5F,(char)0x4F,(char)0x43,(char)0x43,(char)0x55
		,(char)0x52,(char)0x52,(char)0x45,(char)0x44,(char)0x7F,(char)0x7C,(char)0x00,(char)0x1F,(char)0xE9,(char)0x0C
		,(char)0x52,(char)0x65,(char)0x63,(char)0x6F,(char)0x72,(char)0x64,(char)0x51,(char)0x75,(char)0x61,(char)0x6C
		,(char)0x69,(char)0x74,(char)0xBF,(char)0x7E,(char)0x00,(char)0x8A,(char)0x7E,(char)0x0B,(char)0x54,(char)0x5F
		,(char)0x49,(char)0x4D,(char)0x50,(char)0x4C,(char)0x45,(char)0x4D,(char)0x45,(char)0x4E,(char)0x54,(char)0x3F
		,(char)0x30,(char)0x00,(char)0x67,(char)0x65,(char)0x00,(char)0x3F,(char)0xAC,(char)0x00,(char)0x45,(char)0xA3
		,(char)0x05,(char)0x54,(char)0x4F,(char)0x50,(char)0x50,(char)0x45,(char)0xDE,(char)0xE0,(char)0x0E,(char)0x50
		,(char)0x41,(char)0x55,(char)0x53,(char)0x45,(char)0x44,(char)0x5F,(char)0x50,(char)0x4C,(char)0x41,(char)0x59
		,(char)0x42,(char)0x41,(char)0x43,(char)0x5E,(char)0x71,(char)0x00,(char)0x44,(char)0x09,(char)0x03,(char)0x49
		,(char)0x4E,(char)0x47,(char)0xDD,(char)0xFC,(char)0x0A,(char)0x54,(char)0x52,(char)0x41,(char)0x4E,(char)0x53
		,(char)0x49,(char)0x54,(char)0x49,(char)0x4F,(char)0x4E,(char)0xA0,(char)0x0A,(char)0x10,(char)0x4E,(char)0x4F
		,(char)0x5F,(char)0x4D,(char)0x45,(char)0x44,(char)0x49,(char)0x41,(char)0x5F,(char)0x50,(char)0x52,(char)0x45
		,(char)0x53,(char)0x45,(char)0x4E,(char)0x54,(char)0x7F,(char)0x85,(char)0x00,(char)0x5F,(char)0x85,(char)0x08
		,(char)0x54,(char)0x72,(char)0x61,(char)0x63,(char)0x6B,(char)0x55,(char)0x52,(char)0x49,(char)0xA3,(char)0xBA
		,(char)0x00,(char)0x34,(char)0xD4,(char)0x02,(char)0x41,(char)0x56,(char)0x89,(char)0xD4,(char)0x00,(char)0x7F
		,(char)0x19,(char)0x00,(char)0x5C,(char)0x19,(char)0x16,(char)0x62,(char)0x73,(char)0x6F,(char)0x6C,(char)0x75
		,(char)0x74,(char)0x65,(char)0x43,(char)0x6F,(char)0x75,(char)0x6E,(char)0x74,(char)0x65,(char)0x72,(char)0x50
		,(char)0x6F,(char)0x73,(char)0x69,(char)0x74,(char)0x69,(char)0x6F,(char)0x6E,(char)0x51,(char)0xEF,(char)0x02
		,(char)0x69,(char)0x34,(char)0xFF,(char)0x33,(char)0x09,(char)0x3E,(char)0x50,(char)0x6F,(char)0x73,(char)0x73
		,(char)0x69,(char)0x62,(char)0x6C,(char)0x65,(char)0xD1,(char)0xD2,(char)0x01,(char)0x73,(char)0x3F,(char)0xD3
		,(char)0x00,(char)0x3F,(char)0xD3,(char)0x00,(char)0x20,(char)0xD3,(char)0x03,(char)0x79,(char)0x65,(char)0x73
		,(char)0x48,(char)0xD3,(char)0x0A,(char)0x4C,(char)0x61,(char)0x73,(char)0x74,(char)0x43,(char)0x68,(char)0x61
		,(char)0x6E,(char)0x67,(char)0x65,(char)0xFF,(char)0x7C,(char)0x00,(char)0x98,(char)0xEB,(char)0x04,(char)0x4E
		,(char)0x65,(char)0x78,(char)0x74,(char)0xCE,(char)0x7D,(char)0x08,(char)0x4D,(char)0x65,(char)0x74,(char)0x61
		,(char)0x44,(char)0x61,(char)0x74,(char)0x61,(char)0x3F,(char)0x99,(char)0x00,(char)0x26,(char)0x99,(char)0x00
		,(char)0x7F,(char)0x1B,(char)0x00,(char)0x61,(char)0xB4,(char)0x14,(char)0x5F,(char)0x41,(char)0x52,(char)0x47
		,(char)0x5F,(char)0x54,(char)0x59,(char)0x50,(char)0x45,(char)0x5F,(char)0x53,(char)0x65,(char)0x65,(char)0x6B
		,(char)0x54,(char)0x61,(char)0x72,(char)0x67,(char)0x65,(char)0x74,(char)0x3F,(char)0xB6,(char)0x00,(char)0x20
		,(char)0xB6,(char)0x04,(char)0x54,(char)0x69,(char)0x6D,(char)0x65,(char)0x59,(char)0xB5,(char)0x00,(char)0x3F
		,(char)0xEA,(char)0x00,(char)0x07,(char)0xEA,(char)0x00,(char)0x46,(char)0xB4,(char)0x10,(char)0x4D,(char)0x65
		,(char)0x64,(char)0x69,(char)0x75,(char)0x6D,(char)0x57,(char)0x72,(char)0x69,(char)0x74,(char)0x65,(char)0x53
		,(char)0x74,(char)0x61,(char)0x74,(char)0x75,(char)0xBF,(char)0xB5,(char)0x00,(char)0xBF,(char)0xB5,(char)0x00
		,(char)0x2B,(char)0xE6,(char)0x07,(char)0x43,(char)0x75,(char)0x72,(char)0x72,(char)0x65,(char)0x6E,(char)0x74
		,(char)0x49,(char)0x9D,(char)0x02,(char)0x41,(char)0x63,(char)0x04,(char)0x4B,(char)0x01,(char)0x73,(char)0x7F
		,(char)0x4B,(char)0x00,(char)0x5A,(char)0x4B,(char)0x05,(char)0x6C,(char)0x61,(char)0x74,(char)0x69,(char)0x76
		,(char)0x3F,(char)0x66,(char)0x00,(char)0x6F,(char)0x36,(char)0x02,(char)0x63,(char)0x6B,(char)0xFF,(char)0xD1
		,(char)0x00,(char)0x27,(char)0x51,(char)0x00,(char)0x04,(char)0x81,(char)0x05,(char)0x61,(char)0x44,(char)0x75
		,(char)0x72,(char)0x61,(char)0xBF,(char)0x9B,(char)0x00,(char)0x9C,(char)0xEC,(char)0x00,(char)0x09,(char)0xEC
		,(char)0x09,(char)0x50,(char)0x6C,(char)0x61,(char)0x79,(char)0x53,(char)0x70,(char)0x65,(char)0x65,(char)0x64
		,(char)0x7F,(char)0x9A,(char)0x04,(char)0x75,(char)0x65,(char)0x3E,(char)0x31,(char)0xFF,(char)0x96,(char)0x00
		,(char)0x60,(char)0x7B,(char)0x07,(char)0x43,(char)0x6F,(char)0x75,(char)0x6E,(char)0x74,(char)0x65,(char)0x72
		,(char)0x19,(char)0xE2,(char)0x02,(char)0x69,(char)0x34,(char)0xFF,(char)0xFB,(char)0x09,(char)0x3E,(char)0x4E
		,(char)0x75,(char)0x6D,(char)0x62,(char)0x65,(char)0x72,(char)0x4F,(char)0x66,(char)0x45,(char)0x7B,(char)0x00
		,(char)0xD2,(char)0xDE,(char)0x03,(char)0x75,(char)0x69,(char)0x34,(char)0x18,(char)0xDE,(char)0x12,(char)0x52
		,(char)0x61,(char)0x6E,(char)0x67,(char)0x65,(char)0x3E,(char)0x3C,(char)0x6D,(char)0x69,(char)0x6E,(char)0x69
		,(char)0x6D,(char)0x75,(char)0x6D,(char)0x3E,(char)0x30,(char)0x3C,(char)0x2F,(char)0xC8,(char)0x02,(char)0x04
		,(char)0x3C,(char)0x6D,(char)0x61,(char)0x78,(char)0x05,(char)0x05,(char)0x07,(char)0x36,(char)0x35,(char)0x35
		,(char)0x33,(char)0x35,(char)0x3C,(char)0x2F,(char)0xC8,(char)0x03,(char)0x02,(char)0x3C,(char)0x2F,(char)0x13
		,(char)0x10,(char)0x00,(char)0xB4,(char)0xDE,(char)0x15,(char)0x50,(char)0x6F,(char)0x73,(char)0x73,(char)0x69
		,(char)0x62,(char)0x6C,(char)0x65,(char)0x52,(char)0x65,(char)0x63,(char)0x6F,(char)0x72,(char)0x64,(char)0x53
		,(char)0x74,(char)0x6F,(char)0x72,(char)0x61,(char)0x67,(char)0x65,(char)0x05,(char)0x91,(char)0x00,(char)0xFF
		,(char)0x74,(char)0x12,(char)0x75,(char)0x65,(char)0x3E,(char)0x4E,(char)0x4F,(char)0x54,(char)0x5F,(char)0x49
		,(char)0x4D,(char)0x50,(char)0x4C,(char)0x45,(char)0x4D,(char)0x45,(char)0x4E,(char)0x54,(char)0x45,(char)0x44
		,(char)0x72,(char)0x78
	},
	1042,
	4944
};
struct UPnP_StateVariable_AVTransport_A_ARG_TYPE_SeekMode UPnP_StateVariable_AVTransport_A_ARG_TYPE_SeekMode_Impl = 
{
	0,
	90,
	90,
	18,
	220,
	19,
	{"ABS_COUNT","TRACK_NR","REL_TIME",NULL},
	239,
	16
};
struct UPnP_StateVariable_AVTransport_NextAVTransportURI UPnP_StateVariable_AVTransport_NextAVTransportURI_Impl = 
{
	255,
	89,
	344,
	16
};
struct UPnP_StateVariable_AVTransport_PlaybackStorageMedium UPnP_StateVariable_AVTransport_PlaybackStorageMedium_Impl = 
{
	360,
	92,
	452,
	18,
	646,
	19,
	{"UNKNOWN","CD-DA","DVD-VIDEO","HDD","NETWORK",NULL},
	665,
	16
};
struct UPnP_StateVariable_AVTransport_RecordStorageMedium UPnP_StateVariable_AVTransport_RecordStorageMedium_Impl = 
{
	681,
	90,
	771,
	18,
	833,
	19,
	{"NOT_IMPLEMENTED",NULL},
	852,
	16
};
struct UPnP_StateVariable_AVTransport_A_ARG_TYPE_InstanceID UPnP_StateVariable_AVTransport_A_ARG_TYPE_InstanceID_Impl = 
{
	868,
	89,
	957,
	16
};
struct UPnP_StateVariable_AVTransport_CurrentTrack UPnP_StateVariable_AVTransport_CurrentTrack_Impl = 
{
	973,
	80,
	1053,
	19,
	1130,
	20,
	{"0","65535","1"},
	1150,
	16
};
struct UPnP_StateVariable_AVTransport_CurrentTrackDuration UPnP_StateVariable_AVTransport_CurrentTrackDuration_Impl = 
{
	1166,
	91,
	1257,
	16
};
struct UPnP_StateVariable_AVTransport_PossiblePlaybackStorageMedia UPnP_StateVariable_AVTransport_PossiblePlaybackStorageMedia_Impl = 
{
	1273,
	99,
	1372,
	18,
	1566,
	19,
	{"UNKNOWN","CD-DA","DVD-VIDEO","HDD","NETWORK",NULL},
	1585,
	16
};
struct UPnP_StateVariable_AVTransport_CurrentPlayMode UPnP_StateVariable_AVTransport_CurrentPlayMode_Impl = 
{
	1601,
	86,
	1687,
	18,
	1815,
	19,
	{"NORMAL","SHUFFLE","REPEAT_ALL",NULL},
	1834,
	14,
	1854,
	15,
	"NORMAL",
	1869,
	16
};
struct UPnP_StateVariable_AVTransport_TransportStatus UPnP_StateVariable_AVTransport_TransportStatus_Impl = 
{
	1885,
	86,
	1971,
	18,
	2063,
	19,
	{"OK","ERROR_OCCURRED",NULL},
	2082,
	16
};
struct UPnP_StateVariable_AVTransport_CurrentRecordQualityMode UPnP_StateVariable_AVTransport_CurrentRecordQualityMode_Impl = 
{
	2098,
	95,
	2193,
	18,
	2255,
	19,
	{"NOT_IMPLEMENTED",NULL},
	2274,
	16
};
struct UPnP_StateVariable_AVTransport_TransportState UPnP_StateVariable_AVTransport_TransportState_Impl = 
{
	2290,
	85,
	2375,
	18,
	2596,
	19,
	{"STOPPED","PAUSED_PLAYBACK","PLAYING","TRANSITIONING","NO_MEDIA_PRESENT",NULL},
	2615,
	16
};
struct UPnP_StateVariable_AVTransport_CurrentTrackURI UPnP_StateVariable_AVTransport_CurrentTrackURI_Impl = 
{
	2631,
	86,
	2717,
	16
};
struct UPnP_StateVariable_AVTransport_AVTransportURI UPnP_StateVariable_AVTransport_AVTransportURI_Impl = 
{
	2733,
	85,
	2818,
	16
};
struct UPnP_StateVariable_AVTransport_AbsoluteCounterPosition UPnP_StateVariable_AVTransport_AbsoluteCounterPosition_Impl = 
{
	2834,
	90,
	2924,
	16
};
struct UPnP_StateVariable_AVTransport_PossibleRecordQualityModes UPnP_StateVariable_AVTransport_PossibleRecordQualityModes_Impl = 
{
	2940,
	97,
	3037,
	18,
	3099,
	19,
	{"NOT_IMPLEMENTED",NULL},
	3118,
	16
};
struct UPnP_StateVariable_AVTransport_LastChange UPnP_StateVariable_AVTransport_LastChange_Impl = 
{
	3134,
	82,
	3216,
	16
};
struct UPnP_StateVariable_AVTransport_NextAVTransportURIMetaData UPnP_StateVariable_AVTransport_NextAVTransportURIMetaData_Impl = 
{
	3232,
	97,
	3329,
	16
};
struct UPnP_StateVariable_AVTransport_AVTransportURIMetaData UPnP_StateVariable_AVTransport_AVTransportURIMetaData_Impl = 
{
	3345,
	93,
	3438,
	16
};
struct UPnP_StateVariable_AVTransport_A_ARG_TYPE_SeekTarget UPnP_StateVariable_AVTransport_A_ARG_TYPE_SeekTarget_Impl = 
{
	3454,
	92,
	3546,
	16
};
struct UPnP_StateVariable_AVTransport_AbsoluteTimePosition UPnP_StateVariable_AVTransport_AbsoluteTimePosition_Impl = 
{
	3562,
	91,
	3653,
	16
};
struct UPnP_StateVariable_AVTransport_RecordMediumWriteStatus UPnP_StateVariable_AVTransport_RecordMediumWriteStatus_Impl = 
{
	3669,
	94,
	3763,
	18,
	3825,
	19,
	{"NOT_IMPLEMENTED",NULL},
	3844,
	16
};
struct UPnP_StateVariable_AVTransport_CurrentTransportActions UPnP_StateVariable_AVTransport_CurrentTransportActions_Impl = 
{
	3860,
	94,
	3954,
	16
};
struct UPnP_StateVariable_AVTransport_RelativeTimePosition UPnP_StateVariable_AVTransport_RelativeTimePosition_Impl = 
{
	3970,
	91,
	4061,
	16
};
struct UPnP_StateVariable_AVTransport_CurrentTrackMetaData UPnP_StateVariable_AVTransport_CurrentTrackMetaData_Impl = 
{
	4077,
	91,
	4168,
	16
};
struct UPnP_StateVariable_AVTransport_CurrentMediaDuration UPnP_StateVariable_AVTransport_CurrentMediaDuration_Impl = 
{
	4184,
	91,
	4275,
	16
};
struct UPnP_StateVariable_AVTransport_TransportPlaySpeed UPnP_StateVariable_AVTransport_TransportPlaySpeed_Impl = 
{
	4291,
	89,
	4380,
	18,
	4428,
	19,
	{"1",NULL},
	4447,
	16
};
struct UPnP_StateVariable_AVTransport_RelativeCounterPosition UPnP_StateVariable_AVTransport_RelativeCounterPosition_Impl = 
{
	4463,
	90,
	4553,
	16
};
struct UPnP_StateVariable_AVTransport_NumberOfTracks UPnP_StateVariable_AVTransport_NumberOfTracks_Impl = 
{
	4569,
	82,
	4651,
	19,
	4714,
	20,
	{"0","65535",NULL},
	4734,
	16
};
struct UPnP_StateVariable_AVTransport_PossibleRecordStorageMedia UPnP_StateVariable_AVTransport_PossibleRecordStorageMedia_Impl = 
{
	4750,
	97,
	4847,
	18,
	4909,
	19,
	{"NOT_IMPLEMENTED",NULL},
	4928,
	16
};
struct UPnP_StateVariableTable_ConnectionManager UPnP_StateVariableTable_ConnectionManager_Impl = 
{
	{
		(char)0x42,(char)0x3C,(char)0x73,(char)0x74,(char)0x61,(char)0x74,(char)0x65,(char)0x56,(char)0x61,(char)0x72
		,(char)0x69,(char)0x61,(char)0x62,(char)0x6C,(char)0x65,(char)0x20,(char)0x73,(char)0x65,(char)0x6E,(char)0x64
		,(char)0x45,(char)0x76,(char)0x65,(char)0x6E,(char)0x74,(char)0x73,(char)0x3D,(char)0x22,(char)0x6E,(char)0x6F
		,(char)0x22,(char)0x3E,(char)0x3C,(char)0x6E,(char)0x61,(char)0x6D,(char)0x65,(char)0x3E,(char)0x41,(char)0x5F
		,(char)0x41,(char)0x52,(char)0x47,(char)0x5F,(char)0x54,(char)0x59,(char)0x50,(char)0x45,(char)0x5F,(char)0x43
		,(char)0x6F,(char)0x6E,(char)0x6E,(char)0x65,(char)0x63,(char)0x74,(char)0x69,(char)0x6F,(char)0x6E,(char)0x53
		,(char)0x74,(char)0x61,(char)0x74,(char)0x75,(char)0x73,(char)0x3C,(char)0x2F,(char)0x85,(char)0x08,(char)0x12
		,(char)0x3C,(char)0x64,(char)0x61,(char)0x74,(char)0x61,(char)0x54,(char)0x79,(char)0x70,(char)0x65,(char)0x3E
		,(char)0x73,(char)0x74,(char)0x72,(char)0x69,(char)0x6E,(char)0x67,(char)0x3C,(char)0x2F,(char)0x49,(char)0x04
		,(char)0x11,(char)0x3C,(char)0x61,(char)0x6C,(char)0x6C,(char)0x6F,(char)0x77,(char)0x65,(char)0x64,(char)0x56
		,(char)0x61,(char)0x6C,(char)0x75,(char)0x65,(char)0x4C,(char)0x69,(char)0x73,(char)0x74,(char)0x8E,(char)0x04
		,(char)0x05,(char)0x3E,(char)0x4F,(char)0x4B,(char)0x3C,(char)0x2F,(char)0x4D,(char)0x04,(char)0x00,(char)0xCE
		,(char)0x07,(char)0x15,(char)0x43,(char)0x6F,(char)0x6E,(char)0x74,(char)0x65,(char)0x6E,(char)0x74,(char)0x46
		,(char)0x6F,(char)0x72,(char)0x6D,(char)0x61,(char)0x74,(char)0x4D,(char)0x69,(char)0x73,(char)0x6D,(char)0x61
		,(char)0x74,(char)0x63,(char)0x68,(char)0x9D,(char)0x0C,(char)0x14,(char)0x49,(char)0x6E,(char)0x73,(char)0x75
		,(char)0x66,(char)0x66,(char)0x69,(char)0x63,(char)0x69,(char)0x65,(char)0x6E,(char)0x74,(char)0x42,(char)0x61
		,(char)0x6E,(char)0x64,(char)0x77,(char)0x69,(char)0x64,(char)0x74,(char)0x9E,(char)0x0C,(char)0x05,(char)0x55
		,(char)0x6E,(char)0x72,(char)0x65,(char)0x6C,(char)0x45,(char)0x40,(char)0x07,(char)0x43,(char)0x68,(char)0x61
		,(char)0x6E,(char)0x6E,(char)0x65,(char)0x6C,(char)0x9F,(char)0x0B,(char)0x05,(char)0x6B,(char)0x6E,(char)0x6F
		,(char)0x77,(char)0x6E,(char)0x90,(char)0x2D,(char)0x01,(char)0x2F,(char)0x12,(char)0x3A,(char)0x01,(char)0x2F
		,(char)0x4D,(char)0x57,(char)0x01,(char)0x3E,(char)0x3A,(char)0x5B,(char)0x02,(char)0x49,(char)0x44,(char)0x11
		,(char)0x5A,(char)0x02,(char)0x69,(char)0x34,(char)0x0C,(char)0x59,(char)0x00,(char)0xBF,(char)0x1A,(char)0x03
		,(char)0x44,(char)0x69,(char)0x72,(char)0x46,(char)0x75,(char)0x00,(char)0xFF,(char)0x73,(char)0x00,(char)0x85
		,(char)0x5F,(char)0x03,(char)0x70,(char)0x75,(char)0x74,(char)0x9D,(char)0x74,(char)0x06,(char)0x4F,(char)0x75
		,(char)0x74,(char)0x70,(char)0x75,(char)0x74,(char)0xFF,(char)0x4F,(char)0x00,(char)0xE3,(char)0xAA,(char)0x03
		,(char)0x52,(char)0x63,(char)0x73,(char)0x3F,(char)0x4E,(char)0x00,(char)0x8C,(char)0xC3,(char)0x03,(char)0x79
		,(char)0x65,(char)0x73,(char)0xC8,(char)0xC3,(char)0x12,(char)0x53,(char)0x6F,(char)0x75,(char)0x72,(char)0x63
		,(char)0x65,(char)0x50,(char)0x72,(char)0x6F,(char)0x74,(char)0x6F,(char)0x63,(char)0x6F,(char)0x6C,(char)0x49
		,(char)0x6E,(char)0x66,(char)0x6F,(char)0xA3,(char)0xC1,(char)0x00,(char)0xB6,(char)0x1A,(char)0x03,(char)0x69
		,(char)0x6E,(char)0x6B,(char)0x3F,(char)0x1A,(char)0x00,(char)0x2F,(char)0xF8,(char)0x00,(char)0xBF,(char)0x1B
		,(char)0x00,(char)0xB9,(char)0xB8,(char)0x07,(char)0x4D,(char)0x61,(char)0x6E,(char)0x61,(char)0x67,(char)0x65
		,(char)0x72,(char)0x7F,(char)0x38,(char)0x00,(char)0x63,(char)0xD5,(char)0x0B,(char)0x41,(char)0x56,(char)0x54
		,(char)0x72,(char)0x61,(char)0x6E,(char)0x73,(char)0x70,(char)0x6F,(char)0x72,(char)0x74,(char)0xBF,(char)0x87
		,(char)0x00,(char)0x97,(char)0x87,(char)0x07,(char)0x43,(char)0x75,(char)0x72,(char)0x72,(char)0x65,(char)0x6E
		,(char)0x74,(char)0x4C,(char)0xEF,(char)0x01,(char)0x73,(char)0x32,(char)0x88
	},
	347,
	1432
};
struct UPnP_StateVariable_ConnectionManager_A_ARG_TYPE_ConnectionStatus UPnP_StateVariable_ConnectionManager_A_ARG_TYPE_ConnectionStatus_Impl = 
{
	0,
	98,
	98,
	18,
	329,
	19,
	{"OK","ContentFormatMismatch","InsufficientBandwidth","UnreliableChannel","Unknown",NULL},
	348,
	16
};
struct UPnP_StateVariable_ConnectionManager_A_ARG_TYPE_ConnectionID UPnP_StateVariable_ConnectionManager_A_ARG_TYPE_ConnectionID_Impl = 
{
	364,
	90,
	454,
	16
};
struct UPnP_StateVariable_ConnectionManager_A_ARG_TYPE_Direction UPnP_StateVariable_ConnectionManager_A_ARG_TYPE_Direction_Impl = 
{
	470,
	91,
	561,
	18,
	648,
	19,
	{"Input","Output",NULL},
	667,
	16
};
struct UPnP_StateVariable_ConnectionManager_A_ARG_TYPE_RcsID UPnP_StateVariable_ConnectionManager_A_ARG_TYPE_RcsID_Impl = 
{
	683,
	83,
	766,
	16
};
struct UPnP_StateVariable_ConnectionManager_SourceProtocolInfo UPnP_StateVariable_ConnectionManager_SourceProtocolInfo_Impl = 
{
	782,
	90,
	872,
	16
};
struct UPnP_StateVariable_ConnectionManager_SinkProtocolInfo UPnP_StateVariable_ConnectionManager_SinkProtocolInfo_Impl = 
{
	888,
	88,
	976,
	16
};
struct UPnP_StateVariable_ConnectionManager_A_ARG_TYPE_ProtocolInfo UPnP_StateVariable_ConnectionManager_A_ARG_TYPE_ProtocolInfo_Impl = 
{
	992,
	94,
	1086,
	16
};
struct UPnP_StateVariable_ConnectionManager_A_ARG_TYPE_ConnectionManager UPnP_StateVariable_ConnectionManager_A_ARG_TYPE_ConnectionManager_Impl = 
{
	1102,
	99,
	1201,
	16
};
struct UPnP_StateVariable_ConnectionManager_A_ARG_TYPE_AVTransportID UPnP_StateVariable_ConnectionManager_A_ARG_TYPE_AVTransportID_Impl = 
{
	1217,
	91,
	1308,
	16
};
struct UPnP_StateVariable_ConnectionManager_CurrentConnectionIDs UPnP_StateVariable_ConnectionManager_CurrentConnectionIDs_Impl = 
{
	1324,
	92,
	1416,
	16
};
struct UPnP_StateVariableTable_RenderingControl UPnP_StateVariableTable_RenderingControl_Impl = 
{
	{
		(char)0x2B,(char)0x3C,(char)0x73,(char)0x74,(char)0x61,(char)0x74,(char)0x65,(char)0x56,(char)0x61,(char)0x72
		,(char)0x69,(char)0x61,(char)0x62,(char)0x6C,(char)0x65,(char)0x20,(char)0x73,(char)0x65,(char)0x6E,(char)0x64
		,(char)0x45,(char)0x76,(char)0x65,(char)0x6E,(char)0x74,(char)0x73,(char)0x3D,(char)0x22,(char)0x6E,(char)0x6F
		,(char)0x22,(char)0x3E,(char)0x3C,(char)0x6E,(char)0x61,(char)0x6D,(char)0x65,(char)0x3E,(char)0x4D,(char)0x75
		,(char)0x74,(char)0x65,(char)0x3C,(char)0x2F,(char)0xC5,(char)0x02,(char)0x13,(char)0x3C,(char)0x64,(char)0x61
		,(char)0x74,(char)0x61,(char)0x54,(char)0x79,(char)0x70,(char)0x65,(char)0x3E,(char)0x62,(char)0x6F,(char)0x6F
		,(char)0x6C,(char)0x65,(char)0x61,(char)0x6E,(char)0x3C,(char)0x2F,(char)0x89,(char)0x04,(char)0x02,(char)0x3C
		,(char)0x2F,(char)0x4D,(char)0x13,(char)0x01,(char)0x3E,(char)0x25,(char)0x17,(char)0x12,(char)0x41,(char)0x5F
		,(char)0x41,(char)0x52,(char)0x47,(char)0x5F,(char)0x54,(char)0x59,(char)0x50,(char)0x45,(char)0x5F,(char)0x43
		,(char)0x68,(char)0x61,(char)0x6E,(char)0x6E,(char)0x65,(char)0x6C,(char)0x91,(char)0x1A,(char)0x06,(char)0x73
		,(char)0x74,(char)0x72,(char)0x69,(char)0x6E,(char)0x67,(char)0x4C,(char)0x1A,(char)0x10,(char)0x61,(char)0x6C
		,(char)0x6C,(char)0x6F,(char)0x77,(char)0x65,(char)0x64,(char)0x56,(char)0x61,(char)0x6C,(char)0x75,(char)0x65
		,(char)0x4C,(char)0x69,(char)0x73,(char)0x74,(char)0x8E,(char)0x04,(char)0x09,(char)0x3E,(char)0x4D,(char)0x61
		,(char)0x73,(char)0x74,(char)0x65,(char)0x72,(char)0x3C,(char)0x2F,(char)0x4D,(char)0x05,(char)0x00,(char)0xCE
		,(char)0x08,(char)0x02,(char)0x4C,(char)0x46,(char)0xDD,(char)0x07,(char)0x01,(char)0x52,(char)0xD1,(char)0x07
		,(char)0x01,(char)0x2F,(char)0x12,(char)0x1D,(char)0x00,(char)0xFF,(char)0x3B,(char)0x0A,(char)0x49,(char)0x6E
		,(char)0x73,(char)0x74,(char)0x61,(char)0x6E,(char)0x63,(char)0x65,(char)0x49,(char)0x44,(char)0x11,(char)0x57
		,(char)0x03,(char)0x75,(char)0x69,(char)0x34,(char)0x3F,(char)0x56,(char)0x07,(char)0x3E,(char)0x56,(char)0x6F
		,(char)0x6C,(char)0x75,(char)0x6D,(char)0x65,(char)0x93,(char)0x16,(char)0x01,(char)0x32,(char)0x58,(char)0x52
		,(char)0x12,(char)0x52,(char)0x61,(char)0x6E,(char)0x67,(char)0x65,(char)0x3E,(char)0x3C,(char)0x6D,(char)0x69
		,(char)0x6E,(char)0x69,(char)0x6D,(char)0x75,(char)0x6D,(char)0x3E,(char)0x30,(char)0x3C,(char)0x2F,(char)0xC8
		,(char)0x02,(char)0x04,(char)0x3C,(char)0x6D,(char)0x61,(char)0x78,(char)0x05,(char)0x05,(char)0x05,(char)0x31
		,(char)0x30,(char)0x30,(char)0x3C,(char)0x2F,(char)0x48,(char)0x03,(char)0x09,(char)0x3C,(char)0x73,(char)0x74
		,(char)0x65,(char)0x70,(char)0x3E,(char)0x31,(char)0x3C,(char)0x2F,(char)0x05,(char)0x02,(char)0x02,(char)0x3C
		,(char)0x2F,(char)0x13,(char)0x13,(char)0x00,(char)0x7F,(char)0x84,(char)0x0F,(char)0x4D,(char)0x65,(char)0x64
		,(char)0x69,(char)0x61,(char)0x50,(char)0x6C,(char)0x61,(char)0x79,(char)0x65,(char)0x72,(char)0x4E,(char)0x61
		,(char)0x6D,(char)0x65,(char)0x63,(char)0x86,(char)0x00,(char)0xBF,(char)0xA0,(char)0x06,(char)0x50,(char)0x72
		,(char)0x65,(char)0x73,(char)0x65,(char)0x74,(char)0x27,(char)0x1B,(char)0x00,(char)0x5F,(char)0xA1,(char)0x0F
		,(char)0x46,(char)0x61,(char)0x63,(char)0x74,(char)0x6F,(char)0x72,(char)0x79,(char)0x44,(char)0x65,(char)0x66
		,(char)0x61,(char)0x75,(char)0x6C,(char)0x74,(char)0x73,(char)0x3F,(char)0x94,(char)0x00,(char)0xD8,(char)0xE6
		,(char)0x00,(char)0x8A,(char)0x2C,(char)0x00,(char)0x04,(char)0xC2,(char)0x00,(char)0xBF,(char)0x2D,(char)0x00
		,(char)0xBF,(char)0x2D,(char)0x00,(char)0x60,(char)0xFD,(char)0x03,(char)0x79,(char)0x65,(char)0x73,(char)0x88
		,(char)0xFD,(char)0x04,(char)0x4C,(char)0x61,(char)0x73,(char)0x74,(char)0xC4,(char)0xFB,(char)0x01,(char)0x67
		,(char)0x7F,(char)0x75,(char)0x00,(char)0x19,(char)0xDA,(char)0x00,(char)0xFF,(char)0x8E,(char)0x02,(char)0x65
		,(char)0x3E,(char)0x00,(char)0x00
	},
	343,
	1305
};
struct UPnP_StateVariable_RenderingControl_Mute UPnP_StateVariable_RenderingControl_Mute_Impl = 
{
	0,
	76,
	76,
	16
};
struct UPnP_StateVariable_RenderingControl_A_ARG_TYPE_Channel UPnP_StateVariable_RenderingControl_A_ARG_TYPE_Channel_Impl = 
{
	92,
	89,
	181,
	18,
	296,
	19,
	{"Master","LF","RF",NULL},
	315,
	16
};
struct UPnP_StateVariable_RenderingControl_A_ARG_TYPE_InstanceID UPnP_StateVariable_RenderingControl_A_ARG_TYPE_InstanceID_Impl = 
{
	331,
	89,
	420,
	16
};
struct UPnP_StateVariable_RenderingControl_Volume UPnP_StateVariable_RenderingControl_Volume_Impl = 
{
	436,
	74,
	510,
	19,
	585,
	20,
	{"0","100","1"},
	605,
	16
};
struct UPnP_StateVariable_RenderingControl_A_ARG_TYPE_MediaPlayerName UPnP_StateVariable_RenderingControl_A_ARG_TYPE_MediaPlayerName_Impl = 
{
	621,
	97,
	718,
	16
};
struct UPnP_StateVariable_RenderingControl_A_ARG_TYPE_PresetName UPnP_StateVariable_RenderingControl_A_ARG_TYPE_PresetName_Impl = 
{
	734,
	92,
	826,
	18,
	888,
	19,
	{"FactoryDefaults",NULL},
	907,
	16
};
struct UPnP_StateVariable_RenderingControl_PresetNameList UPnP_StateVariable_RenderingControl_PresetNameList_Impl = 
{
	923,
	85,
	1008,
	18,
	1070,
	19,
	{"FactoryDefaults",NULL},
	1089,
	16
};
struct UPnP_StateVariable_RenderingControl_LastChange UPnP_StateVariable_RenderingControl_LastChange_Impl = 
{
	1105,
	82,
	1187,
	16
};
struct UPnP_StateVariable_RenderingControl_MediaPlayerName UPnP_StateVariable_RenderingControl_MediaPlayerName_Impl = 
{
	1203,
	86,
	1289,
	16
};
struct UPnP_ActionTable_AVTransport UPnP_ActionTable_AVTransport_Impl = 
{
	{
		(char)0x22,(char)0x3C,(char)0x61,(char)0x63,(char)0x74,(char)0x69,(char)0x6F,(char)0x6E,(char)0x3E,(char)0x3C
		,(char)0x6E,(char)0x61,(char)0x6D,(char)0x65,(char)0x3E,(char)0x47,(char)0x65,(char)0x74,(char)0x43,(char)0x75
		,(char)0x72,(char)0x72,(char)0x65,(char)0x6E,(char)0x74,(char)0x54,(char)0x72,(char)0x61,(char)0x6E,(char)0x73
		,(char)0x70,(char)0x6F,(char)0x72,(char)0x74,(char)0x41,(char)0x05,(char)0x08,(char)0x03,(char)0x73,(char)0x3C
		,(char)0x2F,(char)0x45,(char)0x08,(char)0x0D,(char)0x3C,(char)0x61,(char)0x72,(char)0x67,(char)0x75,(char)0x6D
		,(char)0x65,(char)0x6E,(char)0x74,(char)0x4C,(char)0x69,(char)0x73,(char)0x74,(char)0x8A,(char)0x03,(char)0x00
		,(char)0xC7,(char)0x0F,(char)0x0A,(char)0x49,(char)0x6E,(char)0x73,(char)0x74,(char)0x61,(char)0x6E,(char)0x63
		,(char)0x65,(char)0x49,(char)0x44,(char)0xC8,(char)0x0B,(char)0x04,(char)0x64,(char)0x69,(char)0x72,(char)0x65
		,(char)0x46,(char)0x18,(char)0x04,(char)0x69,(char)0x6E,(char)0x3C,(char)0x2F,(char)0x8A,(char)0x03,(char)0x21
		,(char)0x3C,(char)0x72,(char)0x65,(char)0x6C,(char)0x61,(char)0x74,(char)0x65,(char)0x64,(char)0x53,(char)0x74
		,(char)0x61,(char)0x74,(char)0x65,(char)0x56,(char)0x61,(char)0x72,(char)0x69,(char)0x61,(char)0x62,(char)0x6C
		,(char)0x65,(char)0x3E,(char)0x41,(char)0x5F,(char)0x41,(char)0x52,(char)0x47,(char)0x5F,(char)0x54,(char)0x59
		,(char)0x50,(char)0x45,(char)0x5F,(char)0xCC,(char)0x12,(char)0x00,(char)0x15,(char)0x0B,(char)0x02,(char)0x3C
		,(char)0x2F,(char)0x4A,(char)0x1F,(char)0x00,(char)0xCF,(char)0x21,(char)0x00,(char)0xCF,(char)0x2C,(char)0x00
		,(char)0x0A,(char)0x21,(char)0x03,(char)0x6F,(char)0x75,(char)0x74,(char)0x62,(char)0x21,(char)0x00,(char)0x59
		,(char)0x40,(char)0x00,(char)0xE1,(char)0x21,(char)0x01,(char)0x2F,(char)0x4E,(char)0x47,(char)0x01,(char)0x2F
		,(char)0xC8,(char)0x56,(char)0x00,(char)0xD0,(char)0x58,(char)0x11,(char)0x44,(char)0x65,(char)0x76,(char)0x69
		,(char)0x63,(char)0x65,(char)0x43,(char)0x61,(char)0x70,(char)0x61,(char)0x62,(char)0x69,(char)0x6C,(char)0x69
		,(char)0x74,(char)0x69,(char)0x65,(char)0xBF,(char)0x57,(char)0x00,(char)0xBF,(char)0x57,(char)0x00,(char)0xAF
		,(char)0x57,(char)0x09,(char)0x50,(char)0x6C,(char)0x61,(char)0x79,(char)0x4D,(char)0x65,(char)0x64,(char)0x69
		,(char)0x61,(char)0x37,(char)0x58,(char)0x08,(char)0x50,(char)0x6F,(char)0x73,(char)0x73,(char)0x69,(char)0x62
		,(char)0x6C,(char)0x65,(char)0x04,(char)0x12,(char)0x0B,(char)0x62,(char)0x61,(char)0x63,(char)0x6B,(char)0x53
		,(char)0x74,(char)0x6F,(char)0x72,(char)0x61,(char)0x67,(char)0x65,(char)0xC7,(char)0x14,(char)0x00,(char)0x30
		,(char)0x7B,(char)0x03,(char)0x52,(char)0x65,(char)0x63,(char)0x7F,(char)0x23,(char)0x00,(char)0x45,(char)0x23
		,(char)0x06,(char)0x52,(char)0x65,(char)0x63,(char)0x6F,(char)0x72,(char)0x64,(char)0xFF,(char)0x22,(char)0x0E
		,(char)0x65,(char)0x63,(char)0x51,(char)0x75,(char)0x61,(char)0x6C,(char)0x69,(char)0x74,(char)0x79,(char)0x4D
		,(char)0x6F,(char)0x64,(char)0x65,(char)0x73,(char)0xBF,(char)0x24,(char)0x00,(char)0x86,(char)0x24,(char)0x00
		,(char)0x4E,(char)0x14,(char)0x00,(char)0xBF,(char)0xA0,(char)0x00,(char)0x4A,(char)0xF9,(char)0x00,(char)0x05
		,(char)0x70,(char)0x04,(char)0x49,(char)0x6E,(char)0x66,(char)0x6F,(char)0xFF,(char)0xF5,(char)0x00,(char)0xFF
		,(char)0xF5,(char)0x00,(char)0xEE,(char)0xF5,(char)0x07,(char)0x4E,(char)0x72,(char)0x54,(char)0x72,(char)0x61
		,(char)0x63,(char)0x6B,(char)0x38,(char)0xF6,(char)0x08,(char)0x4E,(char)0x75,(char)0x6D,(char)0x62,(char)0x65
		,(char)0x72,(char)0x4F,(char)0x66,(char)0x48,(char)0x11,(char)0x00,(char)0x30,(char)0xBE,(char)0x00,(char)0x05
		,(char)0xBD,(char)0x04,(char)0x44,(char)0x75,(char)0x72,(char)0x61,(char)0x44,(char)0xF5,(char)0x00,(char)0x37
		,(char)0xBF,(char)0x07,(char)0x43,(char)0x75,(char)0x72,(char)0x72,(char)0x65,(char)0x6E,(char)0x74,(char)0xCF
		,(char)0x12,(char)0x00,(char)0xB0,(char)0xE0,(char)0x00,(char)0x87,(char)0x11,(char)0x03,(char)0x55,(char)0x52
		,(char)0x49,(char)0xF7,(char)0xE0,(char)0x0A,(char)0x41,(char)0x56,(char)0x54,(char)0x72,(char)0x61,(char)0x6E
		,(char)0x73,(char)0x70,(char)0x6F,(char)0x72,(char)0x46,(char)0x11,(char)0x00,(char)0x7A,(char)0x20,(char)0x08
		,(char)0x4D,(char)0x65,(char)0x74,(char)0x61,(char)0x44,(char)0x61,(char)0x74,(char)0x61,(char)0x7F,(char)0x22
		,(char)0x02,(char)0x6F,(char)0x72,(char)0x4E,(char)0x13,(char)0x00,(char)0xF1,(char)0x86,(char)0x02,(char)0x65
		,(char)0x78,(char)0xFB,(char)0x43,(char)0x00,(char)0x84,(char)0x0F,(char)0x00,(char)0xFF,(char)0x44,(char)0x00
		,(char)0x88,(char)0x20,(char)0x00,(char)0x3F,(char)0x44,(char)0x00,(char)0x92,(char)0x22,(char)0x00,(char)0x3A
		,(char)0x45,(char)0x04,(char)0x50,(char)0x6C,(char)0x61,(char)0x79,(char)0x04,(char)0xFA,(char)0x02,(char)0x75
		,(char)0x6D,(char)0x77,(char)0xCC,(char)0x00,(char)0x44,(char)0x10,(char)0x0B,(char)0x62,(char)0x61,(char)0x63
		,(char)0x6B,(char)0x53,(char)0x74,(char)0x6F,(char)0x72,(char)0x61,(char)0x67,(char)0x65,(char)0x08,(char)0x13
		,(char)0x00,(char)0xF0,(char)0xED,(char)0x06,(char)0x52,(char)0x65,(char)0x63,(char)0x6F,(char)0x72,(char)0x64
		,(char)0xBD,(char)0x22,(char)0x00,(char)0xC6,(char)0x10,(char)0x00,(char)0x3F,(char)0x22,(char)0x05,(char)0x57
		,(char)0x72,(char)0x69,(char)0x74,(char)0x65,(char)0x84,(char)0xFB,(char)0x02,(char)0x75,(char)0x73,(char)0xFD
		,(char)0x21,(char)0x00,(char)0x06,(char)0x55,(char)0x00,(char)0x8D,(char)0x13,(char)0x00,(char)0x61,(char)0xF0
		,(char)0x00,(char)0x09,(char)0xF3,(char)0x03,(char)0x4C,(char)0x69,(char)0x73,(char)0xC5,(char)0x03,(char)0x00
		,(char)0x87,(char)0xE7,(char)0x00,(char)0x08,(char)0x02,(char)0x00,(char)0xC5,(char)0xF5,(char)0x07,(char)0x47
		,(char)0x65,(char)0x74,(char)0x50,(char)0x6F,(char)0x73,(char)0x69,(char)0xC4,(char)0xF1,(char)0x04,(char)0x49
		,(char)0x6E,(char)0x66,(char)0x6F,(char)0x08,(char)0xF7,(char)0x00,(char)0xCE,(char)0x0E,(char)0x00,(char)0x8F
		,(char)0xE2,(char)0x0A,(char)0x49,(char)0x6E,(char)0x73,(char)0x74,(char)0x61,(char)0x6E,(char)0x63,(char)0x65
		,(char)0x49,(char)0x44,(char)0x92,(char)0xE0,(char)0x02,(char)0x69,(char)0x6E,(char)0x63,(char)0xE0,(char)0x0A
		,(char)0x5F,(char)0x41,(char)0x52,(char)0x47,(char)0x5F,(char)0x54,(char)0x59,(char)0x50,(char)0x45,(char)0x5F
		,(char)0xCC,(char)0x12,(char)0x00,(char)0x30,(char)0xE0,(char)0x05,(char)0x54,(char)0x72,(char)0x61,(char)0x63
		,(char)0x6B,(char)0xB7,(char)0xDF,(char)0x07,(char)0x43,(char)0x75,(char)0x72,(char)0x72,(char)0x65,(char)0x6E
		,(char)0x74,(char)0xC7,(char)0x10,(char)0x00,(char)0xB5,(char)0x1E,(char)0x04,(char)0x44,(char)0x75,(char)0x72
		,(char)0x61,(char)0xC4,(char)0xFB,(char)0x00,(char)0xBF,(char)0x20,(char)0x00,(char)0xCE,(char)0x12,(char)0x00
		,(char)0x35,(char)0x41,(char)0x00,(char)0xCA,(char)0xEB,(char)0x00,(char)0x3F,(char)0x43,(char)0x02,(char)0x63
		,(char)0x6B,(char)0xBA,(char)0xFE,(char)0x00,(char)0x85,(char)0x63,(char)0x03,(char)0x55,(char)0x52,(char)0x49
		,(char)0x7F,(char)0x64,(char)0x00,(char)0x89,(char)0x11,(char)0x00,(char)0xB2,(char)0xFC,(char)0x05,(char)0x6C
		,(char)0x54,(char)0x69,(char)0x6D,(char)0x65,(char)0x79,(char)0xFB,(char)0x06,(char)0x6C,(char)0x61,(char)0x74
		,(char)0x69,(char)0x76,(char)0x65,(char)0xC4,(char)0x10,(char)0x00,(char)0x08,(char)0xC4,(char)0x00,(char)0xB2
		,(char)0xFB,(char)0x03,(char)0x41,(char)0x62,(char)0x73,(char)0x3B,(char)0x21,(char)0x07,(char)0x41,(char)0x62
		,(char)0x73,(char)0x6F,(char)0x6C,(char)0x75,(char)0x74,(char)0x3F,(char)0x21,(char)0x08,(char)0x52,(char)0x65
		,(char)0x6C,(char)0x43,(char)0x6F,(char)0x75,(char)0x6E,(char)0x74,(char)0x7F,(char)0x42,(char)0x00,(char)0x05
		,(char)0x11,(char)0x02,(char)0x65,(char)0x72,(char)0x3D,(char)0x43,(char)0x00,(char)0x3C,(char)0x22,(char)0x00
		,(char)0x48,(char)0x43,(char)0x00,(char)0x32,(char)0x22,(char)0x00,(char)0xC9,(char)0xED,(char)0x03,(char)0x4C
		,(char)0x69,(char)0x73,(char)0xC5,(char)0x03,(char)0x00,(char)0x87,(char)0xE1,(char)0x00,(char)0x08,(char)0x02
		,(char)0x00,(char)0x85,(char)0xF0,(char)0x02,(char)0x47,(char)0x65,(char)0x04,(char)0xFF,(char)0x0A,(char)0x6E
		,(char)0x73,(char)0x70,(char)0x6F,(char)0x72,(char)0x74,(char)0x49,(char)0x6E,(char)0x66,(char)0x6F,(char)0x48
		,(char)0xF1,(char)0x00,(char)0x0E,(char)0x0F,(char)0x00,(char)0xCF,(char)0xFD,(char)0x0A,(char)0x49,(char)0x6E
		,(char)0x73,(char)0x74,(char)0x61,(char)0x6E,(char)0x63,(char)0x65,(char)0x49,(char)0x44,(char)0x12,(char)0xFD
		,(char)0x02,(char)0x69,(char)0x6E,(char)0x63,(char)0x78,(char)0x0A,(char)0x5F,(char)0x41,(char)0x52,(char)0x47
		,(char)0x5F,(char)0x54,(char)0x59,(char)0x50,(char)0x45,(char)0x5F,(char)0xCC,(char)0x12,(char)0x00,(char)0x30
		,(char)0xFD,(char)0x00,(char)0x0A,(char)0xEC,(char)0x00,(char)0x06,(char)0x30,(char)0x00,(char)0x85,(char)0xF3
		,(char)0x00,(char)0x37,(char)0xFF,(char)0x00,(char)0x50,(char)0x11,(char)0x00,(char)0x3F,(char)0x23,(char)0x00
		,(char)0x05,(char)0x23,(char)0x02,(char)0x75,(char)0x73,(char)0x7F,(char)0x23,(char)0x00,(char)0x89,(char)0x11
		,(char)0x00,(char)0xB7,(char)0x46,(char)0x05,(char)0x53,(char)0x70,(char)0x65,(char)0x65,(char)0x64,(char)0x7F
		,(char)0x44,(char)0x05,(char)0x74,(char)0x50,(char)0x6C,(char)0x61,(char)0x79,(char)0x47,(char)0x12,(char)0x00
		,(char)0xFF,(char)0x9C,(char)0x00,(char)0xD3,(char)0x9C,(char)0x08,(char)0x53,(char)0x65,(char)0x74,(char)0x74
		,(char)0x69,(char)0x6E,(char)0x67,(char)0x73,(char)0xFF,(char)0x9D,(char)0x00,(char)0xFF,(char)0x9D,(char)0x00
		,(char)0x6E,(char)0xF4,(char)0x00,(char)0x44,(char)0x44,(char)0x03,(char)0x4D,(char)0x6F,(char)0x64,(char)0xB8
		,(char)0x9A,(char)0x00,(char)0x87,(char)0xAD,(char)0x00,(char)0x8A,(char)0x11,(char)0x00,(char)0xF0,(char)0xBD
		,(char)0x09,(char)0x52,(char)0x65,(char)0x63,(char)0x51,(char)0x75,(char)0x61,(char)0x6C,(char)0x69,(char)0x74
		,(char)0xBF,(char)0x21,(char)0x00,(char)0x04,(char)0xCF,(char)0x06,(char)0x52,(char)0x65,(char)0x63,(char)0x6F
		,(char)0x72,(char)0x64,(char)0xCD,(char)0x13,(char)0x00,(char)0x7F,(char)0x79,(char)0x00,(char)0x07,(char)0xE7
		,(char)0x04,(char)0x4E,(char)0x65,(char)0x78,(char)0x74,(char)0x7F,(char)0x75,(char)0x00,(char)0x7F,(char)0x75
		,(char)0x00,(char)0xFF,(char)0xAA,(char)0x00,(char)0xC6,(char)0x7A,(char)0x04,(char)0x61,(char)0x75,(char)0x73
		,(char)0x65,(char)0xFF,(char)0x31,(char)0x00,(char)0xFF,(char)0x31,(char)0x00,(char)0xFF,(char)0x31,(char)0x00
		,(char)0x89,(char)0xAC,(char)0x00,(char)0xBF,(char)0xD8,(char)0x00,(char)0xBF,(char)0xD8,(char)0x00,(char)0xAE
		,(char)0xD8,(char)0x05,(char)0x53,(char)0x70,(char)0x65,(char)0x65,(char)0x64,(char)0x36,(char)0xF9,(char)0x08
		,(char)0x54,(char)0x72,(char)0x61,(char)0x6E,(char)0x73,(char)0x70,(char)0x6F,(char)0x72,(char)0x05,(char)0xD8
		,(char)0x00,(char)0x07,(char)0x12,(char)0x00,(char)0x3F,(char)0x83,(char)0x00,(char)0xC8,(char)0xFD,(char)0x07
		,(char)0x72,(char)0x65,(char)0x76,(char)0x69,(char)0x6F,(char)0x75,(char)0x73,(char)0xBF,(char)0xB5,(char)0x00
		,(char)0xBF,(char)0xB5,(char)0x00,(char)0x3F,(char)0xE7,(char)0x00,(char)0xC6,(char)0x57,(char)0x03,(char)0x65
		,(char)0x65,(char)0x6B,(char)0xFF,(char)0x83,(char)0x00,(char)0xFF,(char)0x83,(char)0x00,(char)0xEE,(char)0x83
		,(char)0x04,(char)0x55,(char)0x6E,(char)0x69,(char)0x74,(char)0xBF,(char)0xD5,(char)0x02,(char)0x45,(char)0x5F
		,(char)0x44,(char)0x3D,(char)0x04,(char)0x4D,(char)0x6F,(char)0x64,(char)0x65,(char)0xB2,(char)0xA3,(char)0x05
		,(char)0x54,(char)0x61,(char)0x72,(char)0x67,(char)0x65,(char)0x7F,(char)0x20,(char)0x00,(char)0x47,(char)0x20
		,(char)0x00,(char)0xC8,(char)0x12,(char)0x00,(char)0x3F,(char)0x72,(char)0x00,(char)0x09,(char)0x72,(char)0x03
		,(char)0x74,(char)0x41,(char)0x56,(char)0x49,(char)0xBC,(char)0x03,(char)0x55,(char)0x52,(char)0x49,(char)0x3F
		,(char)0xF9,(char)0x00,(char)0x3F,(char)0xF9,(char)0x00,(char)0x2E,(char)0xF9,(char)0x06,(char)0x43,(char)0x75
		,(char)0x72,(char)0x72,(char)0x65,(char)0x6E,(char)0x8C,(char)0x2D,(char)0x00,(char)0xAF,(char)0xC8,(char)0x00
		,(char)0x8F,(char)0x3E,(char)0x00,(char)0x3A,(char)0x20,(char)0x08,(char)0x4D,(char)0x65,(char)0x74,(char)0x61
		,(char)0x44,(char)0x61,(char)0x74,(char)0x61,(char)0x3F,(char)0x22,(char)0x01,(char)0x72,(char)0x0E,(char)0x13
		,(char)0x00,(char)0xFF,(char)0x78,(char)0x00,(char)0xCA,(char)0x78,(char)0x04,(char)0x50,(char)0x6C,(char)0x61
		,(char)0x79,(char)0x46,(char)0xAE,(char)0x00,(char)0xBF,(char)0xEC,(char)0x00,(char)0xBF,(char)0xEC,(char)0x00
		,(char)0xAC,(char)0xEC,(char)0x03,(char)0x4E,(char)0x65,(char)0x77,(char)0xD0,(char)0x2D,(char)0x00,(char)0x6E
		,(char)0xEE,(char)0x00,(char)0x87,(char)0x87,(char)0x00,(char)0x0A,(char)0x3F,(char)0x00,(char)0xBF,(char)0xCC
		,(char)0x00,(char)0x88,(char)0xCC,(char)0x03,(char)0x74,(char)0x6F,(char)0x70,(char)0x7F,(char)0xC9,(char)0x00
		,(char)0x7F,(char)0xC9,(char)0x00,(char)0x36,(char)0xFE
	},
	1125,
	7208
};
struct UPnP_Action_AVTransport_GetCurrentTransportActions UPnP_Action_AVTransport_GetCurrentTransportActions_Impl = 
{
	0,
	355
};
struct UPnP_Action_AVTransport_GetDeviceCapabilities UPnP_Action_AVTransport_GetDeviceCapabilities_Impl = 
{
	355,
	642
};
struct UPnP_Action_AVTransport_GetMediaInfo UPnP_Action_AVTransport_GetMediaInfo_Impl = 
{
	997,
	1432
};
struct UPnP_Action_AVTransport_GetPositionInfo UPnP_Action_AVTransport_GetPositionInfo_Impl = 
{
	2429,
	1271
};
struct UPnP_Action_AVTransport_GetTransportInfo UPnP_Action_AVTransport_GetTransportInfo_Impl = 
{
	3700,
	627
};
struct UPnP_Action_AVTransport_GetTransportSettings UPnP_Action_AVTransport_GetTransportSettings_Impl = 
{
	4327,
	485
};
struct UPnP_Action_AVTransport_Next UPnP_Action_AVTransport_Next_Impl = 
{
	4812,
	198
};
struct UPnP_Action_AVTransport_Pause UPnP_Action_AVTransport_Pause_Impl = 
{
	5010,
	199
};
struct UPnP_Action_AVTransport_Play UPnP_Action_AVTransport_Play_Impl = 
{
	5209,
	325
};
struct UPnP_Action_AVTransport_Previous UPnP_Action_AVTransport_Previous_Impl = 
{
	5534,
	202
};
struct UPnP_Action_AVTransport_Seek UPnP_Action_AVTransport_Seek_Impl = 
{
	5736,
	456
};
struct UPnP_Action_AVTransport_SetAVTransportURI UPnP_Action_AVTransport_SetAVTransportURI_Impl = 
{
	6192,
	483
};
struct UPnP_Action_AVTransport_SetPlayMode UPnP_Action_AVTransport_SetPlayMode_Impl = 
{
	6675,
	335
};
struct UPnP_Action_AVTransport_Stop UPnP_Action_AVTransport_Stop_Impl = 
{
	7010,
	198
};
struct UPnP_ActionTable_ConnectionManager UPnP_ActionTable_ConnectionManager_Impl = 
{
	{
		(char)0x1D,(char)0x3C,(char)0x61,(char)0x63,(char)0x74,(char)0x69,(char)0x6F,(char)0x6E,(char)0x3E,(char)0x3C
		,(char)0x6E,(char)0x61,(char)0x6D,(char)0x65,(char)0x3E,(char)0x47,(char)0x65,(char)0x74,(char)0x43,(char)0x75
		,(char)0x72,(char)0x72,(char)0x65,(char)0x6E,(char)0x74,(char)0x43,(char)0x6F,(char)0x6E,(char)0x6E,(char)0x65
		,(char)0xC5,(char)0x06,(char)0x05,(char)0x49,(char)0x44,(char)0x73,(char)0x3C,(char)0x2F,(char)0x85,(char)0x07
		,(char)0x0D,(char)0x3C,(char)0x61,(char)0x72,(char)0x67,(char)0x75,(char)0x6D,(char)0x65,(char)0x6E,(char)0x74
		,(char)0x4C,(char)0x69,(char)0x73,(char)0x74,(char)0x8A,(char)0x03,(char)0x00,(char)0x07,(char)0x0F,(char)0x00
		,(char)0x95,(char)0x0C,(char)0x03,(char)0x64,(char)0x69,(char)0x72,(char)0x86,(char)0x11,(char)0x06,(char)0x3E
		,(char)0x6F,(char)0x75,(char)0x74,(char)0x3C,(char)0x2F,(char)0xCA,(char)0x03,(char)0x16,(char)0x3C,(char)0x72
		,(char)0x65,(char)0x6C,(char)0x61,(char)0x74,(char)0x65,(char)0x64,(char)0x53,(char)0x74,(char)0x61,(char)0x74
		,(char)0x65,(char)0x56,(char)0x61,(char)0x72,(char)0x69,(char)0x61,(char)0x62,(char)0x6C,(char)0x65,(char)0x3E
		,(char)0x56,(char)0x1F,(char)0x00,(char)0xD5,(char)0x0A,(char)0x02,(char)0x3C,(char)0x2F,(char)0x0A,(char)0x20
		,(char)0x01,(char)0x2F,(char)0x4E,(char)0x26,(char)0x01,(char)0x2F,(char)0x08,(char)0x35,(char)0x00,(char)0x22
		,(char)0x37,(char)0x03,(char)0x6E,(char)0x66,(char)0x6F,(char)0x71,(char)0x37,(char)0x00,(char)0x12,(char)0x37
		,(char)0x02,(char)0x69,(char)0x6E,(char)0xE2,(char)0x36,(char)0x0B,(char)0x41,(char)0x5F,(char)0x41,(char)0x52
		,(char)0x47,(char)0x5F,(char)0x54,(char)0x59,(char)0x50,(char)0x45,(char)0x5F,(char)0x4E,(char)0x13,(char)0x00
		,(char)0xA1,(char)0x37,(char)0x00,(char)0x0F,(char)0x5A,(char)0x05,(char)0x52,(char)0x63,(char)0x73,(char)0x49
		,(char)0x44,(char)0x37,(char)0x58,(char)0x00,(char)0x4B,(char)0x21,(char)0x03,(char)0x52,(char)0x63,(char)0x73
		,(char)0xB4,(char)0x1F,(char)0x0B,(char)0x41,(char)0x56,(char)0x54,(char)0x72,(char)0x61,(char)0x6E,(char)0x73
		,(char)0x70,(char)0x6F,(char)0x72,(char)0x74,(char)0xBF,(char)0x21,(char)0x00,(char)0xC5,(char)0x42,(char)0x00
		,(char)0xCF,(char)0x13,(char)0x00,(char)0x30,(char)0x43,(char)0x08,(char)0x50,(char)0x72,(char)0x6F,(char)0x74
		,(char)0x6F,(char)0x63,(char)0x6F,(char)0x6C,(char)0x0C,(char)0x72,(char)0x00,(char)0xFA,(char)0x44,(char)0x00
		,(char)0x8E,(char)0x13,(char)0x00,(char)0x31,(char)0x23,(char)0x03,(char)0x65,(char)0x65,(char)0x72,(char)0x8A
		,(char)0xCD,(char)0x07,(char)0x4D,(char)0x61,(char)0x6E,(char)0x61,(char)0x67,(char)0x65,(char)0x72,(char)0x3F
		,(char)0x6A,(char)0x03,(char)0x50,(char)0x45,(char)0x5F,(char)0xD3,(char)0x14,(char)0x00,(char)0xBE,(char)0x26
		,(char)0x00,(char)0x7F,(char)0x8F,(char)0x00,(char)0xBF,(char)0xB0,(char)0x00,(char)0x84,(char)0xE2,(char)0x01
		,(char)0x44,(char)0x48,(char)0xCE,(char)0x00,(char)0xBF,(char)0xB1,(char)0x03,(char)0x50,(char)0x45,(char)0x5F
		,(char)0xCB,(char)0x12,(char)0x00,(char)0x30,(char)0xD2,(char)0x00,(char)0xC4,(char)0xE7,(char)0x02,(char)0x75
		,(char)0x73,(char)0x7F,(char)0x68,(char)0x00,(char)0x8D,(char)0xF3,(char)0x00,(char)0x88,(char)0x14,(char)0x00
		,(char)0xA1,(char)0xF4,(char)0x00,(char)0x49,(char)0xF7,(char)0x03,(char)0x4C,(char)0x69,(char)0x73,(char)0xC5
		,(char)0x03,(char)0x00,(char)0x07,(char)0xED,(char)0x00,(char)0x08,(char)0x02,(char)0x00,(char)0x05,(char)0xFA
		,(char)0x03,(char)0x47,(char)0x65,(char)0x74,(char)0xD4,(char)0xB7,(char)0x00,(char)0xCE,(char)0x0E,(char)0x00
		,(char)0x10,(char)0x35,(char)0x05,(char)0x6F,(char)0x75,(char)0x72,(char)0x63,(char)0x65,(char)0xF7,(char)0xE5
		,(char)0x00,(char)0x46,(char)0x0F,(char)0x00,(char)0x7E,(char)0xC1,(char)0x04,(char)0x53,(char)0x69,(char)0x6E
		,(char)0x6B,(char)0xF8,(char)0x1F,(char)0x03,(char)0x69,(char)0x6E,(char)0x6B,(char)0xAF,(char)0xE0,(char)0x00
		,(char)0x17,(char)0x52
	},
	342,
	1748
};
struct UPnP_Action_ConnectionManager_GetCurrentConnectionIDs UPnP_Action_ConnectionManager_GetCurrentConnectionIDs_Impl = 
{
	0,
	220
};
struct UPnP_Action_ConnectionManager_GetCurrentConnectionInfo UPnP_Action_ConnectionManager_GetCurrentConnectionInfo_Impl = 
{
	220,
	1200
};
struct UPnP_Action_ConnectionManager_GetProtocolInfo UPnP_Action_ConnectionManager_GetProtocolInfo_Impl = 
{
	1420,
	328
};
struct UPnP_ActionTable_RenderingControl UPnP_ActionTable_RenderingControl_Impl = 
{
	{
		(char)0x17,(char)0x3C,(char)0x61,(char)0x63,(char)0x74,(char)0x69,(char)0x6F,(char)0x6E,(char)0x3E,(char)0x3C
		,(char)0x6E,(char)0x61,(char)0x6D,(char)0x65,(char)0x3E,(char)0x47,(char)0x65,(char)0x74,(char)0x4D,(char)0x75
		,(char)0x74,(char)0x65,(char)0x3C,(char)0x2F,(char)0x85,(char)0x03,(char)0x0D,(char)0x3C,(char)0x61,(char)0x72
		,(char)0x67,(char)0x75,(char)0x6D,(char)0x65,(char)0x6E,(char)0x74,(char)0x4C,(char)0x69,(char)0x73,(char)0x74
		,(char)0x8A,(char)0x03,(char)0x00,(char)0x07,(char)0x0B,(char)0x0A,(char)0x49,(char)0x6E,(char)0x73,(char)0x74
		,(char)0x61,(char)0x6E,(char)0x63,(char)0x65,(char)0x49,(char)0x44,(char)0xC8,(char)0x0B,(char)0x04,(char)0x64
		,(char)0x69,(char)0x72,(char)0x65,(char)0x86,(char)0x13,(char)0x04,(char)0x69,(char)0x6E,(char)0x3C,(char)0x2F
		,(char)0x8A,(char)0x03,(char)0x21,(char)0x3C,(char)0x72,(char)0x65,(char)0x6C,(char)0x61,(char)0x74,(char)0x65
		,(char)0x64,(char)0x53,(char)0x74,(char)0x61,(char)0x74,(char)0x65,(char)0x56,(char)0x61,(char)0x72,(char)0x69
		,(char)0x61,(char)0x62,(char)0x6C,(char)0x65,(char)0x3E,(char)0x41,(char)0x5F,(char)0x41,(char)0x52,(char)0x47
		,(char)0x5F,(char)0x54,(char)0x59,(char)0x50,(char)0x45,(char)0x5F,(char)0xCC,(char)0x12,(char)0x00,(char)0x15
		,(char)0x0B,(char)0x02,(char)0x3C,(char)0x2F,(char)0x4A,(char)0x1F,(char)0x00,(char)0xCF,(char)0x21,(char)0x07
		,(char)0x43,(char)0x68,(char)0x61,(char)0x6E,(char)0x6E,(char)0x65,(char)0x6C,(char)0x3F,(char)0x21,(char)0x02
		,(char)0x45,(char)0x5F,(char)0x09,(char)0x12,(char)0x00,(char)0x71,(char)0x20,(char)0x05,(char)0x75,(char)0x72
		,(char)0x72,(char)0x65,(char)0x6E,(char)0x0D,(char)0x4E,(char)0x00,(char)0x4A,(char)0x42,(char)0x03,(char)0x6F
		,(char)0x75,(char)0x74,(char)0xA2,(char)0x42,(char)0x00,(char)0xC6,(char)0x5C,(char)0x00,(char)0x61,(char)0x3E
		,(char)0x01,(char)0x2F,(char)0xCE,(char)0x63,(char)0x01,(char)0x2F,(char)0x88,(char)0x6E,(char)0x00,(char)0x90
		,(char)0x70,(char)0x05,(char)0x56,(char)0x6F,(char)0x6C,(char)0x75,(char)0x6D,(char)0x3F,(char)0x71,(char)0x00
		,(char)0x3F,(char)0x71,(char)0x00,(char)0x3F,(char)0x71,(char)0x00,(char)0x3F,(char)0x71,(char)0x00,(char)0x39
		,(char)0x71,(char)0x00,(char)0x8E,(char)0x4E,(char)0x00,(char)0xAF,(char)0x71,(char)0x00,(char)0xC8,(char)0x5D
		,(char)0x00,(char)0x3F,(char)0x72,(char)0x00,(char)0x87,(char)0xE2,(char)0x00,(char)0xC4,(char)0xDC,(char)0x07
		,(char)0x50,(char)0x72,(char)0x65,(char)0x73,(char)0x65,(char)0x74,(char)0x73,(char)0xBF,(char)0xE3,(char)0x00
		,(char)0xBF,(char)0xE3,(char)0x00,(char)0x75,(char)0xC3,(char)0x00,(char)0x86,(char)0x2E,(char)0x04,(char)0x4E
		,(char)0x61,(char)0x6D,(char)0x65,(char)0x04,(char)0xAB,(char)0x00,(char)0xF7,(char)0xC5,(char)0x00,(char)0x50
		,(char)0x11,(char)0x00,(char)0x7F,(char)0xC8,(char)0x00,(char)0xC7,(char)0xEB,(char)0x05,(char)0x53,(char)0x65
		,(char)0x6C,(char)0x65,(char)0x63,(char)0xC7,(char)0x56,(char)0x00,(char)0x3F,(char)0xC9,(char)0x00,(char)0x3F
		,(char)0xC9,(char)0x00,(char)0x2E,(char)0xC9,(char)0x00,(char)0xCA,(char)0x54,(char)0x00,(char)0xFF,(char)0xEA
		,(char)0x02,(char)0x45,(char)0x5F,(char)0xCC,(char)0x12,(char)0x00,(char)0x7F,(char)0x55,(char)0x00,(char)0x49
		,(char)0x55,(char)0x05,(char)0x74,(char)0x4D,(char)0x75,(char)0x74,(char)0x65,(char)0xBF,(char)0xAA,(char)0x00
		,(char)0xBF,(char)0xAA,(char)0x00,(char)0xEF,(char)0xFC,(char)0x06,(char)0x68,(char)0x61,(char)0x6E,(char)0x6E
		,(char)0x65,(char)0x6C,(char)0xBF,(char)0xCB,(char)0x02,(char)0x45,(char)0x5F,(char)0x09,(char)0x12,(char)0x00
		,(char)0xF0,(char)0xCA,(char)0x0A,(char)0x44,(char)0x65,(char)0x73,(char)0x69,(char)0x72,(char)0x65,(char)0x64
		,(char)0x4D,(char)0x75,(char)0x74,(char)0xB7,(char)0x74,(char)0x03,(char)0x4D,(char)0x75,(char)0x74,(char)0x7F
		,(char)0x70,(char)0x00,(char)0x4D,(char)0x70,(char)0x05,(char)0x56,(char)0x6F,(char)0x6C,(char)0x75,(char)0x6D
		,(char)0xFF,(char)0x70,(char)0x00,(char)0xFF,(char)0x70,(char)0x00,(char)0xFF,(char)0x70,(char)0x00,(char)0xFF
		,(char)0x70,(char)0x00,(char)0xF9,(char)0x70,(char)0x00,(char)0x8E,(char)0x4E,(char)0x00,(char)0xEE,(char)0xE5
		,(char)0x00,(char)0x88,(char)0x5D,(char)0x00,(char)0x3F,(char)0xE2,(char)0x00,(char)0x07,(char)0xE2,(char)0x1C
		,(char)0x75,(char)0x70,(char)0x6E,(char)0x70,(char)0x52,(char)0x65,(char)0x6E,(char)0x64,(char)0x65,(char)0x72
		,(char)0x65,(char)0x72,(char)0x5F,(char)0x47,(char)0x65,(char)0x74,(char)0x4D,(char)0x65,(char)0x64,(char)0x69
		,(char)0x61,(char)0x50,(char)0x6C,(char)0x61,(char)0x79,(char)0x65,(char)0x72,(char)0x73,(char)0x04,(char)0xF0
		,(char)0x00,(char)0x7F,(char)0xE8,(char)0x00,(char)0x7F,(char)0xE8,(char)0x00,(char)0x6F,(char)0xE8,(char)0x05
		,(char)0x75,(char)0x72,(char)0x72,(char)0x65,(char)0x6E,(char)0xCC,(char)0x30,(char)0x00,(char)0x12,(char)0xEB
		,(char)0x03,(char)0x6F,(char)0x75,(char)0x74,(char)0x23,(char)0xCA,(char)0x00,(char)0x4A,(char)0x41,(char)0x04
		,(char)0x4E,(char)0x61,(char)0x6D,(char)0x65,(char)0xB2,(char)0xEA,(char)0x00,(char)0x98,(char)0x51,(char)0x00
		,(char)0x3F,(char)0x22,(char)0x00,(char)0x3F,(char)0x7D,(char)0x00,(char)0x15,(char)0x7D,(char)0x02,(char)0x53
		,(char)0x65,(char)0x54,(char)0x4C,(char)0x00,(char)0x7F,(char)0xF3,(char)0x00,(char)0x7F,(char)0xF3,(char)0x00
		,(char)0x71,(char)0x59,(char)0x02,(char)0x4E,(char)0x61,(char)0xB8,(char)0xD3,(char)0x00,(char)0x4B,(char)0xF5
		,(char)0x00,(char)0xBF,(char)0x5B,(char)0x00,(char)0x8A,(char)0xD8
	},
	476,
	3362
};
struct UPnP_Action_RenderingControl_GetMute UPnP_Action_RenderingControl_GetMute_Impl = 
{
	0,
	450
};
struct UPnP_Action_RenderingControl_GetVolume UPnP_Action_RenderingControl_GetVolume_Impl = 
{
	450,
	456
};
struct UPnP_Action_RenderingControl_ListPresets UPnP_Action_RenderingControl_ListPresets_Impl = 
{
	906,
	345
};
struct UPnP_Action_RenderingControl_SelectPreset UPnP_Action_RenderingControl_SelectPreset_Impl = 
{
	1251,
	341
};
struct UPnP_Action_RenderingControl_SetMute UPnP_Action_RenderingControl_SetMute_Impl = 
{
	1592,
	449
};
struct UPnP_Action_RenderingControl_SetVolume UPnP_Action_RenderingControl_SetVolume_Impl = 
{
	2041,
	455
};
struct UPnP_Action_RenderingControl_upnpRenderer_GetMediaPlayersList UPnP_Action_RenderingControl_upnpRenderer_GetMediaPlayersList_Impl = 
{
	2496,
	500
};
struct UPnP_Action_RenderingControl_upnpRenderer_SetMediaPlayer UPnP_Action_RenderingControl_upnpRenderer_SetMediaPlayer_Impl = 
{
	2996,
	366
};
struct UPnP_Service_AVTransport UPnP_Service_AVTransport_Impl =
{
	&UPnP_Action_AVTransport_GetCurrentTransportActions_Impl,
	&UPnP_Action_AVTransport_GetDeviceCapabilities_Impl,
	&UPnP_Action_AVTransport_GetMediaInfo_Impl,
	&UPnP_Action_AVTransport_GetPositionInfo_Impl,
	&UPnP_Action_AVTransport_GetTransportInfo_Impl,
	&UPnP_Action_AVTransport_GetTransportSettings_Impl,
	&UPnP_Action_AVTransport_Next_Impl,
	&UPnP_Action_AVTransport_Pause_Impl,
	&UPnP_Action_AVTransport_Play_Impl,
	&UPnP_Action_AVTransport_Previous_Impl,
	&UPnP_Action_AVTransport_Seek_Impl,
	&UPnP_Action_AVTransport_SetAVTransportURI_Impl,
	&UPnP_Action_AVTransport_SetPlayMode_Impl,
	&UPnP_Action_AVTransport_Stop_Impl,
	&UPnP_StateVariable_AVTransport_A_ARG_TYPE_SeekMode_Impl,
	&UPnP_StateVariable_AVTransport_NextAVTransportURI_Impl,
	&UPnP_StateVariable_AVTransport_PlaybackStorageMedium_Impl,
	&UPnP_StateVariable_AVTransport_RecordStorageMedium_Impl,
	&UPnP_StateVariable_AVTransport_A_ARG_TYPE_InstanceID_Impl,
	&UPnP_StateVariable_AVTransport_CurrentTrack_Impl,
	&UPnP_StateVariable_AVTransport_CurrentTrackDuration_Impl,
	&UPnP_StateVariable_AVTransport_PossiblePlaybackStorageMedia_Impl,
	&UPnP_StateVariable_AVTransport_CurrentPlayMode_Impl,
	&UPnP_StateVariable_AVTransport_TransportStatus_Impl,
	&UPnP_StateVariable_AVTransport_CurrentRecordQualityMode_Impl,
	&UPnP_StateVariable_AVTransport_TransportState_Impl,
	&UPnP_StateVariable_AVTransport_CurrentTrackURI_Impl,
	&UPnP_StateVariable_AVTransport_AVTransportURI_Impl,
	&UPnP_StateVariable_AVTransport_AbsoluteCounterPosition_Impl,
	&UPnP_StateVariable_AVTransport_PossibleRecordQualityModes_Impl,
	&UPnP_StateVariable_AVTransport_LastChange_Impl,
	&UPnP_StateVariable_AVTransport_NextAVTransportURIMetaData_Impl,
	&UPnP_StateVariable_AVTransport_AVTransportURIMetaData_Impl,
	&UPnP_StateVariable_AVTransport_A_ARG_TYPE_SeekTarget_Impl,
	&UPnP_StateVariable_AVTransport_AbsoluteTimePosition_Impl,
	&UPnP_StateVariable_AVTransport_RecordMediumWriteStatus_Impl,
	&UPnP_StateVariable_AVTransport_CurrentTransportActions_Impl,
	&UPnP_StateVariable_AVTransport_RelativeTimePosition_Impl,
	&UPnP_StateVariable_AVTransport_CurrentTrackMetaData_Impl,
	&UPnP_StateVariable_AVTransport_CurrentMediaDuration_Impl,
	&UPnP_StateVariable_AVTransport_TransportPlaySpeed_Impl,
	&UPnP_StateVariable_AVTransport_RelativeCounterPosition_Impl,
	&UPnP_StateVariable_AVTransport_NumberOfTracks_Impl,
	&UPnP_StateVariable_AVTransport_PossibleRecordStorageMedia_Impl,
	{
		(char)0x09,(char)0x3C,(char)0x73,(char)0x65,(char)0x72,(char)0x76,(char)0x69,(char)0x63,(char)0x65,(char)0x3E
		,(char)0x48,(char)0x02,(char)0x1A,(char)0x54,(char)0x79,(char)0x70,(char)0x65,(char)0x3E,(char)0x75,(char)0x72
		,(char)0x6E,(char)0x3A,(char)0x73,(char)0x63,(char)0x68,(char)0x65,(char)0x6D,(char)0x61,(char)0x73,(char)0x2D
		,(char)0x75,(char)0x70,(char)0x6E,(char)0x70,(char)0x2D,(char)0x6F,(char)0x72,(char)0x67,(char)0x3A,(char)0x87
		,(char)0x0A,(char)0x10,(char)0x3A,(char)0x41,(char)0x56,(char)0x54,(char)0x72,(char)0x61,(char)0x6E,(char)0x73
		,(char)0x70,(char)0x6F,(char)0x72,(char)0x74,(char)0x3A,(char)0x31,(char)0x3C,(char)0x2F,(char)0x0C,(char)0x0E
		,(char)0x00,(char)0x88,(char)0x13,(char)0x02,(char)0x49,(char)0x64,(char)0xC5,(char)0x10,(char)0x00,(char)0xD0
		,(char)0x0E,(char)0x02,(char)0x49,(char)0x64,(char)0x4C,(char)0x0F,(char)0x02,(char)0x3C,(char)0x2F,(char)0x8A
		,(char)0x0B,(char)0x09,(char)0x3C,(char)0x53,(char)0x43,(char)0x50,(char)0x44,(char)0x55,(char)0x52,(char)0x4C
		,(char)0x3E,(char)0x4B,(char)0x17,(char)0x0B,(char)0x2F,(char)0x73,(char)0x63,(char)0x70,(char)0x64,(char)0x2E
		,(char)0x78,(char)0x6D,(char)0x6C,(char)0x3C,(char)0x2F,(char)0x88,(char)0x07,(char)0x08,(char)0x3C,(char)0x63
		,(char)0x6F,(char)0x6E,(char)0x74,(char)0x72,(char)0x6F,(char)0x6C,(char)0x90,(char)0x0A,(char)0x00,(char)0xC7
		,(char)0x05,(char)0x02,(char)0x3C,(char)0x2F,(char)0x0B,(char)0x08,(char)0x09,(char)0x3C,(char)0x65,(char)0x76
		,(char)0x65,(char)0x6E,(char)0x74,(char)0x53,(char)0x75,(char)0x62,(char)0xD0,(char)0x15,(char)0x00,(char)0x05
		,(char)0x06,(char)0x02,(char)0x3C,(char)0x2F,(char)0xCC,(char)0x07,(char)0x00,(char)0x89,(char)0x31,(char)0x01
		,(char)0x3E,(char)0x00,(char)0x00
	},
	153,
	272,
};
struct UPnP_Service_ConnectionManager UPnP_Service_ConnectionManager_Impl =
{
	&UPnP_Action_ConnectionManager_GetCurrentConnectionIDs_Impl,
	&UPnP_Action_ConnectionManager_GetCurrentConnectionInfo_Impl,
	&UPnP_Action_ConnectionManager_GetProtocolInfo_Impl,
	&UPnP_StateVariable_ConnectionManager_A_ARG_TYPE_ConnectionStatus_Impl,
	&UPnP_StateVariable_ConnectionManager_A_ARG_TYPE_ConnectionID_Impl,
	&UPnP_StateVariable_ConnectionManager_A_ARG_TYPE_Direction_Impl,
	&UPnP_StateVariable_ConnectionManager_A_ARG_TYPE_RcsID_Impl,
	&UPnP_StateVariable_ConnectionManager_SourceProtocolInfo_Impl,
	&UPnP_StateVariable_ConnectionManager_SinkProtocolInfo_Impl,
	&UPnP_StateVariable_ConnectionManager_A_ARG_TYPE_ProtocolInfo_Impl,
	&UPnP_StateVariable_ConnectionManager_A_ARG_TYPE_ConnectionManager_Impl,
	&UPnP_StateVariable_ConnectionManager_A_ARG_TYPE_AVTransportID_Impl,
	&UPnP_StateVariable_ConnectionManager_CurrentConnectionIDs_Impl,
	{
		(char)0x09,(char)0x3C,(char)0x73,(char)0x65,(char)0x72,(char)0x76,(char)0x69,(char)0x63,(char)0x65,(char)0x3E
		,(char)0x48,(char)0x02,(char)0x1A,(char)0x54,(char)0x79,(char)0x70,(char)0x65,(char)0x3E,(char)0x75,(char)0x72
		,(char)0x6E,(char)0x3A,(char)0x73,(char)0x63,(char)0x68,(char)0x65,(char)0x6D,(char)0x61,(char)0x73,(char)0x2D
		,(char)0x75,(char)0x70,(char)0x6E,(char)0x70,(char)0x2D,(char)0x6F,(char)0x72,(char)0x67,(char)0x3A,(char)0x87
		,(char)0x0A,(char)0x16,(char)0x3A,(char)0x43,(char)0x6F,(char)0x6E,(char)0x6E,(char)0x65,(char)0x63,(char)0x74
		,(char)0x69,(char)0x6F,(char)0x6E,(char)0x4D,(char)0x61,(char)0x6E,(char)0x61,(char)0x67,(char)0x65,(char)0x72
		,(char)0x3A,(char)0x31,(char)0x3C,(char)0x2F,(char)0x8C,(char)0x0F,(char)0x00,(char)0x08,(char)0x15,(char)0x02
		,(char)0x49,(char)0x64,(char)0x45,(char)0x12,(char)0x00,(char)0x50,(char)0x10,(char)0x02,(char)0x49,(char)0x64
		,(char)0xD2,(char)0x10,(char)0x02,(char)0x3C,(char)0x2F,(char)0x0A,(char)0x0D,(char)0x09,(char)0x3C,(char)0x53
		,(char)0x43,(char)0x50,(char)0x44,(char)0x55,(char)0x52,(char)0x4C,(char)0x3E,(char)0x51,(char)0x1A,(char)0x0B
		,(char)0x2F,(char)0x73,(char)0x63,(char)0x70,(char)0x64,(char)0x2E,(char)0x78,(char)0x6D,(char)0x6C,(char)0x3C
		,(char)0x2F,(char)0x08,(char)0x09,(char)0x08,(char)0x3C,(char)0x63,(char)0x6F,(char)0x6E,(char)0x74,(char)0x72
		,(char)0x6F,(char)0x6C,(char)0x16,(char)0x0C,(char)0x00,(char)0x47,(char)0x07,(char)0x02,(char)0x3C,(char)0x2F
		,(char)0x8B,(char)0x09,(char)0x09,(char)0x3C,(char)0x65,(char)0x76,(char)0x65,(char)0x6E,(char)0x74,(char)0x53
		,(char)0x75,(char)0x62,(char)0xD6,(char)0x18,(char)0x00,(char)0x85,(char)0x07,(char)0x02,(char)0x3C,(char)0x2F
		,(char)0x4C,(char)0x09,(char)0x00,(char)0x89,(char)0x37,(char)0x01,(char)0x3E,(char)0x00,(char)0x00
	},
	159,
	302,
};
struct UPnP_Service_RenderingControl UPnP_Service_RenderingControl_Impl =
{
	&UPnP_Action_RenderingControl_GetMute_Impl,
	&UPnP_Action_RenderingControl_GetVolume_Impl,
	&UPnP_Action_RenderingControl_ListPresets_Impl,
	&UPnP_Action_RenderingControl_SelectPreset_Impl,
	&UPnP_Action_RenderingControl_SetMute_Impl,
	&UPnP_Action_RenderingControl_SetVolume_Impl,
	&UPnP_Action_RenderingControl_upnpRenderer_GetMediaPlayersList_Impl,
	&UPnP_Action_RenderingControl_upnpRenderer_SetMediaPlayer_Impl,
	&UPnP_StateVariable_RenderingControl_Mute_Impl,
	&UPnP_StateVariable_RenderingControl_A_ARG_TYPE_Channel_Impl,
	&UPnP_StateVariable_RenderingControl_A_ARG_TYPE_InstanceID_Impl,
	&UPnP_StateVariable_RenderingControl_Volume_Impl,
	&UPnP_StateVariable_RenderingControl_A_ARG_TYPE_MediaPlayerName_Impl,
	&UPnP_StateVariable_RenderingControl_A_ARG_TYPE_PresetName_Impl,
	&UPnP_StateVariable_RenderingControl_PresetNameList_Impl,
	&UPnP_StateVariable_RenderingControl_LastChange_Impl,
	&UPnP_StateVariable_RenderingControl_MediaPlayerName_Impl,
	{
		(char)0x09,(char)0x3C,(char)0x73,(char)0x65,(char)0x72,(char)0x76,(char)0x69,(char)0x63,(char)0x65,(char)0x3E
		,(char)0x48,(char)0x02,(char)0x1A,(char)0x54,(char)0x79,(char)0x70,(char)0x65,(char)0x3E,(char)0x75,(char)0x72
		,(char)0x6E,(char)0x3A,(char)0x73,(char)0x63,(char)0x68,(char)0x65,(char)0x6D,(char)0x61,(char)0x73,(char)0x2D
		,(char)0x75,(char)0x70,(char)0x6E,(char)0x70,(char)0x2D,(char)0x6F,(char)0x72,(char)0x67,(char)0x3A,(char)0x87
		,(char)0x0A,(char)0x15,(char)0x3A,(char)0x52,(char)0x65,(char)0x6E,(char)0x64,(char)0x65,(char)0x72,(char)0x69
		,(char)0x6E,(char)0x67,(char)0x43,(char)0x6F,(char)0x6E,(char)0x74,(char)0x72,(char)0x6F,(char)0x6C,(char)0x3A
		,(char)0x31,(char)0x3C,(char)0x2F,(char)0x4C,(char)0x0F,(char)0x00,(char)0xC8,(char)0x14,(char)0x02,(char)0x49
		,(char)0x64,(char)0x05,(char)0x12,(char)0x00,(char)0x10,(char)0x10,(char)0x02,(char)0x49,(char)0x64,(char)0x91
		,(char)0x10,(char)0x02,(char)0x3C,(char)0x2F,(char)0xCA,(char)0x0C,(char)0x09,(char)0x3C,(char)0x53,(char)0x43
		,(char)0x50,(char)0x44,(char)0x55,(char)0x52,(char)0x4C,(char)0x3E,(char)0xD0,(char)0x19,(char)0x0B,(char)0x2F
		,(char)0x73,(char)0x63,(char)0x70,(char)0x64,(char)0x2E,(char)0x78,(char)0x6D,(char)0x6C,(char)0x3C,(char)0x2F
		,(char)0xC8,(char)0x08,(char)0x02,(char)0x3C,(char)0x63,(char)0x86,(char)0x20,(char)0x00,(char)0xD5,(char)0x0B
		,(char)0x01,(char)0x63,(char)0x08,(char)0x17,(char)0x00,(char)0x4B,(char)0x09,(char)0x09,(char)0x3C,(char)0x65
		,(char)0x76,(char)0x65,(char)0x6E,(char)0x74,(char)0x53,(char)0x75,(char)0x62,(char)0x55,(char)0x18,(char)0x00
		,(char)0x45,(char)0x07,(char)0x02,(char)0x3C,(char)0x2F,(char)0x0C,(char)0x09,(char)0x00,(char)0x89,(char)0x36
		,(char)0x01,(char)0x3E,(char)0x00,(char)0x00
	},
	154,
	297,
};
struct UPnP_Device_MediaRenderer UPnP_Device_MediaRenderer_Impl = 
{
	&UPnP_Service_AVTransport_Impl,
	&UPnP_Service_ConnectionManager_Impl,
	&UPnP_Service_RenderingControl_Impl,
	
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	{
		(char)0x61,(char)0x3C,(char)0x3F,(char)0x78,(char)0x6D,(char)0x6C,(char)0x20,(char)0x76,(char)0x65,(char)0x72
		,(char)0x73,(char)0x69,(char)0x6F,(char)0x6E,(char)0x3D,(char)0x22,(char)0x31,(char)0x2E,(char)0x30,(char)0x22
		,(char)0x20,(char)0x65,(char)0x6E,(char)0x63,(char)0x6F,(char)0x64,(char)0x69,(char)0x6E,(char)0x67,(char)0x3D
		,(char)0x22,(char)0x75,(char)0x74,(char)0x66,(char)0x2D,(char)0x38,(char)0x22,(char)0x3F,(char)0x3E,(char)0x0D
		,(char)0x0A,(char)0x3C,(char)0x72,(char)0x6F,(char)0x6F,(char)0x74,(char)0x20,(char)0x78,(char)0x6D,(char)0x6C
		,(char)0x6E,(char)0x73,(char)0x3D,(char)0x22,(char)0x75,(char)0x72,(char)0x6E,(char)0x3A,(char)0x73,(char)0x63
		,(char)0x68,(char)0x65,(char)0x6D,(char)0x61,(char)0x73,(char)0x2D,(char)0x75,(char)0x70,(char)0x6E,(char)0x70
		,(char)0x2D,(char)0x6F,(char)0x72,(char)0x67,(char)0x3A,(char)0x64,(char)0x65,(char)0x76,(char)0x69,(char)0x63
		,(char)0x65,(char)0x2D,(char)0x31,(char)0x2D,(char)0x30,(char)0x22,(char)0x3E,(char)0x0D,(char)0x0A,(char)0x20
		,(char)0x20,(char)0x20,(char)0x3C,(char)0x73,(char)0x70,(char)0x65,(char)0x63,(char)0x56,(char)0x86,(char)0x16
		,(char)0x00,(char)0x86,(char)0x04,(char)0x00,(char)0x44,(char)0x05,(char)0x09,(char)0x6D,(char)0x61,(char)0x6A
		,(char)0x6F,(char)0x72,(char)0x3E,(char)0x31,(char)0x3C,(char)0x2F,(char)0x46,(char)0x02,(char)0x00,(char)0x0A
		,(char)0x06,(char)0x0B,(char)0x69,(char)0x6E,(char)0x6F,(char)0x72,(char)0x3E,(char)0x30,(char)0x3C,(char)0x2F
		,(char)0x6D,(char)0x69,(char)0x6E,(char)0x08,(char)0x06,(char)0x02,(char)0x3C,(char)0x2F,(char)0xD1,(char)0x10
		,(char)0x01,(char)0x3C,(char)0xC6,(char)0x19,(char)0x00,(char)0x0A,(char)0x14,(char)0x00,(char)0xC6,(char)0x1D
		,(char)0x05,(char)0x54,(char)0x79,(char)0x70,(char)0x65,(char)0x3E,(char)0xDB,(char)0x25,(char)0x12,(char)0x3A
		,(char)0x4D,(char)0x65,(char)0x64,(char)0x69,(char)0x61,(char)0x52,(char)0x65,(char)0x6E,(char)0x64,(char)0x65
		,(char)0x72,(char)0x65,(char)0x72,(char)0x3A,(char)0x31,(char)0x3C,(char)0x2F,(char)0x0B,(char)0x0E,(char)0x00
		,(char)0x09,(char)0x27,(char)0x11,(char)0x66,(char)0x72,(char)0x69,(char)0x65,(char)0x6E,(char)0x64,(char)0x6C
		,(char)0x79,(char)0x4E,(char)0x61,(char)0x6D,(char)0x65,(char)0x3E,(char)0x25,(char)0x73,(char)0x3C,(char)0x2F
		,(char)0x4D,(char)0x04,(char)0x00,(char)0xCB,(char)0x30,(char)0x0A,(char)0x6E,(char)0x75,(char)0x66,(char)0x61
		,(char)0x63,(char)0x74,(char)0x75,(char)0x72,(char)0x65,(char)0x72,(char)0xC5,(char)0x09,(char)0x00,(char)0x4D
		,(char)0x04,(char)0x00,(char)0xD5,(char)0x09,(char)0x03,(char)0x55,(char)0x52,(char)0x4C,(char)0x91,(char)0x0A
		,(char)0x03,(char)0x55,(char)0x52,(char)0x4C,(char)0xCB,(char)0x45,(char)0x0F,(char)0x6F,(char)0x64,(char)0x65
		,(char)0x6C,(char)0x44,(char)0x65,(char)0x73,(char)0x63,(char)0x72,(char)0x69,(char)0x70,(char)0x74,(char)0x69
		,(char)0x6F,(char)0x6E,(char)0x06,(char)0x16,(char)0x00,(char)0x50,(char)0x05,(char)0x00,(char)0xCE,(char)0x0B
		,(char)0x00,(char)0xC9,(char)0x29,(char)0x00,(char)0x8A,(char)0x03,(char)0x00,(char)0x4F,(char)0x08,(char)0x05
		,(char)0x75,(char)0x6D,(char)0x62,(char)0x65,(char)0x72,(char)0xCB,(char)0x08,(char)0x03,(char)0x75,(char)0x6D
		,(char)0x62,(char)0x8D,(char)0x28,(char)0x00,(char)0x44,(char)0x1D,(char)0x00,(char)0xC4,(char)0x26,(char)0x0B
		,(char)0x68,(char)0x74,(char)0x74,(char)0x70,(char)0x3A,(char)0x2F,(char)0x2F,(char)0x32,(char)0x35,(char)0x35
		,(char)0x2E,(char)0x04,(char)0x01,(char)0x00,(char)0x07,(char)0x02,(char)0x07,(char)0x3A,(char)0x32,(char)0x35
		,(char)0x35,(char)0x2F,(char)0x3C,(char)0x2F,(char)0x89,(char)0x09,(char)0x00,(char)0x09,(char)0x71,(char)0x05
		,(char)0x73,(char)0x65,(char)0x72,(char)0x69,(char)0x61,(char)0x8C,(char)0x17,(char)0x00,(char)0x4D,(char)0x04
		,(char)0x00,(char)0xC9,(char)0x7A,(char)0x09,(char)0x55,(char)0x44,(char)0x4E,(char)0x3E,(char)0x75,(char)0x75
		,(char)0x69,(char)0x64,(char)0x3A,(char)0xC4,(char)0x52,(char)0x03,(char)0x55,(char)0x44,(char)0x4E,(char)0x4A
		,(char)0x81,(char)0x08,(char)0x69,(char)0x63,(char)0x6F,(char)0x6E,(char)0x4C,(char)0x69,(char)0x73,(char)0x74
		,(char)0xC9,(char)0x85,(char)0x00,(char)0x48,(char)0x05,(char)0x00,(char)0x4C,(char)0x04,(char)0x00,(char)0x86
		,(char)0x85,(char)0x03,(char)0x6D,(char)0x65,(char)0x74,(char)0x04,(char)0x77,(char)0x0B,(char)0x69,(char)0x6D
		,(char)0x61,(char)0x67,(char)0x65,(char)0x2F,(char)0x70,(char)0x6E,(char)0x67,(char)0x3C,(char)0x2F,(char)0x09
		,(char)0x05,(char)0x00,(char)0x0F,(char)0x0B,(char)0x0A,(char)0x77,(char)0x69,(char)0x64,(char)0x74,(char)0x68
		,(char)0x3E,(char)0x34,(char)0x38,(char)0x3C,(char)0x2F,(char)0x86,(char)0x02,(char)0x00,(char)0xCF,(char)0x12
		,(char)0x06,(char)0x68,(char)0x65,(char)0x69,(char)0x67,(char)0x68,(char)0x74,(char)0x05,(char)0x08,(char)0x00
		,(char)0xC7,(char)0x02,(char)0x00,(char)0x0F,(char)0x1B,(char)0x0A,(char)0x64,(char)0x65,(char)0x70,(char)0x74
		,(char)0x68,(char)0x3E,(char)0x33,(char)0x32,(char)0x3C,(char)0x2F,(char)0x86,(char)0x02,(char)0x00,(char)0xCF
		,(char)0x22,(char)0x05,(char)0x75,(char)0x72,(char)0x6C,(char)0x3E,(char)0x2F,(char)0x44,(char)0x2E,(char)0x01
		,(char)0x2E,(char)0x85,(char)0x21,(char)0x03,(char)0x75,(char)0x72,(char)0x6C,(char)0x8D,(char)0x2F,(char)0x01
		,(char)0x2F,(char)0xD0,(char)0x2F,(char)0x00,(char)0x24,(char)0x34,(char)0x02,(char)0x6A,(char)0x70,(char)0x3F
		,(char)0x34,(char)0x00,(char)0x3F,(char)0x34,(char)0x00,(char)0x06,(char)0x34,(char)0x02,(char)0x6A,(char)0x70
		,(char)0x3F,(char)0x34,(char)0x00,(char)0x3F,(char)0x68,(char)0x00,(char)0x3F,(char)0x68,(char)0x00,(char)0x10
		,(char)0x68,(char)0x01,(char)0x32,(char)0x7F,(char)0x68,(char)0x00,(char)0x7F,(char)0x68,(char)0x00,(char)0x7F
		,(char)0x34,(char)0x00,(char)0x55,(char)0x34,(char)0x00,(char)0xA3,(char)0x68,(char)0x02,(char)0x3C,(char)0x2F
		,(char)0x4E,(char)0xD5,(char)0x00,(char)0x84,(char)0xB1,(char)0x03,(char)0x76,(char)0x69,(char)0x63,(char)0x84
		,(char)0xC9,(char)0x04,(char)0x3C,(char)0x2F,(char)0x72,(char)0x6F,(char)0x00,(char)0x00,(char)0x03,(char)0x6F
		,(char)0x74,(char)0x3E,(char)0x00,(char)0x00
	},
	534,
	1515,
	NULL,
	NULL
};

UPnPMicroStackToken UPnPCreateMicroStack(void *Chain, const char* FriendlyName, const char* UDN, const char* SerialNumber, const int NotifyCycleSeconds, const unsigned short PortNum);
/* UPnP Set Function Pointers Methods */
void (*UPnPFP_PresentationPage) (void* upnptoken,struct packetheader *packet);
/*! \var UPnPFP_AVTransport_GetCurrentTransportActions
\brief Dispatch Pointer for AVTransport >> urn:schemas-upnp-org:service:AVTransport:1 >> GetCurrentTransportActions
*/
UPnP_ActionHandler_AVTransport_GetCurrentTransportActions UPnPFP_AVTransport_GetCurrentTransportActions;
/*! \var UPnPFP_AVTransport_GetDeviceCapabilities
\brief Dispatch Pointer for AVTransport >> urn:schemas-upnp-org:service:AVTransport:1 >> GetDeviceCapabilities
*/
UPnP_ActionHandler_AVTransport_GetDeviceCapabilities UPnPFP_AVTransport_GetDeviceCapabilities;
/*! \var UPnPFP_AVTransport_GetMediaInfo
\brief Dispatch Pointer for AVTransport >> urn:schemas-upnp-org:service:AVTransport:1 >> GetMediaInfo
*/
UPnP_ActionHandler_AVTransport_GetMediaInfo UPnPFP_AVTransport_GetMediaInfo;
/*! \var UPnPFP_AVTransport_GetPositionInfo
\brief Dispatch Pointer for AVTransport >> urn:schemas-upnp-org:service:AVTransport:1 >> GetPositionInfo
*/
UPnP_ActionHandler_AVTransport_GetPositionInfo UPnPFP_AVTransport_GetPositionInfo;
/*! \var UPnPFP_AVTransport_GetTransportInfo
\brief Dispatch Pointer for AVTransport >> urn:schemas-upnp-org:service:AVTransport:1 >> GetTransportInfo
*/
UPnP_ActionHandler_AVTransport_GetTransportInfo UPnPFP_AVTransport_GetTransportInfo;
/*! \var UPnPFP_AVTransport_GetTransportSettings
\brief Dispatch Pointer for AVTransport >> urn:schemas-upnp-org:service:AVTransport:1 >> GetTransportSettings
*/
UPnP_ActionHandler_AVTransport_GetTransportSettings UPnPFP_AVTransport_GetTransportSettings;
/*! \var UPnPFP_AVTransport_Next
\brief Dispatch Pointer for AVTransport >> urn:schemas-upnp-org:service:AVTransport:1 >> Next
*/
UPnP_ActionHandler_AVTransport_Next UPnPFP_AVTransport_Next;
/*! \var UPnPFP_AVTransport_Pause
\brief Dispatch Pointer for AVTransport >> urn:schemas-upnp-org:service:AVTransport:1 >> Pause
*/
UPnP_ActionHandler_AVTransport_Pause UPnPFP_AVTransport_Pause;
/*! \var UPnPFP_AVTransport_Play
\brief Dispatch Pointer for AVTransport >> urn:schemas-upnp-org:service:AVTransport:1 >> Play
*/
UPnP_ActionHandler_AVTransport_Play UPnPFP_AVTransport_Play;
/*! \var UPnPFP_AVTransport_Previous
\brief Dispatch Pointer for AVTransport >> urn:schemas-upnp-org:service:AVTransport:1 >> Previous
*/
UPnP_ActionHandler_AVTransport_Previous UPnPFP_AVTransport_Previous;
/*! \var UPnPFP_AVTransport_Seek
\brief Dispatch Pointer for AVTransport >> urn:schemas-upnp-org:service:AVTransport:1 >> Seek
*/
UPnP_ActionHandler_AVTransport_Seek UPnPFP_AVTransport_Seek;
/*! \var UPnPFP_AVTransport_SetAVTransportURI
\brief Dispatch Pointer for AVTransport >> urn:schemas-upnp-org:service:AVTransport:1 >> SetAVTransportURI
*/
UPnP_ActionHandler_AVTransport_SetAVTransportURI UPnPFP_AVTransport_SetAVTransportURI;
/*! \var UPnPFP_AVTransport_SetPlayMode
\brief Dispatch Pointer for AVTransport >> urn:schemas-upnp-org:service:AVTransport:1 >> SetPlayMode
*/
UPnP_ActionHandler_AVTransport_SetPlayMode UPnPFP_AVTransport_SetPlayMode;
/*! \var UPnPFP_AVTransport_Stop
\brief Dispatch Pointer for AVTransport >> urn:schemas-upnp-org:service:AVTransport:1 >> Stop
*/
UPnP_ActionHandler_AVTransport_Stop UPnPFP_AVTransport_Stop;
/*! \var UPnPFP_ConnectionManager_GetCurrentConnectionIDs
\brief Dispatch Pointer for ConnectionManager >> urn:schemas-upnp-org:service:ConnectionManager:1 >> GetCurrentConnectionIDs
*/
UPnP_ActionHandler_ConnectionManager_GetCurrentConnectionIDs UPnPFP_ConnectionManager_GetCurrentConnectionIDs;
/*! \var UPnPFP_ConnectionManager_GetCurrentConnectionInfo
\brief Dispatch Pointer for ConnectionManager >> urn:schemas-upnp-org:service:ConnectionManager:1 >> GetCurrentConnectionInfo
*/
UPnP_ActionHandler_ConnectionManager_GetCurrentConnectionInfo UPnPFP_ConnectionManager_GetCurrentConnectionInfo;
/*! \var UPnPFP_ConnectionManager_GetProtocolInfo
\brief Dispatch Pointer for ConnectionManager >> urn:schemas-upnp-org:service:ConnectionManager:1 >> GetProtocolInfo
*/
UPnP_ActionHandler_ConnectionManager_GetProtocolInfo UPnPFP_ConnectionManager_GetProtocolInfo;
/*! \var UPnPFP_RenderingControl_GetMute
\brief Dispatch Pointer for RenderingControl >> urn:schemas-upnp-org:service:RenderingControl:1 >> GetMute
*/
UPnP_ActionHandler_RenderingControl_GetMute UPnPFP_RenderingControl_GetMute;
/*! \var UPnPFP_RenderingControl_GetVolume
\brief Dispatch Pointer for RenderingControl >> urn:schemas-upnp-org:service:RenderingControl:1 >> GetVolume
*/
UPnP_ActionHandler_RenderingControl_GetVolume UPnPFP_RenderingControl_GetVolume;
/*! \var UPnPFP_RenderingControl_ListPresets
\brief Dispatch Pointer for RenderingControl >> urn:schemas-upnp-org:service:RenderingControl:1 >> ListPresets
*/
UPnP_ActionHandler_RenderingControl_ListPresets UPnPFP_RenderingControl_ListPresets;
/*! \var UPnPFP_RenderingControl_SelectPreset
\brief Dispatch Pointer for RenderingControl >> urn:schemas-upnp-org:service:RenderingControl:1 >> SelectPreset
*/
UPnP_ActionHandler_RenderingControl_SelectPreset UPnPFP_RenderingControl_SelectPreset;
/*! \var UPnPFP_RenderingControl_SetMute
\brief Dispatch Pointer for RenderingControl >> urn:schemas-upnp-org:service:RenderingControl:1 >> SetMute
*/
UPnP_ActionHandler_RenderingControl_SetMute UPnPFP_RenderingControl_SetMute;
/*! \var UPnPFP_RenderingControl_SetVolume
\brief Dispatch Pointer for RenderingControl >> urn:schemas-upnp-org:service:RenderingControl:1 >> SetVolume
*/
UPnP_ActionHandler_RenderingControl_SetVolume UPnPFP_RenderingControl_SetVolume;
/*! \var UPnPFP_RenderingControl_upnpRenderer_GetMediaPlayersList
\brief Dispatch Pointer for RenderingControl >> urn:schemas-upnp-org:service:RenderingControl:1 >> upnpRenderer_GetMediaPlayersList
*/
UPnP_ActionHandler_RenderingControl_upnpRenderer_GetMediaPlayersList UPnPFP_RenderingControl_upnpRenderer_GetMediaPlayersList;
/*! \var UPnPFP_RenderingControl_upnpRenderer_SetMediaPlayer
\brief Dispatch Pointer for RenderingControl >> urn:schemas-upnp-org:service:RenderingControl:1 >> upnpRenderer_SetMediaPlayer
*/
UPnP_ActionHandler_RenderingControl_upnpRenderer_SetMediaPlayer UPnPFP_RenderingControl_upnpRenderer_SetMediaPlayer;


#if defined(WIN32)
#pragma warning( pop )
#endif

const char UPnPDeviceIcon_LGPNG[4058]={
	0x48,0x54,0x54,0x50,0x2F,0x31,0x2E,0x31,0x20,0x32,0x30,0x30,0x20,0x4F,0x4B,0x0D,0x0A,0x43,0x4F,0x4E
	,0x54,0x45,0x4E,0x54,0x2D,0x54,0x59,0x50,0x45,0x3A,0x20,0x69,0x6D,0x61,0x67,0x65,0x2F,0x70,0x6E,0x67
	,0x0D,0x0A,0x43,0x6F,0x6E,0x74,0x65,0x6E,0x74,0x2D,0x4C,0x65,0x6E,0x67,0x74,0x68,0x3A,0x20,0x33,0x39
	,0x39,0x32,0x0D,0x0A,0x0D,0x0A,0x89,0x50,0x4E,0x47,0x0D,0x0A,0x1A,0x0A,0x00,0x00,0x00,0x0D,0x49,0x48
	,0x44,0x52,0x00,0x00,0x00,0x30,0x00,0x00,0x00,0x30,0x08,0x06,0x00,0x00,0x00,0x57,0x02,0xF9,0x87,0x00
	,0x00,0x00,0x01,0x73,0x52,0x47,0x42,0x00,0xAE,0xCE,0x1C,0xE9,0x00,0x00,0x00,0x04,0x67,0x41,0x4D,0x41
	,0x00,0x00,0xB1,0x8F,0x0B,0xFC,0x61,0x05,0x00,0x00,0x00,0x09,0x70,0x48,0x59,0x73,0x00,0x00,0x0E,0xC3
	,0x00,0x00,0x0E,0xC3,0x01,0xC7,0x6F,0xA8,0x64,0x00,0x00,0x0F,0x2D,0x49,0x44,0x41,0x54,0x68,0x43,0xED
	,0x59,0x07,0x50,0x54,0xE7,0xBE,0x37,0xC9,0xDC,0xB9,0x6F,0xDE,0xCC,0x9D,0x7B,0xDF,0xF3,0x4E,0xF2,0x72
	,0x4B,0x5E,0x9E,0x31,0x36,0x54,0xA2,0x62,0x08,0xA2,0x44,0xB1,0x61,0x49,0xD0,0x28,0x28,0x4A,0xB1,0x21
	,0x88,0x80,0x8A,0x11,0xA2,0xE2,0x55,0x44,0x51,0xA4,0x08,0x88,0x10,0x95,0x22,0x1D,0x0B,0x16,0x8A,0x14
	,0x69,0x22,0x75,0x59,0x8A,0x60,0xA3,0x0A,0xEA,0x8D,0x8A,0xBD,0x04,0xD8,0x3D,0x7B,0xF6,0xF7,0xFE,0xDF
	,0x39,0xBB,0xCB,0x02,0x1B,0x63,0x30,0x99,0x3B,0x6F,0xE6,0x9D,0x99,0xDF,0x9C,0x3D,0x87,0xAF,0xFC,0x7F
	,0xFF,0xFE,0x1D,0x06,0x25,0xDC,0x91,0xE3,0xFF,0x32,0xFE,0x9F,0xC0,0xBF,0x1A,0x6F,0x47,0xE0,0xAE,0x0C
	,0xF1,0x77,0x38,0xFA,0xCD,0xA0,0xE3,0xEF,0xFD,0xA0,0x1A,0x7B,0x5B,0x41,0x77,0x99,0xD6,0xFB,0x81,0xE3
	,0x2D,0x08,0x70,0xF8,0x2E,0xFB,0x02,0x16,0x87,0x7B,0x21,0xA4,0xAE,0x59,0x45,0x44,0xD7,0x38,0x2D,0xDC
	,0xED,0xC6,0xF1,0xD6,0xE7,0x88,0x6D,0xEB,0xA4,0x67,0x46,0x42,0xC7,0x98,0x5F,0x88,0x01,0x13,0x88,0x57
	,0xC1,0xEB,0x52,0x3E,0x86,0x3B,0x4D,0xC3,0xD2,0xEF,0x77,0x23,0xAE,0xFD,0x15,0xE2,0x34,0x63,0x98,0x86
	,0x39,0xC4,0xDF,0xEE,0xC4,0xB6,0xAC,0x4C,0x2C,0x08,0xDA,0x8E,0x35,0xF1,0xE1,0xF0,0x2F,0xBF,0x46,0xEF
	,0x7E,0x1D,0xED,0x33,0x0C,0x98,0xC0,0x91,0xEB,0xF7,0xB0,0x2A,0xF6,0x10,0x6C,0xA3,0x03,0x88,0x44,0x21
	,0xBE,0x3E,0xF8,0x2D,0xA6,0xEE,0x5E,0x29,0x08,0xAC,0x76,0x29,0x7F,0x69,0x2D,0x26,0xB8,0x2F,0x84,0x5D
	,0x74,0x20,0x22,0x9B,0x1F,0xA9,0x48,0x33,0xB7,0x13,0xC9,0xF7,0x5D,0x73,0x20,0x78,0xAB,0x18,0x88,0x6B
	,0xFF,0x11,0xDB,0x73,0xB2,0x30,0xCD,0x6B,0x35,0xA6,0xEF,0x75,0xC4,0x8C,0xBD,0xAB,0x61,0x17,0x13,0x48
	,0xC2,0x71,0x08,0xAC,0xAE,0xC3,0x07,0x56,0x43,0x31,0xC7,0x6F,0x3D,0xAC,0xA3,0x0E,0xC0,0x3E,0xF1,0x08
	,0x36,0xA5,0x9F,0x81,0x9F,0xB4,0x1E,0x71,0x02,0xC9,0x7F,0x59,0x0C,0x30,0xED,0xB2,0xCD,0x09,0xB7,0x59
	,0x40,0xFE,0x48,0x2E,0x92,0x0E,0x3D,0x47,0x43,0x8C,0xDB,0x32,0x0B,0x1F,0xDA,0xE8,0x91,0x75,0xDA,0x60
	,0xEA,0x36,0x17,0x29,0xC9,0x07,0x91,0x99,0x14,0x82,0x93,0x71,0x07,0x10,0x1E,0xFE,0x1D,0x3C,0xF6,0xDA
	,0x61,0x96,0x8B,0x11,0x86,0x5A,0x8F,0x82,0xD9,0x3E,0x47,0x78,0x5F,0x2E,0xA1,0xF9,0xDD,0xB4,0x16,0x8B
	,0x07,0xED,0x98,0x60,0x7B,0xBC,0x59,0x8C,0x0C,0x80,0x80,0xA8,0xB9,0xD8,0xB6,0x17,0x70,0x3E,0x15,0x07
	,0xD3,0x6F,0xE7,0x62,0xA7,0xAF,0x03,0x9A,0x0A,0x32,0x60,0xE6,0xFC,0x05,0x7E,0x3F,0xFF,0xDF,0x31,0x7A
	,0x9D,0x21,0x0A,0xCE,0x1E,0x85,0x52,0x5A,0x09,0x10,0xD8,0x9D,0x81,0x97,0x4A,0xA1,0x20,0xBC,0x94,0x94
	,0x20,0x83,0x88,0x59,0x6D,0x31,0xC3,0xF8,0x0D,0x33,0xB1,0x33,0x3F,0xBF,0xF7,0x1E,0xB7,0x79,0x82,0x92
	,0x7E,0xFF,0x7C,0x62,0xF8,0x05,0x04,0x38,0x24,0xB2,0xF4,0x77,0xBB,0x0B,0xAE,0xA7,0xE2,0xB1,0x68,0x9B
	,0x25,0x52,0xE2,0x7D,0xD1,0x55,0x59,0x0A,0x45,0x95,0x14,0x5C,0x55,0x25,0xAE,0xE6,0x9C,0xC5,0x9F,0xE7
	,0xFD,0x01,0x11,0x91,0xBB,0xA0,0xAC,0x14,0x85,0x56,0x13,0xE0,0xD9,0x9D,0xC6,0xF0,0x52,0x89,0xF0,0x9B
	,0x97,0x56,0x09,0x63,0xA4,0x99,0x49,0x30,0x5D,0x3B,0x11,0xD3,0xBC,0x57,0xE1,0x58,0xE3,0x3D,0x72,0x3F
	,0x85,0x80,0xE8,0x36,0x19,0x22,0xDB,0xD8,0x9E,0xBC,0xE0,0x92,0xBA,0x65,0xFA,0x85,0x16,0x38,0xDA,0xF0
	,0x03,0x66,0x78,0x2E,0x43,0x5C,0x94,0x37,0xE4,0x4C,0xA0,0x4A,0x26,0x84,0x14,0x8F,0x8B,0xF3,0x90,0x91
	,0x1C,0x8C,0xCB,0xE7,0xA2,0x50,0x96,0x9E,0x00,0x79,0x95,0x44,0x20,0xC5,0x93,0xC0,0x2F,0x24,0x97,0x51
	,0x9E,0x16,0x8B,0xA2,0x33,0x51,0x68,0xC9,0x4B,0x07,0x5F,0x29,0xD1,0x58,0x44,0x0D,0x99,0xB4,0x02,0xFE
	,0x21,0x1B,0xF1,0xC9,0x8A,0xB1,0xF0,0x2D,0xAF,0xA0,0xBD,0x14,0x08,0x6F,0x95,0x23,0xA0,0xA5,0x0B,0x49
	,0x64,0x89,0xB7,0x24,0xC0,0x26,0x73,0x24,0xFC,0x3D,0xE8,0xAF,0x33,0x21,0x21,0xA3,0x05,0xCD,0x29,0x48
	,0x48,0xE6,0x12,0x0C,0xFF,0x2C,0xCA,0x84,0x6F,0xE0,0x46,0xD8,0x7B,0x2E,0xC4,0x88,0x25,0x1F,0x61,0xB3
	,0xB7,0x0D,0x9E,0x95,0x17,0x23,0x21,0xC2,0x07,0xF3,0xD7,0x4F,0x86,0x8D,0xC7,0x1C,0x78,0x1D,0x70,0x40
	,0x76,0x52,0x28,0x59,0xAC,0x42,0x35,0x57,0xB4,0x84,0x92,0xE6,0xAB,0x89,0x14,0x9D,0x8D,0xC0,0xA7,0xCB
	,0x46,0x60,0x47,0x7E,0x36,0x42,0x9A,0xBB,0xB0,0xF7,0x46,0x37,0x92,0xDA,0x99,0x45,0xDE,0x82,0x00,0x9B
	,0x1C,0xDD,0xF2,0x08,0x63,0x9D,0xA7,0x22,0x31,0xD2,0x47,0xD8,0x9C,0x69,0xB7,0xB3,0xAC,0x0C,0xE1,0xDB
	,0x3D,0x61,0x66,0x34,0x09,0x93,0x0D,0x0C,0x30,0x7D,0xD2,0x64,0x2C,0x9A,0x31,0x03,0xFE,0x1E,0x6E,0x08
	,0x09,0xF2,0x40,0x07,0x59,0x45,0x46,0x16,0xE0,0x68,0xEC,0xB3,0xD2,0x62,0xEC,0xDB,0xB0,0x11,0x06,0x23
	,0x47,0xE1,0x6F,0xEF,0x7F,0x80,0x8F,0x3E,0x78,0x1F,0x53,0xC6,0x8F,0x83,0xAF,0xDB,0x46,0x3C,0x2D,0x2E
	,0xD2,0x10,0x50,0xD0,0xF8,0x1A,0x72,0xA9,0x4F,0x96,0x0E,0x85,0x6B,0x6E,0x21,0xB6,0xD6,0xCB,0x90,0xD8
	,0x4E,0xF1,0x20,0xC4,0x9D,0x6E,0x12,0x3F,0x4B,0x80,0x15,0xA7,0x49,0x5B,0x17,0xC3,0xD3,0x77,0xB5,0xA8
	,0x39,0x42,0xDB,0x85,0x2C,0xCC,0x9E,0x32,0x19,0xBE,0x7E,0xBE,0xB8,0xD5,0xD6,0x06,0xA5,0x52,0x29,0xE0
	,0xE1,0xC3,0x87,0x08,0x0B,0x0B,0x83,0xE1,0x38,0x7D,0x64,0x1D,0x0E,0x13,0xE2,0xE2,0xE6,0xF9,0x73,0x30
	,0x1A,0x37,0x16,0xEE,0x1E,0x1E,0xB8,0x70,0xE1,0x02,0x4A,0x4B,0x4B,0x91,0x97,0x97,0x87,0xD4,0xD4,0x54
	,0xB8,0x6F,0xF1,0x80,0xC1,0x28,0x3D,0xD4,0x9F,0x4A,0x11,0x2D,0x21,0xC4,0x48,0x25,0xD2,0x4E,0x1E,0xC2
	,0x60,0x8B,0xA1,0x70,0x2E,0xBE,0x89,0x78,0x6A,0x57,0x44,0xE1,0x59,0x56,0xEA,0x4F,0x42,0x43,0x80,0x55
	,0x50,0x56,0x5C,0xA2,0x5B,0x9E,0x60,0xC3,0xB9,0x13,0xF8,0x2A,0xD0,0x1D,0xA6,0xFB,0xEC,0xA9,0x10,0xCD
	,0xC5,0x82,0x4D,0xA6,0xE8,0xAE,0x2C,0x17,0xB4,0xF4,0xA2,0xB8,0x18,0x5F,0x1A,0x1A,0xA2,0xAE,0xAE,0x9E
	,0x84,0x46,0xBF,0x8B,0xBD,0x6B,0x23,0x52,0x23,0x86,0x0D,0x43,0x18,0x59,0x68,0x82,0x9E,0x1E,0xA4,0x14
	,0xB0,0xBC,0x8E,0xC1,0x4A,0x25,0x8F,0x6B,0xD7,0xAE,0xC1,0x78,0xE2,0x44,0xDC,0xCB,0xCD,0xD5,0x58,0x82
	,0xA3,0xF1,0x6B,0x3D,0xBF,0xC1,0xDF,0x56,0xEA,0xE1,0xCB,0x9D,0xD6,0x30,0xFC,0x6E,0x01,0xA6,0x52,0x90
	,0x6F,0xCD,0x4C,0x25,0x59,0xBB,0x91,0xA8,0x92,0xB9,0x17,0x81,0x44,0x2A,0x2E,0xAB,0xE3,0x42,0x31,0xD2
	,0x76,0x34,0x0E,0x86,0xB8,0x21,0x2F,0xE5,0x28,0x52,0x4F,0x05,0xE1,0xB3,0x65,0x43,0xD1,0x51,0x52,0x20
	,0xFA,0x3B,0x69,0x68,0x9F,0xAB,0x33,0x82,0x82,0x43,0x04,0x8D,0xFF,0xD4,0xC5,0xFE,0x76,0x38,0x34,0x14
	,0xEF,0xBE,0xF7,0x1E,0x4E,0x24,0x27,0xFD,0xEC,0xD8,0xB4,0xB4,0x34,0xB8,0x2E,0xB3,0xEA,0x21,0x40,0x6E
	,0xD7,0x51,0x9A,0x07,0xFD,0x65,0x9F,0x22,0xFD,0xF4,0x21,0xD4,0x5C,0x38,0x81,0xA8,0x08,0x2F,0x98,0x38
	,0x4C,0xC0,0x74,0xAA,0xF6,0x51,0xCD,0x8F,0xB5,0x08,0x50,0x9A,0x62,0x95,0x71,0xA6,0x8F,0x03,0xA6,0xD8
	,0x4F,0x40,0x7B,0x51,0x16,0x2D,0x22,0x11,0xB4,0xD0,0x29,0x2D,0x45,0x7D,0xE6,0x69,0x61,0x51,0xE6,0x3A
	,0x0A,0xCA,0x38,0x13,0xF5,0x46,0xE3,0xD9,0xB3,0x67,0xAA,0xED,0x7B,0x2E,0x25,0xAF,0x40,0x43,0xE3,0x2B
	,0x64,0x67,0x3F,0xC3,0x3F,0xEF,0xBD,0x42,0x49,0x49,0x11,0x3E,0xFC,0xCB,0x5F,0xD1,0xD9,0xF9,0x0A,0xA7
	,0x4E,0x3F,0x41,0x50,0xC8,0x13,0x34,0xB7,0xBE,0x24,0x81,0x39,0xD5,0x8C,0x9E,0x8B,0xE3,0x38,0x72,0xBB
	,0x71,0xE8,0xAA,0x10,0xAD,0xAC,0xC6,0xAD,0xBC,0x0C,0xCA,0x70,0xF9,0xC2,0xDE,0xEC,0x59,0x56,0x55,0x81
	,0xA0,0xC3,0xEE,0xD0,0x77,0x99,0x81,0xC8,0xA6,0x87,0x44,0x40,0x21,0x5A,0xC0,0x31,0xE9,0x28,0x0C,0x56
	,0x8C,0xC2,0x23,0xDA,0x94,0x0D,0x14,0x72,0x37,0x09,0xAB,0xCE,0xD5,0x62,0xB6,0x90,0x40,0x56,0x5D,0x05
	,0x13,0x23,0x23,0xD5,0xB6,0xEA,0x8B,0xFC,0x1F,0x3C,0xCE,0xA7,0x3D,0xC7,0xBF,0xFD,0xFE,0x29,0x69,0xFD
	,0x47,0xFC,0xF1,0x8F,0x0F,0x11,0x13,0x7B,0x03,0xB3,0x67,0xCE,0x81,0x87,0xC7,0x03,0x7A,0xD7,0x89,0x77
	,0xDF,0xE9,0xC2,0xE0,0xC1,0xF7,0xD1,0xD0,0xF0,0x52,0x35,0x4F,0xEB,0x22,0x03,0x99,0x9B,0x9B,0xA3,0x25
	,0xF5,0x9C,0x46,0x78,0x06,0x96,0x2C,0x18,0xD4,0xCF,0x2C,0x73,0xC9,0xC9,0x13,0xFC,0x43,0x36,0x91,0x4B
	,0xAD,0x20,0x57,0x92,0x61,0x50,0x5C,0xDB,0x2B,0xFC,0x7D,0xF9,0x08,0x94,0xA7,0x1F,0x47,0xC7,0xA5,0x8B
	,0xBD,0x16,0xE8,0x0D,0x9A,0x4C,0x8B,0xCD,0x30,0x31,0xE9,0xE3,0x12,0x24,0xBE,0xB2,0x1B,0xDF,0x6E,0xB9
	,0x87,0x77,0xDE,0xED,0xC2,0x3B,0xEF,0x74,0x0B,0xF7,0x1D,0x3B,0x7E,0xC0,0xAC,0x99,0x66,0x98,0x33,0xF7
	,0x11,0xBD,0xE3,0x09,0x0A,0xBC,0xF7,0x5E,0x17,0xD2,0x2F,0x3C,0x56,0xCD,0xEB,0x7D,0xCD,0x9F,0x3B,0x0F
	,0xB7,0x32,0xD2,0x74,0xEC,0xDB,0x03,0x05,0xE1,0x66,0xF6,0x39,0xBC,0xAA,0x2A,0xC6,0xE7,0x2B,0x59,0xCD
	,0xA8,0xC4,0xA0,0x9D,0xF9,0x85,0x98,0xE6,0x30,0x11,0x8F,0x24,0x85,0xC8,0x4C,0x0C,0xD6,0x39,0x51,0x0D
	,0xB6,0xC0,0x5C,0x63,0x63,0x3C,0xE8,0xE8,0xA0,0x2D,0xD5,0x24,0xE8,0xCE,0xF3,0x68,0xA3,0x6C,0x65,0x68
	,0x78,0x0F,0xBF,0xFB,0xDD,0x03,0x4C,0x98,0xF0,0x00,0x8D,0x4D,0x2F,0x30,0x6E,0xFC,0x78,0x94,0x96,0x75
	,0xC0,0x68,0xD2,0x03,0x7C,0xFC,0x3F,0x4F,0xB1,0xC1,0xF5,0x3E,0xBA,0x65,0x72,0xD5,0xBC,0x9E,0xAB,0xAB
	,0xBB,0x0B,0x9F,0x91,0x6B,0xB2,0x1A,0xA1,0x6B,0x5F,0x35,0x58,0x9A,0xAD,0x4E,0x4F,0xA2,0xFE,0x2A,0x14
	,0x01,0x87,0x36,0x62,0xC9,0xF7,0x5E,0x18,0xB4,0x22,0x26,0x08,0xEB,0x77,0x2F,0x85,0x24,0x2D,0x01,0x19
	,0x09,0x41,0xE4,0x6F,0x3D,0x26,0xD3,0x85,0x78,0x6F,0x6F,0xB8,0x6D,0x76,0x23,0xAD,0xF3,0xAA,0xED,0xC5
	,0x4B,0xA9,0x54,0x80,0xA3,0x38,0xE8,0xEC,0xE2,0xE8,0xCE,0xA1,0xA5,0xB9,0x11,0x1F,0x0C,0xFE,0x33,0x02
	,0xFC,0xFC,0xC0,0x29,0x64,0xE0,0x05,0xC8,0x85,0x71,0x7D,0xAF,0xE3,0x31,0xC7,0xB1,0x6D,0xCD,0x6A,0x9D
	,0xFB,0x69,0x83,0xC5,0xC2,0x0B,0x69,0x31,0x3C,0xBC,0x6C,0x90,0x71,0x22,0x04,0x66,0x07,0x36,0x60,0x90
	,0x75,0xE4,0x7E,0xD8,0x6D,0x35,0x17,0x4A,0x7D,0xD9,0xF9,0x48,0x9D,0x13,0xB5,0xD1,0x4D,0x31,0xF1,0xCD
	,0xF4,0xE9,0x08,0xFF,0xFE,0x08,0x29,0x9E,0x27,0xFD,0xF7,0x76,0x27,0x05,0x4B,0x8D,0x37,0xAF,0x63,0x06
	,0xD5,0x89,0xD2,0xA4,0x44,0x7C,0x31,0x66,0x0C,0x0A,0x0B,0x0A,0xFA,0xB8,0x9D,0x78,0xB1,0xB9,0x55,0xD5
	,0xD5,0x98,0x41,0x71,0xF5,0xF4,0x72,0x4F,0x41,0xFB,0x69,0x48,0x21,0xAF,0x96,0x60,0x5F,0xA0,0x13,0xCE
	,0xC4,0xF8,0x61,0x6E,0x00,0x11,0x70,0xA7,0xDC,0xAA,0x6F,0x37,0x4C,0x48,0x95,0xCF,0x25,0xC5,0xFD,0x26
	,0x29,0xA8,0x4F,0xD1,0x7E,0x66,0xA9,0xF4,0x79,0x71,0x29,0x6C,0xE7,0x7F,0x05,0xAB,0xA5,0x4B,0x50,0x55
	,0x55,0x2D,0x10,0x61,0xD7,0xE3,0xC7,0x8F,0x71,0xC0,0xD7,0x17,0x16,0xB3,0x67,0xA2,0xF5,0x42,0x9A,0x10
	,0x74,0x77,0xB2,0x73,0x60,0x36,0xC9,0x08,0xDE,0xBB,0xBD,0xD1,0x21,0xB8,0x9E,0x78,0xDD,0xB9,0x73,0x17
	,0xDE,0x64,0xCD,0xD9,0xC6,0x53,0xD0,0x96,0xCD,0x32,0x5F,0x7F,0xCB,0x8B,0xC9,0x43,0xFB,0x99,0x9A,0x46
	,0xBA,0xDF,0xCA,0xCF,0xC0,0x6A,0x4F,0x73,0x38,0x9D,0x8C,0xC5,0xA0,0x78,0xF2,0xDD,0x91,0x0E,0x53,0x70
	,0x3A,0x29,0x50,0x68,0x75,0x19,0x98,0x1B,0xC9,0xAA,0xCA,0x91,0x14,0xB3,0x1F,0xC7,0x8F,0xEE,0x12,0xF2
	,0x32,0x0B,0x62,0xED,0xC5,0x58,0x3C,0x14,0x45,0x47,0xC1,0xD1,0xC2,0x02,0x0B,0xA8,0x85,0x58,0xB1,0x78
	,0x31,0x36,0x59,0x5B,0x23,0xFF,0xD8,0x31,0x61,0x13,0xCD,0xA6,0x64,0x76,0x99,0xA4,0x1C,0xA7,0xFC,0xFD
	,0x61,0xBF,0xD8,0x02,0xB6,0xDF,0x2C,0x84,0xE5,0xFC,0x79,0xB0,0x9C,0x35,0x1B,0xF1,0x3E,0x7B,0xD1,0x45
	,0x7F,0xD3,0x5E,0x57,0x1B,0x05,0x27,0x23,0x10,0x1C,0xBA,0x19,0xCF,0xCB,0x4A,0x54,0xD9,0x90,0x52,0x39
	,0xBD,0x4F,0x4D,0x3A,0x88,0x21,0x14,0xC4,0x51,0xAD,0x4F,0x31,0x28,0x91,0xCA,0x73,0xE8,0xD5,0x56,0x18
	,0xB8,0x4C,0xC3,0x3F,0xFC,0x1D,0x91,0x49,0x65,0xFC,0xFB,0x23,0x9E,0x98,0xE6,0x68,0x8C,0xFF,0x5A,0x3E
	,0x94,0x88,0xF9,0xD1,0x24,0x4A,0xA7,0x7D,0x08,0x88,0x1A,0x13,0xB5,0xC6,0x84,0x14,0x63,0x47,0xF5,0x4C
	,0x00,0xA5,0x61,0xED,0xB1,0xEC,0x9D,0xD8,0xFC,0x51,0x55,0x16,0xD6,0x52,0xAF,0xA7,0x3D,0xAE,0x37,0x9E
	,0x50,0x27,0xFB,0x1F,0x5F,0xFD,0x09,0x43,0xA8,0xA0,0xB9,0x78,0x2D,0xA1,0x36,0x65,0x2B,0xDC,0xF6,0xD8
	,0xD2,0xE1,0xC9,0x18,0xC1,0xD2,0xEB,0x54,0x07,0x28,0x8D,0xAA,0x2B,0x5A,0x0C,0x59,0x62,0x4B,0x46,0x2A
	,0x96,0x1F,0xDB,0x07,0x07,0xAA,0x0B,0x61,0x74,0xAA,0x3A,0x58,0x53,0x8F,0x2F,0x1D,0x26,0x51,0xBB,0x5B
	,0xDE,0x2B,0x1F,0xFF,0xD6,0x50,0x9F,0x21,0x7C,0x03,0x5C,0xF0,0xA9,0xBB,0x0D,0x9C,0xCA,0xDA,0xE0,0x7C
	,0x26,0x19,0x6B,0xE2,0x0E,0x63,0xC7,0xC5,0x3C,0xC4,0xB7,0x77,0x69,0x0E,0x3C,0x3D,0xAD,0x84,0x0A,0x62
	,0xC3,0x44,0x5D,0x20,0x1D,0x17,0x23,0xDA,0x65,0x30,0xD8,0xEF,0x89,0xD8,0x88,0x3D,0x42,0x0A,0xD3,0x3E
	,0x5D,0xFD,0x9A,0x60,0x71,0x25,0xFC,0x66,0x67,0x85,0xD2,0x32,0xF0,0x97,0x8A,0x70,0x23,0x35,0x19,0x7F
	,0x5F,0xAA,0x87,0x45,0x85,0xED,0x70,0xA9,0x65,0x1F,0x02,0x58,0x5B,0x2D,0x9E,0x06,0x45,0x88,0x4D,0x9E
	,0x86,0x40,0x5F,0xB0,0x2A,0xB7,0xF5,0x9A,0x1C,0x16,0x65,0x4F,0x31,0x72,0xFD,0x4C,0xB4,0x15,0x66,0xF4
	,0xDB,0xF8,0x57,0x41,0x59,0x39,0xF8,0xE2,0xCB,0xE0,0x0B,0x29,0x53,0xE5,0xE6,0x41,0x79,0x31,0x17,0x9D
	,0x59,0x17,0x31,0x6B,0xAD,0x31,0x66,0x25,0xA7,0x61,0xA9,0x84,0xC3,0x2A,0x29,0x6B,0x36,0x99,0xC6,0xFB
	,0xCB,0xF9,0x93,0x04,0x22,0xDB,0x39,0xD8,0x56,0xCA,0x61,0x45,0x0B,0x98,0x67,0xD6,0x60,0xFA,0x86,0x59
	,0x78,0x5A,0x56,0x2C,0x04,0xB9,0x4E,0x41,0x06,0x00,0x5E,0x52,0x41,0xC2,0x97,0x80,0x2F,0x2A,0x06,0x9F
	,0x47,0x3D,0x0F,0x09,0x2F,0xBF,0x98,0x83,0x4D,0xDB,0x2D,0x61,0x18,0x12,0x4C,0x7B,0xCB,0x89,0x80,0x02
	,0xCB,0x48,0x06,0xE6,0x0D,0xBA,0x0E,0xFA,0xFD,0x08,0xB0,0x96,0x9A,0xB9,0xCF,0xAE,0x9B,0xDD,0xB0,0xAC
	,0xE4,0x05,0x0D,0x30,0x12,0x66,0xA7,0x73,0x31,0x67,0xD3,0x6C,0xDC,0x2F,0xC9,0x23,0x77,0x62,0x01,0xA8
	,0x4D,0x44,0x3C,0x9D,0xB1,0x96,0xBB,0x27,0x58,0xD9,0x7B,0x4A,0x7B,0xE4,0x16,0xBC,0x90,0x41,0x5E,0x47
	,0x9C,0xB2,0x1F,0x1D,0xEC,0x39,0x22,0xE0,0xE9,0xB5,0x02,0x13,0x0E,0xEC,0x24,0xA1,0x65,0x82,0xF0,0x0C
	,0x56,0x15,0x1C,0x42,0xE8,0x88,0xF9,0xDA,0xF3,0x80,0x1A,0x71,0x34,0x28,0x96,0x3A,0xD4,0xB5,0x55,0x5D
	,0x02,0x01,0x2B,0x09,0x23,0x41,0x8B,0x90,0x36,0xCC,0xCE,0x16,0x62,0xCC,0x1A,0x43,0xE4,0x9C,0x0C,0xD3
	,0xB2,0x04,0x3B,0xD0,0x57,0xD0,0x59,0x38,0x1A,0x9F,0xAF,0x18,0x03,0x49,0x7A,0xAC,0x90,0x46,0x79,0x3A
	,0x2F,0xCB,0xAB,0x2B,0x61,0xEF,0x6E,0x8E,0xF6,0x4B,0xE4,0x7E,0xEC,0xFC,0xDC,0x4B,0x68,0x31,0xA6,0x18
	,0xF8,0x92,0x52,0xBC,0xA4,0x5A,0xE0,0xE4,0xBE,0x18,0x13,0x83,0xFC,0x68,0xAF,0x6E,0x8D,0xF0,0x6A,0xF8
	,0x34,0x30,0xF9,0xDE,0x90,0x40,0x60,0x33,0xB1,0xAF,0xE4,0x68,0xA2,0x28,0xBC,0x36,0x16,0xE6,0x36,0xE1
	,0xBF,0xD7,0xCD,0x22,0x4D,0xD9,0xE1,0x59,0xC1,0x45,0x28,0x24,0xAC,0x7F,0xAF,0x40,0x48,0xA8,0x3B,0xBC
	,0x03,0x1C,0xF1,0xA5,0xA3,0x01,0xC6,0xDB,0x0C,0xC3,0x54,0xEA,0xDD,0xC7,0xDB,0x8D,0xC0,0xFB,0x0B,0xFE
	,0x84,0x5D,0x7B,0x56,0xF5,0xB6,0x00,0x0B,0x5A,0x16,0xB0,0x74,0x2C,0xE5,0x0A,0x2F,0x41,0x72,0xE2,0x18
	,0x4C,0x1C,0x4D,0x30,0x2D,0x3E,0x45,0xE3,0x36,0xBD,0x40,0xB2,0x6C,0xBD,0xC6,0xBE,0x50,0xF4,0x96,0x95
	,0x41,0x87,0x0B,0x29,0xE0,0x56,0xDF,0xD5,0x7F,0x11,0x15,0x98,0x3B,0x59,0x4A,0xA8,0x71,0x3B,0x14,0x8C
	,0x11,0xD6,0x63,0xB1,0x7F,0xFF,0x5A,0xDC,0xC9,0x4C,0x01,0x57,0x56,0x2A,0x9C,0x21,0x38,0x12,0xF4,0x65
	,0x65,0x19,0xEE,0x17,0xE7,0xE2,0x71,0x59,0x01,0xBA,0x6A,0x2A,0xC0,0x51,0xF9,0x67,0x99,0x46,0xF0,0xF9
	,0x12,0xF2,0xF9,0xC2,0x42,0xF2,0xF7,0x3C,0xDC,0x48,0x89,0x81,0xC3,0x77,0x5F,0x63,0x88,0x8B,0x19,0x16
	,0x5E,0x6C,0xC4,0x12,0x61,0xFD,0xFE,0x7B,0x2E,0x23,0xB8,0xD6,0xA9,0xBF,0x84,0xF7,0x96,0xB7,0x1F,0x81
	,0x68,0xF2,0x7F,0xDB,0xCA,0x6E,0x5A,0xA8,0xBF,0xF6,0xD5,0x10,0x36,0xA1,0x00,0x5F,0x54,0x70,0x17,0xC3
	,0xB7,0x58,0xE2,0xAF,0x5F,0x0F,0x86,0x33,0x1D,0xFB,0x1A,0xCE,0x24,0x40,0x41,0x01,0xA9,0x20,0xCD,0x0A
	,0x55,0x93,0x6A,0x07,0x5F,0x41,0x87,0x23,0x3A,0x07,0x73,0x97,0x0A,0xF1,0x22,0x27,0x0B,0x75,0xC9,0x91
	,0x08,0xF3,0xDF,0x80,0xA5,0xCE,0x26,0xB0,0xFE,0xD6,0x0C,0xF3,0x37,0x18,0xC3,0x28,0x2C,0x84,0xD6,0x65
	,0x16,0xEF,0xBF,0x97,0x80,0x4A,0x05,0xEC,0xAB,0x89,0xC0,0xDD,0xDE,0xB2,0x32,0x68,0x11,0x60,0xEC,0x38
	,0xEC,0x6F,0x92,0x69,0x09,0x2F,0x2E,0xAA,0x36,0x2B,0x13,0x5C,0xD4,0x90,0x18,0xD8,0x8B,0x2E,0xDD,0xC2
	,0xB0,0xD5,0x46,0xB8,0x7E,0x3E,0x01,0x09,0x21,0xDB,0x31,0x91,0x2A,0xE6,0x62,0x27,0x63,0x44,0x05,0xBB
	,0x21,0x3B,0xE2,0x00,0xCE,0x1E,0xF1,0x86,0xAF,0xCF,0x6A,0xD8,0xBA,0xCE,0xC0,0xBC,0xB5,0x86,0xB0,0xD9
	,0x6C,0x86,0x3D,0xFE,0xEB,0x90,0x4B,0x31,0xF4,0xBC,0xFC,0xB2,0x40,0x30,0x2A,0x72,0x17,0x46,0xEE,0x70
	,0x80,0x55,0x25,0xDB,0x97,0xAD,0xCD,0xF6,0xD2,0x76,0x5F,0x76,0xE7,0x60,0x43,0x41,0x7D,0xFC,0x75,0x16
	,0x88,0xA7,0x3C,0x1B,0x7B,0x57,0x09,0xB7,0x3A,0xB9,0x90,0xB6,0x34,0xA6,0x24,0x4D,0x2F,0x2E,0xBC,0x0B
	,0xE3,0x63,0xD1,0x18,0xB5,0xD3,0x15,0x1F,0x3B,0xCF,0xC7,0x90,0x75,0xA6,0x18,0xE1,0x64,0x8A,0xD1,0x14
	,0xD0,0xB9,0xD9,0xF1,0x50,0xD6,0x54,0x41,0x51,0x5C,0x84,0xAE,0xDC,0x6C,0x54,0x51,0x15,0xCF,0x89,0x3C
	,0x80,0xAC,0x63,0xBE,0xD4,0x2B,0x05,0xE3,0x4A,0x72,0x04,0x1E,0x66,0x9E,0x87,0x9C,0xB2,0x8C,0x76,0x00
	,0x33,0x28,0xA4,0xD5,0xA8,0x48,0x3D,0x8E,0xFF,0xB4,0x1C,0x8A,0xD1,0xDE,0x1B,0x31,0xDA,0x77,0x3B,0xBE
	,0x08,0x0B,0xC7,0xEC,0x94,0x3C,0x58,0x51,0xFD,0x59,0x42,0x7B,0x8B,0xB1,0xC8,0xDC,0x48,0x8E,0xF0,0x36
	,0x31,0x4B,0xFE,0x04,0x01,0x1E,0x31,0x94,0x7D,0x58,0xEE,0x67,0x8C,0xD9,0x64,0xD3,0xF8,0x33,0x18,0xB9
	,0xD9,0x02,0x6B,0x76,0xDB,0x20,0xE1,0x64,0x30,0x6A,0x4B,0x32,0xF1,0xF4,0xE6,0x15,0x74,0x35,0x37,0x40
	,0xD6,0xDA,0x00,0xAE,0xE1,0x3A,0x14,0x35,0x35,0x62,0x50,0x32,0x1F,0x67,0x29,0x93,0xAA,0x28,0xCB,0xE7
	,0xAC,0x20,0x29,0x2E,0x52,0x90,0x13,0x94,0x39,0x74,0x2F,0xBA,0xD4,0x8F,0x00,0x4F,0xFD,0x92,0x9C,0x5C
	,0xED,0xFE,0xE5,0x3C,0xB4,0xE6,0xA6,0xE2,0x4A,0x06,0x9D,0x49,0x92,0x42,0xB0,0x6D,0xDF,0x5A,0x0C,0xB7
	,0x1B,0x87,0x31,0xBB,0x5C,0x48,0x79,0x3F,0x08,0x4A,0x64,0x16,0xF7,0x6F,0x66,0x72,0x52,0x30,0xEB,0x22
	,0x90,0x40,0x04,0x0E,0xB5,0x70,0x82,0xF6,0x97,0x92,0x39,0xC7,0x52,0x0B,0x61,0xEE,0x3E,0x0F,0x77,0x6A
	,0x49,0xA8,0xA6,0x26,0xF0,0x37,0xAE,0x42,0x71,0xB5,0x0E,0xFC,0x95,0x5A,0xD2,0x78,0xB5,0xE8,0xDF,0xEA
	,0x16,0x40,0x5B,0x28,0x56,0x13,0xA8,0xC3,0xE4,0xA9,0xBF,0xE7,0xF3,0xA8,0xB2,0xE6,0x88,0x64,0x78,0x3A
	,0x13,0xA8,0xD3,0x66,0xDF,0x39,0x7D,0xC1,0x3A,0xCE,0x17,0xD4,0xDA,0x1F,0x3D,0xF2,0x0F,0xE8,0xAD,0x99
	,0x8A,0x65,0xA5,0xCF,0x84,0x38,0xF0,0xBA,0x29,0xCA,0xCA,0xEA,0x94,0x0E,0x02,0x1C,0xBC,0x6E,0x88,0xBE
	,0x6E,0x51,0x78,0x1B,0xC3,0x57,0x8D,0xC7,0x8F,0x4D,0x24,0x74,0xD3,0x4D,0x12,0xB4,0x5A,0xE7,0x46,0x0C
	,0x7D,0x7B,0xF6,0x5E,0x20,0x8B,0x28,0x4A,0xA8,0xCA,0x92,0xFB,0x30,0xAB,0xF0,0xF4,0x5B,0x49,0x29,0x57
	,0xE7,0xD8,0x7E,0x10,0x3B,0x56,0xCF,0xB5,0x6B,0x60,0x1A,0x74,0x86,0x3C,0x42,0x81,0x2D,0x57,0xB5,0x3F
	,0x72,0xF5,0x25,0x40,0xEE,0xB3,0xB1,0x8E,0xD2,0x27,0xF9,0x9C,0x51,0xF8,0x61,0xEC,0x39,0xBC,0x19,0x7C
	,0x0B,0x69,0xBE,0x99,0x40,0x1A,0xD7,0xBD,0xC9,0x9B,0x82,0xAC,0x52,0x5E,0x06,0x05,0x3B,0x75,0xBD,0xA6
	,0xFF,0xEF,0x0F,0x29,0x4E,0xD3,0x39,0x42,0xDF,0xD6,0x8D,0x5C,0x48,0x0E,0xA7,0x5A,0x56,0xA7,0x7A,0x7F
	,0xA1,0xD3,0x10,0x88,0x21,0x17,0x5A,0x49,0x4D,0x93,0x15,0x45,0xFB,0x10,0xD7,0x79,0xB8,0x2E,0xC9,0x07
	,0x1D,0x6C,0xC1,0xB7,0x36,0x01,0x8D,0x0D,0xE0,0x6B,0xAF,0xA8,0xB4,0xAD,0xEE,0xE3,0x7F,0x7B,0xF0,0x64
	,0xAD,0xE6,0xF4,0x74,0x7C,0x64,0x60,0x82,0xE5,0xE4,0xDA,0x2B,0xAB,0xD4,0x5F,0x10,0x75,0x10,0x08,0xBB
	,0x25,0x06,0x8A,0x65,0x69,0x07,0xF4,0xED,0x3F,0x07,0x77,0xAB,0x41,0x20,0x00,0xB2,0x82,0x92,0x48,0xF0
	,0x8C,0xCC,0x8D,0x1B,0xE0,0xAB,0xFB,0xB7,0x04,0xBF,0x19,0x28,0xC6,0xBA,0xE9,0xFE,0xF1,0xC7,0xC3,0xB1
	,0xBC,0xF8,0x39,0x91,0x90,0x51,0x9D,0x62,0xF2,0xEA,0x20,0xE0,0xD3,0x28,0xA6,0xAC,0x99,0xA7,0x72,0xB1
	,0xCE,0xC7,0x4E,0x10,0x5A,0xB0,0x40,0x3D,0x05,0x6E,0x6D,0xAD,0x00,0x25,0xB3,0xC2,0x15,0x55,0xD6,0xE9
	,0xBB,0xD9,0x6F,0x00,0x16,0xF0,0x0A,0xDA,0x6B,0xEE,0xE4,0x29,0xF8,0x2A,0xB9,0x5A,0xC8,0x8C,0xA1,0xAD
	,0xDA,0x67,0x02,0x46,0xE0,0xB6,0xF8,0x1F,0x11,0x16,0x20,0x2C,0x80,0x3F,0xF3,0xDB,0x83,0xC4,0x13,0x87
	,0x54,0xDA,0x6F,0x26,0xA1,0x49,0x60,0x5A,0x88,0x65,0x97,0x8A,0xD8,0x58,0x32,0x6B,0x8F,0x0B,0xBD,0x49
	,0x46,0x19,0x08,0xD4,0xD9,0x4D,0x9D,0x20,0x3C,0xED,0xED,0x31,0x65,0x57,0x24,0x35,0x97,0x54,0x68,0x1B
	,0xD4,0x81,0xAC,0x21,0xA0,0x44,0x1C,0x61,0x4D,0x35,0xB9,0x10,0xA5,0xCF,0x8F,0xD6,0x9B,0xA1,0xA5,0xFA
	,0x32,0xD0,0xDC,0xD2,0x8B,0x00,0xD3,0x84,0xB9,0xC9,0x14,0xEA,0x75,0x7A,0x08,0xB0,0x77,0xAF,0xCD,0x42
	,0x6F,0x00,0x36,0xFF,0x49,0x09,0xF5,0x46,0x5A,0x56,0x55,0x48,0x6B,0x50,0x18,0x15,0x49,0x7F,0x13,0x1B
	,0xC0,0xF3,0x81,0x81,0x18,0x63,0xE9,0x2A,0x58,0xC0,0x93,0x0E,0x59,0x7D,0x5C,0x88,0xC3,0xD1,0x5B,0x1C
	,0xAC,0x2B,0xC8,0x02,0xE5,0x8F,0x30,0x66,0xD5,0x04,0xC8,0x5B,0x1B,0x35,0x16,0x50,0xD4,0x93,0xEB,0x08
	,0x8B,0x4A,0x60,0xFC,0xD9,0x38,0xB4,0x66,0xA4,0x0B,0xCF,0x1C,0x59,0xC2,0x69,0x9B,0x05,0x32,0x13,0x0F
	,0x69,0x36,0x1E,0x08,0xBA,0x25,0x65,0x28,0x4F,0x89,0x82,0x9C,0x75,0xA7,0xAA,0x77,0x0F,0xA8,0xD9,0x9B
	,0x3F,0x69,0x92,0x46,0x39,0x77,0xB2,0xB2,0xF0,0xA1,0x9E,0xA1,0x90,0x89,0x5C,0xA8,0x53,0x60,0x9F,0xD8
	,0xB5,0x08,0xC8,0xB1,0xB7,0x91,0xFA,0x10,0x62,0x37,0x33,0x39,0x07,0xEB,0x7D,0x6C,0x84,0x80,0x45,0xB3
	,0x48,0x80,0xA7,0xE2,0x25,0xB8,0x0A,0x1D,0x4A,0x56,0x98,0x9B,0xE3,0x6C,0x40,0x80,0xB0,0x28,0x5B,0x5C
	,0xE8,0x30,0xB5,0x5C,0x6A,0xE0,0xE8,0x7D,0xD8,0x29,0x8E,0x8E,0xC4,0x64,0x7D,0x7D,0xCD,0x33,0x47,0x67
	,0x89,0xE1,0x9F,0x8C,0xC0,0x92,0xFC,0x7B,0x58,0x55,0x25,0x43,0xAC,0xC6,0x02,0x72,0xFC,0x2F,0xF5,0x92
	,0xAE,0x65,0x3F,0x05,0xC9,0x10,0x00,0x00,0x00,0x00,0x49,0x45,0x4E,0x44,0xAE,0x42,0x60,0x82
};
const char UPnPDeviceIcon_LGJPG[1603]={
	0x48,0x54,0x54,0x50,0x2F,0x31,0x2E,0x31,0x20,0x32,0x30,0x30,0x20,0x4F,0x4B,0x0D,0x0A,0x43,0x4F,0x4E
	,0x54,0x45,0x4E,0x54,0x2D,0x54,0x59,0x50,0x45,0x3A,0x20,0x69,0x6D,0x61,0x67,0x65,0x2F,0x6A,0x70,0x67
	,0x0D,0x0A,0x43,0x6F,0x6E,0x74,0x65,0x6E,0x74,0x2D,0x4C,0x65,0x6E,0x67,0x74,0x68,0x3A,0x20,0x31,0x35
	,0x33,0x37,0x0D,0x0A,0x0D,0x0A,0xFF,0xD8,0xFF,0xE0,0x00,0x10,0x4A,0x46,0x49,0x46,0x00,0x01,0x01,0x01
	,0x00,0x60,0x00,0x60,0x00,0x00,0xFF,0xDB,0x00,0x43,0x00,0x08,0x06,0x06,0x07,0x06,0x05,0x08,0x07,0x07
	,0x07,0x09,0x09,0x08,0x0A,0x0C,0x14,0x0D,0x0C,0x0B,0x0B,0x0C,0x19,0x12,0x13,0x0F,0x14,0x1D,0x1A,0x1F
	,0x1E,0x1D,0x1A,0x1C,0x1C,0x20,0x24,0x2E,0x27,0x20,0x22,0x2C,0x23,0x1C,0x1C,0x28,0x37,0x29,0x2C,0x30
	,0x31,0x34,0x34,0x34,0x1F,0x27,0x39,0x3D,0x38,0x32,0x3C,0x2E,0x33,0x34,0x32,0xFF,0xDB,0x00,0x43,0x01
	,0x09,0x09,0x09,0x0C,0x0B,0x0C,0x18,0x0D,0x0D,0x18,0x32,0x21,0x1C,0x21,0x32,0x32,0x32,0x32,0x32,0x32
	,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32
	,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32
	,0x32,0x32,0x32,0x32,0xFF,0xC0,0x00,0x11,0x08,0x00,0x30,0x00,0x30,0x03,0x01,0x22,0x00,0x02,0x11,0x01
	,0x03,0x11,0x01,0xFF,0xC4,0x00,0x1F,0x00,0x00,0x01,0x05,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00
	,0x00,0x00,0x00,0x00,0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0xFF,0xC4,0x00,0xB5
	,0x10,0x00,0x02,0x01,0x03,0x03,0x02,0x04,0x03,0x05,0x05,0x04,0x04,0x00,0x00,0x01,0x7D,0x01,0x02,0x03
	,0x00,0x04,0x11,0x05,0x12,0x21,0x31,0x41,0x06,0x13,0x51,0x61,0x07,0x22,0x71,0x14,0x32,0x81,0x91,0xA1
	,0x08,0x23,0x42,0xB1,0xC1,0x15,0x52,0xD1,0xF0,0x24,0x33,0x62,0x72,0x82,0x09,0x0A,0x16,0x17,0x18,0x19
	,0x1A,0x25,0x26,0x27,0x28,0x29,0x2A,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x43,0x44,0x45,0x46,0x47,0x48
	,0x49,0x4A,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6A,0x73,0x74
	,0x75,0x76,0x77,0x78,0x79,0x7A,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8A,0x92,0x93,0x94,0x95,0x96,0x97
	,0x98,0x99,0x9A,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,0xA8,0xA9,0xAA,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9
	,0xBA,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,0xDA,0xE1
	,0xE2,0xE3,0xE4,0xE5,0xE6,0xE7,0xE8,0xE9,0xEA,0xF1,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,0xF8,0xF9,0xFA,0xFF
	,0xC4,0x00,0x1F,0x01,0x00,0x03,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00
	,0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0xFF,0xC4,0x00,0xB5,0x11,0x00,0x02,0x01
	,0x02,0x04,0x04,0x03,0x04,0x07,0x05,0x04,0x04,0x00,0x01,0x02,0x77,0x00,0x01,0x02,0x03,0x11,0x04,0x05
	,0x21,0x31,0x06,0x12,0x41,0x51,0x07,0x61,0x71,0x13,0x22,0x32,0x81,0x08,0x14,0x42,0x91,0xA1,0xB1,0xC1
	,0x09,0x23,0x33,0x52,0xF0,0x15,0x62,0x72,0xD1,0x0A,0x16,0x24,0x34,0xE1,0x25,0xF1,0x17,0x18,0x19,0x1A
	,0x26,0x27,0x28,0x29,0x2A,0x35,0x36,0x37,0x38,0x39,0x3A,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x53
	,0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6A,0x73,0x74,0x75,0x76,0x77
	,0x78,0x79,0x7A,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8A,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99
	,0x9A,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,0xA8,0xA9,0xAA,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBA,0xC2
	,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,0xDA,0xE2,0xE3,0xE4
	,0xE5,0xE6,0xE7,0xE8,0xE9,0xEA,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,0xF8,0xF9,0xFA,0xFF,0xDA,0x00,0x0C,0x03
	,0x01,0x00,0x02,0x11,0x03,0x11,0x00,0x3F,0x00,0xF5,0x3A,0x6C,0x8D,0xB2,0x36,0x6C,0x13,0x81,0x9C,0x00
	,0x4E,0x7F,0x2E,0x69,0xB3,0x4F,0x0D,0xBC,0x7E,0x64,0xF2,0xC7,0x12,0x74,0xDC,0xEC,0x14,0x7E,0xB5,0x94
	,0xFA,0x98,0xBB,0xCB,0xC5,0x24,0x8B,0x66,0x64,0xF2,0xA2,0x78,0x17,0x7C,0xB7,0x52,0x73,0x95,0x8C,0x73
	,0xC0,0xC1,0xE7,0xD8,0x9C,0x80,0x33,0x5E,0x85,0x4A,0x91,0x82,0xD4,0xF1,0xA9,0x52,0x95,0x59,0x72,0xC4
	,0x96,0xC7,0x5E,0xB3,0xBB,0x8A,0xE1,0x9D,0xD6,0x06,0xB7,0x05,0xA6,0x57,0x61,0x85,0x03,0xF8,0xB3,0xE9
	,0xF9,0x11,0xDC,0x0A,0xB1,0x63,0xA9,0x41,0xA8,0x02,0xD6,0xF9,0x31,0xF5,0x0C,0x48,0x19,0xFC,0x33,0x91
	,0xF8,0x81,0x5C,0xF4,0xFA,0x68,0xB2,0xF1,0x05,0xB6,0xA3,0xA8,0x68,0x57,0x16,0x36,0xC5,0x4C,0x42,0x7B
	,0x9B,0x90,0xCA,0xEC,0x48,0x2B,0xE6,0xED,0x2E,0x3E,0xF0,0x00,0x16,0x23,0x9C,0x75,0x20,0x62,0xCC,0x72
	,0xAE,0xA2,0xBE,0x73,0xF8,0x4E,0x7B,0xB8,0x49,0xCC,0x57,0x96,0x31,0x29,0x5C,0x7A,0xAB,0x31,0x47,0x3E
	,0xCC,0xA3,0x9E,0xD9,0xAE,0x45,0x8B,0x9D,0xF5,0x47,0x6A,0xCB,0xEE,0xB5,0x9A,0x3A,0x4A,0x2B,0x04,0x6A
	,0x07,0x4D,0xB5,0x4B,0xB1,0x72,0xF7,0x5A,0x4B,0xB6,0xDF,0x36,0x50,0x44,0xB6,0xCD,0x9C,0x61,0xF3,0xC9
	,0x00,0xF0,0x73,0xF3,0x29,0xEB,0x9E,0x48,0xD8,0xB7,0xBB,0xB6,0xBB,0x52,0xD6,0xD7,0x11,0x4C,0xAA,0x70
	,0x4C,0x6E,0x18,0x0F,0xCA,0xBA,0xA9,0xD5,0x8D,0x45,0x74,0x71,0x55,0xA3,0x3A,0x52,0xE5,0x91,0x99,0xA0
	,0xC5,0x16,0xA2,0xD7,0x1A,0x95,0xCC,0x5E,0x6C,0xD2,0x48,0xEA,0x8D,0x20,0xC8,0x8D,0x03,0x32,0x84,0x50
	,0x7A,0x70,0xA3,0x3E,0xA4,0x9F,0x6A,0xD0,0x0F,0x69,0xA3,0xEB,0x9A,0x5D,0xDC,0xEA,0x90,0x69,0xF0,0xC1
	,0x2D,0xB0,0x7C,0x61,0x20,0x66,0x31,0xEC,0x27,0xB2,0x8C,0x23,0x2E,0x7B,0x6E,0x03,0xBD,0x55,0xBA,0xD2
	,0xED,0x84,0x57,0x0D,0xE6,0x5C,0xC5,0x14,0x99,0x69,0x22,0x86,0x66,0x45,0x66,0x3D,0x4F,0x1D,0x09,0xEF
	,0x8C,0x67,0xBD,0x3B,0x44,0xD4,0x12,0x6B,0x75,0xD3,0xAE,0x99,0x7E,0xD5,0x12,0x6D,0xDA,0x40,0x02,0x54
	,0x1C,0x6E,0x03,0xBF,0x1D,0x47,0x63,0xED,0x8C,0xF0,0xD4,0xA1,0x52,0x1E,0xFB,0xD5,0x1E,0xC6,0x13,0x15
	,0x46,0xA2,0x58,0x75,0xA3,0xB1,0x47,0xE2,0x25,0x9F,0x88,0x2E,0xAE,0x26,0x58,0x75,0x8B,0x46,0xD1,0x6F
	,0x2C,0x9E,0x05,0xD2,0xF6,0x03,0x2D,0xC4,0xA5,0x4E,0x02,0xF1,0xCF,0x25,0x4E,0xED,0xC0,0x28,0x19,0x3C
	,0x03,0x5A,0x9E,0x03,0x3A,0xC2,0x5B,0xEA,0x37,0xFA,0xBE,0xBF,0x6D,0x7B,0xA7,0xBB,0x03,0x6F,0x18,0x41
	,0x19,0xB5,0x03,0x39,0x57,0x18,0x1B,0x08,0x18,0x05,0x79,0xC6,0x2A,0x6B,0x4B,0x5D,0x32,0xCE,0xE2,0x48
	,0xAC,0xED,0xED,0x20,0x9B,0x68,0x69,0x12,0x14,0x55,0x6C,0x1C,0xE0,0x90,0x39,0xC7,0x06,0x87,0xD3,0xF4
	,0xCB,0xF7,0x4B,0xB7,0xB3,0xB4,0xB8,0x62,0x01,0x59,0x9A,0x25,0x72,0x47,0x62,0x0E,0x2B,0x47,0x8D,0x93
	,0x87,0x27,0x2A,0xB5,0x92,0xD9,0x5F,0x47,0x7D,0xFB,0xF7,0x7D,0x4E,0xAF,0xAA,0x2E,0xE4,0x36,0x62,0x0D
	,0x46,0xE3,0x58,0xB9,0x58,0xD5,0xEC,0x2F,0xAE,0x77,0x44,0x18,0x7C,0xB2,0xA7,0x94,0x88,0xCD,0x8F,0x46
	,0x2A,0xC4,0x7A,0x8E,0x7B,0xD5,0x3D,0x7E,0x28,0xEC,0x23,0x87,0x51,0xB6,0x81,0x63,0xB8,0x8D,0xC2,0x97
	,0x41,0x8D,0xCB,0xFD,0xC6,0xF5,0x04,0x64,0x0C,0xF4,0x24,0x11,0x53,0x6B,0x7A,0xAB,0x5B,0x44,0xD6,0x96
	,0xC1,0x5A,0xF2,0x65,0xC2,0x29,0xE4,0x20,0x3C,0x6F,0x61,0xE8,0x3D,0x3B,0x9E,0x2A,0xB6,0x97,0xA4,0x5B
	,0xAD,0x9D,0xB7,0x99,0x35,0xD5,0xC2,0xC2,0x41,0x44,0x9E,0x76,0x75,0x0C,0xBD,0x0E,0x09,0xE4,0xF7,0x19
	,0xE9,0xDB,0x15,0x95,0x2A,0x15,0x2A,0x7B,0xF1,0xD1,0x1C,0xD8,0xCC,0x55,0x1A,0x4B,0xEA,0xEF,0x57,0x62
	,0x6D,0x6E,0xE9,0x6D,0xED,0xD0,0x36,0xE3,0xB9,0x80,0xDA,0xA0,0x92,0xC4,0x9C,0x00,0x00,0xEA,0x49,0x22
	,0xB3,0xA5,0xD1,0x75,0x3B,0xC8,0xC6,0x6C,0xED,0xE3,0xC1,0xDC,0xBE,0x75,0xD1,0x49,0x10,0xFA,0x82,0x8A
	,0xD8,0x3E,0xE0,0xD6,0x86,0xD1,0x79,0xE2,0xDB,0x28,0x4F,0x29,0x6E,0xAF,0x70,0xDE,0x9C,0x0D,0xA3,0xF5
	,0x7C,0xFF,0x00,0xC0,0x6B,0x51,0xF5,0x18,0xBF,0xB4,0x6E,0x2C,0xCA,0x30,0x92,0x18,0x16,0x72,0x55,0x49
	,0xDC,0xAC,0x58,0x70,0x07,0x24,0x8D,0x9F,0xA8,0xAE,0xBC,0x4E,0x2A,0x70,0xFD,0xDC,0x76,0xEA,0x79,0xF8
	,0x0C,0xBE,0x9E,0x22,0xF5,0xAA,0x5F,0x7D,0x3A,0x6C,0x61,0x8F,0x0F,0x5C,0x85,0x4F,0xB6,0x35,0x8E,0xA8
	,0x42,0x8D,0xCF,0x77,0x68,0x0B,0x86,0xC7,0x38,0x61,0xD4,0x7B,0x11,0x9F,0x73,0x57,0xAD,0xB4,0xFB,0xC1
	,0x6E,0xB0,0x0B,0xB8,0x6D,0x60,0x4E,0x16,0x3B,0x2B,0x61,0x1E,0xD1,0xE9,0xF3,0x16,0x1F,0xA0,0xAD,0x60
	,0x77,0x28,0x23,0x3C,0x8C,0xF2,0x31,0x4B,0x5E,0x5D,0x8F,0xA3,0x54,0xE2,0xB6,0x38,0xE8,0xF4,0x8D,0x53
	,0x4E,0x8A,0x42,0xF6,0xBF,0x6B,0x25,0xD8,0xB4,0xB1,0x4D,0xBA,0x49,0x06,0x4E,0x09,0x0D,0x8C,0x9C,0x63
	,0x8C,0x9F,0x6A,0xD7,0xD1,0x2E,0x23,0xB9,0xB0,0x12,0x44,0xDB,0x91,0x8E,0xE5,0x3E,0xC6,0xB4,0xAE,0x6F
	,0x60,0xB4,0x89,0x24,0x99,0x8A,0xAB,0xCA,0x90,0xAE,0x47,0x56,0x66,0x0A,0x07,0xE6,0x45,0x60,0x78,0x74
	,0x7D,0x9A,0xF3,0x56,0xB0,0x3F,0xF2,0xC2,0xF2,0x4C,0x0F,0x40,0xE7,0xCC,0x5F,0xD2,0x40,0x3F,0x0A,0xF5
	,0x30,0xB8,0x89,0xCE,0x2E,0x9C,0xB6,0x4B,0x43,0xE7,0x73,0x0C,0x05,0x3A,0x0D,0x56,0xA7,0x7D,0x5E,0xBF
	,0x33,0xFF,0xD9
};
const char UPnPDeviceIcon_SMPNG[4058]={
	0x48,0x54,0x54,0x50,0x2F,0x31,0x2E,0x31,0x20,0x32,0x30,0x30,0x20,0x4F,0x4B,0x0D,0x0A,0x43,0x4F,0x4E
	,0x54,0x45,0x4E,0x54,0x2D,0x54,0x59,0x50,0x45,0x3A,0x20,0x69,0x6D,0x61,0x67,0x65,0x2F,0x70,0x6E,0x67
	,0x0D,0x0A,0x43,0x6F,0x6E,0x74,0x65,0x6E,0x74,0x2D,0x4C,0x65,0x6E,0x67,0x74,0x68,0x3A,0x20,0x33,0x39
	,0x39,0x32,0x0D,0x0A,0x0D,0x0A,0x89,0x50,0x4E,0x47,0x0D,0x0A,0x1A,0x0A,0x00,0x00,0x00,0x0D,0x49,0x48
	,0x44,0x52,0x00,0x00,0x00,0x30,0x00,0x00,0x00,0x30,0x08,0x06,0x00,0x00,0x00,0x57,0x02,0xF9,0x87,0x00
	,0x00,0x00,0x01,0x73,0x52,0x47,0x42,0x00,0xAE,0xCE,0x1C,0xE9,0x00,0x00,0x00,0x04,0x67,0x41,0x4D,0x41
	,0x00,0x00,0xB1,0x8F,0x0B,0xFC,0x61,0x05,0x00,0x00,0x00,0x09,0x70,0x48,0x59,0x73,0x00,0x00,0x0E,0xC3
	,0x00,0x00,0x0E,0xC3,0x01,0xC7,0x6F,0xA8,0x64,0x00,0x00,0x0F,0x2D,0x49,0x44,0x41,0x54,0x68,0x43,0xED
	,0x59,0x07,0x50,0x54,0xE7,0xBE,0x37,0xC9,0xDC,0xB9,0x6F,0xDE,0xCC,0x9D,0x7B,0xDF,0xF3,0x4E,0xF2,0x72
	,0x4B,0x5E,0x9E,0x31,0x36,0x54,0xA2,0x62,0x08,0xA2,0x44,0xB1,0x61,0x49,0xD0,0x28,0x28,0x4A,0xB1,0x21
	,0x88,0x80,0x8A,0x11,0xA2,0xE2,0x55,0x44,0x51,0xA4,0x08,0x88,0x10,0x95,0x22,0x1D,0x0B,0x16,0x8A,0x14
	,0x69,0x22,0x75,0x59,0x8A,0x60,0xA3,0x0A,0xEA,0x8D,0x8A,0xBD,0x04,0xD8,0x3D,0x7B,0xF6,0xF7,0xFE,0xDF
	,0x39,0xBB,0xCB,0x02,0x1B,0x63,0x30,0x99,0x3B,0x6F,0xE6,0x9D,0x99,0xDF,0x9C,0x3D,0x87,0xAF,0xFC,0x7F
	,0xFF,0xFE,0x1D,0x06,0x25,0xDC,0x91,0xE3,0xFF,0x32,0xFE,0x9F,0xC0,0xBF,0x1A,0x6F,0x47,0xE0,0xAE,0x0C
	,0xF1,0x77,0x38,0xFA,0xCD,0xA0,0xE3,0xEF,0xFD,0xA0,0x1A,0x7B,0x5B,0x41,0x77,0x99,0xD6,0xFB,0x81,0xE3
	,0x2D,0x08,0x70,0xF8,0x2E,0xFB,0x02,0x16,0x87,0x7B,0x21,0xA4,0xAE,0x59,0x45,0x44,0xD7,0x38,0x2D,0xDC
	,0xED,0xC6,0xF1,0xD6,0xE7,0x88,0x6D,0xEB,0xA4,0x67,0x46,0x42,0xC7,0x98,0x5F,0x88,0x01,0x13,0x88,0x57
	,0xC1,0xEB,0x52,0x3E,0x86,0x3B,0x4D,0xC3,0xD2,0xEF,0x77,0x23,0xAE,0xFD,0x15,0xE2,0x34,0x63,0x98,0x86
	,0x39,0xC4,0xDF,0xEE,0xC4,0xB6,0xAC,0x4C,0x2C,0x08,0xDA,0x8E,0x35,0xF1,0xE1,0xF0,0x2F,0xBF,0x46,0xEF
	,0x7E,0x1D,0xED,0x33,0x0C,0x98,0xC0,0x91,0xEB,0xF7,0xB0,0x2A,0xF6,0x10,0x6C,0xA3,0x03,0x88,0x44,0x21
	,0xBE,0x3E,0xF8,0x2D,0xA6,0xEE,0x5E,0x29,0x08,0xAC,0x76,0x29,0x7F,0x69,0x2D,0x26,0xB8,0x2F,0x84,0x5D
	,0x74,0x20,0x22,0x9B,0x1F,0xA9,0x48,0x33,0xB7,0x13,0xC9,0xF7,0x5D,0x73,0x20,0x78,0xAB,0x18,0x88,0x6B
	,0xFF,0x11,0xDB,0x73,0xB2,0x30,0xCD,0x6B,0x35,0xA6,0xEF,0x75,0xC4,0x8C,0xBD,0xAB,0x61,0x17,0x13,0x48
	,0xC2,0x71,0x08,0xAC,0xAE,0xC3,0x07,0x56,0x43,0x31,0xC7,0x6F,0x3D,0xAC,0xA3,0x0E,0xC0,0x3E,0xF1,0x08
	,0x36,0xA5,0x9F,0x81,0x9F,0xB4,0x1E,0x71,0x02,0xC9,0x7F,0x59,0x0C,0x30,0xED,0xB2,0xCD,0x09,0xB7,0x59
	,0x40,0xFE,0x48,0x2E,0x92,0x0E,0x3D,0x47,0x43,0x8C,0xDB,0x32,0x0B,0x1F,0xDA,0xE8,0x91,0x75,0xDA,0x60
	,0xEA,0x36,0x17,0x29,0xC9,0x07,0x91,0x99,0x14,0x82,0x93,0x71,0x07,0x10,0x1E,0xFE,0x1D,0x3C,0xF6,0xDA
	,0x61,0x96,0x8B,0x11,0x86,0x5A,0x8F,0x82,0xD9,0x3E,0x47,0x78,0x5F,0x2E,0xA1,0xF9,0xDD,0xB4,0x16,0x8B
	,0x07,0xED,0x98,0x60,0x7B,0xBC,0x59,0x8C,0x0C,0x80,0x80,0xA8,0xB9,0xD8,0xB6,0x17,0x70,0x3E,0x15,0x07
	,0xD3,0x6F,0xE7,0x62,0xA7,0xAF,0x03,0x9A,0x0A,0x32,0x60,0xE6,0xFC,0x05,0x7E,0x3F,0xFF,0xDF,0x31,0x7A
	,0x9D,0x21,0x0A,0xCE,0x1E,0x85,0x52,0x5A,0x09,0x10,0xD8,0x9D,0x81,0x97,0x4A,0xA1,0x20,0xBC,0x94,0x94
	,0x20,0x83,0x88,0x59,0x6D,0x31,0xC3,0xF8,0x0D,0x33,0xB1,0x33,0x3F,0xBF,0xF7,0x1E,0xB7,0x79,0x82,0x92
	,0x7E,0xFF,0x7C,0x62,0xF8,0x05,0x04,0x38,0x24,0xB2,0xF4,0x77,0xBB,0x0B,0xAE,0xA7,0xE2,0xB1,0x68,0x9B
	,0x25,0x52,0xE2,0x7D,0xD1,0x55,0x59,0x0A,0x45,0x95,0x14,0x5C,0x55,0x25,0xAE,0xE6,0x9C,0xC5,0x9F,0xE7
	,0xFD,0x01,0x11,0x91,0xBB,0xA0,0xAC,0x14,0x85,0x56,0x13,0xE0,0xD9,0x9D,0xC6,0xF0,0x52,0x89,0xF0,0x9B
	,0x97,0x56,0x09,0x63,0xA4,0x99,0x49,0x30,0x5D,0x3B,0x11,0xD3,0xBC,0x57,0xE1,0x58,0xE3,0x3D,0x72,0x3F
	,0x85,0x80,0xE8,0x36,0x19,0x22,0xDB,0xD8,0x9E,0xBC,0xE0,0x92,0xBA,0x65,0xFA,0x85,0x16,0x38,0xDA,0xF0
	,0x03,0x66,0x78,0x2E,0x43,0x5C,0x94,0x37,0xE4,0x4C,0xA0,0x4A,0x26,0x84,0x14,0x8F,0x8B,0xF3,0x90,0x91
	,0x1C,0x8C,0xCB,0xE7,0xA2,0x50,0x96,0x9E,0x00,0x79,0x95,0x44,0x20,0xC5,0x93,0xC0,0x2F,0x24,0x97,0x51
	,0x9E,0x16,0x8B,0xA2,0x33,0x51,0x68,0xC9,0x4B,0x07,0x5F,0x29,0xD1,0x58,0x44,0x0D,0x99,0xB4,0x02,0xFE
	,0x21,0x1B,0xF1,0xC9,0x8A,0xB1,0xF0,0x2D,0xAF,0xA0,0xBD,0x14,0x08,0x6F,0x95,0x23,0xA0,0xA5,0x0B,0x49
	,0x64,0x89,0xB7,0x24,0xC0,0x26,0x73,0x24,0xFC,0x3D,0xE8,0xAF,0x33,0x21,0x21,0xA3,0x05,0xCD,0x29,0x48
	,0x48,0xE6,0x12,0x0C,0xFF,0x2C,0xCA,0x84,0x6F,0xE0,0x46,0xD8,0x7B,0x2E,0xC4,0x88,0x25,0x1F,0x61,0xB3
	,0xB7,0x0D,0x9E,0x95,0x17,0x23,0x21,0xC2,0x07,0xF3,0xD7,0x4F,0x86,0x8D,0xC7,0x1C,0x78,0x1D,0x70,0x40
	,0x76,0x52,0x28,0x59,0xAC,0x42,0x35,0x57,0xB4,0x84,0x92,0xE6,0xAB,0x89,0x14,0x9D,0x8D,0xC0,0xA7,0xCB
	,0x46,0x60,0x47,0x7E,0x36,0x42,0x9A,0xBB,0xB0,0xF7,0x46,0x37,0x92,0xDA,0x99,0x45,0xDE,0x82,0x00,0x9B
	,0x1C,0xDD,0xF2,0x08,0x63,0x9D,0xA7,0x22,0x31,0xD2,0x47,0xD8,0x9C,0x69,0xB7,0xB3,0xAC,0x0C,0xE1,0xDB
	,0x3D,0x61,0x66,0x34,0x09,0x93,0x0D,0x0C,0x30,0x7D,0xD2,0x64,0x2C,0x9A,0x31,0x03,0xFE,0x1E,0x6E,0x08
	,0x09,0xF2,0x40,0x07,0x59,0x45,0x46,0x16,0xE0,0x68,0xEC,0xB3,0xD2,0x62,0xEC,0xDB,0xB0,0x11,0x06,0x23
	,0x47,0xE1,0x6F,0xEF,0x7F,0x80,0x8F,0x3E,0x78,0x1F,0x53,0xC6,0x8F,0x83,0xAF,0xDB,0x46,0x3C,0x2D,0x2E
	,0xD2,0x10,0x50,0xD0,0xF8,0x1A,0x72,0xA9,0x4F,0x96,0x0E,0x85,0x6B,0x6E,0x21,0xB6,0xD6,0xCB,0x90,0xD8
	,0x4E,0xF1,0x20,0xC4,0x9D,0x6E,0x12,0x3F,0x4B,0x80,0x15,0xA7,0x49,0x5B,0x17,0xC3,0xD3,0x77,0xB5,0xA8
	,0x39,0x42,0xDB,0x85,0x2C,0xCC,0x9E,0x32,0x19,0xBE,0x7E,0xBE,0xB8,0xD5,0xD6,0x06,0xA5,0x52,0x29,0xE0
	,0xE1,0xC3,0x87,0x08,0x0B,0x0B,0x83,0xE1,0x38,0x7D,0x64,0x1D,0x0E,0x13,0xE2,0xE2,0xE6,0xF9,0x73,0x30
	,0x1A,0x37,0x16,0xEE,0x1E,0x1E,0xB8,0x70,0xE1,0x02,0x4A,0x4B,0x4B,0x91,0x97,0x97,0x87,0xD4,0xD4,0x54
	,0xB8,0x6F,0xF1,0x80,0xC1,0x28,0x3D,0xD4,0x9F,0x4A,0x11,0x2D,0x21,0xC4,0x48,0x25,0xD2,0x4E,0x1E,0xC2
	,0x60,0x8B,0xA1,0x70,0x2E,0xBE,0x89,0x78,0x6A,0x57,0x44,0xE1,0x59,0x56,0xEA,0x4F,0x42,0x43,0x80,0x55
	,0x50,0x56,0x5C,0xA2,0x5B,0x9E,0x60,0xC3,0xB9,0x13,0xF8,0x2A,0xD0,0x1D,0xA6,0xFB,0xEC,0xA9,0x10,0xCD
	,0xC5,0x82,0x4D,0xA6,0xE8,0xAE,0x2C,0x17,0xB4,0xF4,0xA2,0xB8,0x18,0x5F,0x1A,0x1A,0xA2,0xAE,0xAE,0x9E
	,0x84,0x46,0xBF,0x8B,0xBD,0x6B,0x23,0x52,0x23,0x86,0x0D,0x43,0x18,0x59,0x68,0x82,0x9E,0x1E,0xA4,0x14
	,0xB0,0xBC,0x8E,0xC1,0x4A,0x25,0x8F,0x6B,0xD7,0xAE,0xC1,0x78,0xE2,0x44,0xDC,0xCB,0xCD,0xD5,0x58,0x82
	,0xA3,0xF1,0x6B,0x3D,0xBF,0xC1,0xDF,0x56,0xEA,0xE1,0xCB,0x9D,0xD6,0x30,0xFC,0x6E,0x01,0xA6,0x52,0x90
	,0x6F,0xCD,0x4C,0x25,0x59,0xBB,0x91,0xA8,0x92,0xB9,0x17,0x81,0x44,0x2A,0x2E,0xAB,0xE3,0x42,0x31,0xD2
	,0x76,0x34,0x0E,0x86,0xB8,0x21,0x2F,0xE5,0x28,0x52,0x4F,0x05,0xE1,0xB3,0x65,0x43,0xD1,0x51,0x52,0x20
	,0xFA,0x3B,0x69,0x68,0x9F,0xAB,0x33,0x82,0x82,0x43,0x04,0x8D,0xFF,0xD4,0xC5,0xFE,0x76,0x38,0x34,0x14
	,0xEF,0xBE,0xF7,0x1E,0x4E,0x24,0x27,0xFD,0xEC,0xD8,0xB4,0xB4,0x34,0xB8,0x2E,0xB3,0xEA,0x21,0x40,0x6E
	,0xD7,0x51,0x9A,0x07,0xFD,0x65,0x9F,0x22,0xFD,0xF4,0x21,0xD4,0x5C,0x38,0x81,0xA8,0x08,0x2F,0x98,0x38
	,0x4C,0xC0,0x74,0xAA,0xF6,0x51,0xCD,0x8F,0xB5,0x08,0x50,0x9A,0x62,0x95,0x71,0xA6,0x8F,0x03,0xA6,0xD8
	,0x4F,0x40,0x7B,0x51,0x16,0x2D,0x22,0x11,0xB4,0xD0,0x29,0x2D,0x45,0x7D,0xE6,0x69,0x61,0x51,0xE6,0x3A
	,0x0A,0xCA,0x38,0x13,0xF5,0x46,0xE3,0xD9,0xB3,0x67,0xAA,0xED,0x7B,0x2E,0x25,0xAF,0x40,0x43,0xE3,0x2B
	,0x64,0x67,0x3F,0xC3,0x3F,0xEF,0xBD,0x42,0x49,0x49,0x11,0x3E,0xFC,0xCB,0x5F,0xD1,0xD9,0xF9,0x0A,0xA7
	,0x4E,0x3F,0x41,0x50,0xC8,0x13,0x34,0xB7,0xBE,0x24,0x81,0x39,0xD5,0x8C,0x9E,0x8B,0xE3,0x38,0x72,0xBB
	,0x71,0xE8,0xAA,0x10,0xAD,0xAC,0xC6,0xAD,0xBC,0x0C,0xCA,0x70,0xF9,0xC2,0xDE,0xEC,0x59,0x56,0x55,0x81
	,0xA0,0xC3,0xEE,0xD0,0x77,0x99,0x81,0xC8,0xA6,0x87,0x44,0x40,0x21,0x5A,0xC0,0x31,0xE9,0x28,0x0C,0x56
	,0x8C,0xC2,0x23,0xDA,0x94,0x0D,0x14,0x72,0x37,0x09,0xAB,0xCE,0xD5,0x62,0xB6,0x90,0x40,0x56,0x5D,0x05
	,0x13,0x23,0x23,0xD5,0xB6,0xEA,0x8B,0xFC,0x1F,0x3C,0xCE,0xA7,0x3D,0xC7,0xBF,0xFD,0xFE,0x29,0x69,0xFD
	,0x47,0xFC,0xF1,0x8F,0x0F,0x11,0x13,0x7B,0x03,0xB3,0x67,0xCE,0x81,0x87,0xC7,0x03,0x7A,0xD7,0x89,0x77
	,0xDF,0xE9,0xC2,0xE0,0xC1,0xF7,0xD1,0xD0,0xF0,0x52,0x35,0x4F,0xEB,0x22,0x03,0x99,0x9B,0x9B,0xA3,0x25
	,0xF5,0x9C,0x46,0x78,0x06,0x96,0x2C,0x18,0xD4,0xCF,0x2C,0x73,0xC9,0xC9,0x13,0xFC,0x43,0x36,0x91,0x4B
	,0xAD,0x20,0x57,0x92,0x61,0x50,0x5C,0xDB,0x2B,0xFC,0x7D,0xF9,0x08,0x94,0xA7,0x1F,0x47,0xC7,0xA5,0x8B
	,0xBD,0x16,0xE8,0x0D,0x9A,0x4C,0x8B,0xCD,0x30,0x31,0xE9,0xE3,0x12,0x24,0xBE,0xB2,0x1B,0xDF,0x6E,0xB9
	,0x87,0x77,0xDE,0xED,0xC2,0x3B,0xEF,0x74,0x0B,0xF7,0x1D,0x3B,0x7E,0xC0,0xAC,0x99,0x66,0x98,0x33,0xF7
	,0x11,0xBD,0xE3,0x09,0x0A,0xBC,0xF7,0x5E,0x17,0xD2,0x2F,0x3C,0x56,0xCD,0xEB,0x7D,0xCD,0x9F,0x3B,0x0F
	,0xB7,0x32,0xD2,0x74,0xEC,0xDB,0x03,0x05,0xE1,0x66,0xF6,0x39,0xBC,0xAA,0x2A,0xC6,0xE7,0x2B,0x59,0xCD
	,0xA8,0xC4,0xA0,0x9D,0xF9,0x85,0x98,0xE6,0x30,0x11,0x8F,0x24,0x85,0xC8,0x4C,0x0C,0xD6,0x39,0x51,0x0D
	,0xB6,0xC0,0x5C,0x63,0x63,0x3C,0xE8,0xE8,0xA0,0x2D,0xD5,0x24,0xE8,0xCE,0xF3,0x68,0xA3,0x6C,0x65,0x68
	,0x78,0x0F,0xBF,0xFB,0xDD,0x03,0x4C,0x98,0xF0,0x00,0x8D,0x4D,0x2F,0x30,0x6E,0xFC,0x78,0x94,0x96,0x75
	,0xC0,0x68,0xD2,0x03,0x7C,0xFC,0x3F,0x4F,0xB1,0xC1,0xF5,0x3E,0xBA,0x65,0x72,0xD5,0xBC,0x9E,0xAB,0xAB
	,0xBB,0x0B,0x9F,0x91,0x6B,0xB2,0x1A,0xA1,0x6B,0x5F,0x35,0x58,0x9A,0xAD,0x4E,0x4F,0xA2,0xFE,0x2A,0x14
	,0x01,0x87,0x36,0x62,0xC9,0xF7,0x5E,0x18,0xB4,0x22,0x26,0x08,0xEB,0x77,0x2F,0x85,0x24,0x2D,0x01,0x19
	,0x09,0x41,0xE4,0x6F,0x3D,0x26,0xD3,0x85,0x78,0x6F,0x6F,0xB8,0x6D,0x76,0x23,0xAD,0xF3,0xAA,0xED,0xC5
	,0x4B,0xA9,0x54,0x80,0xA3,0x38,0xE8,0xEC,0xE2,0xE8,0xCE,0xA1,0xA5,0xB9,0x11,0x1F,0x0C,0xFE,0x33,0x02
	,0xFC,0xFC,0xC0,0x29,0x64,0xE0,0x05,0xC8,0x85,0x71,0x7D,0xAF,0xE3,0x31,0xC7,0xB1,0x6D,0xCD,0x6A,0x9D
	,0xFB,0x69,0x83,0xC5,0xC2,0x0B,0x69,0x31,0x3C,0xBC,0x6C,0x90,0x71,0x22,0x04,0x66,0x07,0x36,0x60,0x90
	,0x75,0xE4,0x7E,0xD8,0x6D,0x35,0x17,0x4A,0x7D,0xD9,0xF9,0x48,0x9D,0x13,0xB5,0xD1,0x4D,0x31,0xF1,0xCD
	,0xF4,0xE9,0x08,0xFF,0xFE,0x08,0x29,0x9E,0x27,0xFD,0xF7,0x76,0x27,0x05,0x4B,0x8D,0x37,0xAF,0x63,0x06
	,0xD5,0x89,0xD2,0xA4,0x44,0x7C,0x31,0x66,0x0C,0x0A,0x0B,0x0A,0xFA,0xB8,0x9D,0x78,0xB1,0xB9,0x55,0xD5
	,0xD5,0x98,0x41,0x71,0xF5,0xF4,0x72,0x4F,0x41,0xFB,0x69,0x48,0x21,0xAF,0x96,0x60,0x5F,0xA0,0x13,0xCE
	,0xC4,0xF8,0x61,0x6E,0x00,0x11,0x70,0xA7,0xDC,0xAA,0x6F,0x37,0x4C,0x48,0x95,0xCF,0x25,0xC5,0xFD,0x26
	,0x29,0xA8,0x4F,0xD1,0x7E,0x66,0xA9,0xF4,0x79,0x71,0x29,0x6C,0xE7,0x7F,0x05,0xAB,0xA5,0x4B,0x50,0x55
	,0x55,0x2D,0x10,0x61,0xD7,0xE3,0xC7,0x8F,0x71,0xC0,0xD7,0x17,0x16,0xB3,0x67,0xA2,0xF5,0x42,0x9A,0x10
	,0x74,0x77,0xB2,0x73,0x60,0x36,0xC9,0x08,0xDE,0xBB,0xBD,0xD1,0x21,0xB8,0x9E,0x78,0xDD,0xB9,0x73,0x17
	,0xDE,0x64,0xCD,0xD9,0xC6,0x53,0xD0,0x96,0xCD,0x32,0x5F,0x7F,0xCB,0x8B,0xC9,0x43,0xFB,0x99,0x9A,0x46
	,0xBA,0xDF,0xCA,0xCF,0xC0,0x6A,0x4F,0x73,0x38,0x9D,0x8C,0xC5,0xA0,0x78,0xF2,0xDD,0x91,0x0E,0x53,0x70
	,0x3A,0x29,0x50,0x68,0x75,0x19,0x98,0x1B,0xC9,0xAA,0xCA,0x91,0x14,0xB3,0x1F,0xC7,0x8F,0xEE,0x12,0xF2
	,0x32,0x0B,0x62,0xED,0xC5,0x58,0x3C,0x14,0x45,0x47,0xC1,0xD1,0xC2,0x02,0x0B,0xA8,0x85,0x58,0xB1,0x78
	,0x31,0x36,0x59,0x5B,0x23,0xFF,0xD8,0x31,0x61,0x13,0xCD,0xA6,0x64,0x76,0x99,0xA4,0x1C,0xA7,0xFC,0xFD
	,0x61,0xBF,0xD8,0x02,0xB6,0xDF,0x2C,0x84,0xE5,0xFC,0x79,0xB0,0x9C,0x35,0x1B,0xF1,0x3E,0x7B,0xD1,0x45
	,0x7F,0xD3,0x5E,0x57,0x1B,0x05,0x27,0x23,0x10,0x1C,0xBA,0x19,0xCF,0xCB,0x4A,0x54,0xD9,0x90,0x52,0x39
	,0xBD,0x4F,0x4D,0x3A,0x88,0x21,0x14,0xC4,0x51,0xAD,0x4F,0x31,0x28,0x91,0xCA,0x73,0xE8,0xD5,0x56,0x18
	,0xB8,0x4C,0xC3,0x3F,0xFC,0x1D,0x91,0x49,0x65,0xFC,0xFB,0x23,0x9E,0x98,0xE6,0x68,0x8C,0xFF,0x5A,0x3E
	,0x94,0x88,0xF9,0xD1,0x24,0x4A,0xA7,0x7D,0x08,0x88,0x1A,0x13,0xB5,0xC6,0x84,0x14,0x63,0x47,0xF5,0x4C
	,0x00,0xA5,0x61,0xED,0xB1,0xEC,0x9D,0xD8,0xFC,0x51,0x55,0x16,0xD6,0x52,0xAF,0xA7,0x3D,0xAE,0x37,0x9E
	,0x50,0x27,0xFB,0x1F,0x5F,0xFD,0x09,0x43,0xA8,0xA0,0xB9,0x78,0x2D,0xA1,0x36,0x65,0x2B,0xDC,0xF6,0xD8
	,0xD2,0xE1,0xC9,0x18,0xC1,0xD2,0xEB,0x54,0x07,0x28,0x8D,0xAA,0x2B,0x5A,0x0C,0x59,0x62,0x4B,0x46,0x2A
	,0x96,0x1F,0xDB,0x07,0x07,0xAA,0x0B,0x61,0x74,0xAA,0x3A,0x58,0x53,0x8F,0x2F,0x1D,0x26,0x51,0xBB,0x5B
	,0xDE,0x2B,0x1F,0xFF,0xD6,0x50,0x9F,0x21,0x7C,0x03,0x5C,0xF0,0xA9,0xBB,0x0D,0x9C,0xCA,0xDA,0xE0,0x7C
	,0x26,0x19,0x6B,0xE2,0x0E,0x63,0xC7,0xC5,0x3C,0xC4,0xB7,0x77,0x69,0x0E,0x3C,0x3D,0xAD,0x84,0x0A,0x62
	,0xC3,0x44,0x5D,0x20,0x1D,0x17,0x23,0xDA,0x65,0x30,0xD8,0xEF,0x89,0xD8,0x88,0x3D,0x42,0x0A,0xD3,0x3E
	,0x5D,0xFD,0x9A,0x60,0x71,0x25,0xFC,0x66,0x67,0x85,0xD2,0x32,0xF0,0x97,0x8A,0x70,0x23,0x35,0x19,0x7F
	,0x5F,0xAA,0x87,0x45,0x85,0xED,0x70,0xA9,0x65,0x1F,0x02,0x58,0x5B,0x2D,0x9E,0x06,0x45,0x88,0x4D,0x9E
	,0x86,0x40,0x5F,0xB0,0x2A,0xB7,0xF5,0x9A,0x1C,0x16,0x65,0x4F,0x31,0x72,0xFD,0x4C,0xB4,0x15,0x66,0xF4
	,0xDB,0xF8,0x57,0x41,0x59,0x39,0xF8,0xE2,0xCB,0xE0,0x0B,0x29,0x53,0xE5,0xE6,0x41,0x79,0x31,0x17,0x9D
	,0x59,0x17,0x31,0x6B,0xAD,0x31,0x66,0x25,0xA7,0x61,0xA9,0x84,0xC3,0x2A,0x29,0x6B,0x36,0x99,0xC6,0xFB
	,0xCB,0xF9,0x93,0x04,0x22,0xDB,0x39,0xD8,0x56,0xCA,0x61,0x45,0x0B,0x98,0x67,0xD6,0x60,0xFA,0x86,0x59
	,0x78,0x5A,0x56,0x2C,0x04,0xB9,0x4E,0x41,0x06,0x00,0x5E,0x52,0x41,0xC2,0x97,0x80,0x2F,0x2A,0x06,0x9F
	,0x47,0x3D,0x0F,0x09,0x2F,0xBF,0x98,0x83,0x4D,0xDB,0x2D,0x61,0x18,0x12,0x4C,0x7B,0xCB,0x89,0x80,0x02
	,0xCB,0x48,0x06,0xE6,0x0D,0xBA,0x0E,0xFA,0xFD,0x08,0xB0,0x96,0x9A,0xB9,0xCF,0xAE,0x9B,0xDD,0xB0,0xAC
	,0xE4,0x05,0x0D,0x30,0x12,0x66,0xA7,0x73,0x31,0x67,0xD3,0x6C,0xDC,0x2F,0xC9,0x23,0x77,0x62,0x01,0xA8
	,0x4D,0x44,0x3C,0x9D,0xB1,0x96,0xBB,0x27,0x58,0xD9,0x7B,0x4A,0x7B,0xE4,0x16,0xBC,0x90,0x41,0x5E,0x47
	,0x9C,0xB2,0x1F,0x1D,0xEC,0x39,0x22,0xE0,0xE9,0xB5,0x02,0x13,0x0E,0xEC,0x24,0xA1,0x65,0x82,0xF0,0x0C
	,0x56,0x15,0x1C,0x42,0xE8,0x88,0xF9,0xDA,0xF3,0x80,0x1A,0x71,0x34,0x28,0x96,0x3A,0xD4,0xB5,0x55,0x5D
	,0x02,0x01,0x2B,0x09,0x23,0x41,0x8B,0x90,0x36,0xCC,0xCE,0x16,0x62,0xCC,0x1A,0x43,0xE4,0x9C,0x0C,0xD3
	,0xB2,0x04,0x3B,0xD0,0x57,0xD0,0x59,0x38,0x1A,0x9F,0xAF,0x18,0x03,0x49,0x7A,0xAC,0x90,0x46,0x79,0x3A
	,0x2F,0xCB,0xAB,0x2B,0x61,0xEF,0x6E,0x8E,0xF6,0x4B,0xE4,0x7E,0xEC,0xFC,0xDC,0x4B,0x68,0x31,0xA6,0x18
	,0xF8,0x92,0x52,0xBC,0xA4,0x5A,0xE0,0xE4,0xBE,0x18,0x13,0x83,0xFC,0x68,0xAF,0x6E,0x8D,0xF0,0x6A,0xF8
	,0x34,0x30,0xF9,0xDE,0x90,0x40,0x60,0x33,0xB1,0xAF,0xE4,0x68,0xA2,0x28,0xBC,0x36,0x16,0xE6,0x36,0xE1
	,0xBF,0xD7,0xCD,0x22,0x4D,0xD9,0xE1,0x59,0xC1,0x45,0x28,0x24,0xAC,0x7F,0xAF,0x40,0x48,0xA8,0x3B,0xBC
	,0x03,0x1C,0xF1,0xA5,0xA3,0x01,0xC6,0xDB,0x0C,0xC3,0x54,0xEA,0xDD,0xC7,0xDB,0x8D,0xC0,0xFB,0x0B,0xFE
	,0x84,0x5D,0x7B,0x56,0xF5,0xB6,0x00,0x0B,0x5A,0x16,0xB0,0x74,0x2C,0xE5,0x0A,0x2F,0x41,0x72,0xE2,0x18
	,0x4C,0x1C,0x4D,0x30,0x2D,0x3E,0x45,0xE3,0x36,0xBD,0x40,0xB2,0x6C,0xBD,0xC6,0xBE,0x50,0xF4,0x96,0x95
	,0x41,0x87,0x0B,0x29,0xE0,0x56,0xDF,0xD5,0x7F,0x11,0x15,0x98,0x3B,0x59,0x4A,0xA8,0x71,0x3B,0x14,0x8C
	,0x11,0xD6,0x63,0xB1,0x7F,0xFF,0x5A,0xDC,0xC9,0x4C,0x01,0x57,0x56,0x2A,0x9C,0x21,0x38,0x12,0xF4,0x65
	,0x65,0x19,0xEE,0x17,0xE7,0xE2,0x71,0x59,0x01,0xBA,0x6A,0x2A,0xC0,0x51,0xF9,0x67,0x99,0x46,0xF0,0xF9
	,0x12,0xF2,0xF9,0xC2,0x42,0xF2,0xF7,0x3C,0xDC,0x48,0x89,0x81,0xC3,0x77,0x5F,0x63,0x88,0x8B,0x19,0x16
	,0x5E,0x6C,0xC4,0x12,0x61,0xFD,0xFE,0x7B,0x2E,0x23,0xB8,0xD6,0xA9,0xBF,0x84,0xF7,0x96,0xB7,0x1F,0x81
	,0x68,0xF2,0x7F,0xDB,0xCA,0x6E,0x5A,0xA8,0xBF,0xF6,0xD5,0x10,0x36,0xA1,0x00,0x5F,0x54,0x70,0x17,0xC3
	,0xB7,0x58,0xE2,0xAF,0x5F,0x0F,0x86,0x33,0x1D,0xFB,0x1A,0xCE,0x24,0x40,0x41,0x01,0xA9,0x20,0xCD,0x0A
	,0x55,0x93,0x6A,0x07,0x5F,0x41,0x87,0x23,0x3A,0x07,0x73,0x97,0x0A,0xF1,0x22,0x27,0x0B,0x75,0xC9,0x91
	,0x08,0xF3,0xDF,0x80,0xA5,0xCE,0x26,0xB0,0xFE,0xD6,0x0C,0xF3,0x37,0x18,0xC3,0x28,0x2C,0x84,0xD6,0x65
	,0x16,0xEF,0xBF,0x97,0x80,0x4A,0x05,0xEC,0xAB,0x89,0xC0,0xDD,0xDE,0xB2,0x32,0x68,0x11,0x60,0xEC,0x38
	,0xEC,0x6F,0x92,0x69,0x09,0x2F,0x2E,0xAA,0x36,0x2B,0x13,0x5C,0xD4,0x90,0x18,0xD8,0x8B,0x2E,0xDD,0xC2
	,0xB0,0xD5,0x46,0xB8,0x7E,0x3E,0x01,0x09,0x21,0xDB,0x31,0x91,0x2A,0xE6,0x62,0x27,0x63,0x44,0x05,0xBB
	,0x21,0x3B,0xE2,0x00,0xCE,0x1E,0xF1,0x86,0xAF,0xCF,0x6A,0xD8,0xBA,0xCE,0xC0,0xBC,0xB5,0x86,0xB0,0xD9
	,0x6C,0x86,0x3D,0xFE,0xEB,0x90,0x4B,0x31,0xF4,0xBC,0xFC,0xB2,0x40,0x30,0x2A,0x72,0x17,0x46,0xEE,0x70
	,0x80,0x55,0x25,0xDB,0x97,0xAD,0xCD,0xF6,0xD2,0x76,0x5F,0x76,0xE7,0x60,0x43,0x41,0x7D,0xFC,0x75,0x16
	,0x88,0xA7,0x3C,0x1B,0x7B,0x57,0x09,0xB7,0x3A,0xB9,0x90,0xB6,0x34,0xA6,0x24,0x4D,0x2F,0x2E,0xBC,0x0B
	,0xE3,0x63,0xD1,0x18,0xB5,0xD3,0x15,0x1F,0x3B,0xCF,0xC7,0x90,0x75,0xA6,0x18,0xE1,0x64,0x8A,0xD1,0x14
	,0xD0,0xB9,0xD9,0xF1,0x50,0xD6,0x54,0x41,0x51,0x5C,0x84,0xAE,0xDC,0x6C,0x54,0x51,0x15,0xCF,0x89,0x3C
	,0x80,0xAC,0x63,0xBE,0xD4,0x2B,0x05,0xE3,0x4A,0x72,0x04,0x1E,0x66,0x9E,0x87,0x9C,0xB2,0x8C,0x76,0x00
	,0x33,0x28,0xA4,0xD5,0xA8,0x48,0x3D,0x8E,0xFF,0xB4,0x1C,0x8A,0xD1,0xDE,0x1B,0x31,0xDA,0x77,0x3B,0xBE
	,0x08,0x0B,0xC7,0xEC,0x94,0x3C,0x58,0x51,0xFD,0x59,0x42,0x7B,0x8B,0xB1,0xC8,0xDC,0x48,0x8E,0xF0,0x36
	,0x31,0x4B,0xFE,0x04,0x01,0x1E,0x31,0x94,0x7D,0x58,0xEE,0x67,0x8C,0xD9,0x64,0xD3,0xF8,0x33,0x18,0xB9
	,0xD9,0x02,0x6B,0x76,0xDB,0x20,0xE1,0x64,0x30,0x6A,0x4B,0x32,0xF1,0xF4,0xE6,0x15,0x74,0x35,0x37,0x40
	,0xD6,0xDA,0x00,0xAE,0xE1,0x3A,0x14,0x35,0x35,0x62,0x50,0x32,0x1F,0x67,0x29,0x93,0xAA,0x28,0xCB,0xE7
	,0xAC,0x20,0x29,0x2E,0x52,0x90,0x13,0x94,0x39,0x74,0x2F,0xBA,0xD4,0x8F,0x00,0x4F,0xFD,0x92,0x9C,0x5C
	,0xED,0xFE,0xE5,0x3C,0xB4,0xE6,0xA6,0xE2,0x4A,0x06,0x9D,0x49,0x92,0x42,0xB0,0x6D,0xDF,0x5A,0x0C,0xB7
	,0x1B,0x87,0x31,0xBB,0x5C,0x48,0x79,0x3F,0x08,0x4A,0x64,0x16,0xF7,0x6F,0x66,0x72,0x52,0x30,0xEB,0x22
	,0x90,0x40,0x04,0x0E,0xB5,0x70,0x82,0xF6,0x97,0x92,0x39,0xC7,0x52,0x0B,0x61,0xEE,0x3E,0x0F,0x77,0x6A
	,0x49,0xA8,0xA6,0x26,0xF0,0x37,0xAE,0x42,0x71,0xB5,0x0E,0xFC,0x95,0x5A,0xD2,0x78,0xB5,0xE8,0xDF,0xEA
	,0x16,0x40,0x5B,0x28,0x56,0x13,0xA8,0xC3,0xE4,0xA9,0xBF,0xE7,0xF3,0xA8,0xB2,0xE6,0x88,0x64,0x78,0x3A
	,0x13,0xA8,0xD3,0x66,0xDF,0x39,0x7D,0xC1,0x3A,0xCE,0x17,0xD4,0xDA,0x1F,0x3D,0xF2,0x0F,0xE8,0xAD,0x99
	,0x8A,0x65,0xA5,0xCF,0x84,0x38,0xF0,0xBA,0x29,0xCA,0xCA,0xEA,0x94,0x0E,0x02,0x1C,0xBC,0x6E,0x88,0xBE
	,0x6E,0x51,0x78,0x1B,0xC3,0x57,0x8D,0xC7,0x8F,0x4D,0x24,0x74,0xD3,0x4D,0x12,0xB4,0x5A,0xE7,0x46,0x0C
	,0x7D,0x7B,0xF6,0x5E,0x20,0x8B,0x28,0x4A,0xA8,0xCA,0x92,0xFB,0x30,0xAB,0xF0,0xF4,0x5B,0x49,0x29,0x57
	,0xE7,0xD8,0x7E,0x10,0x3B,0x56,0xCF,0xB5,0x6B,0x60,0x1A,0x74,0x86,0x3C,0x42,0x81,0x2D,0x57,0xB5,0x3F
	,0x72,0xF5,0x25,0x40,0xEE,0xB3,0xB1,0x8E,0xD2,0x27,0xF9,0x9C,0x51,0xF8,0x61,0xEC,0x39,0xBC,0x19,0x7C
	,0x0B,0x69,0xBE,0x99,0x40,0x1A,0xD7,0xBD,0xC9,0x9B,0x82,0xAC,0x52,0x5E,0x06,0x05,0x3B,0x75,0xBD,0xA6
	,0xFF,0xEF,0x0F,0x29,0x4E,0xD3,0x39,0x42,0xDF,0xD6,0x8D,0x5C,0x48,0x0E,0xA7,0x5A,0x56,0xA7,0x7A,0x7F
	,0xA1,0xD3,0x10,0x88,0x21,0x17,0x5A,0x49,0x4D,0x93,0x15,0x45,0xFB,0x10,0xD7,0x79,0xB8,0x2E,0xC9,0x07
	,0x1D,0x6C,0xC1,0xB7,0x36,0x01,0x8D,0x0D,0xE0,0x6B,0xAF,0xA8,0xB4,0xAD,0xEE,0xE3,0x7F,0x7B,0xF0,0x64
	,0xAD,0xE6,0xF4,0x74,0x7C,0x64,0x60,0x82,0xE5,0xE4,0xDA,0x2B,0xAB,0xD4,0x5F,0x10,0x75,0x10,0x08,0xBB
	,0x25,0x06,0x8A,0x65,0x69,0x07,0xF4,0xED,0x3F,0x07,0x77,0xAB,0x41,0x20,0x00,0xB2,0x82,0x92,0x48,0xF0
	,0x8C,0xCC,0x8D,0x1B,0xE0,0xAB,0xFB,0xB7,0x04,0xBF,0x19,0x28,0xC6,0xBA,0xE9,0xFE,0xF1,0xC7,0xC3,0xB1
	,0xBC,0xF8,0x39,0x91,0x90,0x51,0x9D,0x62,0xF2,0xEA,0x20,0xE0,0xD3,0x28,0xA6,0xAC,0x99,0xA7,0x72,0xB1
	,0xCE,0xC7,0x4E,0x10,0x5A,0xB0,0x40,0x3D,0x05,0x6E,0x6D,0xAD,0x00,0x25,0xB3,0xC2,0x15,0x55,0xD6,0xE9
	,0xBB,0xD9,0x6F,0x00,0x16,0xF0,0x0A,0xDA,0x6B,0xEE,0xE4,0x29,0xF8,0x2A,0xB9,0x5A,0xC8,0x8C,0xA1,0xAD
	,0xDA,0x67,0x02,0x46,0xE0,0xB6,0xF8,0x1F,0x11,0x16,0x20,0x2C,0x80,0x3F,0xF3,0xDB,0x83,0xC4,0x13,0x87
	,0x54,0xDA,0x6F,0x26,0xA1,0x49,0x60,0x5A,0x88,0x65,0x97,0x8A,0xD8,0x58,0x32,0x6B,0x8F,0x0B,0xBD,0x49
	,0x46,0x19,0x08,0xD4,0xD9,0x4D,0x9D,0x20,0x3C,0xED,0xED,0x31,0x65,0x57,0x24,0x35,0x97,0x54,0x68,0x1B
	,0xD4,0x81,0xAC,0x21,0xA0,0x44,0x1C,0x61,0x4D,0x35,0xB9,0x10,0xA5,0xCF,0x8F,0xD6,0x9B,0xA1,0xA5,0xFA
	,0x32,0xD0,0xDC,0xD2,0x8B,0x00,0xD3,0x84,0xB9,0xC9,0x14,0xEA,0x75,0x7A,0x08,0xB0,0x77,0xAF,0xCD,0x42
	,0x6F,0x00,0x36,0xFF,0x49,0x09,0xF5,0x46,0x5A,0x56,0x55,0x48,0x6B,0x50,0x18,0x15,0x49,0x7F,0x13,0x1B
	,0xC0,0xF3,0x81,0x81,0x18,0x63,0xE9,0x2A,0x58,0xC0,0x93,0x0E,0x59,0x7D,0x5C,0x88,0xC3,0xD1,0x5B,0x1C
	,0xAC,0x2B,0xC8,0x02,0xE5,0x8F,0x30,0x66,0xD5,0x04,0xC8,0x5B,0x1B,0x35,0x16,0x50,0xD4,0x93,0xEB,0x08
	,0x8B,0x4A,0x60,0xFC,0xD9,0x38,0xB4,0x66,0xA4,0x0B,0xCF,0x1C,0x59,0xC2,0x69,0x9B,0x05,0x32,0x13,0x0F
	,0x69,0x36,0x1E,0x08,0xBA,0x25,0x65,0x28,0x4F,0x89,0x82,0x9C,0x75,0xA7,0xAA,0x77,0x0F,0xA8,0xD9,0x9B
	,0x3F,0x69,0x92,0x46,0x39,0x77,0xB2,0xB2,0xF0,0xA1,0x9E,0xA1,0x90,0x89,0x5C,0xA8,0x53,0x60,0x9F,0xD8
	,0xB5,0x08,0xC8,0xB1,0xB7,0x91,0xFA,0x10,0x62,0x37,0x33,0x39,0x07,0xEB,0x7D,0x6C,0x84,0x80,0x45,0xB3
	,0x48,0x80,0xA7,0xE2,0x25,0xB8,0x0A,0x1D,0x4A,0x56,0x98,0x9B,0xE3,0x6C,0x40,0x80,0xB0,0x28,0x5B,0x5C
	,0xE8,0x30,0xB5,0x5C,0x6A,0xE0,0xE8,0x7D,0xD8,0x29,0x8E,0x8E,0xC4,0x64,0x7D,0x7D,0xCD,0x33,0x47,0x67
	,0x89,0xE1,0x9F,0x8C,0xC0,0x92,0xFC,0x7B,0x58,0x55,0x25,0x43,0xAC,0xC6,0x02,0x72,0xFC,0x2F,0xF5,0x92
	,0xAE,0x65,0x3F,0x05,0xC9,0x10,0x00,0x00,0x00,0x00,0x49,0x45,0x4E,0x44,0xAE,0x42,0x60,0x82
};
const char UPnPDeviceIcon_SMJPG[1603]={
	0x48,0x54,0x54,0x50,0x2F,0x31,0x2E,0x31,0x20,0x32,0x30,0x30,0x20,0x4F,0x4B,0x0D,0x0A,0x43,0x4F,0x4E
	,0x54,0x45,0x4E,0x54,0x2D,0x54,0x59,0x50,0x45,0x3A,0x20,0x69,0x6D,0x61,0x67,0x65,0x2F,0x6A,0x70,0x67
	,0x0D,0x0A,0x43,0x6F,0x6E,0x74,0x65,0x6E,0x74,0x2D,0x4C,0x65,0x6E,0x67,0x74,0x68,0x3A,0x20,0x31,0x35
	,0x33,0x37,0x0D,0x0A,0x0D,0x0A,0xFF,0xD8,0xFF,0xE0,0x00,0x10,0x4A,0x46,0x49,0x46,0x00,0x01,0x01,0x01
	,0x00,0x60,0x00,0x60,0x00,0x00,0xFF,0xDB,0x00,0x43,0x00,0x08,0x06,0x06,0x07,0x06,0x05,0x08,0x07,0x07
	,0x07,0x09,0x09,0x08,0x0A,0x0C,0x14,0x0D,0x0C,0x0B,0x0B,0x0C,0x19,0x12,0x13,0x0F,0x14,0x1D,0x1A,0x1F
	,0x1E,0x1D,0x1A,0x1C,0x1C,0x20,0x24,0x2E,0x27,0x20,0x22,0x2C,0x23,0x1C,0x1C,0x28,0x37,0x29,0x2C,0x30
	,0x31,0x34,0x34,0x34,0x1F,0x27,0x39,0x3D,0x38,0x32,0x3C,0x2E,0x33,0x34,0x32,0xFF,0xDB,0x00,0x43,0x01
	,0x09,0x09,0x09,0x0C,0x0B,0x0C,0x18,0x0D,0x0D,0x18,0x32,0x21,0x1C,0x21,0x32,0x32,0x32,0x32,0x32,0x32
	,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32
	,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32
	,0x32,0x32,0x32,0x32,0xFF,0xC0,0x00,0x11,0x08,0x00,0x30,0x00,0x30,0x03,0x01,0x22,0x00,0x02,0x11,0x01
	,0x03,0x11,0x01,0xFF,0xC4,0x00,0x1F,0x00,0x00,0x01,0x05,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00
	,0x00,0x00,0x00,0x00,0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0xFF,0xC4,0x00,0xB5
	,0x10,0x00,0x02,0x01,0x03,0x03,0x02,0x04,0x03,0x05,0x05,0x04,0x04,0x00,0x00,0x01,0x7D,0x01,0x02,0x03
	,0x00,0x04,0x11,0x05,0x12,0x21,0x31,0x41,0x06,0x13,0x51,0x61,0x07,0x22,0x71,0x14,0x32,0x81,0x91,0xA1
	,0x08,0x23,0x42,0xB1,0xC1,0x15,0x52,0xD1,0xF0,0x24,0x33,0x62,0x72,0x82,0x09,0x0A,0x16,0x17,0x18,0x19
	,0x1A,0x25,0x26,0x27,0x28,0x29,0x2A,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x43,0x44,0x45,0x46,0x47,0x48
	,0x49,0x4A,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6A,0x73,0x74
	,0x75,0x76,0x77,0x78,0x79,0x7A,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8A,0x92,0x93,0x94,0x95,0x96,0x97
	,0x98,0x99,0x9A,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,0xA8,0xA9,0xAA,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9
	,0xBA,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,0xDA,0xE1
	,0xE2,0xE3,0xE4,0xE5,0xE6,0xE7,0xE8,0xE9,0xEA,0xF1,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,0xF8,0xF9,0xFA,0xFF
	,0xC4,0x00,0x1F,0x01,0x00,0x03,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00
	,0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0xFF,0xC4,0x00,0xB5,0x11,0x00,0x02,0x01
	,0x02,0x04,0x04,0x03,0x04,0x07,0x05,0x04,0x04,0x00,0x01,0x02,0x77,0x00,0x01,0x02,0x03,0x11,0x04,0x05
	,0x21,0x31,0x06,0x12,0x41,0x51,0x07,0x61,0x71,0x13,0x22,0x32,0x81,0x08,0x14,0x42,0x91,0xA1,0xB1,0xC1
	,0x09,0x23,0x33,0x52,0xF0,0x15,0x62,0x72,0xD1,0x0A,0x16,0x24,0x34,0xE1,0x25,0xF1,0x17,0x18,0x19,0x1A
	,0x26,0x27,0x28,0x29,0x2A,0x35,0x36,0x37,0x38,0x39,0x3A,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x53
	,0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6A,0x73,0x74,0x75,0x76,0x77
	,0x78,0x79,0x7A,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8A,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99
	,0x9A,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,0xA8,0xA9,0xAA,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBA,0xC2
	,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,0xDA,0xE2,0xE3,0xE4
	,0xE5,0xE6,0xE7,0xE8,0xE9,0xEA,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,0xF8,0xF9,0xFA,0xFF,0xDA,0x00,0x0C,0x03
	,0x01,0x00,0x02,0x11,0x03,0x11,0x00,0x3F,0x00,0xF5,0x3A,0x6C,0x8D,0xB2,0x36,0x6C,0x13,0x81,0x9C,0x00
	,0x4E,0x7F,0x2E,0x69,0xB3,0x4F,0x0D,0xBC,0x7E,0x64,0xF2,0xC7,0x12,0x74,0xDC,0xEC,0x14,0x7E,0xB5,0x94
	,0xFA,0x98,0xBB,0xCB,0xC5,0x24,0x8B,0x66,0x64,0xF2,0xA2,0x78,0x17,0x7C,0xB7,0x52,0x73,0x95,0x8C,0x73
	,0xC0,0xC1,0xE7,0xD8,0x9C,0x80,0x33,0x5E,0x85,0x4A,0x91,0x82,0xD4,0xF1,0xA9,0x52,0x95,0x59,0x72,0xC4
	,0x96,0xC7,0x5E,0xB3,0xBB,0x8A,0xE1,0x9D,0xD6,0x06,0xB7,0x05,0xA6,0x57,0x61,0x85,0x03,0xF8,0xB3,0xE9
	,0xF9,0x11,0xDC,0x0A,0xB1,0x63,0xA9,0x41,0xA8,0x02,0xD6,0xF9,0x31,0xF5,0x0C,0x48,0x19,0xFC,0x33,0x91
	,0xF8,0x81,0x5C,0xF4,0xFA,0x68,0xB2,0xF1,0x05,0xB6,0xA3,0xA8,0x68,0x57,0x16,0x36,0xC5,0x4C,0x42,0x7B
	,0x9B,0x90,0xCA,0xEC,0x48,0x2B,0xE6,0xED,0x2E,0x3E,0xF0,0x00,0x16,0x23,0x9C,0x75,0x20,0x62,0xCC,0x72
	,0xAE,0xA2,0xBE,0x73,0xF8,0x4E,0x7B,0xB8,0x49,0xCC,0x57,0x96,0x31,0x29,0x5C,0x7A,0xAB,0x31,0x47,0x3E
	,0xCC,0xA3,0x9E,0xD9,0xAE,0x45,0x8B,0x9D,0xF5,0x47,0x6A,0xCB,0xEE,0xB5,0x9A,0x3A,0x4A,0x2B,0x04,0x6A
	,0x07,0x4D,0xB5,0x4B,0xB1,0x72,0xF7,0x5A,0x4B,0xB6,0xDF,0x36,0x50,0x44,0xB6,0xCD,0x9C,0x61,0xF3,0xC9
	,0x00,0xF0,0x73,0xF3,0x29,0xEB,0x9E,0x48,0xD8,0xB7,0xBB,0xB6,0xBB,0x52,0xD6,0xD7,0x11,0x4C,0xAA,0x70
	,0x4C,0x6E,0x18,0x0F,0xCA,0xBA,0xA9,0xD5,0x8D,0x45,0x74,0x71,0x55,0xA3,0x3A,0x52,0xE5,0x91,0x99,0xA0
	,0xC5,0x16,0xA2,0xD7,0x1A,0x95,0xCC,0x5E,0x6C,0xD2,0x48,0xEA,0x8D,0x20,0xC8,0x8D,0x03,0x32,0x84,0x50
	,0x7A,0x70,0xA3,0x3E,0xA4,0x9F,0x6A,0xD0,0x0F,0x69,0xA3,0xEB,0x9A,0x5D,0xDC,0xEA,0x90,0x69,0xF0,0xC1
	,0x2D,0xB0,0x7C,0x61,0x20,0x66,0x31,0xEC,0x27,0xB2,0x8C,0x23,0x2E,0x7B,0x6E,0x03,0xBD,0x55,0xBA,0xD2
	,0xED,0x84,0x57,0x0D,0xE6,0x5C,0xC5,0x14,0x99,0x69,0x22,0x86,0x66,0x45,0x66,0x3D,0x4F,0x1D,0x09,0xEF
	,0x8C,0x67,0xBD,0x3B,0x44,0xD4,0x12,0x6B,0x75,0xD3,0xAE,0x99,0x7E,0xD5,0x12,0x6D,0xDA,0x40,0x02,0x54
	,0x1C,0x6E,0x03,0xBF,0x1D,0x47,0x63,0xED,0x8C,0xF0,0xD4,0xA1,0x52,0x1E,0xFB,0xD5,0x1E,0xC6,0x13,0x15
	,0x46,0xA2,0x58,0x75,0xA3,0xB1,0x47,0xE2,0x25,0x9F,0x88,0x2E,0xAE,0x26,0x58,0x75,0x8B,0x46,0xD1,0x6F
	,0x2C,0x9E,0x05,0xD2,0xF6,0x03,0x2D,0xC4,0xA5,0x4E,0x02,0xF1,0xCF,0x25,0x4E,0xED,0xC0,0x28,0x19,0x3C
	,0x03,0x5A,0x9E,0x03,0x3A,0xC2,0x5B,0xEA,0x37,0xFA,0xBE,0xBF,0x6D,0x7B,0xA7,0xBB,0x03,0x6F,0x18,0x41
	,0x19,0xB5,0x03,0x39,0x57,0x18,0x1B,0x08,0x18,0x05,0x79,0xC6,0x2A,0x6B,0x4B,0x5D,0x32,0xCE,0xE2,0x48
	,0xAC,0xED,0xED,0x20,0x9B,0x68,0x69,0x12,0x14,0x55,0x6C,0x1C,0xE0,0x90,0x39,0xC7,0x06,0x87,0xD3,0xF4
	,0xCB,0xF7,0x4B,0xB7,0xB3,0xB4,0xB8,0x62,0x01,0x59,0x9A,0x25,0x72,0x47,0x62,0x0E,0x2B,0x47,0x8D,0x93
	,0x87,0x27,0x2A,0xB5,0x92,0xD9,0x5F,0x47,0x7D,0xFB,0xF7,0x7D,0x4E,0xAF,0xAA,0x2E,0xE4,0x36,0x62,0x0D
	,0x46,0xE3,0x58,0xB9,0x58,0xD5,0xEC,0x2F,0xAE,0x77,0x44,0x18,0x7C,0xB2,0xA7,0x94,0x88,0xCD,0x8F,0x46
	,0x2A,0xC4,0x7A,0x8E,0x7B,0xD5,0x3D,0x7E,0x28,0xEC,0x23,0x87,0x51,0xB6,0x81,0x63,0xB8,0x8D,0xC2,0x97
	,0x41,0x8D,0xCB,0xFD,0xC6,0xF5,0x04,0x64,0x0C,0xF4,0x24,0x11,0x53,0x6B,0x7A,0xAB,0x5B,0x44,0xD6,0x96
	,0xC1,0x5A,0xF2,0x65,0xC2,0x29,0xE4,0x20,0x3C,0x6F,0x61,0xE8,0x3D,0x3B,0x9E,0x2A,0xB6,0x97,0xA4,0x5B
	,0xAD,0x9D,0xB7,0x99,0x35,0xD5,0xC2,0xC2,0x41,0x44,0x9E,0x76,0x75,0x0C,0xBD,0x0E,0x09,0xE4,0xF7,0x19
	,0xE9,0xDB,0x15,0x95,0x2A,0x15,0x2A,0x7B,0xF1,0xD1,0x1C,0xD8,0xCC,0x55,0x1A,0x4B,0xEA,0xEF,0x57,0x62
	,0x6D,0x6E,0xE9,0x6D,0xED,0xD0,0x36,0xE3,0xB9,0x80,0xDA,0xA0,0x92,0xC4,0x9C,0x00,0x00,0xEA,0x49,0x22
	,0xB3,0xA5,0xD1,0x75,0x3B,0xC8,0xC6,0x6C,0xED,0xE3,0xC1,0xDC,0xBE,0x75,0xD1,0x49,0x10,0xFA,0x82,0x8A
	,0xD8,0x3E,0xE0,0xD6,0x86,0xD1,0x79,0xE2,0xDB,0x28,0x4F,0x29,0x6E,0xAF,0x70,0xDE,0x9C,0x0D,0xA3,0xF5
	,0x7C,0xFF,0x00,0xC0,0x6B,0x51,0xF5,0x18,0xBF,0xB4,0x6E,0x2C,0xCA,0x30,0x92,0x18,0x16,0x72,0x55,0x49
	,0xDC,0xAC,0x58,0x70,0x07,0x24,0x8D,0x9F,0xA8,0xAE,0xBC,0x4E,0x2A,0x70,0xFD,0xDC,0x76,0xEA,0x79,0xF8
	,0x0C,0xBE,0x9E,0x22,0xF5,0xAA,0x5F,0x7D,0x3A,0x6C,0x61,0x8F,0x0F,0x5C,0x85,0x4F,0xB6,0x35,0x8E,0xA8
	,0x42,0x8D,0xCF,0x77,0x68,0x0B,0x86,0xC7,0x38,0x61,0xD4,0x7B,0x11,0x9F,0x73,0x57,0xAD,0xB4,0xFB,0xC1
	,0x6E,0xB0,0x0B,0xB8,0x6D,0x60,0x4E,0x16,0x3B,0x2B,0x61,0x1E,0xD1,0xE9,0xF3,0x16,0x1F,0xA0,0xAD,0x60
	,0x77,0x28,0x23,0x3C,0x8C,0xF2,0x31,0x4B,0x5E,0x5D,0x8F,0xA3,0x54,0xE2,0xB6,0x38,0xE8,0xF4,0x8D,0x53
	,0x4E,0x8A,0x42,0xF6,0xBF,0x6B,0x25,0xD8,0xB4,0xB1,0x4D,0xBA,0x49,0x06,0x4E,0x09,0x0D,0x8C,0x9C,0x63
	,0x8C,0x9F,0x6A,0xD7,0xD1,0x2E,0x23,0xB9,0xB0,0x12,0x44,0xDB,0x91,0x8E,0xE5,0x3E,0xC6,0xB4,0xAE,0x6F
	,0x60,0xB4,0x89,0x24,0x99,0x8A,0xAB,0xCA,0x90,0xAE,0x47,0x56,0x66,0x0A,0x07,0xE6,0x45,0x60,0x78,0x74
	,0x7D,0x9A,0xF3,0x56,0xB0,0x3F,0xF2,0xC2,0xF2,0x4C,0x0F,0x40,0xE7,0xCC,0x5F,0xD2,0x40,0x3F,0x0A,0xF5
	,0x30,0xB8,0x89,0xCE,0x2E,0x9C,0xB6,0x4B,0x43,0xE7,0x73,0x0C,0x05,0x3A,0x0D,0x56,0xA7,0x7D,0x5E,0xBF
	,0x33,0xFF,0xD9
};
struct UPnPDataObject;

// It should not be necessary to expose/modify any of these structures. They are used by the internal stack
struct SubscriberInfo
{
	char* SID;		// Subscription ID
	int SIDLength;
	int SEQ;
	
	int Address;
	unsigned short Port;
	char* Path;
	int PathLength;
	int RefCount;
	int Disposing;
	
	struct timeval RenewByTime;
	
	struct SubscriberInfo *Next;
	struct SubscriberInfo *Previous;
};

struct UPnPDataObject
{
	// Absolutely DO NOT put anything above these 3 function pointers
	ILibChain_PreSelect PreSelect;
	ILibChain_PostSelect PostSelect;
	ILibChain_Destroy Destroy;
	
	void *EventClient;
	void *Chain;
	int UpdateFlag;
	
	// Network Poll
	unsigned int NetworkPollTime;
	
	int ForceExit;
	char *UUID;
	char *UDN;
	char *Serial;
	void *User;
	void *User2;
	
	void *WebServerTimer;
	void *HTTPServer;
	int InitialNotify;
	char* AVTransport_LastChange;
	char* ConnectionManager_SourceProtocolInfo;
	char* ConnectionManager_SinkProtocolInfo;
	char* ConnectionManager_CurrentConnectionIDs;
	char* RenderingControl_LastChange;
	
	struct sockaddr_in addr;
	int addrlen;
	
	// Current local interfaces
	struct sockaddr_in* AddressListV4;
	int AddressListV4Length;
	struct sockaddr_in6* AddressListV6;
	int AddressListV6Length;
	
	// Multicast Addresses
	struct sockaddr_in MulticastAddrV4;
	struct sockaddr_in6 MulticastAddrV6SL;
	struct sockaddr_in6 MulticastAddrV6LL;
	
	int _NumEmbeddedDevices;
	int WebSocketPortNumber;
	
	void **NOTIFY_RECEIVE_socks;
	void **NOTIFY_SEND_socks;
	void **NOTIFY_RECEIVE_socks6;
	void **NOTIFY_SEND_socks6;
	struct timeval CurrentTime;
	struct timeval NotifyTime;
	
	int SID;
	int NotifyCycleTime;
	sem_t EventLock;
	struct SubscriberInfo *HeadSubscriberPtr_AVTransport;
	int NumberOfSubscribers_AVTransport;
	struct SubscriberInfo *HeadSubscriberPtr_ConnectionManager;
	int NumberOfSubscribers_ConnectionManager;
	struct SubscriberInfo *HeadSubscriberPtr_RenderingControl;
	int NumberOfSubscribers_RenderingControl;
	
};

struct MSEARCH_state
{
	char *ST;
	int STLength;
	void *upnp;
	struct sockaddr_in6 dest_addr;
	struct sockaddr_in6 localIPAddress;
	void *Chain;
	void *SubChain;
};
struct UPnPFragmentNotifyStruct
{
	struct UPnPDataObject *upnp;
	int packetNumber;
};

/* Pre-declarations */
void UPnPStreamDescriptionDocument(struct ILibWebServer_Session *session);
void UPnPFragmentedSendNotify(void *data);
void UPnPSendNotify(const struct UPnPDataObject *upnp);
void UPnPSendByeBye(const struct UPnPDataObject *upnp);
void UPnPMainInvokeSwitch();
void UPnPSendDataXmlEscaped(const void* UPnPToken, const char* Data, const int DataLength, const int Terminate);
void UPnPSendData(const void* UPnPToken, const char* Data, const int DataLength, const int Terminate);
int UPnPPeriodicNotify(struct UPnPDataObject *upnp);
void UPnPSendEvent_Body(void *upnptoken, char *body, int bodylength, struct SubscriberInfo *info);
void UPnPProcessMSEARCH(struct UPnPDataObject *upnp, struct packetheader *packet);
struct in_addr UPnP_inaddr;

/*! \fn UPnPGetWebServerToken(const UPnPMicroStackToken MicroStackToken)
\brief Converts a MicroStackToken to a WebServerToken
\par
\a MicroStackToken is the void* returned from a call to UPnPCreateMicroStack. The returned token, is the server token
not the session token.
\param MicroStackToken MicroStack Token
\returns WebServer Token
*/
void* UPnPGetWebServerToken(const UPnPMicroStackToken MicroStackToken)
{
	return(((struct UPnPDataObject*)MicroStackToken)->HTTPServer);
}
int UPnPBuildSendSsdpResponsePacket(void* module, const struct UPnPDataObject *upnp, struct sockaddr* local, struct sockaddr* target, int EmbeddedDeviceNumber, char* USNex, char* ST, char* NTex)
{
	int len;
	UNREFERENCED_PARAMETER( EmbeddedDeviceNumber );
	
	if (local->sa_family == AF_INET)
	{
		// IPv4 address format
		ILibInet_ntop(local->sa_family, &(((struct sockaddr_in*)local)->sin_addr), ILibScratchPad2, sizeof(ILibScratchPad2));
	}
	else if (local->sa_family == AF_INET6)
	{
		// IPv6 address format
		size_t len2;
		ILibScratchPad2[0] = '[';
		ILibInet_ntop(local->sa_family, &(((struct sockaddr_in6*)local)->sin6_addr), ILibScratchPad2 + 1, sizeof(ILibScratchPad2) - 2);
		len2 = strlen(ILibScratchPad2);
		ILibScratchPad2[len2] = ']';
		ILibScratchPad2[len2 + 1] = 0;
	}
	
	len = snprintf(ILibScratchPad, sizeof(ILibScratchPad), "HTTP/1.1 200 OK\r\nLOCATION: http://%s:%d%s\r\nEXT:\r\nSERVER: %s, UPnP/1.0, MicroStack/1.0.5329\r\nUSN: uuid:%s%s\r\nCACHE-CONTROL: max-age=%d\r\nST: %s%s\r\n\r\n", ILibScratchPad2, upnp->WebSocketPortNumber, UPnPXmlLocation, UPnPPLATFORM, upnp->UDN, USNex, upnp->NotifyCycleTime, ST, NTex);
	return ILibAsyncUDPSocket_SendTo(module, target, ILibScratchPad, len, ILibAsyncSocket_MemoryOwnership_USER);
}

int UPnPBuildSendSsdpNotifyPacket(void* module, const struct UPnPDataObject *upnp, struct sockaddr* local, int EmbeddedDeviceNumber, char* USNex, char* NT, char* NTex)
{
	int len;
	struct sockaddr* multicast = NULL;
	char* mcaststr = NULL;
	UNREFERENCED_PARAMETER( EmbeddedDeviceNumber );
	
	if (local->sa_family == AF_INET)
	{
		// IPv4 address format
		ILibInet_ntop(local->sa_family, &(((struct sockaddr_in*)local)->sin_addr), ILibScratchPad2, sizeof(ILibScratchPad2));
		multicast = (struct sockaddr*)&(upnp->MulticastAddrV4);
		mcaststr = UPNP_MCASTv4_GROUP;
	}
	else if (local->sa_family == AF_INET6)
	{
		// IPv6 address format
		size_t len;
		ILibScratchPad2[0] = '[';
		ILibInet_ntop(local->sa_family, &(((struct sockaddr_in6*)local)->sin6_addr), ILibScratchPad2 + 1, sizeof(ILibScratchPad2) - 2);
		len = strlen(ILibScratchPad2);
		ILibScratchPad2[len] = ']';
		ILibScratchPad2[len + 1] = 0;
		if (ILibAsyncSocket_IsIPv6LinkLocal(local))
		{
			multicast = (struct sockaddr*)&(upnp->MulticastAddrV6LL);
			mcaststr = UPNP_MCASTv6_LINK_GROUPB;
		}
		else
		{
			multicast = (struct sockaddr*)&(upnp->MulticastAddrV6SL);
			mcaststr = UPNP_MCASTv6_GROUPB;
		}
	}
	else return 0;
	
	len = snprintf(ILibScratchPad, sizeof(ILibScratchPad), "NOTIFY * HTTP/1.1\r\nLOCATION: http://%s:%d%s\r\nHOST: %s:1900\r\nSERVER: %s, UPnP/1.0, MicroStack/1.0.5329\r\nNTS: ssdp:alive\r\nUSN: uuid:%s%s\r\nCACHE-CONTROL: max-age=%d\r\nNT: %s%s\r\n\r\n", ILibScratchPad2, upnp->WebSocketPortNumber, UPnPXmlLocation, mcaststr, UPnPPLATFORM, upnp->UDN, USNex, upnp->NotifyCycleTime, NT, NTex);
	return ILibAsyncUDPSocket_SendTo(module, multicast, ILibScratchPad, len, ILibAsyncSocket_MemoryOwnership_USER);
}
void UPnPSetDisconnectFlag(UPnPSessionToken token,void *flag)
{
	((struct ILibWebServer_Session*)token)->Reserved10=flag;
}

/*! \fn UPnPIPAddressListChanged(UPnPMicroStackToken MicroStackToken)
\brief Tell the underlying MicroStack that an IPAddress may have changed
\param MicroStackToken Microstack
*/
void UPnPIPAddressListChanged(UPnPMicroStackToken MicroStackToken)
{
	((struct UPnPDataObject*)MicroStackToken)->UpdateFlag = 1;
	ILibForceUnBlockChain(((struct UPnPDataObject*)MicroStackToken)->Chain);
}

void UPnPSSDPSink(ILibAsyncUDPSocket_SocketModule socketModule, char* buffer, int bufferLength, struct sockaddr_in6 *remoteInterface, void *user, void *user2, int *PAUSE)
{
	struct packetheader *packet;
	UNREFERENCED_PARAMETER( user2 );
	UNREFERENCED_PARAMETER( PAUSE );
	
	packet = ILibParsePacketHeader(buffer, 0, bufferLength);
	if (packet != NULL)
	{
		// Fill in the source and local interface addresses
		memcpy(&(packet->Source), remoteInterface, INET_SOCKADDR_LENGTH(remoteInterface->sin6_family));
		ILibAsyncUDPSocket_GetLocalInterface(socketModule, (struct sockaddr*)&(packet->ReceivingAddress));
		
		if (packet->StatusCode == -1 && memcmp(packet->Directive, "M-SEARCH", 8) == 0 && ((struct sockaddr_in6*)(packet->ReceivingAddress))->sin6_family != 0)
		{
			// Process the search request with our Multicast M-SEARCH Handler
			UPnPProcessMSEARCH(user, packet);
		}
		ILibDestructPacket(packet);
	}
}
//
//	Internal underlying Initialization, that shouldn't be called explicitely
// 
// <param name="state">State object</param>
// <param name="NotifyCycleSeconds">Cycle duration</param>
// <param name="PortNumber">Port Number</param>
void UPnPInit(struct UPnPDataObject *state, void *chain, const int NotifyCycleSeconds, const unsigned short PortNumber)
{
	int i;
	struct sockaddr_in any4;
	struct sockaddr_in6 any6;
	UNREFERENCED_PARAMETER( PortNumber );
	
	// Setup ANY addresses
	memset(&any4, 0, sizeof(struct sockaddr_in));
	any4.sin_family = AF_INET;
	any4.sin_port = htons(UPNP_PORT);
	memset(&any6, 0, sizeof(struct sockaddr_in6));
	any6.sin6_family = AF_INET6;
	any6.sin6_port = htons(UPNP_PORT);
	
	state->Chain = chain;
	
	// Setup notification timer
	state->NotifyCycleTime = NotifyCycleSeconds;
	gettimeofday(&(state->CurrentTime), NULL);
	(state->NotifyTime).tv_sec = (state->CurrentTime).tv_sec  + (state->NotifyCycleTime / 2);
	
	// Fetch the list of local IPv4 interfaces
	state->AddressListV4Length = ILibGetLocalIPv4AddressList(&(state->AddressListV4), 1);
	
	// Setup the IPv4 sockets
	if ((state->NOTIFY_SEND_socks = (void**)malloc(sizeof(void*)*(state->AddressListV4Length))) == NULL) ILIBCRITICALEXIT(254);
	if ((state->NOTIFY_RECEIVE_socks = (void**)malloc(sizeof(void*)*(state->AddressListV4Length))) == NULL) ILIBCRITICALEXIT(254);
	
	// Setup multicast IPv4 address
	memset(&state->MulticastAddrV4, 0, sizeof(struct sockaddr_in));
	state->MulticastAddrV4.sin_family = AF_INET;
	state->MulticastAddrV4.sin_port = htons(UPNP_PORT);
	ILibInet_pton(AF_INET, UPNP_MCASTv4_GROUP, &(state->MulticastAddrV4.sin_addr));
	
	// Test IPv6 support
	if (ILibDetectIPv6Support())
	{
		// Fetch the list of local IPv6 interfaces
		state->AddressListV6Length = ILibGetLocalIPv6List(&(state->AddressListV6));
		
		// Setup the IPv6 sockets
		if ((state->NOTIFY_SEND_socks6 = (void**)malloc(sizeof(void*)*(state->AddressListV6Length))) == NULL) ILIBCRITICALEXIT(254);
		if ((state->NOTIFY_RECEIVE_socks6 = (void**)malloc(sizeof(void*)*(state->AddressListV6Length))) == NULL) ILIBCRITICALEXIT(254);
		
		// Setup multicast IPv6 address (Site Local)
		memset(&state->MulticastAddrV6SL, 0, sizeof(struct sockaddr_in6));
		state->MulticastAddrV6SL.sin6_family = AF_INET6;
		state->MulticastAddrV6SL.sin6_port = htons(UPNP_PORT);
		ILibInet_pton(AF_INET6, UPNP_MCASTv6_GROUP, &(state->MulticastAddrV6SL.sin6_addr));
		
		// Setup multicast IPv6 address (Link Local)
		memset(&state->MulticastAddrV6LL, 0, sizeof(struct sockaddr_in6));
		state->MulticastAddrV6LL.sin6_family = AF_INET6;
		state->MulticastAddrV6LL.sin6_port = htons(UPNP_PORT);
		ILibInet_pton(AF_INET6, UPNP_MCASTv6_LINK_GROUP, &(state->MulticastAddrV6LL.sin6_addr));
	}
	
	// Iterate through all the current IPv4 addresses
	for (i = 0; i < state->AddressListV4Length; ++i)
	{
		(state->AddressListV4[i]).sin_port = 0; // Bind to ANY port for outbound packets
		state->NOTIFY_SEND_socks[i] = ILibAsyncUDPSocket_CreateEx(
		chain,
		UPNP_MAX_SSDP_HEADER_SIZE,
		(struct sockaddr*)&(state->AddressListV4[i]),
		ILibAsyncUDPSocket_Reuse_SHARED,
		NULL,
		NULL,
		state);
		
		ILibAsyncUDPSocket_SetMulticastTTL(state->NOTIFY_SEND_socks[i], UPNP_SSDP_TTL);
		ILibAsyncUDPSocket_SetMulticastLoopback(state->NOTIFY_SEND_socks[i], 1);
		
		(state->AddressListV4[i]).sin_port = htons(UPNP_PORT); // Bind to UPnP port for inbound packets
		state->NOTIFY_RECEIVE_socks[i] = ILibAsyncUDPSocket_CreateEx(
		state->Chain,
		UPNP_MAX_SSDP_HEADER_SIZE,
		(struct sockaddr*)&any4,
		ILibAsyncUDPSocket_Reuse_SHARED,
		&UPnPSSDPSink,
		NULL,
		state);
		
		if (!state->NOTIFY_RECEIVE_socks[i]) continue;	// when ip removed causes an error if continued

		ILibAsyncUDPSocket_JoinMulticastGroupV4(state->NOTIFY_RECEIVE_socks[i], (struct sockaddr_in*)&(state->MulticastAddrV4), (struct sockaddr*)&(state->AddressListV4[i]));
		ILibAsyncUDPSocket_SetLocalInterface(state->NOTIFY_RECEIVE_socks[i], (struct sockaddr*)&(state->AddressListV4[i]));
		ILibAsyncUDPSocket_SetMulticastLoopback(state->NOTIFY_RECEIVE_socks[i], 1);
	}
	
	if (state->AddressListV6Length > 0)
	{
		// Iterate through all the current IPv6 interfaces
		for (i = 0; i < state->AddressListV6Length; ++i)
		{
			(state->AddressListV6[i]).sin6_port = 0;
			state->NOTIFY_SEND_socks6[i] = ILibAsyncUDPSocket_CreateEx(
			chain,
			UPNP_MAX_SSDP_HEADER_SIZE,
			(struct sockaddr*)&(state->AddressListV6[i]),
			ILibAsyncUDPSocket_Reuse_SHARED,
			NULL,
			NULL,
			state);
			
			ILibAsyncUDPSocket_SetMulticastTTL(state->NOTIFY_SEND_socks6[i], UPNP_SSDP_TTL);
			ILibAsyncUDPSocket_SetMulticastLoopback(state->NOTIFY_SEND_socks6[i], 1);
			
			(state->AddressListV6[i]).sin6_port = htons(UPNP_PORT); // Bind to UPnP port for inbound packets
			state->NOTIFY_RECEIVE_socks6[i] = ILibAsyncUDPSocket_CreateEx(
			state->Chain,
			UPNP_MAX_SSDP_HEADER_SIZE,
			(struct sockaddr*)&any6,
			ILibAsyncUDPSocket_Reuse_SHARED,
			&UPnPSSDPSink,
			NULL,
			state);
			
			if (ILibAsyncSocket_IsIPv6LinkLocal((struct sockaddr*)&(state->AddressListV6[i])))
			{
				ILibAsyncUDPSocket_JoinMulticastGroupV6(state->NOTIFY_RECEIVE_socks6[i], &(state->MulticastAddrV6LL), state->AddressListV6[i].sin6_scope_id);
			}
			else
			{
				ILibAsyncUDPSocket_JoinMulticastGroupV6(state->NOTIFY_RECEIVE_socks6[i], &(state->MulticastAddrV6SL), state->AddressListV6[i].sin6_scope_id);
			}
			ILibAsyncUDPSocket_SetMulticastLoopback(state->NOTIFY_RECEIVE_socks6[i], 1);
			ILibAsyncUDPSocket_SetLocalInterface(state->NOTIFY_RECEIVE_socks6[i], (struct sockaddr*)&(state->AddressListV6[i]));
		}
	}
}

void UPnPPostMX_Destroy(void *object)
{
	struct MSEARCH_state *mss = (struct MSEARCH_state*)object;
	free(mss->ST);
	free(mss);
}
void UPnPOnPostMX_MSEARCH_SendOK(ILibAsyncUDPSocket_SocketModule socketModule, void *user1, void *user2)
{
	struct MSEARCH_state *mss = (struct MSEARCH_state*)user1;
	UNREFERENCED_PARAMETER( socketModule );
	UNREFERENCED_PARAMETER( user2 );
	
	ILibChain_SafeRemove_SubChain(mss->Chain, mss->SubChain);
	free(mss->ST);
	free(mss);
}

void UPnPPostMX_MSEARCH(void *object)
{
	struct MSEARCH_state *mss = (struct MSEARCH_state*)object;
	void *response_socket;
	void *subChain;
	char *ST = mss->ST;
	int STLength = mss->STLength;
	struct UPnPDataObject *upnp = (struct UPnPDataObject*)mss->upnp;
	int rcode = 0;
	
	subChain = ILibCreateChain();
	
	response_socket = ILibAsyncUDPSocket_CreateEx(
	subChain,
	UPNP_MAX_SSDP_HEADER_SIZE,
	(struct sockaddr*)&(mss->localIPAddress),
	ILibAsyncUDPSocket_Reuse_SHARED,
	NULL,
	UPnPOnPostMX_MSEARCH_SendOK,
	mss);
	
	ILibChain_SafeAdd_SubChain(mss->Chain, subChain);
	mss->SubChain = subChain;
	
	// Search for root device
	if (STLength == 15 && memcmp(ST, "upnp:rootdevice", 15) == 0)
	{
		UPnPBuildSendSsdpResponsePacket(response_socket, upnp, (struct sockaddr*)&(mss->localIPAddress), (struct sockaddr*)&(mss->dest_addr), 0, "::upnp:rootdevice", "upnp:rootdevice", "");
	}
	// Search for everything
	else if (STLength == 8 && memcmp(ST, "ssdp:all", 8) == 0)
	{
		rcode = UPnPBuildSendSsdpResponsePacket(response_socket, upnp, (struct sockaddr*)&(mss->localIPAddress), (struct sockaddr*)&(mss->dest_addr), 0, "::upnp:rootdevice", "upnp:rootdevice", "");
		rcode = UPnPBuildSendSsdpResponsePacket(response_socket, upnp, (struct sockaddr*)&(mss->localIPAddress), (struct sockaddr*)&(mss->dest_addr), 0, "", upnp->UUID, "");
		rcode = UPnPBuildSendSsdpResponsePacket(response_socket, upnp, (struct sockaddr*)&(mss->localIPAddress), (struct sockaddr*)&(mss->dest_addr), 0, "::urn:schemas-upnp-org:device:MediaRenderer:1", "urn:schemas-upnp-org:device:MediaRenderer:1", "");
		rcode = UPnPBuildSendSsdpResponsePacket(response_socket, upnp, (struct sockaddr*)&(mss->localIPAddress), (struct sockaddr*)&(mss->dest_addr), 0, "::urn:schemas-upnp-org:service:AVTransport:1", "urn:schemas-upnp-org:service:AVTransport:1", "");
		rcode = UPnPBuildSendSsdpResponsePacket(response_socket, upnp, (struct sockaddr*)&(mss->localIPAddress), (struct sockaddr*)&(mss->dest_addr), 0, "::urn:schemas-upnp-org:service:ConnectionManager:1", "urn:schemas-upnp-org:service:ConnectionManager:1", "");
		rcode = UPnPBuildSendSsdpResponsePacket(response_socket, upnp, (struct sockaddr*)&(mss->localIPAddress), (struct sockaddr*)&(mss->dest_addr), 0, "::urn:schemas-upnp-org:service:RenderingControl:1", "urn:schemas-upnp-org:service:RenderingControl:1", "");
		
	}
	else if (STLength == (int)strlen(upnp->UUID) && memcmp(ST,upnp->UUID,(int)strlen(upnp->UUID))==0)
	{
		rcode = UPnPBuildSendSsdpResponsePacket(response_socket, upnp, (struct sockaddr*)&(mss->localIPAddress), (struct sockaddr*)&(mss->dest_addr), 0,"",upnp->UUID,"");
	}
	else if (STLength >= 42 && memcmp(ST,"urn:schemas-upnp-org:device:MediaRenderer:",42)==0 && atoi(ST+42)<=1)
	{
		rcode = UPnPBuildSendSsdpResponsePacket(response_socket, upnp, (struct sockaddr*)&(mss->localIPAddress), (struct sockaddr*)&(mss->dest_addr), 0, "::urn:schemas-upnp-org:device:MediaRenderer:1", ST, "");
	}
	else if (STLength >= 41 && memcmp(ST,"urn:schemas-upnp-org:service:AVTransport:",41)==0 && atoi(ST+41)<=1)
	{
		rcode = UPnPBuildSendSsdpResponsePacket(response_socket, upnp, (struct sockaddr*)&(mss->localIPAddress), (struct sockaddr*)&(mss->dest_addr), 0, "::urn:schemas-upnp-org:service:AVTransport:1", ST, "");
	}
	else if (STLength >= 47 && memcmp(ST,"urn:schemas-upnp-org:service:ConnectionManager:",47)==0 && atoi(ST+47)<=1)
	{
		rcode = UPnPBuildSendSsdpResponsePacket(response_socket, upnp, (struct sockaddr*)&(mss->localIPAddress), (struct sockaddr*)&(mss->dest_addr), 0, "::urn:schemas-upnp-org:service:ConnectionManager:1", ST, "");
	}
	else if (STLength >= 46 && memcmp(ST,"urn:schemas-upnp-org:service:RenderingControl:",46)==0 && atoi(ST+46)<=1)
	{
		rcode = UPnPBuildSendSsdpResponsePacket(response_socket, upnp, (struct sockaddr*)&(mss->localIPAddress), (struct sockaddr*)&(mss->dest_addr), 0, "::urn:schemas-upnp-org:service:RenderingControl:1", ST, "");
	}
	
	
	if (rcode == 0)
	{
		ILibChain_SafeRemove_SubChain(mss->Chain, subChain);
		free(mss->ST);
		free(mss);
	}
}

void UPnPProcessMSEARCH(struct UPnPDataObject *upnp, struct packetheader *packet)
{
	char* ST = NULL;
	unsigned long MXVal = 0;
	int STLength = 0, MANOK = 0, MXOK = 0, MX;
	struct packetheader_field_node *node;
	struct MSEARCH_state *mss = NULL;
	
	if (memcmp(packet->DirectiveObj, "*", 1)==0)
	{
		if (memcmp(packet->Version, "1.1", 3)==0)
		{
			node = packet->FirstField;
			while(node!=NULL)
			{
				if (node->FieldLength==2 && strncasecmp(node->Field,"ST",2)==0)
				{
					// This is what is being searched for
					if ((ST = (char*)malloc(1 + node->FieldDataLength)) == NULL) ILIBCRITICALEXIT(254);
					memcpy(ST, node->FieldData, node->FieldDataLength);
					ST[node->FieldDataLength] = 0;
					STLength = node->FieldDataLength;
				}
				else if (node->FieldLength == 3 && strncasecmp(node->Field, "MAN", 3) == 0 && memcmp(node->FieldData, "\"ssdp:discover\"", 15) == 0)
				{
					// This is a required header field
					MANOK = 1;
				}
				else if (node->FieldLength == 2 && strncasecmp(node->Field, "MX", 2) == 0 && ILibGetULong(node->FieldData, node->FieldDataLength, &MXVal) == 0)
				{
					// If the timeout value specified is greater than 10 seconds, just force it down to 10 seconds
					MXOK = 1;
					MXVal = MXVal>10?10:MXVal;
				}
				node = node->NextField;
			}
			if (MANOK != 0 && MXOK != 0)
			{
				if (MXVal == 0)
				{
					MX = 0;
				}
				else
				{
					// The timeout value should be a random number between 0 and the specified value
					MX = (int)(0 + ((unsigned short)rand() % MXVal));
				}
				if ((mss = (struct MSEARCH_state*)malloc(sizeof(struct MSEARCH_state))) == NULL) ILIBCRITICALEXIT(254);
				mss->ST = ST;
				mss->STLength = STLength;
				mss->upnp = upnp;
				memcpy(&(mss->dest_addr), &(packet->Source), sizeof(struct sockaddr_in6));
				memcpy(&(mss->localIPAddress), &(packet->ReceivingAddress), sizeof(struct sockaddr_in6));
				mss->Chain = upnp->Chain;
				
				// Register for a timed callback, so we can respond later
				ILibLifeTime_Add(upnp->WebServerTimer, mss, MX, &UPnPPostMX_MSEARCH, &UPnPPostMX_Destroy);
			}
			else
			{
				free(ST);
			}
		}
	}
}

void UPnPDispatch_AVTransport_GetCurrentTransportActions(char *buffer, int offset, int bufferLength, struct ILibWebServer_Session *ReaderObject)
{
	unsigned long TempULong;
	int OK = 0;
	char *p_InstanceID = NULL;
	int p_InstanceIDLength = 0;
	unsigned int _InstanceID = 0;
	struct ILibXMLNode *xnode = ILibParseXML(buffer, offset, bufferLength);
	struct ILibXMLNode *root = xnode;
	if (ILibProcessXMLNodeList(root)!=0)
	{
		/* The XML is not well formed! */
		ILibDestructXMLNodeList(root);
		UPnPResponse_Error(ReaderObject, 501, "Invalid XML");
		return;
	}
	while(xnode != NULL)
	{
		if (xnode->StartTag != 0 && xnode->NameLength == 8 && memcmp(xnode->Name, "Envelope", 8)==0)
		{
			// Envelope
			xnode = xnode->Next;
			while(xnode != NULL)
			{
				if (xnode->StartTag!=0 && xnode->NameLength == 4 && memcmp(xnode->Name, "Body", 4) == 0)
				{
					// Body
					xnode = xnode->Next;
					while(xnode != NULL)
					{
						if (xnode->StartTag != 0 && xnode->NameLength == 26 && memcmp(xnode->Name, "GetCurrentTransportActions",26) == 0)
						{
							// Inside the interesting part of the SOAP
							xnode = xnode->Next;
							while(xnode != NULL)
							{
								if (xnode->NameLength == 10 && memcmp(xnode->Name, "InstanceID",10)==0)
								{
									p_InstanceIDLength = ILibReadInnerXML(xnode, &p_InstanceID);
									OK |= 1;
								}
								if (xnode->Peer == NULL)
								{
									xnode = xnode->Parent;
									break;
								}
								else
								{
									xnode = xnode->Peer;
								}
							}
						}
						if (xnode != NULL)
						{
							if (xnode->Peer == NULL)
							{
								xnode = xnode->Parent;
								break;
							}
							else
							{
								xnode = xnode->Peer;
							}
						}
					}
				}
				if (xnode != NULL)
				{
					if (xnode->Peer == NULL)
					{
						xnode = xnode->Parent;
						break;
					}
					else
					{
						xnode = xnode->Peer;
					}
				}
			}
		}
		if (xnode != NULL){xnode = xnode->Peer;}
	}
	ILibDestructXMLNodeList(root);
	if (OK != 1)
	{
		UPnPResponse_Error(ReaderObject, 402, "Illegal value");
		return;
	}
	
	/* Type Checking */
	OK = ILibGetULong(p_InstanceID,p_InstanceIDLength, &TempULong);
	if (OK!=0)
	{
		UPnPResponse_Error(ReaderObject,402,"Illegal value");
		return;
	}
	_InstanceID = (unsigned int)TempULong;
	if (UPnPFP_AVTransport_GetCurrentTransportActions == NULL)
	UPnPResponse_Error(ReaderObject,501,"No Function Handler");
	else
	UPnPFP_AVTransport_GetCurrentTransportActions((void*)ReaderObject,_InstanceID);
}

void UPnPDispatch_AVTransport_GetDeviceCapabilities(char *buffer, int offset, int bufferLength, struct ILibWebServer_Session *ReaderObject)
{
	unsigned long TempULong;
	int OK = 0;
	char *p_InstanceID = NULL;
	int p_InstanceIDLength = 0;
	unsigned int _InstanceID = 0;
	struct ILibXMLNode *xnode = ILibParseXML(buffer, offset, bufferLength);
	struct ILibXMLNode *root = xnode;
	if (ILibProcessXMLNodeList(root)!=0)
	{
		/* The XML is not well formed! */
		ILibDestructXMLNodeList(root);
		UPnPResponse_Error(ReaderObject, 501, "Invalid XML");
		return;
	}
	while(xnode != NULL)
	{
		if (xnode->StartTag != 0 && xnode->NameLength == 8 && memcmp(xnode->Name, "Envelope", 8)==0)
		{
			// Envelope
			xnode = xnode->Next;
			while(xnode != NULL)
			{
				if (xnode->StartTag!=0 && xnode->NameLength == 4 && memcmp(xnode->Name, "Body", 4) == 0)
				{
					// Body
					xnode = xnode->Next;
					while(xnode != NULL)
					{
						if (xnode->StartTag != 0 && xnode->NameLength == 21 && memcmp(xnode->Name, "GetDeviceCapabilities",21) == 0)
						{
							// Inside the interesting part of the SOAP
							xnode = xnode->Next;
							while(xnode != NULL)
							{
								if (xnode->NameLength == 10 && memcmp(xnode->Name, "InstanceID",10)==0)
								{
									p_InstanceIDLength = ILibReadInnerXML(xnode, &p_InstanceID);
									OK |= 1;
								}
								if (xnode->Peer == NULL)
								{
									xnode = xnode->Parent;
									break;
								}
								else
								{
									xnode = xnode->Peer;
								}
							}
						}
						if (xnode != NULL)
						{
							if (xnode->Peer == NULL)
							{
								xnode = xnode->Parent;
								break;
							}
							else
							{
								xnode = xnode->Peer;
							}
						}
					}
				}
				if (xnode != NULL)
				{
					if (xnode->Peer == NULL)
					{
						xnode = xnode->Parent;
						break;
					}
					else
					{
						xnode = xnode->Peer;
					}
				}
			}
		}
		if (xnode != NULL){xnode = xnode->Peer;}
	}
	ILibDestructXMLNodeList(root);
	if (OK != 1)
	{
		UPnPResponse_Error(ReaderObject, 402, "Illegal value");
		return;
	}
	
	/* Type Checking */
	OK = ILibGetULong(p_InstanceID,p_InstanceIDLength, &TempULong);
	if (OK!=0)
	{
		UPnPResponse_Error(ReaderObject,402,"Illegal value");
		return;
	}
	_InstanceID = (unsigned int)TempULong;
	if (UPnPFP_AVTransport_GetDeviceCapabilities == NULL)
	UPnPResponse_Error(ReaderObject,501,"No Function Handler");
	else
	UPnPFP_AVTransport_GetDeviceCapabilities((void*)ReaderObject,_InstanceID);
}

void UPnPDispatch_AVTransport_GetMediaInfo(char *buffer, int offset, int bufferLength, struct ILibWebServer_Session *ReaderObject)
{
	unsigned long TempULong;
	int OK = 0;
	char *p_InstanceID = NULL;
	int p_InstanceIDLength = 0;
	unsigned int _InstanceID = 0;
	struct ILibXMLNode *xnode = ILibParseXML(buffer, offset, bufferLength);
	struct ILibXMLNode *root = xnode;
	if (ILibProcessXMLNodeList(root)!=0)
	{
		/* The XML is not well formed! */
		ILibDestructXMLNodeList(root);
		UPnPResponse_Error(ReaderObject, 501, "Invalid XML");
		return;
	}
	while(xnode != NULL)
	{
		if (xnode->StartTag != 0 && xnode->NameLength == 8 && memcmp(xnode->Name, "Envelope", 8)==0)
		{
			// Envelope
			xnode = xnode->Next;
			while(xnode != NULL)
			{
				if (xnode->StartTag!=0 && xnode->NameLength == 4 && memcmp(xnode->Name, "Body", 4) == 0)
				{
					// Body
					xnode = xnode->Next;
					while(xnode != NULL)
					{
						if (xnode->StartTag != 0 && xnode->NameLength == 12 && memcmp(xnode->Name, "GetMediaInfo",12) == 0)
						{
							// Inside the interesting part of the SOAP
							xnode = xnode->Next;
							while(xnode != NULL)
							{
								if (xnode->NameLength == 10 && memcmp(xnode->Name, "InstanceID",10)==0)
								{
									p_InstanceIDLength = ILibReadInnerXML(xnode, &p_InstanceID);
									OK |= 1;
								}
								if (xnode->Peer == NULL)
								{
									xnode = xnode->Parent;
									break;
								}
								else
								{
									xnode = xnode->Peer;
								}
							}
						}
						if (xnode != NULL)
						{
							if (xnode->Peer == NULL)
							{
								xnode = xnode->Parent;
								break;
							}
							else
							{
								xnode = xnode->Peer;
							}
						}
					}
				}
				if (xnode != NULL)
				{
					if (xnode->Peer == NULL)
					{
						xnode = xnode->Parent;
						break;
					}
					else
					{
						xnode = xnode->Peer;
					}
				}
			}
		}
		if (xnode != NULL){xnode = xnode->Peer;}
	}
	ILibDestructXMLNodeList(root);
	if (OK != 1)
	{
		UPnPResponse_Error(ReaderObject, 402, "Illegal value");
		return;
	}
	
	/* Type Checking */
	OK = ILibGetULong(p_InstanceID,p_InstanceIDLength, &TempULong);
	if (OK!=0)
	{
		UPnPResponse_Error(ReaderObject,402,"Illegal value");
		return;
	}
	_InstanceID = (unsigned int)TempULong;
	if (UPnPFP_AVTransport_GetMediaInfo == NULL)
	UPnPResponse_Error(ReaderObject,501,"No Function Handler");
	else
	UPnPFP_AVTransport_GetMediaInfo((void*)ReaderObject,_InstanceID);
}

void UPnPDispatch_AVTransport_GetPositionInfo(char *buffer, int offset, int bufferLength, struct ILibWebServer_Session *ReaderObject)
{
	unsigned long TempULong;
	int OK = 0;
	char *p_InstanceID = NULL;
	int p_InstanceIDLength = 0;
	unsigned int _InstanceID = 0;
	struct ILibXMLNode *xnode = ILibParseXML(buffer, offset, bufferLength);
	struct ILibXMLNode *root = xnode;
	if (ILibProcessXMLNodeList(root)!=0)
	{
		/* The XML is not well formed! */
		ILibDestructXMLNodeList(root);
		UPnPResponse_Error(ReaderObject, 501, "Invalid XML");
		return;
	}
	while(xnode != NULL)
	{
		if (xnode->StartTag != 0 && xnode->NameLength == 8 && memcmp(xnode->Name, "Envelope", 8)==0)
		{
			// Envelope
			xnode = xnode->Next;
			while(xnode != NULL)
			{
				if (xnode->StartTag!=0 && xnode->NameLength == 4 && memcmp(xnode->Name, "Body", 4) == 0)
				{
					// Body
					xnode = xnode->Next;
					while(xnode != NULL)
					{
						if (xnode->StartTag != 0 && xnode->NameLength == 15 && memcmp(xnode->Name, "GetPositionInfo",15) == 0)
						{
							// Inside the interesting part of the SOAP
							xnode = xnode->Next;
							while(xnode != NULL)
							{
								if (xnode->NameLength == 10 && memcmp(xnode->Name, "InstanceID",10)==0)
								{
									p_InstanceIDLength = ILibReadInnerXML(xnode, &p_InstanceID);
									OK |= 1;
								}
								if (xnode->Peer == NULL)
								{
									xnode = xnode->Parent;
									break;
								}
								else
								{
									xnode = xnode->Peer;
								}
							}
						}
						if (xnode != NULL)
						{
							if (xnode->Peer == NULL)
							{
								xnode = xnode->Parent;
								break;
							}
							else
							{
								xnode = xnode->Peer;
							}
						}
					}
				}
				if (xnode != NULL)
				{
					if (xnode->Peer == NULL)
					{
						xnode = xnode->Parent;
						break;
					}
					else
					{
						xnode = xnode->Peer;
					}
				}
			}
		}
		if (xnode != NULL){xnode = xnode->Peer;}
	}
	ILibDestructXMLNodeList(root);
	if (OK != 1)
	{
		UPnPResponse_Error(ReaderObject, 402, "Illegal value");
		return;
	}
	
	/* Type Checking */
	OK = ILibGetULong(p_InstanceID,p_InstanceIDLength, &TempULong);
	if (OK!=0)
	{
		UPnPResponse_Error(ReaderObject,402,"Illegal value");
		return;
	}
	_InstanceID = (unsigned int)TempULong;
	if (UPnPFP_AVTransport_GetPositionInfo == NULL)
	UPnPResponse_Error(ReaderObject,501,"No Function Handler");
	else
	UPnPFP_AVTransport_GetPositionInfo((void*)ReaderObject,_InstanceID);
}

void UPnPDispatch_AVTransport_GetTransportInfo(char *buffer, int offset, int bufferLength, struct ILibWebServer_Session *ReaderObject)
{
	unsigned long TempULong;
	int OK = 0;
	char *p_InstanceID = NULL;
	int p_InstanceIDLength = 0;
	unsigned int _InstanceID = 0;
	struct ILibXMLNode *xnode = ILibParseXML(buffer, offset, bufferLength);
	struct ILibXMLNode *root = xnode;
	if (ILibProcessXMLNodeList(root)!=0)
	{
		/* The XML is not well formed! */
		ILibDestructXMLNodeList(root);
		UPnPResponse_Error(ReaderObject, 501, "Invalid XML");
		return;
	}
	while(xnode != NULL)
	{
		if (xnode->StartTag != 0 && xnode->NameLength == 8 && memcmp(xnode->Name, "Envelope", 8)==0)
		{
			// Envelope
			xnode = xnode->Next;
			while(xnode != NULL)
			{
				if (xnode->StartTag!=0 && xnode->NameLength == 4 && memcmp(xnode->Name, "Body", 4) == 0)
				{
					// Body
					xnode = xnode->Next;
					while(xnode != NULL)
					{
						if (xnode->StartTag != 0 && xnode->NameLength == 16 && memcmp(xnode->Name, "GetTransportInfo",16) == 0)
						{
							// Inside the interesting part of the SOAP
							xnode = xnode->Next;
							while(xnode != NULL)
							{
								if (xnode->NameLength == 10 && memcmp(xnode->Name, "InstanceID",10)==0)
								{
									p_InstanceIDLength = ILibReadInnerXML(xnode, &p_InstanceID);
									OK |= 1;
								}
								if (xnode->Peer == NULL)
								{
									xnode = xnode->Parent;
									break;
								}
								else
								{
									xnode = xnode->Peer;
								}
							}
						}
						if (xnode != NULL)
						{
							if (xnode->Peer == NULL)
							{
								xnode = xnode->Parent;
								break;
							}
							else
							{
								xnode = xnode->Peer;
							}
						}
					}
				}
				if (xnode != NULL)
				{
					if (xnode->Peer == NULL)
					{
						xnode = xnode->Parent;
						break;
					}
					else
					{
						xnode = xnode->Peer;
					}
				}
			}
		}
		if (xnode != NULL){xnode = xnode->Peer;}
	}
	ILibDestructXMLNodeList(root);
	if (OK != 1)
	{
		UPnPResponse_Error(ReaderObject, 402, "Illegal value");
		return;
	}
	
	/* Type Checking */
	OK = ILibGetULong(p_InstanceID,p_InstanceIDLength, &TempULong);
	if (OK!=0)
	{
		UPnPResponse_Error(ReaderObject,402,"Illegal value");
		return;
	}
	_InstanceID = (unsigned int)TempULong;
	if (UPnPFP_AVTransport_GetTransportInfo == NULL)
	UPnPResponse_Error(ReaderObject,501,"No Function Handler");
	else
	UPnPFP_AVTransport_GetTransportInfo((void*)ReaderObject,_InstanceID);
}

void UPnPDispatch_AVTransport_GetTransportSettings(char *buffer, int offset, int bufferLength, struct ILibWebServer_Session *ReaderObject)
{
	unsigned long TempULong;
	int OK = 0;
	char *p_InstanceID = NULL;
	int p_InstanceIDLength = 0;
	unsigned int _InstanceID = 0;
	struct ILibXMLNode *xnode = ILibParseXML(buffer, offset, bufferLength);
	struct ILibXMLNode *root = xnode;
	if (ILibProcessXMLNodeList(root)!=0)
	{
		/* The XML is not well formed! */
		ILibDestructXMLNodeList(root);
		UPnPResponse_Error(ReaderObject, 501, "Invalid XML");
		return;
	}
	while(xnode != NULL)
	{
		if (xnode->StartTag != 0 && xnode->NameLength == 8 && memcmp(xnode->Name, "Envelope", 8)==0)
		{
			// Envelope
			xnode = xnode->Next;
			while(xnode != NULL)
			{
				if (xnode->StartTag!=0 && xnode->NameLength == 4 && memcmp(xnode->Name, "Body", 4) == 0)
				{
					// Body
					xnode = xnode->Next;
					while(xnode != NULL)
					{
						if (xnode->StartTag != 0 && xnode->NameLength == 20 && memcmp(xnode->Name, "GetTransportSettings",20) == 0)
						{
							// Inside the interesting part of the SOAP
							xnode = xnode->Next;
							while(xnode != NULL)
							{
								if (xnode->NameLength == 10 && memcmp(xnode->Name, "InstanceID",10)==0)
								{
									p_InstanceIDLength = ILibReadInnerXML(xnode, &p_InstanceID);
									OK |= 1;
								}
								if (xnode->Peer == NULL)
								{
									xnode = xnode->Parent;
									break;
								}
								else
								{
									xnode = xnode->Peer;
								}
							}
						}
						if (xnode != NULL)
						{
							if (xnode->Peer == NULL)
							{
								xnode = xnode->Parent;
								break;
							}
							else
							{
								xnode = xnode->Peer;
							}
						}
					}
				}
				if (xnode != NULL)
				{
					if (xnode->Peer == NULL)
					{
						xnode = xnode->Parent;
						break;
					}
					else
					{
						xnode = xnode->Peer;
					}
				}
			}
		}
		if (xnode != NULL){xnode = xnode->Peer;}
	}
	ILibDestructXMLNodeList(root);
	if (OK != 1)
	{
		UPnPResponse_Error(ReaderObject, 402, "Illegal value");
		return;
	}
	
	/* Type Checking */
	OK = ILibGetULong(p_InstanceID,p_InstanceIDLength, &TempULong);
	if (OK!=0)
	{
		UPnPResponse_Error(ReaderObject,402,"Illegal value");
		return;
	}
	_InstanceID = (unsigned int)TempULong;
	if (UPnPFP_AVTransport_GetTransportSettings == NULL)
	UPnPResponse_Error(ReaderObject,501,"No Function Handler");
	else
	UPnPFP_AVTransport_GetTransportSettings((void*)ReaderObject,_InstanceID);
}

void UPnPDispatch_AVTransport_Next(char *buffer, int offset, int bufferLength, struct ILibWebServer_Session *ReaderObject)
{
	unsigned long TempULong;
	int OK = 0;
	char *p_InstanceID = NULL;
	int p_InstanceIDLength = 0;
	unsigned int _InstanceID = 0;
	struct ILibXMLNode *xnode = ILibParseXML(buffer, offset, bufferLength);
	struct ILibXMLNode *root = xnode;
	if (ILibProcessXMLNodeList(root)!=0)
	{
		/* The XML is not well formed! */
		ILibDestructXMLNodeList(root);
		UPnPResponse_Error(ReaderObject, 501, "Invalid XML");
		return;
	}
	while(xnode != NULL)
	{
		if (xnode->StartTag != 0 && xnode->NameLength == 8 && memcmp(xnode->Name, "Envelope", 8)==0)
		{
			// Envelope
			xnode = xnode->Next;
			while(xnode != NULL)
			{
				if (xnode->StartTag!=0 && xnode->NameLength == 4 && memcmp(xnode->Name, "Body", 4) == 0)
				{
					// Body
					xnode = xnode->Next;
					while(xnode != NULL)
					{
						if (xnode->StartTag != 0 && xnode->NameLength == 4 && memcmp(xnode->Name, "Next",4) == 0)
						{
							// Inside the interesting part of the SOAP
							xnode = xnode->Next;
							while(xnode != NULL)
							{
								if (xnode->NameLength == 10 && memcmp(xnode->Name, "InstanceID",10)==0)
								{
									p_InstanceIDLength = ILibReadInnerXML(xnode, &p_InstanceID);
									OK |= 1;
								}
								if (xnode->Peer == NULL)
								{
									xnode = xnode->Parent;
									break;
								}
								else
								{
									xnode = xnode->Peer;
								}
							}
						}
						if (xnode != NULL)
						{
							if (xnode->Peer == NULL)
							{
								xnode = xnode->Parent;
								break;
							}
							else
							{
								xnode = xnode->Peer;
							}
						}
					}
				}
				if (xnode != NULL)
				{
					if (xnode->Peer == NULL)
					{
						xnode = xnode->Parent;
						break;
					}
					else
					{
						xnode = xnode->Peer;
					}
				}
			}
		}
		if (xnode != NULL){xnode = xnode->Peer;}
	}
	ILibDestructXMLNodeList(root);
	if (OK != 1)
	{
		UPnPResponse_Error(ReaderObject, 402, "Illegal value");
		return;
	}
	
	/* Type Checking */
	OK = ILibGetULong(p_InstanceID,p_InstanceIDLength, &TempULong);
	if (OK!=0)
	{
		UPnPResponse_Error(ReaderObject,402,"Illegal value");
		return;
	}
	_InstanceID = (unsigned int)TempULong;
	if (UPnPFP_AVTransport_Next == NULL)
	UPnPResponse_Error(ReaderObject,501,"No Function Handler");
	else
	UPnPFP_AVTransport_Next((void*)ReaderObject,_InstanceID);
}

void UPnPDispatch_AVTransport_Pause(char *buffer, int offset, int bufferLength, struct ILibWebServer_Session *ReaderObject)
{
	unsigned long TempULong;
	int OK = 0;
	char *p_InstanceID = NULL;
	int p_InstanceIDLength = 0;
	unsigned int _InstanceID = 0;
	struct ILibXMLNode *xnode = ILibParseXML(buffer, offset, bufferLength);
	struct ILibXMLNode *root = xnode;
	if (ILibProcessXMLNodeList(root)!=0)
	{
		/* The XML is not well formed! */
		ILibDestructXMLNodeList(root);
		UPnPResponse_Error(ReaderObject, 501, "Invalid XML");
		return;
	}
	while(xnode != NULL)
	{
		if (xnode->StartTag != 0 && xnode->NameLength == 8 && memcmp(xnode->Name, "Envelope", 8)==0)
		{
			// Envelope
			xnode = xnode->Next;
			while(xnode != NULL)
			{
				if (xnode->StartTag!=0 && xnode->NameLength == 4 && memcmp(xnode->Name, "Body", 4) == 0)
				{
					// Body
					xnode = xnode->Next;
					while(xnode != NULL)
					{
						if (xnode->StartTag != 0 && xnode->NameLength == 5 && memcmp(xnode->Name, "Pause",5) == 0)
						{
							// Inside the interesting part of the SOAP
							xnode = xnode->Next;
							while(xnode != NULL)
							{
								if (xnode->NameLength == 10 && memcmp(xnode->Name, "InstanceID",10)==0)
								{
									p_InstanceIDLength = ILibReadInnerXML(xnode, &p_InstanceID);
									OK |= 1;
								}
								if (xnode->Peer == NULL)
								{
									xnode = xnode->Parent;
									break;
								}
								else
								{
									xnode = xnode->Peer;
								}
							}
						}
						if (xnode != NULL)
						{
							if (xnode->Peer == NULL)
							{
								xnode = xnode->Parent;
								break;
							}
							else
							{
								xnode = xnode->Peer;
							}
						}
					}
				}
				if (xnode != NULL)
				{
					if (xnode->Peer == NULL)
					{
						xnode = xnode->Parent;
						break;
					}
					else
					{
						xnode = xnode->Peer;
					}
				}
			}
		}
		if (xnode != NULL){xnode = xnode->Peer;}
	}
	ILibDestructXMLNodeList(root);
	if (OK != 1)
	{
		UPnPResponse_Error(ReaderObject, 402, "Illegal value");
		return;
	}
	
	/* Type Checking */
	OK = ILibGetULong(p_InstanceID,p_InstanceIDLength, &TempULong);
	if (OK!=0)
	{
		UPnPResponse_Error(ReaderObject,402,"Illegal value");
		return;
	}
	_InstanceID = (unsigned int)TempULong;
	if (UPnPFP_AVTransport_Pause == NULL)
	UPnPResponse_Error(ReaderObject,501,"No Function Handler");
	else
	UPnPFP_AVTransport_Pause((void*)ReaderObject,_InstanceID);
}

void UPnPDispatch_AVTransport_Play(char *buffer, int offset, int bufferLength, struct ILibWebServer_Session *ReaderObject)
{
	unsigned long TempULong;
	int OK = 0;
	char *p_InstanceID = NULL;
	int p_InstanceIDLength = 0;
	unsigned int _InstanceID = 0;
	char *p_Speed = NULL;
	int p_SpeedLength = 0;
	char* _Speed = "";
	int _SpeedLength;
	struct ILibXMLNode *xnode = ILibParseXML(buffer, offset, bufferLength);
	struct ILibXMLNode *root = xnode;
	if (ILibProcessXMLNodeList(root)!=0)
	{
		/* The XML is not well formed! */
		ILibDestructXMLNodeList(root);
		UPnPResponse_Error(ReaderObject, 501, "Invalid XML");
		return;
	}
	while(xnode != NULL)
	{
		if (xnode->StartTag != 0 && xnode->NameLength == 8 && memcmp(xnode->Name, "Envelope", 8)==0)
		{
			// Envelope
			xnode = xnode->Next;
			while(xnode != NULL)
			{
				if (xnode->StartTag!=0 && xnode->NameLength == 4 && memcmp(xnode->Name, "Body", 4) == 0)
				{
					// Body
					xnode = xnode->Next;
					while(xnode != NULL)
					{
						if (xnode->StartTag != 0 && xnode->NameLength == 4 && memcmp(xnode->Name, "Play",4) == 0)
						{
							// Inside the interesting part of the SOAP
							xnode = xnode->Next;
							while(xnode != NULL)
							{
								if (xnode->NameLength == 10 && memcmp(xnode->Name, "InstanceID",10)==0)
								{
									p_InstanceIDLength = ILibReadInnerXML(xnode, &p_InstanceID);
									OK |= 1;
								}
								else if (xnode->NameLength == 5 && memcmp(xnode->Name, "Speed",5)==0)
								{
									p_SpeedLength = ILibReadInnerXML(xnode, &p_Speed);
									p_Speed[p_SpeedLength]=0;
									OK |= 2;
								}
								if (xnode->Peer == NULL)
								{
									xnode = xnode->Parent;
									break;
								}
								else
								{
									xnode = xnode->Peer;
								}
							}
						}
						if (xnode != NULL)
						{
							if (xnode->Peer == NULL)
							{
								xnode = xnode->Parent;
								break;
							}
							else
							{
								xnode = xnode->Peer;
							}
						}
					}
				}
				if (xnode != NULL)
				{
					if (xnode->Peer == NULL)
					{
						xnode = xnode->Parent;
						break;
					}
					else
					{
						xnode = xnode->Peer;
					}
				}
			}
		}
		if (xnode != NULL){xnode = xnode->Peer;}
	}
	ILibDestructXMLNodeList(root);
	if (OK != 3)
	{
		UPnPResponse_Error(ReaderObject, 402, "Illegal value");
		return;
	}
	
	/* Type Checking */
	OK = ILibGetULong(p_InstanceID,p_InstanceIDLength, &TempULong);
	if (OK!=0)
	{
		UPnPResponse_Error(ReaderObject,402,"Illegal value");
		return;
	}
	_InstanceID = (unsigned int)TempULong;
	_SpeedLength = ILibInPlaceXmlUnEscape(p_Speed);
	_Speed = p_Speed;
	for(OK=0;OK<UPnP_StateVariable_AllowedValues_MAX;++OK)
	{
		if (UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_TransportPlaySpeed->AllowedValues[OK]!=NULL)
		{
			if (strcmp(_Speed,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_TransportPlaySpeed->AllowedValues[OK])==0)
			{
				OK=0;
				break;
			}
		}
		else
		{
			break;
		}
	}
	if (OK!=0)
	{
		UPnPResponse_Error(ReaderObject,402,"Illegal value");
		return;
	}
	if (UPnPFP_AVTransport_Play == NULL)
	UPnPResponse_Error(ReaderObject,501,"No Function Handler");
	else
	UPnPFP_AVTransport_Play((void*)ReaderObject,_InstanceID,_Speed);
}

void UPnPDispatch_AVTransport_Previous(char *buffer, int offset, int bufferLength, struct ILibWebServer_Session *ReaderObject)
{
	unsigned long TempULong;
	int OK = 0;
	char *p_InstanceID = NULL;
	int p_InstanceIDLength = 0;
	unsigned int _InstanceID = 0;
	struct ILibXMLNode *xnode = ILibParseXML(buffer, offset, bufferLength);
	struct ILibXMLNode *root = xnode;
	if (ILibProcessXMLNodeList(root)!=0)
	{
		/* The XML is not well formed! */
		ILibDestructXMLNodeList(root);
		UPnPResponse_Error(ReaderObject, 501, "Invalid XML");
		return;
	}
	while(xnode != NULL)
	{
		if (xnode->StartTag != 0 && xnode->NameLength == 8 && memcmp(xnode->Name, "Envelope", 8)==0)
		{
			// Envelope
			xnode = xnode->Next;
			while(xnode != NULL)
			{
				if (xnode->StartTag!=0 && xnode->NameLength == 4 && memcmp(xnode->Name, "Body", 4) == 0)
				{
					// Body
					xnode = xnode->Next;
					while(xnode != NULL)
					{
						if (xnode->StartTag != 0 && xnode->NameLength == 8 && memcmp(xnode->Name, "Previous",8) == 0)
						{
							// Inside the interesting part of the SOAP
							xnode = xnode->Next;
							while(xnode != NULL)
							{
								if (xnode->NameLength == 10 && memcmp(xnode->Name, "InstanceID",10)==0)
								{
									p_InstanceIDLength = ILibReadInnerXML(xnode, &p_InstanceID);
									OK |= 1;
								}
								if (xnode->Peer == NULL)
								{
									xnode = xnode->Parent;
									break;
								}
								else
								{
									xnode = xnode->Peer;
								}
							}
						}
						if (xnode != NULL)
						{
							if (xnode->Peer == NULL)
							{
								xnode = xnode->Parent;
								break;
							}
							else
							{
								xnode = xnode->Peer;
							}
						}
					}
				}
				if (xnode != NULL)
				{
					if (xnode->Peer == NULL)
					{
						xnode = xnode->Parent;
						break;
					}
					else
					{
						xnode = xnode->Peer;
					}
				}
			}
		}
		if (xnode != NULL){xnode = xnode->Peer;}
	}
	ILibDestructXMLNodeList(root);
	if (OK != 1)
	{
		UPnPResponse_Error(ReaderObject, 402, "Illegal value");
		return;
	}
	
	/* Type Checking */
	OK = ILibGetULong(p_InstanceID,p_InstanceIDLength, &TempULong);
	if (OK!=0)
	{
		UPnPResponse_Error(ReaderObject,402,"Illegal value");
		return;
	}
	_InstanceID = (unsigned int)TempULong;
	if (UPnPFP_AVTransport_Previous == NULL)
	UPnPResponse_Error(ReaderObject,501,"No Function Handler");
	else
	UPnPFP_AVTransport_Previous((void*)ReaderObject,_InstanceID);
}

void UPnPDispatch_AVTransport_Seek(char *buffer, int offset, int bufferLength, struct ILibWebServer_Session *ReaderObject)
{
	unsigned long TempULong;
	int OK = 0;
	char *p_InstanceID = NULL;
	int p_InstanceIDLength = 0;
	unsigned int _InstanceID = 0;
	char *p_Unit = NULL;
	int p_UnitLength = 0;
	char* _Unit = "";
	int _UnitLength;
	char *p_Target = NULL;
	int p_TargetLength = 0;
	char* _Target = "";
	int _TargetLength;
	struct ILibXMLNode *xnode = ILibParseXML(buffer, offset, bufferLength);
	struct ILibXMLNode *root = xnode;
	if (ILibProcessXMLNodeList(root)!=0)
	{
		/* The XML is not well formed! */
		ILibDestructXMLNodeList(root);
		UPnPResponse_Error(ReaderObject, 501, "Invalid XML");
		return;
	}
	while(xnode != NULL)
	{
		if (xnode->StartTag != 0 && xnode->NameLength == 8 && memcmp(xnode->Name, "Envelope", 8)==0)
		{
			// Envelope
			xnode = xnode->Next;
			while(xnode != NULL)
			{
				if (xnode->StartTag!=0 && xnode->NameLength == 4 && memcmp(xnode->Name, "Body", 4) == 0)
				{
					// Body
					xnode = xnode->Next;
					while(xnode != NULL)
					{
						if (xnode->StartTag != 0 && xnode->NameLength == 4 && memcmp(xnode->Name, "Seek",4) == 0)
						{
							// Inside the interesting part of the SOAP
							xnode = xnode->Next;
							while(xnode != NULL)
							{
								if (xnode->NameLength == 10 && memcmp(xnode->Name, "InstanceID",10)==0)
								{
									p_InstanceIDLength = ILibReadInnerXML(xnode, &p_InstanceID);
									OK |= 1;
								}
								else if (xnode->NameLength == 4 && memcmp(xnode->Name, "Unit",4)==0)
								{
									p_UnitLength = ILibReadInnerXML(xnode, &p_Unit);
									p_Unit[p_UnitLength]=0;
									OK |= 2;
								}
								else if (xnode->NameLength == 6 && memcmp(xnode->Name, "Target",6)==0)
								{
									p_TargetLength = ILibReadInnerXML(xnode, &p_Target);
									p_Target[p_TargetLength]=0;
									OK |= 4;
								}
								if (xnode->Peer == NULL)
								{
									xnode = xnode->Parent;
									break;
								}
								else
								{
									xnode = xnode->Peer;
								}
							}
						}
						if (xnode != NULL)
						{
							if (xnode->Peer == NULL)
							{
								xnode = xnode->Parent;
								break;
							}
							else
							{
								xnode = xnode->Peer;
							}
						}
					}
				}
				if (xnode != NULL)
				{
					if (xnode->Peer == NULL)
					{
						xnode = xnode->Parent;
						break;
					}
					else
					{
						xnode = xnode->Peer;
					}
				}
			}
		}
		if (xnode != NULL){xnode = xnode->Peer;}
	}
	ILibDestructXMLNodeList(root);
	if (OK != 7)
	{
		UPnPResponse_Error(ReaderObject, 402, "Illegal value");
		return;
	}
	
	/* Type Checking */
	OK = ILibGetULong(p_InstanceID,p_InstanceIDLength, &TempULong);
	if (OK!=0)
	{
		UPnPResponse_Error(ReaderObject,402,"Illegal value");
		return;
	}
	_InstanceID = (unsigned int)TempULong;
	_UnitLength = ILibInPlaceXmlUnEscape(p_Unit);
	_Unit = p_Unit;
	for(OK=0;OK<UPnP_StateVariable_AllowedValues_MAX;++OK)
	{
		if (UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_A_ARG_TYPE_SeekMode->AllowedValues[OK]!=NULL)
		{
			if (strcmp(_Unit,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_A_ARG_TYPE_SeekMode->AllowedValues[OK])==0)
			{
				OK=0;
				break;
			}
		}
		else
		{
			break;
		}
	}
	if (OK!=0)
	{
		UPnPResponse_Error(ReaderObject,402,"Illegal value");
		return;
	}
	_TargetLength = ILibInPlaceXmlUnEscape(p_Target);
	_Target = p_Target;
	if (UPnPFP_AVTransport_Seek == NULL)
	UPnPResponse_Error(ReaderObject,501,"No Function Handler");
	else
	UPnPFP_AVTransport_Seek((void*)ReaderObject,_InstanceID,_Unit,_Target);
}

void UPnPDispatch_AVTransport_SetAVTransportURI(char *buffer, int offset, int bufferLength, struct ILibWebServer_Session *ReaderObject)
{
	unsigned long TempULong;
	int OK = 0;
	char *p_InstanceID = NULL;
	int p_InstanceIDLength = 0;
	unsigned int _InstanceID = 0;
	char *p_CurrentURI = NULL;
	int p_CurrentURILength = 0;
	char* _CurrentURI = "";
	int _CurrentURILength;
	char *p_CurrentURIMetaData = NULL;
	int p_CurrentURIMetaDataLength = 0;
	char* _CurrentURIMetaData = "";
	int _CurrentURIMetaDataLength;
	struct ILibXMLNode *xnode = ILibParseXML(buffer, offset, bufferLength);
	struct ILibXMLNode *root = xnode;
	if (ILibProcessXMLNodeList(root)!=0)
	{
		/* The XML is not well formed! */
		ILibDestructXMLNodeList(root);
		UPnPResponse_Error(ReaderObject, 501, "Invalid XML");
		return;
	}
	while(xnode != NULL)
	{
		if (xnode->StartTag != 0 && xnode->NameLength == 8 && memcmp(xnode->Name, "Envelope", 8)==0)
		{
			// Envelope
			xnode = xnode->Next;
			while(xnode != NULL)
			{
				if (xnode->StartTag!=0 && xnode->NameLength == 4 && memcmp(xnode->Name, "Body", 4) == 0)
				{
					// Body
					xnode = xnode->Next;
					while(xnode != NULL)
					{
						if (xnode->StartTag != 0 && xnode->NameLength == 17 && memcmp(xnode->Name, "SetAVTransportURI",17) == 0)
						{
							// Inside the interesting part of the SOAP
							xnode = xnode->Next;
							while(xnode != NULL)
							{
								if (xnode->NameLength == 10 && memcmp(xnode->Name, "InstanceID",10)==0)
								{
									p_InstanceIDLength = ILibReadInnerXML(xnode, &p_InstanceID);
									OK |= 1;
								}
								else if (xnode->NameLength == 10 && memcmp(xnode->Name, "CurrentURI",10)==0)
								{
									p_CurrentURILength = ILibReadInnerXML(xnode, &p_CurrentURI);
									p_CurrentURI[p_CurrentURILength]=0;
									OK |= 2;
								}
								else if (xnode->NameLength == 18 && memcmp(xnode->Name, "CurrentURIMetaData",18)==0)
								{
									p_CurrentURIMetaDataLength = ILibReadInnerXML(xnode, &p_CurrentURIMetaData);
									p_CurrentURIMetaData[p_CurrentURIMetaDataLength]=0;
									OK |= 4;
								}
								if (xnode->Peer == NULL)
								{
									xnode = xnode->Parent;
									break;
								}
								else
								{
									xnode = xnode->Peer;
								}
							}
						}
						if (xnode != NULL)
						{
							if (xnode->Peer == NULL)
							{
								xnode = xnode->Parent;
								break;
							}
							else
							{
								xnode = xnode->Peer;
							}
						}
					}
				}
				if (xnode != NULL)
				{
					if (xnode->Peer == NULL)
					{
						xnode = xnode->Parent;
						break;
					}
					else
					{
						xnode = xnode->Peer;
					}
				}
			}
		}
		if (xnode != NULL){xnode = xnode->Peer;}
	}
	ILibDestructXMLNodeList(root);
	if (OK != 7)
	{
		UPnPResponse_Error(ReaderObject, 402, "Illegal value");
		return;
	}
	
	/* Type Checking */
	OK = ILibGetULong(p_InstanceID,p_InstanceIDLength, &TempULong);
	if (OK!=0)
	{
		UPnPResponse_Error(ReaderObject,402,"Illegal value");
		return;
	}
	_InstanceID = (unsigned int)TempULong;
	_CurrentURILength = ILibInPlaceXmlUnEscape(p_CurrentURI);
	_CurrentURI = p_CurrentURI;
	_CurrentURIMetaDataLength = ILibInPlaceXmlUnEscape(p_CurrentURIMetaData);
	_CurrentURIMetaData = p_CurrentURIMetaData;
	if (UPnPFP_AVTransport_SetAVTransportURI == NULL)
	UPnPResponse_Error(ReaderObject,501,"No Function Handler");
	else
	UPnPFP_AVTransport_SetAVTransportURI((void*)ReaderObject,_InstanceID,_CurrentURI,_CurrentURIMetaData);
}

void UPnPDispatch_AVTransport_SetPlayMode(char *buffer, int offset, int bufferLength, struct ILibWebServer_Session *ReaderObject)
{
	unsigned long TempULong;
	int OK = 0;
	char *p_InstanceID = NULL;
	int p_InstanceIDLength = 0;
	unsigned int _InstanceID = 0;
	char *p_NewPlayMode = NULL;
	int p_NewPlayModeLength = 0;
	char* _NewPlayMode = "";
	int _NewPlayModeLength;
	struct ILibXMLNode *xnode = ILibParseXML(buffer, offset, bufferLength);
	struct ILibXMLNode *root = xnode;
	if (ILibProcessXMLNodeList(root)!=0)
	{
		/* The XML is not well formed! */
		ILibDestructXMLNodeList(root);
		UPnPResponse_Error(ReaderObject, 501, "Invalid XML");
		return;
	}
	while(xnode != NULL)
	{
		if (xnode->StartTag != 0 && xnode->NameLength == 8 && memcmp(xnode->Name, "Envelope", 8)==0)
		{
			// Envelope
			xnode = xnode->Next;
			while(xnode != NULL)
			{
				if (xnode->StartTag!=0 && xnode->NameLength == 4 && memcmp(xnode->Name, "Body", 4) == 0)
				{
					// Body
					xnode = xnode->Next;
					while(xnode != NULL)
					{
						if (xnode->StartTag != 0 && xnode->NameLength == 11 && memcmp(xnode->Name, "SetPlayMode",11) == 0)
						{
							// Inside the interesting part of the SOAP
							xnode = xnode->Next;
							while(xnode != NULL)
							{
								if (xnode->NameLength == 10 && memcmp(xnode->Name, "InstanceID",10)==0)
								{
									p_InstanceIDLength = ILibReadInnerXML(xnode, &p_InstanceID);
									OK |= 1;
								}
								else if (xnode->NameLength == 11 && memcmp(xnode->Name, "NewPlayMode",11)==0)
								{
									p_NewPlayModeLength = ILibReadInnerXML(xnode, &p_NewPlayMode);
									p_NewPlayMode[p_NewPlayModeLength]=0;
									OK |= 2;
								}
								if (xnode->Peer == NULL)
								{
									xnode = xnode->Parent;
									break;
								}
								else
								{
									xnode = xnode->Peer;
								}
							}
						}
						if (xnode != NULL)
						{
							if (xnode->Peer == NULL)
							{
								xnode = xnode->Parent;
								break;
							}
							else
							{
								xnode = xnode->Peer;
							}
						}
					}
				}
				if (xnode != NULL)
				{
					if (xnode->Peer == NULL)
					{
						xnode = xnode->Parent;
						break;
					}
					else
					{
						xnode = xnode->Peer;
					}
				}
			}
		}
		if (xnode != NULL){xnode = xnode->Peer;}
	}
	ILibDestructXMLNodeList(root);
	if (OK != 3)
	{
		UPnPResponse_Error(ReaderObject, 402, "Illegal value");
		return;
	}
	
	/* Type Checking */
	OK = ILibGetULong(p_InstanceID,p_InstanceIDLength, &TempULong);
	if (OK!=0)
	{
		UPnPResponse_Error(ReaderObject,402,"Illegal value");
		return;
	}
	_InstanceID = (unsigned int)TempULong;
	_NewPlayModeLength = ILibInPlaceXmlUnEscape(p_NewPlayMode);
	_NewPlayMode = p_NewPlayMode;
	for(OK=0;OK<UPnP_StateVariable_AllowedValues_MAX;++OK)
	{
		if (UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_CurrentPlayMode->AllowedValues[OK]!=NULL)
		{
			if (strcmp(_NewPlayMode,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_CurrentPlayMode->AllowedValues[OK])==0)
			{
				OK=0;
				break;
			}
		}
		else
		{
			break;
		}
	}
	if (OK!=0)
	{
		UPnPResponse_Error(ReaderObject,402,"Illegal value");
		return;
	}
	if (UPnPFP_AVTransport_SetPlayMode == NULL)
	UPnPResponse_Error(ReaderObject,501,"No Function Handler");
	else
	UPnPFP_AVTransport_SetPlayMode((void*)ReaderObject,_InstanceID,_NewPlayMode);
}

void UPnPDispatch_AVTransport_Stop(char *buffer, int offset, int bufferLength, struct ILibWebServer_Session *ReaderObject)
{
	unsigned long TempULong;
	int OK = 0;
	char *p_InstanceID = NULL;
	int p_InstanceIDLength = 0;
	unsigned int _InstanceID = 0;
	struct ILibXMLNode *xnode = ILibParseXML(buffer, offset, bufferLength);
	struct ILibXMLNode *root = xnode;
	if (ILibProcessXMLNodeList(root)!=0)
	{
		/* The XML is not well formed! */
		ILibDestructXMLNodeList(root);
		UPnPResponse_Error(ReaderObject, 501, "Invalid XML");
		return;
	}
	while(xnode != NULL)
	{
		if (xnode->StartTag != 0 && xnode->NameLength == 8 && memcmp(xnode->Name, "Envelope", 8)==0)
		{
			// Envelope
			xnode = xnode->Next;
			while(xnode != NULL)
			{
				if (xnode->StartTag!=0 && xnode->NameLength == 4 && memcmp(xnode->Name, "Body", 4) == 0)
				{
					// Body
					xnode = xnode->Next;
					while(xnode != NULL)
					{
						if (xnode->StartTag != 0 && xnode->NameLength == 4 && memcmp(xnode->Name, "Stop",4) == 0)
						{
							// Inside the interesting part of the SOAP
							xnode = xnode->Next;
							while(xnode != NULL)
							{
								if (xnode->NameLength == 10 && memcmp(xnode->Name, "InstanceID",10)==0)
								{
									p_InstanceIDLength = ILibReadInnerXML(xnode, &p_InstanceID);
									OK |= 1;
								}
								if (xnode->Peer == NULL)
								{
									xnode = xnode->Parent;
									break;
								}
								else
								{
									xnode = xnode->Peer;
								}
							}
						}
						if (xnode != NULL)
						{
							if (xnode->Peer == NULL)
							{
								xnode = xnode->Parent;
								break;
							}
							else
							{
								xnode = xnode->Peer;
							}
						}
					}
				}
				if (xnode != NULL)
				{
					if (xnode->Peer == NULL)
					{
						xnode = xnode->Parent;
						break;
					}
					else
					{
						xnode = xnode->Peer;
					}
				}
			}
		}
		if (xnode != NULL){xnode = xnode->Peer;}
	}
	ILibDestructXMLNodeList(root);
	if (OK != 1)
	{
		UPnPResponse_Error(ReaderObject, 402, "Illegal value");
		return;
	}
	
	/* Type Checking */
	OK = ILibGetULong(p_InstanceID,p_InstanceIDLength, &TempULong);
	if (OK!=0)
	{
		UPnPResponse_Error(ReaderObject,402,"Illegal value");
		return;
	}
	_InstanceID = (unsigned int)TempULong;
	if (UPnPFP_AVTransport_Stop == NULL)
	UPnPResponse_Error(ReaderObject,501,"No Function Handler");
	else
	UPnPFP_AVTransport_Stop((void*)ReaderObject,_InstanceID);
}

#define UPnPDispatch_ConnectionManager_GetCurrentConnectionIDs(buffer,offset,bufferLength, session)\
{\
	if (UPnPFP_ConnectionManager_GetCurrentConnectionIDs == NULL)\
	UPnPResponse_Error(session,501,"No Function Handler");\
	else\
	UPnPFP_ConnectionManager_GetCurrentConnectionIDs((void*)session);\
}

void UPnPDispatch_ConnectionManager_GetCurrentConnectionInfo(char *buffer, int offset, int bufferLength, struct ILibWebServer_Session *ReaderObject)
{
	long TempLong;
	int OK = 0;
	char *p_ConnectionID = NULL;
	int p_ConnectionIDLength = 0;
	int _ConnectionID = 0;
	struct ILibXMLNode *xnode = ILibParseXML(buffer, offset, bufferLength);
	struct ILibXMLNode *root = xnode;
	if (ILibProcessXMLNodeList(root)!=0)
	{
		/* The XML is not well formed! */
		ILibDestructXMLNodeList(root);
		UPnPResponse_Error(ReaderObject, 501, "Invalid XML");
		return;
	}
	while(xnode != NULL)
	{
		if (xnode->StartTag != 0 && xnode->NameLength == 8 && memcmp(xnode->Name, "Envelope", 8)==0)
		{
			// Envelope
			xnode = xnode->Next;
			while(xnode != NULL)
			{
				if (xnode->StartTag!=0 && xnode->NameLength == 4 && memcmp(xnode->Name, "Body", 4) == 0)
				{
					// Body
					xnode = xnode->Next;
					while(xnode != NULL)
					{
						if (xnode->StartTag != 0 && xnode->NameLength == 24 && memcmp(xnode->Name, "GetCurrentConnectionInfo",24) == 0)
						{
							// Inside the interesting part of the SOAP
							xnode = xnode->Next;
							while(xnode != NULL)
							{
								if (xnode->NameLength == 12 && memcmp(xnode->Name, "ConnectionID",12)==0)
								{
									p_ConnectionIDLength = ILibReadInnerXML(xnode, &p_ConnectionID);
									OK |= 1;
								}
								if (xnode->Peer == NULL)
								{
									xnode = xnode->Parent;
									break;
								}
								else
								{
									xnode = xnode->Peer;
								}
							}
						}
						if (xnode != NULL)
						{
							if (xnode->Peer == NULL)
							{
								xnode = xnode->Parent;
								break;
							}
							else
							{
								xnode = xnode->Peer;
							}
						}
					}
				}
				if (xnode != NULL)
				{
					if (xnode->Peer == NULL)
					{
						xnode = xnode->Parent;
						break;
					}
					else
					{
						xnode = xnode->Peer;
					}
				}
			}
		}
		if (xnode != NULL){xnode = xnode->Peer;}
	}
	ILibDestructXMLNodeList(root);
	if (OK != 1)
	{
		UPnPResponse_Error(ReaderObject, 402, "Illegal value");
		return;
	}
	
	/* Type Checking */
	OK = ILibGetLong(p_ConnectionID,p_ConnectionIDLength, &TempLong);
	if (OK!=0)
	{
		UPnPResponse_Error(ReaderObject,402,"Illegal value");
		return;
	}
	_ConnectionID = (int)TempLong;
	if (UPnPFP_ConnectionManager_GetCurrentConnectionInfo == NULL)
	UPnPResponse_Error(ReaderObject,501,"No Function Handler");
	else
	UPnPFP_ConnectionManager_GetCurrentConnectionInfo((void*)ReaderObject,_ConnectionID);
}

#define UPnPDispatch_ConnectionManager_GetProtocolInfo(buffer,offset,bufferLength, session)\
{\
	if (UPnPFP_ConnectionManager_GetProtocolInfo == NULL)\
	UPnPResponse_Error(session,501,"No Function Handler");\
	else\
	UPnPFP_ConnectionManager_GetProtocolInfo((void*)session);\
}

void UPnPDispatch_RenderingControl_GetMute(char *buffer, int offset, int bufferLength, struct ILibWebServer_Session *ReaderObject)
{
	unsigned long TempULong;
	int OK = 0;
	char *p_InstanceID = NULL;
	int p_InstanceIDLength = 0;
	unsigned int _InstanceID = 0;
	char *p_Channel = NULL;
	int p_ChannelLength = 0;
	char* _Channel = "";
	int _ChannelLength;
	struct ILibXMLNode *xnode = ILibParseXML(buffer, offset, bufferLength);
	struct ILibXMLNode *root = xnode;
	if (ILibProcessXMLNodeList(root)!=0)
	{
		/* The XML is not well formed! */
		ILibDestructXMLNodeList(root);
		UPnPResponse_Error(ReaderObject, 501, "Invalid XML");
		return;
	}
	while(xnode != NULL)
	{
		if (xnode->StartTag != 0 && xnode->NameLength == 8 && memcmp(xnode->Name, "Envelope", 8)==0)
		{
			// Envelope
			xnode = xnode->Next;
			while(xnode != NULL)
			{
				if (xnode->StartTag!=0 && xnode->NameLength == 4 && memcmp(xnode->Name, "Body", 4) == 0)
				{
					// Body
					xnode = xnode->Next;
					while(xnode != NULL)
					{
						if (xnode->StartTag != 0 && xnode->NameLength == 7 && memcmp(xnode->Name, "GetMute",7) == 0)
						{
							// Inside the interesting part of the SOAP
							xnode = xnode->Next;
							while(xnode != NULL)
							{
								if (xnode->NameLength == 10 && memcmp(xnode->Name, "InstanceID",10)==0)
								{
									p_InstanceIDLength = ILibReadInnerXML(xnode, &p_InstanceID);
									OK |= 1;
								}
								else if (xnode->NameLength == 7 && memcmp(xnode->Name, "Channel",7)==0)
								{
									p_ChannelLength = ILibReadInnerXML(xnode, &p_Channel);
									p_Channel[p_ChannelLength]=0;
									OK |= 2;
								}
								if (xnode->Peer == NULL)
								{
									xnode = xnode->Parent;
									break;
								}
								else
								{
									xnode = xnode->Peer;
								}
							}
						}
						if (xnode != NULL)
						{
							if (xnode->Peer == NULL)
							{
								xnode = xnode->Parent;
								break;
							}
							else
							{
								xnode = xnode->Peer;
							}
						}
					}
				}
				if (xnode != NULL)
				{
					if (xnode->Peer == NULL)
					{
						xnode = xnode->Parent;
						break;
					}
					else
					{
						xnode = xnode->Peer;
					}
				}
			}
		}
		if (xnode != NULL){xnode = xnode->Peer;}
	}
	ILibDestructXMLNodeList(root);
	if (OK != 3)
	{
		UPnPResponse_Error(ReaderObject, 402, "Illegal value");
		return;
	}
	
	/* Type Checking */
	OK = ILibGetULong(p_InstanceID,p_InstanceIDLength, &TempULong);
	if (OK!=0)
	{
		UPnPResponse_Error(ReaderObject,402,"Illegal value");
		return;
	}
	_InstanceID = (unsigned int)TempULong;
	_ChannelLength = ILibInPlaceXmlUnEscape(p_Channel);
	_Channel = p_Channel;
	for(OK=0;OK<UPnP_StateVariable_AllowedValues_MAX;++OK)
	{
		if (UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_A_ARG_TYPE_Channel->AllowedValues[OK]!=NULL)
		{
			if (strcmp(_Channel,UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_A_ARG_TYPE_Channel->AllowedValues[OK])==0)
			{
				OK=0;
				break;
			}
		}
		else
		{
			break;
		}
	}
	if (OK!=0)
	{
		UPnPResponse_Error(ReaderObject,402,"Illegal value");
		return;
	}
	if (UPnPFP_RenderingControl_GetMute == NULL)
	UPnPResponse_Error(ReaderObject,501,"No Function Handler");
	else
	UPnPFP_RenderingControl_GetMute((void*)ReaderObject,_InstanceID,_Channel);
}

void UPnPDispatch_RenderingControl_GetVolume(char *buffer, int offset, int bufferLength, struct ILibWebServer_Session *ReaderObject)
{
	unsigned long TempULong;
	int OK = 0;
	char *p_InstanceID = NULL;
	int p_InstanceIDLength = 0;
	unsigned int _InstanceID = 0;
	char *p_Channel = NULL;
	int p_ChannelLength = 0;
	char* _Channel = "";
	int _ChannelLength;
	struct ILibXMLNode *xnode = ILibParseXML(buffer, offset, bufferLength);
	struct ILibXMLNode *root = xnode;
	if (ILibProcessXMLNodeList(root)!=0)
	{
		/* The XML is not well formed! */
		ILibDestructXMLNodeList(root);
		UPnPResponse_Error(ReaderObject, 501, "Invalid XML");
		return;
	}
	while(xnode != NULL)
	{
		if (xnode->StartTag != 0 && xnode->NameLength == 8 && memcmp(xnode->Name, "Envelope", 8)==0)
		{
			// Envelope
			xnode = xnode->Next;
			while(xnode != NULL)
			{
				if (xnode->StartTag!=0 && xnode->NameLength == 4 && memcmp(xnode->Name, "Body", 4) == 0)
				{
					// Body
					xnode = xnode->Next;
					while(xnode != NULL)
					{
						if (xnode->StartTag != 0 && xnode->NameLength == 9 && memcmp(xnode->Name, "GetVolume",9) == 0)
						{
							// Inside the interesting part of the SOAP
							xnode = xnode->Next;
							while(xnode != NULL)
							{
								if (xnode->NameLength == 10 && memcmp(xnode->Name, "InstanceID",10)==0)
								{
									p_InstanceIDLength = ILibReadInnerXML(xnode, &p_InstanceID);
									OK |= 1;
								}
								else if (xnode->NameLength == 7 && memcmp(xnode->Name, "Channel",7)==0)
								{
									p_ChannelLength = ILibReadInnerXML(xnode, &p_Channel);
									p_Channel[p_ChannelLength]=0;
									OK |= 2;
								}
								if (xnode->Peer == NULL)
								{
									xnode = xnode->Parent;
									break;
								}
								else
								{
									xnode = xnode->Peer;
								}
							}
						}
						if (xnode != NULL)
						{
							if (xnode->Peer == NULL)
							{
								xnode = xnode->Parent;
								break;
							}
							else
							{
								xnode = xnode->Peer;
							}
						}
					}
				}
				if (xnode != NULL)
				{
					if (xnode->Peer == NULL)
					{
						xnode = xnode->Parent;
						break;
					}
					else
					{
						xnode = xnode->Peer;
					}
				}
			}
		}
		if (xnode != NULL){xnode = xnode->Peer;}
	}
	ILibDestructXMLNodeList(root);
	if (OK != 3)
	{
		UPnPResponse_Error(ReaderObject, 402, "Illegal value");
		return;
	}
	
	/* Type Checking */
	OK = ILibGetULong(p_InstanceID,p_InstanceIDLength, &TempULong);
	if (OK!=0)
	{
		UPnPResponse_Error(ReaderObject,402,"Illegal value");
		return;
	}
	_InstanceID = (unsigned int)TempULong;
	_ChannelLength = ILibInPlaceXmlUnEscape(p_Channel);
	_Channel = p_Channel;
	for(OK=0;OK<UPnP_StateVariable_AllowedValues_MAX;++OK)
	{
		if (UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_A_ARG_TYPE_Channel->AllowedValues[OK]!=NULL)
		{
			if (strcmp(_Channel,UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_A_ARG_TYPE_Channel->AllowedValues[OK])==0)
			{
				OK=0;
				break;
			}
		}
		else
		{
			break;
		}
	}
	if (OK!=0)
	{
		UPnPResponse_Error(ReaderObject,402,"Illegal value");
		return;
	}
	if (UPnPFP_RenderingControl_GetVolume == NULL)
	UPnPResponse_Error(ReaderObject,501,"No Function Handler");
	else
	UPnPFP_RenderingControl_GetVolume((void*)ReaderObject,_InstanceID,_Channel);
}

void UPnPDispatch_RenderingControl_ListPresets(char *buffer, int offset, int bufferLength, struct ILibWebServer_Session *ReaderObject)
{
	unsigned long TempULong;
	int OK = 0;
	char *p_InstanceID = NULL;
	int p_InstanceIDLength = 0;
	unsigned int _InstanceID = 0;
	struct ILibXMLNode *xnode = ILibParseXML(buffer, offset, bufferLength);
	struct ILibXMLNode *root = xnode;
	if (ILibProcessXMLNodeList(root)!=0)
	{
		/* The XML is not well formed! */
		ILibDestructXMLNodeList(root);
		UPnPResponse_Error(ReaderObject, 501, "Invalid XML");
		return;
	}
	while(xnode != NULL)
	{
		if (xnode->StartTag != 0 && xnode->NameLength == 8 && memcmp(xnode->Name, "Envelope", 8)==0)
		{
			// Envelope
			xnode = xnode->Next;
			while(xnode != NULL)
			{
				if (xnode->StartTag!=0 && xnode->NameLength == 4 && memcmp(xnode->Name, "Body", 4) == 0)
				{
					// Body
					xnode = xnode->Next;
					while(xnode != NULL)
					{
						if (xnode->StartTag != 0 && xnode->NameLength == 11 && memcmp(xnode->Name, "ListPresets",11) == 0)
						{
							// Inside the interesting part of the SOAP
							xnode = xnode->Next;
							while(xnode != NULL)
							{
								if (xnode->NameLength == 10 && memcmp(xnode->Name, "InstanceID",10)==0)
								{
									p_InstanceIDLength = ILibReadInnerXML(xnode, &p_InstanceID);
									OK |= 1;
								}
								if (xnode->Peer == NULL)
								{
									xnode = xnode->Parent;
									break;
								}
								else
								{
									xnode = xnode->Peer;
								}
							}
						}
						if (xnode != NULL)
						{
							if (xnode->Peer == NULL)
							{
								xnode = xnode->Parent;
								break;
							}
							else
							{
								xnode = xnode->Peer;
							}
						}
					}
				}
				if (xnode != NULL)
				{
					if (xnode->Peer == NULL)
					{
						xnode = xnode->Parent;
						break;
					}
					else
					{
						xnode = xnode->Peer;
					}
				}
			}
		}
		if (xnode != NULL){xnode = xnode->Peer;}
	}
	ILibDestructXMLNodeList(root);
	if (OK != 1)
	{
		UPnPResponse_Error(ReaderObject, 402, "Illegal value");
		return;
	}
	
	/* Type Checking */
	OK = ILibGetULong(p_InstanceID,p_InstanceIDLength, &TempULong);
	if (OK!=0)
	{
		UPnPResponse_Error(ReaderObject,402,"Illegal value");
		return;
	}
	_InstanceID = (unsigned int)TempULong;
	if (UPnPFP_RenderingControl_ListPresets == NULL)
	UPnPResponse_Error(ReaderObject,501,"No Function Handler");
	else
	UPnPFP_RenderingControl_ListPresets((void*)ReaderObject,_InstanceID);
}

void UPnPDispatch_RenderingControl_SelectPreset(char *buffer, int offset, int bufferLength, struct ILibWebServer_Session *ReaderObject)
{
	unsigned long TempULong;
	int OK = 0;
	char *p_InstanceID = NULL;
	int p_InstanceIDLength = 0;
	unsigned int _InstanceID = 0;
	char *p_PresetName = NULL;
	int p_PresetNameLength = 0;
	char* _PresetName = "";
	int _PresetNameLength;
	struct ILibXMLNode *xnode = ILibParseXML(buffer, offset, bufferLength);
	struct ILibXMLNode *root = xnode;
	if (ILibProcessXMLNodeList(root)!=0)
	{
		/* The XML is not well formed! */
		ILibDestructXMLNodeList(root);
		UPnPResponse_Error(ReaderObject, 501, "Invalid XML");
		return;
	}
	while(xnode != NULL)
	{
		if (xnode->StartTag != 0 && xnode->NameLength == 8 && memcmp(xnode->Name, "Envelope", 8)==0)
		{
			// Envelope
			xnode = xnode->Next;
			while(xnode != NULL)
			{
				if (xnode->StartTag!=0 && xnode->NameLength == 4 && memcmp(xnode->Name, "Body", 4) == 0)
				{
					// Body
					xnode = xnode->Next;
					while(xnode != NULL)
					{
						if (xnode->StartTag != 0 && xnode->NameLength == 12 && memcmp(xnode->Name, "SelectPreset",12) == 0)
						{
							// Inside the interesting part of the SOAP
							xnode = xnode->Next;
							while(xnode != NULL)
							{
								if (xnode->NameLength == 10 && memcmp(xnode->Name, "InstanceID",10)==0)
								{
									p_InstanceIDLength = ILibReadInnerXML(xnode, &p_InstanceID);
									OK |= 1;
								}
								else if (xnode->NameLength == 10 && memcmp(xnode->Name, "PresetName",10)==0)
								{
									p_PresetNameLength = ILibReadInnerXML(xnode, &p_PresetName);
									p_PresetName[p_PresetNameLength]=0;
									OK |= 2;
								}
								if (xnode->Peer == NULL)
								{
									xnode = xnode->Parent;
									break;
								}
								else
								{
									xnode = xnode->Peer;
								}
							}
						}
						if (xnode != NULL)
						{
							if (xnode->Peer == NULL)
							{
								xnode = xnode->Parent;
								break;
							}
							else
							{
								xnode = xnode->Peer;
							}
						}
					}
				}
				if (xnode != NULL)
				{
					if (xnode->Peer == NULL)
					{
						xnode = xnode->Parent;
						break;
					}
					else
					{
						xnode = xnode->Peer;
					}
				}
			}
		}
		if (xnode != NULL){xnode = xnode->Peer;}
	}
	ILibDestructXMLNodeList(root);
	if (OK != 3)
	{
		UPnPResponse_Error(ReaderObject, 402, "Illegal value");
		return;
	}
	
	/* Type Checking */
	OK = ILibGetULong(p_InstanceID,p_InstanceIDLength, &TempULong);
	if (OK!=0)
	{
		UPnPResponse_Error(ReaderObject,402,"Illegal value");
		return;
	}
	_InstanceID = (unsigned int)TempULong;
	_PresetNameLength = ILibInPlaceXmlUnEscape(p_PresetName);
	_PresetName = p_PresetName;
	for(OK=0;OK<UPnP_StateVariable_AllowedValues_MAX;++OK)
	{
		if (UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_A_ARG_TYPE_PresetName->AllowedValues[OK]!=NULL)
		{
			if (strcmp(_PresetName,UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_A_ARG_TYPE_PresetName->AllowedValues[OK])==0)
			{
				OK=0;
				break;
			}
		}
		else
		{
			break;
		}
	}
	if (OK!=0)
	{
		UPnPResponse_Error(ReaderObject,402,"Illegal value");
		return;
	}
	if (UPnPFP_RenderingControl_SelectPreset == NULL)
	UPnPResponse_Error(ReaderObject,501,"No Function Handler");
	else
	UPnPFP_RenderingControl_SelectPreset((void*)ReaderObject,_InstanceID,_PresetName);
}

void UPnPDispatch_RenderingControl_SetMute(char *buffer, int offset, int bufferLength, struct ILibWebServer_Session *ReaderObject)
{
	unsigned long TempULong;
	int OK = 0;
	char *p_InstanceID = NULL;
	int p_InstanceIDLength = 0;
	unsigned int _InstanceID = 0;
	char *p_Channel = NULL;
	int p_ChannelLength = 0;
	char* _Channel = "";
	int _ChannelLength;
	char *p_DesiredMute = NULL;
	int p_DesiredMuteLength = 0;
	int _DesiredMute = 0;
	struct ILibXMLNode *xnode = ILibParseXML(buffer, offset, bufferLength);
	struct ILibXMLNode *root = xnode;
	if (ILibProcessXMLNodeList(root)!=0)
	{
		/* The XML is not well formed! */
		ILibDestructXMLNodeList(root);
		UPnPResponse_Error(ReaderObject, 501, "Invalid XML");
		return;
	}
	while(xnode != NULL)
	{
		if (xnode->StartTag != 0 && xnode->NameLength == 8 && memcmp(xnode->Name, "Envelope", 8)==0)
		{
			// Envelope
			xnode = xnode->Next;
			while(xnode != NULL)
			{
				if (xnode->StartTag!=0 && xnode->NameLength == 4 && memcmp(xnode->Name, "Body", 4) == 0)
				{
					// Body
					xnode = xnode->Next;
					while(xnode != NULL)
					{
						if (xnode->StartTag != 0 && xnode->NameLength == 7 && memcmp(xnode->Name, "SetMute",7) == 0)
						{
							// Inside the interesting part of the SOAP
							xnode = xnode->Next;
							while(xnode != NULL)
							{
								if (xnode->NameLength == 10 && memcmp(xnode->Name, "InstanceID",10)==0)
								{
									p_InstanceIDLength = ILibReadInnerXML(xnode, &p_InstanceID);
									OK |= 1;
								}
								else if (xnode->NameLength == 7 && memcmp(xnode->Name, "Channel",7)==0)
								{
									p_ChannelLength = ILibReadInnerXML(xnode, &p_Channel);
									p_Channel[p_ChannelLength]=0;
									OK |= 2;
								}
								else if (xnode->NameLength == 11 && memcmp(xnode->Name, "DesiredMute",11)==0)
								{
									p_DesiredMuteLength = ILibReadInnerXML(xnode, &p_DesiredMute);
									OK |= 4;
								}
								if (xnode->Peer == NULL)
								{
									xnode = xnode->Parent;
									break;
								}
								else
								{
									xnode = xnode->Peer;
								}
							}
						}
						if (xnode != NULL)
						{
							if (xnode->Peer == NULL)
							{
								xnode = xnode->Parent;
								break;
							}
							else
							{
								xnode = xnode->Peer;
							}
						}
					}
				}
				if (xnode != NULL)
				{
					if (xnode->Peer == NULL)
					{
						xnode = xnode->Parent;
						break;
					}
					else
					{
						xnode = xnode->Peer;
					}
				}
			}
		}
		if (xnode != NULL){xnode = xnode->Peer;}
	}
	ILibDestructXMLNodeList(root);
	if (OK != 7)
	{
		UPnPResponse_Error(ReaderObject, 402, "Illegal value");
		return;
	}
	
	/* Type Checking */
	OK = ILibGetULong(p_InstanceID,p_InstanceIDLength, &TempULong);
	if (OK!=0)
	{
		UPnPResponse_Error(ReaderObject,402,"Illegal value");
		return;
	}
	_InstanceID = (unsigned int)TempULong;
	_ChannelLength = ILibInPlaceXmlUnEscape(p_Channel);
	_Channel = p_Channel;
	for(OK=0;OK<UPnP_StateVariable_AllowedValues_MAX;++OK)
	{
		if (UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_A_ARG_TYPE_Channel->AllowedValues[OK]!=NULL)
		{
			if (strcmp(_Channel,UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_A_ARG_TYPE_Channel->AllowedValues[OK])==0)
			{
				OK=0;
				break;
			}
		}
		else
		{
			break;
		}
	}
	if (OK!=0)
	{
		UPnPResponse_Error(ReaderObject,402,"Illegal value");
		return;
	}
	OK=0;
	if (p_DesiredMuteLength == 4)
	{
		if (strncasecmp(p_DesiredMute,"true",4) == 0)
		{
			OK = 1;
			_DesiredMute = 1;
		}
	}
	if (p_DesiredMuteLength == 5)
	{
		if (strncasecmp(p_DesiredMute,"false",5) == 0)
		{
			OK = 1;
			_DesiredMute = 0;
		}
	}
	if (p_DesiredMuteLength == 1)
	{
		if (memcmp(p_DesiredMute,"0",1) == 0)
		{
			OK = 1;
			_DesiredMute = 0;
		}
		if (memcmp(p_DesiredMute,"1",1) == 0)
		{
			OK = 1;
			_DesiredMute = 1;
		}
	}
	if (OK == 0)
	{
		UPnPResponse_Error(ReaderObject, 402, "Illegal value");
		return;
	}
	if (UPnPFP_RenderingControl_SetMute == NULL)
	UPnPResponse_Error(ReaderObject,501,"No Function Handler");
	else
	UPnPFP_RenderingControl_SetMute((void*)ReaderObject,_InstanceID,_Channel,_DesiredMute);
}

void UPnPDispatch_RenderingControl_SetVolume(char *buffer, int offset, int bufferLength, struct ILibWebServer_Session *ReaderObject)
{
	unsigned long TempULong;
	unsigned long TempULong2;
	int OK = 0;
	char *p_InstanceID = NULL;
	int p_InstanceIDLength = 0;
	unsigned int _InstanceID = 0;
	char *p_Channel = NULL;
	int p_ChannelLength = 0;
	char* _Channel = "";
	int _ChannelLength;
	char *p_DesiredVolume = NULL;
	int p_DesiredVolumeLength = 0;
	unsigned short _DesiredVolume = 0;
	struct ILibXMLNode *xnode = ILibParseXML(buffer, offset, bufferLength);
	struct ILibXMLNode *root = xnode;
	if (ILibProcessXMLNodeList(root)!=0)
	{
		/* The XML is not well formed! */
		ILibDestructXMLNodeList(root);
		UPnPResponse_Error(ReaderObject, 501, "Invalid XML");
		return;
	}
	while(xnode != NULL)
	{
		if (xnode->StartTag != 0 && xnode->NameLength == 8 && memcmp(xnode->Name, "Envelope", 8)==0)
		{
			// Envelope
			xnode = xnode->Next;
			while(xnode != NULL)
			{
				if (xnode->StartTag!=0 && xnode->NameLength == 4 && memcmp(xnode->Name, "Body", 4) == 0)
				{
					// Body
					xnode = xnode->Next;
					while(xnode != NULL)
					{
						if (xnode->StartTag != 0 && xnode->NameLength == 9 && memcmp(xnode->Name, "SetVolume",9) == 0)
						{
							// Inside the interesting part of the SOAP
							xnode = xnode->Next;
							while(xnode != NULL)
							{
								if (xnode->NameLength == 10 && memcmp(xnode->Name, "InstanceID",10)==0)
								{
									p_InstanceIDLength = ILibReadInnerXML(xnode, &p_InstanceID);
									OK |= 1;
								}
								else if (xnode->NameLength == 7 && memcmp(xnode->Name, "Channel",7)==0)
								{
									p_ChannelLength = ILibReadInnerXML(xnode, &p_Channel);
									p_Channel[p_ChannelLength]=0;
									OK |= 2;
								}
								else if (xnode->NameLength == 13 && memcmp(xnode->Name, "DesiredVolume",13)==0)
								{
									p_DesiredVolumeLength = ILibReadInnerXML(xnode, &p_DesiredVolume);
									OK |= 4;
								}
								if (xnode->Peer == NULL)
								{
									xnode = xnode->Parent;
									break;
								}
								else
								{
									xnode = xnode->Peer;
								}
							}
						}
						if (xnode != NULL)
						{
							if (xnode->Peer == NULL)
							{
								xnode = xnode->Parent;
								break;
							}
							else
							{
								xnode = xnode->Peer;
							}
						}
					}
				}
				if (xnode != NULL)
				{
					if (xnode->Peer == NULL)
					{
						xnode = xnode->Parent;
						break;
					}
					else
					{
						xnode = xnode->Peer;
					}
				}
			}
		}
		if (xnode != NULL){xnode = xnode->Peer;}
	}
	ILibDestructXMLNodeList(root);
	if (OK != 7)
	{
		UPnPResponse_Error(ReaderObject, 402, "Illegal value");
		return;
	}
	
	/* Type Checking */
	OK = ILibGetULong(p_InstanceID,p_InstanceIDLength, &TempULong);
	if (OK!=0)
	{
		UPnPResponse_Error(ReaderObject,402,"Illegal value");
		return;
	}
	_InstanceID = (unsigned int)TempULong;
	_ChannelLength = ILibInPlaceXmlUnEscape(p_Channel);
	_Channel = p_Channel;
	for(OK=0;OK<UPnP_StateVariable_AllowedValues_MAX;++OK)
	{
		if (UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_A_ARG_TYPE_Channel->AllowedValues[OK]!=NULL)
		{
			if (strcmp(_Channel,UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_A_ARG_TYPE_Channel->AllowedValues[OK])==0)
			{
				OK=0;
				break;
			}
		}
		else
		{
			break;
		}
	}
	if (OK!=0)
	{
		UPnPResponse_Error(ReaderObject,402,"Illegal value");
		return;
	}
	OK = ILibGetULong(p_DesiredVolume,p_DesiredVolumeLength, &TempULong);
	if (OK!=0)
	{
		UPnPResponse_Error(ReaderObject,402,"Illegal value");
		return;
	}
	else
	{
		OK = 0;
		if (UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_Volume->MinMaxStep[0]!=NULL)
		{
			ILibGetULong(UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_Volume->MinMaxStep[0],(int)strlen(UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_Volume->MinMaxStep[0]), &TempULong2);
			if (TempULong<TempULong2){OK=1;}
		}
		if (UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_Volume->MinMaxStep[1]!=NULL)
		{
			ILibGetULong(UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_Volume->MinMaxStep[1],(int)strlen(UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_Volume->MinMaxStep[1]), &TempULong2);
			if (TempULong>TempULong2){OK=1;}
		}
		if (OK!=0)
		{
			UPnPResponse_Error(ReaderObject,402,"Illegal value");
			return;
		}
		_DesiredVolume = (unsigned short)TempULong;
	}
	if (UPnPFP_RenderingControl_SetVolume == NULL)
	UPnPResponse_Error(ReaderObject,501,"No Function Handler");
	else
	UPnPFP_RenderingControl_SetVolume((void*)ReaderObject,_InstanceID,_Channel,_DesiredVolume);
}

void UPnPDispatch_RenderingControl_upnpRenderer_GetMediaPlayersList(char *buffer, int offset, int bufferLength, struct ILibWebServer_Session *ReaderObject)
{
	unsigned long TempULong;
	int OK = 0;
	char *p_InstanceID = NULL;
	int p_InstanceIDLength = 0;
	unsigned int _InstanceID = 0;
	struct ILibXMLNode *xnode = ILibParseXML(buffer, offset, bufferLength);
	struct ILibXMLNode *root = xnode;
	if (ILibProcessXMLNodeList(root)!=0)
	{
		/* The XML is not well formed! */
		ILibDestructXMLNodeList(root);
		UPnPResponse_Error(ReaderObject, 501, "Invalid XML");
		return;
	}
	while(xnode != NULL)
	{
		if (xnode->StartTag != 0 && xnode->NameLength == 8 && memcmp(xnode->Name, "Envelope", 8)==0)
		{
			// Envelope
			xnode = xnode->Next;
			while(xnode != NULL)
			{
				if (xnode->StartTag!=0 && xnode->NameLength == 4 && memcmp(xnode->Name, "Body", 4) == 0)
				{
					// Body
					xnode = xnode->Next;
					while(xnode != NULL)
					{
						if (xnode->StartTag != 0 && xnode->NameLength == 32 && memcmp(xnode->Name, "upnpRenderer_GetMediaPlayersList",32) == 0)
						{
							// Inside the interesting part of the SOAP
							xnode = xnode->Next;
							while(xnode != NULL)
							{
								if (xnode->NameLength == 10 && memcmp(xnode->Name, "InstanceID",10)==0)
								{
									p_InstanceIDLength = ILibReadInnerXML(xnode, &p_InstanceID);
									OK |= 1;
								}
								if (xnode->Peer == NULL)
								{
									xnode = xnode->Parent;
									break;
								}
								else
								{
									xnode = xnode->Peer;
								}
							}
						}
						if (xnode != NULL)
						{
							if (xnode->Peer == NULL)
							{
								xnode = xnode->Parent;
								break;
							}
							else
							{
								xnode = xnode->Peer;
							}
						}
					}
				}
				if (xnode != NULL)
				{
					if (xnode->Peer == NULL)
					{
						xnode = xnode->Parent;
						break;
					}
					else
					{
						xnode = xnode->Peer;
					}
				}
			}
		}
		if (xnode != NULL){xnode = xnode->Peer;}
	}
	ILibDestructXMLNodeList(root);
	if (OK != 1)
	{
		UPnPResponse_Error(ReaderObject, 402, "Illegal value");
		return;
	}
	
	/* Type Checking */
	OK = ILibGetULong(p_InstanceID,p_InstanceIDLength, &TempULong);
	if (OK!=0)
	{
		UPnPResponse_Error(ReaderObject,402,"Illegal value");
		return;
	}
	_InstanceID = (unsigned int)TempULong;
	if (UPnPFP_RenderingControl_upnpRenderer_GetMediaPlayersList == NULL)
	UPnPResponse_Error(ReaderObject,501,"No Function Handler");
	else
	UPnPFP_RenderingControl_upnpRenderer_GetMediaPlayersList((void*)ReaderObject,_InstanceID);
}

void UPnPDispatch_RenderingControl_upnpRenderer_SetMediaPlayer(char *buffer, int offset, int bufferLength, struct ILibWebServer_Session *ReaderObject)
{
	unsigned long TempULong;
	int OK = 0;
	char *p_InstanceID = NULL;
	int p_InstanceIDLength = 0;
	unsigned int _InstanceID = 0;
	char *p_MediaPlayerName = NULL;
	int p_MediaPlayerNameLength = 0;
	char* _MediaPlayerName = "";
	int _MediaPlayerNameLength;
	struct ILibXMLNode *xnode = ILibParseXML(buffer, offset, bufferLength);
	struct ILibXMLNode *root = xnode;
	if (ILibProcessXMLNodeList(root)!=0)
	{
		/* The XML is not well formed! */
		ILibDestructXMLNodeList(root);
		UPnPResponse_Error(ReaderObject, 501, "Invalid XML");
		return;
	}
	while(xnode != NULL)
	{
		if (xnode->StartTag != 0 && xnode->NameLength == 8 && memcmp(xnode->Name, "Envelope", 8)==0)
		{
			// Envelope
			xnode = xnode->Next;
			while(xnode != NULL)
			{
				if (xnode->StartTag!=0 && xnode->NameLength == 4 && memcmp(xnode->Name, "Body", 4) == 0)
				{
					// Body
					xnode = xnode->Next;
					while(xnode != NULL)
					{
						if (xnode->StartTag != 0 && xnode->NameLength == 27 && memcmp(xnode->Name, "upnpRenderer_SetMediaPlayer",27) == 0)
						{
							// Inside the interesting part of the SOAP
							xnode = xnode->Next;
							while(xnode != NULL)
							{
								if (xnode->NameLength == 10 && memcmp(xnode->Name, "InstanceID",10)==0)
								{
									p_InstanceIDLength = ILibReadInnerXML(xnode, &p_InstanceID);
									OK |= 1;
								}
								else if (xnode->NameLength == 15 && memcmp(xnode->Name, "MediaPlayerName",15)==0)
								{
									p_MediaPlayerNameLength = ILibReadInnerXML(xnode, &p_MediaPlayerName);
									p_MediaPlayerName[p_MediaPlayerNameLength]=0;
									OK |= 2;
								}
								if (xnode->Peer == NULL)
								{
									xnode = xnode->Parent;
									break;
								}
								else
								{
									xnode = xnode->Peer;
								}
							}
						}
						if (xnode != NULL)
						{
							if (xnode->Peer == NULL)
							{
								xnode = xnode->Parent;
								break;
							}
							else
							{
								xnode = xnode->Peer;
							}
						}
					}
				}
				if (xnode != NULL)
				{
					if (xnode->Peer == NULL)
					{
						xnode = xnode->Parent;
						break;
					}
					else
					{
						xnode = xnode->Peer;
					}
				}
			}
		}
		if (xnode != NULL){xnode = xnode->Peer;}
	}
	ILibDestructXMLNodeList(root);
	if (OK != 3)
	{
		UPnPResponse_Error(ReaderObject, 402, "Illegal value");
		return;
	}
	
	/* Type Checking */
	OK = ILibGetULong(p_InstanceID,p_InstanceIDLength, &TempULong);
	if (OK!=0)
	{
		UPnPResponse_Error(ReaderObject,402,"Illegal value");
		return;
	}
	_InstanceID = (unsigned int)TempULong;
	_MediaPlayerNameLength = ILibInPlaceXmlUnEscape(p_MediaPlayerName);
	_MediaPlayerName = p_MediaPlayerName;
	if (UPnPFP_RenderingControl_upnpRenderer_SetMediaPlayer == NULL)
	UPnPResponse_Error(ReaderObject,501,"No Function Handler");
	else
	UPnPFP_RenderingControl_upnpRenderer_SetMediaPlayer((void*)ReaderObject,_InstanceID,_MediaPlayerName);
}



int UPnPProcessPOST(struct ILibWebServer_Session *session, struct packetheader* header, char *bodyBuffer, int offset, int bodyBufferLength)
{
	struct packetheader_field_node *f = header->FirstField;
	char* HOST;
	char* SOAPACTION = NULL;
	int SOAPACTIONLength = 0;
	struct parser_result *r, *r2;
	struct parser_result_field *prf;
	int RetVal = 0;
	
	// Iterate through all the HTTP Headers
	while(f!=NULL)
	{
		if (f->FieldLength == 4 && strncasecmp(f->Field, "HOST", 4) == 0)
		{
			HOST = f->FieldData;
		}
		else if (f->FieldLength == 10 && strncasecmp(f->Field, "SOAPACTION", 10) == 0)
		{
			r = ILibParseString(f->FieldData, 0, f->FieldDataLength, "#", 1);
			SOAPACTION = r->LastResult->data;
			SOAPACTIONLength = r->LastResult->datalength - 1;
			ILibDestructParserResults(r);
		}
		else if (f->FieldLength == 10 && strncasecmp(f->Field, "USER-AGENT", 10) == 0)
		{
			// Check UPnP version of the Control Point which invoked us
			r = ILibParseString(f->FieldData, 0, f->FieldDataLength, " ", 1);
			prf = r->FirstResult;
			while(prf!=NULL)
			{
				if (prf->datalength>5 && memcmp(prf->data, "UPnP/", 5)==0)
				{
					r2 = ILibParseString(prf->data + 5, 0, prf->datalength - 5, ".", 1);
					r2->FirstResult->data[r2->FirstResult->datalength] = 0;
					r2->LastResult->data[r2->LastResult->datalength] = 0;
					if (atoi(r2->FirstResult->data) == 1 && atoi(r2->LastResult->data) > 0)
					{
						session->Reserved9 = 1;
					}
					ILibDestructParserResults(r2);
				}
				prf = prf->NextResult;
			}
			ILibDestructParserResults(r);
		}
		f = f->NextField;
	}
	if (header->DirectiveObjLength==20 && memcmp((header->DirectiveObj)+1,"AVTransport/control",19)==0)
	{
		if (SOAPACTIONLength==26 && memcmp(SOAPACTION,"GetCurrentTransportActions",26)==0)
		{
			UPnPDispatch_AVTransport_GetCurrentTransportActions(bodyBuffer, offset, bodyBufferLength, session);
		}
		else if (SOAPACTIONLength==21 && memcmp(SOAPACTION,"GetDeviceCapabilities",21)==0)
		{
			UPnPDispatch_AVTransport_GetDeviceCapabilities(bodyBuffer, offset, bodyBufferLength, session);
		}
		else if (SOAPACTIONLength==12 && memcmp(SOAPACTION,"GetMediaInfo",12)==0)
		{
			UPnPDispatch_AVTransport_GetMediaInfo(bodyBuffer, offset, bodyBufferLength, session);
		}
		else if (SOAPACTIONLength==15 && memcmp(SOAPACTION,"GetPositionInfo",15)==0)
		{
			UPnPDispatch_AVTransport_GetPositionInfo(bodyBuffer, offset, bodyBufferLength, session);
		}
		else if (SOAPACTIONLength==16 && memcmp(SOAPACTION,"GetTransportInfo",16)==0)
		{
			UPnPDispatch_AVTransport_GetTransportInfo(bodyBuffer, offset, bodyBufferLength, session);
		}
		else if (SOAPACTIONLength==20 && memcmp(SOAPACTION,"GetTransportSettings",20)==0)
		{
			UPnPDispatch_AVTransport_GetTransportSettings(bodyBuffer, offset, bodyBufferLength, session);
		}
		else if (SOAPACTIONLength==4 && memcmp(SOAPACTION,"Next",4)==0)
		{
			UPnPDispatch_AVTransport_Next(bodyBuffer, offset, bodyBufferLength, session);
		}
		else if (SOAPACTIONLength==5 && memcmp(SOAPACTION,"Pause",5)==0)
		{
			UPnPDispatch_AVTransport_Pause(bodyBuffer, offset, bodyBufferLength, session);
		}
		else if (SOAPACTIONLength==4 && memcmp(SOAPACTION,"Play",4)==0)
		{
			UPnPDispatch_AVTransport_Play(bodyBuffer, offset, bodyBufferLength, session);
		}
		else if (SOAPACTIONLength==8 && memcmp(SOAPACTION,"Previous",8)==0)
		{
			UPnPDispatch_AVTransport_Previous(bodyBuffer, offset, bodyBufferLength, session);
		}
		else if (SOAPACTIONLength==4 && memcmp(SOAPACTION,"Seek",4)==0)
		{
			UPnPDispatch_AVTransport_Seek(bodyBuffer, offset, bodyBufferLength, session);
		}
		else if (SOAPACTIONLength==17 && memcmp(SOAPACTION,"SetAVTransportURI",17)==0)
		{
			UPnPDispatch_AVTransport_SetAVTransportURI(bodyBuffer, offset, bodyBufferLength, session);
		}
		else if (SOAPACTIONLength==11 && memcmp(SOAPACTION,"SetPlayMode",11)==0)
		{
			UPnPDispatch_AVTransport_SetPlayMode(bodyBuffer, offset, bodyBufferLength, session);
		}
		else if (SOAPACTIONLength==4 && memcmp(SOAPACTION,"Stop",4)==0)
		{
			UPnPDispatch_AVTransport_Stop(bodyBuffer, offset, bodyBufferLength, session);
		}
		else
		{
			RetVal=1;
		}
	}
	else if (header->DirectiveObjLength==26 && memcmp((header->DirectiveObj)+1,"ConnectionManager/control",25)==0)
	{
		if (SOAPACTIONLength==23 && memcmp(SOAPACTION,"GetCurrentConnectionIDs",23)==0)
		{
			UPnPDispatch_ConnectionManager_GetCurrentConnectionIDs(bodyBuffer, offset, bodyBufferLength, session);
		}
		else if (SOAPACTIONLength==24 && memcmp(SOAPACTION,"GetCurrentConnectionInfo",24)==0)
		{
			UPnPDispatch_ConnectionManager_GetCurrentConnectionInfo(bodyBuffer, offset, bodyBufferLength, session);
		}
		else if (SOAPACTIONLength==15 && memcmp(SOAPACTION,"GetProtocolInfo",15)==0)
		{
			UPnPDispatch_ConnectionManager_GetProtocolInfo(bodyBuffer, offset, bodyBufferLength, session);
		}
		else
		{
			RetVal=1;
		}
	}
	else if (header->DirectiveObjLength==25 && memcmp((header->DirectiveObj)+1,"RenderingControl/control",24)==0)
	{
		if (SOAPACTIONLength==7 && memcmp(SOAPACTION,"GetMute",7)==0)
		{
			UPnPDispatch_RenderingControl_GetMute(bodyBuffer, offset, bodyBufferLength, session);
		}
		else if (SOAPACTIONLength==9 && memcmp(SOAPACTION,"GetVolume",9)==0)
		{
			UPnPDispatch_RenderingControl_GetVolume(bodyBuffer, offset, bodyBufferLength, session);
		}
		else if (SOAPACTIONLength==11 && memcmp(SOAPACTION,"ListPresets",11)==0)
		{
			UPnPDispatch_RenderingControl_ListPresets(bodyBuffer, offset, bodyBufferLength, session);
		}
		else if (SOAPACTIONLength==12 && memcmp(SOAPACTION,"SelectPreset",12)==0)
		{
			UPnPDispatch_RenderingControl_SelectPreset(bodyBuffer, offset, bodyBufferLength, session);
		}
		else if (SOAPACTIONLength==7 && memcmp(SOAPACTION,"SetMute",7)==0)
		{
			UPnPDispatch_RenderingControl_SetMute(bodyBuffer, offset, bodyBufferLength, session);
		}
		else if (SOAPACTIONLength==9 && memcmp(SOAPACTION,"SetVolume",9)==0)
		{
			UPnPDispatch_RenderingControl_SetVolume(bodyBuffer, offset, bodyBufferLength, session);
		}
		else if (SOAPACTIONLength==32 && memcmp(SOAPACTION,"upnpRenderer_GetMediaPlayersList",32)==0)
		{
			UPnPDispatch_RenderingControl_upnpRenderer_GetMediaPlayersList(bodyBuffer, offset, bodyBufferLength, session);
		}
		else if (SOAPACTIONLength==27 && memcmp(SOAPACTION,"upnpRenderer_SetMediaPlayer",27)==0)
		{
			UPnPDispatch_RenderingControl_upnpRenderer_SetMediaPlayer(bodyBuffer, offset, bodyBufferLength, session);
		}
		else
		{
			RetVal=1;
		}
	}
	else
	{
		RetVal=1;
	}
	
	return(RetVal);
}
struct SubscriberInfo* UPnPRemoveSubscriberInfo(struct SubscriberInfo **Head, int *TotalSubscribers, char* SID, int SIDLength)
{
	struct SubscriberInfo *info = *Head;
	while(info != NULL)
	{
		if (SIDLength != -1 && info->SIDLength == SIDLength && memcmp(info->SID, SID, SIDLength) == 0)	//	SIDLength != -1 && - added by jobnik.net
		{
			if (info->Previous) info->Previous->Next = info->Next; else *Head = info->Next;
			if (info->Next) info->Next->Previous = info->Previous;
			break;
		}
		info = info->Next;
	}
	if (info != NULL)
	{
		info->Previous = NULL;
		info->Next = NULL;
		--(*TotalSubscribers);
	}
	return(info);
}

#define UPnPDestructSubscriberInfo(info)\
{\
	free(info->Path);\
	free(info->SID);\
	free(info);\
}

#define UPnPDestructEventObject(EvObject)\
{\
	free(EvObject->PacketBody);\
	free(EvObject);\
}

#define UPnPDestructEventDataObject(EvData)\
{\
	free(EvData);\
}

void UPnPExpireSubscriberInfo(struct UPnPDataObject *d, struct SubscriberInfo *info)
{
	struct SubscriberInfo *t = info;
	while(t->Previous != NULL) { t = t->Previous; }
	if (d->HeadSubscriberPtr_AVTransport==t)
	{
		--(d->NumberOfSubscribers_AVTransport);
	}
	else if (d->HeadSubscriberPtr_ConnectionManager==t)
	{
		--(d->NumberOfSubscribers_ConnectionManager);
	}
	else if (d->HeadSubscriberPtr_RenderingControl==t)
	{
		--(d->NumberOfSubscribers_RenderingControl);
	}
	
	
	if (info->Previous != NULL)
	{
		// This is not the Head
		info->Previous->Next = info->Next;
		if (info->Next != NULL) { info->Next->Previous = info->Previous; }
	}
	else
	{
		// This is the Head
		if (d->HeadSubscriberPtr_AVTransport==info)
		{
			d->HeadSubscriberPtr_AVTransport = info->Next;
			if (info->Next!=NULL)
			{
				info->Next->Previous = NULL;
			}
		}
		else if (d->HeadSubscriberPtr_ConnectionManager==info)
		{
			d->HeadSubscriberPtr_ConnectionManager = info->Next;
			if (info->Next!=NULL)
			{
				info->Next->Previous = NULL;
			}
		}
		else if (d->HeadSubscriberPtr_RenderingControl==info)
		{
			d->HeadSubscriberPtr_RenderingControl = info->Next;
			if (info->Next!=NULL)
			{
				info->Next->Previous = NULL;
			}
		}
		else 
		{
			// Error
			return;
		}
		
	}
	--info->RefCount;
	if (info->RefCount == 0) { UPnPDestructSubscriberInfo(info); }
}

int UPnPSubscriptionExpired(struct SubscriberInfo *info)
{
	int RetVal = 0;
	struct timeval tv;
	gettimeofday(&tv, NULL);
	if ((info->RenewByTime).tv_sec < tv.tv_sec) { RetVal = -1; }
	return(RetVal);
}

void UPnPGetInitialEventBody_AVTransport(struct UPnPDataObject *UPnPObject,char ** body, int *bodylength)
{
	int TempLength;
	TempLength = (int)(25+(int)strlen(UPnPObject->AVTransport_LastChange));
	if ((*body = (char*)malloc(sizeof(char) * TempLength)) == NULL) ILIBCRITICALEXIT(254);
	*bodylength = snprintf(*body, sizeof(char) * TempLength, "LastChange>%s</LastChange",UPnPObject->AVTransport_LastChange);
}
void UPnPGetInitialEventBody_ConnectionManager(struct UPnPDataObject *UPnPObject,char ** body, int *bodylength)
{
	int TempLength;
	TempLength = (int)(177+(int)strlen(UPnPObject->ConnectionManager_SourceProtocolInfo)+(int)strlen(UPnPObject->ConnectionManager_SinkProtocolInfo)+(int)strlen(UPnPObject->ConnectionManager_CurrentConnectionIDs));
	if ((*body = (char*)malloc(sizeof(char) * TempLength)) == NULL) ILIBCRITICALEXIT(254);
	*bodylength = snprintf(*body, sizeof(char) * TempLength, "SourceProtocolInfo>%s</SourceProtocolInfo></e:property><e:property><SinkProtocolInfo>%s</SinkProtocolInfo></e:property><e:property><CurrentConnectionIDs>%s</CurrentConnectionIDs",UPnPObject->ConnectionManager_SourceProtocolInfo,UPnPObject->ConnectionManager_SinkProtocolInfo,UPnPObject->ConnectionManager_CurrentConnectionIDs);
}
void UPnPGetInitialEventBody_RenderingControl(struct UPnPDataObject *UPnPObject,char ** body, int *bodylength)
{
	int TempLength;
	TempLength = (int)(25+(int)strlen(UPnPObject->RenderingControl_LastChange));
	if ((*body = (char*)malloc(sizeof(char) * TempLength)) == NULL) ILIBCRITICALEXIT(254);
	*bodylength = snprintf(*body, sizeof(char) * TempLength, "LastChange>%s</LastChange",UPnPObject->RenderingControl_LastChange);
}


void UPnPProcessUNSUBSCRIBE(struct packetheader *header, struct ILibWebServer_Session *session)
{
	char* SID = NULL;
	int SIDLength = 0;
	struct SubscriberInfo *Info;
	struct packetheader_field_node *f;
	char* packet;
	int packetlength;
	if ((packet = (char*)malloc(50)) == NULL) ILIBCRITICALEXIT(254);
	
	// Iterate through all the HTTP headers
	f = header->FirstField;
	while(f != NULL)
	{
		if (f->FieldLength == 3)
		{
			if (strncasecmp(f->Field, "SID", 3) == 0)
			{
				// Get the Subscription ID
				SID = f->FieldData;
				SIDLength = f->FieldDataLength;
			}
		}
		f = f->NextField;
	}
	sem_wait(&(((struct UPnPDataObject*)session->User)->EventLock));
	if (header->DirectiveObjLength==18 && memcmp(header->DirectiveObj + 1,"AVTransport/event",17)==0)
	{
		Info = UPnPRemoveSubscriberInfo(&(((struct UPnPDataObject*)session->User)->HeadSubscriberPtr_AVTransport),&(((struct UPnPDataObject*)session->User)->NumberOfSubscribers_AVTransport),SID,SIDLength);
		if (Info != NULL)
		{
			--Info->RefCount;
			if (Info->RefCount == 0)
			{
				UPnPDestructSubscriberInfo(Info);
			}
			packetlength = snprintf(packet, 50, "HTTP/1.1 %d %s\r\nContent-Length: 0\r\n\r\n", 200, "OK");
			ILibWebServer_Send_Raw(session, packet, packetlength, 0, 1);
		}
		else
		{
			packetlength = snprintf(packet, 50, "HTTP/1.1 %d %s\r\nContent-Length: 0\r\n\r\n", 412, "Invalid SID");
			ILibWebServer_Send_Raw(session, packet, packetlength, 0, 1);
		}
	}
	else if (header->DirectiveObjLength==24 && memcmp(header->DirectiveObj + 1,"ConnectionManager/event",23)==0)
	{
		Info = UPnPRemoveSubscriberInfo(&(((struct UPnPDataObject*)session->User)->HeadSubscriberPtr_ConnectionManager),&(((struct UPnPDataObject*)session->User)->NumberOfSubscribers_ConnectionManager),SID,SIDLength);
		if (Info != NULL)
		{
			--Info->RefCount;
			if (Info->RefCount == 0)
			{
				UPnPDestructSubscriberInfo(Info);
			}
			packetlength = snprintf(packet, 50, "HTTP/1.1 %d %s\r\nContent-Length: 0\r\n\r\n", 200, "OK");
			ILibWebServer_Send_Raw(session, packet, packetlength, 0, 1);
		}
		else
		{
			packetlength = snprintf(packet, 50, "HTTP/1.1 %d %s\r\nContent-Length: 0\r\n\r\n", 412, "Invalid SID");
			ILibWebServer_Send_Raw(session, packet, packetlength, 0, 1);
		}
	}
	else if (header->DirectiveObjLength==23 && memcmp(header->DirectiveObj + 1,"RenderingControl/event",22)==0)
	{
		Info = UPnPRemoveSubscriberInfo(&(((struct UPnPDataObject*)session->User)->HeadSubscriberPtr_RenderingControl),&(((struct UPnPDataObject*)session->User)->NumberOfSubscribers_RenderingControl),SID,SIDLength);
		if (Info != NULL)
		{
			--Info->RefCount;
			if (Info->RefCount == 0)
			{
				UPnPDestructSubscriberInfo(Info);
			}
			packetlength = snprintf(packet, 50, "HTTP/1.1 %d %s\r\nContent-Length: 0\r\n\r\n", 200, "OK");
			ILibWebServer_Send_Raw(session, packet, packetlength, 0, 1);
		}
		else
		{
			packetlength = snprintf(packet, 50, "HTTP/1.1 %d %s\r\nContent-Length: 0\r\n\r\n", 412, "Invalid SID");
			ILibWebServer_Send_Raw(session, packet, packetlength, 0, 1);
		}
	}
	
	sem_post(&(((struct UPnPDataObject*)session->User)->EventLock));
}

void UPnPTryToSubscribe(char* ServiceName, long Timeout, char* URL, int URLLength, struct ILibWebServer_Session *session)
{
	int *TotalSubscribers = NULL;
	struct SubscriberInfo **HeadPtr = NULL;
	struct SubscriberInfo *NewSubscriber, *TempSubscriber;
	int SIDNumber, rnumber;
	char *SID;
	char *TempString;
	int TempStringLength;
	char *TempString2;
	long TempLong;
	char *packet;
	int packetlength;
	char* path;
	size_t len;
	
	char* escapedURI;
	int escapedURILength;
	
	char *packetbody = NULL;
	int packetbodyLength = 0;
	
	struct parser_result *p;
	struct parser_result *p2;
	
	struct UPnPDataObject *dataObject = (struct UPnPDataObject*)session->User;
	
	if (strncmp(ServiceName,"AVTransport",11)==0)
	{
		TotalSubscribers = &(dataObject->NumberOfSubscribers_AVTransport);
		HeadPtr = &(dataObject->HeadSubscriberPtr_AVTransport);
	}
	if (strncmp(ServiceName,"ConnectionManager",17)==0)
	{
		TotalSubscribers = &(dataObject->NumberOfSubscribers_ConnectionManager);
		HeadPtr = &(dataObject->HeadSubscriberPtr_ConnectionManager);
	}
	if (strncmp(ServiceName,"RenderingControl",16)==0)
	{
		TotalSubscribers = &(dataObject->NumberOfSubscribers_RenderingControl);
		HeadPtr = &(dataObject->HeadSubscriberPtr_RenderingControl);
	}
	
	
	if (*HeadPtr!=NULL)
	{
		NewSubscriber = *HeadPtr;
		while(NewSubscriber != NULL)
		{
			if (UPnPSubscriptionExpired(NewSubscriber) != 0)
			{
				TempSubscriber = NewSubscriber->Next;
				NewSubscriber = UPnPRemoveSubscriberInfo(HeadPtr, TotalSubscribers, NewSubscriber->SID, NewSubscriber->SIDLength);
				UPnPDestructSubscriberInfo(NewSubscriber);
				NewSubscriber = TempSubscriber;
			}
			else
			{
				NewSubscriber = NewSubscriber->Next;
			}
		}
	}
	
	// The Maximum number of subscribers can be bounded
	if (*TotalSubscribers < 10)
	{
		if ((NewSubscriber = (struct SubscriberInfo*)malloc(sizeof(struct SubscriberInfo))) == NULL) ILIBCRITICALEXIT(254);
		memset(NewSubscriber, 0, sizeof(struct SubscriberInfo));
		
		// The SID must be globally unique, so lets generate it using a bunch of random hex characters
		if ((SID = (char*)malloc(43)) == NULL) ILIBCRITICALEXIT(254);
		memset(SID, 0, 38);
		snprintf(SID, 43, "uuid:");
		for(SIDNumber = 5; SIDNumber <= 12; ++SIDNumber)
		{
			rnumber = rand() % 16;
			snprintf(SID + SIDNumber, 43 - SIDNumber, "%x", rnumber);
		}
		snprintf(SID + SIDNumber, 43 - SIDNumber, "-");
		for(SIDNumber = 14; SIDNumber <= 17; ++SIDNumber)
		{
			rnumber = rand()%16;
			snprintf(SID + SIDNumber, 43 - SIDNumber, "%x", rnumber);
		}
		snprintf(SID + SIDNumber, 43 - SIDNumber, "-");
		for(SIDNumber = 19; SIDNumber <= 22; ++SIDNumber)
		{
			rnumber = rand()%16;
			snprintf(SID + SIDNumber, 43 - SIDNumber, "%x", rnumber);
		}
		snprintf(SID + SIDNumber, 43 - SIDNumber, "-");
		for(SIDNumber = 24; SIDNumber <= 27; ++SIDNumber)
		{
			rnumber = rand()%16;
			snprintf(SID + SIDNumber, 43 - SIDNumber, "%x", rnumber);
		}
		snprintf(SID + SIDNumber, 43 - SIDNumber, "-");
		for(SIDNumber = 29; SIDNumber <= 40; ++SIDNumber)
		{
			rnumber = rand()%16;
			snprintf(SID + SIDNumber, 43 - SIDNumber, "%x", rnumber);
		}
		
		p = ILibParseString(URL, 0, URLLength, "://", 3);
		if (p->NumResults == 1)
		{
			ILibWebServer_Send_Raw(session, "HTTP/1.1 412 Precondition Failed\r\nContent-Length: 0\r\n\r\n", 55, 1, 1);
			ILibDestructParserResults(p);
			return;
		}
		TempString = p->LastResult->data;
		TempStringLength = p->LastResult->datalength;
		ILibDestructParserResults(p);
		p = ILibParseString(TempString, 0, TempStringLength,"/", 1);
		p2 = ILibParseString(p->FirstResult->data, 0, p->FirstResult->datalength, ":", 1);
		if ((TempString2 = (char*)malloc(1 + sizeof(char) * p2->FirstResult->datalength)) == NULL) ILIBCRITICALEXIT(254);
		memcpy(TempString2, p2->FirstResult->data, p2->FirstResult->datalength);
		TempString2[p2->FirstResult->datalength] = '\0';
		NewSubscriber->Address = inet_addr(TempString2);
		if (p2->NumResults == 1)
		{
			NewSubscriber->Port = 80;
			if ((path = (char*)malloc(1+TempStringLength - p2->FirstResult->datalength -1)) == NULL) ILIBCRITICALEXIT(254);
			memcpy(path,TempString + p2->FirstResult->datalength,TempStringLength - p2->FirstResult->datalength -1);
			path[TempStringLength - p2->FirstResult->datalength - 1] = '\0';
			NewSubscriber->Path = path;
			NewSubscriber->PathLength = (int)strlen(path);
		}
		else
		{
			ILibGetLong(p2->LastResult->data,p2->LastResult->datalength,&TempLong);
			NewSubscriber->Port = (unsigned short)TempLong;
			if (TempStringLength == p->FirstResult->datalength)
			{
				if ((path = (char*)malloc(2)) == NULL) ILIBCRITICALEXIT(254);
				memcpy(path, "/", 1);
				path[1] = '\0';
			}
			else
			{
				if ((path = (char*)malloc(1 + TempStringLength - p->FirstResult->datalength - 1)) == NULL) ILIBCRITICALEXIT(254);
				memcpy(path,TempString + p->FirstResult->datalength,TempStringLength - p->FirstResult->datalength - 1);
				path[TempStringLength - p->FirstResult->datalength - 1] = '\0';
			}
			NewSubscriber->Path = path;
			NewSubscriber->PathLength = (int)strlen(path);
		}
		ILibDestructParserResults(p);
		ILibDestructParserResults(p2);
		free(TempString2);
		
		if ((escapedURI = (char*)malloc(ILibHTTPEscapeLength(NewSubscriber->Path))) == NULL) ILIBCRITICALEXIT(254);
		escapedURILength = ILibHTTPEscape(escapedURI, NewSubscriber->Path);
		
		free(NewSubscriber->Path);
		NewSubscriber->Path = escapedURI;
		NewSubscriber->PathLength = escapedURILength;
		
		NewSubscriber->RefCount = 1;
		NewSubscriber->Disposing = 0;
		NewSubscriber->Previous = NULL;
		NewSubscriber->SID = SID;
		NewSubscriber->SIDLength = (int)strlen(SID);
		NewSubscriber->SEQ = 0;
		
		// Determine what the subscription renewal cycle is
		gettimeofday(&(NewSubscriber->RenewByTime),NULL);
		(NewSubscriber->RenewByTime).tv_sec += (int)Timeout;
		
		NewSubscriber->Next = *HeadPtr;
		if (*HeadPtr!=NULL) {(*HeadPtr)->Previous = NewSubscriber;}
		*HeadPtr = NewSubscriber;
		++(*TotalSubscribers);
		LVL3DEBUG(printf("\r\n\r\nSubscribed [%s] %d.%d.%d.%d:%d FOR %d Duration\r\n",NewSubscriber->SID,(NewSubscriber->Address)&0xFF,(NewSubscriber->Address>>8)&0xFF,(NewSubscriber->Address>>16)&0xFF,(NewSubscriber->Address>>24)&0xFF,NewSubscriber->Port,Timeout);)
		LVL3DEBUG(printf("TIMESTAMP: %d <%d>\r\n\r\n",(NewSubscriber->RenewByTime).tv_sec-Timeout,NewSubscriber);)
		
		len = 134 + (int)strlen(SID) + (int)strlen(UPnPPLATFORM) + 4;
		if ((packet = (char*)malloc(len)) == NULL) ILIBCRITICALEXIT(254);
		packetlength = snprintf(packet, len, "HTTP/1.1 200 OK\r\nSERVER: %s, UPnP/1.0, MicroStack/1.0.5329\r\nSID: %s\r\nTIMEOUT: Second-%ld\r\nContent-Length: 0\r\n\r\n",UPnPPLATFORM,SID,Timeout);
		if (strcmp(ServiceName,"AVTransport")==0)
		{
			UPnPGetInitialEventBody_AVTransport(dataObject,&packetbody,&packetbodyLength);
		}
		else if (strcmp(ServiceName,"ConnectionManager")==0)
		{
			UPnPGetInitialEventBody_ConnectionManager(dataObject,&packetbody,&packetbodyLength);
		}
		else if (strcmp(ServiceName,"RenderingControl")==0)
		{
			UPnPGetInitialEventBody_RenderingControl(dataObject,&packetbody,&packetbodyLength);
		}
		
		if (packetbody != NULL)
		{
			ILibWebServer_Send_Raw(session, packet, packetlength, 0, 1);
			UPnPSendEvent_Body(dataObject, packetbody, packetbodyLength, NewSubscriber);
			free(packetbody);
		} 
	}
	else
	{
		// Too many subscribers
		ILibWebServer_Send_Raw(session,"HTTP/1.1 412 Too Many Subscribers\r\nContent-Length: 0\r\n\r\n",56,1,1);
	}
}

void UPnPSubscribeEvents(char* path, int pathlength, char* Timeout, int TimeoutLength, char* URL, int URLLength, struct ILibWebServer_Session* session)
{
	long TimeoutVal;
	char* buffer;
	if ((buffer = (char*)malloc(1 + sizeof(char)*pathlength)) == NULL) ILIBCRITICALEXIT(254);
	
	ILibGetLong(Timeout, TimeoutLength, &TimeoutVal);
	memcpy(buffer, path, pathlength);
	buffer[pathlength] = '\0';
	free(buffer);
	if (TimeoutVal>UPnP_MAX_SUBSCRIPTION_TIMEOUT) { TimeoutVal = UPnP_MAX_SUBSCRIPTION_TIMEOUT; }
	if (pathlength==18 && memcmp(path+1,"AVTransport/event",17)==0)
	{
		UPnPTryToSubscribe("AVTransport",TimeoutVal,URL,URLLength,session);
	}
	else if (pathlength==24 && memcmp(path+1,"ConnectionManager/event",23)==0)
	{
		UPnPTryToSubscribe("ConnectionManager",TimeoutVal,URL,URLLength,session);
	}
	else if (pathlength==23 && memcmp(path+1,"RenderingControl/event",22)==0)
	{
		UPnPTryToSubscribe("RenderingControl",TimeoutVal,URL,URLLength,session);
	}
	else
	{
		ILibWebServer_Send_Raw(session,"HTTP/1.1 412 Invalid Service Name\r\nContent-Length: 0\r\n\r\n",56,1,1);
	}
	
}

void UPnPRenewEvents(char* path, int pathlength, char *_SID,int SIDLength, char* Timeout, int TimeoutLength, struct ILibWebServer_Session *ReaderObject)
{
	struct SubscriberInfo *info = NULL;
	long TimeoutVal;
	struct timeval tv;
	char* packet;
	int packetlength;
	char* SID;
	size_t len;
	
	if ((SID = (char*)malloc(SIDLength + 1)) == NULL) ILIBCRITICALEXIT(254);
	memcpy(SID, _SID, SIDLength);
	SID[SIDLength] = '\0';
	
	LVL3DEBUG(gettimeofday(&tv, NULL);)
	LVL3DEBUG(printf("\r\n\r\nTIMESTAMP: %d\r\n", tv.tv_sec);)
	LVL3DEBUG(printf("SUBSCRIBER [%s] attempting to Renew Events for %s Duration [", SID, Timeout);)
	if (pathlength==18 && memcmp(path+1,"AVTransport/event",17)==0)
	{
		info = ((struct UPnPDataObject*)ReaderObject->User)->HeadSubscriberPtr_AVTransport;
	}
	else if (pathlength==24 && memcmp(path+1,"ConnectionManager/event",23)==0)
	{
		info = ((struct UPnPDataObject*)ReaderObject->User)->HeadSubscriberPtr_ConnectionManager;
	}
	else if (pathlength==23 && memcmp(path+1,"RenderingControl/event",22)==0)
	{
		info = ((struct UPnPDataObject*)ReaderObject->User)->HeadSubscriberPtr_RenderingControl;
	}
	
	
	// Find this SID in the subscriber list, and recalculate the expiration timeout
	while(info != NULL && strcmp(info->SID,SID) != 0)
	{
		info = info->Next;
	}
	if (info != NULL)
	{
		ILibGetLong(Timeout, TimeoutLength, &TimeoutVal);
		if (TimeoutVal>UPnP_MAX_SUBSCRIPTION_TIMEOUT) {TimeoutVal = UPnP_MAX_SUBSCRIPTION_TIMEOUT;}
		
		gettimeofday(&tv,NULL);
		(info->RenewByTime).tv_sec = tv.tv_sec + TimeoutVal;
		
		len = 134 + strlen(SID) + 4;
		if ((packet = (char*)malloc(len)) == NULL) ILIBCRITICALEXIT(254);
		packetlength = snprintf(packet, len, "HTTP/1.1 200 OK\r\nSERVER: %s, UPnP/1.0, MicroStack/1.0.5329\r\nSID: %s\r\nTIMEOUT: Second-%ld\r\nContent-Length: 0\r\n\r\n",UPnPPLATFORM,SID,TimeoutVal);
		ILibWebServer_Send_Raw(ReaderObject, packet, packetlength, 0, 1);
		LVL3DEBUG(printf("OK] {%d} <%d>\r\n\r\n", TimeoutVal, info);)
	}
	else
	{
		LVL3DEBUG(printf("FAILED]\r\n\r\n");)
		ILibWebServer_Send_Raw(ReaderObject, "HTTP/1.1 412 Precondition Failed\r\nContent-Length: 0\r\n\r\n", 55, 1, 1);
	}
	free(SID);
}

void UPnPProcessSUBSCRIBE(struct packetheader *header, struct ILibWebServer_Session *session)
{
	char* SID = NULL;
	int SIDLength = 0;
	char* Timeout = NULL;
	int TimeoutLength = 0;
	char* URL = NULL;
	int URLLength = 0;
	struct parser_result *p;
	struct packetheader_field_node *f;
	
	// Iterate through all the HTTP Headers
	f = header->FirstField;
	while(f!=NULL)
	{
		if (f->FieldLength==3 && strncasecmp(f->Field, "SID", 3) == 0)
		{
			// Get the Subscription ID
			SID = f->FieldData;
			SIDLength = f->FieldDataLength;
		}
		else if (f->FieldLength == 8 && strncasecmp(f->Field, "Callback", 8) == 0)
		{
			// Get the Callback URL
			URL = f->FieldData;
			URLLength = f->FieldDataLength;
		}
		else if (f->FieldLength == 7 && strncasecmp(f->Field, "Timeout", 7) == 0)
		{
			// Get the requested timeout value
			Timeout = f->FieldData;
			TimeoutLength = f->FieldDataLength;
		}
		f = f->NextField;
	}
	if (Timeout == NULL)
	{
		// It a timeout wasn't specified, force it to a specific value
		Timeout = "7200";
		TimeoutLength = 4;
	}
	else
	{
		p = ILibParseString(Timeout, 0, TimeoutLength, "-", 1);
		if (p->NumResults == 2)
		{
			Timeout = p->LastResult->data;
			TimeoutLength = p->LastResult->datalength;
			if (TimeoutLength == 8 && strncasecmp(Timeout, "INFINITE", 8) == 0)
			{
				// Infinite timeouts will cause problems, so we don't allow it
				Timeout = "7200";
				TimeoutLength = 4;
			}
		}
		else
		{
			Timeout = "7200";
			TimeoutLength = 4;
		}
		ILibDestructParserResults(p);
	}
	if (SID == NULL)
	{
		// If not SID was specified, this is a subscription request
		// Subscribe
		UPnPSubscribeEvents(header->DirectiveObj, header->DirectiveObjLength, Timeout, TimeoutLength, URL, URLLength, session);
	}
	else
	{
		// If a SID was specified, it is a renewal request for an existing subscription
		// Renew
		UPnPRenewEvents(header->DirectiveObj, header->DirectiveObjLength, SID, SIDLength, Timeout, TimeoutLength, session);
	}
}
void UPnPStreamDescriptionDocument_SCPD(struct ILibWebServer_Session *session, int StartActionList, char *buffer, int offset, int length, int DoneActionList, int Done)
{
	if (StartActionList)
	{
		ILibWebServer_StreamBody(session, "<?xml version=\"1.0\" encoding=\"utf-8\" ?><scpd xmlns=\"urn:schemas-upnp-org:service-1-0\"><specVersion><major>1</major><minor>0</minor></specVersion><actionList>", 157, ILibAsyncSocket_MemoryOwnership_STATIC, 0);
	}
	if (buffer != NULL)
	{
		ILibWebServer_StreamBody(session, buffer + offset, length, ILibAsyncSocket_MemoryOwnership_USER, 0);
	}
	if (DoneActionList)
	{
		ILibWebServer_StreamBody(session, "</actionList><serviceStateTable>", 32, ILibAsyncSocket_MemoryOwnership_STATIC, 0);
	}
	if (Done)
	{
		ILibWebServer_StreamBody(session, "</serviceStateTable></scpd>", 27, ILibAsyncSocket_MemoryOwnership_STATIC, 1);
	}
}
void UPnPProcessHTTPPacket(struct ILibWebServer_Session *session, struct packetheader* header, char *bodyBuffer, int offset, int bodyBufferLength)
{
	int i;
	#if defined(WIN32) || defined(_WIN32_WCE)
	char *responseHeader = "\r\nCONTENT-TYPE:  text/xml; charset=\"utf-8\"\r\nServer: WINDOWS, UPnP/1.0, MicroStack/1.0.5329";
	#elif defined(__SYMBIAN32__)
	char *responseHeader = "\r\nCONTENT-TYPE:  text/xml; charset=\"utf-8\"\r\nServer: SYMBIAN, UPnP/1.0, MicroStack/1.0.5329";
	#else
	char *responseHeader = "\r\nCONTENT-TYPE:  text/xml; charset=\"utf-8\"\r\nServer: POSIX, UPnP/1.0, MicroStack/1.0.5329";
	#endif
	char *errorTemplate = "HTTP/1.1 %d %s\r\nServer: %s, UPnP/1.0, MicroStack/1.0.5329\r\nContent-Length: 0\r\n\r\n";
	char *errorPacket;
	int errorPacketLength;
	char *buffer;
	
	LVL3DEBUG(errorPacketLength = ILibGetRawPacket(header, &errorPacket);)
	LVL3DEBUG(printf("%s\r\n",errorPacket);)
	LVL3DEBUG(free(errorPacket);)			
	
	
	if (header->DirectiveLength == 4 && memcmp(header->Directive,"HEAD", 4) == 0)
	{
		if (header->DirectiveObjLength == strlen(UPnPXmlLocation) && memcmp(header->DirectiveObj, UPnPXmlLocation, strlen(UPnPXmlLocation)) == 0)
		{
			// A HEAD request for the device description document.
			// We stream the document back, so we don't return content length or anything because the actual response won't have it either
			ILibWebServer_StreamHeader_Raw(session, 200, "OK", responseHeader, 1);
			ILibWebServer_StreamBody(session, NULL, 0, ILibAsyncSocket_MemoryOwnership_STATIC, 1);
		}
		else if (header->DirectiveObjLength == 9 && memcmp(header->DirectiveObj, "/icon.png", 1) == 0)
		{
			ILibWebServer_Send_Raw(session, (char*)UPnPDeviceIcon_SMPNG, 66, ILibAsyncSocket_MemoryOwnership_STATIC, 1);
		}
		else if (header->DirectiveObjLength == 9 && memcmp(header->DirectiveObj, "/icon.jpg", 1) == 0)
		{
			ILibWebServer_Send_Raw(session, (char*)UPnPDeviceIcon_SMJPG, 66, ILibAsyncSocket_MemoryOwnership_STATIC, 1);
		}
		else if (header->DirectiveObjLength==10 && memcmp(header->DirectiveObj, "/icon2.png", 1) == 0)
		{
			ILibWebServer_Send_Raw(session, (char*)UPnPDeviceIcon_LGPNG, 66, ILibAsyncSocket_MemoryOwnership_STATIC, 1);
		}
		else if (header->DirectiveObjLength==10 && memcmp(header->DirectiveObj, "/icon2.jpg", 1) == 0)
		{
			ILibWebServer_Send_Raw(session, (char*)UPnPDeviceIcon_LGJPG, 66, ILibAsyncSocket_MemoryOwnership_STATIC, 1);
		}
		else if (header->DirectiveObjLength==21 && memcmp((header->DirectiveObj)+1,"AVTransport/scpd.xml",20)==0)
		{
			ILibWebServer_StreamHeader_Raw(session,200,"OK",responseHeader,1);
			ILibWebServer_StreamBody(session,NULL,0,ILibAsyncSocket_MemoryOwnership_STATIC,1);
		}
		else if (header->DirectiveObjLength==27 && memcmp((header->DirectiveObj)+1,"ConnectionManager/scpd.xml",26)==0)
		{
			ILibWebServer_StreamHeader_Raw(session,200,"OK",responseHeader,1);
			ILibWebServer_StreamBody(session,NULL,0,ILibAsyncSocket_MemoryOwnership_STATIC,1);
		}
		else if (header->DirectiveObjLength==26 && memcmp((header->DirectiveObj)+1,"RenderingControl/scpd.xml",25)==0)
		{
			ILibWebServer_StreamHeader_Raw(session,200,"OK",responseHeader,1);
			ILibWebServer_StreamBody(session,NULL,0,ILibAsyncSocket_MemoryOwnership_STATIC,1);
		}
		
		else
		{
			// A HEAD request for something we don't have
			if ((errorPacket = (char*)malloc(128)) == NULL) ILIBCRITICALEXIT(254);
			errorPacketLength = snprintf(errorPacket, 128, errorTemplate, 404, "File Not Found", UPnPPLATFORM);
			ILibWebServer_Send_Raw(session, errorPacket, errorPacketLength, 0, 1);
		}
	}
	else if (header->DirectiveLength == 3 && memcmp(header->Directive, "GET", 3) == 0)
	{
		if (header->DirectiveObjLength == strlen(UPnPXmlLocation) && memcmp(header->DirectiveObj, UPnPXmlLocation, strlen(UPnPXmlLocation)) == 0)
		{
			// A GET Request for the device description document, so lets stream it back to the client
			UPnPStreamDescriptionDocument(session);
		}
		else if (header->DirectiveObjLength == strlen("/icon.png") && memcmp(header->DirectiveObj, "/icon.png", strlen("/icon.png")) == 0)
		{
			ILibWebServer_Send_Raw(session, (char*)UPnPDeviceIcon_SMPNG, 4058, ILibAsyncSocket_MemoryOwnership_STATIC, 1);
		}
		else if (header->DirectiveObjLength == strlen("/icon2.png") && memcmp(header->DirectiveObj, "/icon2.png", strlen("/icon2.png")) == 0)
		{
			ILibWebServer_Send_Raw(session, (char*)UPnPDeviceIcon_LGPNG, 4058, ILibAsyncSocket_MemoryOwnership_STATIC, 1);
		}
		else if (header->DirectiveObjLength == strlen("/icon.jpg") && memcmp(header->DirectiveObj, "/icon.jpg", strlen("/icon.jpg")) == 0)
		{
			ILibWebServer_Send_Raw(session, (char*)UPnPDeviceIcon_SMJPG, 1603, ILibAsyncSocket_MemoryOwnership_STATIC, 1);
		}
		else if (header->DirectiveObjLength == strlen("/icon2.jpg") && memcmp(header->DirectiveObj, "/icon2.jpg", strlen("/icon2.jpg")) == 0)
		{
			ILibWebServer_Send_Raw(session, (char*)UPnPDeviceIcon_LGJPG, 1603, ILibAsyncSocket_MemoryOwnership_STATIC, 1);
		}
		else if (header->DirectiveObjLength==21 && memcmp((header->DirectiveObj)+1,"AVTransport/scpd.xml",20)==0)
		{
			ILibWebServer_StreamHeader_Raw(session,200,"OK",responseHeader,1);
			UPnPStreamDescriptionDocument_SCPD(session,1,NULL,0,0,0,0);
			buffer = ILibDecompressString((unsigned char*)UPnP_ActionTable_AVTransport_Impl.Reserved,UPnP_ActionTable_AVTransport_Impl.ReservedXL,UPnP_ActionTable_AVTransport_Impl.ReservedUXL);
			if (UPnP_Device_MediaRenderer_Impl.AVTransport->GetCurrentTransportActions!=NULL){UPnPStreamDescriptionDocument_SCPD(session,0,buffer,UPnP_Device_MediaRenderer_Impl.AVTransport->GetCurrentTransportActions->Reserved,UPnP_Device_MediaRenderer_Impl.AVTransport->GetCurrentTransportActions->Reserved2,0,0);}
			if (UPnP_Device_MediaRenderer_Impl.AVTransport->GetDeviceCapabilities!=NULL){UPnPStreamDescriptionDocument_SCPD(session,0,buffer,UPnP_Device_MediaRenderer_Impl.AVTransport->GetDeviceCapabilities->Reserved,UPnP_Device_MediaRenderer_Impl.AVTransport->GetDeviceCapabilities->Reserved2,0,0);}
			if (UPnP_Device_MediaRenderer_Impl.AVTransport->GetMediaInfo!=NULL){UPnPStreamDescriptionDocument_SCPD(session,0,buffer,UPnP_Device_MediaRenderer_Impl.AVTransport->GetMediaInfo->Reserved,UPnP_Device_MediaRenderer_Impl.AVTransport->GetMediaInfo->Reserved2,0,0);}
			if (UPnP_Device_MediaRenderer_Impl.AVTransport->GetPositionInfo!=NULL){UPnPStreamDescriptionDocument_SCPD(session,0,buffer,UPnP_Device_MediaRenderer_Impl.AVTransport->GetPositionInfo->Reserved,UPnP_Device_MediaRenderer_Impl.AVTransport->GetPositionInfo->Reserved2,0,0);}
			if (UPnP_Device_MediaRenderer_Impl.AVTransport->GetTransportInfo!=NULL){UPnPStreamDescriptionDocument_SCPD(session,0,buffer,UPnP_Device_MediaRenderer_Impl.AVTransport->GetTransportInfo->Reserved,UPnP_Device_MediaRenderer_Impl.AVTransport->GetTransportInfo->Reserved2,0,0);}
			if (UPnP_Device_MediaRenderer_Impl.AVTransport->GetTransportSettings!=NULL){UPnPStreamDescriptionDocument_SCPD(session,0,buffer,UPnP_Device_MediaRenderer_Impl.AVTransport->GetTransportSettings->Reserved,UPnP_Device_MediaRenderer_Impl.AVTransport->GetTransportSettings->Reserved2,0,0);}
			if (UPnP_Device_MediaRenderer_Impl.AVTransport->Next!=NULL){UPnPStreamDescriptionDocument_SCPD(session,0,buffer,UPnP_Device_MediaRenderer_Impl.AVTransport->Next->Reserved,UPnP_Device_MediaRenderer_Impl.AVTransport->Next->Reserved2,0,0);}
			if (UPnP_Device_MediaRenderer_Impl.AVTransport->Pause!=NULL){UPnPStreamDescriptionDocument_SCPD(session,0,buffer,UPnP_Device_MediaRenderer_Impl.AVTransport->Pause->Reserved,UPnP_Device_MediaRenderer_Impl.AVTransport->Pause->Reserved2,0,0);}
			if (UPnP_Device_MediaRenderer_Impl.AVTransport->Play!=NULL){UPnPStreamDescriptionDocument_SCPD(session,0,buffer,UPnP_Device_MediaRenderer_Impl.AVTransport->Play->Reserved,UPnP_Device_MediaRenderer_Impl.AVTransport->Play->Reserved2,0,0);}
			if (UPnP_Device_MediaRenderer_Impl.AVTransport->Previous!=NULL){UPnPStreamDescriptionDocument_SCPD(session,0,buffer,UPnP_Device_MediaRenderer_Impl.AVTransport->Previous->Reserved,UPnP_Device_MediaRenderer_Impl.AVTransport->Previous->Reserved2,0,0);}
			if (UPnP_Device_MediaRenderer_Impl.AVTransport->Seek!=NULL){UPnPStreamDescriptionDocument_SCPD(session,0,buffer,UPnP_Device_MediaRenderer_Impl.AVTransport->Seek->Reserved,UPnP_Device_MediaRenderer_Impl.AVTransport->Seek->Reserved2,0,0);}
			if (UPnP_Device_MediaRenderer_Impl.AVTransport->SetAVTransportURI!=NULL){UPnPStreamDescriptionDocument_SCPD(session,0,buffer,UPnP_Device_MediaRenderer_Impl.AVTransport->SetAVTransportURI->Reserved,UPnP_Device_MediaRenderer_Impl.AVTransport->SetAVTransportURI->Reserved2,0,0);}
			if (UPnP_Device_MediaRenderer_Impl.AVTransport->SetPlayMode!=NULL){UPnPStreamDescriptionDocument_SCPD(session,0,buffer,UPnP_Device_MediaRenderer_Impl.AVTransport->SetPlayMode->Reserved,UPnP_Device_MediaRenderer_Impl.AVTransport->SetPlayMode->Reserved2,0,0);}
			if (UPnP_Device_MediaRenderer_Impl.AVTransport->Stop!=NULL){UPnPStreamDescriptionDocument_SCPD(session,0,buffer,UPnP_Device_MediaRenderer_Impl.AVTransport->Stop->Reserved,UPnP_Device_MediaRenderer_Impl.AVTransport->Stop->Reserved2,0,0);}
			free(buffer);
			UPnPStreamDescriptionDocument_SCPD(session,0,NULL,0,0,1,0);
			buffer = ILibDecompressString((unsigned char*)UPnP_StateVariableTable_AVTransport_Impl.Reserved,UPnP_StateVariableTable_AVTransport_Impl.ReservedXL,UPnP_StateVariableTable_AVTransport_Impl.ReservedUXL);
			if (UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_A_ARG_TYPE_SeekMode!=NULL)
			{
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_A_ARG_TYPE_SeekMode->Reserved1,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_A_ARG_TYPE_SeekMode->Reserved1L,ILibAsyncSocket_MemoryOwnership_USER,0);
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_A_ARG_TYPE_SeekMode->Reserved2,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_A_ARG_TYPE_SeekMode->Reserved2L,ILibAsyncSocket_MemoryOwnership_USER,0);
				for(i=0;i<UPnP_StateVariable_AllowedValues_MAX;++i)
				{
					if (UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_A_ARG_TYPE_SeekMode->AllowedValues[i]!=NULL)
					{
						ILibWebServer_StreamBody(session,"<allowedValue>",14,ILibAsyncSocket_MemoryOwnership_STATIC,0);
						ILibWebServer_StreamBody(session,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_A_ARG_TYPE_SeekMode->AllowedValues[i],(int)strlen(UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_A_ARG_TYPE_SeekMode->AllowedValues[i]),ILibAsyncSocket_MemoryOwnership_USER,0);
						ILibWebServer_StreamBody(session,"</allowedValue>",15,ILibAsyncSocket_MemoryOwnership_STATIC,0);
					}
				}
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_A_ARG_TYPE_SeekMode->Reserved3,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_A_ARG_TYPE_SeekMode->Reserved3L,ILibAsyncSocket_MemoryOwnership_USER,0);
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_A_ARG_TYPE_SeekMode->Reserved8,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_A_ARG_TYPE_SeekMode->Reserved8L,ILibAsyncSocket_MemoryOwnership_USER,0);
			}
			if (UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_NextAVTransportURI!=NULL)
			{
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_NextAVTransportURI->Reserved1,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_NextAVTransportURI->Reserved1L,ILibAsyncSocket_MemoryOwnership_USER,0);
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_NextAVTransportURI->Reserved8,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_NextAVTransportURI->Reserved8L,ILibAsyncSocket_MemoryOwnership_USER,0);
			}
			if (UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_PlaybackStorageMedium!=NULL)
			{
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_PlaybackStorageMedium->Reserved1,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_PlaybackStorageMedium->Reserved1L,ILibAsyncSocket_MemoryOwnership_USER,0);
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_PlaybackStorageMedium->Reserved2,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_PlaybackStorageMedium->Reserved2L,ILibAsyncSocket_MemoryOwnership_USER,0);
				for(i=0;i<UPnP_StateVariable_AllowedValues_MAX;++i)
				{
					if (UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_PlaybackStorageMedium->AllowedValues[i]!=NULL)
					{
						ILibWebServer_StreamBody(session,"<allowedValue>",14,ILibAsyncSocket_MemoryOwnership_STATIC,0);
						ILibWebServer_StreamBody(session,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_PlaybackStorageMedium->AllowedValues[i],(int)strlen(UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_PlaybackStorageMedium->AllowedValues[i]),ILibAsyncSocket_MemoryOwnership_USER,0);
						ILibWebServer_StreamBody(session,"</allowedValue>",15,ILibAsyncSocket_MemoryOwnership_STATIC,0);
					}
				}
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_PlaybackStorageMedium->Reserved3,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_PlaybackStorageMedium->Reserved3L,ILibAsyncSocket_MemoryOwnership_USER,0);
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_PlaybackStorageMedium->Reserved8,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_PlaybackStorageMedium->Reserved8L,ILibAsyncSocket_MemoryOwnership_USER,0);
			}
			if (UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_RecordStorageMedium!=NULL)
			{
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_RecordStorageMedium->Reserved1,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_RecordStorageMedium->Reserved1L,ILibAsyncSocket_MemoryOwnership_USER,0);
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_RecordStorageMedium->Reserved2,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_RecordStorageMedium->Reserved2L,ILibAsyncSocket_MemoryOwnership_USER,0);
				for(i=0;i<UPnP_StateVariable_AllowedValues_MAX;++i)
				{
					if (UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_RecordStorageMedium->AllowedValues[i]!=NULL)
					{
						ILibWebServer_StreamBody(session,"<allowedValue>",14,ILibAsyncSocket_MemoryOwnership_STATIC,0);
						ILibWebServer_StreamBody(session,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_RecordStorageMedium->AllowedValues[i],(int)strlen(UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_RecordStorageMedium->AllowedValues[i]),ILibAsyncSocket_MemoryOwnership_USER,0);
						ILibWebServer_StreamBody(session,"</allowedValue>",15,ILibAsyncSocket_MemoryOwnership_STATIC,0);
					}
				}
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_RecordStorageMedium->Reserved3,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_RecordStorageMedium->Reserved3L,ILibAsyncSocket_MemoryOwnership_USER,0);
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_RecordStorageMedium->Reserved8,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_RecordStorageMedium->Reserved8L,ILibAsyncSocket_MemoryOwnership_USER,0);
			}
			if (UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_A_ARG_TYPE_InstanceID!=NULL)
			{
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_A_ARG_TYPE_InstanceID->Reserved1,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_A_ARG_TYPE_InstanceID->Reserved1L,ILibAsyncSocket_MemoryOwnership_USER,0);
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_A_ARG_TYPE_InstanceID->Reserved8,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_A_ARG_TYPE_InstanceID->Reserved8L,ILibAsyncSocket_MemoryOwnership_USER,0);
			}
			if (UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_CurrentTrack!=NULL)
			{
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_CurrentTrack->Reserved1,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_CurrentTrack->Reserved1L,ILibAsyncSocket_MemoryOwnership_USER,0);
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_CurrentTrack->Reserved4,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_CurrentTrack->Reserved4L,ILibAsyncSocket_MemoryOwnership_USER,0);
				if (UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_CurrentTrack->MinMaxStep[0]!=NULL)
				{
					ILibWebServer_StreamBody(session,"<minimum>",9,ILibAsyncSocket_MemoryOwnership_STATIC,0);
					ILibWebServer_StreamBody(session,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_CurrentTrack->MinMaxStep[0],(int)strlen(UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_CurrentTrack->MinMaxStep[0]),ILibAsyncSocket_MemoryOwnership_USER,0);
					ILibWebServer_StreamBody(session,"</minimum>",10,ILibAsyncSocket_MemoryOwnership_STATIC,0);
				}
				if (UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_CurrentTrack->MinMaxStep[1]!=NULL)
				{
					ILibWebServer_StreamBody(session,"<maximum>",9,ILibAsyncSocket_MemoryOwnership_STATIC,0);
					ILibWebServer_StreamBody(session,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_CurrentTrack->MinMaxStep[1],(int)strlen(UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_CurrentTrack->MinMaxStep[1]),ILibAsyncSocket_MemoryOwnership_USER,0);
					ILibWebServer_StreamBody(session,"</maximum>",10,ILibAsyncSocket_MemoryOwnership_STATIC,0);
				}
				if (UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_CurrentTrack->MinMaxStep[2]!=NULL)
				{
					ILibWebServer_StreamBody(session,"<step>",6,ILibAsyncSocket_MemoryOwnership_STATIC,0);
					ILibWebServer_StreamBody(session,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_CurrentTrack->MinMaxStep[2],(int)strlen(UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_CurrentTrack->MinMaxStep[2]),ILibAsyncSocket_MemoryOwnership_USER,0);
					ILibWebServer_StreamBody(session,"</step>",7,ILibAsyncSocket_MemoryOwnership_STATIC,0);
				}
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_CurrentTrack->Reserved5,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_CurrentTrack->Reserved5L,ILibAsyncSocket_MemoryOwnership_USER,0);
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_CurrentTrack->Reserved8,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_CurrentTrack->Reserved8L,ILibAsyncSocket_MemoryOwnership_USER,0);
			}
			if (UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_CurrentTrackDuration!=NULL)
			{
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_CurrentTrackDuration->Reserved1,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_CurrentTrackDuration->Reserved1L,ILibAsyncSocket_MemoryOwnership_USER,0);
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_CurrentTrackDuration->Reserved8,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_CurrentTrackDuration->Reserved8L,ILibAsyncSocket_MemoryOwnership_USER,0);
			}
			if (UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_PossiblePlaybackStorageMedia!=NULL)
			{
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_PossiblePlaybackStorageMedia->Reserved1,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_PossiblePlaybackStorageMedia->Reserved1L,ILibAsyncSocket_MemoryOwnership_USER,0);
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_PossiblePlaybackStorageMedia->Reserved2,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_PossiblePlaybackStorageMedia->Reserved2L,ILibAsyncSocket_MemoryOwnership_USER,0);
				for(i=0;i<UPnP_StateVariable_AllowedValues_MAX;++i)
				{
					if (UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_PossiblePlaybackStorageMedia->AllowedValues[i]!=NULL)
					{
						ILibWebServer_StreamBody(session,"<allowedValue>",14,ILibAsyncSocket_MemoryOwnership_STATIC,0);
						ILibWebServer_StreamBody(session,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_PossiblePlaybackStorageMedia->AllowedValues[i],(int)strlen(UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_PossiblePlaybackStorageMedia->AllowedValues[i]),ILibAsyncSocket_MemoryOwnership_USER,0);
						ILibWebServer_StreamBody(session,"</allowedValue>",15,ILibAsyncSocket_MemoryOwnership_STATIC,0);
					}
				}
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_PossiblePlaybackStorageMedia->Reserved3,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_PossiblePlaybackStorageMedia->Reserved3L,ILibAsyncSocket_MemoryOwnership_USER,0);
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_PossiblePlaybackStorageMedia->Reserved8,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_PossiblePlaybackStorageMedia->Reserved8L,ILibAsyncSocket_MemoryOwnership_USER,0);
			}
			if (UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_CurrentPlayMode!=NULL)
			{
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_CurrentPlayMode->Reserved1,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_CurrentPlayMode->Reserved1L,ILibAsyncSocket_MemoryOwnership_USER,0);
				if (UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_CurrentPlayMode->DefaultValue!=NULL)
				{
					ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_CurrentPlayMode->Reserved6,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_CurrentPlayMode->Reserved6L,ILibAsyncSocket_MemoryOwnership_USER,0);
					ILibWebServer_StreamBody(session,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_CurrentPlayMode->DefaultValue,(int)strlen(UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_CurrentPlayMode->DefaultValue),ILibAsyncSocket_MemoryOwnership_USER,0);
					ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_CurrentPlayMode->Reserved7,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_CurrentPlayMode->Reserved7L,ILibAsyncSocket_MemoryOwnership_USER,0);
				}
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_CurrentPlayMode->Reserved2,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_CurrentPlayMode->Reserved2L,ILibAsyncSocket_MemoryOwnership_USER,0);
				for(i=0;i<UPnP_StateVariable_AllowedValues_MAX;++i)
				{
					if (UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_CurrentPlayMode->AllowedValues[i]!=NULL)
					{
						ILibWebServer_StreamBody(session,"<allowedValue>",14,ILibAsyncSocket_MemoryOwnership_STATIC,0);
						ILibWebServer_StreamBody(session,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_CurrentPlayMode->AllowedValues[i],(int)strlen(UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_CurrentPlayMode->AllowedValues[i]),ILibAsyncSocket_MemoryOwnership_USER,0);
						ILibWebServer_StreamBody(session,"</allowedValue>",15,ILibAsyncSocket_MemoryOwnership_STATIC,0);
					}
				}
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_CurrentPlayMode->Reserved3,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_CurrentPlayMode->Reserved3L,ILibAsyncSocket_MemoryOwnership_USER,0);
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_CurrentPlayMode->Reserved8,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_CurrentPlayMode->Reserved8L,ILibAsyncSocket_MemoryOwnership_USER,0);
			}
			if (UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_TransportStatus!=NULL)
			{
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_TransportStatus->Reserved1,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_TransportStatus->Reserved1L,ILibAsyncSocket_MemoryOwnership_USER,0);
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_TransportStatus->Reserved2,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_TransportStatus->Reserved2L,ILibAsyncSocket_MemoryOwnership_USER,0);
				for(i=0;i<UPnP_StateVariable_AllowedValues_MAX;++i)
				{
					if (UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_TransportStatus->AllowedValues[i]!=NULL)
					{
						ILibWebServer_StreamBody(session,"<allowedValue>",14,ILibAsyncSocket_MemoryOwnership_STATIC,0);
						ILibWebServer_StreamBody(session,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_TransportStatus->AllowedValues[i],(int)strlen(UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_TransportStatus->AllowedValues[i]),ILibAsyncSocket_MemoryOwnership_USER,0);
						ILibWebServer_StreamBody(session,"</allowedValue>",15,ILibAsyncSocket_MemoryOwnership_STATIC,0);
					}
				}
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_TransportStatus->Reserved3,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_TransportStatus->Reserved3L,ILibAsyncSocket_MemoryOwnership_USER,0);
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_TransportStatus->Reserved8,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_TransportStatus->Reserved8L,ILibAsyncSocket_MemoryOwnership_USER,0);
			}
			if (UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_CurrentRecordQualityMode!=NULL)
			{
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_CurrentRecordQualityMode->Reserved1,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_CurrentRecordQualityMode->Reserved1L,ILibAsyncSocket_MemoryOwnership_USER,0);
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_CurrentRecordQualityMode->Reserved2,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_CurrentRecordQualityMode->Reserved2L,ILibAsyncSocket_MemoryOwnership_USER,0);
				for(i=0;i<UPnP_StateVariable_AllowedValues_MAX;++i)
				{
					if (UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_CurrentRecordQualityMode->AllowedValues[i]!=NULL)
					{
						ILibWebServer_StreamBody(session,"<allowedValue>",14,ILibAsyncSocket_MemoryOwnership_STATIC,0);
						ILibWebServer_StreamBody(session,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_CurrentRecordQualityMode->AllowedValues[i],(int)strlen(UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_CurrentRecordQualityMode->AllowedValues[i]),ILibAsyncSocket_MemoryOwnership_USER,0);
						ILibWebServer_StreamBody(session,"</allowedValue>",15,ILibAsyncSocket_MemoryOwnership_STATIC,0);
					}
				}
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_CurrentRecordQualityMode->Reserved3,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_CurrentRecordQualityMode->Reserved3L,ILibAsyncSocket_MemoryOwnership_USER,0);
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_CurrentRecordQualityMode->Reserved8,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_CurrentRecordQualityMode->Reserved8L,ILibAsyncSocket_MemoryOwnership_USER,0);
			}
			if (UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_TransportState!=NULL)
			{
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_TransportState->Reserved1,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_TransportState->Reserved1L,ILibAsyncSocket_MemoryOwnership_USER,0);
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_TransportState->Reserved2,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_TransportState->Reserved2L,ILibAsyncSocket_MemoryOwnership_USER,0);
				for(i=0;i<UPnP_StateVariable_AllowedValues_MAX;++i)
				{
					if (UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_TransportState->AllowedValues[i]!=NULL)
					{
						ILibWebServer_StreamBody(session,"<allowedValue>",14,ILibAsyncSocket_MemoryOwnership_STATIC,0);
						ILibWebServer_StreamBody(session,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_TransportState->AllowedValues[i],(int)strlen(UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_TransportState->AllowedValues[i]),ILibAsyncSocket_MemoryOwnership_USER,0);
						ILibWebServer_StreamBody(session,"</allowedValue>",15,ILibAsyncSocket_MemoryOwnership_STATIC,0);
					}
				}
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_TransportState->Reserved3,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_TransportState->Reserved3L,ILibAsyncSocket_MemoryOwnership_USER,0);
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_TransportState->Reserved8,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_TransportState->Reserved8L,ILibAsyncSocket_MemoryOwnership_USER,0);
			}
			if (UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_CurrentTrackURI!=NULL)
			{
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_CurrentTrackURI->Reserved1,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_CurrentTrackURI->Reserved1L,ILibAsyncSocket_MemoryOwnership_USER,0);
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_CurrentTrackURI->Reserved8,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_CurrentTrackURI->Reserved8L,ILibAsyncSocket_MemoryOwnership_USER,0);
			}
			if (UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_AVTransportURI!=NULL)
			{
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_AVTransportURI->Reserved1,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_AVTransportURI->Reserved1L,ILibAsyncSocket_MemoryOwnership_USER,0);
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_AVTransportURI->Reserved8,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_AVTransportURI->Reserved8L,ILibAsyncSocket_MemoryOwnership_USER,0);
			}
			if (UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_AbsoluteCounterPosition!=NULL)
			{
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_AbsoluteCounterPosition->Reserved1,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_AbsoluteCounterPosition->Reserved1L,ILibAsyncSocket_MemoryOwnership_USER,0);
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_AbsoluteCounterPosition->Reserved8,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_AbsoluteCounterPosition->Reserved8L,ILibAsyncSocket_MemoryOwnership_USER,0);
			}
			if (UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_PossibleRecordQualityModes!=NULL)
			{
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_PossibleRecordQualityModes->Reserved1,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_PossibleRecordQualityModes->Reserved1L,ILibAsyncSocket_MemoryOwnership_USER,0);
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_PossibleRecordQualityModes->Reserved2,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_PossibleRecordQualityModes->Reserved2L,ILibAsyncSocket_MemoryOwnership_USER,0);
				for(i=0;i<UPnP_StateVariable_AllowedValues_MAX;++i)
				{
					if (UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_PossibleRecordQualityModes->AllowedValues[i]!=NULL)
					{
						ILibWebServer_StreamBody(session,"<allowedValue>",14,ILibAsyncSocket_MemoryOwnership_STATIC,0);
						ILibWebServer_StreamBody(session,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_PossibleRecordQualityModes->AllowedValues[i],(int)strlen(UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_PossibleRecordQualityModes->AllowedValues[i]),ILibAsyncSocket_MemoryOwnership_USER,0);
						ILibWebServer_StreamBody(session,"</allowedValue>",15,ILibAsyncSocket_MemoryOwnership_STATIC,0);
					}
				}
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_PossibleRecordQualityModes->Reserved3,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_PossibleRecordQualityModes->Reserved3L,ILibAsyncSocket_MemoryOwnership_USER,0);
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_PossibleRecordQualityModes->Reserved8,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_PossibleRecordQualityModes->Reserved8L,ILibAsyncSocket_MemoryOwnership_USER,0);
			}
			if (UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_LastChange!=NULL)
			{
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_LastChange->Reserved1,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_LastChange->Reserved1L,ILibAsyncSocket_MemoryOwnership_USER,0);
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_LastChange->Reserved8,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_LastChange->Reserved8L,ILibAsyncSocket_MemoryOwnership_USER,0);
			}
			if (UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_NextAVTransportURIMetaData!=NULL)
			{
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_NextAVTransportURIMetaData->Reserved1,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_NextAVTransportURIMetaData->Reserved1L,ILibAsyncSocket_MemoryOwnership_USER,0);
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_NextAVTransportURIMetaData->Reserved8,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_NextAVTransportURIMetaData->Reserved8L,ILibAsyncSocket_MemoryOwnership_USER,0);
			}
			if (UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_AVTransportURIMetaData!=NULL)
			{
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_AVTransportURIMetaData->Reserved1,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_AVTransportURIMetaData->Reserved1L,ILibAsyncSocket_MemoryOwnership_USER,0);
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_AVTransportURIMetaData->Reserved8,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_AVTransportURIMetaData->Reserved8L,ILibAsyncSocket_MemoryOwnership_USER,0);
			}
			if (UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_A_ARG_TYPE_SeekTarget!=NULL)
			{
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_A_ARG_TYPE_SeekTarget->Reserved1,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_A_ARG_TYPE_SeekTarget->Reserved1L,ILibAsyncSocket_MemoryOwnership_USER,0);
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_A_ARG_TYPE_SeekTarget->Reserved8,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_A_ARG_TYPE_SeekTarget->Reserved8L,ILibAsyncSocket_MemoryOwnership_USER,0);
			}
			if (UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_AbsoluteTimePosition!=NULL)
			{
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_AbsoluteTimePosition->Reserved1,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_AbsoluteTimePosition->Reserved1L,ILibAsyncSocket_MemoryOwnership_USER,0);
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_AbsoluteTimePosition->Reserved8,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_AbsoluteTimePosition->Reserved8L,ILibAsyncSocket_MemoryOwnership_USER,0);
			}
			if (UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_RecordMediumWriteStatus!=NULL)
			{
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_RecordMediumWriteStatus->Reserved1,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_RecordMediumWriteStatus->Reserved1L,ILibAsyncSocket_MemoryOwnership_USER,0);
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_RecordMediumWriteStatus->Reserved2,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_RecordMediumWriteStatus->Reserved2L,ILibAsyncSocket_MemoryOwnership_USER,0);
				for(i=0;i<UPnP_StateVariable_AllowedValues_MAX;++i)
				{
					if (UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_RecordMediumWriteStatus->AllowedValues[i]!=NULL)
					{
						ILibWebServer_StreamBody(session,"<allowedValue>",14,ILibAsyncSocket_MemoryOwnership_STATIC,0);
						ILibWebServer_StreamBody(session,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_RecordMediumWriteStatus->AllowedValues[i],(int)strlen(UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_RecordMediumWriteStatus->AllowedValues[i]),ILibAsyncSocket_MemoryOwnership_USER,0);
						ILibWebServer_StreamBody(session,"</allowedValue>",15,ILibAsyncSocket_MemoryOwnership_STATIC,0);
					}
				}
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_RecordMediumWriteStatus->Reserved3,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_RecordMediumWriteStatus->Reserved3L,ILibAsyncSocket_MemoryOwnership_USER,0);
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_RecordMediumWriteStatus->Reserved8,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_RecordMediumWriteStatus->Reserved8L,ILibAsyncSocket_MemoryOwnership_USER,0);
			}
			if (UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_CurrentTransportActions!=NULL)
			{
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_CurrentTransportActions->Reserved1,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_CurrentTransportActions->Reserved1L,ILibAsyncSocket_MemoryOwnership_USER,0);
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_CurrentTransportActions->Reserved8,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_CurrentTransportActions->Reserved8L,ILibAsyncSocket_MemoryOwnership_USER,0);
			}
			if (UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_RelativeTimePosition!=NULL)
			{
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_RelativeTimePosition->Reserved1,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_RelativeTimePosition->Reserved1L,ILibAsyncSocket_MemoryOwnership_USER,0);
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_RelativeTimePosition->Reserved8,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_RelativeTimePosition->Reserved8L,ILibAsyncSocket_MemoryOwnership_USER,0);
			}
			if (UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_CurrentTrackMetaData!=NULL)
			{
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_CurrentTrackMetaData->Reserved1,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_CurrentTrackMetaData->Reserved1L,ILibAsyncSocket_MemoryOwnership_USER,0);
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_CurrentTrackMetaData->Reserved8,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_CurrentTrackMetaData->Reserved8L,ILibAsyncSocket_MemoryOwnership_USER,0);
			}
			if (UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_CurrentMediaDuration!=NULL)
			{
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_CurrentMediaDuration->Reserved1,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_CurrentMediaDuration->Reserved1L,ILibAsyncSocket_MemoryOwnership_USER,0);
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_CurrentMediaDuration->Reserved8,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_CurrentMediaDuration->Reserved8L,ILibAsyncSocket_MemoryOwnership_USER,0);
			}
			if (UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_TransportPlaySpeed!=NULL)
			{
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_TransportPlaySpeed->Reserved1,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_TransportPlaySpeed->Reserved1L,ILibAsyncSocket_MemoryOwnership_USER,0);
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_TransportPlaySpeed->Reserved2,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_TransportPlaySpeed->Reserved2L,ILibAsyncSocket_MemoryOwnership_USER,0);
				for(i=0;i<UPnP_StateVariable_AllowedValues_MAX;++i)
				{
					if (UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_TransportPlaySpeed->AllowedValues[i]!=NULL)
					{
						ILibWebServer_StreamBody(session,"<allowedValue>",14,ILibAsyncSocket_MemoryOwnership_STATIC,0);
						ILibWebServer_StreamBody(session,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_TransportPlaySpeed->AllowedValues[i],(int)strlen(UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_TransportPlaySpeed->AllowedValues[i]),ILibAsyncSocket_MemoryOwnership_USER,0);
						ILibWebServer_StreamBody(session,"</allowedValue>",15,ILibAsyncSocket_MemoryOwnership_STATIC,0);
					}
				}
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_TransportPlaySpeed->Reserved3,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_TransportPlaySpeed->Reserved3L,ILibAsyncSocket_MemoryOwnership_USER,0);
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_TransportPlaySpeed->Reserved8,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_TransportPlaySpeed->Reserved8L,ILibAsyncSocket_MemoryOwnership_USER,0);
			}
			if (UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_RelativeCounterPosition!=NULL)
			{
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_RelativeCounterPosition->Reserved1,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_RelativeCounterPosition->Reserved1L,ILibAsyncSocket_MemoryOwnership_USER,0);
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_RelativeCounterPosition->Reserved8,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_RelativeCounterPosition->Reserved8L,ILibAsyncSocket_MemoryOwnership_USER,0);
			}
			if (UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_NumberOfTracks!=NULL)
			{
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_NumberOfTracks->Reserved1,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_NumberOfTracks->Reserved1L,ILibAsyncSocket_MemoryOwnership_USER,0);
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_NumberOfTracks->Reserved4,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_NumberOfTracks->Reserved4L,ILibAsyncSocket_MemoryOwnership_USER,0);
				if (UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_NumberOfTracks->MinMaxStep[0]!=NULL)
				{
					ILibWebServer_StreamBody(session,"<minimum>",9,ILibAsyncSocket_MemoryOwnership_STATIC,0);
					ILibWebServer_StreamBody(session,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_NumberOfTracks->MinMaxStep[0],(int)strlen(UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_NumberOfTracks->MinMaxStep[0]),ILibAsyncSocket_MemoryOwnership_USER,0);
					ILibWebServer_StreamBody(session,"</minimum>",10,ILibAsyncSocket_MemoryOwnership_STATIC,0);
				}
				if (UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_NumberOfTracks->MinMaxStep[1]!=NULL)
				{
					ILibWebServer_StreamBody(session,"<maximum>",9,ILibAsyncSocket_MemoryOwnership_STATIC,0);
					ILibWebServer_StreamBody(session,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_NumberOfTracks->MinMaxStep[1],(int)strlen(UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_NumberOfTracks->MinMaxStep[1]),ILibAsyncSocket_MemoryOwnership_USER,0);
					ILibWebServer_StreamBody(session,"</maximum>",10,ILibAsyncSocket_MemoryOwnership_STATIC,0);
				}
				if (UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_NumberOfTracks->MinMaxStep[2]!=NULL)
				{
					ILibWebServer_StreamBody(session,"<step>",6,ILibAsyncSocket_MemoryOwnership_STATIC,0);
					ILibWebServer_StreamBody(session,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_NumberOfTracks->MinMaxStep[2],(int)strlen(UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_NumberOfTracks->MinMaxStep[2]),ILibAsyncSocket_MemoryOwnership_USER,0);
					ILibWebServer_StreamBody(session,"</step>",7,ILibAsyncSocket_MemoryOwnership_STATIC,0);
				}
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_NumberOfTracks->Reserved5,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_NumberOfTracks->Reserved5L,ILibAsyncSocket_MemoryOwnership_USER,0);
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_NumberOfTracks->Reserved8,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_NumberOfTracks->Reserved8L,ILibAsyncSocket_MemoryOwnership_USER,0);
			}
			if (UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_PossibleRecordStorageMedia!=NULL)
			{
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_PossibleRecordStorageMedia->Reserved1,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_PossibleRecordStorageMedia->Reserved1L,ILibAsyncSocket_MemoryOwnership_USER,0);
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_PossibleRecordStorageMedia->Reserved2,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_PossibleRecordStorageMedia->Reserved2L,ILibAsyncSocket_MemoryOwnership_USER,0);
				for(i=0;i<UPnP_StateVariable_AllowedValues_MAX;++i)
				{
					if (UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_PossibleRecordStorageMedia->AllowedValues[i]!=NULL)
					{
						ILibWebServer_StreamBody(session,"<allowedValue>",14,ILibAsyncSocket_MemoryOwnership_STATIC,0);
						ILibWebServer_StreamBody(session,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_PossibleRecordStorageMedia->AllowedValues[i],(int)strlen(UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_PossibleRecordStorageMedia->AllowedValues[i]),ILibAsyncSocket_MemoryOwnership_USER,0);
						ILibWebServer_StreamBody(session,"</allowedValue>",15,ILibAsyncSocket_MemoryOwnership_STATIC,0);
					}
				}
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_PossibleRecordStorageMedia->Reserved3,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_PossibleRecordStorageMedia->Reserved3L,ILibAsyncSocket_MemoryOwnership_USER,0);
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_PossibleRecordStorageMedia->Reserved8,UPnP_Device_MediaRenderer_Impl.AVTransport->StateVar_PossibleRecordStorageMedia->Reserved8L,ILibAsyncSocket_MemoryOwnership_USER,0);
			}
			free(buffer);
			UPnPStreamDescriptionDocument_SCPD(session,0,NULL,0,0,0,1);
		}
		else if (header->DirectiveObjLength==27 && memcmp((header->DirectiveObj)+1,"ConnectionManager/scpd.xml",26)==0)
		{
			ILibWebServer_StreamHeader_Raw(session,200,"OK",responseHeader,1);
			UPnPStreamDescriptionDocument_SCPD(session,1,NULL,0,0,0,0);
			buffer = ILibDecompressString((unsigned char*)UPnP_ActionTable_ConnectionManager_Impl.Reserved,UPnP_ActionTable_ConnectionManager_Impl.ReservedXL,UPnP_ActionTable_ConnectionManager_Impl.ReservedUXL);
			if (UPnP_Device_MediaRenderer_Impl.ConnectionManager->GetCurrentConnectionIDs!=NULL){UPnPStreamDescriptionDocument_SCPD(session,0,buffer,UPnP_Device_MediaRenderer_Impl.ConnectionManager->GetCurrentConnectionIDs->Reserved,UPnP_Device_MediaRenderer_Impl.ConnectionManager->GetCurrentConnectionIDs->Reserved2,0,0);}
			if (UPnP_Device_MediaRenderer_Impl.ConnectionManager->GetCurrentConnectionInfo!=NULL){UPnPStreamDescriptionDocument_SCPD(session,0,buffer,UPnP_Device_MediaRenderer_Impl.ConnectionManager->GetCurrentConnectionInfo->Reserved,UPnP_Device_MediaRenderer_Impl.ConnectionManager->GetCurrentConnectionInfo->Reserved2,0,0);}
			if (UPnP_Device_MediaRenderer_Impl.ConnectionManager->GetProtocolInfo!=NULL){UPnPStreamDescriptionDocument_SCPD(session,0,buffer,UPnP_Device_MediaRenderer_Impl.ConnectionManager->GetProtocolInfo->Reserved,UPnP_Device_MediaRenderer_Impl.ConnectionManager->GetProtocolInfo->Reserved2,0,0);}
			free(buffer);
			UPnPStreamDescriptionDocument_SCPD(session,0,NULL,0,0,1,0);
			buffer = ILibDecompressString((unsigned char*)UPnP_StateVariableTable_ConnectionManager_Impl.Reserved,UPnP_StateVariableTable_ConnectionManager_Impl.ReservedXL,UPnP_StateVariableTable_ConnectionManager_Impl.ReservedUXL);
			if (UPnP_Device_MediaRenderer_Impl.ConnectionManager->StateVar_A_ARG_TYPE_ConnectionStatus!=NULL)
			{
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.ConnectionManager->StateVar_A_ARG_TYPE_ConnectionStatus->Reserved1,UPnP_Device_MediaRenderer_Impl.ConnectionManager->StateVar_A_ARG_TYPE_ConnectionStatus->Reserved1L,ILibAsyncSocket_MemoryOwnership_USER,0);
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.ConnectionManager->StateVar_A_ARG_TYPE_ConnectionStatus->Reserved2,UPnP_Device_MediaRenderer_Impl.ConnectionManager->StateVar_A_ARG_TYPE_ConnectionStatus->Reserved2L,ILibAsyncSocket_MemoryOwnership_USER,0);
				for(i=0;i<UPnP_StateVariable_AllowedValues_MAX;++i)
				{
					if (UPnP_Device_MediaRenderer_Impl.ConnectionManager->StateVar_A_ARG_TYPE_ConnectionStatus->AllowedValues[i]!=NULL)
					{
						ILibWebServer_StreamBody(session,"<allowedValue>",14,ILibAsyncSocket_MemoryOwnership_STATIC,0);
						ILibWebServer_StreamBody(session,UPnP_Device_MediaRenderer_Impl.ConnectionManager->StateVar_A_ARG_TYPE_ConnectionStatus->AllowedValues[i],(int)strlen(UPnP_Device_MediaRenderer_Impl.ConnectionManager->StateVar_A_ARG_TYPE_ConnectionStatus->AllowedValues[i]),ILibAsyncSocket_MemoryOwnership_USER,0);
						ILibWebServer_StreamBody(session,"</allowedValue>",15,ILibAsyncSocket_MemoryOwnership_STATIC,0);
					}
				}
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.ConnectionManager->StateVar_A_ARG_TYPE_ConnectionStatus->Reserved3,UPnP_Device_MediaRenderer_Impl.ConnectionManager->StateVar_A_ARG_TYPE_ConnectionStatus->Reserved3L,ILibAsyncSocket_MemoryOwnership_USER,0);
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.ConnectionManager->StateVar_A_ARG_TYPE_ConnectionStatus->Reserved8,UPnP_Device_MediaRenderer_Impl.ConnectionManager->StateVar_A_ARG_TYPE_ConnectionStatus->Reserved8L,ILibAsyncSocket_MemoryOwnership_USER,0);
			}
			if (UPnP_Device_MediaRenderer_Impl.ConnectionManager->StateVar_A_ARG_TYPE_ConnectionID!=NULL)
			{
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.ConnectionManager->StateVar_A_ARG_TYPE_ConnectionID->Reserved1,UPnP_Device_MediaRenderer_Impl.ConnectionManager->StateVar_A_ARG_TYPE_ConnectionID->Reserved1L,ILibAsyncSocket_MemoryOwnership_USER,0);
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.ConnectionManager->StateVar_A_ARG_TYPE_ConnectionID->Reserved8,UPnP_Device_MediaRenderer_Impl.ConnectionManager->StateVar_A_ARG_TYPE_ConnectionID->Reserved8L,ILibAsyncSocket_MemoryOwnership_USER,0);
			}
			if (UPnP_Device_MediaRenderer_Impl.ConnectionManager->StateVar_A_ARG_TYPE_Direction!=NULL)
			{
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.ConnectionManager->StateVar_A_ARG_TYPE_Direction->Reserved1,UPnP_Device_MediaRenderer_Impl.ConnectionManager->StateVar_A_ARG_TYPE_Direction->Reserved1L,ILibAsyncSocket_MemoryOwnership_USER,0);
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.ConnectionManager->StateVar_A_ARG_TYPE_Direction->Reserved2,UPnP_Device_MediaRenderer_Impl.ConnectionManager->StateVar_A_ARG_TYPE_Direction->Reserved2L,ILibAsyncSocket_MemoryOwnership_USER,0);
				for(i=0;i<UPnP_StateVariable_AllowedValues_MAX;++i)
				{
					if (UPnP_Device_MediaRenderer_Impl.ConnectionManager->StateVar_A_ARG_TYPE_Direction->AllowedValues[i]!=NULL)
					{
						ILibWebServer_StreamBody(session,"<allowedValue>",14,ILibAsyncSocket_MemoryOwnership_STATIC,0);
						ILibWebServer_StreamBody(session,UPnP_Device_MediaRenderer_Impl.ConnectionManager->StateVar_A_ARG_TYPE_Direction->AllowedValues[i],(int)strlen(UPnP_Device_MediaRenderer_Impl.ConnectionManager->StateVar_A_ARG_TYPE_Direction->AllowedValues[i]),ILibAsyncSocket_MemoryOwnership_USER,0);
						ILibWebServer_StreamBody(session,"</allowedValue>",15,ILibAsyncSocket_MemoryOwnership_STATIC,0);
					}
				}
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.ConnectionManager->StateVar_A_ARG_TYPE_Direction->Reserved3,UPnP_Device_MediaRenderer_Impl.ConnectionManager->StateVar_A_ARG_TYPE_Direction->Reserved3L,ILibAsyncSocket_MemoryOwnership_USER,0);
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.ConnectionManager->StateVar_A_ARG_TYPE_Direction->Reserved8,UPnP_Device_MediaRenderer_Impl.ConnectionManager->StateVar_A_ARG_TYPE_Direction->Reserved8L,ILibAsyncSocket_MemoryOwnership_USER,0);
			}
			if (UPnP_Device_MediaRenderer_Impl.ConnectionManager->StateVar_A_ARG_TYPE_RcsID!=NULL)
			{
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.ConnectionManager->StateVar_A_ARG_TYPE_RcsID->Reserved1,UPnP_Device_MediaRenderer_Impl.ConnectionManager->StateVar_A_ARG_TYPE_RcsID->Reserved1L,ILibAsyncSocket_MemoryOwnership_USER,0);
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.ConnectionManager->StateVar_A_ARG_TYPE_RcsID->Reserved8,UPnP_Device_MediaRenderer_Impl.ConnectionManager->StateVar_A_ARG_TYPE_RcsID->Reserved8L,ILibAsyncSocket_MemoryOwnership_USER,0);
			}
			if (UPnP_Device_MediaRenderer_Impl.ConnectionManager->StateVar_SourceProtocolInfo!=NULL)
			{
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.ConnectionManager->StateVar_SourceProtocolInfo->Reserved1,UPnP_Device_MediaRenderer_Impl.ConnectionManager->StateVar_SourceProtocolInfo->Reserved1L,ILibAsyncSocket_MemoryOwnership_USER,0);
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.ConnectionManager->StateVar_SourceProtocolInfo->Reserved8,UPnP_Device_MediaRenderer_Impl.ConnectionManager->StateVar_SourceProtocolInfo->Reserved8L,ILibAsyncSocket_MemoryOwnership_USER,0);
			}
			if (UPnP_Device_MediaRenderer_Impl.ConnectionManager->StateVar_SinkProtocolInfo!=NULL)
			{
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.ConnectionManager->StateVar_SinkProtocolInfo->Reserved1,UPnP_Device_MediaRenderer_Impl.ConnectionManager->StateVar_SinkProtocolInfo->Reserved1L,ILibAsyncSocket_MemoryOwnership_USER,0);
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.ConnectionManager->StateVar_SinkProtocolInfo->Reserved8,UPnP_Device_MediaRenderer_Impl.ConnectionManager->StateVar_SinkProtocolInfo->Reserved8L,ILibAsyncSocket_MemoryOwnership_USER,0);
			}
			if (UPnP_Device_MediaRenderer_Impl.ConnectionManager->StateVar_A_ARG_TYPE_ProtocolInfo!=NULL)
			{
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.ConnectionManager->StateVar_A_ARG_TYPE_ProtocolInfo->Reserved1,UPnP_Device_MediaRenderer_Impl.ConnectionManager->StateVar_A_ARG_TYPE_ProtocolInfo->Reserved1L,ILibAsyncSocket_MemoryOwnership_USER,0);
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.ConnectionManager->StateVar_A_ARG_TYPE_ProtocolInfo->Reserved8,UPnP_Device_MediaRenderer_Impl.ConnectionManager->StateVar_A_ARG_TYPE_ProtocolInfo->Reserved8L,ILibAsyncSocket_MemoryOwnership_USER,0);
			}
			if (UPnP_Device_MediaRenderer_Impl.ConnectionManager->StateVar_A_ARG_TYPE_ConnectionManager!=NULL)
			{
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.ConnectionManager->StateVar_A_ARG_TYPE_ConnectionManager->Reserved1,UPnP_Device_MediaRenderer_Impl.ConnectionManager->StateVar_A_ARG_TYPE_ConnectionManager->Reserved1L,ILibAsyncSocket_MemoryOwnership_USER,0);
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.ConnectionManager->StateVar_A_ARG_TYPE_ConnectionManager->Reserved8,UPnP_Device_MediaRenderer_Impl.ConnectionManager->StateVar_A_ARG_TYPE_ConnectionManager->Reserved8L,ILibAsyncSocket_MemoryOwnership_USER,0);
			}
			if (UPnP_Device_MediaRenderer_Impl.ConnectionManager->StateVar_A_ARG_TYPE_AVTransportID!=NULL)
			{
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.ConnectionManager->StateVar_A_ARG_TYPE_AVTransportID->Reserved1,UPnP_Device_MediaRenderer_Impl.ConnectionManager->StateVar_A_ARG_TYPE_AVTransportID->Reserved1L,ILibAsyncSocket_MemoryOwnership_USER,0);
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.ConnectionManager->StateVar_A_ARG_TYPE_AVTransportID->Reserved8,UPnP_Device_MediaRenderer_Impl.ConnectionManager->StateVar_A_ARG_TYPE_AVTransportID->Reserved8L,ILibAsyncSocket_MemoryOwnership_USER,0);
			}
			if (UPnP_Device_MediaRenderer_Impl.ConnectionManager->StateVar_CurrentConnectionIDs!=NULL)
			{
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.ConnectionManager->StateVar_CurrentConnectionIDs->Reserved1,UPnP_Device_MediaRenderer_Impl.ConnectionManager->StateVar_CurrentConnectionIDs->Reserved1L,ILibAsyncSocket_MemoryOwnership_USER,0);
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.ConnectionManager->StateVar_CurrentConnectionIDs->Reserved8,UPnP_Device_MediaRenderer_Impl.ConnectionManager->StateVar_CurrentConnectionIDs->Reserved8L,ILibAsyncSocket_MemoryOwnership_USER,0);
			}
			free(buffer);
			UPnPStreamDescriptionDocument_SCPD(session,0,NULL,0,0,0,1);
		}
		else if (header->DirectiveObjLength==26 && memcmp((header->DirectiveObj)+1,"RenderingControl/scpd.xml",25)==0)
		{
			ILibWebServer_StreamHeader_Raw(session,200,"OK",responseHeader,1);
			UPnPStreamDescriptionDocument_SCPD(session,1,NULL,0,0,0,0);
			buffer = ILibDecompressString((unsigned char*)UPnP_ActionTable_RenderingControl_Impl.Reserved,UPnP_ActionTable_RenderingControl_Impl.ReservedXL,UPnP_ActionTable_RenderingControl_Impl.ReservedUXL);
			if (UPnP_Device_MediaRenderer_Impl.RenderingControl->GetMute!=NULL){UPnPStreamDescriptionDocument_SCPD(session,0,buffer,UPnP_Device_MediaRenderer_Impl.RenderingControl->GetMute->Reserved,UPnP_Device_MediaRenderer_Impl.RenderingControl->GetMute->Reserved2,0,0);}
			if (UPnP_Device_MediaRenderer_Impl.RenderingControl->GetVolume!=NULL){UPnPStreamDescriptionDocument_SCPD(session,0,buffer,UPnP_Device_MediaRenderer_Impl.RenderingControl->GetVolume->Reserved,UPnP_Device_MediaRenderer_Impl.RenderingControl->GetVolume->Reserved2,0,0);}
			if (UPnP_Device_MediaRenderer_Impl.RenderingControl->ListPresets!=NULL){UPnPStreamDescriptionDocument_SCPD(session,0,buffer,UPnP_Device_MediaRenderer_Impl.RenderingControl->ListPresets->Reserved,UPnP_Device_MediaRenderer_Impl.RenderingControl->ListPresets->Reserved2,0,0);}
			if (UPnP_Device_MediaRenderer_Impl.RenderingControl->SelectPreset!=NULL){UPnPStreamDescriptionDocument_SCPD(session,0,buffer,UPnP_Device_MediaRenderer_Impl.RenderingControl->SelectPreset->Reserved,UPnP_Device_MediaRenderer_Impl.RenderingControl->SelectPreset->Reserved2,0,0);}
			if (UPnP_Device_MediaRenderer_Impl.RenderingControl->SetMute!=NULL){UPnPStreamDescriptionDocument_SCPD(session,0,buffer,UPnP_Device_MediaRenderer_Impl.RenderingControl->SetMute->Reserved,UPnP_Device_MediaRenderer_Impl.RenderingControl->SetMute->Reserved2,0,0);}
			if (UPnP_Device_MediaRenderer_Impl.RenderingControl->SetVolume!=NULL){UPnPStreamDescriptionDocument_SCPD(session,0,buffer,UPnP_Device_MediaRenderer_Impl.RenderingControl->SetVolume->Reserved,UPnP_Device_MediaRenderer_Impl.RenderingControl->SetVolume->Reserved2,0,0);}
			if (UPnP_Device_MediaRenderer_Impl.RenderingControl->upnpRenderer_GetMediaPlayersList!=NULL){UPnPStreamDescriptionDocument_SCPD(session,0,buffer,UPnP_Device_MediaRenderer_Impl.RenderingControl->upnpRenderer_GetMediaPlayersList->Reserved,UPnP_Device_MediaRenderer_Impl.RenderingControl->upnpRenderer_GetMediaPlayersList->Reserved2,0,0);}
			if (UPnP_Device_MediaRenderer_Impl.RenderingControl->upnpRenderer_SetMediaPlayer!=NULL){UPnPStreamDescriptionDocument_SCPD(session,0,buffer,UPnP_Device_MediaRenderer_Impl.RenderingControl->upnpRenderer_SetMediaPlayer->Reserved,UPnP_Device_MediaRenderer_Impl.RenderingControl->upnpRenderer_SetMediaPlayer->Reserved2,0,0);}
			free(buffer);
			UPnPStreamDescriptionDocument_SCPD(session,0,NULL,0,0,1,0);
			buffer = ILibDecompressString((unsigned char*)UPnP_StateVariableTable_RenderingControl_Impl.Reserved,UPnP_StateVariableTable_RenderingControl_Impl.ReservedXL,UPnP_StateVariableTable_RenderingControl_Impl.ReservedUXL);
			if (UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_Mute!=NULL)
			{
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_Mute->Reserved1,UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_Mute->Reserved1L,ILibAsyncSocket_MemoryOwnership_USER,0);
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_Mute->Reserved8,UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_Mute->Reserved8L,ILibAsyncSocket_MemoryOwnership_USER,0);
			}
			if (UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_A_ARG_TYPE_Channel!=NULL)
			{
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_A_ARG_TYPE_Channel->Reserved1,UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_A_ARG_TYPE_Channel->Reserved1L,ILibAsyncSocket_MemoryOwnership_USER,0);
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_A_ARG_TYPE_Channel->Reserved2,UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_A_ARG_TYPE_Channel->Reserved2L,ILibAsyncSocket_MemoryOwnership_USER,0);
				for(i=0;i<UPnP_StateVariable_AllowedValues_MAX;++i)
				{
					if (UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_A_ARG_TYPE_Channel->AllowedValues[i]!=NULL)
					{
						ILibWebServer_StreamBody(session,"<allowedValue>",14,ILibAsyncSocket_MemoryOwnership_STATIC,0);
						ILibWebServer_StreamBody(session,UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_A_ARG_TYPE_Channel->AllowedValues[i],(int)strlen(UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_A_ARG_TYPE_Channel->AllowedValues[i]),ILibAsyncSocket_MemoryOwnership_USER,0);
						ILibWebServer_StreamBody(session,"</allowedValue>",15,ILibAsyncSocket_MemoryOwnership_STATIC,0);
					}
				}
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_A_ARG_TYPE_Channel->Reserved3,UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_A_ARG_TYPE_Channel->Reserved3L,ILibAsyncSocket_MemoryOwnership_USER,0);
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_A_ARG_TYPE_Channel->Reserved8,UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_A_ARG_TYPE_Channel->Reserved8L,ILibAsyncSocket_MemoryOwnership_USER,0);
			}
			if (UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_A_ARG_TYPE_InstanceID!=NULL)
			{
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_A_ARG_TYPE_InstanceID->Reserved1,UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_A_ARG_TYPE_InstanceID->Reserved1L,ILibAsyncSocket_MemoryOwnership_USER,0);
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_A_ARG_TYPE_InstanceID->Reserved8,UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_A_ARG_TYPE_InstanceID->Reserved8L,ILibAsyncSocket_MemoryOwnership_USER,0);
			}
			if (UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_Volume!=NULL)
			{
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_Volume->Reserved1,UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_Volume->Reserved1L,ILibAsyncSocket_MemoryOwnership_USER,0);
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_Volume->Reserved4,UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_Volume->Reserved4L,ILibAsyncSocket_MemoryOwnership_USER,0);
				if (UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_Volume->MinMaxStep[0]!=NULL)
				{
					ILibWebServer_StreamBody(session,"<minimum>",9,ILibAsyncSocket_MemoryOwnership_STATIC,0);
					ILibWebServer_StreamBody(session,UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_Volume->MinMaxStep[0],(int)strlen(UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_Volume->MinMaxStep[0]),ILibAsyncSocket_MemoryOwnership_USER,0);
					ILibWebServer_StreamBody(session,"</minimum>",10,ILibAsyncSocket_MemoryOwnership_STATIC,0);
				}
				if (UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_Volume->MinMaxStep[1]!=NULL)
				{
					ILibWebServer_StreamBody(session,"<maximum>",9,ILibAsyncSocket_MemoryOwnership_STATIC,0);
					ILibWebServer_StreamBody(session,UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_Volume->MinMaxStep[1],(int)strlen(UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_Volume->MinMaxStep[1]),ILibAsyncSocket_MemoryOwnership_USER,0);
					ILibWebServer_StreamBody(session,"</maximum>",10,ILibAsyncSocket_MemoryOwnership_STATIC,0);
				}
				if (UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_Volume->MinMaxStep[2]!=NULL)
				{
					ILibWebServer_StreamBody(session,"<step>",6,ILibAsyncSocket_MemoryOwnership_STATIC,0);
					ILibWebServer_StreamBody(session,UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_Volume->MinMaxStep[2],(int)strlen(UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_Volume->MinMaxStep[2]),ILibAsyncSocket_MemoryOwnership_USER,0);
					ILibWebServer_StreamBody(session,"</step>",7,ILibAsyncSocket_MemoryOwnership_STATIC,0);
				}
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_Volume->Reserved5,UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_Volume->Reserved5L,ILibAsyncSocket_MemoryOwnership_USER,0);
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_Volume->Reserved8,UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_Volume->Reserved8L,ILibAsyncSocket_MemoryOwnership_USER,0);
			}
			if (UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_A_ARG_TYPE_MediaPlayerName!=NULL)
			{
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_A_ARG_TYPE_MediaPlayerName->Reserved1,UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_A_ARG_TYPE_MediaPlayerName->Reserved1L,ILibAsyncSocket_MemoryOwnership_USER,0);
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_A_ARG_TYPE_MediaPlayerName->Reserved8,UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_A_ARG_TYPE_MediaPlayerName->Reserved8L,ILibAsyncSocket_MemoryOwnership_USER,0);
			}
			if (UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_A_ARG_TYPE_PresetName!=NULL)
			{
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_A_ARG_TYPE_PresetName->Reserved1,UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_A_ARG_TYPE_PresetName->Reserved1L,ILibAsyncSocket_MemoryOwnership_USER,0);
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_A_ARG_TYPE_PresetName->Reserved2,UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_A_ARG_TYPE_PresetName->Reserved2L,ILibAsyncSocket_MemoryOwnership_USER,0);
				for(i=0;i<UPnP_StateVariable_AllowedValues_MAX;++i)
				{
					if (UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_A_ARG_TYPE_PresetName->AllowedValues[i]!=NULL)
					{
						ILibWebServer_StreamBody(session,"<allowedValue>",14,ILibAsyncSocket_MemoryOwnership_STATIC,0);
						ILibWebServer_StreamBody(session,UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_A_ARG_TYPE_PresetName->AllowedValues[i],(int)strlen(UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_A_ARG_TYPE_PresetName->AllowedValues[i]),ILibAsyncSocket_MemoryOwnership_USER,0);
						ILibWebServer_StreamBody(session,"</allowedValue>",15,ILibAsyncSocket_MemoryOwnership_STATIC,0);
					}
				}
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_A_ARG_TYPE_PresetName->Reserved3,UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_A_ARG_TYPE_PresetName->Reserved3L,ILibAsyncSocket_MemoryOwnership_USER,0);
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_A_ARG_TYPE_PresetName->Reserved8,UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_A_ARG_TYPE_PresetName->Reserved8L,ILibAsyncSocket_MemoryOwnership_USER,0);
			}
			if (UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_PresetNameList!=NULL)
			{
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_PresetNameList->Reserved1,UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_PresetNameList->Reserved1L,ILibAsyncSocket_MemoryOwnership_USER,0);
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_PresetNameList->Reserved2,UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_PresetNameList->Reserved2L,ILibAsyncSocket_MemoryOwnership_USER,0);
				for(i=0;i<UPnP_StateVariable_AllowedValues_MAX;++i)
				{
					if (UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_PresetNameList->AllowedValues[i]!=NULL)
					{
						ILibWebServer_StreamBody(session,"<allowedValue>",14,ILibAsyncSocket_MemoryOwnership_STATIC,0);
						ILibWebServer_StreamBody(session,UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_PresetNameList->AllowedValues[i],(int)strlen(UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_PresetNameList->AllowedValues[i]),ILibAsyncSocket_MemoryOwnership_USER,0);
						ILibWebServer_StreamBody(session,"</allowedValue>",15,ILibAsyncSocket_MemoryOwnership_STATIC,0);
					}
				}
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_PresetNameList->Reserved3,UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_PresetNameList->Reserved3L,ILibAsyncSocket_MemoryOwnership_USER,0);
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_PresetNameList->Reserved8,UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_PresetNameList->Reserved8L,ILibAsyncSocket_MemoryOwnership_USER,0);
			}
			if (UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_LastChange!=NULL)
			{
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_LastChange->Reserved1,UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_LastChange->Reserved1L,ILibAsyncSocket_MemoryOwnership_USER,0);
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_LastChange->Reserved8,UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_LastChange->Reserved8L,ILibAsyncSocket_MemoryOwnership_USER,0);
			}
			if (UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_MediaPlayerName!=NULL)
			{
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_MediaPlayerName->Reserved1,UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_MediaPlayerName->Reserved1L,ILibAsyncSocket_MemoryOwnership_USER,0);
				ILibWebServer_StreamBody(session,buffer+UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_MediaPlayerName->Reserved8,UPnP_Device_MediaRenderer_Impl.RenderingControl->StateVar_MediaPlayerName->Reserved8L,ILibAsyncSocket_MemoryOwnership_USER,0);
			}
			free(buffer);
			UPnPStreamDescriptionDocument_SCPD(session,0,NULL,0,0,0,1);
		}
		
		else
		{
			// A GET Request for something we don't have
			if ((errorPacket = (char*)malloc(128)) == NULL) ILIBCRITICALEXIT(254);
			errorPacketLength = snprintf(errorPacket, 128, errorTemplate, 404, "File Not Found", UPnPPLATFORM);
			ILibWebServer_Send_Raw(session, errorPacket, errorPacketLength, 0, 1);
		}
	}
	else if (header->DirectiveLength == 4 && memcmp(header->Directive,"POST",4) == 0)
	{
		// Defer Control to the POST Handler
		if (UPnPProcessPOST(session, header, bodyBuffer, offset, bodyBufferLength) != 0)
		{
			// A POST for an action that doesn't exist
			UPnPResponse_Error(session, 401, "Invalid Action");
		}
	}
	else if (header->DirectiveLength == 9 && memcmp(header->Directive, "SUBSCRIBE" ,9) == 0)
	{
		// Subscription Handler
		UPnPProcessSUBSCRIBE(header,session);
	}
	else if (header->DirectiveLength == 11 && memcmp(header->Directive, "UNSUBSCRIBE", 11) == 0)
	{
		// UnSubscribe Handler
		UPnPProcessUNSUBSCRIBE(header,session);
	}	else
	{
		// The client tried something we didn't expect/support
		if ((errorPacket = (char*)malloc(128)) == NULL) ILIBCRITICALEXIT(254);
		errorPacketLength = snprintf(errorPacket, 128, errorTemplate, 400, "Bad Request", UPnPPLATFORM);
		ILibWebServer_Send_Raw(session, errorPacket, errorPacketLength, ILibAsyncSocket_MemoryOwnership_CHAIN, 1);
	}
}
void UPnPFragmentedSendNotify_Destroy(void *data);
void UPnPMasterPreSelect(void* object, void *socketset, void *writeset, void *errorset, int* blocktime)
{
	int i;
	struct UPnPDataObject *UPnPObject = (struct UPnPDataObject*)object;	
	struct UPnPFragmentNotifyStruct *f;
	int timeout;
	UNREFERENCED_PARAMETER( socketset );
	UNREFERENCED_PARAMETER( writeset );
	UNREFERENCED_PARAMETER( errorset );
	UNREFERENCED_PARAMETER( blocktime );
	
	if (UPnPObject->InitialNotify == 0)
	{
		// The initial "HELLO" packets were not sent yet, so lets send them
		UPnPObject->InitialNotify = -1;
		
		// In case we were interrupted, we need to flush out the caches of
		// all the control points by sending a "byebye" first, to insure
		// control points don't ignore our "hello" packets thinking they are just
		// periodic re-advertisements.
		UPnPSendByeBye(UPnPObject);
		
		// PacketNumber 0 is the controller, for the rest of the packets. Send
		// one of these to send out an advertisement "group"
		if ((f = (struct UPnPFragmentNotifyStruct*)malloc(sizeof(struct UPnPFragmentNotifyStruct))) == NULL) ILIBCRITICALEXIT(254);
		f->packetNumber = 0;
		f->upnp = UPnPObject;
		
		// We need to inject some delay in these packets to space them out,
		// otherwise we could overflow the inbound buffer of the recipient, causing them
		// to lose packets. And UPnP/1.0 control points are not as robust as UPnP/1.1 control points,
		// so they need all the help they can get ;)
		timeout = (int)(0 + ((unsigned short)rand() % (500)));
		do { f->upnp->InitialNotify = rand(); } while (f->upnp->InitialNotify == 0);
		
		// Register for the timed callback, to actually send the packet
		ILibLifeTime_AddEx(f->upnp->WebServerTimer, f, timeout, &UPnPFragmentedSendNotify, &UPnPFragmentedSendNotify_Destroy);
	}
	
	if (UPnPObject->UpdateFlag != 0)
	{
		struct sockaddr_in any4;
		struct sockaddr_in6 any6;
		
		// Somebody told us that we should recheck our IP Address table, as one of them may have changed
		UPnPObject->UpdateFlag = 0;
		
		// Setup ANY addresses
		memset(&any4, 0, sizeof(struct sockaddr_in));
		any4.sin_family = AF_INET;
		any4.sin_port = htons(UPNP_PORT);
		memset(&any6, 0, sizeof(struct sockaddr_in6));
		any6.sin6_family = AF_INET6;
		any6.sin6_port = htons(UPNP_PORT);
		
		// Clear Sockets
		// Iterate through all the currently bound IPv4 addresses and release the sockets
		if (UPnPObject->AddressListV4 != NULL)
		{
			for (i = 0; i < UPnPObject->AddressListV4Length; ++i) ILibChain_SafeRemove(UPnPObject->Chain, UPnPObject->NOTIFY_SEND_socks[i]);
			free(UPnPObject->NOTIFY_SEND_socks);
			for (i = 0; i < UPnPObject->AddressListV4Length; ++i) ILibChain_SafeRemove(UPnPObject->Chain, UPnPObject->NOTIFY_RECEIVE_socks[i]);
			free(UPnPObject->NOTIFY_RECEIVE_socks);
			free(UPnPObject->AddressListV4);
		}
		if (UPnPObject->AddressListV6 != NULL)
		{
			for (i = 0; i < UPnPObject->AddressListV6Length; ++i) ILibChain_SafeRemove(UPnPObject->Chain, UPnPObject->NOTIFY_SEND_socks6[i]);
			free(UPnPObject->NOTIFY_SEND_socks6);
			for (i = 0; i < UPnPObject->AddressListV6Length; ++i) ILibChain_SafeRemove(UPnPObject->Chain, UPnPObject->NOTIFY_RECEIVE_socks6[i]);
			free(UPnPObject->NOTIFY_RECEIVE_socks6);
			free(UPnPObject->AddressListV6);
		}
		
		// Fetch a current list of ip addresses
		UPnPObject->AddressListV4Length = ILibGetLocalIPv4AddressList(&(UPnPObject->AddressListV4), 1);
		
		// Re-Initialize our SEND socket
		if ((UPnPObject->NOTIFY_SEND_socks = (void**)malloc(sizeof(void*)*(UPnPObject->AddressListV4Length))) == NULL) ILIBCRITICALEXIT(254);
		if ((UPnPObject->NOTIFY_RECEIVE_socks = (void**)malloc(sizeof(void*)*(UPnPObject->AddressListV4Length))) == NULL) ILIBCRITICALEXIT(254);
		
		// Test IPv6 support
		if (ILibDetectIPv6Support())
		{
			// Fetch the list of local IPv6 interfaces
			UPnPObject->AddressListV6Length = ILibGetLocalIPv6List(&(UPnPObject->AddressListV6));
			
			// Setup the IPv6 sockets
			if ((UPnPObject->NOTIFY_SEND_socks6 = (void**)malloc(sizeof(void*)*(UPnPObject->AddressListV6Length))) == NULL) ILIBCRITICALEXIT(254);
			if ((UPnPObject->NOTIFY_RECEIVE_socks6 = (void**)malloc(sizeof(void*)*(UPnPObject->AddressListV6Length))) == NULL) ILIBCRITICALEXIT(254);
		}
		
		// Iterate through all the current IP Addresses
		for (i = 0; i < UPnPObject->AddressListV4Length; ++i)
		{
			(UPnPObject->AddressListV4[i]).sin_port = 0; // Bind to ANY port for outbound packets
			UPnPObject->NOTIFY_SEND_socks[i] = ILibAsyncUDPSocket_CreateEx(
			UPnPObject->Chain,
			UPNP_MAX_SSDP_HEADER_SIZE,
			(struct sockaddr*)&any4,									// (struct sockaddr*)&(UPnPObject->AddressListV4[i]),
			ILibAsyncUDPSocket_Reuse_SHARED,
			NULL,
			NULL,
			UPnPObject);
			
			ILibAsyncUDPSocket_SetMulticastTTL(UPnPObject->NOTIFY_SEND_socks[i], UPNP_SSDP_TTL);
			ILibAsyncUDPSocket_SetMulticastLoopback(UPnPObject->NOTIFY_SEND_socks[i], 1);
			
			(UPnPObject->AddressListV4[i]).sin_port = htons(UPNP_PORT); // Bind to UPnP port for inbound packets
			UPnPObject->NOTIFY_RECEIVE_socks[i] = ILibAsyncUDPSocket_CreateEx(
			UPnPObject->Chain,
			UPNP_MAX_SSDP_HEADER_SIZE,
			(struct sockaddr*)&(UPnPObject->AddressListV4[i]),
			ILibAsyncUDPSocket_Reuse_SHARED,
			&UPnPSSDPSink,
			NULL,
			UPnPObject);
			
			if (!UPnPObject->NOTIFY_RECEIVE_socks[i]) continue;	// when ip removed causes an error if continued

			ILibAsyncUDPSocket_JoinMulticastGroupV4(UPnPObject->NOTIFY_RECEIVE_socks[i], (struct sockaddr_in*)&(UPnPObject->MulticastAddrV4), (struct sockaddr*)&(UPnPObject->AddressListV4[i]));
			ILibAsyncUDPSocket_SetLocalInterface(UPnPObject->NOTIFY_RECEIVE_socks[i], (struct sockaddr*)&(UPnPObject->AddressListV4[i]));
			ILibAsyncUDPSocket_SetMulticastLoopback(UPnPObject->NOTIFY_RECEIVE_socks[i], 1);
		}
		
		if (UPnPObject->AddressListV6Length > 0)
		{
			// Iterate through all the current IPv6 interfaces
			for (i = 0; i < UPnPObject->AddressListV6Length; ++i)
			{
				(UPnPObject->AddressListV6[i]).sin6_port = 0;
				UPnPObject->NOTIFY_SEND_socks6[i] = ILibAsyncUDPSocket_CreateEx(
				UPnPObject->Chain,
				UPNP_MAX_SSDP_HEADER_SIZE,
				(struct sockaddr*)&(UPnPObject->AddressListV6[i]),
				ILibAsyncUDPSocket_Reuse_SHARED,
				NULL,
				NULL,
				UPnPObject);
				
				ILibAsyncUDPSocket_SetMulticastTTL(UPnPObject->NOTIFY_SEND_socks6[i], UPNP_SSDP_TTL);
				ILibAsyncUDPSocket_SetMulticastLoopback(UPnPObject->NOTIFY_SEND_socks6[i], 1);
				
				(UPnPObject->AddressListV6[i]).sin6_port = htons(UPNP_PORT); // Bind to UPnP port for inbound packets
				UPnPObject->NOTIFY_RECEIVE_socks6[i] = ILibAsyncUDPSocket_CreateEx(
				UPnPObject->Chain,
				UPNP_MAX_SSDP_HEADER_SIZE,
				(struct sockaddr*)&any6,							// (struct sockaddr*)&(UPnPObject->AddressListV6[i]),
				ILibAsyncUDPSocket_Reuse_SHARED,
				&UPnPSSDPSink,
				NULL,
				UPnPObject);
				
				if (ILibAsyncSocket_IsIPv6LinkLocal((struct sockaddr*)&(UPnPObject->AddressListV6[i])))
				{
					ILibAsyncUDPSocket_JoinMulticastGroupV6(UPnPObject->NOTIFY_RECEIVE_socks6[i], &(UPnPObject->MulticastAddrV6LL), UPnPObject->AddressListV6[i].sin6_scope_id);
				}
				else
				{
					ILibAsyncUDPSocket_JoinMulticastGroupV6(UPnPObject->NOTIFY_RECEIVE_socks6[i], &(UPnPObject->MulticastAddrV6SL), UPnPObject->AddressListV6[i].sin6_scope_id);
				}
				ILibAsyncUDPSocket_SetMulticastLoopback(UPnPObject->NOTIFY_RECEIVE_socks6[i], 1);
				ILibAsyncUDPSocket_SetLocalInterface(UPnPObject->NOTIFY_RECEIVE_socks6[i], (struct sockaddr*)&(UPnPObject->AddressListV6[i]));
			}
		}
		
		// Iterate through all the packet types, and re-broadcast
		for (i = 1; i <= 4; ++i)
		{
			if ((f = (struct UPnPFragmentNotifyStruct*)malloc(sizeof(struct UPnPFragmentNotifyStruct))) == NULL) ILIBCRITICALEXIT(254);
			f->packetNumber = i;
			f->upnp = UPnPObject;
			
			// Inject some random delay, to spread these packets out, to help prevent the inbound buffer of the recipient from overflowing, causing dropped packets.
			timeout = (int)(0 + ((unsigned short)rand() % (500)));
			ILibLifeTime_AddEx(f->upnp->WebServerTimer, f, timeout, &UPnPFragmentedSendNotify, &UPnPFragmentedSendNotify_Destroy);
		}
	}
}

void UPnPFragmentedSendNotify_Destroy(void *data)
{
	free(data);
}

void UPnPFragmentedSendNotify(void *data)
{
	int i,i2;
	int subsetRange;
	int timeout, timeout2;
	struct UPnPFragmentNotifyStruct *f;
	struct UPnPFragmentNotifyStruct *FNS = (struct UPnPFragmentNotifyStruct*)data;
	
	if (FNS->packetNumber == 0)
	{				
		subsetRange = 5000 / 5; // Make sure all our packets will get out within 5 seconds
		
		// Send the first "group"
		for (i2 = 0; i2 <= 4; ++i2)
		{
			if ((f = (struct UPnPFragmentNotifyStruct*)malloc(sizeof(struct UPnPFragmentNotifyStruct))) == NULL) ILIBCRITICALEXIT(254);
			f->packetNumber = i2 + 1;
			f->upnp = FNS->upnp;
			timeout2 = (rand() % subsetRange);
			ILibLifeTime_AddEx(FNS->upnp->WebServerTimer, f, timeout2, &UPnPFragmentedSendNotify, &UPnPFragmentedSendNotify_Destroy);
		}
		
		// Now Repeat this "group" after 7 seconds, to insure there is no overlap
		for (i2 = 0; i2 <= 4; ++i2)
		{
			if ((f = (struct UPnPFragmentNotifyStruct*)malloc(sizeof(struct UPnPFragmentNotifyStruct))) == NULL) ILIBCRITICALEXIT(254);
			f->packetNumber = i2 + 1;
			f->upnp = FNS->upnp;
			timeout2 = 7000 + (rand() % subsetRange);
			ILibLifeTime_AddEx(FNS->upnp->WebServerTimer, f, timeout2, &UPnPFragmentedSendNotify, &UPnPFragmentedSendNotify_Destroy);
		}
		
		// Calculate the next transmission window and spread the packets
		timeout = (int)((FNS->upnp->NotifyCycleTime / 4) + ((unsigned short)rand() % (FNS->upnp->NotifyCycleTime / 2 - FNS->upnp->NotifyCycleTime / 4)));
		ILibLifeTime_Add(FNS->upnp->WebServerTimer, FNS, timeout, &UPnPFragmentedSendNotify, &UPnPFragmentedSendNotify_Destroy);
	}
	
	for (i = 0; i < FNS->upnp->AddressListV4Length; ++i)
	{
		switch(FNS->packetNumber)
		{
			case 1:
			UPnPBuildSendSsdpNotifyPacket(FNS->upnp->NOTIFY_SEND_socks[i], FNS->upnp, (struct sockaddr*)&(FNS->upnp->AddressListV4[i]), 0, "::upnp:rootdevice", "upnp:rootdevice", "");
			break;
			case 2:
			UPnPBuildSendSsdpNotifyPacket(FNS->upnp->NOTIFY_SEND_socks[i], FNS->upnp, (struct sockaddr*)&(FNS->upnp->AddressListV4[i]), 0, "", "uuid:", FNS->upnp->UDN);
			break;
			case 3:
			UPnPBuildSendSsdpNotifyPacket(FNS->upnp->NOTIFY_SEND_socks[i], FNS->upnp, (struct sockaddr*)&(FNS->upnp->AddressListV4[i]), 0, "::urn:schemas-upnp-org:device:MediaRenderer:1", "urn:schemas-upnp-org:device:MediaRenderer:1", "");
			break;
			case 4:
			UPnPBuildSendSsdpNotifyPacket(FNS->upnp->NOTIFY_SEND_socks[i], FNS->upnp, (struct sockaddr*)&(FNS->upnp->AddressListV4[i]), 0, "::urn:schemas-upnp-org:service:AVTransport:1", "urn:schemas-upnp-org:service:AVTransport:1", "");
			break;
			case 5:
			UPnPBuildSendSsdpNotifyPacket(FNS->upnp->NOTIFY_SEND_socks[i], FNS->upnp, (struct sockaddr*)&(FNS->upnp->AddressListV4[i]), 0, "::urn:schemas-upnp-org:service:ConnectionManager:1", "urn:schemas-upnp-org:service:ConnectionManager:1", "");
			break;
			case 6:
			UPnPBuildSendSsdpNotifyPacket(FNS->upnp->NOTIFY_SEND_socks[i], FNS->upnp, (struct sockaddr*)&(FNS->upnp->AddressListV4[i]), 0, "::urn:schemas-upnp-org:service:RenderingControl:1", "urn:schemas-upnp-org:service:RenderingControl:1", "");
			break;
			
		}
	}
	
	for (i = 0; i < FNS->upnp->AddressListV6Length; ++i)
	{
		switch(FNS->packetNumber)
		{
			case 1:
			UPnPBuildSendSsdpNotifyPacket(FNS->upnp->NOTIFY_SEND_socks6[i], FNS->upnp, (struct sockaddr*)&(FNS->upnp->AddressListV6[i]), 0, "::upnp:rootdevice", "upnp:rootdevice", "");
			break;
			case 2:
			UPnPBuildSendSsdpNotifyPacket(FNS->upnp->NOTIFY_SEND_socks6[i], FNS->upnp, (struct sockaddr*)&(FNS->upnp->AddressListV6[i]), 0, "", "uuid:", FNS->upnp->UDN);
			break;
			case 3:
			UPnPBuildSendSsdpNotifyPacket(FNS->upnp->NOTIFY_SEND_socks6[i], FNS->upnp, (struct sockaddr*)&(FNS->upnp->AddressListV6[i]), 0, "::urn:schemas-upnp-org:device:MediaRenderer:1", "urn:schemas-upnp-org:device:MediaRenderer:1", "");
			break;
			case 4:
			UPnPBuildSendSsdpNotifyPacket(FNS->upnp->NOTIFY_SEND_socks6[i], FNS->upnp, (struct sockaddr*)&(FNS->upnp->AddressListV6[i]), 0, "::urn:schemas-upnp-org:service:AVTransport:1", "urn:schemas-upnp-org:service:AVTransport:1", "");
			break;
			case 5:
			UPnPBuildSendSsdpNotifyPacket(FNS->upnp->NOTIFY_SEND_socks6[i], FNS->upnp, (struct sockaddr*)&(FNS->upnp->AddressListV6[i]), 0, "::urn:schemas-upnp-org:service:ConnectionManager:1", "urn:schemas-upnp-org:service:ConnectionManager:1", "");
			break;
			case 6:
			UPnPBuildSendSsdpNotifyPacket(FNS->upnp->NOTIFY_SEND_socks6[i], FNS->upnp, (struct sockaddr*)&(FNS->upnp->AddressListV6[i]), 0, "::urn:schemas-upnp-org:service:RenderingControl:1", "urn:schemas-upnp-org:service:RenderingControl:1", "");
			break;
			
		}
	}
	
	if (FNS->packetNumber != 0) free(FNS);
}


void UPnPSendNotify(const struct UPnPDataObject *upnp)
{
	int i, i2;
	for (i=0;i<upnp->AddressListV4Length;++i)
	{
		for (i2=0; i2<2; i2++)
		{
			UPnPBuildSendSsdpNotifyPacket(upnp->NOTIFY_SEND_socks[i], upnp, (struct sockaddr*)&(upnp->AddressListV4[i]), 0, "::upnp:rootdevice", "upnp:rootdevice", "");
			UPnPBuildSendSsdpNotifyPacket(upnp->NOTIFY_SEND_socks[i], upnp, (struct sockaddr*)&(upnp->AddressListV4[i]), 0, "", "uuid:", upnp->UDN);
			UPnPBuildSendSsdpNotifyPacket(upnp->NOTIFY_SEND_socks[i], upnp, (struct sockaddr*)&(upnp->AddressListV4[i]), 0, "::urn:schemas-upnp-org:device:MediaRenderer:1", "urn:schemas-upnp-org:device:MediaRenderer:1", "");
			UPnPBuildSendSsdpNotifyPacket(upnp->NOTIFY_SEND_socks[i], upnp, (struct sockaddr*)&(upnp->AddressListV4[i]), 0, "::urn:schemas-upnp-org:service:AVTransport:1", "urn:schemas-upnp-org:service:AVTransport:1", "");
			UPnPBuildSendSsdpNotifyPacket(upnp->NOTIFY_SEND_socks[i], upnp, (struct sockaddr*)&(upnp->AddressListV4[i]), 0, "::urn:schemas-upnp-org:service:ConnectionManager:1", "urn:schemas-upnp-org:service:ConnectionManager:1", "");
			UPnPBuildSendSsdpNotifyPacket(upnp->NOTIFY_SEND_socks[i], upnp, (struct sockaddr*)&(upnp->AddressListV4[i]), 0, "::urn:schemas-upnp-org:service:RenderingControl:1", "urn:schemas-upnp-org:service:RenderingControl:1", "");
			
		}
	}
	for (i=0;i<upnp->AddressListV6Length;++i)
	{
		for (i2=0; i2<2; i2++)
		{
			UPnPBuildSendSsdpNotifyPacket(upnp->NOTIFY_SEND_socks6[i], upnp, (struct sockaddr*)&(upnp->AddressListV6[i]), 0, "::upnp:rootdevice", "upnp:rootdevice", "");
			UPnPBuildSendSsdpNotifyPacket(upnp->NOTIFY_SEND_socks6[i], upnp, (struct sockaddr*)&(upnp->AddressListV6[i]), 0, "", "uuid:", upnp->UDN);
			UPnPBuildSendSsdpNotifyPacket(upnp->NOTIFY_SEND_socks6[i], upnp, (struct sockaddr*)&(upnp->AddressListV6[i]), 0, "::urn:schemas-upnp-org:device:MediaRenderer:1", "urn:schemas-upnp-org:device:MediaRenderer:1", "");
			UPnPBuildSendSsdpNotifyPacket(upnp->NOTIFY_SEND_socks6[i], upnp, (struct sockaddr*)&(upnp->AddressListV6[i]), 0, "::urn:schemas-upnp-org:service:AVTransport:1", "urn:schemas-upnp-org:service:AVTransport:1", "");
			UPnPBuildSendSsdpNotifyPacket(upnp->NOTIFY_SEND_socks6[i], upnp, (struct sockaddr*)&(upnp->AddressListV6[i]), 0, "::urn:schemas-upnp-org:service:ConnectionManager:1", "urn:schemas-upnp-org:service:ConnectionManager:1", "");
			UPnPBuildSendSsdpNotifyPacket(upnp->NOTIFY_SEND_socks6[i], upnp, (struct sockaddr*)&(upnp->AddressListV6[i]), 0, "::urn:schemas-upnp-org:service:RenderingControl:1", "urn:schemas-upnp-org:service:RenderingControl:1", "");
			
		}
	}
}

int UPnPBuildSendSsdpByeByePacket(void* module, const struct UPnPDataObject *upnp, struct sockaddr* target, char* mcastgroup, char* USNex, char* NT, char* NTex, int DeviceID)
{
	int len;
	
	if (DeviceID == 0)
	{
		len = snprintf(ILibScratchPad, sizeof(ILibScratchPad), "NOTIFY * HTTP/1.1\r\nHOST: %s:1900\r\nNTS: ssdp:byebye\r\nUSN: uuid:%s%s\r\nNT: %s%s\r\nContent-Length: 0\r\n\r\n", mcastgroup, upnp->UDN, USNex, NT, NTex);
	}
	else
	{
		if (memcmp(NT, "uuid:", 5) == 0)
		{
			len = snprintf(ILibScratchPad, sizeof(ILibScratchPad), "NOTIFY * HTTP/1.1\r\nHOST: %s:1900\r\nNTS: ssdp:byebye\r\nUSN: uuid:%s_%d%s\r\nNT: %s%s_%d\r\nContent-Length: 0\r\n\r\n", mcastgroup, upnp->UDN, DeviceID, USNex, NT, NTex, DeviceID);
		}
		else
		{
			len = snprintf(ILibScratchPad, sizeof(ILibScratchPad), "NOTIFY * HTTP/1.1\r\nHOST: %s:1900\r\nNTS: ssdp:byebye\r\nUSN: uuid:%s_%d%s\r\nNT: %s%s\r\nContent-Length: 0\r\n\r\n", mcastgroup, upnp->UDN, DeviceID, USNex, NT, NTex);
		}
	}
	return ILibAsyncUDPSocket_SendTo(module, target, ILibScratchPad, len, ILibAsyncSocket_MemoryOwnership_USER);
}

void UPnPSendByeBye(const struct UPnPDataObject *upnp)
{
	int i, i2;
	struct sockaddr* t1;
	char* t2;
	
	for (i=0; i<upnp->AddressListV4Length; ++i)
	{	
		for (i2=0; i2<2; i2++)
		{
			UPnPBuildSendSsdpByeByePacket(upnp->NOTIFY_SEND_socks[i], upnp, (struct sockaddr*)&(upnp->MulticastAddrV4), UPNP_MCASTv4_GROUP, "::upnp:rootdevice", "upnp:rootdevice", "", 0);
			UPnPBuildSendSsdpByeByePacket(upnp->NOTIFY_SEND_socks[i], upnp, (struct sockaddr*)&(upnp->MulticastAddrV4), UPNP_MCASTv4_GROUP, "", "uuid:", upnp->UDN, 0);
			UPnPBuildSendSsdpByeByePacket(upnp->NOTIFY_SEND_socks[i], upnp, (struct sockaddr*)&(upnp->MulticastAddrV4), UPNP_MCASTv4_GROUP, "::urn:schemas-upnp-org:device:MediaRenderer:1", "urn:schemas-upnp-org:device:MediaRenderer:1", "", 0);
			UPnPBuildSendSsdpByeByePacket(upnp->NOTIFY_SEND_socks[i], upnp, (struct sockaddr*)&(upnp->MulticastAddrV4), UPNP_MCASTv4_GROUP, "::urn:schemas-upnp-org:service:AVTransport:1", "urn:schemas-upnp-org:service:AVTransport:1", "", 0);
			UPnPBuildSendSsdpByeByePacket(upnp->NOTIFY_SEND_socks[i], upnp, (struct sockaddr*)&(upnp->MulticastAddrV4), UPNP_MCASTv4_GROUP, "::urn:schemas-upnp-org:service:ConnectionManager:1", "urn:schemas-upnp-org:service:ConnectionManager:1", "", 0);
			UPnPBuildSendSsdpByeByePacket(upnp->NOTIFY_SEND_socks[i], upnp, (struct sockaddr*)&(upnp->MulticastAddrV4), UPNP_MCASTv4_GROUP, "::urn:schemas-upnp-org:service:RenderingControl:1", "urn:schemas-upnp-org:service:RenderingControl:1", "", 0);
			
		}
	}
	
	for (i=0; i<upnp->AddressListV6Length; ++i)
	{	
		if (ILibAsyncSocket_IsIPv6LinkLocal((struct sockaddr*)&(upnp->AddressListV6[i])))
		{
			t1 = (struct sockaddr*)&(upnp->MulticastAddrV6LL);
			t2 = UPNP_MCASTv6_LINK_GROUPB;
		}
		else
		{
			t1 = (struct sockaddr*)&(upnp->MulticastAddrV6SL);
			t2 = UPNP_MCASTv6_GROUPB;
		}
		
		for (i2=0; i2<2; i2++)
		{
			UPnPBuildSendSsdpByeByePacket(upnp->NOTIFY_SEND_socks6[i], upnp, t1, t2, "::upnp:rootdevice", "upnp:rootdevice", "", 0);
			UPnPBuildSendSsdpByeByePacket(upnp->NOTIFY_SEND_socks6[i], upnp, t1, t2, "", "uuid:", upnp->UDN, 0);
			UPnPBuildSendSsdpByeByePacket(upnp->NOTIFY_SEND_socks6[i], upnp, t1, t2, "::urn:schemas-upnp-org:device:MediaRenderer:1", "urn:schemas-upnp-org:device:MediaRenderer:1", "", 0);
			UPnPBuildSendSsdpByeByePacket(upnp->NOTIFY_SEND_socks6[i], upnp, t1, t2, "::urn:schemas-upnp-org:service:AVTransport:1", "urn:schemas-upnp-org:service:AVTransport:1", "", 0);
			UPnPBuildSendSsdpByeByePacket(upnp->NOTIFY_SEND_socks6[i], upnp, t1, t2, "::urn:schemas-upnp-org:service:ConnectionManager:1", "urn:schemas-upnp-org:service:ConnectionManager:1", "", 0);
			UPnPBuildSendSsdpByeByePacket(upnp->NOTIFY_SEND_socks6[i], upnp, t1, t2, "::urn:schemas-upnp-org:service:RenderingControl:1", "urn:schemas-upnp-org:service:RenderingControl:1", "", 0);
			
		}
	}
}

/*! \fn UPnPResponse_Error(const UPnPSessionToken UPnPToken, const int ErrorCode, const char* ErrorMsg)
\brief Responds to the client invocation with a SOAP Fault
\param UPnPToken UPnP token
\param ErrorCode Fault Code
\param ErrorMsg Error Detail
*/
void UPnPResponse_Error(const UPnPSessionToken UPnPToken, const int ErrorCode, const char* ErrorMsg)
{
	char* body;
	int bodylength;
	char* head;
	int headlength;
	int len = 395 + (int)strlen(ErrorMsg);
	
	if ((body = (char*)malloc(len)) == NULL) ILIBCRITICALEXIT(254);
	bodylength = snprintf(body, len, "<s:Envelope\r\n xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\"><s:Body><s:Fault><faultcode>s:Client</faultcode><faultstring>UPnPError</faultstring><detail><UPnPError xmlns=\"urn:schemas-upnp-org:control-1-0\"><errorCode>%d</errorCode><errorDescription>%s</errorDescription></UPnPError></detail></s:Fault></s:Body></s:Envelope>",ErrorCode,ErrorMsg);
	if ((head = (char*)malloc(59)) == NULL) ILIBCRITICALEXIT(254);
	headlength = snprintf(head, 59, "HTTP/1.1 500 Internal\r\nContent-Length: %d\r\n\r\n",bodylength);
	ILibWebServer_Send_Raw((struct ILibWebServer_Session*)UPnPToken, head, headlength, 0, 0);
	ILibWebServer_Send_Raw((struct ILibWebServer_Session*)UPnPToken, body, bodylength, 0, 1);
}

void UPnPResponseGeneric(const UPnPMicroStackToken UPnPToken, const char* ServiceURI, const char* MethodName, const char* Params)
{
	int RVAL = 0;
	char* packet;
	int packetlength;
	struct ILibWebServer_Session *session = (struct ILibWebServer_Session*)UPnPToken;
	size_t len = 239 + strlen(ServiceURI) + strlen(Params) + (strlen(MethodName) * 2);
	
	if ((packet = (char*)malloc(len)) == NULL) ILIBCRITICALEXIT(254);
	packetlength = snprintf(packet, len, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n<s:Envelope s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\" xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\"><s:Body><u:%sResponse xmlns:u=\"%s\">%s</u:%sResponse></s:Body></s:Envelope>",MethodName,ServiceURI,Params,MethodName);
	LVL3DEBUG(printf("SendBody: %s\r\n", packet);)
	#if defined(WIN32) || defined(_WIN32_WCE)
	RVAL=ILibWebServer_StreamHeader_Raw(session, 200, "OK", "\r\nEXT:\r\nCONTENT-TYPE: text/xml; charset=\"utf-8\"\r\nSERVER: WINDOWS, UPnP/1.0, MicroStack/1.0.5329", 1);
	#elif defined(__SYMBIAN32__)
	RVAL=ILibWebServer_StreamHeader_Raw(session, 200, "OK", "\r\nEXT:\r\nCONTENT-TYPE: text/xml; charset=\"utf-8\"\r\nSERVER: SYMBIAN, UPnP/1.0, MicroStack/1.0.5329", 1);
	#else
	RVAL=ILibWebServer_StreamHeader_Raw(session, 200, "OK", "\r\nEXT:\r\nCONTENT-TYPE: text/xml; charset=\"utf-8\"\r\nSERVER: POSIX, UPnP/1.0, MicroStack/1.0.5329", 1);
	#endif
	if (RVAL!=ILibAsyncSocket_SEND_ON_CLOSED_SOCKET_ERROR && RVAL != ILibWebServer_SEND_RESULTED_IN_DISCONNECT)
	{
		RVAL=ILibWebServer_StreamBody(session, packet, packetlength, 0, 1);
	}
}

/*! \fn UPnPResponse_AVTransport_GetCurrentTransportActions(const UPnPSessionToken UPnPToken, const char* unescaped_Actions)
\brief Response Method for AVTransport >> urn:schemas-upnp-org:service:AVTransport:1 >> GetCurrentTransportActions
\param UPnPToken MicroStack token
\param unescaped_Actions Value of argument Actions \b     Note: Automatically Escaped
*/
void UPnPResponse_AVTransport_GetCurrentTransportActions(const UPnPSessionToken UPnPToken, const char* unescaped_Actions)
{
	char* body;
	char *Actions = (char*)malloc(1+ILibXmlEscapeLength(unescaped_Actions));
	
	ILibXmlEscape(Actions, unescaped_Actions);
	if ((body = (char*)malloc(20+strlen(Actions))) == NULL) ILIBCRITICALEXIT(254);
	snprintf(body, 20+strlen(Actions), "<Actions>%s</Actions>", Actions);
	UPnPResponseGeneric(UPnPToken, "urn:schemas-upnp-org:service:AVTransport:1", "GetCurrentTransportActions", body);
	free(body);
	free(Actions);
}

/*! \fn UPnPResponse_AVTransport_GetDeviceCapabilities(const UPnPSessionToken UPnPToken, const char* unescaped_PlayMedia, const char* unescaped_RecMedia, const char* unescaped_RecQualityModes)
\brief Response Method for AVTransport >> urn:schemas-upnp-org:service:AVTransport:1 >> GetDeviceCapabilities
\param UPnPToken MicroStack token
\param unescaped_PlayMedia Value of argument PlayMedia \b     Note: Automatically Escaped
\param unescaped_RecMedia Value of argument RecMedia \b     Note: Automatically Escaped
\param unescaped_RecQualityModes Value of argument RecQualityModes \b     Note: Automatically Escaped
*/
void UPnPResponse_AVTransport_GetDeviceCapabilities(const UPnPSessionToken UPnPToken, const char* unescaped_PlayMedia, const char* unescaped_RecMedia, const char* unescaped_RecQualityModes)
{
	char* body;
	char *PlayMedia = (char*)malloc(1+ILibXmlEscapeLength(unescaped_PlayMedia));
	char *RecMedia = (char*)malloc(1+ILibXmlEscapeLength(unescaped_RecMedia));
	char *RecQualityModes = (char*)malloc(1+ILibXmlEscapeLength(unescaped_RecQualityModes));
	
	ILibXmlEscape(PlayMedia, unescaped_PlayMedia);
	ILibXmlEscape(RecMedia, unescaped_RecMedia);
	ILibXmlEscape(RecQualityModes, unescaped_RecQualityModes);
	if ((body = (char*)malloc(80+strlen(PlayMedia)+strlen(RecMedia)+strlen(RecQualityModes))) == NULL) ILIBCRITICALEXIT(254);
	snprintf(body, 80+strlen(PlayMedia)+strlen(RecMedia)+strlen(RecQualityModes), "<PlayMedia>%s</PlayMedia><RecMedia>%s</RecMedia><RecQualityModes>%s</RecQualityModes>", PlayMedia, RecMedia, RecQualityModes);
	UPnPResponseGeneric(UPnPToken, "urn:schemas-upnp-org:service:AVTransport:1", "GetDeviceCapabilities", body);
	free(body);
	free(PlayMedia);
	free(RecMedia);
	free(RecQualityModes);
}

/*! \fn UPnPResponse_AVTransport_GetMediaInfo(const UPnPSessionToken UPnPToken, const unsigned int NrTracks, const char* unescaped_MediaDuration, const char* unescaped_CurrentURI, const char* unescaped_CurrentURIMetaData, const char* unescaped_NextURI, const char* unescaped_NextURIMetaData, const char* unescaped_PlayMedium, const char* unescaped_RecordMedium, const char* unescaped_WriteStatus)
\brief Response Method for AVTransport >> urn:schemas-upnp-org:service:AVTransport:1 >> GetMediaInfo
\param UPnPToken MicroStack token
\param NrTracks Value of argument NrTracks
\param unescaped_MediaDuration Value of argument MediaDuration \b     Note: Automatically Escaped
\param unescaped_CurrentURI Value of argument CurrentURI \b     Note: Automatically Escaped
\param unescaped_CurrentURIMetaData Value of argument CurrentURIMetaData \b     Note: Automatically Escaped
\param unescaped_NextURI Value of argument NextURI \b     Note: Automatically Escaped
\param unescaped_NextURIMetaData Value of argument NextURIMetaData \b     Note: Automatically Escaped
\param unescaped_PlayMedium Value of argument PlayMedium \b     Note: Automatically Escaped
\param unescaped_RecordMedium Value of argument RecordMedium \b     Note: Automatically Escaped
\param unescaped_WriteStatus Value of argument WriteStatus \b     Note: Automatically Escaped
*/
void UPnPResponse_AVTransport_GetMediaInfo(const UPnPSessionToken UPnPToken, const unsigned int NrTracks, const char* unescaped_MediaDuration, const char* unescaped_CurrentURI, const char* unescaped_CurrentURIMetaData, const char* unescaped_NextURI, const char* unescaped_NextURIMetaData, const char* unescaped_PlayMedium, const char* unescaped_RecordMedium, const char* unescaped_WriteStatus)
{
	char* body;
	char *MediaDuration = (char*)malloc(1+ILibXmlEscapeLength(unescaped_MediaDuration));
	char *CurrentURI = (char*)malloc(1+ILibXmlEscapeLength(unescaped_CurrentURI));
	char *CurrentURIMetaData = (char*)malloc(1+ILibXmlEscapeLength(unescaped_CurrentURIMetaData));
	char *NextURI = (char*)malloc(1+ILibXmlEscapeLength(unescaped_NextURI));
	char *NextURIMetaData = (char*)malloc(1+ILibXmlEscapeLength(unescaped_NextURIMetaData));
	char *PlayMedium = (char*)malloc(1+ILibXmlEscapeLength(unescaped_PlayMedium));
	char *RecordMedium = (char*)malloc(1+ILibXmlEscapeLength(unescaped_RecordMedium));
	char *WriteStatus = (char*)malloc(1+ILibXmlEscapeLength(unescaped_WriteStatus));
	
	ILibXmlEscape(MediaDuration, unescaped_MediaDuration);
	ILibXmlEscape(CurrentURI, unescaped_CurrentURI);
	ILibXmlEscape(CurrentURIMetaData, unescaped_CurrentURIMetaData);
	ILibXmlEscape(NextURI, unescaped_NextURI);
	ILibXmlEscape(NextURIMetaData, unescaped_NextURIMetaData);
	ILibXmlEscape(PlayMedium, unescaped_PlayMedium);
	ILibXmlEscape(RecordMedium, unescaped_RecordMedium);
	ILibXmlEscape(WriteStatus, unescaped_WriteStatus);
	if ((body = (char*)malloc(265+strlen(MediaDuration)+strlen(CurrentURI)+strlen(CurrentURIMetaData)+strlen(NextURI)+strlen(NextURIMetaData)+strlen(PlayMedium)+strlen(RecordMedium)+strlen(WriteStatus))) == NULL) ILIBCRITICALEXIT(254);
	snprintf(body, 265+strlen(MediaDuration)+strlen(CurrentURI)+strlen(CurrentURIMetaData)+strlen(NextURI)+strlen(NextURIMetaData)+strlen(PlayMedium)+strlen(RecordMedium)+strlen(WriteStatus), "<NrTracks>%u</NrTracks><MediaDuration>%s</MediaDuration><CurrentURI>%s</CurrentURI><CurrentURIMetaData>%s</CurrentURIMetaData><NextURI>%s</NextURI><NextURIMetaData>%s</NextURIMetaData><PlayMedium>%s</PlayMedium><RecordMedium>%s</RecordMedium><WriteStatus>%s</WriteStatus>", NrTracks, MediaDuration, CurrentURI, CurrentURIMetaData, NextURI, NextURIMetaData, PlayMedium, RecordMedium, WriteStatus);
	UPnPResponseGeneric(UPnPToken, "urn:schemas-upnp-org:service:AVTransport:1", "GetMediaInfo", body);
	free(body);
	free(MediaDuration);
	free(CurrentURI);
	free(CurrentURIMetaData);
	free(NextURI);
	free(NextURIMetaData);
	free(PlayMedium);
	free(RecordMedium);
	free(WriteStatus);
}

/*! \fn UPnPResponse_AVTransport_GetPositionInfo(const UPnPSessionToken UPnPToken, const unsigned int Track, const char* unescaped_TrackDuration, const char* unescaped_TrackMetaData, const char* unescaped_TrackURI, const char* unescaped_RelTime, const char* unescaped_AbsTime, const int RelCount, const int AbsCount)
\brief Response Method for AVTransport >> urn:schemas-upnp-org:service:AVTransport:1 >> GetPositionInfo
\param UPnPToken MicroStack token
\param Track Value of argument Track
\param unescaped_TrackDuration Value of argument TrackDuration \b     Note: Automatically Escaped
\param unescaped_TrackMetaData Value of argument TrackMetaData \b     Note: Automatically Escaped
\param unescaped_TrackURI Value of argument TrackURI \b     Note: Automatically Escaped
\param unescaped_RelTime Value of argument RelTime \b     Note: Automatically Escaped
\param unescaped_AbsTime Value of argument AbsTime \b     Note: Automatically Escaped
\param RelCount Value of argument RelCount
\param AbsCount Value of argument AbsCount
*/
void UPnPResponse_AVTransport_GetPositionInfo(const UPnPSessionToken UPnPToken, const unsigned int Track, const char* unescaped_TrackDuration, const char* unescaped_TrackMetaData, const char* unescaped_TrackURI, const char* unescaped_RelTime, const char* unescaped_AbsTime, const int RelCount, const int AbsCount)
{
	char* body;
	char *TrackDuration = (char*)malloc(1+ILibXmlEscapeLength(unescaped_TrackDuration));
	char *TrackMetaData = (char*)malloc(1+ILibXmlEscapeLength(unescaped_TrackMetaData));
	char *TrackURI = (char*)malloc(1+ILibXmlEscapeLength(unescaped_TrackURI));
	char *RelTime = (char*)malloc(1+ILibXmlEscapeLength(unescaped_RelTime));
	char *AbsTime = (char*)malloc(1+ILibXmlEscapeLength(unescaped_AbsTime));
	
	ILibXmlEscape(TrackDuration, unescaped_TrackDuration);
	ILibXmlEscape(TrackMetaData, unescaped_TrackMetaData);
	ILibXmlEscape(TrackURI, unescaped_TrackURI);
	ILibXmlEscape(RelTime, unescaped_RelTime);
	ILibXmlEscape(AbsTime, unescaped_AbsTime);
	if ((body = (char*)malloc(212+strlen(TrackDuration)+strlen(TrackMetaData)+strlen(TrackURI)+strlen(RelTime)+strlen(AbsTime))) == NULL) ILIBCRITICALEXIT(254);
	snprintf(body, 212+strlen(TrackDuration)+strlen(TrackMetaData)+strlen(TrackURI)+strlen(RelTime)+strlen(AbsTime), "<Track>%u</Track><TrackDuration>%s</TrackDuration><TrackMetaData>%s</TrackMetaData><TrackURI>%s</TrackURI><RelTime>%s</RelTime><AbsTime>%s</AbsTime><RelCount>%d</RelCount><AbsCount>%d</AbsCount>", Track, TrackDuration, TrackMetaData, TrackURI, RelTime, AbsTime, RelCount, AbsCount);
	UPnPResponseGeneric(UPnPToken, "urn:schemas-upnp-org:service:AVTransport:1", "GetPositionInfo", body);
	free(body);
	free(TrackDuration);
	free(TrackMetaData);
	free(TrackURI);
	free(RelTime);
	free(AbsTime);
}

/*! \fn UPnPResponse_AVTransport_GetTransportInfo(const UPnPSessionToken UPnPToken, const char* unescaped_CurrentTransportState, const char* unescaped_CurrentTransportStatus, const char* unescaped_CurrentSpeed)
\brief Response Method for AVTransport >> urn:schemas-upnp-org:service:AVTransport:1 >> GetTransportInfo
\param UPnPToken MicroStack token
\param unescaped_CurrentTransportState Value of argument CurrentTransportState \b     Note: Automatically Escaped
\param unescaped_CurrentTransportStatus Value of argument CurrentTransportStatus \b     Note: Automatically Escaped
\param unescaped_CurrentSpeed Value of argument CurrentSpeed \b     Note: Automatically Escaped
*/
void UPnPResponse_AVTransport_GetTransportInfo(const UPnPSessionToken UPnPToken, const char* unescaped_CurrentTransportState, const char* unescaped_CurrentTransportStatus, const char* unescaped_CurrentSpeed)
{
	char* body;
	char *CurrentTransportState = (char*)malloc(1+ILibXmlEscapeLength(unescaped_CurrentTransportState));
	char *CurrentTransportStatus = (char*)malloc(1+ILibXmlEscapeLength(unescaped_CurrentTransportStatus));
	char *CurrentSpeed = (char*)malloc(1+ILibXmlEscapeLength(unescaped_CurrentSpeed));
	
	ILibXmlEscape(CurrentTransportState, unescaped_CurrentTransportState);
	ILibXmlEscape(CurrentTransportStatus, unescaped_CurrentTransportStatus);
	ILibXmlEscape(CurrentSpeed, unescaped_CurrentSpeed);
	if ((body = (char*)malloc(126+strlen(CurrentTransportState)+strlen(CurrentTransportStatus)+strlen(CurrentSpeed))) == NULL) ILIBCRITICALEXIT(254);
	snprintf(body, 126+strlen(CurrentTransportState)+strlen(CurrentTransportStatus)+strlen(CurrentSpeed), "<CurrentTransportState>%s</CurrentTransportState><CurrentTransportStatus>%s</CurrentTransportStatus><CurrentSpeed>%s</CurrentSpeed>", CurrentTransportState, CurrentTransportStatus, CurrentSpeed);
	UPnPResponseGeneric(UPnPToken, "urn:schemas-upnp-org:service:AVTransport:1", "GetTransportInfo", body);
	free(body);
	free(CurrentTransportState);
	free(CurrentTransportStatus);
	free(CurrentSpeed);
}

/*! \fn UPnPResponse_AVTransport_GetTransportSettings(const UPnPSessionToken UPnPToken, const char* unescaped_PlayMode, const char* unescaped_RecQualityMode)
\brief Response Method for AVTransport >> urn:schemas-upnp-org:service:AVTransport:1 >> GetTransportSettings
\param UPnPToken MicroStack token
\param unescaped_PlayMode Value of argument PlayMode \b     Note: Automatically Escaped
\param unescaped_RecQualityMode Value of argument RecQualityMode \b     Note: Automatically Escaped
*/
void UPnPResponse_AVTransport_GetTransportSettings(const UPnPSessionToken UPnPToken, const char* unescaped_PlayMode, const char* unescaped_RecQualityMode)
{
	char* body;
	char *PlayMode = (char*)malloc(1+ILibXmlEscapeLength(unescaped_PlayMode));
	char *RecQualityMode = (char*)malloc(1+ILibXmlEscapeLength(unescaped_RecQualityMode));
	
	ILibXmlEscape(PlayMode, unescaped_PlayMode);
	ILibXmlEscape(RecQualityMode, unescaped_RecQualityMode);
	if ((body = (char*)malloc(55+strlen(PlayMode)+strlen(RecQualityMode))) == NULL) ILIBCRITICALEXIT(254);
	snprintf(body, 55+strlen(PlayMode)+strlen(RecQualityMode), "<PlayMode>%s</PlayMode><RecQualityMode>%s</RecQualityMode>", PlayMode, RecQualityMode);
	UPnPResponseGeneric(UPnPToken, "urn:schemas-upnp-org:service:AVTransport:1", "GetTransportSettings", body);
	free(body);
	free(PlayMode);
	free(RecQualityMode);
}

/*! \fn UPnPResponse_AVTransport_Next(const UPnPSessionToken UPnPToken)
\brief Response Method for AVTransport >> urn:schemas-upnp-org:service:AVTransport:1 >> Next
\param UPnPToken MicroStack token
*/
void UPnPResponse_AVTransport_Next(const UPnPSessionToken UPnPToken)
{
	UPnPResponseGeneric(UPnPToken,"urn:schemas-upnp-org:service:AVTransport:1","Next","");
}

/*! \fn UPnPResponse_AVTransport_Pause(const UPnPSessionToken UPnPToken)
\brief Response Method for AVTransport >> urn:schemas-upnp-org:service:AVTransport:1 >> Pause
\param UPnPToken MicroStack token
*/
void UPnPResponse_AVTransport_Pause(const UPnPSessionToken UPnPToken)
{
	UPnPResponseGeneric(UPnPToken,"urn:schemas-upnp-org:service:AVTransport:1","Pause","");
}

/*! \fn UPnPResponse_AVTransport_Play(const UPnPSessionToken UPnPToken)
\brief Response Method for AVTransport >> urn:schemas-upnp-org:service:AVTransport:1 >> Play
\param UPnPToken MicroStack token
*/
void UPnPResponse_AVTransport_Play(const UPnPSessionToken UPnPToken)
{
	UPnPResponseGeneric(UPnPToken,"urn:schemas-upnp-org:service:AVTransport:1","Play","");
}

/*! \fn UPnPResponse_AVTransport_Previous(const UPnPSessionToken UPnPToken)
\brief Response Method for AVTransport >> urn:schemas-upnp-org:service:AVTransport:1 >> Previous
\param UPnPToken MicroStack token
*/
void UPnPResponse_AVTransport_Previous(const UPnPSessionToken UPnPToken)
{
	UPnPResponseGeneric(UPnPToken,"urn:schemas-upnp-org:service:AVTransport:1","Previous","");
}

/*! \fn UPnPResponse_AVTransport_Seek(const UPnPSessionToken UPnPToken)
\brief Response Method for AVTransport >> urn:schemas-upnp-org:service:AVTransport:1 >> Seek
\param UPnPToken MicroStack token
*/
void UPnPResponse_AVTransport_Seek(const UPnPSessionToken UPnPToken)
{
	UPnPResponseGeneric(UPnPToken,"urn:schemas-upnp-org:service:AVTransport:1","Seek","");
}

/*! \fn UPnPResponse_AVTransport_SetAVTransportURI(const UPnPSessionToken UPnPToken)
\brief Response Method for AVTransport >> urn:schemas-upnp-org:service:AVTransport:1 >> SetAVTransportURI
\param UPnPToken MicroStack token
*/
void UPnPResponse_AVTransport_SetAVTransportURI(const UPnPSessionToken UPnPToken)
{
	UPnPResponseGeneric(UPnPToken,"urn:schemas-upnp-org:service:AVTransport:1","SetAVTransportURI","");
}

/*! \fn UPnPResponse_AVTransport_SetPlayMode(const UPnPSessionToken UPnPToken)
\brief Response Method for AVTransport >> urn:schemas-upnp-org:service:AVTransport:1 >> SetPlayMode
\param UPnPToken MicroStack token
*/
void UPnPResponse_AVTransport_SetPlayMode(const UPnPSessionToken UPnPToken)
{
	UPnPResponseGeneric(UPnPToken,"urn:schemas-upnp-org:service:AVTransport:1","SetPlayMode","");
}

/*! \fn UPnPResponse_AVTransport_Stop(const UPnPSessionToken UPnPToken)
\brief Response Method for AVTransport >> urn:schemas-upnp-org:service:AVTransport:1 >> Stop
\param UPnPToken MicroStack token
*/
void UPnPResponse_AVTransport_Stop(const UPnPSessionToken UPnPToken)
{
	UPnPResponseGeneric(UPnPToken,"urn:schemas-upnp-org:service:AVTransport:1","Stop","");
}

/*! \fn UPnPResponse_ConnectionManager_GetCurrentConnectionIDs(const UPnPSessionToken UPnPToken, const char* unescaped_ConnectionIDs)
\brief Response Method for ConnectionManager >> urn:schemas-upnp-org:service:ConnectionManager:1 >> GetCurrentConnectionIDs
\param UPnPToken MicroStack token
\param unescaped_ConnectionIDs Value of argument ConnectionIDs \b     Note: Automatically Escaped
*/
void UPnPResponse_ConnectionManager_GetCurrentConnectionIDs(const UPnPSessionToken UPnPToken, const char* unescaped_ConnectionIDs)
{
	char* body;
	char *ConnectionIDs = (char*)malloc(1+ILibXmlEscapeLength(unescaped_ConnectionIDs));
	
	ILibXmlEscape(ConnectionIDs, unescaped_ConnectionIDs);
	if ((body = (char*)malloc(32+strlen(ConnectionIDs))) == NULL) ILIBCRITICALEXIT(254);
	snprintf(body, 32+strlen(ConnectionIDs), "<ConnectionIDs>%s</ConnectionIDs>", ConnectionIDs);
	UPnPResponseGeneric(UPnPToken, "urn:schemas-upnp-org:service:ConnectionManager:1", "GetCurrentConnectionIDs", body);
	free(body);
	free(ConnectionIDs);
}

/*! \fn UPnPResponse_ConnectionManager_GetCurrentConnectionInfo(const UPnPSessionToken UPnPToken, const int RcsID, const int AVTransportID, const char* unescaped_ProtocolInfo, const char* unescaped_PeerConnectionManager, const int PeerConnectionID, const char* unescaped_Direction, const char* unescaped_Status)
\brief Response Method for ConnectionManager >> urn:schemas-upnp-org:service:ConnectionManager:1 >> GetCurrentConnectionInfo
\param UPnPToken MicroStack token
\param RcsID Value of argument RcsID
\param AVTransportID Value of argument AVTransportID
\param unescaped_ProtocolInfo Value of argument ProtocolInfo \b     Note: Automatically Escaped
\param unescaped_PeerConnectionManager Value of argument PeerConnectionManager \b     Note: Automatically Escaped
\param PeerConnectionID Value of argument PeerConnectionID
\param unescaped_Direction Value of argument Direction \b     Note: Automatically Escaped
\param unescaped_Status Value of argument Status \b     Note: Automatically Escaped
*/
void UPnPResponse_ConnectionManager_GetCurrentConnectionInfo(const UPnPSessionToken UPnPToken, const int RcsID, const int AVTransportID, const char* unescaped_ProtocolInfo, const char* unescaped_PeerConnectionManager, const int PeerConnectionID, const char* unescaped_Direction, const char* unescaped_Status)
{
	char* body;
	char *ProtocolInfo = (char*)malloc(1+ILibXmlEscapeLength(unescaped_ProtocolInfo));
	char *PeerConnectionManager = (char*)malloc(1+ILibXmlEscapeLength(unescaped_PeerConnectionManager));
	char *Direction = (char*)malloc(1+ILibXmlEscapeLength(unescaped_Direction));
	char *Status = (char*)malloc(1+ILibXmlEscapeLength(unescaped_Status));
	
	ILibXmlEscape(ProtocolInfo, unescaped_ProtocolInfo);
	ILibXmlEscape(PeerConnectionManager, unescaped_PeerConnectionManager);
	ILibXmlEscape(Direction, unescaped_Direction);
	ILibXmlEscape(Status, unescaped_Status);
	if ((body = (char*)malloc(233+strlen(ProtocolInfo)+strlen(PeerConnectionManager)+strlen(Direction)+strlen(Status))) == NULL) ILIBCRITICALEXIT(254);
	snprintf(body, 233+strlen(ProtocolInfo)+strlen(PeerConnectionManager)+strlen(Direction)+strlen(Status), "<RcsID>%d</RcsID><AVTransportID>%d</AVTransportID><ProtocolInfo>%s</ProtocolInfo><PeerConnectionManager>%s</PeerConnectionManager><PeerConnectionID>%d</PeerConnectionID><Direction>%s</Direction><Status>%s</Status>", RcsID, AVTransportID, ProtocolInfo, PeerConnectionManager, PeerConnectionID, Direction, Status);
	UPnPResponseGeneric(UPnPToken, "urn:schemas-upnp-org:service:ConnectionManager:1", "GetCurrentConnectionInfo", body);
	free(body);
	free(ProtocolInfo);
	free(PeerConnectionManager);
	free(Direction);
	free(Status);
}

/*! \fn UPnPResponse_ConnectionManager_GetProtocolInfo(const UPnPSessionToken UPnPToken, const char* unescaped_Source, const char* unescaped_Sink)
\brief Response Method for ConnectionManager >> urn:schemas-upnp-org:service:ConnectionManager:1 >> GetProtocolInfo
\param UPnPToken MicroStack token
\param unescaped_Source Value of argument Source \b     Note: Automatically Escaped
\param unescaped_Sink Value of argument Sink \b     Note: Automatically Escaped
*/
void UPnPResponse_ConnectionManager_GetProtocolInfo(const UPnPSessionToken UPnPToken, const char* unescaped_Source, const char* unescaped_Sink)
{
	char* body;
	char *Source = (char*)malloc(1+ILibXmlEscapeLength(unescaped_Source));
	char *Sink = (char*)malloc(1+ILibXmlEscapeLength(unescaped_Sink));
	
	ILibXmlEscape(Source, unescaped_Source);
	ILibXmlEscape(Sink, unescaped_Sink);
	if ((body = (char*)malloc(31+strlen(Source)+strlen(Sink))) == NULL) ILIBCRITICALEXIT(254);
	snprintf(body, 31+strlen(Source)+strlen(Sink), "<Source>%s</Source><Sink>%s</Sink>", Source, Sink);
	UPnPResponseGeneric(UPnPToken, "urn:schemas-upnp-org:service:ConnectionManager:1", "GetProtocolInfo", body);
	free(body);
	free(Source);
	free(Sink);
}

/*! \fn UPnPResponse_RenderingControl_GetMute(const UPnPSessionToken UPnPToken, const int CurrentMute)
\brief Response Method for RenderingControl >> urn:schemas-upnp-org:service:RenderingControl:1 >> GetMute
\param UPnPToken MicroStack token
\param CurrentMute Value of argument CurrentMute
*/
void UPnPResponse_RenderingControl_GetMute(const UPnPSessionToken UPnPToken, const int CurrentMute)
{
	char* body;
	
	if ((body = (char*)malloc(29)) == NULL) ILIBCRITICALEXIT(254);
	snprintf(body, 29, "<CurrentMute>%d</CurrentMute>", (CurrentMute!=0?1:0));
	UPnPResponseGeneric(UPnPToken, "urn:schemas-upnp-org:service:RenderingControl:1", "GetMute", body);
	free(body);
}

/*! \fn UPnPResponse_RenderingControl_GetVolume(const UPnPSessionToken UPnPToken, const unsigned short CurrentVolume)
\brief Response Method for RenderingControl >> urn:schemas-upnp-org:service:RenderingControl:1 >> GetVolume
\param UPnPToken MicroStack token
\param CurrentVolume Value of argument CurrentVolume
*/
void UPnPResponse_RenderingControl_GetVolume(const UPnPSessionToken UPnPToken, const unsigned short CurrentVolume)
{
	char* body;
	
	if ((body = (char*)malloc(38)) == NULL) ILIBCRITICALEXIT(254);
	snprintf(body, 38, "<CurrentVolume>%u</CurrentVolume>", CurrentVolume);
	UPnPResponseGeneric(UPnPToken, "urn:schemas-upnp-org:service:RenderingControl:1", "GetVolume", body);
	free(body);
}

/*! \fn UPnPResponse_RenderingControl_ListPresets(const UPnPSessionToken UPnPToken, const char* unescaped_CurrentPresetNameList)
\brief Response Method for RenderingControl >> urn:schemas-upnp-org:service:RenderingControl:1 >> ListPresets
\param UPnPToken MicroStack token
\param unescaped_CurrentPresetNameList Value of argument CurrentPresetNameList \b     Note: Automatically Escaped
*/
void UPnPResponse_RenderingControl_ListPresets(const UPnPSessionToken UPnPToken, const char* unescaped_CurrentPresetNameList)
{
	char* body;
	char *CurrentPresetNameList = (char*)malloc(1+ILibXmlEscapeLength(unescaped_CurrentPresetNameList));
	
	ILibXmlEscape(CurrentPresetNameList, unescaped_CurrentPresetNameList);
	if ((body = (char*)malloc(48+strlen(CurrentPresetNameList))) == NULL) ILIBCRITICALEXIT(254);
	snprintf(body, 48+strlen(CurrentPresetNameList), "<CurrentPresetNameList>%s</CurrentPresetNameList>", CurrentPresetNameList);
	UPnPResponseGeneric(UPnPToken, "urn:schemas-upnp-org:service:RenderingControl:1", "ListPresets", body);
	free(body);
	free(CurrentPresetNameList);
}

/*! \fn UPnPResponse_RenderingControl_SelectPreset(const UPnPSessionToken UPnPToken)
\brief Response Method for RenderingControl >> urn:schemas-upnp-org:service:RenderingControl:1 >> SelectPreset
\param UPnPToken MicroStack token
*/
void UPnPResponse_RenderingControl_SelectPreset(const UPnPSessionToken UPnPToken)
{
	UPnPResponseGeneric(UPnPToken,"urn:schemas-upnp-org:service:RenderingControl:1","SelectPreset","");
}

/*! \fn UPnPResponse_RenderingControl_SetMute(const UPnPSessionToken UPnPToken)
\brief Response Method for RenderingControl >> urn:schemas-upnp-org:service:RenderingControl:1 >> SetMute
\param UPnPToken MicroStack token
*/
void UPnPResponse_RenderingControl_SetMute(const UPnPSessionToken UPnPToken)
{
	UPnPResponseGeneric(UPnPToken,"urn:schemas-upnp-org:service:RenderingControl:1","SetMute","");
}

/*! \fn UPnPResponse_RenderingControl_SetVolume(const UPnPSessionToken UPnPToken)
\brief Response Method for RenderingControl >> urn:schemas-upnp-org:service:RenderingControl:1 >> SetVolume
\param UPnPToken MicroStack token
*/
void UPnPResponse_RenderingControl_SetVolume(const UPnPSessionToken UPnPToken)
{
	UPnPResponseGeneric(UPnPToken,"urn:schemas-upnp-org:service:RenderingControl:1","SetVolume","");
}

/*! \fn UPnPResponse_RenderingControl_upnpRenderer_GetMediaPlayersList(const UPnPSessionToken UPnPToken, const char* unescaped_CurrentMediaPlayer, const char* unescaped_MediaPlayersList)
\brief Response Method for RenderingControl >> urn:schemas-upnp-org:service:RenderingControl:1 >> upnpRenderer_GetMediaPlayersList
\param UPnPToken MicroStack token
\param unescaped_CurrentMediaPlayer Value of argument CurrentMediaPlayer \b     Note: Automatically Escaped
\param unescaped_MediaPlayersList Value of argument MediaPlayersList \b     Note: Automatically Escaped
*/
void UPnPResponse_RenderingControl_upnpRenderer_GetMediaPlayersList(const UPnPSessionToken UPnPToken, const char* unescaped_CurrentMediaPlayer, const char* unescaped_MediaPlayersList)
{
	char* body;
	char *CurrentMediaPlayer = (char*)malloc(1+ILibXmlEscapeLength(unescaped_CurrentMediaPlayer));
	char *MediaPlayersList = (char*)malloc(1+ILibXmlEscapeLength(unescaped_MediaPlayersList));
	
	ILibXmlEscape(CurrentMediaPlayer, unescaped_CurrentMediaPlayer);
	ILibXmlEscape(MediaPlayersList, unescaped_MediaPlayersList);
	if ((body = (char*)malloc(79+strlen(CurrentMediaPlayer)+strlen(MediaPlayersList))) == NULL) ILIBCRITICALEXIT(254);
	snprintf(body, 79+strlen(CurrentMediaPlayer)+strlen(MediaPlayersList), "<CurrentMediaPlayer>%s</CurrentMediaPlayer><MediaPlayersList>%s</MediaPlayersList>", CurrentMediaPlayer, MediaPlayersList);
	UPnPResponseGeneric(UPnPToken, "urn:schemas-upnp-org:service:RenderingControl:1", "upnpRenderer_GetMediaPlayersList", body);
	free(body);
	free(CurrentMediaPlayer);
	free(MediaPlayersList);
}

/*! \fn UPnPResponse_RenderingControl_upnpRenderer_SetMediaPlayer(const UPnPSessionToken UPnPToken)
\brief Response Method for RenderingControl >> urn:schemas-upnp-org:service:RenderingControl:1 >> upnpRenderer_SetMediaPlayer
\param UPnPToken MicroStack token
*/
void UPnPResponse_RenderingControl_upnpRenderer_SetMediaPlayer(const UPnPSessionToken UPnPToken)
{
	UPnPResponseGeneric(UPnPToken,"urn:schemas-upnp-org:service:RenderingControl:1","upnpRenderer_SetMediaPlayer","");
}


void UPnPSendEventSink(
void *WebReaderToken,
int IsInterrupt,
struct packetheader *header,
char *buffer,
int *p_BeginPointer,
int EndPointer,
int done,
void *subscriber,
void *upnp,
int *PAUSE)	
{
	UNREFERENCED_PARAMETER( WebReaderToken );
	UNREFERENCED_PARAMETER( IsInterrupt );
	UNREFERENCED_PARAMETER( buffer );
	UNREFERENCED_PARAMETER( p_BeginPointer );
	UNREFERENCED_PARAMETER( EndPointer );
	UNREFERENCED_PARAMETER( PAUSE );
	
	if (done != 0 && ((struct SubscriberInfo*)subscriber)->Disposing == 0)
	{
		sem_wait(&(((struct UPnPDataObject*)upnp)->EventLock));
		--((struct SubscriberInfo*)subscriber)->RefCount;
		if (((struct SubscriberInfo*)subscriber)->RefCount == 0)
		{
			LVL3DEBUG(printf("\r\n\r\nSubscriber at [%s] %d.%d.%d.%d:%d was/did UNSUBSCRIBE while trying to send event\r\n\r\n", ((struct SubscriberInfo*)subscriber)->SID, (((struct SubscriberInfo*)subscriber)->Address&0xFF),((((struct SubscriberInfo*)subscriber)->Address>>8)&0xFF),((((struct SubscriberInfo*)subscriber)->Address>>16)&0xFF), ((((struct SubscriberInfo*)subscriber)->Address>>24)&0xFF), ((struct SubscriberInfo*)subscriber)->Port);)
			UPnPDestructSubscriberInfo(((struct SubscriberInfo*)subscriber));
		}
		else if (header == NULL)
		{
			LVL3DEBUG(printf("\r\n\r\nCould not deliver event for [%s] %d.%d.%d.%d:%d UNSUBSCRIBING\r\n\r\n", ((struct SubscriberInfo*)subscriber)->SID, (((struct SubscriberInfo*)subscriber)->Address&0xFF),((((struct SubscriberInfo*)subscriber)->Address>>8)&0xFF), ((((struct SubscriberInfo*)subscriber)->Address>>16)&0xFF), ((((struct SubscriberInfo*)subscriber)->Address>>24)&0xFF), ((struct SubscriberInfo*)subscriber)->Port);)
			// Could not send Event, so unsubscribe the subscriber
			((struct SubscriberInfo*)subscriber)->Disposing = 1;
			UPnPExpireSubscriberInfo(upnp, subscriber);
		}
		sem_post(&(((struct UPnPDataObject*)upnp)->EventLock));
	}
}

void UPnPSendEvent_Body(void *upnptoken, char *body, int bodylength, struct SubscriberInfo *info)
{
	struct UPnPDataObject* UPnPObject = (struct UPnPDataObject*)upnptoken;
	struct sockaddr_in dest;
	int packetLength;
	char *packet;
	int ipaddr;
	int len;
	
	memset(&dest, 0, sizeof(dest));
	dest.sin_addr.s_addr = info->Address;
	dest.sin_port = htons(info->Port);
	dest.sin_family = AF_INET;
	ipaddr = info->Address;
	
	len = info->PathLength + bodylength + 483;
	if ((packet = (char*)malloc(len)) == NULL) ILIBCRITICALEXIT(254);
	packetLength = snprintf(packet, len, "NOTIFY %s HTTP/1.1\r\nSERVER: %s, UPnP/1.0, MicroStack/1.0.5329\r\nHOST: %s:%d\r\nContent-Type: text/xml; charset=\"utf-8\"\r\nNT: upnp:event\r\nNTS: upnp:propchange\r\nSID: %s\r\nSEQ: %d\r\nContent-Length: %d\r\n\r\n<?xml version=\"1.0\" encoding=\"utf-8\"?><e:propertyset xmlns:e=\"urn:schemas-upnp-org:event-1-0\"><e:property><%s></e:property></e:propertyset>",info->Path,UPnPPLATFORM,inet_ntoa(dest.sin_addr),info->Port,info->SID,info->SEQ,bodylength+137,body);
	++info->SEQ;
	
	++info->RefCount;
	ILibWebClient_PipelineRequestEx(UPnPObject->EventClient, (struct sockaddr*)(&dest), packet, packetLength, 0, NULL, 0, 0, &UPnPSendEventSink, info, upnptoken);
}

void UPnPSendEvent(void *upnptoken, char* body, const int bodylength, const char* eventname)
{
	struct SubscriberInfo *info = NULL;
	struct UPnPDataObject* UPnPObject = (struct UPnPDataObject*)upnptoken;
	struct sockaddr_in dest;
	LVL3DEBUG(struct timeval tv;)
	
	if (UPnPObject == NULL)
	{
		free(body);
		return;
	}
	sem_wait(&(UPnPObject->EventLock));
	if (strncmp(eventname,"AVTransport",11)==0)
	{
		info = UPnPObject->HeadSubscriberPtr_AVTransport;
	}
	if (strncmp(eventname,"ConnectionManager",17)==0)
	{
		info = UPnPObject->HeadSubscriberPtr_ConnectionManager;
	}
	if (strncmp(eventname,"RenderingControl",16)==0)
	{
		info = UPnPObject->HeadSubscriberPtr_RenderingControl;
	}
	
	memset(&dest, 0, sizeof(dest));
	while(info != NULL)
	{
		if (!UPnPSubscriptionExpired(info))
		{
			UPnPSendEvent_Body(upnptoken, body, bodylength, info);
		}
		else
		{
			// Remove Subscriber
			LVL3DEBUG(gettimeofday(&tv, NULL);)
			LVL3DEBUG(printf("\r\n\r\nTIMESTAMP: %d\r\n", tv.tv_sec);)
			LVL3DEBUG(printf("Did not renew [%s] %d.%d.%d.%d:%d UNSUBSCRIBING <%d>\r\n\r\n", ((struct SubscriberInfo*)info)->SID, (((struct SubscriberInfo*)info)->Address&0xFF), ((((struct SubscriberInfo*)info)->Address>>8)&0xFF),((((struct SubscriberInfo*)info)->Address>>16)&0xFF), ((((struct SubscriberInfo*)info)->Address>>24)&0xFF), ((struct SubscriberInfo*)info)->Port, info);)
		}
		info = info->Next;
	}
	sem_post(&(UPnPObject->EventLock));
}
/*! \fn UPnPSetState_AVTransport_LastChange(UPnPMicroStackToken upnptoken, char* val)
\brief Sets the state of LastChange << urn:schemas-upnp-org:service:AVTransport:1 << AVTransport \par
\b Note: Must be called at least once prior to start
\param upnptoken The MicroStack token
\param val The new value of the state variable
*/
void UPnPSetState_AVTransport_LastChange(UPnPMicroStackToken upnptoken, char* val)
{
	struct UPnPDataObject *UPnPObject = (struct UPnPDataObject*)upnptoken;
	char* body;
	int bodylength;
	char* valstr;
	if ((valstr = (char*)malloc(ILibXmlEscapeLength(val) + 1)) == NULL) ILIBCRITICALEXIT(254);
	ILibXmlEscape(valstr, val);
	if (UPnPObject->AVTransport_LastChange != NULL) free(UPnPObject->AVTransport_LastChange);
	UPnPObject->AVTransport_LastChange = valstr;
	bodylength = 30 + (int)strlen(valstr);
	if ((body = (char*)malloc(bodylength)) == NULL) ILIBCRITICALEXIT(254);
	bodylength = snprintf(body, bodylength, "%s>%s</%s", "LastChange", valstr, "LastChange");
	UPnPSendEvent(upnptoken, body, bodylength, "AVTransport");
	free(body);
}

/*! \fn UPnPSetState_ConnectionManager_SourceProtocolInfo(UPnPMicroStackToken upnptoken, char* val)
\brief Sets the state of SourceProtocolInfo << urn:schemas-upnp-org:service:ConnectionManager:1 << ConnectionManager \par
\b Note: Must be called at least once prior to start
\param upnptoken The MicroStack token
\param val The new value of the state variable
*/
void UPnPSetState_ConnectionManager_SourceProtocolInfo(UPnPMicroStackToken upnptoken, char* val)
{
	struct UPnPDataObject *UPnPObject = (struct UPnPDataObject*)upnptoken;
	char* body;
	int bodylength;
	char* valstr;
	if ((valstr = (char*)malloc(ILibXmlEscapeLength(val) + 1)) == NULL) ILIBCRITICALEXIT(254);
	ILibXmlEscape(valstr, val);
	if (UPnPObject->ConnectionManager_SourceProtocolInfo != NULL) free(UPnPObject->ConnectionManager_SourceProtocolInfo);
	UPnPObject->ConnectionManager_SourceProtocolInfo = valstr;
	bodylength = 46 + (int)strlen(valstr);
	if ((body = (char*)malloc(bodylength)) == NULL) ILIBCRITICALEXIT(254);
	bodylength = snprintf(body, bodylength, "%s>%s</%s", "SourceProtocolInfo", valstr, "SourceProtocolInfo");
	UPnPSendEvent(upnptoken, body, bodylength, "ConnectionManager");
	free(body);
}

/*! \fn UPnPSetState_ConnectionManager_SinkProtocolInfo(UPnPMicroStackToken upnptoken, char* val)
\brief Sets the state of SinkProtocolInfo << urn:schemas-upnp-org:service:ConnectionManager:1 << ConnectionManager \par
\b Note: Must be called at least once prior to start
\param upnptoken The MicroStack token
\param val The new value of the state variable
*/
void UPnPSetState_ConnectionManager_SinkProtocolInfo(UPnPMicroStackToken upnptoken, char* val)
{
	struct UPnPDataObject *UPnPObject = (struct UPnPDataObject*)upnptoken;
	char* body;
	int bodylength;
	char* valstr;
	if ((valstr = (char*)malloc(ILibXmlEscapeLength(val) + 1)) == NULL) ILIBCRITICALEXIT(254);
	ILibXmlEscape(valstr, val);
	if (UPnPObject->ConnectionManager_SinkProtocolInfo != NULL) free(UPnPObject->ConnectionManager_SinkProtocolInfo);
	UPnPObject->ConnectionManager_SinkProtocolInfo = valstr;
	bodylength = 42 + (int)strlen(valstr);
	if ((body = (char*)malloc(bodylength)) == NULL) ILIBCRITICALEXIT(254);
	bodylength = snprintf(body, bodylength, "%s>%s</%s", "SinkProtocolInfo", valstr, "SinkProtocolInfo");
	UPnPSendEvent(upnptoken, body, bodylength, "ConnectionManager");
	free(body);
}

/*! \fn UPnPSetState_ConnectionManager_CurrentConnectionIDs(UPnPMicroStackToken upnptoken, char* val)
\brief Sets the state of CurrentConnectionIDs << urn:schemas-upnp-org:service:ConnectionManager:1 << ConnectionManager \par
\b Note: Must be called at least once prior to start
\param upnptoken The MicroStack token
\param val The new value of the state variable
*/
void UPnPSetState_ConnectionManager_CurrentConnectionIDs(UPnPMicroStackToken upnptoken, char* val)
{
	struct UPnPDataObject *UPnPObject = (struct UPnPDataObject*)upnptoken;
	char* body;
	int bodylength;
	char* valstr;
	if ((valstr = (char*)malloc(ILibXmlEscapeLength(val) + 1)) == NULL) ILIBCRITICALEXIT(254);
	ILibXmlEscape(valstr, val);
	if (UPnPObject->ConnectionManager_CurrentConnectionIDs != NULL) free(UPnPObject->ConnectionManager_CurrentConnectionIDs);
	UPnPObject->ConnectionManager_CurrentConnectionIDs = valstr;
	bodylength = 50 + (int)strlen(valstr);
	if ((body = (char*)malloc(bodylength)) == NULL) ILIBCRITICALEXIT(254);
	bodylength = snprintf(body, bodylength, "%s>%s</%s", "CurrentConnectionIDs", valstr, "CurrentConnectionIDs");
	UPnPSendEvent(upnptoken, body, bodylength, "ConnectionManager");
	free(body);
}

/*! \fn UPnPSetState_RenderingControl_LastChange(UPnPMicroStackToken upnptoken, char* val)
\brief Sets the state of LastChange << urn:schemas-upnp-org:service:RenderingControl:1 << RenderingControl \par
\b Note: Must be called at least once prior to start
\param upnptoken The MicroStack token
\param val The new value of the state variable
*/
void UPnPSetState_RenderingControl_LastChange(UPnPMicroStackToken upnptoken, char* val)
{
	struct UPnPDataObject *UPnPObject = (struct UPnPDataObject*)upnptoken;
	char* body;
	int bodylength;
	char* valstr;
	if ((valstr = (char*)malloc(ILibXmlEscapeLength(val) + 1)) == NULL) ILIBCRITICALEXIT(254);
	ILibXmlEscape(valstr, val);
	if (UPnPObject->RenderingControl_LastChange != NULL) free(UPnPObject->RenderingControl_LastChange);
	UPnPObject->RenderingControl_LastChange = valstr;
	bodylength = 30 + (int)strlen(valstr);
	if ((body = (char*)malloc(bodylength)) == NULL) ILIBCRITICALEXIT(254);
	bodylength = snprintf(body, bodylength, "%s>%s</%s", "LastChange", valstr, "LastChange");
	UPnPSendEvent(upnptoken, body, bodylength, "RenderingControl");
	free(body);
}


void UPnPDestroyMicroStack(void *object)
{
	struct UPnPDataObject *upnp = (struct UPnPDataObject*)object;
	struct SubscriberInfo  *sinfo,*sinfo2;	UPnPSendByeBye(upnp);
	sem_destroy(&(upnp->EventLock));
	free(upnp->AVTransport_LastChange);
	free(upnp->ConnectionManager_SourceProtocolInfo);
	free(upnp->ConnectionManager_SinkProtocolInfo);
	free(upnp->ConnectionManager_CurrentConnectionIDs);
	free(upnp->RenderingControl_LastChange);
	
	if (upnp->AddressListV4 != NULL) free(upnp->AddressListV4);
	if (upnp->AddressListV6 != NULL) free(upnp->AddressListV6);
	if (upnp->NOTIFY_SEND_socks != NULL) free(upnp->NOTIFY_SEND_socks);
	if (upnp->NOTIFY_RECEIVE_socks != NULL) free(upnp->NOTIFY_RECEIVE_socks);
	if (upnp->NOTIFY_SEND_socks6 != NULL) free(upnp->NOTIFY_SEND_socks6);
	if (upnp->NOTIFY_RECEIVE_socks6 != NULL) free(upnp->NOTIFY_RECEIVE_socks6);
	free(upnp->UUID);
	free(upnp->Serial);
	sinfo = upnp->HeadSubscriberPtr_AVTransport;
	while(sinfo!=NULL)
	{
		sinfo2 = sinfo->Next;
		UPnPDestructSubscriberInfo(sinfo);
		sinfo = sinfo2;
	}
	sinfo = upnp->HeadSubscriberPtr_ConnectionManager;
	while(sinfo!=NULL)
	{
		sinfo2 = sinfo->Next;
		UPnPDestructSubscriberInfo(sinfo);
		sinfo = sinfo2;
	}
	sinfo = upnp->HeadSubscriberPtr_RenderingControl;
	while(sinfo!=NULL)
	{
		sinfo2 = sinfo->Next;
		UPnPDestructSubscriberInfo(sinfo);
		sinfo = sinfo2;
	}
	
}

int UPnPGetLocalPortNumber(UPnPSessionToken token)
{
	return(ILibWebServer_GetPortNumber(((struct ILibWebServer_Session*)token)->Parent));
}

void UPnPSessionReceiveSink(
struct ILibWebServer_Session *sender,
int InterruptFlag,
struct packetheader *header,
char *bodyBuffer,
int *beginPointer,
int endPointer,
int done)
{
	char *txt;
	if (header != NULL && sender->User3 == NULL && done == 0)
	{
		sender->User3 = (void*)~0;
		txt = ILibGetHeaderLine(header,"Expect",6);
		if (txt!=NULL)
		{
			if (strcasecmp(txt,"100-Continue")==0)
			{
				// Expect Continue
				ILibWebServer_Send_Raw(sender,"HTTP/1.1 100 Continue\r\n\r\n",25,ILibAsyncSocket_MemoryOwnership_STATIC,0);
			}
			else
			{
				// Don't understand
				ILibWebServer_Send_Raw(sender,"HTTP/1.1 417 Expectation Failed\r\n\r\n",35,ILibAsyncSocket_MemoryOwnership_STATIC,1);
				ILibWebServer_DisconnectSession(sender);
				return;
			}
		}
	}
	if (header != NULL && done !=0 && InterruptFlag == 0)
	{
		UPnPProcessHTTPPacket(sender, header, bodyBuffer, beginPointer == NULL?0:*beginPointer, endPointer);
		if (beginPointer!=NULL) {*beginPointer = endPointer;}
	}
}
void UPnPSessionSink(struct ILibWebServer_Session *SessionToken, void *user)
{
	SessionToken->OnReceive = &UPnPSessionReceiveSink;
	SessionToken->User = user;
}

void UPnPSetTag(const UPnPMicroStackToken token, void *UserToken)
{
	((struct UPnPDataObject*)token)->User = UserToken;
}

void *UPnPGetTag(const UPnPMicroStackToken token)
{
	return(((struct UPnPDataObject*)token)->User);
}

UPnPMicroStackToken UPnPGetMicroStackTokenFromSessionToken(const UPnPSessionToken token)
{
	return(((struct ILibWebServer_Session*)token)->User);
}

UPnPMicroStackToken UPnPCreateMicroStack(void *Chain, const char* FriendlyName, const char* UDN, const char* SerialNumber, const int NotifyCycleSeconds, const unsigned short PortNum)

{
	struct UPnPDataObject* RetVal;
	struct timeval tv;
	size_t len;
	if ((RetVal = (struct UPnPDataObject*)malloc(sizeof(struct UPnPDataObject))) == NULL) ILIBCRITICALEXIT(254);
	
	gettimeofday(&tv,NULL);
	srand((int)tv.tv_sec);
	UPnP_Device_MediaRenderer_Impl.FriendlyName = FriendlyName;
	UPnP_Device_MediaRenderer_Impl.UDN = UDN;
	UPnP_Device_MediaRenderer_Impl.Serial = SerialNumber;
	if (UPnP_Device_MediaRenderer_Impl.Manufacturer == NULL) {UPnP_Device_MediaRenderer_Impl.Manufacturer = "(: JOBnik! :)";}
	if (UPnP_Device_MediaRenderer_Impl.ManufacturerURL == NULL) {UPnP_Device_MediaRenderer_Impl.ManufacturerURL = "https://www.jobnik.net";}
	if (UPnP_Device_MediaRenderer_Impl.ModelDescription == NULL) {UPnP_Device_MediaRenderer_Impl.ModelDescription = "UPnP Media Renderer for Media Players";}
	if (UPnP_Device_MediaRenderer_Impl.ModelName == NULL) {UPnP_Device_MediaRenderer_Impl.ModelName = "jobnik.net Media Renderer";}
	if (UPnP_Device_MediaRenderer_Impl.ModelNumber == NULL) {UPnP_Device_MediaRenderer_Impl.ModelNumber = "";}
	if (UPnP_Device_MediaRenderer_Impl.ModelURL == NULL) {UPnP_Device_MediaRenderer_Impl.ModelURL = "https://www.jobnik.net";}
	if (UPnP_Device_MediaRenderer_Impl.ProductCode == NULL) {UPnP_Device_MediaRenderer_Impl.ProductCode = "";}
	
	// Complete State Reset
	memset(RetVal, 0, sizeof(struct UPnPDataObject));
	
	RetVal->ForceExit = 0;
	RetVal->PreSelect = &UPnPMasterPreSelect;
	RetVal->PostSelect = NULL;
	RetVal->Destroy = &UPnPDestroyMicroStack;
	RetVal->InitialNotify = 0;
	if (UDN != NULL)
	{
		len = (int)strlen(UDN) + 6;
		if ((RetVal->UUID = (char*)malloc(len)) == NULL) ILIBCRITICALEXIT(254);
		snprintf(RetVal->UUID, len, "uuid:%s", UDN);
		RetVal->UDN = RetVal->UUID + 5;
	}
	if (SerialNumber != NULL)
	{
		len = strlen(SerialNumber) + 1;
		if (len > 20) len = 20;
		if ((RetVal->Serial = (char*)malloc(len)) == NULL) ILIBCRITICALEXIT(254);
		memcpy(RetVal->Serial, SerialNumber, len);
		RetVal->Serial[len - 1] = 0;
	}
	
	RetVal->WebServerTimer = ILibCreateLifeTime(Chain);
	RetVal->HTTPServer = ILibWebServer_Create(Chain, UPNP_HTTP_MAXSOCKETS, PortNum, &UPnPSessionSink, RetVal);
	RetVal->WebSocketPortNumber = (int)ILibWebServer_GetPortNumber(RetVal->HTTPServer);
	
	
	ILibAddToChain(Chain, RetVal);
	UPnPInit(RetVal ,Chain, NotifyCycleSeconds, PortNum);
	
	RetVal->EventClient = ILibCreateWebClient(5, Chain);
	RetVal->UpdateFlag = 0;
	
	
	sem_init(&(RetVal->EventLock), 0, 1);
	UPnPGetConfiguration()->MicrostackToken=RetVal;	return(RetVal);
}








void UPnPStreamDescriptionDocument(struct ILibWebServer_Session *session)
{
	#if defined(WIN32) || defined(_WIN32_WCE)
	char *responseHeader = "\r\nCONTENT-TYPE:  text/xml; charset=\"utf-8\"\r\nServer: WINDOWS, UPnP/1.0, MicroStack/1.0.5329";
	#elif defined(__SYMBIAN32__)
	char *responseHeader = "\r\nCONTENT-TYPE:  text/xml; charset=\"utf-8\"\r\nServer: SYMBIAN, UPnP/1.0, MicroStack/1.0.5329";
	#else
	char *responseHeader = "\r\nCONTENT-TYPE:  text/xml; charset=\"utf-8\"\r\nServer: POSIX, UPnP/1.0, MicroStack/1.0.5329";
	#endif
	
	char *tempString;
	int tempStringLength;
	char *xString,*xString2;
	
	// Device
	ILibWebServer_StreamHeader_Raw(session, 200, "OK", responseHeader, 1);
	
	
	
	xString2 = ILibDecompressString((unsigned char*)UPnP_Device_MediaRenderer_Impl.Reserved,UPnP_Device_MediaRenderer_Impl.ReservedXL,UPnP_Device_MediaRenderer_Impl.ReservedUXL);
	xString = ILibString_Replace(xString2, (int)strlen(xString2), "http://255.255.255.255:255/", 27, "%s", 2);
	free(xString2);
	tempStringLength = (int)(strlen(xString) + strlen(UPnP_Device_MediaRenderer_Impl.Manufacturer) + strlen(UPnP_Device_MediaRenderer_Impl.ManufacturerURL) + strlen(UPnP_Device_MediaRenderer_Impl.ModelDescription) + strlen(UPnP_Device_MediaRenderer_Impl.ModelName)+strlen(UPnP_Device_MediaRenderer_Impl.ModelNumber)+strlen(UPnP_Device_MediaRenderer_Impl.ModelURL) + strlen(UPnP_Device_MediaRenderer_Impl.ProductCode) + strlen(UPnP_Device_MediaRenderer_Impl.FriendlyName) + strlen(UPnP_Device_MediaRenderer_Impl.UDN) + strlen(UPnP_Device_MediaRenderer_Impl.Serial));
	if ((tempString = (char*)malloc(tempStringLength)) == NULL) ILIBCRITICALEXIT(254);
	tempStringLength = snprintf(tempString, tempStringLength, xString, 
	UPnP_Device_MediaRenderer_Impl.FriendlyName,
	UPnP_Device_MediaRenderer_Impl.Manufacturer,
	UPnP_Device_MediaRenderer_Impl.ManufacturerURL,
	UPnP_Device_MediaRenderer_Impl.ModelDescription,
	UPnP_Device_MediaRenderer_Impl.ModelName,
	UPnP_Device_MediaRenderer_Impl.ModelNumber,
	UPnP_Device_MediaRenderer_Impl.ModelURL,
	UPnP_Device_MediaRenderer_Impl.Serial,
	UPnP_Device_MediaRenderer_Impl.UDN);
	free(xString);
	ILibWebServer_StreamBody(session ,tempString, tempStringLength - 19, ILibAsyncSocket_MemoryOwnership_CHAIN, 0);
	
	// Embedded Services
	ILibWebServer_StreamBody(session, "<serviceList>", 13, ILibAsyncSocket_MemoryOwnership_STATIC, 0);
	
	if (UPnP_Device_MediaRenderer_Impl.AVTransport!=NULL)
	{
		xString = ILibDecompressString((unsigned char*)UPnP_Device_MediaRenderer_Impl.AVTransport->Reserved, UPnP_Device_MediaRenderer_Impl.AVTransport->ReservedXL, UPnP_Device_MediaRenderer_Impl.AVTransport->ReservedUXL);
		ILibWebServer_StreamBody(session, xString, UPnP_Device_MediaRenderer_Impl.AVTransport->ReservedUXL, ILibAsyncSocket_MemoryOwnership_CHAIN, 0);
	}
	
	if (UPnP_Device_MediaRenderer_Impl.ConnectionManager!=NULL)
	{
		xString = ILibDecompressString((unsigned char*)UPnP_Device_MediaRenderer_Impl.ConnectionManager->Reserved, UPnP_Device_MediaRenderer_Impl.ConnectionManager->ReservedXL, UPnP_Device_MediaRenderer_Impl.ConnectionManager->ReservedUXL);
		ILibWebServer_StreamBody(session, xString, UPnP_Device_MediaRenderer_Impl.ConnectionManager->ReservedUXL, ILibAsyncSocket_MemoryOwnership_CHAIN, 0);
	}
	
	if (UPnP_Device_MediaRenderer_Impl.RenderingControl!=NULL)
	{
		xString = ILibDecompressString((unsigned char*)UPnP_Device_MediaRenderer_Impl.RenderingControl->Reserved, UPnP_Device_MediaRenderer_Impl.RenderingControl->ReservedXL, UPnP_Device_MediaRenderer_Impl.RenderingControl->ReservedUXL);
		ILibWebServer_StreamBody(session, xString, UPnP_Device_MediaRenderer_Impl.RenderingControl->ReservedUXL, ILibAsyncSocket_MemoryOwnership_CHAIN, 0);
	}
	ILibWebServer_StreamBody(session,"</serviceList>", 14, ILibAsyncSocket_MemoryOwnership_STATIC, 0);
	ILibWebServer_StreamBody(session,"</device>", 9, ILibAsyncSocket_MemoryOwnership_STATIC, 0);
	
	ILibWebServer_StreamBody(session,"</root>", 7, ILibAsyncSocket_MemoryOwnership_STATIC, 1);
}
struct UPnP_Device_MediaRenderer* UPnPGetConfiguration()
{
	return(&(UPnP_Device_MediaRenderer_Impl));
}


