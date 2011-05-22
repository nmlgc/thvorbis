// Touhou Vorbis Compressor
// ------------------------
// thvc_windows.h - Alternative Windows master header _not_ defining the file handling functions
// ------------------------
// "©" Nmlgc, 2011

/*++ BUILD Version: 0001    Increment this if a change has global effects

Copyright (c) Microsoft Corporation. All rights reserved.

Module Name:


    windows.h

Abstract:

    Master include file for Windows applications.

--*/

#ifndef _WINDOWS_
#define _WINDOWS_


#include <sdkddkver.h>

#ifndef _INC_WINDOWS
#define _INC_WINDOWS

#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/*  If defined, the following flags inhibit definition
 *     of the indicated items.
 *
 *  NOGDICAPMASKS     - CC_*, LC_*, PC_*, CP_*, TC_*, RC_
 *  NOVIRTUALKEYCODES - VK_*
 *  NOWINMESSAGES     - WM_*, EM_*, LB_*, CB_*
 *  NOWINSTYLES       - WS_*, CS_*, ES_*, LBS_*, SBS_*, CBS_*
 *  NOSYSMETRICS      - SM_*
 *  NOMENUS           - MF_*
 *  NOICONS           - IDI_*
 *  NOKEYSTATES       - MK_*
 *  NOSYSCOMMANDS     - SC_*
 *  NORASTEROPS       - Binary and Tertiary raster ops
 *  NOSHOWWINDOW      - SW_*
 *  OEMRESOURCE       - OEM Resource values
 *  NOATOM            - Atom Manager routines
 *  NOCLIPBOARD       - Clipboard routines
 *  NOCOLOR           - Screen colors
 *  NOCTLMGR          - Control and Dialog routines
 *  NODRAWTEXT        - DrawText() and DT_*
 *  NOGDI             - All GDI defines and routines
 *  NOKERNEL          - All KERNEL defines and routines
 *  NOUSER            - All USER defines and routines
 *  NONLS             - All NLS defines and routines
 *  NOMB              - MB_* and MessageBox()
 *  NOMEMMGR          - GMEM_*, LMEM_*, GHND, LHND, associated routines
 *  NOMETAFILE        - typedef METAFILEPICT
 *  NOMINMAX          - Macros min(a,b) and max(a,b)
 *  NOMSG             - typedef MSG and associated routines
 *  NOOPENFILE        - OpenFile(), OemToAnsi, AnsiToOem, and OF_*
 *  NOSCROLL          - SB_* and scrolling routines
 *  NOSERVICE         - All Service Controller routines, SERVICE_ equates, etc.
 *  NOSOUND           - Sound driver routines
 *  NOTEXTMETRIC      - typedef TEXTMETRIC and associated routines
 *  NOWH              - SetWindowsHook and WH_*
 *  NOWINOFFSETS      - GWL_*, GCL_*, associated routines
 *  NOCOMM            - COMM driver routines
 *  NOKANJI           - Kanji support stuff.
 *  NOHELP            - Help engine interface.
 *  NOPROFILER        - Profiler interface.
 *  NODEFERWINDOWPOS  - DeferWindowPos routines
 *  NOMCX             - Modem Configuration Extensions
 */

#if defined(RC_INVOKED) && !defined(NOWINRES)

#include <winresrc.h>

#else

#if defined(RC_INVOKED)
/* Turn off a bunch of stuff to ensure that RC files compile OK. */
#define NOATOM
#define NOGDI
#define NOGDICAPMASKS
#define NOMETAFILE
#define NOMINMAX
#define NOMSG
#define NOOPENFILE
#define NORASTEROPS
#define NOSCROLL
#define NOSOUND
#define NOSYSMETRICS
#define NOTEXTMETRIC
#define NOWH
#define NOCOMM
#define NOKANJI
#define NOCRYPT
#define NOMCX
#endif

#if !defined(_68K_) && !defined(_MPPC_) && !defined(_X86_) && !defined(_IA64_) && !defined(_AMD64_) && defined(_M_IX86)
#define _X86_
#endif

#if !defined(_68K_) && !defined(_MPPC_) && !defined(_X86_) && !defined(_IA64_) && !defined(_AMD64_) && defined(_M_AMD64)
#define _AMD64_
#endif

#if !defined(_68K_) && !defined(_MPPC_) && !defined(_X86_) && !defined(_IA64_) && !defined(_AMD64_) && defined(_M_M68K)
#define _68K_
#endif

#if !defined(_68K_) && !defined(_MPPC_) && !defined(_X86_) && !defined(_IA64_) && !defined(_AMD64_) && defined(_M_MPPC)
#define _MPPC_
#endif

