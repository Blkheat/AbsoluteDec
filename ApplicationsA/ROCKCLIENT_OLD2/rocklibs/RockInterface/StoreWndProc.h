#if !defined( __RUI_STORE_WNDPROC_H__ )
#define __RUI_STORE_WNDPROC_H__
//-----------------------------------------------------------------------------

#include "..\\..\\RockClient.h"
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

#define MAX_STORE_ITEM	60

#ifdef CHINA_VERSION
	#define SELLSTOR_RATIO 0.3f
#else
	#define SELLSTOR_RATIO 0.2f
#endif

const int c_MaxStoreItemDrawNum( 25 );

class CStoreWnd : public CWndProc
{
public:
	CStoreWnd()
	{
		NumDealingItem = 0;
		DealingItem[0] = n_STORE_NONE;
		DealingItem[1] = n_STORE_NONE;
		CurDealingItem = n_STORE_NONE;;
	
		NumItemWeapon	= 0;
		NumItemArmor	= 0;
		NumItemSundries	= 0;
		NumItemUse		= 0;

		///-- 콜로니 관련
		NumItemPosition = 0;
		NumItemAtkArms	= 0;
		NumItemBuilding	= 0;
		NumItemGTower	= 0;
		NumItemEtc		= 0; 
				


		memset( ItemWeapon, NULL, sizeof( SPcItem* ) * MAX_STORE_ITEM );
		memset( ItemArmor, NULL, sizeof( SPcItem* ) * MAX_STORE_ITEM );
		memset( ItemSundries, NULL, sizeof( SPcItem* ) * MAX_STORE_ITEM );
		memset( ItemUse, NULL, sizeof( SPcItem* ) * MAX_STORE_ITEM );

		///-- 콜로니 관련
		memset( ItemPosition, NULL, sizeof( SPcItem* ) * MAX_STORE_ITEM );
		memset( ItemAtkArms, NULL, sizeof( SPcItem* ) * MAX_STORE_ITEM );
		memset( ItemBuilding, NULL, sizeof( SPcItem* ) * MAX_STORE_ITEM );
		memset( ItemGTower, NULL, sizeof( SPcItem* ) * MAX_STORE_ITEM );

		memset( ItemEtc , NULL , sizeof( SPcItem*) * MAX_STORE_ITEM );


		StartSlotNum	= 0;
	}
	~CStoreWnd()
	{
		for( int i = 0; i < MAX_STORE_ITEM; ++i )
		{
			SAFE_DELETE(ItemWeapon[i]);
			SAFE_DELETE(ItemArmor[i]);		
			SAFE_DELETE(ItemSundries[i]);		
			SAFE_DELETE(ItemUse[i]);

			///-- 콜로니 관련
			SAFE_DELETE(ItemPosition[i]);
			SAFE_DELETE(ItemAtkArms[i]);
			SAFE_DELETE(ItemBuilding[i]);
			SAFE_DELETE(ItemGTower[i]);
			SAFE_DELETE(ItemEtc[i]);
		}	
	}

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );
	virtual void SetWheelValue( short _wheel );

	void SetShopInfo( SStoreShop* _shop );
	

	CFrameWnd*		FrameWnd;
	SStoreShop*		Shop;
	DWORD			OwnerUID;
	BYTE			theDCRate;			// 아이템 할인률 
	
private:
	int				NumDealingItem;
	N_DEALINGITEM	DealingItem[2];//거래 품목
	N_DEALINGITEM	CurDealingItem;
	
	int				NumItemWeapon;
	int				NumItemArmor;
	int				NumItemSundries;
	int				NumItemUse;
	

	///-- 콜로니 관련
	int				NumItemPosition;
	int				NumItemAtkArms;
	int				NumItemBuilding;
	int				NumItemGTower;

	int				NumItemEtc;

	SPcItem*		ItemWeapon[MAX_STORE_ITEM];
	SPcItem*		ItemArmor[MAX_STORE_ITEM];
	SPcItem*		ItemSundries[MAX_STORE_ITEM];
	SPcItem*		ItemUse[MAX_STORE_ITEM];

	///-- 콜로니 관련
	SPcItem*		ItemPosition[MAX_STORE_ITEM];
	SPcItem*		ItemAtkArms[MAX_STORE_ITEM];
	SPcItem*		ItemBuilding[MAX_STORE_ITEM];
	SPcItem*		ItemGTower[MAX_STORE_ITEM];

	//봉인의성지 
	SPcItem*		ItemEtc[MAX_STORE_ITEM];


	int				StartSlotNum;

	CScrollBar*		ScrollBar;
	CSlot*			StoreSlot[50];
	CStateButton*	SBtn[2];

	SPcDataInven*	PcInven;
	

	void SetShopItem();
	N_DEALINGITEM GetDealingItem( int _dealing_item );

	void SetItemWeapon();
	void SetItemArmor();
	void SetItemSundries();
	void SetItemUse();

	///-- 콜로니 관련
	void SetItemPosition();
	void SetItemAtkArms();
	void SetItemBuilding();
	void SetItemGTower();

	///-- 봉인의 성지 
	void SetItemEtc();
	
	void SetStoreTab( N_DEALINGITEM _dealing_item );

	void SetSlotScroll( int linenum );

	void StoreWndClick( SEventMessage* EMsg );
	void StoreBgSlotClick( SEventMessage* EMsg );

	
	void StoreSlotClick( SEventMessage* EMsg );
	void StoreSlotDBClick( SEventMessage* EMsg );
	void StoreSlotDrag( SEventMessage* EMsg );
	void StoreSlotDrop( SEventMessage* EMsg );
	
	bool CompareSlot( CSlot* cslot, CSlot* tslot );
};

class CStoreBuyWnd : public CWndProc
{
public:
	CStoreBuyWnd(){}
	~CStoreBuyWnd(){}

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );
	

	void OkClose();
	void CancelClose();

	void SetBuyStoreOwner( DWORD _owner_uid );
	void SetBuyItemInfo( SPcItem* _pc_item );
	void CloseNumberInputWnd( SPcItem* _pc_item, int _num_item, BYTE _item_tab );


	CImageBox*	IconImg;  
	CTextBox*	NumTxt;
	CTextBox*	PriceTxt;

	SPcItem*	PcItem;
	int			NumItem;
	DWORD		OwnerUID;

	bool		IsOverNumItem;
	bool		IsOverMoney;
};

class CStoreSellWnd : public CWndProc
{
public:
	CStoreSellWnd(){}
	~CStoreSellWnd(){}

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );
	

	void OkClose();
	void CancelClose();

	void SetSellItemInfo( SPcItem* _pc_item, BYTE _item_tab );
	void CloseNumberInputWnd( SPcItem* _pc_item, int _num_item, BYTE _item_tab );


	CImageBox*	IconImg;
	CTextBox*	NumTxt;
	CTextBox*	PriceTxt;

	SPcItem*	PcItem;
	int			NumItem;

	BYTE		ItemTab;
};


extern	CStoreWnd		StoreWnd;
extern	CStoreBuyWnd	StoreBuyWnd;
extern	CStoreSellWnd	StoreSellWnd;
//-----------------------------------------------------------------------------
#endif  __RUI_STORE_WNDPROC_H__