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

#include "MiniMapWndProc.h"
#include "QuestProc.h"
#include "MainMenuWndProc.h"


#include "..\\..\\Effect.h"
#include "stringmanager.h"
CQuestWnd	QuestWnd;


extern bool	gVisibleQuestWnd;
extern long g_nowTime;

const BYTE c_QuestType_Scroll( 63 );
const BYTE c_QuestType_Wait( 59 );

int SortQList( const void *arg1, const void *arg2 )
{
	SQuestFlag	** a1 = (SQuestFlag	**)arg1;
	SQuestFlag	** a2 = (SQuestFlag	**)arg2;	

	int Rt =  (*a1)->theQuestCode - (*a2)->theQuestCode;

	if( Rt < 0 )
		return -1;

	if( Rt > 0 )
		return 1;

	return 0;
}


//-----------------------------------------------------------------------------
void CQuestWnd::Init()
{
	TabState		= n_QUEST_ALL_TAB;
	
	QuestInfoTable	= g_Pc_Manager.GetQuestInfoTable();
	PcInven			= nRui->GetPcInvenInfo();
	SPcDataInven* PcInven2 = nRui->GetPcInvenInfo();
	InsertHighlight = false;
	QuestCntStartListNum = -1;
	SelectedBtn = -1;

	SetQuestInfo();
}
//-----------------------------------------------------------------------------
void CQuestWnd::CompositionNew()
{
	int center_x	= nRui->Desktop.Width / 2;
	int center_y	= nRui->Desktop.Height / 2;
	
	CFrameWnd*		fw		   = NULL;
	CButton*		btn		   = NULL;
	CStateButton*	sbtn       = NULL;
	SRect			questwnd;
	CImageBox9*		img9;
	CImageBox*		img;
	CTextBox*		tbox;

	questwnd.w	= 336*2 + 4 ;
	questwnd.h	= 500;
	
	questwnd.x	= center_x - ( questwnd.w / 2 );
	questwnd.y	= center_y - ( questwnd.h / 2 );
	
	///-- Frame Wnd
	fw = (CFrameWnd *)nRui->CreateWnd( n_wtFrameWnd, WID_Quest_Wnd, WID_None, questwnd.x, questwnd.y, questwnd.w, questwnd.h, false );
	fw->SetWndTexStyle( fwfTex_Style_Tile9 );			// 1, 3, 9 
	fw->SetClientImage( TID_CTRL_WND_L1 );				// 좌측 위 ( 9개 일때 ) TID 정렬 필수 
	fw->SetIconTID( TID_CTRL_ICON_QUEST );				// 창마다 아이콘 표시 (디폴트 있음)
	fw->SetWndTileSizeLR( 40, 29 );						// TID 넓이 Left, Right		// 그라데이션 사용시 기본 3픽셀 사용하면 좋음.  		
	fw->SetWndTileSizeTB( 31, 11 );						// TID 높이 Top, Bottom		// 위아래 가운데는 자동 패턴. 
	fw->SetCaption( G_STRING_CHAR( IDS_WND_QUEST ) );	// 윈도우 창 이름 
	fw->SetTitle( true );								// 표시 없으면 사용 안함. 
	
	fw->SetWndProc( this );		
	fw->SetVisible( false );
	fw->Initialize();

	// SetFontR()로 그룹화된 폰트 사용시 (버튼은 정형화 완료, 배틀존 예외)
	// SetFontg() 개별적인 폰트를 따로 사용시 
	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_Quest_Wnd, 12, 34, 70, 17, false );
	tbox->SetFontR( n_fRsvFontWndStatic );
	tbox->SetTextColor( 0, 0, 0 );
	tbox->SetAlignText( n_atLeft, n_atCenter);	 	
	tbox->SetText( G_STRING_CHAR( IDS_WND_QUEST_NAME ) );

	///-- 이 아래쪽 텍스트는 사용하지 않는듯....
	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_Quest_Wnd, 15, 134, 252, 15, false ); 
	tbox->SetFontR( n_fRsvFontWndStatic );
	tbox->SetAlignText( n_atLeft, n_atCenter);	 
	tbox->SetTextColor( 0, 0, 0 );
	tbox->SetText( G_STRING_CHAR( IDS_WND_QUEST_ORDER ));
	tbox->SetFocusCheck( false );
	tbox->SetVisible( false );	
	
	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_Quest_Wnd, 15, 164, 252, 15, false );  
	tbox->SetFontR( n_fRsvFontWndStatic );
	tbox->SetAlignText( n_atLeft, n_atCenter);	 
	tbox->SetTextColor( 0, 0, 0 );
	tbox->SetText( G_STRING_CHAR( IDS_WND_QUEST_LIMIT ) );
	tbox->SetFocusCheck( false );
	tbox->SetVisible( false );
	
	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_Quest_Wnd, 15, 149, 252, 15, false ); 
	tbox->SetFontR( n_fRsvFontWndStatic );
	tbox->SetAlignText( n_atLeft, n_atCenter);	 
	tbox->SetTextColor( 0, 0, 0 );
	tbox->SetText( G_STRING_CHAR( IDS_WND_QUEST_RELEVEL ) );
	tbox->SetFocusCheck( false );
	tbox->SetVisible( false );
	
	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_Quest_Wnd, 15, 179, 252, 15, false );  
	tbox->SetFontR( n_fRsvFontWndStatic );
	tbox->SetTextColor( 0, 0, 0 );
	tbox->SetAlignText( n_atLeft, n_atCenter);	 
	tbox->SetText( G_STRING_CHAR( IDS_WND_QUEST_CONDI ) );
	tbox->SetFocusCheck( false );
	tbox->SetVisible( false );
	



	///---------------------------------------------------------------------------	
	///-- Static
	///---------------------------------------------------------------------------		
	// 음영 또는 라인은 TID IMG 따로 생성해서 사용 
	///-- Line	
	//img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_Quest_Wnd, 1, 168, 334, 169, false );   
	//img ->SetClientImage( TID_CTRL_BACK );
	//img->SetFocusCheck( false );

//	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_Quest_Wnd, 340, 214, 334, 1, false );   
//	img ->SetClientImage( TID_CTRL_LINEDOT );


	// 이미지 범용 사용 및 리소스 사이즈 절감 효과 (리스트 박스, 뒷배경으로 깔리는 여러 이미지들 )
	///-- Image9
	img9 = (CImageBox9*)nRui->CreateWnd( n_wtImageBox9, WID_None, WID_Quest_Wnd, 3, 54, 330, 400, false );       	
	img9->SetWndTileSizeLR( 9, 9 );					// 윈도우 생성시와 동일 
	img9->SetWndTileSizeTB( 8, 8 );					// 윈도우 생성시와 동일 
	img9->SetFocusCheck( false );					// 이미지 사용시 필수 
	img9->SetClientImage( TID_CTRL_CONTBACK_L1 );
	img9->SetAdjustPosition( 0, 0 );				// n_wtImageBox9 사용시 필수 호출 

	img9 = (CImageBox9*)nRui->CreateWnd( n_wtImageBox9, WID_None, WID_Quest_Wnd, 342, 54, 330, 440, false );       	
	img9->SetWndTileSizeLR( 9, 9 );
	img9->SetWndTileSizeTB( 8, 8 );
	img9->SetFocusCheck( false );
	img9->SetClientImage( TID_CTRL_CONTBACK_L1 );
	img9->SetAdjustPosition( 0, 0 );

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_Quest_Wnd, 342, 272, 330, 1, false ); 
	img ->SetClientImage( TID_CTRL_LINEDOT );

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_Quest_Wnd, 342, 274+60, 330, 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_Quest_Wnd, 342, 336+ 15+ 15 , 330, 1, false ); 
	img ->SetClientImage( TID_CTRL_LINEDOT );



	StartNpc = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_Quest_Wnd, 349 ,  337 , 252, 30, false );  
	StartNpc->SetFontR( n_fRsvFontWndStatic2 );
	StartNpc->SetText(_RT(""));//test
	StartNpc->SetMultiLine( TRUE );
	StartNpc->SetLineInterval(2);
	StartNpc->SetHID( HID_None );
	StartNpc->SetText(_RT(""));
	StartNpc->SetTextColor(0 , 213 , 0 );
	
	///---------------------------------------------------------------------------	
	///-- Control
	///---------------------------------------------------------------------------	
	///-- X		
	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_Quest_Close_Btn , WID_Quest_Wnd , 653, 7, 13, 13, false );
	btn->SetBtnImage( TID_CTRL_BTN_X, TID_CTRL_BTN_X_OVR, TID_CTRL_BTN_X_CLK, TID_None );

	// 퀘스트포기 버튼
	CancelBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_Quest_Cancel_Btn , WID_Quest_Wnd , 13, 467 , 87, 25, false );	
	CancelBtn->SetBtnImage( TID_CTRL_BTN87, TID_CTRL_BTN87_OVR, TID_CTRL_BTN87_CLK, TID_CTRL_BTN87_DIS );	
	CancelBtn->SetFontR( n_fRsvFontWndButton ); 
	CancelBtn->SetText( G_STRING_CHAR( IDS_WND_QUEST_GIVUP ) );
	CancelBtn->SetBtnHold( true );
 
	// 스크롤바 ( Quest_List )
	ListScroll = (CScrollBar*)nRui->CreateWnd( n_wtScrollBar, WID_Quest_List_Scroll , WID_Quest_Wnd, 320 , 55, 13 , 398 , false );
	ListScroll->InitSlide( stVertSld , 0 , RP_MAX_QUEST_FLAG - QUEST_FIELD_NUM , 0 , 13 );//Type, min, max, value, 썸의 폭
	ListScroll->SetClientImage( TID_CTRL_SCR_BG );
	ListScroll->SetThumbImage( TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR_CLK, TID_None );
	ListScroll->SetScrollBtnA( TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT_CLK, TID_None );
	ListScroll->SetScrollBtnB( TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT_CLK, TID_None );
	
	// 스크롤바 ( Context )
	DescScroll = (CScrollBar*)nRui->CreateWnd( n_wtScrollBar, WID_Quest_Context_Scroll , WID_Quest_Wnd, 658 , 55 , 13 , 217, false );
	DescScroll->InitSlide( stVertSld , 0 , 99 , 0 , 13 );//Type, min, max, value, 썸의 폭
	DescScroll->SetClientImage( TID_CTRL_SCR_BG );
	DescScroll->SetThumbImage( TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR_CLK, TID_None );
	DescScroll->SetScrollBtnA( TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT_CLK, TID_None );
	DescScroll->SetScrollBtnB( TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT_CLK, TID_None );

#ifdef QUEST_COUTLIST_RENEWAL
	

	// 스크롤바 ( Context )
	QuestCountScroll = (CScrollBar*)nRui->CreateWnd( n_wtScrollBar, WID_None , WID_Quest_Wnd, 658 , 272 , 13 , 63, false );
	QuestCountScroll->InitSlide( stVertSld , 0 , 9 , 0 , 13 );//Type, min, max, value, 썸의 폭
	QuestCountScroll->SetClientImage( TID_CTRL_SCR_BG );
	QuestCountScroll->SetThumbImage( TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR_CLK, TID_None );
	QuestCountScroll->SetScrollBtnA( TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT_CLK, TID_None );
	QuestCountScroll->SetScrollBtnB( TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT_CLK, TID_None );
	
	//여기 이미지는 포커스 얻기 위해서 추가한다. 스크롤을 돌리기 위해
	QuestCntScollFocus = (CImageBox9*)nRui->CreateWnd( n_wtImageBox9, WID_None, WID_Quest_Wnd, 342, 272 , 330 - 14 , 63 , false );       	
	QuestCntScollFocus->SetWndTileSizeLR( 9, 9 );
	QuestCntScollFocus->SetWndTileSizeTB( 8, 8 );
	QuestCntScollFocus->SetFocusCheck( true  );
	QuestCntScollFocus->SetClientImage( TID_CTRL_CONTBACK_L1 );
	QuestCntScollFocus->SetAdjustPosition( 0, 0 );
	QuestCntScollFocus->SetClientAlpah(0 );
	


#endif 



	for( int i = 0 ; i < QUEST_FIELD_NUM ; i++ )
	{

	
		QField[i].QuestName = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None , WID_Quest_Wnd, 12, 63 +( 20 * i ) , 237, 15, false );  
		QField[i].QuestName->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
		QField[i].QuestName->SetClientImage( TID_None );
		QField[i].QuestName->SetAlignText( n_atLeft, n_atCenter );
		
		QField[i].TimeLimit = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_Quest_Wnd, 342, 34 , 320, 15, false );
		QField[i].TimeLimit->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
		QField[i].TimeLimit->SetClientImage( TID_None );
		QField[i].TimeLimit->SetAlignText( n_atRight, n_atCenter );
		QField[i].TimeLimit->SetText(_RT(""));
		QField[i].TimeLimit->SetTextColor( 0, 0, 0 );

		///-- 이건 안쓰는듯...;;
	

		for( int j = 0 ; j < 4 ; j++ )
		{
			QField[i].CompleteItem[j] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_Quest_Wnd, 
																	349, 274 + ( 15 * j ), 252, 15, false ); 
			QField[i].CompleteItem[j]->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
			QField[i].CompleteItem[j]->SetClientImage( TID_None );
			QField[i].CompleteItem[j]->SetTextColor( 255, 165, 64 );
			QField[i].CompleteItem[j]->SetAlignText( n_atLeft, n_atCenter );
			QField[i].CompleteItem[j]->SetText(_RT(""));
			QField[i].CompleteItem[j]->SetFocusCheck(false);
		}
	
		SelBtn[i] = (CButton*)nRui->CreateWnd( n_wtButton, WID_None , WID_Quest_Wnd , 12, 63 +( 20 * i ) , 305, 15, false );  
		SelBtn[i]->SetClientImage( TID_None );
		SelBtn[i]->SetBtnImage( TID_None, TID_CTRL_TEXT_OVER, TID_CTRL_TEXT_OVER, TID_None );

		
		QField[i].QuestFlag = NULL;

	}
