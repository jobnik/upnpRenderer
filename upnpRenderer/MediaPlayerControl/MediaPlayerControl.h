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

#ifndef MP_CONTROL_H
#define MP_CONTROL_H

typedef struct {
    DWORD GET_VOLUME;
    DWORD SET_VOLUME;
    DWORD TOGGLE_MUTE;
    DWORD PAUSE;
    DWORD PLAY;
    DWORD STOP;
    DWORD PREVIOUS;
    DWORD NEXT;
    DWORD GET_DURATION;
    DWORD GET_POSITION;
	DWORD GET_POSITION_RESPONSE;
    DWORD SET_POSITION;
    DWORD GET_STATE;
    DWORD GET_FILENAME;
    DWORD GET_FILENAME_RESPONSE;
} mpCommands;

typedef struct {
    TCHAR GET_VOLUME[255];
	TCHAR GET_VOLUME_TAG[50];
    TCHAR SET_VOLUME[255];
    TCHAR TOGGLE_MUTE[255];
	TCHAR TOGGLE_MUTE_TAG[50];
    TCHAR PAUSE[255];
    TCHAR PLAY[255];
    TCHAR STOP[255];
    TCHAR PREVIOUS[255];
    TCHAR NEXT[255];
    TCHAR GET_DURATION[255];
	TCHAR GET_DURATION_TAG[50];
    TCHAR GET_POSITION[255];
	TCHAR GET_POSITION_TAG[50];
    TCHAR SET_POSITION[255];
    TCHAR GET_STATE[255];
	TCHAR GET_STATE_TAG[50];
    TCHAR GET_FILENAME[255];
	TCHAR GET_FILENAME_TAG[50];
} mpCommandsHTTP;

typedef struct {
    wchar_t filename[WBUFFER_SIZE];
    int duration;
    int position;
	int state;
	int volume;
	int mute;
} mpPosition;

struct MP {
    HWND hMP;

    TCHAR NAME[50];
	TCHAR TITLE[50];
    TCHAR PATH[BUFFER_SIZE];
    TCHAR CLASS[50];
    TCHAR CAPTION[50];
	TCHAR HOST[100];
	int PORT;
	TCHAR USER[100];
	TCHAR PASS[100];

    BOOL IS_MS;  // in milliseconds?

    // States
    int STATE_STOP;
    int STATE_PAUSE;
    int STATE_PLAY;

    mpPosition pos;

    mpCommands cm; // WM_COMMAND
    mpCommands cd; // WM_COPYDATA
    mpCommands us; // WM_USER
    mpCommands ap; // WM_APPCOMMAND
	mpCommandsHTTP hp; // HTTP Request
};

// Create MP object
void *mpCreate();

// Load Data from INI File
BOOL mpLoadDataINI(const TCHAR *ini);

// Open MP and get it's handle
BOOL mpOpen(const TCHAR *mpPath, const TCHAR *fp);

// Get main window Handle
HWND mpGetHandle();

// Control MP using SendMessage
int mpSendCommands(WPARAM cmd, LPARAM val);

// Get Volume
int mpGetVolume();

// Set Volume
void mpSetVolume(int vol);

// Toggle Mute
void mpToggleMute();

// Play
void mpPlay();

// Pause
void mpPause();

// Stop
void mpStop();

// Get State (play/pause/stop)
int mpGetState();

// Set Satate
void mpSetState(int state);

// Get Position
int mpGetPosition();

// Set Position
void mpSetPosition(int pos);

// Get Duration
int mpGetDuration();

// Next
void mpNext();

// Previous
void mpPrevious();

// Get filename
wchar_t *mpGetFilename(LPARAM val);

// Free all resources used by MP
void mpFree();

#endif
