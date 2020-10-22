#include "RockPCH.h"
#include "..\\..\\RockClient.h"
#include "..\\..\\VFileSystem.h" 

#include "CWeatherDrop.h"
#include	"Camera.H"

#include "..\\..\\CRenderManager.h"

#define Random(num)  ((int)(((long)rand()*(num))/(RAND_MAX+1)))
#define Randomize()  (srand((unsigned)time(NULL)))
#define RandomA(s,e) ((s)+(Random((e)-(s)+1)))

#define RANDOM_NUM      (FLOAT)((((FLOAT)rand()/(FLOAT)RAND_MAX) - 0.5f) * 2.0f)

#ifdef DIRECT_VERSION_9_MJH
extern IDirect3DDevice9*	g_lpDevice;
#else
extern IDirect3DDevice8*	g_lpDevice;
#endif // DIRECT_VERSION_9_MJH
extern long g_nowTime;

CWeatherDropData::CWeatherDropData()
{
	m_LifeTime = 0.0f;				
	m_NowTime = 0.0f;							
	m_DelayTime = 0.0f;			

	m_AniTexNum = 0;
	m_TexType   = ( rand() % 6 );

    ///--JAPAN_BUG_MODIFY
	m_vLocation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vGravity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vGravityDelta = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_vScale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

}

int CWeatherDropData::Update(float Time)
{
	if( m_DelayTime > 0 )
	{
		if( Time < m_DelayTime )
		{
			m_DelayTime -= Time;
		}
		else
		{
			m_DelayTime = 0;
		}
	}
	else
	if( m_NowTime + Time   >= m_LifeTime )
	{
		return	FALSE;
	}
	else
	{
		m_NowTime   	+=	Time;

		m_vLocation		+=	m_vGravity * Time;

		m_Color.r		+= m_ColorDelta.r * Time;
		m_Color.g		+= m_ColorDelta.g * Time;
		m_Color.b		+= m_ColorDelta.b * Time;
		m_Color.a		+= m_ColorDelta.a * Time;

		m_vGravity		+=	m_vGravityDelta * Time;
	}

	return TRUE;
}


CWeatherDrop::CWeatherDrop()
{
	m_isNight	  = false;
	m_udType	  = n_WNone;
	m_DropType    = WEATHER_TYPE_RAIN;

	m_WasBeforeNight = FALSE;

	m_pTexture[0] = NULL;
	m_pTexture[1] = NULL;
	m_pLeafTexture[0] = NULL;
	m_pLeafTexture[1] = NULL;
	m_pLeafTexture[2] = NULL;
	m_pVB		  = NULL;
	m_pIB		  = NULL;

	for( int i = 0 ; i < 6 ; ++i )
	{
		for( int j = 0 ; j < 3 ; ++j )
		{
			m_pPaperTexture[ i ][ j ] = NULL;
		}
	}

	m_Count       = 200;
}

CWeatherDrop::~CWeatherDrop()
{
	if( m_pTexture[0] != NULL )
	{
		m_pTexture[0]->Release();

		m_pTexture[0] = NULL;
	}

	if( m_pTexture[1] != NULL )
	{

		m_pTexture[1]->Release();

		m_pTexture[1] = NULL;
	}

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
	
	for( int i = 0 ; i < 3 ; ++i )
	{
		if( NULL != m_pLeafTexture[ i ] )
		{
			m_pLeafTexture[ i ] ->Release();

			m_pLeafTexture[ i ] = NULL;
		}
	}

	for(int i = 0 ; i < 6 ; ++i )
	{
		for( int j = 0 ; j < 3 ; ++j )
		{
			if( NULL != m_pPaperTexture[ i ][ j ] )
			{
				m_pPaperTexture[ i ][ j ]->Release();
				
				m_pPaperTexture[ i ][ j ] = NULL;
			}
		}
	}
}

int CWeatherDrop::CreateAll(int Num)
{
	m_List.clear();

	CWeatherDropData TempData;
	
	for( int i = 0 ; i < Num ; ++i )
	{
		m_List.push_back( TempData );

		Create( i , TRUE , D3DXVECTOR3( 0.0f , 0.0 , 0.0f ) );
	}
	
	m_Count = Num;

	return TRUE;
}

