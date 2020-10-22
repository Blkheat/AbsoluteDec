#include "CSphere.h"

LPDIRECT3DVERTEXBUFFER8		CSphere::m_pVB = NULL;
LPDIRECT3DINDEXBUFFER8      CSphere::m_pIB = NULL;

extern IDirect3DDevice8*	g_lpDevice;

CSphere::CSphere()
{
}

CSphere::~CSphere()
{
}

int CSphere::InitDevice(void)
{
    if( FAILED( g_lpDevice->CreateVertexBuffer( SPHERE_VERTEX_COUNT * sizeof( SPHERE_VERTEX ) ,
                                                D3DUSAGE_WRITEONLY , D3DFVF_SPHERE_VERTEX,
                                                D3DPOOL_MANAGED, &m_pVB ) ) )
	{
        return FALSE;
	}
	
	//...................................................................................................
	// 구 만들기
	//...................................................................................................
    SPHERE_VERTEX *vtx;

    if( FAILED( m_pVB->Lock( 0, 0, (BYTE**)&vtx, 0 ) )
	{
		return;		
	}

    FLOAT fDeltaRingAngle = ( D3DX_PI / SPHERE_RING_COUNT );
    FLOAT fDeltaSegAngle  = ( 2.0f * D3DX_PI / SPHERE_SEGMENT_COUNT );

    for( DWORD ring = 0; ring < SPHERE_RING_COUNT ; ring++ )
    {
        FLOAT r0 = sinf( (ring+0) * fDeltaRingAngle );
        FLOAT r1 = sinf( (ring+1) * fDeltaRingAngle );
        FLOAT y0 = cosf( (ring+0) * fDeltaRingAngle );
        FLOAT y1 = cosf( (ring+1) * fDeltaRingAngle );

        for( DWORD seg = 0; seg < ( SPHERE_SEGMENT_COUNT + 1 ); seg++ )
        {
            FLOAT x0 =  r0 * sinf( seg * fDeltaSegAngle );
            FLOAT z0 =  r0 * cosf( seg * fDeltaSegAngle );
            FLOAT x1 =  r1 * sinf( seg * fDeltaSegAngle );
            FLOAT z1 =  r1 * cosf( seg * fDeltaSegAngle );

            vtx->pos = D3DXVECTOR3( x0 , y0 , z0 );
            vtx->u   = -((FLOAT)seg)/SPHERE_SEGMENT_COUNT;
            vtx->v   =  (ring+0)/(FLOAT)SPHERE_RING_COUNT;
            
            vtx++;

            vtx->pos = D3DXVECTOR3(x1,y1,z1);
            vtx->u  = -((FLOAT)seg)/SPHERE_SEGMENT_COUNT;
            vtx->v  =  (ring+1)/(FLOAT)SPHERE_RING_COUNT;

            vtx++;
        }
    }

    m_pVB->Unlock();

	if( FAILED( 
		g_lpDevice->CreateIndexBuffer( ( sizeof( WORD ) * SPHERE_VERTEX_COUNT )   ,
		D3DUSAGE_WRITEONLY	     ,
		D3DFMT_INDEX16 		 ,
		D3DPOOL_MANAGED		 ,
		&m_pIB )) )
	{
		return FALSE;		
	}

	
	WORD *pIB = NULL;

	if( FAILED( m_pIB->Lock( 0, 0, (BYTE**)&pIB, 0 ) )
	{
		return;		
	}

    for( WORD index = 0; index < SPHERE_VERTEX_COUNT ; ++index )
	{
		*pIB = index;

		++pIB;
	}
		
	m_pIB->Unlock();

	return TRUE;
}

int	CSphere::DeleteDevice(void)
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

	return TRUE;
}


int CSphere::Render(void)
{
	g_lpDevice->SetIndices( m_pIB , 0 );
	
	g_lpDevice->SetVertexShader( D3DFVF_SPHERE_VERTEX );

	g_lpDevice->SetStreamSource( 0 , m_pVB , sizeof( SPHERE_VERTEX ) );

	g_lpDevice->DrawIndexedPrimitive( D3DPT_TRIANGLESTRIP , 0 , SPHERE_VERTEX_COUNT , 0 , SPHERE_VERTEX_COUNT - 2 );

	return TRUE;
}
