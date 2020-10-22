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
#include "ItemSelectPotionWndProc.h"
#include "StringManager.h"
#include "..\\..\\VFileSystem.h"

#include "ItemSkillPotionWndProc.h"
#include "InventoryWndProc.h"
#include "EssenceWndProc.h"
#include "SpellStoneWndProc.h"

CItemSelectPotionWndProc	g_ItemSelectPotionWndProc;

//CFontg*				l_Fontg;	
//extern CFontman*	Fontman ;
extern	long	g_nowTime;


CItemSelectPotionWndProc::CItemSelectPotionWndProc()
{
	m_DataTable = NULL;
}


CItemSelectPotionWndProc::~CItemSelectPotionWndProc()
{
	SAFE_DELETE_ARRAY(m_DataTable);
}

//-----------------------------------------------------------------------------
void CItemSelectPotionWndProc::Init()
{

	m_SetMode = n_SkillPotion;
	m_StartSlotNum = 0 ; 
	
}

void CItemSelectPotionWndProc::SetWheelValue( short _wheel )
{
	int value = 0;
	
	ScrollBar->GetValue( &value );
	if( ( _wheel < 0 ) && ( value < 7 ) )
	{
		value += 1;
	}
	else if( ( _wheel > 0 ) && ( value > 0 ) )
	{
		value -= 1;
	}
	
	ScrollBar->SetValue( value );
	SetSlotScroll( value );
}

void CItemSelectPotionWndProc::SetSlotScroll(int linenum )
{

	m_StartSlotNum = linenum * 5;
	
	for( int i = 0; i < ITEMSLOT_MAX; ++i )
	{
		m_CurSlot[i]->PcItem = &m_CurPcItem[m_StartSlotNum + i];
	}
}




BOOL CItemSelectPotionWndProc::SetAddPcItem(SPcItem *PcItem , int Code , int ItemNum )
{

	SItemBaseInfo* item_Info = NULL;
	
	item_Info = g_Pc_Manager.GetItemBaseInfoPtr(Code);

	if(item_Info == NULL)
	{
		return false;
	}
	
	PcItem->Class = g_Pc_Manager.GetItemClsss( Code  );
	PcItem->ItemUID = 1;				
	PcItem->IType	= n_ITYPE_UNUSE;

	if( g_Pc_Manager.GetItemClsss( Code ) == n_Class_Use )
	{
		PcItem->IType	= n_ITYPE_USE;//소모품
	}
	else if( g_Pc_Manager.GetItemClsss( Code) == n_Class_Useless )
	{
		PcItem->IType	= n_ITYPE_UNUSE;//비소모품
	}
	else if( g_Pc_Manager.GetItemClsss( Code  ) == n_Class_Special || 
		g_Pc_Manager.GetItemClsss( Code  ) == n_Class_Eldastone ||
		g_Pc_Manager.GetItemClsss( Code  ) == n_Class_Weapon_StrengthStone ||
		g_Pc_Manager.GetItemClsss( Code  ) == n_Class_Armor_StrengthStone )


	{
		PcItem->IType	= n_ITYPE_SPECIAL;
	}	
	
	PcItem->Amount = ItemNum;
	PcItem->Code	= Code ;			
	PcItem->Tid	= (TEXID)g_Pc_Manager.GetItemTID(  Code   );
	PcItem->ItemTable	= item_Info;	
	
	
	return true;
				
}
			
