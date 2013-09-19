// ColorBtn.cpp : implementation file

#include "stdafx.h"
#include "ColorBtn.h"
#include "Registry.h"
#include "MyString.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// The color table, initialized to windows' 20 static colors

COLORREF CColorBtnDlg::colors[20] =
{
    RGB(0,0,0),
    RGB(128,0,0),
    RGB(0,128,0),
    RGB(128,128,0),
    RGB(0,0,128),
    RGB(128,0,128),
    RGB(0,128,128),
    RGB(192,192,192),
    RGB(192,220,192),
    RGB(166,202,240),
    RGB(255,251,240),
    RGB(160,160,164),
    RGB(128,128,128),
    RGB(255,0,0),
    RGB(0,255,0),
    RGB(255,255,0),
    RGB(0,0,255),
    RGB(255,0,255),
    RGB(0,255,255),
    RGB(255,255,255)
};

// MRU table. See notes for Reset()

BYTE CColorBtnDlg::used[20] =
{    
    1,3,5,7,9,11,13,15,17,19,20,18,16,14,12,10,8,6,4,2    
};

COLORREF CColorBtnDlg::custom[16] = 
{
	RGB(255,255,255),
	RGB(255,255,255),
	RGB(255,255,255),
	RGB(255,255,255),
	RGB(255,255,255),
	RGB(255,255,255),
	RGB(255,255,255),
	RGB(255,255,255),
	RGB(255,255,255),
	RGB(255,255,255),
	RGB(255,255,255),
	RGB(255,255,255),
	RGB(255,255,255),
	RGB(255,255,255),
	RGB(255,255,255),
	RGB(255,255,255)
};

/////////////////////////////////////////////////////////////////////////////
// CColorBtn

CColorBtn::CColorBtn()
{
    currentcolor = RGB(255,255,255);

    dlg.parent = this;   // This will allow the dialog to position itself

    // Create the pens and brushes that we'll need to draw the button

    nullpen.CreateStockObject(NULL_PEN);
    blackpen.CreateStockObject(BLACK_PEN);
    whitepen.CreateStockObject(WHITE_PEN);

    nullbrush.CreateStockObject(NULL_BRUSH);
    backbrush.CreateSolidBrush(GetSysColor(COLOR_3DFACE));
    dkgray.CreatePen(PS_SOLID,1,RGB(128,128,128));         
	Load();
}


CColorBtn::~CColorBtn()
{
}


