#include "stdafx.h"
#include "muzikbrowzerVersion.h"
#include "SMTP.h"
#include "MyString.h"
#include "MyLog.h"
#include "Registry.h"

BOOL
MBVersion::phoneHome() {
	m_talkedToMama = TRUE;
//	logger.ods("phoning home");
	CString reqbody("version=?");
#ifdef _DEBUG
	reqbody = "version=test";
#endif
	reqbody += "&license=";
	reqbody += m_license;

	CSMTP http;
	CStringArray hdrs,body;
	BOOL r = http.http(hdrs,body,
		"www.muzikbrowzer.com",
		"",
		"/dl/current_rev.php",
		reqbody);

	if (!r) 
		return FALSE;
	
	if (body.GetSize()) {
		for(int i = 0; i < body.GetSize(); i++){
			http.entityBodyParse(m_pairs,body.GetAt(i));
		}
	} else {
		return FALSE;
	}
	m_talkedToMama = TRUE;
	return TRUE;
}
unsigned int
MBVersion::publicVersion() {
	if (!m_talkedToMama)
		phoneHome();

	unsigned int version=0;

	CString val = m_pairs.getVal("license");
	if (val.GetLength())
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
	if (!m_talkedToMama)
		phoneHome();

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
BOOL
MBVersion::goodLicense() {

	// phoneHome to check license validity
	
	// This can easily be circumvented by not allowing
	// the pc to connect to the net while the license is
	// good. Just using the reg key to signal that the
	// license is bad even if no longer connected to
	// the net.

	// Arbitrarily chose "DbVer" as a disguise.
	
	RegistryKey reg( HKEY_LOCAL_MACHINE, RegKey );

	// if DbVer is set to 2, then the license was listed
	// as bad on the server and discovered in the phoneHome()
	int check = reg.Read("DbVer",1);

	// We phone home anyway in case the license has been renewed
	if (!m_talkedToMama)
		phoneHome();
	
	CString val = m_pairs.getVal("license");

	// if not connected to the net then val will be null

	// in this case DbVer was previously bad (2) but now
	// it's good so we set it to (1)
	if ("good" == val && 2 == check)
		reg.Write("DbVer",1);

	// license is listed as bad on server so signal it with (2)
	if ("bad" == val)
		reg.Write("DbVer",2);

	return (val != "bad");
}








