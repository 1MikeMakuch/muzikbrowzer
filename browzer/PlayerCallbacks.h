#ifndef __PLAYER_CALLBACKS_H__
#define __PLAYER_CALLBACKS_H__

#include "stdafx.h"
#include "InitDlg.h"

//typedef void (*PlayerRedrawCallback)();
//typedef CString (*PlayerMBDirCallback)();

class PlayerCallbacks {
public:
	void (*redraw)();
	CString (*mbdir)();
	void (*initDb)();
	CString (*getLibraryCounts)();
	void (*statusset)(CString);
	void (*statustempset)(CString);
	void (*UpdateWindow)();
	void (*setDbLocation)(CString);
	CString (*scanDirectories)(const CStringList & directories,
						  InitDlg * initDlg, BOOL scanNew, BOOL bAdd);
};

#endif
