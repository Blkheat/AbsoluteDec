#ifndef _MODEL_H_
#define _MODEL_H_

#include <RockPCH.h>

#include	"VFileSystem.h"	/// sooree pack 

#ifdef DIRECT_VERSION_9_MJH
#include	<D3DX9.H>
#else
#include	<D3DX8.H>
#endif // DIRECT_VERSION_9_MJH 

#define		MAX_NODENAME	20

#include <EFFECT\\EffectVertex.h>
//-----------------------------------------------------------------------------
// 버텍스
//-----------------------------------------------------------------------------
struct MESH_NODE_ID
{
	unsigned int	ID;			// 본번호
};

struct VERTEX_TABLE
{
	D3DXVECTOR3		m_vertex;	// 버텍스
	D3DXVECTOR3		m_normal;	// 노말	
	float			m_u, m_v;	// 텍스쳐 U, V
};

//-----------------------------------------------------------------------------
// 페이스(인덱스)
//-----------------------------------------------------------------------------
class FACE_TABLE
{
public:
	WORD	v1, v2, v3;			// 페이스
};


//-----------------------------------------------------------------------------
// 헤더
//-----------------------------------------------------------------------------
class MESH_HEADER
{	
public:
	unsigned int	m_num_vertex;	// 버텍스갯수
	unsigned int	m_num_face;		// 페이스갯수
	D3DXVECTOR3		m_vNormal;		// 노말값
	unsigned int	m_ani_frame;	// 애니프레임
};

//-----------------------------------------------------------------------------
// 애니메이션
//-----------------------------------------------------------------------------
class MESH_ANI
{
public:
	D3DXQUATERNION	m_qtRot;	// 쿼터니온
	D3DXVECTOR3		m_vPos;		// 이동값		
};


class MESH_FULL_ANI
{
public:
	D3DXQUATERNION	m_qtRot;	// 쿼터니온
	D3DXVECTOR3		m_vPos;		// 이동값
	D3DXVECTOR3		m_vScale;	// 스케일
};

// 버텍스 에니메이션용 추가 By wxywxy
class CMesh_AniVertex
{
public:
	D3DXVECTOR3		m_vertex;
	D3DXVECTOR3		m_normal;
};

//-----------------------------------------------------------------------------
// 클래스
//-----------------------------------------------------------------------------
class Model
{
public:
	bool						m_bInit;			// 읽었니?
	unsigned int				m_ani_type;			// 애니메이션타입
	unsigned int				m_Index;			// 파일인덱스
	WORD						m_race;				// 종족 (밀레나 n_ChrRace_Millena , 레인 n_ChrRace_Rain, 공통 n_ChrRace_Common)
	long						m_lPrevTime;		// 마지막 참조시간

public:		
	MESH_HEADER					m_header;			// 헤더
	VERTEX_TABLE*				m_pVertex;			// 버텍스
	VERTEX_XYZ_DIFFUSE_TEX1*	m_pBatchDefaultVertex;	// 이펙트 배치용 디폴트 버텍스
	VERTEX_XYZ_DIFFUSE_TEX1*	m_pBatchVertex;		// 이펙트 배치용 버텍스
	MESH_NODE_ID*				m_pNodeID;			// 노드아이디	

	MESH_ANI*					m_pAnimation;		// 애니메이션
	MESH_FULL_ANI*				m_pFullAni;			// 스케일애니메이션

	WORD*						m_pIndexList;		// 인덱스 리스트
	CMesh_AniVertex**			m_ppVertexList;		// 버텍스 리스트
	
	BOOL						m_ISOneTimeLock;
	
public:		
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DVERTEXBUFFER9		m_pVB;				// 버텍스버퍼
	LPDIRECT3DINDEXBUFFER9		m_pIB;				// 인덱스버퍼
#else
	LPDIRECT3DVERTEXBUFFER8		m_pVB;				// 버텍스버퍼
	LPDIRECT3DINDEXBUFFER8		m_pIB;				// 인덱스버퍼
#endif // DIRECT_VERSION_9_MJH

public:
	Model() { Clear(); }
	~Model() { Destory(); }	

	void	Clear();
	void	Destory();
	
	bool	ReadVF( CVFileSystem* pVF, char* file_path, bool create_buffer  );	/// sooree pack
	bool	Read( char* file_path, bool create_buffer  );			// 파일읽기	

	BOOL    ReadAndHeight( char* file_path , float &height, bool create_buffer );

	void	DeleteVertex();											// 버텍스버퍼 삭제
};


#endif