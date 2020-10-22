//...............................................................................................................
// 
//	CEFRecoveryItem2.h														 지속성 아이템
//
//...............................................................................................................
#ifndef __CEFRECOVERYITEM2_H
#define __CEFRECOVERYITEM2_H

#include "CEffectBase.h"

class CEFRecoveryItem2 : public CEffectBase
{
public:
	 
	         CEFRecoveryItem2();
	virtual ~CEFRecoveryItem2();

public:

	virtual void	 Init(void);
	virtual int      Update(float dtime);

private:

	DWORD	m_OldTimer;								
	DWORD   m_RangeTimer;								// 지속 시간

};

#endif


