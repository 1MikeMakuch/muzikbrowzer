
#include "StdAfx.h"

#include "MyString.h"
#include "MyLog.h"
#include "TestHarness.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString
String::replace(const CString cstring, const char src, const CString target) {
	int len = cstring.GetLength();
	CString newstring;
	char ch;
	for (int i = 0 ; i < len ; i++ ) {
		ch = cstring[i];
		if (ch == src) {
			if (target.GetLength())
				newstring += target;
		} else {
			newstring += ch;
		}
	}
	return newstring;
}

CString
String::downcase(const CString cstring) {
	CString newstring(cstring);
	newstring.MakeLower();
	return newstring;

	char ch;
	int len = cstring.GetLength();
	for (int i = 0 ; i < len; i++ ) {
		ch = cstring[i];
		if (65 <= ch && ch <= 90) {
			ch += 32;
		}
		newstring += ch;
	}
	return newstring;
}
CString
String::upcase(const CString cstring) {
	CString newstring(cstring);
	newstring.MakeUpper();
	return newstring;
}


// Return the occur'th substring of string delimited by delim.

CString
String::field(const CString &string, 
			  const CString &delim, const long occur) {
	
	if ((string.GetLength() < 1) || (delim.GetLength() < 1) || (occur < 1))
		return (CString) "";
	
	CString substring;
	long delims_found=0;
	for (unsigned long i=0 ; i < string.GetLength() ; ++i) {
		if (String::substring(string,i,delim.GetLength()) == delim) {
						//if (string(i,delim.GetLength()) == delim) {
			++delims_found;
			i += delim.GetLength() -1;
		} else if (delims_found == occur-1) {
			substring += string.GetAt(i);
//			substring += string(i,1);
		} else if (delims_found > occur-1) {
			i = string.GetLength();
		}
	}
	if (delims_found == 0 && occur == 1) 
		substring = string;
	
	return substring;
}
TEST(String, field)
{
	CString s = "a,b,c";
	CString a = String::field(s,",",1);
	CString b = String::field(s,",",2);
	CString c = String::field(s,",",3);
	CHECK(a == CString("a"));
	CHECK(b == CString("b"));
	CHECK(c == CString("c"));

	long n = String::delCount(s,",");
	CHECK(n == 3);
}
// 
// Return the number of substrings of string delimited by delim.

long
String::delCount(const CString &string, const CString &delim) {
	if ((string.GetLength() < 1) || (delim.GetLength() < 1))
		return (long)0;
	
	long delims_found=0;
	
	for (unsigned long i=0 ; i < string.GetLength() ; ++i) {
//		if (string(i,delim.GetLength()) == delim) {
		if (String::substring(string,i,delim.GetLength()) == delim) {
			++delims_found;
			i += delim.GetLength() -1;
		}
	}
	++delims_found;
	return delims_found;
}
// 
// // Return the number of substring's in string.
// long
// String::count(const RWCString &string, const RWCString &substring) {
//     if ((string.GetLength() < 1) || (substring.GetLength() < 1))
// 	return (long)0;
// 
//     long count=0;
// 
//     for (unsigned long i=0 ; i < string.GetLength() ; ++i) {
//         if (string(i,substring.GetLength()) == substring) {
//             ++count;
//             i += substring.GetLength() -1;
//         }
//     }
//     return count;
// }

