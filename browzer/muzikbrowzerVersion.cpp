#include "stdafx.h"
#include "muzikbrowzerVersion.h"
#include "SMTP.h"
#include "MyString.h"
#include "MyLog.h"

unsigned int
MBVersion::publicVersion() {
	CString reqbody("version=?");
#ifdef _DEBUG
	reqbody = "version=test";
#endif
	CSMTP http;
	CStringArray hdrs,body;
	BOOL r = http.http(hdrs,body,
		"www.muzikbrowzer.com",
		"",
		"/dl/current_rev.php",
		reqbody);
	if (!r) 
		return 0;

	unsigned int version=0;
	CString key ;
	if (body.GetSize())
		key = String::extract(body.GetAt(0),"","=");
	if (key != "version")
		return 0;
	CString val ;
	if (body.GetSize())
		val = String::extract(body.GetAt(0),"=","");
	if (val.GetLength() < 5)
		return 0;
	version = s2i(val);

	return version;
}
unsigned int
MBVersion::thisVersion() {
	CString verl = MBVERSION();
	CString vers = String::field(verl," ",1);
	return s2i(vers);
}
unsigned int
MBVersion::s2i(const CString & val)  {
	CString majs,mins,pats;
	majs = String::field(val,".",1);
	mins = String::field(val,".",2);
	pats = String::field(val,".",3);
	unsigned int major,minor,patch;
	major = minor = patch = 0;
	major = atoi(majs);
	minor = atoi(mins);
	patch = atoi(pats);
	
	unsigned int version = patch;
	version += minor * 100;
	version += major * 10000;
	return version;
}
BOOL
MBVersion::needUpdate() {
	unsigned int tVer,pVer;
	tVer = thisVersion();
	logger.log("tVer:"+numToString(tVer));
	pVer = publicVersion();
	logger.log("pVer:"+numToString(pVer));
	if (tVer < pVer) {
		return TRUE;
	} else {
		return FALSE;
	}
}