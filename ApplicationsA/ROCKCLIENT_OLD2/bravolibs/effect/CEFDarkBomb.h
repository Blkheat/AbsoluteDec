#ifndef __CEFDarkBomb_H
#define __CEFDarkBomb_H

#include "CEffectBase.h"

class CEFDarkBomb : public CEffectBase
{
public:
	
	         CEFDarkBomb();
	virtual ~CEFDarkBomb();

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

