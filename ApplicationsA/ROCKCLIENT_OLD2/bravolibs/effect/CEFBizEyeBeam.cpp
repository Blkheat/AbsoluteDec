#include "CEFBizEyeBeam.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include	<SOUND\Sound.H>

#include "..\\..\\CRenderManager.h"

extern long g_nowTime;


CEFBizEyeBeam::CEFBizEyeBeam()
{
	m_IsConEffect = TRUE;
}


CEFBizEyeBeam::~CEFBizEyeBeam()
{
}

void CEFBizEyeBeam::Init(void)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr , m_pDestChr ) )
	{
		SetDestroy(TRUE);
		return;
	}

	//...........................................................................................................
	m_vStartPos.x = m_pSrcChr->m_matRealHair._41;
	m_vStartPos.y = m_pSrcChr->m_matRealHair._42;
	m_vStartPos.z = m_pSrcChr->m_matRealHair._43;

	D3DXMATRIX matBone;
	m_pDestChr->GetBoneLinkTM( 2 , matBone );

	m_vDestPos = 	D3DXVECTOR3 ( matBone._41 ,
								  matBone._42 ,
								  matBone._43 );

	m_RangeTime0 = PositionToTimer( 30  , m_vStartPos  , m_vDestPos );
	m_RangeTime1 = PositionToTimer( 50 , m_vStartPos , m_vDestPos );
		
	m_BillLine.InitDevice( g_lpDevice );

	m_vPos[ 0 ] = m_vPos[ 1 ]  = m_vStartPos;

	m_pTexture = NULL;

	//m_pTexture = g_Particle.m_EffectTextureManager.GetTexture( "laser_up.tga" );
	CStringToCode	HashString( "laser_up.tga" );
	m_HashCode = HashString.GetHashCode();

	m_OldTimer = g_nowTime;

	//................................................................................	
	// 발동 사운드
	//................................................................................	
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( m_pExtraInfo->SkillCode , m_pSrcChr );
}


int	CEFBizEyeBeam::CheckFrame(void)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr ) )
	{
		SetDestroy(TRUE);
		return TRUE;
	}

	return TRUE;
}

int	CEFBizEyeBeam::Render(float dtime)
{
	g_RenderManager.SetTransform( D3DTS_WORLD , &g_Particle.m_matIdentity );
	
	g_RenderManager.SetCullMode( D3DCULL_NONE );
	
	g_RenderManager.SetZWriteEnable( TRUE );
	g_RenderManager.SetZEnable(TRUE );
	
	g_RenderManager.SetAlphaBlendEnable( TRUE );
	g_RenderManager.SetSrcBlend( D3DBLEND_ONE );
	g_RenderManager.SetDestBlend( D3DBLEND_ONE );

	g_RenderManager.SetTexture( 0 , g_Particle.m_EffectTextureManager.GetTexture(m_HashCode) );
	m_BillLine.Render( g_lpDevice );

	g_RenderManager.SetCullMode( D3DCULL_NONE );
	g_RenderManager.SetZWriteEnable( FALSE );
	g_RenderManager.SetAlphaBlendEnable( FALSE );
	g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
	g_RenderManager.SetDestBlend( D3DBLEND_INVSRCALPHA );

	return TRUE;
}

int CEFBizEyeBeam::Update(float dtime)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr , m_pDestChr) )
	{
		SetDestroy(TRUE);
		return TRUE;
	}

	D3DXMATRIX matBone;
	m_pDestChr->GetBoneLinkTM( 2 , matBone );

	m_vDestPos = 	D3DXVECTOR3 ( matBone._41 ,
								  matBone._42 ,
								  matBone._43 );
	
	m_vStartPos.x = m_pSrcChr->m_matRealHair._41;
	m_vStartPos.y = m_pSrcChr->m_matRealHair._42;
	m_vStartPos.z = m_pSrcChr->m_matRealHair._43;

	D3DXVECTOR3 vDir;
		
	vDir = m_vDestPos - m_vStartPos;

	float       Range0     = ( ( ( float ) ( SAFE_TIME_SUB( g_nowTime , m_OldTimer ) ) / ( float ) m_RangeTime0 ) );
	float       Range1     = ( ( ( float ) ( SAFE_TIME_SUB( g_nowTime , m_OldTimer ) ) / ( float ) m_RangeTime1 ) );

	if( Range0 > 1.0f ) 
		Range0 = 1.0f;

	if( Range1 > 1.0f )
	{
		Range1 = 1.0f;

		SetAfterEffect();
		SetDestroy();
	}

	m_vPos[ 0 ] = m_vStartPos + ( vDir * Range0 );  
	m_vPos[ 1 ] = m_vStartPos;// + ( vDir * Range1 );  

	m_BillLine.Update( m_vPos[ 0 ] , m_vPos[ 1 ] , 1.0f , g_Camera.m_vDir , g_lpDevice );

	return TRUE;
}

int CEFBizEyeBeam::SetAfterEffect(void)
{
	g_Particle.SetEffSetCon( 0.0f , EF_NPC_BIZ_BEAMED , ESLT_POS , 8.1f , -1 , -1 , 
		D3DXVECTOR3( m_pDestChr->GetPosTM()._41 , m_pDestChr->GetPosTM()._42 + 10.0f, m_pDestChr->GetPosTM()._43  ) );

	g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , m_pDestChr , m_pExtraInfo );

	//................................................................................	
	// 어택 사운드
	//................................................................................	
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillAttackedSound( m_pExtraInfo->SkillCode , m_pDestChr );

	return TRUE;
}
