/******************************************************************************

 IR unit interface base class

******************************************************************************/
#include "StdAfx.h"

#define THE_REMOTE_RECEIVER
#include "RemoteReceiver.h"
#include "Registry.h"

#include "MyLog.h"
#include "MBMessageBox.h"
#include "MyString.h"
#include "Misc.h"
#include "TestHarness/TestHarness.h"

#include "irman.h"
#include "UsbUirt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Registry keys
const static TCHAR RegDelay[] = _T( "InterKeyDelay" );
const static TCHAR RegPort[] = _T( "Port" );

RemoteReceiver * RemoteReceiver::m_RemoteReceiver = NULL;
RemoteReceiver::m_MBIrTypeT RemoteReceiver::m_MBIrType 
											= RemoteReceiver::MB_IR_NONE;


class Tira : public RemoteReceiver
{
public:
	Tira() {
		logger.logd("Tira::Tira");
	}
	~Tira()
	{
		logger.logd("Tira::~Tira");
	}
	BOOL Open() {return FALSE; }
	void Close() {  }
	BOOL init(CWnd * wnd, UINT msg, const TCHAR * regKey=0, int numKeys=0) {
		logger.logd("Tira::init");
		//RemoteReceiver::init(cwnd);
		return TRUE;
	}
};
RemoteReceiver *
RemoteReceiver::rrcvr() { 
	if (RemoteReceiver::m_RemoteReceiver == NULL) {

		RegistryKey reg( HKEY_LOCAL_MACHINE, RegKey );
		RemoteReceiver::m_MBIrType = 
			(RemoteReceiver::m_MBIrTypeT)RemoteReceiver::GetType();

		if (RemoteReceiver::m_MBIrType == RemoteReceiver::MB_IR_IRMAN) {
			RemoteReceiver::m_RemoteReceiver = new Irman();
		} else if (RemoteReceiver::m_MBIrType == RemoteReceiver::MB_IR_USBUIRT) {
			RemoteReceiver::m_RemoteReceiver = new UsbUirt();
		} else if (RemoteReceiver::m_MBIrType == RemoteReceiver::MB_IR_TIRA) {
			RemoteReceiver::m_RemoteReceiver = new Tira();
		} else { // (m_MBIrType == MB_IR_NONE) {
			RemoteReceiver::m_RemoteReceiver = new RemoteReceiver();
		}
	}
	return RemoteReceiver::m_RemoteReceiver ;
}
void 
RemoteReceiver::destroy() {
	if (RemoteReceiver::m_RemoteReceiver) {
		RemoteReceiver::m_RemoteReceiver->Close();
		delete RemoteReceiver::m_RemoteReceiver;
		RemoteReceiver::m_RemoteReceiver = NULL;
	}
}

void
RemoteReceiver::SetType(unsigned long type) {
	RemoteReceiver::m_MBIrType = (RemoteReceiver::m_MBIrTypeT) type;
	RegistryKey reg( HKEY_LOCAL_MACHINE, RegKey );
	reg.Write("RemoteReceiverType", RemoteReceiver::m_MBIrType);
}
int
RemoteReceiver::GetType() {

	RegistryKey reg( HKEY_LOCAL_MACHINE, RegKey );
	unsigned long t = reg.Read("RemoteReceiverType", 0);
	return t;
}

