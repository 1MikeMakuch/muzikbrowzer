
// BitmapCutterDlg.cpp : implementation file
//

#include "stdafx.h"

#include "BitmapCutter.h"
#include "BitmapCutterDlg.h"

#define LOGOWNER
#include "MyLog.h"

#include "MyString.h"
#include "FolderDialog.h"
#include "SkinDefs.h"
#include "FileUtils.h"
#include "Registry.h"
#include "FExtension.h"
#include "ButtonST.h"
#include "Misc.h"
#include "muzikbrowzerVersion.h"

CRectCtrl::CRectCtrl() {}
CRectCtrl::~CRectCtrl() {}



/////////////////////////////////////////////////////////////////////////////
// CBitmapCutterDlg dialog

CBitmapCutterDlg::CBitmapCutterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBitmapCutterDlg::IDD, pParent), m_PicOuts(this)
{
	//{{AFX_DATA_INIT(CBitmapCutterDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
//	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hBitmapIns = NULL;
	m_hBitmapOuts = NULL;
	m_hBitmapHovers = NULL;

	m_Saved = FALSE;
	m_NeedButtonAssignment = FALSE;
	m_Adjusting = FALSE;
	m_AdjustingRect = NULL;

	m_nTransRedMain = 254;
	m_nTransGreenMain = 0;
	m_nTransBlueMain = 0;

	m_nTransRedPanel = 253;
	m_nTransGreenPanel = 0;
	m_nTransBluePanel = 0;

	m_BgType = BGSTRETCHED;

}
void CBitmapCutterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBitmapCutterDlg)
	DDX_Control(pDX, IDC_CLR_BTN_FG_OUT, m_ColorOtherBtnFgOut);
	DDX_Control(pDX, IDC_CLR_OTHER_FG, m_ColorOtherFg);
	DDX_Control(pDX, IDC_CLR_OTHER_BG, m_ColorOtherBg);
	DDX_Control(pDX, IDC_CLR_BTN_FG_HOV, m_ColorOtherBtnFgHov);
	DDX_Control(pDX, IDC_CLR_BTN_BG_OUT, m_ColorOtherBtnBgOut);
	DDX_Control(pDX, IDC_CLR_BTN_BG_HOV, m_ColorOtherBtnBgHov);
	DDX_Control(pDX, IDC_BGTYPE_STRETCHED, m_BgTypeStretched);
	DDX_Control(pDX, IDC_BGTYPE_TILED, m_BgTypeTiled);
	DDX_Control(pDX, IDC_BGTYPE_FIXED, m_BgTypeFixed);
	DDX_Control(pDX, IDC_FILE_SETTINGS, m_FileSettings);
	DDX_Control(pDX, IDC_BUTTON_FILE_SETTINGS, m_ChooseSettings);
	DDX_Control(pDX, IDC_TVOLUME, m_TVolume);
	DDX_Control(pDX, IDC_TPROGRESS, m_TProgress);
	DDX_Control(pDX, IDC_BUTTON11, m_TMenuButton);
	DDX_Control(pDX, IDC_BUTTON12, m_TMusicButton);
	DDX_Control(pDX, IDC_BUTTON13, m_TPicturesButton);
	DDX_Control(pDX, IDC_BUTTON14, m_TVideoButton);
	DDX_Control(pDX, IDC_BUTTON1, m_TStopButton);
	DDX_Control(pDX, IDC_BUTTON2, m_TPlayButton);
	DDX_Control(pDX, IDC_BUTTON3, m_TPauseButton);
	DDX_Control(pDX, IDC_BUTTON4, m_TReverseButton);
	DDX_Control(pDX, IDC_BUTTON5, m_TForwardButton);
	DDX_Control(pDX, IDC_BUTTON6, m_TRandomButton);
	DDX_Control(pDX, IDC_BUTTON7, m_TShuffleButton);
	DDX_Control(pDX, IDC_BUTTON8, m_TClearButton);
	DDX_Control(pDX, IDC_BUTTON9, m_TLoadButton);
	DDX_Control(pDX, IDC_BUTTON10, m_TSaveButton);
	DDX_Control(pDX, IDC_BUTTON_FILE_OUTS, m_ChooseOuts);
	DDX_Control(pDX, IDC_BUTTON_FILE_INS, m_ChooseIns);
	DDX_Control(pDX, IDC_BUTTON_FILE_HOVERS, m_ChooseHovers);
	DDX_Control(pDX, IDC_BLACK_WHITE, m_BlackWhite);
	DDX_Control(pDX, IDC_BUTTON_DEST, m_ButtonDest);
	DDX_Control(pDX, IDC_SAVE, m_ButtonSave);
	DDX_Control(pDX, IDC_CANCEL, m_Cancel);
	DDX_Control(pDX, IDC_DRAW, m_Draw);
	DDX_Control(pDX, IDC_DRAWSAVE, m_DrawSave);
	DDX_Control(pDX, IDC_HOVER, m_Hovers);
	DDX_Control(pDX, IDC_INS, m_Ins);
	DDX_Control(pDX, IDC_OTHER, m_Other);
	DDX_Control(pDX, IDC_OUTS, m_Outs);
	DDX_Control(pDX, IDC_RED, m_Red);
	DDX_Control(pDX, IDC_RED2, m_TransRedMain);
	DDX_Control(pDX, IDC_RED3, m_TransRedPanel);
	DDX_Control(pDX, IDC_GREEN, m_Green);
	DDX_Control(pDX, IDC_GREEN2, m_TransGreenMain);
	DDX_Control(pDX, IDC_GREEN3, m_TransGreenPanel);
	DDX_Control(pDX, IDC_BLUE, m_Blue);
	DDX_Control(pDX, IDC_BLUE2, m_TransBlueMain);
	DDX_Control(pDX, IDC_BLUE3, m_TransBluePanel);
	DDX_Control(pDX, IDC_CLEAR_HEIGHT, m_ClearHeight);
	DDX_Control(pDX, IDC_CLEAR_WIDTH, m_ClearWidth);
	DDX_Control(pDX, IDC_CLEAR_X, m_ClearX);
	DDX_Control(pDX, IDC_CLEAR_Y, m_ClearY);
	DDX_Control(pDX, IDC_FORWARD_HEIGHT, m_ForwardHeight);
	DDX_Control(pDX, IDC_FORWARD_WIDTH, m_ForwardWidth);
	DDX_Control(pDX, IDC_FORWARD_X, m_ForwardX);
	DDX_Control(pDX, IDC_FORWARD_Y, m_ForwardY);
	DDX_Control(pDX, IDC_LINEWIDTH, m_LineWidth);
	DDX_Control(pDX, IDC_LOAD_HEIGHT, m_LoadHeight);
	DDX_Control(pDX, IDC_LOAD_WIDTH, m_LoadWidth);
	DDX_Control(pDX, IDC_LOAD_X, m_LoadX);
	DDX_Control(pDX, IDC_LOAD_Y, m_LoadY);
	DDX_Control(pDX, IDC_PAUSE_HEIGHT, m_PauseHeight);
	DDX_Control(pDX, IDC_PAUSE_WIDTH, m_PauseWidth);
	DDX_Control(pDX, IDC_PAUSE_X, m_PauseX);
	DDX_Control(pDX, IDC_PAUSE_Y, m_PauseY);
	DDX_Control(pDX, IDC_PLAY_HEIGHT, m_PlayHeight);
	DDX_Control(pDX, IDC_PLAY_WIDTH, m_PlayWidth);
	DDX_Control(pDX, IDC_PLAY_X, m_PlayX);
	DDX_Control(pDX, IDC_PLAY_Y, m_PlayY);
	DDX_Control(pDX, IDC_PROGRESS_HEIGHT, m_ProgressHeight);
	DDX_Control(pDX, IDC_PROGRESS_WIDTH, m_ProgressWidth);
	DDX_Control(pDX, IDC_PROGRESS_X, m_ProgressX);
	DDX_Control(pDX, IDC_PROGRESS_Y, m_ProgressY);
	DDX_Control(pDX, IDC_RANDOM_HEIGHT, m_RandomHeight);
	DDX_Control(pDX, IDC_RANDOM_WIDTH, m_RandomWidth);
	DDX_Control(pDX, IDC_RANDOM_X, m_RandomX);
	DDX_Control(pDX, IDC_RANDOM_Y, m_RandomY);
	DDX_Control(pDX, IDC_REVERSE_HEIGHT, m_ReverseHeight);
	DDX_Control(pDX, IDC_REVERSE_WIDTH, m_ReverseWidth);
	DDX_Control(pDX, IDC_REVERSE_X, m_ReverseX);
	DDX_Control(pDX, IDC_REVERSE_Y, m_ReverseY);
	DDX_Control(pDX, IDC_SAVE_HEIGHT, m_SaveHeight);
	DDX_Control(pDX, IDC_SAVE_WIDTH, m_SaveWidth);
	DDX_Control(pDX, IDC_SAVE_X, m_SaveX);
	DDX_Control(pDX, IDC_SAVE_Y, m_SaveY);
	DDX_Control(pDX, IDC_SHUFFLE_HEIGHT, m_ShuffleHeight);
	DDX_Control(pDX, IDC_SHUFFLE_WIDTH, m_ShuffleWidth);
	DDX_Control(pDX, IDC_SHUFFLE_X, m_ShuffleX);
	DDX_Control(pDX, IDC_SHUFFLE_Y, m_ShuffleY);
	DDX_Control(pDX, IDC_STOP_HEIGHT, m_StopHeight);
	DDX_Control(pDX, IDC_STOP_WIDTH, m_StopWidth);
	DDX_Control(pDX, IDC_STOP_X, m_StopX);
	DDX_Control(pDX, IDC_STOP_Y, m_StopY);
	DDX_Control(pDX, IDC_VOLUME_HEIGHT, m_VolumeHeight);
	DDX_Control(pDX, IDC_VOLUME_WIDTH, m_VolumeWidth);
	DDX_Control(pDX, IDC_VOLUME_X, m_VolumeX);
	DDX_Control(pDX, IDC_VOLUME_Y, m_VolumeY);
	DDX_Control(pDX, IDC_MENU_HEIGHT, m_MenuHeight);
	DDX_Control(pDX, IDC_MENU_WIDTH, m_MenuWidth);
	DDX_Control(pDX, IDC_MENU_X, m_MenuX);
	DDX_Control(pDX, IDC_MENU_Y, m_MenuY);
	DDX_Control(pDX, IDC_MUSIC_HEIGHT, m_MusicHeight);
	DDX_Control(pDX, IDC_MUSIC_WIDTH, m_MusicWidth);
	DDX_Control(pDX, IDC_MUSIC_X, m_MusicX);
	DDX_Control(pDX, IDC_MUSIC_Y, m_MusicY);
	DDX_Control(pDX, IDC_PICTURES_HEIGHT, m_PicturesHeight);
	DDX_Control(pDX, IDC_PICTURES_WIDTH, m_PicturesWidth);
	DDX_Control(pDX, IDC_PICTURES_X, m_PicturesX);
	DDX_Control(pDX, IDC_PICTURES_Y, m_PicturesY);
	DDX_Control(pDX, IDC_VIDEO_HEIGHT, m_VideoHeight);
	DDX_Control(pDX, IDC_VIDEO_WIDTH, m_VideoWidth);
	DDX_Control(pDX, IDC_VIDEO_X, m_VideoX);
	DDX_Control(pDX, IDC_VIDEO_Y, m_VideoY);
	DDX_Control(pDX, IDC_BMPSIZE_HOVERS, m_BmpSizeHovers);
	DDX_Control(pDX, IDC_BMPSIZE_INS, m_BmpSizeIns);
	DDX_Control(pDX, IDC_BMPSIZE_OUTS, m_BmpSizeOuts);
	DDX_Control(pDX, IDC_DESTDIR, m_DestDir);
	DDX_Control(pDX, IDC_FILE_HOVERS, m_FileHovers);
	DDX_Control(pDX, IDC_FILE_INS, m_FileIns);
	DDX_Control(pDX, IDC_FILE_OUTS, m_FileOuts);
	DDX_Control(pDX, IDC_STATIC_CLEAR, m_LabelClear);
	DDX_Control(pDX, IDC_STATIC_FORWARD, m_LabelForward);
	DDX_Control(pDX, IDC_STATIC_LOAD, m_LabelLoad);
	DDX_Control(pDX, IDC_STATIC_PAUSE, m_LabelPause);
	DDX_Control(pDX, IDC_STATIC_PLAY, m_LabelPlay);
	DDX_Control(pDX, IDC_STATIC_PROGRESS, m_LabelProgress);
	DDX_Control(pDX, IDC_STATIC_RANDOM, m_LabelRandom);
	DDX_Control(pDX, IDC_STATIC_REVERSE, m_LabelReverse);
	DDX_Control(pDX, IDC_STATIC_SAVE, m_LabelSave);
	DDX_Control(pDX, IDC_STATIC_SHUFFLE, m_LabelShuffle);
	DDX_Control(pDX, IDC_STATIC_STOP, m_LabelStop);
	DDX_Control(pDX, IDC_STATIC_MENU, m_LabelMenu);
	DDX_Control(pDX, IDC_STATIC_MUSIC, m_LabelMusic);
	DDX_Control(pDX, IDC_STATIC_PICTURES, m_LabelPictures);
	DDX_Control(pDX, IDC_STATIC_VIDEO, m_LabelVideo);
	DDX_Control(pDX, IDC_STATIC_VOLUME, m_LabelVolume);
	DDX_Control(pDX, IDC_PIC_HOVERS, m_PicHovers);
	DDX_Control(pDX, IDC_PIC_OUTS, m_PicOuts);
	DDX_Control(pDX, IDC_PIC_INS, m_PicIns);
	DDX_Control(pDX, IDC_PIC_TEST, m_PicTest);
	DDX_Control(pDX, IDC_INS_LABEL, m_InsLabel);
	DDX_Control(pDX, IDC_TEST_LABEL, m_TestLabel);
	DDX_Control(pDX, IDC_OUTS_LABEL, m_OutsLabel);
	DDX_Control(pDX, IDC_HOVERS_LABEL, m_HoversLabel);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CBitmapCutterDlg, CDialog)
	//{{AFX_MSG_MAP(CBitmapCutterDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_FILE_INS, OnButtonFileIns)
	ON_BN_CLICKED(IDC_BUTTON_FILE_OUTS, OnButtonFileOuts)
	ON_BN_CLICKED(IDC_BUTTON_FILE_HOVERS, OnButtonFileHovers)
	ON_BN_CLICKED(IDC_BUTTON_DEST, OnButtonDest)
	ON_BN_CLICKED(IDC_SAVE, OnSave)
	ON_EN_CHANGE(IDC_X1, OnUpdateXY)
	ON_EN_CHANGE(IDC_RED, OnUpdateRGB)
	ON_BN_CLICKED(IDC_INS, OnUpdateInOut)
	ON_BN_CLICKED(IDC_DRAWSAVE, OnDrawsave)
	ON_BN_CLICKED(IDC_DRAW, OnDrawLines)
	ON_BN_CLICKED(IDC_REREAD, OnReread)
	ON_BN_CLICKED(IDC_BLACK_WHITE, OnRadioBw)
	ON_BN_CLICKED(IDC_OTHER, OnRadioOther)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_ZERO, OnZero)
	ON_BN_CLICKED(IDC_REMOVE_DRAW, OnRemoveDraw)
	ON_BN_CLICKED(IDC_PIC_INS, OnPicIns)
	ON_BN_CLICKED(IDC_BUTTON_FILE_SETTINGS, OnButtonFileSettings)
	ON_BN_CLICKED(IDC_OUTS, OnUpdateInOut)
	ON_BN_CLICKED(IDC_HOVER, OnUpdateInOut)
	ON_BN_CLICKED(IDC_CANCEL, OnCancel)
	ON_EN_CHANGE(IDC_X2, OnUpdateXY)
	ON_EN_CHANGE(IDC_X3, OnUpdateXY)
	ON_EN_CHANGE(IDC_X4, OnUpdateXY)
	ON_EN_CHANGE(IDC_Y1, OnUpdateXY)
	ON_EN_CHANGE(IDC_Y2, OnUpdateXY)
	ON_EN_CHANGE(IDC_Y3, OnUpdateXY)
	ON_EN_CHANGE(IDC_Y4, OnUpdateXY)
	ON_EN_CHANGE(IDC_Y5, OnUpdateXY)
	ON_EN_CHANGE(IDC_Y6, OnUpdateXY)
	ON_EN_CHANGE(IDC_LINEWIDTH, OnUpdateXY)
	ON_EN_CHANGE(IDC_GREEN, OnUpdateRGB)
	ON_EN_CHANGE(IDC_BLUE, OnUpdateRGB)
	ON_BN_CLICKED(IDC_BGTYPE_STRETCHED, OnBgtypeStretched)
	ON_BN_CLICKED(IDC_BGTYPE_FIXED, OnBgtypeFixed)
	ON_BN_CLICKED(IDC_BGTYPE_TILED, OnBgtypeTiled)
	//}}AFX_MSG_MAP
	ON_MESSAGE(MB_BITMAP_CUTTER_MSG, OnArrowKey)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBitmapCutterDlg message handlers

