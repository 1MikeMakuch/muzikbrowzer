
#include "StdAfx.h"
#include <io.h>
#include <fcntl.h>

#include "Misc.h"
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

BOOL fileIsReadable(const CString & file) {
	int fd;
	if ((fd = _open(file, _O_RDONLY)) == -1) {
		return FALSE;
	}
	_close(fd);
	return TRUE;
}
