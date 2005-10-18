// mydiffDlg.cpp : implementation file
//

#include "stdafx.h"
#include "mydiff.h"
#include "mydiffDlg.h"
#include "Process.h"
#include "cverror.h"
#include "MyString.h"
#include "TestHarness.h"
#include "direct.h"


#define LOGOWNER
#include "MyLog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_MSG(UPM_FINISHED)
IMPLEMENT_MSG(UPM_LINE)
IMPLEMENT_MSG(UPM_DIFF_DONE)
IMPLEMENT_MSG(UPM_CSTAT_DONE)
IMPLEMENT_MSG(UPM_GETFILE_DONE)
IMPLEMENT_MSG(UPM_VERIFY_DONE)

/////////////////////////////////////////////////////////////////////////////
// CMydiffDlg dialog

CMydiffDlg::CMydiffDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMydiffDlg::IDD, pParent), m_StatusDlg(NULL)
{
	//{{AFX_DATA_INIT(CMydiffDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	//	m_cstatDone = FALSE;
	//	m_doingCstat = FALSE;
	//	m_diffDone = FALSE;
	//	m_doingDiff = FALSE;
	m_currentFile = -1;
}

void CMydiffDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMydiffDlg)
	DDX_Control(pDX, IDC_EDIT_DIFFPOS, m_EditDiffPos);
	DDX_Control(pDX, IDC_EDIT_CWD, m_EditCWD);
	DDX_Control(pDX, ID_BUTTON_REDIFF, m_ButtonRediff);
	DDX_Control(pDX, ID_BUTTON_PREV_FILE, m_ButtonFilePrev);
	DDX_Control(pDX, ID_BUTTON_PREV_DIFF, m_ButtonDiffPrev);
	DDX_Control(pDX, ID_BUTTON_NEXT_FILE, m_ButtonFileNext);
	DDX_Control(pDX, ID_BUTTON_NEXT_DIFF, m_ButtonDiffNext);
	DDX_Control(pDX, IDC_LIST_DATA2, m_Data2);
	DDX_Control(pDX, IDC_LIST_DATA1, m_Data1);
	DDX_Control(pDX, IDC_LIST_FILES, m_Files);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMydiffDlg, CDialog)
    ON_REGISTERED_MESSAGE(UPM_FINISHED, OnFinished)
	ON_REGISTERED_MESSAGE(UPM_LINE, OnLine)
	ON_REGISTERED_MESSAGE(UPM_CSTAT_DONE, OnCstatDone)
	ON_REGISTERED_MESSAGE(UPM_DIFF_DONE, OnDiffDone)
	ON_REGISTERED_MESSAGE(UPM_GETFILE_DONE, OnGetFileDone)
	ON_REGISTERED_MESSAGE(UPM_VERIFY_DONE, OnVerifyDone)
	//{{AFX_MSG_MAP(CMydiffDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_BUTTON_NEXT_DIFF, OnButtonNextDiff)
	ON_BN_CLICKED(ID_BUTTON_NEXT_FILE, OnButtonNextFile)
	ON_BN_CLICKED(ID_BUTTON_PREV_DIFF, OnButtonPrevDiff)
	ON_BN_CLICKED(ID_BUTTON_PREV_FILE, OnButtonPrevFile)
	ON_BN_CLICKED(ID_BUTTON_REDIFF, OnButtonRediff)
	ON_LBN_DBLCLK(IDC_LIST_FILES, OnDblclkListFiles)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMydiffDlg message handlers


BOOL CMydiffDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_Data1.m_companion = & m_Data2;
	m_Data2.m_companion = & m_Data1;

	m_Files.m_highlights = TRUE;

    TestResult tr;
    TestRegistry::runAllTests(tr);
	if (tr.failures())
		MessageBox(tr.results(), "mydiff Test Results");

	controlsEnable(FALSE);
	getcwd();

	CString args = ::GetCommandLine();

	
	
	CString command = "/cygdrive/c/mkm/src/muzik/mydiff/cstat";
	logger.open("");
	logger.log("mydiff started");

	mysystem(command, UPM_CSTAT_DONE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}
void CMydiffDlg::controlsEnable(BOOL enable) {
	m_ButtonRediff.EnableWindow(enable);
	m_ButtonFilePrev.EnableWindow(enable);
	m_ButtonFileNext.EnableWindow(enable);
	m_ButtonDiffPrev.EnableWindow(enable);
	m_ButtonDiffNext.EnableWindow(enable);
}

