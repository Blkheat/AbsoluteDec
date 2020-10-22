#if !defined( __RUI_INVENTORY_WNDPROC_H__ )
#define __RUI_INVENTORY_WNDPROC_H__
//-----------------------------------------------------------------------------
#include "..\\RockPCH.h"



#include "FrameWnd.h"
#include "Button.h"
#include "ImageBox.h"
#include "TextBox.h"
#include "EditBox.h"
#include "ProgressBar.h"
#include "SlideBar.h"
#include "CheckButton.h"
#include "RadioButton.h"
#include "StateButton.h"
#include "ListBox.h"
#include "ScrollBar.h"
#include "ComboBox.h"
#include "Slot.h"
#include "Wnd.h"

#include "WndProc.h"


#include "..\\..\\bravolibs\\obj\\player_manager.h" //g_Pc_Manager -> 	SItemBaseInfo theItemInfoTable[c_Max_Item_Num];



const WORD c_ItemCode_Portal_Min( 6100 ); 
const WORD c_ItemCode_Portal_Max( 6150 ); 

const WORD c_ItemCode_Cash_Min(11001);
const WORD c_ItemCode_Cash_Max(60000);


const int c_MaxInventoryItemDrawNum( 25 );

#ifdef WIDE_PAY_INVENTORY
const int c_MaxPayInvenItemDrawNum( 25 );
const int c_MaxPayInvenPcItemNum( 125 );		// c_MaxPayInvenItemDrawNum * Box 5 
#endif

enum N_INVENSTATE
{
	n_INVEN_ITEM,
	n_INVEN_QUEST,
	n_INVEN_HORSE
};

enum N_PAYINVENSTATE
{
	n_PAYINVEN_SHOP1 = 0,
	n_PAYINVEN_SHOP2, 
	n_PAYINVEN_SHOP3, 
	n_PAYINVEN_SHOP4, 
	n_PAYINVEN_SHOP5, 
};

typedef enum NInventorySlotIdx
{
	n_SlotIdx_Coat = 0, 
	n_SlotIdx_Pants, 
	n_SlotIdx_Weapon, 
	n_SlotIdx_Shield, 
	n_SlotIdx_Hair, 
	n_SlotIdx_Sub, 
	n_SlotIdx_Glove, 
	n_SlotIdx_Boots, 
	n_SlotIdx_Accessory1, 
	n_SlotIdx_Accessory2, 
	n_Slotidx_Horse,
	n_Slotidx_Horse_Top,
	n_Slotidx_Horse_Middle,
	n_Slotidx_Horse_Bottom,
	n_Slotidx_Skin_Suit,
	n_Slotidx_Skin_Hair,
	n_Slotidx_Skin_Wing,
} NInventorySlotIdx;



/*
	n_Type_Sword		= 1,		// 1  검
	n_Type_TwohandSword	= 2,		// 2  대검
	n_Type_Bow			= 3,		// 3  활
	n_Type_MagicBook	= 4,		// 4  마법서
	n_Type_Staff		= 5,		// 5  스태프
	n_Type_CrystalBall	= 6,		// 6  수정구
	n_Type_Coat			= 11,		// 11  상의  
	n_Type_Pants		= 12,		// 12  하의  
	n_Type_Glove		= 13,		// 13  장갑  
	n_Type_Shoes		= 14,		// 14  부츠
	n_Type_LightCoat	= 21,		// 21  경장비 상의  
	n_Type_LightPants	= 22,		// 22		  하의  
	n_Type_LightGlove	= 23,		// 23		  장갑  
	n_Type_LightShoes	= 24,		// 24		  부츠
	n_Type_MiddleCoat	=  31,		// 31  중장비 상의  
	n_Type_MiddlePants	= 32,		// 32		  하의  
	n_Type_MiddleGlove	= 33,		// 33		  장갑  
	n_Type_MiddleShoes	= 34,		// 34		  부츠
	n_Type_HeavyCoat	= 41,		// 41  Heavy  상의  
	n_Type_HeavyPants	= 42,		// 42		  하의  
	n_Type_HeavyGlove	= 43,		// 43		  장갑  
	n_Type_HeavyShoes	= 44,		// 44		  부츠
	n_Type_MagicCoat	= 51,		// 51  마도복 상의  
	n_Type_MagicPants	= 52,		// 52		  하의  
	n_Type_MagicGlove	= 53,		// 53		  장갑  
	n_Type_MagicShoes	= 54,		// 54		  부츠
	n_Type_PriestCoat	= 61,		// 61  사제복 상의  
	n_Type_PriestPants	= 62,		// 62		  하의  
	n_Type_PriestGlove	= 63,		// 63		  장갑  
	n_Type_PriestShoes	= 64,		// 64		  부츠
	n_Type_SpiritCoat	= 71,		// 71  강신복 상의  
	n_Type_SpiritPants	= 72,		// 72		  하의  
	n_Type_SpiritGlove	= 73,		// 73		  장갑  
	n_Type_SpiritShoes	= 74,		// 74		  부츠

	n_Type_Shield		= 100,		// 100 방패
	n_Type_Arrow		= 101,		// 101 화살
	n_Type_Trap			= 102,		// 102 트랩
	n_Type_Eld			= 103,		// 103 엘드
*/
	

