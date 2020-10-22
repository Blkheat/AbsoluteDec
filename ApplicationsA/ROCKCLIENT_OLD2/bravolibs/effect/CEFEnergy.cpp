#include "CEFEnergy.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include	<SOUND\Sound.H>
#include "..\\..\\CRenderManager.h"

extern long g_nowTime;

CEFEnergy::CEFEnergy()
{
	m_IsConEffect = TRUE;
}

CEFEnergy::~CEFEnergy()
{
}

void CEFEnergy::Init(void)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr , m_pDestChr ) )
	{
		SetDestroy(TRUE);
		return;
	}

	switch( m_ID )
	{
		case EF_ENERGY0:
			
			m_Head = 21726;
			m_Tail = 21729;
			
			break;

		case EF_ENERGY1:
			
			m_Head = 21727;
			m_Tail = 21729;

			break;
		
		case EF_ENERGY2:

			m_Head = 21728;
			m_Tail = 21729;
			
			break;
	}



	CParticleData	*pData = NULL;

	if( ( pData = g_Particle.m_ParticleData.GetKeyDataPointer( m_Head ) ) == NULL )
		return;

	m_FireParticle.m_pReference	    = pData;

	m_FireParticle.m_vMoveLocation	= D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
	m_FireParticle.m_bIsSuppressed	= false;
	m_FireParticle.m_bIsCreate		= true;
	m_FireParticle.m_lUnique		= -1;
	m_FireParticle.m_iIndex			= -1;
	m_FireParticle.m_fSize			= 1.0f;
	
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

	DWORD RangeTime	  = PositionToTimer( 70 , m_vStartPos , vDestPos );

	D3DXMatrixIdentity( &m_FireParticle.m_LinkMat );

	m_Range.SetMaxTime( RangeTime );
	m_Range.Insert( 0 , 0.0f );
	m_Range.Insert( RangeTime , 1.0f );
	
	//................................................................................	
	// 발동 사운드
	//................................................................................	
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( m_pExtraInfo->SkillCode , m_pSrcChr );
}


int CEFEnergy::Render(float dtime)
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

	m_FireParticle.Render();

	g_RenderManager.SetCullMode( D3DCULL_CCW );
	
	g_RenderManager.SetZWriteEnable( TRUE );
	g_RenderManager.SetZEnable( TRUE );
	
	g_RenderManager.SetAlphaBlendEnable( FALSE );

	return TRUE;
}

int CEFEnergy::Update(float dtime)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr , m_pDestChr ) )
	{
		SetDestroy(TRUE);
		return TRUE;
	}

	float Range;
	//............................................................................................................
	// 파이어볼 캐릭터에 충돌 처리 ( 이펙트 .... T.T )
	//............................................................................................................
	if( m_Range.GetValue( SAFE_TIME_SUB( g_nowTime , m_OldTimer ) , Range ) == FALSE )
	{
		SetDestroy();
	
		SetAfterEffect();
	
		return TRUE;
	}
	
	D3DXMATRIX matBone;
	m_pDestChr->GetBoneLinkTM( 2 , matBone );

	D3DXVECTOR3 vDestPos( matBone._41 ,
						  matBone._42 ,
						  matBone._43 );	
	
	D3DXVECTOR3 vPos;

	D3DXVec3Lerp( &vPos , &m_vStartPos , &vDestPos , Range );
	
	m_FireParticle.SetLinkPos( vPos );
	
	m_FireParticle.UpdateLink( dtime );

	g_Particle.DisplayUpdate( m_Tail , vPos );
	
	return TRUE;
}

//................................................................................................................
// 이펙트 처리후 뒤로올 이펙트 셋업
//................................................................................................................
int CEFEnergy::SetAfterEffect(void)
{
	int EffectID = 0;
	
	switch( m_ID )
	{
		case EF_ENERGY0:
			EffectID = EF_ENERGY0_ED;
			break;
		case EF_ENERGY1:
			EffectID = EF_ENERGY1_ED;
			break;
		case EF_ENERGY2:
			EffectID = EF_ENERGY2_ED;
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

	if(m_pExtraInfo)
	{
		m_pExtraInfo->AfterEffect = EffectID;
		m_pExtraInfo->DestType = 0;
		m_pExtraInfo->lifeTime = 2.1f;
	}
	
	g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , m_pDestChr , m_pExtraInfo );
	
	if( m_pSrcChr == g_Pc.GetPlayer() )
		g_Particle.RockCamera( ( rand() % 200 ) + 300 );


	return TRUE;
}
