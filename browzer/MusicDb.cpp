
#include "stdafx.h"
#include "MusicDb.h"
#include "MyLog.h"
//#include "Genres.h"
#include "MyString.h"
#include "id3/tag.h"
#include "id3/misc_support.h"
//#include "id3lib-3.8.0/src/mp3_header.h"
#include "FExtension.h"
#include "Registry.h"
//#include "ID3ModifyStatus.h"
#include "MBMessageBox.h"
#include "MyID3LibMiscSupport.h"
#include "Mp3Header.h"
#include "TestHarness/TestHarness.h"
#include "FileUtils.h"
#include "WmaTagger.h"
#include "MyID3LibMiscSupport.h"

#include "vorbis/codec.h"
#include "vorbis/vorbisfile.h"
#include "oggtagger/oggtagger.h"
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "util/Misc.h"

#pragma hack
// xxx make the increment large before shipping!
#define MMEMORY_SIZE_INCREMENT 5000
#define MMEMORY_RESERVE_BYTES 100
#define MB_GARBAGE_INTERVAL 10
#define MB_DB_VERSION 4

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/////////////////////////////////////////////////////////////////////////////////

TEST(CSongFunctions, CSong)
{
//	CHECK_LONGS_EQUAL( 0, 1);

    Song song = new CSong;
	char label1[] = "lone";
	char val1[] = "val";
	char label2[] = "ltwo";
	char val2[] = "val";
	char label3[] = "label3";
	char val3[] = "val3";
	
    song->setId3(label1, val1);
	song->setId3(label2, val2);
	song->setId3(label3, val3);
	CString Val1 = song->getId3(label1);
	CString Val2 = song->getId3(label2);
	CString Val3 = song->getId3(label3);
	CHECK(Val1 == Val2);
	CHECK(Val1 != Val3);
	song->removeId3(label1);
	song->removeId3(label2);
	song->removeId3(label3);
	CString Val4 = song->getId3(label1);
	CHECK(Val4 == "");

}



CSong::CSong(): _refcnt(0) {

}
CSong::CSong(Song & src): _refcnt(0)  {
    POSITION pos;
    CString key;
    CString val;
    for( pos = src->_obj.GetStartPosition(); pos != NULL; ) {
        src->_obj.GetNextAssoc(pos, key, val);
        if (key.GetLength() && val.GetLength()) {
            _obj.SetAt(key, (LPCTSTR)val);
        }
    }
}

CSong::~CSong() {
#ifdef asdf
    POSITION pos;
    CString key;
    CString val;
    CObject * cval;
    for( pos = _obj.GetStartPosition(); pos != NULL; ) {
        _obj.GetNextAssoc(pos, key, val);
        BOOL r = _obj.Lookup(key,  cval );
        if (r == TRUE) {
            delete cval;
        }
    }
#endif
}

void
CSong::unreference() {
	if (--_refcnt ==0) {
		delete this;
	}
}

#pragma hack
//int
//CSong::operator == (const CSong &a) {
//	// xxx fix this, compare all keys
//	return *this == a;
//}

int
CSong::setId3(const CString & key, const CString & val) {

    if (key.GetLength() && val.GetLength()) {
        _obj.SetAt(key, (LPCTSTR)val);
    }
    return 0;
}

CString
CSong::getId3(const CString & key, int unknown) {

    CString val;
    if (_obj.Lookup(key, val) == 0) { // not found
		val = "";
	}
    if (unknown && val.GetLength() == 0 
		&& (key=="TCON" || key=="TPE1" || key=="TALB"|| key=="TIT2")) {
            val = MBUNKNOWN;
    }
//      setId3(key,val);
    return val;
}

int
CSong::removeId3(const CString & key) {

    if (_obj.RemoveKey(key) == 0) {
        return -1;
    }
    return 0;
}

SongKeys *
CSong::createSongKeys() {
    SongKeys * p = new SongKeys ();
    POSITION pos;
    CString key;
    CString val;
    for( pos = _obj.GetStartPosition(); pos != NULL; ) {
        _obj.GetNextAssoc(pos, key, val);
        if (key.GetLength() && val.GetLength()) {
            p->SetAt(key, (LPCTSTR)val);
        }
    }
    return p;
}
BOOL
CSong::Contains(const CString & keyword) {
	CString kw(keyword);
	kw.MakeLower();
    CString key;
    CString val;

	if (_obj.Lookup("TIT2",val)) {
		val.MakeLower();
		if (-1 < val.Find(kw))
			return TRUE;
	}
	if (_obj.Lookup("TALB",val)) {
		val.MakeLower();
		if (-1 < val.Find(kw))
			return TRUE;
	}
	if (_obj.Lookup("TPE1",val)) {
		val.MakeLower();
		if (-1 < val.Find(kw))
			return TRUE;
	}
	if (_obj.Lookup("TCON",val)) {
		val.MakeLower();
		if (-1 < val.Find(kw))
			return TRUE;
	}

	return FALSE;

}

/////////////////////////////////////////////////////////////////////////////////

MusicLib::MusicLib(InitDlg *id): _id(id), m_totalMp3s(0),
	m_Searching(FALSE)
{
//	init();
}

MusicLib::~MusicLib() {
}

void
MusicLib::readDbLocation() {
    RegistryKey reg( HKEY_LOCAL_MACHINE, RegKey );
    AutoBuf buf(1000);
    reg.Read(RegDbLocation, buf.p, 999, "");
    setDbLocation(buf.p);
}
void 
MusicLib::setDbLocation(const CString & loc) {
	m_dir = loc;
	m_file = loc;
	m_file += "\\";
	m_file += MUZIKBROWZER;
	m_file += ".db";
	m_SongLib.setDbLocation(loc);
}
int
MusicLib::init() {
    
	m_mp3Extensions.RemoveAll();
	m_mp3Extensions.AddTail("mp3");
	m_mp3Extensions.AddTail("wma");
	m_mp3Extensions.AddTail("ogg");

	readDbLocation();
	Genre_init();

	m_SongLib.init(TRUE);
	int r = readDb();
	_id = 0;
	m_libCounts = "";
	return r;

}

int
MusicLib::deleteSongFromPlaylist(PlaylistNode *p) {
    return _playlist.remove(p);

}

static void
insertSort(CStringList &list, const CString &string) {
    POSITION pos = list.GetHeadPosition();
    if (pos == NULL) {
        list.AddTail(string);
        return;
    }
    bool inserted = false;
    for (pos = list.GetHeadPosition(); pos != NULL; ) {
        CString lstring = list.GetAt(pos);

        // If string > lstring ...
        if (string.CompareNoCase(lstring) == -1) {
            list.InsertBefore(pos, string);
            inserted = true;
            break;
        }
        list.GetNext(pos);
    }
    if (!inserted)
        list.AddTail(string);
}

TEST(insertSortTest, insertSort)
{
	CStringList list;
	insertSort(list, CString("abc"));
	insertSort(list, CString("1bc"));
	insertSort(list, CString("xbc"));
	insertSort(list, CString("zbc"));
	insertSort(list, CString("rbc"));
	insertSort(list, CString("dbc"));
    POSITION pos = list.GetHeadPosition();
	CString lastone,string;
	int first = 1;
    for (pos = list.GetHeadPosition(); pos != NULL; ) {
		lastone = string;
        string = list.GetAt(pos);
        list.GetNext(pos);
		if (first) {
			first = 0;
		} else {
			CHECK(lastone < string);
		}
    }
	CHECK(lastone < string);
}

UINT
MusicLib::readDb() {
	m_SongLib.readFromFile();
// commenting out cause the dialog is null at start up
// I don't think I need to garbageCollect at StartUp anyway
//	if (garbageCollect(_id))
//		writeDb();
	return 0;
}

UINT
MusicLib::writeDb() {
	m_SongLib.writeToFile();
    return 0;
}

UINT
MusicLib::addSongToDb(int & ctr, Song &song, const CString & file) {
	CString dir;

	static CString lastdir;

	if (_id) {
		ctr++;
		dir = FileUtil::dirname(file);

		if (lastdir.Compare(dir) != 0) {
			lastdir = dir;
            _id->SendUpdateStatus(1, dir, 0,0);
		}
	}

	return m_SongLib.addSong(song);
}
void
MusicLib::MovePlaylistsToDir() {

	CString errormsg;
	CStringList plist;
	CString oldpdir(m_dir);
	CString newpdir(oldpdir);
	newpdir += "\\" + CS(MBPLAYLISTDIR);
	if (!FileUtil::DirIsWriteable(newpdir)) {
		FileUtil::mkdirp(newpdir);
	}
    CString glob(m_dir);
    glob += "\\";
    glob += MUZIKBROWZER;
	glob += MBPLAYLIST;
    glob += "*";
	glob += MBPLAYLISTEXT;
    CFileFind finder;
    BOOL bWorking = finder.FindFile(glob);
	// get old playlists from mb dir
	int oldtypecount = 0;
    while (bWorking)
    {
        bWorking = finder.FindNextFile();
        CString fname = m_dir;
		fname += "\\" + finder.GetFileName();
        CString mbname = MUZIKBROWZER;
        mbname += MBPLAYLIST;
        plist.AddTail(fname);
		oldtypecount++;
    }
	finder.Close();

	// now convert to m3u
	POSITION pos;
    for (pos = plist.GetHeadPosition(); pos != NULL; ) {
        CString srcpl = plist.GetAt(pos);
		CString dstpl = newpdir;
		FExtension fext(srcpl);
		CStringList playlist;
		CString left;
		left += "\\";
		left += MUZIKBROWZER;
		left += MBPLAYLIST;
		CString right = MBPLAYLISTEXT;
		CString name = String::extract(srcpl,left,right);
		CString bakpl = newpdir + "\\" + name + MBPLAYLISTEXT;
		dstpl += "\\" + name + "." + MBPLAYLISTM3U;
		if (!loadOldPlaylist(name, playlist)) {
			errormsg += "Unable to read playlist\r\n";
			errormsg += srcpl;
			errormsg += "\r\n";
			break;
		}
		CFile myFile2;
		CFileException fileException;
		if ( !myFile2.Open( dstpl,
			CFile::modeWrite | CFile::modeCreate | CFile::typeBinary,
			&fileException ))
		{
			errormsg += "Unable to convert playlist ";
			errormsg += "\r\n";
			errormsg += dstpl;
			errormsg += "\r\n";
			break;
		}
		POSITION pos2;
		for (pos2 = playlist.GetHeadPosition(); pos2 != NULL; ) {
			CString songfile = plist.GetAt(pos2);
			myFile2.Write(songfile.GetBuffer(0),songfile.GetLength());
			myFile2.Write("\r\n",2);
			playlist.GetNext(pos2);

		}
		myFile2.Close();

		// We've successfully converted mbp to m3u. So move the old one
		// into the new dir and we won't try converting again, but we
		// keep it just in case.
		logger.log("moved ", srcpl, " to ", bakpl);
		if (FileUtil::mv(srcpl,bakpl)) {
			LONG e = ::GetLastError();
			logger.log(MBFormatError(e));
		}


        plist.GetNext(pos);
    }
	CString src,dst;
	src = m_dir + "\\muzikbrowzer.files";
	dst = m_dir + "\\muzikbrowzer.mbfls";
	if (!FileUtil::IsReadable(dst))
		FileUtil::mv(src,dst);

	CString msg = "This version of muzikbrowzer converts your old\r\n";
	msg += "playlists into a new format.\r\n\r\n";
	if (errormsg.GetLength()) {
		msg += "Errors encountered during playlist conversion.\r\n";
		msg += "Make a copy of the muzikbrowzer.log file below and\r\n";
		msg += "contact support. " + CS(MBURL);
	} else {
		msg += "Your playlists have been converted to the new industry\r\n";
		msg += "standard m3u format and placed in\r\n\r\n";
		msg += newpdir;
	}
	if (oldtypecount || errormsg.GetLength())
		MBMessageBox("Notice", msg);

}
int
MusicLib::loadOldPlaylist(const CString & name, CStringList & plist) {

    CString dbfilename(m_dir);
    dbfilename += "\\";
    dbfilename += MUZIKBROWZER;
    dbfilename += MBPLAYLIST;
    dbfilename += name;
    dbfilename += MBPLAYLISTEXT;
    const char * pszFileName = (LPCTSTR) dbfilename;
	CFile myFile;
	CFileException fileException;
    int count1 = 0;
    int count3 = 0;

	if ( !myFile.Open( pszFileName,
        CFile::modeRead,
        &fileException ))
	{
        CString msg = "Unable to read playlist ";
		msg += pszFileName;
		msg += msg;
		msg += "\r\n";
        return FALSE;
	}
    if (myFile.GetLength()) {
        AutoBuf buf(myFile.GetLength()+1);
        myFile.Read(buf.p, myFile.GetLength());
        CString genre, artist, album, title, file;
        char * p = buf.p;
        while (p < buf.p + myFile.GetLength()) {
            genre = p;
            p += genre.GetLength()+1;
            artist = p;
            p += artist.GetLength()+1;
            album = p;
            p += album.GetLength()+1;
            title = p;

			// look for file name present. If version > 1.4.0 it should
			// be there, else not. Maybe just go with; if file name ain't
			// there, blow it off.
			// p shoule either be a newline or file name
            p += title.GetLength()+1;
			file = "";
			if ('\r' != *p && '\n' != *p) {
				file = p;
				p += file.GetLength()+1;
			}

			// First get to the eol
            while ((p < buf.p + myFile.GetLength())
                && (p[0] != '\r' && p[0] != '\n'))
                p++;
			// Now skip past any extra crlf's
            while ((p < buf.p + myFile.GetLength())
                && (p[0] == '\r' || p[0] == '\n'))
                p++;
     
			if ("" == file) {
				Song song = new CSong;
				song = getSong(genre,artist,album,title);
				file = song->getId3("FILE");
			}
			if ("" != file) {
				plist.AddTail(file);
			}

        }
    }
    myFile.Close();
	return TRUE;
}

