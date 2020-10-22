#include "RockPCH.h"
#include "..\\RockClient.h"
#include "..\\stringmanager.h"
#include "..\\brother.h"


#include "..\\quadlist.h"
#include "..\\bravolibs\\network\\Protocol.h"
#include "..\\Pc.h"

#include "..\\Map.h"
#include "..\\effect.h"
#include "CChangeClass.h"
#include "..\\CRenderManager.h"


CChangeClass::CChangeClass()
{
	GetAniIndex();
	
	m_pBGTexture = NULL;
	m_pBGModel   = NULL;
	m_pBGImageTexture = NULL;

	VFileHandle* pFH = g_VFEffectStatic.OpenFile( "star.jpg" );	

	if( pFH )
	{
		if( FAILED ( g_RenderManager.CreateTextureFromFileInMemoryEx( pFH->GetData(), pFH->GetSize(),	/// 데이터 포인터, 사이즈 
																	D3DX_DEFAULT, D3DX_DEFAULT, 
																	1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
																	D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
																	D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
																	0, NULL, NULL, &m_pBGTexture ) ) )
		{
			g_VFEffectStatic.CloseFile( pFH );	/// 파일 반드시 닫아야 한다. 

			return;
		}			

		g_VFEffectStatic.CloseFile( pFH );	/// 파일 반드시 닫아야 한다. 
	}

	pFH = g_VFEffectStatic.OpenFile( "starbg.jpg" );	

	if( pFH )
	{
		if( FAILED ( g_RenderManager.CreateTextureFromFileInMemoryEx( pFH->GetData(), pFH->GetSize(),	/// 데이터 포인터, 사이즈 
																	D3DX_DEFAULT, D3DX_DEFAULT, 
																	1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
																	D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
																	D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
																	0, NULL, NULL, &m_pBGImageTexture ) ) )
		{
			g_VFEffectStatic.CloseFile( pFH );	/// 파일 반드시 닫아야 한다. 

			return;
		}			

		g_VFEffectStatic.CloseFile( pFH );	/// 파일 반드시 닫아야 한다. 
	}

	m_pBGModel = SAFE_NEW( Model );	

	if( !m_pBGModel->ReadVF( &g_VFEffectStatic, "star.msh" , TRUE ) )
		return;

	m_fRotAngle    = 0.0f;

#ifdef HHM_USER_RESOLUTION_SELECT// 기존 선택된 해상도(2009/04/24)
	
	m_fWidthRange  = nRui->Desktop.Width / 3.0f;
	
#else
	
	m_fWidthRange  = 1024.0f / 3.0f;
	
#endif// #ifdef HHM_USER_RESOLUTION_SELECT// 기존 선택된 해상도(2009/04/24)

	m_fWidthRange1 = m_fWidthRange * 2.0f;

	m_fThreeRange = 1.0f    / 3.0f;
	m_fTwoRange   = 2.0f    / 3.0f;
	
	bool bIsBeginScene = g_RockClient.m_bIsBeginScene;	
	if( bIsBeginScene )
	{
		g_RockClient.EndScene();
	}

	for( int i = 0 ; i < 2 ; i++ )
	{
		m_SurfaceList[ i ].Init( 1024 , 1024 , g_RockClient.m_lpD3D , g_lpDevice );
	}
	
	if( bIsBeginScene )
	{
		g_RockClient.BeginScene();
	}

	m_pCrackList = NULL;

	MakeCrackPlane();
	
	m_ISMakeBackImage = TRUE;
}

CChangeClass::~CChangeClass()
{
	DeleteAllMemory();
}

void CChangeClass::DeleteAllMemory(void)
{
	if( NULL != m_pBGTexture )
	{
		m_pBGTexture->Release();
		m_pBGTexture = NULL;
	}

	SAFE_DELETE(m_pBGModel);
	SAFE_DELETE_ARRAY(m_pCrackList);

	if( m_pBGImageTexture != NULL )
	{
		m_pBGImageTexture->Release();

		m_pBGImageTexture = NULL;
	}

}


//..........................................................................................................
// Fade 관련
//..........................................................................................................
int CChangeClass::Squence00()
{
	g_RockClient.m_SceneManager.SetGUIDisplay( FALSE );		
	g_RockClient.m_SceneManager.SetSoundPlayMode( SCENE_SQUENCE );
	StopAllSound();

	UpdateSound( 278 );
	UpdateSound( 279 );
	UpdateSound( 280 );
	UpdateSound( 281 );
	UpdateSound( 282 );
	UpdateSound( 283 );

	m_SaveCamera = g_Camera.GetSaveVlue();
	
	SetPlayerAni( n_Stand_Peace9 );
	g_Particle.m_Fade.Start( FADE_STATE_FADE , D3DCOLOR_ARGB( 0 , 0 , 0 , 0  ), 
							 D3DCOLOR_ARGB( 255 , 0 , 0 , 0 ) , 500 , FALSE );

	
	SetWaitFlag( SWT_WAIT );

	return TRUE;
}

int CChangeClass::SquenceCheck00()
{
	if( g_Particle.m_Fade.GetColor() == D3DCOLOR_ARGB( 255 , 0 , 0 , 0 ) )
	{
		SetStartScene();

		g_Particle.m_Fade.Start( FADE_STATE_FADE , D3DCOLOR_ARGB( 255 , 0 , 0 , 0 ) ,
								 D3DCOLOR_ARGB( 0 , 0 , 0 , 0  ), 500 , FALSE );

		m_matPos  = g_Pc.GetPlayer()->GetPosTM();

		g_Pc.GetPlayer()->m_Mov.Clear();

		D3DXMATRIX matIdentity;

		D3DXMatrixIdentity( &matIdentity );
		
		g_Pc.GetPlayer()->SetPosTM( matIdentity );

		g_Camera.SetStartFrame( 11 , g_lpDevice );

		EffectLock();

		g_Particle.SetEffSetCon( 0.0f , 702 , ESLT_POS , EFFECT_SET_LOOP_TIME , -1 , -1 , D3DXVECTOR3( 
			0.0f ,0.0f ,0.0f ) );

		EffectUnLock();

		NextSquence();
	}

	return TRUE;
}

int CChangeClass::Squence01()
{
	SetWaitFlag( SWT_WAIT );

	return TRUE;
}

int CChangeClass::SquenceCheck01()
{
	if( g_Particle.m_Fade.GetColor() == D3DCOLOR_ARGB( 0 , 0 , 0 , 0 ) )
	{
		MakeBackImage();
		
		g_Camera.SetStartFrame( 11 , g_lpDevice );
		
		NextSquence();
	}

	return TRUE;
}

int CChangeClass::SquenceFirstScene01()
{
	DisplayBG();

	return TRUE;
}

//..........................................................................................................
// Scene 02
//..........................................................................................................
int CChangeClass::SquenceFirstScene02()
{
	DisplayBG();
	
	return TRUE;
}


int CChangeClass::Squence02()
{
	SetPlayerAni( m_AniIndex[ 0 ] );
	g_Camera.SetAnimation( 11 );
	PlaySound( 278 , NULL );
	SetWaitFlag( SWT_WAIT );

	return TRUE;
}


