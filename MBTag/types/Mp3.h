#ifndef _MBTAG_TYPES_MP3_H
#define _MBTAG_TYPES_MP3_H

#include "StdAfx.h"
#include "MBTag.h"
#include "MyString.h"
#include "TestHarness/TestHarness.h"
#include "id3.h"
#include "id3/tag.h"
#include "id3/misc_support.h"
#include "MyID3LibMiscSupport.h"
#include "MyLog.h"
#include "types/Wma.h"

class MBMp3Tag : public MBTagType {
	virtual ~MBMp3Tag() {};
	virtual BOOL read(MBTag & tags, const CString & file, const BOOL xvert=TRUE);
	virtual BOOL write(MBTag & tags, const CString & file);
	virtual CString getComments(MBTag & tags, double & rggain, const CString & file);
	virtual CString getInfo(MBTag & tags, const CString & file);
	virtual CString NativeKey2Id3Key(const CString & id3) {return id3;}
	virtual CString Id3Key2NativeKey(const CString & id3) {return id3;}
	virtual BOOL getArt(
			MBTag & tags,
			const CString & file, 
			unsigned char *& rawdata, 
			size_t & nDataSize, 
			const CString & album);
	virtual BOOL setArt(
		MBTag & tags,
		const CString & file,
		unsigned char *& rawdata, 
		size_t & nDataSize);
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
size_t id3_RemoveFrame(ID3_FrameID frameid, ID3_Tag *tag) {
	size_t num_removed = 0;
	ID3_Frame *frame = NULL;

	if (NULL == tag) {
		return num_removed;
	}

	while ((frame = tag->Find(frameid))) {
		frame = tag->RemoveFrame(frame);
		delete frame;
		num_removed++;
	}
	return num_removed;
}
BOOL 
MBMp3Tag::setArt(
		MBTag & tags,
		const CString & file,
		unsigned char *& rawdata, 
		size_t & nDataSize){
	ID3_Tag id3;
	size_t tagsize = id3.Link(file, ID3TT_ALL);

	id3_RemoveFrame(ID3FID_PICTURE, &id3);

	ID3_Frame *hFrame = new ID3_Frame(ID3FID_PICTURE);
// 	hFrame->GetField(ID3FN_DESCRIPTION)->Set(pic_description);
	hFrame->GetField(ID3FN_PICTURETYPE)->Set((uint32) 0);
	hFrame->GetField(ID3FN_MIMETYPE)->Set(tags.getVal("mimetype"));
	hFrame->GetField(ID3FN_DATA)->Set(rawdata, nDataSize);
	id3.AttachFrame(hFrame);
	flags_t updated = id3.Update(ID3TT_ID3V2);
	return TRUE;
}


static BOOL
GetId3Version(const CString & file, int & version) {
	version = 0;
	CFile myFile;
	CFileException fileException;
    int count1 = 0;
    int count3 = 0;

	if ( !myFile.Open( file,
        CFile::modeRead,
        &fileException ))
	{
        CString msg = "Unable to read ";
		msg += file;
		msg += "\r\n";
        return FALSE;
	}
	BOOL r = FALSE;
    if (myFile.GetLength() > 4) {
        char buf[5];
        myFile.Read(buf, 4);
		buf[4] = 0;
		if (strnicmp("id3",buf,3) == 0) {
			version = buf[3];
			r = TRUE;
		}
	}
	myFile.Close();

	return r;
}
BOOL
MBMp3Tag::read(MBTag & tags, const CString & file, const BOOL xvert) {
	if (!file.GetLength())
		return FALSE;

	int version;
	if (GetId3Version(file,version) && 4 == version) {
//		wmfsdk doesn't work like I thought, it's not reading the 2.4 tag,
//		just v1
//		MBWmaTag wma;
//		BOOL r = wma.read(tags,file,xvert);
//		if (r) {
			tags.VerCompDesc("ID3v2.4 tag will be converted to ID3v2.3");
//		}
//		return r;
	}

	if (!tags.ReadAllTags()) {
		MBWmaTag wma;
		CString duration = wma.GetDuration(file);
		tags.setVal("TLEN", duration);  // milliseconds
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
			if (tags.m_KeyCounter) // for HasMultiVal check, don't count id2v1s
				tags.m_KeyCounter->RemoveAll();

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
MBMp3Tag::getComments(MBTag & tags, double & rggain, const CString & file) {
	if (!file.GetLength())
		return "";

	ID3_Tag id3;
	id3.Link(file, ID3TT_ID3V2);
	tags.GettingComments(TRUE);
	ReadAllTags(&id3,&tags);
	tags.GettingComments(FALSE);
	CString rg = tags.getVal("RVA2");
	if (rg.GetLength())
		rggain = atof(rg);
	return tags.GetComments();
}
CString
MBMp3Tag::getInfo(MBTag & tags, const CString & file) {
	if (!file.GetLength())
		return "";
	
	ID3_Tag id3;
	id3.Link(file, ID3TT_ID3V1);
	tags.GettingInfo(TRUE);
	tags.AppendInfo("id3v1","");
	ReadAllTags(&id3,&tags);
	id3.Clear();

	id3.Link(file, ID3TT_ID3V2);
	tags.AppendInfo("id3v2","");
	ReadAllTags(&id3,&tags);
	tags.GettingInfo(FALSE);
	id3.Clear();
	return tags.GetInfo();
}
BOOL
MBMp3Tag::write(MBTag & tags, const CString & file) {
	if (!file.GetLength())
		return FALSE;
	ID3_Tag * id3 = new ID3_Tag;
	size_t tagsize = id3->Link(file, ID3TT_ALL);
	flags_t uflag = ID3TT_ID3V1 | ID3TT_ID3V2;
//	if (id3->HasV1Tag()) {
//		uflag |= ID3TT_ID3V1;
//	}
//	if (id3->HasV2Tag()) {
//		uflag |= ID3TT_ID3V2;
//	}
	
	if (tags.IsDeleteTag()) {
		BOOL r = id3->Strip(uflag);
		delete id3;
		return r;
	} else if (tags.IsDeleteKeys()) {
		if (tags.IsDeleteKey("TCON"))
			ID3_RemoveGenres(id3);
		if (tags.IsDeleteKey("TPE1"))
			ID3_RemoveArtists(id3);
		if (tags.IsDeleteKey("TALB"))
			ID3_RemoveAlbums(id3);
		if (tags.IsDeleteKey("TIT2"))
			ID3_RemoveTitles(id3);
		if (tags.IsDeleteKey("TRCK"))
			ID3_RemoveTracks(id3);
		if (tags.IsDeleteKey("TYER"))
			ID3_RemoveYears(id3);
		if (tags.IsDeleteKey("COMM"))
			ID3_RemoveComments(id3);
	} else {
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
			} else if ("COMM" == key) {
				ID3_RemoveComments(id3,NULL);
				ID3_AddComment(id3, val,TRUE);
//			} else {
//				delete id3;
//				logger.log("bad or unsupported id3 key specified "+key);
//				return FALSE;
			}
		}
	}
	flags_t result = id3->Update(uflag);
	delete id3;
	if (!result /*== ID3TT_NONE*/) {
		return FALSE;
		logger.log("unable to update id3 tag in " + file);
	}
	return TRUE;
}


#endif