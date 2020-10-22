//...............................................................................................................
// 
//	CEFRoundFire.h														 라운드 파이어
//
//...............................................................................................................
#ifndef __CEFRoundFire_H
#define __CEFRoundFire_H

#include "CEffectBase.h"

class CEFRoundFire : public CEffectBase
{
public:
	 
	         CEFRoundFire();
	virtual ~CEFRoundFire();

public:

	virtual void	 Init(void);
	virtual int      Update(float dtime);
	virtual int		 Render(float dtime);

	D3DXVECTOR3		 m_vStartPos[36];
	D3DXVECTOR3		 m_vEndPos[36];	

	float			 m_fRadius;
		
	DWORD			 m_InitTime;		 //초기화시간	
	DWORD			 m_PrevTime;		
	DWORD			 m_RunningTime;		 //전체동작시간	

	BOOL			 m_ISDisplay;
	BOOL			 m_ISDamageProcess;

	CParticleSystem  m_FireParticle[2][36];
};

#endif