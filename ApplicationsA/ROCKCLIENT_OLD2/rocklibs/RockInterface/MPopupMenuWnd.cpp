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

#include "..\\..\\global.h"

#include "..\\..\\quadlist.h"
#include "..\\..\\bravolibs\\network\\Protocol.h"
#include "..\\..\\Pc.h"

#include "MCommunityWnd.h"
#include "MPopupMenuWnd.h"
#include "stringmanager.h"

CMPopupMenuWnd::CMPopupMenuWnd()
{
	m_pMPopupWnd = NULL;	

	m_pMPopTexBtn1 = NULL;
	m_pMPopTexBtn2 = NULL;
	m_pMPopTexBtn3 = NULL;
	m_pMPopTexBtn4 = NULL;

	memset(m_strParam , NULL , MAX_SIZE_256);
	
}

CMPopupMenuWnd::~CMPopupMenuWnd()
{


}

void CMPopupMenuWnd::Init()
{


}

void CMPopupMenuWnd::SetParam( TCHAR * strName )
{
	Rstrcpy(RWCHAR(m_strParam), RWCHAR(strName));
}

void CMPopupMenuWnd::Composition()
{
    m_pMPopupWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_MPopupWnd, 
				             WID_None, 30, 30, 95, 80, false );

	m_pMPopupWnd->SetClientImage( TID_MPopupMenuBgUI );
    m_pMPopupWnd->SetCaption( _RT("MPopup Window") );
    m_pMPopupWnd->SetWndProc( this );
    m_pMPopupWnd->SetTitle( false );

    m_pMPopupWnd->SetVisible( false );
    m_pMPopupWnd->SetMove( false );
    m_pMPopupWnd->SetZorder( n_zoTop );
    m_pMPopupWnd->Initialize();

	m_pMPopTexBtn1 = (CButton*)nRui->CreateWnd( n_wtButton, WID_MPopupBtn1, WID_MPopupWnd, 
	                                2, 10, 91, 15, false );
	m_pMPopTexBtn1->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pMPopTexBtn1->SetClientImage( TID_None );	
	m_pMPopTexBtn1->SetAlignText( n_atCenter, n_atCenter );   
	m_pMPopTexBtn1->SetText( G_STRING_CHAR( IDS_WND_MPOP_CHAT ) );			
	m_pMPopTexBtn1->SetTextColor( 50, 50, 255 );   
	m_pMPopTexBtn1->SetZorder( n_zoTop );
	m_pMPopTexBtn1->SetBtnImage( TID_None, TID_MPopupMenuFocus, 
								   TID_MPopupMenuFocus, TID_None );
	m_pMPopTexBtn2 = (CButton*)nRui->CreateWnd( n_wtButton, WID_MPopupBtn2, WID_MPopupWnd, 
	                                2, 26, 91, 15, false );
	m_pMPopTexBtn2->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pMPopTexBtn2->SetClientImage( TID_None );	
	m_pMPopTexBtn2->SetAlignText( n_atCenter, n_atCenter );   
	m_pMPopTexBtn2->SetText( G_STRING_CHAR( IDS_WND_MPOP_CREROOM ) );			
	m_pMPopTexBtn2->SetTextColor( 50, 50, 255 );   
	m_pMPopTexBtn2->SetZorder( n_zoTop );
	m_pMPopTexBtn2->SetBtnImage( TID_None, TID_MPopupMenuFocus, 
								   TID_MPopupMenuFocus, TID_None );
	m_pMPopTexBtn3 = (CButton*)nRui->CreateWnd( n_wtButton, WID_MPopupBtn3, WID_MPopupWnd, 
	                                2, 42, 91, 15, false );
	m_pMPopTexBtn3->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pMPopTexBtn3->SetClientImage( TID_None );	
	m_pMPopTexBtn3->SetAlignText( n_atCenter, n_atCenter );   
	m_pMPopTexBtn3->SetText( G_STRING_CHAR( IDS_WND_MPOP_INVITE ) );				
	m_pMPopTexBtn3->SetTextColor( 50, 50, 255 );   
	m_pMPopTexBtn3->SetZorder( n_zoTop );
	m_pMPopTexBtn3->SetBtnImage( TID_None, TID_MPopupMenuFocus, 
								   TID_MPopupMenuFocus, TID_None );
	m_pMPopTexBtn4 = (CButton*)nRui->CreateWnd( n_wtButton, WID_MPopupBtn4, WID_MPopupWnd, 
	                                2, 58, 91, 15, false );
	m_pMPopTexBtn4->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pMPopTexBtn4->SetClientImage( TID_None );	
	m_pMPopTexBtn4->SetAlignText( n_atCenter, n_atCenter );   
	m_pMPopTexBtn4->SetText( G_STRING_CHAR( IDS_WND_MPOP_BLOCK ) );				
	m_pMPopTexBtn4->SetTextColor( 50, 50, 255 );   
	m_pMPopTexBtn4->SetZorder( n_zoTop );
	m_pMPopTexBtn4->SetBtnImage( TID_None, TID_MPopupMenuFocus, 
								   TID_MPopupMenuFocus, TID_None );
}

