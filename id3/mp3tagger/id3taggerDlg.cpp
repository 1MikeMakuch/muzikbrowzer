// id3taggerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "id3tagger.h"
#include "id3taggerDlg.h"
#include "DlgProxy.h"

#define LOGOWNER
#include "MyLog.h"

#include "FileFolderDialog.h"
#include <id3.h>
#include <id3/tag.h>
#include <id3/misc_support.h>
#include "MyID3LibMiscSupport.h"
#include "MyString.h"
#include "CheckResult.h"
#include "help.h"
#include "Progress.h"
//#include "Genres.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static CString crlf("\r\n");
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CId3taggerDlg dialog

IMPLEMENT_DYNAMIC(CId3taggerDlg, CDialog);

CId3taggerDlg::CId3taggerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CId3taggerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CId3taggerDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pAutoProxy = NULL;
}

CId3taggerDlg::~CId3taggerDlg()
{
	// If there is an automation proxy for this dialog, set
	//  its back pointer to this dialog to NULL, so it knows
	//  the dialog has been deleted.
	if (m_pAutoProxy != NULL)
		m_pAutoProxy->m_pDialog = NULL;
}

void CId3taggerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CId3taggerDlg)
	DDX_Control(pDX, IDC_BUTTON_SHOWALL, m_ShowAll);
	DDX_Control(pDX, IDC_BUTTON_REMOVEALLFILES, m_FilesRemoveAll);
	DDX_Control(pDX, IDC_BUTTON_REMOVECOMMANDALL, m_CommandRemoveAll);
	DDX_Control(pDX, IDC_COMBO_NEWVALUE, m_SetValue);
	DDX_Control(pDX, IDC_COMBO_MATCHVALUE, m_MatchValue);
	DDX_Control(pDX, IDC_BUTTON_COMMANDSCLEAR, m_CommandsClear);
	DDX_Control(pDX, IDC_COMBO_FIELD1, m_MatchField);
	DDX_Control(pDX, IDC_COMBO_FIELD2, m_SetField);
	DDX_Control(pDX, IDC_BUTTON_REMOVEFILES, m_FilesRemove);
	DDX_Control(pDX, IDC_BUTTON_REMOVECOMMAND, m_CommandRemove);
	DDX_Control(pDX, IDC_BUTTON_CHECK, m_Check);
	DDX_Control(pDX, IDC_BUTTON_ADDFILES, m_FilesAdd);
	DDX_Control(pDX, IDC_BUTTON_ADDCOMMAND, m_CommandAdd);
	DDX_Control(pDX, IDC_LIST_FILES, m_Files);
	DDX_Control(pDX, IDC_LIST_COMMANDS, m_Commands);
	DDX_Control(pDX, IDC_EDIT_TAGINFO, m_TagInfo);
	DDX_Control(pDX, IDC_COMBO_OPERATION, m_Operation);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CId3taggerDlg, CDialog)
	//{{AFX_MSG_MAP(CId3taggerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_BN_CLICKED(ID_HELP, OnHelp)
	ON_BN_CLICKED(IDC_BUTTON_ADDCOMMAND, OnButtonAddcommand)
	ON_BN_CLICKED(IDC_BUTTON_ADDFILES, OnButtonAddfiles)
	ON_BN_CLICKED(IDC_BUTTON_CHECK, OnButtonCheck)
	ON_BN_CLICKED(IDC_BUTTON_REMOVECOMMAND, OnButtonRemovecommand)
	ON_BN_CLICKED(IDC_BUTTON_REMOVEFILES, OnButtonRemovefiles)
	ON_LBN_DBLCLK(IDC_LIST_FILES, OnDblclkListFiles)
	ON_LBN_DBLCLK(IDC_LIST_COMMANDS, OnDblclkListCommands)
	ON_BN_CLICKED(IDC_BUTTON_REMOVEALLFILES, OnButtonRemoveallfiles)
	ON_BN_CLICKED(IDC_BUTTON_REMOVECOMMANDALL, OnButtonRemovecommandall)
	ON_BN_CLICKED(IDC_BUTTON_COMMANDSCLEAR, OnButtonCommandsclear)
	ON_LBN_SELCHANGE(IDC_LIST_FILES, OnSelchangeListFiles)
	ON_CBN_SELCHANGE(IDC_COMBO_FIELD1, OnSelchangeComboField1)
	ON_CBN_EDITCHANGE(IDC_COMBO_MATCHVALUE, OnEditchangeComboMatchvalue)
	ON_BN_CLICKED(IDC_BUTTON_SHOWALL, OnButtonShowall)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CId3taggerDlg message handlers

