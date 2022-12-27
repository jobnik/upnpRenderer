/*
	upnpRenderer v0.1

	(c) 2021 (: JOBnik! :) Arthur Aminov, Israel
	https://www.jobnik.net
*/

#include "../upnpRenderer.h"
#include <shellapi.h>

#include "menus.h"
#include "functions.h"
#include "MediaPlayerControl.h"

extern void upnpStart();
extern void upnpStop();
extern BOOL isUpnpStarted;
extern BOOL isAnyMediaPlayerExists;

void GetMediaPlayersList(char *curMediaPlayer, char *mediaPlayers, HMENU hMenuControl)
{
	char menuString[50] = {0};
	MENUITEMINFO mii = {0};
    int i = 0;

	int mc = GetMenuItemCount(hMenuControl);

	mii.cbSize = sizeof(mii);
	mii.fMask = MIIM_STATE;

	for (; i < mc; i++) {
		GetMenuStringA(hMenuControl, i, menuString, sizeof(menuString), MF_BYPOSITION);
		strcat(mediaPlayers, menuString);
		if (i < mc - 1) strcat(mediaPlayers, ",");

	    GetMenuItemInfo(hMenuControl, i, TRUE, &mii);

	    if (mii.fState == MF_CHECKED) {
	        sprintf(curMediaPlayer, "%s", menuString);
	    }
	}
}

int GetMenuItemPosition(HMENU hMenu, UINT id)
{
	int i = 0;

	for (; i < GetMenuItemCount(hMenu); i++) {
		if (GetMenuItemID(hMenu, i) == id) return i;
	}

	return -1;
}

int GetMenuItemByText(HMENU hMenu, const TCHAR *mFind)
{
    int i = 0;
	TCHAR menuString[50] = {0};

	for (; i < GetMenuItemCount(hMenu); i++) {
		GetMenuString(hMenu, i, menuString, sizeof(menuString), MF_BYPOSITION);
		if (!_tcscmp(menuString, mFind)) return GetMenuItemID(hMenu, i);
	}

    return -1;
}

void CheckControlMenu(const char *cstr, HWND hWin, HMENU hMenuControl)
{
    wchar_t wstr[WBUFFER_SIZE] = {0};
    int mId = 0;

    char2wchar(cstr, wstr);

    mId = GetMenuItemByText(hMenuControl, wstr);

    if (mId != -1) {
        CheckMenuRadioItem(hMenuControl, GetMenuItemID(hMenuControl, 0), GetMenuItemID(hMenuControl, GetMenuItemCount(hMenuControl) - 1), mId, MF_BYCOMMAND);
        SendMessage(hWin, WM_COMMAND, MAKELONG(mId, 0), 0);
    }
}

// https://www.lotushints.com/2013/03/win32-hide-to-system-tray-part-2/
void notifyIconAdd(HWND hWnd, HICON hIcon, HINSTANCE hInst)
{
	NOTIFYICONDATA nid;

	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd = hWnd;
	nid.uID = TRAY_ICONUID;
	nid.uVersion = NOTIFYICON_VERSION;
	nid.uCallbackMessage = WM_TRAYMESSAGE;
	nid.hIcon = hIcon;

	LoadString(hInst, IDR_TRAYMENU_TT, nid.szTip, 128);

	nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;

	Shell_NotifyIcon(NIM_ADD, &nid);
}

void notifyIconDelete(HWND hWnd)
{
	NOTIFYICONDATA nid;

	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd = hWnd;
	nid.uID = TRAY_ICONUID;

	Shell_NotifyIcon(NIM_DELETE, &nid);
}

void notifyIconPopupMenu(HWND hWnd, HMENU hMenu)
{
	POINT cursor;

	GetCursorPos(&cursor);

	TrackPopupMenu(hMenu, TPM_LEFTALIGN, cursor.x, cursor.y, 0, hWnd, NULL);
}