BEGIN_MESSAGE_MAP(CColorBtn, CButton)
	//{{AFX_MSG_MAP(CColorBtn)	
	ON_CONTROL_REFLECT(BN_CLICKED, OnClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorBtn message handlers


void CColorBtn::DrawItem(LPDRAWITEMSTRUCT lpd)
{
    // Draw the button

    CBrush colorbrush;

    CDC DC;

    DC.Attach(lpd->hDC);
    
    int top,left,bottom,right;

    // Store this for convinience

    top    = lpd->rcItem.top;
    left   = lpd->rcItem.left;
    bottom = lpd->rcItem.bottom;
    right  = lpd->rcItem.right;
                
    colorbrush.CreateSolidBrush(currentcolor);
    
    oldpen   = DC.SelectObject(&nullpen);
    oldbrush = DC.SelectObject(&backbrush);

    // Clear the background using the 3DFACE color.

    DC.Rectangle(&lpd->rcItem);

    // Draw the border

    if (!(lpd->itemState & ODS_SELECTED))
    {
        // Button is up

        DC.SelectObject(&blackpen);

        DC.MoveTo(left,bottom-1);
        DC.LineTo(right-1,bottom-1);
        DC.LineTo(right-1,top);

        DC.SelectObject(&dkgray);

        DC.MoveTo(left+1,bottom-2);
        DC.LineTo(right-2,bottom-2);
        DC.LineTo(right-2,top+1);

        DC.SelectObject(&whitepen);

        DC.LineTo(left+1,top+1);
        DC.LineTo(left+1,bottom-2);
        
    }
    else
    {
        // Button is down

        DC.SelectObject(&dkgray);            

        DC.MoveTo(left,bottom-1);

        DC.LineTo(left,top);
        DC.LineTo(right-1,top);

        DC.SelectObject(&whitepen);

        DC.MoveTo(right-1,top-1);

        DC.LineTo(right-1,bottom-1);
        DC.LineTo(left+1,bottom-1);

        DC.SelectObject(&blackpen);

        DC.MoveTo(left+1,bottom-2);
        DC.LineTo(left+1,top+1);
        DC.LineTo(right-2,top+1);

        // by moving this, we get the things inside the button
        // to draw themselves one pixel down and one to the right.
        // This completes the "pushed" effect

        left++;
        right++;
        bottom++;
        top++;

    }

    // The division

    DC.SelectObject(&whitepen);
    
    DC.MoveTo(right-10,top+4);
    DC.LineTo(right-10,bottom-4);

    DC.SelectObject(dkgray);

    DC.MoveTo(right-11,top+4);
    DC.LineTo(right-11,bottom-4);

    // The triangle

    if (lpd->itemState & ODS_DISABLED)
        DC.SelectObject(dkgray);
    else
        DC.SelectObject(blackpen);
    
    DC.MoveTo(right-4,(bottom/2)-1);
    DC.LineTo(right-9,(bottom/2)-1);

    DC.MoveTo(right-5,(bottom/2));
    DC.LineTo(right-8,(bottom/2));

    if (lpd->itemState & ODS_DISABLED)    
    {
        DC.SetPixel(right-4,(bottom/2)-1,RGB(255,255,255));
        DC.SetPixel(right-5,(bottom/2),RGB(255,255,255));
        DC.SetPixel(right-6,(bottom/2)+1,RGB(255,255,255));
    }
    else
    {
        DC.SetPixel(right-6,(bottom/2)+1,RGB(0,0,0));
    }

    if (!(lpd->itemState & ODS_DISABLED))
    {
        // The color rectangle, only if enabled

        DC.SelectObject(&colorbrush);

        DC.Rectangle(left+5,top+4,right-15,bottom-4);    
    }

    if (lpd->itemState & ODS_FOCUS)
    {
        // Draw the focus
        //
        // It would have been nice just to
        // draw a rectangle using a pen created
        // with the PS_ALTERNATE style, but
        // this is not supported by WIN95

        int i;

        for (i=left+3;i<right-4;i+=2)
        {
            DC.SetPixel(i,top+3,RGB(0,0,0));
            DC.SetPixel(i,bottom-4,RGB(0,0,0));
        }

        for (i=top+3;i<bottom-4;i+=2)
        {
            DC.SetPixel(left+3,i,RGB(0,0,0));
            DC.SetPixel(right-4,i,RGB(0,0,0));
        }       
    }
        
    DC.SelectObject(oldpen);
    DC.SelectObject(oldbrush);


    DC.Detach();    
}


void CColorBtn::OnClicked() 
{
    // When the button is clicked, show the dialog.
	
	dlg.currentcolor = currentcolor;
	if (dlg.DoModal() == IDOK)
    {
        currentcolor = CColorBtnDlg::colors[dlg.colorindex];
        InvalidateRect(NULL);
		GetParent()->SendMessage(MB_CONFIG_CBUTTON, 0, 0);  
		Store();
		
    }	
}

// Store and Load use an undocumented CWinApp function

BOOL CColorBtn::Store()
{
//    return (AfxGetApp()->WriteProfileBinary("ColorData", "ColorTable",
//		(LPBYTE)CColorBtnDlg::colors,sizeof(COLORREF)*20) &&
//          AfxGetApp()->WriteProfileBinary("ColorData", "MRU",
//			(LPBYTE)CColorBtnDlg::used,sizeof(BYTE)*20));
	RegistryKey reg( HKEY_LOCAL_MACHINE, RegKey );
	reg.Write( "ColorPickerTable", (LPBYTE)CColorBtnDlg::colors,
		sizeof(COLORREF)*20);
	reg.Write("ColorPickerMRU",(LPBYTE)CColorBtnDlg::used,sizeof(BYTE)*20);
	return TRUE;
}

BOOL CColorBtn::Load()
{
    //BYTE *data = NULL;
    //UINT size;
	AutoBuf buf1(sizeof(COLORREF)*20);
	AutoBuf buf2(sizeof(BYTE)*20);
	memset(buf1.p,0,sizeof(COLORREF)*20);
	memset(buf2.p,0,sizeof(BYTE)*20);

    // This function allocates the memory it needs

    //AfxGetApp()->GetProfileBinary("ColorData", "ColorTable",&data,&size);	
	RegistryKey reg( HKEY_LOCAL_MACHINE, RegKey );
	reg.Read("ColorPickerTable", (void*)buf1.p, sizeof(COLORREF)*20,
		(void*)CColorBtnDlg::colors);

    if (buf1.p)
    {
        // Copy the data into our table and get rid of the buffer

        memcpy((void *)CColorBtnDlg::colors,(void *)buf1.p,sizeof(COLORREF)*20);
        //free((void *)data);

        //AfxGetApp()->GetProfileBinary("ColorData", "MRU",&data,&size);	
		reg.Read("ColorPickerMRU", (void*)buf2.p, 
			sizeof(BYTE)*20,(void *)CColorBtnDlg::used);

        if (buf2.p)
        {
            memcpy((void *)CColorBtnDlg::used,(void *)buf2.p,sizeof(BYTE)*20);
            //free((void *)data);
            return TRUE;
        }
        
    }

    // If the loading fails, back to the defaults
    
    Reset();

    return FALSE;
}


void CColorBtn::Reset()
{
    CColorBtnDlg::colors[0]  = RGB(0,0,0);
    CColorBtnDlg::colors[1]  = RGB(128,0,0);
    CColorBtnDlg::colors[2]  = RGB(0,128,0);
    CColorBtnDlg::colors[3]  = RGB(128,128,0);
    CColorBtnDlg::colors[4]  = RGB(0,0,128);
    CColorBtnDlg::colors[5]  = RGB(128,0,128);
    CColorBtnDlg::colors[6]  = RGB(0,128,128);
    CColorBtnDlg::colors[7]  = RGB(192,192,192);
    CColorBtnDlg::colors[8]  = RGB(192,220,192);
    CColorBtnDlg::colors[9]  = RGB(166,202,240);
    CColorBtnDlg::colors[10] = RGB(255,251,240);
    CColorBtnDlg::colors[11] = RGB(160,160,164);
    CColorBtnDlg::colors[12] = RGB(128,128,128);
    CColorBtnDlg::colors[13] = RGB(255,0,0);
    CColorBtnDlg::colors[14] = RGB(0,255,0);
    CColorBtnDlg::colors[15] = RGB(255,255,0);
    CColorBtnDlg::colors[16] = RGB(0,0,255);
    CColorBtnDlg::colors[17] = RGB(255,0,255);
    CColorBtnDlg::colors[18] = RGB(0,255,255);
    CColorBtnDlg::colors[19] = RGB(255,255,255);

    // This "colorful" (no pun intended) order ensures
    // that the colors at the center of the color table
    // will get replaced first. This preserves the white
    // and black colors even if they're not used (They'll
    // get replaced last).
    
    CColorBtnDlg::used[0]= 1;
    CColorBtnDlg::used[1]= 3;
    CColorBtnDlg::used[2]= 5;
    CColorBtnDlg::used[3]= 7;
    CColorBtnDlg::used[4]= 9;
    CColorBtnDlg::used[5]= 11;
    CColorBtnDlg::used[6]= 13;
    CColorBtnDlg::used[7]= 15;
    CColorBtnDlg::used[8]= 17;
    CColorBtnDlg::used[9]= 19;
    CColorBtnDlg::used[10]= 20;
    CColorBtnDlg::used[11]= 18;
    CColorBtnDlg::used[12]= 16;
    CColorBtnDlg::used[13]= 14;
    CColorBtnDlg::used[14]= 12;
    CColorBtnDlg::used[15]= 10;
    CColorBtnDlg::used[16]= 8;
    CColorBtnDlg::used[17]= 6;
    CColorBtnDlg::used[18]= 4;
    CColorBtnDlg::used[19]= 2;
}


void CColorBtn::Serialize( CArchive& ar )
{
    if (ar.IsStoring())
    {
        ar.Write((void *)CColorBtnDlg::colors,sizeof(COLORREF)*20);
        ar.Write((void *)CColorBtnDlg::used,sizeof(BYTE)*20);
    }
    else
    {
        ar.Read((void *)CColorBtnDlg::colors,sizeof(COLORREF)*20);
        ar.Read((void *)CColorBtnDlg::used,sizeof(BYTE)*20);
    }
}




/////////////////////////////////////////////////////////////////////////////
// CColorBtnDlg dialog


CColorBtnDlg::CColorBtnDlg(COLORREF curcol,CWnd* pParent /*=NULL*/)
	: CDialog(CColorBtnDlg::IDD, pParent), currentcolor(curcol),
	m_pen(NULL)
{
	//{{AFX_DATA_INIT(CColorBtnDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pen = new CPen;
	m_pen->CreatePen(PS_SOLID,1,RGB(0,0,0));
	colorindex = -1;
}
CColorBtnDlg::~CColorBtnDlg() 
{
	if (m_pen)
		delete m_pen;

	CDialog::~CDialog();
}


void CColorBtnDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CColorBtnDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CColorBtnDlg, CDialog)
	//{{AFX_MSG_MAP(CColorBtnDlg)
    ON_BN_CLICKED(IDC_OTHER, OnOther)
	ON_WM_LBUTTONDOWN()	
	ON_WM_LBUTTONUP()
    ON_WM_DRAWITEM()	
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
    ON_COMMAND_RANGE(IDC_COLOR1,IDC_COLOR20,OnColor)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CColorBtnDlg message handlers
