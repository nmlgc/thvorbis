/*
 * Touhou Vorbis Compressor
 *
 * ----
 *
 * File handling.
 * (DLL hooking adopted from jisakujien's "noflashmyon" source.)
 */

#ifndef THVORBIS_FILE_H
#define THVORBIS_FILE_H

#define CREATE_NEW          1
#define CREATE_ALWAYS       2
#define OPEN_EXISTING       3
#define OPEN_ALWAYS         4
#define TRUNCATE_EXISTING   5

HANDLE
WINAPI
CreateFileA(
    __in     LPCSTR lpFileName,
    __in     DWORD dwDesiredAccess,
    __in     DWORD dwShareMode,
    __in_opt LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    __in     DWORD dwCreationDisposition,
    __in     DWORD dwFlagsAndAttributes,
    __in_opt HANDLE hTemplateFile
    );

#ifdef THVORBIS_WINMM_H
__declspec(dllimport)
#endif
BOOL
WINAPI
ReadFile(
    __in        HANDLE hFile,
    __out_bcount_part_opt(Read, *lpRead) __out_data_source(FILE) LPVOID lpBuffer,
    __in        DWORD Read,
    __out_opt   LPDWORD lpRead,
    __inout_opt LPOVERLAPPED lpOverlapped
    );

#ifdef THVORBIS_WINMM_H
__declspec(dllimport)
#endif
DWORD
WINAPI
SetFilePointer(
    __in        HANDLE hFile,
    __in        LONG lDistanceToMove,
    __inout_opt PLONG lpDistanceToMoveHigh,
    __in        DWORD dwMoveMethod
    );

#ifdef THVORBIS_WINMM_H
__declspec(dllimport)
#endif
BOOL WINAPI CloseHandle(__in HANDLE hObject);

#endif /* THVORBIS_FILE_H */
