//...............................................................................................................
// 
//	CEFLightStorm.h														 ∂Û¿Ã∆Æ Ω∫≈Ë
//
//...............................................................................................................
#ifndef __CEFLightStorm_H
#define __CEFLightStorm_H

#include "CEffectBase.h"

class CEFLightStorm : public CEffectBase
{
public:
	 
	         CEFLightStorm();
	virtual ~CEFLightStorm();

	virtual int      Update(float dtime);
	int	Render(float dtime);

public:
	virtual void	 Init(void);

	float			 m_fRadius;
	
	DWORD			 m_InitTime;
	DWORD			 m_PrevTime;
	DWORD			 m_PrevTime2;
	DWORD			 m_GapTime;	
	DWORD			 m_RunningTime;
					 
	BOOL			 m_ISDisplay;
	BOOL			 m_ISDamageProcess;
};

#endif