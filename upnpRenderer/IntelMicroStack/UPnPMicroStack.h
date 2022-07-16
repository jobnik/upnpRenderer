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

#ifndef __UPnPMicrostack__
#define __UPnPMicrostack__


#include "ILibAsyncSocket.h"

/*! \file UPnPMicroStack.h 
	\brief MicroStack APIs for Device Implementation
*/

/*! \defgroup MicroStack MicroStack Module
	\{
*/

struct UPnPDataObject;
struct packetheader;

typedef void* UPnPMicroStackToken;
typedef void* UPnPSessionToken;



/* Complex Type Parsers */


/* Complex Type Serializers */



/* UPnP Stack Management */
UPnPMicroStackToken UPnPCreateMicroStack(void *Chain, const char* FriendlyName, const char* UDN, const char* SerialNumber, const int NotifyCycleSeconds, const unsigned short PortNum);


void UPnPIPAddressListChanged(UPnPMicroStackToken MicroStackToken);
int UPnPGetLocalPortNumber(UPnPSessionToken token);
void* UPnPGetWebServerToken(const UPnPMicroStackToken MicroStackToken);
void UPnPSetTag(const UPnPMicroStackToken token, void *UserToken);
void *UPnPGetTag(const UPnPMicroStackToken token);
UPnPMicroStackToken UPnPGetMicroStackTokenFromSessionToken(const UPnPSessionToken token);

typedef void(*UPnP_ActionHandler_AVTransport_GetCurrentTransportActions) (void* upnptoken,unsigned int InstanceID);
typedef void(*UPnP_ActionHandler_AVTransport_GetDeviceCapabilities) (void* upnptoken,unsigned int InstanceID);
typedef void(*UPnP_ActionHandler_AVTransport_GetMediaInfo) (void* upnptoken,unsigned int InstanceID);
typedef void(*UPnP_ActionHandler_AVTransport_GetPositionInfo) (void* upnptoken,unsigned int InstanceID);
typedef void(*UPnP_ActionHandler_AVTransport_GetTransportInfo) (void* upnptoken,unsigned int InstanceID);
typedef void(*UPnP_ActionHandler_AVTransport_GetTransportSettings) (void* upnptoken,unsigned int InstanceID);
typedef void(*UPnP_ActionHandler_AVTransport_Next) (void* upnptoken,unsigned int InstanceID);
typedef void(*UPnP_ActionHandler_AVTransport_Pause) (void* upnptoken,unsigned int InstanceID);
typedef void(*UPnP_ActionHandler_AVTransport_Play) (void* upnptoken,unsigned int InstanceID,char* Speed);
typedef void(*UPnP_ActionHandler_AVTransport_Previous) (void* upnptoken,unsigned int InstanceID);
typedef void(*UPnP_ActionHandler_AVTransport_Seek) (void* upnptoken,unsigned int InstanceID,char* Unit,char* Target);
typedef void(*UPnP_ActionHandler_AVTransport_SetAVTransportURI) (void* upnptoken,unsigned int InstanceID,char* CurrentURI,char* CurrentURIMetaData);
typedef void(*UPnP_ActionHandler_AVTransport_SetPlayMode) (void* upnptoken,unsigned int InstanceID,char* NewPlayMode);
typedef void(*UPnP_ActionHandler_AVTransport_Stop) (void* upnptoken,unsigned int InstanceID);
typedef void(*UPnP_ActionHandler_ConnectionManager_GetCurrentConnectionIDs) (void* upnptoken);
typedef void(*UPnP_ActionHandler_ConnectionManager_GetCurrentConnectionInfo) (void* upnptoken,int ConnectionID);
typedef void(*UPnP_ActionHandler_ConnectionManager_GetProtocolInfo) (void* upnptoken);
typedef void(*UPnP_ActionHandler_RenderingControl_GetMute) (void* upnptoken,unsigned int InstanceID,char* Channel);
typedef void(*UPnP_ActionHandler_RenderingControl_GetVolume) (void* upnptoken,unsigned int InstanceID,char* Channel);
typedef void(*UPnP_ActionHandler_RenderingControl_ListPresets) (void* upnptoken,unsigned int InstanceID);
typedef void(*UPnP_ActionHandler_RenderingControl_SelectPreset) (void* upnptoken,unsigned int InstanceID,char* PresetName);
typedef void(*UPnP_ActionHandler_RenderingControl_SetMute) (void* upnptoken,unsigned int InstanceID,char* Channel,int DesiredMute);
typedef void(*UPnP_ActionHandler_RenderingControl_SetVolume) (void* upnptoken,unsigned int InstanceID,char* Channel,unsigned short DesiredVolume);
typedef void(*UPnP_ActionHandler_RenderingControl_upnpRenderer_GetMediaPlayersList) (void* upnptoken,unsigned int InstanceID);
typedef void(*UPnP_ActionHandler_RenderingControl_upnpRenderer_SetMediaPlayer) (void* upnptoken,unsigned int InstanceID,char* MediaPlayerName);
/* UPnP Set Function Pointers Methods */
extern void (*UPnPFP_PresentationPage) (void* upnptoken,struct packetheader *packet);
extern UPnP_ActionHandler_AVTransport_GetCurrentTransportActions UPnPFP_AVTransport_GetCurrentTransportActions;
extern UPnP_ActionHandler_AVTransport_GetDeviceCapabilities UPnPFP_AVTransport_GetDeviceCapabilities;
extern UPnP_ActionHandler_AVTransport_GetMediaInfo UPnPFP_AVTransport_GetMediaInfo;
extern UPnP_ActionHandler_AVTransport_GetPositionInfo UPnPFP_AVTransport_GetPositionInfo;
extern UPnP_ActionHandler_AVTransport_GetTransportInfo UPnPFP_AVTransport_GetTransportInfo;
extern UPnP_ActionHandler_AVTransport_GetTransportSettings UPnPFP_AVTransport_GetTransportSettings;
extern UPnP_ActionHandler_AVTransport_Next UPnPFP_AVTransport_Next;
extern UPnP_ActionHandler_AVTransport_Pause UPnPFP_AVTransport_Pause;
extern UPnP_ActionHandler_AVTransport_Play UPnPFP_AVTransport_Play;
extern UPnP_ActionHandler_AVTransport_Previous UPnPFP_AVTransport_Previous;
extern UPnP_ActionHandler_AVTransport_Seek UPnPFP_AVTransport_Seek;
extern UPnP_ActionHandler_AVTransport_SetAVTransportURI UPnPFP_AVTransport_SetAVTransportURI;
extern UPnP_ActionHandler_AVTransport_SetPlayMode UPnPFP_AVTransport_SetPlayMode;
extern UPnP_ActionHandler_AVTransport_Stop UPnPFP_AVTransport_Stop;
extern UPnP_ActionHandler_ConnectionManager_GetCurrentConnectionIDs UPnPFP_ConnectionManager_GetCurrentConnectionIDs;
extern UPnP_ActionHandler_ConnectionManager_GetCurrentConnectionInfo UPnPFP_ConnectionManager_GetCurrentConnectionInfo;
extern UPnP_ActionHandler_ConnectionManager_GetProtocolInfo UPnPFP_ConnectionManager_GetProtocolInfo;
extern UPnP_ActionHandler_RenderingControl_GetMute UPnPFP_RenderingControl_GetMute;
extern UPnP_ActionHandler_RenderingControl_GetVolume UPnPFP_RenderingControl_GetVolume;
extern UPnP_ActionHandler_RenderingControl_ListPresets UPnPFP_RenderingControl_ListPresets;
extern UPnP_ActionHandler_RenderingControl_SelectPreset UPnPFP_RenderingControl_SelectPreset;
extern UPnP_ActionHandler_RenderingControl_SetMute UPnPFP_RenderingControl_SetMute;
extern UPnP_ActionHandler_RenderingControl_SetVolume UPnPFP_RenderingControl_SetVolume;
extern UPnP_ActionHandler_RenderingControl_upnpRenderer_GetMediaPlayersList UPnPFP_RenderingControl_upnpRenderer_GetMediaPlayersList;
extern UPnP_ActionHandler_RenderingControl_upnpRenderer_SetMediaPlayer UPnPFP_RenderingControl_upnpRenderer_SetMediaPlayer;


