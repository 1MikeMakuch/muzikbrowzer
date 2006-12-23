// SOCKETX.CPP -- Extension of the CSocket class
//

#include "stdafx.h"
#include "MSocket.h"
#include "MyString.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



IMPLEMENT_DYNAMIC(MSocket, CSocket)

int MSocket::Send(LPCTSTR lpszStr, UINT uTimeOut, int nFlags)
{
	// If a timeout value was specified, set it
	if (uTimeOut > 0)
		SetTimeOut(uTimeOut);

	// Call base class function
	int nRet = CSocket::Send(lpszStr, strlen(lpszStr), nFlags);

	// If we previously set a timeout
	if (uTimeOut > 0)
	{
		KillTimeOut();
		// If the operation timedout, set a more
		// natural error message
		if (GetLastError() == WSAEINTR)
			SetLastError(WSAETIMEDOUT);
	}
	return nRet;
}


int MSocket::Receive(CString& str, UINT uTimeOut, int nFlags)
{
	//static char szBuf[5000];
	AutoBuf szBuf(5000);
	memset(szBuf.p, 0, 5000);

	// If a timeout value was specified, set it
	if (uTimeOut > 0)
		SetTimeOut(uTimeOut);

	// Call base class function
	int nRet = CSocket::Receive(szBuf.p, 5000, nFlags);

	// If we previously set a timeout
	if (uTimeOut > 0)
	{
		KillTimeOut();
		// If the operation timedout, set a more
		// natural error message
		if (nRet == SOCKET_ERROR)
		{
			if (GetLastError() == WSAEINTR)
				SetLastError(WSAETIMEDOUT);
		}
	}

	// Fill in the CString reference
	str = szBuf.p;
	return nRet;
}

BOOL MSocket::OnMessagePending() 
{
	MSG msg;

	// Watch for our timer message
	if(::PeekMessage(&msg, NULL, WM_TIMER, WM_TIMER, PM_NOREMOVE))
	{
		// If our timer expired...
		if (msg.wParam == (UINT) m_nTimerID)
		{
			// Remove the message
			::PeekMessage(&msg, NULL, WM_TIMER, WM_TIMER, PM_REMOVE);
			// And cancel the call
			CancelBlockingCall();
			return FALSE;
		}
	}
	// Call base class function
	return CSocket::OnMessagePending();
} 


BOOL MSocket::SetTimeOut(UINT uTimeOut) 
{ 
	m_nTimerID = SetTimer(NULL,0,uTimeOut,NULL);
	return m_nTimerID;
} 


BOOL MSocket::KillTimeOut() 
{ 
	return KillTimer(NULL,m_nTimerID);
} 



