#include "..\\RockPCH.h"
#include "..\\..\\RockClient.h"
#include "Rui.h"

#include "Define.h"
#include "Util.h"
#include "Render.h"
#include "Fontman.h"
#include "Resource.h"
#include "Chain.h"
#include "Container.h"

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
#include "Wnd.h"

#include "UiUtil.h"

#include "..\\..\\global.h"

#include "..\\..\\quadlist.h"
#include "..\\..\\bravolibs\\network\\Protocol.h"
#include "..\\..\\Pc.h"
#include "MessenOneChatWnd.h"
#include "stringmanager.h"

extern long g_nowTime;

CMessenOneChatWnd::CMessenOneChatWnd()
{
	
	m_IsOpenInstant = false;
	
	m_pMessenChatBgWnd = NULL;
	m_pUpTitileWnd = NULL;
	m_pMCTitleName = NULL;
	m_pMCCloseBtn = NULL;

	m_pMCOutChatMinWnd = NULL;
	m_pMCMinScrollBar = NULL;
	memset( m_pMMinChatSrtField, NULL, sizeof(CTextBox *) * MINCHATWND_LINE_NUM );

	m_pMCOutChatMidWnd = NULL;
	m_pMCMidScrollBar = NULL;
	memset( m_pMMidChatSrtField, NULL, sizeof(CTextBox *) * MIDCHATWND_LINE_NUM );
	
	m_pMCOutChatMaxWnd = NULL;
	m_pMCMaxScrollBar = NULL;
	memset( m_pMMaxChatSrtField, NULL, sizeof(CTextBox *) * MAXCHATWND_LINE_NUM );	

	m_pDownControlWnd = NULL;
	m_pDownControlBg = NULL;
	m_pSizeAddBtn = NULL;
	m_pSizeSubBtn = NULL;
	m_pMCMsgSendBtn = NULL;
	m_pMCInputMultiEdBox = NULL;	

	
	memset( m_MChatStrDataList, NULL, sizeof( SMChatStrData ) * (MAX_MCHATSTRDATA_NUM + 1) );

	m_nInsertIndex = 0;
	m_nStartListNum = 0;	
	
	m_WndSizeState = ChatWndSizeNone;

	m_uFrameWndID = WID_None;
	m_uTitileWndID = WID_None;
	m_uTitleNameID = WID_None;
	m_uCloseBtnID = WID_None;

	m_uOutChatMinWndID = WID_None;
	m_uMinScrollBarID =  WID_None;

	m_uOutChatMidWndID = WID_None;
	m_uMidScrollBarID = WID_None;

	m_uOutChatMaxWndID = WID_None;
	m_uMaxScrollBarID = WID_None;	
	
	m_uDownControlWndID = WID_None;
	m_uDownControlBgID = WID_None;
	m_uSizeAddBtnID = WID_None;
	m_uSizeSubBtnID = WID_None;
	m_uMsgSendBtnID = WID_None;
	m_uInputMultiEdBoxID = WID_None;
	
	memset(m_strInterDestName , NULL , MAX_SIZE_512 );
	memset(m_strTemp , NULL , MAX_SIZE_512 );
	memset(m_strChatstring , NULL , 120  );

	m_lPrevTime = 0;
}

CMessenOneChatWnd::~CMessenOneChatWnd()
{
	for(int i = 0 ; i < (MAX_MCHATSTRDATA_NUM + 1) ; i++)
	{
		SAFE_DELETE_ARRAY(m_MChatStrDataList[i].pStrChar);
	}
}

CFrameWnd * CMessenOneChatWnd::GetFrameWnd()
{
	return m_pMessenChatBgWnd;
}

void CMessenOneChatWnd::SetInterDestName(void * pstrName)
{
	char* strName = RCHAR(pstrName);
	Rstrcpy( RWCHAR(m_strInterDestName), RWCHAR(strName) );
	
	Rsprintf(RWCHAR(m_strTemp), G_STRING_CHAR( IDS_WND_MESSAGE ), RWCHAR(m_strInterDestName));		
	m_pMCTitleName->SetText(m_strTemp);
}

TCHAR * CMessenOneChatWnd::GetInterDestName()
{
	return m_strInterDestName;
}

UINT CMessenOneChatWnd::GetFrameWndID()
{
	return m_uFrameWndID;
}

UINT CMessenOneChatWnd::GetInputMultiEdID()
{
	return m_uInputMultiEdBoxID;
}

