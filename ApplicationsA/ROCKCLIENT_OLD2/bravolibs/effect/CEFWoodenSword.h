#ifndef __CEFWOODENSWORD_H
#define __CEFWOODENSWORD_H

#include "CEffectBase.h"

class CEFWoodenSword : public CEffectBase
{
public:
	
	         CEFWoodenSword();
	virtual ~CEFWoodenSword();

public:

	virtual int      SetAfterEffect(void);

	virtual void	 Init(void);
};

#endif

