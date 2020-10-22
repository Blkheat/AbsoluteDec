//...............................................................................................................
// 
//	CEFFPopoiAttack												포포이 연타공격
//
//...............................................................................................................
#ifndef __CEFFPopoiAttack_H
#define __CEFFPopoiAttack_H

#include "CEffectBase.h"

class CEFFPopoiAttack : public CEffectBase
{
public:
	 
	         CEFFPopoiAttack();
	virtual ~CEFFPopoiAttack();

public:

	virtual int		 CheckFrame(void);
	virtual int		 Render(float dtime);							
	virtual int      Update(float dtime);
	virtual int      SetAfterEffect(void);

	virtual void	 Init(void);
};

#endif