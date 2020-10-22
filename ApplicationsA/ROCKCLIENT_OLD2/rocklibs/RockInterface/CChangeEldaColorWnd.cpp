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

#include "CChangeEldaColorWnd.h"
#include "MessageBox.h"
#include "MessageWndProc.h"
#include "MessageWndProc.h"
#include "..\\..\\StringManager.h"
#include "ItemEldaMixAWndProc.h"


CChangeEldaColorWnd	* g_pChangeEldaColorWnd = NULL;

CChangeEldaColorWnd::CChangeEldaColorWnd()
{
	m_pFrameWnd = NULL;			
	memset( &m_CPayItem, 0 , sizeof(SPcItem) );				

	m_pCEldaSlot = NULL;
	memset( &m_CEldaItem, 0 , sizeof(SPcItem) );
	
	memset( m_pCREldaCheckSlot , 0 , sizeof(m_pCREldaCheckSlot) );
	memset( m_CREldaCheckItem , 0 , sizeof(m_CREldaCheckItem) );
	memset( m_pCRSelectImg , 0 , sizeof(m_pCRSelectImg) );

	m_pCloseBtn = NULL;
	m_pOkBtn = NULL;

	m_pSelectColorTxt = NULL;
	
	m_nSelectIndex = -1;
	m_ItemID = 0;

	m_IsButtonLock = TRUE;

	m_nMinLevel = 0;
	m_nMaxLevel = 0;

	m_nNowEldaLevel = 0;
}

CChangeEldaColorWnd::~CChangeEldaColorWnd()
{

}
	
void CChangeEldaColorWnd::Init()
{


}

