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
#include "ItemSelectEldaMixWndProc.h"
#include "StringManager.h"
#include "..\\..\\VFileSystem.h"

#include "ItemEldaMixAWndProc.h"
#include "InventoryWndProc.h"

CItemSelectEldaMixWndProc	g_ItemSelectEldaMixWndProc;

//CFontg*				l_Fontg;	
//extern CFontman*	Fontman ;
extern	long	g_nowTime;

const int POTION_START = 7111;  
const int POTION_END = 7115;

const int ELDA_STONE_START = 6201;
const int ELDA_STONE_END = 6310;


const int ITEMSLOT_WIDTH = 5;
const int ITEMSLOT_HEIGHT = 3 ;
const int ITEMSLOT_MAX  = 15;


//-----------------------------------------------------------------------------
void CItemSelectEldaMixWndProc::Init()
{
	for(int i =0 ; i < 11; ++i)
	{
		m_TableEldaCode[i] = ELDA_STONE_START+(i*10);
	}
	

}


BOOL CItemSelectEldaMixWndProc::SetAddPcItem(CSlot *Slot , int SlotNum , int AddItemNum ,BOOL flag)
{
	SItemBaseInfo* item_Info = NULL;
	int Code =0 ;
	int Bundle = 1;
	//���������� 
	// flag == false �̸����� ������ true ��������� 

	
	Code = m_TableEldaCode[m_EldaItemInfo2[SlotNum].EldaStone[AddItemNum]];


	
	item_Info = g_Pc_Manager.GetItemBaseInfoPtr(Code);


	if(item_Info == NULL)
	{
		return false;
	}
	
	
	//�̸����� ������ �����Ѵ�.. 
	if(!flag)
	{
		Slot->theSlotState = n_Slot_Disable;
	}
	
	Slot->PcItem->Class = g_Pc_Manager.GetItemClsss( Code  );
	Slot->PcItem->ItemUID = 1;				
	Slot->PcItem->IType	= n_ITYPE_UNUSE;
	if( g_Pc_Manager.GetItemClsss( Code ) == n_Class_Use )
	{
		Slot->PcItem->IType	= n_ITYPE_USE;//�Ҹ�ǰ
	}
	else if( g_Pc_Manager.GetItemClsss( Code) == n_Class_Useless )
	{
		Slot->PcItem->IType	= n_ITYPE_UNUSE;//��Ҹ�ǰ
	}
	else if( g_Pc_Manager.GetItemClsss( Code  ) == n_Class_Special || 
		g_Pc_Manager.GetItemClsss( Code  ) == n_Class_Eldastone  ||
		g_Pc_Manager.GetItemClsss( Code  ) == n_Class_Weapon_StrengthStone ||
		g_Pc_Manager.GetItemClsss( Code  ) == n_Class_Armor_StrengthStone)
	{
		Slot->PcItem->IType	= n_ITYPE_SPECIAL;
	}				
	Slot->PcItem->Amount = Bundle;
	Slot->PcItem->Code	= Code ;			
	Slot->PcItem->Tid	= (TEXID)g_Pc_Manager.GetItemTID(  Code   );
	Slot->PcItem->ItemTable	= item_Info;		
				
	return true;
				
}
			