RemoteReceiver::RemoteReceiver() : m_lasttick(0), m_keyCodes(NULL)
	,m_ready(FALSE), m_portSet(FALSE), m_bPortReady(FALSE), m_numKeys(0)
	,m_setKey(-1),m_wndMsgHndlr(NULL),m_interKeyDelay(0),m_wmsg(0)
	,m_codelength(0),m_lastcode(0)
{
	logger.logd("RemoteReceiver::RemoteReceiver");
}
RemoteReceiver::~RemoteReceiver()
{
	logger.logd("RemoteReceiver::~RemoteReceiver");
	if (m_keyCodes)
		delete [] m_keyCodes;
}
BOOL
RemoteReceiver::init(CWnd * wndMsgHndlr, UINT wmsg, const TCHAR * regKey, int numKeys) {
	logger.logd("RemoteReceiver::init");
	m_bPortReady = FALSE ;
	m_ready = FALSE;
	m_sComPort = "";
	m_wmsg = wmsg;
	m_regKey = regKey ;
	m_numKeys = numKeys ;
	if (m_keyCodes)
		delete [] m_keyCodes;
	m_keyCodes = new KeyCode[ numKeys ];

	m_setKey = -1 ;
	m_wndMsgHndlr = wndMsgHndlr;

    // Read the port name, inter key delay and all the key codes from
    // the registry, and then fire up the device
    RegistryKey reg( HKEY_LOCAL_MACHINE, m_regKey );

	m_sComPort = reg.ReadCString(RegPort, "");
	if (m_sComPort != "") {
		Port(m_sComPort);
	}

    m_interKeyDelay = reg.Read( RegDelay, IRMAN_DELAY_DEFAULT );
    static KeyCode blankKeyCode;
	memset(&blankKeyCode, 0, sizeof(KeyCode));
    for ( int i = 0; i < m_numKeys; ++i ) {
        TCHAR num[ 16 ];
		TCHAR desc[IRMAN_DESC_LENGTH];
		TCHAR rdesc[IRMAN_DESC_LENGTH];
        memset(m_keyCodes[i].m_code, 0, IRMAN_CODE_MAXLENGTH);
        memset(m_keyCodes[i].m_desc, 0, IRMAN_DESC_LENGTH);
		memset(m_keyCodes[i].m_remotedesc, 0, IRMAN_DESC_LENGTH);
        wsprintf( num, _T( "%03d" ), i );
		wsprintf( desc, _T( "%03d_desc"), i);
		wsprintf( rdesc, _T( "%03d_remote"), i);
        char * ccode = new char[m_codelength+1];
        TCHAR descd[IRMAN_DESC_LENGTH];
		TCHAR rdescd[IRMAN_DESC_LENGTH];
        reg.Read(num, (void*)ccode, m_codelength, blankKeyCode.m_code);
        reg.Read(desc, descd, sizeof(descd), blankKeyCode.m_desc);
		reg.Read(rdesc, rdescd, sizeof(rdescd), blankKeyCode.m_remotedesc);
        memcpy((char*)m_keyCodes[i].m_code, ccode,m_codelength);
        strcpy((char*)m_keyCodes[i].m_desc, descd);
		strcpy((char*)m_keyCodes[i].m_remotedesc, rdescd);
		m_keyCodes[i].m_code[m_codelength] = 0;
		delete ccode;
		//logger.logd(CS("init: ")+num+CS(":")+code(i));
    }

	return TRUE;
}
void
RemoteReceiver::Port(const CString port) {
	m_sComPort = port;
    if (!m_sComPort.GetLength()) {
        m_portSet = FALSE;
    } else {
	    m_portSet = TRUE;
    }
	RegistryKey reg( HKEY_LOCAL_MACHINE, m_regKey );
    reg.Write( RegPort, m_sComPort );
}
int
RemoteReceiver::getDescs(int key, CString & desc, CString & remote) {
	desc = ""; remote = "";
	if (key < 0 || key >= IR_MESSAGE_NUMBER_OF)
		return -1;

	if (m_keyCodes && m_keyCodes[key].m_desc)
		desc = m_keyCodes[key].m_desc;
	if (m_keyCodes && m_keyCodes[key].m_remotedesc)
		remote = m_keyCodes[key].m_remotedesc;
	return 0;
}
CString
RemoteReceiver::code(int i) {
	CString code;
	char buf[10];
//	sprintf(buf, "%02X%02X%02X%02X%02X%02X",
//		m_keyCodes[i].m_code[0], m_keyCodes[i].m_code[1], m_keyCodes[i].m_code[2],
//		m_keyCodes[i].m_code[3], m_keyCodes[i].m_code[4], m_keyCodes[i].m_code[5]);

	int j;
	for(j=0;j<m_codelength;++j) {
		sprintf(buf, "%02X", m_keyCodes[i].m_code[j]);
		buf[2]=0;
		code += buf;
	}
	//buf[m_codelength] = 0;
	//code = buf;
	return code;
}
void
RemoteReceiver::SetKey( int key ) {
    if ( key < 0 || key > m_numKeys ) {
        return ;
    }
    // Just indicate to the reading function that it should store the next
    // sequence instead of matching it
    m_setKey = key;
}

