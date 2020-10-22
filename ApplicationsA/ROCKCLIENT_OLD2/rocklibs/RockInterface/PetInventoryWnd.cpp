// PetInventoryWnd.cpp: implementation of the CPetInventoryWnd class.
//
//////////////////////////////////////////////////////////////////////
#include "..\\..\\RockClient.h"

#include "Rui.h"
//////////////////////////////////////////////////////////////////////////
#include "FrameWnd.h"
#include "Button.h"
#include "ImageBox.h"
#include "TextBox.h"
#include "EditBox.h"
#include "ProgressBar.h"
#include "UpGauge.h"
#include "SlideBar.h"
#include "CheckButton.h"
#include "RadioButton.h"
#include "ListBox.h"
#include "ScrollBar.h"
#include "ComboBox.h"
#include "Spin.h"
#include "Wnd.h"
//////////////////////////////////////////////////////////////////////////

#include "UIMsgWnd.h"
#include "InventoryWndProc.h"
#include "NumberInputWnd.h"
#include "PetMainWndProc.h"
#include "PetInfoWnd.h"
#include "PetInventoryWnd.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPetInventoryWnd g_PetInventoryWnd;

#define NOT_FOCUS -1


void CPetInventoryWnd::Init()
{


	m_SrcSlot = NULL;			
	m_TargetSlot = NULL;
	c_MaxPetInvenNum = 4;
	PcInven  = nRui->GetPcInvenInfo();
	m_CurSlotIndex = NOT_FOCUS ;
	

}

void CPetInventoryWnd::UpdatePetPcItem()
{
	 if( m_CurSlotIndex == NOT_FOCUS)
		 return;

	if(PcInven->thePetData[m_CurSlotIndex] != NULL )
	{
		for(int i=0; i<  PcInven->thePetData[m_CurSlotIndex]->MaxInvenNum; ++i)
		{
			PetInvenSlot[i]->PcItem =  PcInven->thePetData[m_CurSlotIndex]->InvenItem[i];
		}
	}

}



void CPetInventoryWnd::Proc( SEventMessage* EMsg )
{
	if(EMsg->FocusWnd  == CencelBtn->GetID())
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			Close();
		}
	}

	if(EMsg->FocusWnd == PetInvenSlot[0]->GetID() ||
		EMsg->FocusWnd == PetInvenSlot[1]->GetID() ||
		EMsg->FocusWnd == PetInvenSlot[2]->GetID() ||
		EMsg->FocusWnd == PetInvenSlot[3]->GetID() )
	{
		if( EMsg->Notify == NM_SLOT_FOCUS )			{	PetInvenSlotFocus( EMsg );		}
		else if( EMsg->Notify == NM_SLOT_PRESS )	{	PetInvenSlotFocus( EMsg );		}
		else if( EMsg->Notify == NM_SLOT_CLICK )	{	PetInvenSlotClick( EMsg );		}
		else if( EMsg->Notify == NM_SLOT_DBCLICK )	{	PetInvenSlotDBClick( EMsg );	}
		else if( EMsg->Notify == NM_SLOT_DRAG )		{	PetInvenSlotDrag( EMsg );		}
		else if( EMsg->Notify == NM_SLOT_DROP )		{	PetInvenSlotDrop( EMsg );		}
	}
}

// ���� �� �� ��ȭ�� �������� ��� �Ҽ� ���� ��Ȱ�� ó���� �Ѵ� .. 
// ������ ���� ������ �������� ��� �Ҽ� ���� ����� .. 
// ������ ���� ���� ������ ü���ؼ� �ش� ���� Ȱ��ȭ ��Ų�� .. 

void CPetInventoryWnd::Update()
{
/*
	for(int i =0 ; i < 4 ; ++i )
	{
		if( PetInvenSlot[i]->PcItem->IType == n_ITYPE_PET_USE )
		{
			
			PetInvenSlot[i]->theSlotState = n_Slot_Disable;		
			PetInvenSlot[i]->theSlotState = n_Slot_Enable;		
		}
	}
*/	
}

void CPetInventoryWnd::Open()
{
	UpdatePetPcItem();
	nRui->SendEvent( FrameWnd->GetID(), n_emSetVisible, (EPARAM)true, 0, 0, 0 );	
}