#if !defined(_68K_) && !defined(_MPPC_) && !defined(_X86_) && !defined(_M_IX86) && !defined(_AMD64_) && defined(_M_IA64)
#if !defined(_IA64_)
#define _IA64_
#endif // !_IA64_
#endif

#ifndef _MAC
#if defined(_68K_) || defined(_MPPC_)
#define _MAC
#endif
#endif

#if defined (_MSC_VER)
#if ( _MSC_VER >= 800 )
#ifndef __cplusplus
#pragma warning(disable:4116)       // TYPE_ALIGNMENT generates this - move it
                                    // outside the warning push/pop scope.
#endif
#endif
#endif

#ifndef RC_INVOKED
#if     ( _MSC_VER >= 800 )
#pragma warning(disable:4514)
#ifndef __WINDOWS_DONT_DISABLE_PRAGMA_PACK_WARNING__
#pragma warning(disable:4103)
#endif
#if _MSC_VER >= 1200
#pragma warning(push)
#endif
#pragma warning(disable:4001)
#pragma warning(disable:4201)
#pragma warning(disable:4214)
#endif
#include <excpt.h>
#include <stdarg.h>
#endif /* RC_INVOKED */

#include <windef.h>

typedef struct _SECURITY_ATTRIBUTES
{
    DWORD nLength;
    LPVOID lpSecurityDescriptor;
    BOOL bInheritHandle;
} SECURITY_ATTRIBUTES, *PSECURITY_ATTRIBUTES, *LPSECURITY_ATTRIBUTES;

typedef struct _SYSTEMTIME {
    WORD wYear;
    WORD wMonth;
    WORD wDayOfWeek;
    WORD wDay;
    WORD wHour;
    WORD wMinute;
    WORD wSecond;
    WORD wMilliseconds;
} SYSTEMTIME, *PSYSTEMTIME, *LPSYSTEMTIME;


// Ignore source annotations with Visual C++ 6.0
#ifdef VC60
#define __in
#define __in_opt
#define __in_bcount(x)
#define __out
#define __out_opt
#define __bcount_opt(x)
#define __out_bcount_part(x, y)
#define __out_ecount_part_opt(x, y)
#define __out_bcount_part_opt(x, y)
#define __out_data_source(x)
#define __inout
#define __inout_opt
#define  LONG_PTR	 LONG *
#define ULONG_PTR	ULONG *
#endif

__declspec(dllimport) DWORD WINAPI GetCurrentDirectoryA(__in DWORD nBufferLength, __out_ecount_part_opt(nBufferLength, return + 1) LPSTR lpBuffer);
__declspec(dllimport) DWORD WINAPI GetCurrentDirectoryW(__in DWORD nBufferLength, __out_ecount_part_opt(nBufferLength, return + 1) LPWSTR lpBuffer);
#ifdef UNICODE
#define GetCurrentDirectory  GetCurrentDirectoryW
#else
#define GetCurrentDirectory  GetCurrentDirectoryA
#endif // !UNICODE
__declspec(dllimport) UINT WINAPI GetSystemDirectoryA(__out_ecount_part_opt(uSize, return + 1) LPSTR lpBuffer, __in UINT uSize);
__declspec(dllimport) UINT WINAPI GetSystemDirectoryW(__out_ecount_part_opt(uSize, return + 1) LPWSTR lpBuffer, __in UINT uSize);
#ifdef UNICODE
#define GetSystemDirectory  GetSystemDirectoryW
#else
#define GetSystemDirectory  GetSystemDirectoryA
#endif // !UNICODE

__declspec(dllimport) __out_opt HMODULE WINAPI GetModuleHandleA(__in_opt LPCSTR lpModuleName);
__declspec(dllimport) __out_opt HMODULE WINAPI GetModuleHandleW(__in_opt LPCWSTR lpModuleName);
#ifdef UNICODE
#define GetModuleHandle  GetModuleHandleW
#else
#define GetModuleHandle  GetModuleHandleA
#endif // !UNICODE

__declspec(dllimport) __out_opt HMODULE WINAPI LoadLibraryA(__in LPCSTR lpLibFileName);
__declspec(dllimport) __out_opt HMODULE WINAPI LoadLibraryW(__in LPCWSTR lpLibFileName);
#ifdef UNICODE
#define LoadLibrary  LoadLibraryW
#else
#define LoadLibrary  LoadLibraryA
#endif // !UNICODE

__declspec(dllimport) BOOL WINAPI FreeLibrary (__in HMODULE hLibModule);

__declspec(dllimport) FARPROC WINAPI GetProcAddress(__in HMODULE hModule, __in LPCSTR lpProcName);

typedef DWORD (WINAPI *PTHREAD_START_ROUTINE)(
    LPVOID lpThreadParameter
    );
