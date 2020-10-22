#include "..\\RockPCH.h"
#include <wchar.h>
#include "Rui.h"

#include "..\\..\\brother.h"
#include "..\\RockPCH.h"

#include "..\\..\\RockClient.h"

#include	"..\\..\\quadlist.h"
#include	<NETWORK\\Protocol.h>
#include	<Base\\D3DBase.h>
#include	<Obj\\ObjBase.h>
#include	<Map\\Field.h>

#include	"..\\..\\Pc.h"
#include	"..\\..\\jpeglib.h"
#pragma comment( lib, "jpeg.lib" )

#include "Define.h"
#include "Util.h"
#include "Render.h"
#include "Fontman.h"
#include "Resource.h"
#include "Chain.h"
#include "Container.h"

#include "..\\..\\global.h"
#include "..\\..\\RockClient.h"
#include "..\\..\\bravolibs\\obj\\player.h"

#include "EditBox.h"

#include "ExchangeWndProc.h"
#include "PartyInforWnd.h"
#include "LeagueInfoWnd.h"
#include "ChatMainProc.h"
#include "ChatPartWndList.h"
#include "CSocialActWnd.h"
#include "EmblemTexManager.h"
#include "CDelayBarWnd.h"

#include "SystemPrintWnd.h"
#include "Announcement.h"
#include "StringManager.h"
#include "CUserStoreSellWnd.h"
#include "MCommunityWnd.h"
#include "BattleZoneWndProc.h"
#include "ProposeJoinFrontier.h"


extern long		g_nowTime;
//----------------------------------------------------------------------------
// CChatActPrintWnd 채팅대화 활성화 출력창
//----------------------------------------------------------------------------

char		g_cEmblem[RP_MAX_EMBLEM_SIZE];

CChatActPrintWnd::CChatActPrintWnd()
{
	m_IsOpenInstant = false;
	m_lPrevtime = 0;
	
	m_pActChatBgWnd = NULL;
	m_pFilterTabWnd = NULL;
	m_pWholeViewTab = NULL;
	m_pPartyViewTab = NULL;
	m_pGuildViewTab = NULL;
	m_pAnnounViewTab = NULL;
	m_pHelpViewTab = NULL;

	m_pChatPrintWnd = NULL;
	m_pChatOutList = NULL;
	m_pChatScrollBarBg = NULL;
	m_pChatScrollBar = NULL;
	m_pInputFWhisDestBox = NULL;
	m_pTitleTexBox = NULL;

	ZeroMemory(m_strTitle , MAX_SIZE_256 );
	
	m_pChatInputWnd = NULL;
	m_pChatStateMenuWnd = NULL;
	m_pChatStateMenuViewBtn = NULL;
	m_pChatSizeContBtn = NULL;

	m_pChatStateMenuWnd = NULL;
	m_pNormalChatBtn = NULL;
	m_pGuildChatBtn = NULL;
	m_pPartyChatBtn = NULL;
	m_pWhisperChatBtn = NULL;	

	m_nLeftDownPos = SPoint2(0,0);
	m_uSizeState = ChatWndSizeNone;
	m_uFilterState = COutNoneView;
	m_uChatState = n_NormarChat;

	m_lPrevTime = 0;

	ZeroMemory(m_strChatstring , MAX_SIZE_256 );
	ZeroMemory(m_strOldChatstring , MAX_SIZE_256 );
	ZeroMemory(m_strCmd , MAX_SIZE_256 );

	ZeroMemory(m_strParam , MAX_SIZE_256 );
	ZeroMemory(m_strFWhisDest , MAX_SIZE_256 );
	ZeroMemory(m_strBWhisDest , MAX_SIZE_256 );

	m_IsInsertAnnounce = false;
	m_dUseShoutCode = 0;
}	

CChatActPrintWnd::~CChatActPrintWnd()
{

}

void CChatActPrintWnd::Init()
{



}

void CChatActPrintWnd::Composition()
{
	CompositionNew();
}

void CChatActPrintWnd::CompositionOld()
{

}

void CChatActPrintWnd::CompositionNew()
{
	m_nLeftDownPos = SPoint2(1, nRui->Desktop.Height - 59 );	
	
//#ifdef INDONESIA	
//	m_pActChatBgWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_ChatActFrame, WID_None, 
//			m_nLeftDownPos.x, m_nLeftDownPos.y , 386, 0, false );
//#else
	m_pActChatBgWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_ChatActFrame, WID_None, 
			m_nLeftDownPos.x, m_nLeftDownPos.y , 386, 0, false );
//#endif	
	m_pActChatBgWnd->SetClientImage( TID_None );
	m_pActChatBgWnd->SetWndProc( this );	
	m_pActChatBgWnd->SetTitle( false );
	m_pActChatBgWnd->SetVisible( false );	
	m_pActChatBgWnd->SetZorder( n_zoTop );
	m_pActChatBgWnd->SetMove(false);
	m_pActChatBgWnd->Initialize();

//#ifdef INDONESIA
//	m_pFilterTabWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_ChatFilterTabFrame, WID_ChatActFrame, 
//	0, 0, 386, 24, false );
//#else
	m_pFilterTabWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_ChatFilterTabFrame, WID_ChatActFrame, 
	0, 0, 386, 24, false );
//#endif	
	m_pFilterTabWnd->SetWndTexStyle( fwfTex_Style_Tile3 );
	m_pFilterTabWnd->SetClientImage( TID_CHATUPFRAME_LWND );
	m_pFilterTabWnd->SetWndTileSizeLR( 3, 3 );
	m_pFilterTabWnd->SetWndProc( this );	
	m_pFilterTabWnd->SetTitle( false );
	m_pFilterTabWnd->SetVisible( false );	
	m_pFilterTabWnd->SetZorder( n_zoBottom );
	m_pFilterTabWnd->Initialize();

	/*m_pFilterTabWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_ChatFilterTabFrame, WID_ChatActFrame, 
	29, 0, 434, 18, false );
	m_pFilterTabWnd->SetClientImage( TID_None );
	m_pFilterTabWnd->SetWndProc( this );
	m_pFilterTabWnd->SetFrame( false );
	m_pFilterTabWnd->SetTitle( false );
	m_pFilterTabWnd->SetVisible( false );
	m_pFilterTabWnd->SetCloseBtn( false );
	m_pFilterTabWnd->SetZorder( n_zoTop );
	m_pFilterTabWnd->SetMove(false);
	m_pFilterTabWnd->Initialize();*/
	
	m_pWholeViewTab = (CStateButton*)nRui->CreateWnd( n_wtStateButton, WID_None, WID_ChatFilterTabFrame, 
					  17, 0, 52, 18, false );
	m_pWholeViewTab->SetClientImage( TID_CFILTAB_NORMAL );
	m_pWholeViewTab->SetBtnImage( TID_None, TID_None, TID_CFILTAB_SELECT, TID_None );
	m_pWholeViewTab->SetAlignText( n_atCenter, n_atCenter );
	m_pWholeViewTab->SetBlank( 0, 0, 1, 0 );		
	m_pWholeViewTab->SetText( G_STRING_CHAR( IDS_TOTAL ) );								
	m_pWholeViewTab->SetTextColor(255, 255, 255);
	m_pWholeViewTab->SetDirectCheck(false);

	m_pPartyViewTab = (CStateButton*)nRui->CreateWnd( n_wtStateButton, WID_None, WID_ChatFilterTabFrame, 
					  71, 0, 52, 18, false );
	m_pPartyViewTab->SetClientImage( TID_CFILTAB_NORMAL );
	m_pPartyViewTab->SetBtnImage( TID_None, TID_None, TID_CFILTAB_SELECT, TID_None );
	m_pPartyViewTab->SetAlignText( n_atCenter, n_atCenter );
	m_pPartyViewTab->SetBlank( 0, 0, 1, 0 );		
	m_pPartyViewTab->SetText( G_STRING_CHAR( IDS_PARTY ) );								
	m_pPartyViewTab->SetTextColor(255, 255, 255);
	m_pPartyViewTab->SetDirectCheck(false);

	m_pGuildViewTab = (CStateButton*)nRui->CreateWnd( n_wtStateButton, WID_None, WID_ChatFilterTabFrame, 
					  125, 0, 52, 18, false );
	m_pGuildViewTab->SetClientImage( TID_CFILTAB_NORMAL );
	m_pGuildViewTab->SetBtnImage( TID_None, TID_None, TID_CFILTAB_SELECT, TID_None );
	m_pGuildViewTab->SetAlignText( n_atCenter, n_atCenter );
	m_pGuildViewTab->SetBlank( 0, 0, 1, 0 );		
	m_pGuildViewTab->SetText( G_STRING_CHAR( IDS_CHARSTATE_GUILD2 ) );								
	m_pGuildViewTab->SetTextColor(255, 255, 255);
	m_pGuildViewTab->SetDirectCheck(false);

	m_pAnnounViewTab = (CStateButton*)nRui->CreateWnd( n_wtStateButton, WID_None, WID_ChatFilterTabFrame, 
					  179, 0, 52, 18, false );
	m_pAnnounViewTab->SetClientImage( TID_CFILTAB_NORMAL );
	m_pAnnounViewTab->SetBtnImage( TID_None, TID_None, TID_CFILTAB_SELECT, TID_None );
	m_pAnnounViewTab->SetAlignText( n_atCenter, n_atCenter );
	m_pAnnounViewTab->SetBlank( 0, 0, 1, 0 );		
	m_pAnnounViewTab->SetText( G_STRING_CHAR( IDS_NOTICE_T2 ) );								
	m_pAnnounViewTab->SetTextColor(255, 255, 255);
	m_pAnnounViewTab->SetDirectCheck(false);

	m_pHelpViewTab = (CStateButton*)nRui->CreateWnd( n_wtStateButton, WID_None, WID_ChatFilterTabFrame, 
					  233, 0, 52, 18, false );
	m_pHelpViewTab->SetClientImage( TID_CFILTAB_NORMAL );
	m_pHelpViewTab->SetBtnImage( TID_None, TID_None, TID_CFILTAB_SELECT, TID_None );
	m_pHelpViewTab->SetAlignText( n_atCenter, n_atCenter );
	m_pHelpViewTab->SetBlank( 0, 0, 1, 0 );		
	m_pHelpViewTab->SetText( G_STRING_CHAR( IDS_WND_HELP ) );								
	m_pHelpViewTab->SetTextColor(255, 255, 255);
	m_pHelpViewTab->SetDirectCheck(false);

	nRui->SetGroupWnd( 5, m_pWholeViewTab->GetID(), 
					  m_pWholeViewTab->GetID(), m_pPartyViewTab->GetID(), 
					  m_pGuildViewTab->GetID(), m_pAnnounViewTab->GetID(),
					  m_pHelpViewTab->GetID());
//#ifdef INDONESIA	
//	m_pChatPrintWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_ChatPrintFrame, WID_ChatActFrame, 
//	0, 0, 386, 0, false );
//#else
	m_pChatPrintWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_ChatPrintFrame, WID_ChatActFrame, 
	0, 0, 386, 0, false );
//#endif	
	m_pChatPrintWnd->SetClientImage( TID_CLIENT );
	m_pChatPrintWnd->SetClientColor(50, 50, 50);
	m_pChatPrintWnd->SetClientAlpah(75);
	m_pChatPrintWnd->SetWndProc( this );
	
	m_pChatPrintWnd->SetTitle( false );
	m_pChatPrintWnd->SetVisible( false );
	m_pChatPrintWnd->SetZorder( n_zoBottom );
	m_pChatPrintWnd->Initialize();	

//#ifdef INDONESIA	
//	m_pChatOutList = (CChatOutList*)nRui->CreateWnd( n_wtChatOutList, WID_None, WID_ChatPrintFrame, 
//					 19, 0, 386, 0, false );	
//#else
	m_pChatOutList = (CChatOutList*)nRui->CreateWnd( n_wtChatOutList, WID_None, WID_ChatPrintFrame, 
					 19, 0, 386, 0, false );
//#endif	
	m_pChatOutList->SetOutputLevel(ChatWndSizeMid);
	m_pChatOutList->SetChatOutActState(COutState_Act);
	m_pChatOutList->SetFilterState(COutTotalView);
	m_pChatOutList->SetRefpViewChatTxtData(g_ChatMainProc.GetViewChatTxtDataList());
	m_pChatOutList->InitChatOutList();
	
	m_pChatScrollBarBg = (CWnd*)nRui->CreateWnd( n_wtWnd, WID_None, WID_ChatPrintFrame, 
		0, 0, 17, 0, false );	
	m_pChatScrollBarBg->SetClientImage(TID_CLIENT);
	m_pChatScrollBarBg->SetClientColor(50, 50, 50);
	m_pChatScrollBarBg->SetClientAlpah(200);
	
	m_pChatScrollBar =  (CScrollBar*)nRui->CreateWnd( n_wtScrollBar, WID_None, WID_ChatPrintFrame, 
						2, 0, 13, 0, false );
	m_pChatScrollBar->SetClientImage( TID_None );
	m_pChatScrollBar->SetThumbImage( TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR, 
					  TID_CTRL_SCR_BAR, TID_None );
	m_pChatScrollBar->InitSlide( stVertSld, 0, (MAX_CHATDATA_NUM - 0), 0, 13 );//Type, min, max, value, 썸의 폭
	m_pChatScrollBar->SetScrollBtnA( TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT, 
					  TID_CTRL_SCR2_UBT_CLK, TID_None );
	m_pChatScrollBar->SetScrollBtnB( TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT, 
					  TID_CTRL_SCR2_DBT_CLK, TID_None );
	