// Return the substring delimited by 2 different substrings.
// If either substring[1,2] is null the start/end of line is assumed.
CString 
String::extract(const CString &string, const CString &substring1,
    const CString &substring2, const long occur) {

    CString substring;
	if (string.GetLength() == 0) 
		return substring;

    unsigned long len_string, len_sub1, len_sub2, start_pos, end_pos, sublen;
    len_string = string.GetLength();
    len_sub1 = substring1.GetLength();
    len_sub2 = substring2.GetLength();

    if (occur == 1) {} // not implemented

    // 1st find start_pos.
    start_pos = 0;
    if (len_sub1 < 1) {
	    start_pos = 0;
    } else {
        for (unsigned long i = 0 ; i < len_string - (len_sub1 + len_sub2 - 1) ; ++i) {
	        if (string.Mid(i,len_sub1) == substring1) {
		        start_pos = i + len_sub1;
		        i = len_string;
	        }
	    }
    }

    // 2nd find end_pos;
    end_pos = 0;
    if (len_sub2 < 1) {
	    end_pos = len_string-1;
    } else {
        unsigned long stop_pos = len_string - len_sub2;
        for (unsigned long i = start_pos ; i <= stop_pos ; ++i) {
	        if (string.Mid(i,len_sub2) == substring2) {
		        end_pos = i - 1;
		        i = len_string;
	        }
	    }
    }


    sublen = (end_pos - start_pos) + 1;
    if (sublen < 1) {
    	substring = "";
    } else {
        substring = string.Mid(start_pos, sublen);
    }

    return substring;

}
CString
String::substring(const CString & string, const int start, const int length)
{
	CString rstring;
	if (string.GetLength() == 0) return rstring;
	AutoBuf buf(string.GetLength()+1);
	strcpy(buf.p, (LPCTSTR) string);
	int stringLength = string.GetLength();
	if (start > stringLength-1) 
		return rstring;
	//if (length < 1) length = stringLength;

	char * p = buf.p + start;
	// using this 'int ip' makes BoundsChecker happy
	int ip = (int) buf.p;
	int ipstart = (int)p;
	if ((ipstart + length) > (ip + stringLength)) {
		rstring = p;
		return rstring;
	}
	if (length > 0) 
		buf.p[start+length] = 0;
	rstring = p;
	return rstring;

}
TEST(StringSubString, String)
{
	CString test = "123";
	CString test2 = String::substring(test,1,2);
	CHECK(test2 == "23");

	test2 = String::substring(test,0);
	CHECK(test2 == "123");
	test2 = String::substring(test,0,10);
	CHECK(test2 == "123");
	test2 = String::substring(test,2,1);
	CHECK(test2 == "3");
	test2 = String::substring(test,3,1);
	CHECK(test2 == "");

}

CString
String::replace ( const CString &tstring,
				 const CString &newsubstring,
				 const CString &oldsubstring,
				 const unsigned int &occur) {
	
	CString newstring;
	CString work(tstring);
	unsigned int count(occur);
	
	unsigned int oldlen = oldsubstring.GetLength();
	while (work.GetLength()) {
		if ( work.GetLength() >= oldlen && work.Mid ( 0, oldlen )
			== oldsubstring 
			&& ( occur == 0 || count-- )
			) {
			newstring += newsubstring;
			work = work.Mid ( oldlen, work.GetLength() - oldlen );
		} else {
			newstring += work.Mid(0,1);
			work = work.Mid ( 1, work.GetLength() - 1 );
		}
	}
	return newstring;
	
}
TEST(StringReplace, String)
{
	CString test = "__123__456__789__";
	CString newsub,oldsub;
	oldsub = "__" ; newsub = "--";
	CString r1 = String::replace(test, newsub, oldsub);
	CHECK(r1 == "--123--456--789--");
}
int
String::last(const CString & string, const char ch) {
	int pos = -1;
	int n = string.GetLength();
	int i;
	for(i = 0 ; i < n ; ++i) {
		if (string.GetAt(i) == ch) {
			pos = i;
		}
	}
	return pos;
}
AutoBuf::AutoBuf(int size) {
    p = new char[size];
	memset(p, 0, size-1);
}

AutoBuf::~AutoBuf() {
    delete [] p;
}




CString & operator << (CString& left, const CString& right) {
	left += right; 
	return left;
}    
CString & operator << (CString & left, const char * right) {
	CString Right(right);
	left += Right;
	return left;
}

CString & operator << (CString& left, const int right) {
	left += numToString(right);
	return left;
}    

CString & operator << (CString& left, const unsigned int right) {
	left += numToString(right);
	return left;
}    

CString & operator << (CString& left, const unsigned long right) {
	left += numToString(right);
	return left;
}   

CString & operator << (CString& left, const long right) {
	left += numToString(right);
	return left;
}   
CString & operator += (CString& left, const int right) {
	left += numToString(right);
	return left;
}    

CString & operator += (CString& left, const unsigned int right) {
	left += numToString(right);
	return left;
}    

CString & operator += (CString& left, const unsigned long right) {
	left += numToString(right);
	return left;
}   

CString & operator += (CString& left, const long right) {
	left += numToString(right);
	return left;
}   

CString operator << (const char * left, const CString & right) {
	CString string(left);
	string += right;
	return string;
}
CString operator += (const char * left, const CString & right) {
	CString string(left);
	string += right;
	return string;
}


CString
numToString(int i) {
    char buf[64];
    sprintf(buf, "%d", i);
    CString res(buf);
    return res;
}   
    
CString
numToString(unsigned int i) {
    char buf[64];
    sprintf(buf, "%u", i);
    CString res(buf);
    return res;
}   
    
CString
numToString(unsigned long ul) {
    char buf[64];
    sprintf(buf, "%lu", ul);
    CString res(buf);
    return res;
}   
    
CString
numToString(long l) {
    char buf[64];
    sprintf(buf, "%ld", l);
    CString res(buf);
    return res;
}   
    
