// ConfigFonts.cpp : implementation file
//

#include "stdafx.h"
#include "irman_registry.h"
#include "ConfigFonts.h"
#include "MyString.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//class CPlayerDlg;

/////////////////////////////////////////////////////////////////////////////
// CConfigFonts dialog
IMPLEMENT_DYNCREATE(CConfigFonts, CPropertyPage)

CConfigFonts::CConfigFonts(CWnd * p) : CPropertyPage(CConfigFonts::IDD),
    m_PlayerDlg(p), m_lplfTitles(0), m_lplfPanel(0)
{
	//{{AFX_DATA_INIT(CConfigFonts)
	//}}AFX_DATA_INIT
    memset(&m_lfTitles, 0, sizeof(LOGFONT));
	memset(&m_lfPanel, 0, sizeof(LOGFONT));
    init();
    ReadReg();
}
CConfigFonts::~CConfigFonts() {
//	CFont * f = m_SamplePanel.GetFont();
//	f->DeleteObject();
//	f = m_SampleTitles.GetFont();
//	f->DeleteObject();
	m_FontSampleTitles.DeleteObject();
	m_FontSamplePanel.DeleteObject();
}
void CConfigFonts::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigFonts)
	DDX_Control(pDX, IDC_SAMPLE2, m_SamplePanel);
	DDX_Control(pDX, IDC_FONTSIZE2, m_SizePanel);
	DDX_Control(pDX, IDC_FONT2, m_FontPanel);
	DDX_Control(pDX, IDC_BOLD2, m_BoldPanel);
	DDX_Control(pDX, IDC_FONTSIZE, m_SizeTitles);
	DDX_Control(pDX, IDC_BOLD, m_BoldTitles);
	DDX_Control(pDX, IDC_SAMPLE, m_SampleTitles);
	DDX_Control(pDX, IDC_FONT, m_FontTitles);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConfigFonts, CPropertyPage)
	//{{AFX_MSG_MAP(CConfigFonts)
	ON_BN_CLICKED(IDC_BOLD, showSample)
	ON_BN_CLICKED(IDC_BOLD2, showSample)
	ON_CBN_SELENDOK(IDC_FONT, showSample)
	ON_CBN_SELENDOK(IDC_FONT2, showSample)
	ON_CBN_SELENDOK(IDC_FONTSIZE, showSample)
	ON_CBN_SELENDOK(IDC_FONTSIZE2, showSample)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void
CConfigFonts::init() {
    setDefault();
}

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

void
CConfigFonts::setDefault() {

    sscanf(deffont, CCFONTFMT, 
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

    strcpy(m_lfTitles.lfFaceName, deffontfacename);
    memcpy(&m_samplelfTitles, &m_lfTitles, sizeof(LOGFONT));
    m_lplfTitles = &m_lfTitles;

    sscanf(deffont, CCFONTFMT, 
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

    strcpy(m_lfPanel.lfFaceName, deffontfacename);
    memcpy(&m_samplelfPanel, &m_lfPanel, sizeof(LOGFONT));
    m_lplfPanel = &m_lfPanel;

}
/////////////////////////////////////////////////////////////////////////////
// CConfigFonts message handlers

void
CConfigFonts::ReadReg() {
    AutoBuf buf(1000);
    RegistryKey reg( HKEY_LOCAL_MACHINE, RegKey );
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
}


void
CConfigFonts::StoreReg() {
    AutoBuf buf(1000);
    RegistryKey reg( HKEY_LOCAL_MACHINE, RegKey );
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
}

void CConfigFonts::OnOK() 
{
    copy2lf(m_lfTitles, m_lfPanel);

#ifdef asdf
    CString msg;

    m_FontTitles.GetWindowText(msg);
    strcpy(m_lfTitles.lfFaceName, (LPCTSTR)msg);

    int c = m_BoldTitles.GetCheck();
    if (c == 0) {
        m_lfTitles.lfWeight = 400;
    } else 
        m_lfTitles.lfWeight = 700;

    m_SizeTitles.GetWindowText(msg);
    LONG h;
    sscanf((LPCTSTR)msg, "%d", &h);
    h = -h;
    m_lfTitles.lfHeight = h;
#endif
	StoreReg();
	CPropertyPage::OnOK();
}
void
CConfigFonts::copy2lf(LOGFONT & lfT, LOGFONT & lfP) {
    CString msg;
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
    }


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
    }
}