void CMessenOneChatWnd::Init()
{
	nRui->SendEvent( m_uInputMultiEdBoxID, n_emClearText, 0, 0, 0, 0 );
	PostMessage( nRui->GetCurrentWndHandle(), WM_USER + 44, NULL, NULL );	
	SetInterDestName( _RT("") );
	
	for(int i = 0 ; i < (MAX_MCHATSTRDATA_NUM + 1) ; i++)
	{
		SAFE_DELETE_ARRAY(m_MChatStrDataList[i].pStrChar);
	}

	m_nInsertIndex = 0;
	m_nStartListNum = 0;

	SetListScroll(0);
	m_pMCMinScrollBar->ReSetScroll(0, (MAX_MCHATSTRDATA_NUM - MINCHATWND_LINE_NUM), 0);
	m_pMCMidScrollBar->ReSetScroll(0, (MAX_MCHATSTRDATA_NUM - MIDCHATWND_LINE_NUM), 0);
	m_pMCMaxScrollBar->ReSetScroll(0, (MAX_MCHATSTRDATA_NUM - MAXCHATWND_LINE_NUM), 0);
}

void CMessenOneChatWnd::Composition()
{
	int i = 0;
	int iAddPosY =  16;
	
	int center_x	= nRui->Desktop.Width / 2;
	int center_y	= nRui->Desktop.Height / 2;

	SRect MChatWnd;
	MChatWnd.w	= 267;
	MChatWnd.h	= 316;
	MChatWnd.x	= center_x - ( MChatWnd.w / 2 );
	MChatWnd.y	= center_y - ( MChatWnd.h / 2 );

	m_pMessenChatBgWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_None, WID_None, 
		MChatWnd.x, MChatWnd.y, MChatWnd.w, MChatWnd.h, false );
	m_pMessenChatBgWnd->SetClientImage( TID_None );
	m_pMessenChatBgWnd->SetCaption( _RT("Messenger Chat Window") );
	m_pMessenChatBgWnd->SetWndProc( this );
	m_pMessenChatBgWnd->SetTitle( false );
	m_pMessenChatBgWnd->SetVisible( false );
	m_pMessenChatBgWnd->SetZorder( n_zoTop );
	m_pMessenChatBgWnd->Initialize();
	m_uFrameWndID = m_pMessenChatBgWnd->GetID();

	m_pUpTitileWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_None, m_uFrameWndID, 
	0, 0, 267, 27, false );
	m_pUpTitileWnd->SetClientImage( TID_MCUpTitleBgImg );
	m_pUpTitileWnd->SetCaption( _RT("") );
	m_pUpTitileWnd->SetWndProc( this );
	m_pUpTitileWnd->SetTitle( false );
	m_pUpTitileWnd->SetVisible( false );
	m_pUpTitileWnd->SetZorder( n_zoTop );
	m_pUpTitileWnd->Initialize();
	m_uTitileWndID = m_pUpTitileWnd->GetID();

	m_pMCTitleName = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_uTitileWndID, 
	8, 7, 200, 15, false );
	m_pMCTitleName->SetClientImage(TID_CLIENT);
	m_pMCTitleName->SetClientColor(57, 57, 57);
	m_pMCTitleName->SetAlignText( n_atLeft, n_atCenter );
	m_pMCTitleName->SetBlank( 5, 0, 1, 0 );		
	m_pMCTitleName->SetText(_RT(""));
	m_pMCTitleName->SetTextColor(255, 255, 200);
	m_pMCTitleName->SetFocusCheck(false);
	m_uTitleNameID = m_pMCTitleName->GetID();

	m_pMCCloseBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, m_uTitileWndID, 
	                                249, 7, 13, 13, false );
    m_pMCCloseBtn->SetClientImage( TID_None );
    m_pMCCloseBtn->SetBtnImage( TID_None, TID_MCCloseBtnFocus, 
							   TID_MCCloseBtnClick, TID_None );
	m_uCloseBtnID = m_pMCCloseBtn->GetID();

	m_pDownControlWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_None, m_uFrameWndID, 
	0, 240, 267, 76, false );
	m_pDownControlWnd->SetClientImage( TID_None );
	m_pDownControlWnd->SetCaption( _RT("") );
	m_pDownControlWnd->SetWndProc( this );
	m_pDownControlWnd->SetTitle( false );
	m_pDownControlWnd->SetVisible( false );
	m_pDownControlWnd->SetZorder( n_zoTop );
	m_pDownControlWnd->Initialize();
	m_uDownControlWndID = m_pDownControlWnd->GetID();

	m_pDownControlBg = (CWnd*)nRui->CreateWnd( n_wtWnd, WID_None, m_uDownControlWndID, 
	0, 0, 267, 76, false );
	m_pDownControlBg->SetClientImage(TID_MCDownControlBgImg);
	m_uDownControlBgID = m_pDownControlBg->GetID();	

	m_pSizeAddBtn =  (CButton*)nRui->CreateWnd( n_wtButton, WID_None, m_uDownControlWndID, 
	                                221, 53, 19, 17, false );
    m_pSizeAddBtn->SetClientImage( TID_None );
    m_pSizeAddBtn->SetBtnImage( TID_None, TID_MCSizeAddBtnFocus, 
							   TID_MCSizeAddBtnClick, TID_None );
	m_uSizeAddBtnID = m_pSizeAddBtn->GetID();

	m_pSizeSubBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, m_uDownControlWndID, 
	                                243, 53, 19, 17, false );
    m_pSizeSubBtn->SetClientImage( TID_None );
    m_pSizeSubBtn->SetBtnImage( TID_None, TID_MCSizeSubBtnFocus, 
							   TID_MCSizeSubBtnClick, TID_None );
	m_uSizeSubBtnID = m_pSizeSubBtn->GetID();
	
	m_pMCMsgSendBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, m_uDownControlWndID, 
	                                221, 3, 41, 43, false );
    m_pMCMsgSendBtn->SetClientImage( TID_None );
    m_pMCMsgSendBtn->SetBtnImage( TID_None, TID_MCChatSendBtnFocus, TID_MCChatSendBtnClick, TID_None );
	m_pMCMsgSendBtn->SetFontR( n_fRsvFontWndButton );
	m_pMCMsgSendBtn->SetText( G_STRING_CHAR( IDS_WND_MESSAGE_SEND ) );
	
	m_uMsgSendBtnID = m_pMCMsgSendBtn->GetID();

	
	m_pMCInputMultiEdBox = (CMLineEditBox*)nRui->CreateWnd( n_wtMLineEditBox, WID_None, m_uDownControlWndID, 
	                                5, 3, 218, 43, false );
	m_pMCInputMultiEdBox->SetClientImage( TID_None );
	m_pMCInputMultiEdBox->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pMCInputMultiEdBox->SetTextSize( 128 / sizeof(TCHAR) );
	m_pMCInputMultiEdBox->SetReturnActionClear( true );
	m_pMCInputMultiEdBox->InitMLEditBox();
	m_pMCInputMultiEdBox->SetMaxLineNum(3);
	m_pMCInputMultiEdBox->SetMaxLastLineLen(32);
	m_uInputMultiEdBoxID = m_pMCInputMultiEdBox->GetID();

	m_pMCOutChatMinWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_None, m_uFrameWndID, 
	0, 27, 267, 117, false );
	m_pMCOutChatMinWnd->SetClientImage( TID_MCOutChatMinBgImg );
	m_pMCOutChatMinWnd->SetCaption( _RT("") );
	m_pMCOutChatMinWnd->SetWndProc( this );
	m_pMCOutChatMinWnd->SetTitle( false );
	m_pMCOutChatMinWnd->SetVisible( false );
	m_pMCOutChatMinWnd->SetZorder( n_zoTop );
	m_pMCOutChatMinWnd->Initialize();
	m_uOutChatMinWndID = m_pMCOutChatMinWnd->GetID();

	m_pMCMinScrollBar = (CScrollBar*)nRui->CreateWnd( n_wtScrollBar, WID_None, m_uOutChatMinWndID, 
						249, 5, 13, 112, false );
	m_pMCMinScrollBar->SetClientImage( TID_CTRL_SCR_BG );
	m_pMCMinScrollBar->SetThumbImage( TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR_CLK, TID_None );
	m_pMCMinScrollBar->InitSlide( stVertSld, 0, (MAX_MCHATSTRDATA_NUM - MINCHATWND_LINE_NUM), 0, 13 );//Type, min, max, value, 썸의 폭
	m_pMCMinScrollBar->SetScrollBtnA( TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT_CLK, TID_None );
	m_pMCMinScrollBar->SetScrollBtnB( TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT_CLK, TID_None );
	m_uMinScrollBarID = m_pMCMinScrollBar->GetID();
	
	for( i = 0 ; i < MINCHATWND_LINE_NUM ; i++ )
	{
		m_pMMinChatSrtField[i] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_uOutChatMinWndID, 
		5, (iAddPosY * i) + 16, 190, 14, false );
		m_pMMinChatSrtField[i]->SetClientImage(TID_None);
		m_pMMinChatSrtField[i]->SetAlignText( n_atLeft, n_atCenter );
		m_pMMinChatSrtField[i]->SetBlank( 5, 0, 0, 0 );		
		m_pMMinChatSrtField[i]->SetText(_RT(""));
		m_pMMinChatSrtField[i]->SetTextColor(255, 255, 255);
	}


	m_pMCOutChatMidWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_None, m_uFrameWndID, 
	0, 27, 267, 213, false );
	m_pMCOutChatMidWnd->SetClientImage( TID_MCOutChatMidBgImg );
	m_pMCOutChatMidWnd->SetCaption( _RT("") );
	m_pMCOutChatMidWnd->SetWndProc( this );
	m_pMCOutChatMidWnd->SetTitle( false );
	m_pMCOutChatMidWnd->SetVisible( false );
	m_pMCOutChatMidWnd->SetZorder( n_zoTop );
	m_pMCOutChatMidWnd->Initialize();
	m_uOutChatMidWndID = m_pMCOutChatMidWnd->GetID();

	m_pMCMidScrollBar = (CScrollBar*)nRui->CreateWnd( n_wtScrollBar, WID_None, m_uOutChatMidWndID, 
					 249, 5, 13, 208, false );
	m_pMCMidScrollBar->SetClientImage( TID_CTRL_SCR_BG );
	m_pMCMidScrollBar->SetThumbImage( TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR_CLK, TID_None );
	m_pMCMidScrollBar->InitSlide( stVertSld, 0, (MAX_MCHATSTRDATA_NUM - MIDCHATWND_LINE_NUM), 0, 13 );//Type, min, max, value, 썸의 폭
	m_pMCMidScrollBar->SetScrollBtnA( TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT_CLK, TID_None );
	m_pMCMidScrollBar->SetScrollBtnB( TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT_CLK, TID_None );
	m_uMidScrollBarID = m_pMCMidScrollBar->GetID();

	for( i = 0 ; i < MIDCHATWND_LINE_NUM ; i++ )
	{
		m_pMMidChatSrtField[i] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_uOutChatMidWndID, 
		5, (iAddPosY * i) + 16, 190, 14, false );
		m_pMMidChatSrtField[i]->SetClientImage(TID_None);
		m_pMMidChatSrtField[i]->SetAlignText( n_atLeft, n_atCenter );
		m_pMMidChatSrtField[i]->SetBlank( 5, 0, 0, 0 );		
		m_pMMidChatSrtField[i]->SetText(_RT(""));
		m_pMMidChatSrtField[i]->SetTextColor(255, 255, 255);
	}

	m_pMCOutChatMaxWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_None, m_uFrameWndID, 
	0, 27, 267, 313, false );
	m_pMCOutChatMaxWnd->SetClientImage( TID_MCOutChatMaxBgImg );
	m_pMCOutChatMaxWnd->SetCaption( _RT("") );
	m_pMCOutChatMaxWnd->SetWndProc( this );
	m_pMCOutChatMaxWnd->SetTitle( false );
	m_pMCOutChatMaxWnd->SetVisible( false );
	m_pMCOutChatMaxWnd->SetZorder( n_zoTop );
	m_pMCOutChatMaxWnd->Initialize();
	m_uOutChatMaxWndID = m_pMCOutChatMaxWnd->GetID();

	m_pMCMaxScrollBar = (CScrollBar*)nRui->CreateWnd( n_wtScrollBar, WID_None, m_uOutChatMaxWndID, 
				249, 5, 13, 308, false );
	m_pMCMaxScrollBar->SetClientImage( TID_CTRL_SCR_BG );
	m_pMCMaxScrollBar->SetThumbImage( TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR_CLK, TID_None );
	m_pMCMaxScrollBar->InitSlide( stVertSld, 0, (MAX_MCHATSTRDATA_NUM - MAXCHATWND_LINE_NUM), 0, 13 );//Type, min, max, value, 썸의 폭
	m_pMCMaxScrollBar->SetScrollBtnA( TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT_CLK, TID_None );
	m_pMCMaxScrollBar->SetScrollBtnB( TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT_CLK, TID_None );
	m_uMaxScrollBarID = m_pMCMaxScrollBar->GetID();

	for( i = 0 ; i < MAXCHATWND_LINE_NUM ; i++ )
	{
		m_pMMaxChatSrtField[i] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_uOutChatMaxWndID, 
		5, (iAddPosY * i) + 20, 190, 14, false );
		m_pMMaxChatSrtField[i]->SetClientImage(TID_None);
		m_pMMaxChatSrtField[i]->SetAlignText( n_atLeft, n_atCenter );
		m_pMMaxChatSrtField[i]->SetBlank( 5, 0, 0, 0 );		
		m_pMMaxChatSrtField[i]->SetText(_RT(""));
		m_pMMaxChatSrtField[i]->SetTextColor(255, 255, 255);
	}	

}

