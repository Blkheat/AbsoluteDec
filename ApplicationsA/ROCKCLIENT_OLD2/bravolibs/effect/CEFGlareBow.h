//...............................................................................................................
// 
//	CEFGlareBow.h														 화살 노려 쏘기
//
//...............................................................................................................
#ifndef __CEFGLAREBOW_H
#define __CEFGLAREBOW_H

#include "CEffectBase.h"

class CEFGlareBow : public CEffectBase
{
public:
	 
	         CEFGlareBow();
	virtual ~CEFGlareBow();

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


