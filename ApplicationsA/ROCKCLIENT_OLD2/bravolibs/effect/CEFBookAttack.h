//...............................................................................................................
// 
//	CEFBookAttack.h														 책 기본 공격
//
//...............................................................................................................
#ifndef __CEFBOOKATTACK_H
#define __CEFBOOKATTACK_H

#include "CEffectBase.h"

class CEFBookAttack : public CEffectBase
{
public:
	 
	         CEFBookAttack();
	virtual ~CEFBookAttack();

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


