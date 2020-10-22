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

#include "NumberInputWnd.h"
#include "ChatMainProc.h"

#include "StoreWndProc.h"
#include "ExchangeWndProc.h"
#include "WarehouseWndProc.h"
#include "GuildInventoryWndProc.h"
#include "ItemRemodelWndProc.h"
#include "ItemCreateWndProc.h"
#include "stringmanager.h"
#include "ProposeJoinFrontier.h"

#include "PetInventoryWnd.h"
#include "PetMainWndProc.h"
#include "InventoryWndProc.h"
#include "MessageWndProc.h"
#include "MessageBox.h"



CNumberInputWnd		g_NumberInputWnd;
CStrInputWnd		g_StrInputWnd;


TCHAR		gNumber[64] = {0,};
TCHAR		gStrInput[10] = {0,};
int			gBufLen( 64 );

//-----------------------------------------------------------------------------
void CNumberInputWnd::Init()
{
}

//-----------------------------------------------------------------------------
void CNumberInputWnd::Composition()
{
	CButton*		btn;
	CImageBox*      img;

	int center_x	= nRui->Desktop.Width / 2;
	int center_y	= nRui->Desktop.Height / 2;

	SRect number_input_wnd;
	number_input_wnd.w	= 204;
	number_input_wnd.h	= 132;
	number_input_wnd.x	= center_x - ( number_input_wnd.w / 2 );
	number_input_wnd.y	= center_y - ( number_input_wnd.h / 2 );
	
	///-- Frame Wnd
	m_FrameWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_NumberInput_Wnd, WID_None, number_input_wnd.x, number_input_wnd.y, number_input_wnd.w, number_input_wnd.h, false );
	m_FrameWnd->SetWndTexStyle( fwfTex_Style_Tile9 );
	m_FrameWnd->SetClientImage( TID_CTRL_WND_L1 );
	m_FrameWnd->SetIconTID( TID_CTRL_ICON_INVEN );	
	m_FrameWnd->SetWndTileSizeLR( 40, 29 );
	m_FrameWnd->SetWndTileSizeTB( 31, 11 );

	m_FrameWnd->SetCaption( G_STRING_CHAR( IDS_WND_NUMBERINPUT ));		
	m_FrameWnd->SetTitle( true );	
	m_FrameWnd->SetWndProc( this );
	m_FrameWnd->SetVisible( false );
	m_FrameWnd->SetZorder( n_zoTop );
	m_FrameWnd->Initialize();

	///---------------------------------------------------------------------------	
	///-- Static
	///---------------------------------------------------------------------------		
	///-- Line
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_NumberInput_Wnd, 1, 33, 202, 63, false );
	img->SetClientImage( TID_CTRL_BACK );
	img->SetFocusCheck( false );
	
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_NumberInput_Wnd,  1, 32, 202, 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );
	
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_NumberInput_Wnd, 1, 96, 202, 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );
		
	///-- Slot
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_NumberInput_Wnd, 36, 64, 132, 23, false );     
	img ->SetClientImage( TID_CTRL_IMG_ELDTEXT132 );
	
	///-- 원석 (Static)
	msgtbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_NumberInput_Wnd, 36, 40, 130, 17, false );
	msgtbox->SetFontR( n_fRsvFontWndStatic );
	msgtbox->SetText( G_STRING_CHAR( IDS_WND_INPUT_NUMBER ) ); 
	
	m_EditBox = (CEditBox*)nRui->CreateWnd( n_wtEditBox, WID_NumberInput_Edit, WID_NumberInput_Wnd, 51, 65, 94, 20, false );
	m_EditBox->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );

#ifdef UNICODE_ROCK 
	m_EditBox->SetTextSize( 20 );
#else 
	m_EditBox->SetTextSize( 16 );
