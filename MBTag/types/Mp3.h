
#include "StdAfx.h"
#include "MBTag.h"
#include "MyString.h"
#include "TestHarness/TestHarness.h"

class MBMp3Tag : public MBTagType {
	virtual ~MBMp3Tag() {};
	virtual BOOL read(MBTag & tags, const CString & file, const BOOL xvert=TRUE);
	virtual BOOL write(MBTag & tags, const CString & file);
	virtual CString getComments(MBTag & tags, const CString & file){ return "";};
	virtual CString getInfo(MBTag & tags, const CString & file){ return "";};
};

BOOL
MBMp3Tag::read(MBTag & tags, const CString & file, const BOOL xvert) {
	if (!file.GetLength())
		return FALSE;
	return TRUE;
}
BOOL
MBMp3Tag::write(MBTag & tags, const CString & file) {
	if (!file.GetLength())
		return FALSE;
	return TRUE;
}
static int bogus0 = MBTag::addType("mp3", new MBMp3Tag());

