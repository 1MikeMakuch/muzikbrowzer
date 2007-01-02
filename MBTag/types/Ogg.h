
#include "StdAfx.h"
#include "MBTag.h"
#include "share/utf8.h"
#include "MyString.h"
#include "TestHarness/TestHarness.h"
#include <afxtempl.h>
#include "SortedArray.h"
#include "MyLog.h"
#include "WmaTagger.h"
#include "vcedit.h"
#include "vorbis/codec.h"
#include "vorbis/vorbisfile.h"
#include "oggtagger/oggtagger.h"

/*
From http://xiph.org/vorbis/doc/v-comment.html
TITLE			Track/Work name
VERSION			The version field may be used to differentiate multiple versions of the same track title in a single collection. (e.g. remix info)
ALBUM			The collection name to which this track belongs
TRACKNUMBER		The track number of this piece if part of a specific larger collection or album
ARTIST			The artist generally considered responsible for the work. In popular music this is usually the performing band or singer. For classical music it would be the composer. For an audio book it would be the author of the original text.
PERFORMER		The artist(s) who performed the work. In classical music this would be the conductor, orchestra, soloists. In an audio book it would be the actor who did the reading. In popular music this is typically the same as the ARTIST and is omitted.
COPYRIGHT		Copyright attribution, e.g., '2001 Nobody's Band' or '1999 Jack Moffitt'
LICENSE			License information, eg, 'All Rights Reserved', 'Any Use Permitted', a URL to a license such as a Creative Commons license ("www.creativecommons.org/blahblah/license.html") or the EFF Open Audio License ('distributed under the terms of the Open Audio License. see http://www.eff.org/IP/Open_licenses/eff_oal.html for details'), etc.
ORGANIZATION	Name of the organization producing the track (i.e. the 'record label')
DESCRIPTION		A short text description of the contents
GENRE			A short text indication of music genre
DATE			Date the track was recorded
LOCATION		Location where track was recorded
CONTACT			Contact information for the creators or distributors of the track. This could be a URL, an email address, the physical address of the producing label.
ISRC			ISRC number for the track; see the ISRC intro page for more information on ISRC numbers. 
*/

class MBOggTag: public MBTagType {
public:
	MBOggTag() {
		m_convertKeys = TRUE;
		m_Ogg2id3.setVal("GENRE","TCON");
		m_Ogg2id3.setVal("ARTIST","TPE1");
		m_Ogg2id3.setVal("ALBUM","TALB");
		m_Ogg2id3.setVal("TITLE","TIT2");
		m_Ogg2id3.setVal("TRACKNUMBER","TRCK");
		m_Ogg2id3.setVal("DATE","TYER");

		m_id32Ogg.setVal("TCON","GENRE");
		m_id32Ogg.setVal("TPE1","ARTIST");
		m_id32Ogg.setVal("TALB","ALBUM");
		m_id32Ogg.setVal("TIT2","TITLE");
		m_id32Ogg.setVal("TRCK","TRACKNUMBER");
		m_id32Ogg.setVal("TYER","DATE");
		m_id32Ogg.setVal("TLEN",""); // NULL removes it so it doesn't go
		m_id32Ogg.setVal("FILE",""); // into the actual Ogg tag
	}
	virtual ~MBOggTag() {};


	virtual BOOL read(MBTag & tags, const CString & file,
		const BOOL xvert);
	virtual BOOL write(MBTag & tags, const CString & file);
	virtual CString getComments(MBTag & tags, const CString & file);
	virtual CString getInfo(MBTag & tags, const CString & file);

private:
	CString Ogg2id3(const CString & Ogg) {
		if (m_convertKeys && m_Ogg2id3.contains(Ogg))
			return m_Ogg2id3.getVal(Ogg);
		else
			return Ogg;
	}
	CString id32Ogg(const CString & id3) {
		if (m_id32Ogg.contains(id3))
			return m_id32Ogg.getVal(id3);
		else
			return id3;
	}
private:
	MyHash m_Ogg2id3;
	MyHash m_id32Ogg;
	BOOL m_convertKeys;
};

