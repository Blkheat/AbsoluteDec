//----------------------------------------------------------------------------------------------------------------------
// 파일명 : CharCreateWndProc.cpp
//----------------------------------------------------------------------------------------------------------------------
#include "CharCreateWndProc.h"
#include "Rui.h"
#include "StringManager.h"
#include "MessageBox.h"


CharCreateWndProc	g_CharCreateWndProc;

#ifdef DECO_RENEWAL_MJH
CharCreateHairProc	g_CharCreateHairProc;
#endif // DECO_RENEWAL_MJH


CharCreateWndProc::CharCreateWndProc()
{
	m_pFrameWnd = NULL;
	m_Create_Millena_Btn = NULL;
	m_Create_Rain_Btn = NULL;
	m_nationtext_millena = NULL;
	m_nationtext_rain = NULL;
}

CharCreateWndProc::~CharCreateWndProc()
{

}

void CharCreateWndProc::Composition()
{
	SRect FrameRect;

	int nCen_x	= nRui->Desktop.Width / 2;

	FrameRect.w = 172;
	FrameRect.h = 84;

	FrameRect.x = nCen_x - ( FrameRect.w / 2 );
	FrameRect.y = 555;
	
	m_pFrameWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_None, WID_None, 
									FrameRect.x , FrameRect.y, FrameRect.w, FrameRect.h , false );
	
	m_pFrameWnd->SetClientImage( TID_None );
	m_pFrameWnd->SetCaption( _RT("") );
	m_pFrameWnd->SetWndTexStyle( fwfTex_Style_Tile9 );
	m_pFrameWnd->SetClientImage( TID_CTRL_WND_NL1 );
	m_pFrameWnd->SetWndTileSizeLR( 5, 5 );
	m_pFrameWnd->SetWndTileSizeTB( 5, 5 );	
	m_pFrameWnd->SetWndProc( this );
	m_pFrameWnd->SetTitle( false );	
	m_pFrameWnd->SetVisible( false );
	m_pFrameWnd->SetMove( false );	
	m_pFrameWnd->SetZorder( n_zoTop );
	m_pFrameWnd->Initialize();

	//..........................................................................................................
	// 캐릭터 생성 밀레나 버튼
	//..........................................................................................................
	m_Create_Millena_Btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None , m_pFrameWnd->GetID(),
													  20, 4, 60, 76, false );	
	m_Create_Millena_Btn->SetClientImage( TID_None );
	m_Create_Millena_Btn->SetBtnImage( TID_CharCreate_Millena, TID_CharCreate_Millena_On , 
									   TID_CharCreate_Millena_Da, TID_None );
	
	//..........................................................................................................
	// 캐릭터 생성 레인 버튼
	//..........................................................................................................
	m_Create_Rain_Btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None , m_pFrameWnd->GetID(),
												   92, 4, 59, 76, false );	
	m_Create_Rain_Btn->SetClientImage( TID_None );
	m_Create_Rain_Btn->SetBtnImage( TID_CharCreate_Rain, TID_CharCreate_Rain_On , 
									TID_CharCreate_Rain_Da , TID_None );

	///-- 국가(Static)
	m_nationtext_millena= (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pFrameWnd->GetID(),
													  18, 36, 69, 17, false );
	m_nationtext_millena->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdOutline );
	m_nationtext_millena->SetTextColor( 255, 255, 200 );
	m_nationtext_millena->SetClientImage( TID_None );			
	m_nationtext_millena->SetAlignText( n_atCenter, n_atCenter );
	m_nationtext_millena->SetText( G_STRING_CHAR( IDS_MILLENA ) ); 
	m_nationtext_millena->SetFocusCheck( false );
	
	m_nationtext_rain= (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pFrameWnd->GetID(),
													87, 36, 69, 17, false );
	m_nationtext_rain->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdOutline );
	m_nationtext_rain->SetTextColor( 255, 255, 200 );
	m_nationtext_rain->SetClientImage( TID_None );			
	m_nationtext_rain->SetAlignText( n_atCenter, n_atCenter );
	m_nationtext_rain->SetText( G_STRING_CHAR( IDS_RAIN ) ); 
	m_nationtext_rain->SetFocusCheck( false );

}

void CharCreateWndProc::Update()
{
	if( (nRui->GetEMsg()->Mouse.Action == MA_LPRESS) ||
		(nRui->GetEMsg()->Mouse.Action == MA_LDRAG) )
	{
		if( !CheckFocusWnd(nRui->GetFocusWnd()) ) 
		{	
		   Close();		
		}		
	}
}