void CPetInventoryWnd::Close()
{
	nRui->SendEvent( FrameWnd->GetID(), n_emSetVisible, (EPARAM)false, 0, 0, 0 );	
}

void CPetInventoryWnd::Composition()
{

	int center_x	= nRui->Desktop.Width / 2;
	int center_y	= nRui->Desktop.Height / 2;

	SRect invenwnd;

	CImageBox*		img;


	invenwnd.w	= 200;
	invenwnd.h	= 90;

	invenwnd.x	= center_x - ( invenwnd.w / 2 );
	invenwnd.y	= center_y - ( invenwnd.h / 2 );
	

	///-- Frame Wnd
	FrameWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_None, WID_None, invenwnd.x, invenwnd.y, invenwnd.w, invenwnd.h, false );
	FrameWnd->SetWndTexStyle( fwfTex_Style_Tile9 );
	FrameWnd->SetClientImage( TID_CTRL_WND_L1 );
	FrameWnd->SetIconTID( TID_CTRL_ICON_INVEN );	
	FrameWnd->SetWndTileSizeLR( 40, 29 );
	FrameWnd->SetWndTileSizeTB( 31, 11 );

	FrameWnd->SetCaption( G_STRING_CHAR(IDS_PET_INVEN_WND) );	 
	FrameWnd->SetTitle( true );
	FrameWnd->SetWndProc( this );
	FrameWnd->SetVisible( false );
	FrameWnd->Initialize();
	

	///---------------------------------------------------------------------------	
	///-- Static
	///---------------------------------------------------------------------------			
	///-- Line
	//�ڽ����� ���� �κ�  ��׶��� 
//	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, FrameWnd->GetID(), 1, 1, 235, 140, false );
//	img->SetClientImage( TID_CTRL_BACK );
//	img->SetFocusCheck( false );
//	//���� 
//	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None,  FrameWnd->GetID(), 1, 60, 353, 1, false );
//	img ->SetClientImage( TID_CTRL_LINEDOT );
//	//���� 
//	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None,  FrameWnd->GetID(), 1, 261 , 353, 1, false );
//	img ->SetClientImage( TID_CTRL_LINEDOT );
//	//���� 
//	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None,  FrameWnd->GetID(), 1, 223, 151, 1, false );
//	img ->SetClientImage( TID_CTRL_LINEDOT );

	      
	///-- Slot
	///-- Wear Slot
	///-- Item Slots
	int SlotIntval= 35 ;
	int StartX = 30 ; 


	for(int i =0 ; i < 4 ; ++i )
	{

		img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None,  FrameWnd->GetID(), StartX + (SlotIntval* i ) , 40, 35, 35, false );    
		img->SetClientImage( TID_C_BGSLOT_INVEN_ONE );	
		PetInvenSlot[i] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None,  FrameWnd->GetID(), StartX + (SlotIntval* i ) , 40, 35, 35, false );

	}
	
	///---------------------------------------------------------------------------	
	///-- Control
	///---------------------------------------------------------------------------	
	///-- X		
	CencelBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None,  FrameWnd->GetID(), 180, 6, 13, 13, false );   
	CencelBtn->SetBtnImage( TID_CTRL_BTN_X, TID_CTRL_BTN_X_OVR, TID_CTRL_BTN_X_CLK, TID_None );
//	
//	
//	PetInvenSlot[1] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None,  FrameWnd->GetID(), 55 , 42, 35, 35, false );
//	PetInvenSlot[2] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None,  FrameWnd->GetID(), 90 , 42, 35, 35, false );
//	PetInvenSlot[3] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None,  FrameWnd->GetID(), 125, 42, 35, 35, false );

	PetInvenSlot[0]->SetSlotGroup( n_SGROUP_PET_INVENTORY, 0 );	
	PetInvenSlot[0]->SetHID( HID_ITEMTIP );	 //HAN_HID

	PetInvenSlot[1]->SetSlotGroup( n_SGROUP_PET_INVENTORY, 1 );
	PetInvenSlot[1]->SetHID( HID_ITEMTIP );	
	PetInvenSlot[2]->SetSlotGroup( n_SGROUP_PET_INVENTORY, 2 );
	PetInvenSlot[2]->SetHID( HID_ITEMTIP );	
	PetInvenSlot[3]->SetSlotGroup( n_SGROUP_PET_INVENTORY, 3 );
	PetInvenSlot[3]->SetHID( HID_ITEMTIP );	


	Init();

}



