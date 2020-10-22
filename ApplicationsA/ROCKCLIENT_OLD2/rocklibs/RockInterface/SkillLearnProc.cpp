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

#include "..\\..\\quadlist.h"
#include "..\\..\\bravolibs\\network\\Protocol.h"
#include "..\\..\\Pc.h"

#include "SkillLearnProc.h"

#include "stringmanager.h"
#include "InventoryWndProc.h"

CSkillLearnWnd	SkillLearnWnd;





//-----------------------------------------------------------------------------
void CSkillLearnWnd::Init()
{
	LearnedSkillCode = -1;
	SetClassSkillLearnInfo();
}


/*
// 스킬 습득창
//-----------------------------------------------------------------------------
#define WID_SkillLearn_Wnd					2500				// 스킬 습득창			
#define WID_SkillLearn_Close_Btn			2501				// 닫기 버튼
#define WID_SkillLearn_Learn_Btn			2502				// 스킬 습득 버튼
#define WID_SkillLearn_ContextText			2503				// 퀘스트 내용 
#define WID_SkillLearn_ClassText			2504				// 퀘스트 내용 

#define WID_SkillLearn_SkillLearnName1_Txt	2510
#define WID_SkillLearn_NeedSkill11_Txt		2511
#define WID_SkillLearn_NeedSkill21_Txt		2512

#define WID_SkillLearn_SkillLearnName2_Txt	2513
#define WID_SkillLearn_NeedSkill12_Txt		2514
#define WID_SkillLearn_NeedSkill22_Txt		2515

#define WID_SkillLearn_SkillLearnName3_Txt	2516
#define WID_SkillLearn_NeedSkill13_Txt		2517
#define WID_SkillLearn_NeedSkill23_Txt		2518

#define WID_SkillLearn_SkillLearnName4_Txt	2519
#define WID_SkillLearn_NeedSkill14_Txt		2520
#define WID_SkillLearn_NeedSkill24_Txt		2521

#define WID_SkillLearn_Field0_Btn			2530					// 스킬 습득 리스트 버튼 0
#define WID_SkillLearn_Field1_Btn			2531					// 스킬 습득 리스트 버튼 1
#define WID_SkillLearn_Field2_Btn			2532					// 스킬 습득 리스트 버튼 2
#define WID_SkillLearn_Field3_Btn			2533					// 스킬 습득 리스트 버튼 3

#define WID_SkillLearn_List_Scroll			2540					// 스킬 습득 리스트 스크롤
#define WID_SkillLearn_Context_Scroll		2541					// 스킬 습득 내용 스크롤

// 스킬 습득창( 2200 ~ )
//-----------------------------------------------------------------------------
#define TID_SkillLearn_Wnd					2200	
#define TID_CTRL_TEXT_OVER			2201	
#define TID_SkillLearn_LearnBtn_Focus		2202	
#define TID_SkillLearn_LearnBtn_Select		2203	
#define TID_SkillLearn_ScrollUp_Select		2204	
#define TID_SkillLearn_ScrollDown_Select	2205	
#define TID_SkillLearn_Scroll_Thumb			2206	
#define TID_SkillLearn_CloseBtn_Select		2207	

 */
void CSkillLearnWnd::Open()
{
	LearnedSkillCode = -1;
	SetClassSkillLearnInfo();
	nRui->SendEvent( WID_SkillLearn_Wnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );

}

void CSkillLearnWnd::Composition()
{

#ifdef C_SEALED_SKILL_LEARN
	CompositionNew();
#else 
	CompositionOld();
#endif 

}




