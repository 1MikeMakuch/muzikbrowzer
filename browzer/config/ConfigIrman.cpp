//// ConfigIrman.cpp : implementation file
//

#include "stdafx.h"

#include "RemoteReceiver.h"
#include "IRCodes.h"
#include "ConfigIrman.h"
#include "Registry.h"
#include "MyString.h"
#include "MBMessageBox.h"
#include "MyLog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define IDC_IRRECORD_FIRST IDC_IR_B_00_UP
#define IDC_IRRECORD_LAST IDC_IR_B_25_SAVE
#define IDC_IRRECORD_STATUS_FIRST IDC_IR_S_00_UP_STATUS
#define IDC_IRRECORD_DESC_FIRST IDC_IR_D_00_UP_DESC

static int irkey;

/////////////////////////////////////////////////////////////////////////////
// CConfigIrman property page

IMPLEMENT_DYNCREATE(CConfigIrman, CPropertyPage)

CConfigIrman::CConfigIrman(CWnd * p) :
    CPropertyPage(CConfigIrman::IDD), /* m_PlayerDlg(p), */
    m_irrecording(FALSE), m_irtesting(FALSE), m_delay(100),m_lastkey(-1),
	m_keycount(0)
{
    //{{AFX_DATA_INIT(CConfigIrman)
//	m_IRComPortStatus = _T("");
	m_IRDeleteDesc = _T("");
	m_IRDeleteStatus = _T("");
	m_IRDownDesc = _T("");
	m_IRDownStatus = _T("");
	m_IRExitDesc = _T("");
	m_IRExitStatus = _T("");
	m_IRLeftDesc = _T("");
	m_IRLeftStatus = _T("");
	m_IRMenuDesc = _T("");
	m_IRMenuStatus = _T("");
	m_IRNextDesc = _T("");
	m_IRNextStatus = _T("");
	m_IRPageDownDesc = _T("");
	m_IRPageDownStatus = _T("");
	m_IRPageUpDesc = _T("");
	m_IRPageUpStatus = _T("");
	m_IRPauseDesc = _T("");
	m_IRPauseStatus = _T("");
	m_IRPlayDesc = _T("");
	m_IRPlayStatus = _T("");
	m_IRPreviousDesc = _T("");
	m_IRPreviousStatus = _T("");
	m_IRRightDesc = _T("");
	m_IRRightStatus = _T("");
	m_IRSelectDesc = _T("");
	m_IRSelectStatus = _T("");
	m_IRStopDesc = _T("");
	m_IRStopStatus = _T("");
	m_IRUpDesc = _T("");
	m_IRUpStatus = _T("");
	m_IRVolDownDesc = _T("");
	m_IRVolUpDesc = _T("");
	m_IRVolUpStatus = _T("");
	m_IRVolDownStatus = _T("");
	m_delay = 0;
	m_IRReverseDesc = _T("");
	m_IRReverseStatus = _T("");
	m_IRFastForwardDesc = _T("");
	m_IRFastForwardStatus = _T("");
	m_IRAlphaDownDesc = _T("");
	m_IRAlphaDownStatus = _T("");
	m_IRAlphaUpDesc = _T("");
	m_IRAlphaUpStatus = _T("");
	m_IRRandomDesc = _T("");
	m_IRRandomStatus = _T("");
	m_IRSaveDesc = _T("");
	m_IRSaveStatus = _T("");
	m_IRShuffleDesc = _T("");
	m_IRShuffleStatus = _T("");
	m_IRLoadDesc = _T("");
	m_IRLoadStatus = _T("");
	m_IRClearDesc = _T("");
	m_IRClearStatus = _T("");
	//}}AFX_DATA_INIT
}

CConfigIrman::~CConfigIrman()
{
	//    IRReaderStop();
}

