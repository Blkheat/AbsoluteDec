//...............................................................................................................
// 
//	CEFGatherCut.h														 모아 베기 공격
//
//...............................................................................................................
#ifndef __CEFGATHERCUT_H
#define __CEFGATHERCUT_H

#include "CEffectBase.h"

class CEFGatherCut : public CEffectBase
{
public:
	 
	         CEFGatherCut();
	virtual ~CEFGatherCut();

public:

	virtual int		 CheckFrame(void);
	virtual int		 Render(float dtime);							
	virtual int      Update(float dtime);
	virtual int      SetAfterEffect(void);

	virtual void	 Init(void);

private:

	CParticleSystem	 m_SwordParticle;

	DWORD			 m_OldTimer;
};

#endif