

#include "StdAfx.h"


#include "MyLog.h"
#include "MBGlobals.h"
#include "MyString.h"
#include "FileUtils.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
//static char THIS_FILE[] = __FILE__;
#endif

MyLog::~MyLog() {
	if (m_ready)
		close();
}
void
MyLog::open(CString path) {

	CFileException e;

    CString file;
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
	int i;
	CString src = path;
	CString dst = path;
	CString mblog = "muzikbrowzer.log";
	for(i = 2 ; i > 0 ; i--) {
		src = path;
		dst = path;
		CString mbnumberedlogi = "muzikbrowzer" + numToString(i) + ".log";
		CString mbnumberedlogiplusone = 
			"muzikbrowzer" + numToString(i+1) + ".log";
		src = String::replace(path,mbnumberedlogi,mblog);
		dst = String::replace(path,mbnumberedlogiplusone,mblog);
		FileUtil::mv(src,dst);
	}
	src = path;
	dst = String::replace(path,"muzikbrowzer1.log",mblog);
	FileUtil::mv(src,dst);

	file = path;
    _pathfile = file;
    BOOL r = _file.Open(_pathfile, 
        CFile::modeCreate
//        |CFile::modeNoTruncate
        |CFile::modeWrite
//        |CFile::typeText
        |CFile::shareDenyNone,
        &e);

	m_ready = TRUE;
    log("started");
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
#ifndef _DEBUG
	return;
#endif
	log(m1,m2,m3,m4,m5,m6);
}
void
MyLog::log(const CString &m1, const CString &m2, 
		   const CString &m3, const CString &m4,
		   const CString &m5, const CString &m6
		   ) {
	if (m_ready == FALSE) return;
    CString mx;
    CTime t = CTime::GetCurrentTime();
    mx = t.Format("%Y%m%d:%H%M%S ");
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
	OutputDebugString(mx);
	_file.Write(mx,mx.GetLength());
    _file.Flush();
}
void
MyLog::ods(char *  m0) {
	CString m1(m0);
	ods(m1);
}
void
MyLog::ods(CString &  m0) {
    CString mx;
    CTime t = CTime::GetCurrentTime();
    mx = t.Format("%Y%m%d:%H%M%S ");
    mx += m0 ; 
	mx += "\r\n";
	OutputDebugString(mx);
}

void
MyLog::log(char * m1, char * m2,char*m3,char*m4,char*m5,char*m6) {
	CString M1(m1),M2(m2),M3(m3),M4(m4),M5(m5),M6(m6);
	log(M1,M2,M3,M4,M5,M6);
}

AutoLog::AutoLog(CString desc, BOOL todisk) : m_desc(desc), m_ToDisk(todisk)
{
	if (m_ToDisk) 
		logger.logd(m_desc + " begin");
	else
		logger.ods(m_desc + " begin");

}
AutoLog::~AutoLog() 
	{
		if (m_ToDisk)
			logger.logd(m_desc + " end");
		else 
			logger.ods(m_desc + " end");
	}
