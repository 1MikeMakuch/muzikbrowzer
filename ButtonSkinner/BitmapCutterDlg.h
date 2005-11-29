// BitmapCutterDlg.h : header file
//

#if !defined(AFX_BITMAPCUTTERDLG_H__30BEF961_CE53_4E8F_957F_EEC37231EB7E__INCLUDED_)
#define AFX_BITMAPCUTTERDLG_H__30BEF961_CE53_4E8F_957F_EEC37231EB7E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DIBSectionLite.h"
#include "ButtonSt.h"
#include "SkinSlider.h"
#include "ColorStatic.h"
#include "PictureCutter.h"

#define NUM_CRECTS 16
class ButtonStuff
{
public:
	ButtonStuff();
	~ButtonStuff();
	int * x;
	int * y;
	int * w;
	int * h;
	CRect * label;
	CRect * button;
};

/////////////////////////////////////////////////////////////////////////////
// CBitmapCutterDlg dialog
class CRectCtrl
{
public:
	CRectCtrl();
	~CRectCtrl();
	CRect *rect;
	CStatic * label;
};


class CBitmapCutterDlg : public CDialog
{
// Construction
public:
	CBitmapCutterDlg(CWnd* pParent = NULL);	// standard constructor
	CRectCtrl m_rects[NUM_CRECTS];

// Dialog Data
	//{{AFX_DATA(CBitmapCutterDlg)
	enum { IDD = IDD_BITMAPCUTTER_DIALOG };

	CEdit	m_ColorOtherFg;
	CEdit	m_ColorOtherBg;
	CEdit	m_ColorOtherBtnFgHov;
	CEdit	m_ColorOtherBtnFgOut;
	CEdit	m_ColorOtherBtnBgOut;
	CEdit	m_ColorOtherBtnBgHov;
	CButton		m_BgTypeStretched;
	CButton		m_BgTypeTiled;
	CButton		m_BgTypeFixed;
	CStatic	m_FileSettings;
	CButton	m_ChooseSettings;
	CMySliderCtrl m_TVolume;
	CMySliderCtrl m_TProgress;
	CButtonST	m_TMenuButton;
	CButtonST	m_TMusicButton;
	CButtonST	m_TPicturesButton;
	CButtonST	m_TVideoButton;
	CButtonST	m_TStopButton;
	CButtonST	m_TPlayButton;
	CButtonST	m_TPauseButton;
	CButtonST	m_TReverseButton;
	CButtonST	m_TForwardButton;
	CButtonST	m_TRandomButton;
	CButtonST	m_TShuffleButton;
	CButtonST	m_TClearButton;
	CButtonST	m_TLoadButton;
	CButtonST	m_TSaveButton;
	CButton	m_ChooseOuts;
	CButton	m_ChooseIns;
	CButton	m_ChooseHovers;
    CButton m_BlackWhite;
    CButton m_ButtonDest;
    CButton m_ButtonSave;
    CButton m_Cancel;
    CButton m_Draw;
    CButton m_DrawSave;
    CButton m_Hovers;
    CButton m_Ins;
    CButton m_Other;
    CButton m_Outs;
    CEdit   m_Red;
	CEdit   m_TransRedMain;
	CEdit   m_TransRedPanel;
	CEdit   m_Green;
	CEdit   m_TransGreenMain;
	CEdit   m_TransGreenPanel;
	CEdit   m_Blue;
    CEdit   m_TransBlueMain;
    CEdit   m_TransBluePanel;
    CEdit   m_ClearHeight;
    CEdit   m_ClearWidth;
    CEdit   m_ClearX;
    CEdit   m_ClearY;
    CEdit   m_ForwardHeight;
    CEdit   m_ForwardWidth;
    CEdit   m_ForwardX;
    CEdit   m_ForwardY;
    CEdit   m_LineWidth;
    CEdit   m_LoadHeight;
    CEdit   m_LoadWidth;
    CEdit   m_LoadX;
    CEdit   m_LoadY;
    CEdit   m_PauseHeight;
    CEdit   m_PauseWidth;
    CEdit   m_PauseX;
    CEdit   m_PauseY;
    CEdit   m_PlayHeight;
    CEdit   m_PlayWidth;
    CEdit   m_PlayX;
    CEdit   m_PlayY;
    CEdit   m_ProgressHeight;
    CEdit   m_ProgressWidth;
    CEdit   m_ProgressX;
    CEdit   m_ProgressY;
    CEdit   m_RandomHeight;
    CEdit   m_RandomWidth;
    CEdit   m_RandomX;
    CEdit   m_RandomY;
    CEdit   m_ReverseHeight;
    CEdit   m_ReverseWidth;
    CEdit   m_ReverseX;
    CEdit   m_ReverseY;
    CEdit   m_SaveHeight;
    CEdit   m_SaveWidth;
    CEdit   m_SaveX;
    CEdit   m_SaveY;
    CEdit   m_ShuffleHeight;
    CEdit   m_ShuffleWidth;
    CEdit   m_ShuffleX;
    CEdit   m_ShuffleY;
    CEdit   m_StopHeight;
    CEdit   m_StopWidth;
    CEdit   m_StopX;
    CEdit   m_StopY;
    CEdit   m_VolBounds;
    CEdit   m_VolPct;
    CEdit   m_VolumeHeight;
    CEdit   m_VolumeWidth;
    CEdit   m_VolumeX;
    CEdit   m_VolumeY;
    CEdit   m_MenuHeight;
    CEdit   m_MenuWidth;
    CEdit   m_MenuX;
    CEdit   m_MenuY;
    CEdit   m_MusicHeight;
    CEdit   m_MusicWidth;
    CEdit   m_MusicX;
    CEdit   m_MusicY;
    CEdit   m_PicturesHeight;
    CEdit   m_PicturesWidth;
    CEdit   m_PicturesX;
    CEdit   m_PicturesY;
    CEdit   m_VideoHeight;
    CEdit   m_VideoWidth;
    CEdit   m_VideoX;
    CEdit   m_VideoY;
    CStatic m_BmpSizeHovers;
    CStatic m_BmpSizeIns;
    CStatic m_BmpSizeOuts;
    CEdit m_DestDir;
    CEdit m_FileHovers;
    CEdit m_FileIns;
    CEdit m_FileOuts;
	CStatic m_LabelClear;
    CStatic m_LabelForward;
    CStatic m_LabelLoad;
    CStatic m_LabelPause;
    CStatic m_LabelPlay;
    CStatic m_LabelProgress;
    CStatic m_LabelRandom;
    CStatic m_LabelReverse;
    CStatic m_LabelSave;
    CStatic m_LabelShuffle;
    CStatic m_LabelStop;
	CStatic m_LabelMenu;
	CStatic m_LabelMusic;
	CStatic m_LabelPictures;
	CStatic m_LabelVideo;
    CStatic m_LabelVolume;
    CStatic m_PicHovers;
    CPictureCutter m_PicOuts;
    CStatic m_PicIns;
	CStatic m_PicTest;
	CStatic	m_InsLabel;
	CStatic	m_TestLabel;
	CStatic	m_OutsLabel;
	CStatic	m_HoversLabel;
	//}}AFX_DATA
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBitmapCutterDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CRect * m_AdjustingRect;
	HICON m_hIcon;
	CString m_sFileIns;
	CString m_sFileOuts;
	CString m_sFileHovers;
	CString m_sDest;
	CString m_sFileSettings;
	BOOL m_UpdateXY;
	BOOL m_Saved;
	BOOL m_Applied;
	BOOL m_ReRead;
	BOOL m_DrawLines;
	BOOL m_DrawSliderRects;
	BOOL m_DrawSliderChannel;
	enum { BGSTRETCHED=0, BGTILED=1, BGFIXED=2 } m_BgType;

