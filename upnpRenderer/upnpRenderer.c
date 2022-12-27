/*
	upnpRenderer v0.1

	(c) 2021 (: JOBnik! :) Arthur Aminov, Israel
	https://www.jobnik.net

	MicroStack and MicroMediaRenderer
	(c) 2006 - 2011 Intel Corporation

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

#include "../upnpRenderer.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#ifdef _WINSOCK2
	#include <winsock2.h>
#endif
#include <Rpc.h>
#pragma comment(lib, "rpcrt4.lib")  // UuidCreate - Minimum supported OS Win 2000

#include "upnpRenderer.h"
#include "ILibParsers.h"
#include "MicroMediaRenderer.h"
#include "UPnPMicroStack.h"
#include "functions.h"
#include "menus.h"
#include "MediaPlayerControl.h"

#ifdef _VERBOSE
	#define VERBOSEONLY(x) x
#endif

#ifndef _VERBOSE
	#define VERBOSEONLY(x) 
#endif

void *MR_MediaRenderer;

void *MicroStackChain;
void *UPnPmicroStack;

extern struct UPnP_Device_MediaRenderer UPnP_Device_MediaRenderer_Impl;

HANDLE ILib_IPAddressMonitorTerminator;
HANDLE ILib_IPAddressMonitorThread;
DWORD ILib_MonitorSocketReserved;
WSAOVERLAPPED ILib_MonitorSocketStateObject;
SOCKET ILib_MonitorSocket;

HWND hWin = NULL;
HINSTANCE hInst;

HICON hIcon = NULL;
HMENU hMenu = NULL, hMenuControl = NULL;
HANDLE threadUpnp = NULL;

int WebServerPort = 3500;
char friendlyName[100] = {0};
TCHAR lastMediaPlayer[100] = {0};
BOOL isUpnpStarted = FALSE;
BOOL isAnyMediaPlayerExists = FALSE;

extern struct MP *mp;

typedef void (CALLBACK UPNPSTARTEDPROC)();

// Handle volume change
void MROnVolumeChangeRequestSink(enum MR_Enum_AudioChannels Channel, unsigned short Value)
{
    VERBOSEONLY(printft("MROnVolumeChangeRequestSink\r\n");)

	if (mpGetHandle() && Channel == MR_AudioChannel_Master)
	{
		if (Value != 0xffff) {	// not (-1) get volume
			mpSetVolume(Value);
		}

        Value = mpGetVolume();

		MRSetVolume(Channel, Value);	// make sure it's real mp's volume
	
		printft("Volume %d%%\r\n", Value);
	}
}

// Handle mute change
void MROnMuteChangeRequestSink(enum MR_Enum_AudioChannels Channel, int Value)
{
    VERBOSEONLY(printft("MROnMuteChangeRequestSink\r\n");)

	if (mpGetHandle() && Channel == MR_AudioChannel_Master)
	{
		mpToggleMute();

		MRSetMute(Channel, Value);

		printft("Mute %d\r\n", Value);
	}
}

// Handle media URI change
void MROnMediaChangeRequestSink(const char* MediaUri, char *CurrentURIMetaData)
{
    VERBOSEONLY(printft("MROnMediaChangeRequestSink\r\n");)

	wchar_t wtemp[WBUFFER_SIZE] = {0};
	char *mu = replaceWord(MediaUri, "amp;", "");
	char *cstr = getBetween(MediaUri, "upnpRendererMP=", ""); // must be last query param

	MRSetMediaUri(mu);
	MRSetTrack(mu, 1);
	MRSetTrackDurationInfo(0, 1);
	MRSetMediaTotals(1, 0, 1);
	MRSetMetaData(CurrentURIMetaData);  // added by jobnik.net, MetaData as received from the Client

    printft("Media Uri: %s\r\n\r\nMetaData: %s\r\n", mu, CurrentURIMetaData);

	free(mu);

    // Set Media Player from "upnpRendererMP=?" query string param
	if (cstr) {
		CheckControlMenu(cstr, hWin, hMenuControl);
		free(cstr);
	}

	char2wchar(MediaUri, wtemp);

	mpOpen(L"", wtemp);

	printft("Media Player hWnd: %#010x\r\n", mp->hMP);

	MRSetVolume(MR_AudioChannel_Master, mpGetVolume());
}

// Handle position state request
void MROnGetPositionRequestSink(int* seconds, int* absSeconds, int* count, int* absCount, HWND *playerHandle)
{
    char buf[25] = {0};
    int dur = 0;

	VERBOSEONLY(printft("MROnGetPositionRequestSink\r\n");)

	*playerHandle = mpGetHandle();

	// playback position within entire media/playlist in seconds
	*absSeconds = MR_DURATION_UNKNOWN;

	// Playback counter position
	*count = MR_COUNTER_UNKNOWN;

	// Playback counter position - relative to within the entire playlist/media.
	*absCount = MR_COUNTER_UNKNOWN;

	if (!playerHandle) {
		// playback position in seconds
		*seconds = MR_DURATION_UNKNOWN;
		MRSetTrack("", 0);
		MRSetTrackDurationInfo(0, 0);
	} else {
		*seconds = mpGetPosition();
		dur = mpGetDuration();

	    MRSetTrackDurationInfo(dur, 1);

        SetTimeString(buf, *seconds);
        strcat(buf, " / ");

		if (dur < 0) dur = 0;

		SetTimeString(strlen(buf) + buf, dur);

        printft("Position / Duration: %s\r\n", buf);
	}

	/*if (MRGetState() == MR_State_Playing && seconds > 0) {
		if (mp->hp.GET_FILENAME[0] != '\0') {
			// in case file/stream changed in player and not using upnp
    	    MRSetWinTitleUri(mpGetHandle(), mpGetFilename((LPARAM)hWin));
		} else {
			mpGetFilename((LPARAM)hWin);
		}
	}*/

	if (MRGetState() == MR_State_Playing) {
	    //MRSetWinTitle();
	    mpGetFilename((LPARAM)hWin);
	}
}

