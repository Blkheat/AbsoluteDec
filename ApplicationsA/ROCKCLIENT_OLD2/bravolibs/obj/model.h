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
// ���ؽ�
//-----------------------------------------------------------------------------
struct MESH_NODE_ID
{
	unsigned int	ID;			// ����ȣ
};

struct VERTEX_TABLE
{
	D3DXVECTOR3		m_vertex;	// ���ؽ�
	D3DXVECTOR3		m_normal;	// �븻	
	float			m_u, m_v;	// �ؽ��� U, V
};

//-----------------------------------------------------------------------------
// ���̽�(�ε���)
//-----------------------------------------------------------------------------
class FACE_TABLE
{
public:
	WORD	v1, v2, v3;			// ���̽�
};


//-----------------------------------------------------------------------------
// ���
//-----------------------------------------------------------------------------
class MESH_HEADER
{	
public:
	unsigned int	m_num_vertex;	// ���ؽ�����
	unsigned int	m_num_face;		// ���̽�����
	D3DXVECTOR3		m_vNormal;		// �븻��
	unsigned int	m_ani_frame;	// �ִ�������
};

//-----------------------------------------------------------------------------
// �ִϸ��̼�
//-----------------------------------------------------------------------------
class MESH_ANI
{
public:
	D3DXQUATERNION	m_qtRot;	// ���ʹϿ�
	D3DXVECTOR3		m_vPos;		// �̵���		
};


class MESH_FULL_ANI
{
public:
	D3DXQUATERNION	m_qtRot;	// ���ʹϿ�
	D3DXVECTOR3		m_vPos;		// �̵���
	D3DXVECTOR3		m_vScale;	// ������
};

// ���ؽ� ���ϸ��̼ǿ� �߰� By wxywxy
class CMesh_AniVertex
{
public:
	D3DXVECTOR3		m_vertex;
	D3DXVECTOR3		m_normal;
};

//-----------------------------------------------------------------------------
// Ŭ����
//-----------------------------------------------------------------------------
class Model
{
public:
	bool						m_bInit;			// �о���?
	unsigned int				m_ani_type;			// �ִϸ��̼�Ÿ��
	unsigned int				m_Index;			// �����ε���
	WORD						m_race;				// ���� (�з��� n_ChrRace_Millena , ���� n_ChrRace_Rain, ���� n_ChrRace_Common)
	long						m_lPrevTime;		// ������ �����ð�

public:		
	MESH_HEADER					m_header;			// ���
	VERTEX_TABLE*				m_pVertex;			// ���ؽ�
	VERTEX_XYZ_DIFFUSE_TEX1*	m_pBatchDefaultVertex;	// ����Ʈ ��ġ�� ����Ʈ ���ؽ�
	VERTEX_XYZ_DIFFUSE_TEX1*	m_pBatchVertex;		// ����Ʈ ��ġ�� ���ؽ�
	MESH_NODE_ID*				m_pNodeID;			// �����̵�	

	MESH_ANI*					m_pAnimation;		// �ִϸ��̼�
	MESH_FULL_ANI*				m_pFullAni;			// �����Ͼִϸ��̼�

	WORD*						m_pIndexList;		// �ε��� ����Ʈ
	CMesh_AniVertex**			m_ppVertexList;		// ���ؽ� ����Ʈ
	
	BOOL						m_ISOneTimeLock;
	
public:		
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DVERTEXBUFFER9		m_pVB;				// ���ؽ�����
	LPDIRECT3DINDEXBUFFER9		m_pIB;				// �ε�������
#else
	LPDIRECT3DVERTEXBUFFER8		m_pVB;				// ���ؽ�����
	LPDIRECT3DINDEXBUFFER8		m_pIB;				// �ε�������
#endif // DIRECT_VERSION_9_MJH

public:
	Model() { Clear(); }
	~Model() { Destory(); }	

	void	Clear();
	void	Destory();
	
	bool	ReadVF( CVFileSystem* pVF, char* file_path, bool create_buffer  );	/// sooree pack
	bool	Read( char* file_path, bool create_buffer  );			// �����б�	

	BOOL    ReadAndHeight( char* file_path , float &height, bool create_buffer );

	void	DeleteVertex();											// ���ؽ����� ����
};


#endif