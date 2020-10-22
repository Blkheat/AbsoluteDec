#if !defined( __RUI_BATTLEZONE_WNDPROC_H__ )
#define __RUI_BATTLEZONE_WNDPROC_H__ 
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

#include "CharImgButton.h"


const WORD c_Max_BZ_GradeType_Num( 6 );

const WORD c_Max_BZChatMsg_Num( 100 );
const WORD c_Max_Draw_BZChatMsg_Num( 4 );

const WORD c_Max_BZRoomList_Num( 300 );
const WORD c_Max_Draw_BZRoomList_Num( 6 );

const WORD c_Max_BZUserList_Num( 300 );
const WORD c_Max_Draw_BZUserList_Num( 4 );

const WORD c_Max_WaitingRoom_UserNum( 12 );

#ifdef APPLY_BOLD_FONT
const WORD c_Max_ChatMsg_Length( 54 + 8 );
#else
const WORD c_Max_ChatMsg_Length( 64 + 8 );
#endif

const WORD c_Max_RoomName_Length( 40 );
const WORD c_Max_PassWord_Length( 10 );


const WORD c_Max_OptionBtn_UserNum_Num( 5 );
const WORD c_Max_OptionBtn_Time_Num( 5 );
const WORD c_Max_OptionBtn_Item_Num( 2 );
const WORD c_Max_OptionBtn_CanView_Num( 2 );

const WORD c_Max_BZRoomList_SortType_Num( 6 );



typedef enum NBZ_ScrollFocus
{
	n_BZ_Focus_RoomList = 0, 
	n_BZ_Focus_ChatWnd, 
	n_BZ_Focus_UserList,

} NBZ_ScrollFocus;

typedef enum NBZ_DrawSortType
{
	n_BZ_Sort_RoomNum = 0, 
	n_BZ_Sort_RoomName,
	n_BZ_Sort_CanView,
	n_BZ_Sort_GameType, 
	n_BZ_Sort_UserNum, 
	n_BZ_Sort_RoomState, 

} NBZ_DrawSortType;

typedef enum NBattleZoneWndType
{
	n_BattleZone_SelectZoneWnd = 0, 
	n_BattleZone_SelectGradeWnd, 
	n_BattleZone_RoomListWnd, 
	n_BattleZone_CreateRoomWnd, 
	n_BattleZone_WaitingRoomWnd, 

} NBattleZoneWndType;


typedef enum NBattleZoneType
{
	n_BattleZone_Nation = 0, 
	n_BattleZone_Free,

} NBattleZoneType;


typedef enum NBattleZoneGradeType
{
	n_BattleZone_21_30_Grade = 0, 
	n_BattleZone_31_40_Grade, 
	n_BattleZone_41_50_Grade, 
	n_BattleZone_51_65_Grade, 
	n_BattleZone_66_80_Grade, 
	n_BattleZone_81_100_Grade, 

} NBattleZoneGradeType;


typedef enum NBattleZoneRoomState
{
	n_BattleZone_None = 0,
	n_BattleZone_Ready, 
	n_BattleZone_Play,

} NBattleZoneRoomState;

typedef struct SBattleZoneRoomInfo
{
	int						room_idx;
	char					room_name[64];
	int						game_type;
	int						user_num;
	NBattleZoneRoomState	room_state;	
	
} SBattleZoneRoomInfo;


typedef struct SBattleZoneRoomList
{
	CButton*				RoomListBtn;

	CTextBox*				RoomNumTxt;

	CImageBox*				PassWordImg;

	CTextBox*				RoomNameTxt;
	CTextBox*				CanViewTxt;		
	CTextBox*				GameTypeTxt;
	CTextBox*				CurtUserNumTxt;
	CTextBox*				RoomStateTxt;

} SBattleZoneRoomList;

/*
typedef struct SBZ_RoomList
{
	DWORD					theRoomID;
	char					theName[40];
	WORD					theLock:1;			// 잠긴 방 여부 
	WORD					theGameType:6;		// 게임타입 
	WORD					theNum:4;			// 참가한 인원
	WORD					theMaxNum:4;		// 게임인원
	WORD					theStatus:1;		// 상태(준비, 게임)

} SBZ_RoomList;
*/

