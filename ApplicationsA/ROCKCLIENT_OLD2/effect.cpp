#include "RockPCH.h"
#include "VFileSystem.h"

#include	"quadlist.h"

#include    "RockClient.h"
#include    "rocklibs\\RockInterface\\Render.h"
#include    "rocklibs\\RockInterface\\Rui.h"

#include	<NETWORK\\Protocol.h>
#include	<Base\\D3DBase.h>
#include	<EFFECT\\Particle.h>
#include	<Map\\Field.h>
#include	"Map.h"
#include	"Effect.h"
#include	"Camera.h"
#include	<EFFECT\\EffectVertex.h>
#include	<OBJ\\Object_Manager.H>
#include	"brother.h"
#include	<assert.h>
#include	"RockClient.h"
#include    "Effect\\CEFRockShield.h"
#include	"profile\profile.h"
#include	"profile\custom_time.h"
#include    "CRenderManager.h"

extern		BOOL				g_ISObjTexLoad;
extern bool g_ISWeatherVisible;

inline float Distance( float x , float z ) { return (float)sqrt((double)(x*x + z*z)); }
inline float Distance3D(float x ,float y, float z ){ return (float)sqrt((double)(x*x + y*y + z*z)); }

//삭제 할꺼 

#include	"Pc.h"

#define		EFFECTOBJ_CHILDCOUNT	30


#ifdef DIRECT_VERSION_9_MJH
extern LPDIRECT3DDEVICE9		g_lpDevice;
#else
extern LPDIRECT3DDEVICE8		g_lpDevice;
#endif // DIRECT_VERSION_9_MJH
extern	long					g_nowTime;

extern CRockClient				g_RockClient;		// 카메라 워크관련 By wxywxy
extern BOOL						g_ISLandEffect;

CEffectSet::~CEffectSet()
{
	
}

void	CEffectSet::LoadEffectSet(char *FileName)
{
	char Path[ 1024 ] = "";
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined( LOAD_VF ) /////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	wsprintf( Path , "%s\\Effect\\%s" , g_RockClient.GetRootDir() , FileName );
	
	FILE		*fp;
	
	fp = fopen( Path, "rb");	
	
	assert( fp != NULL && "!파일 읽기 실패" );
	{
		fread( &m_iMyType, sizeof(m_iMyType), 1, fp );
		fread( &m_iDrawParCount, sizeof(m_iDrawParCount), 1, fp );
		fread( &m_iDrawBillCount, sizeof(m_iDrawBillCount), 1, fp );
		fread( &m_iDrawObjCount, sizeof(m_iDrawObjCount), 1, fp );
		
		for( int i = 0; i < m_iDrawParCount; i++ )
		{
			fread( &m_ParticleInfo[i], sizeof(m_ParticleInfo[i]), 1, fp );
		}
		for( i = 0; i < m_iDrawBillCount; i++ )
		{
			fread( &m_BillboardInfo[i], sizeof(m_BillboardInfo[i]), 1, fp );
		}
		for( i = 0; i < m_iDrawObjCount; i++ )
		{
			fread( &m_ObjInfo[i], sizeof(m_ObjInfo[i]), 1, fp );
		}
	}
	
	fclose( fp );
	
	///////////////////////////////////////////////////////////////////////////////////////////////////	
#else  ////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////	
	
	VFileHandle* pFH = g_VFEffectRoot.OpenFile( FileName );
	//	if( pFH == NULL ) return false;
	assert( pFH != NULL && "!Effect.Set 파일 읽기 실패" );
	{
		pFH->Read( &m_iMyType, sizeof(m_iMyType) );
		pFH->Read( &m_iDrawParCount, sizeof(m_iDrawParCount) );
		pFH->Read( &m_iDrawBillCount, sizeof(m_iDrawBillCount) );
		pFH->Read( &m_iDrawObjCount, sizeof(m_iDrawObjCount) );
		
		for( int i = 0; i < m_iDrawParCount; i++ )
		{
			pFH->Read( &m_ParticleInfo[i], sizeof(m_ParticleInfo[i]) );
		}
		for(int i = 0; i < m_iDrawBillCount; i++ )
		{
			pFH->Read( &m_BillboardInfo[i], sizeof(m_BillboardInfo[i]) );
		}
		for(int i = 0; i < m_iDrawObjCount; i++ )
		{
			pFH->Read( &m_ObjInfo[i], sizeof(m_ObjInfo[i]) );
		}
	}
	g_VFEffectRoot.CloseFile( pFH );
	
	///////////////////////////////////////////////////////////////////////////////////////////////////	
#endif ////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
	
	
}

CEffSetControl::~CEffSetControl()
{
}

void	CEffSetControl::Init()
{
	m_iSetType	    = -1;
	
	m_fDelayTime    = 0.0f;
	
	m_iType         = -1;
	
	m_fLife         = 0.0f;
	
	m_lTargetUniq   = -1;
	m_iTargetIndex  = -1;
	
	m_vPos			= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_vVec			= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	
	m_fRad			= 0.0f;
	
	m_bMake			= false;
	m_iChildMake	= -1;
	
	m_iTailNum		= -1;
	m_iTailType		= -1;
	
	m_fThetaHoriz	= 0.0f;
	m_fThetaVert	= 180.0f;
	
	m_nHeight		= 0;
	
	m_pSrc = NULL;
	m_pAttackUnit = NULL;
	m_IsDirAttatack = FALSE;
	m_IsGUnitLinkBoneAndSizeFix = FALSE;
	m_aDirectLinkBone = 0;
	m_fAddSize = 0.0f;
	
	m_vDir			= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	
	m_ConSkill = false;		//지속성 스킬인지 여부	
}

CBravoParticle::CBravoParticle() : m_bEffectDisplay(true)
{ 
	D3DXMatrixIdentity( &m_matIdentity );
	
	m_ParticleLockMaxCount   = 300;
	m_ParticleLockLimitCount = 10;
	m_pParticleMemVB	     = NULL;
	m_fAddSize				 = 0.0f;	
	
	m_pAttackUnit = NULL;
	m_IsDirAttatack = FALSE;
	m_IsGUnitLinkBoneAndSizeFix = FALSE;
	
	m_aDirectLinkBone = 0;

	m_ParticleSystemPool.Reserve( 256 );
	
	m_IsRenderScreenBlur = FALSE;
}

CBravoParticle::~CBravoParticle()
{
	m_NpcEffectList.clear();
	
	SAFE_DELETE_ARRAY(m_pParticleMemVB);
	
	SAFE_RELEASE(m_pDotVB);
	SAFE_RELEASE(m_pColorVB);
	SAFE_RELEASE(m_pSpaceVB);
	SAFE_RELEASE(m_pSpaceIB);
	SAFE_RELEASE(m_pParticleVB);
	SAFE_RELEASE(m_pParticleIB);
	
	DeleteAll( TRUE );
}

HRESULT		CBravoParticle::Create(char* dir)
{
	//	Read_Ini( dir );
	m_bDisplay = true;
	
	//light 셋업 
#ifdef DIRECT_VERSION_9_MJH
	D3DLIGHT9 light;
#else
	D3DLIGHT8 light;
#endif // DIRECT_VERSION_9_MJH
	D3DUtil_InitLight( light, D3DLIGHT_DIRECTIONAL, 0.f, 200.0f, -500.0f );
	
	light.Diffuse.r  = 1.0f;
	light.Diffuse.g  = 1.0f;
	light.Diffuse.b  = 1.0f;
	light.Ambient.r  = 1.0f;
	light.Ambient.g  = 1.0f;
	light.Ambient.b  = 1.0f;
	
	g_RenderManager.SetLight( 1, &light );
	
	RestoreDeviceObjects();
	
	g_Anim.InitManager( dir );
	
	// 파티클 로딩
	LoadPaticle( dir );
	
	for( int i = 0; i < 128; i++ )
	{
		EffectDelay[i].Init();
		EffectObjDelay[i].Init();
	}
	
	for(int i = 0; i < 512; i++ )
	{
		m_pEffSetCon[i].Init();
	}
	
	//이펙트 셋트 읽기.
	LoadEffectSet();
	
	// 빌보드 읽기.
	g_Billboard.InitManager( dir, FALSE );
	
	//	m_pTails.LoadTex( dir );
	
	m_lPrevTime			= g_nowTime;
	m_iAtFirst			= 0;
	
	m_DamageNum.InitDevice( g_lpDevice );
	
	m_WeatherDrop.InitDevice( g_lpDevice );
	m_WeatherDrop.SetDropType( WEATHER_TYPE_NONE );
	m_WeatherDrop.CreateAll( 250 );
	
	// 각 이펙트 로딩 데이터 
	m_EffectManager.InitDevice();
	
	return	S_OK;
}

void		CBravoParticle::Make_Npc_Effect( int index, long unique )
{
	return;
}