BOOL CBitmapCutterDlg::OnInitDialog()
{
//	logger.open("bitmapcutter.log");
//	AutoLog al("OnInitDialog",FALSE);
	CDialog::OnInitDialog();

	CString v = "BitmapCutter   ";
	v += MUZIKBROWZER_VERSION;
	SetWindowText(v);

	{
//			AutoLog am("AppMutex",FALSE);
		// App mutex
		CString amemsg,ammsg;
		#define MUZIKBROWZERAPPMUTEX "MuzikbrowzerBitMapCutterAppMutex"
		#define MUZIKBROWZERAPPMUTEXGLOBAL "Global\\MuzikbrowzerBitMapCutterAppMutex"

		PSECURITY_DESCRIPTOR psd = malloc(sizeof(SECURITY_DESCRIPTOR));
		SECURITY_ATTRIBUTES sa ;

		BOOL isdresult = InitializeSecurityDescriptor(psd, 
			SECURITY_DESCRIPTOR_REVISION);
		DWORD e = ::GetLastError();
		if (isdresult == 0) {
			amemsg += "InitializeSecurityDescriptor error: ";
			amemsg += MBFormatError(e);
//			logger.log(amemsg);
		}

		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = psd;
		sa.bInheritHandle = FALSE;

		HANDLE h = CreateMutex(&sa,FALSE, MUZIKBROWZERAPPMUTEXGLOBAL);
		e = GetLastError();
		if (NULL == h || ERROR_ALREADY_EXISTS == e) {
			amemsg = "CreateMutex " + CString(MUZIKBROWZERAPPMUTEXGLOBAL);
			amemsg += "\r\n";
			amemsg += MBFormatError(e);
			amemsg += "\r\n";
			h = CreateMutex(&sa,FALSE, MUZIKBROWZERAPPMUTEX);
			e = GetLastError();
//			logger.log(amemsg);
		}

		if (NULL == h || ERROR_ALREADY_EXISTS == e) {
			ammsg = ("It looks like ");
			ammsg += MUZIKBROWZER;
			ammsg += " is already running.\r\nYou should only run one instance at a time.\r\nContinue at your own risk.\r\n";
			amemsg = "CreateMutex " + CString(MUZIKBROWZERAPPMUTEX);
			amemsg += "\r\n";
			amemsg += MBFormatError(e);
			amemsg += "\r\n";
			int r = MessageBox(ammsg, MUZIKBROWZER, MB_ICONSTOP);
			if (0 == r) {
				amemsg += "user aborted";
//				logger.log(amemsg);
				_exit(0);
			} else {
				amemsg += "user continued";
			}
//			logger.log(amemsg);
		}
		free (psd);
	}

	{
//		AutoLog al("Rest of InitDialog",FALSE);

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
//	SetIcon(m_hIcon, TRUE);			// Set big icon
//	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
//	ReadReg();		
	EnableDisable();

	m_ReRead = FALSE;
	m_DrawLines = FALSE;
	m_UpdateXY = FALSE;
	m_Dragging = FALSE;

	// first get the last saveto dir
	RegistryKey regDest( HKEY_LOCAL_MACHINE, "Software\\BitmapCutter");
	char buf[1000];

	regDest.Read("dest",buf,999,"");
	m_DestDir.SetWindowText(buf);
	m_sDest = buf;

	ReadReg();
	GetClientRect(m_ClientRect);
	GetWindowRect(m_WindowRect);

	Init=TRUE;
	if (m_sFileIns != "") 
		OnButtonFileIns();
	if (m_sFileOuts != "") 
		OnButtonFileOuts();
	if (m_sFileHovers != "") 
		OnButtonFileHovers();
//	if (m_sFileSettings != "")
//		OnButtonFileSettings();
	
	Init=FALSE;
	
	m_LabelMenu.GetWindowRect(MenuLabel);
	ScreenToClient(MenuLabel);
	m_LabelMusic.GetWindowRect(MusicLabel);
	ScreenToClient(MusicLabel);
	m_LabelPictures.GetWindowRect(PicturesLabel);
	ScreenToClient(PicturesLabel);
	m_LabelVideo.GetWindowRect(VideoLabel);
	ScreenToClient(VideoLabel);
	m_LabelStop.GetWindowRect(StopLabel);
	ScreenToClient(StopLabel);
	m_LabelPlay.GetWindowRect(PlayLabel);
	ScreenToClient(PlayLabel);
	m_LabelPause.GetWindowRect(PauseLabel);
	ScreenToClient(PauseLabel);
	m_LabelReverse.GetWindowRect(ReverseLabel);
	ScreenToClient(ReverseLabel);
	m_LabelForward.GetWindowRect(ForwardLabel);
	ScreenToClient(ForwardLabel);
	m_LabelRandom.GetWindowRect(RandomLabel);
	ScreenToClient(RandomLabel);
	m_LabelShuffle.GetWindowRect(ShuffleLabel);
	ScreenToClient(ShuffleLabel);
	m_LabelClear.GetWindowRect(ClearLabel);
	ScreenToClient(ClearLabel);
	m_LabelLoad.GetWindowRect(LoadLabel);
	ScreenToClient(LoadLabel);
	m_LabelSave.GetWindowRect(SaveLabel);
	ScreenToClient(SaveLabel);
	m_LabelVolume.GetWindowRect(VolumeLabel);
	ScreenToClient(VolumeLabel);
	m_LabelProgress.GetWindowRect(ProgressLabel);
	ScreenToClient(ProgressLabel);

//	CreateTest();
//	m_TMenuButton.ShowWindow(SW_HIDE);
//	m_TMusicButton.ShowWindow(SW_HIDE);
//	m_TPicturesButton.ShowWindow(SW_HIDE);
//	m_TVideoButton.ShowWindow(SW_HIDE);
	m_TStopButton.ShowWindow(SW_HIDE);
	m_TPlayButton.ShowWindow(SW_HIDE);
	m_TPauseButton.ShowWindow(SW_HIDE);
	m_TReverseButton.ShowWindow(SW_HIDE);
	m_TForwardButton.ShowWindow(SW_HIDE);
	m_TRandomButton.ShowWindow(SW_HIDE);
	m_TShuffleButton.ShowWindow(SW_HIDE);
	m_TClearButton.ShowWindow(SW_HIDE);
	m_TLoadButton.ShowWindow(SW_HIDE);
	m_TSaveButton.ShowWindow(SW_HIDE);
	m_TVolume.ShowWindow(SW_HIDE);
	m_TProgress.ShowWindow(SW_HIDE);


	CString skindef = m_sDest + "\\" + MB_SKIN_DEF;
	if ("" != m_sDest && FileUtil::IsReadable(skindef)) {
		CreateTest();
	}
	m_Ins.SetCheck(TRUE);
	m_Outs.SetCheck(TRUE);
	m_Hovers.SetCheck(TRUE);
	OnDrawLines();
	EnableDisable();

    m_MenuHeight.EnableWindow(FALSE);
    m_MenuWidth.EnableWindow(FALSE);
    m_MenuX.EnableWindow(FALSE);
    m_MenuY.EnableWindow(FALSE);
    m_MusicHeight.EnableWindow(FALSE);
    m_MusicWidth.EnableWindow(FALSE);
    m_MusicX.EnableWindow(FALSE);
    m_MusicY.EnableWindow(FALSE);
    m_PicturesHeight.EnableWindow(FALSE);
    m_PicturesWidth.EnableWindow(FALSE);
    m_PicturesX.EnableWindow(FALSE);
    m_PicturesY.EnableWindow(FALSE);
    m_VideoHeight.EnableWindow(FALSE);
    m_VideoWidth.EnableWindow(FALSE);
    m_VideoX.EnableWindow(FALSE);
    m_VideoY.EnableWindow(FALSE);
	RedrawWindow();
 
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CBitmapCutterDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this);
		m_DrawSliderRects = TRUE;
		m_DrawSliderChannel = TRUE;
		if (m_sDest != "" && m_sFileIns != "") 
			drawIt(&dc);
	}
}
void CBitmapCutterDlg::drawIt(CDC * dc) {
		CRect crect,wrect;

		m_PicIns.GetWindowRect(wrect);
		crect = wrect;
		ScreenToClient(crect);
		m_STCFlag = TRUE;

		drawLines(dc,crect,wrect, 0, m_cdibIns);

		m_PicOuts.GetWindowRect(wrect);
		crect = wrect;
		ScreenToClient(crect);
		m_STCFlag = TRUE;

		drawLines(dc,crect,wrect, 1, m_cdibOuts);

		m_PicHovers.GetWindowRect(wrect);
		crect = wrect;
		ScreenToClient(crect);
		m_STCFlag = TRUE;
		int InOrOut = 0;

		drawLines(dc,crect,wrect, 1, m_cdibHovers);

		m_PicTest.GetWindowRect(wrect);
		crect = wrect;
		ScreenToClient(crect);
		m_STCFlag = TRUE;
		InOrOut = 0;
		m_DrawSliderRects = FALSE;
		m_DrawSliderChannel = TRUE;

		drawLines(dc,crect,wrect, 1, m_cdibTest);

}
void CBitmapCutterDlg::drawLines(CDC * dc, CRect crect, CRect wrect, 
								 int InOrOut, CDIBSectionLite & cdib) {
	CEditsToInts();

	CRect prect;

	prect = crect;

	CDC tmpDC;
	tmpDC.CreateCompatibleDC(dc);
	HBITMAP oldbm = (HBITMAP)::SelectObject(tmpDC.m_hDC, (HBITMAP)cdib);

	dc->BitBlt(prect.left,prect.top,prect.Width(),prect.Height(),
		&tmpDC,0,0,SRCCOPY);
	(HBITMAP)::SelectObject(tmpDC.m_hDC, oldbm);

	CRect client;

	Menu.left = m_nMenuX;
	Menu.top  = m_nMenuY;
	Menu.bottom = m_nMenuY +m_nMenuHeight - 1;
	Menu.right = m_nMenuX + m_nMenuWidth - 1;
	m_rects[0].rect = &Menu;
	m_rects[0].label = &m_LabelMenu;

	Music.left = m_nMusicX;
	Music.top  = m_nMusicY;
	Music.bottom = m_nMusicY +m_nMusicHeight - 1;
	Music.right = m_nMusicX + m_nMusicWidth - 1;
	m_rects[1].rect = &Music;
	m_rects[1].label = &m_LabelMusic;

	Pictures.left = m_nPicturesX;
	Pictures.top  = m_nPicturesY;
	Pictures.bottom = m_nPicturesY +m_nPicturesHeight - 1;
	Pictures.right = m_nPicturesX + m_nPicturesWidth - 1;
	m_rects[2].rect = &Pictures;
	m_rects[2].label = &m_LabelPictures;

	Video.left = m_nVideoX;
	Video.top  = m_nVideoY;
	Video.bottom = m_nVideoY +m_nVideoHeight - 1;
	Video.right = m_nVideoX + m_nVideoWidth - 1;
	m_rects[3].rect = &Video;
	m_rects[3].label = &m_LabelVideo;
	
	Stop.left = m_nStopX;
	Stop.top  = m_nStopY;
	Stop.bottom = m_nStopY +m_nStopHeight - 1;
	Stop.right = m_nStopX + m_nStopWidth - 1;
	m_rects[4].rect = &Stop;
	m_rects[4].label = &m_LabelStop;
	Play.left = m_nPlayX;
	Play.top  = m_nPlayY;
	Play.bottom = m_nPlayY +m_nPlayHeight - 1;
	Play.right = m_nPlayX + m_nPlayWidth - 1;
	m_rects[5].rect = &Play;
	m_rects[5].label = &m_LabelPlay;
	Pause.left = m_nPauseX;
	Pause.top  = m_nPauseY;
	Pause.bottom = m_nPauseY +m_nPauseHeight - 1;
	Pause.right = m_nPauseX + m_nPauseWidth - 1;
	m_rects[6].rect = &Pause;
	m_rects[6].label = &m_LabelPause;
	Reverse.left = m_nReverseX;
	Reverse.top  = m_nReverseY;
	Reverse.bottom = m_nReverseY +m_nReverseHeight - 1;
	Reverse.right = m_nReverseX + m_nReverseWidth - 1;
	m_rects[7].rect = &Reverse;
	m_rects[7].label = &m_LabelReverse;
	Forward.left = m_nForwardX;
	Forward.top  = m_nForwardY;
	Forward.bottom = m_nForwardY +m_nForwardHeight - 1;
	Forward.right = m_nForwardX + m_nForwardWidth - 1;
	m_rects[8].rect = &Forward;
	m_rects[8].label = &m_LabelForward;
	Random.left = m_nRandomX;
	Random.top  = m_nRandomY;
	Random.bottom = m_nRandomY +m_nRandomHeight - 1;
	Random.right = m_nRandomX + m_nRandomWidth - 1;
	m_rects[9].rect = &Random;
	m_rects[9].label = &m_LabelRandom;
	Shuffle.left = m_nShuffleX;
	Shuffle.top  = m_nShuffleY;
	Shuffle.bottom = m_nShuffleY +m_nShuffleHeight - 1;
	Shuffle.right = m_nShuffleX + m_nShuffleWidth - 1;
	m_rects[10].rect = &Shuffle;
	m_rects[10].label = &m_LabelShuffle;
	Clear.left = m_nClearX;
	Clear.top  = m_nClearY;
	Clear.bottom = m_nClearY +m_nClearHeight - 1;
	Clear.right = m_nClearX + m_nClearWidth - 1;
	m_rects[11].rect = &Clear;
	m_rects[11].label = &m_LabelClear;
	Load.left = m_nLoadX;
	Load.top  = m_nLoadY;
	Load.bottom = m_nLoadY +m_nLoadHeight - 1;
	Load.right = m_nLoadX + m_nLoadWidth - 1;
	m_rects[12].rect = &Load;
	m_rects[12].label = &m_LabelLoad;
	Save.left = m_nSaveX;
	Save.top  = m_nSaveY;
	Save.bottom = m_nSaveY +m_nSaveHeight - 1;
	Save.right = m_nSaveX + m_nSaveWidth - 1;
	m_rects[13].rect = &Save;
	m_rects[13].label = &m_LabelSave;
	Volume.left = m_nVolumeX;
	Volume.top  = m_nVolumeY;
	Volume.bottom = m_nVolumeY +m_nVolumeHeight - 1;
	Volume.right = m_nVolumeX + m_nVolumeWidth - 1;
	m_rects[14].rect = &Volume;
	m_rects[14].label = &m_LabelVolume;
	Progress.left = m_nProgressX;
	Progress.top  = m_nProgressY;
	Progress.bottom = m_nProgressY +m_nProgressHeight - 1;
	Progress.right = m_nProgressX + m_nProgressWidth - 1;
	m_rects[15].rect = &Progress;
	m_rects[15].label = &m_LabelProgress;

	if (m_DrawLines) {
		drawButton(0, dc, wrect, Menu, InOrOut);	
		drawButton(1, dc, wrect, Music, InOrOut);	
		drawButton(2, dc, wrect, Pictures, InOrOut);	
		drawButton(3, dc, wrect, Video, InOrOut);	
		drawButton(4, dc, wrect, Stop, InOrOut);	
		drawButton(5, dc, wrect, Play, InOrOut);
		drawButton(6, dc, wrect, Pause, InOrOut);
		drawButton(7, dc, wrect, Reverse, InOrOut);
		drawButton(8, dc, wrect, Forward, InOrOut);
		drawButton(9, dc, wrect, Random, InOrOut);
		drawButton(10, dc, wrect, Shuffle, InOrOut);
		drawButton(11, dc, wrect, Clear, InOrOut);
		drawButton(12, dc, wrect, Load, InOrOut);
		drawButton(13, dc, wrect, Save, InOrOut);
		//drawButton(dc, wrect, volume, InOrOut);

		if (m_DrawSliderChannel) {
			int chx,chy;
			chx = Volume.left + (Volume.Width() / 2);
			chy = Volume.top;
			line(dc, wrect, chx, chy, chx, chy + Volume.Height());
			chx = Progress.left;
			chy = Progress.top + (Progress.Height() / 2);
			line(dc, wrect, chx, chy, chx + Progress.Width(), chy );
			if (m_DrawSliderRects) {
				drawButton(14,dc,wrect,Volume,InOrOut);
				drawButton(15,dc,wrect,Progress,InOrOut);
			}
		}
	}

}
void CBitmapCutterDlg::drawButton(int buttoni, CDC * dc, CRect wrect, CRect button, int InOrOut) {

	if (button.Width() == 0 || button.Height() == 0)
		return;
	CPen penWhite(PS_SOLID, 1, RGB(255,255,255)); // White
	CPen penBlack(PS_SOLID, 1, RGB(0,0,0)); // black
	CPen penRGB(PS_SOLID,1,RGB(m_nRed,m_nGreen,m_nBlue));;
	CPen penRed(PS_SOLID,2,RGB(255,0,0));

	CPen * penUL;
	CPen * penBR;
	CPen * oldpen = NULL;
	if (InOrOut == 0) {		// In
		penUL = &penBlack;
		penBR = &penWhite;
	} else if (InOrOut == 1) {		// Out
		penUL = &penWhite;
		penBR = &penBlack;
	}
	if (m_RGB) {
		penUL = &penRGB;
		penBR = penUL;
	}
	int i,j;
	for(j = 0 ; j < NUM_CRECTS; j++) {
		if (buttoni != j) {
			CRect intersection, cand,jrect;
			cand = *m_rects[buttoni].rect;
			cand.InflateRect(1,1,1,1);
			jrect = *m_rects[j].rect;
			if (jrect.Width() && jrect.Height()) {
				CString val;
				BOOL r = intersection.IntersectRect(cand, jrect);
				if (r != 0 ) {
					OutputDebugString(val + " red\r\n");
					penUL = &penRed;
					penBR = &penRed;
				}
			}
		}
	}
//	if (10 == buttoni || 11 == buttoni) {
//		penUL = &penRGB;
//		penBR = &penRGB;
//	}
	oldpen = dc->SelectObject(penUL);

	for(i = 0 ; i < m_nLineWidth; i++) {


		// ul to ur
		line(dc, wrect, button.left, button.top + i,
						button.right+1, button.top + i);
		// ul to bl
		line(dc, wrect, button.left + i, button.top, 
						button.left + i, button.bottom+1);
		dc->SelectObject(penBR);
		// bl to br
		line(dc, wrect, button.left+1, button.bottom - i, 
						button.right, button.bottom - i);
		// br to ur
		line(dc, wrect, button.right - i, button.bottom,
						button.right - i, button.top);
	}

	dc->SelectObject(oldpen);
}
void CBitmapCutterDlg::line(CDC * dc, CRect wrect, 
							int x1,int y1, int x2,int y2) {
	x1 += wrect.left;
	x2 += wrect.left;
	y1 += wrect.top;
	y2 += wrect.top;
	CPoint p1(x1,y1);
	CPoint p2(x2,y2);
	if (m_STCFlag) {
		ScreenToClient(&p1);
		ScreenToClient(&p2);
	}

	dc->MoveTo(p1);
	dc->LineTo(p2);

}


// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CBitmapCutterDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}
#define RFIRST  0
#define RSTOP  0
#define RPLAY  1
#define RPAUSE  2
#define RREV  3
#define RFF  4
#define RRANDOM  5
#define RSHUFFLE  6
#define RCLEAR  7
#define RLOAD  8
#define RSAVE  9
#define RVOL  10
#define RLAST  10


void CBitmapCutterDlg::OnButtonFileIns() {
	OnButtonFile(m_FileIns, m_sFileIns, m_hBitmapIns, m_cdibIns,
		m_PicIns, m_PicInsCRect, m_PicInsDlgRect, m_BmpSizeIns);
	m_FileIns.SetWindowText(m_sFileIns);

}
void CBitmapCutterDlg::OnButtonFileOuts() {
	OnButtonFile(m_FileOuts, m_sFileOuts, m_hBitmapOuts, m_cdibOuts,
		m_PicOuts, m_PicOutsCRect, m_PicOutsDlgRect, m_BmpSizeOuts);
	m_cdibTest.Load(m_sFileOuts);
}
void CBitmapCutterDlg::OnButtonFileHovers() {
	OnButtonFile(m_FileHovers, m_sFileHovers, m_hBitmapHovers, m_cdibHovers,
		m_PicHovers, m_PicHoversCRect, m_PicHoversDlgRect, m_BmpSizeHovers);
		m_FileHovers.SetWindowText(m_sFileHovers);
}
void CBitmapCutterDlg::OnButtonFileSettings() 
{
	m_FileSettings.GetWindowText(m_sFileSettings);
	CFileDialog dialog(TRUE, "mbsd", m_sFileSettings,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		"mbsd's  {*.mbsd}|SkinDefCustom.mbsd|All Files {*.*}|*.*||");
	int ret = 0;
	ret = dialog.DoModal();
	if (ret == IDOK) {
	  // a file was selected
		m_sFileSettings = dialog.GetPathName();
		m_FileSettings.SetWindowText(m_sFileSettings);

		UpdateData(FALSE);
	}
	
}
void CBitmapCutterDlg::OnButtonFile(CEdit & clabel, CString & label,
					HBITMAP & hbitmap, CDIBSectionLite & cdib, CStatic & cpic,
					CRect & picrect, CRect & picdlgrect, CStatic & csizelabel) 
{
	m_Saved = FALSE;
	CString path ;

	if (!Init) {
		if (m_ReRead == FALSE) {
			CFileDialog dialog(TRUE, "bmp", label,
				OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
				"bmp's  {*.bmp}|*.bmp|All Files {*.*}|*.*||");
			int ret = 0;
			ret = dialog.DoModal();
			if (ret == IDOK) {
			  // a file was selected
				path = dialog.GetPathName();
				clabel.SetWindowText(path);
				label = path;
				UpdateData(FALSE);
			}	
		}
	}
	m_ReRead = FALSE;

	GetClientRect(m_ClientRect);
	GetWindowRect(m_WindowRect);
	static origwidth = m_WindowRect.Width();
	static origheight = m_WindowRect.Height();
	m_WindowRect.right = m_WindowRect.left + origwidth;
	m_WindowRect.bottom = m_WindowRect.top + origheight;

	if (hbitmap != NULL) 
		::DeleteObject(hbitmap);

	if (label == "")
		return;

	BOOL rload = cdib.Load(label);
	if (rload == FALSE) {
		CString msg ="Can't load "+label;
		MessageBox(  msg, _T("Bmp Cutter Error"), MB_OK | MB_ICONINFORMATION );
		return;
	}

//	hbitmap = (HBITMAP)cdib;
	CRect LabelRect, SizeRect, PicRect;

	m_PicOuts.GetWindowRect(PicRect);
	ScreenToClient(PicRect);
	m_BmpSizeOuts.GetWindowRect(SizeRect);
	ScreenToClient(SizeRect);
	m_OutsLabel.GetWindowRect(LabelRect);
	ScreenToClient(LabelRect);
	int x0 = LabelRect.left;
	int y0 = LabelRect.top;
	m_BmpSizeOuts.MoveWindow(LabelRect.right+5, LabelRect.top,
		SizeRect.Width(),SizeRect.Height(),TRUE);
	m_PicOuts.MoveWindow(LabelRect.left, LabelRect.bottom+10,
		cdib.GetWidth(), cdib.GetHeight(), TRUE);

	int x1 = x0 + cdib.GetWidth() + 10;

	m_PicHovers.GetWindowRect(PicRect);
	ScreenToClient(PicRect);
	m_BmpSizeHovers.GetWindowRect(SizeRect);
	ScreenToClient(SizeRect);
	m_HoversLabel.GetWindowRect(LabelRect);
	ScreenToClient(LabelRect);
	m_HoversLabel.MoveWindow(x1,y0,LabelRect.Width(),LabelRect.Height(),TRUE);
	m_BmpSizeHovers.MoveWindow(x1+ LabelRect.Width()+5,y0,
		SizeRect.Width(),SizeRect.Height(),TRUE);
	m_PicHovers.MoveWindow(x1,LabelRect.bottom+10,
		cdib.GetWidth(), cdib.GetHeight(), TRUE);

	int y1 = LabelRect.bottom + 10 + cdib.GetHeight();

	y1+=5;

	m_PicIns.GetWindowRect(PicRect);
	ScreenToClient(PicRect);
	m_BmpSizeIns.GetWindowRect(SizeRect);
	ScreenToClient(SizeRect);
	m_InsLabel.GetWindowRect(LabelRect);
	ScreenToClient(LabelRect);
	m_InsLabel.MoveWindow(x0,y1,LabelRect.Width(),LabelRect.Height(),TRUE);
	m_BmpSizeIns.MoveWindow(x0+ LabelRect.Width()+5,y1,
		SizeRect.Width(),SizeRect.Height(),TRUE);
	int hy = LabelRect.bottom+10;
	m_PicIns.MoveWindow(x0,hy,
		cdib.GetWidth(), cdib.GetHeight(), TRUE);

	m_PicTest.GetWindowRect(PicRect);
	ScreenToClient(PicRect);
	m_TestLabel.GetWindowRect(LabelRect);
	ScreenToClient(LabelRect);
	m_TestLabel.MoveWindow(x1,y1,LabelRect.Width(),LabelRect.Height(),TRUE);
	m_PicTest.MoveWindow(x1,hy,
		cdib.GetWidth(), cdib.GetHeight(), TRUE);
	m_PicTest.GetWindowRect(PicRect);
	ScreenToClient(PicRect);

	int nr = m_WindowRect.Width();
	int nb = m_WindowRect.Height();
	int tr = PicRect.right;
	int tb = PicRect.bottom;
	if (tr > nr) {
		m_WindowRect.right = m_WindowRect.left + tr + 20;
	}
	if (tb > nb) {
		m_WindowRect.bottom = m_WindowRect.top + tb + 30;
	}
	MoveWindow(m_WindowRect,TRUE);


	

	cpic.GetClientRect(picrect);

	CRect rect;
	cpic.GetWindowRect(rect);
	ScreenToClient(rect);
	picdlgrect = rect;
	
//	ReadReg();
	calc(cdib, csizelabel);
	Invalidate();
}
void CBitmapCutterDlg::calc(CDIBSectionLite & cdib, CStatic & csizelabel) {
	int width = cdib.GetWidth();
	int height = cdib.GetHeight();
	CString bmpsize = numToString(width) 
		+ "," + numToString(height);
	csizelabel.SetWindowText(bmpsize);

	m_UpdateXY = FALSE;
	m_Applied = FALSE;
	UpdateData(TRUE);
	EnableDisable();
	Invalidate(TRUE);
}
void CBitmapCutterDlg::OnApplyVol() 
{
	OnApply();
//	calc();
	Invalidate(TRUE);
}
void CBitmapCutterDlg::OnApply() 
{
	UpdateData(FALSE);

	CString val;

	m_Red.GetWindowText(val);
	if (val != "") m_nRed = atoi(val);
	m_Green.GetWindowText(val);
	if (val != "") m_nGreen = atoi(val);
	m_Blue.GetWindowText(val);
	if (val != "") m_nBlue = atoi(val);

	m_TransRedMain.GetWindowText(val);
	if (val != "") m_nTransRedMain = atoi(val);
	m_TransGreenMain.GetWindowText(val);
	if (val != "") m_nTransGreenMain = atoi(val);
	m_TransBlueMain.GetWindowText(val);
	if (val != "") m_nTransBlueMain = atoi(val);

	m_TransRedPanel.GetWindowText(val);
	if (val != "") m_nTransRedPanel = atoi(val);
	m_TransGreenPanel.GetWindowText(val);
	if (val != "") m_nTransGreenPanel = atoi(val);
	m_TransBluePanel.GetWindowText(val);
	if (val != "") m_nTransBluePanel = atoi(val);

	Invalidate(TRUE);
	m_Applied = TRUE;
	m_UpdateXY = FALSE;
	EnableDisable();
	
}

void CBitmapCutterDlg::OnButtonDest() 
{
	CFolderDialog dialog(m_sDest);

	int ret;
	ret = dialog.DoModal();
	if (ret == IDOK) {
        // a file was selected
		CString path = dialog.GetPathName();
        m_DestDir.SetWindowText(path);
		m_sDest = path;

        UpdateData(FALSE);

	}	
	m_Saved = FALSE;

// save the saveto dir to the real registry
	RegistryKey regDest( HKEY_LOCAL_MACHINE, "Software\\BitmapCutter");
	regDest.Write("dest", m_sDest);

	ReadReg();
	Init=TRUE;
	if (m_sFileIns != "") 
		OnButtonFileIns();
	if (m_sFileOuts != "") 
		OnButtonFileOuts();
	if (m_sFileHovers != "") 
		OnButtonFileHovers();
	if (m_sFileSettings != "")
		OnButtonFileSettings();
	
	CreateTest();
	if (m_sFileHovers != "") 
		OnButtonFileHovers();
	Init=FALSE;
	EnableDisable();
}

void CBitmapCutterDlg::OnCancel() 
{
	StoreReg();
	CDialog::OnCancel();	
}


void CBitmapCutterDlg::EnableDisable() {

	m_ButtonDest.EnableWindow(TRUE);
	m_ChooseIns.EnableWindow(m_sDest != "");
	m_ChooseOuts.EnableWindow(m_sDest != "");
	m_ChooseHovers.EnableWindow(m_sDest != "");
	
	m_ButtonSave.EnableWindow(
		m_sFileIns != ""
		&& m_sDest != ""
		&&	(m_Ins.GetCheck() 
			|| m_Outs.GetCheck()
			|| m_Hovers.GetCheck())
//		&& (!m_UpdateXY || m_Applied)
			);
	m_DrawSave.EnableWindow(
		m_sFileIns != ""
		&& m_sDest != ""
		
			);
//	m_Draw.EnableWindow(
//		(m_Ins.GetCheck() || m_Outs.GetCheck())
//		&& m_sFileIns != ""
//		&& m_sDest != ""
//		);

	m_Ins.EnableWindow(m_sFileIns != "");
	m_Outs.EnableWindow(m_sFileOuts != "");
	m_Hovers.EnableWindow(m_sFileHovers != "");

	if (m_BgType == BGTILED) {
		m_BgTypeStretched.SetCheck(FALSE);
		m_BgTypeTiled.SetCheck(TRUE);
		m_BgTypeFixed.SetCheck(FALSE);
	} else if (m_BgType == BGFIXED) {
		m_BgTypeStretched.SetCheck(FALSE);
		m_BgTypeTiled.SetCheck(FALSE);
		m_BgTypeFixed.SetCheck(TRUE);
	} else {
		m_BgTypeStretched.SetCheck(TRUE);
		m_BgTypeTiled.SetCheck(FALSE);
		m_BgTypeFixed.SetCheck(FALSE);
	}

	Invalidate();




}




