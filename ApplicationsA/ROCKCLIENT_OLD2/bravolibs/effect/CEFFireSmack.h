//...............................................................................................................
// 
//	CEFFireSmack.h														 È­¿° ¿¬Å¸
//
//...............................................................................................................
#ifndef __CEFFireSmack_H
#define __CEFFireSmack_H

#include "CEffectBase.h"

class CEFFireSmack : public CEffectBase
{
public:
	 
	         CEFFireSmack();
	virtual ~CEFFireSmack();

public:

	virtual int		 CheckFrame(void);
	virtual int		 Render(float dtime);							
	virtual int      Update(float dtime);
	virtual int      SetAfterEffect(void);

	virtual void	 Init(void);

};

#endif