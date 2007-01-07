#ifndef _MBTAG_TYPES_FLAC_H_
#define _MBTAG_TYPES_FLAC_H_

#include "StdAfx.h"
#include "MBTag.h"
#include "share/utf8.h"
#include "MyString.h"
#include "TestHarness/TestHarness.h"
#include <afxtempl.h>
#include "SortedArray.h"
#include "MyLog.h"
#include "ocidl.h"
#include "olectl.h"

#include "FLAC++/metadata.h"

extern "C" {
#include "operations.h"
#include "options.h"
}

using namespace FLAC::Metadata;
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

class MBFlacTag: public MBTagType {
public:
	MBFlacTag() {
		m_convertKeys = TRUE;
		m_flac2id3.setVal("GENRE","TCON");
		m_flac2id3.setVal("ARTIST","TPE1");
		m_flac2id3.setVal("ALBUM","TALB");
		m_flac2id3.setVal("TITLE","TIT2");
		m_flac2id3.setVal("TRACKNUMBER","TRCK");
		m_flac2id3.setVal("DATE","TYER");

		m_id32flac.setVal("TCON","GENRE");
		m_id32flac.setVal("TPE1","ARTIST");
		m_id32flac.setVal("TALB","ALBUM");
		m_id32flac.setVal("TIT2","TITLE");
		m_id32flac.setVal("TRCK","TRACKNUMBER");
		m_id32flac.setVal("TYER","DATE");
		m_id32flac.setVal("COMM","COMMENTS"); // for mbtag only
		m_id32flac.setVal("TLEN",""); // NULL removes it so it doesn't go
		m_id32flac.setVal("FILE",""); // into the actual flac tag
	}
	virtual ~MBFlacTag() {};


