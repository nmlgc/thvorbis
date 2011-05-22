// Touhou Vorbis Compressor - No Music patch
// ------------------------
// nomusic_dll.c - DLL hooking
// ------------------------
// "©" Nmlgc, 2011
// DLL hooking adopted from jisakujien's "noflashmyon" source

#include "thvc_windows.h"
#include "thvorbis_generic.h"
#include "patch.h"

// Functions
// =========
// kernel32.dll
typedef HANDLE (*DLL_FUNC_TYPE(kernel32, CreateFileA))(LPCTSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE);
// =========

// Globals
const char	gl_BGMFileSub[16] = "bgm";

// Function pointers
DLL_FUNC_DEF(kernel32, CreateFileA);

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
		HANDLE Ret;

		Ret = kernel32_CreateFileA(lpFileName, dwDesiredAccess, dwShareMode | FILE_SHARE_READ, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);

		// Never return an invalid handle.
		// This fixes the crash on the in-game pause menu caused by a non-existing BGM file.
		if(Ret == INVALID_HANDLE_VALUE)	Ret = 0;
		
		return Ret;
	}
	else	return kernel32_CreateFileA(lpFileName, dwDesiredAccess, dwShareMode | FILE_SHARE_READ, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}

int nomusic_patch(HMODULE hMod)
{
	func_patch_t patch;
	SetFuncPatch(&patch, "CreateFileA", DLL_FUNC(kernel32, CreateFileA), CreateFileA);
	return PatchDllFuncs(hMod, "kernel32.dll", &patch, 1);
}

int dsound_init(HANDLE hModule);
void dsound_exit();

int InitInstance(HMODULE hProc)
{
	HMODULE	hKernel32 = NULL;
	int Ret;
	OutputDebugString("NOMUSIC: Init called.\r\n");
	
	// Initialisation
	hKernel32 = GetModuleHandle("kernel32.dll");
	
	// kernel32.dll functions
	DLL_GET_PROC_ADDRESS(hKernel32, kernel32, CreateFileA);
	
	Ret = nomusic_patch(hProc);
	if(Ret)	return Ret;
	return dsound_init(hProc);
}

void ExitInstance()
{
	OutputDebugString("NOMUSIC: Exit called.\r\n");
	dsound_exit();
}
