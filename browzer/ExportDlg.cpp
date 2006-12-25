// ExportDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ExportDlg.h"
#include "LMCONS.h"
#include "Registry.h"
#include "FileUtils.h"
#include "MBMessageBox.h"
#include "MyString.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ExportDlg dialog


ExportDlg::ExportDlg(CWnd* pParent /*=NULL*/)
	: CDialog(ExportDlg::IDD, pParent), m_Doit(TRUE)
{
	//{{AFX_DATA_INIT(ExportDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void ExportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ExportDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ExportDlg, CDialog)
	//{{AFX_MSG_MAP(ExportDlg)
	ON_EN_CHANGE(IDC_EXPORT_DIR, OnChange)
	ON_EN_CHANGE(IDC_EXPORT_FILE, OnChange)
	ON_EN_CHANGE(IDC_EXPORT_TMPL, OnChange)
	ON_BN_CLICKED(IDC_EXPORT_GENRE, OnChange)
	ON_BN_CLICKED(IDC_EXPORT_ARTIST, OnChange)
	ON_BN_CLICKED(IDC_EXPORT_ALBUM, OnChange)
	ON_BN_CLICKED(IDC_EXPORT_SONG, OnChange)
	ON_BN_CLICKED(IDC_EXPORT_TRACK, OnChange)
	ON_BN_CLICKED(IDC_EXPORT_LENGTH, OnChange)
	ON_BN_CLICKED(IDC_EXPORT_YEAR, OnChange)
	ON_BN_CLICKED(IDC_EXPORT_MP3FILE, OnChange)
	ON_BN_CLICKED(IDC_EXPORT_CSV, OnChange)
	ON_BN_CLICKED(IDC_EXPORT_HTML, OnChange)
	ON_BN_CLICKED(ID_EXPORT_OUPUT_BROWSE, OnExportOuputBrowse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ExportDlg message handlers


void ExportDlg::OnOK() 
{
	m_Genre = ((CButton*)GetDlgItem(IDC_EXPORT_GENRE))->GetCheck();
	m_Artist= ((CButton*)GetDlgItem(IDC_EXPORT_ARTIST))->GetCheck();
	m_Album = ((CButton*)GetDlgItem(IDC_EXPORT_ALBUM))->GetCheck();
	m_Song  = ((CButton*)GetDlgItem(IDC_EXPORT_SONG ))->GetCheck();
	m_Track = ((CButton*)GetDlgItem(IDC_EXPORT_TRACK))->GetCheck();
	m_Length= ((CButton*)GetDlgItem(IDC_EXPORT_LENGTH))->GetCheck();
	m_Year  = ((CButton*)GetDlgItem(IDC_EXPORT_YEAR))->GetCheck();
	m_Commas= ((CButton*)GetDlgItem(IDC_EXPORT_CSV))->GetCheck();
//	m_Text  = ((CButton*)GetDlgItem(IDC_EXPORT_TXT))->GetCheck();
	m_Html  = ((CButton*)GetDlgItem(IDC_EXPORT_HTML))->GetCheck();
	m_Mp3File  = ((CButton*)GetDlgItem(IDC_EXPORT_MP3FILE))->GetCheck();
	((CEdit*)GetDlgItem(IDC_EXPORT_DIR))->GetWindowText(m_Folder);
	((CEdit*)GetDlgItem(IDC_EXPORT_FILE))->GetWindowText(m_File);
	((CEdit*)GetDlgItem(IDC_EXPORT_TMPL))->GetWindowText(m_HtmlTemplate);

	if (!String::endsWith(m_Folder,"\\"))
		m_Folder += "\\";
	CString check0 = m_Folder ;
	check0 += "\\" ;
	check0 += m_File;
	CString check1,check2;

//	check1 = check0;
	check2 = check0;
	check0 += ".csv";
//	check1 += ".txt";
	check2 += ".htm";

	if (FileUtil::IsReadable(check0)
		/*|| FileUtil::IsReadable(check1)*/
		|| FileUtil::IsReadable(check2)
		) {
		if (MBMessageBox("Confirm overwrite?", "The file:\r\n\r\n"
			+ check0 + "\r\n"
			/*+ check1 + "\r\n" */
			//+ check2 + "\r\n"
			+ "\r\nexists. Overwrite?",FALSE, TRUE)) {
			m_Again = FALSE;
		} else {
			m_Again = TRUE;
		}
	} else
		m_Again = FALSE;

	unsigned long bools = 0;
	bools = (0x001 & m_Genre)
	+ (0x0002 * m_Artist)
	+ (0x0004 * m_Album)
	+ (0x0008 * m_Song)
	+ (0x0010 * m_Track)
	+ (0x0020 * m_Length)
	+ (0x0040 * m_Year)
	+ (0x0080 * m_Commas)
	/*+ (0x0100 * m_Text) */
	+ (0x0200 * m_Html)
	+ (0x0400 * m_Mp3File);

	RegistryKey reg( HKEY_LOCAL_MACHINE, RegKey );
	reg.Write("ExpDir", m_Folder);
	reg.Write("ExpFile", m_File);
	reg.Write("ExpBools", bools);
	reg.Write("ExpHtmlTmpl", m_HtmlTemplate);

	
	CDialog::OnOK();
}

BOOL ExportDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	RegistryKey reg( HKEY_LOCAL_MACHINE, RegKey );
	CString folder,file,htmltemplate;
	folder = reg.ReadCString("ExpDir","");
	file = reg.ReadCString("ExpFile","");
	htmltemplate = reg.ReadCString("ExpHtmlTmpl","");
	if (!folder.GetLength()) {
		char buf[UNLEN];
		DWORD len = UNLEN;
		BOOL r = GetUserName(buf,&len);
		folder = "C:\\Documents and Settings\\";
		folder += buf;
		folder += "\\Application Data\\Muzikbrowzer";
		if (!r)
			folder = "C:\\MuzikbrowzerExport";
	}

	if (!file.GetLength()) {
		file = "MuzikbrowzerTags";
	}
	if (!htmltemplate.GetLength()) {
		htmltemplate = "C:\\Program Files\\Muzikbrowzer\\MbHtmlTemplate.txt";
	}
	unsigned long bools = 0;
	bools = reg.Read("ExpBools", 0xffff);
	m_Genre   = 0x001 & bools;
	m_Artist  = 0x002 & bools;
	m_Album   = 0x004 & bools;
	m_Song    = 0x008 & bools;
	m_Track   = 0x010 & bools;
	m_Length  = 0x020 & bools;
	m_Year    = 0x040 & bools;
	m_Commas  = 0x080 & bools;
	/*Text    = 0x100 & bools;*/
	m_Html    = 0x200 & bools;
	m_Mp3File = 0x400 & bools;
	
	((CButton*)GetDlgItem(IDC_EXPORT_GENRE))->SetCheck(m_Genre);
	((CButton*)GetDlgItem(IDC_EXPORT_ARTIST))->SetCheck(m_Artist);
	((CButton*)GetDlgItem(IDC_EXPORT_ALBUM))->SetCheck(m_Album);
	((CButton*)GetDlgItem(IDC_EXPORT_SONG ))->SetCheck(m_Song);
	((CButton*)GetDlgItem(IDC_EXPORT_TRACK))->SetCheck(m_Track);
	((CButton*)GetDlgItem(IDC_EXPORT_LENGTH))->SetCheck(m_Length);
	((CButton*)GetDlgItem(IDC_EXPORT_YEAR))->SetCheck(m_Year);
	((CButton*)GetDlgItem(IDC_EXPORT_CSV))->SetCheck(m_Commas);
	//((CButton*)GetDlgItem(IDC_EXPORT_TXT))->SetCheck(m_Text);
	((CButton*)GetDlgItem(IDC_EXPORT_HTML))->SetCheck(m_Html);
	((CButton*)GetDlgItem(IDC_EXPORT_MP3FILE))->SetCheck(m_Mp3File);
	((CEdit*)GetDlgItem(IDC_EXPORT_DIR))->SetWindowText(folder);
	((CEdit*)GetDlgItem(IDC_EXPORT_FILE))->SetWindowText(file);
	((CEdit*)GetDlgItem(IDC_EXPORT_TMPL))->SetWindowText(htmltemplate);

	m_Again = FALSE;	
	EnableDisable();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void ExportDlg::EnableDisable() {
	m_Genre = ((CButton*)GetDlgItem(IDC_EXPORT_GENRE))->GetCheck();
	m_Artist= ((CButton*)GetDlgItem(IDC_EXPORT_ARTIST))->GetCheck();
	m_Album = ((CButton*)GetDlgItem(IDC_EXPORT_ALBUM))->GetCheck();
	m_Song  = ((CButton*)GetDlgItem(IDC_EXPORT_SONG ))->GetCheck();
	m_Track = ((CButton*)GetDlgItem(IDC_EXPORT_TRACK))->GetCheck();
	m_Length= ((CButton*)GetDlgItem(IDC_EXPORT_LENGTH))->GetCheck();
	m_Year  = ((CButton*)GetDlgItem(IDC_EXPORT_YEAR))->GetCheck();
	m_Commas= ((CButton*)GetDlgItem(IDC_EXPORT_CSV))->GetCheck();
//	m_Text  = ((CButton*)GetDlgItem(IDC_EXPORT_TXT))->GetCheck();
	m_Html  = ((CButton*)GetDlgItem(IDC_EXPORT_HTML))->GetCheck();
	m_Mp3File  = ((CButton*)GetDlgItem(IDC_EXPORT_MP3FILE))->GetCheck();
	((CEdit*)GetDlgItem(IDC_EXPORT_DIR))->GetWindowText(m_Folder);
	((CEdit*)GetDlgItem(IDC_EXPORT_FILE))->GetWindowText(m_File);
	((CEdit*)GetDlgItem(IDC_EXPORT_TMPL))->GetWindowText(m_HtmlTemplate);

	if ((m_Genre || m_Artist || m_Album || m_Song || m_Track || m_Length
		|| m_Year || m_Mp3File) && (m_Commas || m_Html)
		&& (m_Folder.GetLength() && m_File.GetLength())
		&& (!m_Html || m_HtmlTemplate.GetLength())) {
		((CButton*)GetDlgItem(IDOK))->EnableWindow(TRUE);
	} else
		((CButton*)GetDlgItem(IDOK))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_EXPORT_CSV))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_EXPORT_HTML))->ShowWindow(SW_HIDE);
	((CButton*)GetDlgItem(ID_EXPORT_TEMPLATE_BROWSE))->ShowWindow(SW_HIDE);
	((CButton*)GetDlgItem(ID_EXPORT_TEMPLATE_BROWSE))->ShowWindow(SW_HIDE);
	((CButton*)GetDlgItem(IDC_EXPORT_TMPL))->ShowWindow(SW_HIDE);


}
void ExportDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	m_Doit = FALSE;
	CDialog::OnCancel();
}

void ExportDlg::OnChange() 
{
	EnableDisable();
}

void ExportDlg::OnExportOuputBrowse() 
{
	// TODO: Add your control notification handler code here
	
}
