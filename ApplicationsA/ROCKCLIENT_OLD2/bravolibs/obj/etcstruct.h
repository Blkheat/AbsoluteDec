
#ifndef __ETC_STRUCT_H__
#define __ETC_STRUCT_H__

#include	"..\\rocklibs\\RockPCH.h"
#ifdef DIRECT_VERSION_9_MJH
#include	<D3DX9.H>
#else
#include	<D3DX8.H>
#endif // DIRECT_VERSION_9_MJH
#include	"Define.H"
#include	"..\\..\\CRenderManager.h"

//#define	SHADOW_SIZE	32
#define	SHADOW_SIZE	64
//#define	SHADOW_SIZE	128

#ifdef DIRECT_VERSION_9_MJH
extern	LPDIRECT3DDEVICE9	g_lpDevice;
#else
extern	LPDIRECT3DDEVICE8	g_lpDevice;
#endif // DIRECT_VERSION_9_MJH

//-----------------------------------------------------------------------------
// Desc: 알파
//-----------------------------------------------------------------------------
struct ALPHA_INFO
{
	bool			m_enable;
	unsigned int	m_src;
	unsigned int	m_dest;
}; 


//-----------------------------------------------------------------------------
// Desc: 바운드박스
//-----------------------------------------------------------------------------
struct BOUND_INFO
{	
	D3DXVECTOR3		m_min_vertex;	// 최소값 버텍스
	D3DXVECTOR3		m_max_vertex;	// 최대값 버텍스
};


//-----------------------------------------------------------------------------
// Desc: 애니메이션 인덱스
//-----------------------------------------------------------------------------
struct ANI_INFO
{
	int		ani_number;		// 애니메이션 인덱스
	int		sound_number;	// 사운드 인덱스
	int		sound_key;		// 사운드가 나오는 키프레임번호

	ANI_INFO()	{ ani_number = -1; sound_number = -1; sound_key = -1; }
	~ANI_INFO() {}
};


struct ANI_INFO_ACTION
{
	int		Impact_frame;		// 공격시 타격되는 프레임 
	int		range_startframe;	// 콤보 공격시 타이밍 히트 범위 시작지점 
	int		range_endframe;		// 콤보 공격시 타이밍 히트 범위 종료지점 

	ANI_INFO_ACTION() { Impact_frame = -1; range_startframe = -1; range_endframe = -1; }
	~ANI_INFO_ACTION() {}
};

//-----------------------------------------------------------------------------
// Desc: 파티클 구조체
//-----------------------------------------------------------------------------
struct ParticleInfo
{
	int				index;	// 파티클 인덱스
	int				link;	// 모델 링크 인덱스
	D3DXVECTOR3		vPos;	// 파티클 위치

	void Init()
	{
		index = -1;
		link  = -1;
		vPos = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	}
};


//-----------------------------------------------------------------------------
// Desc: 아이템 정보
//-----------------------------------------------------------------------------
struct ARMOR_INFO
{
	int				index[LINK_ITEM_COUNT];
	//by simwoosung
	int				Code[LINK_ITEM_COUNT];
	unsigned int	RHand;
	unsigned int	LHand;	
	unsigned int	Temp1;
	unsigned int	Temp2;		

	ARMOR_INFO() { Clear(); }	 
	void Clear(){ 
					RHand = 0; LHand = 0; Temp1 = 0; Temp2 = 0; 
					memset( &index, -1, sizeof(index) );
					memset( &Code, -1, sizeof(Code) ); 
				}
};


//-----------------------------------------------------------------------------
// Desc: 빌보드 정보
//-----------------------------------------------------------------------------
struct BillboardInfo
{
	int				index;	// 빌보드 인덱스
	int				link;	// 모델 링크 인덱스
	D3DXVECTOR3		vPos;	// 빌보드 위치

	void Init()
	{
		index = -1;
		link  = -1;
		vPos = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	}
};


enum NSHADOW_SIZE
{
	NSHADOW_SIZE_64  = 0,
	NSHADOW_SIZE_128,
	NSHADOW_SIZE_256,
	NSHADOW_SIZE_512,
};

#define Shadow_ReNewTime 0.5f

//-----------------------------------------------------------------------------
// Desc: 그림자 정보
//-----------------------------------------------------------------------------
struct SHADOW_INFO
{
	bool				m_IsInitTex;
	bool				m_IsRender;
	bool				m_IsOneTimeRender;
	float				m_fCumulativeTime;		//텍스쳐가 생성된후 일정시간후에 재갱신-시간 누적

#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DTEXTURE9			pTex;				// 텍스쳐 포인터
	LPDIRECT3DSURFACE9			pSurface;			// 타겟
#else
	LPDIRECT3DTEXTURE8			pTex;				// 텍스쳐 포인터
	LPDIRECT3DSURFACE8			pSurface;			// 타겟
#endif // DIRECT_VERSION_9_MJH
	LPD3DXRENDERTOSURFACE		pRenderToSurface;	// 렌더서페이스				
	D3DXMATRIX					matShadowUV;		// U,V 메트릭스
	D3DXMATRIX					matShadowProj;		// 프로젝션
	D3DXMATRIX					matWorldToLight;	// 라이트뷰
	
	int							m_ShadowSize;
	
	float						fXMax; 
	float						fYMax;

	SHADOW_INFO()	
	{
		m_ShadowSize = 64;
		Init(); 
	}

	~SHADOW_INFO()	{ Clear(); }

