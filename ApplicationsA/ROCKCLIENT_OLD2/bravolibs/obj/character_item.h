
#ifndef _ITEM_H_
#define _ITEM_H_

#include	"VFileSystem.h"	/// sooree pack 

#include	<EFFECT\\EffectSystem.h>

#define		ARROW_INDEX				96		// ȭ�� ������ �ε���


//-----------------------------------------------------------------------------
// Desc: ĳ���� ������ 
//-----------------------------------------------------------------------------
class Character_Item
{
public:		
	unsigned int		m_mod_index;		// �� ��ȣ
	unsigned int		m_tex_index;		// �ؽ��� ��ȣ
	unsigned int		m_particle_count;	// ��ƼŬ����
#ifdef DIRECT_VERSION_9_MJH
	D3DMATERIAL9		m_Material;			// ���͸���
#else
	D3DMATERIAL8		m_Material;			// ���͸���
#endif // DIRECT_VERSION_9_MJH		
	ParticleInfo*		m_pParticleInfo;	// ��ƼŬ����
	CParticleSystem*	m_pParticle;		// ��ƼŬ ������
	unsigned int		m_Index;			// �����ε���

	BYTE				m_sex;				// ����
	BYTE				m_race;				// ����
	BYTE				m_grav;				// �߿䵵
	BYTE				m_class;			// Ŭ����
	BYTE				m_type;				// Ÿ��
	BYTE				m_number;			// ��ȣ (�⺻ ��붧���� -1 �ʿ� )
	BYTE				m_range;			// ��������Ÿ�
	BYTE				m_tex;				// ����ϴ� �˱� �ؽ���	
	D3DXVECTOR3			m_AxisBottom;		// ���� �� ��
	D3DXVECTOR3			m_AxisTop;			// ���� �� ��
	BYTE				m_min;				// �˱���ؽ���ȣ
	unsigned int		m_AttackAniNum;		// �ش� �������� ���� ���� ��ȣ

#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DVERTEXBUFFER9		m_pNormalVB;	// �븻���ؽ�����
	LPDIRECT3DVERTEXBUFFER9		m_pLineVB;		// �ܰ������ؽ�����
#else
	LPDIRECT3DVERTEXBUFFER8		m_pNormalVB;	// �븻���ؽ�����
	LPDIRECT3DVERTEXBUFFER8		m_pLineVB;		// �ܰ������ؽ�����
#endif // DIRECT_VERSION_9_MJH
	
public:
	Character_Item()	{ Clear(); }
	~Character_Item()	{ Destroy(); }

	void	Clear();
	void	Destroy();	

	bool	File__Open( char* file_path );
	bool	File__OpenVF( CVFileSystem* pVF, char* file_path );	/// sooree pack

	bool	CreateVrxBuf( BYTE byType, bool bCreate );
};


//-----------------------------------------------------------------------------
// Desc: ȭ�� ������ 
//-----------------------------------------------------------------------------
class ARROW_TABLE
{
public:
	BYTE			btDisplay;		// �׸���
	BYTE			btLink;			// �����
	float			fTargetRot;		// Ÿ����ȸ����
	long			DestTime;		// �����ð�	
	bool			bPC;			// ����
	int				Index;			// �ε���
	D3DXVECTOR3		vDestPos;		// ������ġ
	D3DXVECTOR3		vCurtPos;		// ������ġ
	D3DXMATRIX		matWorld;		// ������ġ

public:
	ARROW_TABLE()	{ Clear(); }
	~ARROW_TABLE()	{ Clear(); }

	void	Clear();	
	void	Create( bool PC, int table_index, BYTE link, D3DXVECTOR3 Curt, D3DXVECTOR3 Dest );

	void	Display();	
	void	LineDisplay();
	
	float	GetTargetRot();
	int		GetTargetEvent();
	bool	GetTargetPos( D3DXVECTOR3& vPos );
	D3DXVECTOR3	GetTargetPos();
};


#endif