#ifndef __CEFWrath_Bomb_H
#define __CEFWrath_Bomb_H

#include "CEffectBase.h"
#include "Billboard.h"
#include "CFloatSequence.h"
#include <vector>

class CEFWrath_Bomb : public CEffectBase
{
public:
				
	         CEFWrath_Bomb();
	virtual ~CEFWrath_Bomb();

public:

	virtual void	Init(void);
	virtual int		Render(float dtime);							
	virtual int     Update(float dtime);
	void			PositionEffect(float Size);
	virtual int		CheckFrame(void);
	virtual int		SetAfterEffect(void);


private:

	D3DXVECTOR3		 m_vStartPos;
	D3DXVECTOR3		 m_vEndPos;
	D3DXVECTOR3		 m_vPos;
	D3DXVECTOR3		 m_vDir;

	DWORD			 m_OldTimer;
	DWORD			 m_DelayTimer;

	CFloatSequence   m_Range;
	DWORD			 m_RangeTime;
	BOOL			 m_ISCreate;
	float			 m_fDistace;
};

#endif

