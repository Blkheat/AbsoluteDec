

#ifndef		__EFFECTSYSTEM_H_
#define		__EFFECTSYSTEM_H_

#include "Particle.h"
#include "AnimSequence.h"
#include <mmsystem.h>
#include "Timer.h"
#include <list>
#include "CSceneManager.h"

RB_USING
#include "RockResourceManager.h"

class Character;

#pragma once


#define	MAX_PARTICLES	500
#define	MIN_SIZE		0.001f
#define	MAX_SIZE		100.0f
#define	MIN_GRAVITY		-15.0f
#define	MAX_GRAVITY		15.0f
#define	MIN_LIFETIME	0.1f
#define	MAX_LIFETIME	30.0f
#define	MIN_SPEED		0.0f
#define	MAX_SPEED		1250.0f
#define	MIN_ALPHA		0.0f
#define	MAX_ALPHA		3.0f

#define POINT_RENDER	1
#define LINE_RENDER		2
#define	SPACE_RENDER	3
#define SPECIAL			4

#define	GRAVITY			D3DXVECTOR3(0.0f, -9.8f, 0.0f)

#define RANDOM_NUM      (FLOAT)((((FLOAT)rand()/(FLOAT)RAND_MAX) - 0.5f) * 2.0f)
#define Clamp(x, min, max)  x = (x<min  ? min : x<max ? x : max)

extern int   Random(int iMin, int iMax);
extern float Random(float fMin, float fMax);


class	CParticleData
{
public:

	D3DXVECTOR3		m_vPotision;
	DWORD			m_iParType;				// 고정 
	int				m_iTexCount;			//총텍스쳐의 개수 
	int				m_iTexSpeed;			//텍스쳐 에니메이션 속도 
	FLOAT			m_fTheta;					

	FLOAT			m_fLife;				
	FLOAT			m_fLifeVar;				

	FLOAT			m_fSpeed;				
	FLOAT			m_fSpeedVar;			

	FLOAT			m_fSizeStart;			
	FLOAT			m_fSizeVar;				
	FLOAT			m_fSizeEnd;				

	FLOAT			m_fGravityStart;		
	FLOAT			m_fGravityVar;			
	FLOAT			m_fGravityEnd;			

	FLOAT			m_fAlphaStart;			
	FLOAT			m_fAlphaVar;			
	FLOAT			m_fAlphaEnd;			

	D3DXCOLOR		m_ColorStart;			
	D3DXCOLOR		m_ColorVar;				
	D3DXCOLOR		m_ColorEnd;				

	float			m_fAttractPower;		

	char			m_cTexture[64];			
	bool			m_bIsMoving;			
	float			m_fMovingRadius;		
	float			m_fMovingTime;			
	int				m_iMovingDir;			
	
	float			fParticlesNeeded;		//생산량 
	unsigned int	m_uParticlesPerSec;

	bool			m_bIsInstantPlay;			

	FLOAT			m_fRadius;
	FLOAT			m_fPlane_x, m_fPlane_y, m_fPlane_z;
	INT				m_iLocation_Select;

	
	DWORD			m_iRenderingLev;		//중요도와 렌더링 방식 
	
	float			m_fTotalLife;

	bool			m_bLockon;				
	
	bool			m_bIsColliding;			
	bool			m_bIsAttractive;		

	int				m_iMovingType;
	int				m_iMovingSpeedType;

	BYTE			m_SrcBlend;				// 브렌딩 소스
	BYTE			m_DestBlend;			// 블렌딩 데스트
	
	unsigned long	m_HashCode;	
	
public:

	HRESULT		Create( int effect_create );
	void		FileOpen(char *FileName);

public:
	
	//텍스쳐 포인터  
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DTEXTURE9	m_pTexture;
#else
	LPDIRECT3DTEXTURE8	m_pTexture;
#endif // DIRECT_VERSION_9_MJH
	CAnimSequence	  **m_ppAniTextrue;

	char				m_cTexName[ 256 ];	// 보통 텍스쳐면 이름 
	bool				m_bTexAni;			// 텍스쳐 에니메이션이냐.
	int					m_iTexType;			// 멀티텍스쳐넘버

	D3DXVECTOR3			m_vGravityStart;
	D3DXVECTOR3			m_vGravityVar;
	D3DXVECTOR3			m_vGravityEnd;


	float				m_fHeight;	

	void				LoadTexture( char* path );


	CParticleData() 
	{
		m_SrcBlend     = ( BYTE ) D3DBLEND_SRCALPHA;
		m_DestBlend	   = ( BYTE ) D3DBLEND_DESTALPHA;
		m_pTexture     = NULL;
		m_ppAniTextrue = NULL;

		m_HashCode = 0;
	}

	~CParticleData() 
	{
		m_pTexture	   = NULL;
		m_ppAniTextrue = NULL;
	}
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DTEXTURE9 GetTexture();
#else
	LPDIRECT3DTEXTURE8 GetTexture();
#endif // DIRECT_VERSION_9_MJH
};


class	CParticleSystem
{
public:

	int				m_iEffSetCon;						// 이펙트 셋 번호
	D3DXVECTOR3		m_vAddLocation;
	
	///-- By Simwoosung
	BOOL			m_bApplyVel;						// 속도 적용여부
	BOOL			m_IsPosFix;							// 위치 고정여부
	int				m_nPlayerDistance;					// 플레이어와의 거리

