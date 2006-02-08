#ifndef __WMATAGGER_H__
#define __WMATAGGER_H__

#include "StdAfx.h"

typedef CMapStringToString TagHash;

class WmaTag {

	public:
		WmaTag();
		WmaTag(const CString & file);
		~WmaTag() {}

		CString read(const CString & file, const BOOL durOnly=FALSE);

		CString getComments();
		CString getInfo();

		CString getVendor() { return m_vendor; }

		CString getVal(const CString & key);
		void setVal(const CString & key, const CString & val);
		long getTime() { return m_time; }
		CString readTimex(const CString &);

		CString write();
		BOOL exists(const CString &key);
	private:
		CString m_file;
		TagHash m_tags;
		long m_time;
		CString m_vendor;
};

#endif
