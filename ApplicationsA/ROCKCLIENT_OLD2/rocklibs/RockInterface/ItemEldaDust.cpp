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
#include "InventoryWndProc.h"
#include "ItemEldaDust.h"
#include "StringManager.h"


CItemEldaDustWndProc	g_ItemEldaDustWndProc;

CItemEldaDustWndProc::CItemEldaDustWndProc()
{
	m_FrameWnd = NULL;
	CurIndexInfo = 0;

}

CItemEldaDustWndProc::~CItemEldaDustWndProc()
{
	m_ItemData.clear();
}

//-----------------------------------------------------------------------------
void CItemEldaDustWndProc::Init()
{
	m_ItemData.clear();
}


void CItemEldaDustWndProc::Composition()
{
	
	CImageBox*		img = NULL;
	CButton*		btn = NULL ;
	CTextBox*		tbox =NULL;
	SRect WndRect;
	
	int center_x	= nRui->Desktop.Width / 2;
	int center_y	= nRui->Desktop.Height / 2;

	int AddHeight = 54 ; 

	WndRect.w	= 254;
	WndRect.h	= 175;
	WndRect.x	= 520;//center_x - ( WndRect.w / 2 );
	WndRect.y	= 284;//center_y - ( WndRect.h / 2 );

	///-- Frame Wnd
	m_FrameWnd = (CFrameWnd *)nRui->CreateWnd( n_wtFrameWnd, WID_None , WID_None, 
		WndRect.x, WndRect.y , WndRect.w, WndRect.h ,  false );
	m_FrameWnd->SetWndTexStyle( fwfTex_Style_Tile9 );
	m_FrameWnd->SetClientImage( TID_CTRL_WND_L1 );		
	m_FrameWnd->SetWndTileSizeLR( 40, 29 );
	m_FrameWnd->SetWndTileSizeTB( 31, 11 );
	m_FrameWnd->SetMove( true );
	m_FrameWnd->SetWndProc( this );	
	m_FrameWnd->SetTitle( true );
	m_FrameWnd->SetVisible( false);
	m_FrameWnd->Initialize();
	m_FrameWnd->SetCaption(G_STRING_CHAR(IDS_WND_ELDASTONEPOWDER_CONVERSION) );

	SRect sTempRect;

	///-- Line
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_FrameWnd->GetID(), 
									   1, 139 , 252, 1 , false );
	img ->SetClientImage( TID_CTRL_LINEDOT );	
	

	char StrTemp[256] ={0,};
	
	///-- 아이템( Static Text )
	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_FrameWnd->GetID(), 75, 50, 171, 33 , false );
	tbox->SetFontR( n_fRsvFontWndStatic );
	tbox->SetAlignText(n_atCenter , n_atCenter  );
	Rsprintf(RWCHAR(StrTemp) , _RT("[%s]") ,G_STRING_CHAR(IDS_WND_ELDASTONEPOWDER) );
	tbox->SetText( StrTemp );	

	///-- 엘다스톤( Static Text )
	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_FrameWnd->GetID(), 75, 93, 171, 33, false );
	tbox->SetFontR( n_fRsvFontWndStatic );	
	tbox->SetAlignText(n_atCenter , n_atCenter  );
	Rsprintf(RWCHAR(StrTemp) , _RT("[%s]") ,G_STRING_CHAR(IDS_WND_HOLY_PROIMA) );
	tbox->SetText( StrTemp );	


	m_NeedEldTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_FrameWnd->GetID(), 1,  26 , 240, 22, false );
	m_NeedEldTxt->SetFontR( n_fRsvFontWndStatic );	
	m_NeedEldTxt->SetAlignText( n_atRight , n_atCenter );
	m_NeedEldTxt->SetText( G_STRING_CHAR(IDS_WND_EXPENSE) );

	///---------------------------------------------------------------------------	
	///-- Control
	///---------------------------------------------------------------------------	
	///-- 시작 버튼
	///-- 닫기
	m_ExitBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, m_FrameWnd->GetID(), 235, 6, 13, 13, false );
	m_ExitBtn->SetBtnImage( TID_CTRL_BTN_X, TID_CTRL_BTN_X_OVR, TID_CTRL_BTN_X_CLK, TID_None );

	///-- Slot
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_FrameWnd->GetID(), 
		51  , 50 , 35 , 35 , false );       
	img ->SetClientImage( TID_CTRL_SLOT_SUNKEN );
	
	///-- Slot
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_FrameWnd->GetID(), 
		51 , 93 , 35 , 35 , false );            
	img ->SetClientImage( TID_CTRL_SLOT_SUNKEN );

	///-- Slot