CString
numToString(double d) {
    char buf[64];
    sprintf(buf, "%f", d);
    CString res(buf);
    return res;

}

    static const char tableDecToSixtyTwo[] = {'0', '1', '2', '3', '4',
    '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F',
    'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q',
    'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b',
    'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
    'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x',
    'y', 'z'} ;
  
    static const int tableHexToDec[] = {-1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, -1, -1, -1, -1, -1, -1, 
    -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, 
    -1 , -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, 10, 11, 12, 13, 14, 15};
 
    static const int tableSixtyTwoDigitToDec[] = {-1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, -1, -1, -1, -1, 
    -1, -1, -1, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 
    21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 
    35, -1, -1, -1, -1, -1, -1, 36, 37, 38, 39, 40, 41, 42, 
    43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 
    57, 58, 59, 60, 61};

char 
BaseC::DecToSixtyTwoDigit(const char digit) {
	if (digit < 0 || digit > 61) {
		CString msg;
		msg << "DecDigitToSixtyTwo error:" << digit;
		logger.log(msg);
		return '*';
	}
	return tableDecToSixtyTwo[digit];
}

TEST(DecDigitToSixtyTwoDigit, BascC)
{
	int i;
	for(i = 0 ; i < 62 ; ++i) {
		CHECK(tableDecToSixtyTwo[i] == BaseC::DecToSixtyTwoDigit(i));
	}
	CHECK('*' == BaseC::DecToSixtyTwoDigit(-1));
	CHECK('*' == BaseC::DecToSixtyTwoDigit(62));

}
char
BaseC::SixtyTwoDigitToDec(const char digit) {
	char dec= 0;
	if (digit < 0 || digit > 122) {
		CString msg;
		msg << "Unexpected digit in SixtyTwoDigitToDec:" << digit;
		logger.log(msg);
		return -1;
	}
	dec = tableSixtyTwoDigitToDec[digit];
	if (!(0 <= dec && dec <= 61)) {
		CString msg;
		msg << "Unexpected digit in SixtyTwoDigitToDec:" << digit;
		logger.log(msg);
		return -1;
	}
	return dec;
}
TEST(SixtyTwoDigitToDec, BaseC)
{
	char i;
	for(i = 0 ; i < 62 ; ++i) {
		CHECK(i == BaseC::SixtyTwoDigitToDec(tableDecToSixtyTwo[i]));
	}
	CHECK(-1 == BaseC::SixtyTwoDigitToDec(-1));
	CHECK(-1 == BaseC::SixtyTwoDigitToDec(123));
}



char
BaseC::HexDigitToDec(const char ch) {
	char dec= 0;
	if (ch < 0 || ch > 102) {
		CString msg;
		msg << "Unexpected digit in HexDigitToDec:" << ch;
		logger.log(msg);
		return -1;
	}
	dec = tableHexToDec[ch];
	if (!(0 <= dec && dec <= 15)) {
		CString msg;
		msg << "Unexpected digit in HexDigitToDec:" << ch;
		logger.log(msg);
		return -1;
	}
	return dec;
}

TEST(MyStringUtils, BascC)
{
	CHECK((char)0 == BaseC::HexDigitToDec('0'));
	CHECK((char)1 == BaseC::HexDigitToDec('1'));
	CHECK((char)2 == BaseC::HexDigitToDec('2'));
	CHECK((char)3 == BaseC::HexDigitToDec('3'));
	CHECK((char)4 == BaseC::HexDigitToDec('4'));
	CHECK((char)5 == BaseC::HexDigitToDec('5'));
	CHECK((char)6 == BaseC::HexDigitToDec('6'));
	CHECK((char)7 == BaseC::HexDigitToDec('7'));
	CHECK((char)8 == BaseC::HexDigitToDec('8'));
	CHECK((char)9 == BaseC::HexDigitToDec('9'));
	CHECK((char)10 == BaseC::HexDigitToDec('a'));
	CHECK((char)11 == BaseC::HexDigitToDec('b'));
	CHECK((char)12 == BaseC::HexDigitToDec('c'));
	CHECK((char)13 == BaseC::HexDigitToDec('d'));
	CHECK((char)14 == BaseC::HexDigitToDec('e'));
	CHECK((char)15 == BaseC::HexDigitToDec('f'));
	CHECK((char)10 == BaseC::HexDigitToDec('A'));
	CHECK((char)11 == BaseC::HexDigitToDec('B'));
	CHECK((char)12 == BaseC::HexDigitToDec('C'));
	CHECK((char)13 == BaseC::HexDigitToDec('D'));
	CHECK((char)14 == BaseC::HexDigitToDec('E'));
	CHECK((char)15 == BaseC::HexDigitToDec('F'));
	CHECK(-1       == BaseC::HexDigitToDec('z'));
	CHECK(-1       == BaseC::HexDigitToDec(-1));
}