//-----------------------------------------------------------------------------
void CSkillLearnWnd::CompositionNew()
{
	char strTemp[255] = {0,};

	int center_x	= nRui->Desktop.Width / 2;
	int center_y	= nRui->Desktop.Height / 2;

	CFrameWnd*		fw		   = NULL;
	CButton*		btn		   = NULL;
	CStateButton*	sbtn       = NULL;
	SRect			skillwnd;
	CTextBox*		tbox;
	CImageBox9*		img9;
	
	skillwnd.w	= 780;
	skillwnd.h	= 298;
	skillwnd.x	= 150; 
	skillwnd.y	= 357; 	

	// 스킬 습득창
	fw = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_SkillLearn_Wnd, WID_None, skillwnd.x, skillwnd.y, skillwnd.w, skillwnd.h, false );
	fw->SetWndTexStyle( fwfTex_Style_Tile9 );	
	fw->SetClientImage( TID_CTRL_WND_L1 );	
	fw->SetIconTID( TID_CTRL_ICON_SKILL );	
	fw->SetWndTileSizeLR( 40, 29 );
	fw->SetWndTileSizeTB( 31, 11 );
	fw->SetTitle( true );
	fw->SetCaption( G_STRING_CHAR( IDS_WND_SKILLLEARN ) );	 
	fw->SetVisible( false );
	fw->SetWndProc( this );
	fw->Initialize();

	SRect sTempRect;

	sTempRect.x	= 5;
	sTempRect.y	= 51;
	sTempRect.w	= 780-10;
	sTempRect.h	= 107;

	///-- Image9
	img9 = (CImageBox9*)nRui->CreateWnd( n_wtImageBox9, WID_None, WID_SkillLearn_Wnd, 
										 sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );
	img9->SetWndTileSizeLR( 9, 9 );
	img9->SetWndTileSizeTB( 8, 8 );
	img9->SetFocusCheck( false );
	img9->SetClientImage( TID_CTRL_CONTBACK_L1 );
	img9->SetAdjustPosition( 0, 0 );

	sTempRect.x	= 5;
	sTempRect.y	= 162;
	sTempRect.w	= 780-10;
	sTempRect.h	= 104;
	
	img9 = (CImageBox9*)nRui->CreateWnd( n_wtImageBox9, WID_None, WID_SkillLearn_Wnd, 
										 sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );       	
	img9->SetWndTileSizeLR( 9, 9 );
	img9->SetWndTileSizeTB( 8, 8 );
	img9->SetFocusCheck( false );
	img9->SetClientImage( TID_CTRL_CONTBACK_L1 );
	img9->SetAdjustPosition( 0, 0 );

	///-- 직업(Static)
	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_SkillLearn_Wnd, 20, 30, 133, 17, false );
	tbox->SetFontR( n_fRsvFontWndStatic );	
	tbox->SetAlignText( n_atLeft, n_atCenter );
	Rsprintf( RWCHAR(strTemp) , _RT("%s"), G_STRING_CHAR( IDS_CHARSTATE_CLASS ) ); 
	tbox->SetText( strTemp ); 

	///-- 필요스킬1(Static)
	sTempRect.x	= 290;
	sTempRect.y	= 30;
	sTempRect.w	= 100;
	sTempRect.h	= 17;
	
	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_SkillLearn_Wnd, 
									   sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );
	tbox->SetFontR( n_fRsvFontWndStatic );	
	tbox->SetAlignText( n_atLeft, n_atCenter );
	tbox->SetText( G_STRING_CHAR(IDS_WND_LEARNING_COST)); 

	sTempRect.x	= 400;
	sTempRect.y	= 30;
	sTempRect.w	= 133;
	sTempRect.h	= 17;

	///-- 필요스킬1(Static)
	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_SkillLearn_Wnd, 
									   sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );
	tbox->SetFontR( n_fRsvFontWndStatic );	
	tbox->SetAlignText( n_atLeft, n_atCenter );
	Rsprintf( RWCHAR(strTemp), _RT("%s"), G_STRING_CHAR( IDS_WND_NEED_SKILL ) ); 
	tbox->SetText( strTemp ); 

	sTempRect.x	= 500;
	sTempRect.y	= 30;
	sTempRect.w	= 133;
	sTempRect.h	= 17;

	///-- 필요스킬(Static)
	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_SkillLearn_Wnd, 
									   sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );
	tbox->SetFontR( n_fRsvFontWndStatic );	
	tbox->SetAlignText( n_atLeft, n_atCenter );
	tbox->SetText( G_STRING_CHAR(IDS_WND_SKILL_POINT) ); 

	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_SkillLearn_Wnd, 
		600, 30 , 133 , 17, false );
	tbox->SetFontR( n_fRsvFontWndStatic );	
	tbox->SetAlignText( n_atLeft, n_atCenter );
	//	Rsprintf( RWCHAR(strTemp), _RT("%s"), G_STRING_CHAR( IDS_WND_NEED_SKILL ) );
	tbox->SetText( G_STRING_CHAR(IDS_WND_NEED_ITEM) );

	// 직업 
	ClassName = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_SkillLearn_ClassText, WID_SkillLearn_Wnd, 63, 30, 64, 16, false );
	ClassName->SetFontg( n_ftGulimChe, 12, n_fwBold, n_fdNormal );
	ClassName->SetClientImage( TID_None );
	ClassName->SetAlignText( n_atLeft, n_atCenter );	
	ClassName->SetClientColor( 180, 175, 160 );	
	ClassName->SetTextColor( 25, 25, 35 );
	ClassName->SetHID( HID_None );
	ClassName->SetText( _RT("") );

	sTempRect.x	= 780-20;
	sTempRect.y	= 5;
	sTempRect.w	= 13;
	sTempRect.h	= 13;

	// 닫기 버튼
	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_SkillLearn_Close_Btn , WID_SkillLearn_Wnd , 
									 sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );
	btn->SetBtnImage( TID_CTRL_BTN_X, TID_CTRL_BTN_X_OVR, TID_CTRL_BTN_X_CLK, TID_None );	

	sTempRect.x	= 780-17;
	sTempRect.y	= 52;
	sTempRect.w	= 15;
	sTempRect.h	= 105;

	// 스크롤바 ( Quest_List )
	ListScroll = (CScrollBar*)nRui->CreateWnd( n_wtScrollBar, WID_SkillLearn_List_Scroll , WID_SkillLearn_Wnd, 
											   sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );
	ListScroll->InitSlide( stVertSld , 0 , 195 , 0 , 13 );//Type, min, max, value, 썸의 폭
	ListScroll->SetClientImage( TID_CTRL_SCR_BG );
	ListScroll->SetThumbImage( TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR_CLK, TID_None );
	ListScroll->SetScrollBtnA( TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT_CLK, TID_None );
	ListScroll->SetScrollBtnB( TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT_CLK, TID_None );	

	sTempRect.x	= 780-17;
	sTempRect.y	= 163;
	sTempRect.w	= 15;
	sTempRect.h	= 102;
			
	// 스크롤바 ( Context )	
	DescScroll = (CScrollBar*)nRui->CreateWnd( n_wtScrollBar, WID_SkillLearn_Context_Scroll , WID_SkillLearn_Wnd, 
											   sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );
	DescScroll->InitSlide( stVertSld , 0 , 5 , 0 , 13 );//Type, min, max, value, 썸의 폭
	DescScroll->SetClientImage( TID_CTRL_SCR_BG );
	DescScroll->SetThumbImage( TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR_CLK, TID_None );
	DescScroll->SetScrollBtnA( TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT_CLK, TID_None );
	DescScroll->SetScrollBtnB( TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT_CLK, TID_None );
	
	int nAddXValue = 50;
	int i=0;
	int Ypos = 67 ; 

	for( i=0; i < 4; ++i )
	{
		
		//-------------------------------------------------------------------------
		SkillLearnField[i].SkillName = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None , WID_SkillLearn_Wnd, 
			18, Ypos , 302, 16, false );
		SkillLearnField[i].SkillName->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
		SkillLearnField[i].SkillName->SetClientImage( TID_None );
		SkillLearnField[i].SkillName->SetAlignText( n_atLeft, n_atCenter );
		SkillLearnField[i].SkillName->SetText(_RT("가나다라마바사"));
	
		
		
		SkillLearnField[i].NeedSkill1 = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_SkillLearn_Wnd, 
			nAddXValue + 334, Ypos, 98, 16, false );
		SkillLearnField[i].NeedSkill1->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
		SkillLearnField[i].NeedSkill1->SetClientImage( TID_None );
		SkillLearnField[i].NeedSkill1->SetAlignText( n_atLeft, n_atCenter );
		SkillLearnField[i].NeedSkill1->SetText(_RT("가나다라마바사"));
		
		SkillLearnField[i].NeedSkill2 = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_SkillLearn_Wnd, 
			nAddXValue + 469, Ypos, 98, 16, false );
		SkillLearnField[i].NeedSkill2->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
		SkillLearnField[i].NeedSkill2->SetClientImage( TID_None );
		SkillLearnField[i].NeedSkill2->SetAlignText( n_atLeft, n_atCenter );
		SkillLearnField[i].NeedSkill2->SetText(_RT("999"));

		//비용 
		SkillLearnField[i].TxtLernCost = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_SkillLearn_Wnd, 
			nAddXValue + 236 , Ypos , 98, 16, false );
		SkillLearnField[i].TxtLernCost->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
		SkillLearnField[i].TxtLernCost->SetClientImage( TID_None );
		SkillLearnField[i].TxtLernCost->SetAlignText( n_atLeft, n_atCenter );
		SkillLearnField[i].TxtLernCost->SetText(_RT("123456789"));
		
		//필요아이템 
		SkillLearnField[i].TxtNeedItemName = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_SkillLearn_Wnd, 
			580 , Ypos , 98, 16, false );
		SkillLearnField[i].TxtNeedItemName->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
		SkillLearnField[i].TxtNeedItemName->SetClientImage( TID_None );
		SkillLearnField[i].TxtNeedItemName->SetAlignText( n_atLeft, n_atCenter );
		SkillLearnField[i].TxtNeedItemName->SetText(_RT("Light Bible Lv9[1개]"));

		
		/*
		//아이템갯수
		SkillLearnField[i].TetNeedItemNum = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_SkillLearn_Wnd, 
			nAddXValue + 256 , Ypos , 98, 16, false );
		SkillLearnField[i].TetNeedItemNum->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
		SkillLearnField[i].TetNeedItemNum->SetClientImage( TID_None );
		SkillLearnField[i].TetNeedItemNum->SetAlignText( n_atLeft, n_atCenter );
		SkillLearnField[i].TetNeedItemNum->SetText(_RT("99"));
		*/


		SelBtn[i] = (CButton*)nRui->CreateWnd( n_wtButton, WID_SkillLearn_Field0_Btn +i , WID_SkillLearn_Wnd, 
			12, Ypos - 5 , 740, 21, false );
		SelBtn[i]->SetClientImage( TID_None );
		SelBtn[i]->SetBtnImage( TID_None, TID_CTRL_TEXT_OVER, TID_CTRL_TEXT_OVER, TID_None );

		Ypos += 22 ; 
	}
	// 퀘스트 내용
	SkillDescText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_SkillLearn_ContextText, WID_SkillLearn_Wnd, 
												18, 172, nAddXValue + 583 , 50, false );
	SkillDescText->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	SkillDescText->SetClientImage( TID_None );
	SkillDescText->SetClientColor( 180, 175, 160 );
	SkillDescText->SetMultiLine( TRUE );
	SkillDescText->SetTextColor( 255 ,255 ,255 );
	SkillDescText->SetHID( HID_None );
	SkillDescText->SetText( _RT("") );

	// 습득 버튼
	LearnBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_SkillLearn_Learn_Btn , WID_SkillLearn_Wnd ,  (780/2) -29  , 269, 58, 25, false );
	LearnBtn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_CTRL_BTN58_DIS );		
	LearnBtn->SetFontR( n_fRsvFontWndButton );
	LearnBtn->SetText( G_STRING_CHAR( IDS_WND_LEARN_BTN ) );	

	return;
}

//-----------------------------------------------------------------------------
void CSkillLearnWnd::Update()
{
}

