//...............................................................................................................
// 
//	CEFWindBush.h														 윈드 버쉬
//
//...............................................................................................................
#ifndef __CEFWindBush_H
#define __CEFWindBush_H

#include "CEffectBase.h"

class CEFWindBush : public CEffectBase
{
public:
	 
	         CEFWindBush();
	virtual ~CEFWindBush();

	virtual int      Update(float dtime);
	int	Render(float dtime);

public:
	virtual void	 Init(void);
	
	float			 m_fRadius;
	
	DWORD			 m_InitTime;
	DWORD			 m_PrevTime;
	DWORD			 m_GapTime;
	
	BOOL			 m_ISDisplay;
	BOOL			 m_ISDamageProcess;
};

#endif