BOOL
MBOggTag::read(MBTag & tags, const CString & file, const BOOL xvert) {
	m_convertKeys = xvert;
	
	OggVorbis_File * ogg = new OggVorbis_File;
	FILE * OF = fopen(file, "rb");
	if (OF == NULL) {
		logger.log("unable to open " + file);
		delete ogg;
		return FALSE;
	}
	if (ov_open(OF, ogg, NULL, 0) < 0) {
		CString error = file;
		logger.log(" does not appear to be an ogg file");
		fclose(OF);
		delete ogg;
		return FALSE;
	}

	int i;
    char *decoded_value;
	CString out,error;

	vorbis_comment *vc = ov_comment(ogg, -1);
	if (vc->comments < 1) {
		error = "no comment info in ";
		error += file;
		error += "\r\n";
	}
	int ectr = 0;
	for (i = 0; i < vc->comments; i++)
    {
	    if (utf8_decode(vc->user_comments[i], &decoded_value) >= 0)
        {
			CString ucomment = decoded_value;
            free(decoded_value);

			CString key = String::field(ucomment,"=",1);
			CString newvalue = String::extract(ucomment,"=","");
			CString oldvalue;
			key.MakeUpper();
			key = Ogg2id3(key);
			oldvalue = tags.getVal(key);
			if ((!key.CompareNoCase("description") 
					|| !key.CompareNoCase("comments"))
					&& oldvalue.GetLength() && newvalue.GetLength()) {
				newvalue = oldvalue + " " + newvalue;
			}
			if (key.GetLength() && newvalue.GetLength()) {
				tags.setVal(key, newvalue);
			}
        }
    }

	tags.setVal("TLEN", NTS(ov_time_total(ogg, -1)));
	ov_clear(ogg);
	fclose(OF);
	delete ogg;

	m_convertKeys = TRUE;
	return TRUE;
}

BOOL
MBOggTag::write(MBTag & tags, const CString &file) {
	CString error;
	FILE * oggfile;
	vcedit_state *state;
	vorbis_comment *vc;

	oggfile = fopen(file, "rb");
	if (!oggfile) {
		CString msg = "unable to open ";
		msg += file;
		logger.log(msg);
		return FALSE;
	}

	state = vcedit_new_state();
	if (vcedit_open(state, oggfile) < 0) {
		fclose(oggfile);
		CString msg = file;
		msg += " doesn't appear to be an ogg file";
		logger.log(msg);
		return FALSE;
	}

	vc = vcedit_comments(state);
	vorbis_comment_clear(vc);
	vorbis_comment_init(vc);

	POSITION pos;
	CString key,val, comm;

	for(pos = tags.GetSortedHead(); pos != NULL;) {
		tags.GetNextAssoc(pos, key, val);

		char * utf8_val;
		utf8_encode(val, &utf8_val);
		comm = id32Ogg(key);
		comm += "=";
		comm += utf8_val;
		free(utf8_val);
		char * tmp = strdup((LPCTSTR) comm);
		vorbis_comment_add(vc, tmp);
		free(tmp);
	}

	FILE * tmpfile;

	CString tmpfilename = file + ".tmpXYZZY";
	if ((tmpfile = fopen(tmpfilename, "wb")) == NULL) {
		remove(tmpfilename);
		fclose(oggfile);
		CString msg = "unable to create temp ogg file";
		logger.log(msg);
		return FALSE;
	}

	vcedit_write(state, tmpfile);
	fclose(oggfile);
	fclose(tmpfile);

	remove(file);
	if (rename(tmpfilename, file)) {
		CString msg = "unable to replace ogg file with temp";
		logger.log(msg);
		return FALSE;
	}
	return TRUE;
}
CString
MBOggTag::getComments(MBTag & tags, const CString & file) {
	if (tags.GetCount() == 0) {
		read(tags,file,FALSE); // FALSE = don't convert keys
	}
	CString key,val,comments;
	for(POSITION pos = tags.GetSortedHead(); pos != NULL;) {
		tags.GetNextAssoc(pos,key,val);
		key = id32Ogg(key);
		if (key.GetLength()) {
			if (key.CompareNoCase("Description") == 0
				|| key.CompareNoCase("Comments") == 0) {
				if (comments.GetLength())
					comments += " ";
				comments += val;
			}
		}
	}
	return comments;
}
CString
MBOggTag::getInfo(MBTag & tags, const CString & file) {
	if (tags.GetCount() == 0) {
		read(tags,file,FALSE); // FALSE = don't convert keys
	}
	CString key,val,comments;
	for(POSITION pos = tags.GetSortedHead(); pos != NULL;) {
		tags.GetNextAssoc(pos,key,val);
		key = id32Ogg(key);
		if (key.GetLength())
			comments += key + "=" + val + "\r\n";
	}
	return comments;
}

// Register ourselves with the "type manger"
static int bogusogg = MBTag::addType("ogg", new MBOggTag());