// Handle seek command
void MROnSeekRequestSink(enum MR_Enum_SeekModes seekMode, int seekPosition)
{
    VERBOSEONLY(printft("MROnSeekRequestSink\r\n");)

	if (mpGetHandle())
	{
		char buf[10] = {0};

		mpSetPosition(seekPosition);

        SetTimeString(buf, seekPosition);
        printft("Seek position: %s\r\n", buf);
	}
}

// Handle next/previous command
void MROnNextPreviousRequestSink(int trackDelta)
{
	if (mpGetHandle()) {
		if (trackDelta == -1) mpPrevious();
		else mpNext();

        printft("%s\r\n", trackDelta == -1 ? "Previous" : "Next");
	}
}

// Handle play/stop/pause/etc request
void MROnStateChangeRequestSink(enum MR_Enum_States state, enum MR_Enum_PlaySpeeds playSpeeds, char *Media_URI)
{
    VERBOSEONLY(printft("MROnStateChangeRequestSink\r\n");)

	if (state == MR_State_Playing) {
		mpPlay();
        printft("PLAY\r\n");
	} else if (state == MR_State_Paused) {
		mpPause();
        printft("PAUSE\r\n");
	} else if (state == MR_State_Stopped) {
		mpStop();
        printft("STOP\r\n");
	}
}

// Handle play mode change
void MROnPlayModeChangeRequestSink(enum MR_Enum_PlayModes playmode)
{
	/* TODO: Handle play mode change */
}

// Get Transport State
void MROnGetTransportRequestSink()
{
    VERBOSEONLY(printft("MROnGetTransportRequestSink\r\n");)

    int state = 0;

	if (mpGetHandle()) {
	    MRSetStatus(MR_Status_OK);
	    state = mpGetState();

	    if (state == mp->STATE_PLAY) MRSetState(MR_State_Playing);
	    else if (state == mp->STATE_PAUSE) MRSetState(MR_State_Paused);
		else if (state == mp->STATE_STOP) MRSetState(MR_State_Stopped);
		else MRSetState(MR_State_NoMedia);

	} else {
		MRSetState(MR_State_NoMedia);
		MRSetStatus(MR_Status_ERROR);
	}
}

