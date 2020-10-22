// RadioBroadcast.cpp: implementation of the CRadioBroadcast class.
//
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#include "Rui.h"
//////////////////////////////////////////////////////////////////////////
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

#include "RadioBroadcast.h"
#include <shellapi.h>
#include "StringManager.h"
#include "RockClient.h"
CRadioBroadcast *g_pRadioBroadcast = NULL;

CRadioBroadcast::CRadioBroadcast()
{
	IsVisible = false;
	ZeroMemory( &m_Wndcheck ,sizeof(m_Wndcheck));
	strcpy(m_Wndcheck[0].TileName , "光明与黑暗网络电台-电信收听入口 - Microsoft Internet Explorer");
	strcpy(m_Wndcheck[1].TileName , "光明与黑暗网络电台-网通收听入口 - Microsoft Internet Explorer");
	strcpy(m_Wndcheck[0].AddressName , "guanghei.radio-arts.com/dianxin1/" );
	strcpy(m_Wndcheck[1].AddressName , "guanghei.radio-arts.com/wangtong1/" );
	Init();
}

CRadioBroadcast::~CRadioBroadcast()
{


}

void CRadioBroadcast::Init()
{
	for(int i = 0 ; i < 2 ; ++i)
	{
		m_Wndcheck[i].m_IsExplorer = false ; 
		m_Wndcheck[i].m_IsLive = false ; 
		m_Wndcheck[i].m_dwDelayTime = false;
	}
}

void CRadioBroadcast::Open()
{
	Init();
	nRui->SendEvent( m_FrameWnd->GetID(), n_emSetVisible, (EPARAM)true, 0, 0, 0 );
}

void CRadioBroadcast::Close()
{
	//0 光明与黑暗网络电台-电信收听入口
	//1 光明与黑暗网络电台-网通收听入口
	/*
	HWND hHandle1 = FindWindow(NULL, "光明与黑暗网络电台-电信收听入口 - Microsoft Internet Explorer"); 
	HWND hHandle2 = FindWindow(NULL, "光明与黑暗网络电台-网通收听入口 - Microsoft Internet Explorer"); 
	if( hHandle1 || hHandle2 ) 
	{
		PostMessage(hHandle1, WM_CLOSE, NULL,NULL); 
		PostMessage(hHandle2, WM_CLOSE, NULL,NULL); 
	}
	*/

	nRui->SendEvent( m_FrameWnd->GetID(), n_emSetVisible, (EPARAM)false, 0, 0, 0 );
}

void CRadioBroadcast::Composition()
{

	int center_x	= nRui->Desktop.Width / 2;
	int center_y	= nRui->Desktop.Height / 2;
	SRect invenwnd;
	CImageBox9*		img9 = NULL;
	CTextBox*		text = NULL;

	invenwnd.w	= 180;
	invenwnd.h	= 140;
	invenwnd.x	= center_x - ( invenwnd.w / 2 );
	invenwnd.y	= center_y - ( invenwnd.h / 2 );
	///-- Frame Wnd
	m_FrameWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_None, WID_None, invenwnd.x, invenwnd.y, invenwnd.w, invenwnd.h, false );
	m_FrameWnd->SetWndTexStyle( fwfTex_Style_Tile9 );
	m_FrameWnd->SetClientImage( TID_CTRL_WND_L1 );
	m_FrameWnd->SetIconTID( TID_CTRL_ICON_RADIO_BROAD );	
	m_FrameWnd->SetWndTileSizeLR( 40, 29 );
	m_FrameWnd->SetWndTileSizeTB( 31, 11 );
	m_FrameWnd->SetCaption( G_STRING_CHAR(IDS_BROAD_COMPANY) );	 
	m_FrameWnd->SetTitle( true );
	m_FrameWnd->SetWndProc( this );
	m_FrameWnd->SetVisible( false );
	m_FrameWnd->Initialize();

	//捞固瘤 荤阿屈 
	img9 = (CImageBox9*)nRui->CreateWnd( n_wtImageBox9, WID_None, m_FrameWnd->GetID(), 20 , 50, invenwnd.w - 40 , 60  , false );       	
	img9->SetWndTileSizeLR( 9, 9 );
	img9->SetWndTileSizeTB( 8, 8 );
	img9->SetFocusCheck( false );
	img9->SetClientImage( TID_CTRL_CONTBACK_L1 );
	img9->SetAdjustPosition( 0, 0 );
	
	TCHAR AreaStr[RADIO_BROADCAST_BTN_NUM][64]= {0,};

	Rstrcpy(RWCHAR(AreaStr[0]) , G_STRING_CHAR(IDS_TELECOM) );
	Rstrcpy(RWCHAR(AreaStr[1]) , G_STRING_CHAR(IDS_NETCOM) );

	m_pCloseBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None,m_FrameWnd->GetID(), 165, 6, 13, 13, false );  	
	m_pCloseBtn->SetBtnImage( TID_CTRL_BTN_X, TID_CTRL_BTN_X_OVR, TID_CTRL_BTN_X_CLK, TID_None );

	m_CancelBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None,m_FrameWnd->GetID(), (invenwnd.w/ 2) - (58/2) , 110 , 58, 25, false ); 
	m_CancelBtn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_None );
	m_CancelBtn->SetFontR( n_fRsvFontWndButton );
	m_CancelBtn->SetText( G_STRING_CHAR(IDS_WND_COM_CANCEL));	

	text = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None,m_FrameWnd->GetID(), 20, 30  , 60, 23, false );
	text->SetFontR( n_fRsvFontWndStatic );