//#ifdef INDONESIA	
//	m_pTitleTexBox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_ChatPrintFrame, 
//		0, 0, 386, 22, false );
//#else	
	m_pTitleTexBox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_ChatPrintFrame, 
		0, 0, 386, 22, false );
//#endif
	m_pTitleTexBox->SetAlignText(n_atLeft, n_atCenter);
	m_pTitleTexBox->SetClientImage(TID_CHAT_TITLEPATN);
	m_pTitleTexBox->SetBlank( 6, 0, 4, 0 );
	m_pTitleTexBox->SetTextColor(255, 255, 255);
	m_pTitleTexBox->SetText(_RT(""));

	m_pInputFWhisDestBox = (CEditBox*)nRui->CreateWnd( n_wtEditBox, WID_None, WID_ChatPrintFrame, 
		4, 3, 111, 17, false );
	m_pInputFWhisDestBox->SetAlignText(n_atLeft, n_atCenter);
	m_pInputFWhisDestBox->SetClientImage(TID_CLIENT);
	m_pInputFWhisDestBox->SetClientAlpah(128);
	m_pInputFWhisDestBox->SetClientColor(0, 0 ,0);
	m_pInputFWhisDestBox->SetBlank( 6, 0, 2, 0 );
	m_pInputFWhisDestBox->SetTextColor( 255, 175, 255 );
	m_pInputFWhisDestBox->SetTextSize(RP_MAX_PC_NAME);
	m_pInputFWhisDestBox->SetText(_RT(""));
	m_pInputFWhisDestBox->SetZorder( n_zoTop );
	m_pInputFWhisDestBox->SetEditStyle( esText );
	m_pInputFWhisDestBox->SetReturnActionClear( true );
	m_pInputFWhisDestBox->InitEditBox();	
	m_pInputFWhisDestBox->SetVisible(false);
	m_pInputFWhisDestBox->SetHID(HD_WHISPER);
	
#if defined (ONLY_ENGLISH_SET) 		
	m_pChatSizeContBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_ChatPrintFrame, 
	                                550, 363, 15, 15, false );    
#else
	m_pChatSizeContBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_ChatPrintFrame, 
	                                373, 363, 15, 15, false );
#endif
	
	m_pChatSizeContBtn->SetClientImage( TID_CSIZEBTN_NORMAL );
    m_pChatSizeContBtn->SetBtnImage( TID_None, TID_CSIZEBTN_FOCUS, 
							   TID_CSIZEBTN_CLICK, TID_None );

	m_pChatStateMenuWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_ChatStateMenuFrame, WID_ChatActFrame, 
	0, 0, 26, 153, false );
	m_pChatStateMenuWnd->SetClientImage(TID_None);
	m_pChatStateMenuWnd->SetWndProc( this );
	m_pChatStateMenuWnd->SetTitle( false );
	m_pChatStateMenuWnd->SetVisible( false );
	m_pChatStateMenuWnd->SetZorder( n_zoTop );
	m_pChatStateMenuWnd->Initialize();

	m_pNormalChatBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_ChatStateMenuFrame, 
	                                3, 2, 24, 20, false );
    m_pNormalChatBtn->SetClientImage( TID_NORMALCHAT_NORMAL );
    m_pNormalChatBtn->SetBtnImage( TID_None, TID_NORMALCHAT_FOCUS, 
							   TID_NORMALCHAT_CLICK, TID_None );
	m_pNormalChatBtn->SetHID(HD_MNORMAL_CHAT);

	m_pGuildChatBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_ChatStateMenuFrame, 
	                                3, 24, 24, 20, false );
    m_pGuildChatBtn->SetClientImage( TID_GUILDCHAT_NORMAL );
    m_pGuildChatBtn->SetBtnImage( TID_None, TID_GUILDCHAT_FOCUS, 
							   TID_GUILDCHAT_CLICK, TID_None );
	m_pGuildChatBtn->SetHID(HD_MGUILD_CHAT);	

	m_pPartyChatBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_ChatStateMenuFrame, 
	                                3, 46, 24, 20, false );
    m_pPartyChatBtn->SetClientImage( TID_PARTYCHAT_NORMAL );
    m_pPartyChatBtn->SetBtnImage( TID_None, TID_PARTYCHAT_FOCUS, 
							   TID_PARTYCHAT_CLICK, TID_None );
	m_pPartyChatBtn->SetHID(HD_MPARTY_CHAT);

	m_pWhisperChatBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_ChatStateMenuFrame, 
	                                3, 68, 24, 20, false );
    m_pWhisperChatBtn->SetClientImage( TID_WHISCHAT_NORMAL );
    m_pWhisperChatBtn->SetBtnImage( TID_None, TID_WHISCHAT_FOCUS, 
							   TID_WHISCHAT_CLICK, TID_None );
	m_pWhisperChatBtn->SetHID(HD_MWHISPER_CHAT);


	m_pWholeChannelBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_ChatStateMenuFrame, 
	                                3, 90, 24, 20, false );
    m_pWholeChannelBtn->SetClientImage( TID_CHANNELC_NORMAL );
    m_pWholeChannelBtn->SetBtnImage( TID_None, TID_CHANNELC_FOCUS, 
							   TID_CHANNELC_CLICK, TID_None );
	m_pWholeChannelBtn->SetHID(HD_MTCHANNELC_BTN);
	
	m_pWholeServerBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_ChatStateMenuFrame, 
	                                3, 112, 24, 20, false );
    m_pWholeServerBtn->SetClientImage( TID_SERVERC_NORMAL );
    m_pWholeServerBtn->SetBtnImage( TID_None, TID_SERVERC_FOCUS, 
							   TID_SERVERC_CLICK, TID_None );
	m_pWholeServerBtn->SetHID(HD_MTSETVERC_BTN);

	m_pWholeAreaBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_ChatStateMenuFrame, 
	                                3, 134, 24, 20, false );
    m_pWholeAreaBtn->SetClientImage( TID_AREA_BRODCAST_NORMAL );
    m_pWholeAreaBtn->SetBtnImage( TID_None, TID_AREA_BRODCAST_FOCUS, 
							   TID_AREA_BRODCAST_CLICK, TID_None );
	m_pWholeAreaBtn->SetHID(278);
	
//#ifdef INDONESIA	
//	m_pChatInputWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_ChatInputFrame, WID_None, 
//	m_nLeftDownPos.x, m_nLeftDownPos.y - 23, 386, 23, false );
//#else	
	m_pChatInputWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_ChatInputFrame, WID_None, 
	m_nLeftDownPos.x, m_nLeftDownPos.y - 23, 386, 23, false );
//#endif	
	m_pChatInputWnd->SetWndTexStyle( fwfTex_Style_Tile3 );
	m_pChatInputWnd->SetClientImage( TID_CHATINPUT_LWND );
	m_pChatInputWnd->SetWndTileSizeLR( 30, 24 );
	m_pChatInputWnd->SetWndProc( this );


	m_pChatInputWnd->SetTitle( false );
	m_pChatInputWnd->SetVisible( false );
	m_pChatInputWnd->SetMove(false);
	m_pChatInputWnd->SetZorder( n_zoTop );
	m_pChatInputWnd->Initialize();

	m_pChatStateMenuViewBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_ChatInputFrame, 
	                                3, 1, 24, 20, false );
    m_pChatStateMenuViewBtn->SetClientImage( TID_NORMALCHAT_NORMAL );
    m_pChatStateMenuViewBtn->SetBtnImage( TID_None, TID_NORMALCHAT_FOCUS, 
							   TID_NORMALCHAT_CLICK, TID_None );
	m_pInputEditBox = (CChatEditBox*)nRui->CreateWnd( n_wtChatEditBox, WID_None, WID_ChatInputFrame, 
		35, 3, 320, 16, false );
	m_pInputEditBox->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );

#if defined (ONLY_ENGLISH_SET) 	
	m_pInputEditBox->SetTextSize( RP_MAX_CHAT_SIZE - 2 );
	m_pInputEditBox->SetLimitLen( 32 );
#else
	m_pInputEditBox->SetTextSize( 62 );
#endif	
	//m_pInputEditBox->SetTextSize( 152 );
	m_pInputEditBox->SetClientImage( TID_None );	
	m_pInputEditBox->SetClientColor( 0, 0, 0 );
	m_pInputEditBox->SetAlignText( n_atLeft, n_atCenter );
	m_pInputEditBox->SetBlank( 1, 1, 0, 0 );
	m_pInputEditBox->SetText( _RT("") );
	m_pInputEditBox->SetEditStyle( esText );
	m_pInputEditBox->SetReturnActionClear( true );
	m_pInputEditBox->InitEditBox();	
	
	m_pActChatBgWnd->SetMoveLinkFrameWnd(m_pChatInputWnd);	
	
	SetSizeState(ChatWndSizeMin);	
	
	SetFilterTabState(COutTotalView);
	SetChatState(n_NormarChat);
	SetWhisDest(_RT(""));
//////////////////////////////////////////////////////////////////////////
//아이디 클릭시 관련 팝업 뛰우기
//////////////////////////////////////////////////////////////////////////

	m_pChatIDPopupWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_ChatIDPopupFrame, 
				             WID_None, 0, 30, 92, 90 - 16   , false );

	m_pChatIDPopupWnd->SetClientImage( TID_Party_PopupWnd );
    m_pChatIDPopupWnd->SetCaption( _RT("PartyPopup Window") );
    m_pChatIDPopupWnd->SetWndProc( this );
    m_pChatIDPopupWnd->SetTitle( false );
	m_pChatIDPopupWnd->SetVisible( false );
    m_pChatIDPopupWnd->SetMove( false );
    m_pChatIDPopupWnd->SetZorder( n_zoTop );
    m_pChatIDPopupWnd->Initialize();
	
	m_pChatIDPopupBtn1 = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_ChatIDPopupFrame, 
	                                4, 4, 88, 14, false );
	m_pChatIDPopupBtn1->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pChatIDPopupBtn1->SetClientImage( TID_None );	
	m_pChatIDPopupBtn1->SetAlignText( n_atLeft, n_atCenter );   
	m_pChatIDPopupBtn1->SetText( G_STRING_CHAR(IDS_WND_WHISPER) );	
	m_pChatIDPopupBtn1->SetTextColor( 255, 255, 255 );   
	m_pChatIDPopupBtn1->SetZorder( n_zoTop );
	m_pChatIDPopupBtn1->SetBtnImage( TID_None, TID_Party_TextFocus, 
								   TID_None, TID_None );	
	
	m_pChatIDPopupBtn2 = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_ChatIDPopupFrame, 
	                                4, 20, 88, 14, false );
	m_pChatIDPopupBtn2->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pChatIDPopupBtn2->SetClientImage( TID_None );	
	m_pChatIDPopupBtn2->SetAlignText( n_atLeft, n_atCenter );   
	m_pChatIDPopupBtn2->SetText( G_STRING_CHAR(IDS_WND_PRATY_REQUEST)); 
	m_pChatIDPopupBtn2->SetTextColor( 255, 255, 255 );   
	m_pChatIDPopupBtn2->SetZorder( n_zoTop );
	m_pChatIDPopupBtn2->SetBtnImage( TID_None, TID_Party_TextFocus, 
								   TID_None, TID_None );	

	m_pChatIDPopupBtn3 = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_ChatIDPopupFrame, 
	                                4, 36, 88, 14, false );
	m_pChatIDPopupBtn3->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pChatIDPopupBtn3->SetClientImage( TID_None );	
	m_pChatIDPopupBtn3->SetAlignText( n_atLeft, n_atCenter );   
	m_pChatIDPopupBtn3->SetText( G_STRING_CHAR(IDS_WND_LEAGUE_REQUEST) );	
	m_pChatIDPopupBtn3->SetTextColor( 255, 255, 255 );   
	m_pChatIDPopupBtn3->SetZorder( n_zoTop );
	m_pChatIDPopupBtn3->SetBtnImage( TID_None, TID_Party_TextFocus, 
								   TID_None, TID_None );

	m_pChatIDPopupBtn4 = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_ChatIDPopupFrame, 
									4, 52, 88, 14, false );
	m_pChatIDPopupBtn4->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pChatIDPopupBtn4->SetClientImage( TID_None );	
	m_pChatIDPopupBtn4->SetAlignText( n_atLeft, n_atCenter );   
	m_pChatIDPopupBtn4->SetText( G_STRING_CHAR( IDS_WND_COM_CANCEL ));
	m_pChatIDPopupBtn4->SetTextColor( 255, 255, 255 );   
	m_pChatIDPopupBtn4->SetZorder( n_zoTop );
	m_pChatIDPopupBtn4->SetBtnImage( TID_None, TID_Party_TextFocus, 
								   TID_None, TID_None );

}

void CChatActPrintWnd::RePosition()
{
	m_pChatInputWnd->Reposition(m_nLeftDownPos.x, m_nLeftDownPos.y - 23);
}

