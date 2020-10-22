//...............................................................................................................
// 
//	CEFSwordUp.h														 Ä® ÀÎÃ¾Æ®
//
//...............................................................................................................
#ifndef __CEFSwordUp_H
#define __CEFSwordUp_H

#include "CEffectBase.h"
#include <vector>

#define EFFECT_SWORD_UP_EXP

class CEFSwordUp : public CEffectBase
{
public:
	 
	         CEFSwordUp();
	virtual ~CEFSwordUp();

	virtual int		 Render(float dtime);							
	virtual int      Update(float dtime);
	virtual int		 SortRender(float dtime);
	virtual void	 Init(void);

public:

	BYTE							 m_Level;
	int								 m_WeaponType;
	int								 m_Count;
	
private:
	
	CParticleSystem					 m_ParticleLink[ 15 ];

	std::vector		<ENCHANT_POINT>	 m_EnChantList;
};

#endif