BOOL CId3taggerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	CFont font;
	font.CreateStockObject(ANSI_FIXED_FONT);
	m_TagInfo.SetFont(&font);

	EnableDisable();
	Genre_init();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CId3taggerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

void CId3taggerDlg::OnDestroy()
{
	WinHelp(0L, HELP_QUIT);
	CDialog::OnDestroy();
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CId3taggerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CId3taggerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

// Automation servers should not exit when a user closes the UI
//  if a controller still holds on to one of its objects.  These
//  message handlers make sure that if the proxy is still in use,
//  then the UI is hidden but the dialog remains around if it
//  is dismissed.

void CId3taggerDlg::OnClose() 
{
	if (CanExit())
		CDialog::OnClose();
}

void CId3taggerDlg::OnOK() 
{
	if (CanExit())
		CDialog::OnOK();
}

void CId3taggerDlg::OnCancel() 
{
	if (CanExit())
		CDialog::OnCancel();
}

BOOL CId3taggerDlg::CanExit()
{
	// If the proxy object is still around, then the automation
	//  controller is still holding on to this application.  Leave
	//  the dialog around, but hide its UI.
	if (m_pAutoProxy != NULL)
	{
		ShowWindow(SW_HIDE);
		return FALSE;
	}

	return TRUE;
}

void CId3taggerDlg::OnHelp() 
{
	CCheckResult dlg("Ok", helpmsg());
	dlg.DoModal();	
}

void CId3taggerDlg::OnButtonAddcommand() 
{
	TagOp * to = new TagOp;
	CString matchField, op, matchValue, setField, setValue;
	
	m_MatchField.GetWindowText(matchField);
	m_Operation.GetWindowText(op);
	m_MatchValue.GetWindowText(matchValue);
	m_SetField.GetWindowText(setField);
	m_SetValue.GetWindowText(setValue);

	int mfs, ops, mvs;
	mfs = matchField.GetLength() > 0;
	ops = op.GetLength() > 0;
	mvs = matchValue.GetLength() > 0;

	if (
		(setField == "" || setValue == "") ||
		((mfs || ops || mvs) && (!mfs || !ops)) ) {
		m_TagInfo.SetWindowText("Error: incomplete command!");
		return;
	} else {
		m_TagInfo.SetWindowText("");
	}

	to->m_MatchField = matchField;
	m_MatchField.SetWindowText("");
	
	to->m_Op = op;
	m_Operation.SetWindowText("");
	
	to->m_MatchValue = matchValue;
	m_MatchValue.SetWindowText("");
	
	to->m_SetField = setField;
	m_SetField.SetWindowText("");
	
	to->m_SetValue = setValue;
	m_SetValue.SetWindowText("");

	m_Ops.Add(to);

	displayCommands();
	OnButtonCommandsclear();

	if (m_MatchValue.FindStringExact(-1, matchValue) == CB_ERR) {
		m_MatchValue.AddString(matchValue);
	}
	if (m_SetValue.FindStringExact(-1, setValue) == CB_ERR) {
		m_SetValue.AddString(setValue);
	}
	EnableDisable();
}

void CId3taggerDlg::OnButtonAddfiles() 
{
	CFileDialog *dialog;

	dialog = new CFileDialog(TRUE, 
							"mp3",
							m_AddRememberDir,
							OFN_ALLOWMULTISELECT |
							OFN_FILEMUSTEXIST    | 
							OFN_HIDEREADONLY     |
							OFN_EXPLORER,
			"MPEG Audio Files {*.mpg;*.mp1;*.mp2;*.mp3}|*.mpg;*.mp1;*.mp2;*.mp3|All Files {*.*}|*.*||");

	dialog->m_ofn.lpstrFile = (char*)malloc(100000);
	memset(dialog->m_ofn.lpstrFile, 0, 100000);
	dialog->m_ofn.nMaxFile = 99999;

	int ret;
	ret = dialog->DoModal();
	if (ret == IDOK) {
        CString file = dialog->GetPathName();
		m_AddRememberDir = file;
		POSITION pos = dialog->GetStartPosition();
		while (pos) {
			file = dialog->GetNextPathName(pos);
			m_Files.AddString(file);
		}
	}
	free(dialog->m_ofn.lpstrFile);

	delete dialog;
	
	EnableDisable();
}
      
//To allow the user to select multiple files, set the OFN_ALLOWMULTISELECT
//flag before calling DoModal. You need to supply your own filename
//buffer to accommodate the returned list of multiple filenames. Do
//this by replacing m_ofn.lpstrFile with a pointer to a buffer you have
//allocated, after constructing the CFileDialog, but before calling 
//DoModal. Additionally, you must set m_ofn.nMaxFile with the number of
//characters in the buffer pointed to by m_ofn.lpstrFile.
//     
//CFileDialog relies on the COMMDLG.DLL file that ships with Windows 
//versions 3.1 and later.  
     

void CId3taggerDlg::OnButtonCheck() 
{
	CString check, file;
	int num = m_Files.GetCount();
	int i;
	check = "The edit commands you entered will have the following effect(s) on the\r\nfiles you selected, when you click \"Doit\".\r\n\r\n";
	CString commands = displayCommands();
	check += commands;
	CProgress pdlg;
	pdlg.m_Progress.SetRange(0, num);
	pdlg.ShowWindow(SW_SHOWNORMAL);
	for(i = 0 ; i < num ; ++i) {
		m_Files.GetText(i, file);
		check += editMp3(FALSE, file);
		pdlg.m_Progress.SetPos(i);
	}
	pdlg.EndDialog(0);

	CCheckResult dlg("Doit", check);

	check = "Edits Done. Here are the results\r\n\r\n";
	check += commands;

	int ret = dlg.DoModal();
	if (ret == IDOK) {
		CProgress pdlg;
		pdlg.m_Progress.SetRange(0, num);
		pdlg.ShowWindow(SW_SHOWNORMAL);
		for(i = 0 ; i < num ; ++i) {
			m_Files.GetText(i, file);
			check += editMp3(TRUE, file);
			pdlg.m_Progress.SetPos(i);
		}
		pdlg.EndDialog(0);
		CCheckResult dlg("Ok", check);
		dlg.DoModal();
	}
	EnableDisable();
}

CString CId3taggerDlg::editMp3(BOOL EditOrDry, CString file) {
	CString result;
	result = file + crlf;

	ID3_Tag id3, newid3;
	id3.Link(file, ID3TT_ALL);
	flags_t uflag = ID3TT_ID3V2;
	if (id3.HasV1Tag()) {
		uflag |= ID3TT_ID3V1;
	}

	int n = m_Ops.GetSize();
	int i;
	BOOL updated = FALSE;
	for(i = 0 ; i < n ; ++i) {
		TagOp * op = (TagOp*)m_Ops.GetAt(i);
		updated |= DoOp(EditOrDry, op, id3, newid3);
	}

	CStringArray fields;
	fields.Add("Genre");
	fields.Add("Artist");
	fields.Add("Album");
	fields.Add("Title");
	fields.Add("Track");
	fields.Add("Year");

	n = fields.GetSize();
	CString oldval,newval;
	char buf[500];
	CString format;
	if (EditOrDry == TRUE) {
		format = "   %-12s %s\r\n";
	} else {
		format = "   %-12s %-30s\t%s\r\n";
	}
	for(i = 0 ; i < n ; ++i ) {
		oldval = quotedValueFromField(fields.GetAt(i), id3);
		if (EditOrDry == TRUE) {
			sprintf(buf, format, fields.GetAt(i), oldval);
		} else {
			newval = quotedValueFromField(fields.GetAt(i), newid3);
			sprintf(buf, format, fields.GetAt(i), oldval, newval);
		}
		result += buf;
	}
	if (updated && EditOrDry == TRUE) {
		flags_t tags = id3.Update(uflag);
		if (tags == ID3TT_NONE) {
			result += "Error: unable to update id3 tag.\r\n";
		}
	}

	return result;
}

BOOL CId3taggerDlg::DoOp(BOOL EditOrDry, TagOp * Op, ID3_Tag & id3,
							ID3_Tag & newid3) {
	BOOL result = FALSE;
	CString newvalue;
	CString op = Op->m_Op;
	CString matchValue = valueFromField(Op->m_MatchField, id3);
	CString replaceValue = valueFromField(Op->m_SetField, id3);
	if (op == "is equal to") {
		if (Op->m_MatchValue == matchValue) {
			newvalue = Op->m_SetValue;
		}
	} else if (op == "is not equal to") {
		if (Op->m_MatchValue != matchValue) {
			newvalue = Op->m_SetValue;
		}
	} else if (op == "contains") {
		if (matchValue.Find(Op->m_MatchValue) >= 0) {
			newvalue = Op->m_SetValue;
		}
	} else if (op == "does not contain") {
		if (matchValue.Find(Op->m_MatchValue) == -1) {
			newvalue = Op->m_SetValue;
		}
	} else if (op == "is blank") {
		if (matchValue.GetLength() == 0) {
			newvalue = Op->m_SetValue;
		}
	} else if (op == "is not blank") {
		if (matchValue.GetLength() > 0) {
			newvalue = Op->m_SetValue;
		}
	} else if (op == "") {
		newvalue = Op->m_SetValue;
	}
	if (newvalue.GetLength()) {
		result = TRUE;
		if (EditOrDry == TRUE) {
			setField(id3, id3, Op->m_SetField, newvalue);
		} else {
			setField(id3, newid3, Op->m_SetField, newvalue);
		}
	}
	return result;
}

CString CId3taggerDlg::quotedValueFromField(CString field, ID3_Tag & id3) {
	CString value;
	value = valueFromField(field, id3);
	if (value.GetLength()) {
		value = CS("\"") + value + CS("\"");
	}
	return value;
}

CString CId3taggerDlg::valueFromField(CString field, ID3_Tag & id3) {
	CString value;
	if (field == "Genre") {
		value = Genre_normalize(id3_GetGenre(&id3));
	} else if (field == "Artist") {
		value = id3_GetArtist(&id3);
	} else if (field == "Album") {
		value = id3_GetAlbum(&id3);
	} else if (field == "Title") {
		value = id3_GetTitle(&id3);
	} else if (field == "Track") {
		value = id3_GetTrack(&id3);
	} else if (field == "Year") {
		value = id3_GetYear(&id3);
	}
	return value;
}
void CId3taggerDlg::setField(ID3_Tag & srcid3, ID3_Tag & id3,
							 CString & field, CString & invalue) {
	CString genre, artist, album, title, track, year;
	genre = Genre_normalize(id3_GetGenre(&srcid3));
	artist = id3_GetArtist(&srcid3);
	album = id3_GetAlbum(&srcid3);
	title = id3_GetTitle(&srcid3);
	track = id3_GetTrack(&srcid3);
	year = id3_GetYear(&srcid3);
	atoi("");
	CString value = invalue;
	value = String::replace(value, genre, "{Genre}");
	value = String::replace(value, artist, "{Artist}");
	value = String::replace(value, album, "{Album}");
	value = String::replace(value, title, "{Title}");
	value = String::replace(value, track, "{Track}");
	value = String::replace(value, year, "{Year}");

	CString oldval;
	if (field == "Genre") {
		oldval = genre;
	} else if (field == "Artist") {
		oldval = artist;
	} else if (field == "Album") {
		oldval = album;
	} else if (field == "Title") {
		oldval = title;
	} else if (field == "Track" && value.GetLength() && atoi(value)) {
		oldval = track;
	} else if (field == "Year") {
		oldval = year;
	}
	
	if (value.Find("{LastName, FirstName}") >= 0) {
		value = doLastCommaFirst(oldval);
	}
	if (value.Find("{FirstName LastName}") >= 0) {
		value = doFirstLast(oldval);
	}
	
	if (field == "Genre") {
		ID3_AddGenre(&id3, value, true);
	} else if (field == "Artist") {
		ID3_AddArtist(&id3, value, true);
	} else if (field == "Album") {
		ID3_AddAlbum(&id3, value, true);
	} else if (field == "Title") {
		ID3_AddTitle(&id3, value, true);
	} else if (field == "Track" && value.GetLength() && atoi(value)) {
		ID3_AddTrack(&id3, atoi(value), 0, true);
	} else if (field == "Year") {
		ID3_AddYear(&id3, value, true);
	}
}

CString CId3taggerDlg::doLastCommaFirst(const CString & oldval) {
	int n = String::delCount(oldval, " ");
	CString val = String::field(oldval, " ", n);
	val += ", ";
	int i;
	for(i = 1 ; i < n ; ++i) {
		val += String::field(oldval, " ", i);
		val += " ";
	}
	val.TrimLeft();
	val.TrimRight();
	return val;
}
CString CId3taggerDlg::doFirstLast(const CString & oldval) {
	int n = String::delCount(oldval, ",");
	CString val = String::field(oldval, ",", n);
	val.TrimLeft();
	val.TrimRight();
	val += " ";
	int i;
	for(i = 1 ; i < n ; ++i ) {
		val += String::field(oldval, ",", i);
		val.TrimLeft();
		val.TrimRight();
		val += " ";
	}
	val.TrimLeft();
	val.TrimRight();
	return val;
}

void CId3taggerDlg::OnButtonDoit() 
{
	// TODO: Add your control notification handler code here
	
}

void CId3taggerDlg::OnButtonRemovecommand() 
{
	int sel = m_Commands.GetCurSel();
	TagOp * to = (TagOp*)m_Ops.GetAt(sel);
	m_MatchField.SelectString(-1, to->m_MatchField);
	m_SetField.SelectString(-1, to->m_SetField);
	m_Operation.SelectString(-1, to->m_Op);
	m_MatchValue.SetWindowText(to->m_MatchValue);
	m_SetValue.SetWindowText(to->m_SetValue);
	delete to;
	m_Ops.RemoveAt(sel);
	displayCommands();
	EnableDisable();	
}

void CId3taggerDlg::OnButtonRemovefiles() 
{
	int sel = m_Files.GetCurSel();
	m_Files.DeleteString(sel);
	EnableDisable();
}

void CId3taggerDlg::OnDblclkListFiles() 
{
	int sel = m_Files.GetCurSel();
	CString file;
	m_Files.GetText(sel, file);
	ID3_Tag id3;
	id3.Link(file);
	CString info;
	info = displayTag(&id3, TRUE, file);
	m_TagInfo.SetWindowText(info);
	
}

void CId3taggerDlg::OnDblclkListCommands() 
{
	// TODO: Add your control notification handler code here
	
}

void CId3taggerDlg::OnButtonRemoveallfiles() 
{
	m_Files.ResetContent();
	EnableDisable();
}

CString CId3taggerDlg::displayCommands() {
	CString out;
	m_Commands.ResetContent();
	int n = m_Ops.GetSize();
	int i;
	int maxOldField, maxNewField, maxOldValue, maxNewValue, maxOp;
	maxOldField = maxNewField = maxOldValue = maxNewValue = maxOp = 0;
	for(i = 0 ; i < n ; ++i) {
		TagOp * to = (TagOp*)m_Ops.GetAt(i);
		maxOldField = __max(maxOldField, to->m_MatchField.GetLength());
		maxNewField = __max(maxNewField, to->m_SetField.GetLength());
		maxOldValue = __max(maxOldValue, to->m_MatchValue.GetLength());
		maxNewValue = __max(maxNewValue, to->m_SetValue.GetLength());
		maxOp = __max(maxOp, to->m_Op.GetLength());
	}
	CString formatCondVal, formatCondNoVal, formatSet;
	CString fpct = "%";
	CString fs = "s";
	CString fspaces = " ";
	formatCondVal += "If ";
	formatCondVal += fpct + fs + fspaces;
	formatCondVal += fpct + fs + fspaces;
	formatCondVal += CString("\"") + fpct + fs + CString("\"") + fspaces;
	formatCondVal += CString("then set ") + fpct + fs + fspaces;
	formatCondVal += CString("to \"") + fpct + fs + CString("\"");

	formatCondNoVal += "If ";
	formatCondNoVal += fpct + fs + fspaces;
	formatCondNoVal += fpct + fs + fspaces;
	formatCondNoVal += CString("then set ") + fpct + fs + fspaces;
	formatCondNoVal += CString("to \"") + fpct + fs + CString("\"");

	formatSet += CString("Set ") + fpct + fs + fspaces;
	formatSet += CString("to \"") + fpct + fs + CString("\"");

	char buf[200];
	for(i = 0 ; i < n ; ++i) {
		TagOp * to = (TagOp*)m_Ops.GetAt(i);
		if (to->m_Op == "is blank" || to->m_Op == "is not blank") {
			sprintf(buf, formatCondNoVal,
				to->m_MatchField,
				to->m_Op,
				to->m_SetField,
				to->m_SetValue);
		} else if (to->m_MatchField.GetLength() && to->m_MatchValue.GetLength()) {
			sprintf(buf, formatCondVal,
				to->m_MatchField,
				to->m_Op,
				to->m_MatchValue,
				to->m_SetField,
				to->m_SetValue);
		} else if (to->m_MatchField == "" || to->m_Op == "") {
			sprintf(buf, formatSet,
				to->m_SetField,
				to->m_SetValue);
		} else {
			sprintf(buf, "error");
		}
		m_Commands.AddString(buf);
		out += buf;
		out += "\r\n";

	}
	out += "\r\n";

	return out;
}

void CId3taggerDlg::OnButtonRemovecommandall() 
{
	int n = m_Ops.GetSize();
	int i;
	for(i = 0 ; i < n ; ++i) {
		TagOp * to = (TagOp*)m_Ops.GetAt(i);
		delete to;
	}
	m_Ops.RemoveAll();
	m_Commands.ResetContent();
	OnButtonCommandsclear();
	EnableDisable();	
}

void CId3taggerDlg::OnButtonCommandsclear() 
{
	m_MatchField.SetCurSel(0);
	m_SetField.SetCurSel(0);
	m_Operation.SetCurSel(0);
	m_MatchValue.SetWindowText("");
	m_SetValue.SetWindowText("");
	EnableDisable();
}

void CId3taggerDlg::OnSelchangeListFiles() 
{
	OnDblclkListFiles();	
}

void CId3taggerDlg::OnSelchangeComboField1() 
{
	int sel = m_MatchField.GetCurSel();
	m_SetField.SetCurSel(sel);	
}

void CId3taggerDlg::OnEditchangeComboMatchvalue() 
{
}

void CId3taggerDlg::EnableDisable() {
	if (m_Files.GetCount() && m_Commands.GetCount()) {
		m_Check.EnableWindow(TRUE);
	} else {
		m_Check.EnableWindow(FALSE);
	}
	if (m_Files.GetCount()) {
		m_FilesRemove.EnableWindow(TRUE);
		m_FilesRemoveAll.EnableWindow(TRUE);
		m_ShowAll.EnableWindow(TRUE);
	} else {
		m_FilesRemove.EnableWindow(FALSE);
		m_FilesRemoveAll.EnableWindow(FALSE);
		m_ShowAll.EnableWindow(FALSE);
	}
	if (m_Commands.GetCount()) {
		m_CommandRemove.EnableWindow(TRUE);
		m_CommandRemoveAll.EnableWindow(TRUE);
	} else {
		m_CommandRemoveAll.EnableWindow(FALSE);
		m_CommandRemove.EnableWindow(FALSE);
	}
}

void CId3taggerDlg::OnButtonShowall() 
{
	int num = m_Files.GetCount();
	int i;
	CString file,info;
	info = "Tags for all files...\r\n\r\n";
	CProgress pdlg;
	pdlg.m_Progress.SetRange(0, num);
	pdlg.ShowWindow(SW_SHOWNORMAL);
	for(i = 0 ; i < num ; ++i) {
		m_Files.GetText(i, file);
		ID3_Tag id3;
		id3.Link(file);
		info += "\r\n#############################################################################################################\r\n";
		info += file;
		info += "\r\n";
		info += displayTag(&id3);
		pdlg.m_Progress.SetPos(i);
	}
	pdlg.EndDialog(0);
	CCheckResult dlg("Ok", info);
	dlg.DoModal();
}
