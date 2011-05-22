// Touhou Vorbis Compressor - WINMM/TH06 implementation
// ------------------------
// mmio_th06.c - dummies out the mmio processing for TH06
// ------------------------
// "©" Nmlgc, 2010-2011
// DLL hooking adopted from jisakujien's "noflashmyon" source

#include "thvc_windows.h"
#include "thvc_mmsystem.h"
#include "thvorbis.h"
#include "winmm.h"
#include "fake.h"

MMRESULT thvc_mmioAdvance(__in_opt LPMMIOINFO pmmioinfo, __in UINT fuAdvance)
{
	// Do nothing. Decoding is handled at buffer unlocking time
	if(!pmmioinfo)	pmmioinfo = &gl_mmioinfo;
	pmmioinfo->pchNext = pmmioinfo->pchBuffer;
	return MMSYSERR_NOERROR;
}

MMRESULT thvc_mmioClose(__in UINT fuClose)
{
	gl_iLock = 1;
	return MMSYSERR_NOERROR;
}

MMRESULT thvc_mmioDescend(__inout LPMMCKINFO pmmcki, __in_opt const MMCKINFO FAR* pmmckiParent, __in UINT fuDescend)
{
	gl_iChunkSeq = Fake_Descend(pmmcki, gl_iChunkSeq, &gl_VF);
	     if(gl_iChunkSeq == 1)	gl_iLock = 1;
	else if(gl_iChunkSeq == 6)	gl_iLock = 0;

	return MMSYSERR_NOERROR;
}

LONG thvc_mmioSeek(__in LONG lOffset, __in int iOrigin)
{
	switch(iOrigin)
	{
	case 1:	ov_pcm_seek(&gl_VF, lOffset >> 2);
			break;
	}
	return lOffset;
}

MMRESULT thvc_mmioSetInfo(__in LPCMMIOINFO pmmioinfo, __in UINT fuInfo)
{
	return 0;
}
