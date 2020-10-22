//...............................................................................................................
// 
//	CEFBowUp.h														 »∞ ¿Œ√æ∆Æ
//
//...............................................................................................................
#ifndef __CEFBowUp_H
#define __CEFBowUp_H

#include "CEffectBase.h"
#include <vector>

class CEFBowUp : public CEffectBase
{
public:
	 
	         CEFBowUp();
	virtual ~CEFBowUp();

	virtual int		 Render(float dtime);							
	virtual int      Update(float dtime);
	virtual int		 SortRender(float dtime);
	virtual void	 Init(void);

public:

	BYTE							 m_Level;
	int								 m_WeaponType;
	int							   	 m_ParNum;

private:
	
	CParticleSystem					 m_ParticleLink[ 2 ];

	std::vector		<ENCHANT_POINT>	 m_EnChantList;
};

#endif


