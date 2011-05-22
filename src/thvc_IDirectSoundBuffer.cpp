// Touhou Vorbis Compressor
// ------------------------
// thvc_IDirectSoundBuffer.cpp - IDirectSoundBuffer hook
// ------------------------
// "©" Nmlgc, 2010-2011
// DLL hooking adopted from jisakujien's "noflashmyon" source

#include <windows.h>
#include "thvc_mmsystem.h"
#include "dsound_dll.h"
#include "thvc_IDirectSoundBuffer.h"

#ifdef DECODE_AT_UNLOCK
extern "C"
{
#include "thvorbis.h"
#include "winmm.h"
}
#endif

#ifndef NO_DSOUND_HOOK

thvc_IDirectSoundBuffer::thvc_IDirectSoundBuffer(IDirectSoundBuffer *pOriginal)
{
	pIDirectSoundBuffer = pOriginal;
}

thvc_IDirectSoundBuffer::~thvc_IDirectSoundBuffer()
{
}

HRESULT __stdcall thvc_IDirectSoundBuffer::QueryInterface(REFIID riid, void** ppvObj)
{
	*ppvObj = NULL;

	// call this to increase AddRef at original object
	// and to check if such an interface is there

	HRESULT hRes = pIDirectSoundBuffer->QueryInterface(riid, ppvObj); 
	if (hRes == NOERROR)
	{
		// Check which interface was requested.
		// Replace only pointers to sound buffers!
		if(riid == IID_IDirectSoundBuffer || riid == IID_IDirectSoundBuffer8)		*ppvObj = this;
	}
	return hRes;
}

ULONG __stdcall thvc_IDirectSoundBuffer::AddRef()
{
	return(pIDirectSoundBuffer->AddRef());
}

ULONG __stdcall thvc_IDirectSoundBuffer::Release()
{
    // call original routine
	ULONG count = pIDirectSoundBuffer->Release();
	
    // in case no further Ref is there, the Original Object has deleted itself
	// so do we here
	if (count == 0) 
	{
  	    delete(this); 
	}

	return(count);
}

