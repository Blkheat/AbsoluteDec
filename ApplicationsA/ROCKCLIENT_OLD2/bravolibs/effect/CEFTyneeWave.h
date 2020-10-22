#ifndef __CEFTyneeWave_H
#define __CEFTyneeWave_H

#include "CEffectBase.h"
#include "Billboard.h"
#include "CFloatSequence.h"
#include <vector>

class CEFTyneeWave : public CEffectBase
{
public:
	
	         CEFTyneeWave();
	virtual ~CEFTyneeWave();

public:

	virtual void	 Init(void);
	virtual int		 Render(float dtime);							
	virtual int      Update(float dtime);

private:

//	D3DXVECTOR3		 m_vStartPos;
//	D3DXVECTOR3		 m_vEndPos;
	D3DXVECTOR3		 m_vPos;
//	D3DXVECTOR3		 m_vDir;

//	DWORD			 m_OldTimer;

//	CFloatSequence   m_Range;
//	DWORD			 m_RangeTime;
//	BOOL			 m_ISCreate;
//	float			 m_fDistace;
};

#endif

