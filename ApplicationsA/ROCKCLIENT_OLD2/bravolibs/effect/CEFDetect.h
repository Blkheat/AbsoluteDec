//...............................................................................................................
// 
//	CEFDetect.h														 치유의 비
//
//...............................................................................................................
#ifndef __CEFDetect_H
#define __CEFDetect_H

#include "CEffectBase.h"
#include "CTail.h"
#include "Billboard.h"
#include "CFloatSequence.h"

class CEFDetect : public CEffectBase
{
public:
	 
	         CEFDetect();
	virtual ~CEFDetect();

	virtual int      Update(float dtime);

	int	Render(float dtime);

public:

	virtual void	 Init(void);

	DWORD			 m_OldTime;

	CBillboard		 m_Bill0;
	CBillboard		 m_Bill1;

	float			 m_AttackRange;

	CFloatSequence	 m_Range;
	D3DXVECTOR3		 m_vPos;
};

#endif