static 	CPoint m_orig;

BOOL CColorBtnDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

    RECT r,r2;
	
	parent->GetWindowRect(&r);
    
    // Move the dialog to be below the button

    SetWindowPos(NULL,r.left,r.bottom,0,0,SWP_NOSIZE|SWP_NOZORDER);

    GetWindowRect(&r2);

    // Check to see if the dialog has a portion outside the
    // screen, if so, adjust.
    
    if (r2.bottom > GetSystemMetrics(SM_CYVIRTUALSCREEN))
    {   
        r2.top = r.top-(r2.bottom-r2.top);        
    }

    if (r2.right > GetSystemMetrics(SM_CXVIRTUALSCREEN))
    {
        r2.left = GetSystemMetrics(SM_CXVIRTUALSCREEN) - (r2.right-r2.left);
    }

    SetWindowPos(NULL,r2.left,r2.top,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_orig = CPoint(r2.left,r2.top);

    // Capture the mouse, this allows the dialog to close when
    // the user clicks outside.

    // Remember that the dialog has no "close" button.

    SetCapture();

	return TRUE; 
}



void CColorBtnDlg::EndDialog( int nResult )
{
    ReleaseCapture();

    CDialog::EndDialog(nResult);
}


void CColorBtnDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
    RECT r;

    POINT p;
    p.x = point.x;
    p.y = point.y;

    ClientToScreen(&p);

    GetWindowRect(&r);

    // The user clicked...

    if (!PtInRect(&r,p))
    {
        //  ...outside the dialog, close.

        EndDialog(IDCANCEL);
    }
    else
    {
        //  ...inside the dialog. Since this window
        //     has the mouse captured, its children
        //     get no messages. So, check to see
        //     if the click was in one of its children
        //     and tell him.

        //     If the user clicks inside the dialog
        //     but not on any of the controls,
        //     ChildWindowFromPoint returns a
        //     pointer to the dialog. In this
        //     case we do not resend the message
        //     (obviously) because it would cause
        //     a stack overflow.
        
        CWnd *child = ChildWindowFromPoint(point);

        if (child && child != this)
            child->SendMessage(WM_LBUTTONDOWN,0,0l);
    }
	
	CDialog::OnLButtonDown(nFlags, point);
}


void CColorBtnDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpd) 
{
	CDC dc;
    CPen nullpen;
    CBrush brush,bbrush;
    CPen *oldpen;
    CBrush *oldbrush;

    // Draw the wells using the current color table

    nullpen.CreateStockObject(NULL_PEN);
    brush.CreateSolidBrush(colors[nIDCtl-IDC_COLOR1]);

    dc.Attach(lpd->hDC);

    oldpen = dc.SelectObject(&nullpen);
    oldbrush = dc.SelectObject(&brush);

    lpd->rcItem.right++;
    lpd->rcItem.bottom++;

	dc.Rectangle(&lpd->rcItem);

    dc.SelectObject(oldpen);
    dc.SelectObject(oldbrush);

    dc.Detach();
	
	CDialog::OnDrawItem(nIDCtl, lpd);
}
void CColorBtnDlg::OnPaint() 
{
	int clrctl = colorindex + IDC_COLOR1;
	if (!(IDC_COLOR1 <= clrctl && clrctl <= IDC_COLOR20)) {
		for (int i=0;i<20;i++)
		{
	        if (colors[i] == currentcolor)
			{
	            colorindex = i;
				clrctl = colorindex + IDC_COLOR1;
				break;
			}
		}
	}
	if (IDC_COLOR1 <= clrctl && clrctl <= IDC_COLOR20) {
		CWnd * w = GetDlgItem(clrctl);
		if (w) {
			CRect rect;
			w->GetWindowRect(rect);
			ScreenToClient(rect);
			CPaintDC dc(this); 
			rect.InflateRect(1,1,1,1);
			CPen * oldpen = dc.SelectObject(m_pen);
			dc.Rectangle(rect);
			dc.SelectObject(oldpen);
		}
	}
	CDialog::OnPaint();
}