//-----------------------------------------------------------------------------
void CSkillLearnWnd::Proc( SEventMessage* EMsg )
{
	int i = 0 ; 
	bool IsFindItem  = false; 
	bool IsItemNumCheck = false; 


	switch( EMsg->FocusWnd ) 
	{
	case WID_SkillLearn_Close_Btn:
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			nRui->SendEvent( WID_SkillLearn_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
			IsVisible = false;
		}
		break;
		
	case WID_SkillLearn_Learn_Btn:
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			if( LearnedSkillCode != SkillLearnField[SelectedBtn].SkillCode )
			{
				//비용
#ifdef C_SEALED_SKILL_LEARN

				if( SkillLearnField[SelectedBtn].LernCost) 
				{
					if( SkillLearnField[SelectedBtn].LernCost >  nRui->thePcParam.Money) 
					{
						SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NEED_LEARNING_COST) );
						return;
					}
				}
				if( SkillLearnField[SelectedBtn].NeedSkillCode_2 )
				{
					if( SkillLearnField[SelectedBtn].NeedSkillCode_2 > nRui->thePcParam.SkillPoint )
					{
						SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_SHORTAGE_POINT));
						return;
					}
				}
				
				if( SkillLearnField[SelectedBtn].NeedItemCode  )
				{
					IsFindItem = FALSE;

					for( i = 0; i < c_MaxInvenItemNum; ++i )
					{
						if( ( nRui->thePcInven.InvenItem[i] != NULL ) && 
							( nRui->thePcInven.InvenItem[i]->Code == SkillLearnField[SelectedBtn].NeedItemCode ) )
						{
							IsFindItem = TRUE ; 
							
							break;
						}
					}

					if(!IsFindItem)
					{
						SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_CANT_FIND_ITEM));
						return;
					}
				}

#endif 

				//서버에 메세지 날리고....
				g_RockClient.Send_RpCsGetSkill( SkillLearnField[SelectedBtn].SkillCode );							
				LearnedSkillCode = SkillLearnField[SelectedBtn].SkillCode;
				LearnBtn->SetBtnHold( TRUE );
			}
			else
			{
				LearnBtn->SetBtnHold( TRUE );
			}			
			// 다시 접속하면 어차피 지워질 테니까...^ ^;;
//			theClassSkillLessonInfo[StartListNum + SelectedBtn].theNeedLevel = 255;			
//			SetSkillLearnField(SelectedBtn);
//			nRui->SendEvent( WID_SkillLearn_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
//			IsVisible = false;
		}
		break;

	case WID_SkillLearn_Field0_Btn:
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			SkillBntSelected( 0 );
			ScrollFocus = n_SkillLearnListScroll;
		}
		break;
	
	case WID_SkillLearn_Field1_Btn:
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			SkillBntSelected( 1 );	
			ScrollFocus = n_SkillLearnListScroll;
		}
		break;

	case WID_SkillLearn_Field2_Btn:
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			SkillBntSelected( 2 );
			ScrollFocus = n_SkillLearnListScroll;
		}
		break;

	case WID_SkillLearn_Field3_Btn:
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			SkillBntSelected( 3 );
			ScrollFocus = n_SkillLearnListScroll;
		}
		break;

	case WID_SkillLearn_List_Scroll:
		if( ( EMsg->Notify == NM_SLD_MOVEBAR ) || ( EMsg->Notify == NM_SLD_CLICKMOVEBAR ) || ( EMsg->Notify == NM_SLD_BTNMOVERBAR ) )
		{
			SetSkillListScroll( (int)EMsg->fParam );
			ScrollFocus = n_SkillLearnListScroll;
		}
		break;
	case WID_SkillLearn_Context_Scroll:
		if( ( EMsg->Notify == NM_SLD_MOVEBAR ) || ( EMsg->Notify == NM_SLD_CLICKMOVEBAR ) || ( EMsg->Notify == NM_SLD_BTNMOVERBAR ) )
		{
			SetDecsTextScroll( (int)EMsg->fParam );
			ScrollFocus = n_SkillLearnDescScroll;
		}
		break;
	case WID_SkillLearn_ContextText:
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			ScrollFocus = n_SkillLearnDescScroll;
		}
		break;
	}
}

//-----------------------------------------------------------------------------
void CSkillLearnWnd::SetWheelValue( short _wheel )
{
	int value = 0;

	if( ScrollFocus == n_SkillLearnListScroll )
	{
		ListScroll->GetValue( &value );
		if( ( _wheel < 0 ) && ( value < 195 ) )
		{
			value += 1;
		}
		else if( ( _wheel > 0 ) && ( value > 0 ) )
		{
			value -= 1;
		}
		
		ListScroll->SetValue( value );
		SetSkillListScroll( value );
	}
	else if( ScrollFocus == n_SkillLearnDescScroll )
	{
		DescScroll->GetValue( &value );
		if( ( _wheel < 0 ) && ( value < 5 ) )
		{
			value += 1;
		}
		else if( ( _wheel > 0 ) && ( value > 0 ) )
		{
			value -= 1;
		}
		
		DescScroll->SetValue( value );
		SetDecsTextScroll( value );
	}
}


//-----------------------------------------------------------------------------
void CSkillLearnWnd::SkillBntSelected( int aNumBtn )
{
	SelectedBtn = aNumBtn;
	if( SelectedBtn != -1 )
	{
		//선택 라인 반전
		SelBtn[0]->SetClientImage( TID_None );
		SelBtn[1]->SetClientImage( TID_None );
		SelBtn[2]->SetClientImage( TID_None );
		SelBtn[3]->SetClientImage( TID_None );
		SelBtn[SelectedBtn]->SetClientImage( TID_CTRL_TEXT_OVER );

		//퀘스트 설명
		SkillDescText->SetText( SkillLearnField[SelectedBtn].Desc );
		if( SkillLearnField[SelectedBtn].ListType == n_LearnSkill )
		{
			LearnBtn->SetBtnHold( FALSE );		
		}
		else
		{
			LearnBtn->SetBtnHold( TRUE );
		}
	}
	else
	{
		SelBtn[0]->SetClientImage( TID_None );
		SelBtn[1]->SetClientImage( TID_None );
		SelBtn[2]->SetClientImage( TID_None );
		SelBtn[3]->SetClientImage( TID_None );

		SkillDescText->SetText( _RT("") );
		LearnBtn->SetBtnHold( TRUE );	
	}
}

//-----------------------------------------------------------------------------
void CSkillLearnWnd::SetSkillListScroll( int _linenum )
{
	SkillBntSelected( -1 );

	StartListNum = _linenum * 1;
	SetSkillLearnField();		
	
}

//-----------------------------------------------------------------------------
void CSkillLearnWnd::SetDecsTextScroll( int _linenum )
{
	SkillDescText->SetStartLine( _linenum );
}

