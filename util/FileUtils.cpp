#include "StdAfx.h"
#include "FileUtils.h"
#include "MyString.h"
#include "TestHarness.h"
#include "Misc.h"
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <direct.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString
FileUtil::basename(CString & file) {
	long dc = String::delCount(file, "\\");
	return String::field(file, "\\", dc);
}

TEST(FileUtils, basename)
{
	CString x = "\\dir1\\dir2\\filename";
	CString y = FileUtil::basename(x);
	CHECK(y == CString("filename"));
	x = "filename";   y = FileUtil::basename(x);
	CHECK(y == CString ("filename"));
	x = "\\filename"; y = FileUtil::basename(x);
	CHECK(y == CString ("filename"));
	x = "";           y = FileUtil::basename(x);
	CHECK(y == CString (""));
	x = "\\";         y = FileUtil::basename(x);
	CHECK(y == CString (""));
}

CString
FileUtil::dirname(CString  file) {
	int pos = String::last(file, '\\');
	if (pos == 0) {
		return CString("\\");
	}
	if (pos == -1) {
		return CString("");
	}
	return String::substring(file,0,pos);
}
TEST(FileUtils, dirname)
{
	CString x = "\\dir1\\dir2\\filename";
	CString y = FileUtil::dirname(x);
	CHECK(y == CString("\\dir1\\dir2"));	
	
	x = "filename";   y = FileUtil::dirname(x);
	CHECK(y == CString (""));
	
	x = "\\filename"; y = FileUtil::dirname(x);
	CHECK(y == CString ("\\"));
	
	x = "";           y = FileUtil::dirname(x);
	CHECK(y == CString (""));
	
	x = "\\";         y = FileUtil::dirname(x);
	CHECK(y == CString ("\\"));
}
BOOL
FileUtil::IsReadable(CString & file) {
	CFile myFile;
	CFileException fileException;

	if ( !myFile.Open( file,
        CFile::modeRead,
        &fileException ))
	{
       return FALSE;
	} else {
		myFile.Close();
		return TRUE;
	}

}
//TEST(FileUtils, IsReadable)
//{
//	CString file = "c:\\tmp\\xyz.txt";
//	BOOL r = FileUtil::IsReadable(file);
//	CHECK(r == TRUE);
//}

BOOL
FileUtil::IsWriteable(CString & file) {
	CFile myFile;
	CFileException fileException;

	if ( !myFile.Open( file,
        CFile::modeRead | CFile::modeWrite,
        &fileException ))
	{
       return FALSE;
	} else {
		myFile.Close();
		return TRUE;
	}

}

//TEST(FileUtils, IsWriteable)
//{
//	CString file = "c:\\tmp\\xyz.txt";
//	BOOL r = FileUtil::IsWriteable(file);
//	CHECK(r == TRUE);
//}

BOOL
FileUtil::mv(CString & src, CString & dst) {

	return rename( src, dst);

}


BOOL
FileUtil::rm_dir(CString & dirname, BOOL f1, BOOL f2) {
	return _rmdir(dirname);
}