// Handle media info request
void MROnGetMediaRequestSink(HWND *playerHandle)
{
	VERBOSEONLY(printft("MROnGetMediaRequestSink\r\n");)

	*playerHandle = mpGetHandle();

	if (!playerHandle) {
	    MRSetMediaUri("");
		MRSetMediaTotals(0, 0, 0);
	} else {
        MRSetMediaTotals(1, mpGetDuration(), 1);
	}

	/*if (MRGetState() == MR_State_Playing) {
		if (mp->hp.GET_FILENAME[0] != '\0') {
			// in case file/stream changed in player and not using upnp
    		MRSetWinTitleUri(mpGetHandle(), mpGetFilename((LPARAM)hWin));
		} else {
			mpGetFilename((LPARAM)hWin);
		}
	}*/

	/*if (MRGetState() == MR_State_Playing) {
	    MRSetWinTitle();
	    mpGetFilename((LPARAM)hWin);
	}*/
}

void MROnGetMediaPlayersListSink(char* currentMediaPlayer, char* mediaPlayersList)
{
	VERBOSEONLY(printft("MROnGetMediaPlayersListSink\r\n");)

	GetMediaPlayersList(currentMediaPlayer, mediaPlayersList, hMenuControl);
}

void MROnSetMediaPlayerSink(const char* mediaPlayer)
{
	VERBOSEONLY(printft("MROnSetMediaPlayerSink\r\n");)

	CheckControlMenu(mediaPlayer, hWin, hMenuControl);
}

BOOL MRSetWinTitle()
{
	char winTitle[BUFFER_SIZE] = {0};
	char *wt1 = NULL;
	char *wt2 = NULL;
	char *mu = NULL;
	BOOL r = FALSE;
	char title[BUFFER_SIZE] = {0};

	wchar2char(getWindowTitle(mpGetHandle()), winTitle);
	wt1 = strtok(winTitle, " ");
	//wt1 = replaceWord(winTitle, " - PotPlayer", "");
	wchar2char(mp->NAME, title);
	wt2 = replaceWord(wt1, title, "");

	if (!strstr(MRGetMediaUri(), wt2) && strcmp(wt2, "") != 0) {
		MRSetMetaData(NULL);
		MRSetMediaInfo(MR_PROTINFO_HTTP_UNKNOWN, wt2, "");
		MRSetTrackMetadata(MR_PROTINFO_HTTP_UNKNOWN, wt2, "");

		mu = replaceWord(wt2, "amp;", "");
		MRSetMediaUri(mu);
		MRSetTrack(mu, 1);
		MRSetTrackDurationInfo(0, 1);
		MRSetMediaTotals(1, 0, 1);
		free(mu);
		r = TRUE;
	}

	//free(wt1);
	free(wt2);

	return r;
}

// in case played file/stream not from upnp
void MRSetWinTitleUri(HWND playerHandle, wchar_t *mediaUri)
{
	wchar_t *winTitle;
	char cbuf[BUFFER_SIZE] = {0};
	char title[BUFFER_SIZE] = {0};
	char *mu;

    wchar2char(mediaUri, cbuf);
	mu = replaceWord(cbuf, "amp;", "");

	winTitle = getWindowTitle(playerHandle);
	wchar2char(winTitle, title);

	if (strcmp(MRGetMediaUri(), mu))// && title[0] != '\0')
	{
		MRSetMetaData(NULL);
		MRSetMediaInfo(MR_PROTINFO_HTTP_UNKNOWN, title, "");
		MRSetTrackMetadata(MR_PROTINFO_HTTP_UNKNOWN, title, "");

		MRSetMediaUri(mu);
		MRSetTrack(mu, 1);
		MRSetTrackDurationInfo(0, 1);
		MRSetMediaTotals(1, 0, 1);
	}

	if (winTitle) free(winTitle);
	if (mu) free(mu);
}

void CALLBACK ILib_IPAddressMonitor(
IN DWORD dwError, 
IN DWORD cbTransferred, 
IN LPWSAOVERLAPPED lpOverlapped, 
IN DWORD dwFlags 
)
{
	UNREFERENCED_PARAMETER( dwError );
	UNREFERENCED_PARAMETER( cbTransferred );
	UNREFERENCED_PARAMETER( lpOverlapped );
	UNREFERENCED_PARAMETER( dwFlags );

	UPnPIPAddressListChanged(UPnPmicroStack);
	WSAIoctl(ILib_MonitorSocket, SIO_ADDRESS_LIST_CHANGE, NULL, 0, NULL, 0, &ILib_MonitorSocketReserved, &ILib_MonitorSocketStateObject, &ILib_IPAddressMonitor);
}

