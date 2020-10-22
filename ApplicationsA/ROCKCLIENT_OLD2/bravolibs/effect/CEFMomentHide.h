//................................................................................................................
//
//  ¿ù¿µ
//
//................................................................................................................
#ifndef __CEFMomentHide_H
#define __CEFMomentHide_H

#include "CEffectBase.h"
#include "CFloatSequence.h"

class CEFMomentHide: public CEffectBase
{
public:
	 
	         CEFMomentHide();
	virtual ~CEFMomentHide();

public:

	virtual void	 Init(void);
			int		 Update(float dtime);

};


#endif