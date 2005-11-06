// ConfigDisplay.cpp : implementation file
//

//#include "stdafx.h"
#include "Registry.h"
#include "ConfigDisplay.h"
#include "MyString.h"
#include "MBGlobals.h"
#include "SkinDefs.h"
#include "MBMessageBox.h"
#include "MyLog.h"

#include "FileUtils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CCFONTFMT "%08X %08X %08X %08X %08X %02X %02X %02X %02X %02X %02X %02X %02X "
#define CCFONTFACEPOS 69
char deffont[] = "FFFFFFF4 00000000 00000000 00000000 000002BC 00 00 00 00 00 00 00 00 Arial";
char deffontfacename[] = "Arial";
static unsigned int italic;
static unsigned int underline;
static unsigned int strikeout;
static unsigned int charset;
static unsigned int outprecision;
static unsigned int clipprecision;
static unsigned int quality;
static unsigned int pitchandfamily;

/////////////////////////////////////////////////////////////////////////////
// CConfigDisplay property page

IMPLEMENT_DYNCREATE(CConfigDisplay, CPropertyPage)

CConfigDisplay::CConfigDisplay(CWnd * p, PlayerCallbacks * pcb) 
		: CPropertyPage(CConfigDisplay::IDD),
    m_lplfTitles(0), m_lplfPanel(0), m_lplfColHdr(0), m_playercallbacks(pcb),
	m_Modified(FALSE)
{
	//{{AFX_DATA_INIT(CConfigDisplay)
	//}}AFX_DATA_INIT
    memset(&m_lfTitles, 0, sizeof(LOGFONT));
	memset(&m_lfPanel, 0, sizeof(LOGFONT));
	memset(&m_lfColHdr, 0, sizeof(LOGFONT));
	memset(&m_lfCurPlay,0,sizeof(LOGFONT));
    init();
    ReadReg(RegistryKey( HKEY_LOCAL_MACHINE, RegKey ),TRUE);

	// Read skin def
	CString skindef = getSkin(MB_SKIN_DEF);
	RegistryKey regSD(skindef);
	regSD.ReadFile();

	// read skin def custom
	CString save = m_sSkinName;
	CString skindefcustom = getSkin(MB_SKIN_DEF_CUSTOM);
	RegistryKey regSDCustom(skindefcustom);
	regSDCustom.ReadFile();

	// custom overlays standard
	regSD.Copy(regSDCustom);
	ReadReg(regSD);
	m_sSkinName = save;

}

CConfigDisplay::~CConfigDisplay()
{
	m_FontSampleTitles.DeleteObject();
	m_FontSamplePanel.DeleteObject();
	m_FontSampleColHdr.DeleteObject();
	m_FontSampleSel.DeleteObject();
	m_FontSampleHigh.DeleteObject();
	m_FontSampleCurPlay.DeleteObject();
}

void CConfigDisplay::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigDisplay)
	DDX_Control(pDX, IDC_SAMPLE_CURPLY, m_SampleCurPlay);
	DDX_Control(pDX, IDC_FONTSIZE_CURPLY, m_SizeCurPlay);
	DDX_Control(pDX, IDC_FONT_CURPLY, m_FontCurPlay);
	DDX_Control(pDX, IDC_COLOR_TX_CURPLY, m_TxCurPlay);
	DDX_Control(pDX, IDC_COLOR_BK_CURPLY, m_BkCurPlay);
	DDX_Control(pDX, IDC_BOLD_CURPLY, m_BoldCurPlay);
//	DDX_Control(pDX, IDC_BORDER_VERT, m_BorderVert);
//	DDX_Control(pDX, IDC_BORDER_HORZ, m_BorderHorz);
     DDX_Control(pDX, IDC_SPIN_GENRE,			m_GenreSpin);