void CBitmapCutterDlg::OnUpdateXY() 
{
	m_UpdateXY = TRUE;
	m_Saved = FALSE;
	m_Applied = FALSE;
	CString msg;

	m_nLineWidth = atoi(msg);
	EnableDisable();
	
}
void CBitmapCutterDlg::OnUpdateInOut() 
{
	EnableDisable();
}
void CBitmapCutterDlg::OnUpdateRGB() 
{
	EnableDisable();
}
void CBitmapCutterDlg::OnSave() {
	OnApply();
	CString sIn ;
	CString sOut;
	CString sHover;
	CString msg;

	CDC *cdc = GetDC();
	HDC hDC = cdc->m_hDC;
	HDC hdcBmp;
	HBITMAP hOldBitmap;
	HBITMAP hOldBitmap2;
	m_hBitmapHovers=NULL;
	m_hBitmapIns=NULL;
	m_hBitmapOuts=NULL;

	CDC dcTmp;
	dcTmp.CreateCompatibleDC(NULL);
	hdcBmp = ::CreateCompatibleDC(NULL);
	CRect rect;
	m_DrawSliderRects = FALSE;
	m_DrawSliderChannel = TRUE;

	if (m_Ins.GetCheck()) {
		hOldBitmap2 = (HBITMAP)::SelectObject(hdcBmp, (HBITMAP)m_cdibIns);
		m_hBitmapIns = ::CreateCompatibleBitmap(hDC,
			m_cdibIns.GetWidth(),m_cdibIns.GetHeight());
		hOldBitmap = (HBITMAP)::SelectObject(dcTmp.m_hDC, m_hBitmapIns);
		::BitBlt(dcTmp.m_hDC,0,0,m_cdibIns.GetWidth(),m_cdibIns.GetHeight(),
			hdcBmp,0,0,SRCCOPY);
		::SelectObject(hdcBmp, hOldBitmap2);
		
		// drawlines
		m_STCFlag = FALSE;
		rect = CRect(0,0,m_cdibIns.GetWidth(),m_cdibIns.GetHeight());
		drawLines(&dcTmp,rect,rect, 0, m_cdibIns);
		::SelectObject(dcTmp.m_hDC, hOldBitmap);
	}
	if (m_Outs.GetCheck()) {
		hOldBitmap2 = (HBITMAP)::SelectObject(hdcBmp, (HBITMAP)m_cdibOuts);
		m_hBitmapOuts = ::CreateCompatibleBitmap(hDC,m_cdibOuts.GetWidth(),
			m_cdibOuts.GetHeight());
		hOldBitmap = (HBITMAP)::SelectObject(dcTmp.m_hDC, m_hBitmapOuts);
		::BitBlt(dcTmp.m_hDC,0,0,m_cdibOuts.GetWidth(),m_cdibOuts.GetHeight(),
			hdcBmp,0,0,SRCCOPY);
		::SelectObject(hdcBmp, hOldBitmap2);
		::SelectObject(dcTmp.m_hDC, hOldBitmap);
		sOut = m_sDest + "\\" + MB_SKIN_BUTTONBACKGROUND;
		sIn = "";
		sHover = "";
		SplitAndSave(m_PicOutsCRect,sIn,sOut,sHover,msg);

		// drawlines
		hOldBitmap = (HBITMAP)::SelectObject(dcTmp.m_hDC, m_hBitmapOuts);
		rect = CRect(0,0,m_cdibOuts.GetWidth(),m_cdibOuts.GetHeight());
		drawLines(&dcTmp,rect,rect, 1, m_cdibOuts);
		::SelectObject(dcTmp.m_hDC, hOldBitmap);
	}
	if (m_Hovers.GetCheck()) {
		hOldBitmap2 = (HBITMAP)::SelectObject(hdcBmp, (HBITMAP)m_cdibHovers);
		m_hBitmapHovers = ::CreateCompatibleBitmap(hDC,m_cdibHovers.GetWidth(),
			m_cdibHovers.GetHeight());
		hOldBitmap = (HBITMAP)::SelectObject(dcTmp.m_hDC, m_hBitmapHovers);
		::BitBlt(dcTmp.m_hDC,0,0,m_cdibHovers.GetWidth(),m_cdibHovers.GetHeight(),
			hdcBmp,0,0,SRCCOPY);
		::SelectObject(hdcBmp, hOldBitmap2);
		
		// drawlines
		m_STCFlag = FALSE;
		rect = CRect(0,0,m_cdibHovers.GetWidth(),m_cdibHovers.GetHeight());
		int InOrOut = 0;
		if (m_Ins.GetCheck())
			InOrOut = 0;
		else if (m_Outs.GetCheck())
			InOrOut = 1;
		drawLines(&dcTmp,rect,rect, /*InOrOut*/ 1, m_cdibHovers);
		::SelectObject(dcTmp.m_hDC, hOldBitmap);
	}
	
	::DeleteDC(dcTmp.m_hDC);
	::DeleteDC(hdcBmp);
	ReleaseDC(cdc);	
	
	sIn = m_sDest + "\\" + MB_SKIN_BUTTONMENUIN;
	sOut = m_sDest + "\\" + MB_SKIN_BUTTONMENUOUT;
	sHover = m_sDest + "\\" + MB_SKIN_BUTTONMENUHOVER;
//	SplitAndSave(Menu,sIn,sOut,sHover,msg);

	sIn = m_sDest + "\\" + MB_SKIN_BUTTONMUSICIN;
	sOut = m_sDest + "\\" + MB_SKIN_BUTTONMUSICOUT;
	sHover = m_sDest + "\\" + MB_SKIN_BUTTONMUSICHOVER;
	SplitAndSave(Music,sIn,sOut,sHover,msg);

	sIn = m_sDest + "\\" + MB_SKIN_BUTTONPICTURESIN;
	sOut = m_sDest + "\\" + MB_SKIN_BUTTONPICTURESOUT;
	sHover = m_sDest + "\\" + MB_SKIN_BUTTONPICTURESHOVER;
	SplitAndSave(Pictures,sIn,sOut,sHover,msg);

	sIn = m_sDest + "\\" + MB_SKIN_BUTTONVIDEOIN;
	sOut = m_sDest + "\\" + MB_SKIN_BUTTONVIDEOOUT;
	sHover = m_sDest + "\\" + MB_SKIN_BUTTONVIDEOHOVER;
	SplitAndSave(Video,sIn,sOut,sHover,msg);

	// stop
	sIn = m_sDest + "\\" + MB_SKIN_BUTTONSTOPIN;
	sOut = m_sDest + "\\" + MB_SKIN_BUTTONSTOPOUT;
	sHover = m_sDest + "\\" + MB_SKIN_BUTTONSTOPHOVER;
	SplitAndSave(Stop,sIn,sOut,sHover,msg);

	// play
	sIn = m_sDest + "\\" + MB_SKIN_BUTTONPLAYIN;
	sOut = m_sDest + "\\" + MB_SKIN_BUTTONPLAYOUT;
	sHover = m_sDest + "\\" + MB_SKIN_BUTTONPLAYHOVER;
	SplitAndSave(Play,sIn,sOut,sHover,msg);

	// pause
	sIn = m_sDest + "\\" + MB_SKIN_BUTTONPAUSEIN;
	sOut = m_sDest + "\\" + MB_SKIN_BUTTONPAUSEOUT;
	sHover = m_sDest + "\\" + MB_SKIN_BUTTONPAUSEHOVER;
	SplitAndSave(Pause,sIn,sOut,sHover,msg);

	sOut = m_sDest + "\\" + MB_SKIN_BUTTONVOLUMEBACKGROUND;
	sIn = "";
	sHover="";
	SplitAndSave(Volume,sIn,sOut,sHover,msg);
	sOut = m_sDest + "\\" + MB_SKIN_BUTTONPROGRESSBACKGROUND;
	sIn = "";
	sHover="";
	SplitAndSave(Progress,sIn,sOut,sHover,msg);

	sIn = m_sDest + "\\" + MB_SKIN_BUTTONREVERSEIN;
	sOut = m_sDest + "\\" + MB_SKIN_BUTTONREVERSEOUT;
	sHover = m_sDest + "\\" + MB_SKIN_BUTTONREVERSEHOVER;
	SplitAndSave(Reverse,sIn,sOut,sHover,msg);

	sIn = m_sDest + "\\" + MB_SKIN_BUTTONFASTFORWARDIN;
	sOut = m_sDest + "\\" + MB_SKIN_BUTTONFASTFORWARDOUT;
	sHover = m_sDest + "\\" + MB_SKIN_BUTTONFASTFORWARDHOVER;
	SplitAndSave(Forward,sIn,sOut,sHover,msg);

	sIn = m_sDest + "\\" + MB_SKIN_BUTTONRANDOMIN;
	sOut = m_sDest + "\\" + MB_SKIN_BUTTONRANDOMOUT;
	sHover = m_sDest + "\\" + MB_SKIN_BUTTONRANDOMHOVER;
	SplitAndSave(Random,sIn,sOut,sHover,msg);

	sIn = m_sDest + "\\" + MB_SKIN_BUTTONSHUFFLEIN;
	sOut = m_sDest + "\\" + MB_SKIN_BUTTONSHUFFLEOUT;
	sHover = m_sDest + "\\" + MB_SKIN_BUTTONSHUFFLEHOVER;
	SplitAndSave(Shuffle,sIn,sOut,sHover,msg);

	sIn = m_sDest + "\\" + MB_SKIN_BUTTONCLEARIN;
	sOut = m_sDest + "\\" + MB_SKIN_BUTTONCLEAROUT;
	sHover = m_sDest + "\\" + MB_SKIN_BUTTONCLEARHOVER;
	SplitAndSave(Clear,sIn,sOut,sHover,msg);

	sIn = m_sDest + "\\" + MB_SKIN_BUTTONLOADIN;
	sOut = m_sDest + "\\" + MB_SKIN_BUTTONLOADOUT;
	sHover = m_sDest + "\\" + MB_SKIN_BUTTONLOADHOVER;
	SplitAndSave(Load,sIn,sOut,sHover,msg);

	sIn = m_sDest + "\\" + MB_SKIN_BUTTONSAVEIN;
	sOut = m_sDest + "\\" + MB_SKIN_BUTTONSAVEOUT;
	sHover = m_sDest + "\\" + MB_SKIN_BUTTONSAVEHOVER;
	SplitAndSave(Save,sIn,sOut,sHover,msg);

	if (m_hBitmapHovers)
		::DeleteObject(m_hBitmapHovers);
	if (m_hBitmapIns)
		::DeleteObject(m_hBitmapIns);
	if (m_hBitmapOuts)
		::DeleteObject(m_hBitmapOuts);

	StoreReg();
	m_Saved = TRUE;
	EnableDisable();
	msg += "saved.";
	MessageBox(  msg, _T("BitmapCutter"), MB_OK | MB_ICONINFORMATION );
	CreateTest();

}
void CBitmapCutterDlg::SplitAndSave(CRect button,
									CString sIn, CString sOut, 
									CString sHover, CString & msg)
{

	int xSrc, ySrc, width, height;
	xSrc = button.left;
	ySrc = button.top;
	width = button.Width() + 1;   // cause CRects work this way!
	height = button.Height() + 1;

	CDC *cdc = GetDC();
	HDC hDC = cdc->m_hDC;
	HDC hdcTmp;
	HDC hdcBmp;
	HBITMAP hBitmap;
	HBITMAP hOldBitmap;
	HBITMAP hOldBitmap2;

	hdcTmp = ::CreateCompatibleDC(NULL);
	hdcBmp = ::CreateCompatibleDC(NULL);
	hOldBitmap2 = (HBITMAP)::SelectObject(hdcBmp, m_hBitmapIns);

	hBitmap = ::CreateCompatibleBitmap(hDC,width,height);
	hOldBitmap = (HBITMAP)::SelectObject(hdcTmp,hBitmap);
	::BitBlt(hdcTmp,0,0,width,height,hdcBmp,xSrc,ySrc,SRCCOPY);
	
	::SelectObject(hdcTmp,hOldBitmap);
	
	if ((m_Ins.GetCheck() || sOut =="") && sIn != "") {
		FileUtil::BmpSave(hBitmap, sIn);
		msg += sIn;
		msg += "\r\n";
	}
	::DeleteObject(hBitmap);

	::SelectObject(hdcBmp, hOldBitmap2);
	hOldBitmap2 = (HBITMAP)::SelectObject(hdcBmp, m_hBitmapOuts);

	hBitmap = ::CreateCompatibleBitmap(hDC,width,height);
	hOldBitmap = (HBITMAP)::SelectObject(hdcTmp,hBitmap);
	::BitBlt(hdcTmp,0,0,width,height,hdcBmp,xSrc,ySrc,SRCCOPY);
	
	::SelectObject(hdcTmp,hOldBitmap);
	
	if (m_Outs.GetCheck() && sOut != "") {
		FileUtil::BmpSave(hBitmap, sOut);
		msg += sOut;
		msg += "\r\n";
	}
	::DeleteObject(hBitmap);
	::SelectObject(hdcBmp, hOldBitmap2);
	hOldBitmap2 = (HBITMAP)::SelectObject(hdcBmp, m_hBitmapHovers);
	hBitmap = ::CreateCompatibleBitmap(hDC,width,height);
	hOldBitmap = (HBITMAP)::SelectObject(hdcTmp,hBitmap);
	::BitBlt(hdcTmp,0,0,width,height,hdcBmp,xSrc,ySrc,SRCCOPY);
	
	::SelectObject(hdcTmp,hOldBitmap);
	
	if (m_Hovers.GetCheck() && sHover != "") {
		FileUtil::BmpSave(hBitmap, sHover);
		msg += sHover;
		msg += "\r\n";
	}
	::DeleteObject(hBitmap);



	::DeleteDC(hdcTmp);
	::SelectObject(hdcBmp, hOldBitmap2);
	::DeleteDC(hdcBmp);
	ReleaseDC(cdc);	


	::DeleteObject(hBitmap);

}

