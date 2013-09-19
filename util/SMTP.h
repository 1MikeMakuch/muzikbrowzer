#ifndef _SEND_SMTP_H_
#define _SEND_SMTP_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// sendSMTP.h : header file
//

#include "stdafx.h"
#include <afxsock.h>
#include "MSocket.h"
#include "MyString.h"
/////////////////////////////////////////////////////////////////////
// CSMTP command target

class CSMTP : protected MSocket
{
// Attributes
public:

// Operations
public:
    CSMTP();
    virtual ~CSMTP();

// Overrides
public:
		int mail(const CString & server,
		const CString & from,
		const CString & to,
		const CString & headers,
		const CString & body);

		BOOL http(CStringArray & rhdr, CStringArray & rbody,
			const CString & server,
					const CString & headers="",
					const CString & url="",
					const CString & body="");
		BOOL entityBodyParse(MyHash & hash, const CString & body);
		void bodyAddKV(CString & body, const CString & key, const CString & val);

private:
	CString WellFormedAddress(const CString & address);
    BOOL OpenConnection(const CString & szMailServerIPAddress, int port);
    CString GetLastResponse();
    BOOL SendLine(const CString & szMessage);
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CSMTP)
    public:
    //}}AFX_VIRTUAL

    // Generated message map functions
    //{{AFX_MSG(CSMTP)
        // NOTE - the ClassWizard will add and remove member

    //}}AFX_MSG

// Implementation
protected:
    BOOL ReceiveResponse();
    CString szResponse;
	CString m_log;
};

class MyHttp
{
public:
	static BOOL gotoUrl(const CString & url, int showcmd);
};

/////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations


#endif // !defined(AFX_SENDSMTP_H__1BB24553_B9A7_11D1_8A04_
       // FA371DB30444__INCLUDED_)
//End of File
