// PAcceptYesNoWnd.cpp: implementation of the CAcceptYesNoWnd class.
//
//////////////////////////////////////////////////////////////////////

#include "Rui.h"

#include "..\\..\\brother.h"
#include "..\\..\\RockClient.h"


#include "CPAcceptYesNoWnd.h"
#include "ExchangeWndProc.h"
#include "InventoryWndProc.h"
#include "StringManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAcceptYesNoWnd g_AcceptYesNoWnd;


CAcceptYesNoWnd::CAcceptYesNoWnd()
{
	SAFE_STR_CPY( m_strLeaderName, _RT(""), 256 );
	
	m_pTextBox[0] = NULL;
	m_pTextBox[1] = NULL;
	m_pTextBox[2] = NULL;
}

CAcceptYesNoWnd::~CAcceptYesNoWnd()
{
	
}

void CAcceptYesNoWnd::Init()
{
	
}

void CAcceptYesNoWnd::Composition()
{
	int center_x	= nRui->Desktop.Width / 2;
	int center_y	= nRui->Desktop.Height / 2;
	
	CFrameWnd*		fw;	
	CTextBox*        tb;
	
	SRect Acceptwnd;
	Acceptwnd.w = 230;
	Acceptwnd.h = 133;
	Acceptwnd.x	= center_x - ( Acceptwnd.w / 2 );
	Acceptwnd.y	= center_y - ( Acceptwnd.h / 2 );
	
	fw = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_Party_YesNo_Wnd, WID_None, 
		Acceptwnd.x, Acceptwnd.y, Acceptwnd.w, Acceptwnd.h, false );
	fw->SetWndTexStyle( fwfTex_Style_Tile9 );
	fw->SetClientImage( TID_CTRL_WND_L1 );	
	fw->SetIconTID( TID_CTRL_ICON_ALRAM );
	fw->SetWndTileSizeLR( 40, 29 );
	fw->SetWndTileSizeTB( 31, 11 );
	fw->SetTitle( true );
	fw->SetCaption( G_STRING_CHAR( IDS_WND_ALARM ) ); 
	fw->SetWndProc( this );	
	fw->SetVisible( false );
	fw->SetMove( false );
	fw->SetZorder( n_zoTop );
	fw->Initialize();

	///-  초대 수락버튼
	m_button[0] = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_Party_YesNo_Wnd,  28, 102, 58, 25, false );		
	m_button[0]->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_CTRL_BTN58_DIS );
	m_button[0]->SetFontR( n_fRsvFontWndButton );
	m_button[0]->SetFocusCheck ( true );
	m_button[0]->SetText( G_STRING_CHAR( IDS_WND_COM_ACCEPT ) );
	
	///-- 초대 거부버튼
	m_button[1] = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_Party_YesNo_Wnd,  143, 102, 58, 25, false );		
	m_button[1]->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_CTRL_BTN58_DIS );	
	m_button[1]->SetFontR( n_fRsvFontWndButton );	
	m_button[1]->SetFocusCheck( true );
	m_button[1]->SetText( G_STRING_CHAR( IDS_WND_COM_DENINE ) );
	
	tb = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_Party_NotifyTex1, WID_Party_YesNo_Wnd, 0, 41, 230, 16, false );		
	tb->SetFontR( n_fRsvFontWndButton );
	tb->SetClientColor( 180, 175, 160 );	
	tb->SetZorder( n_zoTop );
	tb->SetFocusCheck ( false );
	m_pTextBox[0] = tb;
	
	tb = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_Party_NotifyTex2, WID_Party_YesNo_Wnd, 0, 57, 230, 16, false );
	tb->SetFontR( n_fRsvFontWndButton );
	tb->SetClientColor( 180, 175, 160 );
	tb->SetZorder( n_zoTop );
	tb->SetFocusCheck ( false );

	
	m_pTextBox[1] = tb;
	m_pTextBox[1]->SetText(G_STRING_CHAR( IDS_WND_AL_PT )); 
	
	tb = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_Party_NotifyTex3, WID_Party_YesNo_Wnd, 0, 73, 230, 16, false );		
	tb->SetFontR( n_fRsvFontWndButton );
	tb->SetClientColor( 180, 175, 160 );	
	tb->SetZorder( n_zoTop );
	tb->SetFocusCheck ( false );
	m_pTextBox[2] = tb;
	m_pTextBox[2]->SetText(G_STRING_CHAR( IDS_WND_AL_ASK )); 
	
}

