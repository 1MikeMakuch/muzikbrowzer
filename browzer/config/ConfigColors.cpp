// ConfigColors.cpp : implementation file
//

//#include "stdafx.h"
#include "irman_registry.h"
#include "ConfigColors.h"
#include "MyString.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfigColors property page

IMPLEMENT_DYNCREATE(CConfigColors, CPropertyPage)

CConfigColors::CConfigColors(CWnd * p) : CPropertyPage(CConfigColors::IDD),
    m_PlayerDlg(p)
{
	//{{AFX_DATA_INIT(CConfigColors)
	//}}AFX_DATA_INIT
    setDefaults();
    ReadReg();
}

CConfigColors::~CConfigColors()
{
}

void CConfigColors::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigColors)
	DDX_Control(pDX, IDC_COLOR_TX_PANEL, m_TxPanel);
	DDX_Control(pDX, IDC_COLOR_BK_PANEL, m_BkPanel);
	DDX_Control(pDX, IDC_COLOR_TX_SEL, m_TxSel);
	DDX_Control(pDX, IDC_COLOR_TX_NORMAL, m_TxNormal);
	DDX_Control(pDX, IDC_COLOR_TX_HIGH, m_TxHigh);
	DDX_Control(pDX, IDC_COLOR_BK_SEL, m_BkSel);
	DDX_Control(pDX, IDC_COLOR_BK_NORMAL, m_BkNormal);
	DDX_Control(pDX, IDC_COLOR_BK_HIGH, m_BkHigh);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConfigColors, CPropertyPage)
	//{{AFX_MSG_MAP(CConfigColors)
	ON_BN_CLICKED(IDC_SET_DEFAULT, OnSetDefault)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigColors message handlers
void
CConfigColors::setDefaults() {
    m_vBkPanel = RGB(0,0,0);
    m_vBkNormal = RGB(0,0,0);
    m_vBkHigh = RGB(0,0,0);
    m_vBkSel = 16711680;
    m_vTxPanel = 16777215;
    m_vTxNormal = 16777215;
    m_vTxHigh = 65535;
    m_vTxSel = 65535;
    v2m();
}
void
CConfigColors::ReadReg() {
    RegistryKey reg( HKEY_LOCAL_MACHINE, RegKey );

    m_vBkPanel = reg.Read(RegWindowsColorBkPanel, m_vBkPanel);
    m_vBkNormal = reg.Read(RegWindowsColorBkNormal, m_vBkNormal);
    m_vBkHigh = reg.Read(RegWindowsColorBkHigh, m_vBkHigh);
    m_vBkSel = reg.Read(RegWindowsColorBkSel, m_vBkSel);
    m_vTxPanel = reg.Read(RegWindowsColorTxPanel, m_vTxPanel);
    m_vTxNormal = reg.Read(RegWindowsColorTxNormal, m_vTxNormal);
    m_vTxHigh = reg.Read(RegWindowsColorTxHigh, m_vTxHigh);
    m_vTxSel = reg.Read(RegWindowsColorTxSel, m_vTxSel);
    v2m();
}
void
CConfigColors::StoreReg() {
    RegistryKey reg( HKEY_LOCAL_MACHINE, RegKey );

    reg.Write(RegWindowsColorBkPanel, m_vBkPanel);
    reg.Write(RegWindowsColorBkNormal, m_vBkNormal);
    reg.Write(RegWindowsColorBkHigh, m_vBkHigh);
    reg.Write(RegWindowsColorBkSel, m_vBkSel);
    reg.Write(RegWindowsColorTxPanel, m_vTxPanel);
    reg.Write(RegWindowsColorTxNormal, m_vTxNormal);
    reg.Write(RegWindowsColorTxHigh, m_vTxHigh);
    reg.Write(RegWindowsColorTxSel, m_vTxSel);
}
void CConfigColors::v2m() {
    m_BkPanel.currentcolor = m_vBkPanel;
    m_BkNormal.currentcolor = m_vBkNormal;
    m_BkHigh.currentcolor = m_vBkHigh;
    m_BkSel.currentcolor = m_vBkSel;
    m_TxPanel.currentcolor = m_vTxPanel;
    m_TxNormal.currentcolor = m_vTxNormal;
    m_TxHigh.currentcolor = m_vTxHigh;
    m_TxSel.currentcolor = m_vTxSel;

}
void CConfigColors::m2v() {
    m_vBkPanel = m_BkPanel.currentcolor;
    m_vBkNormal = m_BkNormal.currentcolor;
    m_vBkHigh = m_BkHigh.currentcolor;
    m_vBkSel = m_BkSel.currentcolor;
    m_vTxPanel = m_TxPanel.currentcolor;
    m_vTxNormal = m_TxNormal.currentcolor;
    m_vTxHigh = m_TxHigh.currentcolor;
    m_vTxSel = m_TxSel.currentcolor;
}
void CConfigColors::OnOK() 
{
    m2v();
	StoreReg();
	
	CPropertyPage::OnOK();
}

BOOL CConfigColors::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
    m_BkPanel.SubclassDlgItem(IDC_COLOR_BK_PANEL, (CWnd*)m_PlayerDlg);
    m_BkNormal.SubclassDlgItem(IDC_COLOR_BK_NORMAL, (CWnd*)m_PlayerDlg);
    m_BkHigh.SubclassDlgItem(IDC_COLOR_BK_HIGH, (CWnd*)m_PlayerDlg);
    m_BkSel.SubclassDlgItem(IDC_COLOR_BK_SEL, (CWnd*)m_PlayerDlg);
    m_TxPanel.SubclassDlgItem(IDC_COLOR_TX_PANEL, (CWnd*)m_PlayerDlg);
    m_TxNormal.SubclassDlgItem(IDC_COLOR_TX_NORMAL, (CWnd*)m_PlayerDlg);
    m_TxHigh.SubclassDlgItem(IDC_COLOR_TX_HIGH, (CWnd*)m_PlayerDlg);
    m_TxSel.SubclassDlgItem(IDC_COLOR_TX_SEL, (CWnd*)m_PlayerDlg);
    
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CConfigColors::OnCancel() 
{
	v2m();
	
	CPropertyPage::OnCancel();
}

void CConfigColors::OnSetDefault() 
{
    setDefaults();	
    UpdateData(FALSE);
//    UpdateWindow();
    RedrawWindow();
}
