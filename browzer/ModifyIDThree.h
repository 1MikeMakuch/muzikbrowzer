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

/////////////////////////////////////////////////////////////////////////////
// ModifyIDThree dialog

class ModifyIDThree : public CDialog
{
// Construction
public:
	ModifyIDThree(CStringList *gl, Song song, int, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(ModifyIDThree)
	enum { IDD = IDD_ID3TAG_DIALOG };
	CStatic	m_File;
	CComboBox	m_Genre;
	CStatic	m_OldYear;
	CStatic	m_OldTrack;
	CStatic	m_OldTitle;
	CStatic	m_OldGenre;
	CStatic	m_OldArtist;
	CStatic	m_OldAlbum;
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
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
    int mWindowFlag;
//    ID3_Tag * mID3_Tag;
	Song m_Song;
    CStringList *mGenreList;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODIFYIDTHREE_H__75D89DBB_D31F_11D5_8680_002078049F22__INCLUDED_)
