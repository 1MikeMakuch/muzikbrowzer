/******************************************************************************

  Yet another registry access class

                      Copyright 1999 Gavin Smyth
                 http://www.beesknees.freeserve.co.uk

******************************************************************************/

#include "StdAfx.h"

//#include "irman_common.h"
#include "Registry.h"
#include "MyString.h"
#include "MyLog.h"
#include "TestHarness/TestHarness.h"
#include <stack>
using namespace std;
#include "ConfigFileLexer.h"
#include "Misc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
RegistryKey::RegistryKey():mKeyValPairs(NULL),key(NULL)
{}


void
RegistryKey::init(HKEY base, const TCHAR* keyName )
{
	if( RegCreateKeyEx( base, keyName, 0, _T(""), 0,
                      KEY_QUERY_VALUE | KEY_SET_VALUE,
                      NULL, &key, NULL ) != ERROR_SUCCESS )
		key = NULL;

	mFileName = "";
	if (mKeyValPairs)
		delete mKeyValPairs;
}
void
RegistryKey::init(CString filename)
{
	if (mKeyValPairs)
		delete mKeyValPairs;
	key = NULL;
	mKeyValPairs = new CMapStringToString;
	mFileName = filename;
}

RegistryKey::RegistryKey( HKEY base, const TCHAR* keyName )
	: mKeyValPairs(NULL),key(NULL), m_keyname(keyName)
{
  if( RegCreateKeyEx( base, keyName, 0, _T(""), 0,
                      KEY_QUERY_VALUE | KEY_SET_VALUE,
                      NULL, &key, NULL ) != ERROR_SUCCESS )
		key = NULL;
	mFileName = "";
}
RegistryKey::RegistryKey( CString filename ) : mFileName(filename)
	,mKeyValPairs(NULL),key(NULL)
{
	mKeyValPairs = new CMapStringToString;
}

RegistryKey::~RegistryKey()
{
	if( key != NULL ) {
		RegCloseKey( key );
		key = NULL;
	}
	if (mKeyValPairs)
		delete mKeyValPairs;
}

