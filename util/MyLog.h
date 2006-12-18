#ifndef _MYLOG_H_
#define _MYLOG_H_
#include "stdafx.h"

class MyLog {
	public:
		MyLog():m_ready(FALSE),m_DTStamp(TRUE),m_trim(TRUE),
		m_NoODS(FALSE){};
		~MyLog();
		BOOL open(CString, BOOL trunc=FALSE);
		void close();
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
		void ods(CString &);
		void ods(char * msg);
		void odsRect(const CString & name, const CRect & rect);
		void odsRGB(const COLORREF c);
		CString _pathfile;
		BOOL m_DTStamp;
		BOOL m_trim;
		BOOL m_NoODS;
	private:
		BOOL m_ready;
		CFile _file;
		void trimIt();



};

class AutoLog {
public:
	AutoLog(CString desc, BOOL todisk=FALSE);
	~AutoLog();
private:
	CString m_desc;
	BOOL m_ToDisk;
};

#ifdef LOGOWNER
MyLog logger;
#else
extern MyLog logger;
#endif

#define ODS(x) {\
	CString msg(x);\
	msg += "\r\n";\
	OutputDebugString(x);



#endif