void CChatActPrintWnd::Update()
{
	static DWORD dSwitch_time = g_nowTime;
	static bool  bSwitchState = true;
	
	if(m_IsInsertAnnounce)
	{
		if( SAFE_TIME_COMPARE( g_nowTime , > , SAFE_TIME_ADD( dSwitch_time , 500 ) ) )
		{
			bSwitchState = !bSwitchState;
			dSwitch_time = g_nowTime;
		}
		if(bSwitchState)
		{
			m_pAnnounViewTab->SetClientImage( TID_CFILTAB_NORMAL );
			m_pAnnounViewTab->SetTextColor(200, 200, 200);
		}
		else
		{
			m_pAnnounViewTab->SetClientImage( TID_CFILTAB_SELECT );
			m_pAnnounViewTab->SetTextColor(255, 255, 255);
		}
	}
	else
	{
		if(!bSwitchState)
		{
			m_pAnnounViewTab->SetClientImage( TID_CFILTAB_NORMAL );
			m_pAnnounViewTab->SetTextColor(255, 255, 255);
			bSwitchState = true;
		}
	}
	if( (nRui->GetEMsg()->Mouse.Action == MA_LPRESS) ||
		(nRui->GetEMsg()->Mouse.Action == MA_LDRAG) )
	{
		if( !CheckFocusWnd(WID_ChatIDPopupFrame) ) 
		{	
		   nRui->SendEvent( WID_ChatIDPopupFrame , n_emSetVisible, (EPARAM)false, 0, 0, 0 );		
		}		
	}
}

BOOL CChatActPrintWnd::CheckFocusWnd(WNDID Wid)
{
	WNDID FocusWnd = nRui->GetFocusWnd();

	if( FocusWnd == Wid )
	return true;

   CWnd * Twnd = Container->Get( FocusWnd );
   if(Twnd)
   {
	  WNDID wndid = Twnd->GetParentID();
	  if( wndid == Wid )
		  return true;
   }

   return false;

}


void CChatActPrintWnd::SetInsertAnnounce(bool bFlag)
{
	m_IsInsertAnnounce = bFlag;
}

void CChatActPrintWnd::SetWhisDest(void *pstrDest)
{
	char* strDest = (char*)(pstrDest);
	SAFE_STR_CPY( m_strFWhisDest, strDest, 256 );
	m_pInputFWhisDestBox->SetText(m_strFWhisDest);
	PostMessage( m_pInputFWhisDestBox->GetHEdit(), WM_USER + 44, NULL, NULL );	
}

void CChatActPrintWnd::ResetScroll()
{
	int nLastValue = ( MAX_CHATDATA_NUM - m_pChatOutList->GetOutPutLine() );
	m_pChatScrollBar->ReSetScroll(0, nLastValue, nLastValue);
	m_pChatOutList->SetDelValue(0);	
}

void CChatActPrintWnd::SetListScroll( int _linenum )
{
	int nDelValue = m_pChatScrollBar->GetSldMax() - _linenum;
	m_pChatOutList->SetDelValue(nDelValue);
}

bool CChatActPrintWnd::SetWheelValue(SMouse & theMouse)
{
	if( theMouse.State == n_msWheel && theMouse.WheelValue != 0 )
	{
		POINT curtMousePos;
		GetCursorPos( &curtMousePos );
		ScreenToClient( g_RockClient.GetApp(), &curtMousePos );	
		
		if( IsVisible && 
		  GetFrameWnd()->FocusCheck( curtMousePos.x, curtMousePos.y, false ) )
		{
			SetWheelValue( theMouse.WheelValue );
			return true;
		}	
	}
	
	return false;
}

void CChatActPrintWnd::SetWheelValue( short _wheel )
{
	int value = 0;
	int nMax = 0;

	m_pChatScrollBar->GetValue( &value );
	nMax = m_pChatScrollBar->GetSldMax();
	
	if( ( _wheel < 0 ) && ( value < nMax ) )
	{
		value += 1;
	}
	else if( ( _wheel > 0 ) && ( value > 0 ) )
	{
		value -= 1;
	}
	
	m_pChatScrollBar->SetValue( value );
	SetListScroll( value );
}

void CChatActPrintWnd::Proc( SEventMessage* EMsg )
{
//----------------------------------------------------------------------------------------------	
//상단 필터링 탭 프레임
//----------------------------------------------------------------------------------------------
	if( EMsg->FocusWnd == m_pWholeViewTab->GetID() )
	{	
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			SetFilterTabState(COutTotalView);				
		}	
	}

	if( EMsg->FocusWnd == m_pPartyViewTab->GetID() )
	{	
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			SetFilterTabState(COutPartyView);				
		}	
	}

	if( EMsg->FocusWnd == m_pGuildViewTab->GetID() )
	{	
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			SetFilterTabState(COutGuildView);				
		}	
	}

	if( EMsg->FocusWnd == m_pAnnounViewTab->GetID() )
	{	
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			m_IsInsertAnnounce = false;
			SetFilterTabState(COutAnnounView);				
		}	
	}

	if( EMsg->FocusWnd == m_pHelpViewTab->GetID() )
	{	
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			SetFilterTabState(COutHelpView);				
		}	
	}

//----------------------------------------------------------------------------------------------
//중단 채팅출력 프레임
//----------------------------------------------------------------------------------------------
	if( EMsg->FocusWnd == m_pChatOutList->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			nRui->SendEvent( WID_ChatIDPopupFrame , n_emSetVisible, (EPARAM)false, 0, 0, 0 );
			SChatTxtData *pFocusChatData = m_pChatOutList->GetFocusChatData();
			if(pFocusChatData)
			{
				SAFE_STR_CPY( m_strFWhisDest, pFocusChatData->Dest, 256 );
				SetChatState(n_WhisperChat);
			}
		}	

		
		else if ( EMsg->Notify == NM_BTN_RCLICK )
		{
			nRui->SendEvent( WID_ChatIDPopupFrame , n_emSetVisible, (EPARAM)false, 0, 0, 0 );
			SChatTxtData *pFocusChatData = m_pChatOutList->GetFocusChatData();
			if(pFocusChatData)
			{
				SAFE_STR_CPY( m_strIDName, pFocusChatData->Dest, 32 );
				
				POINT		ptCursor;
				GetCursorPos( &ptCursor );
				::ScreenToClient( g_RockClient.GetApp(), &ptCursor );
				ptCursor.x -= 5;
				ptCursor.y -= 5;
				
				nRui->SendEvent( WID_ChatIDPopupFrame , n_emSetVisible, (EPARAM)true, 0, 0, 0 );		  
				m_pChatIDPopupWnd->SetAbsolutePosition( ptCursor.x, ptCursor.y );
				
				
			}
			else
			{
				SAFE_STR_CPY(m_strIDName , _RT("") , 32 );
			}	
			
			
		}	
	}
	if( EMsg->FocusWnd == m_pChatIDPopupBtn1->GetID()  )
	{
		//귓속말
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			nRui->SendEvent( WID_ChatIDPopupFrame , n_emSetVisible, (EPARAM)false, 0, 0, 0 );
			if( m_strIDName != NULL)
			{
				SAFE_STR_CPY( m_strFWhisDest, m_strIDName , 32 );
				SetChatState(n_WhisperChat);
			}	
		
		}
	}
	else if (EMsg->FocusWnd == m_pChatIDPopupBtn2->GetID() )
	{
		//파티초대
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			nRui->SendEvent( WID_ChatIDPopupFrame , n_emSetVisible, (EPARAM)false, 0, 0, 0 );
			if(  m_strIDName != NULL)
			{
				ExcuteInvitePatry(m_strIDName);
			}	
				
		}

	}		
	else if(EMsg->FocusWnd == m_pChatIDPopupBtn3->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			nRui->SendEvent( WID_ChatIDPopupFrame , n_emSetVisible, (EPARAM)false, 0, 0, 0 );
			if(  m_strIDName != NULL)
			{
				ExcuteInvitePatry(m_strIDName , true );
			}	
			
		}
	}	
	else if( EMsg->FocusWnd == m_pChatIDPopupBtn4->GetID() )
	{
		if(EMsg->Notify == NM_BTN_CLICK)
		{
			nRui->SendEvent( WID_ChatIDPopupFrame , n_emSetVisible , (EPARAM)false , 0 ,0 , 0 );
		}	
	}	

	if( EMsg->FocusWnd == m_pChatScrollBar->GetID()  )
	{
		if( ( EMsg->Notify == NM_SLD_MOVEBAR ) || ( EMsg->Notify == NM_SLD_CLICKMOVEBAR ) 
			|| ( EMsg->Notify == NM_SLD_BTNMOVERBAR ) )
		{
			SetListScroll( (int)EMsg->fParam );				
		}	
	}

	if( EMsg->FocusWnd == m_pInputFWhisDestBox->GetID() )
	{
		if( EMsg->Notify == NM_EDT_RETURN_ACTION )
		{
			nRui->SetEnterFlag(n_nfEditEnter);
			nRui->SendEvent( m_pInputFWhisDestBox->GetID(), n_emGetText, 
				(EPARAM)m_strBWhisDest, 256, 0, 0 );
			SetWhisDest(m_strBWhisDest);
			nRui->SendEvent( m_pInputEditBox->GetID(), n_emSetFocus, 0, 0, 0, 0 );			
		}	
	}

//----------------------------------------------------------------------------------------------
//하단 채팅입력, 컨트롤 프레임
//----------------------------------------------------------------------------------------------
	if( EMsg->FocusWnd == m_pChatStateMenuViewBtn->GetID() )
	{	
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			bool bFlag = !(m_pChatStateMenuWnd->IsVisible());
			nRui->SendEvent( WID_ChatStateMenuFrame, n_emSetVisible, 
			(EPARAM)bFlag, 0, 0, 0 );	
		}	
	}

	if( EMsg->FocusWnd == m_pChatSizeContBtn->GetID() )
	{	
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			UINT uSizeState = m_uSizeState;
			uSizeState++;
			if(uSizeState > ChatWndSizeMax)
			{
				uSizeState = ChatWndSizeMin;
			}
			SetSizeState(uSizeState);
		}	
	}

	if( EMsg->FocusWnd == m_pInputEditBox->GetID() )
	{	
		if( EMsg->Notify == NM_EDT_SPACE_ACTION )
		{
			SlashChatProc();
		}
		   
		if( EMsg->Notify == NM_EDT_RETURN_ACTION )
		{
			int	nBuflen = 256;
		
			ZeroMemory(m_strChatstring , MAX_SIZE_256);
			ZeroMemory(m_strCmd , MAX_SIZE_256);
			ZeroMemory(m_strParam , MAX_SIZE_256);
			
			nRui->SendEvent( m_pInputEditBox->GetID(), n_emGetText, 
				(EPARAM)m_strChatstring, nBuflen, 0, 0 );			
#ifdef UNICODE_ROCK
			swscanf(RWCHAR(m_strChatstring), _RT("%s %s"), RWCHAR(m_strCmd), RWCHAR(m_strParam) );			
#else
			sscanf(m_strChatstring, "%s %s", m_strCmd, m_strParam );	
#endif
	
			
			if(!SlashExcuteProc())
			{
				ChattingProc();	
			}

			nRui->SendEvent( m_pInputEditBox->GetID(), n_emEditDisable, 0, 0, 0, 0 );
			nRui->SendEvent( WID_ChatIDPopupFrame , n_emSetVisible , (EPARAM)false , 0 ,0 , 0 );
			m_IsOpenInstant = false;		
		}
	}

//----------------------------------------------------------------------------------------------
//채팅상태 메뉴 윈도우
//----------------------------------------------------------------------------------------------
	if( EMsg->FocusWnd == m_pNormalChatBtn->GetID() )
	{	
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			SetChatState(n_NormarChat);
		}	
	}

	if( EMsg->FocusWnd == m_pGuildChatBtn->GetID() )
	{	
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			SetChatState(n_GuildChat);
		}	
	}

	if( EMsg->FocusWnd == m_pPartyChatBtn->GetID() )
	{	
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			SetChatState(n_PartyChat);
		}	
	}

	if( EMsg->FocusWnd == m_pWhisperChatBtn->GetID() )
	{	
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			SetChatState(n_WhisperChat);
		}
	}

	if( EMsg->FocusWnd == m_pWholeChannelBtn->GetID() )
	{	
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			SetChatState(n_WholeChannelChat);
		}
	}

	if( EMsg->FocusWnd == m_pWholeServerBtn->GetID() )
	{	
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			SetChatState(n_WholeServerChat);
		}
	}

	if( EMsg->FocusWnd == m_pWholeAreaBtn->GetID() )
	{	
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			SetChatState(n_WholeAreaChat);
		}
	}

	return;
}

