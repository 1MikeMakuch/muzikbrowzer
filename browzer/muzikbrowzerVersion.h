#ifndef _MUZIKBROWZER__VERSION__H__
#define _MUZIKBROWZER__VERSION__H__
static const char MUZIKBROWZER_VERSION[] = "2.0.2 2006-12-25 15:23:07";

#include "MyString.h"

class MBVersion {
public:
	MBVersion(const CString & license):m_license(license),
		m_talkedToMama(FALSE) {}
	~MBVersion(){}
	CString MBVERSION() {
		return CString(MUZIKBROWZER_VERSION);
	}

	unsigned int thisVersion();
	unsigned int publicVersion();
	BOOL phoneHome();
	BOOL needUpdate();
	BOOL goodLicense();
protected:
	unsigned int s2i(const CString & s);
	MyHash m_pairs;
	BOOL m_talkedToMama;
	CString m_license;
};

#endif
