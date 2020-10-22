#include "..\\RockPCH.h"

#include "Rui.h"
#include "..\\..\\brother.h"

#include "Define.h"
#include "Util.h"
#include "Render.h"
//#include "Fontman.h"
#include "Resource.h"
#include "Chain.h"
#include "Container.h"



#include "..\\..\\global.h"
#include "..\\..\\RockClient.h"

#include "..\\..\\bravolibs\\obj\\SkillSystem.h"

//#include "pc.h"
#include "SpellStoneWndProc.h"
#include "StringManager.h"
#include "..\\..\\VFileSystem.h"

#include "ItemSelectPotionWndProc.h"
#include "InventoryWndProc.h"

CSpellStoneWndProc*	g_SpellStoneWndProc;


//CFontg*				l_Fontg;	
//extern CFontman*	Fontman ;
extern	long	g_nowTime;


//-----------------------------------------------------------------------------
void CSpellStoneWndProc::Init()
{


}
CSpellStoneWndProc::CSpellStoneWndProc()
{
//	CreateLoadTable();
}


void CSpellStoneWndProc::Open()
{
	int i=0;
	
	nRui->SendEvent( m_pFrameWnd->GetID() , n_emSetVisible, (EPARAM)true, 0, 0, 0 );
		
	for( i=0 ; i < MAKESPELLSTONEMAX ; ++i )
	{
		m_CurSlot[i]->PcItem->Clear();
	}

	start_btn->SetBtnHold( true );
	cancel_btn->SetBtnHold( false );
	
	mb_button_lock = false;	
	
}
///---------------------------------------------------------------------------
///-- CloseEnchantWnd
///---------------------------------------------------------------------------
///-- ���� â�� �ݴ´�.
///-- �� Resource �� �����Ѵ�. ( Dynamic Texture_M �� �����Ǿ�� �� ) 
void CSpellStoneWndProc::Close()
{

	if(m_pFrameWnd != NULL)
	{
		nRui->SendEvent( m_pFrameWnd->GetID(), n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	}
	
	///TID_ELD_WND_UpGrade ����

}
///---------------------------------------------------------------------------
///-- Composition
///---------------------------------------------------------------------------
void CSpellStoneWndProc::Composition()
{
	CImageBox*		img;
	CButton*		btn = NULL;
	CTextBox*		tbox = NULL;
	
	int center_x	= nRui->Desktop.Width / 2;
	int center_y	= nRui->Desktop.Height / 2;

	SRect mixEldaWnd;
	mixEldaWnd.w	= 295;
	mixEldaWnd.h	= 220;
	mixEldaWnd.x	= center_x - ( mixEldaWnd.w / 2 );
	mixEldaWnd.y	= center_y - ( mixEldaWnd.h / 2 );
	
	///-- Frame Wnd
	m_pFrameWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_None, WID_None, mixEldaWnd.x, mixEldaWnd.y, mixEldaWnd.w, mixEldaWnd.h, false );	
	m_pFrameWnd->SetWndTexStyle( fwfTex_Style_Tile9 );
	m_pFrameWnd->SetClientImage( TID_CTRL_WND_L1 );
	m_pFrameWnd->SetIconTID( TID_CTRL_ICON_INVEN );	
	m_pFrameWnd->SetWndTileSizeLR( 40, 29 );
	m_pFrameWnd->SetWndTileSizeTB( 31, 11 );

	m_pFrameWnd->SetCaption(G_STRING_CHAR(IDS_WND_MAKE_SPELL_STONE));
	m_pFrameWnd->SetTitle( true );	
	m_pFrameWnd->SetWndProc( this );
	m_pFrameWnd->SetVisible( false );
	m_pFrameWnd->Initialize();

	///---------------------------------------------------------------------------	
	///-- Static
	///---------------------------------------------------------------------------		
	///-- Line
	int iAllHeght = 23;

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pFrameWnd->GetID(), 1, 111+iAllHeght, mixEldaWnd.w -2, 55, false );
	img->SetClientImage( TID_CTRL_BACK );
	img->SetFocusCheck( false );

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pFrameWnd->GetID(),  1, 110+iAllHeght, mixEldaWnd.w -2, 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pFrameWnd->GetID(), 1, 166+iAllHeght, mixEldaWnd.w -2, 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );
	


	int sx = 37 , sy = 27;

	for(int i =0 ; i < MAKESPELLSTONEMAX ;  ++i)
	{
		
		m_PreSlot[i] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None , m_pFrameWnd->GetID() , sx+(i*48), 30+iAllHeght, 35, 35, false );	
		m_PreSlot[i]->SetSlotGroup( n_SGROUP_SKILL_POTION );
		m_PreSlot[i]->SetHID( HID_ITEMTIP );
		m_PreSlot[i]->SetSlotItem(&m_PrePcItem[i]);
		m_PreSlot[i]->theSlotState = n_Slot_Disable;
	
		img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pFrameWnd->GetID(), sx+(i*48), 27+35+5+iAllHeght, 35, 35, false );       
		img ->SetClientImage( TID_CTRL_SLOT_SUNKEN );
		m_CurSlot[i] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None , m_pFrameWnd->GetID() , sx+(i*48), 27+35+5+iAllHeght, 35, 35, false );	
		m_CurSlot[i]->SetSlotGroup( n_SGROUP_SKILL_POTION );
		m_CurSlot[i]->SetHID( HID_ITEMTIP );
		m_CurSlot[i]->SetSlotItem( &m_CurPcItem[i]);
		
	}

	///-- Result
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pFrameWnd->GetID(), 132 , 127+iAllHeght, 35, 35, false );            
	img ->SetClientImage( TID_CTRL_SLOT_SUNKEN );
	m_PreSlot[MAKESPELLSTONEMAX] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None , m_pFrameWnd->GetID() ,132 , 127+iAllHeght, 35, 35, false );	
	m_PreSlot[MAKESPELLSTONEMAX]->SetSlotGroup( n_SGROUP_SKILL_POTION );
	m_PreSlot[MAKESPELLSTONEMAX]->SetHID( HID_ITEMTIP );
	m_PreSlot[MAKESPELLSTONEMAX]->SetSlotItem(&m_PrePcItem[MAKESPELLSTONEMAX]);
	

	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pFrameWnd->GetID(), 0 , 112+iAllHeght, mixEldaWnd.w, 17, false );
	tbox->SetFontR( n_fRsvFontWndStatic );	
	tbox->SetAlignText( n_atCenter, n_atCenter );
	tbox->SetText(G_STRING_CHAR(IDS_WND_RESULT_ITEM));	
		 
	m_MoneyText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pFrameWnd->GetID(), 1,  26 , 190, 22, false );
	m_MoneyText->SetFontR( n_fRsvFontWndStatic );	
	m_MoneyText->SetAlignText( n_atRight, n_atCenter );
	m_MoneyText->SetText( G_STRING_CHAR(IDS_WND_EXPENSE) );
	

	///---------------------------------------------------------------------------	
	///-- Control
	///---------------------------------------------------------------------------	
	///-- ���� ��ư
	
	start_btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, m_pFrameWnd->GetID(), (mixEldaWnd.w/2) - (58+ 20 )  , 191, 58, 25, false );
	start_btn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_CTRL_BTN58_DIS );
	start_btn->SetFontR( n_fRsvFontWndButton ); 
	start_btn->SetText( G_STRING_CHAR( IDS_WND_COM_START ) ); 
