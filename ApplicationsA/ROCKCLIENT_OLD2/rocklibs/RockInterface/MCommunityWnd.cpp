#include "..\\..\\RockClient.h"
#include "..\\RockPCH.h"
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
#include "StateButton.h"

#include "..\\..\\global.h"

#include "..\\..\\quadlist.h"
#include "..\\..\\bravolibs\\network\\Protocol.h"
#include "..\\..\\Pc.h"

#include "UiUtil.h"
#include "MCommunityWnd.h"
#include "stringmanager.h"

CMCommunityWnd g_MCommunityWnd;

CMCommunityWnd::CMCommunityWnd()
{
	m_uiMState = n_MS_None;
	m_uiTempMState = n_MS_None;
	
	m_pFrameWnd =    NULL;
	m_pSelfStateTxt = NULL;

	m_pCloseBtn	  =  NULL;
	m_pStateBtn[n_MS_Friend] = NULL;
	m_pStateBtn[n_MS_Guild] = NULL;
	m_pStateBtn[n_MS_Block] = NULL;
	
	m_pScrollBar =	 NULL;

	
	m_pAddBtn = NULL;
	m_pSubBtn = NULL;
	
	m_nChatWndXpos = 0;
	m_nChatWndYPos = 0;

	m_nChatWndOriXpos = 0;
	m_nChatWndOriYPos = 0;

	memset(m_MessenNotifyDestName, NULL, sizeof(TCHAR*) * (MAX_MESSEN_NOTIFYDESTNAME_NUM + 1) );
	m_nDestNameNum = 0;
	m_nViewNotifyWndNum = 0;
}

CMCommunityWnd::~CMCommunityWnd()
{
	for(int i = 0 ; i < m_nDestNameNum ; i++)
	{
		SAFE_DELETE_ARRAY(m_MessenNotifyDestName[i]);
	}
}

int	 CMCommunityWnd::GetViewNotifyWndNum()
{
	return m_nViewNotifyWndNum;
}

void CMCommunityWnd::SetViewNotifyWndNum(int nNum)
{
	m_nViewNotifyWndNum = nNum;
}

int  CMCommunityWnd::GetDestNameNum()
{
	return m_nDestNameNum;
}

void CMCommunityWnd::SetDestNameNum(int nNum)
{
	m_nDestNameNum = nNum;	
}

void CMCommunityWnd::Init()
{
	for(int i = 0 ; i < m_nDestNameNum ; i++)
	{
		SAFE_DELETE_ARRAY(m_MessenNotifyDestName[i]);
	}
	
	m_nDestNameNum = 0;
	m_nViewNotifyWndNum = 0;	
	
	m_MFriendTabWnd.Init();
	m_MGuildTabWnd.Init();
	m_MBlockTabWnd.Init();
	m_MSettingTabWnd.Init();
	
	m_MMsgWnd.Init();
	m_MPopupMenuWnd.Init();

	for(short i = 0 ; i < MAX_MESSEN_ONECHATWND_NUM ; i++ )
	{
		m_MessenOneChatWnd[i].Init();
	}
	
	for(short i = 0 ; i < MAX_MESSEN_NOTIFYWND_NUM ; i++ )
	{
		m_MessenNotifyWnd[i].Init();
	}

	m_MessenMultiChatWnd.Init();
}

