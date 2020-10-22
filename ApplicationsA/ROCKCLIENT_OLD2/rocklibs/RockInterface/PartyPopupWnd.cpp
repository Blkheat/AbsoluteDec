#include "..\\..\\RockClient.h"
#include "..\\RockPCH.h"

#include "..\\..\\brother.h"
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
#include "UIMsgWnd.h"
#include "PartyInforWnd.h"
#include "PartyPopupWnd.h"
#include "stringmanager.h"

CPartyPopupWnd g_PartyPopupWnd;

CPartyPopupWnd::CPartyPopupWnd()
{
	m_pPartyPopupWnd = NULL;
	m_pPartyPopName = NULL;
	m_pPartyPopTexBtn1 = NULL;
	m_pPartyPopTexBtn2 = NULL;
	m_pPartyPopTexBtn3 = NULL;
	m_pPartyPopTexBtn4 = NULL;

	m_pMemberInfo = NULL;
	memset(m_tmpStr, NULL , MAX_SIZE_128 );
}

CPartyPopupWnd::~CPartyPopupWnd()
{

}

void CPartyPopupWnd::Init()
{

}

void CPartyPopupWnd::Composition()
{
    m_pPartyPopupWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_Party_PopupWnd, 
				             WID_None, 0, 30, 92, 90, false );

	m_pPartyPopupWnd->SetClientImage( TID_Party_PopupWnd );
    m_pPartyPopupWnd->SetCaption( _RT("PartyPopup Window") );
    m_pPartyPopupWnd->SetWndProc( this );
    m_pPartyPopupWnd->SetTitle( false );
    m_pPartyPopupWnd->SetVisible( false );
    m_pPartyPopupWnd->SetMove( false );
    m_pPartyPopupWnd->SetZorder( n_zoTop );
    m_pPartyPopupWnd->Initialize();

	m_pPartyPopName = (CLimitLenTextBox*)nRui->CreateWnd( n_wtLimitLenTextBox, WID_Party_PopupName, 
						WID_Party_PopupWnd, 4, 4, 88, 14, false );
    m_pPartyPopName->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );   
    m_pPartyPopName->SetClientImage( TID_None );    
    m_pPartyPopName->SetAlignText( n_atLeft, n_atCenter );   
    m_pPartyPopName->SetText( _RT("") );
    m_pPartyPopName->SetTextColor( 255, 212, 97 );   
    m_pPartyPopName->SetZorder( n_zoTop );	
	
	
	m_pPartyPopTexBtn1 = (CButton*)nRui->CreateWnd( n_wtButton, WID_Party_PopupTextBtn1, WID_Party_PopupWnd, 
	                                4, 20, 88, 14, false );
	m_pPartyPopTexBtn1->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pPartyPopTexBtn1->SetClientImage( TID_None );	
	m_pPartyPopTexBtn1->SetAlignText( n_atLeft, n_atCenter );   
	m_pPartyPopTexBtn1->SetText( G_STRING_CHAR( IDS_WND_PTLD_ASSIGNMENT ) );
	m_pPartyPopTexBtn1->SetTextColor( 255, 255, 255 );   
	m_pPartyPopTexBtn1->SetZorder( n_zoTop );
	m_pPartyPopTexBtn1->SetBtnImage( TID_None, TID_Party_TextFocus, 
								   TID_None, TID_None );	
	
	m_pPartyPopTexBtn2 = (CButton*)nRui->CreateWnd( n_wtButton, WID_Party_PopupTextBtn2, WID_Party_PopupWnd, 
	                                4, 36, 88, 14, false );
	m_pPartyPopTexBtn2->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pPartyPopTexBtn2->SetClientImage( TID_None );	
	m_pPartyPopTexBtn2->SetAlignText( n_atLeft, n_atCenter );   
	m_pPartyPopTexBtn2->SetText( G_STRING_CHAR( IDS_WND_PT_OUT ) );
	m_pPartyPopTexBtn2->SetTextColor( 255, 255, 255 );   
	m_pPartyPopTexBtn2->SetZorder( n_zoTop );
	m_pPartyPopTexBtn2->SetBtnImage( TID_None, TID_Party_TextFocus, 
								   TID_None, TID_None );	

	m_pPartyPopTexBtn3 = (CButton*)nRui->CreateWnd( n_wtButton, WID_Party_PopupTextBtn3, WID_Party_PopupWnd, 
	                                4, 52, 88, 14, false );
	m_pPartyPopTexBtn3->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pPartyPopTexBtn3->SetClientImage( TID_None );	
	m_pPartyPopTexBtn3->SetAlignText( n_atLeft, n_atCenter );   
	m_pPartyPopTexBtn3->SetText( G_STRING_CHAR( IDS_GAUGE_TRANS ) );
	m_pPartyPopTexBtn3->SetTextColor( 255, 255, 255 );   
	m_pPartyPopTexBtn3->SetZorder( n_zoTop );
	m_pPartyPopTexBtn3->SetBtnImage( TID_None, TID_Party_TextFocus, 
								   TID_None, TID_None );

	m_pPartyPopTexBtn4 = (CButton*)nRui->CreateWnd( n_wtButton, WID_Party_PopupTextBtn4, WID_Party_PopupWnd, 
									4, 68, 88, 14, false );
	m_pPartyPopTexBtn4->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pPartyPopTexBtn4->SetClientImage( TID_None );	
	m_pPartyPopTexBtn4->SetAlignText( n_atLeft, n_atCenter );   
	m_pPartyPopTexBtn4->SetText( G_STRING_CHAR( IDS_WND_COM_CANCEL ));
	m_pPartyPopTexBtn4->SetTextColor( 255, 255, 255 );   
	m_pPartyPopTexBtn4->SetZorder( n_zoTop );
	m_pPartyPopTexBtn4->SetBtnImage( TID_None, TID_Party_TextFocus, 
								   TID_None, TID_None );
}

