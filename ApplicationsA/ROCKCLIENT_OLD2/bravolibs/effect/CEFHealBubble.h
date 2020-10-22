#ifndef __CEFHEALBUBBLE_H
#define __CEFHEALBUBBLE_H

#include "CEffectBase.h"
#include "Billboard.h"

class CEFHealBubble : public CEffectBase
{
public:
	
	         CEFHealBubble();
	virtual ~CEFHealBubble();

public:

	virtual int		 Render(float dtime);							
	virtual int      Update(float dtime);
	virtual int      SetAfterEffect(void);

	virtual void	 Init(void);

private:

	CBillboard		 m_Billboard;

	DWORD			 m_OldTimer;
	DWORD			 m_RangeTime;

	D3DXVECTOR3		 m_vStartPos;
};

#endif

