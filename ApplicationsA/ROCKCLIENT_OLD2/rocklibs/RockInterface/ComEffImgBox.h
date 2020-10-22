#ifndef __COMEFFIMGBOX_H__
#define __COMEFFIMGBOX_H__
//-----------------------------------------------------------------------------

#include "UiUtil.h"
#include "ImageBox.h"

enum ComboEffectStep
{
	ComboEffectStep_None,
	ComboEffectStep_01,
	ComboEffectStep_02,
	ComboEffectStep_03,
	ComboEffectStep_04,
	ComboEffectStep_05,
};

enum CrossRenStep
{
	Cross_VerRenderStep,
	Cross_HorRenderStep,
};

struct SComboEffectInfo
{
	bool	IsPushEffect;			  
	bool	IsMax;
};

class CComboStepBar;

class CComEffImgBox : public CImageBox
{
protected:
	bool	m_IsInit;				 //�ʱ�ȭ����
	bool	m_bPosInit;				 //������ �ʱ�ȭ ����	
	UINT	m_uiNowEffStep;		     //���� ����ܰ�	
	
	float	m_fCumulativeTime;		 //����Ÿ��

	float	m_fMaxIntervalTime;		 //�ƽ����� ���͹� Ÿ��
	float	m_fMaxFlashCumulTime;	 //�ƽ����� ���� Ÿ��
	bool	m_bMaxFlashSwitchFlag; 	 //�ƽ����� ����Ī �÷���
	
	bool	m_bStartFlash;			 //������ۿ���	
	bool	m_bFlashFlag;			 //���� �÷���
	float	m_fFCumulTime;			 //���� ��������Ÿ��
	float   m_fFInterTime;			 //���� ��������Ÿ��
	
	bool	m_IsPushEffect;			 //�޺��߰� ����Ʈ ����
	bool	m_IsRenderImg;			 //�޺��������̹��� ��������	
	
	
	bool	m_IsMax;				 //6�ܰ� �̹��� ��������	
	bool	m_bGlowSwitch;			 //�۷ο� ����ġ �÷���
	
	float	m_PushStep_01ExTime;	 //�޺��߰�����ܰ�1 ����Ÿ��
	float	m_PushStep_02ExTime;	 //�޺��߰�����ܰ�2 ����Ÿ��
	float	m_PushStep_03ExTime;	 //�޺��߰�����ܰ�3 ����Ÿ��
	float	m_PushStep_04ExTime;	 //�޺��߰�����ܰ�4 ����Ÿ��
	float	m_PushStep_05ExTime;	 //�޺��߰�����ܰ�5 ����Ÿ��	
	
	float	m_PopStep_01ExTime;		 //�޺����ſ���ܰ�1 ����Ÿ��
	float	m_PopStep_02ExTime;		 //�޺����ſ���ܰ�2 ����Ÿ��
	
	bool	m_bRenderGlow;			 //�۷ο� ��������
	float	m_fGlowTime;			 //�۷ο� ����Ÿ��
	float	m_fGCumulTime;			 //�۷ο� ����Ÿ��	

	
	int	m_iCx;						 //������ ������ġ X	
	int	m_iCy;					     //������ ������ġ Y
	int	m_iCw;					     //������ �������α���	
	int	m_iCh;					     //������ �������α���
	int	m_iColor;  				     //������ ��������
	
	POINT m_RealCenPos;				 //�����߽� ��ġ
	float m_scaleRatio;				 //�������� ũ�����
	
	POINT  m_TempPos;
	POINT  m_sPos;
	POINT  m_ePos;
	POINT  m_cenPos;
	
	TEXID  m_RenderTexImg;		      //������ �̹���
	SComboEffectInfo	m_ComEffInfo; //���� �޺�����Ʈ ����
	
	CDataQueue < SComboEffectInfo >	m_ComEffQueue;


	TEXID  m_uTransImg;				  //��ȯ �̹���
	int	   m_uTimgCw;				  //��ȯ �̹��� ���α���
	int	   m_uTimgCh;				  //��ȯ �̹��� ���α���

public:
	CComboStepBar* 	m_pControlStepBar;

	
public:
	CComEffImgBox(){}
	CComEffImgBox( SDesktop* DT );
	virtual ~CComEffImgBox();
	
	virtual void RenderProc();
	
protected:
	void RenderPushEffStep_01();
	void RenderPushEffStep_02();
	void RenderPushEffStep_03();
	void RenderPushEffStep_04();	
	
	void RenderPopEffStep_01();
	void RenderPopEffStep_02();
	
public:	
	void InitData();
	void InsertComEffInfo( SComboEffectInfo ComEffInfo );
	void UpdateFlashSwitchFlag();

	void SetTransImgID(UINT uImg);
	void SetTransImgIDSize(int nCw, int nCh);

	void GetTransInfo(bool bFlag, int & nCw, int & nCh);

protected:
	bool  m_IsRenderCross;	
	float m_fCrossRenTime;		//ũ�ν� ����Ÿ��
	float m_fCrossCumTime;		//ũ�ν� ����Ÿ��
	UINT  m_fCrossStep;


	void RenderCrossFlash();
};

//-----------------------------------------------------------------------------
#endif	__COMEFFIMGBOX_H__