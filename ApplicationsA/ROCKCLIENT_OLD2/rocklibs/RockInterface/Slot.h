///////////////////////////////////////////////////////////////////////////////
///
///		File		: Slot.h
///		Desc		:
///
///		Author		: Park Soo-Hyun
///		Team		: Program - Client Team
///		Date		: 2004-06-09
///
///		Copyright(c) 2004, Rocksoft Co., Ltd. ALL RIGHTS RESERVED
///
///////////////////////////////////////////////////////////////////////////////
#ifndef __SLOT_H__
#define __SLOT_H__
//-----------------------------------------------------------------------------
#include "Wnd.h"
#include "TextBox.h"


enum N_SLOTGROUP
{
	n_SGROUP_NONE,
	n_SGROUP_QUICK,				//����â - ������
	n_SGROUP_SKILL,				//��ųâ 
	n_SGROUP_SKILL_ShortCut,	//��ųâ ����Ű 
	n_SGROUP_KEEPUPSKILL,		//�̴� ���� ��ų ������ 
	n_SGROUP_WEAR,				//����ǰâ - ����
	n_SGROUP_INVENTORY,			//����ǰâ - �κ��丮	
	n_SGROUP_EXCHANGE,			//��ȯâ
	n_SGROUP_STORE,				//����â
	n_SGROUP_WAREHOUSE,			//����â
	n_SGROUP_TUNING, 			//����â
	n_SGROUP_USER_STORE_SELL,	//���λ��� �ȱ�â
	n_SGROUP_USER_STORE_BUY,	//���λ��� ���â
	n_SGROUP_KEEPUP_COMMERCIALITEM,			// ���Ӽ� ��� ������ 
	n_SGROUP_PAYITEM_WND,		// ���Ӽ� ��� ������ 
	n_SGROUP_PET_INVENTORY,	//�� �κ� 
	n_SGROUP_ELDA_MIX,		// ���� ���� 
	n_SGROUP_ELDA_MIX_PRE0,	// ���� �̸� ����
	n_SGROUP_ELDA_MIX_PRE1,	// ���� �̸� ����
	n_SGROUP_SKILL_POTION,
	n_SGROUP_GUILDINVENTORY,  //��� â��
	n_SGROUP_PROPOSE_FRONTIER,



}; 


enum N_HIGHLIGHT
{
	n_NONE_HIGHLIGHT,
	n_FOCUS_HIGHLIGHT,
	n_PRESS_HIGHLIGHT
};


typedef enum N_SLOTSTATE
{
	n_Slot_Disable, 
	n_Slot_Enable 

} N_SLOTSTATE;



class CSlot : public CTextBox
{
public:
	CSlot() {};
	CSlot( SDesktop* DT );
	virtual ~CSlot(); 

	void Clear();

	//-------------------------------------------------------------------------
	virtual SEventMessage* EventProc( SEventMessage* EMsg );//OR
	virtual void RenderProc();//OR
	virtual IRESULT ReceiveEvent( NEventMessage Event, EPARAM fParam, EPARAM sParam, EPARAM xParam, EPARAM exParam );//OR
	//-------------------------------------------------------------------------

	void SetSlotGroup( N_SLOTGROUP group );
	void SetSlotGroup( N_SLOTGROUP group, int slotnum );
	void SetSlotNum( int slotnum );
	void SetWearMark( int mark );
	void SetWearType( int type );
	
	void SetHighlightImg( TEXID focus_img, TEXID click_img );
	void SetHighlight( N_HIGHLIGHT hl_state ); 
	void SetHighlight( N_HIGHLIGHT hl_state, bool hl_duration );
	void SetHighlight( bool hl_duration );
	void SetSlotItem( SPcItem* pcitem );
	void SetSlotPcItem( SPcItem aPcItem );

	void SetDisableHighlight();

	SPcItem* GetSlotItem();
	SPcItem* GetSlotItemMove();
		
	void SetDelayTimeDisplay( bool aIsDraw, DWORD aDelayTime );// ���� : ms
	void SetDelayShow(bool Show){ IsDelayShow = Show;}

	//by simwoosng
	void SetScaleTrans(bool bFlag);
	void SetButtonSlot(bool bFlag);

	//by dongs 
	void CheckDelay();

	N_SLOTGROUP		Group;//�Ҽ�( ������â, ��ųâ, �κ��丮â(2), ��ȯâ, ����â, ����â )
	int				SlotNum;
	BYTE			Class; //��� �κ������� ���� ���ؼ� ����....
	BYTE			Type;  //���̵� ������ ��� ���ؼ��� ���	

	SPcItem*		PcItem;
	CSlot*			SlotAddr;
	
	N_SLOTSTATE		theSlotState;			
	
protected:
	bool			IsDelayShow;
	bool			IsHighlight;
	bool			HLDuration;
	N_HIGHLIGHT		HLState;
	TEXID			HLImg[3];
	
	//by simwoosung
	bool			IsScaleTrans;
	bool			IsButtonSlot;
	int				m_Bx;
	int				m_By;
		
//	bool			IsDBClick;
//	bool			IsFirstClick;
//	int				NowTime;
//	int				UpTime;


	SEventMessage	UpdateEMsg;
	virtual void UpdateHandler();//OR
	
	virtual void SetAlignText();
	virtual void SetAdjustPosition( int PosX, int PosY );

	//-------------------------------------------------------------------------
	virtual void OnMouseFocus( int Mx, int My );//OR
	virtual void OnMouseLButtonDown( int Mx, int My );//OR
	virtual void OnMouseLButtonPress( int Mx, int My );//OR
	virtual void OnMouseLButtonUp( int Mx, int My );//OR
	//-------------------------------------------------------------------------

};


//-----------------------------------------------------------------------------
#endif	__SLOT_H__