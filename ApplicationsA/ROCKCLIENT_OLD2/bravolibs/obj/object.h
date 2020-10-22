
#ifndef _OBJECT_H_
#define _OBJECT_H_

#include	"VFileSystem.h"	/// sooree pack 

#include	<EFFECT\\EffectSystem.h>
#include	<EFFECT\\Billboard.h>
#include	"ObjBase.H"


//-----------------------------------------------------------------------------
// Desc: 텍스쳐애니메이션 구조체 
//-----------------------------------------------------------------------------
struct TexAniInfo
{
	int					index;			// 모델	인덱스
	int					frame;			// 프레임
	unsigned int		number[5];		// 텍스쳐 인덱스
	unsigned int		count;			// 텍스쳐 갯수		

	void Init()
	{
		index	= -1;
		count	= 0;	
		frame	= -1;
		memset( &number, 0, sizeof(number) );	
	}
};



//-----------------------------------------------------------------------------
// Desc: 오브젝트 애니메이션, 메트릭스 
//-----------------------------------------------------------------------------
class Object_AniPos	:  public node_idx
{
public:	
	SHADOW_INFO			m_Shadow;   			// 그림자 클래스
	TexAniInfo*			m_pTexAni;				// 텍스쳐애니메이션 포인터	
	unsigned int		m_texani_count;			// 텍스쳐애니메이션 갯수

	CParticleSystem*	m_pParticle;			// 파티클 포인터
	ParticleInfo*		m_pParticleInfo;		// 파티클 정보
	unsigned int		m_particle_count;		// 파티클 갯수

	CBillboard*			m_pBillboard;			// 5개 까지 빌보드 로딩 가능.
	BillboardInfo*		m_pBillboardInfo;		// 저장되는 빌보드 정보
	unsigned int		m_billboard_count;		// 빌보드 갯수	

public:
	D3DXMATRIX			m_matPos;				// 오브젝트 위치
	D3DXMATRIX			m_matAni;				// 애니메이션 메트릭스		
	D3DXMATRIX			m_matRot;				// 회전 메트릭스
#ifdef DIRECT_VERSION_9_MJH
	D3DMATERIAL9		m_Material;				// 메터리얼 
#else
	D3DMATERIAL8		m_Material;				// 메터리얼 
#endif // DIRECT_VERSION_9_MJH	
	
	float				m_scale;				// 스케일값
	bool				m_bRotate;				// 회전값
	int					m_ani_frame;			// 현재 프레임	
	float				m_middle_time;			// 1프레임 ~ 2프레임 사이의 중간값 저장 ( 0.0 ~ 1.0 사이 )	
	long				m_pre_time;				// 이전 렌더링한 시간
	unsigned int		m_skip_frame;			// 이전 렌더링한 시간과 현재 시간 차이
	long				m_lPreSoundTime;		// 사운드 출력타임
	
	bool				m_bIsVisible;			// is object in frustum?		
	float				m_fDistance;			// supersong edited - distance from lookat pos		
	BYTE				m_LightType;			// 라이트타입
	long				m_lLightTime;			// 라이트변경시간
	bool				m_bIsParticleAlive;		// 파티클살아있니?
	bool				m_bIsBillboardAlive;	// 파티클살아있니?

	// animation blending
	BYTE				m_btBlended;			// 캐릭터를 가렸는지?
	long				m_lCnt;

	D3DXVECTOR3			m_vBoundList[8];		// Cal Bounding Point By wxywxy
	D3DXPLANE			m_Plane[6];
	D3DXVECTOR3			m_vMin;
	D3DXVECTOR3			m_vMax;
	D3DXVECTOR3			m_vSize;
	
public:
	Object_AniPos()		{ Clear(); m_iEffSetCon		= -1;}
	~Object_AniPos()	{ Destroy(); }

	void	Clear();
	void	Destroy();	
	void	SetPosTM( const D3DXMATRIX& mat );	
	D3DXMATRIX	GetPosTM()	{ return m_matPos; }
	void	SetPosTM2( D3DXMATRIX mat );
	void	SetPosTM3( D3DXMATRIX mat );

public:
	int				m_iEffSetCon;
};


//-----------------------------------------------------------------------------
// Desc: 오브젝트 속성
//-----------------------------------------------------------------------------
class Object_Attr
{
public:	
	bool		m_bNight;			// 밤 상태에 따라서 렌더링여부
	bool		m_bAction;			// 캐릭터 위치에 따라서 애니메이션 여부
	bool		m_bShadow;			// 그림자 생성 여부
	bool		m_bDay;				// 낮 상태에 따라서 렌더링여부
public:
	Object_Attr()	{ m_bNight = false; m_bAction = false; m_bShadow = false; m_bDay = false; }	
};


