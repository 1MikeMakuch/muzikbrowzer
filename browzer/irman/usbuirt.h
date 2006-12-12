// usb-uirt interface

#ifndef _h_usbuirt_h_
#define _h_usbuirt_h_


#include "RemoteReceiver.h"

class UsbUirt : public RemoteReceiver {
    public:
        // Initialise from registry (or take defaults)
		UsbUirt();
        // Shutdown, and rewrite configuration to the registry
        ~UsbUirt( );

        BOOL init(CWnd * wnd, const TCHAR * regKey=0, int numKeys=0);
		BOOL Open( );
        void Close( );

		void Port(const CString);

		int HandleSerialMsg(WPARAM wparam, LPARAM lparam, int & code);

		int OK() { return m_OKreceived; };

        // Discard any characters in the COM port buffers
        BOOL Flush( );
    
	private:
        // Waggle the control lines to power up or down the UsbUirt
        BOOL PowerOn( );
        BOOL PowerOff( )	;
		CSerialMFC m_serial;
		int m_OKreceived;
        DCB m_dcb;
        COMMTIMEOUTS m_CommTimeouts;
        BOOL m_bWriteRC;
        BOOL m_bReadRC;
        DWORD m_iBytesWritten;
        DWORD m_iBytesRead;
        char m_sBuffer[ 128 ];
		int m_lastcode;

};

#endif // _h_usbuirt_h_
