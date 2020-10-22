//................................................................................................................
//
//  해골 아쳐용 스킬
//
//................................................................................................................
#ifndef __CEFBONARCSKILL_H
#define __CEFBONARCSKILL_H

#include "CEffectBase.h"
#include "CDropEffect.h"
#include "Billboard.h"

class CEFBonArcSkill : public CEffectBase
{
public:
	 
	         CEFBonArcSkill();
	virtual ~CEFBonArcSkill();

public:

	virtual void	 Init(void);
	virtual int		 CheckFrame(void);
	virtual int      Update(float dtime);
	virtual int		 Render(float dtime);
	virtual int		 SetAfterEffect();

	void	InitBonABaseAttack();
	void	InitBonAFireArrow();
	void	InitBonADecayArrow();
	void	InitBonAPainArrow();
	void	InitBonADarkArrow();
	void	InitBonACurseArrow();
	void	InitBonAParalysisArrow();
	void	InitBonBRandomArrow();

	int		CheckFrameBonABaseAttack();
	int		CheckFrameBonAFireArrow();
	int		CheckFrameBonADecayArrow();
	int		CheckFrameBonAPainArrow();
	int		CheckFrameBonADarkArrow();
	int		CheckFrameBonACurseArrow();
	int		CheckFrameBonAParalysisArrow();
	int		CheckFrameBonBRandomArrow();

	int		UpdateBonABaseAttack(float dtime);
	int		UpdateBonAFireArrow(float dtime);
	int		UpdateBonADecayArrow(float dtime);
	int		UpdateBonAPainArrow(float dtime);
	int		UpdateBonADarkArrow(float dtime);
	int		UpdateBonACurseArrow(float dtime);
	int		UpdateBonAParalysisArrow(float dtime);
	int		UpdateBonBRandomArrow(float dtime);

	int		RenderBonABaseAttack(float dtime);
	int		RenderBonAFireArrow(float dtime);
	int		RenderBonADecayArrow(float dtime);
	int		RenderBonAPainArrow(float dtime);
	int		RenderBonADarkArrow(float dtime);
	int		RenderBonACurseArrow(float dtime);
	int		RenderBonAParalysisArrow(float dtime);
	int		RenderBonBRandomArrow(float dtime);

	int		SetAEffectBonABaseAttack();
	int		SetAEffectBonAFireArrow();
	int		SetAEffectBonADecayArrow();
	int		SetAEffectBonAPainArrow();
	int		SetAEffectBonADarkArrow();
	int		SetAEffectBonACurseArrow();
	int		SetAEffectBonAParalysisArrow();
	int		SetAEffectBonBRandomArrow();	

	D3DXVECTOR3		 m_vStartPos;
	D3DXMATRIX		 m_matLocal;
	D3DXMATRIX		 m_matRotY;
	
	float			 m_fRadius;
	
	DWORD			 m_InitTime;
	DWORD			 m_RangeTime;
	DWORD			 m_PrevTime;
	DWORD			 m_GapTime;
	
	BOOL			 m_ISDisplay;
	BOOL			 m_ISShot;
	BOOL			 m_ISDamageProcess;

	float			 m_fXRadian;
	float			 m_fYRadian;

	CDropEffect		 m_EffectDrop;

	D3DXVECTOR3		 m_vStartPoses[36];
	D3DXVECTOR3		 m_vEndPos[36];
	D3DXVECTOR3		 m_vDir[36];
	float			 m_fYRadians[36];	
};

#endif