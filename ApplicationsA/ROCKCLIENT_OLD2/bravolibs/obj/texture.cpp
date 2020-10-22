#include "RockPCH.h"

#include	<ddraw.h>
#include	"..\brother.h"

#include	<NETWORK\Protocol.h>
#include	<Base\\D3DBase.h>
#include	"..\\quadlist.h"
#include	"..\\RockClient.h"
#include	"Define.H"
#include	"Texture.H"
#include	"TextureManager.H"
#include	"Player_Manager.H"
#include	<stdio.H>
#include	<assert.H>

#include	"VFileSystem.h"	/// sooree pack 
#include	"profile\profile.h"
#include	"profile\custom_time.h"

#include "..\\global.h"
#include "..\\..\\CRenderManager.h"

extern int g_nTextureDetail;
extern bool g_IsFrameOptimize;

extern BOOL			g_ISObjTexLoad;

#define		MIPMAP_LEVEL		1
#define		TEXTURE_USE_TIME	30000	// 텍스쳐 사용 검사시간(20초)

extern		long				g_nowTime;
#ifdef DIRECT_VERSION_9_MJH
extern		LPDIRECT3DDEVICE9	g_lpDevice;
#else
extern		LPDIRECT3DDEVICE8	g_lpDevice;
#endif // DIRECT_VERSION_9_MJH
extern		TEXTURE_MANAGER		g_TexManager;


///----------------------------------------------------------------------------
///-- Constructor 
///----------------------------------------------------------------------------
void Texture::Clear()
{	
	m_byAttr		= 0;
	m_byType		= 0; 
	m_pD3DTex		= NULL;
	m_bIsLoaded		= false;
	m_bIsLoading	= false;
	m_lPreTime		= g_nowTime;	
}

///----------------------------------------------------------------------------
///-- Destructor
///----------------------------------------------------------------------------
void Texture::Destory()
{
	if( true == m_bIsLoaded && false == m_bIsLoading )
	{		
		if( m_pD3DTex != NULL )
		{
			m_pD3DTex->Release();
			m_pD3DTex	= NULL;			
		}
		
		m_bIsLoaded		= false;
		m_bIsLoading	= false;
		m_lPreTime		= g_nowTime;
	}
}


//-----------------------------------------------------------------------------
// Desc: 텍스쳐 읽기
//-----------------------------------------------------------------------------
bool Texture::Read( char* _szFilePath, int type , BOOL ISSkip )
{		
	if( ISSkip )
	{		
		m_pD3DTex = NULL;
		m_bIsLoading	= false;
		m_bIsLoaded		= true;	
		m_lPreTime		= g_nowTime;       		
		
		return true;
	}

	if( false == m_bIsLoaded && false == m_bIsLoading && NULL == m_pD3DTex )
	{
		// 로딩중 플래그 변경
		m_bIsLoading = true;		

		HRESULT hr;

		FILE*	file = NULL;
		if( NULL == (file = fopen( _szFilePath, "rb" )) )	
		{
			CloseWnd_Error( "!%s 파일을 찾을 수 없습니다.", _szFilePath );			
			return false;
		}

		if( type == n_Texture )
		{
			// 텍스쳐 메모리 할당
			hr = g_RenderManager.CreateTextureFromFileEx( _szFilePath, D3DX_DEFAULT, D3DX_DEFAULT, 
															MIPMAP_LEVEL , 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
			//												D3DX_FILTER_LINEAR, D3DX_FILTER_LINEAR,
															D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
															D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
			//												0xff000000, NULL, NULL, &m_pD3DTex );
															0, NULL, NULL, &m_pD3DTex );
		}
		else if( type == n_Interface )
		{
			hr = g_RenderManager.CreateTextureFromFileEx(	_szFilePath, D3DX_DEFAULT, D3DX_DEFAULT, 
															0, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
															D3DX_FILTER_NONE, D3DX_FILTER_NONE,
			//												0xff000000, NULL, NULL, &m_pD3DTex );
															0, NULL, NULL, &m_pD3DTex );
		}
	
		
		// 텍스쳐로딩 성공
		if( 0 == hr )
		{
			m_bIsLoading	= false;
			m_bIsLoaded		= true;	
			m_lPreTime		= g_nowTime;
		}
		// 텍스쳐로딩 실패
		else
		{
			m_bIsLoaded		= false;
			m_bIsLoading	= false;

			return false;
		}

		return true;
	}

	return true;
}

