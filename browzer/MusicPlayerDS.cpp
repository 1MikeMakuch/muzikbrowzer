

#include "stdafx.h"

//#include "Jukebox.h"
#include <dshow.h>
#include "MusicPlayerDS.h"
#include "DirectShow.h"
#include "DirectShowMediaTypes.h"
#include "MBGlobals.h"
#include "util/MyLog.h"
#include "util/Misc.h"
#include <atlbase.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




/////////////////////////////////////////////////////////////////////////////
// MusicPlayerDS DirectShow code and message handlers

MusicPlayerDS::MusicPlayerDS(CWnd *p, const CString & logpath) 
		: m_PlayerDlg(p), g_rtTotalTime(0), m_SongStarted(FALSE),
		m_logpath(logpath), m_logfile(NULL)
{	
}

MusicPlayerDS::~MusicPlayerDS() 
{
}

void
MusicPlayerDS::OnClose()
{
    // Release DirectShow interfaces
    StopMedia();
    FreeDirectShow();

    // Release COM
    CoUninitialize();
}

BOOL
MusicPlayerDS::init()
{
    // Initialize COM
    CoInitialize(NULL);
	
    // Initialize DirectShow and query for needed interfaces
    HRESULT hr = InitDirectShow();
    if(FAILED(hr))
    {
        RetailOutput(TEXT("Failed to initialize DirectShow!  hr=0x%x\r\n"), hr);
        return FALSE;
    }
#ifdef _DEBUG
	CString dslogfile = m_logpath;
	dslogfile += "\\directshow.log";

	CFileException e;
    BOOL r = m_logcfile.Open(dslogfile, 
        CFile::modeCreate
        |CFile::modeWrite
        |CFile::shareDenyNone,
        &e);

	// Check if we could open the device
	if (r == FALSE) {
		CString e = "MusicPlayerDS::init() ";
		e += MBFormatError(::GetLastError());
		logger.log(e);
		m_logfile = NULL;
	} else {
		m_logfile = m_logcfile.m_hFile;
	}
#endif

    return TRUE;  // return TRUE  unless you set the focus to a control
}

void MusicPlayerDS::OnDestroy() 
{
    FreeDirectShow();	
}

HRESULT MusicPlayerDS::PrepareMedia(LPTSTR lpszMovie)
{
    USES_CONVERSION;
    HRESULT hr = S_OK;

    Say(TEXT("Loading..."));

    // Allow DirectShow to create the FilterGraph for this media file
	CString msg = "Playing ";
	msg += lpszMovie;
	logger.log(msg);
    hr = pGB->RenderFile(T2W(lpszMovie), NULL);
    if (FAILED(hr)) {
        RetailOutput(TEXT("*** Failed(%08lx) in RenderFile(%s)!\r\n"),
                 hr, lpszMovie);
        return hr;
    }
	// Have the graph signal event via window callbacks
	hr = pME->SetNotifyWindow((OAHWND)m_PlayerDlg->m_hWnd, WM_GRAPHNOTIFY, 0);
//		Returns S_OK if successful or E_INVALIDARG if the hwnd ;
	if (hr != S_OK) { 
		RetailOutput(TEXT(
			"*** Failed(%081x) in PrepareMedia,SetNotifyWindow\r\n"),
			hr);
	}
	return hr;
}


//
//  Displays a text string in a status line near the bottom of the dialog
//
void MusicPlayerDS::Say(LPTSTR szText)
{
	RetailOutput(szText);
}

void MusicPlayerDS::Pause() 
{  
    if (g_psCurrent == State_Paused)
    {
        RunMedia();
        Say(TEXT("Running"));
    }
    else
    {
        PauseMedia();
        Say(TEXT("PAUSED"));
    }
}

BOOL MusicPlayerDS::Play() 
{
	BOOL r = TRUE;
  	HRESULT hr = RunMedia();
	if (FAILED(hr)) {
		r = FALSE;
	} else {
		Say(TEXT("Running"));
		m_SongStarted = TRUE;
	}
	logger.log(GetVersion());
	return r;
}

void MusicPlayerDS::ShowState()
{
    HRESULT hr;

    OAFilterState fs;
    hr = pMC->GetState(500, &fs);
    if (FAILED(hr))
    {
        RetailOutput(TEXT("Failed to read graph state!  hr=0x%x\r\n"), hr);
        return;
    }

    // Show debug output for current media state
    switch (fs)
    {
        case State_Stopped:
            RetailOutput(TEXT("State_Stopped\r\n"));
            break;
        case State_Paused:
            RetailOutput(TEXT("State_Paused\r\n"));
            break;
        case State_Running:
            RetailOutput(TEXT("State_Running\r\n"));
            break;
    }
}

