#include "..\\RockClient.h"
#include "..\\stringmanager.h"
#include "..\\brother.h"
#include "..\\RockClient.h"


#include "..\\quadlist.h"
#include "..\\bravolibs\\network\\Protocol.h"
#include "..\\Pc.h"

#include "..\\Map.h"
#include "..\\effect.h"
#include "CRotDisplay.h"

CRotDisplay::CRotDisplay()
{
	m_RotTimer = 2500;
	
	m_ISMan = TRUE;

	switch( g_Pc.GetPlayer()->GetAttrIndex() )
	{
		case n_Millena_Woman:
		case n_Rain_Woman:	
			
			m_ISMan = FALSE;

			break;
	}

	switch( nRui->thePcParam.MainClass )
	{
		case Archer:
			m_AniIndex = 303;
			m_RotTimer = 1466;
			break;

		case Sniper:
			m_AniIndex = n_Skill_FootShoot;
			m_RotTimer = 1900;
			break;

		case Prominas:

			if( m_ISMan )
			{
				//m_AniIndex = ;
				//m_RotTimer = ;
			}
			else
			{
				m_AniIndex = 254;
				m_RotTimer = 3066;
			}

			break;		
		case Priest:
			if( m_ISMan )
			{
				//m_AniIndex = ;
				//m_RotTimer = ;
			}
			else
			{
				m_AniIndex = 255;
				m_RotTimer = 3200;
			}
			break;		
		case Holy_Avenger:
			if( m_ISMan )
			{
				m_AniIndex = 267;
				m_RotTimer = 2900;
			}
			else
			{
				//m_AniIndex = ;
				//m_RotTimer = ;
			}
			break;	
		default:
			{
				m_AniIndex = 0;
				m_RotTimer = 0;
			}
			break; 
	}
}


//..........................................................................................................
// Fade In
//..........................................................................................................
int CRotDisplay::Squence00()
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

int CRotDisplay::SquenceCheck00()
{
	if( g_Particle.m_Fade.GetColor() == D3DCOLOR_ARGB( 255 , 0 , 0 , 0 ) )
	{
		SetStartScene();

		g_Particle.m_Fade.Start( FADE_STATE_FADE , D3DCOLOR_ARGB( 255 , 0 , 0 , 0 ) ,
								 D3DCOLOR_ARGB( 0 , 0 , 0 , 0  ), 500 , FALSE );

		m_fHeight = ( g_Pc.GetPlayer()->GetBoundMaxVec().y - g_Pc.GetPlayer()->GetBoundMinVec().y );

		SetCameraFront( 1.8f );	

		NextSquence();
	}

	return TRUE;
}


//..........................................................................................................
// Fade Out
//..........................................................................................................
int CRotDisplay::Squence01()
{
	SetWaitFlag( SWT_WAIT );

	return TRUE;
}

int CRotDisplay::SquenceCheck01()
{
	if( g_Particle.m_Fade.GetColor() == D3DCOLOR_ARGB( 0 , 0 , 0 , 0 ) )
	{
		NextSquence();
	}

	return TRUE;
}

//..........................................................................................................
// 대기 
//..........................................................................................................
int CRotDisplay::Squence02()
{
	SetWaitFlag( SWT_TIMER , 1000 );

	return TRUE;
}

//..........................................................................................................
// 카메라 회전
//..........................................................................................................
int CRotDisplay::Squence03()
{
	m_fRangeAngle	= 0.0f;
	m_OldTimer		= timeGetTime();

	D3DXVECTOR3	vLength = g_Camera.m_vLookat - g_Camera.m_vEye;
	
	m_fDistance = D3DXVec3Length( &vLength );

	SetPlayerAniTime( 2.0f );

	SetPlayerAni( m_AniIndex );

	SetWaitFlag( SWT_WAIT );

	return TRUE;
}

int CRotDisplay::SquenceCheck03()
{
	DWORD		SubTimer;

	SubTimer = SAFE_TIME_SUB( timeGetTime() , m_OldTimer );

	if( SAFE_TIME_COMPARE( SubTimer , >= , m_RotTimer ) )
	{
		SetCameraFront( 1.8f );	
		NextSquence();
	}
	else
	{
		float fRange = ( float ) SubTimer / ( float ) m_RotTimer;
		float fAngle = 360.0f * fRange;	

		D3DXMATRIX	RotY;
		
		D3DXMatrixRotationY( &RotY , D3DXToRadian( fAngle ) );

		D3DXVECTOR3 vPos;

		D3DXVec3TransformCoord( &vPos , &m_vDir , &RotY );

		float fAdd = ( sinf( D3DXToRadian( fAngle ) ) * 5.0f );
		
//		if( fAdd < 0 )
//		{
//			fAdd = -fAdd;
//		}

		vPos = vPos * ( m_fDistance + fAdd ) + g_Camera.m_vLookat;
		
		g_Camera.SetCamera( vPos , g_Camera.m_vLookat, g_Camera.m_vUp );
	}

	return TRUE;
}


int CRotDisplay::Squence04()
{
	SetWaitFlag( SWT_ANI );

	return TRUE;
}

int CRotDisplay::SquenceCheck04()
{
	if( g_Pc.GetPlayer()->m_ani_index == n_Stand_Peace9 )
		NextSquence();

	return TRUE;
}

int CRotDisplay::Squence05()
{
	SetWaitFlag( SWT_TIMER , 4000 );

	return TRUE;
}

int CRotDisplay::Squence06()
{
	return TRUE;
}

int CRotDisplay::Squence07()
{
	m_SaveCamera = g_Camera.GetSaveVlue();
	
	g_Particle.m_Fade.Start( FADE_STATE_FADE , D3DCOLOR_ARGB( 0 , 0 , 0 , 0  ), 
							 D3DCOLOR_ARGB( 255 , 0 , 0 , 0 ) , 500 , FALSE );

	
	SetWaitFlag( SWT_WAIT );

	return TRUE;
}

int CRotDisplay::SquenceCheck07()
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


int CRotDisplay::Squence08()
{
	SetWaitFlag( SWT_WAIT );

	return TRUE;
}

int CRotDisplay::SquenceCheck08()
{
	if( g_Particle.m_Fade.GetColor() == D3DCOLOR_ARGB( 0 , 0 , 0 , 0 ) )
	{
		NextSquence();
		g_RockClient.m_SceneManager.SetSoundPlayMode( SCENE_NORMAL );

	}

	return TRUE;
}

void CRotDisplay::SetCameraFront(float Range)
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

	m_vDir = -vDir;

	vEyePos.y += 4.0f;
	vLookAt.y += 4.0f;

	g_Camera.SetCamera( vEyePos , vLookAt, vUp );
}

void CRotDisplay::SetStartScene(void)
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

void CRotDisplay::SetEndScene(void)
{
	SetPlayerAniTime( 1.0f );

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