void CMessenOneChatWnd::Update()
{
	if( nRui->GetCurrentWndHandle() == m_pMCInputMultiEdBox->GetHEdit())
	{
		if( nRui->GetEMsg()->Mouse.Action == MA_LUP)
		{	
			 if(m_IsOpenInstant == true)
			 {
				m_IsOpenInstant = false;
				return;
			 }			
			
			 if(nRui->GetFocusWnd() != m_uInputMultiEdBoxID )
			 {
				nRui->SendEvent( m_uInputMultiEdBoxID,n_emEditDisable,
				0, 0, 0, 0 );
			 }
		}
	}
}

void CMessenOneChatWnd::Proc( SEventMessage* EMsg )
{
	memset( m_strChatstring , NULL , 120 );
	int	nBuflen = 120;

	SPcDataParam* PcParam = nRui->GetPcParamInfo();				
	
	if( EMsg->FocusWnd == m_uCloseBtnID )
	{	
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			SetVisible(false);					
		}	
	}

	if( EMsg->FocusWnd == m_uSizeAddBtnID )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			m_WndSizeState++;
			if( m_WndSizeState >= ChatWndSizeMax )
			{
				m_WndSizeState = ChatWndSizeMax;
			}
			SetSizeState(m_WndSizeState);			
		}
	}

	if( EMsg->FocusWnd == m_uSizeSubBtnID )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			m_WndSizeState--;
			if( m_WndSizeState <= ChatWndSizeMin )
			{
				m_WndSizeState = ChatWndSizeMin;
			}
			SetSizeState(m_WndSizeState);
		}
	}

	if( EMsg->FocusWnd == m_uMsgSendBtnID )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			nRui->SendEvent( m_uInputMultiEdBoxID, n_emGetText, (EPARAM)m_strChatstring, nBuflen, 0, 0 );
			if(Rstrcmp(m_strChatstring, _RT("") ) != 0)
			{
				bool bCoverFlag = false;						//도배방지 플래그
	
				if(  SAFE_TIME_COMPARE( g_nowTime , < , SAFE_TIME_ADD( m_lPrevTime , 1200 ) )  )	//1.2초 이전에 입력		
				{
					bCoverFlag = true;
				}				
				
				nRui->SendEvent( m_uInputMultiEdBoxID, n_emClearText, 0, 0, 0, 0 );
				PostMessage( nRui->GetCurrentWndHandle(), WM_USER + 44, NULL, NULL );				
				nRui->SetEnterFlag( n_nfEditEnter );
				
				// 필터링
				if( g_RockClient.m_SlangFilter.Filter( ( char * ) m_strChatstring ) )
				{
					memset( m_strChatstring , 0 , sizeof(TCHAR) * 120 );
					Rsprintf( RWCHAR(m_strChatstring ), _RT("%s") , RWCHAR(g_RockClient.m_SlangFilter.GetString()) );
				}				
				
				//이부분에 메신져 채팅 메세지 전송루틴 추가
				//'\r'문자를 스트링에서 제거한다.			
				DeleteTokenInString(m_strChatstring, _RT('\r'));
				
				if(!bCoverFlag)
				{
					InsertChatContent(PcParam->PCName, m_strChatstring);		//자신채팅문자 출력
					//이곳에서 일대일채팅전송
					g_RockClient.Send_CsMsngrSay(m_strInterDestName, m_strChatstring);
				}

				m_lPrevTime = g_nowTime;
			}
			nRui->SendEvent( m_uInputMultiEdBoxID, n_emSetFocus, 0, 0, 0, 0 );
		}
	}

	if( EMsg->FocusWnd == m_uInputMultiEdBoxID )
	{
		if( EMsg->Notify == NM_EDT_RETURN_ACTION )
		{
			nRui->SendEvent( m_uInputMultiEdBoxID, n_emGetText, (EPARAM)m_strChatstring, nBuflen, 0, 0 );
			nRui->SetEnterFlag( n_nfEditEnter );			
			if(Rstrcmp(m_strChatstring, _RT("") ) != 0)
			{
				bool bCoverFlag = false;						//도배방지 플래그
	
				if(  SAFE_TIME_COMPARE( g_nowTime , < , SAFE_TIME_ADD( m_lPrevTime , 1200 ) )  )	//1.2초 이전에 입력		
				{
					bCoverFlag = true;
				}				
				
				// 필터링
				if( g_RockClient.m_SlangFilter.Filter( ( char * ) m_strChatstring ) )
				{
					memset( m_strChatstring , 0 , sizeof(TCHAR) * 120 );
					Rsprintf( RWCHAR(m_strChatstring ), _RT("%s") , RWCHAR(g_RockClient.m_SlangFilter.GetString()) );
				}				
				
				//이부분에 메신져 채팅 메세지 전송루틴 추가
				//'\r'문자를 스트링에서 제거한다.
				DeleteTokenInString(m_strChatstring, _RT('\r'));
				
				if(!bCoverFlag)
				{	
					InsertChatContent(PcParam->PCName, m_strChatstring);			//자신채팅문자 출력			
					//이곳에서 일대일채팅전송
					g_RockClient.Send_CsMsngrSay(m_strInterDestName, m_strChatstring);
				}
				
				m_lPrevTime = g_nowTime;	
			}
			nRui->SendEvent( m_uInputMultiEdBoxID, n_emSetFocus, 0, 0, 0, 0 );
		}	
	}

	if( (EMsg->FocusWnd == m_uMinScrollBarID) || (EMsg->FocusWnd == m_uMidScrollBarID) || 
		(EMsg->FocusWnd == m_uMaxScrollBarID) )
	{
		if( ( EMsg->Notify == NM_SLD_MOVEBAR ) || ( EMsg->Notify == NM_SLD_CLICKMOVEBAR ) 
			|| ( EMsg->Notify == NM_SLD_BTNMOVERBAR ) )
		{
			SetListScroll( (int)EMsg->fParam );				
		}	
	}
}

