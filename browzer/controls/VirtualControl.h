#ifndef _VIRTUALCONTROL_H_
#define _VIRTUALCONTROL_H_

#include "stdafx.h"
#include "MenuDialog.h"
#include "ExtendedListBox.h"
//#include "MBMessageBox.h"

class MBMessageBoxImpl;
class CPlayerDlg;
class LoadPlaylistDlg;
class CMenuDialog;

class VirtualControl {
	friend class VirtualDialog;
	public:
		VirtualControl();

        void operator = (CComboBox*);
        void operator = (CExtendedListBox*);
        void operator = (CButton*);
		void operator = (CMenuDialog*);
		void operator = (MBMessageBoxImpl *);
		void operator = (CEdit*);
        void up(int);
        void down(int);
//        void select();
        void * ptr() { return m_Ptr; }
        BOOL isList() { return m_List != 0; }
	private:
		CComboBox * m_Combo;
        CExtendedListBox * m_List;
        CButton * m_Button;
        CMenuDialog * m_MenuDialog;
		MBMessageBoxImpl * m_MBox;
		CEdit * m_Edit;
        void * m_Ptr;

        void zero();

};


class VirtualDialog {
	public:
		VirtualDialog();

        void operator = (CPlayerDlg*);
        void operator = (LoadPlaylistDlg*);
        void operator = (MBMessageBoxImpl*);
        void operator = (CMenuDialog*);
        void OnControlSelChange();
        void PrevDlgCtrl();
        void NextDlgCtrl();
        void OnControlClick();
        void OnMenuButton();
        void OnDelete();
        void OnPageUp();
        void OnPageDown();
        void OnExit();
        void * ptr() { return m_Ptr; }
        VirtualControl & control() ;
//        BOOL isList() { return mList != 0; }
	private:

        CPlayerDlg * m_PlayerDlg;
        LoadPlaylistDlg * m_LoadPlaylistDlg;
        MBMessageBoxImpl * m_MBoxDlg;
		CMenuDialog * m_MenuDlg;
        void * m_Ptr;

        void zero();

};
#endif