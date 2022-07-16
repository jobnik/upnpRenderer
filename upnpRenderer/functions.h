////////////////
// by jobnik.net
////////////////

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

BOOL RtlGetVersion(OSVERSIONINFOEX *os);

char* getBetween(const char *s, const char *from, const char *to);

// Convert time string dd:hh:mm:ss (or d:h:m:s) to seconds
int timeToSeconds(const char *sTime);

// Convert char to wchar_t
void char2wchar(const char *s, wchar_t *sw);

// Convert wchar_t to char
void wchar2char(const wchar_t *sw, char *s);

// Convert char to wchar_t or if already wchar_t then return it
wchar_t *str2wstr(const LPVOID lpvStr);

// Function to replace a string with another string
// https://www.geeksforgeeks.org/c-program-replace-word-text-another-given-word/
char* replaceWord(const char* s, const char* oldW, const char* newW);

// Get Title
wchar_t *getWindowTitle(HWND h);

// print in Edit
int printft(char *fmt, ...);

// print in Edit
int wprintft(wchar_t *fmt, ...);

// print in Edit
int tprintft(TCHAR *fmt, ...);

char *getTagValue(char *in, const wchar_t *find);

#endif 