BOOL CConfigFonts::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
    m_FontTitles.SubclassDlgItem(IDC_FONT, (CWnd*)m_PlayerDlg);
    m_FontTitles.Initialize();
    int r = m_FontTitles.SelectString(-1,m_lfTitles.lfFaceName);

    m_FontPanel.SubclassDlgItem(IDC_FONT2, (CWnd*)m_PlayerDlg);
    m_FontPanel.Initialize();
    r = m_FontPanel.SelectString(-1,m_lfPanel.lfFaceName);

    char buf[5];
    int i;
    for (i = 5 ; i < 30; ++i) {
        sprintf(buf, "%d", i);
        m_SizeTitles.AddString(buf);
		m_SizePanel.AddString(buf);
    }
    sprintf(buf, "%d", abs(m_lfTitles.lfHeight));
    m_SizeTitles.SelectString(0, buf);
	sprintf(buf, "%d", abs(m_lfPanel.lfHeight));
	m_SizePanel.SelectString(0, buf);

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

    static bool first = true;
    if (0 && first == true) {
        {
			first = false;
			CFont * f = m_SampleTitles.GetFont();
			CFont newfont;
			LOGFONT lf;
			if (f != NULL) {
				f->GetObject(sizeof(LOGFONT), &lf);
				newfont.CreateFontIndirect(&lf);
			} else {
				newfont.CreateStockObject(ANSI_VAR_FONT);
			}
			m_SampleTitles.SetFont(&newfont);
			newfont.Detach();
			newfont.DeleteObject();
		}
        {
			CFont * f = m_SamplePanel.GetFont();
			CFont newfont;
			LOGFONT lf;
			if (f != NULL) {
				f->GetObject(sizeof(LOGFONT), &lf);
				newfont.CreateFontIndirect(&lf);
			} else {
				newfont.CreateStockObject(ANSI_VAR_FONT);
			}
			m_SamplePanel.SetFont(&newfont);
			newfont.Detach();
			newfont.DeleteObject();
		}


    }

    showSample();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void
CConfigFonts::showSample() {
	{
//		CFont * oldfont = m_SampleTitles.GetFont();
//		if (oldfont != NULL)
//			oldfont->DeleteObject();

		UpdateData(TRUE);
		copy2lf(m_samplelfTitles,m_samplelfPanel);
//		CFont f;
		m_FontSampleTitles.DeleteObject();
		m_FontSampleTitles.CreateFontIndirect(&m_samplelfTitles);
		m_SampleTitles.SetFont(&m_FontSampleTitles, TRUE);
		m_SampleTitles.SetWindowText(m_samplelfTitles.lfFaceName);
//		if (oldfont != NULL) {
//			m_SampleTitles.SetFont(oldfont, FALSE);
//		}
//		f.Detach();
//		f.DeleteObject();
    }
	{
//		CFont * oldfont = m_SamplePanel.GetFont();
//		if (oldfont != NULL)
//			oldfont->DeleteObject();

		UpdateData(TRUE);
		copy2lf(m_samplelfTitles,m_samplelfPanel);
//		CFont f;
		m_FontSamplePanel.DeleteObject();
		m_FontSamplePanel.CreateFontIndirect(&m_samplelfPanel);
		m_SamplePanel.SetFont(&m_FontSamplePanel, TRUE);
		m_SamplePanel.SetWindowText(m_samplelfPanel.lfFaceName);
//		if (oldfont != NULL) {
//			m_SamplePanel.SetFont(oldfont, FALSE);
//		}
//		f.Detach();
//		f.DeleteObject();
    }
}
