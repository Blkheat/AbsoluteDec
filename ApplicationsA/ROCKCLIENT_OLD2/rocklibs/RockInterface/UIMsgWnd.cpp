#include "..\\..\\RockClient.h"
#include "..\\..\\brother.h"
#include "..\\RockPCH.h"
#include "UiUtil.h"

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

#include "..\\..\\global.h"
#include "..\\..\\brother.h"
#include "..\\..\\RockClient.h"

#include "MessageWndProc.h"
#include "ExchangeWndProc.h"
#include "InventoryWndProc.h"
#include "UIMsgWnd.h"
#include "stringmanager.h"
#include "MessageBox.h"

CUIMsgWnd g_UIMsgWnd;

CUIMsgWnd::CUIMsgWnd()
{
	memset(m_strParam , 0  , MAX_SIZE_256 );
	memset(m_strSecondParam , 0  , MAX_SIZE_256 );
		
	m_UIMsg_Type = MSG_PARTYCALL;

	m_pUIMsgFrame = NULL;

	m_pUIMsgOkBtn = NULL;
	m_pUIMsgCancleBtn = NULL;
	m_pUIMsgInEdit = NULL;
	m_pUIMsgLongInEdit = NULL;

	m_pUIMsgTextBox[0] = NULL;
	m_pUIMsgTextBox[1] = NULL;
	m_pUIMsgTextBox[2] = NULL;
#ifdef DECO_RENEWAL_GUILD_MJH
	m_pUIMsgFrameAdd = NULL;
	m_pUIMsgCancleBtnAdd = NULL;
	m_pUIMsgLongInEditAdd = NULL;
	m_pUIMsgMultiBoxAdd = NULL;

#endif // DECO_RENEWAL_GUILD_MJH

	m_dNameID = 0;
	m_ItemID = 0;
	m_aPetSlot = 0;
}

CUIMsgWnd::~CUIMsgWnd()
{

}

void CUIMsgWnd::Init()
{
	m_pUIMsgInEdit->SetText( _RT("") );
}

void CUIMsgWnd::Composition()
{
   int center_x	= nRui->Desktop.Width / 2;
   int center_y	= nRui->Desktop.Height / 2;

   SRect UIMsgWndRect;
   UIMsgWndRect.w = 249;
   UIMsgWndRect.h = 141;
   UIMsgWndRect.x	= center_x - ( UIMsgWndRect.w / 2 );
   UIMsgWndRect.y	= center_y - ( UIMsgWndRect.h / 2 );
   
   m_pUIMsgFrame = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_UIMsgWnd, WID_None, 
	   UIMsgWndRect.x, UIMsgWndRect.y, UIMsgWndRect.w, UIMsgWndRect.h, false );
   
   m_pUIMsgFrame->SetWndTexStyle( fwfTex_Style_Tile9 );
   m_pUIMsgFrame->SetClientImage( TID_CTRL_WND_L1 );	
   m_pUIMsgFrame->SetWndTileSizeLR( 40, 29 );
   m_pUIMsgFrame->SetWndTileSizeTB( 31, 11 );
   m_pUIMsgFrame->SetTitle( true );   
   m_pUIMsgFrame->SetWndProc( this );
   m_pUIMsgFrame->SetVisible( false );
   m_pUIMsgFrame->SetMove( false );
   m_pUIMsgFrame->SetZorder( n_zoTop );
   m_pUIMsgFrame->Initialize();
   m_pUIMsgFrame->SetCaption( G_STRING_CHAR( IDS_WND_PT_REQUEST ) ); 
	  
#ifdef DECO_RENEWAL_GUILD_MJH
 
   UIMsgWndRect.w = 429;
   UIMsgWndRect.h = 141;
   UIMsgWndRect.x	= center_x - ( UIMsgWndRect.w / 2 );
   UIMsgWndRect.y	= center_y - ( UIMsgWndRect.h / 2 );
   
   m_pUIMsgFrameAdd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_UIMsgWndAdd, WID_None, 
	   UIMsgWndRect.x, UIMsgWndRect.y, UIMsgWndRect.w, UIMsgWndRect.h, false );
   
   m_pUIMsgFrameAdd->SetWndTexStyle( fwfTex_Style_Tile9 );
   m_pUIMsgFrameAdd->SetClientImage( TID_CTRL_WND_L1 );	
   m_pUIMsgFrameAdd->SetWndTileSizeLR( 40, 29 );
   m_pUIMsgFrameAdd->SetWndTileSizeTB( 31, 11 );
   m_pUIMsgFrameAdd->SetTitle( true );   
   m_pUIMsgFrameAdd->SetWndProc( this );
   m_pUIMsgFrameAdd->SetVisible( false );
   m_pUIMsgFrameAdd->SetMove( false );
   m_pUIMsgFrameAdd->SetZorder( n_zoTop );
   m_pUIMsgFrameAdd->Initialize();
   m_pUIMsgFrameAdd->SetCaption( G_STRING_CHAR( IDS_WND_PT_REQUEST ) );   
   
   m_pUIMsgLongInEditAdd = (CEditBox*)nRui->CreateWnd( n_wtEditBox, WID_UIMsgLongInAddEdit, WID_UIMsgWndAdd, 15, 75, 400, 17, false );									
   m_pUIMsgLongInEditAdd->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
   m_pUIMsgLongInEditAdd->SetTextSize( 60 );
   m_pUIMsgLongInEditAdd->SetClientImage( TID_UIMsg_InputBox );   
   m_pUIMsgLongInEditAdd->SetAlignText( n_atLeft, n_atCenter );
   m_pUIMsgLongInEditAdd->SetBlank( 4, 0, 0, 0 );
   m_pUIMsgLongInEditAdd->SetEditStyle( esText );
   m_pUIMsgLongInEditAdd->SetText( _RT("") );
   m_pUIMsgLongInEditAdd->SetReturnActionClear( false );
   m_pUIMsgLongInEditAdd->InitEditBox();      
   m_pUIMsgLongInEditAdd->SetVisible( FALSE );

   m_pUIMsgMultiBoxAdd = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_UIMsgWndAdd, 
	                                90, 45, 249, 50, false );   
   m_pUIMsgMultiBoxAdd->SetFontR( n_fRsvFontWndStatic );
   m_pUIMsgMultiBoxAdd->SetClientColor( 180, 175, 160 ); 
   m_pUIMsgMultiBoxAdd->SetZorder( n_zoTop );
   m_pUIMsgMultiBoxAdd->SetVisible( TRUE ); 
   m_pUIMsgMultiBoxAdd->SetAlignText( n_atCenter, n_atTop );   
   m_pUIMsgMultiBoxAdd->SetMultiLine( true );
   m_pUIMsgMultiBoxAdd->SetLineInterval( 5 );
   m_pUIMsgMultiBoxAdd->SetMultiLineAsign(true);   
   m_pUIMsgMultiBoxAdd->SetVisible(false);
   m_pUIMsgMultiBoxAdd->SetText(_RT(""));

   //확인 버튼
   m_pUIMsgOkBtnAdd = (CButton*)nRui->CreateWnd( n_wtButton, WID_UIMsgOkBtnAdd, WID_UIMsgWndAdd, 153, 108, 58, 25, false );	                                   
   m_pUIMsgOkBtnAdd->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_CTRL_BTN58_DIS );
   m_pUIMsgOkBtnAdd->SetFontR( n_fRsvFontWndButton );   
   m_pUIMsgOkBtnAdd->SetText( G_STRING_CHAR( IDS_WND_COM_OK ) );
	

   //취소 버튼
   m_pUIMsgCancleBtnAdd = (CButton*)nRui->CreateWnd( n_wtButton, WID_UIMsgCancleBtnAdd, WID_UIMsgWndAdd, 220, 108, 58, 25, false );	                                
   m_pUIMsgCancleBtnAdd->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_CTRL_BTN58_DIS );
   m_pUIMsgCancleBtnAdd->SetFontR( n_fRsvFontWndButton );
   m_pUIMsgCancleBtnAdd->SetText( G_STRING_CHAR( IDS_WND_COM_CANCEL ) );
   
   img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_UIMsgWnd, 20, 56, 35, 35, false );       
   img ->SetClientImage( TID_CTRL_SLOT_SUNKEN );
   img->SetVisible( false );

   m_item_slot = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None ,  WID_UIMsgWnd, 20, 56, 35, 35, false );	
   m_item_slot->SetSlotGroup( n_SGROUP_PROPOSE_FRONTIER );
   m_item_slot->SetVisible( false );

