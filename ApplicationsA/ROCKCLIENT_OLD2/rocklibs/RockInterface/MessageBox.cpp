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

#include "MessageBox.h"
#include "ChatBoxWndProc.h"

#include "StoreWndProc.h"
#include "ExchangeWndProc.h"
#include "WarehouseWndProc.h"
#include "ItemRemodelWndProc.h"
#include "ItemCreateWndProc.h"
#include "stringmanager.h"

CMessageBox		g_MessageBox;


//-----------------------------------------------------------------------------
void CMessageBox::Composition()
{
	int center_x	= nRui->Desktop.Width / 2;
	int center_y	= nRui->Desktop.Height / 2;

	m32_param[0] = m32_param[1] = m32_param[2] = 0;

	SRect wnd;
	wnd.w	= 204;
	wnd.h	= 100;
	wnd.x	= center_x - ( wnd.w / 2 );
	wnd.y	= center_y - ( wnd.h / 2 );
	
	
	mpC_framewnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_MessageBox, WID_None, wnd.x, wnd.y, wnd.w, wnd.h, false );
	mpC_framewnd->SetWndTexStyle( fwfTex_Style_Tile9 );
	mpC_framewnd->SetClientImage( TID_CTRL_WND_L1 );
	mpC_framewnd->SetIconTID( TID_CTRL_ICON_ALRAM );
	mpC_framewnd->SetWndTileSizeLR( 40, 29 );
	mpC_framewnd->SetWndTileSizeTB( 31, 11 ); 
	mpC_framewnd->SetCaption( _RT("MESSAGE") );
	mpC_framewnd->SetTitle( true );
	mpC_framewnd->SetZorder( n_zoTop );
	mpC_framewnd->SetWndProc( this );	
	mpC_framewnd->SetVisible( false );
	mpC_framewnd->SetMove( false );
	mpC_framewnd->Initialize();
	
	m_Img9BK = (CImageBox9*)nRui->CreateWnd( n_wtImageBox9, WID_None, WID_MessageBox, 30, 2, 62, 72, false );       	
	m_Img9BK->SetWndTileSizeLR( 9, 9 );					// 윈도우 생성시와 동일 
	m_Img9BK->SetWndTileSizeTB( 8, 8 );					// 윈도우 생성시와 동일 
	m_Img9BK->SetFocusCheck( false );					// 이미지 사용시 필수 
	m_Img9BK->SetClientImage( TID_CTRL_CONTBACK_L1 );
	m_Img9BK->SetAdjustPosition( 0, 0 );

	///-- TextBox
	mpc_TextBox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None , WID_MessageBox, 16 , 8 , 90 , 60 , false );		
	mpc_TextBox->SetMultiLine( true );
	mpc_TextBox->SetFontR( n_fRsvMessageBox );		
	mpc_TextBox->SetTextColor( 225, 225, 255 );
	///--mpc_TextBox->SetLineInterval( 3 );
	
	///-- 확인 버튼
	mpC_BT_OK = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_MessageBox, 14, 21, 58, 25, false );	
	mpC_BT_OK->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_CTRL_BTN58_DIS );
	mpC_BT_OK->SetFontR( n_fRsvFontWndButton );   
	mpC_BT_OK->SetText( G_STRING_CHAR( IDS_WND_COM_OK ) );
	
	///-- 취소 버튼
	mpC_BT_CANCEL = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_MessageBox, 52, 22, 58, 25, false );	
	mpC_BT_CANCEL->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_CTRL_BTN58_DIS );	
	mpC_BT_CANCEL->SetFontR( n_fRsvFontWndButton ); 
	mpC_BT_CANCEL->SetText( G_STRING_CHAR( IDS_WND_COM_CANCEL ) );

}

