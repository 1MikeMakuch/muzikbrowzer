
#ifndef __MUSIC_DB_H__
#define __MUSIC_DB_H__

#include "stdafx.h"
#include "MPtr.h"
#include "ExtendedListBox.h"
#include "id3/tag.h"
#include "MBGlobals.h"
#include "PicCache.h"
#include "MyString.h"

class MyLog;

// A MemDb is a hierarchical tree looking structure not
// unlike a file system directory structure. It is comprised
// of doubly linked lists, each list node is either a data
// node or a pointer to another list, hence the analogy
// to directories.  A node is comprised of 5 fields which
// reside in one contigous memory space:
// 
// length		(int)	total length of record
// previous	(int)	pointer to previous record in list
// next		(int)	pointer to next record in list
// data		(int)	pointer to subtree
// label		char[]	name
// 
// length is never 0.  If a pointer is 0 then there ain't one
// i.e. ain't a prev, next, or data list. labels are always
// 0 terminated to make it simpler to retrieve them, i.e.
// just return the address of the label, i.e. (char*)(p +
// MEMDB_LABL_OS)
// 
// The requirements behind MemDb design:
// 
// 1 read/write entire db structure to/from disk as quickly as possible
// 2 minimize memory usage
// 3 performance
// 4 satisfy all requirements with 100's of thousands of records
// 
// C++ Objects could not be used. Serialization and parsing
// for i/o proved to be way too slow. And objects way too
// big, imagine having 100,000 thousand objects created in
// a running program. What I needed was to have the entire
// db contents and necessary data structure contained in one
// contiguous memory space.
// 
// This allows the fastest possible i/o to/from disk. Allows
// the entire db to be read into memory at all times with
// a relatively tiny memory footprint.  Performance penalty
// of the current design (linked lists) is small enough that
// it works very well for MusicLib. Many sequential link-list
// searches are done but they're all very small searches and
// very fast relative to the speed at which a human browses
// through their music library.



typedef CMapStringToString SongKeys;
class CSong;
typedef MPtr<CSong> Song;

class CSong {
	public:
		CSong();
		CSong(Song &);
		~CSong();
		void reference () { _refcnt++;}
		void unreference();
//		int operator == (const CSong &);
		SongKeys _obj;
		int setId3(const CString &, const CString &);
		CString getId3(const CString &, int unknown = 1);
        SongKeys * createSongKeys();
		int removeId3(const CString &);
		int GetCount() { return _obj.GetCount(); }
		BOOL Contains(const CString & kw);
//        CString serialize();
	private:
		int _refcnt;
};

typedef MPtrDLLNode<Song> PlaylistNode;
typedef MPtrDLL<Song> Playlist;

class MList;
class MRecord;

class MMemory {
	friend class MRecord;
	friend class MList;
	friend class MSongLib;
	public:
		MMemory(const CString & file);
		MMemory();
		~MMemory();
		void reference () { _refcnt++;}
		void unreference();
		int writeToFile();
		int readFromFile();
		int readi(int p);
		char * readc(int p);
		int writei(int p, int val);
		int writec(int p, char * val);
		int alloc(int length);
		void setDbLocation(const CString & loc);
		CString getDbLocation() { return m_dir; }
		CString DbFile() { return m_file; }
//		int release() {}
		operator = (MMemory &);
		BOOL dbchanged();
	private:
		int _refcnt;
		char * m_space;
		int m_size;
		int m_sizeOrig;
		int m_next;
		CString m_dir;
		CString m_file;
		void create();
		void * addr(int p);
		int next() { return m_next; }
		CTime m_mtime;
};
typedef MPtr<MMemory> PMemory;

typedef struct DbRecordStruct{
	int length;
	int prev;
	int next;
	int ptr;
} MRecordt;

class MRecord {
	friend class MList;
	friend class MSongLib;
	public:
		MRecord(PMemory & m, int p);
		MRecord(MRecord & r);
		~MRecord(){};
		int & length();
		int & prev();
		int & next();
		int & ptr();
		CString label();
		void label(const CString &);
		static int needed(const CString &);
#pragma hack
		// xxx hack alert: getVal should be in something like MSong
		CString getKey();
		CString getVal();
		CString lookupVal(const CString & key);
		Song createSong();
		int i() { return m_i; }
	protected:
		PMemory & m_mem;
		int m_i;
		PMemory & mem() { return (PMemory &) m_mem; }
		// read only
		//const PMemory & memRO() { return (PMemory &) m_mem; }
		int ptrIdx();
};

class MList {
	public:
		MList(PMemory & m);
		MList(MRecord & r);
		// empty list
		MList();
		MList (MList &);
//		a read only MList?
//		MList(const MRecord & r);
		~MList(){};
		int prepend(const CString & label);
		void remove(const CString & label);
		void removeg(const CString & label);
		void remove(MRecord & record);
		int find(const CString & label);
		int findg(const CString & label);
		int findOrPrepend(const CString & label);
		MRecord record(const CString  &label, BOOL forcecreate = 0);
		MList list(const CString  &label, BOOL forcecreate = 0);

