#ifndef __CEFMagicDelete_H
#define __CEFMagicDelete_H

#include "CEffectBase.h"

class CEFMagicDelete : public CEffectBase
{
public:
	
	         CEFMagicDelete();
	virtual ~CEFMagicDelete();

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

