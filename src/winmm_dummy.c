// Touhou Vorbis Compressor - WINMM implementation
// ------------------------
// winmm_dummy.c - wraps non-intercepted winmm.dll functions referenced by Kioh Gyoku and th075.
// Nothing particularly interesting in this file.
// ------------------------
// "©" Nmlgc, 2011
// DLL hooking adopted from jisakujien's "noflashmyon" source

#include <windows.h>
#include "thvc_mmsystem.h"
#include "winmm.h"

// MIDI
MMRESULT WINAPI midiOutClose( __in HMIDIOUT hmo)
{
	return winmm_midiOutClose(hmo);
}
MMRESULT WINAPI midiOutGetDevCapsA( __in UINT_PTR uDeviceID, __out_bcount(cbmoc) LPMIDIOUTCAPSA pmoc, __in UINT cbmoc)
{
	return winmm_midiOutGetDevCapsA(uDeviceID, pmoc, cbmoc);
}
UINT WINAPI midiOutGetNumDevs(void)
{
	return winmm_midiOutGetNumDevs();
}
MMRESULT WINAPI midiOutLongMsg(__in HMIDIOUT hmo, __in_bcount(cbmh) LPMIDIHDR pmh, __in UINT cbmh)
{
	return winmm_midiOutLongMsg(hmo, pmh, cbmh);
}
MMRESULT WINAPI midiOutOpen( __out LPHMIDIOUT phmo, __in UINT uDeviceID,
    __in_opt DWORD_PTR dwCallback, __in_opt DWORD_PTR dwInstance, __in DWORD fdwOpen)
{
	return winmm_midiOutOpen(phmo, uDeviceID, dwCallback, dwInstance,fdwOpen);
}
MMRESULT WINAPI midiOutPrepareHeader( __in HMIDIOUT hmo, __inout_bcount(cbmh) LPMIDIHDR pmh, __in UINT cbmh)
{
	return winmm_midiOutPrepareHeader(hmo, pmh, cbmh);
}
MMRESULT WINAPI midiOutReset( __in HMIDIOUT hmo)
{
	return winmm_midiOutReset(hmo);
}
MMRESULT WINAPI midiOutShortMsg( __in HMIDIOUT hmo, __in DWORD dwMsg)
{
	return winmm_midiOutShortMsg(hmo, dwMsg);
}
MMRESULT WINAPI midiOutUnprepareHeader(__in HMIDIOUT hmo, __inout_bcount(cbmh) LPMIDIHDR pmh, __in UINT cbmh)
{
	return winmm_midiOutUnprepareHeader(hmo, pmh,cbmh);
}
// ----

MMRESULT mmioAdvance( __in HMMIO hmmio, __in_opt LPMMIOINFO pmmioinfo, __in UINT fuAdvance)
{
	return winmm_mmioAdvance(hmmio, pmmioinfo, fuAdvance);
}
MMRESULT mmioAscend( __in HMMIO hmmio, __in LPMMCKINFO pmmcki, __in UINT fuAscend)
{
	return winmm_mmioAscend(hmmio, pmmcki, fuAscend);
}
MMRESULT mmioClose( __in HMMIO hmmio, __in UINT fuClose)
{
	return winmm_mmioClose(hmmio, fuClose);
}
MMRESULT mmioDescend( __in HMMIO hmmio, __inout LPMMCKINFO pmmcki,
    __in_opt const MMCKINFO FAR* pmmckiParent, __in UINT fuDescend)
{
	return winmm_mmioDescend(hmmio, pmmcki, pmmckiParent, fuDescend);
}
MMRESULT mmioGetInfo( __in HMMIO hmmio, __out LPMMIOINFO pmmioinfo, __in UINT fuInfo)
{
	return winmm_mmioGetInfo(hmmio, pmmioinfo, fuInfo);
}
HMMIO WINAPI mmioOpenA( __inout_bcount_opt(128) LPSTR pszFileName,  __inout_opt LPMMIOINFO pmmioinfo,  __in DWORD fdwOpen)
{
	return winmm_mmioOpenA(pszFileName, pmmioinfo, fdwOpen);
}
HMMIO WINAPI mmioOpenW( __inout_bcount_opt(128) LPWSTR pszFileName,  __inout_opt LPMMIOINFO pmmioinfo,  __in DWORD fdwOpen)
{
	return winmm_mmioOpenW(pszFileName, pmmioinfo, fdwOpen);
}
LONG mmioRead( __in HMMIO hmmio, __out_bcount(cch) HPSTR pch, __in LONG cch)
{
	return winmm_mmioRead(hmmio, pch, cch);
}
LONG WINAPI mmioSeek( __in HMMIO hmmio, __in LONG lOffset, __in int iOrigin)
{
	return winmm_mmioSeek(hmmio, lOffset, iOrigin);
}
MMRESULT mmioSetInfo( __in HMMIO hmmio, __in LPCMMIOINFO pmmioinfo, __in UINT fuInfo)
{
	return winmm_mmioSetInfo(hmmio, pmmioinfo, fuInfo);
}