void CAcceptYesNoWnd::SetLearderName(TCHAR *pName)
{
    SAFE_STR_CPY( m_strLeaderName, pName, 256 );
	
	TCHAR strTempBuf[512] = {0,};
	Rsprintf(RWCHAR(strTempBuf), G_STRING_CHAR( IDS_WND_AL_SUBJECT ), RWCHAR(m_strLeaderName)); 
	
	if(m_pTextBox[0])
		m_pTextBox[0]->SetText(strTempBuf);
}

void CAcceptYesNoWnd::Update()
{
	return;
}

void CAcceptYesNoWnd::Proc( SEventMessage* EMsg )
{
	CWnd* Wnd = NULL;
	
	if( EMsg->Notify == NM_BTN_CLICK )
	{		
		if( m_button[0]->GetID() == EMsg->FocusWnd )
		{
			switch( m_AcceptState )
			{
			case n_ACCEPT_PARTY:
				{					
					g_RockClient.Send_RpCsPartyAnsJoin( m_strLeaderName, true );
				}
				break;
			case n_ACCEPT_EXCHANGE:
				{
					g_RockClient.Send_RpCsAnsExchange( n_Success );
					
					// 거래 승락 
					InventoryWnd.FrameWnd->Reposition( 470, 245 );
#ifdef WIDE_PAY_INVENTORY //유료인벤확장			
					InventoryWnd.Open();
#else
					nRui->SendEvent( WID_InventoryWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
#endif
					g_ExchangeWnd.Open();
					g_ExchangeWnd.m_bIsAccept = TRUE;
					SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_ALLOW_EXCHANGE ), m_strLeaderName );
				}
				break;
			default:
				break;
			}
			nRui->SendEvent( WID_Party_YesNo_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 ); 
		}		
		else if ( m_button[1]->GetID() == EMsg->FocusWnd )
		{
			switch( m_AcceptState )
			{
			case n_ACCEPT_PARTY:
				{					
					g_RockClient.Send_RpCsPartyAnsJoin( m_strLeaderName, false );
				}
				break;
			case n_ACCEPT_EXCHANGE:
				{
					g_RockClient.Send_RpCsAnsExchange( n_Fail );
				}
				break;
			default:
				break;
			}
			nRui->SendEvent( WID_Party_YesNo_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 ); 
			
		}
	}
	
	return;
}


void CAcceptYesNoWnd::Open( N_ACCEPTSTATE aAcceptState )
{
	m_AcceptState = aAcceptState;
	
	nRui->SendEvent( WID_Party_YesNo_Wnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );       
	
	switch( m_AcceptState )
	{
	case n_ACCEPT_PARTY:
		{
			m_pTextBox[1]->SetText(G_STRING_CHAR( IDS_WND_AL_PT ));
		}
		break;
	case n_ACCEPT_EXCHANGE:
		{
			m_pTextBox[1]->SetText(G_STRING_CHAR( IDS_WND_AL_EX ));
		}
		break;
	}
	
	return;
}


void CAcceptYesNoWnd::LogOff()
{	
	if( IsVisible )
	{
		nRui->SendEvent( WID_Party_YesNo_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );      
		
		switch( m_AcceptState )
		{
		case n_ACCEPT_PARTY:
			{
				g_RockClient.Send_RpCsPartyAnsJoin( m_strLeaderName, false );
			}
			break;
		case n_ACCEPT_EXCHANGE:
			{
				g_RockClient.Send_RpCsAnsExchange( n_Fail );
			}
			break;
		}
	}
	
	return;
}



			        
