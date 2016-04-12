/*
 * Touhou Vorbis Compressor
 * Shared patch library
 *
 * ----
 *
 * DLL entry point.
 * (DLL hooking adopted from jisakujien's "noflashmyon" source.)
 */

// And yes, this _has_ to be included in every project.
// Visual C++ won't use it when imported from a library
// and just defaults to msvcrt's one in this case.

#include <windows.h>

// Implemented externally
int InitInstance(HMODULE hProc);
void ExitInstance();

BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	HMODULE hProc = GetModuleHandle(NULL);
	// to avoid compiler lvl4 warnings 
    LPVOID lpDummy = lpReserved;
    lpDummy = NULL;
    
    switch (ul_reason_for_call)
	{
	    case DLL_PROCESS_ATTACH:	if(InitInstance(hProc))	ExitThread(0);
									break;
	    case DLL_PROCESS_DETACH:	ExitInstance();			break;
        
		case DLL_THREAD_ATTACH:  break;
	    case DLL_THREAD_DETACH:  break;
	}
    return TRUE;
}
