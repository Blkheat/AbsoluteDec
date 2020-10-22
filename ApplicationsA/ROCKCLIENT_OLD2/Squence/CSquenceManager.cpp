#include "..\\RockClient.h"

#include "..\\quadlist.h"
#include "..\\bravolibs\\network\\Protocol.h"
#include "..\\Pc.h"

#include "..\\Effect.h"
#include "CSquenceManager.h"
#include "..\\bravolibs\sound\sound.h"

CSquenceManager::CSquenceManager()
{
	m_pSquenceBase = NULL;
	DeleteAllMemory();
}

CSquenceManager::~CSquenceManager()
{
	DeleteAllMemory();
}

void CSquenceManager::DeleteAllMemory(void)
{
	m_Count		   = 0;
	m_ISProcess	   = FALSE;
	m_WaitTye	   = SWT_NONE;
	m_OldTimer	   = 0;
	m_Timer		   = 0;

	m_pFuncCheck.Clear();
	m_pFuncFirstScene.Clear();
	m_pFuncAniFrameScene.Clear();
	m_PlayerNextAniIndex = -1;

	SAFE_DELETE(m_pSquenceBase);
}

void CSquenceManager::FirstSceneUpdate(void)
{
	if( !m_ISProcess )
		return;

	if( m_WaitTye == SWT_TIMER )
	{
		if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( timeGetTime() , m_OldTimer ) , >= , m_Timer ) )
		{
			if( !NextSquence() )
				return;
		}
	}
	else if( m_WaitTye == SWT_NONE )
	{
		if( !NextSquence() )
			return;
	}

	if( m_pFuncFirstScene.GetFunc() != NULL )
		(m_pFuncFirstScene)();
}

void CSquenceManager::Update(void)
{
	if( !m_ISProcess )
		return;
	
	if( m_pFuncCheck.GetFunc() != NULL )
		(m_pFuncCheck)();
}

void CSquenceManager::AniFrameUpdate(void)
{
	if( !m_ISProcess )
		return;
	
	if( m_pFuncAniFrameScene.GetFunc() != NULL )
		(m_pFuncAniFrameScene)();
}


void CSquenceManager::Start(CSquenceBase *pSquenceBase)
{
	if( pSquenceBase == NULL )
		return;

	pSquenceBase->m_pSquenceManager = this;
	
	m_pSquenceBase = pSquenceBase;
	m_Count		   = 0;

	NextSquence();
}

int CSquenceManager::NextSquence(void)
{
//	DebugPrint( "----Next Squence : %d -> %d----\n" , m_Count , m_Count + 1 );
	
	if( m_Count >= SQUENCE_MAX_EVENT || m_pSquenceBase == NULL )
	{
		End();

		return FALSE;
	}

	m_pFuncCheck.Clear();
	m_pFuncFirstScene.Clear();
	m_pFuncAniFrameScene.Clear();

	m_WaitTye = SWT_NONE;

	if( !((m_pSquenceBase->m_pFuncList[ m_Count ])() ) )
	{
		End();

		return FALSE;
	}

	m_pFuncCheck         = m_pSquenceBase->m_pFuncCheckList[ m_Count ];
	m_pFuncFirstScene    = m_pSquenceBase->m_pFuncFirstSceneList[ m_Count ];
	m_pFuncAniFrameScene = m_pSquenceBase->m_pFuncAniFrameSceneList[ m_Count ];

	m_ISProcess = TRUE;

	m_Count++;

	m_OldTimer = timeGetTime();

	return TRUE;
}

void CSquenceManager::End(void)
{
	m_ISProcess  = FALSE;
	m_Count      = 0;
	m_pFuncCheck.Clear();
	m_pFuncFirstScene.Clear();
	m_pFuncAniFrameScene.Clear();

	m_WaitTye    = SWT_NONE;

	SAFE_DELETE(m_pSquenceBase);
}

void CSquenceManager::ResetWaitFlag(SQUENCE_WAIT_TYPE Type)
{
	if( m_WaitTye == Type )
		NextSquence();
}

void CSquenceManager::SetPlayerAni(int AniIndex)
{
	EVENT_DATA_INFO temp;
	
	temp.Clear();
	temp.type   = SM_ANI_SQUENCE;
	temp.motion = AniIndex;
	
	g_Pc.GetPlayer()->PushQueue( temp, TRUE );
}

void CSquenceManager::SetPlayerNextAni(int AniIndex)
{
	m_PlayerNextAniIndex = AniIndex;
}

int CSquenceManager::GetPlayerNextAni(void)
{
	int TempIndex = m_PlayerNextAniIndex;
	
	m_PlayerNextAniIndex = -1;

	return TempIndex;
}

// 이펙트 생성 모드 변경
void CSquenceManager::EffectSquenceLock(void)
{
	m_SaveLockEffectMode = g_RockClient.m_SceneManager.GetEffectCreateMode();
	
	g_RockClient.m_SceneManager.SetEffectCreateMode( SCENE_SQUENCE );
}

// 이펙트 생성 모드 기존 모드로
void CSquenceManager::EffectSquenceUnLock(void)
{
	g_RockClient.m_SceneManager.SetEffectCreateMode( m_SaveLockEffectMode );
}

// 시퀀스 모드시 사운드 
void CSquenceManager::PlaySquenceSound(int Index,float fx,float fy,float fz)
{
	g_Sound.Play( Index , fx , fy , fz , SCENE_SQUENCE );
}

// 시퀀스 모드시 사운드 
void CSquenceManager::PlaySquenceSound(int Index,D3DXVECTOR3 *pvPos)
{
	g_Sound.Play( Index , pvPos , SCENE_SQUENCE );
}

void CSquenceManager::StopAllSound(void)
{
	g_Sound.AllStop();
}

void CSquenceManager::UpdateSound(int Index)
{
	g_Sound.UpdateSound( Index );
}


void CSquenceManager::SetPlayerAniTime(float Range)
{
	g_RockClient.m_SceneManager.SetSquencePlayerAniTime( Range );
}

void CSquenceManager::SetOtherPlayerAniTime(float Range)
{
	g_RockClient.m_SceneManager.SetSquenceOtherPlayerAniTime( Range );
}

