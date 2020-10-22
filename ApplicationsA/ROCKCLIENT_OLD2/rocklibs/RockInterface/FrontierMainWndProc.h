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
	CTextBox*		m_pNickNameTxt;			// ȣĪ 
	CTextBox*		m_pGradeTxt;			// ��� 
	CTextBox*		m_pAccumulationFeeTxt;	// ����ȸ�� 

	CButton*		m_pSecessionBtn;		// Ż�� 
	CButton*		m_pPaymentBtn;			// ���� 

	bool			m_bIsMaster;			// �ڽ��� �޽������� ���� 
	bool			m_bIsMajor;				// �ڽ��� ���������� ���� 

	//==============================================================================
	// MY FRONTIER 
	CTextBox*		m_pFrontierNameTxt;		// �̸� 
	CTextBox*		m_pMasterNameTxt;		// �渶 �̸� 
	CTextBox*		m_pMajorNamesTxt;		// ��� �ӿ� �̸��� 
	CTextBox*		m_pMemberNumTxt;		// ����� 
	CTextBox*		m_pSavedEldTxt;			// ���� ���� 
	CTextBox*		m_pFameTxt;				// ��ġ 
	CTextBox*		m_pColonyTxt;			// �ݷδ� 
	CTextBox*		m_pTotalFameTxt;	    // ������
#ifdef DECO_RENEWAL_GUILD_MJH
	CTextBox*		m_pGuildTotalFameTxt;	// ��� ������
	CTextBox*		m_pGuildLevelTxt;	    // ��� ����
	CTextBox*		m_pGuildHelpTxt;	    // ��� �⿩��   

#endif // DECO_RENEWAL_GUILD_MJH

	CEmblemImageBox*	m_pEmblem;			// ���� (��� ����)

	CButton*		m_pViewAllBtn;			// ��ü ����  
	CButton*		m_pEldShareBtn;			// ���� �й�
	CButton*		m_pIncPersonsBtn;		// �ο� ����
#ifdef DECO_RENEWAL_GUILD_MJH
	CButton*		m_pGuildMenuBtn;	    // ���޴�

	N_FRONTIER_INFO_STATE InfoState;
#endif // DECO_RENEWAL_GUILD_MJH

	int				m_MaxMemberNum;			// �ִ� ��� ����� 

	//==============================================================================
	// �ż�  God's Creature ( ���� â���� )

	CTextBox*       Creaturetext[6];        // static text
	CTextBox*		m_pStatusTxt;			// ����  
	CTextBox*		m_pNameTxt;				// �̸�	
	CTextBox*		m_pTypeTxt;				// ����   
	CTextBox*		m_pLevelTxt;			// ����    
	CTextBox*		m_pExpTxt;				// ����ġ 
	CTextBox*		m_pEnergyTxt;			// Ȱ�� 

//	CProgressBar*	m_ExpBar;	
//	CProgressBar*	m_VitalBar;

	CButton*		m_pSummonsBtn;			// ��ȯ 
	CButton*		m_pCallOffBtn;			// ��ȯ ����
	CButton*		m_pEnergyUpBtn;			// ȸ���� �ֱ�
	
	//===============================================================================
	// ���� ����Ƽ��
	CTextBox*		m_pCoFronName;			// ��������Ƽ�� �̸�
	CTextBox*		m_pCoFMasterName;		// ��������Ƽ�� ������ �̸�
	CTextBox*		m_pCoFMemNum;			// ��������Ƽ�� �����
	CTextBox*		m_pCoFFame;				// ���� ����Ƽ�� ��ġ

	CButton*		m_pCoFBreakBtn;			// ���� ���� 
#ifdef DECO_RENEWAL_GUILD_MJH
	//===============================================================================
	// ��������
	CTextBox*		m_pGuildMaterNameTxt;   // ��帶�����̸�(����â��)
	CTextBox*		m_pGuildMemberNumTxt;	// ��� �ɹ��� (����â��)

	
	//===============================================================================
	// ��������
	CTextBox*		m_pNoticeInfoTxt;       // �������� (����â��)
#endif // DECO_RENEWAL_GUILD_MJH
	CTextBox*       Membertext[3];          // static text 
};
 