	virtual BOOL read(MBTag & tags, const CString & file,
		const BOOL xvert);
	virtual BOOL write(MBTag & tags, const CString & file);
	virtual CString getComments(MBTag & tags, double & rggain, const CString & file);
	virtual CString getInfo(MBTag & tags, const CString & file);
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
	virtual CString NativeKey2Id3Key(const CString & flac) {
		if (m_convertKeys && m_flac2id3.contains(flac))
			return m_flac2id3.getVal(flac);
		else
			return flac;
	}
	virtual CString Id3Key2NativeKey(const CString & id3) {
		if (m_id32flac.contains(id3))
			return m_id32flac.getVal(id3);
		else
			return id3;
	}
private:
	MyHash m_flac2id3;
	MyHash m_id32flac;
	BOOL m_convertKeys;
};
BOOL
MBFlacTag::getArt(
			MBTag & tags,
			const CString & file, 
			unsigned char *& rawdata, 
			size_t & nDataSize, 
			const CString & album) {
	Picture picture;
	FLAC__StreamMetadata_Picture_Type type = (FLAC__StreamMetadata_Picture_Type)(-1);
	char * mime_type=0;
	FLAC__byte * desc=0;
	unsigned int width,height,depth,colors;
	width=height=1000;
	depth=32;
	colors=-1;
	bool r = FLAC::Metadata::get_picture(file,picture,type,mime_type,desc,
		width,height,depth,colors);
	if (r) {
		nDataSize = picture.get_data_length();
		rawdata = new BYTE [ nDataSize ];
		memcpy(rawdata,picture.get_data(),nDataSize);
		
		return TRUE;
	}
	return MBTagType::getArt(tags,file,rawdata,nDataSize,album);
}
BOOL 
MBFlacTag::setArt(
		MBTag & tags,
		const CString & file,
		unsigned char *& rawdata, 
		size_t & nDataSize) {

	IPicture*		iPicture;
	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, nDataSize);
	void* pData = GlobalLock(hGlobal);
	memcpy(pData, rawdata, nDataSize);
	GlobalUnlock(hGlobal);
	IStream* pStream = NULL;
	BOOL loaded = FALSE;
	if (CreateStreamOnHGlobal(hGlobal, TRUE, &pStream) == S_OK) {
		HRESULT hr ;
		if ((hr = OleLoadPicture(pStream, nDataSize, FALSE, IID_IPicture, 
			(LPVOID *)&iPicture)) == S_OK) {
			loaded = TRUE;
		}
		pStream->Release();
	}
	if (!loaded) 
		return FALSE;
	long width,height;
	iPicture->get_Width(&width);
	iPicture->get_Height(&height);
	//iPicture->
	iPicture->Release();

	width = MulDiv(width,	96/*pDC->GetDeviceCaps(LOGPIXELSX)*/, 2540);
	height = MulDiv(height,	96/*pDC->GetDeviceCaps(LOGPIXELSY)*/, 2540);	

	Picture * picture = new Picture;
	picture->set_data((FLAC__byte*)rawdata,nDataSize);

	picture->set_depth(32);
	picture->set_width(width);
	picture->set_height(height);
	picture->set_mime_type(tags.getVal("mimetype"));

	FLAC::Metadata::Chain chain;
	if (!chain.is_valid())
		return FALSE;
	if (!chain.read(file))
		return FALSE;
	FLAC::Metadata::Iterator iter;
	iter.init(chain);
	if (!iter.is_valid())
		return FALSE;
	bool more = TRUE;
	BOOL added = FALSE;
	while(more) {
		FLAC__MetadataType type = iter.get_block_type();
		if (FLAC__METADATA_TYPE_PICTURE == type) {
			iter.set_block(picture);
			added = TRUE;
			break;
		}
		more = iter.next();
	}
	if (!added)
		iter.insert_block_after(picture);

	return chain.write();
}
BOOL
MBFlacTag::read(MBTag & tags, const CString & file, const BOOL xvert) {
	m_convertKeys = xvert;
	
	VorbisComment vc;
	bool r = FLAC::Metadata::get_tags(file, vc);
	if (!r || NULL == &vc)
		return FALSE;

	int i;
    char *decoded_value;
	CString out,error;

//  Don't do this, might have an image and/or Duration for getInfo.
//	if (vc.get_num_comments() < 1) {
//		return FALSE;
//	}
	tags.setVal("FILE", file);
	int ectr = 0;
	for (i = 0; i < vc.get_num_comments(); i++)
    {
	    if (vc.get_comment(i).get_field_name_length() && 
			utf8_decode(vc.get_comment(i).get_field_name(), &decoded_value) 
			>= 0)
        {
			CString okey = decoded_value;
            free(decoded_value);
			if (vc.get_comment(i).get_field_value_length() &&
				utf8_decode(vc.get_comment(i).get_field_value(), 
					&decoded_value) >= 0)
			{
				CString val = decoded_value;
				free(decoded_value);
				CString key(okey);
				key.MakeUpper();
				key = NativeKey2Id3Key(key);
				if (key.GetLength() && val.GetLength()) {
					tags.setVal(key, val);
					if (tags.m_KeyCounter && tags.IsAnMBKey(key)) {
						CString tmp = tags.m_KeyCounter->getVal(key);
						int c = atoi(tmp);
						c++;
						tags.m_KeyCounter->setVal(key,NTS(c));
					}
					if (tags.GettingInfo())
						tags.AppendInfo(okey,val);
					if (tags.GettingComments() 
						&& (0 == key.CompareNoCase("DESCRIPTION")
						|| 0 == key.CompareNoCase("COMMENTS")))
						tags.AppendComments(val);
				}
			}
        }
    }
	StreamInfo si;
	r = FLAC::Metadata::get_streaminfo(file, si);
	if (r) {
		UINT sr = si.get_sample_rate();
		if (sr > 0) {
			UINT64 ts = si.get_total_samples();
			if (ts > 0) {
				UINT D = ts / sr; // seconds
				D *= 1000; // milliseconds
				tags.setVal("TLEN",NTS(D));
			}
		}
	}
	if (tags.GettingInfo()) {
		Picture picture;
		FLAC__StreamMetadata_Picture_Type type = (FLAC__StreamMetadata_Picture_Type)(-1);
		char * mime_type=0;
		FLAC__byte * desc=0;
		unsigned int width,height,depth,colors;
		width=height=1000;
		depth=32;
		colors=-1;
		bool r = FLAC::Metadata::get_picture(file,picture,type,mime_type,desc,
			width,height,depth,colors);
		if (r) {
			mime_type = (char *)picture.get_mime_type();
			width = picture.get_width();
			height = picture.get_height();
			desc = (unsigned char*)picture.get_description();
			CString tmp;
			tmp = CS(mime_type) + " " + 
				NTS(width)+"x"+NTS(height)+" "+desc;
			tags.AppendInfo("Image",tmp);
		}
	}

	m_convertKeys = TRUE;
	return TRUE;
}

