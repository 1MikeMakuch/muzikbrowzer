
#include "StdAfx.h"
#include "Misc.h"
#include <stack>
using namespace std;
#include "ConfigFileLexer.h"
#include "TestHarness.h"
#include "MyLog.h"


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












