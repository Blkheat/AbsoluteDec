#ifndef __CLOGODISPLAYTHREAD_H
#define __CLOGODISPLAYTHREAD_H

#include "AThread.h"

class CLogoDisplayThread : public AThread  
{
public:	
	CLogoDisplayThread();
	virtual ~CLogoDisplayThread();
	//overwritten abstract method
	unsigned ThreadFunction();
};

#endif