void CBitmapCutterDlg::ReadReg() {
	char buf[1000];

	if ("" == m_sDest) return;


	// read the bmp names from the saveto dir
	CString file = m_sDest + "\\SkinBmps.txt";
	RegistryKey regBmps(file);
	regBmps.ReadFile();


	regBmps.Read("Insbmp", buf, 999, "");
    m_FileIns.SetWindowText(buf);
	m_sFileIns = buf;
	
	regBmps.Read("Outsbmp", buf, 999, "");
    m_FileOuts.SetWindowText(buf);
	m_sFileOuts = buf;
	
	regBmps.Read("Hoversbmp", buf, 999, "");
    m_FileHovers.SetWindowText(buf);
	m_sFileHovers = buf;
	
	regBmps.Read("Settings", buf, 999, "");
	m_FileSettings.SetWindowText(buf);
	m_sFileSettings = buf;


	// now read the config for this destdir/bmp
	
	file = m_sDest + "\\" + MB_SKIN_DEF;

	RegistryKey reg(file);
	reg.ReadFile();


	m_nRed = reg.Read("Red",-1);
	if (m_nRed == -1) {
		m_Red.SetWindowText("");
	} else {
		m_Red.SetWindowText(numToString(m_nRed));
	}
	m_nGreen = reg.Read("Green",-1);
	if (m_nGreen == -1) {
		m_Green.SetWindowText("");
	} else {
		m_Green.SetWindowText(numToString(m_nGreen));
	}
	m_nBlue = reg.Read("Blue",-1);
	if (m_nBlue == -1) {
		m_Blue.SetWindowText("");
	} else {
		m_Blue.SetWindowText(numToString(m_nBlue));
	}

	m_nTransRedMain = reg.Read("TransRedMain",254);
	if (m_nTransRedMain == -1) {
		m_TransRedMain.SetWindowText("");
	} else {
		m_TransRedMain.SetWindowText(numToString(m_nTransRedMain));
	}
	m_nTransGreenMain = reg.Read("TransGreenMain",-1);
	if (m_nTransGreenMain == -1) {
		m_TransGreenMain.SetWindowText("");
	} else {
		m_TransGreenMain.SetWindowText(numToString(m_nTransGreenMain));
	}
	m_nTransBlueMain = reg.Read("TransBlueMain",-1);
	if (m_nTransBlueMain == -1) {
		m_TransBlueMain.SetWindowText("");
	} else {
		m_TransBlueMain.SetWindowText(numToString(m_nTransBlueMain));
	}

	m_nTransRedPanel = reg.Read("TransRedPanel",253);
	if (m_nTransRedPanel == -1) {
		m_TransRedPanel.SetWindowText("");
	} else {
		m_TransRedPanel.SetWindowText(numToString(m_nTransRedPanel));
	}
	m_nTransGreenPanel = reg.Read("TransGreenPanel",-1);
	if (m_nTransGreenPanel == -1) {
		m_TransGreenPanel.SetWindowText("");
	} else {
		m_TransGreenPanel.SetWindowText(numToString(m_nTransGreenPanel));
	}
	m_nTransBluePanel = reg.Read("TransBluePanel",-1);
	if (m_nTransBluePanel == -1) {
		m_TransBluePanel.SetWindowText("");
	} else {
		m_TransBluePanel.SetWindowText(numToString(m_nTransBluePanel));
	}
	

	m_RGB = reg.Read("RGB", 0);
	if (m_RGB == 0) {
		m_BlackWhite.SetCheck(TRUE);
		m_Other.SetCheck(FALSE);
	} else {
		m_BlackWhite.SetCheck(FALSE);
		m_Other.SetCheck(TRUE);
	}
	m_nLineWidth = reg.Read("linewidth", 1);
	if (m_nLineWidth < 1) m_nLineWidth = 1;
	m_LineWidth.SetWindowText(numToString(m_nLineWidth));

	m_nMenuX = reg.Read("MenuX", 0);
	m_nMenuY = reg.Read("MenuY", 0);
	m_nMenuWidth = reg.Read("MenuWidth",0);
	m_nMenuHeight = reg.Read("MenuHeight",0);

	m_nMusicX = reg.Read("MusicX", 0);
	m_nMusicY = reg.Read("MusicY", 0);
	m_nMusicWidth = reg.Read("MusicWidth",0);
	m_nMusicHeight = reg.Read("MusicHeight",0);

	m_nPicturesX = reg.Read("PicturesX", 0);
	m_nPicturesY = reg.Read("PicturesY", 0);
	m_nPicturesWidth = reg.Read("PicturesWidth",0);
	m_nPicturesHeight = reg.Read("PicturesHeight",0);

	m_nVideoX = reg.Read("VideoX", 0);
	m_nVideoY = reg.Read("VideoY", 0);
	m_nVideoWidth = reg.Read("VideoWidth",0);
	m_nVideoHeight = reg.Read("VideoHeight",0);

	m_nStopX = reg.Read("StopX", 0);
	m_nStopY = reg.Read("StopY", 0);
	m_nStopWidth = reg.Read("StopWidth",0);
	m_nStopHeight = reg.Read("StopHeight",0);
	m_nPlayX = reg.Read("PlayX", 0);
	m_nPlayY = reg.Read("PlayY", 0);
	m_nPlayWidth = reg.Read("PlayWidth",0);
	m_nPlayHeight = reg.Read("PlayHeight",0);
	m_nPauseX = reg.Read("PauseX", 0);
	m_nPauseY = reg.Read("PauseY", 0);
	m_nPauseWidth = reg.Read("PauseWidth",0);
	m_nPauseHeight = reg.Read("PauseHeight",0);
	m_nReverseX = reg.Read("ReverseX", 0);
	m_nReverseY = reg.Read("ReverseY", 0);
	m_nReverseWidth = reg.Read("ReverseWidth",0);
	m_nReverseHeight = reg.Read("ReverseHeight",0);
	m_nForwardX = reg.Read("ForwardX", 0);
	m_nForwardY = reg.Read("ForwardY", 0);
	m_nForwardWidth = reg.Read("ForwardWidth",0);
	m_nForwardHeight = reg.Read("ForwardHeight",0);
	m_nRandomX = reg.Read("RandomX", 0);
	m_nRandomY = reg.Read("RandomY", 0);
	m_nRandomWidth = reg.Read("RandomWidth",0);
	m_nRandomHeight = reg.Read("RandomHeight",0);
	m_nShuffleX = reg.Read("ShuffleX", 0);
	m_nShuffleY = reg.Read("ShuffleY", 0);
	m_nShuffleWidth = reg.Read("ShuffleWidth",0);
	m_nShuffleHeight = reg.Read("ShuffleHeight",0);
	m_nClearX = reg.Read("ClearX", 0);
	m_nClearY = reg.Read("ClearY", 0);
	m_nClearWidth = reg.Read("ClearWidth",0);
	m_nClearHeight = reg.Read("ClearHeight",0);
	m_nLoadX = reg.Read("LoadX", 0);
	m_nLoadY = reg.Read("LoadY", 0);
	m_nLoadWidth = reg.Read("LoadWidth",0);
	m_nLoadHeight = reg.Read("LoadHeight",0);
	m_nSaveX = reg.Read("SaveX", 0);
	m_nSaveY = reg.Read("SaveY", 0);
	m_nSaveWidth = reg.Read("SaveWidth",0);
	m_nSaveHeight = reg.Read("SaveHeight",0);
	m_nVolumeX = reg.Read("VolumeX", 0);
	m_nVolumeY = reg.Read("VolumeY", 0);
	m_nVolumeWidth = reg.Read("VolumeWidth",0);
	m_nVolumeHeight = reg.Read("VolumeHeight",0);
	m_nProgressX = reg.Read("ProgressX", 0);
	m_nProgressY = reg.Read("ProgressY", 0);
	m_nProgressWidth = reg.Read("ProgressWidth",0);
	m_nProgressHeight = reg.Read("ProgressHeight",0);

	m_nBorderHorz = reg.Read("BorderHorz", 5);
	m_nBorderVert = reg.Read("BorderVert", 5);
	m_nBorderPanel = reg.Read("BorderPanel", 5);

	reg.Read("ColorOtherFg",buf,999,"255,255,255");
	m_ColorOtherFg.SetWindowText(buf);
	reg.Read("ColorOtherBg",buf,999,"0,0,0");
	m_ColorOtherBg.SetWindowText(buf);

	reg.Read("ColorOtherBtnFgHover", buf,999,"255,255,255");
	m_ColorOtherBtnFgHov.SetWindowText(buf);
	reg.Read("ColorOtherBtnFgOut", buf, 999, "200,200,200");
	m_ColorOtherBtnFgOut.SetWindowText(buf);
	reg.Read("ColorOtherBtnBgOut", buf,999, "100,100,100");
	m_ColorOtherBtnBgOut.SetWindowText(buf);
	reg.Read("ColorOtherBtnBgHover", buf,999,"150,150,150");
	m_ColorOtherBtnBgHov.SetWindowText(buf);

	unsigned long e = reg.Read("BackgroundType", BGSTRETCHED);
	if (0 == e) {
		m_BgType = BGSTRETCHED;
	} else if (1 == e) {
		m_BgType = BGTILED;
	} else if (2 == e) {
		m_BgType = BGFIXED;
	}
	IntsToCEdits();
	OnApply();

}
void CBitmapCutterDlg::StoreReg() {

	if ("" == m_sDest) return;

	CEditsToInts();

	// first save the saveto dir to the real registry
	RegistryKey regDest( HKEY_LOCAL_MACHINE, "Software\\BitmapCutter");
	regDest.Write("dest", m_sDest);


	// now save the bmp names to the saveto dir
	CString file = m_sDest + "\\SkinBmps.txt";
	RegistryKey regBmp(file);
	CString msg;
	m_FileIns.GetWindowText(msg);
	regBmp.Write("Insbmp", msg);
	m_FileOuts.GetWindowText(msg);
	regBmp.Write("Outsbmp", msg);
	m_FileHovers.GetWindowText(msg);
	regBmp.Write("Hoversbmp", msg);
	m_FileSettings.GetWindowText(msg);
	regBmp.Write("Settings", msg);
	regBmp.WriteFile();


	// finally store config into dest dir SkinDef.mbsd
	// but first read custom settins from m_FileSettings
	// then allow custom settings to be overriden by 
	// stuff below, just in case.
	
	RegistryKey regCustom(m_sFileSettings);
	if (m_sFileSettings != "") {
		regCustom.ReadFile();
	}

	file = m_sDest + "\\" + MB_SKIN_DEF;
	RegistryKey reg(file);	
		
	if (m_sFileSettings != "") {

		reg.Copy(regCustom);
	}

	


	m_DestDir.GetWindowText(msg);
	reg.Write("dest",msg);

	m_Red.GetWindowText(msg);
	if (msg == "")
		m_nRed = -1;
	else
		m_nRed = atoi(msg);
	reg.Write("Red",m_nRed);

	m_Green.GetWindowText(msg);
	if (msg == "")
		m_nGreen = -1;
	else
		m_nGreen = atoi(msg);
	reg.Write("Green",m_nGreen);

	m_Blue.GetWindowText(msg);
	if (msg == "")
		m_nBlue = -1;
	else
		m_nBlue = atoi(msg);
	reg.Write("Blue",m_nBlue);

	m_TransRedMain.GetWindowText(msg);
	if (msg == "")
		m_nTransRedMain = -1;
	else
		m_nTransRedMain = atoi(msg);
	reg.Write("TransRedMain",m_nTransRedMain);

	m_TransGreenMain.GetWindowText(msg);
	if (msg == "")
		m_nTransGreenMain = -1;
	else
		m_nTransGreenMain = atoi(msg);
	reg.Write("TransGreenMain",m_nTransGreenMain);

	m_TransBlueMain.GetWindowText(msg);
	if (msg == "")
		m_nTransBlueMain = -1;
	else
		m_nTransBlueMain = atoi(msg);
	reg.Write("TransBlueMain",m_nTransBlueMain);

	m_TransRedPanel.GetWindowText(msg);
	if (msg == "")
		m_nTransRedPanel = -1;
	else
		m_nTransRedPanel = atoi(msg);
	reg.Write("TransRedPanel",m_nTransRedPanel);

	m_TransGreenPanel.GetWindowText(msg);
	if (msg == "")
		m_nTransGreenPanel = -1;
	else
		m_nTransGreenPanel = atoi(msg);
	reg.Write("TransGreenPanel",m_nTransGreenPanel);

	m_TransBluePanel.GetWindowText(msg);
	if (msg == "")
		m_nTransBluePanel = -1;
	else
		m_nTransBluePanel = atoi(msg);
	reg.Write("TransBluePanel",m_nTransBluePanel);


	
	m_RGB = m_Other.GetCheck();
	reg.Write("RGB", m_RGB);
	
	m_LineWidth.GetWindowText(msg);
	m_nLineWidth = atoi(msg);
	reg.Write("linewidth",m_nLineWidth);

reg.Write("MenuX", m_nMenuX);
reg.Write("MenuY", m_nMenuY);
reg.Write("MenuWidth", m_nMenuWidth);
reg.Write("MenuHeight", m_nMenuHeight);

reg.Write("MusicX", m_nMusicX);
reg.Write("MusicY", m_nMusicY);
reg.Write("MusicWidth", m_nMusicWidth);
reg.Write("MusicHeight", m_nMusicHeight);

reg.Write("PicturesX", m_nPicturesX);
reg.Write("PicturesY", m_nPicturesY);
reg.Write("PicturesWidth", m_nPicturesWidth);
reg.Write("PicturesHeight", m_nPicturesHeight);

reg.Write("VideoX", m_nVideoX);
reg.Write("VideoY", m_nVideoY);
reg.Write("VideoWidth", m_nVideoWidth);
reg.Write("VideoHeight", m_nVideoHeight);

reg.Write("StopX", m_nStopX);
reg.Write("StopY", m_nStopY);
reg.Write("StopWidth", m_nStopWidth);
reg.Write("StopHeight", m_nStopHeight);
reg.Write("PlayX", m_nPlayX);
reg.Write("PlayY", m_nPlayY);
reg.Write("PlayWidth", m_nPlayWidth);
reg.Write("PlayHeight", m_nPlayHeight);
reg.Write("PauseX", m_nPauseX);
reg.Write("PauseY", m_nPauseY);
reg.Write("PauseWidth", m_nPauseWidth);
reg.Write("PauseHeight", m_nPauseHeight);
reg.Write("ReverseX", m_nReverseX);
reg.Write("ReverseY", m_nReverseY);
reg.Write("ReverseWidth", m_nReverseWidth);
reg.Write("ReverseHeight", m_nReverseHeight);
reg.Write("ForwardX", m_nForwardX);
reg.Write("ForwardY", m_nForwardY);
reg.Write("ForwardWidth", m_nForwardWidth);
reg.Write("ForwardHeight", m_nForwardHeight);
reg.Write("RandomX", m_nRandomX);
reg.Write("RandomY", m_nRandomY);
reg.Write("RandomWidth", m_nRandomWidth);
reg.Write("RandomHeight", m_nRandomHeight);
reg.Write("ShuffleX", m_nShuffleX);
reg.Write("ShuffleY", m_nShuffleY);
reg.Write("ShuffleWidth", m_nShuffleWidth);
reg.Write("ShuffleHeight", m_nShuffleHeight);
reg.Write("ClearX", m_nClearX);
reg.Write("ClearY", m_nClearY);
reg.Write("ClearWidth", m_nClearWidth);
reg.Write("ClearHeight", m_nClearHeight);
reg.Write("LoadX", m_nLoadX);
reg.Write("LoadY", m_nLoadY);
reg.Write("LoadWidth", m_nLoadWidth);
reg.Write("LoadHeight", m_nLoadHeight);
reg.Write("SaveX", m_nSaveX);
reg.Write("SaveY", m_nSaveY);
reg.Write("SaveWidth", m_nSaveWidth);
reg.Write("SaveHeight", m_nSaveHeight);
reg.Write("VolumeX", m_nVolumeX);
reg.Write("VolumeY", m_nVolumeY);
reg.Write("VolumeWidth", m_nVolumeWidth);
reg.Write("VolumeHeight", m_nVolumeHeight);
reg.Write("ProgressX", m_nProgressX);
reg.Write("ProgressY", m_nProgressY);
reg.Write("ProgressWidth", m_nProgressWidth);
reg.Write("ProgressHeight", m_nProgressHeight);
reg.Write("ControlBoxWidth", m_PicInsCRect.Width());
reg.Write("ControlBoxHeight", m_PicInsCRect.Height());
reg.Write("BorderHorz",m_nBorderHorz);
reg.Write("BorderVert",m_nBorderVert);
reg.Write("BorderPanel",m_nBorderPanel);
reg.Write("BackgroundMainType", m_BgType);
reg.Write("BackgroundPanelType", m_BgType);
reg.Write("BackgroundTypes:","0=Stretched, 1=CutAndTiled, 2=Fixed, 3=NormalTiled");

m_ColorOtherFg.GetWindowText(msg);reg.Write("ColorOtherFg",msg);
m_ColorOtherBg.GetWindowText(msg);reg.Write("ColorOtherBg",msg);
m_ColorOtherBtnFgHov.GetWindowText(msg);reg.Write("ColorOtherBtnFgHover",msg);
m_ColorOtherBtnFgOut.GetWindowText(msg);reg.Write("ColorOtherBtnFgOut",msg);
m_ColorOtherBtnBgOut.GetWindowText(msg);reg.Write("ColorOtherBtnBgOut",msg);
m_ColorOtherBtnBgHov.GetWindowText(msg);reg.Write("ColorOtherBtnBgHover",msg);

reg.WriteFile();

}
void CBitmapCutterDlg::IntsToCEdits() {

m_MenuX.SetWindowText(numToString(m_nMenuX));
m_MenuY.SetWindowText(numToString(m_nMenuY));
m_MenuWidth.SetWindowText(numToString(m_nMenuWidth));
m_MenuHeight.SetWindowText(numToString(m_nMenuHeight));

m_MusicX.SetWindowText(numToString(m_nMusicX));
m_MusicY.SetWindowText(numToString(m_nMusicY));
m_MusicWidth.SetWindowText(numToString(m_nMusicWidth));
m_MusicHeight.SetWindowText(numToString(m_nMusicHeight));

m_PicturesX.SetWindowText(numToString(m_nPicturesX));
m_PicturesY.SetWindowText(numToString(m_nPicturesY));
m_PicturesWidth.SetWindowText(numToString(m_nPicturesWidth));
m_PicturesHeight.SetWindowText(numToString(m_nPicturesHeight));

m_VideoX.SetWindowText(numToString(m_nVideoX));
m_VideoY.SetWindowText(numToString(m_nVideoY));
m_VideoWidth.SetWindowText(numToString(m_nVideoWidth));
m_VideoHeight.SetWindowText(numToString(m_nVideoHeight));

m_StopX.SetWindowText(numToString(m_nStopX));
m_StopY.SetWindowText(numToString(m_nStopY));
m_StopWidth.SetWindowText(numToString(m_nStopWidth));
m_StopHeight.SetWindowText(numToString(m_nStopHeight));
m_PlayX.SetWindowText(numToString(m_nPlayX));
m_PlayY.SetWindowText(numToString(m_nPlayY));
m_PlayWidth.SetWindowText(numToString(m_nPlayWidth));
m_PlayHeight.SetWindowText(numToString(m_nPlayHeight));
m_PauseX.SetWindowText(numToString(m_nPauseX));
m_PauseY.SetWindowText(numToString(m_nPauseY));
m_PauseWidth.SetWindowText(numToString(m_nPauseWidth));
m_PauseHeight.SetWindowText(numToString(m_nPauseHeight));
m_ReverseX.SetWindowText(numToString(m_nReverseX));
m_ReverseY.SetWindowText(numToString(m_nReverseY));
m_ReverseWidth.SetWindowText(numToString(m_nReverseWidth));
m_ReverseHeight.SetWindowText(numToString(m_nReverseHeight));
m_ForwardX.SetWindowText(numToString(m_nForwardX));
m_ForwardY.SetWindowText(numToString(m_nForwardY));
m_ForwardWidth.SetWindowText(numToString(m_nForwardWidth));
m_ForwardHeight.SetWindowText(numToString(m_nForwardHeight));
m_RandomX.SetWindowText(numToString(m_nRandomX));
m_RandomY.SetWindowText(numToString(m_nRandomY));
m_RandomWidth.SetWindowText(numToString(m_nRandomWidth));
m_RandomHeight.SetWindowText(numToString(m_nRandomHeight));
m_ShuffleX.SetWindowText(numToString(m_nShuffleX));
m_ShuffleY.SetWindowText(numToString(m_nShuffleY));
m_ShuffleWidth.SetWindowText(numToString(m_nShuffleWidth));
m_ShuffleHeight.SetWindowText(numToString(m_nShuffleHeight));
m_ClearX.SetWindowText(numToString(m_nClearX));
m_ClearY.SetWindowText(numToString(m_nClearY));
m_ClearWidth.SetWindowText(numToString(m_nClearWidth));
m_ClearHeight.SetWindowText(numToString(m_nClearHeight));
m_LoadX.SetWindowText(numToString(m_nLoadX));
m_LoadY.SetWindowText(numToString(m_nLoadY));
m_LoadWidth.SetWindowText(numToString(m_nLoadWidth));
m_LoadHeight.SetWindowText(numToString(m_nLoadHeight));
m_SaveX.SetWindowText(numToString(m_nSaveX));
m_SaveY.SetWindowText(numToString(m_nSaveY));
m_SaveWidth.SetWindowText(numToString(m_nSaveWidth));
m_SaveHeight.SetWindowText(numToString(m_nSaveHeight));
m_VolumeX.SetWindowText(numToString(m_nVolumeX));
m_VolumeY.SetWindowText(numToString(m_nVolumeY));
m_VolumeWidth.SetWindowText(numToString(m_nVolumeWidth));
m_VolumeHeight.SetWindowText(numToString(m_nVolumeHeight));
m_ProgressX.SetWindowText(numToString(m_nProgressX));
m_ProgressY.SetWindowText(numToString(m_nProgressY));
m_ProgressWidth.SetWindowText(numToString(m_nProgressWidth));
m_ProgressHeight.SetWindowText(numToString(m_nProgressHeight));

}
void CBitmapCutterDlg::CEditsToInts() {
	CString msg;

m_MenuX.GetWindowText(msg);m_nMenuX=atoi(msg);
m_MenuY.GetWindowText(msg);m_nMenuY=atoi(msg);
m_MenuWidth.GetWindowText(msg);m_nMenuWidth=atoi(msg);
m_MenuHeight.GetWindowText(msg);m_nMenuHeight=atoi(msg);

m_MusicX.GetWindowText(msg);m_nMusicX=atoi(msg);
m_MusicY.GetWindowText(msg);m_nMusicY=atoi(msg);
m_MusicWidth.GetWindowText(msg);m_nMusicWidth=atoi(msg);
m_MusicHeight.GetWindowText(msg);m_nMusicHeight=atoi(msg);

m_PicturesX.GetWindowText(msg);m_nPicturesX=atoi(msg);
m_PicturesY.GetWindowText(msg);m_nPicturesY=atoi(msg);
m_PicturesWidth.GetWindowText(msg);m_nPicturesWidth=atoi(msg);
m_PicturesHeight.GetWindowText(msg);m_nPicturesHeight=atoi(msg);

m_VideoX.GetWindowText(msg);m_nVideoX=atoi(msg);
m_VideoY.GetWindowText(msg);m_nVideoY=atoi(msg);
m_VideoWidth.GetWindowText(msg);m_nVideoWidth=atoi(msg);
m_VideoHeight.GetWindowText(msg);m_nVideoHeight=atoi(msg);

m_StopX.GetWindowText(msg);m_nStopX=atoi(msg);
m_StopY.GetWindowText(msg);m_nStopY=atoi(msg);
m_StopWidth.GetWindowText(msg);m_nStopWidth=atoi(msg);
m_StopHeight.GetWindowText(msg);m_nStopHeight=atoi(msg);
m_PlayX.GetWindowText(msg);m_nPlayX=atoi(msg);
m_PlayY.GetWindowText(msg);m_nPlayY=atoi(msg);
m_PlayWidth.GetWindowText(msg);m_nPlayWidth=atoi(msg);
m_PlayHeight.GetWindowText(msg);m_nPlayHeight=atoi(msg);
m_PauseX.GetWindowText(msg);m_nPauseX=atoi(msg);
m_PauseY.GetWindowText(msg);m_nPauseY=atoi(msg);
m_PauseWidth.GetWindowText(msg);m_nPauseWidth=atoi(msg);
m_PauseHeight.GetWindowText(msg);m_nPauseHeight=atoi(msg);
m_ReverseX.GetWindowText(msg);m_nReverseX=atoi(msg);
m_ReverseY.GetWindowText(msg);m_nReverseY=atoi(msg);
m_ReverseWidth.GetWindowText(msg);m_nReverseWidth=atoi(msg);
m_ReverseHeight.GetWindowText(msg);m_nReverseHeight=atoi(msg);
m_ForwardX.GetWindowText(msg);m_nForwardX=atoi(msg);
m_ForwardY.GetWindowText(msg);m_nForwardY=atoi(msg);
m_ForwardWidth.GetWindowText(msg);m_nForwardWidth=atoi(msg);
m_ForwardHeight.GetWindowText(msg);m_nForwardHeight=atoi(msg);
m_RandomX.GetWindowText(msg);m_nRandomX=atoi(msg);
m_RandomY.GetWindowText(msg);m_nRandomY=atoi(msg);
m_RandomWidth.GetWindowText(msg);m_nRandomWidth=atoi(msg);
m_RandomHeight.GetWindowText(msg);m_nRandomHeight=atoi(msg);
m_ShuffleX.GetWindowText(msg);m_nShuffleX=atoi(msg);
m_ShuffleY.GetWindowText(msg);m_nShuffleY=atoi(msg);
m_ShuffleWidth.GetWindowText(msg);m_nShuffleWidth=atoi(msg);
m_ShuffleHeight.GetWindowText(msg);m_nShuffleHeight=atoi(msg);
m_ClearX.GetWindowText(msg);m_nClearX=atoi(msg);
m_ClearY.GetWindowText(msg);m_nClearY=atoi(msg);
m_ClearWidth.GetWindowText(msg);m_nClearWidth=atoi(msg);
m_ClearHeight.GetWindowText(msg);m_nClearHeight=atoi(msg);
m_LoadX.GetWindowText(msg);m_nLoadX=atoi(msg);
m_LoadY.GetWindowText(msg);m_nLoadY=atoi(msg);
m_LoadWidth.GetWindowText(msg);m_nLoadWidth=atoi(msg);
m_LoadHeight.GetWindowText(msg);m_nLoadHeight=atoi(msg);
m_SaveX.GetWindowText(msg);m_nSaveX=atoi(msg);
m_SaveY.GetWindowText(msg);m_nSaveY=atoi(msg);
m_SaveWidth.GetWindowText(msg);m_nSaveWidth=atoi(msg);
m_SaveHeight.GetWindowText(msg);m_nSaveHeight=atoi(msg);
m_VolumeX.GetWindowText(msg);m_nVolumeX=atoi(msg);
m_VolumeY.GetWindowText(msg);m_nVolumeY=atoi(msg);
m_VolumeWidth.GetWindowText(msg);m_nVolumeWidth=atoi(msg);
m_VolumeHeight.GetWindowText(msg);m_nVolumeHeight=atoi(msg);
m_ProgressX.GetWindowText(msg);m_nProgressX=atoi(msg);
m_ProgressY.GetWindowText(msg);m_nProgressY=atoi(msg);
m_ProgressWidth.GetWindowText(msg);m_nProgressWidth=atoi(msg);
m_ProgressHeight.GetWindowText(msg);m_nProgressHeight=atoi(msg);


}
void CBitmapCutterDlg::RectToInts() {
	m_nMenuX = Menu.left;
	m_nMenuY = Menu.top;
	m_nMenuWidth = Menu.Width()+1;
	m_nMenuHeight = Menu.Height()+1;

	m_nMusicX = Music.left;
	m_nMusicY = Music.top;
	m_nMusicWidth = Music.Width()+1;
	m_nMusicHeight = Music.Height()+1;

	m_nPicturesX = Pictures.left;
	m_nPicturesY = Pictures.top;
	m_nPicturesWidth = Pictures.Width()+1;
	m_nPicturesHeight = Pictures.Height()+1;

	m_nVideoX = Video.left;
	m_nVideoY = Video.top;
	m_nVideoWidth = Video.Width()+1;
	m_nVideoHeight = Video.Height()+1;

	m_nStopX = Stop.left;
	m_nStopY = Stop.top;
	m_nStopWidth = Stop.Width()+1;
	m_nStopHeight = Stop.Height()+1;
	
	m_nPlayX = Play.left;
	m_nPlayY = Play.top;
	m_nPlayWidth = Play.Width()+1;
	m_nPlayHeight = Play.Height()+1;
	
	m_nPauseX = Pause.left;
	m_nPauseY = Pause.top;
	m_nPauseWidth = Pause.Width()+1;
	m_nPauseHeight = Pause.Height()+1;
	
	m_nReverseX = Reverse.left;
	m_nReverseY = Reverse.top;
	m_nReverseWidth = Reverse.Width()+1;
	m_nReverseHeight = Reverse.Height()+1;
	
	m_nForwardX = Forward.left;
	m_nForwardY = Forward.top;
	m_nForwardWidth = Forward.Width()+1;
	m_nForwardHeight = Forward.Height()+1;
	
	m_nRandomX = Random.left;
	m_nRandomY = Random.top;
	m_nRandomWidth = Random.Width()+1;
	m_nRandomHeight = Random.Height()+1;
	
	m_nShuffleX = Shuffle.left;
	m_nShuffleY = Shuffle.top;
	m_nShuffleWidth = Shuffle.Width()+1;
	m_nShuffleHeight = Shuffle.Height()+1;
	
	m_nClearX = Clear.left;
	m_nClearY = Clear.top;
	m_nClearWidth = Clear.Width()+1;
	m_nClearHeight = Clear.Height()+1;
	
	m_nLoadX = Load.left;
	m_nLoadY = Load.top;
	m_nLoadWidth = Load.Width()+1;
	m_nLoadHeight = Load.Height()+1;
	
	m_nSaveX = Save.left;
	m_nSaveY = Save.top;
	m_nSaveWidth = Save.Width()+1;
	m_nSaveHeight = Save.Height()+1;
	
	m_nVolumeX = Volume.left;
	m_nVolumeY = Volume.top;
	m_nVolumeWidth = Volume.Width()+1;
	m_nVolumeHeight = Volume.Height()+1;
	
	m_nProgressX = Progress.left;
	m_nProgressY = Progress.top;
	m_nProgressWidth = Progress.Width()+1;
	m_nProgressHeight = Progress.Height()+1;
}
void CBitmapCutterDlg::OnDrawsave() 
{

	CDC *cdc = GetDC();
	HDC hDC = cdc->m_hDC;
	HDC hdcBmp;
	HBITMAP hBitmap;
	HBITMAP hOldBitmap;
	HBITMAP hOldBitmap2;
	m_DrawSliderRects = TRUE;
	m_DrawSliderChannel = TRUE;
	CString msg ;

	if (m_Ins.GetCheck()) {
		CDC dcTmp;
		dcTmp.CreateCompatibleDC(NULL);
		hdcBmp = ::CreateCompatibleDC(NULL);
		hOldBitmap2 = (HBITMAP)::SelectObject(hdcBmp, m_hBitmapIns);

		hBitmap = ::CreateCompatibleBitmap(hDC,m_cdibIns.GetWidth(),m_cdibIns.GetHeight());
		hOldBitmap = (HBITMAP)::SelectObject(dcTmp.m_hDC,hBitmap);
		::BitBlt(dcTmp.m_hDC,0,0,m_cdibIns.GetWidth(),m_cdibIns.GetHeight(),
			hdcBmp,0,0,SRCCOPY);
		// drawlines Ins
		m_STCFlag = FALSE;
		CRect rect(0,0,m_cdibIns.GetWidth(),m_cdibIns.GetHeight());
		drawLines(&dcTmp,rect,rect, 0, m_cdibIns);
		::SelectObject(dcTmp.m_hDC,hOldBitmap);
		::DeleteDC(dcTmp.m_hDC);
		::SelectObject(hdcBmp, hOldBitmap2);
		::DeleteDC(hdcBmp);
		CString file = m_sDest;
		file += "\\" + FileUtil::basename(m_sFileIns);
		FExtension ext(file);
		file = ext.filename() + "WithLinesDrawn." + ext.ext();
		FileUtil::BmpSave(hBitmap, file);
		::DeleteObject(hBitmap);	
		msg = file + " saved.\r\n";
	}
	if (m_Outs.GetCheck()) {
		CDC dcTmp;
		dcTmp.CreateCompatibleDC(NULL);
		hdcBmp = ::CreateCompatibleDC(NULL);
		hOldBitmap2 = (HBITMAP)::SelectObject(hdcBmp, m_hBitmapOuts);

		hBitmap = ::CreateCompatibleBitmap(hDC,m_cdibOuts.GetWidth(),m_cdibOuts.GetHeight());
		hOldBitmap = (HBITMAP)::SelectObject(dcTmp.m_hDC,hBitmap);
		::BitBlt(dcTmp.m_hDC,0,0,m_cdibOuts.GetWidth(),m_cdibOuts.GetHeight(),
			hdcBmp,0,0,SRCCOPY);
		// drawlines Outs
		m_STCFlag = FALSE;
		CRect rect(0,0,m_cdibOuts.GetWidth(),m_cdibOuts.GetHeight());
		drawLines(&dcTmp,rect,rect, 1, m_cdibOuts);
		::SelectObject(dcTmp.m_hDC,hOldBitmap);
		::DeleteDC(dcTmp.m_hDC);
		::SelectObject(hdcBmp, hOldBitmap2);
		::DeleteDC(hdcBmp);
		CString file = m_sDest;
		file += "\\" + FileUtil::basename(m_sFileOuts);
		FExtension ext(file);
		file = ext.filename() + "WithLinesDrawn." + ext.ext();
		FileUtil::BmpSave(hBitmap, file);
		::DeleteObject(hBitmap);	
		msg += file + " saved.\r\n";
	}

	if (m_Hovers.GetCheck()) {
		CDC dcTmp;
		dcTmp.CreateCompatibleDC(NULL);
		hdcBmp = ::CreateCompatibleDC(NULL);
		hOldBitmap2 = (HBITMAP)::SelectObject(hdcBmp, m_hBitmapHovers);

		hBitmap = ::CreateCompatibleBitmap(hDC,m_cdibHovers.GetWidth(),m_cdibHovers.GetHeight());
		hOldBitmap = (HBITMAP)::SelectObject(dcTmp.m_hDC,hBitmap);
		::BitBlt(dcTmp.m_hDC,0,0,m_cdibHovers.GetWidth(),m_cdibHovers.GetHeight(),
			hdcBmp,0,0,SRCCOPY);
		// drawlines Hovers
		m_STCFlag = FALSE;
		CRect rect(0,0,m_cdibHovers.GetWidth(),m_cdibHovers.GetHeight());
		drawLines(&dcTmp,rect,rect, 1, m_cdibHovers);
		::SelectObject(dcTmp.m_hDC,hOldBitmap);
		::DeleteDC(dcTmp.m_hDC);
		::SelectObject(hdcBmp, hOldBitmap2);
		::DeleteDC(hdcBmp);
		CString file = m_sDest;
		file += "\\" + FileUtil::basename(m_sFileHovers);
		FExtension ext(file);
		file = ext.filename() + "WithLinesDrawn." + ext.ext();
		FileUtil::BmpSave(hBitmap, file);
		::DeleteObject(hBitmap);	
		msg += file + " saved.\r\n";
	}


	ReleaseDC(cdc);	

	StoreReg();
	MessageBox(  msg, _T("BitmapCutter"), MB_OK | MB_ICONINFORMATION );

}