void CMydiffDlg::getcwd() {
	AutoBuf buf(MAX_PATH);
	_getcwd(buf.p, MAX_PATH );
	m_cwd = buf.p;
	m_EditCWD.SetWindowText(m_cwd);
	UpdateData(FALSE);
}

void CMydiffDlg::OnPaint() 
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
HCURSOR CMydiffDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

///////////////////////////////////////////////////////////////////////
// Button handlers
///////////////////////////////////////////////////////////////////////
int CMydiffDlg::pageHeight() {
    CRect wrect;
    m_Data1.GetWindowRect( wrect ) ;
    int iheight = m_Data1.GetItemHeight(0);

    int pagelen = wrect.Height() / iheight;
	return pagelen;
}
void CMydiffDlg::showDiffPos() {
	if (!m_currentdiff) {
		m_EditDiffPos.SetWindowText("");
		return;
	}
	DiffOb* dob = (DiffOb*)m_DiffList.GetAt(m_currentdiff);
	CString msg = numToString(dob->m_diffNum);
	msg += "/";
	msg += numToString(m_totalNumDiffs);
	m_EditDiffPos.SetWindowText(msg);
	UpdateData(FALSE);
}

void CMydiffDlg::OnButtonNextFile() 
{
	incrementFile();
	dodiff();
}

void CMydiffDlg::OnButtonPrevFile() 
{
	decrementFile();
	dodiff();
}

void CMydiffDlg::OnButtonNextDiff() 
{
	DiffOb * dob = NULL;

	if (m_currentdiff == NULL) {
		m_currentdiff = m_DiffList.GetTailPosition();
		if (m_currentdiff)
			dob = (DiffOb*)m_DiffList.GetAt(m_currentdiff);
	} else if (m_currentdiff == m_diffTail) {
		dob = (DiffOb*)m_DiffList.GetAt(m_currentdiff);
	} else {
		m_DiffList.GetNext(m_currentdiff);
		dob = (DiffOb*) m_DiffList.GetAt(m_currentdiff);
	}
	if (dob == NULL) return;

	int line = dob->m_line;
	int page = pageHeight() / 2;
	line -= page;
	if (line < 0) line = 0;

	m_Data1.SetTopIndex(line);
	m_Data2.SetTopIndex(line);
	
	showDiffPos();
	
}

void CMydiffDlg::OnButtonPrevDiff() 
{
	DiffOb * dob;
	if (m_currentdiff == NULL) {
		m_currentdiff = m_DiffList.GetHeadPosition();
		if (!m_currentdiff) return;
		dob = (DiffOb*)m_DiffList.GetAt(m_currentdiff);
	} else if (m_currentdiff == m_diffHead) {
		dob = (DiffOb*)m_DiffList.GetAt(m_currentdiff);
	} else {
		m_DiffList.GetPrev(m_currentdiff);
		if (!m_currentdiff) return;
		dob = (DiffOb*)m_DiffList.GetAt(m_currentdiff);
	}
	if (dob == NULL) return;
	 
	int line = dob->m_line;
	int page = pageHeight() / 2;
	line -= page;
	if (line < 0) line = 0;

	m_Data1.SetTopIndex(line);
	m_Data2.SetTopIndex(line);
	
	showDiffPos();
}

void CMydiffDlg::OnButtonRediff() 
{
	dodiff();
	
}
void CMydiffDlg::OnDblclkListFiles() 
{
	dodiff();
	
}


///////////////////////////////////////////////////////////////////////
// Msg handlers
///////////////////////////////////////////////////////////////////////
LRESULT CMydiffDlg::OnLine(WPARAM wParam, LPARAM)
{
	CString * s = (CString *)wParam;
	m_ComOutput.AddTail(*s);
//	logger.log(*s);
	delete s;
	return 0;
}

LRESULT CMydiffDlg::OnCstatDone(WPARAM wParam, LPARAM) {
	POSITION pos;
	for(pos = m_ComOutput.GetHeadPosition() ; pos != NULL;) {
		CString file = m_ComOutput.GetAt(pos);
		if (file.GetLength())
			m_Files.AddString(file);
		m_ComOutput.GetNext(pos);
	}
	showStatus("cstat done");
	OnButtonNextFile();

	return 0;
}
LRESULT CMydiffDlg::OnDiffDone(WPARAM wParam, LPARAM) {
	POSITION pos;
	for(pos = m_ComOutput.GetHeadPosition() ; pos != NULL;) {
//		m_DiffBox.AddString(m_ComOutput.GetAt(pos));
		CString tmp = m_ComOutput.GetAt(pos);
		m_Diff.AddTail(tmp);
		m_ComOutput.GetNext(pos);
	}

	showStatus("diff done");
	CString command ;
	if (m_currentStatus == "Locally Modified") {
		command = "cvs update -p ";
		command += m_currentfile;
	} else {
		command = "# noop";
	}

	mysystem(command, UPM_GETFILE_DONE);
	return 0;
}

