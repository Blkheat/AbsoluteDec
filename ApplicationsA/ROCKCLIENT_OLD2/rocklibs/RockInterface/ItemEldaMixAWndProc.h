//----------------------------------------------------------------------------------------------------------------------
// ��  ��   �� : ItemEldaMixAWndProc.h
// ��       �� : �ռ� ���ٽ��� �ռ�â
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
			ELDA_RAW = 0 ,	//���� 
			ELDA_GREEN,		//��� 
			ELDA_BLUE,		//Ǫ����
			ELDA_YELLOW,	//����� 
			ELDA_RED,		// ����
			ELDA_PURPLE,	//���� 
			ELDA_VIOLETE,	//���ֻ� 
			ELDA_NAVY,		//����
			ELDA_SILVER,	//���� 
			ELDA_YELLOW_GREEN,	//���λ� 
			ELDA_ORANGE,	//��Ȳ 
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

	//�������� �̸� �����ش�.
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
	bool mb_button_lock;		///-- ��ư Lock ���� ��ư ������ ���� ����
	SBundleItem EldaStone [6];	
	
	std::vector<sDataEldaStone> m_List;
	std::vector<_SEldaItemInfo> m_EldaCostList;

	int m_CurEldaLevel;
	

	///----------------------------------------------------------------------------------
	/// ���� ������ ���� â
	///----------------------------------------------------------------------------------
	
	CButton*		mcp_chargeWindow_btn;			///-- ���� ������ â ��ư
	BOOL			m_IsViewchargeWindow;			///-- ���� ������ â �����ֱ� ���� 	
	CFrameWnd*		mpCharge_framewnd;				///-- ���� ������ â
	
	///-- ���� ������ ��� 
	//��������� 
	SPcItem			mc_Chargeslot_item[3];
	CSlot*			mcp_Chargeitem_slot[3];
	
	DWORD	m_dPayItem[3];


};




extern	CItemEldaMixAWndProc	g_ItemEldaMixAWndProc;
//-----------------------------------------------------------------------------
#endif  __RUI_ELDA_MIXA_WNDPROC_H__