bool CharCreateWndProc::CheckFocusWnd( UINT  FocusWnd )
{
   if( FocusWnd == m_pFrameWnd->GetID() )
	return true;
   
   CButton *pButton = CharSelectWnd.GetSelect_Create_Btn();
   if(pButton)
   {
	  if( FocusWnd == pButton->GetID() )
		return true;
   }

   CWnd * Twnd = Container->Get( FocusWnd );
   if(Twnd)
   {
	  WNDID wndid = Twnd->GetParentID();
	  if( wndid == m_pFrameWnd->GetID() )
		  return true;
   }

   return false;
}

void CharCreateWndProc::Proc( SEventMessage* EMsg )
{
	
	if( EMsg->FocusWnd == m_Create_Millena_Btn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			Close();
			CharSelectWnd.StartMillenaCameraWalkPostion();
		}
	}

	if( EMsg->FocusWnd == m_Create_Rain_Btn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			Close();
			CharSelectWnd.StartRainCameraWalkPosition();
		}
	}
	
}

void CharCreateWndProc::Open()
{
	nRui->SendEvent( m_pFrameWnd->GetID(), n_emSetVisible, (EPARAM)true, 0, 0, 0 );	
}

void CharCreateWndProc::Close()
{
	nRui->SendEvent( m_pFrameWnd->GetID(), n_emSetVisible, (EPARAM)false, 0, 0, 0 );	
}

