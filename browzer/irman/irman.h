/******************************************************************************

 IR unit interface

 This is a thin layer between the serial port and the knowledge of IR key
 presses. It performs no buffering other than that provided by the underlying
 COM port, and merely queues up sequences of 6 characters before translating
 them to an index into a vector.

 It reads the key code sequences from the registry, and takes a list of
 key names from the calling program. If no key mapping has been defined
 (or even it it has), the SetKey() method can be used to create this
 mapping.

                     Copyright 1999 Gavin Smyth
                http://www.beesknees.freeserve.co.uk

******************************************************************************/

#ifndef _h_irman_h_
#define _h_irman_h_

#include "SerialMFC.h"
#include "IRCodes.h"

#define IRMAN_DESC_LENGTH 50
#define IRMAN_CODE_LENGTH 6
#define IRMAN_DELAY_DEFAULT 2

typedef int (*IrmanCallback)(UINT, LONG);

class CIQ;

class Irman {
    public:
        // Initialise from registry (or take defaults)
		Irman();
        BOOL init( const TCHAR * regKey, 	// Registry key under which to find the values
               int numKeys,				// Number of keys on the Remote
				CWnd * wnd);

        // Shutdown, and rewrite configuration to the registry
        ~Irman( );

        // Get and set the port name (COMx)
        const TCHAR *Port( ) const {
            return m_sComPort;
        }
        void Port( const TCHAR * comPort );
		BOOL portSet() { return m_portSet; }

        // Get and set the inter-key delay (milliseconds)
        unsigned long Delay( ) const {
            return interKeyDelay;
        }
        void Delay( unsigned long d ) {
            interKeyDelay = d;
        }
        // Wait for a data packet to be received and return the index into// the vector that represents it (or -1 if not recognised)
        int Key( );

        // Trigger the next received key sequence to be stored for the
        // indicated key code
        void SetKey( int key );

        // Interrupt the current read - used from a separate task
        void Interrupt( );
        void ready( BOOL b ) {
            m_ready = b;
        }
        BOOL ready( ) {
            return m_ready;
        }
		TCHAR * comPort() { return m_sComPort; }
		CString code(int i);
		int numOfKeys() { return numKeys; }
		void setKeyDesc(int key, const char*p) {
			if (strlen(p))
				strcpy(keyCodes[key].desc, p);
		}
		void getKeyDesc(int key, char *p) {
			if (strlen(keyCodes[key].desc))
				strcpy(p, keyCodes[key].desc);
		}
		CString getKeyDesc(int key) {
			CString s;
			if (strlen(keyCodes[key].desc)) {
				s = keyCodes[key].desc;
			}
			return s;
		}

		void setRemoteDesc(int key, const char*p) {
			if (strlen(p))
				strcpy(keyCodes[key].remotedesc, p);
		}
		void getRemoteDesc(int key, char *p) {
			if (strlen(keyCodes[key].remotedesc))
				strcpy(p, keyCodes[key].remotedesc);
		}
		CString getRemoteDesc(int key) {
			CString s;
			if (strlen(keyCodes[key].remotedesc)) {
				s = keyCodes[key].remotedesc;
			}
			return s;
		}

		int OK() { return m_OKreceived; };
		int getDescs(int key, CString & desc, CString & remote);

    private:
        // Each Irman key is decoded to a sequence of 6 bytes
        struct KeyCode {
            unsigned char code[ 6 ];
			char desc[IRMAN_DESC_LENGTH];
			char remotedesc[IRMAN_DESC_LENGTH];
            bool operator == ( const KeyCode & key ) {
                for ( int i = 0; i < sizeof( code ); ++i )
                    if ( key.code[ i ] != code[ i ] )
                        return false;
                return true;
            }

        };

		CIQ * mCIQ;

        // Where to read/write values - passed to constructor
        const TCHAR *regKey;

        // How many keys on the remote - passed to constructor
        int numKeys;

        // Codes corresponding to each key to be recognised (numKeys long)
        KeyCode *keyCodes;

		BOOL m_portSet;
		CSerialMFC m_serial;
		CWnd * m_wndMsgHndlr;
		int m_OKreceived;


        // Open and close the COM port
    public: bool Open( );
        void Close( );
		int HandleSerialMsg(WPARAM wparam, LPARAM lparam, int & code);
		void SaveKeys();
    private:
        // Read a complete Irman sequence
        // bool Read( KeyCode & );

        // Time (ms) last Irman sequence read
        unsigned long keyTime;
		unsigned long lastKeyTime;

        // Key code to which to set next read key
        volatile int setKey;

        // Low level (blocking, but interruptable via Interrupt() above)
        // COM port read and write
        //bool ReadWait( void *data, unsigned long size );
        //bool WriteWait( const void *data, unsigned long size );

        // Waggle the control lines to power up or down the Irman
        BOOL PowerOn( );
        BOOL PowerOff( )	;

        // Discard any characters in the COM port buffers
        BOOL Flush( );
		int lookup(KeyCode);

        // Time to wait from reading one key ro the next
        volatile unsigned long interKeyDelay;

        // Disable copying
        Irman( const Irman & );
        Irman & operator = ( const Irman & );
        BOOL m_ready;
        BOOL m_bPortReady;
        //HANDLE m_hCom;
        TCHAR m_sComPort[ 5 ];
        DCB m_dcb;
        COMMTIMEOUTS m_CommTimeouts;
        BOOL bWriteRC;
        BOOL bReadRC;
        DWORD iBytesWritten;
        DWORD iBytesRead;
        char sBuffer[ 128 ];
		IrmanCallback m_irmanCallback;
};

#ifdef THE_IRMAN
Irman * theIrman = 0;
#else
extern Irman * theIrman;
#endif

Irman & irman();


#endif // _h_irman_h_
