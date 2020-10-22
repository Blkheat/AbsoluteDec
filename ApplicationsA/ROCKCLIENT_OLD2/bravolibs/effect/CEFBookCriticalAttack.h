//...............................................................................................................
// 
//	CEFBookCriticalAttack.h														 책 기본 공격 ( 크리티컬 )
//
//...............................................................................................................
#ifndef __CEFBOOKCRITICALATTACK_H
#define __CEFBOOKCRITICALATTACK_H

#include "CEffectBase.h"

class CEFBookCriticalAttack : public CEffectBase
{
public:
	 
	         CEFBookCriticalAttack();
	virtual ~CEFBookCriticalAttack();

public:

	virtual int		 CheckFrame(void);
	virtual int		 Render(float dtime);							
	virtual int      Update(float dtime);
	virtual int      SetAfterEffect(void);

	virtual void	 Init(void);

private:

	DWORD			 m_OldTimer[2];
	BOOL			 m_ISDisplay[2];
	D3DXVECTOR3		 m_vStartPos[2];
	CParticleSystem	 m_BallParticle[2];
	CParticleSystem	 m_CenterBallParticle[2];

	DWORD			 m_RangeTime[2];
};

#endif