//
	// 퀘스트 내용
	QuestDescText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_Quest_ContextText, WID_Quest_Wnd, 349 ,  60 , 307, 210, false );  
	QuestDescText->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	QuestDescText->SetClientImage( TID_None );
	QuestDescText->SetText(_RT(""));
	QuestDescText->SetMultiLine( TRUE );
	QuestDescText->SetLineInterval(4);
	
	//퀘스트 10줄로 늘림 .. 
	//그리고 번역되서 넘어온것은 .. 처리한다 . 
	//글자의 맥스 넓이 긿이를 넘어 갔을시 .. 






	QuestReceiveText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_Quest_Wnd, 349 ,  375  , 307, 115, false );  
	QuestReceiveText->SetFontR( n_fRsvFontWndStatic2 );
	QuestReceiveText->SetText(_RT(""));//test
	QuestReceiveText->SetMultiLine( TRUE );
	QuestReceiveText->SetLineInterval(4);
	QuestReceiveText->SetTextColor(223,223,223);



	return;
	



}



void CQuestWnd::Composition()
{
#ifdef QUEST_RENEWAL
	CompositionNew();
#else 
	CompositionOld();
#endif

		
}


void CQuestWnd::QuestTimeCheck(int SelBtn)
{
	
	char temp[255] = {0,};

	for(int i =0 ; i < QUEST_FIELD_NUM ; ++i)
	{
		QField[i].TimeLimit->SetText(_RT(""));				
	}
	
	if( ( QField[SelBtn].QuestFlag != NULL ) )
	{			
		QField[SelBtn].TimeLimit->SetText(_RT(""));	
		
		int QuestTime = 0 ;
		bool bCheck = false ; 
		
		for(int  cnt = 0; cnt < RP_MAX_QUEST_COUNT; ++cnt )
		{
			if( PcInven->QuestTime[cnt] == NULL)
			{
				break;
			}
			
			if( (PcInven->QuestTime[cnt] != NULL)  && 
				PcInven->QuestTime[cnt]->wdQuestCode == QField[SelBtn].QuestFlag->theQuestCode  )
			{
				int GapTime = SAFE_TIME_SUB( g_nowTime , PcInven->QuestTime[cnt]->dwStartTime );
				
				QuestTime = PcInven->QuestTime[cnt]->iCountTime - GapTime;
				
				if( QuestTime <= 0)	QuestTime = 0 ; 
				
				if( QuestTime == 0 )
				{
					Rsprintf( RWCHAR(temp) , G_STRING_CHAR(IDS_WND_LIMIT_TIME_END) );
					QField[SelBtn].TimeLimit->SetText(temp);
				}
				else
				{
					int Min = (QuestTime/1000)/60;
					int Sec = (QuestTime/1000)%60;
					
					if(Min)
					{
						Rsprintf( RWCHAR(temp ), G_STRING_CHAR(IDS_WND_LIMIT_TIME_MIN), Min  );
						QField[SelBtn].TimeLimit->SetText( temp );
					}
					else
					{
						Rsprintf( RWCHAR(temp) , G_STRING_CHAR(IDS_WND_LIMIT_TIME_SEC) , 1 );
						//sprintf( temp, G_STRING_CHAR( IDW_MINIUTES ), QuestMinute );
						QField[SelBtn].TimeLimit->SetText( temp );
					}
				}				
				
				break;
			}
		}
	}	
	
}

//-----------------------------------------------------------------------------
void CQuestWnd::Update()
{
	if( IsVisible && InsertHighlight )
	{
		InsertHighlight = false;
		g_main_buttons.GetQuestButton()->SetBtnImage( TID_OPT_QUEST_BT, TID_OPT_QUEST_BT_OVR, TID_OPT_QUEST_BT_CLK, TID_None );
	}
	
	if( InsertHighlight )
	{		
		static long quest_time = timeGetTime();
		static bool OnTwinkle = false;
		
		if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( timeGetTime() , quest_time ) , > , 500 ) )
		{
			quest_time = timeGetTime();
			
			OnTwinkle = !OnTwinkle;
		}	
		
		if( OnTwinkle == true )
		{
			g_main_buttons.GetQuestButton()->SetBtnImage( TID_OPT_QUEST_BT_OVR, TID_OPT_QUEST_BT_OVR, TID_OPT_QUEST_BT_CLK, TID_None );
		}
		else
		{
			g_main_buttons.GetQuestButton()->SetBtnImage( TID_OPT_QUEST_BT, TID_OPT_QUEST_BT_OVR, TID_OPT_QUEST_BT_CLK, TID_None );			
		}		
	}
	
	

	
	
	return;
}

//-----------------------------------------------------------------------------
void CQuestWnd::Proc( SEventMessage* EMsg )
{


	switch( EMsg->FocusWnd ) 
	{
	case WID_Quest_Close_Btn:
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			nRui->SendEvent( WID_Quest_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
			IsVisible = false;
		}
		break;
	case WID_Quest_Cancel_Btn:
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			if( ( SelectedBtn != -1 ) && ( QField[SelectedBtn].QuestFlag != NULL ) )
			{
				//서버에 메세지 날리고....
				g_RockClient.Send_RpCsGiveupQuest( QField[SelectedBtn].QuestFlag->theQuestCode );
				//리스트 재설정
				//QuestCancel( QField[SelectedBtn].QuestFlag->theQuestCode );
			}
		}
		break;		
	case WID_Quest_List_Scroll:
		if( ( EMsg->Notify == NM_SLD_MOVEBAR ) || ( EMsg->Notify == NM_SLD_CLICKMOVEBAR ) || ( EMsg->Notify == NM_SLD_BTNMOVERBAR ) )
		{
			SetQuestListScroll( (int)EMsg->fParam );
			ScrollFocus = n_QuestScroll;
		}
		break;
	case WID_Quest_Context_Scroll:
		if( ( EMsg->Notify == NM_SLD_MOVEBAR ) || ( EMsg->Notify == NM_SLD_CLICKMOVEBAR ) || ( EMsg->Notify == NM_SLD_BTNMOVERBAR ) )
		{
			SetDecsTextScroll( (int)EMsg->fParam );
			ScrollFocus = n_DescScroll;
		}
		break;
	case WID_Quest_ContextText:
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			ScrollFocus = n_DescScroll;
		}
		break;
	}
#ifdef QUEST_COUTLIST_RENEWAL

	if( QuestCntScollFocus->GetID() == EMsg->FocusWnd) 
	{	
		ScrollFocus = n_QuestCountScroll;
	}

	if( EMsg->FocusWnd == QuestCountScroll->GetID() )
	{
		if( ( EMsg->Notify == NM_SLD_MOVEBAR ) || ( EMsg->Notify == NM_SLD_CLICKMOVEBAR ) || ( EMsg->Notify == NM_SLD_BTNMOVERBAR ) )
		{
			SetQuestCountListScroll( (int)EMsg->fParam );
			ScrollFocus = n_QuestCountScroll;
		}
	}
#endif 
	for(int i= 0 ; i < QUEST_FIELD_NUM ; ++i  )
	{
		if( EMsg->FocusWnd == SelBtn[i]->GetID() )
		{
			if( EMsg->Notify == NM_BTN_CLICK )
			{

				QuestBntSelected( i );
#ifdef QUEST_COUTLIST_RENEWAL
				QuestCountScroll->SetValue(0);
				SetQuestCountListScroll(0);
#endif 
				ScrollFocus = n_QuestScroll;
			}
		}
	}
/*
	if( EMsg->FocusWnd == SelBtn[0]->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			QuestBntSelected( 0 );
			ScrollFocus = n_QuestScroll;
		}
	}

	if( EMsg->FocusWnd == SelBtn[1]->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			QuestBntSelected( 1 );
			ScrollFocus = n_QuestScroll;
		}
	}

	if( EMsg->FocusWnd == SelBtn[2]->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			QuestBntSelected( 2 );
			ScrollFocus = n_QuestScroll;
		}
	}

	if( EMsg->FocusWnd == SelBtn[3]->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			QuestBntSelected( 3 );
			ScrollFocus = n_QuestScroll;
		}
	}

	if( EMsg->FocusWnd == SelBtn[4]->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			QuestBntSelected( 4 );
			ScrollFocus = n_QuestScroll;
		}
	}
*/
}

