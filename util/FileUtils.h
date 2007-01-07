
#ifndef _FileUtils_HH_
#define _FileUtils_HH_

class FileUtil {
public:
	static BOOL	IsReadable(const CString & file);
	static BOOL IsWriteable(const CString & file);
	static BOOL DirIsWriteable(const CString & dir);
	static CString	basename(const CString & file);
	static CString dirname(const CString   &file);
	static BOOL	mv(const CString &, const CString &);
	static BOOL	rm_dir(const CString &, BOOL, BOOL);
	static BOOL rm(const CString & file);
	static BOOL mkdirp(const CString &);
	static BOOL BmpSave(HBITMAP, CString);
	static void BmpLog(HDC, CString, int width, int height, int x=0, int y=0);
	static void BmpLog(HBITMAP, CString);
	static CString FileToString(const CString file);
	static BOOL StringToFile(const CString & string, const CString & file);
	static BOOL BufToFile(const unsigned char * buf, const UINT size, const CString & file);
	static BOOL FileToBuf(unsigned char * buf, UINT & size, const CString & file);
	static BOOL IsParentPath(const CString & fullPath,
		const CString & candPath);
	static CString ParentDir(const CString & path);
	static BOOL IsInSubDir(const CString & dir, const CString & candFilePath);
	static void SortUniqDelSubDirs(CStringArray & list);
};

#endif
