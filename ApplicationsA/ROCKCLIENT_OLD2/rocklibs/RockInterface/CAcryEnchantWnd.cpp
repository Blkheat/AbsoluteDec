#include "..\\RockPCH.h"

#include "Rui.h"
#include "..\\..\\brother.h"

#include "Define.h"
#include "Util.h"
#include "Render.h"
#include "Fontman.h"
#include "Resource.h"
#include "Chain.h"
#include "Container.h"

#include "..\\..\\global.h"
#include "..\\..\\RockClient.h"

#include "StringManager.h"

#include "CAcryEnchantWnd.h"
#include "MessageWndProc.h"
#include "CAcryEnchantRWnd.h"
#include "..\\..\\StringManager.h"
#include "SkillLevelUpWnd.h"
CAcryEnchantWnd	* g_pAcryEnchantWnd = NULL;

CAcryEnchantWnd::CAcryEnchantWnd()
{
	m_pAFrameWnd = NULL;
	m_pAccessarySlot = NULL;
	m_pEnScrollSlot = NULL;

	m_pAddtypeComBox = NULL;
	m_pNowAccAddtype = NULL;
	m_pNeedScrollNum = NULL;

	memset( &m_AccessaryItem, 0 , sizeof(SPcItem) );
	memset( &m_EnScrollItem, 0 , sizeof(SPcItem) );

	memset( m_strTempString , 0 , sizeof(m_strTempString) );

	m_pCloseBtn = NULL;
	m_pStartBtn = NULL;
	m_pCancleBtn = NULL;

	m_IsButtonLock = TRUE;
	
	m_nNowAddAttriType = nAddAttriNone;
	m_nNeedScrollNum = 0;
}

CAcryEnchantWnd::~CAcryEnchantWnd()
{
	

}

UINT CAcryEnchantWnd::GetAddAttriType()
{
	return m_nNowAddAttriType;
}

void CAcryEnchantWnd::SetAddAttriType( UINT uAttriType )
{
	m_nNowAddAttriType = uAttriType;
}

void CAcryEnchantWnd::Init()
{


}

void CAcryEnchantWnd::Open()
{
	m_IsButtonLock = FALSE;	

	m_AccessaryItem.Clear();
	m_EnScrollItem.Clear();
	m_pStartBtn->SetBtnHold( true );
	m_nNeedScrollNum = 0;

	m_nNowAddAttriType = nAddAttriNone;
	m_pAddtypeComBox->SetCurSelIdx( (const int)m_nNowAddAttriType );	
	m_pNowAccAddtype->SetText( G_STRING_CHAR(IDS_NONE) );										
	Rsprintf( RWCHAR(m_strTempString), G_STRING_CHAR(IDS_NOW_NUM) , m_nNeedScrollNum );						
	m_pNeedScrollNum->SetText( m_strTempString );

	m_pAddtypeComBox->SetFocusCheck( true );

	nRui->SendEvent( m_pAFrameWnd->GetID(), n_emSetVisible, (EPARAM)true, 0, 0, 0 );
}

void CAcryEnchantWnd::Close()
{
	nRui->SendEvent( m_pAFrameWnd->GetID(), n_emSetVisible, (EPARAM)false, 0, 0, 0 );
}

void CAcryEnchantWnd::RePosition( int nXPos, int nYPos )
{
	m_pAFrameWnd->Reposition( nXPos, nYPos );
}