int
MusicLib::getPlaylistNames(CExtendedListBox & box) {
	CStringList cslList;
	getPlaylistNames(cslList);
	POSITION pos;
    for (pos = cslList.GetHeadPosition(); pos != NULL; ) {
        CString lstring = cslList.GetAt(pos);
		box.AddString(lstring);
        cslList.GetNext(pos);
	}
	return 0;
}
void sortPlaylist(CStringList & cslList) {
	POSITION pos,pcur,pprev;
	pcur = pprev = NULL;
    for (pos = cslList.GetHeadPosition(); pos != NULL; ) {
        CString lstring = cslList.GetAt(pos);
		if ("Last" == lstring) 
			pcur = pos;
		if ("Previous" == lstring)
			pprev = pos;
        cslList.GetNext(pos);
	}
	if (pprev) {
		cslList.RemoveAt(pprev);
		cslList.AddHead("Previous");
	}
	if (pcur) {
		cslList.RemoveAt(pcur);
		cslList.AddHead("Last");
	}

}
int
MusicLib::getPlaylistNames(CStringList & box) {
    CString glob(m_dir);
    glob += "\\" + CS(MBPLAYLISTDIR) + "\\*." + CS(MBPLAYLISTM3U);
    CFileFind finder;
    BOOL bWorking = finder.FindFile(glob);
    while (bWorking)
    {
        bWorking = finder.FindNextFile();
        CString fname = finder.GetFileName();
		FExtension fext(fname);
		insertSort(box,FileUtil::basename(fext.filename()));
    }
	finder.Close();
	sortPlaylist(box);

    return 0;
}
int
MusicLib::getSongsInPlaylist(const CString & name, CExtendedListBox & box) {
	CStringArray list,playlist;
	CDWordArray colTlen;
	getSongsInPlaylist(name, list, playlist, colTlen);
//	POSITION pos;
	int pos,size;
	size = list.GetSize();
	for(pos = 0 ; pos < size ; pos++) {
//    for (pos = list.GetHeadPosition(); pos != NULL; ) {
        CString lstring = list.GetAt(pos);
		box.AddString(lstring);
//        list.GetNext(pos);
	}
	return 0;
}

int
MusicLib::getSongsInPlaylist(const CString & name, 
							 CStringArray & cslDesc,
							 CStringArray & cslPlaylist,
							 CDWordArray & cwaTlen) {

    CString dbfilename(m_dir);
    dbfilename += "\\" + CS(MBPLAYLISTDIR) + "\\";
    dbfilename += name;
	dbfilename += ".";
    dbfilename += MBPLAYLISTM3U;
    const char * pszFileName = (LPCTSTR) dbfilename;
	CFile myFile;
	CFileException fileException;

	if ( !myFile.Open( pszFileName,
        CFile::modeRead,
        &fileException ))
	{

        CString msg = "Unable to read playlist ";
		msg += dbfilename;
        MBMessageBox(CString("alert"), msg);
        return -1;
	}
    if (myFile.GetLength()) {
        AutoBuf buf(myFile.GetLength()+1);
        myFile.Read(buf.p, myFile.GetLength());
		buf.p[myFile.GetLength()] = 0;
        CString genre, artist, album, song;
        char * p = buf.p;
		char * end = (buf.p + myFile.GetLength());
		
		p = strtok(buf.p,"\r\n");
        while (NULL != p) {
			int tlen = 0;
			if (strlen(p)) {
				CString file(p);
				CString disp;
				if (FileUtil::IsReadable(file)) {
					Song song = m_SongLib.m_files.getSong(file);
					if (song->GetCount()) {
						disp = song->getId3("TIT2");
						disp += " by ";
						disp += song->getId3("TPE1");
						disp += " on ";
						disp += song->getId3("TALB");
						disp += " in ";
						disp += song->getId3("TCON");
						tlen = atoi(song->getId3("TLEN"));
					} else {
						disp = "not in library: " + file;
					}
				} else {
					disp = "unreadable: " + file;
				}
				cslDesc.Add(disp);
				cslPlaylist.Add(file);
				cwaTlen.Add(tlen);
			}
			p = strtok(NULL,"\r\n");
        }
    }
	myFile.Close();
    return 0;
}
int
MusicLib::loadPlaylist(const CString & name, CString & error_msg) {

    CString dbfilename(m_dir);
    dbfilename += "\\" + CS(MBPLAYLISTDIR) + "\\";
    dbfilename += name;
	dbfilename += ".";
    dbfilename += MBPLAYLISTM3U;
    const char * pszFileName = (LPCTSTR) dbfilename;

	CFile myFile;
	CFileException fileException;
	int ecount = 0;

	if ( !myFile.Open( pszFileName,
        CFile::modeRead,
        &fileException ))
	{

        CString msg = "Unable to read playlist ";
		msg += dbfilename;
        MBMessageBox(CString("alert"), msg);
        return -1;
	}
    if (myFile.GetLength()) {
        AutoBuf buf(myFile.GetLength()+1);
        myFile.Read(buf.p, myFile.GetLength());
		buf.p[myFile.GetLength()] = 0;
        CString genre, artist, album, song, file;
        char * p = buf.p;
		char * end = (buf.p + myFile.GetLength());

		p = strtok(buf.p,"\r\n");
        while (NULL != p) {
			if (strlen(p) && '\r' != p[0] && '\n' != p[0]) {
				Song s = m_SongLib.m_files.getSong(p);
				if (s->GetCount())
					addSongToPlaylist(s);
				else {
					if (addFileToPlaylist(p) == 0) {
						error_msg += "not found: " + CS(p);
						if (0 == ecount)
							ecount++;
					}
				}
			}
			p = strtok(NULL,"\r\n");
        }
    }
    myFile.Close();

    return ecount;

}
int
MusicLib::getGenres(CExtendedListBox & box) {
	MList genreList = m_SongLib.genreList();
	MList::Iterator genreIter(genreList);
	
	while (genreIter.more()) {
		MRecord genre = genreIter.next();
		box.AddString(genre.label());;
	}
	return 0;
}
int
MusicLib::getGenres(CStringList & box) {
    CMapStringToString genrehash;
	MList genreList = m_SongLib.genreList();
	MList::Iterator genreIter(genreList);
	
	while (genreIter.more()) {
		MRecord genre = genreIter.next();
		genrehash.SetAt(genre.label(), 0);
	}

    CStringList glist;
    Genre_getGenres(glist);

    POSITION pos = glist.GetHeadPosition();
    for (pos = glist.GetHeadPosition(); pos != NULL; ) {
        CString genre = glist.GetAt(pos);
        genrehash.SetAt(genre, "");
        glist.GetNext(pos);
    }
    CString key;
    CString val;
    for( pos = genrehash.GetStartPosition(); pos != NULL; ) {
        genrehash.GetNextAssoc(pos, key, val);
        insertSort(box, key);
    }

	return 0;
}
int
MusicLib::getArtists(const CString & genrename, CExtendedListBox & box) {
	MList artistList = m_SongLib.artistList(genrename);
	MList::Iterator artistIter(artistList);
	
	while (artistIter.more()) {
		MRecord artist = artistIter.next();
		box.AddString(artist.label());
	}

	return 0;
}
class NameNum : public CObject {
public:
    NameNum(CString name, int num, int p=0) : m_name(name), m_num(num),
        m_p(p) {}
    CString m_name;
    int m_num;
    int m_p;
    static void bsort(CObArray & array, BOOL alpha = FALSE) {
        int size = array.GetSize();
        if (size < 2) return;
        int i,j;
        for (i = 0 ; i < size-1 ; ++i) {
            for (j = size-1 ; j > i ; --j) {
                NameNum * nnj = (NameNum*)array[j];
                NameNum * nnjm1 = (NameNum*)array[j-1];
                if (
					(alpha && nnj->m_name.CompareNoCase(nnjm1->m_name) < 0)
					|| (!alpha && nnj->m_num <= nnjm1->m_num)
					)
                {
                    CString tmpName = nnj->m_name;
                    int tmpNum = nnj->m_num;
                    int tmpp = nnj->m_p;

                    nnj->m_name = nnjm1->m_name;
                    nnj->m_num = nnjm1->m_num;
                    nnj->m_p = nnjm1->m_p;

                    nnjm1->m_name = tmpName;
                    nnjm1->m_num = tmpNum;
                    nnjm1->m_p = tmpp;
                }
            }
        }
    }
};
class SongNameNum : public CObject {
public:
    SongNameNum(Song song, CString name, int num) : m_song(song),
            m_name(name), m_num(num) {}
    Song m_song;
    CString m_name;
    int m_num;
    static void bsort(CObArray & array) {
        int size = array.GetSize();
        if (size < 2) return;
        int i,j;
        for (i = 0 ; i < size-1 ; ++i) {
            for (j = size-1 ; j > i ; --j) {
                SongNameNum * nnj = (SongNameNum*)array[j];
                SongNameNum * nnjm1 = (SongNameNum*)array[j-1];
                if ((nnj->m_num < nnjm1->m_num)
                    || (nnj->m_num == nnjm1->m_num
                    && (nnj->m_name.CompareNoCase(nnjm1->m_name)) < 0))
                {
                    Song tmpSong = nnj->m_song;
                    CString tmpName = nnj->m_name;
                    int tmpNum = nnj->m_num;

                    nnj->m_song = nnjm1->m_song;
                    nnj->m_name = nnjm1->m_name;
                    nnj->m_num = nnjm1->m_num;

                    nnjm1->m_song = tmpSong;
                    nnjm1->m_name = tmpName;
                    nnjm1->m_num = tmpNum;
                }
            }
        }
    }
};
int
MusicLib::getAlbums(const CString & genrename, const CString & artistname,
				   CExtendedListBox & box, BOOL AlbumSortAlpha) {

	MList albumList = m_SongLib.albumList(genrename, artistname);
	MList::Iterator albumIter(albumList);

    CObArray namenums;
    while (albumIter.more()) {
		MRecord album = albumIter.next();
		MList songs(album);
		MList::Iterator songIter(songs);
		if (songIter.more()) {
			MRecord firstSong = songIter.next();
			CString year = firstSong.lookupVal("TYER");
			int iyear = 0;
			if (year != "") {
				iyear = atoi((LPCTSTR)year);
			}
			NameNum * nn = new NameNum(album.label(), iyear);
			namenums.Add(nn);
		}
	}
	if (artistname == MBALL || AlbumSortAlpha) {
		NameNum::bsort(namenums, TRUE);
	} else {
		NameNum::bsort(namenums, FALSE);
	}
    int i;
    for (i = 0 ; i < namenums.GetSize(); ++i) {
        box.AddString(((NameNum*)namenums[i])->m_name);
        delete (NameNum*)namenums[i];
    }
	return 0;
}
int
MusicLib::getSongs(const CString & genrename,
				  const CString & artistname,
				  const CString & albumname,
				  CExtendedListBox & box) {
//	MBAutoTimer t("MusicLib::getSongs");
	MList songList = m_SongLib.songList(genrename, artistname, albumname);
	MList::Iterator songIter(songList);
    CObArray namenums;
    //namenums.SetSize(10);
    while (songIter.more()) {
		MRecord song = songIter.next();
		CString track = song.lookupVal("TRCK");
        int itrack = 0;
        if (track != "") {
            itrack = atoi((LPCTSTR)track);
        }
        NameNum * nn = new NameNum(song.label(), itrack, song.i());
        namenums.Add(nn);
    }
    NameNum::bsort(namenums);
    int i;
    for (i = 0 ; i < namenums.GetSize(); ++i) {
        box.AddString(((NameNum*)namenums[i])->m_name);
        box.SetItemData(i, ((NameNum*)namenums[i])->m_p);
        delete (NameNum*) namenums[i];
	}

	return 0;
}

int
MusicLib::getPlaylist(CExtendedListBox & box) {
	for (PlaylistNode *p = _playlist.head();
			p != (PlaylistNode*)0;
			p = _playlist.next(p)) {
		CString buf;

//		buf = p->_item->getId3("TPE1");
//		buf += " / ";
//		buf += p->_item->getId3("TALB");
//		buf += " / ";
//		buf += p->_item->getId3("TIT2");

		buf = p->_item->getId3("TIT2");
		buf += " by ";
		buf += p->_item->getId3("TPE1");
		buf += " on ";
		buf += p->_item->getId3("TALB");
		buf += " in ";
		buf += p->_item->getId3("TCON");


		int sel = box.AddString(buf);
//        SongKeys * sk = p->_item->createSongKeys();
        box.SetItemDataPtr(sel, (void *) p);
//        logger.log((LPCTSTR)buf);
	}
	return 0;
}

Song
MusicLib::getSong(const CString & genre, const CString & artist,
				const CString & album, const CString & title) {
	MRecord songr = m_SongLib.getSong(genre, artist, album, title);
	Song song = new CSong;
	song->setId3("TCON", genre);
	song->setId3("TPE1", artist);
	song->setId3("TALB", album);
	song->setId3("TIT2", title);
	MList songKVL(songr);
	MList::Iterator songKViter(songKVL);
	while (songKViter.more()) {
		MRecord kv = songKViter.next();
		song->setId3(kv.getKey(), kv.getVal());
	}

    return song;
}
Song
MSongLib::getSong(int pi) {
	MRecord r(m_mem, pi);
	return r.createSong();
}

int
MusicLib::addFileToPlaylist(const CString & file) {
	if (FileUtil::IsReadable(file)) {
		Song addsong = createSongFromFile(file);
		if (addsong->GetCount()) {
			addSongToPlaylist(addsong);
			return 1;
		} 
	}
	return 0;
}
int
MusicLib::addSongToPlaylist(const Song & song) {
	_playlist.append(song);
	return 1;
}

int
MusicLib::addSongToPlaylist(const CString & genrename,
		const CString & artistname, const CString & albumname,
		const CString & songname) {
    int count = 0;
	MList songList = m_SongLib.songList(genrename, artistname, albumname);
	MList::Iterator songIter(songList);
    while (!count && songIter.more()) {
		MRecord songRec = songIter.next();
        if (songRec.label() == songname) {
            count += addSongToPlaylist(songRec.createSong());
        }
    }
	return count;
}

int
MusicLib::addAlbumToPlaylist(const CString & genrename,
		const CString & artistname, const CString & albumname) {
	int count = 0;
	MList songList = m_SongLib.songList(genrename, artistname, albumname);
	MList::Iterator songIter(songList);

    CObArray snamenums;
    while (songIter.more()) {
        Song song = songIter.next().createSong();
        CString title = song->getId3("TIT2");
        CString tracks = song->getId3("TRCK");
        int track = atoi((LPCTSTR)tracks);
        SongNameNum * snn = new SongNameNum(song, title, track);
        snamenums.Add(snn);
    }
    SongNameNum::bsort(snamenums);
    int i;
    for (i = 0; i < snamenums.GetSize(); ++i) {
        addSongToPlaylist(((SongNameNum*)snamenums[i])->m_song);
        delete snamenums[i];
    }
	return 0;
}

int
MusicLib::addArtistToPlaylist(const CString & genrename,
		const CString & artistname) {

	MList albumList = m_SongLib.albumList(genrename, artistname);
	MList::Iterator albumIter(albumList);

    CObArray namenums;
    while (albumIter.more()) {
		MRecord album = albumIter.next();
		MList songs(album);
		MList::Iterator songIter(songs);
		if (songIter.more()) {
			MRecord firstSong = songIter.next();
			CString year = firstSong.lookupVal("TYER");
			int iyear = 0;
			if (year != "") {
				iyear = atoi((LPCTSTR)year);
			}
	        NameNum *snn = new NameNum(album.label(), iyear);
		    namenums.Add(snn);
		}
    }
    NameNum::bsort(namenums);
    int i;
    for (i = 0; i < namenums.GetSize(); ++i) {
        addAlbumToPlaylist(genrename, artistname,
            ((NameNum*)namenums[i])->m_name);
        delete namenums[i];
    }
	return 0;
}

