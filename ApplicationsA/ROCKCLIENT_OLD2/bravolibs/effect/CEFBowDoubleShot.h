//...............................................................................................................
// 
//	CEFBowDoubleShot.h														 화살 발등 쏘기
//
//...............................................................................................................
#ifndef __CEFBowDoubleShot_H
#define __CEFBowDoubleShot_H

#include "CEffectBase.h"
#include "Billboard.h"
class CEFBowDoubleShot : public CEffectBase
{
public:
	 
	         CEFBowDoubleShot();
	virtual ~CEFBowDoubleShot();

public:

	virtual int		 CheckFrame(void);
	virtual int		 Render(float dtime);							
	virtual int      Update(float dtime);
	virtual int      SetAfterEffect(void);

	virtual void	 Init(void);

private:

	DWORD			 m_OldTimer;
	DWORD			 m_RangeTime;

	float			m_TileRate;
	

	D3DXVECTOR3		 m_vStartPos;
	D3DXMATRIX		 m_matLocal;
	D3DXMATRIX		 m_matRotY;

	BOOL			 m_ISShot;
	
	D3DXVECTOR3		m_vRightV;
	
	CBillboard m_Billboard;
};

#endif


