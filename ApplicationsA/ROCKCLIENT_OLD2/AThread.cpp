// AThread.cpp: 
//
//////////////////////////////////////////////////////////////////////
#include "AThread.h" 
//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

  
AThread::AThread() :
	m_ThreadID(-1),
	m_hThread(NULL),
	m_hStopEvent(NULL)
{
	//create the event-object
	m_hStopEvent = ::CreateEvent( NULL   /*security attr*/,
								  FALSE  /* manual-reset*/, 
								  FALSE  /*initial state*/, 
								  NULL   /*event-object name*/); 
}

AThread::~AThread()
{
	//make sure that the thread is not running
	Stop();
	
	//cleanup the event-object
	if (m_hStopEvent != NULL) 
		::CloseHandle(m_hStopEvent); 

	if (m_hThread != NULL)
		::CloseHandle(m_hThread);
}

/*
	Start()

	purpose: call this method to start the thread execution

	param: none

	return value: "true", if the thread could be started, else "false"
*/

bool AThread::Start()
{
	//check if thread is already running
	if (isRunning()) 
		return true;

	if (m_hStopEvent == NULL)
		return false;

	if (m_hThread != NULL)
		::CloseHandle(m_hThread);
	
	m_hThread = (HANDLE)_beginthreadex(NULL/*security*/,
										0/*stacksize*/,
										ThreadStarter/*adress of thread function*/,
										this /*pass ptr to this object as argument*/,
										0/*initflag*/,
										&m_ThreadID/*variable that receives the thread identifier*/);
	if (m_hThread == 0) {
		//failure
		m_ThreadID = -1;
		return false;
	}
	
	return true;
}

/*
	Stop()

	purpose: call this method to stop the thread execution

	param: none

	return value: "true", if the thread could be stopped, else "false"
*/

bool AThread::Stop()
{
	//first check if the thread is running
	if (isRunning() == false)
		return true;

	if (m_hStopEvent != NULL) {
		//trigger the event to signal the thread to come to an end
		if (::SetEvent(m_hStopEvent) == 0) {
			return false;
		}
		::Sleep(0); //give control to other threads
		::WaitForSingleObject(m_hThread, INFINITE); //wait until thread has stopped
		m_ThreadID = -1;
	}
	else 
	{
		return false; //m_hStopEvent == NULL -> ERROR
	}

	return true;
}

/*
	ThreadStarter()

	purpose:	this static method is needed cause the _beginthreadex() function
				wants to have a ptr to a function - therefore you cannot pass
				a ptr to a member function (cause of having implicit this-pointer)
				therefore i introduced this static function that just delegates the call to the
				member function ThreadFunction()


	param: 
		PtrToInstance: holds the ptr to the object-instance to be able to call ThreadFuction() of the
						object

	return value: exit code of thread function
*/

unsigned __stdcall AThread::ThreadStarter(void* PtrToInstance)
{
	AThread *pInstance = reinterpret_cast<AThread*>(PtrToInstance);
	
	unsigned iRet = pInstance->ThreadFunction();	
	
	_endthreadex(iRet/*RetCode*/);
	return iRet;
}



/*
	isStopEventSignaled()

	purpose: call this method to check if the thread function should stop.

	param: none

	return value: "true" if the thread should stop, "false" if the thread can go on
*/

bool AThread::isStopEventSignaled(DWORD dwTimeOut) const
{
	//check if the stop-event object is signaled using the
	//passed-in timeout value
	if (::WaitForSingleObject(m_hStopEvent, dwTimeOut)==WAIT_OBJECT_0 ) 
	{
		return true;
	}
	else
	{
		return false;
	}
}


/*
	isRunning()

	purpose: call this method to check if the thread function is running

	param:
		dwTimeOut: timeout in ms

	return value: "true" if the thread is running, otherwise "false"
*/

bool AThread::isRunning(DWORD dwTimeOut) const
{
	//INFO: thread-handle state is set to nonsignaled while the thread is running
	//and set to signaled when the thread terminates
	if (::WaitForSingleObject(m_hThread, dwTimeOut/*ms*/) == WAIT_TIMEOUT) 
		return true; //the wait timed out -> thread handle is not signaled -> thread is running
	else 
		return false;
}

/*
	getThreadID()

	purpose: call this method to get the thread ID, returns -1 if the thread is not started

	param: none

	return value: the thread id or -1 if the thread is not started
*/

DWORD AThread::getThreadID() const
{
	return m_ThreadID;
}


/*
	getExitCode()

	purpose: call this method to get the exit code of the thread. If the thread is still
				running, the value STILL_ACTIVE will be returned

	param: none

	return value: the thread id or -1 if the thread is not started
*/

DWORD AThread::getExitCode()
{
	if( m_hThread == NULL)	return 0;
	DWORD dwExitCode= 0 ;
	BOOL bSuc = ::GetExitCodeThread(m_hThread, &dwExitCode);
	return dwExitCode;
}
