#ifndef __PLAYER_CALLBACKS_H__
#define __PLAYER_CALLBACKS_H__

#include "stdafx.h"
//#include "MBConfig.h"

//typedef void (*PlayerRedrawCallback)();
//typedef CString (*PlayerMBDirCallback)();
class MBConfig;
class MusicLib;
class CPlayerDlg;
class CExtendedListBox;
class LoadPlaylistDlg ;

class PlayerCallbacks {
public:
	PlayerCallbacks() {
		redraw = NULL;
		mbdir = NULL;
		initDb = NULL;
		getLibraryCounts = NULL;
		statusset = NULL;
		statustempset = NULL;
		UpdateWindow = NULL;
		setDbLocation = NULL;
		Need2Erase = NULL;
		mbconfig = NULL;
		mbconfig = NULL;
		musiclib = NULL;
		playerdlg = NULL;
		dlg = NULL;
		lpdlg = NULL;
		OnPaintCallback = NULL;
		SetSelected = NULL;
	}

	void (*redraw)();
	CString (*mbdir)();
	void (*initDb)();
	CString (*getLibraryCounts)();
	void (*statusset)(CString);
	void (*statustempset)(CString);
	void (*UpdateWindow)();
	void (*setDbLocation)(CString);
	void (*Need2Erase)(BOOL);
	MBConfig * (*mbconfig)();
	MusicLib * (*musiclib)();
	CPlayerDlg * (*playerdlg)();
	CDialog * (*dlg)();
	LoadPlaylistDlg * (*lpdlg)();
	BOOL (*OnPaintCallback)();
	void (*SetSelected)(const CString text, DWORD data);

};

#endif