#endif // DECO_RENEWAL_GUILD_MJH
   
   m_pUIMsgInEdit = (CEditBox*)nRui->CreateWnd( n_wtEditBox, WID_UIMsgInEdit, WID_UIMsgWnd, 69, 81, 108, 17, false );									
   m_pUIMsgInEdit->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
   m_pUIMsgInEdit->SetTextSize( 18 );
   m_pUIMsgInEdit->SetClientImage( TID_UIMsg_InputBox );   
   m_pUIMsgInEdit->SetAlignText( n_atLeft, n_atCenter );
   m_pUIMsgInEdit->SetBlank( 4, 0, 0, 0 );
   m_pUIMsgInEdit->SetEditStyle( esText );
   m_pUIMsgInEdit->SetText( _RT("") );
   m_pUIMsgInEdit->SetReturnActionClear( false );
   m_pUIMsgInEdit->InitEditBox();      
   m_pUIMsgInEdit->SetVisible( FALSE );

   m_pUIMsgLongInEdit = (CEditBox*)nRui->CreateWnd( n_wtEditBox, WID_None, WID_UIMsgWnd, 15, 81, 218, 17, false );									
   m_pUIMsgLongInEdit->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
   m_pUIMsgLongInEdit->SetTextSize( 33 );
   m_pUIMsgLongInEdit->SetClientImage( TID_UIMsg_InputBox );   
   m_pUIMsgLongInEdit->SetAlignText( n_atLeft, n_atCenter );
   m_pUIMsgLongInEdit->SetBlank( 4, 0, 0, 0 );
   m_pUIMsgLongInEdit->SetEditStyle( esText );
   m_pUIMsgLongInEdit->SetText( _RT("") );
   m_pUIMsgLongInEdit->SetReturnActionClear( false );
   m_pUIMsgLongInEdit->InitEditBox();      
   m_pUIMsgLongInEdit->SetVisible( FALSE );

   m_pUIMsgTextBox[0] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_UIMsgNotifyText1, WID_UIMsgWnd, 
	                                0, 58, 249, 15, false );
   m_pUIMsgTextBox[0]->SetFontR( n_fRsvFontWndStatic );   
   m_pUIMsgTextBox[0]->SetClientColor( 180, 175, 160 );
   m_pUIMsgTextBox[0]->SetZorder( n_zoTop );
   m_pUIMsgTextBox[0]->SetVisible( false );
   m_pUIMsgTextBox[0]->SetText(_RT(""));

   m_pUIMsgTextBox[1] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_UIMsgNotifyText2, WID_UIMsgWnd, 
	                                0, 74, 249, 15, false );
   m_pUIMsgTextBox[1]->SetFontR( n_fRsvFontWndStatic );
   m_pUIMsgTextBox[1]->SetClientColor( 180, 175, 160 );
   m_pUIMsgTextBox[1]->SetZorder( n_zoTop );
   m_pUIMsgTextBox[1]->SetVisible( false );
   m_pUIMsgTextBox[1]->SetText(_RT(""));

   m_pUIMsgTextBox[2] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_UIMsgNotifyText3, WID_UIMsgWnd, 
	                                0, 80, 249, 15, false );   
   m_pUIMsgTextBox[2]->SetFontR( n_fRsvFontWndStatic );
   m_pUIMsgTextBox[2]->SetClientColor( 180, 175, 160 ); 
   m_pUIMsgTextBox[2]->SetZorder( n_zoTop );
   m_pUIMsgTextBox[2]->SetVisible( false );
   m_pUIMsgTextBox[2]->SetText(_RT(""));

   m_pUIMsgMultiBox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_UIMsgWnd, 
	                                0, 49, 249, 50, false );   
   m_pUIMsgMultiBox->SetFontR( n_fRsvFontWndStatic );
   m_pUIMsgMultiBox->SetClientColor( 180, 175, 160 ); 
   m_pUIMsgMultiBox->SetZorder( n_zoTop );
   m_pUIMsgMultiBox->SetVisible( TRUE ); 
   m_pUIMsgMultiBox->SetAlignText( n_atCenter, n_atTop );   
   m_pUIMsgMultiBox->SetMultiLine( true );
   m_pUIMsgMultiBox->SetLineInterval( 5 );
   m_pUIMsgMultiBox->SetMultiLineAsign(true);   
   m_pUIMsgMultiBox->SetVisible(false);
   m_pUIMsgMultiBox->SetText(_RT(""));

    //확인 버튼
   m_pUIMsgOkBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_UIMsgOkBtn, WID_UIMsgWnd, 63, 108, 58, 25, false );	                                   
   m_pUIMsgOkBtn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_CTRL_BTN58_DIS );
   m_pUIMsgOkBtn->SetFontR( n_fRsvFontWndButton );   
   m_pUIMsgOkBtn->SetText( G_STRING_CHAR( IDS_WND_COM_OK ) );
	

   //취소 버튼
   m_pUIMsgCancleBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_UIMsgCancleBtn, WID_UIMsgWnd, 130, 108, 58, 25, false );	                                
   m_pUIMsgCancleBtn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_CTRL_BTN58_DIS );
   m_pUIMsgCancleBtn->SetFontR( n_fRsvFontWndButton );
   m_pUIMsgCancleBtn->SetText( G_STRING_CHAR( IDS_WND_COM_CANCEL ) );   
}

void CUIMsgWnd::Update()
{

}

void CUIMsgWnd::Proc( SEventMessage* EMsg )
{
	switch( EMsg->FocusWnd )
	{
		case WID_UIMsgOkBtn:
			{
				if( EMsg->Notify == NM_BTN_CLICK )
				{
					BtnClickProc( WID_UIMsgOkBtn );				
				}
			}
			break;
		case WID_UIMsgCancleBtn:
			{
				if( EMsg->Notify == NM_BTN_CLICK )
				{
					BtnClickProc( WID_UIMsgCancleBtn );			
				}
			}		
			break;
#ifdef DECO_RENEWAL_GUILD_MJH
		case WID_UIMsgOkBtnAdd:
			{
				if( EMsg->Notify == NM_BTN_CLICK )
				{
					BtnClickProc( WID_UIMsgOkBtnAdd );				
				}
			}
			break;
		case WID_UIMsgCancleBtnAdd:
			{
				if( EMsg->Notify == NM_BTN_CLICK )
				{
					BtnClickProc( WID_UIMsgCancleBtnAdd );			
				}
			}		
			break;
#endif // DECO_RENEWAL_GUILD_MJH
		case WID_UIMsgInEdit:
			{
				if( EMsg->Notify == NM_EDT_RETURN_ACTION )
				{
					//확인 버튼과 같은 처리
					BtnClickProc( WID_UIMsgOkBtn );
					nRui->SetEnterFlag( n_nfEditEnter );
				}
			}
			break;
	}

	if( EMsg->FocusWnd == m_pUIMsgLongInEdit->GetID() )
	{
		if( EMsg->Notify == NM_EDT_RETURN_ACTION )
		{
			//확인 버튼과 같은 처리
			BtnClickProc( WID_UIMsgOkBtn );
			nRui->SetEnterFlag( n_nfEditEnter );
		}
	}
	#ifdef DECO_RENEWAL_GUILD_MJH
	if( EMsg->FocusWnd == m_pUIMsgLongInEditAdd->GetID() )
	{
		if( EMsg->Notify == NM_EDT_RETURN_ACTION )
		{
			//확인 버튼과 같은 처리
			BtnClickProc( WID_UIMsgOkBtnAdd );
			nRui->SetEnterFlag( n_nfEditEnter );
		}
	}
	#endif // DECO_RENEWAL_GUILD_MJH
}