//	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_FrameWnd->GetID(), 
//		10 , 93 , 35 , 35 , false );            
//	img ->SetClientImage( TID_CTRL_SLOT_SUNKEN );

	
	///-- 아이템 Slot
	m_EldaDustSlot = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, m_FrameWnd->GetID(),  51, 50, 35, 35, false );	
	m_EldaDustSlot->SetSlotGroup( n_SGROUP_NONE );
	m_EldaDustSlot->SetSlotItem( &m_EldaDust_item);
	m_EldaDustSlot->SetHID(HID_ITEMTIP);

	
	///-- Elda Slot
	m_ProimaSlot = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, m_FrameWnd->GetID(), 51, 93, 35, 35, false );
	m_ProimaSlot->SetSlotGroup( n_SGROUP_NONE );
	m_ProimaSlot->SetSlotItem( &m_Proima_item );
	m_ProimaSlot->SetHID(HID_ITEMTIP);
	///-- Elda Slot
	m_PrevProimaSlot = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, m_FrameWnd->GetID(), 10 , 93, 35, 35, false );
	m_PrevProimaSlot->SetSlotGroup( n_SGROUP_NONE );
	m_PrevProimaSlot->SetSlotItem( &m_PrevProimaItem);
	m_PrevProimaSlot->SetHID(HID_ITEMTIP);
	m_PrevProimaSlot->theSlotState = n_Slot_Disable;


	m_StartBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, m_FrameWnd->GetID(), 48, 145, 58, 25, false );
	m_StartBtn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_CTRL_BTN58_DIS );
	m_StartBtn->SetFontR( n_fRsvFontWndButton ); 
	m_StartBtn->SetText( G_STRING_CHAR(IDS_WND_CONVERSION) ); 
	m_StartBtn->SetBtnHold( true );

	///-- 중단
	m_StopBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, m_FrameWnd->GetID(), 144, 145, 58, 25, false );
	m_StopBtn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_None );	
	m_StopBtn->SetFontR( n_fRsvFontWndButton ); 
	m_StopBtn->SetText( G_STRING_CHAR( IDS_WND_COM_CANCEL ) ); 


	Init();

}



void CItemEldaDustWndProc::Open()
{
	m_iTableCount = g_ItemSelectPotionWndProc.m_iTableCount;

	if( m_ItemData.size() == 0) 
	{
		for(int i =0; i < m_iTableCount ; ++i )
		{
			if( g_ItemSelectPotionWndProc.m_DataTable[i].theLogFlag == n_Eldastone_Powder ) 
			{
				m_ItemData.push_back(g_ItemSelectPotionWndProc.m_DataTable[i]);
			}
		}
	}

	mb_button_lock = false ; 
	m_NeedEldTxt->SetText( G_STRING_CHAR(IDS_WND_EXPENSE) );

	m_EldaDustSlot->PcItem->Clear();
	m_ProimaSlot->PcItem->Clear();
	m_PrevProimaSlot->PcItem->Clear();
	

	nRui->SendEvent( m_FrameWnd->GetID() , n_emSetVisible , (EPARAM)true , 0 ,0, 0  );

}

void CItemEldaDustWndProc::Close()
{

#ifdef C_SEALED_ELDASTONE_DUST
		nRui->SendEvent( m_FrameWnd->GetID() , n_emSetVisible , (EPARAM)false , 0 ,0, 0  );
#endif 

}


///---------------------------------------------------------------------------
///-- Update
///---------------------------------------------------------------------------
///-- 각 엘다 스톤에 따른 Upgrade 결과를 보여준다.
void CItemEldaDustWndProc::Update()
{
	CheckEnchantItemList();
}