//	text->SetTextColor(255,0,0 );	
	text->SetAlignText( n_atCenter, n_atCenter );	
	text->SetText( G_STRING_CHAR(IDS_WND_SVRSEL_SVR) );

	//滚瓢 滴俺 老窜.  

	m_BtnSelect_img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_FrameWnd->GetID(), 20 , 60, invenwnd.w - 40 , 15, false );	
	m_BtnSelect_img->SetClientImage( TID_Server_List_Select_Svr );

	for(int i= 0; i < RADIO_BROADCAST_BTN_NUM ; ++i)
	{
		m_BtnList[ i ] = (CButton*)nRui->CreateWnd( n_wtButton, WID_None , m_FrameWnd->GetID() , 20 , 60 + (25 * i ) , invenwnd.w - 40  , 15 , false );
		m_BtnList[ i ]->SetClientImage( TID_None );
		m_BtnList[ i ]->SetBtnImage( TID_None, TID_None, TID_None , TID_None );	
		m_BtnList[ i ]->SetAlignText( n_atCenter , n_atCenter );
		m_BtnList[ i ]->SetText(AreaStr[i]);
		//m_BtnList[ i ]->SetHID(HID_CARRIER);
	}

}

void CRadioBroadcast::Update()
{
//	hHandle1->PostMessage(WM_SYSCOMMAND , SC_MINIMIZE , NULL ); 

	m_BtnSelect_img->SetVisible( false );
	int i = 0  ; 

	for(i = 0 ;  i < RADIO_BROADCAST_BTN_NUM ; ++i)
	{
		if( m_iSelect == i )
		{
			m_BtnSelect_img->SetWndRepositionR( 20 , 60 + (i * 25)  );
			m_BtnSelect_img->SetVisible( true );
		}
	}
		
	for( i = 0 ; i < 2; ++i)
	{
		HWND hHandle = FindWindow(NULL, m_Wndcheck[i].TileName ); 
		if( hHandle &&  !m_Wndcheck[i].m_IsExplorer ) 
		{
			::SetForegroundWindow(g_RockClient.GetApp());
			m_Wndcheck[i].m_IsExplorer = true;
			PostMessage(hHandle , WM_SYSCOMMAND , SC_MINIMIZE , NULL );
		}
		
		//喘范衬 ?
		if( m_Wndcheck[i].m_IsLive )
		{
			if( timeGetTime() > m_Wndcheck[i].m_dwDelayTime + 10000 )
			{
				m_Wndcheck[i].m_IsLive = false;
			}
		}
	}
}
void CRadioBroadcast::SelectBtn(int Num)
{
	if( Num >  RADIO_BROADCAST_BTN_NUM-1 )
	{
		Num =  RADIO_BROADCAST_BTN_NUM - 1;
	}
	m_iSelect = Num;
}
void CRadioBroadcast::ExploreRun(int Num)
{
	HWND hHandle = FindWindow(NULL,m_Wndcheck[Num].TileName ); 
	if( hHandle  ) 
	{

		PostMessage(hHandle , WM_CLOSE, NULL , NULL ); 
		m_Wndcheck[Num].m_IsLive = false;
		return;
	}
	if(!m_Wndcheck[Num].m_IsLive )
	{
		m_Wndcheck[Num].m_IsExplorer = false; 
		m_Wndcheck[Num].m_dwDelayTime = timeGetTime();
		m_Wndcheck[Num].m_IsLive = true;

		ShellExecute( NULL, "open" , "iexplore",m_Wndcheck[Num].AddressName , NULL, SW_SHOWNORMAL );
		::SetForegroundWindow(g_RockClient.GetApp());
	}
}


void CRadioBroadcast::Proc(SEventMessage* EMsg )
{
	int i = 0;
	if( EMsg->FocusWnd == m_pCloseBtn->GetID()  ||
		EMsg->FocusWnd == m_CancelBtn->GetID()) 
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			Close();
		}
	}	
	for( i = 0 ; i < RADIO_BROADCAST_BTN_NUM; ++i )
	{
		if( m_BtnList[i]->GetID() == EMsg->FocusWnd ) 
		{
			SelectBtn( i);
			if( EMsg->Notify == NM_BTN_CLICK )
			{
				ExploreRun(i);
			}
		}
	}
}