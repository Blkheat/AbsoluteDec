#include "RockPCH.h"

#include "effect.h"
#include "CNetProcThread.h"
#include "RockClient.h"

CNetProcThread * g_pCNetProcThread = NULL;

CNetProcThread::CNetProcThread()
{
	
}

CNetProcThread::~CNetProcThread()
{

}

unsigned CNetProcThread::ThreadFunction()
{
	//while loop will execute till the stop-event
	//is signaled	

	while(isStopEventSignaled() == false) 
	{
		///--g_RockClient.NetworkProc();		
		Sleep(10);
	}
	
	return 0; //return exit-code of thread
}
