#include "CEFNpcFireBall.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include	<SOUND\Sound.H>

#include "..\\..\\CRenderManager.h"

extern long g_nowTime;

CEFNpcFireBall::CEFNpcFireBall()
{
	m_IsConEffect = TRUE;
}

CEFNpcFireBall::~CEFNpcFireBall()
{
}

void CEFNpcFireBall::Init(void)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr , m_pDestChr ) )
	{
		SetDestroy(TRUE, TRUE);
		return;
	}

	CParticleData	*pData = NULL;

	if( ( pData = g_Particle.m_ParticleData.GetKeyDataPointer( PT_NPC_FIRE_BALL ) ) == NULL )
	{
		SetDestroy(TRUE, TRUE);
		return;
	}

	m_SpotParticle.MakeLink( pData );
	
	m_OldTimer    = g_nowTime;
	m_vStartPos   = m_pSrcChr->m_pUserInfo->vSwordAxis[ 0 ];
	
	D3DXMATRIX matBone;
	m_pDestChr->GetBoneLinkTM( 2 , matBone );
	D3DXVECTOR3 vDestPos( matBone._41 ,
						  matBone._42 ,
						  matBone._43 );

	D3DXVECTOR3 vDistance = vDestPos - m_vStartPos;

	m_RangeTime	  = PositionToTimer( 70 , m_vStartPos , vDestPos );

	D3DXMatrixIdentity( &m_SpotParticle.m_LinkMat );

	//................................................................................	
	// 발동 사운드
	//................................................................................	
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( m_pExtraInfo->SkillCode , m_pSrcChr );
}


int CEFNpcFireBall::Render(float dtime)
{
	D3DXMATRIX matLocal;	

	D3DXMatrixIdentity( &matLocal );

	g_RenderManager.SetTransform( D3DTS_WORLD , &matLocal );
	
	g_RenderManager.SetCullMode( D3DCULL_NONE );
	
	g_RenderManager.SetZWriteEnable( FALSE );
	g_RenderManager.SetZEnable( TRUE );
	
	g_RenderManager.SetAlphaBlendEnable(  TRUE );	

	m_SpotParticle.Render();

	g_RenderManager.SetCullMode( D3DCULL_CCW );
	
	g_RenderManager.SetZWriteEnable( TRUE );
	g_RenderManager.SetZEnable( TRUE );
	
	g_RenderManager.SetAlphaBlendEnable(  FALSE );

	return TRUE;
}

int CEFNpcFireBall::Update(float dtime)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr , m_pDestChr ) )
	{
		SetDestroy(TRUE, TRUE);
		return TRUE;
	}

	D3DXMATRIX matBone;
	m_pDestChr->GetBoneLinkTM( 2 , matBone );
	D3DXVECTOR3 vDestPos( matBone._41 ,
						  matBone._42 ,
						  matBone._43 );
	
	D3DXVECTOR3 vDistance = vDestPos - m_vStartPos;
	
	float       Range     = ( ( ( float ) SAFE_TIME_SUB( g_nowTime , m_OldTimer ) / ( float ) m_RangeTime ) );

	if( Range > 1.0f ) 
		Range = 1.0f;

	D3DXVECTOR3 vPos      = m_vStartPos + ( vDistance * Range );  
	
	// vPos.y += ( D3DXVec3Length( &vDistance ) * 0.07f * sinf( ( float ) D3DXToRadian( 180.0f * Range ) ) );

	m_SpotParticle.SetLinkPos( vPos );
	
	m_SpotParticle.UpdateLink( dtime );

	// g_Particle.DisplayUpdate( PR_PRIEST_BALL_TAIL , vPos );
	
	//............................................................................................................
	// 파이어볼 캐릭터에 충돌 처리 ( 이펙트 .... T.T )
	//............................................................................................................
	if( SAFE_TIME_COMPARE( g_nowTime , > , SAFE_TIME_ADD( m_RangeTime , m_OldTimer ) ) )
	{
		SetDestroy();
	
		SetAfterEffect();
	}

	return TRUE;
}

//................................................................................................................
// 이펙트 처리후 뒤로올 이펙트 셋업
//................................................................................................................
int CEFNpcFireBall::SetAfterEffect(void)
{
	switch( m_ID )
	{
		case EF_NPC_FIRE:
			
			g_Particle.m_EffectManager.StandTargetHit( m_pSrcChr , m_pDestChr , m_pExtraInfo );

			return TRUE;
	}

	g_Particle.SetEffSetCon( 0.0f , EF_NPC_FIREED2 , 0 , 2.1f , 
							 m_pDestChr->GetUnique() , m_pDestChr->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr) );
	
	if(m_pExtraInfo)
	{
		m_pExtraInfo->AfterEffect = EF_NPC_FIREED2;
		m_pExtraInfo->DestType = 0;
		m_pExtraInfo->lifeTime = 2.1f;
	}
	
	g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , m_pDestChr , m_pExtraInfo );

	return TRUE;
}