int
RemoteReceiver::lookup(KeyCode & key) {

	int theKey = -1;
	CString msg;

    // Now, key contains a valid code sequence, so do something with it
    if ( m_setKey != -1 ) {

        // If we're in record mode, just use this sequence for the relevant
        // entry in keyCodes[]
        memcpy(m_keyCodes[ m_setKey ].m_code, key.m_code, m_codelength);
        theKey = m_setKey;
        m_setKey = -1;
		return theKey;
    } else {
        // If we're not in record mode, scan the list to find a match,
        for ( int i = 0; i < m_numKeys; ++i ) {
            //if ( key == m_keyCodes[ i ] ) {
			if (key.matches(m_keyCodes[i],m_codelength)) {
                theKey = i;
				break;
			}
		}
    }

	return theKey;
}
void
RemoteReceiver::SaveKeys() {
    RegistryKey reg( HKEY_LOCAL_MACHINE, m_regKey );

    reg.Write( RegDelay, m_interKeyDelay );
    for ( int i = 0; i < m_numKeys; ++i ) {
        TCHAR num[ 16 ];
		TCHAR desc[IRMAN_DESC_LENGTH];
		TCHAR rdesc[IRMAN_DESC_LENGTH];
        wsprintf( num, _T( "%03d" ), i );
		wsprintf(desc, _T("%03d_desc"), i);
		wsprintf(rdesc, _T("%03d_remote"), i);
        reg.Write( num, (void*)m_keyCodes[ i ].m_code,
						(unsigned long)m_codelength );
		reg.Write(desc, m_keyCodes[i].m_desc);
		reg.Write(rdesc, m_keyCodes[i].m_remotedesc);
		reg.Write( RegPort, m_sComPort );
		//logger.logd(CS("Save: ")+ num + ":"+code(i));
    }

}

TEST(RemoteReceivertest, rrtest)
{
	return;
	CWnd * c = new CWnd();
	int t = RemoteReceiver::GetType();
	RemoteReceiver::SetType(RemoteReceiver::MB_IR_USBUIRT);
	RemoteReceiver * rrcvr = RemoteReceiver::reset(c, MB_SERIAL_MESSAGE);
	RemoteReceiver::SetType(RemoteReceiver::MB_IR_TIRA);
	rrcvr = RemoteReceiver::reset(c, MB_SERIAL_MESSAGE);
	RemoteReceiver::SetType(RemoteReceiver::MB_IR_NONE);
	rrcvr = RemoteReceiver::reset(c, MB_SERIAL_MESSAGE);
	RemoteReceiver::SetType(RemoteReceiver::MB_IR_IRMAN);
	rrcvr = RemoteReceiver::reset(c, MB_SERIAL_MESSAGE);
	RemoteReceiver::SetType(t);
	RemoteReceiver::m_RemoteReceiver->destroy();

	delete c;
}
RemoteReceiver *
RemoteReceiver::reset(CWnd * cwnd, UINT msg) {
	if (RemoteReceiver::m_RemoteReceiver) {
		RemoteReceiver::m_RemoteReceiver->destroy();
	}
	RemoteReceiver::m_RemoteReceiver = RemoteReceiver::rrcvr();
	RemoteReceiver::m_RemoteReceiver->init(cwnd,MB_SERIAL_MESSAGE);
	if (RemoteReceiver::m_RemoteReceiver->Open()) {
		//logger.logd("RemoteReceiver: reset succeeded");
	} else {
		//logger.log("RemoteReceiver: reset failed");
		//RemoteReceiver::m_RemoteReceiver->destroy();
		return NULL;
	}
	return RemoteReceiver::m_RemoteReceiver;
}


