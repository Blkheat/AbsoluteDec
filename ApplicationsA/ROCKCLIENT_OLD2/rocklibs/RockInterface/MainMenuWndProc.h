#if !defined( __RUI_MAINMENU_WNDPROC_H__ )
#define __RUI_MAINMENU_WNDPROC_H__
//-----------------------------------------------------------------------------
#include "FrameWnd.h"
#include "Button.h"
#include "ImageBox.h"
#include "TextBox.h"
#include "EditBox.h"
#include "ProgressBar.h"
#include "UpGauge.h"
#include "SlideBar.h"
#include "CheckButton.h"
#include "RadioButton.h"
#include "StateButton.h"
#include "ListBox.h"
#include "ScrollBar.h"
#include "ComboBox.h"
#include "Slot.h"
#include "Wnd.h"
#include "ItemGetButton.h"

#include "WndProc.h"


#include "..\\..\\Bravolibs\\Obj\\SkillSystem.h"


const int c_MaxMainMenuItemDrawNum( 10 );
#ifdef EXPANSION_QUICK_SLOT
const int c_MaxMainMenuExtenItemDrawNum( 20 );
#else
const int c_MaxMainMenuExtenItemDrawNum( 10 );
#endif

#ifdef TAIWAN30MINEVENT
enum N_EVENTSTATE
{
	n_EVENT_OPEN = 0,
	n_EVENT_PLAY, 
	n_EVENT_CLOSE, 

};

enum N_CEVENTSTATE
{
	n_eFail = 0,
	n_eSuccess ,
	n_ePlaying ,
	n_eComplete ,
};
#endif //TAIWAN30MINEVENT

///---------------------------------------------------------------------------
///-- CMainMenuWnd
///---------------------------------------------------------------------------
class CMainMenuWnd : public CWndProc
{
public:
	CMainMenuWnd()
	{
		SlotLine		= 0;
		StartSlotNum	= 0;
	}	
	~CMainMenuWnd(){}

	virtual void Init();
	void InitOld();
	void InitNew();
	virtual void Composition();
	void CompositionOld();
	void CompositionNew();
	virtual void Update();
	void UpdateOld();
	void UpdateNew();
	virtual void Proc( SEventMessage* EMsg );
	void ProcOld( SEventMessage* EMsg );
	void ProcNew( SEventMessage* EMsg );	
	
	
	void UseFunctionKey( N_FUNCTIONKEY fk );// 퀵슬롯 키 입력
	void UseFunctionKeyOld( N_FUNCTIONKEY fk );
	void UseFunctionKeyNew( N_FUNCTIONKEY fk );
	void UpdateQuickSlotItem();//프로토콜 처리때문에....Slot에 변경이 있을때....

	void SetSkillSystem( CSkillSystem* _skillsystem );
	int GetStartSlotNum();

	void SetSkillDelayTime( WORD _skillcode );
	
	void UnLinkQuickSlot( CSlot* slot );

	CSlot **GetQSlotList() { return QSlot; }
	CSlot **GetEQSlotList() { return EQSlot; }

	
	BOOL	CheckItemState(SPcItem* PcItem);
	BOOL	UseItem(CSlot* slot);

	
	void LogOff();


private:

	int		StartSlotNum;
	CSlot*	QSlot[10];

	CButton * theQExtenButton;				///-- 퀵슬롯 확장버튼

	///-- 확장 퀵슬롯 프레임구성
	CFrameWnd * theQExFrame;
	CFrameWnd * theQExFrame2;
	CSlot*	EQSlot[20];
	int iExBtnCount; 
	CTextBox *ExpPercent;
	CTextBox *pSlotLineTxt;


	SPcDataParam* 	thePcParam;
	

	SPcDataInven*	PcInven;
	CSkillSystem*	SkillSystem;

	void QuickSlotClick( SEventMessage* EMsg );
	void QuickSlotDBClick( SEventMessage* EMsg );
	void QuickSlotDrag( SEventMessage* EMsg );
	void QuickSlotDrop( SEventMessage* EMsg );

