/*
 * Touhou Vorbis Compressor
 * Shared patch library
 *
 * ----
 *
 * Import Address Table patching.
 */

#include <windows.h>
#include "patch.h"

// Writes [Val] to [ptr] in the address space of the current process,
// if the current value in [ptr] equals [Prev]
int PatchDWORD(DWORD* ptr, DWORD Prev, DWORD Val)
{
	MEMORY_BASIC_INFORMATION mbi;
	DWORD oldProt;
	VirtualQuery(ptr, &mbi, sizeof(MEMORY_BASIC_INFORMATION));
	VirtualProtect(mbi.BaseAddress, mbi.RegionSize, PAGE_READWRITE, &oldProt);
	if((*ptr) == Prev)	*ptr = Val;
	VirtualProtect(mbi.BaseAddress, mbi.RegionSize, oldProt, &oldProt);
	return 1;
}

// Low-level patching
// ------------------
// Replaces the function pointer of [pThunk] with [pNewFunc]
int PatchFunc(PIMAGE_THUNK_DATA pThunk, void* pNewFunc)
{
	MEMORY_BASIC_INFORMATION mbi;
	DWORD oldProt;
	VirtualQuery(&pThunk->u1.Function, &mbi, sizeof(MEMORY_BASIC_INFORMATION));
	VirtualProtect(mbi.BaseAddress, mbi.RegionSize, PAGE_READWRITE, &oldProt);
	pThunk->u1.Function = (DWORD)pNewFunc;
	VirtualProtect(mbi.BaseAddress, mbi.RegionSize, oldProt, &oldProt);
	return 1;
}

// Searches for [cOrigFuncName] starting from [pOrigFirstThunk].
// Only used on Win9x, where patching by pointer doesn't seem to work
int PatchFuncByName(HMODULE hMod, PIMAGE_THUNK_DATA pOrigFirstThunk, PIMAGE_THUNK_DATA pImpFirstThunk, char* cOrigFuncName, void* pNewFunc)
{
	PIMAGE_THUNK_DATA pOT, pIT;

	// Verify that the newFunc is valid
	if(!pNewFunc || IsBadCodePtr((FARPROC)pNewFunc))	return 0;

	for(pOT = pOrigFirstThunk, pIT = pImpFirstThunk; pOT->u1.Function; pOT++, pIT++)
	{
		if((pOT->u1.Ordinal & IMAGE_ORDINAL_FLAG) != IMAGE_ORDINAL_FLAG)
        {
			PIMAGE_IMPORT_BY_NAME pByName =(PIMAGE_IMPORT_BY_NAME)((DWORD)hMod+(DWORD)(pOT->u1.AddressOfData));
            if(pByName->Name[0] == '\0')
            {
                return 0;
            }
            
            if(_stricmp(cOrigFuncName, (char*)pByName->Name) == 0)
            {
				return PatchFunc(pIT, pNewFunc);
			}
		}
	}
	// Function not found
	return 0;
}

// Searches for [pOrigFunc] starting from [pImpFirstThunk], and replaces it with [pNewFunc] if found
int PatchFuncByPtr(PIMAGE_THUNK_DATA pImpFirstThunk, void* pOrigFunc, void* pNewFunc)
{
	PIMAGE_THUNK_DATA Thunk;

	// Verify that the newFunc is valid
	if(!pNewFunc || IsBadCodePtr((FARPROC)pNewFunc))	return 0;

	for(Thunk = pImpFirstThunk; Thunk->u1.Function; Thunk++)
	{
		if((DWORD*)Thunk->u1.Function == (DWORD*)pOrigFunc)	
		{
			return PatchFunc(Thunk, pNewFunc);
		}
	}
	// Function not found
	return 0;
}