void CMessenOneChatWnd::SetWheelValue( short _wheel )
{
	int value = 0;
	int nMax = 0;

	CScrollBar * pScrollBar =  GetScrollBar();
	if( pScrollBar == NULL )
		return;

	pScrollBar->GetValue( &value );
	nMax = pScrollBar->GetSldMax();

	if( ( _wheel < 0 ) && ( value < nMax ) )
	{
		value += 1;
	}
	else if( ( _wheel > 0 ) && ( value > 0 ) )
	{
		value -= 1;
	}
	
	pScrollBar->SetValue( value );
	SetListScroll( value );
}

void CMessenOneChatWnd::InsertChatContent(TCHAR * pStrName, TCHAR *pStrContent)
{
	TCHAR * pTempBuf = SAFE_NEW_ARRAY( TCHAR , 512 );
	memset(pTempBuf, NULL, sizeof(TCHAR) * 512 );
	Rsprintf( RWCHAR(pTempBuf),G_STRING_CHAR( IDS_WND_MANS_CHAT ), RWCHAR(pStrName) );									
	InsertChatStrData(pTempBuf, 255, 255, 190);

	pTempBuf = NULL;
	pTempBuf = pStrContent;

	while(pTempBuf)
	{
		TCHAR *pTempOut = SAFE_NEW_ARRAY( TCHAR , 512 );
		memset(pTempOut, NULL, sizeof(TCHAR) * 512 );
#ifdef UNICODE_ROCK
		pTempBuf = (TCHAR*)::GetStringReturnInStringW('\n', false, 36, pTempBuf, pTempOut);
#else
		pTempBuf = ::GetStringReturnInString('\n', false, 36, pTempBuf, pTempOut);
#endif
	
		InsertChatStrData(pTempOut, 255, 255, 255);
	}

	SetMChatStrField();
}

