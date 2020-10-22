#include "CEFBowAttack.h"
#include "CEFArrow.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include	<SOUND\Sound.H>
#include "..\\..\\CRenderManager.h"

extern long g_nowTime;

CEFBowAttack::CEFBowAttack()
{
	m_IsConEffect = TRUE;
	m_fXRadian = 0.0f;
	m_fYRadian = 0.0f;
}


CEFBowAttack::~CEFBowAttack()
{

}

void CEFBowAttack::Init(void)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr , m_pDestChr ) )
	{
		SetDestroy(TRUE, TRUE);
		return;
	}

	m_vStartPos   = D3DXVECTOR3( m_pSrcChr->GetRealLHandTM()._41 , 
								 m_pSrcChr->GetRealLHandTM()._42 , 	
								 m_pSrcChr->GetRealLHandTM()._43   ); 
			
	D3DXMATRIX matBone;
	m_pDestChr->GetBoneLinkTM( 2 , matBone );

	D3DXVECTOR3 vDestPos( matBone._41 ,
						  matBone._42 ,
						  matBone._43 );

	m_fXRadian = 0.0f;
	m_fYRadian = m_pSrcChr->m_radian;

	GetXAndYRadian(m_fXRadian, m_fYRadian, m_vStartPos, vDestPos);

	D3DXMATRIX matRotX, matRotY;

	D3DXMatrixRotationX( &matRotX , m_fXRadian );
	D3DXMatrixRotationY( &matRotY , m_fYRadian );
	D3DXMatrixMultiply( &m_matLocal , &matRotX , &matRotY );	
	
	m_matLocal._41 = m_vStartPos.x;
	m_matLocal._42 = m_vStartPos.y;
	m_matLocal._43 = m_vStartPos.z;

	m_RangeTime	  = PositionToTimer( 50 , m_vStartPos , vDestPos );

	m_OldTimer    = g_nowTime;

	g_Particle.SetEffSetCon( 0.0f , EF_BOW_SPOT , 0 , 2.0f , m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex() );
}

int	CEFBowAttack::Render(float dtime)
{
	g_RenderManager.SetTransform( D3DTS_WORLD , &m_matLocal );
	
	g_RenderManager.SetAlphaBlendEnable( FALSE );
	g_RenderManager.SetZWriteEnable( TRUE );
	g_RenderManager.SetZEnable( TRUE );
	g_RenderManager.SetCullMode( D3DCULL_CCW );

	CEFArrow::Render( 0 );

	return TRUE;
}

int CEFBowAttack::Update(float dtime)
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

	//............................................................................................................
	// 캐릭터에 충돌 처리 ( 이펙트 .... T.T )
	//............................................................................................................
	if( Range >= 1.0f ) 
	{
		Range = 1.0f;

		///-- 화살 붙이기...
		g_Particle.m_EffectManager.CreateArrowStuck(
			m_pDestChr,
			m_pSrcChr->m_radian,
			D3DXVECTOR3( RANDOM_NUM , RANDOM_NUM , RANDOM_NUM ) ,
			2 );

		SetDestroy();	
		SetAfterEffect();
		
		return TRUE;
	}

	D3DXVECTOR3 vPos      = m_vStartPos + ( vDistance * Range );	
	
	GetXAndYRadian(m_fXRadian, m_fYRadian, vPos, vDestPos);

	D3DXMATRIX matRotX, matRotY;

	D3DXMatrixRotationX( &matRotX , m_fXRadian );
	D3DXMatrixRotationY( &matRotY , m_fYRadian );
	D3DXMatrixMultiply( &m_matLocal , &matRotX , &matRotY );	
	
	m_matLocal._41 = vPos.x;
	m_matLocal._42 = vPos.y;
	m_matLocal._43 = vPos.z;

	return TRUE;
}

int CEFBowAttack::SetAfterEffect(void)
{
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillAttackedSound( m_pExtraInfo->SkillCode , m_pDestChr );

	g_Particle.m_EffectManager.StandTargetHit( m_pSrcChr , m_pDestChr , m_pExtraInfo );

	return TRUE;
}