void CConfigIrman::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigIrman)
    DDX_Text(pDX, IDC_IR_D_00_UP_DESC, m_IRUpDesc);
    DDX_Text(pDX, IDC_IR_D_01_DOWN_DESC, m_IRDownDesc);
    DDX_Text(pDX, IDC_IR_D_02_LEFT_DESC, m_IRLeftDesc);
    DDX_Text(pDX, IDC_IR_D_03_RIGHT_DESC, m_IRRightDesc);
    DDX_Text(pDX, IDC_IR_D_04_SELECT_DESC, m_IRSelectDesc);
    DDX_Text(pDX, IDC_IR_D_05_MENU_DESC, m_IRMenuDesc);
    DDX_Text(pDX, IDC_IR_D_06_EXIT_DESC, m_IRExitDesc);
    DDX_Text(pDX, IDC_IR_D_07_DELETE_DESC, m_IRDeleteDesc);
    DDX_Text(pDX, IDC_IR_D_08_VOLUP_DESC, m_IRVolUpDesc);
    DDX_Text(pDX, IDC_IR_D_09_VOLDOWN_DESC, m_IRVolDownDesc);
    DDX_Text(pDX, IDC_IR_D_10_PAGEUP_DESC, m_IRPageUpDesc);
    DDX_Text(pDX, IDC_IR_D_11_PAGEDOWN_DESC, m_IRPageDownDesc);
    DDX_Text(pDX, IDC_IR_D_12_PLAY_DESC, m_IRPlayDesc);
    DDX_Text(pDX, IDC_IR_D_13_PAUSE_DESC, m_IRPauseDesc);
    DDX_Text(pDX, IDC_IR_D_14_STOP_DESC, m_IRStopDesc);
    DDX_Text(pDX, IDC_IR_D_15_NEXT_DESC, m_IRNextDesc);
    DDX_Text(pDX, IDC_IR_D_16_PREVIOUS_DESC, m_IRPreviousDesc);
    DDX_Text(pDX, IDC_IR_D_17_FASTFORWARD_DESC, m_IRFastForwardDesc);
    DDX_Text(pDX, IDC_IR_D_18_REVERSE_DESC, m_IRReverseDesc);
    DDX_Text(pDX, IDC_IR_D_19_ALPHAUP_DESC, m_IRAlphaUpDesc);
    DDX_Text(pDX, IDC_IR_D_20_ALPHADOWN_DESC, m_IRAlphaDownDesc);
    DDX_Text(pDX, IDC_IR_D_21_RANDOM_DESC, m_IRRandomDesc);
    DDX_Text(pDX, IDC_IR_D_22_SHUFFLE_DESC, m_IRShuffleDesc);
    DDX_Text(pDX, IDC_IR_D_23_CLEAR_DESC, m_IRClearDesc);
    DDX_Text(pDX, IDC_IR_D_24_LOAD_DESC, m_IRLoadDesc);
    DDX_Text(pDX, IDC_IR_D_25_SAVE_DESC, m_IRSaveDesc);
    DDX_Text(pDX, IDC_IR_S_00_UP_STATUS, m_IRUpStatus);
    DDX_Text(pDX, IDC_IR_S_01_DOWN_STATUS, m_IRDownStatus);
    DDX_Text(pDX, IDC_IR_S_02_LEFT_STATUS, m_IRLeftStatus);
    DDX_Text(pDX, IDC_IR_S_03_RIGHT_STATUS, m_IRRightStatus);
    DDX_Text(pDX, IDC_IR_S_04_SELECT_STATUS, m_IRSelectStatus);
    DDX_Text(pDX, IDC_IR_S_05_MENU_STATUS, m_IRMenuStatus);
    DDX_Text(pDX, IDC_IR_S_06_EXIT_STATUS, m_IRExitStatus);
    DDX_Text(pDX, IDC_IR_S_07_DELETE_STATUS, m_IRDeleteStatus);
    DDX_Text(pDX, IDC_IR_S_08_VOLUP_STATUS, m_IRVolUpStatus);
    DDX_Text(pDX, IDC_IR_S_09_VOLDOWN_STATUS, m_IRVolDownStatus);
    DDX_Text(pDX, IDC_IR_S_10_PAGEUP_STATUS, m_IRPageUpStatus);
    DDX_Text(pDX, IDC_IR_S_11_PAGEDOWN_STATUS, m_IRPageDownStatus);
    DDX_Text(pDX, IDC_IR_S_12_PLAY_STATUS, m_IRPlayStatus);
    DDX_Text(pDX, IDC_IR_S_13_PAUSE_STATUS, m_IRPauseStatus);
    DDX_Text(pDX, IDC_IR_S_14_STOP_STATUS, m_IRStopStatus);
    DDX_Text(pDX, IDC_IR_S_15_NEXT_STATUS, m_IRNextStatus);
    DDX_Text(pDX, IDC_IR_S_16_PREVIOUS_STATUS, m_IRPreviousStatus);
    DDX_Text(pDX, IDC_IR_S_17_FASTFORWARD_STATUS, m_IRFastForwardStatus);
    DDX_Text(pDX, IDC_IR_S_18_REVERSE_STATUS, m_IRReverseStatus);
    DDX_Text(pDX, IDC_IR_S_19_ALPHAUP_STATUS, m_IRAlphaUpStatus);
    DDX_Text(pDX, IDC_IR_S_20_ALPHADOWN_STATUS, m_IRAlphaDownStatus);
    DDX_Text(pDX, IDC_IR_S_21_RANDOM_STATUS, m_IRRandomStatus);
    DDX_Text(pDX, IDC_IR_S_22_SHUFFLE_STATUS, m_IRShuffleStatus);
    DDX_Text(pDX, IDC_IR_S_23_CLEAR_STATUS, m_IRClearStatus);
    DDX_Text(pDX, IDC_IR_S_24_LOAD_STATUS, m_IRLoadStatus);
    DDX_Text(pDX, IDC_IR_S_25_SAVE_STATUS, m_IRSaveStatus);
	DDV_MinMaxInt(pDX, m_delay, 0, 9999);
	DDX_Text(pDX, IDC_IRDELAY, m_delay);
	DDX_Control(pDX, IDC_IRCOMPORT_STATUS, m_IRComPortStatus);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConfigIrman, CPropertyPage)
	//{{AFX_MSG_MAP(CConfigIrman)
	ON_BN_CLICKED(IDC_IRRECORD, OnIrrecord)
	ON_BN_CLICKED(IDC_IRTEST, OnIrtest)
	ON_BN_CLICKED(IDC_IRCOM1, OnIrcom1)
	ON_BN_CLICKED(IDC_IRINITIALIZE, OnIrinitialize)
	ON_BN_CLICKED(IDC_IRCOM2, OnIrcom2)
	ON_BN_CLICKED(IDC_IRCOM3, OnIrcom3)
	ON_BN_CLICKED(IDC_IRCOM4, OnIrcom4)
	ON_BN_CLICKED(IDC_IRCOM5, OnIrcom5)
	ON_BN_CLICKED(IDC_IRCOM6, OnIrcom6)
	ON_BN_CLICKED(IDC_IRCOM7, OnIrcom7)
	ON_BN_CLICKED(IDC_IRCOM8, OnIrcom8)
	ON_EN_KILLFOCUS(IDC_IRDELAY, OnKillfocusIrdelay)
	ON_BN_CLICKED(IDC_IRHELPBUTTON, OnIrhelpbutton)
	ON_LBN_SELCHANGE(IDC_IRTYPE, OnSelchangeIrtype)
	ON_EN_CHANGE(IDC_IRDELAY, OnChangeIrdelay)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(IDC_IRRECORD_FIRST, IDC_IRRECORD_LAST, OnRecordButton)
