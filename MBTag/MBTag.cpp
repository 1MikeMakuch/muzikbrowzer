
#include "StdAfx.h"
#include "MBTag.h"
#include "MyString.h"
#include "TestHarness/TestHarness.h"
#include <afxtempl.h>
#include "SortedArray.h"
#include "FExtension.h"
#include "FileUtils.h"

#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>

// Just a little hash to manage the types and free memory at shutdown.
class AudioTypeHash {
public:
	AudioTypeHash(){
		m_MBKeys.setVal("TCON","1");
		m_MBKeys.setVal("TPE1","1");
		m_MBKeys.setVal("TALB","1");
		m_MBKeys.setVal("TIT2","1");
		m_MBKeys.setVal("TYER","1");
		m_MBKeys.setVal("TRCK","1");
	};
	~AudioTypeHash() {
		CString k;
		MBTagType * p;
		for (POSITION pos = m_types.GetStartPosition(); pos != NULL;) {
			m_types.GetNextAssoc(pos,k,(void *&)p);
			delete (MBTagType*)p;
		}
	}
	void SetAt(const CString & key, MBTagType * type) {
		CString low(key);
		low.MakeLower();
		m_types.SetAt(low,type);
	}
	BOOL Lookup(const CString & key, MBTagType *& type) {
		CString low(key);
		low.MakeLower();
		return m_types.Lookup(low,(void*&)type);
	}

	CMapStringToPtr m_types;
	MyHash m_MBKeys;
};

///////////////////////////////////////////////////////////////
static AudioTypeHash AudioFileTypes;                         //
AudioTypeHash * MBTag::m_types = &AudioFileTypes;            //
///////////////////////////////////////////////////////////////
// Found I have to include these here (or somewhere) in order//
// to get the static addTypes to register at program startup.//
// Hack! They also have to be included after the m_types     //
// instantiation above. So this is a good spot.              //
// even though it breaks the idea of adding types w/out      //
// modifying this base code.                                 //
//                                                           //
#include "types/Mp3.h"                                       //
#include "types/Ogg.h"                                       //
#include "types/Wma.h"                                       //
#include "types/Flac.h"                                      //

//static int bogusmpg = MBTag::addType("mpg", new MBMp3Tag());
//static int bogusmp1 = MBTag::addType("mp1", new MBMp3Tag());
//static int bogusmp2 = MBTag::addType("mp2", new MBMp3Tag());
static int bogusmp3 = MBTag::addType("mp3", new MBMp3Tag());
static int bogusogg = MBTag::addType("ogg", new MBOggTag());
static int bogusflac= MBTag::addType("flac",new MBFlacTag());
static int bogusWma = MBTag::addType("wma", new MBWmaTag());
///////////////////////////////////////////////////////////////


void
MBTag::init() {
	m_tagobj = NULL; 
	m_ReadAllTags = FALSE;
	m_KeyCounter = NULL;
	m_GetInfo = FALSE;
	m_GetComments = FALSE;
	m_DeleteTag = FALSE;
}
MBTag::MBTag() {
	init();
}
MBTag::MBTag(const CString & file):
	m_file(file)
{
	init();	
}

// static member called by the individual <types>.h at startup
int
MBTag::addType(const CString fext, MBTagType * type) {
	MBTag::m_types->SetAt(fext,type);
	return 1;
}

MBTagType *
MBTag::getType(FExtension & fext) {
	MBTagType * type=NULL;
	if (MBTag::m_types->Lookup(fext.ext(),type)) {
		return type;
	}
	return NULL;
}
CString
MBTag::getType() {
	FExtension fext(m_file);
	return fext.ext();
}
BOOL
MBTag::IsAnMBKey(const CString & key) {
	return MBTag::m_types->m_MBKeys.contains(key);
}
BOOL
MBTag::GetExtensions(CStringList & list) {
	CString key;
	void * p;
	for(POSITION pos = MBTag::m_types->m_types.GetStartPosition(); pos !=NULL;) {
		MBTag::m_types->m_types.GetNextAssoc(pos,key,p);
		list.AddTail(key);
	}
	return TRUE;
}
void
MBTag::SetType(const CString & type) {
	FExtension fext(type);
	m_tagobj = getType(fext);
}
void
MBTag::setVal(const CString & key, const CString & val) {
	MyHash::setVal(String::upcase(key),val);
}
CString
MBTag::getVal(const CString & key) {
	return MyHash::getVal(String::upcase(key));
}
void
MBTag::GetDeleteKeys(CStringList & list) {
	CString key,val;
	for(POSITION pos = m_DeleteKeys.GetSortedHead(); pos != NULL;) {
		m_DeleteKeys.GetNextAssoc(pos,key,val);
		list.AddTail(key);
	}
}
// xvert tells whether or not to convert to "standard" keys
BOOL
MBTag::read(const CString & file, const BOOL xvert) {
	if (file.GetLength())
		m_file = file;
	else if (!m_file.GetLength())
		return FALSE;

	if (!m_tagobj) {
		FExtension fext(m_file);
		m_tagobj = getType(fext);
	}
	if (m_tagobj)
		return m_tagobj->read(*this, m_file,xvert);
	else
		return FALSE;
	
	return TRUE;
}
BOOL 
MBTag::HasMultiVals(const CString & file) {
	if (file.GetLength())
		m_file = file;
	else if (!m_file.GetLength())
		return FALSE;
	MyHash KeyCounter;
	m_KeyCounter = &KeyCounter;
	SetReadAllTags(TRUE);
	read(m_file,TRUE);
	SetReadAllTags(FALSE);
	m_KeyCounter = NULL;
	CString key,val;
	for(POSITION pos = MBTag::m_types->m_MBKeys.GetSortedHead(); pos != NULL; ) {
		MBTag::m_types->m_MBKeys.GetNextAssoc(pos,key,val);
		int c = atoi(KeyCounter.getVal(key));
		if (c > 1)
			return TRUE;
	}
	return FALSE;
}
BOOL
MBTag::write() {
	if (!m_file.GetLength())
		return FALSE;

	if (!m_tagobj) {
		FExtension fext(m_file);
		m_tagobj = getType(fext);
	}
	if (m_tagobj)
		return m_tagobj->write(*this, m_file);
	else
		return FALSE;
	
	return TRUE;
}
CString
MBTag::getComments(double & rggain, const CString & file) {
	if (file.GetLength())
		m_file = file;
	else if (!m_file.GetLength())
		return "";

	if (!m_tagobj) {
		FExtension fext(m_file);
		m_tagobj = getType(fext);
	}
	if (m_tagobj)
		return m_tagobj->getComments(*this, rggain, m_file);

	return "";
}
CString
MBTag::getInfo(const CString & file) {
	if (file.GetLength())
		m_file = file;
	else if (!m_file.GetLength())
		return "";

	if (!m_tagobj) {
		FExtension fext(m_file);
		m_tagobj = getType(fext);
	}
	if (m_tagobj)
		return m_tagobj->getInfo(*this, m_file);

	return "";
}

