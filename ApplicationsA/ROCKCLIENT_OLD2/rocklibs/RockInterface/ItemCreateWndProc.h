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
	///- �κ��� ��ȭ�ɶ� ������ üũ�ϴ� �Լ�
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

	CFrameWnd*		m_pWholeFrame;							///-- ��ü ������			
	
	CFrameWnd*		mpC_framewnd;

	///-- ������ ��ȭ�� ���� �غ��� �������� �̸�������
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

	bool mb_button_lock;		///-- ��ư Lock ���� ��ư ������ ���� ����
	bool mb_craft_weapon;		///-- ���� / �Ƹ� ũ����Ʈ  �Ǵ�.
	int mi_skill_level;			///-- �ش� ��ų�� �ְ� ����
	int mi_make_type;			///-- ���� Ÿ�� ���ϱ� ���� ���̴�.
	///-- �з��� /����  : �� ��� Ȱ
	///-- �з��� /�Ƹ�  : ����� ����� ������ ���� 
	///-- ����  /����   : ������ ������ ������
	///-- ����  /�Ƹ�	: ������ ���ź� ������ 
	int mi_item_type;			///-- ���� �������� Ÿ��./ ���� Table �� ��õǾ� �ִ� ��

	//by simwoosung
	int m_nIndexiTable;			///-- ���õ� ���� ������ ���̺��� �ε���

	CButton*		mcp_chargeWindow_btn;			///-- ���� ������ â ��ư
	bool			m_IsViewchargeWindow;			///-- ���� ������ â �����ֱ� ���� 
	
	///----------------------------------------------------------------------------------
	/// ���� ������ ���� â
	///----------------------------------------------------------------------------------
	
	CFrameWnd*		mpCharge_framewnd;				///-- ���� ������ â
	
	///-- ���� ������ ��� 
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