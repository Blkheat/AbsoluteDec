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
	///--  2D��ġ ���ÿ��� ����
	bool			Is3DPos;	
	
	SPcItem			pcInfo;				//������ ����
	bool			isMoney;			//���� �̵� ����
	bool			isGet;				//�������� �޴°����� ����
	int				interNpcId;			//���� Monster,NPC ���̵�
	bool			isMonster;			//Monster���� NPC���� ����
	D3DXVECTOR3		upPosition;			//������ġ
	POINT			upPos;				//2D ������ġ
	
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
	int							  nNPCId;						//���� NPC���̵�		
	bool						  IsActive;						//���� ť���� ���� ����
	
	UINT						  uiNowEffStep;					//���� ����ܰ�
	SItemTransInfo				  ItemTransInfo;				//����������

	float						  fCumulativeTime;				//����Ÿ��

	bool						  bRenderGlow;					//�۷ο� ��������
	float						  fGCumulTime;					//�۷ο� ����Ÿ��

	int							  nCx;							//������ ������ġ X	
	int							  nCy;						    //������ ������ġ Y
	int							  nCw;	     					//������ �������α���	
	int							  nCh;							//������ �������α���
	D3DCOLOR					  dColor;		     			//������ ��������

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
	 bool	m_IsInit;				 //�ʱ�ȭ����
	 	
     float  m_fMonGetStep_01ExTime;	 //���Ϳ���ܰ�1 ����Ÿ��
	 float  m_fMonGetStep_02ExTime;	 //���Ϳ���ܰ�2 ����Ÿ��
	 float  m_fMonGetStep_03ExTime;	 //���Ϳ���ܰ�3 ����Ÿ��
	 float  m_fMonGetStep_04ExTime;	 //���Ϳ���ܰ�4 ����Ÿ��

	 float  m_fNpcGetStep_01ExTime;	 //NPC����ܰ�1 ����Ÿ��
	 float	m_fNpcGetStep_02ExTime;	 //NPC����ܰ�2 ����Ÿ��
	 float	m_fNpcGetStep_03ExTime;	 //NPC����ܰ�3 ����Ÿ��	 
	 	
	 bool   m_bButtonGlow;			 //��ư�۷ο� ��������
	 float	m_fBGCumulTime;			 //��ư �۷ο� ����Ÿ��

	 float	m_fGlowTime;			 //�۷ο� ����Ÿ��
	 	 
	 SItemTransQueueInfo *	m_pItemTransQueueInfo;					//������ ���� ���� �̵�ť���� ������
	 
	 list < SItemTransQueueInfo * > m_MonsterTItemQueueInfoList;	//Monster ������ �̵�ť ���� ����Ʈ	 
	 SItemTransQueueInfo m_NPCTItemQueueInfo;					    //NPC ������ �̵�ť ����
	 
	 SItemTransQueueInfo m_2DPosQueueInfo;							//2D��ġ �̵�ť ����

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