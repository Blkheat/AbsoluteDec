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
#define		TEXTURE_USE_TIME	30000	// �ؽ��� ��� �˻�ð�(20��)

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
// Desc: �ؽ��� �б�
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
		// �ε��� �÷��� ����
		m_bIsLoading = true;		

		HRESULT hr;

		FILE*	file = NULL;
		if( NULL == (file = fopen( _szFilePath, "rb" )) )	
		{
			CloseWnd_Error( "!%s ������ ã�� �� �����ϴ�.", _szFilePath );			
			return false;
		}

		if( type == n_Texture )
		{
			// �ؽ��� �޸� �Ҵ�
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
	
		
		// �ؽ��ķε� ����
		if( 0 == hr )
		{
			m_bIsLoading	= false;
			m_bIsLoaded		= true;	
			m_lPreTime		= g_nowTime;
		}
		// �ؽ��ķε� ����
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
///-- Desc: �ؽ��� �б�
///----------------------------------------------------------------------------
///-- 2005.03.15 / Lyul
///-- a_bDevSkip �� ��� Texture �� Loading ���� �ʴ´�. Default 0

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
		// �ε��� �÷��� ����
		m_bIsLoading = true;	

		HRESULT hr = -1;

		// ���������� ����
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
				///--PRINT_DLG_LOG( "�ؽ��� �ε�" );						
					
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
				///-- �ؽ��� �Ӹ� 1�� �ϸ� �ؽ��� �ε����� ����������...
				///-- �Ӹ��� ��������� �ʾ�. ���Ÿ������� ��Ʈ�� Ƣ�� ������ �Ͼ��.
				///-- ���� �ؽ��� ���Ƿ����� �ؽ��ĳ��� �̸����� ������ ����������...
				///-- ������ ���������� �ʾ�.. 1������ �Ӹ� �ؽ��ĸ� �׳� ����Ѵ�.
				nMipMapLevel = 1;

				/*BYTE * pData = (BYTE *)pFH->GetData();
				DWORD dwMagic = 0;

				memcpy( &dwMagic, pData, sizeof(DWORD) );
				pData += sizeof(DWORD);

				DDSURFACEDESC2 ddSuface2;
				ZeroMemory( &ddSuface2, sizeof( DDSURFACEDESC2));
				memcpy( &ddSuface2, pData, sizeof(DDSURFACEDESC2) );*/
				
				// �ؽ��� �޸� �Ҵ�
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
				
				///--PRINT_DLG_LOG("�ؽ��� �ε� ����ð� %u", SAFE_TIME_SUB( timeGetTime() , dTime ) );

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
			///-- CVFileSystem������ ũ��Ƽ�� �������� ���� �׳� �ǳʶۼ��� �ִ�.
			///--PRINT_DLG_LOG( "!%s ������ ã�� �� �����ϴ�.", _szFilePath );
			//CloseWnd_Error( "!%s ������ ã�� �� �����ϴ�.", _szFilePath );
			m_bIsLoaded		= false;
			m_bIsLoading	= false;
			return false;
		}
		
		// �ؽ��ķε� ����
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
		// �ؽ��ķε� ����
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
// Desc: �ؽ��ļ���
//
// 1. ����Ϸ��� �ؽ��İ� �޸��Ҵ�Ǿ����� �˻��ϱ�
// �Ҵ�Ǿ��ִٸ� �翬�� ����ϰ� ����ð��� �����Ͽ��� �ؽ������Ҵ�ó���� �̿�
// 2. �޸��Ҵ�Ǿ� ���� �ʴٸ� �ؽ��İ����ڿ� �ش� �ؽ����ε����� ������,
// �ؽ��İ����ڿ��� �޸��Ҵ��ϵ��� �Ѵ�
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
	// ������
	//-----------------------------------------------------------------------------
	bool bLoaded = false, bLoading = false;
	if( GMODE_NORMAL == g_RockClient.GetGameMode() )
	{		
		bLoaded		= m_bIsLoaded;
		bLoading	= m_bIsLoading;	

		// �ε����̸� ����
		if( true == bLoading )
		{
			return false;
		}
		else
		{
			// �ε��Ϸ�. �ؽ��ļ���. ����ð�����
			if( true == bLoaded )
			{
				g_RenderManager.SetTexture( 0, m_pD3DTex );	
				m_lPreTime = g_nowTime;
			}
			// �ؽ��Ļ����ϵ��� Ÿ��, �ε��� �ѱ��
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
	// ����
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
// Desc: �ؽ��� ����ϴ��� �˻�
//
// �ؽ��� Ÿ�԰� �����ε����� �޾Ƽ� �ؽ��ĸ� �����Ѵ�.
//
// Copyright (c) 2003.10.20 By KStaR79
//-----------------------------------------------------------------------------
///-- 2005.04.01 / Lyul / DEVSkip �� �����ϳ� �ӵ��� �ΰ��� �� �ִ� Define ���

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
// Desc: �ؽ��� ����ϴ��� �˻�
//
// �ش� �Լ��� �ؽ��� �����ڿ��� ȣ��Ǿ� ���Ǵ� ������, 
// Ÿ�� �ؽ��İ� ���������� ���� �ð��� �˻��Ͽ��� ������Ű�� ���� �Ѵ�.
//
// �ؽ��ĸ� ���� ��ų���� �ؽ����� ���Ӽ�(0~2)���� ������ ����Ѵ�.
// 0 : ��������( �˻��ð��� �Ǿ��ٸ� )
// 1 : ��������( �˻��ð��� 2�������� ��� )
// 2 : �����Ұ�
//
// Copyright (c) 2003.10.20 By KStaR79
//-----------------------------------------------------------------------------
void Texture::UpdateTextureUse()
{	
	long lCheckTime = SAFE_TIME_SUB( g_nowTime , m_lPreTime );

	// �ؽ��ļӼ��� ����ó��
	switch( m_byAttr )
	{
	case DATA_DELETE0:		// NPC, CHA �������� - 3��
		{
			if( SAFE_TIME_COMPARE( lCheckTime , >= , TEXTURE_USE_TIME*6 ) )
				Destory();
		}
		break;
	case DATA_DELETE1:		// ��������
		{
			if( SAFE_TIME_COMPARE( lCheckTime , >= , TEXTURE_USE_TIME*2 ) )
				Destory();
		}
		break;
	case DATA_NON_DELETE:	// �����Ұ�
		break;
	}
}
