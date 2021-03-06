#include "StdAfx.h"
#include "FExtension.h"
#include "MyString.h"
#include "TestHarness.h"
#include "Misc.h"
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>

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
CString
FExtension::filename() {
	CString filename;
    int l = mFile.GetLength();
    if (l < 1) {
        return "";
    }
    int i;
	CString tmp = mFile;
	AutoBuf buf(mFile.GetLength()+1);
	strcpy(buf.p, mFile.GetBuffer(0));
    for (i = l-1 ; i > 0 ; --i) {
        if (buf.p[i] == '.') {
			buf.p[i] = 0;
			filename = buf.p;
			return filename;
        }
    }
    return "";
}
TEST(FExtension, filename)
{
	CString s="/one/two/three.txt";
	FExtension ext(s);
	CHECK(ext.filename() == CString("/one/two/three"));
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
