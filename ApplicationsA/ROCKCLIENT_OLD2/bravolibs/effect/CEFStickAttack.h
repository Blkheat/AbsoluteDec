//...............................................................................................................
// 
//	CEFStickAttack.h														 지팡이 기본 공격
//
//...............................................................................................................
#ifndef __CEFSTICKATTACK_H
#define __CEFSTICKATTACK_H

#include "CEffectBase.h"

class CEFStickAttack : public CEffectBase
{
public:
	 
	         CEFStickAttack();
	virtual ~CEFStickAttack();

public:

	virtual int		 CheckFrame(void);
	virtual int		 Render(float dtime);							
	virtual int      Update(float dtime);
	virtual int      SetAfterEffect(void);

	virtual void	 Init(void);

private:

	CParticleSystem	 m_BallParticle;

	DWORD			 m_OldTimer;
	DWORD			 m_RangeTime;

	D3DXVECTOR3		 m_vStartPos;
};

#endif


