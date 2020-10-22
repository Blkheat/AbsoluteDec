//...............................................................................................................
// 
//	CEFFootBow.h														 화살 발등 쏘기
//
//...............................................................................................................
#ifndef __CEFFootBow_H
#define __CEFFootBow_H

#include "CEffectBase.h"

class CEFFootBow : public CEffectBase
{
public:
	 
	         CEFFootBow();
	virtual ~CEFFootBow();

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
	D3DXMATRIX		 m_matRotY;

	BOOL			 m_ISShot;
};

#endif


