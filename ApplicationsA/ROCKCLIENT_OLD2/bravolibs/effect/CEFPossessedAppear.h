// 
//...............................................................................................................
// 
//	CEFPossessedAppear.h														 ���� ��Ÿ����
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


