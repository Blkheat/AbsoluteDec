#if !defined( __RUI_SKILLSELECTPOTION_WNDPROC_H__ )
#define __RUI_SKILLSELECTPOTION_WNDPROC_H__
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


typedef struct _RealData
{
	int Index;
	int Code; 
}RealData;

const int ITEMSLOT_WIDTH = 5;
const int ITEMSLOT_HEIGHT = 5 ;




//#ifdef C_SEALED_SKILLPOTION_RENEW
	const int ITEMSLOT_MAX  = 25;
	const int ITEM_PCITEM_MAX = 60;
//#else 
//	const int ITEMSLOT_MAX  = 15;
//	const int ITEM_PCITEM_MAX = 15;
//#endif 
const int POTION_START = 7111;  
const int POTION_END = 7115;

const int ELDA_STONE_START = 6211;
const int ELDA_STONE_END = 6310;

class CItemSelectPotionWndProc : public CWndProc
{
public:

	CItemSelectPotionWndProc();
	~CItemSelectPotionWndProc();

	/*
	n_SkillPotion				= 1, //�α� 1 : �������
	n_Ruined_Interpid_Item		= 2, //�α� 2 : �ĸ��� ��� - ��� 
	n_Ruined_Wrath_item			= 3, //�α� 3 : �ĸ��� ��� - �г�
	n_Sealed_Interpid_Item		= 4, //�α� 4 : ������ ��� - ���
	n_Sealed_Wrath_item			= 5, //�α� 5 : ������ ��� - �г�
	n_Eldastone_Powder			= 6, //�α� 6 : ���ٽ��� ���� ��ȯ 
	*/

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );	
	virtual void SetWheelValue( short _wheel );

	void Close();
	void Open(NSkillPotionType Set);
	void Unlock_button(){mb_button_lock = false;};
	bool GetButtonLock(){return mb_button_lock;}
	BOOL SetAddPcItem(SPcItem *PcItem , int Code , int ItemNum );
	BOOL CreateLoadTable();
	void ClearPreItem();
		
	CFrameWnd*		m_pFrameWnd;
	SPcItem			m_CurPcItem[ITEM_PCITEM_MAX];
	CSlot*			m_CurSlot[ITEMSLOT_MAX];
	int				m_iTableCount;

	SSkillPotionTable*		m_DataTable;
	
	std::vector<SSkillPotionTable>	   m_RealItem;	
	
	void SetMode(NSkillPotionType m_Set );
private:
	
	void ItemSlotClick( SEventMessage* EMsg );
	void ItemSlotDBClick( SEventMessage* EMsg );
	void EnchantText();
	
	void SetSlotScroll( int linenum );

	NSkillPotionType m_SetMode; 

	CSlot*			RusultItem;

	CButton*		start_btn;	
	CButton*		cancel_btn;
	CScrollBar*		ScrollBar; 

	
	int m_StartSlotNum;


	bool mb_button_lock;		///-- ��ư Lock ���� ��ư ������ ���� ����
	
};




extern	CItemSelectPotionWndProc	g_ItemSelectPotionWndProc;
//-----------------------------------------------------------------------------
#endif  __RUI_ITEMENCHANT_WNDPROC_H__