#if !defined( __RUI_FRONTIERMAIN_WNDPROC_H__ )
#define __RUI_FRONTIERMAIN_WNDPROC_H__
//-----------------------------------------------------------------------------
#include "..\\RockPCH.h"
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
#include "ListBox.h"
#include "ScrollBar.h"
#include "ComboBox.h"
#include "Spin.h"
#include "Wnd.h"


#include "WndProc.h"
#include "MCommunityWnd.h"




//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//

typedef enum NFrontierGradeType
{
	n_Frontier_Master_Grade = 0, 
	n_Frontier_Major_Grade, 
	n_Frontier_Assist_Grade, 
	n_Frontier_Senior_Grade,
	n_Frontier_Member_Grade, 
	
} NFrontierGradeType;

#ifdef DECO_RENEWAL_GUILD_MJH

enum N_FRONTIER_INFO_STATE
{
	n_FRONTIER_CREATURE_INFO = 0,
	n_FRONTIER_MEMBER_INFO,
	n_FRONTIER_NOTICE_INFO,	
};

#endif // DECO_RENEWAL_GUILD_MJH

class CFrontierMainWnd : public CWndProc
{
public:
	CFrontierMainWnd()
	{
		m_MaxMemberNum = 30;

	}
	~CFrontierMainWnd(){}

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );

	void CompositionNew();
	void CompositionOld();
	
	void Open();
	void Close();
	void LogOff();

	void CheckEnergyUpItem();
	void GetGradeTypeStr( char* aGradeTypeStr, int aGrade );
	
	void CheckChangeMasterMagorName(char* aOldName, char* aNewName);

	void RefreshFrontierMyInfo();
	void RefreshFrontierMyFrontier();
	void RefreshFrontierMyFrontierPet();
	void RefreshFrontierCombineFrontier();

	void RefreshMyFrontierGrade( int aGrade, char* aPCName );
	void SortMyFrontierGrade();
	
	void ClearFrontierMainWnd();
	//==============================================================================
#ifdef DECO_RENEWAL_GUILD_MJH
	void SetInfoState( N_FRONTIER_INFO_STATE state );
	void CloseControlType();
#endif // DECO_RENEWAL_GUILD_MJH
	//==============================================================================
	BOOL IsFullFrontierMember();
	CFrameWnd*		m_pFrameWnd;			

	CButton*		m_pCloseBtn;		
	
	//==============================================================================
	// MY INFO 
	CTextBox*		m_pNickNameTxt;			// 호칭 
	CTextBox*		m_pGradeTxt;			// 등급 
	CTextBox*		m_pAccumulationFeeTxt;	// 누적회비 

	CButton*		m_pSecessionBtn;		// 탈퇴 
	CButton*		m_pPaymentBtn;			// 납부 

	bool			m_bIsMaster;			// 자신이 메스터인지 구분 
	bool			m_bIsMajor;				// 자신이 메이저인지 구분 

	//==============================================================================
	// MY FRONTIER 
	CTextBox*		m_pFrontierNameTxt;		// 이름 
	CTextBox*		m_pMasterNameTxt;		// 길마 이름 
	CTextBox*		m_pMajorNamesTxt;		// 길드 임원 이름들 
	CTextBox*		m_pMemberNumTxt;		// 멤버수 
	CTextBox*		m_pSavedEldTxt;			// 보유 엘드 
	CTextBox*		m_pFameTxt;				// 명성치 
	CTextBox*		m_pColonyTxt;			// 콜로니 
	CTextBox*		m_pTotalFameTxt;	    // 누적명성
#ifdef DECO_RENEWAL_GUILD_MJH
	CTextBox*		m_pGuildTotalFameTxt;	// 길드 누적명성
	CTextBox*		m_pGuildLevelTxt;	    // 길드 레벨
	CTextBox*		m_pGuildHelpTxt;	    // 길드 기여도   

#endif // DECO_RENEWAL_GUILD_MJH

	CEmblemImageBox*	m_pEmblem;			// 엠블럼 (길드 문장)

	CButton*		m_pViewAllBtn;			// 전체 보기  
	CButton*		m_pEldShareBtn;			// 엘드 분배
	CButton*		m_pIncPersonsBtn;		// 인원 증가