void UPnPSetDisconnectFlag(UPnPSessionToken token,void *flag);

/* Invocation Response Methods */
void UPnPResponse_Error(const UPnPSessionToken UPnPToken, const int ErrorCode, const char* ErrorMsg);
void UPnPResponseGeneric(const UPnPSessionToken UPnPToken,const char* ServiceURI,const char* MethodName,const char* Params);
void UPnPResponse_AVTransport_GetCurrentTransportActions(const UPnPSessionToken UPnPToken, const char* Actions);
void UPnPResponse_AVTransport_GetDeviceCapabilities(const UPnPSessionToken UPnPToken, const char* PlayMedia, const char* RecMedia, const char* RecQualityModes);
void UPnPResponse_AVTransport_GetMediaInfo(const UPnPSessionToken UPnPToken, const unsigned int NrTracks, const char* MediaDuration, const char* CurrentURI, const char* CurrentURIMetaData, const char* NextURI, const char* NextURIMetaData, const char* PlayMedium, const char* RecordMedium, const char* WriteStatus);
void UPnPResponse_AVTransport_GetPositionInfo(const UPnPSessionToken UPnPToken, const unsigned int Track, const char* TrackDuration, const char* TrackMetaData, const char* TrackURI, const char* RelTime, const char* AbsTime, const int RelCount, const int AbsCount);
void UPnPResponse_AVTransport_GetTransportInfo(const UPnPSessionToken UPnPToken, const char* CurrentTransportState, const char* CurrentTransportStatus, const char* CurrentSpeed);
void UPnPResponse_AVTransport_GetTransportSettings(const UPnPSessionToken UPnPToken, const char* PlayMode, const char* RecQualityMode);
void UPnPResponse_AVTransport_Next(const UPnPSessionToken UPnPToken);
void UPnPResponse_AVTransport_Pause(const UPnPSessionToken UPnPToken);
void UPnPResponse_AVTransport_Play(const UPnPSessionToken UPnPToken);
void UPnPResponse_AVTransport_Previous(const UPnPSessionToken UPnPToken);
void UPnPResponse_AVTransport_Seek(const UPnPSessionToken UPnPToken);
void UPnPResponse_AVTransport_SetAVTransportURI(const UPnPSessionToken UPnPToken);
void UPnPResponse_AVTransport_SetPlayMode(const UPnPSessionToken UPnPToken);
void UPnPResponse_AVTransport_Stop(const UPnPSessionToken UPnPToken);
void UPnPResponse_ConnectionManager_GetCurrentConnectionIDs(const UPnPSessionToken UPnPToken, const char* ConnectionIDs);
void UPnPResponse_ConnectionManager_GetCurrentConnectionInfo(const UPnPSessionToken UPnPToken, const int RcsID, const int AVTransportID, const char* ProtocolInfo, const char* PeerConnectionManager, const int PeerConnectionID, const char* Direction, const char* Status);
void UPnPResponse_ConnectionManager_GetProtocolInfo(const UPnPSessionToken UPnPToken, const char* Source, const char* Sink);
void UPnPResponse_RenderingControl_GetMute(const UPnPSessionToken UPnPToken, const int CurrentMute);
void UPnPResponse_RenderingControl_GetVolume(const UPnPSessionToken UPnPToken, const unsigned short CurrentVolume);
void UPnPResponse_RenderingControl_ListPresets(const UPnPSessionToken UPnPToken, const char* CurrentPresetNameList);
void UPnPResponse_RenderingControl_SelectPreset(const UPnPSessionToken UPnPToken);
void UPnPResponse_RenderingControl_SetMute(const UPnPSessionToken UPnPToken);
void UPnPResponse_RenderingControl_SetVolume(const UPnPSessionToken UPnPToken);
void UPnPResponse_RenderingControl_upnpRenderer_GetMediaPlayersList(const UPnPSessionToken UPnPToken, const char* CurrentMediaPlayer, const char* MediaPlayersList);
void UPnPResponse_RenderingControl_upnpRenderer_SetMediaPlayer(const UPnPSessionToken UPnPToken);

