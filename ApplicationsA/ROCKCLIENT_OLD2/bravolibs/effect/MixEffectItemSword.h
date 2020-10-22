//...............................................................................................................
// 
//	MixEffectItemSword.h														 Ä® ÀÎÃ¾Æ®
//
//...............................................................................................................
#ifndef __MixEffectItemSword_H
#define __MixEffectItemSword_H

#include "CEffectBase.h"
#include <vector>
#include "Billboard.h"

class MixEffectItemSword : public CEffectBase
{
public:
	 
	         MixEffectItemSword();
	virtual ~MixEffectItemSword();

	virtual int		 Render(float dtime);							
	virtual int      Update(float dtime);
	virtual int		 SortRender(float dtime);
	virtual void	 Init(void);

public:

	BYTE							 m_Level;
	int								 m_WeaponType;
	int								 m_Count;
	
	CBillboard		 m_Sword1;
	CBillboard		 m_Sword2;

private:
	
	CParticleSystem					 m_ParticleLink[ 15 ];

	std::vector		<ENCHANT_POINT>	 m_EnChantList;
};

#endif