//     DDX_Control(pDX, IDC_BORDER_PANEL,         m_PanelWidth);
     DDX_Control(pDX, IDC_SKIN_LIST,            m_SkinList);
     DDX_Control(pDX, IDC_BORDER_WIDTH,         m_BorderWidth);
     DDX_Control(pDX, IDC_BOLD_COLHDR,          m_BoldColHdr);
     DDX_Control(pDX, IDC_BOLD_PANEL,           m_BoldPanel);
     DDX_Control(pDX, IDC_BOLD_TITLES,          m_BoldTitles);
     DDX_Control(pDX, IDC_COLOR_BK_COLHDR,		m_BkColHdr);
     DDX_Control(pDX, IDC_COLOR_BK_HIGH,        m_BkHigh);
     DDX_Control(pDX, IDC_COLOR_BK_NORMAL,		m_BkNormal);
     DDX_Control(pDX, IDC_COLOR_BK_PANEL,		m_BkPanel);
     DDX_Control(pDX, IDC_COLOR_BK_SEL,         m_BkSel);
     DDX_Control(pDX, IDC_COLOR_TX_COLHDR,		m_TxColHdr);
     DDX_Control(pDX, IDC_COLOR_TX_HIGH,        m_TxHigh);
     DDX_Control(pDX, IDC_COLOR_TX_NORMAL,		m_TxNormal);
     DDX_Control(pDX, IDC_COLOR_TX_PANEL,		m_TxPanel);
     DDX_Control(pDX, IDC_COLOR_TX_SEL,         m_TxSel);
     DDX_Control(pDX, IDC_SAMPLE_COLHDR,        m_SampleColHdr);
     DDX_Control(pDX, IDC_SAMPLE_HIGH,          m_SampleHigh);
     DDX_Control(pDX, IDC_SAMPLE_PANEL,         m_SamplePanel);
     DDX_Control(pDX, IDC_SAMPLE_SEL,           m_SampleSel);
     DDX_Control(pDX, IDC_SAMPLE_TITLES,        m_SampleTitles);
     DDX_Control(pDX, IDC_FONTSIZE_COLHDR,		m_SizeColHdr);
     DDX_Control(pDX, IDC_FONTSIZE_PANEL,		m_SizePanel);
     DDX_Control(pDX, IDC_FONTSIZE,             m_SizeTitles);
     DDX_Control(pDX, IDC_FONT_COLHDR,          m_FontColHdr);
     DDX_Control(pDX, IDC_FONT_PANEL,           m_FontPanel);
     DDX_Control(pDX, IDC_FONT,                 m_FontTitles);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConfigDisplay, CPropertyPage)
	//{{AFX_MSG_MAP(CConfigDisplay)
    ON_WM_PAINT()	
    ON_CBN_EDITCHANGE(IDC_FONT_COLHDR,              OnSelchangeFont)
    ON_BN_CLICKED(IDC_SKIN_DELETE,                  OnSkinDelete)
    ON_BN_CLICKED(IDC_BOLD_COLHDR,                  onbold)
    ON_BN_CLICKED(IDC_BOLD_PANEL,                   onbold)
    ON_BN_CLICKED(IDC_BOLD_TITLES,                  onbold)
	ON_BN_CLICKED(IDC_BOLD_CURPLY,                  onbold)
    ON_CBN_EDITCHANGE(IDC_BORDER_PANEL,             OnUpdateWidth)
    ON_CBN_EDITCHANGE(IDC_BORDER_WIDTH,             OnUpdateWidth)
	ON_CBN_EDITCHANGE(IDC_BORDER_HORZ,             OnUpdateWidth)
	ON_CBN_EDITCHANGE(IDC_BORDER_VERT,             OnUpdateWidth)
    ON_CBN_EDITCHANGE(IDC_FONT,                     OnSelchangeFont)
    ON_CBN_EDITCHANGE(IDC_FONT_PANEL,               OnSelchangeFont)
    ON_CBN_EDITCHANGE(IDC_FONTSIZE_COLHDR,			OnSelchangeFont)
    ON_CBN_EDITCHANGE(IDC_FONTSIZE,                 OnSelchangeFont)
    ON_CBN_EDITCHANGE(IDC_FONTSIZE_PANEL,			OnSelchangeFont)
	ON_CBN_EDITCHANGE(IDC_FONTSIZE_CURPLY,			OnSelchangeFont)
    ON_CBN_EDITUPDATE(IDC_BORDER_PANEL,             OnUpdateWidth)
    ON_CBN_EDITUPDATE(IDC_BORDER_WIDTH,             OnUpdateWidth)
	ON_CBN_EDITUPDATE(IDC_BORDER_HORZ,             OnUpdateWidth)
	ON_CBN_EDITUPDATE(IDC_BORDER_VERT,             OnUpdateWidth)
    ON_CBN_EDITUPDATE(IDC_FONT_COLHDR,              OnSelchangeFont)
    ON_CBN_EDITUPDATE(IDC_FONT,                     OnSelchangeFont)
    ON_CBN_EDITUPDATE(IDC_FONT_PANEL,               OnSelchangeFont)
    ON_CBN_EDITUPDATE(IDC_FONTSIZE_COLHDR,			OnSelchangeFont)
    ON_CBN_EDITUPDATE(IDC_FONTSIZE,                 OnSelchangeFont)
    ON_CBN_EDITUPDATE(IDC_FONTSIZE_PANEL,			OnSelchangeFont)
    ON_CBN_SELCHANGE(IDC_BORDER_PANEL,              OnUpdateWidth)
    ON_CBN_SELCHANGE(IDC_BORDER_WIDTH,              OnUpdateWidth)
	ON_CBN_SELCHANGE(IDC_BORDER_HORZ,              OnUpdateWidth)
	ON_CBN_SELCHANGE(IDC_BORDER_VERT,              OnUpdateWidth)
    ON_CBN_SELCHANGE(IDC_FONT_COLHDR,               OnSelchangeFont)
    ON_CBN_SELCHANGE(IDC_FONT,                      OnSelchangeFont)
    ON_CBN_SELCHANGE(IDC_FONT_PANEL,                OnSelchangeFont)
    ON_CBN_SELCHANGE(IDC_FONTSIZE_COLHDR,			OnSelchangeFont)
	ON_CBN_SELCHANGE(IDC_FONTSIZE_CURPLY,			OnSelchangeFont)
    ON_CBN_SELCHANGE(IDC_FONTSIZE,                  OnSelchangeFont)
    ON_CBN_SELCHANGE(IDC_FONTSIZE_PANEL,			OnSelchangeFont)
    ON_CBN_SELENDOK(IDC_BORDER_PANEL,               OnUpdateWidth)
    ON_CBN_SELENDOK(IDC_BORDER_WIDTH,               OnUpdateWidth)
	ON_CBN_SELENDOK(IDC_BORDER_HORZ,               OnUpdateWidth)
	ON_CBN_SELENDOK(IDC_BORDER_VERT,               OnUpdateWidth)
    ON_CBN_SELENDOK(IDC_FONT_COLHDR,                OnSelchangeFont)
    ON_CBN_SELENDOK(IDC_FONT,                       OnSelchangeFont)
    ON_CBN_SELENDOK(IDC_FONT_PANEL,                 OnSelchangeFont)
    ON_CBN_SELENDOK(IDC_FONTSIZE_COLHDR,			OnSelchangeFont)
    ON_CBN_SELENDOK(IDC_FONTSIZE,                   OnSelchangeFont)
    ON_CBN_SELENDOK(IDC_FONTSIZE_PANEL,             OnSelchangeFont)
    ON_CBN_SELENDOK(IDC_SKIN_LIST,                  OnSkinChoose)
    ON_EN_CHANGE(IDC_GENRE_WIDTH,                   OnUpdateWidth)
	ON_CBN_SELENDOK(IDC_COLOR_BK_COLHDR,			OnColorButton)
	ON_BN_CLICKED(IDC_COLOR_BK_COLHDR,              OnColorButton)
	ON_BN_CLICKED(IDC_COLOR_BK_CURPLY,              OnColorButton)
	ON_BN_CLICKED(IDC_COLOR_TX_CURPLY,              OnColorButton)
	ON_BN_CLICKED(IDC_COLOR_BK_HIGH,                OnColorButton)
	ON_BN_CLICKED(IDC_COLOR_BK_NORMAL,              OnColorButton)
	ON_BN_CLICKED(IDC_COLOR_BK_PANEL,               OnColorButton)
	ON_BN_CLICKED(IDC_COLOR_BK_SEL,					OnColorButton)
	ON_BN_CLICKED(IDC_COLOR_BORDERS,                OnColorButton)
	ON_BN_CLICKED(IDC_COLOR_TX_COLHDR,              OnColorButton)
	ON_BN_CLICKED(IDC_COLOR_TX_HIGH,                OnColorButton)
	ON_BN_CLICKED(IDC_COLOR_TX_NORMAL,              OnColorButton)
	ON_BN_CLICKED(IDC_COLOR_TX_PANEL,               OnColorButton)
	ON_BN_CLICKED(IDC_COLOR_TX_SEL,					OnColorButton)
	//}}AFX_MSG_MAP
	ON_MESSAGE(MB_CONFIG_CBUTTON, OnCButtonMessage)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigDisplay message handlers