/* State Variable Eventing Methods */
void UPnPSetState_AVTransport_LastChange(UPnPMicroStackToken microstack,char* val);
void UPnPSetState_ConnectionManager_SourceProtocolInfo(UPnPMicroStackToken microstack,char* val);
void UPnPSetState_ConnectionManager_SinkProtocolInfo(UPnPMicroStackToken microstack,char* val);
void UPnPSetState_ConnectionManager_CurrentConnectionIDs(UPnPMicroStackToken microstack,char* val);
void UPnPSetState_RenderingControl_LastChange(UPnPMicroStackToken microstack,char* val);


#define UPnP_StateVariable_AllowedValues_MAX 6
struct UPnP_StateVariableTable_AVTransport
{
	char Reserved[1042];
	int ReservedXL;
	int ReservedUXL;
};
struct UPnP_StateVariable_AVTransport_A_ARG_TYPE_SeekMode
{
	int Reserved1;
	int Reserved1L;
	int Reserved2;
	int Reserved2L;
	int Reserved3;
	int Reserved3L;
	char *AllowedValues[UPnP_StateVariable_AllowedValues_MAX];
	int Reserved8;
	int Reserved8L;
};
struct UPnP_StateVariable_AVTransport_NextAVTransportURI
{
	int Reserved1;
	int Reserved1L;
	int Reserved8;
	int Reserved8L;
};
struct UPnP_StateVariable_AVTransport_PlaybackStorageMedium
{
	int Reserved1;
	int Reserved1L;
	int Reserved2;
	int Reserved2L;
	int Reserved3;
	int Reserved3L;
	char *AllowedValues[UPnP_StateVariable_AllowedValues_MAX];
	int Reserved8;
	int Reserved8L;
};
struct UPnP_StateVariable_AVTransport_RecordStorageMedium
{
	int Reserved1;
	int Reserved1L;
	int Reserved2;
	int Reserved2L;
	int Reserved3;
	int Reserved3L;
	char *AllowedValues[UPnP_StateVariable_AllowedValues_MAX];
	int Reserved8;
	int Reserved8L;
};
struct UPnP_StateVariable_AVTransport_A_ARG_TYPE_InstanceID
{
	int Reserved1;
	int Reserved1L;
	int Reserved8;
	int Reserved8L;
};
struct UPnP_StateVariable_AVTransport_CurrentTrack
{
	int Reserved1;
	int Reserved1L;
	int Reserved4;
	int Reserved4L;
	int Reserved5;
	int Reserved5L;
	char *MinMaxStep[3];
	int Reserved8;
	int Reserved8L;
};
struct UPnP_StateVariable_AVTransport_CurrentTrackDuration
{
	int Reserved1;
	int Reserved1L;
	int Reserved8;
	int Reserved8L;
};
struct UPnP_StateVariable_AVTransport_PossiblePlaybackStorageMedia
{
	int Reserved1;
	int Reserved1L;
	int Reserved2;
	int Reserved2L;
	int Reserved3;
	int Reserved3L;
	char *AllowedValues[UPnP_StateVariable_AllowedValues_MAX];
	int Reserved8;
	int Reserved8L;
};
struct UPnP_StateVariable_AVTransport_CurrentPlayMode
{
	int Reserved1;
	int Reserved1L;
	int Reserved2;
	int Reserved2L;
	int Reserved3;
	int Reserved3L;
	char *AllowedValues[UPnP_StateVariable_AllowedValues_MAX];
	int Reserved6;
	int Reserved6L;
	int Reserved7;
	int Reserved7L;
	char *DefaultValue;
	int Reserved8;
	int Reserved8L;
};
struct UPnP_StateVariable_AVTransport_TransportStatus
{
	int Reserved1;
	int Reserved1L;
	int Reserved2;
	int Reserved2L;
	int Reserved3;
	int Reserved3L;
	char *AllowedValues[UPnP_StateVariable_AllowedValues_MAX];
	int Reserved8;
	int Reserved8L;
};
struct UPnP_StateVariable_AVTransport_CurrentRecordQualityMode
{
	int Reserved1;
	int Reserved1L;
	int Reserved2;
	int Reserved2L;
	int Reserved3;
	int Reserved3L;
	char *AllowedValues[UPnP_StateVariable_AllowedValues_MAX];
	int Reserved8;
	int Reserved8L;
};
struct UPnP_StateVariable_AVTransport_TransportState
{
	int Reserved1;
	int Reserved1L;
	int Reserved2;
	int Reserved2L;
	int Reserved3;
	int Reserved3L;
	char *AllowedValues[UPnP_StateVariable_AllowedValues_MAX];
	int Reserved8;
	int Reserved8L;
};
struct UPnP_StateVariable_AVTransport_CurrentTrackURI
{
	int Reserved1;
	int Reserved1L;
	int Reserved8;
	int Reserved8L;
};
struct UPnP_StateVariable_AVTransport_AVTransportURI
{
	int Reserved1;
	int Reserved1L;
	int Reserved8;
	int Reserved8L;
};
struct UPnP_StateVariable_AVTransport_AbsoluteCounterPosition
{
	int Reserved1;
	int Reserved1L;
	int Reserved8;
	int Reserved8L;
};
struct UPnP_StateVariable_AVTransport_PossibleRecordQualityModes
{
	int Reserved1;
	int Reserved1L;
	int Reserved2;
	int Reserved2L;
	int Reserved3;
	int Reserved3L;
	char *AllowedValues[UPnP_StateVariable_AllowedValues_MAX];
	int Reserved8;
	int Reserved8L;
};
struct UPnP_StateVariable_AVTransport_LastChange
{
	int Reserved1;
	int Reserved1L;
	int Reserved8;
	int Reserved8L;
};
struct UPnP_StateVariable_AVTransport_NextAVTransportURIMetaData
{
	int Reserved1;
	int Reserved1L;
	int Reserved8;
	int Reserved8L;
};
struct UPnP_StateVariable_AVTransport_AVTransportURIMetaData
{
	int Reserved1;
	int Reserved1L;
	int Reserved8;
	int Reserved8L;
};
struct UPnP_StateVariable_AVTransport_A_ARG_TYPE_SeekTarget
{
	int Reserved1;
	int Reserved1L;
	int Reserved8;
	int Reserved8L;
};
struct UPnP_StateVariable_AVTransport_AbsoluteTimePosition
{
	int Reserved1;
	int Reserved1L;
	int Reserved8;
	int Reserved8L;
};
struct UPnP_StateVariable_AVTransport_RecordMediumWriteStatus
{
	int Reserved1;
	int Reserved1L;
	int Reserved2;
	int Reserved2L;
	int Reserved3;
	int Reserved3L;
	char *AllowedValues[UPnP_StateVariable_AllowedValues_MAX];
	int Reserved8;
	int Reserved8L;
};
struct UPnP_StateVariable_AVTransport_CurrentTransportActions
{
	int Reserved1;
	int Reserved1L;
	int Reserved8;
	int Reserved8L;
};
struct UPnP_StateVariable_AVTransport_RelativeTimePosition
{
	int Reserved1;
	int Reserved1L;
	int Reserved8;
	int Reserved8L;
};
struct UPnP_StateVariable_AVTransport_CurrentTrackMetaData
{
	int Reserved1;
	int Reserved1L;
	int Reserved8;
	int Reserved8L;
};
struct UPnP_StateVariable_AVTransport_CurrentMediaDuration
{
	int Reserved1;
	int Reserved1L;
	int Reserved8;
	int Reserved8L;
};
struct UPnP_StateVariable_AVTransport_TransportPlaySpeed
{
	int Reserved1;
	int Reserved1L;
	int Reserved2;
	int Reserved2L;
	int Reserved3;
	int Reserved3L;
	char *AllowedValues[UPnP_StateVariable_AllowedValues_MAX];
	int Reserved8;
	int Reserved8L;
};
struct UPnP_StateVariable_AVTransport_RelativeCounterPosition
{
	int Reserved1;
	int Reserved1L;
	int Reserved8;
	int Reserved8L;
};
struct UPnP_StateVariable_AVTransport_NumberOfTracks
{
	int Reserved1;
	int Reserved1L;
	int Reserved4;
	int Reserved4L;
	int Reserved5;
	int Reserved5L;
	char *MinMaxStep[3];
	int Reserved8;
	int Reserved8L;
};
struct UPnP_StateVariable_AVTransport_PossibleRecordStorageMedia
{
	int Reserved1;
	int Reserved1L;
	int Reserved2;
	int Reserved2L;
	int Reserved3;
	int Reserved3L;
	char *AllowedValues[UPnP_StateVariable_AllowedValues_MAX];
	int Reserved8;
	int Reserved8L;
};
struct UPnP_StateVariableTable_ConnectionManager
{
	char Reserved[347];
	int ReservedXL;
	int ReservedUXL;
};
struct UPnP_StateVariable_ConnectionManager_A_ARG_TYPE_ConnectionStatus
{
	int Reserved1;
	int Reserved1L;
	int Reserved2;
	int Reserved2L;
	int Reserved3;
	int Reserved3L;
	char *AllowedValues[UPnP_StateVariable_AllowedValues_MAX];
	int Reserved8;
	int Reserved8L;
};
struct UPnP_StateVariable_ConnectionManager_A_ARG_TYPE_ConnectionID
{
	int Reserved1;
	int Reserved1L;
	int Reserved8;
	int Reserved8L;
};
struct UPnP_StateVariable_ConnectionManager_A_ARG_TYPE_Direction
{
	int Reserved1;
	int Reserved1L;
	int Reserved2;
	int Reserved2L;
	int Reserved3;
	int Reserved3L;
	char *AllowedValues[UPnP_StateVariable_AllowedValues_MAX];
	int Reserved8;
	int Reserved8L;
};
struct UPnP_StateVariable_ConnectionManager_A_ARG_TYPE_RcsID
{
	int Reserved1;
	int Reserved1L;
	int Reserved8;
	int Reserved8L;
};
struct UPnP_StateVariable_ConnectionManager_SourceProtocolInfo
{
	int Reserved1;
	int Reserved1L;
	int Reserved8;
	int Reserved8L;
};
struct UPnP_StateVariable_ConnectionManager_SinkProtocolInfo
{
	int Reserved1;
	int Reserved1L;
	int Reserved8;
	int Reserved8L;
};
struct UPnP_StateVariable_ConnectionManager_A_ARG_TYPE_ProtocolInfo
{
	int Reserved1;
	int Reserved1L;
	int Reserved8;
	int Reserved8L;
};
struct UPnP_StateVariable_ConnectionManager_A_ARG_TYPE_ConnectionManager
{
	int Reserved1;
	int Reserved1L;
	int Reserved8;
	int Reserved8L;
};
struct UPnP_StateVariable_ConnectionManager_A_ARG_TYPE_AVTransportID
{
	int Reserved1;
	int Reserved1L;
	int Reserved8;
	int Reserved8L;
};
struct UPnP_StateVariable_ConnectionManager_CurrentConnectionIDs
{
	int Reserved1;
	int Reserved1L;
	int Reserved8;
	int Reserved8L;
};
struct UPnP_StateVariableTable_RenderingControl
{
	char Reserved[343];
	int ReservedXL;
	int ReservedUXL;
};
struct UPnP_StateVariable_RenderingControl_Mute
{
	int Reserved1;
	int Reserved1L;
	int Reserved8;
	int Reserved8L;
};
struct UPnP_StateVariable_RenderingControl_A_ARG_TYPE_Channel
{
	int Reserved1;
	int Reserved1L;
	int Reserved2;
	int Reserved2L;
	int Reserved3;
	int Reserved3L;
	char *AllowedValues[UPnP_StateVariable_AllowedValues_MAX];
	int Reserved8;
	int Reserved8L;
};
struct UPnP_StateVariable_RenderingControl_A_ARG_TYPE_InstanceID
{
	int Reserved1;
	int Reserved1L;
	int Reserved8;
	int Reserved8L;
};
struct UPnP_StateVariable_RenderingControl_Volume
{
	int Reserved1;
	int Reserved1L;
	int Reserved4;
	int Reserved4L;
	int Reserved5;
	int Reserved5L;
	char *MinMaxStep[3];
	int Reserved8;
	int Reserved8L;
};
struct UPnP_StateVariable_RenderingControl_A_ARG_TYPE_MediaPlayerName
{
	int Reserved1;
	int Reserved1L;
	int Reserved8;
	int Reserved8L;
};
struct UPnP_StateVariable_RenderingControl_A_ARG_TYPE_PresetName
{
	int Reserved1;
	int Reserved1L;
	int Reserved2;
	int Reserved2L;
	int Reserved3;
	int Reserved3L;
	char *AllowedValues[UPnP_StateVariable_AllowedValues_MAX];
	int Reserved8;
	int Reserved8L;
};
struct UPnP_StateVariable_RenderingControl_PresetNameList
{
	int Reserved1;
	int Reserved1L;
	int Reserved2;
	int Reserved2L;
	int Reserved3;
	int Reserved3L;
	char *AllowedValues[UPnP_StateVariable_AllowedValues_MAX];
	int Reserved8;
	int Reserved8L;
};
struct UPnP_StateVariable_RenderingControl_LastChange
{
	int Reserved1;
	int Reserved1L;
	int Reserved8;
	int Reserved8L;
};
struct UPnP_StateVariable_RenderingControl_MediaPlayerName
{
	int Reserved1;
	int Reserved1L;
	int Reserved8;
	int Reserved8L;
};
struct UPnP_ActionTable_AVTransport
{
	char Reserved[1125];
	int ReservedXL;
	int ReservedUXL;
};
struct UPnP_Action_AVTransport_GetCurrentTransportActions
{
	int Reserved;
	int Reserved2;
};
struct UPnP_Action_AVTransport_GetDeviceCapabilities
{
	int Reserved;
	int Reserved2;
};
struct UPnP_Action_AVTransport_GetMediaInfo
{
	int Reserved;
	int Reserved2;
};
struct UPnP_Action_AVTransport_GetPositionInfo
{
	int Reserved;
	int Reserved2;
};
struct UPnP_Action_AVTransport_GetTransportInfo
{
	int Reserved;
	int Reserved2;
};
struct UPnP_Action_AVTransport_GetTransportSettings
{
	int Reserved;
	int Reserved2;
};
struct UPnP_Action_AVTransport_Next
{
	int Reserved;
	int Reserved2;
};
struct UPnP_Action_AVTransport_Pause
{
	int Reserved;
	int Reserved2;
};
struct UPnP_Action_AVTransport_Play
{
	int Reserved;
	int Reserved2;
};
struct UPnP_Action_AVTransport_Previous
{
	int Reserved;
	int Reserved2;
};
struct UPnP_Action_AVTransport_Seek
{
	int Reserved;
	int Reserved2;
};
struct UPnP_Action_AVTransport_SetAVTransportURI
{
	int Reserved;
	int Reserved2;
};
struct UPnP_Action_AVTransport_SetPlayMode
{
	int Reserved;
	int Reserved2;
};
struct UPnP_Action_AVTransport_Stop
{
	int Reserved;
	int Reserved2;
};
struct UPnP_ActionTable_ConnectionManager
{
	char Reserved[342];
	int ReservedXL;
	int ReservedUXL;
};
struct UPnP_Action_ConnectionManager_GetCurrentConnectionIDs
{
	int Reserved;
	int Reserved2;
};
struct UPnP_Action_ConnectionManager_GetCurrentConnectionInfo
{
	int Reserved;
	int Reserved2;
};
struct UPnP_Action_ConnectionManager_GetProtocolInfo
{
	int Reserved;
	int Reserved2;
};
struct UPnP_ActionTable_RenderingControl
{
	char Reserved[476];
	int ReservedXL;
	int ReservedUXL;
};
struct UPnP_Action_RenderingControl_GetMute
{
	int Reserved;
	int Reserved2;
};
struct UPnP_Action_RenderingControl_GetVolume
{
	int Reserved;
	int Reserved2;
};
struct UPnP_Action_RenderingControl_ListPresets
{
	int Reserved;
	int Reserved2;
};
struct UPnP_Action_RenderingControl_SelectPreset
{
	int Reserved;
	int Reserved2;
};
struct UPnP_Action_RenderingControl_SetMute
{
	int Reserved;
	int Reserved2;
};
struct UPnP_Action_RenderingControl_SetVolume
{
	int Reserved;
	int Reserved2;
};
struct UPnP_Action_RenderingControl_upnpRenderer_GetMediaPlayersList
{
	int Reserved;
	int Reserved2;
};
struct UPnP_Action_RenderingControl_upnpRenderer_SetMediaPlayer
{
	int Reserved;
	int Reserved2;
};
struct UPnP_Service_AVTransport
{
	struct UPnP_Action_AVTransport_GetCurrentTransportActions *GetCurrentTransportActions;
	struct UPnP_Action_AVTransport_GetDeviceCapabilities *GetDeviceCapabilities;
	struct UPnP_Action_AVTransport_GetMediaInfo *GetMediaInfo;
	struct UPnP_Action_AVTransport_GetPositionInfo *GetPositionInfo;
	struct UPnP_Action_AVTransport_GetTransportInfo *GetTransportInfo;
	struct UPnP_Action_AVTransport_GetTransportSettings *GetTransportSettings;
	struct UPnP_Action_AVTransport_Next *Next;
	struct UPnP_Action_AVTransport_Pause *Pause;
	struct UPnP_Action_AVTransport_Play *Play;
	struct UPnP_Action_AVTransport_Previous *Previous;
	struct UPnP_Action_AVTransport_Seek *Seek;
	struct UPnP_Action_AVTransport_SetAVTransportURI *SetAVTransportURI;
	struct UPnP_Action_AVTransport_SetPlayMode *SetPlayMode;
	struct UPnP_Action_AVTransport_Stop *Stop;
	
