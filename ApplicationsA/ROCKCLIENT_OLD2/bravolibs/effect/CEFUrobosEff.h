//...............................................................................................................
// 
//	CEFUrobosEff.h														 우로보로스 관련 이펙트
//
//...............................................................................................................
#ifndef __CEFUrobosEff_H
#define __CEFUrobosEff_H

#include "CEffectBase.h"
#include "CDropEffect.h"

#include <vector>
using namespace	std;


class CEFUrobosEff : public CEffectBase
{
public:
	CEFUrobosEff();
	virtual ~CEFUrobosEff();


public:
	virtual void	 Init(void);
	virtual int		 CheckFrame(void);
	virtual int      Update(float dtime);
	virtual int		 Render(float dtime);
	virtual int		 SetAfterEffect();


public:
	void	InitPoisonBreath();
	int		CheckPoisonBreath();
	int		UpdaterPoisonBreath(float dtime);
	int		RenderPoisonBreath(float dtime);
	
	int		InitFrameBalkan();
	int		CheckFrameBalkan();
	int		RenderFrameBalkan(float dtime);

	int		CheckDragHowlling();
	int		SetAeffectDragHowlling();
		
	int		InitCharge();
	int		CheckCharge(); 
	int		SetAeffectCharge(); 

	//시간 체크 일단 . 
	DWORD			 m_OldTime;
	DWORD			 m_NowTime;

	//떨어지는  효과 
	CDropEffect		 m_EffectDrop;
	BOOL			 m_ISDamageProcess;
	
	CParticleSystem m_FireParticle;

	CParticleSystem	 m_ParticleLink[20];
	CParticleSystem	 m_AddParticleLink[2];
	BOOL			 m_bStep[20];
	int			m_Count;
	std::vector		<ENCHANT_POINT>	 m_PoingList;


private:
	bool	m_bDust;
	int		m_iPoisonBreathCount;
	int		m_iFrameBalkanLifeTime;
};


#endif