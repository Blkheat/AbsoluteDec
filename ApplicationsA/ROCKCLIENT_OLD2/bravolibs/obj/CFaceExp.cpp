#include "CFaceExp.h"
#include "VFileSystem.h"
#include "..\\..\\RockClient.h"

extern int g_nTextureDetail;

CFaceExpData::CFaceExpData()
{
	m_FaceCode       = 0;
	m_TwinkleWaitMin = 0;
	m_TwinkleWaitMax = 0;
	
	for( int i = 0 ; i < CFaceExpData::FACE_EXP_MAXCOUNT ; ++i )
	{
		//m_TextureName [ i ] = 0;
		m_DisplayTime [ i ] = 0;
		m_pTextureList[ i ] = NULL;
	}
}

CFaceExpData::~CFaceExpData()
{
	DeleteAllMemory();
}

void CFaceExpData::DeleteAllMemory(void)
{
	for( int i = 0 ; i < CFaceExpData::FACE_EXP_MAXCOUNT ; ++i )
	{
		if( NULL != m_pTextureList[ i ] )
		{
			m_pTextureList[ i ]->Release();
			m_pTextureList[ i ] = NULL;
		}
	}
}

DWORD CFaceExpData::GetDisplayTime(FACE_EXP eType)
{
	if( eType >= CFaceExpData::FACE_EXP_MAXCOUNT )
	{
		return 0;
	}

	return m_DisplayTime[ ( int ) eType ];
}
#ifdef DIRECT_VERSION_9_MJH
LPDIRECT3DTEXTURE9 CFaceExpData::GetTexture(FACE_EXP eType)
#else
LPDIRECT3DTEXTURE8 CFaceExpData::GetTexture(FACE_EXP eType)
#endif // DIRECT_VERSION_9_MJH
{
	int Num = ( int ) eType;
	
	if( Num >= CFaceExpData::FACE_EXP_MAXCOUNT )
	{
		return NULL;
	}
	
	int l_nDevSkip = 0;
	l_nDevSkip = GetPrivateProfileInt( "Texture", "Character_Skip", 0, "./dev.ini");
	if( l_nDevSkip )
	{
		return NULL;
	}

	if( NULL == m_pTextureList[ Num ] )
	{
		char TempStr[ 1024 ] = "";

		wsprintf( TempStr , "%s.dds" , ( char * )m_TextureName[ Num ].c_str() );

		VFileHandle* pFH = g_VFChrTex.OpenFile( TempStr );	
		D3DFORMAT fFormat = D3DFMT_UNKNOWN;
		
		if( pFH )
		{
			
			///--PRINT_DLG_LOG("얼굴 텍스쳐 로딩");
			
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

			int nLevel = pow((float)2, (float)g_nTextureDetail);
			uWidth /= nLevel; 
			uHeight /= nLevel;
			
			DWORD aFilter = D3DX_FILTER_LINEAR;
			if( g_nTextureDetail == 0 )
			{
				aFilter = D3DX_FILTER_NONE;
			}
	
			g_RenderManager.CreateTextureFromFileInMemoryEx( pFH->GetData(), pFH->GetSize(),
															uWidth, uHeight, 
															1 , 0, fFormat, D3DPOOL_MANAGED,
															//D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
															//D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
															aFilter,
															D3DX_DEFAULT,
															0, NULL, NULL, &m_pTextureList[ Num ] );

			g_VFChrTex.CloseFile( pFH );	
			

			if( NULL == m_pTextureList[ Num ] )
				return NULL;
		}
		else
			return NULL;

	}

	return m_pTextureList[ Num ];
}

CFaceExp::CFaceExp()
{
}

CFaceExp::~CFaceExp()
{
	DeleteAllMemory();
}

int	CFaceExp::Init(void)
{
    CTextCommand TextCmd;

	if( TextCmd.OpenVF( "faceexp.dat" ) == NULL )
    {
        return false;
    }

    while( TextCmd.GetLine() != NULL )
    {
		if( TextCmd.ISArgEmpty() )
        {
            continue;
        }

		CFaceExpData    *pTempData;

		pTempData = SAFE_NEW( CFaceExpData );	
		
		TextCmd.GetArgValue( 0 , pTempData->m_FaceCode	     );
		TextCmd.GetArgValue( 1 , pTempData->m_TwinkleWaitMin );
		TextCmd.GetArgValue( 2 , pTempData->m_TwinkleWaitMax );
		
		for( int i = 0 ; i < CFaceExpData::FACE_EXP_MAXCOUNT ; ++i )
		{
			//TextCmd.GetArgValue( 3 + ( i * 2 )     , pTempData->m_TextureName[ i ] );
			
			pTempData->m_TextureName[ i ] = TextCmd.GetArg( 3 + ( i * 2 ) );

			TextCmd.GetArgValue( 3 + ( i * 2 ) + 1 , pTempData->m_DisplayTime[ i ] );

			//pTempData->GetTexture( ( CFaceExpData::FACE_EXP ) i );
		}

		m_FaceExpList.push_back( pTempData );
	}

    TextCmd.Close();

	return TRUE;
}

void CFaceExp::DeleteAllMemory(void)
{
	std::vector <CFaceExpData *>::iterator i = m_FaceExpList.begin();

	for( ; i != m_FaceExpList.end() ;  ++i )
	{
		CFaceExpData * pData = (*i);
		SAFE_DELETE(pData);
		(*i) = NULL;
	}

	m_FaceExpList.clear();
}

CFaceExpData *CFaceExp::GetFaceExpInfo(DWORD FaceCode)
{
	std::vector <CFaceExpData *>::iterator i = m_FaceExpList.begin();

	for( ; i != m_FaceExpList.end() ;  ++i )
	{
		if( (*i)->m_FaceCode == FaceCode )
		{
			return (*i);
		}
	}

	return NULL;
}


void CFaceExp::DeleteAllTexture()
{
	std::vector <CFaceExpData *>::iterator i = m_FaceExpList.begin();

	for( ; i != m_FaceExpList.end() ;  ++i )
	{
		CFaceExpData * pData = (*i);
		if(pData)
		{
			pData->DeleteAllMemory();
		}
	}	
}


