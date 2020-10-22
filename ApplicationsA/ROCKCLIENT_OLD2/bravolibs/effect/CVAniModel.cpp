#include "CVAniModel.h"
#include "..\\..\\CRenderManager.h"

CVAniModel::CVAniModel()
{
	m_VB      = NULL;
	m_pVertex = NULL;
	m_pNormal = NULL;

	DeleteAllMemory();
}

CVAniModel::~CVAniModel()
{
	DeleteAllMemory();
}

void CVAniModel::DeleteAllMemory(void)
{
	SAFE_RELEASE( m_VB );

	SAFE_DELETE_ARRAY( m_pVertex );
	SAFE_DELETE_ARRAY( m_pNormal );
	
	m_Init = FALSE;

	m_NowFrame = 0;
	m_PreTime  = 0;

	m_uFrame = 30;
}

int	CVAniModel::Create(Model *pModel,DWORD Time)
{
	if( NULL == pModel )
		return FALSE;
	
	if( m_Init )
		return FALSE;

	if( 4 != pModel->m_ani_type )
		return FALSE;

	m_VertexCount = pModel->m_header.m_num_vertex;

	m_pVertex = SAFE_NEW_ARRAY( D3DXVECTOR3 , m_VertexCount );	
	
	if( NULL == m_pVertex )
	{
		DeleteAllMemory();
		
		return FALSE;
	}

	memset( m_pVertex , 0, m_VertexCount * sizeof( D3DXVECTOR3 ) );
	
	m_pNormal = SAFE_NEW_ARRAY( D3DXVECTOR3 , m_VertexCount );
	
	if( NULL == m_pNormal )
	{
		DeleteAllMemory();

		return FALSE;
	}

	memset( m_pNormal , 0, m_VertexCount * sizeof( D3DXVECTOR3 ) );

	if( FAILED( g_RenderManager.CreateVertexBuffer( pModel->m_header.m_num_vertex * sizeof( OBJECTVERTEX ) , D3DUSAGE_WRITEONLY, 
											 D3DFVF_OBJECTVERTEX, D3DPOOL_MANAGED, &m_VB ) ) )
	{
		return FALSE;
	}

	OBJECTVERTEX	*pVB = NULL;	
#ifdef DIRECT_VERSION_9_MJH
	if( FAILED( m_VB->Lock( 0, 0, (void**)&pVB, 0 ) ) )
#else
	if( FAILED( m_VB->Lock( 0, 0, (BYTE**)&pVB, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH
	{
		DeleteAllMemory();

		return FALSE; 		
	}

	if( pVB && pModel && pModel->m_pVertex )
	{
		///-- By simwoosung
		///-- 이걸 카피할 필요는 없다. 제거 루틴
		//memcpy( pVB , ( OBJECTVERTEX * )&(pModel->m_pVertex) , 
		//sizeof( OBJECTVERTEX ) * (pModel->m_header.m_num_vertex) );
		memset( pVB , 0, pModel->m_header.m_num_vertex * sizeof( OBJECTVERTEX ) );
	}
	else
	{
		DeleteAllMemory();
		m_VB->Unlock();

		return FALSE;
	}
	
	m_VB->Unlock();	

	m_NowFrame = 0;
	m_PreTime  = Time;
	m_Init	   = TRUE;

	return TRUE;
}

int	CVAniModel::Update(Model *pModel,DWORD Time, Character * m_pChar)
{
	if( !m_Init || NULL == pModel )
		return FALSE;

	DWORD	RangeTime   = Time - m_PreTime;
	int		SkipFrame   = RangeTime / m_uFrame;
	float	fRangeFrame = 0.0f;
	int		NextFrame   = 0;

	if( SkipFrame )
		m_PreTime = Time;


	if( ( m_NowFrame + SkipFrame ) >= pModel->m_header.m_ani_frame )
	{
		int AddCount = ( m_NowFrame + SkipFrame ) / pModel->m_header.m_ani_frame;
	
		m_NowFrame   = ( SkipFrame + m_NowFrame ) % pModel->m_header.m_ani_frame;
		RangeTime   -= AddCount * m_uFrame;
	}
	else
	{
	
		m_NowFrame  += SkipFrame;
		RangeTime   -= ( SkipFrame * m_uFrame );
	}

	if( m_pChar && !(m_pChar->m_IsApplyVA) )
	{
		return TRUE;
	}
	
	fRangeFrame = ( float ) ( ( float ) RangeTime / ( float ) m_uFrame );

	if( fRangeFrame == 0 )
	{
		for( int i = 0 ; i < pModel->m_header.m_num_vertex ; ++i )
		{
			m_pVertex[ i ] = pModel->m_ppVertexList[ m_NowFrame ][ i ].m_vertex;
			m_pNormal[ i ] = pModel->m_ppVertexList[ m_NowFrame ][ i ].m_normal;
		}
	}
	else
	{
		NextFrame = m_NowFrame + 1;
		
		if( m_NowFrame >= pModel->m_header.m_ani_frame )
		{
			m_NowFrame = pModel->m_header.m_ani_frame - 1;
		}
	
		if( NextFrame >= pModel->m_header.m_ani_frame )
		{
			NextFrame = 0;
		}
		
		for( int i = 0 ; i < pModel->m_header.m_num_vertex ; ++i )
		{
			D3DXVec3Lerp( &m_pVertex[ i ] , 
						  &pModel->m_ppVertexList[ m_NowFrame ][ i ].m_vertex , 
						  &pModel->m_ppVertexList[ NextFrame  ][ i ].m_vertex , fRangeFrame );

			D3DXVec3Lerp( &m_pNormal[ i ] , 
						  &pModel->m_ppVertexList[ m_NowFrame ][ i ].m_normal , 
						  &pModel->m_ppVertexList[ NextFrame  ][ i ].m_normal , fRangeFrame );
		} 
	}
	
	OBJECTVERTEX	*pVB;	
#ifdef DIRECT_VERSION_9_MJH
	if( FAILED( m_VB->Lock( 0, 0, (void**)&pVB, 0 ) ) )
#else
	if( FAILED( m_VB->Lock( 0, 0, (BYTE**)&pVB, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH
	{
		DeleteAllMemory();
			
		return FALSE; 		
	}

	for( int i = 0 ; i < pModel->m_header.m_num_vertex ; ++i )
	{
		pVB[ i ].Vertex = m_pVertex[ i ];
		pVB[ i ].Normal = m_pNormal[ i ];
		pVB[ i ].u      = pModel->m_pVertex[ i ].m_u;
		pVB[ i ].v      = pModel->m_pVertex[ i ].m_v;
	}

	m_VB->Unlock();	

	return TRUE;
}

int	CVAniModel::Draw(Model *pModel, UINT uFrame)
{
	if( !m_Init )
		return FALSE;	

	g_RenderManager.SetFVF( D3DFVF_OBJECTVERTEX );

	g_RenderManager.SetIndices( pModel->m_pIB , 0 );
	g_RenderManager.SetStreamSource( 0 , m_VB , sizeof(OBJECTVERTEX) );
	
	g_RenderManager.DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, pModel->m_header.m_num_vertex , 0 , pModel->m_header.m_num_face );

	return TRUE;
}

D3DXVECTOR3 CVAniModel::GetVertex(int Num)
{
	if( Num < m_VertexCount && NULL != m_pVertex )
	{
		return m_pVertex[ Num ];
	}

	return D3DXVECTOR3( 1.0f , 1.0f , 1.0f );
}