BOOL CConfigDisplay::OnInitDialog() 
{
	AutoLog alog("CCD::OnInitDialog");
	CPropertyPage::OnInitDialog();	
    
//	m_FontTitles.SubclassDlgItem(IDC_FONT, (CWnd*)m_PlayerDlg);
//	m_FontPanel.SubclassDlgItem(IDC_FONT_PANEL, (CWnd*)m_PlayerDlg);
//	m_FontColHdr.SubclassDlgItem(IDC_FONT_COLHDR, (CWnd*)m_PlayerDlg);
//	m_FontCurPlay.SubclassDlgItem(IDC_FONT_CURPLY, (CWnd*)m_PlayerDlg);
    m_FontPanel.Initialize();
    m_FontTitles.Initialize();
    m_FontColHdr.Initialize();
	m_FontCurPlay.Initialize();


//	m_BkColHdr.SubclassDlgItem(IDC_COLOR_BK_COLHDR, (CWnd*)m_PlayerDlg);
//	m_BkCtrls.SubclassDlgItem(IDC_COLOR_BK_CTRLS, (CWnd*)m_PlayerDlg);
//	m_BkHigh.SubclassDlgItem(IDC_COLOR_BK_HIGH, (CWnd*)m_PlayerDlg);
//	m_BkNormal.SubclassDlgItem(IDC_COLOR_BK_NORMAL, (CWnd*)m_PlayerDlg);
//	m_BkPanel.SubclassDlgItem(IDC_COLOR_BK_PANEL, (CWnd*)m_PlayerDlg);
//	m_BkSel.SubclassDlgItem(IDC_COLOR_BK_SEL, (CWnd*)m_PlayerDlg);
//	m_Border.SubclassDlgItem(IDC_COLOR_BORDERS, (CWnd*)m_PlayerDlg);
//	m_TxColHdr.SubclassDlgItem(IDC_COLOR_TX_COLHDR, (CWnd*)m_PlayerDlg);
//	m_TxCtrls.SubclassDlgItem(IDC_COLOR_TX_CTRLS, (CWnd*)m_PlayerDlg);
//	m_TxHigh.SubclassDlgItem(IDC_COLOR_TX_HIGH, (CWnd*)m_PlayerDlg);
//	m_TxNormal.SubclassDlgItem(IDC_COLOR_TX_NORMAL, (CWnd*)m_PlayerDlg);
//	m_TxPanel.SubclassDlgItem(IDC_COLOR_TX_PANEL, (CWnd*)m_PlayerDlg);
//	m_TxSel.SubclassDlgItem(IDC_COLOR_TX_SEL, (CWnd*)m_PlayerDlg);    


//	init();

    AutoBuf buf(5);
    int i;

    m_SizeTitles.ResetContent();
	m_SizePanel.ResetContent();
	m_SizeColHdr.ResetContent();
	m_SizeCurPlay.ResetContent();
	m_BorderWidth.ResetContent();

    for (i = 5 ; i < 40; ++i) {
        sprintf(buf.p, "%d", i);
        m_SizeTitles.AddString(buf.p);
		m_SizePanel.AddString(buf.p);
		m_SizeColHdr.AddString(buf.p);
		m_SizeCurPlay.AddString(buf.p);
    }

    for (i = 0 ; i < 40; ++i) {
        sprintf(buf.p, "%d", i);
        m_BorderWidth.AddString(buf.p);
    }

	readSkins();
	initFontSels();

	int sel = m_SkinList.SelectString(-1, m_sSkinName);
	m_SkinList.SetCurSel(sel);
	OnSkinChoose();

	sel = m_BorderWidth.SelectString(-1, numToString(m_vBorderWidth));
	m_BorderWidth.SetCurSel(sel);

    showSample();

	m_GenreSpin.SetRange(MB_GENRE_WIDTH_PCT_MIN,MB_GENRE_WIDTH_PCT_MAX);
	m_GenreSpin.SetPos(m_vGenreWidthPct);

	StoreReg(RegistryKey( HKEY_LOCAL_MACHINE, RegKeyPrevVals ),TRUE);

	m_Modified = FALSE;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void
CConfigDisplay::init() {
	CString dir;
	dir = (*m_playercallbacks->mbdir)();
    setDefaults();
	m_SkinDir = dir;
	m_SkinDir += "\\skins";

    RegistryKey reg( HKEY_LOCAL_MACHINE, RegKey );
	AutoBuf buf(1000);
	reg.Read("SkinsDir", buf.p,999,m_SkinDir);
	CString tmp = buf.p;
	if (tmp.GetLength()) {
		m_SkinDir = tmp;
	}
}
void
CConfigDisplay::setDefaults() {
    m_vBkPanel = RGB(0,0,0);
    m_vBkNormal = RGB(0,0,0);
    m_vBkHigh = RGB(0,0,0);
	m_vBkCurPlay = RGB(0,0,0);
	m_vBkSel = 16711680;
    m_vTxPanel = 16777215;
    m_vTxNormal = 16777215;
    m_vTxHigh = 65535;
    m_vTxSel = 65535;
	m_vTxColHdr = 16777215;
	m_vBkColHdr = RGB(0,0,0);
	m_vTxCurPlay = RGB(255,255,255);
    v2m();
	m_sSkinName = MUZIKBROWZER;
	m_vBorderWidth = 5;
	m_vBorderHorz = 5;
	m_vBorderVert = 5;
	m_vGenreWidthPct = 15;

    sscanf(deffont, CCFONTFMT, &m_lfTitles.lfHeight, &m_lfTitles.lfWidth, 
		&m_lfTitles.lfEscapement, &m_lfTitles.lfOrientation,
        &m_lfTitles.lfWeight, &italic, &underline, &strikeout, &charset,
        &outprecision, &clipprecision, &quality, &pitchandfamily);
    m_lfTitles.lfItalic = italic; m_lfTitles.lfUnderline = underline;
    m_lfTitles.lfStrikeOut = strikeout; m_lfTitles.lfCharSet = charset;
    m_lfTitles.lfOutPrecision = outprecision;m_lfTitles.lfClipPrecision = clipprecision;
    m_lfTitles.lfQuality = quality;m_lfTitles.lfPitchAndFamily = pitchandfamily;

    strcpy(m_lfTitles.lfFaceName, deffontfacename);
    memcpy(&m_samplelfTitles, &m_lfTitles, sizeof(LOGFONT));
    m_lplfTitles = &m_lfTitles;

    sscanf(deffont, CCFONTFMT, &m_lfPanel.lfHeight, &m_lfPanel.lfWidth,
		&m_lfPanel.lfEscapement, &m_lfPanel.lfOrientation,
		&m_lfPanel.lfWeight,&italic, &underline, &strikeout, &charset,
        &outprecision, &clipprecision,&quality, &pitchandfamily);
    m_lfPanel.lfItalic = italic; m_lfPanel.lfUnderline = underline;
    m_lfPanel.lfStrikeOut = strikeout;m_lfPanel.lfCharSet = charset;
    m_lfPanel.lfOutPrecision = outprecision;m_lfPanel.lfClipPrecision = clipprecision;
    m_lfPanel.lfQuality = quality;m_lfPanel.lfPitchAndFamily = pitchandfamily;

    strcpy(m_lfPanel.lfFaceName, deffontfacename);
    memcpy(&m_samplelfPanel, &m_lfPanel, sizeof(LOGFONT));
    m_lplfPanel = &m_lfPanel;

    sscanf(deffont, CCFONTFMT, &m_lfColHdr.lfHeight, &m_lfColHdr.lfWidth, 
		&m_lfColHdr.lfEscapement,&m_lfColHdr.lfOrientation,
        &m_lfColHdr.lfWeight,&italic,&underline,&strikeout,&charset,
        &outprecision,&clipprecision,&quality,&pitchandfamily);
    m_lfColHdr.lfItalic = italic;m_lfColHdr.lfUnderline = underline;
    m_lfColHdr.lfStrikeOut = strikeout;m_lfColHdr.lfCharSet = charset;
    m_lfColHdr.lfOutPrecision = outprecision;m_lfColHdr.lfClipPrecision = clipprecision;
    m_lfColHdr.lfQuality = quality;m_lfColHdr.lfPitchAndFamily = pitchandfamily;

    strcpy(m_lfColHdr.lfFaceName, deffontfacename);
    memcpy(&m_samplelfColHdr, &m_lfColHdr, sizeof(LOGFONT));
    m_lplfColHdr = &m_lfColHdr;

    sscanf(deffont, CCFONTFMT, &m_lfCurPlay.lfHeight, &m_lfCurPlay.lfWidth, 
		&m_lfCurPlay.lfEscapement,&m_lfCurPlay.lfOrientation,
        &m_lfCurPlay.lfWeight,&italic,&underline,&strikeout,&charset,
        &outprecision,&clipprecision,&quality,&pitchandfamily);
    m_lfCurPlay.lfItalic = italic;m_lfCurPlay.lfUnderline = underline;
    m_lfCurPlay.lfStrikeOut = strikeout;m_lfCurPlay.lfCharSet = charset;
    m_lfCurPlay.lfOutPrecision = outprecision;m_lfCurPlay.lfClipPrecision = clipprecision;
    m_lfCurPlay.lfQuality = quality;m_lfCurPlay.lfPitchAndFamily = pitchandfamily;

    strcpy(m_lfCurPlay.lfFaceName, deffontfacename);
    memcpy(&m_samplelfCurPlay, &m_lfCurPlay, sizeof(LOGFONT));
    m_lplfCurPlay = &m_lfCurPlay;



}
void
CConfigDisplay::ReadReg(RegistryKey & reg, BOOL readskin) {
	AutoLog alog("CCD::ReadReg");

    m_vBkPanel = reg.Read(RegWindowsColorBkPanel, m_vBkPanel);
    m_vBkNormal = reg.Read(RegWindowsColorBkNormal, m_vBkNormal);
    m_vBkHigh = reg.Read(RegWindowsColorBkHigh, m_vBkHigh);
    m_vBkSel = reg.Read(RegWindowsColorBkSel, m_vBkSel);
    m_vTxPanel = reg.Read(RegWindowsColorTxPanel, m_vTxPanel);
    m_vTxNormal = reg.Read(RegWindowsColorTxNormal, m_vTxNormal);
    m_vTxHigh = reg.Read(RegWindowsColorTxHigh, m_vTxHigh);
    m_vTxSel = reg.Read(RegWindowsColorTxSel, m_vTxSel);
	m_vBkColHdr = reg.Read(RegWindowsColorBkColHdr, m_vBkColHdr);
	m_vTxColHdr = reg.Read(RegWindowsColorTxColHdr, m_vTxColHdr);
	m_vBkCurPlay = reg.Read(RegWindowsColorBkCurPlay, m_vBkCurPlay);
	m_vTxCurPlay = reg.Read(RegWindowsColorTxCurPlay, m_vTxCurPlay);
    v2m();

	m_vBorderWidth = reg.Read(RegWindowsBorderWidth, m_vBorderWidth);
	m_vGenreWidthPct = reg.Read(RegWindowsGenreWidthPct, m_vGenreWidthPct);
	m_vBorderHorz = reg.Read(RegWindowsBorderHorz, m_vBorderHorz);
	m_vBorderVert = reg.Read(RegWindowsBorderVert, m_vBorderVert);

	int sel;
	if (IsWindow(m_BorderWidth.m_hWnd)) {
		sel = m_BorderWidth.SelectString(-1, numToString(m_vBorderWidth));
		m_BorderWidth.SetCurSel(sel);
	}

	if (MB_GENRE_WIDTH_PCT_MIN <= m_vGenreWidthPct
		&& m_vGenreWidthPct <= MB_GENRE_WIDTH_PCT_MAX) {
		if (IsWindow(m_GenreSpin.m_hWnd))
			m_GenreSpin.SetPos(m_vGenreWidthPct);
	} else {
		if (IsWindow(m_GenreSpin.m_hWnd))
			m_GenreSpin.SetPos(MB_GENRE_WIDTH_PCT_DFLT);
		m_vGenreWidthPct = MB_GENRE_WIDTH_PCT_DFLT;
	}

    AutoBuf buf(1000);
	
	if (readskin) {
		reg.Read(RegWindowsSkinName, buf.p, 999,m_sSkinName.GetBuffer(0));
		m_sSkinName = buf.p;

		if (IsWindow(m_SkinList.m_hWnd)) {
			int sel = m_SkinList.SelectString(-1, m_sSkinName);
			if (sel > -1) {
				m_SkinList.SetCurSel(sel);
			}
		}
	}

    reg.Read(RegWindowsFontTitles, buf.p, 999, "");
    if (strlen(buf.p) > CCFONTFACEPOS) {
        sscanf(buf.p, CCFONTFMT, 
            &m_lfTitles.lfHeight, &m_lfTitles.lfWidth, &m_lfTitles.lfEscapement,
            &m_lfTitles.lfOrientation,
            &m_lfTitles.lfWeight,
                    &italic,
            &underline,
            &strikeout,
            &charset,
            &outprecision,
            &clipprecision,
            &quality,
            &pitchandfamily);
        m_lfTitles.lfItalic = italic;
        m_lfTitles.lfUnderline = underline;
        m_lfTitles.lfStrikeOut = strikeout;
        m_lfTitles.lfCharSet = charset;
        m_lfTitles.lfOutPrecision = outprecision;
        m_lfTitles.lfClipPrecision = clipprecision;
        m_lfTitles.lfQuality = quality;
        m_lfTitles.lfPitchAndFamily = pitchandfamily;
        strcpy(m_lfTitles.lfFaceName, (buf.p + CCFONTFACEPOS));
    }

    reg.Read(RegWindowsFontPanel, buf.p, 999, "");
    if (strlen(buf.p) > CCFONTFACEPOS) {
        sscanf(buf.p, CCFONTFMT, 
            &m_lfPanel.lfHeight, &m_lfPanel.lfWidth, &m_lfPanel.lfEscapement,
            &m_lfPanel.lfOrientation,
            &m_lfPanel.lfWeight,
                    &italic,
            &underline,
            &strikeout,
            &charset,
            &outprecision,
            &clipprecision,
            &quality,
            &pitchandfamily);
        m_lfPanel.lfItalic = italic;
        m_lfPanel.lfUnderline = underline;
        m_lfPanel.lfStrikeOut = strikeout;
        m_lfPanel.lfCharSet = charset;
        m_lfPanel.lfOutPrecision = outprecision;
        m_lfPanel.lfClipPrecision = clipprecision;
        m_lfPanel.lfQuality = quality;
        m_lfPanel.lfPitchAndFamily = pitchandfamily;
        strcpy(m_lfPanel.lfFaceName, (buf.p + CCFONTFACEPOS));
    }

    reg.Read(RegWindowsFontColHdr, buf.p, 999, "");
    if (strlen(buf.p) > CCFONTFACEPOS) {
        sscanf(buf.p, CCFONTFMT, 
            &m_lfColHdr.lfHeight, &m_lfColHdr.lfWidth, &m_lfColHdr.lfEscapement,
            &m_lfColHdr.lfOrientation,
            &m_lfColHdr.lfWeight,
                    &italic,
            &underline,
            &strikeout,
            &charset,
            &outprecision,
            &clipprecision,
            &quality,
            &pitchandfamily);
        m_lfColHdr.lfItalic = italic;
        m_lfColHdr.lfUnderline = underline;
        m_lfColHdr.lfStrikeOut = strikeout;
        m_lfColHdr.lfCharSet = charset;
        m_lfColHdr.lfOutPrecision = outprecision;
        m_lfColHdr.lfClipPrecision = clipprecision;
        m_lfColHdr.lfQuality = quality;
        m_lfColHdr.lfPitchAndFamily = pitchandfamily;
        strcpy(m_lfColHdr.lfFaceName, (buf.p + CCFONTFACEPOS));
    }

    reg.Read(RegWindowsFontCurPlay, buf.p, 999, "");
    if (strlen(buf.p) > CCFONTFACEPOS) {
        sscanf(buf.p, CCFONTFMT, 
            &m_lfCurPlay.lfHeight, &m_lfCurPlay.lfWidth, &m_lfCurPlay.lfEscapement,
            &m_lfCurPlay.lfOrientation,
            &m_lfCurPlay.lfWeight,
                    &italic,
            &underline,
            &strikeout,
            &charset,
            &outprecision,
            &clipprecision,
            &quality,
            &pitchandfamily);
        m_lfCurPlay.lfItalic = italic;
        m_lfCurPlay.lfUnderline = underline;
        m_lfCurPlay.lfStrikeOut = strikeout;
        m_lfCurPlay.lfCharSet = charset;
        m_lfCurPlay.lfOutPrecision = outprecision;
        m_lfCurPlay.lfClipPrecision = clipprecision;
        m_lfCurPlay.lfQuality = quality;
        m_lfCurPlay.lfPitchAndFamily = pitchandfamily;
        strcpy(m_lfCurPlay.lfFaceName, (buf.p + CCFONTFACEPOS));
    }
}
void
CConfigDisplay::StoreReg(RegistryKey & reg, BOOL storeskin) {
	AutoLog alog("CCD::StoreREg");

    reg.Write(RegWindowsColorBkPanel, m_vBkPanel);
    reg.Write(RegWindowsColorBkNormal, m_vBkNormal);
    reg.Write(RegWindowsColorBkHigh, m_vBkHigh);
    reg.Write(RegWindowsColorBkSel, m_vBkSel);
    reg.Write(RegWindowsColorTxPanel, m_vTxPanel);
    reg.Write(RegWindowsColorTxNormal, m_vTxNormal);
    reg.Write(RegWindowsColorTxHigh, m_vTxHigh);
    reg.Write(RegWindowsColorTxSel, m_vTxSel);
    reg.Write(RegWindowsColorBkColHdr, m_vBkColHdr);
    reg.Write(RegWindowsColorTxColHdr, m_vTxColHdr);
    reg.Write(RegWindowsColorBkCurPlay, m_vBkCurPlay);
    reg.Write(RegWindowsColorTxCurPlay, m_vTxCurPlay);

	int sel = m_SkinList.GetCurSel();
	if (sel > -1) {
		m_SkinList.GetLBText(sel, m_sSkinName);
	} else {
		m_SkinList.GetWindowText(m_sSkinName);
	}
	if (storeskin) {
		reg.Write(RegWindowsSkinName, m_sSkinName);
	}
	
	sel = m_BorderWidth.GetCurSel();
	if (sel > -1) {
		CString tmp;
		m_BorderWidth.GetLBText(sel, tmp);
		m_vBorderWidth = atoi(tmp);
	} else {
		CString tmp;
		m_BorderWidth.GetWindowText(tmp);
		m_vBorderWidth = atoi(tmp);
	}
	reg.Write(RegWindowsBorderWidth, m_vBorderWidth);
	reg.Write(RegWindowsBorderHorz, m_vBorderHorz);
	reg.Write(RegWindowsBorderVert, m_vBorderVert);
	m_vGenreWidthPct = m_GenreSpin.GetPos();
	reg.Write(RegWindowsGenreWidthPct, m_vGenreWidthPct);

    AutoBuf buf(1000);

    sprintf(buf.p, CCFONTFMT,
        m_lfTitles.lfHeight, m_lfTitles.lfWidth, m_lfTitles.lfEscapement, m_lfTitles.lfOrientation,
        m_lfTitles.lfWeight, m_lfTitles.lfItalic, m_lfTitles.lfUnderline, m_lfTitles.lfStrikeOut,
        m_lfTitles.lfCharSet, m_lfTitles.lfOutPrecision, m_lfTitles.lfClipPrecision,
        m_lfTitles.lfQuality, m_lfTitles.lfPitchAndFamily);
    char * p = buf.p + strlen(buf.p);
    sprintf(p, "%s", m_lfTitles.lfFaceName);
    reg.Write(RegWindowsFontTitles, buf.p);

    sprintf(buf.p, CCFONTFMT,
        m_lfPanel.lfHeight, m_lfPanel.lfWidth, m_lfPanel.lfEscapement, m_lfPanel.lfOrientation,
        m_lfPanel.lfWeight, m_lfPanel.lfItalic, m_lfPanel.lfUnderline, m_lfPanel.lfStrikeOut,
        m_lfPanel.lfCharSet, m_lfPanel.lfOutPrecision, m_lfPanel.lfClipPrecision,
        m_lfPanel.lfQuality, m_lfPanel.lfPitchAndFamily);
    p = buf.p + strlen(buf.p);
    sprintf(p, "%s", m_lfPanel.lfFaceName);
    reg.Write(RegWindowsFontPanel, buf.p);

    sprintf(buf.p, CCFONTFMT,
        m_lfColHdr.lfHeight, m_lfColHdr.lfWidth, m_lfColHdr.lfEscapement, m_lfColHdr.lfOrientation,
        m_lfColHdr.lfWeight, m_lfColHdr.lfItalic, m_lfColHdr.lfUnderline, m_lfColHdr.lfStrikeOut,
        m_lfColHdr.lfCharSet, m_lfColHdr.lfOutPrecision, m_lfColHdr.lfClipPrecision,
        m_lfColHdr.lfQuality, m_lfColHdr.lfPitchAndFamily);
    p = buf.p + strlen(buf.p);
    sprintf(p, "%s", m_lfColHdr.lfFaceName);
    reg.Write(RegWindowsFontColHdr, buf.p);

    sprintf(buf.p, CCFONTFMT,
        m_lfCurPlay.lfHeight, m_lfCurPlay.lfWidth, m_lfCurPlay.lfEscapement, m_lfCurPlay.lfOrientation,
        m_lfCurPlay.lfWeight, m_lfCurPlay.lfItalic, m_lfCurPlay.lfUnderline, m_lfCurPlay.lfStrikeOut,
        m_lfCurPlay.lfCharSet, m_lfCurPlay.lfOutPrecision, m_lfCurPlay.lfClipPrecision,
        m_lfCurPlay.lfQuality, m_lfCurPlay.lfPitchAndFamily);
    p = buf.p + strlen(buf.p);
    sprintf(p, "%s", m_lfCurPlay.lfFaceName);
    reg.Write(RegWindowsFontCurPlay, buf.p);
}


void CConfigDisplay::v2m() {
    m_BkPanel.currentcolor = m_vBkPanel;
    m_BkNormal.currentcolor = m_vBkNormal;
    m_BkHigh.currentcolor = m_vBkHigh;
    m_BkSel.currentcolor = m_vBkSel;
    m_TxPanel.currentcolor = m_vTxPanel;
    m_TxNormal.currentcolor = m_vTxNormal;
    m_TxHigh.currentcolor = m_vTxHigh;
    m_TxSel.currentcolor = m_vTxSel;
	m_BkColHdr.currentcolor = m_vBkColHdr;
	m_TxColHdr.currentcolor = m_vTxColHdr;
	m_BkCurPlay.currentcolor = m_vBkCurPlay;
	m_TxCurPlay.currentcolor = m_vTxCurPlay;
}
void CConfigDisplay::m2v() {
    m_vBkPanel = m_BkPanel.currentcolor;
    m_vBkNormal = m_BkNormal.currentcolor;
    m_vBkHigh = m_BkHigh.currentcolor;
    m_vBkSel = m_BkSel.currentcolor;
    m_vTxPanel = m_TxPanel.currentcolor;
    m_vTxNormal = m_TxNormal.currentcolor;
    m_vTxHigh = m_TxHigh.currentcolor;
    m_vTxSel = m_TxSel.currentcolor;
	m_vBkColHdr = m_BkColHdr.currentcolor;
	m_vTxColHdr = m_TxColHdr.currentcolor;
	m_vBkCurPlay = m_BkCurPlay.currentcolor;
	m_vTxCurPlay = m_TxCurPlay.currentcolor;
}

void CConfigDisplay::initFontSels() {
	int r = m_FontTitles.SelectString(-1,m_lfTitles.lfFaceName);
	r = m_FontPanel.SelectString(-1,m_lfPanel.lfFaceName);
	m_FontColHdr.SelectString(-1,m_lfColHdr.lfFaceName);
	m_FontCurPlay.SelectString(-1,m_lfCurPlay.lfFaceName);

	AutoBuf buf(5);
	sprintf(buf.p, "%d", abs(m_lfTitles.lfHeight));
	m_SizeTitles.SelectString(0, buf.p);
	sprintf(buf.p, "%d", abs(m_lfPanel.lfHeight));
	m_SizePanel.SelectString(0, buf.p);
	sprintf(buf.p, "%d", abs(m_lfColHdr.lfHeight));
	m_SizeColHdr.SelectString(0, buf.p);
	sprintf(buf.p, "%d", abs(m_lfCurPlay.lfHeight));
	m_SizeCurPlay.SelectString(0, buf.p);

	if (m_lfTitles.lfWeight >= 700) {
		m_BoldTitles.SetCheck(1);
	} else {
		m_BoldTitles.SetCheck(0);
	}
	if (m_lfPanel.lfWeight >= 700) {
		m_BoldPanel.SetCheck(1);
	} else {
		m_BoldPanel.SetCheck(0);
	}
	if (m_lfColHdr.lfWeight >= 700) {
		m_BoldColHdr.SetCheck(1);
	} else {
		m_BoldColHdr.SetCheck(0);
	}
	if (m_lfCurPlay.lfWeight >= 700) {
		m_BoldCurPlay.SetCheck(1);
	} else {
		m_BoldCurPlay.SetCheck(0);
	}

}


void CConfigDisplay::OnSetDefault() 
{
	AutoLog alog("CCD::OnSetDefault");
    setDefaults();	
    UpdateData(FALSE);
	initFontSels();
	showSample();
	//modified(TRUE);
    RedrawWindow();
}

void
CConfigDisplay::copy2lf(LOGFONT & lfT, LOGFONT & lfP, LOGFONT & lfC, 
						LOGFONT & lfCP) {
    CString msg;

// titles
    m_FontSelTitles = m_FontTitles.GetCurSel();
    if (m_FontSelTitles > -1) {
        m_FontTitles.GetLBText(m_FontSelTitles, msg);
        strcpy(lfT.lfFaceName, (LPCTSTR)msg);

        int c = m_BoldTitles.GetCheck();
        if (c == 0) {
            lfT.lfWeight = 400;
        } else 
            lfT.lfWeight = 700;
    }
    m_SizeSelTitles = m_SizeTitles.GetCurSel();
    if (m_SizeSelTitles > -1) {
        m_SizeTitles.GetLBText(m_SizeSelTitles, msg);
        LONG h;
        sscanf((LPCTSTR)msg, "%d", &h);
        h = -h;
        lfT.lfHeight = h;
    } else {
        m_SizeTitles.GetWindowText( msg);
        LONG h;
        sscanf((LPCTSTR)msg, "%d", &h);
        h = -h;
        lfT.lfHeight = h;
	}

// panel
    m_FontSelPanel = m_FontPanel.GetCurSel();
    if (m_FontSelPanel > -1) {
        m_FontPanel.GetLBText(m_FontSelPanel, msg);
        strcpy(lfP.lfFaceName, (LPCTSTR)msg);

        int c = m_BoldPanel.GetCheck();
        if (c == 0) {
            lfP.lfWeight = 400;
        } else 
            lfP.lfWeight = 700;
    }
    m_SizeSelPanel = m_SizePanel.GetCurSel();
    if (m_SizeSelPanel > -1) {
        m_SizePanel.GetLBText(m_SizeSelPanel, msg);
        LONG h;
        sscanf((LPCTSTR)msg, "%d", &h);
        h = -h;
        lfP.lfHeight = h;
    } else {
        m_SizePanel.GetWindowText( msg);
        LONG h;
        sscanf((LPCTSTR)msg, "%d", &h);
        h = -h;
        lfP.lfHeight = h;
	}

// col hdrs
    m_FontSelColHdr = m_FontColHdr.GetCurSel();
    if (m_FontSelColHdr > -1) {
        m_FontColHdr.GetLBText(m_FontSelColHdr, msg);
        strcpy(lfC.lfFaceName, (LPCTSTR)msg);

        int c = m_BoldColHdr.GetCheck();
        if (c == 0) {
            lfC.lfWeight = 400;
        } else 
            lfC.lfWeight = 700;
    }
    m_SizeSelColHdr = m_SizeColHdr.GetCurSel();
    if (m_SizeSelColHdr > -1) {
        m_SizeColHdr.GetLBText(m_SizeSelColHdr, msg);
        LONG h;
        sscanf((LPCTSTR)msg, "%d", &h);
        h = -h;
        lfC.lfHeight = h;
    } else {
        m_SizeColHdr.GetWindowText( msg);
        LONG h;
        sscanf((LPCTSTR)msg, "%d", &h);
        h = -h;
        lfC.lfHeight = h;
	}
// cur play
    m_FontSelCurPlay = m_FontCurPlay.GetCurSel();
    if (m_FontSelCurPlay > -1) {
        m_FontCurPlay.GetLBText(m_FontSelCurPlay, msg);
        strcpy(lfCP.lfFaceName, (LPCTSTR)msg);

        int c = m_BoldCurPlay.GetCheck();
        if (c == 0) {
            lfCP.lfWeight = 400;
        } else 
            lfCP.lfWeight = 700;
    }
    m_SizeSelCurPlay = m_SizeCurPlay.GetCurSel();
    if (m_SizeSelCurPlay > -1) {
        m_SizeCurPlay.GetLBText(m_SizeSelCurPlay, msg);
        LONG h;
        sscanf((LPCTSTR)msg, "%d", &h);
        h = -h;
        lfCP.lfHeight = h;
    } else {
        m_SizeCurPlay.GetWindowText( msg);
        LONG h;
        sscanf((LPCTSTR)msg, "%d", &h);
        h = -h;
        lfCP.lfHeight = h;
	}
}

void
CConfigDisplay::showSample() {
	UpdateData(TRUE);
	m2v();
	copy2lf(m_samplelfTitles,m_samplelfPanel,m_samplelfColHdr,m_samplelfCurPlay);

	m_FontSampleTitles.DeleteObject();
	m_FontSampleTitles.CreateFontIndirect(&m_samplelfTitles);
	m_SampleTitles.SetFont(&m_FontSampleTitles, TRUE);
	m_SampleTitles.changeFont(&m_FontSampleTitles);
	m_SampleTitles.setText("Big Fish Howlers");
	m_SampleTitles.SetTextColor(m_vTxNormal);
	m_SampleTitles.SetBkColor(m_vBkNormal);

	m_FontSamplePanel.DeleteObject();
	m_FontSamplePanel.CreateFontIndirect(&m_samplelfPanel);
	m_SamplePanel.SetFont(&m_FontSamplePanel, TRUE);
	m_SamplePanel.changeFont(&m_FontSamplePanel);
	m_SamplePanel.setText("Press Menu for options");
	m_SamplePanel.SetTextColor(m_vTxPanel);
	m_SamplePanel.SetBkColor(m_vBkPanel);

	m_FontSampleColHdr.DeleteObject();
	m_FontSampleColHdr.CreateFontIndirect(&m_samplelfColHdr);
	m_SampleColHdr.SetFont(&m_FontSampleColHdr, TRUE);
	m_SampleColHdr.changeFont(&m_FontSampleColHdr);
	m_SampleColHdr.setText("Artists");
	m_SampleColHdr.SetTextColor(m_vTxColHdr);
	m_SampleColHdr.SetBkColor(m_vBkColHdr);

	m_FontSampleSel.DeleteObject();
	m_FontSampleSel.CreateFontIndirect(&m_samplelfTitles);
	m_SampleSel.SetFont(&m_FontSampleSel, TRUE);
	m_SampleSel.changeFont(&m_FontSampleSel);
	m_SampleSel.setText("Big Fish Howlers");
	m_SampleSel.SetTextColor(m_vTxSel);
	m_SampleSel.SetBkColor(m_vBkSel);

	m_FontSampleHigh.DeleteObject();
	m_FontSampleHigh.CreateFontIndirect(&m_samplelfTitles);
	m_SampleHigh.SetFont(&m_FontSampleHigh, TRUE);
	m_SampleHigh.changeFont(&m_FontSampleHigh);
	m_SampleHigh.setText("Big Fish Howlers");
	m_SampleHigh.SetTextColor(m_vTxHigh);
	m_SampleHigh.SetBkColor(m_vBkHigh);

	m_FontSampleCurPlay.DeleteObject();
	m_FontSampleCurPlay.CreateFontIndirect(&m_samplelfCurPlay);
	m_SampleCurPlay.SetFont(&m_FontSampleCurPlay, TRUE);
	m_SampleCurPlay.changeFont(&m_FontSampleCurPlay);
	m_SampleCurPlay.setText("Got Fish? by Big Fish Howlers");
	m_SampleCurPlay.SetTextColor(m_vTxCurPlay);
	m_SampleCurPlay.SetBkColor(m_vBkCurPlay);


}

void CConfigDisplay::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	showSample();
	
	// TODO: Add your message handler code here
	
	// Do not call CPropertyPage::OnPaint() for painting messages
}