//-----------------------------------------------------------------------------
void CItemEldaDustWndProc::Proc( SEventMessage* EMsg )
{

	if( m_ExitBtn->GetID() == EMsg->FocusWnd || 
		m_StopBtn->GetID() == EMsg->FocusWnd )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			nRui->SendEvent( m_FrameWnd->GetID() , n_emSetVisible, (EPARAM)false, 0, 0, 0 );
		}
	}
	else if(m_ProimaSlot->GetID() == EMsg->FocusWnd || m_EldaDustSlot->GetID() == EMsg->FocusWnd)
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
		if( m_EldaDustSlot->PcItem->IType != n_ITYPE_NONE && 
			m_ProimaSlot->PcItem->IType != n_ITYPE_NONE && 
			mb_button_lock)
		{
			m_StartBtn->SetBtnHold( false );				
		}
		else
		{
			m_StartBtn->SetBtnHold( true );
		}

	}
	else if(m_StartBtn->GetID() == EMsg->FocusWnd)
	{
		
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			
			if( m_ItemData[CurIndexInfo].theMakeMoney > nRui->GetPcParam()->Money  )
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_NOT_ENOUGH_ELD) );	
				return;
			}
			
			SBundleItem aEdlaStonePowder;
			SBundleItem aProima ;
			
			aEdlaStonePowder.theBundle = m_EldaDustSlot->PcItem->Amount;
			aEdlaStonePowder.theItemID = m_EldaDustSlot->PcItem->ItemUID;
			aProima.theBundle = m_ProimaSlot->PcItem->Amount;
			aProima.theItemID = m_ProimaSlot->PcItem->ItemUID;
			mb_button_lock = true;
						
			g_RockClient.Send_SRpCsMakeEldastone(m_ItemData[CurIndexInfo].theCreatePotion ,   aEdlaStonePowder , aProima	);
			Close();
		}

	}

}

///---------------------------------------------------------------------------
///-- CheckItemList - Inven 변경에 따른 인첸트 창의 변경
///---------------------------------------------------------------------------
BOOL CItemEldaDustWndProc::CheckEnchantItemList()
{
	int i = 0;
	int j = 0;
	
	BOOL IsFindItem = FALSE;
	BOOL isFineProima = FALSE;	
	
	///-----------------------------------------------------------------------
	///-- 전체 인벤토리 Access
	for( j = 0; j < c_MaxInvenItemNum; ++j )
	{	
		if( nRui->GetPcInvenInfo()->InvenItem[j] == NULL )
		{
			continue;
		}
		
		///-- item slot
		if( m_EldaDustSlot->PcItem && 
			m_EldaDustSlot->PcItem->ItemUID == nRui->GetPcInvenInfo()->InvenItem[j]->ItemUID )
		{
			IsFindItem = true;			
		}
		
		///-- Elda slot
		if( m_ProimaSlot->PcItem && 
			m_ProimaSlot->PcItem->ItemUID == nRui->GetPcInvenInfo()->InvenItem[j]->ItemUID )
		{
			isFineProima = true;
		}
		
	}
	
	///-----------------------------------------------------------------------
	///-- item 을 찾지 못 했을 경우
	if( !IsFindItem )
	{
		m_EldaDustSlot->PcItem->Clear();
	}
	///-- elda 를 찾지 못 했을 경우
	if( !isFineProima )
	{
		m_ProimaSlot->PcItem->Clear();
	}
		
	///-----------------------------------------------------------------------
	///-- 두 Slot 에 모두 Item 이 있다면 시작 준비
	if( m_EldaDustSlot->PcItem->IType != n_ITYPE_NONE && 
		m_ProimaSlot->PcItem->IType != n_ITYPE_NONE )
	{
		m_StartBtn->SetBtnHold( false );				
	}
	else
	{
		m_StartBtn->SetBtnHold( true );
	}
	
	return FALSE;
}