		int & head();
		int count();

		class Iterator {
			public:
				Iterator(MList & list) : m_list(list) {
					if (m_list.m_empty)
						pos = 0;
					else
						pos = m_list.head();
				}
				~Iterator() {};
				MRecord next();
				//BOOL operator () { return (pos > 0); }
				BOOL more() { return (pos > 0); }
			private:
				MList & m_list;
				int pos;
		};
		friend class Iterator;
	protected:
		int m_headstore;
		PMemory & m_mem;
		BOOL m_empty;
	private:
};

class MTags {
	public:
		MTags(const CString & genre,
			const CString & artist,
			const CString & album,
			const CString & title,
			const CString & tlen);
		MTags(char * buf);
		~MTags(){};
		void serialize(AutoBuf & buf);
		CString mgenre,martist,malbum,mtitle,mtlen;
};

class MFiles {
	public:
		MFiles():m_count(0) {
//			m_files.SetSize(1,100); // doesn't work right
		}
		~MFiles(){}
		void add(const CString & file,
			const CString & genre="",
			const CString & artist="",
			const CString & album="",
			const CString & title="",
			const CString & tlen="");
		void add(int at, const CString & file,
			const CString & genre="",
			const CString & artist="",
			const CString & album="",
			const CString & title="",
			const CString & tlen="");
		void remove(const CString & file);
		void remove(int at);
		BOOL contains(const CString & file, int & at);
		BOOL contains(const CString & file) {
			int at;
			return contains(file, at);
		}
		int write();
		int read();
		void setDbLocation(const CString & loc);
		void removeAll();
		int getLength() { return m_files.GetSize(); }
		CString getAt(int i) { return m_files.GetAt(i); }
		Song getSong(const LPCTSTR file);
		int count() { return m_count; }
	private:
		CString m_loc;
		CString m_dir;
		CString m_idx;
		CStringArray m_files;
		CStringArray m_tags;
		int m_count;
};

class MSongLib {
	public:
		MSongLib();
		~MSongLib();
		void init(BOOL rebuild);
		int addSong(Song &song);
		int addSong0(Song &song);
		MList artistList(const CString & genrename);
		MList albumList(const CString & genrename,
			const CString & artistname);
		int count() { return m_songcount; }
		MList genreList();
		CString getDbLocation() { return m_mem->getDbLocation(); }
		CString getSongVal(const MRecord & r, const CString & key);
		MRecord getSong(const CString & genrename,
			const CString & artistname, const CString & albumname,
			const CString & songname);
		Song getSong(int pi);
		CString getSongVal(const CString & key, const CString & genrename,
			const CString & artistname, const CString & albumname,
			const CString & songname);
		void readFromFile();
		void removeSong(Song & song);
		void removeAlbum(const CString & genrename,
			const CString & artistname, const CString & albumname);
		void removeArtist(const CString & genrename,
			const CString & artistname);
		void removeGenre(const CString & genrename);
		void setDbLocation(const CString  &loc);
		MList songList(const CString & genrename,
			const CString & artistname, const CString & albumname);

		int setSongVal(const CString & key, const CString & value, 
			const CString & genrename,
			const CString & artistname, const CString & albumname,
			const CString & songname);

		operator = (MSongLib &);
		void ShortCopy(MSongLib & );
		void Destroy();
//		CString verify(CString msg, int &, int &, const int);
		void writeToFile();
		BOOL checkReReadDb();
		int m_garbagecollector;
		MFiles m_files;
	private:
		PMemory m_mem;
		int m_songcount;
		int head();
		int m_dirty;
		int m_db_version;
		

		int validate();
	public:
		void dump(CString name="");
		void dumpRecords(CString x);
};

typedef MPtr<MSongLib> MSongLibP;
#include <afxtempl.h>
#include "SortedArray.h"

class ExportDlg;
class ProgressDlg;
class MBTag;
class MusicLib {
	public:
		MusicLib();
		~MusicLib();

