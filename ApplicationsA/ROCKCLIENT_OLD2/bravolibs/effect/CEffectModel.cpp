#include "CEffectModel.h"
#include "..\\Effect.h"
#include "..\\CRenderManager.h"


CEffectModel::CEffectModel()
{
	m_ISModelLoad = FALSE;
	m_pTex		  = NULL;
	m_pModel	  = NULL;
}

CEffectModel::~CEffectModel() 
{
	m_pTex = NULL;
}




int	CEffectModel::InitDevice(char *ModelName, unsigned long	HashCode)
{
	//m_pTex = pTexture;
	m_HashCode = HashCode;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined( LOAD_VF ) /////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	

	try
	{
		char TempStr[ 1024 ] = "";

		m_pModel = SAFE_NEW( Model );		
		
		if( NULL == m_pModel )
			throw(1);

		wsprintf( TempStr , "%s\\Effect\\Static\\%s" , g_RockClient.GetRootDir() , ModelName );

		if( !m_pModel->Read( TempStr ,  TRUE ) )
			throw(1);

	}

///////////////////////////////////////////////////////////////////////////////////////////////////	
#else  ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////


	try
	{
			
		CEffectModel::m_pModel = SAFE_NEW( Model );
		
		if( NULL == m_pModel )
			throw(1);

		if( !m_pModel->ReadVF( &g_VFEffectStatic, ModelName  ,  TRUE ) )
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

		DeleteDevice();
		
		return FALSE;
	}
	
	m_ISModelLoad = TRUE;

	return TRUE;
}

int CEffectModel::DeleteDevice(void)
{
	SAFE_DELETE( m_pModel );
	m_pTex = NULL;

	return TRUE;
}

int CEffectModel::Render(void)
{
	if( !m_ISModelLoad ) return FALSE;
	
	g_RenderManager.SetFVF( D3DFVF_OBJECTVERTEX );

	g_RenderManager.SetTexture( 0, g_Particle.m_EffectTextureManager.GetTexture(m_HashCode) );			

	g_RenderManager.SetIndices( m_pModel->m_pIB , 0 );
	g_RenderManager.SetStreamSource( 0 , m_pModel->m_pVB , sizeof(OBJECTVERTEX) );

	DrawIndexedPrimitive( 
		m_pModel->m_header.m_num_vertex , 
		m_pModel->m_header.m_num_face     );
	
	g_RenderManager.SetTexture( 0, NULL );			

	return TRUE;
}

