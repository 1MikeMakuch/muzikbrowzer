#ifndef _MUZIKBROWZER__VERSION__H__
#define _MUZIKBROWZER__VERSION__H__
static const char MUZIKBROWZER_VERSION[] = "2.0.2 2006-12-23 16:22:44";

class MBVersion {
public:
	MBVersion(){}
	~MBVersion(){}
	CString MBVERSION() {
		return CString(MUZIKBROWZER_VERSION);
	}

	unsigned int thisVersion();
	unsigned int publicVersion();
	BOOL needUpdate();
protected:
	unsigned int s2i(const CString & s);
};

#endif
