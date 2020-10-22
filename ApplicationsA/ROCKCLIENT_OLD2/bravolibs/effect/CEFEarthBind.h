//...............................................................................................................
// 
//	CEFEarthBind.h														 대지의 속박
//
//...............................................................................................................
#ifndef __CEFEarthBind_H
#define __CEFEarthBind_H

#include "CEffectBase.h"

class CEFEarthBind : public CEffectBase
{
public:
	 
	         CEFEarthBind();
	virtual ~CEFEarthBind();

public:

	virtual void	 Init(void);
	virtual int		 CheckFrame(void);
	virtual int      Update(float dtime);
	virtual int		 Render(float dtime);
	
	float			 m_fRadius;
	
	DWORD			 m_InitTime;
	DWORD			 m_PrevTime;
	DWORD			 m_GapTime;
	
	BOOL			 m_ISDisplay;
	BOOL			 m_ISShot;
	BOOL			 m_ISDamageProcess;
};

#endif