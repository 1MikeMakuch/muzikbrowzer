/******************************************************************************

 IR unit interface

 This is a thin layer between the serial port and the knowledge of IR key
 presses. It performs no buffering other than that provided by the underlying
 COM port, and merely queues up sequences of 6 characters before translating
 them to an index into a vector.

 Note that the COM port is not opened until the first read, so that potential
 hangs occur in the separate thread.

                     Copyright 1999 Gavin Smyth
                http://www.beesknees.freeserve.co.uk

******************************************************************************/
#include "StdAfx.h"
#include "irman_common.h"
#define THE_IRMAN
#include "irman.h"
#include "irman_registry.h"
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

// Registry keys
const static TCHAR RegPort[] = _T( "Port" );
const static TCHAR RegDelay[] = _T( "InterKeyDelay" );

Irman & irman() {
	return (Irman &)*theIrman;
}

typedef struct CodeInfoStruct {
	int code;
	DWORD tick;
} CodeInfo;

class CIQ {
	public:
		CIQ():mQueueSize(0),mQueueEnd(0){};
		CIQ(int size) : mQueueSize(0), mQueueEnd ( size - 1),
			mQueueHead(0),mQueueNext(0)
		{
			mQueue = new CodeInfo [ size ];
		};

	~CIQ() {
		delete [] mQueue;
	};

	int append(int code, DWORD tick) {
		if (mQueueSize == mQueueEnd+1 ) {
			return -1;
		}
		++mQueueSize;
		mQueue[mQueueNext].code = code;
		mQueue[mQueueNext].tick = tick;
		++mQueueNext;
		if (mQueueNext > mQueueEnd)
			mQueueNext = 0;
		if (mQueueNext == mQueueHead)
			return -1;
		return 0;
	}

	int removeHead() { 
		if (mQueueSize == 0) return 0;
		--mQueueSize;
		++mQueueHead;
		if (mQueueHead > mQueueEnd) mQueueHead = 0;
		return mQueueHead;
	}
	int head() { return mQueueHead; }
	int length() { return mQueueSize; }
	int empty() { return (mQueueHead == mQueueNext); }
	int getCode() { return mQueue[mQueueHead].code; }
	DWORD getTick() { return mQueue[mQueueHead].tick; }
private:
	CodeInfo * mQueue;
	int mQueueSize;
	const int mQueueEnd;
	int mQueueHead;
	int mQueueNext;
};

TEST(IRMAN, CIQTest)
{
	CIQ ciq(5);
	CHECK( ciq.length() == 0);
	int i,j;
	for(i = 0 ; i < 5; ++i) {
		ciq.append(i,i);
		CHECK( ciq.length() == i+1);
	}
		for(i = 0 ; i < 5; ++i) {
		ciq.append(i,i);
		CHECK( ciq.length() == 5);
	}
	for (i=5, j=0 ; i > 0 ; --i,++j) {
		CHECK( ciq.length() == i );
		CHECK( ciq.getCode() == j);
		CHECK( ciq.getTick() == j);
		ciq.removeHead();
	}
	CHECK( ciq.length() == 0);

	for(i = 0 ; i < 20 ; ++i) {
		for(j = 0 ; j < 3 ; ++j) {
			ciq.append(1,1);
		}
		CHECK( ciq.length() == 3);
		ciq.removeHead();
		CHECK( ciq.length() == 2);
		ciq.removeHead();
		CHECK( ciq.length() == 1);
		ciq.removeHead();
		CHECK( ciq.length() == 0);
	}
}

Irman::Irman() : keyCodes(0), mCIQ(0) 
{
	theIrman = this;
}