LRESULT CMydiffDlg::OnGetFileDone(WPARAM wParam, LPARAM) {
	POSITION pos;
	CString line;
	showStatus("get file done");
	int ktr = 0;
	for(pos = m_ComOutput.GetHeadPosition() ; pos != NULL;) {
		line = m_ComOutput.GetAt(pos);
		if (ktr == 0 && String::substring(line,0,5) == "*****") {
			ktr++;
		} else if (ktr > 0) {
			SrcLine * sl1 = new SrcLine(line, 0, 1, 0);
			SrcLine * sl2 = new SrcLine(line, 0, 1, 0);
			m_File1.AddTail(sl1);
			m_File2.AddTail(sl2);
		}
		m_ComOutput.GetNext(pos);
	}

	if (m_currentStatus == "Locally Modified") {
		showStatus("processing diff");
		processDiff();
		CString v = verify();
		if (v.GetLength()) {
			MessageBox(v, "Verify failed");
		} else {
			showDiff(m_File1, m_Data1);
			showDiff(m_File2, m_Data2);
		}
	}
	killStatus();
	OnButtonPrevDiff();
	controlsEnable(TRUE);
	return 0;
}
LRESULT CMydiffDlg::OnVerifyDone(WPARAM wParam, LPARAM) {
	POSITION pos;
	CString lines;
	int ktr = 0;
	for(pos = m_ComOutput.GetHeadPosition() ; pos != NULL;) {
		lines += m_ComOutput.GetAt(pos);
		lines += "\r\n";
		m_ComOutput.GetNext(pos);
	}
	SrcLine * sl = (SrcLine*) m_File2.getAt(0);
	if (sl && sl->line() != "removed") {
		if (lines != "") {
			MessageBox(lines, "Verify failed!");
		}
	}
//	m_DiffBox.AddString("Verify results:");
//	m_DiffBox.AddString(lines);
	UpdateData(FALSE);
	return 0;
}

