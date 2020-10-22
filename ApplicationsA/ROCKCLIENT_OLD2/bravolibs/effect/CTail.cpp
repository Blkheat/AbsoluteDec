#include "..\\..\\RockClient.h"
#include "..\\..\\CRenderManager.h"
#include "CTail.h"


CTail::CTail():
	m_pTexture( NULL ) ,
	m_ISDisplay( NULL ) ,
	m_pVB( NULL )
{
	
	m_pIB = NULL;	
		
	m_OldTime = timeGetTime();
	m_HashCode = 0;
}

CTail::~CTail()
{
	InvalidateDeviceObject();
}

HRESULT	CTail::InvalidateDeviceObject()
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
	
	return S_OK;
}

#ifdef DIRECT_VERSION_9_MJH
HRESULT CTail::RestoreDeviceObject(LPDIRECT3DDEVICE9 pDevice)
#else
HRESULT CTail::RestoreDeviceObject(LPDIRECT3DDEVICE8 pDevice)
#endif // DIRECT_VERSION_9_MJH
{
	InitDevice(pDevice);
	
	return S_OK;
}

void CTail::Clear(void)
{
	m_vTopList.clear();
	m_vBottomList.clear();
	
	Update();
}

#ifdef DIRECT_VERSION_9_MJH
void CTail::InitDevice(LPDIRECT3DDEVICE9 pDevice)
#else
void CTail::InitDevice(LPDIRECT3DDEVICE8 pDevice)
#endif // DIRECT_VERSION_9_MJH
{
	if( m_pVB == NULL )
	{
		if( FAILED( 
			g_RenderManager.CreateVertexBuffer( sizeof( TAIL_VERTEX ) * TRACE_MAX_COUNT * 2 , 
										 D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY	     ,
										 D3DFVF_TAIL_VERTEX							 ,
										 D3DPOOL_DEFAULT							 ,
										 &m_pVB )) )
		{
			return;		
		}
	
		if( FAILED( 
			g_RenderManager.CreateIndexBuffer( sizeof( WORD ) * ( TRACE_MAX_COUNT * 2 ) , 
										 D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY	     ,
										 D3DFMT_INDEX16 							 ,
										 D3DPOOL_DEFAULT							 ,
										 &m_pIB )) )
		{
			return;		
		}
		
		WORD   *pIndexBuffer = NULL;
#ifdef DIRECT_VERSION_9_MJH
		if( FAILED ( m_pIB->Lock(0, 0, (void**)&pIndexBuffer, 0 ) ) )
#else
		if( FAILED ( m_pIB->Lock(0, 0, (BYTE**)&pIndexBuffer, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH		
		{
			return;
		}
	
		for( int i = 0 ; i < ( TRACE_MAX_COUNT * 2 ) ; i++ )
		{
			pIndexBuffer[ i ] = ( WORD ) i;
		}

		m_pIB->Unlock();
	}
}

#ifdef DIRECT_VERSION_9_MJH
void CTail::AddPoint(D3DXVECTOR3 &v0,D3DXVECTOR3 &v1,LPDIRECT3DDEVICE9 pDevice)
#else
void CTail::AddPoint(D3DXVECTOR3 &v0,D3DXVECTOR3 &v1,LPDIRECT3DDEVICE8 pDevice)
#endif // DIRECT_VERSION_9_MJH
{
	//............................................................................................................
	// 검기 쪼메 길게 한거
	//............................................................................................................
	
	if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( timeGetTime() , m_OldTime ) , < , 50 ) )
	{
		//return;
	}

	m_OldTime = timeGetTime();

	if( m_vTopList.size() >= TAIL_MAX_POINT_BUFFER )
	{
		m_vTopList.pop_back();
		m_vBottomList.pop_back();
	}

	m_vBottomList.push_front( v0 );
	m_vTopList.push_front( v1 );

	if( m_pVB == NULL )
	{
		InitDevice( pDevice );
	}

	Update();
}

void CTail::Update(void)
{
	//..........................................................................................................
	
	if( SHOW_MIN_VERTEX_SIZE > m_vTopList.size() ) 
	{
		m_ISDisplay = FALSE;

		return;
	}

	float Range;
	int Count = 2;
	
	m_TraceBuffer[ 0 ].pos   = *m_vBottomList.begin();
	m_TraceBuffer[ 0 ].u     = 0.0f;
	m_TraceBuffer[ 0 ].v     = 1.0f;

	m_TraceBuffer[ 1 ].pos	 = *m_vTopList.begin();
	m_TraceBuffer[ 1 ].u     = 0.0f;
	m_TraceBuffer[ 1 ].v     = 0.0f;
	
	m_TraceBuffer[ 0 ].color = m_TraceBuffer[ 1 ].color = D3DCOLOR_COLORVALUE( 1 , 1 , 1 , 1.0f );

	for( int i = 2 ; i <= TRACE_MAX_COUNT ; i++ )
	{	
		Range = ( float ) ( ( float ) i / ( float ) ( TRACE_MAX_COUNT - 1 ) );

		MakeBezierCurve( Count , Range );

		Count += 2;
	}
	//..............................................................................................................
	
	BYTE   *pVertice = NULL;
#ifdef DIRECT_VERSION_9_MJH
	if( FAILED ( m_pVB->Lock(0, 0, (void**)&pVertice, 0 ) ) )
#else
	if( FAILED ( m_pVB->Lock(0, 0, (BYTE**)&pVertice, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH
	{
		return;
	}

	memcpy( pVertice , m_TraceBuffer , sizeof( TAIL_VERTEX ) * TRACE_MAX_COUNT * 2 );
	
	m_pVB->Unlock();
	
	
	m_ISDisplay = TRUE;
}

#ifdef DIRECT_VERSION_9_MJH
void CTail::Render(LPDIRECT3DDEVICE9 pDevice)
#else
void CTail::Render(LPDIRECT3DDEVICE8 pDevice)
#endif // DIRECT_VERSION_9_MJH
{
	if( !m_ISDisplay ) return;

	if(m_pTexture)
	{
		g_RenderManager.SetTexture( 0 ,  m_pTexture );
	}
	else if(m_HashCode)
	{
		g_RenderManager.SetTexture( 0 ,  g_RockClient.GetTexture(m_HashCode) );
	}
	
	g_RenderManager.SetFVF( D3DFVF_TAIL_VERTEX );

	g_RenderManager.SetStreamSource( 0 , m_pVB , sizeof( TAIL_VERTEX ) );

	g_RenderManager.SetIndices( m_pIB , 0 );
	
	g_RenderManager.SetAlphaBlendEnable( TRUE );

	g_RenderManager.DrawIndexedPrimitive( D3DPT_TRIANGLESTRIP , 0 , ( TRACE_MAX_COUNT * 2 ) , 0 , ( ( TRACE_MAX_COUNT * 2 ) - 2 ) );

	g_RenderManager.SetAlphaBlendEnable( FALSE );

	g_RenderManager.SetTexture( 0 , NULL );

}

//......................................................................................................................................................
// Bernstein Polynomials ( Vezier Curve )
//
// P(u)=sigma(i=0->n){(n!/i!*(n-i)!)*u^i*(1-u)^(n-i)*Pi}
//
// 0 <= u <= 1로 변하는 동안 
//......................................................................................................................................................
void CTail::MakeBezierCurve( int num , float Range )
{
	float Value;
	int   n     = m_vTopList.size()	- 1;
	unsigned int i;

	m_TraceBuffer[ num     ].pos.x = m_TraceBuffer[ num     ].pos.y = m_TraceBuffer[ num     ].pos.z =
	m_TraceBuffer[ num + 1 ].pos.x = m_TraceBuffer[ num + 1 ].pos.y = m_TraceBuffer[ num + 1 ].pos.z = 0.0f;

	std::deque	<D3DXVECTOR3>::iterator		ib = m_vBottomList.begin();
	std::deque	<D3DXVECTOR3>::iterator		it = m_vTopList.begin();
	
	for( i = 0 ; i < m_vTopList.size() ; i++ )
	{
		Value = CalBezier( i , n , Range );
		
		m_TraceBuffer[ num     ].pos.x += ib->x * Value;
		m_TraceBuffer[ num     ].pos.y += ib->y * Value;
		m_TraceBuffer[ num     ].pos.z += ib->z * Value;

		m_TraceBuffer[ num + 1 ].pos.x += it->x * Value;
		m_TraceBuffer[ num + 1 ].pos.y += it->y * Value;
		m_TraceBuffer[ num + 1 ].pos.z += it->z * Value;
	
		ib++;
		it++;
	}

	float Alpha = 1.0f - Range;
	
	if( Alpha < 0.0f )
	{
		Alpha = 0.0f;
	}

	m_TraceBuffer[ num     ].u     = Range;
	m_TraceBuffer[ num     ].v     = 1.0f;
	
	m_TraceBuffer[ num + 1 ].u     = Range;
	m_TraceBuffer[ num + 1 ].v     = 0.0f;

	m_TraceBuffer[ num	   ].color = m_TraceBuffer[ num + 1 ].color = D3DCOLOR_COLORVALUE( 1 , 1 , 1 , Alpha );
}

int CTail::Factorial(int n)
{
	if(n == 0) return 1;

	return n * Factorial(n - 1);
}

int CTail::Coefficient(int n, int k)
{
	return Factorial(n) / (Factorial(k) * Factorial(n - k));
}

float CTail::CalBezier(int k, int n, float u)
{
	return (float)Coefficient(n, k) * powf(u, (float)k) * powf(1.0f - u, (float)(n - k));
}