int CChangeClass::SquenceCheck02()
{
	return TRUE;
}

int CChangeClass::SquenceAniFrame02()
{
	g_RockClient.m_SceneManager.SetFixFaceExp( CFaceExpData::FACE_EXP_MAXCOUNT );

	if( g_Pc.GetPlayer()->m_ani_frame < 86 )
	{
		g_RockClient.m_SceneManager.SetFixFaceExp( CFaceExpData::FACE_EXP_TWINKLE );
	}
	else if( g_Pc.GetPlayer()->m_ani_frame == 142 )
	{
/*
		if( nRui->thePcParam.Gender == MALE )
		{
			PlaySound( 279 , NULL );
		}
		else
		{
			PlaySound( 280 , NULL );
		}
*/
		NextSquence();
	}

	return TRUE;
}


//..........................................................................................................
// Scene 03
//..........................................................................................................
int CChangeClass::SquenceFirstScene03()
{
	DisplayBG();
	return TRUE;
}

int CChangeClass::Squence03()
{
/*
	g_Particle.m_Fade.Start(FADE_STATE_LOOP,D3DCOLOR_ARGB( 0 , 0 , 0 , 0 ),
											D3DCOLOR_ARGB( 100 , 255 , 255 , 255 ) ,
											D3DCOLOR_ARGB( 0 , 0 , 0 , 0 ),
											100,100,FALSE);
*/
	SetWaitFlag( SWT_TIMER , 100 );

	return TRUE;
}

int CChangeClass::SquenceCheck03()
{
	return TRUE;
}

//..........................................................................................................
// Scene 04
//..........................................................................................................
int CChangeClass::SquenceFirstScene04()
{
	DisplayBG();

	return TRUE;
}

int CChangeClass::Squence04()
{

	g_Camera.SetEndFrame( 11 , g_lpDevice );

	PlaySound( 281 , NULL );
	SetWaitFlag( SWT_TIMER , 300 );

	return TRUE;
}

int CChangeClass::SquenceCheck04()
{
	if( m_SurfaceList[ 0 ].GetReady() && m_ISMakeBackImage == TRUE )
	{
		DrawTexture( 0 , 0 , m_fWidthRange , 768.0f , 
			0 , 0 , m_fThreeRange , 0.75f , D3DCOLOR_XRGB( 255 , 255 , 255 ) , 
			m_SurfaceList[ 0 ].GetTexture() , g_lpDevice );
	}

	return TRUE;
}

//..........................................................................................................
// Scene 05
//..........................................................................................................
int CChangeClass::SquenceFirstScene05()
{
	DisplayBG();

	return TRUE;
}

int CChangeClass::Squence05()
{

	g_Camera.SetEndFrame( 11 , g_lpDevice );

	PlaySound( 281 , NULL );
	SetWaitFlag( SWT_TIMER , 300 );

	return TRUE;
}

int CChangeClass::SquenceCheck05()
{
	if( m_SurfaceList[ 0 ].GetReady() && m_ISMakeBackImage == TRUE )
	{
		DrawTexture( 0 , 0 , m_fWidthRange1 , 768 , 
			0 , 0 , m_fTwoRange , 0.75f , D3DCOLOR_XRGB( 255 , 255 , 255 ) , 
			m_SurfaceList[ 0 ].GetTexture() , g_lpDevice );
	}

	return TRUE;
}

//..........................................................................................................
// Scene 06
//..........................................................................................................
int CChangeClass::Squence06()
{
	PlaySound( 281 , NULL );
	SetWaitFlag( SWT_TIMER , 500 );

	return TRUE;
}