void CMessenOneChatWnd::InsertNotifyContent(TCHAR *pStrContent, UINT Red, UINT Green, UINT Blue)
{
	TCHAR * pTempBuf = NULL;
	pTempBuf = pStrContent;

	while(pTempBuf)
	{
		TCHAR *pTempOut = SAFE_NEW_ARRAY( TCHAR , 512 );
		memset(pTempOut, NULL, sizeof(TCHAR) * 512 );
#ifdef UNICODE_ROCK
		pTempBuf = (TCHAR*)::GetStringReturnInStringW('\n', false, 36, pTempBuf, pTempOut);
		
#else
		pTempBuf = ::GetStringReturnInString('\n', false, 36, pTempBuf, pTempOut);
		
#endif
		InsertChatStrData(pTempOut , Red, Green, Blue);
	}
	
	SetMChatStrField();
}

void CMessenOneChatWnd::SetMChatStrField()
{
	int nLineNum = GetStrLineNum(false);
	
	CTextBox **pStrFieldList = GetStrFieldList();
	if(pStrFieldList == NULL)
		return;
	
	for( int i = 0; i < nLineNum ; ++i )
	{	
		if( (m_nStartListNum + i) > (MAX_MCHATSTRDATA_NUM - 1) )
			continue;		
		
		TCHAR *pStrChat = m_MChatStrDataList[m_nStartListNum + i].pStrChar;
		UINT  REDColor		= m_MChatStrDataList[m_nStartListNum + i].txtREDColor;
		UINT  GREENColor    = m_MChatStrDataList[m_nStartListNum + i].txtGREENColor;
		UINT  BLUEColor    = m_MChatStrDataList[m_nStartListNum + i].txtBLUEColor;
		
		pStrFieldList[i]->SetTextColor(REDColor, GREENColor, BLUEColor);

		if(pStrChat)
		{
			pStrFieldList[i]->SetText(pStrChat);			
		}
		else
		{
			pStrFieldList[i]->SetText(_RT(""));
		}
	}	

}

