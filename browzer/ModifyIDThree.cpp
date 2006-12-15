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
	CString oldgenre = m_Song->getId3(CS("TCON"));

    oldgenre = Genre_normalize((LPCTSTR)oldgenre);

	CString oldartist = m_Song->getId3(CS("TPE1"));
	CString oldalbum = m_Song->getId3(CS("TALB"));
	CString oldtitle = m_Song->getId3(CS("TIT2"));
	CString oldtrack = m_Song->getId3(CS("TRCK"));
	CString oldyear = m_Song->getId3(CS("TYER"));
	CString file = m_Song->getId3("FILE");
	m_File.SetWindowText(file);
	CString newgenre, newartist, newalbum, newtitle, newtrack, newyear;
	newgenre = oldgenre;
	newartist = oldartist;
	newalbum = oldalbum;
	newtitle = oldtitle;
	newtrack = oldtrack;
	newyear = oldyear;

	oldgenre = "\"" + oldgenre + "\"";
	oldartist = "\"" + oldartist + "\"";
	oldalbum = "\"" + oldalbum + "\"";
	oldtitle = "\"" + oldtitle + "\"";
	oldtrack = "\"" + oldtrack + "\"";
	oldyear = "\"" + oldyear + "\"";

	if (MBALL == newartist) {
		newartist = "";
	}
	if (MBALL == newalbum) {
		newalbum = "";
	}
	if (MBALL == newgenre) {
		newgenre = "";
	}

	m_OldGenre.SetWindowText(oldgenre);
    POSITION pos;
	CString tmpGenre;
    for (pos = mGenreList->GetHeadPosition(); pos != NULL; ) {
        tmpGenre = mGenreList->GetAt(pos);
        m_Genre.AddString(tmpGenre);
        mGenreList->GetNext(pos);
    }
    m_Genre.SelectString(0, newgenre );

    int x = m_Genre.GetCount();

    if (mWindowFlag >= 1 /*&& genre != MBALL */) {
        m_Genre.EnableWindow(TRUE);
    }

    if (mWindowFlag >= 2) {
        m_Artist.EnableWindow(TRUE);
		m_Artist.SetWindowText(newartist);
		m_OldArtist.SetWindowText(oldartist);
//		if (artist == MBALL)
//			m_Artist.EnableWindow(FALSE);
    }
    if (mWindowFlag >= 3) {
        m_Album.EnableWindow(TRUE);
        m_Year.EnableWindow(TRUE);
		m_Album.SetWindowText(newalbum);
		m_OldAlbum.SetWindowText(oldalbum);
		m_Year.SetWindowText(newyear);
		m_OldYear.SetWindowText(oldyear);
//		if (album == MBALL) {
//			m_Album.EnableWindow(FALSE);
//		}
    }
    if (mWindowFlag >= 4) {
        m_Title.EnableWindow(TRUE);
        m_Track.EnableWindow(TRUE);
	    m_Title.SetWindowText(newtitle);
		m_OldTitle.SetWindowText(oldtitle);
		m_Track.SetWindowText(newtrack);
		m_OldTrack.SetWindowText(oldtrack);
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
	
	String::trimLR(oldGenre," \"");
	String::trimLR(oldArtist," \"");
	String::trimLR(oldAlbum," \"");
	String::trimLR(oldTitle," \"");
	String::trimLR(oldYear," \"");
	String::trimLR(oldTrack," \"");
	
	String::trimLR(genre);
	String::trimLR(artist);
	String::trimLR(album);
	String::trimLR(title);
	String::trimLR(year);
	String::trimLR(track);

	CString mball(MBALL);
	String::trimLR(mball);


	if ("" == genre || !genre.CompareNoCase(mball)) 
		genre = oldGenre;
	if ("" == artist ||!artist.CompareNoCase(mball)) 
		artist = oldArtist;
	if ("" == album || !album.CompareNoCase(mball)) 
		album = oldAlbum;

    if (oldGenre != genre) {
		m_newSong->setId3(CS("TCON"), genre);
		somethingchanged = 1;
    }
    if (oldArtist != artist) {
		m_newSong->setId3(CS("TPE1"), artist);
		somethingchanged = 1;
    }
    if (oldAlbum != album) {
		m_newSong->setId3(CS("TALB"), album);
		somethingchanged = 1;
    }
    if (oldTitle != title) {
		m_newSong->setId3(CS("TIT2"), title);
		somethingchanged = 1;
    }
    if (oldYear != year) {
		m_newSong->setId3(CS("TYER"), year);
		somethingchanged = 1;
    }
    if (oldTrack != track) {
        int t = atoi((LPCTSTR)track);
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
	
	CDialog::OnCancel();
}
