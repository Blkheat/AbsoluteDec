#include "CExpDisplay.h"
#include "..\\Effect.h"
#include "..\\..\\CRenderManager.h"

//................................................................................................................
#define EXP_NUM_NONE_FADE_TIME			1000
#define EXP_NUM_FADE_TIME				1000
#define EXP_NUM_WAIT_TIME				2500

CExpNum::CExpNum()
{
	m_OldTimer    = timeGetTime();
	m_ISFade      = FALSE;
	m_ISWaitTime  = TRUE;
	m_fAlphaValue = 1.0f;
}

BOOL CExpNum::Update(DWORD Time)
{
	if( m_ISWaitTime )
	{
		if( ( Time - m_OldTimer ) >= EXP_NUM_WAIT_TIME )
		{
			m_OldTimer = SAFE_TIME_SUB( timeGetTime() , 1 );

			m_ISWaitTime = FALSE;
		}
		else
		{
			return TRUE;
		}
	}

	if( m_ISFade == TRUE )
	{
		if( ( Time - m_OldTimer ) >= EXP_NUM_FADE_TIME )
		{
			return FALSE;
		}
		else
		{
			m_fAlphaValue = 1.0f - ( ( float ) ( Time - m_OldTimer ) / ( float ) EXP_NUM_FADE_TIME );

			return TRUE;
		}
	}
	else if( ( Time - m_OldTimer ) >= EXP_NUM_FADE_TIME && m_ISFade == FALSE )
	{
		m_ISFade = TRUE;

		m_fAlphaValue = 1.0f;
		m_OldTimer = Time;    
	}

	return TRUE;
}

//................................................................................................................






CExpDisplay::CExpDisplay()
{
	m_pTexture	  = NULL;
	m_HashCode	  = 0;
	m_pIB		  = NULL;

	for( int i = 0 ; i < 10 ; ++i )
	{
		m_pVB[ i ] = NULL;
	}
	
}

CExpDisplay::~CExpDisplay()
{
	m_pTexture    = NULL;
	SAFE_RELEASE( m_pIB	);

	for( int i = 0 ; i < 11 ; ++i )
	{
		SAFE_RELEASE( m_pVB[ i ] );
	}
}

#ifdef DIRECT_VERSION_9_MJH
void CExpDisplay::InitDevice(LPDIRECT3DDEVICE9 pDevice)
#else
void CExpDisplay::InitDevice(LPDIRECT3DDEVICE8 pDevice)
#endif // DIRECT_VERSION_9_MJH
{
	//m_pTexture    = g_Particle.m_EffectTextureManager.GetTexture( "Number2.TGA" );
	CStringToCode	HashString( "Number2.TGA" );
	m_HashCode = HashString.GetHashCode();

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
		return;		
	}
	
	BYTE   *pIndexBuffer = NULL;
#ifdef DIRECT_VERSION_9_MJH
	if( FAILED ( m_pIB->Lock(0, 0, (void**)&pIndexBuffer, 0 ) ) )