void CAcryEnchantWnd::Composition()
{
	CButton*		btn = NULL;
	CTextBox*		tbox = NULL;
	CImageBox*		img = NULL;
	CImageBox9*		black_img9 = NULL;
	
	int center_x	= nRui->Desktop.Width / 2;
	int center_y	= nRui->Desktop.Height / 2;

	SRect enchantwnd;
	enchantwnd.w	= 254;
	enchantwnd.h	= 350;
	enchantwnd.x	= center_x - ( enchantwnd.w / 2 );
	enchantwnd.y	= 349;
	
	m_pAFrameWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_None, WID_None, 
												enchantwnd.x, enchantwnd.y, enchantwnd.w, enchantwnd.h, false );	
	m_pAFrameWnd->SetWndTexStyle( fwfTex_Style_Tile9 );
	m_pAFrameWnd->SetClientImage( TID_CTRL_WND_L1 );
	m_pAFrameWnd->SetIconTID( TID_CTRL_ICON_INVEN );	
	m_pAFrameWnd->SetWndTileSizeLR( 40, 29 );
	m_pAFrameWnd->SetWndTileSizeTB( 31, 11 );
	m_pAFrameWnd->SetCaption( G_STRING_CHAR(IDS_ENCHANT_ACCESARY) );										
	m_pAFrameWnd->SetTitle( true );	
	m_pAFrameWnd->SetWndProc( this );
	m_pAFrameWnd->SetVisible( false );
	m_pAFrameWnd->Initialize();
	//m_pAFrameWnd->SetZorder( n_zoTop );

	///-- 닫기
	m_pCloseBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, m_pAFrameWnd->GetID(), 
									 235, 6, 13, 13, false );
	m_pCloseBtn->SetBtnImage( TID_CTRL_BTN_X, TID_CTRL_BTN_X_OVR, TID_CTRL_BTN_X_CLK, TID_None );
	

	///-- 장신구( Static Text )
	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pAFrameWnd->GetID(), 38, 36, 80, 17, false );
	tbox->SetFontR( n_fRsvFontWndStatic );
	tbox->SetText( G_STRING_CHAR(IDS_ACCESARY) );	///--  HAN_MSG

	///-- 주문서( Static Text )
	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pAFrameWnd->GetID(), 137, 36, 80, 17, false );
	tbox->SetFontR( n_fRsvFontWndStatic );	
	tbox->SetText( G_STRING_CHAR(IDS_SCROLL) );	///--  HAN_MSG

	///-- Slot
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pAFrameWnd->GetID(), 60, 56, 35, 35, false );       
	img ->SetClientImage( TID_CTRL_SLOT_SUNKEN );

	///-- Slot
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pAFrameWnd->GetID(), 159, 56, 35, 35, false );            
	img ->SetClientImage( TID_CTRL_SLOT_SUNKEN );	
	
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pAFrameWnd->GetID(), 1, 236, 252, 55, false );
	img->SetClientImage( TID_CTRL_BACK );
	img->SetFocusCheck( false );

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pAFrameWnd->GetID(),  1, 236, 252, 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pAFrameWnd->GetID(), 1, 291, 252, 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );

	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pAFrameWnd->GetID(), 1, 251, 252, 17, false );
	tbox->SetFontR( n_fRsvFontWndStatic );	
	tbox->SetAlignText( n_atCenter, n_atTop );
	tbox->SetTextColor( 255, 50, 50 ); 
	tbox->SetMultiLine( true );
	tbox->SetLineInterval( 5 );
	tbox->SetMultiLineAsign(true);   
	tbox->SetText( G_STRING_CHAR(IDS_ACCESARYDESTROYWHENFAILADD) );	///--  HAN_MSG

	///-- 장신구 Slot
	m_pAccessarySlot = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, m_pAFrameWnd->GetID(), 
											 60, 56, 35, 35, false );	
	m_pAccessarySlot->SetSlotGroup( n_SGROUP_TUNING );
	m_pAccessarySlot->SetHID( HID_ITEMTIP );

	///-- 주문서
	m_pEnScrollSlot = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, m_pAFrameWnd->GetID(), 
											 159, 56, 35, 35, false );
	m_pEnScrollSlot->SetSlotGroup( n_SGROUP_TUNING );
	m_pEnScrollSlot->SetHID( HID_ITEMTIP );

	///-- None Item 지정
	m_pAccessarySlot->PcItem = &m_AccessaryItem;		
	m_pEnScrollSlot->PcItem = &m_EnScrollItem;


	///-- 주문서( Static Text )
	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pAFrameWnd->GetID(), 1, 106, 252, 17, false );
	tbox->SetFontR( n_fRsvFontWndStatic );
	tbox->SetText( G_STRING_CHAR(IDS_NOW_ACCESARYATTRI) );	///--  HAN_MSG	
	
	black_img9 = (CImageBox9*)nRui->CreateWnd( n_wtImageBox9, WID_None, m_pAFrameWnd->GetID(), 
								   51, 126, 154, 17, false );       	
	black_img9->SetWndTileSizeLR( 9, 9 );
	black_img9->SetWndTileSizeTB( 8, 8 );
	black_img9->SetFocusCheck( false );
	black_img9->SetClientImage( TID_CTRL_CONTBACK_L1 );
	black_img9->SetAdjustPosition( 0, 0 );
	
	
	m_pNowAccAddtype = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pAFrameWnd->GetID(), 
						0, 126, 250, 17, false );
	m_pNowAccAddtype->SetFontR( n_fRsvFontWndStatic );	
	m_pNowAccAddtype->SetTextColor( 255, 255, 255 );	
	m_pNowAccAddtype->SetText( G_STRING_CHAR(IDS_NONE) );	


	///-- 주문서( Static Text )
	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pAFrameWnd->GetID(), 1, 186, 252, 17, false );
	tbox->SetFontR( n_fRsvFontWndStatic );
	tbox->SetText( G_STRING_CHAR(IDS_NEED_SCROLLNUM) );	

	black_img9 = (CImageBox9*)nRui->CreateWnd( n_wtImageBox9, WID_None, m_pAFrameWnd->GetID(), 
								    51, 206, 154, 17, false );       	
	black_img9->SetWndTileSizeLR( 9, 9 );
	black_img9->SetWndTileSizeTB( 8, 8 );
	black_img9->SetFocusCheck( false );
	black_img9->SetClientImage( TID_CTRL_CONTBACK_L1 );
	black_img9->SetAdjustPosition( 0, 0 );

	m_pNeedScrollNum = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pAFrameWnd->GetID(), 
						0, 206, 250, 17, false );
	m_pNeedScrollNum->SetFontR( n_fRsvFontWndStatic );
	m_pNeedScrollNum->SetTextColor( 255, 255, 255 );
	m_pNeedScrollNum->SetText( "" );	

	///-- 주문서( Static Text )
	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pAFrameWnd->GetID(), 1, 146, 252, 17, false );
	tbox->SetFontR( n_fRsvFontWndStatic );	
	tbox->SetText( G_STRING_CHAR(IDS_APPLY_ADDATTRITYPE) );

	black_img9 = (CImageBox9*)nRui->CreateWnd( n_wtImageBox9, WID_None, m_pAFrameWnd->GetID(), 
								    51, 166, 154, 17, false );       	
	black_img9->SetWndTileSizeLR( 9, 9 );
	black_img9->SetWndTileSizeTB( 8, 8 );
	black_img9->SetFocusCheck( false );
	black_img9->SetClientImage( TID_CTRL_CONTBACK_L1 );
	black_img9->SetAdjustPosition( 0, 0 );

	m_pAddtypeComBox = (CComboBox*)nRui->CreateWnd( n_wtComboBox, WID_None, 
										 m_pAFrameWnd->GetID(), 51, 166, 154, 66, false );			
	m_pAddtypeComBox->SetListboxTileSizeTB( 5, 5 );
	m_pAddtypeComBox->SetListboxTileSizeLR( 5, 5 );
	m_pAddtypeComBox->GetTextBox()->SetTextColor( 255, 255, 255 );
	m_pAddtypeComBox->GetTextBox()->SetAlignText( n_atCenter, n_atCenter );
	m_pAddtypeComBox->SetListboxImage9( TID_CTRL_OUTLINE_L1 );
	m_pAddtypeComBox->AddItem( G_STRING_CHAR(IDS_NONE) , nAddAttriNone );					
	m_pAddtypeComBox->AddItem( G_STRING_CHAR(IDS_RES_OF_PARALYSIS) , nAddAttriParalyRes );	
	m_pAddtypeComBox->AddItem( G_STRING_CHAR(IDS_RES_OF_DISPEL) , nAddAttriDispelRes );		
	m_pAddtypeComBox->SetSelectText( G_STRING_CHAR(IDS_NONE) );							
	///-- m_pAddtypeComBox->SetFocusCheck( false );

	m_pStartBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, m_pAFrameWnd->GetID(), 
								  56, 306, 58, 25, false );
	m_pStartBtn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_CTRL_BTN58_DIS );
	m_pStartBtn->SetFontR( n_fRsvFontWndButton ); 
	m_pStartBtn->SetText( G_STRING_CHAR( IDS_WND_COM_START ) );								
	m_pStartBtn->SetBtnHold( true );

	///-- 중단
	m_pCancleBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, m_pAFrameWnd->GetID(), 
								  144, 306, 58, 25, false );
	m_pCancleBtn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_None );	
	m_pCancleBtn->SetFontR( n_fRsvFontWndButton ); 
	m_pCancleBtn->SetText( G_STRING_CHAR( IDS_WND_COM_CANCEL ) );
	
}

