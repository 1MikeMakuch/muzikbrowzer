// usb-uirt interface

#include "StdAfx.h"
#include "UsbUirt.h"

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

static UsbUirt * p2usbuirt = NULL;

//////////////////////////////////////////////////////////////////////////////
// Begin usbuirt sample code
//////////////////////////////////////////////////////////////////////////////
//HINSTANCE		hinstLib = NULL; 

// Driver handle for UUIRT device
//HUUHANDLE		hDrvHandle;

//unsigned int	drvVersion;

// Globals to hold last-learned IR-Code and its format...
char	gIRCode[2048] = "0000 0071 0000 0032 0080 0040 0010 0010 0010 0030 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0030 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0030 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0030 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0030 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0030 0010 0aad";
int		gIRCodeFormat = UUIRTDRV_IRFMT_PRONTO;

// Globals used during Learning...
char	gLearnBuffer[2048];
int		gLearnFormat;

// UUIRT .dll funtion pointers. These will be assigned when calling LoadDLL()
pfn_UUIRTOpen				fnUUIRTOpen;
pfn_UUIRTClose				fnUUIRTClose;
pfn_UUIRTGetDrvInfo			fn_UUIRTGetDrvInfo;
pfn_UUIRTGetUUIRTInfo		fn_UUIRTGetUUIRTInfo;
pfn_UUIRTGetUUIRTConfig		fn_UUIRTGetUUIRTConfig;
pfn_UUIRTSetUUIRTConfig		fn_UUIRTSetUUIRTConfig;
pfn_UUIRTSetReceiveCallback	fn_UUIRTSetReceiveCallback;
pfn_UUIRTTransmitIR			fn_UUIRTTransmitIR;
pfn_UUIRTLearnIR			fn_UUIRTLearnIR;

/*****************************************************************************/
/* unLoadDLL: Disconnects from .DLL and unloads it from memory				 */
/*																			 */
/* returns: none															 */
/*																			 */
/*****************************************************************************/
void 
UsbUirt::unLoadDLL(void)
{

	BOOL r;
	if (m_hinstLib) {
		r = FreeLibrary(m_hinstLib);
		if (r) {
			logger.logd("UsbUirt::unLoadDLL succeeded");
		} else {
			logger.log("UsbUirt::unLoadDLL failed");
			DWORD err;
			err = GetLastError();
			logger.log(MBFormatError(err));
		}
	}
	m_hinstLib = NULL;
}