void CPetInventoryWnd::PetInvenSlotClick( SEventMessage* EMsg )
{
	if( m_CurSlotIndex == NOT_FOCUS)
		return;

	CSlot*		tslot	= NULL;	// Ŭ���� ��ȯâ ����
	CSlot*		sslot	= NULL;	// ���õ�(pick) ������ ����
	SPcItem*	tmpPcItem = NULL;		//���ҽ� ���� 
	

	if( nRui->GetCursorItem() != NULL )
	{
		tslot = (CSlot*)Container->Get( (WNDID)EMsg->FocusWnd );
		sslot = nRui->GetItemSelectedSlot();
		if( sslot->PcItem == NULL )
		{
			return;
		}
		
		if( tslot->PcItem && (tslot->PcItem == sslot->PcItem) )
		{
			nRui->ResetCursorItem();
		}
		else
		{			
			if( sslot->Group )
			{
				switch( sslot->Group )
				{
					//Pc�κ��丮���� ���κ����� 
				case n_SGROUP_INVENTORY:
					{						
						
						if(sslot->PcItem && sslot->PcItem->ItemTable)
						{
						
							if( sslot->PcItem->ItemTable->theBundle != 1 && sslot->PcItem->Amount > 1 ) 
							{
								g_NumberInputWnd.Open( n_NUMBERINPUT_INVENTORY_PETINVEN, sslot->PcItem );
								m_SrcSlot = sslot->SlotAddr;			
								m_TargetSlot = tslot;
								nRui->ResetCursorItem();
							}
							else 
							{
								
								g_RockClient.Send_CsMoveItemPCtoPet(sslot->PcItem->ItemUID,m_CurSlotIndex , tslot->SlotNum, sslot->PcItem->Amount );	
								nRui->ResetCursorItem();						
								
							}
						}
	
					}
					
					break;

					//�� �κ� ������  �̵� ..  
				case n_SGROUP_PET_INVENTORY:
					{
						SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOT_MOVE_PETINVEN ) );
						nRui->ResetCursorItem();
						/*
						
						//������ ������ �׳� �̵� ... 
						if( tslot->PcItem == NULL )
						{	
							//tslot->SetSlotPcItem( *(sslot->PcItem) );				
							//sslot->SlotAddr->PcItem->Clear();	
							PcInven->thePetData[m_CurSlotIndex]->InvenItem[tslot->SlotNum] = 	PcInven->thePetData[m_CurSlotIndex]->InvenItem[sslot->SlotNum];
							PcInven->thePetData[m_CurSlotIndex]->InvenItem[sslot->SlotNum] = NULL;
							
							UpdatePetPcItem();
							nRui->ResetCursorItem();
						}
						//�������� ������ ���� .~~~
						else
						{

							tmpPcItem = PcInven->thePetData[m_CurSlotIndex]->InvenItem[tslot->SlotNum] ;
							PcInven->thePetData[m_CurSlotIndex]->InvenItem[tslot->SlotNum] = 	PcInven->thePetData[m_CurSlotIndex]->InvenItem[sslot->SlotNum];
							PcInven->thePetData[m_CurSlotIndex]->InvenItem[sslot->SlotNum] = tmpPcItem;


							UpdatePetPcItem();
							nRui->ResetCursorItem();
						}
						*/
						
					}
					break;
				default:
					{
						nRui->ResetCursorItem();
					}
					break;
				}
			}			
		}
	}
	else
	{		
		//Ŀ���� ������ ���̱� �޼���.
		tslot = (CSlot*)Container->Get( (WNDID)EMsg->FocusWnd );
		if(tslot->PcItem == NULL) return;
		
		nRui->SetCursorItem( tslot->PcItem );
		nRui->SetItemSelectedSlot( tslot );
		nRui->theCursorState = n_CT_ITEMDROP;	
		
	}

}


