/*
	Control your "Media Player" using UPnP

	(c) 2021 (: JOBnik! :) Arthur Aminov, Israel
	https://www.jobnik.net

	All data and commands included in configuration .Ini files:
		PotPlayer.ini
		MPC-BE.ini
		MPC-BE-HTTP.ini
		MPC-HC.ini
		MPC-HC-HTTP.ini
		VLC.ini
		VLC-HTTP.ini

	Added only commands this UPnP Renderer can handle.

	You can add your own configurations for your Media Player.
*/

#include "../upnpRenderer.h"
#include <WinInet.h>
#include "MediaPlayerControl.h"
#include "../functions.h"

#pragma comment(lib,"Wininet.lib")

struct MP *mp = NULL;
extern HWND hWin;

// Create MP object
void *mpCreate()
{
	mp = (struct MP*)malloc(sizeof(struct MP));
	memset(mp, 0, sizeof(struct MP));

    return mp;
}

// Load Data from INI File
BOOL mpLoadDataINI(const TCHAR *ini)
{
	TCHAR cCurrentPath[BUFFER_SIZE] = {0};
	TCHAR section[20] = {0};

	GetCurrentDirectory(sizeof(cCurrentPath), cCurrentPath);

#ifdef _DEBUG
	_stprintf(cCurrentPath, TEXT("..\\Debug\\%s.ini"), ini);
#else
	_stprintf(cCurrentPath + _tcslen(cCurrentPath), TEXT("\\%s.ini"), ini);
#endif

	_stprintf(section, TEXT("MediaPlayer"));

	GetPrivateProfileString(section, TEXT("MP_NAME"), TEXT(""), mp->NAME, sizeof(mp->NAME) / sizeof(mp->NAME[0]), cCurrentPath);
	GetPrivateProfileString(section, TEXT("MP_TITLE"), TEXT(""), mp->TITLE, sizeof(mp->TITLE) / sizeof(mp->TITLE[0]), cCurrentPath);
	GetPrivateProfileString(section, TEXT("MP_PATH"), TEXT(""), mp->PATH, sizeof(mp->PATH) / sizeof(mp->PATH[0]), cCurrentPath);
	GetPrivateProfileString(section, TEXT("MP_CLASS"), TEXT(""), mp->CLASS, sizeof(mp->CLASS) / sizeof(mp->CLASS[0]), cCurrentPath);
	GetPrivateProfileString(section, TEXT("MP_CAPTION"), TEXT(""), mp->CAPTION, sizeof(mp->CAPTION) / sizeof(mp->CAPTION[0]), cCurrentPath);

	GetPrivateProfileString(section, TEXT("MP_HOST"), TEXT(""), mp->HOST, sizeof(mp->HOST) / sizeof(mp->HOST[0]), cCurrentPath);
	mp->PORT = GetPrivateProfileInt(section, TEXT("MP_PORT"), 0, cCurrentPath);
	GetPrivateProfileString(section, TEXT("MP_USER"), TEXT(""), mp->USER, sizeof(mp->USER) / sizeof(mp->USER[0]), cCurrentPath);
	GetPrivateProfileString(section, TEXT("MP_PASS"), TEXT(""), mp->PASS, sizeof(mp->PASS) / sizeof(mp->PASS[0]), cCurrentPath);

    _stprintf(section, TEXT("MediaPlayerCommands"));

    mp->IS_MS = GetPrivateProfileInt(section, TEXT("MP_IS_MS"), 0, cCurrentPath);

	mp->STATE_STOP = GetPrivateProfileInt(section, TEXT("MP_STATE_STOP"), 0, cCurrentPath);
	mp->STATE_PAUSE = GetPrivateProfileInt(section, TEXT("MP_STATE_PAUSE"), 0, cCurrentPath);
	mp->STATE_PLAY = GetPrivateProfileInt(section, TEXT("MP_STATE_PLAY"), 0, cCurrentPath);

	mp->cm.GET_VOLUME = GetPrivateProfileInt(section, TEXT("MP_CM_GET_VOLUME"), 0, cCurrentPath);
	mp->cm.SET_VOLUME = GetPrivateProfileInt(section, TEXT("MP_CM_SET_VOLUME"), 0, cCurrentPath);
	mp->cm.TOGGLE_MUTE = GetPrivateProfileInt(section, TEXT("MP_CM_TOGGLE_MUTE"), 0, cCurrentPath);
	mp->cm.PAUSE = GetPrivateProfileInt(section, TEXT("MP_CM_PAUSE"), 0, cCurrentPath);
	mp->cm.PLAY = GetPrivateProfileInt(section, TEXT("MP_CM_PLAY"), 0, cCurrentPath);
	mp->cm.STOP = GetPrivateProfileInt(section, TEXT("MP_CM_STOP"), 0, cCurrentPath);
	mp->cm.PREVIOUS = GetPrivateProfileInt(section, TEXT("MP_CM_PREVIOUS"), 0, cCurrentPath);
	mp->cm.NEXT = GetPrivateProfileInt(section, TEXT("MP_CM_NEXT"), 0, cCurrentPath);
	mp->cm.GET_DURATION = GetPrivateProfileInt(section, TEXT("MP_CM_GET_DURATION"), 0, cCurrentPath);
	mp->cm.GET_POSITION = GetPrivateProfileInt(section, TEXT("MP_CM_GET_POSITION"), 0, cCurrentPath);
	mp->cm.SET_POSITION = GetPrivateProfileInt(section, TEXT("MP_CM_SET_POSITION"), 0, cCurrentPath);
	mp->cm.GET_STATE = GetPrivateProfileInt(section, TEXT("MP_CM_GET_STATE"), 0, cCurrentPath);
	mp->cm.GET_FILENAME = GetPrivateProfileInt(section, TEXT("MP_CM_GET_FILENAME"), 0, cCurrentPath);

	mp->cd.GET_VOLUME = GetPrivateProfileInt(section, TEXT("MP_CD_GET_VOLUME"), 0, cCurrentPath);
	mp->cd.SET_VOLUME = GetPrivateProfileInt(section, TEXT("MP_CD_SET_VOLUME"), 0, cCurrentPath);
	mp->cd.TOGGLE_MUTE = GetPrivateProfileInt(section, TEXT("MP_CD_TOGGLE_MUTE"), 0, cCurrentPath);
	mp->cd.PAUSE = GetPrivateProfileInt(section, TEXT("MP_CD_PAUSE"), 0, cCurrentPath);
	mp->cd.PLAY = GetPrivateProfileInt(section, TEXT("MP_CD_PLAY"), 0, cCurrentPath);
	mp->cd.STOP = GetPrivateProfileInt(section, TEXT("MP_CD_STOP"), 0, cCurrentPath);
	mp->cd.PREVIOUS = GetPrivateProfileInt(section, TEXT("MP_CD_PREVIOUS"), 0, cCurrentPath);
	mp->cd.NEXT = GetPrivateProfileInt(section, TEXT("MP_CD_NEXT"), 0, cCurrentPath);
	mp->cd.GET_DURATION = GetPrivateProfileInt(section, TEXT("MP_CD_GET_DURATION"), 0, cCurrentPath);
	mp->cd.GET_POSITION = GetPrivateProfileInt(section, TEXT("MP_CD_GET_POSITION"), 0, cCurrentPath);
	mp->cd.GET_POSITION_RESPONSE = GetPrivateProfileInt(section, TEXT("MP_CD_GET_POSITION_RESPONSE"), 0, cCurrentPath);
	mp->cd.SET_POSITION = GetPrivateProfileInt(section, TEXT("MP_CD_SET_POSITION"), 0, cCurrentPath);
	mp->cd.GET_STATE = GetPrivateProfileInt(section, TEXT("MP_CD_GET_STATE"), 0, cCurrentPath);
	mp->cd.GET_FILENAME = GetPrivateProfileInt(section, TEXT("MP_CD_GET_FILENAME"), 0, cCurrentPath);
	mp->cd.GET_FILENAME_RESPONSE = GetPrivateProfileInt(section, TEXT("MP_CD_GET_FILENAME_RESPONSE"), 0, cCurrentPath);

	mp->us.GET_VOLUME = GetPrivateProfileInt(section, TEXT("MP_US_GET_VOLUME"), 0, cCurrentPath);
	mp->us.SET_VOLUME = GetPrivateProfileInt(section, TEXT("MP_US_SET_VOLUME"), 0, cCurrentPath);
	mp->us.TOGGLE_MUTE = GetPrivateProfileInt(section, TEXT("MP_US_TOGGLE_MUTE"), 0, cCurrentPath);
	mp->us.PAUSE = GetPrivateProfileInt(section, TEXT("MP_US_PAUSE"), 0, cCurrentPath);
	mp->us.PLAY = GetPrivateProfileInt(section, TEXT("MP_US_PLAY"), 0, cCurrentPath);
	mp->us.STOP = GetPrivateProfileInt(section, TEXT("MP_US_STOP"), 0, cCurrentPath);
	mp->us.PREVIOUS = GetPrivateProfileInt(section, TEXT("MP_US_PREVIOUS"), 0, cCurrentPath);
	mp->us.NEXT = GetPrivateProfileInt(section, TEXT("MP_US_NEXT"), 0, cCurrentPath);
	mp->us.GET_DURATION = GetPrivateProfileInt(section, TEXT("MP_US_GET_DURATION"), 0, cCurrentPath);
	mp->us.GET_POSITION = GetPrivateProfileInt(section, TEXT("MP_US_GET_POSITION"), 0, cCurrentPath);
	mp->us.SET_POSITION = GetPrivateProfileInt(section, TEXT("MP_US_SET_POSITION"), 0, cCurrentPath);
	mp->us.GET_STATE = GetPrivateProfileInt(section, TEXT("MP_US_GET_STATE"), 0, cCurrentPath);
	mp->us.GET_FILENAME = GetPrivateProfileInt(section, TEXT("MP_US_GET_FILENAME"), 0, cCurrentPath);
	
	mp->ap.GET_VOLUME = GetPrivateProfileInt(section, TEXT("MP_AP_GET_VOLUME"), 0, cCurrentPath);
	mp->ap.SET_VOLUME = GetPrivateProfileInt(section, TEXT("MP_AP_SET_VOLUME"), 0, cCurrentPath);
	mp->ap.TOGGLE_MUTE = GetPrivateProfileInt(section, TEXT("MP_AP_TOGGLE_MUTE"), 0, cCurrentPath);
	mp->ap.PAUSE = GetPrivateProfileInt(section, TEXT("MP_AP_PAUSE"), 0, cCurrentPath);
	mp->ap.PLAY = GetPrivateProfileInt(section, TEXT("MP_AP_PLAY"), 0, cCurrentPath);
	mp->ap.STOP = GetPrivateProfileInt(section, TEXT("MP_AP_STOP"), 0, cCurrentPath);
	mp->ap.PREVIOUS = GetPrivateProfileInt(section, TEXT("MP_AP_PREVIOUS"), 0, cCurrentPath);
	mp->ap.NEXT = GetPrivateProfileInt(section, TEXT("MP_AP_NEXT"), 0, cCurrentPath);
	mp->ap.GET_DURATION = GetPrivateProfileInt(section, TEXT("MP_AP_GET_DURATION"), 0, cCurrentPath);
	mp->ap.GET_POSITION = GetPrivateProfileInt(section, TEXT("MP_AP_GET_POSITION"), 0, cCurrentPath);
	mp->ap.SET_POSITION = GetPrivateProfileInt(section, TEXT("MP_AP_SET_POSITION"), 0, cCurrentPath);
	mp->ap.GET_STATE = GetPrivateProfileInt(section, TEXT("MP_AP_GET_STATE"), 0, cCurrentPath);
	mp->ap.GET_FILENAME = GetPrivateProfileInt(section, TEXT("MP_AP_GET_FILENAME"), 0, cCurrentPath);

	GetPrivateProfileString(section, TEXT("MP_HP_GET_VOLUME"), TEXT(""), mp->hp.GET_VOLUME, sizeof(mp->hp.GET_VOLUME) / sizeof(mp->hp.GET_VOLUME[0]), cCurrentPath);
	GetPrivateProfileString(section, TEXT("MP_HP_GET_VOLUME_TAG"), TEXT(""), mp->hp.GET_VOLUME_TAG, sizeof(mp->hp.GET_VOLUME_TAG) / sizeof(mp->hp.GET_VOLUME_TAG[0]), cCurrentPath);
	GetPrivateProfileString(section, TEXT("MP_HP_SET_VOLUME"), TEXT(""), mp->hp.SET_VOLUME, sizeof(mp->hp.SET_VOLUME) / sizeof(mp->hp.SET_VOLUME[0]), cCurrentPath);
	GetPrivateProfileString(section, TEXT("MP_HP_TOGGLE_MUTE"), TEXT(""), mp->hp.TOGGLE_MUTE, sizeof(mp->hp.TOGGLE_MUTE) / sizeof(mp->hp.TOGGLE_MUTE[0]), cCurrentPath);
	GetPrivateProfileString(section, TEXT("MP_HP_TOGGLE_MUTE_TAG"), TEXT(""), mp->hp.TOGGLE_MUTE_TAG, sizeof(mp->hp.TOGGLE_MUTE_TAG) / sizeof(mp->hp.TOGGLE_MUTE_TAG[0]), cCurrentPath);
	GetPrivateProfileString(section, TEXT("MP_HP_PAUSE"), TEXT(""), mp->hp.PAUSE, sizeof(mp->hp.PAUSE) / sizeof(mp->hp.PAUSE[0]), cCurrentPath);
	GetPrivateProfileString(section, TEXT("MP_HP_PLAY"), TEXT(""), mp->hp.PLAY, sizeof(mp->hp.PLAY) / sizeof(mp->hp.PLAY[0]), cCurrentPath);
	GetPrivateProfileString(section, TEXT("MP_HP_STOP"), TEXT(""), mp->hp.STOP, sizeof(mp->hp.STOP) / sizeof(mp->hp.STOP[0]), cCurrentPath);
	GetPrivateProfileString(section, TEXT("MP_HP_PREVIOUS"), TEXT(""), mp->hp.PREVIOUS, sizeof(mp->hp.PREVIOUS) / sizeof(mp->hp.PREVIOUS[0]), cCurrentPath);
	GetPrivateProfileString(section, TEXT("MP_HP_NEXT"), TEXT(""), mp->hp.NEXT, sizeof(mp->hp.NEXT) / sizeof(mp->hp.NEXT[0]), cCurrentPath);
	GetPrivateProfileString(section, TEXT("MP_HP_GET_DURATION"), TEXT(""), mp->hp.GET_DURATION, sizeof(mp->hp.GET_DURATION) / sizeof(mp->hp.GET_DURATION[0]), cCurrentPath);
	GetPrivateProfileString(section, TEXT("MP_HP_GET_DURATION_TAG"), TEXT(""), mp->hp.GET_DURATION_TAG, sizeof(mp->hp.GET_DURATION_TAG) / sizeof(mp->hp.GET_DURATION_TAG[0]), cCurrentPath);
	GetPrivateProfileString(section, TEXT("MP_HP_GET_POSITION"), TEXT(""), mp->hp.GET_POSITION, sizeof(mp->hp.GET_POSITION) / sizeof(mp->hp.GET_POSITION[0]), cCurrentPath);
	GetPrivateProfileString(section, TEXT("MP_HP_GET_POSITION_TAG"), TEXT(""), mp->hp.GET_POSITION_TAG, sizeof(mp->hp.GET_POSITION_TAG) / sizeof(mp->hp.GET_POSITION_TAG[0]), cCurrentPath);
	GetPrivateProfileString(section, TEXT("MP_HP_SET_POSITION"), TEXT(""), mp->hp.SET_POSITION, sizeof(mp->hp.SET_POSITION) / sizeof(mp->hp.SET_POSITION[0]), cCurrentPath);
	GetPrivateProfileString(section, TEXT("MP_HP_GET_STATE"), TEXT(""), mp->hp.GET_STATE, sizeof(mp->hp.GET_STATE) / sizeof(mp->hp.GET_STATE[0]), cCurrentPath);
	GetPrivateProfileString(section, TEXT("MP_HP_GET_STATE_TAG"), TEXT(""), mp->hp.GET_STATE_TAG, sizeof(mp->hp.GET_STATE_TAG) / sizeof(mp->hp.GET_STATE_TAG[0]), cCurrentPath);
	GetPrivateProfileString(section, TEXT("MP_HP_GET_FILENAME"), TEXT(""), mp->hp.GET_FILENAME, sizeof(mp->hp.GET_FILENAME) / sizeof(mp->hp.GET_FILENAME[0]), cCurrentPath);
	GetPrivateProfileString(section, TEXT("MP_HP_GET_FILENAME_TAG"), TEXT(""), mp->hp.GET_FILENAME_TAG, sizeof(mp->hp.GET_FILENAME_TAG) / sizeof(mp->hp.GET_FILENAME_TAG[0]), cCurrentPath);

	return GetLastError() ? FALSE : TRUE;
}