//-----------------------------------------------------------------------------
/*
typedef struct _SSkillLessonInfo
{
	BYTE	theClass;				// 직업코드 (MainClass)
	BYTE	theNeedLevel;			// 습득레벨
	WORD	theSkillCode;			// 스킬코드
	char	theSkillName[20];		// 스킬명
	WORD	theNeedSkillCode1;		// 선행스킬코드1
	WORD	theNeedSkillCode2;		// 선행스킬코드2
}SSkillLessonInfo, * SSkillLessonInfoPtr;
*/
void CSkillLearnWnd::SetClassSkillLearnInfo()
{
	memset( theClassSkillLessonInfo, 0, sizeof(SSkillLessonInfo)*200 );

	SSkillBaseInfo* skill_info = NULL;
	
	int i = 0;
	int num = 0;
	int idx = 0;
	int skill_max = 0;
		
	char strTemp[255] = {0,};


	for( i = 0; i < c_Max_Skill_Num; ++i )
	{
		//		if( nRui->thePcParam.MainClass == g_Pc_Manager.theSkillLessonInfoTable[i].theClass )
		if( IsEnableSkillLearn( nRui->thePcParam.MainClass, g_Pc_Manager.theSkillLessonInfoTable[i].theClass ) )			
		{
			if( g_Pc_Manager.theSkillLessonInfoTable[i].theSkillCode == 0 )
			{
				break;
			}
			
			bool IsLearned = false;
		
			if( g_Pc_Manager.theSkillLessonInfoTable[i].theNeedLevel > MAX_LEVEL ) 
			{
				IsLearned = true; 
			}
			
			for( num = 0; num < RP_MAX_USER_SKILL; ++num )
			{
				if( nRui->GetPcInvenInfo()->SkillItem[num] != NULL )
				{
					
					skill_info = g_Pc_Manager.GetSkillBaseInfoPtr( g_Pc_Manager.theSkillLessonInfoTable[i].theSkillCode );
					
					// 빙의 계열들은 각각 하나씩 배우므로 레벨별로 체크하면 안됨 
					if( g_Pc_Manager.theSkillLessonInfoTable[i].theSkillCode >= 1811 && g_Pc_Manager.theSkillLessonInfoTable[i].theSkillCode <= 1815 )
					{
						if( nRui->GetPcInvenInfo()->SkillItem[num]->Code == g_Pc_Manager.theSkillLessonInfoTable[i].theSkillCode )									
						{
							IsLearned = true;
							break;
						}
					}
					else
					{
						skill_max = ( ( g_Pc_Manager.theSkillLessonInfoTable[i].theSkillCode / 10 ) * 10 ) + 10;			

						if( nRui->GetPcInvenInfo()->SkillItem[num]->Code >= g_Pc_Manager.theSkillLessonInfoTable[i].theSkillCode && 
//							nRui->GetPcInvenInfo()->SkillItem[num]->Code < g_Pc_Manager.theSkillLessonInfoTable[i].theSkillCode + 10
							nRui->GetPcInvenInfo()->SkillItem[num]->Code <= skill_max )									
						{
							IsLearned = true;
							break;
						}
					}
				}
			}
			
			///-- 배울 수 있는 스킬만 등록
			if( !IsLearned && idx < 200 )
			{
				bool IsDisable = false;
				bool IsNeedSkill1Learned = false;
				bool IsNeedSkill2Learned = false;
				int Need_Skill_Max = 0;
				
				///-- 해당 클래스의 스킬이 있다면
				if ( g_Pc_Manager.theSkillLessonInfoTable[i].theSkillCode && g_Pc_Manager.theSkillLessonInfoTable[i].theClass == nRui->thePcParam.MainClass ) 
				{
					///---------------------------------------------------------------------------
					///-- 레벨부족
//					if( nRui->thePcParam.Level < g_Pc_Manager.theSkillLessonInfoTable[i].theNeedLevel )
//					{
//						IsDisable = true;			
//					}					
					///-- 필요 스킬
					if( g_Pc_Manager.theSkillLessonInfoTable[i].theNeedSkillCode1 )
					{
						int num = 0;
						
						for( num = 0; num < RP_MAX_USER_SKILL; ++num )
						{
							if( nRui->GetPcInvenInfo()->SkillItem[num] == NULL )
							{
								continue;
							}
							
							// Base Code 
							Need_Skill_Max = ( ( g_Pc_Manager.theSkillLessonInfoTable[i].theNeedSkillCode1 / 10 ) * 10 ) + 10;			
			
							if( ( nRui->GetPcInvenInfo()->SkillItem[num]->Code >= g_Pc_Manager.theSkillLessonInfoTable[i].theNeedSkillCode1 ) && 
								( nRui->GetPcInvenInfo()->SkillItem[num]->Code <= Need_Skill_Max ) ) 
//							if( nRui->GetPcInvenInfo()->SkillItem[num]->Code == g_Pc_Manager.theSkillLessonInfoTable[i].theNeedSkillCode1 )
							{
								IsNeedSkill1Learned = true;
								break;
							}
						}
						/*
#ifdef C_SEALED_NPC_CHAT_RENEW
					
#else
						if( IsNeedSkill1Learned )
						{
							//두번째 스킬이 있으면 ? 검사 
							if( g_Pc_Manager.theSkillLessonInfoTable[i].theNeedSkillCode2 )
							{
								for( num = 0; num < RP_MAX_USER_SKILL; ++num )
								{
									if( nRui->GetPcInvenInfo()->SkillItem[num] == NULL )
									{
										continue;
									}
									
									// Base Code 
									Need_Skill_Max = ( ( g_Pc_Manager.theSkillLessonInfoTable[i].theNeedSkillCode2 / 10 ) * 10 ) + 10;			
									
									if( ( nRui->GetPcInvenInfo()->SkillItem[num]->Code >= g_Pc_Manager.theSkillLessonInfoTable[i].theNeedSkillCode2 ) && 
										( nRui->GetPcInvenInfo()->SkillItem[num]->Code <= Need_Skill_Max ) ) 
//									if( nRui->GetPcInvenInfo()->SkillItem[num]->Code == g_Pc_Manager.theSkillLessonInfoTable[i].theNeedSkillCode2 )
									{
										IsNeedSkill2Learned = true;
										break;
									}
								}
								
								if( !IsNeedSkill2Learned )
								{
									IsDisable = true;													
								}
							}
						}
						else
						{
							IsDisable = true;								
						}
#endif 
						*/
					}///-- end of if 필요스킬						
					///-- 등록
					if( !IsDisable )
					{
						theClassSkillLessonInfo[idx] = g_Pc_Manager.theSkillLessonInfoTable[i];				
						++idx;
					}
					
				}///-- end of if 스킬
			}///-- 배울 수 있는 스킬만
		}
	}
	
	ListScroll->SetValue( 0 );
	DescScroll->SetValue( 0 );

	StartListNum = 0;
	
	GetClassText( nRui->thePcParam.MainClass, strTemp );
	ClassName->SetText( strTemp );
	
	SetSkillLearnField();
	SkillBntSelected( SelectedBtn );
	ScrollFocus = n_SkillLearnListScroll;
	
	return;
}

