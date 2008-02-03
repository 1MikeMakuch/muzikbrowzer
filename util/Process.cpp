/*****************************************************************************
*           Change Log
*  Date     | Change
*-----------+-----------------------------------------------------------------
*  2-Sep-01 | Created
*****************************************************************************/

#include "stdafx.h"
#include "process.h"
#include "MyLog.h"

#define dim(x) (sizeof(x) / sizeof((x)[0]))

IMPLEMENT_MSG(UPM_FINISHED)
IMPLEMENT_MSG(UPM_LINE)
IMPLEMENT_MSG(UPM_PROCESS_HANDLE)

/****************************************************************************
*                                Process::run
* Result: BOOL
*       TRUE if the process started successfully
*	FALSE if there was an error (use ::GetLastError to get reason)
* Effect: 
*       Creates a child process, redirects stdout/stderr to the output
*	stream. Does not redirect stdin.
****************************************************************************/

BOOL Process::run()
    {
	logger.ods("Process::run begin");
     hreadFromChild = NULL;
     HANDLE hwriteToParent = NULL;
     HANDLE hwriteToParent2 = NULL;

     SECURITY_ATTRIBUTES sa = {sizeof(SECURITY_ATTRIBUTES), NULL, TRUE }; // inheritable handle

     if(!::CreatePipe(&hreadFromChild, &hwriteToParent, &sa, 0))
	{ /* pipe failed */
	 // ::GetLastError() will reveal the cause
	 delete this;
	 return FALSE;
	} /* pipe failed */

     if(!::DuplicateHandle(GetCurrentProcess(),     // duplicate from this process
			   hwriteToParent,	    // this handle 
			   GetCurrentProcess(),     // into this process
			   &hwriteToParent2,        // as this handle
			   0,			    // no access flags (subsumed by DUPLICATE_SAME_ACCESS)
			   TRUE,	            // create inheritable
			   DUPLICATE_SAME_ACCESS))  // create duplicate access
	{ /* duplicate failed */
	 DWORD err = ::GetLastError();
	 ::CloseHandle(hreadFromChild);
	 ::CloseHandle(hwriteToParent);
	 ::SetLastError(err);
	 delete this;
	 return FALSE;
	} /* duplicate failed */

     STARTUPINFO startup;
     PROCESS_INFORMATION procinfo;

     ::ZeroMemory(&startup, sizeof(startup));

     startup.cb = sizeof(startup);
     startup.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
     startup.wShowWindow = SW_HIDE; // hidden console window
     startup.hStdInput = NULL; // not used
     startup.hStdOutput = hwriteToParent;
     startup.hStdError = hwriteToParent2;

     // We want a non-inherited read handle. DuplicateHandle with a
     // NULL target fixes the read side to be non-inheritable
     ::DuplicateHandle(::GetCurrentProcess(),    // in this process
		       hreadFromChild,           // child read handle
		       ::GetCurrentProcess(),    // to this process
		       NULL,                     // modify existing handle
		       0,                        // flags
		       FALSE,                    // not inheritable
		       DUPLICATE_SAME_ACCESS);   // same handle access

     // We need a writeable buffer for the command (silly Windows restriction)
     LPTSTR cmd = command.GetBuffer(command.GetLength() + 1);

     BOOL started = ::CreateProcess(NULL,        // command is part of input string
				    cmd,         // (writeable) command string
				    NULL,        // process security
				    NULL,        // thread security
				    TRUE,        // inherit handles flag
				    0,           // flags
				    NULL,        // inherit environment
				    NULL,        // inherit directory
				    &startup,    // STARTUPINFO
				    &procinfo);  // PROCESS_INFORMATION
     command.ReleaseBuffer();

     if(!started)
	{ /* failed to start */
	 DWORD err = ::GetLastError(); // preserve across CloseHandle calls
	 ::CloseHandle(hreadFromChild);
	 ::CloseHandle(hwriteToParent);
	 ::CloseHandle(hwriteToParent2);
	 ::SetLastError(err);
	 target->PostMessage(UPM_FINISHED, (WPARAM)err, (LPARAM)pid);
	 delete this;
	 return FALSE;
	} /* failed to start */
	 logger.logd("Process.Run()");

     target->PostMessage(UPM_PROCESS_HANDLE, (WPARAM)procinfo.hProcess, (LPARAM)pid);
     
     // Now close the output pipes so we get true EOF/broken pipe
     ::CloseHandle(hwriteToParent);
     ::CloseHandle(hwriteToParent2);
     
     // We have to create a listener thread. We create a worker
     // thread that handles this
     CWinThread * thread = AfxBeginThread(listener, (LPVOID)this);
     if(thread == NULL)
	{ /* failed */
	 DWORD err = ::GetLastError();
	 target->PostMessage(UPM_FINISHED, (WPARAM)err, (LPARAM)pid);
	 ::CloseHandle(hreadFromChild);
	 ::CloseHandle(hwriteToParent);
	 ::CloseHandle(hwriteToParent2);
	 delete this;
	 return FALSE;
	} /* failed */
	 logger.ods("Process::run done");
     return TRUE;
    } // Process::run

/****************************************************************************
*                             Process::listener
* Inputs:
*       LPVOID me: (LPVOID)(CProcess *)
* Result: UINT
*       0, always
* Effect: 
*       Maps back to the C++ space to run the listener thread
****************************************************************************/

UINT Process::listener(LPVOID me)
    {
     ((Process *)me)->listener();
     return 0;
    } // Process::listener

/****************************************************************************
*                             Process::listener
* Result: void
*       
* Effect: 
*       Reads input lines from the child process
****************************************************************************/
#define MAX_LINE_LENGTH 10240

void Process::listener()
    {
//	logger.ods("Process::listener begin");
     TCHAR buffer[MAX_LINE_LENGTH + 1];

     CString * line;
     line = new CString;

     DWORD bytesRead;
     
     while(::ReadFile(hreadFromChild, buffer, sizeof(buffer) - sizeof(TCHAR), &bytesRead, NULL))
	{ /* got data */
	 if(bytesRead == 0)
	    break; // EOF condition
	 
	 buffer[bytesRead] = _T('\0');
	 // Convert to lines
	 LPTSTR b = buffer;
	 while(TRUE)
	    { /* convert and send */
	     LPTSTR p = _tcschr(b, _T('\n'));
	     if(p == NULL)
		{ /* incomplete line */
		 *line += b;
		 break; // leave assembly loop
		} /* incomplete line */
	     else
		{ /* complete line */
		 int offset = 0;
		 if(p - b > 0)
		    { /* get rid of \r */
		     if(p[-1] == _T('\r'))
			offset = 1;
		    } /* get rid of \r */
		 *line += CString(b, (p - b) - offset);
		 target->PostMessage(UPM_LINE, (WPARAM)line, (LPARAM)pid);
		 b = p + 1;
		 line = new CString;
		} /* complete line */
	    } /* convert and send */
	} /* got data */

     DWORD err = ::GetLastError();

     ::CloseHandle(hreadFromChild);


     if(line->GetLength() > 0)
	target->PostMessage(UPM_LINE, (WPARAM)line, (LPARAM)pid);
     else
	delete line;
     
     DWORD status = 0;
     if(err != ERROR_BROKEN_PIPE)
	status = err;
     
     target->PostMessage(UPM_FINISHED, status, (LPARAM)pid);

     delete this;
//	 logger.ods("Process::listener done");
    } // Process::listener
