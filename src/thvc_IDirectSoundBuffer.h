// Touhou Vorbis Compressor
// ------------------------
// thvc_IDirectSoundBuffer.h - IDirectSoundBuffer hook
// ------------------------
// "©" Nmlgc, 2010-2011
// DLL hooking adopted from jisakujien's "noflashmyon" source

#ifndef THVC_IDIRECTSOUNDBUFFER_H
#define THVC_IDIRECTSOUNDBUFFER_H

class thvc_IDirectSoundBuffer : public IDirectSoundBuffer
{
protected:
	IDirectSoundBuffer* pIDirectSoundBuffer;

public:
	 // IUnknown methods
    HRESULT	__stdcall	QueryInterface(REFIID riid, void** ppvObj);
	ULONG	__stdcall	AddRef(void);
	ULONG	__stdcall	Release(void);

    // IDirectSoundBuffer methods
	HRESULT __stdcall GetCaps(LPDSBCAPS pDSBufferCaps);
    HRESULT __stdcall GetCurrentPosition(LPDWORD pdwCurrentPlayCursor, LPDWORD pdwCurrentWriteCursor);
    HRESULT __stdcall GetFormat(LPWAVEFORMATEX pwfxFormat, DWORD dwSizeAllocated, LPDWORD pdwSizeWritten);
    HRESULT __stdcall GetVolume(LPLONG plVolume);
    HRESULT __stdcall GetPan(LPLONG plPan);
    HRESULT __stdcall GetFrequency(LPDWORD pdwFrequency);
    HRESULT __stdcall GetStatus(LPDWORD pdwStatus);
    HRESULT __stdcall Initialize(LPDIRECTSOUND pDirectSound, LPCDSBUFFERDESC pcDSBufferDesc);
    HRESULT __stdcall Lock(DWORD dwOffset, DWORD dwBytes, LPVOID *ppvAudioPtr1, LPDWORD pdwAudioBytes1,
                                           LPVOID *ppvAudioPtr2, LPDWORD pdwAudioBytes2, DWORD dwFlags);
    HRESULT __stdcall Play(DWORD dwReserved1, DWORD dwPriority, DWORD dwFlags);
    HRESULT __stdcall SetCurrentPosition(DWORD dwNewPosition);
    HRESULT __stdcall SetFormat(LPCWAVEFORMATEX pcfxFormat);
    HRESULT __stdcall SetVolume(LONG lVolume);
    HRESULT __stdcall SetPan(LONG lPan);
    HRESULT __stdcall SetFrequency(DWORD dwFrequency);
    HRESULT __stdcall Stop(void);
    HRESULT __stdcall Unlock(LPVOID pvAudioPtr1, DWORD dwAudioBytes1, LPVOID pvAudioPtr2, DWORD dwAudioBytes2);
    HRESULT __stdcall Restore(void);

	thvc_IDirectSoundBuffer(IDirectSoundBuffer *pOriginal);
	virtual ~thvc_IDirectSoundBuffer();
};

#endif /* THVC_IDIRECTSOUNDBUFFER_H */
