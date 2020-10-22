#ifndef __CEFFireBall_H
#define __CEFFireBall_H

#include "CEffectBase.h"

class CEFFireBall : public CEffectBase
{
public:
	
	         CEFFireBall();
	virtual ~CEFFireBall();

public:

	virtual int		 CheckFrame(void);
	virtual int		 Render(float dtime);							
	virtual int      Update(float dtime);
	virtual int      SetAfterEffect(void);

	static  int		 InitDevice(void);
	static  int		 DeleteDevice(void);
	
	virtual void	 Init(void);

private:

	CParticleSystem	 m_FireParticle;

	DWORD			 m_OldTimer;
	DWORD			 m_RangeTime;

	D3DXVECTOR3		 m_vStartPos;
};

#endif

