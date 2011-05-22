// Touhou Vorbis Compressor
// ------------------------
// file.c - Touhou Vorbis file handling
// ------------------------
// "©" Nmlgc, 2010-2011
// DLL hooking adopted from jisakujien's "noflashmyon" source

#include "thvc_windows.h"
#include "thvorbis.h"
#include "thvorbis_generic.h"

// vorbisfile callbacks
// ====================
size_t w32_read(void* dst, size_t elmsize, size_t count, HANDLE file)
{
	DWORD readbytes;
	kernel32_ReadFile(file, dst, count, &readbytes, NULL);
	return readbytes;
}

int w32_seek(HANDLE file, ogg_int64_t bytes, int org)
{
	return kernel32_SetFilePointer(file, (LONG)bytes, NULL, org) != 0xFFFFFFFF;
}

int w32_close(HANDLE file)
{
	return kernel32_CloseHandle(file);
}

long w32_tell(HANDLE file)
{
	return kernel32_SetFilePointer(file, 0, 0, FILE_CURRENT);
}

static ov_callbacks OV_CALLBACKS_WIN32 = 
{
  (size_t (*)(void *, size_t, size_t, void *))  w32_read,
  (int (*)(void *, ogg_int64_t, int))           w32_seek,
  (int (*)(void *))                             w32_close,
  (long (*)(void *))                            w32_tell
};
// ====================

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
    )
{
	// Check if this is our BGM file
	if(MatchFileSub(lpFileName, gl_BGMFileSub))
	{
		if(gl_BGMFile == INVALID_HANDLE_VALUE)
		{
			char NewFN[MAX_PATH] = "\0";
			int FNLen;

			// Get last dot
			for(FNLen = strlen(lpFileName); FNLen > 0; FNLen--)	if(lpFileName[FNLen] == '.')	break;

			strncpy(NewFN, lpFileName, FNLen);
			strncat(NewFN, ".ogg", 4);

			gl_BGMFile = kernel32_CreateFileA(NewFN, dwDesiredAccess, dwShareMode | FILE_SHARE_READ, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
			if(gl_BGMFile != INVALID_HANDLE_VALUE)
			{
				ov_open_callbacks(gl_BGMFile, &gl_VF, NULL, 0, OV_CALLBACKS_WIN32);
			}
			// Never return an invalid handle.
			// This fixes the crash on the in-game pause menu caused by a non-existing BGM file.
			else	gl_BGMFile = 0;
		}

		return gl_BGMFile;
	}
	return kernel32_CreateFileA(lpFileName, dwDesiredAccess, dwShareMode | FILE_SHARE_READ, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}

BOOL
WINAPI
CloseHandle(
    __in HANDLE hObject
    )
{
	// Check if this is our BGM file. We're only closing it at ExitInstance
	if(hObject != gl_BGMFile)	return kernel32_CloseHandle(hObject);
	else						return 1;
}