void CChatActPrintWnd::SlashChatProc()
{
	ZeroMemory(m_strChatstring , MAX_SIZE_256);
	ZeroMemory(m_strCmd , MAX_SIZE_256);
	ZeroMemory(m_strParam , MAX_SIZE_256);

	int	nBuflen = 256;
	nRui->SendEvent( m_pInputEditBox->GetID(), n_emGetText, (EPARAM)m_strChatstring, nBuflen, 0, 0 );

#ifdef UNICODE_ROCK
	swscanf(RWCHAR(m_strChatstring), _RT("%s %s"), RWCHAR(m_strCmd), RWCHAR(m_strParam) );
#else
	sscanf(m_strChatstring, "%s %s", m_strCmd, m_strParam );
#endif
	

	bool bSlashChatFlag = false;	
	
	//일반채팅
	if( (Rstrcmp(m_strCmd, _RT("/s") ) == 0) ||					
		(Rstrcmp(m_strCmd, _RT("/S") ) == 0) )
	//	(Rstrcmp(m_strCmd, _RT("/ㄴ") ) == 0) )				
	{
		SetChatState(n_NormarChat);	
		bSlashChatFlag = true;
	}

	//파티채팅
	if( (Rstrcmp(m_strCmd, _RT("/p") ) == 0) ||					
		(Rstrcmp(m_strCmd, _RT("/P") ) == 0) )					
	//	(Rstrcmp(m_strCmd, _RT("/ㅔ") ) == 0) )				
	{
		SetChatState(n_PartyChat);	
		bSlashChatFlag = true;
	}
	
	//길드채팅
	if( (Rstrcmp(m_strCmd, _RT("/g") ) == 0) ||					
		(Rstrcmp(m_strCmd, _RT("/G") ) == 0)  )				   
	//	(Rstrcmp(m_strCmd, _RT("/ㅎ") ) == 0) )				
	{
		SetChatState(n_GuildChat);
		bSlashChatFlag = true;
	}

	//귓속말 
	if( (Rstrcmp(m_strCmd, _RT("/w") ) == 0) ||					
		(Rstrcmp(m_strCmd, _RT("/W") ) == 0) )					
	//	(Rstrcmp(m_strCmd, _RT("/ㅈ") ) == 0) )				
	{
		//귓속말 대상
    	if(Rstrcmp(m_strParam, _RT("") ) != 0)
		{
			SAFE_STR_CPY( m_strFWhisDest, m_strParam, 256 );
			SetChatState(n_WhisperChat);
			bSlashChatFlag = true;
		}		
	}
	
	//귓속말 마지막 대상자에게 되돌림
	if( (Rstrcmp(m_strCmd, _RT("/r") ) == 0) ||					
		(Rstrcmp(m_strCmd, _RT("/R") ) == 0) )			
	{
		SetChatState(n_WhisperChat);
		bSlashChatFlag = true;				
	}
	
	if(bSlashChatFlag)
	{
		nRui->SendEvent( m_pInputEditBox->GetID(), n_emClearText, 0, 0, 0, 0 );
		PostMessage( nRui->GetCurrentWndHandle(), WM_USER + 44, NULL, NULL );
	}
}

bool CChatActPrintWnd::SlashExcuteProc()
{
	SPARTY_INFO *pPartyInfo = g_PartyInforWnd.GetPartyInfo();
	SPcDataParam* PcParam = nRui->GetPcParamInfo();
	
	LPACTION_DATA pActionData = NULL;		
	//................................................................................................
	// 플레이어 행동 처리부분 By wxywxy 
	//................................................................................................
	if( ( pActionData = g_RockClient.m_Action.CheckKeyWord( ( char * ) m_strCmd ) ) != NULL )
	{
		//라이딩 중이면 사용할수 없다
		if( ( g_Pc.GetPlayer()->m_State.bRide && pActionData->ViewRide == 0 ) 
			|| ( g_DelayBarWnd.IsVisible && DELAY_RIDE_ON == g_DelayBarWnd.GetDelay_Type() ) )
		{
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANNOT_THEFUNCTION_INRIDING ) );										
			return true;
		}			
				
		if( ( g_Pc.GetPlayer()->m_State.bRide == false )  && pActionData->ViewRide == 1 )
		{
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_NOT_RIDING));									
			return true;
		}
		
		if( g_Pc.GetPlayer()->m_State.bRide && pActionData->ViewRide == 1 )
		{
			SNPCBaseInfo* npc_Info = NULL;
			DWORD dNPCCode = g_Map.m_NpcTable[ g_Pc.GetPlayer()->m_State.TransIndex ].code;
			npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( dNPCCode );

			if( !npc_Info )
			{
				return true;
			}

			///-- 말이 아닐경우
			if( npc_Info->theTargetRange != 0 )
			{
				return true;
			}
			
			return true;
		}
		int  iMotionNum = 0 ; 
		switch( g_Pc.GetPlayer()->m_curt_event.type )
		{
		case NULL:
		case SM_SIMSIM:
		case SM_ACT:
#ifdef ADD_MOTION_DANCE
			if( pActionData->Index == 11 )	
			{
				iMotionNum = 23; 
			}
			else
			{
				iMotionNum = pActionData->Index ;
			}
#endif
			g_RockClient.Send_CsExpression( g_Pc.GetPlayer()->GetUnique() , (BYTE)iMotionNum );
			
		}

		return true;
	}
	//................................................................................................


//----------------------------------------------------------------------------------------------------
//		파티초대
//----------------------------------------------------------------------------------------------------	

	if( (Rstrcmp(m_strCmd, _RT("/i") ) == 0) ||					
		(Rstrcmp(m_strCmd, _RT("/I") ) == 0) )					
	//	(Rstrcmp(m_strCmd, _RT("/ㅑ") ) == 0) )				
	{
		
		ExcuteInvitePatry(m_strParam);	
		
		return true;
	}

//----------------------------------------------------------------------------------------------------
//		파티탈퇴
//----------------------------------------------------------------------------------------------------		
	if( (Rstrcmp(m_strCmd, _RT("/l") ) == 0) ||					
		(Rstrcmp(m_strCmd, _RT("/L") ) == 0) )					
	//	(Rstrcmp(m_strCmd, _RT("/ㅣ") ) == 0) )				
	{
		if( !g_PartyInforWnd.IsMemberParty() )
		{
			SystemPrint( n_DarkGreen ,n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOT_BELONG_PARTY ) );		 	   	  	
		   return true;
		}
		
		if( Rstrcmp(m_strParam, _RT("") ) == 0 )
		{
			SAFE_STR_CPY( m_strParam, PcParam->PCName, 256 );
		}
		else
		{
		   if( g_Pc.m_MainPC.lUnique != pPartyInfo->m_dLeaderID )
		   {
			   SystemPrint( n_DarkGreen ,n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOT_AUTHORITY ) );		 			 	      
			  return true;	   
		   }			
		}

		int iLen = Rstrlen(RWCHAR(m_strParam));
		if( iLen > (RP_MAX_PC_NAME - sizeof(RTCHAR)) )
		{
			SystemPrint( n_DarkGreen ,n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_OVERFLOW_MAX_TEXT ) );	 		  
		   return true;	   
		}		
		
		g_RockClient.Send_RpCsPartyLeave(m_strParam);
		return true;
	}

//----------------------------------------------------------------------------------------------------
//		파티이름변경
//----------------------------------------------------------------------------------------------------	
	if( (Rstrcmp(m_strCmd, _RT("/pname") ) == 0) ||					
		(Rstrcmp(m_strCmd, _RT("/PNAME") ) == 0) )					
	{
		if(!g_PartyInforWnd.IsMemberParty())
		{
			SystemPrint( n_DarkGreen ,n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOT_BELONG_PARTY ) );	     		   	
		   return true;
		}

		if( Rstrcmp(m_strParam, _RT("") ) == 0 )
		{
			SystemPrint( n_DarkGreen ,n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_INPUT_PARTY_NAME ) );	 	   
		   return true;		
		}
		else
		{
		   if( g_Pc.m_MainPC.lUnique != pPartyInfo->m_dLeaderID )
		   {
			   SystemPrint( n_DarkGreen ,n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOT_AUTHORITY ) );	      			  
			  return true;	   
		   }
		}
#ifdef UNICODE_ROCK
		int iLen = Rstrlen(m_strParam)*2;
#else
		int iLen = Rstrlen(m_strParam);
#endif
	
		if( iLen > 20 )
		{
			SystemPrint( n_DarkGreen ,n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_OVERFLOW_MAX_TEXT ) );	  
		   return true;	   
		}

		g_RockClient.Send_RpCsPartyChangeName(m_strParam);
		return true;
	}

//----------------------------------------------------------------------------------------------------
//		파티리더변경
//----------------------------------------------------------------------------------------------------	
	if( (Rstrcmp(m_strCmd, _RT("/leader") ) == 0) ||					
		(Rstrcmp(m_strCmd, _RT("/LEADER") ) == 0) )					
	{
		if(!g_PartyInforWnd.IsMemberParty())
		{
			SystemPrint( n_DarkGreen ,n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOT_BELONG_PARTY ) );				 		 	 
		   return true;
		}

		if( Rstrcmp(m_strParam, _RT("") ) == 0 )
		{
			SystemPrint( n_DarkGreen ,n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_INPUT_NEW_MASTER ) );			   	   
		   return true;		
		}
		else
		{
		   if( g_Pc.m_MainPC.lUnique != pPartyInfo->m_dLeaderID )
		   {
			   SystemPrint( n_DarkGreen ,n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOT_AUTHORITY ) );				 	
			  return true;	   
		   }
		}
#ifdef UNICODE_ROCK
		int iLen = Rstrlen(m_strParam)*2;
#else
		int iLen = Rstrlen(m_strParam);
#endif
		if( iLen > (RP_MAX_PC_NAME - sizeof(RTCHAR)) )
		{
			SystemPrint( n_DarkGreen ,n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_OVERFLOW_MAX_TEXT ) );			  
		   return true;	   
		}

		if(Rstrcmp(PcParam->PCName, m_strParam) == 0)
		{
			SystemPrint( n_DarkGreen ,n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CNAT_OWN_NAME ) );			 	 
		   return true;
		}

		if(g_PartyInforWnd.FindPartyMember(m_strParam))
		{
		   g_RockClient.Send_RpCsChangeLeader(m_strParam); 
		}
		else
		{
			SystemPrint( n_DarkGreen ,n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOT_PARTY_MEMBER ));					
		   return true; 
		}

		return true;
	}

//----------------------------------------------------------------------------------------------------
//		파티 루팅방식변경
//----------------------------------------------------------------------------------------------------	
	if( (Rstrcmp(m_strCmd, _RT("/root") ) == 0) ||				
		(Rstrcmp(m_strCmd, _RT("/ROOT") ) == 0) )				
	{
		if(!g_PartyInforWnd.IsMemberParty())
		{
			SystemPrint( n_DarkGreen ,n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOT_BELONG_PARTY ) );				   
		   return true;
		}
		
		if( Rstrcmp(m_strParam, _RT("") ) == 0 )
		{
			SystemPrint( n_DarkGreen ,n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_SET_ITEM_SHARE ) );		 
		   return true;
		}
		else
		{
		   if( g_Pc.m_MainPC.lUnique != pPartyInfo->m_dLeaderID )
		   {
			   SystemPrint( n_DarkGreen ,n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOT_AUTHORITY ) );			  	   
			  return true;	   
		   }
		}
#ifdef UNICODE_ROCK
		int iLen = Rstrlen(m_strParam)*2;
#else
		int iLen = Rstrlen(m_strParam);
#endif
		if( iLen > 30 )
		{
			SystemPrint( n_DarkGreen ,n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_OVERFLOW_MAX_TEXT ) );		   	  
		   return true;	   
		}

		int iHowRoot = -1;

		if(Rstrcmp(m_strParam, G_STRING_CHAR( IDS_SYSPR_PT_FREE_SHARE ) ) == 0)			
		   iHowRoot = n_PartyItem_Free;

		if(Rstrcmp(m_strParam, G_STRING_CHAR( IDS_SYSPR_PT_SEQ_SHARE ) ) == 0)			
		   iHowRoot = n_PartyItem_Seq;

		if(iHowRoot == -1)
		{
			SystemPrint( n_DarkGreen ,n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_PT_SHARE_NOT ) );			   
		   return true;
		}
		else
		{
		   g_RockClient.Send_RpCsPartyItemMgr((BYTE)iHowRoot);
		}

		return true;
	}

//----------------------------------------------------------------------------------------------------
//		거래거절
//----------------------------------------------------------------------------------------------------		
	if( (Rstrcmp(m_strCmd, G_STRING_CHAR( IDS_CMD_EXCHANGE_REFUSE ) ) == 0) )			
		
	{
		g_RockClient.Send_RpCsExchange( (BYTE)n_OPEN );
		g_ExchangeWnd.m_bIsExchangeEnable = TRUE;

		return true;
	}

//----------------------------------------------------------------------------------------------------
//		거래
//----------------------------------------------------------------------------------------------------
	if( (Rstrcmp(m_strCmd, G_STRING_CHAR( IDS_CMD_EXCHANGE ) ) == 0) )					
	
	{
		g_RockClient.Send_RpCsExchange( (BYTE)n_CLOSE );	
		g_ExchangeWnd.m_bIsExchangeEnable = FALSE;

		return true;
	}
	
//----------------------------------------------------------------------------------------------------
//		길드초대
//----------------------------------------------------------------------------------------------------
	if( (Rstrcmp(m_strCmd, G_STRING_CHAR( IDS_SLASHINVITE_FRONTIER ) ) == 0) )		
	{
		TCHAR strTemp[256] = {0,};
		
		if( nRui->GetPcInvenInfo()->FrontierMyInfo == NULL )
			return true;

		//1차적으로 프론티어에 소속되어있는지
		if( ( nRui->GetPcInvenInfo()->FrontierMyInfo == NULL ) || 
			!( ( nRui->GetPcInvenInfo()->FrontierMyInfo->FrontierID != 0 ) && 
			  ( ( nRui->GetPcInvenInfo()->FrontierMyInfo->FrontierGrade == n_FT_Master ) || 
#ifdef DECO_RENEWAL_GUILD_MJH
				( nRui->GetPcInvenInfo()->FrontierMyInfo->FrontierGrade == n_FT_Assist ) ||
				( nRui->GetPcInvenInfo()->FrontierMyInfo->FrontierGrade == n_FT_Major ) )
#else
			  ( nRui->GetPcInvenInfo()->FrontierMyInfo->FrontierGrade == n_FT_Major ) )
#endif // DECO_RENEWAL_GUILD_MJH
			    
			  ) ) 
		{
			SystemPrint( n_DarkGreen ,n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOT_AUTHORITY ) );				 
		    return true;
		}				
		
		if( Rstrcmp(m_strParam, _RT("") ) == 0 )
		{
			SystemPrint( n_DarkGreen ,n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_PT_INPUT_PTMAN ) );				 
		    return true;
		}
