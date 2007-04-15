/******************************************************************************

 IR unit interface base class

******************************************************************************/

#ifndef __RemoteReceiver_hh_
#define __RemoteReceiver_hh_

#include "IRCodes.h"
#include "MyString.h"
#include "MyLog.h"

#define IRMAN_DESC_LENGTH 50
//#define IRMAN_CODE_LENGTH 6
#define IRMAN_DELAY_DEFAULT 2
#define IRMAN_CODE_MAXLENGTH 50
class RemoteReceiver {
public:
	typedef enum {
		MB_IR_NONE = 0,
		MB_IR_IRMAN = 1,
		MB_IR_USBUIRT = 2,
		MB_IR_TIRA = 3,
	} m_MBIrTypeT;
	static RemoteReceiver::m_MBIrTypeT m_MBIrType;
	static RemoteReceiver * m_RemoteReceiver;

	static RemoteReceiver * rrcvr();
	static RemoteReceiver * reset(CWnd * wnd, UINT msg);
	static void destroy();
	static void SetType(unsigned long);
	static int GetType();
	static CString GetTypeString();


	RemoteReceiver();
	virtual ~RemoteReceiver();

	virtual BOOL	init(CWnd * wnd, UINT msg, const TCHAR * regKey=0, int numKeys=0);


	virtual BOOL	Open(){ return TRUE;};
	virtual void	Close(){};


	virtual int		HandleSerialMsg(WPARAM wparam, LPARAM lparam, int & code){ return 0;}
	virtual void	Interrupt( ){};
	
	virtual CString comPort() { return m_sComPort; }
	virtual BOOL	portSet() { return m_portSet; }
	virtual CString Port( ) { return m_sComPort;}
	virtual void	Port( const CString comPort );

	unsigned long	Delay( ) const {return m_interKeyDelay;}
	void			Delay( unsigned long d ) {m_interKeyDelay = d;}
	int				numOfKeys(){ return m_numKeys;};
	int				getDescs(int key, CString & desc, CString & remote);
	void			SetKey( int key );
	CString			code(int i);
	void			ready( BOOL b ) {m_ready = b;}
	BOOL			ready( ) {return m_ready;}
	void			SaveKeys();

	void setKeyDesc(int key, const char*p) {
		if (strlen(p))
			strcpy(m_keyCodes[key].m_desc, p);
	}
	void getKeyDesc(int key, char *p) {
		if (strlen(m_keyCodes[key].m_desc))
			strcpy(p, m_keyCodes[key].m_desc);
	}
	CString getKeyDesc(int key) {
		CString s;
		if (0 <= key && key <= m_numKeys && strlen(m_keyCodes[key].m_desc)) {
			s = m_keyCodes[key].m_desc;
		}
		return s;
	}

	void setRemoteDesc(int key, const char*p) {
		if (strlen(p))
			strcpy(m_keyCodes[key].m_remotedesc, p);
	}
	void getRemoteDesc(int key, char *p) {
		if (strlen(m_keyCodes[key].m_remotedesc))
			strcpy(p, m_keyCodes[key].m_remotedesc);
	}
	CString getRemoteDesc(int key) {
		CString s;
		if (strlen(m_keyCodes[key].m_remotedesc)) {
			s = m_keyCodes[key].m_remotedesc;
		}
		return s;
	}

protected:
    // Each Irman key is decoded to a sequence of 6 bytes
    struct KeyCode {
        unsigned char m_code[ IRMAN_CODE_MAXLENGTH ];
		char m_desc[IRMAN_DESC_LENGTH];
		char m_remotedesc[IRMAN_DESC_LENGTH];
        BOOL operator == ( const KeyCode & key ) {
            for ( int i = 0; i < sizeof( m_code ); ++i )
                if ( key.m_code[ i ] != m_code[ i ] )
                    return false;
            return true;
        }
		BOOL matches(const KeyCode & key, int clen) {
            for ( int i = 0; i < clen; ++i )
                if ( key.m_code[ i ] != m_code[ i ] )
                    return false;
            return true;
        }

    };
	int lookup(KeyCode &);
    // Disable copying
    RemoteReceiver( const RemoteReceiver & );
    RemoteReceiver & operator = ( const RemoteReceiver & );

	CString m_sComPort;
	int m_iComPort;
	CWnd * m_wndMsgHndlr;
    // Key code to which to set next read key
    volatile int m_setKey;
    // Where to read/write values - passed to constructor
    CString m_regKey;
    // How many keys on the remote - passed to constructor
    int m_numKeys;
    // Codes corresponding to each key to be recognised (numKeys long)
    KeyCode *m_keyCodes;
    // Time to wait from reading one key ro the next
    volatile unsigned long m_interKeyDelay;
	DWORD m_lasttick;
	BOOL m_portSet;
	BOOL m_bPortReady;
	BOOL m_ready;
	UINT m_wmsg;
	int m_lastcode;
	UINT m_codelength;
};

#endif // __RemoteReceiver_hh_
