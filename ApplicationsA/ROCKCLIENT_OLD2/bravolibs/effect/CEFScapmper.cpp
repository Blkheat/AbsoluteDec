#include "CEFScapmper.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include	<SOUND\Sound.H>
#include "..\\..\\CRenderManager.h"

extern long		 g_nowTime;

#define SCAPMPER_MAX_COUNT	2

CEFScapmper::CEFScapmper()
{
}

CEFScapmper::~CEFScapmper()
{
	g_Particle.DelEffSetCon( m_ID , 0 , m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex() );
}

int CEFScapmper::Update(float dtime)
{
	if( g_Particle.m_EffectManager.CheckDie( m_pSrcChr ) )
	{
		SetDestroy();

		return TRUE;
	}
	
	if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( g_nowTime , m_OldTime ) , > , 80 ) )
	{
		D3DXVECTOR3 vPos;

		vPos = D3DXVECTOR3( m_pSrcChr->GetPosTM()._41 , m_pSrcChr->GetPosTM()._42 , m_pSrcChr->GetPosTM()._43 );
		
		m_FootList.push_front( vPos );
		
		if( m_FootList.size() > SCAPMPER_MAX_COUNT )
			m_FootList.pop_back();

		m_OldTime = g_nowTime;
	}
	
	return TRUE;
}

int	CEFScapmper::Render(float dtime)
{
	D3DXMATRIX matLocal;	
 
	D3DXMatrixIdentity( &matLocal );

	g_RenderManager.SetTransform( D3DTS_WORLD , &matLocal );
	
	//static 
	float AlphaList[] = {
		0.5f,
		0.2f,
		0.6f,
		0.5f,
		0.4f,
		0.3f	
	};
	
	float Alpha = 0.0f;

	g_RenderManager.SetFVF( D3DFVF_OBJECTVERTEX );	
	g_RenderManager.SetLight( 0, &g_Pc_Manager.theCharacter_Light );

	g_RenderManager.LightEnable( 0, TRUE );

	g_RenderManager.SetAlphaBlendEnable( TRUE );
	g_RenderManager.SetLighting( TRUE );

	g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureColorArg1( 0 , D3DTA_TEXTURE );
	g_RenderManager.SetTextureColorArg2( 0 , D3DTA_DIFFUSE );
	
	
	g_RenderManager.SetTextureAlphaOP  ( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureAlphaArg1( 0 , D3DTA_DIFFUSE );
	g_RenderManager.SetTextureAlphaArg2( 0 , D3DTA_TEXTURE );

	g_RenderManager.DisableTextureStageState( 1 );
	g_RenderManager.DisableTextureStageState( 2 );

	for( int i = 0 ; i < m_FootList.size() ; ++i )
	{
		m_pSrcChr->RenderToAlpha( m_FootList[ i ] + D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
								  AlphaList[ i ], FALSE );
	}

	g_RenderManager.SetLighting( FALSE );
	g_RenderManager.LightEnable( 0, FALSE );

	g_RenderManager.SetAlphaBlendEnable( FALSE );
	
	return TRUE;
}

void CEFScapmper::Init(void)
{
	m_OldTime = g_nowTime;

	g_Particle.SetEffSetCon( 0.0f , m_ID , 0 , EFFECT_SET_LOOP_TIME , m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex() );

	//................................................................................	
	// 발동 사운드
	//................................................................................	
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( m_pExtraInfo->SkillCode , m_pSrcChr );

}