BOOL CALLBACK EnumWndProc(HWND hWnd, LPARAM lparam)
{
    TCHAR *title = getWindowTitle(hWnd);
	TCHAR className[256] = {0};

	GetClassName(hWnd, className, sizeof(className));

	if (title[0] != '\0' && _tcsstr(title, mp->TITLE) && _tcsstr(className, mp->CLASS)) {
		mp->hMP = hWnd;
		free(title);
		return FALSE;
	}

	free(title);

    return TRUE;
}

// Open MP and get it's handle
BOOL mpOpen(const TCHAR *mpPath, const TCHAR *fp)
{
    STARTUPINFOW StartupInfo;
    PROCESS_INFORMATION ProcessInfo;
	TCHAR *mpPathSet = NULL;
	int i = 0;

	mpPathSet = (TCHAR*)calloc(_tcslen(mp->PATH) + _tcslen(fp) + 20 + 1, sizeof(TCHAR));

	_tcscpy(mpPathSet, mpPath[0] == '\0' ? mp->PATH : mpPath);

	if (_tcsstr(mp->NAME, TEXT("MPC-"))) {
		_stprintf(mpPathSet + _tcslen(mpPathSet), TEXT(" /slave %d %s"), hWin, fp);
	} else {
		_stprintf(mpPathSet + _tcslen(mpPathSet), TEXT(" %s"), fp);
	}

	memset(&StartupInfo, 0, sizeof(StartupInfo));
	StartupInfo.cb = sizeof(StartupInfo);
    GetStartupInfoW(&StartupInfo);

	if (CreateProcess(NULL, mpPathSet, NULL, NULL, FALSE, 0, NULL, NULL, &StartupInfo, &ProcessInfo)) {
        CloseHandle(ProcessInfo.hProcess);
        CloseHandle(ProcessInfo.hThread);

		while ((mp->hMP = FindWindow(_tcsstr(mp->CLASS, TEXT("NULL")) ? NULL : mp->CLASS, _tcsstr(mp->CAPTION, TEXT("NULL")) ? NULL : mp->CAPTION)) == NULL && i < 30) {	// 3 seconds max
			Sleep(100);
			i++;
		}

		if (!mp->hMP) {	// try enum windows
			EnumWindows(EnumWndProc, NULL);
		}
	}

	free(mpPathSet);

	return mp->hMP ? TRUE : FALSE;
}

