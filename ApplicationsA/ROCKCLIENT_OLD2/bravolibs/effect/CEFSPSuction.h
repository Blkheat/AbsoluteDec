#ifndef __CEFSPSuction_H
#define __CEFSPSuction_H

#include "CEffectBase.h"

class CEFSPSuction : public CEffectBase
{
public:
	
	         CEFSPSuction();
	virtual ~CEFSPSuction();

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