	void ReadReg();
	void StoreReg();
	void SplitAndSave(CRect button, CString sIn, CString sOut, 
		CString sHover, CString &msg);
	void drawLines(CDC * dc, CRect, CRect, int InOrOut, CDIBSectionLite & cdib);
	void line(CDC *, CRect, int,int,int,int);
	void drawButton(int i, CDC *, CRect, CRect, int InOrOut);
	void calc(CDIBSectionLite & cdib, CStatic & csizelabel);
	void IntsToCEdits();
	void CEditsToInts();
	void RectToInts();
	void EnableDisable();
	void OnApplyVol() ;
	void OnApply() ;
	void drawIt(CDC * dc);
	void OnButtonFile(CEdit & clabel, CString & label,HBITMAP & hbitmap, 
		CDIBSectionLite & cdib, CStatic & cpic,CRect & picrect, 
		CRect & picdlgrect, CStatic & csizelabel);
	void CreateTest();
	void Highlight(CRect r);

	// Generated message map functions
	//{{AFX_MSG(CBitmapCutterDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonFileIns();
	afx_msg void OnButtonFileOuts();
	afx_msg void OnButtonFileHovers() ;
	afx_msg void OnButtonDest();
	afx_msg void OnCancel();
	afx_msg void OnSave();
	afx_msg void OnUpdateXY();
	afx_msg void OnUpdateRGB();
	afx_msg void OnUpdateInOut();
	afx_msg void OnDrawsave();
	afx_msg void OnDrawLines();
	afx_msg void OnReread();
	afx_msg void OnRadioBw();
	afx_msg void OnRadioOther();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnZero();
	afx_msg void OnRemoveDraw();
	afx_msg void OnPicIns();
	afx_msg void OnAdjust();
	afx_msg void OnButtonFileSettings();
	afx_msg void OnBgtypeStretched();
	afx_msg void OnBgtypeFixed();
	afx_msg void OnBgtypeTiled();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg void OnArrowKey(UINT wParam, LONG lParam);
    afx_msg void OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags );
	afx_msg BOOL PreTranslateMessage(MSG* pMsg);
	HBITMAP m_hBitmapIns;
	HBITMAP m_hBitmapOuts;
	HBITMAP m_hBitmapHovers;
	CDIBSectionLite m_cdibIns;
	CDIBSectionLite m_cdibTest;
	CDIBSectionLite m_cdibOuts;
	CDIBSectionLite m_cdibHovers;

	int m_nRed,m_nTransRedMain,m_nTransRedPanel,
		m_nGreen,m_nTransGreenMain,m_nTransGreenPanel,
		m_nBlue,m_nTransBlueMain,m_nTransBluePanel;

	int m_nLineWidth;

	CString m_sColorOtherFg,m_sColorOtherBg,m_sColorBtnFgHov,
		m_sColorBtnFgOut,m_sColorBtnBgOut,m_sColorBtnBgHov;

	CRect Menu,Music,Pictures,Video,Stop,Play,Pause,Reverse,Forward,Random,Shuffle,Clear,
		Load,Save,Volume,Progress;
	CRect MenuLabel,MusicLabel,PicturesLabel,VideoLabel,StopLabel,PlayLabel,PauseLabel,ReverseLabel,ForwardLabel,
		RandomLabel,ShuffleLabel,ClearLabel,LoadLabel,SaveLabel,
		VolumeLabel,ProgressLabel;
	CRect m_PicInsCRect,m_PicInsDlgRect;
	CRect m_PicOutsCRect,m_PicOutsDlgRect;
	CRect m_PicHoversCRect,m_PicHoversDlgRect;
	CRect m_PicTestCRect,m_PicTestDlgRect;
	CRect m_ClientRect,m_WindowRect;
	CPoint m_StartPoint,m_EndPoint;
	BOOL m_STCFlag;
	BOOL m_RGB;
	BOOL m_Dragging;
	BOOL m_NeedButtonAssignment;
	BOOL m_ButtonDrawn;
	BOOL Init;
	BOOL m_Adjusting;

	int	m_nMenuX;
	int	m_nMenuY;
	int	m_nMenuWidth;
	int	m_nMenuHeight;

	int	m_nMusicX;
	int	m_nMusicY;
	int	m_nMusicWidth;
	int	m_nMusicHeight;

	int	m_nPicturesX;
	int	m_nPicturesY;
	int	m_nPicturesWidth;
	int	m_nPicturesHeight;

	int	m_nVideoX;
	int	m_nVideoY;
	int	m_nVideoWidth;
	int	m_nVideoHeight;

	int	m_nStopX;
	int	m_nStopY;
	int	m_nStopWidth;
	int	m_nStopHeight;
	int	m_nPlayX;
	int	m_nPlayY;
	int	m_nPlayWidth;
	int	m_nPlayHeight;
	int	m_nPauseX;
	int	m_nPauseY;
	int	m_nPauseWidth;
	int	m_nPauseHeight;
	int	m_nReverseX;
	int	m_nReverseY;
	int	m_nReverseWidth;
	int	m_nReverseHeight;
	int	m_nForwardX;
	int	m_nForwardY;
	int	m_nForwardWidth;
	int	m_nForwardHeight;
	int	m_nRandomX;
	int	m_nRandomY;
	int	m_nRandomWidth;
	int	m_nRandomHeight;
	int	m_nShuffleX;
	int	m_nShuffleY;
	int	m_nShuffleWidth;
	int	m_nShuffleHeight;
	int	m_nClearX;
	int	m_nClearY;
	int	m_nClearWidth;
	int	m_nClearHeight;
	int	m_nLoadX;
	int	m_nLoadY;
	int	m_nLoadWidth;
	int	m_nLoadHeight;
	int	m_nSaveX;
	int	m_nSaveY;
	int	m_nSaveWidth;
	int	m_nSaveHeight;
	int	m_nVolumeX;
	int	m_nVolumeY;
	int	m_nVolumeWidth;
	int	m_nVolumeHeight;
	int	m_nProgressX;
	int	m_nProgressY;
	int	m_nProgressWidth;
	int	m_nProgressHeight;
	int m_nBorderHorz;
	int m_nBorderVert;
	int m_nBorderPanel;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BITMAPCUTTERDLG_H__30BEF961_CE53_4E8F_957F_EEC37231EB7E__INCLUDED_)