void CConfigDisplay::OnCButtonMessage(WPARAM w, LPARAM l) {
	modified(TRUE);
	showSample();
	Invalidate();
}
void CConfigDisplay::onbold() {
	modified(TRUE);
	showSample();
}
void CConfigDisplay::OnOK() 
{
    m2v();
    copy2lf(m_lfTitles, m_lfPanel, m_lfColHdr,m_lfCurPlay);

	int sel = m_SkinList.GetCurSel();
	if (sel > -1) {
		m_SkinList.GetLBText(sel, m_sSkinName);
	} else {
		m_SkinList.GetWindowText(m_sSkinName);
	}

	sel = m_BorderWidth.GetCurSel();
	if (sel > -1) {
		CString tmp;
		m_BorderWidth.GetLBText(sel, tmp);
		m_vBorderWidth = atoi(tmp);
	}

	StoreReg(RegistryKey( HKEY_LOCAL_MACHINE, RegKey ),TRUE);

	CString skindefcustom = getSkin(MB_SKIN_DEF_CUSTOM, FALSE);
	RegistryKey regSDCustom(skindefcustom);
	StoreReg(regSDCustom);
	regSDCustom.WriteFile();

	if (m_Modified) {
		(*m_playercallbacks->redraw)();
	}
	m_Modified = FALSE;
	modified(FALSE);

	CPropertyPage::OnOK();
}

