#ifndef __CHATPARTWNDLIST_H__
#define __CHATPARTWNDLIST_H__
//-----------------------------------------------------------------------------

#include "TextOutputBase.h"
#include "WndProc.h"

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
#include "Wnd.h"
#include "CTextOutList.h"
#include "ChatEditBox.h"

//----------------------------------------------------------------------------
// CChatActPrintWnd 채팅대화 활성화 출력창
//----------------------------------------------------------------------------
class CChatActPrintWnd : public CWndProc
{
public:
	SPoint2				  m_nLeftDownPos;		//기준이 되는 왼쪽아래위치
	UINT				  m_uSizeState;	

protected:
	long				  m_lPrevtime;

	UINT				  m_uFilterState;
	UINT				  m_uChatState;	
	
	//활성 채팅출력창 - 전체프레임
	CFrameWnd *			  m_pActChatBgWnd;			
	
	//상단 필터링 탭 프레임
	CFrameWnd *			  m_pFilterTabWnd;
	CStateButton*		  m_pWholeViewTab;
	CStateButton*		  m_pPartyViewTab;
	CStateButton*		  m_pGuildViewTab;
	CStateButton*		  m_pAnnounViewTab;
	CStateButton*		  m_pHelpViewTab;
	
	//중단 채팅출력 프레임
	CFrameWnd *			  m_pChatPrintWnd;
	CChatOutList *		  m_pChatOutList;
	CWnd*				  m_pChatScrollBarBg;	
	CScrollBar*			  m_pChatScrollBar;	
	CEditBox *			  m_pInputFWhisDestBox;			
	CTextBox *			  m_pTitleTexBox;
	
	
	TCHAR				  m_strTitle[256];
	
	//하단 채팅입력, 컨트롤 프레임
	CFrameWnd *			  m_pChatInputWnd;
	CButton*			  m_pChatStateMenuViewBtn;
	CButton*			  m_pChatSizeContBtn;
	CChatEditBox*		  m_pInputEditBox;	   //입력 에디트 박스	
	
	//채팅상태 메뉴 윈도우
	CFrameWnd *			  m_pChatStateMenuWnd;
	CButton*			  m_pNormalChatBtn;
	CButton*			  m_pGuildChatBtn;
	CButton*			  m_pPartyChatBtn;
	CButton*			  m_pWhisperChatBtn;
	CButton*			  m_pWholeChannelBtn;
	CButton*			  m_pWholeServerBtn;
	CButton*			  m_pWholeAreaBtn;
	

	TCHAR				  m_strChatstring[256];
	TCHAR				  m_strOldChatstring[256];		//이전 채팅문자열
	long				  m_lPrevTime;					//이전 채팅입력 시간	

	TCHAR				  m_strCmd[256];
	TCHAR				  m_strParam[256];
	TCHAR				  m_strFWhisDest[256];
	TCHAR				  m_strBWhisDest[256];

	bool				  m_IsInsertAnnounce;	

	//대화명 클릭시 팝업 
	CFrameWnd *			  m_pChatIDPopupWnd;
	CButton*			  m_pChatIDPopupBtn1;
	CButton*			  m_pChatIDPopupBtn2;
	CButton*			  m_pChatIDPopupBtn3;
	CButton*			  m_pChatIDPopupBtn4;

	TCHAR				  m_strIDName[32];
	BOOL				  m_IsChatIDPopupVisible;



protected:
	void SlashChatProc();
	bool SlashExcuteProc();
	void ChattingProc();

	BOOL CheckFocusWnd(WNDID Wid);
public:
	bool				  m_IsOpenInstant;		//처음 열리는 순간
	DWORD				  m_dUseShoutCode;		//사용되는 외치기 아이템 코드
	
	CChatActPrintWnd();
	virtual ~CChatActPrintWnd();

	virtual void Init();
    virtual void Composition();
	void CompositionOld();
	void CompositionNew();
    virtual void Update();
    virtual void Proc( SEventMessage* EMsg );
	virtual void SetWheelValue( short _wheel );

public:
	void SetSizeState(UINT uState);
	void SetVisible(bool bFlag);
	void SetFilterTabState(UINT uState);
	void SetChatState(UINT uState);

	bool Send_CsShout(NScrollType ScrollType);
	

	UINT GetChatState() { return m_uChatState; }
	UINT GetFilterState() { return m_uFilterState; }		
	CChatOutList * GetChatOutList()	{ return m_pChatOutList; }
	CChatEditBox * GetChatEditBox()	 { return m_pInputEditBox; }

	void SetListScroll( int _linenum );
	CFrameWnd* GetFrameWnd()  { return m_pActChatBgWnd;	}

	bool SetWheelValue(SMouse & theMouse);
	void ResetScroll();
	void SetWhisDest(void *strDest);
	void SetInsertAnnounce(bool bFlag);
	void RePosition();	

public:
	void ExcuteInvitePatry(TCHAR *strParam, bool bLeaueFlag = false);
};

//----------------------------------------------------------------------------
// CChatNonActPrintWnd 채팅대화 비활성화 출력창
//----------------------------------------------------------------------------
class CChatNonActPrintWnd : public CWndProc
{
public:
	SPoint2				  m_nLeftDownPos;		//기준이 되는 왼쪽아래위치

protected:
	//비활성 채팅출력창 - 전체프레임
	CFrameWnd *			  m_pNonActChatBgWnd;
	CChatOutList *		  m_pChatOutList;
	CWnd*				  m_pChatScrollBarBg;	
	CScrollBar*			  m_pChatScrollBar;
	
	bool				  m_bEnableWeel;

public:
	CChatNonActPrintWnd();
	virtual ~CChatNonActPrintWnd();

	virtual void Init();
    virtual void Composition();
	void CompositionOld();
	void CompositionNew();
    virtual void Update();
    virtual void Proc( SEventMessage* EMsg );
	virtual void SetWheelValue( short _wheel );
	bool SetWheelValue(SMouse & theMouse);

	void SetVisible(bool bFlag);
	void ResetSizeFrame();

	CChatOutList* GetChatOutList()	{ return m_pChatOutList; }
	void SetListScroll( int _linenum );
	bool CheckFocusWnd( UINT  FocusWnd );

	CFrameWnd* GetFrameWnd()  { return m_pNonActChatBgWnd;	}
	void ResetScroll();
	void RePosition();
};


//-----------------------------------------------------------------------------
#endif	__CHATPARTWNDLIST_H__