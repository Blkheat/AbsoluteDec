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
	bool	m_IsInit;				 //초기화여부
	bool	m_bPosInit;				 //포지션 초기화 여부	
	UINT	m_uiNowEffStep;		     //현재 연출단계	
	
	float	m_fCumulativeTime;		 //누적타임

	float	m_fMaxIntervalTime;		 //맥스점멸 인터벌 타임
	float	m_fMaxFlashCumulTime;	 //맥스점멸 누적 타임
	bool	m_bMaxFlashSwitchFlag; 	 //맥스점멸 스위칭 플래그
	
	bool	m_bStartFlash;			 //점멸시작여부	
	bool	m_bFlashFlag;			 //점멸 플래그
	float	m_fFCumulTime;			 //점멸 렌더누적타임
	float   m_fFInterTime;			 //점멸 렌더간격타임
	
	bool	m_IsPushEffect;			 //콤보추가 이펙트 여부
	bool	m_IsRenderImg;			 //콤보게이지이미지 렌더여부	
	
	
	bool	m_IsMax;				 //6단계 이미지 렌더여부	
	bool	m_bGlowSwitch;			 //글로우 스위치 플래그
	
	float	m_PushStep_01ExTime;	 //콤보추가연출단계1 실행타임
	float	m_PushStep_02ExTime;	 //콤보추가연출단계2 실행타임
	float	m_PushStep_03ExTime;	 //콤보추가연출단계3 실행타임
	float	m_PushStep_04ExTime;	 //콤보추가연출단계4 실행타임
	float	m_PushStep_05ExTime;	 //콤보추가연출단계5 실행타임	
	
	float	m_PopStep_01ExTime;		 //콤보제거연출단계1 실행타임
	float	m_PopStep_02ExTime;		 //콤보제거연출단계2 실행타임
	
	bool	m_bRenderGlow;			 //글로우 렌더여부
	float	m_fGlowTime;			 //글로우 렌더타임
	float	m_fGCumulTime;			 //글로우 누적타임	

	
	int	m_iCx;						 //아이템 렌더위치 X	
	int	m_iCy;					     //아이템 렌더위치 Y
	int	m_iCw;					     //아이템 렌더가로길이	
	int	m_iCh;					     //아이템 렌더세로길이
	int	m_iColor;  				     //아이템 렌더색상
	
	POINT m_RealCenPos;				 //실제중심 위치
	float m_scaleRatio;				 //원본과의 크기비율
	
	POINT  m_TempPos;
	POINT  m_sPos;
	POINT  m_ePos;
	POINT  m_cenPos;
	
	TEXID  m_RenderTexImg;		      //렌더링 이미지
	SComboEffectInfo	m_ComEffInfo; //현재 콤보이펙트 정보
	
	CDataQueue < SComboEffectInfo >	m_ComEffQueue;


	TEXID  m_uTransImg;				  //변환 이미지
	int	   m_uTimgCw;				  //변환 이미지 가로길이
	int	   m_uTimgCh;				  //변환 이미지 세로길이

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
	float m_fCrossRenTime;		//크로스 렌더타임
	float m_fCrossCumTime;		//크로스 누적타임
	UINT  m_fCrossStep;


	void RenderCrossFlash();
};

//-----------------------------------------------------------------------------
#endif	__COMEFFIMGBOX_H__