//	start_btn->SetBtnHold( true );

	///-- ���
	cancel_btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, m_pFrameWnd->GetID(), (mixEldaWnd.w/2) + (22), 191, 58, 25, false );
	cancel_btn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_CTRL_BTN58_DIS );	
	cancel_btn->SetFontR( n_fRsvFontWndButton ); 
	cancel_btn->SetText( G_STRING_CHAR( IDS_WND_COM_CANCEL ) ); 
	
	///-- None Item ����

	Init();
}

void CSpellStoneWndProc::ClearPreItem()
{
	int i =0 ;

	for( i = 0; i < MAKESPELLSTONEMAX+1 ; ++i)
	{
		m_PreSlot[i]->PcItem->Clear();
		m_CurSlot[i]->PcItem->Clear();
	}

}

//-- �������� �̸� �����ش� . 
//-- ���ٽ��� ���� + ��1÷���� + ��2÷���� = �������� 



///---------------------------------------------------------------------------
///-- Update
///---------------------------------------------------------------------------
///-- �� ���� ���濡 ���� Upgrade ����� �����ش�.
void CSpellStoneWndProc::Update()
{
	///-- CheckItemList - Inven ���濡 ���� ��þƮ â�� ����
	CheckItemList();
}

//� �ƾ����� Ŭ�� �ߴ°� ? 
BOOL CSpellStoneWndProc::SetClickItem(SPcItem* PcItem)
{
	char tmpStr[256] ={0,};
	char MoneyStr[256] ={0,};
	int i=0,j=0 ;
	
	if( PcItem->ItemTable == NULL )
		return FALSE;

	ClearPreItem();

	std::vector<SSkillPotionTable>::iterator ir = g_ItemSelectPotionWndProc.m_RealItem.begin();
	
	for( ; ir != g_ItemSelectPotionWndProc.m_RealItem.end() ; ++ir)	
	{
		
		//���Կ� �÷����� �ິ�� �㰡�Ѵ�.  
		if(  (*ir).theCreatePotion == PcItem->Code  )
		{
			for(j =0 ; j < MAKESPELLSTONEMAX ; ++j)
			{
				//�̸����� �������� �����. 
				g_ItemSelectPotionWndProc.SetAddPcItem(&m_PrePcItem[j], (*ir).theAddPotion[j].theItemCode , (*ir).theAddPotion[j].theBundle );
			}
			//��� �����۵� ... 
						
			g_ItemSelectPotionWndProc.SetAddPcItem(&m_PrePcItem[MAKESPELLSTONEMAX],  (*ir).theCreatePotion , 1 );	

			InventoryWnd.SetEldText(MoneyStr ,(*ir).theMakeMoney );
			Rsprintf(RWCHAR(tmpStr),_RT("%s : %s"), G_STRING_CHAR(IDS_WND_EXPENSE), RWCHAR(MoneyStr) ); 
			m_MoneyText->SetText(tmpStr);


			return TRUE;
		}
	}	

	return FALSE;
}


