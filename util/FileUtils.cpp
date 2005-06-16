#include "StdAfx.h"
#include "FileUtils.h"
#include "MyString.h"
#include "TestHarness.h"
#include "Misc.h"
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <direct.h>
#include "DIBSectionLite.h"

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
// this doesn't appear to work on dirs
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

BOOL
FileUtil::mkdirp(CString & dirname)
{
	CString tmp;
	tmp = String::replace(dirname, "\\", "/");  // normalize
	long numparts = String::delCount(tmp, "\\");
	long i;
	CString dir(String::field(tmp, "\\", 1));
	dir += "\\";
	for(i = 2 ; i <= numparts ; i++) {
		dir += String::field(tmp, "\\", i);
		dir += "\\";
		if (_mkdir(dir.GetBuffer(0)) != 0) {
			//return FALSE;
		}
	}
	return TRUE;
}
TEST(FileUtil, mkdirp)
{
	CString dir("c:\\mkm\\tmp\\xyz\\xyz2");
	BOOL r = FileUtil::mkdirp(dir);
	CHECK(r == TRUE);

}

BOOL
FileUtil::BmpSave(HBITMAP hbitmap, CString file) {

	CDIBSectionLite dib;
	dib.SetBitmap((HBITMAP) hbitmap);
	return dib.Save(file);

	return TRUE;
}

void
FileUtil::BmpSave(HDC hdcsrc, CString name, int width, int height, int x, int y) {

	static int counter = 0;
	HBITMAP hbmp = ::CreateCompatibleBitmap(hdcsrc,width,height);
	HDC hdc = ::CreateCompatibleDC(NULL);
	HBITMAP oldbmp = (HBITMAP)::SelectObject(hdc, (HBITMAP)hbmp);
	::BitBlt(hdc,0,0,width,height,hdcsrc,x,y,SRCCOPY);
	::SelectObject(hdc, oldbmp);
	CString bmpname = "c:\\mkm\\bmps\\" + name + numToString(counter++) + ".bmp";
	FileUtil::BmpSave(hbmp,bmpname);
	::DeleteObject((HBITMAP)hbmp);
	::DeleteObject((HDC)hdc);
	::DeleteObject((HBITMAP)oldbmp);
}
void
FileUtil::BmpLog(HBITMAP hbitmap, CString name) {
#ifdef _DEBUG
	CDIBSectionLite dib;
	dib.SetBitmap((HBITMAP) hbitmap);
	static int counter =0;
	CString file = "c:\\mkm\\bmps\\" + name + numToString(counter++) + ".bmp";
	dib.Save(file);
#endif
}
