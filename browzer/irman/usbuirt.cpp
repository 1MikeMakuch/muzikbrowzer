// usb-uirt interface

#include "StdAfx.h"
#include "irman_common.h"
#define THE_IRMAN
#include "irman.h"
#include "Registry.h"
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "MyLog.h"
#include "MBMessageBox.h"
#include "MyString.h"
#include "Misc.h"
#include "TestHarness/TestHarness.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

class USBUIRT : public RemoteReceiver
{
public:
	USBUIRT() {
		OutputDebugString("USBUIRT::USBUIRT\r\n");
	}
	~USBUIRT()
	{
		OutputDebugString("USBUIRT::~USBUIRT\r\n");
	}
	BOOL Open() {return FALSE; }
	void Close() { ; }
	BOOL init(CWnd * wnd, const TCHAR * regKey=0, int numKeys=0) {
		OutputDebugString("USBUIRT::init\r\n");
		//RemoteReceiver::init(cwnd);
		return TRUE;
	}
};





UsbUirt::UsbUirt()
{}
UsbUirt::~UsbUirt( ) {

    // Close everything down, and write back to the registry
    Close( );
    delete[] m_keyCodes;

}
BOOL
UsbUirt::init(CWnd * wmdMsgHndlr, const TCHAR * /*regKey*/, int /*numKeys*/) {
	m_bPortReady = FALSE ;
	m_portSet = FALSE;
	m_ready = FALSE;
	m_sComPort = "";
	m_regKey = RegKeyUsbUirt;

	return RemoteReceiver::init(wmdMsgHndlr, m_regKey, IR_MESSAGE_NUMBER_OF);
}
BOOL UsbUirt::Open( ) {
	if (ready())
		return TRUE;

    if (!m_sComPort.GetLength()) {
        return false;
    }
    const TCHAR * error = _T( "Error opening device" );

	CSerialMFC::EPort e = CSerialMFC::CheckPort(m_sComPort);
	if (e != CSerialMFC::EPortAvailable) {
		CString msg;
		msg << m_sComPort << " " << m_serial.EPortError(e);
		MBMessageBox("serial i/o error", msg);
		return false;
	}
	
	LONG o = m_serial.Open(m_sComPort, m_wndMsgHndlr, MB_SERIAL_MESSAGE );
	if (o != ERROR_SUCCESS) {
		CString msg;
		msg << "unable to open " << m_sComPort << " error ["
			<< o << "] " << MBFormatError(o);
		MBMessageBox("serial i/o error", msg);
		return false;
	}
	
	o = m_serial.Setup(CSerial::EBaud9600,CSerial::EData8,
		CSerial::EParNone,CSerial::EStop1);
	if (o != ERROR_SUCCESS) {
		CString msg;
		msg << "unable to Setup " << m_sComPort << " error ["
			<< o << "] " << MBFormatError(o);
		MBMessageBox("serial i/o error", msg);
		return false;
	}

	o = m_serial.SetupHandshaking(CSerial::EHandshakeOff);
	if (o != ERROR_SUCCESS) {
		CString msg;
		msg << "unable to Setup HandshakeOff " << m_sComPort << " error ["
			<< o << "] " << MBFormatError(o);
		MBMessageBox("serial i/o error", msg);
		return false;
	}
	o = m_serial.SetupReadTimeouts(CSerial::EReadTimeoutNonblocking);
	if (o != ERROR_SUCCESS) {
		CString msg;
		msg << "unable to Setup TimeoutNonblocking" << m_sComPort << " error ["
			<< o << "] " << MBFormatError(o);
		MBMessageBox("serial i/o error", msg);
		return false;
	}
	
	if (PowerOff( ) == FALSE)
		return FALSE;
	Sleep( 200 );
	if (PowerOn( ) == FALSE)
		return FALSE;

	Sleep( 100 );	// Time for the output to settle
	Flush( );		// Remove power up garbage
	
	m_serial.Write("I"); // These strings must be ASCII, not Unicode
	Sleep( 2 );			 // Need to have >500us between the 'I' & the 'R'
	m_serial.Write("R");
	m_OKreceived = 0;

	// Need to check that we receive "OK" back from UsbUirt

	return RemoteReceiver::Open();
}
void
UsbUirt::Close( ) {
	m_serial.Close();
	ready(FALSE);
}
int
UsbUirt::HandleSerialMsg(WPARAM wParam, LPARAM lParam, int & returnCode) {

	BOOL more = FALSE;
	returnCode = -1;
	CString msg;

	DWORD tick = 0;
	int code = -1;
	char buf[100];

	DWORD bytesRead;
	KeyCode key;

	bytesRead = 0;
	m_serial.Read(key.m_code, sizeof(key.m_code), &bytesRead);
	if (bytesRead) {
		unsigned int i; unsigned char ch;
		CString received;
		for (i = 0 ; i < bytesRead; ++i) {
			ch = key.m_code[i];
			sprintf(buf, "%02x ", ch);
			received += buf;
		}
		if (bytesRead == IRMAN_CODE_LENGTH) {
			code = lookup(key);
		} else if (m_OKreceived == 0 && bytesRead == 2
				&& key.m_code[0] == 'O' && key.m_code[1] == 'K') {
			m_OKreceived = 1;
			m_ready = TRUE;
		}
	}

	tick = GetTickCount();
	if ((-1 != code && -1 != m_lastcode 
		&& m_lastcode != code)
		|| (tick - m_lasttick) > m_interKeyDelay) {
		returnCode = code;
		m_lastcode = code;
		m_lasttick = tick;
	}
	return 0;
}

