//...............................................................................................................
// 
//	CEFIceSpear.h														 아이스 스피어
//
//...............................................................................................................
#ifndef __CEFIceSpear_H
#define __CEFIceSpear_H

#include "CEffectBase.h"
#include "Billboard.h"


class CEFIceSpear : public CEffectBase
{
public:
	 
	         CEFIceSpear();
	virtual ~CEFIceSpear();

	virtual int      Update(float dtime);
	virtual int		 Render(float dtime);
	virtual int      SetAfterEffect(void);

public:
	virtual void	 Init(void);

public:
	DWORD			 m_InitTime;
	D3DXMATRIX		 m_matLocal;

	DWORD			 m_RangeTime;
	D3DXVECTOR3		 m_vStartPos;

	CBillboard		 m_BillboardIce;
	CParticleSystem	 m_TailParticle;

	float			 m_fXRadian;
	float			 m_fYRadian;
};

#endif