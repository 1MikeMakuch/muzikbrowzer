
 
#include "MyDC.h"
#include "FileUtils.h"


CBmpDC::CBmpDC(HBITMAP bmp):m_hbmp(bmp),m_holdbmp(NULL) {
		CreateCompatibleDC(NULL);
		m_holdbmp = (HBITMAP) SelectObject(m_hbmp);
}
void CBmpDC::SelectOrig() {
		if (m_holdbmp)
			SelectObject(m_holdbmp);
}
void CBmpDC::BmpLog(const CString & name) {
		SelectOrig();
		FileUtil::BmpLog(m_hbmp,name);
		(HBITMAP) SelectObject(m_hbmp);
}

CBmpDC::~CBmpDC() {
		SelectOrig();

}


 
