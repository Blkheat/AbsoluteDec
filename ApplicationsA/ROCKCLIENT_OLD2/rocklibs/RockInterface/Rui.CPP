///////////////////////////////////////////////////////////////////////////////
///
///		File		: Rui.h
///		Desc		:
///
///		Author		: Park Soo-Hyun
///		Team		: Program - Client Team
///		Date		: 
///
///		Copyright(c) 2004, Rocksoft Co., Ltd. ALL RIGHTS RESERVED
///
///////////////////////////////////////////////////////////////////////////////
#include "Rui.h"

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
#include "Spin.h"
#include "StateButton.h"
#include "ChatOutput.h"
#include "MiniMap.h"
#include "CharImgButton.h"
#include "FlashWnd.h"
#include "ItemGetButton.h"
#include "ComEffImgBox.h"
#include "ComEffProgBar.h"
#include "ComboStepBar.h"
#include "FlashFrameWnd.h"
#include "LocalEntireMap.h"
#include "MLineEditBox.h"
#include "LimitLenTextBox.h"
#include "FrameTextBox.h"
#include "CTextOutList.h"
#include "ChatEditBox.h"
#include "RITextBox.h"
#include "CArrowLineList.h"
#include "ImageAni.h"
//#include "Slot.h"

#include "CompntCommon.h"


//#include "Pc.h"
#include "..\\..\\global.h"
#include "..\\..\\RockClient.h"
#include "..\\..\\StringManager.h"
#include "..\\..\\MapDataManager.h"
#ifdef UI_CONTROL_RENEWAL
	#include "..\\..\\UIDataManager.h"
#endif// UI_CONTROL_RENEWAL
#include "..\\..\\bravolibs\\Sound\\Sound.h"

#include "WndProc.h"
#include "LoadingWndProc.h"
#include "LoginWndProc.h"
#include "CharStateWndProc.h"
#include "InventoryWndProc.h"
#include "AbilityUpWndProc.h"
#include "AbilityUpConfirmWnd.h"
#include "SkillWndProc.h"
#include "SkillRegistWnd.h"
#include "SkillLevelUpWnd.h"
#include "SkillInfoWnd.h"
#include "SkillOverWriteWnd.h"
#include "MiniMapWndProc.h"
#include "SystemMenuWndProc.h"
#include "MessageWndProc.h"
#include "StoreWndProc.h"
#include "BankWndProc.h"
#include "QuestProc.h"
#include "CArcherBar.h"
#include "PlayerInfoWnd.h"
#include "PlayerInfoAddWnd.h"
#include "NonPlayerInfoWnd.h"
#include "NonPlayerInfoAddWnd.h"
#include "ExchangeWndProc.h"
#include "WarehouseWndProc.h"

#include "ChantHitBarWnd.h"
#include "ServerListWndProc.h"
#include "MainMenuWndProc.h"
#include "ChatBoxWndProc.h"
#include "LoadingWndProc.h"
#include "CDelayBarWnd.h"
#include "CSlotDelayProc.h"
#include "CBillBoardUIRender.h"
#include "CPAcceptYesNoWnd.h"
#include "UIMsgWnd.h"
#include "PartyInforWnd.h"
#include "PartyPopupWnd.h"
#include "UiUtil.h"
#include "Announcement.h"
#include "EmblemTexManager.h"

#include "..\\..\\quadlist.h"
#include "..\\..\\bravolibs\\network\\Protocol.h"
#include "..\\..\\bravolibs\\obj\\player_manager.h"
#include "..\\..\\Pc.h"

#include "CharSelectWndProc.h"
#include "MessageWndProc.h"
#include "NpcChatWnd.h"
#include "AliveWnd.h"
#include "SkillLearnProc.h"
#include "SysMsgWndProc.h"
#include "..\\..\\Effect.h"
#include "ItemEnchantWndProc.h"
#include "ItemEnchantRWndProc.h"
#include "ItemExtractWndProc.h"
#include "ItemExtractRWndProc.h"
#include "ItemRemodel_SWndProc.h"
#include "ItemRemodel_FWndProc.h"
#include "ItemRemodelWndProc.h"
#include "itemRepairwndproc.h"
#include "itemcreatewndproc.h"
#include "itemcreate_fwndproc.h"
#include "itemcreate_swndproc.h"
#include "itemAccEnchant.h"
#include "ItemAccEnchantRWndProc.h"
#include "ItemEldaDust.h"

#include "MCommunityWnd.h"
#include "MessengerMemoWnd.h"
#include "ChatMainProc.h"
#include "SystemPrintWnd.h"
#include "MessageBox.h"

#include "MiniGame_Elda.h"

#include	"profile\profile.h"
#include	"profile\custom_time.h"
#include    "CLogWnd.h"

#include "BattleZoneWndProc.h"
#include "..\\..\\CRenderManager.h"
#include "CUserStoreSellWnd.h"
#include "CSocialActWnd.h"
#include "CUserStoreSetBuyWnd.h"
#include "CWebShopWnd.h"
#include "FrontierCreateWnd.h"
#include "FrontierMainWndProc.h"

#include "LeagueInfoWnd.h"

#include "PetInfoWnd.h"
#include "PetInventoryWnd.h"
#include "PetMainWndProc.h"

#include "ItemSkillPotionWndProc.h"
#include "ItemEldaMixAWndProc.h"

#include "CColRegionListWnd.h"
#include "CColRegionInfoWnd.h"
#include "CColItemPreViewWnd.h"
#include "CColMapPositionWnd.h"

#include "CGuideWnd.h"

#include "..\\..\\RockClient.h"

#include "..\\..\\UserTime.h"
#include "RadioBroadcast.h"

#include "CAcryEnchantWnd.h"
#include "CAcryEnchantRWnd.h"
#include "CImm.h"
#include "PlayerEtcPrc.h"
#include "ItemEldaMixRWndProc.h"

#include "CChangeEldaColorWnd.h"
#include "CChangeEldaColorRWnd.h"

#include "EssenceWndProc.h"
#include "SpellStoneWndProc.h"
#include "StrengthStoneWndProc.h"

#include "CharCreateWndProc.h"
#include "GuildInventoryWndProc.h"
#include "ProposeJoinFrontier.h"

CRui	*nRui = NULL;

static bool IsInfo			= false;
static bool IsOption		= false;
static bool IsCharSelect	= false;
static bool IsChat			= false;
static bool lb_set_wall = false;		///-- SetClient Image 로 Wall Paper 를 Set 했는지 판단.


extern long		g_nowTime;
#ifdef DIRECT_VERSION_9_MJH
extern IDirect3DDevice9*	g_lpDevice;
#else
extern IDirect3DDevice8*	g_lpDevice;
#endif // DIRECT_VERSION_9_MJH
extern int		g_Version;			///-- G_Client Version
extern bool g_ISCharShowIDVisible;

int g_nCandidate_Page_Size = 9;

///---------------------------------------------------------------------------
///-- CRui
///---------------------------------------------------------------------------
///-- Font 추가 루틴 추가.
#ifdef DIRECT_VERSION_9_MJH
CRui::CRui( LPDIRECT3DDEVICE9 D3DDevice )
#else
CRui::CRui( LPDIRECT3DDEVICE8 D3DDevice )
#endif // DIRECT_VERSION_9_MJH
{
	int i;
	///--char FontPath[1024];
	theD3DDevice = D3DDevice;
	
	Container	= SAFE_NEW( CContainer() );	
	Resource	= SAFE_NEW( CResource( theD3DDevice ) );	
	Render		= SAFE_NEW( CRender( theD3DDevice ) );		
	
	InitTimer();

	bool result = false;
	
	///-- Mini_Game
	m_bOnMiniGame = false;
	theMiniGame = NULL;
	
	m_pCursorBitmap = NULL;
	
	memset(mac_comp_str , NULL, 1024  )	;		///-- !!!!J_IME						
	mb_candidate = false;						///-- Candidate 창 열림 판단.
	ml_candi_start_idx = 0;						///-- 현재 보여줄 Page 의 첫 idx
	ml_candi_sel_idx = 0;						///-- 현재 선택된 idx	
	ml_candi_count = 0;							///-- Candidate Character Count
	
	for( i = 0 ; i < g_nCandidate_Page_Size; ++i )
	{
		memset(&mac_candi_str[i][0] , NULL , sizeof(char) * 100 );
	}
	
	m_convertedPos[0] = 0;
	m_convertedPos[1] = 0;
	bVerticalReading = 0 ; 
		
	ZeroMemory(StrTextReading , sizeof(StrTextReading)  );
	ZeroMemory(StrTextReadingTemp , sizeof(StrTextReadingTemp));

	// 폰트를 설치할 경로를 구한다. 윈도우즈 디렉토리 아래의 Fonts 폴더
	/*GetWindowsDirectory(FontPath, MAX_PATH);
	SAFE_STR_CAT(FontPath, "\\Fonts\\malgun.ttf", 1024);

	FILE* fontfile = NULL;
	fontfile = fopen(FontPath, "rb");
	if(fontfile == NULL)
	{
		result = CopyFile( "malgun.ttf", FontPath, FALSE );
		if( result == FALSE )
		{		
			return;
		}		

		result = AddFontResource( FontPath );
		if( result == FALSE )
		{
		   assert( 0 && "Font 등록 실패" );
		   return;
		}

		SendMessage(HWND_BROADCAST,WM_FONTCHANGE,0,0);
	}
	else
	{
		fclose(fontfile);
	}*/

	m_lEdPrevtime = g_nowTime;
	m_IsSurpportHCursor = true;
	m_WorldIndex = 0;
	m_ChannelIndex = 0 ;

	m_bIsShowUI = true;
	thePcinfoCount =0 ;  
	
	TestNumFame = 0 ; 
	
	g_Imm.Init();
	m_hDllIme	=NULL;
	m_bCheckChinaCHS  =false; 


	ZeroMemory(&m_BugMop,sizeof(MopInfo));
		
	
	m_BugMop.fMopAttack = 0.90f;
	m_BugMop.fMopAttacked = 0.90f;
	m_BugMop.fMopDie = 0.0f ; 
	m_BugMop.fMopSkill = 0.85f; 
	m_BugMop.fMopWait = 0.90f ; 
	m_BugMop.fMopMove = 0.99f ; 

#ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/22)
	
	m_fResolutionRateW = 0.0f;
	m_fResolutionRateH = 0.0f;
	
#endif // #ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/22)
}

//-----------------------------------------------------------------------------
CRui::~CRui()
{
	theSlot.Clear();
	
	int i = 0;
	//SPcItem*	WearItem[RP_MAX_EQUIP];
	for( i = 0; i < RP_MAX_EQUIP; ++i )
	{
		SAFE_DELETE(thePcInven.WearItem[i]);
	}
	
	//SPcItem*	InvenItem[RP_MAX_PAYINVENTORY+RP_MAX_INVENTORY];
	for( i = 0; i < c_MaxInvenItemNum; ++i )
	{
		SAFE_DELETE(thePcInven.InvenItem[i]);
	}
	
	//SPcItem*	InvenQuest[RP_MAX_QUEST_ITEM];
	for( i = 0; i < RP_MAX_QUEST_ITEM; ++i )
	{
		SAFE_DELETE(thePcInven.InvenQuest[i]);
	}

	for( i = 0; i < RP_MAX_QUEST_ITEM; ++i )
	{
		SAFE_DELETE(thePcInven.InvenHorse[i]);	
	}
	
	//SPcItem*	SkillItem[RP_MAX_USER_SKILL];
	for( i = 0; i < RP_MAX_USER_SKILL; ++i )
	{
		SAFE_DELETE(thePcInven.SkillItem[i]);
	}
	
	//SPcItem*	QuickSlot[RP_MAX_QUICK_SLOT];
	for( i = 0; i < RP_MAX_QUICK_SLOT; ++i )
	{
		thePcInven.QuickSlot[i] = NULL;	
	}
	
	for( i = 0; i < RP_MAX_QUEST_FLAG; ++i )
	{
		SAFE_DELETE(thePcInven.QuestFlag[i]);
	}	
	
	for( i = 0; i < RP_MAX_QUEST_COUNT; ++i )
	{
		SAFE_DELETE(thePcInven.QuestCount[i]);
		SAFE_DELETE(thePcInven.QuestTime[i]);
	}	
	
	for( i = 0; i < RP_MAX_PAY_CONTENTS; ++i )
	{
		SAFE_DELETE(thePcInven.PayEffect[i]);
	}
	
	for( i = 0; i < RP_MAX_TIMELIMIT_PAY_CONTENTS; ++i )
	{
		SAFE_DELETE(thePcInven.PayTimeItemList[i]);
	}
	

	SAFE_DELETE(thePcInven.FrontierMyInfo);
	SAFE_DELETE(thePcInven.FrontierInfo);
	SAFE_DELETE(thePcInven.pComBineFrontierInfo);
	
	//pet data
	
	for(i = 0 ; i < 3 ; ++i)
	{
		SAFE_DELETE(thePcInven.thePetData[i]);
	}
	
	memset( thePcInven.FrontierCaller, 0, sizeof( char ) * RP_MAX_PC_NAME );		
	
	SAFE_DELETE(thePcInven.FrontierPetInfo);
	SAFE_DELETE(Container);
	SAFE_DELETE(Resource);
	SAFE_DELETE(Render);
	SAFE_DELETE(theHelpTip);
	SAFE_DELETE(theItemTip);
	SAFE_DELETE(Fontman);
	SAFE_DELETE(theMiniGame);

	//UNICODE_ROCK
	memset(mac_comp_str , NULL , 1024 ); ///-- !!!!J_IME
	mb_candidate = false;						///-- Candidate 창 열림 판단.
	ml_candi_start_idx = 0;						///-- 현재 보여줄 Page 의 첫 idx
	ml_candi_sel_idx = 0;						///-- 현재 선택된 idx	
	ml_candi_count = 0;							///-- Candidate Character Count
	
	m_convertedPos[0] = 0;
	m_convertedPos[1] = 0;
	
	for( i = 0 ; i < g_nCandidate_Page_Size; ++i )
	{
		mac_candi_str[i][0] = 0;
	}
	
	
	if( m_hDllIme )
	{
		FreeLibrary(m_hDllIme);
	}

	InitDlgBalloon();
}

