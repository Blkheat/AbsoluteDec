//...............................................................................................................
// 
//	CEFNpcStoneAttack.h														 돌 던지기
//
//...............................................................................................................
#ifndef __CEFNpcStoneAttack_H
#define __CEFNpcStoneAttack_H

#include "CEffectBase.h"
#include "CEffectModel.h"

class CEFNpcStoneAttack : public CEffectBase
{
public:
	 
	         CEFNpcStoneAttack();
	virtual ~CEFNpcStoneAttack();

public:

	static  int		 InitDevice(void);
	static  int		 DeleteDevice(void);
				
	virtual int		 Render(float dtime);							
	virtual int      Update(float dtime);
	virtual int      SetAfterEffect(void);

	virtual void	 Init(void);

private:

	static CEffectModel	 m_StoneModel[2];

	DWORD			 m_OldTimer;
	DWORD			 m_RangeTime;

	D3DXVECTOR3		 m_vStartPos;
	D3DXMATRIX		 m_matLocal;
};

#endif


