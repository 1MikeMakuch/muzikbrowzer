
#include "stdafx.h"
#include "MusicPlayer.h"
#include "Mp3Header.h"
#include <math.h>
//#include "PlayerDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////////////////
// Xaudio
///////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
// DirectShow
//////////////////////////////////////////////////////////////////////////////////////////

MusicPlayer::MusicPlayer(CWnd *pd, const CString & logpath) :
	/* m_Instance(instance), */ m_PlayerDlg(pd)
{
//	try {
//	m_MP = new MusicPlayerDS(instance, pd);
//	} catch (XaudioException) {
//		pd->EndDialog(0);
//	}
	m_MP = new MusicPlayerDS(m_PlayerDlg, logpath);
}

MusicPlayer::~MusicPlayer() 
{
	m_MP->OnClose();
	delete m_MP;
	m_MP = 0;
	m_PlayerDlg = 0;
}

int
MusicPlayer::init() 
{
	return m_MP->init();
}

int
MusicPlayer::InputOpen(const char *file) {
	m_MP->ResetDirectShow();
	HRESULT hr = m_MP->PrepareMedia((char*)file);
	return !FAILED(hr);
}

int
MusicPlayer::InputClose() {
	m_MP->ResetDirectShow();
//		m_PlayerDlg->StopSeekTimer();
	return 0;
	
}

int
MusicPlayer::Pause() {
	m_MP->Pause();
//	m_PlayerDlg->StopSeekTimer();
	return 0;
	
}

BOOL
MusicPlayer::Play() {
//	m_PlayerDlg->StartSeekTimer();
	if (m_MP->Play()) {
		return TRUE;
	}
	return FALSE;
}
//int
//MusicPlayer::Seek(unsigned long offset, unsigned long range) {
//	return m_MP->Seek(offset,range);
//}
int
MusicPlayer::Stop() {
	m_MP->Stop();
//	m_PlayerDlg->StopSeekTimer();
	return 0;
}
BOOL
MusicPlayer::isPlaying() {
    return m_MP->isPlaying();
}
BOOL
MusicPlayer::isStopped() {
	return m_MP->isStopped();
}

int
MusicPlayer::GetFileDuration() {

    return m_MP->GetFileDuration();

}
long 
MusicPlayer::ReadMediaPosition(void)
{
	return m_MP->ReadMediaPosition();
}
void
MusicPlayer::ReadMediaPosition(long & one, long & two, long & three)
{
	m_MP->ReadMediaPosition(one,two,three);
}
int
MusicPlayer::Seek(long pos, BOOL endpos) {
	return m_MP->Seek(pos, endpos);
}
int
MusicPlayer::SetVolume(int volume) {
	long dcb;
	if (volume >= 50.0) {
		dcb = 0;
	} else if (volume <= 1) {
		dcb = -10000;
	} else {
		dcb = (long) ((log(volume) / log(50)) * 10000) - 10000;
	}


	HRESULT hr = m_MP->SetVolume(dcb);
	return !FAILED(hr);
}
#define M_E        2.7182818284590452354 
int
MusicPlayer::GetVolume(int & volume) {
	long dcb;
	HRESULT hr = m_MP->GetVolume(dcb);
	if (FAILED(hr))
		return FAILED(hr);

	double tmp = log(50) * ((10000 + dcb) / 10000);
	volume = int(pow(M_E, tmp));
	return 0;
}
