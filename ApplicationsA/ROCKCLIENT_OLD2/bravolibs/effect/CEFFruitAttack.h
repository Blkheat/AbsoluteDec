//...............................................................................................................
// 
//	CEFFruitAttack.h														 과일 던지기
//
//...............................................................................................................
#ifndef __CEFFruitAttack_H
#define __CEFFruitAttack_H

#include "CEffectBase.h"
#include "CEffectModel.h"

class CEFFruitAttack : public CEffectBase
{
public:
	 
	         CEFFruitAttack();
	virtual ~CEFFruitAttack();

public:

	static  int		 InitDevice(void);
	static  int		 DeleteDevice(void);
				
	virtual int		 Render(float dtime);							
	virtual int      Update(float dtime);
	virtual int      SetAfterEffect(void);

	virtual void	 Init(void);

private:

	static CEffectModel	 m_FruitModel;

	DWORD			 m_OldTimer;
	DWORD			 m_RangeTime;

	D3DXVECTOR3		 m_vStartPos;
	D3DXMATRIX		 m_matLocal;
};

#endif