CCharSelectWnd::CCharSelectWnd()
{
	m_State     = CSW_SELECT_NONE;

	m_SelectWnd = NULL;

	m_SelectNum = 0;

	//..........................................................................................................
	// 머리 테이블 리스트 만들기
	//..........................................................................................................
	// 밀레나 남자 머리
	m_MillenaHairCodeTable[ 0 ][ 0 ] = 10000;
	m_MillenaHairCodeTable[ 0 ][ 1 ] = 10001;
	m_MillenaHairCodeTable[ 0 ][ 2 ] = 10002;
	m_MillenaHairCodeTable[ 0 ][ 3 ] = 10005;
	m_MillenaHairCodeTable[ 0 ][ 4 ] = 10008;
	m_MillenaHairCodeTable[ 0 ][ 5 ] = 10009;
	m_MillenaHairCodeTable[ 0 ][ 6 ] = 10010;
	m_MillenaHairCodeTable[ 0 ][ 7 ] = 10011;
	m_MillenaHairCodeTable[ 0 ][ 8 ] = 10013;
	m_MillenaHairCodeTable[ 0 ][ 9 ] = 10014;

	
	// 밀레나 여자 머리
	m_MillenaHairCodeTable[ 1 ][ 0 ] = 10100;
	m_MillenaHairCodeTable[ 1 ][ 1 ] = 10103;
	m_MillenaHairCodeTable[ 1 ][ 2 ] = 10104;
	m_MillenaHairCodeTable[ 1 ][ 3 ] = 10106;
	m_MillenaHairCodeTable[ 1 ][ 4 ] = 10107;
	m_MillenaHairCodeTable[ 1 ][ 5 ] = 10108;
	m_MillenaHairCodeTable[ 1 ][ 6 ] = 10110;
	m_MillenaHairCodeTable[ 1 ][ 7 ] = 10111;
	m_MillenaHairCodeTable[ 1 ][ 8 ] = 10112;
	m_MillenaHairCodeTable[ 1 ][ 9 ] = 10113;
	
	// 레인 남자 머리
	m_RainHairCodeTable[ 0 ][ 0 ] = 10003;
	m_RainHairCodeTable[ 0 ][ 1 ] = 10002;
	m_RainHairCodeTable[ 0 ][ 2 ] = 10004;
	m_RainHairCodeTable[ 0 ][ 3 ] = 10006;
	m_RainHairCodeTable[ 0 ][ 4 ] = 10007;
	m_RainHairCodeTable[ 0 ][ 5 ] = 10009;
	m_RainHairCodeTable[ 0 ][ 6 ] = 10012;
	m_RainHairCodeTable[ 0 ][ 7 ] = 10011;
	m_RainHairCodeTable[ 0 ][ 8 ] = 10008;
	m_RainHairCodeTable[ 0 ][ 9 ] = 10013;

	// 레인 여자 머리
	m_RainHairCodeTable[ 1 ][ 0 ] = 10101;
	m_RainHairCodeTable[ 1 ][ 1 ] = 10102;
	m_RainHairCodeTable[ 1 ][ 2 ] = 10105;
	m_RainHairCodeTable[ 1 ][ 3 ] = 10106;
	m_RainHairCodeTable[ 1 ][ 4 ] = 10108;
	m_RainHairCodeTable[ 1 ][ 5 ] = 10109;
	m_RainHairCodeTable[ 1 ][ 6 ] = 10111;
	m_RainHairCodeTable[ 1 ][ 7 ] = 10112;
	m_RainHairCodeTable[ 1 ][ 8 ] = 10113;
	m_RainHairCodeTable[ 1 ][ 9 ] = 10114;

	//..........................................................................................................
	// 얼굴 테이블 리스트 만들기
	//..........................................................................................................
	// 밀레나 남자 얼굴
	m_MillenaFaceCodeTable[ 0 ][ 0 ] = 10200;
	m_MillenaFaceCodeTable[ 0 ][ 1 ] = 10201;
	m_MillenaFaceCodeTable[ 0 ][ 2 ] = 10202;
	m_MillenaFaceCodeTable[ 0 ][ 3 ] = 10203;
	m_MillenaFaceCodeTable[ 0 ][ 4 ] = 10206;
	m_MillenaFaceCodeTable[ 0 ][ 5 ] = 10208;

	// 밀레나 여자 얼굴
	m_MillenaFaceCodeTable[ 1 ][ 0 ] = 10300;
	m_MillenaFaceCodeTable[ 1 ][ 1 ] = 10301;
	m_MillenaFaceCodeTable[ 1 ][ 2 ] = 10302;
	m_MillenaFaceCodeTable[ 1 ][ 3 ] = 10305;
	m_MillenaFaceCodeTable[ 1 ][ 4 ] = 10307;
	m_MillenaFaceCodeTable[ 1 ][ 5 ] = 10309;

	// 레인 남자 얼굴
	m_RainFaceCodeTable[ 0 ][ 0 ] = 10204;
	m_RainFaceCodeTable[ 0 ][ 1 ] = 10201;
	m_RainFaceCodeTable[ 0 ][ 2 ] = 10202;
	m_RainFaceCodeTable[ 0 ][ 3 ] = 10207;
	m_RainFaceCodeTable[ 0 ][ 4 ] = 10205;
	m_RainFaceCodeTable[ 0 ][ 5 ] = 10206;
	
	// 레인 여자 얼굴
	m_RainFaceCodeTable[ 1 ][ 0 ] = 10304;
	m_RainFaceCodeTable[ 1 ][ 1 ] = 10300;
	m_RainFaceCodeTable[ 1 ][ 2 ] = 10306;
	m_RainFaceCodeTable[ 1 ][ 3 ] = 10305;
	m_RainFaceCodeTable[ 1 ][ 4 ] = 10308;
	m_RainFaceCodeTable[ 1 ][ 5 ] = 10309;
	
	m_ISChangeSkyBox = TRUE;

	m_SEQState       = SEQ_NONE;
	pCharList[0] = pCharList[ 1 ] = pCharList[ 2 ] = pCharList[ 3 ] = pCharList[ 4 ] = NULL;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef DECO_RENEWAL_MJH

CharCreateHairProc::CharCreateHairProc()
{
	m_pFrameWnd = NULL;
	m_Create_Hair1 = NULL;
	m_Create_Hair2 = NULL;
	m_Create_Hair3 = NULL;
	m_Create_Hair4 = NULL;
	m_Create_Hair5 = NULL;
	m_Create_Hair6 = NULL;
}

CharCreateHairProc::~CharCreateHairProc()
{

}

void CharCreateHairProc::Composition()
{
	SRect FrameRect;

	int nCen_x	= nRui->Desktop.Width / 2;

	FrameRect.w = 80;
	FrameRect.h = 120;

	FrameRect.x = nCen_x - ( FrameRect.w / 2 );
	FrameRect.y = 555;
	
	m_pFrameWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_None, WID_None, 
									FrameRect.x , FrameRect.y, FrameRect.w, FrameRect.h , false );
	
	m_pFrameWnd->SetClientImage( TID_None );
	m_pFrameWnd->SetCaption( _RT("") );
	m_pFrameWnd->SetWndTexStyle( fwfTex_Style_Tile9 );
	m_pFrameWnd->SetClientImage( TID_CTRL_WND_NL1 );
	m_pFrameWnd->SetWndTileSizeLR( 5, 5 );
	m_pFrameWnd->SetWndTileSizeTB( 5, 5 );	
	m_pFrameWnd->SetWndProc( this );
	m_pFrameWnd->SetTitle( false );	
	m_pFrameWnd->SetVisible( false );
	m_pFrameWnd->SetMove( false );	
	m_pFrameWnd->SetZorder( n_zoTop );
	m_pFrameWnd->Initialize();

	m_Create_Hair1 = (CButton*)nRui->CreateWnd( n_wtButton, WID_None , m_pFrameWnd->GetID(),
													  5, 5, 20, 20, false );	
	m_Create_Hair1->SetClientImage( TID_None );
	m_Create_Hair1->SetBtnImage( TID_CharCreate_Millena, TID_CharCreate_Millena_On , 
									   TID_CharCreate_Millena_Da, TID_None );
	
/*
	//..........................................................................................................
	// 캐릭터 생성 밀레나 버튼
	//..........................................................................................................
	m_Create_Millena_Btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None , m_pFrameWnd->GetID(),
													  20, 4, 60, 76, false );	
	m_Create_Millena_Btn->SetClientImage( TID_None );
	m_Create_Millena_Btn->SetBtnImage( TID_CharCreate_Millena, TID_CharCreate_Millena_On , 
									   TID_CharCreate_Millena_Da, TID_None );
	
	//..........................................................................................................
	// 캐릭터 생성 레인 버튼
	//..........................................................................................................
	m_Create_Rain_Btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None , m_pFrameWnd->GetID(),
												   92, 4, 59, 76, false );	
	m_Create_Rain_Btn->SetClientImage( TID_None );
	m_Create_Rain_Btn->SetBtnImage( TID_CharCreate_Rain, TID_CharCreate_Rain_On , 
									TID_CharCreate_Rain_Da , TID_None );

	///-- 국가(Static)
	m_nationtext_millena= (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pFrameWnd->GetID(),
													  18, 36, 69, 17, false );
	m_nationtext_millena->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdOutline );
	m_nationtext_millena->SetTextColor( 255, 255, 200 );
	m_nationtext_millena->SetClientImage( TID_None );			
	m_nationtext_millena->SetAlignText( n_atCenter, n_atCenter );
	m_nationtext_millena->SetText( G_STRING_CHAR( IDS_MILLENA ) ); 
	m_nationtext_millena->SetFocusCheck( false );
	
	m_nationtext_rain= (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pFrameWnd->GetID(),
													87, 36, 69, 17, false );
	m_nationtext_rain->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdOutline );
	m_nationtext_rain->SetTextColor( 255, 255, 200 );
	m_nationtext_rain->SetClientImage( TID_None );			
	m_nationtext_rain->SetAlignText( n_atCenter, n_atCenter );
	m_nationtext_rain->SetText( G_STRING_CHAR( IDS_RAIN ) ); 
	m_nationtext_rain->SetFocusCheck( false );
*/
}

