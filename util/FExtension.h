#ifndef _FEXTENSION_H_
#define _FEXTENSION_H_

#include "stdafx.h"

class FExtension {
    public:
        FExtension(CString);
        CString ext();
        BOOL operator == (CString);
		BOOL operator == (const char *);
    private:
        CString mFile;
};

class PathUtil {
public:
	static CString basename(const CString & file);
	static CString dir(const CString & file);
};

#endif