//...............................................................................................................
// 
//	CEFBowRain.h														 오월의 비
//
//...............................................................................................................
#ifndef __CEFBowRain_H
#define __CEFBowRain_H

#include "CEffectBase.h"
#include "CTail.h"
#include "CDropEffect.h"

class CEFBowRain : public CEffectBase
{
public:
	 
	         CEFBowRain();
	virtual ~CEFBowRain();

	virtual int		 CheckFrame(void);
	virtual int      Update(float dtime);

	int	Render(float dtime);

public:

	virtual void	 Init(void);

	DWORD			 m_OldTime;

	CDropEffect		 m_EffectDrop;
	BOOL			 m_ISDamageProcess;

	BOOL			 m_ISShot;
};

#endif