///----------------------------------------------------------------------------
///-- Desc: 텍스쳐 읽기
///----------------------------------------------------------------------------
///-- 2005.03.15 / Lyul
///-- a_bDevSkip 일 경우 Texture 를 Loading 하지 않는다. Default 0

bool Texture::ReadVF( CVFileSystem* pVF, char* _szFilePath, int type, int a_nDevSkip ) 	/// sooree pack
{
	///-- Texture Loading Skip / 2005.03.15 / Lyul
	if( a_nDevSkip )
	{		
		m_pD3DTex = NULL;
		m_bIsLoading	= false;
		m_bIsLoaded		= true;	
		m_lPreTime		= g_nowTime;       		
		return true;
	}

	if( false == m_bIsLoaded && false == m_bIsLoading && NULL == m_pD3DTex )
	{
		// 로딩중 플래그 변경
		m_bIsLoading = true;	

		HRESULT hr = -1;

		// 파일포인터 열기
		VFileHandle* pFH = pVF->OpenFile( _szFilePath );

		UINT uWidth = D3DX_DEFAULT;
		UINT uHeight = D3DX_DEFAULT;

		D3DFORMAT fFormat = D3DFMT_UNKNOWN;
		
		if( pFH )
		{
			D3DXIMAGE_INFO imageinfo;			
			
			if( FAILED( D3DXGetImageInfoFromFileInMemory( pFH->GetData(), pFH->GetSize(), &imageinfo ) ) )
			{       
				return true;
			}
			uWidth = imageinfo.Width;
			uHeight = imageinfo.Height;

			fFormat = imageinfo.Format;

			int nLevel = pow((float)2, (float)g_nTextureDetail);
			uWidth /= nLevel; 
			uHeight /= nLevel;		
		}

		if( pFH )
		{						
			if( type == n_Texture )
			{
				///--PRINT_DLG_LOG( "텍스쳐 로딩" );						
					
				DWORD aFilter = D3DX_FILTER_LINEAR;
				if( g_nTextureDetail == 0 )
				{
					aFilter = D3DX_FILTER_NONE;
				}
				
				///--DWORD dTime = timeGetTime();	
				
				int nMipMapLevel = 2;
				
				if(g_IsFrameOptimize)
				{
					nMipMapLevel = 1;
				}

				///-- By simwoosung
				///-- 텍스쳐 밉맵 1로 하면 텍스쳐 로딩렉이 개선되지만...
				///-- 밉맵이 만들어지지 않아. 원거리에서는 도트가 튀는 현상이 일어난다.
				///-- 현재 텍스쳐 하의레벨을 텍스쳐내에 미리만들어서 적용할 예정이지만...
				///-- 아직은 구현되있지 않아.. 1레벨의 밉맵 텍스쳐를 그냥 사용한다.
				nMipMapLevel = 1;

				/*BYTE * pData = (BYTE *)pFH->GetData();
				DWORD dwMagic = 0;

				memcpy( &dwMagic, pData, sizeof(DWORD) );
				pData += sizeof(DWORD);

				DDSURFACEDESC2 ddSuface2;
				ZeroMemory( &ddSuface2, sizeof( DDSURFACEDESC2));
				memcpy( &ddSuface2, pData, sizeof(DDSURFACEDESC2) );*/
				
				// 텍스쳐 메모리 할당
				hr = g_RenderManager.CreateTextureFromFileInMemoryEx( pFH->GetData(), pFH->GetSize(),
																	uWidth, uHeight,
																	//64, 64,
																	nMipMapLevel, 0, fFormat, D3DPOOL_MANAGED,
					//												D3DX_FILTER_LINEAR, D3DX_FILTER_LINEAR,
																	//D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
																	//D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
																	aFilter ,
																	D3DX_DEFAULT,
					//		 										0xff000000, NULL, NULL, &m_pD3DTex );
																	0, NULL, NULL, &m_pD3DTex );
				
				///--PRINT_DLG_LOG("텍스쳐 로딩 경과시간 %u", SAFE_TIME_SUB( timeGetTime() , dTime ) );

			}
			else if( type == n_Interface )
			{
				hr = g_RenderManager.CreateTextureFromFileInMemoryEx( pFH->GetData(), pFH->GetSize(),
																	D3DX_DEFAULT, D3DX_DEFAULT, 
																	1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
																	D3DX_FILTER_NONE, D3DX_FILTER_NONE,
					//												0xff000000, NULL, NULL, &m_pD3DTex );
																	0, NULL, NULL, &m_pD3DTex );
			}
			pVF->CloseFile( pFH );		
		}
		else
		{
			///-- By simwoosung
			///-- CVFileSystem내부의 크리티컬 섹션으로 인해 그냥 건너뛸수도 있다.
			///--PRINT_DLG_LOG( "!%s 파일을 찾을 수 없습니다.", _szFilePath );
			//CloseWnd_Error( "!%s 파일을 찾을 수 없습니다.", _szFilePath );
			m_bIsLoaded		= false;
			m_bIsLoading	= false;
			return false;
		}
		
		// 텍스쳐로딩 성공
		if( 0 == hr )
		{
			if( m_pD3DTex == NULL )
			{
				int a = 0;
			}
			
			m_bIsLoading	= false;
			m_bIsLoaded		= true;	
			m_lPreTime		= g_nowTime;
		}
		// 텍스쳐로딩 실패
		else
		{
			m_bIsLoaded		= false;
			m_bIsLoading	= false;
			
			return false;
		}

		return true;
	}
	
	return true;
}


