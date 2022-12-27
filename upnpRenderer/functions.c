/*
	upnpRenderer v0.1

	(c) 2021 (: JOBnik! :) Arthur Aminov, Israel
	https://www.jobnik.net
*/

#include "upnpRenderer.h"
#include "functions.h"
#include <WinInet.h>

#ifndef ID_PRINT_TEXT
#define ID_PRINT_TEXT   1001
#endif

extern HWND hWin;
typedef void (WINAPI * RtlGetVersion_FUNC) (OSVERSIONINFOEXW *);

// Get Windows version
BOOL RtlGetVersion(OSVERSIONINFOEX *os)
{
    HMODULE hMod;
    RtlGetVersion_FUNC func;
    OSVERSIONINFOEXW *osw = os;

    hMod = LoadLibrary(TEXT("ntdll.dll"));

    if (hMod) {
        func = (RtlGetVersion_FUNC)GetProcAddress(hMod, "RtlGetVersion");
        if (func == 0) {
            FreeLibrary(hMod);
            return FALSE;
        }
        ZeroMemory(osw, sizeof (*osw));
        osw->dwOSVersionInfoSize = sizeof (*osw);
        func(osw);
    } else
        return FALSE;

    FreeLibrary(hMod);

    return TRUE;
}

// Send HTTP Request and get a Response
BOOL sendHttpRequest(TCHAR *server, int port, TCHAR *method, TCHAR *path, TCHAR *agent, TCHAR *user, TCHAR *pass, char *outBuffer, int outBufferSize)
{
	HINTERNET hOpenHandle, hResourceHandle, hConnectHandle;
	DWORD dwStatus;
	DWORD dwStatusSize = sizeof(dwStatus);
	DWORD dwSize = outBufferSize - 1;
	int retries = 0;
	int max_retries = 3;
	BOOL retval = FALSE;

	hOpenHandle = InternetOpen(agent, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
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

// https://stackoverflow.com/questions/24696113/how-to-find-text-between-two-strings-in-c
char* getBetween(const char *s, const char *from, const char *to)
{
	char *sCopy = _strdup(s);
    char *target = NULL;
    char *start, *end;
    int size = 0;

    if (start = strstr(sCopy, from))
    {
        start += strlen(from);
        if (end = strstr(start, to))
        {
			if (end != start) size = end - start;
			else size = strlen(start);

            target = (char*)calloc(size + 1, sizeof(char));
            memcpy(target, start, size);
            target[size] = '\0';
        }
    }

	free(sCopy);

	return target;
}

// Convert time string dd:hh:mm:ss (or d:h:m:s) to seconds
int timeToSeconds(const char *sTime)
{
	char tok[2] = {'0'};						// found token
	int ti = 1;									// token index
	char d = ':';								// time delimiter
	int si = 0;									// string index
	int s = 0;									// calculated seconds
	int t = 1;									// calculated time

	for (si = strlen(sTime) - 1; si >= 0; si--)	// start from seconds
	{
		if (sTime[si] != d) {
    		tok[ti--] = sTime[si];				// save as reversed value
			if (si) continue;
		}

    	s += atoi(tok) * t;						// calculate seconds
    
    	if (t == 3600) t *= 24;					// days
    	else t *= 60;							// minutes, hours
    
		// reset token and its index
		memset(tok, '0', sizeof(tok));
        ti = 1;
	}

	return s;
}

// Convert time string hh:mm:ss (or h:m:s) to seconds
int timeToSecondsSscanf(const char *sTime)
{
	int hh = 0, mm = 0, ss = 0;
	int s = 0;

	if (sscanf(sTime, "%d:%d:%d", &hh, &mm, &ss)) {
		s = ss + (mm * 60) + (hh * 3600);
	}

	return s;
}

// Convert char to wchar_t
void char2wchar(const char *s, wchar_t *sw)
{
	MultiByteToWideChar(0, 0, s, strlen(s), sw, 1024*10);
}

// Convert wchar_t to char
void wchar2char(const wchar_t *sw, char *s)
{
	BOOL fUsedDefaultChar = FALSE;
	WideCharToMultiByte(CP_ACP, 0, sw, -1, s, 1024*10, NULL, &fUsedDefaultChar);
}

// Convert char to wchar_t or if already wchar_t then return it
wchar_t *str2wstr(const LPVOID lpvStr)
{
	wchar_t *wStr;
	int wchar_size = 0;
	int lpiResult = IS_TEXT_UNICODE_NULL_BYTES | IS_TEXT_UNICODE_UNICODE_MASK | IS_TEXT_UNICODE_SIGNATURE;

	if (IsTextUnicode((const LPVOID)lpvStr, wcslen((wchar_t*)lpvStr), &lpiResult)) {
		wStr = (wchar_t*)lpvStr;
	} else {
        wchar_size = MultiByteToWideChar(CP_UTF8, 0, (char*)lpvStr, -1, NULL, 0);
        wStr = (wchar_t*)calloc(wchar_size + 1, sizeof(wchar_t));
    
        MultiByteToWideChar(CP_UTF8, 0, (char*)lpvStr, -1, wStr, wchar_size);
	}

	return wStr;
}

// Function to replace a string with another string
// https://www.geeksforgeeks.org/c-program-replace-word-text-another-given-word/
char* replaceWord(const char* s, const char* oldW, const char* newW)
{
    char* result;
    int i, cnt = 0;
    int newWlen = strlen(newW);
    int oldWlen = strlen(oldW);
  
    // Counting the number of times old word
    // occur in the string
    for (i = 0; s[i] != '\0'; i++) {
        if (strstr(&s[i], oldW) == &s[i]) {
            cnt++;
  
            // Jumping to index after the old word.
            i += oldWlen - 1;
        }
    }
  
    // Making new string of enough length
    result = (char*)malloc(i + cnt * (newWlen - oldWlen) + 1);
  
    i = 0;
    while (*s) {
        // compare the substring with the result
        if (strstr(s, oldW) == s) {
            strcpy(&result[i], newW);
            i += newWlen;
            s += oldWlen;
        }
        else
            result[i++] = *s++;
    }
  
    result[i] = '\0';
    return result;
}

// Get Window Title
TCHAR *getWindowTitle(HWND h)
{
	int titleLen = GetWindowTextLength(h) + 1;

	TCHAR *title = (TCHAR*)malloc(titleLen * sizeof(TCHAR));
	GetWindowText(h, title, titleLen * sizeof(TCHAR));

	return title;
}

// print in Edit
int printft(char *fmt, ...)
{
	int n;
	char *buf;
	va_list vargs;
	HWND hEdit;
	DWORD TextSize;

	va_start(vargs, fmt);
	n = _vsnprintf(0, 0, fmt, vargs);
	buf = (char*)calloc(n + 1, sizeof(char));
	va_end(vargs);

	va_start(vargs, fmt);
	n = _vsnprintf(buf, n + 1, fmt, vargs);

	hEdit = GetDlgItem(hWin, ID_PRINT_TEXT);

	// Move caret to end of text, no selection
	TextSize = GetWindowTextLengthA(hEdit);

	SendMessageA(hEdit, EM_SETSEL, TextSize, TextSize);

	// Append the new data
	SendMessageA(hEdit, EM_REPLACESEL, FALSE, (LPARAM)buf);

	// Be sure it's scrolled into view
	SendMessageA(hEdit, EM_SCROLLCARET, 0, 0);

	// remove selection
	PostMessageA(hEdit, EM_SETSEL, 0, 0);

	va_end(vargs);
	
	free(buf);
	
	return n;
}


// print in Edit
int wprintft(wchar_t *fmt, ...)
{
	int n;
	wchar_t *buf;
	va_list vargs;
	HWND hEdit;
	DWORD TextSize;

	va_start(vargs, fmt);
	n = vswprintf(0, 0, fmt, vargs);
	buf = (wchar_t*)calloc(n + 1, sizeof(wchar_t));
	va_end(vargs);

	va_start(vargs, fmt);
	n = vswprintf(buf, n + 1, fmt, vargs);

	hEdit = GetDlgItem(hWin, ID_PRINT_TEXT);

	// Move caret to end of text, no selection
	TextSize = GetWindowTextLength(hEdit);

	SendMessage(hEdit, EM_SETSEL, TextSize, TextSize);

	// Append the new data
	SendMessage(hEdit, EM_REPLACESEL, FALSE, (LPARAM)buf);

	// Be sure it's scrolled into view
	SendMessage(hEdit, EM_SCROLLCARET, 0, 0);

	// remove selection
	PostMessage(hEdit, EM_SETSEL, 0, 0);

	va_end(vargs);
	
	free(buf);

	return n;
}

// print in Edit
/*int tprintft(TCHAR *fmt, ...)
{
	int n;
	TCHAR *buf;
	va_list vargs;
	HWND hEdit;
	DWORD TextSize;

	va_start(vargs, fmt);
	n = _vstprintf(0, 0, fmt, vargs);
	buf = (TCHAR*)calloc(n + 1, sizeof(TCHAR));
	va_end(vargs);

	va_start(vargs, fmt);
	n = _vstprintf(buf, n + 1, fmt, vargs);

	hEdit = GetDlgItem(hWin, ID_PRINT_TEXT);

	// Move caret to end of text, no selection
	TextSize = GetWindowTextLength(hEdit);

	SendMessage(hEdit, EM_SETSEL, TextSize, TextSize);

	// Append the new data
	SendMessage(hEdit, EM_REPLACESEL, FALSE, (LPARAM)buf);

	// Be sure it's scrolled into view
	SendMessage(hEdit, EM_SCROLLCARET, 0, 0);

	// remove selection
	PostMessage(hEdit, EM_SETSEL, 0, 0);

	va_end(vargs);
	
	free(buf);

	return n;
}*/

char *getTagValue(char *in, const wchar_t *find)
{
	char cbuf[100] = {0};
	char *otag, *ctag;
	char *d = "?";

	wchar2char(find, cbuf);

	otag = strtok(cbuf, d);
	ctag = strtok(NULL, d);

	return getBetween(in, otag, ctag);

	// free out after call
}
