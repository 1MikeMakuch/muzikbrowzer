
#include "StdAfx.h"
#include "Misc.h"
#include <stack>
using namespace std;
#include "ConfigFileLexer.h"
#include "TestHarness.h"
#include "MyLog.h"
#include "FileUtils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString MBFormatError(LONG e) {
	char msgBuf[ 512 ];
	FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, e, 0, msgBuf, sizeof( msgBuf ) / sizeof( char ), NULL );
	CString msg(msgBuf);
	return msg;
}


CPoint crtopright(CRect & rect) {
	CPoint p;
	p.x = rect.right;
	p.y = rect.top;
	return p;
}
CPoint crtopleft(CRect & rect) {
	CPoint p(rect.TopLeft());
	return p;
}
CPoint crbottomright(CRect & rect) {
	CPoint p(rect.BottomRight());
	return p;
}
CPoint crbottomleft(CRect & rect) {
	CPoint p;
	p.x = rect.left;
	p.y = rect.bottom;
	return p;
}



//TEST(ConfigFileParser, ParseTest1)
//{
//	stack<PKVPAIR> kvstack;
//	char * file = "..\\testdata\\ConfigFileParser.testdat";
//	ConfigFileParser(file, &kvstack);
//	AutoBuf buf(1000);
//
//	logger.ods("ConfigFileParserTest");
//
//	int i;
//	AutoBuf keyb(1000);
//	AutoBuf valb(1000);
//	AutoBuf lbuf(1000);
//	char * k,*v;
//	int result;
//	for (i = 10 ; i > 0 ; i--) {
//		CHECK(kvstack.empty() == FALSE);
//		sprintf(keyb.p,"key%d",i);
//		sprintf(valb.p,"val%d",i);
//		k = kvstack.top()->key;
//		v = kvstack.top()->val;
//		sprintf(lbuf.p,"[%s]=[%s]",k,v);
//		logger.ods(lbuf.p);
//		result = strcmp(keyb.p,k);
//		if (0 != result) {
//			CHECK(0 == result);
//		}
//		result = strcmp(valb.p,v);
//		if (0 != result) {
//			CHECK(0 == result);
//		}
//		free(k);free(v);
//		kvstack.pop();
//
//	}
//	CHECK(kvstack.empty() == TRUE);
//}

const char * val10 = "0	 !\"$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
const char * val11 = "100 , 100 , 100";
TEST(ConfigFileParser2, ParseTest2)
{
	stack<KVPair> kvstack;
	char * file = "..\\testdata\\ConfigFileParser.testdat";
	ConfigFileParser(file, &kvstack);
	AutoBuf buf(1000);

	logger.ods("ConfigFileParserTest2");

	int i;
	AutoBuf keyb(1000);
	AutoBuf valb(1000);
	AutoBuf lbuf(1000);
	int result;
	const char * k = kvstack.top().key();
	const char * v = kvstack.top().val();

	CHECK(strcmp(k,"key11") == 0);
	result = strcmp(v,val11);
	CHECK(0 == result);

	kvstack.pop();

	const char * k2 = kvstack.top().key();
	const char * v2 = kvstack.top().val();
	CHECK(strcmp(k2,"key10") == 0);
	result = strcmp(v2,val10) == 0;
	CHECK(0 == result);
	kvstack.pop();
	for (i = 9 ; i > 0 ; i--) {
		CHECK(kvstack.empty() == FALSE);
		sprintf(keyb.p,"key%d",i);
		sprintf(valb.p,"val%d",i);
		const char * k = kvstack.top().key();
		const char * v = kvstack.top().val();
		sprintf(lbuf.p,"[%s]=[%s]",k,v);
		logger.ods(lbuf.p);
		result = strcmp(keyb.p,k);
		CHECK(0 == result);

		result = strcmp(valb.p,v);
		CHECK(0 == result);

		//free(k);free(v);
		kvstack.pop();

	}
	CHECK(kvstack.empty() == TRUE);
}

// Validate font spec from config file
BOOL
MBUtil::ConfigFontValidate(const char * val)
{
	return ConfigFontParser(val);
}
TEST(ConfigFontParser, parsetest)
{

	char * font1 = "0";
	BOOL r = MBUtil::ConfigFontValidate(font1);
	CHECK(r == FALSE);
	char * font2 = "FFFFFEC0 00000000 00000000 00000000 000002BC 00 00 00 00 00 00 00 00 Arial";
	r = MBUtil::ConfigFontValidate(font2);
	CHECK(r == TRUE);

}
CString
MBUtil::CrToRGB(COLORREF rgb) {
	CString srgb  = numToString(GetRValue(rgb)) + ","
		+ numToString(GetGValue(rgb)) + ","
		+ numToString(GetBValue(rgb));
	return srgb;
}