BOOL CConfigDisplay::OnApply() 
{
	return CPropertyPage::OnApply();
}
void CConfigDisplay::OnCancel() 
{
	if (m_Modified) {
		ReadReg(RegistryKey( HKEY_LOCAL_MACHINE, RegKeyPrevVals ),TRUE);
		OnSkinChoose();
		OnOK();
	}
	
	CPropertyPage::OnCancel();
}
BOOL CConfigDisplay::verifySkin(CString skin) {
	AutoLog alog("CCD::verifySkin");
	CStringList bmps;
	bmps.AddTail(MB_SKIN_DEF);
	bmps.AddTail(MB_SKIN_ALBUMART);
	bmps.AddTail(MB_SKIN_BACKGROUNDMAIN);
	bmps.AddTail(MB_SKIN_BUTTONAPPLABELOUT);
	bmps.AddTail(MB_SKIN_BUTTONBACKGROUND);
	bmps.AddTail(MB_SKIN_BUTTONCLEARHOVER);
	bmps.AddTail(MB_SKIN_BUTTONCLEARIN);
	bmps.AddTail(MB_SKIN_BUTTONCLEAROUT);
	bmps.AddTail(MB_SKIN_BUTTONEXITHOVER);
	bmps.AddTail(MB_SKIN_BUTTONEXITIN);
	bmps.AddTail(MB_SKIN_BUTTONEXITOUT);
	bmps.AddTail(MB_SKIN_BUTTONFASTFORWARDHOVER);
	bmps.AddTail(MB_SKIN_BUTTONFASTFORWARDIN);
	bmps.AddTail(MB_SKIN_BUTTONFASTFORWARDOUT);
	bmps.AddTail(MB_SKIN_BUTTONLOADHOVER);
	bmps.AddTail(MB_SKIN_BUTTONLOADIN);
	bmps.AddTail(MB_SKIN_BUTTONLOADOUT);
	bmps.AddTail(MB_SKIN_BUTTONMAXIMIZEHOVER);
	bmps.AddTail(MB_SKIN_BUTTONMAXIMIZEIN);
	bmps.AddTail(MB_SKIN_BUTTONMAXIMIZEOUT);
	bmps.AddTail(MB_SKIN_BUTTONMINIMIZEHOVER);
	bmps.AddTail(MB_SKIN_BUTTONMINIMIZEIN);
	bmps.AddTail(MB_SKIN_BUTTONMINIMIZEOUT);
	bmps.AddTail(MB_SKIN_BUTTONMENUHOVER);
	bmps.AddTail(MB_SKIN_BUTTONMENUIN);
	bmps.AddTail(MB_SKIN_BUTTONMENUOUT);
	bmps.AddTail(MB_SKIN_BUTTONPAUSEOUT);
	bmps.AddTail(MB_SKIN_BUTTONPLAYHOVER);
	bmps.AddTail(MB_SKIN_BUTTONPLAYIN);
	bmps.AddTail(MB_SKIN_BUTTONPLAYOUT);
	bmps.AddTail(MB_SKIN_BUTTONPROGRESSBACKGROUND);
	bmps.AddTail(MB_SKIN_BUTTONPROGRESSKNOB);
	bmps.AddTail(MB_SKIN_BUTTONRANDOMHOVER);
	bmps.AddTail(MB_SKIN_BUTTONRANDOMIN);
	bmps.AddTail(MB_SKIN_BUTTONRANDOMOUT);
	bmps.AddTail(MB_SKIN_BUTTONRESIZEHOVER);
	bmps.AddTail(MB_SKIN_BUTTONRESIZEIN);
	bmps.AddTail(MB_SKIN_BUTTONRESIZEOUT);
	bmps.AddTail(MB_SKIN_BUTTONRESTOREHOVER);
	bmps.AddTail(MB_SKIN_BUTTONRESTOREIN);
	bmps.AddTail(MB_SKIN_BUTTONRESTOREOUT);
	bmps.AddTail(MB_SKIN_BUTTONREVERSEHOVER);
	bmps.AddTail(MB_SKIN_BUTTONREVERSEIN);
	bmps.AddTail(MB_SKIN_BUTTONREVERSEOUT);
	bmps.AddTail(MB_SKIN_BUTTONSAVEHOVER);
	bmps.AddTail(MB_SKIN_BUTTONSAVEIN);
	bmps.AddTail(MB_SKIN_BUTTONSAVEOUT);
	bmps.AddTail(MB_SKIN_BUTTONSHUFFLEHOVER);
	bmps.AddTail(MB_SKIN_BUTTONSHUFFLEIN);
	bmps.AddTail(MB_SKIN_BUTTONSHUFFLEOUT);
	bmps.AddTail(MB_SKIN_BUTTONSTOPHOVER);
	bmps.AddTail(MB_SKIN_BUTTONSTOPIN);
	bmps.AddTail(MB_SKIN_BUTTONSTOPOUT);
	bmps.AddTail(MB_SKIN_BUTTONVOLUMEBACKGROUND);
	bmps.AddTail(MB_SKIN_BUTTONVOLUMEKNOB);
	bmps.AddTail(MB_SKIN_SCROLLBACKGROUND);
	bmps.AddTail(MB_SKIN_SCROLLBUTTON);
	bmps.AddTail(MB_SKIN_SCROLLDOWNARROW);
	bmps.AddTail(MB_SKIN_SCROLLUPARROW);

	POSITION pos;
	CString name,msg,bmp;
	for(pos = bmps.GetHeadPosition(); pos != NULL; ) {
		name = bmps.GetAt(pos);
		bmp = getSkin(skin, name);
		if (bmp == "") {
			msg += name;
			msg += "\r\n";
		}
		bmps.GetNext(pos);
	}
	if (msg != "") {
		CString tmp = "The following bitmaps are missing from the\r\n";
		tmp += skin;
		tmp += " skin.\r\n\r\n";
		tmp += msg;
		//MBMessageBox("Corrupted Skin", tmp, TRUE, FALSE);
		logger.log(tmp);
		return FALSE;
	}

	return TRUE;
}
void CConfigDisplay::OnSkinChoose() 
{
	CString sSkin(m_SkinDir);
	sSkin += "\\";
	CString tmp;
	int sel = m_SkinList.GetCurSel();
	if (sel > -1) {
		m_SkinList.GetLBText(sel, tmp);
	}
	if (OnSkinChoose(tmp)) {
		if (IsWindow(m_SkinList.m_hWnd)) {
			int sel = m_SkinList.SelectString(-1, m_sSkinName);
			if (sel > -1) {
				m_SkinList.SetCurSel(sel);
			}
		}
		UpdateData(FALSE);
		initFontSels();
		showSample();
		modified(TRUE);
		RedrawWindow();
	}
}
BOOL CConfigDisplay::OnSkinChoose(CString skin) 
{
	if (verifySkin(skin)) {
		m_sSkinName = skin;
	} else 
		return FALSE;

	m_sSkinName = skin;

		// Read skin def
	CString skindef = getSkin(MB_SKIN_DEF);
	RegistryKey regSD(skindef);
	regSD.ReadFile();

	// read skin def custom
	CString save = m_sSkinName;
	CString skindefcustom = getSkin(MB_SKIN_DEF_CUSTOM);
	RegistryKey regSDCustom(skindefcustom);
	regSDCustom.ReadFile();

	// overwrite custom on skin def
	regSD.Copy(regSDCustom);
	ReadReg(regSD);
	m_sSkinName = save;

	RegistryKey reg( HKEY_LOCAL_MACHINE, RegKey );
	reg.Write(RegWindowsSkinName, m_sSkinName);
	return TRUE;

//	initFontSels();

}