void MusicPlayerDS::Stop() 
{
    HRESULT hr;

//	Stop playback immediately with IMediaControl::Stop().
    StopMedia();

    // Wait for the stop to propagate to all filters
    OAFilterState fs;
    hr = pMC->GetState(500, &fs);
    if (FAILED(hr))
    {
        RetailOutput(TEXT("Failed to read graph state!  hr=0x%x\r\n"), hr);
    }

    // Reset to beginning of media clip
    LONGLONG pos=0;
    hr = pMS->SetPositions(&pos, AM_SEEKING_AbsolutePositioning ,
                           NULL, AM_SEEKING_NoPositioning);
    if (FAILED(hr))
    {
        RetailOutput(TEXT("Failed to seek to beginning of media!  hr=0x%x\r\n"), hr);
    }
  
    // Display the first frame of the media clip, if it contains video.
    // StopWhenReady() pauses all filters internally (which allows the video
    // renderer to queue and display the first video frame), after which
    // it sets the filters to the stopped state.  This enables easy preview
    // of the video's poster frame.
    hr = pMC->StopWhenReady();
    if (FAILED(hr))
    {
        RetailOutput(TEXT("Failed in StopWhenReady!  hr=0x%x\r\n"), hr);
    }

    Say(TEXT("Stopped"));

    // Reset slider bar and position label back to zero
    ReadMediaPosition();
}

HRESULT MusicPlayerDS::InitDirectShow(void)
{
    HRESULT hr = S_OK;

    g_bAudioOnly = TRUE;

    JIF(CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC,
		IID_IGraphBuilder, (void **)&pGB));
    JIF(pGB->QueryInterface(IID_IMediaControl,  (void **)&pMC));
    JIF(pGB->QueryInterface(IID_IMediaSeeking,  (void **)&pMS));
    JIF(pGB->QueryInterface(IID_IMediaEventEx,  (void **)&pME));
	JIF(pGB->QueryInterface(IID_IBasicAudio,    (void **)&pBA));

	if (m_logfile)
		pGB->SetLogFile(m_logfile);

    return S_OK;

CLEANUP:
    FreeDirectShow();
    return(hr);
}

HRESULT MusicPlayerDS::FreeDirectShow(void)
{
    HRESULT hr=S_OK;

    StopMedia();

    // Disable event callbacks
    if (pME)
        hr = pME->SetNotifyWindow((OAHWND)NULL, 0, 0);

    SAFE_RELEASE(pMC);
    SAFE_RELEASE(pMS);
    SAFE_RELEASE(pME);
	SAFE_RELEASE(pBA);
    SAFE_RELEASE(pGB);

    return hr;
}

void MusicPlayerDS::ResetDirectShow(void)
{
    // Destroy the current filter graph its filters.
    FreeDirectShow();

    // Reinitialize graph builder and query for interfaces
    InitDirectShow();
}


HRESULT MusicPlayerDS::HandleGraphEvent(void)
{
    LONG evCode, evParam1, evParam2;
    HRESULT hr=S_OK;

	// Since we may have a scenario where we're shutting down the application,
	// but events are still being generated, make sure that the event
	// interface is still valid before using it.  It's possible that
	// the interface could be freed during shutdown but later referenced in
	// this callback before the app completely exits.
	if (!pME)
		return S_OK;
    
    while(SUCCEEDED(pME->GetEvent(&evCode, (LONG_PTR *) &evParam1, 
                    (LONG_PTR *) &evParam2, 0)))
    {
        // Spin through the events
        hr = pME->FreeEventParams(evCode, evParam1, evParam2);

        if(EC_COMPLETE == evCode)
        {
			Stop();
			m_PlayerDlg->PostMessage(WM_PLAYLOOP, (WPARAM)0, (LPARAM)0);
        }
    }
    return hr;
}