//-----------------------------------------------------------------------------
bool CRui::CreateDesktop( HINSTANCE hInst, HWND hWnd, int Width, int Height, int Bits )
{
	if( g_RockClient.GetLanguage() == eLang_China )
	{
		g_nCandidate_Page_Size = 10;
	}	
	
	int i = 0;
	
	DurationWnd			= WID_None;//유지 윈도우
	
	
	ModalWnd			= WID_None;//모달 윈도우
	
	
	ActiveWnd			= WID_None;//활성 윈도우
	
	
	FocusWnd			= WID_None;//현재 마우스 위치의 포커스 윈도우 
	FocusPwnd			= WID_None;	
	
	memset( &theEMsg, 0, sizeof( SEventMessage ) );
	isDragging = false;
	
	Desktop.hInst		= hInst;
	Desktop.hWnd		= hWnd;
	Desktop.hCurWnd		= hWnd;
	Desktop.Width		= Width;
	Desktop.Height		= Height;
	Desktop.Bits		= Bits;
	Desktop.DraggingWnd = WID_None;
	Desktop.CurFocusWnd	= WID_None;
	Desktop.SelectedWnd	= WID_None;
	
	
	Fontman				= SAFE_NEW( CFontman( hWnd, theD3DDevice ) );	
	
	theBGround.IsAction	= true;
	theBGround.Effect	= n_efNone;
	theBGround.Img		= TID_LOGIN_BACK;
	theBGround.Size[0]	= 0;
	theBGround.Size[1]	= 0;
	theBGround.Size[2]	= Desktop.Width;
	theBGround.Size[3]	= Desktop.Height;
	theBGround.Color	= D3DCOLOR_ARGB( 255, 255, 255, 255 );
	
	theFGround.IsAction	= false;
	theFGround.Effect	= n_efNone;
	theFGround.Img		= TID_None;
	theFGround.Size[0]	= 0;
	theFGround.Size[1]	= 0;
	theFGround.Size[2]	= Desktop.Width;
	theFGround.Size[3]	= Desktop.Height;
	theFGround.Color	= D3DCOLOR_ARGB( 255, 255, 255, 255 );
	
	/*
	theCursor.Img[0]	= TID_Sursor_Normal;
	theCursor.Pos[0]	= 0;
	theCursor.Pos[1]	= 0;
	theCursor.Color		= D3DCOLOR_ARGB( 255, 255, 255, 255 );
	//*/
	
	isShowInfo			= true;
	
	theEnterFlag		= n_nfUIEnter;
	
	memset( theMessage, 0, sizeof( SMessageStr ) * 256 );
	theNumMsg			= 0;
	theFontg1			= Fontman->GetFont( n_ftGulimChe, 12, n_fwNormal, n_fdOutline );
	theFontg2			= Fontman->GetFont( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	theFontg3			= Fontman->GetFont( n_ftGulimChe, 24, n_fwNormal, n_fdOutline );
	
	///-- Japan IME Candidate List Font
	mc_font_candisel	= Fontman->GetFont( n_ftGulimChe, 12, n_fwNormal, n_fdOutline );
	mc_font_candi		= Fontman->GetFont( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	
	
	theHelpTip			= SAFE_NEW( CHelpTip( &Desktop ) );	
	theHelpTip->theFontg = theFontg2;
	
	theItemTip			= SAFE_NEW( CHelpItem( &Desktop ) );	
    theItemTip->theFontg = theFontg2;
	
	g_ColItemPreViewWnd.Initialize( &Desktop );
	g_ColItemPreViewWnd.m_Fontg = theFontg2;
	
	theNumDlg			= 0;
	for( i = 0; i < MAX_DLGBALLOON; ++i )
	{
		theDlgBalloon[i] = NULL;
	}
	
	thePickPcIdx	= -1;
	
#ifdef ROCKCLIENT_DEV
	theViewPcInfo	= n_VIEW_ALL_PCID;
	//	theViewPcInfo	= n_VIEW_PICK_PCID;	
#else
	theViewPcInfo	= n_VIEW_PICK_PCID;	
#endif				
	
	theCursorType	= n_CT_NORMAL;
	theCursorAniIdx	= 0;// 타이밍 때문에...0이 아닌 1부터 시작....
	theCursor		= Resource->GetCursorInfo( n_CT_NORMAL );	
	theBorder		= Resource->GetBorderInfo();
	
	theCursorState	= n_CT_NORMAL;
	
	//-------------------------------------------------------------------------
	memset( &thePcParam, 0, sizeof( SPcDataParam ) );
	memset( &thePcInven, 0, sizeof( SPcDataInven ) );
	thePcItem = NULL;
	theSlot.Clear();	
	
	for( i = 0; i < RP_MAX_EQUIP; ++i )
	{
		thePcInven.WearItem[i] = NULL;
	}
	for( i = 0; i < c_MaxInvenItemNum; ++i )
	{
		thePcInven.InvenItem[i] = NULL;
	}
	for( i = 0; i < RP_MAX_QUEST_ITEM; ++i )
	{
		thePcInven.InvenQuest[i] = NULL;
	}
	for( i = 0; i < RP_MAX_USER_SKILL; ++i )
	{
		thePcInven.SkillItem[i] = NULL;
	}
	for( i = 0; i < RP_MAX_QUICK_SLOT; ++i )
	{
		thePcInven.QuickSlot[i] = NULL;
	}
	for( i = 0; i < RP_MAX_QUEST_FLAG; ++i )
	{
		thePcInven.QuestFlag[i] = NULL;	
	}	
	for( i = 0; i < RP_MAX_QUEST_COUNT; ++i )
	{
		thePcInven.QuestCount[i] = NULL;	
	}
	for( i = 0; i < RP_MAX_PAY_CONTENTS; ++i )
	{
		thePcInven.PayEffect[i] = NULL;	
	}
	for( i = 0; i < RP_MAX_TIMELIMIT_PAY_CONTENTS; ++i )
	{
		thePcInven.PayTimeItemList[i] = NULL;
	}
	

	thePcInven.FrontierMyInfo = NULL;	
	thePcInven.FrontierInfo = NULL;	
	memset( thePcInven.FrontierCaller, 0, sizeof( char ) * RP_MAX_PC_NAME );		
	thePcInven.FrontierPetInfo = NULL;
	thePcInven.pComBineFrontierInfo = NULL;
	
	//
	for( i = 0; i < 3 ; ++i )
	{
		thePcInven.thePetData[i] = NULL;		
	}

	//-------------------------------------------------------------------------
	
	//하드웨어 커서를 지원하는지 여부 판단 -- 일단 주석처리
	//if(!g_RockClient.GetCaps()->CursorCaps)
	//if(32 != g_RockClient.m_wPixelFormat)
	//{
	//	m_IsSurpportHCursor = false;
	//}	
	
	if(m_IsSurpportHCursor)	//by simwoosung
	{
		if( !SetDeviceCursor(theCursor->img[0] , false) )
		{
			D3DUtil_SetDeviceCursor(g_lpDevice, LoadCursor(NULL, IDC_ARROW), false );
			if(g_RockClient.m_IsDeviceCursor)
			{
				g_RenderManager.ShowCursor( FALSE );
				g_RockClient.m_IsDeviceCursor = FALSE;
			}
			
			g_RenderManager.ShowCursor( TRUE );	
			g_RockClient.m_IsDeviceCursor = TRUE;	
		}
		
		ShowCursor(TRUE);
	}
	
	if( ( Desktop.Width == 800 ) && ( Desktop.Height == 600 ) )
		theResolution = n_800X600;
	else if( ( Desktop.Width == 1024 ) && ( Desktop.Height == 768 ) )
		theResolution = n_1024X768;

#ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/22)
	
	if(theResolution != n_1024X768)
	{
		m_fResolutionRateW = (Desktop.Width / 1024.0f) - 1.0f;// 폭 변동율
		m_fResolutionRateH = (Desktop.Height / 768.0f) - 1.0f;// 높이 변동율
	}
	
#endif // #ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/22)
	
	CompntCommon();	
	
	//콤보타이밍 연출클래스 초기화 by simwoosung
	m_Combo2DTimingEffect.InitEffect();
	//배틀존 진입 이펙트 연출 초기화
	m_BZoneIn2DEffect.InitEffect();
	
	//공지사항 클래스 초기화 by simwoosung
	g_Announcement.Init(&Desktop);
	
	for( i = 0; i < c_Max_ItemDelayInfo; ++i )
	{
		theItemDalayTime[i] = g_nowTime;
	}
	
	//캐릭터 아이디 렌더러 초기화
	m_CharIdRenderer.Init(thePcInfo, theFontg2);
	
	//엠블렌 텍스쳐리스트 로드
	g_EmblemTexManager.LoadEmblemFiles();
	
	return( true );
}

//-----------------------------------------------------------------------------
bool CRui::CreateWnd()
{
	return( true );
}

//-----------------------------------------------------------------------------
CWnd* CRui::CreateWnd( NWindowType WType, WNDID Iid, WNDID Pid, float Xpos, float Ypos, int Width, int Height, bool isRatio )
{
	CWnd* Wnd = NULL;
	
	switch( WType )
	{
		case n_wtWnd:				Wnd = SAFE_NEW( CWnd( &Desktop ) );					break;
		case n_wtFrameWnd:			Wnd = SAFE_NEW( CFrameWnd( &Desktop ) );			break;
		case n_wtButton:			Wnd = SAFE_NEW( CButton( &Desktop ) );				break;
		case n_wtButton9:			Wnd = SAFE_NEW( CButton9( &Desktop ) );				break;	
		case n_wtImageBox:			Wnd = SAFE_NEW( CImageBox( &Desktop ) );			break;
		case n_wtImageAni:			Wnd = SAFE_NEW( CImageAni( &Desktop ) );			break;
		case n_wtImageBox9:			Wnd = SAFE_NEW( CImageBox9( &Desktop ) );			break;
		case n_wtNoImageBox:		Wnd = SAFE_NEW (CNoImageBox(&Desktop) ) ;			break;
		case n_wtEblemImageBox:		Wnd = SAFE_NEW( CEmblemImageBox( &Desktop ) );		break;		
		case n_wtTextBox:			Wnd = SAFE_NEW( CTextBox( &Desktop ) );				break;
		case n_wtEditBox:			Wnd = SAFE_NEW( CEditBox( &Desktop ) );				break;
		case n_wtProgressBar:		Wnd = SAFE_NEW( CProgressBar( &Desktop ) );			break;
		case n_wtSlideBar:			Wnd = SAFE_NEW( CSlideBar( &Desktop ) );			break;
		case n_wtCheckButton:		Wnd = SAFE_NEW( CCheckButton( &Desktop ) );			break;
		case n_wtRadioButton:		Wnd = SAFE_NEW( CRadioButton( &Desktop ) );			break;
		case n_wtListBox:			Wnd = SAFE_NEW( CListBox( &Desktop ) );				break;
		case n_wtScrollBar:			Wnd = SAFE_NEW( CScrollBar( &Desktop ) );			break;
		case n_wtComboBox:			Wnd = SAFE_NEW( CComboBox( &Desktop ) );			break;	
		case n_wtSpin:				Wnd = SAFE_NEW( CSpin( &Desktop ) );				break;
		case n_wtStateButton:		Wnd = SAFE_NEW( CStateButton( &Desktop ) );			break;
		case n_wtChatOutput:		Wnd = SAFE_NEW( CChatOutput( &Desktop ) );			break;
		case n_wtSysMsgOutput:		Wnd = SAFE_NEW( CSysMsgOutput( &Desktop ) );		break;
		case n_wtSlot:				Wnd = SAFE_NEW( CSlot( &Desktop ) );				break;
		case n_wtMiniMap:			Wnd = SAFE_NEW( CMiniMap( &Desktop ) );				break;
		case n_wtUpGauge:			Wnd = SAFE_NEW( CUpGauge( &Desktop ) );				break;
		case n_wtChaImgButton:		Wnd = SAFE_NEW( CCharImgButton( &Desktop ) );		break;
		case n_wtFlashWnd:			Wnd = SAFE_NEW( CFlashWnd( &Desktop ) );			break;
		case n_wtItemTransButton:	Wnd = SAFE_NEW( CItemTransButton( &Desktop ) );		break;
		case n_wtComEffImgBox:		Wnd = SAFE_NEW( CComEffImgBox( &Desktop ) );		break;
		case n_wtComEffProgBar:		Wnd = SAFE_NEW( CComEffProgBar( &Desktop ) );		break;
		case n_wtComboStepBar:		Wnd = SAFE_NEW( CComboStepBar( &Desktop ) );        break;
		case n_wtFlashFrameWnd:		Wnd = SAFE_NEW( CFlashFrameWnd( &Desktop ) );       break;
		case n_wtLocalEntireMap:	Wnd = SAFE_NEW( CLocalEntireMap( &Desktop ) );      break;
		case n_wtMLineEditBox:		Wnd = SAFE_NEW( CMLineEditBox( &Desktop ) );        break;
		case n_wtLimitLenTextBox:	Wnd = SAFE_NEW( CLimitLenTextBox( &Desktop ) );     break;
		case n_wtFrameTextBox:		Wnd = SAFE_NEW( CFrameTextBox( &Desktop ) );        break;
		case n_wtChatOutList:		Wnd = SAFE_NEW( CChatOutList(&Desktop) );			break;
		case n_wtSystemOutList:		Wnd = SAFE_NEW( CSystemOutList(&Desktop) );			break;	
		case n_wtChatEditBox:		Wnd = SAFE_NEW( CChatEditBox(&Desktop) );   	    break;
		case n_wtRITextBox:			Wnd = SAFE_NEW( CRITextBox(&Desktop) );             break;
		case n_wtCharImgBox:		Wnd = SAFE_NEW( CCharImgBox(&Desktop) );		    break;
		case n_wtArrowLineList:		Wnd = SAFE_NEW( CArrowLineList(&Desktop) );			break; 
	}
	
	
	if( Wnd == NULL ){	return( NULL );	}

#ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/22)
	
	if(WType == n_wtFrameWnd)
	{
		int iCenterX = Desktop.Width >> 1;
		int iCenterY = Desktop.Height >> 1;
		
		int iFrameLeft = iCenterX - (Width >> 1);
		int iFrameTop = iCenterY - (Height >> 1);
		
		float fExtentW = 0.0f, fExtentH = 0.0f;
		
		if(Desktop.Width == Width || Desktop.Height == Height)// 전체화면
			Wnd->m_ucCoordStd = 0;
		
		else if(Desktop.Width != Width && Xpos == iFrameLeft)// 화면중앙
			Wnd->m_ucCoordStd = 1;
		
		else// 좌상단
			Wnd->m_ucCoordStd = 2;
	}
	
#endif // #ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/22)
	
	Wnd->CreateWnd( WType, Iid, Pid, Xpos, Ypos, Width, Height, isRatio );
	Container->Add( Wnd );
	
	if( (Pid == WID_DeskTop) || (Pid == WID_None) )//Desktop
	{
		Chain.Add( Wnd );
	}
	else
	{
		CFrameWnd* Pwnd = (CFrameWnd*)Container->Get( Pid );
		Pwnd->RegisterChild( Wnd->GetID() );// WID_None일 경우...ID를 자동 생성하기 때문에...반드시 ID를 얻어야 한다.
	}
	
	return( Wnd );
}

#ifdef UI_CONTROL_RENEWAL

CWnd* CRui::CreateControl( DWORD Defineid ) 
{
	CWnd* wnd = NULL;

	LPUI_DATA pData = NULL;

	pData = g_UiDataManager.GetData( Defineid ); // ui 데이터 정보 받아옴

	wnd = (CButton*)CreateWnd( (NWindowType)pData->dCtrltype, pData->dIid, pData->dPid, 
		             pData->dUi_X, pData->dUi_Y, pData->dUi_W, pData->dUi_H, pData->dUi_Option ); 

	if ( pData->dImage > -1 )
	{
		wnd->SetClientImage( pData->dImage );
	}
	
	if ( pData->dVisible > -1 )
	{
		wnd->SetVisible( pData->dVisible );
	}	
	
	return( wnd );
}

CWnd* CRui::CreateControlRepeetY( DWORD Defineid, DWORD Count, DWORD Num )
{
	CWnd* wnd = NULL;
	
	LPUI_DATA pData = NULL;
	
	// ui 데이터 정보 받아옴
	pData = g_UiDataManager.GetData( Defineid ); 
	
	wnd = (CWnd*)CreateWnd( (NWindowType)pData->dCtrltype, pData->dIid, pData->dPid, 
		pData->dUi_X, pData->dUi_Y + (Num * Count), pData->dUi_W, pData->dUi_H, false ); 
	
	if ( pData->dImage > -1 )
	{
		wnd->SetClientImage( pData->dImage );
	}
	
	if ( pData->dVisible > -1  )
	{
		wnd->SetVisible( pData->dVisible );
	}	
	return( wnd );
}

CWnd* CRui::CreateControlRepeetX( DWORD Defineid, DWORD Count, DWORD Num )
{
	CWnd* wnd = NULL;
	
	LPUI_DATA pData = NULL;
	
	// ui 데이터 정보 받아옴
	pData = g_UiDataManager.GetData( Defineid ); 
	
	wnd = (CWnd*)CreateWnd( (NWindowType)pData->dCtrltype, pData->dIid, pData->dPid, 
		pData->dUi_X + (Num * Count), pData->dUi_Y, pData->dUi_W, pData->dUi_H, false ); 
	
	if ( pData->dImage > -1 )
	{
		wnd->SetClientImage( pData->dImage );
	}
	
	if ( pData->dVisible > -1  )
	{
		wnd->SetVisible( pData->dVisible );
	}	
	
	return( wnd );
}


CWnd* CRui::CreateControlGetID( DWORD Defineid,  WNDID Pid )
{
	CWnd* wnd = NULL;
	
	LPUI_DATA pData = NULL;
	
	// ui 데이터 정보 받아옴
	pData = g_UiDataManager.GetData( Defineid ); 
	
	wnd = (CWnd*)CreateWnd( (NWindowType)pData->dCtrltype, pData->dIid, Pid, 
		pData->dUi_X, pData->dUi_Y, pData->dUi_W, pData->dUi_H, false ); 
	
	if ( pData->dImage > -1 )
	{
		wnd->SetClientImage( pData->dImage );
	}
	
	if ( pData->dVisible > -1  )
	{
		wnd->SetVisible( pData->dVisible );
	}	
	
	return( wnd );
}

#endif //UI_CONTROL_RENEWAL

//-----------------------------------------------------------------------------
IRESULT CRui::SendEvent( WNDID Wid, NEventMessage Event, EPARAM fParam, EPARAM sParam, EPARAM xParam, EPARAM exParam )
{
	CWnd* Wnd = NULL;
	
	if( (Wid == WID_DeskTop) || (Wid == WID_None) )
	{
		return( ReceiveEvent( Event, fParam, sParam, xParam, exParam ) );
	}
	
	Wnd = Container->Get( Wid );
	if( Wnd == NULL ){	return( -1 );	}
	
	return( Wnd->ReceiveEvent( Event, fParam, sParam, xParam, exParam ) );
}

//-----------------------------------------------------------------------------
IRESULT CRui::ReceiveEvent( NEventMessage Event, EPARAM fParam, EPARAM sParam, EPARAM xParam, EPARAM exParam )
{
	int Length = 0;
	TCHAR* Str = NULL;
	
	
	switch( Event )
	{
		//case emDT_ChangeImage;
		//	break;
	case n_emTextOutput:
		Str = (TCHAR*)fParam;
		
		if( ( Str == NULL ) || ( ( Length =Rstrlen( Str ) ) == 0 ) )
		{
			return( 0 );
		}
		
		if( ( Length < 256 ) && ( theNumMsg < 256 ) )
		{
			Rstrcpy( RWCHAR(theMessage[theNumMsg].Text), RWCHAR(Str) );
			theMessage[theNumMsg].x		= (int)sParam;
			theMessage[theNumMsg].y		= (int)xParam;
			theMessage[theNumMsg].Color	= (D3DCOLOR)exParam;
			theNumMsg++;
		}
		break;
		
	case n_emMessageBox:
		//SetModal( WID_MsgBox_Ok );
		
		//SetZorder( )
		//Chain.Ordering( Container->Get( WID_MsgBox_Text ) );
		//SendEvent( WID_MsgBox_Text, n_emSetText, sParam, 255, 255, 255 );
		//SendEvent( WID_MsgBox_Ok, n_emSetCaption, xParam, 0, 0, 0 );
		//SendEvent( WID_MsgBox_Ok, n_emSetVisible, true, 0, 0, 0 );
		
		break;
		
	case n_emChangeCursor:
		//Cursor.Img = fParam;
		break;
		
	case n_emResetFont:
		//Font->ResetFont( (LOGFONT*)fParam );
		break;
		
	case n_emBackGround:
		
		theBGround.IsAction	= (bool)fParam;
		theBGround.Img		= (TEXID)sParam;
		break;
		
	case n_emForeGround:
		break;
	}
	
	return( 0 );
}

//-----------------------------------------------------------------------------
void CRui::ModeInitialize( DWORD gamemode )
{
	switch( gamemode )
	{
	case GMODE_LOGINPAGE:
		break;
		
	case GMODE_NORMAL:
		///SendEvent( WID_MainMenuLv_Wnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
		g_ChatMainProc.SetOutState(COutState_NonAct);
		SendEvent( WID_SystemPrintFrame, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
		
		SendEvent( WID_MainMenu_Wnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
		SendEvent( WID_MainButtons_Wnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
#ifdef TAIWAN30MINEVENT
		SendEvent( WID_MainEventButtons_Wnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
#endif //TAIWAN30MINEVENT
		SendEvent( WID_MainExp_Wnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
		SendEvent( WID_MAIN_ENDU_WARN, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
		SendEvent( WID_MainMenu_Wnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );	
		
		SendEvent( WID_PlayerInfo_Wnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
		SendEvent( WID_AddEffectMsgFrame , n_emSetVisible, (EPARAM)true, 0, 0, 0 );

		PlayerInfoAddWnd.Open();
		SendEvent( WID_MiniMapWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
		
		//SendEvent( WID_ChatMessageWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );		
		
		SetEnterFlag( n_nfUIEnter );
		//SendEvent( WID_ChatBoxWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
		//SendEvent( WID_ChatOutputWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
		break;
	}
}

//-----------------------------------------------------------------------------
void CRui::TextOut( void* str, int x, int y, D3DCOLOR color )
{
	ReceiveEvent( n_emTextOutput, (EPARAM)str, x, y, color );
}

//-----------------------------------------------------------------------------
void CRui::TextOutChat( void* str, D3DCOLOR color )
{
	ChatOutputWnd.ChatTxt->SetChatItem( str, color );
	ChatMessageWnd.ChatTxt->SetChatItem( str, color );
	ChatMessageWnd.Step += 1;
	ChatMessageWnd.ChatTxt->SetOutputStep( ChatMessageWnd.Step );
}

//-----------------------------------------------------------------------------
void CRui::MessageBox( int msg_id, TCHAR* text, TCHAR* caption, TEXID icon_type )
{
	ReceiveEvent( n_emMessageBox, msg_id, (EPARAM)text, (EPARAM)caption, (EPARAM)icon_type );
}

//................................................................................................................
// 말풍선 지우기
//................................................................................................................
void CRui::DeleteDlgBalloon( NCHARTYPE ctype, DWORD id )
{
	for( int i = 0; i < MAX_DLGBALLOON; ++i )
	{
		if( theDlgBalloon[i] != NULL )
		{
			if( theDlgBalloon[i]->IsExistence( ctype, id ) )
			{
				theDlgBalloon[i]->Die();
			}
		}
	}
}


//-----------------------------------------------------------------------------
void CRui::SetDlgBalloon( NCHARTYPE ctype, DWORD id )
{
	for( int i = 0; i < MAX_DLGBALLOON; ++i )
	{
		if( theDlgBalloon[i] != NULL )
		{
			if( theDlgBalloon[i]->IsExistence( ctype, id ) )
			{
				theDlgBalloon[i]->Die();
			}
		}
	}
	
	CDlgBalloon* DlgItem = SAFE_NEW( CDlgBalloon( &Desktop ) );	
	
	DlgItem->Set( ctype, id, thePcInfo, Resource->GetBorderInfo() );
	
	if( theNumDlg >= MAX_DLGBALLOON )
	{
		SAFE_DELETE(theDlgBalloon[0]);	
		
		for( int i = 0; i < (MAX_DLGBALLOON - 1); ++i )
		{
			theDlgBalloon[i] = theDlgBalloon[i + 1];
		}
		
		theDlgBalloon[theNumDlg - 1] = DlgItem;
	}
	else
	{
		theDlgBalloon[theNumDlg] = DlgItem;
		theNumDlg++;
	}
}

//-----------------------------------------------------------------------------
void CRui::SortDlgBalloon()
{
	int i = 0;
	for( i = 0; i < MAX_DLGBALLOON; ++i )
	{
		if( theDlgBalloon[i] != NULL )
		{
			if( !theDlgBalloon[i]->IsLife() )
			{
				SAFE_DELETE(theDlgBalloon[i]);	
			}
		}
	}
	
	static CDlgBalloon* tmpDlgBalloon[MAX_DLGBALLOON];
	int count = 0;
	for( i = 0; i < MAX_DLGBALLOON; ++i )
	{
		if( theDlgBalloon[i] != NULL )
		{
			tmpDlgBalloon[count] = theDlgBalloon[i];
			count++;
		}
		
		theDlgBalloon[i] = NULL;
	}
	
	theNumDlg = count;
	for( i = 0; i < count; ++i )
	{
		theDlgBalloon[i] = tmpDlgBalloon[i];
	}
	
	return;
}

void CRui::InitDlgBalloon()
{
	// 말풍선 초기화
	for( int i = 0; i < MAX_DLGBALLOON; ++i )
	{	
		SAFE_DELETE(nRui->theDlgBalloon[i]);
	}
	
	return;
}

//-----------------------------------------------------------------------------
void CRui::SetModal( WNDID modal_wid )
{
	ModalWnd = modal_wid;
}
///---------------------------------------------------------------------------
///-- ItemPickUp
///---------------------------------------------------------------------------
void CRui::ItemPickUp()
{
	///-----------------------------------------------------------------------
	///-- Item 자동 루팅
	if( g_Pc.GetPlayer()->GetAddEffectState(n_TypeProvoke))
	{
		return;
	}
	if( g_Pc.GetPlayer()->GetAddEffectState(n_TypeNotAttack))
	{
		return;
	}

	///-- Skill 을 사용할 때 CM_MOVE_ATTACK 금지	
	if( g_Pc.GetPlayer()->m_curt_event.type == SM_ATTACK_DELAY && g_Pc.GetPlayer()->m_curt_event.tbl_Idx == g_Map.m_nPickedNpc ||
		g_Pc.GetPlayer()->m_save_event.type == SM_MOVE_ATTACK && g_Pc.GetPlayer()->m_save_event.tbl_Idx == g_Map.m_nPickedNpc ||
		( g_Pc.GetPlayer()->m_curt_event.type == SM_USE_SKILL &&
		g_Pc.GetPlayer()->m_curt_event.status != SM_RESTSKILL_SITDOWN &&
		g_Pc.GetPlayer()->m_curt_event.status != SM_RESTSKILL_KEEPING &&
		g_Pc.GetPlayer()->m_curt_event.status != SM_RESTSKILL_STANDUP ) )	// 휴식 스킬 (유지)	
	{
		return;
	}
	
	if( g_Pc.GetPlayer()->m_curt_event.type == SM_USE_SKILL && 
		( g_Pc.GetPlayer()->m_curt_event.status == SM_SELLSKILL_SITDOWN ||
		g_Pc.GetPlayer()->m_curt_event.status == SM_SELLSKILL_KEEPING ||
		g_Pc.GetPlayer()->m_curt_event.status == SM_SELLSKILL_STANDUP    ) )
	{
		return;
	}
	
	
	short ls_short_dist_index = 0;
	float lf_min_distance = 100.0f;		///-- 인식 가능 거리 100.0f
	float lf_distance = 0.0f;
	float lf_x, lf_z;
	
	for ( short i = MAX_NPC_INTABLE - 1 ; i >= 0 ; --i )
	{
		if( g_Map.m_NpcTable[i].race == c_NpcRace_Item )
		{
			lf_x = g_Map.m_Npc[i].GetPosTM()._41 - g_Pc.GetPlayer()->GetPosTM()._41; 
			lf_z = g_Map.m_Npc[i].GetPosTM()._43 - g_Pc.GetPlayer()->GetPosTM()._43; 
			
			lf_distance = sqrt( ( double ) ( lf_x * lf_x + lf_z * lf_z ) );
			
			
			///-- 목표 아이템 설정
			if( lf_min_distance > lf_distance )
			{
				lf_min_distance = lf_distance; 
				ls_short_dist_index = i;
			}
		}
	}
	
	///-- 인식 가능 거리 안에 아이템이 있다면
	
	
	if( lf_min_distance != 100.0f )
	{
		Sleep(1);
		EVENT_DATA_INFO pickNPC_event;
		pickNPC_event.type = CM_MOVE_ATTACK;
		pickNPC_event.motion = n_Attack9_Basic0;
		pickNPC_event.tbl_Idx = ls_short_dist_index;
		pickNPC_event.race	 = g_Map.m_NpcTable[ls_short_dist_index].race;
		pickNPC_event.index	 = g_Map.m_NpcTable[ls_short_dist_index].index;
		pickNPC_event.unique = g_Map.m_NpcTable[ls_short_dist_index].lUnique;
		pickNPC_event.x		 = g_Map.m_NpcTable[ls_short_dist_index].lDestBlock%(g_Map.m_cSize*2);
		pickNPC_event.y		 = g_Map.m_NpcTable[ls_short_dist_index].lDestBlock/(g_Map.m_cSize*2);
		pickNPC_event.vPos.x = g_Map.m_Npc[ls_short_dist_index].GetPosTM()._41;
		pickNPC_event.vPos.y = g_Map.m_Npc[ls_short_dist_index].GetPosTM()._42;
		pickNPC_event.vPos.z = g_Map.m_Npc[ls_short_dist_index].GetPosTM()._43;
		pickNPC_event.cell = g_Map.m_NpcTable[ls_short_dist_index].lDestBlock; 
		g_Pc.GetPlayer()->PushQueue( pickNPC_event, TRUE );
		return;
	}
	
}
///---------------------------------------------------------------------------
///-- KeyEventProc
///---------------------------------------------------------------------------
void CRui::KeyEventProc()
{
	SRect size;
	static int ctype_count = n_CT_NORMAL;
	static int store_count = 1; 
	static bool IsWnd = false;
	
	
	if( g_RockClient.m_bIsDrawFullScreenUI || 
		!g_RockClient.m_SceneManager.GetKeyInput() )
	{
		if( g_WebShopWnd.IsVisible ) 
		{
			if( theEMsg.Keys[DIK_ESCAPE].Action == n_baDown )
			{
				g_WebShopWnd.Close();
			}
		}
		return;
	}
	
	//////////////////////////////////////////////////////////////////////////
	// 개발자가 사용하는 키 
	//////////////////////////////////////////////////////////////////////////
#ifdef ROCKCLIENT_DEV
	if(g_RockClient.m_EtcInfo.DevMode == 1)
	{
		TestKeyProc(IsWnd);
	}

#endif
	
	
	// 모달 메세지 박스
	//-------------------------------------------------------------------------
	//if( MessageBoxWnd.IsVisible ){	return;	}
	if( ModalWnd != WID_None )
	{
		if( ( theEMsg.Keys[DIK_RETURN].Action == n_baUp ) || ( theEMsg.Keys[DIK_NUMPADENTER].Action == n_baUp ) )
		{
			// 아이템 사기,팔기 창이 떠 있는지 확인( 창 띄울때...에디터 포커스 죽이기 )
			// 있다면....사기,팔기창 처리 하고 리턴
			if( StoreBuyWnd.IsVisible == true )
			{
				StoreBuyWnd.OkClose();
				return;
			}
			else if( StoreSellWnd.IsVisible == true )
			{
				StoreSellWnd.OkClose();
				return;
			}
			else if(g_MessageBox.IsVisible == true )	
			{
				nRui->SetModal( WID_None );
				nRui->SendEvent( WID_MessageBox, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
				g_MessageBox.OkClose();
				return;
				
			}
		}
		return;
	}
	
	if( g_RockClient.GetGameMode() == GMODE_NORMAL )
	{
		if( theEMsg.Keys[DIK_ESCAPE].Action == n_baDown )
		{		
			//펫 생성에서 ESC 눌를 경우 서버에 메세지를 보낸다 . 
			if( g_PetCreateWnd.IsVisible)
			{
				g_RockClient.Send_CsMakePetName("",true);					
			}	
			else if(g_ItemEldaMixRWndProc.IsVisible )
			{
								
			}
			else if( g_RockClient.m_MouseControlType == n_MouseType_UseSkill )		// 지속성 지역 마법을 시전할 경우
			{
				g_RockClient.m_MouseControlType = n_MouseType_Normal;
				
				g_Pc.GetSkillSystem()->theTargetLand_SkillCode = 0;				
				g_Particle.m_EffectManager.SetLandMark( FALSE, 0.0f );				

				g_Pc.GetSkillSystem()->theUseSpellSkill = FALSE;
			}
			else if( g_DelayBarWnd.IsVisible )
			{
				g_DelayBarWnd.Clear();
			}
			else if( NonPlayerInfoWnd.IsVisible && !g_Pc.GetPlayer()->GetAddEffectState(n_TypeProvoke) )
			{
				nRui->SendEvent( WID_NonPlayerInfo_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );			
				NonPlayerInfoWnd.thePickedNpcID = -1;
				NonPlayerInfoWnd.thePickedUserID = -1;
				NonPlayerInfoWnd.SetPickCharacter( NULL );
				
				if( NonPlayerInfoAddWnd.IsVisible )
				{
					nRui->SendEvent( WID_NonPlayerInfoAddWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
					if(g_NonPlayerInfoAdd2Wnd.IsVisible)
					{
						g_NonPlayerInfoAdd2Wnd.Close();
					}			
				}
			}
			else if( Chain.CloseVisibleWnd() == false )							// 현재 떠 있는 창들을 위에서부터 하나씩 닫기 
			{
				IsWnd = !SystemMenuWnd.IsVisible;
				if(IsWnd)
				{
					SystemMenuWnd.Open(g_Pc.GetPlayer()->IsMyPlayerRender);
				}
				else
				{
					SystemMenuWnd.Close();
				}				
			}
		}
	}
	
	// 포커스가 EditBox에 있으면... KeyEvent는 처리하지 않는다.
	//-------------------------------------------------------------------------
	if( IsEditFocus() == true )
	{	
		return;
	}
	//-------------------------------------------------------------------------
	
	//-------------------------------------------------------------------------
	//BY simwoosung 에디트 창에 포커스가 맞추어 있을경우는 바로 위에서 걸러진다.
	//-------------------------------------------------------------------------	
	//현재 게임윈도우가 활성윈도우일경우 
	if( g_RockClient.GetApp() ==  ::GetActiveWindow() ) 
	{
		HIMC  hIMC;
		DWORD  dwConversion;
		DWORD  dwSentence;
		
		if( (hIMC = ImmGetContext( g_RockClient.GetApp() ) ) ) //IME 핸들얻어오기			
		{
			ImmGetConversionStatus( hIMC, &dwConversion, &dwSentence);
			if(dwConversion & IME_CMODE_NATIVE) // 고유문자 모드면...
			{
				//영문 전환
				ImmSetOpenStatus( hIMC, FALSE);
				ImmSetConversionStatus( hIMC, IME_CMODE_ALPHANUMERIC, IME_SMODE_NONE);
				ImmReleaseContext( g_RockClient.GetApp(), hIMC );
				///--PRINT_DLG_LOG("한글 영어로 전환");
			}
		}	
	}
	//-------------------------------------------------------------------------	
	
	// KeyEvent처리....
	if( g_RockClient.GetGameMode() == GMODE_NORMAL )
	{		
		//////////////////////////////////////////////////////////////////////////
		// 유져가 사용하는키 
		//////////////////////////////////////////////////////////////////////////

		UserKeyProc();		//일반키 
	//	CtrlUserKeyProc(IsWnd);	//컨트롤 + N 한국부터 테스트 ...
	}
	
	
}

//-----------------------------------------------------------------------------
N_EVENTRESULT CRui::EventProc( SKeys* Keys, SMouse* Mouse )
{
	if( !g_RockClient.m_SceneManager.GetGUIDisplay() )
		return (n_ERESULT_NONE);
	
	CWnd* Twnd = WID_None;//Temp Window Handle
	CWnd* Swnd = WID_None;//Selected Window Handle
	CWnd* Fwnd = WID_None;
	CWnd* Pwnd = WID_None;
	
	bool is_modal_proc = false;	
	
	//입력(마우스, 키보드) 예외 처리
	if( ( Keys == NULL ) || ( Mouse == NULL ) )
	{
		return( n_ERESULT_ERROR );
	}
	
	///-----------------------------------------------------------------------
	///-- Mini_Game
	if( m_bOnMiniGame && theMiniGame)
	{
		///-- minigame
		theMiniGame->EventProc( Keys, Mouse );
		Mouse->uiLButton = n_baNone;
		Mouse->LButton = n_baNone;
		return ( n_ERESULT_UIPROC );		
	}
	
	//포커스 윈도우 찾기
	
	
	FocusWnd = FindFocusWindow( Mouse->Aposition.x, Mouse->Aposition.y, true );
	Desktop.CurFocusWnd = FocusWnd;	
	
	
	MakeEventMessage( Keys, Mouse, FocusWnd );//입력 상태를 이벤트 메세지로 변환
	KeyEventProc();
#ifdef _DEBUG
	/*
	if( nRui->m_EtcInfo.DevMode == 1)
	{
		if( FocusWnd != WID_None )
		{
			
			CWnd *rWnd = Container->Get( FocusWnd );			
			CWnd *pWnd = NULL;
			
			if( rWnd->GetParentID() != WID_None )
			{
				pWnd = rWnd->GetParentHandle();
			}
			else
			{
				pWnd = rWnd;
			}
			
			WORD WinType = pWnd->GetWindowType();		
			if(WinType == n_wtFrameWnd)
			{
				IPOS2 pos;
				pWnd->GetScenePos(pos);
				
				char buf[128] = {0,};
				Rsprintf( RWCHAR(buf), _RT("MousePos [X:%d] [Y:%d]"), g_RockClient.theMouse.Aposition.x -pos[0] , g_RockClient.theMouse.Aposition.y - pos[1] );		
				nRui->TextOut( buf, (nRui->Desktop.Width>>1) + 250, 40, D3DCOLOR_ARGB( 255, 255, 200, 200 ) );
			}
			
		}
	}
	*/
#endif
	if(g_RockClient.m_IsMouseRotate)
	{
		return n_ERESULT_UIPROC;
	}
	
	if( ModalWnd != WID_None  )
	{
		if( FocusWnd != WID_None )
		{
			Twnd = Container->Get( FocusWnd );
			WNDID wndid = Twnd->GetParentID();
			if( wndid == NULL )
			{
				if( FocusWnd == ModalWnd ){	is_modal_proc = true;	}
			}
			else
			{
				if( wndid == ModalWnd ){	is_modal_proc = true;	}
			}
			
			if( is_modal_proc )
			{
				Twnd = Container->Get( ModalWnd );
				Twnd->EventProc( &theEMsg );
				
				if( ( theEMsg.Mouse.Action == MA_LDROP ) || ( theEMsg.Mouse.Action == MA_LUP ) )
				{
					Desktop.SelectedWnd = WID_None;
				}
			}
		}
		
		return( n_ERESULT_UIPROC );
	}
	else if( Desktop.SelectedWnd != WID_None )
	{
		if(FocusPwnd == WID_None)
			theEMsg.Notify = NM_OUTRANGE;
		
		Swnd = Container->Get( Desktop.SelectedWnd );
		Swnd->EventProc( &theEMsg );
		
		if(FocusPwnd == WID_None)
			theEMsg.Notify = 0;
		
		if( ( theEMsg.Mouse.Action == MA_LDROP ) || ( theEMsg.Mouse.Action == MA_LUP ) )
		{
			Desktop.SelectedWnd = WID_None;
		}
		
		return( n_ERESULT_UIPROC );
	}
	else if( (Desktop.SelectedWnd == WID_None) && (theEMsg.Mouse.Action == MA_LDRAG) )
	{
		//이상태를 거른다. 현재는 아무동작하지않음
		int a = 0;
	}
	else if( FocusPwnd != WID_None )
	{
		//Mouse LButton Down 일때.... Ordering
		if( theEMsg.Mouse.uiLButton == n_baDown )
		{
			//by simwoosung 수정
			CWnd *pWnd = Container->Get( FocusPwnd );			
			WORD WinType = pWnd->GetWindowType();		
						
			Chain.Ordering( pWnd );			
			
			if(WinType == n_wtFrameWnd)
			{
				CFrameWnd *pLinkFrame = ((CFrameWnd *)pWnd)->GetMoveLinkFrameWnd();
				if(pLinkFrame)
				{
					Chain.Ordering( (CWnd *)pLinkFrame );
				}
				
				CFrameWnd *pLinkFrame2 = ((CFrameWnd *)pWnd)->GetMoveLinkFrameWnd2();
				if(pLinkFrame2)
				{
					Chain.Ordering( (CWnd *)pLinkFrame2 );
				}
			}
		}
		
		Fwnd = Container->Get( FocusWnd );
		
		if( Fwnd )
		{
			//부모 윈도우가 있다는건...프레임윈도우라는 뜻...
			//프레임윈도우로 메세지를 보내서.... 자식들까지..처리...
			if( Fwnd->GetParentID() != WID_None )
			{
				Pwnd = Fwnd->GetParentHandle();
				Pwnd->EventProc( &theEMsg );
			}
			else
			{
				Fwnd->EventProc( &theEMsg );
			}
		}		
		return( n_ERESULT_UIPROC );
	}
	else if( FocusPwnd == WID_None )
	{
		if( ( GetCursorItem() != NULL ) && ( theEMsg.Mouse.Action == MA_LUP ) )
		{
			return( n_ERESULT_ITEM_DROP );
			//SetCursorItem( NULL );
			//MessageBox( NULL, "아이템 버리기( SERVER PROC )", "SLOT", MB_OK );
		}
	}
	
	return( n_ERESULT_NONE );
}

void CRui::BillUIRenderProc()
{
	Render->SetState();		
	
	DrawGameUI();
	DrawCharacterID();
	DrawDlgBalloon();
	
	//	Render->End();
	Render->ReleaseState();
}
///---------------------------------------------------------------------------
///-- RenderLogo
///---------------------------------------------------------------------------
///-- 처음 한번만 Logo 를 Display 한다.
extern	Texture		g_LogoTex;
void CRui::RenderLogo(int nAlpha)
{
	
	////-- 배경 이미지를 그려준다.
	STexture	tmpTex;

	// set vertex format	
	g_RenderManager.SetFVF( RUI_VERTEX_FORMAT );
	
	g_RenderManager.SetZEnable( FALSE );
	g_RenderManager.SetLighting( FALSE );
	g_RenderManager.SetFogEnable( FALSE );
	
	g_RenderManager.SetTextureMipFilter( 0 , D3DTEXF_NONE  );
	g_RenderManager.SetTextureMagFilter( 0 , D3DTEXF_POINT );
	g_RenderManager.SetTextureMinFilter( 0 , D3DTEXF_NONE  );
	
	g_RenderManager.SetTextureColorOP( 0 , D3DTOP_SELECTARG1 );
	g_RenderManager.SetTextureColorArg1( 0 , D3DTA_TEXTURE );
	g_RenderManager.SetTextureColorArg2( 0 , D3DTA_DIFFUSE );
	
	g_RenderManager.SetTextureAlphaOP( 0 , D3DTOP_SELECTARG1 );
	g_RenderManager.SetTextureAlphaArg1( 0 , D3DTA_DIFFUSE );
	g_RenderManager.SetTextureAlphaArg2( 0 , D3DTA_TEXTURE );
	
	g_RenderManager.SetAlphaTestEnable( FALSE );
	
	if( nAlpha == 255 )
	{
		g_RenderManager.SetAlphaBlendEnable( FALSE );
	}
	else
	{
		g_RenderManager.SetAlphaBlendEnable( TRUE );
	}
	
	tmpTex.vt[0].diffuse	= D3DCOLOR_ARGB( nAlpha, 255, 255, 255 );
	tmpTex.vt[1].diffuse	= D3DCOLOR_ARGB( nAlpha, 255, 255, 255 );
	tmpTex.vt[2].diffuse	= D3DCOLOR_ARGB( nAlpha, 255, 255, 255 );
	tmpTex.vt[3].diffuse	= D3DCOLOR_ARGB( nAlpha, 255, 255, 255 );

#ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/22)
	
	tmpTex.vt[0].x =  - 0.5f;
	tmpTex.vt[0].y =  - 0.5f;
	
	tmpTex.vt[1].x = tmpTex.vt[0].x + Desktop.Width;
	tmpTex.vt[1].y = tmpTex.vt[0].y;
	
	tmpTex.vt[2].x = tmpTex.vt[0].x + Desktop.Width;
	tmpTex.vt[2].y = tmpTex.vt[0].y + Desktop.Height;
	
	tmpTex.vt[3].x = tmpTex.vt[0].x;
	tmpTex.vt[3].y = tmpTex.vt[0].y + Desktop.Height;
	
	tmpTex.vt[0].tu = 0.0f;
	tmpTex.vt[1].tu = 1.0f;
	tmpTex.vt[2].tu = 1.0f;
	tmpTex.vt[3].tu = 0.0f;
	
	tmpTex.vt[0].tv = 0.0f;
	tmpTex.vt[1].tv = 0.0f;
	tmpTex.vt[2].tv = static_cast<float>(Desktop.Height) / Desktop.Width;
	tmpTex.vt[3].tv	= tmpTex.vt[2].tv;
	
#else
	
	tmpTex.vt[0].x =  - 0.5f;
	tmpTex.vt[0].y =  - 0.5f;		
	tmpTex.vt[1].x = tmpTex.vt[0].x + 1024.0f;
	tmpTex.vt[1].y = tmpTex.vt[0].y;
	tmpTex.vt[2].x = tmpTex.vt[0].x + 1024.0f;
	tmpTex.vt[2].y = tmpTex.vt[0].y + 768.0f;
	tmpTex.vt[3].x = tmpTex.vt[0].x;
	tmpTex.vt[3].y = tmpTex.vt[0].y + 768.0f;
	
	tmpTex.vt[0].tu = 0.0f;
	tmpTex.vt[1].tu = 1.0f;
	tmpTex.vt[2].tu = 1.0f;
	tmpTex.vt[3].tu = 0.0f;
	
	tmpTex.vt[0].tv = 0.0f;
	tmpTex.vt[1].tv = 0.0f;
	tmpTex.vt[2].tv = 768.0f / 1024.0f;
	tmpTex.vt[3].tv	= tmpTex.vt[2].tv;
	
	
	tmpTex.vt[1].x = tmpTex.vt[0].x + 1024.0f;
	tmpTex.vt[1].y = tmpTex.vt[0].y;
	
#endif // #ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/22)

	g_RenderManager.SetTexture( 0 , g_LogoTex.m_pD3DTex );
	g_RenderManager.DrawPrimitiveUP( D3DPT_TRIANGLEFAN , 2, tmpTex.vt, sizeof( SVertex ) );	
	
#ifdef APPLY_INTRO_ANI
	
	m_Logo2DEffect.Render2DEffect( nAlpha );
	
#endif	
	
	g_RenderManager.DisableTexture( 0 );
	
	g_RenderManager.SetZEnable( TRUE );
	g_RenderManager.SetFogEnable( TRUE );
	g_RenderManager.SetLighting( TRUE );
	
}

//-----------------------------------------------------------------------------
void CRui::RenderProc()
{
	if( g_RockClient.m_bGameEnd )
	{
		return;
	}
	
	GetTimeFlag();
	
	//-----------------------------------------------------------------------//
	// * 장면 렌더링( 배경 -> 말풍선 -> 윈도우 컨트롤 -> 툴팁 -> 커서 -> 전경(페이딩용) )
	//-----------------------------------------------------------------------//
	
	if( m_bIsShowUI && SUCCEEDED( g_RockClient.BeginScene() ) ) 
	{
		Render->SetState();// * 렌더링 상태 저장 & UI렌더링 상태 설정
		
		m_Combo2DTimingEffect.RenderProc();
		m_BZoneIn2DEffect.RenderProc();
		
		DrawBackGround();
		
		if( IsInfo == true ){	DrawInfo();		}
		//---------------------------------------------------------------------
		//DrawPoint();
		
		DrawSystemMsg();
		
		DrawWindows();
		
		//if( g_ArcherBar.ISVisible() )
		g_ArcherBar.RenderProc();
		
		DrawToolTip();
		DrawMessage();
		DrawForeGround();	

		g_UserTime.Update();

		///-------------------------------------------------------------------
		///-- Mini Game
		if( m_bOnMiniGame && theMiniGame)
		{
			theMiniGame->RenderProc();
			///-- Game 종료
			if( theMiniGame->theGameMode == n_M_Destroy)
			{
				m_bOnMiniGame = false;
				MiniGameEnd( 0 );				
			}
		}
		
		//공지사항 - UI에서 가장 마지막에 그려져야 한다. 
		g_Announcement.RenderProc();
		
		// IME 변환 스트링 표시 
		DrawIMECovert(); 
		
		DrawCursor();
		
		Render->ReleaseState();	// * 렌더링 상태 복구
		
		g_RockClient.EndScene();
	}
	
}



void CRui::LastRenderProc()
{
	if( SUCCEEDED( g_RockClient.BeginScene() ) ) 
	{
		Render->SetState();// * 렌더링 상태 저장 & UI렌더링 상태 설정
		
		DrawCursor();
		
		Render->ReleaseState();	// * 렌더링 상태 복구
		
		g_RockClient.EndScene();
	}
	
	return;
}

bool CRui::SetDeviceCursor(UINT Tid, bool IsItem)
{
	#ifdef DIRECT_VERSION_9_MJH
	// TID 가 n_CT_NONE 이면 리턴 false         MJH
	if ( Tid == 2179 )
	{
		return false;
	}
	#endif // DIRECT_VERSION_9_MJH
    if(m_pCursorBitmap)
	{
		m_pCursorBitmap->Release();
		m_pCursorBitmap = NULL;
	}
	
	STexture*	Tex = Resource->GetTexture( Tid );
	STexture	tmpTex;
	bool		Clip( true );
	
	///-- Texture 없음 -> Dynamic Loading	
	if( Tex && Resource->Img[Tex->ImgId].pTexture == NULL )
	{		
		Resource->LoadTextureDynamic( Tid );	
		Tex = Resource->GetTexture( Tid );		
	}
	
	if( Tex != NULL)
	{
		memcpy( &tmpTex, Tex, sizeof( STexture ) );	
	}
	else
	{
		return false;
	}
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DTEXTURE9 pTexture = tmpTex.pTexture;
#else
	LPDIRECT3DTEXTURE8 pTexture = tmpTex.pTexture;
#endif // DIRECT_VERSION_9_MJH	
	SImage* Img = Resource->GetImageInfo( tmpTex.ImgId );
	if( (pTexture == NULL) || (Img == NULL) )
	{
		return false;
	}	
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DSURFACE9		m_pSurface;
#else
	LPDIRECT3DSURFACE8		m_pSurface;
#endif // DIRECT_VERSION_9_MJH	
	
	if( FAILED(pTexture->GetSurfaceLevel( 0, &m_pSurface ) ) )
	{
		return false;
	}
	
	if(IsItem)
	{
		tmpTex.x += 1;
		tmpTex.y += 1;
	}
	
	RECT Rect = { tmpTex.x,  tmpTex.y, tmpTex.x + 32, tmpTex.y + 32 };
    
	// Create a surface for the fullscreen cursor
#ifdef DIRECT_VERSION_9_MJH
	if( FAILED( g_RenderManager.CreateOffscreenPlainSurface( 32, 32,
		D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH ,&m_pCursorBitmap , NULL) ) )
#else 
	if( FAILED( g_RenderManager.CreateImageSurface( 32, 32,
		D3DFMT_A8R8G8B8, &m_pCursorBitmap ) ) )
#endif // DIRECT_VERSION_9_MJH

	{
        return false;
	}         
	
	D3DLOCKED_RECT lrSrc, lrDest;
	DWORD * pScrData = NULL;
	DWORD * pDestData = NULL;
    if(	FAILED( m_pSurface->LockRect( &lrSrc, &Rect, 0 ) ) )
		return false;	
	pScrData = (DWORD*)lrSrc.pBits;	
	
	if(	FAILED( m_pCursorBitmap->LockRect( &lrDest, NULL, 0 ) ) )
		return false;
	pDestData = (DWORD*)lrDest.pBits;
	
	int dx = 9;
	int dy = 0;
	
	for(int y = 0; y < 32 ; y++ )
	{
		for(int x = 0; x < 32 ; x++ )
		{
			pDestData[32*y + x] = pScrData[(Img->Width)*y + x];
			
			if(IsItem)
			{
				if( x <= dx)
					pDestData[32*y + x] = 0xffff9933;
				
				if( x == (dx + 1) )
					pDestData[32*y + x] = 0xff000000;	
				
				if(x == 31 || x == 0 || y== 0 || y == 31)
					pDestData[32*y + x] = 0xff000000;
			}
		}
		
		if(IsItem)
		{
			dx--;
		}
	}	
	
	m_pCursorBitmap->UnlockRect();
	m_pSurface->UnlockRect();
	
	m_pSurface->Release();
	
	// Set the device cursor
	if( FAILED(g_RenderManager.SetCursorProperties( 0, 
		0, m_pCursorBitmap ) ) )
	{
		return false;
	}
	
	if(g_RockClient.m_IsDeviceCursor)
	{
		g_RenderManager.ShowCursor( FALSE );
		g_RockClient.m_IsDeviceCursor = FALSE;
	}
	
	g_RenderManager.ShowCursor( TRUE );	
	g_RockClient.m_IsDeviceCursor = TRUE;	
	return true;
}

//-----------------------------------------------------------------------------
WNDID CRui::IsFocus( int Mx, int My )
{	
	//UI에 포커스가 있는지 확인
	FocusPwnd = FindFocusWindow( Mx, My, false ); 
	return( FocusPwnd );
}

//-----------------------------------------------------------------------------
bool CRui::IsEditFocus()
{
	if( Desktop.hWnd == Desktop.hCurWnd )
	{
		return( false );
	}
	
	return( true );
}

//-----------------------------------------------------------------------------
WNDID CRui::FindFocusWindow( int Mx, int My, bool IsChildCheck )
{
	WNDID Twnd = WID_None;
	WNDID Fwnd = WID_None;
	
	stChainNode* Node = Chain.GetHead();
	
	while( Node != NULL )
	{
		if( Node->Wnd && Node->Wnd->IsVisible() )
		{
			if( g_RockClient.m_bIsDrawFullScreenUI )
			{	
				if( ( Node->Wnd->GetID() == ModalWnd && ModalWnd != WID_None ) || 
					Node->Wnd->GetID() == WID_BZ_ROOMLIST_WND ||
					Node->Wnd->GetID() == WID_BZ_CREATEROOM_WND ||
					Node->Wnd->GetID() == WID_BZ_WAITINGROOM_WND ||
					( Node->Wnd->GetParentID() == ModalWnd && ModalWnd != WID_None ) ||
					Node->Wnd->GetParentID() == WID_BZ_ROOMLIST_WND ||
					Node->Wnd->GetParentID() == WID_BZ_CREATEROOM_WND ||
					Node->Wnd->GetParentID() == WID_BZ_WAITINGROOM_WND )
				{
					Twnd = Node->Wnd->FocusCheck( Mx, My, IsChildCheck );
					if( Twnd != WID_None )
					{
						Fwnd = Twnd;
					}
				}
			}
			else
			{
				Twnd = Node->Wnd->FocusCheck( Mx, My, IsChildCheck );
				if( Twnd != WID_None )
				{
					Fwnd = Twnd;
				}
			}
		}
		
		Node = Chain.GetNext( Node );
	}
	return( Fwnd );
}

//-----------------------------------------------------------------------------
HWND CRui::GetWndHandle()
{
	return( Desktop.hWnd );
}

//-----------------------------------------------------------------------------
HWND CRui::GetCurrentWndHandle()
{
	return( Desktop.hCurWnd );
}

//-----------------------------------------------------------------------------
N_RESOLUTION CRui::GetResolution()
{
	return theResolution;
}

//-----------------------------------------------------------------------------
void CRui::MakeEventMessage( SKeys* Keys, SMouse* Mouse, WNDID FocusWnd )
{
	//memset( &EventM, 0, sizeof(stEventMessage) );유지해야 되는 값이 있기 때문에 완전히 초기화 하면 안된다...
	
	theEMsg.Notify		= NM_NONE;
	//theEMsg.FocusWnd	= FocusWnd;
	
	memcpy( theEMsg.Keys, Keys, sizeof( SKeys ) * 256 );
	memcpy( &theEMsg.Mouse, Mouse, sizeof( SMouse ) );
	
	
	//	POINT		ptCursor;
	//	GetCursorPos( &ptCursor );
	//	::ScreenToClient( g_RockClient.GetApp(), &ptCursor );
	
	//	theCursor->pos.x = ptCursor.x;
	//	theCursor->pos.y = ptCursor.y;
	
	//	theCursor->pos.x = theEMsg.Mouse.Aposition.x;
	//	theCursor->pos.y = theEMsg.Mouse.Aposition.y;
	
	//-------------------------------------------------------------------------
	//마우스 이벤트 만들기
	
	
	//-------------------------------------------------------------------------
	
	theEMsg.Mouse.Event = ME_NONE;
	if( theEMsg.Mouse.State == n_msMove )
	{
		theEMsg.Mouse.Event = ME_MOVE;
	}
	
	
	if( theEMsg.Mouse.uiLButton == n_baDown )
	{
		theEMsg.Mouse.Event |= ME_LBDOWN;
	}
	else if( theEMsg.Mouse.uiLButton == n_baPress )
	{
		theEMsg.Mouse.Event |= ME_LBPRESS;
	}
	else if( theEMsg.Mouse.uiLButton == n_baUp )
	{
		theEMsg.Mouse.Event |= ME_LBUP;
	}
	else if( theEMsg.Mouse.uiRButton == n_baUp )
	{
		theEMsg.Mouse.Event |= ME_RBUP;
	}
	else 
	{
		theEMsg.Mouse.Event |= ME_LBNONE;
	}
	
	//-------------------------------------------------------------------------
	theEMsg.Mouse.Action = MA_FOCUS;
	
	switch( theEMsg.Mouse.Event )
	{
		//----------------------------------------------------------------------
		// LBuuton 처리 
		//----------------------------------------------------------------------
	case ME_LBDOWN:
		theEMsg.Mouse.Action = MA_LDOWN;
		break;
		
	case ME_LBPRESS:
		if( isDragging )
		{
			theEMsg.Mouse.Action = MA_LDRAG;
		}
		else
		{
			theEMsg.Mouse.Action = MA_LPRESS;
		}
		break;
		
	case ME_LBUP:
	case ( ME_LBUP|ME_MOVE ):
		if( isDragging )
		{
			if( Desktop.SelectedWnd == Desktop.CurFocusWnd )
			{
				theEMsg.Mouse.Action = MA_LUP;
				isDragging = false;
				//Desktop.DraggingWnd = WID_None;
			}
			else
			{
				theEMsg.Mouse.Action = MA_LDROP;
				isDragging = false;
				//Desktop.DraggingWnd = WID_None;
			}
		}
		else
		{
			theEMsg.Mouse.Action = MA_LUP;
		}	
		break;
		
	case ( ME_LBPRESS|ME_MOVE ):
		if( isDragging )
		{
			theEMsg.Mouse.Action = MA_LDRAG;
		}
		else 
		{
			theEMsg.Mouse.Action = MA_LDRAG;
			isDragging = true;
			Desktop.SelectedWnd = FocusWnd;
			/*
			CWnd* Twnd	= Container->Get( FocusWnd );
			WNDID wndid = Twnd->GetParentID();
			if( wndid == NULL )
			{
			Desktop.DraggingWnd = FocusWnd;	
			}
			else
			{
			Desktop.DraggingWnd = wndid;	
			}
			//*/			
		}
		break;
	case ME_RBUP:
		{
			theEMsg.Mouse.Action = MA_RUP;
		}
		break;
	}
}

void CRui::DrawSystemMsg()
{
	if( ChatMessageWnd.Step > 0 )
	{
		if( SAFE_TIME_COMPARE( g_nowTime , > , SAFE_TIME_ADD( ChatMessageWnd.theOutputTime , 10000 ) ) )
		{
			ChatMessageWnd.Step -= 1;
			ChatMessageWnd.ChatTxt->SetOutputStep( ChatMessageWnd.Step );
			ChatMessageWnd.theOutputTime = g_nowTime;
		}
	}
	
	//	if( ( Desktop.Tflag.Four_1 > 1.0f ) && ( SysMsgOutputWnd.Step > 0 ) )
	//	{
	//		SysMsgOutputWnd.Step -= 1;
	//		SysMsgOutputWnd.SysMsgTxt->SetOutputStep( SysMsgOutputWnd.Step );
	//	}	
	
	return;
}


//-----------------------------------------------------------------------------
void CRui::DrawWindows()
{	
	if( LoadingBarWnd.IsProgress == true )
	{
		return;
	}
	
	stChainNode* Node = Chain.GetHead();
	while( Node != NULL )
	{
		if( Node->Wnd && Node->Wnd->IsVisible() )
		{			
			if( g_RockClient.m_bIsDrawFullScreenUI )
			{	
				if( ( Node->Wnd->GetID() == ModalWnd && ModalWnd != WID_None ) || 
					Node->Wnd->GetID() == WID_BZ_ROOMLIST_WND ||
					Node->Wnd->GetID() == WID_BZ_CREATEROOM_WND ||
					Node->Wnd->GetID() == WID_BZ_WAITINGROOM_WND ||
					( Node->Wnd->GetParentID() == ModalWnd && ModalWnd != WID_None ) ||
					Node->Wnd->GetParentID() == WID_BZ_ROOMLIST_WND ||
					Node->Wnd->GetParentID() == WID_BZ_CREATEROOM_WND ||
					Node->Wnd->GetParentID() == WID_BZ_WAITINGROOM_WND )
				{
					Node->Wnd->RenderProc();
				}
			}
			else
			{
				Node->Wnd->RenderProc();
			}
		}
		Node = Chain.GetNext( Node );
	}
}

//-----------------------------------------------------------------------------
void CRui::DrawDlgBalloon()
{
	if( LoadingBarWnd.IsProgress == true || g_RockClient.GetGameMode() != GMODE_NORMAL )
	{
		return;
	}
	
	g_RenderManager.SetZEnable( TRUE );
	
	int i = 0;
	
	SortDlgBalloon();
	
	for( i = 0; i < MAX_DLGBALLOON; ++i )
	{
		if( theDlgBalloon[i] != NULL )
		{
			theDlgBalloon[i]->RenderProc();
		}
	}
	
	g_RenderManager.SetZEnable( FALSE );
}

void CRui::DrawGameUI()
{
	if( LoadingBarWnd.IsProgress == true || g_RockClient.GetGameMode() != GMODE_NORMAL )
	{
		return;
	}
	
	g_RenderManager.SetZEnable( TRUE );
	
	// 다른 유저들의 머리위에 표시되는 체력과 기력, 마법력 바 
	theNonPlayerInfoMiniBar.RenderProc();
	
	g_RenderManager.SetZEnable( FALSE );
	
	return;
}



//-----------------------------------------------------------------------------
void CRui::DrawToolTip()
{
	if( LoadingBarWnd.IsProgress == true || g_RockClient.GetGameMode() != GMODE_NORMAL )
	{
		return;
	}
	
	CWnd* Fwnd	= NULL;
	CSlot* Slot = NULL;
	HLPID Hid	= HID_None;
	SRect WcSize;//WndClientSize
	SPcItem* PcItem = NULL;		
	
	if( ( FocusWnd != WID_None ) && ( FocusWnd != WID_DeskTop ) )
	{
		Fwnd = Container->Get( FocusWnd );
		Hid = Fwnd->GetHID();
	}

	//by simwoosung
	if(g_RockClient.m_IsMouseRotate)
	{
		return;
	}
	
	if( Hid != HID_None )
	{
		Fwnd->GetClientSize( &WcSize );
		
		//dongs 수정 기본 410 -> 460
		if( ( Hid >= 0 ) && ( Hid <= 600 ) )
		{
			// 기본 툴팁 
			SHelpTip* HTip = Resource->GetHelpTipInfo( Hid );			
			if( HTip )
			{
				theHelpTip->Set( Hid, &WcSize, HTip->Desc, HTip->Color );
				theHelpTip->RenderProc();
			}
		}
		else if( Hid == HID_ITEMTIP )
		{
			if( GetCursorItem() == NULL )
			{
				// 아이템 툴팁 
				Slot = (CSlot*)Fwnd;
				PcItem = Slot->GetSlotItem();				
				BOOL IsColRelationItem = FALSE;
				///-- 콜로니 공성관련아이템 인지 여부를 확인한다.
				if( PcItem != NULL && PcItem->ItemTable )
				{
					if( g_Pc_Manager.IsColonyItem(PcItem->ItemTable) )
					{
						IsColRelationItem = TRUE;
						g_ColItemPreViewWnd.Set( FocusWnd, &WcSize, PcItem );
						g_ColItemPreViewWnd.RenderProc();
					}							
				}		
				
				if( !IsColRelationItem && PcItem != NULL && (PcItem->ItemTable || PcItem->SkillTable) )
				{
					theItemTip->Set( FocusWnd, &WcSize, PcItem );
					theItemTip->RenderProc();
				}
			}
		}
	}
	
	g_PartyInforWnd.RenderToolTipArea();
	CharStateWnd.RenderToolTipName();
	g_ColMapPositionWnd->RenderToolItemName();
}

//-----------------------------------------------------------------------------
void CRui::DrawMessage()
{
	if( LoadingBarWnd.IsProgress == true )
	{
		return;
	}
	
	for( int i = 0; i < theNumMsg; ++i )
	{
		Render->DrawText( theFontg1, theMessage[i].Text, theMessage[i].x, theMessage[i].y, theMessage[i].Color );
	}
	theNumMsg = 0;
	
	// 자동 도움말 공지 메세지
	DrawAutoHelpTip();

}
void CRui::DrawAutoHelpTip()
{
	
	if( g_RockClient.GetGameMode() == GMODE_NORMAL )
	{
		long ElapsedTime = SAFE_TIME_ADD( g_Pc_Manager.theAutoHelpTipMsg.theAutoHelpTipTime , 60000 );
		
		if( SAFE_TIME_COMPARE( ElapsedTime , < , g_nowTime ) )
		{
			if( g_Pc.GetPlayer()->GetAttrIndex() == n_Millena_Man || g_Pc.GetPlayer()->GetAttrIndex() == n_Millena_Woman )
			{
				if( g_Pc_Manager.theAutoHelpTipMsg.theAutoHelpTipStrNum_Millena )
				{
					if( SAFE_TIME_COMPARE( SAFE_TIME_ADD( ElapsedTime , (g_Pc_Manager.theAutoHelpTipMsg.theAutoHelpTipCurIdx * 30000) ) , < , g_nowTime ) )
					{	
						TCHAR *strText = (TCHAR *)(g_Pc_Manager.theAutoHelpTipMsg.theAutoHelpTipAdminMsg_Millena[g_Pc_Manager.theAutoHelpTipMsg.theAutoHelpTipCurIdx]);
						g_ChatMainProc.InsertChatContent(n_HelpChat, _RT(""), strText );
						g_Pc_Manager.theAutoHelpTipMsg.theAutoHelpTipCurIdx++;
					}
					
					if( g_Pc_Manager.theAutoHelpTipMsg.theAutoHelpTipCurIdx >= g_Pc_Manager.theAutoHelpTipMsg.theAutoHelpTipStrNum_Millena )
					{
						g_Pc_Manager.theAutoHelpTipMsg.theAutoHelpTipCurIdx = 0;
						g_Pc_Manager.theAutoHelpTipMsg.theAutoHelpTipTime = g_nowTime;
					}
				}
			}
			else if( g_Pc.GetPlayer()->GetAttrIndex() == n_Rain_Man || g_Pc.GetPlayer()->GetAttrIndex() == n_Rain_Woman )
			{
				if( g_Pc_Manager.theAutoHelpTipMsg.theAutoHelpTipStrNum_Rain )
				{
					if( SAFE_TIME_COMPARE( SAFE_TIME_ADD( ElapsedTime , (g_Pc_Manager.theAutoHelpTipMsg.theAutoHelpTipCurIdx * 30000) ) , < , g_nowTime ) )
					{	
						TCHAR *strText = (TCHAR *)(g_Pc_Manager.theAutoHelpTipMsg.theAutoHelpTipAdminMsg_Rain[g_Pc_Manager.theAutoHelpTipMsg.theAutoHelpTipCurIdx]);
						g_ChatMainProc.InsertChatContent(n_HelpChat, _RT(""), strText );
						g_Pc_Manager.theAutoHelpTipMsg.theAutoHelpTipCurIdx++;
					}
					
					if( g_Pc_Manager.theAutoHelpTipMsg.theAutoHelpTipCurIdx >= g_Pc_Manager.theAutoHelpTipMsg.theAutoHelpTipStrNum_Rain )
					{
						g_Pc_Manager.theAutoHelpTipMsg.theAutoHelpTipCurIdx = 0;
						g_Pc_Manager.theAutoHelpTipMsg.theAutoHelpTipTime = g_nowTime;
					}
				}
			}			
		}
	}
	



}
//-----------------------------------------------------------------------------
void CRui::DrawInfo()
{
	static TCHAR	TimeStr_FPS[40];
	static TCHAR	TimeStr_ElapseT[40];
	static float	Alpha( 0.0f );
	static bool		Aflag( true );
	
	if( !isShowInfo )
	{
		Alpha = 0.0f;
		Aflag = true;
		return;
	}
	
	if( Aflag )
	{
		Alpha += (Desktop.Tflag.One_30 * 4);
		if( Alpha >= 200 )
		{	
			Alpha = 255;
			Aflag = false;
		}
	}
	
	if( Desktop.Tflag.One_2 )
	{
		Rsprintf( RWCHAR(TimeStr_FPS), _RT("FPS : %d"), Desktop.Tflag.FPS );
		Rsprintf( RWCHAR(TimeStr_ElapseT), _RT("ElapseTime : %f"), Desktop.Tflag.ElapseTime );
	}
	
	///Render->Draw( TID_ROCKLOG_B, 10, 0, 128, 128, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
	Render->DrawText( theFontg2, TimeStr_FPS, 150, 30, Desktop.Width-150, 16, D3DCOLOR_ARGB( 255, 255, 255, 0 ),  false, 0 );
	Render->DrawText( theFontg2, TimeStr_ElapseT, 150, 55, Desktop.Width-150, 16, D3DCOLOR_ARGB( 255, 255, 255, 0 ), false, 0 );
}

//-----------------------------------------------------------------------------
void CRui::DrawCursor()
{
	if( LoadingBarWnd.IsProgress == true )
	{
		return;
	}
	
	/*if(!m_IsSurpportHCursor && g_WebShopWnd.IsVisible)
	{
	return;
}*/
	
/*
// 현재 애니메이셔 안되게 수정
if( Desktop.Tflag.One_4 > 1.0f )
{
theCursorAniIdx++;
if( ( theCursorAniIdx >= 6 ) || ( theCursor->img[theCursorAniIdx] == 0 ) )
{
theCursorAniIdx = 0;
}
}

	//*/
	// 오차를 줄이기 위해서 다시 세팅 
	POINT		ptCursor;
	GetCursorPos( &ptCursor );
	::ScreenToClient( g_RockClient.GetApp(), &ptCursor );
	theCursor->pos.x = ptCursor.x;
	theCursor->pos.y = ptCursor.y;
	
	DrawPcItem();
	
	if(!m_IsSurpportHCursor && !g_WebShopWnd.IsVisible)
	{
		if( g_RockClient.m_SceneManager.GetCursorDisplay() )
		{
			Render->Draw( theCursor->img[0], theCursor->pos.x, theCursor->pos.y, RD_DEFAULT, RD_DEFAULT, theCursor->color );
		}
	}
	
	return;
}

///-----------------------------------------------------------------------------
///-- Login Image 를 Draw
///-----------------------------------------------------------------------------
///-- Version / Login Image 출력
void CRui::DrawBackGround()
{
	///-- Login 화면이라면 
	if( theBGround.IsAction )
	{
		int l_nVerX = 10, l_nVerY = 10;
		int l_nMajor, l_nMinor, l_nBugfix;
		TCHAR l_caTemp[100] = {0,};
		TCHAR Nation[10] = {0,};

		///-- 버전 계산
		
		
		l_nMajor = int( g_Version / 1000 );
		l_nMinor = int( ( g_Version % 1000 ) / 10);
		l_nBugfix = g_Version % 10 ;

		
#if defined(KOREA_VERSION)
	Rstrcpy(RWCHAR(Nation),_RT("KR") );
#elif defined(JAPAN_VERSION)
	Rstrcpy(RWCHAR(Nation),_RT("JP"));
#elif defined(INDONESIA_VERSION)
	Rstrcpy(RWCHAR(Nation),_RT("ID"));
#elif defined(SINGAPORE_VERSION)
	Rstrcpy(RWCHAR(Nation),_RT("SG"));
#elif defined(CHINA_VERSION)
	Rstrcpy(RWCHAR(Nation),_RT("CH"));
#elif defined(GLOBAL_VERSION)
	Rstrcpy(RWCHAR(Nation),_RT("GB"));
#elif defined(TAIWAN_VERSION)
	Rstrcpy(RWCHAR(Nation),_RT("TW"));
#elif defined(HONGKONG)
	Rstrcpy(RWCHAR(Nation),_RT("HK"));
#else 
	Rstrcpy(RWCHAR(Nation),_RT("NOT"));
#endif

		Rsprintf ( RWCHAR(l_caTemp), _RT("%s:%d.%d.%d"),RWCHAR(Nation) , l_nMajor, l_nMinor, l_nBugfix);
		
		if( theResolution == n_1024X768 )
		{
			l_nVerX = 944;
			l_nVerY = 710;
		}
		else if( theResolution == n_800X600 )
		{
			l_nVerX = 720;
			l_nVerY = 570;
		}
		
		///-- Login_Back Image 
		Render->Draw( theBGround.Img, theBGround.Size[0], theBGround.Size[1], theBGround.Size[2], theBGround.Size[3], theBGround.Color );		
		///-- Version 
		Render->DrawText( theFontg2, l_caTemp, l_nVerX, l_nVerY, D3DCOLOR_XRGB( 255, 255, 255 ));
		
		//		sprintf ( l_caTemp, _T("%d"), g_Version);
		//		Render->DrawText( theFontg2, l_caTemp, 10, 10, D3DCOLOR_XRGB( 255, 255, 255 ));
		
	}
}

//-----------------------------------------------------------------------------
//-- 로그인후 Loading Image 2005.03.4/Lyul
//-----------------------------------------------------------------------------
//-- 각 종족에 따른 다른 이미지를 보여준다.
void CRui::DrawForeGround()
{	
	
	int v;
	if( LoadingBarWnd.IsVisible == true )
	{
		
		CWnd* Wnd = NULL;
		Wnd = Container->Get( WID_LoadingWnd );
		srand( time(NULL));
		
		//-- 종족 비교후 Texture 지정
		if( !lb_set_wall )
		{
			
			if( LoadingBarWnd.Get_warpmap_id() == c_RockMap_Millena_Dongeon1_TrajaOfSwamp )
			{
				Wnd->SetClientImage( TID_U_LOADING_10 );
			}
			else if( LoadingBarWnd.Get_warpmap_id() == c_RockMap_Rain_Dongeon1_CaveOfStart )
			{
				Wnd->SetClientImage( TID_U_LOADING_17 );
			}
			else if( LoadingBarWnd.Get_warpmap_id() == c_RockMap_Millena_Dongeon2_WoodOfHesitate )
			{
				Wnd->SetClientImage( TID_U_LOADING_20 );
			}
			else if( LoadingBarWnd.Get_warpmap_id() == c_RockMap_Rain_Dongeon2_SwampOfPhilog )
			{
				Wnd->SetClientImage( TID_U_LOADING_21 );
			}
			else if( LoadingBarWnd.Get_warpmap_id() == c_RockMap_Millena_Rain_PVP_Zone )
			{
				Wnd->SetClientImage( TID_U_LOADING_24 );
			}
			else if( LoadingBarWnd.Get_warpmap_id() == c_RockMap_Millena_SpiralMountain || g_Map.m_NowMapID == c_RockMap_Rain_SpiralMountain )
			{
				Wnd->SetClientImage( TID_U_LOADING_25_26 );
			}
#ifdef HOLYLAND_OF_SEAL_LOADING_IMAGE
			else if( (LoadingBarWnd.Get_warpmap_id() == c_RockMap_Hell_of_Silence ||
				LoadingBarWnd.Get_warpmap_id() == c_RockMap_Holy_land_of_seal ||
				LoadingBarWnd.Get_warpmap_id() == c_RockMap_Before_Hell_of_Fire ||
				LoadingBarWnd.Get_warpmap_id() == c_RockMap_Gate_for_Hell_of_Fire ||
				LoadingBarWnd.Get_warpmap_id() == c_RockMap_Gate_for_Hell_of_Silence ||
				LoadingBarWnd.Get_warpmap_id() == c_RockMap_Before_Hell_of_Silence ||
				LoadingBarWnd.Get_warpmap_id() == c_RockMap_Hell_of_Fire ||
				LoadingBarWnd.Get_warpmap_id() == c_RockMap_Gate_for_Hell_of_Pain ||
				LoadingBarWnd.Get_warpmap_id() == c_RockMap_Befor_Hell_of_Pain ||
				LoadingBarWnd.Get_warpmap_id() == c_RockMap_Hell_of_Pain ||
				LoadingBarWnd.Get_warpmap_id() == c_RockMap_Gate_for_Hell_of_Seal ||
				LoadingBarWnd.Get_warpmap_id() == c_RockMap_Before_Hell_of_Seal ||
				LoadingBarWnd.Get_warpmap_id() == c_RockMap_Hell_of_Seal ||
				LoadingBarWnd.Get_warpmap_id() == c_RockMap_Gate_for_Hell_of_Ruin ||
				LoadingBarWnd.Get_warpmap_id() == c_RockMap_Before_Hell_of_Ruin ||
				LoadingBarWnd.Get_warpmap_id() == c_RockMap_Hell_of_Ruin ||
				LoadingBarWnd.Get_warpmap_id() == c_RockMap_Dhasruinen) && LoadingBarWnd.IsFirstLoading==false	)
			{
				v = rand() % 3;
				switch( v )
				{
					//				case 0 :Wnd->SetClientImage( TID_LOADING_WALL_M0 );break;		
				case 1 :Wnd->SetClientImage( TID_LOADING_SEAL_M1 );break;
				case 2 :Wnd->SetClientImage( TID_LOADING_SEAL_M2 );break;
				default:Wnd->SetClientImage( TID_LOADING_SEAL_M3 );break;
				}
				
			}
#endif

			else if( g_Pc.GetPlayer()->GetAttrIndex() == n_Millena_Woman ||  g_Pc.GetPlayer()->GetAttrIndex() == n_Millena_Man) 
			{
				//srand( time(NULL));
				v = rand() % 5;

#ifdef FIRSTLOADING_IMAGEFIXING
				if(LoadingBarWnd.IsFirstLoading)
				{
					v=5;
					LoadingBarWnd.IsFirstLoading=false;
				}
#endif
				
				///-- 밀레나
				switch( v )
				{
					//				case 0 :Wnd->SetClientImage( TID_LOADING_WALL_M0 );break;		
				case 1 :Wnd->SetClientImage( TID_LOADING_WALL_M1 );break;
				case 2 :Wnd->SetClientImage( TID_LOADING_WALL_M2 );break;
				case 3 :Wnd->SetClientImage( TID_LOADING_WALL_M3 );break;
				case 4 :Wnd->SetClientImage( TID_LOADING_WALL_M4 );break;
				case 5 :Wnd->SetClientImage( TID_LOADING_EVENT   );break;	// 첫화면 고정할 이미지 넣어라
				default:Wnd->SetClientImage( TID_LOADING_WALL_M5 );break;
				}
				
			}
			else
			{				
				v = rand() % 5;

#ifdef FIRSTLOADING_IMAGEFIXING
				if(LoadingBarWnd.IsFirstLoading)
				{
					v=5;
					LoadingBarWnd.IsFirstLoading=false;
				}
#endif
				
				switch( v )
				{
						//				case 0 :Wnd->SetClientImage( TID_LOADING_WALL_R0 );break;		
					case 1 :Wnd->SetClientImage( TID_LOADING_WALL_R1 );break;
					case 2 :Wnd->SetClientImage( TID_LOADING_WALL_R2 );break;
					case 3 :Wnd->SetClientImage( TID_LOADING_WALL_R3 );break;
					case 4 :Wnd->SetClientImage( TID_LOADING_WALL_R4 );break;
					case 5 :Wnd->SetClientImage( TID_LOADING_EVENT   );break;	// 첫화면 고정할 이미지 넣어라
					default:Wnd->SetClientImage( TID_LOADING_WALL_R5 );break;
				}
			
			}

#ifdef APPLY_GUIDE				
			char * pStrDesc = g_pGuideWnd->GetRandomHelpGuide();
			if( pStrDesc )
			{
				LoadingBarWnd.GetHelpTex()->SetText( pStrDesc );
			}
#endif
			lb_set_wall = true;
		}
		
		Wnd->RenderProc();		
	}
	
	if( theFGround.IsAction )
	{
		Render->Draw( theFGround.Img, theFGround.Size[0], theFGround.Size[1], theFGround.Size[2], theFGround.Size[3], theFGround.Color );
	}
}

//-----------------------------------------------------------------------------
void CRui::ShowInfo( bool ShowFlag )
{
	isShowInfo = ShowFlag;
}

//-----------------------------------------------------------------------------
void CRui::ShowCursor( bool ShowFlag )
{
	//필요없음....
	//theCursor.IsShow = ShowFlag; 
}

//-----------------------------------------------------------------------------
void CRui::ShowBackGround( bool ActionFlag )
{
	theBGround.IsAction = ActionFlag;
}

//-----------------------------------------------------------------------------
void CRui::ShowForeGround( bool ActionFlag )
{
	theFGround.IsAction = ActionFlag;
}

//-----------------------------------------------------------------------------
void CRui::SetGroupWnd( int Count, WNDID GroupSelectedWnd, ... )
{
	int i( 0 );
	WNDID* WndList = NULL; //WNDID* WndList( NULL ) <-- error
	va_list List = NULL;
	
	WndList = SAFE_NEW_ARRAY( WNDID , Count );	
	va_start( List, GroupSelectedWnd );
	
	for( i = 0; i < Count; ++i )
	{
		WndList[i] = va_arg( List, WNDID );
	}
	
	for( i = 0; i < Count; ++i )
	{
		SendEvent( WndList[i], n_emSetGroupWnd, (EPARAM)Count, (EPARAM)WndList, 0, 0 );
	}
	SendEvent( GroupSelectedWnd, n_emSetSelectedWnd, 0, 0, 0, 0 );
	
	va_end( List );
	
	SAFE_DELETE_ARRAY( WndList );	
}

//-----------------------------------------------------------------------------
void CRui::SetEnterFlag( NEnterFlag EnterFlag )
{
	theEnterFlag = EnterFlag;
}

//-----------------------------------------------------------------------------
void CRui::SetEnterWndID( WNDID EnterWndID )
{
	theEnterWndID = EnterWndID;
}


//-----------------------------------------------------------------------------
void CRui::GetTimeFlag()
{
	//------------------------------------------------------------
    // 프레임 레이트에 독립적인 구조로...개선 필요
	
	
    //------------------------------------------------------------
	double ElapseTime = 0.0;
	
	//------------------------------------------------------------
    // 시간 변수들 초기화(시작시 한번만....)
    //------------------------------------------------------------
	static double secLast    = GetFrequencyTime();
	static double countLast  = GetCountTime();
    static double secFour_1  = secLast;
    static double secThree_1 = secLast;
    static double secTwo_1   = secLast;    
    static double secOne_1   = secLast; 
    static double secOne_2   = secLast; 
    static double secOne_4   = secLast; 
	static double secOne_7   = secLast; 
	static double secOne_10  = secLast; 
    static double secOne_20  = secLast; 
    static double secOne_30  = secLast; 
    static double secOne_40  = secLast; 
    static double secOne_50  = secLast; 
    static double secOne_60  = secLast;
    static double secCurrent = GetFrequencyTime();
	static double countCurrent = GetCountTime();
	
	
    //------------------------------------------------------------
    // 시간 깃발 초기화
    //------------------------------------------------------------
    Desktop.Tflag.Four_1 = Desktop.Tflag.Three_1 = Desktop.Tflag.Two_1  = 0.0f;
    Desktop.Tflag.One_1  = Desktop.Tflag.One_2   = Desktop.Tflag.One_4  = Desktop.Tflag.One_7  = 0.0f; 
    Desktop.Tflag.One_10 = Desktop.Tflag.One_20  = Desktop.Tflag.One_30 = 0.0f;
    Desktop.Tflag.One_40 = Desktop.Tflag.One_50  = Desktop.Tflag.One_60 = 0.0f;
	
    
    //-------------------------------------------------------------------------
    secCurrent = GetFrequencyTime();
	countCurrent = GetCountTime();
	
	//프레임당 경과 시간 & 초당 프레임
	Desktop.Tflag.ElapseTime = secCurrent - secLast;
	Desktop.Tflag.FPS = (int)( GetFrequency() / ( countCurrent - countLast ) );
	
	secLast = secCurrent;
	countLast = countCurrent;
	//-------------------------------------------------------------------------
	
	
	// 4초
	ElapseTime = secCurrent - secFour_1;
    if( ElapseTime >= 4 )
    {
		Desktop.Tflag.Four_1 = (float)(ElapseTime / 4);
        secFour_1 = secCurrent;
    }
    // 3초
	ElapseTime = secCurrent - secThree_1;
    if( ElapseTime >= 3 )
    {
        Desktop.Tflag.Three_1 = (float)(ElapseTime / 3); 
        secThree_1 = secCurrent;
    }
    // 2초
	ElapseTime = secCurrent - secTwo_1;
    if( ElapseTime >= 2 )
    {
        Desktop.Tflag.Two_1 = (float)(ElapseTime / 2); 
        secTwo_1 = secCurrent;
    }
    // 1초
	ElapseTime = secCurrent - secOne_1;
    if( ElapseTime >= 1 )//1/1
    {
        Desktop.Tflag.One_1 = (float)(ElapseTime / 1);
        secOne_1 = secCurrent;
    }
    // 1/2초
	ElapseTime = secCurrent - secOne_2;
    if( ElapseTime >= 0.5 )//1/2
    {
        Desktop.Tflag.One_2 = (float)(ElapseTime / 0.5); 
        secOne_2 = secCurrent;
    }
    // 1/4초
	ElapseTime = secCurrent - secOne_4;
    if( ElapseTime >= 0.25 )//1/4
    {
        Desktop.Tflag.One_4 = (float)(ElapseTime / 0.25);
        secOne_4 = secCurrent;
    }
	// 1/7초
	ElapseTime = secCurrent - secOne_7;
    if( ElapseTime >= 0.1428 )//1/4
    {
        Desktop.Tflag.One_7 = (float)(ElapseTime / 0.1428);
        secOne_7 = secCurrent;
    }
    // 1/10초
	ElapseTime = secCurrent - secOne_10;
    if( ElapseTime >= 0.1 )//1/10
    {
        Desktop.Tflag.One_10 = (float)(ElapseTime / 0.1);
        secOne_10 = secCurrent;
    }
    // 1/20초 
	ElapseTime = secCurrent - secOne_20;
    if( ElapseTime >= 0.05 )//1/20
    {
        Desktop.Tflag.One_20 = (float)(ElapseTime / 0.05); 
        secOne_20 = secCurrent;
    }
    // 1/30초
	ElapseTime = secCurrent - secOne_30;
    if( ElapseTime >= 0.033 )//1/30
    {
        Desktop.Tflag.One_30 = (float)(ElapseTime / 0.033); 
        secOne_30 = secCurrent;
    }
    // 1/40
	ElapseTime = secCurrent - secOne_40;
    if( ElapseTime >= 0.025 )//1/40
    {
        Desktop.Tflag.One_40 = (float)(ElapseTime / 0.025); 
        secOne_40 = secCurrent;
    }
    // 1/50초
	ElapseTime = secCurrent - secOne_50;
    if( ElapseTime >= 0.02 )//1/50
    {
        Desktop.Tflag.One_50 = (float)(ElapseTime / 0.02);
        secOne_50 = secCurrent;
    }
    // 1/60초
	ElapseTime = secCurrent - secOne_60;
    if( ElapseTime >= 0.016 )//1/60
    {
        Desktop.Tflag.One_60 = (float)(ElapseTime / 0.016);
        secOne_60 = secCurrent;
    }
}

///---------------------------------------------------------------------------
///-- UpdatePcData
///---------------------------------------------------------------------------
void CRui::UpdatePcData()
{
	if( g_RockClient.m_bGameEnd )
	{
		return;		
	}
	
	//	Render->SetState();// * 렌더링 상태 저장 & UI렌더링 상태 설정
	//	Render->Begin();	
	
	if( LoadingBarWnd.IsProgress == true )
	{
		///-- Progress 진행
		LoadingBarWnd.Update(); 
		
		///-- Loading 이 끝났다면 Dynamic texture 해제
		if( LoadingBarWnd.IsProgress == false )			
		{				
			///-- 밀레나
			//			Resource->DestroyDynamic( TID_LOADING_WALL_M0 );		
			Resource->DestroyDynamic( TID_LOADING_WALL_M1 );
			Resource->DestroyDynamic( TID_LOADING_WALL_M2 );
			Resource->DestroyDynamic( TID_LOADING_WALL_M3 );
			Resource->DestroyDynamic( TID_LOADING_WALL_M4 );
			Resource->DestroyDynamic( TID_LOADING_WALL_M5 );
			///-- 레인				
			Resource->DestroyDynamic( TID_LOADING_WALL_R1 );
			Resource->DestroyDynamic( TID_LOADING_WALL_R2 );
			Resource->DestroyDynamic( TID_LOADING_WALL_R3 );
			Resource->DestroyDynamic( TID_LOADING_WALL_R4 );
			Resource->DestroyDynamic( TID_LOADING_WALL_R5 );
					
			///-- Unique wall paper
			Resource->DestroyDynamic( TID_U_LOADING_10 );
			Resource->DestroyDynamic( TID_U_LOADING_17 );
			Resource->DestroyDynamic( TID_U_LOADING_20 );
			Resource->DestroyDynamic( TID_U_LOADING_21 );		
			Resource->DestroyDynamic( TID_U_LOADING_24 );
			Resource->DestroyDynamic( TID_U_LOADING_25_26 );
			
			///-- Battle Zone
			Resource->DestroyDynamic( TID_PVP_WND_BG );			
			
			///-- Loading Bar
			Resource->DestroyDynamic( TID_LoadingClient );
			
			///-- Mini Map Texture 해제
			Resource->DestroyDynamic( TID_MAP02_512 );
			Resource->DestroyDynamic( TID_MAP03_512 );
			Resource->DestroyDynamic( TID_MAP04_512 );
			Resource->DestroyDynamic( TID_MAP05_256 );
			Resource->DestroyDynamic( TID_MAP06_512 );
			Resource->DestroyDynamic( TID_MAP07_512 );
			Resource->DestroyDynamic( TID_MAP08_512 );
			Resource->DestroyDynamic( TID_MAP09_512 );
			Resource->DestroyDynamic( TID_MAP10_512 );
			Resource->DestroyDynamic( TID_MAP11_512 );
			Resource->DestroyDynamic( TID_MAP12_512 );
			Resource->DestroyDynamic( TID_MAP13_512 );
			Resource->DestroyDynamic( TID_MAP14_512 );
			Resource->DestroyDynamic( TID_MAP15_512 );
			Resource->DestroyDynamic( TID_MAP16_512 );
			Resource->DestroyDynamic( TID_MAP17_512 );
			Resource->DestroyDynamic( TID_MAP18_512 );
			Resource->DestroyDynamic( TID_MAP19_512 );
			Resource->DestroyDynamic( TID_MAP20_512 );
			Resource->DestroyDynamic( TID_MAP21_512 );
			Resource->DestroyDynamic( TID_MAP22_512 );
			Resource->DestroyDynamic( TID_MAP23_512 );
			Resource->DestroyDynamic( TID_MAP24_128 );
			Resource->DestroyDynamic( TID_MAP25_256 );
			Resource->DestroyDynamic( TID_MAP26_256 );
			Resource->DestroyDynamic( TID_MAP27_256 );
			Resource->DestroyDynamic( TID_MAP28_256 );
			Resource->DestroyDynamic( TID_MAP29_256 );
			Resource->DestroyDynamic( TID_MAP30_512 );
			Resource->DestroyDynamic( TID_MAP33_256 );
			Resource->DestroyDynamic( TID_MAP34_256 );
			Resource->DestroyDynamic( TID_MAP35_256 );
			Resource->DestroyDynamic( TID_MAP36_256 );
			Resource->DestroyDynamic( TID_MAP41_512 );
			Resource->DestroyDynamic( TID_MAP42_512 );
			Resource->DestroyDynamic( TID_MAP43_512 );
			Resource->DestroyDynamic( TID_MAP44_256 );
			Resource->DestroyDynamic( TID_MAP45_512 );
			Resource->DestroyDynamic( TID_MAP46_512 );
			Resource->DestroyDynamic( TID_MAP47_512 );
			Resource->DestroyDynamic( TID_MAP48_512 );
			
			
			///-- NPC Texture 해제
			Resource->DestroyDynamic( TID_Hart );
			Resource->DestroyDynamic( TID_Ohigen );
			Resource->DestroyDynamic( TID_Flora );
			Resource->DestroyDynamic( TID_Arbess );
			Resource->DestroyDynamic( TID_Tituruit );
			Resource->DestroyDynamic( TID_Strange_Old );
			Resource->DestroyDynamic( TID_Rui );
			Resource->DestroyDynamic( TID_ElRoo );
			Resource->DestroyDynamic( TID_Tiris );
			Resource->DestroyDynamic( TID_Nanaha );
			Resource->DestroyDynamic( TID_Reishana );			
			Resource->DestroyDynamic( TID_Arbeit );
			Resource->DestroyDynamic( TID_NewbiFrominas );
			Resource->DestroyDynamic( TID_Sychy );
			Resource->DestroyDynamic( TID_Nas );
			Resource->DestroyDynamic( TID_VolTraja );					
			Resource->DestroyDynamic( TID_Millena_Soldier );
			Resource->DestroyDynamic( TID_Mission_Soldier );			
			Resource->DestroyDynamic( TID_NPC_Port18 );
			Resource->DestroyDynamic( TID_NPC_Port19 );			
			Resource->DestroyDynamic( TID_NPC_Port20 );
			Resource->DestroyDynamic( TID_NPC_Port21 );
			Resource->DestroyDynamic( TID_NPC_Port22 );
			Resource->DestroyDynamic( TID_NPC_Port23 );
			Resource->DestroyDynamic( TID_NPC_Port24 );					
			Resource->DestroyDynamic( TID_NPC_Port25 );
			Resource->DestroyDynamic( TID_NPC_Port26 );
			Resource->DestroyDynamic( TID_NPC_Port27 );
			Resource->DestroyDynamic( TID_NPC_Port28 );
			Resource->DestroyDynamic( TID_rain_oldman );
			Resource->DestroyDynamic( TID_rain_littleboy );
			Resource->DestroyDynamic( TID_rain_littlegirl );
			Resource->DestroyDynamic( TID_rain_man );
			Resource->DestroyDynamic( TID_rain_girl );
			Resource->DestroyDynamic( TID_millena_girl );
			Resource->DestroyDynamic( TID_millena_man );
			Resource->DestroyDynamic( TID_millena_littleboy );
			Resource->DestroyDynamic( TID_millena_oldman );
			Resource->DestroyDynamic( TID_millena_littlegirl );
			Resource->DestroyDynamic( TID_millena_soldier2 );
			Resource->DestroyDynamic( TID_snow_man );
			Resource->DestroyDynamic( TID_rain_soldier2 );
		}
		return;
	}
	else
	{
		if( lb_set_wall )
		{
			lb_set_wall = false;
		}		
		
		if( g_RockClient.m_bIsDrawFullScreenUI )
		{
			return;
		}
		
		Update2DEffect();
		
		if( g_RockClient.GetGameMode() == GMODE_NORMAL )
		{
		
			
			Resource->DeleteNotUsedTex();
			
			MainMenuWnd.Update();
			g_Exp_wnd.Update();
			g_endu_warning.Update();
#ifdef TAIWAN30MINEVENT
			g_main_buttons.Update();
#endif //TAIWAN30MINEVENT
			
			///MainMenuLVgageWnd.Update();
			CharStateWnd.Update();//	캐릭터 상태 윈도우
			
			QuestWnd.Update();
			g_SkillWnd.Update();			
			g_PlayerEtcPrc.Update();
		}
		
		//	로그인
		if( LoginWnd.IsVisible )			{	LoginWnd.Update();	}
		
		//  서버 리스트
		if( ServerListWnd.IsVisible )		{	ServerListWnd.Update();	}
		
		//	시스템 메뉴
		if( SystemMenuWnd.IsVisible )		{	SystemMenuWnd.Update();	}
		
		//	유저 정보
		if( PlayerInfoWnd.IsVisible )		{	PlayerInfoWnd.Update();	}
		
		if( PlayerInfoAddWnd.IsVisible )	{   PlayerInfoAddWnd.Update();  }
		
		//	다른 유저 및 NPC 정보
		if( NonPlayerInfoWnd.IsVisible )	{	NonPlayerInfoWnd.Update();	}
		else 
		{ 
			for( int i = 0; i < RP_MAX_KEEPUP_SKILL; ++i )
			{
				NonPlayerInfoWnd.DeleteAllKeepUpSkill();
			}
		}
		
		if( NonPlayerInfoAddWnd.IsVisible )	{	NonPlayerInfoAddWnd.Update();	}
		if( g_NonPlayerInfoAdd2Wnd.IsVisible )	{	g_NonPlayerInfoAdd2Wnd.Update();	}
		
		//	HitBar Window
		if( g_ChantHitBarWnd.IsVisible )	{	g_ChantHitBarWnd.Update();	}
		
		//  딜레이 바 윈도우		
        if( g_DelayBarWnd.IsVisible )       {    g_DelayBarWnd.Update();    }
		
		//	인벤토리
		if( InventoryWnd.IsVisible )		{	InventoryWnd.Update();	}
		
		//	퀘스트 창
		if( QuestWnd.IsVisible )			{	QuestWnd.Update();	}
		
		//	능력업 처리 윈도우		
		if( AbilityUpWnd.IsVisible )		{	AbilityUpWnd.Update();	}		
		
		//	능력업 확인취소 윈도우		
		if( AbilityUpConfirmWnd.IsVisible )	{	AbilityUpConfirmWnd.Update();	}
		
		//	미니맵
		if( MiniMapWnd.IsVisible )			{	MiniMapWnd.Update();	}
		
		//	교환창
		if( g_ExchangeWnd.IsVisible )		{	g_ExchangeWnd.Update();	}
		else								{	g_ExchangeWnd.CheckWndUpdate(); }
		
		//	보관창
		if( g_WarehouseWnd.IsVisible )		{	g_WarehouseWnd.Update();	}
		
		//	길드 보관창
#ifdef DECO_RENEWAL_GUILD_MJH
		if( g_GuildInvenWnd.IsVisible )		{	g_GuildInvenWnd.Update();	}
#endif // DECO_RENEWAL_GUILD_MJH		
		//	스킬
		if( g_SkillWnd.IsVisible )			{	g_SkillWnd.Update();	}
		
		//	단축키 변경 창 
		//		if( SkillRegistWnd.IsVisible )		{	SkillRegistWnd.Update();	}
		
		//	스킬레벨업처리창
		//		if( SkillLevelUpWnd.IsVisible )		{	SkillLevelUpWnd.Update();	}
		
		//	스킬정보창
		//		if( SkillInfoWnd.IsVisible )		{	SkillInfoWnd.Update();	}
		
		//	단축키 변경 확인창
		//		if( SkillOverWriteWnd.IsVisible )	{	SkillOverWriteWnd.Update();	}
		
		//	상점 거래
		if( StoreWnd.IsVisible )			{	StoreWnd.Update();	}
		
		//	채팅창
		if( ChatMessageWnd.IsVisible )		{	ChatMessageWnd.Update();	}
		if( ChatBoxWnd.IsVisible )			{	ChatBoxWnd.Update();	}
		if( ChatOutputWnd.IsVisible )		{	ChatOutputWnd.Update();	}
		
		// 서버 선택 창
		if( CharSelectWnd.IsVisible )		{ CharSelectWnd.Update(); }
		if( g_CharCreateWndProc.IsVisible ) { g_CharCreateWndProc.Update(); }
#ifdef DECO_RENEWAL_MJH
		if( g_CharCreateHairProc.IsVisible ) { g_CharCreateHairProc.Update(); }
#endif // DECO_RENEWAL_MJH		
		// 메세지 박스 창
		if( MessageBoxWnd.IsVisible )		{ MessageBoxWnd.Update(); }
		
		// NPC 대화창 
		if( NpcChatWnd.IsVisible )			{	NpcChatWnd.Update();	}
		
		//		if( LoadingBarWnd.IsVisible ) 		{ LoadingBarWnd.Update(); }
		
		// 아처 컨디션 게이지
		
		
		if( g_ArcherBar.ISVisible() )		{	g_ArcherBar.Update(); }		
		
		// 파티 초대 수락창
		if( g_AcceptYesNoWnd.IsVisible )    {   g_AcceptYesNoWnd.Update();    }
		
		//메시지 창
		if( g_UIMsgWnd.IsVisible )			{   g_UIMsgWnd.Update();   }
		
		//파티 정보 창
		if(	g_PartyInforWnd.IsMemberParty() )
		{   g_PartyInforWnd.Update();	}
		
		//파티 팝업메뉴창
		if( g_PartyPopupWnd.IsVisible )		{   g_PartyPopupWnd.Update();   }
		
		///-- 아이템 강화창
		if( g_ItemEnchantWndProc.IsVisible ){	g_ItemEnchantWndProc.Update(); } 
		if( g_ItemEnchant_RWndProc.IsVisible ){	g_ItemEnchant_RWndProc.Update(); } 
		if( g_ItemAccEnchant.IsVisible) {g_ItemAccEnchant.Update();}
		if( g_ItemAccEnchant_RWndProc.IsVisible ){ g_ItemAccEnchant_RWndProc.Update();}	
		
		///-- 아이템 강화창
		if( g_ItemExtract_WndProc.IsVisible ){	g_ItemExtract_WndProc.Update(); } 
		if( g_ItemExtract_R_WndProc.IsVisible ){	g_ItemExtract_R_WndProc.Update(); } 
		
		if( g_ItemCreate_WndProc.IsVisible ){	g_ItemCreate_WndProc.Update(); } 
		if( g_ItemCreate_S_WndProc.IsVisible ){	g_ItemCreate_S_WndProc.Update(); } 
		if( g_ItemCreate_F_WndProc.IsVisible ){	g_ItemCreate_F_WndProc.Update(); } 
		
		if( g_ItemRemodel_WndProc.IsVisible ){	g_ItemRemodel_WndProc.Update(); } 
		if( g_ItemRemodel_S_WndProc.IsVisible ){	g_ItemRemodel_S_WndProc.Update(); } 
		if( g_ItemRemodel_F_WndProc.IsVisible ){	g_ItemRemodel_F_WndProc.Update(); }		
		
		if( g_ItemRepair_WndProc.IsVisible ){	g_ItemRepair_WndProc.Update(); }		
		
		//	채팅창
		if( g_ChatMainProc.GetOutState() != COutState_None )
		{
			g_ChatMainProc.Update();
		}
		
		// 시스템 출력창
		if( g_SystemPrintWnd.IsVisible )
		{   g_SystemPrintWnd.Update();   }
		
		///-- 커뮤니티 윈도우창 - 내부에서 각 부속윈도우의 IsVisible을 처리해줌. 
		{   g_MCommunityWnd.Update();   }
		
		///-메신져 메모창
		if( g_MessengerMemoWnd.IsVisible )
		{   g_MessengerMemoWnd.Update();   }
		
		// By simwoosung 슬롯 딜레이 검사루틴 
		//해당 클래스는 윈도우가 아니라 프로세스 루틴임
        g_SlotDelayProc.Update();
		
		//  Combo / Sub Job Log Wnd By wxywxy
		if( g_LogWnd.IsVisible )		{ g_LogWnd.Update();	}
		
		if( g_BZ_SelectZoneWnd.IsVisible )		{ g_BZ_SelectZoneWnd.Update();	}
		if( g_BZ_SelectGradeWnd.IsVisible )		{ g_BZ_SelectGradeWnd.Update();	}
		if( g_BZ_RoomListWnd.IsVisible )		{ g_BZ_RoomListWnd.Update();	}
		if( g_BZ_CreateRoomWnd.IsVisible )		{ g_BZ_CreateRoomWnd.Update();	}
		if( g_BZ_WaitingRoomWnd.IsVisible )		{ g_BZ_WaitingRoomWnd.Update();	}
		if( g_BZ_GameResultWnd.IsVisible )		{ g_BZ_GameResultWnd.Update();	}
		
		if( g_UserStoreSellWnd.IsVisible )		{ g_UserStoreSellWnd.Update();  }
		
		if( g_SocialActWnd.IsVisible )			{ g_SocialActWnd.Update();		}
		
		if( g_UserStoreSetBuyWnd.IsVisible )    { g_UserStoreSetBuyWnd.Update(); }
		
		if( g_WebShopWnd.IsVisible )			{ g_WebShopWnd.Update(); }
		
		if( g_FrontierCreateWnd.IsVisible )		{ g_FrontierCreateWnd.Update(); }
#ifdef DECO_RENEWAL_GUILD_MJH 
		if( g_FrontierCreateScrollWnd.IsVisible )		{ g_FrontierCreateScrollWnd.Update(); }
		if( g_FrontierMenuBtnWnd.IsVisible )		{ g_FrontierMenuBtnWnd.Update(); }
		if( g_ProposeJoinFrontier.IsVisible )		{ g_ProposeJoinFrontier.Update(); }
#endif //DECO_RENEWAL_GUILD_MJH
		
		if( g_FrontierMainWnd.IsVisible )		{ g_FrontierMainWnd.Update(); }
		
		//리그 멤버 정보창
		if( g_LeagueInfoWnd.IsVisible )			{ g_LeagueInfoWnd.Update(); }		
		
		//펫 정보 
		if( g_PetMainWndProc.IsVisible )		{ g_PetMainWndProc.Update(); }
		
		if( g_PetInfoWnd.IsVisible )		{ g_PetInfoWnd.Update(); }
		
		if( g_PetCreateWnd.IsVisible )	{ g_PetCreateWnd.Update(); }		
		
		if( g_ItemSkillPotionWndProc.IsVisible ){ g_ItemSkillPotionWndProc.Update(); }
		if( g_ItemEldaMixAWndProc.IsVisible ){ g_ItemEldaMixAWndProc.Update(); }
    
		///-- 콜로니 관련 By simwoosung
		if( g_ColRegionListWnd.IsVisible ) {	g_ColRegionListWnd.Update(); }
		if( g_ColRegionInfoWnd.IsVisible ) { g_ColRegionInfoWnd.Update(); }	
		if( g_ColMapPositionWnd->IsVisible ) { g_ColMapPositionWnd->Update(); }
		
		///-- 가이드창 - 모험백서
		if( g_pGuideWnd->IsVisible ) { g_pGuideWnd->Update(); }
		
		//라디오 방송 버튼
#ifdef CHINA_VERSION
		if( g_pRadioBroadcast->IsVisible ){ g_pRadioBroadcast->Update();}
#endif	
		///-- 장신구 가공창
		if( g_pAcryEnchantWnd->IsVisible ) { g_pAcryEnchantWnd->Update(); }
		///-- 장신구 가공 결과창
		if( g_pAcryEnchantRWnd->IsVisible ) { g_pAcryEnchantRWnd->Update(); }
		
#ifdef MIX_ELDASTONE
		if( g_ItemEldaMixRWndProc.IsVisible ){g_ItemEldaMixRWndProc.Update();}
#endif 
		
		//엘다스톤 색상 변경창
		if( g_pChangeEldaColorWnd->IsVisible ) { g_pChangeEldaColorWnd->Update(); } 
		//엘다스톤 색상 변경 결과창
		if( g_pChangeEldaColorRWnd->IsVisible ) { g_pChangeEldaColorRWnd->Update(); }

#ifdef 		C_SEALED_ELDASTONE_DUST	
		if( g_ItemEldaDustWndProc.IsVisible ) { g_ItemEldaDustWndProc.Update();}
#endif 

#ifdef C_SUBJOB_RENEW
		if( g_EssenceWndProc->IsVisible ) { g_EssenceWndProc->Update();}
		if( g_SpellStoneWndProc->IsVisible) {g_SpellStoneWndProc->Update();}
		if( g_StrengthStoneWndProc->IsVisible ){g_StrengthStoneWndProc->Update();}
#endif 


	}
	
	//	Render->End();
	//	Render->ReleaseState();	// * 렌더링 상태 복구
}

void CRui::Update2DEffect()
{
	//콤보 타이밍 - by simwoosung
	m_Combo2DTimingEffect.UpdateProc();
	m_BZoneIn2DEffect.UpdateProc();
}

//-----------------------------------------------------------------------------
void CRui::InitMyPcData()
{
	//커서 초기화 by simwoosung
	thePcItem = NULL;
	
	theCursor = Resource->GetCursorInfo( n_CT_NORMAL );
	if(m_IsSurpportHCursor)
	{
		SetDeviceCursor(theCursor->img[0] , false);
	}
	
	//SPcDataParam	thePcParam;
	
	
	memset( &thePcParam, 0, sizeof( SPcDataParam ) );
	
	//SPcDataInven	thePcInven;
    //액션 시스템 레벨 초기화 - thePcParam 초기화 후 콤보게이지 초기화 
	g_Pc.GetPlayer()->theActionSystem.SetComboGauge();
	
	int i = 0;
	//SPcItem*	WearItem[RP_MAX_EQUIP];
	for( i = 0; i < RP_MAX_EQUIP; ++i )
	{
		SAFE_DELETE(thePcInven.WearItem[i]);		
	}
	
	//SPcItem*	InvenItem[c_MaxInvenItemNum];
	for( i = 0; i < c_MaxInvenItemNum; ++i )
	{
		SAFE_DELETE(thePcInven.InvenItem[i]);	
	}
	
	//SPcItem*	InvenQuest[RP_MAX_QUEST_ITEM];
	for( i = 0; i < RP_MAX_QUEST_ITEM; ++i )
	{
		SAFE_DELETE(thePcInven.InvenQuest[i]);
	}
	
	for( i = 0; i < RP_MAX_QUEST_ITEM; ++i )
	{
		SAFE_DELETE(thePcInven.InvenHorse[i]);	
	}
	
	//SPcItem*	SkillItem[RP_MAX_USER_SKILL];
	for( i = 0; i < RP_MAX_USER_SKILL; ++i )
	{
		SAFE_DELETE(thePcInven.SkillItem[i]);
	}
	
	//SPcItem*	QuickSlot[RP_MAX_QUICK_SLOT];
	for( i = 0; i < RP_MAX_QUICK_SLOT; ++i )
	{
		thePcInven.QuickSlot[i] = NULL;
	}
	
	memset( thePcInven.KeepupSkill, 0, sizeof( WORD ) * RP_MAX_KEEPUP_SKILL );
	
	for( i = 0; i < RP_MAX_QUEST_FLAG; ++i )
	{
		SAFE_DELETE(thePcInven.QuestFlag[i]);
	} 
	
	for( i = 0; i < RP_MAX_QUEST_COUNT; ++i )
	{
		SAFE_DELETE(thePcInven.QuestCount[i]);
		SAFE_DELETE(thePcInven.QuestTime[i] );
	}
	
	for( i = 0; i < RP_MAX_PAY_CONTENTS; ++i )
	{
		SAFE_DELETE(thePcInven.PayEffect[i]);
	}

	for( i = 0; i < RP_MAX_TIMELIMIT_PAY_CONTENTS; ++i )
	{
		SAFE_DELETE(thePcInven.PayTimeItemList[i]);
	}
	
	
	SAFE_DELETE(thePcInven.FrontierMyInfo);
	SAFE_DELETE(thePcInven.FrontierInfo);
	SAFE_DELETE(thePcInven.pComBineFrontierInfo);
	
	memset( thePcInven.FrontierCaller, 0, sizeof( char ) * RP_MAX_PC_NAME );		
	
	SAFE_DELETE(thePcInven.FrontierPetInfo);
	
	for( i=0 ; i < 3  ; ++i )
	{
		if(thePcInven.thePetData[i] != NULL)
		{
			for(int j=0; j < RP_MAX_PET_ITEM ; ++j )
			{	
				SAFE_DELETE(thePcInven.thePetData[i]->InvenItem[j]);					
			}
			
			SAFE_DELETE(thePcInven.thePetData[i]);	
		}
		
	}
	
	//토템 초기화 
	memset(&thePcInven.TotemScore , 0 , sizeof(STotemScore));
	//인던 타임 
	memset(&thePcInven.InDunData, 0 , sizeof(SInDunData));
	
	
	return;
}

// 게임 접속시 전송받은 캐릭터 데이터를 저장한다.-----------------------------------------------------------------------
void CRui::SetMyPcData( SRpdPCData* pcdata, int pcdata_size )
{
	//-------------------------------------------------------------------------
	//파라메터 설정
	//-------------------------------------------------------------------------
	Rstrcpy( RWCHAR(thePcParam.PCName), RWCHAR(pcdata->thePCName) );		//thePCName[RP_MAX_PC_NAME]
	Rstrcpy( RWCHAR(thePcParam.AccountID), RWCHAR(pcdata->theAccountID) );	//theAccountID[RP_MAX_ACCOUNT_ID]
	
	thePcParam.Citizen		= pcdata->theCitizen;		// RAIN, MILLENA
	thePcParam.Gender		= pcdata->theGender;		// MALE, FEMALE
	thePcParam.Face			= pcdata->theFace;
	thePcParam.Hair			= pcdata->theHair;
	thePcParam.MainClass	= pcdata->theMainClass;		// 메인직업 30종
	thePcParam.SubClass		= pcdata->theSubClass;		// 서브직업 15종
	
	thePcParam.Exp			= pcdata->theExp;
	thePcParam.Fame			= pcdata->theFame;			// 명성
	thePcParam.Level		= pcdata->theLevel;			// 1~255 후에 확장을 고려하여 좀 여유있게 확보
	thePcParam.Map			= pcdata->theMap;
	thePcParam.Cell			= pcdata->theCell;
	
	thePcParam.SubExp		= pcdata->theSubExp;		///-- Sub Class 경험치
	thePcParam.SubLevel		= pcdata->theSubLevel;		///-- Sub Class Level
	
	//thePcParam.Stat1		= pcdata->theStat1;
	//thePcParam.Stat2		= pcdata->theStat2;
	memcpy( &thePcParam.Stat1, &pcdata->theStat1, sizeof( SRpdSTAT1 ) );
	memcpy( &thePcParam.Stat2, &pcdata->theStat2, sizeof( SRpdSTAT2 ) );
	
	g_Pc.GetPlayer()->theActionSystem.theComboGauge = thePcParam.Stat1.theGage;
	g_Pc.GetPlayer()->theActionSystem.theProimaGauge = thePcParam.Stat1.theGage;
	//	g_Pc.GetPlayer()->theActionSystem.theConditionGauge = thePcParam.Stat1.theConditionGage;
	
	thePcParam.MaxHP		= pcdata->theMaxHP;			// 기본 값 + 유저 설정값 
	thePcParam.MaxSP		= pcdata->theMaxSP;
	thePcParam.MaxMP		= pcdata->theMaxMP;
	
	thePcParam.Power		= pcdata->thePower;
	thePcParam.Vital		= pcdata->theVital;
	thePcParam.Sympathy		= pcdata->theSympathy;
	thePcParam.Intel		= pcdata->theInt;		
	thePcParam.Stamina		= pcdata->theStamina;		
	thePcParam.Dex			= pcdata->theDex;
	//	thePcParam.Power		= pcdata->thePower;			// 파워
	
	
	//	thePcParam.Defense		= pcdata->theDefense;		// 방어
	
	
	//	thePcParam.Sympathy		= pcdata->theSympathy;		// 교감
	//	thePcParam.Resist		= pcdata->theResist;		// 저항
	//	thePcParam.Focus		= pcdata->theFocus;			// 집중
	//	thePcParam.Reaction		= pcdata->theReaction;		// 반응
	
	thePcParam.Charisma		= pcdata->theCharisma;		// 카리스마, 매력
	thePcParam.Luck			= pcdata->theLuck;			// 행운  -10 ~ 10
	thePcParam.Point		= pcdata->thePoint;			// 능력치 올리는 보너스 포인트 
	thePcParam.SkillPoint	= pcdata->theSkillPoint;
	thePcParam.TotalSkillPoint = pcdata->theTotalSkillPoint;
	
	// 전적정보 
	thePcParam.WinFCount	= pcdata->theWinFCount;				// 전적 
	thePcParam.TotalFCount	= pcdata->theTotalFCount;			// 총 전적 
	
	thePcParam.Money		= pcdata->theMoney;
	thePcParam.Weight		= pcdata->theWeight;
	
	thePcParam.Contribution	= pcdata->theContribution;	

#ifdef PK_SYSTEM
	thePcParam.thePKPoint	= pcdata->thePKPoint;
#endif

// 김정기 2009/12/29
// 서버에서 받은 누적 명성치를 저장
#ifdef FAMOUSE_FIGURES_KJK
	thePcParam.TotalFame = pcdata->theTotalFame;	// 누적 명성치
#endif
	
	//-------------------------------------------------------------------------
	//스킬, 인벤토리 설정
	//-------------------------------------------------------------------------
	int WearItemNum			= pcdata->theWearItemNum;
	int InvenItem1Num		= pcdata->theInvenItem1Num;
	int InvenItem2Num		= pcdata->theInvenItem2Num;
	int QuestItemNum		= pcdata->theQuestItemNum;	
	int SkillItemNum		= pcdata->theSkillNum;
	int KeepupSkillItemNum	= pcdata->theKeepupSkillNum;
	int QuickSlotNum		= pcdata->theQuickSlotNum;
	int DelayTimeNum		= pcdata->theDelayTimeNum;
	int QuestFlagNum		= pcdata->theQuestFlagNum;		
	int QuestCountNum		= pcdata->theQuestCountNum;
	//						= pcdata->theRemainBit2:14; // 남는 비트
	
	int PayEffectNum		= pcdata->thePayItemNum;
	
	int RidingNum			= 0;
#ifdef RIDING_SYSTEM
	RidingNum			    = pcdata->theRidingNum;
#endif
	
	int PcDataSize			= pcdata_size;
	int PcDataHeaderSize	= 0;	
	
	int WearItemSize		= sizeof( SRpdWearItemBase ) * WearItemNum;
	int InvenItem1Size		= sizeof( SRpdWearItemBase ) * InvenItem1Num;
	int InvenItem2Size		= sizeof( SRpdWasteItem ) * InvenItem2Num;
	int QuestItemSize		= sizeof( SRpdWasteItem ) * QuestItemNum;	
	int SkillItemSize		= sizeof( WORD ) * SkillItemNum;
	int KeepupSkillItemSize	= sizeof( WORD ) * KeepupSkillItemNum;
	int QuickSlotSize		= sizeof( SRpdQuickSlot ) * QuickSlotNum;
	int DelayTimeSize		= sizeof( SRpdDelayTime ) * DelayTimeNum;	
	int HotKeySize			= sizeof( WORD ) * 3;		// 0: Shift, 1: Space, 2: Combo By wxywxy
	int QuestFlagSize		= sizeof( SQuestFlag ) * QuestFlagNum;	
	int SaveLogSize			= sizeof( char ) * 70;
	int QuestCountSize		= sizeof( SQuestCount ) * QuestCountNum;
	
	int PayEffectSize		= 0;			
	int FrontierInfoSize 	= 0;
	
	PayEffectSize			= sizeof( SPayEffect ) * PayEffectNum;			
	//	SPayEffect				thePayEffectList[RP_MAX_PAY_CONTENTS];
	
	//	FrontierInfoSize 		= sizeof( DWORD ) + sizeof( BYTE ) + ( sizeof( char ) * RP_MAX_FRONTIER_NICK ) + sizeof( DWORD );	
	FrontierInfoSize 		= sizeof( SFrontier_MyInfo );
	
	int RidingSize			= 0;
#ifdef RIDING_SYSTEM
	RidingSize				= sizeof( SRpdWearItemBase ) * RidingNum * RP_MAX_RIDINGITEM;
#endif
	
	//	DWORD				theFrontierID;
	//	BYTE				theFrontierGrade;
	//	char				theFrontierNick[RP_MAX_FRONTIER_NICK];
	//	DWORD				theFrontierDues;
	
	PcDataHeaderSize = PcDataSize - ( WearItemSize + InvenItem1Size + InvenItem2Size + QuestItemSize + SkillItemSize + KeepupSkillItemSize + QuickSlotSize 
		+ DelayTimeSize + HotKeySize + QuestFlagSize + SaveLogSize + QuestCountSize + PayEffectSize + FrontierInfoSize + RidingSize );	
	
	
	BYTE* pBuf = (BYTE*)pcdata;
	
	
	SRpdWearItemBase*	WearItem		= NULL;
	SRpdWearItemBase*	InvenItem1		= NULL;
	SRpdWasteItem*		InvenItem2		= NULL;
	SRpdWasteItem*		QuestItem		= NULL;	
	WORD*				SkillItem		= NULL;
	WORD*				KeepupSkillItem	= NULL;
	SRpdQuickSlot*		QuickSlot		= NULL;
	SRpdDelayTime*		DelayTime		= NULL; 			
	SQuestFlag*			QuestFlag		= NULL;
	SQuestCount*		QuestCount		= NULL;
	SPayEffect*			PayEffect		= NULL;
	SFrontier_MyInfo*	FrontierMyInfo	= NULL;	
	SRpdWearItemBase *	RidingItem		= NULL;
	
	int p = PcDataHeaderSize;
	if( WearItemNum != 0 )
	{
		WearItem = SAFE_NEW_ARRAY( SRpdWearItemBase , WearItemNum );		
		memcpy( WearItem, &pBuf[p], WearItemSize );
	}
	
	p += WearItemSize;
	if( InvenItem1Num != 0 )
	{
		InvenItem1 = SAFE_NEW_ARRAY( SRpdWearItemBase , InvenItem1Num );		
		memcpy( InvenItem1, &pBuf[p], InvenItem1Size );
	}
	
	p += InvenItem1Size;
	if( InvenItem2Num != 0 )
	{
		InvenItem2 = SAFE_NEW_ARRAY( SRpdWasteItem , InvenItem2Num );		
		memcpy( InvenItem2, &pBuf[p], InvenItem2Size );
	}
	
	p += InvenItem2Size;
	if( QuestItemNum != 0 )
	{
		QuestItem = SAFE_NEW_ARRAY( SRpdWasteItem , QuestItemNum );		
		memcpy( QuestItem, &pBuf[p], QuestItemSize );
	}
	
	p += QuestItemSize;
	if( SkillItemNum != 0 )
	{
		SkillItem = SAFE_NEW_ARRAY( WORD , SkillItemNum );		
		memcpy( SkillItem, &pBuf[p], SkillItemSize );
	}
	
	p += SkillItemSize;
	if( KeepupSkillItemNum != 0 )
	{
		KeepupSkillItem	= SAFE_NEW_ARRAY( WORD , KeepupSkillItemNum );		
		memcpy( KeepupSkillItem, &pBuf[p], KeepupSkillItemSize );
	}
	
	p += KeepupSkillItemSize;
	if( QuickSlotNum != 0 )
	{
		QuickSlot = SAFE_NEW_ARRAY( SRpdQuickSlot , QuickSlotNum );		
		memcpy( QuickSlot, &pBuf[p], QuickSlotSize );	
	}
	
	p += QuickSlotSize;
	if( DelayTimeNum != 0 )
	{
		DelayTime = SAFE_NEW_ARRAY( SRpdDelayTime , DelayTimeNum );		
		memcpy( DelayTime, &pBuf[p], DelayTimeSize );
	}
	
	p += DelayTimeSize;
	memcpy( g_Pc.theSkillSystem.theShortCut_SkillCode, &pBuf[p], HotKeySize );	
	
	p += HotKeySize;
	
	//.........................................................................................................
	// 콤보 / 서브직업 읽는 창
	//.........................................................................................................
	g_LogWnd.ResetLog( ( char * ) &pBuf[p] );
	
	p += SaveLogSize;
	//.........................................................................................................
	
	if( QuestFlagNum != 0 )
	{
		QuestFlag = SAFE_NEW_ARRAY( SQuestFlag , QuestFlagNum );		
		memcpy( QuestFlag, &pBuf[p], QuestFlagSize );
	}
	
	p += QuestFlagSize;
	
	if( QuestCountNum != 0 )
	{
		QuestCount = SAFE_NEW_ARRAY( SQuestCount , QuestCountNum );		
		memcpy( QuestCount, &pBuf[p], QuestCountSize );
	}	
	
	int DefineHeaderSize = 0;
	
	DefineHeaderSize = QuestCountSize;
	
	
	//	p += QuestCountSize;
	p += DefineHeaderSize;
	
	if( PayEffectNum != 0 )
	{
		PayEffect = SAFE_NEW_ARRAY( SPayEffect , PayEffectNum );		
		memcpy( PayEffect, &pBuf[p], PayEffectSize );
	}	
	
	DefineHeaderSize = PayEffectSize;
	
	//	p += PayEffectSize;
	p += DefineHeaderSize;
	
	if( FrontierInfoSize != 0 )
	{
		FrontierMyInfo = SAFE_NEW( SFrontier_MyInfo );		
		memcpy( FrontierMyInfo, &pBuf[p], FrontierInfoSize );
	}	
	
	DefineHeaderSize = FrontierInfoSize;
	
	p += DefineHeaderSize;
	
#ifdef RIDING_SYSTEM
	if( RidingSize != 0 )
	{
		RidingItem = SAFE_NEW_ARRAY( SRpdWearItemBase , RidingNum * RP_MAX_RIDINGITEM );		
		memcpy( RidingItem, &pBuf[p], RidingSize );
	}
	
	DefineHeaderSize = RidingSize;

	p += DefineHeaderSize;

#endif
	
	SPcItem* pcitem = NULL;
	int i = 0;
	int j = 0;
	//Wear Slot - Setting
	for( i = 0; i < WearItemNum; ++i )
	{
		// SPcItem객체생성		
		pcitem = SAFE_NEW( SPcItem );		

		// SPcItem객체에 WearItem값 설정
		pcitem->IType		= n_ITYPE_WEAR;	
		
		WORD ItemType = g_Pc_Manager.GetItemClsss( WearItem[i].theCode );
		
		if( ItemType == n_Class_Riding )
		{
			pcitem->IType = n_ITYPE_RIDE;
		}
		else if(ItemType == n_Class_RidingItem)
		{
			pcitem->IType = n_ITYPE_RIDE_WEAR;
		}
		else if( ItemType == n_Class_Skin )
		{
			pcitem->IType = n_ITYPE_SKIN_WEAR;			
		}

		pcitem->Class		= g_Pc_Manager.GetItemClsss( WearItem[i].theCode );
		pcitem->ItemUID		= WearItem[i].theItemUID;
		pcitem->Code		= WearItem[i].theCode;
		pcitem->Tid			= (TEXID)g_Pc_Manager.GetItemTID( WearItem[i].theCode );
		pcitem->Amount		= 1;
		pcitem->ItemTable	= g_Pc_Manager.GetItemBaseInfoPtr( WearItem[i].theCode );
		memcpy( &(pcitem->WearTable), &WearItem[i], sizeof( SRpdWearItemBase ) );

// 	#ifdef HHM_60000_ABOVE_ITEM_COMPOSITION_NOT_EXCHANGE_20090625
// 
// 		if(pcitem->IsItemCode60000Above())
// 		{
// 			SItemBaseInfo* ptItemBaseInfo = pcitem->ItemTable;
// 			if(ptItemBaseInfo)
// 				ptItemBaseInfo->theItemLimitFunc = 
// 					(1 << n_ItemFunc_DropDestory) + (1 << n_ItemFunc_NotExchange) + (1 << n_ItemFunc_NotKeep);
// 		}	
// 
// 	#endif// #ifdef HHM_60000_ABOVE_ITEM_COMPOSITION_NOT_EXCHANGE_20090625

		if( pcitem->ItemTable == NULL )
		{
			SAFE_DELETE( pcitem );
			continue;
		}
		
		// 왼손,오른손....
		//---------------------------------------------------------------------
		if( ( pcitem->ItemTable->theType == n_Type_MagicBook ) || ( pcitem->ItemTable->theType == n_Type_Bow ) || ( pcitem->ItemTable->theType == n_Type_Shield ) )
		{
			pcitem->Class = n_Class_Shield;
		}
		else if( ( pcitem->ItemTable->theType == n_Type_Sword ) || ( pcitem->ItemTable->theType == n_Type_TwohandSword ) ||
			( pcitem->ItemTable->theType == n_Type_Staff ) || ( pcitem->ItemTable->theType == n_Type_CrystalBall ) )
		{
			pcitem->Class = n_Class_Weapon;
		}
		
		//---------------------------------------------------------------------
		SAFE_DELETE( thePcInven.WearItem[WearItem[i].thePosition] );		
		thePcInven.WearItem[WearItem[i].thePosition] = pcitem;
	
#ifdef C_TIME_SUIT_UPDATE		
		if(pcitem->ItemTable->theRare == 255 && pcitem->ItemTable->theMaxEndurance == 0 )
		{
			DWORD dwTime = 0 ; 
			for( int j = 0; j < RP_MAX_TIMELIMIT_PAY_CONTENTS ; ++j )
			{
				if(thePcInven.PayTimeItemList[j] == NULL)
				{
					//두곳 장착과 인벤 아이템 들어가 있는것. . 
					thePcInven.PayTimeItemList[j] = SAFE_NEW( SPayTimeItem );
					thePcInven.PayTimeItemList[j]->theItemCode = WearItem[i].theCode;
					thePcInven.PayTimeItemList[j]->theItemID = WearItem[i].theItemUID;
					memcpy(&dwTime , &WearItem[i].theEndurance , sizeof(DWORD));
					//DWORD dwTime = (WearItem[j].theEndurance << 16 ) | (WearItem[i].theEldaStone);
					thePcInven.PayTimeItemList[j]->theTime = dwTime;
					break;
				}
			}	
		}
#endif

	}// for( i = 0; i < WearItemNum; ++i )
	
	//Inven Slot - Setting
	for( i = 0; i < InvenItem1Num; ++i )
	{
		// SPcItem객체생성
		pcitem = SAFE_NEW( SPcItem );
		
		// SPcItem객체에 InvenItem1값 설정
		WORD ItemType = g_Pc_Manager.GetItemClsss( InvenItem1[i].theCode );

		if( ItemType == n_Class_RidingItem )
		{
			pcitem->IType = n_ITYPE_RIDE_WEAR;
		}
		else if(ItemType == n_Class_Skin) 
		{
			pcitem->IType = n_ITYPE_SKIN_WEAR;
		}	
		else
		{
			pcitem->IType = n_ITYPE_WEAR;
		}
		
		pcitem->Class		= ItemType;		
		pcitem->ItemUID		= InvenItem1[i].theItemUID;
		pcitem->Code		= InvenItem1[i].theCode;
		pcitem->Tid			= (TEXID)g_Pc_Manager.GetItemTID( InvenItem1[i].theCode );
		pcitem->Amount		= 1;
		pcitem->ItemTable	= g_Pc_Manager.GetItemBaseInfoPtr( InvenItem1[i].theCode );
		memcpy( &(pcitem->WearTable), &InvenItem1[i], sizeof( SRpdWearItemBase ) );

// 	#ifdef HHM_60000_ABOVE_ITEM_COMPOSITION_NOT_EXCHANGE_20090625
// 		
// 		if(pcitem->IsItemCode60000Above())
// 		{
// 			SItemBaseInfo* ptItemBaseInfo = pcitem->ItemTable;
// 			if(ptItemBaseInfo)
// 				ptItemBaseInfo->theItemLimitFunc = 
// 					(1 << n_ItemFunc_DropDestory) + (1 << n_ItemFunc_NotExchange) + (1 << n_ItemFunc_NotKeep);
// 		}
// 		
// 		SItemBaseInfo*		aItemTable = g_Pc_Manager.GetItemBaseInfoPtr( InvenItem1[i].theCode );
// 
// 	#endif// #ifdef HHM_60000_ABOVE_ITEM_COMPOSITION_NOT_EXCHANGE_20090625

		
		// 왼손,오른손....
		//---------------------------------------------------------------------
		if( pcitem->ItemTable )
		{
			if( ( pcitem->ItemTable->theType == n_Type_MagicBook ) || ( pcitem->ItemTable->theType == n_Type_Bow ) || ( pcitem->ItemTable->theType == n_Type_Shield ) )
			{
				pcitem->Class = n_Class_Shield;
			}
			else if( ( pcitem->ItemTable->theType == n_Type_Sword ) || ( pcitem->ItemTable->theType == n_Type_TwohandSword ) ||
				( pcitem->ItemTable->theType == n_Type_Staff ) || ( pcitem->ItemTable->theType == n_Type_CrystalBall ) )
			{
				pcitem->Class = n_Class_Weapon;
			}
		}
		//---------------------------------------------------------------------
		
		SAFE_DELETE( thePcInven.InvenItem[InvenItem1[i].thePosition] );		
		thePcInven.InvenItem[InvenItem1[i].thePosition] = pcitem;

#ifdef C_TIME_SUIT_UPDATE		
		if(pcitem->ItemTable &&	pcitem->ItemTable->theRare == 255 && pcitem->ItemTable->theMaxEndurance == 0 )
		{
			DWORD dwTime = 0 ; 
			for( int j = 0; j < RP_MAX_TIMELIMIT_PAY_CONTENTS ; ++j )
			{
				if(thePcInven.PayTimeItemList[j] == NULL)
				{
					//두곳 장착과 인벤 아이템 들어가 있는것. . 
					thePcInven.PayTimeItemList[j] = SAFE_NEW( SPayTimeItem );
					thePcInven.PayTimeItemList[j]->theItemCode = InvenItem1[i].theCode;
					thePcInven.PayTimeItemList[j]->theItemID = InvenItem1[i].theItemUID;
					memcpy(&dwTime , &InvenItem1[i].theEndurance , sizeof(DWORD));
					//DWORD dwTime = (WearItem[j].theEndurance << 16 ) | (WearItem[i].theEldaStone);
					thePcInven.PayTimeItemList[j]->theTime = dwTime;
					break;
				}
			}	
		}
#endif


	}
	
	for( i = 0; i < InvenItem2Num; ++i )
	{
		// SPcItem객체생성
		pcitem = SAFE_NEW( SPcItem );	
		
		// SPcItem객체에 InvenItem2값 설정
		if( g_Pc_Manager.GetItemClsss( InvenItem2[i].theCode ) == n_Class_Use )
		{
			pcitem->IType	= n_ITYPE_USE;//소모품
		}
		else if( g_Pc_Manager.GetItemClsss( InvenItem2[i].theCode ) == n_Class_Useless )
		{
			pcitem->IType	= n_ITYPE_UNUSE;//비소모품
		}
		else if( g_Pc_Manager.GetItemClsss( InvenItem2[i].theCode ) == n_Class_Special || 
			g_Pc_Manager.GetItemClsss( InvenItem2[i].theCode ) == n_Class_Eldastone || 
			g_Pc_Manager.GetItemClsss( InvenItem2[i].theCode ) == n_Class_FrontierPet   ||
			g_Pc_Manager.GetItemClsss( InvenItem2[i].theCode ) == n_Class_Weapon_StrengthStone ||
			g_Pc_Manager.GetItemClsss( InvenItem2[i].theCode ) == n_Class_Armor_StrengthStone)
		{
			pcitem->IType	= n_ITYPE_SPECIAL;
		}
		else if(g_Pc_Manager.GetItemClsss(InvenItem2[i].theCode) == n_Class_PersonalPet)
		{
			pcitem->IType	= n_ITYPE_PET_USE;
		}
		
		pcitem->Class		= g_Pc_Manager.GetItemClsss( InvenItem2[i].theCode );
		pcitem->ItemUID		= InvenItem2[i].theItemUID;
		pcitem->Code		= InvenItem2[i].theCode;
		pcitem->Tid			= (TEXID)g_Pc_Manager.GetItemTID( InvenItem2[i].theCode );
		pcitem->Amount		= InvenItem2[i].theBundle;
		pcitem->ItemTable	= g_Pc_Manager.GetItemBaseInfoPtr( InvenItem2[i].theCode );
		
		SAFE_DELETE( thePcInven.InvenItem[InvenItem2[i].thePosition] );		
		thePcInven.InvenItem[InvenItem2[i].thePosition] = pcitem;
#ifdef C_TIME_SUIT_UPDATE//프리미엄열쇠		
		if(pcitem->ItemTable &&	pcitem->ItemTable->theRare == 255 && pcitem->ItemTable->theMaxEndurance == 0 )
		{
			DWORD dwTime = 0 ; 
			for( int j = 0; j < RP_MAX_TIMELIMIT_PAY_CONTENTS ; ++j )
			{
				if(thePcInven.PayTimeItemList[j] == NULL)
				{
					//두곳 장착과 인벤 아이템 들어가 있는것. . 
					thePcInven.PayTimeItemList[j] = SAFE_NEW( SPayTimeItem );
					thePcInven.PayTimeItemList[j]->theItemCode = InvenItem2[i].theCode;
					thePcInven.PayTimeItemList[j]->theItemID = InvenItem2[i].theItemUID;
					//memcpy(&dwTime , &InvenItem2[i].theEndurance , sizeof(DWORD));
					//DWORD dwTime = (WearItem[j].theEndurance << 16 ) | (WearItem[i].theEldaStone);
					thePcInven.PayTimeItemList[j]->theTime = dwTime;
					break;
				}
			}	
		}
#endif
	}	
	
	// QUEST ITEM SETTING
	for( i = 0; i < QuestItemNum; ++i )
	{
		// SPcItem객체생성
		pcitem = SAFE_NEW( SPcItem );
		
		pcitem->IType		= n_ITYPE_QUEST;//퀘스트
		pcitem->Class		= g_Pc_Manager.GetItemClsss( QuestItem[i].theCode );
		pcitem->ItemUID		= QuestItem[i].theItemUID;
		pcitem->Code		= QuestItem[i].theCode;
		pcitem->Tid			= (TEXID)g_Pc_Manager.GetItemTID( QuestItem[i].theCode );
		pcitem->Amount		= QuestItem[i].theBundle;
		pcitem->ItemTable	= g_Pc_Manager.GetItemBaseInfoPtr( QuestItem[i].theCode );
		
		SAFE_DELETE( thePcInven.InvenQuest[QuestItem[i].thePosition] );
		
		thePcInven.InvenQuest[QuestItem[i].thePosition] = pcitem;
	}
	
	g_SkillWnd.InitPassiveSkillCode();
	
	//Skill Slot - Setting
	for( i = 0; i < SkillItemNum; ++i )
	{

		pcitem = SAFE_NEW( SPcItem );
		
		// SPcItem객체에 SkillItem값 설정
		pcitem->IType		= n_ITYPE_SKILL;
		pcitem->Class		= n_ICLASS_NONE;
		pcitem->ItemUID		= 0;
		pcitem->Code		= SkillItem[i];
		pcitem->Tid			= (TEXID)g_Pc_Manager.GetSkillIconTID( SkillItem[i] );
		pcitem->TidEx		= (TEXID)g_Pc_Manager.GetSkillAttriIconTID( SkillItem[i] );
		pcitem->Amount		= 1;
		pcitem->SkillTable	= g_Pc_Manager.GetSkillBaseInfoPtr( SkillItem[i] );
		pcitem->Nation		= g_Pc.m_MainPC.char_info.theCitizen;
		//by simwoosung
		if(pcitem->SkillTable)
		{
			nRui->UpdateMaxGage(pcitem->SkillTable->theSkillCode , true);
		}
		
		SAFE_DELETE(											 thePcInven.SkillItem[i] );
		thePcInven.SkillItem[i] = pcitem;
		
		if( thePcInven.SkillItem[i] != NULL && thePcInven.SkillItem[i]->SkillTable != NULL )
		{
			if( thePcInven.SkillItem[i]->SkillTable->theSkillType1 == n_PasiveSkill )
			{
				g_SkillWnd.SetPassiveSkillCode( thePcInven.SkillItem[i]->Code );
			}	
		}
	}	
	
	//Keepup Skill Setting
	// 초기화 해주는 부분 ( By wxywxy )
	memset( thePcInven.KeepupSkill, 0, sizeof( WORD ) * RP_MAX_KEEPUP_SKILL );
	
	PlayerInfoWnd.DeleteAllKeepUpSkillCode();		

	for( i = 0; i < KeepupSkillItemNum; ++i )
	{
		memcpy( &thePcInven.KeepupSkill[i], &KeepupSkillItem[i], sizeof( WORD ) );
		
		//.....................................................................................................		
		// 지속 스킬 처리 
		//.....................................................................................................		
		if( KeepupSkillItem[i] )
		{
			// 호흡 조절이나 명상 스킬 모션 처리 
			if( ( KeepupSkillItem[i] >= c_SkillCode_BreathControl_Min && KeepupSkillItem[i] <= c_SkillCode_BreathControl_Max) || 
				( KeepupSkillItem[i] >= c_SkillCode_Meditation_Min && KeepupSkillItem[i] <= c_SkillCode_Meditation_Max) )
			{
			}
			else
			{
				SSkillBaseInfo* skill_Info;
				
				skill_Info = g_Pc_Manager.GetSkillBaseInfoPtr( KeepupSkillItem[i] ); 
				
				if( skill_Info != NULL )
				{
					// 지속성 스킬이냐
					if( skill_Info->theEffectFlag && skill_Info->theKeepupEffect != -1  )
					{
						///--JAPAN_BUG_MODIFY
						g_Pc.GetPlayer()->m_curt_event.dKeepSkillCode = skill_Info->theSkillCode;
						g_Particle.m_EffectManager.Create( ( EFFECT_ID ) skill_Info->theKeepupEffect , g_Pc.GetPlayer() , NULL );
					}
				}

				///--JAPAN_BUG_MODIFY
				g_Pc.GetPlayer()->m_curt_event.dKeepSkillCode = 0;
			}
		}
		//.....................................................................................................		
	}
	
	//QuestFlag Setting
	for( i = 0; i < QuestFlagNum; ++i )
	{
		SAFE_DELETE(thePcInven.QuestFlag[i]);
		
		thePcInven.QuestFlag[i] = SAFE_NEW( SQuestFlag );		
		thePcInven.QuestFlag[i]->theQuestCode = QuestFlag[i].theQuestCode;
		thePcInven.QuestFlag[i]->theFlag = QuestFlag[i].theFlag;
		thePcInven.QuestFlag[i]->theDisplay = QuestFlag[i].theDisplay;
	}	
	
	//Quick Slot - Link ( 다른 슬롯에 있는 아이템과 스킬 링크 )
	int NumInven = InvenItem1Num + InvenItem2Num;
	DWORD theID = 0;
	
	for( i = 0; i < RP_MAX_QUICK_SLOT; ++i )
	{
		if( thePcInven.QuickSlot[i] != NULL )
		{
			thePcInven.QuickSlot[i] = NULL;	
		}
	}
	
	for( i = 0; i < QuickSlotNum; ++i )
	{
		// QuickSlot에 링크되어있는 아이템을 InvenItem이나 SkillItemd에서 찾는다.
		if( QuickSlot[i].theType == n_ITEM )
		{
			theID = QuickSlot[i].theID.theItemID;
			
			for( j = 0; j < c_MaxInvenItemNum; ++j )
			{
				if( thePcInven.InvenItem[j] == NULL )
				{
					continue;
				}
				if( theID == thePcInven.InvenItem[j]->ItemUID )
				{
					// Item을 찾았으면 해당되는 SPcItem의 포인터를 링크(대입)시킨다.
					thePcInven.QuickSlot[QuickSlot[i].thePosition] = thePcInven.InvenItem[j];
					break;
				}
			}
		}
		else if( QuickSlot[i].theType == n_SKILL )
		{
			theID = QuickSlot[i].theID.theSkillID;
			
			for( j = 0; j < RP_MAX_USER_SKILL; ++j )
			{
				if( thePcInven.SkillItem[j] == NULL )
				{
					continue;
				}
				if( theID == thePcInven.SkillItem[j]->Code )
				{
					// Item을 찾았으면 해당되는 SPcItem의 포인터를 링크(대입)시킨다.
					thePcInven.QuickSlot[QuickSlot[i].thePosition] = thePcInven.SkillItem[j];
					break;
				}
			}
		}
	}
	
	//by dongs 곳바로 수정해야함 .. 퀘스트 시간 .. 초기화 
	for( i =0 ; i < RP_MAX_QUEST_COUNT ; ++i )
	{
		SAFE_DELETE(thePcInven.QuestTime[i]);
	}	
	
	for( i = 0; i < QuestCountNum; ++i )
	{
		SAFE_DELETE( thePcInven.QuestCount[i] );
		
		thePcInven.QuestCount[i] = SAFE_NEW( SQuestCount );		
		thePcInven.QuestCount[i]->theQuestCode = QuestCount[i].theQuestCode;
		thePcInven.QuestCount[i]->theType = QuestCount[i].theType;
		thePcInven.QuestCount[i]->theCode = QuestCount[i].theCode;
		thePcInven.QuestCount[i]->theCount = QuestCount[i].theCount;		
		thePcInven.QuestCount[i]->theTotalCount = QuestCount[i].theTotalCount;		
		
		//by dongs 퀘스트 에 시간이 추가된다 11 번이 시간이다 .. n_TIME Ok...
		if(thePcInven.QuestCount[i]->theType == n_TIME )
		{
			BYTE *pTempBuff = (BYTE*)thePcInven.QuestCount[i];
			int iTempTime = 0 ; 
			for(int cnt =	0; cnt < QuestCountNum; ++cnt )
			{				
				if( thePcInven.QuestTime[cnt] == NULL)
				{
					thePcInven.QuestTime[cnt] = SAFE_NEW( SQuestTime );					
					thePcInven.QuestTime[cnt]->wdQuestCode = thePcInven.QuestCount[i]->theQuestCode;
					memcpy(&iTempTime , &pTempBuff[3] , 4 );
					thePcInven.QuestTime[cnt]->iCountTime = iTempTime;
					thePcInven.QuestTime[cnt]->dwStartTime = g_nowTime;					
					break;
				}				
			}
		}
	}	
	
	for( i = 0; i < PayEffectNum; ++i )
	{
		SAFE_DELETE( thePcInven.PayEffect[i] );
		
		thePcInven.PayEffect[i] = SAFE_NEW( SPayEffect );		
		thePcInven.PayEffect[i]->theItemCode = PayEffect[i].theItemCode;
#ifdef KEEPUP_PAYITEM_RENEWAL
		thePcInven.PayEffect[i]->theTime = PayEffect[i].theTime;
#else
		thePcInven.PayEffect[i]->theTime = PayEffect[i].theTime * 1000.0f;
#endif
	
	}

	//펫 초기화 이부분은 서버에서 처리해준다 나중에 .. 	
	//dongs pet 초기화 	
	
	//////////////////////////////////////////////////////////////////////////
	
	
#ifdef FRONTIER
	
	SAFE_DELETE( thePcInven.FrontierMyInfo );
	
	thePcInven.FrontierMyInfo = SAFE_NEW( SFrontier_MyInfo );	
	thePcInven.FrontierMyInfo->FrontierID = FrontierMyInfo->FrontierID;
	thePcInven.FrontierMyInfo->FrontierGrade = FrontierMyInfo->FrontierGrade;
	thePcInven.FrontierMyInfo->FrontierDues = FrontierMyInfo->FrontierDues;
	
	SAFE_STR_CPY( thePcInven.FrontierMyInfo->FrontierNick, FrontierMyInfo->FrontierNick, RP_MAX_FRONTIER_NICK );
	
#endif
	
#ifdef RIDING_SYSTEM	
	//Ride Slot - Setting
	for( i = 0; i < RidingNum * RP_MAX_RIDINGITEM ; ++i )
	{
		
		if(RidingItem[i].theCode == 0)
		{
			continue;
		}			

		pcitem = SAFE_NEW( SPcItem );	
		
		if( g_Pc_Manager.GetItemClsss( RidingItem[i].theCode ) == n_Class_Riding )
		{
			pcitem->IType	= n_ITYPE_RIDE;
		}
		else if( g_Pc_Manager.GetItemClsss( RidingItem[i].theCode ) == n_Class_RidingItem )
		{
			pcitem->IType	= n_ITYPE_RIDE_WEAR;
		}
	
		
		pcitem->Class		= g_Pc_Manager.GetItemClsss( RidingItem[i].theCode );
		pcitem->ItemUID		= RidingItem[i].theItemUID;
		pcitem->Code		= RidingItem[i].theCode;
		pcitem->Tid			= (TEXID)g_Pc_Manager.GetItemTID( RidingItem[i].theCode );
		pcitem->Amount		= 1;
		pcitem->ItemTable	= g_Pc_Manager.GetItemBaseInfoPtr( RidingItem[i].theCode );
		memcpy( &(pcitem->WearTable), &RidingItem[i], sizeof( SRpdWearItemBase ) );
		
		SAFE_DELETE( thePcInven.InvenHorse[RidingItem[i].thePosition] );
		
		thePcInven.InvenHorse[RidingItem[i].thePosition] = pcitem;
		
#ifdef C_TIME_SUIT_UPDATE		
		if(pcitem->ItemTable &&  pcitem->ItemTable->theRare == 255 && pcitem->ItemTable->theMaxEndurance == 0 )
		{
			DWORD dwTime = 0 ; 
			for( int j = 0; j < RP_MAX_TIMELIMIT_PAY_CONTENTS ; ++j )
			{
				if(thePcInven.PayTimeItemList[j] == NULL)
				{
					//두곳 장착과 인벤 아이템 들어가 있는것. . 
			 		thePcInven.PayTimeItemList[j] = SAFE_NEW( SPayTimeItem );
					thePcInven.PayTimeItemList[j]->theItemCode = RidingItem[i].theCode;
					thePcInven.PayTimeItemList[j]->theItemID = RidingItem[i].theItemUID;
					memcpy(&dwTime , &RidingItem[i].theEndurance , sizeof(DWORD));
					//DWORD dwTime = (WearItem[j].theEndurance << 16 ) | (WearItem[i].theEldaStone);
					thePcInven.PayTimeItemList[j]->theTime =dwTime ;
					break;
				}
			}	
		}
#endif

	}
#endif
	
	//-------------------------------------------------------------------------
	// 각 윈도우 초기화....
	//-------------------------------------------------------------------------
	CharStateWnd.Init();
	MainMenuWnd.Init();
	g_Exp_wnd.Init();
	///MainMenuLVgageWnd.Init();
#ifdef TAIWAN30MINEVENT
	g_main_buttons.Init();
#endif //TAIWAN30MINEVENT
	InventoryWnd.Init();
	g_SkillWnd.Init();
	PlayerInfoWnd.Init();
	MiniMapWnd.Init();
	QuestWnd.Init();
	SkillLearnWnd.Init();
	g_UserTime.Init();

	g_LogWnd.SetEnableCombo(thePcParam.MainClass);
	
	g_SlotDelayProc.InitDelayTime( DelayTime , DelayTimeNum );	// 모든 슬롯 초기화 후 사용 (LogOff...Bug)
	
	g_FrontierMainWnd.Init();
	
	g_PlayerEtcPrc.Init();

	//by simwoosung - 임시할당된 메모리 해제
	SAFE_DELETE_ARRAY(WearItem);
	SAFE_DELETE_ARRAY(InvenItem1);
	SAFE_DELETE_ARRAY(InvenItem2);
	SAFE_DELETE_ARRAY(QuestItem);
	SAFE_DELETE_ARRAY(SkillItem);
	SAFE_DELETE_ARRAY(KeepupSkillItem);				
	SAFE_DELETE_ARRAY(QuickSlot);
	SAFE_DELETE_ARRAY(DelayTime);
	SAFE_DELETE_ARRAY(QuestFlag);	
	SAFE_DELETE_ARRAY(QuestCount);	
	SAFE_DELETE_ARRAY( PayEffect );	
	SAFE_DELETE( FrontierMyInfo );
	
#ifdef RIDING_SYSTEM
	SAFE_DELETE_ARRAY( RidingItem );	
#endif	

	return;
}

//-----------------------------------------------------------------------------
void CRui::SetPcInfo( SPcInfo* pcinfo )
{
	thePcInfo = pcinfo;
}

//-----------------------------------------------------------------------------
void CRui::SetPickPcIdx( int index )
{
	thePickPcIdx = index;
}

//-----------------------------------------------------------------------------
void CRui::DrawPoint()
{
	for( int i = 0; i < MAX_CHARACTER; ++i )
	{
		if(	( thePcInfo[i].existence == true ) &&
			( ( thePcInfo[i].spos.x >= 0 ) && ( thePcInfo[i].spos.x < 1024 ) ) &&
			( ( thePcInfo[i].spos.y >= 0 ) && ( thePcInfo[i].spos.y < 768 ) ) )
		{
			Render->DrawPoint( thePcInfo[i].spos.x, thePcInfo[i].spos.y, D3DCOLOR_XRGB( 255, 0, 0 ), 2.0f ); 
		}
	}
}

//-----------------------------------------------------------------------------
void CRui::DrawCharacterID()
{
	if( LoadingBarWnd.IsProgress == true )
	{
		return;
	}
	else if( g_RockClient.GetGameMode() == GMODE_NORMAL )
	{
		theViewPcInfo = n_VIEW_PICK_PCID;
		
		if( g_Pc.GetPlayer()->theMapType == n_FightZone )
		{
			theViewPcInfo = n_VIEW_ALL_PCID;
		}		
	}
	else if( g_RockClient.GetGameMode() == GMODE_SELECT && 
		( CharSelectWnd.m_SEQState == SEQ_SELECT_CHAR || CharSelectWnd.m_SEQState == SEQ_SELECT_ANI ) )
	{
		theViewPcInfo = n_VIEW_ALL_PCID;
	}
	else
	{
		return;
	}
	
	int i = 0;
	
	g_RenderManager.SetZEnable( TRUE );
	
	switch( theViewPcInfo )
	{
	case n_VIEW_NONE_PCID:
		break;
		
	case n_VIEW_ALL_PCID:
		for( i = 0; i < MAX_CHARACTER; ++i )
		{
			if(	( thePcInfo[i].existence == true ) &&

			#ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/24)
				
				( ( thePcInfo[i].spos.x >= 0 ) && ( thePcInfo[i].spos.x < Desktop.Width ) ) &&
				( ( thePcInfo[i].spos.y >= 0 ) && ( thePcInfo[i].spos.y < Desktop.Height ) ) ) // &&
				
			#else
				
				( ( thePcInfo[i].spos.x >= 0 ) && ( thePcInfo[i].spos.x < 1024 ) ) &&
				( ( thePcInfo[i].spos.y >= 0 ) && ( thePcInfo[i].spos.y < 768 ) ) ) // &&
				//				( thePcInfo[i].distance < ( float ) TILE_SIZE * 23 ) )
				
			#endif// #ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/24)
			{				
				
#ifdef ROCKCLIENT_DEV
				char testName_Pos[255] = {0,};
				Rsprintf( RWCHAR(testName_Pos), _RT("%s (%d, %d)(%d)"), RWCHAR(thePcInfo[i].name), ((Character*)thePcInfo[i].pCharacter)->m_Mov.curt.x, ((Character*)thePcInfo[i].pCharacter)->m_Mov.curt.y, 
					((Character*)thePcInfo[i].pCharacter)->m_Mov.curt.y * g_Pc.m_cSize + ((Character*)thePcInfo[i].pCharacter)->m_Mov.curt.x );
				
				DrawCharacterDetailInfoUIId(i);
				//DrawCharacterDetailInfoBillId(i);				
#else			
                DrawCharacterDetailInfoUIId(i);	
				//DrawCharacterDetailInfoBillId(i);			
#endif					
			}
		}
		
		break;
		
		  case n_VIEW_PICK_PCID:
			  if( thePickPcIdx != -1  )
			  {
				  if( ((Character*)thePcInfo[thePickPcIdx].pCharacter)->m_bIsVisible )
				  {
					  DrawCharacterDetailInfoUIId(thePickPcIdx);
					  //DrawCharacterDetailInfoBillId(thePickPcIdx);
				  }
			  }			  
			  
			  for( i = 0; i < MAX_CHARACTER; ++i )
			  {
				  if( thePcInfo[i].pCharacter && ( (Character*)thePcInfo[i].pCharacter )->m_bIsVisible )
				  {
					  
					  if( (thePcInfo[i].ctype == enPLAYER) || ( thePcInfo[i].ctype == enNPC ) || 
						  IsNonPlayerCode( thePcInfo[i].npc_code ) ||
						  ( (Character*)thePcInfo[i].pCharacter )->m_State.nPetState == nPetState_Pet ||
						  ( (Character*)thePcInfo[i].pCharacter )->m_State.nHolyState == nHolyState_Holy || 
						  IsSameNationUnit( thePcInfo[i].pCharacter ) )
					  {
						  //5300 ~5500 아직 NPC 가 추가 안됨 ... 그래서 ... 통과!!!!!
						  if(IsObjectNPCCode(thePcInfo[i].npc_code))
							  continue;
						  
						  if( ( thePcInfo[i].existence == true ) &&

							#ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/24)
							  
							  ( ( thePcInfo[i].spos.x >= 0 ) && ( thePcInfo[i].spos.x < Desktop.Width ) ) &&
							  ( ( thePcInfo[i].spos.y >= 0 ) && ( thePcInfo[i].spos.y < Desktop.Height ) ) && 
							  
							#else
							  
							  ( ( thePcInfo[i].spos.x >= 0 ) && ( thePcInfo[i].spos.x < 1024 ) ) &&
							  ( ( thePcInfo[i].spos.y >= 0 ) && ( thePcInfo[i].spos.y < 768 ) ) && 
							  
							#endif// #ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/24)

							  ( thePcInfo[i].distance < ( float ) TILE_SIZE * 23 )					)
						  {
							  if(i == thePickPcIdx)
								  continue;
							  //dongs 추가 캐릭터 ID 감추기 --1의  경험치를 획득 하셨습니다 .. --
							  //char ID == false 이면 상대방 캐릭터 만 보이지 않게 한다 .. 
							  if(g_ISCharShowIDVisible == true )
							  {
								  DrawCharacterDetailInfoUIId(i);
							 }
							  else if(g_ISCharShowIDVisible == false && IsNonPlayerCode( thePcInfo[i].npc_code ))
							  {
								  DrawCharacterDetailInfoUIId(i);
							  }
							  
							  //DrawCharacterDetailInfoBillId(i);				
						  }
					  }
				  }
			  }			  			  
			  break;
	}
	
	g_RenderManager.SetZEnable( FALSE );
}

void CRui::DrawCharacterDetailInfoBillId(int index)
{
	
}


void CRui::DrawCharacterDetailInfoUIId(int index)
{
    if( !m_CharIdRenderer.SetIndex(index) )
	{
		return;
	}
	
	m_CharIdRenderer.SetCharName(thePcInfo[index].name);
	m_CharIdRenderer.SetPartyName(thePcInfo[index].partyname);
	
#ifdef FRONTIER	
	if(((Character*)thePcInfo[index].pCharacter)->m_bPC == true)
	{
		if(thePcInfo[index].FrontierID)
		{
			m_CharIdRenderer.SetFrontierId(thePcInfo[index].FrontierID);
			m_CharIdRenderer.SetCallName(thePcInfo[index].FrontierNick);
			m_CharIdRenderer.SetFrontierName(thePcInfo[index].FrontierName);
			m_CharIdRenderer.SetFrontierStatus(thePcInfo[index].Status);
		}		
	}
	else
	{
		if(thePcInfo[index].FrontierID)
		{
			m_CharIdRenderer.SetFrontierId(thePcInfo[index].FrontierID);
			m_CharIdRenderer.SetFrontierName(thePcInfo[index].FrontierName);
		}
	}
#endif

#ifdef CPK_SYSTEM
	
	if(((Character*)thePcInfo[index].pCharacter)->m_bPC == true)
	{
		m_CharIdRenderer.SetCaoMark( thePcInfo[index].IsChaosChar );		
	}

#endif
	
	if( g_Pc.GetPlayer()->theMapType == n_FightZone )		//맵타입이 배틀존일경우
	{
		if( g_BZ_WaitingRoomWnd.m_RoomInfo.theGameType == n_Survival )  //게임타입이 서바이벌 일경우
		{
			if( ((Character*)thePcInfo[index].pCharacter)->m_nTableIndex == MAX_USER_INTABLE )
			{
				m_CharIdRenderer.SetPartyColor( D3DCOLOR_XRGB( 200, 200, 255 ) );
				m_CharIdRenderer.SetNameColor( D3DCOLOR_XRGB( 230, 230, 88 ) );
			}
			else
			{
				m_CharIdRenderer.SetPartyColor( D3DCOLOR_XRGB( 255, 100, 100 ) );
				m_CharIdRenderer.SetNameColor( D3DCOLOR_XRGB( 255, 100, 100 ) );
			}
		}
		else //서바이벌이 아닐경우
		{			
			if( ((Character*)thePcInfo[index].pCharacter)->m_bPC == true )
			{
				if( ((Character*)thePcInfo[index].pCharacter)->m_nTableIndex == MAX_USER_INTABLE )
				{
					m_CharIdRenderer.SetPartyColor( D3DCOLOR_XRGB( 200, 200, 255 ) );
					m_CharIdRenderer.SetNameColor( D3DCOLOR_XRGB( 230, 230, 88 ) );
				}
				else
				{
					if( g_Pc.m_MainPC.nFlag == g_Pc.m_PcTable[ ((Character*)thePcInfo[index].pCharacter)->m_nTableIndex ].nFlag )
					{
						m_CharIdRenderer.SetPartyColor( D3DCOLOR_XRGB( 200, 200, 255 ) );
						m_CharIdRenderer.SetNameColor( D3DCOLOR_XRGB( 230, 230, 88 ) );
					}
					else
					{
						m_CharIdRenderer.SetPartyColor( D3DCOLOR_XRGB( 255, 100, 100 ) );
						m_CharIdRenderer.SetNameColor( D3DCOLOR_XRGB( 255, 100, 100 ) );
					}
				}
			}
			else
			{ 
				if( g_BZ_WaitingRoomWnd.m_RoomInfo.theGameType == n_CapturetheHill )
				{
					int i = 0;  
					int npc_idx = 0;
					
					m_CharIdRenderer.SetPartyColor( D3DCOLOR_XRGB( 255, 100, 100 ) );
					m_CharIdRenderer.SetNameColor( D3DCOLOR_XRGB( 255, 100, 100 ) );
					
					for( i = 0; i < c_Max_FightMap_Num; ++i )
					{
						if( g_Pc_Manager.theFightMapInfoTable[i].theMapCode == g_Map.m_NowMapID )
						{
							if( g_Pc.m_MainPC.nFlag == 0 )
							{
								for( npc_idx = 0; npc_idx < 6; ++npc_idx )
								{
									if( g_Pc_Manager.theFightMapInfoTable[i].theNPCPosition[npc_idx].theNPCCode == 
										g_Map.m_NpcTable[ ((Character*)thePcInfo[index].pCharacter)->m_nTableIndex ].code )
									{
										m_CharIdRenderer.SetPartyColor( D3DCOLOR_XRGB( 200, 200, 255 ) );
										m_CharIdRenderer.SetNameColor( D3DCOLOR_XRGB( 230, 230, 88 ) );
									}
								}						
							}
							else if( g_Pc.m_MainPC.nFlag == 1 )
							{
								for( npc_idx = 6; npc_idx < 12; ++npc_idx )
								{
									if( g_Pc_Manager.theFightMapInfoTable[i].theNPCPosition[npc_idx].theNPCCode == 
										g_Map.m_NpcTable[ ((Character*)thePcInfo[index].pCharacter)->m_nTableIndex ].code )
									{
										m_CharIdRenderer.SetPartyColor( D3DCOLOR_XRGB( 200, 200, 255 ) );
										m_CharIdRenderer.SetNameColor( D3DCOLOR_XRGB( 230, 230, 88 ) );
									}
								}						
							}	
							break;
						}
					}
				}
				else
				{
					m_CharIdRenderer.SetPartyColor( D3DCOLOR_XRGB( 255, 100, 100 ) );
					m_CharIdRenderer.SetNameColor( D3DCOLOR_XRGB( 255, 100, 100 ) );
				}
			}
		}
	}	
	else
	{
		if( ((Character*)thePcInfo[index].pCharacter)->m_bPC == false )
		{
			SNPCBaseInfo* npc_info = NULL;		
			npc_info = g_Pc_Manager.GetNPCBaseInfoPtr( g_Map.m_NpcTable[ ((Character*)thePcInfo[index].pCharacter)->m_nTableIndex ].code ); 
			
			if( npc_info && npc_info->theGender == 3 )				// 보스몹 define 된게 없네...잠시...걍..^ ^;;
			{
				m_CharIdRenderer.SetPartyColor( D3DCOLOR_XRGB( 255, 50, 50 ) );
				m_CharIdRenderer.SetNameColor( D3DCOLOR_XRGB( 255, 100, 100 ) );
			}
			else
			{
				// NPC 경우 다른 색깔로 구분 표시 
				if( g_Map.m_NpcTable[ ((Character*)thePcInfo[index].pCharacter)->m_nTableIndex ].code >= c_NonplayerCode_Min &&
					g_Map.m_NpcTable[ ((Character*)thePcInfo[index].pCharacter)->m_nTableIndex ].code <= c_NonplayerCode_Max )
				{
					m_CharIdRenderer.SetPartyColor( D3DCOLOR_XRGB( 100, 200, 100 ) );
					m_CharIdRenderer.SetNameColor( D3DCOLOR_XRGB( 55, 255, 55 ) );
				}
				else if( ((Character*)thePcInfo[index].pCharacter)->m_State.nPetState == nPetState_Pet ||
					((Character*)thePcInfo[index].pCharacter)->m_State.nHolyState == nHolyState_Holy )
				{
					m_CharIdRenderer.SetPartyColor( D3DCOLOR_XRGB( 200, 200, 255 ) );
					m_CharIdRenderer.SetNameColor( D3DCOLOR_XRGB( 230, 230, 88 ) );
				}
				else if( IsSameNationUnit( thePcInfo[index].pCharacter ) )
				{
					m_CharIdRenderer.SetPartyColor( D3DCOLOR_XRGB( 200, 200, 255 ) );
					m_CharIdRenderer.SetNameColor( D3DCOLOR_XRGB( 230, 230, 88 ) );
				}
				else
				{
					m_CharIdRenderer.SetPartyColor( D3DCOLOR_XRGB( 200, 50, 155 ) );
					m_CharIdRenderer.SetNameColor( D3DCOLOR_XRGB( 255, 125, 155 ) );
				}
			}
		}		
		else
		{			
			m_CharIdRenderer.SetPartyColor( D3DCOLOR_XRGB( 200, 200, 255 ) );
			
			if( ((Character*)thePcInfo[index].pCharacter)->m_nTableIndex == MAX_USER_INTABLE )
			{
				m_CharIdRenderer.SetNameColor( CharStateWnd.FameColor(thePcParam.Fame) );
			}
			else
			{
				m_CharIdRenderer.SetNameColor( CharStateWnd.FameColor(g_Pc.m_PcTable[((Character*)thePcInfo[index].pCharacter)->m_nTableIndex].char_info.theFame));
			}			
		}		
	}
	
	m_CharIdRenderer.RecallLayout();
	m_CharIdRenderer.RenderProc();
	
	return;
}

//-----------------------------------------------------------------------------
void CRui::DrawPcItem()
{
	static char str[20] = "";
	static SStringInfo	StrInfo;
	static SRect item;
	static int Ax = 0;
	static int Ay = 0;
	
	static UINT tid = 0;
	
	static BOOL IsItemCursor = FALSE;
	
	if( thePcItem == NULL )
	{
		tid = 0;
		if(IsItemCursor && m_IsSurpportHCursor)
		{
			SetDeviceCursor(theCursor->img[0] , false);
			IsItemCursor = FALSE;		  
		}
	}
	
	if( thePcItem != NULL )
	{
		
		if(m_IsSurpportHCursor)
		{
			if(tid == thePcItem->Tid)
				return;
			
			tid = thePcItem->Tid;
			SetDeviceCursor(thePcItem->Tid, true);
		}
		
		IsItemCursor = TRUE;
		
		if(!m_IsSurpportHCursor && !g_RockClient.m_IsMouseRotate)
		{
			item.x	= theCursor->pos.x - 18;
			item.y	= theCursor->pos.y - 18;
			item.w	= 35;
			item.h	= 35;		
			
			if( g_RockClient.m_SceneManager.GetCursorDisplay() )
			{
				Render->Draw( thePcItem->Tid, item.x, item.y, RD_DEFAULT, RD_DEFAULT, D3DCOLOR_XRGB( 255, 255, 255 ) );
			}			
			
			/*if( ( thePcItem->Amount > 1 ) && ( GetItemSelectedSlot()->Group != n_SGROUP_STORE ) )//
			{
			//아이템의 갯수 표시( 번들 아이템일경우 )
			sprintf( str, _T("%d"), thePcItem->Amount );
			theFontg2->GetStringInfo( str, &StrInfo );
			
			  Ax = 35 - StrInfo.Width;
			  Ax += item.x;
			  Ay = 35 - StrInfo.Height;
			  Ay += item.y;
			  //Render->DrawAlignText( theFontg2, str, Ax, Ay, item.x, item.y, item.w, item.h, D3DCOLOR_XRGB( 255, 255, 255 ) );
		}*/
		}
	}
}

//-----------------------------------------------------------------------------
void CRui::SetCursorState()
{
/*
n_CT_NORMAL,	//노멀			: 기본커서
n_CT_HAND,		//손			: 바닥이나, 인벤토리의 아이템 위에 커서가 위치할 때
n_CT_MOUTH,		//입			: 대화가 가능한 NPC 위에 커서가 위치할 때
n_CT_ITEMDROP	//아이템드롭	: 인벤토리 안에서 아이템을 집어, 인벤토리 밖에 커서가 위치할 때

  //공격가능인가 아닌가?
  //컨트롤 누른상태에서는 다른 유저 공격 가능
  n_CT_SWORD,		//칼			: 칼을 장비하고, 공격가능한 대상 위에 커서가 위치할 때
  n_CT_ARROW,		//화살			: 활을 장비하고, 공격가능한 대상 위에 커서가 위치할 때사정거리 밖일 경우에는 별도로 표시
  n_CT_ARROW_X,	//화살X			: 활을 장비하고, 공격가능한 대상 위에 커서가 위치할 때사정거리 밖일 경우에는 별도로 표시
  n_CT_MAGIC,		//스			: 마법을 시전하고, 공격가능한 대상 위에 커서가 위치할 때사정거리 밖일 경우에는 별도로 표시
  n_CT_MAGIC_X,	//마법X			: 마법을 시전하고, 공격가능한 대상 위에 커서가 위치할 때사정거리 밖일 경우에는 별도로 표시
  n_CT_SKILL,		//스킬			: 대상이 필요한 스킬을 사용 후, 그 대상을 지정할 때
  
	//처리 안함....
	n_CT_FOOT,		//발			: 이동 가능한 필드 위에 커서가 위치할 때
	n_CT_FOOT_X,	//발X			: 이동 불가능한 필드 위에 커서가 위치할 때
	n_CT_PORTAL,	//포탈			: 포탈이 위치한 곳에 커서가 위치할 때
	n_CT_TXTREAD,	//글읽기		: 표지판이나 글씨가 있는 오브젝트 위에 커서가 위치할 때
	n_CT_TXTREAD_X,	//글읽기X		: 너무 멀어서 읽을 수 없는 위치에 있는 표지판이나 글씨에 커서가 위치할 때
	//*/
	
	//by simwoosung
	if(g_RockClient.m_IsMouseRotate)	//마우스가 회전중이면
	{
		return;
	}
	
	if( m_bOnMiniGame && theMiniGame)	//미니게임 동작중이면
	{
		return;
	}
				
	if( g_RockClient.m_MouseControlType == n_MouseType_UseSkill )
	{
		SetCursorType( n_CT_SWORD );
		return;
	}
	
	if(MiniMapWnd.Map->m_bIsPress)
	{
		SetCursorType(n_CT_HAND);
		return;
	}	
	
	theCursorState = n_CT_NORMAL;
	
	if( //( nRui->theCursorState != n_CT_ITEMDROP )|| 
		( GetCursorItem() != NULL ) )
	{
		return;
	}
	
	//커서 위치에 대화할 수 있는 NPC가 있으면....true설정
	//char str[40] = "";
	if( (FocusPwnd == WID_None) || 		
		(FocusPwnd == WID_ChatActFrame) ||
		(FocusPwnd == WID_ChatNonActFrame) ||
		(FocusPwnd == WID_SystemPrintFrame) )//창 뒤에 있다면...커서 처리를 하지 않는다.
	{
		if( ( thePickPcIdx != -1 ) && ( theCursorState != n_CT_ITEMDROP ) )
		{
			if( thePcInfo[thePickPcIdx].ctype == enNPC )
			{
				theCursorState = n_CT_NORMAL;
				
				Character * pCharacter = (Character *)(thePcInfo[thePickPcIdx].pCharacter);
				if( pCharacter->m_curt_event.status == SM_SELLSKILL_SITDOWN ||
					pCharacter->m_curt_event.status == SM_SELLSKILL_KEEPING ||
					pCharacter->m_curt_event.status == SM_SELLSKILL_STANDUP )
				{
					theCursorState = n_CT_SHOP;
				}

				if( g_RockClient.CanAttackPlayer( pCharacter ) )
				{
					theCursorState = n_CT_SWORD;
				}
			}
			else if( thePcInfo[thePickPcIdx].ctype == enMONSTER )
			{
				if( ( thePcInfo[thePickPcIdx].npc_code >= c_NonplayerCode_Min ) && 
					( thePcInfo[thePickPcIdx].npc_code <= c_NonplayerCode_Max ) )
				{
					theCursorState = n_CT_MOUTH;//대화 가능한 NPC이면....
				}
				else
				{	
					if( thePcInfo[thePickPcIdx].race == c_NpcRace_Mob )//몬스터
					{
						theCursorState = n_CT_SWORD;						
						
						Character * pCharacter = (Character *)(thePcInfo[thePickPcIdx].pCharacter);
						
						if( pCharacter && !pCharacter->m_bPC )
						{
							if( pCharacter->m_State.nHolyState == nHolyState_Holy )
							{
								theCursorState = n_CT_NORMAL;
							}

							if( pCharacter->m_State.nPetState == nPetState_Pet )
							{
								theCursorState = n_CT_NORMAL;
							}

							if( IsSameNationUnit( (void *)(pCharacter) ) )
							{
								theCursorState = n_CT_NORMAL;
							}
						}						
					}
					else if( thePcInfo[thePickPcIdx].race == c_NpcRace_Item )//라이브아이템
					{
						theCursorState = n_CT_HAND;
					}
				}
			}
			else if(thePcInfo[thePickPcIdx].ctype == enItem)	//아이템이 picking되었을시
			{
				theCursorState = n_CT_HAND;
			}
			else
			{
				theCursorState = n_CT_NORMAL;
			}
		}
		else
		{
			if( ( theCursorState == n_CT_HAND ) || 
				( theCursorState == n_CT_MOUTH ) || 
				( theCursorState == n_CT_SWORD ) ||
				( theCursorState == n_CT_SHOP ) )
			{
				theCursorState = n_CT_NORMAL;
			}
		}
	}
	else		
	{
		if( ( theCursorState == n_CT_HAND ) || 
			( theCursorState == n_CT_MOUTH ) || 
			( theCursorState == n_CT_SWORD ) ||
			( theCursorState == n_CT_SHOP ) )
		{
			theCursorState = n_CT_NORMAL;
		}
	}	
	
	SetCursorType( (NCursorType)theCursorState );	
	
	//-------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------
void CRui::SetCursorType( NCursorType cursor_type )
{
	if( theCursorType == cursor_type )
	{
		return;
	}
	
	int x				= theCursor->pos.x;
	int y				= theCursor->pos.y;
	theCursorType		= cursor_type;
	theCursorAniIdx		= 0;			//타이밍 때문에...0이 아닌 1부터 시작....?????
	
	theCursor			= Resource->GetCursorInfo( cursor_type );
	theCursor->pos.x	= x; 
	theCursor->pos.y	= y;
	
	if( thePcItem == NULL )
	{
		if(m_IsSurpportHCursor)
		{
			SetDeviceCursor(theCursor->img[0], false);
		}
	}
}

//-----------------------------------------------------------------------------
void CRui::SetCursorItem( SPcItem* pcitem )
{
	if( pcitem == NULL )
	{
		InventoryWnd.ResetWearSlotHighlight();// 장비 슬롯의 하이라이트를 끈다....
		
		//NULL이 들어오는건 아이템이 떨어지는것으로 간주한다...
		if( thePcItem != NULL )
		{
			if( thePcItem->IType == n_ITYPE_SKILL )
			{
				g_Sound.Play( 28 );//스킬 놓기 사운드
				
				
			}
			else
			{
				if( thePcItem->ItemTable )
				{
					g_Sound.Play( thePcItem->ItemTable->theSoundFile );//떨어뜨릴때는 아이템 고유 사운드 출력
				}
			}
			
			thePcItem = NULL;
		}
	}
	else
	{
		thePcItem = pcitem;
		g_Sound.Play( 27 );//아이템, 스킬 집기 사운드
		SetCursorType( n_CT_HAND );
	}
}




//-----------------------------------------------------------------------------
SPcItem* CRui::GetCursorItem()
{
	return( thePcItem );
}

//-----------------------------------------------------------------------------
void CRui::SetItemSelectedSlot( CSlot* slot )
{
	if( slot )
	{
		theSlot.Group         = slot->Group;       
		theSlot.SlotNum       = slot->SlotNum;
		theSlot.Class         = slot->Class;       
		theSlot.PcItem        = slot->PcItem;      
		theSlot.theSlotState  =	slot->theSlotState;
		
		theSlot.SlotAddr      = slot;
	}
	else
	{
		theSlot.Clear();
	}
	return;
}

void CRui::ReSetTIDImage()
{
	//인벤 검사 있냐 없냐 ? 
	for( int k =0 ; k < c_MaxInvenItemNum; ++k )
	{
		if(thePcInven.InvenItem[k] == NULL ) 
		{
			continue;
		}
		
		thePcInven.InvenItem[k]->Tid = (TEXID)g_Pc_Manager.GetItemTID( thePcInven.InvenItem[k]->Code );								
	}
							
	//장비 인벤 검사 
	for( int j=0 ; j < RP_MAX_EQUIP; ++j )
	{
		if(thePcInven.WearItem[j] == NULL) 
		{
			continue;
		}
		
		thePcInven.WearItem[j]->Tid = (TEXID)g_Pc_Manager.GetItemTID( thePcInven.WearItem[j]->Code );
	}
							
	//퀘스트인벤 검사 
	for( int qcnt=0 ; qcnt < RP_MAX_QUEST_ITEM; ++qcnt )
	{
		if(thePcInven.InvenQuest[qcnt] == NULL) 
		{
			continue;
		}
		
		thePcInven.InvenQuest[qcnt]->Tid = (TEXID)g_Pc_Manager.GetItemTID( thePcInven.InvenQuest[qcnt]->Code );		
	}
}

//-----------------------------------------------------------------------------
CSlot* CRui::GetItemSelectedSlot()
{
	return( &theSlot );
}

//-----------------------------------------------------------------------------
void CRui::ResetCursorItem()
{
	SetCursorItem( NULL );
	SetItemSelectedSlot( NULL );
}

//-----------------------------------------------------------------------------
SPcInfo* CRui::GetPcInfo()
{
	return( thePcInfo );
}

//-----------------------------------------------------------------------------
SPcDataParam* CRui::GetPcParamInfo()
{
	return( &thePcParam );
}

//-----------------------------------------------------------------------------
SPcDataInven* CRui::GetPcInvenInfo()
{
	return( &thePcInven );
}
///---------------------------------------------------------------------------
///-- StartUpMiniGame
///---------------------------------------------------------------------------
///-- 2005.05.04 / Lyul
///-- 해당 Mini_game 을 실행한다.
void CRui::MiniGameStartUp( const int ai_game_num, const int ai_arg1 , BYTE theGameTimeRatio)
{		
	SAFE_DELETE(theMiniGame);
	
	theMiniGame = SAFE_NEW( CMiniGame_Elda );	
	m_bOnMiniGame = true;
	theMiniGame->Logic_Prepare_Game( ai_arg1 , ai_game_num, theGameTimeRatio);
	
	///-- !!!! mini_game.
}

BOOL	CRui::GetIsKeepUpFindSKill(const int Code)
{
	// 나의 지속스킬을 찾음 .. 
	for( int i =0 ; i < RP_MAX_KEEPUP_SKILL ; ++i)
	{
		//은신 감지비약 .이다 .. 
		if(nRui->GetPcInvenInfo()->KeepupSkill[i] == Code )
		{
			return true ; 
		}
	}

	return false;
}
///---------------------------------------------------------------------------
///-- MiniGameEnd
///---------------------------------------------------------------------------
///-- 2005.05.04 / Lyul
///-- Mini_Game 을 종료하고 창을 없엔다.
void CRui::MiniGameEnd( const int ai_arg1 )
{
	m_bOnMiniGame = false;		
	SAFE_DELETE( theMiniGame );	
}

void CRui::UpdateMaxGage(WORD aSkill_Code, bool IsGet)
{
	
	BYTE MainClass = thePcParam.MainClass;
	int	 nSkillCategory = aSkill_Code / 10;
	bool IsGageUpdateSkill = false;
	int  nGageIndex = 1;
	int  nGageStep = 0;				
	
	//이부분 하드코딩 ...;;;;;;;
	switch(MainClass)
	{
	case Magician:
	case Prominas:
	case Priest:
	case Holy_Avenger:
		{
			//프로이마의 이해
			if( nSkillCategory == 155 )
			{
				IsGageUpdateSkill = true;
				nGageIndex = 1;
			}
		}
		break;
	case Shaman:
	case Psyche:
		{
			//동기화
			if( nSkillCategory == 180 )
			{
				IsGageUpdateSkill = true;
				nGageIndex = 1;
			}				
		}
		break;
	case Swordman:
	case Knight:
	case Mercenary:
	case Slayer:
		{
			//검술
			
			
			if( nSkillCategory == 10 )
			{
				IsGageUpdateSkill = true;
				nGageIndex = 1;
			}
			
			//대검술
			
			
			if( nSkillCategory == 11 )
			{
				IsGageUpdateSkill = true;
				nGageIndex = 2;
			}
		}
		break;
	case Archer:
	case Sniper:
		{
			//아처의 마음가짐
			
			
			if( nSkillCategory == 25 )
			{
				IsGageUpdateSkill = true;
				nGageIndex = 1;
			}
		}
		break;
	}	
	
	if(IsGageUpdateSkill == false)
		return;
	
	nGageStep = aSkill_Code - (nSkillCategory * 10);
	
	if(IsGet)
	{
		UpdateGetMaxGage(nGageStep, nGageIndex);
	}
	else
	{
		UpdatePutMaxGage(nGageStep, nGageIndex);
	}
}

void CRui::UpdateGetMaxGage(int nGageStep, int nGageIndex)
{
	int nMaxGage = 0;
	
	switch(nGageStep)
	{
	   case 1:	nMaxGage = 5;	break;
	   case 2:  nMaxGage = 10;	break;
	   case 3:  nMaxGage = 15;  break;
	   case 4:  nMaxGage = 25;  break;	
	   case 5:  nMaxGage = 35;  break;
	   case 6:	nMaxGage = 50;	break;
	}
	
	if(nGageIndex == 1)
	{
		thePcParam.theMaxGage1 = nMaxGage;
	}
	
	if(nGageIndex == 2)
	{
		thePcParam.theMaxGage2 = nMaxGage;
	}
}

void CRui::UpdatePutMaxGage(int nGageStep, int nGageIndex)
{
	if(nGageIndex == 1)
	{
		thePcParam.theMaxGage1 = 0;
	}
	
	if(nGageIndex == 2)
	{
		thePcParam.theMaxGage2 = 0;
	}
}

void CRui::LogOff()
{
	m_Combo2DTimingEffect.LogOff();
	g_UserTime.SetLock(false);
	
}



void CRui::SetReadingStr(RTCHAR *Str)
{
	Rsprintf(RWCHAR(StrTextReading), _RT("%s") ,RWCHAR(Str) );
}

void CRui::Render_Reading()
{
	int i;
	SStringInfo ls_strinfo;
	
	ml_width_max = 0;
	const int ReadingSize = 4;
	
	if ( *StrTextReadingTemp[0] == _RT('\0') ) 
		return;
	
	///-- Candidate list Back
	for( i = 0 ; i < ReadingSize ; ++i )
	{
		mc_font_candisel->GetStringInfo( StrTextReadingTemp[i], &ls_strinfo );
		
		if( ml_width_max < ls_strinfo.Width )
		{
			ml_width_max = ls_strinfo.Width;
		}
	}
	
	///-- Candidate List Position
	///-- Candidata List가 찍히는 윈도우의 Y좌표 - ml_list_y
	///-- Candidate List가 찍히는 페이지의 Y좌표 - ml_page_y
	if( ml_candi_y  > 600 )
	{
		ml_list_y  = ml_candi_y - (5 * 15);		
	}
	else
	{
		ml_list_y = ml_candi_y + 10;
	}
	
	
	///-- Candidate Back  - Candidate List 배경 그리기 
	Render->Draw( TID_CLIENT, ml_candi_x - 4, ml_list_y + 5, ml_width_max + 12, ReadingSize * 15 + 8, D3DCOLOR_ARGB( 100, 255, 255, 0 ) );	
	Render->Draw( TID_CLIENT, ml_candi_x - 3, ml_list_y + 6, ml_width_max + 10, ReadingSize * 15 + 6, D3DCOLOR_ARGB( 200, 70, 105, 220 ) );	
	
	///-- Candidate List String - Candidate List 문자열 그리기
	
	
	for( i = 0 ; i < 4 ; ++i )
	{
		CHAR Strtmp[40] = {0,};
		wcscpy(reinterpret_cast<WCHAR*>(Strtmp) , StrTextReadingTemp[i] );
		
		///-- Selected String - 현재 문자열이 - 선택된 문자열이라면
		Render->DrawText( mc_font_candi, Strtmp , ml_candi_x +2, ml_list_y + 15 * i + 10,
			D3DCOLOR_XRGB( 255, 255, 255 ) );	
		
	}
	
}

bool CRui::ReadingProcess()
{
	int TextLen = 0 ; 
	
	TextLen = wcslen(StrTextReading);
	ZeroMemory(StrTextReadingTemp , sizeof(StrTextReadingTemp));
	if( TextLen == 0 ) return false;
	
	WCHAR* begin = StrTextReading;
	WCHAR* end = StrTextReading + TextLen;
	
	for(int i =0 ; i < 4 && begin<end ; ++i )
	{
		WCHAR *Next = CharNextW(begin); 
	
		wcsncpy( StrTextReadingTemp[i] ,  &begin[(Next-begin)-1] , 1 );
			
		begin = Next;
	}
	return true; 
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////


void CRui::GetPrivateReadingString(HWND hEdit)
{
	//m_reading.resize();
	HIMC hImc = ImmGetContext(hEdit);
	if(!hImc)
	{
		m_reading.resize(0);
	}
	
	DWORD dwErr = 0 ; 
	
	   // IMEs that doesn't implement Reading String API


   if( _GetReadingString ) {
	   
	   UINT uMaxUiLen =0  ;
	   BOOL bVertical = 0 ;
	   // Obtain the reading string size
	   int wstrLen = _GetReadingString( hImc, 0, NULL, (PINT)&dwErr, &bVertical, &uMaxUiLen );
	   
	   if( wstrLen == 0 ) {
		   m_reading.resize(0);
	   } else {
		   wchar_t *wstr = (wchar_t*)alloca(sizeof(wchar_t) * wstrLen);
		   _GetReadingString( hImc, wstrLen, wstr, (PINT)&dwErr, &bVertical, &uMaxUiLen );
		   m_reading.assign(wstr, wstr+wstrLen);
	   }
	   
	   m_bVerticalReading = bVertical ? true : false;
	   
	   ImmReleaseContext(hEdit, hImc);
	   
   } 
   else 
   {
	   wchar_t* temp;
	   DWORD tempLen;
	   bool bUnicodeIme = false;
	   INPUTCONTEXT *lpIC = g_Imm.LockIMC(hImc);
	   
	   if(lpIC == NULL) 
	   {
		   temp = NULL;
		   tempLen = 0;
	   }
	   else 
	   {
		   LPBYTE p = 0;
		   switch( m_dwId[0] )
		   {
		   case IMEID_CHT_VER42: // New(Phonetic/ChanJie)IME98  : 4.2.x.x // Win98
		   case IMEID_CHT_VER43: // New(Phonetic/ChanJie)IME98a : 4.3.x.x // WinMe, Win2k
		   case IMEID_CHT_VER44: // New ChanJie IME98b          : 4.4.x.x // WinXP
			   p = *(LPBYTE *)((LPBYTE)g_Imm.LockIMCC( lpIC->hPrivate ) + 24 );
			   if( !p ) break;
			   tempLen = *(DWORD *)( p + 7 * 4 + 32 * 4 );
			   dwErr = *(DWORD *)( p + 8 * 4 + 32 * 4 );
			   temp = (wchar_t *)( p + 56 );
			   bUnicodeIme = true;
			   break;
			   
		   case IMEID_CHT_VER50: // 5.0.x.x // WinME
			   p = *(LPBYTE *)( (LPBYTE)g_Imm.LockIMCC( lpIC->hPrivate ) + 3 * 4 );
			   if( !p ) break;
			   p = *(LPBYTE *)( (LPBYTE)p + 1*4 + 5*4 + 4*2 );
			   if( !p ) break;
			   tempLen = *(DWORD *)(p + 1*4 + (16*2+2*4) + 5*4 + 16);
			   dwErr = *(DWORD *)(p + 1*4 + (16*2+2*4) + 5*4 + 16 + 1*4);
			   temp = (wchar_t *)(p + 1*4 + (16*2+2*4) + 5*4);
			   bUnicodeIme = false;
			   break;
			   
		   case IMEID_CHT_VER51: // 5.1.x.x // IME2002(w/OfficeXP)
		   case IMEID_CHT_VER52: // 5.2.x.x // (w/whistler)
		   case IMEID_CHS_VER53: // 5.3.x.x // SCIME2k or MSPY3 (w/OfficeXP and Whistler)
			   p = *(LPBYTE *)((LPBYTE)g_Imm.LockIMCC( lpIC->hPrivate ) + 4);
			   if( !p ) break;
			   p = *(LPBYTE *)((LPBYTE)p + 1*4 + 5*4);
			   if( !p ) break;
			   tempLen = *(DWORD *)(p + 1*4 + (16*2+2*4) + 5*4 + 16 * 2);
			   dwErr = *(DWORD *)(p + 1*4 + (16*2+2*4) + 5*4 + 16 * 2 + 1*4);
			   temp  = (wchar_t *) (p + 1*4 + (16*2+2*4) + 5*4);
			   bUnicodeIme = true;
			   break;
			   
			   // the code tested only with Win 98 SE (MSPY 1.5/ ver 4.1.0.21)
		   case IMEID_CHS_VER41:
			   {
				   int nOffset;
				   nOffset = ( m_dwId[1] >= 0x00000002 ) ? 8 : 7;
				   
				   p = *(LPBYTE *)((LPBYTE)g_Imm.LockIMCC( lpIC->hPrivate ) + nOffset * 4);
				   if( !p ) break;
				   tempLen = *(DWORD *)(p + 7*4 + 16*2*4);
				   dwErr = *(DWORD *)(p + 8*4 + 16*2*4);
				   dwErr = min( dwErr, tempLen );
				   temp = (wchar_t *)(p + 6*4 + 16*2*1);
				   bUnicodeIme = true;
			   }
			   break;
			   
		   case IMEID_CHS_VER42: // 4.2.x.x // SCIME98 or MSPY2 (w/Office2k, Win2k, WinME, etc)
			   {
				   OSVERSIONINFOA osi;
				   osi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);
				   GetVersionExA( &osi );
				   
				   int nTcharSize = ( osi.dwPlatformId == VER_PLATFORM_WIN32_NT ) ? sizeof(wchar_t) : sizeof(char);
				   p = *(LPBYTE *)((LPBYTE)g_Imm.LockIMCC( lpIC->hPrivate ) + 1*4 + 1*4 + 6*4);
				   if( !p ) break;
				   tempLen = *(DWORD *)(p + 1*4 + (16*2+2*4) + 5*4 + 16 * nTcharSize);
				   dwErr = *(DWORD *)(p + 1*4 + (16*2+2*4) + 5*4 + 16 * nTcharSize + 1*4);
				   temp  = (wchar_t *) (p + 1*4 + (16*2+2*4) + 5*4);
				   bUnicodeIme = ( osi.dwPlatformId == VER_PLATFORM_WIN32_NT ) ? true : false;
			   }
			   break;
			   
		   default:
			   temp = NULL;
			   tempLen = 0;
			   break;
		   }
	   }
	   
	   if(tempLen == 0) {
		   m_reading.resize(0);
	   } else {
		   if( bUnicodeIme ) {
			   m_reading.assign(temp, tempLen);
		   } else {
			   int wstrLen = MultiByteToWideChar(m_codePage, 0, (char*)temp, tempLen, NULL, 0); 
			   wchar_t* wstr = (wchar_t*)alloca(sizeof(wchar_t)*wstrLen);
			   MultiByteToWideChar(m_codePage, 0, (char*)temp, tempLen, wstr, wstrLen); 
			   m_reading.assign(wstr, wstrLen);
		   }
	   }
	   
		g_Imm.UnlockIMCC(lpIC->hPrivate);
		g_Imm.UnlockIMC(hImc);
		nRui->SetReadingStr((RTCHAR*)m_reading.c_str());
		m_bVerticalReading = !GetReadingWindowOrientation();
		nRui->SetReadingVisible(m_bVerticalReading);
    }
	
	ImmReleaseContext(hEdit, hImc);


}


bool CRui::GetReadingWindowOrientation()
{
	bool bHorizontalReading = ( m_hkl == _CHS_HKL ) || ( m_hkl == _CHT_HKL2 ) || ( m_dwId[0] == 0 );
    if( !bHorizontalReading && ( m_dwId[0] & 0x0000FFFF ) == LANG_CHT )
    {
        char szRegPath[MAX_PATH];
        HKEY hKey;
        DWORD dwVer = m_dwId[0] & 0xFFFF0000;
        strcpy( szRegPath, "software\\microsoft\\windows\\currentversion\\" );
        strcat( szRegPath, ( dwVer >= MAKEIMEVERSION( 5, 1 ) ) ? "MSTCIPH" : "TINTLGNT" );
        LONG lRc = RegOpenKeyExA( HKEY_CURRENT_USER, szRegPath, 0, KEY_READ, &hKey );
        if (lRc == ERROR_SUCCESS)
        {
            DWORD dwSize = sizeof(DWORD), dwMapping, dwType;
            lRc = RegQueryValueExA( hKey, "Keyboard Mapping", NULL, &dwType, (PBYTE)&dwMapping, &dwSize );
            if (lRc == ERROR_SUCCESS)
            {
                if ( ( dwVer <= MAKEIMEVERSION( 5, 0 ) && 
					( (BYTE)dwMapping == 0x22 || (BYTE)dwMapping == 0x23 ) )
					||
					( ( dwVer == MAKEIMEVERSION( 5, 1 ) || dwVer == MAKEIMEVERSION( 5, 2 ) ) &&
					(BYTE)dwMapping >= 0x22 && (BYTE)dwMapping <= 0x24 )
					)
                {
                    bHorizontalReading = true;
                }
            }
            RegCloseKey( hKey );
        }
    }
	
	return bHorizontalReading;

}


void CRui::SetupImeApi(HWND hWnd)
{
	char szImeFile[MAX_PATH + 1];
	
	_GetReadingString = NULL;
    _ShowReadingWindow = NULL;
	
	if( ImmGetIMEFileNameA( m_hkl, szImeFile, sizeof(szImeFile)/sizeof(szImeFile[0]) - 1 ) != 0 ) {
		
		if( m_hDllIme ) FreeLibrary( m_hDllIme );
		
		m_hDllIme = LoadLibraryA( szImeFile );
		
		if ( m_hDllIme ) {
			_GetReadingString = (UINT (WINAPI*)(HIMC, UINT, LPWSTR, PINT, BOOL*, PUINT))
				( GetProcAddress( m_hDllIme, "GetReadingString" ) );
			_ShowReadingWindow =(BOOL (WINAPI*)(HIMC, BOOL))
				( GetProcAddress( m_hDllIme, "ShowReadingWindow" ) );
			
			if( _ShowReadingWindow ) {
				HIMC hImc = ImmGetContext(hWnd);
				if(hImc) {
					_ShowReadingWindow( hImc, false );
					ImmReleaseContext(hWnd, hImc);
				}
			}
		}
	}
	


}

int CRui::GetCodePageFromCharset(int charset)
{
	switch( charset )
	{
	case ANSI_CHARSET:
		return 0;
	case SHIFTJIS_CHARSET:
		return 932;
	case HANGUL_CHARSET:
		return 949;
	case GB2312_CHARSET:
		return 936;
	case CHINESEBIG5_CHARSET:
		return 950;
	case GREEK_CHARSET:
		return 1253;
	case TURKISH_CHARSET:
		return 1254;
	case HEBREW_CHARSET:
		return 1255;
	case ARABIC_CHARSET:
		return 1256;
	case BALTIC_CHARSET:
		return 1257;
	case THAI_CHARSET:
		return 874;
	case EASTEUROPE_CHARSET:
		return 1250;
	default:
		return 1252;
	}
}

int CRui::GetCodePageFromLang( LANGID langid )
{
	return GetCodePageFromCharset(GetCharsetFromLang(langid));
}

int CRui::GetCharsetFromLang(LANGID langid)
{
	switch( PRIMARYLANGID(langid) )
	{
	case LANG_JAPANESE:
		return SHIFTJIS_CHARSET;
	case LANG_KOREAN:
		return HANGEUL_CHARSET;
	case LANG_CHINESE:
		switch( SUBLANGID(langid) )
		{
		case SUBLANG_CHINESE_SIMPLIFIED:
			return GB2312_CHARSET;// 간체

		case SUBLANG_CHINESE_TRADITIONAL:// 타이완
		case SUBLANG_CHINESE_HONGKONG:
		case SUBLANG_CHINESE_SINGAPORE:
		case SUBLANG_CHINESE_MACAU:
			return CHINESEBIG5_CHARSET;// 번체

		default:
			return ANSI_CHARSET;
		}
		case LANG_GREEK:
			return GREEK_CHARSET;
		case LANG_TURKISH:
			return TURKISH_CHARSET;
		case LANG_HEBREW:
			return HEBREW_CHARSET;
		case LANG_ARABIC:
			return ARABIC_CHARSET;
		case LANG_ESTONIAN:
		case LANG_LATVIAN:
		case LANG_LITHUANIAN:
			return BALTIC_CHARSET;
		case LANG_THAI:
			return THAI_CHARSET;
		case LANG_CZECH:
		case LANG_HUNGARIAN:
		case LANG_POLISH:
		case LANG_CROATIAN:
		case LANG_MACEDONIAN:
		case LANG_ROMANIAN:
		case LANG_SLOVAK:
		case LANG_SLOVENIAN:
			return EASTEUROPE_CHARSET;
		case LANG_RUSSIAN:
		case LANG_BELARUSIAN:
		case LANG_BULGARIAN:
		case LANG_UKRAINIAN:
			return RUSSIAN_CHARSET;
		default:
			return ANSI_CHARSET;
	}


}

void CRui::GetImeID()
{
	char    szTmp[1024];
	
	m_dwId[0] = m_dwId[1] = 0;
	
    if(!((m_hkl==_CHT_HKL) || (m_hkl==_CHT_HKL2) || (m_hkl==_CHS_HKL)))
		return;
	
	if ( !ImmGetIMEFileNameA( m_hkl, szTmp, ( sizeof(szTmp) / sizeof(szTmp[0]) ) - 1 ) )
        return;
	
    if ( !_GetReadingString ) {
		
        if( ( CompareStringA( LCID_INVARIANT, NORM_IGNORECASE, szTmp, -1, CHT_IMEFILENAME1, -1 ) != CSTR_EQUAL ) &&
            ( CompareStringA( LCID_INVARIANT, NORM_IGNORECASE, szTmp, -1, CHT_IMEFILENAME2, -1 ) != CSTR_EQUAL ) &&
            ( CompareStringA( LCID_INVARIANT, NORM_IGNORECASE, szTmp, -1, CHT_IMEFILENAME3, -1 ) != CSTR_EQUAL ) &&
            ( CompareStringA( LCID_INVARIANT, NORM_IGNORECASE, szTmp, -1, CHS_IMEFILENAME1, -1 ) != CSTR_EQUAL ) &&
            ( CompareStringA( LCID_INVARIANT, NORM_IGNORECASE, szTmp, -1, CHS_IMEFILENAME2, -1 ) != CSTR_EQUAL ) ) {
			
			return;
        }
    }
	
    DWORD   dwVerHandle;
    DWORD   dwVerSize = GetFileVersionInfoSize( szTmp, &dwVerHandle );
	
    if( dwVerSize ) {
		
        LPVOID  lpVerBuffer = alloca( dwVerSize );
		
        if( GetFileVersionInfo( szTmp, dwVerHandle, dwVerSize, lpVerBuffer ) ) {
			
			LPVOID  lpVerData;
			UINT    cbVerData;
			
            if( VerQueryValue( lpVerBuffer, "\\", &lpVerData, &cbVerData ) ) {
				
                DWORD dwVer = ( (VS_FIXEDFILEINFO*)lpVerData )->dwFileVersionMS;
                dwVer = ( dwVer & 0x00ff0000 ) << 8 | ( dwVer & 0x000000ff ) << 16;
                if( _GetReadingString
                    ||
                    ( m_langId == LANG_CHT &&
					( dwVer == MAKEIMEVERSION(4, 2) || 
					dwVer == MAKEIMEVERSION(4, 3) || 
					dwVer == MAKEIMEVERSION(4, 4) || 
					dwVer == MAKEIMEVERSION(5, 0) ||
					dwVer == MAKEIMEVERSION(5, 1) ||
					dwVer == MAKEIMEVERSION(5, 2) ||
					dwVer == MAKEIMEVERSION(6, 0) ) )
                    ||
                    ( m_langId == LANG_CHS &&
					( dwVer == MAKEIMEVERSION(4, 1) ||
					dwVer == MAKEIMEVERSION(4, 2) ||
					dwVer == MAKEIMEVERSION(5, 3) ) ) ) {
					
                    m_dwId[0] = dwVer | m_langId;
                    m_dwId[1] = ( (VS_FIXEDFILEINFO*)lpVerData )->dwFileVersionLS;
                }
            }
        }
    }

}


void CRui::LanguageChange(HWND hWnd , WPARAM wParam, LPARAM lParam)
{
 
	m_hkl = (HKL)lParam;
	m_langId = LOWORD(m_hkl);
	m_codePage = GetCodePageFromLang(m_langId);
	
	// 936 Chinese Simplified
	// 949 Korea IME
	// 932 Japan IME
#ifdef APP_IME_BLOCK_CHS
	//대만, 홍콩 유니코드 사용시 중국어 번자체와 영어 외에 다른 나라 IME 는 막아버림 
	if( m_codePage != 950 && m_codePage != 0 ) 
	{
		m_bCheckChinaCHS = true; 
	}
	else 
	{
		m_bCheckChinaCHS = false;
	}

#endif 

	
	m_reading.resize(0);
	/* Check Property */ 
//	DWORD property = ImmGetProperty(GetKeyboardLayout(0), IGP_PROPERTY);
	
//	m_bUnicodeIme = (property & IME_PROP_UNICODE) ? true : false;
	
	/* Update Indicator */ 
//	CheckToggleState(hWnd);
	
	/* Update m_dwId[] */ 
	GetImeID();
	
	/* Bind Proc */ 
	SetupImeApi(hWnd); 



}

bool CRui::GetCheckIME_Unicode()
{	

#ifdef UNICODE_ROCK
	return m_bCheckChinaCHS; 
#else 
	return 0; 
#endif 
	
}



///---------------------------------------------------------------------------
///-- DrawIMECovert()
///---------------------------------------------------------------------------
void CRui::DrawIMECovert()
{
	BOOL IsViewImeConvert = FALSE;	
	
	///IsViewImeConvert = TRUE;

	if( g_RockClient.GetLanguage() == eLang_China  || 
		g_RockClient.GetLanguage() == eLang_Taiwan || 
		 g_RockClient.GetLanguage() == eLang_Japan || 
		 g_RockClient.GetLanguage() == eLang_HongKong )
	{
		///-- 중국어 입력시 조합문자가 없어도 Candidate List는 그려준다.
#ifdef UNICODE_ROCK
		if( bVerticalReading )
		{

			if( Desktop.FocusEditWnd != WID_None )
			{
				CEditBox *pEditBox = (CEditBox *)(Container->Get( Desktop.FocusEditWnd ));
				Set_Cdi_Pos( pEditBox->Caret.X, pEditBox->Caret.Y );
			}		

			if( ReadingProcess())
			{
			  	Render_Reading();
				
				return;
			}

		}

#endif
		IsViewImeConvert = TRUE;
	}
	
	///-- 일본, 중국 candidate List와 조합되고 있는 문자를 보여준다.
	if( IsViewImeConvert )
	{
		///-- 현재 포커스중인 에디트 윈도우를 찾는다.
		if( Desktop.FocusEditWnd != WID_None )
		{
			CEditBox *pEditBox = (CEditBox *)(Container->Get( Desktop.FocusEditWnd ));
			Set_Cdi_Pos( pEditBox->Caret.X, pEditBox->Caret.Y );
			Complete_Cdi_List();
		}		
		
		if( mb_candidate )
		{
			Render_Candidate();
		}

#ifdef UNICODE_ROCK
		WCHAR* comp_wstr = RWCHAR(mac_comp_str);

		if( comp_wstr[0] == _RT('\0') )
		{
			return;
		}
#else
		if( mac_comp_str[0] == '\0' )
		{
			return;
		}
#endif
		
		HIMC hIMC = NULL;	
		hIMC = ImmGetContext( g_RockClient.GetApp() );	

		GetIMEAttrPos( hIMC );				// 변환중인 글자의 위치를 얻는다. ( m_convertedPos[] )

		/// 끝위치 m_convertedPos[1] 를 구한다. 
		
		int Fw_convertedPos = 0;
		int Cw_convertedPos = 0;
		
		Fw_convertedPos = m_convertedPos[0];
		Cw_convertedPos = m_convertedPos[1]+1;		
		
		///-- 변환중인 글자위치 0번 시작위치와 끝위치가 같을때..
		if( Fw_convertedPos == 0 && Cw_convertedPos == 1 )
		{
			///-- 변환중인 글자를 찍어준다.
			Render->DrawBGText( mc_font_candisel, mac_comp_str, ml_candi_x, ml_candi_y , D3DCOLOR_XRGB( 255, 150, 150 ), D3DCOLOR_XRGB( 225, 220, 220 ) );	
			
			///-- composition string 내에서 커서 위치를 가져온다.
#ifdef UNICODE_ROCK
			long cursor_pos = ImmGetCompositionStringW( hIMC, GCS_CURSORPOS, NULL, 0 );
#else
			long cursor_pos = ImmGetCompositionString( hIMC, GCS_CURSORPOS, NULL, 0 );
#endif			
			//			RLG1( "-=*** cursor_pos = %d", cursor_pos );
			
			int			mac_comp_str_width = 0;
			int			mac_comp_str_height = 0;
			
			///-- cursor_pos 0, 1, 2, 3, 4이면 GetDrawStrWidthg함수의 입력값은
			///--			 0 ,0, 1, 1, 2이런식으로
			///--  cursor_pos 0, 1 이면 mac_comp_str갯수만큼 그외에는 (cursor_pos / 2)만큼
			///-- 문자의 폭을 합산해 스트링의 너비를 구한다.
			mac_comp_str_width = GetDrawStrWidth( mc_font_candisel, mac_comp_str, cursor_pos / 2 );
			///-- 문자열의 높이를 구한다. 
			mac_comp_str_height = GetDrawStrHeight( mc_font_candisel, mac_comp_str );
			
			//			RLG3( "-=*** mac_comp_str = %s, cursor_pos = %d, mac_comp_str_width = %d", mac_comp_str, cursor_pos / 2, mac_comp_str_width );
			
			static long draw_time = timeGetTime();
			static bool OnTwinkle = false;
			
			if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( timeGetTime() , draw_time ) , > , 500 ) )
			{
				draw_time = timeGetTime();
				OnTwinkle = !OnTwinkle;
			}	
			
			if( OnTwinkle == true )
			{
				if( cursor_pos )	/// cursor_pos 가 0이상이면... 
				{
					/// 시작위치에 문자열 너비만큼 더해 캐럿을 그린다.
					Render->Draw( TID_CLIENT, ml_candi_x + mac_comp_str_width, ml_candi_y, 1, mac_comp_str_height - 1, D3DCOLOR_XRGB( 105, 0, 0 ) );
				}
				else
				{
					Render->Draw( TID_CLIENT, ml_candi_x, ml_candi_y, 1, mac_comp_str_height - 1, D3DCOLOR_XRGB( 105, 0, 0 ) );
				}
			}			
		}
		else
		{	
			char mac_comp_str_Fw[1024] = {0,};
			char mac_comp_str_Cw[1024] = {0,};
			char mac_comp_str_Ew[1024] = {0,};
			
			int mac_comp_str_Fw_width = 0;
			int mac_comp_str_Cw_width = 0;
			
			///-- 변환시작 위치가 0이 아닐경우
			if( Fw_convertedPos > 0 )
			{
				///-- 조합문자에서 이미 변환된 문자를 따로 뺀다.
				Rstrncpy( RWCHAR(mac_comp_str_Fw), RWCHAR(mac_comp_str), Fw_convertedPos );
			}
			
			///-- 변환끝 위치가 0이 아닐경우 
			if( Cw_convertedPos > 1 )
			{
#ifdef UNICODE_ROCK
				wcsncpy( RWCHAR(mac_comp_str_Cw), RWCHAR(mac_comp_str) + Fw_convertedPos, Cw_convertedPos - Fw_convertedPos );
				wcscpy(RWCHAR(mac_comp_str_Ew) , RWCHAR(mac_comp_str) + Cw_convertedPos );
#else
				strncpy( mac_comp_str_Cw, mac_comp_str + Fw_convertedPos, Cw_convertedPos - Fw_convertedPos );
				SAFE_STR_CPY( mac_comp_str_Ew, mac_comp_str + Cw_convertedPos, 1024 ); 
#endif
			}
			
			//			RLG2( "-=*** m_convertedPos[0] = %d, m_convertedPos[1] = %d", m_convertedPos[0], m_convertedPos[1] );
			//			RLG3( "-=*** mac_comp_str_Fw = %s, mac_comp_str_Cw = %s, mac_comp_str_Ew = %s", mac_comp_str_Fw, mac_comp_str_Cw, mac_comp_str_Ew );
			
			///-- 이미 변환된 문자의 너비를 구한다.
			mac_comp_str_Fw_width = GetDrawStrWidth( mc_font_candisel, mac_comp_str_Fw, 0 );
			///-- 변환중인 문자의 너비를 구한다.
			mac_comp_str_Cw_width = GetDrawStrWidth( mc_font_candisel, mac_comp_str_Cw, 0 );
			
			// 변환중인 문장의 구절별 블럭 표시 
			Render->DrawBGText( mc_font_candisel, mac_comp_str_Fw, ml_candi_x, ml_candi_y, D3DCOLOR_XRGB( 255, 150, 150 ), D3DCOLOR_XRGB( 155, 150, 150 ) );				
			Render->DrawBGText( mc_font_candisel, mac_comp_str_Ew, ml_candi_x + mac_comp_str_Fw_width + mac_comp_str_Cw_width, ml_candi_y, D3DCOLOR_XRGB( 255, 150, 150 ), D3DCOLOR_XRGB( 155, 150, 150 ) );			
			
			///-- 실제변환중인 문자를 강조표시 
			// 블럭표시 끝부분이 잘려서 나오므로 맨뒤에 그려준다. 
			Render->DrawBGText( mc_font_candisel, mac_comp_str_Cw, ml_candi_x + mac_comp_str_Fw_width, ml_candi_y, D3DCOLOR_XRGB( 255, 150, 150 ), D3DCOLOR_XRGB( 235, 230, 230 ) );	
		}	
	}
	
	return;
}

///---------------------------------------------------------------------------
///-- GetIMEAttrPos()
///-- 변환중인 문자의 구나 행을 블럭 표시하기 위해서 변환되고 있는 위치를 얻는다. 
///---------------------------------------------------------------------------
void CRui::GetIMEAttrPos( HIMC ahIMC )
{
    LONG		lByte;
#ifdef UNICODE_ROCK
	lByte = ImmGetCompositionStringW( ahIMC, GCS_COMPSTR, NULL, 0 );	
#else
    lByte = ImmGetCompositionString( ahIMC, GCS_COMPSTR, NULL, 0 );
#endif	
	if( lByte > 64 )
		lByte = 64;
	
    LPSTR pBuff = SAFE_NEW_ARRAY( CHAR , lByte + 1 );	
	
	///-- 조합 스트링의 attribute를 가져온다.
	///-- Attribute information is rendered in an array of 8-bit (1 Byte)
	///-- values that specifies the status of characters in the composition string
#ifdef UNICODE_ROCK
	ImmGetCompositionStringW( ahIMC, GCS_COMPATTR, pBuff, lByte );	
#else
	ImmGetCompositionString( ahIMC, GCS_COMPATTR, pBuff, lByte );	
#endif
	
	
    pBuff[lByte] = 0;
	
	m_convertedPos[0] = 0;
	m_convertedPos[1] = 0;
	
	int			test[255] = {0,};
	bool		isFirstOK = FALSE;
	
	for( int n = 0; n < lByte; n++ )
	{
		///-- ATTR_INPUT : Character being entered by the user. 
		///-- The IME has yet to convert this character.
		if( pBuff[n] == ATTR_INPUT )
			test[n] = ATTR_INPUT;
		///-- ATTR_TARGET_CONVERTED : Character selected by the user 
		///-- and then converted by the IME.
		else if( pBuff[n] == ATTR_TARGET_CONVERTED )
			test[n] = ATTR_TARGET_CONVERTED;
		///-- ATTR_CONVERTED : Character that the IME has already converted.
		else if( pBuff[n] == ATTR_CONVERTED )
			test[n] = ATTR_CONVERTED;
		///-- ATTR_TARGET_NOTCONVERTED : Character being converted. 
		///-- The user has selected this character but the IME has not yet converted it. 
		else if( pBuff[n] == ATTR_TARGET_NOTCONVERTED )
			test[n] = ATTR_TARGET_NOTCONVERTED;
		///-- ATTR_INPUT_ERROR : An error character that the IME cannot convert. 
		///-- For example, the IME cannot put together some consonants.
		else if( pBuff[n] == ATTR_INPUT_ERROR )
			test[n] = ATTR_INPUT_ERROR;
		///-- ATTR_FIXEDCONVERTED : Characters that the IME will no longer convert.
		else if( pBuff[n] == ATTR_FIXEDCONVERTED )
			test[n] = ATTR_FIXEDCONVERTED;
		else 
			test[n] = 0;
		
		if( test[n] == ATTR_TARGET_CONVERTED )			
		{
			if( !isFirstOK )
			{
				m_convertedPos[0] = n;
				isFirstOK = TRUE;
			}
			else if( n >= m_convertedPos[0] )
				m_convertedPos[1] = n;
		}
		else if( test[n] == ATTR_TARGET_NOTCONVERTED )			
		{
			if( !isFirstOK )
			{
				m_convertedPos[0] = n;
				isFirstOK = TRUE;
			}
			else if( n >= m_convertedPos[0] )
				m_convertedPos[1] = n;
		}
	}	
	
	SAFE_DELETE_ARRAY(pBuff);
	
    return;
}

int CRui::GetStrWidthMax(CFontg* pFontg , void *pStr , int &RealSize,int MaxSize)
{
	//MaxSize 제한한다 . 
	 
	SFont*		tmpFont = NULL;
	int			NumStr = 0;
	int			i = 0;
	int			PosX = 0 ; 

	static FONTID IdxBuf[ MAX_INDEX_BUFFER ];
	memset( IdxBuf, 0, sizeof(FONTID) * MAX_INDEX_BUFFER );
	
	NumStr = pFontg->Confirm( pStr, IdxBuf );
	int StrCount =0 ; 
	
	if(NumStr == 0 )
		return 0;

	for( i = 0; i < NumStr; ++i  )
	{
		tmpFont = pFontg->GetFontasm( IdxBuf[i] );
		
		if( tmpFont != NULL )
		{
			if( PosX < MaxSize  )
			{
				PosX += tmpFont->w;	
				
				//개행 이다 . 
				if( IdxBuf[i] == 35 && IdxBuf[i+1] == 35  )
				{
					RealSize+= 2 ;
					break; 
				}
				else 	if(IdxBuf[ i ] == 92 )
				{
					RealSize++;
					break; 
				}

#ifdef UNICODE_ROCK
				// 무조건 2byte 한문자이다 . 
				StrCount++;
#else 
				
				if( IdxBuf[ i ] < 255 ) 
				{
					StrCount++;
				}
				else 
				{
					StrCount +=2;
				}
#endif 
			}
			else 
			{
				break; 
			}
		}
	}
	
	RealSize +=StrCount;

	return StrCount;
}



int CRui::GetDrawStrWidth( CFontg* pFontg, TCHAR* pStr, long aNumStr )
{	
	int			result_width = 0;
	
	SFont*		tmpFont = NULL;
	int			NumStr = 0;
	int			i = 0;
	
	static FONTID IdxBuf[ 256 ];
	memset( IdxBuf, 0, sizeof(FONTID) * 256 );
	
	NumStr = pFontg->Confirm( pStr, IdxBuf );
	
	if( aNumStr )
	{
		for( i = 0; i < aNumStr; ++i  )
		{
			tmpFont = pFontg->GetFontasm( IdxBuf[ i ] );
			
			if( tmpFont != NULL )
			{
				result_width += tmpFont->w;			
			}
		}
	}
	else
	{		
		for( i = 0; i < NumStr; ++i  )
		{
			tmpFont = pFontg->GetFontasm( IdxBuf[ i ] );
			
			if( tmpFont != NULL )
			{
				result_width += tmpFont->w;			
			}
		}
	}
	
	return result_width;
}

int CRui::GetDrawStrHeight( CFontg* pFontg, TCHAR* pStr )
{	
	int			result_height = 0;
	
	SFont*		tmpFont = NULL;
	int			NumStr = 0;
	int			i = 0;
	
	static FONTID IdxBuf[ 256 ];
	memset( IdxBuf, 0, sizeof(FONTID) * 256 );
	
	NumStr = pFontg->Confirm( pStr, IdxBuf );
	
	for( i = 0; i < NumStr; ++i  )
	{
		tmpFont = pFontg->GetFontasm( IdxBuf[ i ] );
		
		if( tmpFont != NULL )
		{
			if( result_height < tmpFont->h )
			{
				result_height = tmpFont->w;			
			}
		}
	}
	
	return result_height + 1;
}

///---------------------------------------------------------------------------
///-- Set_Cid_Str
///---------------------------------------------------------------------------
///-- Candidate 문자열
void CRui::Set_Cdi_Str( const int al_index, char* apc_str )
{
	int nIndex = al_index + 1;
	if( nIndex == 10 )
	{
		nIndex = 0;
	}	
	
	Rsprintf( RWCHAR(mac_candi_str[al_index]), _RT("%d : %s"), nIndex , RWCHAR(apc_str) );
}
///---------------------------------------------------------------------------
///-- Set_Cid_cmpStr
///---------------------------------------------------------------------------
///-- 조합중인 문자열
void CRui::Set_Cdi_cmpStr( char* apc_str )
{
	if( apc_str )
	{
		Rsprintf( RWCHAR(mac_comp_str), _RT("%s"), RWCHAR(apc_str) );	
	}
	else
	{
		Rsprintf( RWCHAR(mac_comp_str), _RT("") );	
	}	
}
///---------------------------------------------------------------------------
///-- Set_Cdi_Values
///---------------------------------------------------------------------------
///-- Candidate List 관련 Value Set
void CRui::Set_Cdi_Values( const int al_count, const int al_st_idx, const int al_sel_idx )
{
	ml_candi_start_idx = al_st_idx;		
	ml_candi_sel_idx = al_sel_idx;	
	ml_candi_count = al_count;	
}
///---------------------------------------------------------------------------
///-- Complete_Cdi_List
///---------------------------------------------------------------------------
///-- Candidate List 갱신이 완료 되었을 경우 호출
///-- Candidate List 의 위치와 너비를 지정한다.
void CRui::Complete_Cdi_List()
{
	int i;
	SStringInfo ls_strinfo;
	
	ml_width_max = 0;
	
	///-- Candidate list Back
	for( i = 0 ; i < g_nCandidate_Page_Size; ++i )
	{
		mc_font_candisel->GetStringInfo( mac_candi_str[i], &ls_strinfo );
		
		if( ml_width_max < ls_strinfo.Width )
		{
			ml_width_max = ls_strinfo.Width;
		}
	}
	
	///-- Candidate List Position
	///-- Candidata List가 찍히는 윈도우의 Y좌표 - ml_list_y
	///-- Candidate List가 찍히는 페이지의 Y좌표 - ml_page_y
	if( ml_candi_y  > 600 )
	{
		ml_list_y  = ml_candi_y - 152;		
		ml_page_y = ml_list_y - 3 ;
	}
	else
	{
		ml_list_y = ml_candi_y + 10;
		ml_page_y = ml_candi_y + 15 * g_nCandidate_Page_Size + 22;
	}
}
///---------------------------------------------------------------------------
///-- Render_Candidate
///---------------------------------------------------------------------------
///-- 지정된 값들로 Candidate List 를 Draw
void CRui::Render_Candidate()
{	
	int i;	
	char temp[30];
	int ml_add = 0;
	
	///-- Candidate Back  - Candidate List 배경 그리기 
	Render->Draw( TID_CLIENT, ml_candi_x - 4, ml_list_y + 5, ml_width_max + 12, g_nCandidate_Page_Size * 15 + 8, D3DCOLOR_ARGB( 100, 255, 255, 0 ) );	
	Render->Draw( TID_CLIENT, ml_candi_x - 3, ml_list_y + 6, ml_width_max + 10, g_nCandidate_Page_Size * 15 + 6, D3DCOLOR_ARGB( 200, 70, 105, 220 ) );	
	
	///-- Candidate List String - Candidate List 문자열 그리기
	for( i = 0 ; i < g_nCandidate_Page_Size; ++i )
	{
		///-- Selected String - 현재 문자열이 - 선택된 문자열이라면
		if( i + ml_candi_start_idx == ml_candi_sel_idx )
		{
			Render->DrawBGText( mc_font_candisel, mac_candi_str[i], ml_candi_x +2, ml_list_y + 15 * i + 10 ,
				D3DCOLOR_XRGB( 255, 255, 0 ), D3DCOLOR_XRGB( 225, 225, 220 ) );	
		}
		///-- Normal String - 그외 문자열
		else
		{
			Render->DrawText( mc_font_candi, mac_candi_str[i], ml_candi_x +2, ml_list_y + 15 * i + 10,
				D3DCOLOR_XRGB( 255, 255, 255 ) );	
		}		
	}

	///-- Page 오차 보정 - 총갯수가 페이지에 위해 나눠지지 않으면 총페이지 갯수에 1을 추가한다. 
	ml_candi_count % g_nCandidate_Page_Size != 0 ? ml_add  = 1 : ml_add  = 0;	

	///-- 중국어 Candidate List는 페이지다운, 페이지업 키를 쓰기 때문에 한번에 10개씩만 불러온다.
	///-- 따라서 페이지를 찍어줄 필요가 없다.	
	if( g_RockClient.GetLanguage() != eLang_China && 
		g_RockClient.GetLanguage() != eLang_Taiwan && 
		g_RockClient.GetLanguage() != eLang_HongKong)
	{
		///-- page-- 현재페이지/총페이지 쓰기
		Rsprintf( RWCHAR(temp), _RT("%d / %d"), ( ml_candi_sel_idx / g_nCandidate_Page_Size ) + 1, ( ml_candi_count / g_nCandidate_Page_Size ) + ml_add );
		Render->DrawImageNUM( temp, ml_candi_x - 4, ml_page_y, D3DCOLOR_XRGB( 255, 255, 255 ) );
	}
}

void CRui::SetState()
{
	Render->SetState();	
}

void CRui::ReleaseState()
{
	Render->ReleaseState();
}

void CRui::CtrlUserKeyProc(bool& IsWnd)
{
	
}

void CRui::UserKeyProc()
{
	bool IsWnd =false ;

	static int map_type =0;	
	
	BOOL IsCrtl = FALSE;
	BOOL IsShift = FALSE; 

#ifdef QUEST_RENEWAL
	if( theEMsg.Keys[DIK_LSHIFT].Action == n_baPress || theEMsg.Keys[DIK_LSHIFT].Action == n_baPress  )
	{
		IsShift = TRUE;
	}	
#endif 


	if(g_PartyInforWnd.IsMemberParty())
	{
		if( theEMsg.Keys[DIK_LCONTROL].Action == n_baPress || theEMsg.Keys[DIK_RCONTROL].Action == n_baPress  )
		{
			IsCrtl = TRUE;
		}	
	}

	if( IsShift && theEMsg.Keys[DIK_1].Action == n_baDown )
	{
		MainMenuWnd.UseFunctionKey( n_FKSF1 );
	}
	else if( theEMsg.Keys[DIK_1].Action == n_baDown )
	{
		MainMenuWnd.UseFunctionKey( n_FK1 );
	}
	else if( IsShift && theEMsg.Keys[DIK_2].Action == n_baDown )
	{
		MainMenuWnd.UseFunctionKey( n_FKSF2 );
	}
	else if( theEMsg.Keys[DIK_2].Action == n_baDown )
	{
		MainMenuWnd.UseFunctionKey( n_FK2 );
	}
	else if( IsShift && theEMsg.Keys[DIK_3].Action == n_baDown )
	{
		MainMenuWnd.UseFunctionKey( n_FKSF3 );
	}
	else if( theEMsg.Keys[DIK_3].Action == n_baDown )
	{
		MainMenuWnd.UseFunctionKey( n_FK3 );
	}
	else if( IsShift && theEMsg.Keys[DIK_4].Action == n_baDown )
	{
		MainMenuWnd.UseFunctionKey( n_FKSF4 );
	}
	else if( theEMsg.Keys[DIK_4].Action == n_baDown )
	{
		MainMenuWnd.UseFunctionKey( n_FK4 );
	}
	else if( IsShift && theEMsg.Keys[DIK_5].Action == n_baDown )
	{
		MainMenuWnd.UseFunctionKey( n_FKSF5 );
	}
	else if( theEMsg.Keys[DIK_5].Action == n_baDown )
	{
		MainMenuWnd.UseFunctionKey( n_FK5 );
	}
	else if( IsShift && theEMsg.Keys[DIK_6].Action == n_baDown )
	{
		MainMenuWnd.UseFunctionKey( n_FKSF6 );
	}
	else if( theEMsg.Keys[DIK_6].Action == n_baDown )
	{
		MainMenuWnd.UseFunctionKey( n_FK6 );
	}
	else if( IsShift && theEMsg.Keys[DIK_7].Action == n_baDown )
	{
		MainMenuWnd.UseFunctionKey( n_FKSF7 );
	}
	else if( theEMsg.Keys[DIK_7].Action == n_baDown )
	{
		MainMenuWnd.UseFunctionKey( n_FK7 );
	}
	else if( IsShift && theEMsg.Keys[DIK_8].Action == n_baDown )
	{
		MainMenuWnd.UseFunctionKey( n_FKSF8 );
	}
	else if( theEMsg.Keys[DIK_8].Action == n_baDown )
	{
		MainMenuWnd.UseFunctionKey( n_FK8 );
	}
	else if( IsShift && theEMsg.Keys[DIK_9].Action == n_baDown )
	{
		MainMenuWnd.UseFunctionKey( n_FKSF9 );
	}
	else if( theEMsg.Keys[DIK_9].Action == n_baDown )
	{
		MainMenuWnd.UseFunctionKey( n_FK9 );
	}
	else if( IsShift && theEMsg.Keys[DIK_0].Action == n_baDown )
	{
		MainMenuWnd.UseFunctionKey( n_FKSF0 );
	}
	else if( theEMsg.Keys[DIK_0].Action == n_baDown )
	{
		MainMenuWnd.UseFunctionKey( n_FK0 );
	}
	//////////////////////////////////////////////////////////////////////////
	else if( IsCrtl && theEMsg.Keys[DIK_F1].Action == n_baDown )
	{
		g_PartyInforWnd.GiveGagePartyMember(0);
	}
	else if( theEMsg.Keys[DIK_F1].Action == n_baDown )
	{
		MainMenuWnd.UseFunctionKey( n_FKF1 );
	}
	//////////////////////////////////////////////////////////////////////////
	else if( IsCrtl && theEMsg.Keys[DIK_F2].Action == n_baDown )
	{
		g_PartyInforWnd.GiveGagePartyMember(1);
	}
	else if( theEMsg.Keys[DIK_F2].Action == n_baDown )
	{
		MainMenuWnd.UseFunctionKey( n_FKF2 );
	}
	//////////////////////////////////////////////////////////////////////////
	else if( IsCrtl && theEMsg.Keys[DIK_F3].Action == n_baDown )
	{
		g_PartyInforWnd.GiveGagePartyMember(2);
	}
	else if( theEMsg.Keys[DIK_F3].Action == n_baDown )
	{
		MainMenuWnd.UseFunctionKey( n_FKF3 );
	}
	//////////////////////////////////////////////////////////////////////////
	else if( IsCrtl && theEMsg.Keys[DIK_F4].Action == n_baDown )
	{
		g_PartyInforWnd.GiveGagePartyMember(3);
	}
	else if( theEMsg.Keys[DIK_F4].Action == n_baDown )
	{
		MainMenuWnd.UseFunctionKey( n_FKF4 );
	}
	//////////////////////////////////////////////////////////////////////////
	else if( IsCrtl && theEMsg.Keys[DIK_F5].Action == n_baDown )
	{
		g_PartyInforWnd.GiveGagePartyMember(4);
	}
	else if( theEMsg.Keys[DIK_F5].Action == n_baDown )
	{
		MainMenuWnd.UseFunctionKey( n_FKF5 );
	}
	//////////////////////////////////////////////////////////////////////////
	else if( theEMsg.Keys[DIK_F6].Action == n_baDown )
	{
		MainMenuWnd.UseFunctionKey( n_FKF6 );
	}
	//////////////////////////////////////////////////////////////////////////
	else if( theEMsg.Keys[DIK_F7].Action == n_baDown )
	{
		MainMenuWnd.UseFunctionKey( n_FKF7 );
	}
	//////////////////////////////////////////////////////////////////////////
	else if( theEMsg.Keys[DIK_F8].Action == n_baDown )
	{
		MainMenuWnd.UseFunctionKey( n_FKF8 );
	}
	//////////////////////////////////////////////////////////////////////////
	else if( theEMsg.Keys[DIK_F9].Action == n_baDown )
	{
		MainMenuWnd.UseFunctionKey( n_FKF9 );
	}
	else if( theEMsg.Keys[DIK_F10].Action == n_baDown )
	{
		MainMenuWnd.UseFunctionKey( n_FKF0 );
	}
	
	//-------------------------------------------------------------------------
	if( theEMsg.Keys[DIK_V].Action == n_baDown )
	{
		IsWnd = !CharStateWnd.IsVisible;
		SendEvent( WID_StatWnd, n_emSetVisible, (EPARAM)IsWnd, 0, 0, 0 );
		
	}
	//-------------------------------------------------------------------------
#ifdef PET_SYSTEM
	if( theEMsg.Keys[DIK_P].Action == n_baDown )
	{

#ifdef SINGAPORE_BLOCK_PET
	SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_COMING_SOON_PET ) );
#else
		IsWnd = !g_PetMainWndProc.IsVisible;
		SendEvent( WID_PETMAIN_WND, n_emSetVisible, (EPARAM)IsWnd, 0, 0, 0 );
#endif

	}
#endif
	
	if( theEMsg.Keys[DIK_L].Action == n_baDown )
	{
		IsWnd = !g_LogWnd.IsVisible;
		g_LogWnd.IsVisible = IsWnd;
		nRui->SendEvent( g_LogWnd.m_pFrame->GetID(), n_emSetVisible, (EPARAM)IsWnd, 0, 0, 0 );
	}	

#ifdef CPK_SYSTEM	
	if( ( theEMsg.Keys[DIK_LCONTROL].Action == n_baPress ) && (theEMsg.Keys[DIK_K].Action == n_baDown) )
	{
		///-- PK 모드 관련 동작 수행
		g_Pc.GetPlayer()->ExcutePKModeAction();
	}
#endif
	
	if( ( theEMsg.Keys[DIK_LCONTROL].Action == n_baNone ) && ( theEMsg.Keys[DIK_S].Action == n_baDown ) )
	{
		if(g_Pc.GetPlayer()->IsMyPlayerRender)
		{
			IsWnd = !(g_SkillWnd.IsVisible);
			SendEvent( WID_SkillWnd, n_emSetVisible, (EPARAM)IsWnd, 0, 0, 0 );
			
			if( IsWnd == false )
			{
				//				SendEvent( WID_SkillRegistWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
				//				SendEvent( WID_SkillOverWriteWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
				SendEvent( WID_SkillLevelUpWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
				//				SendEvent( WID_SkillInfoWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
			}
		}
	}	
	
	if( theEMsg.Keys[DIK_I].Action == n_baDown )
	{
		if(g_Pc.GetPlayer()->IsMyPlayerRender)
		{
			IsWnd = !InventoryWnd.IsVisible;
			if( NpcChatWnd.IsVisible && IsWnd )
			{				
				InventoryWnd.FrameWnd->Reposition( 500, 394 );
			}
#ifdef WIDE_PAY_INVENTORY //유료인벤확장			
			InventoryWnd.Open();
#else
			nRui->SendEvent( WID_InventoryWnd, n_emSetVisible, (EPARAM)IsWnd, 0, 0, 0 );
#endif
		}
	}
	if( theEMsg.Keys[DIK_Q].Action == n_baDown )
	{
		IsWnd = !QuestWnd.IsVisible;			
		if( IsWnd )				
		{
			QuestWnd.OpenQuestWnd();
		}
		else
		{
			SendEvent( WID_Quest_Wnd, n_emSetVisible, (EPARAM)IsWnd, 0, 0, 0 );
		}
	}
	if( ( theEMsg.Keys[DIK_LCONTROL].Action == n_baPress ) && ( theEMsg.Keys[DIK_S].Action == n_baDown ) )
	{
		IsWnd = !SystemMenuWnd.IsVisible;
		if(IsWnd)
		{
			SystemMenuWnd.Open(g_Pc.GetPlayer()->IsMyPlayerRender);
		}
		else
		{
			SystemMenuWnd.Close();
		}
	}

#if defined( C_PAY_SYSTEM ) && !defined( GLOBAL_VERSION )		

	//인도네시아만 유료싸이트 Y키로 ..변경 
	if( g_RockClient.GetLanguage() == eLang_Indonesia ||
		g_RockClient.GetLanguage() == eLang_Singapore)
	{
		if( ( theEMsg.Keys[DIK_LCONTROL].Action == n_baPress )  && (theEMsg.Keys[DIK_Y].Action == n_baDown) )
		{
			IsWnd = !g_WebShopWnd.IsVisible;
			if( IsWnd )
			{
				if( g_RockClient.GetLanguage() == eLang_Indonesia)
				{
					g_WebShopWnd.OpenURL( L"http://202.78.197.87:8080/" , "" );
				}
				else if(g_RockClient.GetLanguage() == eLang_Singapore)
				{
					g_WebShopWnd.OpenURL( L"http://203.116.79.36:8080/index.asp" , "" );
				}
			}
			else
			{
				g_WebShopWnd.Close();
			}			
		}
	}
	else 
	{
		
		if( ( theEMsg.Keys[DIK_LCONTROL].Action == n_baPress ) && ( theEMsg.Keys[DIK_C].Action == n_baDown ) )
		{
			IsWnd = !g_WebShopWnd.IsVisible;
			if( IsWnd )
			{
				if(g_RockClient.GetLanguage() == eLang_Korea)
				{
					g_WebShopWnd.OpenURL( L"http://itemmall.sigongon.co.kr/index.asp" , "" );
				}
				else if(g_RockClient.GetLanguage() == eLang_Japan)
				{
					g_WebShopWnd.OpenURL( L"http://decoitem.gameleon.jp/" , "" );
				}
				else if(g_RockClient.GetLanguage() == eLang_China )
				{
					g_WebShopWnd.OpenURL( L"http://shop.guanghei.com" , "" );
				}
				else if(g_RockClient.GetLanguage() == eLang_Taiwan)
				{
#ifdef _DEBUG
					g_WebShopWnd.OpenURL( L"http://decomall.unalis.com.tw:8080/index.asp" , "" );
					//g_WebShopWnd.OpenURL( L"http://decomalltest.unalis.com.tw:8080/index.asp" , "" );
#else 
					g_WebShopWnd.OpenURL( L"http://decomall.unalis.com.tw:8080/index.asp" , "" );
#endif 

				}				
				else if(g_RockClient.GetLanguage() == eLang_HongKong)
				{
					g_WebShopWnd.OpenURL( L"http://runupsoweb.runup.com.my/itemmall" , "" );
				}				
			}
			else
			{
				g_WebShopWnd.Close();
			}			
		}		
	}
	
#endif 
	
	if( ( theEMsg.Keys[DIK_LCONTROL].Action == n_baPress ) && ( theEMsg.Keys[DIK_F].Action == n_baDown ) )
	{
		if(g_Pc.GetPlayer()->IsMyPlayerRender)
		{
			IsWnd = !g_FrontierMainWnd.IsVisible;
			if( IsWnd )
			{
				g_FrontierMainWnd.Open();
			}
			else
			{
				g_FrontierMainWnd.Close();
			}	
		}
	}
	else if( theEMsg.Keys[DIK_F].Action == n_baDown )// F키 친구창 열기
	{
		if(g_Pc.GetPlayer()->theMapType != n_FightZone)
		{
			IsWnd = !g_MCommunityWnd.IsVisible;
			g_MCommunityWnd.SetVisible(IsWnd);
		}			
	}
#ifdef APPLY_GUIDE
	else if( ( theEMsg.Keys[DIK_LCONTROL].Action == n_baPress ) && ( theEMsg.Keys[DIK_H].Action == n_baDown ) )
	{
		IsWnd = !g_pGuideWnd->IsVisible;
		if(IsWnd)
		{
			g_pGuideWnd->Open();
		}
		else
		{
			g_pGuideWnd->Close();
		}		
	}
#endif
	///-- Minimap Key 변경		
		
	if( theEMsg.Keys[DIK_M].Action == n_baDown )
	{
		MiniMapWnd.IsMapChange();
	} 
	
	
	if(  g_RockClient.GetGameMode() == GMODE_NORMAL	&&
		 COutState_Act == g_ChatMainProc.GetOutState() &&
		 !g_ChatMainProc.m_ChatActPrintWnd.m_IsOpenInstant )
	{
		 if( ( theEMsg.Keys[DIK_RETURN].Action == n_baNone ) && 
		     ( theEMsg.Keys[DIK_NUMPADENTER].Action == n_baNone ) )
		 {
			g_ChatMainProc.SetOutState(COutState_NonAct);
			//PRINT_DLG_LOG("Appear Chat Focus Loss");
			return;
		 }		 		
	}	
	
	//by simwoosung 이렇게 해야 채팅창 조작감이 올라감 ㅠ.ㅠ;;;	- 현재 자리고수	
	//중국에서는 theEmsg.keys 값이 안들어 오는 관계로 .. CChatActPrintWnd 로 창을 닫는다 .. 
	// 콜백이 다이렉트 인풋보다 상위 계층으로서 먼저 키값을 검사하고. 
	if( ( theEMsg.Keys[DIK_RETURN].Action == n_baUp ) || 
		( theEMsg.Keys[DIK_NUMPADENTER].Action == n_baUp ) )
	{	
		
		if( theEnterFlag == n_nfEditEnter )
		{
			theEnterFlag = n_nfUIEnter;
			m_lEdPrevtime = g_nowTime;
		}		
		else if(  g_RockClient.GetGameMode() == GMODE_NORMAL  )		
		{
			m_lEdPrevtime = g_nowTime;				
			
			//에디트 윈도우의 활성화		
			
			if( COutState_Act == g_ChatMainProc.GetOutState() )
			{
				g_ChatMainProc.SetOutState(COutState_NonAct);
			}
			else
			{

				g_ChatMainProc.SetOutState(COutState_Act);			
				
				HIMC  hIMC;
				DWORD  dwConversion;
				DWORD  dwSentence;
				
				if( hIMC = ImmGetContext( GetActiveWindow() )  )
				{
					//IME 한글로 전환
					ImmGetConversionStatus( hIMC, &dwConversion, &dwSentence);						
							
					if( g_RockClient.GetLanguage() == eLang_Japan )
					{
						ImmSetConversionStatus( hIMC, dwConversion | IME_CMODE_NATIVE 
							| IME_CMODE_FULLSHAPE & ~IME_CMODE_KATAKANA | IME_CMODE_ROMAN, IME_SMODE_AUTOMATIC );
					}
					else if( g_RockClient.GetLanguage() == eLang_China ||
							g_RockClient.GetLanguage() == eLang_Taiwan || g_RockClient.GetLanguage() == eLang_HongKong 	)
					{
						ImmSetConversionStatus( hIMC, dwConversion | IME_CMODE_NATIVE 
							/*| IME_CMODE_FULLSHAPE*/ | IME_CMODE_ROMAN, IME_SMODE_AUTOMATIC );			
					}
					else
					{
#ifdef ONLY_ENGLISH_SET
						ImmSetConversionStatus( hIMC, IME_CMODE_ALPHANUMERIC , IME_SMODE_NONE );	
#else
						ImmSetConversionStatus( hIMC, dwConversion | IME_CMODE_NATIVE, IME_SMODE_NONE );
#endif
	
					}

					ImmSetOpenStatus( hIMC, true );
					ImmReleaseContext( GetActiveWindow(), hIMC );		
				}
			}
		}
	}

	//현재 자신의 캐릭터가 파이트존에 있을경우에
	if( g_Pc.GetPlayer()->theMapType == n_FightZone )		 
	{
		//자신의 캐릭터가 죽어있을경우
		if(g_Pc.GetPlayer()->IsMyPlayerRender)
		{
			if( ( g_Pc.GetPlayer()->m_curt_event.type == SM_STATE_DIE ) && 
				( g_AliveWnd.m_AliveType == n_AnswerType_Observer ) )
			{
				//마우스 왼쪽키가 눌러진 경우
				if( theEMsg.Mouse.LButton == n_baDown )			
				{
					PCTABLE	*pPcTable = NULL;					
					pPcTable = g_Pc.GetNextPcTable(g_Pc.GetPlayer()->m_nViewRefDestIndex);
					
					if( pPcTable )
					{
						g_Camera.m_unique = pPcTable->lUnique;
					}
					else
					{
						g_Camera.m_unique = -1;					
					}
					
					g_Camera.Update();
					g_Camera.MovCamera();
					g_Map.UpdateTerrain( g_Pc.GetDestX(), g_Pc.GetDestY(), true );
				}
			}
		}
		else
		{
			//마우스 왼쪽키가 눌러진 경우
			if( theEMsg.Mouse.LButton == n_baDown )			
			{
				PCTABLE	*pPcTable = NULL;					
				pPcTable = g_Pc.GetNextPcTable(g_Pc.GetPlayer()->m_nViewRefDestIndex);
				
				if( pPcTable )
				{
					g_Camera.m_unique = pPcTable->lUnique;
				}
				else
				{
					pPcTable = g_Pc.GetNextPcTable(g_Pc.GetPlayer()->m_nViewRefDestIndex);
					g_Camera.m_unique = pPcTable->lUnique;
				}
				
				g_Camera.Update();
				g_Camera.MovCamera();
				g_Map.UpdateTerrain( g_Pc.GetDestX(), g_Pc.GetDestY(), true );
			}
		}
	}		
		
}

void CRui::TestKeyProc(bool& IsWnd)
{
	
	//-------------------------------------------------------------------------
	if( theEMsg.Keys[DIK_LCONTROL].Action == n_baPress && theEMsg.Keys[DIK_1].Action == n_baUp )
	{
		IsInfo = !IsInfo;
	}
	
	///-- Temp Key Interface	
	if( ( theEMsg.Keys[DIK_LCONTROL].Action == n_baPress ) && theEMsg.Keys[DIK_O].Action == n_baDown )
	{		
		///nRui->SendEvent( WID_Party_YesNo_Wnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
		///nRui->SendEvent( WID_UIMsgWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
		nRui->SendEvent( WID_SkillWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );	
		/*nRui->SendEvent( WID_SkillLevelUpWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
		nRui->SendEvent( WID_StatWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
		///nRui->SendEvent( WID_Message_ImageWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
		nRui->SendEvent( WID_OptWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
		nRui->SendEvent( WID_InventoryWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
		nRui->SendEvent( WID_MoneyThrow_Wnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
		nRui->SendEvent( WID_SUB_EXTRACT, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
		nRui->SendEvent( WID_SUB_EXTRACT_RESULT, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
		nRui->SendEvent( WID_SUB_REMODEL_FA, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
		nRui->SendEvent( WID_SUB_REMODEL_SUC, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
		nRui->SendEvent( WID_SUB_REMODEL, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
		nRui->SendEvent( WID_SUB_ENCHANT, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
		nRui->SendEvent( WID_SUB_ENCHANT_RESULT, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
		nRui->SendEvent( WID_ItemBuyWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
		nRui->SendEvent( WID_ItemSellWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
		nRui->SendEvent( WID_NumberInput_Wnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );		
		nRui->SendEvent( WID_REPAIR_WND, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
		nRui->SendEvent( WID_StoreWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
		nRui->SendEvent( WID_Exchange_Wnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
		nRui->SendEvent( WID_Warehouse_Wnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
		nRui->SendEvent( WID_ITEMCREATEF_WND, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
		nRui->SendEvent( WID_ITEMCREATE_WND, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
		nRui->SendEvent( WID_AliveWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
		nRui->SendEvent( WID_PartyInforWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
		*/
		
		///nRui->SendEvent( WID_REPAIR_WND, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
		///nRui->SendEvent( WID_Party_PopupWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
		//nRui->SendEvent( WID_StatWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
		//nRui->SendEvent( WID_SkillWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
		//nRui->SendEvent( WID_REPAIR_WND, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
		
		
	}
	if( ( theEMsg.Keys[DIK_LCONTROL].Action == n_baPress ) && theEMsg.Keys[DIK_K].Action == n_baDown)
	{
		
		//		nRui->SendEvent( WID_PETINFO_WND, n_emSetVisible, (EPARAM)true, 0, 0, 0 );	
		//		nRui->SendEvent( WID_PETMAIN_WND, n_emSetVisible, (EPARAM)true, 0, 0, 0 );	
		
		
		
	}	
	
	if( ( theEMsg.Keys[DIK_LCONTROL].Action == n_baPress ) && theEMsg.Keys[DIK_P].Action == n_baDown )
	{
		//nRui->SendEvent( WID_MainMenuLv_Wnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
		//nRui->SendEvent( WID_PlayerInfo_Wnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
		//nRui->SendEvent( WID_MainExp_Wnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
		//nRui->SendEvent( WID_MainButtons_Wnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
		
		nRui->SendEvent( WID_REPAIR_WND, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
		nRui->SendEvent( WID_NumberInput_Wnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
		nRui->SendEvent( WID_ItemBuyWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
		nRui->SendEvent( WID_ItemSellWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
		nRui->SendEvent( WID_InventoryWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
#ifdef WIDE_PAY_INVENTORY //유료인벤확장
		nRui->SendEvent( WID_PayInventory_Wnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );//유료인벤확장
#endif
		
		nRui->SendEvent( WID_StoreWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
		nRui->SendEvent( WID_Warehouse_Wnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
		nRui->SendEvent( WID_AbilityUpWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
		
		nRui->SendEvent( WID_MessageBox, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
		nRui->SendEvent( WID_REPAIR_WND, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
		nRui->SendEvent( WID_ITEMCREATE_WND, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
		nRui->SendEvent( WID_ITEMCREATEF_WND, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
		nRui->SendEvent( WID_SUB_ENCHANT_RESULT, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
		
		
		nRui->SendEvent( WID_ITEMCREATES_WND, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
		
		nRui->SendEvent( WID_SUB_ENCHANT, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
		
		nRui->SendEvent( WID_SUB_EXTRACT_RESULT, n_emSetVisible, (EPARAM)true, 0, 0, 0 );		
		nRui->SendEvent( WID_SUB_EXTRACT, n_emSetVisible, (EPARAM)true, 0, 0, 0 );		
		nRui->SendEvent( WID_SUB_REMODEL, n_emSetVisible, (EPARAM)true, 0, 0, 0 );		
		nRui->SendEvent( WID_SUB_REMODEL_FA, n_emSetVisible, (EPARAM)true, 0, 0, 0 );		
		nRui->SendEvent( WID_SUB_REMODEL_SUC, n_emSetVisible, (EPARAM)true, 0, 0, 0 );		
	}
	
	
	
}