///---------------------------------------------------------------------------
///-- OpenQuestWnd
///---------------------------------------------------------------------------
void CQuestWnd::OpenQuestWnd()
{
	ListScroll->SetValue( 0 );
	SetQuestListScroll( 0 );
		
	for(int i =0 ; i < QUEST_FIELD_NUM ; ++i)
	{
		QField[i].TimeLimit->SetText(_RT(""));				
	}


	nRui->SendEvent( WID_Quest_Wnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
}
//-----------------------------------------------------------------------------
void CQuestWnd::SetWheelValue( short _wheel )
{
	int value = 0;
	int Max   = 0;
	
	if( ScrollFocus == n_QuestScroll )
	{
		Max   = ListScroll->GetSldMax();
		ListScroll->GetValue( &value );
		
		if( ( _wheel < 0 ) && ( value < Max ) )
		{
			value += 1;
		}
		else if( ( _wheel > 0 ) && ( value > 0 ) )
		{
			value -= 1;
		}
		
		ListScroll->SetValue( value );
		SetQuestListScroll( value );
	}
	else if( ScrollFocus == n_DescScroll )
	{
		DescScroll->GetValue( &value );
		if( ( _wheel < 0 ) && ( value < 99 ) )
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
	else if(ScrollFocus == n_QuestCountScroll)
	{
		Max = QuestCountScroll->GetSldMax();
		QuestCountScroll->GetValue( &value );
		if( ( _wheel < 0 ) && ( value < Max ) )
		{
			value += 1;
		}
		else if( ( _wheel > 0 ) && ( value > 0 ) )
		{
			value -= 1;
		}
		
		QuestCountScroll->SetValue( value );
		SetQuestCountListScroll( value );
	}

}

//-----------------------------------------------------------------------------
void CQuestWnd::SetQuestSelect( WORD _quest_code )
{

	int nCount = 5;


	for( int i = 0; i < NumAllFlags; ++i )
	{
		if( ( QuestAllFlags[i] != NULL ) && ( QuestAllFlags[i]->theQuestCode == _quest_code ) )
		{
			for( int j = 0; j < nCount ; ++j )
			{
				if( QuestAllFlags[j+i] != NULL )
				{
					QField[j].QuestFlag = QuestAllFlags[j+i];
				}
			}

			StartListNum = i;
			ListScroll->SetValue( StartListNum );
			
			SetQuestField();
			break;
		}
	}
}

//-----------------------------------------------------------------------------
void CQuestWnd::QuestBntSelected( int _num_btn )
{
	SelectedBtn = _num_btn;	

	DescScroll->SetValue(0);
	SetDecsTextScroll( 0 );



	if( ( SelectedBtn != -1 ) && ( QField[SelectedBtn].QuestFlag != NULL ) &&
		( QField[SelectedBtn].QuestFlag->theFlag == c_QuestType_Scroll || 
		QField[SelectedBtn].QuestFlag->theFlag == c_QuestType_Wait ))			// 퀘스트 스크롤만 포기 가능 
	{
		CancelBtn->SetBtnHold( false );
	}
	else
	{
		CancelBtn->SetBtnHold( true );
	}
	

	for( int i = 0 ; i < QUEST_FIELD_NUM ; i++ )
	{
		
		QField[i].CompleteItem[0]->SetVisible( false );
		QField[i].CompleteItem[1]->SetVisible( false );
		QField[i].CompleteItem[2]->SetVisible( false );
		QField[i].CompleteItem[3]->SetVisible( false );
	}	


	if( _num_btn >= 0 && _num_btn < QUEST_FIELD_NUM )
	{
		QField[_num_btn].TimeLimit->SetVisible( true );				
		QField[_num_btn].CompleteItem[0]->SetVisible( true );
		QField[_num_btn].CompleteItem[1]->SetVisible( true );
		QField[_num_btn].CompleteItem[2]->SetVisible( true );
		QField[_num_btn].CompleteItem[3]->SetVisible( true );
	}
	
	if( SelectedBtn != -1 && QField[ SelectedBtn ].QuestFlag )
	{
		//선택 라인 반전
		for( int i = 0 ; i < QUEST_FIELD_NUM ; i++ )
		{
			SelBtn[i]->SetClientImage( TID_None );
		}
		
		SelBtn[SelectedBtn]->SetClientImage( TID_C_WND_QUEST_SEL );
		
		if( QField[ SelectedBtn ].QuestFlag )
		{		
			//퀘스트 설명
			char TempStr[ 1024 ] = {0,};
			char AddTempStr[ 1024 ] = {0,};
			
			QuestTimeCheck(SelectedBtn);
			

			SQuestInfo* tmp_quest_info = NULL;
			tmp_quest_info = g_Pc_Manager.GetQuestFlagInfo( QField[ SelectedBtn ].QuestFlag->theQuestCode, 
															QField[ SelectedBtn ].QuestFlag->theFlag );
			
			if( CheckQuestComplete( SelectedBtn, tmp_quest_info ) )
			{
				QField[SelectedBtn].QuestName->SetTextColor( 50, 250, 50 );
			}
#ifdef KULUBEACH_QUEST_NAME //쿨루비치 이벤트 퀘스트 네이밍 변경
			else if ( QField[SelectedBtn].QuestFlag->theQuestCode == 8314 || QField[SelectedBtn].QuestFlag->theQuestCode == 8315 )
			{
				QField[SelectedBtn].QuestName->SetTextColor( 255, 0, 0 );
			}
#endif // KULUBEACH_QUEST_NAME
			else
			{
				QField[SelectedBtn].QuestName->SetTextColor( 255, 255, 255 );
			}
			
			Rsprintf( RWCHAR(TempStr) , _RT(" %s") , RWCHAR(QField[SelectedBtn].Desc) );			
#ifdef QUEST_RENEWAL
			QuestDescText->SetText( TempStr );
			
			if( QField[ SelectedBtn ].QuestFlag )
			{
				SetRewardStrNew( QField[ SelectedBtn ].QuestFlag->theQuestCode, QField[ SelectedBtn ].QuestFlag->theFlag, AddTempStr );
				SAFE_STR_CPY( TempStr, AddTempStr, 1024 );
			}
			
			QuestReceiveText->SetText(TempStr);


#else 
		if( QField[ SelectedBtn ].QuestFlag )
			{
				SetRewardStr( QField[ SelectedBtn ].QuestFlag->theQuestCode, QField[ SelectedBtn ].QuestFlag->theFlag, AddTempStr );
				SAFE_STR_CAT( TempStr, AddTempStr, 1024 );
			}
			
			QuestDescText->SetText( TempStr );
#endif 
			
#ifdef QUEST_COUTLIST_RENEWAL
			SetCompleteItemTextNew( SelectedBtn, tmp_quest_info );
#else 
			SetCompleteItemText( SelectedBtn, tmp_quest_info );
#endif 

	
		}
	}
	else
	{
		//선택 라인 반전
		for( int i = 0 ; i < QUEST_FIELD_NUM ; i++ )
		{
			SelBtn[i]->SetClientImage( TID_None );
		}
		  
		QuestDescText->SetText( _RT("") );
#ifdef QUEST_RENEWAL
		QuestReceiveText->SetText(_RT(""));
		StartNpc->SetText(_RT(""));
#endif
	}
}

void CQuestWnd::SetQuestCountListScroll(int _linenum )
{

#ifdef QUEST_COUTLIST_RENEWAL
	if( SelectedBtn != -1 ) 
	{
		QuestCntStartListNum = _linenum * 1;
		
		int i =0 ;
		int FieldCount = 0 ; 
		
		for(i =0 ; i < MAX_QUEST_CONDITION ; ++i)
		{
			QField[ SelectedBtn ].CompleteItem[ i ]->SetText(_RT(""));
		}
		
		for ( i =QuestCntStartListNum ; i < MAX_QUEST_COUNT_LIST  ; ++i)
		{
			if( FieldCount < MAX_QUEST_CONDITION && 
				(Rstrcmp(QField[ SelectedBtn ].m_QuestCountList[i].TextStr , _RT("")) != 0  ) )
			{				
				QField[ SelectedBtn ].CompleteItem[ FieldCount ]->SetText( QField[ SelectedBtn ].m_QuestCountList[i].TextStr );
				++FieldCount;
			}
		}

	}

#endif 


}


//-----------------------------------------------------------------------------
void CQuestWnd::SetQuestListScroll( int _linenum )
{
	QuestBntSelected( -1 );
	
	StartListNum = _linenum * 1;

	for( int i = 0; i < QUEST_FIELD_NUM ; ++i )
	{
		switch( TabState )
		{
		case n_QUEST_ALL_TAB://
			QField[i].QuestFlag = QuestAllFlags[StartListNum + i];
			break;
		case n_QUEST_NORMAL_TAB://
			QField[i].QuestFlag = QuestNormalFlags[StartListNum + i];
			break;
		case n_QUEST_STORY_TAB://
			QField[i].QuestFlag = QuestStoryFlags[StartListNum + i];
			break;
		case n_QUEST_CHANGE_TAB://
			QField[i].QuestFlag = QuestChangeFlags[StartListNum + i];
			break;
		}
	}

	SetQuestField();

}

//-----------------------------------------------------------------------------
void CQuestWnd::SetDecsTextScroll( int _linenum )
{
	QuestDescText->SetStartLine( _linenum );
}

//-----------------------------------------------------------------------------
void CQuestWnd::SetQuestInfo()
{
	NumAllFlags		= 0;
	NumNormalFlags	= 0;
	NumStoryFlags	= 0;
	NumChangeFlags	= 0;
	
	memset( QuestAllFlags, NULL, sizeof( SQuestFlag* ) * RP_MAX_QUEST_FLAG );
	memset(	QuestNormalFlags, NULL, sizeof( SQuestFlag* ) * RP_MAX_QUEST_FLAG );
	memset( QuestStoryFlags, NULL, sizeof( SQuestFlag* ) * RP_MAX_QUEST_FLAG );
	memset( QuestChangeFlags, NULL, sizeof( SQuestFlag* ) * RP_MAX_QUEST_FLAG );
	
	QuestDescText->SetText( _RT("") );

#ifdef QUEST_RENEWAL
	QuestReceiveText->SetText( _RT(""));
	StartNpc->SetText(_RT(""));
#endif
	
	int i = 0;

	for( i = 0; i < RP_MAX_QUEST_FLAG; ++i )
	{
		if( ( PcInven->QuestFlag[i] != NULL ) && ( PcInven->QuestFlag[i]->theDisplay == n_OPEN ) )
		{
			///-- 없는 퀘스트 정보라면..
			SQuestInfo * pQuestInfo = g_Pc_Manager.GetQuestFlagInfo( PcInven->QuestFlag[i]->theQuestCode,
																	 PcInven->QuestFlag[i]->theFlag );
			if( NULL == pQuestInfo )
			{
				PcInven->QuestFlag[i]->theDisplay = n_CLOSE;
				continue;
			}
				
			QuestAllFlags[NumAllFlags] = PcInven->QuestFlag[i];
			NumAllFlags++;
		}
	}

	///-- 여기서 퀘스트를 소팅한다.
	qsort( (void *)QuestAllFlags, (size_t)NumAllFlags, 
					sizeof(SQuestFlag*), SortQList );
	
	for( i = 0; i < RP_MAX_QUEST_FLAG; ++i )
	{
		if( ( PcInven->QuestFlag[i] != NULL ) && ( PcInven->QuestFlag[i]->theDisplay == n_OPEN ) )
		{
			switch( g_Pc_Manager.GetQuestType( PcInven->QuestFlag[i]->theQuestCode ) )
			{
			case n_QUEST_NORMAL_TAB:
				QuestNormalFlags[NumNormalFlags] = PcInven->QuestFlag[i];
				NumNormalFlags++;
				break;
			case n_QUEST_STORY_TAB:
				QuestStoryFlags[NumStoryFlags] = PcInven->QuestFlag[i];
				NumStoryFlags++;
				break;
			case n_QUEST_CHANGE_TAB:
				QuestChangeFlags[NumChangeFlags] = PcInven->QuestFlag[i];
				NumChangeFlags++;
				break;
			}
		}
	}

	InsertHighlight = false;

	if( NumAllFlags > OldNumAllFlags)
	{
		InsertHighlight = true;
	}

	OldNumAllFlags = NumAllFlags;
}

//-----------------------------------------------------------------------------
void CQuestWnd::SetQuestField()
{
	SQuestInfo* tmp_quest_info = NULL;
	
	char temp[64] = {0,};
	char StrTemp[256] ={0,};

	for( int i = 0; i < QUEST_FIELD_NUM; ++i )
	{
		// open,close에 따라..퀘스트창에 표시여부 판단.
		//if( QField[i].QuestFlag->theDisplay != n_CLOSE )	
		
		if( QField[i].QuestFlag != NULL )
		{ 
			// 같은 코드 값이라도... Flag(0,1,.... 255)에 따라 다른 내용 출력
			tmp_quest_info = g_Pc_Manager.GetQuestFlagInfo( QField[i].QuestFlag->theQuestCode, 
															QField[i].QuestFlag->theFlag );
			
			if( tmp_quest_info != NULL )
			{				
				ZeroMemory(StrTemp , 256 );
				memcpy(StrTemp , tmp_quest_info->theName , 64 );
				
				QField[i].QuestName->SetText( StrTemp );
				
				if( CheckQuestComplete( i, tmp_quest_info ) )
				{
					QField[i].QuestName->SetTextColor( 50, 250, 50 );
				}
#ifdef KULUBEACH_QUEST_NAME //쿨루비치 이벤트 퀘스트 네이밍 변경
				else if ( QField[i].QuestFlag->theQuestCode == 8314 || QField[i].QuestFlag->theQuestCode == 8315 )
				{
					QField[i].QuestName->SetTextColor( 255, 0, 0 );
				}
#endif // KULUBEACH_QUEST_NAME
				else
				{
					QField[i].QuestName->SetTextColor( 255, 255, 255 );
				}

#ifdef QUEST_COUTLIST_RENEWAL
				SetCompleteItemTextNew( i, tmp_quest_info );			
#else 
				SetCompleteItemText( i, tmp_quest_info );			
#endif 
				Rstrcpy( RWCHAR(QField[i].Desc), RWCHAR(tmp_quest_info->theDesc ));
			}
			else
			{
				QField[i].QuestName->SetText( _RT("") );
				QField[i].TimeLimit->SetText( _RT("") );
				QField[i].CompleteItem[0]->SetText( _RT("") );
				QField[i].CompleteItem[1]->SetText( _RT("") );
				QField[i].CompleteItem[2]->SetText( _RT("") );
				QField[i].CompleteItem[3]->SetText( _RT("") );
				memset(QField[i].Desc , NULL , MAX_SIZE_1024);	
#ifdef QUEST_COUTLIST_RENEWAL
				memset(QField[i].m_QuestCountList , NULL , sizeof(QField[i].m_QuestCountList));
#endif 
					
#ifdef QUEST_RENEWAL
				StartNpc->SetText(_RT(""));
				QuestReceiveText->SetText(_RT(""));
#endif


			}			
		}
		else
		{
			QField[i].QuestName->SetText( _RT("") );
			QField[i].TimeLimit->SetText( _RT("") );
			QField[i].CompleteItem[0]->SetText( _RT("") );
			QField[i].CompleteItem[1]->SetText( _RT("") );
			QField[i].CompleteItem[2]->SetText( _RT("") );
			QField[i].CompleteItem[3]->SetText( _RT("") );
			memset(QField[i].Desc , NULL , MAX_SIZE_1024);
#ifdef QUEST_COUTLIST_RENEWAL
			memset(QField[i].m_QuestCountList , NULL , sizeof(QField[i].m_QuestCountList));
#endif 
			
#ifdef QUEST_RENEWAL
			StartNpc->SetText(_RT(""));
			QuestReceiveText->SetText(_RT(""));
#endif

		}
	}
}

void CQuestWnd::SetCompleteItemTextNew( int aQFieldIdx, SQuestInfo* aQuestInfo )
{
	int i = 0;
	int num = 0; 
	char temp[64] = {0,};
	int ScrollCount = 0 ; 

	//////////dongs 추가 일반 아이템을  체크 하자 .. ///////////////////////////
	// 한개만 체크가 아니라 아이템 인벤(60) 과 장비 창에 있는데 10 아이템 모두 체크 한다 .. 
	// 이벤 60 체크


	
	
	if( aQuestInfo )
	{
		ZeroMemory( &QField[aQFieldIdx].m_QuestCountList , sizeof(QField[aQFieldIdx].m_QuestCountList) );

		for( i = 0; i < MAX_QUEST_COUNT_LIST; ++i )
		{
		//	QField[ aQFieldIdx ].CompleteItem[ i ]->SetText( _RT("") );		
			
			for( num = 0; num < RP_MAX_QUEST_COUNT; ++num )
			{
				if( PcInven->QuestCount[num] )
				{
					if( ( PcInven->QuestCount[num]->theQuestCode == aQuestInfo->theCode ) &&		
						( PcInven->QuestCount[num]->theCode == aQuestInfo->theCountList[i].theCode ) &&
						( PcInven->QuestCount[num]->theType == aQuestInfo->theCountList[i].theType ) )
					{
						
						//일반 장비면 .. 카운트 리셋 시키고 재계산 한다 .. 퀘템은 알아서 정보를 받아오니깐 ... 음.. 나중에 다처리 .. 
						if( PcInven->QuestCount[num]->theType == n_ITEM )
						{
							PcInven->QuestCount[num]->theCount = 0;
							
							SItemBaseInfo* item_info = NULL;		
							item_info = g_Pc_Manager.GetItemBaseInfoPtr( PcInven->QuestCount[num]->theCode );
							
							if( item_info == NULL )
							{
								break;
							}
							
							//인벤 검사 있냐 없냐 ? 
							for(int k =0 ; k < c_MaxInvenItemNum; ++k)
							{
								if(PcInven->InvenItem[k] == NULL ) continue;
								
								if(PcInven->InvenItem[k]->Code == aQuestInfo->theCountList[i].theCode )
								{
									PcInven->QuestCount[num]->theCount +=  PcInven->InvenItem[k]->Amount;       
								}								
							}
							
							//장비 인벤 검사 
							for(int j=0; j < RP_MAX_EQUIP; ++j )
							{
								if(PcInven->WearItem[j]==NULL) continue;								
								
								if(PcInven->WearItem[j]->Code == aQuestInfo->theCountList[i].theCode)
								{
									PcInven->QuestCount[num]->theCount +=  PcInven->WearItem[j]->Amount;    
								}
							}
							
							//퀘스트인벤 검사 
							for(int qcnt=0; qcnt < RP_MAX_QUEST_ITEM; ++qcnt )
							{
								if(PcInven->InvenQuest[qcnt]==NULL) continue;								
								
								if(PcInven->InvenQuest[qcnt]->Code == aQuestInfo->theCountList[i].theCode)
								{
									PcInven->QuestCount[num]->theCount +=  PcInven->InvenQuest[qcnt]->Amount;    
								}
							}														
							
							if( item_info )
							{
								Rsprintf( RWCHAR(temp), _RT("%s ( %d / %d )"), RWCHAR(item_info->theName), 
									PcInven->QuestCount[num]->theCount, 
									PcInven->QuestCount[num]->theTotalCount );
							}							
							
						}
						else if( PcInven->QuestCount[num]->theType == n_NPC )
						{
							SNPCBaseInfo* npc_Info = NULL;
							npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( PcInven->QuestCount[num]->theCode ); 
							
							if( npc_Info )
							{
								Rsprintf( RWCHAR(temp), _RT("%s ( %d / %d )"), RWCHAR(npc_Info->theName), 
									PcInven->QuestCount[num]->theCount, 
									PcInven->QuestCount[num]->theTotalCount );
							}
						}				
					
						QField[ aQFieldIdx ].m_QuestCountList[i].Index = i;
						QField[ aQFieldIdx ].m_QuestCountList[i].QFieldIdx = aQFieldIdx;
						Rsprintf( RWCHAR(QField[ aQFieldIdx ].m_QuestCountList[i].TextStr), _RT("%s"), temp );
					
						break;
					}	
				}
			}
		}

		if( IsVisible )
		{
			if(SelectedBtn != -1 )
			{
				if( aQFieldIdx == SelectedBtn )
				{
					SetQuestCountListScroll(QuestCntStartListNum);
				}				
			}
		}
		/*
		for(int i = 0 ; i < MAX_QUEST_CONDITION; ++i )
		{
			
			if( QField[ aQFieldIdx ].m_QuestCountList[i].TextStr != NULL )
			{
				QField[ aQFieldIdx ].CompleteItem[ i ]->SetText( QField[ aQFieldIdx ].m_QuestCountList[i].TextStr );
			}
		}
		*/
	}	
	
	return;

}


void CQuestWnd::SetCompleteItemText( int aQFieldIdx, SQuestInfo* aQuestInfo )
{
	int i = 0;
	int num = 0; 
	char temp[64] = {0,};
	
	//////////dongs 추가 일반 아이템을  체크 하자 .. ///////////////////////////
	// 한개만 체크가 아니라 아이템 인벤(60) 과 장비 창에 있는데 10 아이템 모두 체크 한다 .. 
	// 이벤 60 체크
	
	if( aQuestInfo )
	{
		for( i = 0; i < RP_MAX_QUEST_CONDITION; ++i )
		{
			QField[ aQFieldIdx ].CompleteItem[ i ]->SetText( _RT("") );		
			
			for( num = 0; num < RP_MAX_QUEST_COUNT; ++num )
			{
				if( PcInven->QuestCount[num] )
				{
					if( ( PcInven->QuestCount[num]->theQuestCode == aQuestInfo->theCode ) &&		
						( PcInven->QuestCount[num]->theCode == aQuestInfo->theCountList[i].theCode ) &&
						( PcInven->QuestCount[num]->theType == aQuestInfo->theCountList[i].theType ) )
					{
						
						//일반 장비면 .. 카운트 리셋 시키고 재계산 한다 .. 퀘템은 알아서 정보를 받아오니깐 ... 음.. 나중에 다처리 .. 
						if( PcInven->QuestCount[num]->theType == n_ITEM )
						{
							PcInven->QuestCount[num]->theCount = 0;

							SItemBaseInfo* item_info = NULL;		
							item_info = g_Pc_Manager.GetItemBaseInfoPtr( PcInven->QuestCount[num]->theCode );
							
							if( item_info == NULL )
							{
								break;
							}
							
							//인벤 검사 있냐 없냐 ? 
							for(int k =0 ; k < c_MaxInvenItemNum; ++k)
							{
								if(PcInven->InvenItem[k] == NULL ) continue;
								
								if(PcInven->InvenItem[k]->Code == aQuestInfo->theCountList[i].theCode )
								{
									PcInven->QuestCount[num]->theCount +=  PcInven->InvenItem[k]->Amount;       
								}								
							}
							
							//장비 인벤 검사 
							for(int j=0; j < RP_MAX_EQUIP; ++j )
							{
								if(PcInven->WearItem[j]==NULL) continue;								
								
								if(PcInven->WearItem[j]->Code == aQuestInfo->theCountList[i].theCode)
								{
									PcInven->QuestCount[num]->theCount +=  PcInven->WearItem[j]->Amount;    
								}
							}
							
							//퀘스트인벤 검사 
							for(int qcnt=0; qcnt < RP_MAX_QUEST_ITEM; ++qcnt )
							{
								if(PcInven->InvenQuest[qcnt]==NULL) continue;								
								
								if(PcInven->InvenQuest[qcnt]->Code == aQuestInfo->theCountList[i].theCode)
								{
									PcInven->QuestCount[num]->theCount +=  PcInven->InvenQuest[qcnt]->Amount;    
								}
							}														
							
							if( item_info )
							{
								Rsprintf( RWCHAR(temp), _RT("%s ( %d / %d )"), RWCHAR(item_info->theName), 
									PcInven->QuestCount[num]->theCount, 
									PcInven->QuestCount[num]->theTotalCount );
							}							
							
						}
						else if( PcInven->QuestCount[num]->theType == n_NPC )
						{
							SNPCBaseInfo* npc_Info = NULL;
							npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( PcInven->QuestCount[num]->theCode ); 
							
							if( npc_Info )
							{
								Rsprintf( RWCHAR(temp), _RT("%s ( %d / %d )"), RWCHAR(npc_Info->theName), 
									PcInven->QuestCount[num]->theCount, 
									PcInven->QuestCount[num]->theTotalCount );
							}
						}				
						
						QField[ aQFieldIdx ].CompleteItem[ i ]->SetText( temp );		
						
						break;
					}	
				}
			}
		}
	}	
	
	return;
}

void CQuestWnd::SetSQuestCount( SQuestCount* aQuestCount )
{
	if(aQuestCount == NULL)
	{
		return;
	}
				
	SItemBaseInfo* item_info = NULL;		
	item_info = g_Pc_Manager.GetItemBaseInfoPtr( aQuestCount->theCode );
							
	if( item_info == NULL )
	{
		return;
	}

	aQuestCount->theCount = 0;
							
	//인벤 검사 있냐 없냐 ? 
	for(int k =0 ; k < c_MaxInvenItemNum; ++k)
	{
		if(PcInven->InvenItem[k] == NULL ) continue;
		
		if(PcInven->InvenItem[k]->Code == aQuestCount->theCode )
		{
			aQuestCount->theCount +=  PcInven->InvenItem[k]->Amount;       
		}								
	}
							
	//장비 인벤 검사 
	for(int j=0; j < RP_MAX_EQUIP; ++j )
	{
		if(PcInven->WearItem[j]==NULL) continue;								
		
		if(PcInven->WearItem[j]->Code == aQuestCount->theCode  )
		{
			aQuestCount->theCount +=  PcInven->WearItem[j]->Amount;    
		}
	}
							
	//퀘스트인벤 검사 
	for(int qcnt=0; qcnt < RP_MAX_QUEST_ITEM; ++qcnt )
	{
		if(PcInven->InvenQuest[qcnt]==NULL) continue;								
		
		if(PcInven->InvenQuest[qcnt]->Code == aQuestCount->theCode )
		{
			aQuestCount->theCount +=  PcInven->InvenQuest[qcnt]->Amount;    
		}
	}
}

void CQuestWnd::SetCompleteItemText( SRpScQuestCount* aQuestCount )
{
	// 퀘스트 카운트 정보 갱신 
	for( int num = 0; num < RP_MAX_QUEST_COUNT; ++num )
	{
		if( PcInven->QuestCount[num] )
		{
			if( ( PcInven->QuestCount[num]->theCode == aQuestCount->theCode ) && 
				( PcInven->QuestCount[num]->theType == aQuestCount->theType ) &&  
				( PcInven->QuestCount[num]->theQuestCode == aQuestCount->theQuestCode ) )
			{
				PcInven->QuestCount[num]->theCount = aQuestCount->theCount;
				break;
			}	
		}
	}
	
	
	if( aQuestCount->theType == n_ITEM )
	{
		SItemBaseInfo* item_info = NULL;		
		item_info = g_Pc_Manager.GetItemBaseInfoPtr( aQuestCount->theCode );
		
		for( int i = 0; i < QUEST_FIELD_NUM ; ++i )
		{
			if( QField[i].QuestFlag && QField[i].QuestFlag->theQuestCode )
			{
				SQuestInfo* tmp_quest_info = NULL;
				tmp_quest_info = g_Pc_Manager.GetQuestFlagInfo( QField[i].QuestFlag->theQuestCode, QField[i].QuestFlag->theFlag );
				
				if( CheckQuestComplete( i, tmp_quest_info ) )
				{
					QField[i].QuestName->SetTextColor( 50, 250, 50 );
					///-- By simwoosung - 퀘스트가 완료된 - 퀘스트 조건을 이루어냈을때..
					///-- 퀘스트 단축창이 깜빡거린다는게... 왜 이렇게 만들었는지 알수 없는듯..;;;
					///SetInsertHighlight( true );
				}
#ifdef KULUBEACH_QUEST_NAME //쿨루비치 이벤트 퀘스트 네이밍 변경 
				else if ( QField[i].QuestFlag->theQuestCode == 8314 || QField[i].QuestFlag->theQuestCode == 8315 )
				{
				QField[i].QuestName->SetTextColor( 255, 0, 0 );
				}
#endif //#ifdef KULUBEACH_QUEST_NAME 
				else
				{
					QField[i].QuestName->SetTextColor( 255, 255, 255 );
				}
				
				char tmp_info[255] = {0,};
				
				if( tmp_quest_info )
				{
					for( int j = 0; j < RP_MAX_QUEST_CONDITION; ++j )
					{
							if( ( tmp_quest_info->theCountList[j].theCode == aQuestCount->theCode ) &&
							( tmp_quest_info->theCountList[j].theType == aQuestCount->theType ) && 
							( tmp_quest_info->theCode == aQuestCount->theQuestCode ) )
						{
							Rsprintf( RWCHAR(tmp_info), _RT("%s ( %d / %d )"), RWCHAR(item_info->theName), 
								aQuestCount->theCount, aQuestCount->theTotalCount );
							
							QField[i].CompleteItem[j]->SetText( tmp_info );
							
							return;
						}
					}
				}								
			}
		}
	}
	else if( aQuestCount->theType == n_NPC )
	{
		SNPCBaseInfo* npc_Info = NULL;
		npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( aQuestCount->theCode ); 
		
		for( int i = 0; i < QUEST_FIELD_NUM ; ++i )
		{
			if( QField[i].QuestFlag && QField[i].QuestFlag->theQuestCode )
			{				
				SQuestInfo* tmp_quest_info = NULL;
				tmp_quest_info = g_Pc_Manager.GetQuestFlagInfo( QField[i].QuestFlag->theQuestCode, QField[i].QuestFlag->theFlag );
				
				char tmp_info[255] = {0,};
				
				if( tmp_quest_info )
				{
					for( int j = 0; j < RP_MAX_QUEST_CONDITION; ++j )
					{
						if( ( tmp_quest_info->theCountList[j].theCode == aQuestCount->theCode ) &&
							( tmp_quest_info->theCountList[j].theType == aQuestCount->theType ) && 
							( tmp_quest_info->theCode == aQuestCount->theQuestCode ) ) 
						{
							Rsprintf( RWCHAR(tmp_info), _RT("%s ( %d / %d )"), RWCHAR(npc_Info->theName), aQuestCount->theCount, aQuestCount->theTotalCount );
							
							QField[i].CompleteItem[j]->SetText( tmp_info );
							
							return;
						}
					}
				}								
			}
		}
		
	}
	
	return;
}



void CQuestWnd::SetCompleteItemTextNew( SRpScQuestCount* aQuestCount )
{
#ifdef QUEST_COUTLIST_RENEWAL
	
	int ScrollCount = 0 ; 

	// 퀘스트 카운트 정보 갱신 
	for( int num = 0; num < RP_MAX_QUEST_COUNT; ++num )
	{
		if( PcInven->QuestCount[num] )
		{
			if( ( PcInven->QuestCount[num]->theCode == aQuestCount->theCode ) && 
				( PcInven->QuestCount[num]->theType == aQuestCount->theType ) &&  
				( PcInven->QuestCount[num]->theQuestCode == aQuestCount->theQuestCode ) )
			{
				PcInven->QuestCount[num]->theCount = aQuestCount->theCount;
				break;
			}	
		}
	}
	
	
	if( aQuestCount->theType == n_ITEM )
	{
		SItemBaseInfo* item_info = NULL;		
		item_info = g_Pc_Manager.GetItemBaseInfoPtr( aQuestCount->theCode );
		
		for( int i = 0; i < QUEST_FIELD_NUM ; ++i )
		{
			if( QField[i].QuestFlag && QField[i].QuestFlag->theQuestCode )
			{
				SQuestInfo* tmp_quest_info = NULL;
				tmp_quest_info = g_Pc_Manager.GetQuestFlagInfo( QField[i].QuestFlag->theQuestCode, QField[i].QuestFlag->theFlag );
				
				if( CheckQuestComplete( i, tmp_quest_info ) )
				{
					QField[i].QuestName->SetTextColor( 50, 250, 50 );
					///-- By simwoosung - 퀘스트가 완료된 - 퀘스트 조건을 이루어냈을때..
					///-- 퀘스트 단축창이 깜빡거린다는게... 왜 이렇게 만들었는지 알수 없는듯..;;;
					///SetInsertHighlight( true );
				}
				else
				{
					QField[i].QuestName->SetTextColor( 255, 255, 255 );
				}
				
				char tmp_info[255] = {0,};
				
				if( tmp_quest_info )
				{
					for( int j = 0; j < MAX_QUEST_COUNT_LIST; ++j )
					{
							if( ( tmp_quest_info->theCountList[j].theCode == aQuestCount->theCode ) &&
							( tmp_quest_info->theCountList[j].theType == aQuestCount->theType ) && 
							( tmp_quest_info->theCode == aQuestCount->theQuestCode ) )
						{
							Rsprintf( RWCHAR(tmp_info), _RT("%s ( %d / %d )"), RWCHAR(item_info->theName), 
								aQuestCount->theCount, aQuestCount->theTotalCount );
													
							Rsprintf( RWCHAR(QField[ i ].m_QuestCountList[j].TextStr), _RT("%s"), tmp_info );
							
							if( IsVisible ) 
							{
								if( i == SelectedBtn )
								{
									SetQuestCountListScroll(QuestCntStartListNum);
								}
							}
							
						//	QField[i].CompleteItem[j]->SetText( tmp_info );
						
							return;
						}
					}
				}								
			}
		}
	}
	else if( aQuestCount->theType == n_NPC )
	{
		SNPCBaseInfo* npc_Info = NULL;
		npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( aQuestCount->theCode ); 
		
		for( int i = 0; i < QUEST_FIELD_NUM ; ++i )
		{
			if( QField[i].QuestFlag && QField[i].QuestFlag->theQuestCode )
			{				
				SQuestInfo* tmp_quest_info = NULL;
				tmp_quest_info = g_Pc_Manager.GetQuestFlagInfo( QField[i].QuestFlag->theQuestCode, QField[i].QuestFlag->theFlag );
				
				char tmp_info[255] = {0,};
				
				if( tmp_quest_info )
				{
					for( int j = 0; j < MAX_QUEST_COUNT_LIST; ++j )
					{
						if( ( tmp_quest_info->theCountList[j].theCode == aQuestCount->theCode ) &&
							( tmp_quest_info->theCountList[j].theType == aQuestCount->theType ) && 
							( tmp_quest_info->theCode == aQuestCount->theQuestCode ) ) 
						{
							Rsprintf( RWCHAR(tmp_info), _RT("%s ( %d / %d )"), RWCHAR(npc_Info->theName), aQuestCount->theCount, aQuestCount->theTotalCount );
							QField[i].m_QuestCountList[j].Index = j;
							QField[i].m_QuestCountList[j].QFieldIdx = i;
							Rsprintf( RWCHAR(QField[i].m_QuestCountList[j].TextStr), _RT("%s"), tmp_info );
							
							if( IsVisible ) 
							{
								if( i == SelectedBtn )
								{
									SetQuestCountListScroll(QuestCntStartListNum);
								}
							}
							/*
							if( ScrollCount < 4 )
							{
								QField[ i ].CompleteItem[ ScrollCount ]->SetText( m_QuestCountList[j].TextStr );		
								ScrollCount++;
							}
							*/
						//	QField[i].CompleteItem[j]->SetText( tmp_info );
							
							return;
						}
					}
				}								
			}
		}
		
	}

#endif 
	
	return;
}

//-----------------------------------------------------------------------------
void CQuestWnd::SetQuestWindow( WORD _quest_code, BYTE _status )
{
	//퀘스트 창 띄우기 ( n_CLOSE, n_OPEN )
	if( _status == n_OPEN )
	{
		nRui->SendEvent( WID_Quest_Wnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
	}
	else
	{
		nRui->SendEvent( WID_Quest_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	}
	
	//코드에 해당되는 퀘스트 선택처리
	SetQuestSelect( _quest_code );
	QuestBntSelected( 0 );
}


//-----------------------------------------------------------------------------
// 퀘스트 처음 받으면 code, flag 0, display 0 이 처음 날아옴. => new QuestFlag 
// 그 다음 퀘스트가 시작 되었으면, 바로 display 1 (n_OPEN) => new QuestCount 
// 퀘스트 수행 완료시 display 0 (n_CLOSE) 퀘스트창에 보여주지 않는다. 
// 게임 종료시 delete QuestFlag, QuestCount 
//-----------------------------------------------------------------------------
void CQuestWnd::SetQuestStatus( WORD _quest_code, WORD _flag, BYTE _display , DWORD _Time )
{
	
	int i = 0;
	int num = 0; 
	int cnt = 0;

	SPreQuestInfo	*pQuestInfo = GetQuest( _quest_code );
	
	if( pQuestInfo == NULL )
		return;
	
	//char str[40] = "";
	//코드에 해당되는 퀘스트 찾기
	for( i = 0; i < RP_MAX_QUEST_FLAG; ++i )
	{
		///-- 퀘스트 리스트를 이미 가지고 있다면...
		if( ( PcInven->QuestFlag[i] != NULL ) && ( PcInven->QuestFlag[i]->theQuestCode == _quest_code ) )
		{			
			if( _display == n_OPEN )
			{
				SQuestInfo* tmp_quest_info = NULL;
				tmp_quest_info = g_Pc_Manager.GetQuestFlagInfo( _quest_code, _flag );
				
				if( tmp_quest_info )
				{
					
					SystemPrint(n_DarkGreen ,n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_GET_QUEST) , tmp_quest_info->theName );	
#ifdef QUEST_COUTLIST_RENEWAL
					for( num = 0; num < 8; ++num )
#else	
					for( num = 0; num < RP_MAX_QUEST_CONDITION; ++num )
#endif 
					{
						if( tmp_quest_info->theCountList[ num ].theCode != 0 )
						{
							for( cnt = 0; cnt < RP_MAX_QUEST_COUNT; ++cnt )
							{
								if( PcInven->QuestCount[cnt] == NULL )		
								{
									PcInven->QuestCount[cnt] = SAFE_NEW( SQuestCount );									
									PcInven->QuestCount[cnt]->theQuestCode = _quest_code;
									PcInven->QuestCount[cnt]->theType = tmp_quest_info->theCountList[ num ].theType;
									PcInven->QuestCount[cnt]->theCode = tmp_quest_info->theCountList[ num ].theCode;
									PcInven->QuestCount[cnt]->theCount = 0;		
									PcInven->QuestCount[cnt]->theTotalCount = tmp_quest_info->theCountList[ num ].theCount;							
									break;
								}
								else
								{
									if( ( PcInven->QuestCount[cnt]->theQuestCode == _quest_code ) &&
										( PcInven->QuestCount[cnt]->theType == tmp_quest_info->theCountList[ num ].theType ) &&
										( PcInven->QuestCount[cnt]->theCode == tmp_quest_info->theCountList[ num ].theCode ) && 
										( PcInven->QuestCount[cnt]->theTotalCount == tmp_quest_info->theCountList[ num ].theCount ) )
									{
										PcInven->QuestCount[cnt]->theCount = 0;
										break;
									}
								}
							}				
						}
					}					
			
					if(_Time != 0 )
					{
						for( cnt = 0; cnt < RP_MAX_QUEST_COUNT; ++cnt )
						{
							if(PcInven->QuestTime[cnt] == NULL)
							{
								PcInven->QuestTime[cnt] = SAFE_NEW( SQuestTime );								
								PcInven->QuestTime[cnt]->wdQuestCode = _quest_code;
								PcInven->QuestTime[cnt]->iCountTime = _Time;
								PcInven->QuestTime[cnt]->dwStartTime = 	g_nowTime;
								break; 
								
							}
							else if( (PcInven->QuestTime[cnt] != NULL)  && 
								PcInven->QuestTime[cnt]->wdQuestCode == _quest_code )
							{
								PcInven->QuestTime[cnt]->iCountTime = _Time;
								PcInven->QuestTime[cnt]->dwStartTime = 	g_nowTime;
								break;
							}
						}			
					}			
				}
				
			}			
			
			//리스트에 퀘스트가 있으면....내용 갱신
			PcInven->QuestFlag[i]->theFlag		= _flag;
			PcInven->QuestFlag[i]->theDisplay	= _display;
			
			//퀘스트 정보 갱신
			SetQuestInfo();
			ListScroll->SetValue( 0 );
			SetQuestListScroll( 0 );
			
			///-- 시작 NPC와 보고 NPC가 같다면.....
			if( pQuestInfo->theNPCCode == pQuestInfo->theAfterNPCCode )
			{
				if( _flag == 60 )
				{
					int idx = DeleteQuestCode(_quest_code,pQuestInfo->theNPCCode);
					
					if( idx != -1 )
					{
						g_Particle.DelEffSetCon( EF_QUEST_MARK  , 0  , g_Map.m_Npc[idx].GetUnique() , g_Map.m_Npc[idx].GetIndex() );
						g_Particle.DelEffSetCon( EF_QUEST_MARK2 , 0  , g_Map.m_Npc[idx].GetUnique() , g_Map.m_Npc[idx].GetIndex() );
						
						g_Map.m_Npc[idx].m_ISQuestEffectCreated = FALSE;
						g_Map.m_Npc[idx].m_QuestMarkType = NONE_QUEST_MARK;
					}
					
					//by simwoosung
					SMinimapNPCInfo * pNPCInfo = MiniMapWnd.DeleteQuestCode( _quest_code,pQuestInfo->theNPCCode );
					if(pNPCInfo)
					{
						pNPCInfo->nQuestMarkType = NONE_QUEST_MARK;
					}
				}
				else
					if( _flag == 1 )
					{
						int findquest = _quest_code;
						
						int idx = FindQuestNpc(findquest,pQuestInfo->theNPCCode);
						
						if( idx != -1 )
						{
							if( findquest == -1 )
							{
								QUESTDATA	QuestData;
								
								QuestData.Code = _quest_code;
								QuestData.Type = REPORT_QUEST_MARK;
								
								g_Map.m_Npc[idx].thePreQuestCodeList.push_back( QuestData );
							}
							
							g_Particle.DelEffSetCon( EF_QUEST_MARK  , 0  , g_Map.m_Npc[idx].GetUnique() , g_Map.m_Npc[idx].GetIndex() );
							g_Particle.DelEffSetCon( EF_QUEST_MARK2 , 0  , g_Map.m_Npc[idx].GetUnique() , g_Map.m_Npc[idx].GetIndex() );
							
							g_Map.m_Npc[idx].m_ISQuestEffectCreated = FALSE;
							g_Map.m_Npc[idx].m_QuestMarkType = REPORT_QUEST_MARK;
						}
						
						//by simwoosung
						SMinimapNPCInfo * pNPCInfo = MiniMapWnd.FindQuestNPC(_quest_code, pQuestInfo->theNPCCode);
						if(pNPCInfo)
						{
							pNPCInfo->nQuestMarkType = REPORT_QUEST_MARK;
						}
					}
			}
			else ///-- 시작 NPC와 보고 NPC가 틀리다면....
			{
				if( _flag == 60 )				
				{
					int idx = DeleteQuestCode(_quest_code,pQuestInfo->theNPCCode);
					
					if( idx != -1 )
					{
						g_Particle.DelEffSetCon( EF_QUEST_MARK  , 0  , g_Map.m_Npc[idx].GetUnique() , g_Map.m_Npc[idx].GetIndex() );
						g_Particle.DelEffSetCon( EF_QUEST_MARK2 , 0  , g_Map.m_Npc[idx].GetUnique() , g_Map.m_Npc[idx].GetIndex() );
						
						g_Map.m_Npc[idx].m_ISQuestEffectCreated = FALSE;
						g_Map.m_Npc[idx].m_QuestMarkType = NONE_QUEST_MARK;
					}

					//by simwoosung
					SMinimapNPCInfo * pNPCInfo = MiniMapWnd.DeleteQuestCode( _quest_code, pQuestInfo->theNPCCode );
					if(pNPCInfo)
					{
						pNPCInfo->nQuestMarkType = NONE_QUEST_MARK;
					}
					
					idx = DeleteQuestCode(_quest_code,pQuestInfo->theAfterNPCCode );
					
					if( idx != -1 )
					{
						g_Particle.DelEffSetCon( EF_QUEST_MARK  , 0  , g_Map.m_Npc[idx].GetUnique() , g_Map.m_Npc[idx].GetIndex() );
						g_Particle.DelEffSetCon( EF_QUEST_MARK2 , 0  , g_Map.m_Npc[idx].GetUnique() , g_Map.m_Npc[idx].GetIndex() );
						
						g_Map.m_Npc[idx].m_ISQuestEffectCreated = FALSE;
						g_Map.m_Npc[idx].m_QuestMarkType = NONE_QUEST_MARK;
					}
					//by simwoosung
					pNPCInfo = MiniMapWnd.DeleteQuestCode( _quest_code, pQuestInfo->theAfterNPCCode );
					if(pNPCInfo)
					{
						pNPCInfo->nQuestMarkType = NONE_QUEST_MARK;
					}
				}
				else if( _flag == 1 )
				{
					int idx = DeleteQuestCode(_quest_code,pQuestInfo->theNPCCode);
					
					if( idx != -1 )
					{
						g_Particle.DelEffSetCon( EF_QUEST_MARK  , 0  , g_Map.m_Npc[idx].GetUnique() , g_Map.m_Npc[idx].GetIndex() );
						g_Particle.DelEffSetCon( EF_QUEST_MARK2 , 0  , g_Map.m_Npc[idx].GetUnique() , g_Map.m_Npc[idx].GetIndex() );
						
						g_Map.m_Npc[idx].m_ISQuestEffectCreated = FALSE;
						g_Map.m_Npc[idx].m_QuestMarkType = NONE_QUEST_MARK;
					}
					//by simwoosung
					SMinimapNPCInfo * pNPCInfo = MiniMapWnd.DeleteQuestCode( _quest_code, pQuestInfo->theNPCCode );
					if(pNPCInfo)
					{
						pNPCInfo->nQuestMarkType = NONE_QUEST_MARK;
					}
					
					int findquest = _quest_code;
					
					idx = FindQuestNpc(findquest,pQuestInfo->theAfterNPCCode );
					
					if( idx != -1 )
					{
						if( findquest == -1 )
						{
							QUESTDATA	QuestData;
							QuestData.Code = _quest_code;
							QuestData.Type = REPORT_QUEST_MARK;
							
							g_Map.m_Npc[idx].thePreQuestCodeList.push_back( QuestData );
						}
						
						g_Particle.DelEffSetCon( EF_QUEST_MARK  , 0  , g_Map.m_Npc[idx].GetUnique() , g_Map.m_Npc[idx].GetIndex() );
						g_Particle.DelEffSetCon( EF_QUEST_MARK2 , 0  , g_Map.m_Npc[idx].GetUnique() , g_Map.m_Npc[idx].GetIndex() );
						
						g_Map.m_Npc[idx].m_ISQuestEffectCreated = FALSE;
						g_Map.m_Npc[idx].m_QuestMarkType = REPORT_QUEST_MARK;
					}
					//by simwoosung
					pNPCInfo = MiniMapWnd.FindQuestNPC(_quest_code, pQuestInfo->theAfterNPCCode);
					if(pNPCInfo)
					{
						pNPCInfo->nQuestMarkType = REPORT_QUEST_MARK;
					}
				}
			}
			
			///-- 퀘스트 리스트에 존재한다면 리턴...
			return;
		}
	}	
	
	//리스트에 없으면 리스트에 추가
	for( i = 0; i < RP_MAX_QUEST_FLAG; ++i )
	{
		if( PcInven->QuestFlag[i] == NULL )
		{
			SQuestFlag* QuestFlag	= SAFE_NEW( SQuestFlag );			
			QuestFlag->theQuestCode	= _quest_code;
			QuestFlag->theFlag		= _flag;
			QuestFlag->theDisplay	= _display;
			
			PcInven->QuestFlag[i] = QuestFlag;
			
			if( _display == n_OPEN )
			{
				SQuestInfo* tmp_quest_info = NULL;
				tmp_quest_info = g_Pc_Manager.GetQuestFlagInfo( _quest_code, _flag );
				
				if( tmp_quest_info )
				{
					SystemPrint(n_DarkGreen ,n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_GET_QUEST) , tmp_quest_info->theName );	

					for( num = 0; num < RP_MAX_QUEST_CONDITION; ++num )
					{
						if( tmp_quest_info->theCountList[ num ].theCode != 0 )
						{
							for( cnt = 0; cnt < RP_MAX_QUEST_COUNT; ++cnt )
							{
								if( PcInven->QuestCount[cnt] == NULL )		
								{
									PcInven->QuestCount[cnt] = SAFE_NEW( SQuestCount );									
									PcInven->QuestCount[cnt]->theQuestCode = _quest_code;
									PcInven->QuestCount[cnt]->theType = tmp_quest_info->theCountList[ num ].theType;
									PcInven->QuestCount[cnt]->theCode = tmp_quest_info->theCountList[ num ].theCode;
									PcInven->QuestCount[cnt]->theCount = 0;		
									PcInven->QuestCount[cnt]->theTotalCount = tmp_quest_info->theCountList[ num ].theCount;							
									break;
								}
								else
								{
									if( ( PcInven->QuestCount[cnt]->theQuestCode == _quest_code ) &&
										( PcInven->QuestCount[cnt]->theType == tmp_quest_info->theCountList[ num ].theType ) &&
										( PcInven->QuestCount[cnt]->theCode == tmp_quest_info->theCountList[ num ].theCode ) && 
										( PcInven->QuestCount[cnt]->theTotalCount == tmp_quest_info->theCountList[ num ].theCount ) )
									{
										PcInven->QuestCount[cnt]->theCount = 0;
										break;
									}
								}
							}				
						}
					}				
					

					// by dongs QuestCount 시간입력하기 .. 
					//남는 퀘스트 카운트에 값을 입력한다.... 
					//로그인할시 퀘스트 카운트 type 값으로 11값일때 시간으로 나타내어준다 . 
					//time 값이 있으면 퀘스트 카운트에 추가한다 .. 
					// 퀘스트 번호와 타임값 .. ㅋㅋ 
					// 찾을때는 type == 11 인것은 시간을 뿌려준다 .. 
					// 최대한 있는거 이용해야지 .. 
					// 디파인 값이 없다 .. OTL .. 
					
					if(_Time != 0 )
					{
						for( cnt = 0; cnt < RP_MAX_QUEST_COUNT; ++cnt )
						{
							if( PcInven->QuestTime[cnt] == NULL)
							{
								PcInven->QuestTime[cnt] = SAFE_NEW( SQuestTime );							
								
								PcInven->QuestTime[cnt]->wdQuestCode = _quest_code; 
								PcInven->QuestTime[cnt]->iCountTime = _Time;
								PcInven->QuestTime[cnt]->dwStartTime = 	g_nowTime;
								
								break;
							}
							else
							{
								if( PcInven->QuestTime[cnt]->wdQuestCode == _quest_code )
								{									
									PcInven->QuestTime[cnt]->iCountTime = _Time;
									PcInven->QuestTime[cnt]->dwStartTime = 	g_nowTime;
								}
							}
						}			
					}		
				}
			}	
			
			//sprintf( str, "ADD  code:%d,  flag:%d,  display:%d\n", _quest_code, _flag, _display );
			//OutputDebugString( str );
			if( pQuestInfo->theNPCCode == pQuestInfo->theAfterNPCCode )
			{
				if( _flag == 60 )
				{
					int idx = DeleteQuestCode(_quest_code,pQuestInfo->theNPCCode);
					
					if( idx != -1 )
					{
						g_Particle.DelEffSetCon( EF_QUEST_MARK  , 0  , g_Map.m_Npc[idx].GetUnique() , g_Map.m_Npc[idx].GetIndex() );
						g_Particle.DelEffSetCon( EF_QUEST_MARK2 , 0  , g_Map.m_Npc[idx].GetUnique() , g_Map.m_Npc[idx].GetIndex() );
						
						g_Map.m_Npc[idx].m_ISQuestEffectCreated = FALSE;
						g_Map.m_Npc[idx].m_QuestMarkType = NONE_QUEST_MARK;
					}
					//by simwoosung
					SMinimapNPCInfo * pNPCInfo = MiniMapWnd.DeleteQuestCode( _quest_code, pQuestInfo->theNPCCode );
					if(pNPCInfo)
					{
						pNPCInfo->nQuestMarkType = NONE_QUEST_MARK;
					}
				}
				else
					if( _flag == 1 )
					{
						int findquest = _quest_code;
						
						int idx = FindQuestNpc(findquest,pQuestInfo->theNPCCode);
						
						if( idx != -1 )
						{
							if( findquest == -1 )
							{
								QUESTDATA	QuestData;
								
								QuestData.Code = _quest_code;
								QuestData.Type = REPORT_QUEST_MARK;
								
								g_Map.m_Npc[idx].thePreQuestCodeList.push_back( QuestData );
							}
							
							g_Particle.DelEffSetCon( EF_QUEST_MARK  , 0  , g_Map.m_Npc[idx].GetUnique() , g_Map.m_Npc[idx].GetIndex() );
							g_Particle.DelEffSetCon( EF_QUEST_MARK2 , 0  , g_Map.m_Npc[idx].GetUnique() , g_Map.m_Npc[idx].GetIndex() );
							
							g_Map.m_Npc[idx].m_ISQuestEffectCreated = FALSE;
							g_Map.m_Npc[idx].m_QuestMarkType = REPORT_QUEST_MARK;
						}
						//by simwoosung
						SMinimapNPCInfo * pNPCInfo = MiniMapWnd.FindQuestNPC(_quest_code, pQuestInfo->theNPCCode);
						if(pNPCInfo)
						{
							pNPCInfo->nQuestMarkType = REPORT_QUEST_MARK;
						}
					}
			}
			else
			{
				if( _flag == 60 )				
				{
					int idx = DeleteQuestCode(_quest_code,pQuestInfo->theNPCCode);
					
					if( idx != -1 )
					{
						g_Particle.DelEffSetCon( EF_QUEST_MARK  , 0  , g_Map.m_Npc[idx].GetUnique() , g_Map.m_Npc[idx].GetIndex() );
						g_Particle.DelEffSetCon( EF_QUEST_MARK2 , 0  , g_Map.m_Npc[idx].GetUnique() , g_Map.m_Npc[idx].GetIndex() );
						
						g_Map.m_Npc[idx].m_ISQuestEffectCreated = FALSE;
						g_Map.m_Npc[idx].m_QuestMarkType = NONE_QUEST_MARK;
					}
					//by simwoosung
					SMinimapNPCInfo * pNPCInfo = MiniMapWnd.DeleteQuestCode( _quest_code, pQuestInfo->theNPCCode );
					if(pNPCInfo)
					{
						pNPCInfo->nQuestMarkType = NONE_QUEST_MARK;
					}
					
					idx = DeleteQuestCode(_quest_code,pQuestInfo->theAfterNPCCode );
					
					if( idx != -1 )
					{
						g_Particle.DelEffSetCon( EF_QUEST_MARK  , 0  , g_Map.m_Npc[idx].GetUnique() , g_Map.m_Npc[idx].GetIndex() );
						g_Particle.DelEffSetCon( EF_QUEST_MARK2 , 0  , g_Map.m_Npc[idx].GetUnique() , g_Map.m_Npc[idx].GetIndex() );
						
						g_Map.m_Npc[idx].m_ISQuestEffectCreated = FALSE;
						g_Map.m_Npc[idx].m_QuestMarkType = NONE_QUEST_MARK;
					}
					//by simwoosung
					pNPCInfo = MiniMapWnd.DeleteQuestCode( _quest_code, pQuestInfo->theAfterNPCCode );
					if(pNPCInfo)
					{
						pNPCInfo->nQuestMarkType = NONE_QUEST_MARK;
					}
				}
				else
					if( _flag == 1 )
					{
						int idx = DeleteQuestCode(_quest_code,pQuestInfo->theNPCCode);
						
						if( idx != -1 )
						{
							g_Particle.DelEffSetCon( EF_QUEST_MARK  , 0  , g_Map.m_Npc[idx].GetUnique() , g_Map.m_Npc[idx].GetIndex() );
							g_Particle.DelEffSetCon( EF_QUEST_MARK2 , 0  , g_Map.m_Npc[idx].GetUnique() , g_Map.m_Npc[idx].GetIndex() );
							
							g_Map.m_Npc[idx].m_ISQuestEffectCreated = FALSE;
							g_Map.m_Npc[idx].m_QuestMarkType = NONE_QUEST_MARK;
						}
						//by simwoosung
						SMinimapNPCInfo * pNPCInfo = MiniMapWnd.DeleteQuestCode( _quest_code, pQuestInfo->theNPCCode );
						if(pNPCInfo)
						{
							pNPCInfo->nQuestMarkType = NONE_QUEST_MARK;
						}
						
						int findquest = _quest_code;
						
						idx = FindQuestNpc(findquest,pQuestInfo->theAfterNPCCode );
						
						if( idx != -1 )
						{
							if( findquest == -1 )
							{
								QUESTDATA	QuestData;
								
								QuestData.Code = _quest_code;
								QuestData.Type = REPORT_QUEST_MARK;
								
								g_Map.m_Npc[idx].thePreQuestCodeList.push_back( QuestData );
							}
							
							g_Particle.DelEffSetCon( EF_QUEST_MARK  , 0  , g_Map.m_Npc[idx].GetUnique() , g_Map.m_Npc[idx].GetIndex() );
							g_Particle.DelEffSetCon( EF_QUEST_MARK2 , 0  , g_Map.m_Npc[idx].GetUnique() , g_Map.m_Npc[idx].GetIndex() );
							
							g_Map.m_Npc[idx].m_ISQuestEffectCreated = FALSE;
							g_Map.m_Npc[idx].m_QuestMarkType = REPORT_QUEST_MARK;
						}
						//by simwoosung
						pNPCInfo = MiniMapWnd.FindQuestNPC(_quest_code, pQuestInfo->theAfterNPCCode);
						if(pNPCInfo)
						{
							pNPCInfo->nQuestMarkType = REPORT_QUEST_MARK;
						}
					}
			}
			
			break;
		}
	}
	
	//퀘스트 정보 갱신
	SetQuestInfo();
	
	ListScroll->SetValue( 0 );
	SetQuestListScroll( 0 );

	return;
}

SPreQuestInfo *CQuestWnd::GetQuest(WORD quest_code)
{
	for( int i = 0; i < c_Max_PreQuestInfo_Count ; ++i )
	{
		if( g_Pc_Manager.thePreQuestInfoTable[i].theQuestCode == 0 )
		{
			break;
		}
		
		if( g_Pc_Manager.thePreQuestInfoTable[i].theQuestCode == quest_code )
		{
			return &g_Pc_Manager.thePreQuestInfoTable[i];
		}
	}
	
	return NULL;
}

int CQuestWnd::DeleteQuestCode(WORD quest_code,int index)
{
	for( int idx = 0; idx < MAX_NPC_INTABLE; ++idx )
	{
		if( g_Map.m_NpcTable[idx].code == index )
		{
			std::vector<QUESTDATA>::iterator ir;		
			
			ir = g_Map.m_Npc[idx].thePreQuestCodeList.begin();
			for( ; ir != g_Map.m_Npc[idx].thePreQuestCodeList.end() ; ++ir )
			{
				if( ir->Code == quest_code )
				{
					g_Map.m_Npc[idx].thePreQuestCodeList.erase( ir );
					
					return idx;
				}
			}				
		}
	}
	
	return -1;
}

int CQuestWnd::FindQuestNpc(int &quest_code,int index)
{
	for( int idx = 0; idx < MAX_NPC_INTABLE; ++idx )
	{
		if( g_Map.m_NpcTable[idx].code == index )
		{
			std::vector<QUESTDATA>::iterator ir;		
			
			ir = g_Map.m_Npc[idx].thePreQuestCodeList.begin();
			for( ; ir != g_Map.m_Npc[idx].thePreQuestCodeList.end() ; ++ir )
			{
				if( ir->Code == quest_code )
				{
					return idx;
				}
			}	
			
			quest_code = -1;
			
			return idx;
		}
	}
	
	return -1;
}

//-----------------------------------------------------------------------------
void CQuestWnd::QuestCancel( WORD _quest_code )
{

}

//-----------------------------------------------------------------------------

void CQuestWnd::SetRewardStrNew( WORD aQuestCode, BYTE aQuestFlag, char* pStrReward )
{
	SQuestInfo* tmp_quest_info = NULL;
	SPreQuestInfo * pPreQInfo = NULL;  

	tmp_quest_info = g_Pc_Manager.GetQuestFlagInfo( aQuestCode, aQuestFlag );
	pPreQInfo = GetQuest( aQuestCode );

	if( tmp_quest_info )
	{
		char tmp_reward[255] = {0,};
		
		int i = 0; 
		
		//		DWORD				theRewardExp;
		//		DWORD				theRewarddEld;
		//		WORD				theRewardItemCode[3];
		//		BYTE				theRewardItemNum[3];
		
		// ex> 
		// 케루퍼스 10마리를 잡아와라. 
		// 
		// 경험치 500 획득 
		// 엘드 %d 획득 
		// 윈드소드 1개 획득 
		// 회복약 10개 획득 
		// 

		if( tmp_quest_info->theRewardExp || tmp_quest_info->theRewarddEld || tmp_quest_info->theRewardFame )
		{
		
			// "##" 샵 2개로 줄 바꿈. 2바이트 사용 경우 글자 조합시 특수문자 하나만 사용 경우 키값이 조합 키값과 겹칠수도 있기 때문에 2Byte 사용한다. 
			SAFE_STR_CPY( pStrReward, _RT("####"), 1024 );
		}
		
		if( tmp_quest_info->theRewardExp )
		{
			// 경험치 %d 획득 ##
			Rsprintf( RWCHAR(tmp_reward), G_STRING_CHAR( IDS_WND_REWARDEXP ), tmp_quest_info->theRewardExp );
			//			sprintf( tmp_reward, "경험치 %d 획득 ##", tmp_quest_info->theRewardExp );			 
			
			SAFE_STR_CAT( pStrReward, tmp_reward, 1024 );			
		}
		
		if( tmp_quest_info->theRewarddEld )
		{
			// 엘드 %d 획득 ##
			Rsprintf( RWCHAR(tmp_reward), G_STRING_CHAR( IDS_WND_REWARDELD ), tmp_quest_info->theRewarddEld );
			//			sprintf( tmp_reward, "엘드 %d 획득 ##", tmp_quest_info->theRewarddEld );			
			SAFE_STR_CAT( pStrReward, tmp_reward, 1024 );			
		}
		
		if( tmp_quest_info->theRewardFame)
		{
			// 엘드 %d 획득 ##
			Rsprintf( RWCHAR(tmp_reward), G_STRING_CHAR( IDS_WND_GET_FAME ), tmp_quest_info->theRewardFame );
			//			sprintf( tmp_reward, "엘드 %d 획득 ##", tmp_quest_info->theRewarddEld );			
			SAFE_STR_CAT( pStrReward, tmp_reward, 1024 );			
		}

			
		for( i = 0; i < 4 ; ++i )
		{
			if( tmp_quest_info->theRewardItemCode[i] )
			{
				SItemBaseInfo* item_info = NULL;		
				item_info = g_Pc_Manager.GetItemBaseInfoPtr( tmp_quest_info->theRewardItemCode[i] );
				
				if( item_info )
				{
					// [%s] %d개 획득 ##
					Rsprintf( RWCHAR(tmp_reward), G_STRING_CHAR( IDS_WND_REWARDITEM ), RWCHAR(item_info->theName), tmp_quest_info->theRewardItemNum[i] );
					//					sprintf( tmp_reward, "[%s] %d개 획득 ##", item_info->theName, tmp_quest_info->theRewardItemNum[i] );			
					SAFE_STR_CAT( pStrReward, tmp_reward, 1024 );	
				}
			}
		}

		SNPCBaseInfo* npc_Info = NULL;
		char StrTmp[256] = {0,};
		char ResultStr[256] = {0,};

		if( pPreQInfo )
		{
			
			
			if( pPreQInfo->theNPCCode )
			{
				npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( pPreQInfo->theNPCCode );
				
				if( npc_Info )
				{
					Rsprintf( RWCHAR(StrTmp), G_STRING_CHAR( IDS_START_NPCIS ), RWCHAR(npc_Info->theName) );		///-- HAN_MSG
					SAFE_STR_CAT( ResultStr, StrTmp, 256 );
				}
			}

			if( pPreQInfo->theAfterNPCCode )
			{
				npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( pPreQInfo->theAfterNPCCode );
				
				if( npc_Info )
				{
					Rsprintf( RWCHAR(StrTmp), G_STRING_CHAR( IDS_REPORT_NPCIS ), RWCHAR(npc_Info->theName) );		///-- HAN_MSG
					SAFE_STR_CAT( ResultStr, StrTmp, 256 );
				}	
			}
			StartNpc->SetText(ResultStr);
		}		
	}
	
	return;




}


void CQuestWnd::SetRewardStr( WORD aQuestCode, BYTE aQuestFlag, char* pStrReward )
{
	SQuestInfo* tmp_quest_info = NULL;
	SPreQuestInfo * pPreQInfo = NULL;  

	tmp_quest_info = g_Pc_Manager.GetQuestFlagInfo( aQuestCode, aQuestFlag );
	pPreQInfo = GetQuest( aQuestCode );

	if( tmp_quest_info )
	{
		char tmp_reward[255] = {0,};
		
		int i = 0; 
		
		//		DWORD				theRewardExp;
		//		DWORD				theRewarddEld;
		//		WORD				theRewardItemCode[3];
		//		BYTE				theRewardItemNum[3];
		
		// ex> 
		// 케루퍼스 10마리를 잡아와라. 
		// 
		// 경험치 500 획득 
		// 엘드 %d 획득 
		// 윈드소드 1개 획득 
		// 회복약 10개 획득 
		// 

		if( tmp_quest_info->theRewardExp || tmp_quest_info->theRewarddEld || tmp_quest_info->theRewardFame )
		{
		
			// "##" 샵 2개로 줄 바꿈. 2바이트 사용 경우 글자 조합시 특수문자 하나만 사용 경우 키값이 조합 키값과 겹칠수도 있기 때문에 2Byte 사용한다. 
			SAFE_STR_CPY( pStrReward, _RT("####"), 1024 );
		}
		
		if( tmp_quest_info->theRewardExp )
		{
			// 경험치 %d 획득 ##
			Rsprintf( RWCHAR(tmp_reward), G_STRING_CHAR( IDS_WND_REWARDEXP ), tmp_quest_info->theRewardExp );
			//			sprintf( tmp_reward, "경험치 %d 획득 ##", tmp_quest_info->theRewardExp );			 
			
			SAFE_STR_CAT( pStrReward, tmp_reward, 1024 );			
		}
		
		if( tmp_quest_info->theRewarddEld )
		{
			// 엘드 %d 획득 ##
			Rsprintf( RWCHAR(tmp_reward), G_STRING_CHAR( IDS_WND_REWARDELD ), tmp_quest_info->theRewarddEld );
			//			sprintf( tmp_reward, "엘드 %d 획득 ##", tmp_quest_info->theRewarddEld );			
			SAFE_STR_CAT( pStrReward, tmp_reward, 1024 );			
		}
		
		if( tmp_quest_info->theRewardFame)
		{
			// 엘드 %d 획득 ##
			Rsprintf( RWCHAR(tmp_reward), G_STRING_CHAR( IDS_WND_GET_FAME ), tmp_quest_info->theRewardFame );
			//			sprintf( tmp_reward, "엘드 %d 획득 ##", tmp_quest_info->theRewarddEld );			
			SAFE_STR_CAT( pStrReward, tmp_reward, 1024 );			
		}

		int nCount = 3;

#ifdef RENEWAL_QUEST1030
		
		nCount = 4;

#endif
		
		for( i = 0; i < nCount ; ++i )
		{
			if( tmp_quest_info->theRewardItemCode[i] )
			{
				SItemBaseInfo* item_info = NULL;		
				item_info = g_Pc_Manager.GetItemBaseInfoPtr( tmp_quest_info->theRewardItemCode[i] );
				
				if( item_info )
				{
					// [%s] %d개 획득 ##
					Rsprintf( RWCHAR(tmp_reward), G_STRING_CHAR( IDS_WND_REWARDITEM ), RWCHAR(item_info->theName), tmp_quest_info->theRewardItemNum[i] );
					//					sprintf( tmp_reward, "[%s] %d개 획득 ##", item_info->theName, tmp_quest_info->theRewardItemNum[i] );			
					SAFE_STR_CAT( pStrReward, tmp_reward, 1024 );	
				}
			}
		}

		SNPCBaseInfo* npc_Info = NULL;
		
		if( pPreQInfo )
		{
			SAFE_STR_CAT( pStrReward, _RT("##"), 1024 );			
			
			if( pPreQInfo->theNPCCode )
			{
				npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( pPreQInfo->theNPCCode );
				
				if( npc_Info )
				{
					Rsprintf( RWCHAR(tmp_reward), G_STRING_CHAR( IDS_START_NPCIS ), RWCHAR(npc_Info->theName) );		///-- HAN_MSG
					SAFE_STR_CAT( pStrReward, tmp_reward, 1024 );
				}
			}

			if( pPreQInfo->theAfterNPCCode )
			{
				npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( pPreQInfo->theAfterNPCCode );
				
				if( npc_Info )
				{
					Rsprintf( RWCHAR(tmp_reward), G_STRING_CHAR( IDS_REPORT_NPCIS ), RWCHAR(npc_Info->theName) );		///-- HAN_MSG
					SAFE_STR_CAT( pStrReward, tmp_reward, 1024 );
				}	
			}
		}		
	}
	
	return;
}


bool CQuestWnd::CheckQuestComplete( int aQFieldIdx, SQuestInfo* aQuestInfo )
{
	if( aQuestInfo )
	{
		int i = 0;
		int num = 0; 
		int check_cnt = 0;
		int complete_cnt = 0;
		
		bool IsEnd = false;

		for( i = 0; i < RP_MAX_QUEST_CONDITION; ++i )
		{
			for( num = 0; num < RP_MAX_QUEST_COUNT; ++num )
			{
				if( PcInven->QuestCount[num] )
				{
					if( ( PcInven->QuestCount[num]->theQuestCode == aQuestInfo->theCode ) &&		
						( PcInven->QuestCount[num]->theCode == aQuestInfo->theCountList[i].theCode ) &&
						( PcInven->QuestCount[num]->theType == aQuestInfo->theCountList[i].theType ) )
					{
						++check_cnt;
						
						if( PcInven->QuestCount[num]->theCount >= PcInven->QuestCount[num]->theTotalCount )
						{
							++complete_cnt;
						}						
						
						break;
					}	
				}
			}		
		}
		
		if( check_cnt == 0 )
		{
			return false;
		}
		
		if( check_cnt == complete_cnt )
		{
			return true;
		}
	}
	
	return false;
}






void CQuestWnd::CompositionOld()
{
	


	int center_x	= nRui->Desktop.Width / 2;
	int center_y	= nRui->Desktop.Height / 2;
	
	CFrameWnd*		fw		   = NULL;
	CButton*		btn		   = NULL;
	CStateButton*	sbtn       = NULL;
	SRect			questwnd;
	CImageBox9*		img9;
	CImageBox*		img;
	CTextBox*		tbox;

	questwnd.w	= 336;
	questwnd.h	= 448;
	
	questwnd.x	= center_x - ( questwnd.w / 2 );
	questwnd.y	= center_y - ( questwnd.h / 2 );
	
	///-- Frame Wnd
	fw = (CFrameWnd *)nRui->CreateWnd( n_wtFrameWnd, WID_Quest_Wnd, WID_None, questwnd.x, questwnd.y, questwnd.w, questwnd.h, false );
	fw->SetWndTexStyle( fwfTex_Style_Tile9 );			// 1, 3, 9 
	fw->SetClientImage( TID_CTRL_WND_L1 );				// 좌측 위 ( 9개 일때 ) TID 정렬 필수 
	fw->SetIconTID( TID_CTRL_ICON_QUEST );				// 창마다 아이콘 표시 (디폴트 있음)
	fw->SetWndTileSizeLR( 40, 29 );						// TID 넓이 Left, Right		// 그라데이션 사용시 기본 3픽셀 사용하면 좋음.  		
	fw->SetWndTileSizeTB( 31, 11 );						// TID 높이 Top, Bottom		// 위아래 가운데는 자동 패턴. 
	fw->SetCaption( G_STRING_CHAR( IDS_WND_QUEST ) );	// 윈도우 창 이름 
	fw->SetTitle( true );								// 표시 없으면 사용 안함. 
	
	fw->SetWndProc( this );		
	fw->SetVisible( false );
	fw->Initialize();

	// SetFontR()로 그룹화된 폰트 사용시 (버튼은 정형화 완료, 배틀존 예외)
	// SetFontg() 개별적인 폰트를 따로 사용시 
	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_Quest_Wnd, 12, 34, 70, 17, false );
	tbox->SetFontR( n_fRsvFontWndStatic );
	tbox->SetTextColor( 0, 0, 0 );
	tbox->SetAlignText( n_atLeft, n_atCenter);	 	
	tbox->SetText( G_STRING_CHAR( IDS_WND_QUEST_NAME ) );

	///-- 이 아래쪽 텍스트는 사용하지 않는듯....
	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_Quest_Wnd, 15, 134, 252, 15, false ); 
	tbox->SetFontR( n_fRsvFontWndStatic );
	tbox->SetAlignText( n_atLeft, n_atCenter);	 
	tbox->SetTextColor( 0, 0, 0 );
	tbox->SetText( G_STRING_CHAR( IDS_WND_QUEST_ORDER ));
	tbox->SetFocusCheck( false );
	tbox->SetVisible( false );	
	
	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_Quest_Wnd, 15, 164, 252, 15, false );  
	tbox->SetFontR( n_fRsvFontWndStatic );
	tbox->SetAlignText( n_atLeft, n_atCenter);	 
	tbox->SetTextColor( 0, 0, 0 );
	tbox->SetText( G_STRING_CHAR( IDS_WND_QUEST_LIMIT ) );
	tbox->SetFocusCheck( false );
	tbox->SetVisible( false );
	
	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_Quest_Wnd, 15, 149, 252, 15, false ); 
	tbox->SetFontR( n_fRsvFontWndStatic );
	tbox->SetAlignText( n_atLeft, n_atCenter);	 
	tbox->SetTextColor( 0, 0, 0 );
	tbox->SetText( G_STRING_CHAR( IDS_WND_QUEST_RELEVEL ) );
	tbox->SetFocusCheck( false );
	tbox->SetVisible( false );
	
	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_Quest_Wnd, 15, 179, 252, 15, false );  
	tbox->SetFontR( n_fRsvFontWndStatic );
	tbox->SetTextColor( 0, 0, 0 );
	tbox->SetAlignText( n_atLeft, n_atCenter);	 
	tbox->SetText( G_STRING_CHAR( IDS_WND_QUEST_CONDI ) );
	tbox->SetFocusCheck( false );
	tbox->SetVisible( false );
	
	///---------------------------------------------------------------------------	
	///-- Static
	///---------------------------------------------------------------------------		
	// 음영 또는 라인은 TID IMG 따로 생성해서 사용 
	///-- Line	
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_Quest_Wnd, 1, 168, 334, 169, false );   
	img ->SetClientImage( TID_CTRL_BACK );
	img->SetFocusCheck( false );

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_Quest_Wnd, 1, 167, 334, 1, false );   
	img ->SetClientImage( TID_CTRL_LINEDOT );

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_Quest_Wnd, 1, 336, 334, 1, false ); 
	img ->SetClientImage( TID_CTRL_LINEDOT );

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_Quest_Wnd, 1, 401, 334, 1, false ); 	
	img ->SetClientImage( TID_CTRL_LINEDOT );

	// 이미지 범용 사용 및 리소스 사이즈 절감 효과 (리스트 박스, 뒷배경으로 깔리는 여러 이미지들 )
	///-- Image9
	img9 = (CImageBox9*)nRui->CreateWnd( n_wtImageBox9, WID_None, WID_Quest_Wnd, 3, 54, 330, 111, false );       	
	img9->SetWndTileSizeLR( 9, 9 );					// 윈도우 생성시와 동일 
	img9->SetWndTileSizeTB( 8, 8 );					// 윈도우 생성시와 동일 
	img9->SetFocusCheck( false );					// 이미지 사용시 필수 
	img9->SetClientImage( TID_CTRL_CONTBACK_L1 );
	img9->SetAdjustPosition( 0, 0 );				// n_wtImageBox9 사용시 필수 호출 
	
	img9 = (CImageBox9*)nRui->CreateWnd( n_wtImageBox9, WID_None, WID_Quest_Wnd, 3, 175, 330, 159, false );       	
	img9->SetWndTileSizeLR( 9, 9 );
	img9->SetWndTileSizeTB( 8, 8 );
	img9->SetFocusCheck( false );
	img9->SetClientImage( TID_CTRL_CONTBACK_L1 );
	img9->SetAdjustPosition( 0, 0 );

	///---------------------------------------------------------------------------	
	///-- Control
	///---------------------------------------------------------------------------	
	///-- X		
	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_Quest_Close_Btn , WID_Quest_Wnd , 318, 7, 13, 13, false );
	btn->SetBtnImage( TID_CTRL_BTN_X, TID_CTRL_BTN_X_OVR, TID_CTRL_BTN_X_CLK, TID_None );

	// 퀘스트포기 버튼
	CancelBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_Quest_Cancel_Btn , WID_Quest_Wnd , 13, 410, 87, 25, false );	
	CancelBtn->SetBtnImage( TID_CTRL_BTN87, TID_CTRL_BTN87_OVR, TID_CTRL_BTN87_CLK, TID_CTRL_BTN87_DIS );	
	CancelBtn->SetFontR( n_fRsvFontWndButton ); 
	CancelBtn->SetText( G_STRING_CHAR( IDS_WND_QUEST_GIVUP ) );
	CancelBtn->SetBtnHold( true );
 
	// 스크롤바 ( Quest_List )
	ListScroll = (CScrollBar*)nRui->CreateWnd( n_wtScrollBar, WID_Quest_List_Scroll , WID_Quest_Wnd, 320 , 55, 13 , 109 , false );
	ListScroll->InitSlide( stVertSld , 0 , RP_MAX_QUEST_FLAG - 5, 0 , 13 );//Type, min, max, value, 썸의 폭
	ListScroll->SetClientImage( TID_CTRL_SCR_BG );
	ListScroll->SetThumbImage( TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR_CLK, TID_None );
	ListScroll->SetScrollBtnA( TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT_CLK, TID_None );
	ListScroll->SetScrollBtnB( TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT_CLK, TID_None );
	
	// 스크롤바 ( Context )
	DescScroll = (CScrollBar*)nRui->CreateWnd( n_wtScrollBar, WID_Quest_Context_Scroll , WID_Quest_Wnd, 320 , 176 , 13 , 107 + 50, false );
	DescScroll->InitSlide( stVertSld , 0 , 99 , 0 , 13 );//Type, min, max, value, 썸의 폭
	DescScroll->SetClientImage( TID_CTRL_SCR_BG );
	DescScroll->SetThumbImage( TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR_CLK, TID_None );
	DescScroll->SetScrollBtnA( TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT_CLK, TID_None );
	DescScroll->SetScrollBtnB( TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT_CLK, TID_None );

	

	for( int i = 0 ; i < QUEST_FIELD_NUM ; i++ )
	{
		QField[i].QuestName = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None , WID_Quest_Wnd, 12, 63 + ( 20 * i ) , 237, 15, false );  
		QField[i].QuestName->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
		QField[i].QuestName->SetClientImage( TID_None );
		QField[i].QuestName->SetAlignText( n_atLeft, n_atCenter );
		
		QField[i].TimeLimit = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_Quest_Wnd, 12, 34 , 320, 15, false );
		QField[i].TimeLimit->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
		QField[i].TimeLimit->SetClientImage( TID_None );
		QField[i].TimeLimit->SetAlignText( n_atRight, n_atCenter );
		QField[i].TimeLimit->SetText(_RT("Time Limit 22 Miniute"));
		QField[i].TimeLimit->SetTextColor( 0, 0, 0 );
	
		for( int j = 0 ; j < 4 ; j++ )
		{
			QField[i].CompleteItem[j] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_Quest_Wnd, 
																	15, 340 + ( 15 * j ), 252, 15, false ); 
			QField[i].CompleteItem[j]->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
			QField[i].CompleteItem[j]->SetClientImage( TID_None );
			QField[i].CompleteItem[j]->SetTextColor( 0, 0, 0 );
			QField[i].CompleteItem[j]->SetAlignText( n_atLeft, n_atCenter );
		}

		QField[i].QuestFlag = NULL;

		SelBtn[i] = (CButton*)nRui->CreateWnd( n_wtButton, WID_None , WID_Quest_Wnd , 12,  62 + ( 20 * i ) , 301, 15, false );  
		SelBtn[i]->SetClientImage( TID_None );
		SelBtn[i]->SetBtnImage( TID_None, TID_CTRL_TEXT_OVER, TID_CTRL_TEXT_OVER, TID_None );
	}

	// 퀘스트 내용
	QuestDescText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_Quest_ContextText, WID_Quest_Wnd, 9, 182, 307, 145, false );  
	QuestDescText->SetFontR( n_fRsvFontWndStatic2 );
	QuestDescText->SetText(_RT(""));//test
	QuestDescText->SetMultiLine( TRUE );
	QuestDescText->SetLineInterval(4);
	QuestDescText->SetHID( HID_None );
	
	return;
	
}