///---------------------------------------------------------------------------
///-- _SetMessageBoxSize
///---------------------------------------------------------------------------
void CMessageBox::_SetMessageBoxSize( const int al_width, const int al_height )
{	
	int temp_x = ( al_width - ( 58 + 58 + 15) ) /2;
	int temp_texbox_x;	

	///---------------------------------------------------------------------------
	///-- TEXTBox 사이즈 및 위치
	SStringInfo *lpc_strinfo;		
	lpc_strinfo = mpc_TextBox->GetStringInfo();	

	///-- Textbox 가 FrameWnd 보다 작을 경우 Center 정렬
	if( lpc_strinfo->Width < al_width - 40 )
	{	
		mpc_TextBox->SetClientSize( lpc_strinfo->Width, al_height - 79 );
		temp_texbox_x = ( al_width - lpc_strinfo->Width ) / 2;

		m_Img9BK->SetClientSize( al_width - 40 + 12, al_height - 79 + 6 );
	}
	///-- 아닐경우 강제 크기 감소
	else
	{
		mpc_TextBox->SetClientSize( al_width - 40, al_height - 79 );	
		temp_texbox_x = 20;

		m_Img9BK->SetClientSize( al_width - 40 + 12, al_height - 79 + 6 );	
	}
	///-- 위치	
	mpc_TextBox->SetWndRepositionR( temp_texbox_x, 40 );	

	m_Img9BK->SetWndRepositionR( 20 - 6, 40 - 6 );	


	///---------------------------------------------------------------------------
	///-- Frame Wnd	
	mpC_framewnd->SetClientSize( al_width, al_height );

	switch( m_BtnType )
	{
	case n_MB_OK:
		{
			mpC_BT_OK->SetWndRepositionR( al_width / 2 - 29, al_height - 32 );
			
			mpC_BT_CANCEL->SetVisible( false );
		}
		break;
	default:
		{
			mpC_BT_OK->SetWndRepositionR( temp_x, al_height - 32 );
			mpC_BT_CANCEL->SetWndRepositionR( temp_x + 58 + 15, al_height - 32  );

			mpC_BT_CANCEL->SetVisible( true );
		}
		break;
	}
	
	mpc_TextBox->SetFontR( n_fRsvMessageBox );
	mpc_TextBox->SetTextColor( 225, 225, 255 );
	
	/*
	int ll_half_x = al_width / 2;
	int ll_mag = ll_half_x / 8;
	int temp_x = ( ll_half_x - 58 ) /2;

	mpC_framewnd->SetClientSize( al_width, al_height );
	
	mpC_BT_OK->SetWndRepositionR( temp_x + ll_mag , al_height - 35 );
	mpC_BT_CANCEL->SetWndRepositionR( temp_x + ll_half_x - ll_mag , al_height - 35  );
	*/

}
///---------------------------------------------------------------------------
///-- MessageBox
///---------------------------------------------------------------------------
///-- MessageBox 를 Open 한다.
///-- 지정된 스트링 너비만큼 textbox 사이즈를 설정한다.
void CMessageBox::OpenMessageBox( N_MESSAGEBOX_INPUT_STATE ae_instate, void *papt_str, const int al_width, const int al_height, void * apt_title_D, NMB_BtnType aBtnType )
{	 
	me_instate = ae_instate;

	m_BtnType = aBtnType;

	mpc_TextBox->SetText( papt_str);
	_SetMessageBoxSize( al_width, al_height );	
	
	nRui->SetModal( WID_MessageBox );
	nRui->SendEvent( WID_MessageBox, n_emSetVisible, (EPARAM)true, 0, 0, 0 );

	if( apt_title_D )
	{
		mpC_framewnd->SetCaption( apt_title_D );
	}
	else
	{
		mpC_framewnd->SetCaption( G_STRING_CHAR( IDS_WND_ALARM ) ); 
	}
}
///---------------------------------------------------------------------------
///-- SetParam
///---------------------------------------------------------------------------
void CMessageBox::SetParam( const DWORD a32_param1, const DWORD a32_param2 , const DWORD a32_param3 )
{
	m32_param[0] = a32_param1;
	m32_param[1] = a32_param2;
	m32_param[2] = a32_param3;
}