	BOOL			m_IsParticleApplyDis;				// 거리에 영향을 받느냐? 

public:
	
	FLOAT			m_fAge;

	D3DXVECTOR3		m_vLocation;
	D3DXVECTOR3		m_vPrevLocation;
	D3DXVECTOR3		m_vVelocity;
	D3DXVECTOR3		m_vMoveLocation;
	D3DXVECTOR3		m_vAttractLocation;

	///-- By simwoosung
	D3DXVECTOR3		m_vStartPos;						// 생성 시작위치
	
	D3DXVECTOR3		m_vAttackSrcDir;					// 공격 주체 방향
	Character		*m_pAttackUnit;
	BOOL			m_IsDirAttatack;
	BOOL			m_IsGUnitLinkBoneAndSizeFix;
	
	BYTE			m_aDirectLinkBone;
	
	FLOAT			m_fEmissionResidue;					//만들고 남는 값 	
	unsigned int	m_uAliveParticle;


	bool		m_bIsCreate;
	bool		m_bIsSuppressed;
	bool		m_bIsRender;

	float		m_fCreateParTime;

	long		m_lLockonUniq;

	//Attraction을 사용할때 위치를 받아 오기 위한 함수.
	void		LoadReset();

	DWORD		FtoDW( FLOAT f ) { return *((DWORD*)&f); }

	D3DXMATRIX		m_LinkMat;
//	D3DXVECTOR3		m_vLinkPos;
	bool			m_bLinkOk;

	BOOL		    Update( FLOAT fTimeDelta );
	BOOL	        UpdateLink( FLOAT fTimeDelta, BOOL IsInit = FALSE );

	void			SetLinkPos(D3DXVECTOR3 vpos)	
	{
		m_LinkMat._41 = vpos.x;
		m_LinkMat._42 = vpos.y;
		m_LinkMat._43 = vpos.z;

		if(!m_IsPosFix)
		{
			if(m_bApplyVel)
			{
				m_vVelocity = vpos - m_vStartPos;
			}
			m_vStartPos = vpos;
		}
	}

	void			MakeLink(CParticleData *pData)
	{
		if( m_pReference != NULL )
		{
			if( m_pReference != pData )
			{
				std::list	<CParticle *>::iterator i = m_ParticleList.begin();
	
				for( ; i!= m_ParticleList.end() ; )
				{
					CParticle * pData = (*i);				
					m_ParticlePool.DeleteMember( pData );
					i++;
				}
				
				m_ParticleList.clear();
			}
		}		
		
		m_pReference	    = pData;

		m_vMoveLocation	= D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
		m_bIsSuppressed	= false;
		m_bIsCreate		= true;
		m_lUnique		= -1;
		m_iIndex		= -1;
		m_fSize			= 1.0f;

		D3DXMatrixIdentity( &m_LinkMat );
	}
		
	
	HRESULT		Render();
	HRESULT		InvalidateDeviceObjects();

	//CParticle*		m_pParticleHead;
	//CParticle*		m_pParticleTail;
	//CParticle*		m_pPar;

	std::list <CParticle *> m_ParticleList;
	CRockResourceManager < CParticle >  m_ParticlePool;
	
	// supersong edited
	bool			m_bIsVisible;
	bool			m_bIsReady;
	float			m_fRotTheta;
	short			m_nIdx;

	CParticleData  *m_pReference;

	//CParticleSystem*	m_pSystemNext;
	//CParticleSystem*	m_pSystemPrev;

	HRESULT			RenderLoop();
	HRESULT			RenderLock();
	HRESULT			RenderBatchDraw();

	void			AddParticle(CParticle* pParticle);
	void			Update(CParticle* pParticle,FLOAT fTimeDelta);

	void			DeleteSystem();

//	int				testh;
//	int				m_iCalculateNum;
	
//	int				GetLink()			{ return m_iLink; }
//	void			SetLink( int link )	{ m_iLink = link; }

	int				number;

	void			Destroy();

public:

	//땅바닥에서 튀길수 있느냐는 설정 (추가로 땅바닥 높이Y값을 정해야 한다.)
	bool		IsColliding();
	//분출되었던 파티클들을 다시 모으는 작용을 하냐?
	bool		IsAttractive();


public:
	
	// 파티클 시스템이 생성될때 씬 메니저의 상태
	e_SCENE_MODE	m_SceneMode;

	bool			m_bLink;
	int				m_iLink;	

	CTimer			m_Timer;
	CTimer &Timer() { return(m_Timer); }

	long			m_lUnique;		// 사람 유니크
	int				m_iIndex;		// 인덱스

	float			m_fSize;		// 사이즈

	float			fThetaHoriz;
	float			fThetaVert;

	Character	   *m_pSrc;			// 위치 얻어올때 사용
	
	float			m_fSetRadian;	// 첫 시작시 고정된 각도로 처리할때
	
	D3DXMATRIX		m_mLinkBoneMatrix;
	
	 CParticleSystem();
	~CParticleSystem();

	void operator=(const CParticleSystem &Src)
	{
		memcpy( this , (CParticleSystem*)&Src , sizeof( CParticleSystem ) );
	}

};
extern CParticleSystem g_ParticleSystem;


#endif
