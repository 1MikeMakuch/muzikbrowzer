class Process {
    public:
       Process() { }
       Process(const CString & cmd, CWnd * notify, UINT id = 0) {
			ASSERT(notify != NULL);
			command = cmd;
			target = notify;
			pid = id; 
	   }
       BOOL run();
    protected:
       CString command;
       CWnd * target;
       HANDLE hreadFromChild;
       static UINT listener(LPVOID me);
       void listener();
       UINT pid;   // process ID assigned by caller

};


/****************************************************************************
*                                UPM_FINISHED
* Inputs:
*       WPARAM: (WPARAM)(DWORD)::GetLastError()
*		0 if normal termination (including ERROR_BROKEN_PIPE)
*		::GetLastError() if other error
*	LPARAM: pid as established by constructor
* Result: LRESULT
*       Logically void, 0, always
* Effect: 
*       Sent to the parent process to notify it that the process has
*	terminated
****************************************************************************/

#define UPM_FINISHED_MSG _T("UPM_FINISHED-{D741F7E0-9F58-11d5-A043-006067718D04}")

/****************************************************************************
*                                  UPM_LINE
* Inputs:
*       WPARAM: (WPARAM)(CString *): Line to display
*	LPARAM: pid as established by constructor
* Result: LRESULT
*       Logically void, 0, always
* Effect: 
*       Notifies the target window that it should add the line to the
*	output display or otherwise process it
* Notes:
*	The CString is allocated in the process class and must be deleted
*	by the recipient when it is no longer required
****************************************************************************/

#define UPM_LINE_MSG _T("UPM_LINE-{D741F7E1-9F58-11d5-A043-006067718D04}")


/****************************************************************************
*                             UPM_PROCESS_HANDLE
* Inputs:
*       WPARAM: (WPARAM)(HANDLE) The process handle
*	LPARAM: pid established by constructor
* Result: LRESULT
*       Logically void, 0, always
* Effect: 
*       Notifies the parent that a process has been created, and passes
*	the process handle back
****************************************************************************/

#define UPM_PROCESS_HANDLE_MSG _T("UPM_PROCESS_HANDLE-{D741F7E2-9F58-11d5-A043-006067718D04}")


#ifndef IMPLEMENT_MSG
#define IMPLEMENT_MSG(x) static UINT x = ::RegisterWindowMessage(x##_MSG);
#endif

#define UPM_DIFF_DONE_MSG    _T(   "UPM_DIFF_DONE-{D741F7E2-9F58-11d5-A043-006067718D04}")
#define UPM_CSTAT_DONE_MSG   _T(  "UPM_CSTAT_DONE-{D741F7E2-9F58-11d5-A043-006067718D04}")
#define UPM_GETFILE_DONE_MSG _T("UPM_GETFILE_DONE-{D741F7E2-9F58-11d5-A043-006067718D04}")
#define UPM_VERIFY_DONE_MSG  _T( "UPM_VERIFY_DONE-{D741F7E2-9F58-11d5-A043-006067718D04}")



