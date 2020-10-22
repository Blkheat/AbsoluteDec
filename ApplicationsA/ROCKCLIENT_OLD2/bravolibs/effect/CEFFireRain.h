#ifndef __CEFFireRain_H
#define __CEFFireRain_H

#include "CEffectBase.h"
#include "CTail.h"
#include "CDropEffect.h"

class CEFFireRain : public CEffectBase
{
public:
	
	CEFFireRain();
	virtual~CEFFireRain();

	virtual int Update(float dtime);
	int Render(float dtime);

public:

	virtual void	 Init(void);

	//시간 체크 일단 . 
	DWORD			 m_OldTime;
	
	//떨어지는  효과 
	CDropEffect		 m_EffectDrop;
	BOOL			 m_ISDamageProcess;

	CParticleSystem m_FireParticle;

};

#endif	//__CEFFireRain_H
