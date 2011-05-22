// Touhou Vorbis Compressor
// Shared patch library
// ------------------------
// sysdll.h - System DLL loader
// ------------------------
// "©" Nmlgc, 2011
// DLL hooking adopted from jisakujien's "noflashmyon" source

#ifndef PATCHLIB_SYSDLL_H
#define PATCHLIB_SYSDLL_H

// Loads [FN] from the system directory to [TrgHandle].
// If [ConflictSafe] is specified, a copy named <sys_[FN]> is created 
// in the application directory to avoid name clashes on Win9x.
int LoadSystemDll(HMODULE* TrgHandle, char* FN, int ConflictSafe);

#endif /* PATCHLIB_SYSDLL_H */