typedef struct SBZ_UserList
{
	char					UserName[ RP_MAX_PC_NAME ];

} SBZ_UserList;

typedef enum NBattleZoneGameMode
{
	n_BattleZone_DeathMatch = 0,
	n_BattleZone_BossMatch, 
	n_BattleZone_CaptureTheFlag,
	n_BattleZone_CaptureTheGround,
	n_BattleZone_Survival,

} NBattleZoneGameMode;

typedef enum NBattleZoneUserNum
{
	n_BattleZone_Max2 = 0,
	n_BattleZone_Max4, 
	n_BattleZone_Max6,
	n_BattleZone_Max8,
	n_BattleZone_Max12,

} NBattleZoneUserNum;

typedef enum NBattleZoneGameTime
{
	n_BattleZone_Minute3 = 0,
	n_BattleZone_Minute5, 
	n_BattleZone_Minute7,
	n_BattleZone_Minute10,
	n_BattleZone_Minute15,

} NBattleZoneGameTime;

typedef enum NBattleZoneUseItem
{
	n_BattleZone_UseItem_On = 0,
	n_BattleZone_UseItem_Off, 

} NBattleZoneUseItem;

typedef enum NBattleZoneCanView
{
	n_BattleZone_CanView_On = 0,
	n_BattleZone_CanView_Off, 

} NBattleZoneCanView;

typedef enum NBattleZoneViewType
{
	n_BattleZone_NormalZone = 0,
	n_BattleZone_ViewZone,	

}NBattleZoneViewType;

typedef struct SBattleZoneUserList
{
	CImageBox*				SlotBGImg;
	CImageBox*				NationImg;
	CCharImgButton*			CharImgBtn;
	CTextBox*				CharNameTxt;
	CImageBox*				TeamFlagRedImg;
	CImageBox*				TeamFlagGreenImg;
//	CRadioButton*			TeamFlagImg[2]; 
	CTextBox*				CharReadyTxt;
	CButton*				KickBtn;

} SBattleZoneUserList;

typedef enum NBattleZoneTeamFlag
{
	n_BattleZone_TeamFlag_Red = 0,
	n_BattleZone_TeamFlag_Green, 

} NBattleZoneTeamFlag;

typedef enum NBZ_UserState
{
	n_BZ_State_None = 0, 
	n_BZ_State_Ready, 
	n_BZ_State_Wait,

} NBZ_UserState;

typedef struct SBZ_WaitingUserInfo
{
	NBZ_UserState			FighterState;			// 빈슬롯, 준비 상태, 대기중 
	SRpdOtherPC				FighterInfo;			// 국가, 이름, 캐릭터 얼굴 
	BYTE					FighterTeam;			// 1팀, 2팀 
	
} SBZ_WaitingUserInfo;

//by simwoosung
typedef enum NBZ_ChatMsgType
{
	n_BZ_Normal_Chat = 0,
	n_BZ_Whisper_Chat,
	n_BZ_Help_Chat,	

} NBZ_ChatMsgType;

typedef struct SBZ_ChatMsgList
{
	UINT			ChatType;
	TCHAR			ChatMsg[ c_Max_ChatMsg_Length + 1];

	SBZ_ChatMsgList()
	{
		ChatType = n_BZ_Normal_Chat;
		SAFE_STR_CPY( ChatMsg, _RT(""), ( c_Max_ChatMsg_Length + 1 ) );
	}

	void operator=(const SBZ_ChatMsgList &temp)
	{
		ChatType = temp.ChatType;
		SAFE_STR_CPY( ChatMsg, temp.ChatMsg, ( c_Max_ChatMsg_Length + 1 ) );
	}

} SBZ_ChatMsgList;

// 게임 타입별 맵 번호 
typedef struct SBZ_MapInfo
{
	int			MapNum;
	int			MapIdx[c_Max_FightMap_Num];

} SBZ_MapInfo;


