// Touhou Vorbis Compressor - WINMM implementation
// ------------------------
// mmio_kog.c - Kioh Gyoku decode calls
// ------------------------
// "©" Nmlgc, 2010-2011
// DLL hooking adopted from jisakujien's "noflashmyon" source

#include <windows.h>
#include "thvc_mmsystem.h"
#include "thvorbis.h"
#include "winmm.h"
#include "fake.h"

MMRESULT thvc_mmioAdvance(__in_opt LPMMIOINFO pmmioinfo, __in UINT fuAdvance)
{
	int Link;
	long Ret = 1;
	long Read = 0;
	
	char* Buf = gl_mmioinfo.pchBuffer;
	
	while(Buf < gl_mmioinfo.pchEndWrite)
	{
		Ret = ov_read(&gl_VF, Buf, gl_mmioinfo.pchEndWrite - Buf, 0, 2, 1, &Link);
		if(Ret <= 0)	break;
		
		Buf += Ret;
		Read += Ret;
	}
	gl_mmioinfo.lBufOffset += Read;
	gl_mmioinfo.pchNext = gl_mmioinfo.pchBuffer;
	if(pmmioinfo)
	{
		pmmioinfo->lBufOffset = gl_mmioinfo.lBufOffset;
		pmmioinfo->pchNext = gl_mmioinfo.pchNext;
	}
	return MMSYSERR_NOERROR;
}

MMRESULT thvc_mmioClose(__in UINT fuClose)
{
	return MMSYSERR_NOERROR;
}

MMRESULT thvc_mmioDescend(__inout LPMMCKINFO pmmcki, __in_opt const MMCKINFO FAR* pmmckiParent, __in UINT fuDescend)
{
	gl_iChunkSeq = Fake_Descend(pmmcki, gl_iChunkSeq, &gl_VF);
	if(gl_iChunkSeq > 5)	gl_iChunkSeq -= 2;
	
	return MMSYSERR_NOERROR;
}

LONG thvc_mmioSeek(__in LONG lOffset, __in int iOrigin)
{
	switch(iOrigin)
	{
	case 0:	ov_pcm_seek_page(&gl_VF, 0);
			break;
	case 1:	ov_pcm_seek(&gl_VF, lOffset >> 2);
			break;
	}
	return gl_mmioinfo.lBufOffset = lOffset;
}

MMRESULT thvc_mmioSetInfo(__in LPCMMIOINFO pmmioinfo, __in UINT fuInfo)
{
	// This shouldn't be necessary at all
	ogg_int64_t NewPos = (pmmioinfo->lBufOffset - 12) >> 2;
	if(ov_pcm_tell(&gl_VF) != NewPos)	ov_pcm_seek(&gl_VF, NewPos);
		
	memcpy(&gl_mmioinfo, pmmioinfo, sizeof(MMIOINFO));

	return MMSYSERR_NOERROR;
}