int
MusicLib::addGenreToPlaylist(const CString & genrename) {
	MList artistList = m_SongLib.artistList(genrename);
	MList::Iterator artistIter(artistList);
	while (artistIter.more()) {
		addArtistToPlaylist(genrename, artistIter.next().label());
	}
	return 0;
}

int
MusicLib::clearPlaylist() {
    return _playlist.reset();
}

CString
MusicLib::scanDirectories(const CStringList & directories,
						  InitDlg * initDlg, BOOL scanNew, BOOL bAdd) {
	SearchClear();
    CStringList mp3Files,mp3Extensions;
    CString good_results, error_results;
    _id = initDlg;
    AutoBuf buf(1000);

	if (bAdd || scanNew) {
		// if no m_files present have to start from scratch
		if (m_SongLib.m_files.read() == -1) {
			m_SongLib.init(TRUE);
		}
	} else {
		m_SongLib.init(TRUE); // does an m_files.removeAll()
	}
    
    POSITION pos;
    int * abortflag = &(InitDlg::m_Abort);
    _id->SendUpdateStatus(0, "Scanning directories for audio files", 0,0);
    for (pos = directories.GetHeadPosition(); pos != NULL; ) {
        scanDirectory(abortflag, mp3Files, directories.GetAt(pos), 
			scanNew, bAdd);
        directories.GetNext(pos);
        if (InitDlg::m_Abort) {
            error_results += "Aborted by user.";
			return error_results;
            break;
        }
    }
    m_totalMp3s = mp3Files.GetCount();

	// now done with m_files
	m_SongLib.m_files.write();
	// No longer clearing it, gonna be using it
//	m_SongLib.m_files.removeAll();

    _id->SendUpdateStatus(2, "", 0, m_totalMp3s);

    int good_count = 0;
    int ctr = 0;
	int tlen_method_1_count = 0;
	int tlen_method_2_count = 0;
	int tlen_methods_failed_count = 0;
	int added_count = 0;
	CString added = "Files added:\r\n";

    _id->SendUpdateStatus(0, "Reading tag info from audio files", 0,0);
    for (pos = mp3Files.GetHeadPosition(); pos != NULL; ) {
        CString mp3file = mp3Files.GetAt(pos);
        _id->SendUpdateStatus(3, "", ctr, 0);
        if (InitDlg::m_Abort) {
            error_results += "Aborted by user.";
            break;
        }
		Song song = createSongFromFile(mp3file,error_results,
			tlen_method_1_count, tlen_method_2_count, 
			tlen_methods_failed_count);
		CString genre = song->getId3((CString)"TCON");
        CString artist = song->getId3((CString)"TPE1");
        CString album = song->getId3((CString)"TALB");
        CString songn = song->getId3((CString)"TIT2");
        if (genre == MBUNKNOWN || artist == MBUNKNOWN
                || album == MBUNKNOWN || songn == MBUNKNOWN) {
			//			if (songn == MBUNKNOWN) {
			//				song->setId3("TIT2", mp3file);
			//				songn = mp3file;
			//			}
            sprintf(buf.p,
                "%s: audio tag missing field(s), added as: '%s / %s / %s / %s'\r\n",
                (LPCTSTR)mp3file, (LPCTSTR)genre,(LPCTSTR)artist,
				(LPCTSTR)album,(LPCTSTR)songn);
            error_results += buf.p;

        } else {
            ++good_count;
        }

		added_count += addSongToDb(ctr, song, mp3file);
		added += mp3file + "\r\n";
		mp3Files.GetNext(pos);
    }

    int num_albums1,num_songs1, num_albums2, num_songs2;
    num_albums1 = num_songs1 = num_albums2 = num_songs2 = 0;
	CString msg;
	if (scanNew) {
		msg = "Looking for removed/renamed files";
	} else {
		msg = "Verifying database";
	}
//	_id->ShowWindow(SW_HIDE);

    CString verify_results1 = m_SongLib.verify(msg, num_albums1, num_songs1);

    writeDb();

    CString results;
	if (scanNew) {
		results += "Scanned for new audio files - add to database.\r\n";
	} else {
		results += "Scanned for audio files - complete database build.\r\n";
	}
	if (added_count) {
		results += numToString(added_count);
		results += " audio files added.\r\n";
		if (scanNew)
			results += added;
	} else {
		results += "No new audio files found.\r\n";
	}

	results += good_results;
    results += error_results;
    results += verify_results1;

    if (mp3Files.GetCount() == good_count) {
		if (added_count) {
			sprintf(buf.p, 
			"\r\nAll %d audio files found with good/complete audio tags\r\n",
			good_count);
			results += buf.p;
		}
	} else {
		sprintf(buf.p, 
			"\r\n%d audio files found, %d had incomplete audio tags\r\n",
			mp3Files.GetCount(), mp3Files.GetCount()-good_count);
		results += buf.p;
	}

#ifdef _DEBUG
	results += "\r\ntlen_method_1_count: ";
	results += tlen_method_1_count;
	results += "\r\ntlen_method_2_count: ";
	results += tlen_method_2_count;
	results += "\r\ntlen_methods_failed_count: ";
	results += tlen_methods_failed_count;
	results += "\r\n";
#endif
	m_libCounts = "";
	IgetLibraryCounts();

    return results;
}

int
MusicLib::scanDirectory(int * abortflag, CStringList &mp3Files,
					   const CString & directory, BOOL scanNew, BOOL bAdd) {

	// this kludge added so that; if file names appear in the mp3dirlist
	// we'll add 'em. This can happen via Menu/Music/Add
	FExtension ext(directory);
	if (m_mp3Extensions.Find(String::downcase(ext.ext())) != NULL) {
		insertSort(mp3Files, directory);
		return 0;
	} // end of kludge

    CString glob(directory);
	if (glob[glob.GetLength()-1] == '\\') {
		glob += "*.*";
	} else {
		glob += "\\*.*";
	}
    
	if (_id) {
        _id->SendUpdateStatus(1, directory, 0,0);
	}

    CFileFind finder;
    BOOL bWorking = finder.FindFile(glob);
	DWORD r;
	if (!bWorking) {
		r = GetLastError();
	}
	CTime mtime;
    while (bWorking)
    {
        if (*abortflag) {
            return 0;
        }
        bWorking = finder.FindNextFile();
        CString fname = finder.GetFileName();
        if (fname.Left(1) != "." && !finder.IsDots()) {
            if (finder.IsDirectory()) {
                CString newDir(directory);
				if (newDir[newDir.GetLength()-1] != '\\') {
					newDir += "\\";
				}
                newDir += fname;
                scanDirectory(abortflag, mp3Files, newDir, scanNew, bAdd);
            } else {
//				BOOL doit = TRUE;
//				if (scanNew) {
//					finder.GetLastWriteTime(mtime);
//					if (mtime.GetTime() > scanNew) {
//						doit = TRUE;
//					} else {
//						doit = FALSE;
//					}
//				}
//				if (doit) {
				if ((!scanNew && !bAdd) ||
						!m_SongLib.m_files.contains(finder.GetFilePath())) {
					FExtension ext(fname);
					if (m_mp3Extensions.Find(String::downcase(ext.ext())) != NULL) {
	                    insertSort(mp3Files, finder.GetFilePath());
		            }
				}
            }
        }
    }
	finder.Close();

    return 0;

}
Song
MusicLib::createSongFromFile(const CString & mp3file) {
	CString er;
	int t1, t2, tf;
	t1 = t2 = tf = 0;
	return createSongFromFile(mp3file, er, t1, t2, tf);
}
Song
MusicLib::createSongFromFile(const CString & mp3file,
	CString & error_results, int & tlen_method_1_count,
	int & tlen_method_2_count, int & tlen_methods_failed_count)
{
	AutoBuf buf(1000);
	CString tlen;
	Song song = new CSong;
	FExtension fext(mp3file);
	if (fext == "mp3") {

		ID3_Tag * id3 = new ID3_Tag;
		size_t tagsize = id3->Link(mp3file, ID3TT_ALL);
		song = createSongFromId3(id3);

		if (!id3->HasV1Tag() && !id3->HasV2Tag()) {
			sprintf(buf.p, "%s: No ID3 tag\r\n", (LPCTSTR)mp3file);
			error_results += buf.p;
		}

		// If TLEN is set use it
		tlen = song->getId3((CString)"TLEN");

		if (tlen.GetLength() == 0) { // 1st try id3lib
			const Mp3_Headerinfo* mp3i = id3->GetMp3HeaderInfo();
			Mp3Header mp3;
			if (mp3i && mp3i->time) {
				tlen = numToString(mp3i->time * 1000);
				tlen_method_1_count++;
			} else if (mp3.mb_getFilePath(mp3file) == TRUE) { // else try mp3tagtools
				int duration = mp3.getLengthInSeconds();
				duration *= 1000;
				tlen = numToString(duration);
				tlen_method_2_count++;

			} else { // else error
				error_results += mp3file;
				error_results += ": No valid mp3 headers found. Might not be an mp3 file.\r\n";
				tlen_methods_failed_count++;
			}
		}
		delete id3;
		if (tlen.GetLength()) {
			song->setId3("TLEN", tlen);
		}
	} else if (fext == "ogg") {
		OggTag ogg;
		error_results += ogg.read(mp3file);
		song = createSongFromOgg(&ogg);
	} else if (fext == "wma") {
		WmaTag wma;
		error_results += wma.read(mp3file);
		song = createSongFromWma(&wma);
	}

    song->setId3("FILE", (LPCTSTR)mp3file);
	CString genre = song->getId3((CString)"TCON");
	genre = Genre_normalize(genre);
	song->setId3((CString)"TCON", genre);

	CString artist = song->getId3((CString)"TPE1");
	if (artist == "" || artist == MBUNKNOWN)
		song->setId3("TPE1", MBUNKNOWN);
	CString album = song->getId3((CString)"TALB");
	if (album == "" || album == MBUNKNOWN)
		song->setId3("TALB", MBUNKNOWN);

	CString title = song->getId3("TIT2");
	if (title == MBUNKNOWN) {
		song->setId3("TIT2", mp3file);
	}

	return song;
}

CString
MusicLib::writeSongToFile(Song song) {
	CString result;
	CString file = song->getId3(CS("FILE"));
	if (!FileUtil::IsReadable(file)) {
		result = file;
		result += " is unreadable";
		return result;
	}
	if (!FileUtil::IsWriteable(file)) {
		result = file;
		result += " is unwriteable";
		return result;
	}
	FExtension fext(file);
	if (fext == "mp3") {
		ID3_Tag * id3 = new ID3_Tag;
		size_t tagsize = id3->Link(file, ID3TT_ALL);
		flags_t uflag = ID3TT_ID3V2;
		if (id3->HasV1Tag()) {
			uflag |= ID3TT_ID3V1;
		}
		if (id3->HasV2Tag()) {
			uflag |= ID3TT_ID3V2;
		}

		POSITION pos;
		CString key;
		CString val;
		for( pos = song->_obj.GetStartPosition(); pos != NULL; ) {
			song->_obj.GetNextAssoc(pos, key, val);
			if (key.GetLength() && val.GetLength() && val != MBUNKNOWN) {
				if (key == "TCON" ) {
		            Genre_addGenre(*id3, (LPCTSTR)val);
				} else if (key == "TPE1" ) {
				    ID3_AddArtist(id3, (LPCTSTR)val, true);
				} else if (key == "TALB" ) {
					ID3_AddAlbum(id3, (LPCTSTR)val, true);
				} else if (key == "TIT2" ) {
					ID3_AddTitle(id3, (LPCTSTR)val, true);
				} else if (key == "TRCK" ) {
					int t = atoi((LPCTSTR)val);
					ID3_AddTrack(id3, t, 0, true);
				} else if (key == "TYER" ) {
					ID3_AddYear(id3, (LPCTSTR)val, true);
				}
			}
		}
		flags_t tags = id3->Update(uflag);
		if (tags == ID3TT_NONE) {
			result += "unable to update id3 tag in " + file;
		}
		delete id3;
	} else if (fext == "ogg") {
		OggTag ogg(file);
		POSITION pos;
		CString key;
		CString val;
		for( pos = song->_obj.GetStartPosition(); pos != NULL; ) {
			song->_obj.GetNextAssoc(pos, key, val);
			if (key.GetLength() && val.GetLength() && val != MBUNKNOWN) {
				if (key == "TCON" ) {
		            ogg.setVal("genre", val);
				} else if (key == "TPE1" ) {
				    ogg.setVal("artist", val);
				} else if (key == "TALB" ) {
					ogg.setVal("album", val);
				} else if (key == "TIT2" ) {
					ogg.setVal("title", val);
				} else if (key == "TRCK" ) {
					ogg.setVal("tracknumber", val);
				} else if (key == "TYER" ) {
					ogg.setVal("date", val);
				}
			}
		}
		result += ogg.write();
	} else if (fext == "wma") {
// WMA attributes
// Title
// Author
// WM/TrackNumber
// WM/AlbumArtist
// WM/AlbumTitle
// WM/Genre
// WM/GenreID
// WM/Year
// Duration

		WmaTag wma(file);
		POSITION pos;
		CString key;
		CString val;
		for( pos = song->_obj.GetStartPosition(); pos != NULL; ) {
			song->_obj.GetNextAssoc(pos, key, val);
			if (key.GetLength() && val.GetLength() && val != MBUNKNOWN) {
				if (key == "TCON" ) {
		            wma.setVal("WM/Genre", val);
					int gid = Genre_getInt(val);
					// if we get -1 back, leave it since there's no match
					wma.setVal("WM/GenreID", numToString(gid));
				} else if (key == "TPE1" ) {
				    wma.setVal("WM/AlbumArtist", val);
				} else if (key == "TALB" ) {
					wma.setVal("WM/AlbumTitle", val);
				} else if (key == "TIT2" ) {
					wma.setVal("Title", val);
				} else if (key == "TRCK" ) {
					wma.setVal("WM/TrackNumber", val);
				} else if (key == "TYER" ) {
					wma.setVal("WM/Year", val);
				}
			}
		}
		result += wma.write();
	}
	return result;
}