typedef PTHREAD_START_ROUTINE LPTHREAD_START_ROUTINE;

__declspec(dllimport) __out_opt HANDLE WINAPI CreateRemoteThread(__in HANDLE hProcess, __in_opt LPSECURITY_ATTRIBUTES lpThreadAttributes, __in SIZE_T dwStackSize, __in LPTHREAD_START_ROUTINE lpStartAddress, __in_opt LPVOID lpParameter, __in DWORD dwCreationFlags, __out_opt LPDWORD lpThreadId);
__declspec(dllimport) BOOL WINAPI GetExitCodeThread(__in HANDLE hThread, __out LPDWORD lpExitCode);

__declspec(dllimport) DWORD WINAPI WaitForSingleObject(__in HANDLE hHandle, __in DWORD dwMilliseconds);

// Virtual memory
__declspec(dllimport) __bcount_opt(dwSize) LPVOID WINAPI VirtualAlloc(__in_opt LPVOID lpAddress, __in SIZE_T dwSize, __in DWORD flAllocationType, __in DWORD flProtect);
__declspec(dllimport) BOOL WINAPI VirtualFree(__in LPVOID lpAddress,__in SIZE_T dwSize, __in DWORD dwFreeType);
__declspec(dllimport) BOOL WINAPI VirtualProtect(__in LPVOID lpAddress, __in SIZE_T dwSize, __in DWORD flNewProtect, __out PDWORD lpflOldProtect);
__declspec(dllimport) SIZE_T WINAPI VirtualQuery(__in_opt LPCVOID lpAddress, __out_bcount_part(dwLength, return) PMEMORY_BASIC_INFORMATION lpBuffer, __in SIZE_T dwLength);
__declspec(dllimport) __bcount_opt(dwSize) LPVOID WINAPI VirtualAllocEx(__in HANDLE hProcess, __in_opt LPVOID lpAddress, __in SIZE_T dwSize, __in DWORD flAllocationType, __in DWORD flProtect);
__declspec(dllimport) BOOL WINAPI VirtualFreeEx(__in HANDLE hProcess, __in LPVOID lpAddress, __in SIZE_T dwSize, __in DWORD dwFreeType);
__declspec(dllimport) BOOL WINAPI VirtualProtectEx(__in HANDLE hProcess, __in LPVOID lpAddress, __in SIZE_T dwSize, __in DWORD flNewProtect, __out PDWORD lpflOldProtect);

__declspec(dllimport) BOOL WINAPI WriteProcessMemory(__in HANDLE hProcess, __in LPVOID lpBaseAddress, __in_bcount(nSize) LPCVOID lpBuffer, __in SIZE_T nSize, __out_opt SIZE_T * lpNumberOfBytesWritten);

// Pointer verification
__declspec(dllimport) BOOL WINAPI IsBadReadPtr(__in_opt CONST VOID *lp, __in UINT_PTR ucb);
__declspec(dllimport) BOOL WINAPI IsBadWritePtr(__in_opt LPVOID lp, __in UINT_PTR ucb);
__declspec(dllimport) BOOL WINAPI IsBadHugeReadPtr(__in_opt CONST VOID *lp, __in UINT_PTR ucb);
__declspec(dllimport) BOOL WINAPI IsBadHugeWritePtr(__in_opt LPVOID lp, __in UINT_PTR ucb);
__declspec(dllimport) BOOL WINAPI IsBadCodePtr(__in_opt FARPROC lpfn);

// Thunks
/*typedef struct _IMAGE_THUNK_DATA32 {
    union {
        DWORD ForwarderString;      // PBYTE 
        DWORD Function;             // PDWORD
        DWORD Ordinal;
        DWORD AddressOfData;        // PIMAGE_IMPORT_BY_NAME
    } u1;
} IMAGE_THUNK_DATA32;
typedef IMAGE_THUNK_DATA32 * PIMAGE_THUNK_DATA32;

typedef IMAGE_THUNK_DATA32              IMAGE_THUNK_DATA;
typedef PIMAGE_THUNK_DATA32             PIMAGE_THUNK_DATA;*/

__declspec(dllimport) DECLSPEC_NORETURN VOID WINAPI ExitProcess(__in UINT uExitCode);
__declspec(dllimport) DECLSPEC_NORETURN VOID WINAPI ExitThread(__in DWORD dwExitCode);
__declspec(dllimport) VOID WINAPI OutputDebugStringA(__in_opt LPCSTR lpOutputString);
__declspec(dllimport) VOID WINAPI OutputDebugStringW(__in_opt LPCWSTR lpOutputString);
#ifdef UNICODE
#define OutputDebugString  OutputDebugStringW
#else
#define OutputDebugString  OutputDebugStringA
#endif // !UNICODE