#else
	if( FAILED ( m_pIB->Lock(0, 0, (BYTE**)&pIndexBuffer, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH	
	{
		return;
	}
	
	memcpy( pIndexBuffer , IndexList , sizeof( WORD ) * 4 );
	
	m_pIB->Unlock();


	//............................................................................................................
	// 버텍스 버퍼 만들기
	//............................................................................................................
	EXPNUM_VERTEX	VertexList[ 4 ];

	float	Size    = 32.0f;
	float	MaxSize = 512;
	BYTE   *pVertice = NULL;

	VertexList[ 0 ].pos = D3DXVECTOR3( -ExpNum_Width  , -ExpNum_Height , 0.0f );
	VertexList[ 1 ].pos = D3DXVECTOR3( -ExpNum_Width  ,  ExpNum_Height , 0.0f );
	VertexList[ 2 ].pos = D3DXVECTOR3(  ExpNum_Width  , -ExpNum_Height , 0.0f );
	VertexList[ 3 ].pos = D3DXVECTOR3(  ExpNum_Width  ,  ExpNum_Height , 0.0f );

	for( int i = 0 ; i < 11 ; ++i )
	{
		if( i == 10 )
		{
			VertexList[ 0 ].pos = D3DXVECTOR3( -ExpNum_Width * 2 , -ExpNum_Height , 0.0f );
			VertexList[ 1 ].pos = D3DXVECTOR3( -ExpNum_Width * 2 ,  ExpNum_Height , 0.0f );
			VertexList[ 2 ].pos = D3DXVECTOR3(  ExpNum_Width * 2 , -ExpNum_Height , 0.0f );
			VertexList[ 3 ].pos = D3DXVECTOR3(  ExpNum_Width * 2 ,  ExpNum_Height , 0.0f );

			VertexList[ 0 ].u = 0.0f;
			VertexList[ 0 ].v = ( Size * ( i + 1 ) ) / MaxSize;
			
			VertexList[ 1 ].u = 0.0f;
			VertexList[ 1 ].v = ( Size * ( i     ) ) / MaxSize;
			
			VertexList[ 2 ].u = 1.0f;
			VertexList[ 2 ].v = ( Size * ( i + 1 ) ) / MaxSize;
			
			VertexList[ 3 ].u = 1.0f;
			VertexList[ 3 ].v = ( Size * ( i     ) ) / MaxSize;
		}
		else
		{
			VertexList[ 0 ].u = 0.0f;
			VertexList[ 0 ].v = ( Size * ( i + 1 ) ) / MaxSize;
			
			VertexList[ 1 ].u = 0.0f;
			VertexList[ 1 ].v = ( Size * ( i     ) ) / MaxSize;
			
			VertexList[ 2 ].u = 0.5f;
			VertexList[ 2 ].v = ( Size * ( i + 1 ) ) / MaxSize;
			
			VertexList[ 3 ].u = 0.5f;
			VertexList[ 3 ].v = ( Size * ( i     ) ) / MaxSize;
		}
		
		if( FAILED( g_RenderManager.CreateVertexBuffer( ( sizeof( EXPNUM_VERTEX ) * 4 ) ,
			D3DUSAGE_WRITEONLY, D3DFVF_EXPNUM_VERTEX ,
			D3DPOOL_MANAGED, &m_pVB[ i ] ) ) )
		{
			return;		
		}
#ifdef DIRECT_VERSION_9_MJH
		if( FAILED ( m_pVB[ i ]->Lock(0, 0, (void**)&pVertice, 0 ) ) )
#else
		if( FAILED ( m_pVB[ i ]->Lock(0, 0, (BYTE**)&pVertice, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH
		{
			return;
		}
		
		memcpy( pVertice , VertexList , sizeof( HITNUM_VERTEX ) * 4 );
		
		m_pVB[ i ]->Unlock();
	}

}

#ifdef DIRECT_VERSION_9_MJH
void CExpDisplay::Update(LPDIRECT3DDEVICE9 pDevice)
#else
void CExpDisplay::Update(LPDIRECT3DDEVICE8 pDevice)
#endif // DIRECT_VERSION_9_MJH
{
	std::list <CExpNum>::iterator i = m_ExpNumList.begin();
	
	DWORD NowTime = timeGetTime();

	for( ; i != m_ExpNumList.end() ; )
	{
		if( i->Update( NowTime ) )
		{
			i++;
		}
		else
		{
			m_ExpNumList.erase( i++ );
		}
	}
}

void CExpDisplay::Clear(void)
{
	m_ExpNumList.clear();
}

void CExpDisplay::Add(int Num,D3DXVECTOR3 vPos)
{
	if( Num <= 0 )
		return;
	
	RTCHAR TempStr[ 256 ]    = {0,};
	RTCHAR	TempStrNum[ 256 ] = {0,};
	CExpNum	TempExp;

	Rsprintf( TempStr ,_RT("%d") , Num );

	int Count = Rstrlen( TempStr );
	int ConvertNum	  = 0;

	for( int i = 0 ; i < Count ; i++ )
	{
		Rsprintf( (TempStrNum) , _RT("%c") , TempStr[ i ]  );

		ConvertNum = Ratoi( TempStrNum );

		TempExp.m_NumList.push_back( ConvertNum );
	}

	TempExp.m_NumList.push_back( 10 );

	TempExp.m_vLocation = vPos;

	m_ExpNumList.push_back( TempExp );
}

#ifdef DIRECT_VERSION_9_MJH
void CExpDisplay::Render(LPDIRECT3DDEVICE9 pDevice,D3DXMATRIX matView)
#else
void CExpDisplay::Render(LPDIRECT3DDEVICE8 pDevice,D3DXMATRIX matView)
#endif // DIRECT_VERSION_9_MJH
{
	std::list <CExpNum>::iterator i = m_ExpNumList.begin();
#ifdef DIRECT_VERSION_9_MJH
	D3DMATERIAL9 mtrl;
#else
	D3DMATERIAL8 mtrl;
#endif // DIRECT_VERSION_9_MJH	

	
	g_RenderManager.SetFVF( D3DFVF_EXPNUM_VERTEX );

	g_RenderManager.SetCullMode( D3DCULL_NONE );

	g_RenderManager.SetZWriteEnable( FALSE );
	g_RenderManager.SetZEnable( TRUE );
	
	g_RenderManager.SetAlphaBlendEnable(  TRUE );
	g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
	g_RenderManager.SetDestBlend( D3DBLEND_INVSRCALPHA );

	g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureColorArg1( 0 , D3DTA_TEXTURE );
	g_RenderManager.SetTextureColorArg2( 0 , D3DTA_CURRENT );
	
	g_RenderManager.SetTextureAlphaOP  ( 0 ,D3DTOP_MODULATE );
	g_RenderManager.SetTextureAlphaArg1( 0 ,D3DTA_TEXTURE );
	g_RenderManager.SetTextureAlphaArg2( 0 ,D3DTA_CURRENT );

	g_RenderManager.SetTextureMipFilter ( 0 , D3DTEXF_LINEAR );
	g_RenderManager.SetTextureMagFilter ( 0 , D3DTEXF_LINEAR );
	g_RenderManager.SetTextureMinFilter ( 0 , D3DTEXF_LINEAR );

	g_RenderManager.DisableTextureStageState( 1 );

	D3DXVECTOR3	vAdd( ExpNum_Range , 0.0f , 0.0f );
	D3DXVECTOR3	vSub;

	D3DXMATRIX	matSize;
	D3DXMATRIX	matWorld;

	D3DXMatrixIdentity( &matSize );

	g_RenderManager.SetIndices( m_pIB , 0 );

#ifdef DIRECT_VERSION_9_MJH
	ZeroMemory( &mtrl, sizeof(D3DMATERIAL9) );
#else
	ZeroMemory( &mtrl, sizeof(D3DMATERIAL8) );
#endif // DIRECT_VERSION_9_MJH
	mtrl.Power = 1.0f;
	mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
	mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
	mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;

	float Size = 2.0f;

	for( ; i != m_ExpNumList.end() ; ++i )
	{
		// 대기 모드
		if( TRUE == i->m_ISWaitTime )
			continue;

		mtrl.Diffuse.a = mtrl.Ambient.a = i->GetAlpha();
		g_RenderManager.SetMaterial( &mtrl );

		vSub.y  = 0.0f;
		vSub.z  = 0.0f;
		
		if( i->m_NumList.size() > 1 )
		{
			vAdd.x  = ( Size ) * 2.0f;
			vSub.x  = ( ( Size + 2 ) * i->m_NumList.size() ) / 2.0f;
		}
		else
		{
			vAdd.x  = 0.0f;
			vSub.x  = 0.0f;
		}
		
		vAdd.y = 0.0f;
		vAdd.z = 0.0f;
		
		g_RenderManager.SetTexture( 0 ,  g_Particle.m_EffectTextureManager.GetTexture(m_HashCode) ); 

		matSize._11 = Size;
		matSize._22 = Size;
		matSize._33 = Size;

		matView._41 = 0.0f;
		matView._42 = 0.0f;
		matView._43 = 0.0f;

		D3DXVec3TransformCoord( &vAdd , &vAdd , &matView );
		D3DXVec3TransformCoord( &vSub , &vSub , &matView );


		matView._41 = i->m_vLocation.x - vSub.x;
		matView._42 = i->m_vLocation.y - vSub.y;
		matView._43 = i->m_vLocation.z - vSub.z;
		
		for( int j = 0 ; j < i->m_NumList.size() ; ++j )
		{
			D3DXMatrixMultiply( &matWorld , &matSize , &matView );
			
			g_RenderManager.SetTransform( D3DTS_WORLD, &matWorld );
			
			
			g_RenderManager.SetStreamSource( 0 , m_pVB[ i->m_NumList[ j ] ] , sizeof( EXPNUM_VERTEX ) );
			
			g_RenderManager.DrawIndexedPrimitive( D3DPT_TRIANGLESTRIP , 0 , 4 , 0 , 2 );
			
			matView._41 += vAdd.x;
			//matView._42 += vAdd.y;
			matView._43 += vAdd.z;

			if( j == ( i->m_NumList.size() - 2 ) )
			{
				matView._41 += ( vAdd.x * 1 );
				matView._43 += ( vAdd.z * 1 );
			}
		}
	}

	g_RenderManager.SetTexture( 0 , NULL );

	g_RenderManager.SetAlphaBlendEnable(  FALSE );
	g_RenderManager.SetZWriteEnable( TRUE );
	g_RenderManager.SetZEnable( TRUE );

	g_RenderManager.SetAlphaBlendEnable(  FALSE );
	g_RenderManager.SetCullMode( D3DCULL_CCW );
}