void CMessenOneChatWnd::InsertChatStrData(TCHAR * pStrChar, UINT Red, UINT Green, UINT Blue)
{
	if( m_nInsertIndex == MAX_MCHATSTRDATA_NUM )
	{
		SAFE_DELETE_ARRAY(m_MChatStrDataList[0].pStrChar);	
		
		for(int i = 0 ; i < (MAX_MCHATSTRDATA_NUM-1) ; i++)
		{
			m_MChatStrDataList[i].pStrChar = m_MChatStrDataList[i + 1].pStrChar;
			m_MChatStrDataList[i].txtREDColor = m_MChatStrDataList[i + 1].txtREDColor;
			m_MChatStrDataList[i].txtGREENColor = m_MChatStrDataList[i + 1].txtGREENColor;
			m_MChatStrDataList[i].txtBLUEColor = m_MChatStrDataList[i + 1].txtBLUEColor;	
		}
		
		m_MChatStrDataList[MAX_MCHATSTRDATA_NUM-1].pStrChar = pStrChar;
		m_MChatStrDataList[MAX_MCHATSTRDATA_NUM-1].txtREDColor = Red;
		m_MChatStrDataList[MAX_MCHATSTRDATA_NUM-1].txtGREENColor = Green;
		m_MChatStrDataList[MAX_MCHATSTRDATA_NUM-1].txtBLUEColor = Blue;
		
		return;
	}		

	m_MChatStrDataList[m_nInsertIndex].pStrChar = pStrChar;
	m_MChatStrDataList[m_nInsertIndex].txtREDColor = Red;
	m_MChatStrDataList[m_nInsertIndex].txtGREENColor = Green;
	m_MChatStrDataList[m_nInsertIndex].txtBLUEColor = Blue;	

	m_nInsertIndex++;

	int nLineNum = GetStrLineNum(false);
	CScrollBar * pScrollBar =  GetScrollBar();	
	if(pScrollBar == NULL)
		return;
	
	if( m_nInsertIndex > nLineNum )
	{		
		m_nStartListNum = m_nInsertIndex - nLineNum;
		pScrollBar->ReSetScroll(0, (MAX_MCHATSTRDATA_NUM - nLineNum), m_nStartListNum);
		if( m_nStartListNum > (MAX_MCHATSTRDATA_NUM - nLineNum) )
		{
			m_nStartListNum = (MAX_MCHATSTRDATA_NUM - nLineNum);	
		}
	}
}

