// ConfigPassword.cpp : implementation file
//

//#include "stdafx.h"
#include "irman_registry.h"
#include "ConfigPassword.h"
#include "MyString.h"
#include "TestHarness.h"
#include <rpc.h>
#include "MBMessageBox.h"
#include "MyLog.h"
#include "SMTP.h"
#include "mbpw.h"
#include "../muzikbrowzerVersion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfigPassword property page

IMPLEMENT_DYNCREATE(CConfigPassword, CPropertyPage)

CConfigPassword::CConfigPassword(CWnd * p) : CPropertyPage(CConfigPassword::IDD),
    m_PlayerDlg(p), m_TrialMode(1)
{
	//{{AFX_DATA_INIT(CConfigPassword)
	m_HostId = _T("");
	m_Email = _T("");
	m_Firstname = _T("");
	m_Lastname = _T("");
	m_Password = _T("");
	m_Notice = _T("");
	m_TrialExpiration = _T("");
	m_TrialLabel = _T("");
	//}}AFX_DATA_INIT
	m_TrialLabel = "Trial Expires";
    ReadReg();
	validate();
}

CConfigPassword::~CConfigPassword()
{
}

void CConfigPassword::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigPassword)
	DDX_Control(pDX, IDC_VALIDATE_PW, m_ValidatePw);
//	DDX_Control(pDX, IDC_SENDINFO, m_RequestPw);
	DDX_Text(pDX, IDC_CP_HOSTID, m_HostId);
	DDV_MaxChars(pDX, m_HostId, 100);
	DDX_Text(pDX, IDC_CP_EMAIL, m_Email);
	DDV_MaxChars(pDX, m_Email, 100);
	DDX_Text(pDX, IDC_CP_FIRSTNAME, m_Firstname);
	DDV_MaxChars(pDX, m_Firstname, 100);
	DDX_Text(pDX, IDC_CP_LASTNAME, m_Lastname);
	DDV_MaxChars(pDX, m_Lastname, 100);
	DDX_Text(pDX, IDC_CP_PASSWORD, m_Password);
	DDV_MaxChars(pDX, m_Password, 100);
	DDX_Text(pDX, IDC_PASSWORD_MSG, m_Notice);
	DDX_Text(pDX, IDC_CP_TRIAL_EXPIRATION, m_TrialExpiration);
	DDX_Text(pDX, IDC_CP_TRIAL_LABEL, m_TrialLabel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConfigPassword, CPropertyPage)
	//{{AFX_MSG_MAP(CConfigPassword)
	ON_BN_CLICKED(IDC_SENDINFO, OnSendinfo)
	ON_BN_CLICKED(IDC_VALIDATE_PW, OnValidatePw)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigPassword message handlers
void
CConfigPassword::ReadReg() {
    RegistryKey reg( HKEY_LOCAL_MACHINE, RegKey );
	AutoBuf buf(5001);
	char dflt[] = "";
	reg.Read("WindowData", buf.p, 5000, dflt);
	CString h = buf.p;
//	if (h == "") return;
	CString s1 = h2s(h);
	CString host = String::field(s1,",",1);
	CString email = String::field(s1,",",2);
	CString fname = String::field(s1,",",3);
	CString lname = String::field(s1,",",4);
	CString pw = String::field(s1,",",5);
	CString expdate = String::field(s1,",",6);
	host = h2s(host);
	m_Firstname = h2s(fname);
	m_Lastname = h2s(lname);
	m_Email = h2s(email);
	m_Password = h2s(pw);
	m_TrialExpiration = h2s(expdate);
	if (host == "") {
		host = genHost();
	}
	m_HostId = host;


	CString y = String::field(m_TrialExpiration, "/", 1);
	CString m = String::field(m_TrialExpiration, "/", 2);
	CString d = String::field(m_TrialExpiration, "/", 3);
	int iy = atoi(y);
	int im = atoi(m);
	int id = atoi(d);
	CTime exp;
	if (iy < 2003 || iy > 3000 || im < 1 || im > 12 || id < 1 || id > 31
			|| m_TrialExpiration == "") {
		CTime t = CTime::GetCurrentTime();
		time_t now = t.GetTime();
		now += (86400 * MB_TRIAL_DAYS);
		CTime e ( now );
		m_TrialExpiration = e.Format("%Y/%m/%d");
		exp = e;
	} else {
		CTime expnew ( iy, im, id, 0,0,0);
		exp = expnew;
	}
	m_ExpDate = exp;
}
void
CConfigPassword::StoreReg() {
    RegistryKey reg( HKEY_LOCAL_MACHINE, RegKey );

	CString s;
	CString host = s2h(m_HostId);
	CString fname = s2h(m_Firstname);
	CString lname = s2h(m_Lastname);
	CString email = s2h(m_Email);
	CString pw = s2h(m_Password);
	CString mexpdate = m_ExpDate.Format("%Y/%m/%d");
	CString expdate = s2h(mexpdate);
	s = host + "," + email + "," + fname + "," + lname + "," + pw
		+ "," + expdate;
	CString h = s2h(s);

	reg.Write("WindowData", h);
}
void CConfigPassword::OnOK() 
{
	StoreReg();
	validate();	
	CPropertyPage::OnOK();
}

