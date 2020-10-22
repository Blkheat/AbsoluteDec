//...............................................................................................................
// 
//	CEFStickUp.h														 ¡ˆ∆Œ¿Ã ¿Œ√æ∆Æ
//
//...............................................................................................................
#ifndef __CEFStickUp_H
#define __CEFStickUp_H

#include "CEffectBase.h"
#include "Billboard.h"
#include <vector>

class CEFStickUp : public CEffectBase
{
public:
	 
	         CEFStickUp();
	virtual ~CEFStickUp();

	virtual int		 Render(float dtime);							
	virtual int		 SortRender(float dtime);
	virtual int      Update(float dtime);
	virtual void	 Init(void);

public:

	BYTE							 m_Level;
	int								 m_WeaponType;

private:

	CParticleSystem					 m_ParticleLink;
	std::vector		<ENCHANT_POINT>	 m_EnChantList;
};

#endif