void CItemSelectPotionWndProc::Open(NSkillPotionType Set)
{
	//어느창모드로 들어온지 판단함 
	//비약창 , 봉인 , 파멸
	//코드값과 인덱스 값을 가지고 있어라 /? 

	m_SetMode = Set; 
	m_RealItem.clear();	
	int i=0;
	
	for( i=0; i < m_iTableCount ; ++i )
	{
		if( m_DataTable[i].theLogFlag == (int)m_SetMode ) 
		{
			switch(m_SetMode) 
			{
				//비약제조와 엘다스톤 가루는 나누지 않느다.
			case n_SkillPotion:
			case n_Eldastone_Powder:
				m_RealItem.push_back(m_DataTable[i]);
				break;
#ifdef C_SUBJOB_RENEW
			case n_Make_Essence:	m_RealItem.push_back(m_DataTable[i]);		break; 
			case n_Make_R_Strengthstone: m_RealItem.push_back(m_DataTable[i]);	break; 
			case n_Make_M_Strengthstone:m_RealItem.push_back(m_DataTable[i]);	break; 
#endif 
	
			default:
				SItemBaseInfo* item_Info = NULL;
				item_Info = g_Pc_Manager.GetItemBaseInfoPtr(m_DataTable[i].theCreatePotion);
				
				if(item_Info)
				{
					if( item_Info->theCitizen == nRui->thePcParam.Citizen)
					{
						m_RealItem.push_back(m_DataTable[i]);
					}
				}
				break;
			}
			
		}
	}
	
	//아이템 선택창에서 아이템을 누를시 
	//창이름을 미리 바꿔준다. 
	switch(m_SetMode)
	{
		case n_SkillPotion:	
			g_ItemSkillPotionWndProc.m_pFrameWnd->SetCaption(G_STRING_CHAR(IDS_WND_MAKE_POTOIN)); break; 
		case n_Ruined_Interpid_Item	:
		case n_Ruined_Wrath_item	:
		case n_Sealed_Interpid_Item	:
		case n_Sealed_Wrath_item	:
			g_ItemSkillPotionWndProc.m_pFrameWnd->SetCaption(G_STRING_CHAR(IDS_WND_SEALED_CENCEL)); 
			break; 
			
	}
		
	
	ClearPreItem();

	if( m_RealItem.size() > ITEM_PCITEM_MAX )
	{
		MessageBox(NULL , "insert Item 60 overflow" , "" , MB_OK );
	}

	//아이템 정보를 만든다. 
	for( i =0 ; i < m_RealItem.size() ; ++i)
	{
		SetAddPcItem( &m_CurPcItem[i] , m_RealItem[i].theCreatePotion , 1 );
	}	

	start_btn->SetBtnHold( true );
	cancel_btn->SetBtnHold( false );
	
	nRui->SendEvent( m_pFrameWnd->GetID() , n_emSetVisible, (EPARAM)true, 0, 0, 0 );


}
///---------------------------------------------------------------------------
///-- CloseEnchantWnd
///---------------------------------------------------------------------------
///-- 개조 창을 닫는다.
///-- 각 Resource 를 해제한다. ( Dynamic Texture_M 와 연동되어야 함 ) 
void CItemSelectPotionWndProc::Close()
{
	
	g_ItemSkillPotionWndProc.Close();
	if(m_pFrameWnd != NULL)
	{
		nRui->SendEvent( m_pFrameWnd->GetID(), n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	}
	
	///TID_ELD_WND_UpGrade 해제
}
///---------------------------------------------------------------------------
///-- Composition
///---------------------------------------------------------------------------
void CItemSelectPotionWndProc::Composition()
{
	CImageBox*		img;
	CButton*		btn = NULL;
	CTextBox*		tbox = NULL;

	
	int center_x	= nRui->Desktop.Width / 2;
	int center_y	= nRui->Desktop.Height / 2;


	SRect mixEldaWnd;
	mixEldaWnd.w	= 210;
	mixEldaWnd.h	= 270;
	mixEldaWnd.x	= 110;//center_x - ( mixEldaWnd.w / 2 );
	mixEldaWnd.y	= 350;//center_y - ( mixEldaWnd.h / 2 );
	
	///-- Frame Wnd
	m_pFrameWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_None, WID_None, center_x-400 , center_y-100 , mixEldaWnd.w, mixEldaWnd.h, false );	
	m_pFrameWnd->SetWndTexStyle( fwfTex_Style_Tile9 );
	m_pFrameWnd->SetClientImage( TID_CTRL_WND_L1 );
	m_pFrameWnd->SetIconTID( TID_CTRL_ICON_INVEN );	
	m_pFrameWnd->SetWndTileSizeLR( 40, 29 );
	m_pFrameWnd->SetWndTileSizeTB( 31, 11 );

	m_pFrameWnd->SetCaption( G_STRING_CHAR(IDS_WND_SELECT_ITEM) );							
	m_pFrameWnd->SetTitle( true );	
	m_pFrameWnd->SetWndProc( this );
	m_pFrameWnd->SetVisible( false );
	m_pFrameWnd->Initialize();

	///---------------------------------------------------------------------------	
	///-- Static
	///---------------------------------------------------------------------------		
	///-- Line


	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pFrameWnd->GetID(), 1, 225, 202, 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );

	int sx = 10  , sy = 35;

	for(int i =0 ; i < ITEMSLOT_HEIGHT ;  ++i )
	{
		for(int j= 0 ; j <ITEMSLOT_WIDTH; ++j )	
		{
			img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pFrameWnd->GetID(), sx+(j*37), sy+(i*37 ), 35, 35, false );       
			img ->SetClientImage( TID_CTRL_SLOT_SUNKEN );
			m_CurSlot[(i*5)+j] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None , m_pFrameWnd->GetID() , sx+(j*37), sy+(i*37 ), 35, 35, false );	
			m_CurSlot[(i*5)+j]->SetSlotGroup( n_SGROUP_SKILL_POTION );
			m_CurSlot[(i*5)+j]->SetHID( HID_ITEMTIP );
			m_CurSlot[(i*5)+j]->SetSlotItem( &m_CurPcItem[i*5+j]);
		}
	}

	ScrollBar = (CScrollBar*)nRui->CreateWnd( n_wtScrollBar, WID_None, m_pFrameWnd->GetID() , 195, 30, 15, 195, false );
	ScrollBar->InitSlide( stVertSld, 0, 7, 0, 13 );//Type, min, max, value, 썸의 폭
	ScrollBar->SetClientImage( TID_CTRL_SCR_BG );
	ScrollBar->SetThumbImage( TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR_OVR, TID_CTRL_SCR_BAR_CLK, TID_None );
	ScrollBar->SetScrollBtnA( TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT_OVR, TID_CTRL_SCR2_UBT_CLK, TID_None );
	ScrollBar->SetScrollBtnB( TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT_OVR, TID_CTRL_SCR2_DBT_CLK, TID_None );



	///---------------------------------------------------------------------------	
	///-- Control
	///---------------------------------------------------------------------------	
	///-- 시작 버튼
	
	start_btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, m_pFrameWnd->GetID(), 31, 169, 58, 25, false );
	start_btn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_CTRL_BTN58_DIS );
	start_btn->SetFontR( n_fRsvFontWndButton ); 
	start_btn->SetText( G_STRING_CHAR( IDS_WND_COM_START ) ); 
	start_btn->SetBtnHold( true );
	start_btn->SetVisible(false);


	///-- 취소
	cancel_btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, m_pFrameWnd->GetID(), 119, 230, 58, 25, false );
	cancel_btn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_CTRL_BTN58_DIS );	
	cancel_btn->SetFontR( n_fRsvFontWndButton ); 
	cancel_btn->SetText( G_STRING_CHAR( IDS_WND_COM_CANCEL ) ); 
	
	///-- None Item 지정

	Init();

}

