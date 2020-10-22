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

#include "MessageWndProc.h"
#include "MessenMultiChatWnd.h"
#include "stringmanager.h"

extern long g_nowTime;

CMessenMultiChatWnd::CMessenMultiChatWnd()
{
	m_IsActiveWnd = false;
	m_IsViewAllMember = false;
	m_bIsMinimize = false;

	m_pMChatMinBtn = NULL;
	
	m_uMChatMinBtnID = WID_None;
	
	m_pMemberViewFrameWnd = NULL;
	m_pMemberViewBtn = NULL;
	m_pMemberViewBgWnd = NULL;
	m_pMemberHideBtn = NULL;
	m_pViewMemNameTxt = NULL;
	memset( m_pAddViewMemNameTxt, NULL, sizeof( CTextBox * ) * (MAX_MCHAT_MEMNUM_EXCEPT_ME - 1) );

	m_MemberViewFrameWndID = WID_None;
	m_uMemberViewBtnID = WID_None;
	m_uMemberHideBtnID = WID_None;
	m_uMemberViewBgWndID = WID_None;

	memset(m_strChatMasterName , NULL , MAX_SIZE_512);

	memset( m_pStrMemNameList, NULL, sizeof( TCHAR ) * (MAX_MCHAT_MEMNUM_EXCEPT_ME + 1) );
	
	m_nNumOfMemList = 0;

	m_pMChatMinWnd = NULL;
	m_pMCMinTitleName = NULL;
	m_pMChatMaxBtn = NULL;
	m_pMChatCloseBtn = NULL;

	m_uMChatMinWndID = WID_None;
	m_uMCMinTitleNameID = WID_None;
	m_uMChatMaxBtnID = WID_None;
	m_uMChatCloseBtnID = WID_None;
	
	m_lPrevTime = 0;
}

CMessenMultiChatWnd::~CMessenMultiChatWnd()
{
	for(int i = 0 ; i < (MAX_MCHATSTRDATA_NUM + 1) ; i++)
	{
		SAFE_DELETE_ARRAY(m_MChatStrDataList[i].pStrChar);
	}
}

void  CMessenMultiChatWnd::SetActiveWnd(bool bFlag)
{
	m_IsActiveWnd = bFlag;
}

bool CMessenMultiChatWnd::GetActiveWnd()
{
	return m_IsActiveWnd;
}

void CMessenMultiChatWnd::SetChatMasterName(TCHAR * strName)
{
	Rstrcpy( RWCHAR(m_strChatMasterName), RWCHAR(strName) );
	Rsprintf(RWCHAR(m_strTemp), G_STRING_CHAR( IDS_WND_MASTERSROOM ), RWCHAR(m_strChatMasterName));		
	m_pMCTitleName->SetText(m_strTemp);
	m_pMCMinTitleName->SetText(m_strTemp);
}

TCHAR * CMessenMultiChatWnd::GetChatMasterName()
{
	return m_strChatMasterName;
}

void CMessenMultiChatWnd::Init()
{
	CMessenOneChatWnd::Init();

	for(int i = 0 ; i < (MAX_MCHAT_MEMNUM_EXCEPT_ME + 1) ; i++)
	{
		SAFE_DELETE_ARRAY(m_pStrMemNameList[i]);
	}	
	m_nNumOfMemList = 0;
}