#ifdef UNICODE_ROCK
		int iLen = Rstrlen(m_strParam)*2;
#else
		int iLen = Rstrlen(m_strParam);
#endif
		if( iLen > (RP_MAX_PC_NAME - sizeof(RTCHAR)) )
		{
			SystemPrint( n_DarkGreen ,n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_OVERFLOW_MAX_TEXT ) );	 			
			return true;	   
		}

		///-- 가시범위 내에 존재하는지 여부 확인
		PCTABLE *pPcTable = g_Pc.FindPcTable(m_strParam);
		if(!pPcTable)
		{
			Rsprintf(RWCHAR(strTemp), G_STRING_CHAR( IDS_SYSPR_PT_OUTOFVIEW ), RWCHAR(m_strParam));									
			return true;
		}

#ifdef FRONTIER	
		#ifdef DECO_RENEWAL_GUILD_MJH //가입제안
		    
		#else 
			g_RockClient.Send_CsMsngrProposeJoinFrontier((DWORD)pPcTable->lUnique);
		#endif // DECO_RENEWAL_GUILD_MJH
		
#endif 		
	
		return true;
	}


//----------------------------------------------------------------------------------------------------
//		엠블렌 전송
//----------------------------------------------------------------------------------------------------
	
	if( (Rstrcmp(m_strCmd, G_STRING_CHAR( IDS_SLASHSEND_EMBLEN ) ) == 0) )		
	{
	
#ifdef DECO_RENEWAL_GUILD_MJH

		//1차적으로 프론티어장인지를 먼저체크
		//1차적으로 프론티어에 소속되어있는지
		if( ( nRui->GetPcInvenInfo()->FrontierMyInfo == NULL ) || 
			( nRui->GetPcInvenInfo()->FrontierMyInfo->FrontierID == 0 ) || 
		    ( nRui->GetPcInvenInfo()->FrontierMyInfo->FrontierGrade != n_FT_Master ) )
		{
			SystemPrint( n_DarkGreen ,n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOT_AUTHORITY ) );				 
		    return true;
		}
		
		//2차적으로 예비 프론티어는 엠블렌을 등록할수 없다.
		if( ( nRui->GetPcInvenInfo()->FrontierInfo == NULL ) ||
			( n_Preliminary_Frontier == nRui->GetPcInvenInfo()->FrontierInfo->theStatus ) )
		{
			SystemPrint( n_DarkGreen ,n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_PRE_NOT_REG_EMBLEN ) );		 
		    return true;		
		}
		
		//3차적으로 지금 엠블렌이 등록되어있는지 여부를 체크한다.
		VESIONTEX *pVesionTex = 
			g_EmblemTexManager.FindForntierVersionTex(nRui->GetPcInvenInfo()->FrontierMyInfo->FrontierID);
		if(pVesionTex) //등록되어 있으면..
		{
			//인벤중에 아이템 코드가 11023이 존재하지 않으면 등록을 못하게 한다.
			bool bFindFlag = false;
			///--- 인벤창에서 스크롤 아이템중 부활관련 아이템을 찾아낸다.
			///-- 전체 인벤토리 Access
			for(int i = 0; i < c_MaxInvenItemNum; ++i )
			{
				if( nRui->GetPcInvenInfo()->InvenItem[i] == NULL )
				{
					continue;
				}

				if( nRui->GetPcInvenInfo()->InvenItem[i]->ItemTable == NULL )
				{
					continue;
				}	

				if( 11023 == nRui->GetPcInvenInfo()->InvenItem[i]->ItemTable->theCode)
				{
					bFindFlag = true;
					break;
				}		
			}			
			
			if(!bFindFlag)
			{
				SItemBaseInfo* item_Info;
				item_Info = g_Pc_Manager.GetItemBaseInfoPtr( 11023 ); 
				if( item_Info == NULL )
					return true;
							
				SystemPrint( n_DarkGreen ,n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NEED_THEITEM ), 
							item_Info->theName );
				return true;
			}					
		}		
		SItemBaseInfo* item_Info;
		item_Info = g_Pc_Manager.GetItemBaseInfoPtr( 24002 ); 

/*	
		if( Rstrcmp(m_strParam, _RT("") ) == 0 )
		{
			SystemPrint( n_DarkGreen ,n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_INPUT_FILE_NAME ) );				 
		    return true;
		}
		
		char Temp[512]	= {0,};
		FILE * infile = NULL;
#ifdef UNICODE_ROCK
		Rsprintf(RWCHAR(Temp), _RT("%s\\%s"), g_RockClient.GetRootDirW(), RWCHAR(m_strParam) );	
		if ((infile = _wfopen(RWCHAR(Temp), _RT("rb"))) == NULL) 
#else 
		sprintf(Temp, "%s\\%s", g_RockClient.GetRootDir(), m_strParam );	
		if ((infile = fopen(Temp, "rb")) == NULL) 
#endif 

		{
			SystemPrint( n_DarkGreen ,n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOFILE_INDESTPOLDER ) );		
			return true;
		}	
		
		fseek(infile, 0, SEEK_END);
		WORD wEmblemSize = ftell(infile);
		fseek(infile, 0, SEEK_SET);
		
		//파일사이즈 4096Byte이상 크기를 못만들게 한다
		if( wEmblemSize >= RP_MAX_EMBLEM_SIZE )
		{
			TCHAR strBuf[256] = {0,};
			Rsprintf(RWCHAR(strBuf),G_STRING_CHAR( IDS_SYSPR_APPLY_THESIZE ), RP_MAX_EMBLEM_SIZE);
			SystemPrint( n_DarkGreen ,n_SystemMsg, strBuf );				 
		    return true;
		}
		
		memset( g_cEmblem, 0, RP_MAX_EMBLEM_SIZE );

		fread( ( void * )g_cEmblem, wEmblemSize, 1, infile);		
		fclose(infile);
		
		D3DXIMAGE_INFO imageinfo;

		if( FAILED( D3DXGetImageInfoFromFileInMemory( g_cEmblem, wEmblemSize, &imageinfo ) ) )
		{       
			char strError[255] = {0, };
			Rsprintf(RWCHAR(strError), G_STRING_CHAR( IDS_SYSPR_NOACEEPTIMAGE ) );
			SystemPrint( n_DarkGreen ,n_SystemMsg, strError );		
			return true;
		}

		//파일 사이즈를 비교
		if( imageinfo.Width != 32 && imageinfo.Height != 32 )
		{
			SystemPrint( n_DarkGreen ,n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_ONLY_3232SIZE ) );				 
		    return true;
		}
		
		if( imageinfo.Format != D3DFMT_R8G8B8 && imageinfo.Format != D3DFMT_A8R8G8B8 && 
			imageinfo.Format != D3DFMT_X8R8G8B8 )
		{
			SystemPrint( n_DarkGreen ,n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_24AND32MUST_IMAGE ) );				 
		    return true;
		}
*/
#ifdef FRONTIER				
		g_RockClient.Send_CsMsngrUploadEmblem( item_Info->theSoundFile );			
#endif 	

#else	
	
		//1차적으로 프론티어장인지를 먼저체크
		//1차적으로 프론티어에 소속되어있는지
		if( ( nRui->GetPcInvenInfo()->FrontierMyInfo == NULL ) || 
			( nRui->GetPcInvenInfo()->FrontierMyInfo->FrontierID == 0 ) || 
		    ( nRui->GetPcInvenInfo()->FrontierMyInfo->FrontierGrade != n_FT_Master ) )
		{
			SystemPrint( n_DarkGreen ,n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOT_AUTHORITY ) );				 
		    return true;
		}
		
		//2차적으로 예비 프론티어는 엠블렌을 등록할수 없다.
		if( ( nRui->GetPcInvenInfo()->FrontierInfo == NULL ) ||
			( n_Preliminary_Frontier == nRui->GetPcInvenInfo()->FrontierInfo->theStatus ) )
		{
			SystemPrint( n_DarkGreen ,n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_PRE_NOT_REG_EMBLEN ) );		 
		    return true;		
		}
		
		//3차적으로 지금 엠블렌이 등록되어있는지 여부를 체크한다.
		VESIONTEX *pVesionTex = 
			g_EmblemTexManager.FindForntierVersionTex(nRui->GetPcInvenInfo()->FrontierMyInfo->FrontierID);
		if(pVesionTex) //등록되어 있으면..
		{
			//인벤중에 아이템 코드가 11023이 존재하지 않으면 등록을 못하게 한다.
			bool bFindFlag = false;
			///--- 인벤창에서 스크롤 아이템중 부활관련 아이템을 찾아낸다.
			///-- 전체 인벤토리 Access
			for(int i = 0; i < c_MaxInvenItemNum; ++i )
			{
				if( nRui->GetPcInvenInfo()->InvenItem[i] == NULL )
				{
					continue;
				}

				if( nRui->GetPcInvenInfo()->InvenItem[i]->ItemTable == NULL )
				{
					continue;
				}	

				if( 11023 == nRui->GetPcInvenInfo()->InvenItem[i]->ItemTable->theCode)
				{
					bFindFlag = true;
					break;
				}		
			}			
			
			if(!bFindFlag)
			{
				SItemBaseInfo* item_Info;
				item_Info = g_Pc_Manager.GetItemBaseInfoPtr( 11023 ); 
				if( item_Info == NULL )
					return true;
							
				SystemPrint( n_DarkGreen ,n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NEED_THEITEM ), 
							item_Info->theName );
				return true;
			}					
		}		

		
		if( Rstrcmp(m_strParam, _RT("") ) == 0 )
		{
			SystemPrint( n_DarkGreen ,n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_INPUT_FILE_NAME ) );				 
		    return true;
		}
		
		char Temp[512]	= {0,};
		FILE * infile = NULL;
#ifdef UNICODE_ROCK
		Rsprintf(RWCHAR(Temp), _RT("%s\\%s"), g_RockClient.GetRootDirW(), RWCHAR(m_strParam) );	
		if ((infile = _wfopen(RWCHAR(Temp), _RT("rb"))) == NULL) 
#else 
		sprintf(Temp, "%s\\%s", g_RockClient.GetRootDir(), m_strParam );	
		if ((infile = fopen(Temp, "rb")) == NULL) 
#endif 

		{
			SystemPrint( n_DarkGreen ,n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOFILE_INDESTPOLDER ) );		
			return true;
		}	
		
		fseek(infile, 0, SEEK_END);
		WORD wEmblemSize = ftell(infile);
		fseek(infile, 0, SEEK_SET);
		
		//파일사이즈 4096Byte이상 크기를 못만들게 한다
		if( wEmblemSize >= RP_MAX_EMBLEM_SIZE )
		{
			TCHAR strBuf[256] = {0,};
			Rsprintf(RWCHAR(strBuf),G_STRING_CHAR( IDS_SYSPR_APPLY_THESIZE ), RP_MAX_EMBLEM_SIZE);
			SystemPrint( n_DarkGreen ,n_SystemMsg, strBuf );				 
		    return true;
		}
		
		memset( g_cEmblem, 0, RP_MAX_EMBLEM_SIZE );

		fread( ( void * )g_cEmblem, wEmblemSize, 1, infile);		
		fclose(infile);
		
		D3DXIMAGE_INFO imageinfo;

		if( FAILED( D3DXGetImageInfoFromFileInMemory( g_cEmblem, wEmblemSize, &imageinfo ) ) )
		{       
			char strError[255] = {0, };
			Rsprintf(RWCHAR(strError), G_STRING_CHAR( IDS_SYSPR_NOACEEPTIMAGE ) );
			SystemPrint( n_DarkGreen ,n_SystemMsg, strError );		
			return true;
		}

		//파일 사이즈를 비교
		if( imageinfo.Width != 32 && imageinfo.Height != 32 )
		{
			SystemPrint( n_DarkGreen ,n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_ONLY_3232SIZE ) );				 
		    return true;
		}
		
		if( imageinfo.Format != D3DFMT_R8G8B8 && imageinfo.Format != D3DFMT_A8R8G8B8 && 
			imageinfo.Format != D3DFMT_X8R8G8B8 )
		{
			SystemPrint( n_DarkGreen ,n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_24AND32MUST_IMAGE ) );				 
		    return true;
		}

#ifdef FRONTIER				
		g_RockClient.Send_CsMsngrUploadEmblem( wEmblemSize, g_cEmblem );			
#endif 
		
#endif // DECO_RENEWAL_GUILD_MJH	
		return true;
	}

	//스페이스 막기
#ifdef UNICODE_ROCK
	WCHAR *pCmd = (WCHAR *)m_strCmd;
#else 
	char *pCmd = (char *)m_strCmd;
#endif 
	
	if(pCmd)
	{
		if(pCmd[0] == _RT('/'))
		{
			SystemPrint( n_DarkGreen ,n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOTACCEPT_S_COMMAND ) );
			return true;
		}
	}

	return false;
}

