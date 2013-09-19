// usb-uirt interface

#ifndef _h_usbuirt_h_
#define _h_usbuirt_h_


#include "RemoteReceiver.h"
#include "uuirtdrv.h"

class UsbUirt : public RemoteReceiver {
    public:
		UsbUirt();
        ~UsbUirt( );

        BOOL init(CWnd * wnd, UINT msg, const TCHAR * regKey=0, int numKeys=0);
		BOOL Open( );
        void Close( );

		int HandleSerialMsg(WPARAM wparam, LPARAM lparam, int & code);

		int OK() { return m_OKreceived; };

		void WINAPI IRReceiveCallbackx(char *IREventStr, void *userData);
		void WINAPI IRReceiveCallback(char *IREventStr, void *userData);
    
	private:
        // Waggle the control lines to power up or down the UsbUirt

		int m_OKreceived;


		HINSTANCE		m_hinstLib;
		HUUHANDLE		m_hDrvHandle;
		unsigned int	m_drvVersion;
		void unLoadDLL(void);
		BOOL loadDLL(void);

};

#endif // _h_usbuirt_h_
