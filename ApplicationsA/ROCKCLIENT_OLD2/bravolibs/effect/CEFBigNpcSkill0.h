//
//...............................................................................................................
// 
//	CEFBigNpcSkill0.h														 
//
//...............................................................................................................
#ifndef __CEFBigNpcSkill0_H
#define __CEFBigNpcSkill0_H

#include "CEffectBase.h"
#include "CBillboardLine.h"

class CEFBigNpcSkill0 : public CEffectBase
{
public:
	 
	         CEFBigNpcSkill0();
	virtual ~CEFBigNpcSkill0();

public:

	virtual int		 CheckFrame(void);
	virtual int		 Render(float dtime);							
	virtual int      Update(float dtime);
	virtual int      SetAfterEffect(void);

	virtual void	 Init(void);

private:

	int		m_Code;
	int		m_ImpactFrame;

	int		m_AttackEDEffectID;
};

#endif


