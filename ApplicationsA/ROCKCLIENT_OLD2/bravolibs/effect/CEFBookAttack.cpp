#include "CEFBookAttack.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include	<SOUND\Sound.H>
#include "..\\..\\CRenderManager.h"

extern long g_nowTime;


CEFBookAttack::CEFBookAttack()
{
	m_IsConEffect = TRUE;
}


CEFBookAttack::~CEFBookAttack()
{

}

void CEFBookAttack::Init(void)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr , m_pDestChr ) )
	{
		SetDestroy(TRUE, TRUE);
		return;
	}

	CParticleData	*pData = NULL;

	if( ( pData = g_Particle.m_ParticleData.GetKeyDataPointer( PT_BOOK_STAND_ATTACK0 ) ) == NULL )
	{
		SetDestroy(TRUE, TRUE);
		return;
	}

	m_BallParticle.m_pReference	    = pData;

	m_BallParticle.m_vMoveLocation	= D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
	m_BallParticle.m_bIsSuppressed	= false;
	m_BallParticle.m_bIsCreate		= true;
	m_BallParticle.m_lUnique		= -1;
	m_BallParticle.m_iIndex			= -1;
	m_BallParticle.m_fSize			= 1.0f;
	
	m_OldTimer    = g_nowTime;
	
	switch( m_pSrcChr->m_ani_index )
	{
		case n_Attack5_Basic0:
		case n_Attack5_Basic1:
		case n_Attack5_Basic2:
		case n_Attack5_KeyX0:
		case n_Attack5_KeyX1:
		case n_Attack5_KeyX2:
		case n_Attack5_KeyX3:
		case n_Attack5_KeyX4:
		case n_Attack5_KeyC0:
		case n_Attack5_KeyC1:
		case n_Attack5_KeyC2:
		case n_Attack5_KeyC3:
		case n_Attack5_KeyC4:
			
			m_vStartPos   = D3DXVECTOR3( m_pSrcChr->GetRealRHandTM()._41 , 
				m_pSrcChr->GetRealRHandTM()._42 , 	
				m_pSrcChr->GetRealRHandTM()._43   ); 
			
			break;

		case n_Attack5_Basic3:
		case n_Attack5_KeyZ0:
		case n_Attack5_KeyZ1:
		case n_Attack5_KeyZ2:
		case n_Attack5_KeyZ3:
		case n_Attack5_KeyZ4:


			m_vStartPos   = D3DXVECTOR3( m_pSrcChr->GetRealLHandTM()._41 , 
				m_pSrcChr->GetRealLHandTM()._42 , 	
				m_pSrcChr->GetRealLHandTM()._43   ); 
			
			break;

		default:
			
			m_vStartPos   = D3DXVECTOR3( m_pSrcChr->GetRealRHandTM()._41 , 
				m_pSrcChr->GetRealRHandTM()._42 , 	
				m_pSrcChr->GetRealRHandTM()._43   ); 

			break;
	}
	
	D3DXMATRIX matBone;
	m_pDestChr->GetBoneLinkTM( 2 , matBone );

	D3DXVECTOR3 vDestPos( matBone._41 ,
						  matBone._42 ,
						  matBone._43 );

	D3DXVECTOR3 vDistance = vDestPos - m_vStartPos;

	m_RangeTime	  = PositionToTimer( 20 , m_vStartPos , vDestPos );

	D3DXMatrixIdentity( &m_BallParticle.m_LinkMat );

	
	// 손끝에 이펙트
	g_Particle.SetEffSetCon( 0.0f , EF_STAND_BOOK_ATTACK , 0 , 2.0f , m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex() );
}


int	CEFBookAttack::CheckFrame(void)
{

	return TRUE;
}

int	CEFBookAttack::Render(float dtime)
{
	D3DXMATRIX matLocal;	

	D3DXMatrixIdentity( &matLocal );

	g_RenderManager.SetTransform( D3DTS_WORLD , &matLocal );
	
	g_RenderManager.SetCullMode( D3DCULL_NONE );

	g_RenderManager.SetZWriteEnable( FALSE );
	g_RenderManager.SetZEnable( TRUE );

	g_RenderManager.SetAlphaBlendEnable( TRUE );
	g_RenderManager.SetSrcBlend( D3DBLEND_ONE );
	g_RenderManager.SetDestBlend(  D3DBLEND_ONE );

	m_BallParticle.Render();

	g_RenderManager.SetCullMode( D3DCULL_CCW );

	g_RenderManager.SetZWriteEnable( TRUE );
	g_RenderManager.SetZEnable( TRUE );

	g_RenderManager.SetAlphaBlendEnable( FALSE );

	return TRUE;
}

int CEFBookAttack::Update(float dtime)
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
	
	float       Range     = ( ( ( float ) ( SAFE_TIME_SUB( g_nowTime , m_OldTimer ) ) / ( float ) m_RangeTime ) );

	if( Range > 1.0f ) 
		Range = 1.0f;

	D3DXVECTOR3 vPos      = m_vStartPos + ( vDistance * Range );  
	
	m_BallParticle.SetLinkPos( vPos );
	
	m_BallParticle.UpdateLink( dtime );

	g_Particle.DisplayUpdate( PT_BOOK_STAND_ATTACK_TAIL , vPos );

	//............................................................................................................
	// 파이어볼 캐릭터에 충돌 처리 ( 이펙트 .... T.T )
	//............................................................................................................
	if( SAFE_TIME_COMPARE( g_nowTime , > ,  SAFE_TIME_ADD( m_RangeTime , m_OldTimer ) ) )
	{
		SetDestroy();
	
		SetAfterEffect();
	}

	return TRUE;
}

int CEFBookAttack::SetAfterEffect(void)
{
	g_Particle.m_EffectManager.StandTargetHit( m_pSrcChr , m_pDestChr , m_pExtraInfo );

	return TRUE;
}
