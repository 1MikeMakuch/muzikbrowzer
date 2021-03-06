// Picture.h: interface for the CPicture class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PICTURE_H__6098A4C3_D6D5_4711_BC7B_1595F459B480__INCLUDED_)
#define AFX_PICTURE_H__6098A4C3_D6D5_4711_BC7B_1595F459B480__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ocidl.h"
class CPicture  
{
public:
	CPicture(CWnd *, bool square=true);
	virtual ~CPicture();

	bool			Load(LPTSTR sResourceType, LPTSTR sResource);
	bool			Load(CString sFileName);
	bool			Draw(CDC* pDC);
	bool			Draw(CDC* pDC, CPoint Pos);
	bool			Draw(CDC* pDC, CPoint Pos, CSize Size);
	bool			Draw(CDC* pDC, double nSizeRatio);
	bool			Draw(CDC* pDC, CPoint Pos, double nSizeRatio);
	CSize			GetSize(CDC* pDC);
//	bool			LoadFromBuffer(BYTE* pBuff, int nSize, long & width,
//						long & height);
	bool			LoadFromBuffer(BYTE* pBuff, int nSize);
	void			UnLoad();
private:
	static	bool	GetResource(LPSTR lpName, LPSTR lpType, void* pResource, int& nBufSize);


	bool			Draw(CDC* pDC, int x, int y, int cx, int cy);
	IPicture*		m_pPicture;
	enum
	{
		HIMETRIC_INCH	= 2540
	};

	CWnd * m_wnd;
	bool			m_bSquare;
};

#endif // !defined(AFX_PICTURE_H__6098A4C3_D6D5_4711_BC7B_1595F459B480__INCLUDED_)
