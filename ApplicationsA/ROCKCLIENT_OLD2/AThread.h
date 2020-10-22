/*
	IMPORTANT: please select in the project options a
	multithreaded run-time library. If you use a single-threaded library
	the compiler will not know the function _beginthreadex()
	
	(click Settings on the Project menu. Then click the C/C++ tab, and click Code Generation
	in the Category box. See the Use Run-Time Library drop-down box. 
*/


#if !defined(AFX_ATHREAD_H__67DF0E5F_A726_4C7D_8F3D_AFFAC186DA52__INCLUDED_)
#define AFX_ATHREAD_H__67DF0E5F_A726_4C7D_8F3D_AFFAC186DA52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include <process.h> //_beginthreadex, _endthreadex

class AThread  
{
public:
	DWORD getExitCode();
	
	AThread();
	virtual ~AThread();
	virtual bool Start();
	bool isRunning(DWORD dwTimeOut = 50) const;
	bool Stop();
	DWORD getThreadID() const;

protected:
	virtual unsigned ThreadFunction() = 0;
	static unsigned __stdcall ThreadStarter(void* PtrToInstance);
	bool isStopEventSignaled(DWORD dwTimeOut = 10) const;

//attributes
protected:
	unsigned m_ThreadID; //holds the threadID
	HANDLE m_hThread; //holds the HANDLE of the created thread
	HANDLE m_hStopEvent; //holds the handle of the event-object to signal
						//the thread that he has to stop	
};

#endif // !defined(AFX_ATHREAD_H__67DF0E5F_A726_4C7D_8F3D_AFFAC186DA52__INCLUDED_)
