#include "CEFGloveDestruction.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include	<SOUND\Sound.H>

#include "..\\..\\CRenderManager.h"

extern long g_nowTime;

CEFGloveDestruction::CEFGloveDestruction()
{
	m_IsConEffect = TRUE;
}

CEFGloveDestruction::~CEFGloveDestruction()
{
}

void CEFGloveDestruction::Init(void)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr ) )
	{
		SetDestroy(TRUE);
		return;
	}
	
	m_Src_Ani_Index = m_pSrcChr->m_ani_index;
/*
	CParticleData	*pData = NULL;
	if( ( pData = g_Particle.m_ParticleData.GetKeyDataPointer( PT_SWORD_BALL_GATHER_CUT ) ) == NULL )
		return;
	m_SwordParticle.MakeLink( pData );
*/

	m_OldTimer = g_nowTime;
	
	//................................................................................	
	// 발동 사운드
	//................................................................................	
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( m_pExtraInfo->SkillCode , m_pSrcChr );
}


int CEFGloveDestruction::Render(float dtime)
{
	
/*
	D3DXMATRIX matLocal;	

	D3DXMatrixIdentity( &matLocal );

	g_RenderManager.SetTransform( D3DTS_WORLD , &matLocal );
	
	g_RenderManager.SetCullMode( D3DCULL_NONE );
	
	g_RenderManager.SetZWriteEnable( FALSE );
	g_RenderManager.SetZEnable( D3DZB_TRUE );
	
	g_RenderManager.SetAlphaBlendEnable( TRUE );
	g_RenderManager.SetSrcBlend( D3DBLEND_ONE );
	g_RenderManager.SetDestBlend(  D3DBLEND_ONE );
	
	m_SwordParticle.Render();

	g_RenderManager.SetCullMode( D3DCULL_CCW );
	
	g_RenderManager.SetZWriteEnable( TRUE );
	g_RenderManager.SetZEnable( TRUE );
	
	g_RenderManager.SetAlphaBlendEnable( FALSE );
*/

	return TRUE;
}

int CEFGloveDestruction::CheckFrame(void)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr ) )
	{
		SetDestroy(TRUE);
		return TRUE;
	}

	if( m_Src_Ani_Index != m_pSrcChr->m_ani_index )
	{
		SetAfterEffect();
		SetDestroy();
	}

	switch( GetSrcAniCount() )
	{
/*
		case 44:


			g_Particle.SetEffSetCon( 0.0f , EF_GLOVE_DESTRUCTION_ST , 0 , 4.1f , m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex()  );
						
			break;
*/
			case 50:
		
			SetDestroy();
	
			SetAfterEffect();

			break;
	}

	return TRUE;
}

int CEFGloveDestruction::Update(float dtime)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr, m_pDestChr ) )
	{
		SetDestroy(TRUE);
		return TRUE;
	}
/*
	m_SwordParticle.SetLinkPos( m_pSrcChr->m_pUserInfo->vSwordAxis[ 0 ] );
	m_SwordParticle.UpdateLink( dtime );
*/

	return TRUE;
}

//................................................................................................................
// 이펙트 처리후 뒤로올 이펙트 셋업
//................................................................................................................
int CEFGloveDestruction::SetAfterEffect(void)
{
	// 데미지 효과

	//................................................................................	
	// 어택 사운드
	//................................................................................	
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillAttackedSound( m_pExtraInfo->SkillCode , m_pDestChr );

	g_Particle.SetEffSetCon( 0.0f , EF_GLOVE_DESTRUCTION_ED , 0 , 4.1f , 
		m_pDestChr->GetUnique() , m_pDestChr->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr) );
/*

	m_pExtraInfo->AfterEffect = EF_GLOVE_DESTRUCTION_ED;
	m_pExtraInfo->DestType = 0;
	m_pExtraInfo->lifeTime = 2.6f;	

	
	g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , m_pDestChr , m_pExtraInfo );

  */


	return TRUE;
}
