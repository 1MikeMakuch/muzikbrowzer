
#ifndef __MUSIC_DB_H__
#define __MUSIC_DB_H__

#include "stdafx.h"
#include "MPtr.h"
#include "InitDlg.h"
//#include "Genres.h"
#include "ExtendedListBox.h"
#include "id3/tag.h"
//#include "ID3ModifyStatus.h"
//#include "vorbis/vorbisfile.h"
#include "MBGlobals.h"
#include "PicCache.h"

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

class CSong {
	public:
		CSong();
		~CSong();
		void reference () { _refcnt++;}
		void unreference();
//		int operator == (const CSong &);
		SongKeys _obj;
		int setId3(const CString &, const CString &);
		CString getId3(const CString &, int unknown = 1);
        SongKeys * createSongKeys();
		int removeId3(const CString &);
//        CString serialize();
	private:
		int _refcnt;
};

typedef MPtr<CSong> Song;

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
	private:
		char * m_space;
		int m_size;
		int m_sizeOrig;
		int m_next;
		CString m_dir;
		CString m_file;
		void create();
		void * addr(int p);
		int next() { return m_next; }
};

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
		MRecord(MMemory & m, int p);
		MRecord(MRecord & r);
		~MRecord(){};
		int & length();
		int & prev();
		int & next();
		int & ptr();
		CString label();
		void label(const CString &);
		static int needed(const CString &);
		// xxx hack alert: getVal should be in something like MSong
		CString getKey();
		CString getVal();
		CString lookupVal(const CString & key);
		Song createSong();
		int i() { return m_i; }
	protected:
		MMemory & m_mem;
		int m_i;
		MMemory & mem() { return (MMemory &) m_mem; }
		int ptrIdx();
};

class MList {
	public:
		MList(MMemory & m);
		MList(MRecord & r);
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
		MMemory & m_mem;
//		const MMemory & m_memc;
};

class MFiles {
	public:
		MFiles() {
//			m_files.SetSize(1,100); // doesn't work right
		}
		~MFiles(){}
		void add(const CString & file);
		void add(int at, const CString & file);
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
	private:
		CString m_loc;
		CStringArray m_files;

		
};

class MSongLib {
	public:
		MSongLib();
		~MSongLib();
		void init();
		int addSong(Song &song);
		MList artistList(const CString & genrename);
		MList albumList(const CString & genrename,
			const CString & artistname);
		int count() { return m_songcount; }
		MList genreList();
		CString getDbLocation() { return m_mem.getDbLocation(); }
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
		CString verify(CString msg, int &, int &);
		void writeToFile();

		int m_garbagecollector;
		MFiles m_files;
	private:
		MMemory m_mem;
		int m_songcount;
		int head();
		int m_dirty;
		int m_db_version;
		

		int validate();
	public:
		void dump();
};


class OggTag;

class MusicLib
{
	public:
		MusicLib(InitDlg *);
		~MusicLib();

        int addAlbumToPlaylist(const CString &, const CString &,
			const CString &);
        int addArtistToPlaylist(const CString &, const CString &);
        int addGenreToPlaylist(const CString &);
        UINT addSongToDb(int & ctr, Song &, const CString & file = "");
        int addSongToPlaylist(const CString &, const CString &,
			const CString &, const CString &);
//        int addSongToPlaylist(int);
        int addSongToPlaylist(const Song &);
		BOOL apic(const CString & file, uchar *& rawdata, size_t & size);
        int clearPlaylist();
        Song createSongFromId3(ID3_Tag *);
		Song createSongFromOgg(OggTag *);
		Song createSongFromFile(const CString & mp3file);
		Song createSongFromFile(const CString & mp3file,
			CString & error_results, int & t1, int & t2, int & fc);
        void deletePlaylist(const CString &);
        int deleteSongFromPlaylist(PlaylistNode *p);
        void dumpPL(int p);
        int getAlbums(const CString &, const CString &, CExtendedListBox&);
        int getArtists(const CString &, CExtendedListBox&);
        CString getDbLocation() { return m_dir; };
        int getGenres(CExtendedListBox&);
        int getGenres(CStringList&);
//        ID3_Tag getId3(const CString & genre, const CString & artist,
//			const CString & album, const CString & title);
		Song getSong(const CString & genre, const CString & artist,
			const CString & album, const CString & title);
//        CString getId3(void * p, const CString &);
        int getPlaylist(CExtendedListBox &);
        int getPlaylistNames(CExtendedListBox &);
		int getPlaylistNames(CStringList &);
        int getSongs(const CString &, const CString &, const CString &,
			CExtendedListBox&);
        int getSongsInPlaylist(const CString &, CExtendedListBox &);
		CString getSongVal(const CString & key, const CString & genre,
			const CString & artist, const CString & album,
			const CString & song);
        int getTotalMp3s() { return m_totalMp3s; }
        CString getLibraryCounts();
        int init();
        int loadPlaylist(const CString & name, CString & errormsg);
        void modifyID3(Song old, Song newSong);
		void modifyPlaylists(Song old, Song newSong);
		void modifyPlaylist(const CString playlist, Song oldsong,
			Song newsong);
        void movePlaylistUp(int plc, int sel);
        void movePlaylistDown(int plc, int sel);
        Playlist _playlist;
        void RandomizePlaylist();
        void readDbLocation();
        UINT readDb();
        void savePlaylist(const CString &);
		void savePlaylist(Playlist & playlist, const CString & file);
        CString scanDirectories(const CStringList & dirs, InitDlg*,
			BOOL scanNew);
        void searchForMp3s(CStringList & songs, const CString & genre,
			const CString & artist, const CString & album,
			const CString & song);
        void searchForArtists(CStringList & songs, MList & artistList);
		void searchForAlbums(CStringList & songs, MList & albumList);
        void searchForSongs(CStringList & songs, MList & songList,
			const CString & song = "");
        void setDbLocation(const CString & loc);
		int setSongVal(const CString & key, const CString & value, 
			const CString & genrename,
			const CString & artistname, const CString & albumname,
			const CString & songname);
        void shufflePlaylist();
        UINT writeDb();
		CString writeSongToFile(Song song);
//        void exportLibrary();
		MSongLib m_SongLib;
	private:
		
		PicCache m_picCache;
        CString m_dir;
		CString m_file;
		InitDlg *_id;
        int m_totalMp3s;
        CStringList m_mp3Extensions;

        int scanDirectory(int * abortf, CStringList &, const CString &,
			BOOL scanNew);
        int garbageCollect(InitDlg * dialog = NULL);
        
};




#endif
