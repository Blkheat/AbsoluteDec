//...............................................................................................................
// 
//	CEFGTEffect			가드타워 연관 이펙트
//
//...............................................................................................................
#ifndef __CEFGTEffect_H
#define __CEFGTEffect_H

#include "CEffectBase.h"
#include "Billboard.h"
#include "CBillboardLine.h"

class CEFGTEffect : public CEffectBase
{
public:
	 
	         CEFGTEffect();
	virtual ~CEFGTEffect();

protected:

	int				 m_nTargetCount;
	Character	     ** m_ppTagetList;
	EFFECT_EXTRA_INFO	*m_pEFFECT_EXTRA_INFOList;
	float			 * m_pXRadianList;
	float			 * m_pYRadianList;
	DWORD			 * m_pRangeTimeList;
	BOOL			 * m_pIsDamageActList;
	D3DXMATRIX		 * m_pmatLocalList;

	DWORD			 m_InitTime;	

	CBillboard		 m_LinkBillboard;
	D3DXVECTOR3		 m_vStartPos;

	CBillboardLine	 m_Tail;

public:

	virtual int		 CheckFrame(void);
	virtual int		 Render(float dtime);							
	virtual int      Update(float dtime);
	virtual int      SetAfterEffect(void);

	virtual void	 Init(void);

	int				 m_nSrcBoneNum;
	UINT			 m_uBillType;
	UINT			 m_uTaleType;
	UINT			 m_uAttackedEffect;
	UINT			 m_uAttackedType;
};

#endif