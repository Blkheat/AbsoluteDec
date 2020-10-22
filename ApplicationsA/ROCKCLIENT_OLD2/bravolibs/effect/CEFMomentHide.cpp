#include "CEFMomentHide.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include	<SOUND\Sound.H>
#include "..\\..\\\rocklibs\RockInterface\PartyInforWnd.h"

extern long		 g_nowTime;

#define HIDE_ALPHA_VALUE	0.3f

CEFMomentHide::CEFMomentHide()
{
}

CEFMomentHide::~CEFMomentHide()
{
	
	BOOL IsOwnerPet = FALSE;

	if( m_pSrcChr->m_bPC && m_pSrcChr->m_State.nPetState == nPetState_Master &&
		m_pSrcChr->m_State.PetIndex != -1 )
	{
		IsOwnerPet = TRUE;
	}
	
	if(IsOwnerPet)
	{
		int nPetIndex = m_pSrcChr->m_State.PetIndex;
		g_Map.m_Npc[nPetIndex].m_CharAlpha.Clear();
	}
	
	// 노멀 상태로
	m_pSrcChr->m_CharAlpha.Clear();
}

void CEFMomentHide::Init(void)
{
	//......................................................................................................
	// 유저일때
	if( m_pSrcChr->m_bPC )
	{
		BOOL IsOwnerPet = FALSE;
		
		if( m_pSrcChr->m_State.nPetState == nPetState_Master &&
			m_pSrcChr->m_State.PetIndex != -1 )
		{
			IsOwnerPet = TRUE;
		}
		
		// 내 캐릭 인지 검사
		if( m_pSrcChr->GetUnique() == g_Pc.GetPlayer()->GetUnique() )
		{
			m_pSrcChr->m_CharAlpha.Set( CHAR_HIDE , HIDE_ALPHA_VALUE );	
		
			if(IsOwnerPet)
			{
				int nPetIndex = m_pSrcChr->m_State.PetIndex;
				g_Map.m_Npc[nPetIndex].m_CharAlpha.Set( CHAR_HIDE, HIDE_ALPHA_VALUE );
			}
		}
		// 내 파티원인지 검사
		else if( ::IsMyPartyAndLeagueMember( m_pSrcChr->GetUnique() ) )
		{
			m_pSrcChr->m_CharAlpha.Set( CHAR_HIDE , HIDE_ALPHA_VALUE );
	
			if(IsOwnerPet)
			{
				int nPetIndex = m_pSrcChr->m_State.PetIndex;
				g_Map.m_Npc[nPetIndex].m_CharAlpha.Set( CHAR_HIDE, HIDE_ALPHA_VALUE );
			}
		}
		else
		{
			m_pSrcChr->m_CharAlpha.Set( CHAR_NONE_DISPLAY , 0.0f );
		
			if(IsOwnerPet)
			{
				int nPetIndex = m_pSrcChr->m_State.PetIndex;
				g_Map.m_Npc[nPetIndex].m_CharAlpha.Set( CHAR_NONE_DISPLAY , 0.0f );
			}
		}
	}
	else
	{
		m_pSrcChr->m_CharAlpha.Set( CHAR_NONE_DISPLAY , 0.0f );
	}

	//................................................................................	
	// 발동 사운드
	//................................................................................	
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( m_pExtraInfo->SkillCode , m_pSrcChr );
	
	D3DXVECTOR3 vPos( m_pSrcChr->GetPosTM()._41 , m_pSrcChr->GetPosTM()._42 , m_pSrcChr->GetPosTM()._43 );
	
	g_Particle.SetEffSetCon( 0.0f , m_ID , 100 , 2.1f , -1 , -1 , vPos );
}

int CEFMomentHide::Update(float dtime)
{
	if( g_Particle.m_EffectManager.CheckDie( m_pSrcChr ) )
	{
		SetDestroy();

		return TRUE;
	}	

	if( (m_pSrcChr->GetUnique() == g_Pc.GetPlayer()->GetUnique())&& ( m_pSrcChr->m_CharAlpha.GetState() == CHAR_NORMAL ))
	{
		m_pSrcChr->m_CharAlpha.Set( CHAR_HIDE , HIDE_ALPHA_VALUE );
	}

	return TRUE;
}