//    ON_MESSAGE(IR_MESSAGE, OnIRMessage)
	ON_MESSAGE(MB_SERIAL_MESSAGE, OnSerialMsg)
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// CConfigIrman message handlers

BOOL CConfigIrman::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	m_NeedInit = FALSE;

	OnIrinitialize();

	getDescs();

	CListBox * irtype = (CListBox*)GetDlgItem(IDC_IRTYPE);
	irtype->AddString("None");
	irtype->AddString("Irman");
	irtype->AddString("USB-UIRT");
	irtype->AddString("Tira");
	irtype->SetCurSel(RemoteReceiver::m_MBIrType);


	UpdateData(FALSE);
	EnableDisableDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void
CConfigIrman::getDescs() {
	RemoteReceiver * rrcvr = RemoteReceiver::rrcvr();
	if (rrcvr) {
		CString tmp;
		tmp = rrcvr->comPort();
		if (tmp.GetLength()) m_ComPort = tmp;
		m_delay = rrcvr->Delay();
		UpdateData(FALSE);
    
		CWnd * button;
		int button_idx;
		CString desc,rdesc;
		for (button_idx = 0 ; button_idx < rrcvr->numOfKeys() ; button_idx++) {
			button = GetDlgItem(IDC_IRRECORD_DESC_FIRST + button_idx);
			rrcvr->getDescs(button_idx, desc, rdesc);
			button->SetWindowText(rdesc);
		}
		UpdateData(TRUE);
	}
}
void
CConfigIrman::clearDescs() {
	RemoteReceiver * rrcvr = RemoteReceiver::rrcvr();
	if (rrcvr) {
		CString tmp;
		tmp = rrcvr->comPort();
		if (tmp.GetLength()) m_ComPort = tmp;
		m_delay = rrcvr->Delay();
    
		CWnd * button;
		int button_idx;
		CString desc,rdesc;
		for (button_idx = 0 ; button_idx < rrcvr->numOfKeys() ; button_idx++) {
			button = GetDlgItem(IDC_IRRECORD_DESC_FIRST + button_idx);
			button->SetWindowText("");
			button = GetDlgItem(IDC_IRRECORD_STATUS_FIRST + button_idx);
			button->SetWindowText("");
		}
		UpdateData(TRUE);
	}
}
BOOL CConfigIrman::EnableDisableDialog() 
{
	CWnd * button, * desc;
	int button_idx;
	RemoteReceiver * rrcvr = RemoteReceiver::rrcvr();	
	for (button_idx = 0 ; button_idx < (IDC_IRRECORD_LAST - IDC_IRRECORD_FIRST)+1  ; button_idx++) {
		button = GetDlgItem(IDC_IRRECORD_FIRST + button_idx);
		desc = GetDlgItem(IDC_IRRECORD_DESC_FIRST + button_idx);
		if (RemoteReceiver::m_MBIrType != RemoteReceiver::MB_IR_NONE 
				&& (rrcvr && rrcvr->ready() == TRUE) 
				&& m_irrecording == TRUE) {
			button->EnableWindow(TRUE);
			desc->EnableWindow(TRUE);
		} else {
			button->EnableWindow(FALSE);
			desc->EnableWindow(FALSE);
		}
		CWnd * button_status_w = 
		GetDlgItem(IDC_IRRECORD_STATUS_FIRST + button_idx);
		CString msg ;
		if (rrcvr) {
			msg = rrcvr->code(button_idx);
		}

		if (!msg.GetLength() || String::allSameChar(msg,'0')) {
			//msg = "undefined";
			msg = "";
		} else 
			msg = "learned";

		button_status_w->SetWindowText(msg);
	}
	UpdateData(TRUE);

	button = GetDlgItem(IDC_IRRECORD);
	if (!m_NeedInit && rrcvr && rrcvr->ready() == TRUE) {
		button->EnableWindow(TRUE);
		button = GetDlgItem(IDC_IRTEST);
		button->EnableWindow(TRUE);
		m_IRComPortStatus.SetWindowText("Ok");
		GetDlgItem(IDC_IRDELAY)->EnableWindow(TRUE);
		UpdateData(FALSE);
	} else {
		button->EnableWindow(FALSE);
		button = GetDlgItem(IDC_IRTEST);
		button->EnableWindow(FALSE);
		GetDlgItem(IDC_IRDELAY)->EnableWindow(FALSE);
	}
	if (m_NeedInit)
		m_IRComPortStatus.SetWindowText("Uninitialized");
	if (RemoteReceiver::GetType() == RemoteReceiver::MB_IR_NONE)
		m_IRComPortStatus.SetWindowText("");

	button = GetDlgItem(IDC_IRINITIALIZE);
	
	if (RemoteReceiver::GetType() != RemoteReceiver::MB_IR_NONE
			&& (m_NeedInit || (rrcvr && rrcvr->portSet()))) {
		button->EnableWindow(TRUE);
	} else {
		button->EnableWindow(FALSE);
	}

	if (rrcvr) {
		CString tmp;
		tmp = rrcvr->comPort();
		if (tmp.GetLength()) m_ComPort =tmp;
	}
	
	//CheckRadioButton( IDC_IRCOM0NONE, IDC_IRCOM8, IDC_IRCOM0NONE);
    if( _tcslen( m_ComPort ) == 4 && m_ComPort[ 3 ] >= _T('1') &&
		m_ComPort[ 3 ] <= _T('8') ) {
        CheckRadioButton( IDC_IRCOM1, IDC_IRCOM8, 
			IDC_IRCOM1 + m_ComPort[ 3 ] - _T('1') );
	}

	if (RemoteReceiver::m_MBIrType == RemoteReceiver::MB_IR_IRMAN) {
		GetDlgItem(IDC_IRMAN_SETUP)->ShowWindow(SW_NORMAL);
//		GetDlgItem(IDC_IRCOM0NONE)->ShowWindow(SW_NORMAL);
		GetDlgItem(IDC_IRCOM1)->ShowWindow(SW_NORMAL);
		GetDlgItem(IDC_IRCOM2)->ShowWindow(SW_NORMAL);
		GetDlgItem(IDC_IRCOM3)->ShowWindow(SW_NORMAL);
		GetDlgItem(IDC_IRCOM4)->ShowWindow(SW_NORMAL);
		GetDlgItem(IDC_IRCOM5)->ShowWindow(SW_NORMAL);
		GetDlgItem(IDC_IRCOM6)->ShowWindow(SW_NORMAL);
		GetDlgItem(IDC_IRCOM7)->ShowWindow(SW_NORMAL);
		GetDlgItem(IDC_IRCOM8)->ShowWindow(SW_NORMAL);
//		GetDlgItem(IDC_IRINITIALIZE)->ShowWindow(SW_NORMAL);
//		GetDlgItem(IDC_IRCOMPORT_STATUS)->ShowWindow(SW_NORMAL);
	} else {
		GetDlgItem(IDC_IRMAN_SETUP)->ShowWindow(SW_HIDE);
//		GetDlgItem(IDC_IRCOM0NONE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_IRCOM1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_IRCOM2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_IRCOM3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_IRCOM4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_IRCOM5)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_IRCOM6)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_IRCOM7)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_IRCOM8)->ShowWindow(SW_HIDE);
//		GetDlgItem(IDC_IRINITIALIZE)->ShowWindow(SW_HIDE);
//		GetDlgItem(IDC_IRCOMPORT_STATUS)->ShowWindow(SW_HIDE);

	}
	if (m_irtesting) {
		GetDlgItem(IDC_IR_READ_TEST_LABEL)->ShowWindow(SW_NORMAL);
		GetDlgItem(IDC_IRTEST_STATUS)->ShowWindow(SW_NORMAL);
	} else {
		GetDlgItem(IDC_IR_READ_TEST_LABEL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_IRTEST_STATUS)->ShowWindow(SW_HIDE);
	}
	UpdateData(FALSE);	
	return TRUE;
}

