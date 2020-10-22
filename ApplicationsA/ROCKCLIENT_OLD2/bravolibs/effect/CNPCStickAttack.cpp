#include "CNPCStickAttack.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include	<SOUND\Sound.H>

#include "..\\..\\CRenderManager.h"

extern long g_nowTime;


CEFNPCStickAttack::CEFNPCStickAttack()
{
	m_IsConEffect = TRUE;
}


CEFNPCStickAttack::~CEFNPCStickAttack()
{

}

void CEFNPCStickAttack::Init(void)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr , m_pDestChr ) )
	{
		SetDestroy(TRUE, TRUE);
		return;
	}

	CParticleData	*pData = NULL;

	if( ( pData = g_Particle.m_ParticleData.GetKeyDataPointer( PT_NPC_MOUTH_FIRE_BALL1 ) ) == NULL )
		return;

	m_BallParticle.m_pReference	    = pData;

	m_BallParticle.m_vMoveLocation	= D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
	m_BallParticle.m_bIsSuppressed	= false;
	m_BallParticle.m_bIsCreate		= true;
	m_BallParticle.m_lUnique		= -1;
	m_BallParticle.m_iIndex			= -1;
	m_BallParticle.m_fSize			= 1.0f;
	
	m_OldTimer    = g_nowTime;
	
	D3DXMATRIX matStart;

	m_pSrcChr->GetBoneLinkTM(5,matStart);

	m_vStartPos   = D3DXVECTOR3( matStart._41 , matStart._42 , matStart._43 );

	D3DXMATRIX matBone;
	m_pDestChr->GetBoneLinkTM( 2 , matBone );
	D3DXVECTOR3 vDestPos( matBone._41 ,
						  matBone._42 ,
						  matBone._43 );

	D3DXVECTOR3 vDistance = vDestPos - m_vStartPos;

	m_RangeTime	  = PositionToTimer( 70 , m_vStartPos , vDestPos );

	D3DXMatrixIdentity( &m_BallParticle.m_LinkMat );

/*
	// 지팡이 끝에 빌보드 
	g_Billboard.SetManager( 0.0f , BB_STICK_STAND_ATTACK0 , m_vStartPos );
*/
	
}


int	CEFNPCStickAttack::CheckFrame(void)
{

	return TRUE;
}

int	CEFNPCStickAttack::Render(float dtime)
{
	D3DXMATRIX matLocal;	

	D3DXMatrixIdentity( &matLocal );

	g_RenderManager.SetTransform( D3DTS_WORLD , &matLocal );
	
	g_RenderManager.SetCullMode( D3DCULL_NONE );
	
	g_RenderManager.SetZWriteEnable( FALSE );
	g_RenderManager.SetZEnable( TRUE );
	
	g_RenderManager.SetAlphaBlendEnable(  TRUE );
	g_RenderManager.SetSrcBlend( D3DBLEND_ONE );
	g_RenderManager.SetDestBlend( D3DBLEND_ONE );

	m_BallParticle.Render();

	g_RenderManager.SetCullMode( D3DCULL_CCW );
	
	g_RenderManager.SetZWriteEnable( TRUE );
	g_RenderManager.SetZEnable( TRUE );
	
	g_RenderManager.SetAlphaBlendEnable(  FALSE );

	return TRUE;
}

int CEFNPCStickAttack::Update(float dtime)
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
	
	m_BallParticle.SetLinkPos( vPos );
	
	m_BallParticle.UpdateLink( dtime );

	g_Particle.DisplayUpdate( PT_NPC_STICK_BALL , vPos );

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

int CEFNPCStickAttack::SetAfterEffect(void)
{
	g_Particle.m_EffectManager.StandTargetHit( m_pSrcChr , m_pDestChr , m_pExtraInfo );

	return TRUE;
}
