

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
MyLog::log(const CString &msg1, const CString &msg2, const CString &msg3, const CString &msg4) {
	if (m_ready == FALSE) return;
    CString msgx;
    CTime t = CTime::GetCurrentTime();
    msgx = t.Format("%Y%m%d:%H%M%S ");
    msgx += msg1;
	msgx += msg2;
	msgx += msg3;
	msgx += msg4;
	msgx += "\r\n";
	_file.Write(msgx,msgx.GetLength());
    _file.Flush();
}

void
MyLog::log(char * msg1, char * msg2) {
	CString Msg1(msg1),Msg2(msg2);
	log(Msg1,Msg2);
}