BOOL
Irman::init( const TCHAR * regKey_, int numKeys_, CWnd * wndMsgHndlr) {

	regKey = regKey_ ;
	numKeys = numKeys_ ;
	if (keyCodes)
		delete [] keyCodes;
	keyCodes = new KeyCode[ numKeys_ ];
	if (mCIQ)
		delete mCIQ;
	mCIQ = new CIQ(20);
	keyTime = GetTickCount( );
	setKey = -1 ;
	m_bPortReady = FALSE ;
	m_portSet = FALSE;
	m_ready = FALSE;
	m_wndMsgHndlr = wndMsgHndlr;
	m_sComPort[0] = 0;

    // Read the port name, inter key delay and all the key codes from
    // the registry, and then fire up the device
    RegistryKey reg( HKEY_LOCAL_MACHINE, regKey );
	TCHAR comPort[5];
    reg.Read( RegPort, comPort, sizeof( comPort ) / sizeof( TCHAR ),
              _T( "x" ) );
	if (comPort[0] != 'x') {
		Port(comPort);
	}
    interKeyDelay = reg.Read( RegDelay, IRMAN_DELAY_DEFAULT );
    static KeyCode blankKeyCode;
	memset(&blankKeyCode, 0, sizeof(KeyCode));
    for ( int i = 0; i < numKeys; ++i ) {
        TCHAR num[ 16 ];
		TCHAR desc[IRMAN_DESC_LENGTH];
		TCHAR rdesc[IRMAN_DESC_LENGTH];
        memset(keyCodes[i].code, 0, IRMAN_CODE_LENGTH);
        memset(keyCodes[i].desc, 0, IRMAN_DESC_LENGTH);
		memset(keyCodes[i].remotedesc, 0, IRMAN_DESC_LENGTH);
        wsprintf( num, _T( "%03d" ), i );
		wsprintf( desc, _T( "%03d_desc"), i);
		wsprintf( rdesc, _T( "%03d_remote"), i);
        char code[IRMAN_CODE_LENGTH];
        TCHAR descd[IRMAN_DESC_LENGTH];
		TCHAR rdescd[IRMAN_DESC_LENGTH];
        reg.Read(num, (void*)code, sizeof(code), blankKeyCode.code);
        reg.Read(desc, descd, sizeof(descd), blankKeyCode.desc);
		reg.Read(rdesc, rdescd, sizeof(rdescd), blankKeyCode.remotedesc);
        strcpy((char*)keyCodes[i].code, code);
        strcpy((char*)keyCodes[i].desc, descd);
		strcpy((char*)keyCodes[i].remotedesc, rdescd);
    }
    // Don't open the port yet in case it hangs...
	return TRUE;
}

CString
Irman::code(int i) {
	CString code;
	char buf[13];
	sprintf(buf, "%02X%02X%02X%02X%02X%02X",
		keyCodes[i].code[0], keyCodes[i].code[1], keyCodes[i].code[2],
		keyCodes[i].code[3], keyCodes[i].code[4], keyCodes[i].code[5]);
	buf[12] = 0;
	code = buf;
	return code;
}

Irman::~Irman( ) {

    // Close everything down, and write back to the registry
    Close( );
    delete[] keyCodes;
	delete mCIQ;

}
void
Irman::SaveKeys() {
    RegistryKey reg( HKEY_LOCAL_MACHINE, regKey );
    reg.Write( RegPort, m_sComPort );
    reg.Write( RegDelay, interKeyDelay );
    for ( int i = 0; i < numKeys; ++i ) {
        TCHAR num[ 16 ];
		TCHAR desc[IRMAN_DESC_LENGTH];
		TCHAR rdesc[IRMAN_DESC_LENGTH];
        wsprintf( num, _T( "%03d" ), i );
		wsprintf(desc, _T("%03d_desc"), i);
		wsprintf(rdesc, _T("%03d_remote"), i);
        reg.Write( num, (void*)keyCodes[ i ].code,
						(unsigned long)sizeof( keyCodes[i].code ) );
		reg.Write(desc, keyCodes[i].desc);
		reg.Write(rdesc, keyCodes[i].remotedesc);
    }

}