// Get MP handle
HWND mpGetHandle()
{
	if (!IsWindow(mp->hMP)) mp->hMP = FindWindow(_tcsstr(mp->CLASS, TEXT("NULL")) ? NULL : mp->CLASS, _tcsstr(mp->CAPTION, TEXT("NULL")) ? NULL : mp->CAPTION);

	if (!mp->hMP) {	// try enum windows
		EnumWindows(EnumWndProc, NULL);
	}

	return mp->hMP;
}

BOOL mpSendHttpRequest(TCHAR *server, int port, TCHAR *method, TCHAR *path, TCHAR *user, TCHAR *pass, char *outBuffer, int outBufferSize)
{
	HINTERNET hOpenHandle, hResourceHandle, hConnectHandle;
	DWORD dwStatus;
	DWORD dwStatusSize = sizeof(dwStatus);
	DWORD dwSize = outBufferSize - 1;
	int retries = 0;
	int max_retries = 3;
	BOOL retval = FALSE;

	hOpenHandle = InternetOpen(TEXT("upnpRenderer v0.1 by jobnik.net"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	hConnectHandle = InternetConnect(hOpenHandle, server, port, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
	hResourceHandle = HttpOpenRequest(hConnectHandle, method, path, NULL, NULL, NULL, INTERNET_FLAG_KEEP_CONNECTION, 0);

	for (; retries < max_retries; retries++)
	{
		HttpSendRequest(hResourceHandle, NULL, 0, NULL, 0);
		HttpQueryInfo(hResourceHandle, HTTP_QUERY_FLAG_NUMBER | HTTP_QUERY_STATUS_CODE, &dwStatus, &dwStatusSize, NULL);

		switch (dwStatus)
		{
			DWORD userLen, passLen;

			case HTTP_STATUS_PROXY_AUTH_REQ:	// Proxy Authentication Required
				InternetSetOption(hResourceHandle, INTERNET_OPTION_PROXY_USERNAME, user, userLen + 1);
				InternetSetOption(hResourceHandle, INTERNET_OPTION_PROXY_PASSWORD, pass, passLen + 1);
			break;
			case HTTP_STATUS_DENIED:			// Server Authentication Required.
				InternetSetOption(hResourceHandle, INTERNET_OPTION_USERNAME, user, userLen + 1);
				InternetSetOption(hResourceHandle, INTERNET_OPTION_PASSWORD, pass, passLen + 1);
			break;
			case HTTP_STATUS_OK:
				if (outBufferSize) {
					InternetReadFile(hResourceHandle, (LPVOID*)outBuffer, dwSize, &dwSize);
				}
				retries = max_retries;
				retval = TRUE;
			break;
		}
	}

	InternetCloseHandle(hResourceHandle);
	InternetCloseHandle(hConnectHandle);
	InternetCloseHandle(hOpenHandle);

	return retval;
}

// Control MP using SendMessage
int mpSendCommands(WPARAM cmd, LPARAM val)
{
    if (cmd == mp->cm.GET_VOLUME || cmd == mp->cm.SET_VOLUME || cmd == mp->cm.TOGGLE_MUTE ||
		cmd == mp->cm.PAUSE || cmd == mp->cm.PLAY || cmd == mp->cm.STOP ||
		cmd == mp->cm.PREVIOUS || cmd == mp->cm.NEXT ||
		cmd == mp->cm.GET_DURATION || cmd == mp->cm.GET_POSITION || cmd == mp->cm.SET_POSITION ||
		cmd == mp->cm.GET_STATE || cmd == mp->cm.GET_FILENAME)
        {
            return SendMessage(mp->hMP, WM_COMMAND, cmd, val);
        }

    if (cmd == mp->cd.GET_VOLUME || cmd == mp->cd.SET_VOLUME || cmd == mp->cd.TOGGLE_MUTE ||
		cmd == mp->cd.PAUSE || cmd == mp->cd.PLAY || cmd == mp->cd.STOP ||
		cmd == mp->cd.PREVIOUS || cmd == mp->cd.NEXT ||
		cmd == mp->cd.GET_DURATION || cmd == mp->cd.GET_POSITION || cmd == mp->cd.SET_POSITION ||
		cmd == mp->cd.GET_STATE || cmd == mp->cd.GET_FILENAME)
        {
            COPYDATASTRUCT cds = {0};

			if (!val) val = (LPARAM)L"";

        	cds.dwData = cmd;
			cds.cbData = (DWORD)(wcslen((wchar_t*)val) + 1) * sizeof(wchar_t);
       		cds.lpData = (LPVOID)val;

            return SendMessage(mp->hMP, WM_COPYDATA, (WPARAM)hWin, (LPARAM)&cds);
        }

    if (cmd == mp->us.GET_VOLUME || cmd == mp->us.SET_VOLUME || cmd == mp->us.TOGGLE_MUTE ||
		cmd == mp->us.PAUSE || cmd == mp->us.PLAY || cmd == mp->us.STOP ||
		cmd == mp->us.PREVIOUS || cmd == mp->us.NEXT ||
		cmd == mp->us.GET_DURATION || cmd == mp->us.GET_POSITION || cmd == mp->us.SET_POSITION ||
		cmd == mp->us.GET_STATE || cmd == mp->us.GET_FILENAME)
        {
            return SendMessage(mp->hMP, WM_USER, cmd, val);
        }

    if (cmd == mp->ap.GET_VOLUME || cmd == mp->ap.SET_VOLUME || cmd == mp->ap.TOGGLE_MUTE ||
		cmd == mp->ap.PAUSE || cmd == mp->ap.PLAY || cmd == mp->ap.STOP ||
		cmd == mp->ap.PREVIOUS || cmd == mp->ap.NEXT ||
		cmd == mp->ap.GET_DURATION || cmd == mp->ap.GET_POSITION || cmd == mp->ap.SET_POSITION ||
		cmd == mp->ap.GET_STATE || cmd == mp->ap.GET_FILENAME)
        {
            return SendMessage(mp->hMP, WM_APPCOMMAND, 0, (LPARAM)(cmd << 16));
        }

	return 0;
}

int getHttpIntVal(TCHAR *path, TCHAR *tag)
{
	char *cval, outBuf[WBUFFER_SIZE] = {0};
    int iout = 0;

	mpSendHttpRequest(mp->HOST, mp->PORT, TEXT("GET"), path, mp->USER, mp->PASS, &outBuf, WBUFFER_SIZE);

	cval = getTagValue(outBuf, tag);

    if (cval) {
        iout = atoi(cval);
        free(cval);
    }

    return iout;
}

char *getHttpStringVal(TCHAR *path, wchar_t *tag)
{
	char buf[WBUFFER_SIZE] = {0};

	mpSendHttpRequest(mp->HOST, mp->PORT, TEXT("GET"), path, mp->USER, mp->PASS, &buf, WBUFFER_SIZE);

	return getTagValue(buf, tag);
}

// Get Volume
int mpGetVolume()
{
	if (mp->hp.GET_VOLUME[0] != '\0') {
		mp->pos.volume = getHttpIntVal(mp->hp.GET_VOLUME, mp->hp.GET_VOLUME_TAG);
	} else {
		if (mp->cd.GET_VOLUME) {
			mpSendCommands(mp->cm.GET_VOLUME | mp->cd.GET_VOLUME | mp->us.GET_VOLUME | mp->ap.GET_VOLUME, 0);
		} else {
			mp->pos.volume = mpSendCommands(mp->cm.GET_VOLUME | mp->cd.GET_VOLUME | mp->us.GET_VOLUME | mp->ap.GET_VOLUME, 0);
		}
	}

	return mp->pos.volume;
}

// Set Volume
void mpSetVolume(int vol)
{
	TCHAR tbuf[50] = {0};

	if (mp->hp.SET_VOLUME[0] != '\0') {
		_stprintf(tbuf, TEXT("%s%d"), mp->hp.SET_VOLUME, vol);
		mpSendHttpRequest(mp->HOST, mp->PORT, TEXT("GET"), tbuf, mp->USER, mp->PASS, "", 0);
	} else {
		mpSendCommands(mp->cm.SET_VOLUME | mp->cd.SET_VOLUME | mp->us.SET_VOLUME | mp->ap.SET_VOLUME, vol);
	}

	mp->pos.volume = vol;
}

// Toggle Mute
void mpToggleMute()
{
	if (mp->hp.TOGGLE_MUTE[0] != '\0') {
		mpSendHttpRequest(mp->HOST, mp->PORT, TEXT("GET"), mp->hp.TOGGLE_MUTE, mp->USER, mp->PASS, NULL, 0);
	} else {
    	mpSendCommands(mp->cm.TOGGLE_MUTE | mp->cd.TOGGLE_MUTE | mp->us.TOGGLE_MUTE | mp->ap.TOGGLE_MUTE, 0);
	}
}

// Play
void mpPlay()
{
	if (mp->hp.PLAY[0] != '\0') {
		mpSendHttpRequest(mp->HOST, mp->PORT, TEXT("GET"), mp->hp.PLAY, mp->USER, mp->PASS, NULL, 0);
	} else {
	    mpSendCommands(mp->cm.PLAY | mp->cd.PLAY | mp->us.PLAY | mp->ap.PLAY, 0);
	}
}

// Pause
void mpPause()
{
	if (mp->hp.PAUSE[0] != '\0') {
		mpSendHttpRequest(mp->HOST, mp->PORT, TEXT("GET"), mp->hp.PAUSE, mp->USER, mp->PASS, NULL, 0);
	} else {
    	mpSendCommands(mp->cm.PAUSE | mp->cd.PAUSE | mp->us.PAUSE | mp->ap.PAUSE, 0);
	}
}

// Stop
void mpStop()
{
	if (mp->hp.STOP[0] != '\0') {
		mpSendHttpRequest(mp->HOST, mp->PORT, TEXT("GET"), mp->hp.STOP, mp->USER, mp->PASS, NULL, 0);
	} else {
	    mpSendCommands(mp->cm.STOP | mp->cd.STOP | mp->us.STOP | mp->ap.STOP, 0);
	}
}

// Get State (play/pause/stop)
int mpGetState()
{
	if (mp->hp.GET_STATE[0] != '\0') {
        char *cstate = getHttpStringVal(mp->hp.GET_STATE, mp->hp.GET_STATE_TAG);

        if (cstate) {
            cstate = strlwr(cstate);

    		if (strstr(cstate, "play")) mp->pos.state = mp->STATE_PLAY;
    		else if (strstr(cstate, "pause")) mp->pos.state = mp->STATE_PAUSE;
    		else if (strstr(cstate, "stop")) mp->pos.state = mp->STATE_STOP;

    		free(cstate);
        }
	} else {
		if (mp->cd.GET_STATE) {
			mpSendCommands(mp->cm.GET_STATE | mp->cd.GET_STATE | mp->us.GET_STATE | mp->ap.GET_STATE, 0);
		} else {
			mp->pos.state = mpSendCommands(mp->cm.GET_STATE | mp->cd.GET_STATE | mp->us.GET_STATE | mp->ap.GET_STATE, 0);
		}
	}

	return mp->pos.state;
}

// Set Satate
void mpSetState(int state)
{
	mp->pos.state = state;
}

// Get Position
int mpGetPosition()
{
	if (mp->hp.GET_POSITION[0] != '\0') {
		mp->pos.position = getHttpIntVal(mp->hp.GET_POSITION, mp->hp.GET_POSITION_TAG);
	} else {
		if (mp->cd.GET_POSITION) {
			mpSendCommands(mp->cm.GET_POSITION | mp->cd.GET_POSITION | mp->us.GET_POSITION | mp->ap.GET_POSITION, 0);
			return mp->pos.position;
		} else {
			mp->pos.position = mpSendCommands(mp->cm.GET_POSITION | mp->cd.GET_POSITION | mp->us.GET_POSITION | mp->ap.GET_POSITION, 0);
		}
	}

	mp->pos.position /= (mp->IS_MS ? 1000 : 1);

	return mp->pos.position;
}

// Set Position
void mpSetPosition(int pos)
{
	wchar_t wbuf[255] = {0}, wpos[255] = {0};
	char buf[255] = {0};

	mp->pos.position = pos;
    pos *= (mp->IS_MS ? 1000 : 1);

	if (mp->hp.SET_POSITION[0] != '\0') {
		if (_tcsstr(mp->NAME, TEXT("MPC-"))) {
			pos /= (mp->IS_MS ? 1000 : 1);	// GET in MS, SET in Time string format HH:MM:SS
		    SetTimeString(&buf, pos);
		    char2wchar(buf, &wpos);
			_swprintf(wbuf, L"%s%s", mp->hp.SET_POSITION, wpos);
		} else {
			_swprintf(wbuf, L"%s%d", mp->hp.SET_POSITION, pos);
		}

		mpSendHttpRequest(mp->HOST, mp->PORT, TEXT("GET"), wbuf, mp->USER, mp->PASS, "", 0);
	} else {
		mpSendCommands(mp->cm.SET_POSITION | mp->cd.SET_POSITION | mp->us.SET_POSITION | mp->ap.SET_POSITION, mp->cd.SET_POSITION ? (LPARAM)_itow(pos, wbuf, 10) : pos);
	}
}

// Get Duration
int mpGetDuration()
{
	if (mp->hp.GET_DURATION[0] != '\0') {
		mp->pos.duration = getHttpIntVal(mp->hp.GET_DURATION, mp->hp.GET_DURATION_TAG);
	} else {
		if (mp->cd.GET_DURATION) {
			mpSendCommands(mp->cm.GET_DURATION | mp->cd.GET_DURATION | mp->us.GET_DURATION | mp->ap.GET_DURATION, 0);
			return mp->pos.duration;
		} else {
			mp->pos.duration = mpSendCommands(mp->cm.GET_DURATION | mp->cd.GET_DURATION | mp->us.GET_DURATION | mp->ap.GET_DURATION, 0);
		}
	}

	mp->pos.duration /= (mp->IS_MS ? 1000 : 1);

	return mp->pos.duration;
}

// Next
void mpNext()
{
	if (mp->hp.NEXT[0] != '\0') {
		mpSendHttpRequest(mp->HOST, mp->PORT, TEXT("GET"), mp->hp.NEXT, mp->USER, mp->PASS, NULL, 0);
	} else {
	    mpSendCommands(mp->cm.NEXT | mp->cd.NEXT | mp->us.NEXT | mp->ap.NEXT, 0);
	}
}

// Previous
void mpPrevious()
{
	if (mp->hp.PREVIOUS[0] != '\0') {
		mpSendHttpRequest(mp->HOST, mp->PORT, TEXT("GET"), mp->hp.PREVIOUS, mp->USER, mp->PASS, NULL, 0);
	} else {
    	mpSendCommands(mp->cm.PREVIOUS | mp->cd.PREVIOUS | mp->us.PREVIOUS | mp->ap.PREVIOUS, 0);
	}
}

// Get filename
wchar_t *mpGetFilename(LPARAM val)
{
	wchar_t wbuf[WBUFFER_SIZE] = {0};

	if (mp->hp.GET_FILENAME[0] != '\0') {
		char *cfilename = getHttpStringVal(mp->hp.GET_FILENAME, mp->hp.GET_FILENAME_TAG);

        if (cfilename) {
            char2wchar(cfilename, wbuf);
    
    		_swprintf(mp->pos.filename, L"%s", wbuf);
    
    		free(cfilename);
        }

	} else {
	    mpSendCommands(mp->cm.GET_FILENAME | mp->cd.GET_FILENAME | mp->us.GET_FILENAME | mp->ap.GET_FILENAME, mp->cd.GET_FILENAME ? (LPARAM)_itow(val, wbuf, 10) : val);
	}

	return mp->pos.filename;
}

// Free all resources used by MP
void mpFree()
{
	if (mp) free(mp);
}
