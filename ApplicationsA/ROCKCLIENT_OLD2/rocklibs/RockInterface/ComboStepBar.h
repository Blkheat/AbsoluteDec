#ifndef __COMBOSTEPBAR_H__
#define __COMBOSTEPBAR_H__
//-----------------------------------------------------------------------------

#include "UiUtil.h"
#include "Wnd.h"
#include "ComEffImgBox.h"

class CComboStepBar : public CWnd
{
protected:
	int			m_nMaxStep;								//최대 단계값
	bool *		m_pIsStepGlow;							//발광 여부
	float		m_fStepGlowtime;						//스텝 발광 시간	
	float		m_fStepGlowCumtime;						//스텝 발광 누적시간
	
	bool		m_IsSwitchComLevel;						//콤보 레벨전환 여부
	float		m_fSwitchComGlowtime;					//전환 발광여부
	float		m_fSwitchComCumtime;					//전환 발광누적타임


	int			m_nNowStep;								//현재 단계값
	int			m_nGabPixelW;							//스텝 간격- 픽셀값

	int			m_nNowLevel;

	int			m_si;									//시작 인덱스	
	int			m_ei;									//종료 인덱스
	SComboEffectInfo m_ComEffInfo;						//콤보 정보

protected:
	TEXID		m_StepTID;								//스텝으로 찍어주는 이미지
	TEXID		m_StepEffTID;							//스텝 발광 찍어주는 이미지
	int			m_StepCw;								//스텝이미지 가로길이
	int			m_StepCh;								//스텝이미지 세로길이			
	
	int			m_EffTID;								//연출 이미지
	int			m_EffCx;								//연출 이미지 x위치
	int			m_EffCy;								//연출 이미지 y위치
	int			m_EffCw;								//연출 이미지 가로길이
	int			m_EffCh;								//연출 이미지 세로길이
	
public:
	CComEffImgBox * pComEffImgBox[6];
	
public:	
	CComboStepBar() {}
	CComboStepBar( SDesktop* DT );	 
	virtual ~CComboStepBar();

	virtual void RenderProc();
	void RenderSwitchComLevel();

public:
	void		SetMaxStep(int nStep);
	void		SetNowStep(int nStep);
	void		SetGabPixelW(int nCw);

	void SetSkillGauge(int nGauge);
	int  GetNowLevel()	{ return m_nNowLevel; }
	void InitData();

	void SetSwitchComLevel(bool bFlag);

public:

	void		SetStepTid(TEXID uTid);
	void		SetStepEffTid(TEXID uTid);
	void		SetStepSize(int nCw, int nCh);

	void		SetEffTID(TEXID uTid);
	void		SetEffSize(int nCx, int nCy, int nCw, int nCh);
};

//-----------------------------------------------------------------------------
#endif	__COMEFFIMGBOX_H__