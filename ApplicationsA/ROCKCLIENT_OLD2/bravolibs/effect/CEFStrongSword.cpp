#include "CEFStrongSword.h"
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

extern long		 g_nowTime;

CEFStrongSword::CEFStrongSword()
{
}

CEFStrongSword::~CEFStrongSword()
{
}

void CEFStrongSword::Init(void)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr ) )
	{
		SetDestroy();

		return;
	}

	g_Particle.SetEffSetCon( 0.0f , EF_STRONG_SWORD0 , 0 , 2.5f , m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex() );

	//................................................................................	
	// 발동 사운드
	//................................................................................	
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( m_pExtraInfo->SkillCode , m_pSrcChr );
}

