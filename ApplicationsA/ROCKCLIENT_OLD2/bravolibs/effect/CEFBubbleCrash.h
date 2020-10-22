#ifndef __CEFBUBBLECRASH_H
#define __CEFBUBBLECRASH_H

#include "CEffectBase.h"

class CEFBubbleCrash : public CEffectBase
{
public:
	
	         CEFBubbleCrash();
	virtual ~CEFBubbleCrash();

public:

	virtual int		 CheckFrame(void);
	virtual int		 Render(float dtime);							
	virtual int      Update(float dtime);
	virtual int      SetAfterEffect(void);

	virtual void	 Init(void);


private:

	int							m_Type;
	
	DWORD						m_OldTimer;
	DWORD						m_RangeTime;

	D3DXVECTOR3					m_vStartPos;
};

#endif

