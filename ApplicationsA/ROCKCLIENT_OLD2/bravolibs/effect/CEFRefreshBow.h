//...............................................................................................................
// 
//	CEFRefreshBow.h														 치유의 화살
//
//...............................................................................................................
#ifndef __CEFREFRESHBOW_H
#define __CEFREFRESHBOW_H

#include "CEffectBase.h"

class CEFRefreshBow : public CEffectBase
{
public:
	 
	         CEFRefreshBow();
	virtual ~CEFRefreshBow();

public:

	virtual int		 CheckFrame(void);
	virtual int		 Render(float dtime);							
	virtual int      Update(float dtime);
	virtual int      SetAfterEffect(void);

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