/*****************************************************************************/
/* loadDLL: Establish contact with the UUIRTDRV dll and assign function      */
/*			entry points													 */
/*																			 */
/* returns: TRUE on success, FALSE on failure								 */
/*																			 */
/*****************************************************************************/
BOOL 
UsbUirt::loadDLL(void)
{
    // Get a handle to the DLL module.
	//unLoadDLL();
 
    m_hinstLib = LoadLibrary("uuirtdrv"); 
 
    // If the handle is valid, try to get the function address.
 
    if (m_hinstLib != NULL) 
    { 
        fnUUIRTOpen = (pfn_UUIRTOpen) GetProcAddress(m_hinstLib, "UUIRTOpen");
        fnUUIRTClose = (pfn_UUIRTClose) GetProcAddress(m_hinstLib, "UUIRTClose");
		fn_UUIRTGetDrvInfo  = (pfn_UUIRTGetDrvInfo) GetProcAddress(m_hinstLib, "UUIRTGetDrvInfo");
		fn_UUIRTGetUUIRTInfo = (pfn_UUIRTGetUUIRTInfo) GetProcAddress(m_hinstLib, "UUIRTGetUUIRTInfo");
		fn_UUIRTGetUUIRTConfig = (pfn_UUIRTGetUUIRTConfig) GetProcAddress(m_hinstLib, "UUIRTGetUUIRTConfig");
		fn_UUIRTSetUUIRTConfig = (pfn_UUIRTSetUUIRTConfig) GetProcAddress(m_hinstLib, "UUIRTSetUUIRTConfig");
		fn_UUIRTSetReceiveCallback = (pfn_UUIRTSetReceiveCallback) GetProcAddress(m_hinstLib, "UUIRTSetReceiveCallback");
		fn_UUIRTTransmitIR = (pfn_UUIRTTransmitIR) GetProcAddress(m_hinstLib, "UUIRTTransmitIR");
		fn_UUIRTLearnIR = (pfn_UUIRTLearnIR) GetProcAddress(m_hinstLib, "UUIRTLearnIR");

		if (!fnUUIRTOpen || 
			!fnUUIRTClose || 
			!fn_UUIRTGetDrvInfo || 
			!fn_UUIRTGetUUIRTInfo || 
			!fn_UUIRTGetUUIRTConfig || 
			!fn_UUIRTSetUUIRTConfig || 
			!fn_UUIRTSetReceiveCallback || 
			!fn_UUIRTTransmitIR || 
			!fn_UUIRTLearnIR)
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
void WINAPI IRReceiveCallback (char *IREventStr, void *userData) {
	p2usbuirt->IRReceiveCallback(IREventStr, userData);
}
//////////////////////////////////////////////////////////////////////////////
// End usbuirt sample code
//////////////////////////////////////////////////////////////////////////////


UsbUirt::UsbUirt():m_hinstLib(NULL), m_hDrvHandle(NULL), m_drvVersion(0)
	,m_OKreceived(FALSE)
{
	p2usbuirt = this;
	m_codelength = 12;

}
UsbUirt::~UsbUirt( ) {
    Close( );
}
BOOL
UsbUirt::init(CWnd * wmdMsgHndlr, UINT wmsg, const TCHAR * /*regKey*/, int /*numKeys*/) {
	m_portSet = TRUE;
	m_regKey = RegKeyUsbUirt;
	logger.logd("UsbUirt::init");

	return RemoteReceiver::init(wmdMsgHndlr,wmsg, m_regKey, IR_MESSAGE_NUMBER_OF);
}
BOOL UsbUirt::Open( ) {
	logger.logd("UsbUirt::Open");
	if (ready())
		return TRUE;

	if (!loadDLL()) {
		MBMessageBox("Error", "Unable to load USB-UIRT drivers.",TRUE,FALSE);
		Close();
		return FALSE;
	} else 
		logger.logd("UsbUirt::Open dll loaded");

	if (!fn_UUIRTGetDrvInfo(&m_drvVersion)) {
		MBMessageBox("Error", "Unable to retrieve version from USB-UIRT driver.",TRUE,FALSE);
		Close();
		return FALSE;
	} else 
		logger.logd("UsbUirt::Open got version");

	if (m_drvVersion != 0x0100) {
		MBMessageBox("Error", "USB-UIRT invalid version",TRUE,FALSE);
		Close();
		return FALSE;
	} else 
		logger.logd("UsbUirt::Open good version");

	m_hDrvHandle = fnUUIRTOpen();
	if (m_hDrvHandle == INVALID_HANDLE_VALUE)
	{
		DWORD err;
		err = GetLastError();
		if (err == UUIRTDRV_ERR_NO_DLL)
		{
			MBMessageBox("Error", "Unable to find USB-UIRT Driver. Please make sure driver is Installed!",TRUE,FALSE);
		}
		else if (err == UUIRTDRV_ERR_NO_DEVICE)
		{
			MBMessageBox("Error", "Unable to connect to USB-UIRT device!  Please ensure device is connected to the computer!",TRUE,FALSE);
		}
		else if (err == UUIRTDRV_ERR_NO_RESP)
		{
			MBMessageBox("Error", "Unable to communicate with USB-UIRT device!  Please check connections and try again.  If you still have problems, try unplugging and reconnecting your USB-UIRT.  If problem persists, contact Technical Support!",TRUE,FALSE);
		}
		else if (err == UUIRTDRV_ERR_VERSION)
		{
			MBMessageBox("Error", "Your USB-UIRT's firmware is not compatible with this API DLL. Please verify you are running the latest API DLL and that you're using the latest version of USB-UIRT firmware!  If problem persists, contact Technical Support!",TRUE,FALSE);
		}
		else
		{
			MBMessageBox("Error", "Unable to initialize USB-UIRT (unknown error)!\n",TRUE,FALSE);
		}

		unLoadDLL();
		return FALSE;
	} else 
		logger.logd("UsbUirt::Open fnUUirtOPen succeeded");
	
	ready(TRUE);


	fn_UUIRTSetReceiveCallback(m_hDrvHandle, &::IRReceiveCallback, (void *)0xA5A5A5A5);		
	m_OKreceived = TRUE;

	return RemoteReceiver::Open();
}
void
UsbUirt::Close( ) {
	if (ready()) {
		fnUUIRTClose(m_hDrvHandle);
	}
	unLoadDLL();
	ready(FALSE);
}

void WINAPI
UsbUirt::IRReceiveCallbackx(char *IREventStr, void *userData) {
//	char buf[1000];
//	sprintf(buf,"UsbUirt Code = %s, UserData = %08x!!!", IREventStr, 
//		(UINT32)userData);
//	logger.logd(buf);

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

void WINAPI
UsbUirt::IRReceiveCallback(char *IREventStr, void *userData) {
	int returnCode = -1;
	DWORD tick = 0;
	int code = -1;
	KeyCode key;
	static int counter = 0;

	if (IREventStr && strlen(IREventStr) && strlen(IREventStr) <= m_codelength) {
		//memcpy(key.m_code,IREventStr,m_codelength);
		strcpy((char*)key.m_code,IREventStr);

		code = lookup(key);
		counter++;
		if (code != m_lastcode) counter = 0;

		if ((-1 != code && -1 != m_lastcode 
			&& m_lastcode != code)
//			|| (tick - m_lasttick) > m_interKeyDelay) {
			|| (0 == m_interKeyDelay)
			|| (counter >= m_interKeyDelay)) {
			counter = 0;
			returnCode = code;
			m_lastcode = code;
			m_lasttick = tick;
		}
		CString msg;
		if (returnCode != -1) {
			m_wndMsgHndlr->PostMessage(m_wmsg, returnCode, LPARAM(0));
			msg += "IR PostMessage ";
		} else {
			msg += "IR suppressed ";
		}
		msg += "ikd: " + numToString(m_interKeyDelay) + " " + key.m_code;
		msg += " ctr:" + NTS(counter) ;
		logger.log(msg);
	}
}


int
UsbUirt::HandleSerialMsg(WPARAM wParam, LPARAM lParam, int & returnCode) {

	returnCode = wParam;
	return 0;
}