LRESULT CMydiffDlg::OnFinished(WPARAM wParam, LPARAM)
{
	m_Running = FALSE;
	CString msg;
	if(wParam != 0)
	{ /* has error */
		msg = cvError((DWORD)wParam);
		m_ComOutput.AddTail(msg);
	} /* has error */
	OutputDebugString("OnFinished\r\n");
	
	PostMessage(m_msg2post);
	
	return 0;
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

void CMydiffDlg::dodiff() {
	showStatus("dodiff");
	int sel = m_Files.GetCurSel();
	if (sel < 0) return;

	CString entry;
	m_Files.GetText(sel, entry);
	if (entry == "") {
		showStatus("dodiff no file");
		return;
	}
	CString file = String::field(entry,"|",1);
	m_currentStatus = String::field(entry,"|",2);
	m_currentfile = String::field(entry,"|",3);

	m_Data1.ResetContent();
	m_Data2.ResetContent();
	m_File1.RemoveAll();
	m_File2.RemoveAll();
	m_Diff.RemoveAll();
//	m_DiffBox.ResetContent();
	m_currentdiff = NULL;

//	CString res = readDFile(m_currentfile);

	CString command ;
	if (m_currentStatus == "Locally Modified") {
		command = "cvs diff -e --ignore-all-space ";
		command += m_currentfile;
	} else {
		command = "# noop";
	}

	mysystem(command, UPM_DIFF_DONE);

}
CString
CMydiffDlg::readDFile(const CString & file) {
	CFile dfile;
    CFileException fileException;
    if (!dfile.Open( file,
        CFile::modeRead,
        &fileException ))
    {
        CString msg = "Unable to read ";
        msg += file;
        return msg;
    }
    if (dfile.GetLength()) {
        AutoBuf buf(dfile.GetLength()+1);
        dfile.Read(buf.p, dfile.GetLength());
		filebuf2csl(m_CurrFile, buf.p, dfile.GetLength());
	}
	return "";
}

void CMydiffDlg::incrementFile() {
	int sel = m_Files.GetCurSel();
	sel++;
	m_Files.SetCurSel(sel);
	UpdateData(FALSE);
}
void CMydiffDlg::decrementFile() {
	int sel = m_Files.GetCurSel();
	sel--;
	if (sel < 0) sel = 0;
	m_Files.SetCurSel(sel);
	UpdateData(FALSE);
}

void CMydiffDlg::filebuf2csl(CStringList & list, const char * buf,
							 long length) {
	int i = 0;
	char ch, prevch;
	ch = prevch = 0;
	CString line;
	while (i < length) {
		if (ch) prevch = ch;
		char ch = buf[i];
		if (ch == '\r') {
			list.AddTail(line);
			line = "";
		} else if (ch == '\n' && prevch != '\r') {
			list.AddTail(line);
			line = "";
		} else {
			line += ch;
		}
		++i;
	}
}
void CMydiffDlg::processDiff() {
	POSITION pos;
	pos = m_Diff.GetHeadPosition();
	CString line = m_Diff.GetAt(pos);
	while (String::substring(line,0,4) != "diff" && pos != NULL) {
		line = m_Diff.GetNext(pos);
		showStatus(line);
	}
	if (pos) {
		line = m_Diff.GetAt(pos);
		showStatus(line);
	}
	if (line.Find("was removed") >= 0) {
		m_File2.RemoveAll();
		m_File2.insertAt(0, "removed", 1,0,1);
		return;
	} else if (line.Find("Binary file") >= 0) {
		m_File1.RemoveAll();
		m_File2.RemoveAll();
		m_File1.insertAt(0,"binary file", 1,0,1);
	}

	// now we have the 1st ed line
	int lineno1,lineno2;
	char op;
	int currentlineno = 0;

	while (pos != NULL) {
		op = 0;
		lineno1 = lineno2 = -1;
		showStatus(line);
		edGetCom(line, lineno1, lineno2, op);

		switch (op) {
			case 'a': {
				m_Diff.GetNext(pos);
				int actr = lineno1;
				line = m_Diff.GetAt(pos);
				int diffmark = 1;
				while (pos != NULL && line != ".") {
					m_File2.insertAt(actr, line, 1, 1, 0);
					m_File1.insertAt(actr++, "", 1, 0, diffmark);
					diffmark=0;
					m_Diff.GetNext(pos);
					line = m_Diff.GetAt(pos);
//					line = m_Diff.GetNext(pos);
				}
				break;
			}
			case 'c': {
				if (lineno2 == -1) {
					m_File2.removeAt(lineno1-1);
					m_Diff.GetNext(pos);
					int cctr = lineno1-1;
					line = m_Diff.GetAt(pos);
					int insertctr = 0;
					while (pos != NULL && line != ".") {
						m_File2.insertAt(cctr++, line, 1, 1, 0);
						m_Diff.GetNext(pos);
						line = m_Diff.GetAt(pos);
						insertctr++;
					}
					m_File1.getAt(lineno1-1)->setBN(1,1,1);
					int addctr;
					for(addctr = 1 ; addctr < insertctr ; ++addctr) {
						m_File1.insertAt(lineno1, "", 1, 0, 0);
					}
				} else {
					int diffmark = 1;
					int cctr = lineno2-1;
					for(cctr = lineno2-1 ; cctr >= lineno1-1; --cctr) {
						m_File2.removeAt(cctr);
						m_File1.getAt(cctr)->setBN(1,1, diffmark);
						diffmark = 0;
					}
					m_Diff.GetNext(pos);
					cctr = lineno1-1;
					line = m_Diff.GetAt(pos);
					int insertctr = 0;
					while (pos != NULL && line != ".") {
						m_File2.insertAt(cctr++, line, 1, 1, 0);
						m_Diff.GetNext(pos);
						line = m_Diff.GetAt(pos);
						++insertctr;
					}
					int addctr;
					for(addctr = insertctr-1 ; addctr < (lineno2-lineno1);++addctr) {
						m_File2.insertAt(cctr, "", 1, 0, 0);
					}
					for(addctr = lineno2-lineno1;addctr<insertctr-1;++addctr) {
						m_File1.insertAt(lineno2, "", 1, 0, 0);
					}
				}
				break;
			}
			case 'd': {
				if (lineno2 == -1) {
					m_File2.removeAt(lineno1-1);
					m_File2.insertAt(lineno1-1, "", 1, 0, 0);
					m_File1.getAt(lineno1-1)->setBN(1,1,1);
				} else {
					int dctr;
					int diffmark = 1;
					for(dctr = lineno2-1 ; dctr >= lineno1-1 ; --dctr) {
						m_File2.removeAt(dctr);
//void insertAt(const int idx, const CString &, int bold
//			, int number, int diff);
						m_File2.insertAt(dctr, "", 1, 0, 0);
						m_File1.getAt(dctr)->setBN(1,1,diffmark);
						diffmark=0;
					}
				}
				break;
			}
			default: {
				if (line.Find("cvs diff: No newline at end of file")
					>= 0) {
				} else if (line.Find("Binary file") < 0) {
					CString msg = "Unhandled op code from diff! ";
					msg += line;
					MessageBox(msg);
				} else {
					CString msg = "Unknown msg: ";
					msg += line;
					MessageBox(msg);
				}
			}
		}
		if (pos != NULL) {
			m_Diff.GetNext(pos);
			if (pos != NULL)
				line = m_Diff.GetAt(pos);
		}
	}
}
void
CMydiffDlg::showDiff(SrcLineList & srclist, CExtendedListBox & box) {
	POSITION pos;
	if (&srclist == &m_File1) {
		for(pos = m_DiffList.GetHeadPosition() ; pos != NULL;) {
			DiffOb * dob = (DiffOb*) m_DiffList.GetAt(pos);
			delete dob;
			m_DiffList.GetNext(pos);
		}
		m_DiffList.RemoveAll();
	}

	int linectr = 0;
	int newctr = 0;
	int diffctr = 1;
	CString newline;
	for(pos = srclist.GetHeadPosition() ; pos != NULL;) {
		SrcLine * p = (SrcLine*)srclist.GetAt(pos);
		const CString & line = p->line();
		if (p->number()) {
			char buf[1000];
			sprintf(buf, "%04d %s", linectr++, line);
			newline = buf;
		} else {
			newline = line;
		}
		box.insertString(newctr, newline, p->bold(), p->diff());
		if (&srclist == &m_File1) {
			if (p->diff()) {
				DiffOb * dob = new DiffOb;
				dob->m_line = newctr;
				dob->m_diffNum = diffctr++;
				POSITION dpos = m_DiffList.AddTail(dob);
				if (m_currentdiff == NULL) {
					m_currentdiff = dpos;
					m_diffHead = dpos;
				}
				m_diffTail = dpos;
			}
			m_totalNumDiffs = diffctr-1;
		}

		newctr++;
		srclist.GetNext(pos);
	}
	
	showDiffPos();
}
CString CMydiffDlg::verify() {
	CFile myfile;
	CString filename = "c:\\tmp\\mydiff_verify";
	CFileException fileException;
	if ( !myfile.Open( filename,
        CFile::modeWrite | CFile::modeCreate,
        &fileException ))
	{
        CString msg = "Unable to write script ";
		msg += filename;
		return msg;
	}
	POSITION pos;
	for(pos = m_File2.GetHeadPosition() ; pos != NULL;) {
		SrcLine * sl = (SrcLine*)m_File2.GetAt(pos);
		if (sl->number()) {
			const CString & line = sl->line();
			myfile.Write(line, line.GetLength());
			myfile.Write("\n",1);
		}
		m_File2.GetNext(pos);
	}
	myfile.Flush();
	myfile.Close();

	CString command = "diff -w ";
	command += "/cygdrive/c/tmp/mydiff_verify ";
	command += m_currentfile;

	mysystem(command, UPM_VERIFY_DONE);
	return "";
}
void CMydiffDlg::edGetCom(const CString & line, int & lineno1,
						  int & lineno2, char & op) {

	if (line.Find("cvs diff") >= 0) {
		op = 0;
		return;
	}
	int linelength = line.GetLength();
	int i = 0;
	char ch;
	CString aint1,aint2;
	int get2 = 0;
	while (i < linelength) {
		ch = line.GetAt(i);
		if ('0' <= ch && ch <= '9') {
			if (get2) {
				aint2 += ch;
			} else {
				aint1 += ch;
			}
		} else if (ch == ',') {
			get2 = 1;
		} else {
			op = ch;
			i = linelength + 1;
		}
		++i;
	}
	lineno1 = atoi(aint1);
	if (aint2.GetLength())
		lineno2 = atoi(aint2);
}

void CMydiffDlg::mysystem(const CString & command, UINT msg2post) {
// c:\cygwin\bin\bash.exe --login -c cstat

	logger.log(command);
	m_ComOutput.RemoveAll();
	CString scriptname = "c:\\tmp\\mydiff_script.sh";
	CFile script;
	CFileException fileException;
	if ( !script.Open( scriptname,
        CFile::modeWrite | CFile::modeCreate | CFile::shareDenyNone,
        &fileException ))
	{
        CString msg = "Unable to write script ";
		msg += scriptname;
		m_ComOutput.AddTail(msg);
		msg += "\r\n";
		msg += cvError(fileException.m_lOsError);
		showStatus(msg);
        return;
	}
	

	CString tmp = "cd ";
	CString cwd = String::replace(m_cwd, '\\', "\\\\");

	tmp += cwd;
	tmp += "; ";
	tmp += command;
	OutputDebugString(tmp + "\r\n");
	showStatus(command);


    script.Write((LPCTSTR)tmp, tmp.GetLength());
	script.Write("\n", 1);
    script.Flush();
	script.Close();

	CString pcommand = "c:\\cygwin\\bin\\bash.exe --login ";
	pcommand += "c:/tmp/mydiff_script.sh";

	m_msg2post = msg2post;
     Process * p = new Process(pcommand, this);
     m_Running = p->run();
     if(!m_Running)
	 { /* failed */
		DWORD err = ::GetLastError();
		CString errstr = cvError(err);
		m_ComOutput.AddTail(errstr);
	 }
}




void CMydiffDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	static int first = 1;
	if (first) {
		first = 0;
		return;
	}
	
	resizeControls();
	
}