void CBitmapCutterDlg::OnDrawLines() 
{
	StoreReg();
	m_DrawLines = TRUE;
	EnableDisable();
	Invalidate();
	
}

void CBitmapCutterDlg::OnReread() 
{
	m_ReRead = TRUE;
	m_DrawLines = FALSE;
	Init=TRUE;
	OnButtonFileIns() ;
	OnButtonFileOuts() ;
	OnButtonFileHovers() ;
	Init=FALSE;
	
}

void CBitmapCutterDlg::OnRadioBw() 
{
	m_Red.EnableWindow(FALSE);	
	m_Green.EnableWindow(FALSE);
	m_Blue.EnableWindow(FALSE);
	m_RGB = FALSE;
	m_Other.SetCheck(FALSE);
}

void CBitmapCutterDlg::OnRadioOther() 
{
	m_Red.EnableWindow(TRUE);	
	m_Green.EnableWindow(TRUE);
	m_Blue.EnableWindow(TRUE);
	m_RGB = TRUE;
	m_BlackWhite.SetCheck(FALSE);
	
}


void CBitmapCutterDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	
//	CDialog::OnLButtonDown(nFlags, point);
	m_ButtonDrawn = FALSE;

//xxx ins 2 outs
	if (m_PicOutsDlgRect.PtInRect(point)) {
		m_StartPoint = point;
		m_Dragging = TRUE;
		m_NeedButtonAssignment = FALSE;
		m_PicOuts.SetFocus();
	}
	CRect rect;
	if (m_NeedButtonAssignment) {
//xxx ins 2 outs
		m_StartPoint.x -= m_PicOutsDlgRect.left;
		m_StartPoint.y -= m_PicOutsDlgRect.top;
		m_EndPoint.x -= m_PicOutsDlgRect.left;
		m_EndPoint.y -= m_PicOutsDlgRect.top;
		rect = CRect(m_StartPoint,m_EndPoint);
		rect.NormalizeRect();
	}

	CPoint cpoint(point);
