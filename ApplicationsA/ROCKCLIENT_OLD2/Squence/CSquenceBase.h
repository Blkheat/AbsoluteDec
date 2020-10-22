#ifndef __CSQUENCEBASE_H
#define __CSQUENCEBASE_H

#include "RockPCH.h"
#ifdef DIRECT_VERSION_9_MJH
#include "d3dx9math.h"
#else
#include "d3dx8math.h"
#endif // DIRECT_VERSION_9_MJH
#include "..\\CRenderManager.h"
//..........................................................................................................
// CSQFunctor
//..........................................................................................................
template<class T>
class CSQFunctor
{

public:
	
	T       *m_pObj;
	typedef int (T::*pFunc)();

	pFunc m_pFunc;

public:

	  CSQFunctor() 
	  {
		m_pObj  = NULL;
		m_pFunc = NULL;
	  }

	 ~CSQFunctor()
	 {
	 }

	void Clear(void)
	{
		m_pObj  = NULL;
		m_pFunc = NULL;
	}

	void  operator =(const CSQFunctor &TempData)
	{
		m_pObj  = TempData.m_pObj;
		m_pFunc = TempData.m_pFunc;
	}

	int  operator ()()
	{
		if( m_pObj == NULL || m_pFunc == NULL  )
		{
			return FALSE;
		}
		
		T		*pTempObj  = m_pObj;
		pFunc    pTempFunc = m_pFunc;

		return (pTempObj->*pTempFunc)();
	}

	void  Insert(T *pObj,pFunc pf)
	{
		m_pObj  = pObj;
		m_pFunc = pf;
	}

	pFunc GetFunc(void)
	{
		return m_pFunc;
	}

};//..........................................................................................................

#define SQUENCE_MAX_EVENT	30

class CSquenceManager;
class CSquenceBase;

typedef int (CSquenceBase::*pFunc)();

typedef enum SQUENCE_WAIT_TYPE
{
	SWT_NONE	= -1 ,				// 대기 타입 없음
	SWT_ANI			 ,				// 대기 타입 에니메이션
	SWT_CAMERA		 ,				// 대기 타입 카메라
	SWT_TIMER		 ,				// 대기 타입 시간
	SWT_WAIT						// 대기 타입 없음 ( SquenceCheck 쪽에서 다음 시퀀스로 넘긴다 )
};

class CSquenceBase
{
public:
	 
	 CSquenceBase();
	virtual ~CSquenceBase();

public:

	virtual int Squence00()				{ return FALSE; }
	virtual int Squence01()				{ return FALSE; }
	virtual int Squence02()				{ return FALSE; }
	virtual int Squence03()				{ return FALSE; }
	virtual int Squence04()				{ return FALSE; }
	virtual int Squence05()				{ return FALSE; }
	virtual int Squence06()				{ return FALSE; }
	virtual int Squence07()				{ return FALSE; }
	virtual int Squence08()				{ return FALSE; }
	virtual int Squence09()				{ return FALSE; }
	virtual int Squence10()				{ return FALSE; }
	virtual int Squence11()				{ return FALSE; }
	virtual int Squence12()				{ return FALSE; }
	virtual int Squence13()				{ return FALSE; }
	virtual int Squence14()				{ return FALSE; }
	virtual int Squence15()				{ return FALSE; }
	virtual int Squence16()				{ return FALSE; }
	virtual int Squence17()				{ return FALSE; }
	virtual int Squence18()				{ return FALSE; }
	virtual int Squence19()				{ return FALSE; }
	virtual int Squence20()				{ return FALSE; }
	virtual int Squence21()				{ return FALSE; }
	virtual int Squence22()				{ return FALSE; }
	virtual int Squence23()				{ return FALSE; }
	virtual int Squence24()				{ return FALSE; }
	virtual int Squence25()				{ return FALSE; }
	virtual int Squence26()				{ return FALSE; }
	virtual int Squence27()				{ return FALSE; }
	virtual int Squence28()				{ return FALSE; }
	virtual int Squence29()				{ return FALSE; }

public:

	virtual int SquenceCheck00()		{ return TRUE; }
	virtual int SquenceCheck01()		{ return TRUE; }
	virtual int SquenceCheck02()		{ return TRUE; }
	virtual int SquenceCheck03()		{ return TRUE; }
	virtual int SquenceCheck04()		{ return TRUE; }
	virtual int SquenceCheck05()		{ return TRUE; }
	virtual int SquenceCheck06()		{ return TRUE; }
	virtual int SquenceCheck07()		{ return TRUE; }
	virtual int SquenceCheck08()		{ return TRUE; }
	virtual int SquenceCheck09()		{ return TRUE; }
	virtual int SquenceCheck10()		{ return TRUE; }
	virtual int SquenceCheck11()		{ return TRUE; }
	virtual int SquenceCheck12()		{ return TRUE; }
	virtual int SquenceCheck13()		{ return TRUE; }
	virtual int SquenceCheck14()		{ return TRUE; }
	virtual int SquenceCheck15()		{ return TRUE; }
	virtual int SquenceCheck16()		{ return TRUE; }
	virtual int SquenceCheck17()		{ return TRUE; }
	virtual int SquenceCheck18()		{ return TRUE; }
	virtual int SquenceCheck19()		{ return TRUE; }
	virtual int SquenceCheck20()		{ return TRUE; }
	virtual int SquenceCheck21()		{ return TRUE; }
	virtual int SquenceCheck22()		{ return TRUE; }
	virtual int SquenceCheck23()		{ return TRUE; }
	virtual int SquenceCheck24()		{ return TRUE; }
	virtual int SquenceCheck25()		{ return TRUE; }
	virtual int SquenceCheck26()		{ return TRUE; }
	virtual int SquenceCheck27()		{ return TRUE; }
	virtual int SquenceCheck28()		{ return TRUE; }
	virtual int SquenceCheck29()		{ return TRUE; }

public:

