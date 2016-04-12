/*
 * Touhou Vorbis Compressor
 *
 * ----
 *
 * DirectSound API hooks.
 * (DLL hooking adopted from jisakujien's "noflashmyon" source.)
 */

#include <windows.h>
extern "C"
{
	#include "sysdll.h"
}
#include "thvc_mmsystem.h"
#include "dsound_dll.h"
#ifndef NO_DSOUND_HOOK
#include "thvc_IDirectSound8.h"
#endif

// global variables
// ================
HMODULE      gl_hDSound = NULL;

#ifdef DECODE_AT_UNLOCK
const DWORD	gl_BGMBufferSize = 0x56220;
#else
const DWORD	gl_BGMBufferSize = 0xAC440;
#endif
// Functions
// ---------
DLL_FUNC_DEF(dsound, DirectSoundCreate);
DLL_FUNC_DEF(dsound, DirectSoundCreate8);
DLL_FUNC_DEF(dsound, DirectSoundEnumerateA);
// ---------
// ================

HRESULT DirectSoundCreate(LPCGUID lpcGuidDevice, LPDIRECTSOUND* ppDS, LPUNKNOWN pUnkOuter)
{
	return dsound_DirectSoundCreate(lpcGuidDevice, ppDS, pUnkOuter);
}
HRESULT DirectSoundEnumerateA(LPDSENUMCALLBACK lpDSEnumCallback, LPVOID lpContext)
{
	return dsound_DirectSoundEnumerateA(lpDSEnumCallback, lpContext);
}

#ifdef THVC_IDIRECTSOUND8_H
thvc_IDirectSound8*	gl_IDirectSound8 = NULL;

HRESULT DirectSoundCreate8(LPCGUID lpcGuidDevice, LPDIRECTSOUND8* ppDS8, LPUNKNOWN pUnkOuter)
{
	// Get pointer to original object
	HRESULT Ret = dsound_DirectSoundCreate8(lpcGuidDevice, ppDS8, pUnkOuter);
	if(Ret != DS_OK)	return Ret;

	// Create our object
	gl_IDirectSound8 = new thvc_IDirectSound8(*ppDS8);
	*ppDS8 = gl_IDirectSound8;

	return Ret;
}
#else
HRESULT DirectSoundCreate8(LPCGUID lpcGuidDevice, LPDIRECTSOUND8* ppDS8, LPUNKNOWN pUnkOuter)
{
	return dsound_DirectSoundCreate8(lpcGuidDevice, ppDS8, pUnkOuter);
}
#endif

extern "C"
{
	int dsound_init(HANDLE hModule) 
	{
		OutputDebugString("DSOUND: Init called.\r\n");

		// Initialisation
		if(LoadSystemDll(&gl_hDSound, "dsound.dll", 1))	return -1;

		// dsound.dll functions
		DLL_GET_PROC_ADDRESS(gl_hDSound, dsound, DirectSoundCreate);
		DLL_GET_PROC_ADDRESS(gl_hDSound, dsound, DirectSoundCreate8);
		DLL_GET_PROC_ADDRESS(gl_hDSound, dsound, DirectSoundEnumerateA);

		return 0;
	}

	void dsound_exit() 
	{
		OutputDebugString("DSOUND: Exit called.\r\n");

		// Release system dlls
		if(gl_hDSound)	FreeLibrary(gl_hDSound);
	}
}

#ifdef DSOUND_DLL
int InitInstance(HMODULE hProc)	{return dsound_init(hProc);}
void ExitInstance()            	{dsound_exit();}
#endif