void CItemSelectEldaMixWndProc::Open()
{
	
	ClearPreItem();

	//���Կ� �������� �߰��Ѵ�.
	for(int i =0 ; i < m_iTableCount ; ++i)
	{
		SetAddPcItem(m_CurSlot[i] , i , 3 , true );
	}	
	start_btn->SetBtnHold( true );
	cancel_btn->SetBtnHold( false );
	
	nRui->SendEvent( m_pFrameWnd->GetID() , n_emSetVisible, (EPARAM)true, 0, 0, 0 );


}
///---------------------------------------------------------------------------
///-- CloseEnchantWnd
///---------------------------------------------------------------------------
///-- ���� â�� �ݴ´�.
///-- �� Resource �� �����Ѵ�. ( Dynamic Texture_M �� �����Ǿ�� �� ) 
void CItemSelectEldaMixWndProc::Close()
{

	g_ItemEldaMixAWndProc.Close();
	if(m_pFrameWnd != NULL)
	{
		nRui->SendEvent( m_pFrameWnd->GetID(), n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	}
	
	///TID_ELD_WND_UpGrade ����
}
///---------------------------------------------------------------------------
///-- Composition
///---------------------------------------------------------------------------
void CItemSelectEldaMixWndProc::Composition()
{
	CImageBox*		img;
	CButton*		btn = NULL;
	CTextBox*		tbox = NULL;

	
	int center_x	= nRui->Desktop.Width / 2;
	int center_y	= nRui->Desktop.Height / 2;


	SRect mixEldaWnd;
	mixEldaWnd.w	= 205;
	mixEldaWnd.h	= 207;
	mixEldaWnd.x	= 110;//center_x - ( mixEldaWnd.w / 2 );
	mixEldaWnd.y	= 350;//center_y - ( mixEldaWnd.h / 2 );
	
	///-- Frame Wnd
	m_pFrameWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_None, WID_None, mixEldaWnd.x, mixEldaWnd.y, mixEldaWnd.w, mixEldaWnd.h, false );	
	m_pFrameWnd->SetWndTexStyle( fwfTex_Style_Tile9 );
	m_pFrameWnd->SetClientImage( TID_CTRL_WND_L1 );
	m_pFrameWnd->SetIconTID( TID_CTRL_ICON_INVEN );	
	m_pFrameWnd->SetWndTileSizeLR( 40, 29 );
	m_pFrameWnd->SetWndTileSizeTB( 31, 11 );

	m_pFrameWnd->SetCaption(G_STRING_CHAR(IDS_WND_SELECT_ITEM) );							
	m_pFrameWnd->SetTitle( true );	
	m_pFrameWnd->SetWndProc( this );
	m_pFrameWnd->SetVisible( false );
	m_pFrameWnd->Initialize();

	///---------------------------------------------------------------------------	
	///-- Static
	///---------------------------------------------------------------------------		
	///-- Line


	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pFrameWnd->GetID(), 1, 159, 202, 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );
	
	int sx = 10  , sy = 35;

	for(int i =0 ; i < 3 ;  ++i )
	{
		for(int j= 0 ; j <5; ++j )	
		{
			img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pFrameWnd->GetID(), sx+(j*37), sy+(i*37 ), 35, 35, false );       
			img ->SetClientImage( TID_CTRL_SLOT_SUNKEN );
			m_CurSlot[(i*5)+j] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None , m_pFrameWnd->GetID() , sx+(j*37), sy+(i*37 ), 35, 35, false );	
			m_CurSlot[(i*5)+j]->SetSlotGroup( n_SGROUP_ELDA_MIX_PRE0 );
			m_CurSlot[(i*5)+j]->SetHID( HID_ITEMTIP );
			m_CurSlot[(i*5)+j]->SetSlotItem( &m_CurPcItem[i*5+j]);
		
		}
	}


	///---------------------------------------------------------------------------	
	///-- Control
	///---------------------------------------------------------------------------	
	///-- ���� ��ư
	
	start_btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, m_pFrameWnd->GetID(), 31, 169, 58, 25, false );
	start_btn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_CTRL_BTN58_DIS );
	start_btn->SetFontR( n_fRsvFontWndButton ); 
	start_btn->SetText( G_STRING_CHAR( IDS_WND_COM_START ) ); 
	start_btn->SetBtnHold( true );
	start_btn->SetVisible(false);


	///-- ���
	cancel_btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, m_pFrameWnd->GetID(), 119, 169, 58, 25, false );
	cancel_btn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_CTRL_BTN58_DIS );	
	cancel_btn->SetFontR( n_fRsvFontWndButton ); 
	cancel_btn->SetText( G_STRING_CHAR( IDS_WND_COM_CANCEL ) ); 
	
	///-- None Item ����

	CreateLoadTable();
	Init();

}

void CItemSelectEldaMixWndProc::ClearPreItem()
{
	int i =0 ;

	for( i = 0; i < ITEMSLOT_MAX ; ++i)
	{
		m_CurSlot[i]->PcItem->Clear();
	}

}