DWORD WINAPI ILib_IPAddressMonitorLoop(LPVOID args)
{
	UNREFERENCED_PARAMETER( args );

	ILib_MonitorSocket = socket(AF_INET,SOCK_DGRAM,0);
	WSAIoctl(ILib_MonitorSocket, SIO_ADDRESS_LIST_CHANGE, NULL, 0, NULL, 0, &ILib_MonitorSocketReserved, &ILib_MonitorSocketStateObject, &ILib_IPAddressMonitor);
	while(WaitForSingleObjectEx(ILib_IPAddressMonitorTerminator, INFINITE, TRUE) != WAIT_OBJECT_0);
	return 0;
}

void upnpCreate(UPNPSTARTEDPROC *proc)
{
	char *udn;
	char hostname[100];
	WSADATA wsaData;
	UUID uuid = {0};
	OSVERSIONINFOEX osvi;
	int *ips = NULL;
	int i = 0, num_ips = 0;
	DWORD ptid2 = 0;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
	    printft("WinSock couldn't be Started\r\n");
        return;
	}

    if (friendlyName[0] == '\0') {
    	// generate a friendly name that has the host name in it
    	gethostname(hostname, sizeof(hostname));
    
        // get Windows version
    	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
    	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
    	RtlGetVersion(&osvi);
    
    	sprintf(friendlyName, "%s (Windows %d)", hostname, osvi.dwBuildNumber >= 22000 ? 11 : osvi.dwMajorVersion);	// fix for Windows 11
    }

	printft("------------------------------------------------------\r\n");
	printft("Friendly Name: %s\r\n", friendlyName);

	UuidCreate(&uuid);
	UuidToStringA(&uuid, (RPC_CSTR*)&udn);

	printft("UDN: %s\r\n", udn);

    // get local IPs
	num_ips = ILibGetLocalIPAddressList(&ips);

	for (; i < num_ips; i++) {
		printft("IP: %d.%d.%d.%d:%d\r\n", (ips[i]&0xFF), ((ips[i]>>8)&0xFF), ((ips[i]>>16)&0xFF), ((ips[i]>>24)&0xFF), WebServerPort);
	}

	free(ips);

	printft("Server: WINDOWS, UPnP/1.0, MicroStack/1.0.5329\r\n");
	printft("------------------------------------------------------\r\n");

	/*
		*	Set all of the renderer callbacks.
		*	If the UPnP device has multiple renderers, it will need
		*	to map function pointer callbacks for each renderer device.
		*/
	MROnVolumeChangeRequest			= &MROnVolumeChangeRequestSink;
	MROnMuteChangeRequest			= &MROnMuteChangeRequestSink;
	MROnMediaChangeRequest			= &MROnMediaChangeRequestSink;
	MROnGetPositionRequest			= &MROnGetPositionRequestSink;
	MROnSeekRequest					= &MROnSeekRequestSink;
	MROnNextPreviousRequest			= &MROnNextPreviousRequestSink;
	MROnStateChangeRequest			= &MROnStateChangeRequestSink;
	MROnPlayModeChangeRequest		= &MROnPlayModeChangeRequestSink;
	MROnGetTransportRequest			= &MROnGetTransportRequestSink;
	MROnGetMediaRequest				= &MROnGetMediaRequestSink;
	MROnGetMediaPlayersList			= &MROnGetMediaPlayersListSink;
	MROnSetMediaPlayer				= &MROnSetMediaPlayerSink;

	// Each device must have a unique device identifier (UDN) - The UDN should be generated dynamically
	MicroStackChain = ILibCreateChain();

	UPnP_Device_MediaRenderer_Impl.Manufacturer = "(: JOBnik! :)";
	UPnP_Device_MediaRenderer_Impl.ManufacturerURL = "https://www.jobnik.net";
	UPnP_Device_MediaRenderer_Impl.ModelDescription = "UPnP Media Renderer for Media Players";
	UPnP_Device_MediaRenderer_Impl.ModelName = "jobnik.net Media Renderer";
	UPnP_Device_MediaRenderer_Impl.ModelNumber = "";
	UPnP_Device_MediaRenderer_Impl.ModelURL = "https://www.jobnik.net";
	UPnP_Device_MediaRenderer_Impl.ProductCode = "";

	UPnPmicroStack = UPnPCreateMicroStack(MicroStackChain, friendlyName, udn, "", 1800, WebServerPort);
	MR_MediaRenderer = CreateMediaRenderer(MicroStackChain, UPnPmicroStack, ILibCreateLifeTime(MicroStackChain));

	/*
		*	Initialize the streaming engine to empty or last known stream - 
		*	do this after streaming framework is set up.
		*/
	MRSetMediaInfo(MR_PROTINFO_HTTP_UNKNOWN, "", "");
	MRSetTrackMetadata(MR_PROTINFO_HTTP_UNKNOWN, "", "");

	// tell UPnP network that this device has no active streams
	MRSetMediaTotals(0, 0, 0);
	MRSetTrackDurationInfo(0, 0);

	ILib_IPAddressMonitorTerminator = CreateEvent(NULL, TRUE, FALSE, NULL);
	ILib_IPAddressMonitorThread = CreateThread(NULL, 0, &ILib_IPAddressMonitorLoop, NULL, 0, &ptid2);

	if (proc) {
		proc();	// upnpStartedCallback
	}

	// Start the renderer thread chain
    ILibStartChain(MicroStackChain);

	RpcStringFreeA((RPC_CSTR*)&udn);

	// clean up wsastartup
	WSACleanup();
}