void CItemSelectPotionWndProc::ClearPreItem()
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
///-- 각 엘다 스톤에 따른 Upgrade 결과를 보여준다.
void CItemSelectPotionWndProc::Update()
{

	//조합하면 모든 버튼을 비활성화.. 시킨다. 
	if(mb_button_lock)
	{
		start_btn->SetBtnHold(true);
		cancel_btn->SetBtnHold(true);
		nRui->ResetCursorItem();
		return;
	}
	
}



BOOL CItemSelectPotionWndProc::CreateLoadTable()
{
		
	///-- File Size Check
	char FileName[128] = {0,};

	sprintf( FileName, "SkillPotion.dat");
	
	///-- File Open
	VFileHandle* pFH = g_VFEtc.OpenFile( FileName );
	if( pFH == NULL ) 
	{
		MessageBox( NULL, _T("!SkillPotion.dat File Not Found!!!"), _T("알림"), MB_OK );
		return false;
	}

	int li_file_size = pFH->GetSize();
	int size = sizeof( SSkillPotionTable );

	///-- 사이즈 정규성 Check
	if( li_file_size % sizeof( SSkillPotionTable ) != 0 )
	{
		return( false );
	}
	
	///-- Set Count
	m_iTableCount  = li_file_size / sizeof( SSkillPotionTable );

	int a = sizeof( SSkillPotionTable );
	///-- read data
	m_DataTable = SAFE_NEW_ARRAY( SSkillPotionTable , m_iTableCount );
	
	if( m_DataTable == NULL )
	{
		return FALSE;
	}
	int i =0 ;

	for( i = 0; i < m_iTableCount; ++i )
	{
		///-- Read Header
		pFH->Read( ( void * )&m_DataTable[i], sizeof( SSkillPotionTable ));	
	}

	g_VFEtc.CloseFile( pFH );


	return TRUE ;

}


