#include "CBillboardCurve.h"
#include "..\\..\\CRenderManager.h"

//................................................................................................................
// Create class
//................................................................................................................
CBillboardCurve::CBillboardCurve()
{
	m_pVB = NULL;
	m_pIB = NULL;
}

//................................................................................................................
// Destroy class 
//................................................................................................................
CBillboardCurve::~CBillboardCurve()
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
int CBillboardCurve::InitDevice(LPDIRECT3DDEVICE9 pDevice)
#else
int CBillboardCurve::InitDevice(LPDIRECT3DDEVICE8 pDevice)
#endif // DIRECT_VERSION_9_MJH
{
	//............................................................................................................
	// 인덱스 버퍼 만들기
	//............................................................................................................
	if( FAILED( 
		g_RenderManager.CreateIndexBuffer( ( sizeof( WORD ) * BILLBOARD_CURVE_MAX_COUNT * 2 )   ,
		D3DUSAGE_WRITEONLY	     ,
		D3DFMT_INDEX16 		 ,
		D3DPOOL_MANAGED		 ,
		&m_pIB )) )
	{
		return FALSE;		
	}

	WORD   *pIndexBuffer = NULL;
#ifdef DIRECT_VERSION_9_MJH
	if( FAILED ( m_pIB->Lock(0, 0, (void**)&pIndexBuffer, 0 ) ) )
#else
	if( FAILED ( m_pIB->Lock(0, 0, (BYTE**)&pIndexBuffer, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH	
	{
		return FALSE;
	}
	
	for( int i = 0 ; i < ( BILLBOARD_CURVE_MAX_COUNT * 2 ) ; i++ )
	{
		pIndexBuffer[ i ] = ( WORD ) i;
	}
	
	m_pIB->Unlock();
	
	//............................................................................................................
	// 버텍스 버퍼 만들기
	//............................................................................................................
	if( FAILED( g_RenderManager.CreateVertexBuffer( ( sizeof( BILLBOARD_CURVE_VERTEX ) * BILLBOARD_CURVE_MAX_COUNT * 2 ) ,
		D3DUSAGE_WRITEONLY, D3DFVF_BILLBOARD_CURVE_VERTEX ,
		D3DPOOL_MANAGED, &m_pVB ) ) )
	{
		return FALSE;		
	}
	
	return TRUE;
}

//................................................................................................................
// Render
//................................................................................................................
#ifdef DIRECT_VERSION_9_MJH
int CBillboardCurve::Render(LPDIRECT3DDEVICE9 pDevice)
#else
int CBillboardCurve::Render(LPDIRECT3DDEVICE8 pDevice)
#endif // DIRECT_VERSION_9_MJH
{
	if( m_vList.size() < BILLBOARD_CURVE_SHOW_MIN_VERTEX_SIZE ) return FALSE;

	g_RenderManager.SetIndices( m_pIB , 0 );
	
	g_RenderManager.SetFVF( D3DFVF_BILLBOARD_CURVE_VERTEX );

	g_RenderManager.SetStreamSource( 0 , m_pVB , sizeof( BILLBOARD_CURVE_VERTEX ) );

	g_RenderManager.DrawIndexedPrimitive( D3DPT_TRIANGLESTRIP , 0 , ( BILLBOARD_CURVE_MAX_COUNT * 2 ) , 0 , ( ( BILLBOARD_CURVE_MAX_COUNT * 2 ) - 2 ) );

	return TRUE;
}

int CBillboardCurve::AddPoint(D3DXVECTOR3 vPos)
{
	if( m_vList.size() > 0 )
	{
		if( vPos == *m_vList.begin() ) return FALSE;
	}

	if( m_vList.size() >= BILLBOARD_CURVE_MAX_POINT_BUFFER )
	{
		m_vList.pop_back();
	}

	m_vList.push_front( vPos );

	return TRUE;
}

//................................................................................................................
// 넓이 , 카메라 위치 , 카메라 방향 벡터
//................................................................................................................
int CBillboardCurve::Update(float fWidth,D3DXVECTOR3 vEyeDir)
{ 
	if( m_vList.size() < BILLBOARD_CURVE_SHOW_MIN_VERTEX_SIZE ) return FALSE;
	
	//..............................................................................................................
	// 곡선 분할하기
	//..............................................................................................................
	float Range;
	
	m_CurveBuffer[ 0 ].pos   = *m_vList.begin();
	m_CurveBuffer[ 0 ].v     = 0.0f;
	int i = 0;
	for( i = 1 ; i < BILLBOARD_CURVE_MAX_COUNT ; ++i )
	{	
		Range = ( float ) ( ( float ) i / ( float ) ( BILLBOARD_CURVE_MAX_COUNT - 1 ) );

		MakeBezierCurve( i , Range );
	}

	//..............................................................................................................
	// 카메라 방향으로 돌리기
	//..............................................................................................................
	
	D3DXVECTOR3  v0;
	D3DXVECTOR3  v1;
	
	D3DXVECTOR3 vDist; 
	D3DXVECTOR3 side;
	D3DXVECTOR3 add;

	int Count = 0;

	for( i = 0 ; i < BILLBOARD_CURVE_MAX_COUNT - 1; ++i )
	{
		v0 = m_CurveBuffer[ i     ].pos;
		v1 = m_CurveBuffer[ i + 1 ].pos;

		vDist = ( v1 - v0 ); 

		D3DXVec3Cross( &side , &vEyeDir , &vDist );
		D3DXVec3Normalize( &side , &side );

		add = fWidth * side;
		
		m_TraceBuffer[ Count   ].u   = 0.0f;
		m_TraceBuffer[ Count   ].v   = m_CurveBuffer[ i ].v;
		m_TraceBuffer[ Count   ].pos = ( v0 - add );
		++Count;

		m_TraceBuffer[ Count   ].u   = 1.0f;
		m_TraceBuffer[ Count   ].v   = m_CurveBuffer[ i ].v;
		m_TraceBuffer[ Count   ].pos = ( v0 + add );
		++Count;
	}

	m_TraceBuffer[ Count   ].u   = 0.0f;
	m_TraceBuffer[ Count   ].v   = m_CurveBuffer[ i ].v;
	m_TraceBuffer[ Count   ].pos = ( v1 - add );
	++Count;
	
	m_TraceBuffer[ Count   ].u   = 1.0f;
	m_TraceBuffer[ Count   ].v   = m_CurveBuffer[ i ].v;
	m_TraceBuffer[ Count   ].pos = ( v1 + add );
	++Count;

	BYTE   *pVertice = NULL;
#ifdef DIRECT_VERSION_9_MJH
	if( FAILED ( m_pVB->Lock(0, 0, (void**)&pVertice, 0 ) ) )
#else
	if( FAILED ( m_pVB->Lock(0, 0, (BYTE**)&pVertice, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH
	{
		return FALSE;
	}

	memcpy( pVertice , m_TraceBuffer , sizeof( BILLBOARD_CURVE_VERTEX ) * BILLBOARD_CURVE_MAX_COUNT * 2 );
	
	m_pVB->Unlock();
	
	return TRUE;
}


//......................................................................................................................................................
// Bernstein Polynomials ( Vezier Curve )
//
// P(u)=sigma(i=0->n){(n!/i!*(n-i)!)*u^i*(1-u)^(n-i)*Pi}
//
// 0 <= u <= 1로 변하는 동안 
//......................................................................................................................................................
void CBillboardCurve::MakeBezierCurve( int num , float Range )
{
	float Value;
	int   n     = m_vList.size() - 1;
	unsigned int i;

	m_CurveBuffer[ num     ].pos.x = 
	m_CurveBuffer[ num     ].pos.y = 
	m_CurveBuffer[ num     ].pos.z = 0.0f;

	for(  i = 0 ; i < m_vList.size() ; ++i )
	{
		Value = CalBezier( i , n , Range );
		
		m_CurveBuffer[ num     ].pos.x += m_vList[ i ].x * Value;
		m_CurveBuffer[ num     ].pos.y += m_vList[ i ].y * Value;
		m_CurveBuffer[ num     ].pos.z += m_vList[ i ].z * Value;
	}

	m_CurveBuffer[ num     ].v     = Range;
}

int CBillboardCurve::Factorial(int n)
{
	if(n == 0) return 1;

	return n * Factorial(n - 1);
}

int CBillboardCurve::Coefficient(int n, int k)
{
	return Factorial(n) / (Factorial(k) * Factorial(n - k));
}

float CBillboardCurve::CalBezier(int k, int n, float u)
{
	return (float)Coefficient(n, k) * powf(u, (float)k) * powf(1.0f - u, (float)(n - k));
}
