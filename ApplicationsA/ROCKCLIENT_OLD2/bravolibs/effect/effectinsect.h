

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

	//���� ��ġ,	���� ��ġ 
	D3DXVECTOR3			m_vInsect_Loc;
	//����
	D3DXVECTOR3			m_vInsect_Vel;
	//��ġ ��Ʈ���� 
	D3DXMATRIX			m_matInsect;

	//����
	float				m_fInsect_Dir;
	//�ѹ������ΰ��� ���� 
	float				m_fInsect_Life;

	~CInsect();
};

class		CBird
{
public:
	//���� ��ġ,	���� ��ġ 
	D3DXVECTOR3			m_vBird_Loc;
	//��ġ ��Ʈ���� 
	D3DXMATRIX			m_mat_Bird;
	
	//������� ��ġ 
	float				m_fBird_Opp[2];
	//������ ���� 	
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
	//�ִϸ��̼� ��Ʈ���� 
	D3DXMATRIX			m_matIst_Ani[2];

	CInsect				m_Insect[MAX_INSECT];

	CBird				m_Bird[MAX_BIRD];
	
	
	//�������� ���� ������ ���� 
	DWORD				m_iBird_Num;
	//���� ������ ������ 
	D3DXVECTOR3			m_vBird_Pos;
	//������ ���� 
	D3DXVECTOR3			m_vBird_Vel;
	//������ �ӵ� 
	float				m_fBird_Fast;
	//����
	float				m_fBird_Dir;
	//������ �� ����ġ 
	D3DXVECTOR3			m_vBird_Facter;
	//������ ���̸� ���� ���� 
	float				m_fBird_Life;



	float				m_fPrvTime;

	//���� 20������ ���� �Ѵ� (��ġ �ӵ� ����) ó���� �� �ѹ� ���� 
	void				Create_Insect(char* dir, int type);

	//���ؽ� ���� �ȿ������� �ֽô� , �ؽ��ĵ� �����ô� 
	HRESULT				Insect_Setting(char* directory, int insecttype);

	//����� �ӵ��� ���� ���Ͽ� �ش�  ���������� �Ͽ����ÿ� 
	void				Change_Insect( DWORD num );
	void				Relife_Insect( DWORD num );

	void				Change_Bird();

	//���� ������ ���ϸ��̼� �Լ� 
	void				Ani_Insect();
	void				Ani_Bird();
	//���� ���� �ִ� �Լ� 
	float				Dir_Insect(D3DXVECTOR3& v);
	//������ ����� �ٲٴ� �Լ� 
	float				MinusToPlus(float Minus);
	//�ӵ� �극��ũ �Լ� 
	float				LimitVel(float vel);

	//������ �������� ���� ������ ��ġ���� ������ �ش� 
	void				Update_Insect(int insectnum, float time);

	//�׸��� 
	HRESULT				Render_Insect(int isttype, float time);

	~CInsect_Manager();
};



extern	CInsect_Manager		g_Insect;

#endif