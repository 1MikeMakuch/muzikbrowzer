#ifndef _MYLOG_H_
#define _MYLOG_H_
#include "stdafx.h"

class MyLog {
	public:
		MyLog():m_ready(FALSE){};
		~MyLog(){};
		void open(CString);
		void log(const CString &msg1, const CString &msg2=CString(""), 
			const CString &msg3=CString(""), const CString &msg4=CString(""));
		void log(char * msg1, char * msg2=NULL);
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
