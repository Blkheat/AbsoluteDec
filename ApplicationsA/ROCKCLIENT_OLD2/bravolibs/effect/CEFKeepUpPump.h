//................................................................................................................
//
//  소스에 이펙트 붙는거 전부 처리
//
//................................................................................................................
#ifndef __CEFKEEPUPPUMP_H
#define __CEFKEEPUPPUMP_H

#include "CEffectBase.h"

class CEFKeepUpPump: public CEffectBase
{
public:
	 
	         CEFKeepUpPump();
	virtual ~CEFKeepUpPump();

	void	 SetTimer(float Timer) { m_MaxTimer = Timer; }

public:

	virtual void	 Init(void);
	int	Update(float dtime);

private:

	float	m_NowTimer;
	float   m_MaxTimer;

};


#endif