#ifdef DECO_RENEWAL_GUILD_MJH
	CButton*		m_pGuildMenuBtn;	    // 길드메뉴

	N_FRONTIER_INFO_STATE InfoState;
#endif // DECO_RENEWAL_GUILD_MJH

	int				m_MaxMemberNum;			// 최대 허용 멤버수 

	//==============================================================================
	// 신수  God's Creature ( 신의 창조물 )

	CTextBox*       Creaturetext[6];        // static text
	CTextBox*		m_pStatusTxt;			// 상태  
	CTextBox*		m_pNameTxt;				// 이름	
	CTextBox*		m_pTypeTxt;				// 종류   
	CTextBox*		m_pLevelTxt;			// 레벨    
	CTextBox*		m_pExpTxt;				// 경험치 
	CTextBox*		m_pEnergyTxt;			// 활력 

//	CProgressBar*	m_ExpBar;	
//	CProgressBar*	m_VitalBar;

	CButton*		m_pSummonsBtn;			// 소환 
	CButton*		m_pCallOffBtn;			// 소환 해제
	CButton*		m_pEnergyUpBtn;			// 회복제 주기
	
	//===============================================================================
	// 연합 프론티어
	CTextBox*		m_pCoFronName;			// 연합프론티어 이름
	CTextBox*		m_pCoFMasterName;		// 연합프론티어 마스터 이름
	CTextBox*		m_pCoFMemNum;			// 연합프론티어 멤버수
	CTextBox*		m_pCoFFame;				// 연합 프론티어 명성치

	CButton*		m_pCoFBreakBtn;			// 연합 해지 
#ifdef DECO_RENEWAL_GUILD_MJH
	//===============================================================================
	// 길드원정보
	CTextBox*		m_pGuildMaterNameTxt;   // 길드마스터이름(정보창용)
	CTextBox*		m_pGuildMemberNumTxt;	// 길드 맴버수 (정보창용)

	
	//===============================================================================
	// 공지사항
	CTextBox*		m_pNoticeInfoTxt;       // 공지사항 (정보창용)
#endif // DECO_RENEWAL_GUILD_MJH
	CTextBox*       Membertext[3];          // static text 
};
 

/*
#define      IDS_WND_PAYITEM					 637      ///-- 구매 아이템창 

#define      IDS_WND_FRONTIERMAIN                637      ///-- 프론티어 정보창 

#define      IDS_FRONTIERMAIN_MYINFO			 637      ///-- MY INFO   
#define      IDS_FRONTIERMAIN_NICKNAME			 637      ///-- 호  칭   
#define      IDS_FRONTIERMAIN_GRADE				 637      ///-- 등  급 
#define      IDS_FRONTIERMAIN_ACCUMULATIONFEE	 637      ///-- 누적회비  
#define      IDS_FRONTIERMAIN_SECESSION			 637      ///-- 탈  퇴  
#define      IDS_FRONTIERMAIN_PAYMENT            637      ///-- 납  부 

#define      IDS_WND_INPUT_PAYMENT				 637      ///-- 납부할 금액을 입력해주세요. 
#define      IDS_WND_INPUT_ELDSHARE				 637      ///-- 분배할 금액을 입력해주세요. 
#define      IDS_MSGBOX_ASK_SECESSION			 637      ///-- 정말 탈퇴하시겠습니까? 

#define      IDS_FRONTIERMAIN_MYFRONTIER		 637      ///-- MY FRONTIER 
#define      IDS_FRONTIERMAIN_FRONTIERNAME		 637      ///-- Frontier Name
#define      IDS_FRONTIERMAIN_MASTER			 637      ///-- Master ( Name )
#define      IDS_FRONTIERMAIN_MAJOR				 637      ///-- Major ( Name )
#define      IDS_FRONTIERMAIN_ASSIST			 637      ///-- Assist ( Name )
#define      IDS_FRONTIERMAIN_MEMBER			 637      ///-- Member ( Number )
#define      IDS_FRONTIERMAIN_SAVEDELD			 637      ///-- 보유엘드 
#define      IDS_FRONTIERMAIN_ELD				 637      ///-- 엘드 
#define      IDS_FRONTIERMAIN_FAME				 637      ///-- 명 성 치 
#define      IDS_FRONTIERMAIN_COLONY			 637      ///-- 콜 로 니 
#define      IDS_FRONTIERMAIN_VIEWALL			 637      ///-- 전체보기 
#define      IDS_FRONTIERMAIN_ELDSHARE			 637      ///-- 엘드분배

#define      IDS_FRONTIERMAIN_CREATURE			 637      ///-- 신  수  
#define      IDS_FRONTIERMAIN_CREATURESTATUS	 637      ///-- 상  태 
#define      IDS_FRONTIERMAIN_CREATURETYPE		 637      ///-- 종  류 
#define      IDS_FRONTIERMAIN_CREATURELEVEL		 637      ///-- 레  벨 
#define      IDS_FRONTIERMAIN_CREATUREEXP		 637      ///-- 경 험 치 
#define      IDS_FRONTIERMAIN_CREATUREENERGY	 637      ///-- 활  력 
#define      IDS_FRONTIERMAIN_SUMMONS			 637      ///-- 소  환 
#define      IDS_FRONTIERMAIN_CALLOFF 			 637      ///-- 소환해제 
#define      IDS_FRONTIERMAIN_ENERGYUP			 637      ///-- 활력회복 
*/




