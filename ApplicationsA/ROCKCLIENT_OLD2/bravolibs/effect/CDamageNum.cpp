   #include "RockPCH.h"
#include "VFileSystem.h"

#include "CDamageNum.h"
#include "effectsystem.h"
#include "..\\Effect.h"

#include <windows.h>
#include <stdio.h>

#include "..\\..\\CRenderManager.h"

int CHitNum::Update(float Time)
{
	if( m_NowTime + Time >= m_LifeTime )
	{
		return	FALSE;
	}
	else
	{
		if( m_SizeDelta > 0 &&  m_NowTime >= m_LifeTime / 2.0f )
			m_SizeDelta = m_SizeDelta * -1.0f;

		m_NowTime   	+=	Time;
		m_vPrevLocation	 =	m_vPrevLocation;


		m_Color.r		+= m_ColorDelta.r * Time;
		m_Color.g		+= m_ColorDelta.g * Time;
		m_Color.b		+= m_ColorDelta.b * Time;
		m_Color.a		+= m_ColorDelta.a * Time;

		m_vGravity		+=	m_vGravityDelta * Time;
		m_vVelocity		+=	m_vGravity * Time;
		m_vLocation		+=	m_vVelocity	* Time;
	
		m_Size		    +=  m_SizeDelta * Time;
		//m_vLocation.x   -=  m_SizeDelta * Time / 2.0f;
	}

	return TRUE;
}

CDamageNum::CDamageNum()
{
	for( int i = 0 ; i < 10 ; ++i )
	{
		m_pVB[ i ]      = NULL;
	}

	m_pTexture = NULL;
	m_pIB	   = NULL;
}

CDamageNum::~CDamageNum()
{
	for( int i = 0 ; i < 10 ; ++i )
	{
		if( m_pVB[ i ] != NULL )
		{
			m_pVB[ i ]->Release();
			m_pVB[ i ] = NULL;
		}
	}

	if( m_pTexture != NULL )
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}

	if( m_pIB != NULL )
	{
		m_pIB->Release();
		
		m_pIB = NULL;
	}
}

int	CDamageNum::Add(D3DXVECTOR3 vPos,DWORD Damage,int DamageLevel, Character *pCharacter)
{
	RLG1("============================Damage Value : %d ===================================\n" , Damage );

	if( Damage <= 0 ) return FALSE;

	CHitNum		TempData;
	TempData.m_pCharacter = pCharacter;
	
	//............................................................................................................
	// 숫자를 리스트에 넣기
	//............................................................................................................
	char		TempStr[ 512 ] = "";
	char		TempNum[ 512 ] = "";

	Rsprintf( RWCHAR(TempStr) , _RT("%d" ), Damage );

	int Len = Rstrlen( RWCHAR(TempStr) );
	int Num = 0;

	for( int i = 0 ; i < Len ; ++i )
	{
		Rsprintf( RWCHAR(TempNum ), _RT("%c") , RWCHAR(TempStr[ i ]) );

		Num =  Ratoi(RWCHAR(TempNum));

		TempData.m_NumList.push_back( Num );
	}

	//............................................................................................................
	// 파티클 초기화
	//............................................................................................................
	TempData.m_NowTime   =	0.0f;
	
	// 크리티컬
//	if( Damage >= 50 )			// 임시
	if( DamageLevel )
	{
		TempData.m_LifeTime  =  0.5f;
		TempData.m_Color.r   =  2.0f;
		TempData.m_Color.g   =  0.0f;
		TempData.m_Color.b   =  0.0f;
		TempData.m_Color.a   =  2.0f;
		
		TempData.m_ColorDelta.r	 =  0.0f;//( 0.0f - 1.0f ) / TempData.m_LifeTime;
		TempData.m_ColorDelta.g	 =  0.0f;//( 0.0f - 1.0f ) / TempData.m_LifeTime;
		TempData.m_ColorDelta.b	 =  0.0f;//( 0.0f - 1.0f ) / TempData.m_LifeTime;

		TempData.m_Size       = 0.5f; 
		TempData.m_SizeDelta  = ( 10.0f - 0.5f ) / TempData.m_LifeTime;
	
		TempData.m_vVelocity.y = 10.0f;//cosf(RandomPitch);
	}
	// 보통 
	else
	{
		TempData.m_LifeTime  =  0.6f;
		TempData.m_Color.r   =  2.0f;
		TempData.m_Color.g   =  2.0f;
		TempData.m_Color.b   =  2.0f;
		TempData.m_Color.a   =  2.0f;
		
		TempData.m_ColorDelta.r	 =  0.0f;//( 0.0f - 1.0f ) / TempData.m_LifeTime;
		TempData.m_ColorDelta.g	 =  0.0f;//( 0.0f - 1.0f ) / TempData.m_LifeTime;
		TempData.m_ColorDelta.b	 =  0.0f;//( 0.0f - 1.0f ) / TempData.m_LifeTime;

		TempData.m_Size       = 0.5f; 
		TempData.m_SizeDelta  = ( 8.0f - 0.5f ) / TempData.m_LifeTime;

		TempData.m_vVelocity.y = 9.0f;//cosf(RandomPitch);
	}

	TempData.m_ColorDelta.a	 =  ( 0.0f - 2.0f ) / TempData.m_LifeTime;
	

	TempData.m_vGravity      = D3DXVECTOR3( 0.0f , 30.0f , 0.0f );//D3DXVECTOR3( 0.0f , 20.0f , 0.0f ) + RANDOM_NUM * D3DXVECTOR3( 2.0f , 2.0f , 2.0f );
	TempData.m_vGravityDelta = D3DXVECTOR3( 0.0f , 30.0f , 0.0f );//( D3DXVECTOR3( 0.0f , -150.0f , 0.0f ) - TempData.m_vGravity ) / TempData.m_LifeTime;

	float RandomYaw      = RANDOM_NUM * 3.14159f * 2.0f;
	float RandomPitch    = RANDOM_NUM * 10 * 3.14159f / 180.0f ;
	
	TempData.m_vVelocity.x = 0.0f;//sinf(RandomPitch) * cosf(RandomYaw);
	TempData.m_vVelocity.z = 0.0f;//sinf(RandomPitch) * sinf(RandomYaw);
	
	if( g_Map.IsGiantUnit( pCharacter ) )
	{
		TempData.m_vLocation   = vPos;
	}
	else
	{
		TempData.m_vLocation   = vPos + D3DXVECTOR3( RANDOM_NUM * 5.0f , 2.0f , 0.0f );
	}
	
	//TempData.m_vLocation  += TempData.m_vVelocity;	
	
	//float fNewSpeed = 0.0f;//10 + RANDOM_NUM * 5.0f;

	//	TempData.m_vVelocity *= fNewSpeed;

	m_HitNumList.push_back( TempData );

	return TRUE;
}