	virtual int SquenceFirstScene00()	{ return TRUE; }
	virtual int SquenceFirstScene01()	{ return TRUE; }
	virtual int SquenceFirstScene02()	{ return TRUE; }
	virtual int SquenceFirstScene03()	{ return TRUE; }
	virtual int SquenceFirstScene04()	{ return TRUE; }
	virtual int SquenceFirstScene05()	{ return TRUE; }
	virtual int SquenceFirstScene06()	{ return TRUE; }
	virtual int SquenceFirstScene07()	{ return TRUE; }
	virtual int SquenceFirstScene08()	{ return TRUE; }
	virtual int SquenceFirstScene09()	{ return TRUE; }
	virtual int SquenceFirstScene10()	{ return TRUE; }
	virtual int SquenceFirstScene11()	{ return TRUE; }
	virtual int SquenceFirstScene12()	{ return TRUE; }
	virtual int SquenceFirstScene13()	{ return TRUE; }
	virtual int SquenceFirstScene14()	{ return TRUE; }
	virtual int SquenceFirstScene15()	{ return TRUE; }
	virtual int SquenceFirstScene16()	{ return TRUE; }
	virtual int SquenceFirstScene17()	{ return TRUE; }
	virtual int SquenceFirstScene18()	{ return TRUE; }
	virtual int SquenceFirstScene19()	{ return TRUE; }
	virtual int SquenceFirstScene20()	{ return TRUE; }
	virtual int SquenceFirstScene21()	{ return TRUE; }
	virtual int SquenceFirstScene22()	{ return TRUE; }
	virtual int SquenceFirstScene23()	{ return TRUE; }
	virtual int SquenceFirstScene24()	{ return TRUE; }
	virtual int SquenceFirstScene25()	{ return TRUE; }
	virtual int SquenceFirstScene26()	{ return TRUE; }
	virtual int SquenceFirstScene27()	{ return TRUE; }
	virtual int SquenceFirstScene28()	{ return TRUE; }
	virtual int SquenceFirstScene29()	{ return TRUE; }

public:

	virtual int SquenceAniFrame00()	{ return TRUE; }
	virtual int SquenceAniFrame01()	{ return TRUE; }
	virtual int SquenceAniFrame02()	{ return TRUE; }
	virtual int SquenceAniFrame03()	{ return TRUE; }
	virtual int SquenceAniFrame04()	{ return TRUE; }
	virtual int SquenceAniFrame05()	{ return TRUE; }
	virtual int SquenceAniFrame06()	{ return TRUE; }
	virtual int SquenceAniFrame07()	{ return TRUE; }
	virtual int SquenceAniFrame08()	{ return TRUE; }
	virtual int SquenceAniFrame09()	{ return TRUE; }
	virtual int SquenceAniFrame10()	{ return TRUE; }
	virtual int SquenceAniFrame11()	{ return TRUE; }
	virtual int SquenceAniFrame12()	{ return TRUE; }
	virtual int SquenceAniFrame13()	{ return TRUE; }
	virtual int SquenceAniFrame14()	{ return TRUE; }
	virtual int SquenceAniFrame15()	{ return TRUE; }
	virtual int SquenceAniFrame16()	{ return TRUE; }
	virtual int SquenceAniFrame17()	{ return TRUE; }
	virtual int SquenceAniFrame18()	{ return TRUE; }
	virtual int SquenceAniFrame19()	{ return TRUE; }
	virtual int SquenceAniFrame20()	{ return TRUE; }
	virtual int SquenceAniFrame21()	{ return TRUE; }
	virtual int SquenceAniFrame22()	{ return TRUE; }
	virtual int SquenceAniFrame23()	{ return TRUE; }
	virtual int SquenceAniFrame24()	{ return TRUE; }
	virtual int SquenceAniFrame25()	{ return TRUE; }
	virtual int SquenceAniFrame26()	{ return TRUE; }
	virtual int SquenceAniFrame27()	{ return TRUE; }
	virtual int SquenceAniFrame28()	{ return TRUE; }
	virtual int SquenceAniFrame29()	{ return TRUE; }

	
public:
	
	void    EffectLock(void);							// 이펙트 생성 모드 변경
	void    EffectUnLock(void);							// 이펙트 생성 모드 기존 모드로

public:

	CSQFunctor<CSquenceBase>	m_pFuncList[SQUENCE_MAX_EVENT];
	CSQFunctor<CSquenceBase>	m_pFuncCheckList[SQUENCE_MAX_EVENT];
	CSQFunctor<CSquenceBase>	m_pFuncFirstSceneList[SQUENCE_MAX_EVENT];
	CSQFunctor<CSquenceBase>	m_pFuncAniFrameSceneList[SQUENCE_MAX_EVENT];
	
public:

	CSquenceManager			   *m_pSquenceManager;
	
public:

	void	SetWaitFlag(SQUENCE_WAIT_TYPE Value,DWORD Timer = 0);
	void    NextSquence(void);

	void	SetPlayerAni(int AniIndex);
	void	SetPlayerNextAni(int AniIndex);

	void    PlaySound(int Index,float fx,float fy,float fz);
	void	PlaySound(int Index,D3DXVECTOR3 *pvPos);
	void	StopAllSound(void);
	void	UpdateSound(int Index);

	void	SetPlayerAniTime(float Range);
	void	SetOtherPlayerAniTime(float Range);
};

#endif