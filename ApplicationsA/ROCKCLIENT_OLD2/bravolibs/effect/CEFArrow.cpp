#include "CEFArrow.h"
#include "..\\Effect.h"
#include "..\\..\\CRenderManager.h"

#ifdef DIRECT_VERSION_9_MJH
LPDIRECT3DTEXTURE9	CEFArrow::m_pTexArrow      = NULL;
#else
LPDIRECT3DTEXTURE8	CEFArrow::m_pTexArrow      = NULL;
#endif // DIRECT_VERSION_9_MJH
Model			   *CEFArrow::m_pArrowModel[3] = { NULL , };
BOOL				CEFArrow::ISLoad           = FALSE;    
#ifdef DIRECT_VERSION_9_MJH
LPDIRECT3DTEXTURE9  CEFArrow::m_pTexLine       = NULL;
#else
LPDIRECT3DTEXTURE8  CEFArrow::m_pTexLine       = NULL;
#endif // DIRECT_VERSION_9_MJH


int CEFArrow::InitDevice(void)
{
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined( LOAD_VF ) /////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	try
	{
		char TempStr[ 1024 ] = "";

		wsprintf( TempStr , "%s\\Effect\\Static\\arrow.tga" , g_RockClient.GetRootDir() );
		
		if( FAILED ( g_RenderManager.CreateTextureFromFile( TempStr ,  &CEFArrow::m_pTexArrow ) ) )
			throw(0);		

		for( int i = 0 ; i < 3 ; ++i )
		{
			CEFArrow::m_pArrowModel[i] = SAFE_NEW( Model );			
			
			if( NULL == CEFArrow::m_pArrowModel[ i ] )
				throw(1);
			
			if( i == 0 )
			{
				if( !CEFArrow::m_pArrowModel[ i ]->Read( "Effect\\Static\\ef_arrow.msh"  ,  TRUE ) )
					throw(1);
			}
			else
			{
				char TempStr[ 1024 ] = "";
				
				wsprintf( TempStr , "Effect\\Static\\ef_arrow%d.msh" , i );

				if( !CEFArrow::m_pArrowModel[ i ]->Read( TempStr  ,  TRUE ) )
					throw(1);
			}
		}
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
																		0, NULL, NULL, &CEFArrow::m_pTexArrow ) ) )
			{
				g_VFEnv.CloseFile( pFH );	/// 파일 반드시 닫아야 한다. 
				throw(0);
			}			
			g_VFEnv.CloseFile( pFH );	/// 파일 반드시 닫아야 한다. 
		}		

		for( int i = 0 ; i < 3 ; ++i )
		{
			CEFArrow::m_pArrowModel[i] = SAFE_NEW( Model );		
			
			if( NULL == CEFArrow::m_pArrowModel[ i ] )
				throw(1);
			
			if( i == 0 )
			{
				if( !CEFArrow::m_pArrowModel[ i ]->ReadVF( &g_VFEffectStatic, "ef_arrow.msh"  ,  TRUE ) )
					throw(1);
			}
			else
			{
				char TempStr[ 1024 ] = "";
				
				wsprintf( TempStr , "Effect\\Static\\ef_arrow%d.msh" , i );

				if( !CEFArrow::m_pArrowModel[ i ]->ReadVF( &g_VFEffectStatic, TempStr  ,  TRUE ) )
					throw(1);
			}
		}
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

		CEFArrow::DeleteDevice();
		
		return FALSE;
	}
	
	ISLoad = TRUE;

	return TRUE;
}

int CEFArrow::DeleteDevice(void)
{
	SAFE_RELEASE( CEFArrow::m_pTexArrow );
	SAFE_DELETE( CEFArrow::m_pArrowModel[ 0 ] );
	SAFE_DELETE( CEFArrow::m_pArrowModel[ 1 ] );
	SAFE_DELETE( CEFArrow::m_pArrowModel[ 2 ] );
	SAFE_RELEASE( CEFArrow::m_pTexLine );

	return TRUE;
}

int CEFArrow::Render(int Num = 0)
{
	if( !ISLoad ) return FALSE;
	
	g_RenderManager.SetFVF( D3DFVF_OBJECTVERTEX );

	g_RenderManager.SetTexture( 0, CEFArrow::m_pTexArrow );			

	g_RenderManager.SetIndices( CEFArrow::m_pArrowModel[ Num ]->m_pIB , 0 );
	g_RenderManager.SetStreamSource( 0 , CEFArrow::m_pArrowModel[ Num ]->m_pVB , sizeof(OBJECTVERTEX) );

	DrawIndexedPrimitive( 
		CEFArrow::m_pArrowModel[ Num ]->m_header.m_num_vertex , 
		CEFArrow::m_pArrowModel[ Num ]->m_header.m_num_face     );
	
	g_RenderManager.SetTexture( 0, NULL );			

	return TRUE;
}

