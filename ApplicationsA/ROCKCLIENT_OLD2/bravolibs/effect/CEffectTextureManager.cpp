#include "..\\..\\RockClient.h"
#include "CEffectTextureManager.h"

extern long				g_nowTime;

CEffectTextureManager::CEffectTextureManager()
{
	m_lPreTime = g_nowTime;
	m_wDeleteCnt = 0;
}

CEffectTextureManager::~CEffectTextureManager()
{
	DeleteAllMemory();
}

void CEffectTextureManager::DeleteAllMemory(void)
{
	m_TexMap.Clear();
}

#ifdef DIRECT_VERSION_9_MJH
LPDIRECT3DTEXTURE9	CEffectTextureManager::LoadFile(char *pKeyName,char *pFileName)
#else
LPDIRECT3DTEXTURE8	CEffectTextureManager::LoadFile(char *pKeyName,char *pFileName)
#endif // DIRECT_VERSION_9_MJH
{
	EFFECT_TEXTURE *pTempData = NULL;

	pTempData = SAFE_NEW( EFFECT_TEXTURE );	

	if( FAILED(g_RenderManager.CreateTextureFromFileEx( pFileName , D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0,
														D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
														D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
														D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
														0xff000000, NULL, NULL, &pTempData->pTexture )))
	{
		SAFE_DELETE( pTempData );
		
		return NULL;
	}
	
	pTempData->KeyName = pKeyName;

	m_TexMap.Insert( pKeyName , &pTempData );
	
	return pTempData->pTexture;
}

#ifdef DIRECT_VERSION_9_MJH
LPDIRECT3DTEXTURE9	CEffectTextureManager::LoadVirtualFile(CVFileSystem *pVF,char *pFileName)
#else
LPDIRECT3DTEXTURE8	CEffectTextureManager::LoadVirtualFile(CVFileSystem *pVF,char *pFileName)
#endif // DIRECT_VERSION_9_MJH
{
	EFFECT_TEXTURE *pTempData = NULL;
	
	pTempData = SAFE_NEW( EFFECT_TEXTURE );
		
	pTempData->KeyName = pFileName;
	m_TexMap.Insert( pFileName , &pTempData );

	return NULL;
}

int	CEffectTextureManager::AllLoadFromDirectory(char *pPath)
{
	char FileName[ 1024 ] = "";

	wsprintf( FileName , "%s\\Effect\\Tex\\*.tga" , pPath );

	WIN32_FIND_DATA	 fd;
	
	HANDLE hFindFile = FindFirstFile( FileName , &fd );
	
	if( hFindFile == INVALID_HANDLE_VALUE )	return FALSE;

	int Find	= TRUE;

	int Index = 0;

	while( Find != FALSE )
	{			
		wsprintf( FileName , "%s\\Effect\\Tex\\%s" , pPath , fd.cFileName );
		LoadFile( fd.cFileName , FileName );

		Find = FindNextFile( hFindFile, &fd );
	}					

	FindClose( hFindFile );

	return TRUE;
}

int	CEffectTextureManager::AllLoadFromVirtualFile(CVFileSystem *pVF)
{
	VFileHandleList* pFHL;
	VFileHandleList_i it;

	pFHL = pVF->GetFileHandleList();

	for( it = pFHL->begin(); it != pFHL->end(); ++it )
	{
		if( (*it)->GetFileContext()->iType == FT_FILE )
		{
			LoadVirtualFile( pVF , (*it)->GetFileContext()->strName );
		}
	}

	return TRUE;
}
#ifdef DIRECT_VERSION_9_MJH
LPDIRECT3DTEXTURE9	CEffectTextureManager::GetTexture(char *pFileName)
#else
LPDIRECT3DTEXTURE8	CEffectTextureManager::GetTexture(char *pFileName)
#endif // DIRECT_VERSION_9_MJH
{
	EFFECT_TEXTURE *pTempData = NULL;

	pTempData = m_TexMap.Find( pFileName );

	if( NULL == pTempData )
	{
		return NULL;
	}
	
	int l_nDevSkip = 0;
		
	l_nDevSkip = GetPrivateProfileInt( "Texture", "Effect_Skip", 0, "./dev.ini");

	if( l_nDevSkip )
	{
		return NULL;
	}
	
	if( NULL != pTempData && NULL == pTempData->pTexture )
	{
		VFileHandle* pFH = g_VFEffectTex.OpenFile( pTempData->KeyName.c_str() );
		
		if(pFH)
		{
			g_RenderManager.CreateTextureFromFileInMemoryEx( pFH->GetData(), pFH->GetSize(),
															D3DX_DEFAULT, D3DX_DEFAULT, 1, 0,
															D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
															D3DX_FILTER_NONE,
															D3DX_FILTER_NONE,															
															0, NULL, NULL, &pTempData->pTexture );

			///PRINT_DLG_LOG( "이펙트 텍스쳐 동적 생성 %s", pTempData->KeyName.c_str() );
			g_VFEffectTex.CloseFile( pFH );
		}
	}

	return pTempData->pTexture;
}

