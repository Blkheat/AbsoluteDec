       // http://www.coffeero.com/20020102/2-52.htm

#include "RockPCH.h"

#include	<NETWORK\\Protocol.h>
#include	<BASE\\D3DBASE.H>
#include	<time.h>
#include	"quadlist.h"
#include	<OBJ\\ObjBase.H>
#include	<MAP\FIELD.H>
#include	"quadlist.h"
#include	"Map.h"
#include	"RockClient.h"
#include	"Camera.h"
#include	"Mobject.h"
#include	"Npc.h"
#include	"Pc.H"
#include	"Effect.H"
#include	<Map\\Grass.h>
#include	<SOUND\Sound.H>
//#include	"Weather_Manager.H"
#include	"RockClient.h"
#include	"Map\\GTexture.h"
//#include	"Menu_Interface.h"
 
//CRenderBound	g_testBound;

#include	<stdio.h>

#include "profile\\profile.h"

#include "global.h"

#include "RockInterface\\Rui.h"
#include "RockInterface\\LoadingWndProc.h"
#include "CRenderManager.h"
#include "MapDataManager.h"

#include "bravolibs\\effect\\CTextCommand.h"
#include "RockInterface\\PartyInforWnd.h"
#include "RockInterface\\NonPlayerInfoWnd.h"
/// 셀 속성 디버그 
///#define		BLOCK_DEBUG


FILE	*	g_warplog;
DWORD		g_pretime;
DWORD		g_curtime;

#ifdef DIRECT_VERSION_9_MJH
LPDIRECT3DTEXTURE9	g_pCurTextureSky;
#else
LPDIRECT3DTEXTURE8	g_pCurTextureSky;
#endif // DIRECT_VERSION_9_MJH

CCubeSky	g_SkyRender;


extern		DWORD				g_dwFogColor;
extern		int					g_Shadow;
extern		bool				g_bBlockDebug;
extern		float				FtoDW(float f);
extern		long				g_nowTime;
extern		CGrassData			g_GrassList[1024];
//extern		CNpcTalk			g_NpcTalk;
extern		BOOL				g_ISObjTexLoad;
extern		BOOL				g_ISLandEffect;

CBravoWaterTexture	g_WaterTexture;
#ifdef DIRECT_VERSION_9_MJH
extern	LPDIRECT3DDEVICE9	g_lpDevice;
#else
extern	LPDIRECT3DDEVICE8	g_lpDevice;
#endif // DIRECT_VERSION_9_MJH
static	float	g_ffmr;//#define		D3DFVF_PNTD		(D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX2)
static	float	g_fFogEnd;

CTileTexture	g_TileTexture;
extern	int		g_GrsVisible;

extern	int		g_LODRange;	
extern	int		g_ViewLODTerrain;	/// sooree ter opt

//extern int test_mip;
//extern int test_mag;
//extern int test_min;


int	NodeSortProcess( const void *p1, const void *p2)
{	
	NODE_SORT_DATA *pNode1 = ( NODE_SORT_DATA * )p1;
	NODE_SORT_DATA *pNode2 = ( NODE_SORT_DATA * )p2;
	
	//-----------------------------------------------------------------------------
	// 테이블에 값이 없는 경우
	//-----------------------------------------------------------------------------
	if( pNode1->ID == ID_NONE )
	{
		return +1;
	}
	else if( pNode2->ID == ID_NONE )
	{
		return -1;
	}

	//-----------------------------------------------------------------------------
	// p1 위치값
	//-----------------------------------------------------------------------------
	float _fX1, _fZ1, _fX2, _fZ2;

	switch( pNode1->ID )
	{
		case ID_PC:	// PC
		{
			if( g_Pc.GetUser((short)pNode1->Index)->m_bIsVisible == false )
				return +1;			

			_fX1	= g_Pc.GetUser((short)pNode1->Index)->GetPosTM()._41;
			_fZ1	= g_Pc.GetUser((short)pNode1->Index)->GetPosTM()._43;			
		}
		break;
		
		case ID_NPC:	// NPC
		{
			if( g_Map.m_Npc[pNode1->Index].m_bIsVisible == false )
				return +1;			

			_fX1	= g_Map.m_Npc[pNode1->Index].GetPosTM()._41;
			_fZ1	= g_Map.m_Npc[pNode1->Index].GetPosTM()._43;			

		}
		break;

		case ID_OBJ:
		{
			_fX1   = g_Map.m_Object[pNode1->Index].m_pAniPos[pNode1->ChildIndex].GetPosTM()._41;
			_fZ1   = g_Map.m_Object[pNode1->Index].m_pAniPos[pNode1->ChildIndex].GetPosTM()._43;
		}
		break;

		case ID_PLAYER:
		{
			_fX1   = g_Pc.GetPlayer()->GetPosTM()._41;
			_fZ1   = g_Pc.GetPlayer()->GetPosTM()._43;
		}
		break;

		default:		
			break;
	}

	//-----------------------------------------------------------------------------
	// p2 위치값
	//-----------------------------------------------------------------------------
	switch( pNode2->ID )
	{
		case ID_PC:	// PC
		{
			if( g_Pc.GetUser((short)pNode2->Index)->m_bIsVisible == false )
				return +1;			

			_fX2	= g_Pc.GetUser((short)pNode2->Index)->GetPosTM()._41;
			_fZ2	= g_Pc.GetUser((short)pNode2->Index)->GetPosTM()._43;			
		}
		break;
		
		case ID_NPC:	// NPC
		{
			if( g_Map.m_Npc[pNode2->Index].m_bIsVisible == false )
				return +1;			

			_fX2	= g_Map.m_Npc[pNode2->Index].GetPosTM()._41;
			_fZ2	= g_Map.m_Npc[pNode2->Index].GetPosTM()._43;			

		}
		break;

		case ID_OBJ:
		{
			_fX2   = g_Map.m_Object[pNode2->Index].m_pAniPos[pNode2->ChildIndex].GetPosTM()._41;
			_fZ2   = g_Map.m_Object[pNode2->Index].m_pAniPos[pNode2->ChildIndex].GetPosTM()._43;
		}
		break;

		case ID_PLAYER:
		{
			_fX2   = g_Pc.GetPlayer()->GetPosTM()._41;
			_fZ2   = g_Pc.GetPlayer()->GetPosTM()._43;
		}
		break;

		default:		
			break;
	}
	//-----------------------------------------------------------------------------
	// 위치값을 비교 	
	//-----------------------------------------------------------------------------
	float _fD1	= (_fX1*g_Camera.GetAlphaSortRot().x) + (_fZ1*g_Camera.GetAlphaSortRot().z);
	float _fD2	= (_fX2*g_Camera.GetAlphaSortRot().x) + (_fZ2*g_Camera.GetAlphaSortRot().z);
	
	if( _fD1 > _fD2 )
		return -1;
	
	return +1;
}

#ifdef ADD_APPLYWIDE_MAP 
struct NodeSort: public binary_function<NODE_SORT_DATA, NODE_SORT_DATA, bool> //MJHMAP
{ 
       bool operator()( NODE_SORT_DATA &pNode1, NODE_SORT_DATA &pNode2) const 
       { 
		   float _fX1, _fZ1, _fX2, _fZ2;

		switch( pNode1.ID )
		{
			case ID_PC:	// PC
			{
				//if( g_Pc.GetUser((short)pNode1.Index)->m_bIsVisible == false )
				//	return +1;			

				_fX1	= g_Pc.GetUser((short)pNode1.Index)->GetPosTM()._41;
				_fZ1	= g_Pc.GetUser((short)pNode1.Index)->GetPosTM()._43;			
			}
			break;
			
			case ID_NPC:	// NPC
			{
				//if( g_Map.m_Npc[pNode1.Index].m_bIsVisible == false )
				//	return +1;			

				_fX1	= g_Map.m_Npc[pNode1.Index].GetPosTM()._41;
				_fZ1	= g_Map.m_Npc[pNode1.Index].GetPosTM()._43;			

			}
			break;

			case ID_OBJ:
			{
				_fX1   = g_Map.m_Object[pNode1.Index].m_pAniPos[pNode1.ChildIndex].GetPosTM()._41;
				_fZ1   = g_Map.m_Object[pNode1.Index].m_pAniPos[pNode1.ChildIndex].GetPosTM()._43;
			}
			break;

			case ID_PLAYER:
			{
				_fX1   = g_Pc.GetPlayer()->GetPosTM()._41;
				_fZ1   = g_Pc.GetPlayer()->GetPosTM()._43;
			}
			break;

			default:		
				break;
		}

		//-----------------------------------------------------------------------------
		// p2 위치값
		//-----------------------------------------------------------------------------
		switch( pNode2.ID )
		{
			case ID_PC:	// PC
			{
				//if( g_Pc.GetUser((short)pNode2.Index)->m_bIsVisible == false )
				//	return +1;			

				_fX2	= g_Pc.GetUser((short)pNode2.Index)->GetPosTM()._41;
				_fZ2	= g_Pc.GetUser((short)pNode2.Index)->GetPosTM()._43;			
			}
			break;
			
			case ID_NPC:	// NPC
			{
				//if( g_Map.m_Npc[pNode2.Index].m_bIsVisible == false )
				//	return +1;			

				_fX2	= g_Map.m_Npc[pNode2.Index].GetPosTM()._41;
				_fZ2	= g_Map.m_Npc[pNode2.Index].GetPosTM()._43;			

			}
			break;

			case ID_OBJ:
			{
				_fX2   = g_Map.m_Object[pNode2.Index].m_pAniPos[pNode2.ChildIndex].GetPosTM()._41;
				_fZ2   = g_Map.m_Object[pNode2.Index].m_pAniPos[pNode2.ChildIndex].GetPosTM()._43;
			}
			break;

		case ID_PLAYER:
		{
			_fX2   = g_Pc.GetPlayer()->GetPosTM()._41;
			_fZ2   = g_Pc.GetPlayer()->GetPosTM()._43;
		}
		break;

		default:		
			break;
	}
	//-----------------------------------------------------------------------------
	// 위치값을 비교 	
	//-----------------------------------------------------------------------------
	pNode1.distance	= (_fX1*g_Camera.GetAlphaSortRot().x) + (_fZ1*g_Camera.GetAlphaSortRot().z);
	pNode2.distance	= (_fX2*g_Camera.GetAlphaSortRot().x) + (_fZ2*g_Camera.GetAlphaSortRot().z);
		   return (pNode1.distance > pNode2.distance );
       } 
}; 

#endif //ADD_APPLYWIDE_MAP

CTileTexture::CTileTexture()
{
	m_dpBaseTextures	= NULL;
	m_dpBaseTextureLight = NULL;	/// sooree terrain

	m_dpTileTex = NULL;	/// sooree ter opt
}

CTileTexture::~CTileTexture()
{
	m_pd3dDevice	= NULL;
	FinalCleanup();
}

void CTileTexture::CountTexture(const char* rootDir)
{
#if !defined( LOAD_VF )
	/// sooree ter opt
	{
		m_nTileTexCount = 0;

		char fname[MAX_PATH];
		sprintf(fname, "%s\\tiletex%02d.dds", rootDir, m_nTileTexCount);
		FILE *	fp	= fopen(fname, "rb");
		while ( fp )
		{
			++m_nTileTexCount;
			fclose(fp);

			sprintf(fname, "%s\\tiletex%02d.dds", rootDir, m_nTileTexCount);
			fp	= fopen(fname, "rb");
		}
	}
#else
	/// sooree pack
	{
		m_nTileTexCount = 0;
		char buf[256];

		VFileHandleList* pFHL;
		VFileHandleList_i it;

///		g_VFMap.ChangeDir( "/" );	/// 이거 호출될 때는 이미 디렉토리가 같다. 
		pFHL = g_VFMap.GetFileHandleList();
		for( it = pFHL->begin(); it != pFHL->end(); ++it )
		{
			if( (*it)->GetFileContext()->iType == FT_FILE )
			{
				strcpy( buf, (*it)->GetFileContext()->strName);
				buf[7] = '\0';
				if( lstrcmp( buf, "tiletex" ) == 0 )	/// 앞에서 7글자가 tiletex
				{
					GetFileExt( (*it)->GetFileContext()->strName, buf );
					if( lstrcmp( buf, "tga" ) == 0 || lstrcmp( buf, "dds" ) == 0 )/// 확장자가 tga 파일인 넘만 읽는다
					{
						++m_nTileTexCount;
					}
				}
			}
		}
	}
#endif
}

#ifdef DIRECT_VERSION_9_MJH
HRESULT	CTileTexture::InitDeviceObjects(LPDIRECT3DDEVICE9 pd3ddev)
#else
HRESULT	CTileTexture::InitDeviceObjects(LPDIRECT3DDEVICE8 pd3ddev)
#endif // DIRECT_VERSION_9_MJH
{
	m_pd3dDevice	= pd3ddev;
	return	(m_pd3dDevice) ? S_OK : Error("Invalid Device");
}

HRESULT	CTileTexture::LoadAllUnits( const char* rootDir )
{
	CountTexture(rootDir);

	char fname[256] = "";
	char lca_error[300] = "";

#if !defined( LOAD_VF )
	/// sooree ter opt 
	{
#ifdef DIRECT_VERSION_9_MJH
		m_dpTileTex = SAFE_NEW_ARRAY( LPDIRECT3DTEXTURE9 , m_nTileTexCount );	
#else
		m_dpTileTex = SAFE_NEW_ARRAY( LPDIRECT3DTEXTURE8 , m_nTileTexCount );	
#endif // DIRECT_VERSION_9_MJH

		for ( int i = 0; i < m_nTileTexCount; ++i ) 
		{
			sprintf(fname, "%s\\tiletex%02d.dds", rootDir, i);
			if ( FAILED ( g_RenderManager.CreateTextureFromFileEx( fname, D3DX_DEFAULT, D3DX_DEFAULT, 2, 0, 
																D3DFMT_UNKNOWN, D3DPOOL_MANAGED, 
																D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR, 
																D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR, 
																0, NULL, NULL, &m_dpTileTex[i] ) ) )
			{
				return	Error(fname);
			}
		}
	}

	sprintf( fname, "%s\\base_00.dds", rootDir );
	if ( FAILED ( g_RenderManager.CreateTextureFromFileEx( fname, D3DX_DEFAULT, D3DX_DEFAULT, 2, 0, 
														D3DFMT_UNKNOWN, D3DPOOL_MANAGED, 
														D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR, 
														D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR, 
		//												0xff000000, NULL, NULL, &m_dpBaseTextures ) ) )
														0, NULL, NULL, &m_dpBaseTextures ) ) )
	{
		return Error( fname );
	}

	/// sooree terrain
	sprintf( fname, "%s\\base_01.dds", rootDir );
	if ( FAILED ( g_RenderManager.CreateTextureFromFileEx(  fname, D3DX_DEFAULT, D3DX_DEFAULT, 2, 0, 
														D3DFMT_UNKNOWN, D3DPOOL_MANAGED, 
														D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR, 
														D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR, 
		//												0xff000000, NULL, NULL, &m_dpBaseTextureLight ) ) )
														0, NULL, NULL, &m_dpBaseTextureLight ) ) )
	{
		return Error( fname );
	}
#else
	//////////////////////////////////////////
	/// sooree pack
	VFileHandle* pFH;
#ifdef DIRECT_VERSION_9_MJH
	m_dpTileTex = SAFE_NEW_ARRAY( LPDIRECT3DTEXTURE9 , m_nTileTexCount );
#else
	m_dpTileTex = SAFE_NEW_ARRAY( LPDIRECT3DTEXTURE8 , m_nTileTexCount );
#endif // DIRECT_VERSION_9_MJH	

	for( int i = 0; i < m_nTileTexCount; ++i ) 
	{
		sprintf( fname, "tiletex%02d.dds", i );	/// 주의!! 나중에 dds로 바꾸면 여기도 바꾸어야 함!!
		pFH = g_VFMap.OpenFile( fname );
		if( pFH == NULL )
		{
			sprintf( lca_error, "VF_OPEN ERROR : %s", fname ) ;
			return Error(lca_error);
		}
	
		HRESULT result = g_RenderManager.CreateTextureFromFileInMemoryEx( pFH->GetData(), pFH->GetSize(),		
																	D3DX_DEFAULT, D3DX_DEFAULT, 6, 0, 
																	D3DFMT_UNKNOWN, D3DPOOL_MANAGED, 
																	D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR, 
																	D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR, 
																	// 0, NULL, NULL, &m_dpTileTex[i] ) ) )
																	0, NULL, NULL, &m_dpTileTex[i] ) ; 

		if( result < 0 )
		{
			g_VFMap.CloseFile( pFH );			
			sprintf( lca_error, "DX_CREATE ERROR : %s", fname ) ;			
			return	Error( lca_error );
		}
		g_VFMap.CloseFile( pFH );
	}

	strcpy( fname, "base_00.dds" );
	pFH = g_VFMap.OpenFile( fname );
	if( pFH == NULL ) return Error(fname);

	if( FAILED( g_RenderManager.CreateTextureFromFileInMemoryEx( pFH->GetData(), pFH->GetSize(),
																D3DX_DEFAULT, D3DX_DEFAULT, 2, 0, 
																D3DFMT_UNKNOWN, D3DPOOL_MANAGED, 
																D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR, 
																D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR, 
				//												0xff000000, NULL, NULL, &m_dpBaseTextures ) ) )
																0, NULL, NULL, &m_dpBaseTextures ) ) )
	{
		g_VFMap.CloseFile( pFH );
		return	Error(fname);
	}
	g_VFMap.CloseFile( pFH );

	/// sooree terrain
	if(g_Map.m_uMapType != MAP_TYPE_COLONY)
	{
		sprintf(fname, "base_01.dds");
	}
	else
	{
		sprintf(fname, "base_01_%d.dds", g_Map.m_nExColInfo.m_nMapIndex);
	}	
	
	pFH = g_VFMap.OpenFile( fname );
	if( pFH == NULL ) return Error(fname);

	if( FAILED( g_RenderManager.CreateTextureFromFileInMemoryEx( pFH->GetData(), pFH->GetSize(),
																D3DX_DEFAULT, D3DX_DEFAULT, 2, 0, 
																D3DFMT_UNKNOWN, D3DPOOL_MANAGED, 
																D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR, 
																D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR, 
				//												0xff000000, NULL, NULL, &m_dpBaseTextureLight ) ) )
																0, NULL, NULL, &m_dpBaseTextureLight ) ) )
	{
		g_VFMap.CloseFile( pFH );
		return	Error(fname);
	}
	g_VFMap.CloseFile( pFH );

#endif

	return	S_OK;
}

HRESULT CTileTexture::ReSetShadowMap( const char* rootDir )
{
	char fname[256] = "";
	VFileHandle* pFH = NULL;

	if(g_Map.m_uMapType != MAP_TYPE_COLONY)
	{
		return	S_OK;
	}

	sprintf(fname, "base_01_%d.dds", g_Map.m_nExColInfo.m_nMapIndex);
	
	pFH = g_VFMap.OpenFile( fname );
	if( pFH == NULL ) return Error(fname);

	SAFE_RELEASE( m_dpBaseTextureLight );	

	if( FAILED( g_RenderManager.CreateTextureFromFileInMemoryEx( pFH->GetData(), pFH->GetSize(),
																D3DX_DEFAULT, D3DX_DEFAULT, 2, 0, 
																D3DFMT_UNKNOWN, D3DPOOL_MANAGED, 
																D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR, 
																D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,														
																0, NULL, NULL, &m_dpBaseTextureLight ) ) )
	{
		g_VFMap.CloseFile( pFH );
		return	Error(fname);
	}
	
	return	S_OK;
}

HRESULT	CTileTexture::FinalCleanup()
{
/*///
	for ( int i = 0; i < m_nCount; i ++ )
	{
		SAFE_RELEASE( m_dpTextures[i] );
		SAFE_RELEASE( m_dpTextures2[i] );
	}
	SAFE_DELETE_ARRAY( m_dpTextures );
	SAFE_DELETE_ARRAY( m_dpTextures2 );
///*/

	/// sooree ter opt
	{
		for( int i = 0; i < m_nTileTexCount; ++i )
		{
			SAFE_RELEASE( m_dpTileTex[i] );	
		}
		SAFE_DELETE_ARRAY( m_dpTileTex );	
	}

	SAFE_RELEASE( m_dpBaseTextures );
	SAFE_RELEASE( m_dpBaseTextureLight );	/// sooree terrain

	m_nCount = 0;
	m_nTileTexCount = 0;

	return	S_OK;
}
#ifdef DIRECT_VERSION_9_MJH
LPDIRECT3DTEXTURE9	CTileTexture::GetTextureSelection(const int& selTex, const unsigned char& bysel)
#else
LPDIRECT3DTEXTURE8	CTileTexture::GetTextureSelection(const int& selTex, const unsigned char& bysel)
#endif // DIRECT_VERSION_9_MJH
{
///	return( (bysel)? m_dpTextures2[selTex] : m_dpTextures[selTex] );
	return NULL;
}

