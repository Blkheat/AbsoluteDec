#ifndef __CEFBIGBUBBLE_H
#define __CEFBIGBUBBLE_H

#include "CEffectBase.h"

class CEFBigBubble : public CEffectBase
{
public:
	
	         CEFBigBubble();
	virtual ~CEFBigBubble();

public:

	virtual int		 Render(float dtime);							
	virtual int      Update(float dtime);
	virtual int      SetAfterEffect(void);

	virtual void	 Init(void);

private:

	CParticleSystem	 m_Particle;

	DWORD			 m_OldTimer;
	DWORD			 m_RangeTime;

	D3DXVECTOR3		 m_vStartPos;
};

#endif