CString 
BaseC::DecToSixtyTwoString(__int64 dec) {
	__int64 quotient = dec;
	__int64 quotient2;
	CString ch;
	CString sixty2;
	while(quotient > 0) {
		quotient2= quotient / 62;
		int remainder = (int)(quotient % 62);
		ch = BaseC::DecToSixtyTwoDigit(remainder);
		sixty2 = ch + sixty2;
		quotient = quotient2;
	}
	return sixty2;
}

CString 
BaseC::HexStringToSixtyTwoString(const CString & h) {
	int i,n;
	n = h.GetLength();
	i = 0;
	int numLeadingZeros = 0;
	char ch = h.GetAt(i);
	while(i < n && ch == '0') {
		++numLeadingZeros;
		ch = h.GetAt(++i);
	}

	CString sixty2;
	__int64 value = 0;
	__int64 place;
	place = 1;
	n = h.GetLength();
	for(i = n-1 ; i >= 0 ; --i) {
		char ch = h.GetAt(i);
		char val = BaseC::HexDigitToDec(ch);
		if (val == -1) {
			CString msg; msg << "unexpected h in hTo62:" << h;
			logger.log(msg);
			return sixty2;
		}
		__int64 placeValue = val * place;
		if (placeValue < 0) {
			CString msg; msg << "overflow HexStringToSixtyTwoString "
				<< h;
			logger.log(msg);
			return CString("");
		}
		value += placeValue;
		place *= 16;
	}
	sixty2 = BaseC::DecToSixtyTwoString(value);
	i = 0;
	while (i++ < numLeadingZeros)
		sixty2 = "0" + sixty2;

	return sixty2;
}

CString
BaseC::SixtyTwoStringToHexString(const CString & sixty2){
	int i,n;
	n = sixty2.GetLength();
	i = 0;
	int numLeadingZeros = 0;
	char ch = sixty2.GetAt(i);
	while(i < n && ch == '0') {
		++numLeadingZeros;
		ch = sixty2.GetAt(++i);
	}

	CString hex;
	__int64 value = 0;
	__int64 place;
	place = 1;
	for (i = n-1 ; i >= 0 ; --i) {
		char ch = sixty2.GetAt(i);
		char val = BaseC::SixtyTwoDigitToDec(ch);
		if (val == -1) {
			CString msg;
			msg << "unexpected digit in SixtyTwoStringToHexString: "
				<< ch;
			logger.log(msg);
			return hex;
		}
		__int64 placeValue = val * place;
		if (placeValue < 0) {
			CString msg; msg << "overflow SixtyTwoStringToHexString "
				<< sixty2;
			logger.log(msg);
			return CString("");
		}
		value += placeValue;
		place *= 62;
	}
	char buf[1000];
	sprintf(buf, "%I64x", value);
	hex = buf;
	
	i = 0;
	while (i++ < numLeadingZeros)
		hex = "0" + hex;

	return hex;
}

TEST(HexTo62AndBack, BaseC)
{
	char buf[50];
	sprintf(buf,"%x", 61);
	CString hex = buf;
	CString sixty2 = BaseC::HexStringToSixtyTwoString(hex);
	CHECK(hex == BaseC::SixtyTwoStringToHexString(sixty2));
	hex = "4fabc123";
	sixty2 = BaseC::HexStringToSixtyTwoString(hex);
	CHECK(hex == BaseC::SixtyTwoStringToHexString(sixty2));

	int i;
	hex = "f";
	CString hex2;
	for(i = 0 ; i < 15 ; ++i) {
		sixty2 = BaseC::HexStringToSixtyTwoString(hex);
		hex2 = BaseC::SixtyTwoStringToHexString(sixty2);
		CHECK(hex == hex2);
		hex += "f";

	}

}

CString getRGB(COLORREF rgb) {
	BYTE r = ((BYTE) ((rgb) >> 16)) ;
	BYTE g = ((BYTE) (((WORD) (rgb)) >> 8));
	BYTE b =  ((BYTE) (rgb)) ;
	AutoBuf buf(100);
	sprintf(buf.p, "%d %d %d", r,g,b);
	return CString(buf.p);
}


int
MyUtil::random(int span) {
	float ratio = rand() / (float)RAND_MAX;
	return (int)(ratio * span);
}
void
MyUtil::seed() {
	srand( (unsigned)time( NULL ) );
}