void CChangeEldaColorWnd::Composition()
{
	CButton*		btn = NULL;
	CTextBox*		tbox = NULL;
	CImageBox*		img = NULL;
	CImageBox9*		black_img9 = NULL;
	
	///-- �ܰ� ������	
	int center_x	= nRui->Desktop.Width / 2;
	int center_y	= nRui->Desktop.Height / 2;

	SRect enchantwnd;
	enchantwnd.w	= 240;
	enchantwnd.h	= 327;
	enchantwnd.x	= center_x - ( enchantwnd.w / 2 );
	enchantwnd.y	= center_y - ( enchantwnd.h / 2 );
	
	m_pFrameWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_None, WID_None, 
											   enchantwnd.x, enchantwnd.y, enchantwnd.w, enchantwnd.h, 
											   false );	
	m_pFrameWnd->SetWndTexStyle( fwfTex_Style_Tile9 );
	m_pFrameWnd->SetClientImage( TID_CTRL_WND_L1 );
	m_pFrameWnd->SetIconTID( TID_CTRL_ICON_INVEN );	
	m_pFrameWnd->SetWndTileSizeLR( 40, 29 );
	m_pFrameWnd->SetWndTileSizeTB( 31, 11 );
	m_pFrameWnd->SetCaption( G_STRING_CHAR(IDS_WND_ELDACHANGE_COLOR) );			///--HAN_MSG										
	m_pFrameWnd->SetTitle( true );	
	m_pFrameWnd->SetWndProc( this );
	m_pFrameWnd->SetVisible( false );
	m_pFrameWnd->Initialize();

	///-- �ݱ�
	m_pCloseBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, m_pFrameWnd->GetID(), 
									 221, 6, 13, 13, false );
	m_pCloseBtn->SetBtnImage( TID_CTRL_BTN_X, TID_CTRL_BTN_X_OVR, TID_CTRL_BTN_X_CLK, TID_None );
	
	///-- ��ű�( Static Text )
	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pFrameWnd->GetID(), 
									   80, 36, 80, 17, false );
	tbox->SetFontR( n_fRsvFontWndStatic );
	tbox->SetText( G_STRING_CHAR(IDS_ELDASTONE) );	///--  HAN_MSG

	///-- Slot
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pFrameWnd->GetID(), 
									   101, 56, 35, 35, false );       
	img ->SetClientImage( TID_CTRL_SLOT_SUNKEN );

		///-- ��ű� Slot
	m_pCEldaSlot = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, m_pFrameWnd->GetID(), 
											 101, 56, 35, 35, false );	
	m_pCEldaSlot->SetSlotGroup( n_SGROUP_TUNING );
	m_pCEldaSlot->SetHID( HID_ITEMTIP );
	
	///-- None Item ����
	m_pCEldaSlot->PcItem = &m_CEldaItem;

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pFrameWnd->GetID(),  
									   1, 99, 238, 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );

	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pFrameWnd->GetID(), 1, 107, 238, 17, false );
	tbox->SetFontR( n_fRsvFontWndStatic );	
	tbox->SetAlignText( n_atCenter, n_atTop );
	tbox->SetTextColor( 50, 50, 50 ); 
	tbox->SetMultiLine( true );
	tbox->SetLineInterval( 5 );
	tbox->SetMultiLineAsign(true);   
	tbox->SetText( G_STRING_CHAR(IDS_SYSPR_SELECT_ELDA_COLOR) );	///--  HAN_MSG

	///-- ������
	m_pCREldaCheckSlot[0] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, m_pFrameWnd->GetID(), 
									 12, 146, 35, 35, false );
	m_pCREldaCheckSlot[0]->SetSlotGroup( n_SGROUP_TUNING );
	m_pCREldaCheckSlot[0]->SetHID( HID_ITEMTIP );
	m_pCREldaCheckSlot[0]->PcItem = &m_CREldaCheckItem[0];

	///-- ��Ȳ��
	m_pCREldaCheckSlot[1] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, m_pFrameWnd->GetID(), 
									 57, 146, 35, 35, false );	
	m_pCREldaCheckSlot[1]->SetSlotGroup( n_SGROUP_TUNING );
	m_pCREldaCheckSlot[1]->SetHID( HID_ITEMTIP );
	m_pCREldaCheckSlot[1]->PcItem = &m_CREldaCheckItem[1];

	///-- �����
	m_pCREldaCheckSlot[2] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, m_pFrameWnd->GetID(), 
									 102, 146, 35, 35, false );	
	m_pCREldaCheckSlot[2]->SetSlotGroup( n_SGROUP_TUNING );
	m_pCREldaCheckSlot[2]->SetHID( HID_ITEMTIP );
	m_pCREldaCheckSlot[2]->PcItem = &m_CREldaCheckItem[2];

	///-- ���
	m_pCREldaCheckSlot[3] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, m_pFrameWnd->GetID(), 
									 147, 146, 35, 35, false );	
	m_pCREldaCheckSlot[3]->SetSlotGroup( n_SGROUP_TUNING );
	m_pCREldaCheckSlot[3]->SetHID( HID_ITEMTIP );
	m_pCREldaCheckSlot[3]->PcItem = &m_CREldaCheckItem[3];

	///-- ���λ�
	m_pCREldaCheckSlot[4] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, m_pFrameWnd->GetID(), 
									 192, 146, 35, 35, false );
	m_pCREldaCheckSlot[4]->SetSlotGroup( n_SGROUP_TUNING );
	m_pCREldaCheckSlot[4]->SetHID( HID_ITEMTIP );
	m_pCREldaCheckSlot[4]->PcItem = &m_CREldaCheckItem[4];

	///-- ����
	m_pCREldaCheckSlot[5] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, m_pFrameWnd->GetID(), 
									 12, 186, 35, 35, false );
	m_pCREldaCheckSlot[5]->SetSlotGroup( n_SGROUP_TUNING );
	m_pCREldaCheckSlot[5]->SetHID( HID_ITEMTIP );
	m_pCREldaCheckSlot[5]->PcItem = &m_CREldaCheckItem[5];

	///-- Ǫ����
	m_pCREldaCheckSlot[6] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, m_pFrameWnd->GetID(), 
									 57, 186, 35, 35, false );
	m_pCREldaCheckSlot[6]->SetSlotGroup( n_SGROUP_TUNING );
	m_pCREldaCheckSlot[6]->SetHID( HID_ITEMTIP );
	m_pCREldaCheckSlot[6]->PcItem = &m_CREldaCheckItem[6];

	///-- ����
	m_pCREldaCheckSlot[7] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, m_pFrameWnd->GetID(), 
									 102, 186, 35, 35, false );
	m_pCREldaCheckSlot[7]->SetSlotGroup( n_SGROUP_TUNING );
	m_pCREldaCheckSlot[7]->SetHID( HID_ITEMTIP );
	m_pCREldaCheckSlot[7]->PcItem = &m_CREldaCheckItem[7];

	///-- ���ֻ�
	m_pCREldaCheckSlot[8] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, m_pFrameWnd->GetID(), 
									 147, 186, 35, 35, false );
	m_pCREldaCheckSlot[8]->SetSlotGroup( n_SGROUP_TUNING );
	m_pCREldaCheckSlot[8]->SetHID( HID_ITEMTIP );
	m_pCREldaCheckSlot[8]->PcItem = &m_CREldaCheckItem[8];

	///-- �����
	m_pCREldaCheckSlot[9] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, m_pFrameWnd->GetID(), 
									 192, 186, 35, 35, false );
	m_pCREldaCheckSlot[9]->SetSlotGroup( n_SGROUP_TUNING );
	m_pCREldaCheckSlot[9]->SetHID( HID_ITEMTIP );
	m_pCREldaCheckSlot[9]->PcItem = &m_CREldaCheckItem[9];

	m_pCRSelectImg[0] = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pFrameWnd->GetID(), 
									   12, 146, 35, 35, false );
	m_pCRSelectImg[0]->SetClientImage( TID_SlotHighLight );
	m_pCRSelectImg[0]->SetFocusCheck(false);
	m_pCRSelectImg[0]->SetVisible(false);

	m_pCRSelectImg[1] = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pFrameWnd->GetID(), 
									   57, 146, 35, 35, false );
	m_pCRSelectImg[1]->SetClientImage( TID_SlotHighLight );
	m_pCRSelectImg[1]->SetFocusCheck(false);
	m_pCRSelectImg[1]->SetVisible(false);

	m_pCRSelectImg[2] = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pFrameWnd->GetID(), 
									   102, 146, 35, 35, false );
	m_pCRSelectImg[2]->SetClientImage( TID_SlotHighLight );
	m_pCRSelectImg[2]->SetFocusCheck(false);
	m_pCRSelectImg[2]->SetVisible(false);

	m_pCRSelectImg[3] = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pFrameWnd->GetID(), 
									   147, 146, 35, 35, false );
	m_pCRSelectImg[3]->SetClientImage( TID_SlotHighLight );
	m_pCRSelectImg[3]->SetFocusCheck(false);
	m_pCRSelectImg[3]->SetVisible(false);

	m_pCRSelectImg[4] = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pFrameWnd->GetID(), 
									   192, 146, 35, 35, false );
	m_pCRSelectImg[4]->SetClientImage( TID_SlotHighLight );
	m_pCRSelectImg[4]->SetFocusCheck(false);
	m_pCRSelectImg[4]->SetVisible(false);

	m_pCRSelectImg[5] = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pFrameWnd->GetID(), 
									   12, 186, 35, 35, false );
	m_pCRSelectImg[5]->SetClientImage( TID_SlotHighLight );
	m_pCRSelectImg[5]->SetFocusCheck(false);
	m_pCRSelectImg[5]->SetVisible(false);

	m_pCRSelectImg[6] = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pFrameWnd->GetID(), 
									   57, 186, 35, 35, false );
	m_pCRSelectImg[6]->SetClientImage( TID_SlotHighLight );
	m_pCRSelectImg[6]->SetFocusCheck(false);
	m_pCRSelectImg[6]->SetVisible(false);

	m_pCRSelectImg[7] = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pFrameWnd->GetID(), 
									   102, 186, 35, 35, false );
	m_pCRSelectImg[7]->SetClientImage( TID_SlotHighLight );
	m_pCRSelectImg[7]->SetFocusCheck(false);
	m_pCRSelectImg[7]->SetVisible(false);	
	
	m_pCRSelectImg[8] = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pFrameWnd->GetID(), 
									   147, 186, 35, 35, false );
	m_pCRSelectImg[8]->SetClientImage( TID_SlotHighLight );
	m_pCRSelectImg[8]->SetFocusCheck(false);
	m_pCRSelectImg[8]->SetVisible(false);

	m_pCRSelectImg[9] = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pFrameWnd->GetID(), 
									   192, 186, 35, 35, false );
	m_pCRSelectImg[9]->SetClientImage( TID_SlotHighLight );
	m_pCRSelectImg[9]->SetFocusCheck(false);
	m_pCRSelectImg[9]->SetVisible(false);

	///-- ��ű�( Static Text )
	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pFrameWnd->GetID(), 
									   1, 228, 238, 17, false );
	tbox->SetFontR( n_fRsvFontWndStatic );
	tbox->SetText( G_STRING_CHAR(IDS_SELECTED_COLOR) );	///--  HAN_MSG

	///-- ��ű�( Static Text )
	m_pSelectColorTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pFrameWnd->GetID(), 
									   1, 248, 238, 17, false );
	m_pSelectColorTxt->SetFontR( n_fRsvFontWndStatic );
	m_pSelectColorTxt->SetTextColor( 50, 50, 50 ); 
	m_pSelectColorTxt->SetText( G_STRING_CHAR(IDS_NONSELECTED) );	///--  HAN_MSG	

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pFrameWnd->GetID(), 
									   1, 268, 238, 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );	

	m_pOkBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, m_pFrameWnd->GetID(), 
								  91, 285, 58, 25, false );
	m_pOkBtn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_CTRL_BTN58_DIS );
	m_pOkBtn->SetFontR( n_fRsvFontWndButton ); 
	m_pOkBtn->SetText( G_STRING_CHAR( IDS_WND_COM_OK ) );								
	m_pOkBtn->SetBtnHold( true );	
	
	SetNonSelectColorElda();
	
}