#ifdef DIRECT_VERSION_9_MJH
LPDIRECT3DTEXTURE9	CEffectTextureManager::GetTexture(unsigned long nHashCode)
#else
LPDIRECT3DTEXTURE8	CEffectTextureManager::GetTexture(unsigned long nHashCode)
#endif // DIRECT_VERSION_9_MJH
{
	EFFECT_TEXTURE *pTempData = NULL;

	pTempData = m_TexMap.Find( nHashCode );

	if( NULL == pTempData )
	{
		return NULL;
	}

	int l_nDevSkip = 0;
		
	l_nDevSkip = GetPrivateProfileInt( "Texture", "Effect_Skip", 0, "./dev.ini");

	if( l_nDevSkip )
	{
		return NULL;
	}
	
	if( NULL != pTempData && NULL == pTempData->pTexture )
	{
		VFileHandle* pFH = g_VFEffectTex.OpenFile( pTempData->KeyName.c_str() );		
		
		D3DFORMAT fFormat = D3DFMT_UNKNOWN;
		
		if(pFH)
		{
			UINT uWidth = D3DX_DEFAULT;
			UINT uHeight = D3DX_DEFAULT;

			D3DXIMAGE_INFO imageinfo;			
			if( FAILED( D3DXGetImageInfoFromFileInMemory( pFH->GetData(), pFH->GetSize(), &imageinfo ) ) )
			{       
				return NULL;
			}
			
			uWidth = imageinfo.Width;
			uHeight = imageinfo.Height;
			fFormat = imageinfo.Format;
			
			g_RenderManager.CreateTextureFromFileInMemoryEx( pFH->GetData(), pFH->GetSize(),
															uWidth, uHeight, 1, 0,
															fFormat, D3DPOOL_MANAGED,
															D3DX_FILTER_NONE,
															D3DX_FILTER_NONE,															
															0, NULL, NULL, &pTempData->pTexture );

			///PRINT_DLG_LOG( "이펙트 텍스쳐 동적 생성 %s", pTempData->KeyName.c_str() );
			g_VFEffectTex.CloseFile( pFH );
		}
	}
	
	pTempData->m_lPreTime = g_nowTime;

	return pTempData->pTexture;
}

void CEffectTextureManager::DeleteNotUsedTex()
{
	if( SAFE_TIME_COMPARE( g_nowTime , >= , SAFE_TIME_ADD( m_lPreTime , c_EffectDeleteTex_Update_Time ) ) )
	{
		m_lPreTime = g_nowTime;		
		vector < unsigned long > * pvList = m_TexMap.GetnList();		
		
		EFFECT_TEXTURE *pTempData = NULL;

		if(pvList->size())
		{
			pTempData = m_TexMap.Find( (*pvList)[m_wDeleteCnt] );

			if(pTempData->pTexture)
			{
				long lCheckTime = SAFE_TIME_SUB( g_nowTime , pTempData->m_lPreTime );
				if( SAFE_TIME_COMPARE( lCheckTime , >= , 180000 ) )
				{
					SAFE_RELEASE(pTempData->pTexture);
					///PRINT_DLG_LOG( "동적 이펙트 텍스쳐 해제 %s", pTempData->KeyName.c_str() );
				}			
			}			
			
			m_wDeleteCnt++;
			if( m_wDeleteCnt >= pvList->size() )
			{
				m_wDeleteCnt = 0;
			}
		}
	}
}

void CEffectTextureManager::WriteProfile(void)
{

#ifndef __EF_MANAGER_PROFILE	
	
		return;

#else

	std::map <unsigned long,EFFECT_TEXTURE *>::iterator	i = m_TexMap.m_HashTable.begin();

	FILE *fp = NULL;

	fp = fopen( "EffectTexture_Profile.txt" , "wt" );

	if( fp == NULL )
		return;

	fprintf( fp , "Effect Texture Manager ( Reference Count )\n" );

	int				Count = 0;
	
	EFFECT_TEXTURE *pData = NULL;

	for( ; i != m_TexMap.m_HashTable.end() ; ++i )
	{
		pData = ( EFFECT_TEXTURE * )(i->second);

		fprintf( fp , "(%5d) %20s - %5d\n" , Count , (char *)pData->KeyName.c_str() , pData->ReferenceCount );

		Count++;
	}

	fclose( fp );
#endif

}

HRESULT	CEffectTextureManager::InvalidateDeviceObjects()
{
	return S_OK;
}

HRESULT	CEffectTextureManager::RestoreDeviceObjects()
{

	return S_OK;
}



