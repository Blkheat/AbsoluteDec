//...............................................................................................................
// 
//	CEFGolemEffect			거신병 연관 이펙트
//
//...............................................................................................................
#ifndef __CEFGolemEffect_H
#define __CEFGolemEffect_H

#include "CEffectBase.h"

class CEFGolemEffect : public CEffectBase
{
public:
	 
	         CEFGolemEffect();
	virtual ~CEFGolemEffect();

public:

	virtual int		 CheckFrame(void);
	virtual int		 Render(float dtime);							
	virtual int      Update(float dtime);
	virtual int      SetAfterEffect(void);

	virtual void	 Init(void);
	void SetNoDamageEffect();
	
	int		m_nMiddleImpacFrame;
	int		m_nLastImpacFrame;
	UINT	m_uTaleType;
	UINT    m_uRightHandEffId;
	UINT    m_uRightHandType;
	UINT	m_uLeftHandEffId;
	UINT	m_uLeftHandType;
	UINT	m_uExploEffId;
	UINT	m_uExploEffType;
	UINT	m_uAttackedEffect;
	UINT	m_uAttackedType;
};

#endif