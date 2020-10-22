//CEFCommonFlySill

#ifndef __CEFCOMMONFLYSKILL_H
#define __CEFCOMMONFLYSKILL_H

#include "CEffectBase.h"
#include "Billboard.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include	<SOUND\Sound.H>
#include "..\\..\\CRenderManager.h"

class CEFCommonFlySkill : public CEffectBase
{
public:
	
typedef	enum CommonFlySkill_Effect_Type
		{
			NONE		   = -1 ,
			BILLBOARD_LINK      ,
			BILLBOARD_SPOT      ,
			PARTICLE_LINK       ,
			PARTICLE_SPOT     
		};

public:
	
	         CEFCommonFlySkill();
	virtual ~CEFCommonFlySkill();

public:

	virtual int		 Render(float dtime);							
	virtual int      Update(float dtime);
	virtual int      SetAfterEffect(void);

	virtual void	 Init(void);

			void	 SetInit(int   FrontNum,CommonFlySkill_Effect_Type FrontType,int TailNum,CommonFlySkill_Effect_Type TailType,
							 int AttackedNum,DWORD RangeTime,Character *pSrc,int BoneNum,BOOL ISSKILL, BOOL IsAttacked = TRUE);

private:

	CParticleSystem				m_Particle[ 2 ];
	CBillboard					m_Billboard[ 2 ];

	CommonFlySkill_Effect_Type	m_Type[ 2 ];
	
	int							m_EffectNum[ 2 ];
	int							m_AttackedNum;

	DWORD						m_OldTimer;
	DWORD						m_RangeTime;

	D3DXVECTOR3					m_vStartPos;
	D3DXVECTOR3					m_vPos;
	DWORD						m_Time;

	BOOL						m_ISRender;
	int							m_BoneNum;

	BOOL						m_ISSkill;
	
	//By simwoosung				
	BOOL						m_IsAttacked;
};

#endif