	struct UPnP_StateVariable_AVTransport_A_ARG_TYPE_SeekMode *StateVar_A_ARG_TYPE_SeekMode;
	struct UPnP_StateVariable_AVTransport_NextAVTransportURI *StateVar_NextAVTransportURI;
	struct UPnP_StateVariable_AVTransport_PlaybackStorageMedium *StateVar_PlaybackStorageMedium;
	struct UPnP_StateVariable_AVTransport_RecordStorageMedium *StateVar_RecordStorageMedium;
	struct UPnP_StateVariable_AVTransport_A_ARG_TYPE_InstanceID *StateVar_A_ARG_TYPE_InstanceID;
	struct UPnP_StateVariable_AVTransport_CurrentTrack *StateVar_CurrentTrack;
	struct UPnP_StateVariable_AVTransport_CurrentTrackDuration *StateVar_CurrentTrackDuration;
	struct UPnP_StateVariable_AVTransport_PossiblePlaybackStorageMedia *StateVar_PossiblePlaybackStorageMedia;
	struct UPnP_StateVariable_AVTransport_CurrentPlayMode *StateVar_CurrentPlayMode;
	struct UPnP_StateVariable_AVTransport_TransportStatus *StateVar_TransportStatus;
	struct UPnP_StateVariable_AVTransport_CurrentRecordQualityMode *StateVar_CurrentRecordQualityMode;
	struct UPnP_StateVariable_AVTransport_TransportState *StateVar_TransportState;
	struct UPnP_StateVariable_AVTransport_CurrentTrackURI *StateVar_CurrentTrackURI;
	struct UPnP_StateVariable_AVTransport_AVTransportURI *StateVar_AVTransportURI;
	struct UPnP_StateVariable_AVTransport_AbsoluteCounterPosition *StateVar_AbsoluteCounterPosition;
	struct UPnP_StateVariable_AVTransport_PossibleRecordQualityModes *StateVar_PossibleRecordQualityModes;
	struct UPnP_StateVariable_AVTransport_LastChange *StateVar_LastChange;
	struct UPnP_StateVariable_AVTransport_NextAVTransportURIMetaData *StateVar_NextAVTransportURIMetaData;
	struct UPnP_StateVariable_AVTransport_AVTransportURIMetaData *StateVar_AVTransportURIMetaData;
	struct UPnP_StateVariable_AVTransport_A_ARG_TYPE_SeekTarget *StateVar_A_ARG_TYPE_SeekTarget;
	struct UPnP_StateVariable_AVTransport_AbsoluteTimePosition *StateVar_AbsoluteTimePosition;
	struct UPnP_StateVariable_AVTransport_RecordMediumWriteStatus *StateVar_RecordMediumWriteStatus;
	struct UPnP_StateVariable_AVTransport_CurrentTransportActions *StateVar_CurrentTransportActions;
	struct UPnP_StateVariable_AVTransport_RelativeTimePosition *StateVar_RelativeTimePosition;
	struct UPnP_StateVariable_AVTransport_CurrentTrackMetaData *StateVar_CurrentTrackMetaData;
	struct UPnP_StateVariable_AVTransport_CurrentMediaDuration *StateVar_CurrentMediaDuration;
	struct UPnP_StateVariable_AVTransport_TransportPlaySpeed *StateVar_TransportPlaySpeed;
	struct UPnP_StateVariable_AVTransport_RelativeCounterPosition *StateVar_RelativeCounterPosition;
	struct UPnP_StateVariable_AVTransport_NumberOfTracks *StateVar_NumberOfTracks;
	struct UPnP_StateVariable_AVTransport_PossibleRecordStorageMedia *StateVar_PossibleRecordStorageMedia;
	