void CChatActPrintWnd::ExcuteInvitePatry(TCHAR *strParam, bool bLeaueFlag)
{
	SPARTY_INFO *pPartyInfo = g_PartyInforWnd.GetPartyInfo();
	SPcDataParam* PcParam = nRui->GetPcParamInfo();	
	
	SAFE_STR_CPY( m_strParam, strParam, 256 );
	
	if( Rstrcmp(PcParam->PCName, m_strParam) == 0 )
	{
		SystemPrint( n_DarkGreen ,n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_PT_CANT_INVITE_ME ) );		
		return;
	}
	
	if( g_PartyInforWnd.IsMemberParty()	&&
	( g_Pc.m_MainPC.lUnique != pPartyInfo->m_dLeaderID ) )
	{
		SystemPrint( n_DarkGreen ,n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_PT_MASTER_INVITE ) );			
		return;
	}

	if(g_PartyInforWnd.IsMemberParty())
	{
		if(NORMAL_PARTY == g_PartyInforWnd.GetPartyType())
		{	
			bLeaueFlag = false;
			
			int iTotPMemNum = g_PartyInforWnd.GetTotPartyMemNumber();
			if( iTotPMemNum >= MAX_PARTY_MEMBERNUM )
			{
				SystemPrint( n_DarkGreen ,n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_PT_INVITE_MAX ) );					
				return;	
			}
		}
		else
		{
			bLeaueFlag = true;
			
			//신수를 가지고있는지 체크
			/*
				if(Rstrcmp(nRui->thePcParam.PCName, nRui->GetPcInvenInfo()->FrontierCaller) != 0 )
				{
					SystemPrint( n_DarkGreen ,n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_ONLY_HOLLCALLER) );		
					return;
				}			
			*/

			int iTotPMemNum = g_LeagueInfoWnd.GetLeagueMemSize();
			if( iTotPMemNum >= MAX_LEAGUE_MEMBER_NUM - 1 )
			{
				SystemPrint( n_DarkGreen ,n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_PT_INVITE_MAX ) );					
				return;	
			}	
		}
	}
#ifdef UNICODE_ROCK
	int iLen = Rstrlen(m_strParam)*2;
#else
	int iLen = Rstrlen(m_strParam);
#endif
	if( iLen > (RP_MAX_PC_NAME - sizeof(RTCHAR)) )
	{
		SystemPrint( n_DarkGreen ,n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_OVERFLOW_MAX_TEXT ) );	 			
		return;	   
	}
	
	if(bLeaueFlag)
	{
		g_RockClient.Send_RpCsPartyAskJoin(m_strParam, n_LEAGUE);
	}
	else
	{
		g_RockClient.Send_RpCsPartyAskJoin(m_strParam, n_PARTY);
	}
}

void CChatActPrintWnd::ChattingProc()
{
	bool bCoverFlag = false;						//도배방지 플래그
	
	if(  SAFE_TIME_COMPARE( g_nowTime , < , SAFE_TIME_ADD( m_lPrevTime , 1200 ) )  )	//1.2초 이전에 입력		
	{
		bCoverFlag = true;
	}
	
	// 필터링
	if( g_RockClient.m_SlangFilter.Filter( ( char * ) m_strChatstring ) )
	{
		memset( m_strChatstring , 0 , sizeof(TCHAR) * 256 );
		Rsprintf( RWCHAR(m_strChatstring) , _RT("%s") , RWCHAR(g_RockClient.m_SlangFilter.GetString()) );
	}

	if( Rstrlen( m_strChatstring ) != 0 )
	{	
		switch(m_uChatState)
		{
			case n_NormarChat:
				{
					//여기서 채팅데이터 채팅창에 추가										
					if(!bCoverFlag)
					{
						/*
						g_ChatMainProc.InsertChatContent(n_NormarChat, nRui->thePcParam.PCName,
													 m_strChatstring);					

						if( g_Pc.GetPlayer()->m_curt_event.status != SM_SELLSKILL_SITDOWN && 
							g_Pc.GetPlayer()->m_curt_event.status != SM_SELLSKILL_KEEPING &&
							g_Pc.GetPlayer()->m_curt_event.status != SM_SELLSKILL_STANDUP &&
							g_Pc.GetPlayer()->IsMyPlayerRender )
						{
							g_RockClient.SetPlayerMessage( m_strChatstring );
						}
						*/
						g_RockClient.Send_RpCsSay( m_strChatstring );

					}
					else
					{
						g_ChatMainProc.InsertChatContent(n_NormarChat, nRui->thePcParam.PCName,
													 RWCHAR(G_STRING_CHAR( IDS_COVER_PREVENT_APPLY )) );												
					}
				}
				break;
			case n_WhisperChat:
				{
					if(!bCoverFlag)
					{
						nRui->SendEvent( m_pInputFWhisDestBox->GetID(), n_emGetText, 
						(EPARAM)m_strBWhisDest, 256, 0, 0 );
						SetWhisDest(m_strBWhisDest);							
#ifdef	NETWORK_CLIENT


						if(g_MCommunityWnd.m_MBlockTabWnd.ExistBlockName(m_strFWhisDest))
						{
							if(!g_RockClient.m_bIsDrawFullScreenUI)
							{
								SystemPrint( n_DarkGreen ,n_SystemMsg,G_STRING_CHAR(IDS_SYSPR_INTERCEPTION_NOT_WHISPER));	
							}
							else
							{
								if(g_BZ_RoomListWnd.IsVisible)
								{
									g_BZ_RoomListWnd.InsertBZHelpChatMsg(G_STRING_CHAR(IDS_SYSPR_INTERCEPTION_NOT_WHISPER));
								}
								else if(g_BZ_WaitingRoomWnd.IsVisible)
								{
									g_BZ_WaitingRoomWnd.InsertBZHelpChatMsg(G_STRING_CHAR(IDS_SYSPR_INTERCEPTION_NOT_WHISPER));
								}
							}
						}
						else
						{
							g_RockClient.Send_RpCsWhisper(m_strFWhisDest , m_strChatstring);
						}
						
#endif
					}		
				}
				break;
			case n_PartyChat:
				{
					if(!g_PartyInforWnd.IsMemberParty())
					{
						SystemPrint(n_DarkGreen ,n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOT_BELONG_PARTY ) );							
						return;
					}
				   
				   if(!bCoverFlag)
				   {
#ifdef	NETWORK_CLIENT					
						g_RockClient.Send_RpCsPartyChat( m_strChatstring );
#endif			
				   }
				}
				break;
			case n_GuildChat:
				{
					///-- 프론티어에 속하지 않으면 보내지않음
#ifdef FRONTIER						
				   if( ( nRui->GetPcInvenInfo()->FrontierMyInfo == NULL ) || 
					( nRui->GetPcInvenInfo()->FrontierMyInfo->FrontierID == 0 ) )
				   {
						SystemPrint(n_DarkGreen ,n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOT_IN_FRONTIER ) );	
						return;
				   }
				 
				   if(!bCoverFlag)
				   {
						g_RockClient.Send_CsMsngrFrontierSay( m_strChatstring );
				   }
#endif		
				}
				break;
			case n_WholeServerChat:
				{
				   if(!bCoverFlag)
				   {						
						if(m_dUseShoutCode)
						{
							g_RockClient.Send_CsShout( m_dUseShoutCode, m_strChatstring );
						}
						else
						{
#ifdef MEGAPHONE_RENEW
							Send_CsShout(n_Scroll_AllChannel_Shout);
#endif 
						}
						m_dUseShoutCode = 0;
				   }
				}
				break;
			case n_WholeChannelChat:
				{
				   if(!bCoverFlag)
				   {
						if(m_dUseShoutCode)
						{
							g_RockClient.Send_CsShout( m_dUseShoutCode, m_strChatstring );
						}
						else
						{
#ifdef MEGAPHONE_RENEW
							Send_CsShout(n_Scroll_Channel_Shout);
#endif 
						}
						m_dUseShoutCode = 0;
				   }
				   break;
				}
			case n_WholeAreaChat:
				{

					if(!bCoverFlag)
					{
#ifdef TEST_AREA_CHATING
						g_ChatMainProc.InsertChatContent( n_WholeAreaChat, nRui->GetPcParamInfo()->PCName , m_strChatstring);
#else
						
						if(m_dUseShoutCode)
						{
							g_RockClient.Send_CsShout( m_dUseShoutCode, m_strChatstring );
						}
						else
						{
#ifdef MEGAPHONE_RENEW
							Send_CsShout(n_Scroll_Map_Shout);
#endif 
						}
						m_dUseShoutCode = 0;
					
#endif
						
					
					}


				}
				break;
		}
	}

	m_lPrevTime = g_nowTime;
	
}

bool CChatActPrintWnd::Send_CsShout(NScrollType ScrollType)
{
#ifdef MEGAPHONE_RENEW
	//전체 갱신
#endif 

	bool bFindFlag = false;
	int  nFindIndex = -1;
	///-- 전체 인벤토리 Access
	for(int i = 0; i < c_MaxInvenItemNum; ++i )
	{
		if( nRui->GetPcInvenInfo()->InvenItem[i] == NULL )
		{
			continue;
		}
				
		if( (ScrollType == nRui->GetPcInvenInfo()->InvenItem[i]->ItemTable->theReHP) &&
			(n_Type_Scroll== nRui->GetPcInvenInfo()->InvenItem[i]->ItemTable->theType ) )
		{
			bFindFlag = true;
			nFindIndex = i;
			break;
		}		
	}


	if(!bFindFlag)
	{
		
		SItemBaseInfo* item_Info;
		item_Info = g_Pc_Manager.GetItemBaseInfoPtr( 11026 ); 
		if( item_Info == NULL )
			return FALSE;
		/*
		bool bsysPrint = false; 

		//체크한다.  채널1 메가폰이 없을경우 메세지 출력 처리 부분 .. 
		//11026 없고 14937 이 있으면 메세지 출력을 하지 말아라 .. 
		if(wCode == 11026)
		{
			for(i = 0; i < c_MaxInvenItemNum; ++i )
			{
				if( nRui->GetPcInvenInfo()->InvenItem[i] == NULL )
				{
					continue;
				}
							
				if( 14937 == nRui->GetPcInvenInfo()->InvenItem[i]->ItemTable->theCode)
				{
					return FALSE;
					break; 
				}		
			}
		}

		if(wCode == 14937 )	return FALSE; // 같은종류 채널 메가폰은 두개야 ㅠㅠ 메세지 출력 중복 방지를 위해 
		
		*/
		SystemPrint( n_DarkGreen ,n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NEED_THEITEM ), 
					item_Info->theName );
		
		//메가폰이 없을경우 일반챗으로 돌린다. 
		SetChatState(n_NormarChat);

		return TRUE;

	}
	else
	{
		g_RockClient.Send_CsShout( nRui->GetPcInvenInfo()->InvenItem[nFindIndex]->ItemUID, 
								   m_strChatstring );
		return TRUE;
	}

}

void CChatActPrintWnd::SetSizeState(UINT uState)
{
	if( (m_uSizeState == uState) || (uState == ChatWndSizeNone) )
	{
		return;
	}
	
	m_uSizeState = uState;

	SRect ActChatBgWndRect;
	SRect FilterTabWndRect;
	SRect ChatPrintWndRect;
	SRect ChatInputWndRect;
	SRect ChatStateMenuWndRect;	

	IPOS2 FilterTabWndRPos;
	IPOS2 ChatPrintWndRPos;	
	IPOS2 ChatStateMenuWndRPos;	

	m_pActChatBgWnd->GetClientSize(&ActChatBgWndRect);
	m_pFilterTabWnd->GetClientSize(&FilterTabWndRect);
	m_pChatPrintWnd->GetClientSize(&ChatPrintWndRect);	
	m_pChatStateMenuWnd->GetClientSize(&ChatStateMenuWndRect);


	m_pChatInputWnd->GetClientSize(&ChatInputWndRect);
	
	
	m_pFilterTabWnd->GetWndPosition(FilterTabWndRPos);
	m_pChatPrintWnd->GetWndPosition(ChatPrintWndRPos);	
	m_pChatStateMenuWnd->GetWndPosition(ChatStateMenuWndRPos);	
	
	SRect OutListRect;	
	SRect ScrollBarRect;
	SRect ScrollBarBgRect;
	SRect TitleTexBoxRect;
	
	IPOS2 OutListRPos;
	IPOS2 ScrollBarRPos;
	IPOS2 ScrollBarBgRPos;
	IPOS2 TitleTexBoxRPos;
	IPOS2 InputFWhisDestBoxRPos;
	IPOS2 ChatSizeContBtnRPos;
	
	m_pChatOutList->SetOutputLevel(m_uSizeState);
	m_pChatOutList->GetClientSize(&OutListRect);
	m_pChatOutList->GetWndPosition(OutListRPos);
	
	m_pChatScrollBar->GetClientSize(&ScrollBarRect);
	m_pChatScrollBar->GetWndPosition(ScrollBarRPos);

	m_pChatScrollBarBg->GetClientSize(&ScrollBarBgRect);
	m_pChatScrollBarBg->GetWndPosition(ScrollBarBgRPos);

	m_pTitleTexBox->GetClientSize(&TitleTexBoxRect);
	m_pTitleTexBox->GetWndPosition(TitleTexBoxRPos);
	
	m_pInputFWhisDestBox->GetWndPosition(InputFWhisDestBoxRPos);
	m_pChatSizeContBtn->GetWndPosition(ChatSizeContBtnRPos);
	
	ChatPrintWndRect.h = OutListRect.h;
	ChatPrintWndRect.h += TitleTexBoxRect.h;	
	
	m_pChatPrintWnd->ResetSize(&ChatPrintWndRect);
	
	ScrollBarBgRect.h = OutListRect.h;
	m_pChatScrollBarBg->ResetSize(&ScrollBarBgRect);

	ScrollBarRect.h = OutListRect.h;
	m_pChatScrollBar->ResetSize(&ScrollBarRect);

	int nLastValue = ( MAX_CHATDATA_NUM - m_pChatOutList->GetOutPutLine() );
	m_pChatScrollBar->ReSetScroll(0, nLastValue, nLastValue);

	m_pChatScrollBarBg->SetWndRepositionR(ScrollBarBgRPos[0], 0);
	m_pChatScrollBar->SetWndRepositionR(ScrollBarRPos[0], 0);
	m_pChatOutList->SetWndRepositionR(OutListRPos[0], 0);
	m_pTitleTexBox->SetWndRepositionR(TitleTexBoxRPos[0], OutListRect.h);
	m_pInputFWhisDestBox->SetWndRepositionR(TitleTexBoxRPos[0] + 2, OutListRect.h + 4);	
	m_pChatSizeContBtn->SetWndRepositionR(TitleTexBoxRPos[0] + 363, OutListRect.h + 4);

	
	ActChatBgWndRect.h = FilterTabWndRect.h + ChatPrintWndRect.h;
	ActChatBgWndRect.y = m_nLeftDownPos.y - ChatInputWndRect.h - ActChatBgWndRect.h;
	
	m_pActChatBgWnd->ResetSize(&ActChatBgWndRect);
	m_pFilterTabWnd->SetWndRepositionR(FilterTabWndRPos[0], 0);
	m_pChatPrintWnd->SetWndRepositionR(ChatPrintWndRPos[0], FilterTabWndRect.h);
	m_pChatStateMenuWnd->SetWndRepositionR(ChatStateMenuWndRPos[0], 
										  FilterTabWndRect.h + ChatPrintWndRect.h - 1 - ChatStateMenuWndRect.h);

	
}