void CMessenOneChatWnd::SetListScroll( int _linenum )
{
	m_nStartListNum = _linenum * 1;
	
	SetMChatStrField();	
}

void CMessenOneChatWnd::SetVisible(bool Flag)
{
	if(Flag)
	{
		m_IsOpenInstant = true;
		
		nRui->SendEvent( m_uFrameWndID, n_emSetVisible, 
			(EPARAM)true, 0, 0, 0 );
		nRui->SendEvent( m_uTitileWndID, n_emSetVisible, 
			(EPARAM)true, 0, 0, 0 );
		nRui->SendEvent( m_uDownControlWndID, n_emSetVisible, 
			(EPARAM)true, 0, 0, 0 );

		nRui->SendEvent( m_uInputMultiEdBoxID, n_emSetFocus,
			(EPARAM)true, 0, 0, 0 );
		SetSizeState(ChatWndSizeMid);
	}
	else
	{
		Init();
		
		m_pMCInputMultiEdBox->ClearText();
		m_pMCInputMultiEdBox->GetWindowEditText();
		
		nRui->SendEvent( m_uFrameWndID, n_emSetVisible, 
			(EPARAM)false, 0, 0, 0 );
		nRui->SendEvent( m_uTitileWndID, n_emSetVisible, 
			(EPARAM)false, 0, 0, 0 );
		nRui->SendEvent( m_uOutChatMinWndID, n_emSetVisible, 
			(EPARAM)false, 0, 0, 0 );
		nRui->SendEvent( m_uOutChatMidWndID, n_emSetVisible, 
			(EPARAM)false, 0, 0, 0 );
		nRui->SendEvent( m_uOutChatMaxWndID, n_emSetVisible, 
			(EPARAM)false, 0, 0, 0 );
		nRui->SendEvent( m_uDownControlWndID, n_emSetVisible, 
			(EPARAM)false, 0, 0, 0 );
	}
}

