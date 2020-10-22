
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
// Desc: 본 클래스 (애니메이션 정보를 가지고 있다)
//       본 - 본노드 - 애니메이션(행렬)       
//-----------------------------------------------------------------------------
class Bone_Animation
{
public:	
	D3DXQUATERNION	m_qtRot;		// 쿼터니온
	D3DXVECTOR3		m_vPos;			// 이동값	
};


class Bone_Node
{
public:
	unsigned int 		m_self_id;					// 자기ID
	D3DXMATRIX			m_matLocal;					// 로컬 메트릭스	
	Bone_Animation*		m_pAnimation;				// 애니메이션

	Bone_Node() { Clear(); }
	void	Clear() { m_pAnimation = NULL; }
};


class Bone
{
public:
	unsigned int 	m_num_node;		// 노드갯수	
	unsigned int 	m_ani_frame;	// 애니 프레임		
	Bone_Node*		m_pNode;		// 노드		
	unsigned int	m_Index;		// 파일인덱스
	WORD			m_race;			// 종족 (밀레나 n_ChrRace_Millena , 레인 n_ChrRace_Rain, 공통 n_ChrRace_Common)
	long			m_lPrevTime;	// 마지막 참조시간

public:
	Bone()	{ Clear(); }
	~Bone()	{ Destory(); }

	void	Clear();
	void	Destory();						// 파괴자	

	bool	Read( char* file_path );		// 애니메이션 파일 읽기
	bool	ReadVF( CVFileSystem* pVF, char* file_path );	/// sooree pack
};


#endif