typedef struct SBZ_FightRecord
{
	int			TotalCnt;
	int			WinCnt;
	int			DrawCnt;
	int			LoseCnt;
	int			FightCntPerDay;			// 하후 총 80경기 중 몇개임 했는지 체크 

	SBZ_FightRecord()
	{
		TotalCnt = 0;
		WinCnt = 0;
		DrawCnt = 0;
		LoseCnt = 0;
		FightCntPerDay = 0;			
	}

} SBZ_FightRecord;


//--------------------------------------------------------------------------------------
// 경기 선택 
//--------------------------------------------------------------------------------------
class CBZ_SelectZoneWndProc : public CWndProc
{
public:
	NBattleZoneWndType		m_BZ_WndType;
	NBattleZoneType			m_BZ_ZoneType;
	
	SBZ_FightRecord			m_BZ_FightRecord;

public:

	CBZ_SelectZoneWndProc();  
	~CBZ_SelectZoneWndProc(); 

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );

	void Open( int aUserNum_Nation, int aUserNum_Free );
	
private:
	
	CFrameWnd*				m_pFrameWnd;
	
	CButton*				m_pCloseBtn;	

//	CTextBox*				m_pSelectZoneTxt;				// Title

	CButton*				m_pSelectZone_NationBtn;	
	CButton*				m_pSelectZone_FreeBtn;	

	CImageBox*				m_pSelectZoneBtn_FocusImg;	

	CTextBox*				m_pNationBattleTxt;
	CTextBox*				m_pNationBattle_DescTxt1;
	CTextBox*				m_pNationBattle_DescTxt2;

	CTextBox*				m_pFreeBattleTxt;
	CTextBox*				m_pFreeBattle_DescTxt1;
	CTextBox*				m_pFreeBattle_DescTxt2;

	CProgressBar*			m_pCongestion_Nation;			// 혼잡도 표시 
	CProgressBar*			m_pCongestion_Free;				// 혼잡도 표시 

};



//--------------------------------------------------------------------------------------
// 등급 선택 
//--------------------------------------------------------------------------------------
class CBZ_SelectGradeWndProc : public CWndProc
{
public:
	NBattleZoneGradeType	m_BZ_GradeType;
	char					m_BZ_GradeRange[ c_Max_BZ_GradeType_Num ][32];

	int						m_BZ_Congestion[ c_Max_BZ_GradeType_Num ];

public:

	CBZ_SelectGradeWndProc();  
	~CBZ_SelectGradeWndProc(); 

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );

	void SetSelectZoneTxt();
	void SetCongestionPGBar();
		
private:
	
	CFrameWnd*				m_pFrameWnd;
	
	CButton*				m_pCloseBtn;	

//	CTextBox*				m_pSelectGradeTxt;
	CTextBox*				m_pSelectZoneTxt;

	CImageBox*				m_pClass_1thImg;
	CTextBox*				m_pClass_1thTxt;

	CButton*				m_pLevel_21_30Btn;
	CButton*				m_pLevel_31_40Btn;
	CButton*				m_pLevel_41_50Btn;

	CTextBox*				m_pBasicLevel_1thTxt;
	CTextBox*				m_pNormalLevel_1thTxt;
	CTextBox*				m_pHighLevel_1thTxt;

	CTextBox*				m_pLevel_21_30Txt;
	CTextBox*				m_pLevel_31_40Txt;
	CTextBox*				m_pLevel_41_50Txt;

	CImageBox*				m_pLevel_21_30BKImg;
	CImageBox*				m_pLevel_31_40BKImg;
	CImageBox*				m_pLevel_41_50BKImg;

	CProgressBar*			m_pLevel_21_30PGBar;
	CProgressBar*			m_pLevel_31_40PGBar;
	CProgressBar*			m_pLevel_41_50PGBar;
	
	CImageBox*				m_pClass_2thImg;
	CTextBox*				m_pClass_2thTxt;

	CButton*				m_pLevel_51_65Btn;
	CButton*				m_pLevel_66_80Btn;
	CButton*				m_pLevel_81_100Btn;

	CTextBox*				m_pBasicLevel_2thTxt;
	CTextBox*				m_pNormalLevel_2thTxt;
	CTextBox*				m_pHighLevel_2thTxt;

	CTextBox*				m_pLevel_51_65Txt;
	CTextBox*				m_pLevel_66_80Txt;
	CTextBox*				m_pLevel_81_100Txt;

	CImageBox*				m_pLevel_51_65BKImg;
	CImageBox*				m_pLevel_66_80BKImg;
	CImageBox*				m_pLevel_81_100BKImg;
	
	CProgressBar*			m_pLevel_51_65PGBar;
	CProgressBar*			m_pLevel_66_80PGBar;
	CProgressBar*			m_pLevel_81_100PGBar;

};