#ifdef DIRECT_VERSION_9_MJH
int CDamageNum::Render(LPDIRECT3DDEVICE9 pDevice,D3DXMATRIX matView)
#else
int CDamageNum::Render(LPDIRECT3DDEVICE8 pDevice,D3DXMATRIX matView)
#endif // DIRECT_VERSION_9_MJH
{
	std::list	<CHitNum> ::iterator	i = m_HitNumList.begin();

	g_RenderManager.SetTexture( 0 ,  m_pTexture ); 
	
	g_RenderManager.SetFVF( D3DFVF_HITNUM_VERTEX );
#ifdef DIRECT_VERSION_9_MJH
	D3DMATERIAL9 mtrl;
#else
	D3DMATERIAL8 mtrl;
#endif // DIRECT_VERSION_9_MJH

	g_RenderManager.SetCullMode( D3DCULL_NONE );

	g_RenderManager.SetAlphaBlendEnable( TRUE );
	g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
	g_RenderManager.SetDestBlend( D3DBLEND_INVSRCALPHA );

	g_RenderManager.SetZWriteEnable( FALSE );
	g_RenderManager.SetZEnable( TRUE );

	g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureColorArg1( 0 , D3DTA_TEXTURE );
	g_RenderManager.SetTextureColorArg2( 0 , D3DTA_CURRENT );
	
	
	g_RenderManager.SetTextureAlphaOP  ( 0 , D3DTOP_MODULATE);
	g_RenderManager.SetTextureAlphaArg1( 0 , D3DTA_TEXTURE);
	g_RenderManager.SetTextureAlphaArg2( 0 , D3DTA_CURRENT);
	
	g_RenderManager.DisableTextureStageState( 1 );

	D3DXVECTOR3	vAdd( HitNum_Range , 0.0f , 0.0f );
	D3DXVECTOR3	vSub;

	matView._41 = 0.0f;
	matView._42 = 0.0f;
	matView._43 = 0.0f;
	
	D3DXMATRIX	matSize;
	D3DXMATRIX	matWorld;
	D3DXMATRIX	matTemp;

	D3DXMatrixIdentity( &matSize );
	D3DXMatrixIdentity( &matTemp );

	g_RenderManager.SetIndices( m_pIB , 0 );
#ifdef DIRECT_VERSION_9_MJH
	ZeroMemory( &mtrl, sizeof(D3DMATERIAL9) );
#else
	ZeroMemory( &mtrl, sizeof(D3DMATERIAL8) );
#endif // DIRECT_VERSION_9_MJH
	mtrl.Power = 1.0f;

	for( ; i != m_HitNumList.end() ; ++i )
	{
		mtrl.Diffuse.r = mtrl.Ambient.r = i->m_Color.r;
		mtrl.Diffuse.g = mtrl.Ambient.g = i->m_Color.g;
		mtrl.Diffuse.b = mtrl.Ambient.b = i->m_Color.b;
		mtrl.Diffuse.a = mtrl.Ambient.a = i->m_Color.a;

		g_RenderManager.SetMaterial( &mtrl );

		vSub.y  = 0.0f;
		vSub.z  = 0.0f;

		if( i->m_NumList.size() > 1 )
		{
			vAdd.x = i->m_Size * 2.0f;
			vSub.x  = ( i->m_Size * i->m_NumList.size() ) / 2.0f;
		}
		else
		{
			vAdd.x  = 0.0f;
			vSub.x  = 0.0f;
		}

		vAdd.y = 0.0f;
		vAdd.z = 0.0f;

		matSize._11 = i->m_Size;
		matSize._22 = i->m_Size;
		matSize._33 = i->m_Size;
	
		matView._41 = 0.0f;
		matView._42 = 0.0f;
		matView._43 = 0.0f;
		
		D3DXVec3TransformCoord( &vAdd , &vAdd , &matView );
		D3DXVec3TransformCoord( &vSub , &vSub , &matView );

		Character *pCharacter = i->m_pCharacter;		

		D3DXVECTOR3 vVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		if( g_Map.IsGiantUnit( pCharacter ) )
		{
			float fSize = (float)g_Map.m_NpcTable[pCharacter->m_nTableIndex].theNpcSize;
			//vVec = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			//D3DXVec3TransformCoord( &vVec , &vVec , &matView );
			D3DXVECTOR3	vDest = g_Camera.m_vEye;
			vDest.y = 0.0f;
			vVec = vDest - D3DXVECTOR3(pCharacter->GetBonCenterTM()._41, 
									   0.0f, 
									   pCharacter->GetBonCenterTM()._43);			
			
			vVec.y = 0.0f;
			D3DXVec3Normalize( &vVec , &vVec );
			vVec *= (fSize * 10);
		}

		matView._41 = i->m_vLocation.x - vSub.x;
		matView._42 = i->m_vLocation.y - vSub.y;
		matView._43 = i->m_vLocation.z - vSub.z;

		vAdd = vAdd * 0.8f;	
		
		for( int j = 0 ; j < i->m_NumList.size() ; ++j )
		{
			D3DXMatrixMultiply( &matWorld , &matSize , &matView );
			matWorld._41 += vVec.x;
			matWorld._42 += vVec.y;
			matWorld._43 += vVec.z;

			g_RenderManager.SetTransform( D3DTS_WORLD, &matWorld );

			g_RenderManager.SetStreamSource( 0 , m_pVB[ i->m_NumList[ j ] ] , sizeof( HITNUM_VERTEX ) );

			g_RenderManager.DrawIndexedPrimitive( D3DPT_TRIANGLESTRIP , 0 , 4 , 0 , 2 );

			matView._41 += vAdd.x;
			matView._42 += vAdd.y;
			matView._43 += vAdd.z;
		}
	}

	g_RenderManager.SetTexture( 0 , NULL );
	g_RenderManager.SetZWriteEnable( TRUE );
	g_RenderManager.SetZEnable( TRUE );
	g_RenderManager.SetCullMode( D3DCULL_CCW );
	g_RenderManager.SetAlphaBlendEnable( FALSE );
	
	return TRUE;
}

