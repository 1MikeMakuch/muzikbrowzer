// ExtendedListBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CExtendedListBox window

class CExtendedListBox : public CListBox
{
// Construction
public:
	CExtendedListBox();

public:
	// default implementation uses LSB of item data; override to get other behaviour
	virtual BOOL IsItemBold(UINT) const;
	virtual BOOL IsItemDiff(UINT) const;

// Attributes
public:
	int insertString(int nIndex, LPCTSTR lpszItem, int b, int d);



// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtendedListBox)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
// Implementation
public:
	virtual ~CExtendedListBox();
	CExtendedListBox * m_companion;
	BOOL m_highlights;

	// Generated message map functions
protected:
	//{{AFX_MSG(CExtendedListBox)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
