//...............................................................................................................
// 
//	CEFSpiritFireSmack.h														 기열 연타
//
//...............................................................................................................
#ifndef __CEFSpiritFireSmack_H
#define __CEFSpiritFireSmack_H

#include "CEffectBase.h"

class CEFSpiritFireSmack : public CEffectBase
{
public:
	 
	         CEFSpiritFireSmack();
	virtual ~CEFSpiritFireSmack();

	virtual int		 CheckFrame(void);
	virtual int      Update(float dtime);
	int	Render(float dtime);

public:
	virtual void	 Init(void);
	virtual int SetAfterEffect(void);

	float	m_fRightHandHeight;	
};

#endif