void CChatActPrintWnd::SetFilterTabState(UINT uState)
{
	if( (m_uFilterState == uState) || (uState == COutNoneView) )
	{
		return;
	}

	m_uFilterState = uState;

	m_pWholeViewTab->SetTextColor(200, 200, 200);
	m_pPartyViewTab->SetTextColor(200, 200, 200);
	m_pGuildViewTab->SetTextColor(200, 200, 200);
	m_pAnnounViewTab->SetTextColor(200, 200, 200);
	m_pHelpViewTab->SetTextColor(200, 200, 200);

	switch(m_uFilterState)
	{
		case COutTotalView:
			{
				m_pWholeViewTab->SetTextColor(255, 255, 255);
				nRui->SendEvent( m_pWholeViewTab->GetID(), n_emSetChecked, 0, 0, 0, 0 );
				m_pChatOutList->SetFilterState(COutTotalView);
				SetChatState(n_NormarChat);

				g_ChatMainProc.m_ChatNonActPrintWnd.GetChatOutList()->SetFilterState(COutTotalView);
				g_ChatMainProc.UpdateViewChatTxtData();
			}
			break;
		case COutPartyView:
			{
				m_pPartyViewTab->SetTextColor(255, 255, 255);
				nRui->SendEvent( m_pPartyViewTab->GetID(), n_emSetChecked, 0, 0, 0, 0 );
				m_pChatOutList->SetFilterState(COutPartyView);
				SetChatState(n_PartyChat);

				g_ChatMainProc.m_ChatNonActPrintWnd.GetChatOutList()->SetFilterState(COutPartyView);
				g_ChatMainProc.UpdateViewChatTxtData();
			}
			break;
		case COutGuildView:
			{
				m_pGuildViewTab->SetTextColor(255, 255, 255);
				nRui->SendEvent( m_pGuildViewTab->GetID(), n_emSetChecked, 0, 0, 0, 0 );
				m_pChatOutList->SetFilterState(COutGuildView);
				SetChatState(n_GuildChat);

				g_ChatMainProc.m_ChatNonActPrintWnd.GetChatOutList()->SetFilterState(COutGuildView);
				g_ChatMainProc.UpdateViewChatTxtData();
			}
			break;
		case COutAnnounView:
			{
				m_pAnnounViewTab->SetTextColor(255, 255, 255);
				nRui->SendEvent( m_pAnnounViewTab->GetID(), n_emSetChecked, 0, 0, 0, 0 );
				m_pChatOutList->SetFilterState(COutAnnounView);			

				g_ChatMainProc.m_ChatNonActPrintWnd.GetChatOutList()->SetFilterState(COutAnnounView);
				g_ChatMainProc.UpdateViewChatTxtData();
			}
			break;
		case COutHelpView:
			{
				m_pHelpViewTab->SetTextColor(255, 255, 255);
				nRui->SendEvent( m_pHelpViewTab->GetID(), n_emSetChecked, 0, 0, 0, 0 );
				m_pChatOutList->SetFilterState(COutHelpView);			

				g_ChatMainProc.m_ChatNonActPrintWnd.GetChatOutList()->SetFilterState(COutHelpView);	
				g_ChatMainProc.UpdateViewChatTxtData();				
			}
			break;
	}

	ResetScroll();	
}

void CChatActPrintWnd::SetChatState(UINT uState)
{
	m_uChatState = uState;
	m_pInputEditBox->SetText( _RT("") );
	m_pTitleTexBox->SetBlank( 6, 0, 4, 0 );
	m_pInputFWhisDestBox->SetVisible(false);
	
	switch(m_uChatState)
	{
		case n_NormarChat:
			{
				m_pChatStateMenuViewBtn->SetClientImage( TID_NORMALCHAT_NORMAL );
				m_pChatStateMenuViewBtn->SetBtnImage( TID_None, TID_NORMALCHAT_FOCUS, 
							   TID_NORMALCHAT_CLICK, TID_None );
				m_pChatStateMenuViewBtn->SetHID(HD_MNORMAL_CHAT);				

				m_pNormalChatBtn->SetClientImage( TID_NORMALCHAT_NORMAL );
				m_pGuildChatBtn->SetClientImage( TID_GUILDCHAT_CLICK );
				m_pPartyChatBtn->SetClientImage( TID_PARTYCHAT_CLICK );
				m_pWhisperChatBtn->SetClientImage( TID_WHISCHAT_CLICK );
				m_pWholeChannelBtn->SetClientImage( TID_CHANNELC_CLICK );
				m_pWholeServerBtn->SetClientImage( TID_SERVERC_CLICK );
				m_pWholeAreaBtn->SetClientImage( TID_AREA_BRODCAST_CLICK );
				//------------------------------------------------------------------
				m_pInputEditBox->SetTextColor(255, 255, 255);
				m_pTitleTexBox->SetTextColor(255, 255, 255);
				Rsprintf( RWCHAR(m_strTitle), _RT("%s :"), G_STRING_CHAR( IDS_WND_NORMAL_CHAT ) );					
				m_pTitleTexBox->SetText(m_strTitle);
			}
			break;
		case n_WhisperChat:
			{
				m_pChatStateMenuViewBtn->SetClientImage( TID_WHISCHAT_NORMAL );
				m_pChatStateMenuViewBtn->SetBtnImage( TID_None, TID_WHISCHAT_FOCUS, 
							   TID_WHISCHAT_CLICK, TID_None );
				m_pChatStateMenuViewBtn->SetHID(HD_MWHISPER_CHAT);

				m_pNormalChatBtn->SetClientImage( TID_NORMALCHAT_CLICK );
				m_pGuildChatBtn->SetClientImage( TID_GUILDCHAT_CLICK );
				m_pPartyChatBtn->SetClientImage( TID_PARTYCHAT_CLICK );
				m_pWhisperChatBtn->SetClientImage( TID_WHISCHAT_NORMAL );
				m_pWholeChannelBtn->SetClientImage( TID_CHANNELC_CLICK );
				m_pWholeServerBtn->SetClientImage( TID_SERVERC_CLICK );
				m_pWholeAreaBtn->SetClientImage( TID_AREA_BRODCAST_CLICK );
		
				m_pTitleTexBox->SetBlank( 116, 0, 4, 0 );

				//------------------------------------------------------------------
				m_pInputEditBox->SetTextColor(255, 175, 255 );
				m_pTitleTexBox->SetTextColor(255, 175, 255 );		

				Rsprintf(RWCHAR(m_strTitle), _RT("%s :"), G_STRING_CHAR( IDS_WND_WHISPER_TO ) );								
				m_pTitleTexBox->SetText(m_strTitle);
				
				m_pInputFWhisDestBox->SetVisible(true);
				m_pInputFWhisDestBox->SetText(m_strFWhisDest);
				nRui->SendEvent( m_pInputFWhisDestBox->GetID(), n_emSetFocus, 0, 0, 0, 0 );
			}
			break;
		case n_PartyChat:
			{
				m_pChatStateMenuViewBtn->SetClientImage( TID_PARTYCHAT_NORMAL );
				m_pChatStateMenuViewBtn->SetBtnImage( TID_None, TID_PARTYCHAT_FOCUS, 
							   TID_PARTYCHAT_CLICK, TID_None );
				m_pChatStateMenuViewBtn->SetHID(HD_MPARTY_CHAT);

				m_pNormalChatBtn->SetClientImage( TID_NORMALCHAT_CLICK );
				m_pGuildChatBtn->SetClientImage( TID_GUILDCHAT_CLICK );
				m_pPartyChatBtn->SetClientImage( TID_PARTYCHAT_NORMAL );
				m_pWhisperChatBtn->SetClientImage( TID_WHISCHAT_CLICK );
				m_pWholeChannelBtn->SetClientImage( TID_CHANNELC_CLICK );
				m_pWholeServerBtn->SetClientImage( TID_SERVERC_CLICK );

				m_pWholeAreaBtn->SetClientImage( TID_AREA_BRODCAST_CLICK );
				//------------------------------------------------------------------
				m_pInputEditBox->SetTextColor(122, 255, 255);
				m_pTitleTexBox->SetTextColor(122, 255, 255);
				Rsprintf(RWCHAR(m_strTitle), _RT("%s :"), G_STRING_CHAR( IDS_WND_PARTY_CHAT ) );					
				m_pTitleTexBox->SetText(m_strTitle);
			}
			break;
		case n_GuildChat:
			{
				m_pChatStateMenuViewBtn->SetClientImage( TID_GUILDCHAT_NORMAL );
				m_pChatStateMenuViewBtn->SetBtnImage( TID_None, TID_GUILDCHAT_FOCUS, 
							   TID_GUILDCHAT_CLICK, TID_None );
				m_pChatStateMenuViewBtn->SetHID(HD_MGUILD_CHAT);

				m_pNormalChatBtn->SetClientImage( TID_NORMALCHAT_CLICK );
				m_pGuildChatBtn->SetClientImage( TID_GUILDCHAT_NORMAL );
				m_pPartyChatBtn->SetClientImage( TID_PARTYCHAT_CLICK );
				m_pWhisperChatBtn->SetClientImage( TID_WHISCHAT_CLICK );
				m_pWholeChannelBtn->SetClientImage( TID_CHANNELC_CLICK );
				m_pWholeServerBtn->SetClientImage( TID_SERVERC_CLICK );
				m_pWholeAreaBtn->SetClientImage( TID_AREA_BRODCAST_CLICK );
			
				//------------------------------------------------------------------
				m_pInputEditBox->SetTextColor(122, 255, 122 );
				m_pTitleTexBox->SetTextColor(122, 255, 122 );
				Rsprintf(RWCHAR(m_strTitle), _RT("%s :"), G_STRING_CHAR( IDS_WND_GUILD_CHAT ) );					
				m_pTitleTexBox->SetText(m_strTitle);
			}
			break;
		case n_WholeChannelChat:
			{
				m_pChatStateMenuViewBtn->SetClientImage( TID_CHANNELC_NORMAL );
				m_pChatStateMenuViewBtn->SetBtnImage( TID_None, TID_CHANNELC_FOCUS, 
							   TID_CHANNELC_CLICK, TID_None );
				m_pChatStateMenuViewBtn->SetHID(HD_MTCHANNELC_BTN);
				
				m_pNormalChatBtn->SetClientImage( TID_NORMALCHAT_CLICK );
				m_pGuildChatBtn->SetClientImage( TID_GUILDCHAT_CLICK );
				m_pPartyChatBtn->SetClientImage( TID_PARTYCHAT_CLICK );
				m_pWhisperChatBtn->SetClientImage( TID_WHISCHAT_CLICK );
				m_pWholeChannelBtn->SetClientImage( TID_CHANNELC_NORMAL );
				m_pWholeServerBtn->SetClientImage( TID_SERVERC_CLICK );
				m_pWholeAreaBtn->SetClientImage( TID_AREA_BRODCAST_CLICK );
				//------------------------------------------------------------------
				m_pInputEditBox->SetTextColor(197, 231, 255 );
				m_pTitleTexBox->SetTextColor(197, 231, 255 );
				Rsprintf(RWCHAR(m_strTitle), _RT("%s :"), G_STRING_CHAR( IDS_TOTAL_CHANNEL_CHAT ) );							
				m_pTitleTexBox->SetText(m_strTitle);
			}
			break;
		case n_WholeServerChat:
			{
				m_pChatStateMenuViewBtn->SetClientImage( TID_SERVERC_NORMAL );
				m_pChatStateMenuViewBtn->SetBtnImage( TID_None, TID_SERVERC_FOCUS, 
							   TID_SERVERC_CLICK, TID_None );
				m_pChatStateMenuViewBtn->SetHID(HD_MTSETVERC_BTN);

				m_pNormalChatBtn->SetClientImage( TID_NORMALCHAT_CLICK );
				m_pGuildChatBtn->SetClientImage( TID_GUILDCHAT_CLICK );
				m_pPartyChatBtn->SetClientImage( TID_PARTYCHAT_CLICK );
				m_pWhisperChatBtn->SetClientImage( TID_WHISCHAT_CLICK );
				m_pWholeChannelBtn->SetClientImage( TID_CHANNELC_CLICK );
				m_pWholeServerBtn->SetClientImage( TID_SERVERC_NORMAL );
				m_pWholeAreaBtn->SetClientImage( TID_AREA_BRODCAST_CLICK );
				//------------------------------------------------------------------
				m_pInputEditBox->SetTextColor(255, 208, 249 );
				m_pTitleTexBox->SetTextColor(255, 208, 249 );
				Rsprintf(RWCHAR(m_strTitle), _RT("%s :"), G_STRING_CHAR( IDS_TOTAL_SERVER_CHAT ) );					
				m_pTitleTexBox->SetText(m_strTitle);
			}
			break;			
		case n_WholeAreaChat:
			{
				m_pChatStateMenuViewBtn->SetClientImage( TID_AREA_BRODCAST_NORMAL );
				m_pChatStateMenuViewBtn->SetBtnImage( TID_None,  TID_AREA_BRODCAST_FOCUS, 
					 TID_AREA_BRODCAST_CLICK, TID_None );
				m_pChatStateMenuViewBtn->SetHID(HD_MTSETVERC_BTN);
				
				m_pNormalChatBtn->SetClientImage( TID_NORMALCHAT_CLICK );
				m_pGuildChatBtn->SetClientImage( TID_GUILDCHAT_CLICK );
				m_pPartyChatBtn->SetClientImage( TID_PARTYCHAT_CLICK );
				m_pWhisperChatBtn->SetClientImage( TID_WHISCHAT_CLICK );
				m_pWholeChannelBtn->SetClientImage( TID_CHANNELC_CLICK );
				m_pWholeServerBtn->SetClientImage( TID_SERVERC_CLICK );
				m_pWholeAreaBtn->SetClientImage( TID_AREA_BRODCAST_NORMAL );
				m_pInputEditBox->SetTextColor(252, 126, 100 );
				m_pTitleTexBox->SetTextColor(252, 126, 100);
				Rsprintf(RWCHAR(m_strTitle), _RT("%s :"), G_STRING_CHAR( IDS_WND_AREA_CHAT ) );					
				m_pTitleTexBox->SetText(m_strTitle);
			}
			break;	


	}

	if( m_pChatStateMenuWnd->IsVisible() )
	{
		nRui->SendEvent( WID_ChatStateMenuFrame, n_emSetVisible, 
			(EPARAM)false, 0, 0, 0 );		
	}
}

