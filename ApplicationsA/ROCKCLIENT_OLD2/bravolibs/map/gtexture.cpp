#include "RockPCH.h"
#include "VFileSystem.h"

#include	"..\\quadlist.h"
#include	<Base\\D3DBase.h>
#include	"GTexture.h"
#include	"..\\RockClient.h"

#include	<stdio.h>

/****************************************************************************************************
	* TEXTURE LIST - PARENT CLASS
****************************************************************************************************/
CEnvTexture::CEnvTexture()
{
}

CEnvTexture::~CEnvTexture()
{
//	FinalCleanup();
}

void	CEnvTexture::FinalCleanup()
{
//	for ( short i = 0; i < m_nCount; i ++ )
//		_RELEASE(m_pTexList[i]);
//	_DELETE_ARRAY(m_pTexList);
}

short	CEnvTexture::CountTexture(char* type)
{
/*	FILE *	fp;
	char	path[128]	= {0,};
	short	nResult	= 0;

	do
	{
		sprintf(path, "%s\\env\\%s%04d.dds", g_RockClient.GetRootDir(), type, nResult);
		fp	= fopen(path, "rb");
		if ( fp )
			nResult ++;
		else
			return	nResult;
		fclose(fp);
	}while ( fp );
*/
	return	0;
}

#ifdef DIRECT_VERSION_9_MJH
bool	CEnvTexture::Create(LPDIRECT3DDEVICE9 pDevice, char* type)
#else
bool	CEnvTexture::Create(LPDIRECT3DDEVICE8 pDevice, char* type)
#endif // DIRECT_VERSION_9_MJH
{
	return	true;
}
/****************************************************************************************************
	* END OF PARENT CLASS
****************************************************************************************************/

CGTexture::CGTexture()
{
}

CGTexture::~CGTexture()
{
	FinalCleanup();
}

DWORD	CGTexture::CountTexture()
{
	FILE *	fp = NULL;
	char	path[128]	= {0,};
	short	nResult	= 0;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined( LOAD_VF ) /////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	do
	{
		sprintf(path, "%s\\env\\grs%04d.dds", g_RockClient.GetRootDir(), nResult);
		fp	= fopen(path, "rb");
		if ( fp )
			nResult ++;
		else
			return	nResult;
		fclose(fp);
	}while ( fp );
///////////////////////////////////////////////////////////////////////////////////////////////////	
#else  ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

	VFileHandle* pFH = NULL;

	do
	{
		sprintf(path, "grs%04d.dds", nResult);
		pFH = g_VFEnv.OpenFile( path );	
		if( pFH )
			nResult ++;
		else
			return	nResult;
		g_VFEnv.CloseFile( pFH );		
	} while ( pFH );
///////////////////////////////////////////////////////////////////////////////////////////////////	
#endif ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

	return	0;
}

#ifdef DIRECT_VERSION_9_MJH
bool	CGTexture::Create(LPDIRECT3DDEVICE9 pDevice)
#else
bool	CGTexture::Create(LPDIRECT3DDEVICE8 pDevice)
#endif // DIRECT_VERSION_9_MJH
{
	DWORD			i;
	m_cCount	= CountTexture();
	// add name str
#ifdef DIRECT_VERSION_9_MJH
	m_pTexList	= SAFE_NEW_ARRAY( LPDIRECT3DTEXTURE9 , m_cCount );	
#else
	m_pTexList	= SAFE_NEW_ARRAY( LPDIRECT3DTEXTURE8 , m_cCount );	
#endif // DIRECT_VERSION_9_MJH
	char			filename[128];	

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined( LOAD_VF ) /////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	for ( i = 0; i < m_cCount; i ++ )
	{
		ZeroMemory(filename, sizeof(char)*128);
//		sprintf(filename, "%s\\Env\\grs%04d.tga", g_RockClient.GetRootDir(), i);
		sprintf(filename, "%s\\Env\\grs%04d.dds", g_RockClient.GetRootDir(), i);

		if ( FAILED ( g_RenderManager.CreateTextureFromFileEx( filename,
			D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN,
			D3DPOOL_MANAGED, D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR,
//			D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR, D3DCOLOR_XRGB(37,64,20), NULL, NULL, &m_pTexList [i]) ) )
			D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR, 0, NULL, NULL, &m_pTexList [i]) ) )
		{
///			MessageBox(NULL, filename, "Loading Failed", MB_OK);
			return	false;
		}
	}
///////////////////////////////////////////////////////////////////////////////////////////////////	
#else  ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

	for ( i = 0; i < m_cCount; i ++ )
	{
		ZeroMemory(filename, sizeof(char)*128);
		sprintf(filename, "grs%04d.dds", i);

		VFileHandle* pFH = g_VFEnv.OpenFile( filename );	
		if( pFH )
		{
			HRESULT hr = g_RenderManager.CreateTextureFromFileInMemoryEx( pFH->GetData(), pFH->GetSize(),	/// 데이터 포인터, 사이즈 
																		D3DX_DEFAULT, D3DX_DEFAULT, 
																		1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
																		D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
																		D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
																		0, NULL, NULL, &m_pTexList[i] );
			
			g_VFEnv.CloseFile( pFH );	/// 파일 반드시 닫아야 한다. 
		}		
	}
///////////////////////////////////////////////////////////////////////////////////////////////////	
#endif ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

	return	true;
}

