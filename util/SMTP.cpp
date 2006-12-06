// sendSMTP.cpp : implementation file
//

#include "stdafx.h"
#include "SMTP.h"
#include "MyString.h"
#include "Misc.h"
#include "MBMessageBox.h"
#include "MyLog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////
// CSMTP

CSMTP::CSMTP()
{
}

CSMTP::~CSMTP()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CSMTP, CSocket)
    //{{AFX_MSG_MAP(CSMTP)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif  // 0

/////////////////////////////////////////////////////////////////////
// CSMTP member functions


BOOL CSMTP::SendLine(const CString & szMessage)
{
    BOOL Status;

    // allocate enough space for the command plus enough to add a \n
    // char

	CString msg = szMessage;
	msg += "\r\n";
	m_log += msg;

    // Send the message
    if(Send((LPCTSTR)msg, msg.GetLength()) != SOCKET_ERROR)
    {
        // if there wasn't an error get the response;
        Status=ReceiveResponse();
    }
    else
    {
        Status=false;
    }

    return Status;
}

BOOL CSMTP::ReceiveResponse()
{
    // I only care about the numeric value
    int numReceived=Receive(szResponse);
	m_log += szResponse;
    if(numReceived == szResponse.GetLength())
    {
        // I have more data to get,  I will want to flush the rest,
        // but I don't really need it.
    }

    BOOL Status=false;

	char ch = szResponse.GetAt(0);
    switch(ch)
    {
    case '1':       // not possible, but code for anyways
    case '2':       // positive response
    case '3':       // intermediate response, but go ahead and
                    // continue
        Status=true;
        break;
    case '4':
    case '5':
        Status=false;
        break;
    default:
        // if anything other than the above happen, assume a failure
        Status=false;
    }

    return Status;
}

CString CSMTP::GetLastResponse()
{
    return CString(szResponse);
}

BOOL CSMTP::OpenConnection(const CString & server, int port)
{
    BOOL Status;
    // Create a socket
    Status=Create();

    if(Status)
    {
        // connect to the SMTP Service
        Status=Connect((LPCTSTR)server,port);

        if(Status){
            // Check response from the SMTP Service
            Status=ReceiveResponse();
        }
    } else {
		CString e = MBFormatError(GetLastError());
	}
    return Status;
}
class CWSAStartup {
public:
	CWSAStartup() {
		WORD wVersionRequested;
		WSADATA wsaData;
		int err;
 
		wVersionRequested = MAKEWORD( 2, 2 );
 
		err = WSAStartup( wVersionRequested, &wsaData );
		if ( err != 0 ) {
			/* Tell the user that we could not find a usable */
			/* WinSock DLL.                                  */
			return;
		}
	};
	~CWSAStartup() {
		 WSACleanup( );
	};
};
int
CSMTP::mail(const CString & serverHostPort,
		const CString & from,
		const CString & to,
		const CString & headers,
		const CString & body)
{
//	CWSAStartup wsastartup;
	if (!AfxSocketInit())
	{
		MBMessageBox("Info", "Unable to send info to Muzikbrowzer");
		return FALSE;
	}

	//
	//  HELO
	//
    CString host = String::field(serverHostPort,":",1);
	CString sPort = String::field(serverHostPort,":",2);
	int port = 25;
	if (sPort != "") {
		port = atoi(sPort);
	}

	BOOL status = OpenConnection(host,port);
	if (status == FALSE) return FALSE;

    long numparts = String::delCount(host, ".");
    CString domain;
    if (numparts >= 3) {
		for(long i = numparts-1 ; i <= numparts ; ++i) {
			if (domain.GetLength()) domain += ".";
			domain += String::field(host, ".", i);
		}
    } else {
        domain = host;
    }
    CString msg("HELO ");
    msg += domain;
	status = SendLine(msg); if (status == FALSE) return FALSE;
	
	//
	// Sender
	//
	
	msg = "MAIL FROM: ";
	//msg += WellFormedAddress(from);
	msg += from;
	status = SendLine(msg); if (status == FALSE) return FALSE;
	
	//
	// Recipients
	//
	
	int n = String::delCount(to, ",");
	CString tmpTo;
	for (long i = 1 ; i <= n ; ++i) {
		msg = "RCPT TO: ";
		tmpTo = String::field(to, ",", i);
//		msg += WellFormedAddress(tmpTo);
		msg += tmpTo;
		status = SendLine(msg); if (status == FALSE) return FALSE;
	}
	
	//
	// headers and body
	//
	
	msg = "DATA";
	status = SendLine(msg); if (status == FALSE) return FALSE;
	
	if (headers.GetLength()) {
		msg = headers;
		msg += "\r\n\r\n";
	}
	
	
	if (body.GetLength()) {
		msg += body;
		//String::replace(msg, "\n.", "\n..");
	}
	
	msg += "\r\n.\r\n";
	status = SendLine(msg); if (status == FALSE) return FALSE;

	logger.log(m_log);
	
	return TRUE;	
}

CString 
CSMTP::WellFormedAddress(const CString & address)
{
    CString result;

	if (address.GetAt(0) != '<') {
		result = "<";
		result += address;
		result += ">";
	} else {
		result = address;
	}

    return result;
}
