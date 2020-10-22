
#ifndef _OBJECT_H_
#define _OBJECT_H_

#include	"VFileSystem.h"	/// sooree pack 

#include	<EFFECT\\EffectSystem.h>
#include	<EFFECT\\Billboard.h>
#include	"ObjBase.H"


//-----------------------------------------------------------------------------
// Desc: �ؽ��ľִϸ��̼� ����ü 
//-----------------------------------------------------------------------------
struct TexAniInfo
{
	int					index;			// ��	�ε���
	int					frame;			// ������
	unsigned int		number[5];		// �ؽ��� �ε���
	unsigned int		count;			// �ؽ��� ����		

	void Init()
	{
		index	= -1;
		count	= 0;	
		frame	= -1;
		memset( &number, 0, sizeof(number) );	
	}
};



//-----------------------------------------------------------------------------
// Desc: ������Ʈ �ִϸ��̼�, ��Ʈ���� 
//-----------------------------------------------------------------------------
class Object_AniPos	:  public node_idx
{
public:	
	SHADOW_INFO			m_Shadow;   			// �׸��� Ŭ����
	TexAniInfo*			m_pTexAni;				// �ؽ��ľִϸ��̼� ������	
	unsigned int		m_texani_count;			// �ؽ��ľִϸ��̼� ����

	CParticleSystem*	m_pParticle;			// ��ƼŬ ������
	ParticleInfo*		m_pParticleInfo;		// ��ƼŬ ����
	unsigned int		m_particle_count;		// ��ƼŬ ����

	CBillboard*			m_pBillboard;			// 5�� ���� ������ �ε� ����.
	BillboardInfo*		m_pBillboardInfo;		// ����Ǵ� ������ ����
	unsigned int		m_billboard_count;		// ������ ����	

public:
	D3DXMATRIX			m_matPos;				// ������Ʈ ��ġ
	D3DXMATRIX			m_matAni;				// �ִϸ��̼� ��Ʈ����		
	D3DXMATRIX			m_matRot;				// ȸ�� ��Ʈ����
#ifdef DIRECT_VERSION_9_MJH
	D3DMATERIAL9		m_Material;				// ���͸��� 
#else
	D3DMATERIAL8		m_Material;				// ���͸��� 
#endif // DIRECT_VERSION_9_MJH	
	
	float				m_scale;				// �����ϰ�
	bool				m_bRotate;				// ȸ����
	int					m_ani_frame;			// ���� ������	
	float				m_middle_time;			// 1������ ~ 2������ ������ �߰��� ���� ( 0.0 ~ 1.0 ���� )	
	long				m_pre_time;				// ���� �������� �ð�
	unsigned int		m_skip_frame;			// ���� �������� �ð��� ���� �ð� ����
	long				m_lPreSoundTime;		// ���� ���Ÿ��
	
	bool				m_bIsVisible;			// is object in frustum?		
	float				m_fDistance;			// supersong edited - distance from lookat pos		
	BYTE				m_LightType;			// ����ƮŸ��
	long				m_lLightTime;			// ����Ʈ����ð�
	bool				m_bIsParticleAlive;		// ��ƼŬ����ִ�?
	bool				m_bIsBillboardAlive;	// ��ƼŬ����ִ�?

	// animation blending
	BYTE				m_btBlended;			// ĳ���͸� ���ȴ���?
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
// Desc: ������Ʈ �Ӽ�
//-----------------------------------------------------------------------------
class Object_Attr
{
public:	
	bool		m_bNight;			// �� ���¿� ���� ����������
	bool		m_bAction;			// ĳ���� ��ġ�� ���� �ִϸ��̼� ����
	bool		m_bShadow;			// �׸��� ���� ����
	bool		m_bDay;				// �� ���¿� ���� ����������
public:
	Object_Attr()	{ m_bNight = false; m_bAction = false; m_bShadow = false; m_bDay = false; }	
};


//-----------------------------------------------------------------------------
// Desc: ������Ʈ Ŭ����
//-----------------------------------------------------------------------------
class Object	
{	
public:	
	D3DXVECTOR3			m_vLight;				// ����Ʈ��ġ
	bool				m_bShadowDisplay;		// �׸��� �׸��� �÷���

	Object_AniPos*		m_pAniPos;				// �ִϸ��̼� & ��ġ ����
	unsigned int		m_child_count;			// ������Ʈ ����
	unsigned int		m_mod_count;			// �� ����
	int*				m_pTex_Index;			// �ؽ��� ��ȣ