void CMessenMultiChatWnd::SetVisible(bool Flag)
{
	SRect rect;
	
	if(Flag)
	{
		m_IsOpenInstant = true;
		
		m_IsActiveWnd = true;		
		
		SetSizeState(ChatWndSizeMid);
		SetAllMemberWndListView(false);

		m_pMessenChatBgWnd->GetClientSize(&rect);
		m_pMChatMinWnd->SetAbsolutePosition(rect.x, rect.y);
		SetMinimize(false);		
	}
	else
	{
		m_IsActiveWnd = false;

		Init();
		
		m_pMCInputMultiEdBox->ClearText();
		m_pMCInputMultiEdBox->GetWindowEditText();
		
		if(m_bIsMinimize)
		{
			m_pMChatMinWnd->GetClientSize(&rect);			
			m_pMessenChatBgWnd->SetAbsolutePosition(rect.x, rect.y);	
			nRui->SendEvent( m_uMChatMinWndID, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
		}
		else
		{
			m_pMessenChatBgWnd->GetClientSize(&rect);
			m_pMChatMinWnd->SetAbsolutePosition(rect.x, rect.y);			
			
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
			nRui->SendEvent( m_MemberViewFrameWndID, n_emSetVisible, 
				(EPARAM)false, 0, 0, 0 );
		}	
	}
}

void CMessenMultiChatWnd::SetMinimize(bool bFlag)
{
	m_bIsMinimize = bFlag;
	SRect rect;	

	if(m_bIsMinimize)
	{
		m_pMessenChatBgWnd->GetClientSize(&rect);
		
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
		nRui->SendEvent( m_MemberViewFrameWndID, n_emSetVisible, 
			(EPARAM)false, 0, 0, 0 );		
		
		m_pMChatMinWnd->SetAbsolutePosition(rect.x, rect.y);	
		nRui->SendEvent( m_uMChatMinWndID, n_emSetVisible, (EPARAM)true, 0, 0, 0 );	
	}
	else
	{
		m_pMChatMinWnd->GetClientSize(&rect);
		nRui->SendEvent( m_uMChatMinWndID, n_emSetVisible, (EPARAM)false, 0, 0, 0 );

		m_pMessenChatBgWnd->SetAbsolutePosition(rect.x, rect.y);		
		
		nRui->SendEvent( m_uFrameWndID, n_emSetVisible, 
			(EPARAM)true, 0, 0, 0 );
		nRui->SendEvent( m_uTitileWndID, n_emSetVisible, 
			(EPARAM)true, 0, 0, 0 );
		nRui->SendEvent( m_uDownControlWndID, n_emSetVisible, 
			(EPARAM)true, 0, 0, 0 );
		nRui->SendEvent( m_MemberViewFrameWndID, n_emSetVisible, 
			(EPARAM)true, 0, 0, 0 );
		nRui->SendEvent( m_uInputMultiEdBoxID, n_emSetFocus,
			(EPARAM)true, 0, 0, 0 );

		SetSizeState(m_WndSizeState);
		SetAllMemberWndListView(m_IsViewAllMember);
	}
}

void CMessenMultiChatWnd::Composition()
{
	int i = 0;
	
	CMessenOneChatWnd::Composition();

	m_pMCCloseBtn->SetHID(HD_MCBREAKBTN);
	
	m_pMChatMinBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, m_uTitileWndID, 
	                                233, 7, 13, 13, false );
    m_pMChatMinBtn->SetClientImage( TID_CTRL_BTNMIN );
    m_pMChatMinBtn->SetBtnImage( TID_CTRL_BTNMIN, TID_CTRL_BTNMIN_OVR, 
							   TID_CTRL_BTNMIN_CLK, TID_None );
	m_pMChatMinBtn->SetHID(HD_MCMIN);
	m_uMChatMinBtnID = m_pMChatMinBtn->GetID();

	SRect temRect;
	m_pDownControlBg->GetClientSize(&temRect);

	m_pViewMemNameTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_uDownControlWndID, 
	5, 53, 212, 17, false );
	m_pViewMemNameTxt->SetClientImage(TID_None);
	m_pViewMemNameTxt->SetAlignText( n_atLeft, n_atCenter );
	m_pViewMemNameTxt->SetBlank( 5, 0, 1, 0 );		
	m_pViewMemNameTxt->SetText(_RT(""));
	m_pViewMemNameTxt->SetTextColor(255, 255, 200);		

	m_pMemberViewFrameWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_None, WID_None, 
		temRect.x, (temRect.y + temRect.h) - 6, 221, 87, false );
	m_pMemberViewFrameWnd->SetClientImage( TID_None );
	m_pMemberViewFrameWnd->SetCaption( _RT("") );
	m_pMemberViewFrameWnd->SetWndProc( this );
	m_pMemberViewFrameWnd->SetTitle( false );
	m_pMemberViewFrameWnd->SetVisible( false );
	m_pMemberViewFrameWnd->SetZorder( n_zoTop );
	m_pMemberViewFrameWnd->SetMove(false);
	m_pMemberViewFrameWnd->Initialize();
	m_MemberViewFrameWndID = m_pMemberViewFrameWnd->GetID();

	m_pMessenChatBgWnd->SetMoveLinkFrameWnd(m_pMemberViewFrameWnd);

	m_pMemberViewBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, m_MemberViewFrameWndID, 
	                                2, 6, 214, 19, false );
    m_pMemberViewBtn->SetClientImage( TID_MCMemViewBtnNomal );
    m_pMemberViewBtn->SetBtnImage( TID_None, TID_MCMemViewBtnFocus, 
							   TID_MCMemViewBtnClick, TID_None );
	m_pMemberViewBtn->SetTextColor(255, 255, 255);
	m_pMemberViewBtn->SetText(G_STRING_CHAR( IDS_WND_OPEN_CHATLIST ));									
	m_pMemberViewBtn->SetAlignText( n_atCenter, n_atCenter);
	m_pMemberViewBtn->SetBlank( 0, 0, 0, 0 );
	m_pMemberViewBtn->SetClickTxt(false);

	m_uMemberViewBtnID = m_pMemberViewBtn->GetID();

	m_pMemberViewBgWnd = (CWnd*)nRui->CreateWnd( n_wtWnd, WID_None, m_MemberViewFrameWndID, 
	0, 0, 221, 78, false );
	m_pMemberViewBgWnd->SetClientImage(TID_MCMemListBgImg);
	m_uMemberViewBgWndID = m_pMemberViewBgWnd->GetID();

	for( i = 0 ; i < (MAX_MCHAT_MEMNUM_EXCEPT_ME - 1) ; i++ )
	{
		m_pAddViewMemNameTxt[i] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_MemberViewFrameWndID, 
		5, (i * 18) + 1, 212, 17, false );
		m_pAddViewMemNameTxt[i]->SetClientImage(TID_None);
		m_pAddViewMemNameTxt[i]->SetAlignText( n_atLeft, n_atCenter );
		m_pAddViewMemNameTxt[i]->SetBlank( 5, 0, 1, 0 );		
		m_pAddViewMemNameTxt[i]->SetText(_RT(""));
		m_pAddViewMemNameTxt[i]->SetTextColor(255, 255, 200);			
	}

	m_pMemberHideBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, m_MemberViewFrameWndID, 
	                                2, 78, 214, 19, false );
    m_pMemberHideBtn->SetClientImage( TID_MCMemViewBtnNomal );
    m_pMemberHideBtn->SetBtnImage( TID_None, TID_MCMemViewBtnFocus, 
							   TID_MCMemViewBtnClick, TID_None );
	m_pMemberHideBtn->SetTextColor(255, 255, 255);
	m_pMemberHideBtn->SetText(G_STRING_CHAR( IDS_WND_CLOSE_CHATLIST ));									
	m_pMemberHideBtn->SetAlignText( n_atCenter, n_atCenter);
	m_pMemberHideBtn->SetBlank( 0, 0, 0, 0 );
	m_pMemberHideBtn->SetClickTxt(false);
	m_uMemberHideBtnID = m_pMemberHideBtn->GetID();

	SetAllMemberWndListView(false);

	m_pMChatMinWnd = (CFlashFrameWnd*)nRui->CreateWnd( n_wtFlashFrameWnd, WID_None, WID_None, 0, 0, 267, 27, false );
	m_pMChatMinWnd->SetClientImage( TID_MCUpTitleBgImg );
	m_pMChatMinWnd->SetCaption( _RT("") );
	m_pMChatMinWnd->SetWndProc( this );
	m_pMChatMinWnd->SetTitle( false );
	m_pMChatMinWnd->SetMove( true );
	m_pMChatMinWnd->SetVisible( false );
	m_pMChatMinWnd->SetZorder( n_zoTop );	
	m_pMChatMinWnd->Initialize();
	m_pMChatMinWnd->SetIntervalTime(0.4f);
	m_pMChatMinWnd->SetFlashImage(TID_CLIENT);
	m_pMChatMinWnd->SetFlashColorOption(D3DTOP_ADD);
	m_pMChatMinWnd->SetFlashColor(D3DCOLOR_ARGB(63, 55, 55, 55 ));
	m_pMChatMinWnd->SetActiveFlash(false);
	m_pMChatMinWnd->SetFlashImgSize(0, 0, 267, 27);
	m_uMChatMinWndID = m_pMChatMinWnd->GetID();

	m_pMCMinTitleName = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_uMChatMinWndID, 
	8, 7, 200, 15, false );
	m_pMCMinTitleName->SetClientImage(TID_CLIENT);
	m_pMCMinTitleName->SetClientColor(57, 57, 57);
	m_pMCMinTitleName->SetAlignText( n_atLeft, n_atCenter );
	m_pMCMinTitleName->SetBlank( 5, 0, 1, 0 );		
	m_pMCMinTitleName->SetText(_RT(""));
	m_pMCMinTitleName->SetTextColor(255, 255, 200);
	m_pMCMinTitleName->SetFocusCheck(false);
	m_uMCMinTitleNameID = m_pMCMinTitleName->GetID();

	m_pMChatMaxBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, m_uMChatMinWndID, 
	                                233, 7, 13, 13, false );
    m_pMChatMaxBtn->SetClientImage( TID_CTRL_BTNBOX );
    m_pMChatMaxBtn->SetBtnImage( TID_CTRL_BTNBOX, TID_CTRL_BTNBOX_OVR, 
							   TID_CTRL_BTNBOX_CLK, TID_None );
	m_pMChatMaxBtn->SetHID(HD_MCMAX);

	m_uMChatMaxBtnID = m_pMChatMaxBtn->GetID();	
	
	m_pMChatCloseBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, m_uMChatMinWndID, 
	                                249, 7, 13, 13, false );
    m_pMChatCloseBtn->SetClientImage( TID_None );
    m_pMChatCloseBtn->SetBtnImage( TID_None, TID_MCCloseBtnFocus, 
							   TID_MCCloseBtnClick, TID_None );
	m_pMChatCloseBtn->SetHID(HD_MCBREAKBTN);
	m_uMChatCloseBtnID = m_pMChatCloseBtn->GetID();		
}