#endif
	m_EditBox->SetClientImage( TID_None );
	m_EditBox->SetClientColor( 0, 0, 0 );
	m_EditBox->SetAlignText( n_atLeft, n_atCenter );
	m_EditBox->SetBlank( 2, 0, 0, 0 );
	m_EditBox->SetEditStyle( esText );
	m_EditBox->SetText( _RT("") );
	m_EditBox->SetEditStyle( esOnlyNumeric );
	m_EditBox->SetReturnActionClear( false );
	m_EditBox->InitEditBox();
	m_EditBox->SetHID( HID_None );
	
	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_NumberInput_OkBtn, WID_NumberInput_Wnd, 29, 101, 58, 25, false );    
	btn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_CTRL_BTN58_DIS );
	btn->SetFontR( n_fRsvFontWndButton );   
	btn->SetText( G_STRING_CHAR( IDS_WND_COM_OK ) ); 
	
	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_NumberInput_CancelBtn, WID_NumberInput_Wnd, 117, 101, 58, 25, false );    
	btn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_CTRL_BTN58_DIS );
	btn->SetFontR( n_fRsvFontWndButton );   
	btn->SetText( G_STRING_CHAR( IDS_WND_COM_CANCEL ) ); 

}

//-----------------------------------------------------------------------------
void CNumberInputWnd::Update()
{
}