class CInventoryWnd : public CWndProc
{
public:
	
	BOOL			m_IsSendSetItemEquip;
	long			m_SendSetItmCheckTime;
	
	// 패시브 스킬 코드 
	WORD			theSkillCode_BasicCure;				// 기초 치료 
	WORD			theSkillCode_MasicPharmacy;			// 마법약 지식 

	WORD			theSkillCode_Sword;					// 검 
	WORD			theSkillCode_TwohandSword;			// 대검 
	WORD			theSkillCode_Bow;					// 활 
	WORD			theSkillCode_Shield;				// 방패 
	WORD			theSkillCode_MagicBook;				// 마법서 
	WORD			theSkillCode_Staff;					// 지팡이 
	WORD			theSkillCode_CrystalBall;			// 수정구 

	WORD			theSkillCode_LightArmor;			// 경장비 
	WORD			theSkillCode_MiddleArmor;			// 중장비 
	WORD			theSkillCode_HeavyArmor;			// 헤비장비 
	WORD			theSkillCode_ServeArmor;			// 서브장비 
	WORD			theSkillCode_MasicArmor;			// 마도복 
	WORD			theSkillCode_PriestArmor;			// 사제복 
	WORD			theSkillCode_SpiritArmor;			// 강신복

    //by simwoosung
	WORD			theSkillCode_CitizenBless;          //가호 효과
	
	
	bool			CheckWearPassiveSkill( SItemBaseInfo* aItemInfo );		// 장비 착용을 패시브 스킬을 익혔는지 검사한다. 
	bool			CheckUseItemPassiveSkill( SItemBaseInfo* aItemInfo );	// 아이템 사용을 위한 패시브 스킬을 익혔는지 검사한다. 

	// by dongs
	bool			bHorseInvenImg;

public:
	CFrameWnd*	FrameWnd;
#ifdef WIDE_PAY_INVENTORY
	CFrameWnd*	PayFrameWnd; 
#endif 
	CImageBox*	WearBgRain;
	CImageBox*	WearBgMillena;

	CImageBox*	m_BgSlotInven;
	CImageBox*	m_BgSlotInvenHorse;
#ifdef WIDE_PAY_INVENTORY
	CStateButton*	Statebtn; 
	CStateButton*	Statebtn1;
	CStateButton*	Statebtn2;
	CStateButton*	Statebtn3;
	CStateButton*	Statebtn4;
#endif
	CInventoryWnd()
	{
		InvenState		= n_INVEN_ITEM;
		TabType			= n_InvenTab;
		StartSlotNum	= 0;
		m_BgSlotInven	= NULL;
		WearBgRain		= NULL;
		WearBgMillena	= NULL;
		m_BgSlotInvenHorse = NULL;

		m_IsSendSetItemEquip = FALSE;
		m_SendSetItmCheckTime = 0;
#ifdef WIDE_PAY_INVENTORY
		m_PayInvenState = n_PAYINVEN_SHOP1; // 유료인벤확장	
		PayStartSlotNum = 0; // 유료인벤확장	
#endif
	}
	~CInventoryWnd(){}

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );
	virtual void SetWheelValue( short _wheel );

	void ResetWearSlotHighlight();
	
	void UpdateInvenSlotItem();//프로토콜 처리때문에....Slot에 변경이 있을때....
	void UpdateWearSlotItem();
	

	void CloseNumberInputWnd( int aNumItem ); 

	int GetStartSlotNum();
	int GetBlankSlotNum();
	int GetFirstSlotNum();
	
	void SetEldText( char* aEldText, DWORD aEld );
	void SetEldText( void* aEldText );
	
	void SetSrcSlot(CSlot* Src);
	void SetTargetSlot(CSlot* Target);
	BYTE GetTabType()		{ return TabType; }

	//dongs
	void InvenPosChange(bool bRide);
	void SetSlotPosition(int SlotNum);
	int GetSlotPosition(){return m_SlotPosition;}
	BOOL IsEmptyWearSlot();
	void FindDeleteItem(DWORD ItemUID);
	void GetInvenSlot( DWORD nSlot);
	void SetPayInvenPcItem( BYTE aItemNum, URpdItem aItemList[] );
#ifdef WIDE_PAY_INVENTORY	
	
	void Open();
#endif
	
private:
	
	N_INVENSTATE	InvenState;
	BYTE			TabType;
	int				StartSlotNum;
#ifdef WIDE_PAY_INVENTORY
	N_PAYINVENSTATE	m_PayInvenState;
	int				PayStartSlotNum; //유료인벤확장