	char Reserved[153];
	int ReservedXL;
	int ReservedUXL;
};
struct UPnP_Service_ConnectionManager
{
	struct UPnP_Action_ConnectionManager_GetCurrentConnectionIDs *GetCurrentConnectionIDs;
	struct UPnP_Action_ConnectionManager_GetCurrentConnectionInfo *GetCurrentConnectionInfo;
	struct UPnP_Action_ConnectionManager_GetProtocolInfo *GetProtocolInfo;
	
	struct UPnP_StateVariable_ConnectionManager_A_ARG_TYPE_ConnectionStatus *StateVar_A_ARG_TYPE_ConnectionStatus;
	struct UPnP_StateVariable_ConnectionManager_A_ARG_TYPE_ConnectionID *StateVar_A_ARG_TYPE_ConnectionID;
	struct UPnP_StateVariable_ConnectionManager_A_ARG_TYPE_Direction *StateVar_A_ARG_TYPE_Direction;
	struct UPnP_StateVariable_ConnectionManager_A_ARG_TYPE_RcsID *StateVar_A_ARG_TYPE_RcsID;
	struct UPnP_StateVariable_ConnectionManager_SourceProtocolInfo *StateVar_SourceProtocolInfo;
	struct UPnP_StateVariable_ConnectionManager_SinkProtocolInfo *StateVar_SinkProtocolInfo;
	struct UPnP_StateVariable_ConnectionManager_A_ARG_TYPE_ProtocolInfo *StateVar_A_ARG_TYPE_ProtocolInfo;
	struct UPnP_StateVariable_ConnectionManager_A_ARG_TYPE_ConnectionManager *StateVar_A_ARG_TYPE_ConnectionManager;
	struct UPnP_StateVariable_ConnectionManager_A_ARG_TYPE_AVTransportID *StateVar_A_ARG_TYPE_AVTransportID;
	struct UPnP_StateVariable_ConnectionManager_CurrentConnectionIDs *StateVar_CurrentConnectionIDs;
	
