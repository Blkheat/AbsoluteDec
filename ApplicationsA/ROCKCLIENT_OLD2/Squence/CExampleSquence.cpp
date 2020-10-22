#include "..\\RockClient.h"
#include "..\\stringmanager.h"
#include "..\\brother.h"
#include "..\\RockClient.h"


#include "..\\quadlist.h"
#include "..\\bravolibs\\network\\Protocol.h"
#include "..\\Pc.h"

#include "..\\Map.h"
#include "..\\effect.h"
#include "CExampleSquence.h"


CExampleSquence::CExampleSquence()
{
	switch( nRui->thePcParam.MainClass )
	{
		case Swordman:
/*
			m_AniIndexList[ 0 ] = ;
			m_AniIndexList[ 1 ] = ;
			m_AniIndexList[ 2 ] = ;
			m_AniIndexList[ 3 ] = ;
*/
			break;
		case Archer:
/*
			m_AniIndexList[ 0 ] = ;
			m_AniIndexList[ 1 ] = ;
			m_AniIndexList[ 2 ] = ;
			m_AniIndexList[ 3 ] = ;
*/
			break;
		case Knight:
/*
			m_AniIndexList[ 0 ] = ;
			m_AniIndexList[ 1 ] = ;
			m_AniIndexList[ 2 ] = ;
			m_AniIndexList[ 3 ] = ;
*/
			break;
		case Mercenary:
/*
			m_AniIndexList[ 0 ] = ;
			m_AniIndexList[ 1 ] = ;
			m_AniIndexList[ 2 ] = ;
			m_AniIndexList[ 3 ] = ;
*/
			break;
		case Slayer:
/*
			m_AniIndexList[ 0 ] = ;
			m_AniIndexList[ 1 ] = ;
			m_AniIndexList[ 2 ] = ;
			m_AniIndexList[ 3 ] = ;
*/
			break;
		case Sniper:
/*
			m_AniIndexList[ 0 ] = ;
			m_AniIndexList[ 1 ] = ;
			m_AniIndexList[ 2 ] = ;
			m_AniIndexList[ 3 ] = ;
*/
			break;
		case Shaman:
/*
			m_AniIndexList[ 0 ] = ;
			m_AniIndexList[ 1 ] = ;
			m_AniIndexList[ 2 ] = ;
			m_AniIndexList[ 3 ] = ;
*/
			break;
		case Magician:
/*
			m_AniIndexList[ 0 ] = ;
			m_AniIndexList[ 1 ] = ;
			m_AniIndexList[ 2 ] = ;
			m_AniIndexList[ 3 ] = ;
*/
			break;
		case Psyche:
/*
			m_AniIndexList[ 0 ] = ;
			m_AniIndexList[ 1 ] = ;
			m_AniIndexList[ 2 ] = ;
			m_AniIndexList[ 3 ] = ;
*/
			break;
	}
}


int CExampleSquence::Squence00()
{
	g_RockClient.m_SceneManager.SetGUIDisplay( FALSE );		
	g_RockClient.m_SceneManager.SetSoundPlayMode( SCENE_SQUENCE );
	StopAllSound();

	m_SaveCamera = g_Camera.GetSaveVlue();
	
	SetPlayerAni( n_Stand_Peace9 );

	g_Particle.m_Fade.Start( FADE_STATE_FADE , D3DCOLOR_ARGB( 0 , 0 , 0 , 0  ), 
							 D3DCOLOR_ARGB( 255 , 0 , 0 , 0 ) , 500 , FALSE );

	
	SetWaitFlag( SWT_WAIT );

	return TRUE;
}

int CExampleSquence::SquenceCheck00()
{
	if( g_Particle.m_Fade.GetColor() == D3DCOLOR_ARGB( 255 , 0 , 0 , 0 ) )
	{
		SetStartScene();

		g_Particle.m_Fade.Start( FADE_STATE_FADE , D3DCOLOR_ARGB( 255 , 0 , 0 , 0 ) ,
								 D3DCOLOR_ARGB( 0 , 0 , 0 , 0  ), 500 , FALSE );

		m_fHeight = ( g_Pc.GetPlayer()->GetBoundMaxVec().y - g_Pc.GetPlayer()->GetBoundMinVec().y );

		SetCameraFront( 1.4f );	

		NextSquence();
	}

	return TRUE;
}