void upnpStartedCallback()
{
	if (isAnyMediaPlayerExists) wprintft(TEXT("Controlling Media Player: %s\r\n"), lastMediaPlayer);
	else wprintft(TEXT("Couldn't find any Media Player to Control\r\n"));

	isUpnpStarted = TRUE;
}

DWORD WINAPI threadUpnpCreate(LPVOID args)
{
    upnpCreate((UPNPSTARTEDPROC*)args);
	return 0;
}

void upnpStart()
{
    iniLoadConf();
	threadUpnp = CreateThread(NULL, 0, &threadUpnpCreate, (UPNPSTARTEDPROC*)upnpStartedCallback, 0, NULL);
}

void upnpStop()
{
	closesocket(ILib_MonitorSocket);
	SetEvent(ILib_IPAddressMonitorTerminator);
	WaitForSingleObject(ILib_IPAddressMonitorThread, INFINITE);
	CloseHandle(ILib_IPAddressMonitorTerminator);
	ILibStopChain(MicroStackChain);

	WaitForSingleObject(threadUpnp, INFINITE);
	CloseHandle(threadUpnp);
	threadUpnp = NULL;

	printft("\r\nUPnP Stopped\r\n");
}

// Load configuration
BOOL iniLoadConf()
{
	TCHAR cCurrentPath[BUFFER_SIZE] = {0};
	TCHAR section[20] = {0};
	TCHAR tFriendlyName[100] = {0};

	GetCurrentDirectory(sizeof(cCurrentPath), cCurrentPath);

#ifdef _DEBUG
	_tcscat(cCurrentPath, TEXT("..\\..\\Debug\\upnpRenderer.ini"));
#else
	_tcscat(cCurrentPath, TEXT("\\upnpRenderer.ini"));
#endif

	_stprintf(section, TEXT("upnpRenderer"));

    WebServerPort = GetPrivateProfileInt(section, TEXT("port"), 0, cCurrentPath);
    
	GetPrivateProfileString(section, TEXT("friendlyName"), TEXT(upnpVersionString), tFriendlyName, sizeof(tFriendlyName) / sizeof(tFriendlyName[0]), cCurrentPath);
	GetPrivateProfileString(section, TEXT("lastMediaPlayer"), TEXT("PotPlayer"), lastMediaPlayer, sizeof(lastMediaPlayer) / sizeof(lastMediaPlayer[0]), cCurrentPath);

	wchar2char(tFriendlyName, friendlyName);

	return GetLastError() ? FALSE : TRUE;
}

