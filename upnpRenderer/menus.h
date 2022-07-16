/*
	(c) 2021 (: JOBnik! :) Arthur Aminov, Israel
	https://www.jobnik.net
*/

#ifndef MENUS_H
#define MENUS_H

#define TRAY_ICONUID    100
#define WM_TRAYMESSAGE  (WM_USER + 1)

void GetMediaPlayersList(char *curMediaPlayer, char *mediaPlayers, HMENU hMenuControl);

int GetMenuItemPosition(HMENU hMenu, UINT id);

int GetMenuItemByText(HMENU hMenu, const TCHAR *mFind);

void CheckControlMenu(const char *cstr, HWND hWin, HMENU hMenuControl);

// https://www.lotushints.com/2013/03/win32-hide-to-system-tray-part-2/
void notifyIconAdd(HWND hWnd, HICON hIcon, HINSTANCE hInst);

void notifyIconDelete(HWND hWnd);

void notifyIconPopupMenu(HWND hWnd, HMENU hMenu);

// add "Control" popup menu by finding configuration files (.ini)
BOOL iniToControlMenu(HWND hWnd, HMENU hMenu, TCHAR *lastMediaPlayer, HMENU *hMenuControl);

LRESULT wmCommandMenu(HWND h, UINT m, WPARAM w, LPARAM l, char *friendlyName, HMENU hMenu, HMENU hMenuControl, HANDLE threadUpnp);

#endif 
