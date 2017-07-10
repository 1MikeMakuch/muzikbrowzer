// Tira interface

#include "StdAfx.h"
#include "Tira.h"

#include "Registry.h"
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

static Tira * p2Tira = NULL;


#include <string.h>
using namespace std;
#define STR_W(S) S
#define TIRA_LIBRARY	"tira2.dll"
typedef char SCHAR;
#define STRCPY  strcpy
static SCHAR LibName[200];

//////////////////////////////////////////////////////////////////////////////
int EmptyFunc() {
//	printf("API Library not loaded\n");
	logger.log("Tira Lib not loaded");
	return 1;
};

typedef int (__stdcall * tira_six_byte_cb) (const char * eventstring);

typedef int (__stdcall * t_tira_init) ();
typedef int (__stdcall * t_tira_cleanup) ();
typedef int (__stdcall * t_tira_set_handler) (tira_six_byte_cb cb);
typedef int (__stdcall * t_tira_start) ( int PortID);
typedef int (__stdcall * t_tira_stop) ();
typedef int (__stdcall * t_tira_start_capture) ();
typedef int (__stdcall * t_tira_cancel_capture) ();
typedef int (__stdcall * t_tira_get_captured_data)
                   (const unsigned char** data, int* size );;
typedef int (__stdcall * t_tira_transmit)
       (int repeat, int frequency, const unsigned char* data, const int dataSize );
typedef int (__stdcall * t_tira_delete)(const unsigned char* ptr);

typedef int (__stdcall * t_tira_set_dword)(unsigned int addr, unsigned int val);
typedef const char* (__stdcall * t_tira_get_version)(int component);

typedef int (__stdcall * t_tira_access_feature)(
                                          unsigned int FeatureID,
                                          bool Write,
                                          unsigned int* Value,
                                          unsigned int Mask);

t_tira_init               p_tira_init = (t_tira_init)EmptyFunc;
t_tira_cleanup            p_tira_cleanup = (t_tira_cleanup)EmptyFunc;;
t_tira_set_handler        p_tira_set_handler = (t_tira_set_handler)EmptyFunc;;
t_tira_start              p_tira_start = (t_tira_start)EmptyFunc;;
t_tira_stop               p_tira_stop = (t_tira_stop)EmptyFunc;;
t_tira_start_capture      p_tira_start_capture = (t_tira_start_capture)EmptyFunc;;
t_tira_cancel_capture     p_tira_cancel_capture = (t_tira_cancel_capture)EmptyFunc;;
t_tira_get_captured_data  p_tira_get_captured_data = (t_tira_get_captured_data)EmptyFunc;;
t_tira_transmit           p_tira_transmit = (t_tira_transmit)EmptyFunc;;
t_tira_delete             p_tira_delete = (t_tira_delete)EmptyFunc;;
t_tira_set_dword          p_tira_set_dword = (t_tira_set_dword) EmptyFunc;
t_tira_get_version        p_tira_get_version = (t_tira_get_version) EmptyFunc;
t_tira_access_feature     p_tira_access_feature = (t_tira_access_feature) EmptyFunc;

#define FUNC_VAR_ERASE(FF) \
   p_##FF = (t_##FF) EmptyFunc;
/*****************************************************************************/
/* unLoadDLL: Disconnects from .DLL and unloads it from memory				 */
/*																			 */
/* returns: none															 */
/*																			 */
/*****************************************************************************/
void 
Tira::unLoadDLL(void)
{

	BOOL r;
	if (m_hinstLib) {
		p_tira_cleanup();
		r = FreeLibrary(m_hinstLib);
		if (r) {
			logger.logd("Tira::unLoadDLL succeeded");
		} else {
			logger.log("Tira::unLoadDLL failed");
			DWORD err;
			err = GetLastError();
			logger.log(MBFormatError(err));
		}
	}
	m_hinstLib = NULL;

	FUNC_VAR_ERASE(tira_init);
	FUNC_VAR_ERASE(tira_cleanup);
	FUNC_VAR_ERASE(tira_set_handler);
	FUNC_VAR_ERASE(tira_start);
	FUNC_VAR_ERASE(tira_stop);
	FUNC_VAR_ERASE(tira_start_capture);
	FUNC_VAR_ERASE(tira_cancel_capture);
	FUNC_VAR_ERASE(tira_get_captured_data);
	FUNC_VAR_ERASE(tira_set_dword);
	FUNC_VAR_ERASE(tira_transmit);
	FUNC_VAR_ERASE(tira_delete);
	FUNC_VAR_ERASE(tira_get_version);
	FUNC_VAR_ERASE(tira_access_feature);
}

