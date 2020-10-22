#include "CEFHide.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include	<SOUND\Sound.H>
#include "..\\..\\\rocklibs\RockInterface\PartyInforWnd.h"

extern long		 g_nowTime;

#define HIDE_ALPHA_VALUE	0.4f
#define HIDE_FACE_MAX_TIME	2000

CEFHide::CEFHide()
{
}

CEFHide::~CEFHide()
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

void CEFHide::Init(void)
{
	m_ISFade = FALSE;
	
	m_AlphaSequence.Insert( 0    , 1.0f				);
	m_AlphaSequence.Insert( HIDE_FACE_MAX_TIME , 0.3f );
	m_AlphaSequence.SetMaxTime( HIDE_FACE_MAX_TIME );

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
			m_ISFade = TRUE;
			m_pSrcChr->m_CharAlpha.Set( CHAR_HIDE , 1.0f );
			

			
			if(IsOwnerPet)
			{
				int nPetIndex = m_pSrcChr->m_State.PetIndex;
				g_Map.m_Npc[nPetIndex].m_CharAlpha.Set( CHAR_HIDE, HIDE_ALPHA_VALUE );
			}


		}
		// 내 파티원인지 검사
		else if( ::IsMyPartyAndLeagueMember( m_pSrcChr->GetUnique() ) )
		{
			m_ISFade = TRUE;
			m_pSrcChr->m_CharAlpha.Set( CHAR_HIDE , 1.0f );

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

	m_OldTimer = g_nowTime;
}

int CEFHide::Update(float dtime)
{
	if( g_Particle.m_EffectManager.CheckDie( m_pSrcChr ) )
	{
		SetDestroy();

		return TRUE;
	}
	
	if( m_ISFade )
	{
		float Value = 0.0f;
		
		if( TRUE == m_AlphaSequence.GetValue( SAFE_TIME_SUB( g_nowTime , m_OldTimer ) , Value ) )
		{
			m_pSrcChr->m_CharAlpha.Set( CHAR_HIDE , Value );
		}
		else
		{
			m_ISFade = FALSE;
			m_pSrcChr->m_CharAlpha.Set( CHAR_HIDE , HIDE_ALPHA_VALUE );
		}
	}
	else
	{
		// 내 캐릭 인지 검사
		if( (m_pSrcChr->GetUnique() == g_Pc.GetPlayer()->GetUnique())&& 
			( m_pSrcChr->m_CharAlpha.GetState() == CHAR_NORMAL ))
		{
			m_ISFade = TRUE;			
		}
	}

	return TRUE;
}