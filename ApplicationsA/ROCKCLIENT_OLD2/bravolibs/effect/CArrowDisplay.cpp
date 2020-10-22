#include "CArrowDisplay.h"
#include "..\\..\\CRenderManager.h"

CArrowDisplay::CArrowDisplay()
{
	m_pModel   = NULL;
	m_pTexture = NULL;
}

CArrowDisplay::~CArrowDisplay()
{
	DeleteAllMemory();
}

void CArrowDisplay::DeleteAllMemory(void)
{
	SAFE_DELETE( m_pModel );
	SAFE_RELEASE( m_pTexture );
}

#ifdef DIRECT_VERSION_9_MJH
int CArrowDisplay::Init(LPDIRECT3DDEVICE9 pDevice)
#else
int CArrowDisplay::Init(LPDIRECT3DDEVICE8 pDevice)
#endif // DIRECT_VERSION_9_MJH
{
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined( LOAD_VF ) /////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	try
	{
		if( FAILED ( g_RenderManager.CreateTextureFromFile( "Effect\\Static\\arrow.tga"  , &m_pTexture ) ) )
			throw(0);

		m_pModel = SAFE_NEW( Model );		
		
		if( NULL == m_pModel )
			throw(1);

		if( !m_pModel->Read( "Effect\\Static\\arrow.msh" ,  TRUE ) )
			throw(1);
	}

///////////////////////////////////////////////////////////////////////////////////////////////////	
#else  ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////


	try
	{
		VFileHandle* pFH = g_VFEffectStatic.OpenFile( "arrow.tga" );	
		if( pFH )
		{
			if( FAILED ( g_RenderManager.CreateTextureFromFileInMemoryEx( pFH->GetData(), pFH->GetSize(),	/// 데이터 포인터, 사이즈 
																		D3DX_DEFAULT, D3DX_DEFAULT, 
																		1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
																		D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
																		D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
																		0, NULL, NULL, &m_pTexture ) ) )
			{
				g_VFEnv.CloseFile( pFH );	/// 파일 반드시 닫아야 한다. 
				throw(0);
			}			
			g_VFEnv.CloseFile( pFH );	/// 파일 반드시 닫아야 한다. 
		}		
		
		m_pModel = SAFE_NEW( Model );		
		
		if( NULL == m_pModel )
			throw(1);

		if( !m_pModel->ReadVF( &g_VFEffectStatic, "arrow.msh" ,  TRUE ) )
			throw(1);
	}

///////////////////////////////////////////////////////////////////////////////////////////////////	
#endif ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

	catch(int Type)
	{
		switch( Type )
		{
			case 0:
///				MessageBox( NULL, "Error Texture Load", "Effect WoodenSword", MB_OK );
				break;
			case 1:
///				MessageBox( NULL , "Error Load Model" , "Effect WoodenSword" , MB_OK );
				break;
		}

		DeleteAllMemory();
		
		return FALSE;
	}
	
	return TRUE;
}

#ifdef DIRECT_VERSION_9_MJH
int CArrowDisplay::Render(LPDIRECT3DDEVICE9 pDevice)
#else
int CArrowDisplay::Render(LPDIRECT3DDEVICE8 pDevice)
#endif // DIRECT_VERSION_9_MJH
{
	g_RenderManager.SetFVF( D3DFVF_OBJECTVERTEX );

	g_RenderManager.SetTexture( 0, m_pTexture );			

	g_RenderManager.SetIndices( m_pModel->m_pIB , 0 );
	g_RenderManager.SetStreamSource( 0 , m_pModel->m_pVB , sizeof(OBJECTVERTEX) );

	DrawIndexedPrimitive( m_pModel->m_header.m_num_vertex , m_pModel->m_header.m_num_face );

	g_RenderManager.SetTexture( 0, NULL );			
	
	return TRUE;
}