void	CGTexture::FinalCleanup()
{
	DWORD i	= 0;
	for ( i = 0; i < m_cCount; i ++ )
		_RELEASE(m_pTexList[i]);
	_DELETE_ARRAY(m_pTexList);
}

CBravoSkyTexture::CBravoSkyTexture()
{

}

CBravoSkyTexture::~CBravoSkyTexture()
{
//	FinalCleanup();
}

short	CBravoSkyTexture::CountTexture()
{
/*	FILE *	fp;
	char	path[128]	= {0,};
	short	nResult	= 0;

	do
	{
		sprintf(path, "%s\\env\\sky%04d.dds", g_RockClient.GetRootDir(), nResult);
		fp	= fopen(path, "rb");
		if ( fp )
			nResult ++;
		else
			return	nResult;
		fclose(fp);
	}while ( fp );
*/
	return	0;
}

#ifdef DIRECT_VERSION_9_MJH
bool	CBravoSkyTexture::Create(LPDIRECT3DDEVICE9 pDevice)
#else
bool	CBravoSkyTexture::Create(LPDIRECT3DDEVICE8 pDevice)
#endif // DIRECT_VERSION_9_MJH
{
	return	true;
}

void	CBravoSkyTexture::FinalCleanup()
{
/*	if ( !m_nCount )
		return;
	for ( short i = 0; i < m_nCount; i ++ )
	{
		if ( m_pTexList[i] )
		{
			if( m_pTexList[i] != NULL )
			{
				m_pTexList[i]->Release();

				m_pTexList[i] = NULL;
			}

			if( m_pEveTexList[i] != NULL )
			{
				m_pEveTexList[i]->Release();
				m_pEveTexList[i] = NULL;
			}

			if( m_pNightTexList[i] != NULL )
			{
				m_pNightTexList[i]->Release();
				m_pNightTexList[i]	= NULL;
			}
		}
		
		_DELETE_ARRAY(m_szNameList[i]);
	}
	_DELETE_ARRAY(m_pTexList);
	_DELETE_ARRAY(m_pEveTexList);
	_DELETE_ARRAY(m_pNightTexList);
	_DELETE_ARRAY(m_szNameList);
*/
}

short	CBravoSkyTexture::GetIndexByName(char* name)
{
/*	for ( short i = 0; i < m_nCount; i ++ )
	{
		if ( strcmp(name, m_szNameList[i]) == 0 )
			return i;
	}
*/
	return 0;
}

/*******************************************************
	* CLOUD
*******************************************************/
CBravoCloudTexture::CBravoCloudTexture()
{
}

CBravoCloudTexture::~CBravoCloudTexture()
{
//	FinalCleanup();
}

void	CBravoCloudTexture::FinalCleanup()
{
//	for ( short i = 0; i < m_nCount; i ++ )
//		_RELEASE(m_pTexList[i]);
//	_DELETE_ARRAY(m_pTexList);
}

short	CBravoCloudTexture::CountTexture()
{
/*	FILE *	fp;
	char	path[128]	= {0,};
	short	nResult	= 0;

	do
	{
		sprintf(path, "%s\\env\\cloud%04d.dds", g_RockClient.GetRootDir(), nResult);
		fp	= fopen(path, "rb");
		if ( fp )
			nResult ++;
		else
			return	nResult;

		fclose(fp);
	}while ( fp );
*/
	return	0;
}

#ifdef DIRECT_VERSION_9_MJH
bool	CBravoCloudTexture::Create(LPDIRECT3DDEVICE9 pDevice)
#else
bool	CBravoCloudTexture::Create(LPDIRECT3DDEVICE8 pDevice)
#endif // DIRECT_VERSION_9_MJH
{
	return	true;
}

/*******************************************************
	* WATER
*******************************************************/
CBravoWaterTexture::CBravoWaterTexture()
{
}

CBravoWaterTexture::~CBravoWaterTexture()
{
	FinalCleanup();
}

void	CBravoWaterTexture::FinalCleanup()
{
	for ( short i = 0; i < m_nCount; i ++ )
		_RELEASE(m_pTexList[i]);
	_DELETE_ARRAY(m_pTexList);
}

