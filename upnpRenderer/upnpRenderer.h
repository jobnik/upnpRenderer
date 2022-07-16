/*
	(c) 2021 (: JOBnik! :) Arthur Aminov, Israel
	https://www.jobnik.net
*/

#pragma once

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>

#define upnpVersionString "upnpRenderer v0.1 by jobnik.net"
#define upnpAbout "upnpRenderer v0.1\r\n(c) 2021 (: JOBnik! :) Arthur Aminov, Israel\r\nBased on Intel's UPnP Media Renderer"

#define BUFFER_SIZE     1024
#define WBUFFER_SIZE    BUFFER_SIZE * 10

#define ID_DIALOG_MAIN  1000
#define ID_PRINT_TEXT	1001
#define IDI_ICON		1010

#define ID_TAB			110
#define IDC_STATIC	    -1

#define IDR_TRAYMENU        10000
#define IDR_TRAYMENU_TT     10001
#define IDM_FRIENDLYNAME    10002
#define IDM_START_STOP      10003
#define IDM_CONTROL         10004
#define IDM_PLAYERHANDLE    10005
#define IDM_ABOUT           10006
#define IDM_CLOSE           10007

#define MESS(id, m, w, l) SendDlgItemMessage(hWin, id, m, (WPARAM)w, (LPARAM)l)
#define DLGITEM(id) GetDlgItem(hWin, id)