// Returns the import descriptor of [DLLName]
// Adapted from http://forum.sysinternals.com/createprocess-api-hook_topic13138.html
PIMAGE_IMPORT_DESCRIPTOR WINAPI GetDLLImportDesc(HMODULE hMod, const char* DLLName)
{
	PIMAGE_DOS_HEADER pDosH;
	PIMAGE_NT_HEADERS pNTH;
	PIMAGE_IMPORT_DESCRIPTOR pImportDesc;

	if(!hMod || !DLLName)	return 0;

	// Get DOS Header
	pDosH = (PIMAGE_DOS_HEADER) hMod;

	// Verify that the PE is valid by checking e_magic's value and DOS Header size
	if(IsBadReadPtr(pDosH, sizeof(IMAGE_DOS_HEADER)))	return 0;

	if(pDosH->e_magic != IMAGE_DOS_SIGNATURE)	return 0;

	// Find the NT Header by using the offset of e_lfanew value from hMod
	pNTH = (PIMAGE_NT_HEADERS) ((DWORD) pDosH + (DWORD) pDosH->e_lfanew);

	// Verify that the NT Header is correct
	if(IsBadReadPtr(pNTH, sizeof(IMAGE_NT_HEADERS)))	return 0;

	if(pNTH->Signature != IMAGE_NT_SIGNATURE)	return 0;

	// iat patching
	pImportDesc = (PIMAGE_IMPORT_DESCRIPTOR) ((DWORD) pDosH + 
		(DWORD) (pNTH->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress));

	if(pImportDesc == (PIMAGE_IMPORT_DESCRIPTOR) pNTH)   return 0;

	while(pImportDesc->Name)
	{
		// pImportDesc->Name gives the name of the module
		char *name = (char *) ((DWORD) pDosH + (DWORD) (pImportDesc->Name));
		// stricmp returns 0 if strings are equal, case insensitive
		if(_stricmp(name, DLLName) == 0)
		{
			return pImportDesc;
		}
		++pImportDesc;
	}
	return NULL;
}
// ------------------

// High-level patching
// -------------------
// Convenience function to set a single func_patch_t entry
void SetFuncPatch(func_patch_t* patch, char* old_func, void* old_ptr, void* new_ptr)
{
	patch->old_func = old_func;
	patch->old_ptr = old_ptr;
	patch->new_ptr = new_ptr;
}

// Patches [FuncPatchCount] functions in the [FuncPatch] array
int PatchDllFuncs(HMODULE hMod, char* DllName, func_patch_t* FuncPatch, UINT FuncPatchCount)
{
	PIMAGE_IMPORT_DESCRIPTOR	ImpDesc;
	PIMAGE_THUNK_DATA	pOrigThunk;
	PIMAGE_THUNK_DATA	pImpThunk;
	OSVERSIONINFO	WinVer;
	int Ret = FuncPatchCount;
	UINT c;

	ImpDesc = GetDLLImportDesc(hMod, DllName);
	if(!ImpDesc)	return -1;

	pOrigThunk = (PIMAGE_THUNK_DATA)((DWORD)hMod + (DWORD)ImpDesc->OriginalFirstThunk);
	pImpThunk  = (PIMAGE_THUNK_DATA)((DWORD)hMod + (DWORD)ImpDesc->FirstThunk);

	// Patchan method is dependent of the Windows version we're running on
	// Windows 98 doesn't support patchan by pointer
	memset(&WinVer, 0, sizeof(OSVERSIONINFO));
	WinVer.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&WinVer);

	for(c = 0; c < FuncPatchCount; c++)
	{
		if(WinVer.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS && WinVer.dwMinorVersion <= 10)
		{
			Ret -= PatchFuncByName(hMod, pOrigThunk, pImpThunk, FuncPatch[c].old_func, FuncPatch[c].new_ptr);
		}
		else
		{
			Ret -= PatchFuncByPtr(pImpThunk, FuncPatch[c].old_ptr, FuncPatch[c].new_ptr);
		}
	}
	return Ret;
}
// -------------------
