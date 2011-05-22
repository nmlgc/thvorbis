// Touhou Vorbis Compressor
// ------------------------
// read_th075.c - TH075 decode calls
// ------------------------
// "©" Nmlgc, 2010-2011
// DLL hooking adopted from jisakujien's "noflashmyon" source

#include "thvc_windows.h"
#include <stdlib.h>
#include "thvorbis.h"
#include "thvorbis_generic.h"
#include "fake.h"

unsigned long gl_VPos = 0;	// Virtual File Position

// Chunks
// ------
// http://www.sonicspot.com/guide/wavefiles.html has an overview on those

typedef struct
{
	char ID[4];	// "cue "
	unsigned int ChunkSize;	// chunk data size, defined as 4 + ([Points] * sizeof(RIFF_cue_point))
	unsigned int Points;	// number of cue points
} RIFF_cue;

typedef struct
{
	unsigned int Num;		// unique identification value
	unsigned int Position;	// play order position
	char DataChunkID[4];	// RIFF ID of corresponding data chunk
	unsigned int ListChunkOffset;	// offset from [DataChunkID] to the LIST chunk, or 0 if no such chunk is present
	unsigned int BlockStart;	// offset from [DataChunkID] to a (unspecified) "block" containing the sample
	unsigned int SampleOffset;	// offset from [BlockStart] to the sample
} RIFF_cue_point;

typedef struct
{
	char ID[4];	// "LIST"
	unsigned int ChunkSize;
	char adtl[4];	// "adtl"
} RIFF_LIST;

typedef struct
{
	char ID[4];	// "ltxt"
	unsigned int ChunkSize;
	unsigned int CueNum;	// ID of the cue this text chunk pertains to
	unsigned int SampleLength;
} RIFF_ltxt;
// ------

ogg_int64_t SubtrackPCMTotal(OggVorbis_File* vf, int Link)
{
	char *CurFN, *NextFN;
	ogg_int64_t Ret = ov_pcm_total(vf, Link);
	vorbis_comment* vc = ov_comment(vf, Link);
	if(vc)
	{
		CurFN = vorbis_comment_query(vc, "SOURCE", 0);
		Link++;
		vc = ov_comment(&gl_VF, Link);
		NextFN = vorbis_comment_query(vc, "SOURCE", 0);
		if(!strcmp(CurFN, NextFN))
		{
			Ret += ov_pcm_total(&gl_VF, Link);
		}
	}
	return Ret;
}

// Encryption
void Tasofro_CryptHeaderV1(char* hdr, const unsigned int hdrSize)
{
	unsigned short k = 0x64, l = 0x64;
	unsigned short c;

	for(c = 0; c < hdrSize; ++c)
	{
		hdr[c] ^= k;
		k += l; l += 0x4D;
	}
}

// Archive faking
// Called only directly after CreateFile
// --------------
int Fake_Count(char* Buf, DWORD Read, OggVorbis_File* vf)
{
	vorbis_comment* vc;
	char* p;
	long l;

	vc = ov_comment(vf, -1);
	if(!vc)	return 0;

	l = strtol(vorbis_comment_query(vc, "TOTALTRACKS", 0), &p, 10);

	memcpy(Buf, &l, Read);
	return Read;
}

int Fake_Table(char* Buf, DWORD Read, OggVorbis_File* vf)
{
	// Walk through the links and build the Tasofro header
	char* LastFN = NULL;
	int Link;
	unsigned int Offset = 0;
	unsigned int r = 0;
	
	for(Link = 0; (Link < vf->links) && ( (r + 0x6C) <= Read); Link++)
	{
		vorbis_comment* vc = ov_comment(vf, Link);
		char* FN = vorbis_comment_query(vc, "SOURCE", 0);
		unsigned int Size = ov_pcm_total(vf, Link) << 2;

		if(!LastFN || strcmp(FN, LastFN))
		{
			if(LastFN)	r += 0x6C;
			// File name
			strcpy(Buf + r, FN);
			strcat(Buf + r, ".wav");
			// Size
			*((unsigned int*)(Buf + r + 0x64)) = Size;
			// Offset
			*((unsigned int*)(Buf + r + 0x68)) = Offset;

			LastFN = FN;
		}
		else
		{
			// Add this bitstream to the total size of the track
			*((unsigned int*)(Buf + r + 0x64)) += Size;
		}
		Offset += Size;
	}

	Tasofro_CryptHeaderV1(Buf, r + 0x6C);
	return Read;
}
// --------------

