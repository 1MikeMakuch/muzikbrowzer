#include "stdafx.h"
#include "resource.h"

/****************************************************************************
*                                   cvError
* Inputs:
*       DWORD err: Error code
* Result: CString
*       Printable error code
****************************************************************************/

CString cvError(DWORD err)
    {
     LPTSTR s;
     if(::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM,
			NULL,
			err,
			0,
			(LPTSTR)&s,
			0,
			NULL) == 0)
        { /* failed */
	 // See if it is a known error code
	 CString fmt;                          
	 fmt.LoadString(IDS_UNKNOWN_ERROR);    
	 CString t;
	 t.Format(fmt, err, err);
	 return t;
	} /* failed */
     else
        { /* success */
	 LPTSTR p = _tcschr(s, _T('\r'));
	 if(p != NULL)
	    { /* lose CRLF */
	     *p = _T('\0');
	    } /* lose CRLF */

	 CString Error = s; // copy to a CString
	 ::LocalFree(s);
	 return Error;
	} /* success */
    } // cvError
