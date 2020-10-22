//...............................................................................................................
// 
//	CEFCastStone.h														 Ä³½ºÆ® ½ºÅæ
//
//...............................................................................................................
#ifndef __CEFCastStone_H
#define __CEFCastStone_H

#include "CEffectBase.h"
#include "Billboard.h"

class CEFCastStone : public CEffectBase
{
public:
	 
	         CEFCastStone();
	virtual ~CEFCastStone();

public:

	virtual void	 Init(void);
	virtual int      Update(float dtime);
	virtual int		 Render(float dtime);
	virtual int      SetAfterEffect(void);
	
	public:
	DWORD			 m_InitTime;
	D3DXMATRIX		 m_matLocal;

	DWORD			 m_RangeTime;
	D3DXVECTOR3		 m_vStartPos;

	CBillboard		 m_BillboardStone;

	float			 m_fXRadian;
	float			 m_fYRadian;
};

#endif