void CMessenMultiChatWnd::Update()
{
	CMessenOneChatWnd::Update();
}

void CMessenMultiChatWnd::Proc( SEventMessage* EMsg )
{
	TCHAR tMessage[256]	= {0,};
	memset(m_strChatstring , NULL , 120 );
	int	nBuflen = 120;

	SPcDataParam* PcParam = nRui->GetPcParamInfo();	
	
	if( (EMsg->FocusWnd == m_uCloseBtnID) || (EMsg->FocusWnd == m_uMChatCloseBtnID) )
	{	
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			Init();					
			g_RockClient.Send_CsMsngrChatExitRoom();						
			SetVisible(false);			
		}	
	}

	if(	EMsg->FocusWnd == m_uMChatMinBtnID )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			SetMinimize(true);				
		}
	}

	if(	EMsg->FocusWnd == m_uMChatMaxBtnID )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			m_pMChatMinWnd->SetActiveFlash(false);
			SetMinimize(false);
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
				nRui->SendEvent( m_uInputMultiEdBoxID, n_emClearText, 0, 0, 0, 0 );
				PostMessage( nRui->GetCurrentWndHandle(), WM_USER + 44, NULL, NULL );				
				nRui->SetEnterFlag( n_nfEditEnter );
				
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
					InsertChatContent(PcParam->PCName, m_strChatstring);		//자신채팅문자 출력
					//이곳에서 멀티채팅전송
				}

				if(m_IsActiveWnd)
				{
					if(!bCoverFlag)
					{
						g_RockClient.Send_CsMsngrChatSay(m_strChatstring);
					}
				}
				else
				{
					Rsprintf(RWCHAR(tMessage), G_STRING_CHAR( IDS_MSGBOX_CANT_TRANS_MSG ) );	 
				    MessageBoxWnd.Open( tMessage, 1500, NULL);
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
					InsertChatContent(PcParam->PCName, m_strChatstring);		//자신채팅문자 출력
					//이곳에서 멀티채팅전송
				}

				if(m_IsActiveWnd)
				{
				    if(!bCoverFlag)
					{
						g_RockClient.Send_CsMsngrChatSay(m_strChatstring);
					}
				}
				else
				{
					Rsprintf(RWCHAR(tMessage), G_STRING_CHAR( IDS_MSGBOX_CANT_TRANS_MSG ) );	 
					MessageBoxWnd.Open( tMessage, 1500, NULL);
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

	if( EMsg->FocusWnd == m_uMemberViewBtnID )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			SetAllMemberWndListView(true);
		}
	}

	if( EMsg->FocusWnd == m_uMemberHideBtnID )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			SetAllMemberWndListView(false);			
		}
	}

}