BOOL CConfigPassword::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	OnValidatePw();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CConfigPassword::OnCancel() 
{
	CPropertyPage::OnCancel();
}
CString
CConfigPassword::s2h(const CString & s) {
	CString h;
	int n = s.GetLength();
	int i;
	char buf[3];
	for (i = 0 ; i < n ; ++i) {
		char ch = s.GetAt(i);
		sprintf(buf,"%02x", (int)ch+1);
		h += buf;
	}
	CString hr;
	for (i = h.GetLength()-1 ; i >= 0 ; --i) {
		hr += h[i];
	}
	return hr;
}
CString
CConfigPassword::h2s(const CString & h) {
	CString s,hr;
	int i;
	for(i = h.GetLength()-1 ; i >= 0 ; --i) {
		hr += h[i];
	}
	int n = hr.GetLength();
	for (i = 0 ; i < n ; i += 2) {
		char buf[3];
		int ich;
		buf[0] = hr[i];
		buf[1] = hr[i+1];
		buf[2] = 0;
		sscanf(buf, "%02x", &ich);
		--ich;
		s += (char) ich;
	}
	return s;
}

CString
CConfigPassword::genHost() {
//RPC_STATUS RPC_ENTRY UuidCreate( 
//  UUID *  Uuid  

	//RPC_STATUS RPC_ENTRY UuidToString( 
	//  UUID *  Uuid,                  
	//  unsigned char * *  StringUuid  
	//);
 
	//CString GUIDgen()
	//{
	// GUID guid;
	// CoCreateGuid(&guid);
	// BYTE * str;
	// UuidToString((UUID*)&guid, &str);
	// 
	// CString unique((LPTSTR)str);
	//
	// RpcStringFree(&str);
	//
	// unique.Replace(_T("-"), _T("_"));
	//
	// return unique;
	//} 

	char buf[MAX_PATH];
	DWORD size = MAX_PATH;
	GetComputerName(buf,&size);
	CString hostname = buf;
	if (hostname != "") {
		return hostname;
	}

	CString x;

	UUID uuid;
	RPC_STATUS status = UuidCreate(&uuid);
	switch(status) {
	case RPC_S_OK:
		break;
	case RPC_S_UUID_LOCAL_ONLY:
		break;
	case RPC_S_UUID_NO_ADDRESS:
		break;
	}
	unsigned char * stringuuid;
	status = UuidToString(&uuid, &stringuuid);
	if (status != RPC_S_OK) {
		MBMessageBox("Error", "Error genHost");
		return x;
	}
	x = stringuuid;
	RpcStringFree(&stringuuid);
	
	// 60ab82d8-d53e-4e9d-96d8-dfea01ed26ac
	x = uuidEncode(x);

	return x;
}
CString
CConfigPassword::uuidEncode(const CString & uuid) {
	CString encoded;
	long nsubs = String::delCount(uuid, "-");
	long i;
	for(i = 1 ; i <= nsubs ; ++i) {
		CString part = String::field(uuid, "-", i);
		CString encodedPart = BaseC::HexStringToSixtyTwoString(part);
		if (encoded != "")
			encoded += "-";
		encoded += encodedPart;
	}
	return encoded;
}
CString
CConfigPassword::uuidDecode(const CString & uuid) {
	CString decoded;
	long nsubs = String::delCount(uuid, "-");
	long i;
	for(i = 1; i <= nsubs; ++i) {
		CString part = String::field(uuid,"-",i);
		CString decodedPart = BaseC::SixtyTwoStringToHexString(part);
		if (decoded != "")
			decoded += "-";
		decoded += decodedPart;
	}
	return decoded;
}
TEST(uuidEncode, CConfigPassword)
{
	CString Uuid,UuidEncoded,UuidDecoded;

	UUID uuid;
	int i;
	for(i = 0 ; i < 10 ; i++) {
		RPC_STATUS status = UuidCreate(&uuid);
		switch(status) {
		case RPC_S_OK:
			break;
		case RPC_S_UUID_LOCAL_ONLY:
			break;
		case RPC_S_UUID_NO_ADDRESS:
			break;
		}
		unsigned char * stringuuid;
		status = UuidToString(&uuid, &stringuuid);
		if (status != RPC_S_OK) {
			MBMessageBox("Error", "Error genHost");
		}
		Uuid = stringuuid;
		RpcStringFree(&stringuuid);
		
		// 60ab82d8-d53e-4e9d-96d8-dfea01ed26ac
		UuidEncoded = CConfigPassword::uuidEncode(Uuid);
		UuidDecoded = CConfigPassword::uuidDecode(UuidEncoded);
		CHECK(Uuid == UuidDecoded);
	}
}