void CConfigIrman::OnRecordButton(UINT nID ) 
{
	int button = nID - IDC_IRRECORD_FIRST;
	CWnd * button_status_w = GetDlgItem(IDC_IRRECORD_STATUS_FIRST + button);

	if (m_irrecording) {
		button_status_w->SetWindowText("reading...");
		RemoteReceiver * rrcvr = RemoteReceiver::rrcvr();	
		if (rrcvr)
			rrcvr->SetKey(button);
	}
	SetModified(TRUE);
}

void CConfigIrman::OnIrrecord() 
{
	m_irrecording = TRUE;
	m_irtesting = FALSE;
//	RemoteReceiver * rrcvr = RemoteReceiver::rrcvr();	
//	rrcvr->Open();
	EnableDisableDialog();
	SetModified(TRUE);
}

void CConfigIrman::OnIrtest() 
{
	m_irtesting = TRUE;
	m_irrecording = FALSE;
//    IRReaderStart();
//	RemoteReceiver * rrcvr = RemoteReceiver::rrcvr();	
//	rrcvr->Open();
	UpdateData(TRUE);
	RemoteReceiver * rrcvr = RemoteReceiver::rrcvr();	
	if (rrcvr) rrcvr->Delay(m_delay);
	EnableDisableDialog();
	SetModified(TRUE);
}


