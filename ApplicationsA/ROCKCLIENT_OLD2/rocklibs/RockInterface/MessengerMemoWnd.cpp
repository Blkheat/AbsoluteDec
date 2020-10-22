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

#include "UiUtil.h"

#include "..\\..\\global.h"

#include "..\\..\\quadlist.h"
#include "..\\..\\bravolibs\\network\\Protocol.h"
#include "..\\..\\Pc.h"

#include "MessengerMemoWnd.h"
#include "stringmanager.h"

CMessengerMemoWnd g_MessengerMemoWnd;

CMessengerMemoWnd::CMessengerMemoWnd()
{
	m_pMessenMemoBgWnd = NULL;
	m_pMMCloseBtn = NULL;
	m_pMMTitleImgBox = NULL;
	m_pMMTransStateTxt = NULL;
	m_pMMInterDestTxt = NULL;
	m_pMMInMLineEdit = NULL;
	m_pMMOutputTxt = NULL;
	m_pMMScrollHideImgBox = NULL;
	m_pMMConfirmBtn = NULL;

	m_MMState = MMSendState;

	memset(m_strInterName , NULL , MAX_SIZE_512 );
	memset(m_strTransContent, NULL , MAX_SIZE_512 );
	
}

CMessengerMemoWnd::~CMessengerMemoWnd()
{


}

void CMessengerMemoWnd::Init()
{


}

void CMessengerMemoWnd::SetInterName(TCHAR *pStrName)
{
	Rstrcpy( RWCHAR(m_strInterName), RWCHAR(pStrName) );
	m_pMMInterDestTxt->SetText(m_strInterName);
}

void CMessengerMemoWnd::SetTransContent(TCHAR *pTransContent)
{
	Rstrcpy( RWCHAR(m_strTransContent), RWCHAR(pTransContent) );
	m_pMMOutputTxt->SetText(m_strTransContent);
}

void CMessengerMemoWnd::Composition()
{
	int center_x	= nRui->Desktop.Width / 2;
	int center_y	= nRui->Desktop.Height / 2;

	SRect MMemoWnd;
	MMemoWnd.w	= 222;
	MMemoWnd.h	= 217;
	MMemoWnd.x	= center_x - ( MMemoWnd.w / 2 );
	MMemoWnd.y	= center_y - ( MMemoWnd.h / 2 );

	m_pMessenMemoBgWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_MessenMemoWnd, WID_None, 
		MMemoWnd.x, MMemoWnd.y, MMemoWnd.w, MMemoWnd.h, false );
	m_pMessenMemoBgWnd->SetClientImage( TID_MessenMemoBgUI );
	m_pMessenMemoBgWnd->SetCaption( _RT("Messenger Memo Window") );
	m_pMessenMemoBgWnd->SetWndProc( this );
	m_pMessenMemoBgWnd->SetTitle( false );
	m_pMessenMemoBgWnd->SetVisible( false );
	m_pMessenMemoBgWnd->SetZorder( n_zoTop );
	m_pMessenMemoBgWnd->Initialize();

	m_pMMCloseBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_MMCloseBtn, WID_MessenMemoWnd, 
	                                204, 8, 13, 13, false );
    m_pMMCloseBtn->SetClientImage( TID_None );
    m_pMMCloseBtn->SetBtnImage( TID_None, TID_MMCloseBtnFocus, 
							   TID_MMCloseBtnClick, TID_None );

	m_pMMTransStateTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_MMTransStateTxt, WID_MessenMemoWnd, 
	11, 34, 35, 14, false );
	m_pMMTransStateTxt->SetClientImage(TID_None);
	m_pMMTransStateTxt->SetAlignText( n_atLeft, n_atCenter );
	m_pMMTransStateTxt->SetBlank( 5, 0, 0, 0 );		
	m_pMMTransStateTxt->SetText(_RT(""));
	m_pMMTransStateTxt->SetTextColor(255, 255, 255);	
	
	m_pMMInterDestTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_MMInterDestTxt, WID_MessenMemoWnd, 
	61, 34, 131, 14, false );
	m_pMMInterDestTxt->SetClientImage(TID_None);
	m_pMMInterDestTxt->SetAlignText( n_atLeft, n_atCenter );
	m_pMMInterDestTxt->SetBlank( 5, 0, 0, 0 );		
	m_pMMInterDestTxt->SetText(_RT(""));
	m_pMMInterDestTxt->SetTextColor(255, 255, 255);

	m_pMMOutputTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_MMOutputTxt, WID_MessenMemoWnd, 
	9, 68, 185, 96, false );
	m_pMMOutputTxt->SetClientImage(TID_None);
	m_pMMOutputTxt->SetAlignText( n_atLeft, n_atCenter );
	m_pMMOutputTxt->SetMultiLine(true);
	m_pMMOutputTxt->SetText(_RT(""));
	m_pMMOutputTxt->SetTextColor(255, 255, 255);
	m_pMMOutputTxt->SetVisible(false);
	
	
	m_pMMInMLineEdit = (CMLineEditBox*)nRui->CreateWnd( n_wtMLineEditBox, WID_MMInMLineEdit, WID_MessenMemoWnd, 
	                                9, 68, 185, 96, false );
	m_pMMInMLineEdit->SetClientImage( TID_None );
	m_pMMInMLineEdit->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pMMInMLineEdit->SetTextSize( 400 );
	m_pMMInMLineEdit->SetReturnEnterKey( true );
	m_pMMInMLineEdit->SetReturnActionClear( false );
	m_pMMInMLineEdit->InitMLEditBox();
	

	m_pMMScrollHideImgBox = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_MMScrollHideImgBox, WID_MessenMemoWnd, 
	206, 53, 12, 123, false );
	m_pMMScrollHideImgBox->SetClientImage(TID_MMScrollHideImg);

	m_pMMConfirmBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_MMConfirmBtn, WID_MessenMemoWnd, 
	                                171, 181, 47, 28, false );
    m_pMMConfirmBtn->SetClientImage( TID_None );
    m_pMMConfirmBtn->SetBtnImage( TID_None, TID_MMConfirmBtnFocus, 
							   TID_MMConfirmBtnClick, TID_None );
	m_pMMConfirmBtn->SetAlignText( n_atCenter, n_atCenter);
	m_pMMConfirmBtn->SetText(_RT(""));							
	m_pMMConfirmBtn->SetTextColor(50, 50, 125);
}

