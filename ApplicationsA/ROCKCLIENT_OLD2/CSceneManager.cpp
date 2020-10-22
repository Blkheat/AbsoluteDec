#include "RockClient.h"
#include "stringmanager.h"
#include "brother.h"
#include "RockClient.h"


#include "quadlist.h"
#include "bravolibs\\network\\Protocol.h"
#include "Pc.h"

#include "Map.h"
#include "effect.h"
#include "CSceneManager.h"

CSceneManager::CSceneManager()
{
	Init();
}

CSceneManager::~CSceneManager()
{
}

// 디폴트값 설정
void CSceneManager::Init(void)
{
	m_ISKeyInput		    = TRUE;			
	m_ISGUIDisplay		    = TRUE;			
	m_ISOBJDisplay			= TRUE;			
	m_ISNPCDisplay			= TRUE;					
	m_ISPlayerDisplay		= TRUE;		
	m_ISOtherPlayerDisplay	= TRUE;	
	m_ISTerrainDisplay		= TRUE;
	m_ISCursorDisplay		= TRUE;
	m_ISSkyBoxDisplay		= TRUE;
	m_ISFogDisplay			= TRUE;
	m_ISWeatherDisplay		= TRUE;
	m_ISInsectDisplay		= TRUE;

	m_BackGroundColor		= D3DCOLOR_XRGB( 0 , 0 , 0 );

	m_SceneMode				= SCENE_NORMAL;
	m_EffectCreateMode      = SCENE_NORMAL;
	m_EffectDisplayMode		= SCENE_NORMAL;

	m_SoundPlayMode			= SCENE_NORMAL;	

	m_ISPlayerWeaponDisplay      = TRUE;
	m_ISOtherPlayerWeaponDisplay = TRUE;
	
	m_SquencePlayerAniTimeRange      = 1.0f;			// 플레이어 에니메이션 속도
	m_SquenceOtherPlayerAniTimeRange = 1.0f;			// 다른 플레이어 에니메이션 속도
	m_FaceExp = CFaceExpData::FACE_EXP_MAXCOUNT;
}

void CSceneManager::SetCursorDisplay(BOOL Value)
{ 
	m_ISCursorDisplay      = Value; 

	g_RenderManager.ShowCursor( m_ISCursorDisplay );
	g_RockClient.m_IsDeviceCursor = m_ISCursorDisplay;
}

