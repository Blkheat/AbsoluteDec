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

#include "AbilityUpWndProc.h"
#include "AbilityUpConfirmWnd.h"
#include "stringmanager.h"


CAbilityUpWnd	AbilityUpWnd;
#define TEXT_COLOR_STATIC 90, 90, 90


//-----------------------------------------------------------------------------
void CAbilityUpWnd::Init()
{
}

//-----------------------------------------------------------------------------
void CAbilityUpWnd::Composition()
{

	CFrameWnd*		fw;
	CButton*		btn;
	CImageBox*		img;


	int center_x	= nRui->Desktop.Width / 2;
	int center_y	= nRui->Desktop.Height / 2;

	SRect aupwnd;
	aupwnd.w	= 256;
	aupwnd.h	= 168;
	aupwnd.x	= center_x - ( aupwnd.w / 2 );
	aupwnd.y	= center_y - ( aupwnd.y	/ 2 );

	///-- Frame Wnd
	fw = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_AbilityUpWnd, WID_None, aupwnd.x, aupwnd.y, aupwnd.w, aupwnd.h, false );
	fw->SetWndTexStyle( fwfTex_Style_Tile9 );
	fw->SetClientImage( TID_CTRL_WND_L1 );
	fw->SetIconTID( TID_CTRL_ICON_STATUS );		
	fw->SetWndTileSizeLR( 40, 29 );
	fw->SetWndTileSizeTB( 31, 11 ); 		
	
	fw->SetTitle( true );
	fw->SetCaption( G_STRING_CHAR( IDS_WND_ABIL_UP ) );
	
	fw->SetWndProc( this );	
	fw->SetVisible( false );
	fw->Initialize();
	
	///---------------------------------------------------------------------------	
	///-- Static
	///---------------------------------------------------------------------------		
	
	///-- Line
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_AbilityUpWnd, 1, 101, 254, 29, false );  
	img ->SetClientImage( TID_CTRL_BACK );
	img->SetFocusCheck( false );

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_AbilityUpWnd, 1, 100, 254, 1, false );  
	img ->SetClientImage( TID_CTRL_LINEDOT );

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_AbilityUpWnd, 1, 130, 254, 1, false );  
	img ->SetClientImage( TID_CTRL_LINEDOT );
	
	///-- Edit BG
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_AbilityUpWnd, 63, 37, 35, 23, false );     
	img ->SetClientImage( TID_CTRL_IMG_ELDTEXT35 );
	
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_AbilityUpWnd, 63, 62, 35, 23, false );     
	img ->SetClientImage( TID_CTRL_IMG_ELDTEXT35 );

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_AbilityUpWnd, 182, 37, 35, 23, false );     
	img ->SetClientImage( TID_CTRL_IMG_ELDTEXT35 );

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_AbilityUpWnd, 182, 62, 35, 23, false );      
	img ->SetClientImage( TID_CTRL_IMG_ELDTEXT35 );

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_AbilityUpWnd, 94, 105, 35, 23, false );      
	img ->SetClientImage( TID_CTRL_IMG_ELDTEXT35 );


	///-----------------------------------------------------------------------
	///-- Static Text : 파워 / 교감
	Static_Text[0]= (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_AbilityUpWnd, 5, 39, 55, 20, false );
	Static_Text[0]->SetFontR( n_fRsvFontWndStatic );
	Static_Text[0]->SetText( G_STRING_CHAR( IDS_CHARSTATE_POWER2 ) );	
	

	///-- Static Text : 지구력 / 지능
	Static_Text[1]= (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_AbilityUpWnd, 123, 39, 55, 20, false );
	Static_Text[1]->SetFontR( n_fRsvFontWndStatic );
	Static_Text[1]->SetText( G_STRING_CHAR( IDS_CHARSTATE_STAMINA2 ) );	
	
	
	///-- Static Text : 체력
	Static_Text[2]= (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_AbilityUpWnd, 5, 64, 55, 20, false );
	Static_Text[2]->SetFontR( n_fRsvFontWndStatic );
	Static_Text[2]->SetText( G_STRING_CHAR( IDS_CHARSTATE_VITAL2 ) );	
	

	///-- Static Text : 민첩
	Static_Text[3]= (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_AbilityUpWnd, 123, 64, 55, 20, false );
	Static_Text[3]->SetFontR( n_fRsvFontWndStatic );	
	Static_Text[3]->SetText( G_STRING_CHAR( IDS_CHARSTATE_DEX2 ) );	
	

	///-- Static Text : 남은 능력업 포인트
	Static_Text[4]= (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_AbilityUpWnd, 7, 107, 80, 20, false );
	Static_Text[4]->SetFontR( n_fRsvFontWndStatic );
	Static_Text[4]->SetText( G_STRING_CHAR( IDS_CHARSTATE_UPPOINT ) );	
	
	///---------------------------------------------------------------------------	
	///-- Control
	///---------------------------------------------------------------------------	
	///-- X	
	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_Aup_CloseBtn, WID_AbilityUpWnd, 237, 6, 13, 13, false );        
	btn->SetBtnImage( TID_CTRL_BTN_X, TID_CTRL_BTN_X_OVR, TID_CTRL_BTN_X_CLK, TID_None );	
	
	///-- 확인 버튼
	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_Aup_OkBtn, WID_AbilityUpWnd, 55, 138, 58, 25, false );
	btn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_None );		
	btn->SetFontR( n_fRsvFontWndButton );   
	btn->SetText( G_STRING_CHAR( IDS_WND_COM_OK ) ); 
	
	///-- 취소 버튼
	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_Aup_CancelBtn, WID_AbilityUpWnd, 143, 138, 58, 25, false );
	btn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_None );		
	btn->SetFontR( n_fRsvFontWndButton );   
	btn->SetText( G_STRING_CHAR( IDS_WND_COM_CANCEL ) ); 


	SRect sTempRect;

	sTempRect.x	= 171;
	sTempRect.y	= 104;
	sTempRect.w	= 72;
	sTempRect.h	= 25;

