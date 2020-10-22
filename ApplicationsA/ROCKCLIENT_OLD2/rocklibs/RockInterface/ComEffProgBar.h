#ifndef __COMEFFPROGBAR_H__
#define __COMEFFPROGBAR_H__
//-----------------------------------------------------------------------------

#include "ProgressBar.h"
#include "ComEffImgBox.h"

class CComEffProgBar : public CProgressBar
{
protected:	
	bool	m_IsInit;				 //초기화여부
	
	int m_iNowLevel;

	int m_iPrevGPercent;
	int m_iNowGPercent;

	bool    m_bSwitchComLevel;		 //콤보레벨 전환여부
	bool	m_bRenderGlow;			 //글로우 렌더여부
	float	m_fGlowTime;			 //글로우 렌더타임
	float	m_fGCumulTime;			 //글로우 누적타임
	
	bool	m_bFlashFlag;			 //점멸 플래그
	float	m_fFCumulTime;			 //점멸 렌더누적타임
	float   m_fFInterTime;			 //점멸 렌더간격타임
	
	int		m_si;					 //시작 인덱스	
	int	    m_ei;					 //종료 인덱스
	SComboEffectInfo m_ComEffInfo;	 //콤보 정보

public:
	CComEffProgBar(){}
	CComEffProgBar( SDesktop* DT );
	virtual ~CComEffProgBar();
	
	virtual void RenderProc();
public:
	void InitData();
	void SetSkillGauge(int Gauge);
	int  GetNowLevel()	{ return m_iNowLevel; }

public:
	CComEffImgBox * pComEffImgBox[5];
	void SetSwitchComLevelFlag(bool flag) 
	{
		m_bSwitchComLevel = flag;
		m_bFlashFlag = true;
		m_fFCumulTime = 0.0f;
	}
};

//-----------------------------------------------------------------------------
#endif	__COMEFFPROGBAR_H__