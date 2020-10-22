//...............................................................................................................
// 
//	CEFRefreshSword.h														 쾌검 ( 지속 스킬 )
//
//...............................................................................................................
#ifndef __CEFREFRESHSWORD_H
#define __CEFREFRESHSWORD_H

#include "CEffectBase.h"
#include "CFadeLoop.h"

class CEFRefreshSword : public CEffectBase
{
public:
	 
	         CEFRefreshSword();
	virtual ~CEFRefreshSword();

public:

	virtual void	 Init(void);
};

#endif