	Playlist	_playlist;
	     int	addAlbumToPlaylist(const CString &, const CString &, const CString &);
	     int	addArtistToPlaylist(const CString &, const CString &);
	     int	addFileToPlaylist(const CString & file);
	     int	addGenreToPlaylist(const CString &);
	    UINT	addSongToDb(ProgressDlg *pd, Song &, const CString & file = "");
	     int	addSongToPlaylist(const CString &, const CString &, const CString &, const CString &);
	     int	addSongToPlaylist(const Song &);
	    BOOL	apic(const CString & file, uchar *& rawdata, size_t & size, const CString & album);
		BOOL	checkReReadDb();
	     int	clearPlaylist();
unsigned int	countSongsInGenre(const CString & genre, CArray<unsigned int, unsigned int> & array);
	    Song	createSongFromFile(const CString & mp3file);
	    Song	createSongFromFile(const CString & mp3file, CString & error_results, int & t1, int & t2, int & fc);
		Song	createSongFromMBTag(MBTag & tag);
		void	DeleteDb();
	    void	deletePlaylist(const CString &);
	     int	deleteSongFromPlaylist(PlaylistNode *p);
		BOOL	deleteSong(ProgressDlg *p, Playlist & songs, CString & results);
	    void	dumpPL(int p);
		void	preExport(ExportDlg *);
		void	export(ProgressDlg *pd, ExportDlg *, MyLog & html, MyLog & csv, 
					CSortedArray<CString, CString&> & genrelist,
					CString & t1, CString & t2, CString & t3, CString & t4);
		void	exportCsv(ExportDlg *, MyLog *, Song);
		void	exportTxt(ExportDlg *, MyLog *, Song);
		void	exportHtml(ExportDlg *, MyLog *, Song, CString & tmpl);
		CString expQE(const CString & entry, const CString in);
		void    garbageCollectTest();
	     int	getAlbums(const CString &, const CString &, CExtendedListBox&, BOOL albumsortalpha=TRUE);
	     int	getArtists(const CString &, CExtendedListBox&);
	 CString	getDbLocation() { return m_dir; };
	 CString 	getComments(double & rggain, const CString & file);
	     int	getGenres(CExtendedListBox&);
	     int	getGenres(CStringArray&);
	 CString	getLibraryCounts();
	     int	getPlaylist(CExtendedListBox &);
	     int	getPlaylistNames(CExtendedListBox &);
	     int	getPlaylistNames(CStringArray &);
		 void	getRandomPlaylist(const CString & genre=MBALL);
		 int	getSongCount();
	 CString	getSongFileName(const int i);
	    Song	getSong(const CString & genre, const CString & artist, const CString & album, const CString & title);
	     int	getSongs(const CString &, const CString &, const CString &, CExtendedListBox&);
	     int	getSongsInPlaylist(const CString & name, CExtendedListBox &);
	     int	getSongsInPlaylist(const CString & name, CStringArray & desc, CStringArray & plist, CDWordArray & tlenArray);
	 CString	getSongVal(const CString & key, const CString & genre, const CString & artist, const CString & album, const CString & song);
	     int	getTotalMp3s() { return m_totalMp3s; }
	     int	init(const BOOL rebuildOnly=FALSE);
	     int	iSearch(const CString name, MSongLib & db, MSongLib & results);
	     int	loadOldPlaylist(const CString & name, CStringList & playlist);
	     int	loadPlaylist(const CString & name, CString & errormsg);
	MSongLib	m_SongLib;
		BOOL	preDeleteSong(Song & song,CStringArray & deletes);
	    BOOL	preModifyID3(Song & old, Song & newSong);
		BOOL	modifyID3(ProgressDlg *p, Playlist & songs, Song & newsong, CString & results);
	    void	movePlaylistDown(int plc, int sel);
	    void	MovePlaylistsToDir();
	    void	movePlaylistUp(int plc, int sel);
		void	NormalizeTagField(CString & tag);
	    void	RandomizePlaylist();
	    UINT	readDb();
	    void	readDbLocation();
		void	RebuildOnly(const CStringArray & dirs,const CStringArray & excludes);
	    BOOL	renamePlaylist(const CString src,const CString dest, BOOL overwrite=FALSE);
	    void	savePlaylist(const CString &);
	    void	savePlaylist(const CStringArray & list, const CString & file);
	    void	savePlaylist(Playlist & playlist, const CString & file);
		BOOL	Scan(const CStringArray & dirs, const CStringArray & excludes, BOOL bNew, BOOL bAdd);
	 CString	scanDirectories(const CStringArray & dirs,const CStringArray & excludes, ProgressDlg*, BOOL scanNew, BOOL bAdd);
	     int	Search(const CString name);
	    void	SearchCancel();
	    void	SearchClear();
	    void	searchForAlbums(Playlist & songs, MList & albumList);
	    void	searchForArtists(Playlist & songs, MList & artistList);
	    void	searchForMp3s(Playlist & songs, Song & song);
	    void	searchForSongs(Playlist & songs, MList & songList,	   const	CString & song = "");
	    void	SearchSetup();
	    void	setDbLocation(const CString & loc);
	     int	setSongVal(const CString & key, const CString & value, const CString & genrename, const CString & artistname, const CString & albumname, const CString & songname);
	    void	shufflePlaylist();
		void	updatePlaylist(Song oldsong, Song newsong);
		CString	JustVerify();
	    UINT	writeDb();
	 CString	writeSongToFile(Song song);
private:
		BOOL m_Searching;
		MSongLib m_SaveLib;
		PicCache m_picCache;
        CString m_dir;
		CString m_file;
        int m_totalMp3s;
        CStringList m_mp3Extensions;
		CString m_libCounts;
		MFiles * m_pSearchFiles;
		BOOL m_RebuildOnly;

        int scanDirectory(ProgressDlg * pd, CStringArray & mp3files, 
			const CStringArray & excludes, const CString &,
			BOOL scanNew, BOOL bAdd);
        int garbageCollect(ProgressDlg * dlg, BOOL test=FALSE);

		CString IgetLibraryCounts();
        
};




#endif