#ifdef APPLY_ENGLISH_ORIGIN_SIZE
	
	sTempRect.x	= 148;
	sTempRect.w	= 92;

#endif	
	
	///-- 다시 배분
	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_Aup_DistriBtn, WID_AbilityUpWnd, 
									 sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );	
	btn->SetBtnImage( TID_CTRL_BTN72, TID_CTRL_BTN72_OVR, TID_CTRL_BTN72_CLK, TID_None );		
	btn->SetFontR( n_fRsvFontWndButton );   
	btn->SetText( G_STRING_CHAR( IDS_WND_RE_DISTRUCTION) );
	btn->SetHID( HD_UP_REALLOCATION );


	
	//포인트 출력
	Point_Txt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_Aup_PointText, WID_AbilityUpWnd, 96, 107, 30, 20, false );
	Point_Txt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	//Point_Txt->SetClientColor( 180, 175, 160 );
	Point_Txt->SetClientImage( TID_None );
	Point_Txt->SetAlignText( n_atCenter, n_atCenter );
	Point_Txt->SetText( _RT("999") );
	Point_Txt->SetTextColor( 255, 255, 0 );
	
	///-----------------------------------------------------------------------
	

	///-- 파워
	Power_spin = (CSpin*)nRui->CreateWnd( n_wtSpin, WID_Aup_AttSpin, WID_AbilityUpWnd, 71, 37, 50, 24, false );
	Power_spin->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	Power_spin->SetClientImage( TID_None );	
	Power_spin->SetAlignText( n_atRight, n_atCenter );
	Power_spin->SetTextColor( 255, 255, 255 );
	Power_spin->SetBlank( 5, 0 );
	Power_spin->InitSpin( 0, 0, 65535, true, 17, 12  );
	Power_spin->SetUpBtnImage( TID_CTRL_SCR3_UBT, TID_CTRL_SCR3_UBT_OVR, TID_CTRL_SCR3_UBT_CLK, TID_CTRL_SCR3_UBT_DIS );
	Power_spin->SetDownBtnImage( TID_CTRL_SCR3_DBT, TID_CTRL_SCR3_DBT_OVR, TID_CTRL_SCR3_DBT_CLK, TID_CTRL_SCR3_DBT_DIS );
	Power_spin->SetHID( HD_UP_POWER );

	///-- 교감
	Sympathy_spin = (CSpin*)nRui->CreateWnd( n_wtSpin, WID_Aup_MagSpin, WID_AbilityUpWnd, 71, 37, 50, 24, false );
	Sympathy_spin->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	Sympathy_spin->SetClientImage( TID_None );	
	Sympathy_spin->SetAlignText( n_atRight, n_atCenter );
	Sympathy_spin->SetTextColor( 255, 255, 255 );
	Sympathy_spin->SetBlank( 5, 0 );
	Sympathy_spin->InitSpin( 0, 0, 65535, true , 17, 12 );	
	Sympathy_spin->SetUpBtnImage( TID_CTRL_SCR3_UBT, TID_CTRL_SCR3_UBT_OVR, TID_CTRL_SCR3_UBT_CLK, TID_CTRL_SCR3_UBT_DIS );
	Sympathy_spin->SetDownBtnImage( TID_CTRL_SCR3_DBT, TID_CTRL_SCR3_DBT_OVR, TID_CTRL_SCR3_DBT_CLK, TID_CTRL_SCR3_DBT_DIS );
	Sympathy_spin->SetHID( HD_UP_SYMPATHY );
	
	
	///-- 지구력
	Stamina_spin = (CSpin*)nRui->CreateWnd( n_wtSpin, WID_Aup_AgiSpin, WID_AbilityUpWnd, 190, 37, 50, 24, false );
	Stamina_spin->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	Stamina_spin->SetClientImage( TID_None );	
	Stamina_spin->SetAlignText( n_atRight, n_atCenter );
	Stamina_spin->SetTextColor( 255, 255, 255 );
	Stamina_spin->SetBlank( 5, 0 );
	Stamina_spin->InitSpin( 0, 0, 65535, true , 17, 12 );
	Stamina_spin->SetUpBtnImage( TID_CTRL_SCR3_UBT, TID_CTRL_SCR3_UBT_OVR, TID_CTRL_SCR3_UBT_CLK, TID_CTRL_SCR3_UBT_DIS );
	Stamina_spin->SetDownBtnImage( TID_CTRL_SCR3_DBT, TID_CTRL_SCR3_DBT_OVR, TID_CTRL_SCR3_DBT_CLK, TID_CTRL_SCR3_DBT_DIS );
	Stamina_spin->SetHID( HD_UP_FOCUS );

	
	///-- 지능
	Int_spin = (CSpin*)nRui->CreateWnd( n_wtSpin, WID_Aup_ResSpin, WID_AbilityUpWnd, 190, 37, 50, 24, false );
	Int_spin->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	Int_spin->SetClientImage( TID_None );	
	Int_spin->SetAlignText( n_atRight, n_atCenter );
	Int_spin->SetTextColor( 255, 255, 255 );
	Int_spin->SetBlank( 5, 0 ); 
	Int_spin->InitSpin( 0, 0, 65535, true, 17, 12  );
	Int_spin->SetUpBtnImage( TID_CTRL_SCR3_UBT, TID_CTRL_SCR3_UBT_OVR, TID_CTRL_SCR3_UBT_CLK, TID_CTRL_SCR3_UBT_DIS );
	Int_spin->SetDownBtnImage( TID_CTRL_SCR3_DBT, TID_CTRL_SCR3_DBT_OVR, TID_CTRL_SCR3_DBT_CLK, TID_CTRL_SCR3_DBT_DIS );
	Int_spin->SetHID( HD_UP_RESIST );
	
	
	//체력
	Vital_spin = (CSpin*)nRui->CreateWnd( n_wtSpin, WID_Aup_DefSpin, WID_AbilityUpWnd, 71, 63 , 50, 24, false );
	Vital_spin->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	Vital_spin->SetClientImage( TID_None );
	//Vital_spin->SetClientColor( 0, 0, 0 );
	Vital_spin->SetAlignText( n_atRight, n_atCenter );
	Vital_spin->SetTextColor( 255, 255, 255 );
	Vital_spin->SetBlank( 5, 0 );
	Vital_spin->InitSpin( 0, 0, 65535, true, 17, 12 );	
	Vital_spin->SetUpBtnImage( TID_CTRL_SCR3_UBT, TID_CTRL_SCR3_UBT_OVR, TID_CTRL_SCR3_UBT_CLK, TID_CTRL_SCR3_UBT_DIS );
	Vital_spin->SetDownBtnImage( TID_CTRL_SCR3_DBT, TID_CTRL_SCR3_DBT_OVR, TID_CTRL_SCR3_DBT_CLK, TID_CTRL_SCR3_DBT_DIS );
	Vital_spin->SetHID( HD_UP_DEFEND );
	
	//민첩
	Dex_spin = (CSpin*)nRui->CreateWnd( n_wtSpin, WID_Aup_DexSpin, WID_AbilityUpWnd, 190, 63, 50, 24, false );
	Dex_spin->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	Dex_spin->SetClientImage( TID_None );
	//Dex_spin->SetClientColor( 0, 0, 0 );
	Dex_spin->SetAlignText( n_atRight, n_atCenter );
	Dex_spin->SetTextColor( 255, 255, 255 );
	Dex_spin->SetBlank( 5, 0 );
	Dex_spin->InitSpin( 0, 0, 65535, true, 17, 12  );
	Dex_spin->SetUpBtnImage( TID_CTRL_SCR3_UBT, TID_CTRL_SCR3_UBT_OVR, TID_CTRL_SCR3_UBT_CLK, TID_CTRL_SCR3_UBT_DIS );
	Dex_spin->SetDownBtnImage( TID_CTRL_SCR3_DBT, TID_CTRL_SCR3_DBT_OVR, TID_CTRL_SCR3_DBT_CLK, TID_CTRL_SCR3_DBT_DIS );
	Dex_spin->SetHID( HD_UP_REACTION );
	
}

