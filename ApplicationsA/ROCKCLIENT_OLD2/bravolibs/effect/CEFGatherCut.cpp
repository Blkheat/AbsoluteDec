#include "CEFGatherCut.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include	<SOUND\Sound.H>

#include "..\\..\\CRenderManager.h"

extern long g_nowTime;

CEFGatherCut::CEFGatherCut()
{
	m_IsConEffect = TRUE;
}

CEFGatherCut::~CEFGatherCut()
{
}

void CEFGatherCut::Init(void)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr ) )
	{
		SetDestroy(TRUE);
		return;
	}
	
	m_Src_Ani_Index = m_pSrcChr->m_ani_index;

	CParticleData	*pData = NULL;

	if( ( pData = g_Particle.m_ParticleData.GetKeyDataPointer( PT_SWORD_BALL_GATHER_CUT ) ) == NULL )
		return;

	m_SwordParticle.MakeLink( pData );

	m_OldTimer = g_nowTime;
	
	//................................................................................	
	// 발동 사운드
	//................................................................................	
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( m_pExtraInfo->SkillCode , m_pSrcChr );
}


int CEFGatherCut::Render(float dtime)
{
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

	return TRUE;
}

int CEFGatherCut::CheckFrame(void)
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
		case 10:
			
			m_pSrcChr->m_IsApplyVA = TRUE;
			m_pSrcChr->m_IsNextApplyVA = TRUE;
			m_pSrcChr->BoneRender();
			
			g_Particle.DisplayUpdate( PT_SWORD_GATHER_CUT , m_pSrcChr->m_pUserInfo->vSwordAxis[ 0 ] );
			
			break;

		case 39:
		
			SetDestroy();
	
			SetAfterEffect();

			break;
	}

	return TRUE;
}

int CEFGatherCut::Update(float dtime)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr, m_pDestChr ) )
	{
		SetDestroy(TRUE);
		return TRUE;
	}

	m_SwordParticle.SetLinkPos( m_pSrcChr->m_pUserInfo->vSwordAxis[ 0 ] );
	
	m_SwordParticle.UpdateLink( dtime );

	return TRUE;
}

//................................................................................................................
// 이펙트 처리후 뒤로올 이펙트 셋업
//................................................................................................................
int CEFGatherCut::SetAfterEffect(void)
{
	// 데미지 효과

	//................................................................................	
	// 어택 사운드
	//................................................................................	
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillAttackedSound( m_pExtraInfo->SkillCode , m_pDestChr );

	g_Particle.SetEffSetCon( 0.0f , EF_GATHER_CUT_ATTACKED , 0 , 2.6f , 
		m_pDestChr->GetUnique() , m_pDestChr->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr) );

	if(m_pExtraInfo)
	{
		m_pExtraInfo->AfterEffect = EF_GATHER_CUT_ATTACKED;
		m_pExtraInfo->DestType = 0;
		m_pExtraInfo->lifeTime = 2.6f;
	}
	
	g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , m_pDestChr , m_pExtraInfo );

	return TRUE;
}
