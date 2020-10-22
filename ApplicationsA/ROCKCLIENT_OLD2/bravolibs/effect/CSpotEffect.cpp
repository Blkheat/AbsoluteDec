#include "..\\..\\rocklibs\\RockPCH.h"
#include "CSpotEffect.h"
#include "VFileSystem.h"
#include "..\\..\\CRenderManager.h"

CSpotEffect::CSpotEffect()
{
	m_Tex   = NULL;
	m_State = DISPLAY_NONE;
}

CSpotEffect::~CSpotEffect()
{
	if( m_Tex != NULL )
	{
		m_Tex->Release();

		m_Tex = NULL;
	}
}

#ifdef DIRECT_VERSION_9_MJH
int	CSpotEffect::Init(LPDIRECT3DDEVICE9 pDevice)
#else
int	CSpotEffect::Init(LPDIRECT3DDEVICE8 pDevice)
#endif // DIRECT_VERSION_9_MJH
{
	return FALSE;

	m_VertexList[ 0 ].pos = D3DXVECTOR4(			0.0f   ,			  0.0f  , 0.0f , 1.0f );
	m_VertexList[ 1 ].pos = D3DXVECTOR4( ( float ) 1024.0f ,			  0.0f	, 0.0f , 1.0f );
	m_VertexList[ 2 ].pos = D3DXVECTOR4(			0.0f   , ( float ) 768.0f , 0.0f , 1.0f );
	m_VertexList[ 3 ].pos = D3DXVECTOR4( ( float ) 1024.0f , ( float ) 768.0f , 0.0f , 1.0f );

	m_VertexList[ 0 ].tu0  = 0.0f;
	m_VertexList[ 0 ].tv0  = 0.0f;

	m_VertexList[ 1 ].tu0  = 1.0f;
	m_VertexList[ 1 ].tv0  = 0.0f;

	m_VertexList[ 2 ].tu0  = 0.0f;
	m_VertexList[ 2 ].tv0  = 1.0f;

	m_VertexList[ 3 ].tu0  = 1.0f;
	m_VertexList[ 3 ].tv0  = 1.0f;
/*
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined( LOAD_VF ) /////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////	
#else  ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
*/
	VFileHandle* pFH = g_VFEtc.OpenFile( "banjac.TGA" );	/// OpenFile(...)에서 이미 메모리에 올라와 있음 
				

	if( pFH )
	{
		HRESULT hr = g_RenderManager.CreateTextureFromFileInMemoryEx( pFH->GetData(), pFH->GetSize(),	/// 데이터 포인터, 사이즈 
																	D3DX_DEFAULT, D3DX_DEFAULT, 
																	1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
																	D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
																	D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
																	0, NULL, NULL, &m_Tex );
			
		g_VFEnv.CloseFile( pFH );	/// 파일 반드시 닫아야 한다. 
	}		
/*
///////////////////////////////////////////////////////////////////////////////////////////////////	
#endif ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
*/
	return TRUE;
}

void CSpotEffect::SetStart(DWORD Time)
{
	m_State      = DISPLAY_PROCESS;
	m_OldTimer   = timeGetTime();
	m_RangeTimer = Time;
}

#ifdef DIRECT_VERSION_9_MJH
void CSpotEffect::Update(LPDIRECT3DDEVICE9 pDevice)
#else
void CSpotEffect::Update(LPDIRECT3DDEVICE8 pDevice)
#endif // DIRECT_VERSION_9_MJH
{
	if( m_State == DISPLAY_NONE )
		return;


	if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( timeGetTime() , m_OldTimer ) , > , m_RangeTimer ) )
	{
		m_State = DISPLAY_NONE;

		return;	
	}

	g_RenderManager.SetFVF( D3DFVF_SPORT2DVERTEXT );
	g_RenderManager.SetTexture( 0 , m_Tex );

	g_RenderManager.SetZEnable( FALSE );
	g_RenderManager.SetLighting( FALSE );
	g_RenderManager.SetFogEnable( FALSE );

	g_RenderManager.SetTextureMipFilter ( 0 , D3DTEXF_LINEAR );
	g_RenderManager.SetTextureMagFilter ( 0 , D3DTEXF_LINEAR );
	g_RenderManager.SetTextureMinFilter ( 0 , D3DTEXF_LINEAR );

	g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureColorArg1( 0 , D3DTA_DIFFUSE );
	g_RenderManager.SetTextureColorArg2( 0 , D3DTA_TEXTURE );

	g_RenderManager.SetAlphaBlendEnable(  TRUE );
	g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
	g_RenderManager.SetDestBlend( D3DBLEND_INVSRCALPHA );

	g_RenderManager.DrawPrimitiveUP( D3DPT_TRIANGLESTRIP , 2 , m_VertexList , sizeof( SPOT2DVERTEX ) );
	
	g_RenderManager.SetTexture( 0 , NULL );

	g_RenderManager.SetAlphaBlendEnable( FALSE );
    g_RenderManager.SetLighting( FALSE );
	g_RenderManager.SetZEnable( TRUE );
}