void CChangeEldaColorWnd::Update()
{


}

void CChangeEldaColorWnd::SetNonSelectColorElda()
{
	int i = 0;
	
	for( i = 0 ; i < 10 ; i++ )
	{
		m_pCRSelectImg[i]->SetVisible(false);
		m_pCREldaCheckSlot[i]->theSlotState = n_Slot_Disable;
	}	
	
	m_nSelectIndex = -1;		
	m_pSelectColorTxt->SetText( G_STRING_CHAR(IDS_NONSELECTED) );				
}

void CChangeEldaColorWnd::Proc( SEventMessage* EMsg )
{
	if( EMsg->FocusWnd == m_pCloseBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			Close();
		}
	}	
	
	if( EMsg->FocusWnd == m_pOkBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			if( m_pCEldaSlot->PcItem->IType == n_ITYPE_NONE )
			{
				MessageBoxWnd.Open( G_STRING_CHAR( IDS_SYSPR_FIRST_UP_ELDASTON ) , 1200, NULL );
				return;
			}
			
			if( m_nSelectIndex == -1 )
			{
				MessageBoxWnd.Open( G_STRING_CHAR( IDS_SYSPR_NONSELECTEDCOLOR ) , 1200, NULL );		///--	HAN_MSG
				return;
			}			
			
			if( !m_IsButtonLock )
			{
				///-- ���⿡�� ������ ��Ŷ�� ������.
				//g_RockClient.Send_CsChangeEldaStone()				
				DWORD aEldaStoneID;
				DWORD aPayItemID;
				WORD aSelectedEldaStoneCode;

				aEldaStoneID = m_pCEldaSlot->PcItem->ItemUID;
				aPayItemID = m_CPayItem.ItemUID;
				aSelectedEldaStoneCode = m_pCREldaCheckSlot[m_nSelectIndex]->PcItem->ItemTable->theCode;
				
				g_MessageBox.SetParam( aEldaStoneID , aPayItemID , aSelectedEldaStoneCode );
				g_MessageBox.OpenMessageBox( n_MESSAGEBOX_CHANGE_ELDA_COLOR, 
											 G_STRING_CHAR( IDS_SYSPR_DO_CHANGE_COLOR ) , 300, 100 );	///-- HAN_MSG
				
				///--g_RockClient.Send_CsChangeEldaStone( aEldaStoneID, aPayItemID, aSelectedEldaStoneCode );				

				m_IsButtonLock = TRUE;
				Close();
			}
		}
	}

	if( EMsg->FocusWnd == m_pCEldaSlot->GetID() )
	{
		if( EMsg->Notify == NM_SLOT_CLICK )			
		{	
			ItemSlotClick( EMsg );		
		}
		else if( EMsg->Notify == NM_SLOT_DBCLICK )	
		{	
			ItemSlotDBClick( EMsg );	
		}
		
		///-- �� Slot �� ��� Item �� �ִٸ� ���� �غ�
		if( m_pCEldaSlot->PcItem->IType != n_ITYPE_NONE && 
			m_nSelectIndex != -1 )
		{
			m_pOkBtn->SetBtnHold( false );		
		}
		else
		{
			m_pOkBtn->SetBtnHold( true );
		}		
	}

	int i, j;
	i = j = 0;
	
	for( i = 0 ; i < 10 ; i++ )
	{
		if( m_pCREldaCheckSlot[i]->GetID() == EMsg->FocusWnd )
		{			
			if( EMsg->Notify == NM_SLOT_CLICK )
			{
				if( m_pCEldaSlot->PcItem->IType == n_ITYPE_NONE )
				{
					MessageBoxWnd.Open( G_STRING_CHAR( IDS_SYSPR_FIRST_UP_ELDASTON ) , 1200, NULL );
					return;
				}

				//���� �������� ���� �ߴٸ� .. 
				if( m_pCREldaCheckSlot[i]->PcItem->ItemTable &&  
					m_pCREldaCheckSlot[i]->PcItem->ItemTable->theSetID == m_pCEldaSlot->PcItem->ItemTable->theSetID )
				{					
					MessageBoxWnd.Open( G_STRING_CHAR( IDS_MSG_SAMENOT_CHANGECOLOR ) , 1200, NULL );
					return;
				}

				for( j = 0 ; j < 10 ; j++ )
				{
					m_pCRSelectImg[j]->SetVisible(false);
					m_pCREldaCheckSlot[j]->theSlotState = n_Slot_Disable;
				}		
				
				m_pCRSelectImg[i]->SetVisible(true);
				m_pCREldaCheckSlot[i]->theSlotState = n_Slot_Enable;
				m_nSelectIndex = i;		
				
				SetSelectColorTxt();

				///-- �� Slot �� ��� Item �� �ִٸ� ���� �غ�
				if( m_pCEldaSlot->PcItem->IType != n_ITYPE_NONE && 
					m_nSelectIndex != -1 )
				{
					m_pOkBtn->SetBtnHold( false );		
				}
				else
				{
					m_pOkBtn->SetBtnHold( true );
				}	
			}					
		}
	}
}