// Timer
// -----
MMRESULT WINAPI timeBeginPeriod( __in UINT uPeriod)
{
	return winmm_timeBeginPeriod(uPeriod);
}
MMRESULT WINAPI timeEndPeriod( __in UINT uPeriod)
{
	return winmm_timeEndPeriod(uPeriod);
}
MMRESULT WINAPI timeGetDevCaps( __out_bcount(cbtc) LPTIMECAPS ptc, __in UINT cbtc)
{
	return winmm_timeGetDevCaps(ptc, cbtc);
}
DWORD WINAPI timeGetTime(void)
{
	return winmm_timeGetTime();
}
MMRESULT WINAPI timeSetEvent( __in UINT uDelay, __in UINT uResolution,
    __in LPTIMECALLBACK fptc, __in DWORD_PTR dwUser, __in UINT fuEvent)
{
	return winmm_timeSetEvent(uDelay, uResolution, fptc, dwUser, fuEvent);
}
MMRESULT WINAPI timeKillEvent( __in UINT uTimerID)
{
	return winmm_timeKillEvent(uTimerID);
}
// -----
#ifdef WINMM_FULL
// Mixer
MMRESULT WINAPI mixerClose( __in HMIXER hmx)
{
	return winmm_mixerClose(hmx);
}
MMRESULT WINAPI mixerGetControlDetailsW( __in_opt HMIXEROBJ hmxobj, __in LPMIXERCONTROLDETAILS pmxcd, __in DWORD fdwDetails)
{
	return winmm_mixerGetControlDetailsW(hmxobj, pmxcd, fdwDetails);
}
MMRESULT WINAPI mixerGetLineControlsW( __in_opt HMIXEROBJ hmxobj, __inout LPMIXERLINECONTROLSW pmxlc, __in DWORD fdwControls)
{
	return winmm_mixerGetLineControlsW(hmxobj, pmxlc, fdwControls);
}
MMRESULT WINAPI mixerGetLineInfoW( __in_opt HMIXEROBJ hmxobj, __out LPMIXERLINEW pmxl, __in DWORD fdwInfo)
{
	return winmm_mixerGetLineInfoW(hmxobj, pmxl, fdwInfo);
}
MMRESULT WINAPI mixerOpen( __out_opt LPHMIXER phmx, __in UINT uMxId, __in_opt DWORD_PTR dwCallback, __in_opt DWORD_PTR dwInstance, __in DWORD fdwOpen)
{
	return winmm_mixerOpen(phmx, uMxId, dwCallback, dwInstance, fdwOpen);
}
MMRESULT WINAPI mixerSetControlDetails( __in_opt HMIXEROBJ hmxobj, __in LPMIXERCONTROLDETAILS pmxcd, __in DWORD fdwDetails)
{
	return winmm_mixerSetControlDetails(hmxobj, pmxcd, fdwDetails);
}
// -----
// waveIn
// ------
MMRESULT WINAPI waveInAddBuffer( __in HWAVEIN hwi, __inout_bcount(cbwh) LPWAVEHDR pwh, __in UINT cbwh)
{
	return winmm_waveInAddBuffer(hwi, pwh, cbwh);
}
MMRESULT WINAPI waveInClose( __in HWAVEIN hwi)
{
	return winmm_waveInClose(hwi);
}
MMRESULT WINAPI waveInGetDevCapsW( __in UINT_PTR uDeviceID, __out_bcount(cbwic) LPWAVEINCAPSW pwic, __in UINT cbwic)
{
	return winmm_waveInGetDevCapsW(uDeviceID, pwic, cbwic);
}
UINT WINAPI waveInGetNumDevs(void)
{
	return winmm_waveInGetNumDevs();
}
MMRESULT WINAPI waveInGetPosition( __in HWAVEIN hwi, __inout_bcount(cbmmt) LPMMTIME pmmt, __in UINT cbmmt)
{
	return winmm_waveInGetPosition(hwi, pmmt, cbmmt);
}
MMRESULT WINAPI waveInMessage( __in_opt HWAVEIN hwi, __in UINT uMsg, __in_opt DWORD_PTR dw1, __in_opt DWORD_PTR dw2)
{
	return winmm_waveInMessage(hwi, uMsg, dw1, dw2);
}
MMRESULT WINAPI waveInOpen( __out_opt LPHWAVEIN phwi, __in UINT uDeviceID,
    __in LPCWAVEFORMATEX pwfx, __in_opt DWORD_PTR dwCallback, __in_opt DWORD_PTR dwInstance, __in DWORD fdwOpen)
{
	return winmm_waveInOpen(phwi, uDeviceID, pwfx, dwCallback, dwInstance, fdwOpen);
}
MMRESULT WINAPI waveInPrepareHeader( __in HWAVEIN hwi, __inout_bcount(cbwh) LPWAVEHDR pwh, __in UINT cbwh)
{
	return winmm_waveInPrepareHeader(hwi, pwh, cbwh);
}
MMRESULT WINAPI waveInReset( __in HWAVEIN hwi)
{
	return winmm_waveInReset(hwi);
}
MMRESULT WINAPI waveInStart( __in HWAVEIN hwi)
{
	return winmm_waveInStart(hwi);
}
MMRESULT WINAPI waveInStop( __in HWAVEIN hwi)
{
	return winmm_waveInStop(hwi);
}
MMRESULT WINAPI waveInUnprepareHeader( __in HWAVEIN hwi, __inout_bcount(cbwh) LPWAVEHDR pwh, __in UINT cbwh)
{
	return winmm_waveInUnprepareHeader(hwi, pwh, cbwh);
}
// ------
// waveOut
// -------
MMRESULT WINAPI waveOutClose( __in HWAVEOUT hwo)
{
	return winmm_waveOutClose(hwo);
}
MMRESULT WINAPI waveOutGetDevCapsW( __in UINT_PTR uDeviceID, __out LPWAVEOUTCAPSW pwoc, __in UINT cbwoc)
{
	return winmm_waveOutGetDevCapsW(uDeviceID, pwoc, cbwoc);
}
UINT WINAPI waveOutGetNumDevs(void)
{
	return winmm_waveOutGetNumDevs();
}
MMRESULT WINAPI waveOutGetPosition( __in HWAVEOUT hwo, __inout_bcount(cbmmt) LPMMTIME pmmt, __in UINT cbmmt)
{
	return winmm_waveOutGetPosition(hwo, pmmt, cbmmt);
}
MMRESULT WINAPI waveOutGetVolume(HWAVEOUT hwo, LPDWORD pdwVolume)
{
	return winmm_waveOutGetVolume(hwo, pdwVolume);
}
MMRESULT WINAPI waveOutMessage( __in_opt HWAVEOUT hwo, __in UINT uMsg, __in DWORD_PTR dw1, __in DWORD_PTR dw2)
{
	return winmm_waveOutMessage(hwo, uMsg, dw1, dw2);
}
MMRESULT WINAPI waveOutOpen( __out_opt LPHWAVEOUT phwo, __in UINT uDeviceID,
    __in LPCWAVEFORMATEX pwfx, __in_opt DWORD_PTR dwCallback, __in_opt DWORD_PTR dwInstance, __in DWORD fdwOpen)
{
	return winmm_waveOutOpen(phwo, uDeviceID, pwfx, dwCallback, dwInstance, fdwOpen);
}
MMRESULT WINAPI waveOutPause( __in HWAVEOUT hwo)
{
	return winmm_waveOutPause(hwo);
}
MMRESULT WINAPI waveOutPrepareHeader( __in HWAVEOUT hwo, __inout_bcount(cbwh) LPWAVEHDR pwh,  __in UINT cbwh)
{
	return winmm_waveOutPrepareHeader(hwo, pwh, cbwh);
}
MMRESULT WINAPI waveOutReset( __in HWAVEOUT hwo)
{
	return winmm_waveOutReset(hwo);
}
MMRESULT WINAPI waveOutRestart( __in HWAVEOUT hwo)
{
	return winmm_waveOutRestart(hwo);
}
MMRESULT WINAPI waveOutSetVolume(HWAVEOUT hwo, DWORD dwVolume)
{
	return winmm_waveOutSetVolume(hwo, dwVolume);
}
MMRESULT WINAPI waveOutUnprepareHeader( __in HWAVEOUT hwo, __inout_bcount(cbwh) LPWAVEHDR pwh, __in UINT cbwh)
{
	return winmm_waveOutUnprepareHeader(hwo, pwh, cbwh);
}
MMRESULT WINAPI waveOutWrite( __in HWAVEOUT hwo, __inout_bcount(cbwh) LPWAVEHDR pwh, __in UINT cbwh)
{
	return winmm_waveOutWrite(hwo, pwh, cbwh);
}
// -------
#endif
