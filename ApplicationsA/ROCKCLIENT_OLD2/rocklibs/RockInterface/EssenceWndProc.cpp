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
#include "EssenceWndProc.h"
#include "StringManager.h"
#include "..\\..\\VFileSystem.h"

#include "ItemSelectPotionWndProc.h"
#include "InventoryWndProc.h"

CEssenceWndProc	*g_EssenceWndProc;
extern	long	g_nowTime;



//-----------------------------------------------------------------------------
void CEssenceWndProc::Init()
{


}
CEssenceWndProc::CEssenceWndProc()
{
//	CreateLoadTable();
}


void CEssenceWndProc::Open()
{
	//IsVisible = true; 
	nRui->SendEvent( m_pFrameWnd->GetID() , n_emSetVisible, (EPARAM)true, 0, 0, 0 );

	ChangeGold[0]->SetText( _RT("") );
	ChangeGold[1]->SetText( _RT("") );
	EditItemCreateNum->SetText(_RT(""));
	m_MaxItemCount =0; 

//	start_btn->SetBtnHold( true );
//	cancel_btn->SetBtnHold( false );
	
	//mb_button_lock = false;	
	
}
///---------------------------------------------------------------------------
///-- CloseEnchantWnd
///---------------------------------------------------------------------------
///-- ���� â�� �ݴ´�.
///-- �� Resource �� �����Ѵ�. ( Dynamic Texture_M �� �����Ǿ�� �� ) 
void CEssenceWndProc::Close()
{
	if(m_pFrameWnd != NULL)
	{
		nRui->SendEvent( m_pFrameWnd->GetID(), n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	}
}
///---------------------------------------------------------------------------
///-- Composition
///---------------------------------------------------------------------------
void CEssenceWndProc::Composition()
{
	CImageBox*		img = NULL;
	CButton*		btn = NULL;
	CTextBox*		tbox = NULL;
	CImageBox9*		img9 = NULL;

	int center_x	= nRui->Desktop.Width / 2;
	int center_y	= nRui->Desktop.Height / 2;

	SRect mixEldaWnd;
	mixEldaWnd.w	= 236;
	mixEldaWnd.h	= 277;
	mixEldaWnd.x	= center_x - ( mixEldaWnd.w / 2 );
	mixEldaWnd.y	= center_y - ( mixEldaWnd.h / 2 );
	
	///-- Frame Wnd
	m_pFrameWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_None, WID_None, mixEldaWnd.x, mixEldaWnd.y, mixEldaWnd.w, mixEldaWnd.h, false );	
	m_pFrameWnd->SetWndTexStyle( fwfTex_Style_Tile9 );
	m_pFrameWnd->SetClientImage( TID_CTRL_WND_L1 );
	m_pFrameWnd->SetIconTID( TID_CTRL_ICON_INVEN );	
	m_pFrameWnd->SetWndTileSizeLR( 40, 29 );
	m_pFrameWnd->SetWndTileSizeTB( 31, 11 );

	m_pFrameWnd->SetCaption( G_STRING_CHAR(IDS_WND_ESSENCE_MAKE) );	//HAN_MSG	
	m_pFrameWnd->SetTitle( true );	
	m_pFrameWnd->SetWndProc( this );
	m_pFrameWnd->SetVisible( false );
	m_pFrameWnd->Initialize();

	///---------------------------------------------------------------------------	
	///-- Static
	///---------------------------------------------------------------------------		
	///-- Line

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pFrameWnd->GetID(), 1, 145, mixEldaWnd.w -2 , 81, false );
	img->SetClientImage( TID_CTRL_BACK );
	img->SetFocusCheck( false );

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pFrameWnd->GetID(),  1, 144, mixEldaWnd.w -2 , 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pFrameWnd->GetID(), 1, 225, mixEldaWnd.w -2 , 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );

	//////////////////////////////////////////////////////////////////////////
	//���� / �Ѱ���
	//////////////////////////////////////////////////////////////////////////
	
