//...............................................................................................................
// 
//	CEFFireArmor.h														 ���̾� �Ƹ� ( ���Ӽ� )
//
//...............................................................................................................
#ifndef __CEFFireArmor_H
#define __CEFFireArmor_H

#include "CEffectBase.h"

class CEFFireArmor : public CEffectBase
{
public:
	 
	         CEFFireArmor();
	virtual ~CEFFireArmor();

public:

	virtual void	 Init(void);
	virtual int      Update(float dtime);

};

#endif


