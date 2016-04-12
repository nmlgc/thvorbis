/*
 * Touhou Vorbis Compressor
 * WINMM implementation
 *
 * ----
 *
 * Win32 API hook registration.
 * (DLL hooking adopted from jisakujien's "noflashmyon" source.)
 */

#include "thvc_windows.h"
#include "thvc_mmsystem.h"
#include "file.h"
#include "patch.h"
#include "sysdll.h"
#include "thvorbis.h"
#include "thvorbis_generic.h"
#include "winmm.h"

// global variables
// ================
HMODULE		gl_hWinMM = NULL;

#ifdef _DEBUG
FILE* Log;
#endif
// Functions
// ---------
DLL_FUNC_DEF(winmm, mmioAdvance);
DLL_FUNC_DEF(winmm, mmioAscend);
DLL_FUNC_DEF(winmm, mmioClose);
DLL_FUNC_DEF(winmm, mmioDescend);
DLL_FUNC_DEF(winmm, mmioGetInfo);
DLL_FUNC_DEF(winmm, mmioOpenA);
DLL_FUNC_DEF(winmm, mmioOpenW);
DLL_FUNC_DEF(winmm, mmioRead);
DLL_FUNC_DEF(winmm, mmioSeek);
DLL_FUNC_DEF(winmm, mmioSetInfo);

DLL_FUNC_DEF(kernel32, CreateFileA);
// ---------
// ================

int winmm_patch(HMODULE hMod)
{
	int Ret;
	func_patch_t patch[9];

	SetFuncPatch(&patch[0], "mmioAdvance", DLL_FUNC(winmm, mmioAdvance), mmioAdvance);
	SetFuncPatch(&patch[1], "mmioAscend", DLL_FUNC(winmm, mmioAscend), mmioAscend);
	SetFuncPatch(&patch[2], "mmioClose", DLL_FUNC(winmm, mmioClose), mmioClose);
	SetFuncPatch(&patch[3], "mmioDescend", DLL_FUNC(winmm, mmioDescend), mmioDescend);
	SetFuncPatch(&patch[4], "mmioGetInfo", DLL_FUNC(winmm, mmioGetInfo), mmioGetInfo);
	SetFuncPatch(&patch[5], "mmioOpenA", DLL_FUNC(winmm, mmioOpenA), mmioOpenA);
	SetFuncPatch(&patch[6], "mmioRead", DLL_FUNC(winmm, mmioRead), mmioRead);
	SetFuncPatch(&patch[7], "mmioSeek", DLL_FUNC(winmm, mmioSeek), mmioSeek);
	SetFuncPatch(&patch[8], "mmioSetInfo", DLL_FUNC(winmm, mmioSetInfo), mmioSetInfo);

	Ret = PatchDllFuncs(hMod, "winmm.dll", patch, 9);
	if(Ret)	return Ret;

	SetFuncPatch(&patch[0], "CreateFileA", DLL_FUNC(kernel32, CreateFileA), CreateFileA);
	return PatchDllFuncs(hMod, "kernel32.dll", patch, 1);
}

// Loads DLL functions
int winmm_init(HMODULE hProc)
{
	HMODULE	hKernel32;

	OutputDebugString("THVORBIS_WINMM: Init called.\r\n");
	
	if(LoadSystemDll(&gl_hWinMM, "winmm.dll", 0))	return -1;
	
	// winmm.dll functions
	DLL_GET_PROC_ADDRESS(gl_hWinMM, winmm, mmioAdvance);
	DLL_GET_PROC_ADDRESS(gl_hWinMM, winmm, mmioAscend);
	DLL_GET_PROC_ADDRESS(gl_hWinMM, winmm, mmioClose);
	DLL_GET_PROC_ADDRESS(gl_hWinMM, winmm, mmioDescend);
	DLL_GET_PROC_ADDRESS(gl_hWinMM, winmm, mmioGetInfo);
	DLL_GET_PROC_ADDRESS(gl_hWinMM, winmm, mmioOpenA);
	DLL_GET_PROC_ADDRESS(gl_hWinMM, winmm, mmioOpenW);
	DLL_GET_PROC_ADDRESS(gl_hWinMM, winmm, mmioRead);
	DLL_GET_PROC_ADDRESS(gl_hWinMM, winmm, mmioSeek);
	DLL_GET_PROC_ADDRESS(gl_hWinMM, winmm, mmioSetInfo);

	hKernel32 = GetModuleHandle("kernel32.dll");
	// kernel32.dll functions
	DLL_GET_PROC_ADDRESS(hKernel32, kernel32, CreateFileA);

#ifdef _DEBUG
	Log = fopen("thvorbis_winmm_debug.txt", "wt");
#endif
	return 0;
}

// Unloads DLL handles
void winmm_exit() 
{    
    OutputDebugString("THVORBIS_WINMM: Exit called.\r\n");

	// Free system DLLs
	if(gl_hWinMM)	FreeLibrary(gl_hWinMM);
#ifdef _DEBUG
	fclose(Log);
#endif
}