int
MusicLib::garbageCollect(InitDlg * dialog) {
	SearchClear();
	if (m_SongLib.m_garbagecollector < MB_GARBAGE_INTERVAL) {
		return 0;
	}
	m_SongLib.m_garbagecollector = 0;
	if (dialog) {
		CString msg = "Performing database maintenance. Just a minute please...";
		dialog->SetLabel(msg);
		dialog->ProgressRange(0,m_SongLib.count());
	}

	int ctr = 0;
	MSongLib newSongLib;
	newSongLib.init(TRUE);
	newSongLib.setDbLocation(m_SongLib.getDbLocation());
	MList genreList = m_SongLib.genreList();
	MList::Iterator genreIter(genreList);
	while (genreIter.more()) {
		MRecord genre = genreIter.next();
		if (genre.label() != MBALL) {
			MList artistList = m_SongLib.artistList(genre.label());
			MList::Iterator artistIter(artistList);
			while (artistIter.more()) {
				MRecord artist = artistIter.next();
				MList albumList = m_SongLib.albumList(genre.label(),
					artist.label());
				MList::Iterator albumIter(albumList);
				while (albumIter.more()) {
					if (dialog) dialog->ProgressPos(ctr);
					MRecord album = albumIter.next();
					MList songList = m_SongLib.songList(genre.label(),
						artist.label(), album.label());
					MList::Iterator songIter(songList);
					while (songIter.more()) {
						MRecord songr = songIter.next();
						Song song = songr.createSong();
						newSongLib.addSong(song);
						++ctr;
					}
				}
			}
		}
	}
	m_SongLib = newSongLib;
//	m_SongLib.addAllGenre();
//	writeDb();
	return 1;
}
void
MusicLib::SearchSetup() {
	if (m_Searching)
		return;
	m_Searching = TRUE;
	m_SaveLib = m_SongLib;
}
void
MusicLib::SearchCancel() {
	m_SongLib = m_SaveLib;
	m_Searching = FALSE;
	IgetLibraryCounts();
}
void
MusicLib::SearchClear() {
	if (!m_Searching)
		return;
	m_Searching = FALSE;
	m_SongLib = m_SaveLib;
	IgetLibraryCounts();
}
int
MusicLib::Search(const CString keywords) {
	CString word;
	int found = 0;
	int n = String::delCount(keywords," ");
	for(int i = 1 ; i <= n ; i++) {
		word = String::field(keywords," ",i);
		MSongLib results;
		results.init(TRUE);
		found = iSearch(word, m_SongLib, results);
		if (found) {
			m_SongLib = results;
		} else {
			i = n + 1;
		}
	}

	IgetLibraryCounts();
	return found;
}
int
MusicLib::iSearch(const CString keyword, MSongLib & db, MSongLib & results) {
	int found = 0;
	MList genreList = db.genreList();
	MList::Iterator genreIter(genreList);
	while (genreIter.more()) {
		MRecord genre = genreIter.next();
		if (genre.label() != MBALL) {
			MList artistList = db.artistList(genre.label());
			MList::Iterator artistIter(artistList);
			while (artistIter.more()) {
				MRecord artist = artistIter.next();
				MList albumList = db.albumList(genre.label(),
					artist.label());
				MList::Iterator albumIter(albumList);
				while (albumIter.more()) {
					MRecord album = albumIter.next();
					MList songList = db.songList(genre.label(),
						artist.label(), album.label());
					MList::Iterator songIter(songList);
					while (songIter.more()) {
						MRecord songr = songIter.next();
						Song song = songr.createSong();
						if (song->Contains(keyword)) {
							results.addSong(song);
							found++;
						}
					}
				}
			}
		}
	}
	return found;
}

Song
MusicLib::createSongFromId3(ID3_Tag * id3) {
    CString genre = id3_GetGenre(id3);
    CString artist = id3_GetArtist(id3);
    CString album = id3_GetAlbum(id3);
    CString title = id3_GetTitle(id3);
    CString year = id3_GetYear(id3);
    CString track = id3_GetTrack(id3);
	CString tlen = id3_GetTLEN(id3);

    CString cgenre = Genre_normalize(genre);
    Song song = new CSong;
    song->setId3("TCON", (LPCTSTR)cgenre);
    song->setId3("TPE1", artist);
    song->setId3("TALB", album);
    song->setId3("TIT2", title);
    song->setId3("TRCK", track);
    song->setId3("TYER", year);
	if (tlen != "")
		song->setId3("TLEN", tlen);
    return song;
}
Song
MusicLib::createSongFromOgg(OggTag * ogg) {

	CString kv,key,val;
	Song song = new CSong;
	CString genre,artist,album,title,year,track;

	genre = ogg->getVal("genre");
	artist = ogg->getVal("artist");
	album = ogg->getVal("album");
	title = ogg->getVal("title");
	year = ogg->getVal("date");
	track = ogg->getVal("tracknumber");

    CString cgenre = Genre_normalize(genre);
    song->setId3("TCON", (LPCTSTR)cgenre);
    song->setId3("TPE1", artist);
    song->setId3("TALB", album);
    song->setId3("TIT2", title);
    song->setId3("TRCK", track);
    song->setId3("TYER", year);
	song->setId3("TLEN", numToString(ogg->getTime()*1000));

	return song;
}
// WMA attributes
// Title
// Author
// WM/TrackNumber
// WM/AlbumTitle
// WM/Genre
// WM/GenreID
// WM/Year
// Duration

Song
MusicLib::createSongFromWma(WmaTag * wma) {

	CString kv,key,val;
	Song song = new CSong;
	CString genre,artist,album,title,year,track, duration;

	genre = wma->getVal("WM/Genre");
	artist = wma->getVal("WM/AlbumArtist");
	album = wma->getVal("WM/AlbumTitle");
	title = wma->getVal("Title");
	year = wma->getVal("WM/Year");
	track = wma->getVal("WM/TrackNumber");
	duration = wma->getVal("Duration");
	if (duration.GetLength()) {
		float d = atof(duration);
		d = d / 10000;
		int d2 = (int)d;
		duration = numToString(d2);
	}

    CString cgenre = Genre_normalize(genre);
    song->setId3("TCON", (LPCTSTR)cgenre);
    song->setId3("TPE1", artist);
    song->setId3("TALB", album);
    song->setId3("TIT2", title);
    song->setId3("TRCK", track);
    song->setId3("TYER", year);
	song->setId3("TLEN", duration);

	return song;
}

void
MusicLib::deletePlaylist(const CString & name) {
	CString src;
	src = m_dir + "\\";
	src += MBPLAYLISTDIR ;
	src += "\\" + name + "." + MBPLAYLISTM3U;
    
	FileUtil::rm(src);

}
BOOL 
MusicLib::renamePlaylist(const CString srcname,const CString dstname, 
						 BOOL overwrite)
{
	CString src,dst;
	src = m_dir + "\\";
	src += MBPLAYLISTDIR ;
	src += "\\" + srcname + "." + MBPLAYLISTM3U;
	dst = m_dir + "\\";
	dst += MBPLAYLISTDIR ;
	dst += "\\" + dstname + "." + MBPLAYLISTM3U;
	if (FileUtil::IsReadable(dst)) {
		if (TRUE == overwrite) {
			FileUtil::mv(src,dst);
		} else
			return FALSE;
	} else
		FileUtil::mv(src,dst);
	return TRUE;
}
void
MusicLib::savePlaylist(Playlist & playlist, const CString & file) {

	CString playlistDir = m_dir;
	playlistDir += "\\";
	playlistDir += MBPLAYLISTDIR;

	if (!FileUtil::IsWriteable(playlistDir)) {
		FileUtil::mkdirp(playlistDir);
	}

    CString dbfilename = playlistDir;
    dbfilename += "\\";
    dbfilename += file;
    dbfilename += "." MBPLAYLISTM3U;

	if ("Last" == file) {
		CString previous = m_dir + "\\" + MBPLAYLISTDIR "\\Previous.m3u";
		FileUtil::mv(dbfilename, previous);
	}

	CFile myFile;
	CFileException fileException;
	if ( !myFile.Open( (LPCTSTR)dbfilename,
        CFile::modeWrite | CFile::modeCreate,
        &fileException ))
	{
        CString msg = "Unable to save playlist ";
        msg += dbfilename;
        MBMessageBox(CString("alert"), msg);
        return ;
	}

    CString buf;
	for (PlaylistNode *p = playlist.head();
		p != (PlaylistNode*)0;
		p = playlist.next(p)) {
			buf += p->_item->getId3("FILE");
		    buf += "\r\n";
    }
    myFile.Write(buf,buf.GetLength());
    myFile.Flush();

}
void
MusicLib::savePlaylist(const CString & file) {
	savePlaylist(_playlist, file);
}
void
MusicLib::savePlaylist(const CStringArray & list, const CString & name) {
	int size = list.GetSize();
	CString playlistDir = m_dir;
	playlistDir += "\\";
	playlistDir += MBPLAYLISTDIR;

	if (!FileUtil::IsWriteable(playlistDir)) {
		FileUtil::mkdirp(playlistDir);
	}

    CString dbfilename = playlistDir;
    dbfilename += "\\";
    dbfilename += name;
    dbfilename += "." MBPLAYLISTM3U;

	CFile myFile;
	CFileException fileException;
	if ( !myFile.Open( (LPCTSTR)dbfilename,
        CFile::modeWrite | CFile::modeCreate,
        &fileException ))
	{
        CString msg = "Unable to save playlist ";
        msg += dbfilename;
        MBMessageBox(CString("alert"), msg);
        return ;
	}
	for(int pos = 0 ; pos < size ; pos++) {
		myFile.Write(list.GetAt(pos), list.GetAt(pos).GetLength());
		myFile.Write("\r\n",2);
	}

    myFile.Flush();


}

void
MusicLib::RandomizePlaylist() {
    shufflePlaylist();
    Playlist newplaylist;
    srand( (unsigned)time( NULL ) );
    while (_playlist.size() > 0) {
        float ratio = rand() / (float)RAND_MAX;
        int pos = (int)(ratio * _playlist.size());
        newplaylist.append(_playlist[pos]);
        _playlist.remove(pos);
    }
    _playlist.reset();
    while (newplaylist.head()) {
        _playlist.append(newplaylist.head()->_item);
        newplaylist.remove(newplaylist.head());
    }
}
#ifdef asdf
void
MusicLib::shufflePlaylist() {
    int n = _playlist.size();
    int i;
    char buf[100];
    CMapStringToOb map;
    CStringList *list;
    for (i = 0; i < n; ++i) {
        Song song = _playlist[i];
        CString artist = song->getId3("TPE1");
        if (map.Lookup(artist, (CObject *&) list) == 0) {
            list = new CStringList;
            map.SetAt(artist, list);
//            delete list;
        }
        sprintf(buf, "%d", i);
        CString spos = buf;
        list->AddTail(spos);
    }

    Playlist newplaylist;
    POSITION pos;
    CString artist;
    pos = map.GetStartPosition();
    while (pos != NULL) {
        map.GetNextAssoc(pos, artist, (CObject *&)list);

        if (list->IsEmpty()) {
            map.RemoveKey((LPCTSTR)artist);
            delete list;
        } else {
            CString mappos = list->RemoveHead();
            int ppos = atoi((LPCTSTR)mappos);
            Song song = _playlist[ppos];
            newplaylist.append(song);
        }
        if (pos == NULL) {
            pos = map.GetStartPosition();
        }
    }
    _playlist.reset();
    while (newplaylist.head()) {
        _playlist.append(newplaylist.head()->_item);
        newplaylist.remove(newplaylist.head());
    }
}
#endif
void
MusicLib::shufflePlaylist() {
    int n = _playlist.size();
    int i;
    AutoBuf buf(100);
    CMapStringToOb map;
    CStringList *list;
    for (i = 0; i < n; ++i) {
        Song song = _playlist[i];
        CString artist = song->getId3("TALB");
        if (map.Lookup(artist, (CObject *&) list) == 0) {
            list = new CStringList;
            map.SetAt(artist, list);
//            delete list;
        }
        sprintf(buf.p, "%d", i);
        CString spos = buf.p;
        list->AddTail(spos);
    }

    Playlist newplaylist;
    POSITION pos;
    CString artist;
    pos = map.GetStartPosition();
    while (pos != NULL) {
        map.GetNextAssoc(pos, artist, (CObject *&)list);

        if (list->IsEmpty()) {
            map.RemoveKey((LPCTSTR)artist);
            delete list;
        } else {
            CString mappos = list->RemoveHead();
            int ppos = atoi((LPCTSTR)mappos);
            Song song = _playlist[ppos];
            newplaylist.append(song);
        }
        if (pos == NULL) {
            pos = map.GetStartPosition();
        }
    }
    _playlist.reset();
    while (newplaylist.head()) {
        _playlist.append(newplaylist.head()->_item);
        newplaylist.remove(newplaylist.head());
    }
}
void
MusicLib::modifyID3(Song oldSong, Song newSong) {
	SearchClear();
    CString oldGenre, oldArtist, oldAlbum, oldTitle, oldYear, oldTrack;
    CString newGenre, newArtist, newAlbum, newTitle, newYear, newTrack;
    oldGenre = oldSong->getId3("TCON",0);
    newGenre = newSong->getId3("TCON",0);
    oldArtist = oldSong->getId3("TPE1",0);
    newArtist = newSong->getId3("TPE1",0);
    oldAlbum = oldSong->getId3("TALB",0);
    newAlbum = newSong->getId3("TALB",0);
    oldTitle = oldSong->getId3("TIT2",0);
    newTitle = newSong->getId3("TIT2",0);
    oldTrack = oldSong->getId3("TRCK");
    newTrack = newSong->getId3("TRCK");
    oldYear = oldSong->getId3("TYER");
    newYear = newSong->getId3("TYER");

    InitDlg * dialog = new InitDlg(1,0);
	CString msg = "Gathering files...";
	dialog->SetLabel(msg);
	dialog->ShowWindow(SW_SHOWNORMAL);
	dialog->UpdateWindow();

	Playlist songs;
    if (oldGenre == "") oldGenre = MBALL;
	// old* vars need to be null to search as wildcard
    searchForMp3s(songs, oldGenre, oldArtist, oldAlbum, oldTitle);
	int count = songs.size();
	dialog->ProgressRange(0, count);

	msg = "The following files will be modified.\r\nClick OK to continue or Cancel to abort.\r\n";
	AutoBuf buf(1000);
	CString fmt = "%15s: %20s,  %15s: %s\r\n";
	sprintf(buf.p,fmt,"old genre",oldGenre,"new genre",newGenre);
	msg += buf.p;
	sprintf(buf.p,fmt,"old artist",oldArtist,"new artist",newArtist);
	msg += buf.p;
	sprintf(buf.p,fmt,"old album",oldAlbum,"new album",newAlbum);
	msg += buf.p;
	sprintf(buf.p,fmt,"old title",oldTitle,"new title",newTitle);
	msg += buf.p;
	sprintf(buf.p,fmt,"old track#",oldTrack,"new track",newTrack);
	msg += buf.p;
	sprintf(buf.p,fmt,"old year",oldYear,"new year",newYear);
	msg += buf.p;
	msg += "\r\n";

    for (PlaylistNode *p = songs.head();
	  p != (PlaylistNode*)0; p = songs.next(p))
    {
        msg += p->_item->getId3("FILE");
		msg += "\r\n";
    }

	dialog->EndDialog(0);
	delete dialog;
	int r = MBMessageBox("Notice", msg, TRUE, TRUE);
	if (r == 0) {
		MBMessageBox("Notice","Edit not performed");
		return;
	}

	dialog = new InitDlg(1,0);
	dialog->m_InitLabel = "Modifying audio files...";
	dialog->SetLabel(msg);
	dialog->ShowWindow(SW_SHOWNORMAL);
	dialog->UpdateWindow();

	CString result;
    int ctr = 1;
    for (p = songs.head();
		p != (PlaylistNode*)0; p = songs.next(p))
    {
		CString file = p->_item->getId3("FILE");

        dialog->UpdateStatus(file);
        dialog->ProgressPos(ctr++);

		int updateFlag = 0;
		Song addsong = new CSong(p->_item);
        if (newGenre != "" && newGenre != MBUNKNOWN) {
			addsong->setId3(CS("TCON"), newGenre);
			updateFlag = 1;
        }
        if (newArtist != "" && newArtist != MBUNKNOWN) {
			addsong->setId3(CS("TPE1"), newArtist);
			updateFlag = 1;
        }
        if (newAlbum != "" && newAlbum != MBUNKNOWN) {
			addsong->setId3(CS("TALB"), newAlbum);
			updateFlag = 1;
        }
        if (newTitle != "" && newTitle != MBUNKNOWN) {
			addsong->setId3(CS("TIT2"), newTitle);
			updateFlag = 1;
        }
        if (newTrack != "") {
			addsong->setId3(CS("TRCK"), newTrack);
			updateFlag = 1;
        }
        if (newYear != "") {
			addsong->setId3(CS("TYER"), newYear);
			updateFlag = 1;
        }

        if (updateFlag) {
			Song delsong = new CSong(p->_item);
			CString oneresult = writeSongToFile(addsong);
			if (oneresult.GetLength()) {
				result += oneresult;
				result += "\r\n";
			} else {
#pragma hack
				// hack alert m_files only used for scanning/verifying,
				// so keep it empty cause addSong will pre-exit if...
				// not any more
//				m_SongLib.m_files.removeAll();
				m_SongLib.removeSong(delsong);
				m_SongLib.addSong(addsong);
			}
        }
    }
	if (result.GetLength()) {
//		result += "\r\nYou'll need to re-scan";
		MBMessageBox(CString("alert"), result);
	}
	m_SongLib.m_garbagecollector++;
    garbageCollect(dialog);
	writeDb();
	dialog->EndDialog(0);
	delete dialog;
	// playlists are now filename only
	//modifyPlaylists(oldSong, newSong);
    return;
}


