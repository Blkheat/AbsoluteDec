#ifndef __CEFNpcFireBall_H
#define __CEFNpcFireBall_H

#include "CEffectBase.h"

class CEFNpcFireBall : public CEffectBase
{
public:
	
	         CEFNpcFireBall();
	virtual ~CEFNpcFireBall();

public:

	virtual int		 Render(float dtime);							
	virtual int      Update(float dtime);
	virtual int      SetAfterEffect(void);

	virtual void	 Init(void);

private:

	CParticleSystem	 m_SpotParticle;

	DWORD			 m_OldTimer;
	DWORD			 m_RangeTime;

	D3DXVECTOR3		 m_vStartPos;
};

#endif