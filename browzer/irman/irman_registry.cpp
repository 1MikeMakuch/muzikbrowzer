/******************************************************************************

  Yet another registry access class

                      Copyright 1999 Gavin Smyth
                 http://www.beesknees.freeserve.co.uk

******************************************************************************/

#include "StdAfx.h"

#include "irman_common.h"
#include "irman_registry.h"
#include "MyString.h"
#include "MyLog.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

RegistryKey::RegistryKey( HKEY base, const TCHAR* keyName )
{
  if( RegCreateKeyEx( base, keyName, 0, _T(""), 0,
                      KEY_QUERY_VALUE | KEY_SET_VALUE,
                      NULL, &key, NULL ) != ERROR_SUCCESS )
    key = NULL;
//  CString msg = CString("Registry ") + keyName;
//  logger.log(msg);
}

RegistryKey::~RegistryKey()
{
  if( key != NULL )
  {
    RegCloseKey( key );
    key = NULL;
  }
}
///////////////////////////////////////////////////////////////////////
// Character strings
///////////////////////////////////////////////////////////////////////
void RegistryKey::Read( const TCHAR* value, TCHAR* data,
					   unsigned long maxSize, const TCHAR* deflt ) const
{
  DWORD byteSize = maxSize * sizeof( TCHAR );
  if( !ReadData( value, data, byteSize, REG_SZ ) )
    _tcsnccpy( data, deflt, maxSize );
  // Ensure null termination
  data[ maxSize - 1 ] = 0;
//  CString msg = CString("Registry read  ") + value + CString(": ") + data;
//  logger.log(msg);
}

void RegistryKey::Write( const TCHAR* value, const TCHAR* data ) const
{
  // Take \0 into account
  WriteData( value, data, ( _tcslen( data ) + 1 ) * sizeof( TCHAR ),
	  REG_SZ );
//  CString msg = CString("Registry write ") + value + CString(": ") + data;
//  logger.log(msg);
}
///////////////////////////////////////////////////////////////////////
// long ints
///////////////////////////////////////////////////////////////////////
unsigned long RegistryKey::Read( const TCHAR* value, unsigned long deflt ) const
{
  unsigned long retVal;
  unsigned long data;
  unsigned long size = sizeof( data );
  if( !ReadData( value, &data, size, REG_DWORD ) )
    retVal = deflt;
  else
    retVal = data;
//  CString msg = CString("Registry read  ") + CString(value) + CString(": ");
//  msg += retVal;
//  logger.log(msg);
  return retVal;
}

void RegistryKey::Write( const TCHAR* value, unsigned long data ) const
{
  WriteData( value, &data, sizeof( data ), REG_DWORD );
//  CString msg = CString("Registry write ") + value + CString(": ");
//  msg += data;
//  logger.log(msg);

}
///////////////////////////////////////////////////////////////////////
// binary data
///////////////////////////////////////////////////////////////////////
void RegistryKey::Read( const TCHAR* value, void* data,
					   unsigned long desiredSize, const void* deflt ) const
{
  unsigned long size = desiredSize;
  if( !ReadData( value, data, size, REG_BINARY ) && size == desiredSize )
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
bool RegistryKey::ReadData( const TCHAR* value, void* data, unsigned long& size, unsigned long desiredType ) const
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