void CChangeEldaColorWnd::SetSelectColorTxt()
{
	switch( m_nSelectIndex )
	{
		case 0:
			{
				m_pSelectColorTxt->SetText( G_STRING_CHAR( IDS_COLOR_RED ) );	///-- HAN_MSG
			}
			break;
		case 1:
			{
				m_pSelectColorTxt->SetText( G_STRING_CHAR( IDS_COLOR_ORANGE ) );	///-- HAN_MSG
			}
			break;
		case 2:
			{
				m_pSelectColorTxt->SetText( G_STRING_CHAR( IDS_COLOR_YELLOW ) );	///-- HAN_MSG
			}
			break;
		case 3:
			{
				m_pSelectColorTxt->SetText( G_STRING_CHAR( IDS_COLOR_GREEN ) );	///-- HAN_MSG
			}
			break;
		case 4:
			{
				m_pSelectColorTxt->SetText( G_STRING_CHAR( IDS_COLOR_LIGHTGREEN ) );	///-- HAN_MSG
			}
			break;
		case 5:
			{
				m_pSelectColorTxt->SetText( G_STRING_CHAR( IDS_COLOR_INDIGO ) );	///-- HAN_MSG
			}
			break;
		case 6:
			{
				m_pSelectColorTxt->SetText( G_STRING_CHAR( IDS_COLOR_BLUE ) );	///-- HAN_MSG
			}
			break;
		case 7:
			{
				m_pSelectColorTxt->SetText( G_STRING_CHAR( IDS_COLOR_SILVER ) );	///-- HAN_MSG
			}
			break;
		case 8:
			{
				m_pSelectColorTxt->SetText( G_STRING_CHAR( IDS_COLOR_PURPLE ) );	///-- HAN_MSG
			}
			break;
		case 9:
			{
				m_pSelectColorTxt->SetText( G_STRING_CHAR( IDS_COLOR_VIOLET ) );	///-- HAN_MSG
			}
			break;
	}
}

