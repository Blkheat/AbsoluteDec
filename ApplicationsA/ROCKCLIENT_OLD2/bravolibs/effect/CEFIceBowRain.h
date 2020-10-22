//...............................................................................................................
// 
//	CEFIceBowRain.h														 얼음의 화살비
//
//...............................................................................................................
#ifndef __CEFIceBowRain_H
#define __CEFIceBowRain_H

#include "CEffectBase.h"
#include "CTail.h"
#include "CDropEffect.h"
#include "Billboard.h"

class CEFIceBowRain : public CEffectBase
{
public:
	 
	         CEFIceBowRain();
	virtual ~CEFIceBowRain();

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