void CAcryEnchantWnd::Update()
{


}

void CAcryEnchantWnd::Proc( SEventMessage* EMsg )
{
	if( EMsg->FocusWnd == m_pCloseBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			Close();
		}
	}	
	
	if( EMsg->FocusWnd == m_pStartBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			if( m_nNowAddAttriType == nAddAttriNone )
			{
				MessageBoxWnd.Open( G_STRING_CHAR( IDS_MESSAGE_SELECT_APPLADDATTRITYPE ), 1200, NULL );	
				return;
			}			
			
			if( !m_IsButtonLock )
			{
				DWORD aAddAccUID = m_pAccessarySlot->PcItem->ItemUID;				
				DWORD aAddScrollUID = m_pEnScrollSlot->PcItem->ItemUID;

				BYTE aAddType = 0;
				if( m_nNowAddAttriType == nAddAttriParalyRes )
				{	
					aAddType = 12;
				}
				else if( m_nNowAddAttriType == nAddAttriDispelRes )
				{
					aAddType = 13;
				}
				
				g_RockClient.Send_CsMakeAddAccessory( aAddAccUID, aAddType, aAddScrollUID );
				m_IsButtonLock = TRUE;
				Close();
			}
		}
	}

	if( EMsg->FocusWnd == m_pCancleBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			Close();
		}
	}

	if( EMsg->FocusWnd == m_pAddtypeComBox->GetID() )
	{
		if( EMsg->Notify == NM_CB_TEXT_EXCHANGE )	
		{
			UINT uAddType = m_pAddtypeComBox->GetCurSelValue();
			if( uAddType != m_nNowAddAttriType )
			{
				m_nNowAddAttriType = uAddType;				
			}
		}
	}

	if( EMsg->FocusWnd == m_pAccessarySlot->GetID() ||
		EMsg->FocusWnd == m_pEnScrollSlot->GetID() )
	{
		if( EMsg->Notify == NM_SLOT_CLICK )			
		{	
			ItemSlotClick( EMsg );		
		}
		else if( EMsg->Notify == NM_SLOT_DBCLICK )	
		{	
			ItemSlotDBClick( EMsg );	
		}
		
		///-- 두 Slot 에 모두 Item 이 있다면 시작 준비
		if( m_pAccessarySlot->PcItem->IType != n_ITYPE_NONE && 
			m_pEnScrollSlot->PcItem->IType != n_ITYPE_NONE )
		{
			m_pStartBtn->SetBtnHold( false );		
		}
		else
		{
			m_pStartBtn->SetBtnHold( true );
		}		
	}
}