void CMessenOneChatWnd::SetSizeState(UINT uState)
{
	m_WndSizeState = uState;	
	
	nRui->SendEvent( m_uOutChatMinWndID, n_emSetVisible, 
			(EPARAM)false, 0, 0, 0 );
	nRui->SendEvent( m_uOutChatMidWndID, n_emSetVisible, 
			(EPARAM)false, 0, 0, 0 );
	nRui->SendEvent( m_uOutChatMaxWndID, n_emSetVisible, 
			(EPARAM)false, 0, 0, 0 );

	SRect FrameRect;
	SRect ControlRect;
	SRect TitleRect;
	SRect ChatWndRect;
	m_pMessenChatBgWnd->GetClientSize(&FrameRect);
	m_pDownControlWnd->GetClientSize(&ControlRect);
	m_pUpTitileWnd->GetClientSize(&TitleRect);
	
	switch(m_WndSizeState)
	{
		case ChatWndSizeMin:
			{
				m_pSizeSubBtn->SetEnable(false);
				m_pSizeSubBtn->SetClientImage(TID_MCSizeSubBtnNone);
				m_pSizeAddBtn->SetEnable(true);
				m_pSizeAddBtn->SetClientImage(TID_None);
				
				m_pMCOutChatMinWnd->GetClientSize(&ChatWndRect);
				FrameRect.y = ControlRect.y - ChatWndRect.h;
				FrameRect.y -= TitleRect.h;

				FrameRect.h = TitleRect.h + ChatWndRect.h + ControlRect.h;

				m_pMessenChatBgWnd->ResetSize(&FrameRect);

				m_pUpTitileWnd->SetWndRepositionR(0, 0);
				m_pMCOutChatMinWnd->SetWndRepositionR(0, TitleRect.h);
				m_pDownControlWnd->SetWndRepositionR(0, TitleRect.h + ChatWndRect.h);
				
				nRui->SendEvent( m_uOutChatMinWndID, n_emSetVisible, 
				(EPARAM)true, 0, 0, 0 );

			}
			break;
		case ChatWndSizeMid:
			{
				m_pSizeSubBtn->SetEnable(true);
				m_pSizeSubBtn->SetClientImage(TID_None);
				m_pSizeAddBtn->SetEnable(true);
				m_pSizeAddBtn->SetClientImage(TID_None);				
				
				m_pMCOutChatMidWnd->GetClientSize(&ChatWndRect);
				
				FrameRect.y = ControlRect.y - ChatWndRect.h;
				FrameRect.y -= TitleRect.h;

				FrameRect.h = TitleRect.h + ChatWndRect.h + ControlRect.h;

				m_pMessenChatBgWnd->ResetSize(&FrameRect);

				m_pUpTitileWnd->SetWndRepositionR(0, 0);
				m_pMCOutChatMidWnd->SetWndRepositionR(0, TitleRect.h);
				m_pDownControlWnd->SetWndRepositionR(0, TitleRect.h + ChatWndRect.h);
				
				nRui->SendEvent( m_uOutChatMidWndID, n_emSetVisible, 
				(EPARAM)true, 0, 0, 0 );
			}
			break;
		case ChatWndSizeMax:
			{
				m_pSizeSubBtn->SetEnable(true);
				m_pSizeSubBtn->SetClientImage(TID_None);
				m_pSizeAddBtn->SetEnable(false);
				m_pSizeAddBtn->SetClientImage(TID_MCSizeAddBtnNone);

				m_pMCOutChatMaxWnd->GetClientSize(&ChatWndRect);	
				
				FrameRect.y = ControlRect.y - ChatWndRect.h;
				FrameRect.y -= TitleRect.h;

				FrameRect.h = TitleRect.h + ChatWndRect.h + ControlRect.h;

				m_pMessenChatBgWnd->ResetSize(&FrameRect);

				m_pUpTitileWnd->SetWndRepositionR(0, 0);
				m_pMCOutChatMaxWnd->SetWndRepositionR(0, TitleRect.h);
				m_pDownControlWnd->SetWndRepositionR(0, TitleRect.h + ChatWndRect.h);
				
				nRui->SendEvent( m_uOutChatMaxWndID, n_emSetVisible, 
				(EPARAM)true, 0, 0, 0 );
			}
			break;
	}
	
	int nLineNum = GetStrLineNum(false);
	CScrollBar * pScrollBar =  GetScrollBar();	
	if(pScrollBar == NULL)
		return;
	
	if( m_nInsertIndex > nLineNum )
	{		
		m_nStartListNum = m_nInsertIndex - nLineNum;
		
		pScrollBar->ReSetScroll(0, (MAX_MCHATSTRDATA_NUM - nLineNum), m_nStartListNum);
		if( m_nStartListNum > (MAX_MCHATSTRDATA_NUM - nLineNum) )
		{
			m_nStartListNum = (MAX_MCHATSTRDATA_NUM - nLineNum);	
		}
	}

	SetMChatStrField();

}

int CMessenOneChatWnd::GetStrLineNum(bool bCompareFlag)
{
	switch(m_WndSizeState)
	{
		case ChatWndSizeMin:
			{
				if(bCompareFlag)
				{
					return (int)min(MINCHATWND_LINE_NUM, m_nInsertIndex);
				}
				else
				{
					return (int)MINCHATWND_LINE_NUM;
				}
			}
			break;
		case ChatWndSizeMid:
			{
				if(bCompareFlag)
				{
					return (int)min(MIDCHATWND_LINE_NUM, m_nInsertIndex);
				}
				else
				{
					return (int)MIDCHATWND_LINE_NUM;
				}
			}
			break;
		case ChatWndSizeMax:
			{
				if(bCompareFlag)
				{
					return (int)min(MAXCHATWND_LINE_NUM, m_nInsertIndex);
				}
				else
				{
					return (int)MAXCHATWND_LINE_NUM;
				}
			}
			break;
	}
	return 0;
}

CTextBox ** CMessenOneChatWnd::GetStrFieldList()
{
	switch(m_WndSizeState)
	{
		case ChatWndSizeMin:
			{
				return m_pMMinChatSrtField;
			}
			break;
		case ChatWndSizeMid:
			{
				return m_pMMidChatSrtField;
			}
			break;
		case ChatWndSizeMax:
			{
				return m_pMMaxChatSrtField;
			}
			break;
	}

	return NULL;
}

CScrollBar * CMessenOneChatWnd::GetScrollBar()
{
	switch(m_WndSizeState)
	{
		case ChatWndSizeMin:
			{
				return m_pMCMinScrollBar;
			}
			break;
		case ChatWndSizeMid:
			{
				return m_pMCMidScrollBar;
			}
			break;
		case ChatWndSizeMax:
			{
				return m_pMCMaxScrollBar;
			}
			break;
	}
	return NULL;
}