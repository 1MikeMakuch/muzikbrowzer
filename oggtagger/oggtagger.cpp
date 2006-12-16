
#include "StdAfx.h"
#include "vcedit.h"
#include "vorbis/codec.h"
#include "vorbis/vorbisfile.h"
#include "oggtagger.h"
#include "utf8.h"
#include "util/MyString.h"
#include "TestHarness/TestHarness.h"
#include <afxtempl.h>
#include "SortedArray.h"

//#include "MBGlobals.h"

OggTag::OggTag() {}
OggTag::OggTag(const CString & file) {
	read(file);
}

CString
OggTag::read(const CString & file) {
	m_file = file;
	OggVorbis_File * ogg = new OggVorbis_File;
	FILE * OF = fopen(m_file, "rb");
	if (OF == NULL) {
		CString error = "unable to open " + m_file + "\r\n";
		delete ogg;
		return error;
	}
	if (ov_open(OF, ogg, NULL, 0) < 0) {
		CString error = m_file;
		error += " does not appear to be an ogg file\r\n";
		fclose(OF);
		delete ogg;
		return error;
	}

	int i;
    char *decoded_value;
	CString out,error;

	vorbis_comment *vc = ov_comment(ogg, -1);
	if (vc->comments < 1) {
		error = "no comment info in ";
		error += m_file;
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
			key.MakeLower();
			oldvalue = getVal(key);
			if ((!key.CompareNoCase("description") 
					|| !key.CompareNoCase("comments"))
					&& oldvalue.GetLength() && newvalue.GetLength()) {
				newvalue = oldvalue + " " + newvalue;
			}
			if (key.GetLength() && newvalue.GetLength()) {
				m_tags.SetAt(key, newvalue);
			}
        }
    }
	if (vc->vendor) {
		m_vendor = vc->vendor;
	}
	m_time = (long) ov_time_total(ogg, -1);
	ov_clear(ogg);
	delete ogg;
	return "";
}

CString 
OggTag::getComments() {

	CString out;
	POSITION pos;
	CString key,val;
	for(pos = m_tags.GetStartPosition(); pos != NULL; ) {
		m_tags.GetNextAssoc(pos, key, val);
		out += key;
		out += "=";
		out += val;
		out += "\r\n";
	}
	return out;
}
CString
OggTag::getInfo() {

	OggVorbis_File ogg;
	FILE * OF = fopen(m_file, "rb");
	if (OF == NULL) {
		return CS("unable to open " + m_file);
	}
	if (ov_open(OF, &ogg, NULL, 0) < 0) {
		fclose(OF);
		return CS(m_file + " does not appear to be an ogg file");
	}
	vorbis_comment *vc = ov_comment(&ogg, -1);
	int i;
	CString kv,key,val;

	CString out;
	CSortedArray<CString, CString &> tagsList;
	for(i = 0 ; i < vc->comments; ++i) {
		tagsList.Add(CS(vc->user_comments[i]));
		//out += vc->user_comments[i];
		//out += "\r\n";
	}
	if (vc->vendor) {
		tagsList.Add(CS(vc->vendor));
	}

	tagsList.Add("Duration="+numToString(getTime()));

	vorbis_info * vi = ov_info(&ogg, -1);
//	out += "ogg encoding version ";
//	out += numToString(vi->version);
	tagsList.Add("ogg encoding version "+numToString(vi->version));
//	out += "\r\n";
//	out += "ogg channels ";
//	out += numToString(vi->channels);
	tagsList.Add("ogg channels "+numToString(vi->channels));
//	out += "\r\n";
//	out += "ogg sample rate ";
//	out += numToString(vi->rate);
	tagsList.Add("ogg sample rate "+numToString(vi->rate));
//	out += "\r\n";
	ov_clear(&ogg);

	tagsList.SetCompareFunction(String::CompareCase);
	tagsList.Sort();

	for (i=0; i < tagsList.GetSize(); i++) {
		CString& rkey = tagsList.ElementAt(i);
		out += rkey + "\r\n";
	}

	return out;
}

//TEST(OggTag, getComments)
//{
//	CString file = "..\\testdata\\01.ogg";
//	OggTag ogg;
//	ogg.read(file);
//	CString comments = ogg.getComments();
//	comments = ogg.getInfo();
//}

CString
OggTag::getVal(const CString & srcKey) {

	CString val;
	CString key(srcKey);
	key.MakeLower();
	if (m_tags.Lookup(key, val) != 0) {
		return val;
	} else {
		return "";
	}
}

//TEST(OggTag, getVal)
//{
//	CString file = "..\\testdata\\01.ogg";
//	OggTag ogg(file);
//	CString artist = ogg.getVal("Artist");
//}

void
OggTag::setVal(const CString & srcKey, const CString & srcVal) {
	CString key(srcKey);
	key.MakeLower();
	m_tags.SetAt(key, srcVal);
}

CString
OggTag::write() {
	CString error;
	FILE * oggfile;
	vcedit_state *state;
	vorbis_comment *vc;

	oggfile = fopen(m_file, "rb");
	if (!oggfile) {
		CString msg = "unable to open ";
		msg += m_file;
		return msg;
	}

	state = vcedit_new_state();
	if (vcedit_open(state, oggfile) < 0) {
		fclose(oggfile);
		CString msg = m_file;
		msg += " doesn't appear to be an ogg file";
		return msg;
	}

	vc = vcedit_comments(state);
	vorbis_comment_clear(vc);
	vorbis_comment_init(vc);

	POSITION pos;
	CString key,val, comm;

	for(pos = m_tags.GetStartPosition(); pos != NULL;) {
		m_tags.GetNextAssoc(pos, key, val);

		char * utf8_val;
		utf8_encode(val, &utf8_val);
		comm = key;
		comm += "=";
		comm += utf8_val;
		free(utf8_val);
		char * tmp = strdup((LPCTSTR) comm);
		vorbis_comment_add(vc, tmp);
		free(tmp);
	}

	FILE * tmpfile;

	CString tmpfilename = m_file + ".tmpXYZZY";
	if ((tmpfile = fopen(tmpfilename, "wb")) == NULL) {
		remove(tmpfilename);
		fclose(oggfile);
		CString msg = "unable to create temp ogg file";
		return msg;
	}

	vcedit_write(state, tmpfile);
	fclose(oggfile);
	fclose(tmpfile);

	remove(m_file);
	if (rename(tmpfilename, m_file)) {
		CString msg = "unable to replace ogg file with temp";
		return msg;
	}
	return "";
}
//TEST(OggTag, write)
//{
//	CString file = "..\\testdata\\01.ogg";
//	OggTag ogg;
//	ogg.read(file);
//	ogg.setVal("artist", "123");
//	ogg.write();
//
//	OggTag ogg2(file);
//	CString artist2 = ogg2.getVal("artist");
//	CHECK(artist2 == "123");
//}