void CMCommunityWnd::Composition()
{
	int center_x	= nRui->Desktop.Width / 2;
	int center_y	= nRui->Desktop.Height / 2;

	SRect MessengerWnd;
	MessengerWnd.w	= 380;
	MessengerWnd.h	= 278;
	MessengerWnd.x	= center_x - ( MessengerWnd.w / 2 );
	MessengerWnd.y	= center_y - ( MessengerWnd.h / 2 );

	m_pFrameWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_MCommunityBgUI, WID_None, MessengerWnd.x, MessengerWnd.y, MessengerWnd.w, MessengerWnd.h, false );
	m_pFrameWnd->SetClientImage( TID_MessengerBgUI );
	m_pFrameWnd->SetIconTID( TID_CTRL_ICON_FRIEND );
	m_pFrameWnd->SetCaption( G_STRING_CHAR( IDS_WND_FRIEND ) );
	m_pFrameWnd->SetWndProc( this );
	m_pFrameWnd->SetTitle( true );
	m_pFrameWnd->SetRegion(false);
	m_pFrameWnd->SetVisible( false );
	m_pFrameWnd->Initialize();
	
	m_pSelfStateTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_MCommunityBgUI, 
	252, 4, 100, 17, false );
	m_pSelfStateTxt->SetClientImage(TID_None);
	m_pSelfStateTxt->SetAlignText( n_atCenter, n_atCenter );
	m_pSelfStateTxt->SetBlank( 0, 0, 1, 0 );		
	m_pSelfStateTxt->SetText(_RT(""));
	m_pSelfStateTxt->SetTextColor(255, 255, 200);
	m_pSelfStateTxt->SetFocusCheck(false);

	m_pCloseBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_MCloseBtn, WID_MCommunityBgUI,361, 6, 13, 13, false );   
	m_pCloseBtn->SetBtnImage( TID_CTRL_BTN_X, TID_CTRL_BTN_X_OVR, TID_CTRL_BTN_X_CLK, TID_None );

	m_pStateBtn[n_MS_Friend] = (CStateButton*)nRui->CreateWnd( n_wtStateButton, WID_MFriendTabActBtn, WID_MCommunityBgUI, 1, 26, 23, 27, false );   
	m_pStateBtn[n_MS_Friend]->SetClientImage( TID_None );
	m_pStateBtn[n_MS_Friend]->SetBtnImage( TID_MTAB_FRIEND, TID_MTAB_FRIEND, TID_MTAB_FRIEND_ON, TID_None );
	m_pStateBtn[n_MS_Friend]->SetDirectCheck(false);
	m_pStateBtn[n_MS_Friend]->SetHID(HD_F_FRIEND);

	m_pStateBtn[n_MS_Guild] = (CStateButton*)nRui->CreateWnd( n_wtStateButton, WID_MGuildTabActBtn, WID_MCommunityBgUI, 1, 52, 23, 27, false );  
	m_pStateBtn[n_MS_Guild]->SetClientImage( TID_None );
	m_pStateBtn[n_MS_Guild]->SetBtnImage( TID_MTAB_GUILD, TID_MTAB_GUILD, TID_MTAB_GUILD_ON, TID_None );
	m_pStateBtn[n_MS_Guild]->SetDirectCheck(false);
	m_pStateBtn[n_MS_Guild]->SetHID(HD_F_GUILD);

	m_pStateBtn[n_MS_Block] = (CStateButton*)nRui->CreateWnd( n_wtStateButton, WID_MBlockTabActBtn, WID_MCommunityBgUI, 1, 78, 23, 27, false );   
	m_pStateBtn[n_MS_Block]->SetClientImage( TID_None );
	m_pStateBtn[n_MS_Block]->SetBtnImage( TID_MTAB_BLOCK, TID_MTAB_BLOCK, TID_MTAB_BLOCK_ON, TID_None );
	m_pStateBtn[n_MS_Block]->SetDirectCheck(false);
	m_pStateBtn[n_MS_Block]->SetHID(HD_F_BLOCK);
	
	m_pStateBtn[n_MS_Setting] = (CStateButton*)nRui->CreateWnd( n_wtStateButton, WID_MSettingTabActBtn, WID_MCommunityBgUI, 1, 104, 23, 27, false );  
	m_pStateBtn[n_MS_Setting]->SetClientImage( TID_None );
	m_pStateBtn[n_MS_Setting]->SetBtnImage( TID_MTAB_SETTING, TID_MTAB_SETTING, TID_MTAB_SETTING_ON, TID_None );
	m_pStateBtn[n_MS_Setting]->SetDirectCheck(false);
	m_pStateBtn[n_MS_Setting]->SetHID(HD_F_SETTING);
	
	nRui->SetGroupWnd( 4, WID_MFriendTabActBtn, WID_MFriendTabActBtn, WID_MGuildTabActBtn, WID_MBlockTabActBtn, WID_MSettingTabActBtn );

	m_pScrollBar = (CScrollBar*)nRui->CreateWnd( n_wtScrollBar, WID_MScrollBar, WID_MCommunityBgUI, 363, 32, 13, 211, false );
	m_pScrollBar->SetClientImage( TID_CTRL_SCR_BG );
	m_pScrollBar->SetThumbImage( TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR_CLK, TID_None );
	m_pScrollBar->InitSlide( stVertSld, 0, 50, 0, 13 );//Type, min, max, value, 썸의 폭
	m_pScrollBar->SetScrollBtnA( TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT_CLK, TID_None );
	m_pScrollBar->SetScrollBtnB( TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT_CLK, TID_None );

	
	m_pAddBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_MAddBtn, WID_MCommunityBgUI, 138, 247, 58, 25, false ); 
	m_pAddBtn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_None );
	m_pAddBtn->SetFontR( n_fRsvFontWndButton );
	m_pAddBtn->SetText( G_STRING_CHAR( IDS_WND_ADD ) );	
	
	m_pSubBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_MSubBtn, WID_MCommunityBgUI, 198, 247, 58, 25, false );   
	m_pSubBtn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_None );		
	m_pSubBtn->SetFontR( n_fRsvFontWndButton );
	m_pSubBtn->SetText( G_STRING_CHAR( IDS_WND_DELETE ) );	

	m_MFriendTabWnd.Composition();
	m_MGuildTabWnd.Composition();
	m_MBlockTabWnd.Composition();
	m_MSettingTabWnd.Composition();

	m_MMsgWnd.Composition();
	m_MPopupMenuWnd.Composition();

	for( int i = 0 ; i < MAX_MESSEN_ONECHATWND_NUM ; i++ )
	{
		m_MessenOneChatWnd[i].Composition();
	}
	
	int nXpos, nYpos;	
	nXpos = nRui->Desktop.Width - ( 153 + 15 );
	if( nRui->GetResolution() == n_800X600 )
	{
		nYpos = 554 - (31 + 5);
	}
	if( nRui->GetResolution() == n_1024X768 )
	{
		nYpos = 722 - (31 + 5);
	}

#ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/22)
	
	else
		nYpos = 722 - (31 + 5);
	
#endif // #ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/22)

	
	m_MessenNotifyWndPos[0].x = nXpos;
	m_MessenNotifyWndPos[0].y = nYpos;
	
	CFlashWnd * pFlashWnd = NULL;
	for(short i = 0 ; i < MAX_MESSEN_NOTIFYWND_NUM ; i++ )
	{
		m_MessenNotifyWndPos[i + 1].x = nXpos;
		m_MessenNotifyWndPos[i + 1].y = nYpos - ( (i + 1) * (31 + 5));
	
		m_MessenNotifyWnd[i].Composition(i, m_MessenNotifyWndPos[i + 1].x, 
											m_MessenNotifyWndPos[i + 1].y);
	}

	m_MessenMultiChatWnd.Composition();	

	SRect tempRect;
	CFrameWnd * pFrameWnd = m_MessenOneChatWnd[0].GetFrameWnd();
	pFrameWnd->GetClientSize(&tempRect);

	m_nChatWndXpos = tempRect.x;
	m_nChatWndYPos = tempRect.y;

	m_nChatWndOriXpos = m_nChatWndXpos;
	m_nChatWndOriYPos = m_nChatWndYPos;
	
	SetMessenState( n_MsngrStateOnline );	
}

void CMCommunityWnd::Update()
{
	if( m_MFriendTabWnd.IsVisible )			
	{   m_MFriendTabWnd.Update();  }
	if( m_MGuildTabWnd.IsVisible )			
	{   m_MGuildTabWnd.Update();  }
	if( m_MBlockTabWnd.IsVisible )			
	{   m_MBlockTabWnd.Update();  }
	if( m_MSettingTabWnd.IsVisible )		
	{   m_MSettingTabWnd.Update();  }
	if( m_MMsgWnd.IsVisible )
	{   m_MMsgWnd.Update();	 }
	if( m_MPopupMenuWnd.IsVisible )
	{	m_MPopupMenuWnd.Update();  }

	for( int i = 0 ; i < MAX_MESSEN_ONECHATWND_NUM ; i++ )
	{
		if( m_MessenOneChatWnd[i].IsVisible ) 
		{   m_MessenOneChatWnd[i].Update();  }
	}

	if( m_MessenNotifyRefWnd.IsVisible )
	{	m_MessenNotifyRefWnd.Update();  }	
	
	for(short i = 0 ; i < MAX_MESSEN_NOTIFYWND_NUM ; i++ )
	{
		if( m_MessenNotifyWnd[i].IsVisible )
		{  m_MessenNotifyWnd[i].Update();  }
	}
	
	if( m_MessenMultiChatWnd.IsVisible )
	{ m_MessenMultiChatWnd.Update(); }
}

