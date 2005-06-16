
#ifndef _FileUtils_HH_
#define _FileUtils_HH_

class FileUtil {
public:
	static BOOL	IsReadable(CString & file);
	static BOOL IsWriteable(CString & file);
	static CString	basename(CString & file);
	static CString dirname(CString  file);
	static BOOL	mv(CString &, CString &);
	static BOOL	rm_dir(CString &, BOOL, BOOL);
	static BOOL mkdirp(CString &);
	static BOOL BmpSave(HBITMAP, CString);
	static void BmpSave(HDC, CString, int width, int height, int x=0, int y=0);
	static void BmpLog(HBITMAP, CString);
};

#endif
