
#include "stdafx.h"
#include "MusicPlayerWMP.h"
#include <math.h>
#include "wmpplayer4.h"
#include "wmpcontrols.h"
#include "wmpmedia.h"
#include "wmpsettings.h"
#include "util/MyLog.h"
#include "util/Misc.h"
#include "util/MyString.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////////////////////
// WMP
//////////////////////////////////////////////////////////////////////////////////////////

MusicPlayerWMP::MusicPlayerWMP(CWMPPlayer4 *pd, const CString & logpath) :
	m_MP(pd)
{
	m_FileLoaded= FALSE;
}

MusicPlayerWMP::~MusicPlayerWMP() 
{}

int
MusicPlayerWMP::init() 
{
	logger.log("WMP version:",m_MP->GetVersionInfo());
	logger.log("WMP status:" ,m_MP->GetStatus());
	return 0;
}

int
MusicPlayerWMP::InputOpen(const char *file) {
	m_file = file;
	logger.log("InputOpen ",m_file);
//	logger.log("InputOpen playcount:",numToString(m_MP->GetSettings().GetPlayCount()));

	m_MP->GetSettings().SetAutoStart(FALSE);
	CWMPMedia nm = m_MP->newMedia(m_file);
	m_MP->SetCurrentMedia(nm);
	m_FileLoaded = TRUE;
//	logger.log("InputOpen playcount:",numToString(m_MP->GetSettings().GetPlayCount()));
	logger.log("WMP status:" ,m_MP->GetStatus());

	return 1;
//	m_MP->ResetDirectShow();
//	HRESULT hr = m_MP->PrepareMedia((char*)file);
//	return !FAILED(hr);
}

int
MusicPlayerWMP::InputClose() {
	m_MP->close();
	m_FileLoaded = FALSE;
	return 0;
}

int
MusicPlayerWMP::Pause() {
	m_MP->GetControls().pause();
	return 0;
}

BOOL
MusicPlayerWMP::Play() {
//	m_MP->openPlayer(m_file);
	if (m_FileLoaded) 
		m_MP->GetControls().play();
	logger.log("WMP status:" ,m_MP->GetStatus());
	logger.log("WMP playstate:", numToString(m_MP->GetPlayState()));
	return TRUE;
}

int
MusicPlayerWMP::Stop() {
	m_MP->GetControls().stop();
	return 0;
}
BOOL
MusicPlayerWMP::isPlaying() {
	long s = m_MP->GetPlayState();
	return (s == 3);

}
BOOL
MusicPlayerWMP::isStopped() {
	long s = m_MP->GetPlayState();
	return (1 == s || 0 == s || 10 == s);
}

long
MusicPlayerWMP::GetFileDuration() {

	return (long)m_MP->GetCurrentMedia().GetDuration();
}
long 
MusicPlayerWMP::ReadMediaPosition(void)
{
	//return m_MP->ReadMediaPosition();
	return (long)m_MP->GetControls().GetCurrentPosition();
}
void
MusicPlayerWMP::ReadMediaPosition(long & pos, long & dur, long & pct)
{
	dur = GetFileDuration();
	pos = ReadMediaPosition();
	if (dur)
		pct = (100 * pos )/ dur;
	else
		pct = 0;
}
int
MusicPlayerWMP::Seek(long pos, BOOL /*endpos*/) {
	m_MP->GetControls().SetCurrentPosition(pos);
	return 0;
}
int
MusicPlayerWMP::SetVolume(int volume) {
	int dcb;
	if (volume >= 50) {
		dcb = 100;
	} else if (volume < 1) {
		dcb = 0;
	} else {
		dcb = volume * 2;
	}

	// WMP volume range == 0 - 100;
	m_MP->GetSettings().SetVolume(dcb);
	return 0;

}

long
MusicPlayerWMP::GetVolume(int & volume) {
	volume = m_MP->GetSettings().GetVolume();
	return volume;
}
