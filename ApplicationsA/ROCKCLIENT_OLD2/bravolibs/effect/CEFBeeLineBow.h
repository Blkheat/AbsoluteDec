//...............................................................................................................
// 
//	CEFBeeLineBow.h														 직선 화살류
//
//...............................................................................................................
#ifndef __CEFBeeLineBow_H
#define __CEFBeeLineBow_H

#include "CEffectBase.h"

class CEFBeeLineBow : public CEffectBase
{
public:
	 
	         CEFBeeLineBow();
	virtual ~CEFBeeLineBow();

public:

	virtual int		 CheckFrame(void);	
	virtual int		 Render(float dtime);							
	virtual int      Update(float dtime);
	virtual int      SetAfterEffect(void);

	virtual void	 Init(void);

	void			 SetExtra(int TailNum,int DamageNum,DWORD Time,float DelayTime,int DamageType = 0);

private:

	DWORD			 m_OldTimer;
	DWORD			 m_RangeTime;

	D3DXVECTOR3		 m_vStartPos;
	D3DXMATRIX		 m_matLocal;
	
	int				 m_TailParticleNum;
	int				 m_DamageEffectNum;
	int				 m_DamageEffectType;	
	DWORD			 m_SpeedToOneTime;	
	float			 m_EffectDelayTime;
	
	BOOL			 m_ISShot;
	
	float			 m_fXRadian;
	float			 m_fYRadian;
};

#endif