	bool CompareSlot( CSlot* cslot, CSlot* tslot );		


public:

	int		SlotLine;


	void QuickSlotLineChange();

};


///---------------------------------------------------------------------------
///-- CMainEXPWnd
///---------------------------------------------------------------------------
class CMainEXPWnd: public CWndProc
{
protected:
	CProgressBar*	ExpGauge;	
	SPcDataParam* 	thePcParam;
	CTextBox*		ExpText;
	CTextBox*		ExpPercent;
public:
	virtual void Composition();
	void CompositionOld();
	void CompositionNew();
	virtual void Update();
	void UpdateOld();
	void UpdateNew();
	virtual void Init();
};

///---------------------------------------------------------------------------
///-- CMainButtonWnd
// 화면 우하단 버튼들
///---------------------------------------------------------------------------
class CMainButtonWnd: public CWndProc
{
public:	
	CButton*		mapD_Buttons[11];

#ifdef TAIWAN30MINEVENT
	DWORD m_CurTime;
	DWORD m_NowTime;
	DWORD m_Cumulative;
	DWORD m_MilliSec;
	DWORD AllTime;
	int BaseTime;
	bool OnTwinkle;
	CButton*		 event_Buttons;
	CTextBox*        event_Text;
	N_EVENTSTATE    e_State;
	TCHAR str[258];
#endif //TAIWAN30MINEVENT

protected:
	CImageBox*		mapD_BtnImage[11];	
	CItemTransButton* InVenButton;
	CItemTransButton* QuestButton;
		
public:	
	virtual void Composition();
	void CompositionOld();
	void CompositionNew();
#ifdef TAIWAN30MINEVENT

	void GetSignalStart( BYTE eStart);
	void GetSignalEnd( BYTE eEnd);
	
	virtual void Update();
	void UpdateOld();
	void UpdateNew();
	virtual void Init();

#endif //TAIWAN30MINEVENT
	
	virtual void Proc( SEventMessage* EMsg );
	void LogOff();
	CItemTransButton* GetInVenButton() { return InVenButton; }
	CItemTransButton* GetQuestButton() { return QuestButton; }
};

///---------------------------------------------------------------------------
///-- CMainEnduranceWarning
///---------------------------------------------------------------------------
class CMainEnduranceWarning: public CWndProc
{
protected:
	int mi_index;
	
public:	
	CFrameWnd*		FrameWnd;
	virtual void Composition();
	CSlot*	mcp_slots[RP_MAX_EQUIP];
	//virtual void Proc( SEventMessage* EMsg );
	//void LogOff();
	virtual void Update();
};

//
///---------------------------------------------------------------------------
///-- CMainMenuLVgageWnd
///---------------------------------------------------------------------------
/*class CMainMenuLVgageWnd : public CWndProc
{
protected:
	UINT			m_WIDToolTipDestWnd;
	float			m_fCumulTime;
	float			m_fRatio;
	CFontg*			theFontg;
	
public:
	CMainMenuLVgageWnd()
	{
		PcParam	= NULL;
		m_WIDToolTipDestWnd = WID_None;
		m_fCumulTime = 0.0f;
		m_fRatio = 0.0f;

		theFontg = NULL;
	}	
	~CMainMenuLVgageWnd(){}

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );
	
	void InitLvGauge();
	
	void SetToolTipDestWnd( UINT WIDToolTipDestWnd );
	void RenderToolTipName();

	SPcDataParam*	PcParam;

	CUpGauge*	UpGage;
	CImageBox*	HighlightImg;	
	CButton	   *pStateOpenBtn;
};
extern	CMainMenuLVgageWnd	MainMenuLVgageWnd;
*/



extern	CMainButtonWnd		g_main_buttons;
extern	CMainEXPWnd			g_Exp_wnd;
extern	CMainMenuWnd		MainMenuWnd;
extern	CMainEnduranceWarning		g_endu_warning;
//-----------------------------------------------------------------------------
#endif  __RUI_MAINMENU_WNDPROC_H__