void CConfigIrman::OnIrinitialize() 
{
    m_IRComPortStatus.SetWindowText("Uninitialized");

	RemoteReceiver * rrcvr = RemoteReceiver::reset(this, MB_SERIAL_MESSAGE);
	int type = rrcvr->GetType();

	m_irrecording = FALSE;
	m_irtesting = FALSE;

	CButton * button;
	button = (CButton*)GetDlgItem(IDC_IRRECORD);
	button->SetCheck(0);
	button = (CButton*)GetDlgItem(IDC_IRTEST);
	button->SetCheck(0);

	if (NULL == rrcvr) {
		CString msg = "unable to initialize Remote Receiver.\r\nCheck muzikbrowzer.log for details.";
		if (RemoteReceiver::MB_IR_TIRA == type)
			msg += "\r\nTira not supported yet.";
		MBMessageBox("Remote Receiver Error",msg,TRUE,FALSE);
		return;
	}
	m_NeedInit = FALSE;

	GetDlgItem(IDC_IRTEST_STATUS)->SetWindowText("");
	m_keycount = 0;
	UpdateData(FALSE);
	EnableDisableDialog();
	SetModified(TRUE);
	getDescs();
}
void CConfigIrman::OnIrnone() 
{
	RemoteReceiver * rrcvr = RemoteReceiver::rrcvr();	
	if (rrcvr) rrcvr->Port("");
	m_IRComPortStatus.SetWindowText("");
	UpdateData(FALSE);
	EnableDisableDialog();	
	SetModified(TRUE);
}

