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
#include "NumberInputWnd.h"



#include "..\\..\\global.h"
#include "..\\..\\RockClient.h"

#include "..\\..\\bravolibs\\obj\\SkillSystem.h"

//#include "pc.h"F
#include "ItemRemodelWndProc.h"
#include "stringmanager.h"


CItemRemodel_WndProc	g_ItemRemodel_WndProc;
SRemodelStone * g_RemodelTable;
///---------------------------------------------------------------------------
///-- Constructor
///---------------------------------------------------------------------------
CItemRemodel_WndProc::CItemRemodel_WndProc()
{
	g_RemodelTable = NULL;
	mb_loadRemodelTable = false; 	
	mi_table_count = 0;
	
}
///---------------------------------------------------------------------------
///-- Destructror
///---------------------------------------------------------------------------
CItemRemodel_WndProc::~CItemRemodel_WndProc()
{
	SAFE_DELETE_ARRAY( g_RemodelTable );
}
//-----------------------------------------------------------------------------
void CItemRemodel_WndProc::Init()
{
}

///---------------------------------------------------------------------------
///-- Composition
///---------------------------------------------------------------------------
void CItemRemodel_WndProc::Composition()
{
	CFrameWnd*		fw;
	CTextBox*		tbox;
	CImageBox*		img;
	CButton*		btn;
	
	int center_x	= nRui->Desktop.Width / 2;
	int center_y	= nRui->Desktop.Height / 2;


	SRect enchantwnd;
	enchantwnd.w	= 204;
	enchantwnd.h	= 200;
	enchantwnd.x	= center_x - ( enchantwnd.w / 2 );
	enchantwnd.y	= center_y - ( enchantwnd.h / 2 );
	
	///-- Frame Wnd
	fw = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_SUB_REMODEL, WID_None, enchantwnd.x, enchantwnd.y, enchantwnd.w, enchantwnd.h, false );
	fw->SetWndTexStyle( fwfTex_Style_Tile9 );
	fw->SetClientImage( TID_CTRL_WND_L1 );
	fw->SetIconTID( TID_CTRL_ICON_INVEN );	
	fw->SetWndTileSizeLR( 40, 29 );
	fw->SetWndTileSizeTB( 31, 11 );
	
	fw->SetCaption( G_STRING_CHAR( IDS_WND_REMODEL ) );		
	fw->SetTitle( true  );	
	fw->SetWndProc( this );
	fw->SetVisible( false );
	fw->Initialize();
	
	///---------------------------------------------------------------------------	
	///-- Static
	///---------------------------------------------------------------------------	
	///-- Line
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_SUB_REMODEL, 1, 111, 202, 55, false );
	img->SetClientImage( TID_CTRL_BACK );
	img->SetFocusCheck( false );
	
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_SUB_REMODEL,  1, 110, 202, 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );
	
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_SUB_REMODEL, 1, 166, 202, 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );
	

	///-- Slot
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_SUB_REMODEL, 14, 56, 35, 35, false );             
	img ->SetClientImage( TID_CTRL_SLOT_SUNKEN );

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_SUB_REMODEL, 80, 56, 35, 35, false );         
	img ->SetClientImage( TID_CTRL_SLOT_SUNKEN );

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_SUB_REMODEL, 120, 56, 35, 35, false );         
	img ->SetClientImage( TID_CTRL_SLOT_SUNKEN );

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_SUB_REMODEL, 160, 56, 35, 35, false );           
	img ->SetClientImage( TID_CTRL_SLOT_SUNKEN );
	
	///-- + img
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_SUB_REMODEL, 56, 65, 19, 19, false );               
	img ->SetClientImage( TID_C_WND_ELDA_SB_PLUS );
	
	///-- 결과( Static Text )
	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_SUB_REMODEL, 45, 95, 110, 17, false );
	tbox->SetFontR( n_fRsvFontWndStatic );
	tbox->SetText( G_STRING_CHAR( IDS_WND_REMODEL_GUESS_ELDA ));
	
	///-- 원석( Static Text )
	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_SUB_REMODEL, 2, 36, 60, 17, false );
	tbox->SetFontR( n_fRsvFontWndStatic );
	tbox->SetText( G_STRING_CHAR( IDS_WND_ITEM_ROUGH_ELDA ));

	///-- 첨가물( Static Text )
	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_SUB_REMODEL, 98, 36, 80, 17, false );
	tbox->SetFontR( n_fRsvFontWndStatic );	
	tbox->SetText( G_STRING_CHAR( IDS_WND_ITEM_ADDITIVE ));
	
	///---------------------------------------------------------------------------	
	///-- Control
	///---------------------------------------------------------------------------	
	///-- X
	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_SUB_REMODEL_BT_CLOSE, WID_SUB_REMODEL, 185, 6, 13, 13, false );
	btn->SetBtnImage( TID_CTRL_BTN_X, TID_CTRL_BTN_X_OVR, TID_CTRL_BTN_X_CLK, TID_None );
	
	///-- 가공 버튼
	mcp_remodel_btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_SUB_REMODEL_BT_REM, WID_SUB_REMODEL, 31, 171, 58, 25, false );
	mcp_remodel_btn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_CTRL_BTN58_DIS );	
	mcp_remodel_btn->SetFontR( n_fRsvFontWndButton ); 
	mcp_remodel_btn->SetText( G_STRING_CHAR( IDS_WND_REMODEL_BT ) );	
	mcp_remodel_btn->SetBtnHold( true );
	
	///-- 중단 버튼
	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_SUB_REMODEL_BT_ABT, WID_SUB_REMODEL, 119, 171, 58, 25, false );	
	btn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_CTRL_BTN58_DIS );	
	btn->SetFontR( n_fRsvFontWndButton ); 
	btn->SetText( G_STRING_CHAR( IDS_WND_COM_CANCEL ) );	

	///-- 아이템 Slot
	mcp_item_slot[0] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_SUB_REMODEL_SLOT_OP1, WID_SUB_REMODEL, 80, 56, 35, 35, false );
	mcp_item_slot[0]->PcItem = &(mc_itemslot_item[0]);
	mcp_item_slot[0]->SetSlotGroup( n_SGROUP_TUNING );	

	mcp_item_slot[1] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_SUB_REMODEL_SLOT_OP2, WID_SUB_REMODEL, 120, 56, 35, 35, false );
	mcp_item_slot[1]->SetSlotGroup( n_SGROUP_TUNING );
	mcp_item_slot[1]->PcItem = &(mc_itemslot_item[1]);

	mcp_item_slot[2] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_SUB_REMODEL_SLOT_OP3, WID_SUB_REMODEL, 160, 56, 35, 35, false );
	mcp_item_slot[2]->SetSlotGroup( n_SGROUP_TUNING );
	mcp_item_slot[2]->PcItem = &(mc_itemslot_item[2]);
	
	mcp_elda_slot = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_SUB_REMODEL_SLOT_ORIN, WID_SUB_REMODEL, 14, 56, 35, 35, false );
	mcp_elda_slot->PcItem = &mc_eldaslot_item;
	mcp_elda_slot->SetSlotGroup( n_SGROUP_TUNING );
	
	
	///-- TEXT
	mcp_remodel_item = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_SUB_REMODEL_TEXT2, WID_SUB_REMODEL, 5, 123, 190, 14, false );
	mcp_remodel_item->SetFontR( n_fRsvFontWndStatic );	
	mcp_remodel_item->SetBlank( 0, 0 );	

	///-- TEXT
	mcp_remodel_Prob = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_SUB_REMODEL_TEXT1, WID_SUB_REMODEL, 5, 141, 190, 14, false );
	mcp_remodel_Prob->SetFontR( n_fRsvFontWndStatic );	
	mcp_remodel_Prob->SetBlank( 0, 0 );	
	
	
	if( !mb_loadRemodelTable )
	{
		LoadRemodelTable();
	}
}
///---------------------------------------------------------------------------
///-- OpenEnchantRWnd
///---------------------------------------------------------------------------
void CItemRemodel_WndProc::OpenRemodel_RWnd()
{
	int j = 0;
	int li_max  =0;
	static TCHAR tmpStr[128] = {0,};

	nRui->SendEvent( WID_SUB_REMODEL, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
	mcp_item_slot[0]->PcItem->Clear();
	mcp_item_slot[1]->PcItem->Clear();
	mcp_item_slot[2]->PcItem->Clear();
	mcp_elda_slot->PcItem->Clear();
	mcp_remodel_btn->SetBtnHold( true );
	mb_button_lock = false;

	
	///-- 조합 불가						
	Rsprintf( RWCHAR(tmpStr), _RT( "" ));
	mcp_remodel_item->SetText( tmpStr );
	mcp_remodel_Prob->SetText( tmpStr );
	
	
	///-- 전체 Skill 중에 개조 최고 레벨 찾음
	for( j = 0; j < RP_MAX_USER_SKILL; ++j )
	{	
		if( nRui->GetPcInvenInfo()->SkillItem[j] == NULL )
		{
			continue;
		}
		
		///-- Max Level 찾음
		if( nRui->GetPcInvenInfo()->SkillItem[j]->Code >= c_SkillCode_EldaRemodel_Min &&  nRui->GetPcInvenInfo()->SkillItem[j]->Code <= c_SkillCode_EldaRemodel_Max )
		{
			if( li_max < nRui->GetPcInvenInfo()->SkillItem[j]->Code - c_SkillCode_EldaRemodel_Min + 1 )
			{
				li_max = nRui->GetPcInvenInfo()->SkillItem[j]->Code - c_SkillCode_EldaRemodel_Min + 1;
			}
			
		}
	}

	mi_skill_level = li_max;

	
}

//-----------------------------------------------------------------------------
void CItemRemodel_WndProc::Update()
{
}

//-----------------------------------------------------------------------------
void CItemRemodel_WndProc::Proc( SEventMessage* EMsg )
{
	SBundleItem lc_item[3];
	int i;
	
	switch( EMsg->FocusWnd )
	{
		///-- 강화
	case WID_SUB_REMODEL_BT_REM:		
		if( EMsg->Notify == NM_BTN_CLICK && !mb_button_lock)
		{			
			if( mc_eldaslot_item.IType != n_ITYPE_NONE )
			{
				///-- Data Copy
				for( i = 0 ; i < 3; ++i )
				{
					if( mc_itemslot_item[i].IType == n_ITYPE_NONE )
					{
						lc_item[i].theItemID = 0;
						lc_item[i].theBundle = 0;
					}
					else
					{						
						lc_item[i].theItemID = mc_itemslot_item[i].ItemUID;
						lc_item[i].theBundle = mc_itemslot_item[i].Amount;
					}
				}
			}
			//g_RockClient.Send_CsRemodelStone( mc_eldaslot_item.ItemUID, lc_item);			
			nRui->SendEvent( WID_SUB_REMODEL, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
			mb_button_lock = true;
		}
		break;
		
	case WID_SUB_REMODEL_BT_ABT:
	case WID_SUB_REMODEL_BT_CLOSE:

		if( EMsg->Notify == NM_BTN_CLICK )    
		{
			nRui->SendEvent( WID_SUB_REMODEL, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
			IsVisible = false;
		}
		break;
		
	case WID_SUB_REMODEL_SLOT_OP1:
	case WID_SUB_REMODEL_SLOT_OP2:
	case WID_SUB_REMODEL_SLOT_OP3:
	case WID_SUB_REMODEL_SLOT_ORIN:
		{
			if( EMsg->Notify == NM_SLOT_CLICK )			{	ItemSlotClick( EMsg );		}
			else if( EMsg->Notify == NM_SLOT_DBCLICK )	{	ItemSlotDBClick( EMsg );	}		
					
			///-- 가공 가능한지 Check
			CheckRemodelEnable();
		}
		break;
		
	}
	
	
}
///---------------------------------------------------------------------------
///-- CloseNumberInputWnd
///---------------------------------------------------------------------------
void CItemRemodel_WndProc::CloseNumberInputWnd( int aNumItem )
{
	if( !mcp_temp_num_src_slot )
	{
		return;
	}

	if( mcp_temp_num_src_slot->PcItem )
	{
		if( mcp_temp_num_src_slot->PcItem->Amount >= aNumItem )
		{
			mcp_temp_num_tga_slot->SetSlotPcItem( *(mcp_temp_num_src_slot->PcItem) );				
			mcp_temp_num_tga_slot->PcItem->Amount = aNumItem;
		}
		else
		{
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOT_ENOUGH_VOLUME ) );
		}
	}

	///-- 가공 가능한지 Check
	CheckRemodelEnable();
	return;
}
///---------------------------------------------------------------------------
///-- GetProbability
///---------------------------------------------------------------------------
void CItemRemodel_WndProc::GetProbability()
{
	int j;
	int li_max = 0;
	int li_prob;
	TCHAR tmpStr[128] = {0,};
	
	///-- 아이템 없을 경우 0
	 if( mcp_elda_slot->PcItem->IType == n_ITYPE_NONE )
	 {
		 Rsprintf( RWCHAR(tmpStr), _RT( "" ));	
		 mcp_remodel_Prob->SetText( tmpStr );
	 }
	///-----------------------------------------------------------------------
	///-- 전체 인벤토리 Access
	for( j = 0; j < RP_MAX_USER_SKILL; ++j )
	{	
		if( nRui->GetPcInvenInfo()->SkillItem[j] == NULL )
		{
			continue;
		}
		
		///-- Max Level 찾음
		if( nRui->GetPcInvenInfo()->SkillItem[j]->Code >= c_SkillCode_EldaRemodel_Min &&  nRui->GetPcInvenInfo()->SkillItem[j]->Code <= c_SkillCode_EldaRemodel_Max )
		{
			if( li_max < nRui->GetPcInvenInfo()->SkillItem[j]->Code - c_SkillCode_EldaRemodel_Min +1)
			{
				li_max = nRui->GetPcInvenInfo()->SkillItem[j]->Code - c_SkillCode_EldaRemodel_Min +1;
			}
			
		}
	}
	
	
	///-- 결과확률 Text
	li_prob = 70 - (  mcp_elda_slot->PcItem->ItemTable->theGrade -  li_max ) * 10;
		
	if( li_prob >100 )
	{
		li_prob = 100;
	}

	if( li_prob < 0 )
	{
		li_prob = 0;
	}

	Rsprintf( RWCHAR(tmpStr), G_STRING_CHAR( IDS_WND_REMODEL_PROB ), li_prob);	
	mcp_remodel_Prob->SetText( tmpStr );
				
				
}
///---------------------------------------------------------------------------
///-- CheckExtract_ItemList
///---------------------------------------------------------------------------
///-- 가공 가능한지 Check 하고 Button 을 활성화 한다.
void CItemRemodel_WndProc::CheckRemodelEnable()
{
	int i, j, k;
	bool lb_match[3] = { false};
	bool lb_ck_table[3] = { false,};
	int li_suc_percnet = 0;

	TCHAR tmpStr[128] = {0,};

		

	if ( !mb_loadRemodelTable )
	{
		return;
	}

	///-- 전체 Table Access
	for( i = 0 ; i < mi_table_count ; ++i )
	{
		///-- 원석이 동일한 제조를 비교한다.
		if ( mcp_elda_slot->PcItem->IType != n_ITYPE_NONE && mcp_elda_slot->PcItem->Code == g_RemodelTable[i].theGemStoneCode ) 
		{
			///-- flag 초기화
			lb_match[0] = lb_match[1] = lb_match[2] = false;
			lb_ck_table[0] = lb_ck_table[1] = lb_ck_table[2] =false; 

			///-- 아이템 슬롯 Access
			for( j = 0 ; j < 3 ; ++j )
			{
				///-- 제조 Table 
				for( k = 0 ; k < 3 ; ++k )
				{
					///-- 빈 Slot
					if( mcp_item_slot[j]->PcItem->IType == n_ITYPE_NONE)
					{
						if( g_RemodelTable[i].theAddItem[k].theItemCode == 0 && !lb_ck_table[k])
						{
							lb_match[j] = true;
							lb_ck_table[k] = true;
							k = 4;
						}
					}
					///-- 아이템이 있는 Slot
					else
					{
						if( mcp_item_slot[j]->PcItem->Code == g_RemodelTable[i].theAddItem[k].theItemCode &&
							mcp_item_slot[j]->PcItem->Amount == g_RemodelTable[i].theAddItem[k].theBundle &&
							!lb_ck_table[k])
						{
							lb_match[j] = true;
							lb_ck_table[k] = true;
							k = 4;
						}
					}
				}///-- end of for Table Slot				
			}///-- end of for Item Slot
			
			///---------------------------------------------------------------
			///-- 조합가능 , 버튼 및 Text Set
			if( lb_match[0] && lb_match[1] && lb_match[2] )
			{
				///-- Button
				mcp_remodel_btn->SetBtnHold( false );
				//li_suc_percnet = 


				///-- 결과 Stone
				SItemBaseInfo* item_Info = NULL;
				item_Info = g_Pc_Manager.GetItemBaseInfoPtr( g_RemodelTable[i].theStoneCode );
				if( !item_Info )
				{
					MessageBox( NULL, "Critical Error: Missiong EldaStone From Remodel Table", NULL, NULL);
				}

				///-- 결과 item Text
				Rsprintf( RWCHAR(tmpStr), _RT( "%s" ), RWCHAR(item_Info->theName) );	
				mcp_remodel_item->SetText( tmpStr );
				

				///-- 성공 확률
				GetProbability();
				nRui->Refresh_ItemHelpTip();
				return;
			}
		}///-- End of if 원석 동일
	}///-- 전체Table Access 끝
	
	
	///-- 조합 불가						
	Rsprintf(RWCHAR(tmpStr), _RT( "" ));
	mcp_remodel_item->SetText( tmpStr );
	mcp_remodel_Prob->SetText( tmpStr );
	
	mcp_remodel_btn->SetBtnHold( true );
}
///---------------------------------------------------------------------------
///-- CheckExtract_ItemList
///---------------------------------------------------------------------------
BOOL CItemRemodel_WndProc::CheckRemodel_ItemList()
{
	int i = 0;
	int j = 0;
	
	bool lb_isFind_item[3] = { FALSE, FALSE, FALSE };
	bool lb_isFind_elda = FALSE;
	
	
	if( !mcp_item_slot[0]->PcItem && !mcp_item_slot[1]->PcItem && !mcp_item_slot[2]->PcItem && !mcp_elda_slot->PcItem ) 
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
		
		///-- Item Check
		for( i = 0; i < 3; i++)
		{
			///-- item slot
			if( mcp_item_slot[i]->PcItem && mcp_item_slot[i]->PcItem->ItemUID == nRui->GetPcInvenInfo()->InvenItem[j]->ItemUID )
			{
				lb_isFind_item[i] = true;

				///-- 수량 체크
				if( mcp_item_slot[i]->PcItem->Amount > nRui->GetPcInvenInfo()->InvenItem[j]->Amount )
				{
					mcp_item_slot[i]->PcItem->Clear();
				}
			}
		}
		
		///-- Elda slot
		if( mcp_elda_slot->PcItem && mcp_elda_slot->PcItem->ItemUID == nRui->GetPcInvenInfo()->InvenItem[j]->ItemUID )
		{
			lb_isFind_elda = true;
		}
	}
	
	///-----------------------------------------------------------------------
	///-- item 을 찾지 못 했을 경우
	///-- Item Check
	for( i = 0; i < 3; i++)
	{
		///-- item slot
		if( !lb_isFind_item[i] )
		{
			mcp_item_slot[i]->PcItem->Clear();
		}
	}

	///-- elda 를 찾지 못 했을 경우
	if( !lb_isFind_elda )
	{
		mcp_elda_slot->PcItem->Clear();
	}
	
	CheckRemodelEnable();

	/*
	if( isFind )
	{
	if( mcp_item_slot->PcItem->IType == n_ITYPE_USE )
		{
			// 실제 가지고 있는것 보다 많이 교환하려구 하면...교환창에 넣고 아이템 떨군게야...ㅡㅡ; 사기꾼!!! 
			if( mcp_item_slot->PcItem->Amount > nRui->GetPcInvenInfo()->InvenItem[j]->Amount )
			{
				mcp_item_slot->PcItem = NULL;
				//tslot->PcItem = NULL;//->Clear();
				return TRUE;
			}
		}			
	}
	else
	{
		mcp_item_slot->PcItem = NULL;
		return TRUE;
	}*/

	return FALSE;
}


