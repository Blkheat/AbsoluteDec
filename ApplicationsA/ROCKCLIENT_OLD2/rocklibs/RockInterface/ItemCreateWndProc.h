#if !defined( __RUI_ITEMDREATE_WNDPROC_H__ )
#define __RUI_ITEMDREATE_WNDPROC_H__ 
//-----------------------------------------------------------------------------
#include "FrameWnd.h"
#include "Button.h"
#include "ImageBox.h"
#include "TextBox.h"
#include "EditBox.h"
#include "ProgressBar.h"
#include "SlideBar.h"
#include "CheckButton.h"
#include "RadioButton.h"
#include "ListBox.h"
#include "ScrollBar.h"
#include "ComboBox.h"
#include "Slot.h"
#include "Wnd.h"


#include "WndProc.h"


class CItemCreateWndProc : public CWndProc
{
public:
	CItemCreateWndProc();  
	~CItemCreateWndProc(); 

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );
	
   
	void OpenCreate_Wnd(  NCraftType ae_type, BYTE aB_citizen );
	void CloseCreate_Wnd();

	void CheckCreate();
	BOOL LoadCreateTable();
	///- 인벤이 변화될때 아이템 체크하는 함수
	BOOL CheckCreate_ItemList();

	
	void CloseNumberInputWnd( int aNumItem );
	void Unlock_button(){mb_button_lock = false;};
	void Change_make_type(CSlot *pSlot = NULL);
	bool Get_Craft_Weapon(){ return mb_craft_weapon;};
	
	int	 GetTableCount(void)	{ return mi_table_count;}

	bool IsMakeItemSubJob(CSlot *pSlot);
	bool IsMakeItemSubJob(BYTE aType);
	
	CSlot* GetTarGetSlot()	{ return mcp_target_slot; }

	//by simwoosung
	void ViewComponentItemList();
	void ClearComponentItemList();
	
private:
	
	void GetProbability();
	void ItemSlotClick( SEventMessage* EMsg );
	void ItemSlotDBClick( SEventMessage* EMsg );

	CFrameWnd*		m_pWholeFrame;							///-- 전체 프레임			
	
	CFrameWnd*		mpC_framewnd;

	///-- 아이템 강화를 위해 준비할 아이템을 미리보여줌
	SPcItem			mc_preitemslot_item[5];
	CSlot*			mcp_preitem_slot[5];	
	
	SPcItem			mc_itemslot_item[5];
	SPcItem			mc_eldaslot_item;
	SPcItem			mc_target_item;

	CSlot*			mcp_item_slot[5];
	CSlot*			mcp_elda_slot;
	CSlot*			mcp_temp_num_src_slot;
	CSlot*			mcp_temp_num_tga_slot;
	CSlot*			mcp_target_slot;
	
	CImageBox*		mcp_item_type;

	CTextBox*		mcp_create_type;
	CTextBox*		mcp_Item;
	///CTextBox*		mcp_create_item;	
	CButton*		mcp_create_btn;	
	CButton*		mcp_change_type;	

	
	bool mb_loadCreatelTable;
	int mi_table_count;

	bool mb_button_lock;		///-- 버튼 Lock 가공 버튼 여러번 누름 방지
	bool mb_craft_weapon;		///-- 웨폰 / 아머 크래프트  판단.
	int mi_skill_level;			///-- 해당 스킬의 최고 레벨
	int mi_make_type;			///-- 제조 타입 비교하기 위한 값이다.
	///-- 밀레나 /웨폰  : 검 대검 활
	///-- 밀레나 /아머  : 경장비 중장비 헤비장비 방패 
	///-- 레인  /웨폰   : 마법서 수정구 스태프
	///-- 레인  /아머	: 마도복 강신복 사제복 
	int mi_item_type;			///-- 제조 아이템의 타입./ 실제 Table 에 명시되어 있는 값

	//by simwoosung
	int m_nIndexiTable;			///-- 선택된 제조 아이템 테이블의 인덱스

	CButton*		mcp_chargeWindow_btn;			///-- 유료 윈도우 창 버튼
	bool			m_IsViewchargeWindow;			///-- 유료 아이템 창 보여주기 여부 
	
	///----------------------------------------------------------------------------------
	/// 유료 아이템 관련 창
	///----------------------------------------------------------------------------------
	
	CFrameWnd*		mpCharge_framewnd;				///-- 유료 윈도우 창
	
	///-- 유료 아이템 등록 
	SPcItem			mc_Chargeslot_item[3];
	CSlot*			mcp_Chargeitem_slot[3];

	DWORD	dPayItem[3];
public:
	void OpenCharge_Wnd();
	void CloseCharge_Wnd();
};



extern	CItemCreateWndProc	g_ItemCreate_WndProc;
extern SMaKeItemTable * g_CreatelTable;
//-----------------------------------------------------------------------------
#endif  __RUI_ITEMDREATE_WNDPROC_H__ 