int CDamageNum::Update(float Time)
{
	std::list	<CHitNum> ::iterator	i = m_HitNumList.begin();

	for( ; i != m_HitNumList.end() ; )
	{
		if( i->Update( Time ) )
		{
			i++;
		}
		else
		{
			m_HitNumList.erase( i++ );
		}
	}

	//	DebugPrint( "Damage Num %d\n" , m_HitNumList.size() );

	return TRUE;
}

#ifdef DIRECT_VERSION_9_MJH
int CDamageNum::InitDevice(LPDIRECT3DDEVICE9 pDevice)
#else
int CDamageNum::InitDevice(LPDIRECT3DDEVICE8 pDevice)
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
	HITNUM_VERTEX	VertexList[ 4 ];

	float	Size    = 32.0f;
	float	MaxSize = 512;

	VertexList[ 0 ].pos = D3DXVECTOR3( -HitNum_Width  , -HitNum_Height , 0.0f );
	VertexList[ 1 ].pos = D3DXVECTOR3( -HitNum_Width  ,  HitNum_Height , 0.0f );
	VertexList[ 2 ].pos = D3DXVECTOR3(  HitNum_Width  , -HitNum_Height , 0.0f );
	VertexList[ 3 ].pos = D3DXVECTOR3(  HitNum_Width  ,  HitNum_Height , 0.0f );

	for( int i = 0 ; i < 10 ; ++i )
	{
		VertexList[ 0 ].u = 0.0f;
		VertexList[ 0 ].v = ( Size * ( i + 1 ) ) / MaxSize;
		
		VertexList[ 1 ].u = 0.0f;
		VertexList[ 1 ].v = ( Size * ( i     ) ) / MaxSize;
		
		VertexList[ 2 ].u = 1.0f;
		VertexList[ 2 ].v = ( Size * ( i + 1 ) ) / MaxSize;
		
		VertexList[ 3 ].u = 1.0f;
		VertexList[ 3 ].v = ( Size * ( i     ) ) / MaxSize;

		if( FAILED( g_RenderManager.CreateVertexBuffer( ( sizeof( HITNUM_VERTEX ) * 4 ) ,
			D3DUSAGE_WRITEONLY, D3DFVF_HITNUM_VERTEX ,
			D3DPOOL_MANAGED, &m_pVB[ i ] ) ) )
		{
			return FALSE;		
		}

		BYTE   *pVertice = NULL;
#ifdef DIRECT_VERSION_9_MJH
		if( FAILED ( m_pVB[ i ]->Lock(0, 0, (void**)&pVertice, 0 ) ) )
#else
		if( FAILED ( m_pVB[ i ]->Lock(0, 0, (BYTE**)&pVertice, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH		
		{
			return FALSE;
		}
		
		memcpy( pVertice , VertexList , sizeof( HITNUM_VERTEX ) * 4 );
		
		m_pVB[ i ]->Unlock();

	}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined( LOAD_VF ) /////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	if ( FAILED ( g_RenderManager.CreateTextureFromFile( "Etc\\Number.tga" , &m_pTexture ) ) )
	{
//		MessageBox( NULL, "ERROR Texture Load", "오류", NULL );

		return	false;
	}

///////////////////////////////////////////////////////////////////////////////////////////////////	
#else  ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
	
	VFileHandle* pFH = g_VFEtc.OpenFile( "Number.tga" );
	if( pFH )
	{
		g_RenderManager.CreateTextureFromFileInMemoryEx( pFH->GetData(), pFH->GetSize(),
														D3DX_DEFAULT, D3DX_DEFAULT, 
														1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
														D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
														D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
														0, NULL, NULL, &m_pTexture );
		g_VFEtc.CloseFile( pFH );
	}
///////////////////////////////////////////////////////////////////////////////////////////////////	
#endif ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////


	return TRUE;
}

//................................................................................................................
// 데미지 숫자 표시 ( 스킬에서 사용 ) 
//................................................................................................................
int CDamageNum::Add(Character *pSrc,DWORD Damage,BYTE Critical)
{
	D3DXVECTOR3	 vHeight = D3DXVECTOR3( pSrc->GetPosTM()._41 , pSrc->GetPosTM()._42 , pSrc->GetPosTM()._43 );

	if( g_Map.IsGiantUnit( pSrc ) )
	{
		vHeight.y += 38.0f; //(pSrc->GetBoundMaxPos().y - pSrc->GetBoundMaxVec().y) * 0.3f; 
							   
	}
	else
	{
		vHeight.y += pSrc->GetBoundMaxPos().y * 1.2f;
	}

	if( Damage <= 0 )
	{
//		g_Particle.DisplayUpdate( PT_MISS , vHeight );
	}

	int DamageLevel = 0;

	if( Critical != 0 )
	{
		DamageLevel = 2;
	}
	
	Add( vHeight , Damage , DamageLevel, pSrc );

	return TRUE;
}

