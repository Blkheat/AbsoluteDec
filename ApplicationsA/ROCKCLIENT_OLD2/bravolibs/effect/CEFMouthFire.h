#ifndef __CEFMouthFire_H
#define __CEFMouthFire_H

#include "CEffectBase.h"

class CEFMouthFire : public CEffectBase
{
public:
	
	         CEFMouthFire();
	virtual ~CEFMouthFire();

public:

	void			 SetInit(int Fire,int Tail,DWORD Time,int Link,int DamageID);

	int				 m_TailNum;
	int				 m_Link;
	int				 m_FireNum;
	DWORD			 m_Time;
	int				 m_DamageID;

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