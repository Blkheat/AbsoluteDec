//...............................................................................................................
// 
//	CEFBookUp.h														 √• ¿Œ√æ∆Æ
//
//...............................................................................................................
#ifndef __CEFBookUp_H
#define __CEFBookUp_H

#include "CEffectBase.h"
#include "Billboard.h"
#include <vector>

class CEFBookUp : public CEffectBase
{
public:
	 
	         CEFBookUp();
	virtual ~CEFBookUp();

	virtual int		 Render(float dtime);							
	virtual int      Update(float dtime);
	virtual int		 SortRender(float dtime);
	virtual void	 Init(void);

public:

	BYTE							 m_Level;
	int								 m_WeaponType;

private:

//	CParticleSystem					 m_ParticleLink;
	CBillboard						 m_Billboard;

	std::vector		<ENCHANT_POINT>	 m_EnChantList;
};

#endif


