/*
 * Touhou Vorbis Compressor
 * TH075 implementation
 *
 * ----
 *
 * D3D8 API hook.
 * (DLL hooking adopted from jisakujien's "noflashmyon" source.)
 */

#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include "d3d8.h"
#include "patch.h"
#include "sysdll.h"

HMODULE gl_hD3D8 = NULL;

// Functions
// ---------
typedef IDirect3D8*	 (*DLL_FUNC_TYPE(d3d8, Direct3DCreate8))(UINT);

DLL_FUNC_DEF(d3d8, Direct3DCreate8);
// ---------

IDirect3D8 * WINAPI Direct3DCreate8(UINT SDKVersion)
{
	return d3d8_Direct3DCreate8(SDKVersion);
}

// Loads DLL functions
int d3d8_init(HMODULE hProc)
{
	OutputDebugString("D3D8: Init called.\r\n");
	
	// d3d8.dll functions
	if(LoadSystemDll(&gl_hD3D8, "d3d8.dll", 1))	return -1;
	DLL_GET_PROC_ADDRESS(gl_hD3D8, d3d8, Direct3DCreate8);
	
	/*// th075 performance hack
	// Seems to be useless though...
	PatchDWORD((DWORD*)0x407c65, 0x100000, 0x56220);
	PatchDWORD((DWORD*)0x407f6a,  0x20000, 0xAC44);*/
	return 0;
}

// Unloads DLL handles
void d3d8_exit() 
{    
    OutputDebugString("D3D8: Exit called.\r\n");

	// Free system DLLs
	if(gl_hD3D8)	FreeLibrary(gl_hD3D8);
}
