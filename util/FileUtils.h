
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
};

#endif