void
MusicLib::searchForMp3s(Playlist & songs, const CString & genrename_,
	const CString & artistname, const CString & albumname,
	const CString & songname) {

	if (genrename_ == "")
		return;
	CString genrename = Genre_normalize(genrename_);

	MList artistList = m_SongLib.artistList(genrename);
	if (artistname != "") {
		MList albumList = m_SongLib.albumList(genrename, artistname);
		if (albumname != "") {
			MList songList = m_SongLib.songList(genrename, artistname,
				albumname);
			searchForSongs(songs, songList, songname);
		} else {
			searchForAlbums(songs, albumList);
		}
	} else {
		searchForArtists(songs, artistList);
	}
}

void
MusicLib::searchForArtists(Playlist & songs, MList & artistList) {
	MList::Iterator artistIter(artistList);
	while (artistIter.more()) {
		MRecord artist = artistIter.next();
		MList albumList(artist);
		searchForAlbums(songs, albumList);
	}
}
void
MusicLib::searchForAlbums(Playlist & songs, MList & albumList) {
	MList::Iterator albumIter(albumList);
	while (albumIter.more()) {
		MRecord album = albumIter.next();
		MList songList(album);
		searchForSongs(songs, songList);
	}
}
void
MusicLib::searchForSongs(Playlist & songs, MList & songList,
						 const CString & songname) {
	if (songname != "") {
		MRecord song = songList.record(songname);
		songs.append(song.createSong());
	} else {
		MList::Iterator songIter(songList);
		while (songIter.more()) {
			MRecord song = songIter.next();
			songs.append(song.createSong());
		}
	}
}

void
MusicLib::dumpPL(int playlistCurrent) {
    CString buf;
    AutoBuf cbuf(100);
    sprintf(cbuf.p,"%d: ", playlistCurrent);
    buf = cbuf.p;
    for (PlaylistNode *p = _playlist.head();
    p != (PlaylistNode*)0; p = _playlist.next(p))
    {
    
        //		buf = p->_item->getId3("TPE1");
        //		buf += " / ";
        //		buf += p->_item->getId3("TALB");
        //		buf += " / ";
        buf += p->_item->getId3("TIT2");
        buf += " ";
    }
    buf += "\r\n";
    OutputDebugString(buf);

}
void
MusicLib::movePlaylistDown(int plcurrent, int element) {
    _playlist.moveDown(element);
    // dumpPL(plcurrent);
}
void
MusicLib::movePlaylistUp(int plcurrent, int element) {
    _playlist.moveUp(element);
    //dumpPL(plcurrent);
}
CString
MusicLib::getLibraryCounts() {
	if (m_libCounts.GetLength())
		return m_libCounts;
	else
		return IgetLibraryCounts();
}
CString
MusicLib::IgetLibraryCounts() {
	int genrecount, artistcount, albumcount, songcount;
	genrecount = artistcount = albumcount = songcount = 0;
	MList genreList = m_SongLib.genreList();
	MList::Iterator genreIter(genreList);
	while (genreIter.more()) {
		MRecord genre = genreIter.next();
		if (genre.label() != MBALL) {
			++genrecount;
			MList artistList(genre);
			MList::Iterator artistIter(artistList);
			while (artistIter.more()) {
				MRecord artist = artistIter.next();
				if (artist.label() != MBALL) {
					++artistcount;
					MList albumList(artist);
					MList::Iterator albumIter(albumList);
					while (albumIter.more()) {
						++albumcount;
						MRecord album = albumIter.next();
						MList songList(album);
						MList::Iterator songIter(songList);
						while (songIter.more()) {
							songIter.next();
							++songcount;
						}
					}

				}
			}
		}
	}

    CString mesg;

    AutoBuf buf(1000);
    sprintf(buf.p, "%d Genres, %d Artists, %d Albums, %d Songs",
        genrecount, artistcount, albumcount, songcount);
    mesg = buf.p;
	m_libCounts = mesg;
    return mesg;
}


BOOL
MusicLib::apic(const CString & file, uchar *& rawdata, size_t & nDataSize) {

	//	if (m_picCache.read(file, rawdata, nDataSize)) {
	//		return TRUE;
	//	}
	FExtension fext(file);
	if (fext == "mp3") {
		ID3_Tag id3;
		size_t tagsize = id3.Link(file, ID3TT_ALL);

		ID3_Tag::Iterator* iter = id3.CreateIterator();
		const ID3_Frame* frame = NULL; 

		while (rawdata == NULL && NULL != (frame = iter->GetNext())) { 
			ID3_FrameID eFrameID = frame->GetID();
			if (eFrameID == ID3FID_PICTURE) {

				nDataSize  = frame->GetField(ID3FN_DATA)->Size();

				rawdata = new BYTE [ nDataSize ];
				memcpy(rawdata, frame->GetField(ID3FN_DATA)->GetRawBinary(),
					nDataSize);
	//			m_picCache.write(file, rawdata, nDataSize);
				if (nDataSize) {
					delete iter;
					return TRUE;
				}


			}
		}
		delete iter;
	}

	// not in apic so look for folder.jpg in dir
	CString folderjpg = FileUtil::dirname(file);
	folderjpg += "\\folder.jpg";
	int fd = _open(folderjpg, _O_RDONLY|_O_BINARY);
	if (fd > 0) {
		struct _stat statbuf;
		int fs = _stat(folderjpg, &statbuf );
		if (fs != 0) {
			close(fd);
			return FALSE;
		}

		nDataSize = statbuf.st_size;
		rawdata = new BYTE [ nDataSize ];
		int r = _read(fd, (void*) rawdata, nDataSize);
		close(fd);
		if (r != nDataSize) {
			delete rawdata;
			return FALSE;
		}
//		m_picCache.write(file, rawdata, nDataSize);
		return TRUE;
	}
	// not in folder.jpg so look for cover.jpg in dir
	folderjpg = FileUtil::dirname(file);
	folderjpg += "\\cover.jpg";
	fd = _open(folderjpg, _O_RDONLY|_O_BINARY);
	if (fd > 0) {
		struct _stat statbuf;
		int fs = _stat(folderjpg, &statbuf );
		if (fs != 0) {
			close(fd);
			return FALSE;
		}

		nDataSize = statbuf.st_size;
		rawdata = new BYTE [ nDataSize ];
		int r = _read(fd, (void*) rawdata, nDataSize);
		close(fd);
		if (r != nDataSize) {
			delete rawdata;
			return FALSE;
		}
//		m_picCache.write(file, rawdata, nDataSize);
		return TRUE;
	}
	return FALSE;
}


/////////////////////////////////////////////////////////////////////////////////



// m_next is -1 until an allocation is done which prevents access to
// unallocated memory

MMemory::MMemory(const CString & file) :
	m_sizeOrig(MMEMORY_SIZE_INCREMENT), m_file(file),
	m_space(0), m_next(-1), _refcnt(0)
{
	create();
}
MMemory::MMemory() :
	m_sizeOrig(MMEMORY_SIZE_INCREMENT),
	m_space(0), m_next(-1), _refcnt(0)
{
	create();
}

MMemory::~MMemory() {
	if (m_space)
		free(m_space);
}
void
MMemory::unreference() {
	if (--_refcnt ==0) {
		delete this;
	}
}
MMemory::operator = (MMemory & mem) {
	m_sizeOrig = mem.m_sizeOrig;
	m_dir = mem.m_dir;
	m_file = mem.m_file;
	if (m_space) {
		free(m_space);
		m_space = 0;
		m_size = 0;
		m_next = -1;
	}
	if (mem.m_space && mem.m_size > 0) {
// short copy
//		m_space = (char *) malloc(mem.m_size);
//		memcpy(m_space, mem.m_space, mem.m_size);
		m_space = mem.m_space;
		mem.m_space = 0;
		m_size = mem.m_size;
		m_next = mem.m_next;
	}
}
void
MMemory::create() {
	if (m_space)
		free(m_space);
	m_size = m_sizeOrig;
	m_space = (char*)malloc(m_size);
	memset(m_space, 0, m_size);
	m_next = -1;
}
int
MMemory::alloc(int length) {
	if (m_next == -1) m_next = 0;
	int newsize = m_size;
	while (length > newsize - m_next) {
		newsize += MMEMORY_SIZE_INCREMENT;
	}
	m_space = (char*)realloc(m_space, newsize);
	memset(m_space + m_size, 0, newsize - m_size);
	m_size = newsize;
	int r = m_next;
	m_next += length;
	return r;
}

int
MMemory::readi(int p) {
	if (!(0 <= p && p <= m_next)) return -1;
	int * i = (int *)((char *) m_space + p);
	return *i;
}
char *
MMemory::readc(int p) {
	if (!(0 <= p && p <= m_next)) return NULL;
	return m_space + p;
}
int
MMemory::writei(int p, int val) {
	if (!(0 <= p && p <= m_next)) return -1;
	int * w = (int *)((char *)m_space + p);
	*w = val;
	return 0;
}
int 
MMemory::writec(int p, char * val) {
	if (!(0 <= p && p <= m_next)) return -1;
	char * w = (m_space + p);
	strcpy(w, val);
	return 0;
}
void 
MMemory::setDbLocation(const CString & loc) {
	m_dir = loc;
	m_file = loc;
	m_file += "\\";
	m_file += MUZIKBROWZER;
	m_file += ".mb";
}
int
MMemory::readFromFile() {

	if (m_file.GetLength() == 0) return -1;
    CString dbfilename = m_file;

    const char * pszFileName = (LPCTSTR) dbfilename;
	CFile myFile;
	CFileException fileException;

	if ( !myFile.Open( pszFileName,
        CFile::modeRead,
        &fileException ))
	{
        return -1;
	}
    if (myFile.GetLength()) {
        if (m_space) free(m_space);
        m_size = myFile.GetLength();
        m_space = (char*)malloc(m_size);

        myFile.Read(m_space, myFile.GetLength());
        m_next = myFile.GetLength();
		return 0;
    }
	return -1;
}

int
MMemory::writeToFile() {
	if (m_file.GetLength() == 0 || m_next < 0) return -1;
    CString dbfilename = m_file;

    const char * pszFileName = (LPCTSTR) dbfilename;
	CFile myFile;
	CFileException fileException;

	if ( !myFile.Open( pszFileName,
        CFile::modeWrite | CFile::modeCreate,
        &fileException ))
	{
        CString msg = "Unable to save muzikbrowzer database ";
		msg += dbfilename;
        MBMessageBox(CString("alert"), msg);
        return -1;
	}

    myFile.Write(m_space, m_next);
    myFile.Flush();
    return 0;
}
void *
MMemory::addr(int p) {
	if (p < m_next) {
		return (void *) ((char*)m_space + p);
	} else {
		MBMessageBox("Warning", "Fatal error: MMemory::addr: "+numToString(p)+" "+numToString(m_next));
		ASSERT(FALSE);
		return NULL;
	}
}

#define MBTESTFILE "muzikbrowzer.ck"
TEST(PMemoryTests, PMemory)
{
	PMemory m;
	m = new MMemory(MBTESTFILE);
	int i = m->writei(0,14);
	CHECK(i == -1);
	int p = m->alloc(100);
	CHECK(p == 0);
	i = m->writei(0,14);
	CHECK(i == 0);
	i = m->writei(4,15);
	CHECK(i == 0);
	i = m->writei(8,16);
	CHECK(i == 0);
	i = m->writec(12,"abc");
	CHECK(i == 0);

	i = m->readi(0);
	CHECK(i == 14);
	i = m->readi(4);
	CHECK(i == 15);
	i = m->readi(8);
	CHECK(i == 16);
	char * t = m->readc(12);
	CHECK(strcmp(t,"abc") == 0);
	m->writeToFile();

	PMemory m2;
	m2 = new MMemory(MBTESTFILE);
	m2->readFromFile();
	i = m2->readi(0);
	CHECK(i == 14);
	i = m2->readi(4);
	CHECK(i == 15);
	i = m2->readi(8);
	CHECK(i == 16);
	t = m2->readc(12);
	CHECK(strcmp(t,"abc") == 0);

	CFile::Remove(MBTESTFILE);

}
TEST(MMemoryTests, MMemory)
{
	MMemory m(MBTESTFILE);
	int i = m.writei(0,14);
	CHECK(i == -1);
	int p = m.alloc(100);
	CHECK(p == 0);
	i = m.writei(0,14);
	CHECK(i == 0);
	i = m.writei(4,15);
	CHECK(i == 0);
	i = m.writei(8,16);
	CHECK(i == 0);
	i = m.writec(12,"abc");
	CHECK(i == 0);

	i = m.readi(0);
	CHECK(i == 14);
	i = m.readi(4);
	CHECK(i == 15);
	i = m.readi(8);
	CHECK(i == 16);
	char * t = m.readc(12);
	CHECK(strcmp(t,"abc") == 0);
	m.writeToFile();

	MMemory m2(MBTESTFILE);
	m2.readFromFile();
	i = m2.readi(0);
	CHECK(i == 14);
	i = m2.readi(4);
	CHECK(i == 15);
	i = m2.readi(8);
	CHECK(i == 16);
	t = m2.readc(12);
	CHECK(strcmp(t,"abc") == 0);

	CFile::Remove(MBTESTFILE);

}

