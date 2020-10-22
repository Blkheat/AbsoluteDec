//----------------------------------------------------------------------------------------------------------------------
// 파  일   명 : ItemEldaMixAWndProc.h
// 용       도 : 합성 엘다스톤 합성창
//----------------------------------------------------------------------------------------------------------------------
#if !defined( __RUI_ELDA_MIXA_WNDPROC_H__ )
#define __RUI_ELDA_MIXA_WNDPROC_H__
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
#include "ImageAni.h"

#include <vector>
#include "WndProc.h"

const int c_MiX_ELDA_ITEM_NUM  = 6;
const int c_MAX_MIXELDA_PAY = 3;


#pragma pack(push,1)
struct _SEldaItemInfo
{
	
	BYTE	Grade;
	DWORD	NeedMoney;
	WORD	SucRate;

	_SEldaItemInfo()
	{
		Grade =0;
		NeedMoney = 0 ;
		SucRate = 0 ; 
	}
};


struct sDataEldaStone
{
	int Code;
	int Grade;
	int Color;
};


#pragma pack(pop)


const int SLOT_MAX = 6;


class CItemEldaMixAWndProc : public CWndProc
{
public:
	CItemEldaMixAWndProc();
	~CItemEldaMixAWndProc();

	
	enum ELDA_VARIEYT
	{
			ELDA_RAW = 0 ,	//원석 
			ELDA_GREEN,		//녹색 
			ELDA_BLUE,		//푸른색
			ELDA_YELLOW,	//노란색 
			ELDA_RED,		// 빨강
			ELDA_PURPLE,	//보라 
			ELDA_VIOLETE,	//자주색 
			ELDA_NAVY,		//남색
			ELDA_SILVER,	//은색 
			ELDA_YELLOW_GREEN,	//연두색 
			ELDA_ORANGE,	//주황 
	};

	enum CITIZEN_ITEM 
	{
		MILLENA_WEPON,
		MILLENA_ARMOR,
		RAIN_WEPON,
		RAIN_ARMOR,
	};

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );	
	
	void Close();
	void Open(int State);

	int GetItemCode(int Grade , int Color);
	int GetWindowsState(){return m_iWindowState;};

	void OpenCharge_Wnd();
	void CloseCharge_Wnd();

private:

	BOOL CheckItemList();
	void ItemSlotClick( SEventMessage* EMsg );
	void ItemSlotDBClick( SEventMessage* EMsg );
	void CheckItemAdd(SPcItem* PrePcItem ,SPcItem* CurPcItem,SPcItem* SrcPcItem , SPcItem *TarGetPcItem);
	
	BOOL CreateLoadTable();
	
	void Unlock_button(){mb_button_lock = false;};
	bool GetButtonLock(){return mb_button_lock;}

	//아이템을 미리 보여준다.
	void ClearPreItem();
	
	BOOL SetAddPcItem(CSlot *Slot , int Code ,BOOL flag = false);
	
	
	int GetMakeCost(int Grade);

	void PrevItemList(CSlot *Slot);
	void GetEldStr(char *Str , DWORD Eld);
	
	CITIZEN_ITEM	m_eCitizen_Item;

	SPcItem			m_CurPcItem[c_MiX_ELDA_ITEM_NUM];
	SPcItem			m_PrePcItem[c_MiX_ELDA_ITEM_NUM];
	CSlot*			m_CurSlot[c_MiX_ELDA_ITEM_NUM];
	CSlot*			m_PreSlot[c_MiX_ELDA_ITEM_NUM];
	
	

	CFrameWnd*		m_pFrameWnd;
	CFrameWnd*		m_pFrameWndBase;

	CImageBox*		m_PreItemImg[6];
	
	CButton*		start_btn;	
	CButton*		cancel_btn;
	CTextBox*		m_MoneyText;

	int	m_iWindowState;
	
	int	m_BaseEldaInfo[4][6];
	bool mb_button_lock;		///-- 버튼 Lock 가공 버튼 여러번 누름 방지
	SBundleItem EldaStone [6];	
	
	std::vector<sDataEldaStone> m_List;
	std::vector<_SEldaItemInfo> m_EldaCostList;

	int m_CurEldaLevel;
	

	///----------------------------------------------------------------------------------
	/// 유료 아이템 관련 창
	///----------------------------------------------------------------------------------
	
	CButton*		mcp_chargeWindow_btn;			///-- 유료 윈도우 창 버튼
	BOOL			m_IsViewchargeWindow;			///-- 유료 아이템 창 보여주기 여부 	
	CFrameWnd*		mpCharge_framewnd;				///-- 유료 윈도우 창
	
	///-- 유료 아이템 등록 
	//유료아이템 
	SPcItem			mc_Chargeslot_item[3];
	CSlot*			mcp_Chargeitem_slot[3];
	
	DWORD	m_dPayItem[3];


};




extern	CItemEldaMixAWndProc	g_ItemEldaMixAWndProc;
//-----------------------------------------------------------------------------
#endif  __RUI_ELDA_MIXA_WNDPROC_H__