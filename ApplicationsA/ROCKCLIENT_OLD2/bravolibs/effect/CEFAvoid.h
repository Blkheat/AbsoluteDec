//...............................................................................................................
// 
//	CEFAvoid.h														 쾌검 ( 지속 스킬 )
//
//...............................................................................................................
#ifndef __CEFAVOID_H
#define __CEFAVOID_H

#include "CEffectBase.h"
#include "CFadeLoop.h"

class CEFAvoid : public CEffectBase
{
public:
	 
	         CEFAvoid();
	virtual ~CEFAvoid();

public:

	virtual void	 Init(void);

};

#endif