// get the RGB value from a string like "100,100,100"
BOOL
MBUtil::RgbTriple(const char * vali, unsigned long & rgb) 
{
	AutoBuf val(strlen(vali)+1);
	strcpy(val.p, vali);
	char * pr,*pg,*pb;
	pr = pg = pb = NULL;

	pr = strtok(val.p,", ");
	if (NULL == pr)	return FALSE;
	pg = strtok(NULL,", ");
	if (NULL == pg)	return FALSE;
	pb = strtok(NULL,", ");
	if (NULL == pb)	return FALSE;

	int r,g,b;
	r = g = b = 0;
	r = atoi(pr); g = atoi(pg) ; b = atoi(pb);

	rgb = RGB(r,g,b);
	return TRUE;
}

TEST(MBUtilRgbTriple, test1)
{
	char *buf1 = "10,20,30";

	unsigned long rgb,rgb2;
	BOOL r = MBUtil::RgbTriple(buf1, rgb);
	CHECK(TRUE == r);
	CHECK(1971210 == rgb);

	char *buf2= "";
	r = MBUtil::RgbTriple(buf2, rgb);
	CHECK(FALSE == r);

	char *buf3 = ",";
	r = MBUtil::RgbTriple(buf3, rgb);
	CHECK(FALSE == r);

	char *buf4 = "0,0,";
	r = MBUtil::RgbTriple(buf4, rgb);
	CHECK(FALSE == r);

	char * buf5 = " 1 , 1 , 1 ";
	r = MBUtil::RgbTriple(buf5, rgb);

	char * buf6 = "1,1,1";
	r = MBUtil::RgbTriple(buf6, rgb2);
	CHECK(rgb == rgb2);

	char * buf7 = " 1 1 1 ";
	r = MBUtil::RgbTriple(buf7, rgb2);
	CHECK(rgb == rgb2);

	char * buf8 = " 1  1  1  1 ";
	r = MBUtil::RgbTriple(buf8, rgb2);
	CHECK(rgb == rgb2);

}

void MBUtil::BmpToDC(CDC* pDC, BitmapToCRect * bmcr, 
						   BOOL doTrans, COLORREF bkcolor, int offset)
{
    if (!bmcr)
        return ;
	
    HBITMAP    pbmpOldBmp = NULL;
	HBITMAP hMask = NULL;
	UINT BITBLTTYPE = SRCCOPY;

	// offset of 0 is used for painting the main dialog background + the
	// resize border (2). offset of 2 is used for remainder of 
	// background panels so they line up properly. If using this func
	// for other use just leave offset at 0.
	int x,y,srcwidth,srcheight,destwidth,destheight;
	x = bmcr->m_rect.left+offset;
	y =  bmcr->m_rect.top+offset;
	
	// dst dimensions
	destwidth = bmcr->m_rect.Width();
	destheight = bmcr->m_rect.Height();
	
	// src dimensions
	srcwidth = bmcr->m_width;
	srcheight = bmcr->m_height;

	if (doTrans) { // do the transparent stuff
		hMask = MBUtil::CreateBitmapMask(bmcr->m_hBitmap, destwidth, destheight,
			srcwidth, srcheight, bmcr->m_loStyle,bkcolor);

		BitmapToCRect bmcrTrans((HBITMAP)hMask, bmcr->m_rect, bmcr->m_loStyle, 
			srcwidth,srcheight, CS("MBUtil::BmpToDC"));
		MBUtil::CutAndTileBmp(pDC,&bmcrTrans,offset,SRCAND);

		BITBLTTYPE = SRCPAINT;
	}
	MBUtil::CutAndTileBmp(pDC, bmcr, offset, BITBLTTYPE);
	
	::DeleteObject(hMask);
	
    return ;
}
//void MBUtil::CutAndTileBmp(CDC* pDC, HDC hdcSrc, HBITMAP bmp,
//		CRect & destrect, LayOutStyle los, 
//		int srcwidth, int srcheight,
//		int offset,UINT BITBLTTYPE ) {
//
//	BitmapToCRect bmcr(bmp, destrect, los, 
//		srcwidth,srcheight, CS("MBUtil::BmpToDC"), hdcSrc);
//	MBUtil::CutAndTileBmp(pDC, &bmcr, offset, BITBLTTYPE);
//}
static void tileloop(const int xoff,  const int yoff,
					 const int destwidth, const int destheight,
					 const int left, const int right,
					 const int top  , const int bottom,
					 const int width, const int height,
					 const int x_src, const int y_src,
					 CDC * dc, CDC * pDC, const DWORD rop,
					 int PaintEdges)
{
	int lwidth = width;
	int lheight = height;
	int trimWidth = width * PaintEdges;    // PaintEdges == 1 or 0
	int trimHeight = height * PaintEdges;
	int x,y;
	for (x = left; x < right ; x += width) {
		for (y = top; y < bottom; y += height) {
			if (x > right - width)
				lwidth =  ((xoff + destwidth) - trimWidth) - x;
			if (y > bottom - height)
				lheight = ((yoff + destheight) - trimHeight) - y;
			pDC->BitBlt(x,y, lwidth, lheight,
				dc, x_src, y_src, rop);
			lwidth = width; lheight = height;
		}
	}
}

