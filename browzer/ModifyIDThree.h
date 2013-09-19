#if !defined(AFX_MODIFYIDTHREE_H__75D89DBB_D31F_11D5_8680_002078049F22__INCLUDED_)
#define AFX_MODIFYIDTHREE_H__75D89DBB_D31F_11D5_8680_002078049F22__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModifyIDThree.h : header file
//
#include "Resource.h"
#include "id3/tag.h"
#include "MusicDb.h"
#include "HListBox.h"

/////////////////////////////////////////////////////////////////////////////
// ModifyIDThree dialog

class ModifyIDThree : public CDialog
{
// Construction
public:
	ModifyIDThree(CStringArray *gl, Song song, int wflag,
		CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(ModifyIDThree)
	enum { IDD = IDD_ID3TAG_DIALOG };
	CStatic	m_OldValsLabel;
	CHListBox	m_TagData;
	CEdit		m_File;
	CComboBox	m_Genre;
	CEdit	m_OldYear;
	CEdit	m_OldTrack;
	CEdit	m_OldTitle;
	CEdit	m_OldGenre;
	CEdit	m_OldArtist;
	CEdit	m_OldAlbum;
	CEdit	m_Year;
	CEdit	m_Track;
	CEdit	m_Title;
	CEdit	m_Artist;
	CEdit	m_Album;
	//}}AFX_DATA

//    ID3_Tag m_newID3_Tag;
	Song m_newSong;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ModifyIDThree)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(ModifyIDThree)
	virtual BOOL OnInitDialog();
	afx_msg void OnMoreid3();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
    int mWindowFlag;
//    ID3_Tag * mID3_Tag;
	Song m_Song;
    CStringArray *mGenreList;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODIFYIDTHREE_H__75D89DBB_D31F_11D5_8680_002078049F22__INCLUDED_)
