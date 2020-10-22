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
#include <shellapi.h>

#include "MessageWndProc.h"
#include "SystemMenuWndProc.h"

#include "stringmanager.h"
CMessageBoxWnd MessageBoxWnd;
//..............................................................................................................
// 초기화
//..............................................................................................................
void CMessageBoxWnd::Init()
{
	m_pParentWndProc = NULL;
	m_bProcessClose = FALSE;
	m_OpenReturnUrl = FALSE;

	memset( m_ReturnURL, 0, sizeof(m_ReturnURL));
}

//..............................................................................................................
// 컨트롤 구성
//..............................................................................................................
void CMessageBoxWnd::Composition()
{
	int center_x	= nRui->Desktop.Width  / 2;
	int center_y	= nRui->Desktop.Height / 2;

	m_R.w	= 200 + 30 ;
	m_R.h	= 100 + 20 ;

	m_R.x	= center_x - ( m_R.w / 2 );
	m_R.y	= center_y - ( m_R.h / 2 );

	m_MessageBoxWnd = ( CFrameWnd * ) 
		nRui->CreateWnd( n_wtFrameWnd , WID_MessageWnd, WID_None, 0 , 0 , nRui->Desktop.Width , nRui->Desktop.Height , false );
	
	m_MessageBoxWnd->SetClientImage( TID_None );
	m_MessageBoxWnd->SetMove( FALSE );
	m_MessageBoxWnd->SetZorder( n_zoTop );

	m_MessageBoxWnd->SetCaption( _RT("MessageBox Window") );
	m_MessageBoxWnd->SetWndProc( this );
	m_MessageBoxWnd->SetTitle( false );
	m_MessageBoxWnd->SetVisible( false );
	m_MessageBoxWnd->Initialize();
	
	m_DialogImage = ( CImageBox * )
		nRui->CreateWnd( n_wtFrameWnd , WID_Message_ImageWnd, WID_MessageWnd, m_R.x , m_R.y , m_R.w , m_R.h , false );

	m_DialogImage->SetClientImage( TID_MessageBox_WIN );	

	m_ContextBox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_Message_Context_Btn , WID_MessageWnd, 
												m_R.x + 46 , m_R.y + 8 , 180 , 100 , false );
	
	m_ContextBox->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_ContextBox->SetClientImage( TID_None );
	m_ContextBox->SetClientColor( 180, 175, 160 );
	m_ContextBox->SetMultiLine( TRUE );
	m_ContextBox->SetTextColor( 100 ,100, 100);

	m_ContextBox->SetAlignText( n_atCenter , n_atCenter );
	m_ContextBox->SetVisible( TRUE );

	m_OKBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_Message_OK_Btn , WID_MessageWnd , 0, 0 , 58 , 25 , false );		
	m_OKBtn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_None );
	m_OKBtn->SetFontR( n_fRsvFontWndButton );
	m_OKBtn->SetText( G_STRING_CHAR( IDS_WND_COM_OK ) );	

	m_CancelBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_Message_Cancel_Btn , WID_MessageWnd , 0, 0 , 58 , 25 , false );	
	m_CancelBtn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_None );
	m_CancelBtn->SetFontR( n_fRsvFontWndButton );
	m_CancelBtn->SetText( G_STRING_CHAR( IDS_WND_COM_CANCEL ) );
}

//..............................................................................................................
// 갱신
//..............................................................................................................
void CMessageBoxWnd::Update()
{
	//..........................................................................................................
	// 시간 처리
	//..........................................................................................................
	if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( timeGetTime() , m_OldTime) , >= , m_LifeTime )
		&& m_Type == MB_TYPE_TIMER )
	{
		SendParent( NM_MSG_TIMEOVER );
		Close();

#ifdef GLOBAL_LOGIN_VERSION		
		if( m_OpenReturnUrl )
		{
			ShellExecute( NULL, "open" , "iexplore", m_ReturnURL , NULL, SW_SHOWNORMAL );
			m_OpenReturnUrl = FALSE;
			memset( m_ReturnURL, 0, sizeof(m_ReturnURL));
		}
#endif

		if(m_bProcessClose)
		{
			SendMessage( g_RockClient.GetApp() , WM_CLOSE, NULL, 1 );
			m_bProcessClose = FALSE;
		}
	}
}