int CChangeClass::SquenceCheck06()
{
	if( m_SurfaceList[ 0 ].GetReady() && m_ISMakeBackImage == TRUE )
	{
#ifdef HHM_USER_RESOLUTION_SELECT// 기존 선택된 해상도(2009/04/24)
		
		DrawTexture( 0.0f , 0.0f , nRui->Desktop.Width, nRui->Desktop.Height ,
			
#else
			
		DrawTexture( 0.0f , 0.0f , 1024.0f , 768.0f ,
			
#endif// #ifdef HHM_USER_RESOLUTION_SELECT// 기존 선택된 해상도(2009/04/24)

		0.0f , 0.0f , 1.0f , 0.75f , D3DCOLOR_XRGB( 255 , 255 , 255 ) , 
			m_SurfaceList[ 0 ].GetTexture() , g_lpDevice );
	}

	return TRUE;
}

//..........................................................................................................
// Scene 7
//..........................................................................................................
int CChangeClass::SquenceFirstScene07()
{
	DisplayBG();

	return TRUE;
}

int CChangeClass::Squence07()
{
	g_Camera.SetStartFrame( 11 , g_lpDevice );
	g_Particle.DelEffSetCon( 702 , ESLT_POS , -1 , -1 );

	PlaySound( 282 , NULL );
	PlaySound( 283 , NULL );

	EffectLock();
	g_Particle.SetEffSetCon( 0.2f , 703 , 100 , 3.0f , -1 , -1 , D3DXVECTOR3( 0.0f , 0.0f ,0.0f ) );
	EffectUnLock();
/*	
	g_Particle.m_Fade.Start(FADE_STATE_LOOP,D3DCOLOR_ARGB( 0 , 0 , 0 , 0 ),
											D3DCOLOR_ARGB( 100 , 255 , 255 , 255 ) ,
											D3DCOLOR_ARGB( 0 , 0 , 0 , 0 ),
											100,100,FALSE);
*/
	SetWaitFlag( SWT_TIMER , 2000 );
//	SetWaitFlag( SWT_TIMER , 20000 );

	return TRUE;
}

int CChangeClass::SquenceCheck07()
{
//	DrawPlayer();

	HRESULT hr;

	bool bIsBeginScene = g_RockClient.m_bIsBeginScene;	
	if( bIsBeginScene )
	{
		g_RockClient.EndScene();
	}			
	
	if( SUCCEEDED( hr = m_SurfaceList[ 1 ].BeginScene() ) )
	{
		D3DXVECTOR3 vEye    = g_Camera.m_vEye;
		D3DXVECTOR3 vLookAt = g_Camera.m_vLookat;
		D3DXVECTOR3 vUp     = g_Camera.m_vUp;
		
		D3DXMATRIX  matView;	
		
		D3DXVECTOR3 vTempEye( 0.0f , 0.0f , -1.0f );
		D3DXVECTOR3 vTempLookAt( 0.0f , 0.0f , 0.0f );
		D3DXVECTOR3 vTempUp( 0.0f , 1.0f , 0.0f );
		D3DXMATRIX matProj;
		
		D3DXMatrixLookAtLH( &matView , &vTempEye, &vTempLookAt, &vTempUp );
		g_RenderManager.SetTransform(D3DTS_VIEW , &matView);

	#ifdef HHM_USER_RESOLUTION_SELECT// 기존 선택된 해상도(2009/04/24)
		
		D3DXMatrixOrthoLH( &matProj , nRui->Desktop.Width , nRui->Desktop.Height , 0.0f , 10000.0f );
		
	#else
		
		D3DXMatrixOrthoLH( &matProj , 1024.0f , 768.0f , 0.0f , 10000.0f );

	#endif// #ifdef HHM_USER_RESOLUTION_SELECT// 기존 선택된 해상도(2009/04/24)

		D3DXMATRIX mtrOffset; 
		D3DXMatrixTranslation( &mtrOffset, -0.5, -0.5, 0.0 ); 
		matProj = mtrOffset * matProj; 
		
		g_RenderManager.SetTransform(D3DTS_PROJECTION, &matProj );
		
		g_RenderManager.Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER , D3DCOLOR_ARGB(0,0,0,0), 1.0f, 0 );		
		
		DrawCrack();
		
		g_Camera.SetCamera( vEye , vLookAt , vUp );
		g_RenderManager.SetTransform(D3DTS_PROJECTION, &g_Camera.m_matProj );
		
		m_SurfaceList[ 1 ].EndScene();
	}

	if( bIsBeginScene )
	{
		g_RockClient.BeginScene();
	}

	if( SUCCEEDED( hr ) )
	{
		g_RenderManager.SetAlphaTestEnable( TRUE );
		g_RenderManager.SetAlphaFunc( D3DCMP_NOTEQUAL );
		g_RenderManager.SetAlphaRef( 0 );
		
		g_RenderManager.SetAlphaBlendEnable( TRUE );
		g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
		g_RenderManager.SetDestBlend( D3DBLEND_INVSRCALPHA );

	#ifdef HHM_USER_RESOLUTION_SELECT// 기존 선택된 해상도(2009/04/24)
		
		DrawTexture( 0.0f , 0.0f , nRui->Desktop.Width , nRui->Desktop.Height ,
			
	#else
			
		DrawTexture( 0.0f , 0.0f , 1024.0f , 768.0f ,
			
	#endif// #ifdef HHM_USER_RESOLUTION_SELECT// 기존 선택된 해상도(2009/04/24)

			0.0f , 0.0f , 1.0f , 0.75f , D3DCOLOR_ARGB( 255 , 255 , 255 , 255 ) , 
			m_SurfaceList[ 1 ].GetTexture() , g_lpDevice );
		
		g_RenderManager.SetAlphaTestEnable( FALSE );
		g_RenderManager.SetAlphaBlendEnable( FALSE );
	}

	return TRUE;
}

//..........................................................................................................
// Fade 관련
//..........................................................................................................
int CChangeClass::SquenceFirstScene08()
{
	DisplayBG();

	return TRUE;
}

int CChangeClass::Squence08()
{
	g_Particle.m_Fade.Start( FADE_STATE_FADE , D3DCOLOR_ARGB( 0 , 0 , 0 , 0  ), 
							 D3DCOLOR_ARGB( 255 , 0 , 0 , 0 ) , 1000 , FALSE );

	
	SetWaitFlag( SWT_WAIT );

	return TRUE;
}

int CChangeClass::SquenceCheck08()
{
	if( g_Particle.m_Fade.GetColor() == D3DCOLOR_ARGB( 255 , 0 , 0 , 0 ) )
	{
		g_Particle.m_Fade.Start( FADE_STATE_FADE , D3DCOLOR_ARGB( 255 , 0 , 0 , 0 ) ,
								 D3DCOLOR_ARGB( 0 , 0 , 0 , 0  ), 1000 , FALSE );

		g_Pc.GetPlayer()->SetPosTM( m_matPos );

		NextSquence();

		SetPlayerAni( n_Stand_Peace9 );
	}

	return TRUE;
}

int CChangeClass::Squence09()
{
	SetWaitFlag( SWT_WAIT );

	return TRUE;
}



int CChangeClass::SquenceCheck09()
{
	if( g_Particle.m_Fade.GetColor() == D3DCOLOR_ARGB( 0 , 0 , 0 , 0 ) )
	{
		SetEndScene();

		NextSquence();

		g_Camera.SetSaveVlue( m_SaveCamera );

		g_RockClient.m_SceneManager.SetSoundPlayMode( SCENE_NORMAL );

	}

	return TRUE;
}
//..........................................................................................................

void CChangeClass::SetStartScene(void)
{
	g_RockClient.m_SceneManager.SetKeyInput( FALSE );
	g_RockClient.m_SceneManager.SetOBJDisplay( FALSE );		
	g_RockClient.m_SceneManager.SetNPCDisplay( FALSE );		
	g_RockClient.m_SceneManager.SetPlayerDisplay( TRUE );	
	g_RockClient.m_SceneManager.SetOtherPlayerDisplay( FALSE );	
	g_RockClient.m_SceneManager.SetTerrainDisplay( FALSE );		
	g_RockClient.m_SceneManager.SetCursorDisplay( FALSE );		
	g_RockClient.m_SceneManager.SetSkyBoxDisplay( FALSE );		
	g_RockClient.m_SceneManager.SetFogDisplay( FALSE );			
	g_RockClient.m_SceneManager.SetBGColor( D3DCOLOR_XRGB( 0 , 0 , 0 ) );
	g_RockClient.m_SceneManager.SetWeatherDisplay( FALSE );
	g_RockClient.m_SceneManager.SetInsectDisplay( FALSE );
	g_RockClient.m_SceneManager.SetSceneMode( SCENE_SQUENCE );
	g_RockClient.m_SceneManager.SetEffectDisplayMode( SCENE_SQUENCE );
}

void CChangeClass::SetEndScene(void)
{
	g_RockClient.m_SceneManager.SetKeyInput( TRUE );
	g_RockClient.m_SceneManager.SetGUIDisplay( TRUE );		
	g_RockClient.m_SceneManager.SetOBJDisplay( TRUE );		
	g_RockClient.m_SceneManager.SetNPCDisplay( TRUE );		
	g_RockClient.m_SceneManager.SetPlayerDisplay( TRUE );	
	g_RockClient.m_SceneManager.SetOtherPlayerDisplay( TRUE );	
	g_RockClient.m_SceneManager.SetTerrainDisplay( TRUE );		
	g_RockClient.m_SceneManager.SetCursorDisplay( TRUE );		
	g_RockClient.m_SceneManager.SetSkyBoxDisplay( TRUE );		
	g_RockClient.m_SceneManager.SetFogDisplay( TRUE );			
	g_RockClient.m_SceneManager.SetBGColor( D3DCOLOR_XRGB( 0 , 0 , 0 ) );
	g_RockClient.m_SceneManager.SetWeatherDisplay( TRUE );
	g_RockClient.m_SceneManager.SetInsectDisplay( TRUE );
	g_RockClient.m_SceneManager.SetSceneMode( SCENE_NORMAL );
	g_RockClient.m_SceneManager.SetEffectDisplayMode( SCENE_NORMAL );
}

void CChangeClass::DisplayBG(void)
{
	//......................................................................................................
	// Render State
	//......................................................................................................
	g_RenderManager.SetCullMode( D3DCULL_CCW );
	g_RenderManager.SetFillMode( D3DFILL_SOLID );
	g_RenderManager.SetFogEnable( FALSE );
	g_RenderManager.SetZFunc( D3DCMP_LESSEQUAL );
	g_RenderManager.SetAlphaTestEnable( FALSE );
	
	g_RenderManager.SetTextureMagFilter( 0 , D3DTEXF_LINEAR );
	g_RenderManager.SetTextureMinFilter( 0 , D3DTEXF_LINEAR );
	g_RenderManager.SetTextureMipFilter( 0 , D3DTEXF_LINEAR );

	g_RenderManager.SetTextureMagFilter( 1 , D3DTEXF_LINEAR );
	g_RenderManager.SetTextureMinFilter( 1 , D3DTEXF_LINEAR );
	g_RenderManager.SetTextureMipFilter( 1 , D3DTEXF_LINEAR );

	g_RenderManager.SetTextureTransFormFlags( 0 , D3DTTFF_DISABLE );
	g_RenderManager.SetTextureTransFormFlags( 1 , D3DTTFF_DISABLE );

	g_RenderManager.DisableTextureStageState( 1 );

	g_RenderManager.SetTextureColorOP( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureColorArg1( 0 , D3DTA_CURRENT );
	g_RenderManager.SetTextureColorArg2( 0 , D3DTA_TEXTURE );

	g_RenderManager.SetTextureAlphaOP( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureAlphaArg1( 0 , D3DTA_CURRENT );
	g_RenderManager.SetTextureAlphaArg2( 0 , D3DTA_TEXTURE );

	g_RenderManager.SetAlphaBlendEnable( TRUE );
	g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
	g_RenderManager.SetDestBlend( D3DBLEND_DESTALPHA );
	g_RenderManager.SetZEnable( FALSE );
	g_RenderManager.SetZWriteEnable( FALSE );

	g_RenderManager.SetLighting( TRUE );
	//......................................................................................................
	D3DXVECTOR3 vEye    = g_Camera.m_vEye;
	D3DXVECTOR3 vLookAt = g_Camera.m_vLookat;
	D3DXVECTOR3 vUp     = g_Camera.m_vUp;
	
	D3DXMATRIX  matView;	
	D3DXMATRIX  matIdentity;
	D3DXMATRIX  matRot;

	D3DXVECTOR3 vTempEye( 0.0f , 0.0f , -1.5f );
	D3DXVECTOR3 vTempLookAt( 0.0f , 0.0f , 0.0f );
	D3DXVECTOR3 vTempUp( 0.0f , 1.0f , 0.0f );

	D3DXMatrixIdentity( &matIdentity );

	D3DXMatrixLookAtLH( &matView , &vTempEye, &vTempLookAt, &vTempUp );
	
	g_RenderManager.SetTransform(D3DTS_VIEW , &matView);

	m_fRotAngle += 10 * g_Particle.m_fTimeD;

	if( m_fRotAngle >= 360.0f )
		m_fRotAngle -= 360.0f;

	D3DXMatrixRotationZ( &matRot , D3DXToRadian( m_fRotAngle ) );

	g_RenderManager.SetTransform( D3DTS_WORLD, &matRot );	
	
	D3DXMATRIX matProj;
			
	D3DXMatrixPerspectiveFovLH( &matProj, 160.0f , 1.333f, 1.0f, 100.0f );
	g_RenderManager.SetTransform(D3DTS_PROJECTION, &matProj );


	g_RenderManager.SetTexture( 0, m_pBGTexture );			
	g_RenderManager.SetFVF( D3DFVF_OBJECTVERTEX );
	
	g_RenderManager.SetIndices( m_pBGModel->m_pIB , 0 );
	g_RenderManager.SetStreamSource( 0 , m_pBGModel->m_pVB , sizeof(OBJECTVERTEX) );

	g_RenderManager.SetCullMode( D3DCULL_NONE );
	//g_RenderManager.SetFillMode( D3DFILL_WIREFRAME );

	SetColor( D3DCOLOR_COLORVALUE( 0.49f , 0.33f , 0.54f , 1.0f ));
	DrawIndexedPrimitive( m_pBGModel->m_header.m_num_vertex , m_pBGModel->m_header.m_num_face     );

	D3DXMATRIX matTrans;

	D3DXMatrixIdentity( &matTrans );

	//matTrans._22 = ( float ) ( ( float ) NowTimer - ( float ) m_OldTimer[ 1 ] ) / ( float ) 1000;

	g_RenderManager.SetTextureTransFormFlags( 0 , D3DTTFF_COUNT3 );
	
	g_RenderManager.SetTransform( D3DTS_WORLD, &matIdentity );	
	D3DXMatrixPerspectiveFovLH( &matProj, 160.0f, 1.333f, 1.0f, 15.0f );

	D3DXMatrixRotationZ( &matTrans , D3DXToRadian( m_fRotAngle ) );

	g_RenderManager.SetTransform( D3DTS_TEXTURE0 , &matTrans );

	D3DXMatrixPerspectiveFovLH( &matProj , D3DX_PI/4.25f, 1.333f, 1.0f, 100.0f );

	SetColor( D3DCOLOR_COLORVALUE( 1.0f , 1.0f , 1.0f , 0.5f  ));
	DrawIndexedPrimitive( m_pBGModel->m_header.m_num_vertex , m_pBGModel->m_header.m_num_face     );

	D3DXMatrixRotationY( &matTrans , D3DXToRadian( m_fRotAngle ) );

	g_RenderManager.SetTransform( D3DTS_TEXTURE0 , &matTrans );

	SetColor( D3DCOLOR_COLORVALUE( 1.0f , 1.0f , 1.0f , 0.5f ));
	DrawIndexedPrimitive( m_pBGModel->m_header.m_num_vertex , m_pBGModel->m_header.m_num_face     );

	g_RenderManager.SetTextureTransFormFlags( 0 , D3DTTFF_DISABLE );


	//......................................................................................................
	g_Camera.SetCamera(vEye,vLookAt,vUp,FALSE);
	g_RenderManager.SetZEnable( TRUE );
	g_RenderManager.SetZWriteEnable( TRUE );
	g_RenderManager.SetAlphaBlendEnable( FALSE );

}


void CChangeClass::SetColor(D3DXCOLOR Value)
{
#ifdef DIRECT_VERSION_9_MJH
	D3DMATERIAL9 mtrl;
	ZeroMemory( &mtrl, sizeof(D3DMATERIAL9) );
#else
	D3DMATERIAL8 mtrl;
	ZeroMemory( &mtrl, sizeof(D3DMATERIAL8) );
#endif // DIRECT_VERSION_9_MJH

	mtrl.Diffuse.r = mtrl.Ambient.r = Value.r;
	mtrl.Diffuse.g = mtrl.Ambient.g = Value.g;
	mtrl.Diffuse.b = mtrl.Ambient.b = Value.b;
	mtrl.Diffuse.a = mtrl.Ambient.a = Value.a;

	g_RenderManager.SetMaterial( &mtrl );
}

typedef struct s_VERTEX2D
{
    D3DXVECTOR4 pos;
    D3DCOLOR    color;
    FLOAT       tu0, tv0;

	s_VERTEX2D()
	{
		pos.z = pos.w = 1.0f;
	}

}VERTEX2D;

#define D3DFVF_VERTEX2D ( D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1 )
#define D3DFVF_VERTEX3D ( D3DFVF_XYZ|D3DFVF_TEX1 )

void CChangeClass::DrawTexture( float x, float y, float w, float h, 
								float tx, float ty, float tw, float th,
#ifdef DIRECT_VERSION_9_MJH
								D3DCOLOR color , LPDIRECT3DTEXTURE9 pTex , LPDIRECT3DDEVICE9 pDevice )
#else
								D3DCOLOR color , LPDIRECT3DTEXTURE8 pTex , LPDIRECT3DDEVICE8 pDevice )
#endif // DIRECT_VERSION_9_MJH
{
	VERTEX2D	v[4];
	
	
	v[0].color = v[1].color = v[2].color = v[3].color = color;
	
	v[0].pos.x = (float)x - 0.5f;
	v[0].pos.y = (float)y - 0.5f;
	v[1].pos.x = v[0].pos.x + w;
	v[1].pos.y = v[0].pos.y;
	v[2].pos.x = v[0].pos.x + w;
	v[2].pos.y = v[0].pos.y + h;
	v[3].pos.x = v[0].pos.x;
	v[3].pos.y = v[0].pos.y + h;

	v[0].tu0 = (float)tx;
	v[0].tv0 = (float)ty;
	v[1].tu0 = v[0].tu0 + tw;
	v[1].tv0 = v[0].tv0;
	v[2].tu0 = v[0].tu0 + tw;
	v[2].tv0 = v[0].tv0 + th;
	v[3].tu0 = v[0].tu0;
	v[3].tv0 = v[0].tv0 + th;

	v[0].pos.z = 0.0f;
	v[1].pos.z = 0.0f;
	v[2].pos.z = 0.0f;
	v[3].pos.z = 0.0f;

	g_RenderManager.SetTextureTransFormFlags( 0 , D3DTTFF_DISABLE );
	g_RenderManager.SetTextureTransFormFlags( 1 , D3DTTFF_DISABLE );
	g_RenderManager.DisableTextureStageState( 1 );
	g_RenderManager.DisableTextureStageState( 0 );

	g_RenderManager.SetTextureColorOP( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureColorArg1( 0 , D3DTA_CURRENT );
	g_RenderManager.SetTextureColorArg2( 0 , D3DTA_TEXTURE );
	g_RenderManager.SetTextureAlphaOP( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureAlphaArg1( 0 , D3DTA_CURRENT );
	g_RenderManager.SetTextureAlphaArg2( 0 , D3DTA_TEXTURE );

	g_RenderManager.SetFVF( D3DFVF_VERTEX2D );
	g_RenderManager.SetTexture( 0 , pTex );
	g_RenderManager.DrawPrimitiveUP(D3DPT_TRIANGLEFAN , 2, v , sizeof( VERTEX2D ) );
}

void CChangeClass::DrawPlayer(void)
{	
	g_RenderManager.SetMaterial( &g_Pc_Manager.theCharacter_Material );	
	g_RenderManager.SetLight( 0, &g_Pc_Manager.theCharacter_Light );	

	g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureColorArg1( 0 , D3DTA_CURRENT );
	g_RenderManager.SetTextureColorArg2( 0 , D3DTA_TEXTURE );
	
	g_RenderManager.SetTextureAlphaOP  ( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureAlphaArg1( 0 , D3DTA_CURRENT );
	g_RenderManager.SetTextureAlphaArg2( 0 , D3DTA_TEXTURE );
	
	g_RenderManager.DisableTextureStageState( 1 );
	g_RenderManager.DisableTextureStageState( 2 );
	
	g_RenderManager.SetFVF( D3DFVF_OBJECTVERTEX );	
	
	g_RenderManager.SetAlphaBlendEnable( TRUE );
	g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
	g_RenderManager.SetDestBlend( D3DBLEND_INVSRCALPHA );

	g_RenderManager.SetZEnable( TRUE );
	g_RenderManager.SetZWriteEnable( TRUE );

	g_RenderManager.SetCullMode( D3DCULL_CCW );
	g_RenderManager.SetLighting( TRUE );
	g_RenderManager.LightEnable( 0 , TRUE );

	g_Pc.GetPlayer()->Display();

	g_RenderManager.SetLighting( TRUE );
	g_RenderManager.SetAlphaBlendEnable( FALSE );
}

void CChangeClass::DrawCrack(void)
{
	g_RenderManager.SetTextureTransFormFlags( 0 , D3DTTFF_DISABLE );
	g_RenderManager.SetTextureTransFormFlags( 1 , D3DTTFF_DISABLE );
	g_RenderManager.DisableTextureStageState( 1 );
	g_RenderManager.DisableTextureStageState( 0 );

/*
	g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureColorArg1( 0 , D3DTA_CURRENT );
	g_RenderManager.SetTextureColorArg2( 0 , D3DTA_TEXTURE );
	
	g_RenderManager.SetTextureAlphaOP  ( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureAlphaArg1( 0 , D3DTA_CURRENT );
	g_RenderManager.SetTextureAlphaArg2( 0 , D3DTA_TEXTURE );
*/
	g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_SELECTARG1 );
	g_RenderManager.SetTextureColorArg1( 0 , D3DTA_TEXTURE );
	
	g_RenderManager.SetTextureAlphaOP  ( 0 , D3DTOP_SELECTARG1 );
	g_RenderManager.SetTextureAlphaArg1( 0 , D3DTA_TEXTURE );

    g_RenderManager.SetAmbient( 0x000F0F0F );
	g_RenderManager.SetCullMode( D3DCULL_NONE );
	g_RenderManager.SetLighting( FALSE );
	g_RenderManager.SetZEnable( TRUE );
	g_RenderManager.SetZWriteEnable( TRUE );
	g_RenderManager.SetAlphaBlendEnable( FALSE );

	g_RenderManager.SetFVF( D3DFVF_VERTEX3D );
	g_RenderManager.SetTexture( 0 , m_SurfaceList[ 0 ].GetTexture() );
	g_RenderManager.SetFillMode( D3DFILL_SOLID );

	D3DXMATRIX matTrans;
	D3DXMATRIX matRot;
	D3DXMATRIX matLocal;

	D3DXMatrixIdentity( &matTrans );
	D3DXMatrixIdentity( &matLocal );

	for( int i = 0 ; i < m_CrackCount ; ++i )
	{
		m_pCrackList[ i ].vPos.x += m_pCrackList[ i ].AddX * g_Particle.m_fTimeD;
		m_pCrackList[ i ].vPos.y += m_pCrackList[ i ].AddY * g_Particle.m_fTimeD;

		matTrans._41 = m_pCrackList[ i ].vPos.x;
		matTrans._42 = m_pCrackList[ i ].vPos.y;

		m_pCrackList[ i ].m_Angle += m_pCrackList[ i ].RotAdd;

		switch( m_pCrackList[ i ].RotType )
		{
			case 0:
				D3DXMatrixRotationX( &matRot , D3DXToRadian( m_pCrackList[ i ].m_Angle ) );
				break;

			case 1:
				D3DXMatrixRotationY( &matRot , D3DXToRadian( m_pCrackList[ i ].m_Angle ) );
				break;

			case 2:
				D3DXMatrixRotationZ( &matRot , D3DXToRadian( m_pCrackList[ i ].m_Angle ) );
				break;
		}

		D3DXMatrixMultiply( &matLocal , &matRot , &matTrans );

		g_RenderManager.SetTransform( D3DTS_WORLD, &matLocal );	

		g_RenderManager.DrawPrimitiveUP(D3DPT_TRIANGLELIST , 1 , m_pCrackList[ i ].CrackVertex , 
										sizeof( VERTEX3D ) );
	}


	D3DXMatrixIdentity( &matLocal );
	g_RenderManager.SetTransform( D3DTS_WORLD, &matLocal );	
}

void CChangeClass::MakeCrackPlane(void)
{
	int m_XCount = 8; 
	int m_YCount = 8;
/*	
	m_XCount = 1; 
	m_YCount = 1;
*/
	m_CrackCount = m_XCount * m_YCount * 2;

#ifdef HHM_USER_RESOLUTION_SELECT// 기존 선택된 해상도(2009/04/24)
	
	float fXSize = nRui->Desktop.Width / ( float ) m_XCount;
	float fYSize = nRui->Desktop.Height / ( float ) m_YCount;
	
	float fXHalf = nRui->Desktop.Width / 2.0f;
	float fYHalf = nRui->Desktop.Height  / 2.0f;
	
#else
	
	float fXSize = 1024.0f / ( float ) m_XCount;
	float fYSize = 768.0f / ( float ) m_YCount;
	
	float fXHalf = 1024.0f / 2.0f;
	float fYHalf = 768.0f  / 2.0f;
	
#endif// #ifdef HHM_USER_RESOLUTION_SELECT// 기존 선택된 해상도(2009/04/24)

	
	float fXHalfSize = fXSize / 2.0f;
	float fYHalfSize = fYSize / 2.0f;

	float AddTu = 1.0f / ( float ) m_XCount;
	float AddTv = 1.0f / ( float ) m_YCount;

	m_pCrackList = SAFE_NEW_ARRAY( CRACKTRIANGLE , m_CrackCount );

	int Count = 0;

	float RotMin,RotMax;
	float TransMin,TransMax;

/*	*/
	RotMin   = 5.0f;
	RotMax   = 20.0f;
	TransMin = 1000.0f;
	TransMax = 1500.0f;
/*
	RotMin   = 0.0f;
	RotMax   = 0.0f;
	TransMin = 0.0f;
	TransMax = 0.0f;
*/
	for( int i = 0 ; i < m_XCount ; ++i )
	{
		for( int j = 0 ; j < m_YCount ; ++j )
		{
			m_pCrackList[ Count ].CrackVertex[ 0 ].vPos.x = -fXHalfSize;
			m_pCrackList[ Count ].CrackVertex[ 0 ].vPos.y = -fYHalfSize;
			m_pCrackList[ Count ].CrackVertex[ 0 ].vPos.z =  0.0f;
			m_pCrackList[ Count ].CrackVertex[ 0 ].Tu     = AddTu * i;
			m_pCrackList[ Count ].CrackVertex[ 0 ].Tv     = AddTv * ( j + 1 );

			m_pCrackList[ Count ].CrackVertex[ 1 ].vPos.x = -fXHalfSize;
			m_pCrackList[ Count ].CrackVertex[ 1 ].vPos.y =  fYHalfSize;
			m_pCrackList[ Count ].CrackVertex[ 1 ].vPos.z =  0.0f;
			m_pCrackList[ Count ].CrackVertex[ 1 ].Tu     = AddTu * i;
			m_pCrackList[ Count ].CrackVertex[ 1 ].Tv     = AddTv * j;

			m_pCrackList[ Count ].CrackVertex[ 2 ].vPos.x =  fXHalfSize;
			m_pCrackList[ Count ].CrackVertex[ 2 ].vPos.y = -fYHalfSize;
			m_pCrackList[ Count ].CrackVertex[ 2 ].vPos.z =  0.0f;
			m_pCrackList[ Count ].CrackVertex[ 2 ].Tu     = AddTu * ( i + 1 );
			m_pCrackList[ Count ].CrackVertex[ 2 ].Tv     = AddTv * ( j + 1 );
			
			m_pCrackList[ Count ].vPos.x = -fXHalf + ( i * fXSize ) + fXHalfSize;
			m_pCrackList[ Count ].vPos.y =  fYHalf - ( j * fYSize ) - fYHalfSize;
			m_pCrackList[ Count ].vPos.z = fXSize * ( m_CrackCount - Count );

			m_pCrackList[ Count ].RotType = RandomNum( 0 , 2 );
			m_pCrackList[ Count ].RotAdd  = RandomNum( RotMin , RotMax );		
	
			m_pCrackList[ Count ].AddX	  = RandomNum( TransMin , TransMax );			
			m_pCrackList[ Count ].AddY    = RandomNum( TransMin , TransMax );	

			if( RandomNum( 0 , 1 ) )
				m_pCrackList[ Count ].AddX = -m_pCrackList[ Count ].AddX;

			if( RandomNum( 0 , 1 ) )
				m_pCrackList[ Count ].AddY = -m_pCrackList[ Count ].AddY;

			Count++;

			m_pCrackList[ Count ].CrackVertex[ 0 ].vPos.x =  fXHalfSize;
			m_pCrackList[ Count ].CrackVertex[ 0 ].vPos.y = -fYHalfSize;
			m_pCrackList[ Count ].CrackVertex[ 0 ].vPos.z =  0.0f;
			m_pCrackList[ Count ].CrackVertex[ 0 ].Tu     = AddTu * ( i + 1 );
			m_pCrackList[ Count ].CrackVertex[ 0 ].Tv     = AddTv * ( j + 1 );
		
			m_pCrackList[ Count ].CrackVertex[ 1 ].vPos.x = -fXHalfSize;
			m_pCrackList[ Count ].CrackVertex[ 1 ].vPos.y =  fYHalfSize;
			m_pCrackList[ Count ].CrackVertex[ 1 ].vPos.z =  0.0f;
			m_pCrackList[ Count ].CrackVertex[ 1 ].Tu     = AddTu * i;
			m_pCrackList[ Count ].CrackVertex[ 1 ].Tv     = AddTv * j;

			m_pCrackList[ Count ].CrackVertex[ 2 ].vPos.x =  fXHalfSize;
			m_pCrackList[ Count ].CrackVertex[ 2 ].vPos.y =  fYHalfSize;
			m_pCrackList[ Count ].CrackVertex[ 2 ].vPos.z =  0.0f;
			m_pCrackList[ Count ].CrackVertex[ 2 ].Tu     = AddTu * ( i + 1 );
			m_pCrackList[ Count ].CrackVertex[ 2 ].Tv     = AddTv * j;

			m_pCrackList[ Count ].vPos.x = -fXHalf + ( i * fXSize ) + fXHalfSize;
			m_pCrackList[ Count ].vPos.y =  fYHalf - ( j * fYSize ) - fYHalfSize;
			m_pCrackList[ Count ].vPos.z = fXSize * ( m_CrackCount - Count );

			m_pCrackList[ Count ].RotType = RandomNum( 0 , 2 );
			m_pCrackList[ Count ].RotAdd  = RandomNum( RotMin , RotMax );		
	
			m_pCrackList[ Count ].AddX	  = RandomNum( TransMin , TransMax );			
			m_pCrackList[ Count ].AddY    = RandomNum( TransMin , TransMax );	

			if( RandomNum( 0 , 1 ) )
				m_pCrackList[ Count ].AddX = -m_pCrackList[ Count ].AddX;

			if( RandomNum( 0 , 1 ) )
				m_pCrackList[ Count ].AddY = -m_pCrackList[ Count ].AddY;

			Count++;
		}
	}
}

void CChangeClass::GetAniIndex(void)
{
	int m_WeaponType = g_Pc.GetPlayer()->GetWeaponType();
	
	switch( nRui->thePcParam.MainClass )
	{
		case Swordman:
			// 대검
			if( m_WeaponType == 0 )
			{
				m_AniIndex[ 0 ] = n_ClassChage0;
				m_AniIndex[ 1 ] = n_ClassChage1;
			}
			// 한손검
			else 
			{
				m_AniIndex[ 0 ] = n_ClassChage4;
				m_AniIndex[ 1 ] = n_ClassChage5;
			}
			break;

		case Knight:
				m_AniIndex[ 0 ] = n_ClassChage6;
				m_AniIndex[ 1 ] = n_ClassChage7;
			break;

		case Mercenary:
				m_AniIndex[ 0 ] = n_ClassChage4;
				m_AniIndex[ 1 ] = n_ClassChage5;
			break;
		
		case Slayer:
				m_AniIndex[ 0 ] = n_ClassChage0;
				m_AniIndex[ 1 ] = n_ClassChage1;
			break;

		case Shaman:
			m_AniIndex[ 0 ] = n_ClassChage12;
			m_AniIndex[ 1 ] = n_ClassChage13;
			break;

		case Magician:
			m_AniIndex[ 0 ] = n_ClassChage8;
			m_AniIndex[ 1 ] = n_ClassChage9;
			break;

		case Psyche:
			m_AniIndex[ 0 ] = n_ClassChage12;
			m_AniIndex[ 1 ] = n_ClassChage13;
			break;

		case Archer:
			m_AniIndex[ 0 ] = n_ClassChage2;
			m_AniIndex[ 1 ] = n_ClassChage3;
			break;

		case Sniper:
			m_AniIndex[ 0 ] = n_ClassChage2;
			m_AniIndex[ 1 ] = n_ClassChage3;
			break;

		case Prominas:
			m_AniIndex[ 0 ] = n_ClassChage8;
			m_AniIndex[ 1 ] = n_ClassChage9;
			break;		

		case Priest:
			m_AniIndex[ 0 ] = n_ClassChage10;
			m_AniIndex[ 1 ] = n_ClassChage11;
			break;		

		case Holy_Avenger:
			m_AniIndex[ 0 ] = n_ClassChage10;
			m_AniIndex[ 1 ] = n_ClassChage11;
			break;		
	}
}


int CChangeClass::DrawPlayerFrame(Character *pChar,int AniIndex,int AniFrame)
{
	int					ani_number;					// 현재 애니메이션 번호
	int					pre_ani_number;				// 이전 애니메이션 번호
	int					ani_index;					// 현재 애니메이션 인덱스	
	int					ani_frame;					// 애니 프레임
	unsigned int		end_frame;					// 현재 종료 프레임			
	unsigned int		pre_end_frame;				// 이전 종표 프레임			
	
	int					blend_frame;				// 모션 블렌딩 프레임 
	
	int					move_frame;					// 이동 프레임 
	float				move_middle_time;			// 이동 보간 프레임
	
	float				move_percent;				// 이동거리 퍼센트 
	float				ani_percent;				// 애니메이션 퍼센트 
	
	float				step_time;					// 보폭타임
	float				ani_time;					// 애니 타임	
	float				middle_time;				// 1프레임 ~ 2프레임 사이의 중간값 저장 ( 0.0 ~ 1.0 사이 )	
	long				PreTime;					// 이전 렌더링한 시간	
	long				SimTime;					// 심심한 시간	
	D3DXMATRIX			matChr;
	
	ani_number		 = pChar->m_ani_number;		
	pre_ani_number   = pChar->m_pre_ani_number;	
	ani_index	 	 = pChar->m_ani_index;		
	ani_frame		 = pChar->m_ani_frame;		
	end_frame		 = pChar->m_end_frame;		
	pre_end_frame	 = pChar->m_pre_end_frame;	
	
	blend_frame		 = pChar->m_blend_frame;	
	
	move_frame		 = pChar->m_move_frame;		
	move_middle_time = pChar->m_move_middle_time;
	
	move_percent	 = pChar->m_move_percent;	
	ani_percent		 = pChar->m_ani_percent;	
	
	step_time		 = pChar->m_step_time;		
	ani_time		 = pChar->m_ani_time;		
	middle_time		 = pChar->m_middle_time;	
	PreTime			 = pChar->m_PreTime;		
	SimTime			 = pChar->m_SimTime;		
	matChr           = pChar->GetPosTM();	
	
	pChar->AnimationPlayNoneBlend( AniIndex , -1 );	
	pChar->AniRender();	

	g_RenderManager.SetMaterial( &g_Pc_Manager.theCharacter_Material );	
	g_RenderManager.SetLight( 0, &g_Pc_Manager.theCharacter_Light );	

	g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureColorArg1( 0 , D3DTA_CURRENT );
	g_RenderManager.SetTextureColorArg2( 0 , D3DTA_TEXTURE );
	
	g_RenderManager.SetTextureAlphaOP  ( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureAlphaArg1( 0 , D3DTA_CURRENT );
	g_RenderManager.SetTextureAlphaArg2( 0 , D3DTA_TEXTURE );
	
	g_RenderManager.DisableTextureStageState( 1 );
	g_RenderManager.DisableTextureStageState( 2 );
	
	g_RenderManager.SetFVF( D3DFVF_OBJECTVERTEX );	
	
	g_RenderManager.SetAlphaBlendEnable( TRUE );
	g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
	g_RenderManager.SetDestBlend( D3DBLEND_INVSRCALPHA );

	g_RenderManager.SetZEnable( TRUE );
	g_RenderManager.SetZWriteEnable( TRUE );

	g_RenderManager.SetCullMode( D3DCULL_CCW );
	g_RenderManager.SetLighting( TRUE );
	g_RenderManager.LightEnable( 0 , TRUE );

	g_Pc.GetPlayer()->Display();

	g_RenderManager.SetLighting( FALSE );
	g_RenderManager.SetAlphaBlendEnable( FALSE );

	pChar->m_ani_number		  = ani_number;		
	pChar->m_pre_ani_number   = pre_ani_number;	
	pChar->m_ani_index	 	  = ani_index;		
	pChar->m_ani_frame		  = ani_frame;		
	pChar->m_end_frame		  = end_frame;		
	pChar->m_pre_end_frame	  = pre_end_frame;	
	
	pChar->m_blend_frame	  =	blend_frame;	
	
	pChar->m_move_frame		  = move_frame;		
	pChar->m_move_middle_time = move_middle_time;
	
	pChar->m_move_percent	  = move_percent;	
	pChar->m_ani_percent	  =	ani_percent;	
	
	pChar->m_step_time		  = step_time;		
	pChar->m_ani_time		  = ani_time;		
	pChar->m_middle_time	  =	middle_time;	
	pChar->m_PreTime		  =	PreTime;		
	pChar->m_SimTime		  = SimTime;		
	
	return TRUE;
}

void CChangeClass::MakeBackImage(void)
{
	g_RockClient.m_SceneManager.SetFixFaceExp( CFaceExpData::FACE_EXP_NORMAL );

	bool bIsBeginScene = g_RockClient.m_bIsBeginScene;	
	if( bIsBeginScene )
	{
		g_RockClient.EndScene();
	}
	
	if( SUCCEEDED( m_SurfaceList[ 1 ].BeginScene() ) )
	{
		g_Camera.SetEndFrame( 12 , g_lpDevice );
		
		g_RenderManager.Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER , D3DCOLOR_ARGB(0,0,0,0), 1.0f, 0 );		

		g_RenderManager.SetZWriteEnable( FALSE );

	#ifdef HHM_USER_RESOLUTION_SELECT// 기존 선택된 해상도(2009/04/24)
		
		DrawTexture( 0.0f , 0.0f , nRui->Desktop.Width , nRui->Desktop.Height ,
			
	#else
			
		DrawTexture( 0.0f , 0.0f , 1024.0f , 768.0f ,
			
	#endif// #ifdef HHM_USER_RESOLUTION_SELECT// 기존 선택된 해상도(2009/04/24)

			0.0f , 0.0f , 1.0f , 0.75f , D3DCOLOR_XRGB( 255 , 255 , 255 ) , 
			m_pBGImageTexture , g_lpDevice );
		
		g_RenderManager.SetZWriteEnable( TRUE );
		
		DrawPlayerFrame( g_Pc.GetPlayer() , m_AniIndex[ 1 ] , -1 );
		m_SurfaceList[ 1 ].EndScene();

		if( SUCCEEDED( m_SurfaceList[ 0 ].BeginScene() ) )
		{
			g_RenderManager.Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER , D3DCOLOR_ARGB(0,0,0,0), 1.0f, 0 );
			
		#ifdef HHM_USER_RESOLUTION_SELECT// 기존 선택된 해상도(2009/04/24)
			
			DrawTexture( 0.0f , 0.0f , m_fWidthRange , nRui->Desktop.Height ,
				
		#else
				
			DrawTexture( 0.0f , 0.0f , m_fWidthRange , 768.0f ,
				
		#endif// #ifdef HHM_USER_RESOLUTION_SELECT// 기존 선택된 해상도(2009/04/24)

				0.0f , 0.0f , m_fThreeRange , 0.75f , D3DCOLOR_XRGB( 255 , 255 , 255 ) , 
				m_SurfaceList[ 1 ].GetTexture() , g_lpDevice );

			m_SurfaceList[ 0 ].EndScene();
		}
		else
		{
			m_ISMakeBackImage = FALSE;
		}

	}
	else
	{
		m_ISMakeBackImage = FALSE;
	}

	if( SUCCEEDED( m_SurfaceList[ 1 ].BeginScene() ) )
	{
		g_RenderManager.Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER , D3DCOLOR_ARGB(0,0,0,0), 1.0f, 0 );		

		g_Camera.SetEndFrame( 13 , g_lpDevice );
		
		g_RenderManager.SetZWriteEnable( FALSE );

	#ifdef HHM_USER_RESOLUTION_SELECT// 기존 선택된 해상도(2009/04/24)
		
		DrawTexture( 0.0f , 0.0f , nRui->Desktop.Width , nRui->Desktop.Height ,
			
	#else
			
		DrawTexture( 0.0f , 0.0f , 1024.0f , 768.0f ,
			
	#endif// #ifdef HHM_USER_RESOLUTION_SELECT// 기존 선택된 해상도(2009/04/24)

			0.0f , 0.0f , 1.0f , 0.75f , D3DCOLOR_XRGB( 255 , 255 , 255 ) , 
			m_pBGImageTexture , g_lpDevice );
		
		g_RenderManager.SetZWriteEnable( TRUE );

		DrawPlayerFrame( g_Pc.GetPlayer() , m_AniIndex[ 1 ] , -1 );
		
		m_SurfaceList[ 1 ].EndScene();

		if( SUCCEEDED( m_SurfaceList[ 0 ].BeginScene() ) )
		{
			DrawTexture( m_fWidthRange * 1.0f , 0.0f , m_fWidthRange , 768.0f ,
				m_fThreeRange , 0.0f , m_fThreeRange , 0.75f , D3DCOLOR_XRGB( 255 , 255 , 255 ) , 
				m_SurfaceList[ 1 ].GetTexture() , g_lpDevice );

			m_SurfaceList[ 0 ].EndScene();
		}
		else
		{
			m_ISMakeBackImage = FALSE;
		}

	}
	else
	{
		m_ISMakeBackImage = FALSE;
	}

	if( SUCCEEDED( m_SurfaceList[ 1 ].BeginScene() ) )
	{
		g_RenderManager.Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER , D3DCOLOR_ARGB(0,0,0,0), 1.0f, 0 );		

		g_Camera.SetEndFrame( 14 , g_lpDevice );
		
		g_RenderManager.SetZWriteEnable( FALSE );
		
		DrawTexture( 0.0f , 0.0f , 1024.0f , 768.0f ,
			0.0f , 0.0f , 1.0f , 0.75f , D3DCOLOR_XRGB( 255 , 255 , 255 ) , 
			m_pBGImageTexture , g_lpDevice );
		
		g_RenderManager.SetZWriteEnable( TRUE );

		DrawPlayerFrame( g_Pc.GetPlayer() , m_AniIndex[ 1 ] , -1 );
		
		m_SurfaceList[ 1 ].EndScene();

		if( SUCCEEDED( m_SurfaceList[ 0 ].BeginScene() ) )
		{
			DrawTexture( m_fWidthRange * 2.0f , 0.0f , m_fWidthRange , 768.0f ,
				m_fThreeRange * 2.0f , 0.0f , m_fThreeRange , 0.75f , D3DCOLOR_XRGB( 255 , 255 , 255 ) , 
				m_SurfaceList[ 1 ].GetTexture() , g_lpDevice );

			m_SurfaceList[ 0 ].EndScene();
		}
		else
		{
			m_ISMakeBackImage = FALSE;
		}

	}
	else
	{
		m_ISMakeBackImage = FALSE;
	}

	if( bIsBeginScene )
	{
		g_RockClient.BeginScene();
	}

	g_RockClient.m_SceneManager.SetFixFaceExp( CFaceExpData::FACE_EXP_MAXCOUNT );
}
