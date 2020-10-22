//...............................................................................................................
// 
//	CEFStoneRian.h														 운석의 비
//
//...............................................................................................................
#ifndef __CEFStoneRian_H
#define __CEFStoneRian_H

#include "CEffectBase.h"
#include "CDropEffect.h"
#include "Billboard.h"

class CEFStoneRian : public CEffectBase
{
public:
	 
	         CEFStoneRian();
	virtual ~CEFStoneRian();

	virtual int      Update(float dtime);
	int	Render(float dtime);

public:
	virtual void	 Init(void);
	
	float			 m_fRadius;
	
	DWORD			 m_InitTime;
	BOOL			 m_IsStart;

	CDropEffect		 m_EffectDrop;
	BOOL			 m_ISDamageProcess;

	CBillboard		 m_BillboardStone;
	
	BOOL			 m_ISDisplay;
};

#endif