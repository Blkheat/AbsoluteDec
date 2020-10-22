//...............................................................................................................
// 
//	CEFLandDamage.h														 광역마법 데미지
//
//...............................................................................................................
#ifndef __CEFLandDamage_H
#define __CEFLandDamage_H

#include "CEffectBase.h"
#include "CTail.h"
#include "CDropEffect.h"

class CEFLandDamage : public CEffectBase
{
public:
	 
	         CEFLandDamage();
	virtual ~CEFLandDamage();

	virtual int      Update(float dtime);

public:

	virtual void	 Init(void);

	DWORD			 m_WaitTime;
	DWORD			 m_OldTime;
	void			 SetDelayTime(DWORD Time)	{ m_WaitTime = Time;}
};

#endif