//-----------------------------------------------------------------------------
// Desc: 오브젝트 클래스
//-----------------------------------------------------------------------------
class Object	
{	
public:	
	D3DXVECTOR3			m_vLight;				// 라이트위치
	bool				m_bShadowDisplay;		// 그림자 그리기 플래그

	Object_AniPos*		m_pAniPos;				// 애니메이션 & 위치 정보
	unsigned int		m_child_count;			// 오브젝트 갯수
	unsigned int		m_mod_count;			// 모델 갯수
	int*				m_pTex_Index;			// 텍스쳐 번호

	int					GetLodAttr(void)	{ return m_nLodAttr; }
	int					GetISBlend(void)	{ return m_ISBlend;}
	void				ReSetBoundBox(D3DXVECTOR3 & min, D3DXVECTOR3 & max);
	
	Object_Attr*		m_pAttr;				// 오브젝트 속성

protected:	
#ifdef DIRECT_VERSION_9_MJH
	D3DMATERIAL9		m_Material;				// 메터리얼
#else
	D3DMATERIAL8		m_Material;				// 메터리얼
#endif // DIRECT_VERSION_9_MJH		
	int*				m_pMod_Index;			// 모델인덱스		
	int*				m_pAni_Type;			// 애니메이션 종류			
	int					m_ani_index;			// 애니메이션 인덱스
	int					m_nLodAttr;				// LOD속성
	int					m_ISBlend;
	BYTE				m_byBlendAttr;			// 블렌딩속성

protected:	
	int					m_sound_time;			// 사운드 출력타임
	unsigned int		m_sound_index;			// 사운드 인덱스					

public:
	bool				m_bScale;
	bool				m_bAniPlay;				// 애니메이션 플레이
	float				m_ani_time;				// 애니메이션 타임	
	unsigned int		m_end_frame;			// 마지막 프레임			

public:
	BOUND_INFO			m_Bound;				// 바운드 박스
	ALPHA_INFO			m_alpha;				// 알파블렌딩		
	

protected:
	bool	AlphaProcess();								// 알파블렌딩 셋팅
	void	AnimationProcess( int obj );				// 애니메이션 프레임처리
	void	BoneAnimation( int obj, int mod );			// 메쉬애니메이션
	void	MeshAnimation( int obj, int mod );			// 피지크 계산
	void	MouseAnimation( int obj, int mod );			// 마우스애니메이션
	void	VertexAnimation( int obj, int mod );		// 버텍스 에니메이션 By wxywxy
	bool	GetActionAni();								// 액션 애니메이션있니?
	
public:
	Object()	{ Clear(); }
	~Object()	{ Destroy(); }

	//-----------------------------------------------------------------------------
	// Desc: 생성, 파괴, 파일읽기
	//-----------------------------------------------------------------------------
	void	Clear();
	void	Destroy();

	bool	File__Open( const char* file_path, bool bEffect = false );		// 오브젝트 파일 읽기
	/// sooree pack
	bool	File__OpenVF( CVFileSystem* pVF, const char* file_path, bool bEffect = false );		// 오브젝트 파일 읽기

	//-----------------------------------------------------------------------------
	// Desc: 디스플레이
	//-----------------------------------------------------------------------------
	void	Display();														// 일반오브젝트	
	void	Display( int child );
	void	DisplayBound(int child);	
	// 오브젝트 그리기
	void	EffectDisplayUpdate();											// 이펙트
	void	BillboardDisplayUpdate();										// 빌보드 이펙트 
	void	Render( int obj, int mod );										// 렌더링	
	void	RenderBound( int obj, int mod );

	//-----------------------------------------------------------------------------
	// Desc: 이펙트 디스플레이
	//-----------------------------------------------------------------------------
	void	EffectRender( int obj, float alpha = 1.0f );
	void	EffectMeshAnimation( int obj, int mod );

	//-----------------------------------------------------------------------------
	// Desc: LOD로 인한 빌보드 렌더링
	//-----------------------------------------------------------------------------	
	void	MaterialBlend( int index );
	void	BilDisplay( Object* pObj, int child );		

	//-----------------------------------------------------------------------------
	// Desc: 그림자
	//-----------------------------------------------------------------------------
	void	ShadowMake();			// 그림자 만들기
	void	ShadowDisplay();		// 그림자 그리기
	bool	GetAlpahEnable();		// 알파블렌딩하니?
			
	D3DXVECTOR3	GetBoundMinPos()	{ return m_Bound.m_min_vertex; }		// 바운드박스 Max값 리턴
	D3DXVECTOR3	GetBoundMaxPos()	{ return m_Bound.m_max_vertex; }		// 바운드박스 Min값 리턴
			
	bool		m_bIsLoaded;				// is obj loaded?
	DWORD		m_dwIndex;					// obj index	
	void		SetupIndex(DWORD index)	{ m_dwIndex = index; }
	DWORD		GetIndex()				{ return m_dwIndex; }	

	CRenderBound	m_CollisionBox;
};


#endif