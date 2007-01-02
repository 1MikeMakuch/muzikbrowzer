#ifndef _MBTAG_TYPES_MP3_H
#define _MBTAG_TYPES_MP3_H

#include "StdAfx.h"
#include "MBTag.h"
#include "MyString.h"
#include "TestHarness/TestHarness.h"
#include "id3/tag.h"
#include "id3/misc_support.h"
#include "MyID3LibMiscSupport.h"
#include "MyLog.h"
#include "types/Wma.h"

class MBMp3Tag : public MBTagType {
	virtual ~MBMp3Tag() {};
	virtual BOOL read(MBTag & tags, const CString & file, const BOOL xvert=TRUE);
	virtual BOOL write(MBTag & tags, const CString & file);
	virtual CString getComments(MBTag & tags, const CString & file);
	virtual CString getInfo(MBTag & tags, const CString & file);
	virtual CString NativeKey2Id3Key(const CString & id3) {return id3;}
	virtual CString Id3Key2NativeKey(const CString & id3) {return id3;}
	virtual BOOL getArt(
			MBTag & tags,
			const CString & file, 
			unsigned char *& rawdata, 
			size_t & nDataSize, 
			const CString & album);
};
BOOL
MBMp3Tag::getArt(
		MBTag & tags,
			const CString & file, 
			unsigned char *& rawdata, 
			size_t & nDataSize, 
			const CString & album)
{
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
			if (nDataSize) {
				delete iter;
				return TRUE;
			}


		}
	}
	delete iter;
	return MBTagType::getArt(tags,file,rawdata,nDataSize,album);
}
BOOL
MBMp3Tag::read(MBTag & tags, const CString & file, const BOOL xvert) {
	if (!file.GetLength())
		return FALSE;

	if (!tags.ReadAllTags()) {
		MBTag wmatags;
		MBWmaTag wma;
		wma.SetReadDurationOnly();
		wma.read(wmatags,file,TRUE);
		tags.setVal("TLEN", wmatags.getVal("TLEN"));  // milliseconds
	}

	{
		ID3_Tag * id3 = new ID3_Tag;
		size_t tagsize = id3->Link(file, /*ID3TT_ALL */ID3TT_ID3V1);

		if (tags.ReadAllTags()) {
			ReadAllTags(id3,&tags);
		} else {
			tags.setVal("TCON", id3_GetGenre(id3));
			tags.setVal("TPE1", id3_GetArtist(id3));
			tags.setVal("TALB", id3_GetAlbum(id3));
			tags.setVal("TIT2", id3_GetTitle(id3));
			tags.setVal("TYER", id3_GetYear(id3));
			tags.setVal("TRCK", id3_GetTrack(id3));
			
		}
		delete id3;
	}
	{ // v2 overwrites v1
		ID3_Tag * id3 = new ID3_Tag;
		size_t tagsize = id3->Link(file, ID3TT_ID3V2);

		if (tags.ReadAllTags()) {
			ReadAllTags(id3,&tags);
		} else {
			CString tmp;
			tmp = id3_GetGenre(id3);
			if (tmp.GetLength()) tags.setVal("TCON",tmp);
			tmp = id3_GetArtist(id3);
			if (tmp.GetLength()) tags.setVal("TPE1",tmp);
			tmp = id3_GetAlbum(id3);
			if (tmp.GetLength()) tags.setVal("TALB",tmp);
			tmp = id3_GetTitle(id3);
			if (tmp.GetLength()) tags.setVal("TIT2",tmp);
			tmp = id3_GetYear(id3);
			if (tmp.GetLength()) tags.setVal("TYER",tmp);
			tmp = id3_GetTrack(id3);
			if (tmp.GetLength()) tags.setVal("TRCK",tmp);
		}
		delete id3;
	}

	return TRUE;
}
CString
MBMp3Tag::getComments(MBTag & tags, const CString & file) {
	if (!file.GetLength())
		return "";

	MBTag wmatags;
	MBWmaTag wma;
	wma.read(wmatags,file,FALSE);

	CString comment = wmatags.getVal("Description");
	CString tmp = wmatags.getVal("WM/Lyrics");
	if (tmp.GetLength()) {
		comment += " Lyrics: "+tmp;
	}
	tmp = wmatags.getVal("WM/Composer");
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


#endif