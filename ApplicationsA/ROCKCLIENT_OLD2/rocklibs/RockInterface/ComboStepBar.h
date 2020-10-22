#ifndef __COMBOSTEPBAR_H__
#define __COMBOSTEPBAR_H__
//-----------------------------------------------------------------------------

#include "UiUtil.h"
#include "Wnd.h"
#include "ComEffImgBox.h"

class CComboStepBar : public CWnd
{
protected:
	int			m_nMaxStep;								//�ִ� �ܰ谪
	bool *		m_pIsStepGlow;							//�߱� ����
	float		m_fStepGlowtime;						//���� �߱� �ð�	
	float		m_fStepGlowCumtime;						//���� �߱� �����ð�
	
	bool		m_IsSwitchComLevel;						//�޺� ������ȯ ����
	float		m_fSwitchComGlowtime;					//��ȯ �߱�����
	float		m_fSwitchComCumtime;					//��ȯ �߱�����Ÿ��


	int			m_nNowStep;								//���� �ܰ谪
	int			m_nGabPixelW;							//���� ����- �ȼ���

	int			m_nNowLevel;

	int			m_si;									//���� �ε���	
	int			m_ei;									//���� �ε���
	SComboEffectInfo m_ComEffInfo;						//�޺� ����

protected:
	TEXID		m_StepTID;								//�������� ����ִ� �̹���
	TEXID		m_StepEffTID;							//���� �߱� ����ִ� �̹���
	int			m_StepCw;								//�����̹��� ���α���
	int			m_StepCh;								//�����̹��� ���α���			
	
	int			m_EffTID;								//���� �̹���
	int			m_EffCx;								//���� �̹��� x��ġ
	int			m_EffCy;								//���� �̹��� y��ġ
	int			m_EffCw;								//���� �̹��� ���α���
	int			m_EffCh;								//���� �̹��� ���α���
	
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