void CSkillLearnWnd::SetSkillLearnField()
{	
	int j;
	char str[512] = {0, };
	int ll_listnum = 0;
	
	///-- reset All
	for( j = 0; j < 4 ; j++ )
	{
		SkillLearnField[ j ].ListType = n_BlankSkill;
		SkillLearnField[ j ].SkillCode = 0;
		SkillLearnField[ j ].NeedSkillCode_1 = 0;
		SkillLearnField[ j ].NeedSkillCode_2 = 0;
		SkillLearnField[ j ].SkillName->SetText( _RT("") );		// 스킬명	
		SkillLearnField[ j ].NeedSkill1->SetText( _RT("") );		// 필요 스킬 1 
		SkillLearnField[ j ].NeedSkill2->SetText( _RT("") );		// 필요 스킬 2 
		SkillLearnField[ j ].TxtLernCost->SetText( _RT("") );		// 필요 스킬 2 
		SkillLearnField[ j ].Desc[0] = _RT('\0');					// 설명
#ifdef C_SEALED_SKILL_LEARN
		//SkillLearnField[ j ].TxtNeedSkillPoint->SetText( _RT(""));
		SkillLearnField[ j ].TxtNeedItemName->SetText( _RT(""));
#endif 
		SkillDescText->SetText(_RT(""));
	}

	///-- 모든 스킬을 비교해서
	for( j = 0 ; j < 200 ; ++j )
	{
		///-- 스킬이 있다면
		if ( StartListNum + j < 200 && theClassSkillLessonInfo[StartListNum + j].theSkillCode ) 
		{
			SkillLearnField[ll_listnum].SkillCode = theClassSkillLessonInfo[StartListNum + j].theSkillCode;
			SkillLearnField[ll_listnum].NeedSkillCode_1 = theClassSkillLessonInfo[StartListNum + j].theNeedSkillCode1;
			SkillLearnField[ll_listnum].NeedSkillCode_2 = theClassSkillLessonInfo[StartListNum + j].theNeedSkillCode2;
			SkillLearnField[ll_listnum].LernCost =  0;
			
#ifdef CLINET_SKILL_LERNCOST
			SkillLearnField[ll_listnum].LernCost = theClassSkillLessonInfo[StartListNum + j].theCost;
#endif 

#ifdef C_SEALED_SKILL_LEARN
			SkillLearnField[ll_listnum].NeedSkillCode_2 = theClassSkillLessonInfo[StartListNum + j].theSkillPoint;
			SkillLearnField[ll_listnum].NeedItemCode = theClassSkillLessonInfo[StartListNum + j].theNeedItemCode;
			SkillLearnField[ll_listnum].NeedItemNum = theClassSkillLessonInfo[StartListNum + j].theNeedItemNum;
#endif

			SSkillBaseInfo* skill_info = NULL;
			skill_info = g_Pc_Manager.GetSkillBaseInfoPtr( SkillLearnField[ll_listnum].SkillCode );
			
		
			if( skill_info == NULL )
				return;

//			sprintf( str, "%s (Lv%d)", theClassSkillLessonInfo[StartListNum + j].theSkillName, theClassSkillLessonInfo[StartListNum + j].theNeedLevel );
			Rsprintf( RWCHAR(str) , _RT("%s (Lv%d)") , RWCHAR(skill_info->theSkillName), theClassSkillLessonInfo[StartListNum + j].theNeedLevel );
			SkillLearnField[ll_listnum].SkillName->SetText( str );
			 
			
			Rsprintf( RWCHAR(SkillLearnField[ll_listnum].Desc), _RT("%s"), RWCHAR(skill_info->theDesc) );
						
			SkillLearnField[ll_listnum].SkillName->SetTextColor( 255 ,255 ,255 );
			SkillLearnField[ll_listnum].NeedSkill1->SetTextColor( 255 ,255 ,255 );
			SkillLearnField[ll_listnum].NeedSkill2->SetTextColor( 255 ,255 ,255 );
			SkillLearnField[ll_listnum].TxtLernCost->SetTextColor( 255 ,255 ,255 );
			SkillLearnField[ll_listnum].ListType = n_LearnSkill;

#ifdef CLINET_SKILL_LERNCOST
			InventoryWnd.SetEldText(str ,SkillLearnField[ll_listnum].LernCost);
			//sprintf( str, "%s", SkillLearnField[ll_listnum].LernCost  );
			SkillLearnField[ll_listnum].TxtLernCost->SetText( str ); 
#endif

			

			if( SkillLearnField[ll_listnum].NeedSkillCode_1 )
			{
				skill_info = g_Pc_Manager.GetSkillBaseInfoPtr( SkillLearnField[ll_listnum].NeedSkillCode_1 );
				
				if( skill_info )
					Rsprintf( RWCHAR(str), _RT("%s"), RWCHAR(skill_info->theSkillName) );

				SkillLearnField[ll_listnum].NeedSkill1->SetText( str );
			}
			else
			{
				SkillLearnField[ll_listnum].NeedSkill1->SetText( _RT("") );
			}



			//
			// G_STRING_CHAR(IDS_SYSPR_SHORTAGE_POINT)	//포인트 부족 ; 
			// G_STRING_CHAR(IDS_WND_NOT_NEEDITEM)
			// G_STRING_CHAR(IDS_SYSPR_NEED_LEARNING_COST)
			

#ifdef C_SEALED_SKILL_LEARN 
			if( SkillLearnField[ll_listnum].NeedSkillCode_2 )
			{
				Rsprintf( RWCHAR(str), _RT("[%d]"), SkillLearnField[ll_listnum].NeedSkillCode_2  );	
				
				SkillLearnField[ll_listnum].NeedSkill2->SetText( str );
			}
			else
			{
				SkillLearnField[ll_listnum].NeedSkill2->SetText( _RT("") );
			}

			if( SkillLearnField[ll_listnum].NeedItemCode )
			{
				SItemBaseInfo *item_info = NULL;
				item_info = g_Pc_Manager.GetItemBaseInfoPtr( SkillLearnField[ll_listnum].NeedItemCode );
				if( item_info )
					Rsprintf( RWCHAR(str), _RT("%s [%d%s]"), RWCHAR(item_info->theName) , 
					SkillLearnField[ll_listnum].NeedItemNum , G_STRING_CHAR(IDS_WND_AMOUNT) );	
								
				SkillLearnField[ll_listnum].TxtNeedItemName->SetText( str );
			}
			else
			{
				SkillLearnField[ll_listnum].TxtNeedItemName->SetText( _RT("") );
			}

#else 
			if( SkillLearnField[ll_listnum].NeedSkillCode_2 )
			{
			

				skill_info = g_Pc_Manager.GetSkillBaseInfoPtr( SkillLearnField[ll_listnum].NeedSkillCode_2 );

				if( skill_info )
					Rsprintf( RWCHAR(str), _RT("%s"), RWCHAR(skill_info->theSkillName) );

				SkillLearnField[ll_listnum].NeedSkill2->SetText( str );
			}
			else
			{
				SkillLearnField[ll_listnum].NeedSkill2->SetText( _RT("") );
			}
#endif 


			ll_listnum ++;
			
			if( ll_listnum == 4)
				return;
		}
	}
	
}	

	
	/*
	if( theClassSkillLessonInfo[StartListNum + aListNum].theSkillCode )
	{
	SkillLearnField[aListNum].SkillCode = theClassSkillLessonInfo[StartListNum + aListNum].theSkillCode;
	SkillLearnField[aListNum].NeedSkillCode_1 = theClassSkillLessonInfo[StartListNum + aListNum].theNeedSkillCode1;
	SkillLearnField[aListNum].NeedSkillCode_2 = theClassSkillLessonInfo[StartListNum + aListNum].theNeedSkillCode2;
	
	  sprintf( str, "%s (Lv%d)", theClassSkillLessonInfo[StartListNum + aListNum].theSkillName, theClassSkillLessonInfo[StartListNum + aListNum].theNeedLevel );
	  SkillLearnField[aListNum].SkillName->SetText( str );
	  
		SSkillBaseInfo* skill_info = NULL;
		skill_info = g_Pc_Manager.GetSkillBaseInfoPtr( SkillLearnField[aListNum].SkillCode );
		sprintf( SkillLearnField[aListNum].Desc, "%s", skill_info->theDesc );
		//		SkillDescText->SetText( SkillLearnField[aListNum].Desc );
		
		if( SkillLearnField[aListNum].NeedSkillCode_1 )
		{
			skill_info = g_Pc_Manager.GetSkillBaseInfoPtr( SkillLearnField[aListNum].NeedSkillCode_1 );
			sprintf( str, "%s", skill_info->theSkillName );
			SkillLearnField[aListNum].NeedSkill1->SetText( str );
		}
		else
		{
			SkillLearnField[aListNum].NeedSkill1->SetText( _RT("") );
		}
		

		if( SkillLearnField[aListNum].NeedSkillCode_2 )
		{
			skill_info = g_Pc_Manager.GetSkillBaseInfoPtr( SkillLearnField[aListNum].NeedSkillCode_2 );
			sprintf( str, "%s", skill_info->theSkillName );
			SkillLearnField[aListNum].NeedSkill2->SetText( str );
		}
		else
		{
			SkillLearnField[aListNum].NeedSkill2->SetText( _RT("") );
		}

		bool IsDisable = false;
		bool IsNeedSkill1Learned = false;
		bool IsNeedSkill2Learned = false;

		// 레벨이 부족하거나 필요 스킬이 없을때...Disable 시킨다
		if( nRui->thePcParam.Level < theClassSkillLessonInfo[StartListNum + aListNum].theNeedLevel )
		{
			IsDisable = true;
			SkillLearnField[aListNum].ListType = n_NotNeedLevel;
		}
//		else 
		{
			if( SkillLearnField[aListNum].NeedSkillCode_1 )
			{
				int num = 0;

				for( num = 0; num < RP_MAX_USER_SKILL; ++num )
				{
					if( nRui->GetPcInvenInfo()->SkillItem[num] == NULL )
					{
						break;
					}

					if( nRui->GetPcInvenInfo()->SkillItem[num]->Code == SkillLearnField[aListNum].NeedSkillCode_1 )
					{
						IsNeedSkill1Learned = true;
						break;
					}
				}

				if( IsNeedSkill1Learned )
				{
					if( SkillLearnField[aListNum].NeedSkillCode_2 )
					{
						for( num = 0; num < RP_MAX_USER_SKILL; ++num )
						{
							if( nRui->GetPcInvenInfo()->SkillItem[num] == NULL )
							{
								break;
							}
							
							if( nRui->GetPcInvenInfo()->SkillItem[num]->Code == SkillLearnField[aListNum].NeedSkillCode_2 )
							{
								IsNeedSkill2Learned = true;
								break;
							}
						}

						if( !IsNeedSkill2Learned )
						{
							IsDisable = true;							
							SkillLearnField[aListNum].ListType = n_NotNeedSkill;
						}
					}
				}
				else
				{
					IsDisable = true;
					SkillLearnField[aListNum].ListType = n_NotNeedSkill;					
				}
			}
		}

		if( IsDisable )
		{
			SkillLearnField[aListNum].SkillName->SetTextColor( 180, 150, 150 );			

			if( IsNeedSkill1Learned )
			{
				SkillLearnField[aListNum].NeedSkill1->SetTextColor( 255, 255, 255 );
			}
			else
			{
				SkillLearnField[aListNum].NeedSkill1->SetTextColor( 180, 150, 150 );
			}
			
			if( IsNeedSkill2Learned )
			{
				SkillLearnField[aListNum].NeedSkill2->SetTextColor( 255, 255, 255 );
			}
			else
			{
				SkillLearnField[aListNum].NeedSkill2->SetTextColor( 180, 150, 150 );
			}
		}
		else
		{
			SkillLearnField[aListNum].SkillName->SetTextColor( 255 ,255 ,255 );
			SkillLearnField[aListNum].NeedSkill1->SetTextColor( 255 ,255 ,255 );
			SkillLearnField[aListNum].NeedSkill2->SetTextColor( 255 ,255 ,255 );
			SkillLearnField[aListNum].ListType = n_LearnSkill;
		}
	}
	else
	{
		SkillLearnField[aListNum].ListType = n_BlankSkill;
		SkillLearnField[aListNum].SkillCode = 0;
		SkillLearnField[aListNum].NeedSkillCode_1 = 0;
		SkillLearnField[aListNum].NeedSkillCode_2 = 0;
		SkillLearnField[aListNum].SkillName->SetText( _RT("") );		// 스킬명	
		SkillLearnField[aListNum].NeedSkill1->SetText( _RT("") );	// 필요 스킬 1 
		SkillLearnField[aListNum].NeedSkill2->SetText( _RT("") );	// 필요 스킬 2 
		SkillLearnField[aListNum].Desc[0] = '\0';					//설명
	}
	
	return;
}
*/