void CWeatherDrop::Init()
{
	m_Character_Light = g_Pc_Manager.theCharacter_Light;
	m_Object_Light = g_Pc_Manager.theObject_Light;
	m_Terrain_Light = g_Pc_Manager.theTerrain_Light;
	m_Sky_Light = g_Pc_Manager.theSky_Light;

	LPENV_DATA SetEnvData = g_Map.m_SetEnv.GetSet();	

	m_Fog_Color.x = SetEnvData->FogR;
	m_Fog_Color.y = SetEnvData->FogG;
	m_Fog_Color.z = SetEnvData->FogB;	

	m_lPrevtime = g_nowTime;

	if( !m_WasBeforeNight )
	{
		m_isNight = false;
	}
	else
	{
		float fRatio = 1.0f;

		m_Character_Light.Ambient.r = 
		g_Pc_Manager.theCharacter_Light.Ambient.r 
		- (g_Pc_Manager.theCharacter_Light.Ambient.r - 0.85f) * fRatio;
		m_Character_Light.Ambient.g = 	
			g_Pc_Manager.theCharacter_Light.Ambient.g 
			- (g_Pc_Manager.theCharacter_Light.Ambient.g - 0.85f) * fRatio;
		m_Character_Light.Ambient.b = 	
			g_Pc_Manager.theCharacter_Light.Ambient.b 
			- (g_Pc_Manager.theCharacter_Light.Ambient.b - 0.85f) * fRatio;
		m_Character_Light.Diffuse = m_Character_Light.Ambient;
	
		m_Object_Light.Ambient.r = 
			g_Pc_Manager.theObject_Light.Ambient.r 
			- (g_Pc_Manager.theObject_Light.Ambient.r - 0.75f) * fRatio;
		m_Object_Light.Ambient.g = 	
			g_Pc_Manager.theObject_Light.Ambient.g 
			- (g_Pc_Manager.theObject_Light.Ambient.g - 0.75f) * fRatio;
		m_Object_Light.Ambient.b = 	
			g_Pc_Manager.theObject_Light.Ambient.b 
			- (g_Pc_Manager.theObject_Light.Ambient.b - 0.75f) * fRatio;
		m_Object_Light.Diffuse = m_Object_Light.Ambient;

		m_Terrain_Light.Ambient.r = 
			g_Pc_Manager.theTerrain_Light.Ambient.r 
			- (g_Pc_Manager.theTerrain_Light.Ambient.r - 0.75f) * fRatio;
		m_Terrain_Light.Ambient.g = 	
			g_Pc_Manager.theTerrain_Light.Ambient.g 
			- (g_Pc_Manager.theTerrain_Light.Ambient.g - 0.75f) * fRatio;
		m_Terrain_Light.Ambient.b = 	
			g_Pc_Manager.theTerrain_Light.Ambient.b 
			- (g_Pc_Manager.theTerrain_Light.Ambient.b - 0.75f) * fRatio;
		m_Terrain_Light.Diffuse = m_Terrain_Light.Ambient;

		m_Sky_Light.Ambient.r = 
			g_Pc_Manager.theSky_Light.Ambient.r 
			- (g_Pc_Manager.theSky_Light.Ambient.r - 0.7f) * fRatio;
		m_Sky_Light.Ambient.g = 	
			g_Pc_Manager.theSky_Light.Ambient.g 
			- (g_Pc_Manager.theSky_Light.Ambient.g - 0.7f) * fRatio;
		m_Sky_Light.Ambient.b = 	
			g_Pc_Manager.theSky_Light.Ambient.b 
			- (g_Pc_Manager.theSky_Light.Ambient.b - 0.7f) * fRatio;
		m_Sky_Light.Diffuse = m_Sky_Light.Ambient;
		
		
		m_Fog_Color.x = g_Pc_Manager.theFog_Color.x
						- ( g_Pc_Manager.theFog_Color.x - 40 ) * fRatio;
		m_Fog_Color.y = g_Pc_Manager.theFog_Color.y
						- ( g_Pc_Manager.theFog_Color.y - 40 ) * fRatio;
		m_Fog_Color.z = g_Pc_Manager.theFog_Color.z
						- ( g_Pc_Manager.theFog_Color.z - 40 ) * fRatio;
		
		m_Sky_Light.Ambient.r = 0.7f;
		m_Sky_Light.Ambient.g = 0.7f;
		m_Sky_Light.Ambient.b = 0.7f;

		m_Sky_Light.Diffuse = m_Sky_Light.Ambient;

		m_udType = n_WNone;
		m_lPrevtime = g_nowTime;
		g_Map.m_SkyBox.LoadTexture( g_lpDevice , 18 , 0 );
		m_isNight = true;
	}
}