//--------------------------------------------------------------------------------------
// 대전장 리스트  
//--------------------------------------------------------------------------------------
class CBZ_RoomListWndProc : public CWndProc
{
public:
	std::vector < SRpScFightRoomList::_SFightRoom >	m_BZ_RoomInfoList;			
	std::vector < SBZ_UserList >	m_BZ_UserNameList;			
	
	int								m_SelectRoomIndex;
	int								m_SelectUserIndex;
	
	NBZ_DrawSortType				m_BZ_Draw_SortType;

	std::vector < SRpScFightRoomList::_SFightRoom >	m_BZ_RoomInfoList_Waiting;		

	// 채팅키 도움말 테이블 필요 

public:

	CImageBox*						m_pDisableWndImg;

	
public:

	CBZ_RoomListWndProc();  
	~CBZ_RoomListWndProc(); 

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );

	void	Open();
	void	Close();
	void	SortDrawRoomList();
	void	SetRefreshRoomList();
	void	SetRefreshChatMsg();
	void	SetRefreshUserList();
	void	SetGameTypeText( int aGameType, char* aGameTypeTxt );
	void	SetRoomStateText( int aRoomState, char* aRoomStateTxt );
	void	SetWheelValue( short _wheel );
	void	SetRoomListScroll( int aLineNum );
	void	SetUserListScroll( int aLineNum );
	void	SetChatMsgScroll( int aLineNum );
	void	SetFocusWndColor( bool aIsFocus );
	
	CEditBox *	GetChatInputBox()  {  return m_pChatInputBox; }
	
//-----------------------------------------------------------------------------------
//채팅 연관 변수, 함수 정의부분 //by simwoosung	
//-----------------------------------------------------------------------------------	
	void	ChatMsgInit();
	void	SetChatType(UINT uChatType);
	void	SetWhisDest(void * strDest);
	
	void	InsertBZNomalChatMsg(TCHAR *strDest, TCHAR *strMsg);
	void	InsertBZWhisperChatMsg(TCHAR *strDest, TCHAR *strMsg, bool bToFlag);
	void	InsertBZHelpChatMsg(void *strMsg);
	//by simwoosung
public:
	SBZ_ChatMsgList			m_BZ_ChatMsgList[c_Max_BZChatMsg_Num + 1];
	UINT					m_uChatType;
	
protected:
	TCHAR					m_strChatstring[c_Max_ChatMsg_Length * 2];
	TCHAR					m_strTempBuf1[c_Max_ChatMsg_Length * 2];
	TCHAR					m_strTempBuf2[c_Max_ChatMsg_Length * 2];
	TCHAR					m_strWhisDest[256];

	int						m_nInsertIndex;
	SBZ_ChatMsgList			m_tempChatMsgList;
	
	void	InsertBZChatMsg();
	void    InsertBZChatMsg(TCHAR * strInString);

//-----------------------------------------------------------------------------------	
//-----------------------------------------------------------------------------------	
	
private:
	
	CFrameWnd*				m_pFrameWnd;
	
	CButton*				m_pGoOutBtn;	

	CImageBox*				m_pRoomListImg;
	CImageBox*				m_pChatWndImg;
	CImageBox*				m_pUserListImg;	
	