//xxx ins 2 outs
	cpoint.x -= m_PicOutsDlgRect.left;
	cpoint.y -= m_PicOutsDlgRect.top;
	if (Menu.PtInRect(cpoint)) {
		m_AdjustingRect = &Menu;
	} else if (Music.PtInRect(cpoint)) {
		m_AdjustingRect = &Music;
	} else if (Pictures.PtInRect(cpoint)) {
		m_AdjustingRect = &Pictures;
	} else if (Video.PtInRect(cpoint)) {
		m_AdjustingRect = &Video;
	} else if (Stop.PtInRect(cpoint)) {
		m_AdjustingRect = &Stop;
	} else if (Play.PtInRect(cpoint)) {
		m_AdjustingRect = &Play;
	} else if (Pause.PtInRect(cpoint)) {
		m_AdjustingRect = &Pause;
	} else if (Reverse.PtInRect(cpoint)) {
		m_AdjustingRect = &Reverse;
	} else if (Forward.PtInRect(cpoint)) {
		m_AdjustingRect = &Forward;
	} else if (Random.PtInRect(cpoint)) {
		m_AdjustingRect = &Random;
	} else if (Shuffle.PtInRect(cpoint)) {
		m_AdjustingRect = &Shuffle;
	} else if (Clear.PtInRect(cpoint)) {
		m_AdjustingRect = &Clear;
	} else if (Load.PtInRect(cpoint)) {
		m_AdjustingRect = &Load;
	} else if (Save.PtInRect(cpoint)) {
		m_AdjustingRect = &Save;
	} else if (Volume.PtInRect(cpoint)) {
		m_AdjustingRect = &Volume;
	} else if (Progress.PtInRect(cpoint)) {
		m_AdjustingRect = &Progress;
	}

	if (MenuLabel.PtInRect(point)) {
		Highlight(Menu);
		if (FALSE == m_NeedButtonAssignment) 
			return;
		Menu = rect;
		m_nMenuX = Menu.left;
		m_nMenuY = Menu.top;
		m_nMenuWidth = Menu.Width();
		m_nMenuHeight = Menu.Height();
	}
	if (MusicLabel.PtInRect(point)) {
		Highlight(Music);
		if (FALSE == m_NeedButtonAssignment) 
			return;
		Music = rect;
		m_nMusicX = Music.left;
		m_nMusicY = Music.top;
		m_nMusicWidth = Music.Width();
		m_nMusicHeight = Music.Height();
	}
	if (PicturesLabel.PtInRect(point)) {
		Highlight(Pictures);
		if (FALSE == m_NeedButtonAssignment) 
			return;
		Pictures = rect;
		m_nPicturesX = Pictures.left;
		m_nPicturesY = Pictures.top;
		m_nPicturesWidth = Pictures.Width();
		m_nPicturesHeight = Pictures.Height();
	}
	if (VideoLabel.PtInRect(point)) {
		Highlight(Video);
		if (FALSE == m_NeedButtonAssignment) 
			return;
		Video = rect;
		m_nVideoX = Video.left;
		m_nVideoY = Video.top;
		m_nVideoWidth = Video.Width();
		m_nVideoHeight = Video.Height();
	}


	if (StopLabel.PtInRect(point)) {
		Highlight(Stop);
		if (FALSE == m_NeedButtonAssignment) 
			return;
		Stop = rect;
		m_nStopX = Stop.left;
		m_nStopY = Stop.top;
		m_nStopWidth = Stop.Width();
		m_nStopHeight = Stop.Height();
	}

	if (PlayLabel.PtInRect(point)) {
		Highlight(Play);if (FALSE == m_NeedButtonAssignment) return;
		Play = rect;
		m_nPlayX = Play.left;
		m_nPlayY = Play.top;
		m_nPlayWidth = Play.Width();
		m_nPlayHeight = Play.Height();
	}
	if (PauseLabel.PtInRect(point)) {
		Highlight(Pause);if (FALSE == m_NeedButtonAssignment) return;
		Pause = rect;
		m_nPauseX = Pause.left;
		m_nPauseY = Pause.top;
		m_nPauseWidth = Pause.Width();
		m_nPauseHeight = Pause.Height();
	}
	if (ReverseLabel.PtInRect(point)) {
		Highlight(Reverse);if (FALSE == m_NeedButtonAssignment) return;
		Reverse = rect;
		m_nReverseX = Reverse.left;
		m_nReverseY = Reverse.top;
		m_nReverseWidth = Reverse.Width();
		m_nReverseHeight = Reverse.Height();
	}
	if (ForwardLabel.PtInRect(point)) {
		Highlight(Forward);if (FALSE == m_NeedButtonAssignment) return;
		Forward = rect;
		m_nForwardX = Forward.left;
		m_nForwardY = Forward.top;
		m_nForwardWidth = Forward.Width();
		m_nForwardHeight = Forward.Height();
	}
	if (RandomLabel.PtInRect(point)) {
		Highlight(Random);if (FALSE == m_NeedButtonAssignment) return;
		Random = rect;
		m_nRandomX = Random.left;
		m_nRandomY = Random.top;
		m_nRandomWidth = Random.Width();
		m_nRandomHeight = Random.Height();
	}
	if (ShuffleLabel.PtInRect(point)) {
		Highlight(Shuffle);if (FALSE == m_NeedButtonAssignment) return;
		Shuffle = rect;
		m_nShuffleX = Shuffle.left;
		m_nShuffleY = Shuffle.top;
		m_nShuffleWidth = Shuffle.Width();
		m_nShuffleHeight = Shuffle.Height();
	}
	if (ClearLabel.PtInRect(point)) {
		Highlight(Clear);if (FALSE == m_NeedButtonAssignment) return;
		Clear = rect;
		m_nClearX = Clear.left;
		m_nClearY = Clear.top;
		m_nClearWidth = Clear.Width();
		m_nClearHeight = Clear.Height();
	}
	if (LoadLabel.PtInRect(point)) {
		Highlight(Load);if (FALSE == m_NeedButtonAssignment) return;
		Load = rect;
		m_nLoadX = Load.left;
		m_nLoadY = Load.top;
		m_nLoadWidth = Load.Width();
		m_nLoadHeight = Load.Height();
	}
	if (SaveLabel.PtInRect(point)) {
		Highlight(Save);if (FALSE == m_NeedButtonAssignment) return;
		Save = rect;
		m_nSaveX = Save.left;
		m_nSaveY = Save.top;
		m_nSaveWidth = Save.Width();
		m_nSaveHeight = Save.Height();
	}
	if (VolumeLabel.PtInRect(point)) {
		Highlight(Volume);if (FALSE == m_NeedButtonAssignment) return;
		Volume = rect;
		m_nVolumeX = Volume.left;
		m_nVolumeY = Volume.top;
		m_nVolumeWidth = Volume.Width();
		m_nVolumeHeight = Volume.Height();
	}
	if (ProgressLabel.PtInRect(point)) {
		Highlight(Progress);if (FALSE == m_NeedButtonAssignment) return;
		Progress = rect;
		m_nProgressX = Progress.left;
		m_nProgressY = Progress.top;
		m_nProgressWidth = Progress.Width();
		m_nProgressHeight = Progress.Height();
	}
	m_NeedButtonAssignment = FALSE;

	IntsToCEdits();
	OnDrawLines();
	Invalidate();
}
void CBitmapCutterDlg::Highlight(CRect rect) {
	CDC * dc = GetDC();
	CBrush br;
	br.CreateSolidBrush(RGB(255,255,255));
	CRect wrect;
//xxx ins 2 outs
	m_PicOuts.GetWindowRect(wrect);
	ScreenToClient(wrect);
	CRect r;
	r.left = wrect.left + rect.left;
	r.top = wrect.top + rect.top;
	r.right = r.left + rect.Width();
	r.bottom = r.top + rect.Height();
	dc->FillRect(r,&br);
	ReleaseDC(dc);
}

void CBitmapCutterDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	//CDialog::OnLButtonUp(nFlags, point);
	m_Dragging = FALSE;
	if (m_ButtonDrawn)
		m_NeedButtonAssignment = TRUE;
}

void CBitmapCutterDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	if (m_Dragging == FALSE) {
		CDialog::OnMouseMove(nFlags, point);
		return;
	}
	if (!m_PicOutsDlgRect.PtInRect(point)) {
		point.x = __min(point.x,m_PicOutsDlgRect.right);
		point.x = __max(point.x,m_PicOutsDlgRect.left);
		point.y = __min(point.y,m_PicOutsDlgRect.bottom);
		point.y = __max(point.y,m_PicOutsDlgRect.top);
	}

	m_EndPoint = point;
	CRect rect (m_StartPoint,m_EndPoint);
	CBrush br;
	br.CreateSolidBrush(RGB(255,255,255));

	CDC * dc = GetDC();
	drawIt(dc);
	rect.NormalizeRect();
	dc->FrameRect(rect, &br);
	m_ButtonDrawn = TRUE;
}





void CBitmapCutterDlg::OnZero() 
{
	m_nMenuX = 0;
	m_nMenuY = 0;
	m_nMenuWidth = 0;
	m_nMenuHeight = 0;

	m_nMusicX = 0;
	m_nMusicY = 0;
	m_nMusicWidth = 0;
	m_nMusicHeight = 0;

	m_nPicturesX = 0;
	m_nPicturesY = 0;
	m_nPicturesWidth = 0;
	m_nPicturesHeight = 0;

	m_nVideoX = 0;
	m_nVideoY = 0;
	m_nVideoWidth = 0;
	m_nVideoHeight = 0;

	m_nStopX = 0;
	m_nStopY = 0;
	m_nStopWidth = 0;
	m_nStopHeight = 0;
	m_nPlayX = 0;
	m_nPlayY = 0;
	m_nPlayWidth = 0;
	m_nPlayHeight = 0;
	m_nPauseX = 0;
	m_nPauseY = 0;
	m_nPauseWidth = 0;
	m_nPauseHeight = 0;
	m_nReverseX = 0;
	m_nReverseY = 0;
	m_nReverseWidth = 0;
	m_nReverseHeight = 0;
	m_nForwardX = 0;
	m_nForwardY = 0;
	m_nForwardWidth = 0;
	m_nForwardHeight = 0;
	m_nRandomX = 0;
	m_nRandomY = 0;
	m_nRandomWidth = 0;
	m_nRandomHeight = 0;
	m_nShuffleX = 0;
	m_nShuffleY = 0;
	m_nShuffleWidth = 0;
	m_nShuffleHeight = 0;
	m_nClearX = 0;
	m_nClearY = 0;
	m_nClearWidth = 0;
	m_nClearHeight = 0;
	m_nLoadX = 0;
	m_nLoadY = 0;
	m_nLoadWidth = 0;
	m_nLoadHeight = 0;
	m_nSaveX = 0;
	m_nSaveY = 0;
	m_nSaveWidth = 0;
	m_nSaveHeight = 0;
	m_nVolumeX = 0;
	m_nVolumeY = 0;
	m_nVolumeWidth = 0;
	m_nVolumeHeight = 0;
	m_nProgressX = 0;
	m_nProgressY = 0;
	m_nProgressWidth = 0;
	m_nProgressHeight = 0;
	IntsToCEdits();
	Invalidate();
	
}