void CConfigIrman::OnIrcom1() 
{
	RemoteReceiver * rrcvr = RemoteReceiver::rrcvr();	
	if (rrcvr) rrcvr->Port("COM1");
	m_IRComPortStatus.SetWindowText("Uninitialized");
	UpdateData(FALSE);
	EnableDisableDialog();
	SetModified(TRUE);
}
void CConfigIrman::OnIrcom2() 
{
	RemoteReceiver * rrcvr = RemoteReceiver::rrcvr();	
	if (rrcvr) rrcvr->Port("COM2");
	m_IRComPortStatus.SetWindowText("Uninitialized");
	UpdateData(FALSE);	
	EnableDisableDialog();
	SetModified(TRUE);
}

void CConfigIrman::OnIrcom3() 
{
	RemoteReceiver * rrcvr = RemoteReceiver::rrcvr();	
	if (rrcvr) rrcvr->Port("COM3");
	m_IRComPortStatus.SetWindowText("Uninitialized");
	UpdateData(FALSE);
	EnableDisableDialog();
	SetModified(TRUE);
}

void CConfigIrman::OnIrcom4() 
{
	RemoteReceiver * rrcvr = RemoteReceiver::rrcvr();	
	if (rrcvr) rrcvr->Port("COM4");
	m_IRComPortStatus.SetWindowText("Uninitialized");
	UpdateData(FALSE);	
	EnableDisableDialog();
	SetModified(TRUE);
}