///---------------------------------------------------------------------------
///-- CheckItemList - Inven ���濡 ���� ��þƮ â�� ����
///---------------------------------------------------------------------------
BOOL CSpellStoneWndProc::CheckItemList()
{
	int i = 0;
	int j = 0;
	
	bool lb_isFind_item = FALSE;
	bool lb_isFind_elda = FALSE;
	
	bool b_isFind_elda[MAKESPELLSTONEMAX] ;
	
	ZeroMemory(&b_isFind_elda , sizeof(b_isFind_elda));

	if( !m_CurSlot[0]->PcItem && !m_CurSlot[1]->PcItem && 
		!m_CurSlot[2]->PcItem && m_CurSlot[3]->PcItem && m_CurSlot[4]->PcItem) 
	{
		return FALSE;
	}

	///-----------------------------------------------------------------------
	///-- ��ü �κ��丮 Access
	for( j = 0; j < c_MaxInvenItemNum; ++j )
	{	
		if( nRui->GetPcInvenInfo()->InvenItem[j] == NULL )
		{
			continue;
		}
		for(i =0 ; i< MAKESPELLSTONEMAX ; ++i )
		{
			///-- Elda slot
			if( m_CurSlot[i]->PcItem && 
				m_CurSlot[i]->PcItem->ItemUID == nRui->GetPcInvenInfo()->InvenItem[j]->ItemUID )
			{
				b_isFind_elda[i] = true;
			}
		}
	}

	///-----------------------------------------------------------------------
	///-- item �� ã�� �� ���� ���
	for(i = 0 ; i <MAKESPELLSTONEMAX ; ++i)
	{	
		if(!b_isFind_elda[i])
			m_CurSlot[i]->PcItem->Clear();
	}

	///-----------------------------------------------------------------------
	///-- �� Slot �� ��� Item �� �ִٸ� ���� �غ�
	if( m_CurSlot[0]->PcItem->IType != n_ITYPE_NONE && 
		m_CurSlot[1]->PcItem->IType != n_ITYPE_NONE && 
		m_CurSlot[2]->PcItem->IType != n_ITYPE_NONE &&
		m_CurSlot[3]->PcItem->IType != n_ITYPE_NONE &&
		m_CurSlot[4]->PcItem->IType != n_ITYPE_NONE )
	{
		
		if(!mb_button_lock)
		{
			mb_button_lock = true; 
			start_btn->SetBtnHold( false );				
		}
				
	}
	else
	{
		mb_button_lock = false; 
		start_btn->SetBtnHold( true );
	}

	return FALSE;
}


