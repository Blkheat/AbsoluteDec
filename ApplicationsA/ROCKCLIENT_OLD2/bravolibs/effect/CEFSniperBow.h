//...............................................................................................................
// 
//	CEFSniperBow.h														 화살 저격
//
//...............................................................................................................
#ifndef __CEFSNIPERBOW_H
#define __CEFSNIPERBOW_H

#include "CEffectBase.h"

class CEFSniperBow : public CEffectBase
{
public:
	 
	         CEFSniperBow();
	virtual ~CEFSniperBow();

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


