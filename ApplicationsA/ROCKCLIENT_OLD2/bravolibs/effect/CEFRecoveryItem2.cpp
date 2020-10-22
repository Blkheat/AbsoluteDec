#include "CEFRecoveryItem2.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include	<SOUND\Sound.H>

extern long		 g_nowTime;


CEFRecoveryItem2::CEFRecoveryItem2()
{
}

CEFRecoveryItem2::~CEFRecoveryItem2()
{
	g_Particle.DelEffSetCon( m_ID , 0 , m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex() );
}

void CEFRecoveryItem2::Init(void)
{
	g_Particle.SetEffSetCon( 0.0f , m_ID , 0 , EFFECT_SET_LOOP_TIME , m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex() );

	m_RangeTimer = g_Pc_Manager.theItemInfoTable[ g_Pc_Manager.theItemCodeTable[ m_pExtraInfo->SkillCode ] ].theReHPSpeed;

	if( m_RangeTimer < g_Pc_Manager.theItemInfoTable[ g_Pc_Manager.theItemCodeTable[ m_pExtraInfo->SkillCode ] ].theReSPSpeed )	
	{
		m_RangeTimer = g_Pc_Manager.theItemInfoTable[ g_Pc_Manager.theItemCodeTable[ m_pExtraInfo->SkillCode ] ].theReSPSpeed;
	}
	
	if( m_RangeTimer < g_Pc_Manager.theItemInfoTable[ g_Pc_Manager.theItemCodeTable[ m_pExtraInfo->SkillCode ] ].theReMPSpeed )	
	{
		m_RangeTimer = g_Pc_Manager.theItemInfoTable[ g_Pc_Manager.theItemCodeTable[ m_pExtraInfo->SkillCode ] ].theReMPSpeed;
	}
	
	m_RangeTimer = m_RangeTimer * 1000;

	m_OldTimer = g_nowTime;
}

int CEFRecoveryItem2::Update(float dtime)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr ) )
	{
		SetDestroy();

		return TRUE;
	}

	if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( g_nowTime , m_OldTimer ) , >= , m_RangeTimer ) )
	{
		SetDestroy();
	}

	return TRUE;
}