//	ImgSlot[0] = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pFrameWnd->GetID(), 15, 57 , 35 , 35, false );
//	ImgSlot[0]->SetClientImage( TID_ACT_COMPLIMENTS );
//	ImgSlot[0]->SetFocusCheck( false );

	m_PreSlot[0] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None , m_pFrameWnd->GetID() , 15 , 57 , 35, 35, false );	
	m_PreSlot[0]->SetSlotGroup( n_SGROUP_SKILL_POTION );
	m_PreSlot[0]->SetHID( HID_ITEMTIP );
	m_PreSlot[0]->SetSlotItem(&m_PrePcItem[0]);
	//m_PreSlot[0]->theSlotState = n_Slot_Disable;
	//////////////////////////////////////////////////////////////////////////
	m_PreSlot[1] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None , m_pFrameWnd->GetID() , (mixEldaWnd.w/2) - (35>>1), 164 , 35 , 35, false );
	m_PreSlot[1]->SetSlotGroup( n_SGROUP_SKILL_POTION );
	m_PreSlot[1]->SetHID( HID_ITEMTIP );
	m_PreSlot[1]->SetSlotItem(&m_PrePcItem[1]);
	//m_PreSlot[1]->theSlotState = n_Slot_Disable;

	img9 = (CImageBox9*)nRui->CreateWnd( n_wtImageBox9, WID_None, m_pFrameWnd->GetID(), 
		75 , 65  , 120, 23 , false );       	
	img9->SetWndTileSizeLR( 9, 9 );
	img9->SetWndTileSizeTB( 8, 8 );
	img9->SetFocusCheck( false );
	img9->SetClientImage( TID_CTRL_CONTBACK_L1 );
	img9->SetAdjustPosition( 0, 0 );
	
	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pFrameWnd->GetID(), 75,  46 , 120, 22, false );
	tbox->SetFontR( n_fRsvFontWndStatic );	
	tbox->SetAlignText( n_atRight, n_atCenter );
	tbox->SetText( G_STRING_CHAR(IDS_ESSENCE_MIN_MAX)); 
	
	ChangeGold[0] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pFrameWnd->GetID(), 75,  67 , 120, 22, false );
	ChangeGold[0]->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	ChangeGold[0]->SetClientImage( TID_None );	
	ChangeGold[0]->SetTextColor( 255, 255, 255 );
	ChangeGold[0]->SetAlignText( n_atCenter, n_atCenter );
	ChangeGold[0]->SetText( _RT("999/999") );

	
	//////////////////////////////////////////////////////////////////////////
	//���
	//////////////////////////////////////////////////////////////////////////
	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pFrameWnd->GetID(), 15 ,  104 , 120 , 22, false );
	tbox->SetFontR( n_fRsvFontWndStatic );	
	tbox->SetAlignText( n_atCenter, n_atCenter );
	tbox->SetText( G_STRING_CHAR(IDS_WND_EXPENSE) );
	
	img9 = (CImageBox9*)nRui->CreateWnd( n_wtImageBox9, WID_None, m_pFrameWnd->GetID(), 
		15 , 120 , 120, 23 , false );       	
	img9->SetWndTileSizeLR( 9, 9 );
	img9->SetWndTileSizeTB( 8, 8 );
	img9->SetFocusCheck( false );
	img9->SetClientImage( TID_CTRL_CONTBACK_L1 );
	img9->SetAdjustPosition( 0, 0 );

	ChangeGold[1] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pFrameWnd->GetID(), 17,  122 , 120, 22, false );
	ChangeGold[1]->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	ChangeGold[1]->SetClientImage(TID_None);
	ChangeGold[1]->SetTextColor( 255, 255, 255 );
	ChangeGold[1]->SetAlignText( n_atCenter, n_atCenter );
	ChangeGold[1]->SetText( _RT("500,000") );

	
	//////////////////////////////////////////////////////////////////////////
	//��� ������ 
	//////////////////////////////////////////////////////////////////////////
	

	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pFrameWnd->GetID(), 0 ,  146 , mixEldaWnd.w , 22, false );
	tbox->SetFontR( n_fRsvFontWndStatic );	
	tbox->SetAlignText( n_atCenter ,  n_atCenter );
	tbox->SetText( G_STRING_CHAR(IDS_ESSENCE_CREATE_ITEM) ); 
/*	
	ImgSlot[1] = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pFrameWnd->GetID(), 
		(mixEldaWnd.w/2) - (35>>1), 164 , 35 , 35, false );
	ImgSlot[1]->SetClientImage( TID_ACT_COMPLIMENTS );
	ImgSlot[1]->SetFocusCheck( false );
*/
	
	
	img9 = (CImageBox9*)nRui->CreateWnd( n_wtImageBox9, WID_None, m_pFrameWnd->GetID(), 
		114 , 200  , 120, 23 , false );       	
	img9->SetWndTileSizeLR( 9, 9 );
	img9->SetWndTileSizeTB( 8, 8 );
	img9->SetFocusCheck( false );
	img9->SetClientImage( TID_CTRL_CONTBACK_L1 );
	img9->SetAdjustPosition( 0, 0 );


	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pFrameWnd->GetID(), 114 ,  202 , 120, 22, false );
	tbox->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	tbox->SetClientImage(TID_None);
	tbox->SetClientColor( 0, 0, 0 );
	tbox->SetAlignText( n_atRight, n_atCenter );
	tbox->SetText( G_STRING_CHAR(IDS_WND_AMOUNT));
	

	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pFrameWnd->GetID(), 0 ,  202 , 114, 22, false );
	tbox->SetFontR( n_fRsvFontWndStatic );	
	tbox->SetAlignText( n_atCenter, n_atCenter );
	tbox->SetText( G_STRING_CHAR(IDS_WND_EDIT_NEED_COUNT));
	

	EditItemCreateNum = (CEditBox*)nRui->CreateWnd( n_wtEditBox, WID_None, m_pFrameWnd->GetID(), 
		150 , 202 , 120,  23, false );
	EditItemCreateNum->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