///---------------------------------------------------------------------------
///-- ItemSlotDBClick
///---------------------------------------------------------------------------
///-- Item 제거
void CItemEldaDustWndProc::ItemSlotDBClick( SEventMessage* EMsg )
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
///-- Item 올리기
///-- !!!! 원석과 스톤의 구분은 Hard Coding
void CItemEldaDustWndProc::ItemSlotClick( SEventMessage* EMsg )
{
	CSlot*		targetslot	= NULL;//현재 장비 슬롯
	CSlot*		srcslot		= NULL;//선택된(pick) 인벤 슬롯
	SPcItem*	tmpPcItem	= NULL;
	int			position	= 0;
	int i = 0 ; 
	char NeedMoney[256] = {0,};
	char TempStr[256] = {0,};


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
		
		if( targetslot->PcItem && (targetslot->PcItem == srcslot->PcItem) )
		{
			nRui->ResetCursorItem();
		}
		
		///-------------------------------------------------------------------
		///-- 엘다 가루일경우
		if( targetslot == m_EldaDustSlot )
		{
			if(  srcslot->Group == n_SGROUP_INVENTORY   )
			{
				bool IsEldaDustFine = false; 

				for( i =0 ; i < m_ItemData.size() ; ++i )
				{
					//엘다 가루가 테이블에 있는거냐 ? 
					if( m_ItemData[i].theAddPotion[0].theItemCode == srcslot->PcItem->Code ) 
					{
						IsEldaDustFine = true;
						if( srcslot->PcItem->Amount >= m_ItemData[i].theAddPotion[0].theBundle )
						{
							CurIndexInfo = i;
							///-- Copy item
							targetslot->SetSlotPcItem( *(srcslot->PcItem) );	
							targetslot->PcItem->Amount = m_ItemData[i].theAddPotion[0].theBundle;
							///-- Del Cursor
							//targetslot->PcItem = srcslot->PcItem;
							nRui->ResetCursorItem();		

							InventoryWnd.SetEldText(NeedMoney ,m_ItemData[i].theMakeMoney );
							Rsprintf(RWCHAR(TempStr),_RT("%s : %s"), G_STRING_CHAR(IDS_WND_EXPENSE), RWCHAR(NeedMoney) ); 
							m_NeedEldTxt->SetText(TempStr);
							
														
							PrevSetPcItem(m_PrevProimaSlot , m_ItemData[i].theAddPotion[1].theItemCode , m_ItemData[i].theAddPotion[1].theBundle , 0 );


							return ; 
						}
						else
						{
							SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_NOT_ENOUGH_VOLUME) );	
							return;
						}

					}
				}
				if( !IsEldaDustFine )
				{
					SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_MSG_PUTON_ELDAPOWDER) );	
					return;
				}
			
			}
		
		}
		///-------------------------------------------------------------------
		///-- 프로이마 
		else if( targetslot == m_ProimaSlot )
		{
			if( ( srcslot->Group == n_SGROUP_INVENTORY ))
			{	

				if( m_EldaDustSlot->PcItem->IType == n_ITYPE_NONE ) 
				{
					SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_WND_DUST_ON_FIRST) );	
					return;
				}
				
				//엘다 가루가 테이블에 있는거냐 ? 
				if( m_ItemData[CurIndexInfo].theAddPotion[1].theItemCode == srcslot->PcItem->Code ) 
				{
					
					if( srcslot->PcItem->Amount >= m_ItemData[CurIndexInfo].theAddPotion[1].theBundle )
					{
						///-- Copy item
						targetslot->SetSlotPcItem( *(srcslot->PcItem) );	
						targetslot->PcItem->Amount =  m_ItemData[CurIndexInfo].theAddPotion[1].theBundle;
						///-- Del Cursor
						//targetslot->PcItem = srcslot->PcItem;
						nRui->ResetCursorItem();
						return;
					}
					else
					{
						SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_NOT_ENOUGH_VOLUME) );	
						return;
					}
					
				}
				else 
				{
					SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_MSG_THISITEM_NOT_USE));	
					return;
				}
				
			}
		}
	}

}

void CItemEldaDustWndProc::PrevSetPcItem(CSlot *Slot , int Code , int ItemNum , bool SlotEnable)
{
	SItemBaseInfo* item_Info = NULL;
	item_Info = g_Pc_Manager.GetItemBaseInfoPtr(Code);
	
	if(item_Info == NULL)
	{
		return;
	}

	Slot->theSlotState = (N_SLOTSTATE)SlotEnable;
	Slot->PcItem->Class = g_Pc_Manager.GetItemClsss( Code  );
	Slot->PcItem->ItemUID = 1;				
	Slot->PcItem->IType	= n_ITYPE_UNUSE;
	
	if( g_Pc_Manager.GetItemClsss( Code ) == n_Class_Use )
	{
		Slot->PcItem->IType	= n_ITYPE_USE;//소모품
	}
	else if( g_Pc_Manager.GetItemClsss( Code) == n_Class_Useless )
	{
		Slot->PcItem->IType	= n_ITYPE_UNUSE;//비소모품
	}
	else if( g_Pc_Manager.GetItemClsss( Code  ) == n_Class_Special || 
		g_Pc_Manager.GetItemClsss( Code  ) == n_Class_Eldastone ||
		g_Pc_Manager.GetItemClsss( Code  ) == n_Class_Weapon_StrengthStone ||
		g_Pc_Manager.GetItemClsss( Code  ) == n_Class_Armor_StrengthStone)
		

	{
		Slot->PcItem->IType	= n_ITYPE_SPECIAL;
	}	
	
	Slot->PcItem->Amount = ItemNum;
	Slot->PcItem->Code	= Code ;			
	Slot->PcItem->Tid	= (TEXID)g_Pc_Manager.GetItemTID(  Code   );
	Slot->PcItem->ItemTable	= item_Info;	

}

void CItemEldaDustWndProc::LogOff()
{
	Close();
	m_ItemData.clear();
}

BOOL  CItemEldaDustWndProc::CheckItem(int iCode , int iBundle )
{

	return FALSE;
}