bool Irman::Open( ) {
	if (ready())
		return TRUE;

    if (m_sComPort == 0 || strlen(m_sComPort) < 1) {
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

	// Need to check that we receive "OK" back from Irman

	return TRUE;
}

void
Irman::Close( ) {
	m_serial.Close();
	ready(FALSE);
}
void
Irman::Port( const TCHAR * comPort ) {
    _tcsnccpy( m_sComPort, comPort, sizeof( m_sComPort ) / sizeof( TCHAR ) );
    m_sComPort[ sizeof( m_sComPort ) / sizeof( TCHAR ) - 1 ] = 0;

    // Reopen the port if the name changed - I could have checked the new
    // and old names and if they were the same, skip the reopen. However,
    // this way, I can force a recover from a "stuck" I/O port...
    Close( );
    if (comPort == NULL || strlen(comPort) == 0) {
        m_portSet = FALSE;
    } else {
	    m_portSet = TRUE;
    }
}

void
Irman::SetKey( int key ) {
    if ( key < 0 || key > numKeys ) {
        return ;
    }
    // Just indicate to the reading function that it should store the next
    // sequence instead of matching it
    setKey = key;
}


void
Irman::Interrupt( ) {

    // On NT or 98, this should use CancelIo, but that function isn't present
    // on 95, so bodge it by signalling the event...
    // CancelIo( m_hCom );
    //  SetEvent( ioCompletion );
}


// Power the Irman device by twiddling the control lines
BOOL
Irman::PowerOff( ) {
	BOOL o = m_serial.EscapeCommFunction(CLRDTR);
	if (o == FALSE) {
		DWORD e = GetLastError();
		CString msg;
		msg << "unable to CLRDTR Irman " << m_sComPort << " error ["
			<< e << "] " << MBFormatError(e);
		MBMessageBox("serial i/o error", msg);
		return FALSE;
	}
	o = m_serial.EscapeCommFunction(CLRRTS);
	if (o == FALSE) {
		DWORD e = GetLastError();
		CString msg;
		msg << "unable to CLRRTS Irman " << m_sComPort << " error ["
			<< e << "] " << MBFormatError(e);
		MBMessageBox("serial i/o error", msg);
		return FALSE;
	}
	return TRUE;
}

BOOL
Irman::PowerOn( ) {

	BOOL o = m_serial.EscapeCommFunction(SETDTR);
	if (o == FALSE) {
		DWORD e = GetLastError();
		CString msg;
		msg << "unable to SETDTR Irman " << m_sComPort << " error ["
			<< e << "] " << MBFormatError(e);
		MBMessageBox("serial i/o error", msg);
		return FALSE;
	}
	o = m_serial.EscapeCommFunction(SETRTS);
	if (o == FALSE) {
		DWORD e = GetLastError();
		CString msg;
		msg << "unable to SETRTS Irman " << m_sComPort << " error ["
			<< e << "] " << MBFormatError(e);
		MBMessageBox("serial i/o error", msg);
		return FALSE;
	}
	return TRUE;
}

// Throw away anything in the COM port buffers, and set the ignore time
// to start from now.
BOOL
Irman::Flush( ) {

	LONG e = m_serial.Purge();
	if (e != ERROR_SUCCESS) {
		CString msg;
		msg << "unable to PurgeComm " << m_sComPort << " error ["
			<< e << "] " << MBFormatError(e);
		MBMessageBox("serial i/o error", msg);
		return false;
	}
	keyTime = GetTickCount( );
	return TRUE;
}

int
Irman::HandleSerialMsg(WPARAM wParam, LPARAM lParam, int & returnCode) {

	BOOL more = FALSE;
	returnCode = -1;
//	CString msg;
	int localcode = -1;

	//		const CSerialMFC::EEvent eEvent = CSerialMFC::EEvent(LOWORD(wParam));
	//		const CSerialMFC::EError eError = CSerialMFC::EError(HIWORD(wParam));
			char buf[100];
	//
	//		switch (eEvent)
	//		{
	//		case CSerialMFC::EEventRecv: {
			DWORD bytesRead;
			KeyCode key;
			do
			{
				// Read data from the COM-port
				bytesRead = 0;
				m_serial.Read(key.code, sizeof(key.code), &bytesRead);
				if (bytesRead) {
					unsigned int i; unsigned char ch;
					CString received;
					for (i = 0 ; i < bytesRead; ++i) {
						ch = key.code[i];
						sprintf(buf, "%02x ", ch);
						received += buf;
					}
//					msg << CS("R:") << bytesRead << CS(" ")
//						<< received << CS(" ");
					if (bytesRead == IRMAN_CODE_LENGTH) {
						localcode = lookup(key);
						if (localcode != -1) {
							CString desc = getKeyDesc(localcode);
							mCIQ->append(localcode,GetTickCount());
//							msg += desc + " queued up";
						} else {
//							msg += "unknown sequence";
						}
					} else if (m_OKreceived == 0 && bytesRead == 2
							&& key.code[0] == 'O' && key.code[1] == 'K') {
						m_OKreceived = 1;
						m_ready = TRUE;
//						msg += "OK";
					}
//					msg += "\r\n";
//					OutputDebugString(msg);
				}
			} while (bytesRead == sizeof(key.code));
			//			break;
			//		}
			//		default:
			//			sprintf(buf,"%d", eEvent);
			//			CString msg = "eEvent: ";
			//			msg += buf;
			//			OutputDebugString(msg);
			//		}

	static DWORD lasttick = 0;
	static int lastcode = 0;
	static int ctr = 0;
	static DWORD tick = 0;
	static int code = 0;
	
	while(mCIQ->length()) {
		lastcode = code; lasttick = tick;
		code = mCIQ->getCode();
		tick = mCIQ->getTick();
		mCIQ->removeHead();
		if (code == lastcode) {
			++ctr;
		} else {
			ctr = 1;
		}
		if (ctr >= interKeyDelay) {
			returnCode = code;
			ctr = 0;
//			msg = "Invoking ";
//			msg += getKeyDesc(code);
//			msg += "\r\n";
//			OutputDebugString(msg);
			break;
		}
	}

	return (mCIQ->length() > 0);
}

int
Irman::lookup(KeyCode key) {

	int theKey = -1;
	CString msg;

    // Loop for a minimum time, to get rid of old duplicate/inverted messages
	//	lastKeyTime = keyTime;
	//    keyTime = GetTickCount();
	//	if (keyTime - lastKeyTime < interKeyDelay) {
	//		msg = "ignoring key within interKeyDelay\r\n";
	//		OutputDebugString(msg);
	//		return theKey;
	//	}

    // Now, key contains a valid code sequence, so do something with it
    if ( setKey != -1 ) {

        // If we're in record mode, just use this sequence for the relevant
        // entry in keyCodes[]
        memcpy(keyCodes[ setKey ].code, key.code, IRMAN_CODE_LENGTH);
        theKey = setKey;
        setKey = -1;
		return theKey;
    } else {

        // If we're not in record mode, scan the list to find a match,
        // and repeat for up to the inter key period before giving up and
        // admitting it's unrecognised - the reason for the loop is to
        // catch any inversions along the way

        for ( int i = 0; i < numKeys; ++i ) {
            if ( key == keyCodes[ i ] ) {
                theKey = i;
				break;
			}
		}
    }

	return theKey;

	if (theKey == -1) {
		CString msg;
		msg << "Unrecognized code received from Irman.";
		MBMessageBox("serial i/o error", msg);
	} else {
		(*m_irmanCallback)(theKey, 0);
	}
}
int Irman::getDescs(int key, CString & desc, CString & remote) {
	desc = ""; remote = "";
	if (key < 0 || key >= IR_MESSAGE_NUMBER_OF)
		return -1;

	desc = keyCodes[key].desc;
	remote = keyCodes[key].remotedesc;
	return 0;
}
/////////////////////////////////////////////////////////////////////////

#ifdef asdf
int
Irman::HandleSerialMsg(WPARAM wParam, LPARAM lParam) {
   
	const CSerialMFC::EEvent eEvent = CSerialMFC::EEvent(LOWORD(wParam));
    const CSerialMFC::EError eError = CSerialMFC::EError(HIWORD(wParam));
	char buf[100];

    switch (eEvent)
    {
    case CSerialMFC::EEventRecv: {
		DWORD bytesRead;
		KeyCode key;
		do
		{
			// Read data from the COM-port
			bytesRead = 0;
			m_serial.Read(key.code, sizeof(key.code), &bytesRead);
			if (bytesRead) {
				unsigned int i; unsigned char ch;
				CString received;
				for (i = 0 ; i < bytesRead; ++i) {
					ch = key.code[i];
					sprintf(buf, "%02x ", ch);
					received += buf;
				}

				sprintf(buf,"%d", bytesRead);
//				CString msg = "R:";
//				msg += buf;
//				msg += " ";
//				msg += received + "\r\n";
//				OutputDebugString(msg);
			}
			if (bytesRead == IRMAN_CODE_LENGTH) {
				return lookup(key);
			} else if (m_OKreceived == 0) {
				if (bytesRead == 2 && key.code[0] == 'O'
						&& key.code[1] == 'K') {
					m_OKreceived = 1;
					m_ready = TRUE;
				}
			}
		} while (bytesRead == sizeof(key.code));
        break; 
	}
	default:
		sprintf(buf,"%d", eEvent);
//		CString msg = "eEvent: ";
//		msg += buf;
//		OutputDebugString(msg);
    }
	return -1;
}



// works except for dups during long processing
int
Irman::HandleSerialMsg(WPARAM wParam, LPARAM lParam, int & code) {

	static int nextFifo = 0;
	static int retrieveFifo = 0;
	static DWORD lastTick = 0;
	static DWORD tick = 0;
	BOOL more = FALSE;
	code = -1;
//	CString msg;

//	if (nextFifo == 0) {
		const CSerialMFC::EEvent eEvent = CSerialMFC::EEvent(LOWORD(wParam));
		const CSerialMFC::EError eError = CSerialMFC::EError(HIWORD(wParam));
		char buf[100];

		//		switch (eEvent)
		//		{
		//		case CSerialMFC::EEventRecv: {
			DWORD bytesRead;
			KeyCode key;
			do
			{
				// Read data from the COM-port
				bytesRead = 0;
				m_serial.Read(key.code, sizeof(key.code), &bytesRead);
				tick = GetTickCount();
				if (bytesRead) {
					unsigned int i; unsigned char ch;
					CString received;
					for (i = 0 ; i < bytesRead; ++i) {
						ch = key.code[i];
						sprintf(buf, "%02x ", ch);
						received += buf;
					}
					sprintf(buf,"%03d bytes, % 15d tick diff ",
						bytesRead, tick-lastTick);
//					msg = CS("R:") + CS(buf) + CS(" ") + received + CS(" ");
					if (bytesRead == IRMAN_CODE_LENGTH) {
						code = lookup(key);
						if (code != -1) {
							CString desc = getKeyDesc(code);
							if (tick - lastTick > interKeyDelay) {
								lastTick = tick;
								m_fifo[nextFifo].code = code;
								m_fifo[nextFifo].tick = tick;
								++nextFifo;
//								msg += desc + " queued up";
							} else {
								code = -1;
//								msg += desc + CS(" ignore / interKeyDelay:");
//								msg += interKeyDelay;
							}
						} else {
//							msg += "unknown sequence";
						}
					} else if (m_OKreceived == 0 && bytesRead == 2
							&& key.code[0] == 'O' && key.code[1] == 'K') {
						m_OKreceived = 1;
						m_ready = TRUE;
//						msg += "OK";
					}
//					msg += "\r\n";
//					OutputDebugString(msg);
				}
			} while (bytesRead == sizeof(key.code));
			//			break;
			//		}
			//		default:
			//			sprintf(buf,"%d", eEvent);
			//			CString msg = "eEvent: ";
			//			msg += buf;
			//			OutputDebugString(msg);
			//		}
//	}

	if (nextFifo) {
		code = m_fifo[retrieveFifo].code;
		++retrieveFifo;
		more = TRUE;
		if (retrieveFifo == nextFifo) {
			retrieveFifo = nextFifo = 0;
			more = FALSE;
		}
	}
	return more;
}
#endif