void CMessenMultiChatWnd::SetWheelValue( short _wheel )
{
	CMessenOneChatWnd::SetWheelValue( _wheel );	

}

void CMessenMultiChatWnd::SetAllMemberWndListView(bool bFlag)
{
	int i = 0;
	
	m_IsViewAllMember = bFlag;
	SRect BaseRect;
	m_pMemberViewBtn->SetVisible(false);
	m_pMemberViewBgWnd->SetVisible(false);
	m_pMemberHideBtn->SetVisible(false);
	
	for( i = 0 ; i < (MAX_MCHAT_MEMNUM_EXCEPT_ME - 1) ; i++ )
	{
		m_pAddViewMemNameTxt[i]->SetVisible(false);
	}

	m_pMemberViewFrameWnd->GetClientSize(&BaseRect);	
	
	if(m_IsViewAllMember)
	{
		m_pMemberViewBgWnd->SetVisible(true);
		m_pMemberHideBtn->SetVisible(true);
		for( i = 0 ; i < (MAX_MCHAT_MEMNUM_EXCEPT_ME - 1) ; i++ )
		{
			m_pAddViewMemNameTxt[i]->SetVisible(true);
		}
		BaseRect.h = 97;		
		m_pMemberViewFrameWnd->ResetSize(&BaseRect);	
	}
	else
	{
		m_pMemberViewBtn->SetVisible(true);
		BaseRect.h = 19;
		m_pMemberViewFrameWnd->ResetSize(&BaseRect);
	}
}

