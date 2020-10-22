
#ifndef _ITEM_H_
#define _ITEM_H_

#include	"VFileSystem.h"	/// sooree pack 

#include	<EFFECT\\EffectSystem.h>

#define		ARROW_INDEX				96		// 화살 아이템 인덱스


//-----------------------------------------------------------------------------
// Desc: 캐릭터 아이템 
//-----------------------------------------------------------------------------
class Character_Item
{
public:		
	unsigned int		m_mod_index;		// 모델 번호
	unsigned int		m_tex_index;		// 텍스쳐 번호
	unsigned int		m_particle_count;	// 파티클갯수
#ifdef DIRECT_VERSION_9_MJH
	D3DMATERIAL9		m_Material;			// 메터리얼
#else
	D3DMATERIAL8		m_Material;			// 메터리얼
#endif // DIRECT_VERSION_9_MJH		
	ParticleInfo*		m_pParticleInfo;	// 파티클정보
	CParticleSystem*	m_pParticle;		// 파티클 포인터
	unsigned int		m_Index;			// 파일인덱스

	BYTE				m_sex;				// 성별
	BYTE				m_race;				// 종족
	BYTE				m_grav;				// 중요도
	BYTE				m_class;			// 클래스
	BYTE				m_type;				// 타입
	BYTE				m_number;			// 번호 (기본 사용때문에 -1 필요 )
	BYTE				m_range;			// 무기사정거리
	BYTE				m_tex;				// 사용하는 검기 텍스쳐	
	D3DXVECTOR3			m_AxisBottom;		// 무기 축 하
	D3DXVECTOR3			m_AxisTop;			// 무기 축 상
	BYTE				m_min;				// 검기버텍스번호
	unsigned int		m_AttackAniNum;		// 해당 아이템의 공격 에니 번호

#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DVERTEXBUFFER9		m_pNormalVB;	// 노말버텍스버퍼
	LPDIRECT3DVERTEXBUFFER9		m_pLineVB;		// 외각선버텍스버퍼
#else
	LPDIRECT3DVERTEXBUFFER8		m_pNormalVB;	// 노말버텍스버퍼
	LPDIRECT3DVERTEXBUFFER8		m_pLineVB;		// 외각선버텍스버퍼
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
// Desc: 화살 정보값 
//-----------------------------------------------------------------------------
class ARROW_TABLE
{
public:
	BYTE			btDisplay;		// 그리기
	BYTE			btLink;			// 본노드
	float			fTargetRot;		// 타겟의회전값
	long			DestTime;		// 도착시간	
	bool			bPC;			// 유저
	int				Index;			// 인덱스
	D3DXVECTOR3		vDestPos;		// 도착위치
	D3DXVECTOR3		vCurtPos;		// 시작위치
	D3DXMATRIX		matWorld;		// 현재위치

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