	void	Init()
	{			
		pTex			 = NULL;
		pSurface		 = NULL;
		pRenderToSurface = NULL;		

		m_fCumulativeTime = 0.0f;
		m_IsRender = false;
		m_IsInitTex = false;
		m_IsOneTimeRender = false;

		fXMax = 0.0f;
		fYMax = 0.0f;
	}
	void	Clear()
	{
		SAFE_RELEASE( pRenderToSurface );
		SAFE_RELEASE( pSurface );
		SAFE_RELEASE( pTex );

		Init();
	}	

	BOOL	Create()
	{
		Clear();
		
		m_fCumulativeTime = 0.0f;
		m_IsRender = false;		
		
		if( pRenderToSurface == NULL )
		{
			if( FAILED( g_RenderManager.CreateRenderToSurface( m_ShadowSize, m_ShadowSize,  D3DFMT_A8R8G8B8,
													FALSE, D3DFMT_UNKNOWN, &pRenderToSurface ) ) )
			{
				return FALSE;
			}
		}
		
		if( NULL == pTex )
		{			
			if( FAILED( g_RenderManager.CreateTexture( m_ShadowSize, m_ShadowSize, 1, 
											D3DUSAGE_RENDERTARGET,  D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &pTex ) ) )
			{
				return FALSE;
			}

			if( pTex )
			{
				if( FAILED( pTex->GetSurfaceLevel( 0, &pSurface ) ) )
				{
					return FALSE;
				}
			}		
		}

		m_IsInitTex = true;

		return TRUE;
	}

	BOOL	Create(int nSize)
	{
		Clear();
		
		m_fCumulativeTime = 0.0f;
		m_IsRender = false;
		m_IsOneTimeRender = false;
		
		m_ShadowSize = nSize;
		
		if( pRenderToSurface == NULL )
		{
			if( FAILED( g_RenderManager.CreateRenderToSurface( m_ShadowSize, m_ShadowSize,  D3DFMT_A8R8G8B8, FALSE, 
												   D3DFMT_UNKNOWN, &pRenderToSurface ) ) )
			{
				return FALSE;
			}
		}
		
		if( NULL == pTex )
		{			
			if( FAILED( g_RenderManager.CreateTexture( m_ShadowSize, m_ShadowSize, 1, D3DUSAGE_RENDERTARGET, 
										    D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &pTex ) ) )
			{
				return FALSE;
			}

			if( pTex )
			{
				if( FAILED( pTex->GetSurfaceLevel( 0, &pSurface ) ) )
				{
					return FALSE;
				}
			}
		}
		
		m_IsInitTex = true;

		return TRUE;
	}
};


//-----------------------------------------------------------------------------
// Desc: 퀘스트 테이블
//-----------------------------------------------------------------------------
#define		QUEST_ATTR_COUNT	5		// 퀘스트대상 갯수

// 퀘스트 속성
struct	ATTR
{	
	BYTE		Class;			// 퀘스트 종류
	DWORD		Index;			// 퀘스트 인덱스
	DWORD		Text;			// 퀘스트 인덱스
};

// 퀘스트 조건
struct	REQ
{	
	BYTE		Order;			// 제공방법		
	BYTE		Job;			// 제한 직업	
	BYTE		Level;			// 제한 레벨
	BYTE		Time;			// 제한 시간
	int			Quest;			// 필요 퀘스트	
	int			OrderNpc[QUEST_ATTR_COUNT];	// 제공 NPC		
	REQ()	{ Init(); }
	void	Init() { Quest = -1; memset( OrderNpc, -1, sizeof(OrderNpc) ); }
};

// 퀘스트 대상
struct	TARGET
{
	BYTE		Type[QUEST_ATTR_COUNT];
	int			Index[QUEST_ATTR_COUNT];	
	BYTE		CurCount[QUEST_ATTR_COUNT];
	BYTE		MaxCount[QUEST_ATTR_COUNT];	
	
	BYTE		Order[QUEST_ATTR_COUNT];	// 목표종류
	DWORD		MapID[QUEST_ATTR_COUNT];	// 맵 번호
	TARGET()	{ Init(); }
	void		Init() 	{ memset( Index,	-1, sizeof(Index) ); }
};


// 퀘스트 테이블
struct QUEST_TABLE
{
	ATTR		attr;
	REQ			req;
	TARGET		target;
};

/*
//-----------------------------------------------------------------------------
// Desc: 프레임스킵
//-----------------------------------------------------------------------------
class FrameSkip 
{ 
public: 
    inline void Clear() 
    { 
        SetFramePerSec( 60.0f ); 
        m_Timer = 0.0f; 
    } 
    inline void SetFramePerSec( float fps ) 
    { 
        m_SecPerFrame = 1.0f/fps; 
    } 
    
    // 원하는 프레임보다 너무 빠르면, 
    // false를 반환해서 코드를 동작시키지 않도록 한다. 
    // dt는 '초'단위 (밀리초 아님!!!) 
    inline bool Update( float dt ) 
    { 
        m_Timer += dt;       
        if( m_Timer < 0 ) 
		{
			return false;       
		}
        // 한프레임에 해당하는 시간을 뺀다. 
        m_Timer -= m_SecPerFrame; 
        return true; 
    } 
    
    // Update후에 호출해서 frame skip을 수행해야 하는지 검사한다. 
    // frame skip이 일어나야 한다면 true를 반환한다. 
    inline bool IsFrameSkip() 
    { 
        return m_Timer >= 0; 
    } 
    
    // 멤버변수와 생성/소멸자. 
    FrameSkip(){ Clear(); } 
    virtual ~FrameSkip(){} 

protected: 
    float m_Timer; 
    float m_SecPerFrame; 
}; 
*/

#endif