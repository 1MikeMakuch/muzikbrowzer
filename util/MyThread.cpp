#include "stdafx.h"
#include "MyThread.h"

BOOL MyThreadClass::CreateNewThread()
{
m_hThread = CreateThread(
	NULL,
	0,
	(LPTHREAD_START_ROUTINE)_ThreadFunc,
	(LPVOID) this,
	CREATE_SUSPENDED,
	(LPDWORD) &m_lpId);

	  if(m_hThread == NULL)
		    return false;

	m_bActive = TRUE;
	return TRUE;
}
BOOL MyThreadClass::CloseHandle() {
	return ::CloseHandle(m_hThread);
}

BOOL MyThreadClass::Suspend()
{
	 m_bActive = false;
	  return(-1 != SuspendThread(m_hThread));//win32 API
}

BOOL MyThreadClass::Kill()
{
	 return TerminateThread(m_hThread, 1); //win32 API
}

BOOL MyThreadClass::Resume()
{
	 m_bActive = TRUE;
	  return(-1 != ResumeThread(m_hThread)); //win32 API
}

BOOL MyThreadClass::Wait()
{
	 return (WAIT_OBJECT_0
	          == WaitForSingleObject(m_hThread, INFINITE));
	  //win32 API
}

BOOL MyThreadClass::IsActive()
{
	 return m_bActive;
}

DWORD  WINAPI _ThreadFunc(LPVOID  pvThread) {
	MyThreadClass* pThread = (MyThreadClass*)pvThread;  //typecast

	pThread->ThreadEntry(); //initialize
	pThread->Run();
	pThread->ThreadExit(); //finalize
	return NULL;
}