#pragma hack
//////////////////////////////////////////////////////////////////
// One caveat, one cannot assign an int reference to one of the 
// reference accessors below. They are only to be used for immediate
// read & write
// xxx figure out how to prevent it.
MRecord::MRecord(PMemory & m, int p) : m_mem(m), m_i(p) {}
MRecord::MRecord(MRecord & r) : m_mem(r.m_mem), m_i(r.m_i) {}
int &
MRecord::length() {
	MRecordt * r = (MRecordt*)m_mem->addr(m_i);
	return (int &) r->length;
}
int &
MRecord::next() {
	MRecordt * r = (MRecordt*)m_mem->addr(m_i);
	return (int &) r->next;
}
int &
MRecord::prev() {
	MRecordt * r = (MRecordt*)m_mem->addr(m_i);
	return (int &) r->prev;
}
int &
MRecord::ptr() {
	MRecordt * r = (MRecordt*)m_mem->addr(m_i);
	return (int &) r->ptr;
}
CString
MRecord::label() {
	char * p = ((char*)m_mem->addr(m_i)) + sizeof(MRecordt);
	CString r (p);
	return r;
}
void
MRecord::label(const CString & val) {
	char * p = ((char*)m_mem->addr(m_i)) + sizeof(MRecordt);
	strcpy(p, (LPCTSTR)val);
}
int
MRecord::needed(const CString & val) {
	return sizeof(MRecordt) + val.GetLength() + 1;
}
int
MRecord::ptrIdx() {
	return m_i + (3 * sizeof(int));
}
TEST(MRecordTests, MRecord) 
{
	PMemory m;
	m = new MMemory(MBTESTFILE);
	int pi = m->alloc(100);

	MRecord r(m,pi);
	r.length() = 1;
	r.prev() = 2;
	r.next() = 3;
	r.ptr() = 4;
	r.label("abc");

	CHECK(r.length() == 1);
	CHECK(r.prev() == 2);
	CHECK(r.next() == 3);
	CHECK(r.ptr() == 4);
	CHECK(r.label() == (CString)"abc");

	m->writeToFile();

	PMemory m2;
	m2 = new MMemory(MBTESTFILE);
	m2->readFromFile();
	pi = m2->alloc(50);

	MRecord r2(m2,pi);
	r2.length() = 21;
	r2.prev() = 22;
	r2.next() = 23;
	r2.ptr() = 24;
	r2.label("xyz");

	m2->writeToFile();

	PMemory m3;
	m3 = new MMemory(MBTESTFILE);
	m3->readFromFile();

	MRecord r3(m3,0);

	MRecord r4(m3,100);
	
	CHECK(r3.length() == 1);
	CHECK(r3.prev() == 2);
	CHECK(r3.next() == 3);
	CHECK(r3.ptr() == 4);
	CHECK(r3.label() == (CString)"abc");

	CHECK(r4.length() == 21);
	CHECK(r4.prev() == 22);
	CHECK(r4.next() == 23);
	CHECK(r4.ptr() == 24);
	CHECK(r4.label() == (CString)"xyz");

	CFile::Remove(MBTESTFILE);
}

CString
MRecord::getKey() {
	return String::substring(label(),0,4);
}
CString
MRecord::getVal() {
	return String::substring(label(),5);
}
CString
MRecord::lookupVal(const CString & key) {
	if (ptr() == 0)
		return CString("");
	MList keyValList(*this);
	MList::Iterator iter(keyValList);
	while (iter.more()) {
		MRecord kv = iter.next();
		if (kv.getKey() == key) {
			return kv.getVal();
		}
	}
	return CString("");
}
Song
MRecord::createSong() {
	Song song = new CSong;
	MList keyvals (*this);
	MList::Iterator kvIter(keyvals);
	while (kvIter.more()) {
		MRecord kv = kvIter.next();
		song->setId3(kv.getKey(), kv.getVal());
	}
	return song;
}

/////////////////////////////////////////////////////////////////
// This creates the initial list in a MMemory.
// The first 100 bytes are reserved.
// 1st 4 bytes -> head pointer
// 2nd 4 bytes -> songcount
// 3rd 4 bytes -> garbage collector counter
// The 1st 4 bytes are reserved for the head pointer, 2nd 4 bytes
// for songcount

MList::MList(PMemory & m) : m_headstore(0), m_mem(m)
{
	if (m_mem->m_next == -1) {
		int pi = m_mem->alloc(MMEMORY_RESERVE_BYTES);
		m_mem->writei(pi, -1); // initialize list head to -1
		ASSERT(head() == -1); // which indicates empty list
	}
}

// This one's for creating a list under another list element, like
// a mkdir. The record's ptr member is the headstore
MList::MList(MRecord & r) : m_mem(r.mem())
{
	m_headstore = r.ptrIdx();
	int h = m_mem->readi(m_headstore);
	if (h == 0) {
		m_mem->writei(m_headstore, -1); // initialize list head to -1
		ASSERT(head() == -1); // which indicates empty list
	}
}
// A read only list?
//MList::MList(const MRecord & r) : m_mem(r.memRO())
//{
//	m_headstore = r.ptrIdx();
//	int h = m_mem->readi(m_headstore);
//	ASSERT(head() != -1); // which indicates empty list
//}

// Same as the int refs returned by MRecord, only use this for immediate
// read/write
int &
MList::head() 
{
	int * h = (int *)m_mem->addr(m_headstore);
	return (int &) *h;
}

int
MList::find(const CString & label) 
{
	int pi = head();
	while(pi > 0) {

		MRecord pr(m_mem,pi);
		if (pr.label() == label) {
			return pi;
		}
		pi = pr.next();
	}
	return -1;
}
int
MList::findg(const CString & label) 
{
	int pi = head();
	while(pi > 0) {

		MRecord pr(m_mem,pi);
		if (String::substring(pr.label(),0, label.GetLength()) == label) {
			return pi;
		}
		pi = pr.next();
	}
	return -1;
}
int
MList::findOrPrepend(const CString & label) {
	int p = find(label);
	if (p != -1) {
		return p;
	}
	return prepend(label);
}
// finds or creates a record in list and returns it
MRecord
MList::record(const CString & label, BOOL forcecreate) {
	int p;
	if (forcecreate) {
		p = prepend(label);
	} else {
		p = findOrPrepend(label);
	}
//	if (-1 == p) {
//		return EmptyMRecord;
//	}
	MRecord r(m_mem, p);
	return r;
}
// finds or creates a record in list and creates a sub list
// on it.
MList
MList::list(const CString & label, BOOL forcecreate) {
	MRecord r = record(label, forcecreate);
	MList l(r);
	return l;
}
void
MList::remove(const CString & label) 
{
	int pi = find(label);
	if (pi == -1)
		return;
	
	MRecord r(m_mem,pi);

	// remove head
	if (pi == head()) {
		if (r.next() == 0) {
			head() = -1;		// empty list
		} else {
			head() = r.next();	// remove head of non-empty list
			MRecord rh(m_mem, head());
			rh.prev() = 0;
		}
		return;
	}
	ASSERT(r.prev() != 0);

	// set prev->next to next;
	MRecord rprev(m_mem, r.prev());
	rprev.next() = r.next();

	// if not tail next->prev = prev
	if (r.next()) {
		MRecord rnext(m_mem, r.next());
		rnext.prev() = r.prev();
	}
}
void
MList::removeg(const CString & label) 
{
	int pi = findg(label);
	if (pi == -1)
		return;
	
	MRecord r(m_mem,pi);

	// remove head
	if (pi == head()) {
		if (r.next() == 0) {
			head() = -1;		// empty list
		} else {
			head() = r.next();	// remove head of non-empty list
			MRecord rh(m_mem, head());
			rh.prev() = 0;
		}
		return;
	}
	ASSERT(r.prev() != 0);

	// set prev->next to next;
	MRecord rprev(m_mem, r.prev());
	rprev.next() = r.next();

	// if not tail next->prev = prev
	if (r.next()) {
		MRecord rnext(m_mem, r.next());
		rnext.prev() = r.prev();
	}
}
void
MList::remove(MRecord & r) {
	// remove head
	int pi = r.m_i;
	if (pi == head()) {
		if (r.next() == 0) {
			head() = -1;		// empty list
		} else {
			head() = r.next();	// remove head of non-empty list
			MRecord rh(m_mem, head());
			rh.prev() = 0;
		}
		return;
	}
	ASSERT(r.prev() != 0);

	// set prev->next to next;
	MRecord rprev(m_mem, r.prev());
	rprev.next() = r.next();

	// if not tail next->prev = prev
	if (r.next()) {
		MRecord rnext(m_mem, r.next());
		rnext.prev() = r.prev();
	}
}
int
MList::prepend(const CString & label) 
{
	int s = MRecord::needed(label);
	int pi = m_mem->alloc(s);
	if (head() == -1)
		head() = 0;

	// point head->prev = new record
	if (head() != 0) {
		MRecord oldhead(m_mem, head());
		oldhead.prev() = pi;
	}

	// newrecord->next = head;
	MRecord r(m_mem, pi);
	r.length() = s;
	r.prev() = 0;
	r.next() = head();
	r.ptr() = 0;
	r.label(label);
	
	// head becomes the newrecord
	head() = pi;
	return pi;
}

MRecord
MList::Iterator::next() {
	MRecord r(m_list.m_mem, pos);
	pos = r.next();
	return r;
}
int
MList::count() {
	MList::Iterator iter (*this);
	int count = 0;
	while (iter.more()) {
		++count;
		iter.next();
	}
	return count;
}
TEST(MListTests, MList)
{
	PMemory m;
	m = new MMemory(MBTESTFILE);
	MList list(m);

	// add five elements and check all records/fields
	list.prepend("one");
	list.prepend("two");
	list.prepend("three");
	list.prepend("four");
	list.prepend("five");

	int pi = list.find("one");
	CHECK(pi == MMEMORY_RESERVE_BYTES);
	MRecord r(m, pi);
	CHECK(r.label() == "one");
	CHECK(r.length() == 20);
	CHECK(r.prev() == 20 + MMEMORY_RESERVE_BYTES);
	CHECK(r.next() == 0);
	CHECK(r.ptr() == 0);

	MRecord r2(m,r.prev());
	CHECK(r2.label() == "two");
	CHECK(r2.length() == 20);
	CHECK(r2.prev() == 40 + MMEMORY_RESERVE_BYTES);
	CHECK(r2.next() == MMEMORY_RESERVE_BYTES);
	CHECK(r2.ptr() == 0);

	MRecord r3(m, r2.prev());
	CHECK(r3.label() == "three");
	CHECK(r3.length() == 22);
	CHECK(r3.prev() == 62 + MMEMORY_RESERVE_BYTES);
	CHECK(r3.next() == 20 + MMEMORY_RESERVE_BYTES);
	CHECK(r3.ptr() == 0);

	MRecord r4(m, r3.prev());
	CHECK(r4.label() == "four");
	CHECK(r4.length() == 21);
	CHECK(r4.prev() == 83 + MMEMORY_RESERVE_BYTES);
	CHECK(r4.next() == 40 + MMEMORY_RESERVE_BYTES);
	CHECK(r4.ptr() == 0);

	MRecord r5(m, r4.prev());
	CHECK(r5.label() == "five");
	CHECK(r5.length() == 21);
	CHECK(r5.prev() == 0);
	CHECK(r5.next() == 62 + MMEMORY_RESERVE_BYTES);
	CHECK(r5.ptr() == 0);

	// delete middle, tail and head and check all records/fields

	// middle
	pi = list.find("three");
	CHECK(pi == 40 + MMEMORY_RESERVE_BYTES);
	list.remove("three");
	pi = list.find("three");
	CHECK(pi == -1);

	pi = list.find("two");
	MRecord r22(m,pi);
	CHECK(r22.label() == "two");
	CHECK(r22.length() == 20);
	CHECK(r22.prev() == 62 + MMEMORY_RESERVE_BYTES);
	CHECK(r22.next() == MMEMORY_RESERVE_BYTES);
	CHECK(r22.ptr() == 0);

	pi = list.find("four");
	MRecord r42(m, pi);
	CHECK(r42.label() == "four");
	CHECK(r42.length() == 21);
	CHECK(r42.prev() == 83 + MMEMORY_RESERVE_BYTES);
	CHECK(r42.next() == 20 + MMEMORY_RESERVE_BYTES);
	CHECK(r42.ptr() == 0);

	// tail
	pi = list.find("one");
	CHECK(pi == MMEMORY_RESERVE_BYTES);
	list.remove("one");
	pi = list.find("one");
	CHECK(pi == -1);

	pi = list.find("two");
	MRecord r23(m, pi);
	CHECK(r22.label() == "two");
	CHECK(r22.length() == 20);
	CHECK(r22.prev() == 62 + MMEMORY_RESERVE_BYTES);
	CHECK(r22.next() == 0);
	CHECK(r22.ptr() == 0);

	// head
	pi = list.find("five");
	CHECK(pi == 83 + MMEMORY_RESERVE_BYTES);
	list.remove("five");
	pi = list.find("five");
	CHECK(pi == -1);
	pi = list.find("four");
	MRecord r43(m, pi);
	CHECK(r42.label() == "four");
	CHECK(r42.length() == 21);
	CHECK(r42.prev() == 0);
	CHECK(r42.next() == 20 + MMEMORY_RESERVE_BYTES);
	CHECK(r42.ptr() == 0);

	// now should only be two & four left
	int p = list.head();
	MRecord rh(m, p);
	CHECK(rh.label() == "four");
	p = rh.next();
	CHECK(p == 20 + MMEMORY_RESERVE_BYTES);
	CHECK(rh.prev() == 0);
	MRecord rh2(m, p);
	CHECK(rh2.label() == "two");
	CHECK(rh2.next() == 0);
	CHECK(rh2.prev() == 62 + MMEMORY_RESERVE_BYTES);
}

