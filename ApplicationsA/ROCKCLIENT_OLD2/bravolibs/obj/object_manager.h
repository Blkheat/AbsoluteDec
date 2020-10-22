
#ifndef _OBJECT_MANAGER_H_
#define _OBJECT_MANAGER_H_


#include	<windows.h>
#ifdef DIRECT_VERSION_9_MJH
#include	<D3DX9.H>
#else
#include	<D3DX8.H>
#endif // DIRECT_VERSION_9_MJH
#include	"Model.H"
#include	"Texture.H"
#include	"Animation.H"

#include	"VFileSystem.h"	/// sooree pack 

#define		MOUSEPOINT_MODCOUNT		2
#define		SELECTOBJ_MODCOUNT		3
#define		LODRANGE_COUNT			3

#define		MAX_OBJCOUNT			200
#define		MAX_OBJ_MODCOUNT		200
#define		MAX_OBJ_TEXCOUNT		200
#define		MAX_OBJ_ANICOUNT		64

//.......................................................................................................................................................................................................
// 맵 카운트 번호 By wxywxy dong 맵로딩 ... 
//.......................................................................................................................................................................................................
#define		MAX_MAPIDCOUNT			80
//.......................................................................................................................................................................................................


//#define		SET_COUNT				2
#define		SET_COUNT				MAX_MAPIDCOUNT

/*
#define		OBJ_MOD_COUNT			512	
#define		OBJ_TEX_COUNT			256
#define		OBJ_ANI_COUNT			16
*/
 
//-----------------------------------------------------------------------------
// Desc: Set_Manager( Model, Texture, Animation, Obj 파일에 대한 셋트 )
//-----------------------------------------------------------------------------
struct	OBJ_SET_INFO
{
	OBJ_SET_INFO()
	{
		for( int i = 0; i < MAX_OBJCOUNT; ++i )
		{
			nIndex[i] = 0;
			nlod[i] = -3;
		}
	}

	int		nIndex[MAX_OBJCOUNT];
	int		nlod[MAX_OBJCOUNT];
	int 	ISBlend[MAX_OBJCOUNT];												// 블렌딩 여부
};

struct	OBJ_SET_MANAGER
{	
	OBJ_SET_MANAGER()
	{
		for( int i = 0; i < SET_COUNT; ++i )
		{
			for( int j = 0; j < MAX_MAPIDCOUNT; ++j )
			{
				byMapId[i][j] = 255;
			}
		}
	}

	BYTE			byMapId[SET_COUNT][MAX_MAPIDCOUNT];		// 셋트에 사용하는 맵아이디	
	OBJ_SET_INFO	SetInfo[SET_COUNT];						// 오브젝트 셋트
};


//-----------------------------------------------------------------------------
// Desc: Object_Manager( Object_Model, Object_Texture, Object_Animation )
//-----------------------------------------------------------------------------
class Object_Manager
{	
public:
	// 모델, 텍스쳐, 애니메이션 포인터
	Model		m_Mod[MAX_OBJ_MODCOUNT];
	Texture		m_Tex[MAX_OBJ_TEXCOUNT];
	Bone		m_Ani[MAX_OBJ_ANICOUNT];

	// 셋트
	int					m_nCurtSet;
	int					m_nSetCount;
	OBJ_SET_MANAGER		m_SetManager;
	

	// 마우스포인터에서 사용
	Model		m_PointMod[MOUSEPOINT_MODCOUNT];
	Texture		m_PointTex;

	// 캐릭터 선택화면에서 사용	
	float		m_TempRot;
	float		m_TempTime;
	D3DXMATRIX	m_matTempRot;
	Model		m_TempMod[SELECTOBJ_MODCOUNT];
	Texture		m_TempTex;

	// LOD RANGE (옵션에서 바꿀수있다)
	int			m_LodRange[LODRANGE_COUNT];

public:
	 
	 Object_Manager();
	~Object_Manager();

	//-----------------------------------------------------------------------------
	// 초기화, 파괴자
	//-----------------------------------------------------------------------------
	void	Clear();
	void	Destroy();	
	void	ClearObj();

	void	InvalidateDeviceObjects();
	void	RestoreDeviceObjects();

	//-----------------------------------------------------------------------------
	// 애니메이션과 관련없는 버텍스버퍼 삭제
	//-----------------------------------------------------------------------------
	void	DeleteVertex();

	//-----------------------------------------------------------------------------
	// 오브젝트 셋트처리
	//-----------------------------------------------------------------------------
	bool	Set__InfoVF( CVFileSystem* pVF, char* file_path, int index );	/// sooree pack
	bool	Set__Info( char* file_path, int number );
	int		GetNowSet( int map_id = -1 );
	void	SetLodRange( int range );
	void	UpdateLodRange();

	//-----------------------------------------------------------------------------
	// 오브젝트에 관련된 파일 메모리에 할당(모델,텍스쳐,애니메이션)
	//-----------------------------------------------------------------------------
	void	SetCreate( char* file_path );
	bool	Create( char* file_path, bool bTempObj = false );	

	int		GetFileCountVF( CVFileSystem* pVF, char* file_ext );	/// sooree pack
	int		GetFileCount( char* file_path, char* file_ext );

	char*	GetFileName( int count, bool& bFind );	
	int		GetLodAttr( int _nIndex );
	int		GetBlendAttr( int _nIndex );
	int		GetISBlendAttr( int _nIndex );

	int			GetVertexCount( int index );//        { return m_Mod[ index ].m_header.m_num_vertex; }
	D3DXVECTOR3	GetVertex( int index, int number );// { return m_Mod[ index ].m_pVertex[ number ].m_vertex; }	

	void		InitLockState(void);
};


extern Object_Manager	g_Obj_Manager;


#endif