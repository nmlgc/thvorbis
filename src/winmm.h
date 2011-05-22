// Touhou Vorbis Compressor - WINMM implementation
// ------------------------
// winmm.h - Main DLL code
// ------------------------
// "©" Nmlgc, 2010-2011
// DLL hooking adopted from jisakujien's "noflashmyon" source

#ifndef THVORBIS_WINMM_H
#define THVORBIS_WINMM_H

#include <stdio.h>
#include "thvc_mmsystem.h"
#include "patch.h"

// Functions
// =========
// winmm.dll

// MMIO
typedef MMRESULT (*DLL_FUNC_TYPE(winmm, mmioAdvance))(HMMIO, LPMMIOINFO, UINT);
typedef MMRESULT (*DLL_FUNC_TYPE(winmm, mmioAscend))(HMMIO, LPMMCKINFO, UINT);
typedef MMRESULT (*DLL_FUNC_TYPE(winmm, mmioClose))(HMMIO, UINT);
typedef MMRESULT (*DLL_FUNC_TYPE(winmm, mmioDescend))(HMMIO, LPMMCKINFO, LPCMMCKINFO, UINT);
typedef MMRESULT (*DLL_FUNC_TYPE(winmm, mmioGetInfo))(HMMIO, LPMMIOINFO, UINT);
typedef HMMIO	 (*DLL_FUNC_TYPE(winmm, mmioOpenA))(LPSTR, LPMMIOINFO, DWORD);
typedef HMMIO	 (*DLL_FUNC_TYPE(winmm, mmioOpenW))(LPWSTR, LPMMIOINFO, DWORD);
typedef LONG	 (*DLL_FUNC_TYPE(winmm, mmioRead))(HMMIO, HPSTR, LONG);
typedef LONG	 (*DLL_FUNC_TYPE(winmm, mmioSeek))(HMMIO, LONG, int);
typedef MMRESULT (*DLL_FUNC_TYPE(winmm, mmioSetInfo))(HMMIO, LPCMMIOINFO, UINT);
// =========

// Globals
extern char	gl_StreamBuf[MMIO_DEFAULTBUFFER];	// global streaming buffer
extern char gl_LastFN[MAX_PATH];	// last BGM file
extern MMIOINFO gl_mmioinfo;		// custom MMIOINFO structure
extern int	gl_iChunkSeq;			// RIFF chunk faking sequence position
extern int	gl_iLock;				// decoding lock state

extern HMODULE		gl_hWinMM;

#ifdef _DEBUG
extern FILE* Log;
#endif

// Function pointers
DLL_FUNC_DEC(winmm, mmioAdvance);
DLL_FUNC_DEC(winmm, mmioAscend);
DLL_FUNC_DEC(winmm, mmioClose);
DLL_FUNC_DEC(winmm, mmioDescend);
DLL_FUNC_DEC(winmm, mmioGetInfo);
DLL_FUNC_DEC(winmm, mmioOpenA);
DLL_FUNC_DEC(winmm, mmioOpenW);
DLL_FUNC_DEC(winmm, mmioRead);
DLL_FUNC_DEC(winmm, mmioSeek);
DLL_FUNC_DEC(winmm, mmioSetInfo);

// mmio.c
#ifdef _DEBUG
void mmioDebug(LPCMMIOINFO pmi);
void mckiDebug(LPMMCKINFO pmmcki);
#endif

// Custom function definitions
MMRESULT thvc_mmioAdvance(__in_opt LPMMIOINFO pmmioinfo, __in UINT fuAdvance);
MMRESULT thvc_mmioDescend(__inout LPMMCKINFO pmmcki, __in_opt const MMCKINFO FAR* pmmckiParent, __in UINT fuDescend);
LONG thvc_mmioSeek(__in LONG lOffset, __in int iOrigin);
MMRESULT thvc_mmioSetInfo(__in LPCMMIOINFO pmmioinfo, __in UINT fuInfo);
MMRESULT thvc_mmioClose(__in UINT fuClose);

// winmm_patch.c
int winmm_patch(HMODULE hMod);
int winmm_init(HMODULE hProc);
void winmm_exit();

#endif /* THVORBIS_WINMM_H */
