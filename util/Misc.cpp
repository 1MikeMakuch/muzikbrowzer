
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
	CHECK(strcmp(k,"key10") == 0);
	result = strcmp(v,val10) == 0;
	CHECK(result == 0);
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
		if (0 != result) {
			CHECK(0 == result);
		}
		result = strcmp(valb.p,v);
		if (0 != result) {
			CHECK(0 == result);
		}
		//free(k);free(v);
		kvstack.pop();

	}
	CHECK(kvstack.empty() == TRUE);
}