void CharCreateHairProc::Update()
{
	
	if( (nRui->GetEMsg()->Mouse.Action == MA_LPRESS) ||
		(nRui->GetEMsg()->Mouse.Action == MA_LDRAG) )
	{
		if( !CheckFocusWnd(nRui->GetFocusWnd()) ) 
		{	
		   Close();		
		}		
	}
	
}

bool CharCreateHairProc::CheckFocusWnd( UINT  FocusWnd )
{
	
   if( FocusWnd == m_pFrameWnd->GetID() )
	return true;
 
   CButton *pButton = CharSelectWnd.GetSelect_Create_Btn();
   if(pButton)
   {
	  if( FocusWnd == pButton->GetID() )
		return true;
   }
  
   CWnd * Twnd = Container->Get( FocusWnd );
   if(Twnd)
   {
	  WNDID wndid = Twnd->GetParentID();
	  if( wndid == m_pFrameWnd->GetID() )
		  return true;
   }
   	
   return false;
}

void CharCreateHairProc::Proc( SEventMessage* EMsg )
{
	if( EMsg->FocusWnd == m_Create_Hair1->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			
		}
	}
/*
	if( EMsg->FocusWnd == m_Create_Hair2->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
		
		}
	}

	if( EMsg->FocusWnd == m_Create_Hair3->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
		
		}
	}

	if( EMsg->FocusWnd == m_Create_Hair4->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
		
		}
	}

	if( EMsg->FocusWnd == m_Create_Hair5->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
		
		}
	}

	if( EMsg->FocusWnd == m_Create_Hair6->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
		
		}
	}
	*/
}

void CharCreateHairProc::Open()
{
	//nRui->SendEvent( m_pFrameWnd->GetID(), n_emSetVisible, (EPARAM)true, 0, 0, 0 );
	bool IsView = m_pFrameWnd->IsVisible();
	IsView = !IsView;
	if( IsView )
	{ 
		nRui->SendEvent( m_pFrameWnd->GetID(), n_emSetVisible, (EPARAM)true, 0, 0, 0 );	
	}
	else
	{
		nRui->SendEvent( m_pFrameWnd->GetID(), n_emSetVisible, (EPARAM)false, 0, 0, 0 );	
	}
}

void CharCreateHairProc::Close()
{
	nRui->SendEvent( m_pFrameWnd->GetID(), n_emSetVisible, (EPARAM)false, 0, 0, 0 );	
}

#endif// DECO_RENEWAL_MJH