//...............................................................................................................
// 
//	CEFNPCStickAttack.h														 지팡이 기본 공격
//
//...............................................................................................................
#ifndef __CEFNPCStickAttack_H
#define __CEFNPCStickAttack_H

#include "CEffectBase.h"

class CEFNPCStickAttack : public CEffectBase
{
public:
	 
	         CEFNPCStickAttack();
	virtual ~CEFNPCStickAttack();

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

	D3DXVECTOR3		 m_vAdd;
};

#endif