#ifdef UNICODE_ROCK
	EditItemCreateNum->SetTextSize( 6 );
#else 
	EditItemCreateNum->SetTextSize( 3 );
#endif 
	
	EditItemCreateNum->SetClientImage( TID_None );
	EditItemCreateNum->SetClientColor( 255, 255, 255 );
	EditItemCreateNum->SetAlignText( n_atLeft, n_atCenter );
	EditItemCreateNum->SetBlank( 2, 0, 0, 0 );
	EditItemCreateNum->SetEditStyle( esOnlyNumeric );
	EditItemCreateNum->SetReturnActionClear( false );
	EditItemCreateNum->SetText( _RT("") );						// ���� �̸��� �Է��ϼ���
	EditItemCreateNum->InitEditBox();
	
	///---------------------------------------------------------------------------	
	///-- Control
	///---------------------------------------------------------------------------	
	///-- ���� ��ư
	start_btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, m_pFrameWnd->GetID(), (mixEldaWnd.w/2) - 58, 241, 58, 25, false );
	start_btn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_CTRL_BTN58_DIS );
	start_btn->SetFontR( n_fRsvFontWndButton ); 
	start_btn->SetText( G_STRING_CHAR( IDS_WND_COM_START ) ); 
//	start_btn->SetBtnHold( true );

	///-- ���
	cancel_btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, m_pFrameWnd->GetID(), (mixEldaWnd.w/2)  , 241, 58, 25, false );
	cancel_btn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_CTRL_BTN58_DIS );	
	cancel_btn->SetFontR( n_fRsvFontWndButton ); 
	cancel_btn->SetText( G_STRING_CHAR( IDS_WND_COM_CANCEL ) ); 
	
	///-- None Item ����

	Init();
}


///---------------------------------------------------------------------------
///-- Update
///---------------------------------------------------------------------------
///-- �� ���� ���濡 ���� Upgrade ����� �����ش�.
void CEssenceWndProc::Update()
{
	///-- CheckItemList - Inven ���濡 ���� ��þƮ â�� ����
	CheckItemList();
}

void CEssenceWndProc::ClearPreItem()
{
	m_PreSlot[0]->PcItem->Clear();
	m_PreSlot[1]->PcItem->Clear();
	m_MaxItemCount = 0 ; 

}


//� �ƾ����� Ŭ�� �ߴ°� ? 
BOOL CEssenceWndProc::SetClickItem(SPcItem* PcItem)
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
			g_ItemSelectPotionWndProc.SetAddPcItem(&m_PrePcItem[0], (*ir).theAddPotion[0].theItemCode , 1 );
			ExChangeItemMaxCount = (*ir).theAddPotion[0].theBundle;

			//��� ������ 
			g_ItemSelectPotionWndProc.SetAddPcItem(&m_PrePcItem[1],  (*ir).theCreatePotion , 1 );	
			break; 		
		}
	}	
	
	//�Ѱ����� ã�ƶ�. 
	int MaxItemCount = 0 ; 
	
	for( j = 0; j < c_MaxInvenItemNum; ++j )
	{	
		if( nRui->GetPcInvenInfo()->InvenItem[j] == NULL )
		{
			continue;
		}
		///-- Elda slot
		if( m_PreSlot[0]->PcItem && 
			m_PreSlot[0]->PcItem->Code == nRui->GetPcInvenInfo()->InvenItem[j]->Code)
		{
			MaxItemCount += nRui->GetPcInvenInfo()->InvenItem[j]->Amount;
		}
	}
	
	char StrTmp[128]={0,};
	Rsprintf(RWCHAR(StrTmp) , _RT("%d") , MaxItemCount / ExChangeItemMaxCount );
	EditItemCreateNum->SetText( StrTmp);
	


	return FALSE;
}


