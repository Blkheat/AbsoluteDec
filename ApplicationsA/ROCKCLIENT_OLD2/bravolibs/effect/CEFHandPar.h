#ifndef __CEFHandPar_H
#define __CEFHandPar_H

#include "CEffectBase.h"
#include "Billboard.h"
#include "CFloatSequence.h"
#include <vector>

class CEFHandPar : public CEffectBase
{
public:
	
	         CEFHandPar();
	virtual ~CEFHandPar();

public:

	virtual void	 Init(void);
	virtual int		 Render(float dtime);							
	virtual int      Update(float dtime);

private:

	D3DXVECTOR3		 m_vStartPos;
	D3DXVECTOR3		 m_vEndPos;
	D3DXVECTOR3		 m_vPos;
	D3DXVECTOR3		 m_vDir;

	DWORD			 m_OldTimer;

	CFloatSequence   m_Range;
	DWORD			 m_RangeTime;
	BOOL			 m_ISCreate;
	float			 m_fDistace;
};

#endif

