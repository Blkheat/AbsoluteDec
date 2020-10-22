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
#include "..\\..\\bravolibs\\obj\\player.h"

#include "EditBox.h"
#include "PartyInforWnd.h"

#include "ChatBoxWndProc.h"
#include "StringManager.h"
#include "MCommunityWnd.h"
#include "BattleZoneWndProc.h"



CChatMessageWnd	ChatMessageWnd;
CChatBoxWnd		ChatBoxWnd;
CChatOutputWnd	ChatOutputWnd;

extern long		g_nowTime;


//-----------------------------------------------------------------------------
void CChatBoxWnd::Init()
{
}

//-----------------------------------------------------------------------------
void CChatBoxWnd::Composition()
{
	//-------------------------------------------------------------------------
	//
	//	채팅 입력창
	//
	//-------------------------------------------------------------------------
	CFrameWnd* fw = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_ChatBoxWnd, WID_None, 166, (nRui->Desktop.Height - 49 - 28), 665, 28, false );
	fw->SetClientImage( TID_Chat_Wnd );//TID_Chat_Wnd
	fw->SetCaption( _RT("ChatBox Window") );
	fw->SetWndProc( this );
	fw->SetTitle( false );	
	fw->SetVisible( false );
	fw->SetMove( false );
	fw->SetZorder( n_zoTop );
	fw->Initialize();
	
	CEditBox* eb = (CEditBox*)nRui->CreateWnd( n_wtEditBox, WID_ChatBox_MessageEdit, WID_ChatBoxWnd, 60, 6, 573, 16, false );
	eb->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	eb->SetTextSize( 62 );
	eb->SetClientImage( TID_None );	
	eb->SetClientColor( 0, 0, 0 );
	eb->SetAlignText( n_atLeft, n_atCenter );
	eb->SetBlank( 1, 1, 0, 0 );
	eb->SetText( _RT("") );
	eb->SetEditStyle( esText );
	eb->SetReturnActionClear( true );
	eb->InitEditBox();
	//WID_ChatBox_SizeBtn
	CButton* btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_ChatBox_SizeBtn, WID_ChatBoxWnd, 639, 4, 19, 18, false );
	btn->SetBtnImage( TID_None, TID_Chat_SizeBtn_Focus, TID_Chat_SizeBtn_Select, TID_None );

	//WID_ChatBox_ChatTypeBtn
	StateImg = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_ChatBox_ChatTypeImg, WID_ChatBoxWnd, 7, 5, 46, 18, false );
	StateImg->SetClientImage( TID_None );
	StateImg->SetClientColor( 255, 255, 255 );

	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_ChatBox_ChatTypeBtn, WID_ChatBoxWnd, 7, 5, 46, 18, false );
	btn->SetBtnImage( TID_None, TID_None, TID_None, TID_None );
}

//-----------------------------------------------------------------------------
void CChatBoxWnd::Update()
{
}

