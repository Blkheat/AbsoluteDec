//...............................................................................................................
// 
//	CEFTigerAttack												ȣ���� 4 ��Ÿ����
//
//...............................................................................................................
#ifndef __CEFTigerAttack_H
#define __CEFTigerAttack_H

#include "CEffectBase.h"

class CEFTigerAttack : public CEffectBase
{
public:
	 
	         CEFTigerAttack();
	virtual ~CEFTigerAttack();

public:

	virtual int		 CheckFrame(void);
	virtual int		 Render(float dtime);							
	virtual int      Update(float dtime);
	virtual int      SetAfterEffect(void);

	virtual void	 Init(void);
};

#endif