bool CSkillLearnWnd::IsEnableSkillLearn( BYTE aMainClass, WORD aNeedLearnClass )
{
	bool is_enable = false;

	// 현재 자신의 클래스에 따라서 배울 수 있는 스킬인지 판단 
	switch( aMainClass )
	{
	case Swordman:
	case Archer:
	case Magician:
	case Shaman:
		{
			if( aMainClass == aNeedLearnClass )
			{
				is_enable = true;			
			}
		}
		break;
	case Knight:		
	case Mercenary:			
	case Slayer:		
		{
			if( aMainClass == aNeedLearnClass )
			{
				is_enable = true;			
			}
			else if( aNeedLearnClass == Swordman )
			{
				is_enable = true;			
			}
		}
		break;
	case Sniper:	
		{
			if( aMainClass == aNeedLearnClass )
			{
				is_enable = true;			
			}
			else if( aNeedLearnClass == Archer )
			{
				is_enable = true;			
			}
		}
	case Prominas:				
	case Priest:		
	case Holy_Avenger:	
		{
			if( aMainClass == aNeedLearnClass )
			{
				is_enable = true;			
			}
			else if( aNeedLearnClass == Magician )
			{
				is_enable = true;			
			}
		}
		break;	
	case Psyche:		
		{
			if( aMainClass == aNeedLearnClass )
			{
				is_enable = true;			
			}
			else if( aNeedLearnClass == Shaman )
			{
				is_enable = true;			
			}
		}
		break;
	default:
		break;
	}

	return is_enable;
}

void CSkillLearnWnd::CompositionOld()
{
	char strTemp[255] = {0.};

	int center_x	= nRui->Desktop.Width / 2;
	int center_y	= nRui->Desktop.Height / 2;

	CFrameWnd*		fw		   = NULL;
	CButton*		btn		   = NULL;
	CStateButton*	sbtn       = NULL;
	SRect			skillwnd;
	CTextBox*		tbox;
	CImageBox9*		img9;
	
	skillwnd.w	= 615;
	skillwnd.h	= 298;
	skillwnd.x	= 200; //center_x - ( skillwnd.w / 2 );
	skillwnd.y	= 357; //center_y - ( skillwnd.h / 2 );

#ifdef APPLY_ENGLISH_ORIGIN_SIZE
	
	skillwnd.w	= 715;
	skillwnd.h	= 298;
	skillwnd.x	= 150; 
	skillwnd.y	= 357; 	

#endif

	// 스킬 습득창
	fw = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_SkillLearn_Wnd, WID_None, skillwnd.x, skillwnd.y, skillwnd.w, skillwnd.h, false );
	fw->SetWndTexStyle( fwfTex_Style_Tile9 );	
	fw->SetClientImage( TID_CTRL_WND_L1 );	
	fw->SetIconTID( TID_CTRL_ICON_SKILL );	
	fw->SetWndTileSizeLR( 40, 29 );
	fw->SetWndTileSizeTB( 31, 11 );
	fw->SetTitle( true );
	fw->SetCaption( G_STRING_CHAR( IDS_WND_SKILLLEARN ) );	 
	fw->SetVisible( false );
	fw->SetWndProc( this );
	fw->Initialize();

	SRect sTempRect;

	sTempRect.x	= 5;
	sTempRect.y	= 51;
	sTempRect.w	= 598;
	sTempRect.h	= 107;

#ifdef APPLY_ENGLISH_ORIGIN_SIZE
	
	sTempRect.w	= 698;

#endif	

	///-- Image9
	img9 = (CImageBox9*)nRui->CreateWnd( n_wtImageBox9, WID_None, WID_SkillLearn_Wnd, 
										 sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );
	img9->SetWndTileSizeLR( 9, 9 );
	img9->SetWndTileSizeTB( 8, 8 );
	img9->SetFocusCheck( false );
	img9->SetClientImage( TID_CTRL_CONTBACK_L1 );
	img9->SetAdjustPosition( 0, 0 );

	sTempRect.x	= 5;
	sTempRect.y	= 162;
	sTempRect.w	= 598;
	sTempRect.h	= 104;

#ifdef APPLY_ENGLISH_ORIGIN_SIZE
	
	sTempRect.w	= 698;

#endif	
	
	img9 = (CImageBox9*)nRui->CreateWnd( n_wtImageBox9, WID_None, WID_SkillLearn_Wnd, 
										 sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );       	
	img9->SetWndTileSizeLR( 9, 9 );
	img9->SetWndTileSizeTB( 8, 8 );
	img9->SetFocusCheck( false );
	img9->SetClientImage( TID_CTRL_CONTBACK_L1 );
	img9->SetAdjustPosition( 0, 0 );

	///-- 직업(Static)
	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_SkillLearn_Wnd, 20, 30, 133, 17, false );
	tbox->SetFontR( n_fRsvFontWndStatic );	
	tbox->SetAlignText( n_atLeft, n_atCenter );
	Rsprintf( RWCHAR(strTemp) , _RT("%s"), G_STRING_CHAR( IDS_CHARSTATE_CLASS ) ); 
	tbox->SetText( strTemp ); 

	///-- 필요스킬1(Static)