TEST(MListFindOrPrepend, MList)
{
	PMemory m;
	m = new MMemory(MBTESTFILE);
	MList list(m);
	int pi = list.findOrPrepend("one");
	CHECK(pi == MMEMORY_RESERVE_BYTES);

	pi = list.findOrPrepend("one");
	CHECK(pi == MMEMORY_RESERVE_BYTES);

	pi = list.findOrPrepend("two");
	CHECK(pi == 20 + MMEMORY_RESERVE_BYTES);
	//list.dump("1");
}

TEST(MListTreeTests1, MList)
{
	PMemory m;
	m = new MMemory(MBTESTFILE);
	MList list(m);
	list.prepend("one");
	list.prepend("two");
	list.prepend("three");
	//list.dump("1");

	int pi = list.find("one");
	MRecord r1(m, pi);
	MList sublist(r1);
	sublist.prepend("one one");
	sublist.prepend("one two");
	sublist.prepend("one three");
	
	//list.dump("2");

}

TEST(MListTreeTests2, MList)
{
	CString genrename = "rock";
	CString artistname = "Aerosmith";
	CString albumname = "Big Ones";
	
	PMemory m_mem;
	m_mem = new MMemory;
	
	int i;
	for (i = 1 ; i < 4 ; ++i) {
		MList genreList(m_mem);
		int p = genreList.findOrPrepend(genrename);
		MRecord genreRecord(m_mem, p);
		
		MList artistList(genreRecord);
		p = artistList.findOrPrepend(artistname);
		MRecord artistRecord(m_mem, p);

		MList albumList(artistRecord);
		p = albumList.findOrPrepend(albumname);
		MRecord albumRecord(m_mem, p);

		MList titleList(albumRecord);

		char buf[50];
		sprintf(buf, "title %d", i);
		titleList.prepend(buf);
		//genreList.dump("treetest2");
	}
	MList genreList(m_mem);
	int p = genreList.findOrPrepend(genrename);
	MRecord genreRecord(m_mem, p);
	
	MList artistList(genreRecord);
	p = artistList.findOrPrepend(artistname);
	MRecord artistRecord(m_mem, p);

	MList albumList(artistRecord);
	p = albumList.findOrPrepend(albumname);
	MRecord albumRecord(m_mem, p);

	MList titleList(albumRecord);
	p = titleList.head();
	MRecord song1(m_mem, p);
	MRecord song2(m_mem, song1.next());
	MRecord song3(m_mem, song2.next());
	CHECK(song1.label() == "title 3");
	CHECK(song2.label() == "title 2");
	CHECK(song3.label() == "title 1");
	CHECK(song3.next() == 0);

}

TEST(MListTreeTests3, MList)
{
	CString genrename = "rock";
	CString artistname = "Aerosmith";
	CString albumname = "Big Ones";
	
	PMemory m_mem;
	m_mem = new MMemory;
	
	int i;
	for (i = 1 ; i < 4 ; ++i) {
		MList genreList(m_mem);
		MRecord genreRecord = genreList.record(genrename);

		MList artistList(genreRecord);
		MRecord artistRecord = artistList.record(artistname);
		
		MList albumList(artistRecord);
		MRecord albumRecord = albumList.record(albumname);

		MList titleList(albumRecord);

		char buf[50];
		sprintf(buf, "title %d", i);
		titleList.prepend(buf);
		//genreList.dump("treetest3");
	}
	MList genreList(m_mem);
	MRecord genreRecord = genreList.record(genrename);
	
	MList artistList(genreRecord);
	MRecord artistRecord = artistList.record(artistname);

	MList albumList(artistRecord);
	MRecord albumRecord = albumList.record(albumname);

	MList titleList(albumRecord);
	int p = titleList.head();
	MRecord song1(m_mem, p);
	MRecord song2(m_mem, song1.next());
	MRecord song3(m_mem, song2.next());
	CHECK(song1.label() == "title 3");
	CHECK(song2.label() == "title 2");
	CHECK(song3.label() == "title 1");
	CHECK(song3.next() == 0);

}
/////////////////////////////////////////////////////////////////////
MTags::MTags(const CString & genre,
			const CString & artist,
			const CString & album,
			const CString & title,
			const CString & tlen)
			: mgenre(genre),martist(artist),malbum(album),mtitle(title),
			mtlen(tlen)
{}

void
MTags::serialize(AutoBuf & buf) {
	int l = mgenre.GetLength()   + martist.GetLength() 
			+ malbum.GetLength() + mtitle.GetLength() 
			+ mtlen.GetLength()
			+ 27; // LlllDddddddd,.....
	buf.size(l+2);
	sprintf(buf.p,"%04d%s,%04d%s,%04d%s,%04d%s,%04d%s\r\n",
		mgenre.GetLength(), mgenre.GetBuffer(0),
		martist.GetLength(), martist.GetBuffer(0),
		malbum.GetLength(), malbum.GetBuffer(0),
		mtitle.GetLength(), mtitle.GetBuffer(0),
		mtlen.GetLength(), mtlen.GetBuffer(0));
}

// 0004Rock0009Aerosmith

MTags::MTags(char * buf) {
	int l=0;
	char * p = buf;
	
	sscanf(p, "%04d",&l);
	p[l + 4] = 0;
	p += 4;
	mgenre = p;

	p += l+1;
	sscanf(p, "%04d",&l);
	p[l + 4] = 0;
	p += 4;
	martist = p;

	p += l+1;
	sscanf(p, "%04d",&l);
	p[l + 4] = 0;
	p += 4;
	malbum = p;

	p += l+1;
	sscanf(p, "%04d",&l);
	p[l + 4] = 0;
	p += 4;
	mtitle = p;

	p += l+1;
	sscanf(p, "%04d",&l);
	p[l + 4] = 0;
	p += 4;
	mtlen = p;
	
}

TEST(MTags, tagstest)
{
	MTags tag("Rock","Aerosmith","Rocks","Walk this way", "240");
	AutoBuf buf;
	tag.serialize(buf);
	MTags tag2(buf.p);
}

/////////////////////////////////////////////////////////////////////
void
MFiles::add(const CString & file,
			const CString & genre,
			const CString & artist,
			const CString & album,
			const CString & title,
			const CString & tlen) {
	int at;
	if (!contains(file, at)) {
		add(at, file, genre,artist,album,title,tlen);
	}
}
void
MFiles::add(int at, const CString & file,
			const CString & genre,
			const CString & artist,
			const CString & album,
			const CString & title,
			const CString & tlen) {
	m_files.InsertAt(at, file);
	
	MTags tags(genre,artist,album,title,tlen);
	AutoBuf buf;
	tags.serialize(buf);
	m_tags.InsertAt(at, buf.p);
}
void
MFiles::remove(const CString & file) {
	int at;
	if (contains(file, at)) {
		remove(at);

	}
}
void
MFiles::remove(int at) {
	m_files.RemoveAt(at);
	m_tags.RemoveAt(at);
}
Song
MFiles::getSong(const LPCTSTR file) {
	int at = -1;
	Song song = new CSong;
	if (contains(file,at)) {
		CString s = m_tags[at];
		MTags tags(s.GetBuffer(0));
		song->setId3("TCON",tags.mgenre);
		song->setId3("TPE1",tags.martist);
		song->setId3("TALB",tags.malbum);
		song->setId3("TIT2",tags.mtitle);
		song->setId3("FILE",file);
		song->setId3("TLEN",tags.mtlen);
	}
	return song;
}
BOOL
MFiles::contains(const CString & file, int & at) {
	at = 0;

	int low = 0;
	int high = m_files.GetSize() - 1;
	int middle;
	
	while( low <= high )
	{
		middle = ( low  + high ) / 2;
		
		if( file == m_files[  middle ] ) {//match
			at = middle;
			return TRUE;
		} else if( file < m_files[ middle ] )
			high = middle - 1;		//search low end of array
		else
			low = middle + 1;		//search high end of array
	}
	at = low;
	
	return FALSE;		//search key not found

}
int
MFiles::write() {
	CFile file;
    CFileException fileException;
    if (!file.Open( (LPCTSTR)m_loc, 
			CFile::modeCreate | CFile::modeWrite,
			&fileException )) {
		return -1;
	}

	CArchive ar( &file, CArchive::store);
	m_files.Serialize(ar);
	ar.Close();
	file.Close();

    if (!file.Open( (LPCTSTR)m_idx, 
			CFile::modeCreate | CFile::modeWrite,
			&fileException )) {
		return -1;
	}

	CArchive ar2( &file, CArchive::store);
	m_tags.Serialize(ar2);
	ar2.Close();
	file.Close();
	return 0;
}

int
MFiles::read() {
	CFile file;
    CFileException fileException;
    if (!file.Open( (LPCTSTR)m_loc, 
			CFile::modeRead,
			&fileException )) {
		return -1;
	}

	CArchive ar( &file, CArchive::load);
	m_files.RemoveAll();
	m_files.Serialize(ar);
	ar.Close();
	file.Close();

    if (!file.Open( (LPCTSTR)m_idx, 
			CFile::modeRead,
			&fileException )) {
		return -1;
	}

	CArchive ar2( &file, CArchive::load);
	m_tags.RemoveAll();
	m_tags.Serialize(ar2);
	ar2.Close();
	file.Close();
	return 0;
}
void
MFiles::removeAll() {
	m_files.RemoveAll();
	m_tags.RemoveAll();
}
void
MFiles::setDbLocation(const CString & loc) {
	m_dir = loc;
	m_loc = m_dir;
	m_loc += "\\";
	m_loc += MUZIKBROWZER;
	m_loc += ".mbfls";
	m_idx = m_dir + "\\";
	m_idx += MUZIKBROWZER;
	m_idx += ".mbtgs";
}
TEST(MFilesTest, MFiles)
{
	MFiles files;
	files.setDbLocation("..\\testdata");
	files.add("c","genre1","artist1","album1","title1","tlen1");
	files.add("a","genre2","artist2","album2","title2","tlen2");
	files.add("z","genre3","artist3","album3","title3","tlen3");
	files.add("x","genre4","artist4","album4","title4","tlen4");
	files.add("b","genre5","artist5","album5","title5","tlen5");
	files.write();

	files.read();
	int n = files.getLength();
	int i;
	for(i = 0 ; i < n ; ++i) {
		CString tmp = files.getAt(i);
	}
	CHECK(n == 5)
}

	
/////////////////////////////////////////////////////////////////////

MSongLib::MSongLib() : m_songcount(0), m_garbagecollector(0), m_dirty(0),
	m_db_version(MB_DB_VERSION), m_mem(new MMemory)
 {}

MSongLib::~MSongLib()
{
	if (m_dirty) {
		writeToFile();
	}
}

MSongLib::operator = (MSongLib & songlib) {
	m_songcount = songlib.m_songcount;
	m_mem = songlib.m_mem;
}

void
MSongLib::init(BOOL rebuild) {
	m_mem->create();
	m_songcount = 0;
	m_garbagecollector = 0;
	if (rebuild)
		m_files.removeAll();
}
int
MSongLib::addSong(Song & song) {
	//PMemory m_mem(MBTESTFILE);
	++m_songcount;
    CString artistname, albumname, titlename, genrename,year,track,file,tlen;

	file = song->getId3("FILE");

	// Prevent same file being added more than once
	// Now gonna manage it nicely, being used by loadplaylist et al
	int at;
	if (m_files.contains(file, at)) {
//		m_files.remove(at);
		return 0;
	} 


	artistname = song->getId3("TPE1");
	albumname = song->getId3("TALB");
	titlename = song->getId3("TIT2");
    genrename = song->getId3("TCON");
	tlen = song->getId3("TLEN");

	m_files.add(at, file,genrename,artistname,albumname,titlename,tlen);

	MList genreList(m_mem);
	MList artistList = genreList.list(genrename);
	MList albumList = artistList.list(artistname);
	MList titleList = albumList.list(albumname);
	MList id3List = titleList.list(titlename, 1);

	MList allArtistsList = genreList.list(MBALL);
	MList allAlbumList = allArtistsList.list(artistname);
	MList allTitleList = allAlbumList.list(albumname);
	allTitleList.prepend(titlename);

	MRecord titleRecord = titleList.record(titlename);
	MRecord allTitleRecord = allTitleList.record(titlename);

    AutoBuf buf(1000);

    POSITION pos;
    for (pos = song->_obj.GetStartPosition(); pos != NULL;) {
        CString key,val;
        song->_obj.GetNextAssoc(pos, key, val);
        if (val != "") {
		    sprintf(buf.p, "%s %s", (LPCTSTR)key, (LPCTSTR)val);
			id3List.prepend(buf.p);
        }
    }
	allTitleRecord.ptr() = titleRecord.ptr();

	MList allAllAlbumList = allArtistsList.list(MBALL);
	MList allAllTitleList = allAllAlbumList.list(albumname);
	allAllTitleList.prepend(titlename);
	MRecord allAllTitleRecord = allAllTitleList.record(titlename);
	allAllTitleRecord.ptr() = titleRecord.ptr();

    return 1;
}
void
MSongLib::removeSong(Song & song2remove) {
    CString artistname, albumname, songname, genrename;
	artistname = song2remove->getId3("TPE1");
	albumname = song2remove->getId3("TALB");
	songname = song2remove->getId3("TIT2");
    genrename = song2remove->getId3("TCON");
	CString filename = song2remove->getId3("FILE");

	MList genreList(m_mem);
	MList artistList = genreList.list(genrename);
	MList albumList = artistList.list(artistname);
	MList songList = albumList.list(albumname);
	MList * tagList = NULL;
	MRecord * song = NULL;
	if (filename.GetLength()) {
		MList::Iterator songIter(songList);
		BOOL found = FALSE;
		while (songIter.more() && !found) {
			if (song != NULL)
				delete song;
			song = new MRecord (songIter.next() );
			CString sfile = song->lookupVal("FILE");
			if (filename == sfile) {
				tagList = new MList ( *song );
				found = TRUE;
			}
		}
	} else {
		tagList = new MList (songList.list(songname));
		song = new MRecord ( albumList.record(songname));
	}
	if (tagList != NULL) {
		MList::Iterator tagIter(*tagList);
		while (tagIter.more()) {
			tagList->remove(tagIter.next().label());
		}
		delete tagList;
	}
	songList.remove(*song);
	delete song;

	if (songList.count() == 0) {
		albumList.remove(albumname);
		if (albumList.count() == 0) {
			artistList.remove(artistname);
			if (artistList.count() == 0) {
				genreList.remove(genrename);
			}
		}
	}

	// all/artist removeall
	MList allArtistList = genreList.list(MBALL);
	MList allAlbumList = allArtistList.list(artistname);
	MList allSongList = allAlbumList.list(albumname);
//	allSongList.remove();

	song = NULL;
	if (filename.GetLength()) {
		MList::Iterator songIter(allSongList);
		BOOL found = FALSE;
		while (songIter.more() && !found) {
			if (song)
				delete song;
			song = new MRecord (songIter.next() );
			CString sfile = song->lookupVal("FILE");
			if (filename == sfile) {
				found = TRUE;
			}
		}
	} else {
		song = new MRecord ( allAlbumList.record(songname));
	}
	allSongList.remove(*song);
	delete song;

	if (allSongList.count() == 0) {
		allAlbumList.remove(albumname);
		if (allAlbumList.count() == 0) {
			allArtistList.remove(artistname);
			if (allArtistList.count() == 0) {
				genreList.remove(MBALL);
			}
		}
	}

	// all/all removeall
	MList allAllArtistList = genreList.list(MBALL);
	MList allAllAlbumList = allAllArtistList.list(MBALL);
	MList allAllSongList = allAllAlbumList.list(albumname);

	song = NULL;
	if (filename.GetLength()) {
		MList::Iterator songIter(allAllSongList);
		BOOL found = FALSE;
		while (songIter.more() && !found) {
			if (song)
				delete song;
			song = new MRecord (songIter.next() );
			CString sfile = song->lookupVal("FILE");
			if (filename == sfile) {
				found = TRUE;
			}
		}
	} else {
		song = new MRecord ( allAllAlbumList.record(songname));
	}
	allAllSongList.remove(*song);
	delete song;

	if (allAllSongList.count() == 0) {
		allAllAlbumList.remove(albumname);
		if (allAllAlbumList.count() == 0) {
			allAllArtistList.remove(MBALL);
			if (allAllArtistList.count() == 0) {
				genreList.remove(MBALL);
			}
		}
	}
	m_files.remove(filename);
}
void
MSongLib::removeAlbum(const CString & genrename, const CString & artistname,
					  const CString & albumname) {
	MList genreList(m_mem);
	MList artistList = genreList.list(genrename);
	MList albumList = artistList.list(artistname);
	MList songList = albumList.list(albumname);

	MList::Iterator songIter(songList);
	while (songIter.more()) {
		removeSong(songIter.next().createSong());
	}
}
void
MSongLib::removeArtist(const CString & genrename,
					   const CString & artistname) {
	MList genreList(m_mem);
	MList artistList = genreList.list(genrename);
	MList albumList = artistList.list(artistname);
	MList::Iterator albumIter(albumList);
	while(albumIter.more()) {
		removeAlbum(genrename, artistname, albumIter.next().label());
	}
}
void
MSongLib::removeGenre(const CString & genrename) {
	MList genreList(m_mem);
	MList artistList = genreList.list(genrename);
	MList::Iterator artistIter(artistList);
	while (artistIter.more()) {
		removeArtist(genrename, artistIter.next().label());
	}
}
void
MSongLib::setDbLocation(const CString & loc) {
	m_mem->setDbLocation(loc);
	m_files.setDbLocation(loc);
}
int
MSongLib::head() {
	return m_mem->readi(0);
}
void
MSongLib::readFromFile() {
	int r = m_mem->readFromFile();
	if (r == 0) {
		m_db_version = m_mem->readi(12);
		r = validate();
	}
	if (r == 0) {
		m_songcount = m_mem->readi(4);
		m_garbagecollector = m_mem->readi(8);
		if (m_files.read() == -1) { // create files list if !exists
			int a,r;
			r = MBMessageBox("Advisory","Database maintenance required.\r\nClick OK to continue.",TRUE,TRUE);
			if (IDOK == r)
				verify("Performing database maintenance", a, r);
			else
				_exit(0);
//		} else {
//			m_files.removeAll();
		}
	}
	m_dirty = 0;
}
void
MSongLib::writeToFile() {
	m_mem->writei(4,m_songcount);
	m_mem->writei(8,m_garbagecollector);
	m_mem->writei(12,MB_DB_VERSION);
	m_mem->writeToFile();
#ifdef _DEBUG
	dump();
#endif
	m_dirty = 0;
	m_files.write();
//	m_files.removeAll();

}