void CMPopupMenuWnd::Update()
{
	if( (nRui->GetEMsg()->Mouse.Action == MA_LPRESS) ||
		(nRui->GetEMsg()->Mouse.Action == MA_LDRAG) )
	{
		if( !CheckFocusWnd(nRui->GetFocusWnd()) ) 
		{	
		   nRui->SendEvent( WID_MPopupWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );		
		}		
	}
}

void CMPopupMenuWnd::SetActiveMenu(bool bMenu1, bool bMenu2, bool bMenu3, bool bMenu4)
{
	m_pMPopTexBtn1->SetTextColor(50, 50, 255);
	m_pMPopTexBtn1->SetEnable(bMenu1);

	m_pMPopTexBtn2->SetTextColor(50, 50, 255);
	m_pMPopTexBtn2->SetEnable(bMenu2);

	m_pMPopTexBtn3->SetTextColor(50, 50, 255);
	m_pMPopTexBtn3->SetEnable(bMenu3);

	m_pMPopTexBtn4->SetTextColor(50, 50, 255);
	m_pMPopTexBtn4->SetEnable(bMenu4);

	if(!bMenu1)
	{
		m_pMPopTexBtn1->SetTextColor(106, 106, 106);
	}

	if(!bMenu2)
	{
		m_pMPopTexBtn2->SetTextColor(106, 106, 106);
	}

	if(!bMenu3)
	{
		m_pMPopTexBtn3->SetTextColor(106, 106, 106);
	}

	if(!bMenu4)
	{
		m_pMPopTexBtn4->SetTextColor(106, 106, 106);
	}
}

bool CMPopupMenuWnd::CheckFocusWnd( UINT  FocusWnd )
{
   if( FocusWnd == WID_MPopupWnd )
	return true;

   CWnd * Twnd = Container->Get( FocusWnd );
   if(Twnd)
   {
	  WNDID wndid = Twnd->GetParentID();
	  if( wndid == WID_MPopupWnd )
		  return true;
   }

   return false;
}

void CMPopupMenuWnd::Proc( SEventMessage* EMsg )
{
	switch( EMsg->FocusWnd )
	{
	   case WID_MPopupBtn1:
		   if( EMsg->Notify == NM_BTN_CLICK )
		   {
			   g_MCommunityWnd.OpenMessenOneChatWnd(m_strParam);
			   nRui->SendEvent( WID_MPopupWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
		   }
		   break;
	   case WID_MPopupBtn2:
		   if( EMsg->Notify == NM_BTN_CLICK )
		   {
			  	//방만들기 - 이쪽에서 방을 만들게되는 상대의 아이디를 전송			    
				if(!g_MCommunityWnd.m_MessenMultiChatWnd.IsVisible)
				{
					g_RockClient.Send_CsMsngrChatCreateRoom();
					g_RockClient.Send_CsMsngrChatInviteAsk(m_strParam);
				}
				nRui->SendEvent( WID_MPopupWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
		   }
		   break;
		case WID_MPopupBtn3:
		   if( EMsg->Notify == NM_BTN_CLICK )
		   {
				//채팅초대 - 채팅에 참여하고 있어야 할성화되는 기능
			    //여기서도 원래 초대만 가능하게
				if(g_MCommunityWnd.m_MessenMultiChatWnd.IsVisible)
				{				
					g_RockClient.Send_CsMsngrChatInviteAsk(m_strParam);
				}
				nRui->SendEvent( WID_MPopupWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
		   }
		   break;
	   case WID_MPopupBtn4:
		   if( EMsg->Notify == NM_BTN_CLICK )
		   {
			   //차단 전송보내기
			   g_RockClient.Send_CsMsngrMemberBlock((BYTE)n_MsngrInsert, m_strParam );

			   nRui->SendEvent( WID_MPopupWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
		   }
		   break;
	}
}