#define TileLoop \
		tileloop(x,y,destwidth,destheight,\
			left,right,top,bottom,width,height,\
			x_src,y_src,dc,pDC,BITBLTTYPE, 0);

#define TileLoopNoEdges \
		tileloop(x,y,destwidth,destheight,\
			left,right,top,bottom,width,height,\
			x_src,y_src,dc,pDC,BITBLTTYPE, 1);

void MBUtil::CutAndTileBmp(CDC* pDC, BitmapToCRect * bmcr, 
						   int offset, UINT		BITBLTTYPE )
{
    CDC * dc = NULL;
	HBITMAP pbmpOldBmp = NULL;
	if (NULL != bmcr->m_dc) {
		dc = bmcr->m_dc;
	} else {
		dc = new CDC();
		dc->CreateCompatibleDC(NULL);
		pbmpOldBmp = (HBITMAP)::SelectObject(dc->m_hDC, bmcr->m_hBitmap);
	}


	int x,y,srcwidth,srcheight,destwidth,destheight;
	x = bmcr->m_rect.left+offset;
	y =  bmcr->m_rect.top+offset;
	
	// dst dimensions
	destwidth = bmcr->m_rect.Width();
	destheight = bmcr->m_rect.Height();
	
	// src dimensions
	srcwidth = bmcr->m_width;
	srcheight = bmcr->m_height;

	int x_src,y_src,x_dst,y_dst;
	int width,height;
	int thirdwidth = bmcr->m_width / 3;
	int thirdheight = bmcr->m_height / 3;
	if (thirdwidth > destwidth/3)
		thirdwidth = destwidth / 3;
	if (thirdheight > destheight/3)
		thirdheight = destheight / 3;

	x_src = y_src = x_dst = y_dst = 0;
	x_dst = x;
	y_dst = y;
	int left,right,top,bottom;

    if ( bmcr->m_loStyle == LO_FIXED )
    {
		pDC->BitBlt(bmcr->m_rect.left+offset, bmcr->m_rect.top+offset, 
				destwidth, destheight, dc, 0, 0, BITBLTTYPE);
    } else if (bmcr->m_loStyle == LO_TILED0) {
		left = x;
		right = x + destwidth;
		top = y;
		bottom = y + destheight;
		width = srcwidth;
		height = srcheight;
		TileLoop;
		
		left = x + width;
		right = x + destwidth;
		top = y + height;
		bottom = y + destheight;
		width = srcwidth;
		height = srcheight;
		TileLoop;
    } else if (bmcr->m_loStyle == LO_TILED) {
		width = thirdwidth;
		height = thirdheight;

//goto bottomedge;
		// middle
		x_src = thirdwidth;
		y_src = thirdheight;
		left = width + x;
		right = (destwidth - width) + x;
		top = height + y;
		bottom = (destheight - height) + y;
		TileLoopNoEdges;

		// top edge
		x_src = thirdwidth;
		y_src = 0;
		top = y;;
		bottom = top + height;
		TileLoopNoEdges;

		// right edge
		x_src = bmcr->m_width - thirdwidth;
		y_src = thirdheight;
		top = height + y;
		bottom = (destheight - height) + y;
		left = (destwidth - width) + x;
		right = destwidth + x;
		TileLoopNoEdges;
//bottomedge:
		// bottom edge
		x_src = width;
		y_src = bmcr->m_height - thirdheight;
		top = (destheight - height) + y;
		bottom = destheight + y;
		left = width + x;
		right = (destwidth - width) + x;
		TileLoopNoEdges;
//goto bottomcorners;
		// left edge
		x_src = 0;
		y_src = thirdheight;
		top = height + y;
		bottom = (destheight - height) + y;
		left = x;
		right = width + x;
		TileLoopNoEdges;


		// upper left
		x_src = y_src = x_dst = y_dst = 0;
		x_dst = x;
		y_dst = y;
		width = width = thirdwidth;
		height = height = thirdheight;
		pDC->BitBlt(x_dst,y_dst, width, height, dc, x_src, y_src, BITBLTTYPE);

		// upper right
		x_src = bmcr->m_width - thirdwidth;
		x_dst = (destwidth - thirdwidth)+x;
		pDC->BitBlt(x_dst,y_dst, width, height, dc, x_src, y_src, BITBLTTYPE);
//bottomcorners:
		// lower left
		x_src = 0;
		y_src = bmcr->m_height - thirdheight;
		x_dst = x;
		y_dst = (destheight - thirdheight)+y;
		pDC->BitBlt(x_dst,y_dst, width, height, dc, x_src, y_src, BITBLTTYPE);

		// lower right
		x_src = bmcr->m_width - thirdwidth;
		x_dst = (destwidth - thirdwidth)+x;
		y_dst = (destheight - thirdheight)+y;
		pDC->BitBlt(x_dst,y_dst, width, height, dc, x_src, y_src, BITBLTTYPE);
	}
    else if (bmcr->m_loStyle == LO_CENTER)
    {
        int ixOrg = x + (destwidth - bmcr->m_width) / 2;
        int iyOrg = y + (destheight - bmcr->m_height) / 2;
        pDC->BitBlt(ixOrg+offset, iyOrg+offset, destwidth, destheight, dc, 0, 0, BITBLTTYPE);
    }
    else if ( bmcr->m_loStyle == LO_STRETCHED)
    {
        pDC->StretchBlt(x, y, destwidth, destheight, 
			dc, 0, 0, srcwidth, srcheight, BITBLTTYPE);
    }
//quitit:
	if (NULL == bmcr->m_dc) {
		::SelectObject(dc->m_hDC, pbmpOldBmp);
		dc->DeleteDC();
		delete dc;
	}

}