//-----------------------------------------------------------------------------
void CNumberInputWnd::Proc( SEventMessage* EMsg )
{
	switch( EMsg->FocusWnd )
	{
	case WID_NumberInput_Edit:
		if( EMsg->Notify == NM_EDT_RETURN_ACTION )
		{
			OkClose();
			nRui->SetEnterFlag(n_nfEditEnter);
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
		break;
	}
}

///---------------------------------------------------------------------------
///-- Open
///---------------------------------------------------------------------------
void CNumberInputWnd::Open( N_NUMBER_INPUT_STATE aInputState, SPcItem* pPcItem )
{
	m_InputState = aInputState;
	m_PcItem = pPcItem;

	memset(gNumber , NULL, 64 );

	nRui->SendEvent( WID_NumberInput_Edit, n_emSetFocus, 0, 0, 0, 0 );
	nRui->SendEvent( WID_NumberInput_Edit, n_emSetText, (EPARAM)gNumber, 255, 255, 255 );
	nRui->SendEvent( WID_NumberInput_Wnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
	nRui->SetModal( WID_NumberInput_Wnd );

	//dongs 인벤에서 바닥으로 버리거나 교환 할떄 .. 자동으로 MAX 값이 나오게 ..
	if(m_PcItem != NULL)
	{
		TCHAR str[32]= {0,};	
		
		switch(m_InputState) 
		{
			case n_NUMBERINPUT_INVENTORY_ITEMDROP:	
#ifdef DECO_RENEWAL_GUILD_MJH
			case n_NUMBERINPUT_INVENTORY_GUILDINVEN:
			case n_NUMBERINPUT_GUILDINVEN_INVENTORY:
#endif // DECO_RENEWAL_GUILD_MJH
			case n_NUMBERINPUT_INVENTORY_SELLSTORE:	
			case n_NUMBERINPUT_INVENTORY_EXCHANGE:	
			case n_NUMBERINPUT_INVENTORY_WAREHOUSE:
			case n_NUMBERINPUT_WAREHOUSE_INVENTORY:
			case n_NUMBERINPUT_INVENTORY_PETINVEN:
			case n_NUMBERINPUT_PETINVEN_INVENTORY:
				{		
					Ritoa(m_PcItem->Amount,RWCHAR(str),10);
					m_EditBox->SetText( str );
					break;

				}
			case n_NUMBERINPUT_BUYSTORE_INVENTORY :
				{
					//살수있는 최대 겟수 
					//장비품은 들은 번들을 뺀다. 

					if( m_PcItem->IType != n_ITYPE_WEAR )
					{
						Ritoa((int)m_PcItem->ItemTable->theBundle ,RWCHAR(str),10);
					}
					else 
					{
						m_PcItem->ItemTable->theBundle = 1; 
						Ritoa((int)m_PcItem->ItemTable->theBundle ,RWCHAR(str),10);
					}
					m_EditBox->SetText( str );
					break;
				}
		}
	}
	
	return;
}














/*
//-----------------------------------------------------------------------------	
void CNumberInputWnd::Open( CWndProc* _link_wnd, SPcItem* _pc_item, BYTE _item_tab )
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
void CNumberInputWnd::Open( N_NUMBER_INPUT_PROC _niproc, SPcItem* _pc_item )
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
*/
//-----------------------------------------------------------------------------	
void CNumberInputWnd::OkClose()
{
	long num_item( 0 );

	nRui->SendEvent( WID_NumberInput_Edit, n_emGetText, (EPARAM)gNumber, gBufLen, 0, 0 );

	if( Rstrcmp(gNumber, _RT("")) == 0 )
		return;

	num_item = Ratol( RWCHAR(gNumber) );

	if( num_item == 0 )
		return;
		
	nRui->SendEvent( WID_NumberInput_Edit, n_emEditDisable, 0, 0, 0, 0 );
	nRui->SendEvent( WID_NumberInput_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	nRui->SetModal( WID_None );
	
	switch( m_InputState )
	{
	case n_NUMBERINPUT_INVENTORY_ITEMDROP:
		{			
			
#ifdef ITEM_LIMIT_FUNC				
			BYTE aItemLimitFunc = 0;

			if( m_PcItem->ItemTable )
			{
				aItemLimitFunc = m_PcItem->ItemTable->theItemLimitFunc;
			}

		#ifdef HHM_60000_ABOVE_ITEM_COMPOSITION_NOT_EXCHANGE_20090625
			
			if(m_PcItem->IsItemCode60000Above())
			{
				g_MessageBox.SetParam( m_PcItem->ItemUID, num_item );
				g_MessageBox.OpenMessageBox( n_MESSAGEBOX_INVENTORY_ITEMDROP, G_STRING_CHAR( IDS_SYSWHOLEDESTROYITEM ), 300, 100 );
			}
			else
			{
				if( CHECK_ITEMFUNC( aItemLimitFunc, n_ItemFunc_DropDestory ) )// 버리면서 파괴한다
				{
					g_MessageBox.SetParam( m_PcItem->ItemUID, num_item );
					g_MessageBox.OpenMessageBox( n_MESSAGEBOX_INVENTORY_ITEMDROP, G_STRING_CHAR( IDS_SYSWHOLEDESTROYITEM ), 300, 100 );
				}
				else// 단지 버린다
				{
					g_MessageBox.SetParam( m_PcItem->ItemUID, num_item );
					g_MessageBox.OpenMessageBox( n_MESSAGEBOX_INVENTORY_ITEMDROP, G_STRING_CHAR( IDS_MSGBOX_ASK_DROP ), 250, 100 );				
				}
			}

		#else

			if( CHECK_ITEMFUNC( aItemLimitFunc, n_ItemFunc_DropDestory ) )// 버리면서 파괴한다
			{
				g_MessageBox.SetParam( m_PcItem->ItemUID, num_item );
				g_MessageBox.OpenMessageBox( n_MESSAGEBOX_INVENTORY_ITEMDROP, G_STRING_CHAR( IDS_SYSWHOLEDESTROYITEM ), 300, 100 );
			}
			else// 단지 버린다
			{
				g_MessageBox.SetParam( m_PcItem->ItemUID, num_item );
				g_MessageBox.OpenMessageBox( n_MESSAGEBOX_INVENTORY_ITEMDROP, G_STRING_CHAR( IDS_MSGBOX_ASK_DROP ), 250, 100 );				
			}
			
		#endif// #ifdef HHM_60000_ABOVE_ITEM_COMPOSITION_NOT_EXCHANGE_20090625


		#ifdef HHM_12180_12188_ABOVE_ITEM_COMPOSITION_NOT_EXCHANGE_20090824

			if(m_PcItem->IsItemCode12180_12188_Above())
			{
				g_MessageBox.SetParam( m_PcItem->ItemUID, num_item );
				g_MessageBox.OpenMessageBox( n_MESSAGEBOX_INVENTORY_ITEMDROP, G_STRING_CHAR( IDS_SYSWHOLEDESTROYITEM ), 300, 100 );
			}
			else
			{
				if( CHECK_ITEMFUNC( aItemLimitFunc, n_ItemFunc_DropDestory ) )// 버리면서 파괴한다
				{
					g_MessageBox.SetParam( m_PcItem->ItemUID, num_item );
					g_MessageBox.OpenMessageBox( n_MESSAGEBOX_INVENTORY_ITEMDROP, G_STRING_CHAR( IDS_SYSWHOLEDESTROYITEM ), 300, 100 );
				}
				else// 단지 버린다
				{
					g_MessageBox.SetParam( m_PcItem->ItemUID, num_item );
					g_MessageBox.OpenMessageBox( n_MESSAGEBOX_INVENTORY_ITEMDROP, G_STRING_CHAR( IDS_MSGBOX_ASK_DROP ), 250, 100 );				
				}
			}
			
		#else
			
			if( CHECK_ITEMFUNC( aItemLimitFunc, n_ItemFunc_DropDestory ) )// 버리면서 파괴한다
			{
				g_MessageBox.SetParam( m_PcItem->ItemUID, num_item );
				g_MessageBox.OpenMessageBox( n_MESSAGEBOX_INVENTORY_ITEMDROP, G_STRING_CHAR( IDS_SYSWHOLEDESTROYITEM ), 300, 100 );
			}
			else// 단지 버린다
			{
				g_MessageBox.SetParam( m_PcItem->ItemUID, num_item );
				g_MessageBox.OpenMessageBox( n_MESSAGEBOX_INVENTORY_ITEMDROP, G_STRING_CHAR( IDS_MSGBOX_ASK_DROP ), 250, 100 );				
			}

		#endif// #ifdef HHM_12180_12188_ABOVE_ITEM_COMPOSITION_NOT_EXCHANGE_20090824

#else			
			g_RockClient.Send_RpCsDrop( m_PcItem->ItemUID, num_item, n_InvenTab );

#endif
			
			if( COutState_Act == g_ChatMainProc.GetOutState())
			{
				UINT uEid = g_ChatMainProc.m_ChatActPrintWnd.GetChatEditBox()->GetID();
				nRui->SendEvent( uEid, n_emSetFocus, 0, 0, 0, 0 );
			}	
		}
		break;
	case n_NUMBERINPUT_INVENTORY_Q_ITEMDROP:
		{
			g_RockClient.Send_RpCsDrop( m_PcItem->ItemUID, num_item, n_QuestTab );
			
			if( COutState_Act == g_ChatMainProc.GetOutState())
			{
				UINT uEid = g_ChatMainProc.m_ChatActPrintWnd.GetChatEditBox()->GetID();
				nRui->SendEvent( uEid, n_emSetFocus, 0, 0, 0, 0 );
			}	
		}
		break;
		
	case n_NUMBERINPUT_INVENTORY_SELLSTORE:
		{
			StoreSellWnd.CloseNumberInputWnd( m_PcItem, num_item, n_InvenTab );
		}
		break;
	case n_NUMBERINPUT_BUYSTORE_INVENTORY:
		{
			StoreBuyWnd.CloseNumberInputWnd( m_PcItem, num_item, n_InvenTab );
		}
		break;
	case n_NUMBERINPUT_INVENTORY_EXCHANGE:
		{
			g_ExchangeWnd.CloseNumberInputWnd( num_item );
		}
		break;
	case n_NUMBERINPUT_INVENTORY_WAREHOUSE:
		{
			g_WarehouseWnd.CloseNumberInputWnd( num_item );			
		}
		break;
	case n_NUMBERINPUT_WAREHOUSE_INVENTORY:
		{
			g_RockClient.Send_RpCsDraw( m_PcItem->ItemUID, num_item );
		}
		break;
#ifdef DECO_RENEWAL_GUILD_MJH
	case n_NUMBERINPUT_INVENTORY_GUILDINVEN:
		{
			g_GuildInvenWnd.CloseNumberInputWnd( num_item );			
		}
		break;

	case n_NUMBERINPUT_GUILDINVEN_INVENTORY:
		{
			g_RockClient.Send_RpCsDrawFrontier( m_PcItem->ItemUID, num_item );
		}
		break;
	case n_NUMBERINPUT_INVENTORY_PROPOSEFRON:
		{
			g_ProposeJoinFrontier.CloseNumberInputWnd( num_item );
		}
		break;
#endif // DECO_RENEWAL_GUILD_MJH
	case n_NUMBERINPUT_INVENTORY_TUNING:
		{
			g_ItemRemodel_WndProc.CloseNumberInputWnd( num_item );			
		}
		break;
	case n_NUMBERINPUT_INVENTORY_CREATE	:
		{
			g_ItemCreate_WndProc.CloseNumberInputWnd( num_item );
		}
		break;
	case n_NUMBERINPUT_INVENTORY_PETINVEN:
		{
			g_PetInventoryWnd.CloseNumberInputWnd(num_item);

		}
		break;
	case n_NUMBERINPUT_PETINVEN_INVENTORY:
		{
			InventoryWnd.CloseNumberInputWnd(num_item);

		}
		break;


	default:
		break;
	}

	return;
}

//-----------------------------------------------------------------------------	
void CNumberInputWnd::CancelClose()
{
	//m_EditBox->EditDisable();
	nRui->SendEvent( WID_NumberInput_Edit, n_emEditDisable, 0, 0, 0, 0 );
	nRui->SendEvent( WID_NumberInput_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	nRui->SetModal( WID_None );

	if( COutState_Act == g_ChatMainProc.GetOutState())
	{
		UINT uEid = g_ChatMainProc.m_ChatActPrintWnd.GetChatEditBox()->GetID();
		nRui->SendEvent( uEid, n_emSetFocus, 0, 0, 0, 0 );
	}

	return;
}

//-----------------------------------------------------------------------------	
//-----------------------------------------------------------------------------
void CStrInputWnd::Composition()
{
	CImageBox*      img;

	int center_x	= nRui->Desktop.Width / 2;
	int center_y	= nRui->Desktop.Height / 2;

	SRect number_input_wnd;
	number_input_wnd.w	= 204;
	number_input_wnd.h	= 132;
	number_input_wnd.x	= center_x - ( number_input_wnd.w / 2 );
	number_input_wnd.y	= center_y - ( number_input_wnd.h / 2 );
	
	///-- Frame Wnd
	m_FrameWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_None, WID_None, number_input_wnd.x, number_input_wnd.y, number_input_wnd.w, number_input_wnd.h, false );
	m_FrameWnd->SetWndTexStyle( fwfTex_Style_Tile9 );
	m_FrameWnd->SetClientImage( TID_CTRL_WND_L1 );
	m_FrameWnd->SetIconTID( TID_CTRL_ICON_INVEN );	
	m_FrameWnd->SetWndTileSizeLR( 40, 29 );
	m_FrameWnd->SetWndTileSizeTB( 31, 11 );

	m_FrameWnd->SetCaption( G_STRING_CHAR( IDS_PASS ) );		
	m_FrameWnd->SetTitle( true );	
	m_FrameWnd->SetWndProc( this );
	m_FrameWnd->SetVisible( false );
	m_FrameWnd->SetZorder( n_zoTop );
	m_FrameWnd->Initialize();

	///---------------------------------------------------------------------------	
	///-- Static
	///---------------------------------------------------------------------------		
	///-- Line
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_FrameWnd->GetID(), 1, 33, 202, 63, false );
	img->SetClientImage( TID_CTRL_BACK );
	img->SetFocusCheck( false );
	
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_FrameWnd->GetID(),  1, 32, 202, 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );
	
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_FrameWnd->GetID(), 1, 96, 202, 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );
	

	///-- Slot
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_FrameWnd->GetID(), 36, 64, 132, 23, false );     
	img ->SetClientImage( TID_CTRL_IMG_ELDTEXT132 );
	
	///-- 원석 (Static)
	msgtbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_FrameWnd->GetID(), 36, 40, 130, 17, false );
	msgtbox->SetFontR( n_fRsvFontWndStatic );
	msgtbox->SetText( G_STRING_CHAR( IDS_MSGBOX_INPUT_ROOMPASS ) ); 
	
	m_EditBox = (CEditBox*)nRui->CreateWnd( n_wtEditBox, WID_None, m_FrameWnd->GetID(), 51, 65, 94, 20, false );
	m_EditBox->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_EditBox->SetTextSize( 16 );
	m_EditBox->SetClientImage( TID_None );
	m_EditBox->SetClientColor( 0, 0, 0 );
	m_EditBox->SetAlignText( n_atLeft, n_atCenter );
	m_EditBox->SetBlank( 2, 0, 0, 0 );
	m_EditBox->SetEditStyle( esText );
	m_EditBox->SetText( _RT("") );
	m_EditBox->SetEditStyle( esPassword );
	m_EditBox->SetReturnActionClear( false );
	m_EditBox->InitEditBox();
	m_EditBox->SetHID( HID_None );
	
	button[0] = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, m_FrameWnd->GetID(), 29, 101, 58, 25, false );    
	button[0]->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_CTRL_BTN58_DIS );
	button[0]->SetFontR( n_fRsvFontWndButton );   
	button[0]->SetText( G_STRING_CHAR( IDS_WND_COM_OK ) ); 
	
	button[1]= (CButton*)nRui->CreateWnd( n_wtButton, WID_None, m_FrameWnd->GetID(), 117, 101, 58, 25, false );    
	button[1]->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_CTRL_BTN58_DIS );
	button[1]->SetFontR( n_fRsvFontWndButton );   
	button[1]->SetText( G_STRING_CHAR( IDS_WND_COM_CANCEL ) ); 

}


