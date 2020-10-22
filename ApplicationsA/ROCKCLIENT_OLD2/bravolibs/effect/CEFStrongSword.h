//...............................................................................................................
// 
//	CEFStrongSword.h														 쾌검 ( 지속 스킬 )
//
//...............................................................................................................
#ifndef __CEFSTRONGSWORD_H
#define __CEFSTRONGSWORD_H

#include "CEffectBase.h"
#include "CFadeLoop.h"

class CEFStrongSword : public CEffectBase
{
public:
	 
	         CEFStrongSword();
	virtual ~CEFStrongSword();

public:

	virtual void	 Init(void);
};

#endif