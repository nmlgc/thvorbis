/*
 * Touhou Vorbis Compressor
 *
 * ----
 *
 * Alternative mmsystem header *not* defining our custom functions as
 * dllimport.
 */

/*==========================================================================
 *
 *  mmsystem.h -- Include file for Multimedia API's
 *
 *  Version 4.00
 *
 *  Copyright (C) 1992-1998 Microsoft Corporation.  All Rights Reserved.
 *
 *--------------------------------------------------------------------------
 *
 *  Define:         Prevent inclusion of:
 *  --------------  --------------------------------------------------------
 *  MMIONOTIMER     Timer support
 *  MMNOMMIO        Multimedia file I/O support
 *  MMNOWAVE        Waveform support
 *  MMNOMIDI        MIDI support
 *  MMNOMIXER       Mixer support
 *  MMNOMMSYSTEM    General MMSYSTEM functions
 *
 *==========================================================================
 */

#ifndef _INC_MMSYSTEM
#define _INC_MMSYSTEM   /* #defined if mmsystem.h has been included */


#ifdef _WIN32
#include <pshpack1.h>
#else
#ifndef RC_INVOKED
#pragma pack(1)
#endif
#endif

#ifdef __cplusplus
extern "C" {            /* Assume C declarations for C++ */
#endif  /* __cplusplus */

#ifdef _WIN32
#ifndef _WINMM_
#define WINMMAPI DECLSPEC_IMPORT
#else
#define WINMMAPI
#endif
#define _loadds
#define _huge
#else
#define WINMMAPI
#endif


#ifdef _MAC
#include <macwin32.h>
#endif //_MAC

/****************************************************************************

                    General constants and data types

****************************************************************************/


/* general constants */
#define MAXPNAMELEN      32     /* max product name length (including NULL) */
#define MAXERRORLENGTH   256    /* max error text length (including NULL) */
#define MAX_JOYSTICKOEMVXDNAME 260 /* max oem vxd name length (including NULL) */

/*
 *  Microsoft Manufacturer and Product ID's (these have been moved to
 *  MMREG.H for Windows 4.00 and above).
 */
#if (WINVER <= 0x0400)
#ifndef MM_MICROSOFT
#define MM_MICROSOFT            1   /* Microsoft Corporation */
#endif

#ifndef MM_MIDI_MAPPER
#define MM_MIDI_MAPPER          1   /* MIDI Mapper */
#define MM_WAVE_MAPPER          2   /* Wave Mapper */
#define MM_SNDBLST_MIDIOUT      3   /* Sound Blaster MIDI output port */
#define MM_SNDBLST_MIDIIN       4   /* Sound Blaster MIDI input port */
#define MM_SNDBLST_SYNTH        5   /* Sound Blaster internal synthesizer */
#define MM_SNDBLST_WAVEOUT      6   /* Sound Blaster waveform output */
#define MM_SNDBLST_WAVEIN       7   /* Sound Blaster waveform input */
#define MM_ADLIB                9   /* Ad Lib-compatible synthesizer */
#define MM_MPU401_MIDIOUT      10   /* MPU401-compatible MIDI output port */
#define MM_MPU401_MIDIIN       11   /* MPU401-compatible MIDI input port */
#define MM_PC_JOYSTICK         12   /* Joystick adapter */
#endif
#endif



/* general data types */

#ifdef _WIN32
typedef UINT        MMVERSION;  /* major (high byte), minor (low byte) */
#else
typedef UINT        VERSION;    /* major (high byte), minor (low byte) */
#endif
typedef __success( return == 0) UINT        MMRESULT;   /* error return code, 0 means no error */
                                /* call as if(err=xxxx(...)) Error(err); else */
#define _MMRESULT_

typedef UINT FAR   *LPUINT;



/* MMTIME data structure */
typedef struct mmtime_tag
{
    UINT            wType;      /* indicates the contents of the union */
    union
    {
        DWORD       ms;         /* milliseconds */
        DWORD       sample;     /* samples */
        DWORD       cb;         /* byte count */
        DWORD       ticks;      /* ticks in MIDI stream */

        /* SMPTE */
        struct
        {
            BYTE    hour;       /* hours */
            BYTE    min;        /* minutes */
            BYTE    sec;        /* seconds */
            BYTE    frame;      /* frames  */
            BYTE    fps;        /* frames per second */
            BYTE    dummy;      /* pad */
#ifdef _WIN32
            BYTE    pad[2];
#endif
        } smpte;

        /* MIDI */
        struct
        {
            DWORD songptrpos;   /* song pointer position */
        } midi;
    } u;
} MMTIME, *PMMTIME, NEAR *NPMMTIME, FAR *LPMMTIME;

/* types for wType field in MMTIME struct */
#define TIME_MS         0x0001  /* time in milliseconds */
#define TIME_SAMPLES    0x0002  /* number of wave samples */
#define TIME_BYTES      0x0004  /* current byte offset */
#define TIME_SMPTE      0x0008  /* SMPTE time */
#define TIME_MIDI       0x0010  /* MIDI time */
#define TIME_TICKS      0x0020  /* Ticks within MIDI stream */

/*
 *
 *
 */
#define MAKEFOURCC(ch0, ch1, ch2, ch3)                              \
                ((DWORD)(BYTE)(ch0) | ((DWORD)(BYTE)(ch1) << 8) |   \
                ((DWORD)(BYTE)(ch2) << 16) | ((DWORD)(BYTE)(ch3) << 24 ))



/****************************************************************************

                    Multimedia Extensions Window Messages

****************************************************************************/

#define MM_JOY1MOVE         0x3A0           /* joystick */
#define MM_JOY2MOVE         0x3A1
#define MM_JOY1ZMOVE        0x3A2
#define MM_JOY2ZMOVE        0x3A3
#define MM_JOY1BUTTONDOWN   0x3B5
#define MM_JOY2BUTTONDOWN   0x3B6
#define MM_JOY1BUTTONUP     0x3B7
#define MM_JOY2BUTTONUP     0x3B8

#define MM_MCINOTIFY        0x3B9           /* MCI */

#define MM_WOM_OPEN         0x3BB           /* waveform output */
#define MM_WOM_CLOSE        0x3BC
#define MM_WOM_DONE         0x3BD

#define MM_WIM_OPEN         0x3BE           /* waveform input */
#define MM_WIM_CLOSE        0x3BF
#define MM_WIM_DATA         0x3C0

#define MM_MIM_OPEN         0x3C1           /* MIDI input */
#define MM_MIM_CLOSE        0x3C2
#define MM_MIM_DATA         0x3C3
#define MM_MIM_LONGDATA     0x3C4
#define MM_MIM_ERROR        0x3C5
#define MM_MIM_LONGERROR    0x3C6

#define MM_MOM_OPEN         0x3C7           /* MIDI output */
#define MM_MOM_CLOSE        0x3C8
#define MM_MOM_DONE         0x3C9

/* these are also in msvideo.h */
#ifndef MM_DRVM_OPEN
 #define MM_DRVM_OPEN       0x3D0           /* installable drivers */
 #define MM_DRVM_CLOSE      0x3D1
 #define MM_DRVM_DATA       0x3D2
 #define MM_DRVM_ERROR      0x3D3
#endif

/* these are used by msacm.h */
#define MM_STREAM_OPEN      0x3D4
#define MM_STREAM_CLOSE     0x3D5
#define MM_STREAM_DONE      0x3D6
#define MM_STREAM_ERROR     0x3D7

#if(WINVER >= 0x0400)
#define MM_MOM_POSITIONCB   0x3CA           /* Callback for MEVT_POSITIONCB */

#ifndef MM_MCISIGNAL
 #define MM_MCISIGNAL        0x3CB
#endif

#define MM_MIM_MOREDATA      0x3CC          /* MIM_DONE w/ pending events */

#endif /* WINVER >= 0x0400 */


#define MM_MIXM_LINE_CHANGE     0x3D0       /* mixer line change notify */
#define MM_MIXM_CONTROL_CHANGE  0x3D1       /* mixer control change notify */


/****************************************************************************

                String resource number bases (internal use)

****************************************************************************/

#define MMSYSERR_BASE          0
#define WAVERR_BASE            32
#define MIDIERR_BASE           64
#define TIMERR_BASE            96
#define JOYERR_BASE            160
#define MCIERR_BASE            256
#define MIXERR_BASE            1024

#define MCI_STRING_OFFSET      512
#define MCI_VD_OFFSET          1024
#define MCI_CD_OFFSET          1088
#define MCI_WAVE_OFFSET        1152
#define MCI_SEQ_OFFSET         1216

/****************************************************************************

                        General error return values

****************************************************************************/

/* general error return values */
#define MMSYSERR_NOERROR      0                    /* no error */
#define MMSYSERR_ERROR        (MMSYSERR_BASE + 1)  /* unspecified error */
#define MMSYSERR_BADDEVICEID  (MMSYSERR_BASE + 2)  /* device ID out of range */
#define MMSYSERR_NOTENABLED   (MMSYSERR_BASE + 3)  /* driver failed enable */
#define MMSYSERR_ALLOCATED    (MMSYSERR_BASE + 4)  /* device already allocated */
#define MMSYSERR_INVALHANDLE  (MMSYSERR_BASE + 5)  /* device handle is invalid */
#define MMSYSERR_NODRIVER     (MMSYSERR_BASE + 6)  /* no device driver present */
#define MMSYSERR_NOMEM        (MMSYSERR_BASE + 7)  /* memory allocation error */
#define MMSYSERR_NOTSUPPORTED (MMSYSERR_BASE + 8)  /* function isn't supported */
#define MMSYSERR_BADERRNUM    (MMSYSERR_BASE + 9)  /* error value out of range */
#define MMSYSERR_INVALFLAG    (MMSYSERR_BASE + 10) /* invalid flag passed */
#define MMSYSERR_INVALPARAM   (MMSYSERR_BASE + 11) /* invalid parameter passed */
#define MMSYSERR_HANDLEBUSY   (MMSYSERR_BASE + 12) /* handle being used */
                                                   /* simultaneously on another */
                                                   /* thread (eg callback) */
#define MMSYSERR_INVALIDALIAS (MMSYSERR_BASE + 13) /* specified alias not found */
#define MMSYSERR_BADDB        (MMSYSERR_BASE + 14) /* bad registry database */
#define MMSYSERR_KEYNOTFOUND  (MMSYSERR_BASE + 15) /* registry key not found */
#define MMSYSERR_READERROR    (MMSYSERR_BASE + 16) /* registry read error */
#define MMSYSERR_WRITEERROR   (MMSYSERR_BASE + 17) /* registry write error */
#define MMSYSERR_DELETEERROR  (MMSYSERR_BASE + 18) /* registry delete error */
#define MMSYSERR_VALNOTFOUND  (MMSYSERR_BASE + 19) /* registry value not found */
#define MMSYSERR_NODRIVERCB   (MMSYSERR_BASE + 20) /* driver does not call DriverCallback */
#define MMSYSERR_MOREDATA     (MMSYSERR_BASE + 21) /* more data to be returned */
#define MMSYSERR_LASTERROR    (MMSYSERR_BASE + 21) /* last error in range */

#if (WINVER < 0x030a) || defined(_WIN32)
DECLARE_HANDLE(HDRVR);
#endif /* ifdef WINVER < 0x030a */

/****************************************************************************

                          Driver callback support

****************************************************************************/

/* flags used with waveOutOpen(), waveInOpen(), midiInOpen(), and */
/* midiOutOpen() to specify the type of the dwCallback parameter. */

#define CALLBACK_TYPEMASK   0x00070000l    /* callback type mask */
#define CALLBACK_NULL       0x00000000l    /* no callback */
#define CALLBACK_WINDOW     0x00010000l    /* dwCallback is a HWND */
#define CALLBACK_TASK       0x00020000l    /* dwCallback is a HTASK */
#define CALLBACK_FUNCTION   0x00030000l    /* dwCallback is a FARPROC */
#ifdef _WIN32
#define CALLBACK_THREAD     (CALLBACK_TASK)/* thread ID replaces 16 bit task */
#define CALLBACK_EVENT      0x00050000l    /* dwCallback is an EVENT Handle */
#endif
typedef void (CALLBACK DRVCALLBACK)(HDRVR hdrvr, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2);

typedef DRVCALLBACK FAR *LPDRVCALLBACK;
#ifdef _WIN32
typedef DRVCALLBACK     *PDRVCALLBACK;
#endif

#ifndef MMNOTIMER
/****************************************************************************

                            Timer support

****************************************************************************/

/* timer error return values */
#define TIMERR_NOERROR        (0)                  /* no error */
#define TIMERR_NOCANDO        (TIMERR_BASE+1)      /* request not completed */
#define TIMERR_STRUCT         (TIMERR_BASE+33)     /* time struct size */

/* timer data types */
typedef void (CALLBACK TIMECALLBACK)(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2);

typedef TIMECALLBACK FAR *LPTIMECALLBACK;

/* flags for fuEvent parameter of timeSetEvent() function */
#define TIME_ONESHOT    0x0000   /* program timer for single event */
#define TIME_PERIODIC   0x0001   /* program for continuous periodic event */

#ifdef _WIN32
#define TIME_CALLBACK_FUNCTION      0x0000  /* callback is function */
#define TIME_CALLBACK_EVENT_SET     0x0010  /* callback is event - use SetEvent */
#define TIME_CALLBACK_EVENT_PULSE   0x0020  /* callback is event - use PulseEvent */
#endif

#if WINVER >= 0x0501
#define TIME_KILL_SYNCHRONOUS   0x0100  /* This flag prevents the event from occurring */
                                        /* after the user calls timeKillEvent() to */
                                        /* destroy it. */
#endif // WINVER >= 0x0501



/* timer device capabilities data structure */
typedef struct timecaps_tag {
    UINT    wPeriodMin;     /* minimum period supported  */
    UINT    wPeriodMax;     /* maximum period supported  */
} TIMECAPS, *PTIMECAPS, NEAR *NPTIMECAPS, FAR *LPTIMECAPS;

/* timer function prototypes */
MMRESULT WINAPI timeGetSystemTime( __out_bcount(cbmmt) LPMMTIME pmmt, __in UINT cbmmt);
DWORD WINAPI timeGetTime(void);
MMRESULT WINAPI timeSetEvent( __in UINT uDelay, __in UINT uResolution,
    __in LPTIMECALLBACK fptc, __in DWORD_PTR dwUser, __in UINT fuEvent);
MMRESULT WINAPI timeKillEvent( __in UINT uTimerID);
MMRESULT WINAPI timeGetDevCaps( __out_bcount(cbtc) LPTIMECAPS ptc, __in UINT cbtc);
MMRESULT WINAPI timeBeginPeriod( __in UINT uPeriod);
MMRESULT WINAPI timeEndPeriod( __in UINT uPeriod);

#endif  /* ifndef MMNOTIMER */

#ifndef MMNOMMSYSTEM
/****************************************************************************

                    General MMSYSTEM support

****************************************************************************/

#if (WINVER <= 0x030A)
UINT WINAPI mmsystemGetVersion(void);
#endif
#ifdef _WIN32
#define OutputDebugStr  OutputDebugString
#else
void WINAPI OutputDebugStr(LPCSTR);
#endif

#endif  /* ifndef MMNOMMSYSTEM */

#ifndef MMNOWAVE
/****************************************************************************

                        Waveform audio support

****************************************************************************/

/* waveform audio error return values */
#define WAVERR_BADFORMAT      (WAVERR_BASE + 0)    /* unsupported wave format */
#define WAVERR_STILLPLAYING   (WAVERR_BASE + 1)    /* still something playing */
#define WAVERR_UNPREPARED     (WAVERR_BASE + 2)    /* header not prepared */
#define WAVERR_SYNC           (WAVERR_BASE + 3)    /* device is synchronous */
#define WAVERR_LASTERROR      (WAVERR_BASE + 3)    /* last error in range */

/* waveform audio data types */
DECLARE_HANDLE(HWAVE);
DECLARE_HANDLE(HWAVEIN);
DECLARE_HANDLE(HWAVEOUT);
typedef HWAVEIN FAR *LPHWAVEIN;
typedef HWAVEOUT FAR *LPHWAVEOUT;
typedef DRVCALLBACK WAVECALLBACK;
typedef WAVECALLBACK FAR *LPWAVECALLBACK;

/* wave callback messages */
#define WOM_OPEN        MM_WOM_OPEN
#define WOM_CLOSE       MM_WOM_CLOSE
#define WOM_DONE        MM_WOM_DONE
#define WIM_OPEN        MM_WIM_OPEN
#define WIM_CLOSE       MM_WIM_CLOSE
#define WIM_DATA        MM_WIM_DATA

/* device ID for wave device mapper */
#define WAVE_MAPPER     ((UINT)-1)

/* flags for dwFlags parameter in waveOutOpen() and waveInOpen() */
#define  WAVE_FORMAT_QUERY         0x0001
#define  WAVE_ALLOWSYNC            0x0002
#if(WINVER >= 0x0400)
#define  WAVE_MAPPED               0x0004
#define  WAVE_FORMAT_DIRECT        0x0008
#define  WAVE_FORMAT_DIRECT_QUERY  (WAVE_FORMAT_QUERY | WAVE_FORMAT_DIRECT)
#endif /* WINVER >= 0x0400 */

/* wave data block header */
typedef struct wavehdr_tag {
    LPSTR       lpData;                 /* pointer to locked data buffer */
    DWORD       dwBufferLength;         /* length of data buffer */
    DWORD       dwBytesRecorded;        /* used for input only */
    DWORD_PTR   dwUser;                 /* for client's use */
    DWORD       dwFlags;                /* assorted flags (see defines) */
    DWORD       dwLoops;                /* loop control counter */
    struct wavehdr_tag FAR *lpNext;     /* reserved for driver */
    DWORD_PTR   reserved;               /* reserved for driver */
} WAVEHDR, *PWAVEHDR, NEAR *NPWAVEHDR, FAR *LPWAVEHDR;

/* flags for dwFlags field of WAVEHDR */
#define WHDR_DONE       0x00000001  /* done bit */
#define WHDR_PREPARED   0x00000002  /* set if this header has been prepared */
#define WHDR_BEGINLOOP  0x00000004  /* loop start block */
#define WHDR_ENDLOOP    0x00000008  /* loop end block */
#define WHDR_INQUEUE    0x00000010  /* reserved for driver */

/* waveform output device capabilities structure */
#ifdef _WIN32

typedef struct tagWAVEOUTCAPSA {
    WORD    wMid;                  /* manufacturer ID */
    WORD    wPid;                  /* product ID */
    MMVERSION vDriverVersion;      /* version of the driver */
    CHAR    szPname[MAXPNAMELEN];  /* product name (NULL terminated string) */
    DWORD   dwFormats;             /* formats supported */
    WORD    wChannels;             /* number of sources supported */
    WORD    wReserved1;            /* packing */
    DWORD   dwSupport;             /* functionality supported by driver */
} WAVEOUTCAPSA, *PWAVEOUTCAPSA, *NPWAVEOUTCAPSA, *LPWAVEOUTCAPSA;
typedef struct tagWAVEOUTCAPSW {
    WORD    wMid;                  /* manufacturer ID */
    WORD    wPid;                  /* product ID */
    MMVERSION vDriverVersion;      /* version of the driver */
    WCHAR   szPname[MAXPNAMELEN];  /* product name (NULL terminated string) */
    DWORD   dwFormats;             /* formats supported */
    WORD    wChannels;             /* number of sources supported */
    WORD    wReserved1;            /* packing */
    DWORD   dwSupport;             /* functionality supported by driver */
} WAVEOUTCAPSW, *PWAVEOUTCAPSW, *NPWAVEOUTCAPSW, *LPWAVEOUTCAPSW;
#ifdef UNICODE
typedef WAVEOUTCAPSW WAVEOUTCAPS;
typedef PWAVEOUTCAPSW PWAVEOUTCAPS;
typedef NPWAVEOUTCAPSW NPWAVEOUTCAPS;
typedef LPWAVEOUTCAPSW LPWAVEOUTCAPS;
#else
typedef WAVEOUTCAPSA WAVEOUTCAPS;
typedef PWAVEOUTCAPSA PWAVEOUTCAPS;
typedef NPWAVEOUTCAPSA NPWAVEOUTCAPS;
typedef LPWAVEOUTCAPSA LPWAVEOUTCAPS;
#endif // UNICODE
typedef struct tagWAVEOUTCAPS2A {
    WORD    wMid;                  /* manufacturer ID */
    WORD    wPid;                  /* product ID */
    MMVERSION vDriverVersion;      /* version of the driver */
    CHAR    szPname[MAXPNAMELEN];  /* product name (NULL terminated string) */
    DWORD   dwFormats;             /* formats supported */
    WORD    wChannels;             /* number of sources supported */
    WORD    wReserved1;            /* packing */
    DWORD   dwSupport;             /* functionality supported by driver */
    GUID    ManufacturerGuid;      /* for extensible MID mapping */
    GUID    ProductGuid;           /* for extensible PID mapping */
    GUID    NameGuid;              /* for name lookup in registry */
} WAVEOUTCAPS2A, *PWAVEOUTCAPS2A, *NPWAVEOUTCAPS2A, *LPWAVEOUTCAPS2A;
typedef struct tagWAVEOUTCAPS2W {
    WORD    wMid;                  /* manufacturer ID */
    WORD    wPid;                  /* product ID */
    MMVERSION vDriverVersion;      /* version of the driver */
    WCHAR   szPname[MAXPNAMELEN];  /* product name (NULL terminated string) */
    DWORD   dwFormats;             /* formats supported */
    WORD    wChannels;             /* number of sources supported */
    WORD    wReserved1;            /* packing */
    DWORD   dwSupport;             /* functionality supported by driver */
    GUID    ManufacturerGuid;      /* for extensible MID mapping */
    GUID    ProductGuid;           /* for extensible PID mapping */
    GUID    NameGuid;              /* for name lookup in registry */
} WAVEOUTCAPS2W, *PWAVEOUTCAPS2W, *NPWAVEOUTCAPS2W, *LPWAVEOUTCAPS2W;
#ifdef UNICODE
typedef WAVEOUTCAPS2W WAVEOUTCAPS2;
typedef PWAVEOUTCAPS2W PWAVEOUTCAPS2;
typedef NPWAVEOUTCAPS2W NPWAVEOUTCAPS2;
typedef LPWAVEOUTCAPS2W LPWAVEOUTCAPS2;
#else
typedef WAVEOUTCAPS2A WAVEOUTCAPS2;
typedef PWAVEOUTCAPS2A PWAVEOUTCAPS2;
typedef NPWAVEOUTCAPS2A NPWAVEOUTCAPS2;
typedef LPWAVEOUTCAPS2A LPWAVEOUTCAPS2;
#endif // UNICODE

#else
typedef struct waveoutcaps_tag {
    WORD    wMid;                  /* manufacturer ID */
    WORD    wPid;                  /* product ID */
    VERSION vDriverVersion;        /* version of the driver */
    char    szPname[MAXPNAMELEN];  /* product name (NULL terminated string) */
    DWORD   dwFormats;             /* formats supported */
    WORD    wChannels;             /* number of sources supported */
    DWORD   dwSupport;             /* functionality supported by driver */
} WAVEOUTCAPS, *PWAVEOUTCAPS, NEAR *NPWAVEOUTCAPS, FAR *LPWAVEOUTCAPS;
#endif

/* flags for dwSupport field of WAVEOUTCAPS */
#define WAVECAPS_PITCH          0x0001   /* supports pitch control */
#define WAVECAPS_PLAYBACKRATE   0x0002   /* supports playback rate control */
#define WAVECAPS_VOLUME         0x0004   /* supports volume control */
#define WAVECAPS_LRVOLUME       0x0008   /* separate left-right volume control */
#define WAVECAPS_SYNC           0x0010
#define WAVECAPS_SAMPLEACCURATE 0x0020


/* waveform input device capabilities structure */
#ifdef _WIN32

typedef struct tagWAVEINCAPSA {
    WORD    wMid;                    /* manufacturer ID */
    WORD    wPid;                    /* product ID */
    MMVERSION vDriverVersion;        /* version of the driver */
    CHAR    szPname[MAXPNAMELEN];    /* product name (NULL terminated string) */
    DWORD   dwFormats;               /* formats supported */
    WORD    wChannels;               /* number of channels supported */
    WORD    wReserved1;              /* structure packing */
} WAVEINCAPSA, *PWAVEINCAPSA, *NPWAVEINCAPSA, *LPWAVEINCAPSA;
typedef struct tagWAVEINCAPSW {
    WORD    wMid;                    /* manufacturer ID */
    WORD    wPid;                    /* product ID */
    MMVERSION vDriverVersion;        /* version of the driver */
    WCHAR   szPname[MAXPNAMELEN];    /* product name (NULL terminated string) */
    DWORD   dwFormats;               /* formats supported */
    WORD    wChannels;               /* number of channels supported */
    WORD    wReserved1;              /* structure packing */
} WAVEINCAPSW, *PWAVEINCAPSW, *NPWAVEINCAPSW, *LPWAVEINCAPSW;
#ifdef UNICODE
typedef WAVEINCAPSW WAVEINCAPS;
typedef PWAVEINCAPSW PWAVEINCAPS;
typedef NPWAVEINCAPSW NPWAVEINCAPS;
typedef LPWAVEINCAPSW LPWAVEINCAPS;
#else
typedef WAVEINCAPSA WAVEINCAPS;
typedef PWAVEINCAPSA PWAVEINCAPS;
typedef NPWAVEINCAPSA NPWAVEINCAPS;
typedef LPWAVEINCAPSA LPWAVEINCAPS;
#endif // UNICODE
typedef struct tagWAVEINCAPS2A {
    WORD    wMid;                    /* manufacturer ID */
    WORD    wPid;                    /* product ID */
    MMVERSION vDriverVersion;        /* version of the driver */
    CHAR    szPname[MAXPNAMELEN];    /* product name (NULL terminated string) */
    DWORD   dwFormats;               /* formats supported */
    WORD    wChannels;               /* number of channels supported */
    WORD    wReserved1;              /* structure packing */
    GUID    ManufacturerGuid;        /* for extensible MID mapping */
    GUID    ProductGuid;             /* for extensible PID mapping */
    GUID    NameGuid;                /* for name lookup in registry */
} WAVEINCAPS2A, *PWAVEINCAPS2A, *NPWAVEINCAPS2A, *LPWAVEINCAPS2A;
typedef struct tagWAVEINCAPS2W {
    WORD    wMid;                    /* manufacturer ID */
    WORD    wPid;                    /* product ID */
    MMVERSION vDriverVersion;        /* version of the driver */
    WCHAR   szPname[MAXPNAMELEN];    /* product name (NULL terminated string) */
    DWORD   dwFormats;               /* formats supported */
    WORD    wChannels;               /* number of channels supported */
    WORD    wReserved1;              /* structure packing */
    GUID    ManufacturerGuid;        /* for extensible MID mapping */
    GUID    ProductGuid;             /* for extensible PID mapping */
    GUID    NameGuid;                /* for name lookup in registry */
} WAVEINCAPS2W, *PWAVEINCAPS2W, *NPWAVEINCAPS2W, *LPWAVEINCAPS2W;
#ifdef UNICODE
typedef WAVEINCAPS2W WAVEINCAPS2;
typedef PWAVEINCAPS2W PWAVEINCAPS2;
typedef NPWAVEINCAPS2W NPWAVEINCAPS2;
typedef LPWAVEINCAPS2W LPWAVEINCAPS2;
#else
typedef WAVEINCAPS2A WAVEINCAPS2;
typedef PWAVEINCAPS2A PWAVEINCAPS2;
typedef NPWAVEINCAPS2A NPWAVEINCAPS2;
typedef LPWAVEINCAPS2A LPWAVEINCAPS2;
#endif // UNICODE

#else
typedef struct waveincaps_tag {
    WORD    wMid;                    /* manufacturer ID */
    WORD    wPid;                    /* product ID */
    VERSION vDriverVersion;          /* version of the driver */
    char    szPname[MAXPNAMELEN];    /* product name (NULL terminated string) */
    DWORD   dwFormats;               /* formats supported */
    WORD    wChannels;               /* number of channels supported */
} WAVEINCAPS, *PWAVEINCAPS, NEAR *NPWAVEINCAPS, FAR *LPWAVEINCAPS;
#endif

/* defines for dwFormat field of WAVEINCAPS and WAVEOUTCAPS */
#define WAVE_INVALIDFORMAT     0x00000000       /* invalid format */
#define WAVE_FORMAT_1M08       0x00000001       /* 11.025 kHz, Mono,   8-bit  */
#define WAVE_FORMAT_1S08       0x00000002       /* 11.025 kHz, Stereo, 8-bit  */
#define WAVE_FORMAT_1M16       0x00000004       /* 11.025 kHz, Mono,   16-bit */
#define WAVE_FORMAT_1S16       0x00000008       /* 11.025 kHz, Stereo, 16-bit */
#define WAVE_FORMAT_2M08       0x00000010       /* 22.05  kHz, Mono,   8-bit  */
#define WAVE_FORMAT_2S08       0x00000020       /* 22.05  kHz, Stereo, 8-bit  */
#define WAVE_FORMAT_2M16       0x00000040       /* 22.05  kHz, Mono,   16-bit */
#define WAVE_FORMAT_2S16       0x00000080       /* 22.05  kHz, Stereo, 16-bit */
#define WAVE_FORMAT_4M08       0x00000100       /* 44.1   kHz, Mono,   8-bit  */
#define WAVE_FORMAT_4S08       0x00000200       /* 44.1   kHz, Stereo, 8-bit  */
#define WAVE_FORMAT_4M16       0x00000400       /* 44.1   kHz, Mono,   16-bit */
#define WAVE_FORMAT_4S16       0x00000800       /* 44.1   kHz, Stereo, 16-bit */

#define WAVE_FORMAT_44M08      0x00000100       /* 44.1   kHz, Mono,   8-bit  */
#define WAVE_FORMAT_44S08      0x00000200       /* 44.1   kHz, Stereo, 8-bit  */
#define WAVE_FORMAT_44M16      0x00000400       /* 44.1   kHz, Mono,   16-bit */
#define WAVE_FORMAT_44S16      0x00000800       /* 44.1   kHz, Stereo, 16-bit */
#define WAVE_FORMAT_48M08      0x00001000       /* 48     kHz, Mono,   8-bit  */
#define WAVE_FORMAT_48S08      0x00002000       /* 48     kHz, Stereo, 8-bit  */
#define WAVE_FORMAT_48M16      0x00004000       /* 48     kHz, Mono,   16-bit */
#define WAVE_FORMAT_48S16      0x00008000       /* 48     kHz, Stereo, 16-bit */
#define WAVE_FORMAT_96M08      0x00010000       /* 96     kHz, Mono,   8-bit  */
#define WAVE_FORMAT_96S08      0x00020000       /* 96     kHz, Stereo, 8-bit  */
#define WAVE_FORMAT_96M16      0x00040000       /* 96     kHz, Mono,   16-bit */
#define WAVE_FORMAT_96S16      0x00080000       /* 96     kHz, Stereo, 16-bit */


#ifndef WAVE_FORMAT_PCM

/* OLD general waveform format structure (information common to all formats) */
typedef struct waveformat_tag {
    WORD    wFormatTag;        /* format type */
    WORD    nChannels;         /* number of channels (i.e. mono, stereo, etc.) */
    DWORD   nSamplesPerSec;    /* sample rate */
    DWORD   nAvgBytesPerSec;   /* for buffer estimation */
    WORD    nBlockAlign;       /* block size of data */
} WAVEFORMAT, *PWAVEFORMAT, NEAR *NPWAVEFORMAT, FAR *LPWAVEFORMAT;

/* flags for wFormatTag field of WAVEFORMAT */
#define WAVE_FORMAT_PCM     1


/* specific waveform format structure for PCM data */
typedef struct pcmwaveformat_tag {
    WAVEFORMAT  wf;
    WORD        wBitsPerSample;
} PCMWAVEFORMAT, *PPCMWAVEFORMAT, NEAR *NPPCMWAVEFORMAT, FAR *LPPCMWAVEFORMAT;
#endif /* WAVE_FORMAT_PCM */

#ifndef _WAVEFORMATEX_
#define _WAVEFORMATEX_

/*
 *  extended waveform format structure used for all non-PCM formats. this
 *  structure is common to all non-PCM formats.
 */
typedef struct tWAVEFORMATEX
{
    WORD        wFormatTag;         /* format type */
    WORD        nChannels;          /* number of channels (i.e. mono, stereo...) */
    DWORD       nSamplesPerSec;     /* sample rate */
    DWORD       nAvgBytesPerSec;    /* for buffer estimation */
    WORD        nBlockAlign;        /* block size of data */
    WORD        wBitsPerSample;     /* number of bits per sample of mono data */
    WORD        cbSize;             /* the count in bytes of the size of */
                                    /* extra information (after cbSize) */
} WAVEFORMATEX, *PWAVEFORMATEX, NEAR *NPWAVEFORMATEX, FAR *LPWAVEFORMATEX;

#endif /* _WAVEFORMATEX_ */
typedef const WAVEFORMATEX FAR *LPCWAVEFORMATEX;

/* waveform audio function prototypes */
UINT WINAPI waveOutGetNumDevs(void);

#ifdef _WIN32

MMRESULT WINAPI waveOutGetDevCapsA( __in UINT_PTR uDeviceID, __out LPWAVEOUTCAPSA pwoc, __in UINT cbwoc);
MMRESULT WINAPI waveOutGetDevCapsW( __in UINT_PTR uDeviceID, __out LPWAVEOUTCAPSW pwoc, __in UINT cbwoc);
#ifdef UNICODE
#define waveOutGetDevCaps  waveOutGetDevCapsW
#else
#define waveOutGetDevCaps  waveOutGetDevCapsA
#endif // !UNICODE

#else
MMRESULT WINAPI waveOutGetDevCaps( UINT uDeviceID, LPWAVEOUTCAPS pwoc, UINT cbwoc);
#endif

#if (WINVER >= 0x0400)
MMRESULT WINAPI waveOutGetVolume( __in_opt HWAVEOUT hwo, __out LPDWORD pdwVolume);
MMRESULT WINAPI waveOutSetVolume( __in_opt HWAVEOUT hwo, __in DWORD dwVolume);
#else
MMRESULT WINAPI waveOutGetVolume(UINT uId, LPDWORD pdwVolume);
MMRESULT WINAPI waveOutSetVolume(UINT uId, DWORD dwVolume);
#endif

#ifdef _WIN32

MMRESULT WINAPI waveOutGetErrorTextA( __in MMRESULT mmrError, __out_ecount(cchText) LPSTR pszText, __in UINT cchText);
MMRESULT WINAPI waveOutGetErrorTextW( __in MMRESULT mmrError, __out_ecount(cchText) LPWSTR pszText, __in UINT cchText);
#ifdef UNICODE
#define waveOutGetErrorText  waveOutGetErrorTextW
#else
#define waveOutGetErrorText  waveOutGetErrorTextA
#endif // !UNICODE

#else
MMRESULT WINAPI waveOutGetErrorText(MMRESULT mmrError, LPSTR pszText, UINT cchText);
#endif

MMRESULT WINAPI waveOutOpen( __out_opt LPHWAVEOUT phwo, __in UINT uDeviceID,
    __in LPCWAVEFORMATEX pwfx, __in_opt DWORD_PTR dwCallback, __in_opt DWORD_PTR dwInstance, __in DWORD fdwOpen);

MMRESULT WINAPI waveOutClose( __in HWAVEOUT hwo);
MMRESULT WINAPI waveOutPrepareHeader( __in HWAVEOUT hwo, __inout_bcount(cbwh) LPWAVEHDR pwh,  __in UINT cbwh);
MMRESULT WINAPI waveOutUnprepareHeader( __in HWAVEOUT hwo, __inout_bcount(cbwh) LPWAVEHDR pwh, __in UINT cbwh);
MMRESULT WINAPI waveOutWrite( __in HWAVEOUT hwo, __inout_bcount(cbwh) LPWAVEHDR pwh, __in UINT cbwh);
MMRESULT WINAPI waveOutPause( __in HWAVEOUT hwo);
MMRESULT WINAPI waveOutRestart( __in HWAVEOUT hwo);
MMRESULT WINAPI waveOutReset( __in HWAVEOUT hwo);
MMRESULT WINAPI waveOutGetPosition( __in HWAVEOUT hwo, __inout_bcount(cbmmt) LPMMTIME pmmt, __in UINT cbmmt);
MMRESULT WINAPI waveOutBreakLoop( __in HWAVEOUT hwo);
MMRESULT WINAPI waveOutGetPitch( __in HWAVEOUT hwo, __out LPDWORD pdwPitch);
MMRESULT WINAPI waveOutSetPitch( __in HWAVEOUT hwo, __in DWORD dwPitch);
MMRESULT WINAPI waveOutGetPlaybackRate( __in HWAVEOUT hwo, __out LPDWORD pdwRate);
MMRESULT WINAPI waveOutSetPlaybackRate( __in HWAVEOUT hwo, __in DWORD dwRate);
MMRESULT WINAPI waveOutGetID( __in HWAVEOUT hwo, __out LPUINT puDeviceID);

#if (WINVER >= 0x030a)
#ifdef _WIN32
MMRESULT WINAPI waveOutMessage( __in_opt HWAVEOUT hwo, __in UINT uMsg, __in DWORD_PTR dw1, __in DWORD_PTR dw2);
#else
DWORD WINAPI waveOutMessage(HWAVEOUT hwo, UINT uMsg, DWORD dw1, DWORD dw2);
#endif
#endif /* ifdef WINVER >= 0x030a */

 UINT WINAPI waveInGetNumDevs(void);

#ifdef _WIN32

MMRESULT WINAPI waveInGetDevCapsA( __in UINT_PTR uDeviceID, __out_bcount(cbwic) LPWAVEINCAPSA pwic, __in UINT cbwic);
MMRESULT WINAPI waveInGetDevCapsW( __in UINT_PTR uDeviceID, __out_bcount(cbwic) LPWAVEINCAPSW pwic, __in UINT cbwic);
#ifdef UNICODE
#define waveInGetDevCaps  waveInGetDevCapsW
#else
#define waveInGetDevCaps  waveInGetDevCapsA
#endif // !UNICODE

#else
MMRESULT WINAPI waveInGetDevCaps(UINT uDeviceID, LPWAVEINCAPS pwic, UINT cbwic);
#endif

#ifdef _WIN32

MMRESULT WINAPI waveInGetErrorTextA(__in MMRESULT mmrError, __out_ecount(cchText) LPSTR pszText, __in UINT cchText);
MMRESULT WINAPI waveInGetErrorTextW(__in MMRESULT mmrError, __out_ecount(cchText) LPWSTR pszText, __in UINT cchText);
#ifdef UNICODE
#define waveInGetErrorText  waveInGetErrorTextW
#else
#define waveInGetErrorText  waveInGetErrorTextA
#endif // !UNICODE

#else
MMRESULT WINAPI waveInGetErrorText(MMRESULT mmrError, LPSTR pszText, UINT cchText);
#endif

MMRESULT WINAPI waveInOpen( __out_opt LPHWAVEIN phwi, __in UINT uDeviceID,
    __in LPCWAVEFORMATEX pwfx, __in_opt DWORD_PTR dwCallback, __in_opt DWORD_PTR dwInstance, __in DWORD fdwOpen);

MMRESULT WINAPI waveInClose( __in HWAVEIN hwi);
MMRESULT WINAPI waveInPrepareHeader( __in HWAVEIN hwi, __inout_bcount(cbwh) LPWAVEHDR pwh, __in UINT cbwh);
MMRESULT WINAPI waveInUnprepareHeader( __in HWAVEIN hwi, __inout_bcount(cbwh) LPWAVEHDR pwh, __in UINT cbwh);
MMRESULT WINAPI waveInAddBuffer( __in HWAVEIN hwi, __inout_bcount(cbwh) LPWAVEHDR pwh, __in UINT cbwh);
MMRESULT WINAPI waveInStart( __in HWAVEIN hwi);
MMRESULT WINAPI waveInStop( __in HWAVEIN hwi);
MMRESULT WINAPI waveInReset( __in HWAVEIN hwi);
MMRESULT WINAPI waveInGetPosition( __in HWAVEIN hwi, __inout_bcount(cbmmt) LPMMTIME pmmt, __in UINT cbmmt);
MMRESULT WINAPI waveInGetID( __in HWAVEIN hwi, __in LPUINT puDeviceID);

#if (WINVER >= 0x030a)
#ifdef _WIN32
MMRESULT WINAPI waveInMessage( __in_opt HWAVEIN hwi, __in UINT uMsg, __in_opt DWORD_PTR dw1, __in_opt DWORD_PTR dw2);
#else
DWORD WINAPI waveInMessage(HWAVEIN hwi, UINT uMsg, DWORD dw1, DWORD dw2);
#endif
#endif /* ifdef WINVER >= 0x030a */

#endif  /* ifndef MMNOWAVE */

#ifndef MMNOMIDI
/****************************************************************************

                            MIDI audio support

****************************************************************************/

/* MIDI error return values */
#define MIDIERR_UNPREPARED    (MIDIERR_BASE + 0)   /* header not prepared */
#define MIDIERR_STILLPLAYING  (MIDIERR_BASE + 1)   /* still something playing */
#define MIDIERR_NOMAP         (MIDIERR_BASE + 2)   /* no configured instruments */
#define MIDIERR_NOTREADY      (MIDIERR_BASE + 3)   /* hardware is still busy */
#define MIDIERR_NODEVICE      (MIDIERR_BASE + 4)   /* port no longer connected */
#define MIDIERR_INVALIDSETUP  (MIDIERR_BASE + 5)   /* invalid MIF */
#define MIDIERR_BADOPENMODE   (MIDIERR_BASE + 6)   /* operation unsupported w/ open mode */
#define MIDIERR_DONT_CONTINUE (MIDIERR_BASE + 7)   /* thru device 'eating' a message */
#define MIDIERR_LASTERROR     (MIDIERR_BASE + 7)   /* last error in range */

/* MIDI audio data types */
DECLARE_HANDLE(HMIDI);
DECLARE_HANDLE(HMIDIIN);
DECLARE_HANDLE(HMIDIOUT);
DECLARE_HANDLE(HMIDISTRM);
typedef HMIDI FAR *LPHMIDI;
typedef HMIDIIN FAR *LPHMIDIIN;
typedef HMIDIOUT FAR *LPHMIDIOUT;
typedef HMIDISTRM FAR *LPHMIDISTRM;
typedef DRVCALLBACK MIDICALLBACK;
typedef MIDICALLBACK FAR *LPMIDICALLBACK;
#define MIDIPATCHSIZE   128
typedef WORD PATCHARRAY[MIDIPATCHSIZE];
typedef WORD FAR *LPPATCHARRAY;
typedef WORD KEYARRAY[MIDIPATCHSIZE];
typedef WORD FAR *LPKEYARRAY;

/* MIDI callback messages */
#define MIM_OPEN        MM_MIM_OPEN
#define MIM_CLOSE       MM_MIM_CLOSE
#define MIM_DATA        MM_MIM_DATA
#define MIM_LONGDATA    MM_MIM_LONGDATA
#define MIM_ERROR       MM_MIM_ERROR
#define MIM_LONGERROR   MM_MIM_LONGERROR
#define MOM_OPEN        MM_MOM_OPEN
#define MOM_CLOSE       MM_MOM_CLOSE
#define MOM_DONE        MM_MOM_DONE

#if(WINVER >= 0x0400)
#define MIM_MOREDATA      MM_MIM_MOREDATA
#define MOM_POSITIONCB    MM_MOM_POSITIONCB
#endif /* WINVER >= 0x0400 */

/* device ID for MIDI mapper */
#define MIDIMAPPER     ((UINT)-1)
#define MIDI_MAPPER    ((UINT)-1)

#if(WINVER >= 0x0400)
/* flags for dwFlags parm of midiInOpen() */
#define MIDI_IO_STATUS      0x00000020L
#endif /* WINVER >= 0x0400 */

/* flags for wFlags parm of midiOutCachePatches(), midiOutCacheDrumPatches() */
#define MIDI_CACHE_ALL      1
#define MIDI_CACHE_BESTFIT  2
#define MIDI_CACHE_QUERY    3
#define MIDI_UNCACHE        4

/* MIDI output device capabilities structure */
#ifdef _WIN32

typedef struct tagMIDIOUTCAPSA {
    WORD    wMid;                  /* manufacturer ID */
    WORD    wPid;                  /* product ID */
    MMVERSION vDriverVersion;      /* version of the driver */
    CHAR    szPname[MAXPNAMELEN];  /* product name (NULL terminated string) */
    WORD    wTechnology;           /* type of device */
    WORD    wVoices;               /* # of voices (internal synth only) */
    WORD    wNotes;                /* max # of notes (internal synth only) */
    WORD    wChannelMask;          /* channels used (internal synth only) */
    DWORD   dwSupport;             /* functionality supported by driver */
} MIDIOUTCAPSA, *PMIDIOUTCAPSA, *NPMIDIOUTCAPSA, *LPMIDIOUTCAPSA;
typedef struct tagMIDIOUTCAPSW {
    WORD    wMid;                  /* manufacturer ID */
    WORD    wPid;                  /* product ID */
    MMVERSION vDriverVersion;      /* version of the driver */
    WCHAR   szPname[MAXPNAMELEN];  /* product name (NULL terminated string) */
    WORD    wTechnology;           /* type of device */
    WORD    wVoices;               /* # of voices (internal synth only) */
    WORD    wNotes;                /* max # of notes (internal synth only) */
    WORD    wChannelMask;          /* channels used (internal synth only) */
    DWORD   dwSupport;             /* functionality supported by driver */
} MIDIOUTCAPSW, *PMIDIOUTCAPSW, *NPMIDIOUTCAPSW, *LPMIDIOUTCAPSW;
#ifdef UNICODE
typedef MIDIOUTCAPSW MIDIOUTCAPS;
typedef PMIDIOUTCAPSW PMIDIOUTCAPS;
typedef NPMIDIOUTCAPSW NPMIDIOUTCAPS;
typedef LPMIDIOUTCAPSW LPMIDIOUTCAPS;
#else
typedef MIDIOUTCAPSA MIDIOUTCAPS;
typedef PMIDIOUTCAPSA PMIDIOUTCAPS;
typedef NPMIDIOUTCAPSA NPMIDIOUTCAPS;
typedef LPMIDIOUTCAPSA LPMIDIOUTCAPS;
#endif // UNICODE
typedef struct tagMIDIOUTCAPS2A {
    WORD    wMid;                  /* manufacturer ID */
    WORD    wPid;                  /* product ID */
    MMVERSION vDriverVersion;      /* version of the driver */
    CHAR    szPname[MAXPNAMELEN];  /* product name (NULL terminated string) */
    WORD    wTechnology;           /* type of device */
    WORD    wVoices;               /* # of voices (internal synth only) */
    WORD    wNotes;                /* max # of notes (internal synth only) */
    WORD    wChannelMask;          /* channels used (internal synth only) */
    DWORD   dwSupport;             /* functionality supported by driver */
    GUID    ManufacturerGuid;      /* for extensible MID mapping */
    GUID    ProductGuid;           /* for extensible PID mapping */
    GUID    NameGuid;              /* for name lookup in registry */
} MIDIOUTCAPS2A, *PMIDIOUTCAPS2A, *NPMIDIOUTCAPS2A, *LPMIDIOUTCAPS2A;
typedef struct tagMIDIOUTCAPS2W {
    WORD    wMid;                  /* manufacturer ID */
    WORD    wPid;                  /* product ID */
    MMVERSION vDriverVersion;      /* version of the driver */
    WCHAR   szPname[MAXPNAMELEN];  /* product name (NULL terminated string) */
    WORD    wTechnology;           /* type of device */
    WORD    wVoices;               /* # of voices (internal synth only) */
    WORD    wNotes;                /* max # of notes (internal synth only) */
    WORD    wChannelMask;          /* channels used (internal synth only) */
    DWORD   dwSupport;             /* functionality supported by driver */
    GUID    ManufacturerGuid;      /* for extensible MID mapping */
    GUID    ProductGuid;           /* for extensible PID mapping */
    GUID    NameGuid;              /* for name lookup in registry */
} MIDIOUTCAPS2W, *PMIDIOUTCAPS2W, *NPMIDIOUTCAPS2W, *LPMIDIOUTCAPS2W;
#ifdef UNICODE
typedef MIDIOUTCAPS2W MIDIOUTCAPS2;
typedef PMIDIOUTCAPS2W PMIDIOUTCAPS2;
typedef NPMIDIOUTCAPS2W NPMIDIOUTCAPS2;
typedef LPMIDIOUTCAPS2W LPMIDIOUTCAPS2;
#else
typedef MIDIOUTCAPS2A MIDIOUTCAPS2;
typedef PMIDIOUTCAPS2A PMIDIOUTCAPS2;
typedef NPMIDIOUTCAPS2A NPMIDIOUTCAPS2;
typedef LPMIDIOUTCAPS2A LPMIDIOUTCAPS2;
#endif // UNICODE

#else
typedef struct midioutcaps_tag {
    WORD    wMid;                  /* manufacturer ID */
    WORD    wPid;                  /* product ID */
    VERSION vDriverVersion;        /* version of the driver */
    char    szPname[MAXPNAMELEN];  /* product name (NULL terminated string) */
    WORD    wTechnology;           /* type of device */
    WORD    wVoices;               /* # of voices (internal synth only) */
    WORD    wNotes;                /* max # of notes (internal synth only) */
    WORD    wChannelMask;          /* channels used (internal synth only) */
    DWORD   dwSupport;             /* functionality supported by driver */
} MIDIOUTCAPS, *PMIDIOUTCAPS, NEAR *NPMIDIOUTCAPS, FAR *LPMIDIOUTCAPS;
#endif

/* flags for wTechnology field of MIDIOUTCAPS structure */
#define MOD_MIDIPORT    1  /* output port */
#define MOD_SYNTH       2  /* generic internal synth */
#define MOD_SQSYNTH     3  /* square wave internal synth */
#define MOD_FMSYNTH     4  /* FM internal synth */
#define MOD_MAPPER      5  /* MIDI mapper */
#define MOD_WAVETABLE   6  /* hardware wavetable synth */
#define MOD_SWSYNTH     7  /* software synth */

/* flags for dwSupport field of MIDIOUTCAPS structure */
#define MIDICAPS_VOLUME          0x0001  /* supports volume control */
#define MIDICAPS_LRVOLUME        0x0002  /* separate left-right volume control */
#define MIDICAPS_CACHE           0x0004
#if(WINVER >= 0x0400)
#define MIDICAPS_STREAM          0x0008  /* driver supports midiStreamOut directly */
#endif /* WINVER >= 0x0400 */

/* MIDI input device capabilities structure */
#ifdef _WIN32

typedef struct tagMIDIINCAPSA {
    WORD        wMid;                   /* manufacturer ID */
    WORD        wPid;                   /* product ID */
    MMVERSION   vDriverVersion;         /* version of the driver */
    CHAR        szPname[MAXPNAMELEN];   /* product name (NULL terminated string) */
#if (WINVER >= 0x0400)
    DWORD   dwSupport;             /* functionality supported by driver */
#endif
} MIDIINCAPSA, *PMIDIINCAPSA, *NPMIDIINCAPSA, *LPMIDIINCAPSA;
typedef struct tagMIDIINCAPSW {
    WORD        wMid;                   /* manufacturer ID */
    WORD        wPid;                   /* product ID */
    MMVERSION   vDriverVersion;         /* version of the driver */
    WCHAR       szPname[MAXPNAMELEN];   /* product name (NULL terminated string) */
#if (WINVER >= 0x0400)
    DWORD   dwSupport;             /* functionality supported by driver */
#endif
} MIDIINCAPSW, *PMIDIINCAPSW, *NPMIDIINCAPSW, *LPMIDIINCAPSW;
#ifdef UNICODE
typedef MIDIINCAPSW MIDIINCAPS;
typedef PMIDIINCAPSW PMIDIINCAPS;
typedef NPMIDIINCAPSW NPMIDIINCAPS;
typedef LPMIDIINCAPSW LPMIDIINCAPS;
#else
typedef MIDIINCAPSA MIDIINCAPS;
typedef PMIDIINCAPSA PMIDIINCAPS;
typedef NPMIDIINCAPSA NPMIDIINCAPS;
typedef LPMIDIINCAPSA LPMIDIINCAPS;
#endif // UNICODE
typedef struct tagMIDIINCAPS2A {
    WORD        wMid;                   /* manufacturer ID */
    WORD        wPid;                   /* product ID */
    MMVERSION   vDriverVersion;         /* version of the driver */
    CHAR        szPname[MAXPNAMELEN];   /* product name (NULL terminated string) */
#if (WINVER >= 0x0400)
    DWORD       dwSupport;              /* functionality supported by driver */
#endif
    GUID        ManufacturerGuid;       /* for extensible MID mapping */
    GUID        ProductGuid;            /* for extensible PID mapping */
    GUID        NameGuid;               /* for name lookup in registry */
} MIDIINCAPS2A, *PMIDIINCAPS2A, *NPMIDIINCAPS2A, *LPMIDIINCAPS2A;
typedef struct tagMIDIINCAPS2W {
    WORD        wMid;                   /* manufacturer ID */
    WORD        wPid;                   /* product ID */
    MMVERSION   vDriverVersion;         /* version of the driver */
    WCHAR       szPname[MAXPNAMELEN];   /* product name (NULL terminated string) */
#if (WINVER >= 0x0400)
    DWORD       dwSupport;              /* functionality supported by driver */
#endif
    GUID        ManufacturerGuid;       /* for extensible MID mapping */
    GUID        ProductGuid;            /* for extensible PID mapping */
    GUID        NameGuid;               /* for name lookup in registry */
} MIDIINCAPS2W, *PMIDIINCAPS2W, *NPMIDIINCAPS2W, *LPMIDIINCAPS2W;
#ifdef UNICODE
typedef MIDIINCAPS2W MIDIINCAPS2;
typedef PMIDIINCAPS2W PMIDIINCAPS2;
typedef NPMIDIINCAPS2W NPMIDIINCAPS2;
typedef LPMIDIINCAPS2W LPMIDIINCAPS2;
#else
typedef MIDIINCAPS2A MIDIINCAPS2;
typedef PMIDIINCAPS2A PMIDIINCAPS2;
typedef NPMIDIINCAPS2A NPMIDIINCAPS2;
typedef LPMIDIINCAPS2A LPMIDIINCAPS2;
#endif // UNICODE

#else
typedef struct midiincaps_tag {
    WORD    wMid;                  /* manufacturer ID */
    WORD    wPid;                  /* product ID */
    VERSION vDriverVersion;        /* version of the driver */
    char    szPname[MAXPNAMELEN];  /* product name (NULL terminated string) */
#if (WINVER >= 0x0400)
    DWORD   dwSupport;             /* functionality supported by driver */
#endif
} MIDIINCAPS, *PMIDIINCAPS, NEAR *NPMIDIINCAPS, FAR *LPMIDIINCAPS;
#endif


/* MIDI data block header */
typedef struct midihdr_tag {
    LPSTR       lpData;               /* pointer to locked data block */
    DWORD       dwBufferLength;       /* length of data in data block */
    DWORD       dwBytesRecorded;      /* used for input only */
    DWORD_PTR   dwUser;               /* for client's use */
    DWORD       dwFlags;              /* assorted flags (see defines) */
    struct midihdr_tag far *lpNext;   /* reserved for driver */
    DWORD_PTR   reserved;             /* reserved for driver */
#if (WINVER >= 0x0400)
    DWORD       dwOffset;             /* Callback offset into buffer */
    DWORD_PTR   dwReserved[8];        /* Reserved for MMSYSTEM */
#endif
} MIDIHDR, *PMIDIHDR, NEAR *NPMIDIHDR, FAR *LPMIDIHDR;


#if(WINVER >= 0x0400)
typedef struct midievent_tag
{
    DWORD       dwDeltaTime;          /* Ticks since last event */
    DWORD       dwStreamID;           /* Reserved; must be zero */
    DWORD       dwEvent;              /* Event type and parameters */
    DWORD       dwParms[1];           /* Parameters if this is a long event */
} MIDIEVENT;

typedef struct midistrmbuffver_tag
{
    DWORD       dwVersion;                  /* Stream buffer format version */
    DWORD       dwMid;                      /* Manufacturer ID as defined in MMREG.H */
    DWORD       dwOEMVersion;               /* Manufacturer version for custom ext */
} MIDISTRMBUFFVER;
#endif /* WINVER >= 0x0400 */

/* flags for dwFlags field of MIDIHDR structure */
#define MHDR_DONE       0x00000001       /* done bit */
#define MHDR_PREPARED   0x00000002       /* set if header prepared */
#define MHDR_INQUEUE    0x00000004       /* reserved for driver */
#define MHDR_ISSTRM     0x00000008       /* Buffer is stream buffer */
#if(WINVER >= 0x0400)
/* */
/* Type codes which go in the high byte of the event DWORD of a stream buffer */
/* */
/* Type codes 00-7F contain parameters within the low 24 bits */
/* Type codes 80-FF contain a length of their parameter in the low 24 */
/* bits, followed by their parameter data in the buffer. The event */
/* DWORD contains the exact byte length; the parm data itself must be */
/* padded to be an even multiple of 4 bytes long. */
/* */

#define MEVT_F_SHORT        0x00000000L
#define MEVT_F_LONG         0x80000000L
#define MEVT_F_CALLBACK     0x40000000L

#define MEVT_EVENTTYPE(x)   ((BYTE)(((x)>>24)&0xFF))
#define MEVT_EVENTPARM(x)   ((DWORD)((x)&0x00FFFFFFL))

#define MEVT_SHORTMSG       ((BYTE)0x00)    /* parm = shortmsg for midiOutShortMsg */
#define MEVT_TEMPO          ((BYTE)0x01)    /* parm = new tempo in microsec/qn     */
#define MEVT_NOP            ((BYTE)0x02)    /* parm = unused; does nothing         */

/* 0x04-0x7F reserved */

#define MEVT_LONGMSG        ((BYTE)0x80)    /* parm = bytes to send verbatim       */
#define MEVT_COMMENT        ((BYTE)0x82)    /* parm = comment data                 */
#define MEVT_VERSION        ((BYTE)0x84)    /* parm = MIDISTRMBUFFVER struct       */

/* 0x81-0xFF reserved */

#define MIDISTRM_ERROR      (-2)

/* */
/* Structures and defines for midiStreamProperty */
/* */
#define MIDIPROP_SET        0x80000000L
#define MIDIPROP_GET        0x40000000L

/* These are intentionally both non-zero so the app cannot accidentally */
/* leave the operation off and happen to appear to work due to default */
/* action. */

#define MIDIPROP_TIMEDIV    0x00000001L
#define MIDIPROP_TEMPO      0x00000002L

typedef struct midiproptimediv_tag
{
    DWORD       cbStruct;
    DWORD       dwTimeDiv;
} MIDIPROPTIMEDIV, FAR *LPMIDIPROPTIMEDIV;

typedef struct midiproptempo_tag
{
    DWORD       cbStruct;
    DWORD       dwTempo;
} MIDIPROPTEMPO, FAR *LPMIDIPROPTEMPO;

#endif /* WINVER >= 0x0400 */

/* MIDI function prototypes */
UINT WINAPI midiOutGetNumDevs(void);
#if(WINVER >= 0x0400)
MMRESULT WINAPI midiStreamOpen( __out LPHMIDISTRM phms, __inout_ecount(cMidi) LPUINT puDeviceID, __in DWORD cMidi, __in_opt DWORD_PTR dwCallback, __in_opt DWORD_PTR dwInstance, __in DWORD fdwOpen);
MMRESULT WINAPI midiStreamClose( __in HMIDISTRM hms);

MMRESULT WINAPI midiStreamProperty( __in HMIDISTRM hms, __inout_bcount(sizeof(DWORD)+sizeof(DWORD)) LPBYTE lppropdata, __in DWORD dwProperty);
MMRESULT WINAPI midiStreamPosition( __in HMIDISTRM hms, __out_bcount(cbmmt) LPMMTIME lpmmt, __in UINT cbmmt);

MMRESULT WINAPI midiStreamOut( __in HMIDISTRM hms, __out_bcount(cbmh) LPMIDIHDR pmh, __in UINT cbmh);
MMRESULT WINAPI midiStreamPause( __in HMIDISTRM hms);
MMRESULT WINAPI midiStreamRestart( __in HMIDISTRM hms);
MMRESULT WINAPI midiStreamStop( __in HMIDISTRM hms);

#ifdef _WIN32
MMRESULT WINAPI midiConnect( __in HMIDI hmi, __in HMIDIOUT hmo, __in_opt LPVOID pReserved);
MMRESULT WINAPI midiDisconnect( __in HMIDI hmi, __in HMIDIOUT hmo, __in_opt LPVOID pReserved);
#endif
#endif /* WINVER >= 0x0400 */

#ifdef _WIN32

MMRESULT WINAPI midiOutGetDevCapsA( __in UINT_PTR uDeviceID, __out_bcount(cbmoc) LPMIDIOUTCAPSA pmoc, __in UINT cbmoc);
MMRESULT WINAPI midiOutGetDevCapsW( __in UINT_PTR uDeviceID, __out_bcount(cbmoc) LPMIDIOUTCAPSW pmoc, __in UINT cbmoc);
#ifdef UNICODE
#define midiOutGetDevCaps  midiOutGetDevCapsW
#else
#define midiOutGetDevCaps  midiOutGetDevCapsA
#endif // !UNICODE

#else
MMRESULT WINAPI midiOutGetDevCaps(UINT uDeviceID, LPMIDIOUTCAPS pmoc, UINT cbmoc);
#endif

#if (WINVER >= 0x0400)
MMRESULT WINAPI midiOutGetVolume( __in_opt HMIDIOUT hmo, __out LPDWORD pdwVolume);
MMRESULT WINAPI midiOutSetVolume( __in_opt HMIDIOUT hmo, __in DWORD dwVolume);
#else
MMRESULT WINAPI midiOutGetVolume(UINT uId, LPDWORD pdwVolume);
MMRESULT WINAPI midiOutSetVolume(UINT uId, DWORD dwVolume);
#endif

#ifdef _WIN32

MMRESULT WINAPI midiOutGetErrorTextA( __in MMRESULT mmrError, __out_ecount(cchText) LPSTR pszText, __in UINT cchText);
MMRESULT WINAPI midiOutGetErrorTextW( __in MMRESULT mmrError, __out_ecount(cchText) LPWSTR pszText, __in UINT cchText);
#ifdef UNICODE
#define midiOutGetErrorText  midiOutGetErrorTextW
#else
#define midiOutGetErrorText  midiOutGetErrorTextA
#endif // !UNICODE

#else
MMRESULT WINAPI midiOutGetErrorText(MMRESULT mmrError, LPSTR pszText, UINT cchText);
#endif

MMRESULT WINAPI midiOutOpen( __out LPHMIDIOUT phmo, __in UINT uDeviceID,
    __in_opt DWORD_PTR dwCallback, __in_opt DWORD_PTR dwInstance, __in DWORD fdwOpen);
MMRESULT WINAPI midiOutClose( __in HMIDIOUT hmo);
MMRESULT WINAPI midiOutPrepareHeader( __in HMIDIOUT hmo, __inout_bcount(cbmh) LPMIDIHDR pmh, __in UINT cbmh);
MMRESULT WINAPI midiOutUnprepareHeader(__in HMIDIOUT hmo, __inout_bcount(cbmh) LPMIDIHDR pmh, __in UINT cbmh);
MMRESULT WINAPI midiOutShortMsg( __in HMIDIOUT hmo, __in DWORD dwMsg);
MMRESULT WINAPI midiOutLongMsg(__in HMIDIOUT hmo, __in_bcount(cbmh) LPMIDIHDR pmh, __in UINT cbmh);
MMRESULT WINAPI midiOutReset( __in HMIDIOUT hmo);
MMRESULT WINAPI midiOutCachePatches( __in HMIDIOUT hmo, __in UINT uBank, __in_ecount(MIDIPATCHSIZE) LPWORD pwpa, __in UINT fuCache);
MMRESULT WINAPI midiOutCacheDrumPatches( __in HMIDIOUT hmo, __in UINT uPatch, __in_ecount(MIDIPATCHSIZE) LPWORD pwkya, __in UINT fuCache);
MMRESULT WINAPI midiOutGetID( __in HMIDIOUT hmo, __out LPUINT puDeviceID);

#if (WINVER >= 0x030a)
#ifdef _WIN32
MMRESULT WINAPI midiOutMessage( __in_opt HMIDIOUT hmo, __in UINT uMsg, __in_opt DWORD_PTR dw1, __in_opt DWORD_PTR dw2);
#else
DWORD WINAPI midiOutMessage(HMIDIOUT hmo, UINT uMsg, DWORD dw1, DWORD dw2);
#endif
#endif /* ifdef WINVER >= 0x030a */

UINT WINAPI midiInGetNumDevs(void);

#ifdef _WIN32

MMRESULT WINAPI midiInGetDevCapsA( __in UINT_PTR uDeviceID, __out_bcount(cbmic) LPMIDIINCAPSA pmic, __in UINT cbmic);
MMRESULT WINAPI midiInGetDevCapsW( __in UINT_PTR uDeviceID, __out_bcount(cbmic) LPMIDIINCAPSW pmic, __in UINT cbmic);
#ifdef UNICODE
#define midiInGetDevCaps  midiInGetDevCapsW
#else
#define midiInGetDevCaps  midiInGetDevCapsA
#endif // !UNICODE
MMRESULT WINAPI midiInGetErrorTextA( __in MMRESULT mmrError, __out_ecount(cchText) LPSTR pszText, __in UINT cchText);
MMRESULT WINAPI midiInGetErrorTextW( __in MMRESULT mmrError, __out_ecount(cchText) LPWSTR pszText, __in UINT cchText);
#ifdef UNICODE
#define midiInGetErrorText  midiInGetErrorTextW
#else
#define midiInGetErrorText  midiInGetErrorTextA
#endif // !UNICODE

#else
MMRESULT WINAPI midiInGetDevCaps(UINT uDeviceID, LPMIDIINCAPS pmic, UINT cbmic);
MMRESULT WINAPI midiInGetErrorText(MMRESULT mmrError, __out_ecount(cchText) LPSTR pszText, UINT cchText);
#endif

MMRESULT WINAPI midiInOpen( __out LPHMIDIIN phmi, __in UINT uDeviceID,
        __in_opt DWORD_PTR dwCallback, __in_opt DWORD_PTR dwInstance, __in DWORD fdwOpen);
MMRESULT WINAPI midiInClose( __in HMIDIIN hmi);
MMRESULT WINAPI midiInPrepareHeader( __in HMIDIIN hmi, __inout_bcount(cbmh) LPMIDIHDR pmh, __in UINT cbmh);
MMRESULT WINAPI midiInUnprepareHeader( __in HMIDIIN hmi, __inout_bcount(cbmh) LPMIDIHDR pmh, __in UINT cbmh);
MMRESULT WINAPI midiInAddBuffer( __in HMIDIIN hmi, __out_bcount(cbmh) LPMIDIHDR pmh, __in UINT cbmh);
MMRESULT WINAPI midiInStart( __in HMIDIIN hmi);
MMRESULT WINAPI midiInStop( __in HMIDIIN hmi);
MMRESULT WINAPI midiInReset( __in HMIDIIN hmi);
MMRESULT WINAPI midiInGetID( __in HMIDIIN hmi, __out LPUINT puDeviceID);

#if (WINVER >= 0x030a)
#ifdef _WIN32
MMRESULT WINAPI midiInMessage( __in_opt HMIDIIN hmi, __in UINT uMsg, __in_opt DWORD_PTR dw1, __in_opt DWORD_PTR dw2);
#else
DWORD WINAPI midiInMessage(HMIDIIN hmi, UINT uMsg, DWORD dw1, DWORD dw2);
#endif
#endif /* ifdef WINVER >= 0x030a */


#endif  /* ifndef MMNOMIDI */

#ifndef MMNOMIXER
/****************************************************************************

                            Mixer Support

****************************************************************************/

DECLARE_HANDLE(HMIXEROBJ);
typedef HMIXEROBJ FAR *LPHMIXEROBJ;

DECLARE_HANDLE(HMIXER);
typedef HMIXER     FAR *LPHMIXER;

#define MIXER_SHORT_NAME_CHARS   16
#define MIXER_LONG_NAME_CHARS    64

/* */
/*  MMRESULT error return values specific to the mixer API */
/* */
/* */
#define MIXERR_INVALLINE            (MIXERR_BASE + 0)
#define MIXERR_INVALCONTROL         (MIXERR_BASE + 1)
#define MIXERR_INVALVALUE           (MIXERR_BASE + 2)
#define MIXERR_LASTERROR            (MIXERR_BASE + 2)


#define MIXER_OBJECTF_HANDLE    0x80000000L
#define MIXER_OBJECTF_MIXER     0x00000000L
#define MIXER_OBJECTF_HMIXER    (MIXER_OBJECTF_HANDLE|MIXER_OBJECTF_MIXER)
#define MIXER_OBJECTF_WAVEOUT   0x10000000L
#define MIXER_OBJECTF_HWAVEOUT  (MIXER_OBJECTF_HANDLE|MIXER_OBJECTF_WAVEOUT)
#define MIXER_OBJECTF_WAVEIN    0x20000000L
#define MIXER_OBJECTF_HWAVEIN   (MIXER_OBJECTF_HANDLE|MIXER_OBJECTF_WAVEIN)
#define MIXER_OBJECTF_MIDIOUT   0x30000000L
#define MIXER_OBJECTF_HMIDIOUT  (MIXER_OBJECTF_HANDLE|MIXER_OBJECTF_MIDIOUT)
#define MIXER_OBJECTF_MIDIIN    0x40000000L
#define MIXER_OBJECTF_HMIDIIN   (MIXER_OBJECTF_HANDLE|MIXER_OBJECTF_MIDIIN)
#define MIXER_OBJECTF_AUX       0x50000000L


UINT WINAPI mixerGetNumDevs(void);

#ifdef _WIN32

typedef struct tagMIXERCAPSA {
    WORD            wMid;                   /* manufacturer id */
    WORD            wPid;                   /* product id */
    MMVERSION       vDriverVersion;         /* version of the driver */
    CHAR            szPname[MAXPNAMELEN];   /* product name */
    DWORD           fdwSupport;             /* misc. support bits */
    DWORD           cDestinations;          /* count of destinations */
} MIXERCAPSA, *PMIXERCAPSA, *LPMIXERCAPSA;
typedef struct tagMIXERCAPSW {
    WORD            wMid;                   /* manufacturer id */
    WORD            wPid;                   /* product id */
    MMVERSION       vDriverVersion;         /* version of the driver */
    WCHAR           szPname[MAXPNAMELEN];   /* product name */
    DWORD           fdwSupport;             /* misc. support bits */
    DWORD           cDestinations;          /* count of destinations */
} MIXERCAPSW, *PMIXERCAPSW, *LPMIXERCAPSW;
#ifdef UNICODE
typedef MIXERCAPSW MIXERCAPS;
typedef PMIXERCAPSW PMIXERCAPS;
typedef LPMIXERCAPSW LPMIXERCAPS;
#else
typedef MIXERCAPSA MIXERCAPS;
typedef PMIXERCAPSA PMIXERCAPS;
typedef LPMIXERCAPSA LPMIXERCAPS;
#endif // UNICODE
typedef struct tagMIXERCAPS2A {
    WORD            wMid;                   /* manufacturer id */
    WORD            wPid;                   /* product id */
    MMVERSION       vDriverVersion;         /* version of the driver */
    CHAR            szPname[MAXPNAMELEN];   /* product name */
    DWORD           fdwSupport;             /* misc. support bits */
    DWORD           cDestinations;          /* count of destinations */
    GUID            ManufacturerGuid;       /* for extensible MID mapping */
    GUID            ProductGuid;            /* for extensible PID mapping */
    GUID            NameGuid;               /* for name lookup in registry */
} MIXERCAPS2A, *PMIXERCAPS2A, *LPMIXERCAPS2A;
typedef struct tagMIXERCAPS2W {
    WORD            wMid;                   /* manufacturer id */
    WORD            wPid;                   /* product id */
    MMVERSION       vDriverVersion;         /* version of the driver */
    WCHAR           szPname[MAXPNAMELEN];   /* product name */
    DWORD           fdwSupport;             /* misc. support bits */
    DWORD           cDestinations;          /* count of destinations */
    GUID            ManufacturerGuid;       /* for extensible MID mapping */
    GUID            ProductGuid;            /* for extensible PID mapping */
    GUID            NameGuid;               /* for name lookup in registry */
} MIXERCAPS2W, *PMIXERCAPS2W, *LPMIXERCAPS2W;
#ifdef UNICODE
typedef MIXERCAPS2W MIXERCAPS2;
typedef PMIXERCAPS2W PMIXERCAPS2;
typedef LPMIXERCAPS2W LPMIXERCAPS2;
#else
typedef MIXERCAPS2A MIXERCAPS2;
typedef PMIXERCAPS2A PMIXERCAPS2;
typedef LPMIXERCAPS2A LPMIXERCAPS2;
#endif // UNICODE

#else
typedef struct tMIXERCAPS {
    WORD            wMid;                   /* manufacturer id */
    WORD            wPid;                   /* product id */
    VERSION         vDriverVersion;         /* version of the driver */
    char            szPname[MAXPNAMELEN];   /* product name */
    DWORD           fdwSupport;             /* misc. support bits */
    DWORD           cDestinations;          /* count of destinations */
} MIXERCAPS, *PMIXERCAPS, FAR *LPMIXERCAPS;
#endif


#ifdef _WIN32

MMRESULT WINAPI mixerGetDevCapsA( __in UINT_PTR uMxId, __out_bcount(cbmxcaps) LPMIXERCAPSA pmxcaps, __in UINT cbmxcaps);
MMRESULT WINAPI mixerGetDevCapsW( __in UINT_PTR uMxId, __out_bcount(cbmxcaps) LPMIXERCAPSW pmxcaps, __in UINT cbmxcaps);
#ifdef UNICODE
#define mixerGetDevCaps  mixerGetDevCapsW
#else
#define mixerGetDevCaps  mixerGetDevCapsA
#endif // !UNICODE

#else
MMRESULT WINAPI mixerGetDevCaps(UINT uMxId, LPMIXERCAPS pmxcaps, UINT cbmxcaps);
#endif

MMRESULT WINAPI mixerOpen( __out_opt LPHMIXER phmx, __in UINT uMxId, __in_opt DWORD_PTR dwCallback, __in_opt DWORD_PTR dwInstance, __in DWORD fdwOpen);


MMRESULT WINAPI mixerClose( __in HMIXER hmx);

DWORD WINAPI mixerMessage( __in_opt HMIXER hmx, __in UINT uMsg, __in_opt DWORD_PTR dwParam1, __in_opt DWORD_PTR dwParam2);

#ifdef _WIN32

typedef struct tagMIXERLINEA {
    DWORD       cbStruct;               /* size of MIXERLINE structure */
    DWORD       dwDestination;          /* zero based destination index */
    DWORD       dwSource;               /* zero based source index (if source) */
    DWORD       dwLineID;               /* unique line id for mixer device */
    DWORD       fdwLine;                /* state/information about line */
    DWORD_PTR   dwUser;                 /* driver specific information */
    DWORD       dwComponentType;        /* component type line connects to */
    DWORD       cChannels;              /* number of channels line supports */
    DWORD       cConnections;           /* number of connections [possible] */
    DWORD       cControls;              /* number of controls at this line */
    CHAR        szShortName[MIXER_SHORT_NAME_CHARS];
    CHAR        szName[MIXER_LONG_NAME_CHARS];
    struct {
        DWORD       dwType;                 /* MIXERLINE_TARGETTYPE_xxxx */
        DWORD       dwDeviceID;             /* target device ID of device type */
        WORD        wMid;                   /* of target device */
        WORD        wPid;                   /*      " */
        MMVERSION   vDriverVersion;         /*      " */
        CHAR        szPname[MAXPNAMELEN];   /*      " */
    } Target;
} MIXERLINEA, *PMIXERLINEA, *LPMIXERLINEA;
typedef struct tagMIXERLINEW {
    DWORD       cbStruct;               /* size of MIXERLINE structure */
    DWORD       dwDestination;          /* zero based destination index */
    DWORD       dwSource;               /* zero based source index (if source) */
    DWORD       dwLineID;               /* unique line id for mixer device */
    DWORD       fdwLine;                /* state/information about line */
    DWORD_PTR   dwUser;                 /* driver specific information */
    DWORD       dwComponentType;        /* component type line connects to */
    DWORD       cChannels;              /* number of channels line supports */
    DWORD       cConnections;           /* number of connections [possible] */
    DWORD       cControls;              /* number of controls at this line */
    WCHAR       szShortName[MIXER_SHORT_NAME_CHARS];
    WCHAR       szName[MIXER_LONG_NAME_CHARS];
    struct {
        DWORD       dwType;                 /* MIXERLINE_TARGETTYPE_xxxx */
        DWORD       dwDeviceID;             /* target device ID of device type */
        WORD        wMid;                   /* of target device */
        WORD        wPid;                   /*      " */
        MMVERSION   vDriverVersion;         /*      " */
        WCHAR       szPname[MAXPNAMELEN];   /*      " */
    } Target;
} MIXERLINEW, *PMIXERLINEW, *LPMIXERLINEW;
#ifdef UNICODE
typedef MIXERLINEW MIXERLINE;
typedef PMIXERLINEW PMIXERLINE;
typedef LPMIXERLINEW LPMIXERLINE;
#else
typedef MIXERLINEA MIXERLINE;
typedef PMIXERLINEA PMIXERLINE;
typedef LPMIXERLINEA LPMIXERLINE;
#endif // UNICODE

#else
typedef struct tMIXERLINE {
    DWORD       cbStruct;               /* size of MIXERLINE structure */
    DWORD       dwDestination;          /* zero based destination index */
    DWORD       dwSource;               /* zero based source index (if source) */
    DWORD       dwLineID;               /* unique line id for mixer device */
    DWORD       fdwLine;                /* state/information about line */
    DWORD       dwUser;                 /* driver specific information */
    DWORD       dwComponentType;        /* component type line connects to */
    DWORD       cChannels;              /* number of channels line supports */
    DWORD       cConnections;           /* number of connections [possible] */
    DWORD       cControls;              /* number of controls at this line */
    char        szShortName[MIXER_SHORT_NAME_CHARS];
    char        szName[MIXER_LONG_NAME_CHARS];
    struct {
        DWORD   dwType;                 /* MIXERLINE_TARGETTYPE_xxxx */
        DWORD   dwDeviceID;             /* target device ID of device type */
        WORD    wMid;                   /* of target device */
        WORD    wPid;                   /*      " */
        VERSION vDriverVersion;         /*      " */
        char    szPname[MAXPNAMELEN];   /*      " */
    } Target;
} MIXERLINE, *PMIXERLINE, FAR *LPMIXERLINE;
#endif

/* */
/*  MIXERLINE.fdwLine */
/* */
/* */
#define MIXERLINE_LINEF_ACTIVE              0x00000001L
#define MIXERLINE_LINEF_DISCONNECTED        0x00008000L
#define MIXERLINE_LINEF_SOURCE              0x80000000L


/* */
/*  MIXERLINE.dwComponentType */
/* */
/*  component types for destinations and sources */
/* */
/* */
#define MIXERLINE_COMPONENTTYPE_DST_FIRST       0x00000000L
#define MIXERLINE_COMPONENTTYPE_DST_UNDEFINED   (MIXERLINE_COMPONENTTYPE_DST_FIRST + 0)
#define MIXERLINE_COMPONENTTYPE_DST_DIGITAL     (MIXERLINE_COMPONENTTYPE_DST_FIRST + 1)
#define MIXERLINE_COMPONENTTYPE_DST_LINE        (MIXERLINE_COMPONENTTYPE_DST_FIRST + 2)
#define MIXERLINE_COMPONENTTYPE_DST_MONITOR     (MIXERLINE_COMPONENTTYPE_DST_FIRST + 3)
#define MIXERLINE_COMPONENTTYPE_DST_SPEAKERS    (MIXERLINE_COMPONENTTYPE_DST_FIRST + 4)
#define MIXERLINE_COMPONENTTYPE_DST_HEADPHONES  (MIXERLINE_COMPONENTTYPE_DST_FIRST + 5)
#define MIXERLINE_COMPONENTTYPE_DST_TELEPHONE   (MIXERLINE_COMPONENTTYPE_DST_FIRST + 6)
#define MIXERLINE_COMPONENTTYPE_DST_WAVEIN      (MIXERLINE_COMPONENTTYPE_DST_FIRST + 7)
#define MIXERLINE_COMPONENTTYPE_DST_VOICEIN     (MIXERLINE_COMPONENTTYPE_DST_FIRST + 8)
#define MIXERLINE_COMPONENTTYPE_DST_LAST        (MIXERLINE_COMPONENTTYPE_DST_FIRST + 8)

#define MIXERLINE_COMPONENTTYPE_SRC_FIRST       0x00001000L
#define MIXERLINE_COMPONENTTYPE_SRC_UNDEFINED   (MIXERLINE_COMPONENTTYPE_SRC_FIRST + 0)
#define MIXERLINE_COMPONENTTYPE_SRC_DIGITAL     (MIXERLINE_COMPONENTTYPE_SRC_FIRST + 1)
#define MIXERLINE_COMPONENTTYPE_SRC_LINE        (MIXERLINE_COMPONENTTYPE_SRC_FIRST + 2)
#define MIXERLINE_COMPONENTTYPE_SRC_MICROPHONE  (MIXERLINE_COMPONENTTYPE_SRC_FIRST + 3)
#define MIXERLINE_COMPONENTTYPE_SRC_SYNTHESIZER (MIXERLINE_COMPONENTTYPE_SRC_FIRST + 4)
#define MIXERLINE_COMPONENTTYPE_SRC_COMPACTDISC (MIXERLINE_COMPONENTTYPE_SRC_FIRST + 5)
#define MIXERLINE_COMPONENTTYPE_SRC_TELEPHONE   (MIXERLINE_COMPONENTTYPE_SRC_FIRST + 6)
#define MIXERLINE_COMPONENTTYPE_SRC_PCSPEAKER   (MIXERLINE_COMPONENTTYPE_SRC_FIRST + 7)
#define MIXERLINE_COMPONENTTYPE_SRC_WAVEOUT     (MIXERLINE_COMPONENTTYPE_SRC_FIRST + 8)
#define MIXERLINE_COMPONENTTYPE_SRC_AUXILIARY   (MIXERLINE_COMPONENTTYPE_SRC_FIRST + 9)
#define MIXERLINE_COMPONENTTYPE_SRC_ANALOG      (MIXERLINE_COMPONENTTYPE_SRC_FIRST + 10)
#define MIXERLINE_COMPONENTTYPE_SRC_LAST        (MIXERLINE_COMPONENTTYPE_SRC_FIRST + 10)


/* */
/*  MIXERLINE.Target.dwType */
/* */
/* */
#define MIXERLINE_TARGETTYPE_UNDEFINED      0
#define MIXERLINE_TARGETTYPE_WAVEOUT        1
#define MIXERLINE_TARGETTYPE_WAVEIN         2
#define MIXERLINE_TARGETTYPE_MIDIOUT        3
#define MIXERLINE_TARGETTYPE_MIDIIN         4
#define MIXERLINE_TARGETTYPE_AUX            5

#ifdef _WIN32

MMRESULT WINAPI mixerGetLineInfoA( __in_opt HMIXEROBJ hmxobj, __out LPMIXERLINEA pmxl, __in DWORD fdwInfo);
MMRESULT WINAPI mixerGetLineInfoW( __in_opt HMIXEROBJ hmxobj, __out LPMIXERLINEW pmxl, __in DWORD fdwInfo);
#ifdef UNICODE
#define mixerGetLineInfo  mixerGetLineInfoW
#else
#define mixerGetLineInfo  mixerGetLineInfoA
#endif // !UNICODE

#else
MMRESULT WINAPI mixerGetLineInfo(HMIXEROBJ hmxobj, LPMIXERLINE pmxl, DWORD fdwInfo);
#endif

#define MIXER_GETLINEINFOF_DESTINATION      0x00000000L
#define MIXER_GETLINEINFOF_SOURCE           0x00000001L
#define MIXER_GETLINEINFOF_LINEID           0x00000002L
#define MIXER_GETLINEINFOF_COMPONENTTYPE    0x00000003L
#define MIXER_GETLINEINFOF_TARGETTYPE       0x00000004L

#define MIXER_GETLINEINFOF_QUERYMASK        0x0000000FL


MMRESULT WINAPI mixerGetID( __in_opt HMIXEROBJ hmxobj, __out UINT FAR *puMxId, __in DWORD fdwId);


/* */
/*  MIXERCONTROL */
/* */
/* */
#ifdef _WIN32

typedef struct tagMIXERCONTROLA {
    DWORD           cbStruct;           /* size in bytes of MIXERCONTROL */
    DWORD           dwControlID;        /* unique control id for mixer device */
    DWORD           dwControlType;      /* MIXERCONTROL_CONTROLTYPE_xxx */
    DWORD           fdwControl;         /* MIXERCONTROL_CONTROLF_xxx */
    DWORD           cMultipleItems;     /* if MIXERCONTROL_CONTROLF_MULTIPLE set */
    CHAR            szShortName[MIXER_SHORT_NAME_CHARS];
    CHAR            szName[MIXER_LONG_NAME_CHARS];
    union {
        struct {
            LONG    lMinimum;           /* signed minimum for this control */
            LONG    lMaximum;           /* signed maximum for this control */
        };
        struct {
            DWORD   dwMinimum;          /* unsigned minimum for this control */
            DWORD   dwMaximum;          /* unsigned maximum for this control */
        };
        DWORD       dwReserved[6];
    } Bounds;
    union {
        DWORD       cSteps;             /* # of steps between min & max */
        DWORD       cbCustomData;       /* size in bytes of custom data */
        DWORD       dwReserved[6];      /* !!! needed? we have cbStruct.... */
    } Metrics;
} MIXERCONTROLA, *PMIXERCONTROLA, *LPMIXERCONTROLA;
typedef struct tagMIXERCONTROLW {
    DWORD           cbStruct;           /* size in bytes of MIXERCONTROL */
    DWORD           dwControlID;        /* unique control id for mixer device */
    DWORD           dwControlType;      /* MIXERCONTROL_CONTROLTYPE_xxx */
    DWORD           fdwControl;         /* MIXERCONTROL_CONTROLF_xxx */
    DWORD           cMultipleItems;     /* if MIXERCONTROL_CONTROLF_MULTIPLE set */
    WCHAR           szShortName[MIXER_SHORT_NAME_CHARS];
    WCHAR           szName[MIXER_LONG_NAME_CHARS];
    union {
        struct {
            LONG    lMinimum;           /* signed minimum for this control */
            LONG    lMaximum;           /* signed maximum for this control */
        };
        struct {
            DWORD   dwMinimum;          /* unsigned minimum for this control */
            DWORD   dwMaximum;          /* unsigned maximum for this control */
        };
        DWORD       dwReserved[6];
    } Bounds;
    union {
        DWORD       cSteps;             /* # of steps between min & max */
        DWORD       cbCustomData;       /* size in bytes of custom data */
        DWORD       dwReserved[6];      /* !!! needed? we have cbStruct.... */
    } Metrics;
} MIXERCONTROLW, *PMIXERCONTROLW, *LPMIXERCONTROLW;
#ifdef UNICODE
typedef MIXERCONTROLW MIXERCONTROL;
typedef PMIXERCONTROLW PMIXERCONTROL;
typedef LPMIXERCONTROLW LPMIXERCONTROL;
#else
typedef MIXERCONTROLA MIXERCONTROL;
typedef PMIXERCONTROLA PMIXERCONTROL;
typedef LPMIXERCONTROLA LPMIXERCONTROL;
#endif // UNICODE

#else
typedef struct tMIXERCONTROL {
    DWORD           cbStruct;           /* size in bytes of MIXERCONTROL */
    DWORD           dwControlID;        /* unique control id for mixer device */
    DWORD           dwControlType;      /* MIXERCONTROL_CONTROLTYPE_xxx */
    DWORD           fdwControl;         /* MIXERCONTROL_CONTROLF_xxx */
    DWORD           cMultipleItems;     /* if MIXERCONTROL_CONTROLF_MULTIPLE set */
    char            szShortName[MIXER_SHORT_NAME_CHARS];
    char            szName[MIXER_LONG_NAME_CHARS];
    union {
        struct {
            LONG    lMinimum;           /* signed minimum for this control */
            LONG    lMaximum;           /* signed maximum for this control */
        };
        struct {
            DWORD   dwMinimum;          /* unsigned minimum for this control */
            DWORD   dwMaximum;          /* unsigned maximum for this control */
        };
        DWORD       dwReserved[6];
    } Bounds;
    union {
        DWORD       cSteps;             /* # of steps between min & max */
        DWORD       cbCustomData;       /* size in bytes of custom data */
        DWORD       dwReserved[6];      /* !!! needed? we have cbStruct.... */
    } Metrics;
} MIXERCONTROL, *PMIXERCONTROL, FAR *LPMIXERCONTROL;
#endif

/* */
/*  MIXERCONTROL.fdwControl */
/* */
/* */
#define MIXERCONTROL_CONTROLF_UNIFORM   0x00000001L
#define MIXERCONTROL_CONTROLF_MULTIPLE  0x00000002L
#define MIXERCONTROL_CONTROLF_DISABLED  0x80000000L


/* */
/*  MIXERCONTROL_CONTROLTYPE_xxx building block defines */
/* */
/* */
#define MIXERCONTROL_CT_CLASS_MASK          0xF0000000L
#define MIXERCONTROL_CT_CLASS_CUSTOM        0x00000000L
#define MIXERCONTROL_CT_CLASS_METER         0x10000000L
#define MIXERCONTROL_CT_CLASS_SWITCH        0x20000000L
#define MIXERCONTROL_CT_CLASS_NUMBER        0x30000000L
#define MIXERCONTROL_CT_CLASS_SLIDER        0x40000000L
#define MIXERCONTROL_CT_CLASS_FADER         0x50000000L
#define MIXERCONTROL_CT_CLASS_TIME          0x60000000L
#define MIXERCONTROL_CT_CLASS_LIST          0x70000000L


#define MIXERCONTROL_CT_SUBCLASS_MASK       0x0F000000L

#define MIXERCONTROL_CT_SC_SWITCH_BOOLEAN   0x00000000L
#define MIXERCONTROL_CT_SC_SWITCH_BUTTON    0x01000000L

#define MIXERCONTROL_CT_SC_METER_POLLED     0x00000000L

#define MIXERCONTROL_CT_SC_TIME_MICROSECS   0x00000000L
#define MIXERCONTROL_CT_SC_TIME_MILLISECS   0x01000000L

#define MIXERCONTROL_CT_SC_LIST_SINGLE      0x00000000L
#define MIXERCONTROL_CT_SC_LIST_MULTIPLE    0x01000000L


#define MIXERCONTROL_CT_UNITS_MASK          0x00FF0000L
#define MIXERCONTROL_CT_UNITS_CUSTOM        0x00000000L
#define MIXERCONTROL_CT_UNITS_BOOLEAN       0x00010000L
#define MIXERCONTROL_CT_UNITS_SIGNED        0x00020000L
#define MIXERCONTROL_CT_UNITS_UNSIGNED      0x00030000L
#define MIXERCONTROL_CT_UNITS_DECIBELS      0x00040000L /* in 10ths */
#define MIXERCONTROL_CT_UNITS_PERCENT       0x00050000L /* in 10ths */


/* */
/*  Commonly used control types for specifying MIXERCONTROL.dwControlType */
/* */

#define MIXERCONTROL_CONTROLTYPE_CUSTOM         (MIXERCONTROL_CT_CLASS_CUSTOM | MIXERCONTROL_CT_UNITS_CUSTOM)
#define MIXERCONTROL_CONTROLTYPE_BOOLEANMETER   (MIXERCONTROL_CT_CLASS_METER | MIXERCONTROL_CT_SC_METER_POLLED | MIXERCONTROL_CT_UNITS_BOOLEAN)
#define MIXERCONTROL_CONTROLTYPE_SIGNEDMETER    (MIXERCONTROL_CT_CLASS_METER | MIXERCONTROL_CT_SC_METER_POLLED | MIXERCONTROL_CT_UNITS_SIGNED)
#define MIXERCONTROL_CONTROLTYPE_PEAKMETER      (MIXERCONTROL_CONTROLTYPE_SIGNEDMETER + 1)
#define MIXERCONTROL_CONTROLTYPE_UNSIGNEDMETER  (MIXERCONTROL_CT_CLASS_METER | MIXERCONTROL_CT_SC_METER_POLLED | MIXERCONTROL_CT_UNITS_UNSIGNED)
#define MIXERCONTROL_CONTROLTYPE_BOOLEAN        (MIXERCONTROL_CT_CLASS_SWITCH | MIXERCONTROL_CT_SC_SWITCH_BOOLEAN | MIXERCONTROL_CT_UNITS_BOOLEAN)
#define MIXERCONTROL_CONTROLTYPE_ONOFF          (MIXERCONTROL_CONTROLTYPE_BOOLEAN + 1)
#define MIXERCONTROL_CONTROLTYPE_MUTE           (MIXERCONTROL_CONTROLTYPE_BOOLEAN + 2)
#define MIXERCONTROL_CONTROLTYPE_MONO           (MIXERCONTROL_CONTROLTYPE_BOOLEAN + 3)
#define MIXERCONTROL_CONTROLTYPE_LOUDNESS       (MIXERCONTROL_CONTROLTYPE_BOOLEAN + 4)
#define MIXERCONTROL_CONTROLTYPE_STEREOENH      (MIXERCONTROL_CONTROLTYPE_BOOLEAN + 5)
#define MIXERCONTROL_CONTROLTYPE_BASS_BOOST     (MIXERCONTROL_CONTROLTYPE_BOOLEAN + 0x00002277)
#define MIXERCONTROL_CONTROLTYPE_BUTTON         (MIXERCONTROL_CT_CLASS_SWITCH | MIXERCONTROL_CT_SC_SWITCH_BUTTON | MIXERCONTROL_CT_UNITS_BOOLEAN)
#define MIXERCONTROL_CONTROLTYPE_DECIBELS       (MIXERCONTROL_CT_CLASS_NUMBER | MIXERCONTROL_CT_UNITS_DECIBELS)
#define MIXERCONTROL_CONTROLTYPE_SIGNED         (MIXERCONTROL_CT_CLASS_NUMBER | MIXERCONTROL_CT_UNITS_SIGNED)
#define MIXERCONTROL_CONTROLTYPE_UNSIGNED       (MIXERCONTROL_CT_CLASS_NUMBER | MIXERCONTROL_CT_UNITS_UNSIGNED)
#define MIXERCONTROL_CONTROLTYPE_PERCENT        (MIXERCONTROL_CT_CLASS_NUMBER | MIXERCONTROL_CT_UNITS_PERCENT)
#define MIXERCONTROL_CONTROLTYPE_SLIDER         (MIXERCONTROL_CT_CLASS_SLIDER | MIXERCONTROL_CT_UNITS_SIGNED)
#define MIXERCONTROL_CONTROLTYPE_PAN            (MIXERCONTROL_CONTROLTYPE_SLIDER + 1)
#define MIXERCONTROL_CONTROLTYPE_QSOUNDPAN      (MIXERCONTROL_CONTROLTYPE_SLIDER + 2)
#define MIXERCONTROL_CONTROLTYPE_FADER          (MIXERCONTROL_CT_CLASS_FADER | MIXERCONTROL_CT_UNITS_UNSIGNED)
#define MIXERCONTROL_CONTROLTYPE_VOLUME         (MIXERCONTROL_CONTROLTYPE_FADER + 1)
#define MIXERCONTROL_CONTROLTYPE_BASS           (MIXERCONTROL_CONTROLTYPE_FADER + 2)
#define MIXERCONTROL_CONTROLTYPE_TREBLE         (MIXERCONTROL_CONTROLTYPE_FADER + 3)
#define MIXERCONTROL_CONTROLTYPE_EQUALIZER      (MIXERCONTROL_CONTROLTYPE_FADER + 4)
#define MIXERCONTROL_CONTROLTYPE_SINGLESELECT   (MIXERCONTROL_CT_CLASS_LIST | MIXERCONTROL_CT_SC_LIST_SINGLE | MIXERCONTROL_CT_UNITS_BOOLEAN)
#define MIXERCONTROL_CONTROLTYPE_MUX            (MIXERCONTROL_CONTROLTYPE_SINGLESELECT + 1)
#define MIXERCONTROL_CONTROLTYPE_MULTIPLESELECT (MIXERCONTROL_CT_CLASS_LIST | MIXERCONTROL_CT_SC_LIST_MULTIPLE | MIXERCONTROL_CT_UNITS_BOOLEAN)
#define MIXERCONTROL_CONTROLTYPE_MIXER          (MIXERCONTROL_CONTROLTYPE_MULTIPLESELECT + 1)
#define MIXERCONTROL_CONTROLTYPE_MICROTIME      (MIXERCONTROL_CT_CLASS_TIME | MIXERCONTROL_CT_SC_TIME_MICROSECS | MIXERCONTROL_CT_UNITS_UNSIGNED)
#define MIXERCONTROL_CONTROLTYPE_MILLITIME      (MIXERCONTROL_CT_CLASS_TIME | MIXERCONTROL_CT_SC_TIME_MILLISECS | MIXERCONTROL_CT_UNITS_UNSIGNED)

/* */
/*  MIXERLINECONTROLS */
/* */
#ifdef _WIN32

typedef struct tagMIXERLINECONTROLSA {
    DWORD           cbStruct;       /* size in bytes of MIXERLINECONTROLS */
    DWORD           dwLineID;       /* line id (from MIXERLINE.dwLineID) */
    union {
        DWORD       dwControlID;    /* MIXER_GETLINECONTROLSF_ONEBYID */
        DWORD       dwControlType;  /* MIXER_GETLINECONTROLSF_ONEBYTYPE */
    };
    DWORD           cControls;      /* count of controls pmxctrl points to */
    DWORD           cbmxctrl;       /* size in bytes of _one_ MIXERCONTROL */
    LPMIXERCONTROLA pamxctrl;       /* pointer to first MIXERCONTROL array */
} MIXERLINECONTROLSA, *PMIXERLINECONTROLSA, *LPMIXERLINECONTROLSA;
typedef struct tagMIXERLINECONTROLSW {
    DWORD           cbStruct;       /* size in bytes of MIXERLINECONTROLS */
    DWORD           dwLineID;       /* line id (from MIXERLINE.dwLineID) */
    union {
        DWORD       dwControlID;    /* MIXER_GETLINECONTROLSF_ONEBYID */
        DWORD       dwControlType;  /* MIXER_GETLINECONTROLSF_ONEBYTYPE */
    };
    DWORD           cControls;      /* count of controls pmxctrl points to */
    DWORD           cbmxctrl;       /* size in bytes of _one_ MIXERCONTROL */
    LPMIXERCONTROLW pamxctrl;       /* pointer to first MIXERCONTROL array */
} MIXERLINECONTROLSW, *PMIXERLINECONTROLSW, *LPMIXERLINECONTROLSW;
#ifdef UNICODE
typedef MIXERLINECONTROLSW MIXERLINECONTROLS;
typedef PMIXERLINECONTROLSW PMIXERLINECONTROLS;
typedef LPMIXERLINECONTROLSW LPMIXERLINECONTROLS;
#else
typedef MIXERLINECONTROLSA MIXERLINECONTROLS;
typedef PMIXERLINECONTROLSA PMIXERLINECONTROLS;
typedef LPMIXERLINECONTROLSA LPMIXERLINECONTROLS;
#endif // UNICODE

#else
typedef struct tMIXERLINECONTROLS {
    DWORD           cbStruct;       /* size in bytes of MIXERLINECONTROLS */
    DWORD           dwLineID;       /* line id (from MIXERLINE.dwLineID) */
    union {
        DWORD       dwControlID;    /* MIXER_GETLINECONTROLSF_ONEBYID */
        DWORD       dwControlType;  /* MIXER_GETLINECONTROLSF_ONEBYTYPE */
    };
    DWORD           cControls;      /* count of controls pmxctrl points to */
    DWORD           cbmxctrl;       /* size in bytes of _one_ MIXERCONTROL */
    LPMIXERCONTROL  pamxctrl;       /* pointer to first MIXERCONTROL array */
} MIXERLINECONTROLS, *PMIXERLINECONTROLS, FAR *LPMIXERLINECONTROLS;
#endif


/* */
/* */
/* */
#ifdef _WIN32

MMRESULT WINAPI mixerGetLineControlsA( __in_opt HMIXEROBJ hmxobj, __inout LPMIXERLINECONTROLSA pmxlc, __in DWORD fdwControls);
MMRESULT WINAPI mixerGetLineControlsW( __in_opt HMIXEROBJ hmxobj, __inout LPMIXERLINECONTROLSW pmxlc, __in DWORD fdwControls);
#ifdef UNICODE
#define mixerGetLineControls  mixerGetLineControlsW
#else
#define mixerGetLineControls  mixerGetLineControlsA
#endif // !UNICODE

#else
MMRESULT WINAPI mixerGetLineControls(HMIXEROBJ hmxobj, LPMIXERLINECONTROLS pmxlc, DWORD fdwControls);
#endif

#define MIXER_GETLINECONTROLSF_ALL          0x00000000L
#define MIXER_GETLINECONTROLSF_ONEBYID      0x00000001L
#define MIXER_GETLINECONTROLSF_ONEBYTYPE    0x00000002L

#define MIXER_GETLINECONTROLSF_QUERYMASK    0x0000000FL


typedef struct tMIXERCONTROLDETAILS {
    DWORD           cbStruct;       /* size in bytes of MIXERCONTROLDETAILS */
    DWORD           dwControlID;    /* control id to get/set details on */
    DWORD           cChannels;      /* number of channels in paDetails array */
    union {
        HWND        hwndOwner;      /* for MIXER_SETCONTROLDETAILSF_CUSTOM */
        DWORD       cMultipleItems; /* if _MULTIPLE, the number of items per channel */
    };
    DWORD           cbDetails;      /* size of _one_ details_XX struct */
    LPVOID          paDetails;      /* pointer to array of details_XX structs */
} MIXERCONTROLDETAILS, *PMIXERCONTROLDETAILS, FAR *LPMIXERCONTROLDETAILS;


/* */
/*  MIXER_GETCONTROLDETAILSF_LISTTEXT */
/* */
/* */
#ifdef _WIN32

typedef struct tagMIXERCONTROLDETAILS_LISTTEXTA {
    DWORD           dwParam1;
    DWORD           dwParam2;
    CHAR            szName[MIXER_LONG_NAME_CHARS];
} MIXERCONTROLDETAILS_LISTTEXTA, *PMIXERCONTROLDETAILS_LISTTEXTA, *LPMIXERCONTROLDETAILS_LISTTEXTA;
typedef struct tagMIXERCONTROLDETAILS_LISTTEXTW {
    DWORD           dwParam1;
    DWORD           dwParam2;
    WCHAR           szName[MIXER_LONG_NAME_CHARS];
} MIXERCONTROLDETAILS_LISTTEXTW, *PMIXERCONTROLDETAILS_LISTTEXTW, *LPMIXERCONTROLDETAILS_LISTTEXTW;
#ifdef UNICODE
typedef MIXERCONTROLDETAILS_LISTTEXTW MIXERCONTROLDETAILS_LISTTEXT;
typedef PMIXERCONTROLDETAILS_LISTTEXTW PMIXERCONTROLDETAILS_LISTTEXT;
typedef LPMIXERCONTROLDETAILS_LISTTEXTW LPMIXERCONTROLDETAILS_LISTTEXT;
#else
typedef MIXERCONTROLDETAILS_LISTTEXTA MIXERCONTROLDETAILS_LISTTEXT;
typedef PMIXERCONTROLDETAILS_LISTTEXTA PMIXERCONTROLDETAILS_LISTTEXT;
typedef LPMIXERCONTROLDETAILS_LISTTEXTA LPMIXERCONTROLDETAILS_LISTTEXT;
#endif // UNICODE

#else
typedef struct tMIXERCONTROLDETAILS_LISTTEXT {
    DWORD           dwParam1;
    DWORD           dwParam2;
    char            szName[MIXER_LONG_NAME_CHARS];
} MIXERCONTROLDETAILS_LISTTEXT, *PMIXERCONTROLDETAILS_LISTTEXT, FAR *LPMIXERCONTROLDETAILS_LISTTEXT;
#endif

/* */
/*  MIXER_GETCONTROLDETAILSF_VALUE */
/* */
/* */
typedef struct tMIXERCONTROLDETAILS_BOOLEAN {
    LONG            fValue;
}       MIXERCONTROLDETAILS_BOOLEAN,
      *PMIXERCONTROLDETAILS_BOOLEAN,
 FAR *LPMIXERCONTROLDETAILS_BOOLEAN;

typedef struct tMIXERCONTROLDETAILS_SIGNED {
    LONG            lValue;
}       MIXERCONTROLDETAILS_SIGNED,
      *PMIXERCONTROLDETAILS_SIGNED,
 FAR *LPMIXERCONTROLDETAILS_SIGNED;


typedef struct tMIXERCONTROLDETAILS_UNSIGNED {
    DWORD           dwValue;
}       MIXERCONTROLDETAILS_UNSIGNED,
      *PMIXERCONTROLDETAILS_UNSIGNED,
 FAR *LPMIXERCONTROLDETAILS_UNSIGNED;


#ifdef _WIN32

MMRESULT WINAPI mixerGetControlDetailsA( __in_opt HMIXEROBJ hmxobj, __inout LPMIXERCONTROLDETAILS pmxcd, __in DWORD fdwDetails);
MMRESULT WINAPI mixerGetControlDetailsW( __in_opt HMIXEROBJ hmxobj, __inout LPMIXERCONTROLDETAILS pmxcd, __in DWORD fdwDetails);
#ifdef UNICODE
#define mixerGetControlDetails  mixerGetControlDetailsW
#else
#define mixerGetControlDetails  mixerGetControlDetailsA
#endif // !UNICODE

#else
MMRESULT WINAPI mixerGetControlDetails(HMIXEROBJ hmxobj, LPMIXERCONTROLDETAILS pmxcd, DWORD fdwDetails);
#endif

#define MIXER_GETCONTROLDETAILSF_VALUE      0x00000000L
#define MIXER_GETCONTROLDETAILSF_LISTTEXT   0x00000001L

#define MIXER_GETCONTROLDETAILSF_QUERYMASK  0x0000000FL


MMRESULT WINAPI mixerSetControlDetails( __in_opt HMIXEROBJ hmxobj, __in LPMIXERCONTROLDETAILS pmxcd, __in DWORD fdwDetails);

#define MIXER_SETCONTROLDETAILSF_VALUE      0x00000000L
#define MIXER_SETCONTROLDETAILSF_CUSTOM     0x00000001L

#define MIXER_SETCONTROLDETAILSF_QUERYMASK  0x0000000FL


#endif /* ifndef MMNOMIXER */

#ifndef MMNOMMIO
/****************************************************************************

                        Multimedia File I/O support

****************************************************************************/

/* MMIO error return values */
#define MMIOERR_BASE                256
#define MMIOERR_FILENOTFOUND        (MMIOERR_BASE + 1)  /* file not found */
#define MMIOERR_OUTOFMEMORY         (MMIOERR_BASE + 2)  /* out of memory */
#define MMIOERR_CANNOTOPEN          (MMIOERR_BASE + 3)  /* cannot open */
#define MMIOERR_CANNOTCLOSE         (MMIOERR_BASE + 4)  /* cannot close */
#define MMIOERR_CANNOTREAD          (MMIOERR_BASE + 5)  /* cannot read */
#define MMIOERR_CANNOTWRITE         (MMIOERR_BASE + 6)  /* cannot write */
#define MMIOERR_CANNOTSEEK          (MMIOERR_BASE + 7)  /* cannot seek */
#define MMIOERR_CANNOTEXPAND        (MMIOERR_BASE + 8)  /* cannot expand file */
#define MMIOERR_CHUNKNOTFOUND       (MMIOERR_BASE + 9)  /* chunk not found */
#define MMIOERR_UNBUFFERED          (MMIOERR_BASE + 10) /*  */
#define MMIOERR_PATHNOTFOUND        (MMIOERR_BASE + 11) /* path incorrect */
#define MMIOERR_ACCESSDENIED        (MMIOERR_BASE + 12) /* file was protected */
#define MMIOERR_SHARINGVIOLATION    (MMIOERR_BASE + 13) /* file in use */
#define MMIOERR_NETWORKERROR        (MMIOERR_BASE + 14) /* network not responding */
#define MMIOERR_TOOMANYOPENFILES    (MMIOERR_BASE + 15) /* no more file handles  */
#define MMIOERR_INVALIDFILE         (MMIOERR_BASE + 16) /* default error file error */

/* MMIO constants */
#define CFSEPCHAR       '+'             /* compound file name separator char. */

/* MMIO data types */
typedef DWORD           FOURCC;         /* a four character code */
typedef char _huge *    HPSTR;          /* a huge version of LPSTR */
DECLARE_HANDLE(HMMIO);                  /* a handle to an open file */
typedef LRESULT (CALLBACK MMIOPROC)(LPSTR lpmmioinfo, UINT uMsg,
            LPARAM lParam1, LPARAM lParam2);
typedef MMIOPROC FAR *LPMMIOPROC;


/* general MMIO information data structure */
typedef struct _MMIOINFO
{
        /* general fields */
        DWORD           dwFlags;        /* general status flags */
        FOURCC          fccIOProc;      /* pointer to I/O procedure */
        LPMMIOPROC      pIOProc;        /* pointer to I/O procedure */
        UINT            wErrorRet;      /* place for error to be returned */
        HTASK           htask;          /* alternate local task */

        /* fields maintained by MMIO functions during buffered I/O */
        LONG            cchBuffer;      /* size of I/O buffer (or 0L) */
        HPSTR           pchBuffer;      /* start of I/O buffer (or NULL) */
        HPSTR           pchNext;        /* pointer to next byte to read/write */
        HPSTR           pchEndRead;     /* pointer to last valid byte to read */
        HPSTR           pchEndWrite;    /* pointer to last byte to write */
        LONG            lBufOffset;     /* disk offset of start of buffer */

        /* fields maintained by I/O procedure */
        LONG            lDiskOffset;    /* disk offset of next read or write */
        DWORD           adwInfo[3];     /* data specific to type of MMIOPROC */

        /* other fields maintained by MMIO */
        DWORD           dwReserved1;    /* reserved for MMIO use */
        DWORD           dwReserved2;    /* reserved for MMIO use */
        HMMIO           hmmio;          /* handle to open file */
} MMIOINFO, *PMMIOINFO, NEAR *NPMMIOINFO, FAR *LPMMIOINFO;
typedef const MMIOINFO FAR *LPCMMIOINFO;

/* RIFF chunk information data structure */
typedef struct _MMCKINFO
{
        FOURCC          ckid;           /* chunk ID */
        DWORD           cksize;         /* chunk size */
        FOURCC          fccType;        /* form type or list type */
        DWORD           dwDataOffset;   /* offset of data portion of chunk */
        DWORD           dwFlags;        /* flags used by MMIO functions */
} MMCKINFO, *PMMCKINFO, NEAR *NPMMCKINFO, FAR *LPMMCKINFO;
typedef const MMCKINFO *LPCMMCKINFO;

/* bit field masks */
#define MMIO_RWMODE     0x00000003      /* open file for reading/writing/both */
#define MMIO_SHAREMODE  0x00000070      /* file sharing mode number */

/* constants for dwFlags field of MMIOINFO */
#define MMIO_CREATE     0x00001000      /* create new file (or truncate file) */
#define MMIO_PARSE      0x00000100      /* parse new file returning path */
#define MMIO_DELETE     0x00000200      /* create new file (or truncate file) */
#define MMIO_EXIST      0x00004000      /* checks for existence of file */
#define MMIO_ALLOCBUF   0x00010000      /* mmioOpen() should allocate a buffer */
#define MMIO_GETTEMP    0x00020000      /* mmioOpen() should retrieve temp name */

#define MMIO_DIRTY      0x10000000      /* I/O buffer is dirty */


/* read/write mode numbers (bit field MMIO_RWMODE) */
#define MMIO_READ       0x00000000      /* open file for reading only */
#define MMIO_WRITE      0x00000001      /* open file for writing only */
#define MMIO_READWRITE  0x00000002      /* open file for reading and writing */

/* share mode numbers (bit field MMIO_SHAREMODE) */
#define MMIO_COMPAT     0x00000000      /* compatibility mode */
#define MMIO_EXCLUSIVE  0x00000010      /* exclusive-access mode */
#define MMIO_DENYWRITE  0x00000020      /* deny writing to other processes */
#define MMIO_DENYREAD   0x00000030      /* deny reading to other processes */
#define MMIO_DENYNONE   0x00000040      /* deny nothing to other processes */

/* various MMIO flags */
#define MMIO_FHOPEN             0x0010  /* mmioClose: keep file handle open */
#define MMIO_EMPTYBUF           0x0010  /* mmioFlush: empty the I/O buffer */
#define MMIO_TOUPPER            0x0010  /* mmioStringToFOURCC: to u-case */
#define MMIO_INSTALLPROC    0x00010000  /* mmioInstallIOProc: install MMIOProc */
#define MMIO_GLOBALPROC     0x10000000  /* mmioInstallIOProc: install globally */
#define MMIO_REMOVEPROC     0x00020000  /* mmioInstallIOProc: remove MMIOProc */
#define MMIO_UNICODEPROC    0x01000000  /* mmioInstallIOProc: Unicode MMIOProc */
#define MMIO_FINDPROC       0x00040000  /* mmioInstallIOProc: find an MMIOProc */
#define MMIO_FINDCHUNK          0x0010  /* mmioDescend: find a chunk by ID */
#define MMIO_FINDRIFF           0x0020  /* mmioDescend: find a LIST chunk */
#define MMIO_FINDLIST           0x0040  /* mmioDescend: find a RIFF chunk */
#define MMIO_CREATERIFF         0x0020  /* mmioCreateChunk: make a LIST chunk */
#define MMIO_CREATELIST         0x0040  /* mmioCreateChunk: make a RIFF chunk */


/* message numbers for MMIOPROC I/O procedure functions */
#define MMIOM_READ      MMIO_READ       /* read */
#define MMIOM_WRITE    MMIO_WRITE       /* write */
#define MMIOM_SEEK              2       /* seek to a new position in file */
#define MMIOM_OPEN              3       /* open file */
#define MMIOM_CLOSE             4       /* close file */
#define MMIOM_WRITEFLUSH        5       /* write and flush */

#if (WINVER >= 0x030a)
#define MMIOM_RENAME            6       /* rename specified file */
#endif /* ifdef WINVER >= 0x030a */

#define MMIOM_USER         0x8000       /* beginning of user-defined messages */

/* standard four character codes */
#define FOURCC_RIFF     mmioFOURCC('R', 'I', 'F', 'F')
#define FOURCC_LIST     mmioFOURCC('L', 'I', 'S', 'T')

/* four character codes used to identify standard built-in I/O procedures */
#define FOURCC_DOS      mmioFOURCC('D', 'O', 'S', ' ')
#define FOURCC_MEM      mmioFOURCC('M', 'E', 'M', ' ')

/* flags for mmioSeek() */
#ifndef SEEK_SET
#define SEEK_SET        0               /* seek to an absolute position */
#define SEEK_CUR        1               /* seek relative to current position */
#define SEEK_END        2               /* seek relative to end of file */
#endif  /* ifndef SEEK_SET */

/* other constants */
#define MMIO_DEFAULTBUFFER      8192    /* default buffer size */

/* MMIO macros */
#define mmioFOURCC(ch0, ch1, ch2, ch3)  MAKEFOURCC(ch0, ch1, ch2, ch3)

/* MMIO function prototypes */
#ifdef _WIN32

FOURCC WINAPI mmioStringToFOURCCA( LPCSTR sz, __in UINT uFlags);
FOURCC WINAPI mmioStringToFOURCCW( LPCWSTR sz, __in UINT uFlags);
#ifdef UNICODE
#define mmioStringToFOURCC  mmioStringToFOURCCW
#else
#define mmioStringToFOURCC  mmioStringToFOURCCA
#endif // !UNICODE
LPMMIOPROC WINAPI mmioInstallIOProcA( __in FOURCC fccIOProc, __in_opt LPMMIOPROC pIOProc, __in DWORD dwFlags);
LPMMIOPROC WINAPI mmioInstallIOProcW( __in FOURCC fccIOProc, __in_opt LPMMIOPROC pIOProc, __in DWORD dwFlags);
#ifdef UNICODE
#define mmioInstallIOProc  mmioInstallIOProcW
#else
#define mmioInstallIOProc  mmioInstallIOProcA
#endif // !UNICODE
HMMIO WINAPI mmioOpenA( __inout_bcount_opt(128) LPSTR pszFileName,  __inout_opt LPMMIOINFO pmmioinfo,  __in DWORD fdwOpen);
HMMIO WINAPI mmioOpenW( __inout_bcount_opt(128) LPWSTR pszFileName,  __inout_opt LPMMIOINFO pmmioinfo,  __in DWORD fdwOpen);
#ifdef UNICODE
#define mmioOpen  mmioOpenW
#else
#define mmioOpen  mmioOpenA
#endif // !UNICODE
MMRESULT WINAPI mmioRenameA( __in LPCSTR pszFileName, __in LPCSTR pszNewFileName, __in_opt LPCMMIOINFO pmmioinfo, __in DWORD fdwRename);
MMRESULT WINAPI mmioRenameW( __in LPCWSTR pszFileName, __in LPCWSTR pszNewFileName, __in_opt LPCMMIOINFO pmmioinfo, __in DWORD fdwRename);
#ifdef UNICODE
#define mmioRename  mmioRenameW
#else
#define mmioRename  mmioRenameA
#endif // !UNICODE
#else
FOURCC WINAPI mmioStringToFOURCC( LPCSTR sz, UINT uFlags);
LPMMIOPROC WINAPI mmioInstallIOProc( FOURCC fccIOProc, LPMMIOPROC pIOProc, DWORD dwFlags);
HMMIO WINAPI mmioOpen(__inout_opt LPSTR pszFileName, LPMMIOINFO pmmioinfo, DWORD fdwOpen);
#if (WINVER >= 0x030a)
MMRESULT WINAPI mmioRename( __in LPCSTR pszFileName, __in LPCSTR pszNewFileName, __in_opt const MMIOINFO FAR* pmmioinfo, __in DWORD fdwRename);
#endif /* ifdef WINVER >= 0x030a */
#endif

MMRESULT WINAPI mmioClose( __in HMMIO hmmio, __in UINT fuClose);
LONG WINAPI mmioRead( __in HMMIO hmmio, __out_bcount(cch) HPSTR pch, __in LONG cch);
LONG WINAPI mmioWrite( __in HMMIO hmmio, __out_bcount(cch) const char _huge* pch, __in LONG cch);
LONG WINAPI mmioSeek( __in HMMIO hmmio, __in LONG lOffset, __in int iOrigin);
MMRESULT WINAPI mmioGetInfo( __in HMMIO hmmio, __out LPMMIOINFO pmmioinfo, __in UINT fuInfo);
MMRESULT WINAPI mmioSetInfo( __in HMMIO hmmio, __in LPCMMIOINFO pmmioinfo, __in UINT fuInfo);
MMRESULT WINAPI mmioSetBuffer( __in HMMIO hmmio, __out_ecount_opt(cchBuffer) LPSTR pchBuffer, __in LONG cchBuffer,
    __in UINT fuBuffer);
MMRESULT WINAPI mmioFlush( __in HMMIO hmmio, __in UINT fuFlush);
MMRESULT WINAPI mmioAdvance( __in HMMIO hmmio, __in_opt LPMMIOINFO pmmioinfo, __in UINT fuAdvance);
LRESULT WINAPI mmioSendMessage( __in HMMIO hmmio, __in UINT uMsg,
    __in_opt LPARAM lParam1, __in_opt LPARAM lParam2);
MMRESULT WINAPI mmioDescend( __in HMMIO hmmio, __inout LPMMCKINFO pmmcki,
    __in_opt const MMCKINFO FAR* pmmckiParent, __in UINT fuDescend);
MMRESULT WINAPI mmioAscend( __in HMMIO hmmio, __in LPMMCKINFO pmmcki, __in UINT fuAscend);
MMRESULT WINAPI mmioCreateChunk(__in HMMIO hmmio, __in LPMMCKINFO pmmcki, __in UINT fuCreate);

#endif  /* ifndef MMNOMMIO */

/****************************************************************************

                        DISPLAY Driver extensions

****************************************************************************/

#ifndef NEWTRANSPARENT
    #define NEWTRANSPARENT  3           /* use with SetBkMode() */

    #define QUERYROPSUPPORT 40          /* use to determine ROP support */
#endif  /* ifndef NEWTRANSPARENT */

/****************************************************************************

                        DIB Driver extensions

****************************************************************************/

#define SELECTDIB       41                      /* DIB.DRV select dib escape */
#define DIBINDEX(n)     MAKELONG((n),0x10FF)


/****************************************************************************

                        ScreenSaver support

    The current application will receive a syscommand of SC_SCREENSAVE just
    before the screen saver is invoked.  If the app wishes to prevent a
    screen save, return non-zero value, otherwise call DefWindowProc().

****************************************************************************/

#ifndef SC_SCREENSAVE

    #define SC_SCREENSAVE   0xF140

#endif  /* ifndef SC_SCREENSAVE */


#ifdef __cplusplus
}                       /* End of extern "C" { */
#endif  /* __cplusplus */

#ifdef _WIN32
#include <poppack.h>
#else
#ifndef RC_INVOKED
#pragma pack()
#endif
#endif

#endif  /* _INC_MMSYSTEM */