void CConfigIrman::OnIrcom5() 
{
	RemoteReceiver * rrcvr = RemoteReceiver::rrcvr();	
	if (rrcvr) rrcvr->Port("COM5");
	m_IRComPortStatus.SetWindowText("Uninitialized");
	UpdateData(FALSE);	
	EnableDisableDialog();
	SetModified(TRUE);
}

void CConfigIrman::OnIrcom6() 
{
	RemoteReceiver * rrcvr = RemoteReceiver::rrcvr();	
	if (rrcvr) rrcvr->Port("COM6");
	m_IRComPortStatus.SetWindowText("Uninitialized");
	UpdateData(FALSE);	
	EnableDisableDialog();
	SetModified(TRUE);
}

void CConfigIrman::OnIrcom7() 
{
	RemoteReceiver * rrcvr = RemoteReceiver::rrcvr();	
	if (rrcvr) rrcvr->Port("COM7");
	m_IRComPortStatus.SetWindowText("Uninitialized");
	UpdateData(FALSE);	
	EnableDisableDialog();
	SetModified(TRUE);
}

void CConfigIrman::OnIrcom8() 
{
	RemoteReceiver * rrcvr = RemoteReceiver::rrcvr();	
	if (rrcvr) rrcvr->Port("COM8");
	m_IRComPortStatus.SetWindowText("Uninitialized");
	UpdateData(FALSE);	
	EnableDisableDialog();
	SetModified(TRUE);
}

void CConfigIrman::OnKillfocusIrdelay() 
{
	UpdateData(TRUE);
	RemoteReceiver * rrcvr = RemoteReceiver::rrcvr();	
	if (rrcvr) rrcvr->Delay(m_delay);
	SetModified(TRUE);
}

int
CConfigIrman::HandleIRMessage(int key) {

	if (!m_irtesting) {
		return 0;
	}
	RemoteReceiver * rrcvr = RemoteReceiver::rrcvr();	
	if (rrcvr && 0 <= key && key < rrcvr->numOfKeys()) {
		CWnd * functionButton = GetDlgItem(IDC_IRRECORD_FIRST + key);
		if (key == m_lastkey) {
			m_keycount++;
		} else {
			m_keycount = 1;
		}
		m_lastkey = key;
		CString caption;
		functionButton->GetWindowText(caption);
		CWnd * status = GetDlgItem(IDC_IRTEST_STATUS);
		caption += ":" + numToString(m_keycount);
		status->SetWindowText(caption);
	}

    return 0;
}


afx_msg LRESULT
CConfigIrman::OnSerialMsg (WPARAM wParam, LPARAM lParam) {
	int key;
	BOOL more = TRUE;
	RemoteReceiver * rrcvr = RemoteReceiver::rrcvr();	
	while (rrcvr && more) {
		more = rrcvr->HandleSerialMsg(wParam, lParam, key);
		HandleIRMessage(key);
	}
	EnableDisableDialog();
	SetModified(TRUE);
	return 0;

}
void CConfigIrman::OnOK() 
{
	CWnd * button;
	int button_idx;
	CString desc;
	RemoteReceiver * rrcvr = RemoteReceiver::rrcvr();	
	if (rrcvr) {
		for (button_idx = 0 ; button_idx < rrcvr->numOfKeys() ; button_idx++) {
			button = GetDlgItem(IDC_IRRECORD_DESC_FIRST + button_idx);
 			button->GetWindowText(desc);
			if (desc.GetLength()) {
				rrcvr->setRemoteDesc(button_idx, (const char*)desc);
			}

			button = GetDlgItem(IDC_IRRECORD_FIRST + button_idx);
 			button->GetWindowText(desc);
			if (desc.GetLength()) {
				rrcvr->setKeyDesc(button_idx, (const char*)desc);
			}
		}
		rrcvr->SaveKeys();
	}
	// Don't do this, if hit apply then no longer testing - annoying
	//m_irtesting = FALSE;
	SetModified(FALSE);
	CPropertyPage::OnOK();
}
BOOL CConfigIrman::OnApply() 
{
	OnOK();
	return CPropertyPage::OnApply();
}