void CUIMsgWnd::BtnClickProc( int WIDBtn )
{
	static TCHAR m_strTemp[256] = {0,};
	static TCHAR m_strTempAdd[256] = {0,};
	D3DCOLOR tcolor = D3DCOLOR_XRGB( 0, 215, 5 );
	
	switch( m_UIMsg_Type )
	{
		case MSG_PARTYCALL:			//파티 요청
		case MSG_LEAGUECALL:
			{
			   if( WIDBtn == WID_UIMsgOkBtn )
			   {
				   //파티 수락 보내기
#ifdef	NETWORK_CLIENT
				   g_RockClient.Send_RpCsPartyAnsJoin(m_strParam, true);
#endif				   
				   nRui->SendEvent( WID_UIMsgWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );

			   }
			   if( WIDBtn == WID_UIMsgCancleBtn )
			   {
				   //파티 거부 보내기
#ifdef	NETWORK_CLIENT        
				   g_RockClient.Send_RpCsPartyAnsJoin(m_strParam, false);
#endif
				   nRui->SendEvent( WID_UIMsgWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
				   
			   }
			}
			break;
		case MSG_PARTYBREAK:
		case MSG_LEAGUEBREAK://파티 해체
			{
				if( WIDBtn == WID_UIMsgOkBtn )
				{
				   //파티 해체 
				   nRui->SendEvent( WID_UIMsgWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );

				}
				if( WIDBtn == WID_UIMsgCancleBtn )
				{
                   nRui->SendEvent( WID_UIMsgWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );	
				}
			}
			break;
		case MSG_PARTYGIVE: //파티 양도
			{
				if( WIDBtn == WID_UIMsgOkBtn )
				{
				   //파티 양도 보내기    
				   g_RockClient.Send_RpCsChangeLeader(m_strParam);				   
				   nRui->SendEvent( WID_UIMsgWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
				}
				if( WIDBtn == WID_UIMsgCancleBtn )
				{
                   nRui->SendEvent( WID_UIMsgWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );	
				}
			}
			break;
		case MSG_PARTYIN:
		case MSG_LEAGUEIN:   //파티 초대  
			{
				if( WIDBtn == WID_UIMsgOkBtn )
				{
				   m_pUIMsgInEdit->GetText( m_strTemp, 18 );
				   m_pUIMsgInEdit->SetText( _RT("") );

				   if( Rstrcmp( m_strTemp, _RT("") ) == 0)
				   {
					   SystemPrint(n_DarkGreen ,n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_PT_INPUT_PTMAN ) ); 					  
				      return;
				   }

				   SPcDataParam* PcParam = nRui->GetPcParamInfo();				   
				   if( Rstrcmp(PcParam->PCName, m_strTemp) == 0 )
				   {
					   SystemPrint(n_DarkGreen ,n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_PT_CANT_INVITE_ME ));  
	                  return;
				   }
				   else
				   {
					   g_RockClient.Send_RpCsPartyAskJoin(m_strTemp, n_PARTY);				   
				   }					
					//파티 초대 보내기 
				   nRui->SendEvent( WID_UIMsgWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
				}
				if( WIDBtn == WID_UIMsgCancleBtn )
				{
                   nRui->SendEvent( WID_UIMsgWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );	
				}
			}
			break;
		case MSG_PARTYOUT:
		case MSG_LEAGUEOUT:  //파티 탈퇴
			{
				if( WIDBtn == WID_UIMsgOkBtn )
				{
#ifdef	NETWORK_CLIENT        
				   g_RockClient.Send_RpCsPartyLeave(m_strParam);
#endif					
					//파티 탈퇴 보내기 
				   nRui->SendEvent( WID_UIMsgWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
				}
				if( WIDBtn == WID_UIMsgCancleBtn )
				{
                   nRui->SendEvent( WID_UIMsgWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );	
				}
			}
			break;
		 case MSG_PARTYRENAME:
		 case MSG_LEAGUENAME:  //파티 이름바꾸기
			 {
				if( WIDBtn == WID_UIMsgOkBtn )
				{
				   //파티 이름 바꾸기 보내기 
				   m_pUIMsgInEdit->GetText( m_strTemp, 18 );
				   m_pUIMsgInEdit->SetText( _RT("") );

				   if( Rstrcmp( m_strTemp, _RT("") ) == 0)
				   {
					  SystemPrint(n_DarkGreen ,n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_INPUT_PARTY_NAME ));					  
				      return;
				   }		

				   if(g_RockClient.m_SlangFilter.IsCheckSymbol(G_STRING_CHAR( IDS_SYSPR_PET_CREATE_NOT_WRITE ), RWCHAR(m_strTemp) ))
				   {
				      MessageBoxWnd.Open( G_STRING_CHAR( IDS_SYSPR_PET_CREATE_NOT_WRITE ) , 1200 , NULL );
				      return;
				   }

				   if( !g_RockClient.m_SlangFilter.IsValidString( m_strTemp ) )
				   {
				 	  MessageBoxWnd.Open( G_STRING_CHAR( IDS_MSGBOX_NAME_SPECIAL_TEXT ) , 1500 , NULL );
					  return;
				   }		
				
				   if( g_RockClient.m_SlangFilter.ISFindFilterName( m_strTemp ) )
				   {
					  MessageBoxWnd.Open( G_STRING_CHAR( IDS_MSGBOX_NAME_INVALID_TEXT ) , 1500 , NULL );
					  return;
				   }	
				   
				   //파티, 리그 이름 필터링
				   if( g_RockClient.m_SlangFilter.Filter( ( char * ) m_strTemp ) )
				   {
						memset( m_strTemp , 0 , sizeof(TCHAR) * 256 );
						Rsprintf( RWCHAR(m_strTemp) , _RT("%s") , RWCHAR(g_RockClient.m_SlangFilter.GetString())  );
				   }
#ifdef	NETWORK_CLIENT        
				   g_RockClient.Send_RpCsPartyChangeName(m_strTemp);
#endif				   
				   nRui->SendEvent( WID_UIMsgWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
				}
				if( WIDBtn == WID_UIMsgCancleBtn )
				{
                   nRui->SendEvent( WID_UIMsgWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );	
				}
			 }
			 break;
		case MSG_PARTYGOAWAY:       //강제 퇴장시키기
			{
				if( WIDBtn == WID_UIMsgOkBtn )
				{
#ifdef	NETWORK_CLIENT        
				   g_RockClient.Send_RpCsPartyLeave(m_strParam);
#endif	
				   //멤버 강제 퇴장시키기 
				   nRui->SendEvent( WID_UIMsgWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
				}
				if( WIDBtn == WID_UIMsgCancleBtn )
				{
                   nRui->SendEvent( WID_UIMsgWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );	
				}			
			}		
			break;
		case MSG_ACCEPT_EXCHANGE:	 //거래요청
			{
				if( WIDBtn == WID_UIMsgOkBtn )
				{
#ifdef	NETWORK_CLIENT        
					g_RockClient.Send_RpCsAnsExchange( n_Success );
						
					// 거래 승락 
					InventoryWnd.FrameWnd->Reposition( 470, 245 );
#ifdef WIDE_PAY_INVENTORY //유료인벤확장			
					InventoryWnd.Open();
#else
					nRui->SendEvent( WID_InventoryWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
#endif
					g_ExchangeWnd.Open();
					g_ExchangeWnd.m_bIsAccept = TRUE;
					SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_ALLOW_EXCHANGE ), m_strParam );
#endif	
				  
				}
				if( WIDBtn == WID_UIMsgCancleBtn )
				{
                    // 거래거절
					g_RockClient.Send_RpCsAnsExchange( n_Fail );
				}
			}
			break;
		case MSG_MFRIENDINVITE:		//친구초대
			{
				if( WIDBtn == WID_UIMsgOkBtn )
				{
				//친구 초대에 응함
#ifdef	NETWORK_CLIENT        
				   g_RockClient.Send_CsMsngrMemberInsertAns(true, m_strParam);
#endif					   
				   nRui->SendEvent( WID_UIMsgWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
				}
				if( WIDBtn == WID_UIMsgCancleBtn )
				{
                   //친구 초대 거절
#ifdef	NETWORK_CLIENT        
				   g_RockClient.Send_CsMsngrMemberInsertAns(false, m_strParam);
#endif					   					
				   nRui->SendEvent( WID_UIMsgWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );	
				}

			}
			break;
		case MSG_MCHATINVITE:		//채팅 초대작업
			{
				if( WIDBtn == WID_UIMsgOkBtn )
				{
#ifdef	NETWORK_CLIENT        
				   g_RockClient.Send_CsMsngrChatInviteAns(true, m_strParam, m_strSecondParam);
#endif	
				   //채팅 초대에 응함
				   nRui->SendEvent( WID_UIMsgWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
				   memset(m_strSecondParam , NULL , MAX_SIZE_256  );

				}
				if( WIDBtn == WID_UIMsgCancleBtn )
				{
#ifdef	NETWORK_CLIENT        
				   g_RockClient.Send_CsMsngrChatInviteAns(false, m_strParam, m_strSecondParam);
#endif	          				
					//채팅 초대 거절
				   nRui->SendEvent( WID_UIMsgWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
				   memset(m_strSecondParam , NULL , MAX_SIZE_256  );
				}

			}
			break;
		case MSG_FRONTIERINVITE:	//프론티어 초대작업
			{
				if( WIDBtn == WID_UIMsgOkBtn )
				{

#ifdef FRONTIER				   
				   g_RockClient.Send_CsMsngrAnswerJoinFrontier( n_Success );
#endif				
				   //프론티어 초대에 응함
				   nRui->SendEvent( WID_UIMsgWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );

				}
				if( WIDBtn == WID_UIMsgCancleBtn )
				{
#ifdef FRONTIER	  
          			g_RockClient.Send_CsMsngrAnswerJoinFrontier( n_Fail );	
#endif
					//프론티어 초대 거절
				   nRui->SendEvent( WID_UIMsgWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );				  
				}

			}
			break;
		case MSG_MOVE_DEST:	 //출두 하고자하는 대상
			{
				if( WIDBtn == WID_UIMsgOkBtn )
				{
				   m_pUIMsgInEdit->GetText( m_strTemp, 18 );
				   m_pUIMsgInEdit->SetText( _RT("") );

				   if( Rstrcmp( m_strTemp, _RT("") ) == 0)
				   {
					   SystemPrint(n_DarkGreen ,n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_INPUT_MOVE_DEST ) );		  
				      return;
				   }

				   SPcDataParam* PcParam = nRui->GetPcParamInfo();				   
				   if( Rstrcmp(PcParam->PCName, m_strTemp) == 0 )
				   {
					   SystemPrint(n_DarkGreen ,n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANNOT_MOVE_MYSELF ) );  
	                  return;
				   }
				   else
				   {
					   g_RockClient.Send_CsUseMovePayItem(m_ItemID, m_strTemp);	   
				   }
				   
				   m_ItemID = 0;
				   nRui->SendEvent( WID_UIMsgWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
				}
				
				if( WIDBtn == WID_UIMsgCancleBtn )
				{
                    m_ItemID = 0;
					nRui->SendEvent( WID_UIMsgWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );	
				}	

			}
			break;
		case MSG_CALL_DEST:	 //소환 하고자하는 대상
			{
				if( WIDBtn == WID_UIMsgOkBtn )
				{
				   m_pUIMsgInEdit->GetText( m_strTemp, 18 );
				   m_pUIMsgInEdit->SetText( _RT("") );

				   if( Rstrcmp( m_strTemp, _RT("") ) == 0)
				   {
					   SystemPrint(n_DarkGreen ,n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_INPUT_CALL_DEST ) ); 					  
				      return;
				   }

				   SPcDataParam* PcParam = nRui->GetPcParamInfo();				   
				   if( Rstrcmp(PcParam->PCName, m_strTemp) == 0 )
				   {
					   SystemPrint(n_DarkGreen ,n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANNOT_CALL_MYSELF ) );     
	                  return;
				   }
				   else
				   {
					   g_RockClient.Send_CsUseMovePayItem(m_ItemID, m_strTemp);	   
				   }
				   
				   m_ItemID = 0;				   
				   nRui->SendEvent( WID_UIMsgWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
				}
				
				if( WIDBtn == WID_UIMsgCancleBtn )
				{
                   m_ItemID = 0;
				   nRui->SendEvent( WID_UIMsgWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );	
				}
			}
			break;
		case MSG_CALL_ASK:	 //소환 요청작업
			{
				if( WIDBtn == WID_UIMsgOkBtn )
				{
					g_RockClient.Send_CsAnsMove(m_strParam, TRUE);				   
					nRui->SendEvent( WID_UIMsgWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
				}				
				if( WIDBtn == WID_UIMsgCancleBtn )
				{
                    g_RockClient.Send_CsAnsMove(m_strParam, FALSE);	
					nRui->SendEvent( WID_UIMsgWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );	
				}
			}
			break;
		case MSG_INCOMBINE_FRONTIER:	//연합프론티어 요청작업
			{
				if( WIDBtn == WID_UIMsgOkBtn )
				{
					g_RockClient.Send_CsMsngrAnsCombine( m_dNameID, TRUE );
					m_dNameID = 0;
					nRui->SendEvent( WID_UIMsgWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
				}				
				if( WIDBtn == WID_UIMsgCancleBtn )
				{
					g_RockClient.Send_CsMsngrAnsCombine( m_dNameID, FALSE );
					m_dNameID = 0;
					nRui->SendEvent( WID_UIMsgWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );	
				}
			}
			break;
		case MSG_RE_PETNAME:			//펫 이름 바꾸기 작업
			{
				if( WIDBtn == WID_UIMsgOkBtn )
				{
					TCHAR * strTemp = m_strTemp;
					
					m_pUIMsgInEdit->GetText( m_strTemp, 18 );
					m_pUIMsgInEdit->SetText( _RT("") );					
					
					if( Rstrcmp( m_strTemp, _RT("") ) == 0)
					{
					   SystemPrint(n_DarkGreen ,n_SystemMsg, G_STRING_CHAR( IDS_SYSINPUT_NEWNAME ) );			  
					   return;
					}

					int iLen = Rstrlen(RWCHAR(m_strTemp));
					if( iLen > ( RP_MAX_PET_NAME - 1 ) )
					{
					   SystemPrint( n_DarkGreen ,n_SystemMsg, G_STRING_CHAR( IDS_SYSOVERNAMELENGTH ) );	
					   return;	   
					}

					///-- 네임에 ' '<--스페이스 안들어가게 막기
					char *str = (char *)m_strTemp;	
					while( *str != '\0' )
					{
						if( *str == ' '  )
						{
							MessageBoxWnd.Open( G_STRING_CHAR( IDS_MSG_REMOVE_SPACE_INFNAME  ), 1200, NULL );							
							return;	
						}

						str++;
					}

					if(g_RockClient.m_SlangFilter.IsCheckSymbol(G_STRING_CHAR( IDS_SYSPR_PET_CREATE_NOT_WRITE ), RWCHAR(m_strTemp) ))
					{
						MessageBoxWnd.Open( G_STRING_CHAR( IDS_SYSPR_PET_CREATE_NOT_WRITE ) , 1200 , NULL );
						return;
					}

					if( !g_RockClient.m_SlangFilter.IsValidString( m_strTemp ) )
					{
						MessageBoxWnd.Open( G_STRING_CHAR( IDS_MSGBOX_NAME_SPECIAL_TEXT ) , 1500 , NULL );
						return;
					}		
					
					if( g_RockClient.m_SlangFilter.ISFindFilterName( m_strTemp ) )
					{
						MessageBoxWnd.Open( G_STRING_CHAR( IDS_MSGBOX_NAME_INVALID_TEXT ) , 1500 , NULL );
						return;
					}	
					
					//펫 이름 필터링
					if( g_RockClient.m_SlangFilter.Filter( ( char * ) m_strTemp ) )
					{
						memset( m_strTemp , 0 , sizeof(TCHAR) * 256 );
						Rsprintf( RWCHAR(m_strTemp) , _RT("%s") , RWCHAR(g_RockClient.m_SlangFilter.GetString()) );
					}
					
					g_RockClient.Send_CsUsePetItem( m_aPetSlot, m_ItemID, m_strTemp );
					m_aPetSlot = 0;
					m_ItemID = 0;					
					nRui->SendEvent( WID_UIMsgWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
				}				
				
				if( WIDBtn == WID_UIMsgCancleBtn )
				{
					m_aPetSlot = 0;
					m_ItemID = 0;					
					nRui->SendEvent( WID_UIMsgWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );	
				}
			}
			break;
		case MSG_RE_PCNAME:
			{
				if( WIDBtn == WID_UIMsgOkBtn )
				{
					TCHAR * strTemp = m_strTemp;
					
					m_pUIMsgInEdit->GetText( m_strTemp, RP_MAX_PC_NAME - sizeof(RTCHAR) );
					m_pUIMsgInEdit->SetText( _RT("") );					
					
					if( Rstrcmp( m_strTemp, _RT("") ) == 0 )
					{
					   SystemPrint(n_DarkGreen ,n_SystemMsg, G_STRING_CHAR( IDS_SYSINPUT_NEWNAME ) );						  
					   return;
					}

					int iLen = Rstrlen(RWCHAR(m_strTemp));
					if( iLen > ( RP_MAX_PC_NAME - sizeof(RTCHAR) ) )
					{
					   SystemPrint( n_DarkGreen ,n_SystemMsg, G_STRING_CHAR( IDS_SYSOVERNAMELENGTH ) );	
					   return;	   
					}

					///-- 네임에 ' '<--스페이스 안들어가게 막기
					char *str = (char *)m_strTemp;	
					while( *str != '\0' )
					{
						if( *str == ' '  )
						{
							MessageBoxWnd.Open( G_STRING_CHAR( IDS_MSG_REMOVE_SPACE_INFNAME  ), 1200, NULL );							
							return;	
						}

						str++;
					}

					if(g_RockClient.m_SlangFilter.IsCheckSymbol(G_STRING_CHAR( IDS_SYSPR_PET_CREATE_NOT_WRITE ), RWCHAR(m_strTemp) ))
					{
						MessageBoxWnd.Open( G_STRING_CHAR( IDS_SYSPR_PET_CREATE_NOT_WRITE ) , 1200 , NULL );
						return;
					}
  
					if( !g_RockClient.m_SlangFilter.IsValidString( m_strTemp ) )
					{
						MessageBoxWnd.Open( G_STRING_CHAR( IDS_MSGBOX_NAME_SPECIAL_TEXT ) , 1500 , NULL );
						return;
					}		
					
					if( g_RockClient.m_SlangFilter.ISFindFilterName( m_strTemp ) )
					{
						MessageBoxWnd.Open( G_STRING_CHAR( IDS_MSGBOX_NAME_INVALID_TEXT ) , 1500 , NULL );
						return;
					}	

					//이름 필터링
					if( g_RockClient.m_SlangFilter.Filter( ( char * ) m_strTemp ) )
					{
						memset( m_strTemp , 0 , sizeof(TCHAR) * 256 );
						Rsprintf( RWCHAR(m_strTemp) , _RT("%s") , RWCHAR(g_RockClient.m_SlangFilter.GetString()) );
					}

					g_RockClient.Send_CsChangeName( m_ItemID, m_strTemp );				
					m_ItemID = 0;					
					nRui->SendEvent( WID_UIMsgWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
				}

				if( WIDBtn == WID_UIMsgCancleBtn )
				{
					m_ItemID = 0;					
					nRui->SendEvent( WID_UIMsgWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );	
				}
			}
			break;
		case MSG_RE_FRONTIER_NAME:
			{
				if( WIDBtn == WID_UIMsgOkBtn )
				{
					TCHAR * strTemp = m_strTemp;
					
					m_pUIMsgLongInEdit->GetText( m_strTemp, 33 );
					m_pUIMsgLongInEdit->SetText( _RT("") );					
					
					if( Rstrcmp( m_strTemp, _RT("") ) == 0 )
					{
					   SystemPrint(n_DarkGreen ,n_SystemMsg, G_STRING_CHAR( IDS_SYSINPUT_NEWNAME ) );					  
					   return;
					}

					int iLen = Rstrlen(RWCHAR(m_strTemp));
					if( iLen > ( RP_MAX_FRONTIER_NAME - 1 ) )
					{
					   SystemPrint( n_DarkGreen ,n_SystemMsg, G_STRING_CHAR( IDS_SYSOVERNAMELENGTH ) );	
					   return;	   
					}

					///-- 프론티어 네임에 ' '<--스페이스 안들어가게 막기
					char *str = (char *)m_strTemp;	
					while( *str != '\0' )
					{
						if( *str == ' '  )
						{
							MessageBoxWnd.Open( G_STRING_CHAR( IDS_MSG_REMOVE_SPACE_INFNAME  ), 1200, NULL );							
							return;	
						}

						str++;
					}

					if(g_RockClient.m_SlangFilter.IsCheckSymbol(G_STRING_CHAR( IDS_SYSPR_PET_CREATE_NOT_WRITE ), RWCHAR(m_strTemp) ))
					{
						MessageBoxWnd.Open( G_STRING_CHAR( IDS_SYSPR_PET_CREATE_NOT_WRITE ) , 1200 , NULL );
						return;
					}

					if( !g_RockClient.m_SlangFilter.IsValidString( m_strTemp ) )
					{
						MessageBoxWnd.Open( G_STRING_CHAR( IDS_MSGBOX_NAME_SPECIAL_TEXT ) , 1500 , NULL );
						return;
					}		
					
					if( g_RockClient.m_SlangFilter.ISFindFilterName( m_strTemp ) )
					{
						MessageBoxWnd.Open( G_STRING_CHAR( IDS_MSGBOX_NAME_INVALID_TEXT ) , 1500 , NULL );
						return;
					}					

					//이름 필터링
					if( g_RockClient.m_SlangFilter.Filter( ( char * ) m_strTemp ) )
					{
						memset( m_strTemp , 0 , sizeof(TCHAR) * 256 );
						Rsprintf( RWCHAR(m_strTemp) , _RT("%s") , RWCHAR(g_RockClient.m_SlangFilter.GetString()) );
					}

					g_RockClient.Send_CsChangeName( m_ItemID, m_strTemp );				
					m_ItemID = 0;					
					nRui->SendEvent( WID_UIMsgWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
				}

				if( WIDBtn == WID_UIMsgCancleBtn )
				{
					m_ItemID = 0;					
					nRui->SendEvent( WID_UIMsgWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );	
				}
			}
			break;
		case MSG_RE_GENDER:
			{
				if( WIDBtn == WID_UIMsgOkBtn )
				{
					g_RockClient.Send_RpCsUse( m_ItemID );				
					m_ItemID = 0;					
					nRui->SendEvent( WID_UIMsgWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
				}

				if( WIDBtn == WID_UIMsgCancleBtn )
				{
					m_ItemID = 0;					
					nRui->SendEvent( WID_UIMsgWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );	
				}
			}
			break;
#ifdef DECO_RENEWAL_GUILD_MJH
			case MSG_FRONTIERMYINFO_INPUT: // 자기소개서 작성
			{
				if( WIDBtn == WID_UIMsgOkBtnAdd )
				{	
					TCHAR * strTemp = m_strTemp;
					m_pUIMsgLongInEditAdd->GetText( m_strTemp, 60 );
					m_pUIMsgLongInEditAdd->SetText( _RT("") );
					m_ItemID = 0;	
					g_MessageBox.SetStrParam( strTemp ); 
					g_MessageBox.OpenMessageBox( n_MESSAGEBOX_CHANGE_MYINFO, G_STRING_CHAR( IDS_FRONTIERMAIN_MYINFO ) , 250, 100 );
					nRui->SendEvent( WID_UIMsgWndAdd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
				}

				if( WIDBtn == WID_UIMsgCancleBtnAdd )
				{			
					m_ItemID = 0;	
					nRui->SendEvent( WID_UIMsgWndAdd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );	
				}
			}
			break;
			case MSG_FRONTIERNOTICE_INPUT: // 프론티어 공지사항 작성 
			{
				if( WIDBtn == WID_UIMsgOkBtnAdd )
				{	
					TCHAR * strTemp = m_strTemp;
					m_pUIMsgLongInEditAdd->GetText( m_strTemp, 60 );
					m_pUIMsgLongInEditAdd->SetText( _RT("") );
					m_ItemID = 0;
					g_MessageBox.SetStrParam( strTemp );
					g_MessageBox.OpenMessageBox( n_MESSAGEBOX_CHANGE_NOTICE, G_STRING_CHAR( IDS_NOTICE_T2 ) , 250, 100 );
					nRui->SendEvent( WID_UIMsgWndAdd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
				}

				if( WIDBtn == WID_UIMsgCancleBtnAdd )
				{
					m_ItemID = 0;	
					nRui->SendEvent( WID_UIMsgWndAdd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );	
				}
			}
			break;
#endif // DECO_RENEWAL_GUILD_MJH
#ifdef DECO_RENEWAL_CORPORATION_MJH
			case MSG_CORPORATION_JOIN_1: // 1군단 가입
			{
				if( WIDBtn == WID_UIMsgOkBtn )
				{
					g_RockClient.Send_CsMsngrCorpJoin( 1 );				
					m_ItemID = 0;					
					nRui->SendEvent( WID_UIMsgWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
				}

				if( WIDBtn == WID_UIMsgCancleBtn )
				{
					m_ItemID = 0;					
					nRui->SendEvent( WID_UIMsgWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );	
				}
			}
			break;
			case MSG_CORPORATION_JOIN_2: // 2군단 가입
			{
				if( WIDBtn == WID_UIMsgOkBtn )
				{
					g_RockClient.Send_CsMsngrCorpJoin( 2 );				
					m_ItemID = 0;					
					nRui->SendEvent( WID_UIMsgWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
				}

				if( WIDBtn == WID_UIMsgCancleBtn )
				{
					m_ItemID = 0;					
					nRui->SendEvent( WID_UIMsgWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );	
				}
			}
			break;
			case MSG_CORPORATION_JOIN_3: // 3군단 가입
			{
				if( WIDBtn == WID_UIMsgOkBtn )
				{
					g_RockClient.Send_CsMsngrCorpJoin( 3 );				
					m_ItemID = 0;					
					nRui->SendEvent( WID_UIMsgWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
				}

				if( WIDBtn == WID_UIMsgCancleBtn )
				{
					m_ItemID = 0;					
					nRui->SendEvent( WID_UIMsgWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );	
				}
			}
			break;
			case MSG_CORPORATION_LEAVE: // 군단탈퇴
			{
				if( WIDBtn == WID_UIMsgOkBtn )
				{
					g_RockClient.Send_CsMsngrCorpLeave();				
					m_ItemID = 0;					
					nRui->SendEvent( WID_UIMsgWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
				}

				if( WIDBtn == WID_UIMsgCancleBtn )
				{
					m_ItemID = 0;					
					nRui->SendEvent( WID_UIMsgWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );	
				}
			}
			break;
#endif // DECO_RENEWAL_CORPORATION_MJH
	}
}
void CUIMsgWnd::UpdateItemSlot()
{
	m_item_slot->PcItem = &m_pPcItem;
}
void CUIMsgWnd::SetParam( TCHAR * strParam )
{
	Rstrcpy( RWCHAR(m_strParam), RWCHAR(strParam) );
}

void CUIMsgWnd::SetSecondParam( TCHAR * strParam )
{
	Rstrcpy( RWCHAR(m_strSecondParam), RWCHAR(strParam) );
}

void CUIMsgWnd::SetParamID( DWORD dNameID )
{
	m_dNameID = dNameID;
}

void CUIMsgWnd::SetItemID( DWORD ItemID )
{
	m_ItemID = ItemID; 
}

void CUIMsgWnd::SetPetSlot( BYTE aPetSlot )
{
	m_aPetSlot = aPetSlot;
}

void CUIMsgWnd::SetMsgType( UINT UIMsgType )
{
	TCHAR strTempBuf[512] = {0,};
	
	m_UIMsg_Type = UIMsgType;
	
	m_pUIMsgInEdit->SetVisible( false );
	m_pUIMsgLongInEdit->SetVisible( false );
	m_pUIMsgTextBox[0]->SetWndRepositionR(0, 49);
	m_pUIMsgTextBox[1]->SetWndRepositionR(0, 65);
	m_pUIMsgTextBox[2]->SetWndRepositionR(0, 81);
	m_pUIMsgTextBox[0]->SetTextColor( 100, 100, 0 );

	m_pUIMsgTextBox[0]->SetVisible( false );
	m_pUIMsgTextBox[1]->SetVisible( false );
	m_pUIMsgTextBox[2]->SetVisible( false );
	m_pUIMsgMultiBox->SetVisible( false );
#ifdef DECO_RENEWAL_GUILD_MJH
	m_pUIMsgLongInEditAdd->SetVisible( false );
	m_pUIMsgMultiBoxAdd->SetVisible( false );
#endif // DECO_RENEWAL_GUILD_MJH

	switch( m_UIMsg_Type )
	{
		case MSG_PARTYCALL:
			{
				Rsprintf( RWCHAR(strTempBuf), G_STRING_CHAR( IDS_WND_AL_SUBJECT2 ), RWCHAR(m_strParam) );				

				m_pUIMsgFrame->SetCaption( G_STRING_CHAR( IDS_WND_PT_REQUEST ) ); 	
								
				m_pUIMsgTextBox[0]->SetVisible(true);
				m_pUIMsgTextBox[1]->SetVisible(true);
				m_pUIMsgTextBox[2]->SetVisible(true);				
				
				m_pUIMsgTextBox[0]->SetText(strTempBuf);
				m_pUIMsgTextBox[1]->SetText( G_STRING_CHAR( IDS_PT_INV_MSG1 ) );			
				m_pUIMsgTextBox[2]->SetText( G_STRING_CHAR( IDS_PT_INV_MSG2 ) );				
			}
			break;
		case MSG_LEAGUECALL:
			{
				Rsprintf( RWCHAR(strTempBuf), G_STRING_CHAR( IDS_WND_AL_SUBJECT2 ), RWCHAR(m_strParam) );
				m_pUIMsgFrame->SetCaption( G_STRING_CHAR( IDS_CALL_LEAGUE ) ) ;
				
				m_pUIMsgTextBox[0]->SetVisible(true);
				m_pUIMsgTextBox[1]->SetVisible(true);
				m_pUIMsgTextBox[2]->SetVisible(true);
				
				m_pUIMsgTextBox[0]->SetText(strTempBuf);
				m_pUIMsgTextBox[1]->SetText( G_STRING_CHAR( IDS_INVITE_LEAGUE ) );			
				m_pUIMsgTextBox[2]->SetText( G_STRING_CHAR( IDS_PT_INV_MSG2 ) );
			}
			break;
		case MSG_PARTYBREAK:
			{
				m_pUIMsgFrame->SetCaption( G_STRING_CHAR( IDS_WND_PT_DESTROY )); 	
				m_pUIMsgTextBox[0]->SetWndRepositionR(0, 58);
				m_pUIMsgTextBox[1]->SetWndRepositionR(0, 74);
				
				m_pUIMsgTextBox[0]->SetVisible(true);
				m_pUIMsgTextBox[1]->SetVisible(true);
				m_pUIMsgTextBox[2]->SetVisible(false);				

				m_pUIMsgTextBox[0]->SetText( G_STRING_CHAR( IDS_PT_DEST_MSG1 ) );					
				m_pUIMsgTextBox[1]->SetText( G_STRING_CHAR( IDS_PT_DEST_MSG2 ) );					
			}
			break;
		case MSG_LEAGUEBREAK:
			{
				m_pUIMsgFrame->SetCaption( G_STRING_CHAR( IDS_BREAK_LEAGUE ) );			
				m_pUIMsgTextBox[0]->SetWndRepositionR(0, 58);
				m_pUIMsgTextBox[1]->SetWndRepositionR(0, 74);

				m_pUIMsgTextBox[0]->SetVisible(true);
				m_pUIMsgTextBox[1]->SetVisible(true);
				m_pUIMsgTextBox[2]->SetVisible(false);	

				m_pUIMsgTextBox[0]->SetText( G_STRING_CHAR( IDS_BR_LE_1 ) );					
				m_pUIMsgTextBox[1]->SetText( G_STRING_CHAR( IDS_BR_LE_2 ) );		
			}
			break;
		case MSG_PARTYGIVE:
			{
				Rsprintf(RWCHAR(strTempBuf), G_STRING_CHAR( IDS_WND_TOMAN ), RWCHAR(m_strParam));					

				m_pUIMsgFrame->SetCaption( G_STRING_CHAR( IDS_WND_PT_ASSIGNMENT )); 	
				
				m_pUIMsgTextBox[0]->SetWndRepositionR(0, 58);
				m_pUIMsgTextBox[1]->SetWndRepositionR(0, 74);

				m_pUIMsgTextBox[0]->SetVisible(true);
				m_pUIMsgTextBox[1]->SetVisible(true);
				m_pUIMsgTextBox[2]->SetVisible(false);	
				
				m_pUIMsgTextBox[0]->SetText(strTempBuf);
				m_pUIMsgTextBox[1]->SetText( G_STRING_CHAR( IDS_PT_AS_MSG1 ) );		

			}
			break;
		case MSG_PARTYIN:
			{
				m_pUIMsgFrame->SetCaption( G_STRING_CHAR( IDS_WND_PT_REQUEST ) ); 	
				m_pUIMsgInEdit->SetVisible( true );

				m_pUIMsgTextBox[0]->SetVisible(true);
				m_pUIMsgTextBox[1]->SetVisible(true);
				m_pUIMsgTextBox[2]->SetVisible(false);				

				m_pUIMsgTextBox[0]->SetText( G_STRING_CHAR( IDS_PT_REQ_MSG1 ) );			
				m_pUIMsgTextBox[1]->SetText( G_STRING_CHAR( IDS_PT_REQ_MSG2 ) );					
			}
			break;
		case MSG_LEAGUEIN:
			{
				m_pUIMsgFrame->SetCaption( G_STRING_CHAR( IDS_CALL_LEAGUE ) ); 		
				m_pUIMsgInEdit->SetVisible( true );

				m_pUIMsgTextBox[0]->SetVisible(true);
				m_pUIMsgTextBox[1]->SetVisible(true);
				m_pUIMsgTextBox[2]->SetVisible(false);	

				m_pUIMsgTextBox[0]->SetText( G_STRING_CHAR( IDS_CAL_LE_1 ) );		 	
				m_pUIMsgTextBox[1]->SetText( G_STRING_CHAR( IDS_CAL_LE_2 ) );
			}
			break;
		case MSG_PARTYOUT:
			{
				m_pUIMsgFrame->SetCaption( G_STRING_CHAR( IDS_WND_PT_OUT ) ); 	
				m_pUIMsgTextBox[0]->SetWndRepositionR(0, 58);
				m_pUIMsgTextBox[1]->SetWndRepositionR(0, 74);

				m_pUIMsgTextBox[0]->SetVisible(true);
				m_pUIMsgTextBox[1]->SetVisible(true);
				m_pUIMsgTextBox[2]->SetVisible(false);

				m_pUIMsgTextBox[0]->SetText( G_STRING_CHAR( IDS_PT_OUT_MSG1 ) );				
				m_pUIMsgTextBox[1]->SetText( G_STRING_CHAR( IDS_PT_OUT_MSG2 ) );				
			}
			break;
		case MSG_LEAGUEOUT:
			{
				m_pUIMsgFrame->SetCaption( G_STRING_CHAR( IDS_OUT_LEAGUE ) ); 		
				m_pUIMsgTextBox[0]->SetWndRepositionR(0, 58);
				m_pUIMsgTextBox[1]->SetWndRepositionR(0, 74);
				
				m_pUIMsgTextBox[0]->SetVisible(true);
				m_pUIMsgTextBox[1]->SetVisible(true);
				m_pUIMsgTextBox[2]->SetVisible(false);

				m_pUIMsgTextBox[0]->SetText( G_STRING_CHAR( IDS_OUT_LE_1 ) );					
				m_pUIMsgTextBox[1]->SetText( G_STRING_CHAR( IDS_OUT_LE_2 ) );
			}
			break;
		case MSG_PARTYRENAME:
			{
				m_pUIMsgFrame->SetCaption( G_STRING_CHAR( IDS_WND_PT_RENAME ) ); 	
				m_pUIMsgInEdit->SetVisible( true );
				
				m_pUIMsgTextBox[0]->SetVisible(true);
				m_pUIMsgTextBox[1]->SetVisible(true);
				m_pUIMsgTextBox[2]->SetVisible(false);

				m_pUIMsgTextBox[0]->SetText( G_STRING_CHAR( IDS_PT_REN_MSG1 ) );		
				m_pUIMsgTextBox[1]->SetText( G_STRING_CHAR( IDS_PT_REN_MSG2 ) );					
			}
			break;
		case MSG_LEAGUENAME:
			{
				m_pUIMsgFrame->SetCaption( G_STRING_CHAR( IDS_RENAME_LEAGUE ) ); 		
				m_pUIMsgInEdit->SetVisible( true );

				m_pUIMsgTextBox[0]->SetVisible(true);
				m_pUIMsgTextBox[1]->SetVisible(true);
				m_pUIMsgTextBox[2]->SetVisible(false);				
		
				m_pUIMsgTextBox[0]->SetText( G_STRING_CHAR( IDS_RENA_LE_1 ) );		
				m_pUIMsgTextBox[1]->SetText( G_STRING_CHAR( IDS_RENA_LE_2 ) );	
			}
			break;
		case MSG_PARTYGOAWAY:
			{
				Rsprintf( RWCHAR(strTempBuf), G_STRING_CHAR( IDS_WND_MAN_OBJECT ), RWCHAR(m_strParam) );				

				m_pUIMsgFrame->SetCaption( G_STRING_CHAR( IDS_WND_PT_KICK ) ); 	
							
				m_pUIMsgTextBox[0]->SetVisible(true);
				m_pUIMsgTextBox[1]->SetVisible(true);
				m_pUIMsgTextBox[2]->SetVisible(true);				
				
				m_pUIMsgTextBox[0]->SetText(strTempBuf);
				m_pUIMsgTextBox[1]->SetText( G_STRING_CHAR( IDS_PT_KICK_MSG1 ));				
				m_pUIMsgTextBox[2]->SetText( G_STRING_CHAR( IDS_PT_KICK_MSG2 ));				
			}
			break;
		case MSG_ACCEPT_EXCHANGE:
			{
				Rsprintf( RWCHAR(strTempBuf), G_STRING_CHAR( IDS_WND_AL_SUBJECT2 ), RWCHAR(m_strParam) );				

				m_pUIMsgFrame->SetCaption( G_STRING_CHAR( IDS_WND_EX_REQUEST ) ); 
				
				m_pUIMsgTextBox[0]->SetVisible(true);
				m_pUIMsgTextBox[1]->SetVisible(true);
				m_pUIMsgTextBox[2]->SetVisible(true);
								
				m_pUIMsgTextBox[0]->SetText(strTempBuf);
				m_pUIMsgTextBox[1]->SetText( G_STRING_CHAR( IDS_EX_REQ_MSG1 ) );	    
				m_pUIMsgTextBox[2]->SetText( G_STRING_CHAR( IDS_EX_REQ_MSG2 ) );		    
			}
			break;
		case MSG_MFRIENDINVITE:
			{
				Rsprintf( RWCHAR(strTempBuf), G_STRING_CHAR( IDS_WND_AL_SUBJECT2 ), RWCHAR(m_strParam) );				
				
				m_pUIMsgFrame->SetCaption( G_STRING_CHAR( IDS_WND_FR_REQUEST ) );
				
				m_pUIMsgTextBox[0]->SetVisible(true);
				m_pUIMsgTextBox[1]->SetVisible(true);
				m_pUIMsgTextBox[2]->SetVisible(true);

				m_pUIMsgTextBox[0]->SetText(strTempBuf);
				m_pUIMsgTextBox[1]->SetText( G_STRING_CHAR( IDS_FR_REQ_MSG1 ) );	
				m_pUIMsgTextBox[2]->SetText( G_STRING_CHAR( IDS_FR_REQ_MSG2 ) );	
			}
			break;
		case MSG_MCHATINVITE:
			{
				Rsprintf( RWCHAR(strTempBuf), G_STRING_CHAR( IDS_WND_INVITE_CHATROOM ), RWCHAR(m_strParam), RWCHAR(m_strSecondParam) ); 

				m_pUIMsgFrame->SetCaption( G_STRING_CHAR( IDS_WND_FE_CHATTING )); 
				
				m_pUIMsgTextBox[0]->SetVisible(true);
				m_pUIMsgTextBox[1]->SetVisible(true);
				m_pUIMsgTextBox[2]->SetVisible(true);

				m_pUIMsgTextBox[0]->SetText(strTempBuf);
				m_pUIMsgTextBox[1]->SetText( G_STRING_CHAR( IDS_INV_REQ_MSG1 ) );		 
				m_pUIMsgTextBox[2]->SetText( G_STRING_CHAR( IDS_INV_REQ_MSG2 ));			 
			}
			break;
		case MSG_FRONTIERINVITE:
			{
				Rsprintf( RWCHAR(strTempBuf), G_STRING_CHAR( IDS_WND_AL_SUBJECT2 ), RWCHAR(m_strParam) );
				
				m_pUIMsgFrame->SetCaption( G_STRING_CHAR( IDS_INVITE_FRONTIER ) );
				
				m_pUIMsgTextBox[0]->SetVisible(true);
				m_pUIMsgTextBox[1]->SetVisible(true);
				m_pUIMsgTextBox[2]->SetVisible(true);

				m_pUIMsgTextBox[0]->SetText(strTempBuf);
				m_pUIMsgTextBox[1]->SetText( G_STRING_CHAR( IDS_SYSPR_INVITE_FRONTIER ) );		 
				m_pUIMsgTextBox[2]->SetText( G_STRING_CHAR( IDS_PT_INV_MSG2 ) );
#ifdef DECO_RENEWAL_GUILD_MJH
				img->SetVisible( true );
				m_item_slot->SetVisible( true );
#endif // DECO_RENEWAL_GUILD_MJH
				
				
			}
			break;
		case MSG_MOVE_DEST:
			{
				m_pUIMsgFrame->SetCaption( G_STRING_CHAR( IDS_MOVING_DEST ) ); 										
				m_pUIMsgInEdit->SetVisible( true );

				m_pUIMsgTextBox[0]->SetVisible(true);
				m_pUIMsgTextBox[1]->SetVisible(true);
				m_pUIMsgTextBox[2]->SetVisible(false);
		
				m_pUIMsgTextBox[0]->SetText( G_STRING_CHAR( IDS_SYSPR_MOVING_DEST_L1 ) );					
				m_pUIMsgTextBox[1]->SetText( G_STRING_CHAR( IDS_SYSPR_MOVING_DEST_L2 ) );				
			}
			break;
		case MSG_CALL_DEST:
			{
				m_pUIMsgFrame->SetCaption( G_STRING_CHAR( IDS_CALLING_DEST ) ); 								
				m_pUIMsgInEdit->SetVisible( TRUE );
			
				m_pUIMsgTextBox[0]->SetVisible(true);
				m_pUIMsgTextBox[1]->SetVisible(true);
				m_pUIMsgTextBox[2]->SetVisible(false);

				m_pUIMsgTextBox[0]->SetText( G_STRING_CHAR( IDS_SYSPR_CALLING_DEST_L1 ) );					
				m_pUIMsgTextBox[1]->SetText( G_STRING_CHAR( IDS_SYSPR_CALLING_DEST_L2 ) );				
			}
			break;
		case MSG_CALL_ASK:
			{
				Rsprintf( RWCHAR(strTempBuf), G_STRING_CHAR( IDS_WND_AL_SUBJECT2 ), RWCHAR(m_strParam) );				

				m_pUIMsgFrame->SetCaption( G_STRING_CHAR( IDS_ASK_CALLING ) );
				
				m_pUIMsgTextBox[0]->SetVisible(true);
				m_pUIMsgTextBox[1]->SetVisible(true);
				m_pUIMsgTextBox[2]->SetVisible(true);
				
				m_pUIMsgTextBox[0]->SetText(strTempBuf);
				m_pUIMsgTextBox[1]->SetText( G_STRING_CHAR( IDS_SYSPR_ASK_CALLING_L1 ) );					
				m_pUIMsgTextBox[2]->SetText( G_STRING_CHAR( IDS_SYSPR_ASK_CALLING_L2 ) );					
			}
			break;
		case MSG_INCOMBINE_FRONTIER:
			{
				Rsprintf( RWCHAR(strTempBuf), _RT("[%s]님이##연합프론티어를 요청하셨습니다.##수락하시겠습니까?"), RWCHAR(m_strParam) );		///-- HAN_MSG
				
				m_pUIMsgFrame->SetCaption( _RT("연합요청") );	///-- HAN_MSG
				m_pUIMsgMultiBox->SetVisible( true );
				m_pUIMsgMultiBox->SetText( strTempBuf );
			}			
			break;
		case MSG_RE_PETNAME:
			{
				Rsprintf( RWCHAR(strTempBuf), G_STRING_CHAR( IDS_SYSINPUT_CHANGE_PETNAME ) , RWCHAR(m_strParam) );		
				
				m_pUIMsgInEdit->SetVisible( TRUE );
				
				m_pUIMsgFrame->SetCaption( G_STRING_CHAR( IDS_CHANGE_PETNAME ) );	
				m_pUIMsgMultiBox->SetVisible( true );
				m_pUIMsgMultiBox->SetText( strTempBuf );
			}
			break;
		case MSG_RE_PCNAME:
			{
				Rsprintf( RWCHAR(strTempBuf), G_STRING_CHAR( IDS_SYSINPUT_CHANGE_NAME ), RWCHAR(m_strParam) );		

				m_pUIMsgInEdit->SetVisible( TRUE );

				m_pUIMsgFrame->SetCaption( G_STRING_CHAR( IDS_CHANGE_NAME ) );	
				m_pUIMsgMultiBox->SetVisible( true );
				m_pUIMsgMultiBox->SetText( strTempBuf );
			}
			break;
		case MSG_RE_FRONTIER_NAME:
			{
				Rsprintf( RWCHAR(strTempBuf), G_STRING_CHAR( IDS_SYSINPUT_CHANGEFRONNAME ), RWCHAR(m_strParam) );		

				m_pUIMsgLongInEdit->SetVisible( TRUE );

				m_pUIMsgFrame->SetCaption( G_STRING_CHAR( IDS_CHANGE_FRONNAME ) );	
				m_pUIMsgMultiBox->SetVisible( true );
				m_pUIMsgMultiBox->SetText( strTempBuf );
				nRui->SendEvent( m_pUIMsgLongInEdit->GetID() , n_emSetFocus, 0, 0, 0, 0 );
			}
			break;
		case MSG_RE_GENDER:
			{
				Rsprintf( RWCHAR(strTempBuf), G_STRING_CHAR( IDS_SYS_CHANGE_GENDER ), RWCHAR(m_strParam) );		
				
				m_pUIMsgFrame->SetCaption( G_STRING_CHAR( IDS_CHANGE_GENDER ) );	
				m_pUIMsgMultiBox->SetVisible( true );
				m_pUIMsgMultiBox->SetText( strTempBuf );
			}
			break;
#ifdef DECO_RENEWAL_GUILD_MJH
			case MSG_FRONTIERMYINFO_INPUT:
			{
				Rsprintf( RWCHAR(strTempBuf),  _RT("[%s]님의 프로필을 입력해주세요 (최대 30자)"), RWCHAR(m_strParam) );		

				m_pUIMsgLongInEditAdd->SetVisible( TRUE );

				m_pUIMsgFrameAdd->SetCaption( _RT("자기소개") );	
				m_pUIMsgMultiBoxAdd->SetVisible( true );
				m_pUIMsgMultiBoxAdd->SetText( strTempBuf );
				nRui->SendEvent( m_pUIMsgLongInEditAdd->GetID() , n_emSetFocus, 0, 0, 0, 0 );
			}
			break;
			case MSG_FRONTIERNOTICE_INPUT:
			{
				Rsprintf( RWCHAR(strTempBuf),  _RT("공지사항을 입력해주세요 (최대 30자)"), RWCHAR(m_strParam) );		
				m_pUIMsgFrameAdd->SetCaption( _RT("공지사항") );	
				m_pUIMsgMultiBoxAdd->SetVisible( true );
				m_pUIMsgMultiBoxAdd->SetText( strTempBuf );
				nRui->SendEvent( m_pUIMsgLongInEditAdd->GetID() , n_emSetFocus, 0, 0, 0, 0 );
			}
			break;
#endif // DECO_RENEWAL_GUILD_MJH
			case MSG_CORPORATION_JOIN_1:
			{
				Rsprintf( RWCHAR(strTempBuf),  _RT("제 1군단에 가입하시겠습니까?"), RWCHAR(m_strParam) );		

				m_pUIMsgFrame->SetCaption( _RT("군단가입") );	
				m_pUIMsgMultiBox->SetVisible( true );
				m_pUIMsgMultiBox->SetText( strTempBuf );
			}
			break;	
			case MSG_CORPORATION_JOIN_2:
			{
				Rsprintf( RWCHAR(strTempBuf),  _RT("제 2군단에 가입하시겠습니까?"), RWCHAR(m_strParam) );		

				m_pUIMsgFrame->SetCaption( _RT("군단가입") );	
				m_pUIMsgMultiBox->SetVisible( true );
				m_pUIMsgMultiBox->SetText( strTempBuf );
			}
			break;
			case MSG_CORPORATION_JOIN_3:
			{
				Rsprintf( RWCHAR(strTempBuf),  _RT("제 3군단에 가입하시겠습니까?"), RWCHAR(m_strParam) );		

				m_pUIMsgFrame->SetCaption( _RT("군단가입") );	
				m_pUIMsgMultiBox->SetVisible( true );
				m_pUIMsgMultiBox->SetText( strTempBuf );
			}
			break;
			case MSG_CORPORATION_LEAVE:
			{
				Rsprintf( RWCHAR(strTempBuf),  _RT("정말 군단에서 탈퇴하시겠습니까?"), RWCHAR(m_strParam) );		

				m_pUIMsgFrame->SetCaption( _RT("군단탈퇴") );	
				m_pUIMsgMultiBox->SetVisible( true );
				m_pUIMsgMultiBox->SetText( strTempBuf );
			}
			break;
	}
}

void CUIMsgWnd::LogOff()
{
	if( IsVisible )
	{
		nRui->SendEvent( WID_UIMsgWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
#ifdef DECO_RENEWAL_GUILD_MJH
		nRui->SendEvent( WID_UIMsgWndAdd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
#endif // DECO_RENEWAL_GUILD_MJH

		switch(m_UIMsg_Type)
		{
		  case MSG_PARTYCALL:
			  {
				  //g_RockClient.Send_RpCsPartyAnsJoin( RWCHAR(m_strParam), false );	
			  }
			  break;
		  case MSG_ACCEPT_EXCHANGE:
			  {
				  //g_RockClient.Send_RpCsAnsExchange( n_Fail );
			  }			  
			  break;
		}
	}
}