void CMessengerMemoWnd::Update()
{


}

void CMessengerMemoWnd::Proc( SEventMessage* EMsg )
{
	int	nBuflen = 100;
	
	switch(EMsg->FocusWnd)
	{
		case WID_MMCloseBtn:
			if( EMsg->Notify == NM_BTN_CLICK )
			{
						

			}
			break;
		case WID_MMConfirmBtn:
			if( EMsg->Notify == NM_BTN_CLICK )
			{
				if(m_MMState == MMSendState)						//보내기
				{
					nRui->SendEvent( WID_MMInMLineEdit, n_emGetText, (EPARAM)m_strTransContent, nBuflen, 0, 0 );
					nRui->SendEvent( WID_MMInMLineEdit, n_emClearText, 0, 0, 0, 0 );
					PostMessage( nRui->GetCurrentWndHandle(), WM_USER + 44, NULL, NULL );

					// 필터링
					if( g_RockClient.m_SlangFilter.Filter( ( char * ) m_strTransContent ) )
					{
						memset( m_strTransContent , 0 , sizeof(TCHAR) * 100 );
						Rsprintf( RWCHAR(m_strTransContent) , _RT("%s") , RWCHAR(g_RockClient.m_SlangFilter.GetString()) );
					}
					//'\r'문자를 스트링에서 제거한다.
					DeleteTokenInString(m_strTransContent, _RT('\r'));

				}

				if(m_MMState == MMRecvState)
				{


				}			
			}
			break;
	}
}

void CMessengerMemoWnd::SetMMemoState(UINT uState)
{
	m_MMState = uState;

	m_pMMOutputTxt->SetVisible(false);
	m_pMMInMLineEdit->SetVisible(false);

	switch(m_MMState)
	{
		case MMSendState:
			{
				m_pMMTransStateTxt->SetText( G_STRING_CHAR( IDS_WND_MESSAGE_TO ) );			 
				m_pMMConfirmBtn->SetText( G_STRING_CHAR( IDS_WND_MESSAGE_SEND ) );			 	 
				m_pMMInMLineEdit->SetVisible(true);
				nRui->SendEvent( WID_MMInMLineEdit, n_emSetFocus, 0, 0, 0, 0 );					
			}
			break;
		case MMRecvState:
			{
				m_pMMTransStateTxt->SetText( G_STRING_CHAR( IDS_WND_MESSAGE_FROM ) );			 
				m_pMMConfirmBtn->SetText(G_STRING_CHAR( IDS_WND_COM_CLOSE ));				 
				m_pMMOutputTxt->SetVisible(true);
			}
			break;
	}
}