void CChangeEldaColorWnd::ItemSlotClick( SEventMessage* EMsg )
{
	CSlot*		targetslot	= NULL;//���� ��� ����
	CSlot*		srcslot		= NULL;//���õ�(pick) �κ� ����
	SPcItem*	tmpPcItem	= NULL;
	int			position	= 0;

	SPcItem		tempSItem;
	TCHAR tMessage[256]	= {0,};

	///-- �������� ��� ���� ���
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
		if( targetslot == m_pCEldaSlot )
		{
			if(  srcslot->Group == n_SGROUP_INVENTORY   )
			{
				///-- ���ʿ��� ���ٽ����� �ƴ� �������� �����Ҽ� ���ٰ� ������ ��..
				BYTE aClass = srcslot->PcItem->ItemTable->theClass;
				if( aClass != n_Class_Eldastone )
				{
					MessageBoxWnd.Open( G_STRING_CHAR( IDS_SYSPR_NOT_ELDA_ITEM ) , 1200, NULL );					
					return;
				}
				
				if( aClass == n_Class_Eldastone && srcslot->PcItem->Code >	60000 )
				{
					MessageBoxWnd.Open( G_STRING_CHAR( IDS_MSG_NOT_USE_MIXELDA_STONE ) , 1200, NULL );					
					return;
				}

				if( aClass == n_Class_Eldastone && srcslot->PcItem->ItemTable->theSetID == CItemEldaMixAWndProc::ELDA_RAW )
				{
					MessageBoxWnd.Open( G_STRING_CHAR( IDS_MSG_NOT_USE_BASEELDA_STONE ) , 1200, NULL );					
					return;
				}

				///-- �ö󰡰� �Ǵ� ���ٽ����� ������ ���Ѵ�.
				int iLevel = srcslot->PcItem->ItemTable->theGrade/100;
				///-- ���� ���ٽ��� ������� ������ �� ��밡������ üũ�Ѵ�.
				if( ( iLevel < m_nMinLevel ) || ( iLevel > m_nMaxLevel ) )
				{
					Rsprintf( RWCHAR( tMessage ), G_STRING_CHAR( IDS_SYSPR_MINMAXLEVELCHNAGEELDACOLOR ) , 
							  m_nMinLevel, m_nMaxLevel );	///--HAN_MSG
					MessageBoxWnd.Open( tMessage , 1200, NULL );
					return;	
				}

				SetNonSelectColorElda();
				m_nNowEldaLevel = iLevel;
			
				///-- Copy item
				targetslot->SetSlotPcItem( *(srcslot->PcItem) );
				m_CEldaItem.Amount = 1;
				nRui->ResetCursorItem();

				ViewApplyEldaStoneList();
			}
		}	
	}
}