void CPartyPopupWnd::Update()
{


}

void CPartyPopupWnd::Proc( SEventMessage* EMsg )
{
	CWnd* Wnd = NULL;
	switch( EMsg->FocusWnd )
	{
	      case WID_Party_PopupTextBtn1:
			  {
				  //리더양도
				  if( EMsg->Notify == NM_BTN_CLICK )
				  {
					  nRui->SendEvent( WID_Party_PopupWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
					  g_UIMsgWnd.Init();
					  if(m_pMemberInfo)
					  {
						 g_UIMsgWnd.SetParam(m_pMemberInfo->m_strPCName);
					  }
					  g_UIMsgWnd.SetMsgType( MSG_PARTYGIVE );
					  if( !g_UIMsgWnd.IsVisible )
			          nRui->SendEvent( WID_UIMsgWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
				  }				  
			  }
			  break;
		   case WID_Party_PopupTextBtn2:
			  {
				  //파티탈퇴
				  if( EMsg->Notify == NM_BTN_CLICK )
				  {
					  nRui->SendEvent( WID_Party_PopupWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
					  g_UIMsgWnd.Init();
					  if(m_pMemberInfo)
					  {
						 g_UIMsgWnd.SetParam(m_pMemberInfo->m_strPCName);
					  }
					  g_UIMsgWnd.SetMsgType( MSG_PARTYGOAWAY );
					  if( !g_UIMsgWnd.IsVisible )
			          nRui->SendEvent( WID_UIMsgWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
				  }				  
			  }
			  break;
		   case WID_Party_PopupTextBtn3:
			  {
				  if( EMsg->Notify == NM_BTN_CLICK )
				  {
					  if(m_pMemberInfo)
					  {
						 g_PartyInforWnd.GiveGagePartyMember(m_pMemberInfo);						 					  
					  }					  
					  //게이지 전송 루틴 추가
					  nRui->SendEvent( WID_Party_PopupWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );					  
				  }				  
			  }
			  break;				  
			  
		  case WID_Party_PopupTextBtn4:
			  {
				  if( EMsg->Notify == NM_BTN_CLICK )
				  {
					  nRui->SendEvent( WID_Party_PopupWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );				 			
				  }
			  }
			  break;	
	}
}

BOOL CPartyPopupWnd::CheckFocusWnd( UINT  FocusWnd )
{
   if( FocusWnd == WID_Party_PopupWnd )
	return TRUE;


   CWnd * Twnd = Container->Get( FocusWnd );
   if(Twnd)
   {
	  WNDID wndid = Twnd->GetParentID();
	  if( wndid == WID_Party_PopupWnd )
		  return TRUE;
   }
 	
   return FALSE;
}

void CPartyPopupWnd::SetActiveMemnu(bool bMenu1, bool bMenu2, bool bMenu3, bool bMenu4)
{
	m_pPartyPopTexBtn1->SetTextColor( 255, 255, 255 );
	m_pPartyPopTexBtn1->SetEnable(bMenu1);

	m_pPartyPopTexBtn2->SetTextColor( 255, 255, 255 );
	m_pPartyPopTexBtn2->SetEnable(bMenu2);

	m_pPartyPopTexBtn3->SetTextColor( 255, 255, 255 );
	m_pPartyPopTexBtn3->SetEnable(bMenu3);

	m_pPartyPopTexBtn4->SetTextColor( 255, 255, 255 );
	m_pPartyPopTexBtn4->SetEnable(bMenu4);	
	
	if( !bMenu1 )
	{
		m_pPartyPopTexBtn1->SetTextColor( 166, 166, 166 );
	}

	if( !bMenu2 )
	{
		m_pPartyPopTexBtn2->SetTextColor( 166, 166, 166 );
	}

	if( !bMenu3 )
	{
		m_pPartyPopTexBtn3->SetTextColor( 166, 166, 166 );
	}

	if( !bMenu4 )
	{
		m_pPartyPopTexBtn4->SetTextColor( 166, 166, 166 );
	}
	
	if(NORMAL_PARTY == g_PartyInforWnd.GetPartyType())
	{
		m_pPartyPopTexBtn2->SetText( G_STRING_CHAR( IDS_WND_PT_OUT ) );
	}
	else
	{
		m_pPartyPopTexBtn2->SetText( G_STRING_CHAR( IDS_OUT_LEAGUE ) );
	}
}

void CPartyPopupWnd::SetMemberInfo( SPARTYMEMBER_INFO * pMemberInfo )
{
	m_pMemberInfo = pMemberInfo;
	UpdatePartyPopupInfo();
}

void CPartyPopupWnd::UpdatePartyPopupInfo()
{
	if(m_pMemberInfo)
	{
	 	Rsprintf( RWCHAR(m_tmpStr), _RT("[%s]"), RWCHAR(m_pMemberInfo->m_strPCName) );
		m_pPartyPopName->SetText(m_tmpStr);
		m_pPartyPopName->SetLimitLength(10);
	}
}

void CPartyPopupWnd::LogOff()
{
	if( IsVisible )
	{
		nRui->SendEvent( WID_Party_PopupWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );	
	}
}