void CMessageBoxWnd::SendParent(UINT Notify)
{
	if( m_pParentWndProc != NULL )
	{
		SEventMessage Msg;
		
		memset( &Msg , 0 , sizeof( SEventMessage ) );
		
		Msg.FocusWnd = WID_MessageWnd;
		Msg.Notify   = Notify;
		
		m_pParentWndProc->Proc( &Msg );
	}
}

void CMessageBoxWnd::SetRurnURL(char * pstrReturnUrl)
{
	m_OpenReturnUrl = FALSE;
	memset( m_ReturnURL, 0, sizeof(m_ReturnURL));
	
	if( pstrReturnUrl == NULL )
	{
		return;
	}

	SAFE_STR_CPY( m_ReturnURL, pstrReturnUrl, 100);

	if( m_ReturnURL[0] == '\0' )
	{
		return;
	}

	m_OpenReturnUrl = TRUE;
}

void CMessageBoxWnd::Close(void)
{
	nRui->SendEvent( WID_MessageWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	m_OKBtn->SetWndRepositionR( 0  , 0 );
	m_CancelBtn->SetWndRepositionR( 0 , 0 );
}

//..............................................................................................................
// 열기
//..............................................................................................................
void CMessageBoxWnd::Open(void *pContext,MESSAGEBOX_TYPE Type,CWndProc *pParent, BOOL bProcessClose )
{
	m_ContextBox->SetText( pContext );

	m_CancelBtn->SetVisible( FALSE );
	m_OKBtn->SetVisible( FALSE );

	switch( Type )
	{
		case MB_TYPE_NONE:
			break;	

		case MB_TYPE_OK:

			m_OKBtn->SetWndRepositionR( m_R.x + 70 + 15 , m_R.y + 68 + 20 );
			m_OKBtn->SetVisible( TRUE );
			break;	

		case MB_TYPE_CANCEL:
		
	
			m_OKBtn->SetWndRepositionR( m_R.x + 34 +15 , m_R.y + 68 + 20  );
			m_CancelBtn->SetWndRepositionR( m_R.x + 110 +15, m_R.y + 68 + 20 );
	
			
			m_OKBtn->SetVisible( TRUE );
			m_CancelBtn->SetVisible( TRUE );
			break;	
	}

	m_Type = Type;
	m_pParentWndProc = pParent;
	m_bProcessClose  = bProcessClose;

	nRui->SendEvent( WID_MessageWnd, n_emSetVisible, (EPARAM)TRUE , 0, 0, 0 );
}

//..............................................................................................................
// 열기
//..............................................................................................................
void CMessageBoxWnd::Open(void *pContext,DWORD Time,CWndProc *pParent, BOOL bProcessClose)
{
	m_ContextBox->SetText( pContext );

	m_CancelBtn->SetVisible( FALSE );
	m_OKBtn->SetVisible( FALSE );

	m_LifeTime       = Time;
	m_OldTime        = timeGetTime();
	m_pParentWndProc = pParent;

	m_Type           = MB_TYPE_TIMER;
	m_bProcessClose  = bProcessClose; 

	nRui->SendEvent( WID_MessageWnd, n_emSetVisible, (EPARAM)TRUE , 0, 0, 0 );
}

//..............................................................................................................
// 열기
//..............................................................................................................
void CMessageBoxWnd::Open(void *pContext)
{
	m_ContextBox->SetText( pContext );

	m_CancelBtn->SetVisible( FALSE );
	m_OKBtn    ->SetVisible( FALSE );

	m_pParentWndProc = NULL;

	m_Type           = MB_TYPE_NONE;

	nRui->SendEvent( WID_MessageWnd, n_emSetVisible, (EPARAM)TRUE , 0, 0, 0 );
}


//..............................................................................................................
// 메세지 처리
//..............................................................................................................
void CMessageBoxWnd::Proc( SEventMessage* EMsg )
{
	
	switch( EMsg->FocusWnd )
	{
		//......................................................................................................
		// OK 버튼
		//......................................................................................................
		case WID_Message_OK_Btn:

			if( EMsg->Notify == NM_BTN_CLICK )
			{
				SendParent( NM_MSG_OK );
				Close();
				if(m_bProcessClose)
				{
					SystemMenuWnd.GameLogOff();
					m_bProcessClose = FALSE;
				}
			}
			
			break;

		//......................................................................................................
		// Cancel 버튼
		//......................................................................................................
		case WID_Message_Cancel_Btn:

			if( EMsg->Notify == NM_BTN_CLICK )
			{
				SendParent( NM_MSG_CANCEL );

				Close();
			}
			
			break;
	}
	
}