//-----------------------------------------------------------------------------
void CSpellStoneWndProc::Proc( SEventMessage* EMsg )
{

	if(EMsg->FocusWnd == cancel_btn->GetID())
	{
		if(EMsg->Notify == NM_BTN_CLICK)
		{
			nRui->SendEvent(m_pFrameWnd->GetID() , n_emSetVisible , (EPARAM)false , 0 , 0 , 0 );
			
		}
	}

	for(int i =0 ; i < MAKESPELLSTONEMAX ; ++i)
	{
		if(EMsg->FocusWnd == m_CurSlot[i]->GetID() )
		{
			if( EMsg->Notify == NM_SLOT_CLICK)
			{
				ItemSlotClick(EMsg);
			}
			if(EMsg->Notify == NM_SLOT_DBCLICK)
			{
				ItemSlotDBClick(EMsg);
			}


		}
	}

	//��ŸƮ �ϸ� ��ư ��Ȱ��ȭ .. 
	if(EMsg->FocusWnd == start_btn->GetID() )
	{
		if(EMsg->Notify == NM_BTN_CLICK )
		{
			for(int i = 0 ; i < MAKESPELLSTONEMAX ; ++i)
			{
				Potion[i].theItemID = m_CurPcItem[i].ItemUID;
				Potion[i].theBundle = m_CurPcItem[i].Amount ; 
			}
						
			g_RockClient.Send_SRpCsMakeStrengthstone( m_PrePcItem[MAKESPELLSTONEMAX].Code , Potion );

			Close();
		
			//�κ��丮�� disible ��Ų��. 
			nRui->ResetCursorItem();
		
		}

	}

}

///---------------------------------------------------------------------------
///-- ItemSlotDBClick
///---------------------------------------------------------------------------
///-- Item ����
void CSpellStoneWndProc::ItemSlotDBClick( SEventMessage* EMsg )
{
	CSlot*		tslot	= NULL;	
	CSlot*		sslot	= NULL;
	
	tslot = (CSlot*)Container->Get( (WNDID)EMsg->FocusWnd );				
	sslot = nRui->GetItemSelectedSlot();
	
	
	if( tslot->PcItem )
	{
		tslot->PcItem->Clear();		
	}

	return;
}
///---------------------------------------------------------------------------
///-- ItemSlotClick
///---------------------------------------------------------------------------
///-- Item �ø���
///-- !!!! ������ ������ ������ Hard Coding

void CSpellStoneWndProc::ItemSlotClick( SEventMessage* EMsg )
{

	CSlot*		targetslot	= NULL;//���� ��� ����
	CSlot*		srcslot		= NULL;//���õ�(pick) �κ� ����
	SPcItem*	tmpPcItem	= NULL;
	int			position	= 0;

	///-- �������� ��� ���� ���
	if( nRui->GetCursorItem() != NULL )
	{
		
		targetslot = (CSlot*)Container->Get( (WNDID)EMsg->FocusWnd );
		srcslot = nRui->GetItemSelectedSlot();

		if( srcslot->PcItem == NULL )
		{
			return;
		}		
		
		if ( srcslot->Group != n_SGROUP_INVENTORY)
		{	
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_OUT_INVENTORY_MOVE));			
			return;
		}
	
		if( targetslot->PcItem && (targetslot->PcItem == srcslot->PcItem) )
		{
			nRui->ResetCursorItem();
		}
	
		///-------------------------------------------------------------------
		///-- �����ິ �� �����ϴ� . 
		for( int i = 0 ; i < MAKESPELLSTONEMAX ; ++i)
		{
			if( targetslot == m_CurSlot[i]) 
			{
				CheckItemAdd(&m_PrePcItem[i] , &m_CurPcItem[i] ,srcslot->PcItem , targetslot->PcItem );
			}
		}
	}

	
	/*
	else
	{
	//Ŀ���� ������ ���̱�
	targetslot = (CSlot*)Container->Get( (WNDID)EMsg->FocusWnd );
	
	  nRui->SetCursorItem( targetslot->PcItem );
	  nRui->SetItemSelectedSlot( targetslot );
	  }
	*/
}
	//�̸���������۰� ������Ų�� 
void CSpellStoneWndProc::CheckItemAdd(SPcItem* PrePcItem ,SPcItem* CurPcItem,SPcItem* SrcPcItem , SPcItem *TarGetPcItem)
{
	if(PrePcItem == NULL ||	CurPcItem == NULL || TarGetPcItem->ItemTable != NULL)
		return;
	
	if(PrePcItem->Code == SrcPcItem->Code)
	{
		if( SrcPcItem->Amount >= PrePcItem->Amount )
		{
			///-- Copy item
			*TarGetPcItem = *SrcPcItem ;
			TarGetPcItem->Amount = PrePcItem->Amount;
			nRui->ResetCursorItem();
		}
		else
		{
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_NEED_ITEM_COUNT) );
		}
	}
	else
	{
		SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_ANOTHER_ITEM) );	
	}

}