///---------------------------------------------------------------------------
///-- ItemSlotDBClick
///---------------------------------------------------------------------------
///-- Item 제거
void CItemRemodel_WndProc::ItemSlotDBClick( SEventMessage* EMsg )
{
	CSlot*		tslot	= NULL;	
	CSlot*		sslot	= NULL;
	
	tslot = (CSlot*)Container->Get( (WNDID)EMsg->FocusWnd );				
	sslot = nRui->GetItemSelectedSlot();
	
	
	if( tslot->PcItem )
	{
		tslot->PcItem->Clear();
		///tslot->PcItem = NULL;//->Clear();
	}

	return;
}
///---------------------------------------------------------------------------
///-- ItemSlotClick
///---------------------------------------------------------------------------
///-- Item 올리기
void CItemRemodel_WndProc::ItemSlotClick( SEventMessage* EMsg )
{
	CSlot*		targetslot	= NULL;//현재 장비 슬롯
	CSlot*		srcslot		= NULL;//선택된(pick) 인벤 슬롯
	SPcItem*	tmpPcItem	= NULL;
	int			position	= 0;

	///-- 아이템을 들고 있을 경우
	if( nRui->GetCursorItem() != NULL )
	{
		targetslot = (CSlot*)Container->Get( (WNDID)EMsg->FocusWnd );
		srcslot = nRui->GetItemSelectedSlot();

		if( srcslot->PcItem == NULL )
		{
			return;
		}
		
		
		if( targetslot->PcItem && (targetslot->PcItem == srcslot->PcItem) )
		{
			nRui->ResetCursorItem();
		}
		///-------------------------------------------------------------------
		///-- Item Slot
		if( targetslot == mcp_item_slot[0] || targetslot == mcp_item_slot[1] || targetslot == mcp_item_slot[2] )
		{
			if( ( srcslot->Group == n_SGROUP_INVENTORY ) && ( srcslot->PcItem->Class == n_Class_Special ) )
			{
				///-- 같은 아이템의 중복 등록 방지
				if( ( mcp_item_slot[0]->PcItem->IType != n_ITYPE_NONE && mcp_item_slot[0]->PcItem->Code == srcslot->PcItem->Code ) ||
					( mcp_item_slot[1]->PcItem->IType != n_ITYPE_NONE && mcp_item_slot[1]->PcItem->Code == srcslot->PcItem->Code ) ||
					( mcp_item_slot[2]->PcItem->IType != n_ITYPE_NONE && mcp_item_slot[2]->PcItem->Code == srcslot->PcItem->Code ) )
				{
					return;
				}

				///-- Amount 
				if( srcslot->PcItem->ItemTable->theBundle != 1 && srcslot->PcItem->Amount > 1 ) 								
				{
					g_NumberInputWnd.Open( n_NUMBERINPUT_INVENTORY_TUNING, targetslot->PcItem );
					mcp_temp_num_src_slot = srcslot->SlotAddr;
					mcp_temp_num_tga_slot = targetslot;
					
					///-- Del Cursor					
					nRui->ResetCursorItem();	
					
				}
				else
				{
					///-- Copy item
					targetslot->SetSlotPcItem( *(srcslot->PcItem) );								

					///-- Del Cursor					
					nRui->ResetCursorItem();	
				}
				
				
			}
			else if ( srcslot->Group == n_SGROUP_WEAR)
			{				
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_ENCHANT_WEAR ) );				
			}
		}
		///-------------------------------------------------------------------
		///-- elda Slot
		else if( targetslot == mcp_elda_slot )
		{
			if( ( srcslot->Group == n_SGROUP_INVENTORY ) && ( srcslot->PcItem->Class == n_Class_Eldastone ) )
			{	
				if( mi_skill_level  < srcslot->PcItem->ItemTable->theGrade )
				{
					SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_OVER_ITEM_LEVEL ) );	
					return;
				}
				
				///-- Copy item
				targetslot->SetSlotPcItem( *(srcslot->PcItem) );	
				targetslot->PcItem->Amount = 1;
				//targetslot->PcItem = srcslot->PcItem;

				///-- Del Cursor
				nRui->ResetCursorItem();
			}
		}

	}
	/*
	else
	{
	//커서에 아이템 붙이기
	targetslot = (CSlot*)Container->Get( (WNDID)EMsg->FocusWnd );
	
	  nRui->SetCursorItem( targetslot->PcItem );
	  nRui->SetItemSelectedSlot( targetslot );
	  }
	*/
}