typedef struct _OVERLAPPED {
    ULONG_PTR Internal;
    ULONG_PTR InternalHigh;
    union {
        struct {
            DWORD Offset;
            DWORD OffsetHigh;
        };

        PVOID Pointer;
    };

    HANDLE  hEvent;
} OVERLAPPED, *LPOVERLAPPED;

#define FILE_BEGIN           0
#define FILE_CURRENT         1
#define FILE_END             2

#define INVALID_HANDLE_VALUE ((HANDLE)(LONG_PTR)-1)

__declspec(dllimport) int WINAPI MessageBoxA(__in_opt HWND hWnd, __in_opt  LPCSTR lpText, __in_opt  LPCSTR lpCaption, __in UINT uType);
__declspec(dllimport) int WINAPI MessageBoxW(__in_opt HWND hWnd, __in_opt LPCWSTR lpText, __in_opt LPCWSTR lpCaption, __in UINT uType);
#ifdef UNICODE
#define MessageBox  MessageBoxW
#else
#define MessageBox  MessageBoxA
#endif // !UNICODE

/*
 * MessageBox() Flags
 */
#define MB_OK                       0x00000000L
#define MB_OKCANCEL                 0x00000001L
#define MB_ABORTRETRYIGNORE         0x00000002L
#define MB_YESNOCANCEL              0x00000003L
#define MB_YESNO                    0x00000004L
#define MB_RETRYCANCEL              0x00000005L
#if(WINVER >= 0x0500)
#define MB_CANCELTRYCONTINUE        0x00000006L
#endif /* WINVER >= 0x0500 */

// #include <winbase.h>
#include <wingdi.h>
// #include <winuser.h>
#if !defined(_MAC) || defined(_WIN32NLS)
// #include <winnls.h>
#endif
#ifndef _MAC
// #include <wincon.h>
#include <winver.h>
#endif
#if !defined(_MAC) || defined(_WIN32REG)
// #include <winreg.h>
#endif
#ifndef _MAC
#include <winnetwk.h>
#endif

// #include <stralign.h>

// Version
#ifdef VC60
typedef struct _OSVERSIONINFOA {
    DWORD dwOSVersionInfoSize;
    DWORD dwMajorVersion;
    DWORD dwMinorVersion;
    DWORD dwBuildNumber;
    DWORD dwPlatformId;
    CHAR   szCSDVersion[ 128 ];     // Maintenance string for PSS usage
} OSVERSIONINFOA, *POSVERSIONINFOA, *LPOSVERSIONINFOA;

typedef struct _OSVERSIONINFOW {
    DWORD dwOSVersionInfoSize;
    DWORD dwMajorVersion;
    DWORD dwMinorVersion;
    DWORD dwBuildNumber;
    DWORD dwPlatformId;
    WCHAR  szCSDVersion[ 128 ];     // Maintenance string for PSS usage
} OSVERSIONINFOW, *POSVERSIONINFOW, *LPOSVERSIONINFOW, RTL_OSVERSIONINFOW, *PRTL_OSVERSIONINFOW;
#ifdef UNICODE
typedef OSVERSIONINFOW OSVERSIONINFO;
typedef POSVERSIONINFOW POSVERSIONINFO;
typedef LPOSVERSIONINFOW LPOSVERSIONINFO;
#else
typedef OSVERSIONINFOA OSVERSIONINFO;
typedef POSVERSIONINFOA POSVERSIONINFO;
typedef LPOSVERSIONINFOA LPOSVERSIONINFO;
#endif // UNICODE

//
// dwPlatformId defines:
//

#define VER_PLATFORM_WIN32s			0
#define VER_PLATFORM_WIN32_WINDOWS	1
#define VER_PLATFORM_WIN32_NT		2

#endif // VC60
__declspec(dllimport) BOOL WINAPI GetVersionExA(__inout LPOSVERSIONINFOA lpVersionInformation);
__declspec(dllimport) BOOL WINAPI GetVersionExW(__inout LPOSVERSIONINFOW lpVersionInformation);
#ifdef UNICODE
#define GetVersionEx  GetVersionExW
#else
#define GetVersionEx  GetVersionExA
#endif // !UNICODE

__declspec(dllimport) DWORD WINAPI GetLastError(VOID);


#ifndef RC_INVOKED
#if     ( _MSC_VER >= 800 )
#if _MSC_VER >= 1200
#pragma warning(pop)
#else
#pragma warning(default:4001)
#pragma warning(default:4201)
#pragma warning(default:4214)
/* Leave 4514 disabled.  It's an unneeded warning anyway. */
#endif
#endif
#endif /* RC_INVOKED */

#endif /* RC_INVOKED */

#endif /* _INC_WINDOWS */
#endif /* _WINDOWS_ */


