//------------------------------------------------------------------------------
// File: Globals.h
//
// Desc: DirectShow sample code - global data for Jukebox application.
//
// Copyright (c) 1998-2001 Microsoft Corporation.  All rights reserved.
//------------------------------------------------------------------------------


#include "stdafx.h"
#include <Dshow.h>

#include "DirectShow.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
// Global data
//
IGraphBuilder *pGB = NULL;
IMediaSeeking *pMS = NULL;
IMediaControl *pMC = NULL;
IMediaEventEx *pME = NULL;
//IBasicVideo   *pBV = NULL;
//IVideoWindow  *pVW = NULL;
IBasicAudio   *pBA = NULL;
IBaseFilter * pAsfFilter = NULL;
IFileSourceFilter * pAsfSource = NULL;

FILTER_STATE g_psCurrent=State_Stopped;

BOOL g_bLooping=FALSE,
     g_bAudioOnly=FALSE,
     g_bDisplayEvents=FALSE,
     g_bGlobalMute=FALSE,
     g_bPlayThrough=FALSE;
