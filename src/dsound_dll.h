/*
 * Touhou Vorbis Compressor
 *
 * ----
 *
 * Declarations of DirectSound hooks and shared data.
 * (DLL hooking adopted from jisakujien's "noflashmyon" source.)
 */

// Visual C++ 6.0
// typedef unsigned long*	DWORD_PTR;

#ifndef THVORBIS_DSOUND_DLL_H
#define THVORBIS_DSOUND_DLL_H

#include <dsound.h>
#include "patch.h"

// Functions
// =========
// dsound.dll
typedef HRESULT (*DLL_FUNC_TYPE(dsound, DirectSoundCreate))(LPCGUID, LPDIRECTSOUND*, LPUNKNOWN);
typedef HRESULT (*DLL_FUNC_TYPE(dsound, DirectSoundCreate8))(LPCGUID, LPDIRECTSOUND8*, LPUNKNOWN);
typedef HRESULT (*DLL_FUNC_TYPE(dsound, DirectSoundEnumerateA))(LPDSENUMCALLBACK, LPVOID);
// =========

// Globals
extern HMODULE      gl_hDSound;

extern const DWORD	gl_BGMBufferSize;

// Function pointers
DLL_FUNC_DEC(dsound, DirectSoundCreate);
DLL_FUNC_DEC(dsound, DirectSoundCreate8);
DLL_FUNC_DEC(dsound, DirectSoundEnumerateA);

// dsound_shared.cpp
void dsound_PerfHack(DWORD& dwOffset, DWORD& dwBytes, DWORD& dwFlags);

#endif /* THVORBIS_DSOUND_DLL_H */