void
UsbUirt::Port( const CString comPort ) {

    // Reopen the port if the name changed - I could have checked the new
    // and old names and if they were the same, skip the reopen. However,
    // this way, I can force a recover from a "stuck" I/O port...
    Close( );
	RemoteReceiver::Port(comPort);

}

// Power the UsbUirt device by twiddling the control lines
BOOL
UsbUirt::PowerOff( ) {
	BOOL o = m_serial.EscapeCommFunction(CLRDTR);
	if (o == FALSE) {
		DWORD e = GetLastError();
		CString msg;
		msg << "unable to CLRDTR UsbUirt " << m_sComPort << " error ["
			<< e << "] " << MBFormatError(e);
		MBMessageBox("serial i/o error", msg);
		return FALSE;
	}
	o = m_serial.EscapeCommFunction(CLRRTS);
	if (o == FALSE) {
		DWORD e = GetLastError();
		CString msg;
		msg << "unable to CLRRTS UsbUirt " << m_sComPort << " error ["
			<< e << "] " << MBFormatError(e);
		MBMessageBox("serial i/o error", msg);
		return FALSE;
	}
	return TRUE;
}

BOOL
UsbUirt::PowerOn( ) {

	BOOL o = m_serial.EscapeCommFunction(SETDTR);
	if (o == FALSE) {
		DWORD e = GetLastError();
		CString msg;
		msg << "unable to SETDTR UsbUirt " << m_sComPort << " error ["
			<< e << "] " << MBFormatError(e);
		MBMessageBox("serial i/o error", msg);
		return FALSE;
	}
	o = m_serial.EscapeCommFunction(SETRTS);
	if (o == FALSE) {
		DWORD e = GetLastError();
		CString msg;
		msg << "unable to SETRTS UsbUirt " << m_sComPort << " error ["
			<< e << "] " << MBFormatError(e);
		MBMessageBox("serial i/o error", msg);
		return FALSE;
	}
	return TRUE;
}

// Throw away anything in the COM port buffers, and set the ignore time
// to start from now.
BOOL
UsbUirt::Flush( ) {

	LONG e = m_serial.Purge();
	if (e != ERROR_SUCCESS) {
		CString msg;
		msg << "unable to PurgeComm " << m_sComPort << " error ["
			<< e << "] " << MBFormatError(e);
		MBMessageBox("serial i/o error", msg);
		return false;
	}
//	m_keyTime = GetTickCount( );
	return TRUE;
}




/////////////////////////////////////////////////////////////////////////