//#define      IDS_SYSPR


// Need to Frontier ICON => pMainFrame->SetIconTID( TID_CTRL_ICON_STATUS );						


//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//

typedef struct SFrontierMemberList
{
	CButton*		NameBtn;				// 이름  
	CTextBox*		LevelTxt;			    // 레벨 
	CTextBox*		MapTxt;				    // 지역 
	CTextBox*		NickNameTxt;			// 호칭 
	CTextBox*		GradeTxt;				// 등급 
	CTextBox*		AccumulationFeeTxt;		// 누적회비 
	CTextBox*		FameTxt;				// 명성치 
		
	void Init()
	{
		NameBtn->SetText( _RT( "" ) );
		LevelTxt->SetText( _RT( "" ) );
		MapTxt->SetText( _RT( "" ) );
		NickNameTxt->SetText( _RT( "" ) );
		GradeTxt->SetText( _RT( "" ) );
		AccumulationFeeTxt->SetText( _RT( "" ) );
		FameTxt->SetText( _RT( "" ) );
	}
	
} SFrontierMemberList;

typedef struct SFMemberInfo
{	
	char		Name[32];
	char		NickName[32];
	int			Grade;
	int			AccumulationFee;
	int			Fame;
	short       Level;
	short       TheMap;
	char		theIntroduce[30];	// 길드 자기소개
	
} SFMemberInfo;

typedef enum NFM_DrawSortType
{
	n_FM_Sort_Name = 0, 
	n_FM_Sort_Level,
	n_FM_Sort_Map,
	n_FM_Sort_NickName, 
	n_FM_Sort_Grade, 
	n_FM_Sort_AccumulationFee, 
	n_FM_Sort_Fame, 

} NFM_DrawSortType;


const WORD c_Max_Draw_MemberList( 8 );
const WORD c_Max_FrontierMember_SortType( 7 );


class CFrontierMemberListWnd : public CWndProc
{
public:
	CFrontierMemberListWnd(){}
	~CFrontierMemberListWnd(){}

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );
		
	void Open();
	void Close();
	void ClearFrontierMemberListWnd();	

	void SortDrawMemberList();
	void RefreshMemberInfoList();
	
	void SetWheelValue( short _wheel );
	void SetFMemberListScroll( int aLinenum );
		
	
	//==============================================================================	

	CFrameWnd*				m_pFrameWnd;			

	CButton*				m_pCloseBtn;		
	
	CScrollBar*				m_pListScroll;

	int						m_StartListNum;

	NFM_DrawSortType		m_FM_Draw_SortType; 
			
	CButton*				m_pSortMemberListBtn[ c_Max_FrontierMember_SortType ];	
	SFrontierMemberList		m_DrawMemberList[ c_Max_Draw_MemberList ];

	std::vector < SFMemberInfo >	m_FrontierMemberInfoList;			

};