void CAcryEnchantWnd::ItemSlotClick( SEventMessage* EMsg )
{
	CSlot*		targetslot	= NULL;//현재 장비 슬롯
	CSlot*		srcslot		= NULL;//선택된(pick) 인벤 슬롯
	SPcItem*	tmpPcItem	= NULL;
	int			position	= 0;

	SPcItem		tempSItem;

	///-- 아이템을 들고 있을 경우
	if( nRui->GetCursorItem() != NULL )
	{
		targetslot = (CSlot*)Container->Get( (WNDID)EMsg->FocusWnd );
		srcslot = nRui->GetItemSelectedSlot();

		if( srcslot->PcItem == NULL )
		{
			return;
		}

		if( srcslot->PcItem->ItemTable == NULL )
		{
			return;
		}

		if( targetslot->PcItem && ( targetslot->PcItem == srcslot->PcItem ) )
		{
			nRui->ResetCursorItem();
		}

		///-------------------------------------------------------------------
		///-- Item Slot
		if( targetslot == m_pAccessarySlot )
		{
			if(  srcslot->Group == n_SGROUP_INVENTORY   )
			{
				///-- 이쪽에서 장신구가 아닌 아이템은 가공할수 없다고 나오게 함..
				BYTE aClass = srcslot->PcItem->ItemTable->theClass;
				if( aClass != n_Class_Accessory )
				{
					SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_ENCHANT_ONLYACCESARY ) );	
					return;
				}		
				
				if( targetslot->PcItem )
				{
					tempSItem = *(targetslot->PcItem);
				}				
				
				///-- Copy item
				targetslot->SetSlotPcItem( *(srcslot->PcItem) );								
				///-- Del Cursor
				//targetslot->PcItem = srcslot->PcItem;			

				if( !SetAEnchantText() )
				{
					targetslot->SetSlotPcItem( tempSItem );	
					SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_CANNOTENCHANT_MORE ) );	
					return;
				}

				nRui->ResetCursorItem();

				m_pEnScrollSlot->PcItem->Clear();
				m_pStartBtn->SetBtnHold( true );							
			}
			else if( srcslot->Group == n_SGROUP_WEAR )
			{				
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_ENCHANT_WEAR ) );			
			}
		}
		else if( targetslot == m_pEnScrollSlot )
		{
			if( srcslot->Group == n_SGROUP_INVENTORY  )
			{	
				///-- 가공할 장신구 아이템이 먼저 올라가 있지 않으면 리턴시킨다.
				if( m_pAccessarySlot->PcItem->IType == n_ITYPE_NONE )
				{
					SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_FIRSTUPADDACCESSARY ) );
					return;
				}

				///-- 주문서 아이템이 아닐경우에는 리턴시킨다.
				if( srcslot->PcItem->ItemTable->theType != n_Type_Scroll || 
					srcslot->PcItem->ItemTable->theReHP != n_Scroll_AddAccesory )
				{
					SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_FIRSTUPADDSCROLL )  );
					return;
				}				
				
				//총 개수가 필요한 개수 보다 크면 
				if( srcslot->PcItem->Amount >=  m_nNeedScrollNum )
				{
					targetslot->SetSlotPcItem( *(srcslot->PcItem) );				
					targetslot->PcItem->Amount = m_nNeedScrollNum;						
				}
				else
				{			
					SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOT_ENOUGH_VOLUME ) );
				}			

				///-- Del Cursor
				nRui->ResetCursorItem();
			}
		}	
	}
}

