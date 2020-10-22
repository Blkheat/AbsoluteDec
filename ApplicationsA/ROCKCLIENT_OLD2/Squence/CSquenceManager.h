#ifndef __CSQUENCEMANAGER_H
#define __CSQUENCEMANAGER_H

#include "RockPCH.h"
#include "CSquenceBase.h"
#include "..\\CSceneManager.h"

class CSquenceManager
{
public:
	 
	 CSquenceManager();
	~CSquenceManager();

public:

	void	FirstSceneUpdate(void);
	void	Update(void);
	void	AniFrameUpdate(void);

	void    Start(CSquenceBase *pSquenceBase);
	void	ResetWaitFlag(SQUENCE_WAIT_TYPE Type);
	void	DeleteAllMemory(void);

	void	SetPlayerAni(int AniIndex);
	void	SetPlayerNextAni(int AniIndex);
	int     GetPlayerNextAni(void);

	void    EffectSquenceLock(void);							// 이펙트 생성 모드 변경
	void    EffectSquenceUnLock(void);							// 이펙트 생성 모드 기존 모드로

	void    PlaySquenceSound(int Index,float fx,float fy,float fz);
	void	PlaySquenceSound(int Index,D3DXVECTOR3 *pvPos);
	void	StopAllSound(void);
	void	UpdateSound(int Index);

	void	SetPlayerAniTime(float Range);
	void	SetOtherPlayerAniTime(float Range);
	
protected:
	
	int							m_Count;
	CSquenceBase			   *m_pSquenceBase;
	CSQFunctor<CSquenceBase>	m_pFuncCheck;
	CSQFunctor<CSquenceBase>	m_pFuncFirstScene;
	CSQFunctor<CSquenceBase>	m_pFuncAniFrameScene;
	DWORD						m_OldTimer;

public:
	
	SQUENCE_WAIT_TYPE			m_WaitTye;
	DWORD						m_Timer;
	BOOL						m_ISProcess;

protected:

	int  	NextSquence(void);
	void	End(void);

	int		m_PlayerNextAniIndex;

	e_SCENE_MODE m_SaveLockEffectMode;
	
	BOOL		 m_ISNextSquenceRun;
};

#endif