HRESULT		CBravoParticle::RestoreDeviceObjects()
{
	//점 버텍스 버퍼를 쓸때 
	if( FAILED(g_RenderManager.CreateVertexBuffer( sizeof(DOTVERTEX), D3DUSAGE_WRITEONLY, D3DFVF_DOTVERTEX,
		D3DPOOL_MANAGED, &m_pDotVB ) ) )
	{
		return E_FAIL;
	}
	
	DOTVERTEX* pDotVertices;
#ifdef DIRECT_VERSION_9_MJH
	if( FAILED( m_pDotVB->Lock(0, 0, (void**)&pDotVertices, 0) ) )
#else
	if( FAILED( m_pDotVB->Lock(0, 0, (BYTE**)&pDotVertices, 0) ) )
#endif // DIRECT_VERSION_9_MJH
		return E_FAIL;
	
	pDotVertices->v	=	D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	
	m_pDotVB->Unlock();	
	
	//라인 버텍스 버퍼를 쓸때 
	if( FAILED(g_RenderManager.CreateVertexBuffer( 2*sizeof(COLORVERTEX), D3DUSAGE_WRITEONLY, D3DFVF_COLORVERTEX,
		D3DPOOL_MANAGED, &m_pColorVB ) ) )
	{
		return E_FAIL;
	}
	
	COLORVERTEX* pColorVertices;
#ifdef DIRECT_VERSION_9_MJH
	if( FAILED( m_pColorVB->Lock( 0, 0, (void**)&pColorVertices, 0 ) ) )
#else
	if( FAILED( m_pColorVB->Lock( 0, 0, (BYTE**)&pColorVertices, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH
		return E_FAIL;
	
	pColorVertices[0].v	=	D3DXVECTOR3(0.0f, -3.0f, 0.0f);
	pColorVertices[1].v	=	D3DXVECTOR3(0.0f, 3.0f, 0.0f);
	pColorVertices[0].color	=	0x11999999;
	pColorVertices[1].color	=	0x11999999;
	
	m_pColorVB->Unlock();
	
	//스페이스 인덱스 버퍼를 쓸때 
	HRESULT hr = g_RenderManager.CreateIndexBuffer( 4 * sizeof(WORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, 
		D3DPOOL_MANAGED, &m_pSpaceIB);
	
	if( hr != S_OK )
		return hr;
	
	WORD	*pSpaceIndices;
#ifdef DIRECT_VERSION_9_MJH
	if ( FAILED ( m_pSpaceIB->Lock( 0 , 4 * sizeof(WORD) , (void**)&pSpaceIndices, 0 ) ) )
#else
	if ( FAILED ( m_pSpaceIB->Lock( 0 , 4 * sizeof(WORD) , (BYTE**)&pSpaceIndices, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH	
		return	E_FAIL;
	
	pSpaceIndices[0] = 0;
	pSpaceIndices[1] = 1;
	pSpaceIndices[2] = 2;
	pSpaceIndices[3] = 3;
	
	m_pSpaceIB->Unlock();
	
	//스페이스 버텍스 버퍼를 쓸때 
	if( FAILED(g_RenderManager.CreateVertexBuffer( 4*sizeof(SPACEVERTEX), D3DUSAGE_WRITEONLY, D3DFVF_SPACEVERTEX,
		D3DPOOL_MANAGED, &m_pSpaceVB ) ) )
	{
		return E_FAIL;
	}
	
	SPACEVERTEX* pSpaceVertices;
#ifdef DIRECT_VERSION_9_MJH
	if( FAILED( m_pSpaceVB->Lock( 0, 0, (void**)&pSpaceVertices, 0 ) ) )
#else
	if( FAILED( m_pSpaceVB->Lock( 0, 0, (BYTE**)&pSpaceVertices, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH
		return E_FAIL;
	
	pSpaceVertices[0].v	=	D3DXVECTOR3( -0.5f , -0.5f , 0.0f );
	pSpaceVertices[1].v	=	D3DXVECTOR3( -0.5f ,  0.5f , 0.0f );
	pSpaceVertices[2].v	=	D3DXVECTOR3(  0.5f , -0.5f , 0.0f );
	pSpaceVertices[3].v	=	D3DXVECTOR3(  0.5f ,  0.5f , 0.0f );
	
	pSpaceVertices[0].tu	=	0.0f;
	pSpaceVertices[0].tv	=	1.0f;
	
	pSpaceVertices[1].tu	=	0.0f;
	pSpaceVertices[1].tv	=	0.0f;
	
	pSpaceVertices[2].tu	=	1.0f;
	pSpaceVertices[2].tv	=	1.0f;
	
	pSpaceVertices[3].tu	=	1.0f;
	pSpaceVertices[3].tv	=	0.0f;
	
	m_pSpaceVB->Unlock();
	
	//.......................................................................................................
	// 파티클 일괄적으로 찍어줄때
	//.......................................................................................................
	// 버텍스 버퍼 만들기
	//.......................................................................................................
	m_pParticleMemVB = SAFE_NEW_ARRAY( VERTEX_XYZ_DIFFUSE_TEX1 , m_ParticleLockMaxCount * 4 );	
	if( m_pParticleMemVB == NULL )
		return E_FAIL;

	if( FAILED( g_RenderManager.CreateVertexBuffer( 4 * sizeof(VERTEX_XYZ_DIFFUSE_TEX1) * m_ParticleLockMaxCount , D3DUSAGE_WRITEONLY
		, D3DFVF_VERTEX_XYZ_DIFFUSE_TEX1 , D3DPOOL_MANAGED , &m_pParticleVB ) ) )
	{
		return E_FAIL;
	}
	
	for( int i = 0 ; i < 4 * m_ParticleLockMaxCount; i += 4 )
	{
		m_pParticleMemVB[ i     ].tu =	0.0f;
		m_pParticleMemVB[ i     ].tv =	1.0f;
		
		m_pParticleMemVB[ i + 1 ].tu =	0.0f;
		m_pParticleMemVB[ i + 1 ].tv =	0.0f;
		
		m_pParticleMemVB[ i + 2 ].tu =	1.0f;
		m_pParticleMemVB[ i + 2 ].tv =	1.0f;
		
		m_pParticleMemVB[ i + 3 ].tu =	1.0f;
		m_pParticleMemVB[ i + 3 ].tv =	0.0f;
	}
	
	//.......................................................................................................
	// 인덱스 버퍼만들기
	//.......................................................................................................
	hr = g_RenderManager.CreateIndexBuffer( 6 * sizeof(WORD) * m_ParticleLockMaxCount , D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, 
		D3DPOOL_MANAGED , &m_pParticleIB );
	
	if( hr != S_OK )
		return hr;
#ifdef DIRECT_VERSION_9_MJH
	if ( FAILED ( m_pParticleIB->Lock( 0 , 0 , (void**)&pSpaceIndices, 0 ) ) )
#else
	if ( FAILED ( m_pParticleIB->Lock( 0 , 0 , (BYTE**)&pSpaceIndices, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH	
		return	E_FAIL;
	
	int Count = 0;
	
	for(int i = 0 ; i < 6 * m_ParticleLockMaxCount ; i += 6 )
	{
		pSpaceIndices[ i + 0 ] = ( WORD ) Count;
		pSpaceIndices[ i + 1 ] = ( WORD ) Count + 1;
		pSpaceIndices[ i + 2 ] = ( WORD ) Count + 2;
		pSpaceIndices[ i + 3 ] = ( WORD ) Count + 2;
		pSpaceIndices[ i + 4 ] = ( WORD ) Count + 1;
		pSpaceIndices[ i + 5 ] = ( WORD ) Count + 3;
		
		Count += 4;
	}
	
	m_pParticleIB->Unlock();
	
	//.......................................................................................................
	
	return	S_OK;
}



//
HRESULT	CBravoParticle::Display()
{
	g_RenderManager.SetFogEnable( FALSE );
	
	//static D3DXMATRIX	matWorldYaw;
	//static D3DXMATRIX	matWorldPitch;

	m_matInverseCamera = g_Camera.m_matInvView;
	m_matInverseCamera._41 = 0.0f;
	m_matInverseCamera._42 = 0.0f;
	m_matInverseCamera._43 = 0.0f;
	
	m_EffectTextureManager.DeleteNotUsedTex();
	
	// 이펙트 메니저 처리 루틴 by wxywxy
	m_EffectManager.Update( m_fTimeD );
	
	EffSetConProcess( m_fTimeD );
	
	//이펙트 딜레이 루틴.
	EffectDelayProcess( m_fTimeD );
	
	EffectObjDelayProcess( m_fTimeD );
	
	EffectStopProcess( m_fTimeD );

	// 비
	if( g_RockClient.m_SceneManager.GetWeatherDisplay() )
		RainControl( m_fTimeD );
	
	
	g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureColorArg1( 0 , D3DTA_TEXTURE );
	g_RenderManager.SetTextureColorArg2( 0 , D3DTA_CURRENT );	
	
	g_RenderManager.SetTextureAlphaOP  ( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureAlphaArg1( 0 , D3DTA_TEXTURE );
	g_RenderManager.SetTextureAlphaArg2( 0 , D3DTA_CURRENT );
	
	g_RenderManager.DisableTextureStageState( 1 );
	g_RenderManager.DisableTextureStageState( 2 );
	
	g_RenderManager.SetAlphaTestEnable( TRUE );
	g_RenderManager.SetAlphaRef( 0 );
	g_RenderManager.SetAlphaFunc( D3DCMP_NOTEQUAL );
	
	g_RenderManager.SetAlphaBlendEnable( TRUE );
	g_RenderManager.SetSrcBlend( D3DBLEND_ONE );
	g_RenderManager.SetDestBlend( D3DBLEND_ONE );
	
	g_RenderManager.LightEnable( 1, TRUE );
	g_RenderManager.SetLighting( FALSE );
	
	g_RenderManager.SetZWriteEnable( FALSE );
	g_RenderManager.SetCullMode( D3DCULL_NONE );
	
	g_RenderManager.SetZEnable( TRUE );
	g_RenderManager.SetZFunc( D3DCMP_LESSEQUAL );	
	
	g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
	g_RenderManager.SetDestBlend( D3DBLEND_INVSRCALPHA );
	
	g_RenderManager.SetCullMode( D3DCULL_CCW );
	g_RenderManager.SetLighting( TRUE );
	
	// 인섹트
	if( g_Map.m_byMapType != MS_INDOOR &&
		( m_WeatherDrop.GetDropType() == WEATHER_TYPE_NONE || !g_ISWeatherVisible ) && g_ISLandEffect && g_RockClient.m_SceneManager.GetInsectDisplay() )
	{	
		LPENV_DATA SetEnvData = g_Map.m_SetEnv.GetSet();	
		
		if( SetEnvData )
		{
			g_RenderManager.SetZWriteEnable( FALSE );
			g_RenderManager.SetCullMode( D3DCULL_NONE );
			
			if( g_ISObjTexLoad )
			{
				// 나비
				if( SetEnvData->IsInsect0 )
					g_Insect.Render_Insect(1, m_fTimeD);
				
				// 새
				if( SetEnvData->IsInsect1 )
					g_Insect.Render_Insect(2, m_fTimeD);
			}
			
			g_RenderManager.SetZWriteEnable( TRUE );
			g_RenderManager.SetCullMode( D3DCULL_CCW );
		}
	}
	
	g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
	g_RenderManager.SetDestBlend( D3DBLEND_DESTALPHA );

	// 이펙트오브젝트 렌더링	
	DisplayEffectObj( m_fTimeD );	

	g_RenderManager.SetZWriteEnable( FALSE );
	g_RenderManager.SetZEnable( TRUE );
	g_RenderManager.SetZFunc( D3DCMP_LESSEQUAL );
	
	UpdateManager( m_fTimeD );
	
	g_RenderManager.SetCullMode( D3DCULL_NONE );
	g_RenderManager.SetZWriteEnable( FALSE );
	
	g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureColorArg1( 0 , D3DTA_TEXTURE );
	g_RenderManager.SetTextureColorArg2( 0 , D3DTA_CURRENT );
	
	
	g_RenderManager.SetTextureAlphaOP  ( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureAlphaArg1( 0 , D3DTA_TEXTURE );
	g_RenderManager.SetTextureAlphaArg2( 0 , D3DTA_CURRENT );

#ifdef BATCH_RENDER	
	g_RenderManager.SetLighting( FALSE );
#else	//	BATCH_RENDER
	g_RenderManager.SetLighting( TRUE );
#endif	//	BATCH_RENDER

	g_Billboard.DisplayManager( m_fTimeD );

	g_RenderManager.SetLighting( FALSE );
	m_EffectManager.Render( m_fTimeD );// 모든 이펙트 그림
	g_RenderManager.SetLighting( TRUE );

	if(!m_bDisplay)
		return S_OK;
	
	
	g_RenderManager.SetCullMode( D3DCULL_CCW );
	g_RenderManager.LightEnable( 1, FALSE );
	
	g_RenderManager.SetZWriteEnable( TRUE );
	g_RenderManager.SetZEnable( TRUE );
	
	g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
	g_RenderManager.SetDestBlend( D3DBLEND_INVSRCALPHA );
	
	g_RenderManager.SetAlphaTestEnable( TRUE );
	g_RenderManager.SetAlphaRef( 0 );
	g_RenderManager.SetZFunc( D3DCMP_LESSEQUAL );	
	
	g_RenderManager.LightEnable( 0 , TRUE );
	g_RenderManager.LightEnable( 1, TRUE );
	
	g_RenderManager.SetLighting( TRUE );	
	
	m_DamageNum.Update( m_fTimeD );
	
	m_ExpDisplay.Update( g_lpDevice );
	
	
	m_DamageNum.Render( g_lpDevice , g_Camera.m_matInvView );
	
	m_ExpDisplay.Render( g_lpDevice , g_Camera.m_matInvView );
	
	g_RenderManager.LightEnable( 1, FALSE );
	
	g_RenderManager.SetLighting( FALSE );
	
	RockCheck();
	
	//.......................................................................................................
	// 시퀀스 체크 By wxywxy
	//.......................................................................................................
	g_RenderManager.SetZWriteEnable( TRUE );
	g_RenderManager.SetZEnable( TRUE );
	g_RenderManager.SetLighting( FALSE );
	g_RenderManager.SetAlphaFunc( D3DCMP_ALWAYS );
	g_RenderManager.SetAlphaBlendEnable( FALSE );
	
	g_RockClient.m_SquenceManager.Update();
	g_RenderManager.SetZFunc( D3DCMP_LESSEQUAL );
	
	//.......................................................................................................
	
	//by simwoosung
	g_RockClient.SetCharacterInfo();	

	if( g_RockClient.m_SceneManager.GetGUIDisplay() )
		nRui->BillUIRenderProc();
	
	Glow();
	
	if(m_IsRenderScreenBlur)
		ScreenBlur();
	
    m_Combo2DEffect.Update();
	m_Combo2DEffect.Draw( g_lpDevice );
	
	g_Particle.m_SpotEffect.Update( g_lpDevice );
	
	g_RenderManager.SetZWriteEnable( TRUE );

	///--JAPAN_BUG_MODIFY
	DWORD dFogEnable = g_RenderManager.GetFogEnable();
	g_RenderManager.SetFogEnable( dFogEnable );
	
	return	S_OK;
}

void CBravoParticle::Glow(void)
{
	if( m_GlowAlpha == 0 ) return;
	
	g_RenderManager.SetLighting( FALSE );
	g_RenderManager.SetAlphaTestEnable( FALSE );
	g_RenderManager.SetFogEnable( FALSE );
	
	g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureColorArg1( 0 , D3DTA_TEXTURE );
	g_RenderManager.SetTextureColorArg2( 0 , D3DTA_DIFFUSE );
	
	
	g_RenderManager.SetTextureAlphaOP  ( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureAlphaArg1( 0 , D3DTA_DIFFUSE );
	g_RenderManager.SetTextureAlphaArg2( 0 , D3DTA_TEXTURE );
	
	g_RenderManager.DisableTextureStageState( 1 );
	g_RenderManager.DisableTextureStageState( 2 );	
	
	g_RenderManager.SetTextureMipFilter ( 0 , D3DTEXF_LINEAR );
	g_RenderManager.SetTextureMagFilter ( 0 , D3DTEXF_LINEAR );
	g_RenderManager.SetTextureMinFilter ( 0 , D3DTEXF_LINEAR );
	
	g_RenderManager.SetAlphaBlendEnable( TRUE );
	g_RenderManager.SetSrcBlend( D3DBLEND_ONE );
	g_RenderManager.SetDestBlend(  D3DBLEND_ZERO );
	
	bool bIsBeginScene = g_RockClient.m_bIsBeginScene;	
	if( bIsBeginScene )
	{
		g_RockClient.EndScene();
	}
	
	if(m_GlowAlpha)
	{
		m_Glow.Process( g_lpDevice , NULL );
	}
	
	g_RenderManager.SetAlphaBlendEnable( FALSE );	
	
	if( bIsBeginScene )
	{
		g_RockClient.BeginScene();
	}
	
	if(m_GlowAlpha)
	{
		m_Glow.Render( g_lpDevice );
	}
}

void CBravoParticle::ScreenBlur()
{
	g_RenderManager.SetLighting( FALSE );
	g_RenderManager.SetAlphaTestEnable( FALSE );
	g_RenderManager.SetFogEnable( FALSE );
	
	g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureColorArg1( 0 , D3DTA_TEXTURE );
	g_RenderManager.SetTextureColorArg2( 0 , D3DTA_DIFFUSE );
	
	
	g_RenderManager.SetTextureAlphaOP  ( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureAlphaArg1( 0 , D3DTA_DIFFUSE );
	g_RenderManager.SetTextureAlphaArg2( 0 , D3DTA_TEXTURE );
	
	g_RenderManager.DisableTextureStageState( 1 );
	g_RenderManager.DisableTextureStageState( 2 );	
	
	g_RenderManager.SetTextureMipFilter ( 0 , D3DTEXF_LINEAR );
	g_RenderManager.SetTextureMagFilter ( 0 , D3DTEXF_LINEAR );
	g_RenderManager.SetTextureMinFilter ( 0 , D3DTEXF_LINEAR );
	
	g_RenderManager.SetAlphaBlendEnable( TRUE );
	g_RenderManager.SetSrcBlend( D3DBLEND_ONE );
	g_RenderManager.SetDestBlend(  D3DBLEND_ZERO );
	
	bool bIsBeginScene = g_RockClient.m_bIsBeginScene;	
	if( bIsBeginScene )
	{
		g_RockClient.EndScene();
	}
	
	m_ScreenBlur.Process( g_lpDevice , NULL );	
	g_RenderManager.SetAlphaBlendEnable( FALSE );		
	g_RockClient.BeginScene();

	m_ScreenBlur.Render( g_lpDevice );	
}

void	CBravoParticle::UpdateManager( FLOAT fTimeDelta )
{
	D3DXMATRIX  mat;
	D3DXVECTOR3 vPos;
	
	// 파티클 카운트
	DWORD		ParticleCount = 0;	
	
	//	CParticleSystem* prNext = NULL;
	//	if( pParticleSystem->m_pSystemNext != NULL )
	//	prNext = pParticleSystem->m_pSystemNext;
	
	std::list <CParticleSystem *>::iterator i = m_ParticleSystemList.begin();
	
	CParticleSystem	*pParticleSystem = NULL;
	
	for( ; i != m_ParticleSystemList.end() ; )
	{
		pParticleSystem = (*i);
		
		//...........................................................................................................
		// 파티클 시스템 삭제
		//...........................................................................................................
		if ( pParticleSystem->m_bIsSuppressed == true && pParticleSystem->m_uAliveParticle == 0 )
		{
			pParticleSystem->Destroy();		
		
			m_ParticleSystemPool.DeleteMember( pParticleSystem );
			
			(*i) = NULL;
			
			m_ParticleSystemList.erase( i++ );
			
			continue;
		}
		//...........................................................................................................
		// 파티클 시스템 처리
		//...........................................................................................................
		else
		{			
			//.............................................................................................
			// 캐릭터 소스로 찾는 방식
			//.............................................................................................
			if( !pParticleSystem->m_IsDirAttatack && 
				( ( pParticleSystem->m_iLink < 10 && pParticleSystem->m_iLink > 0 ) || 
				    pParticleSystem->m_aDirectLinkBone )  &&
				    m_pEffSetCon[ pParticleSystem->m_iEffSetCon ].m_iType != ESLT_SRC_RAD
				)
			{
				if( pParticleSystem->m_pSrc != NULL )
				{
					if( pParticleSystem->m_pSrc->m_nTableIndex != -1 )
					{
						BOOL bFlag = TRUE;
						
						if( pParticleSystem->m_aDirectLinkBone )
						{
							bFlag = pParticleSystem->m_pSrc->GetDirectBoneLinkTM( pParticleSystem->m_aDirectLinkBone , mat );
						}
						else
						{
							bFlag = pParticleSystem->m_pSrc->GetBoneLinkTM( pParticleSystem->m_iLink , mat, FALSE );
						}
						
						if( bFlag )
						{
							vPos.x = mat._41;
							vPos.y = mat._42;
							vPos.z = mat._43;
							
							pParticleSystem->SetLinkPos( vPos );
							
							if( !pParticleSystem->UpdateLink( fTimeDelta ) )
							{
								pParticleSystem->m_bIsSuppressed = TRUE;
								
								pParticleSystem->Destroy();								
		
								m_ParticleSystemPool.DeleteMember( pParticleSystem );
	
								
								(*i) = NULL;
								
								m_ParticleSystemList.erase( i++ );
								
								continue;
							}
							else 
							{
								if( pParticleSystem->m_pSrc->m_bIsVisible &&
									pParticleSystem->m_pSrc->m_CharAlpha.GetState() != CHAR_NONE_DISPLAY &&
									pParticleSystem->m_pSrc->m_IsAllLTexture 
									)
								{
										
									BOOL IsParticleRender = TRUE;
									
									if( pParticleSystem->m_pSrc == g_Pc.GetPlayer() && !(g_Pc.GetPlayer()->IsMyPlayerRender) )
									{
										IsParticleRender = FALSE;		
									}									
									
									if( IsParticleRender )
									{
										pParticleSystem->Render();
									}
								}
							}
							
						}
						else
						{
							pParticleSystem->m_bIsSuppressed = TRUE;
							
							pParticleSystem->Destroy();							
			
							m_ParticleSystemPool.DeleteMember( pParticleSystem );

							
							(*i) = NULL;
							
							m_ParticleSystemList.erase( i++ );
							
							continue;
						}
					}
					else
					{
						pParticleSystem->m_bIsSuppressed = TRUE;
						
						pParticleSystem->Destroy();						
		
						m_ParticleSystemPool.DeleteMember( pParticleSystem );

						
						(*i) = NULL;
						
						m_ParticleSystemList.erase( i++ );
						
						continue;
					}
				}
				else
				{
					pParticleSystem->m_bIsSuppressed = TRUE;
					
					pParticleSystem->Destroy();
					
					m_ParticleSystemPool.DeleteMember( pParticleSystem );
					
					(*i) = NULL;
					
					m_ParticleSystemList.erase( i++ );
					
					continue;
				}
			}
			else
			{
				if( pParticleSystem->m_pSrc != NULL )
				{
					if( pParticleSystem->m_pSrc->m_nTableIndex != -1 )
					{
						///-- 일단 툴쪽에서 손을 봐야 할듯 하지만...
						///-- 파티클이 잔상으로 남는게 아니라... 따라가게 만드는 경우
						///-- 임시적으로 예외사항을 둔다...;;
						///-- By simwoosung
						if( m_pEffSetCon[ pParticleSystem->m_iEffSetCon ].m_iSetType == EF_BEFORE_ATTACKER_MARK )
						{
							vPos.x = pParticleSystem->m_pSrc->GetPosTM()._41;
							vPos.y = pParticleSystem->m_pSrc->GetPosTM()._42;
							vPos.z = pParticleSystem->m_pSrc->GetPosTM()._43;							
						
							pParticleSystem->m_bLinkOk = TRUE;
						}
					}
				}
				
				BOOL bResult = TRUE;
		
				bResult = !pParticleSystem->Update( fTimeDelta );					
				
				if( bResult )
				{
					pParticleSystem->m_bIsSuppressed = TRUE;
					
					pParticleSystem->Destroy();					
		
					m_ParticleSystemPool.DeleteMember( pParticleSystem );
						
					(*i) = NULL;
					
					m_ParticleSystemList.erase( i++ );
					
					continue;
				}
				else
				{
					if( pParticleSystem->m_lUnique != -1 )
					{
						if( pParticleSystem->m_pSrc != NULL )
						{
							if( pParticleSystem->m_pSrc->m_nTableIndex != -1 )
							{
								if( pParticleSystem->m_pSrc->m_bIsVisible &&
									pParticleSystem->m_pSrc->m_CharAlpha.GetState() != CHAR_NONE_DISPLAY &&
									pParticleSystem->m_pSrc->m_IsAllLTexture )
								{
										
									BOOL IsParticleRender = TRUE;
									
									if( pParticleSystem->m_pSrc == g_Pc.GetPlayer() && !(g_Pc.GetPlayer()->IsMyPlayerRender) )
									{
										IsParticleRender = FALSE;		
									}									
									
									if( IsParticleRender )
									{
										pParticleSystem->Render();
									}
								}
							}
							else
							{
								pParticleSystem->m_bIsSuppressed = TRUE;
								
								pParticleSystem->Destroy();								
		
								m_ParticleSystemPool.DeleteMember( pParticleSystem );
									
								(*i) = NULL;
								
								m_ParticleSystemList.erase( i++ );
								
								continue;
							}
						}
						else
						{
							pParticleSystem->m_bIsSuppressed = TRUE;
							
							pParticleSystem->Destroy();						
		
							m_ParticleSystemPool.DeleteMember( pParticleSystem );
								
							(*i) = NULL;
							
							m_ParticleSystemList.erase( i++ );
							
							continue;
						}
					}
					else
					{
						pParticleSystem->Render();
					}
				}
				
			}
			//.............................................................................................
			// 파티클 카운트
			ParticleCount += pParticleSystem->m_uAliveParticle;
			
			i++;
		}
		
	}
}

void    CBravoParticle::UpdateManager( CParticleSystem* pParticleSystem, FLOAT fTimeDelta )
{
	if(pParticleSystem == NULL)
	{
		return;
	}
	
	if( pParticleSystem->Update( fTimeDelta ) )
	{
		pParticleSystem->Render();
	}
}

void	CBravoParticle::AddParticleSystem(CParticleSystem* pParticleSystem)
{
	
}

void		CBravoParticle::DisplayUpdate( int number , long Uniq )
{
	CParticleData	*pData = NULL;
	
	if( ( pData = m_ParticleData.GetKeyDataPointer( number ) ) == NULL )
		return;	

	CParticleSystem* m_pAddParticleSystem = m_ParticleSystemPool.NewMember();	
	
	if( NULL == m_pAddParticleSystem )
		return;	
	
	m_pAddParticleSystem->m_pReference	    = pData;
	m_pAddParticleSystem->m_bIsSuppressed	= false;
	m_pAddParticleSystem->m_bIsCreate		= true;
	m_pAddParticleSystem->m_lLockonUniq		= Uniq;
	m_pAddParticleSystem->m_lUnique			= -1;
	m_pAddParticleSystem->m_iIndex			= -1;
	m_pAddParticleSystem->m_fSize			= 1.0f;
	
	m_ParticleSystemList.push_back( m_pAddParticleSystem );
}


void		CBravoParticle::DisplayUpdateNotApplyDis( int number, D3DXVECTOR3& v )
{
	CParticleData	*pData = NULL;
	
	if( ( pData = m_ParticleData.GetKeyDataPointer( number ) ) == NULL )
		return;	

	CParticleSystem* m_pAddParticleSystem = m_ParticleSystemPool.NewMember();
	
	if( NULL == m_pAddParticleSystem )
		return;
	
	m_pAddParticleSystem->m_IsParticleApplyDis = FALSE;
	m_pAddParticleSystem->m_pReference      = pData;
	m_pAddParticleSystem->m_vMoveLocation	= v;
	m_pAddParticleSystem->m_bIsSuppressed	= false;
	m_pAddParticleSystem->m_bIsCreate		= true;
	m_pAddParticleSystem->m_lUnique			= -1;
	m_pAddParticleSystem->m_iIndex			= -1;
	m_pAddParticleSystem->m_fSize			= 1.0f;
	m_pAddParticleSystem->m_iEffSetCon		= -1;
	m_pAddParticleSystem->m_fSize			= 1.0f;
	
	m_ParticleSystemList.push_back( m_pAddParticleSystem );
}

void		CBravoParticle::DisplayUpdate( int number, D3DXVECTOR3& v, int effsetcon, int fSize )
{
	CParticleData	*pData = NULL;
	
	if( ( pData = m_ParticleData.GetKeyDataPointer( number ) ) == NULL )
		return;	

	CParticleSystem* m_pAddParticleSystem = m_ParticleSystemPool.NewMember();
	
	if( NULL == m_pAddParticleSystem )
		return;
	
	m_pAddParticleSystem->m_pReference      = pData;
	m_pAddParticleSystem->m_vMoveLocation	= v;
	m_pAddParticleSystem->m_bIsSuppressed	= false;
	m_pAddParticleSystem->m_bIsCreate		= true;
	m_pAddParticleSystem->m_lUnique			= -1;
	m_pAddParticleSystem->m_iIndex			= -1;
	m_pAddParticleSystem->m_fSize			= 1.0f;
	m_pAddParticleSystem->m_iEffSetCon		= effsetcon;
	m_pAddParticleSystem->m_fSize = fSize;
	
	m_ParticleSystemList.push_back( m_pAddParticleSystem );
}


void		CBravoParticle::DisplayUpdate( int number, long Uniq, D3DXVECTOR3& v)
{
	CParticleData	*pData = NULL;
	
	if( ( pData = m_ParticleData.GetKeyDataPointer( number ) ) == NULL )
		return;	

	CParticleSystem* m_pAddParticleSystem = m_ParticleSystemPool.NewMember();	
	
	if( NULL == m_pAddParticleSystem )
		return;
	
	m_pAddParticleSystem->m_pReference      = pData;
	m_pAddParticleSystem->m_vMoveLocation	= v;
	m_pAddParticleSystem->m_bIsSuppressed	= false;
	m_pAddParticleSystem->m_bIsCreate		= true;
	m_pAddParticleSystem->m_lUnique			= -1;
	m_pAddParticleSystem->m_iIndex			= -1;
	m_pAddParticleSystem->m_fSize			= 1.0f;
	m_pAddParticleSystem->m_lLockonUniq		= Uniq;
	
	m_ParticleSystemList.push_back( m_pAddParticleSystem );
}


void		CBravoParticle::DisplayUpdate( int number, long Uniq, int LinkLoc )
{
	CParticleData	*pData = NULL;
	
	if( ( pData = m_ParticleData.GetKeyDataPointer( number ) ) == NULL )
		return;	

	CParticleSystem* m_pAddParticleSystem = m_ParticleSystemPool.NewMember();	

	if( NULL == m_pAddParticleSystem )
		return;
	
	m_pAddParticleSystem->m_pReference	    = pData;
	m_pAddParticleSystem->m_bIsSuppressed	= false;
	m_pAddParticleSystem->m_bIsCreate		= true;
	
	D3DXMATRIX	mat = g_Pc.GetHandTM( LinkLoc, Uniq );
	D3DXVECTOR3 v = D3DXVECTOR3( mat._41, mat._42, mat._43 );
	m_pAddParticleSystem->m_vMoveLocation	= v;
	
	m_ParticleSystemList.push_back( m_pAddParticleSystem );
}

Character  *CBravoParticle::GetCharacter(long Unique,int nIndex)
{
	if( Unique != -1 )
	{
		if( nIndex == -1 )
		{
			if( g_Pc.m_MainPC.lUnique == Unique )
			{
				return ( Character * ) g_Pc.GetPlayer();
			}
			else
			{
				return ( Character * )g_Pc.GetUser( Unique );
			}
		}
		else
		{
			int Index;
			
			if( ( Index = g_Map.FindNpc( Unique ) ) == MAX_NPC_INTABLE )
			{
				return NULL;
			}
			
			return ( Character * ) &g_Map.m_Npc[ Index ];
		}
	}
	
	return NULL;
}

void		CBravoParticle::DisplayUpdate( int number, long Uniq, int index, int LinkLoc, D3DXVECTOR3& v, bool bFlag, 
								   int effsetcon, D3DXVECTOR3 vAttackSrcDir, Character * pAttackUnit,
								   BOOL	IsDirAttatack, BOOL IsGUnitLinkBoneAndSizeFix, BYTE aDirectLinkBone )
{
	CParticleData	*pData = NULL;
	Character		*pSrc  = NULL;
	
	if( Uniq != -1 )
	{
		pSrc = GetCharacter( Uniq , index );
		
		if( pSrc == NULL )
			return;
	}	
	
	if( ( pData = m_ParticleData.GetKeyDataPointer( number ) ) == NULL )
		return;	

	CParticleSystem* m_pAddParticleSystem = m_ParticleSystemPool.NewMember();	
	
	if( NULL == m_pAddParticleSystem )
		return;
	
	m_pAddParticleSystem->m_pReference = pData;	
	
	m_pAddParticleSystem->m_vAddLocation    = v;
	m_pAddParticleSystem->m_lUnique			= Uniq;
	m_pAddParticleSystem->m_iIndex			= index;
	m_pAddParticleSystem->m_bIsSuppressed	= false;
	m_pAddParticleSystem->m_bIsCreate		= true;
	m_pAddParticleSystem->m_iLink			= LinkLoc;
	m_pAddParticleSystem->m_fSize			= 1.0f;
	m_pAddParticleSystem->m_iEffSetCon		= effsetcon;
	m_pAddParticleSystem->m_pSrc			= pSrc;
	m_pAddParticleSystem->m_vAttackSrcDir	= vAttackSrcDir;
	m_pAddParticleSystem->m_pAttackUnit		= pAttackUnit;
	m_pAddParticleSystem->m_IsDirAttatack	= IsDirAttatack;
	m_pAddParticleSystem->m_IsGUnitLinkBoneAndSizeFix = IsGUnitLinkBoneAndSizeFix;
	m_pAddParticleSystem->m_aDirectLinkBone = aDirectLinkBone;
	
	
	m_ParticleSystemList.push_back( m_pAddParticleSystem );
}

void		CBravoParticle::DisplayUpdate( int number, long Uniq, int index, int linkLoc, D3DXVECTOR3 v, int effsetcon,
								   D3DXVECTOR3 vAttackSrcDir, Character * pAttackUnit,
								   BOOL	IsDirAttatack, BOOL IsGUnitLinkBoneAndSizeFix, BYTE aDirectLinkBone )
{
	CParticleData	*pData = NULL;
	Character		*pSrc  = NULL;
	
	if( Uniq != -1 )
	{
		pSrc = GetCharacter( Uniq , index );
		
		if( pSrc == NULL )
			return;
	}	
	
	if( ( pData = m_ParticleData.GetKeyDataPointer( number ) ) == NULL )
		return;	

	CParticleSystem* m_pAddParticleSystem = m_ParticleSystemPool.NewMember();	
	
	if( NULL == m_pAddParticleSystem )
		return;
	
	m_pAddParticleSystem->m_pReference = pData;	
	
	m_pAddParticleSystem->m_lUnique			= Uniq;
	m_pAddParticleSystem->m_iIndex			= index;
	m_pAddParticleSystem->m_bIsSuppressed	= false;
	m_pAddParticleSystem->m_bIsCreate		= true;
	m_pAddParticleSystem->m_iLink			= linkLoc;
	m_pAddParticleSystem->m_fSize			= 1.0f;
	m_pAddParticleSystem->m_iEffSetCon		= effsetcon;
	m_pAddParticleSystem->m_pSrc			= pSrc;
	m_pAddParticleSystem->m_vAttackSrcDir	= vAttackSrcDir;
	m_pAddParticleSystem->m_pAttackUnit		= pAttackUnit;
	m_pAddParticleSystem->m_IsDirAttatack	= IsDirAttatack;
	m_pAddParticleSystem->m_IsGUnitLinkBoneAndSizeFix = IsGUnitLinkBoneAndSizeFix;
	m_pAddParticleSystem->m_aDirectLinkBone = aDirectLinkBone;
	
	// 이펙트 셋에 걸려있는 각도값
	m_pAddParticleSystem->m_fSetRadian		= m_pEffSetCon[effsetcon].m_fRad;	
	m_pAddParticleSystem->m_vMoveLocation = v; 
	
	if( ( linkLoc < 10 && linkLoc > 0 ) || aDirectLinkBone )
	{
		D3DXMATRIX matLink;		
		
		if( !(m_pAddParticleSystem->m_IsDirAttatack) )
		{
			if( aDirectLinkBone )
			{
				pSrc->GetDirectBoneLinkTM( m_pAddParticleSystem->m_aDirectLinkBone , matLink );
			}
			else
			{
				pSrc->GetBoneLinkTM( m_pAddParticleSystem->m_iLink , matLink, FALSE );
			}	
			
			m_pAddParticleSystem->m_vMoveLocation.x += matLink._41;
			m_pAddParticleSystem->m_vMoveLocation.y += matLink._42;
			m_pAddParticleSystem->m_vMoveLocation.z += matLink._43;
		}
		else
		{
			m_pAddParticleSystem->m_vMoveLocation.x += pSrc->GetBonCenterTM()._41;
			m_pAddParticleSystem->m_vMoveLocation.y += pSrc->GetBonCenterTM()._42;
			m_pAddParticleSystem->m_vMoveLocation.z += pSrc->GetBonCenterTM()._43;
		}
	}
	else
	{
		m_pAddParticleSystem->m_vMoveLocation.x += pSrc->GetBonCenterTM()._41;
		m_pAddParticleSystem->m_vMoveLocation.y += pSrc->GetBonCenterTM()._42;
		m_pAddParticleSystem->m_vMoveLocation.z += pSrc->GetBonCenterTM()._43;
	}	
	
	if( g_Map.IsGiantUnit( pSrc ) )
	{
		if( m_pAddParticleSystem->m_IsDirAttatack ||
			m_pEffSetCon[ effsetcon ].m_iSetType == EF_ATTACK_MISS )
		{
			m_pAddParticleSystem->m_vMoveLocation.y += 38.0f; 
				//( pSrc->GetBoundMaxPos().y - pSrc->GetBoundMinVec().y ) * 0.3f;
		}
		else
		{
			m_pAddParticleSystem->m_vMoveLocation.y +=  
				( pSrc->GetBoundMaxPos().y - pSrc->GetBoundMinVec().y ) * pData->m_fHeight;
		}		
	}
	else
	{
		m_pAddParticleSystem->m_vMoveLocation.y +=  ( pSrc->GetBoundMaxVec().y - pSrc->GetBoundMinVec().y ) * 
			pData->m_fHeight;
	}
	
	m_ParticleSystemList.push_back( m_pAddParticleSystem );
}

void		CBravoParticle::DisplayUpdate( int number, long Uniq, int index, int linkLoc, bool link )
{
	CParticleData	*pData = NULL;
	Character		*pSrc  = NULL;
	
	if( Uniq != -1 )
	{
		pSrc = GetCharacter( Uniq , index );
		
		if( pSrc == NULL )
			return;
	}
	
	if( ( pData = m_ParticleData.GetKeyDataPointer( number ) ) == NULL )
		return;	

	CParticleSystem* m_pAddParticleSystem = m_ParticleSystemPool.NewMember();	
	
	if( NULL == m_pAddParticleSystem )
		return;
	
	m_pAddParticleSystem->m_pReference = pData;	
	
	m_pAddParticleSystem->m_lUnique			= Uniq;
	m_pAddParticleSystem->m_iIndex			= index;
	m_pAddParticleSystem->m_bIsSuppressed	= false;
	m_pAddParticleSystem->m_bIsCreate		= true;
	m_pAddParticleSystem->m_iLink			= linkLoc;
	m_pAddParticleSystem->m_fSize			= 1.0f;
	m_pAddParticleSystem->m_bLink			= true;
	m_pAddParticleSystem->m_pSrc			= pSrc;	
	
	
	m_ParticleSystemList.push_back( m_pAddParticleSystem );
}

void		CBravoParticle::StopDisplay( int number )
{
	
	std::list <CParticleSystem *>::iterator i = m_ParticleSystemList.begin();
	
	for( ; i != m_ParticleSystemList.end() ; ++i )
	{
		if( (*i)->m_pReference->m_iParType == (DWORD)number )
		{
			(*i)->m_bIsSuppressed	= true;
		}
	}	
}

void		CBravoParticle::StopDisplay( int number, long Uniq )
{
	std::list <CParticleSystem *>::iterator i = m_ParticleSystemList.begin();
	
	for( ; i != m_ParticleSystemList.end() ; ++i )
	{
		if( (*i)->m_pReference->m_iParType == (DWORD)number && (*i)->m_lLockonUniq == Uniq )
		{
			(*i)->m_bIsSuppressed	= true;
		}
	}
	
}

void		CBravoParticle::StopDisplay( int number, D3DXVECTOR3& loc, int effsetcon, BOOL IsNowDel )
{
	std::list <CParticleSystem *>::iterator i = m_ParticleSystemList.begin();
	
	for( ; i != m_ParticleSystemList.end() ; ++i )
	{
		if( (*i)->m_pReference->m_iParType == (DWORD)number && 
			(*i)->m_vMoveLocation		   ==		 loc	&&
			(*i)->m_iEffSetCon			   ==	     effsetcon )
		{
			if( IsNowDel )
			{
				(*i)->Destroy();			
				CParticleSystem *pData = (*i);			
				m_ParticleSystemPool.DeleteMember( pData );			
				(*i) = NULL;				
				m_ParticleSystemList.erase( i++ );
			}
			else
			{
				if( (*i)->m_bIsSuppressed )
					continue;				
				(*i)->m_bIsSuppressed	= true;				
				break;
			}
			
			break;
		}
	}
	
}

void		CBravoParticle::StopDisplay( int number, long Uniq, int LinkLoc )
{
	std::list <CParticleSystem *>::iterator i = m_ParticleSystemList.begin();
	
	for( ; i != m_ParticleSystemList.end() ; ++i )
	{
		if(	(*i)->m_pReference->m_iParType == (DWORD) number && 
			(*i)->m_lLockonUniq			   ==		  Uniq      )
		{
			if( (*i)->m_bIsSuppressed )
				continue;
			
			(*i)->m_bIsSuppressed	= true;
			
			break;
		}
	}
	
}

void		CBravoParticle::StopDisplay( int number, long Uniq, int index, int linkLoc, int effsetcon, BOOL IsNowDel )
{
	std::list <CParticleSystem *>::iterator i = m_ParticleSystemList.begin();
	
	for( ; i != m_ParticleSystemList.end() ; ++i )
	{
		if(	(*i)->m_pReference->m_iParType == (DWORD)number && 
			(*i)->m_lUnique				   == Uniq			&& 
			(*i)->m_iIndex				   == index			&& 
			(*i)->m_iEffSetCon			   == effsetcon		  )
		{
			
			if( IsNowDel )
			{
				(*i)->Destroy();			
				CParticleSystem *pData = (*i);			
				m_ParticleSystemPool.DeleteMember( pData );			
				(*i) = NULL;				
				m_ParticleSystemList.erase( i++ );
			}
			else
			{
				if( (*i)->m_bIsSuppressed )
					continue;
				
				(*i)->m_bIsSuppressed	= true;
				
				break;
			}
		}
	}
	
}

void		CBravoParticle::StopDisplay( long Uniq, int index )
{
	std::list <CParticleSystem *>::iterator i = m_ParticleSystemList.begin();
	
	for( ; i != m_ParticleSystemList.end() ; ++i )
	{
		if( index != -1 )
		{
			if(	(*i)->m_lUnique == Uniq &&
				(*i)->m_iIndex == index )
			{
				(*i)->m_bIsSuppressed	= true;
			}
		}
		else
		{
			if(	(*i)->m_lUnique == Uniq )
			{
				(*i)->m_bIsSuppressed	= true;
			}
		}
	}
	
}

void		CBravoParticle::DeleteAll(BOOL ISEnd )
{
	std::list <CParticleSystem *>::iterator i = m_ParticleSystemList.begin();
	
	for( ; i != m_ParticleSystemList.end() ; )
	{
		//		if( (*i)->m_lUnique == g_Pc.GetMainPCUniqe() && !ISEnd )
		if( (*i)->m_pSrc == g_Pc.GetPlayer() && !ISEnd )
		{
			i++;
			
			continue;
		}
		else
		{
			(*i)->Destroy();			
			
			CParticleSystem *pData = (*i);			
		
			m_ParticleSystemPool.DeleteMember( pData );
		
			(*i) = NULL;
			
			m_ParticleSystemList.erase( i++ );
		}
	}
}


void		CBravoParticle::RainControl( float time )
{
	if( m_WeatherDrop.GetDropType() != WEATHER_TYPE_NONE && g_ISWeatherVisible )
	{
		D3DXVECTOR3 Pos;
		
		Pos = g_Pc.GetPcInnerPos( -1 , g_Pc.m_MainPC.lUnique , 8.0f );
		
		m_WeatherDrop.Update(time, Pos);
		m_WeatherDrop.Render( g_lpDevice , g_Camera.GetView() , Pos );
	}
	
	m_WeatherDrop.UpdateDNType();
}


//------------------------------------------------------------------------------
//	Desc: 빌보드.
//------------------------------------------------------------------------------
CBillboardManager::CBillboardManager()
{
	for(int i = 0 ; i < 19 ; i++)
	{
		m_pVB[i] = NULL;
		m_pIB[i] = NULL;

		m_pIndicesCount[i] = 0;
		m_pVerticesCount[i] = 0;

	}
	ZeroMemory(m_strDir , MAX_SIZE_256 );
}

CBillboardManager::~CBillboardManager()
{
	InvalidateDeviceObject();
	
	m_pExtraModelList.ClearPointer();
}

HRESULT		CBillboardManager::InvalidateDeviceObject()
{
	for(int i = 0 ; i < eBDType_Count ; i++)
	{
		if(m_pVB[i] != NULL)
		{
			m_pVB[i]->Release();
			m_pVB[i] = NULL;       
		}
		
		if(m_pIB[i] != NULL)
		{
			m_pIB[i]->Release();
			m_pIB[i] = NULL;       
		}	
#ifdef BATCH_RENDER	
// 		SAFE_DELETE_ARRAY( m_pIndices[i] );	
// 		SAFE_DELETE_ARRAY( m_pVertices[i] );
// 		SAFE_DELETE_ARRAY( m_pDefaultVertices[i] );
// 		SAFE_DELETE_ARRAY( m_pDefaultCopyVertices[i] );
		
#endif	//	BATCH_RENDER
	}	

#ifdef BATCH_RENDER	
	SAFE_DELETE_ARRAY( m_pIndices );	
	SAFE_DELETE_ARRAY( m_pVertices );
	SAFE_DELETE_ARRAY( m_pDefaultVertices );
	SAFE_DELETE_ARRAY( m_pDefaultCopyVertices );
#endif	//	BATCH_RENDER

	return S_OK;
}

#ifdef DIRECT_VERSION_9_MJH
HRESULT		CBillboardManager::RestoreDeviceObject(LPDIRECT3DDEVICE9 pDevice)
#else
HRESULT		CBillboardManager::RestoreDeviceObject(LPDIRECT3DDEVICE8 pDevice)
#endif // DIRECT_VERSION_9_MJH
{
	InitManager(m_strDir, TRUE);	
	
	return S_OK;
}

HRESULT		CBillboardManager::InitManager(char* dir, BOOL bRestore)
{
	strcpy(m_strDir, dir);
	
#ifdef BATCH_RENDER
	m_pVertices				= SAFE_NEW_ARRAY( VERTEX_XYZ_DIFFUSE_TEX1*, eBDType_Count );
	m_pDefaultVertices		= SAFE_NEW_ARRAY( VERTEX_XYZ_DIFFUSE_TEX1*, eBDType_Count );
	m_pDefaultCopyVertices	= SAFE_NEW_ARRAY( VERTEX_XYZ_DIFFUSE_TEX1*, eBDType_Count );
	m_pIndices				= SAFE_NEW_ARRAY( WORD* , eBDType_Count );
#endif	//	BATCH_RENDER
	// 0. 실드
	MakeShield();
	// 1. 세로 방향 면
	MakeVerticalPlane();
	// 2. 가로 방향 면
	MakeHorizonPlane();
	// 3. 세로 방향 직사각형
	MakeVerticalLongPlane();
	// 4. 가로 방향 직사각형
	MakeHorizonLongPlane();
	// 5. 실린더
	MakeCylinder();
	// 6. 실린더 위가 큰거
	MakeCylinderUp();
	// 7. 반구	
	MakeHemisphere();
	// 8. 실린더 아래가 큰거
	MakeCylinderDown();
	// 9. 구
	MakeSphere();
	// 10. 도넛 ( UV 개별적 )
	MakeDonut();
	// 11. 도넛 ( UV 전체 )
	MakeDonutUVOne();
	// 12. 스팟 
	MakeSpot();
	// 13. 스팟 ( 뒤로 굽은거 )
	MakeSpot1();
	// 14. 스팟 ( 뒤로 굽은거 )
	MakeSpot2();
	// 15. 칼흔
	MakeSword();
	// 16. 크로스 평면
	MakeCrossPlane();
	// 17. 실린더 각진거 6각
	MakeCylinder6();
	// 18. 실린더 각진거 8각
	MakeCylinder8();
	
	if(!bRestore)
	{
		//빌보드 데이터 읽기 
		LoadBillboard( dir );	
		// 빌보드 도형 ( 맥스 익스포트 용 ) 읽기
		LoadExtraModel( dir );
	}
#ifdef BATCH_RENDER
	SetDefaultVectex();
#endif	//	BATCH_RENDER
	return S_OK;
}

void		CBillboardManager::SetManager( float delaytime, int type, long uniq, int index, int effsetcon )
{
	CBillboardData	*pData = NULL;
	Character		*pSrc  = NULL;
	
	if( uniq != -1 )
	{
		pSrc = g_Particle.GetCharacter( uniq , index );
		
		if( pSrc == NULL )
			return;
	}
	
	if( ( pData = m_BillboardData.GetKeyDataPointer( type ) ) == NULL )
		return;
	
	CBillboard NewBillboard;
	
	// m_iMyType 과도 같은 type
	NewBillboard.m_pBillboardData	= pData;
	
	NewBillboard.m_lUnique			= uniq;
	NewBillboard.m_iIndex			= index;
	NewBillboard.m_fDelayTime		= delaytime;
	
	NewBillboard.m_vStartSize    	= NewBillboard.m_pBillboardData->m_vStartSize * 1.0f;
	NewBillboard.m_vEndSize			= NewBillboard.m_pBillboardData->m_vEndSize * 1.0f;
	
	NewBillboard.m_vMovVec			= NewBillboard.m_pBillboardData->m_vMovVec * 1.0f;
	NewBillboard.m_fRadius			= NewBillboard.m_pBillboardData->m_fRadius * 1.0f;
	
	NewBillboard.m_vLoc				= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	NewBillboard.m_iEffSetCon		= effsetcon;
	NewBillboard.m_pSrc				= pSrc;
	
	NewBillboard.ReStart();
	
	m_pBillboard.push_back( NewBillboard );

#ifdef BATCH_RENDER
	m_pBillboard.sort();
#endif	//	BATCH_RENDER
}

// 오브젝트 용 ( 기본 위치값 설정해야지 )
void CBillboardManager::SetManager( float delaytime, int type, long uniq, int index,
								   D3DXVECTOR3 &loc, int Link, int effsetcon )
{
	CBillboardData	*pData = NULL;
	Character		*pSrc  = NULL;
	
	if( uniq != -1 )
	{
		pSrc = g_Particle.GetCharacter( uniq , index );		
		if( pSrc == NULL )
			return;
	}	
	
	if( ( pData = m_BillboardData.GetKeyDataPointer( type ) ) == NULL )
		return;
	
	CBillboard NewBillboard;
	
	// m_iMyType 과도 같은 type
	NewBillboard.m_pBillboardData	= pData;
	
	NewBillboard.m_lUnique			= uniq;
	NewBillboard.m_iIndex			= index;
	NewBillboard.m_fDelayTime	    = delaytime;
	
	NewBillboard.m_vStartSize	    = NewBillboard.m_pBillboardData->m_vStartSize * 1.0f;
	NewBillboard.m_vEndSize	        = NewBillboard.m_pBillboardData->m_vEndSize * 1.0f;
	
	NewBillboard.m_vMovVec	        = NewBillboard.m_pBillboardData->m_vMovVec * 1.0f;
	NewBillboard.m_fRadius	        = NewBillboard.m_pBillboardData->m_fRadius * 1.0f;
	
	NewBillboard.m_iEffSetCon       = effsetcon;
	NewBillboard.m_BoneLink         = Link;
	NewBillboard.m_aDirectLinkBone  = g_Particle.m_pEffSetCon[ effsetcon ].m_aDirectLinkBone;
	
	NewBillboard.m_pSrc			    = pSrc;
	NewBillboard.m_vAttackSrcDir	= g_Particle.m_pEffSetCon[ effsetcon ].m_vDir;
	NewBillboard.m_uSetType			= g_Particle.m_pEffSetCon[ effsetcon ].m_iSetType;
	NewBillboard.m_pAttackUnit		= g_Particle.m_pEffSetCon[ effsetcon ].m_pAttackUnit;
	NewBillboard.m_IsDirAttatack	= g_Particle.m_pEffSetCon[ effsetcon ].m_IsDirAttatack;
	NewBillboard.m_IsGUnitLinkBoneAndSizeFix = g_Particle.m_pEffSetCon[ effsetcon ].m_IsGUnitLinkBoneAndSizeFix;
	NewBillboard.m_vLoc				= loc;	
		
	if( g_Particle.m_pEffSetCon[ effsetcon ].m_iType == ESLT_SRC_RAD )
	{
		NewBillboard.m_fSetRadian = g_Particle.m_pEffSetCon[ effsetcon ].m_fRad;
		
		if( ( Link < 10 && Link > 0 ) || NewBillboard.m_aDirectLinkBone )
		{
			D3DXMATRIX matLink;
		
			if( !NewBillboard.m_IsDirAttatack )
			{
				if( NewBillboard.m_aDirectLinkBone )
				{
					pSrc->GetDirectBoneLinkTM( NewBillboard.m_aDirectLinkBone, matLink );
				}
				else
				{
					pSrc->GetBoneLinkTM( Link , matLink, FALSE );
				}

				NewBillboard.m_vLoc.x += matLink._41;
				NewBillboard.m_vLoc.y += matLink._42;
				NewBillboard.m_vLoc.z += matLink._43;
			}
			else
			{
				NewBillboard.m_vLoc.x += g_Particle.m_pEffSetCon[ effsetcon ].m_pSrc->GetBonCenterTM()._41;
				NewBillboard.m_vLoc.y += g_Particle.m_pEffSetCon[ effsetcon ].m_pSrc->GetBonCenterTM()._42;
				NewBillboard.m_vLoc.z += g_Particle.m_pEffSetCon[ effsetcon ].m_pSrc->GetBonCenterTM()._43;
			}			
		}
		else
		{
			NewBillboard.m_vLoc.x += g_Particle.m_pEffSetCon[ effsetcon ].m_pSrc->GetBonCenterTM()._41;
			NewBillboard.m_vLoc.y += g_Particle.m_pEffSetCon[ effsetcon ].m_pSrc->GetBonCenterTM()._42;
			NewBillboard.m_vLoc.z += g_Particle.m_pEffSetCon[ effsetcon ].m_pSrc->GetBonCenterTM()._43;
		}	
		
		if( g_Map.IsGiantUnit( pSrc ) )
		{
			if( NewBillboard.m_IsDirAttatack ||
				g_Particle.m_pEffSetCon[ effsetcon ].m_iSetType == EF_ATTACK_MISS )
			{
				NewBillboard.m_vLoc.y += 38.0f;
					//( pSrc->GetBoundMaxPos().y - pSrc->GetBoundMinVec().y ) * 0.3f;
			}
			else
			{
				NewBillboard.m_vLoc.y +=  
					( pSrc->GetBoundMaxPos().y - pSrc->GetBoundMinVec().y ) * pData->m_fHeight;
			}
		}
		else
		{
			NewBillboard.m_vLoc.y +=  ( pSrc->GetBoundMaxVec().y - pSrc->GetBoundMinVec().y ) * 
				pData->m_fHeight;
		}		
	}
	else
	{
		NewBillboard.m_fSetRadian = g_Particle.m_pEffSetCon[ effsetcon ].m_fRad;
	}	
	
	NewBillboard.ReStart();	
	m_pBillboard.push_back( NewBillboard );

#ifdef BATCH_RENDER
	m_pBillboard.sort();
#endif	//	BATCH_RENDER
	
}
								   
BOOL CompSortByEffectType( const void* arg1, const void* arg2 )
{
	CBillboard* first	= (CBillboard*)arg1;
    CBillboard* second	= (CBillboard*)arg2;
	
	if(first->m_pBillboardData->m_iType > second->m_pBillboardData->m_iType)
		return TRUE;
	else
		return FALSE;
}

void CBillboardManager::SetManager( float delaytime, int type, D3DXVECTOR3& loc, 
								   int effsetcon, int nHeight )
{
	CBillboardData *pData = NULL;
	
	if( ( pData = m_BillboardData.GetKeyDataPointer( type ) ) == NULL )
		return;
	
	CBillboard NewBillboard;
	
	// m_iMyType 과도 같은 type
	NewBillboard.m_pBillboardData	= pData;
	NewBillboard.m_lUnique			= -1;
	NewBillboard.m_iIndex			= -1;
	NewBillboard.m_fDelayTime		= delaytime;
	
	NewBillboard.m_vStartSize	    = NewBillboard.m_pBillboardData->m_vStartSize;
	NewBillboard.m_vEndSize			= NewBillboard.m_pBillboardData->m_vEndSize;
	
	NewBillboard.m_vMovVec		    = NewBillboard.m_pBillboardData->m_vMovVec;
	NewBillboard.m_fRadius			= NewBillboard.m_pBillboardData->m_fRadius;
	
	NewBillboard.m_iEffSetCon		= effsetcon;
	
	NewBillboard.m_fSetRadian       = g_Particle.m_pEffSetCon[ effsetcon ].m_fRad;
	NewBillboard.m_nHeight			= (float)nHeight;
	NewBillboard.m_uSetType			= g_Particle.m_pEffSetCon[ effsetcon ].m_iSetType;
	NewBillboard.m_vLoc				= loc;	
	
	NewBillboard.ReStart();
	m_pBillboard.push_back( NewBillboard );

#ifdef BATCH_RENDER
	m_pBillboard.sort();
#endif	//	BATCH_RENDER
}

void CBillboardManager::DisplayManager( float timed )
{
	list<CBillboard>::iterator i = m_pBillboard.begin();
	
	for( ;i != m_pBillboard.end(); )
	{
		D3DXMATRIX  mat;
		
		if( i->m_fDelayTime <= 0.0f )
		{
			if( i->m_pBillboardData->m_iLoop > i->m_iCurLoop )
			{
				int nEffSetCon = i->m_iEffSetCon;
				int nEffType = ESLT_NONE;
				float fRadian = 0.0f;

				if( nEffSetCon != -1 )
				{
					nEffType = g_Particle.m_pEffSetCon[ nEffSetCon ].m_iType;	
					fRadian = g_Particle.m_pEffSetCon[ nEffSetCon ].m_fRad;
				}
				
				//.............................................................................................
				// 캐릭터 소스로 찾는 방식
				//.............................................................................................
				if( !(i->m_IsDirAttatack) && ( i->m_BoneLink != 0 || i->m_aDirectLinkBone != 0 ) 
					&& nEffSetCon != ESLT_SRC_RAD )
				{
					if( i->m_pSrc != NULL )
					{
						if( i->m_pSrc->m_nTableIndex == -1 )
						{
							m_pBillboard.erase( i++ );							
							continue;
						}
						
						BOOL bFlag = TRUE;
						
						if( i->m_aDirectLinkBone )
						{
							bFlag = i->m_pSrc->GetDirectBoneLinkTM( i->m_aDirectLinkBone, mat );
						}
						else
						{
							bFlag = i->m_pSrc->GetBoneLinkTM( i->m_BoneLink , mat, FALSE );
						}
						
						if( bFlag )
						{
							i->m_vLinkBone.x = mat._41;
							i->m_vLinkBone.y = mat._42;
							i->m_vLinkBone.z = mat._43;
							
							if( nEffType == ESLT_CHARWHOLELINK )
							{
								i->m_mLinkBoneMatrix = mat;
							}
						}
						else
						{
							m_pBillboard.erase( i++ );
							
							continue;
						}
					}
					else
					{
						m_pBillboard.erase( i++ );
						
						continue;
					}
				}
				else if( nEffType == ESLT_POS && fRadian != 0.0f )
				{
					i->m_fSetRadian = fRadian;
				}
				//.............................................................................................
				
				if( !i->UpdateBillboard( timed ) )
				{
					m_pBillboard.erase( i++ );
					
					continue;
				}
				
				if( i->m_lUnique != -1 )
				{
					if( i->m_pSrc != NULL )
					{
						if( i->m_pSrc->m_nTableIndex == -1 )
						{
							m_pBillboard.erase( i++ );
							
							continue;
						}
						else
						{
							if( i->m_pSrc->m_bIsVisible == TRUE &&
								i->m_pSrc->m_CharAlpha.GetState() != CHAR_NONE_DISPLAY &&
								i->m_pSrc->m_IsAllLTexture )
							{
									
									BOOL IsBillboadRender = TRUE;
									
									if( i->m_pSrc == g_Pc.GetPlayer() && !(g_Pc.GetPlayer()->IsMyPlayerRender) )
									{
										IsBillboadRender = FALSE;		
									}									
									
									if( IsBillboadRender )
									{
//#if 0
#ifdef BATCH_RENDER
										i->DrawBatchBillboard();
#else	//	BATCH_RENDER
										i->DrawBillboard();
#endif	//	BATCH_RENDER
									}						
							}								
						}
					}
					else 
					{
						m_pBillboard.erase( i++ );
						
						continue;
					}
				}
				else
				{
//#if 0
#ifdef BATCH_RENDER
					i->DrawBatchBillboard();
#else	//	BATCH_RENDER
					i->DrawBillboard();
#endif	//	BATCH_RENDER
				}			
				
				i->m_fCurLife	+= timed;	
				
				if( i->m_pBillboardData->m_fLife <= i->m_fCurLife )
				{
					if( !i->m_pBillboardData->m_bInfinityLoop )
					{
						i->m_iCurLoop++;
						i->m_fCurLife = 0.0f;
					}
					else //-- 무한 루프일경우
					{
						int nMok = (int)(i->m_fCurLife / i->m_pBillboardData->m_fLife);
						float fRemainder = i->m_fCurLife - (i->m_pBillboardData->m_fLife * nMok);
						i->m_fCurLife = fRemainder;
					}					
					
					// 각도 증감 모드일때
					if( i->m_pBillboardData->m_bBlend )
					{
						float TempAngle = ( float ) D3DXToDegree( i->m_AddRadian ) + 
							( float ) D3DXToDegree( i->m_pBillboardData->m_fAngleX + ( i->m_pBillboardData->m_fRotVel * 6.28f ) );
						
						if( TempAngle >= 360.0f )
						{
							TempAngle -= 360.0f;
						}
						else
							if( TempAngle <= -360.0f )
							{
								TempAngle += 360.0f;
							}
							
							i->m_AddRadian = ( float ) D3DXToRadian( TempAngle );
					}
					else
					{
						i->m_AddRadian = 0.0f;
					}
					
					
					if( i->m_pBillboardData->m_bSizePump )
						i->m_bPumping	= !i->m_bPumping;
					
					if( i->m_pBillboardData->m_bMovePump )
						i->m_bMoving	= !i->m_bMoving;
					
					if( i->m_pBillboardData->m_bBlendLoop )
						i->m_LoopBlending = !i->m_LoopBlending;
					
					i->ColorReset();
				}
			}
			else
			{
				m_pBillboard.erase( i++ );
				
				continue;
			}
			
			i++;
		}
		else
		{
			i->m_fDelayTime	-= timed;			
			i++;
		}
	}
#ifdef BATCH_RENDER
//	CBillboardData*		prevBillboardData	= g_RenderManager.GetPrevBillboardData();
//	if(prevBillboardData)
#ifdef DIRECT_VERSION_9_MJH
LPDIRECT3DTEXTURE9	prevBillboardTexture = g_RenderManager.GetPrevBillboardTexture();
#else
LPDIRECT3DTEXTURE8	prevBillboardTexture = g_RenderManager.GetPrevBillboardTexture();
#endif // DIRECT_VERSION_9_MJH
	if(prevBillboardTexture)
	{
		DWORD dColOp = g_RenderManager.GetTextureColorOP( 0 );
		DWORD dColArg1 = g_RenderManager.GetTextureColorArg1( 0 );
		DWORD dColArg2 = g_RenderManager.GetTextureColorArg2( 0 );
		
		DWORD dAlpOp = g_RenderManager.GetTextureAlphaOP( 0 );
		DWORD dAlpArg1 = g_RenderManager.GetTextureAlphaArg1( 0 );
		DWORD dAlpArg2 = g_RenderManager.GetTextureAlphaArg2( 0 );

		g_RenderManager.SetTextureAddressU( 0 , D3DTADDRESS_WRAP );
		g_RenderManager.SetTextureAddressV( 0 , D3DTADDRESS_WRAP );
		g_RenderManager.SetTextureAddressW( 0 , D3DTADDRESS_WRAP );

		g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_MODULATE );
		g_RenderManager.SetTextureColorArg1( 0 , D3DTA_TEXTURE );
		g_RenderManager.SetTextureColorArg2( 0 , D3DTA_DIFFUSE );
		
		g_RenderManager.SetTextureAlphaOP  ( 0 , D3DTOP_MODULATE );
		g_RenderManager.SetTextureAlphaArg1( 0 , D3DTA_TEXTURE );
		g_RenderManager.SetTextureAlphaArg2( 0 , D3DTA_DIFFUSE );		
		g_RenderManager.SetLighting( FALSE );
		g_RenderManager.SetCullMode( D3DCULL_NONE );
		g_RenderManager.DisableTextureStageState( 1 );
		g_RenderManager.DisableTextureStageState( 2 );
// 		g_RenderManager.SetZWriteEnable( FALSE );
// 		g_RenderManager.SetAlphaBlendEnable(  TRUE );

//		g_RenderManager.SetTexture( 0, g_RenderManager.GetPrevBillboardTexture() );
// 		g_RenderManager.SetTexture( 0, prevBillboardData->GetTexture() );
// 		g_RenderManager.SetSrcBlend( prevBillboardData->m_SrcBlend );
// 		g_RenderManager.SetDestBlend( prevBillboardData->m_DestBlend );
		g_RenderManager.SetTexture( 0, prevBillboardTexture );
		
//		g_RenderManager.SetTexture( 0, prevBillboardTexture );
		g_RenderManager.SetSrcBlend( g_RenderManager.m_PrevSrcBlend );
		g_RenderManager.SetDestBlend( g_RenderManager.m_PrevDestBlend );
		g_RenderManager.EndBatchPrimitive();
		g_RenderManager.SetBeginBatchRendering(FALSE);

		g_RenderManager.SetTextureColorOP  ( 0 , dColOp );
		g_RenderManager.SetTextureColorArg1( 0 , dColArg1 );
		g_RenderManager.SetTextureColorArg2( 0 , dColArg2 );
		
		g_RenderManager.SetTextureAlphaOP  ( 0 , dAlpOp );
		g_RenderManager.SetTextureAlphaArg1( 0 , dAlpArg1 );
		g_RenderManager.SetTextureAlphaArg2( 0 , dAlpArg2 );
	}
#endif	//	BATCH_RENDER
}

//................................................................................................................
void		CBillboardManager::DisplayManager( CBillboard &Billboard,float timed )
{
	CBillboard &pBillboard = Billboard;
	
	D3DXMATRIX  mat;
	
	if( pBillboard.m_pBillboardData->m_iLoop > pBillboard.m_iCurLoop )
	{
		if( !pBillboard.UpdateBillboardNoneSet( timed ) )
		{
			return;	
		}		
		
		pBillboard.m_fCurLife += timed;
		
		if( pBillboard.m_pBillboardData->m_fLife <= pBillboard.m_fCurLife )
		{
			if( !pBillboard.m_pBillboardData->m_bInfinityLoop )
			{
				pBillboard.m_iCurLoop++;
				pBillboard.m_fCurLife = 0.0f;
			}
			else //-- 무한 루프일경우
			{
				int nMok = (int)(pBillboard.m_fCurLife / pBillboard.m_pBillboardData->m_fLife);
				float fRemainder = pBillboard.m_fCurLife - (pBillboard.m_pBillboardData->m_fLife * nMok);
				pBillboard.m_fCurLife = fRemainder;
			}			
			
			// 각도 증감 모드일때
			if( pBillboard.m_pBillboardData->m_bBlend )
			{
				float TempAngle = ( float ) D3DXToDegree( pBillboard.m_AddRadian ) + 
					( float ) D3DXToDegree( pBillboard.m_pBillboardData->m_fAngleX + ( pBillboard.m_pBillboardData->m_fRotVel * 6.28f ) );
				
				if( TempAngle >= 360.0f )
				{
					TempAngle -= 360.0f;
				}
				else
					if( TempAngle <= -360.0f )
					{
						TempAngle += 360.0f;
					}
					
					pBillboard.m_AddRadian = ( float ) D3DXToRadian( TempAngle );
			}
			else
			{
				pBillboard.m_AddRadian = 0.0f;
			}			
			
			if( pBillboard.m_pBillboardData->m_bSizePump )
				pBillboard.m_bPumping	= !pBillboard.m_bPumping;
			
			if( pBillboard.m_pBillboardData->m_bMovePump )
				pBillboard.m_bMoving	= !pBillboard.m_bMoving;
			
			if( pBillboard.m_pBillboardData->m_bBlendLoop )
				pBillboard.m_LoopBlending = !pBillboard.m_LoopBlending;
			
			pBillboard.ColorReset();
		}
	}
}
//................................................................................................................

void		CBillboardManager::DelManager( int type, long uniq, int index, int effsetcon )
{
	list< CBillboard >::iterator i = m_pBillboard.begin();
	
	for( ;i != m_pBillboard.end(); i++ )
	{
		CBillboard &pBillboard = *i;
		
		if( pBillboard.m_pBillboardData->m_iMyType == type && 
			pBillboard.m_lUnique == uniq && 
			pBillboard.m_iIndex == index &&
			pBillboard.m_iEffSetCon	== effsetcon )
		{
			m_pBillboard.erase( i++ );
		}
	}
}


void		CBillboardManager::DelManager( int type, D3DXVECTOR3& loc, int effsetcon )
{
	list< CBillboard >::iterator i = m_pBillboard.begin(); 
	
	for( ; i != m_pBillboard.end(); i++ )
	{
		CBillboard &pBillboard = *i;
		if( pBillboard.m_pBillboardData->m_iMyType == type && 
			pBillboard.m_vLoc == loc &&
			pBillboard.m_iEffSetCon	== effsetcon )
		{
			m_pBillboard.erase( i++ );
		}
	}
}


void		CBillboardManager::DelManager( long uniq, int index )
{
	
	//	vector< CBillboard >::iterator p = m_pBillboard.end();
	//	int size = m_pBillboard.size();
	
	for( list< CBillboard >::iterator i = m_pBillboard.begin(); i != m_pBillboard.end(); i++ )
	{
		CBillboard &pBillboard = *i;
		
		if( index != -1 )
		{
			if( pBillboard.m_lUnique == uniq && pBillboard.m_iIndex == index )
			{
				m_pBillboard.erase( i++ );
			}
		}
		else
		{
			if( pBillboard.m_lUnique == uniq )
			{
				m_pBillboard.erase( i++ );
			}
		}
	}
}

void		CBillboardManager::DelAllManager(BOOL ISEnd)
{
	if( g_RockClient.m_IsMapLoding )
	{
		list <CBillboard> ::iterator i = m_pBillboard.begin();
		
		for( ; i != m_pBillboard.end() ; )
		{
			if( i->m_lUnique == g_Pc.GetMainPCUniqe() && !ISEnd )
			{
				i++;
				continue;
			}
			else
			{
				m_pBillboard.erase( i++ );
			}
		}
	}
	else
	{
		list <CBillboard> ::iterator i = m_pBillboard.begin();
		
		for( ; i != m_pBillboard.end() ; )
		{
			m_pBillboard.erase( i++ );
		}
	}
}

//................................................................................................................
// 빌보드 데이터 읽기 
//................................................................................................................
int	CBillboardManager::LoadBillboard(char *pPath)
{
	char FileName[ 1024 ] = "";
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined( LOAD_VF ) /////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	wsprintf( FileName , "%s\\Effect\\*.eb" , pPath );
	
	WIN32_FIND_DATA	 fd;
	
	HANDLE hFindFile = FindFirstFile( FileName , &fd );
	
	if( hFindFile == INVALID_HANDLE_VALUE )	return FALSE;
	
	int Find	= TRUE;
	
	int Index = 0;
	
	CBillboardData	 TempData;
	CBillboardData	*pData = NULL;
	
	while( Find != FALSE )
	{			
		Index = GetIndexKeyInFileName( fd.cFileName );
		
		if( Index == -1 )
			continue;
		
		pData = m_BillboardData.Insert( Index , TempData );
		
		pData->LoadData( fd.cFileName );
		
		pData->m_iMyType = Index;
		
		Find = FindNextFile( hFindFile, &fd );
	}					
	
	FindClose( hFindFile );
	
	///////////////////////////////////////////////////////////////////////////////////////////////////	
#else  ////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
	
	VFileHandleList* pFHL;
	VFileHandleList_i it;
	
	CBillboardData	 TempData;
	CBillboardData	*pData = NULL;
	
	char buf[256] = {0,};
	int Index = 0;
	
	pFHL = g_VFEffectRoot.GetFileHandleList();
	for( it = pFHL->begin(); it != pFHL->end(); ++it )
	{
		if( (*it)->GetFileContext()->iType == FT_FILE )
		{
			GetFileExt( (*it)->GetFileContext()->strName, buf );
			
			if( lstrcmp( buf, "eb" ) == 0 )	
			{
				Index = GetIndexKeyInFileName( (*it)->GetFileContext()->strName );
				if( Index == -1 )
					continue;
				
				pData = m_BillboardData.Insert( Index , TempData );
				
				pData->LoadData( (*it)->GetFileContext()->strName );
				
				pData->m_iMyType = Index;
			}
		}
	}
	
	///////////////////////////////////////////////////////////////////////////////////////////////////	
#endif ////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
	
	return TRUE;
}

//................................................................................................................
// 익스 포트 데이터 읽기
//................................................................................................................
int	CBillboardManager::LoadExtraModel(char *pPath)
{
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined( LOAD_VF ) /////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	char FileName[ 1024 ];
	char FullPath[ 1024 ];
	
	
	wsprintf( FileName , "%s\\Effect\\static\\*.bmd" , pPath );
	
	WIN32_FIND_DATA	 fd;
	
	HANDLE hFindFile = FindFirstFile( FileName , &fd );
	
	if( hFindFile == INVALID_HANDLE_VALUE )	return FALSE;
	
	int Find	= TRUE;
	
	int Index = 0;
	
	while( Find != FALSE )
	{			
		Index = GetIndexKeyInFileName( fd.cFileName );
		
		if( Index == -1 )
			continue;
		
		Model	*pTempData = NULL;
		
		pTempData = SAFE_NEW( Model );			
		
		wsprintf( FullPath , "%s\\Effect\\static\\%s" , pPath , fd.cFileName );
		
		pTempData->Read( FullPath , TRUE  );
		
		g_Billboard.m_pExtraModelList.Insert( Index , pTempData );
		
		Find = FindNextFile( hFindFile, &fd );
	}					
	
	FindClose( hFindFile );
	
	///////////////////////////////////////////////////////////////////////////////////////////////////	
#else  ////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
	
	VFileHandleList* pFHL;
	VFileHandleList_i it;
	
	char buf[256] = {0,};
	int Index = 0;

	map <int, int> tempList;
	
	pFHL = g_VFEffectStatic.GetFileHandleList();
	for( it = pFHL->begin(); it != pFHL->end(); ++it )
	{
		if( (*it)->GetFileContext()->iType == FT_FILE )
		{
			GetFileExt( (*it)->GetFileContext()->strName, buf );
			
			if( lstrcmp( buf, "bmd" ) == 0 )	
			{
				string strTemp = (*it)->GetFileContext()->strName;
				Index = GetIndexKeyInFileName( (*it)->GetFileContext()->strName );
				if( Index == -1 )
					continue;

				map < int, int >::iterator fPos = tempList.find(Index);
				if(fPos != tempList.end())
				{
					continue;
				}

				tempList.insert(map <int, int>::value_type(Index, Index));
				
				Model	*pTempData = NULL;
				
				pTempData = SAFE_NEW( Model );
				
				pTempData->ReadVF( &g_VFEffectStatic , (*it)->GetFileContext()->strName , TRUE );
				
				m_pExtraModelList.Insert( Index , pTempData );
			}
		}
	}
	
	///////////////////////////////////////////////////////////////////////////////////////////////////	
#endif ////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
	
	return TRUE;
}

//-----------------------------------------------------------------------------
// 이펙트 오브젝트에서 사용할 mod, tex 파일을 로딩한다.
// Effect폴더에 있는 이펙트 오브젝트파일을 로딩해서 자식갯수(EFFECTOBJ_CHILDCOUNT)
// 만큼 생성한다
//-----------------------------------------------------------------------------
void	 CBravoParticle::CreateEffectObj( char* file_pos )
{
	//ProfileBegin( "CBravoParticle::CreateEffectObj" );
	char file_path[256];	
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined( LOAD_VF ) /////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	//-----------------------------------------------------------------------------
	// 모델파일 읽기
	//-----------------------------------------------------------------------------
	int mod_count = g_Obj_Manager.GetFileCount( file_pos, "\\Effect\\Data\\*.msh" );
	for( int i = 0; i < mod_count; ++i )
	{
		///-- Critical Error Check : Array Index
		assert( i < 128 ); 
		sprintf( file_path, "%s\\Effect\\Data\\%08d.msh", file_pos, i  );
		m_Mod[i].Read( file_path, true );		
	}
	
	//-----------------------------------------------------------------------------
	// 텍스쳐 파일 메모리에 올리기
	//-----------------------------------------------------------------------------
	int tex_count = g_Obj_Manager.GetFileCount( file_pos, "\\Effect\\Data\\*.tex" );	
	for( i = 0; i < tex_count; ++i )
	{
		///-- Critical Error Check : Array Index
		assert( i < 128 ); 
		sprintf( file_path, "%s\\Effect\\Data\\%08d.tex", file_pos, i  );		
		m_Tex[i].Read( file_path, n_Texture );		
	}
	
	//-----------------------------------------------------------------------------
	// 이펙트 오브젝트 파일읽기
	//-----------------------------------------------------------------------------
	m_effectobjcount = g_Obj_Manager.GetFileCount( file_pos, "\\Effect\\Data\\*.obj" );	
	for( i = 0; i < m_effectobjcount; ++i )
	{
		///-- Critical Error Check : Array Index
		assert( i < 128 ); 
		sprintf( file_path, "%s\\Effect\\Data\\%08d.obj", file_pos, i  );		
		m_EffectObj[i].m_child_count = EFFECTOBJ_CHILDCOUNT;
		m_EffectObj[i].SetupIndex( i );
		m_EffectObj[i].File__Open( file_path, true );
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////	
#else  ////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
	
	//-----------------------------------------------------------------------------
	// 모델파일 읽기
	//-----------------------------------------------------------------------------
	//ProfileBegin( "EffObjModel" );
	int mod_count = g_Obj_Manager.GetFileCountVF( &g_VFEffectData, "msh" );
	for( int i = 0; i < mod_count; ++i )
	{
		///-- Critical Error Check : Array Index
		assert( i < 128 ); 
		sprintf( file_path, "%08d.msh", i  );		
		m_Mod[i].ReadVF( &g_VFEffectData, file_path, true );		
	}
	
	//ProfileEnd( "EffObjModel" );
	//-----------------------------------------------------------------------------
	// 텍스쳐 파일 메모리에 올리기
	//-----------------------------------------------------------------------------
	//ProfileBegin( "EffObjtex" );
	int tex_count = g_Obj_Manager.GetFileCountVF( &g_VFEffectData, "tex" );
	for(int i = 0; i < tex_count; ++i )
	{
		///-- Critical Error Check : Array Index
		assert( i < 128 ); 
		sprintf( file_path, "%08d.tex", i  );			
		m_Tex[i].ReadVF( &g_VFEffectData, file_path, n_Texture );		
	}
	//ProfileEnd( "EffObjtex" );
	
	//-----------------------------------------------------------------------------
	// 이펙트 오브젝트 파일읽기
	//-----------------------------------------------------------------------------
	//ProfileBegin( "effobjobj" );
	m_effectobjcount = g_Obj_Manager.GetFileCountVF( &g_VFEffectData, "obj" );
	for(int i = 0; i < m_effectobjcount; ++i )
	{
		///-- Critical Error Check : Array Index
		assert( i < 128 ); 
		sprintf( file_path, "%08d.obj", i  );		
		m_EffectObj[i].m_child_count = EFFECTOBJ_CHILDCOUNT;
		m_EffectObj[i].SetupIndex( i );
		m_EffectObj[i].File__OpenVF( &g_VFEffectData, file_path, true );
	}
	//ProfileEnd( "effobjobj" );
	
	///////////////////////////////////////////////////////////////////////////////////////////////////	
#endif ////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
	//ProfileEnd( "CBravoParticle::CreateEffectObj" );
}  


void CBravoParticle::SetEffSetCon( float delaytime, int settype, int type, float life,
								  long tuniq, int tidx, D3DXVECTOR3 pos, float rad, int tail, int nHeight )
{
	//............................................................................................................
	// 셋 화일 있는지 검사
	//............................................................................................................
	CEffectSet *pEffectSet = NULL;
	
	pEffectSet = m_EffectSet.GetKeyDataPointer( settype );
	
	if( NULL == pEffectSet )	
		return;
	//............................................................................................................
	
	D3DXVECTOR3 vDir = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	
	if( ESLT_CHARPOS == type || 
		ESLT_SRC_RAD == type ||
		ESLT_CPOS_ICAMERA == type ||
		ESLT_CHARWHOLELINK == type )
	{
		if( tuniq == -1 )
		{
			return;
		}
		else
		{
			Character *pSrc = g_Particle.GetCharacter( tuniq , tidx );
			
			if( pSrc == NULL )
			{
				return;
			}
			else if( pSrc->m_nTableIndex == -1 )
			{
				return;
			}
			
			if( m_IsDirAttatack && m_pAttackUnit )
			{
				D3DXVECTOR3 vSrctPos = D3DXVECTOR3( m_pAttackUnit->GetPosTM()._41, 
												    m_pAttackUnit->GetPosTM()._42, 
													m_pAttackUnit->GetPosTM()._43);				
				
				D3DXVECTOR3 vDestPos = D3DXVECTOR3( pSrc->GetPosTM()._41, 
												    pSrc->GetPosTM()._42, 
												    pSrc->GetPosTM()._43);
				vDir = vSrctPos - vDestPos;
				D3DXVec3Normalize(&vDir, &vDir);
				
				if(!g_Map.IsGiantUnit(pSrc))	//대상이 대형유닛 일때만 적용
				{
					m_IsDirAttatack = FALSE;
					m_pAttackUnit = NULL;
				}
			}			
		}		
	}
	
	for( int i = 0; i < 512; i++ )
	{
		if( m_pEffSetCon[i].m_iType == ESLT_NONE )
		{
			m_pEffSetCon[i].m_iType	= type;
			m_pEffSetCon[i].m_fLife	= life;
			m_pEffSetCon[i].m_lTargetUniq	= tuniq;
			m_pEffSetCon[i].m_iTargetIndex	= tidx;
			m_pEffSetCon[i].m_vPos			= pos;
			m_pEffSetCon[i].m_fRad			= rad;
			m_pEffSetCon[i].m_fDelayTime	= delaytime;
			m_pEffSetCon[i].m_iSetType		= settype;
			m_pEffSetCon[i].m_iTailType		= tail;
			m_pEffSetCon[i].m_SceneMode     = g_RockClient.m_SceneManager.GetEffectCreateMode();
			m_pEffSetCon[i].m_nHeight = nHeight;
			m_pEffSetCon[i].m_vDir = vDir;
			m_pEffSetCon[i].m_pAttackUnit = m_pAttackUnit;
			m_pEffSetCon[i].m_IsDirAttatack = m_IsDirAttatack;
			m_pEffSetCon[i].m_IsGUnitLinkBoneAndSizeFix = m_IsGUnitLinkBoneAndSizeFix;
			m_pEffSetCon[i].m_aDirectLinkBone = m_aDirectLinkBone;
			m_pEffSetCon[i].m_fAddSize = m_fAddSize;
			
			if( ISZERO(EFFECT_SET_LOOP_TIME - life)  )
			{	
				m_pEffSetCon[i].m_ConSkill = true;
			}			
			break;
		}
	}
	
	if(m_pAttackUnit)
	{
		m_pAttackUnit = NULL;
	}

	if(m_IsDirAttatack)
	{
		m_IsDirAttatack = FALSE;
	}

	if(m_IsGUnitLinkBoneAndSizeFix)
	{
		m_IsGUnitLinkBoneAndSizeFix = FALSE;
	}

	if(m_aDirectLinkBone)
	{
		m_aDirectLinkBone = 0;
	}

	if( !ISZERO(m_fAddSize) )
	{
		m_fAddSize = 0.0f;
	}
}

///-- 일단 이루틴은 플레이어가 맵을 이동하더라도 자신의 이펙트는 유지하기 때문에... 잠시 보류
void CBravoParticle::EffSetConAllInit(BOOL ISEnd)
{
	for( int i = 0; i < 512; i++ )
	{
		if( m_pEffSetCon[i].m_iType != ESLT_NONE )
		{
			if( m_pEffSetCon[i].m_iTargetIndex == -1 )
			{
				if( ( m_pEffSetCon[i].m_lTargetUniq == g_Pc.m_MainPC.lUnique ) && !ISEnd )
				{
					continue;
				}
			}

			m_pEffSetCon[i].Init();
		}
	}
}

//...................................................................................................................
// 이펙터 셋트 처리부분 ( 이펙트 이동 효과 처리 m_pEffSetCon[i].m_iType 별로 하드 코딩 T.T; ) By wxywxy
//...................................................................................................................
void	CBravoParticle::EffSetConProcess( float fTimeDelta )
{
	for( int i = 0; i < 512; i++ )
	{
		//...........................................................................................................
		// 이펙트가 있는지 검사
		//...........................................................................................................
		if( m_pEffSetCon[i].m_iType != ESLT_NONE )
		{
			//.......................................................................................................
			// 효과전 딜레이 타임 체크
			//.......................................................................................................
			if( m_pEffSetCon[i].m_fDelayTime <= 0.0f )
			{
				//...................................................................................................
				// 처음 시작인감?
				//...................................................................................................
				if( m_pEffSetCon[i].m_bMake )
				{
					//...............................................................................................
					// 살아 있을때
					//...............................................................................................
					if( m_pEffSetCon[i].m_fLife > 0.0f )
						Process_EffectSet( i , fTimeDelta );				// 이펙트 이동 처리 By wxywxy
					//...............................................................................................
					// 죽을때
					//...............................................................................................
					else 
						Stop_EffectSet( i );								// 이펙트 종료 처리 By wxywxy
				}
				//..................................................................................................
				// 이펙트 만들기
				//..................................................................................................
				else
				{
					e_SCENE_MODE SaveMode = g_RockClient.m_SceneManager.GetEffectCreateMode();
					
					g_RockClient.m_SceneManager.SetEffectCreateMode( m_pEffSetCon[i].m_SceneMode );
					
					Setup_EffectSet( i );		// 이펙트 셋 초기 설정 By wxywxy
					
					g_RockClient.m_SceneManager.SetEffectCreateMode( SaveMode );
				}
				
			}
			//.......................................................................................................
			// 효과전 딜레이타입중일 때
			//.......................................................................................................
			else
			{
				m_pEffSetCon[i].m_fDelayTime	-= fTimeDelta;
			}
			
		}// 이펙트가 없을때
		
	}// 이펙트 루프
}

void	CBravoParticle::DelEffSetCon_All( int settype, int type )
{
	CEffectSet	*pEffectSet = NULL;
	
	for( int i = 0; i < 512; i++ )
	{
		if( m_pEffSetCon[i].m_iType != ESLT_NONE )
		{
			if( m_pEffSetCon[i].m_iSetType	   == settype &&
				m_pEffSetCon[i].m_iType		   == type       )
			{
				pEffectSet = m_EffectSet.GetKeyDataPointer( m_pEffSetCon[i].m_iSetType );
				
				if( pEffectSet == NULL ) return;
				
				switch( m_pEffSetCon[i].m_iType )
				{
				case ESLT_CHARPOS:
				case ESLT_CHARWHOLELINK:					
				case ESLT_CPOS_ICAMERA:					
					{
						for( int j = 0; j < pEffectSet->m_iDrawParCount; j++ )
						{
							StopDisplay( pEffectSet->m_ParticleInfo[j].index,
								m_pEffSetCon[i].m_lTargetUniq, m_pEffSetCon[i].m_iTargetIndex,
								-1, i );
						}
						for(int j = 0; j < pEffectSet->m_iDrawBillCount; j++ )
						{
							g_Billboard.DelManager( pEffectSet->m_BillboardInfo[j].index,
								m_pEffSetCon[i].m_lTargetUniq, m_pEffSetCon[i].m_iTargetIndex, i );
						}
					}
					break;
					
				case ESLT_POS:				  
					{	
						D3DXVECTOR3 vTempPos;
						
						for( int j = 0; j < pEffectSet->m_iDrawParCount; j++ )
						{
							vTempPos = m_pEffSetCon[i].m_vPos + pEffectSet->m_ParticleInfo[j].vPos;
							
							StopDisplay( pEffectSet->m_ParticleInfo[j].index,
								vTempPos , i );
						}
						for(int j = 0; j < pEffectSet->m_iDrawBillCount; j++ )
						{
							vTempPos = m_pEffSetCon[i].m_vPos + pEffectSet->m_BillboardInfo[j].vPos;
							
							g_Billboard.DelManager( pEffectSet->m_BillboardInfo[j].index,
								vTempPos , i );
						}
					}
					
					break;
				}
				
				for( int k = 0; k < m_effectobjcount; ++k )
				{	
					for( int l = 0; l < EFFECTOBJ_CHILDCOUNT; ++l )
					{
						if( m_EffectObj[k].m_pAniPos[l].m_iEffSetCon == i )
							DelEffectObj( k, l );					
					}
				}
				
				m_pEffSetCon[i].Init();
			}
		}
	}
}

void	CBravoParticle::DelEffSetCon( int SetNum )
{
	CEffectSet	*pEffectSet = NULL;
	
	if( m_pEffSetCon[SetNum].m_iType != -1 )
	{
		pEffectSet = m_EffectSet.GetKeyDataPointer( m_pEffSetCon[SetNum].m_iSetType );
		
		if( pEffectSet == NULL ) return;
		
		switch( m_pEffSetCon[SetNum].m_iType )
		{
		case ESLT_CHARPOS:	// 제자리 뻥
		case ESLT_CHARWHOLELINK:		
		case ESLT_CPOS_ICAMERA:
			{
				for( int j = 0; j < pEffectSet->m_iDrawParCount; j++ )
				{
					StopDisplay( pEffectSet->m_ParticleInfo[j].index,
						m_pEffSetCon[SetNum].m_lTargetUniq, m_pEffSetCon[SetNum].m_iTargetIndex,
						-1, SetNum );
				}
				for(int j = 0; j < pEffectSet->m_iDrawBillCount; j++ )
				{
					g_Billboard.DelManager( pEffectSet->m_BillboardInfo[j].index,
						m_pEffSetCon[SetNum].m_lTargetUniq, m_pEffSetCon[SetNum].m_iTargetIndex, SetNum );
				}
			}
			break;
		case ESLT_POS:
			{	
				D3DXVECTOR3 vTempPos;
				
				for( int j = 0; j < pEffectSet->m_iDrawParCount; j++ )
				{
					vTempPos = m_pEffSetCon[SetNum].m_vPos + pEffectSet->m_ParticleInfo[j].vPos;
					
					StopDisplay( pEffectSet->m_ParticleInfo[j].index,
						vTempPos , SetNum );
				}
				for(int j = 0; j < pEffectSet->m_iDrawBillCount; j++ )
				{
					vTempPos = m_pEffSetCon[SetNum].m_vPos + pEffectSet->m_BillboardInfo[j].vPos;
					
					g_Billboard.DelManager( pEffectSet->m_BillboardInfo[j].index,
						vTempPos , SetNum );
				}
			}
			
			break;		
		}

		for( int k = 0; k < m_effectobjcount; ++k )
		{	
			for( int l = 0; l < EFFECTOBJ_CHILDCOUNT; ++l )
			{
				if( m_EffectObj[k].m_pAniPos[l].m_iEffSetCon == SetNum )
					DelEffectObj( k, l );				
			}
		}
		
		m_pEffSetCon[SetNum].Init();
	}
}

void	CBravoParticle::DelEffSetCon( int settype, int type, long tuniq, int tidx, BOOL IsNowDel, BOOL IsAllDelete )
{
	CEffectSet	*pEffectSet = NULL;
	
	for( int i = 0; i < 512; i++ )
	{
		if( m_pEffSetCon[i].m_iType != ESLT_NONE )
		{
			if( m_pEffSetCon[i].m_iSetType	   == settype &&
				m_pEffSetCon[i].m_iType		   == type    &&
				m_pEffSetCon[i].m_lTargetUniq  == tuniq   &&
				m_pEffSetCon[i].m_iTargetIndex == tidx )
			{
				pEffectSet = m_EffectSet.GetKeyDataPointer( m_pEffSetCon[i].m_iSetType );
				
				if( pEffectSet == NULL ) return;
				
				switch( m_pEffSetCon[i].m_iType )
				{
				case ESLT_CHARPOS:
				case ESLT_CHARWHOLELINK:				
				case ESLT_CPOS_ICAMERA:
					{
						for( int j = 0; j < pEffectSet->m_iDrawParCount; j++ )
						{
							StopDisplay( pEffectSet->m_ParticleInfo[j].index,
								m_pEffSetCon[i].m_lTargetUniq, m_pEffSetCon[i].m_iTargetIndex,
								-1, i, IsNowDel );
						}
						
						for(int j = 0; j < pEffectSet->m_iDrawBillCount; j++ )
						{
							g_Billboard.DelManager( pEffectSet->m_BillboardInfo[j].index,
								m_pEffSetCon[i].m_lTargetUniq, m_pEffSetCon[i].m_iTargetIndex, i );
						}
					}
					break;
					
				case ESLT_POS:
					{	
						D3DXVECTOR3 vTempPos;
						
						for( int j = 0; j < pEffectSet->m_iDrawParCount; j++ )
						{
							vTempPos = m_pEffSetCon[i].m_vPos + pEffectSet->m_ParticleInfo[j].vPos;
							
							StopDisplay( pEffectSet->m_ParticleInfo[j].index,
								vTempPos , i, IsNowDel );
						}

						for(int j = 0; j < pEffectSet->m_iDrawBillCount; j++ )
						{
							vTempPos = m_pEffSetCon[i].m_vPos + pEffectSet->m_BillboardInfo[j].vPos;
							
							g_Billboard.DelManager( pEffectSet->m_BillboardInfo[j].index,
								vTempPos , i );
						}
					}
					
					break;
				}
				
				for( int k = 0; k < m_effectobjcount; ++k )
				{	
					for( int l = 0; l < EFFECTOBJ_CHILDCOUNT; ++l )
					{
						if( m_EffectObj[k].m_pAniPos[l].m_iEffSetCon == i )
							DelEffectObj( k, l );						
					}
				}

				m_pEffSetCon[i].Init();
			
				///--JAPAN_BUG_MODIFY
				///-- 아 진짜... 이상하게 만들어져있네 
				if( !IsAllDelete )
				{
					///--PRINT_DLG_LOG("이펙트 - 연결 효과 사라짐");
					break;
				}
			}
		}
	}
}

void	CBravoParticle::DelEffSetCon( long tuniq, int tidx )
{
	CEffectSet	*pEffectSet = NULL;
	
	for( int i = 0; i < 512; i++ )
	{
		if( m_pEffSetCon[i].m_iType != ESLT_NONE )
		{
			if( m_pEffSetCon[i].m_lTargetUniq  == tuniq   &&
				m_pEffSetCon[i].m_iTargetIndex == tidx )
			{
				pEffectSet = m_EffectSet.GetKeyDataPointer( m_pEffSetCon[i].m_iSetType );
				
				if( pEffectSet == NULL ) return;
				
				switch( m_pEffSetCon[i].m_iType )
				{
				case ESLT_CHARPOS:
				case ESLT_CHARWHOLELINK:
				case ESLT_SRC_RAD:
				case ESLT_CPOS_ICAMERA:
					{
						for( int j = 0; j < pEffectSet->m_iDrawParCount; j++ )
						{
							StopDisplay( pEffectSet->m_ParticleInfo[j].index,
								m_pEffSetCon[i].m_lTargetUniq, m_pEffSetCon[i].m_iTargetIndex,
								-1, i );
						}
						for(int j = 0; j < pEffectSet->m_iDrawBillCount; j++ )
						{
							g_Billboard.DelManager( pEffectSet->m_BillboardInfo[j].index,
								m_pEffSetCon[i].m_lTargetUniq, m_pEffSetCon[i].m_iTargetIndex, i );
						}
					}
					break;
					
				case ESLT_POS:
					{	
						D3DXVECTOR3 vTempPos;
						
						for( int j = 0; j < pEffectSet->m_iDrawParCount; j++ )
						{
							vTempPos = m_pEffSetCon[i].m_vPos + pEffectSet->m_ParticleInfo[j].vPos;
							
							StopDisplay( pEffectSet->m_ParticleInfo[j].index,
								vTempPos , i );
						}
						for(int j = 0; j < pEffectSet->m_iDrawBillCount; j++ )
						{
							vTempPos = m_pEffSetCon[i].m_vPos + pEffectSet->m_BillboardInfo[j].vPos;
							
							g_Billboard.DelManager( pEffectSet->m_BillboardInfo[j].index,
								vTempPos , i );
						}
					}
					
					break;
				}
				for( int k = 0; k < m_effectobjcount; ++k )
				{	
					for( int l = 0; l < EFFECTOBJ_CHILDCOUNT; ++l )
					{
						if( m_EffectObj[k].m_pAniPos[l].m_iEffSetCon == i )
							DelEffectObj( k, l );					
					}
				}
				m_pEffSetCon[i].Init();
			}
		}
	}
}


//-----------------------------------------------------------------------------
// 이펙트 오브젝트 디스플레이
//-----------------------------------------------------------------------------]
void	CBravoParticle::DisplayEffectObj( float TimeD )
{	
	// 애니메이션 프레임이 마지막 프레임까지 했다면 삭제... 임시로...
	// 나중에는 스킬 속성에 따라서 처리해야 한다
	for( int i = 0; i < m_effectobjcount; ++i )
	{	
		for( int j = 0; j < EFFECTOBJ_CHILDCOUNT; ++j )
		{
			if( m_EffectObj[i].m_pAniPos[j].m_bIsVisible == true )
			{
				if( m_EffectObj[i].m_pAniPos[j].m_iEffSetCon != -1 )
				{
					D3DXMATRIX mat = m_EffectObj[i].m_pAniPos[j].GetPosTM();
					mat._41 = m_pEffSetCon[m_EffectObj[i].m_pAniPos[j].m_iEffSetCon].m_vPos.x;
					mat._42 = m_pEffSetCon[m_EffectObj[i].m_pAniPos[j].m_iEffSetCon].m_vPos.y;
					mat._43 = m_pEffSetCon[m_EffectObj[i].m_pAniPos[j].m_iEffSetCon].m_vPos.z;
					m_EffectObj[i].m_pAniPos[j].SetPosTM3( mat );
				}
				
				if( m_pEffSetCon[m_EffectObj[i].m_pAniPos[j].m_iEffSetCon].m_iType == 0 )
				{
					if( (DWORD)m_EffectObj[i].m_pAniPos[j].m_ani_frame == m_EffectObj[i].m_end_frame )				
					{
						DelEffectObj( i, j );
					}
				}
			}
		}		
	}
	
	// 렌더링
	g_RenderManager.SetFVF( D3DFVF_OBJECTVERTEX );	
	
	for(int i = 0; i < m_effectobjcount; ++i )
	{		
		// 자식 갯수만큼		
		for( int j = 0; j < EFFECTOBJ_CHILDCOUNT; ++j )
		{
			// 보여야지 그린다			
			if( m_EffectObj[i].m_pAniPos[j].m_bIsVisible == true )
			{
				m_EffectObj[i].EffectRender( j, 1.0f );						
			}			
		}		
	}
	
}

void	CBravoParticle::AddEffectObj( int index, int tuniq, int tidx, D3DXVECTOR3 pos,
									 float rad, int effsetcon )
{
	D3DXMATRIX matPos;
	D3DXMatrixIdentity(&matPos);
	
	int j = 0;
	for( int i = 0; i < m_effectobjcount; ++i )
	{
		if( m_EffectObj[i].m_dwIndex == (DWORD)index )
		{
			for( j = 0; j < EFFECTOBJ_CHILDCOUNT; ++j )
			{
				if( m_EffectObj[i].m_pAniPos[j].m_bIsVisible == false )
				{					
					m_EffectObj[i].m_pAniPos[j].Clear();
					m_EffectObj[i].m_pAniPos[j].m_bIsVisible = true;
					m_EffectObj[i].m_pAniPos[j].m_iEffSetCon	= effsetcon;
					if( pos.x != 0.0f || pos.y != 0.0f || pos.z != 0.0f )
					{
						D3DXMATRIX mat2;
						D3DXMATRIX mat3;
						// 방향으로 회전.
						D3DXMatrixRotationY( &mat2, rad );
						D3DXMatrixMultiply( &matPos, &mat2, &matPos );
						m_EffectObj[i].m_pAniPos[j].SetPosTM3( matPos );
					}
					else
					{
						D3DXMATRIX mat2;
						D3DXMATRIX mat3;
						// 방향으로 회전.
						D3DXMatrixRotationY( &mat2, rad );
						D3DXMatrixMultiply( &matPos, &mat2, &matPos );
						matPos._41 += g_Pc.GetPcInnerPos( tidx, tuniq, 0.0f).x;
						matPos._43 += g_Pc.GetPcInnerPos( tidx, tuniq, 0.0f).z;
						m_EffectObj[i].m_pAniPos[j].SetPosTM3( matPos );
					}				
					return;
				}
			}
			if( j == EFFECTOBJ_CHILDCOUNT )
				return;
		}
	}
}

//-----------------------------------------------------------------------------
// 이펙트 오브젝트 렌더링 루틴에서 삭제
// m_bIsVisible을 false로 바꾼다
									 //-----------------------------------------------------------------------------
void	CBravoParticle::DelEffectObj( int index, int child )
{	
	m_EffectObj[index].m_pAniPos[child].Clear();
	m_EffectObj[index].m_pAniPos[child].m_bIsVisible = false;
	memset( &m_EffectObj[index].m_pAniPos[child].m_Material.Diffuse, 
		1, sizeof(m_EffectObj[index].m_pAniPos[child].m_Material.Diffuse) );
}




//==============================================================================
//==============================================================================
// 이펙트 딜레이 루틴.. 
//==============================================================================
//==============================================================================

void	CBravoParticle::AddEffectDelay( int type, float life, int number, long uniq, int index, 
									   int linkindex, D3DXVECTOR3& loc, int effsetcon )
{
	for( int i = 0; i < 128; i++ )
	{
		// 비어 있는 놈을.
		if( EffectDelay[i].Type == -1 )
		{
			EffectDelay[i].Type = type;
			EffectDelay[i].Life = life;
			EffectDelay[i].Number = number;
			EffectDelay[i].Uniq = uniq;
			EffectDelay[i].Index = index;
			EffectDelay[i].LinkIndex = linkindex;
			EffectDelay[i].Loc = loc;
			EffectDelay[i].m_iEffSetCon = effsetcon;
			EffectDelay[i].vAttackSrcDir = m_pEffSetCon[ effsetcon ].m_vDir;
			EffectDelay[i].m_pAttackUnit = m_pEffSetCon[ effsetcon ].m_pAttackUnit;
			EffectDelay[i].m_IsDirAttatack = m_pEffSetCon[ effsetcon ].m_IsDirAttatack;
			EffectDelay[i].m_IsGUnitLinkBoneAndSizeFix = m_pEffSetCon[ effsetcon ].m_IsGUnitLinkBoneAndSizeFix;
			EffectDelay[i].m_aDirectLinkBone = m_pEffSetCon[ effsetcon ].m_aDirectLinkBone;
			break;
		}
	}
}

void	CBravoParticle::DelEffectDelay( int num )
{
	EffectDelay[ num ].Init();
}

void	CBravoParticle::EffectDelayProcess( float time )
{
	for( int i = 0; i < 128; i++ )
	{
		// 비어 있는 놈을.
		if( EffectDelay[i].Type != -1 )
		{
			EffectDelay[i].Life -= time;
			
			if( EffectDelay[i].Life < 0.0f )
			{
				e_SCENE_MODE SaveMode = g_RockClient.m_SceneManager.GetEffectCreateMode();
				
				g_RockClient.m_SceneManager.SetEffectCreateMode( m_pEffSetCon[ EffectDelay[i].m_iEffSetCon ].m_SceneMode );
				
				if( EffectDelay[i].Type == 0 )
				{
					//					DisplayUpdate( EffectDelay[i].Number );
				}
				else if( EffectDelay[i].Type == 1 )
				{
					DisplayUpdate( EffectDelay[i].Number, EffectDelay[i].Uniq );
				}
				else if( EffectDelay[i].Type == 2 )
				{
					DisplayUpdate( EffectDelay[i].Number, EffectDelay[i].Loc, EffectDelay[i].m_iEffSetCon );
				}
				else if( EffectDelay[i].Type == 3 )
				{
					DisplayUpdate( EffectDelay[i].Number, EffectDelay[i].Uniq, EffectDelay[i].Loc );
				}
				else if( EffectDelay[i].Type == 5 ) ///-- ESLT_CHARPOS, ESLT_CHARWHOLELINK, ESLT_CPOS_ICAMERA
				{
					DisplayUpdate( EffectDelay[i].Number, EffectDelay[i].Uniq, EffectDelay[i].Index,
						EffectDelay[i].LinkIndex, EffectDelay[i].Loc, false, EffectDelay[i].m_iEffSetCon, 
						EffectDelay[i].vAttackSrcDir, EffectDelay[i].m_pAttackUnit,
						EffectDelay[i].m_IsDirAttatack, EffectDelay[i].m_IsGUnitLinkBoneAndSizeFix,
						EffectDelay[i].m_aDirectLinkBone );
				}
				else if( EffectDelay[i].Type == 6 )
				{
					DisplayUpdate( EffectDelay[i].Number, EffectDelay[i].Uniq, EffectDelay[i].Index,
						EffectDelay[i].LinkIndex, EffectDelay[i].Loc , EffectDelay[i].m_iEffSetCon,  
						EffectDelay[i].vAttackSrcDir, EffectDelay[i].m_pAttackUnit, 
						EffectDelay[i].m_IsDirAttatack, EffectDelay[i].m_IsGUnitLinkBoneAndSizeFix,
						EffectDelay[i].m_aDirectLinkBone );
				}
				
				g_RockClient.m_SceneManager.SetEffectCreateMode( SaveMode );
				
				DelEffectDelay( i );
			}
		}
	}
}



//==============================================================================
//==============================================================================
// 제한시간 있는 이펙트 들..
//==============================================================================
//==============================================================================

void	CBravoParticle::AddEffectStop( int type, float life, int number, long uniq, int index,
									  int linkindex, D3DXVECTOR3& loc, int effsetcon )
{
	for( int i = 0; i < 128; i++ )
	{
		// 비어 있는 놈을.
		if( EffectStop[i].Type == -1 )
		{
			EffectStop[i].Type = type;
			EffectStop[i].Life = life;
			EffectStop[i].Number = number;
			EffectStop[i].Uniq = uniq;
			EffectStop[i].Index = index;
			EffectStop[i].LinkIndex = linkindex;
			EffectStop[i].Loc = loc;
			EffectStop[i].m_iEffSetCon = effsetcon;
			break;
		}
	}
}
void	CBravoParticle::DelEffectStop( int num )
{
	// 안쓴다고 해놈.
	EffectStop[num].Type = -1;
}
void	CBravoParticle::EffectStopProcess( float time )
{
	for( int i = 0; i < 128; i++ )
	{
		// 비어 있는 놈을.
		if( EffectStop[i].Type != -1 )
		{
			EffectStop[i].Life -= time;
			if( EffectStop[i].Life < 0.0f )
			{
				if( EffectStop[i].Type == 0 )
				{
					StopDisplay( EffectStop[i].Number, EffectStop[i].Uniq );
				}
				else if( EffectStop[i].Type == 1 )
				{
					StopDisplay( EffectStop[i].Number, EffectStop[i].Loc, EffectStop[i].m_iEffSetCon );
				}
				else if( EffectStop[i].Type == 2 )
				{
					StopDisplay( EffectStop[i].Number, EffectStop[i].Uniq, EffectStop[i].LinkIndex );
				}
				else if( EffectStop[i].Type == 5 || EffectStop[i].Type == 6 )
				{
					StopDisplay( EffectStop[i].Number, EffectStop[i].Uniq, EffectStop[i].Index,
						EffectStop[i].LinkIndex, EffectStop[i].m_iEffSetCon );
				}
				DelEffectStop( i );
			}
		}
	}
}



//==============================================================================
//==============================================================================
// 이펙트 OBJ딜레이 루틴.. 
//==============================================================================
//==============================================================================



void	CBravoParticle::AddEffectObjDelay( float life, int index, D3DXMATRIX mat, D3DXMATRIX pcuniqmat,
										  bool child, int pcuniq, int pcindex, int effsetcon )
{
	for( int i = 0; i < 128; i++ )
	{
		// 비어 있는 놈을.
		if( !EffectObjDelay[i].Use )
		{
			EffectObjDelay[i].Use = true;
			EffectObjDelay[i].Life = life;
			EffectObjDelay[i].Index = index;
			EffectObjDelay[i].MatPos = mat;
			EffectObjDelay[i].PcUniqMat = pcuniqmat;
			EffectObjDelay[i].Child = child;
			EffectObjDelay[i].m_iPcUniq = pcuniq;
			EffectObjDelay[i].m_iPcIdx = pcindex;
			EffectObjDelay[i].m_iEffSetCon = effsetcon;
			break;
		}
	}
}
void	CBravoParticle::DelEffectObjDelay( int num )
{
	EffectObjDelay[num].Use = false;
}
void	CBravoParticle::EffectObjDelayProcess( float time )
{
	for( int i = 0; i < 128; i++ )
	{
		// 비어 있는 놈을.
		if( EffectObjDelay[i].Use )
		{
			EffectObjDelay[i].Life -= time;
			if( EffectObjDelay[i].Life < 0.0f )
			{
				DelEffectObjDelay( i );
			}
		}
	}
}



float	CBravoParticle::VectorToDir(D3DXVECTOR3& v)
{
	if( v.z == 0 ){ v.z = 0.01f; }
	
	float	Dir = 0.0f;
	double	Theta = v.x/v.z;
	
	if( v.x > 0 && v.z > 0)
	{
		Dir = (float)atan(Theta) - 3.14159f;	
	}
	else if( v.x > 0 && v.z < 0)
	{
		Dir = (float)atan(Theta);
	}
	else if( v.x < 0 && v.z > 0)
	{
		Dir = (float)atan(Theta) + 3.14159f;
	}
	else if( v.x < 0 && v.z < 0)
	{
		Dir = (float)atan(Theta);
	}
	else if( v.x == 0 )
	{
		Dir = (float)atan(Theta);
	}
	
	return	Dir;
}

HRESULT	CAnimManager::InitManager(char* dir)
{
	char FileName[ 1024 ] = "";
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined( LOAD_VF ) /////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	wsprintf( FileName , "%s\\Effect\\Tex\\Ani\\*.mmt" , dir );
	
	WIN32_FIND_DATA	 fd;
	
	HANDLE hFindFile = FindFirstFile( FileName , &fd );
	
	if( hFindFile == INVALID_HANDLE_VALUE )	return FALSE;
	
	int Find	= TRUE;
	
	int Index = 0;
	
	CAnimSequence	*pData = NULL;
	
	while( Find != FALSE )
	{			
		Index = GetIndexKeyInFileName( fd.cFileName );
		
		if( Index == -1 )
			continue;
		
		pData = SAFE_NEW( CAnimSequence );
		
		g_Anim.m_AniData.Insert( Index , pData );
		
		pData->LoadAni( fd.cFileName );
		
		Find = FindNextFile( hFindFile, &fd );
	}					
	
	FindClose( hFindFile );
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#else ///////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	VFileHandleList* pFHL;
	VFileHandleList_i it;
	
	CAnimSequence	*pData = NULL;
	
	char buf[256] = {0,};
	int Index = 0;
	
	pFHL = g_VFEffectTexAni.GetFileHandleList();
	for( it = pFHL->begin(); it != pFHL->end(); ++it )
	{
		if( (*it)->GetFileContext()->iType == FT_FILE )
		{
			GetFileExt( (*it)->GetFileContext()->strName, buf );
			
			if( lstrcmp( buf, "mmt" ) == 0 || lstrcmp( buf, "MMT" ) == 0 )	
			{
				Index = GetIndexKeyInFileName( (*it)->GetFileContext()->strName );
				if( Index == -1 )
					continue;
				
				pData = SAFE_NEW( CAnimSequence );
				
				g_Anim.m_AniData.Insert( Index , pData );
				
				pData->LoadAni( (*it)->GetFileContext()->strName );		
			}
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif //////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	return TRUE;
}


void	CBravoParticle::AddDropItemEffect( int index, long pos )
{
	D3DXVECTOR3 v;
	v.x = (pos % (g_Map.GetMapSize()*2)) * 8.0f + 4.0f;
	v.y = g_Map.Get__MapHeight(pos % (g_Map.GetMapSize()*2), pos / (g_Map.GetMapSize()*2));
	v.z = (pos / (g_Map.GetMapSize()*2)) * 8.0f + 4.0f;
	//힐링 포션
	if( index >= 268475648 && index <= 268475651 )
	{
		g_Particle.DisplayUpdate( 19, v );
		g_Particle.AddEffectStop( 1, 1.0f, 19, -1, -1, -1, v );
	}
	else if( index >= 268475652 && index <= 268475655 )
	{
		g_Particle.DisplayUpdate( 19, v );
		g_Particle.AddEffectStop( 1, 1.0f, 19, -1, -1, -1, v );
	}
}

//..................................................................................................................
// 이펙트 셋 초기 설정 By wxywxy
//..................................................................................................................
void CBravoParticle::Setup_EffectSet(int i)
{
	CEffectSet *pEffectSet = NULL;
	
	pEffectSet = m_EffectSet.GetKeyDataPointer( m_pEffSetCon[i].m_iSetType );
	
	if( pEffectSet == NULL ) 
		return;
	
	m_pEffSetCon[i].m_bMake = true;
	m_pEffSetCon[i].m_pSrc  = NULL;
	
	e_SCENE_MODE SaveMode = g_RockClient.m_SceneManager.GetEffectCreateMode();
				
	g_RockClient.m_SceneManager.SetEffectCreateMode( m_pEffSetCon[i].m_SceneMode );
	
	//..............................................................................................................
	// 이펙트 종류에 따라 설정 초기화
	//..............................................................................................................
	switch( m_pEffSetCon[i].m_iType )
	{
		//..........................................................................................................
		// 캐릭터 자리에서 생성
		//..........................................................................................................
	case ESLT_CHARPOS:
	case ESLT_CHARWHOLELINK:
	case ESLT_CPOS_ICAMERA:
		{
			m_pEffSetCon[i].m_pSrc = g_Particle.GetCharacter( m_pEffSetCon[i].m_lTargetUniq , m_pEffSetCon[i].m_iTargetIndex );
			
			if( m_pEffSetCon[i].m_pSrc == NULL )
			{
				Stop_EffectSet( i );
				
				g_RockClient.m_SceneManager.SetEffectCreateMode( SaveMode );
				
				return;
			}
			else
			{
				if( m_pEffSetCon[i].m_pSrc->m_nTableIndex == -1 )
				{
					Stop_EffectSet( i );
					
					g_RockClient.m_SceneManager.SetEffectCreateMode( SaveMode );
					
					return;
				}
			}			
			
			D3DXVECTOR3 tempv = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
			
			D3DXVECTOR3 vTempPos;
			
			for( int j = 0; j < pEffectSet->m_iDrawParCount; j++ )
			{
				vTempPos = pEffectSet->m_ParticleInfo[j].vPos;				
				
				if( !ISZERO( m_pEffSetCon[i].m_fRad ) )
				{
					D3DXMATRIX matRotY;			
					D3DXMatrixRotationY( &matRotY , m_pEffSetCon[i].m_fRad );		
					D3DXVec3TransformCoord( &vTempPos , &vTempPos , &matRotY );
				}
				
				if( !ISZERO( m_pEffSetCon[i].m_fAddSize ) )
				{
					vTempPos += ( m_pEffSetCon[i].m_fAddSize ) * vTempPos;
				}
				
				vTempPos += m_pEffSetCon[i].m_vPos;
				
				AddEffectDelay( 5 ,
					pEffectSet->m_ParticleInfo[j].delayTime,
					pEffectSet->m_ParticleInfo[j].index,
					m_pEffSetCon[i].m_lTargetUniq, 
					m_pEffSetCon[i].m_iTargetIndex,
					pEffectSet->m_ParticleInfo[ j ].BoneLink ,
					vTempPos ,
					i );
			}
			
			for(int j = 0; j < pEffectSet->m_iDrawBillCount; j++ )
			{
				BYTE aLinkBone = pEffectSet->m_BillboardInfo[ j ].BoneLink;	
				
				vTempPos = pEffectSet->m_BillboardInfo[j].vPos;
				
				if(!ISZERO(m_pEffSetCon[i].m_fRad))
				{
					D3DXMATRIX matRotY;			
					D3DXMatrixRotationY( &matRotY , m_pEffSetCon[i].m_fRad );		
					D3DXVec3TransformCoord( &vTempPos , &vTempPos , &matRotY );
				}

				if( !ISZERO( m_pEffSetCon[i].m_fAddSize ) )
				{
					vTempPos += ( ( m_pEffSetCon[i].m_fAddSize ) * vTempPos );
				}
				
				vTempPos += m_pEffSetCon[i].m_vPos;
				
				g_Billboard.SetManager( 
					pEffectSet->m_BillboardInfo[j].delayTime ,
					pEffectSet->m_BillboardInfo[j].index,
					m_pEffSetCon[i].m_lTargetUniq, 
					m_pEffSetCon[i].m_iTargetIndex,
					vTempPos ,
					pEffectSet->m_BillboardInfo[ j ].BoneLink ,
					i );
				
			}
			
			for(int j = 0; j < pEffectSet->m_iDrawObjCount; j++ )
			{
				AddEffectObj( pEffectSet->m_ObjInfo[j].index,
					m_pEffSetCon[i].m_lTargetUniq, m_pEffSetCon[i].m_iTargetIndex,
					tempv, m_pEffSetCon[i].m_fRad, i );
			}
		}
		break;
		
		//..........................................................................................................
		// 입력된 자리에서 처리
		//..........................................................................................................
	case ESLT_POS:
		{
			D3DXVECTOR3 vTempPos;
			
			for( int j = 0; j < pEffectSet->m_iDrawParCount; j++ )
			{
				vTempPos = pEffectSet->m_ParticleInfo[j].vPos;				
				
				if( !ISZERO( m_pEffSetCon[i].m_fRad ) )
				{
					D3DXMATRIX matRotY;			
					D3DXMatrixRotationY( &matRotY , m_pEffSetCon[i].m_fRad );		
					D3DXVec3TransformCoord( &vTempPos , &vTempPos , &matRotY );
				}
				
				if( !ISZERO( m_pEffSetCon[i].m_fAddSize ) )
				{
					vTempPos += ( ( m_pEffSetCon[i].m_fAddSize ) * vTempPos );
				}
				
				vTempPos += m_pEffSetCon[i].m_vPos;
				
				AddEffectDelay( 2, 
					pEffectSet->m_ParticleInfo[j].delayTime,
					pEffectSet->m_ParticleInfo[j].index,
					-1, -1,
					-1, vTempPos , i );
			}			
			
			for( int j = 0; j < pEffectSet->m_iDrawBillCount; j++ )
			{
				vTempPos = pEffectSet->m_BillboardInfo[j].vPos;
				
				if(!ISZERO(m_pEffSetCon[i].m_fRad))
				{
					D3DXMATRIX matRotY;			
					D3DXMatrixRotationY( &matRotY , m_pEffSetCon[i].m_fRad );		
					D3DXVec3TransformCoord( &vTempPos , &vTempPos , &matRotY );
				}

				if( !ISZERO( m_pEffSetCon[i].m_fAddSize ) )
				{
					vTempPos += ( ( m_pEffSetCon[i].m_fAddSize ) * vTempPos );
				}
				
				vTempPos += m_pEffSetCon[i].m_vPos;
				
				g_Billboard.SetManager( pEffectSet->m_BillboardInfo[j].delayTime ,
					pEffectSet->m_BillboardInfo[j].index, vTempPos , i, m_pEffSetCon[i].m_nHeight );
			}
			
			for(int j = 0; j < pEffectSet->m_iDrawObjCount; j++ )
			{
				AddEffectObj( pEffectSet->m_ObjInfo[j].index,
					-1, -1,	m_pEffSetCon[i].m_vPos, m_pEffSetCon[i].m_fRad, i );
			}
			
		}
		break;
		
		
		//..........................................................................................................
		// 입력된 자리에서 처리
		//..........................................................................................................
	case ESLT_SRC_RAD:
		{
			m_pEffSetCon[i].m_pSrc = g_Particle.GetCharacter( m_pEffSetCon[i].m_lTargetUniq , m_pEffSetCon[i].m_iTargetIndex );
			
			if( m_pEffSetCon[i].m_pSrc == NULL )
			{
				Stop_EffectSet( i );
				
				g_RockClient.m_SceneManager.SetEffectCreateMode( SaveMode );
				
				return;
			}
			else
			{
				if( m_pEffSetCon[i].m_pSrc->m_nTableIndex == -1 )
				{
					Stop_EffectSet( i );					
					g_RockClient.m_SceneManager.SetEffectCreateMode( SaveMode );					
					return;
				}
			}
			
			D3DXVECTOR3 vTempPos;
			
			for( int j = 0; j < pEffectSet->m_iDrawParCount; j++ )
			{
				vTempPos = m_pEffSetCon[i].m_vPos + pEffectSet->m_ParticleInfo[j].vPos;
				
				AddEffectDelay( 6 ,
					pEffectSet->m_ParticleInfo[j].delayTime,
					pEffectSet->m_ParticleInfo[j].index,
					m_pEffSetCon[i].m_lTargetUniq, 
					m_pEffSetCon[i].m_iTargetIndex,
					pEffectSet->m_ParticleInfo[ j ].BoneLink ,
					vTempPos,
					i );
			}
			
			for(int j = 0; j < pEffectSet->m_iDrawBillCount; j++ )
			{
				vTempPos = m_pEffSetCon[i].m_vPos + pEffectSet->m_BillboardInfo[j].vPos;
				
				g_Billboard.SetManager( 
					pEffectSet->m_BillboardInfo[j].delayTime ,
					pEffectSet->m_BillboardInfo[j].index,
					m_pEffSetCon[i].m_lTargetUniq, 
					m_pEffSetCon[i].m_iTargetIndex,
					vTempPos ,
					pEffectSet->m_BillboardInfo[ j ].BoneLink ,
					i );
				
			}
			
			for(int j = 0; j < pEffectSet->m_iDrawObjCount; j++ )
			{
				AddEffectObj( pEffectSet->m_ObjInfo[j].index,
					m_pEffSetCon[i].m_lTargetUniq, m_pEffSetCon[i].m_iTargetIndex,
					vTempPos, m_pEffSetCon[i].m_fRad, i );
			}
			
		}
		break;
	}
	
}

//..................................................................................................................
// 이펙트 처리		   By wxywxy
//..................................................................................................................
void CBravoParticle::Process_EffectSet(int i,float fTimeDelta)
{
	//..............................................................................................................
	// 이펙트 타입 별로 처리
	//..............................................................................................................
	switch( m_pEffSetCon[i].m_iType )
	{
		//..........................................................................................................
		// 캐릭터 현재위치에서 처리
		//..........................................................................................................
	case ESLT_CHARPOS:
	case ESLT_CHARWHOLELINK:
	case ESLT_CPOS_ICAMERA:
		{
			if( m_pEffSetCon[i].m_pSrc != NULL )
			{
				if( m_pEffSetCon[i].m_pSrc->m_nTableIndex != -1 )
				{
					if(!m_pEffSetCon[i].m_ConSkill)
					{
						m_pEffSetCon[i].m_vPos.x = m_pEffSetCon[i].m_pSrc->GetBonCenterTM()._41;
						m_pEffSetCon[i].m_vPos.y = m_pEffSetCon[i].m_pSrc->GetBonCenterTM()._42;
						m_pEffSetCon[i].m_vPos.z = m_pEffSetCon[i].m_pSrc->GetBonCenterTM()._43;
					}
					else
					{
						m_pEffSetCon[i].m_vPos.x = m_pEffSetCon[i].m_pSrc->GetBonCenterTM()._41;
						m_pEffSetCon[i].m_vPos.y = m_pEffSetCon[i].m_pSrc->GetBonCenterTM()._42;
						m_pEffSetCon[i].m_vPos.z = m_pEffSetCon[i].m_pSrc->GetBonCenterTM()._43;
					}
				}
				else
				{
					Stop_EffectSet( i );
				}
			}
			else
			{
				Stop_EffectSet( i );
			}
		}
		break;
		
		//..........................................................................................................
		// 시작위치 그대로 유지
		//..........................................................................................................
	case ESLT_POS:
	case ESLT_SRC_RAD:
		{
		}
		break;
	}
	
	//..............................................................................................................
	// 이펙트 라이프 타임 줄이기
	//..............................................................................................................
	if( m_pEffSetCon[i].m_fLife < EFFECT_SET_LOOP_TIME )
		m_pEffSetCon[i].m_fLife -= fTimeDelta;
	
}

//..................................................................................................................
// 이펙트 종료		   By wxywxy
//..................................................................................................................
void CBravoParticle::Stop_EffectSet(int i)
{
	CEffectSet	*pEffectSet = NULL;
	
	pEffectSet = m_EffectSet.GetKeyDataPointer( m_pEffSetCon[i].m_iSetType );
	
	if( pEffectSet == NULL ) return;
	
	switch( m_pEffSetCon[i].m_iType )
	{
		//..........................................................................................................
		// 꼬리 없는것
		//..........................................................................................................
	case ESLT_CHARPOS:
	case ESLT_CHARWHOLELINK:
	case ESLT_POS:
	case ESLT_SRC_RAD:
	case ESLT_CPOS_ICAMERA:	
		{
			for( int j = 0; j < pEffectSet->m_iDrawParCount; j++ )
			{
				StopDisplay( pEffectSet->m_ParticleInfo[j].index,
					m_pEffSetCon[i].m_lTargetUniq, m_pEffSetCon[i].m_iTargetIndex,
					-1, i );
			}
			for(int j = 0; j < pEffectSet->m_iDrawBillCount; j++ )
			{
				g_Billboard.DelManager( pEffectSet->m_BillboardInfo[j].index,
					m_pEffSetCon[i].m_lTargetUniq, m_pEffSetCon[i].m_iTargetIndex, i );
			}
		}
		break;
	}
	
	// 임시 By wxywxy
	for( int k = 0; k < m_effectobjcount; ++k )
	{	
		for( int l = 0; l < EFFECTOBJ_CHILDCOUNT; ++l )
		{
			if( m_pEffSetCon[m_EffectObj[k].m_pAniPos[l].m_iEffSetCon].m_iType != 0 )
			{
				if( m_EffectObj[k].m_pAniPos[l].m_iEffSetCon == i )
					DelEffectObj( k, l );
			}
		}
	}
	
	m_pEffSetCon[i].Init();
}

//..................................................................................................................
// 카메라 흔들림 By wxywxy
//..................................................................................................................
void CBravoParticle::RockCamera(DWORD Timer)
{
	m_CameraOldTimer  = timeGetTime();
	m_CameraRockTimer = Timer;
	m_ISCameraRock    = TRUE;
	
	m_fOldCameraYaw   = g_Camera.m_fCameraYaw;
	m_fOldCameraPitch = g_Camera.m_fCameraPitch;
}

//..................................................................................................................
// 카메라 흔들림 체크 by wxywxy
//..................................................................................................................
void CBravoParticle::RockCheck(void)
{
	if( !m_ISCameraRock )
		return;
	
	if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( timeGetTime() , m_CameraOldTimer ) , >= , m_CameraRockTimer ) )
	{
		g_Camera.m_fCameraYaw	= m_fOldCameraYaw; 
		g_Camera.m_fCameraPitch	= m_fOldCameraPitch;
		
		m_ISCameraRock = FALSE;
	}
	else
	{
		g_Camera.m_fCameraYaw	= m_fOldCameraYaw   + D3DXToRadian( RANDOM_NUM * ( rand()%3 ) );
		g_Camera.m_fCameraPitch	= m_fOldCameraPitch + D3DXToRadian( RANDOM_NUM * ( rand()%3 ) );
	}
	
	g_Camera.Update();
}

//..................................................................................................................
// 파티클 로딩 관련
//..................................................................................................................
int	CBravoParticle::LoadPaticle(char *pPath)
{
#if !defined( LOAD_VF ) /////////////////////////////////////////////////////////////////////////////////////
	
	char FileName[ 1024 ] = "";
	wsprintf( FileName , "%s\\Effect\\*.nb" , pPath );
	
	WIN32_FIND_DATA	 fd;
	
	HANDLE hFindFile = FindFirstFile( FileName , &fd );
	
	if( hFindFile == INVALID_HANDLE_VALUE )	return FALSE;
	
	int Find	= TRUE;
	
	int Index = 0;
	
	CParticleData	 TempData;
	CParticleData	*pData = NULL;
	
	while( Find != FALSE )
	{			
		Index = GetIndexKeyInFileName( fd.cFileName );
		
		if( Index == -1 )
			continue;
		
		pData = m_ParticleData.Insert( Index , TempData );
		
		pData->FileOpen( fd.cFileName );
		
		Find = FindNextFile( hFindFile, &fd );
	}					
	
	FindClose( hFindFile );
	
#else ///////////////////////////////////////////////////////////////////////////////////////////////////////
	
	CParticleData	 TempData;
	CParticleData	*pData = NULL;
	
	char buf[256] = {0,};
	int Index = 0;
	
	VFileHandleList* pFHL = g_VFEffectRoot.GetFileHandleList();
	for(VFileHandleList_i it = pFHL->begin(); it != pFHL->end(); ++it )
	{
		if( (*it)->GetFileContext()->iType != FT_FILE )
			continue;

		GetFileExt( (*it)->GetFileContext()->strName, buf );// 확장자 가져오기
		if( lstrcmp( buf, "nb" ) == 0 )	// .nb라면
		{
			Index = GetIndexKeyInFileName( (*it)->GetFileContext()->strName );// 파일명으로부터 _이후에 인덱스 값을 읽는다
			if( Index == -1 )
				continue;
			
			pData = m_ParticleData.Insert( Index , TempData );// 파일명으로 얻어온 인덱스와 임시 데이터로 공간을 확보
			pData->FileOpen( (*it)->GetFileContext()->strName );// 실제 데이터를 입력
		}
	}
	
#endif //////////////////////////////////////////////////////////////////////////////////////////////////////
	
	return TRUE;
}


//..................................................................................................................
// 이펙트 셋 로딩 관련
//..................................................................................................................
int	CBravoParticle::LoadEffectSet(void)
{
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined( LOAD_VF ) /////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	char FileName[ 1024 ] = "";
	
	wsprintf( FileName , "%s\\Effect\\*.eff" , g_RockClient.GetRootDir() );
	
	WIN32_FIND_DATA	 fd;
	
	HANDLE hFindFile = FindFirstFile( FileName , &fd );
	
	if( hFindFile == INVALID_HANDLE_VALUE )	return FALSE;
	
	int Find	= TRUE;
	
	int Index = 0;
	
	CEffectSet	 TempData;
	CEffectSet	*pData = NULL;
	
	while( Find != FALSE )
	{			
		Index = GetIndexKeyInFileName( fd.cFileName );
		
		if( Index == -1 )
			continue;
		
		pData = m_EffectSet.Insert( Index , TempData );
		
		pData->LoadEffectSet( fd.cFileName );
		
		Find = FindNextFile( hFindFile, &fd );
	}					
	
	FindClose( hFindFile );
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#else ///////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	VFileHandleList* pFHL;
	VFileHandleList_i it;
	
	CEffectSet	 TempData;
	CEffectSet	*pData = NULL;
	
	char buf[256] = {0,};
	int Index = 0;

	string strTempName = "";
	string strConList[4] = { "ar_!_1111.eff" , "ar_!_1112.eff" , "ar_!_1113.eff", "ar_!_1114.eff" };
	
	pFHL = g_VFEffectRoot.GetFileHandleList();
	for( it = pFHL->begin(); it != pFHL->end(); ++it )
	{
		if( (*it)->GetFileContext()->iType == FT_FILE )
		{
			string steTempName = (*it)->GetFileContext()->strName;
			
			BOOL IsFind = FALSE;
			
			for( int i = 0 ; i < 4 ; i++ )
			{
				if( strConList[i] == steTempName )
				{
					IsFind = TRUE;
					break;
				}
			}
			
			if(IsFind)
			{
				continue;
			}
			
			GetFileExt( (*it)->GetFileContext()->strName, buf );
			
			if( lstrcmp( buf, "eff" ) == 0 )	
			{
				Index = GetIndexKeyInFileName( (*it)->GetFileContext()->strName );
				if( Index == -1 )
					continue;
				
				pData = m_EffectSet.GetKeyDataPointer( Index );
				
				pData = m_EffectSet.Insert( Index , TempData );
				
				pData->LoadEffectSet( (*it)->GetFileContext()->strName );				
			}
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif //////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	return TRUE;
}

void CBillboardManager::SetDefaultVectex()
{
	for(int i = 0; i < eBDType_Count; i++)
	{
		if(m_pVerticesCount[i] == 0)
		{
			m_pIndices[i]				= NULL;
			m_pVertices[i]				= NULL;
			m_pDefaultVertices[i]		= NULL;
			m_pDefaultCopyVertices[i]	= NULL;
			continue;
		}
		m_pDefaultVertices[i]		= SAFE_NEW_ARRAY( VERTEX_XYZ_DIFFUSE_TEX1, m_pVerticesCount[i] );
		m_pDefaultCopyVertices[i]	= SAFE_NEW_ARRAY( VERTEX_XYZ_DIFFUSE_TEX1, m_pVerticesCount[i] );
		memcpy( m_pDefaultVertices[i], m_pVertices[i], sizeof(VERTEX_XYZ_DIFFUSE_TEX1) * m_pVerticesCount[i]);
	}
}

VERTEX_XYZ_DIFFUSE_TEX1* CBillboardManager::GetDefaultVectexByType(int iType)
{
	if(m_pVerticesCount[iType] == 0)
			return NULL;
	memcpy( m_pDefaultCopyVertices[iType], m_pDefaultVertices[iType], sizeof(VERTEX_XYZ_DIFFUSE_TEX1) * m_pVerticesCount[iType]);
	return m_pDefaultCopyVertices[iType];
}

//................................................................................................................
// 0. 실드
//................................................................................................................
int	CBillboardManager::MakeShield(void)
{
	if( FAILED(g_RenderManager.CreateIndexBuffer( 12*sizeof(WORD), 
		D3DUSAGE_WRITEONLY, D3DFMT_INDEX16,
		D3DPOOL_MANAGED, &m_pIB[0] ) ) )
	{
		return	E_FAIL;
	}

#ifdef BATCH_RENDER
	m_pVerticesCount[eBDType_Shield]	= 6;
	m_pIndicesCount[eBDType_Shield]	= 12;
	
	m_pVertices[eBDType_Shield]	= SAFE_NEW_ARRAY( VERTEX_XYZ_DIFFUSE_TEX1, m_pVerticesCount[eBDType_Shield] );
	m_pIndices[eBDType_Shield]	= SAFE_NEW_ARRAY( WORD , m_pIndicesCount[eBDType_Shield] );
#endif	//	BATCH_RENDER
	WORD	*pIndices;
#ifdef DIRECT_VERSION_9_MJH
	if ( FAILED ( m_pIB[0]->Lock(0, 12*sizeof(WORD), (void**)&pIndices, 0) ) )
#else
	if ( FAILED ( m_pIB[0]->Lock(0, 12*sizeof(WORD), (BYTE**)&pIndices, 0) ) )
#endif // DIRECT_VERSION_9_MJH
		return	E_FAIL;
	pIndices[0] = 0;
	pIndices[1] = 1;
	pIndices[2] = 2;
	pIndices[3] = 2;
	pIndices[4] = 1;
	pIndices[5] = 3;
	pIndices[6] = 2;
	pIndices[7] = 3;
	pIndices[8] = 4;
	pIndices[9] = 4;
	pIndices[10] = 3;
	pIndices[11] = 5;
	
#ifdef BATCH_RENDER
	memcpy( m_pIndices[eBDType_Shield], pIndices, sizeof(WORD) * m_pIndicesCount[eBDType_Shield]);
#endif	//	BATCH_RENDER

	m_pIB[0]->Unlock();
	
#ifdef BATCH_RENDER
	if( FAILED(g_RenderManager.CreateVertexBuffer( 6 * sizeof(VERTEX_XYZ_DIFFUSE_TEX1),
		D3DUSAGE_WRITEONLY, D3DFVF_VERTEX_XYZ_DIFFUSE_TEX1,
		D3DPOOL_MANAGED, &m_pVB[eBDType_Shield] ) ) )
	{
		return E_FAIL;
	}
	VERTEX_XYZ_DIFFUSE_TEX1* pVertices;
#else	//	 BATCH_RENDER
	if( FAILED(g_RenderManager.CreateVertexBuffer( 6 * sizeof(BILLBOARDVERTEX),
		D3DUSAGE_WRITEONLY, D3DFVF_BILLBOARDVERTEX,
		D3DPOOL_MANAGED, &m_pVB[0] ) ) )
	{
		return E_FAIL;
	}
	
	BILLBOARDVERTEX* pVertices;
#endif	//	BATCH_RENDER
#ifdef DIRECT_VERSION_9_MJH
	if( FAILED( m_pVB[0]->Lock( 0, 0, (void**)&pVertices, 0 ) ) )
#else
	if( FAILED( m_pVB[0]->Lock( 0, 0, (BYTE**)&pVertices, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH
		return E_FAIL;
	
	pVertices[0].v	=	D3DXVECTOR3(-0.5f, -0.5f, 0.0f);
	pVertices[1].v	=	D3DXVECTOR3(-0.5f, 0.5f, 0.0f);
	pVertices[2].v	=	D3DXVECTOR3(0.0f, -0.5f, -0.2f);
	pVertices[3].v	=	D3DXVECTOR3(0.0f, 0.5f, -0.2f);
	pVertices[4].v	=	D3DXVECTOR3(0.5f, -0.5f, 0.0f);
	pVertices[5].v	=	D3DXVECTOR3(0.5f, 0.5f, 0.0f);
	
	pVertices[0].tu	=	0.0f;
	pVertices[0].tv	=	1.0f;
	pVertices[1].tu	=	0.0f;
	pVertices[1].tv	=	0.0f;
	pVertices[2].tu	=	0.5f;
	pVertices[2].tv	=	1.0f;
	pVertices[3].tu	=	0.5f;
	pVertices[3].tv	=	0.0f;
	pVertices[4].tu	=	1.0f;
	pVertices[4].tv	=	1.0f;
	pVertices[5].tu	=	1.0f;
	pVertices[5].tv	=	0.0f;
	
#ifdef BATCH_RENDER
	memcpy( m_pVertices[eBDType_Shield], pVertices, 
		sizeof(VERTEX_XYZ_DIFFUSE_TEX1) * m_pVerticesCount[eBDType_Shield]);
#endif	//	BATCH_RENDER

	m_pVB[0]->Unlock();
	
	return TRUE;
}

//................................................................................................................
// 1. 세로 방향 면
//................................................................................................................
int	CBillboardManager::MakeVerticalPlane(void)
{
	if( FAILED(g_RenderManager.CreateIndexBuffer( 6*sizeof(WORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16,
		D3DPOOL_MANAGED, &m_pIB[1])))
	{
		return	E_FAIL;
	}

#ifdef BATCH_RENDER
	m_pIndicesCount[eBDType_VerticalPlane]	= 6;
	m_pVerticesCount[eBDType_VerticalPlane]	= 4;
	
	m_pIndices[eBDType_VerticalPlane]	
		= SAFE_NEW_ARRAY( WORD , m_pIndicesCount[eBDType_VerticalPlane] );
	m_pVertices[eBDType_VerticalPlane]	
		= SAFE_NEW_ARRAY( VERTEX_XYZ_DIFFUSE_TEX1, m_pVerticesCount[eBDType_VerticalPlane] );
#endif	//	BATCH_RENDER

	WORD	*pIndices;
#ifdef DIRECT_VERSION_9_MJH
	if ( FAILED ( m_pIB[1]->Lock(0, 6*sizeof(WORD), (void**)&pIndices, 0) ) )
#else
	if ( FAILED ( m_pIB[1]->Lock(0, 6*sizeof(WORD), (BYTE**)&pIndices, 0) ) )
#endif // DIRECT_VERSION_9_MJH
		return	E_FAIL;
	
	pIndices[0] = 0;
	pIndices[1] = 1;
	pIndices[2] = 3;
	pIndices[3] = 3;
	pIndices[4] = 1;
	pIndices[5] = 2;
	
#ifdef BATCH_RENDER
	memcpy( m_pIndices[eBDType_VerticalPlane], pIndices, sizeof(WORD) * m_pIndicesCount[eBDType_VerticalPlane]);
#endif	//	BATCH_RENDER

	m_pIB[1]->Unlock();
	
#ifdef BATCH_RENDER
	if( FAILED(g_RenderManager.CreateVertexBuffer( 4 * sizeof(VERTEX_XYZ_DIFFUSE_TEX1),
		D3DUSAGE_WRITEONLY, D3DFVF_VERTEX_XYZ_DIFFUSE_TEX1,
		D3DPOOL_MANAGED, &m_pVB[eBDType_VerticalPlane] ) ) )
	{
		return E_FAIL;
	}
	VERTEX_XYZ_DIFFUSE_TEX1* pVertices;
#else	//	 BATCH_RENDER
	if( FAILED(g_RenderManager.CreateVertexBuffer( 4*sizeof(BILLBOARDVERTEX), D3DUSAGE_WRITEONLY, D3DFVF_BILLBOARDVERTEX,
		D3DPOOL_MANAGED, &m_pVB[1] ) ) )
	{
		return E_FAIL;
	}
	
	BILLBOARDVERTEX* pVertices;
#endif	//	BATCH_RENDER
#ifdef DIRECT_VERSION_9_MJH
	if( FAILED( m_pVB[1]->Lock( 0, 0, (void**)&pVertices, 0 ) ) )
#else
	if( FAILED( m_pVB[1]->Lock( 0, 0, (BYTE**)&pVertices, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH
		return E_FAIL;
	
	pVertices[0].v	=	D3DXVECTOR3(-0.5f, -0.5f, 0.0f);
	pVertices[1].v	=	D3DXVECTOR3(-0.5f, 0.5f, 0.0f);
	pVertices[2].v	=	D3DXVECTOR3(0.5f, 0.5f, 0.0f);
	pVertices[3].v	=	D3DXVECTOR3(0.5f, -0.5f, 0.0f);
	pVertices[0].tu	=	0.0f;
	pVertices[0].tv	=	1.0f;
	pVertices[1].tu	=	0.0f;
	pVertices[1].tv	=	0.0f;
	pVertices[2].tu	=	1.0f;
	pVertices[2].tv	=	0.0f;
	pVertices[3].tu	=	1.0f;
	pVertices[3].tv	=	1.0f;
	
#ifdef BATCH_RENDER
	memcpy( m_pVertices[eBDType_VerticalPlane], pVertices, 
		sizeof(VERTEX_XYZ_DIFFUSE_TEX1) * m_pVerticesCount[eBDType_VerticalPlane]);
#endif	//	BATCH_RENDER

	m_pVB[1]->Unlock();
	
	return TRUE;
}

//................................................................................................................
// 2. 가로 방향 면
//................................................................................................................
int	CBillboardManager::MakeHorizonPlane(void)
{
	//인덱스 버퍼
	if( FAILED(g_RenderManager.CreateIndexBuffer( 6*sizeof(WORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16,
		D3DPOOL_MANAGED, &m_pIB[2]) ) )
	{
		return	E_FAIL;
	}
	
#ifdef BATCH_RENDER
	m_pIndicesCount[eBDType_HorizonPlane]	= 6;
	m_pVerticesCount[eBDType_HorizonPlane]	= 4;
		
	m_pIndices[eBDType_HorizonPlane]	= SAFE_NEW_ARRAY( WORD , m_pIndicesCount[eBDType_HorizonPlane] );
	m_pVertices[eBDType_HorizonPlane]	= SAFE_NEW_ARRAY( VERTEX_XYZ_DIFFUSE_TEX1, m_pVerticesCount[eBDType_HorizonPlane] );
#endif	//	BATCH_RENDER

	WORD	*pIndices;
#ifdef DIRECT_VERSION_9_MJH
	if ( FAILED ( m_pIB[2]->Lock(0, 6*sizeof(WORD), (void**)&pIndices, 0) ) )
#else
	if ( FAILED ( m_pIB[2]->Lock(0, 6*sizeof(WORD), (BYTE**)&pIndices, 0) ) )
#endif // DIRECT_VERSION_9_MJH
		return	E_FAIL;
	pIndices[0] = 0;
	pIndices[1] = 3;
	pIndices[2] = 2;
	pIndices[3] = 2;
	pIndices[4] = 1;
	pIndices[5] = 0;
	
#ifdef BATCH_RENDER
	memcpy( m_pIndices[eBDType_HorizonPlane], pIndices, sizeof(WORD) * m_pIndicesCount[eBDType_HorizonPlane]);
#endif	//	BATCH_RENDER

	m_pIB[2]->Unlock();
	
	//버텍스 버퍼
#ifdef BATCH_RENDER
	if( FAILED(g_RenderManager.CreateVertexBuffer( 4 * sizeof(VERTEX_XYZ_DIFFUSE_TEX1),
		D3DUSAGE_WRITEONLY, D3DFVF_VERTEX_XYZ_DIFFUSE_TEX1,
		D3DPOOL_MANAGED, &m_pVB[eBDType_HorizonPlane] ) ) )
	{
		return E_FAIL;
	}
	VERTEX_XYZ_DIFFUSE_TEX1* pVertices;
#else	//	BATCH_RENDER
	if( FAILED(g_RenderManager.CreateVertexBuffer( 4*sizeof(BILLBOARDVERTEX), D3DUSAGE_WRITEONLY,
		D3DFVF_BILLBOARDVERTEX, D3DPOOL_MANAGED, &m_pVB[2] ) ) )
	{
		return E_FAIL;
	}
	
	BILLBOARDVERTEX* pVertices;
#endif	//	BATCH_RENDER
#ifdef DIRECT_VERSION_9_MJH
	if( FAILED(m_pVB[2]->Lock( 0, 0, (void**)&pVertices, 0) ) )
#else
	if( FAILED(m_pVB[2]->Lock( 0, 0, (BYTE**)&pVertices, 0) ) )
#endif // DIRECT_VERSION_9_MJH
		return E_FAIL;
	
	pVertices[0].v	=	D3DXVECTOR3(-0.5f, 0.0f, 0.5f);
	pVertices[1].v	=	D3DXVECTOR3(-0.5f, 0.0f, -0.5f);
	pVertices[2].v	=	D3DXVECTOR3(0.5f, 0.0f, -0.5f);
	pVertices[3].v	=	D3DXVECTOR3(0.5f, 0.0f, 0.5f);
	
	pVertices[0].tu	=	0.0f;
	pVertices[0].tv	=	1.0f;
	pVertices[1].tu	=	0.0f;
	pVertices[1].tv	=	0.0f;
	pVertices[2].tu	=	1.0f;
	pVertices[2].tv	=	0.0f;
	pVertices[3].tu	=	1.0f;
	pVertices[3].tv	=	1.0f;
	
#ifdef BATCH_RENDER
	memcpy( m_pVertices[eBDType_HorizonPlane], pVertices, 
		sizeof(VERTEX_XYZ_DIFFUSE_TEX1) * m_pVerticesCount[eBDType_HorizonPlane]);
#endif	//	BATCH_RENDER

	m_pVB[2]->Unlock();
	
	return TRUE;
}

//................................................................................................................
// 3. 세로 방향 직사각형
//................................................................................................................
int	CBillboardManager::MakeVerticalLongPlane(void)
{
	if( FAILED(g_RenderManager.CreateIndexBuffer( 6*sizeof(WORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16,
		D3DPOOL_MANAGED, &m_pIB[3])))
	{
		return	E_FAIL;
	}
#ifdef BATCH_RENDER
	m_pIndicesCount[eBDType_VerticalLongPlane]	= 6;
	m_pVerticesCount[eBDType_VerticalLongPlane]	= 4;
	
	m_pIndices[eBDType_VerticalLongPlane]	= SAFE_NEW_ARRAY( WORD , m_pIndicesCount[eBDType_VerticalLongPlane] );
	m_pVertices[eBDType_VerticalLongPlane]	= SAFE_NEW_ARRAY( VERTEX_XYZ_DIFFUSE_TEX1, m_pVerticesCount[eBDType_VerticalLongPlane] );
	
#endif	//	BATCH_RENDER

	WORD	*pIndices;
#ifdef DIRECT_VERSION_9_MJH
	if ( FAILED ( m_pIB[3]->Lock(0, 6*sizeof(WORD), (void**)&pIndices, 0) ) )
#else
	if ( FAILED ( m_pIB[3]->Lock(0, 6*sizeof(WORD), (BYTE**)&pIndices, 0) ) )
#endif // DIRECT_VERSION_9_MJH
		return	E_FAIL;
	
	pIndices[0] = 0;
	pIndices[1] = 1;
	pIndices[2] = 3;
	pIndices[3] = 3;
	pIndices[4] = 1;
	pIndices[5] = 2;
	
#ifdef BATCH_RENDER
	memcpy( m_pIndices[eBDType_VerticalLongPlane], pIndices, sizeof(WORD) * m_pIndicesCount[eBDType_VerticalLongPlane]);
#endif	//	BATCH_RENDER

	m_pIB[3]->Unlock();

#ifdef BATCH_RENDER
	if( FAILED(g_RenderManager.CreateVertexBuffer( 4 * sizeof(VERTEX_XYZ_DIFFUSE_TEX1),
		D3DUSAGE_WRITEONLY, D3DFVF_VERTEX_XYZ_DIFFUSE_TEX1,
		D3DPOOL_MANAGED, &m_pVB[eBDType_VerticalLongPlane] ) ) )
	{
		return E_FAIL;
	}
	VERTEX_XYZ_DIFFUSE_TEX1* pVertices;
#else	//	BATCH_RENDER
	if( FAILED(g_RenderManager.CreateVertexBuffer( 4*sizeof(BILLBOARDVERTEX), D3DUSAGE_WRITEONLY, D3DFVF_BILLBOARDVERTEX,
		D3DPOOL_MANAGED, &m_pVB[3] ) ) )
	{
		return E_FAIL;
	}
	
	BILLBOARDVERTEX* pVertices;
#endif	//	BATCH_RENDER
#ifdef DIRECT_VERSION_9_MJH
	if( FAILED( m_pVB[3]->Lock( 0, 0, (void**)&pVertices, 0 ) ) )
#else
	if( FAILED( m_pVB[3]->Lock( 0, 0, (BYTE**)&pVertices, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH
		return E_FAIL;
	
	pVertices[0].v	=	D3DXVECTOR3(-0.5f, 0.0f, 0.0f);
	pVertices[1].v	=	D3DXVECTOR3(-0.5f, 8.0f, 0.0f);
	pVertices[2].v	=	D3DXVECTOR3(0.5f, 8.0f, 0.0f);
	pVertices[3].v	=	D3DXVECTOR3(0.5f, 0.0f, 0.0f);
	pVertices[0].tu	=	0.0f;
	pVertices[0].tv	=	1.0f;
	pVertices[1].tu	=	0.0f;
	pVertices[1].tv	=	0.0f;
	pVertices[2].tu	=	1.0f;
	pVertices[2].tv	=	0.0f;
	pVertices[3].tu	=	1.0f;
	pVertices[3].tv	=	1.0f;
	
#ifdef BATCH_RENDER
	memcpy( m_pVertices[eBDType_VerticalLongPlane], pVertices, 
		sizeof(VERTEX_XYZ_DIFFUSE_TEX1) * m_pVerticesCount[eBDType_VerticalLongPlane]);
#endif	//	BATCH_RENDER

	m_pVB[3]->Unlock();
	
	return TRUE;
}

//................................................................................................................
// 4. 가로 방향 직사각형
//................................................................................................................
int CBillboardManager::MakeHorizonLongPlane(void)
{
	//인덱스 버퍼
	if( FAILED(g_RenderManager.CreateIndexBuffer( 6*sizeof(WORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16,
		D3DPOOL_MANAGED, &m_pIB[4]) ) )
	{
		return	E_FAIL;
	}
#ifdef BATCH_RENDER
	m_pIndicesCount[eBDType_HorizonLongPlane]	= 6;
	m_pVerticesCount[eBDType_HorizonLongPlane]	= 4;
		
	m_pIndices[eBDType_HorizonLongPlane]	= SAFE_NEW_ARRAY( WORD , m_pIndicesCount[eBDType_HorizonLongPlane] );
	m_pVertices[eBDType_HorizonLongPlane]	= SAFE_NEW_ARRAY( VERTEX_XYZ_DIFFUSE_TEX1, m_pVerticesCount[eBDType_HorizonLongPlane] );
#endif	//	BATCH_RENDER
	WORD	*pIndices;
#ifdef DIRECT_VERSION_9_MJH
	if ( FAILED ( m_pIB[4]->Lock(0, 6*sizeof(WORD), (void**)&pIndices, 0) ) )
#else
	if ( FAILED ( m_pIB[4]->Lock(0, 6*sizeof(WORD), (BYTE**)&pIndices, 0) ) )
#endif // DIRECT_VERSION_9_MJH
		return	E_FAIL;
	pIndices[0] = 0;
	pIndices[1] = 3;
	pIndices[2] = 2;
	pIndices[3] = 2;
	pIndices[4] = 1;
	pIndices[5] = 0;
	
#ifdef BATCH_RENDER
	memcpy( m_pIndices[eBDType_HorizonLongPlane], pIndices, sizeof(WORD) * m_pIndicesCount[eBDType_HorizonLongPlane]);
#endif	//	BATCH_RENDER

	m_pIB[4]->Unlock();
	
	//버텍스 버퍼
#ifdef BATCH_RENDER
	if( FAILED(g_RenderManager.CreateVertexBuffer( 4 * sizeof(VERTEX_XYZ_DIFFUSE_TEX1),
		D3DUSAGE_WRITEONLY, D3DFVF_VERTEX_XYZ_DIFFUSE_TEX1,
		D3DPOOL_MANAGED, &m_pVB[eBDType_HorizonLongPlane] ) ) )
	{
		return E_FAIL;
	}
	VERTEX_XYZ_DIFFUSE_TEX1* pVertices;
#else	//	BATCH_RENDER
	if( FAILED(g_RenderManager.CreateVertexBuffer( 4*sizeof(BILLBOARDVERTEX), D3DUSAGE_WRITEONLY,
		D3DFVF_BILLBOARDVERTEX, D3DPOOL_MANAGED, &m_pVB[4] ) ) )
	{
		return E_FAIL;
	}
	BILLBOARDVERTEX* pVertices;
#endif	//	BATCH_RENDER
#ifdef DIRECT_VERSION_9_MJH
	if( FAILED(m_pVB[4]->Lock( 0, 0, (void**)&pVertices, 0) ) )
#else
	if( FAILED(m_pVB[4]->Lock( 0, 0, (BYTE**)&pVertices, 0) ) )
#endif // DIRECT_VERSION_9_MJH
		return E_FAIL;
	
	pVertices[0].v	=	D3DXVECTOR3(-0.5f, 0.0f, 0.0f);
	pVertices[1].v	=	D3DXVECTOR3(-0.5f, 0.0f, -8.0f);
	pVertices[2].v	=	D3DXVECTOR3(0.5f, 0.0f, -8.0f);
	pVertices[3].v	=	D3DXVECTOR3(0.5f, 0.0f, 0.0f);
	
	pVertices[0].tu	=	0.0f;
	pVertices[0].tv	=	1.0f;
	pVertices[1].tu	=	0.0f;
	pVertices[1].tv	=	0.0f;
	pVertices[2].tu	=	1.0f;
	pVertices[2].tv	=	0.0f;
	pVertices[3].tu	=	1.0f;
	pVertices[3].tv	=	1.0f;
	
#ifdef BATCH_RENDER
	memcpy( m_pVertices[eBDType_HorizonLongPlane], pVertices, 
		sizeof(VERTEX_XYZ_DIFFUSE_TEX1) * m_pVerticesCount[eBDType_HorizonLongPlane]);
#endif	//	BATCH_RENDER

	m_pVB[4]->Unlock();
	
	return TRUE;
}

//................................................................................................................
// 5. 실린더
//................................................................................................................
int	CBillboardManager::MakeCylinder(void)
{
	//인덱스 버퍼
	if( FAILED(g_RenderManager.CreateIndexBuffer( 144*sizeof(WORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16,
		D3DPOOL_MANAGED, &m_pIB[5]) ) )
	{
		return	E_FAIL;
	}
#ifdef BATCH_RENDER
	m_pIndicesCount[eBDType_Cylinder]	= 144;
	m_pVerticesCount[eBDType_Cylinder]	= 50;
		
	m_pIndices[eBDType_Cylinder]	= SAFE_NEW_ARRAY( WORD , m_pIndicesCount[eBDType_Cylinder] );
	m_pVertices[eBDType_Cylinder]	= SAFE_NEW_ARRAY( VERTEX_XYZ_DIFFUSE_TEX1, m_pVerticesCount[eBDType_Cylinder] );
#endif	//	BATCH_RENDER

	WORD	*pIndices;
#ifdef DIRECT_VERSION_9_MJH
	if ( FAILED ( m_pIB[5]->Lock(0, sizeof(WORD), (void**)&pIndices, 0) ) )
#else
	if ( FAILED ( m_pIB[5]->Lock(0, sizeof(WORD), (BYTE**)&pIndices, 0) ) )
#endif // DIRECT_VERSION_9_MJH
		return	E_FAIL;
	
	for( int i = 0; i < 24; i++ )
	{
		pIndices[0+i*6] = 2+i*2;
		pIndices[1+i*6] = 1+i*2;
		pIndices[2+i*6] = 0+i*2;
		pIndices[3+i*6] = 1+i*2;
		pIndices[4+i*6] = 2+i*2;
		pIndices[5+i*6] = 3+i*2;
	}
	
#ifdef BATCH_RENDER
	memcpy( m_pIndices[eBDType_Cylinder], pIndices, sizeof(WORD) * m_pIndicesCount[eBDType_Cylinder]);
#endif	//	BATCH_RENDER

	m_pIB[5]->Unlock();
	
#ifdef BATCH_RENDER
	if( FAILED(g_RenderManager.CreateVertexBuffer( 25*2 * sizeof(VERTEX_XYZ_DIFFUSE_TEX1),
		D3DUSAGE_WRITEONLY, D3DFVF_VERTEX_XYZ_DIFFUSE_TEX1,
		D3DPOOL_MANAGED, &m_pVB[eBDType_Cylinder] ) ) )
	{
		return E_FAIL;
	}
	VERTEX_XYZ_DIFFUSE_TEX1* pVertices;
#else	//	 BATCH_RENDER
	if( FAILED( g_RenderManager.CreateVertexBuffer( 25*2*sizeof(BILLBOARDVERTEX),
		D3DUSAGE_WRITEONLY, D3DFVF_BILLBOARDVERTEX,
		D3DPOOL_MANAGED, &m_pVB[5] ) ) )
	{
		return E_FAIL;
	}
	BILLBOARDVERTEX* pVertices;
#endif	//	BATCH_RENDER
#ifdef DIRECT_VERSION_9_MJH
	if( FAILED( m_pVB[5]->Lock( 0, 0, (void**)&pVertices, 0 ) ) )
#else
	if( FAILED( m_pVB[5]->Lock( 0, 0, (BYTE**)&pVertices, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH
		return E_FAIL;
	
	for( DWORD j = 0; j < 25; j++ )
	{
		FLOAT theta = (2*D3DX_PI*j)/24.0f;
		pVertices[2*j+0].v = D3DXVECTOR3( sinf(theta),0.0f, cosf(theta) );
		pVertices[2*j+1].v = D3DXVECTOR3( sinf(theta), 2.0f, cosf(theta) );
		
		FLOAT u = (float)j/24.0f;
		pVertices[2*j+0].tu = u;
		pVertices[2*j+1].tu = u;
		
		pVertices[2*j+0].tv = 1.0f;
		pVertices[2*j+1].tv = 0.0f;
	}
	
#ifdef BATCH_RENDER
	memcpy( m_pVertices[eBDType_Cylinder], pVertices, 
		sizeof(VERTEX_XYZ_DIFFUSE_TEX1) * m_pVerticesCount[eBDType_Cylinder]);
#endif	//	BATCH_RENDER

	m_pVB[5]->Unlock();
	
	return TRUE;
}

//................................................................................................................
// 6. 실린더 위가 큰거
//................................................................................................................
int	CBillboardManager::MakeCylinderUp(void)
{
	//인덱스 버퍼
	if( FAILED(g_RenderManager.CreateIndexBuffer( 144*sizeof(WORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16,
		D3DPOOL_MANAGED, &m_pIB[6]) ) )
	{
		return	E_FAIL;
	}
#ifdef BATCH_RENDER
	m_pIndicesCount[eBDType_CylinderUp]		= 144;
	m_pVerticesCount[eBDType_CylinderUp]	= 50;
	
	m_pIndices[eBDType_CylinderUp]	= SAFE_NEW_ARRAY( WORD , m_pIndicesCount[eBDType_CylinderUp] );
	m_pVertices[eBDType_CylinderUp]	= SAFE_NEW_ARRAY( VERTEX_XYZ_DIFFUSE_TEX1, m_pVerticesCount[eBDType_CylinderUp] );
#endif	//	BATCH_RENDER
	WORD	*pIndices;
#ifdef DIRECT_VERSION_9_MJH
	if ( FAILED ( m_pIB[6]->Lock(0, sizeof(WORD), (void**)&pIndices, 0) ) )
#else
	if ( FAILED ( m_pIB[6]->Lock(0, sizeof(WORD), (BYTE**)&pIndices, 0) ) )
#endif // DIRECT_VERSION_9_MJH
		return	E_FAIL;
	
	for( int i = 0; i < 24; i++ )
	{
		pIndices[0+i*6] = 2+i*2;
		pIndices[1+i*6] = 1+i*2;
		pIndices[2+i*6] = 0+i*2;
		pIndices[3+i*6] = 1+i*2;
		pIndices[4+i*6] = 2+i*2;
		pIndices[5+i*6] = 3+i*2;
	}
	
#ifdef BATCH_RENDER
	memcpy( m_pIndices[eBDType_CylinderUp], pIndices, sizeof(WORD) * m_pIndicesCount[eBDType_CylinderUp]);
#endif	//	BATCH_RENDER

	m_pIB[6]->Unlock();
	
#ifdef BATCH_RENDER
	if( FAILED(g_RenderManager.CreateVertexBuffer( 25*2 * sizeof(VERTEX_XYZ_DIFFUSE_TEX1),
		D3DUSAGE_WRITEONLY, D3DFVF_VERTEX_XYZ_DIFFUSE_TEX1,
		D3DPOOL_MANAGED, &m_pVB[eBDType_CylinderUp] ) ) )
	{
		return E_FAIL;
	}
	VERTEX_XYZ_DIFFUSE_TEX1* pVertices;
#else	//	BATCH_RENDER
	if( FAILED( g_RenderManager.CreateVertexBuffer( 25*2*sizeof(BILLBOARDVERTEX),
		D3DUSAGE_WRITEONLY, D3DFVF_BILLBOARDVERTEX,
		D3DPOOL_MANAGED, &m_pVB[6] ) ) )
	{
		return E_FAIL;
	}
	BILLBOARDVERTEX* pVertices;
#endif	//	BATCH_RENDER
#ifdef DIRECT_VERSION_9_MJH
	if( FAILED( m_pVB[6]->Lock( 0, 0, (void**)&pVertices, 0 ) ) )
#else
	if( FAILED( m_pVB[6]->Lock( 0, 0, (BYTE**)&pVertices, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH
		return E_FAIL;
	
	for( DWORD j=0; j<25; j++ )
	{
		FLOAT theta = (2*D3DX_PI*j)/24.0f;
		pVertices[2*j+0].v = D3DXVECTOR3( sinf(theta),0.0f, cosf(theta) );
		pVertices[2*j+1].v = D3DXVECTOR3( sinf(theta)*1.5f, 2.0f, cosf(theta)*1.5f );
		
		FLOAT u = (float)j/24.0f;
		pVertices[2*j+0].tu = u;
		pVertices[2*j+1].tu = u;
		
		pVertices[2*j+0].tv = 1.0f;
		pVertices[2*j+1].tv = 0.0f;
	}
	
#ifdef BATCH_RENDER
	memcpy( m_pVertices[eBDType_CylinderUp], pVertices, 
		sizeof(VERTEX_XYZ_DIFFUSE_TEX1) * m_pVerticesCount[eBDType_CylinderUp]);
#endif	//	BATCH_RENDER

	m_pVB[6]->Unlock();
	
	return TRUE;
}

//................................................................................................................
// 7. 반구
//................................................................................................................
int CBillboardManager::MakeHemisphere(void)
{
	if( FAILED( g_RenderManager.CreateVertexBuffer( SPHERE_VERTEX_COUNT * sizeof( BILLBOARDVERTEX ) ,
		D3DUSAGE_WRITEONLY , D3DFVF_BILLBOARDVERTEX ,
		D3DPOOL_MANAGED, &m_pVB[ 7 ] ) ) )
	{
		return FALSE;
	}
	
	//...................................................................................................
	// 구 만들기
	//...................................................................................................
	BILLBOARDVERTEX *vtx;
#ifdef DIRECT_VERSION_9_MJH
	if( FAILED( m_pVB[ 7 ]->Lock( 0, 0, (void**)&vtx, 0 ) ) )
#else
	if( FAILED( m_pVB[ 7 ]->Lock( 0, 0, (BYTE**)&vtx, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH	
	{
		return FALSE;		
	}
	
	FLOAT fDeltaRingAngle = ( ( D3DX_PI * 0.5f ) / SPHERE_RING_COUNT );
	FLOAT fDeltaSegAngle  = ( 2.0f * D3DX_PI / SPHERE_SEGMENT_COUNT );
	
	for( DWORD ring = 0; ring < SPHERE_RING_COUNT ; ring++ )
	{
		FLOAT r0 = cosf( (ring+0) * fDeltaRingAngle );
		FLOAT r1 = cosf( (ring+1) * fDeltaRingAngle );
		FLOAT y0 = sinf( (ring+0) * fDeltaRingAngle );
		FLOAT y1 = sinf( (ring+1) * fDeltaRingAngle );
		
		for( DWORD seg = 0; seg < ( SPHERE_SEGMENT_COUNT + 1 ); seg++ )
		{
			FLOAT x0 =  r0 * cosf( seg * fDeltaSegAngle );
			FLOAT z0 =  r0 * sinf( seg * fDeltaSegAngle );
			FLOAT x1 =  r1 * cosf( seg * fDeltaSegAngle );
			FLOAT z1 =  r1 * sinf( seg * fDeltaSegAngle );
			
			
			vtx->v    = D3DXVECTOR3( x0 , y0 , z0 );
			vtx->tu   = -((FLOAT)seg)/SPHERE_SEGMENT_COUNT;
			vtx->tv   =  (ring+0)/(FLOAT)SPHERE_RING_COUNT;
			
			vtx++;
			
			vtx->v   = D3DXVECTOR3( x1 , y1 , z1 );
			vtx->tu  = -((FLOAT)seg)/SPHERE_SEGMENT_COUNT;
			vtx->tv  =  (ring+1)/(FLOAT)SPHERE_RING_COUNT;
			
			vtx++;
		}
	}
	
	m_pVB[ 7 ]->Unlock();
	
	if( FAILED( 
		g_RenderManager.CreateIndexBuffer( ( sizeof( WORD ) * SPHERE_VERTEX_COUNT )   ,
		D3DUSAGE_WRITEONLY	 ,
		D3DFMT_INDEX16 		 ,
		D3DPOOL_MANAGED		 ,
		&m_pIB[7] )) )
	{
		return FALSE;		
	}
	
	
	WORD *pIB = NULL;
#ifdef DIRECT_VERSION_9_MJH
	if( FAILED( m_pIB[7]->Lock( 0, 0, (void**)&pIB, 0 ) ) )
#else
	if( FAILED( m_pIB[7]->Lock( 0, 0, (BYTE**)&pIB, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH	
	{
		return FALSE;		
	}
	
	for( WORD index = 0; index < SPHERE_VERTEX_COUNT ; ++index )
	{
		*pIB = index;
		
		++pIB;
	}
	
	m_pIB[7]->Unlock();
	
	
	return TRUE;
}

//................................................................................................................
// 8. 실린더 아래가 큰거
//................................................................................................................
int CBillboardManager::MakeCylinderDown(void)
{
	//인덱스 버퍼
	if( FAILED(g_RenderManager.CreateIndexBuffer( 144*sizeof(WORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16,
		D3DPOOL_MANAGED, &m_pIB[8]) ) )
	{
		return	E_FAIL;
	}
#ifdef BATCH_RENDER
	m_pIndicesCount[eBDType_CylinderDown]	= 144;
	m_pVerticesCount[eBDType_CylinderDown]	= 50;
	
	m_pIndices[eBDType_CylinderDown]	= SAFE_NEW_ARRAY( WORD , m_pIndicesCount[eBDType_CylinderDown] );
	m_pVertices[eBDType_CylinderDown]	= SAFE_NEW_ARRAY( VERTEX_XYZ_DIFFUSE_TEX1, m_pVerticesCount[eBDType_CylinderDown] );
#endif	//	BATCH_RENDER

	WORD	*pIndices;
#ifdef DIRECT_VERSION_9_MJH
	if ( FAILED ( m_pIB[8]->Lock(0, sizeof(WORD), (void**)&pIndices, 0) ) )
#else
	if ( FAILED ( m_pIB[8]->Lock(0, sizeof(WORD), (BYTE**)&pIndices, 0) ) )
#endif // DIRECT_VERSION_9_MJH
		return	E_FAIL;
	
	for( int i = 0; i < 24; i++ )
	{
		pIndices[0+i*6] = 2+i*2;
		pIndices[1+i*6] = 1+i*2;
		pIndices[2+i*6] = 0+i*2;
		pIndices[3+i*6] = 1+i*2;
		pIndices[4+i*6] = 2+i*2;
		pIndices[5+i*6] = 3+i*2;
	}
	
#ifdef BATCH_RENDER
	memcpy( m_pIndices[eBDType_CylinderDown], pIndices, sizeof(WORD) * m_pIndicesCount[eBDType_CylinderDown]);
#endif	//	BATCH_RENDER

	m_pIB[8]->Unlock();
	
#ifdef BATCH_RENDER
	if( FAILED(g_RenderManager.CreateVertexBuffer( 25*2 * sizeof(VERTEX_XYZ_DIFFUSE_TEX1),
		D3DUSAGE_WRITEONLY, D3DFVF_VERTEX_XYZ_DIFFUSE_TEX1,
		D3DPOOL_MANAGED, &m_pVB[eBDType_CylinderDown] ) ) )
	{
		return E_FAIL;
	}
	VERTEX_XYZ_DIFFUSE_TEX1* pVertices;
#else	//	BATCH_RENDER
	if( FAILED( g_RenderManager.CreateVertexBuffer( 25*2*sizeof(BILLBOARDVERTEX),
		D3DUSAGE_WRITEONLY, D3DFVF_BILLBOARDVERTEX,
		D3DPOOL_MANAGED, &m_pVB[8] ) ) )
	{
		return E_FAIL;
	}
	BILLBOARDVERTEX* pVertices;
#endif	//	BATCH_RENDER
#ifdef DIRECT_VERSION_9_MJH
	if( FAILED( m_pVB[8]->Lock( 0, 0, (void**)&pVertices, 0 ) ) )
#else
	if( FAILED( m_pVB[8]->Lock( 0, 0, (BYTE**)&pVertices, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH
		return E_FAIL;
	
	for( DWORD j=0; j<25; j++ )
	{
		FLOAT theta = (2*D3DX_PI*j)/24.0f;
		pVertices[2*j+0].v = D3DXVECTOR3( sinf(theta)*0.5f,cosf(theta)*0.5f, 0.0f );
		pVertices[2*j+1].v = D3DXVECTOR3( sinf(theta)*1.5f,cosf(theta)*1.5f, 2.0f  );
		
		FLOAT u = (float)j/24.0f;
		pVertices[2*j+0].tu = u;
		pVertices[2*j+1].tu = u;
		
		pVertices[2*j+0].tv = 1.0f;
		pVertices[2*j+1].tv = 0.0f;
	}
#ifdef BATCH_RENDER
	memcpy( m_pVertices[eBDType_CylinderDown], pVertices, 
		sizeof(VERTEX_XYZ_DIFFUSE_TEX1) * m_pVerticesCount[eBDType_CylinderDown]);
#endif	//	BATCH_RENDER
	m_pVB[8]->Unlock();
	
	return TRUE;
}

//................................................................................................................
// 9. 구
//................................................................................................................
int CBillboardManager::MakeSphere(void)
{
	if( FAILED( g_RenderManager.CreateVertexBuffer( SPHERE_VERTEX_COUNT * sizeof( BILLBOARDVERTEX ) ,
		D3DUSAGE_WRITEONLY , D3DFVF_BILLBOARDVERTEX ,
		D3DPOOL_MANAGED, &m_pVB[ 9 ] ) ) )
	{
		return FALSE;
	}
	
	//...................................................................................................
	// 구 만들기
	//...................................................................................................
	BILLBOARDVERTEX *vtx;
#ifdef DIRECT_VERSION_9_MJH
	if( FAILED( m_pVB[ 9 ]->Lock( 0, 0, (void**)&vtx, 0 ) ) )
#else
	if( FAILED( m_pVB[ 9 ]->Lock( 0, 0, (BYTE**)&vtx, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH	
	{
		return FALSE;		
	}
	
	FLOAT fDeltaRingAngle = ( D3DX_PI / SPHERE_RING_COUNT );
	FLOAT fDeltaSegAngle  = ( ( 2.0f * D3DX_PI ) / SPHERE_SEGMENT_COUNT );
	
	for( DWORD ring = 0; ring < SPHERE_RING_COUNT ; ring++ )
	{
		FLOAT r0 = sinf( (ring+0) * fDeltaRingAngle );
		FLOAT y0 = cosf( (ring+0) * fDeltaRingAngle );
		FLOAT r1 = sinf( (ring+1) * fDeltaRingAngle );
		FLOAT y1 = cosf( (ring+1) * fDeltaRingAngle );
		
		for( DWORD seg = 0; seg < ( SPHERE_SEGMENT_COUNT + 1 ); seg++ )
		{
			FLOAT x0 =  r0 * sinf( seg * fDeltaSegAngle );
			FLOAT z0 =  r0 * cosf( seg * fDeltaSegAngle );
			FLOAT x1 =  r1 * sinf( seg * fDeltaSegAngle );
			FLOAT z1 =  r1 * cosf( seg * fDeltaSegAngle );
			
			vtx->v = D3DXVECTOR3( x0 , y0 , z0 );
			vtx->tu   = -((FLOAT)seg)/SPHERE_SEGMENT_COUNT;
			vtx->tv   =  (ring+0)/(FLOAT)SPHERE_RING_COUNT;
			
			vtx++;
			
			vtx->v = D3DXVECTOR3( x1 , y1 , z1);
			vtx->tu  = -((FLOAT)seg)/SPHERE_SEGMENT_COUNT;
			vtx->tv  =  (ring+1)/(FLOAT)SPHERE_RING_COUNT;
			
			vtx++;
		}
	}
	
	m_pVB[ 9 ]->Unlock();
	
	if( FAILED( 
		g_RenderManager.CreateIndexBuffer( ( sizeof( WORD ) * SPHERE_VERTEX_COUNT )   ,
		D3DUSAGE_WRITEONLY	 ,
		D3DFMT_INDEX16 		 ,
		D3DPOOL_MANAGED		 ,
		&m_pIB[9] )) )
	{
		return FALSE;		
	}
	
	
	WORD *pIB = NULL;
#ifdef DIRECT_VERSION_9_MJH
	if( FAILED( m_pIB[9]->Lock( 0, 0, (void**)&pIB, 0 ) ) )
#else
	if( FAILED( m_pIB[9]->Lock( 0, 0, (BYTE**)&pIB, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH	
	{
		return FALSE;		
	}
	
	for( WORD index = 0; index < SPHERE_VERTEX_COUNT ; ++index )
	{
		*pIB = index;
		
		++pIB;
	}
	
	m_pIB[9]->Unlock();
	
	return TRUE;
}

//................................................................................................................
// 10. 도넛
//................................................................................................................
int	CBillboardManager::MakeDonut(void)
{
	int    Div         = 30;
	float  Radius0     = 0.8f;
	float  Radius1     = 1.0f;
	int    FaceCount   = Div * 2;
	int    IndexCount  = FaceCount + 2;
	int    VertexCount = ( Div + 1 ) * 2;
	
	//............................................................................................................
	// 인덱스 버퍼 만들기
	//............................................................................................................
	if( FAILED( 
		g_RenderManager.CreateIndexBuffer( ( sizeof( WORD ) * IndexCount )   ,
		D3DUSAGE_WRITEONLY	     ,
		D3DFMT_INDEX16 		 ,
		D3DPOOL_MANAGED		 ,
		&m_pIB[10] )) )
	{
		return FALSE;		
	}
	
	WORD   *pIndexBuffer = NULL;
#ifdef DIRECT_VERSION_9_MJH
	if( FAILED ( m_pIB[10]->Lock(0, 0, (void**)&pIndexBuffer, 0 ) ) )
#else
	if( FAILED ( m_pIB[10]->Lock(0, 0, (BYTE**)&pIndexBuffer, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH	
	{
		return FALSE;
	}
	
	for( int i = 0; i < IndexCount ; ++i )
	{
		pIndexBuffer[ i ] = (WORD) i;
	}
	
	m_pIB[10]->Unlock();
	
	
	//............................................................................................................
	// 버텍스 버퍼 만들기
	//............................................................................................................
	BILLBOARDVERTEX	*pVertexList = NULL;
	
	if( FAILED( g_RenderManager.CreateVertexBuffer( ( sizeof( BILLBOARDVERTEX ) * VertexCount ) ,
		D3DUSAGE_WRITEONLY, D3DFVF_BILLBOARDVERTEX ,
		D3DPOOL_MANAGED, &m_pVB[10] ) ) )
	{
		return FALSE;		
	}
	
	BILLBOARDVERTEX   *pVertice = NULL;
#ifdef DIRECT_VERSION_9_MJH
	if( FAILED ( m_pVB[10]->Lock(0, 0, (void**)&pVertice, 0 ) ) )
#else
	if( FAILED ( m_pVB[10]->Lock(0, 0, (BYTE**)&pVertice, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH	
	{
		return FALSE;
	}
	
	D3DXVECTOR3	 vPos0( Radius0 , 0.0f , 0.0f );
	D3DXVECTOR3	 vPos1( Radius1 , 0.0f , 0.0f );
	D3DXMATRIX	 RotY;
	
	float		 RadY = 360.0f / ( float ) Div; 
	
	RadY = D3DXToRadian( RadY );
	
	for(int i = 0 ; i < ( Div + 1 ) ; ++i )
	{
		D3DXMatrixRotationY( &RotY , ( RadY * i ) );
		
		D3DXVec3TransformCoord( &pVertice->v , &vPos0 , &RotY );
		
		pVertice->tu = ( float ) i;
		pVertice->tv = 1.0f;
		pVertice++;
		
		D3DXVec3TransformCoord( &pVertice->v , &vPos1 , &RotY );
		
		pVertice->tu = ( float ) i;
		pVertice->tv = 0.0f;
		
		pVertice++;
	}
	
	m_pVB[10]->Unlock();
	
	return TRUE;
}

//................................................................................................................
// 11. 도넛 ( UV 전체 )
//................................................................................................................
int	CBillboardManager::MakeDonutUVOne(void)
{
	int    Div         = 30;
	float  Radius0     = 0.8f;
	float  Radius1     = 1.0f;
	int    FaceCount   = Div * 2;
	int    IndexCount  = FaceCount + 2;
	int    VertexCount = ( Div + 1 ) * 2;
	
	//............................................................................................................
	// 인덱스 버퍼 만들기
	//............................................................................................................
	if( FAILED( 
		g_RenderManager.CreateIndexBuffer( ( sizeof( WORD ) * IndexCount )   ,
		D3DUSAGE_WRITEONLY	     ,
		D3DFMT_INDEX16 		 ,
		D3DPOOL_MANAGED		 ,
		&m_pIB[11] )) )
	{
		return FALSE;		
	}
	
	WORD   *pIndexBuffer = NULL;
#ifdef DIRECT_VERSION_9_MJH
	if( FAILED ( m_pIB[11]->Lock(0, 0, (void**)&pIndexBuffer, 0 ) ) )
#else
	if( FAILED ( m_pIB[11]->Lock(0, 0, (BYTE**)&pIndexBuffer, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH	
	{
		return FALSE;
	}
	
	for( int i = 0; i < IndexCount ; ++i )
	{
		pIndexBuffer[ i ] = (WORD) i;
	}
	
	m_pIB[11]->Unlock();
	
	
	//............................................................................................................
	// 버텍스 버퍼 만들기
	//............................................................................................................
	BILLBOARDVERTEX	*pVertexList = NULL;
	
	if( FAILED( g_RenderManager.CreateVertexBuffer( ( sizeof( BILLBOARDVERTEX ) * VertexCount ) ,
		D3DUSAGE_WRITEONLY, D3DFVF_BILLBOARDVERTEX ,
		D3DPOOL_MANAGED, &m_pVB[11] ) ) )
	{
		return FALSE;		
	}
	
	BILLBOARDVERTEX   *pVertice = NULL;
#ifdef DIRECT_VERSION_9_MJH
	if( FAILED ( m_pVB[11]->Lock(0, 0, (void**)&pVertice, 0 ) ) )
#else
	if( FAILED ( m_pVB[11]->Lock(0, 0, (BYTE**)&pVertice, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH	
	{
		return FALSE;
	}
	
	D3DXVECTOR3	 vPos0( Radius0 , 0.0f , 0.0f );
	D3DXVECTOR3	 vPos1( Radius1 , 0.0f , 0.0f );
	D3DXMATRIX	 RotY;
	
	float		 RadY = 360.0f / ( float ) Div; 
	
	RadY = D3DXToRadian( RadY );
	
	for(int i = 0 ; i < ( Div + 1 ) ; ++i )
	{
		D3DXMatrixRotationY( &RotY , ( RadY * i ) );
		
		D3DXVec3TransformCoord( &pVertice->v , &vPos0 , &RotY );
		
		pVertice->tu = ( float ) i / ( float ) ( Div + 1 );
		pVertice->tv = 1.0f;
		pVertice++;
		
		D3DXVec3TransformCoord( &pVertice->v , &vPos1 , &RotY );
		
		pVertice->tu = ( float ) i / ( float ) ( Div + 1 );
		pVertice->tv = 0.0f;
		
		pVertice++;
	}
	
	m_pVB[11]->Unlock();
	
	return TRUE;
}

//................................................................................................................
// 12. 스팟 
//................................................................................................................
int	CBillboardManager::MakeSpot(void)
{
	int Div = 10;
	int IndexCount  = ( Div * 6 );
	int VertexCount = Div * 4;
	
	//............................................................................................................
	// 인덱스 버퍼 만들기
	//............................................................................................................
	if( FAILED( 
		g_RenderManager.CreateIndexBuffer( ( sizeof( WORD ) * IndexCount )   ,
		D3DUSAGE_WRITEONLY	     ,
		D3DFMT_INDEX16 		 ,
		D3DPOOL_MANAGED		 ,
		&m_pIB[12] )) )
	{
		return FALSE;		
	}
	
#ifdef BATCH_RENDER
	m_pIndicesCount[eBDType_Spot]	= 60;
	m_pVerticesCount[eBDType_Spot]	= 40;
	
	m_pIndices[eBDType_Spot]	= SAFE_NEW_ARRAY( WORD , m_pIndicesCount[eBDType_Spot] );
	m_pVertices[eBDType_Spot]	= SAFE_NEW_ARRAY( VERTEX_XYZ_DIFFUSE_TEX1, m_pVerticesCount[eBDType_Spot] );
#endif	//	BATCH_RENDER

	WORD   *pIndexBuffer = NULL;
#ifdef DIRECT_VERSION_9_MJH
	if( FAILED ( m_pIB[12]->Lock(0, 0, (void**)&pIndexBuffer, 0 ) ) )
#else
	if( FAILED ( m_pIB[12]->Lock(0, 0, (BYTE**)&pIndexBuffer, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH	
	{
		return FALSE;
	}
	
	for( int i = 0; i < Div ; ++i )
	{
		pIndexBuffer[ 6 * i + 0 ] = (WORD) 4 * i + 0;
		pIndexBuffer[ 6 * i + 1 ] = (WORD) 4 * i + 1;
		pIndexBuffer[ 6 * i + 2 ] = (WORD) 4 * i + 2;
		pIndexBuffer[ 6 * i + 3 ] = (WORD) 4 * i + 1;
		pIndexBuffer[ 6 * i + 4 ] = (WORD) 4 * i + 3;
		pIndexBuffer[ 6 * i + 5 ] = (WORD) 4 * i + 2;
	}
#ifdef BATCH_RENDER
	memcpy( m_pIndices[eBDType_Spot], pIndexBuffer, sizeof(WORD) * m_pIndicesCount[eBDType_Spot]);
#endif	//	BATCH_RENDER
	m_pIB[12]->Unlock();
	
	
	//............................................................................................................
	// 버텍스 버퍼 만들기
	//............................................................................................................
#ifdef BATCH_RENDER
	if( FAILED(g_RenderManager.CreateVertexBuffer( m_pVerticesCount[eBDType_Spot] * sizeof(VERTEX_XYZ_DIFFUSE_TEX1),
		D3DUSAGE_WRITEONLY, D3DFVF_VERTEX_XYZ_DIFFUSE_TEX1,
		D3DPOOL_MANAGED, &m_pVB[eBDType_Spot] ) ) )
	{
		return E_FAIL;
	}
	VERTEX_XYZ_DIFFUSE_TEX1* pVertice;
#else	//	BATCH_RENDER
	if( FAILED( g_RenderManager.CreateVertexBuffer( ( sizeof( BILLBOARDVERTEX ) * VertexCount ) ,
		D3DUSAGE_WRITEONLY, D3DFVF_BILLBOARDVERTEX ,
		D3DPOOL_MANAGED, &m_pVB[12] ) ) )
	{
		return FALSE;		
	}
	BILLBOARDVERTEX   *pVertice = NULL;
#endif	//	BATCH_RENDER
#ifdef DIRECT_VERSION_9_MJH
	if( FAILED ( m_pVB[12]->Lock(0, 0, (void**)&pVertice, 0 ) ) )
#else
	if( FAILED ( m_pVB[12]->Lock(0, 0, (BYTE**)&pVertice, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH	
	{
		return FALSE;
	}
	
	D3DXVECTOR3	 vPos0( -0.5f , 1.0f , 0.0f );
	D3DXVECTOR3	 vPos1( -0.5f , 5.0f , 0.0f );
	D3DXVECTOR3	 vPos2(  0.5f , 1.0f , 0.0f );
	D3DXVECTOR3	 vPos3(  0.5f , 5.0f , 0.0f );
	
	D3DXMATRIX	 Rot;
	
	float		 Rad = 360.0f / ( float ) Div; 
	
	Rad = D3DXToRadian( Rad );
#ifdef BATCH_RENDER
	VERTEX_XYZ_DIFFUSE_TEX1* pRealVertice = pVertice;
#endif	//	BATCH_RENDER
	for(int i = 0 ; i < Div ; ++i )
	{
		D3DXMatrixRotationZ( &Rot , ( Rad * i ) );
		
		D3DXVec3TransformCoord( &pVertice->v , &vPos0 , &Rot );
		
		pVertice->tu = 0.0f;
		pVertice->tv = 1.0f;
		pVertice++;
		
		D3DXVec3TransformCoord( &pVertice->v , &vPos1 , &Rot );
		
		pVertice->tu = 0.0f;
		pVertice->tv = 0.0f;
		pVertice++;
		
		D3DXVec3TransformCoord( &pVertice->v , &vPos2 , &Rot );
		
		pVertice->tu = 1.0f;
		pVertice->tv = 1.0f;
		pVertice++;
		
		D3DXVec3TransformCoord( &pVertice->v , &vPos3 , &Rot );
		
		pVertice->tu = 1.0f;
		pVertice->tv = 0.0f;
		pVertice++;
	}
#ifdef BATCH_RENDER
	memcpy( m_pVertices[eBDType_Spot], pRealVertice, 
		sizeof(VERTEX_XYZ_DIFFUSE_TEX1) * m_pVerticesCount[eBDType_Spot]);
#endif	//	BATCH_RENDER
	m_pVB[12]->Unlock();
	
	return TRUE;	
}

// 13. 스팟 ( 뒤로 굽은거 )
int CBillboardManager::MakeSpot1(void)
{
	int Div = 10;
	int IndexCount  = ( Div * 6 );
	int VertexCount = Div * 4;
	
	//............................................................................................................
	// 인덱스 버퍼 만들기
	//............................................................................................................
	if( FAILED( 
		g_RenderManager.CreateIndexBuffer( ( sizeof( WORD ) * IndexCount )   ,
		D3DUSAGE_WRITEONLY	     ,
		D3DFMT_INDEX16 		 ,
		D3DPOOL_MANAGED		 ,
		&m_pIB[13] )) )
	{
		return FALSE;		
	}
#ifdef BATCH_RENDER
	m_pIndicesCount[eBDType_Spot1]  = 60;
	m_pVerticesCount[eBDType_Spot1]	= 40;
	m_pIndices[eBDType_Spot1]	= SAFE_NEW_ARRAY( WORD , m_pIndicesCount[eBDType_Spot1] );
	m_pVertices[eBDType_Spot1]	= SAFE_NEW_ARRAY( VERTEX_XYZ_DIFFUSE_TEX1, m_pVerticesCount[eBDType_Spot1] );
#endif	//	BATCH_RENDER
	WORD   *pIndexBuffer = NULL;
#ifdef DIRECT_VERSION_9_MJH
	if( FAILED ( m_pIB[13]->Lock(0, 0, (void**)&pIndexBuffer, 0 ) ) )
#else
	if( FAILED ( m_pIB[13]->Lock(0, 0, (BYTE**)&pIndexBuffer, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH	
	{
		return FALSE;
	}
	
	for( int i = 0; i < Div ; ++i )
	{
		pIndexBuffer[ 6 * i + 0 ] = (WORD) 4 * i + 0;
		pIndexBuffer[ 6 * i + 1 ] = (WORD) 4 * i + 1;
		pIndexBuffer[ 6 * i + 2 ] = (WORD) 4 * i + 2;
		pIndexBuffer[ 6 * i + 3 ] = (WORD) 4 * i + 1;
		pIndexBuffer[ 6 * i + 4 ] = (WORD) 4 * i + 3;
		pIndexBuffer[ 6 * i + 5 ] = (WORD) 4 * i + 2;
	}
#ifdef BATCH_RENDER
	memcpy( m_pIndices[eBDType_Spot1], pIndexBuffer, sizeof(WORD) * m_pIndicesCount[eBDType_Spot1]);
#endif	//	BATCH_RENDER
	m_pIB[13]->Unlock();
		
	//............................................................................................................
	// 버텍스 버퍼 만들기
	//............................................................................................................
#ifdef BATCH_RENDER
	if( FAILED(g_RenderManager.CreateVertexBuffer( m_pVerticesCount[eBDType_Spot1] * sizeof(VERTEX_XYZ_DIFFUSE_TEX1),
		D3DUSAGE_WRITEONLY, D3DFVF_VERTEX_XYZ_DIFFUSE_TEX1,
		D3DPOOL_MANAGED, &m_pVB[eBDType_Spot1] ) ) )
	{
		return E_FAIL;
	}
	VERTEX_XYZ_DIFFUSE_TEX1* pVertice;
#else	//	BATCH_RENDER
	if( FAILED( g_RenderManager.CreateVertexBuffer( ( sizeof( BILLBOARDVERTEX ) * VertexCount ) ,
		D3DUSAGE_WRITEONLY, D3DFVF_BILLBOARDVERTEX ,
		D3DPOOL_MANAGED, &m_pVB[13] ) ) )
	{
		return FALSE;	
	}
	BILLBOARDVERTEX   *pVertice = NULL;
#endif	//	BATCH_RENDER
#ifdef DIRECT_VERSION_9_MJH
	if( FAILED ( m_pVB[13]->Lock(0, 0, (void**)&pVertice, 0 ) ) )
#else
	if( FAILED ( m_pVB[13]->Lock(0, 0, (BYTE**)&pVertice, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH
	{
		return FALSE;
	}
	
	D3DXVECTOR3	 vPos0( -0.5f , 1.0f , 0.0f );
	D3DXVECTOR3	 vPos1( -0.5f , 5.0f , 3.0f );
	D3DXVECTOR3	 vPos2(  0.5f , 1.0f , 0.0f );
	D3DXVECTOR3	 vPos3(  0.5f , 5.0f , 3.0f );
	
	D3DXMATRIX	 Rot;
	
	float		 Rad = 360.0f / ( float ) Div; 
	
	Rad = D3DXToRadian( Rad );
#ifdef BATCH_RENDER
	VERTEX_XYZ_DIFFUSE_TEX1* pRealVertice = pVertice;
#endif	//	BATCH_RENDER
	for(int i = 0 ; i < Div ; ++i )
	{
		D3DXMatrixRotationZ( &Rot , ( Rad * i ) );
		
		D3DXVec3TransformCoord( &pVertice->v , &vPos0 , &Rot );
		
		pVertice->tu = 0.0f;
		pVertice->tv = 1.0f;
		pVertice++;
		
		D3DXVec3TransformCoord( &pVertice->v , &vPos1 , &Rot );
		
		pVertice->tu = 0.0f;
		pVertice->tv = 0.0f;
		pVertice++;
		
		D3DXVec3TransformCoord( &pVertice->v , &vPos2 , &Rot );
		
		pVertice->tu = 1.0f;
		pVertice->tv = 1.0f;
		pVertice++;
		
		D3DXVec3TransformCoord( &pVertice->v , &vPos3 , &Rot );
		
		pVertice->tu = 1.0f;
		pVertice->tv = 0.0f;
		pVertice++;
	}
#ifdef BATCH_RENDER
	memcpy( m_pVertices[eBDType_Spot1], pRealVertice, 
		sizeof(VERTEX_XYZ_DIFFUSE_TEX1) * m_pVerticesCount[eBDType_Spot1]);
#endif	//	BATCH_RENDER
	m_pVB[13]->Unlock();
	
	return TRUE;	
}

// 13. 스팟 ( 위로 굽은거 )
int CBillboardManager::MakeSpot2(void)
{
	int Div = 10;
	int IndexCount  = ( Div * 6 );
	int VertexCount = Div * 4;
	
	//............................................................................................................
	// 인덱스 버퍼 만들기
	//............................................................................................................
	if( FAILED( 
		g_RenderManager.CreateIndexBuffer( ( sizeof( WORD ) * IndexCount )   ,
		D3DUSAGE_WRITEONLY	     ,
		D3DFMT_INDEX16 		 ,
		D3DPOOL_MANAGED		 ,
		&m_pIB[14] )) )
	{
		return FALSE;		
	}
#ifdef BATCH_RENDER
	m_pIndicesCount[eBDType_Spot2]	= 60;
	m_pVerticesCount[eBDType_Spot2]	= 40;
	
	m_pIndices[eBDType_Spot2]	= SAFE_NEW_ARRAY( WORD , m_pIndicesCount[eBDType_Spot2] );
	m_pVertices[eBDType_Spot2]	= SAFE_NEW_ARRAY( VERTEX_XYZ_DIFFUSE_TEX1, m_pVerticesCount[eBDType_Spot2] );
#endif	//	BATCH_RENDER
	WORD   *pIndexBuffer = NULL;
#ifdef DIRECT_VERSION_9_MJH
	if( FAILED ( m_pIB[14]->Lock(0, 0, (void**)&pIndexBuffer, 0 ) ) )
#else
	if( FAILED ( m_pIB[14]->Lock(0, 0, (BYTE**)&pIndexBuffer, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH	
	{
		return FALSE;
	}
	
	for( int i = 0; i < Div ; ++i )
	{
		pIndexBuffer[ 6 * i + 0 ] = (WORD) 4 * i + 0;
		pIndexBuffer[ 6 * i + 1 ] = (WORD) 4 * i + 1;
		pIndexBuffer[ 6 * i + 2 ] = (WORD) 4 * i + 2;
		pIndexBuffer[ 6 * i + 3 ] = (WORD) 4 * i + 1;
		pIndexBuffer[ 6 * i + 4 ] = (WORD) 4 * i + 3;
		pIndexBuffer[ 6 * i + 5 ] = (WORD) 4 * i + 2;
	}
#ifdef BATCH_RENDER
	memcpy( m_pIndices[eBDType_Spot2], pIndexBuffer, sizeof(WORD) * m_pIndicesCount[eBDType_Spot2]);
#endif	//	BATCH_RENDER
	m_pIB[14]->Unlock();
	
	
	//............................................................................................................
	// 버텍스 버퍼 만들기
	//............................................................................................................
#ifdef BATCH_RENDER
	if( FAILED(g_RenderManager.CreateVertexBuffer( m_pVerticesCount[eBDType_Spot2] * sizeof(VERTEX_XYZ_DIFFUSE_TEX1),
		D3DUSAGE_WRITEONLY, D3DFVF_VERTEX_XYZ_DIFFUSE_TEX1,
		D3DPOOL_MANAGED, &m_pVB[eBDType_Spot2] ) ) )
	{
		return E_FAIL;
	}
	VERTEX_XYZ_DIFFUSE_TEX1* pVertice;
#else	//	BATCH_RENDER
	if( FAILED( g_RenderManager.CreateVertexBuffer( ( sizeof( BILLBOARDVERTEX ) * VertexCount ) ,
		D3DUSAGE_WRITEONLY, D3DFVF_BILLBOARDVERTEX ,
		D3DPOOL_MANAGED, &m_pVB[14] ) ) )
	{
		return FALSE;		
	}
	BILLBOARDVERTEX   *pVertice = NULL;
#endif	//	BATCH_RENDER
#ifdef DIRECT_VERSION_9_MJH
	if( FAILED ( m_pVB[14]->Lock(0, 0, (void**)&pVertice, 0 ) ) )
#else
	if( FAILED ( m_pVB[14]->Lock(0, 0, (BYTE**)&pVertice, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH	
	{
		return FALSE;
	}
	
	D3DXVECTOR3	 vPos0( -0.5f , 0.0f , 1.0f );
	D3DXVECTOR3	 vPos1( -0.5f , 4.0f , 4.0f );
	D3DXVECTOR3	 vPos2(  0.5f , 0.0f , 1.0f );
	D3DXVECTOR3	 vPos3(  0.5f , 4.0f , 4.0f );
	
	D3DXMATRIX	 Rot;
	
	float		 Rad = 360.0f / ( float ) Div; 
	
	Rad = D3DXToRadian( Rad );
#ifdef BATCH_RENDER
	VERTEX_XYZ_DIFFUSE_TEX1* pRealVertice = pVertice;
#endif	//	BATCH_RENDER
	for(int i = 0 ; i < Div ; ++i )
	{
		D3DXMatrixRotationY( &Rot , ( Rad * i ) );
		
		D3DXVec3TransformCoord( &pVertice->v , &vPos0 , &Rot );
		
		pVertice->tu = 0.0f;
		pVertice->tv = 1.0f;
		pVertice++;
		
		D3DXVec3TransformCoord( &pVertice->v , &vPos1 , &Rot );
		
		pVertice->tu = 0.0f;
		pVertice->tv = 0.0f;
		pVertice++;
		
		D3DXVec3TransformCoord( &pVertice->v , &vPos2 , &Rot );
		
		pVertice->tu = 1.0f;
		pVertice->tv = 1.0f;
		pVertice++;
		
		D3DXVec3TransformCoord( &pVertice->v , &vPos3 , &Rot );
		
		pVertice->tu = 1.0f;
		pVertice->tv = 0.0f;
		pVertice++;
	}
#ifdef BATCH_RENDER
	memcpy( m_pVertices[eBDType_Spot2], pRealVertice, 
		sizeof(VERTEX_XYZ_DIFFUSE_TEX1) * m_pVerticesCount[eBDType_Spot2]);
#endif	//	BATCH_RENDER
	m_pVB[14]->Unlock();
	
	return TRUE;	
}

// 15. 칼흔
int	CBillboardManager::MakeSword(void)
{
	//............................................................................................................
	// 인덱스 버퍼 만들기
	//............................................................................................................
	if( FAILED( 
		g_RenderManager.CreateIndexBuffer( ( sizeof( WORD ) * 12 )   ,
		D3DUSAGE_WRITEONLY	     ,
		D3DFMT_INDEX16 		 ,
		D3DPOOL_MANAGED		 ,
		&m_pIB[15] )) )
	{
		return FALSE;		
	}
#ifdef BATCH_RENDER
	m_pIndicesCount[eBDType_Sword]  = 12;
	m_pVerticesCount[eBDType_Sword]	= 8;
	m_pIndices[eBDType_Sword]	= SAFE_NEW_ARRAY( WORD , m_pIndicesCount[eBDType_Sword] );
	m_pVertices[eBDType_Sword]	= SAFE_NEW_ARRAY( VERTEX_XYZ_DIFFUSE_TEX1, m_pVerticesCount[eBDType_Sword] );
#endif	//	BATCH_RENDER
	WORD   *pIndexBuffer = NULL;
#ifdef DIRECT_VERSION_9_MJH
	if( FAILED ( m_pIB[15]->Lock(0, 0, (void**)&pIndexBuffer, 0 ) ) )
#else
	if( FAILED ( m_pIB[15]->Lock(0, 0, (BYTE**)&pIndexBuffer, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH	
	{
		return FALSE;
	}
	
	for( int i = 0; i < 2 ; ++i )
	{
		pIndexBuffer[ 6 * i + 0 ] = (WORD) 4 * i + 0;
		pIndexBuffer[ 6 * i + 1 ] = (WORD) 4 * i + 1;
		pIndexBuffer[ 6 * i + 2 ] = (WORD) 4 * i + 2;
		pIndexBuffer[ 6 * i + 3 ] = (WORD) 4 * i + 1;
		pIndexBuffer[ 6 * i + 4 ] = (WORD) 4 * i + 3;
		pIndexBuffer[ 6 * i + 5 ] = (WORD) 4 * i + 2;
	}
#ifdef BATCH_RENDER
	memcpy( m_pIndices[eBDType_Sword], pIndexBuffer, sizeof(WORD) * m_pIndicesCount[eBDType_Sword]);
#endif	//	BATCH_RENDER
	m_pIB[15]->Unlock();
	
	
	//............................................................................................................
	// 버텍스 버퍼 만들기
	//............................................................................................................
#ifdef BATCH_RENDER
	if( FAILED(g_RenderManager.CreateVertexBuffer( m_pVerticesCount[eBDType_Sword] * sizeof(VERTEX_XYZ_DIFFUSE_TEX1),
		D3DUSAGE_WRITEONLY, D3DFVF_VERTEX_XYZ_DIFFUSE_TEX1,
		D3DPOOL_MANAGED, &m_pVB[eBDType_Sword] ) ) )
	{
		return E_FAIL;
	}
	VERTEX_XYZ_DIFFUSE_TEX1* pVertice;
#else	//	BATCH_RENDER
	if( FAILED( g_RenderManager.CreateVertexBuffer( ( sizeof( BILLBOARDVERTEX ) * 8 ) ,
		D3DUSAGE_WRITEONLY, D3DFVF_BILLBOARDVERTEX ,
		D3DPOOL_MANAGED, &m_pVB[15] ) ) )
	{
		return FALSE;		
	}
	BILLBOARDVERTEX   *pVertice = NULL;
#endif	//	BATCH_RENDER
#ifdef DIRECT_VERSION_9_MJH
	if( FAILED ( m_pVB[15]->Lock(0, 0, (void**)&pVertice, 0 ) ) )
#else
	if( FAILED ( m_pVB[15]->Lock(0, 0, (BYTE**)&pVertice, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH	
	{
		return FALSE;
	}
	
	D3DXVECTOR3	 vPos0(  0.0f , 0.0f , 0.0f );
	D3DXVECTOR3	 vPos1(  0.0f , 1.0f , 0.0f );
	D3DXVECTOR3	 vPos2(  0.0f , 0.0f , 1.0f );
	D3DXVECTOR3	 vPos3(  0.0f , 1.0f , 1.0f );
	
	D3DXMATRIX	 Rot;
	
	float		 Rad = 30.0f; 
	
	Rad = D3DXToRadian( Rad );
#ifdef BATCH_RENDER
	VERTEX_XYZ_DIFFUSE_TEX1* pRealVertice = pVertice;
#endif	//	BATCH_RENDER
	D3DXMatrixRotationY( &Rot , Rad );
	D3DXVec3TransformCoord( &pVertice->v , &vPos0 , &Rot );
	
	pVertice->tu = 0.0f;
	pVertice->tv = 1.0f;
	pVertice++;
	
	D3DXVec3TransformCoord( &pVertice->v , &vPos1 , &Rot );
	
	pVertice->tu = 0.0f;
	pVertice->tv = 0.0f;
	pVertice++;
	
	D3DXVec3TransformCoord( &pVertice->v , &vPos2 , &Rot );
	
	pVertice->tu = 1.0f;
	pVertice->tv = 1.0f;
	pVertice++;
	
	D3DXVec3TransformCoord( &pVertice->v , &vPos3 , &Rot );
	
	pVertice->tu = 1.0f;
	pVertice->tv = 0.0f;
	pVertice++;
	
	
	D3DXMatrixRotationY( &Rot , -Rad );
	D3DXVec3TransformCoord( &pVertice->v , &vPos0 , &Rot );
	
	pVertice->tu = 0.0f;
	pVertice->tv = 1.0f;
	pVertice++;
	
	D3DXVec3TransformCoord( &pVertice->v , &vPos1 , &Rot );
	
	pVertice->tu = 0.0f;
	pVertice->tv = 0.0f;
	pVertice++;
	
	D3DXVec3TransformCoord( &pVertice->v , &vPos2 , &Rot );
	
	pVertice->tu = 1.0f;
	pVertice->tv = 1.0f;
	pVertice++;
	
	D3DXVec3TransformCoord( &pVertice->v , &vPos3 , &Rot );
	
	pVertice->tu = 1.0f;
	pVertice->tv = 0.0f;
	pVertice++;
#ifdef BATCH_RENDER
	memcpy( m_pVertices[eBDType_Sword], pRealVertice, 
		sizeof(VERTEX_XYZ_DIFFUSE_TEX1) * m_pVerticesCount[eBDType_Sword]);
#endif	//	BATCH_RENDER
	m_pVB[15]->Unlock();
	
	return TRUE;
}

// 16. 크로스 평면
int CBillboardManager::MakeCrossPlane(void)
{
	//............................................................................................................
	// 인덱스 버퍼 만들기
	//............................................................................................................
	if( FAILED( 
		g_RenderManager.CreateIndexBuffer( ( sizeof( WORD ) * 12 )   ,
		D3DUSAGE_WRITEONLY	     ,
		D3DFMT_INDEX16 		 ,
		D3DPOOL_MANAGED		 ,
		&m_pIB[16] )) )
	{
		return FALSE;		
	}
#ifdef BATCH_RENDER
	m_pIndicesCount[eBDType_CrossPlane]  = 12;
	m_pVerticesCount[eBDType_CrossPlane]	= 8;
	m_pIndices[eBDType_CrossPlane]	= SAFE_NEW_ARRAY( WORD , m_pIndicesCount[eBDType_CrossPlane] );
	m_pVertices[eBDType_CrossPlane]	= SAFE_NEW_ARRAY( VERTEX_XYZ_DIFFUSE_TEX1, m_pVerticesCount[eBDType_CrossPlane] );
#endif	//	BATCH_RENDER
	WORD   *pIndexBuffer = NULL;
#ifdef DIRECT_VERSION_9_MJH
	if( FAILED ( m_pIB[16]->Lock(0, 0, (void**)&pIndexBuffer, 0 ) ) )
#else
	if( FAILED ( m_pIB[16]->Lock(0, 0, (BYTE**)&pIndexBuffer, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH	
	{
		return FALSE;
	}
	
	for( int i = 0; i < 2 ; ++i )
	{
		pIndexBuffer[ 6 * i + 0 ] = (WORD) 4 * i + 0;
		pIndexBuffer[ 6 * i + 1 ] = (WORD) 4 * i + 1;
		pIndexBuffer[ 6 * i + 2 ] = (WORD) 4 * i + 2;
		pIndexBuffer[ 6 * i + 3 ] = (WORD) 4 * i + 1;
		pIndexBuffer[ 6 * i + 4 ] = (WORD) 4 * i + 3;
		pIndexBuffer[ 6 * i + 5 ] = (WORD) 4 * i + 2;
	}
#ifdef BATCH_RENDER
	memcpy( m_pIndices[eBDType_CrossPlane], pIndexBuffer, sizeof(WORD) * m_pIndicesCount[eBDType_CrossPlane]);
#endif	//	BATCH_RENDER
	m_pIB[16]->Unlock();
	
	
	//............................................................................................................
	// 버텍스 버퍼 만들기
	//............................................................................................................
#ifdef BATCH_RENDER
	if( FAILED(g_RenderManager.CreateVertexBuffer( m_pVerticesCount[eBDType_CrossPlane] * sizeof(VERTEX_XYZ_DIFFUSE_TEX1),
		D3DUSAGE_WRITEONLY, D3DFVF_VERTEX_XYZ_DIFFUSE_TEX1,
		D3DPOOL_MANAGED, &m_pVB[eBDType_CrossPlane] ) ) )
	{
		return E_FAIL;
	}
	VERTEX_XYZ_DIFFUSE_TEX1* pVertice;
#else	//	BATCH_RENDER
	if( FAILED( g_RenderManager.CreateVertexBuffer( ( sizeof( BILLBOARDVERTEX ) * 8 ) ,
		D3DUSAGE_WRITEONLY, D3DFVF_BILLBOARDVERTEX ,
		D3DPOOL_MANAGED, &m_pVB[16] ) ) )
	{
		return FALSE;		
	}
	BILLBOARDVERTEX   *pVertice = NULL;
#endif	//	BATCH_RENDER
#ifdef DIRECT_VERSION_9_MJH
	if( FAILED ( m_pVB[16]->Lock(0, 0, (void**)&pVertice, 0 ) ) )
#else
	if( FAILED ( m_pVB[16]->Lock(0, 0, (BYTE**)&pVertice, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH	
	{
		return FALSE;
	}
	
	D3DXVECTOR3	 vPos0(  -0.5f ,  0.0f , 0.0f );
	D3DXVECTOR3	 vPos1(  -0.5f ,  1.0f , 0.0f );
	D3DXVECTOR3	 vPos2(   0.5f ,  0.0f , 0.0f );
	D3DXVECTOR3	 vPos3(   0.5f ,  1.0f , 0.0f );
	
	D3DXMATRIX	 Rot;
	
	float		 Rad = 90.0f; 
	
	Rad = D3DXToRadian( Rad );
#ifdef BATCH_RENDER
	VERTEX_XYZ_DIFFUSE_TEX1* pRealVertice = pVertice;
#endif	//	BATCH_RENDER
	pVertice->v = vPos0;
	
	pVertice->tu = 0.0f;
	pVertice->tv = 1.0f;
	pVertice++;
	
	pVertice->v = vPos1;
	
	pVertice->tu = 0.0f;
	pVertice->tv = 0.0f;
	pVertice++;
	
	pVertice->v = vPos2;
	
	pVertice->tu = 1.0f;
	pVertice->tv = 1.0f;
	pVertice++;
	
	pVertice->v = vPos3;
	
	pVertice->tu = 1.0f;
	pVertice->tv = 0.0f;
	pVertice++;
	
	
	D3DXMatrixRotationY( &Rot , Rad );
	D3DXVec3TransformCoord( &pVertice->v , &vPos0 , &Rot );
	
	pVertice->tu = 0.0f;
	pVertice->tv = 1.0f;
	pVertice++;
	
	D3DXVec3TransformCoord( &pVertice->v , &vPos1 , &Rot );
	
	pVertice->tu = 0.0f;
	pVertice->tv = 0.0f;
	pVertice++;
	
	D3DXVec3TransformCoord( &pVertice->v , &vPos2 , &Rot );
	
	pVertice->tu = 1.0f;
	pVertice->tv = 1.0f;
	pVertice++;
	
	D3DXVec3TransformCoord( &pVertice->v , &vPos3 , &Rot );
	
	pVertice->tu = 1.0f;
	pVertice->tv = 0.0f;
	pVertice++;
	
#ifdef BATCH_RENDER
	memcpy( m_pVertices[eBDType_CrossPlane], pRealVertice, 
		sizeof(VERTEX_XYZ_DIFFUSE_TEX1) * m_pVerticesCount[eBDType_CrossPlane]);
#endif	//	BATCH_RENDER

	m_pVB[16]->Unlock();
	
	return TRUE;
}

// 17. 실린더 각진거 6각
int CBillboardManager::MakeCylinder6(void)
{
	int    Div         = 6;
	float  Radius0     = 1.0f;
	float  Radius1     = 1.0f;
	int    FaceCount   = Div * 2;
	int    IndexCount  = FaceCount + 2;
	int    VertexCount = ( Div + 1 ) * 2;
	
	//............................................................................................................
	// 인덱스 버퍼 만들기
	//............................................................................................................
	if( FAILED( 
		g_RenderManager.CreateIndexBuffer( ( sizeof( WORD ) * IndexCount )   ,
		D3DUSAGE_WRITEONLY	     ,
		D3DFMT_INDEX16 		 ,
		D3DPOOL_MANAGED		 ,
		&m_pIB[17] )) )
	{
		return FALSE;		
	}
	
	WORD   *pIndexBuffer = NULL;
#ifdef DIRECT_VERSION_9_MJH
	if( FAILED ( m_pIB[17]->Lock(0, 0, (void**)&pIndexBuffer, 0 ) ) )
#else
	if( FAILED ( m_pIB[17]->Lock(0, 0, (BYTE**)&pIndexBuffer, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH	
	{
		return FALSE;
	}
	
	for( int i = 0; i < IndexCount ; ++i )
	{
		pIndexBuffer[ i ] = (WORD) i;
	}
	
	m_pIB[17]->Unlock();
	
	
	//............................................................................................................
	// 버텍스 버퍼 만들기
	//............................................................................................................
	BILLBOARDVERTEX	*pVertexList = NULL;
	
	if( FAILED( g_RenderManager.CreateVertexBuffer( ( sizeof( BILLBOARDVERTEX ) * VertexCount ) ,
		D3DUSAGE_WRITEONLY, D3DFVF_BILLBOARDVERTEX ,
		D3DPOOL_MANAGED, &m_pVB[17] ) ) )
	{
		return FALSE;		
	}
	
	BILLBOARDVERTEX   *pVertice = NULL;
#ifdef DIRECT_VERSION_9_MJH
	if( FAILED ( m_pVB[17]->Lock(0, 0, (void**)&pVertice, 0 ) ) )
#else
	if( FAILED ( m_pVB[17]->Lock(0, 0, (BYTE**)&pVertice, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH	
	{
		return FALSE;
	}
	
	D3DXVECTOR3	 vPos0( Radius0 , 0.0f , 0.0f );
	D3DXVECTOR3	 vPos1( Radius1 , 1.0f , 0.0f );
	D3DXMATRIX	 RotY;
	
	float		 RadY = 360.0f / ( float ) Div; 
	
	RadY = D3DXToRadian( RadY );
	
	for(int i = 0 ; i < ( Div + 1 ) ; ++i )
	{
		D3DXMatrixRotationY( &RotY , ( RadY * i ) );
		
		D3DXVec3TransformCoord( &pVertice->v , &vPos0 , &RotY );
		
		pVertice->tu = i;
		pVertice->tv = 1.0f;
		pVertice++;
		
		D3DXVec3TransformCoord( &pVertice->v , &vPos1 , &RotY );
		
		pVertice->tu = i;
		pVertice->tv = 0.0f;
		
		pVertice++;
	}
	
	m_pVB[17]->Unlock();
	
	return TRUE;
}

// 18. 실린더 각진거 8각
int	CBillboardManager::MakeCylinder8(void)
{
	int    Div         = 8;
	float  Radius0     = 1.0f;
	float  Radius1     = 1.0f;
	int    FaceCount   = Div * 2;
	int    IndexCount  = FaceCount + 2;
	int    VertexCount = ( Div + 1 ) * 2;
	
	//............................................................................................................
	// 인덱스 버퍼 만들기
	//............................................................................................................
	if( FAILED( 
		g_RenderManager.CreateIndexBuffer( ( sizeof( WORD ) * IndexCount )   ,
		D3DUSAGE_WRITEONLY	     ,
		D3DFMT_INDEX16 		 ,
		D3DPOOL_MANAGED		 ,
		&m_pIB[18] )) )
	{
		return FALSE;		
	}
	
	WORD   *pIndexBuffer = NULL;
#ifdef DIRECT_VERSION_9_MJH
	if( FAILED ( m_pIB[18]->Lock(0, 0, (void**)&pIndexBuffer, 0 ) ) )
#else
	if( FAILED ( m_pIB[18]->Lock(0, 0, (BYTE**)&pIndexBuffer, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH	
	{
		return FALSE;
	}
	
	for( int i = 0; i < IndexCount ; ++i )
	{
		pIndexBuffer[ i ] = (WORD) i;
	}
	
	m_pIB[18]->Unlock();
	
	
	//............................................................................................................
	// 버텍스 버퍼 만들기
	//............................................................................................................
	BILLBOARDVERTEX	*pVertexList = NULL;
	
	if( FAILED( g_RenderManager.CreateVertexBuffer( ( sizeof( BILLBOARDVERTEX ) * VertexCount ) ,
		D3DUSAGE_WRITEONLY, D3DFVF_BILLBOARDVERTEX ,
		D3DPOOL_MANAGED, &m_pVB[18] ) ) )
	{
		return FALSE;		
	}
	
	BILLBOARDVERTEX   *pVertice = NULL;
#ifdef DIRECT_VERSION_9_MJH
	if( FAILED ( m_pVB[18]->Lock(0, 0, (void**)&pVertice, 0 ) ) )
#else
	if( FAILED ( m_pVB[18]->Lock(0, 0, (BYTE**)&pVertice, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH	
	{
		return FALSE;
	}
	
	D3DXVECTOR3	 vPos0( Radius0 , 0.0f , 0.0f );
	D3DXVECTOR3	 vPos1( Radius1 , 1.0f , 0.0f );
	D3DXMATRIX	 RotY;
	
	float		 RadY = 360.0f / ( float ) Div; 
	
	RadY = D3DXToRadian( RadY );
	
	for(int i = 0 ; i < ( Div + 1 ) ; ++i )
	{
		D3DXMatrixRotationY( &RotY , ( RadY * i ) );
		
		D3DXVec3TransformCoord( &pVertice->v , &vPos0 , &RotY );
		
		pVertice->tu = i;
		pVertice->tv = 1.0f;
		pVertice++;
		
		D3DXVec3TransformCoord( &pVertice->v , &vPos1 , &RotY );
		
		pVertice->tu = i;
		pVertice->tv = 0.0f;
		
		pVertice++;
	}
	
	m_pVB[18]->Unlock();
	
	return TRUE;
}

