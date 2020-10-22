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
//#include "pc.h"
#include "ItemExchangeWndProc.h"


CItemExchangeWnd	ItemExchangeWnd;


//-----------------------------------------------------------------------------
void CItemExchangeWnd::Init()
{
}

//-----------------------------------------------------------------------------
void CItemExchangeWnd::Composition()
{
	/*
	CFrameWnd*		fw;
//	CEditBox*		eb;
	CButton*		btn;
	CTextBox*		tb;
//	CProgressBar*	pb;
//	CSpin*			spin;
//	CStateButton*	sbtn;
//	CScrollBar*		sb;
	
	int center_x	= nRui->Desktop.Width / 2;
	int center_y	= nRui->Desktop.Height / 2;


	SRect exchangewnd;
	exchangewnd.w	= 225;
	exchangewnd.h	= 330;
	exchangewnd.x	= center_x - ( exchangewnd.w / 2 );
	exchangewnd.y	= center_y - ( exchangewnd.h / 2 );
	
	fw = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_ExchangeWnd, WID_None, exchangewnd.x, exchangewnd.y, exchangewnd.w, exchangewnd.h, false );
	fw->SetClientImage( TID_Exchange_Wnd );
	fw->SetCaption( _RT("ItemExchange Window") );
	fw->SetWndProc( this );
	fw->SetFrame( false );
	fw->SetTitle( false );
	fw->SetCloseBtn( false );
	fw->SetVisible( false );
	fw->Initialize();

	

	//확인 버튼
	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_Exchange_OkBtn, WID_ExchangeWnd, 27, 306, 61, 20, false );
	btn->SetBtnImage( TID_None, TID_Exchange_OkBtn_Focus, TID_Exchange_OkBtn_Select, TID_None );
	//취소 버튼
	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_Exchange_CancelBtn, WID_ExchangeWnd, 136, 306, 61, 20, false );
	btn->SetBtnImage( TID_None, TID_Exchange_CancelBtn_Focus, TID_Exchange_CancelBtn_Select, TID_None );
	

	// GIVE
	tb = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_Exchange_GiveMoneyText, WID_ExchangeWnd, 37, 136, 94, 20, false );
	tb->SetClientImage( TID_None );
	tb->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	tb->SetAlignText( n_atCenter, n_atCenter );
	tb->SetText( _RT("") );
	tb->SetTextColor( 255, 255, 0 );

	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_Exchange_GiveBtn, WID_ExchangeWnd, 17, 136, 19, 20, false );
	btn->SetBtnImage( TID_None, TID_None, TID_Exchange_MoneyBtn_Select, TID_None );

	CSlot* GiveSlot[10];
	GiveSlot[0] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Exchange_GiveSlot0, WID_ExchangeWnd, 17, 56, 35, 35, false );
	GiveSlot[0]->SetSlotGroup( n_SGROUP_EXCHANGE_GIVE );
	GiveSlot[1] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Exchange_GiveSlot1, WID_ExchangeWnd, 56, 56, 35, 35, false );
	GiveSlot[1]->SetSlotGroup( n_SGROUP_EXCHANGE_GIVE );
	GiveSlot[2] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Exchange_GiveSlot2, WID_ExchangeWnd, 95, 56, 35, 35, false );
	GiveSlot[2]->SetSlotGroup( n_SGROUP_EXCHANGE_GIVE );
	GiveSlot[3] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Exchange_GiveSlot3, WID_ExchangeWnd, 134, 56, 35, 35, false );
	GiveSlot[3]->SetSlotGroup( n_SGROUP_EXCHANGE_GIVE );
	GiveSlot[4] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Exchange_GiveSlot4, WID_ExchangeWnd, 173, 56, 35, 35, false );
	GiveSlot[4]->SetSlotGroup( n_SGROUP_EXCHANGE_GIVE );
	GiveSlot[5] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Exchange_GiveSlot5, WID_ExchangeWnd, 17, 95, 35, 35, false );
	GiveSlot[5]->SetSlotGroup( n_SGROUP_EXCHANGE_GIVE );
	GiveSlot[6] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Exchange_GiveSlot6, WID_ExchangeWnd, 56, 95, 35, 35, false );
	GiveSlot[6]->SetSlotGroup( n_SGROUP_EXCHANGE_GIVE );
	GiveSlot[7] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Exchange_GiveSlot7, WID_ExchangeWnd, 95, 95, 35, 35, false );
	GiveSlot[7]->SetSlotGroup( n_SGROUP_EXCHANGE_GIVE );
	GiveSlot[8] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Exchange_GiveSlot8, WID_ExchangeWnd, 134, 95, 35, 35, false );
	GiveSlot[8]->SetSlotGroup( n_SGROUP_EXCHANGE_GIVE );
	GiveSlot[9] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Exchange_GiveSlot9, WID_ExchangeWnd, 173, 95, 35, 35, false );
	GiveSlot[9]->SetSlotGroup( n_SGROUP_EXCHANGE_GIVE );

	// TAKE
	tb = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_Exchange_TakeMoneyText, WID_ExchangeWnd, 37, 276, 94, 20, false );
	tb->SetClientImage( TID_None );
	tb->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	tb->SetAlignText( n_atCenter, n_atCenter );
	tb->SetText( _RT("") );
	tb->SetTextColor( 255, 255, 0 );

	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_Exchange_TakeBtn, WID_ExchangeWnd, 17, 276, 19, 20, false );
	btn->SetBtnImage( TID_None, TID_None, TID_Exchange_MoneyBtn_Select, TID_None );

	CSlot* TakeSlot[10];
	TakeSlot[0] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Exchange_TakeSlot0, WID_ExchangeWnd, 17, 196, 35, 35, false );
	TakeSlot[0]->SetSlotGroup( n_SGROUP_EXCHANGE_TAKE );
	TakeSlot[1] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Exchange_TakeSlot1, WID_ExchangeWnd, 56, 196, 35, 35, false );
	TakeSlot[1]->SetSlotGroup( n_SGROUP_EXCHANGE_TAKE );
	TakeSlot[2] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Exchange_TakeSlot2, WID_ExchangeWnd, 95, 196, 35, 35, false );
	TakeSlot[2]->SetSlotGroup( n_SGROUP_EXCHANGE_TAKE );
	TakeSlot[3] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Exchange_TakeSlot3, WID_ExchangeWnd, 134, 196, 35, 35, false );
	TakeSlot[3]->SetSlotGroup( n_SGROUP_EXCHANGE_TAKE );
	TakeSlot[4] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Exchange_TakeSlot4, WID_ExchangeWnd, 173, 196, 35, 35, false );
	TakeSlot[4]->SetSlotGroup( n_SGROUP_EXCHANGE_TAKE );
	TakeSlot[5] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Exchange_TakeSlot5, WID_ExchangeWnd, 17, 235, 35, 35, false );
	TakeSlot[5]->SetSlotGroup( n_SGROUP_EXCHANGE_TAKE );
	TakeSlot[6] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Exchange_TakeSlot6, WID_ExchangeWnd, 56, 235, 35, 35, false );
	TakeSlot[6]->SetSlotGroup( n_SGROUP_EXCHANGE_TAKE );
	TakeSlot[7] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Exchange_TakeSlot7, WID_ExchangeWnd, 95, 235, 35, 35, false );
	TakeSlot[7]->SetSlotGroup( n_SGROUP_EXCHANGE_TAKE );
	TakeSlot[8] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Exchange_TakeSlot8, WID_ExchangeWnd, 134, 235, 35, 35, false );
	TakeSlot[8]->SetSlotGroup( n_SGROUP_EXCHANGE_TAKE );
	TakeSlot[9] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Exchange_TakeSlot9, WID_ExchangeWnd, 173, 235, 35, 35, false );
	TakeSlot[9]->SetSlotGroup( n_SGROUP_EXCHANGE_TAKE );
	*/
}

//-----------------------------------------------------------------------------
void CItemExchangeWnd::Update()
{
}

//-----------------------------------------------------------------------------
void CItemExchangeWnd::Proc( SEventMessage* EMsg )
{
}

//-----------------------------------------------------------------------------
void CItemExchangeWnd::SetWheelValue( short _wheel )
{
}

//-----------------------------------------------------------------------------