void CAcryEnchantWnd::ItemSlotDBClick( SEventMessage* EMsg )
{
	CSlot*		tslot	= NULL;	
	CSlot*		sslot	= NULL;
	
	tslot = (CSlot*)Container->Get( (WNDID)EMsg->FocusWnd );				
	sslot = nRui->GetItemSelectedSlot();	
	
	if( tslot->PcItem )
	{
		tslot->PcItem->Clear();	
		if( tslot == m_pAccessarySlot )
		{
			m_nNowAddAttriType = nAddAttriNone;
			m_pAddtypeComBox->SetCurSelIdx( (const int)m_nNowAddAttriType );
			m_nNeedScrollNum = 0;
			m_pNowAccAddtype->SetText( G_STRING_CHAR(IDS_NONE) );		
			Rsprintf( RWCHAR(m_strTempString), G_STRING_CHAR(IDS_NOW_NUM) , m_nNeedScrollNum );				
			m_pNeedScrollNum->SetText( m_strTempString );

			m_pEnScrollSlot->PcItem->Clear();
			m_pStartBtn->SetBtnHold( true );
		}
	}
}

BOOL CAcryEnchantWnd::CheckAEnchant_ItemList()
{
	int i = 0;
	int j = 0;
	
	bool lb_isFind_item = FALSE;
	bool lb_isFind_Scroll = FALSE;	
	
	if( !m_pAccessarySlot->PcItem && !m_pEnScrollSlot->PcItem ) 
	{
		return FALSE;
	}

	///-----------------------------------------------------------------------
	///-- 전체 인벤토리 Access
	for( j = 0; j < c_MaxInvenItemNum; ++j )
	{	
		if( nRui->GetPcInvenInfo()->InvenItem[j] == NULL )
		{
			continue;
		}
		
		///-- item slot
		if( m_pAccessarySlot->PcItem && 
			m_pAccessarySlot->PcItem->ItemUID == nRui->GetPcInvenInfo()->InvenItem[j]->ItemUID )
		{
			lb_isFind_item = true;			
		}

		///-- Scroll slot
		if( m_pEnScrollSlot->PcItem && 
			m_pEnScrollSlot->PcItem->ItemUID == nRui->GetPcInvenInfo()->InvenItem[j]->ItemUID )
		{
			lb_isFind_Scroll = true;
		}
	}

	///-----------------------------------------------------------------------
	///-- item 을 찾지 못 했을 경우
	if( !lb_isFind_item )
	{
		m_pAccessarySlot->PcItem->Clear();
		m_nNowAddAttriType = nAddAttriNone;
		m_pAddtypeComBox->SetCurSelIdx( (const int)m_nNowAddAttriType );
		m_nNeedScrollNum = 0;
		m_pNowAccAddtype->SetText( G_STRING_CHAR(IDS_NONE) );		
		Rsprintf( RWCHAR(m_strTempString), G_STRING_CHAR(IDS_NOW_NUM) , m_nNeedScrollNum );
		m_pNeedScrollNum->SetText( m_strTempString );

		m_pEnScrollSlot->PcItem->Clear();
	}
	///-- Scroll 를 찾지 못 했을 경우
	if( !lb_isFind_Scroll )
	{
		m_pEnScrollSlot->PcItem->Clear();
	}
	
	///-----------------------------------------------------------------------
	///-- 두 Slot 에 모두 Item 이 있다면 시작 준비
	if( m_pAccessarySlot->PcItem->IType != n_ITYPE_NONE && 
		m_pEnScrollSlot->PcItem->IType != n_ITYPE_NONE )
	{
		m_pStartBtn->SetBtnHold( false );				
	}
	else
	{
		m_pStartBtn->SetBtnHold( true );
	}	
	
	return TRUE;
}

