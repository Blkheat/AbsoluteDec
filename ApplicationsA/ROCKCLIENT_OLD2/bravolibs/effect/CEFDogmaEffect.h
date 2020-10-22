//...............................................................................................................
// 
//	CEFDogmaEffect.h														 도그마 베인 관련 이펙트
//
//...............................................................................................................
#ifndef __CEFDogmaEffect_H
#define __CEFDogmaEffect_H

#include "CEffectBase.h"
#include "CDropEffect.h"

class CEFDogmaEffect : public CEffectBase
{
public:
	CEFDogmaEffect();
	virtual ~CEFDogmaEffect();


public:
	virtual void	 Init(void);
	virtual int		 CheckFrame(void);
	virtual int      Update(float dtime);
	virtual int		 Render(float dtime);
	virtual HRESULT	 InvalidateDeviceObject();
#ifdef DIRECT_VERSION_9_MJH
	virtual HRESULT	 RestoreDeviceObject (LPDIRECT3DDEVICE9 pDevice);
#else
	virtual HRESULT	 RestoreDeviceObject (LPDIRECT3DDEVICE8 pDevice);
#endif // DIRECT_VERSION_9_MJH


public:
	void	InitDogmaFireRain();
	void	InitDogmaFrameStorm();// 이펙트 실행 안됨
	void	InitDogmaWrathOneCut();// 분노의 일력
	void	InitDogmaSwordFlash();// 검진
	void	InitDogmaFootQuake();// 발구르기
	void	InitDogmaWrathOneCut2();// 분노의 일격2
	void	InitDogmaCircleSword();
	void	InitMasterDogmaAEffect();// 변신 후 화염이펙트 초기화

	int		CheckDogmaFireRain();
	int		CheckDogmaFrameStorm();
	int		CheckDogmaWrathOneCut();
	int		CheckDogmaSwordFlash();
	int		CheckDogamFootQuake();
	int		CheckDogmaWrathOneCut2();
	int		CheckDogmaCircleSword();

	int		UpdateDogmaFireRain(float dtime);
	int		UpdateDogmaWrathOneCut(float dtime);
	int		UpdateDogmaSwordFlash(float dtime);
	int		UpdateDogmaFootQuake(float dtime);
	int		UpdateDogmaWrathOneCut2(float dtime);
	int		UpdateDogmaCircleSword(float dtime);
	int		UpdateMasterDogmaAEffect(float dtime);
	
	int		RenderDogmaFireRain(float dtime);
	int		RenderDogmaFrameStorm(float dtime);
	int		RenderMasterDogmaAEffect(float dtime);// 변신 후 화염이펙트 그린다

	int		SetAEffectDogmaCircleSword();


public:
	float			 m_fRadius;
	
	DWORD			 m_InitTime;
	DWORD			 m_RangeTime;
	DWORD			 m_PrevTime;
	DWORD			 m_GapTime;

	BOOL			 m_ISShot;
	BOOL			 m_ISDamageProcess;

	CDropEffect		 m_EffectDrop;
	CParticleSystem  m_ParticleData;

	CParticleSystem	 m_ParticleLink[2];
	CTail			 m_SwordTail;			///-- 검기


private:
	enum
	{
#ifdef HHM_SEALED_TEMPLE_BOSSMOB_EFFECT_LIMIT_20091207
		
		ME_MASTER_FIRE_EFFECT_COUNT = 2,

#else

		ME_MASTER_FIRE_EFFECT_COUNT = 6,

#endif// #ifdef HHM_SEALED_TEMPLE_BOSSMOB_EFFECT_LIMIT_20091207
	};


private:
	int				 m_iFireRainCount;
	bool			 m_bWalkDust;
	bool			 m_bWrathOneCut2;

	CParticleSystem	 m_kMasterFireLink[ME_MASTER_FIRE_EFFECT_COUNT]; 
	float			 m_fMasterFireSize;
	int				 m_aiMasterFireBone[ME_MASTER_FIRE_EFFECT_COUNT];// 변신 후 화염이펙트를 붙힐 뼈대 인덱스
};

#endif