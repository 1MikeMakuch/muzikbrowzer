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
BOOL
FileUtil::BufToFile(const unsigned char * string, const UINT size, const CString & file) {
	CFile cfile;
	CFileException e;
    BOOL r = cfile.Open(file, 
        CFile::modeCreate
        |CFile::modeWrite
        |CFile::shareDenyNone,
        &e);
	if (0 == r)
		return r;
	cfile.Write(string, size);
	cfile.Flush();
	return TRUE;
}
BOOL 
FileUtil::FileToBuf(unsigned char * buf, UINT & size, const CString & file) {
	CString string;
	CFile myFile;
	CFileException fileException;

	if ( !myFile.Open( file,
        CFile::modeRead,
        &fileException ))
	{
        return FALSE;
	}
	size = myFile.GetLength();
	if (NULL == buf)
		return TRUE;

    if (size && buf) {
        myFile.Read(buf, size );
		myFile.Close();
	}
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
BOOL 
FileUtil::IsParentPath(const CString & fullPath,
					   const CString & candParent) {
	CString path(fullPath);
	if (String::endsWith(path,"\\"))
		path.TrimRight("\\");
	CString parent(candParent);
	if (String::endsWith(parent,"\\"))
		parent.TrimRight("\\");
	int dirs = String::delCount(parent,"\\");
	CString fPath;
	CString fCand;
	for(int i=1; i<=dirs; ++i) {
		fPath += String::field(path,"\\",i) + "\\";
		fCand += String::field(parent,"\\",i) + "\\";
	}
	return (fPath.CompareNoCase(fCand)==0);
}
CString
FileUtil::ParentDir(const CString & path) {

	CString parent;
	CString fpath(path);
	if (String::endsWith(fpath,"\\"))
		fpath.TrimRight("\\");
	int dirs = String::delCount(fpath,"\\");
	for(int i=1; i<dirs; ++i) {
		parent += String::field(fpath,"\\",i) + "\\";
	}
	if (String::endsWith(parent,"\\"))
		parent.TrimRight("\\");
	return parent;
}

TEST(FileUtil,IsParentPath) 
{
	CString path("c:\\mkm\\musictests\\Aerosmith");
	CString partial("c:\\mkm\\music");
	CHECK(FALSE == FileUtil::IsParentPath(path,partial));
	partial = path;
	CHECK(TRUE == FileUtil::IsParentPath(path,partial));
	partial = "c:\\";
	CHECK(TRUE == FileUtil::IsParentPath(path,partial));
}

TEST(FileUtil, ParentDir)
{
	CString path("c:\\mkm\\music");
	CHECK("c:\\mkm" == FileUtil::ParentDir(path));
	path = "c:\\mkm\\";
	CHECK("c:" == FileUtil::ParentDir(path));
}
BOOL
FileUtil::IsInSubDir(const CString & candParent, const CString & filePath) {
	CString dirname = FileUtil::dirname(filePath);
	return FileUtil::IsParentPath(dirname,candParent);
}

void
FileUtil::SortUniqDelSubDirs(CStringArray & list) {
	AutoLog al("FileUtil::SortUniqDelSubDirs");
	String::Uniq(list);
	MyHash hash;
	String::copyCStringArray2MyHash(hash,list);
	list.RemoveAll();
	POSITION pos;
	CString ppath,dir,path,val ;
	for (pos = hash.m_hash.GetStartPosition(); pos != NULL;) {
		hash.m_hash.GetNextAssoc(pos, path, val);
		ppath = path;
		BOOL eliminate = FALSE;
		while(ppath.GetLength() && !eliminate) {
			ppath = FileUtil::dirname(ppath);
			if (hash.contains(ppath))
				eliminate = TRUE;
		}
		if (!eliminate)
			list.Add(path);
	}
	String::Sort(list);
}
TEST(FileUtil,SortEliminateDupsAndSubDirs)
{
	CStringArray list;
	list.Add("c:\\mkm\\one");
	list.Add("c:\\mkm\\one");
	list.Add("c:\\mkm\\one");
	list.Add("c:\\mkm\\one\\two");
	list.Add("c:\\mkm\\one\\two\\three");

	list.Add("c:\\mkm\\two");
	list.Add("c:\\mkm\\two");
	list.Add("c:\\mkm\\two");
	list.Add("c:\\mkm\\two\\two");
	list.Add("c:\\mkm\\two\\two\\three");

	list.Add("c:\\mkm\\one");
	list.Add("c:\\mkm\\onetwo");
	list.Add("c:\\mkm\\onetwothree");
	list.Add("c:\\mkm\\onetwothree\\four\\five");
	FileUtil::SortUniqDelSubDirs(list);
	int pos;
	int ctr=0;
	for(pos = 0; pos < list.GetSize();pos++) {
		CString tmp = list.GetAt(pos);
		switch (ctr) {
		case 0:
			CHECK("c:\\mkm\\one" == tmp);break;
		case 1:
			CHECK("c:\\mkm\\onetwo" == tmp);break;
		case 2:
			CHECK("c:\\mkm\\onetwothree" == tmp);break;
		case 3:
			CHECK("c:\\mkm\\two" == tmp);break;
		}
		++ctr;
	}


}
BOOL
FileUtil::GetFileStatus(CFileStatus & status, const CString & file) {
    const char * pszFileName = (LPCTSTR) file;
	CFile myFile;
	CFileException fileException;

	if ( !myFile.Open( pszFileName,
        CFile::modeRead,
        &fileException ))
	{
		status.m_ctime = 0;
		status.m_mtime = 0;
		status.m_atime = 0;
		status.m_size = 0;
		status.m_attribute = 0;
        return FALSE;
	}
	myFile.GetStatus(status);
	return TRUE;
}