BOOL
MBTag::getArt(
			const CString & file, 
			unsigned char *& rawdata, 
			size_t & nDataSize, 
			const CString & album)
{
	if (file.GetLength())
		m_file = file;
	else if (!m_file.GetLength())
		return FALSE;

	if (!m_tagobj) {
		FExtension fext(m_file);
		m_tagobj = getType(fext);
	}
	if (m_tagobj)
		return m_tagobj->getArt(
			*this,
			m_file,
			rawdata,
			nDataSize,
			album);

	return FALSE;
}
CString
MBTag::Id3Key2NativeKey(const CString & key) {
	if (!m_tagobj) {
		FExtension fext(m_file);
		m_tagobj = getType(fext);
	}
	CString ikey(key);
	if (m_tagobj)
		ikey = m_tagobj->Id3Key2NativeKey(key);
	return ikey;
}
void
MBTag::setValId3Key(const CString & key, const CString & val) {
	if (!m_tagobj) {
		FExtension fext(m_file);
		m_tagobj = getType(fext);
	}
	CString ikey(key);
	if (m_tagobj)
		ikey = m_tagobj->Id3Key2NativeKey(key);
	MyHash::setVal(ikey,val);
	return;
}
CString
MBTag::getValId3Key(const CString & key) {
	if (!m_tagobj) {
		FExtension fext(m_file);
		m_tagobj = getType(fext);
	}
	CString ikey(key);
	if (m_tagobj)
		ikey = m_tagobj->Id3Key2NativeKey(key);
	return MyHash::getVal(ikey);
}
void
MBTag::setValNativeKey(const CString & key, const CString & val) {
	if (!m_tagobj) {
		FExtension fext(m_file);
		m_tagobj = getType(fext);
	}
	CString ikey(key);
	if (m_tagobj)
		ikey = m_tagobj->NativeKey2Id3Key(key);
	MyHash::setVal(ikey,val);
	return;
}
CString
MBTag::getValNativeKey(const CString & key) {
	if (!m_tagobj) {
		FExtension fext(m_file);
		m_tagobj = getType(fext);
	}
	CString ikey(key);
	if (m_tagobj)
		ikey = m_tagobj->NativeKey2Id3Key(key);
	return MyHash::getVal(ikey);
}


// Look for art on disk.
// Derived members call this one if art not found in 
// the tag. 
BOOL
MBTagType::getArt(
			MBTag & tags,
			const CString & file, 
			unsigned char *& rawdata, 
			size_t & nDataSize, 
			const CString & album)
{

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
		return TRUE;
	}
	// not in cover.jpg so look for albumname.jpg in dir
	folderjpg = FileUtil::dirname(file);
	folderjpg += "\\" + album + ".jpg";
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
		return TRUE;
	}
	return FALSE;
}



TEST(MBTag,test)
{
	MBTag mbtag;
	mbtag.read("..\\testdata\\1.flac");
//	mbtag.logd("test 1.flac");
	CTime t = CTime::GetCurrentTime();
	CString OrigName = mbtag.getVal("TPE1");
	CString name = "artist name " + t.Format("%Y%m%d:%H%M%S ");

	
	mbtag.setVal("TPE1",name);
	mbtag.write();

	MBTag mbtag2;
	mbtag2.read("..\\testdata\\1.flac");
//	mbtag2.logd("test 1.flac");

	CString namecheck = mbtag2.getVal("TPE1");
	CHECK(name == namecheck);
	mbtag2.setVal("TPE1", OrigName);
	mbtag2.write();

//	MBTag mp3tag;
//	mp3tag.read("..\\testdata\\Amber.mp3");
//	mp3tag.logd("test Amber.mp3");


}
