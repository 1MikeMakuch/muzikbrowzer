#include "StdAfx.h"
#include "FExtension.h"
#include "MyString.h"
#include "TestHarness.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

FExtension::FExtension(CString fname): 	mFile(fname) {
}

CString
FExtension::ext() {
    CString extension;
    int l = mFile.GetLength();
    if (l < 1) {
        return extension;
    }
    int i;
    const char * buf = (LPCTSTR)mFile;
    for (i = l-1 ; i > 0 ; --i) {
        if (buf[i] == '.') {
            extension = &(buf[i+1]);
            return extension;
        }
    }
    return extension;
}

BOOL
FExtension::operator == (CString extension) {
    if (ext().CompareNoCase(extension) == 0) {
        return TRUE;
    } else {
        return FALSE;
    }
}

BOOL
FExtension::operator == (const char * extension) {
	CString Extension(extension);
    return (*this == Extension);
}

CString
PathUtil::basename(const CString & path) {

    int spos = String::last(path, '\\');
    if (spos == -1)
    return path;
    
    size_t len = path.GetLength();
    size_t ln_pos = spos + 1; // last node beginning position;
    return path.Right(path.GetLength() - ln_pos);
}
TEST(PathUtil, basename)
{
	CString tmp = "\\abc\\def\\ghi";
	CString tmp2 = PathUtil::basename(tmp);
	CHECK(tmp2 == "ghi");
}

CString
PathUtil::dir(const CString & path) {
	
    int spos = String::last(path, '\\');
    if (spos == -1)
		return ".";
    else if (spos == 0)
		return path.Left(1);
	
    return path.Left(spos);
}
TEST(PathUtil, dir)
{
	CString tmp = "\\abc\\dev\\ghi";
	CString tmp2 = PathUtil::dir(tmp);
	CHECK(tmp2 = "\\abc\\dev");
	tmp = "\\abc";
	tmp2 = PathUtil::dir(tmp);
	CHECK(tmp2 == "\\");
}