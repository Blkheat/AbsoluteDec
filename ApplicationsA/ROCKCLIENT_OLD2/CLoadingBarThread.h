#ifndef __CLOADINGBARTHREAD_H
#define __CLOADINGBARTHREAD_H

#include "AThread.h"

enum NLoadingBarStep
{
	nLoadingNone = 0,
	nLoadingStart,
	nLoadingSFadeIn,
	nLoadingSFadeOut,
	nLoadingEFadeIn,	
	nLoadingEnd,
};

class CLoadingBarThread : public AThread  
{

public:
	
	CLoadingBarThread();
	virtual ~CLoadingBarThread();

	//overwritten abstract method
	unsigned ThreadFunction();

	NLoadingBarStep m_nNowStep;
};

extern CLoadingBarThread g_CLoadingBarThread;

#endif