int CExampleSquence::Squence01()
{
	SetWaitFlag( SWT_WAIT );

	return TRUE;
}

int CExampleSquence::SquenceCheck01()
{
	if( g_Particle.m_Fade.GetColor() == D3DCOLOR_ARGB( 0 , 0 , 0 , 0 ) )
	{
		NextSquence();
	}

	return TRUE;
}

int CExampleSquence::Squence02()
{
	SetPlayerAni( m_AniIndexList[ 0 ] );
	SetPlayerNextAni( m_AniIndexList[ 1 ] );
	
	SetWaitFlag( SWT_ANI );

	return TRUE;
}

int CExampleSquence::Squence03()
{
	SetCameraRight(1.2f);
	//PlaySound( 127 , NULL );

	g_Particle.m_Fade.Start(FADE_STATE_LOOP,D3DCOLOR_ARGB( 0 , 0 , 0 , 0 ),
											D3DCOLOR_ARGB( 100 , 255 , 255 , 255 ) ,
											D3DCOLOR_ARGB( 0 , 0 , 0 , 0 ),
											100,100,FALSE);

	SetPlayerNextAni( m_AniIndexList[ 2 ] );
	SetWaitFlag( SWT_ANI );

	return TRUE;
}

int CExampleSquence::Squence04()
{
	SetCameraLeft(1.2f);
	//PlaySound( 127 , NULL );

	g_Particle.m_Fade.Start(FADE_STATE_LOOP,D3DCOLOR_ARGB( 0 , 0 , 0 , 0 ),
											D3DCOLOR_ARGB( 100 , 255 , 255 , 255 ) ,
											D3DCOLOR_ARGB( 0 , 0 , 0 , 0 ),
											100,100,FALSE);

	SetPlayerNextAni( m_AniIndexList[ 3 ] );

	EffectLock();
	g_Particle.SetEffSetCon( 1.3f , EF_LEVEL_UP , 0 , 3.1f , g_Pc.GetMainPCUniqe() , -1 );
	EffectUnLock();

	SetWaitFlag( SWT_ANI );

	return TRUE;
}

int CExampleSquence::Squence05()
{
	SetCameraFront( 1.4f );

	SetWaitFlag( SWT_ANI );

	return TRUE;
}

int CExampleSquence::Squence06()
{
	SetWaitFlag( SWT_TIMER , 4000 );

	return TRUE;
}

int CExampleSquence::Squence07()
{
	return TRUE;
}

int CExampleSquence::Squence08()
{
	m_SaveCamera = g_Camera.GetSaveVlue();
	
	g_Particle.m_Fade.Start( FADE_STATE_FADE , D3DCOLOR_ARGB( 0 , 0 , 0 , 0  ), 
							 D3DCOLOR_ARGB( 255 , 0 , 0 , 0 ) , 500 , FALSE );

	
	SetWaitFlag( SWT_WAIT );

	return TRUE;
}

int CExampleSquence::SquenceCheck08()
{
	if( g_Particle.m_Fade.GetColor() == D3DCOLOR_ARGB( 255 , 0 , 0 , 0 ) )
	{
		SetEndScene();

		g_Camera.SetSaveVlue( m_SaveCamera );

		g_Particle.m_Fade.Start( FADE_STATE_FADE , D3DCOLOR_ARGB( 255 , 0 , 0 , 0 ) ,
								 D3DCOLOR_ARGB( 0 , 0 , 0 , 0  ), 500 , FALSE );

		NextSquence();
	}

	return TRUE;
}


int CExampleSquence::Squence09()
{
	SetWaitFlag( SWT_WAIT );

	return TRUE;
}

int CExampleSquence::SquenceCheck09()
{
	if( g_Particle.m_Fade.GetColor() == D3DCOLOR_ARGB( 0 , 0 , 0 , 0 ) )
	{
		NextSquence();
		g_RockClient.m_SceneManager.SetSoundPlayMode( SCENE_NORMAL );

	}

	return TRUE;
}



