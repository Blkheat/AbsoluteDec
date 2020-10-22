//...............................................................................................................
// 
//	CEFBallUp.h														 ������ ��þƮ
//
//...............................................................................................................
#ifndef __CEFBallUp_H
#define __CEFBallUp_H

#include "CEffectBase.h"
#include "Billboard.h"
#include <vector>

class CEFBallUp : public CEffectBase
{
public:
	 
	         CEFBallUp();
	virtual ~CEFBallUp();

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


