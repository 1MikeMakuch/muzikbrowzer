// usb-uirt interface

#ifndef _h_tira_h_
#define _h_tira_h_


#include "RemoteReceiver.h"


class Tira : public RemoteReceiver {
    public:
		Tira();
        ~Tira( );

        BOOL init(CWnd * wnd, UINT msg, const TCHAR * regKey=0, int numKeys=0);
		BOOL Open( );
        void Close( );

		void Port(const CString);

		int HandleSerialMsg(WPARAM wparam, LPARAM lparam, int & code);

		int OK() { return TRUE; };

		void WINAPI IRReceiveCallback (const char *eventstring);
    
	private:

		HINSTANCE		m_hinstLib;
		void unLoadDLL(void);
		BOOL loadDLL(void);

};

#endif // _h_Tira_h_