BOOL CAcryEnchantWnd::SetAEnchantText()
{
	///-- 장신구일 경우에는 현재 장신구에 적용되어 있는
	///-- 주문서의 종류를 파악해서..
	///-- m_pNowAccAddtype <-- 현재 장신구의 부가속성 찍어주고..
	///-- m_pAddtypeComBox <-- 부가 속성타입 설정해주고..
	///-- 이미 부가속성이 적용되어있는 아이템일 경우
	///-- m_pAddtypeComBox을 비활성화 시켜준다.
	///-- m_pNeedScrollNum 필요 주문서 개수도 찍어준다.
	///-- m_nNeedScrollNum <-- 이것도 설정

	SItemBaseInfo* item_Info = NULL;
	
	WORD aScrollCode = 0;
	aScrollCode =  m_pAccessarySlot->PcItem->WearTable.theEldaStone;

	BYTE aSkillLevel = 0;

	if( aScrollCode )
	{
		m_nNowAddAttriType = nAddAttriNone;
		
		item_Info = g_Pc_Manager.GetItemBaseInfoPtr( aScrollCode );

		if( item_Info )
		{
			WORD aOwnSkillCode = 0;
			aOwnSkillCode = item_Info->theOwnSkillCode;				
			if( aOwnSkillCode )
			{
				SSkillBaseInfo* skill_Info = NULL;
				skill_Info = g_Pc_Manager.GetSkillBaseInfoPtr( aOwnSkillCode ); 

				if( skill_Info )
				{
					if( skill_Info->theAttackAttri == 12 )
					{
						m_nNowAddAttriType = nAddAttriParalyRes;
					}
					else if( skill_Info->theAttackAttri == 13 )
					{
						m_nNowAddAttriType = nAddAttriDispelRes;
					}

					///-- 스킬 숙련도
					
					aSkillLevel =  g_SkillLevelUpWnd.GetActiveSKillLevel(skill_Info);
					BYTE aMaxLevel = skill_Info->theMaxLevel;
					
					if( aSkillLevel )
					{
						if( aSkillLevel >= aMaxLevel )
						{
							return FALSE;
						}
					}
					
					m_nNeedScrollNum = aSkillLevel + 1;				
				}
			}
		}
	}
	else
	{
		m_nNowAddAttriType = nAddAttriNone;
	}

	m_pAddtypeComBox->SetCurSelIdx( (const int)m_nNowAddAttriType );

	if( m_nNowAddAttriType == nAddAttriNone )
	{
		m_pAddtypeComBox->SetFocusCheck( true );
		m_nNeedScrollNum = 1;
		m_pNowAccAddtype->SetText( G_STRING_CHAR(IDS_NONE) );		
	}
	else
	{
		m_pAddtypeComBox->SetFocusCheck( false );
		
		if( m_nNowAddAttriType == nAddAttriParalyRes )
		{
			Rsprintf( RWCHAR(m_strTempString), G_STRING_CHAR(IDS_RES_OF_PARALYSIS_PER), aSkillLevel );		
		}
		else
		{
			Rsprintf( RWCHAR(m_strTempString), G_STRING_CHAR(IDS_RES_OF_DISPEL_PER), aSkillLevel );		
		}

		m_pNowAccAddtype->SetText( m_strTempString );
	}

	Rsprintf( RWCHAR(m_strTempString), G_STRING_CHAR(IDS_NOW_NUM) , m_nNeedScrollNum );					
	m_pNeedScrollNum->SetText( m_strTempString );

	return TRUE;
}