#ifndef		_____CLIENT_FILE_INFO_____
#define		_____CLIENT_FILE_INFO_____

#define		STOREDFILE_NAME		"new.orm"

#include	"grassnormal.h"

#ifdef ADD_APPLYWIDE_MAP //1024맵 확장
	#define APPLYWIDE_MAP
#endif // ADD_APPLYWIDE_MAP

struct		FILEWARP
{
	int		nDestWarpIdx;
	int		nDestMapID;
};

struct		attr
{
	WORD			wAttr;
	WORD			wType;
	float			fHeight;
};

typedef	struct	_OBJ_INFO
{
	WORD			wIndex;
	D3DXMATRIX		matPos;
}OBJ_INFO;

typedef	struct	_NPC_INFO
{
	WORD			wIndex;
	D3DXMATRIX		matPos;
}NPC_INFO;

typedef	struct	_GRS_INFO
{
	WORD			wIndex;
	D3DXMATRIX		matPos;
}GRS_INFO;

typedef	struct	_PARTICLE_INFO
{
	WORD			wIndex;
	D3DXVECTOR3		vPos;
	float			fRot;
}PARTICLE_INFO;

typedef	struct	_WATERFALL_INFO
{
	WORD			wIndex;
	D3DXMATRIX		matPos;
}WATERFALL_INFO;

typedef	struct	_MAP_ATTR
{
	BYTE			byAttr;
	WORD			wHeight;
}MAP_ATTR;

typedef	struct	_MAP_ATTR2
{
	BYTE			byAttr;
	BYTE			byType;
	WORD			wHeight;
}MAP_ATTR2;

typedef	struct	_NPC_INFO2
{
	WORD			wIndex;
	D3DXVECTOR3		vPos;
}NPC_INFO2;

typedef	struct	_GRS_INFO2
{
	WORD			wIndex;
	D3DXVECTOR3		vPos;
}GRS_INFO2;


typedef	struct	_WATER_INFO
{
	D3DXVECTOR3		vPos[4];
	WORD			wListIdx;
	WORD			wPos;
	WORD			wDir;
}WATER_INFO;

typedef	struct	_COLOR_RGB
{
	BYTE			r, g, b;
}COLOR_RGB;


class	GRASS_POINT
{
public:
	short	x, y;
	
	BYTE	idx;
	BYTE	byRange;
	BYTE	byDensity;
	bool	bIsReady;
	bool	bIsVisible;

	CGrassList	*	pHeader;
	BOOL			Add(CGrassList* Node);
	BOOL			FinalCleanup();
	BOOL			Display();		

	GRASS_POINT *	pNext;
	GRASS_POINT *	pPrev;

	GRASS_POINT();
	~GRASS_POINT();
};


typedef	struct	_FILE_INFO
{
	BYTE			byType;
	WORD			cSize;
	float	*		fHeights;
	BYTE *			byColor;
	WORD			dwTexIdx;
	WORD	*		dwTileTexIdx;
	WORD	*		dwMultiTileTexIdx;	// add multitile at 2002.10.11
#ifdef DIRECT_VERSION_9_MJH
	D3DMATERIAL9	Material;
	D3DLIGHT9		Light;
#else
	D3DMATERIAL8	Material;
	D3DLIGHT8		Light;
#endif // DIRECT_VERSION_9_MJH
	WORD			wObjCount;
	OBJ_INFO *		pObjList;
	WORD			wHighestFloor;
	attr *			pAttr;
	attr *			pAttr2F;
	FILEWARP		WarpList[9];
	WORD			wNpcCount;
	NPC_INFO *		pNpcList;
	WORD			wGrsCount;
	GRS_INFO *		pGrsList;
	WORD			wParticleCount;
	PARTICLE_INFO *	pParticleList;
	WORD			wWaterfallCount;
	WATERFALL_INFO *	pWaterfallList;
	WORD			wWaterCount;
	WATER_INFO *	pWaterList;
	short			m_nWaterHeight;
	BYTE			m_byWaterVariance;
	bool			m_bShowWater;
	bool			m_bShowCloud;
	BYTE			m_byCloudTexture;
	bool			m_bShowSky;
	BYTE			m_bySkyTexture;
}FILE_INFO;