//   use an std::auto_ptr here to handle object cleanup automatically
//   ID3_Tag::Iterator* iter = myTag.CreateIterator();
//   ID3_Frame* myFrame = NULL;
//   while (NULL != (myFrame = iter->GetNext()))
//   {
//     // do something with myFrame
//   }
//   delete iter;

MList
MSongLib::genreList() {
	MList g(m_mem);
	return g;
}
MList
MSongLib::artistList(const CString & genrename) {
	MList genrelist = genreList();
	MList artistlist = genrelist.list(genrename, FALSE);
	return artistlist;
}
MList
MSongLib::albumList(const CString & genrename, const CString & artistname) {
	MList artistlist = artistList(genrename);
	MList albumlist = artistlist.list(artistname, FALSE);
	return albumlist;
}
MList
MSongLib::songList(const CString & genrename, const CString & artistname,
				   const CString & albumname) {
	MList albumlist = albumList(genrename, artistname);
	MList songlist = albumlist.list(albumname, FALSE);
	return songlist;
}
MRecord
MSongLib::getSong(const CString & genrename, const CString & artistname,
				   const CString & albumname, const CString & songname) {
	MList songlist = songList(genrename, artistname, albumname);
	MRecord song = songlist.record(songname);
	return song;
}
CString
MSongLib::getSongVal(const CString & key, const CString & genrename,
					 const CString & artistname, const CString & albumname,
					 const CString & songname) {
	MRecord song = getSong(genrename, artistname, albumname, songname);
	CString val = song.lookupVal(key);
	return val;
}
CString
MusicLib::getSongVal(const CString & key, const CString & genrename,
					 const CString & artistname, const CString & albumname,
					 const CString & songname) {
	CString val = m_SongLib.getSongVal(key, genrename, artistname, albumname,
		songname);
	return val;
}

void 
MSongLib::dump(CString name) {
    int i;
    AutoBuf buf(1000);

    CString dbfilename = m_mem->DbFile();

	if (name != "") {
		dbfilename += name;
	}
    dbfilename += ".txt";

    const char * pszFileName = (LPCTSTR) dbfilename;
	CFile myFile;
	CFileException fileException;

	if ( !myFile.Open( pszFileName,
        CFile::modeWrite | CFile::modeCreate,
        &fileException ))
	{
        CString msg = "Unable to create muzikbrowzer database file ";
		msg += dbfilename;
        MBMessageBox(CString("alert"), msg);
        return ;
	}

    sprintf(buf.p, 
"DBV:%d Head:%07d Size:%07d SongCount:%d gc:%d ###########################################\n",
 m_db_version, head(), m_mem->m_size, m_songcount, m_garbagecollector);
    myFile.Write(buf.p, strlen(buf.p));
    sprintf(buf.p, "%7s %7s %7s %7s %7s %s\n", "pos", "length", "prev", "next",
        "ptr", "label");
    myFile.Write(buf.p, strlen(buf.p));
    i = (MMEMORY_RESERVE_BYTES);
    while (i < m_mem->next()) {
		MRecord r(m_mem, i);

        sprintf(buf.p, "%07d %07d %07d %07d %07d %s\n",i,
			r.length(), r.prev(), r.next(), r.ptr(), r.label());
        myFile.Write(buf.p, strlen(buf.p));
		if (r.length() < 1) {
			CString msg = "Error: length < 1 found";
			MBMessageBox(CString("Alert"), msg);
			return;
		}
        i += r.length();
    }


    myFile.Flush();

}
int
MSongLib::validate() {
	if (m_db_version != MB_DB_VERSION) {
		init(TRUE);
		CString msg = "The muzikbrowzer database appears to be an\r\nolder incompatible version.\r\n";
		msg += "You must rebuild by doing a Scan in Configuration.";
		MBMessageBox("Error", msg);
		return -1;
	}
    int i = (MMEMORY_RESERVE_BYTES);
	int next = m_mem->next();
    while (i < next) {
		MRecord r(m_mem, i);
		int rlength = r.length();
		if (r.prev() < 0 || r.prev() > next
			|| r.next() < 0 || r.next() > next
			|| r.ptr() < -1 || r.next() > next
			|| r.ptr() >= next
			|| rlength < 1)
		{
			init(TRUE);
			CString msg = "muzikbrowzer database corrupted.\r\n";
			msg += "Rebuild by doing a Scan in config.";
			MBMessageBox("Error", msg);
			return -1;
		}
		char * label = (char*)m_mem->addr(i + sizeof(MRecordt));
		if (label) {
			int llen = strlen(label);
			if (llen != rlength - (sizeof(MRecordt) + 1)) {
				init(TRUE);
				CString msg = "muzikbrowzer database corrupted.\r\n";
				msg += "Rebuild by doing a Scan in config.";
				MBMessageBox("Error", msg);
				return -1;
			}
		}
        i += rlength;
    }
	return 0;
}


CString
MSongLib::verify(CString msg, int & total_albums, int & total_songs) {
    CString results, contents, removed;
    int num_genres = 0;
    int total_artists = 0;
	int total_song_count = 0;
	Playlist removeList;
	m_files.removeAll();

    InitDlg * dialog = new InitDlg(1,0);
	dialog->SetLabel(msg);
	dialog->ShowWindow(SW_SHOWNORMAL);
//	dialog->SendUpdateStatus(2, "", 0, count());
	dialog->ProgressRange(0, count());
	dialog->UpdateWindow();
	
    AutoBuf buf(1000);
    contents = "\r\nLibrary contains:\r\n";
	MList genrelist = genreList();
	MList::Iterator genreIter(genrelist);
    while (genreIter.more()) {
		MRecord genre(genreIter.next());
		int num_artists = 0;
		int num_albums = 0;
		int num_songs = 0;
		if (genre.label() != MBALL) {
			num_genres++;
			MList artistList(genre);
			MList::Iterator artistIter(artistList);
			while (artistIter.more()) {
				num_artists++;
				MList albumList(artistIter.next());
				MList::Iterator albumIter(albumList);
				while (albumIter.more()) {
					num_albums++;
					MList songList(albumIter.next());
					MList::Iterator songIter(songList);
					while (songIter.more()) {
						MRecord song = songIter.next();
						CString file = song.lookupVal("FILE");
						if (!FileUtil::IsReadable(file)) {
							Song rem = song.createSong();
							removeList.append(rem);
							removed += file + "\r\n";
						} else {
							num_songs++;
							m_files.add(file,
								song.lookupVal("TCON"),
								song.lookupVal("TPE1"),
								song.lookupVal("TALB"),
								song.lookupVal("TIT2"),
								song.lookupVal("TLEN")
								);
						}
						dialog->ProgressPos(total_song_count++);
						dialog->UpdateWindow();
					}
				}
			}
			total_artists += num_artists;
			total_albums += num_albums;
			total_songs += num_songs;
			sprintf(buf.p, "%s %d artists, %d albums, %d songs\r\n", (LPCTSTR)genre.label(),
				num_artists, num_albums, num_songs);
			contents += buf.p;
		}
    }

	int removed_count = 0;
	for (PlaylistNode *p = removeList.head(); p != (PlaylistNode*)0; p = removeList.next(p)) 
	{
		removeSong(p->_item);
		++removed_count;
	}

    sprintf(buf.p, "totals %d genres, %d artists, %d albums, %d songs\r\n",
        num_genres, total_artists, total_albums, total_songs);
    contents += buf.p;
	if (removed_count) {
		results += numToString(removed_count)
		+ " audio files removed or renamed.\r\n";
		results += "Files removed/renamed:\r\n"
		+ removed + "\r\n";
		m_garbagecollector++;
	}
	results += contents;
	m_files.write();
//	m_files.removeAll();
	dialog->EndDialog(IDOK);
	delete dialog;
    return results;
}

int
MusicLib::setSongVal(const CString & key, const CString & value, 
			const CString & genrename,
			const CString & artistname, const CString & albumname,
			const CString & songname)
{
	int r = m_SongLib.setSongVal(key, value, genrename, artistname,
								   albumname, songname);
	return r;
}

int
MSongLib::setSongVal(const CString & key, const CString & value, 
			const CString & genrename,
			const CString & artistname, const CString & albumname,
			const CString & songname)
{
	int r = 0;
	MRecord song = getSong(genrename, artistname, albumname, songname);
	MList id3list(song);
	id3list.removeg(key);
	CString kv = key;
	kv += " ";
	kv += value;
	id3list.prepend(kv);
	m_garbagecollector++;
	m_dirty = 1;
	return r;
}
//TEST(setSongVal, MSongLib)
//{
//	MSongLib songlib;
//	songlib.init();
//	songlib.setDbLocation(".");
//
//	Song song = new CSong;
//	song->setId3("TALB", CString("Big Ones"));
//	song->setId3("TCON", CString("rock"));
//	song->setId3("TIT2", CString("Walk On Water"));
//	song->setId3("TLEN", CString("295366"));
//	song->setId3("TPE1", CString("Aerosmith"));
//	song->setId3("TRCK", CString("1"));
//
//	songlib.addSong(song);
////	songlib.writeToFile();
//
//	songlib.setSongVal("TLEN", "12345", "rock", "Aerosmith", "Big Ones", "Walk On Water");
////	songlib.writeToFile();
//	
//}

//	MRecord song = getSong(genrename, artistname, albumname, songname);
//	CString val = song.lookupVal(key);
//TEST(removeSong, MSongLib)
//{
//	MSongLib songlib;
//	songlib.init();
//	songlib.setDbLocation(CString("."));
//	
//	Song song = new CSong;
//	song->setId3("TALB", CString("Big Ones"));
//	song->setId3("TCON", CString("rock"));
//	song->setId3("TIT2", CString("Walk On Water"));
//	song->setId3("TLEN", CString("295366"));
//	song->setId3("TPE1", CString("Aerosmith"));
//	song->setId3("TRCK", CString("1"));
//
//	songlib.addSong(song);
////	songlib.addAllGenre();
////	songlib.writeToFile();
//
//	songlib.removeSong(song);
////	songlib.writeToFile();
//
//	songlib.addSong(song);
////	songlib.writeToFile();
//
//	songlib.addSong(song);
//	songlib.removeGenre("rock");
////	songlib.writeToFile();
//}

//TEST(garbageCollect, MSongLib)
//{
//	ID3_Tag id3old;
//	ID3_AddArtist(&id3old, "Aerosmith");
//	ID3_AddAlbum(&id3old, "Big Ones");
//	ID3_AddTitle(&id3old, "Walk On Water");
//	ID3_AddGenre(&id3old, "rock");
//	ID3_AddComment(&id3old, "xyzzy");
//
//	ID3_Tag id3new (id3old);
//	ID3_AddArtist(&id3new, "Aerosmithxxx", true);
//
//	MSongLib songlib;
//	songlib.init();
//	songlib.setDbLocation(CString("."));
//	
//	Song song = new CSong;
//	song->setId3("TALB", CString("Big Ones"));
//	song->setId3("TCON", CString("rock"));
//	song->setId3("TIT2", CString("Walk On Water"));
//	song->setId3("TPE1", CString("Aerosmith"));
//
//	songlib.addSong(song);
//
//
//}