short	CBravoWaterTexture::CountTexture()
{
	FILE *	fp = NULL;
	char	path[128]	= {0,};
	short	nResult	= 0;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined( LOAD_VF ) /////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	do
	{
		sprintf(path, "%s\\env\\water%04d.dds", g_RockClient.GetRootDir(), nResult);
		fp	= fopen(path, "rb");
		if ( fp )
			nResult ++;
		else
			return	nResult;

		fclose(fp);
	}while ( fp );
///////////////////////////////////////////////////////////////////////////////////////////////////	
#else  ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

	VFileHandle* pFH = NULL;
	do
	{
		sprintf(path, "water%04d.dds", nResult);
		pFH = g_VFEnv.OpenFile( path );	
		if( pFH )
			nResult ++;
		else
			return	nResult;
		g_VFEnv.CloseFile( pFH );		
	}while ( pFH );
///////////////////////////////////////////////////////////////////////////////////////////////////	
#endif ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

	return	0;
}


#ifdef DIRECT_VERSION_9_MJH
bool	CBravoWaterTexture::Create(LPDIRECT3DDEVICE9 pDevice)
#else
bool	CBravoWaterTexture::Create(LPDIRECT3DDEVICE8 pDevice)
#endif // DIRECT_VERSION_9_MJH
{
	m_nCount	= CountTexture();
#ifdef DIRECT_VERSION_9_MJH
	m_pTexList	= SAFE_NEW_ARRAY( LPDIRECT3DTEXTURE9 , m_nCount );
#else
	m_pTexList	= SAFE_NEW_ARRAY( LPDIRECT3DTEXTURE8 , m_nCount );
#endif // DIRECT_VERSION_9_MJH		
	char		path[128]	= {0,};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined( LOAD_VF ) /////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	for ( short i = 0; i < m_nCount; i ++ )
	{
//		sprintf(path, "%s\\env\\water%04d.tga", g_RockClient.GetRootDir(), i);
		sprintf(path, "%s\\env\\water%04d.dds", g_RockClient.GetRootDir(), i);
		if ( FAILED ( g_RenderManager.CreateTextureFromFile( path, &m_pTexList[i] ) ) )
		{
///			MessageBox(NULL, "Texture Loading Failed6", path, MB_OK);
			return	false;
		}
	}
///////////////////////////////////////////////////////////////////////////////////////////////////	
#else  ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

	for ( short i = 0; i < m_nCount; i ++ )
	{
		sprintf(path, "water%04d.dds", i);

		VFileHandle* pFH = g_VFEnv.OpenFile( path );	
		if( pFH )
		{
			HRESULT hr = g_RenderManager.CreateTextureFromFileInMemoryEx( pFH->GetData(), pFH->GetSize(),	/// 데이터 포인터, 사이즈 
																		D3DX_DEFAULT, D3DX_DEFAULT, 
																		1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
																		D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
																		D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
																		0, NULL, NULL, &m_pTexList[i] );
			
			g_VFEnv.CloseFile( pFH );	/// 파일 반드시 닫아야 한다. 
		}		
	}
///////////////////////////////////////////////////////////////////////////////////////////////////	
#endif ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////


	return	true;
}

/*******************************************************
	* WATERFALL
*******************************************************/
CBravoWaterfallTexture::CBravoWaterfallTexture()
{
}

CBravoWaterfallTexture::~CBravoWaterfallTexture()
{
//	FinalCleanup();
}

void	CBravoWaterfallTexture::FinalCleanup()
{
/*	for ( short i = 0; i < m_nCount; i ++ )
	{
		_RELEASE(m_pTexList[i]);
//		_RELEASE(m_pFallTexList[i]);
		for ( int e = 0; e < 8; e ++ )
			_RELEASE(m_pFallTexture[i][e]);
		_DELETE_ARRAY(m_pFallTexture[i]);
	}
	_DELETE_ARRAY(m_pTexList);
//	_DELETE_ARRAY(m_pFallTexList);
	_DELETE_ARRAY(m_pFallTexture);
	_RELEASE(m_pTextureWave);
	m_nCount	= 0;
*/
}

short	CBravoWaterfallTexture::CountTexture()
{
/*	FILE *	fp;
	char	path[128]	= {0,};
	short	nResult	= 0;

	do
	{
		sprintf(path, "%s\\env\\waterfall%04d.dds", g_RockClient.GetRootDir(), nResult);
		fp	= fopen(path, "rb");
		if ( fp )
			nResult ++;
		else
			return	nResult;

		fclose(fp);
	}while ( fp );
*/
	return	0;
}

#ifdef DIRECT_VERSION_9_MJH
bool	CBravoWaterfallTexture::Create(LPDIRECT3DDEVICE9 pDevice)
#else
bool	CBravoWaterfallTexture::Create(LPDIRECT3DDEVICE8 pDevice)
#endif // DIRECT_VERSION_9_MJH
{
	return	true;
}
