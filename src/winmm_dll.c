// Touhou Vorbis Compressor - WINMM implementation
// ------------------------
// winmm_dll.c - DLL hooking
// ------------------------
// "©" Nmlgc, 2011
// DLL hooking adopted from jisakujien's "noflashmyon" source

#include <windows.h>
#include "thvc_mmsystem.h"
#include "thvorbis.h"
#include "winmm.h"
#include "patch.h"

// global variables
// ================
OggVorbis_File gl_VF;

char	gl_StreamBuf[MMIO_DEFAULTBUFFER];
char	gl_LastFN[MAX_PATH] = {'\0'};
MMIOINFO gl_mmioinfo;
int		gl_iChunkSeq = 0;
int     gl_iLock = 0;

// ================

int dsound_init(HMODULE hProc);
void dsound_exit();

int InitInstance(HMODULE hProc)
{
	int Ret;

	// Initialization
	memset(&gl_VF, 0, sizeof(OggVorbis_File));

#ifdef DECODE_AT_UNLOCK
	Ret = dsound_init(hProc);
	if(Ret)	return Ret;
#endif
	
	Ret = winmm_init(hProc);
	if(Ret)	return Ret;
	return winmm_patch(hProc);
}
void ExitInstance()
{
	ov_clear(&gl_VF);

#ifdef PATCH_WINMM
	dsound_exit();
#endif
	winmm_exit();
}
