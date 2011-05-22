// Touhou Vorbis Compressor
// ------------------------
// thvorbis_patch.c - DLL hooking
// ------------------------
// "©" Nmlgc, 2010-2011
// DLL hooking adopted from jisakujien's "noflashmyon" source

#include "thvc_windows.h"
#include "file.h"
#include "patch.h"
#include "thvorbis.h"
#include "thvorbis_generic.h"

// Functions
// ---------
DLL_FUNC_DEF(kernel32, CreateFileA);
DLL_FUNC_DEF(kernel32, ReadFile);
DLL_FUNC_DEF(kernel32, SetFilePointer);
DLL_FUNC_DEF(kernel32, CloseHandle);
// ---------

// ================

int kernel32_patch(HMODULE hMod)
{
	func_patch_t patch[4];

	SetFuncPatch(&patch[0], "CreateFileA", DLL_FUNC(kernel32, CreateFileA), CreateFileA);
	SetFuncPatch(&patch[1], "ReadFile", DLL_FUNC(kernel32, ReadFile), ReadFile);
	SetFuncPatch(&patch[2], "SetFilePointer", DLL_FUNC(kernel32, SetFilePointer), SetFilePointer);
	SetFuncPatch(&patch[3], "CloseHandle", DLL_FUNC(kernel32, CloseHandle), CloseHandle);

	return PatchDllFuncs(hMod, "kernel32.dll", patch, 4);
}

// Loads DLL functions
int kernel32_init(HMODULE hProc)
{
	HMODULE	hKernel32 = GetModuleHandle("kernel32.dll");
	
	// kernel32.dll functions
	DLL_GET_PROC_ADDRESS(hKernel32, kernel32, CreateFileA);
	DLL_GET_PROC_ADDRESS(hKernel32, kernel32, ReadFile);
	DLL_GET_PROC_ADDRESS(hKernel32, kernel32, SetFilePointer);
	DLL_GET_PROC_ADDRESS(hKernel32, kernel32, CloseHandle);

	return 0;
}

void kernel32_exit()
{
}
