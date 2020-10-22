#include "CEFSonBrush.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include	<SOUND\Sound.H>
#include "CEFArrow.h"


extern long		 g_nowTime;

CEFSonBrush::CEFSonBrush()
{
	m_IsConEffect = TRUE;
}

CEFSonBrush::~CEFSonBrush()
{
}

void CEFSonBrush::Init(void)
{
	//................................................................................	
	// 발동 사운드
	//................................................................................	
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( m_pExtraInfo->SkillCode , m_pSrcChr );
	
	m_OldTime = g_nowTime;

	D3DXVECTOR3 vRange( 0.0f , 0.0f , 0.0f );
	D3DXMATRIX  matRotY;

	int			Count = 0;
	float	    fRadius = g_Particle.m_EffectManager.GetAttackRange( m_pExtraInfo->SkillCode );

	Count = Random( 5 , 10 );

	for( int i = 0 ; i < Count ; ++i )
	{
		vRange.x = Random( 0.0f , fRadius );
		vRange.z = vRange.y = 0.0f;

		D3DXMatrixRotationY( &matRotY , D3DXToRadian( Random( 0 , 360 ) ) );

		D3DXVec3TransformCoord( &vRange , &vRange , &matRotY );

		vRange.x += GetCellCenterX();
		vRange.z += GetCellCenterZ();
		vRange.y  = g_Map.Get__MapHeightF( vRange.x , vRange.z );

		g_Particle.SetEffSetCon( 0.0f , EF_SON_BURSH , 100 , 2.1f , -1 , -1 , vRange );
	}
	
	Character			*pTempDest = NULL;
	EFFECT_EXTRA_INFO    Info;	
	
	if( m_pExtraInfo != NULL )
	{
		// 1:N 데미지
		int Count = m_pExtraInfo->GetTargetCount();		
		
		for( int i = 0 ; i < Count ; ++i )
		{
			if( m_pExtraInfo->GetTargetInfo( &pTempDest , &Info , i ) )
			{
				g_Particle.SetEffSetCon( 0.0f , EF_SON_BURSHED , 0 , 4.1f , 
					pTempDest->GetUnique() , pTempDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr) );
				
				Info.AfterEffect = EF_SON_BURSHED;
				Info.DestType = 0;
				Info.lifeTime = 4.1f;
				g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , pTempDest , &Info );
			}

			Info.Clear();
		}
	}

	SetDestroy();
}

int CEFSonBrush::Update(float dtime)
{
	if( g_Particle.m_EffectManager.CheckDie( m_pSrcChr ) )
	{
		SetDestroy();

		return TRUE;
	}
	
	return TRUE;
}

int	CEFSonBrush::Render(float dtime)
{
	return TRUE;
}
