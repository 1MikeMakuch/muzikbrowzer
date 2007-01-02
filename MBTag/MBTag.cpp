
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
	AudioTypeHash(){};
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
private:
	CMapStringToPtr m_types;
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

static int bogusmpg = MBTag::addType("mpg", new MBMp3Tag());
static int bogusmp1 = MBTag::addType("mp1", new MBMp3Tag());
static int bogusmp2 = MBTag::addType("mp2", new MBMp3Tag());
static int bogusmp3 = MBTag::addType("mp3", new MBMp3Tag());
static int bogusogg = MBTag::addType("ogg", new MBOggTag());
static int bogusflac= MBTag::addType("flac",new MBFlacTag());
static int bogusWma = MBTag::addType("wma", new MBWmaTag());
///////////////////////////////////////////////////////////////


MBTag::MBTag(): m_tagobj(NULL)
{
}
MBTag::MBTag(const CString & file): m_file(file)
{
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
void
MBTag::SetType(const CString & type) {
	FExtension fext(type);
	m_tagobj = getType(fext);
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
MBTag::getComments(const CString & file) {
	if (file.GetLength())
		m_file = file;
	else if (!m_file.GetLength())
		return "";

	if (!m_tagobj) {
		FExtension fext(m_file);
		m_tagobj = getType(fext);
	}
	if (m_tagobj)
		return m_tagobj->getComments(*this, m_file);

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
	CString name = "artist name " + t.Format("%Y%m%d:%H%M%S ");

	
	mbtag.setVal("TPE1",name);
	mbtag.write();

	MBTag mbtag2;
	mbtag2.read("..\\testdata\\1.flac");
//	mbtag2.logd("test 1.flac");

	CString namecheck = mbtag2.getVal("TPE1");
	CHECK(name == namecheck);

//	MBTag mp3tag;
//	mp3tag.read("..\\testdata\\Amber.mp3");
//	mp3tag.logd("test Amber.mp3");


}
