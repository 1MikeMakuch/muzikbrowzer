// ModifyIDThree.cpp : implementation file
//

#include "stdafx.h"

#include "ModifyIDThree.h"
#include "id3/tag.h"
#include "id3/misc_support.h"
//#include "Genres.h"
#include "ID3Display.h"
#include "MyID3LibMiscSupport.h"
#include "MyString.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ModifyIDThree dialog


ModifyIDThree::ModifyIDThree(CStringList * gl, Song song,
							 int flag, CWnd* pParent /*=NULL*/)
	: CDialog(ModifyIDThree::IDD, pParent), m_Song(song), mWindowFlag(flag),
    mGenreList(gl)
{
	//{{AFX_DATA_INIT(ModifyIDThree)
	//}}AFX_DATA_INIT
	m_newSong = new CSong;
}


void ModifyIDThree::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ModifyIDThree)
	DDX_Control(pDX, IDC_TAG_FILE, m_File);
	DDX_Control(pDX, IDC_EDIT_GENRE, m_Genre);
	DDX_Control(pDX, IDC_OLDYEAR, m_OldYear);
	DDX_Control(pDX, IDC_OLDTRACK, m_OldTrack);
	DDX_Control(pDX, IDC_OLDTITLE, m_OldTitle);
	DDX_Control(pDX, IDC_OLDGENRE, m_OldGenre);
	DDX_Control(pDX, IDC_OLDARTIST, m_OldArtist);
	DDX_Control(pDX, IDC_OLDALBUM, m_OldAlbum);
	DDX_Control(pDX, IDC_EDIT_YEAR, m_Year);
	DDX_Control(pDX, IDC_EDIT_TRACK, m_Track);
	DDX_Control(pDX, IDC_EDIT_TITLE, m_Title);
	DDX_Control(pDX, IDC_EDIT_ARTIST, m_Artist);
	DDX_Control(pDX, IDC_EDIT_ALBUM, m_Album);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ModifyIDThree, CDialog)
	//{{AFX_MSG_MAP(ModifyIDThree)
	ON_BN_CLICKED(IDC_MOREID3, OnMoreid3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ModifyIDThree message handlers

BOOL ModifyIDThree::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    m_Year.EnableWindow(FALSE);
    m_Track.EnableWindow(FALSE);
    m_Title.EnableWindow(FALSE);
    m_Genre.EnableWindow(FALSE);
    m_Artist.EnableWindow(FALSE);
    m_Album.EnableWindow(FALSE);

//    CString genre = id3_GetGenre(mID3_Tag);
	CString genre = m_Song->getId3(CS("TCON"));

    genre = Genre_normalize((LPCTSTR)genre);

	CString artist = m_Song->getId3(CS("TPE1"));
	CString album = m_Song->getId3(CS("TALB"));
	CString title = m_Song->getId3(CS("TIT2"));
	CString track = m_Song->getId3(CS("TRCK"));
	CString year = m_Song->getId3(CS("TYER"));
	CString file = m_Song->getId3("FILE");
	m_File.SetWindowText(file);

	m_OldGenre.SetWindowText(genre);
    POSITION pos;
	CString tmpGenre;
    for (pos = mGenreList->GetHeadPosition(); pos != NULL; ) {
        tmpGenre = mGenreList->GetAt(pos);
        m_Genre.AddString(tmpGenre);
        mGenreList->GetNext(pos);
    }
    m_Genre.SelectString(0, genre );

    int x = m_Genre.GetCount();

    if (mWindowFlag >= 1 && genre != MBALL ) {
        m_Genre.EnableWindow(TRUE);
    }

    if (mWindowFlag >= 2) {
        m_Artist.EnableWindow(TRUE);
		m_Artist.SetWindowText(artist);
		m_OldArtist.SetWindowText(artist);
    }
    if (mWindowFlag >= 3) {
        m_Album.EnableWindow(TRUE);
        m_Year.EnableWindow(TRUE);
		m_Album.SetWindowText(album);
		m_OldAlbum.SetWindowText(album);
		m_Year.SetWindowText(year);
		m_OldYear.SetWindowText(year);
    }
    if (mWindowFlag >= 4) {
        m_Title.EnableWindow(TRUE);
        m_Track.EnableWindow(TRUE);
	    m_Title.SetWindowText(title);
		m_OldTitle.SetWindowText(title);
		m_Track.SetWindowText(track);
		m_OldTrack.SetWindowText(track);
    }
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void ModifyIDThree::OnMoreid3() 
{
	ID3Display * dialog;
    dialog = new ID3Display(m_Song);
    int ret = dialog->DoModal();
    delete dialog;
	
}

void ModifyIDThree::OnOK() 
{

	int somethingchanged = 0;
    CString oldGenre,oldArtist,oldAlbum,oldTitle,oldYear,oldTrack;
    m_OldGenre.GetWindowText(oldGenre);
    m_OldArtist.GetWindowText(oldArtist);
    m_OldAlbum.GetWindowText(oldAlbum);
    m_OldTitle.GetWindowText(oldTitle);
    m_OldYear.GetWindowText(oldYear);
    m_OldTrack.GetWindowText(oldTrack);

    CString genre,artist,album,title,year,track;
//    m_Genre.GetLBText(m_Genre.GetCurSel(), genre ) ;
    m_Genre.GetWindowText(genre);
    m_Artist.GetWindowText(artist);
    m_Album.GetWindowText(album);
    m_Title.GetWindowText(title);
    m_Year.GetWindowText(year);
    m_Track.GetWindowText(track);


    if (oldGenre != genre) {
//        Genre_addGenre(m_newID3_Tag, (LPCTSTR) genre);
		m_newSong->setId3(CS("TCON"), genre);
		somethingchanged = 1;
    }
    if (oldArtist != artist) {
//        ID3_AddArtist(&m_newID3_Tag, (LPCTSTR) artist, true);
		m_newSong->setId3(CS("TPE1"), artist);
		somethingchanged = 1;
    }
    if (oldAlbum != album) {
//        ID3_AddAlbum(&m_newID3_Tag, (LPCTSTR) album, true);
		m_newSong->setId3(CS("TALB"), album);
		somethingchanged = 1;
    }
    if (oldTitle != title) {
//        ID3_AddTitle(&m_newID3_Tag, (LPCTSTR) title, true);
		m_newSong->setId3(CS("TIT2"), title);
		somethingchanged = 1;
    }
    if (oldYear != year) {
//        ID3_AddYear(&m_newID3_Tag, (LPCTSTR) year, true);
		m_newSong->setId3(CS("TYER"), year);
		somethingchanged = 1;
    }
    if (oldTrack != track) {
        int t = atoi((LPCTSTR)track);
//        ID3_AddTrack(&m_newID3_Tag, t, 0, true);
		m_newSong->setId3(CS("TRCK"), track);
		somethingchanged = 1;
    }

	if (somethingchanged) {
		CDialog::OnOK();
	} else {
		CDialog::OnCancel();
	}
}

void ModifyIDThree::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}
