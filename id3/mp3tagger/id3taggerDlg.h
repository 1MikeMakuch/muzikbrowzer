// id3taggerDlg.h : header file
//

#if !defined(AFX_ID3TAGGERDLG_H__E3951244_B894_472E_B944_08A06DA32BE4__INCLUDED_)
#define AFX_ID3TAGGERDLG_H__E3951244_B894_472E_B944_08A06DA32BE4__INCLUDED_

#include <id3.h>
#include <id3/tag.h>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CId3taggerDlgAutoProxy;
class TagOp;
/////////////////////////////////////////////////////////////////////////////
// CId3taggerDlg dialog

class CId3taggerDlg : public CDialog
{
	DECLARE_DYNAMIC(CId3taggerDlg);
	friend class CId3taggerDlgAutoProxy;

// Construction
public:
	CId3taggerDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CId3taggerDlg();

// Dialog Data
	//{{AFX_DATA(CId3taggerDlg)
	enum { IDD = IDD_ID3TAGGER_DIALOG };
	CButton	m_ShowAll;
	CButton	m_FilesRemoveAll;
	CButton	m_CommandRemoveAll;
	CComboBox	m_SetValue;
	CComboBox	m_MatchValue;
	CButton	m_CommandsClear;
	CComboBox	m_MatchField;
	CComboBox	m_SetField;
	CButton	m_FilesRemove;
	CButton	m_CommandRemove;
	CButton	m_Doit;
	CButton	m_Check;
	CButton	m_FilesAdd;
	CButton	m_CommandAdd;
	CListBox	m_Files;
	CListBox	m_Commands;
	CEdit	m_TagInfo;
	CComboBox	m_Operation;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CId3taggerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CId3taggerDlgAutoProxy* m_pAutoProxy;
	HICON m_hIcon;
	CObArray m_Ops;
	CString m_AddRememberDir;

	BOOL CanExit();
	CString displayCommands();
	CString editMp3(BOOL EditOrDryRun, CString file);
	BOOL DoOp(BOOL EditOrDryRun, TagOp * op, ID3_Tag & id3,
		ID3_Tag & newid3);
	CString valueFromField(CString field, ID3_Tag & id3);
	CString quotedValueFromField(CString field, ID3_Tag & id3);
	void setField(ID3_Tag & srcId3, ID3_Tag & id3, CString & field, CString & value);
	CString doLastCommaFirst(const CString & old);
	CString doFirstLast(const CString & old);
	void EnableDisable();

	// Generated message map functions
	//{{AFX_MSG(CId3taggerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnHelp();
	afx_msg void OnButtonAddcommand();
	afx_msg void OnButtonAddfiles();
	afx_msg void OnButtonCheck();
	afx_msg void OnButtonDoit();
	afx_msg void OnButtonRemovecommand();
	afx_msg void OnButtonRemovefiles();
	afx_msg void OnDblclkListFiles();
	afx_msg void OnDblclkListCommands();
	afx_msg void OnButtonRemoveallfiles();
	afx_msg void OnButtonRemovecommandall();
	afx_msg void OnButtonCommandsclear();
	afx_msg void OnSelchangeListFiles();
	afx_msg void OnSelchangeComboField1();
	afx_msg void OnEditchangeComboMatchvalue();
	afx_msg void OnButtonShowall();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

class TagOp : public CObject {
public:
	CString m_MatchField;
	CString m_Op;
	CString m_MatchValue;
	CString m_SetField;
	CString m_SetValue;
};



//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.





#endif // !defined(AFX_ID3TAGGERDLG_H__E3951244_B894_472E_B944_08A06DA32BE4__INCLUDED_)