//-----------------------------------------------------------------------------
void CStrInputWnd::Proc( SEventMessage* EMsg )
{
	if( EMsg->FocusWnd == m_EditBox->GetID() )
	{
		if( EMsg->Notify == NM_EDT_RETURN_ACTION )
		{
			OkClose();
			nRui->SetEnterFlag(n_nfEditEnter);
		}
		
	}
	
	if( EMsg->FocusWnd == button[0]->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			OkClose();
		}
		
	}

	if( EMsg->FocusWnd == button[1]->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			CancelClose();
		}
		
	}
}

///---------------------------------------------------------------------------
///-- Open
///---------------------------------------------------------------------------
void CStrInputWnd::Open( N_STR_INPUT_STATE aInputState, DWORD aArg1 )
{
	m_InputState = aInputState;

	switch( aInputState )
	{
	case n_STRINPUT_BZ_PASSWORD:
		{
			m_FrameWnd->SetCaption( G_STRING_CHAR( IDS_PASS ) );		
			msgtbox->SetText( G_STRING_CHAR( IDS_MSGBOX_INPUT_ROOMPASS ) ); 
			m_Arg1 = aArg1;
			m_EditBox->ChangeEditStyle( esPassword );
			m_EditBox->SetTextSize( 16 );
		}
		break;
	case n_STRINPUT_FRONTIERPET_NAME:
		{			
			m_FrameWnd->SetCaption( G_STRING_CHAR( IDS_FRONTIERMAIN_CREATURE ) );		
			msgtbox->SetText( G_STRING_CHAR( IDS_MSGBOX_INPUT_FRONTIERPETNAME ) ); 
			m_Arg1 = aArg1;
			m_EditBox->ChangeEditStyle( esText );
			m_EditBox->SetTextSize( 16 );
		}
		break;
	default:
		break;
	}

	memset(gNumber , NULL , 64 );
		
	nRui->SendEvent( m_EditBox->GetID(), n_emSetFocus, 0, 0, 0, 0 );
	nRui->SendEvent( m_EditBox->GetID(), n_emSetText, (EPARAM)gStrInput, 255, 255, 255 );
	nRui->SendEvent( m_FrameWnd->GetID(), n_emSetVisible, (EPARAM)true, 0, 0, 0 );
	nRui->SetModal( m_FrameWnd->GetID() );
	
	return;
}

