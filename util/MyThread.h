#ifndef _MY_MYTHREAD_CLASS_H_
#define _MY_MYTHREAD_CLASS_H_


class MyThreadClass
{
public:
	MyThreadClass(){}
	virtual ~MyThreadClass(){Kill();}

//Thread Management
	BOOL CreateNewThread();
	BOOL Wait(); //Wait for thread to end
	BOOL Suspend(); //Suspend the thread
	BOOL Resume(); //Resume a suspended thread
	BOOL Kill(); //Terminate a thread
	BOOL IsActive(); //Check for activity
	BOOL CloseHandle();

//override these functions in the derived class
	virtual void ThreadEntry(){ }
	virtual void ThreadExit(){ }
	virtual void Run(){ }

//a friend
	friend DWORD  WINAPI _ThreadFunc(LPVOID  pvThread);

protected:
	HANDLE m_hThread; //Thread handle
	BOOL m_bActive; //activity indicator
	DWORD m_lpId; //Thread ID
};



#endif
