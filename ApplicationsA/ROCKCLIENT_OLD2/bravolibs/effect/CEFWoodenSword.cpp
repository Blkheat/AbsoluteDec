#include "CEFWoodenSword.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include <SOUND\Sound.H>
#include <NETWORK\\Protocol.h>
#include <Base\\D3DBase.h>
#include "..\\RockClient.h"
#include "..\\quadlist.h"
#include <Map\\Field.h>
#include "..\\Map.H"
#include "..\\PC.H"
#include "..\\Effect.H"
#include "..\\Camera.H"
#include "..\\RockClient.h"

extern CBravoMap g_Map;
extern long		 g_nowTime;


CEFWoodenSword::CEFWoodenSword()
{
	m_IsConEffect = TRUE;
}

CEFWoodenSword::~CEFWoodenSword()
{
}

void CEFWoodenSword::Init(void)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr , m_pDestChr ) )
	{
		SetDestroy();

		return;
	}

	int ID;
	
	switch( m_ID )
	{
		case EF_WOODENSWORD0:
		case EF_WOODENSWORD1:
			
			ID = EF_WOODENSWORD_ATTACKED0;

			break;

		case EF_WOODENSWORD2:
		case EF_WOODENSWORD3:

			ID = EF_WOODENSWORD_ATTACKED2;

			break;

		case EF_WOODENSWORD4:

			ID = EF_WOODENSWORD_ATTACKED4;

			break;
	}

	//................................................................................	
	// 발동 사운드
	//................................................................................	
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( m_pExtraInfo->SkillCode , m_pSrcChr );
	
	
	
	D3DXVECTOR3 vPos = D3DXVECTOR3( m_pDestChr->GetBonCenterTM()._41 , 
									m_pDestChr->GetBonCenterTM()._42 , 
									m_pDestChr->GetBonCenterTM()._43 );
	
	g_Particle.SetEffSetCon( 0.0f , ID , 100 , 2.6f , -1 , -1 , vPos );

	if( m_pSrcChr == g_Pc.GetPlayer() )
		g_Particle.RockCamera( ( rand() % 200 ) + 300 );

	SetAfterEffect();
}

//................................................................................................................
// 이펙트 처리후 뒤로올 이펙트 셋업
//................................................................................................................
int CEFWoodenSword::SetAfterEffect(void)
{
	//................................................................................	
	// 어택 사운드
	//................................................................................	
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillAttackedSound( m_pExtraInfo->SkillCode , m_pDestChr );

	g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , m_pDestChr , m_pExtraInfo );

	return TRUE;
}
