#ifndef __CArcherBar_H
#define __CArcherBar_H

#include "WndProc.h"
#include "FrameWnd.h"

#include <iostream>
#include <vector>

typedef enum HIT_TYPE
{

	HIT_ZONE	  = 0 ,
	CRITICAL_ZONE = 1 ,
	NONE_HIT_ZONE = 2

};

typedef struct s_ArcherTimer
{
	HIT_TYPE	Type;
	int			PosX;
	
	DWORD		StartTime;
	DWORD		EndTime;

	int			TID;

}ARCHER_TIMER;

typedef struct s_HIT_ZONE_TABLE
{

	std::vector<HIT_TYPE>		List;

}HIT_ZONE_TABLE;

class CArcherBar:public CWndProc
{
public:
	 CArcherBar();
	~CArcherBar();

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );
	
	void RenderProc();

	void End(void);
	void Start(int Level);
	int  Hit(void);

	int  ISVisible(void);
	BOOL ISHIT(void) { return m_ISHIT; }

public:

	CFrameWnd    *m_pFrameWnd;
	
private:

	void DeleteMemory(void);

private:
	
	int								m_BarPX;
	int								m_BarPY;
	BOOL							m_ISHIT;							// 히트 했남?

	std::vector<ARCHER_TIMER>		m_TimerList;

	std::vector<HIT_ZONE_TABLE>		m_HitTable;

	HIT_TYPE						m_SelectHit;

	int								m_Count;

	DWORD							m_OldTime;
	
	int								m_GageWidth;
	int								m_StopGageWidth;
	
	//by simwoosung
	float							m_fFlashTime;						//성공시-점멸 타임
	float							m_fCumulativeTime;					//누적 타임
	bool							m_bIsCritical;
	float							m_fGlowTime;						//점멸 타임
	bool							m_bIsGlow;
	float							m_fGowCumtime;

	SRect							m_rGlowRect;
	SPoint2							m_pCenPoint;
	
	int								m_nCriticalWidth;
	int								m_nHitWidth;
	int								m_ArcherMaxTime;
	
public:	
	int								m_Percent;
	float							m_fVelRatio;
};

extern	CArcherBar	g_ArcherBar;

#endif