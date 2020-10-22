#include "CEFHandPar.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include	<SOUND\Sound.H>

extern long g_nowTime;


CEFHandPar::CEFHandPar()
{
	m_IsConEffect = TRUE;
}

CEFHandPar::~CEFHandPar()
{
}

void CEFHandPar::Init(void)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr ) )
	{
		SetDestroy();

		return;
	}

	int m_TileCount = 10;

	Character			*pTempDest = NULL;
	EFFECT_EXTRA_INFO    Info;

	// 1:N 데미지
	if( m_pExtraInfo != NULL )
	{
		SSkillBaseInfo* skill_Info;
	
		skill_Info = g_Pc_Manager.GetSkillBaseInfoPtr( m_pExtraInfo->SkillCode ); 

		if( skill_Info == NULL )
			return;

		m_TileCount = ( int ) ( g_Particle.m_EffectManager.GetAttackRange( m_pExtraInfo->SkillCode ) / 8.0f );

		int Count = m_pExtraInfo->GetTargetCount();		
		
		for( int i = 0 ; i < Count ; ++i )
		{
			if( m_pExtraInfo->GetTargetInfo( &pTempDest , &Info , i ) )
			{
				g_Particle.SetEffSetCon( 0.0f , EF_HAND_PAR_ED , 0 , 2.6f , pTempDest->GetUnique() , 
					pTempDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr) );
				
				Info.AfterEffect = EF_HAND_PAR_ED;
				Info.DestType = 0;
				Info.lifeTime = 2.6f;
				
				g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , pTempDest , &Info );
			}

			Info.Clear();
		}
	}


	m_OldTimer    = g_nowTime;
	
	m_vStartPos   = D3DXVECTOR3( m_pSrcChr->GetPosCenterBoneTM()._41 , 
								 0.0f ,
								 m_pSrcChr->GetPosCenterBoneTM()._43  );
	
	
	m_vEndPos = m_vStartPos;

	m_fDistace = 8.0f * m_TileCount;
	
	m_RangeTime = 10 * m_TileCount;

	m_vDir = m_pSrcChr->m_vDir;

	m_Range.Insert( 0 , 0.0f );
	m_Range.Insert( m_RangeTime , m_fDistace );

	m_ISCreate = TRUE;
	//................................................................................	
	// 발동 사운드
	//................................................................................	
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( m_pExtraInfo->SkillCode , m_pSrcChr );

}


int CEFHandPar::Render(float dtime)
{
	return TRUE;
}

int CEFHandPar::Update(float dtime)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr ) )
	{
		SetDestroy();

		return TRUE;
	}

	if( m_ISCreate )
	{
		float Size = 0.0f;
		
		if( m_Range.GetValue( SAFE_TIME_SUB( g_nowTime , m_OldTimer ) , Size ) == FALSE )
		{
			m_ISCreate = FALSE;
		
			SetDestroy();
		}
		
		m_vPos = m_vStartPos + m_vDir * Size;
		
		m_vPos.y = g_Map.PHF_GetHeight( m_vPos.x , m_vPos.z );
		
		g_Particle.SetEffSetCon( 0.0f , EF_HAND_PAR , 100 , 1.5f , -1 , -1 , m_vPos , m_pSrcChr->GetDirection() );
	}
	
	return TRUE;
}