HBITMAP MBUtil::CreateBitmapMask(HBITMAP hSourceBitmap, 
				DWORD destwidth, DWORD destheight,
				DWORD srcwidth, DWORD srcheight,
				LayOutStyle los,
				COLORREF bkcolor)
{
	HBITMAP		hMask		= NULL;
	CDC			dcSrc;
	CDC			dcDest;
	CDC			dcTmp;
	HBITMAP		hbmSrcT		= NULL;
	HBITMAP		hbmDestT	= NULL;
	HBITMAP		hbmNewSrc	= NULL;
	HBITMAP		hbmTmp		= NULL;
	COLORREF	crSaveBk;
	COLORREF	crSaveDestText;

	CDC dc;
	dc.CreateCompatibleDC(NULL);

	hMask = ::CreateCompatibleBitmap(dc.m_hDC, srcwidth,srcheight);
	if (hMask == NULL)	return NULL;

	dcSrc.CreateCompatibleDC(NULL);
	dcDest.CreateCompatibleDC(NULL);

//	FileUtil::BmpLog(hSourceBitmap, "hSrc");
	
	hbmSrcT = (HBITMAP)dcSrc.SelectObject(hSourceBitmap);
	hbmDestT = (HBITMAP)dcDest.SelectObject(hMask);

	crSaveBk = dcSrc.SetBkColor(bkcolor);

	dcDest.BitBlt(0,0,srcwidth,srcheight,&dcSrc,0,0,SRCCOPY);

	crSaveDestText = dcSrc.SetTextColor(RGB(255, 255, 255));
	dcSrc.SetBkColor(RGB(0, 0, 0));

	dcSrc.BitBlt(0,0,srcwidth,srcheight,&dcDest,0,0,SRCAND);

	dcDest.SetTextColor(crSaveDestText);

	dcSrc.SetBkColor(crSaveBk);
	dcSrc.SelectObject(hbmSrcT);
	dcDest.SelectObject(hbmDestT);

	return hMask;
} // End of CreateBitmapMask