//-----------------------------------------------------------------------------
void CChatBoxWnd::Proc( SEventMessage* EMsg )
{
	static TCHAR		chatstring[64]			= "";
	static TCHAR		receiver_message[64]	= "";
	static TCHAR		saymessage[64]			= "";
	
	int			buflen = 64;
	//int			len;

	SRect	opsize;
	SRect	resize;
	int		i			= 0;
	bool	find_tok	= false;

	char	tstr[40] = "";
	//int		tstep = 0;

	switch( EMsg->FocusWnd )
	{
	case WID_ChatBox_ChatTypeBtn:
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			switch( ChatState )
			{
			case n_NormarChat:				
				break;		
				break;
			/*case n_GuildChat:
				StateImg->SetClientImage( TID_Chat_ChatType_Guild );
				nRui->SendEvent( WID_ChatBox_MessageEdit, n_emSetText, (EPARAM)_T(""), 255, 255, 255 );
				break;*/
			}			
		}
		break;
	case WID_ChatBox_SizeBtn:
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			SizeState++;
			if( SizeState >= 3 )
			{
				SizeState = 0;
			}

			ChatOutputWnd.ChatTxt->SetOutputLevel( SizeState );//theValue;
			ChatOutputWnd.ChatWnd->GetClientSize( &opsize );
			
			resize.x = opsize.x;
			resize.y = opsize.y;
			resize.w = opsize.w;
			resize.h = 12;
			ChatOutputWnd.Fimg_up->ResetSize( &resize );
		}
		break;
	case WID_ChatBox_MessageEdit:
		if( EMsg->Notify == NM_EDT_RETURN_ACTION )
		{
			nRui->SendEvent( WID_ChatBox_MessageEdit, n_emGetText, (EPARAM)chatstring, buflen, 0, 0 );
			nRui->SetEnterFlag( n_nfEditEnter );

			// 필터링
			if( g_RockClient.m_SlangFilter.Filter( ( char * ) chatstring ) )
			{
				memset( chatstring , 0 , 64 );
				Rsprintf( RWCHAR(chatstring) , _RT("%s") , RWCHAR(g_RockClient.m_SlangFilter.GetString()) );
			}
			
			switch( ChatState )
			{
			case n_NormarChat:
				if( Rstrlen( chatstring ) != 0 )
				{
					Rsprintf( RWCHAR(saymessage), _RT("%s : %s"), RWCHAR(nRui->thePcParam.PCName), RWCHAR(chatstring) );
					//sprintf( saymessage, "%s", chatstring );				
					
					ChatOutputWnd.ChatTxt->SetChatItem( saymessage, D3DCOLOR_XRGB( 255, 255, 44 ) );
					ChatMessageWnd.ChatTxt->SetChatItem( saymessage, D3DCOLOR_XRGB( 255, 255, 44 ) );
					
					g_RockClient.SetPlayerMessage( chatstring );
					
					//by simwosung Step이 0이면 초기 시간설정
					if(ChatMessageWnd.Step == 0)
                      ChatMessageWnd.theOutputTime = g_nowTime;					
					
					ChatMessageWnd.Step += 1;
					ChatMessageWnd.ChatTxt->SetOutputStep( ChatMessageWnd.Step );
					
					nRui->SendEvent( WID_ChatBox_MessageEdit, n_emSetFocus, 0, 0, 0, 0 );
					g_RockClient.Send_RpCsSay( chatstring );

				}
				else
				{
					nRui->SendEvent( WID_ChatBoxWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
					nRui->SendEvent( WID_ChatOutputWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
					nRui->SendEvent( WID_ChatMessageWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
				}
				break;
			case n_WhisperChat:
				//_receiver = _tcstok( saymessage, " " );
				//if( _tcslen( receiver_message ) != 0 )
				if( Rstrlen( chatstring ) != 0 )
				{	
					nRui->SendEvent( WID_ChatBox_MessageEdit, n_emSetFocus, 0, 0, 0, 0 );				
					/*_tcscat( Receiver, " " );
					/nRui->SendEvent( WID_ChatBox_MessageEdit, n_emSetText, (EPARAM)Receiver, 255, 255, 255 );*/
				}
				else
				{
					nRui->SendEvent( WID_ChatBoxWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
					nRui->SendEvent( WID_ChatOutputWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
					nRui->SendEvent( WID_ChatMessageWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
				}


				break;
			case n_FriendChat:
				ChatOutputWnd.ChatTxt->SetChatItem( G_STRING_CHAR( IDS_PREPARE ) , D3DCOLOR_XRGB( 255, 0, 0 ) );
				ChatMessageWnd.ChatTxt->SetChatItem( G_STRING_CHAR( IDS_PREPARE ) , D3DCOLOR_XRGB( 255, 0, 0 ) );
				ChatMessageWnd.Step += 1;
				ChatMessageWnd.ChatTxt->SetOutputStep( ChatMessageWnd.Step );

				nRui->SendEvent( WID_ChatBoxWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
				nRui->SendEvent( WID_ChatOutputWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
				nRui->SendEvent( WID_ChatMessageWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
				break;
			case n_PartyChat:
				if( Rstrlen( chatstring ) != 0 )
				{	
					
					nRui->SendEvent( WID_ChatBox_MessageEdit, n_emSetFocus, 0, 0, 0, 0 );					
					if(!g_PartyInforWnd.IsMemberParty())
					{
						ChatOutputWnd.ChatTxt->SetChatItem( G_STRING_CHAR( IDS_SYSPR_NOT_BELONG_PARTY ) , D3DCOLOR_XRGB( 255, 0, 0 ) );
					  return;
					}
					
					Rsprintf( RWCHAR(saymessage), _RT("%s"), RWCHAR(chatstring) );

					//by simwosung Step이 0이면 초기 시간설정 
					if(ChatMessageWnd.Step == 0)
                      ChatMessageWnd.theOutputTime = g_nowTime;				
					
#ifdef	NETWORK_CLIENT
					g_RockClient.Send_RpCsPartyChat( chatstring );
#endif				
				}
				else
				{
					nRui->SendEvent( WID_ChatBoxWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
					nRui->SendEvent( WID_ChatOutputWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
					nRui->SendEvent( WID_ChatMessageWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );				
				}
				break;
			case n_GuildChat:
				ChatOutputWnd.ChatTxt->SetChatItem( G_STRING_CHAR( IDS_PREPARE ) , D3DCOLOR_XRGB( 255, 0, 0 ) );
				ChatMessageWnd.ChatTxt->SetChatItem( G_STRING_CHAR( IDS_PREPARE ) , D3DCOLOR_XRGB( 255, 0, 0 ) );
				ChatMessageWnd.Step += 1;
				ChatMessageWnd.ChatTxt->SetOutputStep( ChatMessageWnd.Step );

				nRui->SendEvent( WID_ChatBoxWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
				nRui->SendEvent( WID_ChatOutputWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
				nRui->SendEvent( WID_ChatMessageWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
				break;
			}
		}
		break;
	}
}


//-----------------------------------------------------------------------------
void CChatOutputWnd::Init()
{
}

//-----------------------------------------------------------------------------
void CChatOutputWnd::Composition()
{
	SRect fwsize;
	SRect cosize;

	//-------------------------------------------------------------------------
	//
	//	채팅출력창
	//
	//-------------------------------------------------------------------------
	ChatWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_ChatOutputWnd, WID_None, 166, (nRui->Desktop.Height - 49 - 28 - 120), 665, 120, false );
	ChatWnd->SetClientImage( TID_None );
	ChatWnd->SetCaption( _RT("ChatOutput Window") );
	ChatWnd->SetWndProc( this );	
	ChatWnd->SetTitle( false );	
	ChatWnd->SetVisible( false );
	ChatWnd->SetMove( false );
	ChatWnd->SetFocusCheck( false );
	ChatWnd->SetZorder( n_zoTop );
	ChatWnd->Initialize();

	ChatTxt = (CChatOutput*)nRui->CreateWnd( n_wtChatOutput, WID_ChatOutput_Text, WID_ChatOutputWnd, 0, 0, 20, 20, false );
	ChatTxt->SetClientImage( TID_CLIENT );
	ChatTxt->SetOutputLevel( 1 );//초기 Output창 사이즈....
	
	ChatWnd->GetClientSize( &fwsize );
	ChatTxt->GetClientSize( &cosize );

	SRect frame;
	frame.x = 0;
	frame.y = 0;
	frame.w = fwsize.w;
	frame.h = 12;
	
	Fimg_up = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_ChatOutput_FUpImg, WID_ChatOutputWnd, frame.x, frame.y, frame.w, frame.h, false );
	Fimg_up->SetClientImage( TID_Chat_FrameBar );
	Fimg_up->SetClientColor( 255, 255, 255 );
}

//-----------------------------------------------------------------------------
void CChatOutputWnd::Update()
{
}

//-----------------------------------------------------------------------------
void CChatOutputWnd::Proc( SEventMessage* EMsg )
{
	/*	
	static CChatOutput*	optext = (CChatOutput*)Container->Get( WID_ChatOutput_Text );
	int line_value = 0;
	

	switch( EMsg->FocusWnd )
	{
	case WID_ChatOutput_Scroll5:
		if( ( EMsg->Notify == NM_SLD_MOVEBAR ) || ( EMsg->Notify == NM_SLD_CLICKMOVEBAR ) || ( EMsg->Notify == NM_SLD_BTNMOVERBAR ) )
		{
			line_value = optext->GetNumLine() - ( 99 - EMsg->fParam );// 99 - scroll의 max값....
			optext->SetCurLine( line_value );
		}
		break;
	case WID_ChatOutput_Scroll8:
		if( ( EMsg->Notify == NM_SLD_MOVEBAR ) || ( EMsg->Notify == NM_SLD_CLICKMOVEBAR ) || ( EMsg->Notify == NM_SLD_BTNMOVERBAR ) )
		{
			line_value = optext->GetNumLine() - ( 99 - EMsg->fParam );
			optext->SetCurLine( line_value );
		}
		break;
	}
	//*/
}






//-----------------------------------------------------------------------------
void CChatMessageWnd::Init()
{
}

//-----------------------------------------------------------------------------
void CChatMessageWnd::Composition()
{
	SRect fwsize;
	SRect cosize;

	//-------------------------------------------------------------------------
	//
	//	채팅출력창
	//
	//-------------------------------------------------------------------------
	ChatWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_ChatMessageWnd, WID_None, 166, (nRui->Desktop.Height - 49 - 12), 665, 12, false );
	ChatWnd->SetClientImage( TID_None );
	ChatWnd->SetCaption( _RT("ChatMessage Window") );
	ChatWnd->SetWndProc( this );	
	ChatWnd->SetTitle( false );	
	ChatWnd->SetVisible( false );
	ChatWnd->SetMove( false );
	ChatWnd->SetFocusCheck( false );
	ChatWnd->SetZorder( n_zoBottom );
	ChatWnd->Initialize();

	ChatTxt = (CChatOutput*)nRui->CreateWnd( n_wtChatOutput, WID_ChatMessage_Text, WID_ChatMessageWnd, 0, 0, 20, 20, false );
	ChatTxt->SetClientImage( TID_Chat_MessageBG );
	ChatTxt->SetOutputStep( 0 );//초기 Output창 사이즈....
	ChatTxt->SetMaxLine( 16 );
	
	theOutputTime = g_nowTime;
}

//-----------------------------------------------------------------------------
void CChatMessageWnd::Update()
{
}

//-----------------------------------------------------------------------------
void CChatMessageWnd::Proc( SEventMessage* EMsg )
{
}

//-----------------------------------------------------------------------------