HRESULT MusicPlayerDS::DisplayFileDuration(void)
{
    HRESULT hr;

    if (!pMS)
        return E_NOINTERFACE;

    // Is media time supported for this file?
    if (S_OK != pMS->IsFormatSupported(&TIME_FORMAT_MEDIA_TIME))
        return E_NOINTERFACE;

    // Read the time format to restore later
    GUID guidOriginalFormat;
    hr = pMS->GetTimeFormat(&guidOriginalFormat);
    if (FAILED(hr))
        return hr;

    // Ensure media time format for easy display
    hr = pMS->SetTimeFormat(&TIME_FORMAT_MEDIA_TIME);
    if (FAILED(hr))
        return hr;

    // Read the file's duration
    LONGLONG llDuration;
    hr = pMS->GetDuration(&llDuration);
    if (FAILED(hr))
        return hr;

    // Return to the original format
    if (guidOriginalFormat != TIME_FORMAT_MEDIA_TIME)
    {
        hr = pMS->SetTimeFormat(&guidOriginalFormat);
        if (FAILED(hr))
            return hr;
    }

    // Convert the LONGLONG duration into human-readable format
    unsigned long nTotalMS = (unsigned long) llDuration / 10000; // 100ns -> ms
    int nMS = nTotalMS % 1000;
    int nSeconds = nTotalMS / 1000;
    int nMinutes = nSeconds / 60;
    nSeconds %= 60;

    return hr;
}
int MusicPlayerDS::GetFileDuration(void)
{
    HRESULT hr;

    if (!pMS)
        return -1;

    // Is media time supported for this file?
    if (S_OK != pMS->IsFormatSupported(&TIME_FORMAT_MEDIA_TIME))
        return -1;

    // Read the time format to restore later
    GUID guidOriginalFormat;
    hr = pMS->GetTimeFormat(&guidOriginalFormat);
    if (FAILED(hr))
        return -1;

    // Ensure media time format for easy display
    hr = pMS->SetTimeFormat(&TIME_FORMAT_MEDIA_TIME);
    if (FAILED(hr))
        return -1;

    // Read the file's duration
    LONGLONG llDuration;
    hr = pMS->GetDuration(&llDuration);
    if (FAILED(hr))
        return -1;

    // Return to the original format
    if (guidOriginalFormat != TIME_FORMAT_MEDIA_TIME)
    {
        hr = pMS->SetTimeFormat(&guidOriginalFormat);
        if (FAILED(hr))
            return -1;
    }
	g_rtTotalTime = llDuration;

    // Convert the LONGLONG duration into human-readable format
    unsigned long nTotalMS = (unsigned long) llDuration / 10000; // 100ns -> ms
    int nMS = nTotalMS % 1000;
    int nSeconds = nTotalMS / 1000;

    return nSeconds;
}





