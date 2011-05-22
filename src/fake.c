// Touhou Vorbis Compressor
// ------------------------
// fake.c - Shared data faking functions
// ------------------------
// "©" Nmlgc, 2011

#include <windows.h>
#include "thvc_mmsystem.h"
#include "thvorbis.h"
#include <vorbis/vorbisfile.h>

// WAVEFORMATEX
int Fake_WAVEFORMATEX(char* Buf, DWORD Read, OggVorbis_File* vf)
{
	// vf->current_link == -1 would cause ov_info to return an invalid structure
	vorbis_info* vi = ov_info(vf, vf->current_link == -1 ? 0 : -1);
	WAVEFORMATEX* wfx = (WAVEFORMATEX*)Buf;

	if(!vi)	return 0;

	wfx->wFormatTag = 0x1;
	wfx->nChannels = vi->channels;
	wfx->nSamplesPerSec = vi->rate;
	wfx->wBitsPerSample = 16;
	wfx->nBlockAlign = (wfx->nChannels * wfx->wBitsPerSample) / 8;
	wfx->nAvgBytesPerSec = wfx->nSamplesPerSec * wfx->nBlockAlign;
	return Read;
}

// Descend
int Fake_Descend(__inout LPMMCKINFO pmmcki, int iChunkSeq, OggVorbis_File* vf)
{
	const FOURCC cRIFF = 0x46464952;	// "RIFF"
	const FOURCC cWAVE = 0x45564157;	// "WAVE"
	const FOURCC cFmt  = 0x20746D66;	// "fmt"
	const FOURCC cData = 0x61746164;	// "data"

	pmmcki->fccType = 0;
	switch(iChunkSeq)
	{
	case 0:	// RIFF
			pmmcki->ckid = cRIFF;
			pmmcki->cksize = ov_pcm_total(&gl_VF, -1) << 2;
			pmmcki->fccType = cWAVE;
			pmmcki->dwDataOffset = 8;
			break;
	case 1:	// fmt
			pmmcki->ckid = cFmt;
			pmmcki->cksize = 16;
			pmmcki->dwDataOffset = 20;
			break;
	default: // data
			pmmcki->ckid = cData;
			pmmcki->cksize = ov_pcm_total(&gl_VF, -1) << 2;
			pmmcki->dwDataOffset = 44;
			break;
	}
	return iChunkSeq + 1;
}