/*****************************************************************************/
/* loadDLL: Establish contact with the UUIRTDRV dll and assign function      */
/*			entry points													 */
/*																			 */
/* returns: TRUE on success, FALSE on failure								 */
/*																			 */
/*****************************************************************************/
BOOL 
Tira::loadDLL(void)
{
    // Get a handle to the DLL module.
	//unLoadDLL();
 
    m_hinstLib = LoadLibrary(TIRA_LIBRARY); 
 
    // If the handle is valid, try to get the function address.
 
    if (m_hinstLib != NULL) 
    { 
		p_tira_init           = (t_tira_init)					GetProcAddress(m_hinstLib, "tira_init");
		p_tira_cleanup        = (t_tira_cleanup)				GetProcAddress(m_hinstLib, "tira_cleanup");
		p_tira_set_handler    = (t_tira_set_handler)			GetProcAddress(m_hinstLib, "tira_set_handler");
		p_tira_start          = (t_tira_start)					GetProcAddress(m_hinstLib, "tira_start");
		p_tira_stop           = (t_tira_stop)					GetProcAddress(m_hinstLib, "tira_stop");
		p_tira_transmit       = (t_tira_transmit)				GetProcAddress(m_hinstLib, "tira_transmit");
		p_tira_delete         = (t_tira_delete)					GetProcAddress(m_hinstLib, "tira_delete");
		p_tira_get_version    = (t_tira_get_version)			GetProcAddress(m_hinstLib, "tira_get_version");
		p_tira_access_feature = (t_tira_access_feature)			GetProcAddress(m_hinstLib, "tira_access_feature");
		#ifdef p_tira_USE_CAPTURE
		p_tira_start_capture     = (t_tira_start_capture)		GetProcAddress(m_hinstLib, "tira_start_capture");
		p_tira_cancel_capture    = (t_tira_cancel_capture)		GetProcAddress(m_hinstLib, "tira_cancel_capture");
		p_tira_get_captured_data = (t_tira_get_captured_data)	GetProcAddress(m_hinstLib, "tira_get_captured_data");
		p_tira_set_dword         = (t_tira_set_dword)			GetProcAddress(m_hinstLib, "tira_set_dword");
		#endif

		if (!p_tira_init ||
				!p_tira_cleanup ||
				!p_tira_set_handler ||
				!p_tira_start ||
				!p_tira_stop ||
				!p_tira_transmit ||
				!p_tira_delete ||
				!p_tira_get_version ||
				!p_tira_access_feature
		#ifdef p_tira_USE_CAPTURE
				||
				!p_tira_start_capture ||
				!p_tira_cancel_capture ||
				!p_tira_get_captured_data ||
				!p_tira_set_dword
		#endif
			)
		{
			unLoadDLL();
			return FALSE;
		}


		return TRUE;
	}
	return FALSE;
}

/*****************************************************************************/
/* IRReceiveCallback: Receive IR Callback Procedure						     */
/*																			 */
/* This procedure is called by the UUIRT .dll whenever an IRcode is received */
/* The IRcode is passed to the callback in UIR format.						 */
/*																			 */
/*****************************************************************************/
//void WINAPI IRReceiveCallback (char *IREventStr, void *userData) {
//	p2Tira->IRReceiveCallback(IREventStr, userData);
//}
//int __stdcall OurCalback(const char * eventstring) {
//  printf("IR Data %s\n", eventstring);
//   return 0;
//};

int __stdcall IRReceiveCallback (const char * eventstring) {
	p2Tira->IRReceiveCallback(eventstring);
   return 0;
};
//////////////////////////////////////////////////////////////////////////////
// End Tira sample code
//////////////////////////////////////////////////////////////////////////////


Tira::Tira():m_hinstLib(NULL)
{
	p2Tira = this;
	m_codelength = 12;

}
Tira::~Tira( ) {
    Close( );
}
BOOL
Tira::init(CWnd * wmdMsgHndlr, UINT wmsg, const TCHAR * /*regKey*/, int /*numKeys*/) {
	m_portSet = TRUE;
	m_regKey = RegKeyTira;
	logger.logd("Tira::init");

	return RemoteReceiver::init(wmdMsgHndlr,wmsg, m_regKey, IR_MESSAGE_NUMBER_OF);
}
void
Tira::Port( const CString comPort ) {
    Close( );
	RemoteReceiver::Port(comPort);
}

BOOL Tira::Open( ) {
	CWaitCursor c;
	logger.logd("Tira::Open");
	
	if (ready())
		return TRUE;

	CString sComPort (m_sComPort);
	sComPort = String::replace(sComPort,"","COM");
	int comPort = atoi(sComPort);

	if (comPort < 1) {
		MBMessageBox("Error", "Invalid COM port "+m_sComPort);
		return FALSE;
	}



	if (!loadDLL()) {
		MBMessageBox("Error", "Unable to load Tira drivers.",TRUE,FALSE);
		Close();
		return FALSE;
	} else 
		logger.logd("Tira::Open dll loaded");

	int r = p_tira_init();

	r = p_tira_start(comPort);
	if (1 == r) {
		logger.logd("Tira::Open succeeded");
	} else {
		logger.log("Tira::Open tira_start() failed:"+NTS(r));
//		return FALSE;
	}
	
	ready(TRUE);

	p_tira_set_handler(&::IRReceiveCallback);

	return RemoteReceiver::Open();
}
void
Tira::Close( ) {
	unLoadDLL();
	ready(FALSE);
}
void WINAPI
Tira::IRReceiveCallback(const char *IREventStr ) {

	logger.logd("Tira IRReceive:"+CS(IREventStr ));

	int returnCode = -1;
	DWORD tick = 0;
	int code = -1;

	KeyCode key;
//	logger.logd("xyzzy:" + numToString(m_lasttick) + " " + numToString(m_interKeyDelay));

	if (IREventStr && strlen(IREventStr) && strlen(IREventStr) <= m_codelength) {
		//memcpy(key.m_code,IREventStr,m_codelength);
		strcpy((char*)key.m_code,IREventStr);
		code = lookup(key);
		tick = GetTickCount();
		if ((-1 != code && -1 != m_lastcode 
			&& m_lastcode != code)
			|| (tick - m_lasttick) > m_interKeyDelay) {
			returnCode = code;
			m_lastcode = code;
			m_lasttick = tick;
		}

		if (returnCode != -1) {
			m_wndMsgHndlr->PostMessage(m_wmsg, returnCode, LPARAM(0));
		}
//		logger.logd("delay " + numToString(m_interKeyDelay));
	}
}

int
Tira::HandleSerialMsg(WPARAM wParam, LPARAM lParam, int & returnCode) {

	returnCode = wParam;
	return 0;
}