//-----------------------------------------------------------------------------	
void CStrInputWnd::OkClose()
{
	long num_item( 0 );

	nRui->SendEvent( m_EditBox->GetID(), n_emGetText, (EPARAM)gStrInput, m_EditBox->GetTextLimitLen() + 1, 0, 0 );

	if( Rstrcmp(gStrInput, _RT("")) == 0 )
		return;

	///num_item = atol( gStrInput );

	///if( num_item == 0 )
		///return;
		
	nRui->SendEvent( m_EditBox->GetID(), n_emEditDisable, 0, 0, 0, 0 );
	nRui->SendEvent( m_FrameWnd->GetID(), n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	nRui->SetModal( WID_None );

	switch( m_InputState )
	{
	case n_STRINPUT_BZ_PASSWORD:
		{
			g_RockClient.Send_CsJoinFightRoom( m_Arg1, gStrInput );			
		}
		break;
	case n_STRINPUT_FRONTIERPET_NAME:
		{
			if( gStrInput[0] == '\0' )
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_MSGBOX_INPUT_FRONTIERPETNAME ) );

				Open( m_InputState, m_Arg1 );
			}
			else
			{
				char strFilter[255] = {0,};

				SAFE_STR_CPY( strFilter, gStrInput, 255 );

				if(g_RockClient.m_SlangFilter.IsCheckSymbol(G_STRING_CHAR( IDS_SYSPR_PET_CREATE_NOT_WRITE ), RWCHAR(gStrInput) ))
				{
					MessageBoxWnd.Open( G_STRING_CHAR( IDS_SYSPR_PET_CREATE_NOT_WRITE ) , 1200 , NULL );
					return;
				}

				if( !g_RockClient.m_SlangFilter.IsValidString( gStrInput ) )
				{
					MessageBoxWnd.Open( G_STRING_CHAR( IDS_MSGBOX_NAME_SPECIAL_TEXT ) , 1500 , NULL );
					return;
				}		
				
				if( g_RockClient.m_SlangFilter.ISFindFilterName( gStrInput ) )
				{
					MessageBoxWnd.Open( G_STRING_CHAR( IDS_MSGBOX_NAME_INVALID_TEXT ) , 1500 , NULL );
					return;
				}

				if( g_RockClient.m_SlangFilter.Filter( strFilter ) )
				{
					Rsprintf( RWCHAR(strFilter), _RT("%s"), RWCHAR(g_RockClient.m_SlangFilter.GetString()) );
				}

				if( Rstrcmp( RWCHAR(gStrInput), RWCHAR(strFilter) ) == 0 )
				{
					g_RockClient.Send_CsCreateFrontierPet( m_Arg1, gStrInput );
				}
				else
				{
					SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_MSGBOX_NAME_SLANG_TEXT ) );
					Open( m_InputState, m_Arg1 );
				}
			}
		}
		break;
	default:
		break;
	}

	return;
}

//-----------------------------------------------------------------------------	
void CStrInputWnd::CancelClose()
{
	//m_EditBox->EditDisable();
	nRui->SendEvent( m_EditBox->GetID(), n_emEditDisable, 0, 0, 0, 0 );
	nRui->SendEvent( m_FrameWnd->GetID(), n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	nRui->SetModal( WID_None );

	switch( m_InputState )
	{
	case n_STRINPUT_BZ_PASSWORD:
		{
			if( COutState_Act == g_ChatMainProc.GetOutState())
			{
				UINT uEid = g_ChatMainProc.m_ChatActPrintWnd.GetChatEditBox()->GetID();
				nRui->SendEvent( uEid, n_emSetFocus, 0, 0, 0, 0 );
			}
		}
		break;
	case n_STRINPUT_FRONTIERPET_NAME:
		{
		}
		break;
	default:
		break;
	}

	return;
}
