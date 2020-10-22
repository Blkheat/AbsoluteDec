#ifndef __CEFEnergy_H
#define __CEFEnergy_H

#include "CEffectBase.h"
#include "CFloatSequence.h"

class CEFEnergy : public CEffectBase
{
public:
	
	         CEFEnergy();
	virtual ~CEFEnergy();

public:

	virtual int		 Render(float dtime);							
	virtual int      Update(float dtime);
	virtual int      SetAfterEffect(void);
	virtual void	 Init(void);

private:

	DWORD			 m_OldTimer;
	D3DXVECTOR3		 m_vStartPos;
	CFloatSequence   m_Range;
	CParticleSystem	 m_FireParticle;
	int				 m_Head;
	int				 m_Tail;
};

#endif

