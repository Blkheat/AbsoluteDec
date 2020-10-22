//...............................................................................................................
// 
//	CEFBowAttack.h														 화살 기본 공격
//
//...............................................................................................................
#ifndef __CEFBOWATTACK_H
#define __CEFBOWATTACK_H

#include "CEffectBase.h"

class CEFBowAttack : public CEffectBase
{
public:
	 
	         CEFBowAttack();
	virtual ~CEFBowAttack();

public:

	virtual int		 Render(float dtime);							
	virtual int      Update(float dtime);
	virtual int      SetAfterEffect(void);

	virtual void	 Init(void);

private:

	DWORD			 m_OldTimer;
	DWORD			 m_RangeTime;

	D3DXVECTOR3		 m_vStartPos;
	D3DXMATRIX		 m_matLocal;
	
	float			 m_fXRadian;
	float			 m_fYRadian;

};

#endif


