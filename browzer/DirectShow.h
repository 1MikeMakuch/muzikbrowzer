//------------------------------------------------------------------------------
// File: PlayVideo.h
//
// Desc: DirectShow sample code - declarations for media control functions.
//
// Copyright (c) 1998-2001 Microsoft Corporation.  All rights reserved.
//------------------------------------------------------------------------------


#ifndef PLAY_VID_H
#define PLAY_VID_H

//
// Constants
//
#define VOLUME_FULL     0L
#define VOLUME_SILENCE  -10000L



//
// Macros
//
#define SAFE_RELEASE(i) {if (i) i->Release(); i = NULL;}

#define JIF(x) if (FAILED(hr=(x))) \
    {RetailOutput(TEXT("FAILED(0x%x) ") TEXT(#x) TEXT("\n"), hr); goto CLEANUP;}

//
// Global data
//
extern IGraphBuilder *pGB;
extern IMediaSeeking *pMS;
extern IMediaControl *pMC;
extern IMediaEventEx *pME;
//extern IBasicVideo   *pBV;
extern IBasicAudio   *pBA;
//extern IVideoWindow  *pVW;

extern FILTER_STATE g_psCurrent;
extern BOOL g_bLooping, g_bAudioOnly, g_bPlayThrough;
extern BOOL g_bDisplayEvents, g_bGlobalMute;


//-----------------------------------------------------------------------------
// External function-prototypes
//-----------------------------------------------------------------------------
HRESULT RunMedia(void);
HRESULT StopMedia(void);
HRESULT PauseMedia(void);
HRESULT PlayMedia(LPTSTR lpszMovie, HINSTANCE hInstance);
HRESULT CheckMovieState(BOOL *pbComplete);
HRESULT GetInterfaces(void);
HRESULT MuteAudio(void);
HRESULT ResumeAudio(void);
void CleanupInterfaces(void);
void ToggleFullscreen(void);

void RetailOutput(const TCHAR *tszErr, ...);

#endif // !defined(PLAY_VID_H)