struct	BRVF
{
	BYTE			byType;
	WORD			cSize;
	BYTE			byBaseTextureIndex;/// 새로 추가
	BYTE			byTextureIndex;
	BYTE			bySkyTextureIndex;
	BYTE			byCloudTextureIndex;
	BYTE			byWaterCount;	/// sooree water
	BOOL			bShowSky;
	BOOL			bShowCloud;
	BYTE			bShowWater;	
	BYTE			byWaterHeightVariance;
	float			fWaterBaseHeight;
	BYTE			byWaterRunsDir;
#ifdef DIRECT_VERSION_9_MJH
	D3DMATERIAL9	d3dMaterial;
	D3DLIGHT9		d3dLight;
#else
	D3DMATERIAL8	d3dMaterial;
	D3DLIGHT8		d3dLight;
#endif // DIRECT_VERSION_9_MJH
	WORD			nObjCount;// 맵의 오브젝트 개수
	WORD			nNpcCount;
	WORD			nGrsCount;
	WORD			nEffectCount;
	WORD			nWaterfallCount;
	MAP_ATTR	*	pAttrList;
	MAP_ATTR2	*	pAttrList2;
	short		*	pHeightList;
	COLOR_RGB	*	pColorList;
	WORD		*	pTileIndexList;
	OBJ_INFO	*	pObjList;
	NPC_INFO2	*	pNpcList;
	PARTICLE_INFO *	pEffectList;
	GRASS_POINT	*	pGrsList;
	WATERFALL_INFO *pWaterfallList;
	FILEWARP		WarpList[9];
};

/****** SERVER FILE INFO ******/

struct Warp_Info
{
	Warp_Info() { m_My_ID = -1; m_Map_ID = -1; m_Map_Warp = -1; }
	int		m_My_ID;		// 현재 워프 존이 몇번인가..
	int		m_Map_ID;		// 워프존이 연결될 맵 번호
	int		m_Map_Warp;		// 연결될 맵의 워프 번호
};


struct Block
{
	Block()
	{
		for( int i = 0; i < 3; i++ ) 
		{
			m_num_item	= 0;
			m_item_stack[i]	= 0;		// item index에서 0은 없는것
		}
		m_is_blocked	= false;	
		m_someone_is_in	= false;
		m_avail_PK		= true;
		m_is_linked		= false;
	}
	__int8			m_num_item;			// 해당 블럭의 아이템수. 최대 3개
	
	unsigned int	m_item_stack[3];

	bool			m_is_blocked;		// 이동 가능 유무
	bool			m_someone_is_in;	// 어떤 캐릭터의 존재 유무
	bool			m_avail_PK;			// PK가능 여부
	bool			m_is_linked;

	Warp_Info		m_Warp;				// 워프시 참조할 인덱스
	DWORD			m_pos;				// 해당 타일의 번호
	int				m_xblock;			// 타일이 속하는 xblock 번호
	int				m_cell;				// 타일이 속하는 xblock의 cell번호
};

struct		SFILEHEADER
{
	int		m_map_id;
	int		m_map_size;
	int		m_map_floor;
};

struct		SFILENPC
{
	int		m_nIndex;
	DWORD	m_nPos;
};

struct		SERVERFILE
{
	SFILEHEADER	Header;
	Block *		Block;
	int			NpcCount;
	SFILENPC *	pNpcList;
};

class	CFileInfo
{
public:
	FILE_INFO	File;
	SERVERFILE	ServerFile;

	bool		Store(char* filename);
	bool		Load(char* filename);
};


#endif