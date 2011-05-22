// Touhou Vorbis Compressor
// Shared patch library
// ------------------------
// sysdll.c - System DLL loader
// ------------------------
// "©" Nmlgc, 2011
// DLL hooking adopted from jisakujien's "noflashmyon" source

#include <windows.h>
#include <stdio.h>

// Loads [FN] from the system directory to [TrgHandle].
// If [ConflictSafe] is specified, a copy named <sys_[FN]> is created 
// in the application directory to avoid name clashes on Win9x.
int LoadSystemDll(HMODULE* TrgHandle, char* FN, int ConflictSafe)
{
	char sysdll[MAX_PATH];
	
	if(!FN || !TrgHandle)	return -1;

	// Getting path to system dir
	GetSystemDirectory(sysdll,MAX_PATH);
	// Append dll name
	strcat(sysdll,"\\");
	strcat(sysdll, FN);

	if(ConflictSafe)
	{
		OSVERSIONINFO	WinVer;

		// To avoid the DLL name bug on Win9x, we're copying the system DLL
		// as [sys_[FN]] into our game directory.
		memset(&WinVer, 0, sizeof(OSVERSIONINFO));
		WinVer.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		GetVersionEx(&WinVer);

		if(WinVer.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
		{
			char localdll[MAX_PATH] = {"sys_"};
			strcat(localdll, FN);

			*TrgHandle = LoadLibrary(localdll);
			if(!*TrgHandle)
			{
				CopyFile(sysdll, localdll, TRUE);
				*TrgHandle = LoadLibrary(localdll);
			}
		}
		else	*TrgHandle = LoadLibrary(sysdll);
	}
	else	*TrgHandle = LoadLibrary(sysdll);
		
	if(*TrgHandle)
	{
		OutputDebugString("LoadSystemDll: ");
		OutputDebugString(FN);
		OutputDebugString("\n");
	}
	else
	{
		sprintf(sysdll, "ERROR: LoadSystemDll(%s) failed!\n", FN);
		OutputDebugString(sysdll);
	}

	return TrgHandle == 0;
}