//-----------------------------------------------------------------------------
void CAbilityUpWnd::Update()
{
}

//-----------------------------------------------------------------------------
void CAbilityUpWnd::Proc( SEventMessage* EMsg )
{

	switch( EMsg->FocusWnd )
	{
	case WID_Aup_DistriBtn://다시 배분
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			InitPointUpProc();//초기값으로 다시 셋팅
		}
		break;
	case WID_Aup_OkBtn://배분
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			ValPower	= Power.value - Power.min;
			ValVital	= Vital.value - Vital.min;
			ValStamina	= Stamina.value - Stamina.min;
			ValDex = Dex.value - Dex.min;
			ValSympathy	= Sympathy.value - Sympathy.min;
			ValInt	= Intel.value - Intel.min;

			if( ( ValPower + ValVital + ValStamina + ValDex + ValSympathy + ValInt ) != 0 )
			{
				Rsprintf( RWCHAR(TmpStr), _RT("%s + %d"), G_STRING_CHAR( IDS_CHARSTATE_POWER2 ), ValPower );
				AbilityUpConfirmWnd.PowerTxt->SetText( TmpStr );
				Rsprintf( RWCHAR(TmpStr), _RT("%s + %d"), G_STRING_CHAR( IDS_CHARSTATE_VITAL2 ), ValVital );
				AbilityUpConfirmWnd.VitalTxt->SetText( TmpStr );
				Rsprintf( RWCHAR(TmpStr), _RT("%s + %d"), G_STRING_CHAR( IDS_CHARSTATE_STAMINA2 ), ValStamina );
				AbilityUpConfirmWnd.StaminaTxt->SetText( TmpStr );
				Rsprintf( RWCHAR(TmpStr), _RT("%s + %d"), G_STRING_CHAR( IDS_CHARSTATE_DEX2 ), ValDex );
				AbilityUpConfirmWnd.DexTxt->SetText( TmpStr );
				Rsprintf( RWCHAR(TmpStr), _RT("%s + %d"), G_STRING_CHAR( IDS_CHARSTATE_SYMPATHY2 ), ValSympathy );
				AbilityUpConfirmWnd.SympathyTxt->SetText( TmpStr );
				Rsprintf( RWCHAR(TmpStr), _RT("%s + %d"), G_STRING_CHAR( IDS_CHARSTATE_INTELLI2 ), ValInt );
				AbilityUpConfirmWnd.IntTxt->SetText( TmpStr );
				
				nRui->SendEvent( WID_AbilityUpWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
				nRui->SendEvent( WID_AbilityUpConfirmWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
				nRui->SetModal( WID_AbilityUpConfirmWnd );//모달 처리
			}
			else
			{
				nRui->SendEvent( WID_AbilityUpWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
				nRui->SetModal( WID_None );//모달 처리
				InitPointUpProc();//초기값으로 다시 셋팅
			}
		}
		break;
	case WID_Aup_CancelBtn://배분 취소
	case WID_Aup_CloseBtn://닫기 
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			nRui->SendEvent( WID_AbilityUpWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
			nRui->SetModal( WID_None );//모달 처리
			InitPointUpProc();//초기값으로 다시 셋팅
		}
		break;
	case WID_Aup_AttSpin://파워
		switch( EMsg->Notify )
		{
		case NM_SPIN_UP:
			PointUpProc( &Power, Power_spin, (int)EMsg->fParam );
			break;
		case NM_SPIN_DOWN:
			PointDownProc( &Power, Power_spin, (int)EMsg->fParam );
			break;
		}

		Rsprintf( RWCHAR(TmpStr), _RT("%d"), cLvPoint );
		Point_Txt->SetText( TmpStr );

		break;
	case WID_Aup_DefSpin://체력
		switch( EMsg->Notify )
		{
		case NM_SPIN_UP:
			PointUpProc( &Vital, Vital_spin, (int)EMsg->fParam );
			break;
		case NM_SPIN_DOWN:
			PointDownProc( &Vital, Vital_spin, (int)EMsg->fParam );
			break;
		}
		
		Rsprintf( RWCHAR(TmpStr), _RT("%d"), cLvPoint );
		Point_Txt->SetText( TmpStr );
		
		break;
	case WID_Aup_AgiSpin://지구력
		switch( EMsg->Notify )
		{
		case NM_SPIN_UP:
			PointUpProc( &Stamina, Stamina_spin, (int)EMsg->fParam );
			break;
		case NM_SPIN_DOWN:
			PointDownProc( &Stamina, Stamina_spin, (int)EMsg->fParam );
			break;
		}
		
		Rsprintf( RWCHAR(TmpStr), _RT("%d"), cLvPoint );
		Point_Txt->SetText( TmpStr );

		break;
	case WID_Aup_DexSpin://민첩
		switch( EMsg->Notify )
		{
		case NM_SPIN_UP:
			PointUpProc( &Dex, Dex_spin, (int)EMsg->fParam );
			break;
		case NM_SPIN_DOWN:
			PointDownProc( &Dex, Dex_spin, (int)EMsg->fParam );
			break;
		}
		
		Rsprintf( RWCHAR(TmpStr), _RT("%d"), cLvPoint );
		Point_Txt->SetText( TmpStr );

		break;
	case WID_Aup_MagSpin://교감
		switch( EMsg->Notify )
		{
		case NM_SPIN_UP:
			PointUpProc( &Sympathy, Sympathy_spin, (int)EMsg->fParam );
			break;
		case NM_SPIN_DOWN:
			PointDownProc( &Sympathy, Sympathy_spin, (int)EMsg->fParam );
			break;
		}
		
		Rsprintf( RWCHAR(TmpStr), _RT("%d"), cLvPoint );
		Point_Txt->SetText( TmpStr );

		break;
	case WID_Aup_ResSpin://지능
		switch( EMsg->Notify )
		{
		case NM_SPIN_UP:
			PointUpProc( &Intel, Int_spin, (int)EMsg->fParam );
			break;
		case NM_SPIN_DOWN:
			PointDownProc( &Intel, Int_spin, (int)EMsg->fParam );
			break;
		}
		
		Rsprintf( RWCHAR(TmpStr), _RT("%d"), cLvPoint );
		Point_Txt->SetText( TmpStr );

		break;
	}
}

//-----------------------------------------------------------------------------	
void CAbilityUpWnd::InitPointUpProc()
{
	cLvPoint		= LvPoint;

	Power.value		= Power.min;
	Vital.value	= Vital.min;
	Stamina.value		= Stamina.min;
	Dex.value	= Dex.min;
	Sympathy.value	= Sympathy.min;
	Intel.value	= Intel.min;

	Power.max		= Power.value + cLvPoint;
	Vital.max		= Vital.value + cLvPoint;
	Stamina.max		= Stamina.value + cLvPoint;
	Dex.max	= Dex.value + cLvPoint;
	Sympathy.max	= Sympathy.value + cLvPoint;
	Intel.max		= Intel.value + cLvPoint;

	Power_spin->SetValue( Power.value, Power.min, Power.max );
	Vital_spin->SetValue( Vital.value, Vital.min, Vital.max );
	Stamina_spin->SetValue( Stamina.value, Stamina.min, Stamina.max );
	Dex_spin->SetValue( Dex.value, Dex.min, Dex.max );
	Sympathy_spin->SetValue( Sympathy.value, Sympathy.min, Sympathy.max );
	Int_spin->SetValue( Intel.value, Intel.min, Intel.max );


	Rsprintf( RWCHAR(TmpStr), _RT("%d"), LvPoint );
	Point_Txt->SetText( TmpStr );
}

//-----------------------------------------------------------------------------	
void CAbilityUpWnd::PointUpProc( SValue* attribute, CSpin* spin, int value )
{

#ifndef MIX_ELDASTONE
	if( value > 255 )
	{
		spin->SetValue(255);
		SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_WND_MAX_OVERFLOW) );
		return;
	}
