#ifndef __COMEFFPROGBAR_H__
#define __COMEFFPROGBAR_H__
//-----------------------------------------------------------------------------

#include "ProgressBar.h"
#include "ComEffImgBox.h"

class CComEffProgBar : public CProgressBar
{
protected:	
	bool	m_IsInit;				 //�ʱ�ȭ����
	
	int m_iNowLevel;

	int m_iPrevGPercent;
	int m_iNowGPercent;

	bool    m_bSwitchComLevel;		 //�޺����� ��ȯ����
	bool	m_bRenderGlow;			 //�۷ο� ��������
	float	m_fGlowTime;			 //�۷ο� ����Ÿ��
	float	m_fGCumulTime;			 //�۷ο� ����Ÿ��
	
	bool	m_bFlashFlag;			 //���� �÷���
	float	m_fFCumulTime;			 //���� ��������Ÿ��
	float   m_fFInterTime;			 //���� ��������Ÿ��
	
	int		m_si;					 //���� �ε���	
	int	    m_ei;					 //���� �ε���
	SComboEffectInfo m_ComEffInfo;	 //�޺� ����

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