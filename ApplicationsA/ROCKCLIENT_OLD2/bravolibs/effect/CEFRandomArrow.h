//...............................................................................................................
// 
//	CEFRandomArrow.h														 ³­»ç
//
//...............................................................................................................
#ifndef __CEFRandomArrow_H
#define __CEFRandomArrow_H

#include "CEffectBase.h"
#include "CDropEffect.h"
#include "Billboard.h"

class CEFRandomArrow : public CEffectBase
{
public:
	 
	         CEFRandomArrow();
	virtual ~CEFRandomArrow();

public:

	virtual void	 Init(void);
	virtual	int		 CheckFrame(void);
	virtual int      Update(float dtime);
	virtual int		 Render(float dtime);
	virtual int      SetAfterEffect(void);

	float			 m_fRadius;
	
	DWORD			 m_InitTime;
	DWORD			 m_PrevTime;
	DWORD			 m_GapTime;
	DWORD			 m_RangeTime;
	
	BOOL			 m_ISDisplay;
	BOOL			 m_ISShot;
	BOOL			 m_ISDamageProcess;

	D3DXVECTOR3		 m_vStartPos[36];
	D3DXVECTOR3		 m_vEndPos[36];
	D3DXVECTOR3		 m_vDir[36];
	float			 m_fYRadian[36];
	
	CParticleSystem  m_Particle[36];
	CBillboard		 m_Billboard;
	
	float			 m_fXRadian;
};

#endif