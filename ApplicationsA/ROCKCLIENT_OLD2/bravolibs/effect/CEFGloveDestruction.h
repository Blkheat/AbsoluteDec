//...............................................................................................................
// 
//	CEFGloveDestruction.h														 모아 베기 공격
//
//...............................................................................................................
#ifndef __CEFGloveDestruction_H
#define __CEFGloveDestruction_H

#include "CEffectBase.h"

class CEFGloveDestruction : public CEffectBase
{
public:
	 
	         CEFGloveDestruction();
	virtual ~CEFGloveDestruction();

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