void CMessenMultiChatWnd::InsertChatMemName(TCHAR * strName, bool bReNewFlag)
{
	if( m_nNumOfMemList < MAX_MCHAT_MEMNUM_EXCEPT_ME )
	{
		if( ExistMemName(strName) )
		{
			return;
		}
		
		char *pstrName = SAFE_NEW_ARRAY( char , MAX_SIZE_512 );		
		memset(pstrName, 0, sizeof(char) * MAX_SIZE_512 );
		Rstrcpy( RWCHAR(pstrName), RWCHAR(strName) );	

		m_pStrMemNameList[m_nNumOfMemList] = pstrName;
		m_nNumOfMemList++;

		if(bReNewFlag)
		{
			SetMemNameTxtList();
		}
	}
}

void CMessenMultiChatWnd::DeleteChatMemName(TCHAR * strName, bool bReNewFlag)
{
	for(int i = 0; i < m_nNumOfMemList ; i++)
	{
		if( m_pStrMemNameList[i] && !Rstrcmp(strName, m_pStrMemNameList[i]) )
		{
			SAFE_DELETE_ARRAY(m_pStrMemNameList[i]);
			
			for(int j = i ; j < m_nNumOfMemList ; j++)
			{
				m_pStrMemNameList[j] = m_pStrMemNameList[j + 1];
			}
			
			m_nNumOfMemList--;
			
			if(bReNewFlag)
			{
				SetMemNameTxtList();
			}
			break;
		}	
	}
}

void CMessenMultiChatWnd::SetMemNameTxtList()
{
	
	for( int i = 0 ; i < MAX_MCHAT_MEMNUM_EXCEPT_ME ; i++ )
	{
		Rsprintf( RWCHAR(m_strTemp), _RT("%s: "), G_STRING_CHAR( IDS_WND_ROOM_MEMBER ) );
		
		if( i == 0 )
		{
			if(m_pStrMemNameList[i])
			{
				Rstrcat(RWCHAR(m_strTemp), RWCHAR(m_pStrMemNameList[i])); 
				m_pViewMemNameTxt->SetText(m_strTemp);
			}
			else
			{
				m_pViewMemNameTxt->SetText(_RT(""));
			}
		}
		else
		{
			if(m_pStrMemNameList[i])
			{
				Rstrcat(RWCHAR(m_strTemp), RWCHAR(m_pStrMemNameList[i])); 			
				m_pAddViewMemNameTxt[i-1]->SetText(m_strTemp);
			}
			else
			{
				m_pAddViewMemNameTxt[i-1]->SetText(_RT(""));
			}
		}
	}
}

bool CMessenMultiChatWnd::ExistMemName(TCHAR *strName)
{
	for( int i = 0 ; i < m_nNumOfMemList ; i++ )
	{		
		if( m_pStrMemNameList[i] && !Rstrcmp(strName, m_pStrMemNameList[i]) )
			return true;		
	}

	return false;
}

void CMessenMultiChatWnd::InsertChatContent(TCHAR * pStrName, TCHAR *pStrContent)
{
	CMessenOneChatWnd::InsertChatContent(pStrName, pStrContent);

	if(m_bIsMinimize)
	{
		m_pMChatMinWnd->SetActiveFlash(true);
	}	
}

void CMessenMultiChatWnd::InsertNotifyContent(TCHAR *pStrContent, UINT Red, UINT Green, UINT Blue)
{
	TCHAR * pTempBuf = NULL;
	pTempBuf = pStrContent;

	while(pTempBuf)
	{
		TCHAR *pTempOut = SAFE_NEW_ARRAY( TCHAR , 512 );
		memset(pTempOut, NULL, sizeof(TCHAR) * 512 );
#ifdef UNICODE_ROCK
		pTempBuf = (CHAR*)::GetStringReturnInStringW('\\', false, 36, pTempBuf, pTempOut);
#else
		pTempBuf = ::GetStringReturnInString('\\', false, 36, pTempBuf, pTempOut);
#endif
	
		InsertChatStrData(pTempOut, Red, Green, Blue);
	}
	
	SetMChatStrField();

	if(m_bIsMinimize)
	{
		m_pMChatMinWnd->SetActiveFlash(true);
	}
}