#include "CEFNpcStoneAttack.h"
#include "CEFArrow.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include	<SOUND\Sound.H>

#include "..\\..\\CRenderManager.h"

extern long g_nowTime;

CEffectModel	 CEFNpcStoneAttack::m_StoneModel[2];

int	CEFNpcStoneAttack::InitDevice(void)
{
	CStringToCode	HashString( "stone.tga" );
	static unsigned long HashCode = HashString.GetHashCode();
	
	CEFNpcStoneAttack::m_StoneModel[0].InitDevice( "stone.msh" , HashCode );
	CEFNpcStoneAttack::m_StoneModel[1].InitDevice( "stone1.msh" , HashCode );

	return TRUE;
}

int	CEFNpcStoneAttack::DeleteDevice(void)
{
	CEFNpcStoneAttack::m_StoneModel[0].DeleteDevice();
	CEFNpcStoneAttack::m_StoneModel[1].DeleteDevice();

	return TRUE;
}

CEFNpcStoneAttack::CEFNpcStoneAttack()
{
	m_IsConEffect = TRUE;
}


CEFNpcStoneAttack::~CEFNpcStoneAttack()
{
}

void CEFNpcStoneAttack::Init(void)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr , m_pDestChr ) )
	{
		SetDestroy(TRUE);
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

	D3DXVECTOR3 vDistance = vDestPos - m_vStartPos;

	D3DXMatrixRotationY( &m_matLocal , m_pSrcChr->m_radian );

	m_matLocal._41 = m_vStartPos.x;
	m_matLocal._42 = m_vStartPos.y;
	m_matLocal._43 = m_vStartPos.z;

	m_RangeTime	  = PositionToTimer( 70 , m_vStartPos , vDestPos );

	m_OldTimer    = g_nowTime;

	//................................................................................	
	// 발동 사운드
	//................................................................................	
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( m_pExtraInfo->SkillCode , m_pSrcChr );

}

int	CEFNpcStoneAttack::Render(float dtime)
{
	g_RenderManager.SetTransform( D3DTS_WORLD , &m_matLocal );
	
	g_RenderManager.SetCullMode( D3DCULL_CCW );
	
	g_RenderManager.SetZWriteEnable( TRUE );
	g_RenderManager.SetZEnable( TRUE );
	
	g_RenderManager.SetAlphaBlendEnable( FALSE );

	switch( m_ID )
	{
		case EF_ROCK_SHOT:
			CEFNpcStoneAttack::m_StoneModel[0].Render();
			break;
		case EF_ROCK_SHOT1:
			CEFNpcStoneAttack::m_StoneModel[1].Render();
			break;
	}
	

	return TRUE;
}

int CEFNpcStoneAttack::Update(float dtime)
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

	//............................................................................................................
	// 캐릭터에 충돌 처리 ( 이펙트 .... T.T )
	//............................................................................................................
	if( Range >= 1.0f ) 
	{
		Range = 1.0f;

		SetDestroy();
	
		SetAfterEffect();
		
		return TRUE;
	}

	D3DXVECTOR3 vPos      = m_vStartPos + ( vDistance * Range );  
	float		RangeRadius = D3DXToRadian( 180 ) * Range;

	vPos.y = vPos.y + sinf( RangeRadius ) * 30.0f;

	m_matLocal._41 = vPos.x;
	m_matLocal._42 = vPos.y;
	m_matLocal._43 = vPos.z;

	return TRUE;
}

int CEFNpcStoneAttack::SetAfterEffect(void)
{
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillAttackedSound( m_pExtraInfo->SkillCode , m_pDestChr );

	UINT uAttackedID = EF_ROCK_SHOTED;
	
	switch( m_ID )
	{
		case EF_ROCK_SHOT:
			g_Particle.SetEffSetCon( 0.0f , EF_ROCK_SHOTED , 0 , 4.1f , 
				m_pDestChr->GetUnique() , m_pDestChr->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr) );
			uAttackedID = EF_ROCK_SHOTED;
			break;
		case EF_ROCK_SHOT1:
			g_Particle.SetEffSetCon( 0.0f , EF_ROCK_SHOTED1 , 0 , 4.1f , 
				m_pDestChr->GetUnique() , m_pDestChr->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr) );
			uAttackedID = EF_ROCK_SHOTED1;
			break;
	}

	if(m_pExtraInfo)
	{
		m_pExtraInfo->AfterEffect = uAttackedID;
		m_pExtraInfo->DestType = 0;
		m_pExtraInfo->lifeTime = 4.1f;
	}
	
	g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , m_pDestChr , m_pExtraInfo );

	return TRUE;
}
