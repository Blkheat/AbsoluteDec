//...............................................................................................................
// 
//	CEFBigPress.h														 ∫Ú«¡∑πΩ∫
//
//...............................................................................................................
#ifndef __CEFBigPress_H
#define __CEFBigPress_H

#include "CEffectBase.h"
#include "CTail.h"
#include "CFloatSequence.h"
#include "Billboard.h"

class CEFBigPress : public CEffectBase
{
public:
	 
	         CEFBigPress();
	virtual ~CEFBigPress();

	virtual int      Update(float dtime);
	int	Render(float dtime);

public:

	virtual void	 Init(void);

	DWORD			 m_OldTime;
	CFloatSequence   m_Range;
	D3DXVECTOR3		 m_vStartPos;
	D3DXVECTOR3		 m_vEndPos;
	D3DXVECTOR3		 m_vPos;

	CBillboard		 m_BillRock;
};

#endif