// Fakes loop point data based from the current link
int Fake_Chunks(char* Buf, DWORD Read, OggVorbis_File* vf)
{
	vorbis_comment* vc;
	char *CurFN, *NextFN;
	ogg_int64_t LoopStart = 0, LoopLen = 0;
	int Link = vf->current_link;

	RIFF_cue* cue = (RIFF_cue*)Buf;
	RIFF_cue_point* point = (RIFF_cue_point*)(Buf += sizeof(RIFF_cue));
	RIFF_LIST* list = (RIFF_LIST*)(Buf += sizeof(RIFF_cue_point));
	RIFF_ltxt* ltxt = (RIFF_ltxt*)(Buf += sizeof(RIFF_LIST));

	// Since this is always called at the first link of a subtrack, 
	// the loop point equals the length of the current link...
	LoopStart = ov_pcm_total(vf, Link);

	// ... and the length of the next equals the looping length
	vc = ov_comment(vf, Link);
	if(vc)
	{
		CurFN = vorbis_comment_query(vc, "SOURCE", 0);
		Link++;
		vc = ov_comment(vf, Link);
		NextFN = vorbis_comment_query(vc, "SOURCE", 0);
		if(!strcmp(CurFN, NextFN))
		{
			LoopLen = ov_pcm_total(vf, Link);
		}
	}

	// This is the only place where we're writing samples,
	// since they get converted to bytes by the game

	// "cue " chunk
	memcpy(cue->ID, "cue", 4);
	cue->ChunkSize = 0x1C;
	cue->Points = 0x01;

	// cue point 1
	point->Num = 0x01;
	point->Position = LoopStart;
	memcpy(point->DataChunkID, "data", 4);
	point->ListChunkOffset = 0;
	point->BlockStart = 0;
	point->SampleOffset = LoopStart;

	// "LIST" chunk
	memcpy(list->ID, "LIST", 4);
	memcpy(list->adtl, "adtl", 4);
	list->ChunkSize = 0x14;

	// "ltxt" chunk
	memcpy(ltxt->ID, "ltxt", 4);
	ltxt->ChunkSize = 0x08;
	ltxt->CueNum = 0x0;
	ltxt->SampleLength = LoopLen;

	return Read;
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
		long Ret = -1;
		char* Buf = (char*)lpBuffer;
		
		if(Read == 0)	return 1;
		
		// Faking
		switch(gl_VPos)
		{
		case 1:	Ret = *lpRead = Fake_Table(Buf, Read, &gl_VF);
				gl_VPos++;
				break;
		}
		switch(Read)
		{
		// file count in archive
		case 0x02:	Ret = *lpRead = Fake_Count(Buf, Read, &gl_VF);
					gl_VPos = 1;
					break;
		case 0x140:	Ret = *lpRead = Fake_Chunks(Buf, Read, &gl_VF);
					gl_VPos = 2;
					break;
		// length of some chunk
		case 0x04:
			switch(gl_VPos)
			{
			// size of the fmt chunk, although it's not actually used by the program
			case 2: *((unsigned int*)Buf) = 0x10;
					break;
			// data, in case it's a looping track (?)
			case 3:	*((unsigned int*)Buf) = SubtrackPCMTotal(&gl_VF, gl_VF.current_link) << 2;
					break;
			// And that fixes non-looping tracks, for some reason I forgot
			default:	*((unsigned int*)Buf) = 0x0;
						break;
			}
			gl_VPos++;
			Ret = *lpRead = Read;
			break;
		// WAVEFORMATEX
		case 0x12:	Ret = *lpRead = Fake_WAVEFORMATEX(Buf, Read, &gl_VF);
					gl_VPos = 3;
					break;
		}
		if(Ret != -1)	return Ret;
		
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
		// Convert to samples
		lDistanceToMove >>= 2;

		switch(dwMoveMethod)
		{
		case FILE_CURRENT:
			// Ignore any seeks from the current position, since this will most likely not end well
			return ov_pcm_tell(&gl_VF) << 2;
			break;
		case FILE_END:		lDistanceToMove = ov_pcm_total(&gl_VF, -1) - lDistanceToMove;
							break;
		}
		ov_pcm_seek(&gl_VF, lDistanceToMove);

		lDistanceToMove <<= 2;
		return lDistanceToMove;
	}
	else	return kernel32_SetFilePointer(hFile, lDistanceToMove, lpDistanceToMoveHigh, dwMoveMethod);
}