///---------------------------------------------------------------------------
///-- CheckItemList - Inven ���濡 ���� ��þƮ â�� ����
///---------------------------------------------------------------------------
BOOL CEssenceWndProc::CheckItemList()
{
	int i = 0;
	int j = 0;
	char StrTmp[256] ={0,};
	char CreateitemStr[256] ={0,};

	

	///-----------------------------------------------------------------------
	///-- ��ü �κ��丮 Access
	int ItemCount = 0; 
	

	for( j = 0; j < c_MaxInvenItemNum; ++j )
	{	
		if( nRui->GetPcInvenInfo()->InvenItem[j] == NULL )
		{
			continue;
		}
		///-- Elda slot
		if( m_PreSlot[0]->PcItem && 
			m_PreSlot[0]->PcItem->Code == nRui->GetPcInvenInfo()->InvenItem[j]->Code)
		{
			ItemCount += nRui->GetPcInvenInfo()->InvenItem[j]->Amount;
		}
	}

	m_MaxItemCount = ItemCount;

	EditItemCreateNum->GetText(CreateitemStr  , 128);
	
	int MakeEssenceNum = Ratoi(RWCHAR(CreateitemStr));
	
	if( MakeEssenceNum != 0 && m_MaxItemCount != 0 )
	{
		int MakeItemCount = (MakeEssenceNum * ExChangeItemMaxCount) / m_MaxItemCount ; 
		//���� 
		Rsprintf(RWCHAR(StrTmp) , _RT("%d/%d") ,MakeEssenceNum * ExChangeItemMaxCount , m_MaxItemCount );
		ChangeGold[0]->SetText( StrTmp );	
		//�ʿ��� ��
		Rsprintf(RWCHAR(StrTmp) , _RT("%d") ,MakeEssenceNum * 50000 );
		ChangeGold[1]->SetText( StrTmp );
	}
	else 
	{
		
		ChangeGold[0]->SetText( _RT(""));	
		ChangeGold[1]->SetText( _RT("") );	
	}


		
	///-----------------------------------------------------------------------
	///-- �� Slot �� ��� Item �� �ִٸ� ���� �غ�
/*
	if( m_CurSlot[0]->PcItem->IType != n_ITYPE_NONE && 
		m_CurSlot[1]->PcItem->IType != n_ITYPE_NONE && 
		m_CurSlot[2]->PcItem->IType != n_ITYPE_NONE )
	{
		//if( start_btn->GetBtnState() != ) 
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
*/

	return FALSE;
}


//-----------------------------------------------------------------------------
void CEssenceWndProc::Proc( SEventMessage* EMsg )
{

	if(EMsg->FocusWnd == cancel_btn->GetID())
	{
		if(EMsg->Notify == NM_BTN_CLICK)
		{
			nRui->SendEvent(m_pFrameWnd->GetID() , n_emSetVisible , (EPARAM)false , 0 , 0 , 0 );
		}
	}      
	//��ŸƮ �ϸ� ��ư ��Ȱ��ȭ .. 
	if(EMsg->FocusWnd == start_btn->GetID() )
	{
		if(EMsg->Notify == NM_BTN_CLICK )
		{
			
			//�κ��丮�� disible ��Ų��. 
			char StrTmp[128]={0,};

			EditItemCreateNum->GetText(StrTmp , 128);
			int CurCount = Ratoi(RWCHAR(StrTmp) );
			
			SItemBaseInfo* item_Info = NULL;
			item_Info = g_Pc_Manager.GetItemBaseInfoPtr( m_PreSlot[1]->PcItem->Code );	
	
			if( CurCount > item_Info->theBundle ) 
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_NOTMAKE_OVERCOUNT),  item_Info->theBundle  );	
				return;
			}

			if( m_MaxItemCount	== 0 ) 
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_NEED_ITEM_COUNT),  item_Info->theBundle  );	
				return;
			}
			else if( CurCount > (m_MaxItemCount/10) )
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_CREATEITEM_OVERCOUNT) ,  item_Info->theBundle  );	
				return;
			}

			g_RockClient.Send_SRpCsMakeEssence(m_PreSlot[1]->PcItem->Code , CurCount  );
			Close();
		}
	}
}

///---------------------------------------------------------------------------
///-- ItemSlotDBClick
///---------------------------------------------------------------------------
///-- Item ����
void CEssenceWndProc::ItemSlotDBClick( SEventMessage* EMsg )
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

void CEssenceWndProc::ItemSlotClick( SEventMessage* EMsg )
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
		
		if ( srcslot->Group != n_SGROUP_INVENTORY )
		{	
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_OUT_INVENTORY_MOVE));			
			return;
		}

		if( targetslot->PcItem && (targetslot->PcItem == srcslot->PcItem) )
		{
			nRui->ResetCursorItem();
		}
	}
}