void CColorBtnDlg::OnColor(UINT id)
{

    // A well has been clicked, set the color index
    // and close.

    colorindex = id-IDC_COLOR1;
    
    int i;

    // This color is now the MRU

    for (i=0;i<20;i++)
    {
        if (used[colorindex] > used[i])
        {
            used[i]++;
        }
    }

    used[colorindex] = 1;

    EndDialog(IDOK);

}
UINT 
CColorBtnDlg::ColorDlgWindowProc(HWND hwnd, UINT message, 
							WPARAM wParam, LPARAM lParam)
{
	CRect r,r2;

	if (message == WM_INITDIALOG)
	{
		// place it right where the button dlg is, unless...
		::GetWindowRect(hwnd, &r);


		// Move the dialog to be below the button

		::SetWindowPos(hwnd,HWND_TOP, m_orig.x,m_orig.y,0,0,SWP_NOSIZE|SWP_NOZORDER);

		::GetWindowRect(hwnd,&r2);

		// Check to see if the dialog has a portion outside the
		// screen, if so, adjust.

		if (r2.bottom > GetSystemMetrics(SM_CYVIRTUALSCREEN))
		{   
			r2.top = r.top-(r2.bottom-r2.top);        
		}

		if (r2.right > GetSystemMetrics(SM_CXVIRTUALSCREEN))
		{
			r2.left = GetSystemMetrics(SM_CXVIRTUALSCREEN) - (r2.right-r2.left);
		}

		::SetWindowPos(hwnd,HWND_TOP,r2.left,r2.top,0,0,SWP_NOSIZE|SWP_NOZORDER);

		//customize the dialog caption
		::SetWindowText(hwnd, "Choose Color");
		return 1;
	}
	return 0;
}
void CColorBtnDlg::OnOther() 
{
	// The "Other" button.

	int i;
	COLORREF newcolor;
	RegistryKey reg( HKEY_LOCAL_MACHINE, RegKey );
	AutoBuf buf(sizeof(COLORREF)*16);
	reg.Read("ColorPickerCustom", (void*)buf.p, sizeof(COLORREF)*16,
		(void*)CColorBtnDlg::custom);
    if (buf.p)
    {
        // Copy the data into our table and get rid of the buffer

        memcpy((void *)CColorBtnDlg::custom,(void *)buf.p,
			sizeof(COLORREF)*16);
	}	

    ReleaseCapture();



	CColorDialog dlg;//(currentcolor,0,this);
	dlg.m_cc.rgbResult = currentcolor;
	dlg.m_cc.lpCustColors = CColorBtnDlg::custom;

    dlg.m_cc.Flags |= 
		CC_RGBINIT 
		| CC_ANYCOLOR 
		;
	dlg.m_cc.lpfnHook = ColorDlgWindowProc;

	if (dlg.DoModal() == IDOK)
    {
		reg.Write( "ColorPickerCustom", (void*)CColorBtnDlg::custom,
			sizeof(COLORREF)*16);

        // The user clicked OK.
        // set the color and close
        
        newcolor = dlg.GetColor();            
		
        // Check to see if the selected color is
        // already in the table.

        colorindex = -1;

        for (i=0;i<20;i++)
        {
            if (colors[i] == newcolor)
            {
                colorindex = i;
                break;
            }
        }

        // If the color was not found,
        // replace the LRU with this color
 
        if (colorindex == -1)
        {
            for (i=0;i<20;i++)
            {
                if (used[i] == 20)
                {
                    colors[i] = newcolor;
                    colorindex = i;                                 
                    break;
                }
            }
        }

        // This is the new MRU

        for (i=0;i<20;i++)
        {
            if (used[colorindex] > used[i])
            {
                used[i]++;
            }         
        }

        used[colorindex] = 1;

        EndDialog(IDOK);

        return;
    }

    // If the user clicked "Cancel" reclaim the mouse capture.

    SetCapture();        	
}


void CColorBtnDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{

    // See notes for OnLButtonDown.
    
    CWnd *child = ChildWindowFromPoint(point,CWP_ALL);
    
    if (child && child != this)
        child->SendMessage(WM_LBUTTONDOWN,0,0l);	
	
	CDialog::OnLButtonUp(nFlags, point);
}


