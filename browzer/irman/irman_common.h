/******************************************************************************

  Common file - exists mainly to facilitate precompiled headers

  Needs to be #included first in any source file, and the project set up
  to use this for precompiler headers

                      Copyright 1999 Gavin Smyth
                 http://www.beesknees.freeserve.co.uk

******************************************************************************/

#ifndef _h_common_h_
#define _h_common_h_

//#define STRICT
//#include "stdafx.h"
#include <tchar.h>

// Note: it'd be nice to use STL features - for example, strings - however,
// strings are not thread safe in the MS STL implementation, so avoid all
// potential problems by doing allocationns manually.

// Most Win32 functions return FALSE on failure, and the precise error may
// be found via GetLastError().

inline BOOL Verify( BOOL expr )
{
# ifdef _DEBUG
    if( !expr )
    {
      TCHAR msgBuf[ 512 ];
      FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                     NULL, GetLastError(), 0, msgBuf, sizeof( msgBuf ) / sizeof( TCHAR ), NULL );
      MessageBox( NULL, msgBuf, _T("Irman Error"), MB_OK | MB_ICONINFORMATION );
    }
# endif
  return expr;
}

#endif // _h_common_h_