//-----------------------------------------------------------------------------
void CItemSelectPotionWndProc::Proc( SEventMessage* EMsg )
{

	if(EMsg->FocusWnd == cancel_btn->GetID())
	{
		if(EMsg->Notify == NM_BTN_CLICK)
		{
			g_ItemSkillPotionWndProc.Close();
			nRui->SendEvent(m_pFrameWnd->GetID() , n_emSetVisible , (EPARAM)false , 0 , 0 , 0 );
		}
	}
	
	else if(EMsg->FocusWnd == ScrollBar->GetID() )
	{
		if( ( EMsg->Notify == NM_SLD_MOVEBAR ) || ( EMsg->Notify == NM_SLD_CLICKMOVEBAR ) || ( EMsg->Notify == NM_SLD_BTNMOVERBAR ) )
		{
			SetSlotScroll( (int)EMsg->fParam );
		}
	}
	
	for(int i =0 ; i < ITEMSLOT_MAX ; ++i)
	{
		if((EMsg->FocusWnd == m_CurSlot[i]->GetID()) && m_CurSlot[i]->PcItem->ItemTable != NULL)
		{
			if( EMsg->Notify == NM_SLOT_CLICK)
			{
				//약병을 창을 띄운다.
				switch(m_SetMode)
				{
				case n_Make_Essence:
					{
						int center_x	= nRui->Desktop.Width / 2;
						int center_y	= nRui->Desktop.Height / 2;
						
						
						SRect size;
						m_pFrameWnd->GetClientSize(&size);
						size.x += (size.w + 15);
						g_EssenceWndProc->m_pFrameWnd->Reposition( size.x , size.y );	
						g_EssenceWndProc->m_pFrameWnd->GetClientSize( &size );
						size.x += (size.w + 15);
						InventoryWnd.FrameWnd->Reposition( size.x, size.y );		
						
#ifdef WIDE_PAY_INVENTORY //유료인벤확장			
						InventoryWnd.Open();
#else
						nRui->SendEvent( WID_InventoryWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
#endif			
						nRui->SendEvent( WID_Warehouse_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
						
					}
					g_EssenceWndProc->Open();
					g_EssenceWndProc->SetClickItem(m_CurSlot[i]->PcItem);
					break;
				case n_Make_R_Strengthstone:
				case n_Make_M_Strengthstone:
					if( !g_SpellStoneWndProc->IsVisible )
					{
						SRect Rect;
						m_pFrameWnd->GetClientSize(&Rect);
						g_SpellStoneWndProc->m_pFrameWnd->Reposition(Rect.x +Rect.w ,Rect.y);
						g_SpellStoneWndProc->Open();
						g_SpellStoneWndProc->m_pFrameWnd->GetClientSize(&Rect);
						
						if(!InventoryWnd.IsVisible)
						{
							InventoryWnd.FrameWnd->Reposition(Rect.x + Rect.w   ,Rect.y );
							nRui->SendEvent(WID_InventoryWnd , n_emSetVisible , (EPARAM)true , 0 , 0 , 0 );				
						}
					}
					g_SpellStoneWndProc->SetClickItem(m_CurSlot[i]->PcItem);						
					break; 
				default:
					if( !g_ItemSkillPotionWndProc.IsVisible)
					{
						
						SRect Rect;
						m_pFrameWnd->GetClientSize(&Rect);
						g_ItemSkillPotionWndProc.m_pFrameWnd->Reposition(Rect.x +Rect.w ,Rect.y);
						g_ItemSkillPotionWndProc.Open();
						g_ItemSkillPotionWndProc.m_pFrameWnd->GetClientSize(&Rect);
						
						if(!InventoryWnd.IsVisible)
						{
							InventoryWnd.FrameWnd->Reposition(Rect.x + Rect.w   ,Rect.y );
							nRui->SendEvent(WID_InventoryWnd , n_emSetVisible , (EPARAM)true , 0 , 0 , 0 );				
						}
						
					}
					//현재 어떤 아이템을 클릭했냐 약병창에 정보를 넘겨 줘야 한다. 
					g_ItemSkillPotionWndProc.SetClickItem(m_CurSlot[i]->PcItem);
				    break;
				}
			
			
			}
		}
	}

	//스타트 하면 버튼 비활성화 .. 
	if(EMsg->FocusWnd == start_btn->GetID() )
	{
		if(EMsg->Notify == NM_BTN_CLICK && !mb_button_lock )
		{
			mb_button_lock = true;
			//인벤토리를 disible 시킨다. 
			nRui->ResetCursorItem();
			
		}

	}

	if(mb_button_lock)
	{
		start_btn->SetBtnHold( true );
		cancel_btn->SetBtnHold( true);
	}


}
