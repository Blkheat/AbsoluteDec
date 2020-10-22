#ifndef __CEFICESPIRE_H
#define __CEFICESPIRE_H

#include "CEffectBase.h"

class CEFIceSpire : public CEffectBase
{
public:
	
	         CEFIceSpire();
	virtual ~CEFIceSpire();

public:

	virtual int      SetAfterEffect(void);

	virtual void	 Init(void);
};

#endif

