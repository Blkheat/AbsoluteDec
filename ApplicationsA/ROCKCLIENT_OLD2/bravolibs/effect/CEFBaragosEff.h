//...............................................................................................................
// 
//	CEFBaragosEff.h														 바라고스 관련 이펙트
//
//...............................................................................................................
#ifndef __CEFBaragosEff_H
#define __CEFBaragosEff_H

#include "CEffectBase.h"
#include "CDropEffect.h"

#include <list>
using namespace	std;

struct BRESS_POS
{
	DWORD		dInitTime;
	D3DXVECTOR3 vStartPos;
	D3DXVECTOR3 vEndPos;

	BRESS_POS()
	{
		memset(this, 0, sizeof(BRESS_POS));
	}
	
	void operator=(BRESS_POS &temp)
	{
		dInitTime = temp.dInitTime;
		vStartPos = temp.vStartPos;
		vEndPos = temp.vEndPos;
	}	
};

class CEFBaragosEff : public CEffectBase
{
public:
	CEFBaragosEff();
	virtual ~CEFBaragosEff();


public:
	enum BESS_STATE
	{
		BRESS_STATE_NONE = 0,
		BRESS_STATE_SHOT,
		BRESS_STATE_MOVE,
		BRESS_STATE_CLOSE,
	};
	
	enum THUNDER_STATE
	{
		THUNDER_STATE_NONE = 0,
		THUNDER_STATE_SHOT,
		THUNDER_STATE_MOVE,
		THUNDER_STATE_CLOSE,
	};


public:
	virtual void	 Init(void);
	virtual int		 CheckFrame(void);
	virtual int      Update(float dtime);
	virtual int		 Render(float dtime);


public:
	void	InitHowlOfDevil();
	void	InitThunderBreak();// 선더 브레이크(기모음)
	void	InitBerSerk();// 광폭화 초기화	
	
	// 프레임 번호에 다른 이펙트를 붙힌다.
	int		CheckDragonBress();
	int		CheckHowlOfDevil();
	int		CheckThunderBreak();
	int		CheckBerserk();// 광폭화 이펙트 프레임 체크

	int		UpdateDragonBress(float dtime);
	int		UpdateHowlOfDevil(float dtime);
	int		UpdateThunderBreak(float dtime);

	int		RenderDragonBress(float dtime);
	int		RenderHowlOfDevil(float dtime);
	int		RenderBerserk(float dtime);// 광폭화 그리기

	DWORD			 m_InitTime;
	DWORD			 m_RangeTime;
	DWORD			 m_PrevTime;
	DWORD			 m_GapTime;

	DWORD			 m_ParticleTime;
	float			 m_fRadius;

	D3DXVECTOR3		 m_vPreStartPos;
	D3DXVECTOR3		 m_vMoveDir;
	D3DXVECTOR3		 m_vTempPos;

	BOOL			 m_ISShot;
	BOOL			 m_ISDamageProcess;

	BESS_STATE		 m_nBressState;
	THUNDER_STATE	 m_nThunderState;

	list < BRESS_POS > m_BressParPosInfoList;	

	CParticleSystem	 m_ParticleLink[20];
	CParticleSystem	 m_AddParticleLink[2];


private:
	enum
	{
#ifdef HHM_SEALED_TEMPLE_BOSSMOB_EFFECT_LIMIT_20091207
		
		ME_BERSERK_COUNT = 4,

#else
		
		ME_BERSERK_COUNT = 12,

#endif// #ifdef HHM_SEALED_TEMPLE_BOSSMOB_EFFECT_LIMIT_20091207
	};


private:
	bool	m_bDragonBressMouseFire;// 입 주위의 화염 중 큰 화염을 온오프
	int		m_iDragonBress1132Count;	

	bool	m_bHowlOfDevilBlur;

	int		m_iThunderBreak1129Count;

	float	m_fBerserkFireEffSize;// 이펙트 사이즈
	int		m_aiBerserkFireBone[ME_BERSERK_COUNT];// 이펙트를 붙힐 뼈대 인덱스
};

#endif