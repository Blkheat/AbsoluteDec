#include "CEFHealBubble.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include	<SOUND\Sound.H>
#include "..\\..\\CRenderManager.h"

extern long g_nowTime;

CEFHealBubble::CEFHealBubble()
{
	m_IsConEffect = TRUE;
}

CEFHealBubble::~CEFHealBubble()
{
}

void CEFHealBubble::Init(void)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr , m_pDestChr ) )
	{
		SetDestroy(TRUE);
		return;
	}

	CBillboardData *pData = NULL ;
	
	if( NULL == ( pData = g_Billboard.m_BillboardData.GetKeyDataPointer( 22409 ) ) )
	{
		return;
	}

	m_Billboard.MakeLink( pData );

	m_OldTimer    = g_nowTime;
	
	D3DXMATRIX matStart;

	// 머리 위치 구하기
	m_pSrcChr->GetBoneLinkTM( 1 , matStart );

	m_vStartPos.x = matStart._41;
	m_vStartPos.y = matStart._42;
	m_vStartPos.z = matStart._43;
	
	D3DXMATRIX matBone;
	m_pDestChr->GetBoneLinkTM( 2 , matBone );
	D3DXVECTOR3 vDestPos( matBone._41 ,
						  matBone._42 ,
						  matBone._43 );

	D3DXVECTOR3 vDistance = vDestPos - m_vStartPos;

	m_RangeTime	  = PositionToTimer( 70 , m_vStartPos , vDestPos );

	//................................................................................	
	// 발동 사운드
	//................................................................................	
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( m_pExtraInfo->SkillCode , m_pSrcChr );
}


int CEFHealBubble::Render(float dtime)
{
	D3DXMATRIX matLocal;	

	D3DXMatrixIdentity( &matLocal );

	g_RenderManager.SetTransform( D3DTS_WORLD , &matLocal );
	
	g_RenderManager.SetCullMode( D3DCULL_NONE );
	
	g_RenderManager.SetZWriteEnable( FALSE );
	g_RenderManager.SetZEnable( D3DZB_TRUE );
	
	g_RenderManager.SetAlphaBlendEnable( TRUE );
	g_RenderManager.SetSrcBlend( D3DBLEND_ONE );
	g_RenderManager.SetDestBlend( D3DBLEND_ONE );

#ifdef BATCH_RENDER
	m_Billboard.DrawBatchBillboard();
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DTEXTURE9	prevBillboardTexture = g_RenderManager.GetPrevBillboardTexture();
#else
	LPDIRECT3DTEXTURE8	prevBillboardTexture = g_RenderManager.GetPrevBillboardTexture();
#endif // DIRECT_VERSION_9_MJH
	if(prevBillboardTexture)
	{
		g_RenderManager.SetTexture( 0, prevBillboardTexture );
		g_RenderManager.SetSrcBlend( g_RenderManager.m_PrevSrcBlend );
		g_RenderManager.SetDestBlend( g_RenderManager.m_PrevDestBlend );
		g_RenderManager.EndBatchPrimitive();
		g_RenderManager.SetBeginBatchRendering(FALSE);
	}
#else	//	BATCH_RENDER
	m_Billboard.DrawBillboard();
#endif	//	BATCH_RENDER
	

	g_RenderManager.SetCullMode( D3DCULL_CCW );
	
	g_RenderManager.SetZWriteEnable( TRUE );
	g_RenderManager.SetZEnable( TRUE );
	
	g_RenderManager.SetAlphaBlendEnable( FALSE );

	return TRUE;
}

int CEFHealBubble::Update(float dtime)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr , m_pDestChr ) )
	{
		SetDestroy(TRUE);
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
	
	m_Billboard.m_vLoc = vPos;

	g_Billboard.DisplayManager( m_Billboard , dtime );

	g_Particle.DisplayUpdate( 21409 , vPos );
	
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
int CEFHealBubble::SetAfterEffect(void)
{
	//................................................................................	
	// 어택 사운드
	//................................................................................	
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillAttackedSound( m_pExtraInfo->SkillCode , m_pDestChr );

	g_Particle.SetEffSetCon( 0.0f , EF_HEAL_BUBBLE_ED , 0 , 2.1f , 
		m_pDestChr->GetUnique() , m_pDestChr->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr) );

	if(m_pExtraInfo)
	{
		m_pExtraInfo->AfterEffect = EF_HEAL_BUBBLE_ED;
		m_pExtraInfo->DestType = 0;
		m_pExtraInfo->lifeTime = 2.1f;
	}
	
	g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , m_pDestChr , m_pExtraInfo );
	
	return TRUE;
}