void CConfigIrman::OnCancel() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CPropertyPage::OnCancel();
}



void CConfigIrman::OnIrhelpbutton() 
{
	CString msg;
	msg += "Select the COM port your IRman is plugged into.\r\n";
	msg += "Click \"Initialize\". You should get an \"OK\" \r\n";
	msg += "in the status.\r\n";
	msg += "\r\n";
	msg += "Click \"Record\".  Click \"Up\".\r\n";
	msg += "Now press the UP button on your remote control\r\n";
	msg += "and the code window should display the code\r\n";
	msg += "read by Irman.  Repeat for all remaining control\r\n";
	msg += "buttons.\r\n";
	msg += "\r\n";
	msg += "Click \"Test\".  Now click various buttons on your\r\n";
	msg += "remote control and you should see the correct\r\n";
	msg += "description displayed in the Read Window.  Eash\r\n";
	msg += "press of a button on your remote control should\r\n";
	msg += "increment the Read count by 1.\r\n";
	msg += "\r\n";
	msg += "Some remote controls send more than one signal per\r\n";
	msg += "button press. In this case the Read count will\r\n";
	msg += "increment by more than 1 each time.\r\n";
	msg += "\r\n";
	msg += "The Key Suppress parameter allows Muzikbrowzer to\r\n";
	msg += "ignore the extra signals sent by your remote\r\n";
	msg += "control. Successive signals received within this\r\n";
	msg += "time parameter will be ignored.\r\n";
	msg += "\r\n";
	msg += "The Key Suppress value should be as low as possible\r\n";
	msg += "without producing Read increments greater than 1.\r\n";

	MBMessageBox("IRMan Help",msg,FALSE,FALSE);
	
}

void CConfigIrman::OnSetfocusIrdelay() 
{
	OnIrinitialize() ;
	
}

void CConfigIrman::OnSelchangeIrtype() 
{
	m_NeedInit = TRUE;
	m_IRComPortStatus.SetWindowText("");
	CListBox * irtype = (CListBox*)GetDlgItem(IDC_IRTYPE);
	int sel = irtype->GetCurSel();
	CString type;
	irtype->GetText(sel, type);
	logger.logd("irtype:"+type);	

	if ("Irman" == type) {
		RemoteReceiver::SetType(RemoteReceiver::MB_IR_IRMAN);
		RegistryKey reg( HKEY_LOCAL_MACHINE, RegKeyIrman );
		m_ComPort = reg.ReadCString("Port","");
		m_delay = (unsigned long)reg.Read("InterKeyDelay",0);
	} else if ("USB-UIRT" == type) {
		RemoteReceiver::SetType(RemoteReceiver::MB_IR_USBUIRT);
		RegistryKey reg( HKEY_LOCAL_MACHINE, RegKeyUsbUirt );
		m_delay = (unsigned long)reg.Read("InterKeyDelay",0);
	} else if ("Tira" == type) {
		RemoteReceiver::SetType(RemoteReceiver::MB_IR_TIRA);
		RegistryKey reg( HKEY_LOCAL_MACHINE, RegKeyTira );
		m_delay = (unsigned long)reg.Read("InterKeyDelay",0);
	} else { // "None"
		RemoteReceiver::SetType(RemoteReceiver::MB_IR_NONE);
		m_IRComPortStatus.SetWindowText("");
	}

	UpdateData(FALSE);
	if (RemoteReceiver::GetType() == RemoteReceiver::MB_IR_NONE) {
		RemoteReceiver::reset(this,MB_SERIAL_MESSAGE);
	}
	EnableDisableDialog();
	clearDescs();
	SetModified(TRUE);
}





void CConfigIrman::OnChangeIrdelay() 
{
	UpdateData(TRUE);
	RemoteReceiver * rrcvr = RemoteReceiver::rrcvr();	
	if (rrcvr) rrcvr->Delay(m_delay);
}
void CConfigIrman::getSettings(MyHash & settings) {
	settings.setVal("RemoteType",RemoteReceiver::GetTypeString());
}