// Original methods
HRESULT __stdcall thvc_IDirectSoundBuffer::GetCaps(LPDSBCAPS pDSBufferCaps)
{
	return pIDirectSoundBuffer->GetCaps(pDSBufferCaps);
}
HRESULT __stdcall thvc_IDirectSoundBuffer::GetCurrentPosition(LPDWORD pdwCurrentPlayCursor, LPDWORD pdwCurrentWriteCursor)
{
	return pIDirectSoundBuffer->GetCurrentPosition(pdwCurrentPlayCursor, pdwCurrentWriteCursor);
}
HRESULT __stdcall thvc_IDirectSoundBuffer::GetFormat(LPWAVEFORMATEX pwfxFormat, DWORD dwSizeAllocated, LPDWORD pdwSizeWritten)
{
	return pIDirectSoundBuffer->GetFormat(pwfxFormat, dwSizeAllocated, pdwSizeWritten);
}
HRESULT __stdcall thvc_IDirectSoundBuffer::GetVolume(LPLONG plVolume)
{
	return pIDirectSoundBuffer->GetVolume(plVolume);
}
HRESULT __stdcall thvc_IDirectSoundBuffer::GetPan(LPLONG plPan)
{
	return pIDirectSoundBuffer->GetPan(plPan);
}
HRESULT __stdcall thvc_IDirectSoundBuffer::GetFrequency(LPDWORD pdwFrequency)
{
	return pIDirectSoundBuffer->GetFrequency(pdwFrequency);
}
HRESULT __stdcall thvc_IDirectSoundBuffer::GetStatus(LPDWORD pdwStatus)
{
	return pIDirectSoundBuffer->GetStatus(pdwStatus);
}
HRESULT __stdcall thvc_IDirectSoundBuffer::Initialize(LPDIRECTSOUND pDirectSound, LPCDSBUFFERDESC pcDSBufferDesc)
{
	return pIDirectSoundBuffer->Initialize(pDirectSound, pcDSBufferDesc);
}
HRESULT __stdcall thvc_IDirectSoundBuffer::Lock(DWORD dwOffset, DWORD dwBytes, LPVOID *ppvAudioPtr1, LPDWORD pdwAudioBytes1,
                                           LPVOID *ppvAudioPtr2, LPDWORD pdwAudioBytes2, DWORD dwFlags)
{
#ifdef DSOUND_PERF_HACK
	static DWORD dwGlobalOffset = -1;
#ifdef SLOW
	static const DWORD dwMaxBytes = (gl_BGMBufferSize >> 1) - (gl_BGMBufferSize >> 3);
#else
	static const DWORD dwMaxBytes = (gl_BGMBufferSize >> 2);
#endif
		
	// Limit the number of bytes to read
	if(dwBytes > dwMaxBytes)	{dwBytes = dwMaxBytes;	dwGlobalOffset = 0;}

	if(dwGlobalOffset != -1)
	{
		dwOffset = dwGlobalOffset;
		dwGlobalOffset += dwBytes;
		if(dwGlobalOffset >= gl_BGMBufferSize)	dwGlobalOffset -= gl_BGMBufferSize;
	}
#endif
	return pIDirectSoundBuffer->Lock(dwOffset, dwBytes, ppvAudioPtr1, pdwAudioBytes1,
                                           ppvAudioPtr2, pdwAudioBytes2, dwFlags);
}
HRESULT __stdcall thvc_IDirectSoundBuffer::Play(DWORD dwReserved1, DWORD dwPriority, DWORD dwFlags)
{
	return pIDirectSoundBuffer->Play(dwReserved1, dwPriority, dwFlags);
}
HRESULT __stdcall thvc_IDirectSoundBuffer::SetCurrentPosition(DWORD dwNewPosition)
{
	return pIDirectSoundBuffer->SetCurrentPosition(dwNewPosition);
}
HRESULT __stdcall thvc_IDirectSoundBuffer::SetFormat(LPCWAVEFORMATEX pcfxFormat)
{
	return pIDirectSoundBuffer->SetFormat(pcfxFormat);
}
HRESULT __stdcall thvc_IDirectSoundBuffer::SetVolume(LONG lVolume)
{
	return pIDirectSoundBuffer->SetVolume(lVolume);
}
HRESULT __stdcall thvc_IDirectSoundBuffer::SetPan(LONG lPan)
{
	return pIDirectSoundBuffer->SetPan(lPan);
}
HRESULT __stdcall thvc_IDirectSoundBuffer::SetFrequency(DWORD dwFrequency)
{
	return pIDirectSoundBuffer->SetFrequency(dwFrequency);
}
HRESULT __stdcall thvc_IDirectSoundBuffer::Stop()
{
	return pIDirectSoundBuffer->Stop();
}
#ifdef DECODE_AT_UNLOCK
// With TH06, the whole decoding happens here
int ov_decode(OggVorbis_File* vf, char* Buf, DWORD ToRead)
{
	int Link;
	long Ret;
	DWORD Read = ToRead;

	if(!vf || !vf->datasource || !Buf)	return 0;

	while(Read > 0)
	{
		Ret = ov_read(vf, Buf, Read, 0, 2, 1, &Link);
		if(!Ret)
		{
			ov_pcm_seek_page(vf, vf->pcmlengths[vf->current_link]);
		}
		
		Read -= Ret;
		Buf += Ret;
	}
	return ToRead - Read;
}

HRESULT __stdcall thvc_IDirectSoundBuffer::Unlock(LPVOID pvAudioPtr1, DWORD dwAudioBytes1, LPVOID pvAudioPtr2, DWORD dwAudioBytes2)
{
	if(!gl_iLock)
	{
#ifdef _DEBUG
		fprintf(Log, "IDirectSoundBuffer::Unlock: %d + %d bytes\n", dwAudioBytes1, dwAudioBytes2);
#endif
		ov_decode(&gl_VF, (char*)pvAudioPtr1, dwAudioBytes1);
		ov_decode(&gl_VF, (char*)pvAudioPtr2, dwAudioBytes2);
	}
	else
	{
		dwAudioBytes1 = 0;
		dwAudioBytes2 = 0;
	}

	return pIDirectSoundBuffer->Unlock(pvAudioPtr1, dwAudioBytes1, pvAudioPtr2, dwAudioBytes2);
}
#else
HRESULT __stdcall thvc_IDirectSoundBuffer::Unlock(LPVOID pvAudioPtr1, DWORD dwAudioBytes1, LPVOID pvAudioPtr2, DWORD dwAudioBytes2)
{
	return pIDirectSoundBuffer->Unlock(pvAudioPtr1, dwAudioBytes1, pvAudioPtr2, dwAudioBytes2);
}
#endif
HRESULT __stdcall thvc_IDirectSoundBuffer::Restore()
{
	return pIDirectSoundBuffer->Restore();
}
#endif /* NO_DSOUND_HOOK */