void CExampleSquence::SetCameraFront(float Range)
{
	float RadY = g_Pc.GetPlayer()->GetDirection();

	D3DXVECTOR3	vDir( 0.0f , 0.0f , 1.0f );
	D3DXVECTOR3 vUp( 0.0f , 1.0f , 0.0f );

	D3DXMATRIX  matRotY;
	D3DXVECTOR3 vEyePos;
	D3DXVECTOR3 vLookAt;

	D3DXVECTOR3 vHeight( 0.0f , m_fHeight , 0.0f );
	D3DXVECTOR3 vHeightHalf = vHeight / 2.0f;
	float		fHalfAngle  = ( D3DX_PI / 4.0f ) / 2.0f;

	vHeightHalf = vHeightHalf * Range;

	D3DXMatrixRotationY( &matRotY , RadY );
	D3DXVec3TransformCoord( &vDir , &vDir , &matRotY );

	vLookAt.x = g_Pc.GetPlayer()->GetPosTM()._41;
	vLookAt.y = g_Pc.GetPlayer()->GetPosTM()._42 + m_fHeight / 2.0f;
	vLookAt.z = g_Pc.GetPlayer()->GetPosTM()._43;

	vHeightHalf = vHeightHalf / tan( fHalfAngle );
	
	float fDistance = D3DXVec3Length( &vHeightHalf );

	vEyePos = vLookAt + ( -vDir * fDistance );

	g_Camera.SetCamera( vEyePos , vLookAt, vUp );
}

void CExampleSquence::SetCameraRight(float Range)
{
	float RadY = g_Pc.GetPlayer()->GetDirection();

	D3DXVECTOR3	vDir( 0.0f , 0.0f , 1.0f );
	D3DXVECTOR3 vUp( 0.0f , 1.0f , 0.0f );

	D3DXMATRIX  matRotY;
	D3DXVECTOR3 vEyePos;
	D3DXVECTOR3 vLookAt;

	D3DXVECTOR3 vHeight( 0.0f , m_fHeight , 0.0f );
	D3DXVECTOR3 vHeightHalf = vHeight / 2.0f;
	float		fHalfAngle  = ( D3DX_PI / 4.0f ) / 2.0f;

	vHeightHalf = vHeightHalf * Range;

	D3DXMatrixRotationY( &matRotY , RadY + D3DXToRadian( 90 ));
	D3DXVec3TransformCoord( &vDir , &vDir , &matRotY );

	vLookAt.x = g_Pc.GetPlayer()->GetPosTM()._41;
	vLookAt.y = g_Pc.GetPlayer()->GetPosTM()._42 + m_fHeight / 2.0f;
	vLookAt.z = g_Pc.GetPlayer()->GetPosTM()._43;

	vHeightHalf = vHeightHalf / tan( fHalfAngle );
	
	float fDistance = D3DXVec3Length( &vHeightHalf );

	vEyePos = vLookAt + ( -vDir * fDistance );

	g_Camera.SetCamera( vEyePos , vLookAt, vUp );
}

void CExampleSquence::SetCameraLeft(float Range)
{
	float RadY = g_Pc.GetPlayer()->GetDirection();

	D3DXVECTOR3	vDir( 0.0f , 0.0f , 1.0f );
	D3DXVECTOR3 vUp( 0.0f , 1.0f , 0.0f );

	D3DXMATRIX  matRotY;
	D3DXVECTOR3 vEyePos;
	D3DXVECTOR3 vLookAt;

	D3DXVECTOR3 vHeight( 0.0f , m_fHeight , 0.0f );
	D3DXVECTOR3 vHeightHalf = vHeight / 2.0f;
	float		fHalfAngle  = ( D3DX_PI / 4.0f ) / 2.0f;

	vHeightHalf = vHeightHalf * Range;

	D3DXMatrixRotationY( &matRotY , RadY + D3DXToRadian( 270 ));
	D3DXVec3TransformCoord( &vDir , &vDir , &matRotY );

	vLookAt.x = g_Pc.GetPlayer()->GetPosTM()._41;
	vLookAt.y = g_Pc.GetPlayer()->GetPosTM()._42 + m_fHeight / 2.0f;
	vLookAt.z = g_Pc.GetPlayer()->GetPosTM()._43;

	vHeightHalf = vHeightHalf / tan( fHalfAngle );
	
	float fDistance = D3DXVec3Length( &vHeightHalf );

	vEyePos = vLookAt + ( -vDir * fDistance );

	g_Camera.SetCamera( vEyePos , vLookAt, vUp );
}

void CExampleSquence::SetStartScene(void)
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

void CExampleSquence::SetEndScene(void)
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