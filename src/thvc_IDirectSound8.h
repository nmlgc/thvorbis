// Touhou Vorbis Compressor
// ------------------------
// thvc_IDirectSound8.h - IDirectSound8 hook
// ------------------------
// "©" Nmlgc, 2010-2011
// DLL hooking adopted from jisakujien's "noflashmyon" source

#ifndef THVC_IDIRECTSOUND8_H
#define THVC_IDIRECTSOUND8_H

class thvc_IDirectSound8 : public IDirectSound8
{
protected:
	IDirectSound8* pIDirectSound8;

public:
	 // IUnknown methods
    HRESULT	__stdcall	QueryInterface(REFIID riid, void** ppvObj);
	ULONG	__stdcall	AddRef(void);
	ULONG	__stdcall	Release(void);

    // IDirectSound methods
	HRESULT	__stdcall	Compact(void);
    HRESULT __stdcall	CreateSoundBuffer(LPCDSBUFFERDESC pcDSBufferDesc, LPDIRECTSOUNDBUFFER *ppDSBuffer, LPUNKNOWN pUnkOuter);
    HRESULT __stdcall	GetCaps(LPDSCAPS pDSCaps);
    HRESULT __stdcall	DuplicateSoundBuffer(LPDIRECTSOUNDBUFFER pDSBufferOriginal, LPDIRECTSOUNDBUFFER *ppDSBufferDuplicate);
    HRESULT __stdcall	SetCooperativeLevel(HWND hwnd, DWORD dwLevel);
    HRESULT __stdcall	GetSpeakerConfig(LPDWORD pdwSpeakerConfig);
    HRESULT __stdcall	SetSpeakerConfig(DWORD dwSpeakerConfig);
    HRESULT __stdcall	Initialize(LPCGUID pcGuidDevice);

    // IDirectSound8 methods
    HRESULT __stdcall	VerifyCertification(LPDWORD pdwCertified);

	thvc_IDirectSound8(IDirectSound8 *pOriginal);
	virtual ~thvc_IDirectSound8();
};

// Globals
extern thvc_IDirectSound8*	gl_IDirectSound8;

#endif /* THVC_IDIRECTSOUND8_H */
