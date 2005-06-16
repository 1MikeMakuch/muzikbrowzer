#ifndef _MYLOG_H_
#define _MYLOG_H_
#include "stdafx.h"

class MyLog {
	public:
		MyLog():m_ready(FALSE){};
		~MyLog(){};
		void open(CString);
		void log(
			const CString &msg1,
			const CString &msg2=CString(""),
			const CString &msg3=CString(""),
			const CString &msg4=CString(""),
			const CString &msg5=CString(""),
			const CString &msg6=CString(""));
		void logd(
			const CString &msg1,
			const CString &msg2=CString(""),
			const CString &msg3=CString(""),
			const CString &msg4=CString(""),
			const CString &msg5=CString(""),
			const CString &msg6=CString(""));
		void log(char * msg1,
			char * msg2=NULL,
			char * msg3=NULL,
			char * msg4=NULL,
			char * msg5=NULL,
			char * msg6=NULL);
        CString location() { return _pathfile; }
	private:
		BOOL m_ready;
		CFile _file;
        CString _pathfile;
};

class AutoLog {
public:
	AutoLog(CString desc);
	~AutoLog();
private:
	CString m_desc;
};

#ifdef LOGOWNER
MyLog logger;
#else
extern MyLog logger;
#endif

#endif