void CChatActPrintWnd::SetVisible(bool bFlag)
{
	nRui->SendEvent( WID_ChatActFrame, n_emSetVisible, 
			(EPARAM)bFlag, 0, 0, 0 );
	nRui->SendEvent( WID_ChatFilterTabFrame, n_emSetVisible, 
			(EPARAM)bFlag, 0, 0, 0 );
	nRui->SendEvent( WID_ChatPrintFrame, n_emSetVisible, 
			(EPARAM)bFlag, 0, 0, 0 );
	nRui->SendEvent( WID_ChatInputFrame, n_emSetVisible, 
			(EPARAM)bFlag, 0, 0, 0 );

	if( bFlag )
	{
		m_IsOpenInstant = true;
		m_lPrevtime = g_nowTime;
		nRui->SendEvent( m_pInputEditBox->GetID(), n_emSetFocus, 0, 0, 0, 0 );
		ResetScroll();
	}

	return;
}

//----------------------------------------------------------------------------
// CChatNonActPrintWnd 채팅대화 비활성화 출력창
//----------------------------------------------------------------------------

CChatNonActPrintWnd::CChatNonActPrintWnd()
{
	m_nLeftDownPos = SPoint2(0,0);	
	m_pNonActChatBgWnd = NULL;
	m_pChatOutList = NULL;
	m_pChatScrollBarBg = NULL;
	m_pChatScrollBar = NULL;
	
	m_bEnableWeel = false;
}

CChatNonActPrintWnd::~CChatNonActPrintWnd()
{



}

void CChatNonActPrintWnd::Init()
{



}

void CChatNonActPrintWnd::Composition()
{
	CompositionNew();
}

void CChatNonActPrintWnd::CompositionOld()
{

}

void CChatNonActPrintWnd::CompositionNew()
{
	m_nLeftDownPos = SPoint2(1, nRui->Desktop.Height - 59 );
	
//#ifdef INDONESIA	
//	m_pNonActChatBgWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_ChatNonActFrame, WID_None, 
//		m_nLeftDownPos.x, m_nLeftDownPos.y , 386, 0, false );
//#else
	m_pNonActChatBgWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_ChatNonActFrame, WID_None, 
		m_nLeftDownPos.x, m_nLeftDownPos.y , 386, 0, false );
//#endif		
	m_pNonActChatBgWnd->SetClientImage( TID_None );
	m_pNonActChatBgWnd->SetWndProc( this );
	m_pNonActChatBgWnd->SetTitle( false );
	m_pNonActChatBgWnd->SetVisible( false );
	m_pNonActChatBgWnd->SetZorder( n_zoBottom );
	m_pNonActChatBgWnd->SetMove(false);
	//m_pNonActChatBgWnd->SetFocusCheck(false);
	m_pNonActChatBgWnd->Initialize();

//#ifdef INDONESIA	
//	m_pChatOutList = (CChatOutList*)nRui->CreateWnd( n_wtChatOutList, WID_None, WID_ChatNonActFrame, 
//					 19, 0, 357, 0, false );	
//#else
	m_pChatOutList = (CChatOutList*)nRui->CreateWnd( n_wtChatOutList, WID_None, WID_ChatNonActFrame, 
					 19, 0, 357, 0, false );
//#endif
	m_pChatOutList->SetOutputLevel(ChatWndSizeMin);
	m_pChatOutList->SetChatOutActState(COutState_NonAct);
	m_pChatOutList->SetFilterState(COutTotalView);
	m_pChatOutList->SetFocusCheck(false);
	m_pChatOutList->SetRefpViewChatTxtData(g_ChatMainProc.GetViewChatTxtDataList());
	m_pChatOutList->InitChatOutList();

	m_pChatScrollBarBg = (CWnd*)nRui->CreateWnd( n_wtWnd, WID_None, WID_ChatNonActFrame, 
		0, 0, 17, 0, false );	
	m_pChatScrollBarBg->SetClientImage(TID_CLIENT);
	m_pChatScrollBarBg->SetClientColor(50, 50, 50);
	m_pChatScrollBarBg->SetClientAlpah(200);
	
	m_pChatScrollBar =  (CScrollBar*)nRui->CreateWnd( n_wtScrollBar, WID_None, WID_ChatNonActFrame, 
						2, 0, 13, 0, false );
	m_pChatScrollBar->SetClientImage( TID_None );
	m_pChatScrollBar->SetThumbImage( TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR, 
					  TID_CTRL_SCR_BAR, TID_None );
	m_pChatScrollBar->InitSlide( stVertSld, 0, (MAX_CHATDATA_NUM - 0), 0, 13 );//Type, min, max, value, 썸의 폭
	m_pChatScrollBar->SetScrollBtnA( TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT, 
					  TID_CTRL_SCR2_UBT_CLK, TID_None );
	m_pChatScrollBar->SetScrollBtnB( TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT, 
					  TID_CTRL_SCR2_DBT_CLK, TID_None );
	
	m_pChatOutList->SetLinkScroll(m_pChatScrollBar, m_pChatScrollBarBg);

	ResetSizeFrame();
}

void CChatNonActPrintWnd::RePosition()
{
	m_pNonActChatBgWnd->Reposition(m_nLeftDownPos.x, m_nLeftDownPos.y);
}

void CChatNonActPrintWnd::ResetScroll()
{
	int nLastValue = ( MAX_CHATDATA_NUM - m_pChatOutList->GetOutPutLine() );
	m_pChatScrollBar->ReSetScroll(0, nLastValue, nLastValue);
	m_pChatOutList->SetDelValue(0);
}

void CChatNonActPrintWnd::Update()
{
	if( nRui->GetEMsg()->Mouse.Action == MA_LUP)
	{	
		if( nRui->GetFocusWnd() != m_pChatScrollBar->GetID() )
		{
			m_bEnableWeel = false;		
		}
	}	
}

bool CChatNonActPrintWnd::CheckFocusWnd( UINT  FocusWnd )
{
   if( FocusWnd == WID_ChatNonActFrame )
	return true;

   CWnd * Twnd = Container->Get( FocusWnd );
   if(Twnd)
   {
	  WNDID wndid = Twnd->GetParentID();
	  if( wndid == WID_ChatNonActFrame )
		  return true;
   }

   return false;
}

bool CChatNonActPrintWnd::SetWheelValue(SMouse & theMouse)
{
	if(!m_pChatScrollBar->IsVisible() || !m_bEnableWeel )
	{
		return false;
	}	
	
	if( theMouse.State == n_msWheel && theMouse.WheelValue != 0 )
	{
		POINT curtMousePos;
		GetCursorPos( &curtMousePos );
		ScreenToClient( g_RockClient.GetApp(), &curtMousePos );	
		
		if( IsVisible && 
		  GetFrameWnd()->FocusCheck( curtMousePos.x, curtMousePos.y, false ) )
		{
			SetWheelValue( theMouse.WheelValue );
			return true;
		}	
	}
	
	return false;
}

void CChatNonActPrintWnd::SetWheelValue( short _wheel )
{
	if(m_pChatScrollBar->IsVisible())
	{
		int value = 0;
		int nMax = 0;

		m_pChatScrollBar->GetValue( &value );
		nMax = m_pChatScrollBar->GetSldMax();
		
		if( ( _wheel < 0 ) && ( value < nMax ) )
		{
			value += 1;
		}
		else if( ( _wheel > 0 ) && ( value > 0 ) )
		{
			value -= 1;
		}
		
		m_pChatScrollBar->SetValue( value );
		SetListScroll( value );
	}
}

void CChatNonActPrintWnd::Proc( SEventMessage* EMsg )
{
	if( EMsg->FocusWnd == m_pNonActChatBgWnd->GetID() )
	{
		if( !g_ExchangeWnd.m_bIsExchangeLock ) 
		{
			if( EMsg->Notify == NM_FWND_CLICK )		
			{
				CSlot*			src_slot	= NULL;
				SPcItem*		pcitem	= nRui->GetCursorItem();				
								
				if( pcitem != NULL )
				{
					src_slot = nRui->GetItemSelectedSlot();
					
					if( src_slot->PcItem == NULL )
					{
						return;
					}
		
					if( src_slot->Group == n_SGROUP_QUICK )
					{
						for( int i = 0; i < RP_MAX_QUICK_SLOT; ++i )
						{
							if( nRui->GetPcInvenInfo()->QuickSlot[i] == nRui->GetCursorItem() )
							{
								g_RockClient.Send_RpCsUnLinkQuickSlot( i );
								break;
							}
						}
					}
					nRui->ResetCursorItem();
				}
			}
		}
	}
	else if( EMsg->FocusWnd == m_pChatScrollBar->GetID()  )
	{
		if( ( EMsg->Notify == NM_SLD_MOVEBAR ) || ( EMsg->Notify == NM_SLD_CLICKMOVEBAR ) 
			|| ( EMsg->Notify == NM_SLD_BTNMOVERBAR ) )
		{
			m_bEnableWeel = true;
			SetListScroll( (int)EMsg->fParam );				
		}	
	}
}

void CChatNonActPrintWnd::SetVisible(bool bFlag)
{
	if(bFlag)
	{
		ResetScroll();
	}

	nRui->SendEvent( WID_ChatNonActFrame, n_emSetVisible, 
			(EPARAM)bFlag, 0, 0, 0 );
}

void CChatNonActPrintWnd::ResetSizeFrame()
{

	SRect NoneActChatBgWndRect;
	
	SRect OutListRect;
	
	SRect ScrollBarRect;
	SRect ScrollBarBgRect;

	IPOS2 ScrollBarRPos;
	IPOS2 ScrollBarBgRPos;
	IPOS2 OutListRPos;

	m_pNonActChatBgWnd->GetClientSize(&NoneActChatBgWndRect);	
	
	m_pChatScrollBar->GetWndPosition(ScrollBarRPos);	
	m_pChatScrollBarBg->GetWndPosition(ScrollBarBgRPos);

	m_pChatOutList->GetClientSize(&OutListRect);
	m_pChatOutList->GetWndPosition(OutListRPos);	

	NoneActChatBgWndRect.y = m_nLeftDownPos.y - OutListRect.h;
	NoneActChatBgWndRect.h = OutListRect.h;	

	m_pNonActChatBgWnd->ResetSize(&NoneActChatBgWndRect);	
	
	m_pChatOutList->SetWndRepositionR(OutListRPos[0], 0);
	
	m_pChatScrollBarBg->SetWndRepositionR(ScrollBarBgRPos[0], 0);
	m_pChatScrollBarBg->GetClientSize(&ScrollBarBgRect);
	ScrollBarBgRect.h = OutListRect.h;
	m_pChatScrollBarBg->ResetSize(&ScrollBarBgRect);	
	
	m_pChatScrollBar->GetClientSize(&ScrollBarRect);	
	ScrollBarRect.h = OutListRect.h;
	m_pChatScrollBar->ResetSize(&ScrollBarRect);	
	m_pChatScrollBar->SetWndRepositionR(ScrollBarRPos[0], 0);

	ResetScroll();
	
}

void CChatNonActPrintWnd::SetListScroll( int _linenum )
{
	int nDelValue = m_pChatScrollBar->GetSldMax() - _linenum;
	m_pChatOutList->SetDelValue(nDelValue);
}