//-----------------------------------------------------------------------------
// Desc: 텍스쳐셋팅
//
// 1. 사용하려는 텍스쳐가 메모리할당되었는지 검사하기
// 할당되어있다면 당연히 사용하고 현재시간을 저장하여서 텍스쳐재할당처리때 이용
// 2. 메모리할당되어 있지 않다면 텍스쳐관리자에 해당 텍스쳐인덱스를 보내서,
// 텍스쳐관리자에서 메모리할당하도록 한다
//
// Copyright (c) 2003.10.20 By KStaR79
//-----------------------------------------------------------------------------
bool Texture::SetTexture( BYTE byType, int Index )
{		
	m_byType = byType; 
	
	if( Index >= c_Chr_Tex_Count+c_Npc_Tex_Count )
	{
		RLG1( "Texture Not Loading <FileNum = %d>", Index );
		return false;
	}	

	//-----------------------------------------------------------------------------
	// 게임중
	//-----------------------------------------------------------------------------
	bool bLoaded = false, bLoading = false;
	if( GMODE_NORMAL == g_RockClient.GetGameMode() )
	{		
		bLoaded		= m_bIsLoaded;
		bLoading	= m_bIsLoading;	

		// 로딩중이면 실패
		if( true == bLoading )
		{
			return false;
		}
		else
		{
			// 로딩완료. 텍스쳐셋팅. 현재시간저장
			if( true == bLoaded )
			{
				g_RenderManager.SetTexture( 0, m_pD3DTex );	
				m_lPreTime = g_nowTime;
			}
			// 텍스쳐생성하도록 타입, 인덱스 넘기기
			else
			{
				DATA_CREATE_ATTR *pCreateTexAttr = NULL;
				pCreateTexAttr = SAFE_NEW( DATA_CREATE_ATTR );				
				if( pCreateTexAttr )
				{
					bool bFlag = false;
					bool bCreate = false;

					EnterCriticalSection( &g_TexManager.m_csTextureFlag );					
					
					bCreate = true;
					std::list <DATA_CREATE_ATTR *>::iterator i = g_TexManager.theCreateTex.begin();
					if(i != g_TexManager.theCreateTex.end())
					{
						bFlag = true;
					}
				
					if(bFlag)
					{
						for( ; i != g_TexManager.theCreateTex.end();  )
						{
							DATA_CREATE_ATTR * pData = (*i);
							
							if( pData && ( pData->byType == byType ) && ( pData->Index == Index ) )
							{
								bCreate = false;
								break;
							}

							++i;					
						}
					}
					
					bool InsertFlag = false;				
					
					if(bCreate)
					{
						pCreateTexAttr->byType = byType;
						pCreateTexAttr->Index  = Index;
						g_TexManager.theCreateTex.push_back( pCreateTexAttr );
						InsertFlag = true;
					}

					LeaveCriticalSection( &g_TexManager.m_csTextureFlag );

					if( !bCreate || !InsertFlag )
					{
						SAFE_DELETE(pCreateTexAttr);
					}
				}
			}
		}
	}
	//-----------------------------------------------------------------------------
	// 예외
	//-----------------------------------------------------------------------------
	else
	{
		bLoaded		= m_bIsLoaded;
		bLoading	= m_bIsLoading;

		if( bLoaded )
		{
			m_lPreTime	= g_nowTime;
			g_RenderManager.SetTexture( 0, m_pD3DTex );
		}
		else if( true == UpdateTexture(byType, Index) )
		{
			bLoaded		= true;
			m_lPreTime	= g_nowTime;
			g_RenderManager.SetTexture( 0, m_pD3DTex );
		}	
	}
	
	return bLoaded;	
}



