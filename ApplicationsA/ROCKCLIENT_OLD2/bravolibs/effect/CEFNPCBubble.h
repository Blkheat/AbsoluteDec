#ifndef __CEFNPCBubble_H
#define __CEFNPCBubble_H

#include "CEffectBase.h"
#include "Billboard.h"

class CEFNPCBubble : public CEffectBase
{
public:
	
	         CEFNPCBubble();
	virtual ~CEFNPCBubble();

public:

	virtual int		 CheckFrame(void);
	virtual int		 Render(float dtime);							
	virtual int      Update(float dtime);
	virtual int      SetAfterEffect(void);

	static  int		 InitDevice(void);
	static  int		 DeleteDevice(void);
	
	virtual void	 Init(void);

private:

	CBillboard		 m_Billboard;


	DWORD			 m_OldTimer;
	DWORD			 m_RangeTime;

	D3DXVECTOR3		 m_vStartPos;
};

#endif