LRESULT wmCopyData(HWND h, UINT m, WPARAM w, LPARAM l)
{
	PCOPYDATASTRUCT pcp;
	int iData = 0;
	wchar_t *swData;

	pcp = (PCOPYDATASTRUCT)l;
	swData = str2wstr(pcp->lpData);

	iData = (int)_wtoi(swData);

	if (pcp->dwData == mp->cd.GET_VOLUME) mp->pos.volume = iData;
	if (pcp->dwData == mp->cd.GET_STATE) mp->pos.state = iData;
    if (pcp->dwData == mp->cd.GET_POSITION || pcp->dwData == mp->cd.GET_POSITION_RESPONSE) mp->pos.position = iData / (mp->IS_MS ? 1000 : 1);

    if (pcp->dwData == mp->cd.GET_DURATION)
    {
	    if (_tcsstr(mp->NAME, TEXT("MPC-"))) {
		    iData = (int)_wtoi(wcsrev(wcstok(wcsrev(swData), L"|")));    // duration
		    _swprintf(mp->pos.filename, L"%s", wcsrev(wcstok(NULL, L"|")));    // filename
		}

        mp->pos.duration = iData / (mp->IS_MS ? 1000 : 1);
	}

    if (pcp->dwData == mp->cd.GET_FILENAME || pcp->dwData == mp->cd.GET_FILENAME_RESPONSE)
    {
        if (MRGetState() == MR_State_Playing) {
    	    if (!_tcsstr(mp->NAME, TEXT("MPC-"))) { // don't update for MPC as it returns only filename without the path
    		    _swprintf(mp->pos.filename, L"%s", swData);    // filename

                // in case file/stream changed in player and not using upnp
    	        MRSetWinTitleUri(mpGetHandle(), mp->pos.filename);
    	    }
        }

        wprintft(L"%s filename: %s\r\n", mp->NAME, mp->pos.filename);
	}

	if (swData != (wchar_t*)pcp->lpData) {
		free(swData);
	}

	return 1;
}

LRESULT wmTrayMessage(HWND h, UINT m, WPARAM w, LPARAM l)
{
	switch(l) {
		case WM_LBUTTONDBLCLK:
			ShowWindow(h, SW_SHOWNORMAL);
			SetForegroundWindow(h);
			PostMessage(DLGITEM(ID_PRINT_TEXT), EM_SETSEL, 0, 0);	// remove selection in Edit
			//notifyIconDelete(h);
		break;
		case WM_RBUTTONUP:
		    SetForegroundWindow(h);
			notifyIconPopupMenu(h, hMenu);
		break;
		default:
			return DefWindowProc(h, m, w, l);
	}

	return 1;
}

BOOL CALLBACK dialogproc(HWND h, UINT m, WPARAM w, LPARAM l)
{
	switch (m) {
		case WM_COMMAND: return wmCommandMenu(h, m, w, l, friendlyName, hMenu, hMenuControl, threadUpnp); break;
		case WM_COPYDATA: return wmCopyData(h, m, w, l); break;
		case WM_TRAYMESSAGE: return wmTrayMessage(h, m, w, l); break;
		case WM_INITMENUPOPUP: return wmCommandMenu(h, m, w, l, friendlyName, hMenu, hMenuControl, threadUpnp); break;
		case WM_SIZE:
			if (w == SIZE_MINIMIZED) {
				//notifyIconAdd(h, hIcon, hInst);
				ShowWindow(h, SW_HIDE);
			} else {
				MoveWindow(DLGITEM(ID_PRINT_TEXT), 0, 0, LOWORD(l), HIWORD(l), 1);
			}
			return 1;
		break;
		case WM_CLOSE:
			if (threadUpnp) upnpStop();

			mpFree();

			notifyIconDelete(h);

			EndDialog(h, 0);

			DestroyIcon(hIcon);

        	DestroyMenu(hMenu);

			return 1;
		break;
		case WM_INITDIALOG:
			hWin = h;
			hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON));

		    SendMessage(h, WM_SETICON, ICON_BIG, (LPARAM)hIcon);

			upnpStart();

    		hMenu = (HMENU)GetSubMenu(LoadMenu(hInst, MAKEINTRESOURCE(IDR_TRAYMENU)), 0);
    
    		//SetMenuDefaultItem(hMenu, IDR_TRAYMENU, FALSE);

			if (!iniToControlMenu(h, hMenu, lastMediaPlayer, &hMenuControl)) {
				MessageBox(h, TEXT("Couldn't find any configuration .ini files.\r\nNo Media Player to Control."), TEXT("Warning"), MB_OK | MB_ICONWARNING);
			}

			// set edit box limit size
			MESS(ID_PRINT_TEXT, EM_SETLIMITTEXT, 0, 0);

			notifyIconAdd(h, hIcon, hInst);

			return 1;
		break;
	}

	return 0;
}

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	hInst = hInstance;

	DialogBox(hInst, (TCHAR*)ID_DIALOG_MAIN, 0, &dialogproc);

	return 0;
}
