#include "CEFGageTrans.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include	<SOUND\Sound.H>

#include "..\\..\\CRenderManager.h"

extern long g_nowTime;

CEFGageTrans::CEFGageTrans()
{
}

CEFGageTrans::~CEFGageTrans()
{
}

void CEFGageTrans::Init(void)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr , m_pDestChr ) )
	{
		SetDestroy();

		return;
	}

	CParticleData	*pData = NULL;

	if( ( pData = g_Particle.m_ParticleData.GetKeyDataPointer( PT_GAGE_BALL ) ) == NULL )
		return;

	m_SpotParticle.MakeLink( pData );
	
	m_OldTimer    = g_nowTime;
	
	D3DXMATRIX matPos;	

	m_pSrcChr->GetBoneLinkTM( 1 , matPos );
	
	m_vStartPos = D3DXVECTOR3( matPos._41 , matPos._42 , matPos._43 );

	D3DXMATRIX matBone;
	m_pDestChr->GetBoneLinkTM( 2 , matBone );

	D3DXVECTOR3 vDestPos( matBone._41 ,
						  matBone._42 ,
						  matBone._43 );

	D3DXVECTOR3 vDistance = vDestPos - m_vStartPos;

	m_RangeTime	  = PositionToTimer( 70 , m_vStartPos , vDestPos );

	D3DXMatrixIdentity( &m_SpotParticle.m_LinkMat );

/*
	//................................................................................	
	// 발동 사운드
	//................................................................................	
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( m_pExtraInfo->SkillCode , m_pSrcChr );
*/
	g_Particle.SetEffSetCon( 0.0f , EF_GAGE_TRANS , 0 , 4.1f , m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex() );
	g_Particle.SetEffSetCon( 0.0f , EF_SUB_GAGE   , 0 , 4.1f , m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex() );
}


int CEFGageTrans::Render(float dtime)
{
	D3DXMATRIX matLocal;	

	D3DXMatrixIdentity( &matLocal );

	g_RenderManager.SetTransform( D3DTS_WORLD , &matLocal );
	
	g_RenderManager.SetCullMode( D3DCULL_NONE );
	
	g_RenderManager.SetZWriteEnable(FALSE );
	g_RenderManager.SetZEnable( D3DZB_TRUE);
	
	g_RenderManager.SetAlphaBlendEnable( TRUE);
	
	m_SpotParticle.Render();

	g_RenderManager.SetCullMode( D3DCULL_CCW );
	
	g_RenderManager.SetZWriteEnable(TRUE );
	g_RenderManager.SetZEnable( TRUE);
	
	g_RenderManager.SetAlphaBlendEnable( FALSE);
	
	return TRUE;
}

int CEFGageTrans::Update(float dtime)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr , m_pDestChr ) )
	{
		SetDestroy();

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
	
	vPos.y += ( 10.0f * sinf( ( float ) D3DXToRadian( 180.0f * Range ) ) );

	m_SpotParticle.SetLinkPos( vPos );
	
	m_SpotParticle.UpdateLink( dtime );

	g_Particle.DisplayUpdate( PT_GAGE_BALL_TAIL , vPos );
	
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
int CEFGageTrans::SetAfterEffect(void)
{
	g_Particle.SetEffSetCon( 0.0f , EF_GAGE_TRANSED , 0 , 4.1f , 
			   m_pDestChr->GetUnique() , m_pDestChr->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr) );
//	g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , m_pDestChr , m_pExtraInfo );
	g_Particle.SetEffSetCon( 0.0f , EF_PLUS_GAGE , 0 , 4.1f , 
			   m_pDestChr->GetUnique() , m_pDestChr->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr) );

	return TRUE;
}
