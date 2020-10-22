//...............................................................................................................
// 
//	CEFHealRainBow.h														 치유의 비
//
//...............................................................................................................
#ifndef __CEFHealRainBow_H
#define __CEFHealRainBow_H

#include "CEffectBase.h"
#include "CTail.h"
#include "CDropEffect.h"
#include "Billboard.h"

class CEFHealRainBow : public CEffectBase
{
public:
	 
	         CEFHealRainBow();
	virtual ~CEFHealRainBow();

	virtual int		 CheckFrame(void);
	virtual int      Update(float dtime);

	int	Render(float dtime);

public:

	virtual void	 Init(void);

	DWORD			 m_OldTime;

	CDropEffect		 m_EffectDrop;
	BOOL			 m_ISDamageProcess;

	CBillboard		 m_Billboard;

	BOOL			 m_ISShot;
};

#endif


