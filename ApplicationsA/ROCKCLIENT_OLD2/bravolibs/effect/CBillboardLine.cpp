#include "CBillboardLine.h"
#include "CRenderManager.h"

//................................................................................................................
// Create class
//................................................................................................................
CBillboardLine::CBillboardLine()
{
	m_pVB = NULL;
	m_pIB = NULL;
}

//................................................................................................................
// Destroy class
//................................................................................................................
CBillboardLine::~CBillboardLine()
{
	if( m_pVB != NULL )
	{
		m_pVB->Release();

		m_pVB = NULL;
	}

	if( m_pIB != NULL )
	{
		m_pIB->Release();

		m_pIB = NULL;
	}

}

//................................................................................................................
// InitDevice
//................................................................................................................
#ifdef DIRECT_VERSION_9_MJH
int CBillboardLine::InitDevice(LPDIRECT3DDEVICE9 pDevice)
#else
int CBillboardLine::InitDevice(LPDIRECT3DDEVICE8 pDevice)
#endif // DIRECT_VERSION_9_MJH
{
	//............................................................................................................
	// 인덱스 버퍼 만들기
	//............................................................................................................
	WORD			IndexList[ 4 ] = 
	{
		0 , 1 , 2 , 3 
	};

	if( FAILED( 
		g_RenderManager.CreateIndexBuffer( ( sizeof( WORD ) * 4 )   ,
		D3DUSAGE_WRITEONLY	     ,
		D3DFMT_INDEX16 		 ,
		D3DPOOL_MANAGED		 ,
		&m_pIB )) )
	{
		return FALSE;		
	}
	
	BYTE   *pIndexBuffer = NULL;
#ifdef DIRECT_VERSION_9_MJH
	if( FAILED ( m_pIB->Lock(0, 0, (void**)&pIndexBuffer, 0 ) ) )
#else
	if( FAILED ( m_pIB->Lock(0, 0, (BYTE**)&pIndexBuffer, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH	
	{
		return FALSE;
	}
	
	memcpy( pIndexBuffer , IndexList , sizeof( WORD ) * 4 );
	
	m_pIB->Unlock();


	//............................................................................................................
	// 버텍스 버퍼 만들기
	//............................................................................................................
	BILLBOARD_LINE_VERTEX	VertexList[ 4 ];

	VertexList[ 0 ].pos = D3DXVECTOR3( -0.5f , 0.0f , 0.0f );
	VertexList[ 1 ].pos = D3DXVECTOR3( -0.5f , 1.0f , 0.0f );
	VertexList[ 2 ].pos = D3DXVECTOR3(  0.5f , 0.0f , 0.0f );
	VertexList[ 3 ].pos = D3DXVECTOR3(  0.5f , 1.0f , 0.0f );

	VertexList[ 0 ].u = 0.0f;
	VertexList[ 0 ].v = 1;
		
	VertexList[ 1 ].u = 0.0f;
	VertexList[ 1 ].v = 0.0f;
		
	VertexList[ 2 ].u = 1.0f;
	VertexList[ 2 ].v = 1.0f;
		
	VertexList[ 3 ].u = 1.0f;
	VertexList[ 3 ].v = 0.0f;

	if( FAILED( g_RenderManager.CreateVertexBuffer( ( sizeof( BILLBOARD_LINE_VERTEX ) * 4 ) ,
		D3DUSAGE_WRITEONLY, D3DFVF_BILLBOARD_LINE_VERTEX ,
		D3DPOOL_MANAGED, &m_pVB ) ) )
	{
		return FALSE;		
	}
	
	BYTE   *pVertice = NULL;
#ifdef DIRECT_VERSION_9_MJH
	if( FAILED ( m_pVB->Lock(0, 0, (void**)&pVertice, 0 ) ) )
#else
	if( FAILED ( m_pVB->Lock(0, 0, (BYTE**)&pVertice, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH	
	{
		return FALSE;
	}
	
	memcpy( pVertice , VertexList , sizeof( BILLBOARD_LINE_VERTEX ) * 4 );
	
	m_pVB->Unlock();

	return TRUE;
}

//................................................................................................................
// Render
//................................................................................................................
#ifdef DIRECT_VERSION_9_MJH
int CBillboardLine::Render(LPDIRECT3DDEVICE9 pDevice)
#else
int CBillboardLine::Render(LPDIRECT3DDEVICE8 pDevice)
#endif // DIRECT_VERSION_9_MJH
{
	g_RenderManager.SetIndices( m_pIB , 0 );
	
	g_RenderManager.SetFVF( D3DFVF_BILLBOARD_LINE_VERTEX );

	g_RenderManager.SetStreamSource( 0 , m_pVB , sizeof( BILLBOARD_LINE_VERTEX ) );

	g_RenderManager.DrawIndexedPrimitive( D3DPT_TRIANGLESTRIP , 0 , 4 , 0 , 2 );

	return TRUE;
}

//................................................................................................................
// v0 ~ v1 빌보드 축 , 넓이 , 카메라 방향 벡터
//................................................................................................................
int CBillboardLine::Update(D3DXVECTOR3 v0,D3DXVECTOR3 v1,float fWidth,
#ifdef DIRECT_VERSION_9_MJH
						   D3DXVECTOR3 vEyeDir,LPDIRECT3DDEVICE9 pDevice)
#else
						   D3DXVECTOR3 vEyeDir,LPDIRECT3DDEVICE8 pDevice)
#endif // DIRECT_VERSION_9_MJH
{
	D3DXVECTOR3 side;
	
	D3DXVECTOR3 vDist = ( v1 - v0 );

	D3DXVec3Cross( &side , &vEyeDir , &vDist );

	D3DXVec3Normalize( &side , &side );

	BILLBOARD_LINE_VERTEX   *pVertice = NULL;
#ifdef DIRECT_VERSION_9_MJH
	if( FAILED ( m_pVB->Lock(0, 0, (void**)&pVertice, 0 ) ) )
#else
	if( FAILED ( m_pVB->Lock(0, 0, (BYTE**)&pVertice, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH	
	{
		return FALSE;
	}
	
	D3DXVECTOR3 add = fWidth * side;
	
	pVertice[ 0 ].pos = ( v0 - add );
	pVertice[ 1 ].pos = ( v1 - add );
	pVertice[ 2 ].pos = ( v0 + add );
	pVertice[ 3 ].pos = ( v1 + add );

	m_pVB->Unlock();

	return TRUE;
}
