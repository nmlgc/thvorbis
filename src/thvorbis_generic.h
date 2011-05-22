// Touhou Vorbis Compressor
// ------------------------
// thvorbis_generic.h - Definitions for the generic implementation
// ------------------------
// "©" Nmlgc, 2010-2011
// DLL hooking adopted from jisakujien's "noflashmyon" source

#ifndef THVORBIS_THVORBIS_GENERIC_H
#define THVORBIS_THVORBIS_GENERIC_H

#include "patch.h"

// Functions
// =========
// kernel32.dll
typedef HANDLE (*DLL_FUNC_TYPE(kernel32, CreateFileA))(LPCTSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE);
typedef BOOL   (*DLL_FUNC_TYPE(kernel32, ReadFile))(HANDLE, LPVOID, DWORD, LPDWORD, LPOVERLAPPED);
typedef DWORD  (*DLL_FUNC_TYPE(kernel32, SetFilePointer))(HANDLE, LONG, PLONG, DWORD);
typedef BOOL   (*DLL_FUNC_TYPE(kernel32, CloseHandle))(HANDLE);
// =========

// Globals
extern HANDLE		gl_BGMFile;
extern const char	gl_BGMFileSub[16];

// Function pointers
DLL_FUNC_DEC(kernel32, CreateFileA);
DLL_FUNC_DEC(kernel32, ReadFile);
DLL_FUNC_DEC(kernel32, SetFilePointer);
DLL_FUNC_DEC(kernel32, CloseHandle);

char* MatchFileSub(const char* str, const char* sub);

// thvorbis_patch.c
int kernel32_patch(HMODULE hMod);
int kernel32_init(HMODULE hProc);
void kernel32_exit();

#endif /* THVORBIS_THVORBIS_GENERIC_H */
