
#include "StdAfx.h"
#include "VirtualControl.h"
#include "PlayerDlg.h"
#include "LoadPlaylistDlg.h"
#include "MBMessageBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

VirtualControl::VirtualControl()
{
    zero();
}

void
VirtualControl::operator = (CComboBox * p) {
    zero();
    m_Combo = p;
    m_Ptr = (void*) p;
}

void
VirtualControl::operator = (CExtendedListBox * p) {
    zero();
    m_List = p;
    m_Ptr = (void*) p;
}
void
VirtualControl::operator = (CButton * p) {
    zero();
    m_Button = p;
    m_Ptr = (void*) p;
}
void
VirtualControl::operator = (CMenuDialog * p) {
    zero();
    m_MenuDialog = p;
    m_Ptr = (void*) p;
}
void
VirtualControl::operator = (MBMessageBoxImpl * p) {
    zero();
    m_MBox = p;
    m_Ptr = (void*) p;
}
void
VirtualControl::operator = (CEdit * p) {
    zero();
    m_Edit = p;
    m_Ptr = (void*) p;
}

void
VirtualControl::up(int n) {
    if (m_List) {
        int cur = m_List->GetCurSel();
        int newc = cur - n;
        if (newc < 0) newc = 0;
        m_List->SetCurSel(newc);
    } else if (m_Combo) {
        int cur = m_Combo->GetCurSel();
        int newc = cur - n;
        if (newc < 0) cur = 0;
        m_Combo->SetCurSel(newc);
    } else if (m_MenuDialog) {
        m_MenuDialog->PrevDlgCtrl();
        m_MenuDialog->prevbutton();
	} else if (m_Edit) {
        m_Edit->LineScroll(-5,0);
    }
}

void
VirtualControl::down(int n) {
    if (m_List) {
        int cur = m_List->GetCurSel();
        int max = m_List->GetCount();
        max--;
        int newcur = cur + n;
        if (newcur > max) newcur = max;
        m_List->SetCurSel(newcur);
    } else if (m_Combo) {
        int cur = m_Combo->GetCurSel();
        int max = m_Combo->GetCount();
        max--;
        int newcur = cur + n;
        if (newcur > max) newcur = max;
        m_Combo->SetCurSel(newcur);
    } else if (m_MenuDialog) {
        m_MenuDialog->NextDlgCtrl();
        m_MenuDialog->nextbutton();
	} else if (m_Edit) {
        m_Edit->LineScroll(5,0);
    }
}

void
VirtualControl::zero() {
    m_Combo = 0;
    m_List = 0;
    m_Button = 0;
    m_Ptr = 0;
    m_MenuDialog = 0;
	m_MBox = 0;
	m_Edit = 0;
}
////////////////////////////////////////////////////////////////////////////
VirtualDialog::VirtualDialog()
{
    zero();
}
void
VirtualDialog::operator = (MBMessageBoxImpl * p) {
    zero();
    m_MBoxDlg = p;
    m_Ptr = (void*) p;
}

void
VirtualDialog::operator = (CPlayerDlg * p) {
    zero();
    m_PlayerDlg = p;
    m_Ptr = (void*) p;
}

void
VirtualDialog::operator = (LoadPlaylistDlg * p) {
    zero();
    m_LoadPlaylistDlg = p;
    m_Ptr = (void*) p;
}
void
VirtualDialog::operator = (CMenuDialog *p) {
	zero();
	m_MenuDlg = p;
	m_Ptr = (void*)p;
}

void
VirtualDialog::zero() {
    m_PlayerDlg = 0;
    m_LoadPlaylistDlg = 0;
    m_MBoxDlg = 0;
    m_Ptr = 0;
	m_MenuDlg = 0;

}
void
VirtualDialog::OnControlSelChange() {
    if (m_PlayerDlg) {
        m_PlayerDlg->OnControlSelChange();
    } else if (m_LoadPlaylistDlg) {
        m_LoadPlaylistDlg->OnControlSelChange();
    }
}
void
VirtualDialog::PrevDlgCtrl() {
    if (m_PlayerDlg) {
        m_PlayerDlg->PrevDlgCtrl();
    } else if (m_LoadPlaylistDlg) {
        m_LoadPlaylistDlg->PrevDlgCtrl();
    } else if (m_MenuDlg) {
		m_MenuDlg->PrevDlgCtrl();
	} else if (m_MBoxDlg) {
		m_MBoxDlg->m_Control->m_Edit->LineScroll(0,-10);
	}
}
void
VirtualDialog::NextDlgCtrl() {
    if (m_PlayerDlg) {
        m_PlayerDlg->NextDlgCtrl();
    } else if (m_LoadPlaylistDlg) {
        m_LoadPlaylistDlg->NextDlgCtrl();
    } else if (m_MenuDlg) {
		m_MenuDlg->NextDlgCtrl();
	} else if (m_MBoxDlg) {
		m_MBoxDlg->m_Control->m_Edit->LineScroll(0,10);
	}
}
void
VirtualDialog::OnControlClick() {
    if (m_PlayerDlg) {
        m_PlayerDlg->OnControlClick();
    } else if (m_LoadPlaylistDlg) {
        m_LoadPlaylistDlg->OnControlClick();
    } else if (m_MBoxDlg) {
        m_MBoxDlg->OnOK();
    } else if (m_MenuDlg) {
		m_MenuDlg->OnControlClick();
	}
}
void
VirtualDialog::OnMenuButton() {
    if (m_PlayerDlg) {
        m_PlayerDlg->OnMenuButton();
    } else if (m_LoadPlaylistDlg) {
        m_LoadPlaylistDlg->OnMenuButton();
    }
}
void
VirtualDialog::OnDelete() {
    if (m_PlayerDlg) {
        m_PlayerDlg->OnDelete();
    } else if (m_LoadPlaylistDlg) {
        m_LoadPlaylistDlg->OnDelete();
    }
}
void
VirtualDialog::OnPageUp() {
    if (m_PlayerDlg) {
        m_PlayerDlg->OnPageUp();
    } else if (m_LoadPlaylistDlg) {
        m_LoadPlaylistDlg->OnPageUp();
    }
}
void
VirtualDialog::OnPageDown() {
    if (m_PlayerDlg) {
        m_PlayerDlg->OnPageDown();
    } else if (m_LoadPlaylistDlg) {
        m_LoadPlaylistDlg->OnPageDown();
    }
}

VirtualControl &
VirtualDialog::control() {
    if (m_PlayerDlg) {
        return *m_PlayerDlg->m_Control;
    } else if (m_LoadPlaylistDlg) {
        return *m_LoadPlaylistDlg->m_Control;
    } else if (m_MBoxDlg) {
        return *m_MBoxDlg->m_Control;
    } else if (m_MenuDlg) {
		*m_MenuDlg->m_Control = m_MenuDlg;		
		return *m_MenuDlg->m_Control;
	}
	abort();
	return *m_PlayerDlg->m_Control;
}
void
VirtualDialog::OnExit() {
    if (m_PlayerDlg) {
        m_PlayerDlg->OnExit();
    } else if (m_LoadPlaylistDlg) {
        m_LoadPlaylistDlg->OnExit();
    } else if (m_MBoxDlg) {
		m_MBoxDlg->OnExit();
	} else if (m_MenuDlg) {
		m_MenuDlg->OnExit();
	}
}
