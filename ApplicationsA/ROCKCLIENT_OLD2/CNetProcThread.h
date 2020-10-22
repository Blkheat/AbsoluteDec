#ifndef __CNETPROCTHREAD_H
#define __CNETPROCTHREAD_H

#include "AThread.h"

class CNetProcThread : public AThread  
{

public:
	
	CNetProcThread();
	virtual ~CNetProcThread();

	//overwritten abstract method
	unsigned ThreadFunction();	
};

extern CNetProcThread * g_pCNetProcThread;

#endif