#endif

	attribute->value = value;
	
	



	// min보다 크다는건...레벨업할 수 있는 포인터가 남아있다는 얘기....
	if( cLvPoint > cLvMin )
	{
		cLvPoint--;
	}
	
	Power.max	= Power.value + cLvPoint;
	Vital.max	= Vital.value + cLvPoint;
	Stamina.max	= Stamina.value + cLvPoint;
	Dex.max= Dex.value + cLvPoint;
	Sympathy.max= Sympathy.value + cLvPoint;
	Intel.max	= Intel.value + cLvPoint;

	Power_spin->SetValue( Power.min, Power.max );
	Vital_spin->SetValue( Vital.min, Vital.max );
	Stamina_spin->SetValue( Stamina.min, Stamina.max );
	Dex_spin->SetValue( Dex.min, Dex.max );
	Sympathy_spin->SetValue( Sympathy.min, Sympathy.max );
	Int_spin->SetValue( Intel.min, Intel.max );

}

//-----------------------------------------------------------------------------	
void CAbilityUpWnd::PointDownProc( SValue* attribute, CSpin* spin, int value )
{

	attribute->value = value;

	// max보다 작으면...레벨업 포인터를 원래대로 돌린다.
	if( cLvPoint < cLvMax )
	{
		cLvPoint++;
	}

	Power.max	= Power.value + cLvPoint;
	Vital.max	= Vital.value + cLvPoint;
	Stamina.max	= Stamina.value + cLvPoint;
	Dex.max= Dex.value + cLvPoint;
	Sympathy.max= Sympathy.value + cLvPoint;
	Intel.max	= Intel.value + cLvPoint;

	Power_spin->SetValue( Power.min, Power.max );
	Vital_spin->SetValue( Vital.min, Vital.max );
	Stamina_spin->SetValue( Stamina.min, Stamina.max );
	Dex_spin->SetValue( Dex.min, Dex.max );
	Sympathy_spin->SetValue( Sympathy.min, Sympathy.max );
	Int_spin->SetValue( Intel.min, Intel.max );
}
///---------------------------------------------------------------------------
///-- Select_Race
///---------------------------------------------------------------------------
///-- 각 종족별로 활성되는 SPinBox 차이를 준다.
void CAbilityUpWnd::Select_Race( const WORD aw_race )
{
	///-- 밀레나
	if( aw_race )
	{
		Power_spin->SetEnable( true );
		Power_spin->SetVisible( true );
		Stamina_spin->SetEnable( true );
		Stamina_spin->SetVisible( true );
		
		Sympathy_spin->SetEnable( false );
		Sympathy_spin->SetVisible( false );
		Int_spin->SetEnable( false );
		Int_spin->SetVisible( false );

		Static_Text[0]->SetText( G_STRING_CHAR( IDS_CHARSTATE_POWER2 ) );	
		Static_Text[1]->SetText( G_STRING_CHAR( IDS_CHARSTATE_STAMINA2 ) );	
	}
	///-- 레인 
	else
	{
		Power_spin->SetEnable( false );
		Power_spin->SetVisible( false );
		Stamina_spin->SetEnable( false );
		Stamina_spin->SetVisible( false );

		Sympathy_spin->SetEnable( true );
		Sympathy_spin->SetVisible( true );
		Int_spin->SetEnable( true );
		Int_spin->SetVisible( true );

		Static_Text[0]->SetText( G_STRING_CHAR( IDS_CHARSTATE_SYMPATHY2 ) );
		Static_Text[1]->SetText( G_STRING_CHAR( IDS_CHARSTATE_INTELLI2 ) );	
	}

}

//-----------------------------------------------------------------------------	