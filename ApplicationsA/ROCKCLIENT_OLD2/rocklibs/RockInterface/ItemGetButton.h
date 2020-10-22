#ifndef __ITEMGETBUTTON_H__
#define __ITEMGETBUTTON_H__
//-----------------------------------------------------------------------------

#include "UiUtil.h"
#include "Wnd.h"
#include "Button.h"

#include <list>

using namespace	std;

enum ITemTransEffStep
{
	ITemTransEffStep_None,
	ITemTransEffStep_01,
	ITemTransEffStep_02,
	ITemTransEffStep_03,
	ITemTransEffStep_04,
};

struct SItemTransInfo
{
	///--  2D위치 관련여부 여부
	bool			Is3DPos;	
	
	SPcItem			pcInfo;				//아이템 정보
	bool			isMoney;			//돈의 이동 여부
	bool			isGet;				//아이템을 받는것인지 여부
	int				interNpcId;			//연관 Monster,NPC 아이디
	bool			isMonster;			//Monster인지 NPC인지 여부
	D3DXVECTOR3		upPosition;			//생성위치
	POINT			upPos;				//2D 생성위치
	
	SItemTransInfo()
	{
		memset(this, 0, sizeof(SItemTransInfo));		
	}
	
	void operator=(const SItemTransInfo &temp)
	{
		Is3DPos		= temp.Is3DPos;		
		pcInfo		= temp.pcInfo;
		isMoney		= temp.isMoney;
		isGet		= temp.isGet;
		interNpcId  = temp.interNpcId;
		isMonster	= temp.isMonster;
		upPosition	= temp.upPosition;
		upPos		= temp.upPos;
	}
};

struct SItemTransQueueInfo
{
	int							  nNPCId;						//관계 NPC아이디		
	bool						  IsActive;						//현재 큐정보 동작 유무
	
	UINT						  uiNowEffStep;					//현재 연출단계
	SItemTransInfo				  ItemTransInfo;				//아이템정보

	float						  fCumulativeTime;				//누적타임

	bool						  bRenderGlow;					//글로우 렌더여부
	float						  fGCumulTime;					//글로우 누적타임

	int							  nCx;							//아이템 렌더위치 X	
	int							  nCy;						    //아이템 렌더위치 Y
	int							  nCw;	     					//아이템 렌더가로길이	
	int							  nCh;							//아이템 렌더세로길이
	D3DCOLOR					  dColor;		     			//아이템 렌더색상

	POINT						  SPos;	
	POINT						  EPos;

	POINT						  TempPos;

	CDataQueue < SItemTransInfo > ItemTransQueue;

	SItemTransQueueInfo()
	{
		ItemTransQueue.Clear();
		
		nNPCId = -1;
		IsActive = false;
		
		uiNowEffStep = ITemTransEffStep_None;

		fCumulativeTime = 0.0f;

		bRenderGlow = false;
		fGCumulTime = 0.0f;

		nCx = 0;							
		nCy = 0;						    
		nCw = 0;	     						
		nCh = 0;							
    	dColor = 0;
	}
};

class CItemTransButton : public CButton
{
protected:
	 bool	m_IsInit;				 //초기화여부
	 	
     float  m_fMonGetStep_01ExTime;	 //몬스터연출단계1 실행타임
	 float  m_fMonGetStep_02ExTime;	 //몬스터연출단계2 실행타임
	 float  m_fMonGetStep_03ExTime;	 //몬스터연출단계3 실행타임
	 float  m_fMonGetStep_04ExTime;	 //몬스터연출단계4 실행타임

	 float  m_fNpcGetStep_01ExTime;	 //NPC연출단계1 실행타임
	 float	m_fNpcGetStep_02ExTime;	 //NPC연출단계2 실행타임
	 float	m_fNpcGetStep_03ExTime;	 //NPC연출단계3 실행타임	 
	 	
	 bool   m_bButtonGlow;			 //버튼글로우 렌더여부
	 float	m_fBGCumulTime;			 //버튼 글로우 누적타임

	 float	m_fGlowTime;			 //글로우 렌더타임
	 	 
	 SItemTransQueueInfo *	m_pItemTransQueueInfo;					//아이템 현재 동작 이동큐정보 포인터
	 
	 list < SItemTransQueueInfo * > m_MonsterTItemQueueInfoList;	//Monster 아이템 이동큐 정보 리스트	 
	 SItemTransQueueInfo m_NPCTItemQueueInfo;					    //NPC 아이템 이동큐 정보
	 
	 SItemTransQueueInfo m_2DPosQueueInfo;							//2D위치 이동큐 정보

public:
	 CItemTransButton() {}
	 CItemTransButton( SDesktop* DT );	 
	 virtual ~CItemTransButton();

	 void UpdateProc();	 
	 
	 virtual void RenderProc();
	 void RenderItemTransProc( SItemTransQueueInfo * pItemTransQueueInfo );

protected:
	 void RenderMonGetEffStep_01();
	 void RenderMonGetEffStep_02();
	 void RenderMonGetEffStep_03();
	 void RenderMonGetEffStep_04();

	 void RenderNpcGetEffStep_01();
	 void RenderNpcGetEffStep_02();
	 void RenderNpcGetEffStep_03();

	 void Render2DPosGetEffStep_01();
	 void Render2DPosGetEffStep_02();
	 void Render2DPosGetEffStep_03();
	 void Render2DPosGetEffStep_04();
 
public:
	 void InitData();
	 void InsertItemTransInfo(SItemTransInfo ItemTransInfo);
	 
	 SItemTransQueueInfo * FindMonsterTransItemQueueInfo(int nInterNpcId);
	 void SetActivateItemTransQueueInfo(int nInterNpcId);
};

//-----------------------------------------------------------------------------
#endif	__ITEMGETBUTTON_H__