// add "Control" popup menu by finding configuration files (.ini)
BOOL iniToControlMenu(HWND hWnd, HMENU hMenu, TCHAR *lastMediaPlayer, HMENU *hMenuControl)
{
	TCHAR szDir[BUFFER_SIZE], szDirTemp[BUFFER_SIZE];
	WIN32_FIND_DATA ffd, ffdMP;
	HANDLE hFind = INVALID_HANDLE_VALUE, hFindMP = INVALID_HANDLE_VALUE;
	TCHAR mp_name[50] = {0}, mp_path[1024] = {0};
	int i = 0;
	BOOL isMediaPlayerExists = FALSE;

	GetCurrentDirectory(sizeof(szDir), szDir);

	#ifdef _DEBUG
		_tcscat(szDir, TEXT("..\\..\\Debug\\"));
	#else
		_tcscat(szDir, TEXT("\\"));
	#endif

	_stprintf(szDirTemp, TEXT("%s%s"), szDir, TEXT("*.ini"));

	hFind = FindFirstFile(szDirTemp, &ffd);

	do {
		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) continue;

		_stprintf(szDirTemp, TEXT("%s%s"), szDir, ffd.cFileName);

		// make sure it's a valid configuration file
		GetPrivateProfileString(TEXT("MediaPlayer"), TEXT("MP_NAME"), TEXT(""), mp_name, sizeof(mp_name) / sizeof(mp_name[0]), szDirTemp);
		GetPrivateProfileString(TEXT("MediaPlayer"), TEXT("MP_PATH"), TEXT(""), mp_path, sizeof(mp_path) / sizeof(mp_path[0]), szDirTemp);

		if (mp_name[0] != '\0') {
			hFindMP = FindFirstFile(mp_path, &ffdMP);	// check if media player exists

			if (hFindMP != INVALID_HANDLE_VALUE) {
				isMediaPlayerExists = isAnyMediaPlayerExists = TRUE;
			} else {
				isMediaPlayerExists = FALSE;
			}

			if (!*hMenuControl) *hMenuControl = CreatePopupMenu();
			AppendMenu(*hMenuControl, MF_STRING | (!isMediaPlayerExists ? MF_DISABLED : 0), IDM_PLAYERHANDLE + i, mp_name);

			// set default player to control
			if (!_tcscmp(mp_name, lastMediaPlayer) && isMediaPlayerExists) {
				SendMessage(hWnd, WM_COMMAND, MAKELONG(IDM_PLAYERHANDLE + i, 0), 0);
			}

			i += 1000;
		}
	} while (FindNextFile(hFind, &ffd) != 0);

	FindClose(hFind);
	FindClose(hFindMP);

	if (*hMenuControl) {
		InsertMenu(hMenu, IDM_ABOUT, MF_POPUP | MF_BYCOMMAND, (UINT_PTR)*hMenuControl, TEXT("Control"));
		return TRUE;
	} else {
		wprintft(TEXT("Couldn't find any Media Player to Control"));
	}

	return FALSE;
}

LRESULT wmCommandMenu(HWND h, UINT m, WPARAM w, LPARAM l, char *friendlyName, HMENU hMenu, HMENU hMenuControl, HANDLE threadUpnp)
{
    if (m == WM_INITMENUPOPUP) {
		if (GetMenuItemID((HMENU)w, 0) == -1) {
			if (!threadUpnp) {
				ModifyMenu((HMENU)w, 0, MF_BYPOSITION | MF_STRING, 0, TEXT(upnpVersionString));
			} else {
				ModifyMenuA((HMENU)w, 0, MF_BYPOSITION | MF_STRING, 0, friendlyName);
			}
		}
    } else {
    	DWORD wmId = LOWORD(w);
    	DWORD wmEvent = HIWORD(w);
    
    	switch (wmId) {
    		case IDM_START_STOP:
    			if (threadUpnp) {
                    ModifyMenu(hMenu, IDM_START_STOP, MF_BYCOMMAND | MF_STRING, IDM_START_STOP, TEXT("Start"));
    			    upnpStop();
    			} else {
                    ModifyMenu(hMenu, IDM_START_STOP, MF_BYCOMMAND | MF_STRING, IDM_START_STOP, TEXT("Stop"));
    				upnpStart();
    			}
    		break;
    		case IDM_ABOUT:
    		    MessageBox(h, TEXT(upnpAbout), TEXT("About"), MB_OK | MB_ICONINFORMATION);
    		break;
    		case IDM_CLOSE:
    			SendMessage(h, WM_CLOSE, 0, 0);
    		break;
    		default:
    		{
            	int menuPos = GetMenuItemPosition(hMenuControl, wmId);
            
            	if (menuPos >= 0) {
            		TCHAR menuString[50] = {0};
	                TCHAR cCurrentPath[BUFFER_SIZE] = {0};

            		CheckMenuRadioItem(hMenuControl, 0, GetMenuItemCount(hMenuControl) - 1, menuPos, MF_BYPOSITION);
            
            		GetMenuString(hMenuControl, wmId, menuString, sizeof(menuString), MF_BYCOMMAND);
            
            		mpFree();
            		mpCreate();
            		mpLoadDataINI(menuString);

                    GetCurrentDirectory(sizeof(cCurrentPath), cCurrentPath);
                    
                    #ifdef _DEBUG
                    	_stprintf(cCurrentPath, TEXT("..\\debug\\upnpRenderer.ini"));
                    #else
                    	_stprintf(cCurrentPath, TEXT("%s\\upnpRenderer.ini"), cCurrentPath);
                    #endif

                    WritePrivateProfileString(TEXT("upnpRenderer"), TEXT("lastMediaPlayer"), menuString, cCurrentPath);

            		if (isUpnpStarted) wprintft(TEXT("Controlling Media Player: %s\r\n"), menuString);
            	} else {
    			    return DefWindowProc(h, m, w, l);
            	}
    		}
    	}
    }

    return 1;
}