void CPetInventoryWnd::PetInvenSlotDBClick( SEventMessage* EMsg )
{
	//������ ���	
	CSlot* slot = (CSlot*)Container->Get( EMsg->FocusWnd );

	if( slot->PcItem )
	{
		if( slot->PcItem->IType == n_ITYPE_PET_USE && slot->PcItem->ItemTable )		//�Ҹ� �������� ���
		{
			if( slot->PcItem->ItemTable && slot->PcItem->ItemTable->theType == n_Type_Scroll && 
				slot->PcItem->ItemTable->theReHP == n_Scroll_PetName )
			{
				///-- ���� �ش� ���Կ� ���� ��ȯ���̶�� �̸��� �ٲܼ� ����.
				if( g_PetInfoWnd.IsVisible && ( g_PetInfoWnd.m_CurPetNum == m_CurSlotIndex ) )
				{
					SystemPrint( n_DarkGreen ,n_SystemMsg, G_STRING_CHAR(IDS_SYSERROR_CALLPET) );
					return;
				}				
				
				if( g_RockClient.m_PcInven->thePetData[m_CurSlotIndex] )
				{
					g_UIMsgWnd.SetParam( g_RockClient.m_PcInven->thePetData[m_CurSlotIndex]->Name );
					g_UIMsgWnd.SetItemID( slot->PcItem->ItemUID );
					g_UIMsgWnd.SetPetSlot( (BYTE)m_CurSlotIndex );

					g_UIMsgWnd.SetMsgType(MSG_RE_PETNAME);
					nRui->SendEvent( WID_UIMsgWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
					nRui->SendEvent( WID_UIMsgInEdit, n_emSetFocus, 0, 0, 0, 0 );
				}
				
				return;
			}
			
			// ��� ������ = ��ũ�� ���������� ó�� �Ǿ� ���� 
			// ��ũ�� �������� ���� �����Ͽ� ��� �ϴ� ������ ó�� ( ����, �󱼺��� )
			g_RockClient.Send_CsUsePetItem( m_CurSlotIndex,slot->PcItem->ItemUID );							
			nRui->ResetCursorItem();
		}
	}	
}

void CPetInventoryWnd::PetInvenSlotDrag( SEventMessage* EMsg )
{
	
	int idx = 0;
	CSlot* slot = (CSlot*)Container->Get( EMsg->FocusWnd );
	
	if( ( slot->PcItem != NULL ) && ( EMsg->FocusWnd != (WNDID)EMsg->fParam ) && ( slot->PcItem->IType != n_ITYPE_NONE ) )
	{
		if( slot->PcItem->IType != n_ITYPE_NONE )
		{
			nRui->theCursorState = n_CT_ITEMDROP;

			//Ŀ���� ������ ���̱�
			if( nRui->GetCursorItem() == NULL )
			{
				nRui->SetCursorItem( slot->PcItem );
				nRui->SetItemSelectedSlot( slot );
			}
		}
	}


}
void CPetInventoryWnd::SetSrcSlot(CSlot* Src)
{
	m_SrcSlot	= Src;
}

void CPetInventoryWnd::SetTargetSlot(CSlot* Target)
{
	m_TargetSlot	= Target;
}

void CPetInventoryWnd::PetInvenSlotDrop( SEventMessage* EMsg )
{
	
	CSlot* sslot = (CSlot*)Container->Get( EMsg->FocusWnd );		// ����ִ� Src Slot
	CSlot* tslot = (CSlot*)Container->Get( (WNDID)EMsg->fParam );	// �������� Target Slot
	SPcItem*	tmpPcItem = NULL;
	

	if( ((WNDID)EMsg->fParam != WID_None ) && ( tslot->GetWindowType() == n_wtSlot ) )
	{
		if( sslot == tslot )
		{
			nRui->ResetCursorItem();
		}
		else if( ( sslot->PcItem != NULL ) && ( sslot->PcItem->IType != n_ITYPE_NONE ) ) 
		{
				switch( tslot->Group )
				{	

				//PC�κ���  ���� ??? 
				case n_SGROUP_INVENTORY:
					{							
						if( sslot->PcItem->ItemTable->theBundle != 1 && sslot->PcItem->Amount > 1 ) 
						{
							g_NumberInputWnd.Open( n_NUMBERINPUT_PETINVEN_INVENTORY, sslot->PcItem );
							
							InventoryWnd.SetSrcSlot(sslot);			
							InventoryWnd.SetTargetSlot(tslot);
							
							nRui->ResetCursorItem();
						}
						else
						{							
					
							g_RockClient.Send_CsMoveItemPettoPC(sslot->PcItem->ItemUID,m_CurSlotIndex , tslot->SlotNum, sslot->PcItem->Amount );
							nRui->ResetCursorItem();						
						}
					}
					break;
					//�� �κ��丮 ������  ���? �ӿ�ȯ�� ����� ~ �������� ~~ 
				case n_SGROUP_PET_INVENTORY:
					{
											
						SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOT_MOVE_PETINVEN ) );
						nRui->ResetCursorItem();
					//���������� ���� ������ ��������. 
					/*
						
						//������ ������ �׳� �̵� ... 
						if( tslot->PcItem == NULL )
						{	
				
							PcInven->thePetData[m_CurSlotIndex]->InvenItem[tslot->SlotNum] = 	PcInven->thePetData[m_CurSlotIndex]->InvenItem[sslot->SlotNum];
							PcInven->thePetData[m_CurSlotIndex]->InvenItem[sslot->SlotNum] = NULL;
							
							UpdatePetPcItem();
							nRui->ResetCursorItem();
						}

						//�������� ������ ���� .~~~
						//�׸��� ���� ������ ������ .. ��ġ�� (���� �߰� ... )... 
						else
						{

							SPcItem		pSrctmpItem;
							pSrctmpItem = *(sslot->PcItem);
							SPcItem		pTargettmpItem;
							pTargettmpItem = *(tslot->PcItem);
							tmpPcItem = PcInven->thePetData[m_CurSlotIndex]->InvenItem[tslot->SlotNum] ;
							PcInven->thePetData[m_CurSlotIndex]->InvenItem[tslot->SlotNum] = 	PcInven->thePetData[m_CurSlotIndex]->InvenItem[sslot->SlotNum];
							PcInven->thePetData[m_CurSlotIndex]->InvenItem[sslot->SlotNum] = tmpPcItem;


							UpdatePetPcItem();
							nRui->ResetCursorItem();
						}
				*/


					}
				break;

				default:
					{
						nRui->ResetCursorItem();
					}
					break;
				}
		
			
		}
	}


	nRui->ResetCursorItem();
	return;

}


