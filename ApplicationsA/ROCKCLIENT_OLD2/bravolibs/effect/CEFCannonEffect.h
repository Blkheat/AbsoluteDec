//...............................................................................................................
// 
//	CEFCannonEffect			거대 대포형 연관 이펙트
//
//...............................................................................................................
#ifndef __CEFCannonEffect_H
#define __CEFCannonEffect_H

#include "CEffectBase.h"
#include "Billboard.h"

class CEFCannonEffect : public CEffectBase
{
public:
	 
	         CEFCannonEffect();
	virtual ~CEFCannonEffect();

protected:
	
	float			 m_fXRadian;
	float			 m_fYRadian;
	
	DWORD			 m_RangeTime;
	DWORD			 m_InitTime;
	
	CBillboard		 m_LinkBillboard;
	D3DXVECTOR3		 m_vStartPos;
	D3DXMATRIX		 m_matLocal;
	
	BOOL			 m_ISShot;

public:

	virtual int		 CheckFrame(void);
	virtual int		 Render(float dtime);							
	virtual int      Update(float dtime);
	virtual int      SetAfterEffect(void);

	virtual void	 Init(void);

	int  m_nSrcBoneNum;
	int	 m_nImpacFrame;
	UINT m_uEnergyGetEffId;
	UINT m_uEnergyGetType;
	UINT m_uBillType;	
	UINT m_uTaleType;
	UINT m_uPercuEffId;
	UINT m_uPercuEffType;
	UINT m_uExploEffId;
	UINT m_uExploEffType;
	UINT m_uAttackedEffect;
	UINT m_uAttackedType;	
};

#endif