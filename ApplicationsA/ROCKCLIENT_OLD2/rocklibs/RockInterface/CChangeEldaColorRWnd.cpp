#include "..\\RockPCH.h"
#include "Rui.h"
#include "..\\..\\brother.h"

#include "Define.h"
#include "Util.h"
#include "Render.h"
#include "Fontman.h"
#include "Resource.h"
#include "Chain.h"
#include "Container.h"

#include "..\\..\\global.h"
#include "..\\..\\RockClient.h"

#include "StringManager.h"
#include "CChangeEldaColorRWnd.h"
#include "MessageWndProc.h"
#include "..\\..\\StringManager.h"

CChangeEldaColorRWnd * g_pChangeEldaColorRWnd = NULL;

CChangeEldaColorRWnd::CChangeEldaColorRWnd()
{
	m_pCRFrameWnd = NULL;

	m_ChangeEldaSlot = NULL;
	memset( &m_ChangeEldaItem, 0, sizeof(SPcItem) );	
	
	m_pResultTxt = NULL;
	m_pCloseBtn = NULL;			
	m_pConfirmBtn = NULL;
	m_pIConX = NULL;
	
	m_IsSuccess = FALSE;
}

CChangeEldaColorRWnd::~CChangeEldaColorRWnd()
{

}
	
void CChangeEldaColorRWnd::Init()
{

}

void CChangeEldaColorRWnd::Composition()
{
	CButton*		btn = NULL;
	CTextBox*		tbox = NULL;
	CImageBox*		img = NULL;
	CImageBox9*		black_img9 = NULL;
	
	int center_x	= nRui->Desktop.Width / 2;
	int center_y	= nRui->Desktop.Height / 2;	
	
	SRect enchantwnd;
	enchantwnd.w	= 204;
	enchantwnd.h	= 170;
	enchantwnd.x	= center_x - ( enchantwnd.w / 2 );
	enchantwnd.y	= center_y - ( enchantwnd.h / 2 );

	m_pCRFrameWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_None, WID_None, 
		enchantwnd.x, enchantwnd.y, enchantwnd.w, enchantwnd.h, false );
	m_pCRFrameWnd->SetWndTexStyle( fwfTex_Style_Tile9 );
	m_pCRFrameWnd->SetClientImage( TID_CTRL_WND_L1 );
	m_pCRFrameWnd->SetIconTID( TID_CTRL_ICON_INVEN );	
	m_pCRFrameWnd->SetWndTileSizeLR( 40, 29 );
	m_pCRFrameWnd->SetWndTileSizeTB( 31, 11 );

	m_pCRFrameWnd->SetCaption( G_STRING_CHAR( IDS_WND_ELDACHANGE_COLOR ) );		///-- HAN_MSG			
	m_pCRFrameWnd->SetTitle( true );	
	m_pCRFrameWnd->SetWndProc( this );
	m_pCRFrameWnd->SetVisible( false );
	m_pCRFrameWnd->SetMove( false );
	m_pCRFrameWnd->Initialize();

	m_pCloseBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, m_pCRFrameWnd->GetID(), 
									 185, 6, 13, 13, false );	
	m_pCloseBtn->SetBtnImage( TID_CTRL_BTN_X, TID_CTRL_BTN_X_OVR, TID_CTRL_BTN_X_CLK, TID_None );

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pCRFrameWnd->GetID(), 1, 31, 202, 35, false );
	img->SetClientImage( TID_CTRL_BACK );
	img->SetFocusCheck( false );
	
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pCRFrameWnd->GetID(), 1, 30, 202, 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );
	
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pCRFrameWnd->GetID(), 1, 66, 202, 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );

	m_pResultTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pCRFrameWnd->GetID(), 
											   1, 43, 202, 17, false );
	m_pResultTxt->SetFontR( n_fRsvFontWndStatic );	
	m_pResultTxt->SetAlignText( n_atCenter, n_atTop );
	m_pResultTxt->SetTextColor( 255, 50, 50 ); 
	m_pResultTxt->SetMultiLine( true );
	m_pResultTxt->SetLineInterval( 5 );
	m_pResultTxt->SetMultiLineAsign(true);   
	m_pResultTxt->SetText( "" );	

	///-- Slot
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pCRFrameWnd->GetID(), 84, 76, 35, 35, false );         
	img ->SetClientImage( TID_CTRL_SLOT_SUNKEN );	
	
	m_ChangeEldaSlot = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, m_pCRFrameWnd->GetID(), 
		84, 76, 35, 35, false );	
	m_ChangeEldaSlot->PcItem = &m_ChangeEldaItem;	
	m_ChangeEldaSlot->SetSlotGroup( n_SGROUP_TUNING );
	m_ChangeEldaSlot->SetHID( HID_ITEMTIP );

	m_pIConX = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pCRFrameWnd->GetID(), 
				88, 80, 27, 27, false );
	m_pIConX->SetClientImage( TID_C_WND_ELDA_SB_X );
	m_pIConX->SetVisible( true );

	m_pConfirmBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, m_pCRFrameWnd->GetID(), 
											   74, 126, 58, 25, false );
	m_pConfirmBtn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_CTRL_BTN58_DIS );
	m_pConfirmBtn->SetFontR( n_fRsvFontWndButton );   
	m_pConfirmBtn->SetText( G_STRING_CHAR( IDS_WND_COM_OK ) );
}

void CChangeEldaColorRWnd::Update()
{

}

void CChangeEldaColorRWnd::Proc( SEventMessage* EMsg )
{
	if( EMsg->FocusWnd == m_pCloseBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			Close();
		}
	}

	if( EMsg->FocusWnd == m_pConfirmBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			Close();
		}
	}
}

void CChangeEldaColorRWnd::Open(BOOL IsSuccess)
{
	m_IsSuccess = IsSuccess;

	if( m_IsSuccess )
	{
		m_pResultTxt->SetTextColor( 50, 50, 255 );
		m_pResultTxt->SetText( G_STRING_CHAR( IDS_SYSPR_SUCCE_CHANGECOLOR ) );		///-- HAN_MSG

		m_pIConX->SetVisible( false );
		
	}
	else
	{
		m_pResultTxt->SetTextColor( 255, 50, 50 );
		m_pResultTxt->SetText( G_STRING_CHAR( IDS_SYSPR_FAIL_CHANGECOLOR ) );		///-- HAN_MSG

		m_pIConX->SetVisible( true );		
	}

	nRui->SendEvent( m_pCRFrameWnd->GetID(), n_emSetVisible, (EPARAM)true, 0, 0, 0 );
}

void CChangeEldaColorRWnd::Close()
{
	nRui->SendEvent( m_pCRFrameWnd->GetID(), n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	m_IsSuccess = FALSE;
}
	
void CChangeEldaColorRWnd::SetPcItem( SPcItem * pPcItem )
{
	static char strString[256] = "";	
	
	if( pPcItem )
	{
		memcpy( &m_ChangeEldaItem, pPcItem, sizeof(SPcItem) );
		m_ChangeEldaItem.Amount = 1;
	}
}

void CChangeEldaColorRWnd::RePosition( int nXPos, int nYPos )
{
	m_pCRFrameWnd->Reposition( nXPos, nYPos );
}