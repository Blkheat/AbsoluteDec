#ifndef __CEFGageTrans_H
#define __CEFGageTrans_H

#include "CEffectBase.h"

class CEFGageTrans : public CEffectBase
{
public:
	
	         CEFGageTrans();
	virtual ~CEFGageTrans();

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