void CBitmapCutterDlg::OnRemoveDraw() 
{
	m_DrawLines = FALSE;
	Invalidate();
	
}
void CBitmapCutterDlg::CreateTest() {

	if (m_sFileIns == ""
		|| m_sFileOuts == ""
		|| m_sFileHovers == "") {
		return;
	}

	

	COLORREF transclr = RGB(255,0,0);

	m_TMenuButton.SetBitmaps(
		m_sDest + "\\" + MB_SKIN_BUTTONMENUIN,	transclr,	
		m_sDest + "\\" + MB_SKIN_BUTTONMENUHOVER,	transclr,	
		m_sDest + "\\" + MB_SKIN_BUTTONMENUOUT,		transclr);

	m_TMusicButton.SetBitmaps(
		m_sDest + "\\" + MB_SKIN_BUTTONMUSICIN,	transclr,	
		m_sDest + "\\" + MB_SKIN_BUTTONMUSICHOVER,	transclr,	
		m_sDest + "\\" + MB_SKIN_BUTTONMUSICOUT,		transclr);

	m_TPicturesButton.SetBitmaps(
		m_sDest + "\\" + MB_SKIN_BUTTONPICTURESIN,	transclr,	
		m_sDest + "\\" + MB_SKIN_BUTTONPICTURESHOVER,	transclr,	
		m_sDest + "\\" + MB_SKIN_BUTTONPICTURESOUT,		transclr);

	m_TVideoButton.SetBitmaps(
		m_sDest + "\\" + MB_SKIN_BUTTONVIDEOIN,	transclr,	
		m_sDest + "\\" + MB_SKIN_BUTTONVIDEOHOVER,	transclr,	
		m_sDest + "\\" + MB_SKIN_BUTTONVIDEOOUT,		transclr);

	m_TStopButton.SetBitmaps(
		m_sDest + "\\" + MB_SKIN_BUTTONSTOPIN,	transclr,	
		m_sDest + "\\" + MB_SKIN_BUTTONSTOPHOVER,	transclr,	
		m_sDest + "\\" + MB_SKIN_BUTTONSTOPOUT,		transclr);

	m_TPlayButton.SetBitmaps(
		m_sDest + "\\" + MB_SKIN_BUTTONPLAYIN,	transclr,	
		m_sDest + "\\" + MB_SKIN_BUTTONPLAYHOVER,	transclr,	
		m_sDest + "\\" + MB_SKIN_BUTTONPLAYOUT,		transclr);

	m_TPauseButton.SetBitmaps(
		m_sDest + "\\" + MB_SKIN_BUTTONPAUSEIN,	transclr,	
		m_sDest + "\\" + MB_SKIN_BUTTONPAUSEHOVER,	transclr,	
		m_sDest + "\\" + MB_SKIN_BUTTONPAUSEOUT,		transclr);

	m_TShuffleButton.SetBitmaps(
		m_sDest + "\\" + MB_SKIN_BUTTONSHUFFLEIN, transclr, 
		m_sDest + "\\" + MB_SKIN_BUTTONSHUFFLEHOVER, transclr, 
		m_sDest + "\\" + MB_SKIN_BUTTONSHUFFLEOUT, transclr);

	m_TSaveButton.SetBitmaps(
		m_sDest + "\\" + MB_SKIN_BUTTONSAVEIN, transclr, 
		m_sDest + "\\" + MB_SKIN_BUTTONSAVEHOVER, transclr, 
		m_sDest + "\\" + MB_SKIN_BUTTONSAVEOUT, transclr);

	m_TReverseButton.SetBitmaps(
		m_sDest + "\\" + MB_SKIN_BUTTONREVERSEIN, transclr, 
		m_sDest + "\\" + MB_SKIN_BUTTONREVERSEHOVER, transclr, 
		m_sDest + "\\" + MB_SKIN_BUTTONREVERSEOUT, transclr);

	m_TRandomButton.SetBitmaps(
		m_sDest + "\\" + MB_SKIN_BUTTONRANDOMIN, transclr, 
		m_sDest + "\\" + MB_SKIN_BUTTONRANDOMHOVER, transclr, 
		m_sDest + "\\" + MB_SKIN_BUTTONRANDOMOUT, transclr);

	m_TLoadButton.SetBitmaps(
		m_sDest + "\\" + MB_SKIN_BUTTONLOADIN, transclr, 
		m_sDest + "\\" + MB_SKIN_BUTTONLOADHOVER, transclr, 
		m_sDest + "\\" + MB_SKIN_BUTTONLOADOUT, transclr);

	m_TForwardButton.SetBitmaps(
		m_sDest + "\\" + MB_SKIN_BUTTONFASTFORWARDIN, transclr, 
		m_sDest + "\\" + MB_SKIN_BUTTONFASTFORWARDHOVER, transclr, 
		m_sDest + "\\" + MB_SKIN_BUTTONFASTFORWARDOUT, transclr);

	m_TClearButton.SetBitmaps(
		m_sDest + "\\" + MB_SKIN_BUTTONCLEARIN, transclr, 
		m_sDest + "\\" + MB_SKIN_BUTTONCLEARHOVER, transclr, 
		m_sDest + "\\" + MB_SKIN_BUTTONCLEAROUT, transclr);
	m_TVolume.SetBitmaps(
		m_sDest + "\\" + MB_SKIN_BUTTONVOLUMEBACKGROUND,
		transclr,
		m_sDest + "\\" + MB_SKIN_BUTTONVOLUMEKNOB,
		transclr
		);
	m_TProgress.SetBitmaps(
		m_sDest + "\\" + MB_SKIN_BUTTONPROGRESSBACKGROUND,
		transclr,
		m_sDest + "\\" + MB_SKIN_BUTTONPROGRESSKNOB,
		transclr
		);
	m_TProgress.SetOrientation(CMySliderCtrl::HORIZONTAL);
	m_TMenuButton.ShowWindow(SW_NORMAL);
	m_TMusicButton.ShowWindow(SW_NORMAL);
	m_TPicturesButton.ShowWindow(SW_NORMAL);
	m_TVideoButton.ShowWindow(SW_NORMAL);
	m_TStopButton.ShowWindow(SW_NORMAL);
	m_TPlayButton.ShowWindow(SW_NORMAL);
	m_TPauseButton.ShowWindow(SW_NORMAL);
	m_TReverseButton.ShowWindow(SW_NORMAL);
	m_TForwardButton.ShowWindow(SW_NORMAL);
	m_TRandomButton.ShowWindow(SW_NORMAL);
	m_TShuffleButton.ShowWindow(SW_NORMAL);
	m_TClearButton.ShowWindow(SW_NORMAL);
	m_TLoadButton.ShowWindow(SW_NORMAL);
	m_TSaveButton.ShowWindow(SW_NORMAL);
	m_TVolume.ShowWindow(SW_NORMAL);
	m_TProgress.ShowWindow(SW_NORMAL);

	CString skindef = m_sDest + "\\" + MB_SKIN_DEF;
	RegistryKey regSD(skindef);
	regSD.ReadFile();

	CRect wrect;
	m_PicTest.GetWindowRect(wrect);
	ScreenToClient(wrect);
	int ControlBoxLeft = wrect.left;
	int ControlBoxTop = wrect.top;
	int x,y,width,height;

	x = ControlBoxLeft + regSD.Read("MenuX",0);
	y = ControlBoxTop + regSD.Read("MenuY", 0);
	width = regSD.Read("MenuWidth",0);
	height = regSD.Read("MenuHeight",0);
	m_TMenuButton.MoveWindow(x, y, width, height, TRUE);

	x = ControlBoxLeft + regSD.Read("MusicX",0);
	y = ControlBoxTop + regSD.Read("MusicY", 0);
	width = regSD.Read("MusicWidth",0);
	height = regSD.Read("MusicHeight",0);
	m_TMusicButton.MoveWindow(x, y, width, height, TRUE);

	x = ControlBoxLeft + regSD.Read("PicturesX",0);
	y = ControlBoxTop + regSD.Read("PicturesY", 0);
	width = regSD.Read("PicturesWidth",0);
	height = regSD.Read("PicturesHeight",0);
	m_TPicturesButton.MoveWindow(x, y, width, height, TRUE);

	x = ControlBoxLeft + regSD.Read("VideoX",0);
	y = ControlBoxTop + regSD.Read("VideoY", 0);
	width = regSD.Read("VideoWidth",0);
	height = regSD.Read("VideoHeight",0);
	m_TVideoButton.MoveWindow(x, y, width, height, TRUE);

	x = ControlBoxLeft + regSD.Read("StopX",0);
	y = ControlBoxTop + regSD.Read("StopY", 0);
	width = regSD.Read("StopWidth",0);
	height = regSD.Read("StopHeight",0);
	m_TStopButton.MoveWindow(x, y, width, height, TRUE);

	x = ControlBoxLeft + regSD.Read("PlayX",0);
	y = ControlBoxTop + regSD.Read("PlayY", 0);
	width = regSD.Read("PlayWidth",0);
	height = regSD.Read("PlayHeight",0);
	m_TPlayButton.MoveWindow(x, y, width, height, TRUE);
	
	x = ControlBoxLeft + regSD.Read("PauseX",0);
	y = ControlBoxTop + regSD.Read("PauseY", 0);
	width = regSD.Read("PauseWidth",0);
	height = regSD.Read("PauseHeight",0);
	m_TPauseButton.MoveWindow(x, y, width, height, TRUE);
	
	x = ControlBoxLeft + regSD.Read("ReverseX",0);
	y = ControlBoxTop + regSD.Read("ReverseY", 0);
	width = regSD.Read("ReverseWidth",0);
	height = regSD.Read("ReverseHeight",0);
	m_TReverseButton.MoveWindow(x, y, width, height, TRUE);

	x = ControlBoxLeft + regSD.Read("ForwardX",0);
	y = ControlBoxTop + regSD.Read("ForwardY", 0);
	width = regSD.Read("ForwardWidth",0);
	height = regSD.Read("ForwardHeight",0);
	m_TForwardButton.MoveWindow(x, y, width, height, TRUE);
	
	x = ControlBoxLeft + regSD.Read("RandomX",0);
	y = ControlBoxTop + regSD.Read("RandomY", 0);
	width = regSD.Read("RandomWidth",0);
	height = regSD.Read("RandomHeight",0);
	m_TRandomButton.MoveWindow(x, y, width, height, TRUE);

	x = ControlBoxLeft + regSD.Read("ShuffleX",0);
	y = ControlBoxTop + regSD.Read("ShuffleY", 0);
	width = regSD.Read("ShuffleWidth",0);
	height = regSD.Read("ShuffleHeight",0);
	m_TShuffleButton.MoveWindow(x, y, width, height, TRUE);

	x = ControlBoxLeft + regSD.Read("ClearX",0);
	y = ControlBoxTop + regSD.Read("ClearY", 0);
	width = regSD.Read("ClearWidth",0);
	height = regSD.Read("ClearHeight",0);
	m_TClearButton.MoveWindow(x, y, width, height, TRUE);

	x = ControlBoxLeft + regSD.Read("LoadX",0);
	y = ControlBoxTop + regSD.Read("LoadY", 0);
	width = regSD.Read("LoadWidth",0);
	height = regSD.Read("LoadHeight",0);
	m_TLoadButton.MoveWindow(x, y, width, height, TRUE);

	x = ControlBoxLeft + regSD.Read("SaveX",0);
	y = ControlBoxTop + regSD.Read("SaveY", 0);
	width = regSD.Read("SaveWidth",0);
	height = regSD.Read("SaveHeight",0);
	m_TSaveButton.MoveWindow(x, y, width, height, TRUE);

	x = ControlBoxLeft + regSD.Read("VolumeX",0);
	y = ControlBoxTop + regSD.Read("VolumeY", 0);
	width = regSD.Read("VolumeWidth",0);
	height = regSD.Read("VolumeHeight",0);
	m_TVolume.MoveWindow(x, y, width, height, TRUE);

	x = ControlBoxLeft + regSD.Read("ProgressX",0);
	y = ControlBoxTop + regSD.Read("ProgressY", 0);
	width = regSD.Read("ProgressWidth",0);
	height = regSD.Read("ProgressHeight",0);
	m_TProgress.MoveWindow(x, y, width, height, TRUE);




}


void CBitmapCutterDlg::OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags ) {
    if (nChar == VK_LEFT) {

    } else if (nChar == VK_RIGHT) {

    } else if ((nChar == 85 || nChar == 117
					|| nChar == 68 || nChar == 100)) {

	}
}

void CBitmapCutterDlg::OnPicIns() 
{
	// TODO: Add your control notification handler code here
	
}

void CBitmapCutterDlg::OnAdjust() 
{
	if (m_Adjusting) {
		m_Adjusting = FALSE;
	} else  {
		m_Adjusting = TRUE;
		m_PicOuts.SetFocus();
	}
	
}
BOOL CBitmapCutterDlg::PreTranslateMessage(MSG* pMsg)
{	// disable ESC & ENTER from killing the dialog
    if (pMsg->message == WM_KEYDOWN) {
        if (pMsg->wParam == VK_RETURN) {
            return TRUE;
		} else if (pMsg->wParam == VK_ESCAPE) {
			return TRUE;
        } else if (pMsg->wParam == VK_DELETE) {
             return TRUE;
        }
    }
    return CDialog::PreTranslateMessage(pMsg);
}
void CBitmapCutterDlg::OnArrowKey(UINT shifted, LONG lParam)  {
	if (m_AdjustingRect == NULL) {
		return;
	}

	if (shifted) {
		if (lParam== VK_LEFT) {
			m_AdjustingRect->right--;
		} else if (lParam== VK_RIGHT) {
			m_AdjustingRect->right++;
		} else if (lParam== VK_DOWN) {
			m_AdjustingRect->bottom++;
		} else if (lParam== VK_UP) {
			m_AdjustingRect->bottom--;
		}
	} else {
		if (lParam== VK_LEFT) {
			m_AdjustingRect->left--;
			m_AdjustingRect->right--;
		} else if (lParam== VK_RIGHT) {
			m_AdjustingRect->left++;
			m_AdjustingRect->right++;
		} else if (lParam== VK_DOWN) {
			m_AdjustingRect->top++;
			m_AdjustingRect->bottom++;
		} else if (lParam== VK_UP) {
			m_AdjustingRect->top--;
			m_AdjustingRect->bottom--;
		}
	}
	RectToInts();
	IntsToCEdits();
	Invalidate();
}






void CBitmapCutterDlg::OnBgtypeStretched() 
{
	m_BgType = BGSTRETCHED;
	EnableDisable();
	
}

void CBitmapCutterDlg::OnBgtypeFixed() 
{
	m_BgType = BGFIXED;
	EnableDisable();
}

void CBitmapCutterDlg::OnBgtypeTiled() 
{
	m_BgType = BGTILED;
	EnableDisable();
	
}
