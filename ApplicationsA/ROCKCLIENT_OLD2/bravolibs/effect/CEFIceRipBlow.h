//CEFIceRipBlow
#ifndef __CEFIceRipBlow_H
#define __CEFIceRipBlow_H

#include "CEffectBase.h"

class CEFIceRipBlow : public CEffectBase
{
public:
	
	         CEFIceRipBlow();
	virtual ~CEFIceRipBlow();

public:

	virtual int		 Render(float dtime);							
	virtual int      Update(float dtime);
	virtual int      SetAfterEffect(void);

	virtual void	 Init(void);

private:

	DWORD			 m_OldTimer;
	DWORD			 m_RangeTime;

	D3DXVECTOR3		 m_vStartPos;
};

#endif