//-----------------------------------------------------------------------------
// Desc: 텍스쳐 사용하는지 검사
//
// 텍스쳐 타입과 파일인덱스를 받아서 텍스쳐를 생성한다.
//
// Copyright (c) 2003.10.20 By KStaR79
//-----------------------------------------------------------------------------
///-- 2005.04.01 / Lyul / DEVSkip 을 구현하나 속도에 민감할 수 있다 Define 사용

#define _DEV_SKIP 

bool Texture::UpdateTexture( BYTE byType, int Index )
{
	int l_nDevSkip = 0;
	/// sooree pack
	char szFilePath[256];
	switch( byType )
	{
		///-------------------------------------------------------------------
		///-- NPC
	case 0:				
#if !defined( LOAD_VF )
		sprintf( szFilePath, "%s\\Npc\\Tex\\%08d.dds", g_RockClient.GetRootDir(), g_Pc_Manager.m_pTex[Index]->m_Index );
		return( Read( szFilePath, n_Texture ) );
#else	

#ifdef _DEV_SKIP
		l_nDevSkip = GetPrivateProfileInt( "Texture", "NPC_Skip", 0, "./dev.ini");
#endif
		sprintf( szFilePath, "%08d.dds", g_Pc_Manager.m_pTex[Index]->m_Index );
		return( ReadVF( &g_VFNpcTex, szFilePath, n_Texture, l_nDevSkip ) );
#endif
		break;

		///-------------------------------------------------------------------
		///-- CHR
	case 1:		
#if !defined( LOAD_VF )
		sprintf( szFilePath, "%s\\Chr\\Tex\\%08d.dds", g_RockClient.GetRootDir(), g_Pc_Manager.m_pTex[Index]->m_Index );
		return( Read( szFilePath, n_Texture ) );
#else
	
#ifdef _DEV_SKIP
		l_nDevSkip = GetPrivateProfileInt( "Texture", "Character_Skip", 0, "./dev.ini");
#endif
		sprintf( szFilePath, "%08d.dds", g_Pc_Manager.m_pTex[Index]->m_Index );
		return( ReadVF( &g_VFChrTex, szFilePath, n_Texture, l_nDevSkip ) );
#endif
		break;	

		///-------------------------------------------------------------------
		///-- OBJ
	case 2:	
#if !defined( LOAD_VF )
		sprintf( szFilePath, "%s\\Obj\\Data\\%08d.dds", g_RockClient.GetCurrentMapPath(), g_Obj_Manager.m_Tex[Index].m_Index );
		return( Read( szFilePath, n_Texture ) );
#else

//		if( g_ISObjTexLoad )
//		{
		g_VFMap.ChangeDir( "/Obj/Data" );
		sprintf( szFilePath, "%08d.dds", g_Obj_Manager.m_Tex[Index].m_Index );
		return( ReadVF( &g_VFMap, szFilePath, n_Texture, !g_ISObjTexLoad ) );
//		}
#endif
		break;

	default:
		return false;
	}

	return false;
}


//-----------------------------------------------------------------------------
// Desc: 텍스쳐 사용하는지 검사
//
// 해당 함수는 텍스쳐 관리자에서 호출되어 사용되는 것으로, 
// 타겟 텍스쳐가 마지막으로 사용된 시간을 검사하여서 삭제시키는 일을 한다.
//
// 텍스쳐를 삭제 시킬때는 텍스쳐의 사용속성(0~2)값을 가지고 계산한다.
// 0 : 삭제가능( 검색시간이 되었다면 )
// 1 : 삭제가능( 검색시간의 2배지났을 경우 )
// 2 : 삭제불가
//
// Copyright (c) 2003.10.20 By KStaR79
//-----------------------------------------------------------------------------
void Texture::UpdateTextureUse()
{	
	long lCheckTime = SAFE_TIME_SUB( g_nowTime , m_lPreTime );

	// 텍스쳐속성에 따라서처리
	switch( m_byAttr )
	{
	case DATA_DELETE0:		// NPC, CHA 삭제가능 - 3분
		{
			if( SAFE_TIME_COMPARE( lCheckTime , >= , TEXTURE_USE_TIME*6 ) )
				Destory();
		}
		break;
	case DATA_DELETE1:		// 삭제결정
		{
			if( SAFE_TIME_COMPARE( lCheckTime , >= , TEXTURE_USE_TIME*2 ) )
				Destory();
		}
		break;
	case DATA_NON_DELETE:	// 삭제불가
		break;
	}
}
