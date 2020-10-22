#include "CFixSizeBillboard.h"
#include "..\\..\\CRenderManager.h"

CFixSizeBillboard::CFixSizeBillboard()
{
	m_VB = NULL;
	m_IB = NULL;

	D3DXMatrixIdentity( &m_matRot   );
	D3DXMatrixIdentity( &m_matScale );
	D3DXMatrixIdentity( &m_matWorld );

#ifdef DIRECT_VERSION_9_MJH
	ZeroMemory( &m_mtrl, sizeof(D3DMATERIAL9) );
#else
	ZeroMemory( &m_mtrl, sizeof(D3DMATERIAL8) );
#endif // DIRECT_VERSION_9_MJH

	m_mtrl.Power = 1.0f;
}

CFixSizeBillboard::~CFixSizeBillboard()
{
	InvalidateDeviceObject();
}

HRESULT	CFixSizeBillboard::InvalidateDeviceObject()
{
	if( m_VB != NULL )
	{
		m_VB->Release();
		m_VB = NULL;
	}

	if( m_IB != NULL )
	{
		m_IB->Release();
		m_IB = NULL;
	}	
	return S_OK;
}

#ifdef DIRECT_VERSION_9_MJH
HRESULT	CFixSizeBillboard::RestoreDeviceObject(LPDIRECT3DDEVICE9 pDevice)
#else
HRESULT	CFixSizeBillboard::RestoreDeviceObject(LPDIRECT3DDEVICE8 pDevice)
#endif // DIRECT_VERSION_9_MJH
{
	
	Init( pDevice );
	
	return S_OK;
}

#ifdef DIRECT_VERSION_9_MJH
void CFixSizeBillboard::Init(LPDIRECT3DDEVICE9 pDevice)
#else
void CFixSizeBillboard::Init(LPDIRECT3DDEVICE8 pDevice)
#endif // DIRECT_VERSION_9_MJH
{
	if( FAILED( g_RenderManager.CreateVertexBuffer( 4 * sizeof( VERTEX_FIXSIZE_BILLBOARD ) ,
			D3DUSAGE_WRITEONLY , D3DFVF_VERTEXT_FIXSIZE_BILLBOARD ,
			D3DPOOL_MANAGED, &m_VB ) ) )
	{
		return;
	}
		
	VERTEX_FIXSIZE_BILLBOARD *vtx;
	
	float fSize	= 0.5f;
#ifdef DIRECT_VERSION_9_MJH
	if( FAILED( m_VB->Lock( 0, 0, (void**)&vtx, 0 ) ) )
#else
	if( FAILED( m_VB->Lock( 0, 0, (BYTE**)&vtx, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH
	{
		return;		
	}
	
	float YPos = 0.0f;

	float offset = 0.5f / 512.0f;

	vtx[0].pos = D3DXVECTOR3( -fSize , -fSize , 0.0f );
	vtx[0].tu   = 0.0f;
	vtx[0].tv   = 1.0f;

	vtx[1].pos = D3DXVECTOR3( -fSize ,  fSize  , 0.0f );
	vtx[1].tu   = 0.0f;
	vtx[1].tv   = 0.0f;

	vtx[2].pos = D3DXVECTOR3(  fSize , -fSize , 0.0f );
	vtx[2].tu   = 1.0f;
	vtx[2].tv   = 1.0f;

	vtx[3].pos = D3DXVECTOR3(  fSize ,  fSize , 0.0f );
	vtx[3].tu   = 0.0f;
	vtx[3].tv   = 1.0f;

	m_VB->Unlock();
		
	if( FAILED( 
		g_RenderManager.CreateIndexBuffer( ( sizeof( WORD ) * 4 )   ,
		D3DUSAGE_WRITEONLY	 ,
		D3DFMT_INDEX16 		 ,
		D3DPOOL_MANAGED		 ,
		&m_IB )) )
	{
		return;		
	}
		
	WORD *pIB = NULL;
#ifdef DIRECT_VERSION_9_MJH
	if( FAILED( m_IB->Lock( 0, 0, (void**)&pIB, 0 ) ) )
#else
	if( FAILED( m_IB->Lock( 0, 0, (BYTE**)&pIB, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH
	{
		return;		
	}
		
	for( int i = 0 ; i < 4 ; ++i )
	{
		*pIB = ( WORD ) i;
	
		 pIB++;	
	}

	m_IB->Unlock();

	return;
}

void CFixSizeBillboard::SetCamera(float Fov,D3DXVECTOR3 vEyePos,D3DXMATRIX matView)
{
	matView._41 = 0.0f;	
	matView._42 = 0.0f;	
	matView._43 = 0.0f;	

	D3DXMatrixInverse( &m_matRot , NULL , &matView );

	m_vEyePos = vEyePos;
	
	m_fScale = tanf( Fov * 0.5f ) / 550.0f;
}

#ifdef DIRECT_VERSION_9_MJH
void CFixSizeBillboard::Draw(D3DXVECTOR3 vPos,float width,float height,D3DCOLORVALUE color,LPDIRECT3DDEVICE9 pDevice)
#else
void CFixSizeBillboard::Draw(D3DXVECTOR3 vPos,float width,float height,D3DCOLORVALUE color,LPDIRECT3DDEVICE8 pDevice)
#endif // DIRECT_VERSION_9_MJH
{
	D3DXVECTOR3 vLength = vPos - m_vEyePos;

	float       Length  = D3DXVec3Length( &vLength );

	D3DXMatrixIdentity( &m_matWorld );

	m_matScale._11  = m_fScale * Length * width;
	m_matScale._22  = m_fScale * Length * height;

	D3DXMatrixMultiply( &m_matWorld , &m_matWorld , &m_matScale );
	D3DXMatrixMultiply( &m_matWorld , &m_matWorld , &m_matRot   );
	
	m_matWorld._41 = vPos.x;
	m_matWorld._42 = vPos.y;
	m_matWorld._43 = vPos.z;

    m_mtrl.Diffuse.r = m_mtrl.Ambient.r = color.r;
    m_mtrl.Diffuse.g = m_mtrl.Ambient.g = color.g;
    m_mtrl.Diffuse.b = m_mtrl.Ambient.b = color.b;
    m_mtrl.Diffuse.a = m_mtrl.Ambient.a = color.a;

	g_RenderManager.SetMaterial( &m_mtrl );

	g_RenderManager.SetTransform( D3DTS_WORLD , &m_matWorld );

	g_RenderManager.SetIndices( m_IB , 0 );

	g_RenderManager.SetFVF( D3DFVF_VERTEXT_FIXSIZE_BILLBOARD );
	
	g_RenderManager.SetStreamSource( 0 , m_VB , sizeof( VERTEX_FIXSIZE_BILLBOARD ) );

	g_RenderManager.DrawIndexedPrimitive( D3DPT_TRIANGLESTRIP , 0 , 4 , 0 , 2 );
}
