#include "..\\RockPCH.h"

#include "Rui.h"

#include "Define.h"
#include "Util.h"
#include "Render.h"
#include "Fontman.h"
#include "Resource.h"
#include "Chain.h"
#include "Container.h"

#include "..\\..\\global.h"
#include "..\\..\\RockClient.h"

#include "AbilityUpConfirmWnd.h"
#include "AbilityUpWndProc.h"
#include "CharStateWndProc.h"
#include "StringManager.h"

CAbilityUpConfirmWnd	AbilityUpConfirmWnd;



//-----------------------------------------------------------------------------
void CAbilityUpConfirmWnd::Init()
{
}

//-----------------------------------------------------------------------------
void CAbilityUpConfirmWnd::Composition()
{
	CFrameWnd*		fw;
	CButton*		btn;

	int center_x	= nRui->Desktop.Width / 2;
	int center_y	= nRui->Desktop.Height / 2;

	SRect aup_okcancel_wnd;
	aup_okcancel_wnd.w	= 204;
	aup_okcancel_wnd.h	= 200;
	aup_okcancel_wnd.x	= center_x - ( aup_okcancel_wnd.w / 2 );
	aup_okcancel_wnd.y	= center_y - ( aup_okcancel_wnd.h / 2 );
	
	
	
	
	fw = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_AbilityUpConfirmWnd, WID_None, aup_okcancel_wnd.x, aup_okcancel_wnd.y, aup_okcancel_wnd.w, aup_okcancel_wnd.h, false );
	fw->SetWndTexStyle( fwfTex_Style_Tile9 );
	fw->SetClientImage( TID_CTRL_WND_L1 );
	fw->SetIconTID( TID_CTRL_ICON_ALRAM );	
	fw->SetWndTileSizeLR( 40, 29 );
	fw->SetWndTileSizeTB( 31, 11 );	
	fw->SetCaption( G_STRING_CHAR( IDS_WND_ALARM ) );
	fw->SetTitle( true );
	fw->SetZorder( n_zoTop );
	fw->SetWndProc( this );		
	fw->SetVisible( false );
	fw->Initialize();

	
	///-- 확인 버튼
	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_AupCfm_OkBtn, WID_AbilityUpConfirmWnd, 21, 171, 58, 25, false );	
	btn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_CTRL_BTN58_DIS );
	btn->SetFontR( n_fRsvFontWndButton );   
	btn->SetText( G_STRING_CHAR( IDS_WND_COM_OK ) );
	
	///-- 취소 버튼
	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_AupCfm_CancelBtn, WID_AbilityUpConfirmWnd, 121, 171, 58, 25, false );	
	btn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_CTRL_BTN58_DIS );
	btn->SetFontR( n_fRsvFontWndButton );   
	btn->SetText( G_STRING_CHAR( IDS_WND_COM_CANCEL ) );

	PowerTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_AbilityUpConfirmWnd, 40, 40, 125, 12, false );
	PowerTxt->SetFontR( n_fRsvMessageBox );	

	VitalTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_AbilityUpConfirmWnd, 40, 55, 125, 12, false );
	VitalTxt->SetFontR( n_fRsvMessageBox );	

	StaminaTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_AbilityUpConfirmWnd, 40, 70, 125, 12, false );
	StaminaTxt->SetFontR( n_fRsvMessageBox );	

	DexTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_AbilityUpConfirmWnd, 40, 85, 125, 12, false );
	DexTxt->SetFontR( n_fRsvMessageBox );	

	SympathyTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_AbilityUpConfirmWnd, 40, 100, 125, 12, false );
	SympathyTxt->SetFontR( n_fRsvMessageBox );	

	IntTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_AbilityUpConfirmWnd, 40, 115, 125, 12, false );
	IntTxt->SetFontR( n_fRsvMessageBox );	

	DescTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_AbilityUpConfirmWnd, 20, 145, 164, 14, false );
	DescTxt->SetFontR( n_fRsvMessageBox );	
	DescTxt->SetText( G_STRING_CHAR( IDS_WND_ABIL_UP_CONFIRM ) );
	
}

//-----------------------------------------------------------------------------
void CAbilityUpConfirmWnd::Update()
{
}

//-----------------------------------------------------------------------------
void CAbilityUpConfirmWnd::Proc( SEventMessage* EMsg )
{
	switch( EMsg->FocusWnd )
	{
	case WID_AupCfm_OkBtn:
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			// 서버 처리....
			nRui->SendEvent( WID_AbilityUpConfirmWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
			g_RockClient.Send_RpCsPoint( AbilityUpWnd.ValPower, AbilityUpWnd.ValVital, AbilityUpWnd.ValSympathy, AbilityUpWnd.ValInt, AbilityUpWnd.ValStamina, AbilityUpWnd.ValDex );			

			InitTextBox();
			nRui->SetModal( WID_None );//모달 처리
		}
		break;	
	case WID_AupCfm_CancelBtn:
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			nRui->SendEvent( WID_AbilityUpConfirmWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
			InitTextBox();

			nRui->SetModal( WID_None );//모달 처리
		}
		break;
	}
}

//-----------------------------------------------------------------------------	
void CAbilityUpConfirmWnd::InitTextBox()
{

}

//-----------------------------------------------------------------------------	