void MusicPlayerDS::DisplayECEvent(long lEventCode, long lParam1, long lParam2)
{
    static TCHAR szMsg[256];
    BOOL bMatch = TRUE;

#define HANDLE_EC(c)                              \
    case c:                                       \
        wsprintf(szMsg, TEXT("%s\0"), TEXT(#c));  \
        break;

    switch (lEventCode)
    {
        HANDLE_EC(EC_ACTIVATE);
        HANDLE_EC(EC_BUFFERING_DATA);
        HANDLE_EC(EC_CLOCK_CHANGED);
        HANDLE_EC(EC_COMPLETE);
        HANDLE_EC(EC_DEVICE_LOST);
        HANDLE_EC(EC_DISPLAY_CHANGED);
        HANDLE_EC(EC_END_OF_SEGMENT);
        HANDLE_EC(EC_ERROR_STILLPLAYING);
        HANDLE_EC(EC_ERRORABORT);
        HANDLE_EC(EC_EXTDEVICE_MODE_CHANGE);
        HANDLE_EC(EC_FULLSCREEN_LOST);
        HANDLE_EC(EC_GRAPH_CHANGED);
        HANDLE_EC(EC_LENGTH_CHANGED);
        HANDLE_EC(EC_NEED_RESTART);
        HANDLE_EC(EC_NOTIFY_WINDOW);
        HANDLE_EC(EC_OLE_EVENT);
        HANDLE_EC(EC_OPENING_FILE);
        HANDLE_EC(EC_PALETTE_CHANGED);
        HANDLE_EC(EC_PAUSED);
        HANDLE_EC(EC_QUALITY_CHANGE);
        HANDLE_EC(EC_REPAINT);
        HANDLE_EC(EC_SEGMENT_STARTED);
        HANDLE_EC(EC_SHUTTING_DOWN);
        HANDLE_EC(EC_SNDDEV_IN_ERROR);
        HANDLE_EC(EC_SNDDEV_OUT_ERROR);
        HANDLE_EC(EC_STARVATION);
        HANDLE_EC(EC_STEP_COMPLETE);
        HANDLE_EC(EC_STREAM_CONTROL_STARTED);
        HANDLE_EC(EC_STREAM_CONTROL_STOPPED);
        HANDLE_EC(EC_STREAM_ERROR_STILLPLAYING);
        HANDLE_EC(EC_STREAM_ERROR_STOPPED);
        HANDLE_EC(EC_TIMECODE_AVAILABLE);
        HANDLE_EC(EC_USERABORT);
        HANDLE_EC(EC_VIDEO_SIZE_CHANGED);
        HANDLE_EC(EC_WINDOW_DESTROYED);

    default:
        bMatch = FALSE;
        RetailOutput(TEXT("  Received unknown event code (0x%x)\r\n"), lEventCode);
        break;
    }
}

long MusicPlayerDS::ReadMediaPosition()
{
    HRESULT hr;
    REFERENCE_TIME rtNow;

    // Read the current stream position
    hr = pMS->GetCurrentPosition(&rtNow);
    if (FAILED(hr))
        return 0;

	long lTick = 0;
    // Convert position into a percentage value and update slider position
    if (g_rtTotalTime != 0)
    {
        lTick = (long)((rtNow * 100) / g_rtTotalTime);
    }
	return lTick;
}
void MusicPlayerDS::ReadMediaPosition(long & positionseconds,
						long & durationseconds, long & positionpct) {
    HRESULT hr;
    REFERENCE_TIME rtNow;

    // Read the current stream position
    hr = pMS->GetCurrentPosition(&rtNow);
    if (FAILED(hr))
        return ;

    // Convert position into a percentage value and update slider position
    if (g_rtTotalTime != 0)
    {
        positionpct = (long)((rtNow * 100) / g_rtTotalTime);
    }
    positionseconds = (unsigned long) (rtNow         / 10000000);
	durationseconds = (unsigned long) (g_rtTotalTime / 10000000);

	if (m_SongStarted) {
		m_SongStarted = FALSE;
		EnumFilters (pGB);
	}
	return;
}

void MusicPlayerDS::UpdatePosition(REFERENCE_TIME rtNow) 
{
    HRESULT hr;

    // If no reference time was passed in, read the current position
    if (rtNow == 0)
    {
        // Read the current stream position
        hr = pMS->GetCurrentPosition(&rtNow);
        if (FAILED(hr))
            return;
    }

    // Convert the LONGLONG duration into human-readable format
    unsigned long nTotalMS = (unsigned long) rtNow / 10000; // 100ns -> ms
    int nSeconds = nTotalMS / 1000;
    int nMinutes = nSeconds / 60;
    nSeconds %= 60;

}
BOOL
MusicPlayerDS::isPlaying() {
    HRESULT hr;

    OAFilterState fs;
    hr = pMC->GetState(500, &fs);
    if (FAILED(hr))
    {
        RetailOutput(TEXT("Failed to read graph state!  hr=0x%x\r\n"), hr);
        return FALSE;
    }
	if (fs == State_Running) return TRUE;
	return FALSE;
}
BOOL
MusicPlayerDS::isStopped() {
    HRESULT hr;

    OAFilterState fs;
    hr = pMC->GetState(500, &fs);
    if (FAILED(hr))
    {
        RetailOutput(TEXT("Failed to read graph state!  hr=0x%x\r\n"), hr);
        return FALSE;
    }
	if (fs == State_Stopped) return TRUE;
	return FALSE;
}
int
MusicPlayerDS::Seek(long pos, BOOL endpos) {
    HRESULT hr;
    static OAFilterState state;
    static BOOL bStartOfScroll = TRUE;
	
    // If the file is not seekable, the trackbar is disabled. 
	
    // Pause when the scroll action begins.
    if (bStartOfScroll) 
    {       
        hr = pMC->GetState(10, &state);
        bStartOfScroll = FALSE;
        hr = pMC->Pause();
    }
    
    // Update the position continuously.
    REFERENCE_TIME rtNew = (g_rtTotalTime * pos) / 100;
	
    hr = pMS->SetPositions(&rtNew, AM_SEEKING_AbsolutePositioning,
		NULL,   AM_SEEKING_NoPositioning);
	
    // Restore the state at the end.
    if (endpos == TRUE)
    {
        if (state == State_Stopped)
            hr = pMC->Stop();
        else if (state == State_Running) 
            hr = pMC->Run();
		
        bStartOfScroll = TRUE;
    }
	
	return 0;
}

//The volume value is in 1/100ths of decibels and ranges from -10000 to 0.
HRESULT
MusicPlayerDS::SetVolume(long level) {
	if (isPlaying()) {
		return pBA->put_Volume(level);
	} else {
		return 0;
	}
}
HRESULT
MusicPlayerDS::GetVolume(long & level) {
	if (isPlaying()) {
		return pBA->get_Volume(&level);
	} else {
		return 0;
	}
}

void
MusicPlayerDS::EnumFilters (IFilterGraph *pGraph) 
{
    IEnumFilters *pEnum = NULL;
    IBaseFilter *pFilter;
    ULONG cFetched;

    pGraph->EnumFilters(&pEnum);
    while(pEnum->Next(1, &pFilter, &cFetched) == S_OK)
    {
        FILTER_INFO FilterInfo;
        char szName[256];
        
        pFilter->QueryFilterInfo(&FilterInfo);
        WideCharToMultiByte(CP_ACP, 0, FilterInfo.achName, -1, szName, 256, 0, 0);
		CString msg = "DS filter:";
		msg += szName;
		logger.log(msg);
//		FilterProps(pFilter);
//        MessageBox(NULL, szName, "Filter name", MB_OK);

        FilterInfo.pGraph->Release();
        pFilter->Release();
    }
    pEnum->Release();
}

void MusicPlayerDS::FilterProps(IBaseFilter *pFilter) 
{
    HRESULT hr;
//    TCHAR szNameToFind[128];
    ISpecifyPropertyPages *pSpecify;
	if (!pFilter)
		return;

    // Discover if this filter contains a property page
    hr = pFilter->QueryInterface(IID_ISpecifyPropertyPages,
		(void **)&pSpecify);
    if (SUCCEEDED(hr)) 
    {
        do 
        {
            FILTER_INFO FilterInfo;
            hr = pFilter->QueryFilterInfo(&FilterInfo);
            if (FAILED(hr))
                break;

            CAUUID caGUID;
            hr = pSpecify->GetPages(&caGUID);
            if (FAILED(hr))
                break;

            pSpecify->Release();
        
            // Display the filter's property page
            OleCreatePropertyFrame(
                m_PlayerDlg->m_hWnd,                 // Parent window
                0,                      // x (Reserved)
                0,                      // y (Reserved)
                FilterInfo.achName,     // Caption for the dialog box
                1,                      // Number of filters
                (IUnknown **)&pFilter,  // Pointer to the filter 
                caGUID.cElems,          // Number of property pages
                caGUID.pElems,          // Pointer to property page CLSIDs
                0,                      // Locale identifier
                0,                      // Reserved
                NULL                    // Reserved
				);
            CoTaskMemFree(caGUID.pElems);
            FilterInfo.pGraph->Release(); 

        } while(0);
    }

    pFilter->Release();
}


extern DWORD GetDXVersion();




//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: Entry point to the program. Initializes everything, and pops
//       up a message box with the results of the GetDXVersion call
//-----------------------------------------------------------------------------
CString MusicPlayerDS::GetVersion()
{
    CString strResult;
    DWORD  dwDXVersion = GetDXVersion();

    switch( dwDXVersion )
    {
        case 0x000:
            strResult = _T("No DirectX installed" );
            break;
        case 0x100:
            strResult = _T("DirectX 1 installed" );
            break;
        case 0x200:
            strResult = _T("DirectX 2 installed" );
            break;
        case 0x300:
            strResult = _T("DirectX 3 installed" );
            break;
        case 0x500:
            strResult = _T("DirectX 5 installed" );
            break;
        case 0x600:
            strResult = _T("DirectX 6 installed" );
            break;
        case 0x601:
            strResult = _T("DirectX 6.1 installed" );
            break;
        case 0x700:
            strResult = _T("DirectX 7" );
            break;
        case 0x800:
            strResult = _T("DirectX 8.0 installed" );
            break;
        case 0x801:
            strResult = _T("DirectX 8.1 or better installed" );
            break;
        default:
            strResult = _T("Unknown version of DirectX installed." );
            break;
    }
	return strResult;


    

}