void CMCommunityWnd::SetVisible(bool bFlag)
{

	if(bFlag)
	{
		nRui->SendEvent( WID_MCommunityBgUI, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
		SetMessengerState(n_MS_Friend, false);
		nRui->SendEvent( WID_MSettingTabActBtn, n_emSetUnChecked,
				0, 0, 0, 0 );
	}
	else
	{
		nRui->SendEvent( WID_MCommunityBgUI, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
		SetMessengerState(n_MS_None, false);
	}
	
}

void CMCommunityWnd::Proc( SEventMessage* EMsg )
{
	switch( EMsg->FocusWnd )
	{
	case WID_MCloseBtn:
			if( EMsg->Notify == NM_BTN_CLICK )
			{
				SetVisible(false);				
			}
			break;	
		case WID_MFriendTabActBtn:
			if( EMsg->Notify == NM_BTN_CLICK )
			{
				SetMessengerState( n_MS_Friend, false );				
			}
			break;
		case WID_MGuildTabActBtn:
			if( EMsg->Notify == NM_BTN_CLICK )
			{
				SetMessengerState( n_MS_Guild, false );				
			}
			break;
		case WID_MBlockTabActBtn:
			if( EMsg->Notify == NM_BTN_CLICK )
			{
				SetMessengerState( n_MS_Block, false );				
			}
			break;
		case WID_MSettingTabActBtn:
			if( EMsg->Notify == NM_BTN_CLICK )
			{
				if( EMsg->Notify == NM_BTN_CLICK )
				{
					SetMessengerState( n_MS_Setting, false );				
				}
			}
			break;
		case WID_MScrollBar:
			if( ( EMsg->Notify == NM_SLD_MOVEBAR ) || ( EMsg->Notify == NM_SLD_CLICKMOVEBAR ) 
				|| ( EMsg->Notify == NM_SLD_BTNMOVERBAR ) )
			{
				//상황에 따라 이동
				SetMessengerListScroll( (int)EMsg->fParam );				
			}
			break;
		case WID_MAddBtn:
			if( EMsg->Notify == NM_BTN_CLICK )
			{
				AddBtnProc();
			}
			break;
		case WID_MSubBtn:
			if( EMsg->Notify == NM_BTN_CLICK )
			{
				SubBtnProc();				
			}
			break;
	}
}

///---------------------------------------------------------------------------
///-- SetMessengerState
///---------------------------------------------------------------------------
void CMCommunityWnd::SetMessengerState( UINT uiState , bool reDraw)
{
	
	if( (m_uiMState != uiState) || reDraw );
	else
	{
		return;
	}

	if(m_MMsgWnd.IsVisible)
	{
		m_MMsgWnd.SetVisible(false);
	}
	
	m_uiMState = uiState;

	m_pAddBtn->SetVisible(false);
	m_pSubBtn->SetVisible(false);
	
	nRui->SendEvent( m_MFriendTabWnd.GetMFieldTabWndId(), n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	nRui->SendEvent( m_MGuildTabWnd.GetMFieldTabWndId(), n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	nRui->SendEvent( WID_MBlockTabWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	nRui->SendEvent( WID_MSettingTabWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	
	m_MSettingTabWnd.ResetSettingData();
	
	switch(m_uiMState)
	{
		case n_MS_Friend:
			{
				nRui->SendEvent( WID_MFriendTabActBtn, n_emSetChecked, 0, 0, 0, 0 );				
				m_pFrameWnd->SetCaption( G_STRING_CHAR( IDS_WND_FRIEND ));
				nRui->SendEvent( m_MFriendTabWnd.GetMFieldTabWndId(), n_emSetVisible, (EPARAM)true, 0, 0, 0 );
				m_pAddBtn->SetVisible(true);
				m_pSubBtn->SetVisible(true);
				//m_pAddBtn->SetClientImage(TID_MFriendAddBtn);
				//m_pSubBtn->SetClientImage(TID_MFriendSubBtn);
				
				m_MFriendTabWnd.FieldBntSelected(-1);
				m_MFriendTabWnd.SetListScroll(0);
				m_pScrollBar->ReSetScroll(0, (MAX_SFRIEND_DATA_NUM - 6), 0);
			}
			break;
		case n_MS_Guild:
			{
				nRui->SendEvent( WID_MGuildTabActBtn, n_emSetChecked, 0, 0, 0, 0 );
				m_pFrameWnd->SetCaption( G_STRING_CHAR( IDS_WND_GUILD ));
				nRui->SendEvent( m_MGuildTabWnd.GetMFieldTabWndId(), n_emSetVisible, (EPARAM)true, 0, 0, 0 );

				m_MGuildTabWnd.FieldBntSelected(-1);
				m_MGuildTabWnd.SetListScroll(0);
				m_pScrollBar->ReSetScroll(0, (MAX_SFRIEND_DATA_NUM - 6), 0);
			}
			break;
		case n_MS_Block:
			{
				nRui->SendEvent( WID_MBlockTabActBtn, n_emSetChecked, 0, 0, 0, 0 );
				m_pFrameWnd->SetCaption( G_STRING_CHAR( IDS_WND_BLOCK ) );
				nRui->SendEvent( WID_MBlockTabWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
				m_pAddBtn->SetVisible(true);
				m_pSubBtn->SetVisible(true);
				//m_pAddBtn->SetClientImage(TID_MBlockAddBtn);
				//m_pSubBtn->SetClientImage(TID_MBlockSubBtn);
				
				m_MBlockTabWnd.FieldBntSelected(-1);
				m_MBlockTabWnd.SetListScroll(0);
				m_pScrollBar->ReSetScroll(0, (MAX_SBLOCK_DATA_NUM/2 - MAX_BLOCKFIELD_LINE/2), 0);
			}
			break;
		case n_MS_Setting:
			{
				nRui->SendEvent( WID_MSettingTabActBtn, n_emSetChecked, 0, 0, 0, 0 );
				m_pFrameWnd->SetCaption( G_STRING_CHAR( IDS_WND_SETTING ) );
				nRui->SendEvent( WID_MSettingTabWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
				m_pAddBtn->SetVisible(false);
				m_pSubBtn->SetVisible(false);
			}
			break;
	}	
}

void CMCommunityWnd::SetWheelValue( short _wheel )
{
	int value = 0;
	int nMax = 0;

	m_pScrollBar->GetValue( &value );
	nMax = m_pScrollBar->GetSldMax();
	
	if( ( _wheel < 0 ) && ( value < nMax ) )
	{
		value += 1;
	}
	else if( ( _wheel > 0 ) && ( value > 0 ) )
	{
		value -= 1;
	}
	
	m_pScrollBar->SetValue( value );
	SetMessengerListScroll( value );
}

void CMCommunityWnd::SetMessengerListScroll( int _linenum )
{
	switch(m_uiMState)
	{
		case n_MS_Friend:
			{
				m_MFriendTabWnd.SetListScroll(_linenum);
			}
			break;
		case n_MS_Guild:
			{
				m_MGuildTabWnd.SetListScroll(_linenum);
			}
			break;
		case n_MS_Block:
			{
				m_MBlockTabWnd.SetListScroll(_linenum);
			}
			break;
	}
}

bool CMCommunityWnd::SetMessenChatWndWheelValue(SMouse & theMouse)
{
	
	if( theMouse.State == n_msWheel && theMouse.WheelValue != 0 )
	{
		POINT curtMousePos;
		GetCursorPos( &curtMousePos );
		ScreenToClient( g_RockClient.GetApp(), &curtMousePos );
		
		for( int i = 0 ; i < MAX_MESSEN_ONECHATWND_NUM ; i++ )
		{
			if( ( m_MessenOneChatWnd[i].IsVisible == true ) && 
			 m_MessenOneChatWnd[i].GetFrameWnd()->FocusCheck( curtMousePos.x, curtMousePos.y, false ) )
			{
				m_MessenOneChatWnd[i].SetWheelValue( theMouse.WheelValue );
				return true;
			}	
		}

		if( ( m_MessenMultiChatWnd.IsVisible == true ) && 
			m_MessenMultiChatWnd.GetFrameWnd()->FocusCheck( curtMousePos.x, curtMousePos.y, false ) )
		{
			m_MessenMultiChatWnd.SetWheelValue( theMouse.WheelValue );
			return true;
		}
	}
	
	return false;
}

void CMCommunityWnd::AddBtnProc()
{
	switch(m_uiMState)
	{
		case n_MS_Friend:
			{
				SetMessengerState(n_MS_None, false);
				m_MMsgWnd.SetMsgType(MSG_MINPUTFRIEND);
				m_MMsgWnd.SetVisible(true);
				nRui->SendEvent( WID_MMsgInEdit, n_emSetFocus, 0, 0, 0, 0 );			
			}
			break;
		case n_MS_Guild:
			{

			}
			break;
		case n_MS_Block:
			{
				SetMessengerState(n_MS_None, false);
				m_MMsgWnd.SetMsgType(MSG_MINPUTBLOCK);
				m_MMsgWnd.SetVisible(true);
				nRui->SendEvent( WID_MMsgInEdit, n_emSetFocus, 0, 0, 0, 0 );				
			}
			break;
	}
	
}

void CMCommunityWnd::SubBtnProc()
{
	switch(m_uiMState)
	{
		case n_MS_Friend:
			{
				SFriendData *pFriendData = m_MFriendTabWnd.GetSelectData();
				if(pFriendData)
				{
					SetMessengerState(n_MS_None, false);
					m_MMsgWnd.SetParam(pFriendData->strFName);
					m_MMsgWnd.SetMsgType(MSG_MDELETEFRIEND);
					m_MMsgWnd.SetVisible(true);										
				}
			}
			break;
		case n_MS_Guild:
			{

			}
			break;
		case n_MS_Block:
			{
				SBlockData *pBlockData = m_MBlockTabWnd.GetSelectData();
				if(pBlockData)
				{
					SetMessengerState(n_MS_None, false);
					m_MMsgWnd.SetParam(pBlockData->strBDestName);
					m_MMsgWnd.SetMsgType(MSG_MDELETEBLOCK);
					m_MMsgWnd.SetVisible(true);
				}
			}
			break;
	}
}

void CMCommunityWnd::OpenMessenOneChatWnd(TCHAR * strParam)
{
	
	CMessenOneChatWnd *pMessenOneChatWnd = NULL;
	
	if( pMessenOneChatWnd = FindMessenOneChatWnd(strParam) )
	{
		return;
	}	
	
	if( pMessenOneChatWnd = FindMessenOneChatWnd(_RT("")) )
	{
		pMessenOneChatWnd->SetInterDestName(strParam);
		CFrameWnd * pFrameWnd = pMessenOneChatWnd->GetFrameWnd();
		POINT pos;
		GetMessenOneChatWndPos(pos);
		pFrameWnd->SetAbsolutePosition(pos.x, pos.y);
		pMessenOneChatWnd->SetVisible(true);	
	}	
}

CMessenOneChatWnd * CMCommunityWnd::FindMessenOneChatWnd(void * pstrParam)
{
	char* strParam = (char*)(pstrParam);
	for( int i = 0 ; i < MAX_MESSEN_ONECHATWND_NUM ; i++ )
	{
		//이미 같은윈도우가 존재한다면 
		if( Rstrcmp( m_MessenOneChatWnd[i].GetInterDestName(), strParam ) == 0 ) 
		{
			return &m_MessenOneChatWnd[i];
		}		
	}			
	return NULL;
}

void CMCommunityWnd::GetMessenOneChatWndPos(POINT & pos)
{
	pos.x = m_nChatWndXpos;
	pos.y = m_nChatWndYPos;

	m_nChatWndXpos += 10;
	m_nChatWndYPos += 10;

	if(m_nChatWndXpos > m_nChatWndOriXpos + 40)
	{
		m_nChatWndXpos = m_nChatWndOriXpos - 40;
	}
	
	if( m_nChatWndYPos > m_nChatWndOriYPos + 40 ) 
	{
		m_nChatWndYPos = m_nChatWndOriYPos - 40;
	}
}

void CMCommunityWnd::InsertOneChatContent(TCHAR * pStrName, TCHAR * pStrContent)
{
	CMessenOneChatWnd *pMessenOneChatWnd = NULL;	
	//채팅윈도우가 돌아가는경우
	if( pMessenOneChatWnd = FindMessenOneChatWnd(pStrName) )
	{
		pMessenOneChatWnd->InsertChatContent(pStrName, pStrContent);
		
		return;
	}
	else //그렇지 않을경우에는 
	{
		
		if( m_nDestNameNum >= MAX_MESSEN_NOTIFYDESTNAME_NUM )     //숫자제한
			return;

		pMessenOneChatWnd = FindMessenOneChatWnd(_RT(""));
		if(pMessenOneChatWnd == NULL)
		{
			return;
		}
	
		pMessenOneChatWnd->SetInterDestName(pStrName);
		pMessenOneChatWnd->InsertChatContent(pStrName, pStrContent);
		
		//스트링 배열에 보관
		TCHAR *pDestName = SAFE_NEW_ARRAY( TCHAR , 512 );		
		memset(pDestName, 0, sizeof(TCHAR)*512);		
		Rstrcpy(RWCHAR(pDestName), RWCHAR(pStrName));
		m_MessenNotifyDestName[m_nDestNameNum] = pDestName;		
		m_nDestNameNum++;

		if( m_nDestNameNum <= MAX_MESSEN_NOTIFYWND_NUM )
		{
			ActionInsertMessenNotifyWnd();		
		}
	}
}

void CMCommunityWnd::ActionInsertMessenNotifyWnd()
{
	m_MessenNotifyWnd[m_nViewNotifyWndNum].MoveAction(NState_UpAction, m_MessenNotifyWndPos[m_nViewNotifyWndNum],
													m_MessenNotifyWndPos[m_nViewNotifyWndNum + 1]);
	m_MessenNotifyWnd[m_nViewNotifyWndNum].SetInterDestName(m_MessenNotifyDestName[m_nViewNotifyWndNum]);
	nRui->SendEvent( m_MessenNotifyWnd[m_nViewNotifyWndNum].GetFrameID(), n_emSetVisible, 
			(EPARAM)true, 0, 0, 0 );

	m_nViewNotifyWndNum++;
}

void CMCommunityWnd::InsertMultiChatContent(bool IsSystem, UINT uRed, UINT uGreen, UINT uBlue, 
								TCHAR * pStrName, TCHAR * pStrContent)
{
	if(!m_MessenMultiChatWnd.IsVisible)
		return;	
	
	if(IsSystem)
	{
		m_MessenMultiChatWnd.InsertNotifyContent(pStrContent, uRed, uGreen, uBlue);
	}
	else
	{
		m_MessenMultiChatWnd.InsertChatContent(pStrName, pStrContent);
	}	
}

void CMCommunityWnd::SetMessenState(UINT uMessenState)
{
	switch(uMessenState)
	{
		case n_MsngrStateOnline:
			{
				m_pSelfStateTxt->SetTextColor(255, 255, 200);
				m_pSelfStateTxt->SetText( G_STRING_CHAR( IDS_MSGER_STATE_ONLINE ) );
			}
			break;
		case n_MsngrStateRest:
			{
				m_pSelfStateTxt->SetTextColor(255, 255, 200);
				m_pSelfStateTxt->SetText( G_STRING_CHAR( IDS_MSGER_STATE_REST ) );	
			}
			break;
		case n_MsngrStateEat:
			{
				m_pSelfStateTxt->SetTextColor(255, 255, 200);
				m_pSelfStateTxt->SetText( G_STRING_CHAR( IDS_MSGER_STATE_EATTING ));
			}
			break;
		case n_MsngrStateOffline:
			{
				m_pSelfStateTxt->SetTextColor(255, 130, 130);
				m_pSelfStateTxt->SetText( G_STRING_CHAR( IDS_MSGER_STATE_OFFLINE ));		
			}
			break;
	}
}

void CMCommunityWnd::LogOff()
{
	SetVisible(false);

	for(int i = 0 ; i < MAX_MESSEN_ONECHATWND_NUM ; i++ )
	{
		if(m_MessenOneChatWnd[i].IsVisible)
		{
			m_MessenOneChatWnd[i].SetVisible(false);
		}
	}
	
	for(short i = 0 ; i < MAX_MESSEN_NOTIFYWND_NUM ; i++ )
	{
		if(m_MessenNotifyWnd[i].IsVisible)
		{
			nRui->SendEvent( m_MessenNotifyWnd[i].GetFrameID(), n_emSetVisible, 
			(EPARAM)false, 0, 0, 0 );	
		}		
	}

	if(m_MessenMultiChatWnd.IsVisible)
	{
		m_MessenMultiChatWnd.SetVisible(false);
	}

	Init();
}