	int					GetLodAttr(void)	{ return m_nLodAttr; }
	int					GetISBlend(void)	{ return m_ISBlend;}
	void				ReSetBoundBox(D3DXVECTOR3 & min, D3DXVECTOR3 & max);
	
	Object_Attr*		m_pAttr;				// ������Ʈ �Ӽ�

protected:	
#ifdef DIRECT_VERSION_9_MJH
	D3DMATERIAL9		m_Material;				// ���͸���
#else
	D3DMATERIAL8		m_Material;				// ���͸���
#endif // DIRECT_VERSION_9_MJH		
	int*				m_pMod_Index;			// ���ε���		
	int*				m_pAni_Type;			// �ִϸ��̼� ����			
	int					m_ani_index;			// �ִϸ��̼� �ε���
	int					m_nLodAttr;				// LOD�Ӽ�
	int					m_ISBlend;
	BYTE				m_byBlendAttr;			// �����Ӽ�

protected:	
	int					m_sound_time;			// ���� ���Ÿ��
	unsigned int		m_sound_index;			// ���� �ε���					

public:
	bool				m_bScale;
	bool				m_bAniPlay;				// �ִϸ��̼� �÷���
	float				m_ani_time;				// �ִϸ��̼� Ÿ��	
	unsigned int		m_end_frame;			// ������ ������			

public:
	BOUND_INFO			m_Bound;				// �ٿ�� �ڽ�
	ALPHA_INFO			m_alpha;				// ���ĺ���		
	

protected:
	bool	AlphaProcess();								// ���ĺ��� ����
	void	AnimationProcess( int obj );				// �ִϸ��̼� ������ó��
	void	BoneAnimation( int obj, int mod );			// �޽��ִϸ��̼�
	void	MeshAnimation( int obj, int mod );			// ����ũ ���
	void	MouseAnimation( int obj, int mod );			// ���콺�ִϸ��̼�
	void	VertexAnimation( int obj, int mod );		// ���ؽ� ���ϸ��̼� By wxywxy
	bool	GetActionAni();								// �׼� �ִϸ��̼��ִ�?
	
public:
	Object()	{ Clear(); }
	~Object()	{ Destroy(); }

	//-----------------------------------------------------------------------------
	// Desc: ����, �ı�, �����б�
	//-----------------------------------------------------------------------------
	void	Clear();
	void	Destroy();

	bool	File__Open( const char* file_path, bool bEffect = false );		// ������Ʈ ���� �б�
	/// sooree pack
	bool	File__OpenVF( CVFileSystem* pVF, const char* file_path, bool bEffect = false );		// ������Ʈ ���� �б�

	//-----------------------------------------------------------------------------
	// Desc: ���÷���
	//-----------------------------------------------------------------------------
	void	Display();														// �Ϲݿ�����Ʈ	
	void	Display( int child );
	void	DisplayBound(int child);	
	// ������Ʈ �׸���
	void	EffectDisplayUpdate();											// ����Ʈ
	void	BillboardDisplayUpdate();										// ������ ����Ʈ 
	void	Render( int obj, int mod );										// ������	
	void	RenderBound( int obj, int mod );

	//-----------------------------------------------------------------------------
	// Desc: ����Ʈ ���÷���
	//-----------------------------------------------------------------------------
	void	EffectRender( int obj, float alpha = 1.0f );
	void	EffectMeshAnimation( int obj, int mod );

	//-----------------------------------------------------------------------------
	// Desc: LOD�� ���� ������ ������
	//-----------------------------------------------------------------------------	
	void	MaterialBlend( int index );
	void	BilDisplay( Object* pObj, int child );		

	//-----------------------------------------------------------------------------
	// Desc: �׸���
	//-----------------------------------------------------------------------------
	void	ShadowMake();			// �׸��� �����
	void	ShadowDisplay();		// �׸��� �׸���
	bool	GetAlpahEnable();		// ���ĺ����ϴ�?
			
	D3DXVECTOR3	GetBoundMinPos()	{ return m_Bound.m_min_vertex; }		// �ٿ��ڽ� Max�� ����
	D3DXVECTOR3	GetBoundMaxPos()	{ return m_Bound.m_max_vertex; }		// �ٿ��ڽ� Min�� ����
			
	bool		m_bIsLoaded;				// is obj loaded?
	DWORD		m_dwIndex;					// obj index	
	void		SetupIndex(DWORD index)	{ m_dwIndex = index; }
	DWORD		GetIndex()				{ return m_dwIndex; }	

	CRenderBound	m_CollisionBox;
};


#endif