/*
#define      IDS_WND_PAYITEM					 637      ///-- ���� ������â 

#define      IDS_WND_FRONTIERMAIN                637      ///-- ����Ƽ�� ����â 

#define      IDS_FRONTIERMAIN_MYINFO			 637      ///-- MY INFO   
#define      IDS_FRONTIERMAIN_NICKNAME			 637      ///-- ȣ  Ī   
#define      IDS_FRONTIERMAIN_GRADE				 637      ///-- ��  �� 
#define      IDS_FRONTIERMAIN_ACCUMULATIONFEE	 637      ///-- ����ȸ��  
#define      IDS_FRONTIERMAIN_SECESSION			 637      ///-- Ż  ��  
#define      IDS_FRONTIERMAIN_PAYMENT            637      ///-- ��  �� 

#define      IDS_WND_INPUT_PAYMENT				 637      ///-- ������ �ݾ��� �Է����ּ���. 
#define      IDS_WND_INPUT_ELDSHARE				 637      ///-- �й��� �ݾ��� �Է����ּ���. 
#define      IDS_MSGBOX_ASK_SECESSION			 637      ///-- ���� Ż���Ͻðڽ��ϱ�? 

#define      IDS_FRONTIERMAIN_MYFRONTIER		 637      ///-- MY FRONTIER 
#define      IDS_FRONTIERMAIN_FRONTIERNAME		 637      ///-- Frontier Name
#define      IDS_FRONTIERMAIN_MASTER			 637      ///-- Master ( Name )
#define      IDS_FRONTIERMAIN_MAJOR				 637      ///-- Major ( Name )
#define      IDS_FRONTIERMAIN_ASSIST			 637      ///-- Assist ( Name )
#define      IDS_FRONTIERMAIN_MEMBER			 637      ///-- Member ( Number )
#define      IDS_FRONTIERMAIN_SAVEDELD			 637      ///-- �������� 
#define      IDS_FRONTIERMAIN_ELD				 637      ///-- ���� 
#define      IDS_FRONTIERMAIN_FAME				 637      ///-- �� �� ġ 
#define      IDS_FRONTIERMAIN_COLONY			 637      ///-- �� �� �� 
#define      IDS_FRONTIERMAIN_VIEWALL			 637      ///-- ��ü���� 
#define      IDS_FRONTIERMAIN_ELDSHARE			 637      ///-- ����й�

#define      IDS_FRONTIERMAIN_CREATURE			 637      ///-- ��  ��  
#define      IDS_FRONTIERMAIN_CREATURESTATUS	 637      ///-- ��  �� 
#define      IDS_FRONTIERMAIN_CREATURETYPE		 637      ///-- ��  �� 
#define      IDS_FRONTIERMAIN_CREATURELEVEL		 637      ///-- ��  �� 
#define      IDS_FRONTIERMAIN_CREATUREEXP		 637      ///-- �� �� ġ 
#define      IDS_FRONTIERMAIN_CREATUREENERGY	 637      ///-- Ȱ  �� 
#define      IDS_FRONTIERMAIN_SUMMONS			 637      ///-- ��  ȯ 
#define      IDS_FRONTIERMAIN_CALLOFF 			 637      ///-- ��ȯ���� 
#define      IDS_FRONTIERMAIN_ENERGYUP			 637      ///-- Ȱ��ȸ�� 
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
	CButton*		NameBtn;				// �̸�  
	CTextBox*		LevelTxt;			    // ���� 
	CTextBox*		MapTxt;				    // ���� 
	CTextBox*		NickNameTxt;			// ȣĪ 
	CTextBox*		GradeTxt;				// ��� 
	CTextBox*		AccumulationFeeTxt;		// ����ȸ�� 
	CTextBox*		FameTxt;				// ��ġ 
		
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
	char		theIntroduce[30];	// ��� �ڱ�Ұ�
	
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


//#define      IDS_WND_FRONTIERMEMBERLIST				 637      ///-- ����Ƽ�� ȸ�� ����â 


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
	CTextBox*		m_pMemberNameTxt;		// �̸�  
	CTextBox*		m_pNickNameTxt;			// ȣĪ 	
	CTextBox*		m_pGradeTxt;			// ��� 	
	CTextBox*		m_pAccumulationFeeTxt;	// ����ȸ�� 
	CTextBox*		m_pFameTxt;				// ��ġ 
	CTextBox*		m_pMyinfoTxt;			// �ڱ�Ұ�  

	CEditBox*		m_pNickNameEdit;		// ȣĪ ���� 
	CComboBox*		m_pGradeCBox;			// ��� ����

#ifdef DECO_RENEWAL_GUILD_MJH
	CButton*		m_pWhisperBtn;          //�ӼӸ�
	CButton*		m_pPartyinvitationBtn;  //��Ƽ�ʴ�
	CButton*		m_pMasterchangeBtn;     //���������
#endif // DECO_RENEWAL_GUILD_MJH
	CButton*		m_pKickBtn;
	
	CButton*		m_pOKBtn;		
	CButton*		m_pCancelBtn;	
	
	NFrontierGradeType  m_GradeType;		// ������ ��� ���� 

	char			m_FrontierGradeStr[ 4 ][ 32 ];	// ��޺� �ؽ�Ʈ ǥ�� 
	
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
	
	CTextBox*		m_pDescTxt[2];		// �̸�  
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
	CButton*		m_pSecessionBtn;		// Ż�� 
	CButton*		m_pPaymentBtn;			// ����
	CButton*		m_pMyinfoBtn;	    	// �ڱ�Ұ� 
	CButton*		m_pNoticeBtn;			// ��������
	

};

#endif //DECO_RENEWAL_GUILD_MJH

#define WID_FRONTIERMEMBEREDIT_WND					6170 


//#define      IDS_WND_FRONTIERMEMBEREDIT				 637      ///-- ����Ƽ�� ȸ�� ���� ����â
//#define      IDS_WND_FRONTIERMEMBEREDIT_KICK		 637      ///-- ���� Ż��
//#define      IDS_MSGBOX_ASK_REALLYKICK				 637      ///-- ���� Ż�� ��Ű�ڽ��ϱ�?




extern  CFrontierAddPersons g_FrontierAddPersons;
extern	CFrontierMainWnd	g_FrontierMainWnd;
extern	CFrontierMemberListWnd	g_FrontierMemberListWnd;
extern	CFrontierMemberEditWnd  g_FrontierMemberEditWnd;
#ifdef DECO_RENEWAL_GUILD_MJH
extern	CFrontierMenuBtnWnd  g_FrontierMenuBtnWnd;
#endif // DECO_RENEWAL_GUILD_MJH



//-----------------------------------------------------------------------------
#endif  __RUI_FRONTIERMAIN_WNDPROC_H__




