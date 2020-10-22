#ifndef __CEFHolyBomb_H
#define __CEFHolyBomb_H

#include "CEffectBase.h"

class CEFHolyBomb : public CEffectBase
{
public:
	
	         CEFHolyBomb();
	virtual ~CEFHolyBomb();

public:

	virtual int		 CheckFrame(void);
	virtual int		 Render(float dtime);							
	virtual int      Update(float dtime);
	virtual int      SetAfterEffect(void);

	virtual void	 Init(void);


private:

	int							m_Type;
	
	DWORD						m_OldTimer;
	DWORD						m_RangeTime;

	D3DXVECTOR3					m_vStartPos;
	D3DXVECTOR3					m_vDestPos;
	D3DXVECTOR3					m_vDistance	;
	
	
	CParticleSystem m_BallParticle[2];

};

#endif