void CChangeEldaColorWnd::ItemSlotDBClick( SEventMessage* EMsg )
{
	CSlot*		tslot	= NULL;	
	CSlot*		sslot	= NULL;
	
	tslot = (CSlot*)Container->Get( (WNDID)EMsg->FocusWnd );				
	sslot = nRui->GetItemSelectedSlot();	
	
	if( tslot->PcItem )
	{
		tslot->PcItem->Clear();		
	}

	m_nNowEldaLevel = 0;
	ClearApplyEldaStoneList();
	SetNonSelectColorElda();
}

void CChangeEldaColorWnd::ViewApplyEldaStoneList()
{
	DWORD nItemCode = 0;
	
	//���� �ڵ� �߰� 

	/*
	ELDA_RAW = 0 ,	//���� 
	ELDA_GREEN,		//��� 
	ELDA_BLUE,		//Ǫ����
	ELDA_YELLOW,	//����� 
	ELDA_RED,		// ����
	ELDA_PURPLE,	//���� 
	ELDA_VIOLETE,	//���ֻ� 
	ELDA_NAVY,		//����
	ELDA_SILVER,	//���� 
	ELDA_YELLOW_GREEN,	//���λ� 
	ELDA_ORANGE,	//��Ȳ 
	*/

	if( m_nNowEldaLevel )
	{
		///������
		
		nItemCode = g_ItemEldaMixAWndProc.GetItemCode(m_nNowEldaLevel , CItemEldaMixAWndProc::ELDA_RED );
		SetSlotItem( nItemCode , m_pCREldaCheckSlot[0] );
		 
		//��Ȳ��
		nItemCode = g_ItemEldaMixAWndProc.GetItemCode(m_nNowEldaLevel , CItemEldaMixAWndProc::ELDA_ORANGE );
		SetSlotItem( nItemCode , m_pCREldaCheckSlot[1] );

		//�����
		nItemCode = g_ItemEldaMixAWndProc.GetItemCode(m_nNowEldaLevel , CItemEldaMixAWndProc::ELDA_YELLOW );
		SetSlotItem( nItemCode , m_pCREldaCheckSlot[2] );

		//���
		nItemCode = g_ItemEldaMixAWndProc.GetItemCode(m_nNowEldaLevel , CItemEldaMixAWndProc::ELDA_GREEN );
		SetSlotItem( nItemCode , m_pCREldaCheckSlot[3] );

		//���λ�
		nItemCode = g_ItemEldaMixAWndProc.GetItemCode(m_nNowEldaLevel , CItemEldaMixAWndProc::ELDA_YELLOW_GREEN );
		SetSlotItem( nItemCode , m_pCREldaCheckSlot[4] );

		//����
		nItemCode = g_ItemEldaMixAWndProc.GetItemCode(m_nNowEldaLevel , CItemEldaMixAWndProc::ELDA_NAVY );
		SetSlotItem( nItemCode , m_pCREldaCheckSlot[5] );

		//Ǫ����
		nItemCode = g_ItemEldaMixAWndProc.GetItemCode(m_nNowEldaLevel , CItemEldaMixAWndProc::ELDA_BLUE );
		SetSlotItem( nItemCode , m_pCREldaCheckSlot[6] );

		//����
		nItemCode = g_ItemEldaMixAWndProc.GetItemCode(m_nNowEldaLevel , CItemEldaMixAWndProc::ELDA_SILVER );
		SetSlotItem( nItemCode , m_pCREldaCheckSlot[7] );

		//���ֻ�
		nItemCode = g_ItemEldaMixAWndProc.GetItemCode(m_nNowEldaLevel , CItemEldaMixAWndProc::ELDA_VIOLETE );
		SetSlotItem( nItemCode , m_pCREldaCheckSlot[8] );

		//�����
		nItemCode = g_ItemEldaMixAWndProc.GetItemCode(m_nNowEldaLevel , CItemEldaMixAWndProc::ELDA_PURPLE );
		SetSlotItem( nItemCode , m_pCREldaCheckSlot[9] );
	}
}

