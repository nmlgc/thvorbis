/*
 * Touhou Vorbis Compressor
 * WINMM implementation
 *
 * ----
 *
 * Shared mmio hooks.
 * (DLL hooking adopted from jisakujien's "noflashmyon" source.)
 */

#include "thvc_windows.h"
#include "thvc_mmsystem.h"
#include "thvorbis_generic.h"
#include "thvorbis.h"
#include "winmm.h"
#include "fake.h"
#include "file.h"

// th06 calls this function to check if the WAV BGM is present
// So we just replace ".wav" with ".ogg" here
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
	char* Ext = MatchFileSub(lpFileName, ".wav");
	if(Ext)
	{
		int FNLen = (Ext - lpFileName);
		char NewFN[MAX_PATH] = "\0";

		strncpy(NewFN, lpFileName, FNLen);
		strncpy(NewFN + FNLen, ".ogg", 5);

		return kernel32_CreateFileA(NewFN, dwDesiredAccess, dwShareMode | FILE_SHARE_READ, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
	}
	else	return kernel32_CreateFileA(lpFileName, dwDesiredAccess, dwShareMode | FILE_SHARE_READ, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}

// vorbisfile callbacks
// Directly calling the kernel32 file handling functions here.
// ====================
size_t w32_read(void* dst, size_t elmsize, size_t count, HANDLE file)
{
	DWORD readbytes;
	ReadFile(file, dst, count, &readbytes, NULL);
	return readbytes;
}

int w32_seek(HANDLE file, ogg_int64_t bytes, int org)
{
	return SetFilePointer(file, (LONG)bytes, NULL, org) != 0xFFFFFFFF;
}

int w32_close(HANDLE file)
{
	return CloseHandle(file);
}

long w32_tell(HANDLE file)
{
	return SetFilePointer(file, 0, 0, FILE_CURRENT);
}

static ov_callbacks OV_CALLBACKS_WIN32 = 
{
  (size_t (*)(void *, size_t, size_t, void *))  w32_read,
  (int (*)(void *, ogg_int64_t, int))           w32_seek,
  (int (*)(void *))                             w32_close,
  (long (*)(void *))                            w32_tell
};
// ====================

#ifdef _DEBUG
void mmioDebug(LPCMMIOINFO pmi)
{
	if(pmi)
	{
		fprintf(Log, "dwFlags: %5x, cchBuffer: %d, pchBuffer: %x, Next: %x, EndRead: %x, EndWrite: %x, lBufOffset: %d, lDiskOffset: %d, wErrorRet: %d",
			pmi->dwFlags, pmi->cchBuffer, pmi->pchBuffer, pmi->pchNext, pmi->pchEndRead, pmi->pchEndWrite, pmi->lBufOffset, pmi->lDiskOffset, pmi->wErrorRet);
	}
	fprintf(Log, "\n");
	fflush(Log);
}

void mckiDebug(LPMMCKINFO pmmcki)
{
	char ckid[5];
	char fccType[5];

	memcpy(ckid, &pmmcki->ckid, 4);	ckid[4] = '\0';
	memcpy(fccType, &pmmcki->fccType, 4);	fccType[4] = '\0';

	if(pmmcki)
	{
		fprintf(Log, "ckid: ");
		fprintf(Log, ckid);
		fprintf(Log, ", cksize: %d, fccType: ", pmmcki->cksize);
		fprintf(Log, fccType);
		fprintf(Log, ", dwDataOffset: %d", pmmcki->dwDataOffset);
	}
	fprintf(Log, "\n");
	fflush(Log);
}
#endif

MMRESULT mmioAscend( __in HMMIO hmmio, __in LPMMCKINFO pmmcki, __in UINT fuAscend)
{
#ifdef _DEBUG
	fprintf(Log, "mmioAscend: ");
	mckiDebug(pmmcki);
#endif
	if(hmmio == (HMMIO)&gl_VF)	return MMSYSERR_NOERROR;
	else						return winmm_mmioAscend(hmmio, pmmcki, fuAscend);
}

MMRESULT mmioClose( __in HMMIO hmmio, __in UINT fuClose)
{
#ifdef _DEBUG
	fprintf(Log, "mmioClose: %d\n", fuClose);
#endif
	if(hmmio == (HMMIO)&gl_VF)
	{
		memset(&gl_StreamBuf, 0, MMIO_DEFAULTBUFFER);
		ov_raw_seek(&gl_VF, 0);
		return thvc_mmioClose(fuClose);
	}
	else	return winmm_mmioClose(hmmio, fuClose);
}

MMRESULT mmioGetInfo( __in HMMIO hmmio, __out LPMMIOINFO pmmioinfo, __in UINT fuInfo)
{
	MMRESULT Ret = 0;

	if(hmmio == (HMMIO)&gl_VF)	memcpy(pmmioinfo, &gl_mmioinfo, sizeof(MMIOINFO));
	else						Ret = winmm_mmioGetInfo(hmmio, pmmioinfo, fuInfo);
	
#ifdef _DEBUG
	fprintf(Log, "mmioGetInfo: ");
	fflush(Log);
	mmioDebug(pmmioinfo);
#endif
	return Ret;
}

HMMIO WINAPI mmioOpenA( __inout_bcount_opt(128) LPSTR pszFileName,  __inout_opt LPMMIOINFO pmmioinfo,  __in DWORD fdwOpen)
{
#ifdef _DEBUG
	fprintf(Log, "\nmmioOpen: FN: %s, dwOpenFlags: %x\n", pszFileName, fdwOpen);
#endif

	if(pszFileName)
	{
		size_t Len;
		Len = strlen(pszFileName);
		gl_iChunkSeq = 0;
		gl_mmioinfo.lDiskOffset = 0;
		if(strncmp(gl_LastFN, pszFileName, Len))
		{
			// Different file, open the new one
			char NewFN[MAX_PATH] = "\0";	// We need the old one for the comparison above
			HANDLE BGMFile = NULL;			// local, vorbisfile will own the handle

			// Safely get last dot
			for(Len; Len >= 0; Len--)	if(pszFileName[Len] == '.')	break;

			strncpy(NewFN, pszFileName, Len);
			strncat(NewFN, ".ogg", 4);
			
			ov_clear(&gl_VF);
			memset(&gl_mmioinfo, 0, sizeof(MMIOINFO));
			
			BGMFile = kernel32_CreateFileA(NewFN, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if(BGMFile != INVALID_HANDLE_VALUE)
			{
#ifdef _DEBUG
				fprintf(Log, "Success! Opening Ogg Vorbis file...\n");
#endif
				strcpy(gl_LastFN, pszFileName);

				ov_open_callbacks(BGMFile, &gl_VF, NULL, 0, OV_CALLBACKS_WIN32);
				
				if(fdwOpen & MMIO_ALLOCBUF)
				{
					gl_mmioinfo.cchBuffer = MMIO_DEFAULTBUFFER;
					gl_mmioinfo.pchBuffer = gl_StreamBuf;
				}
				gl_mmioinfo.dwFlags = fdwOpen;
				gl_mmioinfo.pchNext = gl_mmioinfo.pchBuffer;
				gl_mmioinfo.pchEndRead = 
				gl_mmioinfo.pchEndWrite = gl_mmioinfo.pchBuffer + gl_mmioinfo.cchBuffer;

				return (HMMIO)&gl_VF;
			}
		}
		// Return handle to vorbisfile if we want to load the same one again
		else return (HMMIO)&gl_VF;
	}
#ifdef _DEBUG
	fprintf(Log, "Defaulting to WINMM.mmioOpen.\n");
#endif
	return winmm_mmioOpenA(pszFileName, pmmioinfo, fdwOpen);
}

HMMIO WINAPI mmioOpenW( __inout_bcount_opt(128) LPWSTR pszFileName,  __inout_opt LPMMIOINFO pmmioinfo,  __in DWORD fdwOpen)
{
	return winmm_mmioOpenW(pszFileName, pmmioinfo, fdwOpen);
}

LONG mmioRead( __in HMMIO hmmio, __out_bcount(cch) HPSTR pch, __in LONG cch)
{
#ifdef _DEBUG
	fprintf(Log, "\nmmioRead: %d bytes -> %x\n", cch, pch);
	fflush(Log);
#endif
	// Rebuild WAVEFORMATEX structure
	if(hmmio == (HMMIO)&gl_VF)	return Fake_WAVEFORMATEX(pch, cch, &gl_VF);
	else						return winmm_mmioRead(hmmio, pch, cch);
}

// Wrappers around the game-specific calls
// --------
MMRESULT mmioAdvance( __in HMMIO hmmio, __in_opt LPMMIOINFO pmmioinfo, __in UINT fuAdvance)
{
	MMRESULT Ret;
	if(hmmio == (HMMIO)&gl_VF)	Ret = thvc_mmioAdvance(pmmioinfo, fuAdvance);
	else				Ret = winmm_mmioAdvance(hmmio, pmmioinfo, fuAdvance);
#ifdef _DEBUG
	fprintf(Log, "mmioAdvance: ");
	mmioDebug(pmmioinfo);
#endif
	return Ret;
}

MMRESULT mmioDescend( __in HMMIO hmmio, __inout LPMMCKINFO pmmcki,
    __in_opt const MMCKINFO FAR* pmmckiParent, __in UINT fuDescend)
{
	MMRESULT Ret;
	if(hmmio == (HMMIO)&gl_VF)	Ret = thvc_mmioDescend(pmmcki, pmmckiParent, fuDescend);
	else				Ret = winmm_mmioDescend(hmmio, pmmcki, pmmckiParent, fuDescend);
#ifdef _DEBUG
	fprintf(Log, "mmioDescend: Chunk sequence: %d\n", gl_iChunkSeq - 1);
	fprintf(Log, "After: ");
	mckiDebug(pmmcki);
#endif
	return Ret;
}

LONG mmioSeek( __in HMMIO hmmio, __in LONG lOffset, __in int iOrigin)
{
#ifdef _DEBUG
	fprintf(Log, "\nmmioSeek: %d from %d\n", lOffset, iOrigin);
	fflush(Log);
#endif
	if(hmmio == (HMMIO)&gl_VF)	return thvc_mmioSeek(lOffset, iOrigin);
	else				return winmm_mmioSeek(hmmio, lOffset, iOrigin);
}

MMRESULT mmioSetInfo( __in HMMIO hmmio, __in LPCMMIOINFO pmmioinfo, __in UINT fuInfo)
{
#ifdef _DEBUG
	fprintf(Log, "mmioSetInfo: ");
	mmioDebug(pmmioinfo);
#endif
	if(hmmio == (HMMIO)&gl_VF)	return thvc_mmioSetInfo(pmmioinfo, fuInfo);
	else				return winmm_mmioSetInfo(hmmio, pmmioinfo, fuInfo);
}
// --------