#define WID_FRONTIERMEMBERLIST_WND					6160 


//#define      IDS_WND_FRONTIERMEMBERLIST				 637      ///-- 프론티어 회원 정보창 


//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//

class CFrontierMemberEditWnd : public CWndProc
{
public:
	CFrontierMemberEditWnd (){}
	~CFrontierMemberEditWnd (){}

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );
	
	void Open( SFMemberInfo aMemberInfo );
//	void RefreshIntroduceInfo();
	void Close();

	//==============================================================================	

	CFrameWnd*		m_pFrameWnd;			

	CButton*		m_pCloseBtn;		
	
	//==============================================================================	
	CTextBox*		m_pMemberNameTxt;		// 이름  
	CTextBox*		m_pNickNameTxt;			// 호칭 	
	CTextBox*		m_pGradeTxt;			// 등급 	
	CTextBox*		m_pAccumulationFeeTxt;	// 누적회비 
	CTextBox*		m_pFameTxt;				// 명성치 
	CTextBox*		m_pMyinfoTxt;			// 자기소개  

	CEditBox*		m_pNickNameEdit;		// 호칭 수정 
	CComboBox*		m_pGradeCBox;			// 등급 조정

#ifdef DECO_RENEWAL_GUILD_MJH
	CButton*		m_pWhisperBtn;          //귓속말
	CButton*		m_pPartyinvitationBtn;  //파티초대
	CButton*		m_pMasterchangeBtn;     //길드장위임
#endif // DECO_RENEWAL_GUILD_MJH
	CButton*		m_pKickBtn;
	
	CButton*		m_pOKBtn;		
	CButton*		m_pCancelBtn;	
	
	NFrontierGradeType  m_GradeType;		// 변경할 등급 저장 

	char			m_FrontierGradeStr[ 4 ][ 32 ];	// 등급별 텍스트 표시 
	
	SFMemberInfo	m_MemberInfo;

};

class CFrontierAddPersons : public CWndProc
{

public:
	CFrontierAddPersons(){}
	~CFrontierAddPersons(){}

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg);

	void Open();
	void Close();
	
	CFrameWnd*		m_pFrameWnd;
	
	CTextBox*		m_pDescTxt[2];		// 이름  
	CButton*		m_pOKBtn;		
	CButton*		m_pCancelBtn;	

};

#ifdef DECO_RENEWAL_GUILD_MJH

class CFrontierMenuBtnWnd : public CWndProc
{

public:
	CFrontierMenuBtnWnd(){}
	~CFrontierMenuBtnWnd(){}

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg);

	void Open();
	void Close();
	
	CFrameWnd*		m_pFrameWnd;
	CButton*        m_pCloseBtn;
	CButton*		m_pSecessionBtn;		// 탈퇴 
	CButton*		m_pPaymentBtn;			// 납부
	CButton*		m_pMyinfoBtn;	    	// 자기소개 
	CButton*		m_pNoticeBtn;			// 공지사항
	

};

#endif //DECO_RENEWAL_GUILD_MJH

#define WID_FRONTIERMEMBEREDIT_WND					6170 


//#define      IDS_WND_FRONTIERMEMBEREDIT				 637      ///-- 프론티어 회원 정보 수정창
//#define      IDS_WND_FRONTIERMEMBEREDIT_KICK		 637      ///-- 강제 탈퇴
//#define      IDS_MSGBOX_ASK_REALLYKICK				 637      ///-- 정말 탈퇴 시키겠습니까?




extern  CFrontierAddPersons g_FrontierAddPersons;
extern	CFrontierMainWnd	g_FrontierMainWnd;
extern	CFrontierMemberListWnd	g_FrontierMemberListWnd;
extern	CFrontierMemberEditWnd  g_FrontierMemberEditWnd;
#ifdef DECO_RENEWAL_GUILD_MJH
extern	CFrontierMenuBtnWnd  g_FrontierMenuBtnWnd;
#endif // DECO_RENEWAL_GUILD_MJH



//-----------------------------------------------------------------------------
#endif  __RUI_FRONTIERMAIN_WNDPROC_H__




