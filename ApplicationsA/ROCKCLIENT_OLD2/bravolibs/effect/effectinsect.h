

#ifndef			__EFFECTINSECT_H_
#define			__EFFECTINSECT_H_

//#include	"EffectBase.h"



#define			MAX_INSECT			2
#define			MAX_BIRD			5
#define			MAX_MOVINGINSECT	80
#define			MAX_MOVINGBIRD		160


class		CInsect
{
public:

	//현재 위치,	이전 위치 
	D3DXVECTOR3			m_vInsect_Loc;
	//방향
	D3DXVECTOR3			m_vInsect_Vel;
	//위치 매트릭스 
	D3DXMATRIX			m_matInsect;

	//방향
	float				m_fInsect_Dir;
	//한방향으로가는 수명 
	float				m_fInsect_Life;

	~CInsect();
};

class		CBird
{
public:
	//현재 위치,	이전 위치 
	D3DXVECTOR3			m_vBird_Loc;
	//위치 매트릭스 
	D3DXMATRIX			m_mat_Bird;
	
	//상대적인 위치 
	float				m_fBird_Opp[2];
	//날개짓 멈춤 	
	bool				m_bStop_Bird;
	~CBird();
};




class		CInsect_Manager
{
public:
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DVERTEXBUFFER9		m_pGlimmerVB;
	LPDIRECT3DINDEXBUFFER9		m_pGlimmerIB;
	LPDIRECT3DINDEXBUFFER9		m_pButterFlyIB[2];
	LPDIRECT3DVERTEXBUFFER9		m_pButterFlyVB[2];
	LPDIRECT3DTEXTURE9			m_pInsectTex[4];
#else
	LPDIRECT3DVERTEXBUFFER8		m_pGlimmerVB;
	LPDIRECT3DINDEXBUFFER8		m_pGlimmerIB;
	LPDIRECT3DINDEXBUFFER8		m_pButterFlyIB[2];
	LPDIRECT3DVERTEXBUFFER8		m_pButterFlyVB[2];
	LPDIRECT3DTEXTURE8			m_pInsectTex[4];
#endif // DIRECT_VERSION_9_MJH

	D3DXVECTOR3			m_vInsect_Str_Pos;
	//애니메이션 매트릭스 
	D3DXMATRIX			m_matIst_Ani[2];

	CInsect				m_Insect[MAX_INSECT];

	CBird				m_Bird[MAX_BIRD];
	
	
	//랜덤으로 숫자 생성을 위한 
	DWORD				m_iBird_Num;
	//현재 새들의 포지션 
	D3DXVECTOR3			m_vBird_Pos;
	//새들의 방향 
	D3DXVECTOR3			m_vBird_Vel;
	//새들의 속도 
	float				m_fBird_Fast;
	//방향
	float				m_fBird_Dir;
	//방향의 휨 영향치 
	D3DXVECTOR3			m_vBird_Facter;
	//새들의 높이를 위한 수명 
	float				m_fBird_Life;



	float				m_fPrvTime;

	//벌레 20마리를 생성 한다 (위치 속도 방향) 처음에 단 한번 실행 
	void				Create_Insect(char* dir, int type);

	//버텍스 버퍼 안에정보를 넣시다 , 텍스쳐도 읽읍시다 
	HRESULT				Insect_Setting(char* directory, int insecttype);

	//방향과 속도를 새로 정하여 준다  정점에도착 하였을시에 
	void				Change_Insect( DWORD num );
	void				Relife_Insect( DWORD num );

	void				Change_Bird();

	//날개 움직임 에니메이션 함수 
	void				Ani_Insect();
	void				Ani_Bird();
	//방향 정해 주는 함수 
	float				Dir_Insect(D3DXVECTOR3& v);
	//음수는 양수로 바꾸는 함수 
	float				MinusToPlus(float Minus);
	//속도 브레이크 함수 
	float				LimitVel(float vel);

	//정점에 도착하지 않은 벌레는 위치값을 갱신해 준다 
	void				Update_Insect(int insectnum, float time);

	//그리자 
	HRESULT				Render_Insect(int isttype, float time);

	~CInsect_Manager();
};



extern	CInsect_Manager		g_Insect;

#endif