#ifndef __OGGTAGGER_H
#define __OGGTAGGER_H

#include "StdAfx.h"

//#include "vcedit.h"
//#include "vorbis/codec.h"
//#include "vorbis/vorbisfile.h"

typedef CMapStringToString TagHash;

class OggTag {

	public:
		OggTag();
		OggTag(const CString & file);
		~OggTag() {}

		CString read(const CString & file);

		CString getComments();
		CString getInfo();
//		CString setComments(const CString & comments);

		CString getVendor() { return m_vendor; }
//		CString setVendor(const CString & vendor);

		CString getVal(const CString & key);
		void setVal(const CString & key, const CString & val);
		long getTime() { return m_time; }

		CString write();
	private:
		CString m_file;
		TagHash m_tags;
		long m_time;
		CString m_vendor;
};

#endif