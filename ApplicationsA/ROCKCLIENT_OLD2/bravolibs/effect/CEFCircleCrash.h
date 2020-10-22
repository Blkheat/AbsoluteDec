//...............................................................................................................
// 
//	CEFCircleCrash.h														 서클크래쉬
//
//...............................................................................................................
#ifndef __CEFCircleCrash_H
#define __CEFCircleCrash_H

#include "CEffectBase.h"

class CEFCircleCrash : public CEffectBase
{
public:
	 
	         CEFCircleCrash();
	virtual ~CEFCircleCrash();

	virtual void	 Init(void);
	virtual int		 CheckFrame(void);
	virtual int      Update(float dtime);
	int	Render(float dtime);

public:	
	virtual int SetAfterEffect(void);
	
	DWORD  m_InitTime;
	BOOL   m_IsDamageProc;
};

#endif