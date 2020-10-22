#ifndef __CEFROCKPRESS_H
#define __CEFROCKPRESS_H

#include "CEffectBase.h"
#include <vector>

typedef enum
{
	ROCKPRESS_START_WAIT	,				// 시작시 공중에서 대기
	ROCKPRESS_FALL			,				// 돌 떨어져요
	ROCKPRESS_LANDING_WAIT  ,				// 죽을때 
	ROCKPRESS_SELF_DESTROY 
	
}ROCKPRESS_STATE;


typedef struct s_RockPressData
{
	BOOL				ISLanding;						// 바닥 도착
	DWORD				OldTimer;						// 대기 시간
	DWORD				WaitTimer;						// 대기 시간
	D3DXVECTOR3			vPos;							// 위치
	float				StartHeight;					// 캐릭터 부터의 거리
	float				fDir;							// 속도
	float				fAlpha;
	int					Type;

	ROCKPRESS_STATE		State;

	s_RockPressData()
	{
		ISLanding = FALSE;
		State     = ROCKPRESS_START_WAIT;
		fAlpha    = 0.0f;
	}
	
}ROCKPRESS_DATA;


class CEFRockPress : public CEffectBase
{
public:
	
	         CEFRockPress();
	virtual ~CEFRockPress();

public:

	virtual int		 CheckFrame(void);
	virtual int		 Render(float dtime);							
	virtual int      Update(float dtime);
	virtual int      SetAfterEffect(void);

	static  int		 InitDevice(void);
	static  int		 DeleteDevice(void);
	
	virtual void	 Init(void);

private:

	ROCKPRESS_DATA				m_RockInfo;
	
	D3DXVECTOR3					m_vSkyEffectPos;	

#ifdef DIRECT_VERSION_9_MJH
	static LPDIRECT3DTEXTURE9	m_pRockTexture;
#else
	static LPDIRECT3DTEXTURE8	m_pRockTexture;
#endif // DIRECT_VERSION_9_MJH
	static Model			   *m_pRockModelList[3];

	ROCKPRESS_STATE				m_State;

	CParticleSystem				m_CloudParticle;
};

#endif