const CString CConfigDisplay::getSkin(const CString skinname, 
									  const CString key) {
	CString saveit = m_sSkinName;
	m_sSkinName = skinname;
	CString skin = getSkin(key);
	m_sSkinName = saveit;
	return skin;
}
const CString CConfigDisplay::getSkin(const CString key, BOOL readcheck) {
	CString glob(m_SkinDir);
	glob += "\\";
	glob += m_sSkinName;
	glob += "\\";
	glob += key;

	if (!readcheck || FileUtil::IsReadable(glob)) {
		return glob;
	}
//	// See issue 299
//	if (MB_SKIN_ALBUMART == key) {
//		CString skey(key);
//		if (FileUtil::IsReadable(skey)) {
//			return key;
//		}
//	}
	return CString ("");
}

void CConfigDisplay::OnSkinDelete()				{modified(TRUE);}
void CConfigDisplay::OnEditchangeSkinList()		{modified(TRUE);}
void CConfigDisplay::OnUpdateWidth()			{modified(TRUE);}
void CConfigDisplay::OnSelendokSkinList()		{modified(TRUE);}
void CConfigDisplay::OnColorButton()			{
	showSample();
	modified(TRUE);
}
void CConfigDisplay::OnSelchangeFont() {	showSample();	modified(TRUE);}