#ifdef CLINET_SKILL_LERNCOST

	sTempRect.x	= 256;
	sTempRect.y	= 30;
	sTempRect.w	= 100;
	sTempRect.h	= 17;

#ifdef APPLY_ENGLISH_ORIGIN_SIZE
	
	sTempRect.x	= 306;

#endif	
	
	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_SkillLearn_Wnd, 
									   sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );
	tbox->SetFontR( n_fRsvFontWndStatic );	
	tbox->SetAlignText( n_atLeft, n_atCenter );
	tbox->SetText( G_STRING_CHAR(IDS_WND_LEARNING_COST)); 

#endif

	sTempRect.x	= 350;
	sTempRect.y	= 30;
	sTempRect.w	= 133;
	sTempRect.h	= 17;

#ifdef APPLY_ENGLISH_ORIGIN_SIZE
	
	sTempRect.x	= 400;

#endif	

	///-- 필요스킬1(Static)
	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_SkillLearn_Wnd, 
									   sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );
	tbox->SetFontR( n_fRsvFontWndStatic );	
	tbox->SetAlignText( n_atLeft, n_atCenter );
	Rsprintf( RWCHAR(strTemp), _RT("%s"), G_STRING_CHAR( IDS_WND_NEED_SKILL ) ); 
	tbox->SetText( strTemp ); 

	sTempRect.x	= 478;
	sTempRect.y	= 30;
	sTempRect.w	= 133;
	sTempRect.h	= 17;

#ifdef APPLY_ENGLISH_ORIGIN_SIZE
	
	sTempRect.x	= 528;

#endif	

	///-- 필요스킬(Static)
	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_SkillLearn_Wnd, 
									   sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );
	tbox->SetFontR( n_fRsvFontWndStatic );	
	tbox->SetAlignText( n_atLeft, n_atCenter );
	Rsprintf( RWCHAR(strTemp), _RT("%s"), G_STRING_CHAR( IDS_WND_NEED_SKILL ) );
	tbox->SetText( strTemp ); 
	
	// 직업 
	ClassName = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_SkillLearn_ClassText, WID_SkillLearn_Wnd, 63, 30, 64, 16, false );
	ClassName->SetFontg( n_ftGulimChe, 12, n_fwBold, n_fdNormal );
	ClassName->SetClientImage( TID_None );
	ClassName->SetAlignText( n_atLeft, n_atCenter );	
	ClassName->SetClientColor( 180, 175, 160 );	
	ClassName->SetTextColor( 25, 25, 35 );
	ClassName->SetHID( HID_None );
	ClassName->SetText( _RT("") );

	sTempRect.x	= 592;
	sTempRect.y	= 5;
	sTempRect.w	= 13;
	sTempRect.h	= 13;

#ifdef APPLY_ENGLISH_ORIGIN_SIZE
	
	sTempRect.x	= 692;

#endif	
		
	// 닫기 버튼
	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_SkillLearn_Close_Btn , WID_SkillLearn_Wnd , 
									 sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );
	btn->SetBtnImage( TID_CTRL_BTN_X, TID_CTRL_BTN_X_OVR, TID_CTRL_BTN_X_CLK, TID_None );	

	sTempRect.x	= 600;
	sTempRect.y	= 52;
	sTempRect.w	= 15;
	sTempRect.h	= 105;

#ifdef APPLY_ENGLISH_ORIGIN_SIZE
	
	sTempRect.x	= 700;

#endif	

	// 스크롤바 ( Quest_List )
	ListScroll = (CScrollBar*)nRui->CreateWnd( n_wtScrollBar, WID_SkillLearn_List_Scroll , WID_SkillLearn_Wnd, 
											   sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );
	ListScroll->InitSlide( stVertSld , 0 , 195 , 0 , 13 );//Type, min, max, value, 썸의 폭
	ListScroll->SetClientImage( TID_CTRL_SCR_BG );
	ListScroll->SetThumbImage( TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR_CLK, TID_None );
	ListScroll->SetScrollBtnA( TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT_CLK, TID_None );
	ListScroll->SetScrollBtnB( TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT_CLK, TID_None );	

	sTempRect.x	= 600;
	sTempRect.y	= 163;
	sTempRect.w	= 15;
	sTempRect.h	= 102;

#ifdef APPLY_ENGLISH_ORIGIN_SIZE
	
	sTempRect.x	= 700;

#endif	
			
	// 스크롤바 ( Context )	
	DescScroll = (CScrollBar*)nRui->CreateWnd( n_wtScrollBar, WID_SkillLearn_Context_Scroll , WID_SkillLearn_Wnd, 
											   sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );
	DescScroll->InitSlide( stVertSld , 0 , 5 , 0 , 13 );//Type, min, max, value, 썸의 폭
	DescScroll->SetClientImage( TID_CTRL_SCR_BG );
	DescScroll->SetThumbImage( TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR_CLK, TID_None );
	DescScroll->SetScrollBtnA( TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT_CLK, TID_None );
	DescScroll->SetScrollBtnB( TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT_CLK, TID_None );
	
	int nAddXValue = 0;

#ifdef APPLY_ENGLISH_ORIGIN_SIZE
	
	nAddXValue = 50;

