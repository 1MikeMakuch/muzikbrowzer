
#include "StdAfx.h"
#include "MBTag.h"
#include "MyString.h"
#include "TestHarness/TestHarness.h"
#include "id3/tag.h"
#include "id3/misc_support.h"
#include "WmaTagger.h"
#include "MyID3LibMiscSupport.h"
#include "MyLog.h"

class MBMp3Tag : public MBTagType {
	virtual ~MBMp3Tag() {};
	virtual BOOL read(MBTag & tags, const CString & file, const BOOL xvert=TRUE);
	virtual BOOL write(MBTag & tags, const CString & file);
	virtual CString getComments(MBTag & tags, const CString & file);
	virtual CString getInfo(MBTag & tags, const CString & file);
};

BOOL
MBMp3Tag::read(MBTag & tags, const CString & file, const BOOL xvert) {
	if (!file.GetLength())
		return FALSE;

	ID3_Tag * id3 = new ID3_Tag;
	size_t tagsize = id3->Link(file, ID3TT_ALL);

	WmaTag wma;
	wma.read(file,TRUE);

	tags.setVal("TCON", id3_GetGenre(id3));
	tags.setVal("TPE1", id3_GetArtist(id3));
	tags.setVal("TALB", id3_GetAlbum(id3));
	tags.setVal("TIT2", id3_GetTitle(id3));
	tags.setVal("TYER", id3_GetYear(id3));
	tags.setVal("TRCK", id3_GetTrack(id3));
	tags.setVal("TLEN", wma.getVal("Duration"));

	delete id3;

	return TRUE;
}
CString
MBMp3Tag::getComments(MBTag & tags, const CString & file) {
	if (!file.GetLength())
		return "";
	WmaTag wma;
	wma.read(file,TRUE);
	CString comment = wma.getVal("Description");
	CString tmp = wma.getVal("WM/Lyrics");
	if (tmp.GetLength()) {
		comment += " Lyrics: "+tmp;
	}
	tmp = wma.getVal("WM/Composer");
	if (tmp.GetLength()) {
		comment += " Composer(s): "+tmp;
	}
// Author no good, it's usually just Artist
//		tmp = wma.getVal("Author");
//		if (tmp.GetLength()) {
//			comment += " Author(s): "+tmp;
//		}
	return comment;
}
CString
MBMp3Tag::getInfo(MBTag & tags, const CString & file) {
	if (!file.GetLength())
		return "";
	ID3_Tag id3;
	id3.Link(file, ID3TT_ALL);
	CString msg = ::displayTag(&id3, FALSE, file);
	msg += "\r\n";
	msg += file;
	return msg;
}
BOOL
MBMp3Tag::write(MBTag & tags, const CString & file) {
	if (!file.GetLength())
		return FALSE;
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
	for( pos = tags.GetSortedHead(); pos != NULL; ) {
		tags.GetNextAssoc(pos, key, val);
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
	flags_t result = id3->Update(uflag);
	delete id3;
	if (result == ID3TT_NONE) {
		return FALSE;
		logger.log("unable to update id3 tag in " + file);
	}
	return TRUE;
}
static int bogusmpg = MBTag::addType("mpg", new MBMp3Tag());
static int bogusmp1 = MBTag::addType("mp1", new MBMp3Tag());
static int bogusmp2 = MBTag::addType("mp2", new MBMp3Tag());
static int bogusmp3 = MBTag::addType("mp3", new MBMp3Tag());