double CConfigDisplay::getGenreWidthPct() {
	int nTmp = m_vGenreWidthPct;
	nTmp = __min(nTmp, MB_GENRE_WIDTH_PCT_MAX);
	nTmp = __max(nTmp, MB_GENRE_WIDTH_PCT_MIN);
	double phpct = (double) nTmp / 100;
	return phpct;
}

void CConfigDisplay::modified(BOOL b) {
	SetModified(b);
	m_Modified = b;
	logger.ods(CString("cd modded ") + numToString(b));
}
void CConfigDisplay::getSkins(CStringList & skinlist) {
	AutoLog alog("CCD::getSkins");
    CString glob(m_SkinDir);
    glob += "\\";
    glob += "*";
    CFileFind finder;
    BOOL bWorking = finder.FindFile(glob);
    while (bWorking)
    {
        bWorking = finder.FindNextFile();
        CString fname = finder.GetFileName();
		CString skin = fname;
		if (skin != MUZIKBROWZER && skin != "." && skin != "..") {
			// verifying here delays the menu from popping up
			if (1 /* verifySkin(skin)*/)
				skinlist.AddTail(skin);
		}
    }
	finder.Close();
}

void CConfigDisplay::readSkins() {
	AutoLog alog("CCD::readSkins");
	m_SkinList.ResetContent();
	//m_SkinList.AddString(MUZIKBROWZER);
    CString glob(m_SkinDir);
    glob += "\\";
    glob += "*";
    CFileFind finder;
    BOOL bWorking = finder.FindFile(glob);
    while (bWorking)
    {
        bWorking = finder.FindNextFile();
        CString fname = finder.GetFileName();
		CString skin = fname;
		if (skin != MUZIKBROWZER && skin != "." && skin != "..") {
			// verifying here delays the menu from popping up
			if (1 /*verifySkin(skin)*/)
				m_SkinList.AddString(skin);
		}
    }
	finder.Close();
}