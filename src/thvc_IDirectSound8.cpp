// Touhou Vorbis Compressor
// ------------------------
// thvc_IDirectSound8.cpp - IDirectSound8 hook
// ------------------------
// "©" Nmlgc, 2010-2011
// DLL hooking adopted from jisakujien's "noflashmyon" source

#include <windows.h>
#include "thvc_mmsystem.h"
#include "dsound_dll.h"
#include "thvc_IDirectSound8.h"
#include "thvc_IDirectSoundBuffer.h"

#ifndef NO_DSOUND_HOOK

thvc_IDirectSound8::thvc_IDirectSound8(IDirectSound8 *pOriginal)
{
	pIDirectSound8 = pOriginal;
}

thvc_IDirectSound8::~thvc_IDirectSound8()
{
}

HRESULT thvc_IDirectSound8::QueryInterface(REFIID riid, void** ppvObj)
{
	*ppvObj = NULL;

	// call this to increase AddRef at original object
	// and to check if such an interface is there

	HRESULT hRes = pIDirectSound8->QueryInterface(riid, ppvObj); 
	if (hRes == NOERROR) // if OK, send our "fake" address
	{
		*ppvObj = this;
	}
	return hRes;
}

ULONG thvc_IDirectSound8::AddRef()
{
	return(pIDirectSound8->AddRef());
}

ULONG thvc_IDirectSound8::Release()
{
    // call original routine
	ULONG count = pIDirectSound8->Release();
	
    // in case no further Ref is there, the Original Object has deleted itself
	// so do we here
	if (count == 0) 
	{
		gl_IDirectSound8 = NULL;
  	    delete(this); 
	}

	return(count);
}

// Original methods
HRESULT thvc_IDirectSound8::Compact()
{
	return pIDirectSound8->Compact();
}

HRESULT thvc_IDirectSound8::CreateSoundBuffer(LPCDSBUFFERDESC pcDSBufferDesc, LPDIRECTSOUNDBUFFER *ppDSBuffer, LPUNKNOWN pUnkOuter)
{
	HRESULT Ret;

	// Get original object
	Ret = pIDirectSound8->CreateSoundBuffer(pcDSBufferDesc, ppDSBuffer, pUnkOuter);
	if(Ret != DS_OK)	return Ret;

	// only replace the BGM buffer
	if(pcDSBufferDesc->dwBufferBytes == gl_BGMBufferSize)
	{
		thvc_IDirectSoundBuffer*	New = new thvc_IDirectSoundBuffer(*ppDSBuffer);
		*ppDSBuffer = New;
	}
	return Ret;
}

HRESULT thvc_IDirectSound8::GetCaps(LPDSCAPS pDSCaps)
{
	return pIDirectSound8->GetCaps(pDSCaps);
}
HRESULT thvc_IDirectSound8::DuplicateSoundBuffer(LPDIRECTSOUNDBUFFER pDSBufferOriginal, LPDIRECTSOUNDBUFFER *ppDSBufferDuplicate)
{
	return pIDirectSound8->DuplicateSoundBuffer(pDSBufferOriginal, ppDSBufferDuplicate);
}
HRESULT thvc_IDirectSound8::SetCooperativeLevel(HWND hwnd, DWORD dwLevel)
{
	return pIDirectSound8->SetCooperativeLevel(hwnd, dwLevel);
}
HRESULT thvc_IDirectSound8::GetSpeakerConfig(LPDWORD pdwSpeakerConfig)
{
	return pIDirectSound8->GetSpeakerConfig(pdwSpeakerConfig);
}
HRESULT thvc_IDirectSound8::SetSpeakerConfig(DWORD dwSpeakerConfig)
{
	return pIDirectSound8->SetSpeakerConfig(dwSpeakerConfig);
}
HRESULT thvc_IDirectSound8::Initialize(LPCGUID pcGuidDevice)
{
	return pIDirectSound8->Initialize(pcGuidDevice);
}
HRESULT thvc_IDirectSound8::VerifyCertification(LPDWORD pdwCertified)
{
	return pIDirectSound8->VerifyCertification(pdwCertified);
}

#endif /* NO_DSOUND_HOOK */