//TEST(mail, CSMTP)
//{
//	CSMTP smtp;
//	CString headers;
//	headers << "From: mkm\nTo: mkm\nSubject: test\n\n";
//	smtp.mail("cotton.kooware.com:25", "root@cotton",
//		"mkm@cotton.kooware.com", headers, "test");
//}

void CConfigPassword::OnSendinfo() 
{
	UpdateData();
	if (m_Firstname.GetLength() == 0
		|| m_Lastname.GetLength() == 0
		|| m_Email.GetLength() == 0) {
		MBMessageBox("Alert", "You must fill in name/email first");
		return;
	}
	CString serverPort, from, to, headers, body;
	serverPort = "muzikbrowzer.com:25";
	from = "pwrequest@muzikbrowzer.com";
	to = "pwrequest@muzikbrowzer.com";
	headers = "From: pwrequest@muzikbrowzer.com\r\n";
	headers += "Reply-To: ";
	headers += m_Email;
	headers += "\r\n";
	headers += "To: pwrequest@muzikbrowzer.com\r\n";
	headers += "Subject: Password Request\r\n";
	body = MBPW_HOSTID + m_HostId + "\r\n";
	body += MBPW_EXP + m_ExpDate.Format("%Y/%m/%d") + "\r\n";
	body += MBPW_FNAME + m_Firstname + "\r\n";
	body += MBPW_LNAME + m_Lastname + "\r\n";
	body += MBPW_EMAIL + m_Email + "\r\n";
	body += MBPW_MBVERSION + CString(MUZIKBROWZER_VERSION) + "\r\n";

	CSMTP smtp;
	BOOL r = smtp.mail(serverPort,from,to,headers,body);
	if (r != TRUE) {
		CString msg = "Unable to send information to muzikbrowzer.com.";
		msg += "\r\nPlease contact muzikbrowzer support.";
		MBMessageBox("Alert", msg);
	} else {
		CString msg = "Information sent to muzikbrowzer.com\r\n";
		msg += "You will receive your password within a few days.";
		MBMessageBox("Advisory", msg);
	}
}
void CConfigPassword::validate() {
	CString p = createPassword();
	if (m_Password == p) {
		m_TrialMode = 0;
		m_Notice = "Valid password. Trial Mode removed.";
		m_TrialLabel = "No Expiration";
		m_TrialExpiration = "";
	} else {
		if (CTime::GetCurrentTime() < m_ExpDate) {
			m_TrialMode = 1;
m_Notice = "To purhcase muzikbrowzer and remove Trial Mode go to www.muzikbrowzer.com ";
m_Notice += "and click on Purchase. If you've already purchased muzikbrowzer you ";
m_Notice += "should have received your password, enter it along with your name and ";
m_Notice += "email address above. \r\n\r\n";
      
m_Notice += "If you've already purchased muzikbrowzer and you need a new password ";
m_Notice += "go to www.muzikbrowzer.com Support and enter a support Ticket.";

		} else {
			m_TrialMode = 2;
			m_Notice = "Your Trial period has expired. You may purchase a muzikbrowzer ";
			m_Notice += "license at www.muzikbrowzer.com";
			m_TrialExpiration = "";
			m_TrialLabel = "Expired";
		}
		
	}
}

CString
CConfigPassword::createPassword() {
	CString msg;
	//	CString host = s2h(m_HostId);
	//	CString fname = s2h(m_Firstname);
	//	CString lname = s2h(m_Lastname);
	//	CString email = s2h(m_Email);
	//msg << m_HostId << m_Firstname << m_Lastname << m_Email;

	AutoBuf bhost(1000);
	AutoBuf bfname(1000);
	AutoBuf blname(1000);
	AutoBuf bemail(1000);
	strcpy(bhost.p,(LPCTSTR)m_HostId);
	strcpy(bfname.p,(LPCTSTR)m_Firstname);
	strcpy(blname.p,(LPCTSTR)m_Lastname);
	strcpy(bemail.p,(LPCTSTR)m_Email);

	char * pmd5 = mbpwcreate(bhost.p,bfname.p,blname.p,bemail.p);

	CString md5 = pmd5;
	free(pmd5);

	return md5;
}


void CConfigPassword::OnValidatePw() 
{
	UpdateData();
	validate();
	if (m_TrialMode) {
//		m_RequestPw.EnableWindow(TRUE);
		m_ValidatePw.EnableWindow(TRUE);
		m_Notice = "Invalid Password: " + m_Notice;
	} else {
//		m_RequestPw.EnableWindow(FALSE);
	}
	UpdateData(FALSE);
	
}