BOOL RegistryKey::Copy(const RegistryKey & src) {
    POSITION pos;
    CString key;
    CString val;
	CString data;
	AutoBuf buf(1000);	
	CStringList slist;
    for( pos = src.mKeyValPairs->GetStartPosition(); pos != NULL; ) {
        src.mKeyValPairs->GetNextAssoc(pos, key, val);
#pragma hack // this needs to go away before release
// Commented out 3/5/06
//		if ("L " == val.Left(2) || "S " == val.Left(2)) // old "L ", "S " stuff
//			val = val.Mid(2);
		mKeyValPairs->SetAt(key, val);
	}
	return TRUE;
}
BOOL RegistryKey::WriteFile()
{
	if (mFileName == "") return FALSE;

    POSITION pos;
    CString key;
    CString val;
	CString data;
	AutoBuf buf(1000);	
	CStringList slist;
    for( pos = mKeyValPairs->GetStartPosition(); pos != NULL; ) {
        mKeyValPairs->GetNextAssoc(pos, key, val);
		String::insertSort(slist, key);
	}

    for (pos = slist.GetHeadPosition(); pos != NULL; ) {
        CString key = slist.GetAt(pos);
		if (mKeyValPairs->Lookup(key, val)) {
			if (key.GetLength() && val.GetLength()) {
				sprintf(buf.p, "%s: %s\r\n", key, val);
				data += buf.p;
			}
		}
		slist.GetNext(pos);
	}


//    for( pos = mKeyValPairs->GetStartPosition(); pos != NULL; ) {
//        mKeyValPairs->GetNextAssoc(pos, key, val);
//        if (key.GetLength() && val.GetLength()) {
//            sprintf(buf.p, "%s: %s\r\n", key, val);
//			data += buf.p;
//        }
//    }
	if (data == "") return FALSE;

	CFile cfile;
	CFileException e;
	if (cfile.Open(mFileName, 
		CFile::modeCreate
		//        |CFile::modeNoTruncate
		|CFile::modeWrite
		//        |CFile::typeText
		|CFile::shareDenyNone,
		&e) == FALSE) {
		return FALSE;
	}

	cfile.Write(data, data.GetLength());
	cfile.Flush();
	cfile.Close();
	return TRUE;


}
BOOL RegistryKey::ReadFile()
{
	if (mFileName == "") return FALSE;

	stack<KVPair> kvstack;
	ConfigFileParser(mFileName, &kvstack);
	char * pkey,*pval;

	while(!kvstack.empty()) {
		pkey = kvstack.top().key();
		pval = kvstack.top().val();
		mKeyValPairs->SetAt(pkey,pval);
		kvstack.pop();
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////
// Character strings
///////////////////////////////////////////////////////////////////////
void RegistryKey::Read( const TCHAR* value, TCHAR* data,
					   unsigned long maxSize, const TCHAR* deflt ) const
{

	if (key != NULL) {
		DWORD byteSize = maxSize * sizeof( TCHAR );
		if( !ReadData( value, data, byteSize, REG_SZ ) )
		_tcsnccpy( data, deflt, maxSize );
		// Ensure null termination
	} else {
		CString key(value);
	    CString val;
		if (mKeyValPairs->Lookup(key, val) == 0) { // not found
			val = deflt;
		} else {
			// just to remain backwards compatible with inhouse versions only
			// can go prior to release
#pragma hack
// COmmented out 3/5/06
//			if (val.Left(2) == "L " || val.Left(2) == "S ")
//				val = val.Right(val.GetLength()-2);
		}
		_tcsnccpy( data, val.GetBuffer(0), maxSize );

	}
	data[ maxSize - 1 ] = 0;
}
CString RegistryKey::ReadCString(const CString & key, const CString & dflt) {
	AutoBuf buf(1000);
	Read(
		(const TCHAR*) key,
		buf.p,
		999,
		(const TCHAR*) dflt);

	CString v = buf.p;
	return v;
}

void RegistryKey::Write( const TCHAR* value, const TCHAR* data ) const
{
	if (key != NULL) {
  // Take \0 into account
		WriteData( value, data, ( _tcslen( data ) + 1 ) * sizeof( TCHAR ),
			REG_SZ );
	} else {
		CString val;//("S ");
		val += data;
		mKeyValPairs->SetAt(value, val);
	}

}

TEST(Registry, FileWrite)
{
	RegistryKey rk("..\\testdata\\registry.dat");
	rk.Write("key1","val1");
	rk.Write("key11","val1.1");
	rk.Write("key12","val1.2");
	rk.Write("key13","X val1.3");
	rk.Write("key14","104");
	rk.Write("key2", 100);
	rk.Write("key9", 900);
	rk.Write("key8", 800);
	rk.Write("key7", 700);
	rk.Write("key3", 300);
	rk.Write("key4", 400);
	rk.Write("key5", 500);
	rk.Write("key39", 39);
	rk.Write("Colorrgb", "100,100,100");
	BOOL r = rk.WriteFile();
	CHECK(r == TRUE);

	RegistryKey rk2("..\\testdata\\registry.dat");
	r = rk2.ReadFile();
	CHECK(r == TRUE);

	TCHAR data[100];
	rk2.Read("key1", (TCHAR*) &data, (unsigned long) 100, (const TCHAR*)"default");
	unsigned long val2 = rk2.Read((const TCHAR*)"key2", (unsigned long)999);
	CString val(data);
	CHECK(val == "val1");
	CHECK(val2 == 100);
	int v = rk2.Read("key39", 1);
	CHECK(39 == v);

	rk2.Read("key11", (TCHAR*) &data, (unsigned long) 100, (const TCHAR*)"default");
	val = data;
	CHECK(val == "val1.1");

	rk2.Read("key12", (TCHAR*) &data, (unsigned long) 100, (const TCHAR*)"default");
	val = data;
	CHECK(val == "val1.2");

	rk2.Read("key13", (TCHAR*) &data, (unsigned long) 100, (const TCHAR*)"default");
	val = data;
	CHECK(val == "X val1.3");

	val2 = rk2.Read((const TCHAR*)"key14", (unsigned long)999);
	CHECK(val2 == 104);

	val2 = rk2.Read((const TCHAR*)"Colorrgb", (unsigned long)999);
	CHECK(6579300 == val2);
}

///////////////////////////////////////////////////////////////////////
// long ints
///////////////////////////////////////////////////////////////////////
unsigned long RegistryKey::Read( const TCHAR* value, unsigned long deflt ) const
{
	unsigned long retVal;
	unsigned long data;
	unsigned long size = sizeof( data );

	if (key != NULL) {
		if( !ReadData( value, &data, size, REG_DWORD ) )
			retVal = deflt;
		else
			retVal = data;
	} else {
		CString key(value);
		CString val;
		if (mKeyValPairs->Lookup(key, val) == 0) { // not found
			val = numToString(deflt);
		} else {
			// just to remain backwards compatible with inhouse versions only
			// can go prior to release
#pragma hack
// COmmented out 3/5/06
//			if (val.Left(2) == "L " || val.Left(2) == "S ")
//				val = val.Right(val.GetLength()-2);

			// if it's a key containing "Color" and the value has 2
			// commas surrounded by non white space, it must be an rgb! Argh!
			if (key.Find("Color") > -1
				&& MBUtil::RgbTriple((LPCTSTR)val,retVal))
				return retVal;
		}
		retVal = atol(val.GetBuffer(0));
	}

	return retVal;
}

void RegistryKey::Write( const TCHAR* value, unsigned long data ) const
{
	if (key != NULL) {
		WriteData( value, &data, sizeof( data ), REG_DWORD );
	} else {
		CString val;//("L ");
		val += numToString(data);
		mKeyValPairs->SetAt(value, val);
	}
}
///////////////////////////////////////////////////////////////////////
// binary data
///////////////////////////////////////////////////////////////////////
void RegistryKey::Read( const TCHAR* value, void* data,
					   unsigned long desiredSize, const void* deflt ) const
{
  unsigned long size = desiredSize;
  if( !ReadData( value, data, size, REG_BINARY ) || size != desiredSize )
    CopyMemory( data, deflt, desiredSize );
//  CString msg = CString("Registry read  ") + CString(value) + CString(": BIN");
//  logger.log(msg);
}

void RegistryKey::Write( const TCHAR* value, const void* data,
						unsigned long size ) const
{
  WriteData( value, data, size, REG_BINARY );
//  CString msg = CString("Registry write ") + value + CString(": BIN");
//  logger.log(msg);
}

///////////////////////////////////////////////////////////////////////
// privates
///////////////////////////////////////////////////////////////////////
bool RegistryKey::ReadData( const TCHAR* value, void* data, 
	unsigned long& size, unsigned long desiredType ) const
{
  if( key == NULL )
    return false;

  unsigned long type;
  return RegQueryValueEx( key, value, 0, &type,
                          static_cast< BYTE* >( data ),
                          &size ) == ERROR_SUCCESS &&
         type == desiredType;
}

bool RegistryKey::WriteData( const TCHAR* value, const void* data,
							unsigned long size, unsigned long type ) const
{
  if( key == NULL )
    return false;

  return RegSetValueEx( key, value, 0, type,
                        static_cast< CONST BYTE* >( data ),
                        size ) == ERROR_SUCCESS;
}
