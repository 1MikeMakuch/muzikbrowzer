#ifndef _MYLOG_H_
#define _MYLOG_H_
#include "stdafx.h"

class MyLog {
	public:
		MyLog():m_ready(FALSE){};
		~MyLog(){};
		void open(CString);
		void log(CString);
        CString location() { return _pathfile; }
	private:
		BOOL m_ready;
		CFile _file;
        CString _pathfile;
};

#ifdef LOGOWNER
MyLog logger;
#else
extern MyLog logger;
#endif

#endif