#endif
	
	// 소모품 아이템 개수 입력시 필요 
	CSlot*				m_SrcSlot;			
	CSlot*				m_TargetSlot;


	CTextBox*	MoneyTxt;
	CScrollBar* ScrollBar;
	
	CSlot*	WearSlot[RP_MAX_EQUIP];
	CSlot*	InvenSlot[25];
#ifdef WIDE_PAY_INVENTORY	
	CSlot*	m_ItemSlot[25]; //유료인벤확장 슬롯
#endif
	//by simwoosung
	CButton* m_pPopC_ItemBtn;				//구매 아이템 가져오기 버튼 
	
	SPcDataParam*	PcParam;
	SPcDataInven*	PcInven;

	//by dongs Inven현재위치 
	int m_SlotPosition ;

	void SetInvenState( N_INVENSTATE state );//
	void SetSlotScroll( int linenum );//
	int	GetWearIndex( BYTE wearclass, BYTE wearType = 0 );
#ifdef WIDE_PAY_INVENTORY
	void SetPayInvenState( N_PAYINVENSTATE state );	
#endif
	void WearSlotClick( SEventMessage* EMsg );
	void WearSlotDBClick( SEventMessage* EMsg );
	void WearSlotDrag( SEventMessage* EMsg );
	void WearSlotDrop( SEventMessage* EMsg );

	void InvenBgSlotClick( SEventMessage* EMsg );
	
	void InvenSlotFocus( SEventMessage* EMsg );
	void InvenSlotClick( SEventMessage* EMsg );
	void InvenSlotDBClick( SEventMessage* EMsg );
	void InvenSlotDrag( SEventMessage* EMsg );
	void InvenSlotDrop( SEventMessage* EMsg );


	void ClickQuickSlot();
	void ClickInventory( CSlot* cslot, CSlot* pslot );
	void ClickWear( CSlot* cslot, CSlot* pslot );
	void ClickExchange( CSlot* cslot, CSlot* tslot );
	void ClickStore( CSlot* slot );
	void ClickBank( CSlot* slot );
#ifdef DECO_RENEWAL_GUILD_MJH
	void ClickGuild( CSlot* slot );
#endif // DECO_RENEWAL_GUILD_MJH

	void ClickTuning();
	void ClickSkill();
	void ClickPayItemWnd( CSlot* slot );
	void ClickPetInven(CSlot* slot,CSlot * tslot);
	

	void DropQuickSlot( CSlot* cslot, CSlot* tslot );
	void DropInventory( CSlot* cslot, CSlot* tslot );
	void DropPetInventory(CSlot* slot , CSlot* tslot);

	void DropWear( CSlot* cslot, CSlot* tslot );
	void DropExchange( CSlot* cslot, CSlot* tslot );
	void DropStore( CSlot* slot );
	void DropBank( CSlot* slot ); 
#ifdef DECO_RENEWAL_GUILD_MJH
	void DropGuild( CSlot* slot );
#endif // DECO_RENEWAL_GUILD_MJH
	void DropTuning();
	void DropSkill();
	void DropPayItemWnd( CSlot* slot );
	
	
	bool CompareSlot( CSlot* cslot, CSlot* tslot );
	
	// 월드에서 아이템을 습득 했을경우 처리
};


enum N_ELDINPUTSTATE
{
	n_ELDINPUT_INVENTORY_MONEYTHROW = 0,			// 인벤창에서 Eld를 버린다 
	n_ELDINPUT_INVENTORY_EXCHANGE,					// 인벤창에서 교환할 Eld 입력 
	n_ELDINPUT_INVENTORY_WAREHOUSE,					// 인벤창에서 창고에 저장할 Eld 입력  
	n_ELDINPUT_EXCHANGE_DRAWOUT, 					// 교환창에서 교환할 돈을 차감한다 
	n_ELDINPUT_WAREHOUSE_DRAWOUT, 					// 보관창에서 돈을 인출한다
	n_ELDINPUT_FRONTIER_PAYMENT,					// 길드창에서 회비를 납부한다. 
	n_ELDINPUT_FRONTIER_ELDSHARE,					// 길드창에서 엘드를 분배한다. 
	n_ELDINPUT_FRONTIER_PRESENT                     // 초대창에서 엘드 선물
	
};


class CMoneyThrowWnd : public CWndProc
{
public:
	CFrameWnd*			m_FrameWnd;
	CEditBox*			m_EldInput;
	CTextBox*			m_UseInfo;

	N_ELDINPUTSTATE		m_InputState;

public:
	CMoneyThrowWnd(){}
	~CMoneyThrowWnd(){}

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );


	void Open( N_ELDINPUTSTATE aInputState );	
	void OkClose();
	void CancelClose();
	void DeleteComma(char *Str);


};


extern	CInventoryWnd	InventoryWnd;
extern	CMoneyThrowWnd	g_MoneyThrowWnd;

//-----------------------------------------------------------------------------
#endif  __RUI_INVENTORY_WNDPROC_H__