void
CMydiffDlg::resizeControls() {

	CRect wrect;

#define CONTROL_HSPACE 2
#define CONTROL_VSPACE 1

	CRect dialog;
    GetClientRect(dialog);
    int dtlx = dialog.TopLeft().x;
    int dtly = dialog.TopLeft().y;

    int buttonRow_x = 0;
    int buttonRow_y = 0;

	m_EditCWD.GetWindowRect(wrect);
	int x = buttonRow_x;
	int y = buttonRow_y;
	int width = dialog.Width() * 0.25;
	int height = wrect.Height();
	m_EditCWD.MoveWindow(x,y,width,height, TRUE);

	m_ButtonFilePrev.GetWindowRect(wrect);
	x += CONTROL_HSPACE + width + CONTROL_HSPACE;
	width = wrect.Width();
	m_ButtonFilePrev.MoveWindow(x,y,width,height,TRUE);

	m_ButtonFileNext.GetWindowRect(wrect);
	x += CONTROL_HSPACE + width + CONTROL_HSPACE;
	m_ButtonFileNext.MoveWindow(x,y,width,height,TRUE);

	m_ButtonRediff.GetWindowRect(wrect);
	x += CONTROL_HSPACE + width + CONTROL_HSPACE;
	m_ButtonRediff.MoveWindow(x,y,width,height,TRUE);

	m_ButtonDiffPrev.GetWindowRect(wrect);
	x += CONTROL_HSPACE + width + CONTROL_HSPACE;
	m_ButtonDiffPrev.MoveWindow(x,y,width,height,TRUE);

	m_ButtonDiffNext.GetWindowRect(wrect);
	x += CONTROL_HSPACE + width + CONTROL_HSPACE;
	m_ButtonDiffNext.MoveWindow(x,y,width,height,TRUE);

	m_EditDiffPos.GetWindowRect(wrect);
	x += CONTROL_HSPACE + width + CONTROL_HSPACE;
	m_EditDiffPos.MoveWindow(x,y,wrect.Width(),height,TRUE);

	m_Files.GetWindowRect(wrect);
	x = buttonRow_x;
	y += height + CONTROL_VSPACE;
	width = dialog.Width() * 0.15;
	height = dialog.Height() - y;
	m_Files.MoveWindow(x,y,width,height,TRUE);

	m_Data1.GetWindowRect(wrect);
	x += CONTROL_HSPACE + width + CONTROL_HSPACE;
	width = (dialog.Width() - (width + (CONTROL_HSPACE * 4))) / 2;
	m_Data1.MoveWindow(x,y,width,height,TRUE);

	m_Data2.GetWindowRect(wrect);
	x += CONTROL_HSPACE + width + CONTROL_HSPACE;
	m_Data2.MoveWindow(x,y,width,height,TRUE);

}

void CMydiffDlg::showStatus(const CString & msg) {
	if (m_StatusDlg == NULL) {
		m_StatusDlg = new StatusDlg;
	}
	m_StatusDlg->showStatus(msg);
}
void CMydiffDlg::killStatus() {
	if (m_StatusDlg) {
		delete m_StatusDlg;
	}
	m_StatusDlg = NULL;
}