void CChangeEldaColorWnd::ClearApplyEldaStoneList()
{
	for( int  i = 0 ; i < 10 ; i++ )
	{
		m_pCREldaCheckSlot[i]->PcItem->Clear();
	}
}

void CChangeEldaColorWnd::SetSlotItem(DWORD nItemCode , CSlot * pSlot)
{
	SItemBaseInfo* item_Info = NULL;
	item_Info = g_Pc_Manager.GetItemBaseInfoPtr( nItemCode );
	if( item_Info )
	{
		pSlot->PcItem->Class = g_Pc_Manager.GetItemClsss( nItemCode );
		pSlot->PcItem->ItemUID = 1;				
		pSlot->PcItem->IType	= n_ITYPE_UNUSE;
		
		if( g_Pc_Manager.GetItemClsss( nItemCode ) == n_Class_Use )
		{
			pSlot->PcItem->IType	= n_ITYPE_USE;//�Ҹ�ǰ
		}
		else if( g_Pc_Manager.GetItemClsss( nItemCode ) == n_Class_Useless )
		{
			pSlot->PcItem->IType	= n_ITYPE_UNUSE;//��Ҹ�ǰ
		}
		else if( g_Pc_Manager.GetItemClsss( nItemCode ) == n_Class_Special || 
				 g_Pc_Manager.GetItemClsss( nItemCode ) == n_Class_Eldastone ||
				 g_Pc_Manager.GetItemClsss( nItemCode ) == n_Class_Weapon_StrengthStone ||
				 g_Pc_Manager.GetItemClsss( nItemCode ) == n_Class_Armor_StrengthStone)
		{
			pSlot->PcItem->IType	= n_ITYPE_SPECIAL;
		}				
		
		pSlot->PcItem->Amount = 1;		
		pSlot->PcItem->Tid	= (TEXID)g_Pc_Manager.GetItemTID( nItemCode );
		pSlot->PcItem->ItemTable	= item_Info;	
	}	
}

