#ifndef __CEFPriestStandAttack_H
#define __CEFPriestStandAttack_H

#include "CEffectBase.h"

class CEFPriestStandAttack : public CEffectBase
{
public:
	
	         CEFPriestStandAttack();
	virtual ~CEFPriestStandAttack();

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