void CMessageBox::SetStrParam( char* aParam1, char* aParam2, char* aParam3 )
{
	if( aParam1 )
	{
		SAFE_STR_CPY( mStr_param[0], aParam1, 255 );
	}

	if( aParam2 )
	{
		SAFE_STR_CPY( mStr_param[1], aParam2, 255 );
	}

	if( aParam3 )
	{
		SAFE_STR_CPY( mStr_param[2], aParam3, 255 );
	}
}


///---------------------------------------------------------------------------
///-- Proc
///---------------------------------------------------------------------------
void CMessageBox::Proc( SEventMessage* EMsg )
{
	static int x =200,y = 90;

	///-- 확인 버튼
	if( EMsg->FocusWnd == mpC_BT_OK->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{		
			//x += 4;
			//y += 4;
			//OpenMessageBox( n_MESSAGEBOX_INVENTORY_ITEMDROP, "sdfsdf테스트 입니다",x,y );
			nRui->SetModal( WID_None );
			nRui->SendEvent( WID_MessageBox, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
			OkClose();
		}
	}
	///-- 취소 버튼
	else if( EMsg->FocusWnd == mpC_BT_CANCEL->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			//x -= 4;
			//y -= 4;
			//OpenMessageBox( n_MESSAGEBOX_INVENTORY_ITEMDROP, "sdfsdf테스트 입니다\n두번째 줄이고요,",x,y );
			nRui->SetModal( WID_None );
			nRui->SendEvent( WID_MessageBox, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
		}
	}
	/*
	switch( EMsg->FocusWnd )
	{
	case i:
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			nRui->SendEvent( WID_MessageBox, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
		}
		break;
	case WID_NumberInput_OkBtn:
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			OkClose();
		}
		break;	
	case WID_NumberInput_CancelBtn:
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			CancelClose();	
		}
	default:
		break;
	}*/
}


///---------------------------------------------------------------------------
///-- OkClose
///---------------------------------------------------------------------------
void CMessageBox::OkClose()
{
	
	switch( me_instate )
	{
	case n_MESSAGEBOX_INVENTORY_ITEMDROP:
		{			
			if( m32_param[1] )// 버리면서 파괴한다
			{
				g_RockClient.Send_RpCsDrop( m32_param[0] , m32_param[1], n_InvenTab );
				m32_param[1] = 0;
			}
			else// 단순 버린다
			{
				g_RockClient.Send_RpCsDrop( m32_param[0] , 1, n_InvenTab );
			}
		}
		break;
	case n_MESSAGEBOX_INVENTORY_W_ITEMDROP:
		{			
			g_RockClient.Send_RpCsDrop( m32_param[0] , 1, n_WearTab );			
		}
		break;
	case n_MESSAGEBOX_INVENTORY_R_ITEMDROP:
		{
			g_RockClient.Send_RpCsDrop( m32_param[0] , 1, n_RidingTab );
		}
		break;
	case n_MESSAGEBOX_FRONTIER_SECESSION:
		{			
#ifdef FRONTIER		
			g_RockClient.Send_CsMsngrLeaveFrontier( mStr_param[0] );
#endif
		}
		break;
	case n_MESSAGEBOX_FRONTIER_KICK:
		{			
#ifdef FRONTIER		
			g_RockClient.Send_CsMsngrLeaveFrontier( mStr_param[0] );
#endif
		}
		break;
	case n_MESSAGEBOX_SEPERATE_COMBINE_FRON:
		{
#ifdef 	COLONY_UPDATE
			g_RockClient.Send_CsMsngrSeperateF();
#endif
		}
		break;
	case n_MESSAGEBOX_CHANGE_ELDA_COLOR:
		{
			g_RockClient.Send_CsChangeEldaStone( m32_param[0], m32_param[1], m32_param[2] );		
		}
		break;
	case n_MESSAGEBOX_CHANGE_GUILDMASTER:
		{
#ifdef DECO_RENEWAL_GUILD_MJH
			g_RockClient.Send_CsChangeFrontierMaster( mStr_param[0] ); // 길드 마스터 변경
#endif // DECO_RENEWAL_GUILD_MJH
		}
		break;
	case n_MESSAGEBOX_CHANGE_NOTICE:
		{
#ifdef DECO_RENEWAL_GUILD_MJH // 공지사항 변경
			g_RockClient.Send_CsChangeFrontierNotice( mStr_param[0] , nRui->GetPcInvenInfo()->FrontierMyInfo->FrontierID );
#endif // DECO_RENEWAL_GUILD_MJH
		}
		break;
	case n_MESSAGEBOX_CHANGE_MYINFO:
		{
#ifdef DECO_RENEWAL_GUILD_MJH // 자기소개란 변경 
			g_RockClient.Send_CsChangeFrontierMemberIntro( mStr_param[0] );
#endif // DECO_RENEWAL_GUILD_MJH
		}
		break;
	default:
		break;
	}

	return;
}

/*
//-----------------------------------------------------------------------------
void CMessageBox::Update()
{
}


void CMessageBox::Open( N_NUMBER_INPUT_STATE aInputState, SPcItem* pPcItem )
{
	m_InputState = aInputState;
	m_PcItem = pPcItem;

	_tcscpy( gNumber, "" );
	
	nRui->SendEvent( WID_NumberInput_Edit, n_emSetFocus, 0, 0, 0, 0 );
	nRui->SendEvent( WID_NumberInput_Edit, n_emSetText, (EPARAM)gNumber, 255, 255, 255 );
	nRui->SendEvent( WID_NumberInput_Wnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
	nRui->SetModal( WID_NumberInput_Wnd );
	
	return;
}
/*
//-----------------------------------------------------------------------------	
void CMessageBox::Open( CWndProc* _link_wnd, SPcItem* _pc_item, BYTE _item_tab )
{
	m_LinkWnd = _link_wnd;
	m_ItemTab	= _item_tab;

	m_NIProc	= n_ProcNone;
	m_PcItem	= _pc_item;

	_tcscpy( gNumber, "" );

	nRui->SendEvent( WID_NumberInput_Edit, n_emSetText, (EPARAM)gNumber, 255, 255, 255 );
	nRui->SendEvent( WID_NumberInput_Edit, n_emSetFocus, 0, 0, 0, 0 );
	nRui->SendEvent( WID_NumberInput_Wnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
	nRui->SetModal( WID_NumberInput_Wnd );
	//m_EditBox->SetEditFocus();//직접 하는건 왜 안될까?...기억이...-.-
}

//-----------------------------------------------------------------------------	
void CMessageBox::Open( N_NUMBER_INPUT_PROC _niproc, SPcItem* _pc_item )
{
	m_LinkWnd = NULL;
	m_NIProc	= _niproc;
	m_PcItem	= _pc_item;

	_tcscpy( gNumber, "" );

	nRui->SendEvent( WID_NumberInput_Edit, n_emSetText, (EPARAM)gNumber, 255, 255, 255 );
	nRui->SendEvent( WID_NumberInput_Edit, n_emSetFocus, 0, 0, 0, 0 );
	nRui->SendEvent( WID_NumberInput_Wnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
	nRui->SetModal( WID_NumberInput_Wnd );
}

//-----------------------------------------------------------------------------	
void CMessageBox::CancelClose()
{
	//m_EditBox->EditDisable();
	nRui->SendEvent( WID_NumberInput_Edit, n_emEditDisable, 0, 0, 0, 0 );
	nRui->SendEvent( WID_NumberInput_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	nRui->SetModal( WID_None );

	if( ChatBoxWnd.IsVisible == true )
	{
		nRui->SendEvent( WID_ChatBox_MessageEdit, n_emSetFocus, 0, 0, 0, 0 );
	}

	return;
}

//-----------------------------------------------------------------------------	
*/