

#include "StdAfx.h"


#include "MyLog.h"
#include "MBGlobals.h"
#include "muzikbrowzerVersion.h"
#include "MyString.h"
#include "FileUtils.h"
#include <afxmt.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
//static char THIS_FILE[] = __FILE__;
#endif

#define MBMAXLOGFILE 1000000

MyLog::~MyLog() {
	if (m_ready)
		close();
}
BOOL
MyLog::open(CString path, BOOL truncate) {

	CFileException e;

	if (path == "") {
        TCHAR szBuff[_MAX_PATH];
        if ((::GetModuleFileName(NULL, szBuff, _MAX_PATH)) != 0) {
			int n = strlen(szBuff);
			int i;
			for (i = n-1 ; i > 0 ; --i) {
				if (szBuff[i] == '\\') {
					szBuff[i] = 0;
					i = 0;
				}
			}
			path = szBuff;
			path += "\\";
			path += MUZIKBROWZER;
			path += ".log";
		}
	}
	
    _pathfile = path;
	m_ready = TRUE;
	if (TRUE == truncate) {
		m_ready = _file.Open(_pathfile, 
	        CFile::modeCreate
			|CFile::modeReadWrite
			|CFile::shareDenyNone,
			&e);
	} else {
		m_ready = _file.Open(_pathfile, 
			CFile::modeCreate
			|CFile::modeNoTruncate
			|CFile::modeReadWrite
			|CFile::shareDenyNone,
			&e);
	}
	if (m_ready != 0) {
		_file.SeekToEnd();
		return TRUE;
	} else {
		return FALSE;
	}
}

void
MyLog::close() {
	_file.Close();
}

void
MyLog::logd(const CString &m1, const CString &m2, 
		   const CString &m3, const CString &m4,
		   const CString &m5, const CString &m6
		   ) {

	if (m_loglevel < 1)
		return;

	log(m1,m2,m3,m4,m5,m6);
}
CCriticalSection MyLogCS;
void
MyLog::log(const CString &m1, const CString &m2, 
		   const CString &m3, const CString &m4,
		   const CString &m5, const CString &m6
		   ) {
	if (m_ready == FALSE) return;

    CString mx;
	if (m_DTStamp) {
		CTime t = CTime::GetCurrentTime();
		mx = t.Format("%Y%m%d:%H%M%S ");
	}
    mx += m1 ; 
	if (m2.GetLength()) {
		mx += " " ;	mx += m2 ; 
		if (m3.GetLength()) {
			mx += " " ;	mx += m3 ;
			if (m4.GetLength()) {
				mx += " ";	mx += m4 ; 
				if (m5.GetLength()) {
					mx += " ";	mx += m5 ; 
					if (m6.GetLength()) {
						mx += " "; mx += m6;
					}
				}
			}
		}
	}
	mx += "\r\n";
	if (!m_NoODS)
		OutputDebugString(mx);

	CSingleLock lock(&MyLogCS);
	lock.Lock();
	trimIt();
	_file.SeekToEnd();
	_file.Write(mx,mx.GetLength());
    _file.Flush();
	lock.Unlock();
}
void
MyLog::trimIt() {
	if (!m_trim) return;
	UINT cursize = _file.GetLength();
	if (cursize > MBMAXLOGFILE) {
		int halfsize = cursize / 2;
		_file.Seek(-halfsize,CFile::end);
		AutoBuf buf(halfsize);
		_file.Read(buf.p, halfsize);
		_file.Close();
		open(_pathfile, TRUE);
		_file.Write(buf.p,halfsize);
		log(CS("Muzikbrowzer version: ") + CS(MUZIKBROWZER_VERSION));
	}
}
void
MyLog::ods(char *  m0) {
#ifdef _DEBUG
	CString m1(m0);
	ods(m1);
#endif
}
void
MyLog::ods(CString &  m0) {
#ifdef _DEBUG
    CString mx;
    CTime t = CTime::GetCurrentTime();
    mx = t.Format("%Y%m%d:%H%M%S ");
    mx += m0 ; 
	mx += "\r\n";
	OutputDebugString(mx);
#endif
}
void
MyLog::odsRect(const CString & name, const CRect & rect) {
#ifdef _DEBUG
	CString m(name);
	m += " L:";
	m += numToString(rect.left) + " T:";
	m += numToString(rect.top) + " W:";
	m += numToString(rect.Width()) + " H:";
	m += numToString(rect.Height());
	ods(m);
	return;
#endif
}
void
MyLog::odsRGB(const COLORREF c) {
#ifdef _DEBUG
	CString t = "RGB: ";
	t += numToString(GetRValue(c)) + ",";
	t += numToString(GetGValue(c)) + ",";
	t += numToString(GetBValue(c));
	ods(t);
#endif
}

void
MyLog::log(char * m1, char * m2,char*m3,char*m4,char*m5,char*m6) {
	CString M1(m1),M2(m2),M3(m3),M4(m4),M5(m5),M6(m6);
	log(M1,M2,M3,M4,M5,M6);
}

AutoLog::AutoLog(const CString & desc) : m_desc(desc)
{
	CString tab = String::numspaces(AutoLogTabCounter);
	logger.logd(tab + m_desc + " begin");
	AutoLogTabCounter++;
	if (AutoLogTabCounter > 20)
		AutoLogTabCounter = 0;
}
AutoLog::~AutoLog() {
	AutoLogTabCounter--;
	if (AutoLogTabCounter < 0)
		AutoLogTabCounter =0;
	CString tab = String::numspaces(AutoLogTabCounter);
	logger.logd(tab + m_desc + " end");

}