//	CTextBox*				m_pRoomListTxt;

	CTextBox*				m_pGradeTypeTxt;

	CTextBox*				m_pRoomNumTxt;
	CTextBox*				m_pRoomNameTxt;
	CTextBox*				m_pCanViewTxt;
	CTextBox*				m_pGameTypeTxt;
	CTextBox*				m_pCurtUserNumTxt;
	CTextBox*				m_pRoomStateTxt;
	
	CButton*				m_pSortRoomListBtn[ c_Max_BZRoomList_SortType_Num ];

	SBattleZoneRoomList		m_BZ_RoomList[ c_Max_Draw_BZRoomList_Num ];

	CImageBox*				m_BZ_RoomList_SelectImg;

	CScrollBar*				m_pRoomListScroll;

	CButton*				m_pFindReadyRoomBtn;	
	CButton*				m_pCreateRoomBtn;
	CButton*				m_pCanViewRoomBtn;
	CButton*				m_pJoinRoomBtn;	
	
	CTextBox*				m_pChatWndTxt;
	CButton*				m_pChatHelpBtn;		
	CTextBox*				m_pChatMsgTxt[ c_Max_Draw_BZChatMsg_Num ];
	CScrollBar*				m_pChatMsgScroll;
	CEditBox*				m_pChatInputBox;
	CButton*				m_pWhisperBtn;	

	CTextBox*				m_pUserListTxt;

	CButton*				m_pUserListBtn[ c_Max_Draw_BZUserList_Num ];
	
	CScrollBar*				m_pUserListScroll;

	CImageBox*				m_BZ_UserList_SelectImg;
	
	int						m_StartLineNum_RoomList;
	int						m_StartLineNum_ChatMsg;
	int						m_StartLineNum_UserList;

	NBZ_ScrollFocus			m_ScrollFocus;

	BOOL					m_bIsOnlyWaitingRoom;
	
	//by simwoosung
	long					m_lPrevTime;					//이전 채팅입력 시간	

};



//--------------------------------------------------------------------------------------
// 게임방 만들기   
//--------------------------------------------------------------------------------------
class CBZ_CreateRoomWndProc : public CWndProc
{
public:
	NBattleZoneGameMode		m_GameMode;
	NBattleZoneUserNum		m_MaxUserNum;
	NBattleZoneGameTime		m_MaxGameTime;
	NBattleZoneUseItem		m_UseItem;
	NBattleZoneCanView		m_CanView;

	int						m_GameModeValue[ c_Max_GameMode_Num ];
	int						m_MaxUserNumValue[ c_Max_OptionBtn_UserNum_Num ];
	int						m_MaxGameTimeValue[ c_Max_OptionBtn_Time_Num ];
	int						m_UseItemValue[ c_Max_OptionBtn_Item_Num ];
	int						m_CanViewValue[ c_Max_OptionBtn_CanView_Num ];

	char					m_RoomName[ c_Max_RoomName_Length ];
	char					m_PassWord[ c_Max_PassWord_Length ];

	// 게임 모드 설명 테이블 필요 

public:

	CBZ_CreateRoomWndProc();  
	~CBZ_CreateRoomWndProc(); 

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );
	
	void	Open();
	void	Close();
	void	SetModeDescScroll( int aLineNum );

private:
	
	CFrameWnd*				m_pFrameWnd;
	
	CImageBox*				m_pRoomNameImg;
	CImageBox*				m_pGameModeImg;
	CImageBox*				m_pOptionImg;

	CTextBox*				m_pCreateRoomTxt;

	CTextBox*				m_pRoomNameTxt;
	CEditBox*				m_pNameInputBox;

	CTextBox*				m_pPasswordTxt;
	CEditBox*				m_pPasswordInputBox;

	CTextBox*				m_pGameModeTxt;

//	CListBox*				m_pModeListBox;
//	CButton*				m_pModeList1Btn;
//	CButton*				m_pModeList2Btn;
//	CButton*				m_pModeList3Btn;
//	CButton*				m_pModeList4Btn;
//	CButton*				m_pModeList5Btn;
	CComboBox*				m_pModeListCBox;

//	CScrollBar*				m_pModeListScroll;

