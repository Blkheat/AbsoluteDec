#include "CEFBookCriticalAttack.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include	<SOUND\Sound.H>
#include "..\\..\\CRenderManager.h"

extern long g_nowTime;


CEFBookCriticalAttack::CEFBookCriticalAttack()
{
	m_IsConEffect = TRUE;
}


CEFBookCriticalAttack::~CEFBookCriticalAttack()
{

}

void CEFBookCriticalAttack::Init(void)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr , m_pDestChr ) )
	{
		SetDestroy(TRUE, TRUE);
		return;
	}
	
	m_Src_Ani_Index = m_pSrcChr->m_ani_index;

	m_ISDisplay[0] = TRUE;
	m_ISDisplay[1] = FALSE;

	CParticleData	*pData = NULL;

	if( ( pData = g_Particle.m_ParticleData.GetKeyDataPointer( PT_BOOK_STAND_CRITICAL_ATTACK ) ) == NULL )
		return;

	m_BallParticle[0].MakeLink( pData );
	m_BallParticle[1].MakeLink( pData );

	if( ( pData = g_Particle.m_ParticleData.GetKeyDataPointer( PT_BOOK_STAND_ATTACK0 ) ) == NULL )
		return;
	
	m_CenterBallParticle[0].MakeLink( pData );
	m_CenterBallParticle[1].MakeLink( pData );
	
	m_vStartPos[0]   = D3DXVECTOR3( m_pSrcChr->GetRealRHandTM()._41 , 
									m_pSrcChr->GetRealRHandTM()._42 , 	
									m_pSrcChr->GetRealRHandTM()._43   ); 

	//.......................................................................................................

	D3DXMATRIX matBone;
	m_pDestChr->GetBoneLinkTM( 2 , matBone );

	D3DXVECTOR3 vDestPos( matBone._41 ,
						  matBone._42 ,
						  matBone._43 );

	D3DXVECTOR3 vDistance = vDestPos - m_vStartPos[ 0 ];

	m_RangeTime[0]   = PositionToTimer( 20 , m_vStartPos[0] , vDestPos );

	D3DXMatrixIdentity( &m_BallParticle[0].m_LinkMat );
	D3DXMatrixIdentity( &m_CenterBallParticle[0].m_LinkMat );

	m_OldTimer[0]    = g_nowTime;
	//.......................................................................................................

	
	// 손끝에 이펙트
	//g_Particle.SetEffSetCon( 0.0f , EF_STAND_BOOK_ATTACK , 0 , 2.0f , m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex() );
	g_Billboard.SetManager( 0.0f , 
					BB_BOOK_CRITICAL_RIGHT_LINK ,
					m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex() ,
					D3DXVECTOR3( 0.0f , 0.0f , 0.0f )  ,
					5 );

}


int	CEFBookCriticalAttack::CheckFrame(void)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr , m_pDestChr ) )
	{
		SetDestroy(TRUE, TRUE);
		return TRUE;
	}

	if( m_Src_Ani_Index != m_pSrcChr->m_ani_index )
	{
		SetAfterEffect();
		SetDestroy();
	}	

	// 왼손 에서 나가는 것 초기화
//	if( m_pSrcChr->m_ani_frame == 31 && m_pSrcChr->m_ani_index == n_Attack5_Basic4 )
	if( GetSrcAniCount() == 31 && m_pSrcChr->m_ani_index == n_Attack5_Basic4 )
	{
		m_pSrcChr->m_IsApplyVA = TRUE;
		m_pSrcChr->m_IsNextApplyVA = TRUE;
		m_pSrcChr->BoneRender();		
		
		m_vStartPos[1]   = D3DXVECTOR3( m_pSrcChr->GetRealLHandTM()._41 , 
										m_pSrcChr->GetRealLHandTM()._42 , 	
										m_pSrcChr->GetRealLHandTM()._43   ); 
	

		D3DXMATRIX matBone;
		m_pDestChr->GetBoneLinkTM( 2 , matBone );

		D3DXVECTOR3 vDestPos( matBone._41 ,
							  matBone._42 ,
						      matBone._43 );
		
		D3DXVECTOR3 vDistance = vDestPos - m_vStartPos[ 1 ];
		
		m_RangeTime[1]   = PositionToTimer( 20 , m_vStartPos[ 1 ] , vDestPos );
		
		D3DXMatrixIdentity( &m_BallParticle[1].m_LinkMat );
		D3DXMatrixIdentity( &m_CenterBallParticle[1].m_LinkMat );
		
		m_OldTimer[1]    = g_nowTime;
		m_ISDisplay[1]   = TRUE;

		g_Billboard.SetManager( 0.0f , 
						BB_BOOK_CRITICAL_LEFT_LINK ,
						m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex() ,
						D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
						4 );
	}

	return TRUE;
}

