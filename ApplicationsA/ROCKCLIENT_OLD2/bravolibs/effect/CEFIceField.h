//...............................................................................................................
// 
//	CEFIceField.h														 아이스 필드
//
//...............................................................................................................
#ifndef __CEFIceField_H
#define __CEFIceField_H

#include "CEffectBase.h"

class CEFIceField : public CEffectBase
{
public:
	 
	         CEFIceField();
	virtual ~CEFIceField();

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