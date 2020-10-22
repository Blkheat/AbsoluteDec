//...............................................................................................................
// 
//	CEFFireStormSmash.h														 ¿­Ç³ °­Å¸
//
//...............................................................................................................
#ifndef __CEFFireStormSmash_H
#define __CEFFireStormSmash_H

#include "CEffectBase.h"

class CEFFireStormSmash : public CEffectBase
{

public:
	 
	         CEFFireStormSmash();
	virtual ~CEFFireStormSmash();

	virtual int		 CheckFrame(void);
	virtual int      Update(float dtime);
	int	Render(float dtime);

public:
	virtual void	 Init(void);
	virtual int SetAfterEffect(void);	
};

#endif