void CChangeEldaColorWnd::CheckElda_ItemList()
{
	int i = 0;
	int j = 0;
	
	bool lb_isFind_Elda = FALSE;

	if( !m_pCEldaSlot->PcItem ) 
	{
		return;
	}
		///-----------------------------------------------------------------------
	///-- ��ü �κ��丮 Access
	for( j = 0; j < c_MaxInvenItemNum; ++j )
	{	
		if( nRui->GetPcInvenInfo()->InvenItem[j] == NULL )
		{
			continue;
		}
		
		///-- item slot
		if( m_pCEldaSlot->PcItem && 
			m_pCEldaSlot->PcItem->ItemUID == nRui->GetPcInvenInfo()->InvenItem[j]->ItemUID )
		{
			lb_isFind_Elda = true;			
		}	
	}

	///-----------------------------------------------------------------------
	///-- item �� ã�� �� ���� ���
	if( !lb_isFind_Elda )
	{
		m_pCEldaSlot->PcItem->Clear();
		m_nNowEldaLevel = 0;
		ClearApplyEldaStoneList();
		SetNonSelectColorElda();
	}

	///-----------------------------------------------------------------------
	///-- �� Slot �� ��� Item �� �ִٸ� ���� �غ�
	if( m_pCEldaSlot->PcItem->IType != n_ITYPE_NONE  )
	{
		m_pOkBtn->SetBtnHold( false );				
	}
	else
	{
		m_pOkBtn->SetBtnHold( true );
	}	
}

void CChangeEldaColorWnd::Open()
{
	m_nSelectIndex = -1;
	m_IsButtonLock = FALSE;	
	m_pSelectColorTxt->SetText( G_STRING_CHAR( IDS_NONSELECTED ) );		///-- HAN_MSG

	nRui->SendEvent( m_pFrameWnd->GetID(), n_emSetVisible, (EPARAM)true, 0, 0, 0 );
	ClearApplyEldaStoneList();
	m_pCEldaSlot->PcItem->Clear();	
}

void CChangeEldaColorWnd::Close()
{
	m_nMinLevel = 0;
	m_nMaxLevel = 0;

	m_nNowEldaLevel = 0;
	ClearApplyEldaStoneList();
	m_pCEldaSlot->PcItem->Clear();	
	SetNonSelectColorElda();

	nRui->SendEvent( m_pFrameWnd->GetID(), n_emSetVisible, (EPARAM)false, 0, 0, 0 );
}

void CChangeEldaColorWnd::SetPayPcItem( SPcItem * pPcItem )
{
	m_CPayItem = *pPcItem;
	///-- ���ٽ��� ������� �ּ�, �ִ� ������ ���Ѵ�.
	
	if( !m_CPayItem.ItemTable )
	{
		return;
	}

	if( m_CPayItem.ItemTable->theType != n_Type_Scroll || 
		m_CPayItem.ItemTable->theReHP != n_Scroll_ChangeEldaColor )
		///-- ���ٽ��� ���� ���� ������ Ŭ����
	{
		return;
	}

	m_CPayItem.Amount = 1;

	m_nMinLevel = m_CPayItem.ItemTable->theReSP;
	m_nMaxLevel = m_CPayItem.ItemTable->theReMP;

	m_nNowEldaLevel = 0;
	m_pSelectColorTxt->SetText( G_STRING_CHAR( IDS_NONSELECTED ) );
}

