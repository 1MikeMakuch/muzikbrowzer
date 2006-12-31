#ifndef _MYSTRING_H
#define _MYSTRING_H

#include "stdafx.h"

#define CS(CS_string) CString(CS_string)

class MyHash {
public:
	MyHash(){}
	~MyHash(){
		m_hash.RemoveAll();
		m_sortedKeys.RemoveAll();
	}
	void setVal(const CString & key, const CString & val);
	CString getVal(const CString & key);
	BOOL contains(const CString & key);
	void SortedKeys(CStringList & keys);
	POSITION GetSortedHead();
	void GetNextAssoc(POSITION & pos, CString & key, CString & val);
	CMapStringToString m_hash;
	void logd(const CString & msg);
	int GetCount() { return m_hash.GetCount(); }

private:
	CStringList m_sortedKeys;
};

class String {
    private:
		String() {}
    public:
		~String() {}

    public:

        // Return the nth substring delimited by substring in string. The
	// first substring is 1, not 0. If either delim or occur is not in
	// the string null is returned, unless occur specifies 1. If occur
	// is 1 and delim is not found, then string is returned.
    static CString field(const CString &string, 
                     const CString &delim, const long occur);

	// Return the number of substrings delimited by substring in string.
	// If delim is not contained in string then 1 is returned.
    static long delCount(const CString &string, const CString &delim);

	// Returns the number of occurences of substring in string.
//         static long count(const RWCString &string, 
// 	                                           const RWCString &substring);

        // Return the substring delimited by 2 different substrings.
	// If either substring[1,2] is null the start/end of line is assumed.
	static CString extract(const CString &string,
	     const CString &substring1,
	     const CString &substring2,
	     const long occur=1);

	static CString substring(const CString & string,
		const int start, const int length = 0);

	// Simple string replacement
	// Replaces occur occurances of oldsubstring with newsubmstring in
	// string, occur=0 for all.
	static CString replace(const CString &string,
		const CString &newsubstring, 
		const CString &oldsubstring, 
		const unsigned int &occur=0);

	static CString replace(const CString string, const char src,
		const CString target);
	static CString downcase(const CString string);
	static CString upcase(const CString string);
	static int last(const CString & string, const char ch);
	static void insertSort(CStringList &list, const CString &string);
	static void insertSort(CStringArray &list, const CString &string);
	static CString stripws(const CString & string);
	static void trimLR(CString & string, const CString & trim="");
	static CString upDir(const CString & string);
	static void secs2HMS(const time_t secs, int & h, int & m, int & s);
	static CString secs2HMS(const time_t secs);

	static BOOL endsWith(const CString & string, const CString & end);
	static BOOL contains(const CString & string, const CString & contain);
	static BOOL equalUpToMin(const CString & string1, const CString & string2,
		BOOL ignorecase=FALSE);

	static int String::CompareNoCase(CString& element1, 
		CString& element2) 
	{
		return element1.CompareNoCase(element2);
	}
	static int String::CompareCase(CString& element1, 
		CString& element2) 
	{
		return element1.Compare(element2);
	}
	static void copyCStringList(CStringList & dest, const CStringList & src);
	static void copyCStringArray(CStringArray & dest, const CStringArray & src);
	static BOOL CStringListContains(const CStringList & list, 
		const CString & string);
	static BOOL CStringArrayContains(const CStringArray & list, 
		const CString & string);
	static void Sort(CStringList & list);
	static void Sort(CStringArray & list);
	static void Uniq(CStringArray & list);
	static BOOL CStringArray2File(const CString & file, CStringArray & array);
	static BOOL CStringArrayFromFile(CStringArray & array,const CString & file);
	static void CStringList2CStringArray(CStringArray & array, const CStringList & list);
	static void copyCStringArray2MyHash(MyHash & tmplist,const CStringArray & list);
	static CString numspaces(const int num);
};

class AutoBuf {
public:
	AutoBuf();
	void size(int size);
    AutoBuf(int size);
    ~AutoBuf();
    char * p;
};


CString & operator << (CString & left, const CString & right); 
CString & operator << (CString & left, const char * right);
CString & operator << (CString & left, const int           right); 
CString & operator << (CString & left, const unsigned int  right); 
CString & operator << (CString & left, const unsigned long right); 
CString & operator << (CString & left, const long          right); 

CString & operator += (CString & left, const int           right); 
CString & operator += (CString & left, const unsigned int  right); 
CString & operator += (CString & left, const unsigned long right); 
CString & operator += (CString & left, const long          right); 

CString operator << (const char * left, const CString & right); 
CString operator += (const char * left, const CString & right);

CString   numToString(int i);
CString   numToString(unsigned int i); 
CString   numToString(unsigned long ul);
CString   numToString(long ul);
CString   numToString(double d);

#define NTS(_NUM_TO_STRING_PARAM1_) numToString(_NUM_TO_STRING_PARAM1_)

class BaseC {
	public:
		static char		DecToSixtyTwoDigit	(const char digit);
		static char		SixtyTwoDigitToDec	(const char digit);
		static char		HexDigitToDec		(const char digit);
		static CString	DecToSixtyTwoString(__int64 i);
		static CString	HexStringToSixtyTwoString(const CString &h);
		static CString	SixtyTwoStringToHexString(const CString &s2);
};

CString getRGB(COLORREF rgb) ;

class MyUtil {
private:
	MyUtil(){};
public:
	~MyUtil(){};
	// returns random # between 0 and span-1, inclusive
	static int random(int span) ;
	// call this once first
	static void seed() ;
};

#endif
