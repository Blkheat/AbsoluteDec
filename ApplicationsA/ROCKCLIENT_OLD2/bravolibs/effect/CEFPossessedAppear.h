// 
//...............................................................................................................
// 
//	CEFPossessedAppear.h														 빙신 나타나기
//
//...............................................................................................................
#ifndef __CEFPossessedAppear_H
#define __CEFPossessedAppear_H

#include "CEffectBase.h"

class CEFPossessedAppear : public CEffectBase
{
public:
	 
	         CEFPossessedAppear();
	virtual ~CEFPossessedAppear();

public:

	virtual void	 Init(void);
	virtual int      Update(float dtime);
};

#endif


