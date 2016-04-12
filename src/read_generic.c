/*
 * Touhou Vorbis Compressor
 *
 * ----
 *
 * Generic decode calls.
 * (DLL hooking adopted from jisakujien's "noflashmyon" source.)
 */

#include "thvc_windows.h"
#include "thvorbis.h"
#include "thvorbis_generic.h"

int Fake_ZWAV(char* Buf, LPDWORD lpRead, OggVorbis_File* vf)
{
	vorbis_comment* vc;
	const char* ZWAV = "ZWAV";
	char* Tag;
	int c;

	vc = ov_comment(vf, -1);
	if(!vc)	return 0;

	if(!(Tag = vorbis_comment_query(vc, ZWAV, 0)))	return 0;
	if(strlen(Tag) < 2)	return 0;
	
	// Undo the replacements of the Vorbis compressor
	for(c = 0; c < 2; c++)
	{
		if(Tag[c] == -1) Tag[c] = 0;
	}

	memset(Buf, 0, 0x10);
	memcpy(Buf, ZWAV, 4);
	Buf[0x4] = 0x01;

	Buf[0x8] = Tag[0];
	Buf[0x9] = Tag[1];
	*lpRead = 0x10;

	// Continue to the next bitstream
	ov_pcm_seek_page(vf, 4);
	return 1;
}

BOOL
WINAPI
ReadFile(
    __in        HANDLE hFile,
    __out_bcount_part_opt(Read, *lpRead) __out_data_source(FILE) LPVOID lpBuffer,
    __in        DWORD Read,
    __out_opt   LPDWORD lpRead,
    __inout_opt LPOVERLAPPED lpOverlapped
    )
{
	if(hFile == gl_BGMFile)
	{
		int Link;
		long Ret;
		char* Buf = (char*)lpBuffer;

		if(Read == 0)	return 1;
		
		// TH07 wants to verify the ZWAV header, so we construct a fake one from the tag
		if(gl_VF.current_link == 0 && Read == 0x10)	return Fake_ZWAV(Buf, lpRead, &gl_VF);
		
		while(Read > 0)
		{
			Ret = ov_read(&gl_VF, Buf, Read, 0, 2, 1, &Link);
			if(Ret < 0)	return 0;

			Buf += Ret;
			Read -= Ret;
		}
		if(lpRead)	*lpRead = (Buf - (char*)lpBuffer);
		return 1;
	}
	else	return kernel32_ReadFile(hFile, lpBuffer, Read, lpRead, lpOverlapped);
}

DWORD
WINAPI
SetFilePointer(
    __in        HANDLE hFile,
    __in        LONG lDistanceToMove,
    __inout_opt PLONG lpDistanceToMoveHigh,
    __in        DWORD dwMoveMethod
    )
{
	if(hFile == gl_BGMFile)
	{
		if((lDistanceToMove == 0) && (dwMoveMethod == FILE_CURRENT))
		{
			// Tell
			return ov_pcm_tell(&gl_VF) << 2;
		}
		else
		{
			// Seek

			// Convert to samples
			lDistanceToMove >>= 2;

			switch(dwMoveMethod)
			{
			case FILE_CURRENT:	lDistanceToMove += ov_pcm_tell(&gl_VF);
								break;
			case FILE_END:		lDistanceToMove = ov_pcm_total(&gl_VF, -1) - lDistanceToMove;
								break;
			}
			ov_pcm_seek(&gl_VF, lDistanceToMove);
			return lDistanceToMove << 2;
		}
	}
	else	return kernel32_SetFilePointer(hFile, lDistanceToMove, lpDistanceToMoveHigh, dwMoveMethod);
}
