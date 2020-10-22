#ifndef __CITEMTEXLOADTHREAD_H
#define __CITEMTEXLOADTHREAD_H

#include "AThread.h"

class CItemTexLoadThread : public AThread  
{

public:
	
	CItemTexLoadThread();
	virtual ~CItemTexLoadThread();

	//overwritten abstract method
	unsigned ThreadFunction();

	virtual bool Start();
};

extern CItemTexLoadThread g_CItemTexLoadThread;

#endif