#ifdef DIRECT_VERSION_9_MJH
int CWeatherDrop::InitDevice(LPDIRECT3DDEVICE9 pDevice)
#else
int CWeatherDrop::InitDevice(LPDIRECT3DDEVICE8 pDevice)
#endif // DIRECT_VERSION_9_MJH
{
	//............................................................................................................
	// 인덱스 버퍼 만들기
	//............................................................................................................
	WORD IndexList[ 4 ] = 
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
	BYTE   *pVertice = NULL;
		
	WEATHER_DROP_VERTEX	VertexList[ 4 ];

	VertexList[ 0 ].u = 0.0f;
	VertexList[ 0 ].v = 1.0f;
		
	VertexList[ 1 ].u = 0.0f;
	VertexList[ 1 ].v = 0.0f;
		
	VertexList[ 2 ].u = 1.0f;
	VertexList[ 2 ].v = 1.0f;
		
	VertexList[ 3 ].u = 1.0f;
	VertexList[ 3 ].v = 0.0f;

	VertexList[ 0 ].pos = D3DXVECTOR3( -0.5f , -0.5f , 0.0f );
	VertexList[ 1 ].pos = D3DXVECTOR3( -0.5f ,  0.5f , 0.0f );
	VertexList[ 2 ].pos = D3DXVECTOR3(  0.5f , -0.5f , 0.0f );
	VertexList[ 3 ].pos = D3DXVECTOR3(  0.5f ,  0.5f , 0.0f );

	if( FAILED( g_RenderManager.CreateVertexBuffer( ( sizeof( WEATHER_DROP_VERTEX ) * 4 ) ,
				D3DUSAGE_WRITEONLY, D3DFVF_WEATHER_DROP_VERTEX ,
				D3DPOOL_MANAGED, &m_pVB ) ) )
	{
		return FALSE;		
	}
#ifdef DIRECT_VERSION_9_MJH
	if( FAILED ( m_pVB->Lock(0, 0, (void**)&pVertice, 0 ) ) )
#else
	if( FAILED ( m_pVB->Lock(0, 0, (BYTE**)&pVertice, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH
	{
		return FALSE;
	}
		
	memcpy( pVertice , VertexList , sizeof( WEATHER_DROP_VERTEX ) * 4 );
		
	m_pVB->Unlock();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined( LOAD_VF ) /////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	if ( FAILED ( g_RenderManager.CreateTextureFromFile( "Etc\\Snow.tga" , &m_pTexture[ 0 ] ) ) )
	{
///		MessageBox( NULL, "Etc\\Snow.tga ERROR Texture Load", "오류", NULL );
	
		return FALSE;
	}

	if ( FAILED ( g_RenderManager.CreateTextureFromFile( "Etc\\Rain.tga" , &m_pTexture[ 1 ] ) ) )
	{
///		MessageBox( NULL, "Etc\\Snow.tga ERROR Texture Load", "오류", NULL );
	
		return FALSE;
	}


	char TempStr[ 1024 ] = "";

	for( int i = 0 ; i < 3 ; ++i ) 
	{
		wsprintf( TempStr , "Etc\\sakura%d.TGA" , i );

		if ( FAILED ( g_RenderManager.CreateTextureFromFile( TempStr, &m_pLeafTexture[ i ] ) ) )
		{
			return FALSE;
		}
		
	}

///////////////////////////////////////////////////////////////////////////////////////////////////	
#else  ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

	VFileHandle* pFH = g_VFEtc.OpenFile( "Snow.tga" );

	if( pFH )
	{
		g_RenderManager.CreateTextureFromFileInMemoryEx( pFH->GetData(), pFH->GetSize(),
														D3DX_DEFAULT, D3DX_DEFAULT, 
														1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
														D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
														D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
														0, NULL, NULL, &m_pTexture[ 0 ] );
		g_VFEtc.CloseFile( pFH );
	}

	pFH = g_VFEtc.OpenFile( "Rain.tga" );

	if( pFH )
	{
		g_RenderManager.CreateTextureFromFileInMemoryEx( pFH->GetData(), pFH->GetSize(),
														D3DX_DEFAULT, D3DX_DEFAULT, 
														1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
														D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
														D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
														0, NULL, NULL, &m_pTexture[ 1 ] );
		g_VFEtc.CloseFile( pFH );
	}

	char TempStr[ 1024 ] = "";

	for( int i = 0 ; i < 3 ; ++i ) 
	{
		wsprintf( TempStr , "sakura0%d.TGA" , i + 1 );
	
		pFH = g_VFEtc.OpenFile( TempStr );

		if( pFH )
		{
			g_RenderManager.CreateTextureFromFileInMemoryEx( pFH->GetData(), pFH->GetSize(),
															D3DX_DEFAULT, D3DX_DEFAULT, 
															1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
															D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
															D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
															0, NULL, NULL, &m_pLeafTexture[ i ] );
			g_VFEtc.CloseFile( pFH );
		}
	}

	for( int i = 0 ; i < 6 ; ++i ) 
	{
		for( int j = 0 ; j < 3 ; ++j )
		{
			wsprintf( TempStr , "paper%d%d.TGA" , i , j );
			
			pFH = g_VFEtc.OpenFile( TempStr );
			
			if( pFH )
			{
				g_RenderManager.CreateTextureFromFileInMemoryEx( pFH->GetData(), pFH->GetSize(),
														D3DX_DEFAULT, D3DX_DEFAULT, 
														1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
														D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
														D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
														0, NULL, NULL, &m_pPaperTexture	[ i ][ j ] );
				g_VFEtc.CloseFile( pFH );
			}
		}
	}

	

///////////////////////////////////////////////////////////////////////////////////////////////////	
#endif ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
	
	return TRUE;
}

int CWeatherDrop::Create(int i,BOOL IsOnetime,D3DXVECTOR3 Pos)
{
	if( m_DropType == WEATHER_TYPE_RAIN )
	{
		m_List[ i ].m_vGravity  = D3DXVECTOR3( 0.0f , -300.0f , 0.0f ) + RANDOM_NUM * D3DXVECTOR3( 3.0f , -200.0f , 3.0f );

		m_List[ i ].m_LifeTime  = 5.0f + RANDOM_NUM * 5.0f;

		m_List[ i ].m_vScale.x  = RANDOM_NUM * 0.2f + 0.8f;
		m_List[ i ].m_vScale.y  = 15.0f + RANDOM_NUM * 30.0f;
		m_List[ i ].m_vScale.z  = 1.0f;
	}
	else if( m_DropType == WEATHER_TYPE_SNOW )
	{
//		m_List[ i ].m_vGravity  = D3DXVECTOR3( 0.0f , -80.0f , 0.0f ) + RANDOM_NUM * D3DXVECTOR3( 15.5f , -30.0f , 15.5f );
		m_List[ i ].m_vGravity  = D3DXVECTOR3( 0.0f , -50.0f , 0.0f ) + RANDOM_NUM * D3DXVECTOR3( 15.5f , -10.0f , 15.5f );

		m_List[ i ].m_LifeTime  = 10.0f + RANDOM_NUM * 10.0f;
		m_List[ i ].m_vScale.x  = m_List[ i ].m_vScale.y  = 2.0f + RANDOM_NUM * 1.0f;
		m_List[ i ].m_vScale.z  = 1.0f;
	}
	else if( m_DropType == WEATHER_TYPE_LEAF )
	{
		m_List[ i ].m_vGravity  = D3DXVECTOR3( 0.0f , -15.0f , 0.0f ) + RANDOM_NUM * D3DXVECTOR3( 15.5f , -5.0f , 15.5f );

		m_List[ i ].m_LifeTime  = 10.0f + RANDOM_NUM * 10.0f;
		m_List[ i ].m_vScale.x  = m_List[ i ].m_vScale.y  = 2.0f + RANDOM_NUM * 1.0f;
		m_List[ i ].m_vScale.z  = 1.0f;

		m_List[ i ].m_AniTimeRange = RandomA( 100 , 300 );
		m_List[ i ].m_OldAniTime   = g_nowTime;
		m_List[ i ].m_AniTexNum    = 0;
	}
	else if( m_DropType == WEATHER_TYPE_PAPER )
	{
		m_List[ i ].m_vGravity  = D3DXVECTOR3( 0.0f , -25.0f , 0.0f ) + RANDOM_NUM * D3DXVECTOR3( 15.5f , -5.0f , 15.5f );

		m_List[ i ].m_LifeTime  = 10.0f + RANDOM_NUM * 10.0f;
		m_List[ i ].m_vScale.x  = m_List[ i ].m_vScale.y  = 2.0f + RANDOM_NUM * 1.0f;
		m_List[ i ].m_vScale.z  = 1.0f;

		m_List[ i ].m_AniTimeRange = RandomA( 100 , 300 );
		m_List[ i ].m_OldAniTime   = g_nowTime;
		m_List[ i ].m_AniTexNum    = 0;
		m_List[ i ].m_TexType	   = ( rand() % 6 );
	}

	m_List[ i ].m_vPos	     	 = Pos;

	m_List[ i ].m_vLocation.x    = RandomA( WEATHER_DROP_MIN_X , WEATHER_DROP_MAX_X );
	
	m_List[ i ].m_vLocation.y    = Pos.y;

	m_List[ i ].m_vLocation.z    = RandomA( WEATHER_DROP_MIN_Z , WEATHER_DROP_MAX_Z );
	
	m_List[ i ].m_Color.r   =  0.6f;
	m_List[ i ].m_Color.g   =  0.6f;
	m_List[ i ].m_Color.b   =  0.6f;
	m_List[ i ].m_Color.a   =  0.7f;
	
	m_List[ i ].m_ColorDelta.r	 =  ( 0.0f - m_List[ i ].m_Color.r ) / m_List[ i ].m_LifeTime;
	m_List[ i ].m_ColorDelta.g	 =  ( 0.0f - m_List[ i ].m_Color.g ) / m_List[ i ].m_LifeTime;
	m_List[ i ].m_ColorDelta.b	 =  ( 0.0f - m_List[ i ].m_Color.b ) / m_List[ i ].m_LifeTime;
	m_List[ i ].m_ColorDelta.a	 =  ( 0.0f - m_List[ i ].m_Color.a ) / m_List[ i ].m_LifeTime;
	
	m_List[ i ].m_vGravityDelta = m_List[ i ].m_vGravity / m_List[ i ].m_LifeTime;
	
	if( IsOnetime ==  TRUE )
	{
		m_List[ i ].m_DelayTime = RANDOM_NUM * 10.0f;
		
		if( m_List[ i ].m_DelayTime < 0 ) 
		{
			m_List[ i ].m_DelayTime = m_List[ i ].m_DelayTime * -1.0f;
		}
	}
	else
	{
		m_List[ i ].m_DelayTime = 0.0f;
	}

	m_List[ i ].m_NowTime   =	0.0f;

	m_Fade.Init( 1024 , 1024 );

	return TRUE;
}

int CWeatherDrop::Update(float Time,D3DXVECTOR3 Pos)
{
	std::vector <CWeatherDropData>	::iterator i = m_List.begin();
	
	int Count = 0;

	for( ; i != m_List.end() ; ++i )
	{
		if( !i->Update( Time ) ||
			 i->m_vLocation.y < WEATHER_DROP_MIN_Y )
		{
			Create( Count , FALSE , Pos );
		}
		else
		{
			if( m_DropType == WEATHER_TYPE_LEAF || m_DropType == WEATHER_TYPE_PAPER )
			{
				if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( g_nowTime , i->m_OldAniTime ) , > , i->m_AniTimeRange ) )
				{
					i->m_AniTexNum++;
					
					if( i->m_AniTexNum > 2 )
						i->m_AniTexNum = 0;

					i->m_OldAniTime = g_nowTime;
				}
			}
		}

		
		Count++;
	}

	return TRUE;
}

#ifdef DIRECT_VERSION_9_MJH
int CWeatherDrop::Render(LPDIRECT3DDEVICE9 pDevice,D3DXMATRIX matView,D3DXVECTOR3 Pos)
#else
int CWeatherDrop::Render(LPDIRECT3DDEVICE8 pDevice,D3DXMATRIX matView,D3DXVECTOR3 Pos)
#endif // DIRECT_VERSION_9_MJH
{
	
	///--JAPAN_BUG_MODIFY
	DWORD dColorOp = g_RenderManager.GetTextureColorOP(1);
	DWORD dAlphaOp = g_RenderManager.GetTextureAlphaOP(1);
	
	g_RenderManager.SetTextureColorOP( 1 , D3DTOP_DISABLE );
	g_RenderManager.SetTextureAlphaOP( 1 , D3DTOP_DISABLE );
	
	switch( m_DropType )
	{
		case WEATHER_TYPE_NONE:	

			return TRUE;
			
		case WEATHER_TYPE_RAIN:
			
			RenderRain( pDevice , matView , Pos );

			break;

		case WEATHER_TYPE_SNOW:
			
			RenderSnow( pDevice , matView , Pos );

			break;

		case WEATHER_TYPE_LEAF:
			
			RenderLeaf( pDevice , matView , Pos );

			break;

		case WEATHER_TYPE_PAPER:

			RenderPapaer( pDevice , matView , Pos );
	}

	g_RenderManager.SetTextureColorOP( 1 , dColorOp );
	g_RenderManager.SetTextureAlphaOP( 1 , dAlphaOp );

	return TRUE;
}

#ifdef DIRECT_VERSION_9_MJH
int CWeatherDrop::RenderRain(LPDIRECT3DDEVICE9 pDevice,D3DXMATRIX matView,D3DXVECTOR3 Pos)
#else
int CWeatherDrop::RenderRain(LPDIRECT3DDEVICE8 pDevice,D3DXMATRIX matView,D3DXVECTOR3 Pos)
#endif // DIRECT_VERSION_9_MJH
{
	g_RenderManager.SetTexture( 0 ,  m_pTexture[ 1 ] ); 
	
    g_RenderManager.SetFVF( D3DFVF_WEATHER_DROP_VERTEX );

	D3DXMATRIX	matWorld;
	D3DXMATRIX	matPos;

	//D3DXMatrixInverse( &matView , NULL , &matView ); 

	g_RenderManager.SetAlphaBlendEnable( TRUE );
	g_RenderManager.SetSrcBlend( D3DBLEND_ONE );
	g_RenderManager.SetDestBlend( D3DBLEND_ONE );
	
	g_RenderManager.SetZWriteEnable( FALSE );
	g_RenderManager.SetCullMode( D3DCULL_NONE );

	g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureColorArg1( 0 , D3DTA_TEXTURE );
	g_RenderManager.SetTextureColorArg2( 0 , D3DTA_CURRENT );
	
	
	g_RenderManager.SetTextureAlphaOP  ( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureAlphaArg1( 0 , D3DTA_TEXTURE );
	g_RenderManager.SetTextureAlphaArg2( 0 , D3DTA_CURRENT );

	matView._41 = 0.0f;
	matView._42 = 0.0f;
	matView._43 = 0.0f;

	g_RenderManager.SetIndices( m_pIB , 0 );

	std::vector <CWeatherDropData>	::iterator i = m_List.begin();

#ifdef DIRECT_VERSION_9_MJH
	D3DMATERIAL9 mtrl;
#else
	D3DMATERIAL8 mtrl;
#endif // DIRECT_VERSION_9_MJH
	
	ZeroMemory( &mtrl , sizeof( mtrl ) );
	mtrl.Power = 1.0f;

	g_RenderManager.SetStreamSource( 0 , m_pVB , sizeof( WEATHER_DROP_VERTEX ) );

	matPos = matView;

	for( ; i != m_List.end() ; ++i )
	{
		if( i->m_DelayTime > 0 )
			continue;
		
		D3DXMatrixRotationY( &matWorld, -g_Camera.m_fCameraYaw );
		D3DXMatrixIdentity( &matPos );
		
		matPos._11 = i->m_vScale.x;
		matPos._22 = i->m_vScale.y;
		matPos._33 = i->m_vScale.z;

		D3DXMatrixMultiply( &matPos , &matPos , &matWorld );
/*
		matPos._41 = Pos.x + i->m_vLocation.x;
		matPos._42 = i->m_vLocation.y;
		matPos._43 = Pos.z + i->m_vLocation.z;
*/
		matPos._41 = i->m_vPos.x + i->m_vLocation.x;
		matPos._42 = i->m_vLocation.y;
		matPos._43 = i->m_vPos.z + i->m_vLocation.z;

		g_RenderManager.SetTransform( D3DTS_WORLD, &matPos );

		mtrl.Diffuse.r = mtrl.Ambient.r = i->m_Color.r;

		mtrl.Diffuse.g = mtrl.Ambient.g = i->m_Color.g;

		mtrl.Diffuse.b = mtrl.Ambient.b = i->m_Color.b;

		mtrl.Diffuse.a = mtrl.Ambient.a = i->m_Color.a;

		g_RenderManager.SetMaterial( &mtrl );

		g_RenderManager.DrawIndexedPrimitive( D3DPT_TRIANGLESTRIP , 0 , 4 , 0 , 2 );
	}

	g_RenderManager.SetTexture( 0 , NULL );

	g_RenderManager.SetZWriteEnable( TRUE );

	g_RenderManager.SetAlphaBlendEnable( FALSE );
	g_RenderManager.SetCullMode( D3DCULL_CCW );

	return TRUE;

}

#ifdef DIRECT_VERSION_9_MJH
int CWeatherDrop::RenderSnow(LPDIRECT3DDEVICE9 pDevice,D3DXMATRIX matView,D3DXVECTOR3 Pos)
#else
int CWeatherDrop::RenderSnow(LPDIRECT3DDEVICE8 pDevice,D3DXMATRIX matView,D3DXVECTOR3 Pos)
#endif // DIRECT_VERSION_9_MJH
{
	g_RenderManager.SetTexture( 0 ,  m_pTexture[ 0 ] ); 
	
	g_RenderManager.SetFVF( D3DFVF_WEATHER_DROP_VERTEX );

	D3DXMATRIX	matWorld;
	D3DXMATRIX	matPos;

	g_RenderManager.SetAlphaBlendEnable( TRUE );
	g_RenderManager.SetSrcBlend( D3DBLEND_ONE );
	g_RenderManager.SetDestBlend( D3DBLEND_ONE );
	g_RenderManager.SetZWriteEnable( FALSE );
	
	g_RenderManager.SetCullMode( D3DCULL_NONE );

	g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureColorArg1( 0 , D3DTA_TEXTURE );
	g_RenderManager.SetTextureColorArg2( 0 , D3DTA_CURRENT );
	
	
	g_RenderManager.SetTextureAlphaOP  ( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureAlphaArg1( 0 , D3DTA_TEXTURE );
	g_RenderManager.SetTextureAlphaArg2( 0 , D3DTA_CURRENT );

	matView._41 = 0.0f;
	matView._42 = 0.0f;
	matView._43 = 0.0f;

	g_RenderManager.SetIndices( m_pIB , 0 );

	std::vector <CWeatherDropData>	::iterator i = m_List.begin();

#ifdef DIRECT_VERSION_9_MJH
	D3DMATERIAL9 mtrl;
#else
	D3DMATERIAL8 mtrl;
#endif // DIRECT_VERSION_9_MJH
	
	ZeroMemory( &mtrl , sizeof( mtrl ) );
	mtrl.Power = 1.0f;

	g_RenderManager.SetStreamSource( 0 , m_pVB , sizeof( WEATHER_DROP_VERTEX ) );

	//matPos = matView;

	D3DXMatrixInverse( &matView  , NULL , &matView ); 

	for( ; i != m_List.end() ; ++i )
	{
		if( i->m_DelayTime > 0 )
			continue;

		D3DXMatrixScaling( &matWorld , i->m_vScale.x , i->m_vScale.y , i->m_vScale.z );

		D3DXMatrixMultiply( &matPos , &matWorld , &matView );

/*		
		matPos._41 = Pos.x + i->m_vLocation.x;
		matPos._42 = i->m_vLocation.y;
		matPos._43 = Pos.z + i->m_vLocation.z;
*/
		matPos._41 = i->m_vPos.x + i->m_vLocation.x;
		matPos._42 = i->m_vLocation.y;
		matPos._43 = i->m_vPos.z + i->m_vLocation.z;

		g_RenderManager.SetTransform( D3DTS_WORLD, &matPos );

		mtrl.Diffuse.r = mtrl.Ambient.r = i->m_Color.r;

		mtrl.Diffuse.g = mtrl.Ambient.g = i->m_Color.g;

		mtrl.Diffuse.b = mtrl.Ambient.b = i->m_Color.b;

		mtrl.Diffuse.a = mtrl.Ambient.a = i->m_Color.a;

		g_RenderManager.SetMaterial( &mtrl );

		g_RenderManager.DrawIndexedPrimitive( D3DPT_TRIANGLESTRIP , 0 , 4 , 0 , 2 );
	}

	g_RenderManager.SetTexture( 0 , NULL );
	
	g_RenderManager.SetCullMode( D3DCULL_CCW );
	
	g_RenderManager.SetZWriteEnable( TRUE );
	
	g_RenderManager.SetAlphaBlendEnable(  FALSE );

	return TRUE;
}

#ifdef DIRECT_VERSION_9_MJH
int  CWeatherDrop::RenderLeaf(LPDIRECT3DDEVICE9 pDevice,D3DXMATRIX matView,D3DXVECTOR3 Pos)
#else
int  CWeatherDrop::RenderLeaf(LPDIRECT3DDEVICE8 pDevice,D3DXMATRIX matView,D3DXVECTOR3 Pos)
#endif // DIRECT_VERSION_9_MJH
{
	g_RenderManager.SetFVF( D3DFVF_WEATHER_DROP_VERTEX );

	D3DXMATRIX	matWorld;
	D3DXMATRIX	matPos;

	g_RenderManager.SetAlphaBlendEnable( TRUE );
	g_RenderManager.SetSrcBlend( D3DBLEND_ONE );
	g_RenderManager.SetDestBlend( D3DBLEND_ONE );
	g_RenderManager.SetZWriteEnable( FALSE );
	
	g_RenderManager.SetCullMode( D3DCULL_NONE );

	g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureColorArg1( 0 , D3DTA_TEXTURE );
	g_RenderManager.SetTextureColorArg2( 0 , D3DTA_CURRENT );
	
	
	g_RenderManager.SetTextureAlphaOP  ( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureAlphaArg1( 0 , D3DTA_TEXTURE );
	g_RenderManager.SetTextureAlphaArg2( 0 , D3DTA_CURRENT );

	matView._41 = 0.0f;
	matView._42 = 0.0f;
	matView._43 = 0.0f;

	g_RenderManager.SetIndices( m_pIB , 0 );

	std::vector <CWeatherDropData>	::iterator i = m_List.begin();

#ifdef DIRECT_VERSION_9_MJH
	D3DMATERIAL9 mtrl;
#else
	D3DMATERIAL8 mtrl;
#endif // DIRECT_VERSION_9_MJH
	
	ZeroMemory( &mtrl , sizeof( mtrl ) );
	mtrl.Power = 1.0f;

	g_RenderManager.SetStreamSource( 0 , m_pVB , sizeof( WEATHER_DROP_VERTEX ) );

	//matPos = matView;

	D3DXMatrixInverse( &matView  , NULL , &matView ); 

	for( ; i != m_List.end() ; ++i )
	{
		if( i->m_DelayTime > 0 )
			continue;

		D3DXMatrixScaling( &matWorld , i->m_vScale.x , i->m_vScale.y , i->m_vScale.z );

		D3DXMatrixMultiply( &matPos , &matWorld , &matView );

		matPos._41 = i->m_vPos.x + i->m_vLocation.x;
		matPos._42 = i->m_vLocation.y;
		matPos._43 = i->m_vPos.z + i->m_vLocation.z;

		g_RenderManager.SetTexture( 0 , m_pLeafTexture[ i->m_AniTexNum ] ); 
		
		g_RenderManager.SetTransform( D3DTS_WORLD, &matPos );

		mtrl.Diffuse.r = mtrl.Ambient.r = i->m_Color.r;

		mtrl.Diffuse.g = mtrl.Ambient.g = i->m_Color.g;

		mtrl.Diffuse.b = mtrl.Ambient.b = i->m_Color.b;

		mtrl.Diffuse.a = mtrl.Ambient.a = i->m_Color.a;

		g_RenderManager.SetMaterial( &mtrl );

		g_RenderManager.DrawIndexedPrimitive( D3DPT_TRIANGLESTRIP , 0 , 4 , 0 , 2 );
	}

	g_RenderManager.SetTexture( 0 , NULL );
	
	g_RenderManager.SetCullMode( D3DCULL_CCW );
	
	g_RenderManager.SetZWriteEnable( TRUE );
	
	g_RenderManager.SetAlphaBlendEnable(  FALSE );

	return TRUE;
}

#ifdef DIRECT_VERSION_9_MJH
int CWeatherDrop::RenderPapaer(LPDIRECT3DDEVICE9 pDevice,D3DXMATRIX matView,D3DXVECTOR3 Pos)
#else
int CWeatherDrop::RenderPapaer(LPDIRECT3DDEVICE8 pDevice,D3DXMATRIX matView,D3DXVECTOR3 Pos)
#endif // DIRECT_VERSION_9_MJH
{
	g_RenderManager.SetFVF( D3DFVF_WEATHER_DROP_VERTEX );

	D3DXMATRIX	matWorld;
	D3DXMATRIX	matPos;

	g_RenderManager.SetAlphaBlendEnable( TRUE );
	g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
	g_RenderManager.SetDestBlend( D3DBLEND_INVSRCALPHA );
	g_RenderManager.SetZWriteEnable( FALSE );
	
	g_RenderManager.SetCullMode( D3DCULL_NONE );

	g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureColorArg1( 0 , D3DTA_TEXTURE );
	g_RenderManager.SetTextureColorArg2( 0 , D3DTA_CURRENT );
	
	
	g_RenderManager.SetTextureAlphaOP  ( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureAlphaArg1( 0 , D3DTA_TEXTURE );
	g_RenderManager.SetTextureAlphaArg2( 0 , D3DTA_CURRENT );

	matView._41 = 0.0f;
	matView._42 = 0.0f;
	matView._43 = 0.0f;

	g_RenderManager.SetIndices( m_pIB , 0 );

	std::vector <CWeatherDropData>	::iterator i = m_List.begin();

#ifdef DIRECT_VERSION_9_MJH
	D3DMATERIAL9 mtrl;
#else
	D3DMATERIAL8 mtrl;
#endif // DIRECT_VERSION_9_MJH
	
	ZeroMemory( &mtrl , sizeof( mtrl ) );
	mtrl.Power = 1.0f;

	g_RenderManager.SetStreamSource( 0 , m_pVB , sizeof( WEATHER_DROP_VERTEX ) );

	//matPos = matView;

	D3DXMatrixInverse( &matView  , NULL , &matView ); 

	for( ; i != m_List.end() ; ++i )
	{
		if( i->m_DelayTime > 0 )
			continue;

		D3DXMatrixScaling( &matWorld , i->m_vScale.x , i->m_vScale.y , i->m_vScale.z );

		D3DXMatrixMultiply( &matPos , &matWorld , &matView );

		matPos._41 = i->m_vPos.x + i->m_vLocation.x;
		matPos._42 = i->m_vLocation.y;
		matPos._43 = i->m_vPos.z + i->m_vLocation.z;

		g_RenderManager.SetTexture( 0 , m_pPaperTexture[ i->m_TexType ][ i->m_AniTexNum ] ); 
		
		g_RenderManager.SetTransform( D3DTS_WORLD, &matPos );

		mtrl.Diffuse.r = mtrl.Ambient.r = i->m_Color.r;

		mtrl.Diffuse.g = mtrl.Ambient.g = i->m_Color.g;

		mtrl.Diffuse.b = mtrl.Ambient.b = i->m_Color.b;

		mtrl.Diffuse.a = mtrl.Ambient.a = i->m_Color.a;

		g_RenderManager.SetMaterial( &mtrl );

		g_RenderManager.DrawIndexedPrimitive( D3DPT_TRIANGLESTRIP , 0 , 4 , 0 , 2 );
	}

	g_RenderManager.SetTexture( 0 , NULL );
	
	g_RenderManager.SetCullMode( D3DCULL_CCW );
	
	g_RenderManager.SetZWriteEnable( TRUE );
	
	g_RenderManager.SetAlphaBlendEnable(  FALSE );

	return TRUE;
}


void CWeatherDrop::SetDropType(WeatherDropType Type)
{ 
	if( m_WasBeforeNight )
	{
		return;
	}
	
	///-- 이전에 밤이였다면....
	if(m_isNight)
	{
		if(WEATHER_TYPE_NIGHT == Type && m_DropType == Type)
		{
			return;
		}		
		
		SetDNType(n_WNighttoDay);
	}	
	
	m_DropType = Type; 
	
	switch( m_DropType )
	{
	case WEATHER_TYPE_NONE:	
		
		m_Fade.Start( FADE_STATE_FADE , 
			m_Fade.GetColor() , D3DCOLOR_ARGB( 0 , 0 , 0 , 0 ) , 2000 , TRUE );
		
		break;
		
	case WEATHER_TYPE_RAIN:
		
		m_Fade.Start( FADE_STATE_FADE , 
			m_Fade.GetColor() , D3DCOLOR_ARGB( 30 , 0 , 0 , 55 ) , 2000 , TRUE );
		
		break;
		
	case WEATHER_TYPE_SNOW:
		
		m_Fade.Start( FADE_STATE_FADE , 
			m_Fade.GetColor() , D3DCOLOR_ARGB( 30, 200 , 200 , 255 ) , 2000 , TRUE );
		
		break;
		
	case WEATHER_TYPE_NIGHT:
		{
			SetDNType(n_WDaytoNight);
		}
		break;
	}
	
}

void CWeatherDrop::Reset(void)
{
	m_List.clear();

	m_DropType = WEATHER_TYPE_NONE;

	m_Fade.Start( FADE_STATE_FADE , 
		m_Fade.GetColor() , D3DCOLOR_ARGB( 0 , 0 , 0 , 0 ) , 0 , TRUE );
}

void CWeatherDrop::SetDNType(UINT dType)
{
	m_udType = dType;
	m_lPrevtime = g_nowTime;
	m_Onetime = true;

	if(n_WNone == m_udType)
	{
		Init();
		return;
	}	
}

void CWeatherDrop::UpdateDNType()
{
	if( g_Map.m_NowMapID == 0 && g_RockClient.GetLanguage() == eLang_Korea ) 
	{
		D3DXVECTOR3	vDir = (g_Camera.m_vLookat - g_Camera.m_vEye) * 10.0f;				
		vDir.y = -8.0f;
		D3DXVec3Normalize( &vDir , &vDir );
		m_Character_Light.Direction = vDir;
	}	
	
	if(n_WNone == m_udType)
	{
		return;
	}

	long lDeltime = g_nowTime - m_lPrevtime;
	bool bEnd = false;
	float fRatio = (float)lDeltime / (float)WEATHER_NIGHT_FADE_TIME;

	if( lDeltime > WEATHER_NIGHT_FADE_TIME )
	{
		bEnd = true;
		fRatio = 1.0f;
	}	 
	
	switch(m_udType)
	{
		case n_WDaytoNight:
			{
				
				if( g_Map.m_NowMapID == 0 && g_RockClient.GetLanguage() == eLang_Korea )
				{
					m_Character_Light.Ambient.r = 
					g_Pc_Manager.theCharacter_Light.Ambient.r 
					- (g_Pc_Manager.theCharacter_Light.Ambient.r - 0.65f) * fRatio;
					m_Character_Light.Ambient.g = 	
						g_Pc_Manager.theCharacter_Light.Ambient.g 
						- (g_Pc_Manager.theCharacter_Light.Ambient.g - 0.65f) * fRatio;
					m_Character_Light.Ambient.b = 	
						g_Pc_Manager.theCharacter_Light.Ambient.b 
						- (g_Pc_Manager.theCharacter_Light.Ambient.b - 0.65f) * fRatio;
					m_Character_Light.Diffuse = m_Character_Light.Ambient;
				}
				else
				{
					m_Character_Light.Ambient.r = 
					g_Pc_Manager.theCharacter_Light.Ambient.r 
					- (g_Pc_Manager.theCharacter_Light.Ambient.r - 0.85f) * fRatio;
					m_Character_Light.Ambient.g = 	
						g_Pc_Manager.theCharacter_Light.Ambient.g 
						- (g_Pc_Manager.theCharacter_Light.Ambient.g - 0.85f) * fRatio;
					m_Character_Light.Ambient.b = 	
						g_Pc_Manager.theCharacter_Light.Ambient.b 
						- (g_Pc_Manager.theCharacter_Light.Ambient.b - 0.85f) * fRatio;
					m_Character_Light.Diffuse = m_Character_Light.Ambient;
				}				
				
				m_Object_Light.Ambient.r = 
					g_Pc_Manager.theObject_Light.Ambient.r 
					- (g_Pc_Manager.theObject_Light.Ambient.r - 0.75f) * fRatio;
				m_Object_Light.Ambient.g = 	
					g_Pc_Manager.theObject_Light.Ambient.g 
					- (g_Pc_Manager.theObject_Light.Ambient.g - 0.75f) * fRatio;
				m_Object_Light.Ambient.b = 	
					g_Pc_Manager.theObject_Light.Ambient.b 
					- (g_Pc_Manager.theObject_Light.Ambient.b - 0.75f) * fRatio;
				m_Object_Light.Diffuse = m_Object_Light.Ambient;

				m_Terrain_Light.Ambient.r = 
					g_Pc_Manager.theTerrain_Light.Ambient.r 
					- (g_Pc_Manager.theTerrain_Light.Ambient.r - 0.75f) * fRatio;
				m_Terrain_Light.Ambient.g = 	
					g_Pc_Manager.theTerrain_Light.Ambient.g 
					- (g_Pc_Manager.theTerrain_Light.Ambient.g - 0.75f) * fRatio;
				m_Terrain_Light.Ambient.b = 	
					g_Pc_Manager.theTerrain_Light.Ambient.b 
					- (g_Pc_Manager.theTerrain_Light.Ambient.b - 0.75f) * fRatio;
				m_Terrain_Light.Diffuse = m_Terrain_Light.Ambient;

				m_Sky_Light.Ambient.r = 
					g_Pc_Manager.theSky_Light.Ambient.r 
					- (g_Pc_Manager.theSky_Light.Ambient.r - 0.7f) * fRatio;
				m_Sky_Light.Ambient.g = 	
					g_Pc_Manager.theSky_Light.Ambient.g 
					- (g_Pc_Manager.theSky_Light.Ambient.g - 0.7f) * fRatio;
				m_Sky_Light.Ambient.b = 	
					g_Pc_Manager.theSky_Light.Ambient.b 
					- (g_Pc_Manager.theSky_Light.Ambient.b - 0.7f) * fRatio;
				m_Sky_Light.Diffuse = m_Sky_Light.Ambient;
				
				
				m_Fog_Color.x = g_Pc_Manager.theFog_Color.x
								- ( g_Pc_Manager.theFog_Color.x - 40 ) * fRatio;
				m_Fog_Color.y = g_Pc_Manager.theFog_Color.y
								- ( g_Pc_Manager.theFog_Color.y - 40 ) * fRatio;
				m_Fog_Color.z = g_Pc_Manager.theFog_Color.z
								- ( g_Pc_Manager.theFog_Color.z - 40 ) * fRatio;				
				
				if(bEnd)
				{
					m_Sky_Light.Ambient.r = 0.7f;
					m_Sky_Light.Ambient.g = 0.7f;
					m_Sky_Light.Ambient.b = 0.7f;

					m_Sky_Light.Diffuse = m_Sky_Light.Ambient;

					m_udType = n_WNone;
					m_lPrevtime = g_nowTime;
					g_Map.m_SkyBox.LoadTexture( g_lpDevice , 18 , 0 );
					m_isNight = true;
				}
			}
			break;
		case n_WNighttoDay:
			{
				if(m_Onetime)
				{
					LPENV_DATA SetEnvData = g_Map.m_SetEnv.GetSet();
					g_Map.m_SkyBox.LoadTexture( g_lpDevice , SetEnvData->SkyIndex , 0 );
					m_Onetime = false;
				}
				
				if( g_Map.m_NowMapID == 0 && g_RockClient.GetLanguage() == eLang_Korea )
				{
					m_Character_Light.Ambient.r = 0.65f +
						(g_Pc_Manager.theCharacter_Light.Ambient.r - 0.65f) * fRatio;
					m_Character_Light.Ambient.g = 0.65f +
						(g_Pc_Manager.theCharacter_Light.Ambient.g - 0.65f) * fRatio;
					m_Character_Light.Ambient.b = 0.65f +
						(g_Pc_Manager.theCharacter_Light.Ambient.b - 0.65f) * fRatio;
					m_Character_Light.Diffuse = m_Character_Light.Ambient;
				}
				else
				{
					m_Character_Light.Ambient.r = 0.85f +
						(g_Pc_Manager.theCharacter_Light.Ambient.r - 0.85f) * fRatio;
					m_Character_Light.Ambient.g = 0.85f +
						(g_Pc_Manager.theCharacter_Light.Ambient.g - 0.85f) * fRatio;
					m_Character_Light.Ambient.b = 0.85f +
						(g_Pc_Manager.theCharacter_Light.Ambient.b - 0.85f) * fRatio;
					m_Character_Light.Diffuse = m_Character_Light.Ambient;
				
				}

				m_Object_Light.Ambient.r = 0.75f +
					(g_Pc_Manager.theObject_Light.Ambient.r - 0.75f) * fRatio;
				m_Object_Light.Ambient.g = 0.75f +
					(g_Pc_Manager.theObject_Light.Ambient.g - 0.75f) * fRatio;
				m_Object_Light.Ambient.b = 0.75f +
					(g_Pc_Manager.theObject_Light.Ambient.b - 0.75f) * fRatio;
				m_Object_Light.Diffuse = m_Object_Light.Ambient;

				m_Terrain_Light.Ambient.r = 0.75f +
					(g_Pc_Manager.theTerrain_Light.Ambient.r - 0.75f) * fRatio;
				m_Terrain_Light.Ambient.g = 0.75f +
					(g_Pc_Manager.theTerrain_Light.Ambient.g - 0.75f) * fRatio;
				m_Terrain_Light.Ambient.b = 0.75f +
					(g_Pc_Manager.theTerrain_Light.Ambient.b - 0.75f) * fRatio;
				m_Terrain_Light.Diffuse = m_Terrain_Light.Ambient;

				m_Sky_Light.Ambient.r = 0.7f +
					(g_Pc_Manager.theSky_Light.Ambient.r - 0.7f) * fRatio;
				m_Sky_Light.Ambient.g = 0.7f +
					(g_Pc_Manager.theSky_Light.Ambient.g - 0.7f) * fRatio;
				m_Sky_Light.Ambient.b = 0.7f +
					(g_Pc_Manager.theSky_Light.Ambient.b - 0.7f) * fRatio;
				m_Sky_Light.Diffuse = m_Sky_Light.Ambient;

				m_Fog_Color.x = 40
								+ ( g_Pc_Manager.theFog_Color.x - 40 ) * fRatio;
				m_Fog_Color.y = 40
								+ ( g_Pc_Manager.theFog_Color.y - 40 ) * fRatio;
				m_Fog_Color.z = 40
								+ ( g_Pc_Manager.theFog_Color.z - 40 ) * fRatio;					
				
				if(bEnd)
				{
					m_udType = n_WNone;					
					Init();
					m_isNight = false;
				}
			}
			break;
	}
}