int	CEFBookCriticalAttack::Render(float dtime)
{
	D3DXMATRIX matLocal;	

	D3DXMatrixIdentity( &matLocal );

	g_RenderManager.SetTransform( D3DTS_WORLD , &matLocal );
	
	g_RenderManager.SetCullMode( D3DCULL_NONE );

	g_RenderManager.SetZWriteEnable( FALSE );
	g_RenderManager.SetZEnable( TRUE );

	g_RenderManager.SetAlphaBlendEnable( TRUE );
	g_RenderManager.SetSrcBlend( D3DBLEND_ONE );
	g_RenderManager.SetDestBlend( D3DBLEND_ONE );

	
	for( int i = 0 ; i < 2 ; ++i )
	{
		if( m_ISDisplay[ i ] )
		{
			m_BallParticle[i].Render();
			m_CenterBallParticle[i].Render();
		}
	}
	
	g_RenderManager.SetCullMode( D3DCULL_CCW );

	g_RenderManager.SetZWriteEnable( TRUE );
	g_RenderManager.SetZEnable( TRUE );

	g_RenderManager.SetAlphaBlendEnable( FALSE );

	return TRUE;
}

int CEFBookCriticalAttack::Update(float dtime)
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
	
	for( int i = 0 ; i < 2 ; ++i )
	{
		if( !m_ISDisplay[ i ] )
			continue;
		
		D3DXVECTOR3 vDistance = vDestPos - m_vStartPos[ i ];
		
		float       Range     = ( ( ( float ) SAFE_TIME_SUB( g_nowTime , m_OldTimer[ i ] ) / ( float ) m_RangeTime[ i ]  ) );
		
		if( Range > 1.0f ) 
			Range = 1.0f;
		
		D3DXVECTOR3 vPos      = m_vStartPos[ i ] + ( vDistance * Range );  
		
		m_BallParticle[ i ].SetLinkPos( vPos );
		m_CenterBallParticle[ i ].SetLinkPos( vPos );

		m_BallParticle[ i ].UpdateLink( dtime );
		m_CenterBallParticle[ i ].UpdateLink( dtime );
		
		g_Particle.DisplayUpdate( PT_BOOK_STAND_ATTACK_TAIL , vPos );
	}


	//............................................................................................................
	// 파이어볼 캐릭터에 충돌 처리 ( 이펙트 .... T.T )
	//............................................................................................................
	if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( g_nowTime , m_OldTimer[ 1 ] ) , > , m_RangeTime[ 1 ] ) && m_ISDisplay[1] )
	{
		g_Particle.m_EffectManager.StandTargetHitOnlyDamage( m_pSrcChr , m_pDestChr , m_pExtraInfo );
			
		SetDestroy();
	}

	if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( g_nowTime , m_OldTimer[ 0 ] ) , > , m_RangeTime[ 0 ] ) && m_ISDisplay[ 0 ] )
	{
		SetAfterEffect();

		m_ISDisplay[ 0 ] = FALSE;
		
		m_pDestChr->theVibrationTime = g_nowTime;
		m_pDestChr->theIsVibration   = TRUE;
	}

	return TRUE;
}

int CEFBookCriticalAttack::SetAfterEffect(void)
{
	g_Particle.m_EffectManager.StandTargetHitNoDamage( m_pSrcChr , m_pDestChr , m_pExtraInfo );

	return TRUE;
}
