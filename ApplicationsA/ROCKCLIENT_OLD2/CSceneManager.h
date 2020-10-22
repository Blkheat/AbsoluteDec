#ifndef __CSCENEMANAGER_H
#define __CSCENEMANAGER_H

#include "RockPCH.h"
#ifdef DIRECT_VERSION_9_MJH
#include "d3dx9math.h"
#else
#include "d3dx8math.h"
#endif // DIRECT_VERSION_9_MJH
#include "obj\\CFaceExp.h"

typedef enum e_SCENE_MODE
{
	SCENE_NORMAL  ,
	SCENE_SQUENCE 
};


class CSceneManager
{
public:
	
	 CSceneManager();
	~CSceneManager();

public:

	void	SetKeyInput(BOOL Value)					{ m_ISKeyInput			 = Value; }
	void	SetGUIDisplay(BOOL Value)				{ m_ISGUIDisplay		 = Value; }
	void	SetOBJDisplay(BOOL Value)				{ m_ISOBJDisplay		 = Value; }
	void	SetNPCDisplay(BOOL Value)				{ m_ISNPCDisplay		 = Value; }
	void	SetPlayerDisplay(BOOL Value)			{ m_ISPlayerDisplay      = Value; }
	void	SetOtherPlayerDisplay(BOOL Value)		{ m_ISOtherPlayerDisplay = Value; }
	void	SetTerrainDisplay(BOOL Value)			{ m_ISTerrainDisplay     = Value; }
	void	SetCursorDisplay(BOOL Value);
	void	SetSkyBoxDisplay(BOOL Value)			{ m_ISSkyBoxDisplay		 = Value; }
	void	SetFogDisplay(BOOL Value)				{ m_ISFogDisplay		 = Value; }
	void	SetBGColor(D3DXCOLOR Color)				{ m_BackGroundColor		 = Color; }  
	void    SetWeatherDisplay(BOOL Value)			{ m_ISWeatherDisplay     = Value; }
	void	SetInsectDisplay(BOOL Value)			{ m_ISInsectDisplay      = Value; }
	void	SetSceneMode(e_SCENE_MODE Mode)			{ m_SceneMode			 = Mode;  }
	void    SetEffectDisplayMode(e_SCENE_MODE Mode) { m_EffectDisplayMode	 = Mode;  }
	void	SetSoundPlayMode(e_SCENE_MODE Mode)		{ m_SoundPlayMode		 = Mode;  }
	void	SetEffectCreateMode(e_SCENE_MODE Mode)	{ m_EffectCreateMode     = Mode;  }
	
	void	SetPlayerWeaponDisplay(BOOL Value)		{ m_ISPlayerWeaponDisplay = Value; }
	void	SetOtherPlayerWeaponDisplay(BOOL Value) { m_ISOtherPlayerWeaponDisplay = Value; }

	void	SetSquencePlayerAniTime(float Range)			{ m_SquencePlayerAniTimeRange      = Range;	}
	void	SetSquenceOtherPlayerAniTime(float Range)		{ m_SquenceOtherPlayerAniTimeRange = Range;	}
	void	SetFixFaceExp(CFaceExpData::FACE_EXP Value)		{ m_FaceExp = Value; }

public:

	BOOL	GetKeyInput(void)					{ return m_ISKeyInput;			  }
	BOOL	GetGUIDisplay(void)					{ return m_ISGUIDisplay;		  }
	BOOL	GetOBJDisplay(void)					{ return m_ISOBJDisplay;		  }
	BOOL	GetNPCDisplay(void)					{ return m_ISNPCDisplay;		  }
	BOOL	GetPlayerDisplay(void)				{ return m_ISPlayerDisplay;		  }
	BOOL	GetOtherPlayerDisplay(void)			{ return m_ISOtherPlayerDisplay;  }
	BOOL	GetTerrainDisplay(void)				{ return m_ISTerrainDisplay;	  }
	BOOL	GetCursorDisplay(void)				{ return m_ISCursorDisplay;		  }
	BOOL	GetSkyBoxDisplay(void)				{ return m_ISSkyBoxDisplay;		  }
	BOOL	GetFogDisplay(void)					{ return m_ISFogDisplay;		  }
	BOOL	GetWeatherDisplay(void)				{ return m_ISWeatherDisplay;	  }
	BOOL	GetInsectDisplay(void)				{ return m_ISInsectDisplay;		  }
	
	BOOL	GetPlayerWeaponDisplay(void)		{ return m_ISPlayerWeaponDisplay; }
	BOOL	GetOtherPlayerWeaponDisplay(void)	{ return m_ISOtherPlayerWeaponDisplay; }

	float	GetSquencePlayerAniTime(void)		{ return m_SquencePlayerAniTimeRange;	    }
	float	GetSquenceOtherPlayerAniTime(void)	{ return m_SquenceOtherPlayerAniTimeRange;	}

	e_SCENE_MODE GetSceneMode(void)				{ return m_SceneMode;		   	  }
	e_SCENE_MODE GetEffectDisplayMode(void)		{ return m_EffectDisplayMode;     }
	e_SCENE_MODE GetEffectCreateMode(void)		{ return m_EffectCreateMode;	  }

	e_SCENE_MODE GetSoundPlayMode(void)			{ return m_SoundPlayMode;	      }

	D3DXCOLOR GetBGColor(void)					{ return m_BackGroundColor;		  }  
	CFaceExpData::FACE_EXP	GetFixFaceExp(void)	{ return m_FaceExp; }

public:

	void	Init(void);							// 디폴트값 설정

private:

	BOOL	m_ISKeyInput;						// 키 입력여부
	BOOL	m_ISGUIDisplay;						// 인터페이스 출력 여부
	BOOL	m_ISOBJDisplay;						// 오브젝트 출력 여부
	BOOL	m_ISNPCDisplay;						// NPC 출력 여부
	BOOL	m_ISPlayerDisplay;					// 플레이어 출력 여부
	BOOL	m_ISOtherPlayerDisplay;				// 다른 플레이어 출력 여부
	BOOL	m_ISTerrainDisplay;					// 지형 출력 여부
	BOOL	m_ISCursorDisplay;					// 커서 출력 여부
	BOOL	m_ISSkyBoxDisplay;					// 스카이 박스 출력 여부
	BOOL	m_ISFogDisplay;						// 포그 출력 여부
	BOOL	m_ISWeatherDisplay;					// 날시 출력 여부
	BOOL	m_ISInsectDisplay;					// 곤충들 출력 여부
	BOOL	m_ISPlayerWeaponDisplay;			// 플레이어 무기 출력 여부
	BOOL	m_ISOtherPlayerWeaponDisplay;		// 다른 플레이어 무기 출력 여부

	D3DXCOLOR	m_BackGroundColor;				// 화면 지울때 색상

	e_SCENE_MODE m_SceneMode;					// 현재 씬상태
	e_SCENE_MODE m_EffectDisplayMode;			// 이펙트 출력 모드
	e_SCENE_MODE m_EffectCreateMode;			// 이펙트 생성 모드	
	e_SCENE_MODE m_SoundPlayMode;				// 사운드 플레이 모드

	float	m_SquencePlayerAniTimeRange;				// 플레이어 에니메이션 속도
	float	m_SquenceOtherPlayerAniTimeRange;			// 다른 플레이어 에니메이션 속도

	CFaceExpData::FACE_EXP m_FaceExp;
};

#endif