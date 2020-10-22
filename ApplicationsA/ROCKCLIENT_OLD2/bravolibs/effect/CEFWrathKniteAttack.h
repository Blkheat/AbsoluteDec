//...............................................................................................................
// 
//	CEFWrathKniteAttack.h														 분노의 검진
//
//...............................................................................................................
#ifndef __CEFWrathKniteAttack_H
#define __CEFWrathKniteAttack_H

#include "CEffectBase.h"

class CEFWrathKniteAttack : public CEffectBase
{
public:
	 
	         CEFWrathKniteAttack();
	virtual ~CEFWrathKniteAttack();

	virtual int		 CheckFrame(void);
	virtual int      Update(float dtime);
	int	Render(float dtime);	

public:
	virtual void	 Init(void);
	virtual int SetAfterEffect(void);	
};

#endif