static BOOL ReadOldTagFlac(const CString & file, CStringList & list) {
	VorbisComment vc;
	bool r = FLAC::Metadata::get_tags(file, vc);
	if (!r || NULL == &vc)
		return FALSE;

	int i;
    char *decoded_value;
	CString out,error;
	
	if (vc.get_num_comments() < 1) {
		return FALSE;
	}
	CString val;
	int ectr = 0;
	for (i = 0; i < vc.get_num_comments(); i++)
    {
	    if (vc.get_comment(i).get_field_name_length() && 
			utf8_decode(vc.get_comment(i).get_field_name(), &decoded_value) 
			>= 0)
        {
			val = decoded_value;
            free(decoded_value);
			if (vc.get_comment(i).get_field_value_length() &&
				utf8_decode(vc.get_comment(i).get_field_value(), 
					&decoded_value) >= 0)
			{
				val += CS("=") + decoded_value;
				free(decoded_value);
			}
			if (val.GetLength())
				list.AddTail(val);
        }
    }
	return TRUE;
}
static void freemakeargv(int n, char * argv[]);
static int makeargv(CStringList & slist, char *** argvp);

BOOL
MBFlacTag::write(MBTag & tags, const CString &file) {
	// This is a total hack! Using metaflac,
	// creating a cl and calling it! It was too easy
	// to do this way.
	CStringList oldvals;
	if (!ReadOldTagFlac(file,oldvals))
		return FALSE;

	CString key,val,oldkey,oldval,tmp;
	CStringList args;
	CStringArray newcomments;
	args.AddTail("bogus");
	args.AddTail("--remove-all-tags");
	POSITION pos ;

	// 1st put back everything we're not changing
	if (!tags.IsDeleteTag()) {
		for(pos = oldvals.GetHeadPosition(); pos != NULL; oldvals.GetNext(pos)) {
			oldkey = String::field(oldvals.GetAt(pos),"=",1);
			oldval = String::extract(oldvals.GetAt(pos),"=","");
			key = NativeKey2Id3Key(String::upcase(oldkey));
			if (!tags.contains(key)
				&& !tags.IsDeleteKey(key)
				) {
				tmp = "--set-tag=" + oldkey + "=" + oldval;
				newcomments.Add(tmp);
				logger.ods(tmp);
			}
		}
	
	// Now make the edit
	
		for(pos = tags.GetSortedHead(); pos != NULL;) {
			tags.GetNextAssoc(pos, key, val);
			key = Id3Key2NativeKey(key);
			if (key.GetLength() && !tags.IsDeleteKey(key)) {
				tmp = "--set-tag=" + key + "=" + val;
				newcomments.Add(tmp);
				logger.ods(tmp);
			}
		}
	}
	String::SortNoCase(newcomments);
	for(int idx = 0 ; idx < newcomments.GetSize(); ++idx) {
		args.AddTail(newcomments.GetAt(idx));
	}

	args.AddTail(file);
	UINT n = args.GetCount();

	char ** argv ;
	int argc = makeargv(args,&argv);
	if (0 == argc) return FALSE;

	int i=0;
	while(argv[i]) {
		logger.ods("argv["+NTS(i)+"] "+CS(argv[i]));
		++i;
	}

// This is basically metaflac here...

	CommandLineOptions options;
	int ret = 0;

	init_options(&options);

	// need to init these getopt args each time
	share__optarg = 0;
	share__optind = 0;
	share__opterr = 0;

	if(parse_options(n, argv, &options))
		ret = !do_operations(&options);

	free_options(&options);

// ... to here

	freemakeargv(n, argv);

	return TRUE;
}
CString
MBFlacTag::getComments(MBTag & tags, double & rggain, const CString & file) {
	tags.GettingComments(TRUE);
	read(tags,file,FALSE); // FALSE = don't convert keys
	tags.GettingComments(FALSE);
	CString rg = tags.getVal("REPLAYGAIN_TRACK_GAIN");
	if (rg.GetLength())
		rggain = atof(rg);
	return tags.GetComments();
}
CString
MBFlacTag::getInfo(MBTag & tags, const CString & file) {

	tags.GettingInfo(TRUE);
	read(tags,file,FALSE); // FALSE = don't convert keys
	tags.GettingInfo(FALSE);
	return tags.GetInfo();
}
int makeargv(CStringList & slist, char *** argvp) {
	int n = slist.GetCount();
	if (0 == n) return 0;
	*argvp = (char**)malloc((n+1)*sizeof(char*));
	int i=0;
	POSITION pos;
	for(pos = slist.GetHeadPosition(); pos != NULL; slist.GetNext(pos)) {
		*((*argvp) + i) = strdup(slist.GetAt(pos));
		++i;
	}
	if (i != n) return 0;
	*((*argvp) + n) = NULL;
	return n;
}

void freemakeargv(int argc, char * argv[]) {
	if (argv == NULL)
		return;
	for(int i = 0 ; i < argc; ++i) {
		free(argv[i]);
		argv[i] = NULL;
	}
	if (*argv != NULL)
		free(*argv);
	free(argv);
}

#endif