//...............................................................................................................
// 
//	CEFRecoveryItem2.h														 ���Ӽ� ������
//
//...............................................................................................................
#ifndef __CEFRECOVERYITEM2_H
#define __CEFRECOVERYITEM2_H

#include "CEffectBase.h"

class CEFRecoveryItem2 : public CEffectBase
{
public:
	 
	         CEFRecoveryItem2();
	virtual ~CEFRecoveryItem2();

public:

	virtual void	 Init(void);
	virtual int      Update(float dtime);

private:

	DWORD	m_OldTimer;								
	DWORD   m_RangeTimer;								// ���� �ð�

};

#endif


