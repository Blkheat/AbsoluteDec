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
	n_SGROUP_QUICK,				//메인창 - 퀵슬롯
	n_SGROUP_SKILL,				//스킬창 
	n_SGROUP_SKILL_ShortCut,	//스킬창 단축키 
	n_SGROUP_KEEPUPSKILL,		//미니 지속 스킬 아이콘 
	n_SGROUP_WEAR,				//소지품창 - 장착
	n_SGROUP_INVENTORY,			//소지품창 - 인벤토리	
	n_SGROUP_EXCHANGE,			//교환창
	n_SGROUP_STORE,				//상점창
	n_SGROUP_WAREHOUSE,			//보관창
	n_SGROUP_TUNING, 			//개조창
	n_SGROUP_USER_STORE_SELL,	//개인상점 팔기창
	n_SGROUP_USER_STORE_BUY,	//개인상점 사기창
	n_SGROUP_KEEPUP_COMMERCIALITEM,			// 지속성 상용 아이템 
	n_SGROUP_PAYITEM_WND,		// 지속성 상용 아이템 
	n_SGROUP_PET_INVENTORY,	//펫 인벤 
	n_SGROUP_ELDA_MIX,		// 엘다 조합 
	n_SGROUP_ELDA_MIX_PRE0,	// 엘다 미리 보기
	n_SGROUP_ELDA_MIX_PRE1,	// 엘다 미리 보기
	n_SGROUP_SKILL_POTION,
	n_SGROUP_GUILDINVENTORY,  //길드 창고
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
		
	void SetDelayTimeDisplay( bool aIsDraw, DWORD aDelayTime );// 단위 : ms
	void SetDelayShow(bool Show){ IsDelayShow = Show;}

	//by simwoosng
	void SetScaleTrans(bool bFlag);
	void SetButtonSlot(bool bFlag);

	//by dongs 
	void CheckDelay();

	N_SLOTGROUP		Group;//소속( 퀵슬롯창, 스킬창, 인벤토리창(2), 교환창, 상점창, 은행창 )
	int				SlotNum;
	BYTE			Class; //장비 인벤에서만 쓰기 위해서 만듬....
	BYTE			Type;  //라이딩 아이템 장비를 위해서만 사용	

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