void CPetInventoryWnd::PetInvenSlotFocus( SEventMessage* EMsg )
{
	static CSlot* tslot = NULL;
	
	tslot = (CSlot*)Container->Get( (WNDID)EMsg->FocusWnd );
	if( ( tslot->PcItem != NULL ) && ( nRui->GetCursorItem() == NULL ) )
	{
		if( tslot->PcItem->IType != n_ITYPE_NONE )
		{
			nRui->theCursorState = n_CT_HAND;
		}
	}
}



void CPetInventoryWnd::CloseNumberInputWnd( int aNumItem  )
{

	if( !m_SrcSlot )
	{
		return;
	}

	if( m_SrcSlot->PcItem )
	{
		
		if( m_SrcSlot->PcItem->Amount >= aNumItem )
		{
			
			
			g_RockClient.Send_CsMoveItemPCtoPet(m_SrcSlot->PcItem->ItemUID,m_CurSlotIndex , m_TargetSlot->SlotNum, aNumItem );
			//m_TargetSlot->SetSlotPcItem( *(m_SrcSlot->PcItem) );				
			//m_TargetSlot->PcItem->Amount = aNumItem;
			//	if(PcInven->thePetData[m_CurSlotIndex] !=NULL )
			//	{
			//		PcInven->thePetData[m_CurSlotIndex]->InvenItem[m_TargetSlot->SlotNum] = m_SrcSlot->PcItem;
			//		PcInven->thePetData[m_CurSlotIndex]->InvenItem[m_TargetSlot->SlotNum]->Amount = aNumItem;
			//		PcInven->InvenItem[m_SrcSlot->SlotNum] = NULL;
			//		UpdatePetPcItem();
			//		InventoryWnd.UpdateInvenSlotItem();
			//	}
			
		}
		else
		{
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOT_ENOUGH_NUM ) );
		}
	

	}


	
	return;


}