	char Reserved[159];
	int ReservedXL;
	int ReservedUXL;
};
struct UPnP_Service_RenderingControl
{
	struct UPnP_Action_RenderingControl_GetMute *GetMute;
	struct UPnP_Action_RenderingControl_GetVolume *GetVolume;
	struct UPnP_Action_RenderingControl_ListPresets *ListPresets;
	struct UPnP_Action_RenderingControl_SelectPreset *SelectPreset;
	struct UPnP_Action_RenderingControl_SetMute *SetMute;
	struct UPnP_Action_RenderingControl_SetVolume *SetVolume;
	struct UPnP_Action_RenderingControl_upnpRenderer_GetMediaPlayersList *upnpRenderer_GetMediaPlayersList;
	struct UPnP_Action_RenderingControl_upnpRenderer_SetMediaPlayer *upnpRenderer_SetMediaPlayer;
	
	struct UPnP_StateVariable_RenderingControl_Mute *StateVar_Mute;
	struct UPnP_StateVariable_RenderingControl_A_ARG_TYPE_Channel *StateVar_A_ARG_TYPE_Channel;
	struct UPnP_StateVariable_RenderingControl_A_ARG_TYPE_InstanceID *StateVar_A_ARG_TYPE_InstanceID;
	struct UPnP_StateVariable_RenderingControl_Volume *StateVar_Volume;
	struct UPnP_StateVariable_RenderingControl_A_ARG_TYPE_MediaPlayerName *StateVar_A_ARG_TYPE_MediaPlayerName;
	struct UPnP_StateVariable_RenderingControl_A_ARG_TYPE_PresetName *StateVar_A_ARG_TYPE_PresetName;
	struct UPnP_StateVariable_RenderingControl_PresetNameList *StateVar_PresetNameList;
	struct UPnP_StateVariable_RenderingControl_LastChange *StateVar_LastChange;
	struct UPnP_StateVariable_RenderingControl_MediaPlayerName *StateVar_MediaPlayerName;
	
	char Reserved[154];
	int ReservedXL;
	int ReservedUXL;
};
struct UPnP_Device_MediaRenderer
{
	struct UPnP_Service_AVTransport *AVTransport;
	struct UPnP_Service_ConnectionManager *ConnectionManager;
	struct UPnP_Service_RenderingControl *RenderingControl;
	
	const char *FriendlyName;
	const char *UDN;
	const char *Serial;
	const char *Manufacturer;
	const char *ManufacturerURL;
	const char *ModelDescription;
	const char *ModelName;
	const char *ModelNumber;
	const char *ModelURL;
	const char *ProductCode;
	char Reserved[534];
	int ReservedXL;
	int ReservedUXL;
	void *User;
	void *MicrostackToken;
};

struct UPnP_Device_MediaRenderer* UPnPGetConfiguration();



/*! \} */
#endif