/// sooree ter opt 
#ifdef DIRECT_VERSION_9_MJH
LPDIRECT3DTEXTURE9	CTileTexture::GetTileTexture( int nTileTexIndexIn,
#else
LPDIRECT3DTEXTURE8	CTileTexture::GetTileTexture( int nTileTexIndexIn,
#endif // DIRECT_VERSION_9_MJH 
												  int& nTileGroupOut, 
												  int& nTileTypeIdxOut, 
												  int& nTileBlendSeqOut, 
												  int& nSubTileOut )
{
	nSubTileOut = ( nTileTexIndexIn % 10000 ) / 1000;

	nTileTexIndexIn = nTileTexIndexIn - 1000 * nSubTileOut;

	nTileGroupOut = ( nTileTexIndexIn % 10000 ) / 14;		/// set group
	nTileTypeIdxOut = ( nTileTexIndexIn % 10000 ) % 14;		/// 0 ~ 13  : Tex Type. Including Rotate Information
	nTileBlendSeqOut = nTileTexIndexIn / 10000;				/// 0 ~ 4	: 2 element blend sequence

	if( nTileGroupOut < 0 || nTileGroupOut >= m_nTileTexCount ) return NULL;

	return m_dpTileTex[nTileGroupOut];
}

struct	FVF_EFFECTMAP
{
	float	x, y, z;
	DWORD	color;
	float	u1, v1;
	float	u2, v2;
};

#define	D3DFVF_EFFECTMAP	(D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX2)

//typedef	FVF_BRAVOCLOUD	FVF_EFFECTMAP;

CEffectMap		g_EffectMap;

/****************************************************************************************************
	* GrassPoint
****************************************************************************************************/

GRASS_POINT::GRASS_POINT()
{
	idx			= 0;
	byRange		= 0;
	byDensity	= 0;
	pHeader	= NULL;
	bIsReady	= false;
	bIsVisible	= false;
}

GRASS_POINT::~GRASS_POINT()
{
//	FinalCleanup();
}

BOOL	GRASS_POINT::FinalCleanup()
{
	/*
//	CGrassList	* pLink	= pHeader;
	CGrassList	* pTemp	= NULL;
	CGrassList **	pt;

	pt	= &pHeader;

	while ( *pt )
	{
		pTemp	= (*pt)->m_pNext;
//		delete	pLink;
//		pLink	= NULL;

		delete	*pt;
		*pt	= NULL;

		pt	= &pTemp;
//		pTemp	= pLink->m_pNext;
//		pt	= &pLink;
//		delete	*pt;
//		*pt		= NULL;
//		delete	pLink;
//		pLink	= NULL;
//		pLink	= pTemp;
	}

//	delete	pHeader;
//	pHeader	= NULL;
*/
	CGrassList	*	pLink	= pHeader;
	CGrassList	*	pTemp;

	while ( pLink )
	{
		pTemp	= pLink->m_pNext;
		delete	pLink;
		pLink	= NULL;
		pLink	= pTemp;
	}

	return	TRUE;
}

BOOL	GRASS_POINT::Add(CGrassList * Node)
{
	if ( !pHeader )
	{
		pHeader	 = SAFE_NEW( CGrassList );		
		pHeader->m_pNext	= NULL;
	}

	CGrassList	*	pLink	= pHeader;

	while ( pLink )
	{
		if ( pLink->m_pNext == NULL )
		{
			pLink->m_pNext	= Node;
			Node->m_pNext	= NULL;
		}
		pLink	= pLink->m_pNext;
	}

	return	TRUE;
}

BOOL	GRASS_POINT::Display()
{
	if ( !pHeader )
		return	FALSE;

	CGrassList	*	pLink	= pHeader->m_pNext;

	while ( pLink )
	{
		pLink->Display();
		pLink	= pLink->m_pNext;
	}

	return	TRUE;
}

CGrassPoint::CGrassPoint()
{
	m_nCount	= 0;
	m_nCurrent	= -1;
	m_nCurrentEmpty	= 0;
	m_Data	= NULL;
	FinalCleanup();
}

CGrassPoint::~CGrassPoint()
{
	FinalCleanup();
}

GRASS_POINT*	CGrassPoint::GetTailNode()
{
	GRASS_POINT	*	pReturn	= m_pDataHeader->pNext;

	while ( pReturn )
	{
		if ( pReturn->pNext == NULL )
			return	pReturn;
		pReturn	= pReturn->pNext;
	}

	return	NULL;
}

HRESULT	CGrassPoint::FinalCleanup()
{
	for ( short i = 0; i < m_nCount; i ++ )
	{
		m_Data[i].FinalCleanup();
		m_Data[i].bIsReady	= false;
	}
	if ( m_Data )
		delete	[]	m_Data;
	m_Data	= NULL;
	m_nCount	= 0;
	m_nCurrent	= -1;
	m_nCurrentEmpty	= 0;

	return	S_OK;
}

HRESULT	CGrassPoint::Add(GRASS_POINT* pNode)
{
	GRASS_POINT *	pLink	= m_pDataHeader;
	//GRASS_POINT *	pLink	= NULL;

	while ( pLink )
	{
		if ( pLink->pNext == NULL )
			break;
		pLink	= pLink->pNext;
	}
	pLink->pNext	= pNode;
	pNode->pPrev	= pLink;
	pNode->pNext	= NULL;

	m_nCount	++;

	return	S_OK;
}

HRESULT	CGrassPoint::Add(const short& x, const short& y, const BYTE& range, const BYTE& density, const BYTE& idx)
{
	m_Data[m_nCount].bIsReady	= true;
	m_Data[m_nCount].x			= x;
	m_Data[m_nCount].y			= y;
	m_Data[m_nCount].byRange	= range;
	m_Data[m_nCount].byDensity	= density;
	m_Data[m_nCount].idx		= idx;

//	m_nCurrentEmpty		= i;
	m_nCount	++;

	return	S_OK;
}

HRESULT	CGrassPoint::Del()
{
	if ( m_nCurrent == -1 )
		return	S_OK;

	m_Data[m_nCurrent].bIsReady	= false;
	m_Data[m_nCurrent].x		= 0;
	m_Data[m_nCurrent].y		= 0;
	m_Data[m_nCurrent].byRange	= 0;
	m_Data[m_nCurrent].byDensity	= 0;
	m_Data[m_nCurrent].idx		= 0;

	m_Data[m_nCurrent].FinalCleanup();

	m_nCount	--;

	return	S_OK;
}

#ifdef DIRECT_VERSION_9_MJH
HRESULT	CGrassPoint::DisplayCurrent(LPDIRECT3DDEVICE9 pDevice)
#else
HRESULT	CGrassPoint::DisplayCurrent(LPDIRECT3DDEVICE8 pDevice)
#endif // DIRECT_VERSION_9_MJH
{
	return	S_OK;
}

/****************************************************************************************************
	* GrassPoint
****************************************************************************************************/

/******************************************************************************************
	* static stuff node
******************************************************************************************/

/* end of static node */

void		SetupMaxMin(D3DXVECTOR3* max, D3DXVECTOR3* min)
{
	float	temp;

	// setup max
	if ( max->x < min->x )
	{
		temp	= max->x;
		max->x	= min->x;
		min->x	= temp;
	}
	if ( max->y < min->y )
	{
		temp	= max->y;
		max->y	= min->y;
		min->y	= temp;
	}
	if ( max->z < min->z )
	{
		temp	= max->z;
		max->z	= min->z;
		min->z	= temp;
	}
}

CBravoMap::CBravoMap()
{
	theWaterListNum = 0;
	theWaterList = NULL;

	D3DXMatrixIdentity( &m_matWaterTrans );
	D3DXMatrixIdentity( &m_matIdentity   );
	m_fWaterRange   = 0.0f;
	m_OldWaterTimer = timeGetTime();
	m_pNpcList = NULL;
	m_NpcCount = 0;

	m_uMapType = MAP_TYPE_NORMAL;
	
	ClearPvPFieldInfoList();
}

CBravoMap::~CBravoMap()
{
	/// sooree terrain
	theTerrain.FinalCleanup();

	FinalCleanup();
	_RELEASE(m_pTextureWater);
	_RELEASE(m_pTextureWaterfall);
	_RELEASE(m_pTextureMulti);
	_RELEASE(m_pTextureFlow);
}

void CBravoMap::ClearPvPFieldInfoList()
{
	PvPFieldInfoListPos aPos = g_Map.m_PvPFieldInfoList.begin();

	for( ; aPos != g_Map.m_PvPFieldInfoList.end(); ++aPos )
	{
		SPvPFieldInfo * pPvPFieldInfo = (SPvPFieldInfo *)(*aPos);
		SAFE_DELETE( pPvPFieldInfo );
	}
	
	m_PvPFieldInfoList.clear();
}

DWORD	CBravoMap::CountBrvf()
{
	/*.....sh
	DWORD			count	= 0;
	char			path[128]	= {0,};

	sprintf(path, "%s\\map\\*.orm", g_RockClient.GetRootDir());

	WIN32_FIND_DATA		data;
	HANDLE				hFile	= FindFirstFile(path, &data);

	int		Find	= 1;
	while ( Find != 0 )
	{
		Find	= FindNextFile(hFile, &data);
		count	++;
	}

	FindClose(hFile);
	return	count;
	//*/
	return 0;
}

HRESULT	CBravoMap::InvalidateDeviceObject()
{
	m_Cloud.InvalidateDeviceObject();
	m_SkyBox.InvalidateDeviceObject();
	
	for(int i = 0; i < MAX_NPC_INTABLE; ++i )	
	{
		m_Npc[i].m_Shadow.Clear();
	}

	return S_OK;
}

#ifdef DIRECT_VERSION_9_MJH
HRESULT	CBravoMap::RestoreDeviceObject(LPDIRECT3DDEVICE9 pDevice)
#else
HRESULT	CBravoMap::RestoreDeviceObject(LPDIRECT3DDEVICE8 pDevice)
#endif // DIRECT_VERSION_9_MJH
{
	m_Cloud.RestoreDeviceObject(pDevice);
	m_SkyBox.RestoreDeviceObject(pDevice);
	
	return S_OK;
}

void  CBravoMap::CreateShadowList()
{

}

#ifdef DIRECT_VERSION_9_MJH
bool CBravoMap::Create( LPDIRECT3DDEVICE9 pDevice )
#else
bool CBravoMap::Create( LPDIRECT3DDEVICE8 pDevice )
#endif // DIRECT_VERSION_9_MJH
{
	theWireframe = false;

	m_pTexture	= NULL;
	
	// create water textures
	g_WaterTexture.Create( g_lpDevice );

	if ( pDevice == NULL )
	{
///		MessageBox(NULL, "INVALID DEVICE", "FIELD", MB_OK);
		return false;
	}
	m_pDevice = pDevice;

	m_NowMapID = -1;

	thePreCX = 0;
	thePreCY = 0;

	/// sooree mempool : CBravoMap::Create()에서 한번 생성하고 계속 가지고 간다. 
	const int		size	= 64 * 64;	/// 맵 전체를 그리는데는 안쓰입니다. 부분적인 타일 렌더링 용 /// sooree terrain
	const short		vtxsize	= size * 4;
	const short		idxsize	= size * 6;

	if ( FAILED ( g_RenderManager.CreateIndexBuffer(	idxsize*sizeof(WORD), D3DUSAGE_WRITEONLY, 
												D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIBuffer) ) )
	{
///		MessageBox(NULL, "Error - Create(IB)", NULL, MB_OK);
		return false;
	}
	if ( FAILED ( g_RenderManager.CreateVertexBuffer( vtxsize*sizeof(TERVTX), D3DUSAGE_WRITEONLY, 
												 D3DFVF_TERVTX, D3DPOOL_MANAGED, &m_pVBuffer) ) )
	{
///		MessageBox(NULL, "Error - Create(VB)", NULL, MB_OK);
		return false;
	}
	/// <------------------------------------------------------------------- sooree mempool

//	m_Sky.InitDevice(m_pDevice);

	m_MaterialGrass.Diffuse.r	= 0.7f;
	m_MaterialGrass.Diffuse.g	= 0.7f;
	m_MaterialGrass.Diffuse.b	= 0.7f;
	m_MaterialGrass.Diffuse.a	= 0.7f;
	m_MaterialGrass.Ambient.r	= 0.7f;
	m_MaterialGrass.Ambient.g	= 0.7f;
	m_MaterialGrass.Ambient.b	= 0.7f;
	m_MaterialGrass.Ambient.a	= 0.7f;

	m_MaterialWater.Diffuse.r	= 1.0f;
	m_MaterialWater.Diffuse.g	= 1.0f;
	m_MaterialWater.Diffuse.b	= 1.0f;
	m_MaterialWater.Diffuse.a	= 1.0f;
	m_MaterialWater.Ambient.r	= 1.0f;
	m_MaterialWater.Ambient.g	= 1.0f;
	m_MaterialWater.Ambient.b	= 1.0f;
	m_MaterialWater.Ambient.a	= 1.0f;

	m_pNodeObj	= NULL;
#ifdef DIRECT_VERSION_9_MJH
	ZeroMemory(&m_Light, sizeof(D3DLIGHT9));
#else
	ZeroMemory(&m_Light, sizeof(D3DLIGHT8));
#endif // DIRECT_VERSION_9_MJH
	m_Light.Type		= D3DLIGHT_DIRECTIONAL;

	m_Light.Diffuse.r	= 1.0f;
	m_Light.Diffuse.g	= 1.0f;
	m_Light.Diffuse.b	= 1.0f;
	m_Light.Ambient.r	= 0.8f;
	m_Light.Ambient.g	= 0.8f;
	m_Light.Ambient.b	= 0.8f;
	m_Light.Direction.x	= 100;
	m_Light.Direction.y	= -100.0f;
	m_Light.Direction.z	= 100;

	m_Light.Range		= 1000.0f;


#ifdef BLOCK_DEBUG
	if ( FAILED ( g_RenderManager.CreateVertexBuffer(  40*40*4 * sizeof(vtxpos), D3DUSAGE_WRITEONLY, 
												  D3DFVF_XYZ|D3DFVF_DIFFUSE, D3DPOOL_MANAGED, &m_pVBPos) ) )
	{
		return false;
	}
	if ( FAILED ( g_RenderManager.CreateIndexBuffer( 40*40*6 * sizeof(WORD), D3DUSAGE_WRITEONLY, 
												D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIBPos) ) )
	{
		return false;
	}
#endif

//	g_EffectMap.CreateEnvironment(m_pDevice);

	m_lPrevTime = g_nowTime;
	m_lPrevTime2 = g_nowTime;

	// 아이템 데이타 메모리 할당
	g_TileTexture.InitDeviceObjects( m_pDevice );
	//g_TileTexture.LoadAllUnits( fname  );//.....sh  
	

	m_WaveList.BuildAllUnits( pDevice );

//	g_testBound.InitDeviceObjects(pDevice);
//	g_testBound.RestoreDeviceObjects();
//	g_testBound.Update(D3DXVECTOR3(-5.0f,0.0f,-5.0f),D3DXVECTOR3(5.0f,10.0f,5.0f));

	return true;
}

void	CBravoMap::FinalCleanup()
{
	//m_ShadowList[NSHADOW_SIZE_64].Clear();
	//m_ShadowList[NSHADOW_SIZE_128].Clear();
	//m_ShadowList[NSHADOW_SIZE_256].Clear();	
	
	short	i;
	_RELEASE(m_pVBuffer);
	_RELEASE(m_pIBuffer);

#ifdef BLOCK_DEBUG
	_RELEASE(m_pVBPos);
	_RELEASE(m_pIBPos);
#endif

//	for ( DWORD i = 0; i < m_cSize; i ++ )
//		_DELETE_ARRAY(m_pDisplayMap[i]);
//	_DELETE_ARRAY(m_pDisplayMap);
//	for ( i = 0; i < m_cSize*2; i ++ )
//		_DELETE_ARRAY(m_pAttrMap[i]);
//	_DELETE_ARRAY(m_pAttrMap);

	for ( i = 0; i < m_dwCount; i ++ )
	{
		_DELETE_ARRAY( m_szNameList[i] );
	}

	_DELETE_ARRAY( m_szNameList );

//	_RELEASE(m_pTexture);

	CleanupObj();

	CleanupNpc();

//	CleanupGrass();

	CleanupWaterfall();

	CleanupObjNode();

	m_pDevice		= NULL;
	m_cSize			= 0;
	
	if ( m_pNodeObj )
	{
		CleanupObjNode();
	}	
	
///	if ( m_bShowWater )
///	{
///		m_BravoWater.FinalCleanup();
///	}
	/// sooree water
	theWaterListNum = 0;
	
	SAFE_DELETE_ARRAY( theWaterList );

/*
	if ( m_bShowCloud )
	{
		m_BravoCloud.FinalCleanup();
	}
	if ( m_bShowSky )
	{
//		m_BravoSky.FinalCleanup();
		g_SkyRender.FinalCleanup();
	}
*/
	m_SkyBox.DeleteAllMemory();
	m_Cloud.DeleteAllMemory();

	SAFE_DELETE_ARRAY( m_pNpcList );

	m_NpcCount = 0;
	
	m_GrassPoint.FinalCleanup();



	m_uMapType = MAP_TYPE_NORMAL;	
	
	m_EmbObjList.clear();
	m_NationObjList.clear();

	m_GiantUnitList.clear();
	m_GiantBilbList.clear();

	ClearPvPFieldInfoList();
}

void	CBravoMap::Cleanup()
{
///	_RELEASE(m_pVBuffer);	/// sooree mempool : Create()에서 한번 생성하고 계속 가지고 간다. 
///	_RELEASE(m_pIBuffer);	/// sooree mempool : Create()에서 한번 생성하고 계속 가지고 간다. 
	
	int current = timeGetTime();

	m_pVtxList		= NULL;
	m_pIdxList		= NULL;

	SAFE_DELETE_ARRAY( m_pNpcList );

	m_NpcCount = 0;

	/// sooree water
	theWaterListNum = 0;

	SAFE_DELETE_ARRAY( theWaterList );

	if ( m_bShowCloud )
	{
		m_BravoCloud.FinalCleanup();
	}
/*
	if ( m_bShowSky )
	{
		g_SkyRender.FinalCleanup();
	}
*/
	// 스카이 박스
	m_SkyBox.DeleteAllMemory();
	m_Cloud.DeleteAllMemory();


//	if ( m_ILODLand )	/// sooree mempool
//	{
//		delete	m_ILODLand;
//		m_ILODLand	= NULL;
//	}

	current = timeGetTime();
	if ( m_pNodeObj )
	{
		CleanupObjNode();			/// 이게 0.8초 정도 먹네요. 개선 필요. 
	}

	m_GrassPoint.FinalCleanup();	/// 이게 0.8초 정도 먹네요. 개선 필요.



	m_uMapType = MAP_TYPE_NORMAL;	
	
	m_EmbObjList.clear();
	m_NationObjList.clear();

	m_GiantUnitList.clear();
	m_GiantBilbList.clear();

	ClearPvPFieldInfoList();
}

bool	CBravoMap::Load(DWORD idx)
{
	LoadingBarWnd.SetPercent(50);		

//	Cleanup();
	Cleanup();
	CleanupObj();	
	m_bObjSortInit = false;		// 거리에 따른 Alpha Obj 처리를 위해..
	CleanupWaterfall();	

	SAFE_DELETE_ARRAY( m_pNpcList );

	m_NpcCount = 0;
	
	FILE	*	fp;
	char	name[128];		

	// 현재 맵 번호저장
	m_NowMapID = idx;

	ZeroMemory(name, sizeof(name));

	sprintf( theMapPath, "%s\\map\\map%d", g_RockClient.GetRootDir(), idx );

	g_RockClient.SetCurrentMapPath( theMapPath );

	sprintf( name, "%s\\%d.orm", theMapPath, idx );

	if( NULL == (fp=fopen(name,"rb") ) )
	{
		char temp[256];
		sprintf( temp, "!%s -> %d 맵을 로딩할 수 없습니다", name, idx );
///		MessageBox(NULL, temp, NULL, MB_OK);
		return	false;
	}

	fread(&m_NewFile.byType					,sizeof(BYTE		),1,	fp);		
	fread(&m_NewFile.cSize					,sizeof(WORD		),1,	fp);
	fread(&m_NewFile.byTextureIndex			,sizeof(BYTE		),1,	fp);
	fread(&m_NewFile.bySkyTextureIndex		,sizeof(BYTE		),1,	fp);
	fread(&m_NewFile.byCloudTextureIndex	,sizeof(BYTE		),1,	fp);
	fread(&m_NewFile.byWaterCount			,sizeof(BYTE		),1,	fp);	/// sooree water
	fread(&m_NewFile.bShowSky				,sizeof(BOOL		),1,	fp);
	fread(&m_NewFile.bShowCloud				,sizeof(BOOL		),1,	fp);

	fread(&m_NewFile.bShowWater				,sizeof(BOOL		),1,	fp);	/// 읽기는 읽지만 무시합니다. 	
	fread(&m_NewFile.byWaterHeightVariance	,sizeof(BYTE		),1,	fp);	/// 읽기는 읽지만 무시합니다. 
	fread(&m_NewFile.fWaterBaseHeight		,sizeof(float		),1,	fp);	/// 읽기는 읽지만 무시합니다. 
	fread(&m_NewFile.byWaterRunsDir			,sizeof(BYTE		),1,	fp);	/// 읽기는 읽지만 무시합니다. 
#ifdef DIRECT_VERSION_9_MJH
	fread(&m_NewFile.d3dMaterial			,sizeof(D3DMATERIAL9),1,	fp);
	fread(&m_NewFile.d3dLight				,sizeof(D3DLIGHT9	),1,	fp);
#else
	fread(&m_NewFile.d3dMaterial			,sizeof(D3DMATERIAL8),1,	fp);
	fread(&m_NewFile.d3dLight				,sizeof(D3DLIGHT8	),1,	fp);
#endif // DIRECT_VERSION_9_MJH
	fread(&m_NewFile.nObjCount				,sizeof(WORD		),1,	fp);
	fread(&m_NewFile.nNpcCount				,sizeof(WORD		),1,	fp);
	fread(&m_NewFile.nGrsCount				,sizeof(WORD		),1,	fp);
	fread(&m_NewFile.nEffectCount			,sizeof(WORD		),1,	fp);
	fread(&m_NewFile.nWaterfallCount		,sizeof(WORD		),1,	fp);

	m_NewFile.pAttrList2	 =  SAFE_NEW_ARRAY( MAP_ATTR2 , (m_NewFile.cSize<<1)*(m_NewFile.cSize<<1) );
	m_NewFile.pHeightList	 =  SAFE_NEW_ARRAY( short , (m_NewFile.cSize+1)*(m_NewFile.cSize+1) );	
	m_NewFile.pColorList	 =  SAFE_NEW_ARRAY( COLOR_RGB , (m_NewFile.cSize+1)*(m_NewFile.cSize+1) );
	m_NewFile.pTileIndexList =  SAFE_NEW_ARRAY( WORD , m_NewFile.cSize*m_NewFile.cSize );	
	m_NewFile.pObjList		 =  SAFE_NEW_ARRAY( OBJ_INFO , m_NewFile.nObjCount );	
	m_NewFile.pNpcList		 =  SAFE_NEW_ARRAY( NPC_INFO2 , m_NewFile.nNpcCount );	
	m_NewFile.pEffectList	 =  SAFE_NEW_ARRAY( PARTICLE_INFO , m_NewFile.nEffectCount );	
	m_GrassPoint.m_Data		 =  SAFE_NEW_ARRAY( GRASS_POINT , m_NewFile.nGrsCount );	
	m_NewFile.pWaterfallList =  SAFE_NEW_ARRAY( WATERFALL_INFO , m_NewFile.nWaterfallCount );	

	fread(m_NewFile.pAttrList2			,sizeof(MAP_ATTR2)		,(m_NewFile.cSize<<1)*(m_NewFile.cSize<<1), fp);
	fread(m_NewFile.pHeightList			,sizeof(short)			,(m_NewFile.cSize+1)*(m_NewFile.cSize+1),	fp);
	fread(m_NewFile.pColorList			,sizeof(COLOR_RGB)		,(m_NewFile.cSize+1)*(m_NewFile.cSize+1),	fp);
	fread(m_NewFile.pTileIndexList		,sizeof(WORD)			,m_NewFile.cSize*m_NewFile.cSize,			fp);
	fread(m_NewFile.pObjList			,sizeof(OBJ_INFO)		,m_NewFile.nObjCount,						fp);
	fread(m_NewFile.pNpcList			,sizeof(NPC_INFO2)		,m_NewFile.nNpcCount,						fp);
	fread(m_NewFile.pEffectList			,sizeof(PARTICLE_INFO)	,m_NewFile.nEffectCount,					fp);
	fread(m_GrassPoint.m_Data			,sizeof(GRASS_POINT)	,m_NewFile.nGrsCount,						fp);
	fread(m_NewFile.pWaterfallList		,sizeof(WATERFALL_INFO)	,m_NewFile.nWaterfallCount,					fp);
	fread(m_NewFile.WarpList			,sizeof(FILEWARP)		,9,											fp);

//	fclose(fp);		/// 맨 뒤로 옮김 
	
	// setup field
	m_byMapType				= m_NewFile.byType;
	m_cSize					= m_NewFile.cSize;
	
	m_bySelTexture			= m_NewFile.byTextureIndex;
	m_byCurSkyTexNum		= m_NewFile.bySkyTextureIndex;
	m_byCurCloudTexNum		= m_NewFile.byCloudTextureIndex;

//	m_bShowSky				= m_NewFile.bShowSky;
	m_bShowSky				= TRUE;
	m_bShowCloud			= m_NewFile.bShowCloud;

/*	/// sooree water
	/// *.orm 구조 변경 : RockWater.h 참조 
	m_byCurWaterTexNum		= m_NewFile.byWaterTextureIndex;
	m_bShowWater			= m_NewFile.bShowWater;
	m_byHighestWaterPlus	= m_NewFile.byWaterHeightVariance;
	m_fBaseHeightWater		= m_NewFile.fWaterBaseHeight;
	m_byRunsDir				= m_NewFile.byWaterRunsDir; 
	m_BravoWater.m_byRunsDir= m_NewFile.byWaterRunsDir;	*/
	m_byCurWaterTexNum		= 0;
	m_byHighestWaterPlus = 0;
	m_fBaseHeightWater = 0;
	m_byRunsDir = 0;


	m_Material				= m_NewFile.d3dMaterial;
	m_Material.Diffuse.a	= 1.0f;
	m_Light					= m_NewFile.d3dLight;
	m_Light.Diffuse.a		= 1.0f;
	
	//	m_dwObjCount			= m_NewFile.nObjCount;
	m_dwNpcCount			= m_NewFile.nNpcCount;	
	m_dwGrassCount			= m_NewFile.nGrsCount;
	m_nParticleCount		= m_NewFile.nEffectCount;
	m_nWaterfallCount		= m_NewFile.nWaterfallCount;

	/// sooree ter opt : theTerrain.CreateTerrain() 앞으로 위치 조정.
	g_TileTexture.FinalCleanup();
	g_TileTexture.LoadAllUnits( theMapPath );/// base, detail texture loading

	LoadTerrainNew( m_NewFile.pHeightList, m_NewFile.pTileIndexList, m_NewFile.pColorList );

	/// sooree terrain
	theTerrain.FinalCleanup();

	theTerrain.CreateTerrain(	m_pDevice, &g_Camera, 8000, 
								m_NewFile.cSize, m_NewFile.pHeightList, m_NewFile.pTileIndexList );
	
	theTerrain.SetLODQuality( g_LODRange );

	/// sooree ter opt
	if( g_ViewLODTerrain == 0 )
	{
		g_Pc_Manager.theFog_End = ( ( g_LODRange + 1.2f ) * PATCH_SIZE ) * VERTEX_INTERVAL;
	}

	m_dwNpcCount			= m_NewFile.nNpcCount;	
	m_dwGrassCount			= m_NewFile.nGrsCount;
	m_nParticleCount		= m_NewFile.nEffectCount;
	m_nWaterfallCount		= m_NewFile.nWaterfallCount;


	m_Material.Diffuse.r	= 0.70f;
	m_Material.Diffuse.g	= 0.80f;
	m_Material.Diffuse.b	= 0.75f;
	m_Material.Ambient.r	= 0.75f;
	m_Material.Ambient.g	= 0.85f;
	m_Material.Ambient.b	= 0.80f;
	m_Material.Specular.r	= 0.70f;
	m_Material.Specular.g	= 0.80f;
	m_Material.Specular.b	= 0.75f;	
	m_Material.Emissive.r	= 0.70f;
	m_Material.Emissive.g	= 0.80f;
	m_Material.Emissive.b	= 0.75f;
	m_Material.Power		= 1.0f;
	
#ifdef _DEBUG
{
	DWORD tick = timeGetTime();
#endif
	
	LoadingBarWnd.SetPercent(65);	

	g_Object.FinalCleanup();/// 맵별 오브젝트 리스트 작성
	if( g_Object.Create( theMapPath ) == false )
	{
		return false;
	}

	CreateObjIndex(); //.obj 로딩

#ifdef _DEBUG	
	DebugPrint( "!\r\n Obj Load : %.3f초", SAFE_TIME_SUB( timeGetTime() , tick ) / 1000.0f );
	///--PRINT_DLG_LOG( "!\r\n Obj Load : %.3f초", SAFE_TIME_SUB( timeGetTime() , tick ) / 1000.0f );
}
#endif

	LoadingBarWnd.SetPercent(75);	


	///------------------------------------------------------------------------

	short i = 0;
	DWORD x, y;

	for( y = 0; y < m_cSize * 2; ++y )
	{
		for( x = 0; x < m_cSize * 2; ++x )
		{
			m_pAttrMap[y][x].byAttr		= (BYTE)m_NewFile.pAttrList2[(y*(m_cSize<<1))+x].byAttr;
			m_pAttrMap[y][x].byType		= (BYTE)m_NewFile.pAttrList2[(y*(m_cSize<<1))+x].byType;
			
			/// sooree height : map editor에서 속성 편집된 Cell만 속성 높이 가진다 ( MATTR_SELECT, MATTR_SELECTOLD )
			if( m_pAttrMap[y][x].byAttr == 13 || m_pAttrMap[y][x].byAttr == 14 )
			{
				m_pAttrMap[y][x].nHeight	= (short)m_NewFile.pAttrList2[(y*(m_cSize<<1))+x].wHeight;
			}
			else
			{
				m_pAttrMap[y][x].nHeight = 0;
			}
		}
	}
	/// 못가는 셀 주변에 속성 높이가 있다면 못가는 셀도 주변 속성 높이에 맞추어 주자 
	short maxAttrHT;
	for( y = 1; y < m_cSize * 2 - 1; ++y )
	{
		for( x = 1; x < m_cSize * 2 - 1; ++x )
		{
			if( m_pAttrMap[y][x].byAttr == MATTR_OBJ )
			{
				maxAttrHT = 0;
				if( m_pAttrMap[y-1][x].byAttr == 13 || m_pAttrMap[y-1][x].byAttr == 14 )
					maxAttrHT = max( maxAttrHT, m_pAttrMap[y-1][x].nHeight );
				if( m_pAttrMap[y+1][x].byAttr == 13 || m_pAttrMap[y+1][x].byAttr == 14 )
					maxAttrHT = max( maxAttrHT, m_pAttrMap[y+1][x].nHeight );
				if( m_pAttrMap[y][x-1].byAttr == 13 || m_pAttrMap[y][x-1].byAttr == 14 )
					maxAttrHT = max( maxAttrHT, m_pAttrMap[y][x-1].nHeight );
				if( m_pAttrMap[y][x+1].byAttr == 13 || m_pAttrMap[y][x+1].byAttr == 14 )
					maxAttrHT = max( maxAttrHT, m_pAttrMap[y][x+1].nHeight );
				/// 보정용. 2셀까지 
				if( (y-2 > 0) && (m_pAttrMap[y-2][x].byAttr == 13 || m_pAttrMap[y-2][x].byAttr == 14 ))
					maxAttrHT = max( maxAttrHT, m_pAttrMap[y-2][x].nHeight );
				if( (y+2 < m_cSize*2) && (m_pAttrMap[y+2][x].byAttr == 13 || m_pAttrMap[y+2][x].byAttr == 14 ))
					maxAttrHT = max( maxAttrHT, m_pAttrMap[y+2][x].nHeight );
				if( (x-2 > 0) && (m_pAttrMap[y][x-2].byAttr == 13 || m_pAttrMap[y][x-2].byAttr == 14 ))
					maxAttrHT = max( maxAttrHT, m_pAttrMap[y][x-2].nHeight );
				if( (x+2 < m_cSize*2) && (m_pAttrMap[y][x+2].byAttr == 13 || m_pAttrMap[y][x+2].byAttr == 14 ))
					maxAttrHT = max( maxAttrHT, m_pAttrMap[y][x+2].nHeight );

				if( maxAttrHT > m_pDisplayMap[y / 2][x / 2].Pos[0].y )	/// 지형 높이 보다 높아야 겠지? 
				{
					m_pAttrMap[y][x].nHeight = maxAttrHT;
				}
			}
		}
	}

	for ( i = 0; i < 9; i ++ )
	{
		SetupWarpInfo(i, m_NewFile.WarpList[i].nDestMapID, m_NewFile.WarpList[i].nDestWarpIdx);
	}

	InitMapData();	

	for( int block_x = 0; block_x < m_cSize * 2; ++block_x )
	{
		for( int block_y= 0; block_y < m_cSize * 2; ++block_y )
		{			
			m_Block[block_x][block_y].Attr = m_pAttrMap[block_y][block_x].byAttr;
			if( m_Block[block_x][block_y].Attr == 12 || m_Block[block_x][block_y].Attr == 13 || m_Block[block_x][block_y].Attr == 14 )
			{
				m_Block[block_x][block_y].Attr = 0;	
			}
		}
	}

	m_GrassPoint.m_nCount	= m_NewFile.nGrsCount;

	if ( m_GrassPoint.m_nCount )
	{
		for ( i = 0; i < m_NewFile.nGrsCount; i ++ )
		{
			m_GrassPoint.m_Data[i].bIsReady	= true;
			BuildGrassWithPoint(m_GrassPoint.m_Data[i].idx, i);
		}
	}

	m_dwGrassCount	= 0;
	CreateObjNode();
	m_dwGrassCount	= m_NewFile.nGrsCount;

	for ( i = 0; i < m_File.wParticleCount; i ++ )
	{
		CreateParticleTemp(m_File.pParticleList[i].wIndex);
		m_ParticleTemp.m_vMoveLocation	= m_File.pParticleList[i].vPos;
		m_ParticleTemp.m_fRotTheta	= m_File.pParticleList[i].fRot;
		AddParticle();
	}

	for ( i = 0; i < m_nWaterfallCount; i ++ )
	{
//		m_WaterfallList[i].Build(m_pDevice, g_WaterfallTexture.GetTexture(m_NewFile.pWaterfallList[i].wIndex), g_WaterfallTexture.GetFallTexture(m_NewFile.pWaterfallList[i].wIndex), m_NewFile.pWaterfallList[i].wIndex);
//		m_WaterfallList[i].SetPosTM(m_NewFile.pWaterfallList[i].matPos);
	}

	/// sooree water 
	theWaterListNum = 0;
	if( m_NewFile.byWaterCount > 0 )
	{
		theWaterList = SAFE_NEW_ARRAY( CRockWater , m_NewFile.byWaterCount );
		
		if( theWaterList )
		{
			theWaterListNum = m_NewFile.byWaterCount;

			SRockWaterInfo waterInfo;
#ifdef DIRECT_VERSION_9_MJH
			LPDIRECT3DTEXTURE9 tex0 = NULL;
			LPDIRECT3DTEXTURE9 tex1 = NULL;
#else
			LPDIRECT3DTEXTURE8 tex0 = NULL;
			LPDIRECT3DTEXTURE8 tex1 = NULL;
#endif // DIRECT_VERSION_9_MJH

			for( i = 0; i < theWaterListNum; ++i )
			{
				if( fread( &waterInfo, sizeof( SRockWaterInfo ), 1, fp) )
				{
					tex0 = g_WaterTexture.GetTexture( waterInfo.theTextureIndexs & 0xf );
					if( waterInfo.theTextureIndexs >> 4 == 0xf )
					{
						tex1 = NULL;
					}
					else
					{
						tex1 = g_WaterTexture.GetTexture( ( waterInfo.theTextureIndexs >> 4 ) & 0xf );
					}
					
					if( theWaterList[i].Create( m_pDevice, tex0, tex1, &waterInfo ) == false )
					{
						DebugPrint( "!내부 오류로 물 생성 실패\r\n");
						continue;
					}
				} /// if
			} /// for
		} /// if
	} //// if

	fclose(fp);


	m_NpcCount = m_NewFile.nNpcCount;
	m_pNpcList = SAFE_NEW_ARRAY( NPC_INFO2 , m_NpcCount );	
	
	for( i = 0 ; i < m_NpcCount ; ++i )
	{
		memcpy( &m_pNpcList[ i ] , &m_NewFile.pNpcList[ i ] , sizeof( NPC_INFO2 ) );

		m_pNpcList[ i ].vPos.x = ( int ) ( m_pNpcList[ i ].vPos.x / 8 );
		m_pNpcList[ i ].vPos.z = ( int ) ( m_pNpcList[ i ].vPos.z / 8 );
	}
 
	SAFE_DELETE_ARRAY( m_NewFile.pAttrList2	);
	SAFE_DELETE_ARRAY( m_NewFile.pHeightList );	
	SAFE_DELETE_ARRAY( m_NewFile.pColorList );	
	SAFE_DELETE_ARRAY( m_NewFile.pTileIndexList );
	SAFE_DELETE_ARRAY( m_NewFile.pObjList );		
	SAFE_DELETE_ARRAY( m_NewFile.pNpcList );		
	SAFE_DELETE_ARRAY( m_NewFile.pEffectList );	
	SAFE_DELETE_ARRAY( m_GrassPoint.m_Data );		
	SAFE_DELETE_ARRAY( m_NewFile.pWaterfallList );

//	if ( m_NewFile.nObjCount )
//		delete	[]	m_NewFile.pObjList;
//	if ( m_NewFile.nNpcCount )
//		delete	[]	m_NewFile.pNpcList;
//	if ( m_NewFile.nEffectCount )
//		delete	[]	m_NewFile.pEffectList;	
//	if ( m_NewFile.nGrsCount )
//		delete	[]	m_NewFile.pGrsList;
//	if ( m_NewFile.nWaterfallCount )
//		delete	[]	m_NewFile.pWaterfallList;

	//........................................................................................................
	// 환경 관련 처리 ( 하늘 / 구름 / 머터리얼 / 라이트 ) 밤낮 나중에 추가 By wxywxy
	//........................................................................................................
	if( m_SetEnv.Set( idx , ENV_DAYLIGHT ) )
	{
	
		LPENV_DATA SetEnvData = m_SetEnv.GetSet();
	
		m_SkyBox.EnableSkyBox( TRUE );
		m_SkyBox.InitDevice( m_pDevice , 4.0f , 4.0f , 4.0f );
		m_SkyBox.LoadTexture( m_pDevice , SetEnvData->SkyIndex , 0 );

		m_Cloud.EnableCloud( m_bShowCloud );
		m_Cloud.InitDevice( m_pDevice , 4.0f );
		m_Cloud.LoadTexture( m_pDevice , SetEnvData->CloudIndex , ENV_DAYLIGHT );
	}

	LoadingBarWnd.SetPercent(85);

	MakeObjectBoundBox();

	return	m_bReady = true;
}

/// sooree pack
bool	CBravoMap::LoadVF(DWORD idx)
{

	//ProfileBegin("< CBravoMap::LoadVF >");
	LoadingBarWnd.SetPercent(50);		

	int current = timeGetTime();

	//ProfileBegin( "Cleanup" );
	Cleanup();
	//ProfileEnd( "Cleanup" );

	//ProfileBegin( "CleanupObj" );
	CleanupObj();	
	//ProfileEnd( "CleanupObj" );

#ifdef NEW_CLIENT_MAPTYPE	
	m_uMapType = G_MAP_MAPTYPE(idx);
#endif	

	//ProfileBegin( "CleanupWaterfall" );
	m_bObjSortInit = false;		// 거리에 따른 Alpha Obj 처리를 위해..
	CleanupWaterfall();	
	//ProfileEnd( "CleanupWaterfall" );

	//ProfileBegin( "LoadVF_ETC" );
	
	SAFE_DELETE_ARRAY( m_pNpcList );
	
	m_NpcCount = 0;

	DebugPrint( "\r\n LoadVF -->[1] :%.3f", SAFE_TIME_SUB( timeGetTime() , current ) / 1000.0f );
	///--PRINT_DLG_LOG(" LoadVF -->[1] :%.3f", SAFE_TIME_SUB( timeGetTime() , current ) / 1000.0f );

	// 현재 맵 번호저장
	m_NowMapID = idx;

	sprintf( theMapPath, "%s\\data\\map\\map%d", g_RockClient.GetRootDir(), idx );
	g_RockClient.SetCurrentMapPath( theMapPath );

	char	name[MAX_PATH];		
	sprintf( name, "%s\\map\\map%d.rvf", g_RockClient.GetRootDir(), idx );

	/// 이 안에서 CloseFileSystem() 미리 한번 하니까, 전에 열었던 놈은 자동으로 닫힘 
	if( g_VFMap.OpenFileSystem( name ) == FALSE ) return false;
	
	if(m_uMapType == MAP_TYPE_COLONY)
	{
		sprintf( name, "%d_%d.orm", idx, m_nExColInfo.m_nMapIndex );
	}
	else
	{
		sprintf( name, "%d.orm", idx );
	}

	//ProfileEnd( "LoadVF_ETC" );
	
	VFileHandle* pFH = g_VFMap.OpenFile( name );
	if( pFH == NULL )
	{
		char temp[256];
		sprintf( temp, "!%s -> %d 맵을 로딩할 수 없습니다", name, idx );
///		MessageBox(NULL, temp, NULL, MB_OK);
		return	false;
	}

	//ProfileBegin( "m_NewFile" );
	pFH->Read( &m_NewFile.byType				, sizeof(BYTE		) );		
	pFH->Read( &m_NewFile.cSize					, sizeof(WORD		) );
	pFH->Read( &m_NewFile.byTextureIndex		, sizeof(BYTE		) );
	pFH->Read( &m_NewFile.bySkyTextureIndex		, sizeof(BYTE		) );
	pFH->Read( &m_NewFile.byCloudTextureIndex	, sizeof(BYTE		) );
	pFH->Read( &m_NewFile.byWaterCount			, sizeof(BYTE		) );	/// sooree water
	pFH->Read( &m_NewFile.bShowSky				, sizeof(BOOL		) );
	pFH->Read( &m_NewFile.bShowCloud			, sizeof(BOOL		) );

	pFH->Read( &m_NewFile.bShowWater			, sizeof(BOOL		) );	/// 읽기는 읽지만 무시합니다. 	
	pFH->Read( &m_NewFile.byWaterHeightVariance	, sizeof(BYTE		) );	/// 읽기는 읽지만 무시합니다. 
	pFH->Read( &m_NewFile.fWaterBaseHeight		, sizeof(float		) );	/// 읽기는 읽지만 무시합니다. 
	pFH->Read( &m_NewFile.byWaterRunsDir		, sizeof(BYTE		) );	/// 읽기는 읽지만 무시합니다. 

#ifdef DIRECT_VERSION_9_MJH
	pFH->Read( &m_NewFile.d3dMaterial			, sizeof(D3DMATERIAL9) );
	pFH->Read( &m_NewFile.d3dLight				, sizeof(D3DLIGHT9	) );
#else
	pFH->Read( &m_NewFile.d3dMaterial			, sizeof(D3DMATERIAL8) );
	pFH->Read( &m_NewFile.d3dLight				, sizeof(D3DLIGHT8	) );
#endif // DIRECT_VERSION_9_MJH
	pFH->Read( &m_NewFile.nObjCount				, sizeof(WORD		) );// 맵의 오브젝트 개수를 읽어온다
	pFH->Read( &m_NewFile.nNpcCount				, sizeof(WORD		) );
	pFH->Read( &m_NewFile.nGrsCount				, sizeof(WORD		) );
	pFH->Read( &m_NewFile.nEffectCount			, sizeof(WORD		) );
	pFH->Read( &m_NewFile.nWaterfallCount		, sizeof(WORD		) );

	m_NewFile.pAttrList2		= SAFE_NEW_ARRAY( MAP_ATTR2 , (m_NewFile.cSize<<1)*(m_NewFile.cSize<<1) );	
	m_NewFile.pHeightList		= SAFE_NEW_ARRAY( short , (m_NewFile.cSize+1)*(m_NewFile.cSize+1) );	
	m_NewFile.pColorList		= SAFE_NEW_ARRAY( COLOR_RGB , (m_NewFile.cSize+1)*(m_NewFile.cSize+1) );	
	m_NewFile.pTileIndexList	= SAFE_NEW_ARRAY( WORD , m_NewFile.cSize*m_NewFile.cSize );	
	
	if( m_NewFile.nObjCount )// 배경 오브젝트 개수만큼 리스트 생성
	{
		m_NewFile.pObjList		= SAFE_NEW_ARRAY( OBJ_INFO , m_NewFile.nObjCount );		
	}	

	m_NewFile.pNpcList			= SAFE_NEW_ARRAY( NPC_INFO2 , m_NewFile.nNpcCount );	
	if( m_NewFile.nEffectCount )
	{
		m_NewFile.pEffectList	= SAFE_NEW_ARRAY( PARTICLE_INFO , m_NewFile.nEffectCount );		
	}
	m_GrassPoint.m_Data			= SAFE_NEW_ARRAY( GRASS_POINT , m_NewFile.nGrsCount );	
	m_NewFile.pWaterfallList	= SAFE_NEW_ARRAY( WATERFALL_INFO , m_NewFile.nWaterfallCount );	

	pFH->Read( m_NewFile.pAttrList2			, sizeof(MAP_ATTR2)		* (m_NewFile.cSize<<1)*(m_NewFile.cSize<<1)	);
	pFH->Read( m_NewFile.pHeightList		, sizeof(short)			* (m_NewFile.cSize+1)*(m_NewFile.cSize+1)	);
	pFH->Read( m_NewFile.pColorList			, sizeof(COLOR_RGB)		* (m_NewFile.cSize+1)*(m_NewFile.cSize+1)	);
	pFH->Read( m_NewFile.pTileIndexList		, sizeof(WORD)			* m_NewFile.cSize*m_NewFile.cSize			);

	if( m_NewFile.nObjCount )// 배경 오브젝트 리스트 읽어온다
	{
		pFH->Read( m_NewFile.pObjList			, sizeof(OBJ_INFO)		* m_NewFile.nObjCount						);
	}	

	pFH->Read( m_NewFile.pNpcList			, sizeof(NPC_INFO2)		* m_NewFile.nNpcCount						);
	if( m_NewFile.nEffectCount )
	{
		pFH->Read( m_NewFile.pEffectList		, sizeof(PARTICLE_INFO)	* m_NewFile.nEffectCount					);
	}
	pFH->Read( m_GrassPoint.m_Data			, sizeof(GRASS_POINT)	* m_NewFile.nGrsCount						);
	pFH->Read( m_NewFile.pWaterfallList		, sizeof(WATERFALL_INFO)* m_NewFile.nWaterfallCount					);
	pFH->Read( m_NewFile.WarpList			, sizeof(FILEWARP)		* 9											);

	/// sooree water : file close를 위해 앞으로 당김 
	short i = 0;
	theWaterListNum = 0;
	//ProfileEnd( "m_NewFile" );
	if( m_NewFile.byWaterCount > 0 )
	{
		theWaterList = SAFE_NEW_ARRAY( CRockWater , m_NewFile.byWaterCount );		
		if( theWaterList )
		{
			theWaterListNum = m_NewFile.byWaterCount;

			SRockWaterInfo waterInfo;
#ifdef DIRECT_VERSION_9_MJH
			LPDIRECT3DTEXTURE9 tex0 = NULL;
			LPDIRECT3DTEXTURE9 tex1 = NULL;
#else
			LPDIRECT3DTEXTURE8 tex0 = NULL;
			LPDIRECT3DTEXTURE8 tex1 = NULL;
#endif // DIRECT_VERSION_9_MJH

			union 
			{
				unsigned char value;
				
				struct 
				{
					unsigned char data0:4;
					unsigned char data1:4;
					
				}Bit;
				
			}Data;

			//ProfileBegin( "(for)waterInfo" );
			for( i = 0; i < theWaterListNum; ++i )
			{
				if( pFH->Read( &waterInfo, sizeof( SRockWaterInfo ) ) )
				{
					Data.value = ( unsigned char ) waterInfo.theTextureIndexs;

					tex0 = g_WaterTexture.GetTexture( Data.Bit.data0 );

					if( Data.Bit.data1 >= g_WaterTexture.GetCount() )
					{
						tex1 = NULL;
					}
					else
						tex1 = g_WaterTexture.GetTexture( Data.Bit.data1 );
					
					if( theWaterList[i].Create( m_pDevice, tex0, tex1, &waterInfo ) == false )
					{
						DebugPrint( "!내부 오류로 물 생성 실패\r\n");
						continue;
					}
				} 
			} /// for
			//ProfileEnd( "(for)waterInfo" );
		} /// if
	} //// if

	g_VFMap.CloseFile( pFH );

	if(m_uMapType == MAP_TYPE_COLONY)
	{
		///-- By simwoosung
		///-- 맵이 콜로니 맵일 경우 embobj.txt - 엠블렌 적용 obj파일과 
		///-- nationobj.txt 국가 분류로 나누어진 obj파일을 읽는다.
		LoadEmbObjAndNationObj();		
	}
	
	// setup field
	m_byMapType				= m_NewFile.byType;
	m_cSize					= m_NewFile.cSize;
	///-- m_cSize는 텍스쳐 사이즈의 절반
	
	m_bySelTexture			= m_NewFile.byTextureIndex;
	m_byCurSkyTexNum		= m_NewFile.bySkyTextureIndex;
	m_byCurCloudTexNum		= m_NewFile.byCloudTextureIndex;

	m_bShowSky				= TRUE;
	m_bShowCloud			= m_NewFile.bShowCloud;

/*	/// sooree water
	/// *.orm 구조 변경 : RockWater.h 참조 
	m_byCurWaterTexNum		= m_NewFile.byWaterTextureIndex;
	m_bShowWater			= m_NewFile.bShowWater;
	m_byHighestWaterPlus	= m_NewFile.byWaterHeightVariance;
	m_fBaseHeightWater		= m_NewFile.fWaterBaseHeight;
	m_byRunsDir				= m_NewFile.byWaterRunsDir; 
	m_BravoWater.m_byRunsDir= m_NewFile.byWaterRunsDir;	*/
	m_byCurWaterTexNum		= 0;
	m_byHighestWaterPlus = 0;
	m_fBaseHeightWater = 0;
	m_byRunsDir = 0;


	m_Material				= m_NewFile.d3dMaterial;
	m_Material.Diffuse.a	= 1.0f;
	m_Light					= m_NewFile.d3dLight;
	m_Light.Diffuse.a		= 1.0f;
	
	//	m_dwObjCount			= m_NewFile.nObjCount;
	m_dwNpcCount			= m_NewFile.nNpcCount;	
	m_dwGrassCount			= m_NewFile.nGrsCount;
	m_nParticleCount		= m_NewFile.nEffectCount;
	m_nWaterfallCount		= m_NewFile.nWaterfallCount;

	/// sooree ter opt : theTerrain.CreateTerrain() 앞으로 위치 조정.
	//ProfileBegin( "g_TileTexture.FinalCleanup" );
	g_TileTexture.FinalCleanup();
	//ProfileEnd( "g_TileTexture.FinalCleanup" );

	//ProfileBegin( "g_TileTexture.LoadAllUnits" );
	g_TileTexture.LoadAllUnits( theMapPath );/// base, detail texture loading
	//ProfileEnd( "g_TileTexture.LoadAllUnits" );

	///-- m_pDisplayMap[y][x]의 각 타일을 구성하는 네좌표와 텍스쳐인덱스를 채운다.
	//ProfileBegin( "LoadTerrainNew" );
	LoadTerrainNew(m_NewFile.pHeightList, m_NewFile.pTileIndexList, m_NewFile.pColorList);
	//ProfileEnd( "LoadTerrainNew" );

	/// sooree terrain
	//ProfileBegin( "theTerrain.FinalCleanup" );
	theTerrain.FinalCleanup();
	//ProfileEnd( "theTerrain.FinalCleanup" );

	///-- 지형 생성
	//ProfileBegin( "theTerrain.CreateTerrain" );
	theTerrain.CreateTerrain(	m_pDevice, &g_Camera, 8000, 
								m_NewFile.cSize, m_NewFile.pHeightList, m_NewFile.pTileIndexList );
	//ProfileEnd( "theTerrain.CreateTerrain" );

	///-- 배경시야 설정
	//ProfileBegin( "theTerrain.SetLODQuality" );
	theTerrain.SetLODQuality( g_LODRange );
	//ProfileEnd( "theTerrain.SetLODQuality" );

	/// sooree ter opt
	if( g_ViewLODTerrain == 0 )
	{
		g_Pc_Manager.theFog_End = ( ( g_LODRange + 1.2f ) * PATCH_SIZE ) * VERTEX_INTERVAL;
	}

	m_dwNpcCount			= m_NewFile.nNpcCount;	
	m_dwGrassCount			= m_NewFile.nGrsCount;
	m_nParticleCount		= m_NewFile.nEffectCount;
	m_nWaterfallCount		= m_NewFile.nWaterfallCount;

	m_Material.Diffuse.r	= 0.70f;
	m_Material.Diffuse.g	= 0.80f;
	m_Material.Diffuse.b	= 0.75f;
	m_Material.Ambient.r	= 0.75f;
	m_Material.Ambient.g	= 0.85f;
	m_Material.Ambient.b	= 0.80f;
	m_Material.Specular.r	= 0.70f;
	m_Material.Specular.g	= 0.80f;
	m_Material.Specular.b	= 0.75f;	
	m_Material.Emissive.r	= 0.70f;
	m_Material.Emissive.g	= 0.80f;
	m_Material.Emissive.b	= 0.75f;
	m_Material.Power		= 1.0f;
	
	DebugPrint( "\r\n LoadVF -->[2] :%.3f", SAFE_TIME_SUB( timeGetTime() , current ) / 1000.0f );
	///--PRINT_DLG_LOG( "LoadVF -->[2] :%.3f", SAFE_TIME_SUB( timeGetTime() , current ) / 1000.0f );

#ifdef _DEBUG
{
	DWORD tick = timeGetTime();
#endif
	
	LoadingBarWnd.SetPercent(65);	

	//ProfileBegin( "g_Object.FinalCleanup" );
	g_Object.FinalCleanup();/// 맵별 오브젝트 리스트 작성
	//ProfileEnd( "g_Object.FinalCleanup" );
	
	//ProfileBegin( "g_Object.Create" );
	if( g_Object.Create( theMapPath ) == false )
	{
		return false;
	}
	//ProfileEnd( "g_Object.Create" );

	//ProfileBegin( "CreateObjIndex" );
	CreateObjIndex(); //.obj 로딩
	//ProfileEnd( "CreateObjIndex" );

#ifdef _DEBUG	
	DebugPrint( "!\r\n Obj Load : %.3f초", SAFE_TIME_SUB( timeGetTime() , tick ) / 1000.0f );
	///--PRINT_DLG_LOG("Obj Load : %.3f초", SAFE_TIME_SUB( timeGetTime() , tick ) / 1000.0f);	
	DebugPrint( "!\r\n LoadVF -->[3] :%.3f\r\n", SAFE_TIME_SUB( timeGetTime() , current ) / 1000.0f );
	///--PRINT_DLG_LOG("LoadVF -->[3] :%.3f\r\n", SAFE_TIME_SUB( timeGetTime() , current ) / 1000.0f );	
}
#endif

	LoadingBarWnd.SetPercent(75);

	///------------------------------------------------------------------------
	DWORD x, y;
	//ProfileBegin( "m_pAttrMap(for)" );
	for( y = 0; y < m_cSize * 2; ++y )
	{
		for( x = 0; x < m_cSize * 2; ++x )
		{
			m_pAttrMap[y][x].byAttr		= (BYTE)m_NewFile.pAttrList2[(y*(m_cSize<<1))+x].byAttr;
			m_pAttrMap[y][x].byType		= (BYTE)m_NewFile.pAttrList2[(y*(m_cSize<<1))+x].byType;
			
			/// sooree height : map editor에서 속성 편집된 Cell만 속성 높이 가진다 ( MATTR_SELECT, MATTR_SELECTOLD )
			if( m_pAttrMap[y][x].byAttr == 13 || m_pAttrMap[y][x].byAttr == 14 )
			{
				m_pAttrMap[y][x].nHeight	= (short)m_NewFile.pAttrList2[(y*(m_cSize<<1))+x].wHeight;
			}
			else
			{
				m_pAttrMap[y][x].nHeight = 0;
			}
		}
	}
	//ProfileEnd( "m_pAttrMap(for)" );
	
	/// 못가는 셀 주변에 속성 높이가 있다면 못가는 셀도 주변 속성 높이에 맞추어 주자 
	short maxAttrHT;
	//ProfileBegin( "m_pAttrMap2(for)" );
	for( y = 1; y < m_cSize * 2 - 1; ++y )
	{
		for( x = 1; x < m_cSize * 2 - 1; ++x )
		{
			if( m_pAttrMap[y][x].byAttr == MATTR_OBJ )
			{
				maxAttrHT = 0;
				if( m_pAttrMap[y-1][x].byAttr == 13 || m_pAttrMap[y-1][x].byAttr == 14 )
					maxAttrHT = max( maxAttrHT, m_pAttrMap[y-1][x].nHeight );
				if( m_pAttrMap[y+1][x].byAttr == 13 || m_pAttrMap[y+1][x].byAttr == 14 )
					maxAttrHT = max( maxAttrHT, m_pAttrMap[y+1][x].nHeight );
				if( m_pAttrMap[y][x-1].byAttr == 13 || m_pAttrMap[y][x-1].byAttr == 14 )
					maxAttrHT = max( maxAttrHT, m_pAttrMap[y][x-1].nHeight );
				if( m_pAttrMap[y][x+1].byAttr == 13 || m_pAttrMap[y][x+1].byAttr == 14 )
					maxAttrHT = max( maxAttrHT, m_pAttrMap[y][x+1].nHeight );
				
				/// 보정용. 2셀까지 
				if( (y-2 > 0) && (m_pAttrMap[y-2][x].byAttr == 13 || m_pAttrMap[y-2][x].byAttr == 14 ))
					maxAttrHT = max( maxAttrHT, m_pAttrMap[y-2][x].nHeight );
				if( (y+2 < m_cSize*2-1) && (m_pAttrMap[y+2][x].byAttr == 13 || m_pAttrMap[y+2][x].byAttr == 14 ))
					maxAttrHT = max( maxAttrHT, m_pAttrMap[y+2][x].nHeight );
				if( (x-2 > 0) && (m_pAttrMap[y][x-2].byAttr == 13 || m_pAttrMap[y][x-2].byAttr == 14 ))
					maxAttrHT = max( maxAttrHT, m_pAttrMap[y][x-2].nHeight );
				if( (x+2 < m_cSize*2-1) && (m_pAttrMap[y][x+2].byAttr == 13 || m_pAttrMap[y][x+2].byAttr == 14 ))
					maxAttrHT = max( maxAttrHT, m_pAttrMap[y][x+2].nHeight );

				if( maxAttrHT > m_pDisplayMap[y / 2][x / 2].Pos[0].y )	/// 지형 높이 보다 높아야 겠지? 
				{
					m_pAttrMap[y][x].nHeight = maxAttrHT;
				}
			}
		}
	}//ProfileEnd( "m_pAttrMap2(for)" );

	//ProfileBegin( "SetupWarpInfo(for)" );
	for ( i = 0; i < 9; i ++ )
	{
		SetupWarpInfo(i, m_NewFile.WarpList[i].nDestMapID, m_NewFile.WarpList[i].nDestWarpIdx);
	}
	//ProfileEnd( "SetupWarpInfo(for)" );

	//ProfileBegin( "InitMapData" );
	InitMapData();	
	//ProfileEnd( "InitMapData" );
	
	//ProfileBegin( "m_Block(for)" );
	
	for( int block_x = 0; block_x < m_cSize * 2; ++block_x )
	{
		for( int block_y= 0; block_y < m_cSize * 2; ++block_y )
		{			
			m_Block[block_x][block_y].Attr = m_pAttrMap[block_y][block_x].byAttr;
			
			if( m_Block[block_x][block_y].Attr == 12 || m_Block[block_x][block_y].Attr == 13 
				|| m_Block[block_x][block_y].Attr == 14 )
			{
				m_Block[block_x][block_y].Attr = 0;	
			}
		}
	}
	
	//ProfileEnd( "m_Block(for)" );

	m_GrassPoint.m_nCount	= m_NewFile.nGrsCount;
	
	if ( m_GrassPoint.m_nCount )
	{
		//ProfileBegin( "BuildGrassWithPoint(for)" );
		for ( i = 0; i < m_NewFile.nGrsCount; i ++ )
		{
			m_GrassPoint.m_Data[i].bIsReady	= true;
			BuildGrassWithPoint(m_GrassPoint.m_Data[i].idx, i);
		}
		//ProfileEnd( "BuildGrassWithPoint(for)" );
	}
	
	
	m_dwGrassCount	= 0;
	//ProfileBegin( "CreateObjNode" );
	CreateObjNode();
	//ProfileEnd( "CreateObjNode" );

	m_dwGrassCount	= m_NewFile.nGrsCount;

	//ProfileBegin( "CreateParticleTemp" );
	for ( i = 0; i < m_File.wParticleCount; i ++ )
	{
		CreateParticleTemp(m_File.pParticleList[i].wIndex);
		m_ParticleTemp.m_vMoveLocation	= m_File.pParticleList[i].vPos;
		m_ParticleTemp.m_fRotTheta	= m_File.pParticleList[i].fRot;
		AddParticle();
	}
	//ProfileEnd( "CreateParticleTemp" );

	//ProfileBegin( "LoadVF_ETC2" );
	for ( i = 0; i < m_nWaterfallCount; i ++ )
	{
//		m_WaterfallList[i].Build(m_pDevice, g_WaterfallTexture.GetTexture(m_NewFile.pWaterfallList[i].wIndex), g_WaterfallTexture.GetFallTexture(m_NewFile.pWaterfallList[i].wIndex), m_NewFile.pWaterfallList[i].wIndex);
//		m_WaterfallList[i].SetPosTM(m_NewFile.pWaterfallList[i].matPos);
	}

	m_NpcCount = m_NewFile.nNpcCount;
	m_pNpcList = SAFE_NEW_ARRAY( NPC_INFO2 , m_NpcCount );
	
	for( i = 0 ; i < m_NpcCount ; ++i )
	{
		memcpy( &m_pNpcList[ i ] , &m_NewFile.pNpcList[ i ] , sizeof( NPC_INFO2 ) );

		m_pNpcList[ i ].vPos.x = ( int ) ( m_pNpcList[ i ].vPos.x / 8 );
		m_pNpcList[ i ].vPos.z = ( int ) ( m_pNpcList[ i ].vPos.z / 8 );
	}
	
	SAFE_DELETE_ARRAY( m_NewFile.pAttrList2 );
	SAFE_DELETE_ARRAY( m_NewFile.pHeightList );
	SAFE_DELETE_ARRAY( m_NewFile.pColorList );
	SAFE_DELETE_ARRAY( m_NewFile.pTileIndexList );
	SAFE_DELETE_ARRAY( m_NewFile.pEffectList );	
	
	if( m_NewFile.nObjCount )
	{
		SAFE_DELETE_ARRAY(m_NewFile.pObjList);		
	}

	SAFE_DELETE_ARRAY(m_NewFile.pNpcList);

	if( m_NewFile.nEffectCount )
	{
		SAFE_DELETE_ARRAY(m_NewFile.pEffectList);
	}

	SAFE_DELETE_ARRAY(m_NewFile.pWaterfallList);
#ifdef _DEBUG
	DebugPrint( "\r\n LoadVF -->[4] :%.3f\r\n", SAFE_TIME_SUB( timeGetTime() , current ) / 1000.0f );
#endif
	///--PRINT_DLG_LOG("LoadVF -->[4] :%.3f\r\n", SAFE_TIME_SUB( timeGetTime() , current ) / 1000.0f );

	//ProfileEnd( "LoadVF_ETC2" );
	//........................................................................................................
	// 환경 관련 처리 ( 하늘 / 구름 / 머터리얼 / 라이트 ) 밤낮 나중에 추가 By wxywxy
	//........................................................................................................
	if( m_SetEnv.Set( idx , ENV_DAYLIGHT ) )
	{
	
		LPENV_DATA SetEnvData = m_SetEnv.GetSet();
	
		m_SkyBox.EnableSkyBox( TRUE );
		//ProfileBegin( "m_SkyBox.InitDevice" );
		m_SkyBox.InitDevice( m_pDevice , 4.0f , 4.0f , 4.0f );
		//ProfileEnd( "m_SkyBox.InitDevice" );
		
		//ProfileBegin( "m_SkyBox.LoadTexture" );
		m_SkyBox.LoadTexture( m_pDevice , SetEnvData->SkyIndex , 0 );	
		//ProfileEnd( "m_SkyBox.LoadTexture" );

		m_Cloud.EnableCloud( m_bShowCloud );
		//ProfileBegin( "m_Cloud.InitDevice" );
		m_Cloud.InitDevice( m_pDevice , 4.0f );
		//ProfileEnd( "m_Cloud.InitDevice" );

		//ProfileBegin( "m_Cloud.LoadTexture" );
		m_Cloud.LoadTexture( m_pDevice , SetEnvData->CloudIndex , ENV_DAYLIGHT );
		//ProfileEnd( "m_Cloud.LoadTexture" );
	}

	LoadingBarWnd.SetPercent(85);	
	
	//ProfileEnd( "< CBravoMap::LoadVF >" );

	MakeObjectBoundBox();

	return	m_bReady = true;
}

// Load(), LoadVF(), ReSetMapIndex()에서 호출
void CBravoMap::MakeObjectBoundBox(void)
{
	if( m_bObjSortInit == FALSE )
	{
		//ProfileBegin("< m_bObjSortInit >");	

		int   number = 0;
		float Temp = 0.0f;

		D3DXVECTOR3 vMin( 0.0f , 0.0f , 0.0f );
		D3DXVECTOR3 vMax( 0.0f , 0.0f , 0.0f );

		for( int i = 0; i < m_ObjCount; ++i )
		{	
			for( int j = 0; j < m_Object[i].m_child_count; ++j )
			{
				m_SortTable.Insert( ID_OBJ , i , j , FALSE );

				vMin	= m_Object[i].GetBoundMinPos();
				vMax	= m_Object[i].GetBoundMaxPos();

				m_Object[i].m_pAniPos[j].m_vBoundList[ 0 ] = D3DXVECTOR3( vMin.x , vMin.y , vMin.z );
				m_Object[i].m_pAniPos[j].m_vBoundList[ 1 ] = D3DXVECTOR3( vMin.x , vMax.y , vMin.z );
				m_Object[i].m_pAniPos[j].m_vBoundList[ 2 ] = D3DXVECTOR3( vMax.x , vMin.y , vMin.z );
				m_Object[i].m_pAniPos[j].m_vBoundList[ 3 ] = D3DXVECTOR3( vMax.x , vMax.y , vMin.z );
				m_Object[i].m_pAniPos[j].m_vBoundList[ 4 ] = D3DXVECTOR3( vMin.x , vMin.y , vMax.z );
				m_Object[i].m_pAniPos[j].m_vBoundList[ 5 ] = D3DXVECTOR3( vMin.x , vMax.y , vMax.z );
				m_Object[i].m_pAniPos[j].m_vBoundList[ 6 ] = D3DXVECTOR3( vMax.x , vMin.y , vMax.z );
				m_Object[i].m_pAniPos[j].m_vBoundList[ 7 ] = D3DXVECTOR3( vMax.x , vMax.y , vMax.z );

				D3DXVec3TransformCoord(&m_Object[i].m_pAniPos[j].m_vBoundList[ 0 ], &m_Object[i].m_pAniPos[j].m_vBoundList[ 0 ], &m_Object[i].m_pAniPos[j].GetPosTM());
				D3DXVec3TransformCoord(&m_Object[i].m_pAniPos[j].m_vBoundList[ 1 ], &m_Object[i].m_pAniPos[j].m_vBoundList[ 1 ], &m_Object[i].m_pAniPos[j].GetPosTM());
				D3DXVec3TransformCoord(&m_Object[i].m_pAniPos[j].m_vBoundList[ 2 ], &m_Object[i].m_pAniPos[j].m_vBoundList[ 2 ], &m_Object[i].m_pAniPos[j].GetPosTM());
				D3DXVec3TransformCoord(&m_Object[i].m_pAniPos[j].m_vBoundList[ 3 ], &m_Object[i].m_pAniPos[j].m_vBoundList[ 3 ], &m_Object[i].m_pAniPos[j].GetPosTM());
				D3DXVec3TransformCoord(&m_Object[i].m_pAniPos[j].m_vBoundList[ 4 ], &m_Object[i].m_pAniPos[j].m_vBoundList[ 4 ], &m_Object[i].m_pAniPos[j].GetPosTM());
				D3DXVec3TransformCoord(&m_Object[i].m_pAniPos[j].m_vBoundList[ 5 ], &m_Object[i].m_pAniPos[j].m_vBoundList[ 5 ], &m_Object[i].m_pAniPos[j].GetPosTM());
				D3DXVec3TransformCoord(&m_Object[i].m_pAniPos[j].m_vBoundList[ 6 ], &m_Object[i].m_pAniPos[j].m_vBoundList[ 6 ], &m_Object[i].m_pAniPos[j].GetPosTM());
				D3DXVec3TransformCoord(&m_Object[i].m_pAniPos[j].m_vBoundList[ 7 ], &m_Object[i].m_pAniPos[j].m_vBoundList[ 7 ], &m_Object[i].m_pAniPos[j].GetPosTM());

				D3DXPlaneFromPoints( &m_Object[i].m_pAniPos[j].m_Plane[ 0 ] , &m_Object[i].m_pAniPos[j].m_vBoundList[ 3 ] ,
													 &m_Object[i].m_pAniPos[j].m_vBoundList[ 1 ] ,
													 &m_Object[i].m_pAniPos[j].m_vBoundList[ 0 ]  );

				D3DXPlaneFromPoints( &m_Object[i].m_pAniPos[j].m_Plane[ 1 ] , &m_Object[i].m_pAniPos[j].m_vBoundList[ 0 ] ,
													 &m_Object[i].m_pAniPos[j].m_vBoundList[ 1 ] ,
													 &m_Object[i].m_pAniPos[j].m_vBoundList[ 5 ]  );

				D3DXPlaneFromPoints( &m_Object[i].m_pAniPos[j].m_Plane[ 2 ] , &m_Object[i].m_pAniPos[j].m_vBoundList[ 5 ] ,
													 &m_Object[i].m_pAniPos[j].m_vBoundList[ 7 ] ,
													 &m_Object[i].m_pAniPos[j].m_vBoundList[ 6 ]  );

				D3DXPlaneFromPoints( &m_Object[i].m_pAniPos[j].m_Plane[ 3 ] , &m_Object[i].m_pAniPos[j].m_vBoundList[ 6 ] ,
													 &m_Object[i].m_pAniPos[j].m_vBoundList[ 7 ] ,
													 &m_Object[i].m_pAniPos[j].m_vBoundList[ 3 ]  );

				D3DXPlaneFromPoints( &m_Object[i].m_pAniPos[j].m_Plane[ 4 ] , &m_Object[i].m_pAniPos[j].m_vBoundList[ 7 ] ,
													 &m_Object[i].m_pAniPos[j].m_vBoundList[ 5 ] ,
													 &m_Object[i].m_pAniPos[j].m_vBoundList[ 1 ]  );

				D3DXPlaneFromPoints( &m_Object[i].m_pAniPos[j].m_Plane[ 5 ] , &m_Object[i].m_pAniPos[j].m_vBoundList[ 0 ] ,
													 &m_Object[i].m_pAniPos[j].m_vBoundList[ 4 ] ,
													 &m_Object[i].m_pAniPos[j].m_vBoundList[ 6 ]  );

				
				D3DXVec3TransformCoord(&m_Object[i].m_pAniPos[j].m_vMin, &vMin, &m_Object[i].m_pAniPos[j].GetPosTM());
				D3DXVec3TransformCoord(&m_Object[i].m_pAniPos[j].m_vMax, &vMax, &m_Object[i].m_pAniPos[j].GetPosTM());
				
				if( m_Object[i].m_pAniPos[j].m_vMin.x > m_Object[i].m_pAniPos[j].m_vMax.x )
				{
					Temp = m_Object[i].m_pAniPos[j].m_vMin.x;
					m_Object[i].m_pAniPos[j].m_vMin.x = m_Object[i].m_pAniPos[j].m_vMax.x;
					m_Object[i].m_pAniPos[j].m_vMax.x = Temp;
				}
				
				if( m_Object[i].m_pAniPos[j].m_vMin.y > m_Object[i].m_pAniPos[j].m_vMax.y )
				{
					Temp = m_Object[i].m_pAniPos[j].m_vMin.y;
					m_Object[i].m_pAniPos[j].m_vMin.y = m_Object[i].m_pAniPos[j].m_vMax.y;
					m_Object[i].m_pAniPos[j].m_vMax.y = Temp;
				}

				if( m_Object[i].m_pAniPos[j].m_vMin.z > m_Object[i].m_pAniPos[j].m_vMax.z )
				{
					Temp = m_Object[i].m_pAniPos[j].m_vMin.z;
					m_Object[i].m_pAniPos[j].m_vMin.z = m_Object[i].m_pAniPos[j].m_vMax.z;
					m_Object[i].m_pAniPos[j].m_vMax.z = Temp;
				}
				
				m_Object[i].m_pAniPos[j].m_vSize = m_Object[i].m_pAniPos[j].m_vMax - m_Object[i].m_pAniPos[j].m_vMin;

// 				if( m_Object[i].m_pAniPos[j].m_vSize.x < 0.0f || 
// 					m_Object[i].m_pAniPos[j].m_vSize.y < 0.0f ||
// 					m_Object[i].m_pAniPos[j].m_vSize.z < 0.0f )
// 				{
// 					int a = 0;
// 				}

				number++;
			}
		}
		
		g_Map.m_SortTable.Insert( ID_PLAYER , 0 , 0 , FALSE );

		m_SortCount = number;

		m_bObjSortInit = TRUE;

		//ProfileEnd( "< m_bObjSortInit >");	
	}
}

bool CBravoMap::UpdateTerrain( int cx, int cy, bool bAlways )
{
	///ProfileBegin( "UpdateTerrain" );

	if( bAlways == false && ( ( thePreCX == cx && thePreCY == cy ) ) )
	{
		return false; 
	}
	else
	{
		thePreCX = short( cx );
		thePreCY = short( cy );
	}

	if( !m_bReady )
	{
		return	false;	
	}

//	qsort( (void *)&m_SortTable.m_SortList[ 0 ], m_SortTable.GetTableMaxCount() , sizeof(NODE_SORT_DATA) , NodeSortProcess );		
	
	/// sooree terrain
	///-- 캐릭터 위치를 기준으로 터레인을 재구성한다.
	theTerrain.Reset( cx * CLIENT_TILESIZE + CLIENT_TILESIZE/2, cy * CLIENT_TILESIZE + CLIENT_TILESIZE/2 );
	/// sooree ter opt
		
	///-- 게임 내에서 원경지형이 ROAM 지형을 뜻한다.
	if( g_ViewLODTerrain )
	{
		theTerrain.Tessellate();
		///-- 이곳에서 ROAM을 구성하는 Patch의 인덱스버퍼를 재구성한다. 
		theTerrain.Update();	
	}

// 타일속성	
#ifdef BLOCK_DEBUG
	updateonter( cx, cy );
#endif

	/// sooree terrain 
	///==================================================================================================
	///	그림자 그리는 것 때문에 기존의 바닥 그리던 버텍스 버퍼는 계속 갱신해줍니다.
	/// 이것도 새로운 지형 엔진에 맞추어 재작업될 필요가 있습니다.
	///==================================================================================================

	///-- 현재 위치를 반으로 나누고
	///-- 타일링 영역을 위한 시작과 끝 인덱스를 구한다.
	DWORD	y, x;	
	int		divx	= (cx >> 1);
	int		divy	= (cy >> 1);

	sx	= ( ( (divx) - DISPLAY_RANGE ) < 0 ) ? 0 : (divx) - DISPLAY_RANGE;
	sy	= ( ( (divy) - DISPLAY_RANGE ) < 0 ) ? 0 : (divy) - DISPLAY_RANGE;
	ex	= ( ( (divx) + DISPLAY_RANGE ) >= m_cSize) ? m_cSize : (divx) + DISPLAY_RANGE;
	ey	= ( ( (divy) + DISPLAY_RANGE ) >= m_cSize) ? m_cSize : (divy) + DISPLAY_RANGE;

	dwTileVisibleCount	= 0;

	float	centerh		= 0;
	float	interval	= 0;
#ifdef DIRECT_VERSION_9_MJH
	if ( FAILED ( m_pIBuffer->Lock(0, 0, (void**)&m_pIdxList, 0 ) ) )
#else
	if ( FAILED ( m_pIBuffer->Lock(0, 0, (BYTE**)&m_pIdxList, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH
	{
		return false;
	}
#ifdef DIRECT_VERSION_9_MJH
	if ( FAILED ( m_pVBuffer->Lock(0, 0, (void**)&m_pVtxList, 0 ) ) )
#else
	if ( FAILED ( m_pVBuffer->Lock(0, 0, (BYTE**)&m_pVtxList, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH
	{
		return false;
	}

	m_dwVtxCount = 0;
	m_dwIdxCount = 0;
	DWORD dwVtxCount = 0;
	DWORD dwIdxCount = 0;

	for ( y = sy; y < ey; y ++ )
	{
		for ( x = sx; x < ex; x ++ )
		{
			if ( (y >= 0 && y < m_cSize) || (x >= 0 && x < m_cSize) )
			{
				centerh	= ( m_pDisplayMap[y][x].Pos[0].y + m_pDisplayMap[y][x].Pos[2].y ) / 2;
				interval = abs( (int)( m_pDisplayMap[y][x].Pos[0].y - m_pDisplayMap[y][x].Pos[2].y ) ) * 4;

				if ( false == ( m_pDisplayMap[y][x].bIsVisible = g_Camera.CubeInFrustum( m_pDisplayMap[y][x].Pos[0].x + ( TILE_SIZE / 2 ),
																						centerh,
																						m_pDisplayMap[y][x].Pos[0].z + ( TILE_SIZE / 2 ), 
																						TILE_SIZE * 3, 
																						interval,
																						TILE_SIZE * 3 ) ) )
				{
					/// 안보이는 타일 
					continue;
				}

				++dwTileVisibleCount;

				m_pDisplayMap[y][x].vidx = dwVtxCount;
				m_pVtxList[dwVtxCount] = m_pDisplayMap[y][x].Pos[0];
				++dwVtxCount;
				m_pVtxList[dwVtxCount] = m_pDisplayMap[y][x].Pos[1];
				++dwVtxCount;
				m_pVtxList[dwVtxCount] = m_pDisplayMap[y][x].Pos[2];
				++dwVtxCount;
				m_pVtxList[dwVtxCount] = m_pDisplayMap[y][x].Pos[3];
				++dwVtxCount;

				m_pIdxList[dwIdxCount] = (WORD)dwVtxCount-4;
				++dwIdxCount;
				m_pIdxList[dwIdxCount] = (WORD)dwVtxCount-3;
				++dwIdxCount;
				m_pIdxList[dwIdxCount] = (WORD)dwVtxCount-2;
				++dwIdxCount;
				m_pIdxList[dwIdxCount] = (WORD)dwVtxCount-2;
				++dwIdxCount;
				m_pIdxList[dwIdxCount] = (WORD)dwVtxCount-1;
				++dwIdxCount;
				m_pIdxList[dwIdxCount] = (WORD)dwVtxCount-4;
				++dwIdxCount;
			}
		}
	}

	m_dwVtxCount = dwVtxCount;
	m_dwIdxCount = dwIdxCount;

	if( FAILED ( m_pIBuffer->Unlock() ) )
	{
		return false;
	}
	if( FAILED ( m_pVBuffer->Unlock() ) )
	{
		return false;
	}

	///==================================================================================================
	///ProfileBegin( "Update Object" );

	UpdateObj2();
	UpdateNpc();
	g_Pc.UpdateUserTable();
	
///		UpdateGrass();	/// 프러스텀 컬링만 하는 함수. 어차피 DisplayGrass()에서 정확히 한번 더 하기 때문에 필요 없음.
	
	UpdateParticle();
	UpdateWaterfall();

	///ProfileEnd( "Update Object" );
	///ProfileEnd( "UpdateTerrain" );

	/// Profile Log
	///ProfileDumpOutputToBuffer();
	
	return	true;
}


//-----------------------------------------------------------------------------
// Desc: 오브젝트 인덱스 생성
//-----------------------------------------------------------------------------
void	CBravoMap::CreateObjIndex()
{			
#if defined( LOAD_VF )
	g_VFMap.ChangeDir( "/obj" );	/// sooree pack
#endif

	//-----------------------------------------------------------------------------
	// 총갯수저장 
	//-----------------------------------------------------------------------------
	m_AllObjCount = m_NewFile.nObjCount;		

	//-----------------------------------------------------------------------------
	// 오브젝트 인덱스를 비교해서 같은 오브젝트가 몇 개 있는지 알아내기	
	// 총오브젝트 갯수만큼 메모리할당
	//-----------------------------------------------------------------------------
	OBJ_INFO_TABLE*	pObjTable = SAFE_NEW_ARRAY( OBJ_INFO_TABLE , m_NewFile.nObjCount );	

	int number = 0;
	for ( int i = 0; i < m_NewFile.nObjCount; ++i )
	{			
		for( int j = 0; j < SAMEOBJ_COUNT; ++j )
		{
			//-----------------------------------------------------------------------------
			// 새로운 오브젝트
			//-----------------------------------------------------------------------------
			if( pObjTable[j].Index != m_NewFile.pObjList[i].wIndex && pObjTable[j].Index == -1 )
			{				
				pObjTable[j].Index = m_NewFile.pObjList[i].wIndex;
				pObjTable[j].Same[pObjTable[j].ChildCount] = i;
				pObjTable[j].ChildCount++;
				
				// 카운트증가
				number++;
				m_ObjCount++;

				DebugPrint("m_ObjCount : %d, i = %d, pObjTable[%d].Index = %d\n", m_ObjCount, i, j, pObjTable[j].Index);
				break;
			}
			//-----------------------------------------------------------------------------
			// 기존에 있던 오브젝트
			//-----------------------------------------------------------------------------
			else if( pObjTable[j].Index == m_NewFile.pObjList[i].wIndex )
			{
				// 현재 인덱스저장
				pObjTable[j].Same[pObjTable[j].ChildCount] = i;
				pObjTable[j].ChildCount++;				
				
				// 카운트증가				
				number++;
				break;
			}
		}
	}

	//-----------------------------------------------------------------------------
	// 오브젝트생성. 애니메이션, 메트릭스 가지고 있는 클래스 같은 오브젝트 갯수만큼 메모리 할당	
	//-----------------------------------------------------------------------------
	for(int i = 0; i < m_ObjCount; ++i )
	{		
		if( pObjTable[i].ChildCount < 1 )
		{
			pObjTable[i].ChildCount = 1;
		}
		
		///-- 실제 오브젝트에 인덱스 셋팅
		m_Object[i].SetupIndex( pObjTable[i].Index );		
		m_Object[i].m_child_count = pObjTable[i].ChildCount;	

		char Obj_Name[255] = {0,};
		
		///-- 인덱스에 연결된 obj파일네임 불러오기
		char *pName = g_Object.GetName( pObjTable[i].Index );

		if( pName == NULL )
		{
			MessageBox( NULL, "Error CBravoMap::CreateObjIndex", "Error", MB_OK );

			return;
		}

		sprintf( Obj_Name, "%s", pName );

#if !defined( LOAD_VF )
		if( Obj_Name[0] == '\0' )
		{
			char error[255] = {0,};
			sprintf( error, "MapID = %d, Nothing Object File : %d.obj", m_NowMapID, pObjTable[i].Index );
			MessageBox( NULL, error, "Error", MB_OK );
		}
		else
		{
			if( m_Object[i].File__Open( Obj_Name ) == false )
			{
				return;
			}
		}
#else
		if( Obj_Name[0] == '\0' )
		{
			char error[255] = {0,};
			sprintf( error, "MapID = %d, Nothing Object File : %d.obj", m_NowMapID, pObjTable[i].Index );
			MessageBox( NULL, error, "Error", MB_OK );
		}
		else
		{
			///-- 실제 오브젝트에 obj파일을 연결 msh, dds 등록
			/// sooree pack		
			if( m_Object[i].File__OpenVF( &g_VFMap, g_Object.GetName(pObjTable[i].Index) ) == false )
			{
				return;
			}
		}
#endif
	
		for( int j = 0; j < pObjTable[i].ChildCount; ++j )
		{	
			if( &(m_Object[i].m_pAniPos[j]) )
			{
				m_Object[i].m_pAniPos[j].SetPosTM( m_NewFile.pObjList[pObjTable[i].Same[j]].matPos );			
				m_Object[i].m_pAniPos[j].m_bIsVisible = true;
			}
		}
	}

	//-----------------------------------------------------------------------------
	// 인포테이블삭제
	//----------------------------------------------------------------------------	-
	
	SAFE_DELETE_ARRAY( pObjTable );

	//-----------------------------------------------------------------------------
	// 알파오브젝트 테이블 만들기 플래그설정
	//-----------------------------------------------------------------------------
	if( m_PreMapID != m_NowMapID )
	{		
		m_bObjSortInit	= false;
		m_PreMapID		= m_NowMapID;
	}

	m_SortTable.Clear();
	m_SortTable.SetTableMaxCount( m_AllObjCount + MAX_NPC_INTABLE + MAX_USER_INTABLE );
	
	g_Particle.m_EffectManager.DeleteAllLandEffect();

}


//-----------------------------------------------------------------------------
// Desc: 그림자 생성
//-----------------------------------------------------------------------------
void	CBravoMap::CreateShadow()
{	
	if( m_bReady == true && g_Shadow > 0 )
	{
		// 그림자텍스쳐설정	
		g_RenderManager.SetTexture( 0, NULL );

		g_RenderManager.SetTFactorColor( D3DCOLOR_ARGB(100, 0, 0, 0) );

		g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_SELECTARG1 );
		g_RenderManager.SetTextureColorArg1( 0 , D3DTA_TFACTOR );
		
		
		g_RenderManager.SetTextureAlphaOP  ( 0 , D3DTOP_SELECTARG1 );
		g_RenderManager.SetTextureAlphaArg1( 0 , D3DTA_TFACTOR );

		g_RenderManager.SetAlphaBlendEnable( FALSE );

		// 메트릭스설정
		D3DXMATRIX	matInit;
		D3DXMatrixIdentity( &matInit );
		g_RenderManager.SetTransform( D3DTS_VIEW,	&matInit );	
		g_RenderManager.SetTransform( D3DTS_WORLD,	&matInit );	
		
		g_RenderManager.SetFVF( D3DFVF_OBJECTVERTEX );

		// 그림자텍스쳐생성				
		ShadowMakeNpc();
		g_Pc.ShadowMake();
//		ShadowMakeObj();

		// 메트릭스복구	
		g_RenderManager.SetTransform( D3DTS_VIEW,		&g_Camera.m_matView );	
		g_RenderManager.SetTransform( D3DTS_PROJECTION,	&g_Camera.m_matProj );	
		
		// 그림자텍스쳐초기
		g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_MODULATE );
		g_RenderManager.SetTextureColorArg1( 0 , D3DTA_TEXTURE );
		
		
		g_RenderManager.SetTextureAlphaOP  ( 0 , D3DTOP_MODULATE );
		g_RenderManager.SetTextureAlphaArg1( 0 , D3DTA_TEXTURE );
		
		g_RenderManager.SetAlphaBlendEnable( TRUE );
	}
}


//-----------------------------------------------------------------------------
// Desc: 그림자 그리기
//-----------------------------------------------------------------------------
void	CBravoMap::DisplayShadow()
{	
	if( m_bReady == true && g_Shadow > 0 )
	{		
		g_RenderManager.SetAlphaTestEnable( TRUE );
		//g_RenderManager.SetAlphaFunc( D3DCMP_ALWAYS );
		g_RenderManager.SetAlphaBlendEnable( TRUE );

		DWORD dMipFilter = 0;
		DWORD dMagFilter = 0;
		DWORD dMinFilter = 0;		
		
		// 그림자타엡에 따라서 셋팅
		switch( g_Shadow )
		{
		case 1:
			{
				g_RenderManager.SetSrcBlend( D3DBLEND_ZERO );
				g_RenderManager.SetDestBlend( D3DBLEND_INVSRCALPHA );
				
				g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_SELECTARG1 );
				g_RenderManager.SetTextureColorArg1( 0 , D3DTA_TEXTURE );				
				
				g_RenderManager.SetTextureAlphaOP  ( 0 , D3DTOP_MODULATE );
				g_RenderManager.SetTextureAlphaArg1( 0 , D3DTA_TEXTURE );

				g_RenderManager.SetTextureAddressU( 0 , D3DTADDRESS_CLAMP );
				g_RenderManager.SetTextureAddressV( 0 , D3DTADDRESS_CLAMP );
				g_RenderManager.SetTextureAddressW( 0 , D3DTADDRESS_CLAMP );
				
				// 텍스쳐 좌표를 카메라 공간에 변환된 정점의 위치를 사용한다.
				g_RenderManager.SetTextureCoordIndex(  0 , D3DTSS_TCI_CAMERASPACEPOSITION );
				
				// 동차좌표계 텍스쳐 변환 행렬을 사용하도록 설정해준다.
				g_RenderManager.SetTextureTransFormFlags( 0 , D3DTTFF_COUNT4|D3DTTFF_PROJECTED );
			}
			break;
		case 2:
			{
				g_RenderManager.SetSrcBlend( D3DBLEND_ZERO );
				g_RenderManager.SetDestBlend( D3DBLEND_INVSRCALPHA );
#ifdef DIRECT_VERSION_9_MJH
				g_RenderManager.GetSamplerState(0, D3DSAMP_MIPFILTER, &dMipFilter);
				g_RenderManager.GetSamplerState(0, D3DSAMP_MAGFILTER, &dMagFilter);
				g_RenderManager.GetSamplerState(0, D3DSAMP_MINFILTER, &dMinFilter);
#else
				g_RenderManager.GetTextureStageState(0, D3DTSS_MIPFILTER, &dMipFilter);
				g_RenderManager.GetTextureStageState(0, D3DTSS_MAGFILTER, &dMagFilter);
				g_RenderManager.GetTextureStageState(0, D3DTSS_MINFILTER, &dMinFilter);
#endif // DIRECT_VERSION_9_MJH
				
				g_RenderManager.SetTextureMipFilter( 0 , D3DTEXF_LINEAR );
				g_RenderManager.SetTextureMagFilter( 0 , D3DTEXF_LINEAR );
				g_RenderManager.SetTextureMinFilter( 0 , D3DTEXF_POINT );
				
				g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_SELECTARG1 );
				g_RenderManager.SetTextureColorArg1( 0 , D3DTA_TEXTURE );				
				
				g_RenderManager.SetTextureAlphaOP  ( 0 , D3DTOP_SELECTARG1 );
				g_RenderManager.SetTextureAlphaArg1( 0 , D3DTA_TEXTURE );

				g_RenderManager.SetTextureAddressU( 0 , D3DTADDRESS_CLAMP );
				g_RenderManager.SetTextureAddressV( 0 , D3DTADDRESS_CLAMP );
				g_RenderManager.SetTextureAddressW( 0 , D3DTADDRESS_CLAMP );
				
				// 텍스쳐 좌표를 카메라 공간에 변환된 정점의 위치를 사용한다.
				g_RenderManager.SetTextureCoordIndex(  0 , D3DTSS_TCI_CAMERASPACEPOSITION );

				// 동차좌표계 텍스쳐 변환 행렬을 사용하도록 설정해준다.
				g_RenderManager.SetTextureTransFormFlags( 0 , D3DTTFF_COUNT4|D3DTTFF_PROJECTED );
			}
			break;
		}
		
		g_RenderManager.SetTexture( 0 , NULL );
		g_RenderManager.SetTexture( 1 , NULL );

		// 그림자 그리기
		g_Pc.ShadowDisplay();		
		
		if( g_RockClient.m_SceneManager.GetNPCDisplay() )
			ShadowDisplayNpc();

		if( g_Shadow == 2 )
		{
			g_RenderManager.SetTextureMipFilter( 0 , dMipFilter );
			g_RenderManager.SetTextureMagFilter( 0 , dMagFilter );
			g_RenderManager.SetTextureMinFilter( 0 , dMinFilter );
		}

		g_RenderManager.SetTexture( 1, NULL );

		g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
		g_RenderManager.SetDestBlend( D3DBLEND_INVSRCALPHA );
		
		g_RenderManager.SetZWriteEnable( TRUE);
		g_RenderManager.SetZEnable( TRUE );

		g_RenderManager.SetTextureCoordIndex( 0 , 0 );
		g_RenderManager.SetTextureTransFormFlags( 0 , D3DTTFF_DISABLE );

		g_RenderManager.SetTextureCoordIndex( 1 , 1 );
		g_RenderManager.SetTextureTransFormFlags( 1 , D3DTTFF_DISABLE );
		
		g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_MODULATE );
		g_RenderManager.SetTextureColorArg1( 0 , D3DTA_TEXTURE );
		g_RenderManager.SetTextureColorArg2( 0 , D3DTA_DIFFUSE );
		
		g_RenderManager.SetTextureAddressU( 0 , D3DTADDRESS_WRAP );
		g_RenderManager.SetTextureAddressV( 0 , D3DTADDRESS_WRAP );
		g_RenderManager.SetTextureAddressW( 0 , D3DTADDRESS_WRAP );

		g_RenderManager.SetAlphaFunc( D3DCMP_LESSEQUAL );
	}
}


// 선택맵 디스플레이
void	CBravoMap::SetLight( int type )
{
#ifdef DIRECT_VERSION_9_MJH
	ZeroMemory( &m_Light, sizeof(D3DLIGHT9) );
#else
	ZeroMemory( &m_Light, sizeof(D3DLIGHT8) );
#endif // DIRECT_VERSION_9_MJH
	m_Light.Type		= D3DLIGHT_DIRECTIONAL;

/*
	m_Light.Diffuse.r	= 0.9f;
	m_Light.Diffuse.g	= 0.9f;
	m_Light.Diffuse.b	= 0.9f;
	m_Light.Ambient.r	= 0.7f;
	m_Light.Ambient.g	= 0.7f;
	m_Light.Ambient.b	= 0.7f;
	m_Light.Direction.x	=  0.0f;
	m_Light.Direction.y	= -1.0f;
	m_Light.Direction.z	=  1.0f;
	m_Light.Range		= 500.0f;
//*/

	
	m_Light.Diffuse.r	= 1.f;
	m_Light.Diffuse.g	= 1.f;
	m_Light.Diffuse.b	= 1.f;
	m_Light.Ambient.r	= 1.f;
	m_Light.Ambient.g	= 1.f;
	m_Light.Ambient.b	= 1.f;
	m_Light.Direction.x	=  0.0f;
	m_Light.Direction.y	= -1.0f;
	m_Light.Direction.z	=  1.0f;
	m_Light.Range		= 500.0f;


	/*
	m_Light.Specular.r	= 1.0f;
	m_Light.Specular.g	= 0.5f;
	m_Light.Specular.b	= 0.5f;
	m_Light.Specular.a	= 1.0f;
	m_Light.Attenuation0 = 1.0f;
	*/
}

void	CBravoMap::DisplayTempMap()
{
	// 옵션셋팅				
	g_RenderManager.SetFillMode( D3DFILL_SOLID );

	g_RenderManager.SetZWriteEnable(D3DZB_TRUE );
	g_RenderManager.SetZEnable( D3DZB_TRUE );
	g_RenderManager.SetZFunc( D3DCMP_LESSEQUAL );

	g_RenderManager.SetAlphaTestEnable( TRUE );
	g_RenderManager.SetAlphaRef( 0 );
	g_RenderManager.SetAlphaFunc( D3DCMP_NOTEQUAL );
	g_RenderManager.SetAlphaBlendEnable( TRUE );

	g_RenderManager.SetTextureTransFormFlags( 0 , D3DTTFF_DISABLE );

	g_RenderManager.SetTextureMipFilter ( 0 , D3DTEXF_LINEAR );
	g_RenderManager.SetTextureMagFilter ( 0 , D3DTEXF_LINEAR );
	g_RenderManager.SetTextureMinFilter ( 0 , D3DTEXF_LINEAR );

	g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureColorArg1( 0 , D3DTA_TEXTURE );
	g_RenderManager.SetTextureColorArg2( 0 , D3DTA_DIFFUSE );
	
	
	g_RenderManager.SetTextureAlphaOP  ( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureAlphaArg1( 0 , D3DTA_TEXTURE );
	g_RenderManager.SetTextureAlphaArg2( 0 , D3DTA_DIFFUSE );


	// 오브젝트라이트 사용
	g_RenderManager.SetTexture( 1, NULL );
	g_RenderManager.SetLight( 0, &m_Light );
	g_RenderManager.LightEnable( 0, TRUE );

	g_RenderManager.SetLighting( FALSE );
	g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
	g_RenderManager.SetDestBlend(  D3DBLEND_INVSRCALPHA );

	g_RenderManager.SetFVF( D3DFVF_OBJECTVERTEX );
		
	// 오브젝트
	g_RenderManager.SetCullMode( D3DCULL_NONE );

	if( m_TempObj.m_pAniPos != NULL )
	{
		m_TempObj.m_pAniPos[0].m_bIsVisible = true;
		m_TempObj.Display();
	}

	// 자기가 만든 캐릭터
	g_RenderManager.SetFVF( D3DFVF_OBJECTVERTEX );
	g_RenderManager.SetCullMode( D3DCULL_NONE );
	
	g_Pc.DisplayTempPlayer();
}

// 물 높이 구하기 By wxywxy
float CBravoMap::GetWaterHeight(float x,float z)
{
	float x0,x1;
	float y0,y1;
	
	if( theWaterList )
	{
		for( int i = 0; i < theWaterListNum; ++i )
		{

			x0 = theWaterList[i].GetWaterInfo()->theLeftX * 16.0f;
			y0 = theWaterList[i].GetWaterInfo()->theBottomZ * 16.0f;
			x1 = x0 + theWaterList[i].GetWaterInfo()->theWidth * 16.0f;
			y1 = y0 + theWaterList[i].GetWaterInfo()->theHeight * 16.0f;

			if( x0 < x && x < x1 &&
				y0 < z && x < y1   )
			{
				//return theWaterList[i].GetHeight( 0 );
				return theWaterList[i].GetWaterInfo()->theHeightBase + theWaterList[i].GetWaterInfo()->theHeightVariance + 0.5f;
			}
		}
	}

	return 0.0f;
}


HRESULT	CBravoMap::Display()
{	
	//By Simwoosung

	static int nROldDestX = -1;
	static int nROldDestY = -1;

	//현재 내가 배틀존에 있고 내가 죽어있을때
	//by simwoosung
	//int mapnum = g_Map.m_NowMapID;
	if(g_Pc.GetPlayer()->theMapType == n_FightZone)		 
	{
		if(g_Pc.GetPlayer()->IsMyPlayerRender)
		{	//자신의 캐릭터가 죽어있을경우
			if(g_Pc.GetPlayer()->m_curt_event.type == SM_STATE_DIE)
			{
				int nIndex = g_Pc.GetPlayer()->m_nViewRefDestIndex;
				
				if(nIndex != -1)	//선택된 캐릭터가 내자신이 아니라면
				{
					g_Camera.Update();
					g_Camera.MovCamera();								
					if( (nROldDestX != g_Pc.GetDestX()) || (nROldDestY != g_Pc.GetDestY()) )
					{
						nROldDestX = g_Pc.GetDestX();
						nROldDestY = g_Pc.GetDestY();
						g_Map.UpdateTerrain( g_Pc.GetDestX(), g_Pc.GetDestY(), true );
					}
				}		
			}
		}
		else
		{
			int nIndex = g_Pc.GetPlayer()->m_nViewRefDestIndex;
				
			if(nIndex != -1)	//선택된 캐릭터가 내자신이 아니라면
			{
				g_Camera.Update();
				g_Camera.MovCamera();								
				if( (nROldDestX != g_Pc.GetDestX()) || (nROldDestY != g_Pc.GetDestY()) )
				{
					nROldDestX = g_Pc.GetDestX();
					nROldDestY = g_Pc.GetDestY();
					g_Map.UpdateTerrain( g_Pc.GetDestX(), g_Pc.GetDestY(), true );
				}
			}
		}
	}	
	
	g_Particle.m_fTimeD    = (float)( SAFE_TIME_SUB( g_nowTime , g_Particle.m_lPrevTime ) ) / 1000.0f;
	g_Particle.m_lPrevTime = g_nowTime;
	
	if( g_Particle.m_fTimeD <= 0.0f || g_Particle.m_fTimeD > 5.0f )
		g_Particle.m_fTimeD = 0.002f;

	ProfileBegin( "< CBravoMap::Display()PC >" );
	//...........................................................................................................
	// 텍스처 에니 관련 
	//...........................................................................................................
	ProfileBegin( "TEXTURE_ANI" );
	if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( timeGetTime() , m_OldWaterTimer ) , >= , WATER_TRANS_TIMER ) )
	{
		m_fWaterRange   = 0.0f;
		m_OldWaterTimer = timeGetTime();
	
		D3DXMatrixIdentity( &m_matWaterTrans );
	}
	else
	{
		m_fWaterRange   = ( float ) SAFE_TIME_SUB( timeGetTime() , m_OldWaterTimer ) / ( float ) WATER_TRANS_TIMER;

		D3DXMatrixIdentity( &m_matWaterTrans );

		m_matWaterTrans._32 = -m_fWaterRange;
	}
	ProfileEnd( "TEXTURE_ANI" );
	//...........................................................................................................
	g_RockClient.m_SquenceManager.FirstSceneUpdate();


	ProfileBegin( "Set renderstate" );
	//......................................................................................................
	// 렌더링 설정
	//......................................................................................................
	g_RenderManager.SetCullMode( D3DCULL_CCW );
	g_RenderManager.SetFillMode( D3DFILL_SOLID );
	g_RenderManager.SetFogEnable( FALSE );
	g_RenderManager.SetLighting( FALSE );
	g_RenderManager.SetZFunc( D3DCMP_LESSEQUAL );
	g_RenderManager.SetAlphaTestEnable( TRUE );
	g_RenderManager.SetAlphaBlendEnable( TRUE );
	g_RenderManager.SetAlphaFunc( D3DCMP_NOTEQUAL );
	
	g_RenderManager.SetTextureMagFilter( 0 , D3DTEXF_LINEAR );
	g_RenderManager.SetTextureMinFilter( 0 , D3DTEXF_LINEAR );
	g_RenderManager.SetTextureMipFilter( 0 , D3DTEXF_LINEAR );

	g_RenderManager.SetTextureMagFilter( 1 , D3DTEXF_LINEAR );
	g_RenderManager.SetTextureMinFilter( 1 , D3DTEXF_LINEAR );
	g_RenderManager.SetTextureMipFilter( 1 , D3DTEXF_LINEAR );

	g_RenderManager.SetTextureTransFormFlags( 0 , D3DTTFF_DISABLE );
	g_RenderManager.SetTextureTransFormFlags( 1 , D3DTTFF_DISABLE );

	g_RenderManager.SetTextureColorOP( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureColorArg1( 0 , D3DTA_TEXTURE );
	g_RenderManager.SetTextureColorArg2( 0 , D3DTA_DIFFUSE );

	//-----------------------------------------------------------------------------
	// 바닥타일, 스카이박스색 변경
	//-----------------------------------------------------------------------------	
//	UpdateTileColor();	

	//........................................................................................................
	// 스카이박스
	//........................................................................................................
	
	g_RenderManager.SetFogEnable( FALSE );

	ProfileEnd( "Set renderstate" );

	ProfileBegin( "SetEnvData" );

	LPENV_DATA SetEnvData = m_SetEnv.GetSet();	
	if( SetEnvData )	
	{
		if ( m_bShowSky && m_byMapType == MS_FIELD )	
		{
			D3DXMATRIX matProj;

#ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/23)			
			
			D3DXMatrixPerspectiveFovLH( &matProj, g_Camera.m_fFOV, g_Camera.m_fAspect, 1.0f, 5.0f );
			
#else
			
			D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI/4.25f, 1.333f, 1.0f, 5.0f );
			
#endif// #ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/23)

			g_RenderManager.SetTransform(D3DTS_PROJECTION, &matProj );
			
			m_SkyBox.SetPos( g_Camera.m_vEye.x , g_Camera.m_vEye.y , g_Camera.m_vEye.z );
			
#ifndef BOUND_BOX_DISPLAY
		
			if( g_RockClient.m_SceneManager.GetSkyBoxDisplay() )
				m_SkyBox.Display( m_pDevice );
#endif
		}

		g_Camera.ReloadProjTM();

		D3DXMATRIX matWorld;

		D3DXMatrixIdentity( &matWorld );

		matWorld._11 = matWorld._33 = SetEnvData->CloudSize;
		matWorld._42 = SetEnvData->CloudHeight;

		m_Cloud.SetMatrix( matWorld );

#ifndef BOUND_BOX_DISPLAY
		if( g_RockClient.m_SceneManager.GetSkyBoxDisplay() )
			m_Cloud.Display( m_pDevice );
#endif
	}
	

#ifdef BOUND_BOX_DISPLAY

	if( g_Camera.m_ISBoundCamera )	
	{
		D3DXMATRIX  matView;
		
		// D3DXVECTOR3 vLookat( g_Pc.GetPlayer()->GetPosTM()._41 , g_Pc.GetPlayer()->GetPosTM()._42 , g_Pc.GetPlayer()->GetPosTM()._43  );
		D3DXVECTOR3 vLookat = g_Camera.m_vEye;
		D3DXVECTOR3 vEye;
		D3DXVECTOR3 vUp( 0.0f , 0.0f , 1.0f );
		
		vEye = vLookat;
		vEye.y += g_RockClient.m_TopCameraHeight;
		
		D3DXMatrixLookAtLH( &matView , &vEye , &vLookat, &vUp );
		
		g_RenderManager.SetTransform( D3DTS_VIEW, &matView );
	}

#endif

	g_RenderManager.DisableTextureStageState( 1 );
	g_RenderManager.DisableTextureStageState( 2 );
	
	ProfileEnd( "SetEnvData" );
	//........................................................................................................
	ProfileBegin( "D3DRS_FOGCOLOR" );
    D3DXMATRIX matWorld;
    D3DXMatrixIdentity( &matWorld );
    g_RenderManager.SetTransform( D3DTS_WORLD, &matWorld );

	// 렌더링 설정
	g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
	g_RenderManager.SetDestBlend( D3DBLEND_INVSRCALPHA );
	g_RenderManager.SetZWriteEnable( TRUE );
	g_RenderManager.SetZEnable( TRUE );

	//-----------------------------------------------------------------------------
	// 포그 설정 ( 맵별로 설정 다르게 ) By wxywxy
	//-----------------------------------------------------------------------------		
	if( SetEnvData )	
	{
		g_Pc_Manager.theFog_Color.x = SetEnvData->FogR;
		g_Pc_Manager.theFog_Color.y = SetEnvData->FogG;
		g_Pc_Manager.theFog_Color.z = SetEnvData->FogB;
	
		g_Pc_Manager.theFog_Start   = SetEnvData->FogStart;
		
		if( g_ViewLODTerrain )
			g_Pc_Manager.theFog_End = SetEnvData->FogEnd;
		else
		{
			g_Pc_Manager.theFog_End = ( ( g_LODRange + 1.2f ) * PATCH_SIZE ) * VERTEX_INTERVAL;

//			g_Pc_Manager.theFog_End = SetEnvData->FogEnd;
/*
			if( SetEnvData->FogEnd < g_Pc_Manager.theFog_End )
			{
				g_Pc_Manager.theFog_End = SetEnvData->FogEnd;
			}
*/
		}

		if( g_Pc_Manager.theFog_Start > g_Pc_Manager.theFog_End )
		{
			float Temp = g_Pc_Manager.theFog_Start;

			g_Pc_Manager.theFog_Start = g_Pc_Manager.theFog_End;
			g_Pc_Manager.theFog_End   = Temp;
		}

		g_RenderManager.SetFogEnable( FALSE );
		g_RenderManager.SetFogColor( D3DCOLOR_XRGB( (int) g_Particle.m_WeatherDrop.m_Fog_Color.x, 
												    (int) g_Particle.m_WeatherDrop.m_Fog_Color.y, 
													(int) g_Particle.m_WeatherDrop.m_Fog_Color.z )
 								   );

		g_RenderManager.SetFogStart( g_Pc_Manager.theFog_Start );
		g_RenderManager.SetFogEnd( g_Pc_Manager.theFog_End );
		g_RenderManager.SetFogTableMode( D3DFOG_NONE );
		g_RenderManager.SetFogVertexMode( D3DFOG_LINEAR );
		g_RenderManager.SetFogRangeEnable( TRUE );
		g_RenderManager.SetFogDensity( SetEnvData->FogDensity );
	}

	if( theWireframe )
		g_RenderManager.SetFillMode( D3DFILL_WIREFRAME );
	else
		g_RenderManager.SetFillMode( D3DFILL_SOLID );

	ProfileEnd( "D3DRS_FOGCOLOR" );
	/// sooree terrain ------------------------------------------------------------->

	//........................................................................................................
	// 맵별로 머터리얼 라이트 바꾸기 By wxywxy
	//........................................................................................................
	ProfileBegin( "SetMaterial" );
	if( SetEnvData )
	{
		g_Pc_Manager.theTerrain_Material.Ambient  = SetEnvData->LandMaterial.Ambient;
		g_Pc_Manager.theTerrain_Material.Diffuse  = SetEnvData->LandMaterial.Diffuse;
		g_Pc_Manager.theTerrain_Material.Emissive = SetEnvData->LandMaterial.Emissive;
		g_Pc_Manager.theTerrain_Material.Specular = SetEnvData->LandMaterial.Specular;
		g_Pc_Manager.theTerrain_Material.Power    = SetEnvData->LandMaterial.Power;

		
		if( g_ISObjTexLoad )
		{
			g_Pc_Manager.theTerrain_Light.Ambient     = SetEnvData->LandLight.Ambient;
			g_Pc_Manager.theTerrain_Light.Diffuse     = SetEnvData->LandLight.Diffuse;
			g_Pc_Manager.theTerrain_Light.Specular    = SetEnvData->LandLight.Specular;
		}
		
	}
	
	g_RenderManager.SetMaterial( &g_Pc_Manager.theTerrain_Material );	
	g_RenderManager.SetLight( 0, &g_Particle.m_WeatherDrop.m_Terrain_Light );


	g_RenderManager.LightEnable( 0, TRUE );
	g_RenderManager.LightEnable( 1, FALSE );

	g_RenderManager.SetLighting( TRUE );

	g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
	g_RenderManager.SetDestBlend( D3DBLEND_INVSRCALPHA );

	g_RenderManager.SetZWriteEnable( TRUE );
	g_RenderManager.SetZEnable( TRUE );
	
	g_RenderManager.SetAlphaRef( 0x00000000 );
	
	g_RenderManager.SetAlphaFunc( D3DCMP_NOTEQUAL );

	g_RenderManager.SetCullMode( D3DCULL_CCW );

	g_RenderManager.SetTextureMipFilter( 0,  D3DTEXF_LINEAR );
	g_RenderManager.SetTextureMagFilter( 0,  D3DTEXF_LINEAR );
	g_RenderManager.SetTextureMinFilter( 0,  D3DTEXF_POINT  );

	g_RenderManager.SetTextureMipFilter( 1,  D3DTEXF_LINEAR );
	g_RenderManager.SetTextureMagFilter( 1,  D3DTEXF_LINEAR );
	g_RenderManager.SetTextureMinFilter( 1,  D3DTEXF_LINEAR );

	g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureColorArg1( 0 , D3DTA_TEXTURE   );
	g_RenderManager.SetTextureColorArg2( 0 , D3DTA_DIFFUSE   );

	g_RenderManager.SetTextureColorOP  ( 1 , D3DTOP_MODULATE2X );
	g_RenderManager.SetTextureColorArg1( 1 , D3DTA_TEXTURE     );
	g_RenderManager.SetTextureColorArg2( 1 , D3DTA_CURRENT     );
	
	g_RenderManager.SetTextureAlphaOP  ( 0 , D3DTOP_MODULATE   );
	g_RenderManager.SetTextureAlphaArg1( 0 , D3DTA_DIFFUSE     );
	g_RenderManager.SetTextureAlphaArg2( 0 , D3DTA_CURRENT     );

	g_RenderManager.SetTextureAddressU( 1 , D3DTADDRESS_WRAP );
	g_RenderManager.SetTextureAddressV( 1 , D3DTADDRESS_WRAP );
	g_RenderManager.SetTextureAddressW( 1 , D3DTADDRESS_WRAP );
	
	if( g_RockClient.m_SceneManager.GetFogDisplay() )
		g_RenderManager.SetFogEnable( TRUE );
	else
		g_RenderManager.SetFogEnable( FALSE );

	ProfileEnd( "SetMaterial" );

	/// sooree ter opt
	if( g_ViewLODTerrain ) //m_cSize
	{
		ProfileBegin( "g_ViewLODTerrain" );
		g_RenderManager.SetTextureAddressU( 0 , D3DTADDRESS_WRAP );
		g_RenderManager.SetTextureAddressV( 0 , D3DTADDRESS_WRAP );
		
		if( g_RockClient.m_SceneManager.GetTerrainDisplay() )
			theTerrain.Render(); 

		ProfileEnd( "g_ViewLODTerrain" );
	}
	else
	{
		ProfileBegin( "D3DXMATRIX matProj" );
		D3DXMATRIX matProj; 
		D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI/4, 1.333f, 1.0f, g_Pc_Manager.theFog_End + 40.0f );
		g_RenderManager.SetTransform(D3DTS_PROJECTION, &matProj);
		ProfileEnd( "D3DXMATRIX matProj" );
	}

	// 타일 경계선 안보이기 
	g_RenderManager.SetTextureAddressU( 0 , D3DTADDRESS_CLAMP );	
	g_RenderManager.SetTextureAddressV( 0 , D3DTADDRESS_CLAMP );
	
	
	ProfileBegin( "theTerrain.RenderTileMap()" );
	
	if( g_RockClient.m_SceneManager.GetTerrainDisplay() )
		theTerrain.RenderTileMap();
	
	ProfileEnd( "theTerrain.RenderTileMap()" );
	

	/// 원상 복귀 
	g_RenderManager.SetTextureAddressU( 0 , D3DTADDRESS_WRAP );	
	g_RenderManager.SetTextureAddressV( 0 , D3DTADDRESS_WRAP );

	g_RenderManager.SetTextureCoordIndex( 0 , 0 );
	g_RenderManager.SetTextureCoordIndex( 1 , 1 );

	g_RenderManager.SetTextureTransFormFlags( 0 , D3DTTFF_DISABLE );
	g_RenderManager.SetTextureTransFormFlags( 1 , D3DTTFF_DISABLE );

	g_RenderManager.DisableTexture( 0 );
	g_RenderManager.DisableTexture( 1 );

	g_RenderManager.SetTextureMinFilter( 0 , D3DTEXF_LINEAR );
	///<----------------------------------------------------------- sooree terrain

	//-----------------------------------------------------------------------------
	// 광원타일, 그림자( 여기서는 포그를 꺼야한다 ), 라이트설정
	//-----------------------------------------------------------------------------

	ProfileBegin( "UpdateLightTile();" );
	
	if( g_RockClient.m_SceneManager.GetTerrainDisplay() )
	{	
		g_RenderManager.SetFogEnable( FALSE );
		UpdateLightTile();
	}

	ProfileEnd( "UpdateLightTile();" );
	
	//-----------------------------------------------------------------------------
	// 타일속성	
	//-----------------------------------------------------------------------------
#ifdef BLOCK_DEBUG
	displayonter();			
#endif
	g_RenderManager.SetFogEnable( FALSE );

	//-----------------------------------------------------------------------------
	// 아이템
	//-----------------------------------------------------------------------------	

	//-----------------------------------------------------------------------------
	// 캐릭터
	//-----------------------------------------------------------------------------	
	///g_Pc.Display();	오브젝트 Display 뒤로.... by soohyun
	
	
	//-----------------------------------------------------------------------------
	// 파티클, 오브젝트 ( 지형 이펙트 옵션 )
	//-----------------------------------------------------------------------------	
	ProfileBegin( "g_Map.EffectDisplayUpdate()" );
	if( g_ISLandEffect )	
		g_Map.EffectDisplayUpdate();
	ProfileEnd( "g_Map.EffectDisplayUpdate()" );

//	m_Light.Diffuse.r	= 0.75f;
//	m_Light.Diffuse.g	= 0.75f;
//	m_Light.Diffuse.b	= 0.75f;
//	m_Light.Ambient.r	= 0.65f;
//	m_Light.Ambient.g	= 0.75f;
//	m_Light.Ambient.b	= 0.70f;


//...........................................................................................................
// 이전 루틴 By wxywxy
//...........................................................................................................
/*
	ProfileBegin( "DisplayObj();" );
	DisplayObj();
	ProfileEnd( "DisplayObj();" );
	
	//-----------------------------------------------------------------------------
	// 캐릭터
	//-----------------------------------------------------------------------------	
//	m_Light.Diffuse.r	= 0.75f;
//	m_Light.Diffuse.g	= 0.75f;
//	m_Light.Diffuse.b	= 0.75f;
//	m_Light.Ambient.r	= 0.70f;
//	m_Light.Ambient.g	= 0.75f;
//	m_Light.Ambient.b	= 0.70f;	


	ProfileBegin( "g_Pc.Display();" );
	g_Pc.Display();
	ProfileEnd( "g_Pc.Display();" );
	
*/

	g_RenderManager.SetLight( 0, &m_Light );
	g_RenderManager.SetMaterial( &m_Material );					
	
	ProfileBegin( "DisplayGrass();" );
	
	if( g_RockClient.m_SceneManager.GetTerrainDisplay() )
	{
		if( g_ISObjTexLoad )
			DisplayGrass();
	}

	ProfileEnd( "DisplayGrass();" );	

	//........................................................................................................
	// 맵별로 머터리얼 라이트 바꾸기 By wxywxy
	//........................................................................................................
	if( SetEnvData )
	{
		g_Pc_Manager.theObject_Material.Ambient  = SetEnvData->ObjectMaterial.Ambient;
		g_Pc_Manager.theObject_Material.Diffuse  = SetEnvData->ObjectMaterial.Diffuse;
		g_Pc_Manager.theObject_Material.Emissive = SetEnvData->ObjectMaterial.Emissive;
		g_Pc_Manager.theObject_Material.Specular = SetEnvData->ObjectMaterial.Specular;
		g_Pc_Manager.theObject_Material.Power    = SetEnvData->ObjectMaterial.Power;

		if( !g_ISObjTexLoad )
		{
			g_Pc_Manager.theObject_Light.Ambient.r     = SetEnvData->FogR / 255.0f * 0.3f;
			g_Pc_Manager.theObject_Light.Ambient.g     = SetEnvData->FogG / 255.0f * 0.3f;
			g_Pc_Manager.theObject_Light.Ambient.b     = SetEnvData->FogB / 255.0f * 0.3f;
			g_Pc_Manager.theObject_Light.Ambient.a     = 1.0f;
			
			g_Pc_Manager.theObject_Light.Diffuse       = g_Pc_Manager.theObject_Light.Ambient;
			g_Pc_Manager.theObject_Light.Specular      = SetEnvData->ObjectLight.Specular;
		}
		else
		{
			g_Pc_Manager.theObject_Light.Ambient     = SetEnvData->ObjectLight.Ambient;
			g_Pc_Manager.theObject_Light.Diffuse     = SetEnvData->ObjectLight.Diffuse;
			g_Pc_Manager.theObject_Light.Specular    = SetEnvData->ObjectLight.Specular;
		}
		
		g_RenderManager.SetMaterial( &g_Pc_Manager.theTerrain_Material );	
		g_RenderManager.SetLight( 0, &g_Pc_Manager.theTerrain_Light );	
		
		g_RenderManager.SetLighting( TRUE );
	}
	
	g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureColorArg1( 0 , D3DTA_TEXTURE   );
	g_RenderManager.SetTextureColorArg2( 0 , D3DTA_CURRENT    );

	g_RenderManager.SetTextureAlphaOP  ( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureAlphaArg1( 0 , D3DTA_TEXTURE   );
	g_RenderManager.SetTextureAlphaArg2( 0 , D3DTA_CURRENT   );

	if( g_RockClient.m_SceneManager.GetFogDisplay() )
		g_RenderManager.SetFogEnable( TRUE );
	else
		g_RenderManager.SetFogEnable( FALSE );

	g_RenderManager.SetAlphaTestEnable( FALSE );

	//.......................................................................................................
	// 맵 오브젝트 NPC 소팅후 출력 루틴 By wxywxy
	//.......................................................................................................
	ProfileBegin( "DisplaySortNode()" );
	DisplaySortNode();
	ProfileEnd( "DisplaySortNode()" );
	//.......................................................................................................
	g_RenderManager.SetFogEnable( FALSE );
	
	
	ProfileBegin( "SetRenderState_g_particle" );
	g_RenderManager.SetMaterial( &g_Pc_Manager.theTerrain_Material );	
	g_RenderManager.SetLight( 0, &g_Particle.m_WeatherDrop.m_Terrain_Light );	

	g_RenderManager.LightEnable( 0, TRUE );
	g_RenderManager.LightEnable( 1, FALSE );
	
	g_RenderManager.SetLighting( TRUE );

	g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
	g_RenderManager.SetDestBlend( D3DBLEND_INVSRCALPHA );
	g_RenderManager.SetZWriteEnable( TRUE );
	g_RenderManager.SetZEnable( TRUE );

	g_RenderManager.SetAlphaRef( 0x00000000 );
	g_RenderManager.SetAlphaFunc( D3DCMP_NOTEQUAL );

	g_RenderManager.SetTextureMipFilter( 0 , D3DTEXF_LINEAR );
	g_RenderManager.SetTextureMagFilter( 0 , D3DTEXF_LINEAR );
	g_RenderManager.SetTextureMinFilter( 0 , D3DTEXF_LINEAR ); 

	g_RenderManager.SetTextureMipFilter( 1 , D3DTEXF_LINEAR );
	g_RenderManager.SetTextureMagFilter( 1 , D3DTEXF_LINEAR );
	g_RenderManager.SetTextureMinFilter( 1 , D3DTEXF_LINEAR ); 

	g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureColorArg1( 0 , D3DTA_TEXTURE );
	g_RenderManager.SetTextureColorArg2( 0 , D3DTA_DIFFUSE );
	
	g_RenderManager.SetTextureAlphaOP  ( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureAlphaArg1( 0 , D3DTA_DIFFUSE   );
	g_RenderManager.SetTextureAlphaArg2( 0 , D3DTA_CURRENT   );

	g_RenderManager.SetTextureColorOP  ( 1 , D3DTOP_MODULATE2X );
	g_RenderManager.SetTextureColorArg1( 1 , D3DTA_TEXTURE );
	g_RenderManager.SetTextureColorArg2( 1 , D3DTA_CURRENT );

	/// 원상 복귀 
	g_RenderManager.SetTextureAddressU( 0 , D3DTADDRESS_WRAP );
	g_RenderManager.SetTextureAddressV( 0 , D3DTADDRESS_WRAP );

	g_RenderManager.SetTextureAddressU( 1 , D3DTADDRESS_WRAP );
	g_RenderManager.SetTextureAddressV( 1 , D3DTADDRESS_WRAP );
	g_RenderManager.SetTextureAddressW( 1 , D3DTADDRESS_WRAP );

	g_RenderManager.SetTextureCoordIndex( 0 , 0 );
	g_RenderManager.SetTextureTransFormFlags( 0 , D3DTTFF_DISABLE );

	g_RenderManager.SetTextureCoordIndex( 1 , 1 );
	g_RenderManager.SetTextureTransFormFlags( 1 , D3DTTFF_DISABLE );

	g_RenderManager.DisableTexture( 0 );
	g_RenderManager.DisableTexture( 1 );
	
	//.......................................................................................................	
	// 물 & 폭포
	//.......................................................................................................	
	g_RenderManager.SetFogEnable( FALSE );
	g_RenderManager.SetLighting( FALSE );
	g_RenderManager.SetAlphaBlendEnable( TRUE );

	g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_BLENDDIFFUSEALPHA );
	g_RenderManager.SetTextureColorArg1( 0 , D3DTA_TEXTURE );
	g_RenderManager.SetTextureColorArg2( 0 , D3DTA_DIFFUSE );
	
	g_RenderManager.SetTextureAlphaOP  ( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureAlphaArg1( 0 , D3DTA_TEXTURE );
	g_RenderManager.SetTextureAlphaArg2( 0 , D3DTA_DIFFUSE );
	
	ProfileBegin( "DisplayWaterfall()" );

	if( g_RockClient.m_SceneManager.GetTerrainDisplay() )
		DisplayWaterfall();
	
	ProfileEnd( "DisplayWaterfall()" );
	
	g_RenderManager.SetLight( 0, &g_Pc_Manager.theTerrain_Light );	

	g_RenderManager.SetLighting( FALSE );
	g_RenderManager.SetFogEnable( FALSE );

	if( g_RockClient.m_SceneManager.GetTerrainDisplay() )
		g_Particle.m_EffectManager.RenderToLandMark( g_Particle.m_fTimeD );
		
	// Water

	g_RenderManager.SetLighting( TRUE );
	g_RenderManager.SetFogEnable( FALSE );

	g_RenderManager.SetTextureTransFormFlags( 0 , D3DTTFF_DISABLE );
	//g_RenderManager.SetTextureTransFormFlags( 1 , D3DTTFF_DISABLE );

	g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureColorArg1( 0 , D3DTA_TEXTURE );
	g_RenderManager.SetTextureColorArg2( 0 , D3DTA_DIFFUSE );

	/// sooree water
	ProfileBegin( "theWaterList[i].Render" );
	if( theWaterList )
	{
		for( int i = 0; i < theWaterListNum; ++i )
		{
			theWaterList[i].Update();
		
			if( g_RockClient.m_SceneManager.GetTerrainDisplay() )
				theWaterList[i].Render( m_pDevice );
		}
	}	

	ProfileEnd( "theWaterList[i].Render" );
	//.......................................................................................................	

	ProfileEnd( "SetRenderState_g_particle" );	

	ProfileBegin( "g_Particle.Display()" );
	
	
	int l_nDevSkip = 0;

#ifdef _DEBUG 
	
	l_nDevSkip = GetPrivateProfileInt( "Game", "Effect_Render_Skip", 0, "./dev.ini");	

#endif

	if( !l_nDevSkip )
		g_Particle.Display();// 이펙트 그리기
	
	ProfileEnd( "g_Particle.Display()" );

	ProfileEnd( "< CBravoMap::Display()PC >" );	

	g_RenderManager.SetAlphaBlendEnable( FALSE );
	
	return	S_OK;
}



///-----------------------------------------------------------------------------------
/// pick by sooree
///-----------------------------------------------------------------------------------

float CBravoMap::PHF_GetHeight( const float aX, const float aZ )
{
	return( Get__MapHeightF( aX, aZ ) );	/// real-float 좌표로 지형 높이 구하기(속성 높이 포함) 
}

float CBravoMap::PHF_GetHeight( const int aX, const int aZ )
{
	return( Get__MapHeight( aX << 1, aZ << 1 ) );	/// 논리적 cell 좌표로 높이 구하기(속성 높이 포함) 
}

bool CBravoMap::PHF_IsValidPoint( const float aX, const float aZ )
{
	/// 뷰 프러스텀에 없는 폴리곤이 피킹되었을 때는 무시 
	if( g_Camera.PointInFrustum( aX, PHF_GetHeight( aX, aZ ), aZ ) )
	{
		return true;
	}
	return false;
}

bool CBravoMap::PHF_CellIntersectTriangle( int x, int z )
{
	float fBary1, fBary2;
	float fDist;
	float pointX, pointZ;

	{
		D3DXVECTOR3 v0( m_pDisplayMap[z][x].Pos[0].x, PHF_GetHeight( x, z ), m_pDisplayMap[z][x].Pos[0].z );
		D3DXVECTOR3 v1( m_pDisplayMap[z + 1][x].Pos[0].x, PHF_GetHeight( x, z + 1 ), m_pDisplayMap[z + 1][x].Pos[0].z );
		D3DXVECTOR3 v2( m_pDisplayMap[z + 1][x + 1].Pos[0].x, PHF_GetHeight( x + 1, z + 1 ), m_pDisplayMap[z + 1][x + 1].Pos[0].z );

		if( g_Camera.PointInFrustum( v0.x, v0.y, v0.z ) )
		{
			if( PHF_IntersectTriangle(	v0, v1, v2, &fDist, &fBary1, &fBary2 ) )
			{
				pointX = v0.x + fBary1 * ( v1.x - v0.x ) + fBary2 * ( v2.x - v0.x );
				pointZ = v0.z + fBary1 * ( v1.z - v0.z ) + fBary2 * ( v2.z - v0.z );

				if( PHF_IsValidPoint( pointX, pointZ ) )
				{
					PHF_SetPickedXZ( pointX, pointZ );
					return true;
				}
			}
		}
	}

	{
		D3DXVECTOR3 v0( m_pDisplayMap[z + 1][x + 1].Pos[0].x, PHF_GetHeight( x + 1, z + 1 ), m_pDisplayMap[z + 1][x + 1].Pos[0].z );
		D3DXVECTOR3 v1( m_pDisplayMap[z][x + 1].Pos[0].x, PHF_GetHeight( x + 1, z ), m_pDisplayMap[z][x + 1].Pos[0].z );
		D3DXVECTOR3 v2( m_pDisplayMap[z][x].Pos[0].x, PHF_GetHeight( x, z ), m_pDisplayMap[z][x].Pos[0].z );

		if( g_Camera.PointInFrustum( v0.x, v0.y, v0.z ) )
		{
			if( PHF_IntersectTriangle(	v0, v1, v2, &fDist, &fBary1, &fBary2 ) )
			{
				pointX = v0.x + fBary1 * ( v1.x - v0.x ) + fBary2 * ( v2.x - v0.x );
				pointZ = v0.z + fBary1 * ( v1.z - v0.z ) + fBary2 * ( v2.z - v0.z );

				if( PHF_IsValidPoint( pointX, pointZ ) )
				{
					PHF_SetPickedXZ( pointX, pointZ );
					return true;
				}
			}
		}
	}

	return false;
}

void CBravoMap::PHF_VertexComparePick(  float aCenterX, float aCenterZ, float aDistance )
{
	///-------------------------------------------------------------------
	/// aCenterX, aCenterZ으로 중심 cell과 distance cell 값을 구함 
	/// 중심 cell부터 distance를 하나씩 증가시켜 나가면서 가까운 폴리곤부터 IntersectTriangle()로 비교. 
	///-------------------------------------------------------------------
	int cx = (int)( aCenterX / 16.0f );
	int cz = (int)( aCenterZ / 16.0f );
	int dist = (int)( aDistance / 16.0f ) + 1; // 올림 

	int sx, ex; 
	int sz, ez;

	if( PHF_CellIntersectTriangle( cx, cz ) )	/// d == 0
	{
		return;
	}

	for( int d = 1; d < dist; ++d )
	{
		sx = cx - d;	if( sx < 0 ) sx = 0;
		ex = cx + d;	if( ex > m_cSize - 2 ) ex = m_cSize - 2;
		sz = cz - d;	if( sz < 0 ) sz = 0;
		ez = cz + d;	if( ez > m_cSize - 2 ) ez = m_cSize - 2;

		for( int x = sx; x <= ex; ++x )
		{
			if( PHF_CellIntersectTriangle( x, sz ) )
			{
				return;
			}

			if( PHF_CellIntersectTriangle( x, ez ) )
			{
				return;
			}
		}

		for( int z = sz + 1; z < ez; ++z )
		{
			if( PHF_CellIntersectTriangle( sx, z ) )
			{
				return;
			}

			if( PHF_CellIntersectTriangle( ex, z ) )
			{
				return;
			}
		}
	}
}
 
HRESULT	 CBravoMap::PHF_PickDest( HWND hWnd )
{
	float pickX;
	float pickZ;

	if ( PHF_Pick(	hWnd, &g_Camera.m_matProj, &g_Camera.m_matView, 
						g_RockClient.GetDisplayWidth(), g_RockClient.GetDisplayHeight(), 
						g_Pc.GetPlayer()->GetPosTM()._41, g_Pc.GetPlayer()->GetPosTM()._43, 
						&pickX, &pickZ, 1024.0f ) )
	{
		m_dwPickedX = (DWORD)( pickX / 8.0f );
		m_dwPickedY = (DWORD)( pickZ / 8.0f );
		
		m_RealPickedX = pickX;
		m_RealPickedY = pickZ;

		return S_OK;
	}
	
	return	E_FAIL;
}

///-----------------------------------------------------------------------------------



DWORD	CBravoMap::GetWarp1()
{
	for( DWORD y = 0; y < m_cSize * 2; y ++ )
	{
		for( DWORD x = 0; x < m_cSize * 2; x ++ )
		{
			//if ( m_pAttrMap[y][x].dwAttr == MATTR_WARP )
			if( m_pAttrMap[y][x].byAttr == MATTR_WARP )
			{
				return	(y*(m_cSize*2))+x;
			}
		}
	}

	return	0;
}


bool	CBravoMap::UpdateObj()
{
	if( g_RockClient.GetGameMode() != GMODE_NORMAL )	
		return false;
		
	dwObjVisibleCount	= 0;
		
	//D3DXMATRIX	matPos = g_Pc.GetPosTM(-1);
	D3DXMATRIX		matPos;
	D3DXMatrixIdentity(&matPos);
	matPos._41	= g_Camera.m_vEye.x;
	matPos._42	= g_Camera.m_vEye.y;
	matPos._43	= g_Camera.m_vEye.z;


	
	for( int i = 0; i < m_ObjCount; ++i )
	{
		if ( m_Object[i].m_bIsLoaded )
		{				
			for( int j = 0; j < m_Object[i].m_child_count; ++j )
			{							
/*
				D3DXVECTOR3		min	= m_Object[i].m_Bound.m_min_vertex;
				D3DXVECTOR3		max	= m_Object[i].m_Bound.m_max_vertex;
				D3DXMATRIX		mat;
				D3DXMatrixIdentity(&mat);

				mat._41	= m_Object[i].m_pAniPos[j].GetPosTM()._41;
				mat._42	= m_Object[i].m_pAniPos[j].GetPosTM()._42;
				mat._43	= m_Object[i].m_pAniPos[j].GetPosTM()._43;

				D3DXVec3TransformCoord(&min, &min, &mat);
				D3DXVec3TransformCoord(&max, &max, &mat);
				
				float	sizex	= (max.x - min.x);
				float	sizey	= (max.y - min.y);
				float	sizez	= (max.z - min.z);

				if ( true == (m_Object[i].m_pAniPos[j].m_bIsVisible = g_Camera.CubeInFrustum(min.x, min.y, min.z, sizex, sizey, sizez) ) )
					dwObjVisibleCount++;
*/
				D3DXVECTOR3 Min;
				D3DXVECTOR3 Max;
				
				Min = m_Object[i].m_pAniPos[j].m_vBoundList[ 0 ];
				Max = m_Object[i].m_pAniPos[j].m_vBoundList[ 0 ];

				for( int n = 1 ; n < 8 ; ++n )
				{
					if( Min.x > m_Object[i].m_pAniPos[j].m_vBoundList[ n ].x )
						Min.x = m_Object[i].m_pAniPos[j].m_vBoundList[ n ].x;

					if( Min.y > m_Object[i].m_pAniPos[j].m_vBoundList[ n ].y )
						Min.y = m_Object[i].m_pAniPos[j].m_vBoundList[ n ].y;

					if( Min.z > m_Object[i].m_pAniPos[j].m_vBoundList[ n ].z )
						Min.z = m_Object[i].m_pAniPos[j].m_vBoundList[ n ].z;

					if( Max.x < m_Object[i].m_pAniPos[j].m_vBoundList[ n ].x )
						Max.x = m_Object[i].m_pAniPos[j].m_vBoundList[ n ].x;

					if( Max.y < m_Object[i].m_pAniPos[j].m_vBoundList[ n ].y )
						Max.y = m_Object[i].m_pAniPos[j].m_vBoundList[ n ].y;

					if( Max.z < m_Object[i].m_pAniPos[j].m_vBoundList[ n ].z )
						Max.z = m_Object[i].m_pAniPos[j].m_vBoundList[ n ].z;
				}
				

				if ( true == (m_Object[i].m_pAniPos[j].m_bIsVisible = g_Camera.CubeInFrustum(
					m_Object[i].m_pAniPos[j].m_vMin.x, 
					m_Object[i].m_pAniPos[j].m_vMin.y, 
					m_Object[i].m_pAniPos[j].m_vMin.z, 
					m_Object[i].m_pAniPos[j].m_vSize.x ,
					m_Object[i].m_pAniPos[j].m_vSize.y ,
					m_Object[i].m_pAniPos[j].m_vSize.z  ) ) )
					dwObjVisibleCount++;


				// calculate distance 				
				//if( m_dwIdx == 1 )
				//{
//					const float	_x  = (matPos._41-mat._41)*(matPos._41-mat._41);
//					const float	_y 	= (matPos._43-mat._43)*(matPos._43-mat._43);			

//					if ( m_Object[i].m_pAniPos[j].m_fDistance > 512.0f )
//					{
//						m_Object[i].m_pAniPos[j].m_bIsVisible	= false;
//					}
//				//}
			}
		}		
	}
	return	true;
}
/*
void	CBravoMap::DisplayWater()
{
	for ( short i = 0; i < m_nWaterCount; i ++ )
	{
		if ( m_Water[i].m_bIsVisible )
			m_Water[i].Display();
	}
}
*/
/**********
	* STATIC NODE UPDATE WITH QUAD TREE
**********/
int tempcout = 0;

bool	CBravoMap::CreateObjNode()
{
	m_nDepth	= 5;//m_cSize>>5;

	// allocate
	{
		m_pNodeObj			= SAFE_NEW( node_obj );		
		m_pNodeObj->cx		= (m_cSize*TILE_SIZE)>>1;
		m_pNodeObj->cy		= (m_cSize*TILE_SIZE)>>1;
		m_pNodeObj->size	= (m_cSize*TILE_SIZE)>>1;
		m_pNodeObj->idxlist.init();
		m_pNodeObj->grslist.init();
	}
	
	RecursiveCreateNode(m_pNodeObj, 0);

	for( short i = 0; i < m_ObjCount; i++ )
	{
		for( short j = 0; j < m_Object[i].m_child_count; j ++ )
		{
			FindNode(i, j, m_pNodeObj, 0);
		}
	}

	return	true;
}

void	CBravoMap::CleanupObjNode()
{
	for( short i = 0; i < m_ObjCount; i++ )
	{
		for( short j = 0; j < m_Object[i].m_child_count; j ++ )
		{
			m_Object[i].m_pAniPos[j].cleanup();
		}
	}
	
	if( m_pNodeObj )
	{
		RecursiveCleanupNode(&m_pNodeObj, 0);
	}
}


void	CBravoMap::RecursiveCleanupNode(node_obj** node, short depth)
{
	if ( depth > m_nDepth+1 )
	{
		return;
	}

	node_obj *	tchild[4];

	for ( short i = 0; i < 4; i ++ )
	{		
		if ( (*node)->child[i] )
			tchild[i]	= (*node)->child[i];
	}

	_DELETE((*node)->idxlist.pheader);
	_DELETE((*node)->grslist.pheader);
	_DELETE(*node);

//	fprintf(fp, "\r\n%dth depth node cleanup", depth);

	for (short i = 0; i < 4; i ++ )
		RecursiveCleanupNode(&tchild[i], depth+1);

//	node_obj * temp[4];
//	for ( short i = 0; i < 4; i ++ )
//		temp[i]	= node->child[i];

//	node->idxlist.cleanup();
//	node->grslist.cleanup();
//	_DELETE(node);

}

bool	CBravoMap::RecursiveCreateNode(node_obj* node, short depth)
{
	if ( depth > m_nDepth )
		return	true;

	for ( short i = 0; i < 4; i ++ )
	{
		tempcout++;
		node->child[i]	= SAFE_NEW( node_obj );
		
		if(NULL == node->child[i])
		{
			return false;
		}		

		node->child[i]->idxlist.init();
		node->child[i]->grslist.init();
		if ( node->child[i] )
//			fprintf(fp, "\r\nnode created depth - %d\r\n", depth);
		switch ( i )
		{
		case	nodedir_bl:
			node->child[i]->size	= node->size >> 1;
			node->child[i]->cx		= node->cx - node->child[i]->size;
			node->child[i]->cy		= node->cy - node->child[i]->size;
			break;
		case	nodedir_br:
			node->child[i]->size	= node->size >> 1;
			node->child[i]->cx		= node->cx + node->child[i]->size;
			node->child[i]->cy		= node->cy - node->child[i]->size;
			break;
		case	nodedir_tl:
			node->child[i]->size	= node->size >> 1;
			node->child[i]->cx		= node->cx - node->child[i]->size;
			node->child[i]->cy		= node->cy + node->child[i]->size;
			break;
		case	nodedir_tr:
			node->child[i]->size	= node->size >> 1;
			node->child[i]->cx		= node->cx + node->child[i]->size;
			node->child[i]->cy		= node->cy + node->child[i]->size;
			break;
		}
		
		for ( short j = 0; j < 4; j ++ )
			node->child[i]->child[j] = NULL;

		RecursiveCreateNode(node->child[i], depth+1);
	}

	return	false;
}

BYTE	CBravoMap::ObjInTree(const short& idx, const short& sidx, const short& cx, const short& cy)
{
/*
	D3DXVECTOR3		min	= m_Object[idx].m_Bound.m_min_vertex;
	D3DXVECTOR3		max	= m_Object[idx].m_Bound.m_max_vertex;
	SetupMaxMin(&max, &min);
	D3DXMATRIX		mat;
	D3DXMatrixIdentity(&mat);

	mat._41	= m_Object[idx].m_pAniPos[sidx].GetPosTM()._41;
	mat._42	= m_Object[idx].m_pAniPos[sidx].GetPosTM()._42;
	mat._43	= m_Object[idx].m_pAniPos[sidx].GetPosTM()._43;

	D3DXVec3TransformCoord(&min, &min, &mat);
	D3DXVec3TransformCoord(&max, &max, &mat);


	short	minx = min.x;
	short	miny = min.z;
	short	maxx = max.x;
	short	maxy = max.z;

	if		(minx < cx && maxx < cx && miny > cy && maxy > cy) return 2;
	else if (minx > cx && maxx > cx && miny > cy && maxy > cy) return 3;
	else if (minx < cx && maxx < cx && miny < cy && maxy < cy) return 0;
	else if (minx > cx && maxx > cx && miny < cy && maxy < cy) return 1;
	else	return 4;

	return	nodedir_parent;
	*/

/*
	D3DXVECTOR3		min	= m_Object[idx].m_Bound.m_min_vertex;
	D3DXVECTOR3		max	= m_Object[idx].m_Bound.m_max_vertex;
	D3DXMATRIX		mat;
	D3DXMatrixIdentity(&mat);

	mat._41	= m_Object[idx].m_pAniPos[sidx].GetPosTM()._41;
	mat._42	= m_Object[idx].m_pAniPos[sidx].GetPosTM()._42;
	mat._43	= m_Object[idx].m_pAniPos[sidx].GetPosTM()._43;

//	D3DXVec3TransformCoord(&min, &min, &mat);
//	D3DXVec3TransformCoord(&max, &max, &mat);


//	short	minx = min.x;
//	short	miny = min.z;
//	short	maxx = max.x;
//	short	maxy = max.z;

	if		((min.x+mat._41) < cx && (max.x+mat._41) < cx && (min.z+mat._43) > cy && (max.z+mat._43) > cy) return 2;
	else if ((min.x+mat._41) > cx && (max.x+mat._41) > cx && (min.z+mat._43) > cy && (max.z+mat._43) > cy) return 3;
	else if ((min.x+mat._41) < cx && (max.x+mat._41) < cx && (min.z+mat._43) < cy && (max.z+mat._43) < cy) return 0;
	else if ((min.x+mat._41) > cx && (max.x+mat._41) > cx && (min.z+mat._43) < cy && (max.z+mat._43) < cy) return 1;
	else	return 4;
*/
	//.......................................................................................................
	// By wxywxy
	//.......................................................................................................
	D3DXVECTOR3 vMin	= m_Object[idx].GetBoundMinPos();
	D3DXVECTOR3 vMax	= m_Object[idx].GetBoundMaxPos();
	
	D3DXVec3TransformCoord(&vMin, &vMin, &m_Object[idx].m_pAniPos[sidx].GetPosTM());
	D3DXVec3TransformCoord(&vMax, &vMax, &m_Object[idx].m_pAniPos[sidx].GetPosTM());

	if( vMin.x > vMax.x )
	{
		float Temp = vMin.x;

		vMin.x = vMax.x;
		vMax.x = Temp;
	}

/*
	if( vMin.y > vMax.y )
	{
		float Temp = vMin.y;

		vMin.y = vMax.y;
		vMax.y = Temp;
	}
*/

	if( vMin.z > vMax.z )
	{
		float Temp = vMin.z;

		vMin.z = vMax.z;
		vMax.z = Temp;
	}

	if		((vMin.x) < cx && (vMax.x) < cx && (vMin.z) > cy && (vMax.z) > cy) return 2;
	else if ((vMin.x) > cx && (vMax.x) > cx && (vMin.z) > cy && (vMax.z) > cy) return 3;
	else if ((vMin.x) < cx && (vMax.x) < cx && (vMin.z) < cy && (vMax.z) < cy) return 0;
	else if ((vMin.x) > cx && (vMax.x) > cx && (vMin.z) < cy && (vMax.z) < cy) return 1;
	else	return 4;

	return	nodedir_parent;
}

void	CBravoMap::FindNode(const short& idx, const short& sidx, node_obj* node, short depth)
{
	if ( depth > m_nDepth )
	{
		///-- 쿼드트리 깊이값이 넘어가면 해당 노드에 오브젝트가 리스트로 등록된다.
		m_Object[idx].m_pAniPos[sidx].nidx	= idx;
		m_Object[idx].m_pAniPos[sidx].nsidx	= sidx;
		node->idxlist.addtail((node_idx*)&m_Object[idx].m_pAniPos[sidx]);
		return;
	}

	int result = ObjInTree(idx, sidx, node->cx, node->cy);
	
	if (result == 4)
	{
		///-- 결과값이 4일때 해당 노드에 오브젝트가 리스트로 등록된다.
		m_Object[idx].m_pAniPos[sidx].nidx	= idx;
		m_Object[idx].m_pAniPos[sidx].nsidx	= sidx;
		node->idxlist.addtail((node_idx*)&m_Object[idx].m_pAniPos[sidx]);
		
		return;
	}
	else	///--지정된 트리 자식노드에서 검색루틴을 실행한다.
	{
		FindNode(idx, sidx, node->child[result], depth+1);
	}
}


int		g_nobjupdated	= 0;
int		g_grsupdated	= 0;

int	g_node_count = 0;

bool	CBravoMap::UpdateObj2()
{
	dwGrassVisibleCount	= 0;
	dwObjVisibleCount	= 0;
	g_nobjupdated		= 0;
	g_grsupdated		= 0;

	D3DXMATRIX	matPos = g_Pc.GetPosTM(-1);

	for ( short i = 0; i < m_ObjCount; i ++ )
	{
		for ( short j = 0; j < m_Object[i].m_child_count; j ++ )
		{
			m_Object[i].m_pAniPos[j].m_bIsVisible = false;
		}
	}

	g_node_count++;
	RecursiveNodeVisible( m_pNodeObj, 0 );
	
	return	true;
}


void	CBravoMap::RecursiveNodeVisible(node_obj* node, short depth)
{
	if ( node == NULL || depth > m_nDepth+1 )
	{
		return;
	}	
	
	D3DXMATRIX	matPos = g_Pc.GetPosTM(-1);

	///-- By simwoosung
	float fx = node->cx - node->size;
	float fy = node->cy - node->size; 
	
	if ( g_Camera.NodeInFrustum( fx, 0, fy, node->size * 2, 5000, node->size * 2 ) == true )	
	{
		g_node_count++; ///test
		
		if( NULL == node->idxlist.pheader )
			return;

		///-- 현재 노드내 등록된 오브젝트 들을 탐색한다.
		node_idx *	plink	= node->idxlist.pheader->pnext;
		
		while( plink )
		{
			g_nobjupdated++;
			CheckObjBlending(plink->nidx, plink->nsidx);
			plink	= plink->pnext;
		}

		for ( short i = 0; i < 4; i ++ )
		{
			RecursiveNodeVisible( node->child[i], depth + 1 );
		}
	}
}


short	CBravoMap::PickNpc()
{
	if ( m_pDisplayMap == NULL || m_pDevice == NULL )
		return	-1;

	if( !g_Pc.GetPlayer()->GetAddEffectState(n_TypeProvoke) )
	{
		m_nPickedNpc	= -1;
	}	

	POINT	ptCursor;
	GetCursorPos(&ptCursor);
	::ScreenToClient(g_RockClient.GetApp(), &ptCursor);
//	ptCursor.x	+= 2;
//	ptCursor.y	+= 10;

	D3DXVECTOR3	vPickRayDir;
	D3DXVECTOR3	vPickRayOrig;

	D3DXMATRIX		matProj;
	matProj	= g_Camera.m_matProj;
	
	GetCursorPos(&ptCursor);
//	ptCursor.x	+= 2;
//	ptCursor.y	+= 10;
	::ScreenToClient(g_RockClient.GetApp(), &ptCursor);

	D3DXVECTOR3	v;
	v.x	= (((2.0f*ptCursor.x)/g_RockClient.GetDisplayWidth())-1) / matProj._11;
	v.y	= -(((2.0f*ptCursor.y)/g_RockClient.GetDisplayHeight())-1) / matProj._22;
	v.z	= 1.0f;
	
	D3DXMATRIX		matView, m;
	matView	= g_Camera.m_matView;
	D3DXMatrixInverse(&m, NULL, &matView);

	vPickRayDir.x  = v.x*m._11 + v.y*m._21 + v.z*m._31;
    vPickRayDir.y  = v.x*m._12 + v.y*m._22 + v.z*m._32;
    vPickRayDir.z  = v.x*m._13 + v.y*m._23 + v.z*m._33;
    vPickRayOrig.x = m._41;
    vPickRayOrig.y = m._42;
    vPickRayOrig.z = m._43;

	FLOAT	fBary1, fBary2;
	FLOAT	fDist;
	
	D3DXVECTOR3		v1[3];	// 1st face
	D3DXVECTOR3		v2[3];	// 2nd face
	D3DXMATRIX		matpos;
	D3DXMATRIX		matrot;
	D3DXVECTOR3		min, max;

	int nIndex		= -1;

	m_SortTable.StartNPCPointer();

	while( ( nIndex = m_SortTable.GetNPC() ) != -1 )
	{
		if( m_NpcTable[nIndex].lUnique == -1 )
			continue;
		if( m_Npc[nIndex].m_bIsLoaded == false || m_Npc[nIndex].m_bIsVisible == false )
			continue;
		if( m_Npc[nIndex].m_curt_event.type == SM_STATE_DIE )
			continue;
		if( m_Npc[nIndex].m_State.nRideState == nRideState_Ride )
			continue;

		D3DXMatrixIdentity( &matpos );		
		D3DXMatrixIdentity( &matrot );		

		matpos._41	= m_Npc[nIndex].GetPosTM()._41;
		matpos._42	= m_Npc[nIndex].GetPosTM()._42;
		matpos._43	= m_Npc[nIndex].GetPosTM()._43;

		//D3DXVECTOR3		min	= m_Npc[nIndex].m_Bound.m_min_vertex;
		//D3DXVECTOR3		max	= m_Npc[nIndex].m_Bound.m_max_vertex;

		D3DXVECTOR3 min;
		D3DXVECTOR3 max;

		//by dongs 몬스터 픽이 잘안돼서 ...  UI 픽인덱스로 비교하여 리턴 함 .. 
		if(  nRui->thePickPcIdx != -1 && 
			(g_Map.m_NpcTable[nIndex].lUnique == nRui->thePcInfo[nRui->thePickPcIdx].id ))
		{
			// 부가효과 (강제타겟) 처리 
			if( g_Pc.GetPlayer()->GetAddEffectState(n_TypeProvoke) )
			{
				if( m_nPickedNpc == NonPlayerInfoWnd.thePickedNpcID )
				{
					return m_nPickedNpc;
				}
				else
				{
					return -1;
				}
			}			
			
			if( GetNpcAlive(nIndex) == false )
			{
				return -1;
			}

			m_nPickedNpc = nIndex;	

			return nIndex; 
		}
		else
		{
			continue;
		}	
		
		if( g_Map.m_Npc[nIndex].m_State.nCharacterType != nCharacterTypePlayerNPC )
		{
			min	= g_Pc_Manager.GetBoundMinPos( &m_Npc[nIndex] );
			max	= g_Pc_Manager.GetBoundMaxPos( &m_Npc[nIndex] );
		}
		else
		{
			min	= g_Pc_Manager.GetMinBoundVec( &m_Npc[nIndex] );
			max	= g_Pc_Manager.GetMaxBoundVec( &m_Npc[nIndex] );
		}

		for( int j = 0; j < 6; j ++ )
		{
			switch( j )
			{
			// front side
			case 0:				
				v1[0].x	= min.x;
				v1[0].y	= min.y;
				v1[0].z	= min.z;
				v1[1].x	= min.x;
				v1[1].y	= max.y;
				v1[1].z	= min.z;
				v1[2].x	= max.x;
				v1[2].y	= max.y;
				v1[2].z	= min.z;

				v2[0].x	= max.x;
				v2[0].y	= max.y;
				v2[0].z	= min.z;
				v2[1].x	= max.x;
				v2[1].y	= min.y;
				v2[1].z	= min.z;
				v2[2].x	= min.x;
				v2[2].y	= min.y;
				v2[2].z	= min.z;
				break; 
			// right side
			case 1:		
				v1[0].x	= max.x;
				v1[0].y	= min.y;
				v1[0].z	= min.z;
				v1[1].x	= max.x;
				v1[1].y	= max.y;
				v1[1].z	= min.z;
				v1[2].x	= max.x;
				v1[2].y	= max.y;
				v1[2].z	= max.z;

				v2[0].x	= max.x;
				v2[0].y	= max.y;
				v2[0].z	= max.z;
				v2[1].x	= max.x;
				v2[1].y	= min.y;
				v2[1].z	= max.z;
				v2[2].x	= max.x;
				v2[2].y	= min.y;
				v2[2].z	= min.z;
				break;
			// back side
			case 2:		
				v1[0].x	= max.x;
				v1[0].y	= min.y;
				v1[0].z	= max.z;
				v1[1].x	= min.x;
				v1[1].y	= max.y;
				v1[1].z	= max.z;
				v1[2].x	= max.x;
				v1[2].y	= max.y;
				v1[2].z	= max.z;

				v2[0].x	= min.x;
				v2[0].y	= max.y;
				v2[0].z	= max.z;
				v2[1].x	= max.x;
				v2[1].y	= min.y;
				v2[1].z	= max.z;
				v2[2].x	= min.x;
				v2[2].y	= min.y;
				v2[2].z	= max.z;
				break;
			// left side
			case 3:		
				v1[0].x	= min.x;
				v1[0].y	= min.y;
				v1[0].z	= max.z;
				v1[1].x	= min.x;
				v1[1].y	= max.y;
				v1[1].z	= min.z;
				v1[2].x	= min.x;
				v1[2].y	= max.y;
				v1[2].z	= max.z;

				v2[0].x	= min.x;
				v2[0].y	= max.y;
				v2[0].z	= min.z;
				v2[1].x	= min.x;
				v2[1].y	= min.y;
				v2[1].z	= max.z;
				v2[2].x	= min.x;
				v2[2].y	= min.y;
				v2[2].z	= min.z;
				break;
			// top side
			case 4:					
				v1[0].x	= min.x;
				v1[0].y	= max.y;
				v1[0].z	= min.z;
				v1[1].x	= min.x;
				v1[1].y	= max.y;
				v1[1].z	= max.z;
				v1[2].x	= max.x;
				v1[2].y	= max.y;
				v1[2].z	= min.z;

				v2[0].x	= min.x;
				v2[0].y	= max.y;
				v2[0].z	= max.z;
				v2[1].x	= max.x;
				v2[1].y	= max.y;
				v2[1].z	= max.z;
				v2[2].x	= max.x;
				v2[2].y	= max.y;
				v2[2].z	= min.z;
				break;
			// bottom side
			case 5:						
				v1[0].x	= min.x;
				v1[0].y	= min.y;
				v1[0].z	= min.z;
				v1[1].x	= min.x;
				v1[1].y	= min.y;
				v1[1].z	= max.z;
				v1[2].x	= max.x;
				v1[2].y	= min.y;
				v1[2].z	= min.z;

				v2[0].x	= min.x;
				v2[0].y	= min.y;
				v2[0].z	= max.z;
				v2[1].x	= max.x;
				v2[1].y	= min.y;
				v2[1].z	= max.z;
				v2[2].x	= max.x;
				v2[2].y	= min.y;
				v2[2].z	= min.z;
				break; 
			}

			D3DXMatrixRotationYawPitchRoll( &matrot, m_Npc[nIndex].m_Mov.now_Yrad, m_Npc[nIndex].m_Mov.now_Xrad, 0.0f );
			D3DXVec3TransformCoord( &v1[0], &v1[0], &matrot );
			D3DXVec3TransformCoord( &v1[1], &v1[1], &matrot );
			D3DXVec3TransformCoord( &v1[2], &v1[2], &matrot );
			D3DXVec3TransformCoord( &v2[0], &v2[0], &matrot );
			D3DXVec3TransformCoord( &v2[1], &v2[1], &matrot );
			D3DXVec3TransformCoord( &v2[2], &v2[2], &matrot );

			D3DXVec3TransformCoord( &v1[0], &v1[0], &matpos );
			D3DXVec3TransformCoord( &v1[1], &v1[1], &matpos );
			D3DXVec3TransformCoord( &v1[2], &v1[2], &matpos );
			D3DXVec3TransformCoord( &v2[0], &v2[0], &matpos );
			D3DXVec3TransformCoord( &v2[1], &v2[1], &matpos );
			D3DXVec3TransformCoord( &v2[2], &v2[2], &matpos );

			if( IntersectTriangle(vPickRayOrig, vPickRayDir, v1[0], v1[1], v1[2], &fDist, &fBary1, &fBary2) || 
				IntersectTriangle(vPickRayOrig, vPickRayDir, v2[0], v2[1], v2[2], &fDist, &fBary1, &fBary2) )
			{				
				// 부가효과 (강제타겟) 처리 
				if( g_Pc.GetPlayer()->GetAddEffectState(n_TypeProvoke) )
				{
					if( m_nPickedNpc == NonPlayerInfoWnd.thePickedNpcID )
					{
						return m_nPickedNpc;
					}
					else
					{
						return -1;
					}
				}				
				
				// 죽어있거나, 현재 클릭한 대상이라면.. 무시
				if( GetNpcAlive(nIndex) == false )
					return -1;				

				// 클릭한 NPC 저장
			
				m_nPickedNpc = nIndex;	

				return nIndex;
			}
			

		}
	}
	return -1;
}

bool CBravoMap::IsNoBuildingUnit(D3DXVECTOR3 vOriPos, D3DXVECTOR3 vDir, float fLength)
{	
	
	D3DXMATRIX		matpos;
    D3DXMATRIX		matrot;
    D3DXVECTOR3		min, max;

	D3DXVECTOR3		vDestPoint = vOriPos + (fLength * vDir);

    D3DXVECTOR3 v1[3];

    bool IsNexInner = false;
   
    Character *pBuildCharacter = NULL;

    list < Character * >::iterator iPos;
  
	for( iPos = m_GiantBilbList.begin() ; iPos != m_GiantBilbList.end() ; ++iPos )
	{
		pBuildCharacter = (Character *)(*iPos);
		
		if(pBuildCharacter == NULL)
			continue;
		if(pBuildCharacter->m_bPC)
			continue;

		if( m_NpcTable[pBuildCharacter->m_nTableIndex].lUnique == -1 )
			continue;

		if( pBuildCharacter->m_bIsLoaded == false )
			continue;
		
		if( pBuildCharacter->m_curt_event.type == SM_STATE_DIE )
			continue;

		if( !g_Map.IsGiantUnit( pBuildCharacter ) )
			continue;

		D3DXMatrixIdentity( &matpos );		
		D3DXMatrixIdentity( &matrot );		

		matpos._41	= pBuildCharacter->GetPosTM()._41;
		matpos._42	= pBuildCharacter->GetPosTM()._42;
		matpos._43	= pBuildCharacter->GetPosTM()._43;

		D3DXVECTOR3 min;
		D3DXVECTOR3 max;
	
		min	= g_Pc_Manager.GetCBoundMinPos( pBuildCharacter );
		max	= g_Pc_Manager.GetCBoundMaxPos( pBuildCharacter );

		max.y = 100000.0f;
		min.y = -100000.0f;		

		for( int j = 0; j < 4; j ++ )
		{
			switch( j )
			{
			// front side
			case 0:				
				v1[0].x	= min.x;
				v1[0].y	= min.y;
				v1[0].z	= min.z;
				v1[1].x	= min.x;
				v1[1].y	= max.y;
				v1[1].z	= min.z;
				v1[2].x	= max.x;
				v1[2].y	= max.y;
				v1[2].z	= min.z;
				
				break; 
			// right side
			case 1:		
				v1[0].x	= max.x;
				v1[0].y	= min.y;
				v1[0].z	= min.z;
				v1[1].x	= max.x;
				v1[1].y	= max.y;
				v1[1].z	= min.z;
				v1[2].x	= max.x;
				v1[2].y	= max.y;
				v1[2].z	= max.z;

				break;
			// back side
			case 2:		
				v1[0].x	= max.x;
				v1[0].y	= min.y;
				v1[0].z	= max.z;
				v1[1].x	= min.x;
				v1[1].y	= max.y;
				v1[1].z	= max.z;
				v1[2].x	= min.x;
				v1[2].y	= min.y;
				v1[2].z	= max.z;

				break;
			// left side
			case 3:		
				v1[0].x	= min.x;
				v1[0].y	= min.y;
				v1[0].z	= max.z;			
				v1[1].x	= min.x;
				v1[1].y	= max.y;
				v1[1].z	= max.z;
				v1[2].x	= min.x;
				v1[2].y	= max.y;
				v1[2].z	= min.z;

				break;			
			}			

			D3DXMatrixRotationYawPitchRoll( &matrot, pBuildCharacter->m_Mov.now_Yrad, 
											pBuildCharacter->m_Mov.now_Xrad, 0.0f );
			D3DXVec3TransformCoord( &v1[0], &v1[0], &matrot );
			D3DXVec3TransformCoord( &v1[1], &v1[1], &matrot );
			D3DXVec3TransformCoord( &v1[2], &v1[2], &matrot );		

			D3DXVec3TransformCoord( &v1[0], &v1[0], &matpos );
			D3DXVec3TransformCoord( &v1[1], &v1[1], &matpos );
			D3DXVec3TransformCoord( &v1[2], &v1[2], &matpos );
			
			D3DXPLANE Plane;
			D3DXPlaneFromPoints( &Plane , &v1[0] , &v1[1] , &v1[2] );

			if( FALSE == m_Collision.ISIntersectPlane( vOriPos , vDestPoint , Plane ) )
			{
				continue;
			}

			D3DXVECTOR3	vCollision;
			D3DXPlaneIntersectLine( &vCollision , &Plane , &vOriPos , &vDestPoint );

			if( m_Collision.ISInsideFace( v1[0] , v1[1] , v1[2] , vCollision ) )
				return false;
		}
	}

	return true;
}

bool CBravoMap::IsNoGiantUnit(D3DXVECTOR3 vNexPos)
{
    D3DXMATRIX		matpos;
    D3DXMATRIX		matrot;
    D3DXVECTOR3		min, max;

    D3DXVECTOR3 v1[3];

    bool IsNexInner = false;
   
    Character *pGiantCharacter = NULL;

    list < Character * >::iterator iPos;
  
	for( iPos = m_GiantUnitList.begin() ; iPos != m_GiantUnitList.end() ; ++iPos )
	{
		pGiantCharacter = (Character *)(*iPos);
		
		if(pGiantCharacter == NULL)
			continue;
		if(pGiantCharacter->m_bPC)
			continue;

		if( m_NpcTable[pGiantCharacter->m_nTableIndex].lUnique == -1 )
			continue;

		if( pGiantCharacter->m_bIsLoaded == false || pGiantCharacter->m_bIsVisible == false )
			continue;
		if( pGiantCharacter->m_curt_event.type == SM_STATE_DIE )
			continue;
		if( pGiantCharacter->m_State.nRideState == nRideState_Ride )
			continue;

		if( !g_Map.IsGiantUnit( pGiantCharacter ) )
			continue;	

		D3DXMatrixIdentity( &matpos );		
		D3DXMatrixIdentity( &matrot );		

		matpos._41	= pGiantCharacter->GetPosTM()._41;
		matpos._42	= 0.0f;
		matpos._43	= pGiantCharacter->GetPosTM()._43;

		D3DXVECTOR3 min;
		D3DXVECTOR3 max;
	
		min	= g_Pc_Manager.GetCBoundMinPos( pGiantCharacter );
		max	= g_Pc_Manager.GetCBoundMaxPos( pGiantCharacter );

		//min += (min * 0.15f);
		//max += (max * 0.15f);		

		BOOL IsNexInner[4] = {FALSE, FALSE, FALSE, FALSE};

		for( int j = 0; j < 4; j ++ )
		{
			switch( j )
			{
			// front side
			case 0:				
				v1[0].x	= min.x;
				v1[0].y	= min.y;
				v1[0].z	= min.z;
				v1[1].x	= min.x;
				v1[1].y	= max.y;
				v1[1].z	= min.z;
				v1[2].x	= max.x;
				v1[2].y	= max.y;
				v1[2].z	= min.z;
				
				break; 
			// right side
			case 1:		
				v1[0].x	= max.x;
				v1[0].y	= min.y;
				v1[0].z	= min.z;
				v1[1].x	= max.x;
				v1[1].y	= max.y;
				v1[1].z	= min.z;
				v1[2].x	= max.x;
				v1[2].y	= max.y;
				v1[2].z	= max.z;

				break;
			// back side
			case 2:		
				v1[0].x	= max.x;
				v1[0].y	= min.y;
				v1[0].z	= max.z;
				v1[1].x	= min.x;
				v1[1].y	= max.y;
				v1[1].z	= max.z;
				v1[2].x	= min.x;
				v1[2].y	= min.y;
				v1[2].z	= max.z;

				break;
			// left side
			case 3:		
				v1[0].x	= min.x;
				v1[0].y	= min.y;
				v1[0].z	= max.z;			
				v1[1].x	= min.x;
				v1[1].y	= max.y;
				v1[1].z	= max.z;
				v1[2].x	= min.x;
				v1[2].y	= max.y;
				v1[2].z	= min.z;

				break;			
			}			

			D3DXMatrixRotationYawPitchRoll( &matrot, pGiantCharacter->m_Mov.now_Yrad, 
											pGiantCharacter->m_Mov.now_Xrad, 0.0f );
			D3DXVec3TransformCoord( &v1[0], &v1[0], &matrot );
			D3DXVec3TransformCoord( &v1[1], &v1[1], &matrot );
			D3DXVec3TransformCoord( &v1[2], &v1[2], &matrot );		

			D3DXVec3TransformCoord( &v1[0], &v1[0], &matpos );
			D3DXVec3TransformCoord( &v1[1], &v1[1], &matpos );
			D3DXVec3TransformCoord( &v1[2], &v1[2], &matpos );	
			
			D3DXVECTOR3 vNormal;
			D3DXVec3Cross(&vNormal, &(v1[1] - v1[0]), &(v1[2] - v1[1]));
			D3DXVec3Normalize(&vNormal, &vNormal);
			float d0 = (D3DXVec3Dot(&vNormal, &v1[0]));
			
			if(d0 > D3DXVec3Dot(&vNormal, &vNexPos) )
			{
				IsNexInner[j] = TRUE;
			}
		}

		///-- 안에서는 밖으로 나갈수 있어도 밖에서 안으로는 들어오지 못하게한다.
		///-- 현재 위치가 박스내부가 아닐경우...
		///-- 다음 위치가 박스 내부에 존재한다면
		if( IsNexInner[0] && IsNexInner[1] && IsNexInner[2] && IsNexInner[3] )
		{
			return false;
		}
	}

   return true;
}

BOOL CBravoMap::IsGiantUnit(Character *pCharacter)
{
	if( pCharacter )
	{
		if( pCharacter->m_State.nCharacterType == nCharacterTypeBigMoster ||
			pCharacter->m_State.nCharacterType == nCharacterTypeBulding )
		{
			return TRUE;
		}
	}
	
	return FALSE;
}

bool CBravoMap::IsNoGiantUnit(D3DXVECTOR3 vStartPos, D3DXVECTOR3 vNexPos, Character *pCharacter)
{
   if ( m_pDisplayMap == NULL || m_pDevice == NULL )
		return	true;

   D3DXMATRIX		matpos;
   D3DXMATRIX		matrot;
   D3DXVECTOR3		min, max;

   D3DXVECTOR3 v1[3];
   
   bool IsInRealInner = false; 
   
   Character *pGiantCharacter = NULL;

   D3DXVECTOR3 vRealPos = D3DXVECTOR3(pCharacter->GetPosTM()._41, 0.0f, pCharacter->GetPosTM()._43);   
  
   list < Character * >::iterator iPos;
  
   for( iPos = m_GiantUnitList.begin() ; iPos != m_GiantUnitList.end() ; ++iPos )
	{
		pGiantCharacter = (Character *)(*iPos);
	   
		if(pGiantCharacter == NULL)
			continue;
		if(pGiantCharacter->m_bPC)
			continue;

		if( m_NpcTable[pGiantCharacter->m_nTableIndex].lUnique == -1 )
			continue;

		if( pGiantCharacter->m_bIsLoaded == false || pGiantCharacter->m_bIsVisible == false )
			continue;
		if( pGiantCharacter->m_curt_event.type == SM_STATE_DIE )
			continue;
		if( pGiantCharacter->m_State.nRideState == nRideState_Ride )
			continue;

		if( !g_Map.IsGiantUnit( pGiantCharacter ) ) 
			continue;		

		D3DXMatrixIdentity( &matpos );		
		D3DXMatrixIdentity( &matrot );		

		matpos._41	= pGiantCharacter->GetPosTM()._41;
		matpos._42	= 0.0f;
		matpos._43	= pGiantCharacter->GetPosTM()._43;

		D3DXVECTOR3 min;
		D3DXVECTOR3 max;
	
		min	= g_Pc_Manager.GetCBoundMinPos( pGiantCharacter );
		max	= g_Pc_Manager.GetCBoundMaxPos( pGiantCharacter );

		//min += (min * 0.15f);
		//max += (max * 0.15f);
		
		BOOL IsStartInner[4] = {FALSE, FALSE, FALSE, FALSE};
		BOOL IsRealInner[4] = {FALSE, FALSE, FALSE, FALSE};
		BOOL IsNexInner[4] = {FALSE, FALSE, FALSE, FALSE};

		for( int j = 0; j < 4; j ++ )
		{
			switch( j )
			{
			// front side
			case 0:				
				v1[0].x	= min.x;
				v1[0].y	= min.y;
				v1[0].z	= min.z;
				v1[1].x	= min.x;
				v1[1].y	= max.y;
				v1[1].z	= min.z;
				v1[2].x	= max.x;
				v1[2].y	= max.y;
				v1[2].z	= min.z;
				
				break; 
			// right side
			case 1:		
				v1[0].x	= max.x;
				v1[0].y	= min.y;
				v1[0].z	= min.z;
				v1[1].x	= max.x;
				v1[1].y	= max.y;
				v1[1].z	= min.z;
				v1[2].x	= max.x;
				v1[2].y	= max.y;
				v1[2].z	= max.z;

				break;
			// back side
			case 2:		
				v1[0].x	= max.x;
				v1[0].y	= min.y;
				v1[0].z	= max.z;
				v1[1].x	= min.x;
				v1[1].y	= max.y;
				v1[1].z	= max.z;
				v1[2].x	= min.x;
				v1[2].y	= min.y;
				v1[2].z	= max.z;

				break;
			// left side
			case 3:		
				v1[0].x	= min.x;
				v1[0].y	= min.y;
				v1[0].z	= max.z;			
				v1[1].x	= min.x;
				v1[1].y	= max.y;
				v1[1].z	= max.z;
				v1[2].x	= min.x;
				v1[2].y	= max.y;
				v1[2].z	= min.z;

				break;			
			}			

			D3DXMatrixRotationYawPitchRoll( &matrot, pGiantCharacter->m_Mov.now_Yrad, 
											pGiantCharacter->m_Mov.now_Xrad, 0.0f );
			D3DXVec3TransformCoord( &v1[0], &v1[0], &matrot );
			D3DXVec3TransformCoord( &v1[1], &v1[1], &matrot );
			D3DXVec3TransformCoord( &v1[2], &v1[2], &matrot );		

			D3DXVec3TransformCoord( &v1[0], &v1[0], &matpos );
			D3DXVec3TransformCoord( &v1[1], &v1[1], &matpos );
			D3DXVec3TransformCoord( &v1[2], &v1[2], &matpos );	
			
			D3DXVECTOR3 vNormal;
			D3DXVec3Cross(&vNormal, &(v1[1] - v1[0]), &(v1[2] - v1[1]));
			D3DXVec3Normalize(&vNormal, &vNormal);
			float d0 = (D3DXVec3Dot(&vNormal, &v1[0]));
			float fNowD0 = D3DXVec3Dot(&vNormal, &vStartPos);		
			
			///-- 현재 포지션이 해당 평면의 내부에 있는지를 체크한다.
			///-- 노말값이 작아야 평면 뒤쪽에 존재하는것이다.
			if(d0 > fNowD0 )
			{
				IsStartInner[j] = TRUE;
			}

			if(d0 > D3DXVec3Dot(&vNormal, &vRealPos) )
			{
				IsRealInner[j] = TRUE;
			}
			
			if(d0 > D3DXVec3Dot(&vNormal, &vNexPos) )
			{
				IsNexInner[j] = TRUE;
			}

		}
		//출발 위치가 박스 내부에 존재한다면..
		if( IsStartInner[0]  && IsStartInner[1] && IsStartInner[2] && IsStartInner[3] )
		{
			if( !IsRealInner[0] || !IsRealInner[1] || !IsRealInner[2] || !IsRealInner[3] )
			{
				pCharacter->m_Mov.vCVtPos.x = vRealPos.x;
				pCharacter->m_Mov.vCVtPos.z = vRealPos.z;
			}
			
			return true;
		}
		else //-- 출발 위치가 박스 내부에 존재하지않으면...
		{
			if( !IsRealInner[0] || !IsRealInner[1] || !IsRealInner[2] || !IsRealInner[3] )
			{
				pCharacter->m_Mov.vCVtPos.x = vRealPos.x;
				pCharacter->m_Mov.vCVtPos.z = vRealPos.z;
			}
			else
			{
				IsInRealInner = true;
			}
		}

		///-- 안에서는 밖으로 나갈수 있어도 밖에서 안으로는 들어오지 못하게한다.
		///-- 현재 위치가 박스내부가 아닐경우...
		///-- 다음 위치가 박스 내부에 존재한다면
		if( IsNexInner[0] && IsNexInner[1] && IsNexInner[2] && IsNexInner[3] )
		{
			return false;
		}		
	}

   ///-- 어떤 충돌박스 유닛과도 만나지 않을 경우
   if( !IsInRealInner )
   {
	   pCharacter->m_Mov.vCVtPos.x = vRealPos.x;
	   pCharacter->m_Mov.vCVtPos.z = vRealPos.z;
   }

   return true;
}

void CBravoMap::LoadEmbObjAndNationObj()
{
	CTextCommand TextCmd;

	if( TextCmd.OpenVF( &g_VFMap, "embobj.txt" ) != NULL )
	{
		while( TextCmd.GetLine() != NULL )
		{
			if( TextCmd.ISArgEmpty() )
			{
				continue;
			}

			WORD nEmbObj = 0;		
			TextCmd.GetArgValue( 0, nEmbObj);		
			m_EmbObjList.insert(map <WORD , WORD>::value_type(nEmbObj, nEmbObj));	
		}

		TextCmd.Close(&g_VFMap);
	}

	if( TextCmd.OpenVF( &g_VFMap, "nationobj.txt" ) != NULL )
	{
		while( TextCmd.GetLine() != NULL )
		{
			if( TextCmd.ISArgEmpty() )
			{
				continue;
			}

			WORD nNationObj = 0;
			WORD nNation = 0;
			TextCmd.GetArgValue( 0, nNationObj);
			TextCmd.GetArgValue( 1, nNation);

			SNationOBJ sNatObj;
			sNatObj.dOBjNum = nNationObj;
			sNatObj.nNation = (int)nNation; 
			
			m_NationObjList.insert(map <WORD , SNationOBJ>::value_type(nNationObj, sNatObj));	
		}

		TextCmd.Close(&g_VFMap);
	}
}

BOOL CBravoMap::IsEmbObj(WORD nObjNum)
{
	EmbObjListPos iPos = m_EmbObjList.find(nObjNum);

	if(iPos == m_EmbObjList.end())
	{
		return FALSE;	
	}

	return TRUE;
}

SNationOBJ * CBravoMap::FineNationOBJPtr(WORD nObjNum)
{
	NationOBJListPos iPos = m_NationObjList.find(nObjNum);
	
	if(iPos == m_NationObjList.end())
	{
		return NULL;	
	}
	
	return (SNationOBJ *)(&((*iPos).second));
}

BOOL CBravoMap::ReSetMapIndex(int nIndex)
{
	if(m_uMapType != MAP_TYPE_COLONY)
	{
		return FALSE;
	}

	if(m_nExColInfo.m_nMapIndex == nIndex)
	{
		return FALSE;
	}

	m_nExColInfo.m_nMapIndex = nIndex;
	m_bObjSortInit = false;

	if ( m_pNodeObj )
	{
		CleanupObjNode();			/// 이게 0.8초 정도 먹네요. 개선 필요. 
	}

	CleanupObj();	

	char	name[MAX_PATH];		
	sprintf( name, "%s\\map\\map%d.rvf", g_RockClient.GetRootDir(), m_NowMapID );

	/// 이 안에서 CloseFileSystem() 미리 한번 하니까, 전에 열었던 놈은 자동으로 닫힘 
	if( g_VFMap.OpenFileSystem( name ) == FALSE ) 
	{
		return FALSE;
	}

	sprintf( name, "%d_%d.orm", m_NowMapID, m_nExColInfo.m_nMapIndex );

	VFileHandle* pFH = g_VFMap.OpenFile( name );
	if( pFH == NULL )
	{
		return FALSE;
	}

	SAFE_DELETE_ARRAY(m_NewFile.pObjList);

	BRVF TempFile;
	CGrassPoint TempGrassPoint;

	pFH->Read( &TempFile.byType					, sizeof(BYTE		) );		
	pFH->Read( &TempFile.cSize					, sizeof(WORD		) );
	pFH->Read( &TempFile.byTextureIndex			, sizeof(BYTE		) );
	pFH->Read( &TempFile.bySkyTextureIndex		, sizeof(BYTE		) );
	pFH->Read( &TempFile.byCloudTextureIndex	, sizeof(BYTE		) );
	pFH->Read( &TempFile.byWaterCount			, sizeof(BYTE		) );	/// sooree water
	pFH->Read( &TempFile.bShowSky				, sizeof(BOOL		) );
	pFH->Read( &TempFile.bShowCloud				, sizeof(BOOL		) );

	pFH->Read( &TempFile.bShowWater				, sizeof(BOOL		) );	/// 읽기는 읽지만 무시합니다. 	
	pFH->Read( &TempFile.byWaterHeightVariance	, sizeof(BYTE		) );	/// 읽기는 읽지만 무시합니다. 
	pFH->Read( &TempFile.fWaterBaseHeight		, sizeof(float		) );	/// 읽기는 읽지만 무시합니다. 
	pFH->Read( &TempFile.byWaterRunsDir			, sizeof(BYTE		) );	/// 읽기는 읽지만 무시합니다. 

#ifdef DIRECT_VERSION_9_MJH
	pFH->Read( &TempFile.d3dMaterial			, sizeof(D3DMATERIAL9) );
	pFH->Read( &TempFile.d3dLight				, sizeof(D3DLIGHT9	) );
#else
	pFH->Read( &TempFile.d3dMaterial			, sizeof(D3DMATERIAL8) );
	pFH->Read( &TempFile.d3dLight				, sizeof(D3DLIGHT8	) );
#endif // DIRECT_VERSION_9_MJH
	pFH->Read( &TempFile.nObjCount				, sizeof(WORD		) );
	pFH->Read( &TempFile.nNpcCount				, sizeof(WORD		) );
	pFH->Read( &TempFile.nGrsCount				, sizeof(WORD		) );
	pFH->Read( &TempFile.nEffectCount			, sizeof(WORD		) );
	pFH->Read( &TempFile.nWaterfallCount		, sizeof(WORD		) );

	TempFile.pAttrList2		= SAFE_NEW_ARRAY( MAP_ATTR2 , (TempFile.cSize<<1)*(TempFile.cSize<<1) );	
	TempFile.pHeightList	= SAFE_NEW_ARRAY( short , (TempFile.cSize+1)*(TempFile.cSize+1) );	
	TempFile.pColorList		= SAFE_NEW_ARRAY( COLOR_RGB , (TempFile.cSize+1)*(TempFile.cSize+1) );	
	TempFile.pTileIndexList	= SAFE_NEW_ARRAY( WORD , TempFile.cSize*TempFile.cSize );
	
	if( TempFile.nObjCount )
	{
		TempFile.pObjList	= SAFE_NEW_ARRAY( OBJ_INFO , TempFile.nObjCount );		
	}
	TempFile.pNpcList		= SAFE_NEW_ARRAY( NPC_INFO2 , TempFile.nNpcCount );
	if( TempFile.nEffectCount )
	{
		TempFile.pEffectList = SAFE_NEW_ARRAY( PARTICLE_INFO , TempFile.nEffectCount );	
	}

	TempGrassPoint.m_Data	= SAFE_NEW_ARRAY( GRASS_POINT , TempFile.nGrsCount );	
	TempFile.pWaterfallList	= SAFE_NEW_ARRAY( WATERFALL_INFO , TempFile.nWaterfallCount );	

	pFH->Read( TempFile.pAttrList2		, sizeof(MAP_ATTR2)		* (TempFile.cSize<<1)*(TempFile.cSize<<1)	);
	pFH->Read( TempFile.pHeightList		, sizeof(short)			* (TempFile.cSize+1)*(TempFile.cSize+1)	);
	pFH->Read( TempFile.pColorList		, sizeof(COLOR_RGB)		* (TempFile.cSize+1)*(TempFile.cSize+1)	);
	pFH->Read( TempFile.pTileIndexList	, sizeof(WORD)			* TempFile.cSize*TempFile.cSize			);

	if( TempFile.nObjCount )
	{
		pFH->Read( TempFile.pObjList		, sizeof(OBJ_INFO)		* TempFile.nObjCount						);
	}

	pFH->Read( TempFile.pNpcList		, sizeof(NPC_INFO2)		* TempFile.nNpcCount						);

	if( TempFile.nEffectCount )
	{
		pFH->Read( TempFile.pEffectList		, sizeof(PARTICLE_INFO)	* TempFile.nEffectCount					);
	}
	
	pFH->Read( TempGrassPoint.m_Data	, sizeof(GRASS_POINT)	* TempFile.nGrsCount						);
	pFH->Read( TempFile.pWaterfallList	, sizeof(WATERFALL_INFO)* TempFile.nWaterfallCount					);
	pFH->Read( TempFile.WarpList		, sizeof(FILEWARP)		* 9											);	

	g_VFMap.CloseFile( pFH );

	SAFE_DELETE_ARRAY(TempFile.pAttrList2);
	SAFE_DELETE_ARRAY(TempFile.pHeightList);
	SAFE_DELETE_ARRAY(TempFile.pColorList);
	SAFE_DELETE_ARRAY(TempFile.pTileIndexList);
	SAFE_DELETE_ARRAY(TempFile.pNpcList);
	
	if(TempFile.nEffectCount)
	{
		SAFE_DELETE_ARRAY(TempFile.pEffectList);
	}

	SAFE_DELETE_ARRAY(TempGrassPoint.m_Data);
	SAFE_DELETE_ARRAY(TempFile.pWaterfallList);

	m_NewFile.nObjCount = TempFile.nObjCount;	
	m_NewFile.pObjList = TempFile.pObjList;

	g_TileTexture.ReSetShadowMap( theMapPath );

	CreateObjIndex();

	CreateObjNode();

	if( m_NewFile.nObjCount )
	{
		SAFE_DELETE_ARRAY(m_NewFile.pObjList);
	}

	MakeObjectBoundBox();	

	int destx = g_Pc.GetDestX();
	int desty = g_Pc.GetDestY();	

	g_Camera.Update();
	g_Camera.m_bRotBySort = true;
	g_Map.UpdateTerrain(destx, desty, true);

	return TRUE;
}

#define		NONBLENDING_OBJ		67
short	g_NonBlendingObjList[NONBLENDING_OBJ]	= 
{104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,128,5,6,36,38,83,84,85,86,
87,89,90,93,94,99,103,129,130,8,9,10,11,158,159,160,161,167,152,153,154,4,0,1,2,3,35,87,174,175,185,189,190,193};

bool	CBravoMap::CheckObjBlending(const short& idx, const short& sidx)
{

	BYTE				btBlend	= m_Object[idx].m_pAniPos[sidx].m_btBlended;

	// character pos	( build to 0,0,0 )
	D3DXMATRIX			matCameraPos;
	D3DXMatrixIdentity(&matCameraPos);
	
	// 카메라 워킹 관련 By wxywxy
	if( g_RockClient.GetGameMode() == GMODE_NORMAL )
	{
		matCameraPos._41	= g_Pc.GetPlayer()->GetPosTM()._41 + g_Camera.m_vEye.x - g_Camera.m_vLookat.x;
		matCameraPos._42	= g_Pc.GetPlayer()->GetPosTM()._42 + g_Camera.m_vEye.y - g_Camera.m_vLookat.y;
		matCameraPos._43	= g_Pc.GetPlayer()->GetPosTM()._43 + g_Camera.m_vEye.z - g_Camera.m_vLookat.z;
	}
	else
	{
		matCameraPos._41	= g_Camera.m_vEye.x;
		matCameraPos._42	= g_Camera.m_vEye.y;
		matCameraPos._43	= g_Camera.m_vEye.z;
	}

	D3DXMATRIX			matChar	= g_Pc.GetPosTM(-1);
	D3DXVECTOR3			vSub	= D3DXVECTOR3(matChar._41,matChar._42,matChar._43);

	// obj pos			( obj - char )
	D3DXVECTOR3			vMin	= m_Object[idx].m_Bound.m_min_vertex;
	D3DXVECTOR3			vMax	= m_Object[idx].m_Bound.m_max_vertex;
	D3DXMATRIX			matTemp;
	D3DXMatrixIdentity(&matTemp);

	matTemp._41	= m_Object[idx].m_pAniPos[sidx].GetPosTM()._41;
	matTemp._42	= m_Object[idx].m_pAniPos[sidx].GetPosTM()._42;
	matTemp._43	= m_Object[idx].m_pAniPos[sidx].GetPosTM()._43;

//	D3DXMatrixScaling(&matTemp, 2.0f, 2.0f, 2.0f);
	D3DXVec3TransformCoord(&vMin, &vMin, &matTemp);
	D3DXVec3TransformCoord(&vMax, &vMax, &matTemp);
	SetupMaxMin(&vMax, &vMin);
	
	float				sizex	= (vMax.x-vMin.x);
	float				sizey	= (vMax.y-vMin.y);
	float				sizez	= (vMax.z-vMin.z);

	// camera pos		( delta series )

	// calculate
	float				fToCross	= 0.0f;
	D3DXVECTOR3			vFinal;

	//.......................................................................................................	
	// Cull Object By wxywxy
	//.......................................................................................................	
	if ( true == (m_Object[idx].m_pAniPos[sidx].m_bIsVisible = g_Camera.CubeInFrustum( 
		m_Object[idx].m_pAniPos[sidx].m_vMin.x  ,
		m_Object[idx].m_pAniPos[sidx].m_vMin.y  ,
		m_Object[idx].m_pAniPos[sidx].m_vMin.z  ,
		m_Object[idx].m_pAniPos[sidx].m_vSize.x ,
		m_Object[idx].m_pAniPos[sidx].m_vSize.y ,
		m_Object[idx].m_pAniPos[sidx].m_vSize.z  ) ) )
	{
		dwObjVisibleCount++;
	}	
	//.......................................................................................................	

	//if ( m_byMapType == MS_INDOOR )
	//	return	false;	

	if ( m_Object[idx].m_pAniPos[sidx].m_bIsVisible == false )
	{
		return	false;	
	}	


	// 카메라와 오브젝트거리
	float	_x  = (matCameraPos._41-matTemp._41)*(matCameraPos._41-matTemp._41);
	float	_y 	= (matCameraPos._42-matTemp._42)*(matCameraPos._42-matTemp._42);
	float	_z 	= (matCameraPos._43-matTemp._43)*(matCameraPos._43-matTemp._43);
	m_Object[idx].m_pAniPos[sidx].m_fDistance = sqrt(_x+_y+_z);	

	float fDis = m_Object[idx].m_pAniPos[sidx].m_fDistance;
	/*if( idx == 9 )
	{
		int a = 0;
	}*/

	///---카메라와 해당 오브젝트 간의 거리 구하기

	//.......................................................................................................
	// 오브젝트 바운딩 박스 체크 By wxywxy
	//.......................................................................................................
	if( g_RockClient.GetGameMode() == GMODE_NORMAL && m_Object[idx].m_pAniPos[sidx].m_bIsVisible == TRUE )
	{
		static int index[] = {
					0 , 1 , 2 ,
					2 , 1 , 3 ,
					2 , 3 , 6 ,
					6 , 3 , 7 ,
					4 , 5 , 1 , 
					4 , 1 , 0 ,
					6 , 7 , 5 ,
					6 , 5 , 4 ,
					1 , 5 , 3 ,
					3 , 5 , 7 ,
					2 , 6 , 4 , 
					2 , 4 , 0
		};

		///-- vPos 캐릭터 위치
		D3DXVECTOR3	vPos = D3DXVECTOR3( g_Pc.GetPlayer()->GetPosTM()._41 , 
			g_Pc.GetPlayer()->GetPosTM()._42 + ( g_Pc.GetPlayer()->GetBoundMaxPos().y - 
												 g_Pc.GetPlayer()->GetBoundMinPos().y ) * 0.5f ,
			g_Pc.GetPlayer()->GetPosTM()._43 );
		
		D3DXVECTOR3 vGet(0.0f,0.0f,0.0f);
		D3DXVECTOR3 vCamera = g_Camera.m_vEye;
		
		BOOL ISCheck = FALSE;
		int  Count   = 0;

		///-- m_ISBlend <-- 이 옵션은 맵툴 에서 설정하는것인듯...
		///-- 여기는 오브젝트 캐릭터와 카메라 사이 충돌 체크하는 곳인듯...
		if( m_Object[idx].GetISBlend() == TRUE )
		{
			if( CheckPointInBox( &vCamera , m_Object[idx].m_pAniPos[sidx].m_Plane ) == TRUE )
			{
				ISCheck = TRUE;
				
				if( m_Object[idx].m_pAniPos[sidx].m_btBlended != 0 )
				{			
					m_Object[idx].m_pAniPos[sidx].m_lCnt = SAFE_TIME_ADD( g_nowTime , 500 );
					m_Object[idx].m_pAniPos[sidx].m_btBlended = 0;
				}

			}
			else if( CheckPointInBox( &vPos , m_Object[idx].m_pAniPos[sidx].m_Plane ) == TRUE  )
			{
				ISCheck = TRUE;
				
				if( m_Object[idx].m_pAniPos[sidx].m_btBlended != 0 )
				{			
					m_Object[idx].m_pAniPos[sidx].m_lCnt = SAFE_TIME_ADD( g_nowTime , 500 );
					m_Object[idx].m_pAniPos[sidx].m_btBlended = 0;
				}

			}
			else
			{
				// 선 검색
				for( int i = 0 ; i < 12 ; ++i )
				{
					if( m_Collision.ISFaceCollision( m_Object[idx].m_pAniPos[sidx].m_vBoundList[ index[ Count ] ] , 
						m_Object[idx].m_pAniPos[sidx].m_vBoundList[ index[ Count + 1 ] ] , 
						m_Object[idx].m_pAniPos[sidx].m_vBoundList[ index[ Count + 2 ] ] ,
						vCamera , vPos , vGet ) )
					{
						ISCheck = TRUE;
						
						if( m_Object[idx].m_pAniPos[sidx].m_btBlended == 0 )
							break;
						
						m_Object[idx].m_pAniPos[sidx].m_lCnt = SAFE_TIME_ADD( g_nowTime , 500 );
						m_Object[idx].m_pAniPos[sidx].m_btBlended = 0;
						
						break;
					}
					
					Count+= 3;
				}
			}
		}

		if( !ISCheck )
		{
			m_Object[idx].m_pAniPos[sidx].m_btBlended = 2;
			m_Object[idx].m_pAniPos[sidx].m_lCnt = 0;
		}		
	}

/*	
///	by sooree : 어색해 보여서 막았습니다. 


//	if ( g_Camera.m_fCameraPitch <= 0.3f )
//		goto	objblend_final;


//	if ( m_Object[idx].m_pAniPos[sidx].m_fDistance > 384.0f )
//	{
//		m_Object[idx].m_pAniPos[sidx].m_bIsVisible	= false;
//	}	

//	for ( short i = 0; i < NONBLENDING_OBJ; i ++ )
//	{
//		if ( g_NonBlendingObjList[i] == m_Object[idx].m_dwIndex )
//			return	true;
//	}

*/
	return	false;
}

void	CBravoMap::CheckPcOnGrass(node_obj * node, short depth)
{
}

bool	CBravoMap::UpdateWaterfall()
{
	D3DXMATRIX		matChar	= g_Pc.GetPosTM(-1);
	D3DXMATRIX		matTemp;

	for ( short i = 0; i < m_nWaterfallCount; i ++ )
	{
		if ( m_WaterfallList[i].Enable() )
		{
			matTemp	= m_WaterfallList[i].GetPosTM();
			const float	_x  = (matChar._41-matTemp._41)*(matChar._41-matTemp._41);
			const float	_y 	= (matChar._43-matTemp._43)*(matChar._43-matTemp._43);			

			float	distance	= sqrt(_x+_y);

			if ( distance > 500.0f )
			{
				m_WaterfallList[i].SetVisible(false);
			}
			else
				m_WaterfallList[i].SetVisible(true);
		}
	}
	return	true;
}

void	CBravoMap::ProcessLightByTime()
{

}

bool	CBravoMap::updateonter(const short& cx, const short& cy)
{
	vtxpos	*	pVertices;
	WORD	*	pIndices;

	short	sx, sy, ex, ey;
#ifdef DIRECT_VERSION_9_MJH
	if ( FAILED ( m_pVBPos->Lock(0, 0, (void**)&pVertices, 0 ) ) )
#else
	if ( FAILED ( m_pVBPos->Lock(0, 0, (BYTE**)&pVertices, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH
	{
		return false;
	}
#ifdef DIRECT_VERSION_9_MJH
	if ( FAILED ( m_pIBPos->Lock(0, 0, (void**)&pIndices, 0 ) ) )
#else
	if ( FAILED ( m_pIBPos->Lock(0, 0, (BYTE**)&pIndices, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH
	{
		return false;
	}

	sx	= ((cx-20)<0) ? 0 : cx-20;
	sy	= ((cy-20)<0) ? 0 : cy-20;
	ex	= ((cx+20)>m_cSize<<1) ? m_cSize<<1 : cx+20;
	ey	= ((cy+20)>m_cSize<<1) ? m_cSize<<1 : cy+20;

	short	nvtxcount	= 0;
	short	nidxcount	= 0;

	short	x, y;

	for ( y = sy; y < ey; y ++ )
	{
		for ( x = sx; x < ex; x ++ )
		{
			unsigned long color;
			if( m_pAttrMap[y][x].byAttr == MATTR_OBJ || m_pAttrMap[y][x].byAttr == 13 || m_pAttrMap[y][x].byAttr == 14 )
			{
				if ( nvtxcount >= 40*40*4 || nidxcount >= 40*40*6 )
					continue;

				if( m_Block[x][y].Attr == 1 ) color = D3DCOLOR_ARGB(64, 255, 0, 0);
				else color = D3DCOLOR_ARGB(64, 0, 0, 255);

				pVertices[nvtxcount].x	= x*8.0f;
				pVertices[nvtxcount].y	= ((m_pAttrMap[y][x].nHeight != 0)? m_pAttrMap[y][x].nHeight : (Get__MapHeight( x, y ) + 0.5f));
				pVertices[nvtxcount].z	= y*8.0f;
				pVertices[nvtxcount].color	= color;
				++nvtxcount;

				pVertices[nvtxcount].x	= x*8.0f;
				pVertices[nvtxcount].y	= ((m_pAttrMap[y][x].nHeight != 0)? m_pAttrMap[y][x].nHeight : (Get__MapHeight( x, y ) + 0.5f));
				pVertices[nvtxcount].z	= (y+1)*8.0f;
				pVertices[nvtxcount].color	= color;
				++nvtxcount;

				pVertices[nvtxcount].x	= (x+1)*8.0f;
				pVertices[nvtxcount].y	= ((m_pAttrMap[y][x].nHeight != 0)? m_pAttrMap[y][x].nHeight : (Get__MapHeight( x, y ) + 0.5f));
				pVertices[nvtxcount].z	= (y+1)*8.0f;
				pVertices[nvtxcount].color	= color;
				++nvtxcount;

				pVertices[nvtxcount].x	= (x+1)*8.0f;
				pVertices[nvtxcount].y	= ((m_pAttrMap[y][x].nHeight != 0)? m_pAttrMap[y][x].nHeight : (Get__MapHeight( x, y ) + 0.5f));
				pVertices[nvtxcount].z	= y*8.0f;
				pVertices[nvtxcount].color	= color;
				++nvtxcount;

				pIndices[nidxcount++]	= nvtxcount-4;
				pIndices[nidxcount++]	= nvtxcount-3;
				pIndices[nidxcount++]	= nvtxcount-1;
				pIndices[nidxcount++]	= nvtxcount-2;
				pIndices[nidxcount++]	= nvtxcount-1;
				pIndices[nidxcount++]	= nvtxcount-3;
			}
		}
	}

/*
	PHF_PickDest(g_RockClient.GetApp());

	x	= m_dwPickedX;
	y	= m_dwPickedY;

	pVertices[nvtxcount].x	= x*8.0f;
	pVertices[nvtxcount].y	= Get__MapHeight( x, y ) + 1;	/// sooree height
	pVertices[nvtxcount].z	= y*8.0f;
	pVertices[nvtxcount].color	= D3DCOLOR_XRGB(0,255,255);
	nvtxcount	++;

	pVertices[nvtxcount].x	= x*8.0f;
	pVertices[nvtxcount].y	= Get__MapHeight( x, y ) + 1;	/// sooree height
	pVertices[nvtxcount].z	= (y+1)*8.0f;
	pVertices[nvtxcount].color	= D3DCOLOR_XRGB(0,255,255);
	nvtxcount	++;

	pVertices[nvtxcount].x	= (x+1)*8.0f;
	pVertices[nvtxcount].y	= Get__MapHeight( x, y ) + 1;	/// sooree height
	pVertices[nvtxcount].z	= (y+1)*8.0f;
	pVertices[nvtxcount].color	= D3DCOLOR_XRGB(0,255,255);
	nvtxcount	++;

	pVertices[nvtxcount].x	= (x+1)*8.0f;
	pVertices[nvtxcount].y	= Get__MapHeight( x, y ) + 1;	/// sooree height
	pVertices[nvtxcount].z	= y*8.0f;
	pVertices[nvtxcount].color	= D3DCOLOR_XRGB(0,255,255);
	nvtxcount	++;

	pIndices[nidxcount++]	= nvtxcount-4;
	pIndices[nidxcount++]	= nvtxcount-3;
	pIndices[nidxcount++]	= nvtxcount-1;
	pIndices[nidxcount++]	= nvtxcount-2;
	pIndices[nidxcount++]	= nvtxcount-1;
	pIndices[nidxcount++]	= nvtxcount-3;

	// pc
	x	= g_Pc.GetDestX();
	y	= g_Pc.GetDestY();

	pVertices[nvtxcount].x	= x*8.0f;
	pVertices[nvtxcount].y	= Get__MapHeight( x, y ) + 1;	/// sooree height
	pVertices[nvtxcount].z	= y*8.0f;
	pVertices[nvtxcount].color	= D3DCOLOR_XRGB(255,0,255);
	nvtxcount	++;

	pVertices[nvtxcount].x	= x*8.0f;
	pVertices[nvtxcount].y	= Get__MapHeight( x, y ) + 1;	/// sooree height
	pVertices[nvtxcount].z	= (y+1)*8.0f;
	pVertices[nvtxcount].color	= D3DCOLOR_XRGB(255,0,255);
	nvtxcount	++;

	pVertices[nvtxcount].x	= (x+1)*8.0f;
	pVertices[nvtxcount].y	= Get__MapHeight( x, y ) + 1;	/// sooree height
	pVertices[nvtxcount].z	= (y+1)*8.0f;
	pVertices[nvtxcount].color	= D3DCOLOR_XRGB(255,0,255);
	nvtxcount	++;

	pVertices[nvtxcount].x	= (x+1)*8.0f;
	pVertices[nvtxcount].y	= Get__MapHeight( x, y ) + 1;	/// sooree height
	pVertices[nvtxcount].z	= y*8.0f;
	pVertices[nvtxcount].color	= D3DCOLOR_XRGB(255,0,255);
	nvtxcount	++;

	pIndices[nidxcount++]	= nvtxcount-4;
	pIndices[nidxcount++]	= nvtxcount-3;
	pIndices[nidxcount++]	= nvtxcount-1;
	pIndices[nidxcount++]	= nvtxcount-2;
	pIndices[nidxcount++]	= nvtxcount-1;
	pIndices[nidxcount++]	= nvtxcount-3;

	// pc dirtile
	x	= g_Pc.GetPlayer()->m_Mov.path.x;
	y	= g_Pc.GetPlayer()->m_Mov.path.y;

	pVertices[nvtxcount].x	= x*8.0f;
	pVertices[nvtxcount].y	= Get__MapHeight( x, y ) + 1;	/// sooree height
	pVertices[nvtxcount].z	= y*8.0f;
	pVertices[nvtxcount].color	= D3DCOLOR_XRGB(255,255,0);
	nvtxcount	++;

	pVertices[nvtxcount].x	= x*8.0f;
	pVertices[nvtxcount].y	= Get__MapHeight( x, y ) + 1;	/// sooree height
	pVertices[nvtxcount].z	= (y+1)*8.0f;
	pVertices[nvtxcount].color	= D3DCOLOR_XRGB(255,255,0);
	nvtxcount	++;

	pVertices[nvtxcount].x	= (x+1)*8.0f;
	pVertices[nvtxcount].y	= Get__MapHeight( x, y ) + 1;	/// sooree height
	pVertices[nvtxcount].z	= (y+1)*8.0f;
	pVertices[nvtxcount].color	= D3DCOLOR_XRGB(255,255,0);
	nvtxcount	++;

	pVertices[nvtxcount].x	= (x+1)*8.0f;
	pVertices[nvtxcount].y	= Get__MapHeight( x, y ) + 1;	/// sooree height
	pVertices[nvtxcount].z	= y*8.0f;
	pVertices[nvtxcount].color	= D3DCOLOR_XRGB(255,255,0);
	nvtxcount	++;

	pIndices[nidxcount++]	= nvtxcount-4;
	pIndices[nidxcount++]	= nvtxcount-3;
	pIndices[nidxcount++]	= nvtxcount-1;
	pIndices[nidxcount++]	= nvtxcount-2;
	pIndices[nidxcount++]	= nvtxcount-1;
	pIndices[nidxcount++]	= nvtxcount-3;
*/
	m_nvtxinvb	= nvtxcount;
	m_nidxinib	= nidxcount;

	m_pVBPos->Unlock();
	m_pIBPos->Unlock();

	return	true;
}

void	CBravoMap::displayonter()
{
	g_RenderManager.SetLighting( FALSE );

	g_RenderManager.SetTransform(D3DTS_WORLD, &m_matWorld);
	g_RenderManager.SetStreamSource(0, m_pVBPos, sizeof(vtxpos));	

	g_RenderManager.SetFVF( D3DFVF_XYZ|D3DFVF_DIFFUSE );

	g_RenderManager.SetIndices(m_pIBPos, 0);
	g_RenderManager.DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, m_nvtxinvb, 0, m_nidxinib/3);

	g_RenderManager.SetLighting( TRUE );
}

////////////////////////////////////////////////////////////////////////////////////////////
//
//
CEffectMap::CEffectMap()
{
	m_pTexture	= NULL;
	m_pVB		= NULL;
	m_pIB		= NULL;
}

CEffectMap::~CEffectMap()
{
//	_RELEASE(m_pTexture);
	_RELEASE(m_pVB);
	_RELEASE(m_pIB);
}

#ifdef DIRECT_VERSION_9_MJH
HRESULT	CEffectMap::CreateEnvironment(LPDIRECT3DDEVICE9 pDevice)
#else
HRESULT	CEffectMap::CreateEnvironment(LPDIRECT3DDEVICE8 pDevice)
#endif // DIRECT_VERSION_9_MJH
{
	D3DXMatrixIdentity(&m_matPos);
	D3DXMatrixIdentity(&m_matTexPos);

//	m_dwFVF	= D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1;
	m_dwFVF		= D3DFVF_EFFECTMAP;

	if ( FAILED ( g_RenderManager.CreateVertexBuffer(1024*sizeof(FVF_EFFECTMAP), D3DUSAGE_WRITEONLY, m_dwFVF, D3DPOOL_MANAGED, &m_pVB) ) )
	{
///		MessageBox(NULL, "Error - Create(VB)", "EffectMap", MB_OK);
		return E_FAIL;
	}
	if ( FAILED ( g_RenderManager.CreateIndexBuffer(1600*sizeof(WORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIB) ) )
	{
///		MessageBox(NULL, "Error - Create(IB)", "EffectMap", MB_OK);
		return E_FAIL;
	}

	return	S_OK;
}

HRESULT	CEffectMap::Update(const D3DXMATRIX& mat)
{
	WORD *			pIndices;
//	FVF_LIGHTMAP *	pVertices;
	FVF_EFFECTMAP *	pVertices;

//	m_matTexPos._31	= (short)(mat._41)%16;
//	m_matTexPos._32	= (short)(mat._43)%16;

	// 프로젝션 구하기
	//D3DXMATRIX	matProj;
	//D3DXMatrixPerspectiveFovLH( matProj, D3DX_PI/4, 1.3f, 1.0f, 1000.0f );	
	
	D3DXMATRIX	matMiddle;
	//matMiddle._11 = 3.01883f;
	matMiddle._11 = 2.0f;
	matMiddle._12 = 0.0f;
	matMiddle._13 = 0.0f;
	matMiddle._14 = 0.0f;
	matMiddle._21 = 0.0f;
	//matMiddle._22 = 3.49848f;
	matMiddle._22 = 2.0f;
	matMiddle._23 = 0.0f;
	matMiddle._24 = 0.0f;
	matMiddle._31 = 0.0f;
	matMiddle._32 = 0.0f;
	matMiddle._33 = 1.0f;
	matMiddle._34 = 1.0f;
	matMiddle._41 = 0.0f;
	matMiddle._42 = 0.0f;
	matMiddle._43 = -1.0f;
	matMiddle._44 = 0.0f;

	D3DXMATRIX		matWorldToLight;
	D3DXVECTOR3	vPos	= D3DXVECTOR3(mat._41,mat._42+20.0f, mat._43-1.0f);
	D3DXMatrixLookAtLH(&matWorldToLight, &vPos, &D3DXVECTOR3(mat._41,mat._42,mat._43),&D3DXVECTOR3(0.0f,1.0f,0.0f));

	D3DXMATRIX	offset;					
	D3DXMATRIX	matCameraToWorld;
	D3DXMatrixInverse( &matCameraToWorld, NULL, &g_Camera.m_matView );
	D3DXMatrixIdentity( &offset );
	offset._11 =  0.5f;
	offset._22 = -0.5f;	
	offset._31 =  0.5f;
	offset._32 =  0.5f;
	//offset._31 =  0.2f;
	//offset._32 =  1.0f;		
	
	// 텍스쳐행렬만들기
	D3DXMatrixMultiply( &m_matTexPos, &matCameraToWorld, &matWorldToLight );
	D3DXMatrixMultiply( &m_matTexPos, &m_matTexPos, &matMiddle );	
	D3DXMatrixMultiply( &m_matTexPos, &m_matTexPos, &offset );

	short	x	= (short)(mat._41/16);
	short	y	= (short)(mat._43/16);

//	float	tx	= mat._41/8.0f;
//	float	ty	= mat._43/8.0f;

//	m_matTexPos._31	= 1.0f-((mat._41-(tx*8))/8.0f);
//	m_matTexPos._32	= ((mat._43-(ty*8))/8.0f);

	m_nVtxCount	= 0;
	m_nIdxCount	= 0;
#ifdef DIRECT_VERSION_9_MJH
	if ( FAILED ( m_pIB->Lock(0, 0, (void**)&pIndices, 0 ) ) )
#else
	if ( FAILED ( m_pIB->Lock(0, 0, (BYTE**)&pIndices, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH
	{
///		MessageBox(NULL, "Error - Lock IB", "EffectMap", MB_OK);
		return	E_FAIL;
	}
#ifdef DIRECT_VERSION_9_MJH
	if ( FAILED ( m_pVB->Lock(0, 0, (void**)&pVertices, 0 ) ) )
#else
	if ( FAILED ( m_pVB->Lock(0, 0, (BYTE**)&pVertices, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH
	{
///		MessageBox(NULL, "Error - Lock VB", "EffectMap", MB_OK);
		return	E_FAIL;
	}

	for ( short ylop = y-1, v = 3; ylop < y+2; ylop ++, v -- )
	{
		for ( short xlop = x-1, u = 0; xlop < x+2; xlop ++, u ++)
		{
			pVertices[m_nVtxCount].x		= g_Map.m_pDisplayMap[ylop][xlop].Pos[0].x;
			pVertices[m_nVtxCount].y		= g_Map.m_pDisplayMap[ylop][xlop].Pos[0].y;
			pVertices[m_nVtxCount].z		= g_Map.m_pDisplayMap[ylop][xlop].Pos[0].z;
			pVertices[m_nVtxCount].color	= D3DCOLOR_ARGB(255,255,255,255);
			pVertices[m_nVtxCount].u1		= g_Map.m_pDisplayMap[ylop][xlop].Pos[0].tu;
			pVertices[m_nVtxCount].v1		= g_Map.m_pDisplayMap[ylop][xlop].Pos[0].tv;
			pVertices[m_nVtxCount].u2		= (float)(u*0.33f);
			pVertices[m_nVtxCount].v2		= (float)(v*0.33f);
			m_nVtxCount++;

			pVertices[m_nVtxCount].x		= g_Map.m_pDisplayMap[ylop][xlop].Pos[1].x;
			pVertices[m_nVtxCount].y		= g_Map.m_pDisplayMap[ylop][xlop].Pos[1].y;
			pVertices[m_nVtxCount].z		= g_Map.m_pDisplayMap[ylop][xlop].Pos[1].z;
			pVertices[m_nVtxCount].color	= D3DCOLOR_ARGB(255,255,255,255);
			pVertices[m_nVtxCount].u1		= g_Map.m_pDisplayMap[ylop][xlop].Pos[1].tu;
			pVertices[m_nVtxCount].v1		= g_Map.m_pDisplayMap[ylop][xlop].Pos[1].tv;
			pVertices[m_nVtxCount].u2		= (float)(u*0.33f);
			pVertices[m_nVtxCount].v2		= (float)((v-1)*0.33f);
			m_nVtxCount++;

			pVertices[m_nVtxCount].x		= g_Map.m_pDisplayMap[ylop][xlop].Pos[2].x;
			pVertices[m_nVtxCount].y		= g_Map.m_pDisplayMap[ylop][xlop].Pos[2].y;
			pVertices[m_nVtxCount].z		= g_Map.m_pDisplayMap[ylop][xlop].Pos[2].z;
			pVertices[m_nVtxCount].color	= D3DCOLOR_ARGB(255,255,255,255);
			pVertices[m_nVtxCount].u1		= g_Map.m_pDisplayMap[ylop][xlop].Pos[2].tu;
			pVertices[m_nVtxCount].v1		= g_Map.m_pDisplayMap[ylop][xlop].Pos[2].tv;
			pVertices[m_nVtxCount].u2		= (float)((u+1)*0.33f);
			pVertices[m_nVtxCount].v2		= (float)((v-1)*0.33f);
			m_nVtxCount++;

			pVertices[m_nVtxCount].x		= g_Map.m_pDisplayMap[ylop][xlop].Pos[3].x;
			pVertices[m_nVtxCount].y		= g_Map.m_pDisplayMap[ylop][xlop].Pos[3].y;
			pVertices[m_nVtxCount].z		= g_Map.m_pDisplayMap[ylop][xlop].Pos[3].z;
			pVertices[m_nVtxCount].color	= D3DCOLOR_ARGB(255,255,255,255);
			pVertices[m_nVtxCount].u1		= g_Map.m_pDisplayMap[ylop][xlop].Pos[3].tu;
			pVertices[m_nVtxCount].v1		= g_Map.m_pDisplayMap[ylop][xlop].Pos[3].tv;
			pVertices[m_nVtxCount].u2		= (float)((u+1)*0.33f);
			pVertices[m_nVtxCount].v2		= (float)(v*0.33f);
			m_nVtxCount++;

			pIndices[m_nIdxCount++]			= m_nVtxCount-4;
			pIndices[m_nIdxCount++]			= m_nVtxCount-3;
			pIndices[m_nIdxCount++]			= m_nVtxCount-2;
			pIndices[m_nIdxCount++]			= m_nVtxCount-2;
			pIndices[m_nIdxCount++]			= m_nVtxCount-1;
			pIndices[m_nIdxCount++]			= m_nVtxCount-4;
		}
	}

	m_pVB->Unlock();
	m_pIB->Unlock();

	return	S_OK;
}

HRESULT	CEffectMap::Update(const D3DXVECTOR3& v)
{
	return	S_OK;
}

HRESULT	CEffectMap::Update(const float& x, const float& y, const float& z)
{
	return	S_OK;
}

#ifdef DIRECT_VERSION_9_MJH
HRESULT	CEffectMap::Display(LPDIRECT3DDEVICE9 pDevice)
#else
HRESULT	CEffectMap::Display(LPDIRECT3DDEVICE8 pDevice)
#endif // DIRECT_VERSION_9_MJH
{
	g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_SELECTARG1 );
	g_RenderManager.SetTextureColorArg1( 0 , D3DTA_TEXTURE );
	
	
	g_RenderManager.SetTextureAlphaOP  ( 0 , D3DTOP_SELECTARG1);
	g_RenderManager.SetTextureAlphaArg1( 0 , D3DTA_TEXTURE);

	g_RenderManager.SetAlphaBlendEnable( TRUE );

	g_RenderManager.SetAlphaTestEnable( TRUE );
	g_RenderManager.SetAlphaRef( 0 );
	g_RenderManager.SetAlphaFunc( D3DCMP_GREATEREQUAL );

	g_RenderManager.SetTextureAddressU( 1 , D3DTADDRESS_CLAMP );
	g_RenderManager.SetTextureAddressV( 1 , D3DTADDRESS_CLAMP );
	g_RenderManager.SetTextureAddressW( 1 , D3DTADDRESS_CLAMP);

	g_RenderManager.SetTextureCoordIndex( 1 , D3DTSS_TCI_CAMERASPACEPOSITION );
	g_RenderManager.SetTextureTransFormFlags( 1 , D3DTTFF_COUNT4|D3DTTFF_PROJECTED );

	g_RenderManager.SetTextureColorOP  ( 1 , D3DTOP_MODULATE4X );
	g_RenderManager.SetTextureColorArg1( 1 , D3DTA_TEXTURE );
	g_RenderManager.SetTextureColorArg2( 1 , D3DTA_CURRENT );
	
	
	g_RenderManager.SetTextureAlphaOP  ( 1 , D3DTOP_MODULATE );
	

	g_RenderManager.SetTexture(0, g_Map.m_pTexture);
	g_RenderManager.SetTexture(1, m_pTexture);
	g_RenderManager.SetTransform(D3DTS_WORLD, &m_matPos);
	g_RenderManager.SetStreamSource(0, m_pVB, sizeof(FVF_LIGHTMAP));	
	
	g_RenderManager.SetFVF( m_dwFVF );

	g_RenderManager.SetIndices(m_pIB, 0);
	g_RenderManager.DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, m_nVtxCount, 0, m_nIdxCount/3);

	g_RenderManager.SetTextureColorOP  ( 0 ,  D3DTOP_MODULATE );
	g_RenderManager.SetTextureColorArg1( 0 ,  D3DTA_TEXTURE );
	
	g_RenderManager.SetTextureAlphaOP  ( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureAlphaArg1( 0 , D3DTA_TEXTURE );

	g_RenderManager.SetTextureColorOP  ( 1 , D3DTOP_DISABLE );
	g_RenderManager.SetTextureColorArg1( 1 , D3DTA_TEXTURE );
	
	
	g_RenderManager.SetTextureAlphaOP  ( 1 , D3DTOP_DISABLE );
	g_RenderManager.SetTextureAlphaArg1( 1 , D3DTA_TEXTURE );
	
	g_RenderManager.SetTextureCoordIndex (  1 , D3DTSS_TCI_PASSTHRU );
	g_RenderManager.SetTextureTransFormFlags( 1 , D3DTTFF_DISABLE );

	g_RenderManager.SetTextureAddressU ( 1 , D3DTADDRESS_WRAP );
	g_RenderManager.SetTextureAddressV ( 1 ,D3DTADDRESS_WRAP );
	g_RenderManager.SetTextureAddressW( 1 ,D3DTADDRESS_WRAP );
	
	return	S_OK;
}




void	CBravoMap::BuildGrassWithPoint(const short& idx, const short& listidx)
{
	float	radius = 0.0f, px = 0.0f, py = 0.0f;
//	short	inc	= 0.0f;

	GRASS_POINT *	pData	= &m_GrassPoint.m_Data[listidx];
	CGrassList	*	pNode;

	for ( BYTE y = 0; y < pData->byRange; y += 5 )
	{
		for ( BYTE x = 0; x < pData->byRange; x += 5 )
		{
			if ( rand()%10 < (pData->byDensity* g_GrsVisible / 3 ) )
			{
				pNode	= SAFE_NEW( CGrassList );

				m_nCurGrass	= idx;
				pNode->m_pData	= &g_GrassList[idx];
				pNode->m_nGrassIdx	= idx;
				pNode->m_matPos._41	= (pData->x*(TILE_SIZE/2))+x + (rand()%10) - (pData->byRange>>1);
				pNode->m_matPos._42	= GetAttrHeight(((pData->x*(TILE_SIZE/2))+px)/8, ((pData->y*(TILE_SIZE/2))+py)/8);
				pNode->m_matPos._43	= (pData->y*(TILE_SIZE/2))+y + (rand()%10)- (pData->byRange>>1);
				pData->Add(pNode);
			}
		}
	}
}

HRESULT		CBravoMap::DisplayGrass()
{
	g_Obj_Manager.InitLockState();

	g_RenderManager.SetLighting( TRUE );
	g_RenderManager.SetCullMode( D3DCULL_NONE );

	g_RenderManager.SetAlphaTestEnable( TRUE );
	g_RenderManager.SetAlphaRef( 0x00000050 );
	g_RenderManager.SetAlphaFunc( D3DCMP_GREATEREQUAL );

	g_RenderManager.SetAlphaBlendEnable( TRUE );
	g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
	g_RenderManager.SetDestBlend(  D3DBLEND_INVSRCALPHA );

	GRASS_POINT	*	pData;
	dwGrassVisibleCount	= 0;

	// character pos	( build to 0,0,0 )
	D3DXMATRIX			matCameraPos;
	D3DXMatrixIdentity(&matCameraPos);
	matCameraPos._41	= g_Pc.GetPlayer()->GetPosTM()._41 + g_Camera.m_vEye.x - g_Camera.m_vLookat.x;
	matCameraPos._42	= g_Pc.GetPlayer()->GetPosTM()._42 + g_Camera.m_vEye.y - g_Camera.m_vLookat.y;
	matCameraPos._43	= g_Pc.GetPlayer()->GetPosTM()._43 + g_Camera.m_vEye.z - g_Camera.m_vLookat.z;

#ifdef DIRECT_VERSION_9_MJH
	D3DMATERIAL9 GrsMaterial;
#else
	D3DMATERIAL8 GrsMaterial;
#endif // DIRECT_VERSION_9_MJH	
	ZeroMemory( &GrsMaterial, sizeof(GrsMaterial) );
	
	GrsMaterial.Diffuse.a = 0.9f;
	GrsMaterial.Emissive.r = 1.0f;
	GrsMaterial.Emissive.b = 1.0f;
	GrsMaterial.Emissive.g = 1.0f;
	GrsMaterial.Emissive.a = 1.0f;
	GrsMaterial.Specular.r = 1.0f;
	GrsMaterial.Specular.b = 1.0f;
	GrsMaterial.Specular.g = 1.0f;
	GrsMaterial.Specular.a = 1.0f;

	short	tx, ty, tfHeight;
	float   fDistance;
	float	_x, _y, _z;
	//char	bugstr[32];
	float	range = ( ( g_Obj_Manager.m_LodRange[0] > 700 )? 700:g_Obj_Manager.m_LodRange[0] );

	for( short i = 0; i < m_dwGrassCount; i ++ )
	{
		//여기서 뻗음
		pData	= &m_GrassPoint.m_Data[i];

		if ( pData->bIsReady )
			pData->bIsVisible	= g_Camera.CubeInFrustum(	pData->x*8, GetAttrHeight(pData->x, pData->y), pData->y*8, 
															pData->byRange * 0.7f, GetAttrHeight(pData->x,pData->y)+30.0f, pData->byRange * 0.7f);

		if( pData->bIsVisible )
		{
			tx = pData->x * 8 + 4;
			ty = pData->y * 8 + 4;
			tfHeight = g_Map.GetAttrHeight(pData->x,pData->y);

			
			// 카메라와 오브젝트거리
			_x  = (matCameraPos._41-tx)*(matCameraPos._41-tx);
			_y 	= (matCameraPos._42-tfHeight)*(matCameraPos._42-tfHeight);
			_z 	= (matCameraPos._43-ty)*(matCameraPos._43-ty);
			fDistance = sqrt(_x+_y+_z);	
		
			if( fDistance >= range ) 
			{
				pData->bIsVisible = false;
				continue;
			}
			else if( fDistance > range - 80 )
			{
				GrsMaterial.Diffuse.a = 0.9f - ( (fDistance - (range - 80.0f) ) / 80.0f) * 0.9f;
			}
			else
			{
				GrsMaterial.Diffuse.a = 0.9f;
			}

			g_RenderManager.SetMaterial(&GrsMaterial);	

			if ( pData->idx >= g_Grass.m_cCount )
			{
			}
			else
			{
				pData->Display();
				dwGrassVisibleCount	++;
			}
		}
	}

	g_RenderManager.SetCullMode( D3DCULL_CCW );

	g_RenderManager.SetAlphaTestEnable( FALSE );
	g_RenderManager.SetAlphaBlendEnable( FALSE );

	return	S_OK;
}





//-----------------------------------------------------------------------------
// Desc: 맵블럭초기화
//-----------------------------------------------------------------------------
void		CBravoMap::InitMapData()
{
	for( int x = 0; x < m_cSize*2; ++x )
	{
		for( int y = 0; y < m_cSize*2; ++y )
			m_Block[x][y].Init();
	}	
}


//-----------------------------------------------------------------------------
// Desc: 맵블럭값 리턴
//-----------------------------------------------------------------------------
int		CBravoMap::FindMapData( int _x, int _y )
{
	return m_Block[_x][_y].Attr;	
}	


//-----------------------------------------------------------------------------
// Desc: 맵블럭에 값넣기
//-----------------------------------------------------------------------------
bool		CBravoMap::SetMapData( int _nAttr, int _nIndex, long _lBlock, bool _bPC )
{
	if( _lBlock%(m_cSize*2) > (m_cSize*2) )
		return false;
	if( _lBlock/(m_cSize*2) > (m_cSize*2) )
		return false;	
	MapDataProcess( _nAttr, _nIndex, _lBlock%g_Pc.m_cSize, _lBlock/g_Pc.m_cSize, _bPC );

	return true;
}

bool		CBravoMap::SetMapData( int _nAttr, int _nIndex, int _x, int _y, bool _bPC )
{
	if( _x > m_cSize*2 )
		return false;
	if( _y > m_cSize*2 )
		return false;
	MapDataProcess( _nAttr, _nIndex, _x, _y, _bPC );

	return true;
}


//-----------------------------------------------------------------------------
// Desc: 타입에 따른 맵블럭처리
// 여러타일을 먹는 경우에.. 짝수타입은.. 왼쪽아래가 기본타일이 된다
//-----------------------------------------------------------------------------
void		CBravoMap::MapDataProcess( int _nAttr, int _nIndex, int _x, int _y, bool _bPC )
{
	//-----------------------------------------------------------------------------
	// 속성 인덱스에 따른 블락타입받기
	//-----------------------------------------------------------------------------
	BYTE	byBlockType		= 1;
	int		lTargetIndex	= 0;
	long	lTargetUnique	= 0;
		
	// PC
	if( _bPC == true )
	{
		lTargetIndex	= _nIndex;		

		if( _nIndex ==  MAX_USER_INTABLE)
		{
			lTargetUnique = g_Pc.m_MainPC.lUnique;
		}
		else
		{
			lTargetUnique = g_Pc.m_PcTable[_nIndex].lUnique;
		}
	}
	// NPC
	else
	{	
		// 신수가 낄수있어서 블럭 체크 하지않아 Boa요 By wxywxy		
		if( g_Map.m_Npc[_nIndex].m_State.nHolyState == nHolyState_Holy )
			return;
		
		// 라이딩 NPC도 블록 체크하지 않는다. By simwoosung
		if( g_Map.m_Npc[_nIndex].m_State.nRideState == nRideState_Ride )
			return;		

		lTargetIndex	= g_Map.m_NpcTable[_nIndex].index;
		lTargetUnique	= g_Map.m_NpcTable[_nIndex].lUnique;
		
		SNPCBaseInfo* npc_Info;
		npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( g_Map.m_NpcTable[_nIndex].code ); 
		byBlockType = npc_Info->theSize;
		
		///-- BY Simwoosung
		///-- 펫의 경우는 일단 낄경우를 대비해.... 1*1블록체크만 한다.
		///-- 몬스터는? 뭐 못빠져나오면 죽이고 나오면 되는데...
		///-- 거대몬스터일 경우 문제가 될지도 차후수정		
		if( g_Map.m_Npc[_nIndex].m_State.nPetState == nPetState_Pet )
		{
			byBlockType = 1;
		}

		///-- By simwoosung
		///-- 일단 몹크기가 5이상인것만 블록체크
		///-- 5이상 인것은 움직이지 않는 거대몹, 거대 NPC정도인듯... 
		///-- 움직이지 않는이상 끼는 버그는 생기지 않는다.
		///-- 이소스를 그대로 적용하면... 공격거리가 맞지 않는다고 나옴..
		///-- 서버쪽과 맞추는 작업이 필요할듯..;;;;;
		/*--if( npc_Info->theSize < 5 )
		{
			byBlockType = 1;
		}*/

		byBlockType = 1;

//		byBlockType = npc_Info->theBlockType;
//		byBlockType		= g_Pc_Manager.GetNpcBlockType( _nIndex );	/// [NiceguY] NPC Table 만들때 다시 수정...
	}

	int x = 0;
	int y = 0;
	int realX = 0;
	int realY = 0;
	//-----------------------------------------------------------------------------
	// 블락타입에 따라서 처리하기
	//-----------------------------------------------------------------------------
	
	int nBlockSize = byBlockType - 1;

	for( x = -nBlockSize; x <= nBlockSize ; ++x )
	{
		for( y = -nBlockSize; y <= nBlockSize ; ++y )
		{
			realX = _x + x;
			realY = _y + y;
			if( realX < 0 || realY < 0 || realX >= MAX_MAP_BLOCK || realY >= MAX_MAP_BLOCK )
			{
				continue;
			}
			m_Block[realX][realY].SetData( _nAttr, lTargetIndex, lTargetUnique );	
		}
	}
}

//-----------------------------------------------------------------------------
// Desc: 맵블럭 처리 함수
//-----------------------------------------------------------------------------
void	MAP_BLOCK::Init() 
{ 
	Attr	= 0;
	Index	= -1;
	Unique	= 0;
}

void	MAP_BLOCK::SetData( char _Attr, int _nIndex, long _lUnique )
{
	// 속성값에 따라서처리
	switch( _Attr )
	{
	case CLEAR_BLOCK:	// CLEAR
		{
			Init();
			/*
			if( FindTarget( _nIndex, _lUnique ) == true )
				Init();
			else
			{
				#ifdef GAMETYPE_TEST
					MessageBox( NULL, "MapBlock SetData Error1", "Error", MB_OK );
				#endif
			}
			*/
		}
		break;
	default:
		{
			Attr	= _Attr;
			Index	= _nIndex;
			Unique	= _lUnique;
			/*
			// 속성값 저장
			if( GetClear() == true )
			{
				Attr	= _Attr;
				Index	= _nIndex;
				Unique	= _lUnique;
			}
			else
			{
				#ifdef GAMETYPE_TEST
					MessageBox( NULL, "MapBlock SetData Error2", "Error", MB_OK );
				#endif
			}
			*/
		}
		break;
	}	
}


bool	MAP_BLOCK::GetClear()
{
	switch( Attr )
	{	
	case OBJ_BLOCK:		// OBJECT
		return false;
	case NPC_BLOCK:		// NPC
		return false;
	case PC_BLOCK:		// PC
		return false;
	}	
	return true;
}


bool	MAP_BLOCK::FindTarget( int _nIndex, long _lUnique )
{
	if( Index == _nIndex && Unique == _lUnique )
		return true;
	return false;
}


//..........................................................................................................
// 맵 오브젝트 + 캐릭터 소트시켜서 찍어 보아요
//..........................................................................................................
void CBravoMap::DisplaySortNode(void)

{			
	ProfileBegin("< DisplaySortNode >");
#if ( defined(BOUND_BOX_DISPLAY) || defined(HHM_DRAW_FRUSTUM_LINE) )
//#ifdef BOUND_BOX_DISPLAY || if def()
	
	g_Camera.DrawFrustumLine();

#endif

	int ISReSort = FALSE;
	
	if( GMODE_NORMAL != g_RockClient.GetGameMode() )
	{
		if( TRUE == g_Camera.m_bRotBySort )
		{
			ISReSort = TRUE;
			g_Camera.m_bRotBySort = false;
		}
	}
	else if( SAFE_TIME_COMPARE( SAFE_TIME_ADD( m_fSortTime , 500.0f ) , <= , g_nowTime ) )
	{
		// 카메라가 회전했었는지에 따라 소트한다. 
		if( true == g_Camera.m_bRotBySort )
		{
			// 카메라회전 플래그 false로 변경한다
			g_Camera.m_bRotBySort = false;
			
			ISReSort = TRUE;
		}
	}	
	
	if( SAFE_TIME_COMPARE( SAFE_TIME_ADD( g_Pc_Manager.m_lPreSortTime , 5000.0f ) , <= , g_nowTime ) &&
		GMODE_NORMAL == g_RockClient.GetGameMode()            )
	{
		static int preCX = 0;
		static int preCY = 0;

		// 위치가 이동했었는지에 따라 소트한다. 
		if( preCX == g_Pc.GetDestX() && preCY == g_Pc.GetDestY() && GMODE_NORMAL == g_RockClient.GetGameMode() )
		{
			g_Pc_Manager.m_lPreSortTime = g_nowTime;
		}
		else
		{
			// 시간저장
			g_Pc_Manager.m_lPreSortTime = g_nowTime;
			ISReSort = TRUE;
			preCX = g_Pc.GetDestX();
			preCY = g_Pc.GetDestY();
		}
	}	
	
	//......................................................................................................
	// 소 트
	//......................................................................................................
#ifdef _DEBUG		
	
	int l_nDevSkip = 0;		
	l_nDevSkip = GetPrivateProfileInt( "Game", "ObJ_Sort_Skip", 0, "./dev.ini");

	if( l_nDevSkip )
	{
		ISReSort = FALSE;
	}

#endif
	
	if( ISReSort )
	{
		m_fSortTime = g_nowTime;
		g_Pc_Manager.m_lPreSortTime = g_nowTime;

		ProfileBegin("< alp_Sort >");

#ifdef ADD_APPLYWIDE_MAP 
		sort(m_SortTable.m_SortList.begin(), m_SortTable.m_SortList.end(), NodeSort() );
#else
		//nmemb의 크기를 가지는 배열을 QuickSort한다.
		qsort( (void *)&m_SortTable.m_SortList[ 0 ],// 배열 시작 요소
			m_SortTable.GetTableMaxCount(),// 배열 크기
			sizeof(NODE_SORT_DATA),// 배열의 요소는 size크기를 가진다.
			NodeSortProcess );// 정렬에 사용될 함수인 compar의 포인터를 넘겨준다.
							  // compar함수는 더 크면 0, 작거나 같으면 0보다 큰 수를 리턴하도록 만든다. 
#endif //ADD_APPLYWIDE_MAP

		ProfileEnd("< alp_Sort >");	
	}

	//......................................................................................................
	// 출력 하기
	//......................................................................................................
	{
		NODE_SORT_DATA	*pNode = NULL;
		int				 Count = 0;
		ProfileBegin("< m_SortTable.GetTableMaxCount()_local >");	
		int				ll_TableMaxCount = m_SortTable.GetTableMaxCount();
		ProfileEnd("< m_SortTable.GetTableMaxCount()_local >");	
		
		ProfileBegin("< m_SortTable.GetTableMaxCount() >");	
		for( int i = 0 ; i <  ll_TableMaxCount; ++i )
		{
			//ProfileBegin("< m_SortTable.Get() >");	
			pNode = m_SortTable.Get( i );
			//ProfileEnd("< m_SortTable.Get() >");	

			//ProfileBegin("< m_SortTable.GetCount~() >");	
			if( Count >= m_SortTable.GetCount() )
			{
			
				//ProfileEnd("< m_SortTable.GetCount~() >");	
				break;
			}
			//ProfileEnd("< m_SortTable.GetCount~() >");
			
						
			switch( pNode->ID )
			{
				//...........................................................................................
				// NPC
				//...........................................................................................
				case ID_NPC:
					//ProfileBegin("< alp_NPC >");

					if( g_Map.m_Npc[pNode->Index].m_bIsLoaded == true && g_Map.m_Npc[pNode->Index].m_bIsVisible == true )
					{
						SetCharacterObjectRenderState();
						
						// 오브젝트 NPC 일때는 그려주지 않는다 (맵에서 한번 그려주었기 때문에...^^)
						if( g_Map.m_NpcTable[pNode->Index].code >= 5300 && g_Map.m_NpcTable[pNode->Index].code <= 5400 )
						{
							g_Map.m_Npc[pNode->Index].m_matRealRHand  = 
								g_Map.m_Npc[pNode->Index].m_matRealLHand  = 
								g_Map.m_Npc[pNode->Index].m_matRealHair   = 
								g_Map.m_Npc[pNode->Index].m_matRealbreast = 
								g_Map.m_Npc[pNode->Index].m_matRealCenter = 
								g_Map.m_Npc[pNode->Index].m_matRealLFoot  = 
								g_Map.m_Npc[pNode->Index].m_matRealRFoot  = 
								g_Map.m_Npc[pNode->Index].m_matBoneCenter = g_Map.m_Npc[pNode->Index].GetPosTM();				
						}
						else
						{
							//ProfileBegin( "g_Map.m_Npc[nIndex].AniRender" );
							g_Map.m_Npc[pNode->Index].AniRender();// NPC와 애완동물을 그린다
							//ProfileEnd( "g_Map.m_Npc[nIndex].AniRender" );
							
							//ProfileBegin( "g_Map.m_Npc[nIndex].Display" );
							if( g_RockClient.m_SceneManager.GetNPCDisplay() )
							{
								g_Map.m_Npc[pNode->Index].Display( 0, pNode->Index );
							}
							else
							{
								g_Map.m_Npc[pNode->Index].PreTextureLoad();
							}
							//ProfileEnd( "g_Map.m_Npc[nIndex].Display" );
							
							g_Map.m_Npc[pNode->Index].m_ISAniProcess = TRUE;
/*							
#ifdef BOUND_BOX_DISPLAY
							if( g_Map.m_Npc[pNode->Index].m_bIsVisible == TRUE )
								g_Map.m_Npc[pNode->Index].DisplayBound( pNode->ChildIndex );
#endif
*/							

						}
						
						EndCharacterObjectRenderState();
						
						g_RenderManager.LightEnable( 0 , TRUE );
						g_Map.m_Npc[pNode->Index].EffectSortRender();
						g_RenderManager.LightEnable( 0 , FALSE );
						
#ifdef BOUND_BOX_DISPLAY
						//by simwoosung - 바운딩 박스 렌더
						g_Map.m_Npc[pNode->Index].DisplayBoundBox();
#endif
					}
					else
					{
						// 본위치 계산 하지 않을때 m_matChr 로 대체 ( 이펙트 사용 )
						g_Map.m_Npc[pNode->Index].m_matRealRHand  = 
							g_Map.m_Npc[pNode->Index].m_matRealLHand  = 
							g_Map.m_Npc[pNode->Index].m_matRealHair   = 
							g_Map.m_Npc[pNode->Index].m_matRealbreast = 
							g_Map.m_Npc[pNode->Index].m_matRealCenter = 
							g_Map.m_Npc[pNode->Index].m_matRealLFoot  = 
							g_Map.m_Npc[pNode->Index].m_matRealRFoot  = 
							g_Map.m_Npc[pNode->Index].m_matBoneCenter = g_Map.m_Npc[pNode->Index].GetPosTM();				
						
					}
					//ProfileEnd("< alp_NPC >");
					break;	

				//...........................................................................................
				// PC
				//...........................................................................................
				case ID_PC:
					//ProfileBegin("< alp_PC >");
					if( g_Pc.m_Pc[pNode->Index].m_bIsLoaded == true && g_Pc.m_Pc[pNode->Index].m_bIsVisible == true )
					{

						if( g_Pc.m_PcTable[pNode->Index].bRender == true  
							&& g_Pc.m_Pc[pNode->Index].m_CharAlpha.GetState() != CHAR_NONE_DISPLAY )
						{

							//ProfileBegin( "m_Pc[nIndex].AniRender()" );
							g_Pc.m_Pc[pNode->Index].AniRender();// 본인 외에 다른 캐릭터들을 그린다.
							//	ProfileEnd( "m_Pc[nIndex].AniRender()" );
							
							SetCharacterObjectRenderState();

						//	ProfileBegin( "m_Pc[nIndex].Display" );
							if( g_RockClient.m_SceneManager.GetOtherPlayerDisplay() )
							{
								g_Pc.m_Pc[pNode->Index].Display( 0, pNode->Index );
							}
							else
							{
								g_Pc.m_Pc[pNode->Index].PreTextureLoad();
							}						
							
						//	ProfileEnd( "m_Pc[nIndex].Display" );
							g_Pc.m_Pc[pNode->Index].m_ISAniProcess = TRUE;
		
							// Sword Tail
							SetSwordTailRenderState();

							if( g_RockClient.m_SceneManager.GetOtherPlayerDisplay() )
							{
								if( !g_Pc.m_Pc[pNode->Index].DisplayAfterImage() )
								{
									// 검기 지우기 ( By wxywxy )
									g_Pc.m_Pc[pNode->Index].m_SwordTail.Clear();
								}
							}
							else
								g_Pc.m_Pc[pNode->Index].m_SwordTail.Clear();


							EndCharacterObjectRenderState();

							g_RenderManager.LightEnable( 0 , TRUE );
							g_Pc.m_Pc[pNode->Index].EffectSortRender();
							g_RenderManager.LightEnable( 0 , FALSE );
							
							//by simwoosung - 바운딩 박스 렌더
							//g_Pc.m_Pc[pNode->Index].DisplayBoundBox();							
						}
						else if(g_Pc.m_Pc[pNode->Index].m_CharAlpha.GetState() == CHAR_NONE_DISPLAY)
						{
							g_Pc.m_Pc[pNode->Index].PreTextureLoad();
						}						
					}
					else
					{
						g_Pc.m_Pc[pNode->Index].m_matRealRHand  = 
						g_Pc.m_Pc[pNode->Index].m_matRealLHand  = 
						g_Pc.m_Pc[pNode->Index].m_matRealHair   = 
						g_Pc.m_Pc[pNode->Index].m_matRealbreast = 				
						g_Pc.m_Pc[pNode->Index].m_matRealCenter = 
						g_Pc.m_Pc[pNode->Index].m_matRealLFoot  = 
						g_Pc.m_Pc[pNode->Index].m_matRealRFoot  = 
						g_Pc.m_Pc[pNode->Index].m_matBoneCenter = g_Pc.m_Pc[pNode->Index].GetPosTM();				

					//	ProfileEnd("< alp_PC >");
						continue;
					}
					
			
				//	ProfileEnd("< alp_PC >");
					break;	

				//...........................................................................................
				// OBJ
				//...........................................................................................
				case ID_OBJ:	
					{
			//		ProfileBegin("< alp_Object >");
		//			ProfileBegin("< alp_Object_d >");
		//			ProfileBegin("< alp_Object_h >");
		//			ProfileBegin("< alp_Object_f >");
			//		ProfileEnd("< alp_Object_f >");
			//		ProfileEnd("< alp_Object_h >");
					SetMapObjectRenderState();

					if( g_RockClient.m_SceneManager.GetOBJDisplay() )
					{
						m_Object[pNode->Index].Display( pNode->ChildIndex );// 배경 오브젝트를 그린다
					}
					
#ifdef BOUND_BOX_DISPLAY

					if( !m_Object[pNode->Index].m_CollisionBox.m_bIsReady &&
						m_Object[pNode->Index].m_pAniPos[pNode->ChildIndex].m_bIsVisible == TRUE )
					{
						m_Object[pNode->Index].m_CollisionBox.InitDeviceObjects( m_pDevice );
						m_Object[pNode->Index].m_CollisionBox.RestoreDeviceObjects();
						m_Object[pNode->Index].m_CollisionBox.Update( m_Object[pNode->Index].GetBoundMinPos() , m_Object[pNode->Index].GetBoundMaxPos() );
					}
					
//					if( m_Object[pNode->Index].m_pAniPos[pNode->ChildIndex].m_bIsVisible == TRUE )
//						m_Object[pNode->Index].DisplayBound( pNode->ChildIndex );

					//.......................................................................................
					// 계산된 바운딩 박스 By wxywxy
					//.......................................................................................
					FVF_PD	Vertices[ 36 ];
			
					int BoxIndex[] = {
						0 , 1 ,
						1 , 3 ,
						3 , 2 ,
						0 , 2 ,
						4 , 5 ,
						5 , 7 ,
						7 , 6 ,
						4 , 6 ,
						1 , 5 ,
						3 , 7 ,
						0 , 4 ,
						2 , 6
					};

					int Count = 0;
					D3DXCOLOR Color;

					for( int i =0 ; i < 12 ; ++i )
					{
						Vertices[ Count     ].v = m_Object[pNode->Index].m_pAniPos[pNode->ChildIndex].m_vBoundList[ BoxIndex[ Count     ] ];
						Vertices[ Count + 1 ].v = m_Object[pNode->Index].m_pAniPos[pNode->ChildIndex].m_vBoundList[ BoxIndex[ Count + 1 ] ];
					
						if( pNode->Index == 0 && pNode->ChildIndex == 0 )
						{
							Color = D3DCOLOR_XRGB( 255 , 0 , 0 );
						}
						else if( pNode->Index == 0 && pNode->ChildIndex == 1 )
						{
							Color = D3DCOLOR_XRGB( 0 , 255 , 0 );
						}
						else if( pNode->Index == 1 && pNode->ChildIndex == 0 )
						{
							Color = D3DCOLOR_XRGB( 0 , 0 , 255 );
						}
						else if( pNode->Index == 1 && pNode->ChildIndex == 1 )
						{
							Color = D3DCOLOR_XRGB( 255 , 255 , 255 );
						}
						else
						{
							Color = D3DCOLOR_XRGB( 0 , 0 , 0 );
						}

						
						Vertices[ Count     ].color = Color;
						Vertices[ Count + 1 ].color = Color;

						Count += 2;
					}

					g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_SELECTARG1 );
					g_RenderManager.SetTextureColorArg1( 0 , D3DTA_TEXTURE  );
					g_RenderManager.SetTextureColorArg2( 0 , D3DTA_DIFFUSE );
					
					g_RenderManager.SetTextureAlphaOP  ( 0 , D3DTOP_SELECTARG1 );
					g_RenderManager.SetTextureAlphaArg1( 0 , D3DTA_TEXTURE );
					g_RenderManager.SetTextureAlphaArg2( 0 , D3DTA_DIFFUSE );					
					
					g_RenderManager.SetTexture( 0 , NULL );
					g_RenderManager.SetTransform(D3DTS_WORLD, &g_Particle.m_matIdentity );

					g_RenderManager.SetLighting( FALSE );
					g_RenderManager.SetFillMode( D3DFILL_WIREFRAME );
					g_RenderManager.SetFVF(D3DFVF_PD);
					
					g_RenderManager.DrawPrimitiveUP(D3DPT_LINELIST , 12, Vertices,sizeof(FVF_PD) );
					
					g_RenderManager.SetFillMode( D3DFILL_SOLID );
					g_RenderManager.SetLighting( TRUE );

					//.......................................................................................



#endif					
					EndMapObjectRenderState();
				//	ProfileEnd("< alp_Object_d >");
			//		ProfileEnd("< alp_Object >");
					}

					break;	
				
				//...........................................................................................
				// PLAYER
				//...........................................................................................
				case ID_PLAYER:
			//		ProfileBegin("< alp_Player >");
					if( g_Pc.m_MainPC.bRender == FALSE || g_RockClient.GetGameMode() != GMODE_NORMAL )
					{
			//			ProfileEnd("< alp_Player >");
							continue;
					}					


					if( g_Pc.GetPlayer()->m_CharAlpha.GetState() != CHAR_NONE_DISPLAY )
					{


						g_Pc.GetPlayer()->AniRender();

						SetCharacterObjectRenderState();

						if( g_RockClient.m_SceneManager.GetPlayerDisplay() &&
							g_Pc.GetPlayer()->IsMyPlayerRender )
							g_Pc.GetPlayer()->Display();
							
						g_Pc.GetPlayer()->m_ISAniProcess = TRUE;
					}
					
					// Sword Tail
					SetSwordTailRenderState();
					
					if( g_RockClient.m_SceneManager.GetPlayerDisplay() &&
						g_Pc.GetPlayer()->IsMyPlayerRender )
					{
						if( !g_Pc.GetPlayer()->DisplayAfterImage() )
						{
							// 검기 지우기 ( By wxywxy )
							g_Pc.GetPlayer()->m_SwordTail.Clear();
						}
					}
					else
						g_Pc.GetPlayer()->m_SwordTail.Clear();


					EndCharacterObjectRenderState();

					g_RenderManager.LightEnable( 0 , TRUE );
					g_Pc.GetPlayer()->EffectSortRender();
					g_RenderManager.LightEnable( 0 , FALSE );
					
				//	ProfileEnd("< alp_Player >");
					break;	
				
				//...........................................................................................
				// None
				//...........................................................................................
				case ID_NONE :
					continue;
			}

			Count++;
		}
		ProfileEnd("< m_SortTable.GetTableMaxCount() >");	
	
	}
	ProfileEnd("< DisplaySortNode >");				
}

//..........................................................................................................
// SetMapObjectRenderState
//..........................................................................................................
void CBravoMap::SetMapObjectRenderState(void)
{
	g_RenderManager.SetMaterial( &g_Pc_Manager.theObject_Material );	
	g_RenderManager.SetLight( 0, &g_Particle.m_WeatherDrop.m_Object_Light );

	g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureColorArg1( 0 , D3DTA_TEXTURE );
	g_RenderManager.SetTextureColorArg2( 0 , D3DTA_DIFFUSE );
	
	
	g_RenderManager.SetTextureAlphaOP  ( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureAlphaArg1( 0 , D3DTA_CURRENT );
	g_RenderManager.SetTextureAlphaArg2( 0 , D3DTA_TEXTURE );

	g_RenderManager.DisableTextureStageState( 1 );
	g_RenderManager.DisableTextureStageState( 2 );
		
	g_RenderManager.SetLighting( TRUE );
	
	if( g_ISObjTexLoad )
	{
		g_RenderManager.SetAlphaTestEnable( TRUE );
		g_RenderManager.SetAlphaRef( 0x00 );
		g_RenderManager.SetAlphaFunc( D3DCMP_NOTEQUAL );
	}
	else
	{	
		g_RenderManager.SetAlphaTestEnable( FALSE );
	}

	g_RenderManager.SetZWriteEnable( TRUE );

	g_RenderManager.SetZEnable( TRUE );

	g_RenderManager.SetZFunc( D3DCMP_LESSEQUAL );
	g_RenderManager.SetCullMode( D3DCULL_NONE );
	
	g_RenderManager.SetFVF( D3DFVF_OBJECTVERTEX );	
	
	g_RenderManager.SetAlphaBlendEnable( TRUE );
	g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
	g_RenderManager.SetDestBlend( D3DBLEND_INVSRCALPHA );
	g_RenderManager.LightEnable( 0 , TRUE );
}

//..........................................................................................................
// EndMapObjectRenderState
//..........................................................................................................
void CBravoMap::EndMapObjectRenderState(void)
{
	g_RenderManager.SetAlphaBlendEnable( FALSE );
	g_RenderManager.SetAlphaTestEnable( FALSE );
	g_RenderManager.SetZFunc( D3DCMP_LESSEQUAL );
	g_RenderManager.SetAlphaRef( 0 );
	g_RenderManager.SetLighting( FALSE );
	g_RenderManager.LightEnable( 0 , FALSE );
}
	
//..........................................................................................................
// SetCharacterObjectRenderState
//..........................................................................................................
void CBravoMap::SetCharacterObjectRenderState(void)
{
		
	g_RenderManager.SetLight( 0, &g_Particle.m_WeatherDrop.m_Character_Light );
	g_RenderManager.LightEnable( 0 , TRUE );

	g_RenderManager.SetMaterial( &g_Pc_Manager.theCharacter_Material );

	g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureColorArg1( 0 , D3DTA_DIFFUSE );
	g_RenderManager.SetTextureColorArg2( 0 , D3DTA_TEXTURE );	
	
	g_RenderManager.SetTextureAlphaOP  ( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureAlphaArg1( 0 , D3DTA_CURRENT );
	g_RenderManager.SetTextureAlphaArg2( 0 , D3DTA_TEXTURE );
	
	g_RenderManager.DisableTextureStageState( 1 );
	g_RenderManager.DisableTextureStageState( 2 );
	
	g_RenderManager.SetFVF( D3DFVF_OBJECTVERTEX );	
	
	g_RenderManager.SetAlphaBlendEnable( TRUE );
	g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
	g_RenderManager.SetDestBlend( D3DBLEND_INVSRCALPHA );
	
	///-- 캐릭터 유닛에 알파테스팅을 적용한다.
	///-- 캐릭이 죽을시에는 알파값을 줄이게 되므로 알파테스팅을 꺼줘야 한다.
	g_RenderManager.SetAlphaTestEnable( TRUE );
	g_RenderManager.SetAlphaRef( 0xA0 );
	g_RenderManager.SetAlphaFunc( D3DCMP_GREATER );

	g_RenderManager.SetCullMode( D3DCULL_NONE );
	g_RenderManager.SetLighting( TRUE );	
}

//..........................................................................................................
// EndCharacterObjectRenderState
//..........................................................................................................
void CBravoMap::EndCharacterObjectRenderState(void)
{
	g_RenderManager.SetCullMode( D3DCULL_CCW );
	g_RenderManager.SetAlphaBlendEnable( FALSE );
	g_RenderManager.SetAlphaTestEnable( FALSE );
	g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
	g_RenderManager.SetDestBlend( D3DBLEND_INVSRCALPHA );

	g_RenderManager.SetLighting( TRUE );
	g_RenderManager.SetZWriteEnable( TRUE );
	g_RenderManager.SetLighting( FALSE );
	g_RenderManager.LightEnable( 0 , FALSE );
}

//..........................................................................................................
// SetSwordTailRenderState
//..........................................................................................................
void CBravoMap::SetSwordTailRenderState(void)
{
	D3DXMATRIX	matWorld;
	D3DXMatrixIdentity( &matWorld );

	g_RenderManager.SetTransform( D3DTS_WORLD, &matWorld );

	g_RenderManager.SetLighting( FALSE );

	g_RenderManager.SetZWriteEnable( FALSE );
	g_RenderManager.SetZEnable( TRUE );

	g_RenderManager.SetTextureMipFilter ( 0 , D3DTEXF_LINEAR );
	g_RenderManager.SetTextureMagFilter ( 0 , D3DTEXF_LINEAR );
	g_RenderManager.SetTextureMinFilter ( 0 , D3DTEXF_LINEAR );

	g_RenderManager.SetCullMode( D3DCULL_NONE );

	g_RenderManager.SetAlphaBlendEnable( TRUE );
	g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
	g_RenderManager.SetDestBlend( D3DBLEND_DESTALPHA );
/*
	g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureColorArg1( 0 , D3DTA_TEXTURE );
	g_RenderManager.SetTextureColorArg2( 0 , D3DTA_DIFFUSE );


	g_RenderManager.SetTextureAlphaOP  ( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureAlphaArg1( 0 , D3DTA_TEXTURE );
	g_RenderManager.SetTextureAlphaArg2( 0 , D3DTA_DIFFUSE );
*/
	g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_SELECTARG1 );
	g_RenderManager.SetTextureColorArg1( 0 , D3DTA_TEXTURE );

	g_RenderManager.SetTextureAlphaOP  ( 0 , D3DTOP_SELECTARG1 );
	g_RenderManager.SetTextureAlphaArg1( 0 , D3DTA_DIFFUSE );

	g_RenderManager.SetZFunc( D3DCMP_LESSEQUAL );

	g_RenderManager.DisableTextureStageState( 1 );
	g_RenderManager.DisableTextureStageState( 2 );
}

void CBravoMap::SetShadowMakeRenderState()
{
	// 그림자텍스쳐설정	
	g_RenderManager.SetTexture( 0, NULL );

	g_RenderManager.SetTFactorColor( D3DCOLOR_ARGB(150, 150, 150, 150) );

	g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_SELECTARG1 );
	g_RenderManager.SetTextureColorArg1( 0 , D3DTA_TFACTOR );
	
	
	g_RenderManager.SetTextureAlphaOP  ( 0 , D3DTOP_SELECTARG1 );
	g_RenderManager.SetTextureAlphaArg1( 0 , D3DTA_TFACTOR );

	g_RenderManager.SetAlphaBlendEnable( FALSE );

	// 메트릭스설정
	D3DXMATRIX	matInit;
	D3DXMatrixIdentity( &matInit );
	g_RenderManager.SetTransform( D3DTS_VIEW,	&matInit );	
	g_RenderManager.SetTransform( D3DTS_WORLD,	&matInit );	
	
	g_RenderManager.SetFVF( D3DFVF_OBJECTVERTEX );

}

void CBravoMap::EndShadowMakeRenderState()
{
	// 메트릭스복구	
	g_RenderManager.SetTransform( D3DTS_VIEW,		&g_Camera.m_matView );	
	g_RenderManager.SetTransform( D3DTS_PROJECTION,	&g_Camera.m_matProj );	
	
	// 그림자텍스쳐초기
	g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureColorArg1( 0 , D3DTA_TEXTURE );
	
	
	g_RenderManager.SetTextureAlphaOP  ( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureAlphaArg1( 0 , D3DTA_TEXTURE );
	
	g_RenderManager.SetAlphaBlendEnable( TRUE );
}

void CBravoMap::SetShadowDisplayRenderState()
{
	g_RenderManager.SetAlphaTestEnable( TRUE );
	g_RenderManager.SetAlphaFunc( D3DCMP_ALWAYS );
	g_RenderManager.SetAlphaBlendEnable( TRUE );

	// 그림자타엡에 따라서 셋팅
	switch( g_Shadow )
	{
	case 1:
		{
			g_RenderManager.SetSrcBlend( D3DBLEND_ZERO );
			g_RenderManager.SetDestBlend( D3DBLEND_INVSRCALPHA );
			
			g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_SELECTARG1 );
			g_RenderManager.SetTextureColorArg1( 0 , D3DTA_TEXTURE );
			
			
			g_RenderManager.SetTextureAlphaOP  ( 0 , D3DTOP_MODULATE );
			g_RenderManager.SetTextureAlphaArg1( 0 , D3DTA_TEXTURE );

			g_RenderManager.SetTextureAddressU( 0 , D3DTADDRESS_CLAMP );
			g_RenderManager.SetTextureAddressV( 0 , D3DTADDRESS_CLAMP );
			g_RenderManager.SetTextureAddressW( 0 , D3DTADDRESS_CLAMP );
			
			g_RenderManager.SetTextureCoordIndex(  0 , D3DTSS_TCI_CAMERASPACEPOSITION );
			g_RenderManager.SetTextureTransFormFlags( 0 , D3DTTFF_COUNT4|D3DTTFF_PROJECTED );
		}
		break;
	case 2:
		{
			g_RenderManager.SetSrcBlend( D3DBLEND_ZERO );
			g_RenderManager.SetDestBlend( D3DBLEND_SRCCOLOR );
			
			g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_SELECTARG1 );
			g_RenderManager.SetTextureColorArg1( 0 , D3DTA_TEXTURE );
			
			
			g_RenderManager.SetTextureAlphaOP  ( 0 , D3DTOP_SELECTARG1 );
			g_RenderManager.SetTextureAlphaArg1( 0 , D3DTA_TEXTURE );

			g_RenderManager.SetTextureAddressU( 0 , D3DTADDRESS_CLAMP );
			g_RenderManager.SetTextureAddressV( 0 , D3DTADDRESS_CLAMP );
			g_RenderManager.SetTextureAddressW( 0 , D3DTADDRESS_CLAMP );
			
			g_RenderManager.SetTextureCoordIndex(  0 , D3DTSS_TCI_CAMERASPACEPOSITION );
			g_RenderManager.SetTextureTransFormFlags( 0 , D3DTTFF_COUNT4|D3DTTFF_PROJECTED );
		}
		break;
	}
	
	g_RenderManager.SetTexture( 0 , NULL );
	g_RenderManager.SetTexture( 1 , NULL );
}

void CBravoMap::EndShadowDisplayRenderState()
{
	g_RenderManager.SetTexture( 1, NULL );

	g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
	g_RenderManager.SetDestBlend( D3DBLEND_INVSRCALPHA );
	
	g_RenderManager.SetZWriteEnable( TRUE);
	g_RenderManager.SetZEnable( TRUE );

	g_RenderManager.SetTextureCoordIndex( 0 , 0 );
	g_RenderManager.SetTextureTransFormFlags( 0 , D3DTTFF_DISABLE );

	g_RenderManager.SetTextureCoordIndex( 1 , 1 );
	g_RenderManager.SetTextureTransFormFlags( 1 , D3DTTFF_DISABLE );
	
	g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureColorArg1( 0 , D3DTA_TEXTURE );
	g_RenderManager.SetTextureColorArg2( 0 , D3DTA_DIFFUSE );
	
	g_RenderManager.SetTextureAddressU( 0 , D3DTADDRESS_WRAP );
	g_RenderManager.SetTextureAddressV( 0 , D3DTADDRESS_WRAP );
	g_RenderManager.SetTextureAddressW( 0 , D3DTADDRESS_WRAP );

	g_RenderManager.SetAlphaFunc( D3DCMP_LESSEQUAL );
}

int CBravoMap::CheckPointInBox(D3DXVECTOR3 *pVec,D3DXPLANE *pPlane)
{
	for(int i = 0; i < 6; i++ )
	{
		if( D3DXPlaneDotCoord( &pPlane[ i ] , pVec ) > 0 )
		{
			return false;
		}
	}
	
	return true;
}	


