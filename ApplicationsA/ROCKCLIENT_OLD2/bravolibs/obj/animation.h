
#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include	"VFileSystem.h"	/// sooree pack 

#ifdef DIRECT_VERSION_9_MJH
#include	<D3DX9.H>
#else
#include	<D3DX8.H>
#endif // DIRECT_VERSION_9_MJH
#define		MAX_NODENAME	20

//-----------------------------------------------------------------------------
// Desc: �� Ŭ���� (�ִϸ��̼� ������ ������ �ִ�)
//       �� - ����� - �ִϸ��̼�(���)       
//-----------------------------------------------------------------------------
class Bone_Animation
{
public:	
	D3DXQUATERNION	m_qtRot;		// ���ʹϿ�
	D3DXVECTOR3		m_vPos;			// �̵���	
};


class Bone_Node
{
public:
	unsigned int 		m_self_id;					// �ڱ�ID
	D3DXMATRIX			m_matLocal;					// ���� ��Ʈ����	
	Bone_Animation*		m_pAnimation;				// �ִϸ��̼�

	Bone_Node() { Clear(); }
	void	Clear() { m_pAnimation = NULL; }
};


class Bone
{
public:
	unsigned int 	m_num_node;		// ��尹��	
	unsigned int 	m_ani_frame;	// �ִ� ������		
	Bone_Node*		m_pNode;		// ���		
	unsigned int	m_Index;		// �����ε���
	WORD			m_race;			// ���� (�з��� n_ChrRace_Millena , ���� n_ChrRace_Rain, ���� n_ChrRace_Common)
	long			m_lPrevTime;	// ������ �����ð�

public:
	Bone()	{ Clear(); }
	~Bone()	{ Destory(); }

	void	Clear();
	void	Destory();						// �ı���	

	bool	Read( char* file_path );		// �ִϸ��̼� ���� �б�
	bool	ReadVF( CVFileSystem* pVF, char* file_path );	/// sooree pack
};


#endif