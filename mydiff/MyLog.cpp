

#include "StdAfx.h"


#include "MyLog.h"
#include "MBGlobals.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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
		}
	}
	file = path;
	file += "\\";
	file += MUZIKBROWZER;
    file += ".log";
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
MyLog::log(CString msg) {
	if (m_ready == FALSE) return;
    CString msgx;
    CTime t = CTime::GetCurrentTime();
    msgx = t.Format("%d%m%y:%H%M%S ");
    msgx += msg;
	msgx += "\r\n";
	_file.Write(msgx,msgx.GetLength());
    _file.Flush();
}
