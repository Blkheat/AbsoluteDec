#include "CEFFireBall.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include	<SOUND\Sound.H>
#include "..\\..\\CRenderManager.h"

extern long g_nowTime;


int CEFFireBall::InitDevice(void)
{
	return TRUE;
}

int	CEFFireBall::DeleteDevice(void)
{
	return TRUE;
}


CEFFireBall::CEFFireBall()
{
	m_IsConEffect = TRUE;
}

CEFFireBall::~CEFFireBall()
{
}

void CEFFireBall::Init(void)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr , m_pDestChr ) )
	{
		SetDestroy(TRUE);
		return;
	}		
	
	m_OldTimer    = g_nowTime;
	
	m_vStartPos   = D3DXVECTOR3( m_pSrcChr->GetRealRHandTM()._41 , 
								 m_pSrcChr->GetRealRHandTM()._42 ,
								 m_pSrcChr->GetRealRHandTM()._43  );
	
	D3DXMATRIX matBone;
	m_pDestChr->GetBoneLinkTM( 2 , matBone );

	D3DXVECTOR3 vDestPos( matBone._41 ,
						  matBone._42 ,
						  matBone._43 );

	D3DXVECTOR3 vDistance = vDestPos - m_vStartPos;

	CParticleData	*pData = NULL;
	if( ( pData = g_Particle.m_ParticleData.GetKeyDataPointer( PT_FIREBALL ) ) == NULL )
		return;

	m_FireParticle.MakeLink(pData);

	m_RangeTime	  = PositionToTimer( 70 , m_vStartPos , vDestPos );	

	//................................................................................	
	// 발동 사운드
	//................................................................................	
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( m_pExtraInfo->SkillCode , m_pSrcChr );
}


int CEFFireBall::Render(float dtime)
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

	m_FireParticle.Render();	

	g_RenderManager.SetCullMode( D3DCULL_CCW );
	
	g_RenderManager.SetZWriteEnable( TRUE );
	g_RenderManager.SetZEnable( TRUE );

	return TRUE;
}

int CEFFireBall::CheckFrame(void)
{
	return TRUE;
}

int CEFFireBall::Update(float dtime)
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
	
	// vPos.y += ( D3DXVec3Length( &vDistance ) * 0.07f * sinf( ( float ) D3DXToRadian( 180.0f * Range ) ) );

	m_FireParticle.SetLinkPos( vPos );	
	m_FireParticle.UpdateLink( dtime );

	g_Particle.DisplayUpdate( PT_FIREBALL_TAIL , vPos );	

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
int CEFFireBall::SetAfterEffect(void)
{
	int EffectID = 0;
	
	switch( m_ID )
	{
		case EF_FIREBALL0:
			EffectID = EF_FIREBALL_ATTACKED0;
			break;
		case EF_FIREBALL1:
			EffectID = EF_FIREBALL_ATTACKED1;
			break;
		case EF_FIREBALL2:
			EffectID = EF_FIREBALL_ATTACKED2;
			break;
		case EF_FIREBALL3:
			EffectID = EF_FIREBALL_ATTACKED3;
			break;
		case EF_FIREBALL4:
			EffectID = EF_FIREBALL_ATTACKED4;
			break;
	}

	// 데미지 효과
	
	//................................................................................	
	// 어택 사운드
	//................................................................................	
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillAttackedSound( m_pExtraInfo->SkillCode , m_pDestChr );

	g_Particle.SetEffSetCon( 0.0f , EffectID , 0 , 2.1f , 
							 m_pDestChr->GetUnique() , m_pDestChr->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr) );

	if( m_pExtraInfo != NULL )
	{
		m_pExtraInfo->AfterEffect = EffectID;
		m_pExtraInfo->DestType = 0;
		m_pExtraInfo->lifeTime = 2.1f;	
	
		g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , m_pDestChr , m_pExtraInfo );
	}
	
	if( m_pSrcChr == g_Pc.GetPlayer() )
		g_Particle.RockCamera( ( rand() % 200 ) + 300 );


	return TRUE;
}
