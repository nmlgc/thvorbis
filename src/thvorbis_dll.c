// Touhou Vorbis Compressor
// ------------------------
// thvorbis_dll.c - DLL hooking
// ------------------------
// "©" Nmlgc, 2010-2011
// DLL hooking adopted from jisakujien's "noflashmyon" source

#include <windows.h>
#include "thvorbis.h"
#include "thvorbis_generic.h"
#ifdef D3D8_HOOK
int d3d8_init(HMODULE hProc);
void d3d8_exit();
#endif

// global variables
// ================
OggVorbis_File gl_VF;

HANDLE		gl_BGMFile = INVALID_HANDLE_VALUE;
const char	gl_BGMFileSub[16] = "bgm";	// Matches thbgm, thbgm_tr, albgm and th075bgm. Hopefully these won't change.
// ================

int InitInstance(HMODULE hProc)
{
	int Ret;

	OutputDebugString("THVORBIS: Init called.\r\n");

	// Initialisation
	memset(&gl_VF, 0, sizeof(OggVorbis_File));

#ifdef D3D8_HOOK
	Ret = d3d8_init(hProc);
	if(Ret)	return Ret;
#endif

	Ret = kernel32_init(hProc);
	if(Ret)	return Ret;
	return kernel32_patch(hProc);
}

// Unloads DLL handles
void ExitInstance()
{    
    OutputDebugString("THVORBIS: Exit called.\r\n");

#ifdef D3D8_HOOK
	d3d8_exit();
#endif

	kernel32_exit();
}