#endif	
	
	//-------------------------------------------------------------------------
	SkillLearnField[0].SkillName = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_SkillLearn_SkillLearnName1_Txt, WID_SkillLearn_Wnd, 
								   18, 67, 302, 16, false );
	SkillLearnField[0].SkillName->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	SkillLearnField[0].SkillName->SetClientImage( TID_None );
	SkillLearnField[0].SkillName->SetAlignText( n_atLeft, n_atCenter );
	SkillLearnField[0].NeedSkill1 = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_SkillLearn_NeedSkill11_Txt, WID_SkillLearn_Wnd, 
								   nAddXValue + 354, 67, 98, 16, false );
	SkillLearnField[0].NeedSkill1->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	SkillLearnField[0].NeedSkill1->SetClientImage( TID_None );
	SkillLearnField[0].NeedSkill1->SetAlignText( n_atLeft, n_atCenter );
	SkillLearnField[0].NeedSkill2 = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_SkillLearn_NeedSkill21_Txt, WID_SkillLearn_Wnd, 
								   nAddXValue + 479, 67, 98, 16, false );
	SkillLearnField[0].NeedSkill2->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	SkillLearnField[0].NeedSkill2->SetClientImage( TID_None );
	SkillLearnField[0].NeedSkill2->SetAlignText( n_atLeft, n_atCenter );
	SkillLearnField[0].TxtLernCost = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_SkillLearn_LernCost_1_Txt, WID_SkillLearn_Wnd, 
								   nAddXValue + 256 , 67 , 98, 16, false );
	SkillLearnField[0].TxtLernCost->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	SkillLearnField[0].TxtLernCost->SetClientImage( TID_None );
	SkillLearnField[0].TxtLernCost->SetAlignText( n_atLeft, n_atCenter );


	//-------------------------------------------------------------------------
	SkillLearnField[1].SkillName = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_SkillLearn_SkillLearnName2_Txt, WID_SkillLearn_Wnd, 
									18, 89, 302, 16, false );
	SkillLearnField[1].SkillName->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	SkillLearnField[1].SkillName->SetClientImage( TID_None );
	SkillLearnField[1].SkillName->SetAlignText( n_atLeft, n_atCenter );
	SkillLearnField[1].NeedSkill1 = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_SkillLearn_NeedSkill12_Txt, WID_SkillLearn_Wnd, 
									nAddXValue + 354, 89, 98, 16, false );
	SkillLearnField[1].NeedSkill1->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	SkillLearnField[1].NeedSkill1->SetClientImage( TID_None );
	SkillLearnField[1].NeedSkill1->SetAlignText( n_atLeft, n_atCenter );
	SkillLearnField[1].NeedSkill2 = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_SkillLearn_NeedSkill22_Txt, WID_SkillLearn_Wnd, 
									nAddXValue + 479, 89, 98, 16, false );
	SkillLearnField[1].NeedSkill2->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	SkillLearnField[1].NeedSkill2->SetClientImage( TID_None );
	SkillLearnField[1].NeedSkill2->SetAlignText( n_atLeft, n_atCenter );	
	SkillLearnField[1].TxtLernCost = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_SkillLearn_LernCost_2_Txt, WID_SkillLearn_Wnd, 
									nAddXValue + 256, 89, 98, 16, false );
	SkillLearnField[1].TxtLernCost->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	SkillLearnField[1].TxtLernCost->SetClientImage( TID_None );
	SkillLearnField[1].TxtLernCost->SetAlignText( n_atLeft, n_atCenter );

	
	//-------------------------------------------------------------------------
	SkillLearnField[2].SkillName = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_SkillLearn_SkillLearnName3_Txt, WID_SkillLearn_Wnd, 
									18, 111, 302, 16, false );
	SkillLearnField[2].SkillName->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	SkillLearnField[2].SkillName->SetClientImage( TID_None );
	SkillLearnField[2].SkillName->SetAlignText( n_atLeft, n_atCenter );
	SkillLearnField[2].NeedSkill1 = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_SkillLearn_NeedSkill13_Txt, WID_SkillLearn_Wnd, 
									nAddXValue + 354, 111, 98, 16, false );
	SkillLearnField[2].NeedSkill1->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	SkillLearnField[2].NeedSkill1->SetClientImage( TID_None );
	SkillLearnField[2].NeedSkill1->SetAlignText( n_atLeft, n_atCenter );
	SkillLearnField[2].NeedSkill2 = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_SkillLearn_NeedSkill23_Txt, WID_SkillLearn_Wnd, 
									nAddXValue + 479, 111, 98, 16, false );
	SkillLearnField[2].NeedSkill2->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	SkillLearnField[2].NeedSkill2->SetClientImage( TID_None );
	SkillLearnField[2].NeedSkill2->SetAlignText( n_atLeft, n_atCenter );	
	SkillLearnField[2].TxtLernCost = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_SkillLearn_LernCost_3_Txt, WID_SkillLearn_Wnd, 
									nAddXValue + 256, 111, 98, 16, false );
	SkillLearnField[2].TxtLernCost->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	SkillLearnField[2].TxtLernCost->SetClientImage( TID_None );
	SkillLearnField[2].TxtLernCost->SetAlignText( n_atLeft, n_atCenter );


	//-------------------------------------------------------------------------
	SkillLearnField[3].SkillName = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_SkillLearn_SkillLearnName4_Txt, WID_SkillLearn_Wnd,
									18, 133, 302, 16, false );
	SkillLearnField[3].SkillName->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	SkillLearnField[3].SkillName->SetClientImage( TID_None );
	SkillLearnField[3].SkillName->SetAlignText( n_atLeft, n_atCenter );
	SkillLearnField[3].NeedSkill1 = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_SkillLearn_NeedSkill14_Txt, WID_SkillLearn_Wnd, 
									nAddXValue + 354, 133, 98, 16, false );
	SkillLearnField[3].NeedSkill1->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	SkillLearnField[3].NeedSkill1->SetClientImage( TID_None );
	SkillLearnField[3].NeedSkill1->SetAlignText( n_atLeft, n_atCenter );
	SkillLearnField[3].NeedSkill2 = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_SkillLearn_NeedSkill24_Txt, WID_SkillLearn_Wnd, 
									nAddXValue + 479, 133, 98, 16, false );
	SkillLearnField[3].NeedSkill2->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	SkillLearnField[3].NeedSkill2->SetClientImage( TID_None );
	SkillLearnField[3].NeedSkill2->SetAlignText( n_atLeft, n_atCenter );	

	SkillLearnField[3].TxtLernCost = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_SkillLearn_LernCost_4_Txt, WID_SkillLearn_Wnd, 
									nAddXValue + 256, 133, 98, 16, false );
	SkillLearnField[3].TxtLernCost->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	SkillLearnField[3].TxtLernCost->SetClientImage( TID_None );
	SkillLearnField[3].TxtLernCost->SetAlignText( n_atLeft, n_atCenter );
	//-------------------------------------------------------------------------	

	nAddXValue = 0;

#ifdef APPLY_ENGLISH_ORIGIN_SIZE
	
	nAddXValue = 100;

#endif	

	// 퀘스트 셀렉트 버튼 4개 
	SelBtn[0] = (CButton*)nRui->CreateWnd( n_wtButton, WID_SkillLearn_Field0_Btn, WID_SkillLearn_Wnd, 
										   12, 62, nAddXValue + 583, 21, false );
	SelBtn[0]->SetClientImage( TID_None );
	SelBtn[0]->SetBtnImage( TID_None, TID_CTRL_TEXT_OVER, TID_CTRL_TEXT_OVER, TID_None );

	SelBtn[1] = (CButton*)nRui->CreateWnd( n_wtButton, WID_SkillLearn_Field1_Btn, WID_SkillLearn_Wnd, 
										   12, 84, nAddXValue + 583, 21, false );
	SelBtn[1]->SetClientImage( TID_None );
	SelBtn[1]->SetBtnImage( TID_None, TID_CTRL_TEXT_OVER, TID_CTRL_TEXT_OVER, TID_None );

	SelBtn[2] = (CButton*)nRui->CreateWnd( n_wtButton, WID_SkillLearn_Field2_Btn, WID_SkillLearn_Wnd, 
										   12, 106, nAddXValue + 583, 21, false );
	SelBtn[2]->SetClientImage( TID_None );
	SelBtn[2]->SetBtnImage( TID_None, TID_CTRL_TEXT_OVER, TID_CTRL_TEXT_OVER, TID_None );

	SelBtn[3] = (CButton*)nRui->CreateWnd( n_wtButton, WID_SkillLearn_Field3_Btn, WID_SkillLearn_Wnd, 
										   12, 128, nAddXValue + 583, 21, false );
	SelBtn[3]->SetClientImage( TID_None );
	SelBtn[3]->SetBtnImage( TID_None, TID_CTRL_TEXT_OVER, TID_CTRL_TEXT_OVER, TID_None );

	// 퀘스트 내용
	SkillDescText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_SkillLearn_ContextText, WID_SkillLearn_Wnd, 
												18, 172, nAddXValue + 583 , 50, false );
	SkillDescText->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	SkillDescText->SetClientImage( TID_None );
	SkillDescText->SetClientColor( 180, 175, 160 );
	SkillDescText->SetMultiLine( TRUE );
	SkillDescText->SetTextColor( 255 ,255 ,255 );
	SkillDescText->SetHID( HID_None );
	SkillDescText->SetText( _RT("") );

	// 습득 버튼
	LearnBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_SkillLearn_Learn_Btn , WID_SkillLearn_Wnd ,  532, 269, 58, 25, false );
	LearnBtn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_CTRL_BTN58_DIS );		
	LearnBtn->SetFontR( n_fRsvFontWndButton );
	LearnBtn->SetText( G_STRING_CHAR( IDS_WND_LEARN_BTN ) );	


#ifdef CLINET_SKILL_LERNCOST
#else
	SkillLearnField[0].TxtLernCost->SetVisible(false);
	SkillLearnField[1].TxtLernCost->SetVisible(false);
	SkillLearnField[2].TxtLernCost->SetVisible(false);
	SkillLearnField[3].TxtLernCost->SetVisible(false);
#endif

	return;
}