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
#include "MyLog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString
FileUtil::basename(const CString & file) {
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
FileUtil::dirname(const CString   &file) {
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
FileUtil::IsReadable(const CString & file) {

	DWORD result = GetFileAttributes(file);
	if (0xFFFFFFFF == result) {
		return FALSE;
	} else {
		return TRUE;
	}

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
FileUtil::IsWriteable(const CString & file) {
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
BOOL
FileUtil::DirIsWriteable(const CString & file) {
	DWORD result = GetFileAttributes(file);
	if (FILE_ATTRIBUTE_DIRECTORY == result) {
		return TRUE;
	} else {
		return FALSE;
	}

}

//TEST(FileUtils, IsWriteable)
//{
//	CString file = "c:\\tmp\\xyz.txt";
//	BOOL r = FileUtil::IsWriteable(file);
//	CHECK(r == TRUE);
//}

BOOL
FileUtil::mv(const CString & src, const CString & dst) {

	_unlink(dst);
	return rename( src, dst);

}


BOOL
FileUtil::rm_dir(const CString & dirname, BOOL f1, BOOL f2) {
	return _rmdir(dirname);
}
BOOL
FileUtil::rm(const CString & file) {
	return _unlink(file);
}

BOOL
FileUtil::mkdirp(const CString & dirname)
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
FileUtil::BmpLog(HDC hdcsrc, CString name, int width, int height, int x, int y) {
	HBITMAP hbmp = (HBITMAP)::CreateCompatibleBitmap(hdcsrc,width,height);
	HDC hdc = (HDC)::CreateCompatibleDC(NULL);
	HBITMAP oldbmp = (HBITMAP)::SelectObject(hdc, (HBITMAP)hbmp);
	::BitBlt(hdc,0,0,width,height,hdcsrc,x,y,SRCCOPY);
	::SelectObject(hdc, oldbmp);
	FileUtil::BmpLog(hbmp,name);
	::DeleteObject((HBITMAP)hbmp);
	::DeleteDC((HDC)hdc);
//	::DeleteObject((HBITMAP)oldbmp);
}
void
FileUtil::BmpLog(HBITMAP hbitmap, CString name) {
#ifdef _DEBUG
	static CMapStringToOb map;
	CObjectInt * oint;
//	oint.m_int = 0;
	if (map.Lookup(name, (CObject*&)oint)) {
		oint->m_int++;
	} else {
		oint = new CObjectInt(0);
		map.SetAt(name,oint);
	}

	CDIBSectionLite dib;
	dib.SetBitmap((HBITMAP) hbitmap);

	CString file = "c:\\mkm\\bmps\\" + name + numToString(oint->m_int) + ".bmp";
	dib.Save(file);
#endif
}

#ifdef dontdoit
TEST(FileUtil, BmpLog)
{
	CDIBSectionLite dib;
	dib.Load("..\\testdata\\red.bmp");
	FileUtil::BmpLog((HBITMAP) dib, "key1");

	HDC hdcS = ::CreateCompatibleDC(NULL);
	HDC hdcD = ::CreateCompatibleDC(NULL);
	
	HBITMAP hOldSBmp = (HBITMAP)::SelectObject(hdcS, (HBITMAP) dib);
	HBITMAP hNewBmp = ::CreateCompatibleBitmap(
		hdcS,dib.GetWidth(),dib.GetHeight());

	HBITMAP hOldDBmp = (HBITMAP)::SelectObject(hdcD, hNewBmp);

	::BitBlt(hdcD,0,0,dib.GetWidth(),dib.GetHeight(),hdcS,0,0,SRCCOPY);

	::SelectObject(hdcD, hOldDBmp);
	::SelectObject(hdcS, hOldSBmp);

	FileUtil::BmpLog(hNewBmp,"key2");
	::DeleteObject(hNewBmp);
	::DeleteDC(hdcS);
	::DeleteDC(hdcD);
}
#endif

CString
FileUtil::FileToString(const CString file) {
	CString string;
	CFile myFile;
	CFileException fileException;
    int count1 = 0;
    int count3 = 0;

	if ( !myFile.Open( file,
        CFile::modeRead,
        &fileException ))
	{
        CString msg = "Unable to read ";
		msg += file;
		msg += "\r\n";
        return msg;
	}
    if (myFile.GetLength()) {
        AutoBuf buf(myFile.GetLength()+1);
        myFile.Read(buf.p, myFile.GetLength());
		buf.p[myFile.GetLength()] = 0;
		string = buf.p;
		myFile.Close();
	}
	return string;
}
BOOL
FileUtil::StringToFile(const CString & string, const CString & file) {
	CFile cfile;
	CFileException e;
    BOOL r = cfile.Open(file, 
        CFile::modeCreate
        |CFile::modeWrite
        |CFile::shareDenyNone,
        &e);
	if (0 == r)
		return r;
	cfile.Write(string,string.GetLength());
	cfile.Flush();
	return TRUE;
}
TEST(FileUtilFileToString,StringToFile)
{
	CString s = "123";
	CString f = "..\\testdata\\FileUtilTest";
	BOOL r = FileUtil::StringToFile(s,f);
	CHECK(TRUE == r);
	CString s2 = FileUtil::FileToString(f);
	CHECK(s2 == s);
}