///---------------------------------------------------------------------------
///-- LoadRemodelTable
///---------------------------------------------------------------------------
///-- Remodel Table 을 읽어 들인다.
///-- 최초 Wnd 가 Open 될 때 한번만 읽음
///-- !!! Unpack 버전 미작업
BOOL CItemRemodel_WndProc::LoadRemodelTable()
{
	int i;

	TCHAR filename[50];
	
///////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined( LOAD_VF ) ///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
	FILE* fp;
	sprintf( filename, "etc\\remodelstone.dat");
	
	fp = _tfopen( filename, _T("rb") );//VSS올리때...옵션 주의(읽기전용)일때....
	if( fp == NULL )
	{
		MessageBox( NULL, _T("!RemodelStone.dat 파일 열기 실패!!!"), _T("알림"), MB_OK );
		return false;
	}
	
	///-- read header
	fread( ( void * )&ls_data_header, sizeof( ELDA_Data_Header ), 1, fp);
	
	///-- Seek
	fseek( fp, ai_index * sizeof( ELDA_Data_Set ) , SEEK_CUR );
	
	///-- Read Header
	fread( ( void * )&ls_data_set, sizeof( ELDA_Data_Set ), 1, fp);
	
	mb_loadRemodelTable = true;
///////////////////////////////////////////////////////////////////////////////////////////////////	
#else  ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
	
	
	
	///-- File Size Check
	sprintf( filename, "remodelstone.dat");

	///-- File Open
	VFileHandle* pFH = g_VFEtc.OpenFile( filename );
	if( pFH == NULL ) 
	{
		MessageBox( NULL, _T("!RemodelStone.dat 파일 열기 실패!!!"), _T("알림"), MB_OK );
		return false;
	}

	int li_file_size = pFH->GetSize();
	
	///-- 사이즈 정규성 Check
	if( li_file_size % sizeof( SRemodelStone ) != 0 )
	{
		return( false );
	}

	///-- Set Count
	mi_table_count = li_file_size / sizeof( SRemodelStone );

	int a = sizeof( SRemodelStone );
	///-- read data
	g_RemodelTable = SAFE_NEW_ARRAY( SRemodelStone , mi_table_count );	
	if( g_RemodelTable == NULL )
	{
		return FALSE;
	}

	for( i = 0; i < mi_table_count; ++i )
	{
		///-- Read Header
		pFH->Read( ( void * )&g_RemodelTable[i], sizeof( SRemodelStone ));
		int a = sizeof( SRemodelStone );
	}


	mb_loadRemodelTable = true;
	g_VFEtc.CloseFile( pFH );
	return true;
///////////////////////////////////////////////////////////////////////////////////////////////////	
#endif ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
	
}
