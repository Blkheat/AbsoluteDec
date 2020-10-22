//
//...............................................................................................................
// 
//	CEFFastBow.h														 화살 기본 공격
//
//...............................................................................................................
#ifndef __CEFFastBow_H
#define __CEFFastBow_H

#include "CEffectBase.h"

class CEFFastBow : public CEffectBase
{
public:
	 
	         CEFFastBow();
	virtual ~CEFFastBow();

public:

	virtual int		 Render(float dtime);							
	virtual int      Update(float dtime);
	virtual int      SetAfterEffect(void);
	virtual int      CheckFrame(void);
	virtual void	 Init(void);

private:

	DWORD			 m_OldTimer;
	DWORD			 m_RangeTime;

	D3DXVECTOR3		 m_vStartPos;
	D3DXMATRIX		 m_matLocal;

	BOOL			 m_ISShot;

	float			 m_fXRadian;
	float			 m_fYRadian;
};

#endif