//	CTextBox*				m_pGameModeDescTxt;
	CTextBox*				m_pModeDescTxt;
	CScrollBar*				m_pModeDescScroll;

	CTextBox*				m_pOtherOptionTxt;
	
	CTextBox*				m_pMaxUserNumTxt;
	CRadioButton*			m_pMaxUserNumRBtn[ c_Max_OptionBtn_UserNum_Num ];

	CTextBox*				m_pMaxTimeTxt;
	CRadioButton*			m_pMaxTimeRBtn[ c_Max_OptionBtn_Time_Num ];

	CTextBox*				m_pUseItemTxt;
	CRadioButton*			m_pUseItemRBtn[ c_Max_OptionBtn_Item_Num ];
	
	CTextBox*				m_pCanViewTxt;
	CRadioButton*			m_pCanViewRBtn[ c_Max_OptionBtn_CanView_Num ];

	CButton*				m_pOKBtn;	
	CButton*				m_pCancelBtn;	


};


//--------------------------------------------------------------------------------------
// 게임 대기방  
//--------------------------------------------------------------------------------------
class CBZ_WaitingRoomWndProc : public CWndProc
{
public:
	_SFightRoomInfo			m_RoomInfo;
	NBattleZoneGameTime		m_MaxGameTime;
	NBattleZoneUseItem		m_UseItem;
	NBattleZoneViewType		m_ZoneViewType;

	WORD					m_WaitingRoomID;		// 방 번호 
	DWORD					m_MasterID;				// 방장 아이디  
	
	SBZ_WaitingUserInfo		m_WaitingUserInfo[ c_Max_WaitingRoom_UserNum ];

//	SRpdOtherPC				m_FighterInfo[ c_Max_WaitingRoom_UserNum ];
//	BYTE					m_FighterTeam[ c_Max_WaitingRoom_UserNum ];				
//	NBZ_UserState			m_FighterState[ c_Max_WaitingRoom_UserNum ];			

//	int						m_SelfSlotIdx;			// 자신의 슬롯 번호 

	BOOL					m_bIsReady;				
	BOOL					m_bIsMaster;			// 자신이 방장인지 여부 

	SBZ_MapInfo				m_FightMapInfo[c_Max_GameMode_Num];

	int						m_SelectUserIndex;

public:

	CBZ_WaitingRoomWndProc();  
	~CBZ_WaitingRoomWndProc(); 

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );
	
	void	Open( bool aIsNewGame );	
	void	Close();

	void	SetFightMapInfo();
	void	SetInitUserSlot();	
	void	JoinedAppearPC( int aSlotIndex );
	void	OutDisappearPC( int aSlotIndex );
	void	JoinedUserListSlot( SRpdOtherPC aPcInfo, int aTeamInfo, NBZ_UserState aUserState );
	void	OutUserListSlot( DWORD aPCID );

	void	SetChangeRoomMaster( DWORD aMasterID );
	void	SetChangeSelectTeam( DWORD aPCID, int aTeamFlag );
	void	SetChangeFightReady( DWORD aPCID, int aStateFlag );
	void	SetChangeRoomInfo( BYTE aMapCode, BYTE aTimeOut, BYTE aItemFlag, char* aRoomName );	
	void	SendChangeRoomInfo();

	void    SetChatMsgScroll( int aLineNum );
	void	SetRefreshChatMsg();

//-----------------------------------------------------------------------------------
//채팅 연관 변수, 함수 정의부분 //by simwoosung	
//-----------------------------------------------------------------------------------	
	void	ChatMsgInit();
	void	SetChatType(UINT uChatType);
	void	SetWhisDest(void * strDest);
	
	void	InsertBZNomalChatMsg(TCHAR *strDest, TCHAR *strMsg);
	void	InsertBZWhisperChatMsg(TCHAR *strDest, TCHAR *strMsg, bool bToFlag);
	void	InsertBZHelpChatMsg(void *strMsg);
	//by simwoosung
public:
	SBZ_ChatMsgList			m_BZ_ChatMsgList[c_Max_BZChatMsg_Num + 1];
	UINT					m_uChatType;
	
protected:
	TCHAR					m_strChatstring[c_Max_ChatMsg_Length * 2];
	TCHAR					m_strTempBuf1[c_Max_ChatMsg_Length * 2];
	TCHAR					m_strTempBuf2[c_Max_ChatMsg_Length * 2];
	TCHAR					m_strWhisDest[256];

	int						m_nInsertIndex;
	SBZ_ChatMsgList			m_tempChatMsgList;
	
	void	InsertBZChatMsg();
	void    InsertBZChatMsg(TCHAR * strInString);