///---------------------------------------------------------------------------
///-- Update
///---------------------------------------------------------------------------
///-- �� ���� ���濡 ���� Upgrade ����� �����ش�.
void CItemSelectEldaMixWndProc::Update()
{


	//�����ϸ� ��� ��ư�� ��Ȱ��ȭ.. ��Ų��. 
	if(mb_button_lock)
	{
		start_btn->SetBtnHold(true);
		cancel_btn->SetBtnHold(true);
		nRui->ResetCursorItem();
		return;
	}
	
}



BOOL CItemSelectEldaMixWndProc::CreateLoadTable()
{
		
	///-- File Size Check
	char FileName[128] = "";

	sprintf( FileName, "MixElda.dat");
	
	///-- File Open
	VFileHandle* pFH = g_VFEtc.OpenFile( FileName );
	if( pFH == NULL ) 
	{
		MessageBox( NULL, _T("!MixElda.dat File Not Found!!!"), _T("�˸�"), MB_OK );
		return false;
	}

	int li_file_size = pFH->GetSize();
	
	///-- ������ ���Լ� Check
	if( li_file_size % sizeof( SEldaItemInfo2 ) != 0 )
	{
		return( false );
	}
	
	///-- Set Count
	m_iTableCount  = li_file_size / sizeof( SEldaItemInfo2 );

	///-- read data
	m_EldaItemInfo2 = SAFE_NEW_ARRAY( SEldaItemInfo2 , m_iTableCount );	
	if( m_EldaItemInfo2 == NULL )
	{
		return FALSE;
	}
	int i =0 ;

	for( i = 0; i < m_iTableCount; ++i )
	{
		///-- Read Header
		pFH->Read( ( void * )&m_EldaItemInfo2[i], sizeof( SEldaItemInfo2 ));	
	}

	g_VFEtc.CloseFile( pFH );


	return TRUE ;

}


//-----------------------------------------------------------------------------
void CItemSelectEldaMixWndProc::Proc( SEventMessage* EMsg )
{

	if(EMsg->FocusWnd == cancel_btn->GetID())
	{
		if(EMsg->Notify == NM_BTN_CLICK)
		{
			g_ItemEldaMixAWndProc.Close();
			nRui->SendEvent(m_pFrameWnd->GetID() , n_emSetVisible , (EPARAM)false , 0 , 0 , 0 );
		}
	}

	for(int i =0 ; i < ITEMSLOT_MAX ; ++i)
	{
		if(EMsg->FocusWnd == m_CurSlot[i]->GetID()&& m_CurSlot[i]->PcItem->ItemTable != NULL )
		{
			if( EMsg->Notify == NM_SLOT_CLICK)
			{
				//�ິ�� â�� ����.
				if( !g_ItemEldaMixAWndProc.IsVisible)
				{
				
					SRect Rect;
					m_pFrameWnd->GetClientSize(&Rect);
//					g_ItemEldaMixAWndProc.m_pFrameWnd->Reposition(Rect.x +Rect.w ,Rect.y);
//					g_ItemEldaMixAWndProc.Open();
//					g_ItemEldaMixAWndProc.m_pFrameWnd->GetClientSize(&Rect);

					
					//�κ��� �ȶ������� ?
					if(!InventoryWnd.IsVisible)
					{
						InventoryWnd.FrameWnd->Reposition(Rect.x + Rect.w   ,Rect.y );
						nRui->SendEvent(WID_InventoryWnd , n_emSetVisible , (EPARAM)true , 0 , 0 , 0 );				
						
					}


				}
				//���� � �������� Ŭ���߳� �ິâ�� ������ �Ѱ� ��� �Ѵ�. 
//				g_ItemEldaMixAWndProc.SetClickItem(m_CurSlot[i]->PcItem);
				
			
			}
		}
	}

	//��ŸƮ �ϸ� ��ư ��Ȱ��ȭ .. 
	if(EMsg->FocusWnd == start_btn->GetID() )
	{
		if(EMsg->Notify == NM_BTN_CLICK && !mb_button_lock )
		{
			mb_button_lock = true;
			//�κ��丮�� disible ��Ų��. 
			nRui->ResetCursorItem();
			
		}

	}

	if(mb_button_lock)
	{
		start_btn->SetBtnHold( true );
		cancel_btn->SetBtnHold( true);
	}


}
