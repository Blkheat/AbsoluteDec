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

	void	Init(void);							// ����Ʈ�� ����

private:

	BOOL	m_ISKeyInput;						// Ű �Է¿���
	BOOL	m_ISGUIDisplay;						// �������̽� ��� ����
	BOOL	m_ISOBJDisplay;						// ������Ʈ ��� ����
	BOOL	m_ISNPCDisplay;						// NPC ��� ����
	BOOL	m_ISPlayerDisplay;					// �÷��̾� ��� ����
	BOOL	m_ISOtherPlayerDisplay;				// �ٸ� �÷��̾� ��� ����
	BOOL	m_ISTerrainDisplay;					// ���� ��� ����
	BOOL	m_ISCursorDisplay;					// Ŀ�� ��� ����
	BOOL	m_ISSkyBoxDisplay;					// ��ī�� �ڽ� ��� ����
	BOOL	m_ISFogDisplay;						// ���� ��� ����
	BOOL	m_ISWeatherDisplay;					// ���� ��� ����
	BOOL	m_ISInsectDisplay;					// ����� ��� ����
	BOOL	m_ISPlayerWeaponDisplay;			// �÷��̾� ���� ��� ����
	BOOL	m_ISOtherPlayerWeaponDisplay;		// �ٸ� �÷��̾� ���� ��� ����

	D3DXCOLOR	m_BackGroundColor;				// ȭ�� ���ﶧ ����

	e_SCENE_MODE m_SceneMode;					// ���� ������
	e_SCENE_MODE m_EffectDisplayMode;			// ����Ʈ ��� ���
	e_SCENE_MODE m_EffectCreateMode;			// ����Ʈ ���� ���	
	e_SCENE_MODE m_SoundPlayMode;				// ���� �÷��� ���

	float	m_SquencePlayerAniTimeRange;				// �÷��̾� ���ϸ��̼� �ӵ�
	float	m_SquenceOtherPlayerAniTimeRange;			// �ٸ� �÷��̾� ���ϸ��̼� �ӵ�

	CFaceExpData::FACE_EXP m_FaceExp;
};

#endif