//-----------------------------------------------------------------------------------	
//-----------------------------------------------------------------------------------

private:
	
	CFrameWnd*				m_pFrameWnd;
	
	CTextBox*				m_pRoomNameTxt;

	CImageBox*				m_pCharListImg;
	CImageBox*				m_pMapSelectImg;
	CImageBox*				m_pChatWndImg;
	CImageBox*				m_pOptionImg;

	CImageBox*				m_pTeam1Img;
	CImageBox*				m_pTeam2Img;
	
	CButton*				m_pTeam1Btn;
	CButton*				m_pTeam2Btn;
	CButton*				m_pReadyORStartBtn;

	CTextBox*				m_pTeam1Txt;
	CTextBox*				m_pTeam2Txt;

	SBattleZoneUserList		m_BZ_UserList[ c_Max_WaitingRoom_UserNum ];

	CTextBox*				m_pModeNameTxt;
	CImageBox*				m_pMiniMapImg;
	CComboBox*				m_pMapComboBox; 
	
	CTextBox*				m_pChatWndTxt;
	CTextBox*				m_pChatMsgTxt[ c_Max_Draw_BZChatMsg_Num ];
	CScrollBar*				m_pChatMsgScroll;
	CEditBox*				m_pChatInputBox;
	CButton*				m_pWhisperBtn;		
	CButton*				m_pChatHelpBtn;		

	CTextBox*				m_pOptionTxt;

	CTextBox*				m_pMaxTimeTxt;
	CRadioButton*			m_pMaxTimeRBtn[ c_Max_OptionBtn_Time_Num ];

	CTextBox*				m_pUseItemTxt;
	CRadioButton*			m_pUseItemRBtn[ c_Max_OptionBtn_Item_Num ];
	
	CButton*				m_pExitBtn;

	int						m_StartLineNum_ChatMsg;	

	CImageBox*				m_BZ_UserList_SelectImg;

	//by simwoosung
	long					m_lPrevTime;					//이전 채팅입력 시간
};


class CBZ_GameResultWndProc : public CWndProc
{
public:

	CBZ_GameResultWndProc();  
	~CBZ_GameResultWndProc(); 

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );
	

	void Open( BYTE aResult, DWORD aFame, DWORD aContribution, DWORD aExp, DWORD aEld, WORD aItemCode, BYTE aItemNum );
	void Close();

private:
	
	CFrameWnd*				m_pFrameWnd;
	
	CImageBox*				m_pWinImg;				
	CImageBox*				m_pLostImg;				
	CImageBox*				m_pDrawImg;				

	CImageBox9*				m_pResultImg;				
	
	CTextBox*				m_pContributionTxt;			// 국가 공헌도 
	CTextBox*				m_pContributionValueTxt;	// 국가 공헌도 수치 
	
	CTextBox*				m_pRespectTxt;				// 명성치 
	CTextBox*				m_pRespectValueTxt;			// 명성치 수치 

	CTextBox*				m_pExpTxt;					// 경험치  
	CTextBox*				m_pExpValueTxt;				// 경험치 수치 

	CTextBox*				m_pEldTxt;					// 엘드 획득 

	CTextBox*				m_pPresentItemTxt;			// 아이템 획득 

	long					m_pViewTime;			

};


extern	CBZ_SelectZoneWndProc		g_BZ_SelectZoneWnd;
extern	CBZ_SelectGradeWndProc		g_BZ_SelectGradeWnd;
extern	CBZ_RoomListWndProc			g_BZ_RoomListWnd;
extern	CBZ_CreateRoomWndProc		g_BZ_CreateRoomWnd;
extern	CBZ_WaitingRoomWndProc		g_BZ_WaitingRoomWnd;
extern	CBZ_GameResultWndProc		g_BZ_GameResultWnd;






//-----------------------------------------------------------------------------
#endif  __RUI_BATTLEZONE_WNDPROC_H__
 