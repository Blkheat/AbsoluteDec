#include "..\\RockPCH.h"


#include "Rui.h"

/*
#include "Define.h"

#include "Util.h"
#include "Render.h"
#include "Fontman.h"
#include "Resource.h"
#include "Chain.h"
#include "Container.h"
//*/

#include "..\\..\\GTimer.h"

#include "..\\..\\brother.h"
#include "..\\..\\RockClient.h"
#include "..\\..\\NetworkMgr.h"
#include "..\\..\\global.h"

#include "..\\..\\quadlist.h"
#include "..\\..\\bravolibs\\network\\Protocol.h"
#include "..\\..\\Pc.h"

#include "..\\..\\bravolibs\\obj\\player_manager.h"

#include "MainMenuWndProc.h"

#include "SystemMenuWndProc.h"
#include "CharStateWndProc.h"
#include "SkillWndProc.h"
#include "InventoryWndProc.h"
#include "QuestProc.h"

#include "CDelayBarWnd.h"
#include "ExchangeWndProc.h"
#include "MCommunityWnd.h"
#include "clogwnd.h"
#include "StringManager.h"
#include "CUserStoreSellWnd.h"
#include "FrontierMainWndProc.h"
#include "CWebShopWnd.h"
#include "UIMsgWnd.h"
#include "ChatMainProc.h"
#include "CColMapPositionWnd.h"

#include "CGuideWnd.h"
#include "RadioBroadcast.h"
#include "CChangeEldaColorWnd.h"

CMainMenuWnd		MainMenuWnd;
CMainButtonWnd		g_main_buttons;
CMainEXPWnd			g_Exp_wnd;
CMainEnduranceWarning		g_endu_warning;
///CMainMenuLVgageWnd	MainMenuLVgageWnd;


extern long		g_nowTime;

#ifdef TAIWAN30MINEVENT
	Bool  E_Start = false;
	Bool  E_End = false;
	N_CEVENTSTATE n_eState;
#endif //TAIWAN30MINEVENT

//-----------------------------------------------------------------------------
void CMainMenuWnd::Init()
{
	InitNew();
}

void CMainMenuWnd::InitOld()
{

}

void CMainMenuWnd::InitNew()
{
	int i = 0;
	iExBtnCount = 0 ;
	PcInven = nRui->GetPcInvenInfo();
	
	for( i = 0; i < c_MaxMainMenuItemDrawNum; ++i )
	{
		QSlot[i]->PcItem = PcInven->QuickSlot[i];

		//스킬코드채우기....
		if( QSlot[i]->PcItem == NULL )
		{
			SkillSystem->theQuickSlot_SkillCode[i] = 0;
		}
		else if( QSlot[i]->PcItem->IType == n_ITYPE_USE )
		{
			SkillSystem->theQuickSlot_SkillCode[i] = 0;
		}
		else if( QSlot[i]->PcItem->IType == n_ITYPE_SKILL )
		{
			SkillSystem->theQuickSlot_SkillCode[i] = QSlot[i]->PcItem->Code;
		}
	}	

	int j = 0;

	for( j = 0; j < c_MaxMainMenuExtenItemDrawNum; ++j )
	{
		EQSlot[j]->PcItem = PcInven->QuickSlot[i];

		//스킬코드채우기....
		if( EQSlot[j]->PcItem == NULL )
		{
			SkillSystem->theQuickSlot_SkillCode[i] = 0;
		}
		else if( EQSlot[j]->PcItem->IType == n_ITYPE_USE )
		{
			SkillSystem->theQuickSlot_SkillCode[i] = 0;
		}
		else if( EQSlot[j]->PcItem->IType == n_ITYPE_SKILL )
		{
			SkillSystem->theQuickSlot_SkillCode[i] = EQSlot[j]->PcItem->Code;
		}

		i++;
	}
	
	thePcParam	= nRui->GetPcParamInfo();
}

//-----------------------------------------------------------------------------
void CMainMenuWnd::Composition()
{
	CompositionNew();	
}

void CMainMenuWnd::CompositionOld()
{

}

void CMainMenuWnd::CompositionNew()
{
	CFrameWnd*		fw		= NULL;
	CButton*		btn		= NULL;
	CImageBox*		img		= NULL;	

	//-------------------------------------------------------------------------
	//
	//	메인 메뉴	
	//
	//-------------------------------------------------------------------------

#ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/22)
	
	if(true)
	{
		fw = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_MainMenu_Wnd, WID_None, 
			0, nRui->Desktop.Height - 58, 408, 45, false );
		
#else
		
	if( nRui->GetResolution() == n_800X600 )
	{
		
	}
	else if( nRui->GetResolution() == n_1024X768 )
	{
		fw = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_MainMenu_Wnd, WID_None, 
			0, 710, 408, 45, false );
			
#endif // #ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/22)

		//fw->SetClientColor( 160, 160, 160 );
		fw->SetClientImage( TID_None );
		fw->SetCaption( _RT("MainMenu") );
		fw->SetWndProc( this );
		fw->SetTitle( false );
		fw->SetVisible( false );
		fw->SetMove( false );
		fw->SetRegion( false );
		fw->SetZorder( n_zoTop );		
		fw->Initialize();


		img= (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_MainMenu_Wnd, 
			0, 0, 386, 45, false );       
		img->SetClientImage( TID_MAIN_SKILL );

		///---------------------------------------------------------------------
		///-- Slots
		QSlot[0] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_MainMenu_QSlot0, WID_MainMenu_Wnd, 4,  5, 35, 35, false ); 
		QSlot[1] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_MainMenu_QSlot1, WID_MainMenu_Wnd, 42, 5, 35, 35, false );
		QSlot[2] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_MainMenu_QSlot2, WID_MainMenu_Wnd, 80, 5, 35, 35, false );
		QSlot[3] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_MainMenu_QSlot3, WID_MainMenu_Wnd, 118, 5, 35, 35, false );
		QSlot[4] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_MainMenu_QSlot4, WID_MainMenu_Wnd, 156, 5, 35, 35, false );
		QSlot[5] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_MainMenu_QSlot5, WID_MainMenu_Wnd, 194, 5, 35, 35, false );
		QSlot[6] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_MainMenu_QSlot6, WID_MainMenu_Wnd, 232, 5, 35, 35, false );
		QSlot[7] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_MainMenu_QSlot7, WID_MainMenu_Wnd, 270, 5, 35, 35, false );
		QSlot[8] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_MainMenu_QSlot8, WID_MainMenu_Wnd, 308, 5, 35, 35, false );
		QSlot[9] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_MainMenu_QSlot9, WID_MainMenu_Wnd, 346, 5, 35, 35, false );

		QSlot[0]->SetSlotGroup( n_SGROUP_QUICK );
		QSlot[0]->SetSlotNum( 0 );
		QSlot[0]->SetHID( HID_ITEMTIP );
		QSlot[1]->SetSlotGroup( n_SGROUP_QUICK );
		QSlot[1]->SetSlotNum( 1 );
		QSlot[1]->SetHID( HID_ITEMTIP );
		QSlot[2]->SetSlotGroup( n_SGROUP_QUICK );
		QSlot[2]->SetSlotNum( 2 );
		QSlot[2]->SetHID( HID_ITEMTIP );
		QSlot[3]->SetSlotGroup( n_SGROUP_QUICK );
		QSlot[3]->SetSlotNum( 3 );
		QSlot[3]->SetHID( HID_ITEMTIP );
		QSlot[4]->SetSlotGroup( n_SGROUP_QUICK );
		QSlot[4]->SetSlotNum( 4 );
		QSlot[4]->SetHID( HID_ITEMTIP );
		QSlot[5]->SetSlotGroup( n_SGROUP_QUICK );
		QSlot[5]->SetSlotNum( 5 );
		QSlot[5]->SetHID( HID_ITEMTIP );
		QSlot[6]->SetSlotGroup( n_SGROUP_QUICK );
		QSlot[6]->SetSlotNum( 6 );
		QSlot[6]->SetHID( HID_ITEMTIP );
		QSlot[7]->SetSlotGroup( n_SGROUP_QUICK );
		QSlot[7]->SetSlotNum( 7 );
		QSlot[7]->SetHID( HID_ITEMTIP );
		QSlot[8]->SetSlotGroup( n_SGROUP_QUICK );
		QSlot[8]->SetSlotNum( 8 );
		QSlot[8]->SetHID( HID_ITEMTIP );
		QSlot[9]->SetSlotGroup( n_SGROUP_QUICK );
		QSlot[9]->SetSlotNum( 9 );
		QSlot[9]->SetHID( HID_ITEMTIP );

		///-- Slot Num ( Static )
		img= (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_MainMenu_Wnd, 3, 3, 349, 9, false );       
		img->SetClientImage( TID_MAIN_NUMBER ); 
		img->SetFocusCheck( false );

		theQExtenButton = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_MainMenu_Wnd, 
							386, 0, 21, 45, false );           
		theQExtenButton->SetBtnImage( TID_skillwindow_button, TID_skillwindow_button_ovr,
									  TID_skillwindow_button_clk, TID_None );
		theQExtenButton->SetHID( 137 );

		///-- 확장키 슬롯 프레임 구성
		theQExFrame = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_None, WID_None, 
			0, nRui->Desktop.Height - 58 - 46 , 386, 45, false );	
		theQExFrame->SetClientImage( TID_None );
		theQExFrame->SetCaption( _RT("MainMenu") );
		theQExFrame->SetWndProc( this );
		theQExFrame->SetTitle( false );
		theQExFrame->SetVisible( false );
		theQExFrame->SetMove( false );
		theQExFrame->SetRegion( false );
		theQExFrame->SetZorder( n_zoTop );		
		theQExFrame->Initialize();

		img= (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, theQExFrame->GetID(), 
			0, 0, 386, 45, false );       
		img->SetClientImage( TID_MAIN_SKILL );

	
	
		///---------------------------------------------------------------------
		///-- Slots
		EQSlot[0] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_MainMenu_EQSlot0, theQExFrame->GetID(), 4,  5, 35, 35, false ); 
		EQSlot[1] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_MainMenu_EQSlot1, theQExFrame->GetID(), 42, 5, 35, 35, false );
		EQSlot[2] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_MainMenu_EQSlot2, theQExFrame->GetID(), 80, 5, 35, 35, false );
		EQSlot[3] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_MainMenu_EQSlot3, theQExFrame->GetID(), 118, 5, 35, 35, false );
		EQSlot[4] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_MainMenu_EQSlot4, theQExFrame->GetID(), 156, 5, 35, 35, false );
		EQSlot[5] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_MainMenu_EQSlot5, theQExFrame->GetID(), 194, 5, 35, 35, false );
		EQSlot[6] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_MainMenu_EQSlot6, theQExFrame->GetID(), 232, 5, 35, 35, false );
		EQSlot[7] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_MainMenu_EQSlot7, theQExFrame->GetID(), 270, 5, 35, 35, false );
		EQSlot[8] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_MainMenu_EQSlot8, theQExFrame->GetID(), 308, 5, 35, 35, false );
		EQSlot[9] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_MainMenu_EQSlot9, theQExFrame->GetID(), 346, 5, 35, 35, false );

		EQSlot[0]->SetSlotGroup( n_SGROUP_QUICK );
		EQSlot[0]->SetSlotNum( 10 );
		EQSlot[0]->SetHID( HID_ITEMTIP );
		EQSlot[1]->SetSlotGroup( n_SGROUP_QUICK );
		EQSlot[1]->SetSlotNum( 11 );
		EQSlot[1]->SetHID( HID_ITEMTIP );
		EQSlot[2]->SetSlotGroup( n_SGROUP_QUICK );
		EQSlot[2]->SetSlotNum( 12 );
		EQSlot[2]->SetHID( HID_ITEMTIP );
		EQSlot[3]->SetSlotGroup( n_SGROUP_QUICK );
		EQSlot[3]->SetSlotNum( 13 );
		EQSlot[3]->SetHID( HID_ITEMTIP );
		EQSlot[4]->SetSlotGroup( n_SGROUP_QUICK );
		EQSlot[4]->SetSlotNum( 14 );
		EQSlot[4]->SetHID( HID_ITEMTIP );
		EQSlot[5]->SetSlotGroup( n_SGROUP_QUICK );
		EQSlot[5]->SetSlotNum( 15 );
		EQSlot[5]->SetHID( HID_ITEMTIP );
		EQSlot[6]->SetSlotGroup( n_SGROUP_QUICK );
		EQSlot[6]->SetSlotNum( 16 );
		EQSlot[6]->SetHID( HID_ITEMTIP );
		EQSlot[7]->SetSlotGroup( n_SGROUP_QUICK );
		EQSlot[7]->SetSlotNum( 17 );
		EQSlot[7]->SetHID( HID_ITEMTIP );
		EQSlot[8]->SetSlotGroup( n_SGROUP_QUICK );
		EQSlot[8]->SetSlotNum( 18 );
		EQSlot[8]->SetHID( HID_ITEMTIP );
		EQSlot[9]->SetSlotGroup( n_SGROUP_QUICK );
		EQSlot[9]->SetSlotNum( 19 );
		EQSlot[9]->SetHID( HID_ITEMTIP );			

		///-- Slot Num ( Static )
		img= (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, theQExFrame->GetID(), 3, 3, 353, 9, false );       
		img->SetClientImage( TID_skillwindow_f ); 
		img->SetFocusCheck( false );


#ifdef EXPANSION_QUICK_SLOT		
		///-- 확장키 슬롯 프레임 구성
		theQExFrame2 = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_None, WID_None, 
			0, nRui->Desktop.Height - 58 - 46 - 46  , 386, 45, false );	
		theQExFrame2->SetClientImage( TID_None );
		theQExFrame2->SetCaption( _RT("MainMenu") );
		theQExFrame2->SetWndProc( this );
		theQExFrame2->SetTitle( false );
		theQExFrame2->SetVisible( false );
		theQExFrame2->SetMove( false );
		theQExFrame2->SetRegion( false );
		theQExFrame2->SetZorder( n_zoTop );		
		theQExFrame2->Initialize();

	
		img= (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, theQExFrame2->GetID(), 
			0, 0, 386, 45, false );       
		img->SetClientImage( 3714 );
 




		EQSlot[10] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_MainMenu_EQSlot10, theQExFrame2->GetID(), 4,  5, 35, 35, false ); 
		EQSlot[11] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_MainMenu_EQSlot11, theQExFrame2->GetID(), 42, 5, 35, 35, false );
		EQSlot[12] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_MainMenu_EQSlot12, theQExFrame2->GetID(), 80, 5, 35, 35, false );
		EQSlot[13] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_MainMenu_EQSlot13, theQExFrame2->GetID(), 118, 5, 35, 35, false );
		EQSlot[14] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_MainMenu_EQSlot14, theQExFrame2->GetID(), 156, 5, 35, 35, false );
		EQSlot[15] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_MainMenu_EQSlot15, theQExFrame2->GetID(), 194, 5, 35, 35, false );
		EQSlot[16] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_MainMenu_EQSlot16, theQExFrame2->GetID(), 232, 5, 35, 35, false );
		EQSlot[17] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_MainMenu_EQSlot17, theQExFrame2->GetID(), 270, 5, 35, 35, false );
		EQSlot[18] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_MainMenu_EQSlot18, theQExFrame2->GetID(), 308, 5, 35, 35, false );
		EQSlot[19] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_MainMenu_EQSlot19, theQExFrame2->GetID(), 346, 5, 35, 35, false );

		EQSlot[10]->SetSlotGroup( n_SGROUP_QUICK );
		EQSlot[10]->SetSlotNum( 20 );
		EQSlot[10]->SetHID( HID_ITEMTIP );
		EQSlot[11]->SetSlotGroup( n_SGROUP_QUICK );
		EQSlot[11]->SetSlotNum( 21 );
		EQSlot[11]->SetHID( HID_ITEMTIP );
		EQSlot[12]->SetSlotGroup( n_SGROUP_QUICK );
		EQSlot[12]->SetSlotNum( 22 );
		EQSlot[12]->SetHID( HID_ITEMTIP );
		EQSlot[13]->SetSlotGroup( n_SGROUP_QUICK );
		EQSlot[13]->SetSlotNum( 23 );
		EQSlot[13]->SetHID( HID_ITEMTIP );
		EQSlot[14]->SetSlotGroup( n_SGROUP_QUICK );
		EQSlot[14]->SetSlotNum( 24 );
		EQSlot[14]->SetHID( HID_ITEMTIP );
		EQSlot[15]->SetSlotGroup( n_SGROUP_QUICK );
		EQSlot[15]->SetSlotNum( 25 );
		EQSlot[15]->SetHID( HID_ITEMTIP );
		EQSlot[16]->SetSlotGroup( n_SGROUP_QUICK );
		EQSlot[16]->SetSlotNum( 26 );
		EQSlot[16]->SetHID( HID_ITEMTIP );
		EQSlot[17]->SetSlotGroup( n_SGROUP_QUICK );
		EQSlot[17]->SetSlotNum( 27 );
		EQSlot[17]->SetHID( HID_ITEMTIP );
		EQSlot[18]->SetSlotGroup( n_SGROUP_QUICK );
		EQSlot[18]->SetSlotNum( 28 );
		EQSlot[18]->SetHID( HID_ITEMTIP );
		EQSlot[19]->SetSlotGroup( n_SGROUP_QUICK );
		EQSlot[19]->SetSlotNum( 29 );
		EQSlot[19]->SetHID( HID_ITEMTIP );		
#endif
	}
}

//-----------------------------------------------------------------------------
void CMainMenuWnd::Update()
{ 
	UpdateNew();
}

void CMainMenuWnd::UpdateOld()
{
	
}

void CMainMenuWnd::UpdateNew()
{
	CSlot *pSlot = NULL;
	
	for( int i = 0; i < c_MaxMainMenuItemDrawNum + c_MaxMainMenuExtenItemDrawNum ; ++i )
	{
		if( i < c_MaxMainMenuItemDrawNum )
		{
			pSlot = QSlot[i];	
		}
		else
		{
			pSlot = EQSlot[i - c_MaxMainMenuItemDrawNum];
		}
		
		if( pSlot == NULL )
		{
			continue;
		}		
		
		if( g_ExchangeWnd.m_bIsExchangeLock )
		{
			pSlot->theSlotState = n_Slot_Disable;
		}
		//.................................................................................................
		// 유료 아이템 뿅망치류를 들고 있을때
		//.................................................................................................
		else if( g_Pc.GetPlayer()->m_ISApproachWeapon )
		{
			if( pSlot->PcItem != NULL )
			{
				if( pSlot->PcItem->SkillTable != NULL )
				{
					if( g_Pc_Manager.ISUseSkillOnApproachWeapon( pSlot->PcItem->SkillTable->theSkillCode ) )
					{
						pSlot->theSlotState = n_Slot_Enable;
					}
					else
					{
						pSlot->theSlotState = n_Slot_Disable;
					}
				}
				else
				{
					pSlot->theSlotState = n_Slot_Enable;
				}
			}
			else
				pSlot->theSlotState = n_Slot_Disable;
		}
		else if( g_UserStoreSellWnd.ISUserSellMode() )
		{
			if( g_UserStoreSellWnd.m_UserStoreSellOpenMode && pSlot->PcItem != NULL )
			{
				if( pSlot->PcItem->SkillTable != NULL )
				{
					if( pSlot->PcItem->SkillTable->theSkillCode == c_SkillCode_UserSellStore )
					{
						pSlot->theSlotState = n_Slot_Enable;
					}
					else
					{
						pSlot->theSlotState = n_Slot_Disable;
					}
				}
				else
				{
					pSlot->theSlotState = n_Slot_Disable;
				}
			}
			else
				pSlot->theSlotState = n_Slot_Disable;
		}
		else
		{
			pSlot->theSlotState = n_Slot_Enable;
			
			if( pSlot->PcItem != NULL )
			{
				if( pSlot->PcItem->IsDelay == false )
				{
					SkillSystem->theQuickSlot_DelayTime[i] = true;
				}
			}
			else
			{
				SkillSystem->theQuickSlot_DelayTime[i] = true;
			}
			
			// 구입할 수 없는 아이템들은 Disable 시킨다 
			if( pSlot->PcItem )
			{	
				// 현재 스킬을 사용할 수 있는 상태인지 표시
				if( pSlot->PcItem->SkillTable && pSlot->PcItem->IType == n_ITYPE_SKILL )
				{
					if( pSlot->PcItem->SkillTable->theSkillType1 != n_PasiveSkill )
					{
						if( pSlot->PcItem->SkillTable->theSkillCode == c_SkillCode_BasicAttack_Millena || 
							pSlot->PcItem->SkillTable->theSkillCode == c_SkillCode_BasicAttack_Rain )
						{
							continue;
						}
						
						if( g_Pc.theSkillSystem.CheckAttackTarget( pSlot->PcItem->SkillTable ) == false || 
							g_Pc.theSkillSystem.CheckTargetStatus( pSlot->PcItem->SkillTable ) == false || 		
							//g_Pc.theSkillSystem.CheckAttackFrame( pSlot->PcItem->SkillTable ) == false || 
							//g_Pc.theSkillSystem.CheckDelayTime( pSlot->PcItem->SkillTable ) == false ||
							g_Pc.theSkillSystem.CheckAttackRange( pSlot->PcItem->SkillTable ) == false || 
							g_Pc.theSkillSystem.CheckNeedStatus( pSlot->PcItem->SkillTable ) == false ||
							g_Pc.theSkillSystem.CheckNeedItem( pSlot->PcItem->SkillTable ) == false )
						{
							pSlot->theSlotState = n_Slot_Disable;
						}

					}
				}
				// 사용할 수 있는 아이템인지 체크 
				else if( pSlot->PcItem->ItemTable && pSlot->PcItem->IType == n_ITYPE_USE )
				{
					///--JAPAN_BUG_MODIFY
					if( g_Pc.GetPlayer()->GetAddEffectState(n_TypeNotItem))
					{
						pSlot->theSlotState = n_Slot_Disable;
					}					
					
					// 필요 레벨 체크 
					if( pSlot->PcItem->ItemTable->theLimitLevel > nRui->thePcParam.Level )
					{
						pSlot->theSlotState = n_Slot_Disable;		
					}
					
					if( InventoryWnd.CheckUseItemPassiveSkill( pSlot->PcItem->ItemTable ) == false )
					{					
						pSlot->theSlotState = n_Slot_Disable;		
					}
				}
			}				
		}
	}

//	DWORD NeedExp	= g_Pc_Manager.GetStatNeedExp( thePcParam->MainClass, thePcParam->Level );
//	DWORD CurExp	= thePcParam->Exp;
}

//-----------------------------------------------------------------------------
void CMainMenuWnd::Proc( SEventMessage* EMsg )
{
	ProcNew(EMsg);	
}

void CMainMenuWnd::ProcOld( SEventMessage* EMsg )
{		

}

void CMainMenuWnd::ProcNew( SEventMessage* EMsg )
{
	if( !g_Pc.GetPlayer()->IsMyPlayerRender )
	{
		return;
	}	
	
	switch( EMsg->FocusWnd )
	{
	case WID_MainMenu_QSlot0:
	case WID_MainMenu_QSlot1:
	case WID_MainMenu_QSlot2:
	case WID_MainMenu_QSlot3:
	case WID_MainMenu_QSlot4:
	case WID_MainMenu_QSlot5:
	case WID_MainMenu_QSlot6:
	case WID_MainMenu_QSlot7:
	case WID_MainMenu_QSlot8:
	case WID_MainMenu_QSlot9:
		{
			if( !g_ExchangeWnd.m_bIsExchangeLock  ) 
			{
				if( g_UserStoreSellWnd.ISUserSellMode() )
				{
					if( g_UserStoreSellWnd.m_UserStoreSellOpenMode )
					{
						if( EMsg->Notify != NM_SLOT_DBCLICK )
							return;

						int Count = EMsg->FocusWnd - WID_MainMenu_QSlot0;
						
						if( QSlot[ Count ]->GetSlotItem() == NULL )
							return;
						
						if( QSlot[ Count ]->GetSlotItem()->SkillTable == NULL )
							return;
						
						if( QSlot[ Count ]->GetSlotItem()->SkillTable->theSkillCode != c_SkillCode_UserSellStore )
							return;

						if( EMsg->Notify == NM_SLOT_DBCLICK )	{	QuickSlotDBClick( EMsg );	}
					}
				}
				else
				{
					if( EMsg->Notify == NM_SLOT_CLICK )		{	QuickSlotClick( EMsg );		}
					if( EMsg->Notify == NM_SLOT_DBCLICK )	{	QuickSlotDBClick( EMsg );	}
					if( EMsg->Notify == NM_SLOT_DRAG )		{	QuickSlotDrag( EMsg );		}
					if( EMsg->Notify == NM_SLOT_DROP )		{	QuickSlotDrop( EMsg );		}
				}
			}
		}
		break;
	case WID_MainMenu_EQSlot0:
	case WID_MainMenu_EQSlot1:
	case WID_MainMenu_EQSlot2:
	case WID_MainMenu_EQSlot3:
	case WID_MainMenu_EQSlot4:
	case WID_MainMenu_EQSlot5:
	case WID_MainMenu_EQSlot6:
	case WID_MainMenu_EQSlot7:
	case WID_MainMenu_EQSlot8:
	case WID_MainMenu_EQSlot9:
	case WID_MainMenu_EQSlot10:				
	case WID_MainMenu_EQSlot11:				
	case WID_MainMenu_EQSlot12:				
	case WID_MainMenu_EQSlot13:				
	case WID_MainMenu_EQSlot14:				
	case WID_MainMenu_EQSlot15:			
	case WID_MainMenu_EQSlot16:				
	case WID_MainMenu_EQSlot17:				
	case WID_MainMenu_EQSlot18:				
	case WID_MainMenu_EQSlot19:				
		{
			if( !g_ExchangeWnd.m_bIsExchangeLock  ) 
			{
				if( g_UserStoreSellWnd.ISUserSellMode() )
				{
					if( g_UserStoreSellWnd.m_UserStoreSellOpenMode )
					{
						if( EMsg->Notify != NM_SLOT_DBCLICK )
							return;

						int Count = EMsg->FocusWnd - WID_MainMenu_EQSlot0;
						
						if( EQSlot[ Count ]->GetSlotItem() == NULL )
							return;
						
						if( EQSlot[ Count ]->GetSlotItem()->SkillTable == NULL )
							return;
						
						if( EQSlot[ Count ]->GetSlotItem()->SkillTable->theSkillCode != c_SkillCode_UserSellStore )
							return;

						if( EMsg->Notify == NM_SLOT_DBCLICK )	{	QuickSlotDBClick( EMsg );	}
					}
				}
				else
				{
					if( EMsg->Notify == NM_SLOT_CLICK )		{	QuickSlotClick( EMsg );		}
					if( EMsg->Notify == NM_SLOT_DBCLICK )	{	QuickSlotDBClick( EMsg );	}
					if( EMsg->Notify == NM_SLOT_DRAG )		{	QuickSlotDrag( EMsg );		}
					if( EMsg->Notify == NM_SLOT_DROP )		{	QuickSlotDrop( EMsg );		}
				}
			}
		}
		break;
	}

	if( EMsg->FocusWnd == theQExtenButton->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
#ifdef EXPANSION_QUICK_SLOT
			iExBtnCount++;

			if( iExBtnCount == 1 )
			{
				g_ChatMainProc.ReRPosition(0, -46);	
				theQExFrame->SetVisible(true);
				theQExFrame2->SetVisible( false);
			}
			else if( iExBtnCount == 2 )
			{
				g_ChatMainProc.ReRPosition(0, -46);
				theQExFrame->SetVisible(true);
				theQExFrame2->SetVisible(true);
			}
			else if( iExBtnCount == 3 ) 
			{
				//원상복구
				g_ChatMainProc.ReRPosition(0, 46*2  );
				theQExFrame->SetVisible(false);
				theQExFrame2->SetVisible(false);
				iExBtnCount =0; 
			}
#else
			bool bVisible = theQExFrame->IsVisible();
			bVisible = !bVisible;
			theQExFrame->SetVisible(bVisible);
			
			if(bVisible)
			{
				g_ChatMainProc.ReRPosition(0, -46);
			}
			else
			{
				g_ChatMainProc.ReRPosition(0, 46);	
			}
#endif
		}
	}
}

//-----------------------------------------------------------------------------
void CMainMenuWnd::UpdateQuickSlotItem()
{
	int i = 0;
	for( i = 0; i < c_MaxMainMenuItemDrawNum; ++i )
	{
		QSlot[i]->PcItem = PcInven->QuickSlot[StartSlotNum + i];
	}

	for( int j = 0; j < c_MaxMainMenuExtenItemDrawNum ; ++j )
	{
		EQSlot[j]->PcItem = PcInven->QuickSlot[i];
		i++;
	}
}

//-----------------------------------------------------------------------------
void CMainMenuWnd::SetSkillSystem( CSkillSystem* _skillsystem )
{
	SkillSystem = _skillsystem;
}

//-----------------------------------------------------------------------------
void CMainMenuWnd::QuickSlotLineChange()
{
	StartSlotNum = SlotLine * c_MaxMainMenuItemDrawNum;
	int i = 0;
	for( i = 0; i < c_MaxMainMenuItemDrawNum; ++i )
	{
		QSlot[i]->PcItem = PcInven->QuickSlot[StartSlotNum + i];		

		if( QSlot[i]->PcItem == NULL )
		{
			SkillSystem->theQuickSlot_SkillCode[i] = 0;
		}
		else if( QSlot[i]->PcItem->IType == n_ITYPE_USE )
		{
			SkillSystem->theQuickSlot_SkillCode[i] = 0;
		}
		else if( QSlot[i]->PcItem->IType == n_ITYPE_SKILL )
		{
			SkillSystem->theQuickSlot_SkillCode[i] = QSlot[i]->PcItem->Code;
		}	
	}

	int j = 0;

	for( j = 0; j < c_MaxMainMenuExtenItemDrawNum; ++j )
	{
		EQSlot[j]->PcItem = PcInven->QuickSlot[i];

		//스킬코드채우기....
		if( EQSlot[j]->PcItem == NULL )
		{
			SkillSystem->theQuickSlot_SkillCode[i] = 0;
		}
		else if( EQSlot[j]->PcItem->IType == n_ITYPE_USE )
		{
			SkillSystem->theQuickSlot_SkillCode[i] = 0;
		}
		else if( EQSlot[j]->PcItem->IType == n_ITYPE_SKILL )
		{
			SkillSystem->theQuickSlot_SkillCode[i] = EQSlot[j]->PcItem->Code;
		}

		i++;
	}	
}

//-----------------------------------------------------------------------------
void CMainMenuWnd::QuickSlotClick( SEventMessage* EMsg )
{	
	CSlot*			dest_slot	= NULL;
	CSlot*			src_slot	= NULL;
	SPcItem*		pcitem	= nRui->GetCursorItem();

	///if( !nRui->m_KeyLock.QuickSlotKey.GetCheckTime(g_nowTime))
	///	return;

	int i = 0;

	if( pcitem != NULL )
	{
		dest_slot = (CSlot*)Container->Get( EMsg->FocusWnd );
		src_slot = nRui->GetItemSelectedSlot();
		
		if( src_slot->PcItem == NULL )
		{
			return;
		}
		
		if( dest_slot->PcItem && (dest_slot->PcItem == src_slot->PcItem) )
		{
			nRui->ResetCursorItem();
			return;
		}

		switch( src_slot->Group )
		{
		case n_SGROUP_QUICK:
			{
				if( dest_slot->PcItem == NULL )				// 내려 놓은 곳이 빈슬롯이면 이동임 
				{
					for( i = 0; i < RP_MAX_QUICK_SLOT; ++i )
					{
						if( PcInven->QuickSlot[i] == nRui->GetCursorItem() )
						{
							g_RockClient.Send_RpCsUnLinkQuickSlot( i );
							break;
						}
					}
				}
				
				//새로운 자리로 링크
				SRpdQuickSlot	Qslot;
				
				if( nRui->GetCursorItem() )
				{
					if( nRui->GetCursorItem()->ItemUID != 0 )
					{
						Qslot.theID.theItemID	= nRui->GetCursorItem()->ItemUID;
						Qslot.theType			= n_ITEM;
					}
					else
					{
						Qslot.theID.theSkillID	= nRui->GetCursorItem()->Code;
						Qslot.theType			= n_SKILL;
						
						//스킬이면...스킬코드 갱신, 스킬코드채우기....
						///--JAPAN_BUG_MODIFY	
						///SkillSystem->theQuickSlot_SkillCode[dest_slot->SlotNum] = dest_slot->PcItem->Code;
					}
					
					Qslot.thePosition = StartSlotNum + dest_slot->SlotNum;
					g_RockClient.Send_RpCsLinkQuickSlot( Qslot ); 
				}
			}
			break;
		case n_SGROUP_INVENTORY:
			{
				if( pcitem->IType == n_ITYPE_USE )
				{
					//국가 체크 루틴
					WORD Citizen = (WORD)(src_slot->PcItem->ItemTable->theCitizen);
					if( RAIN == Citizen|| MILLENA == Citizen )	//아이템이 국가 종속 아이템이라면
					{
						if( Citizen != nRui->thePcParam.Citizen )		//플레이어 국가 아이템이 아니라면
						{
							SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_OTHER_NATION_ITEM )  );	
							return;
						}
					}	
	
#ifdef PREMIUMZONE_KEY
		if( src_slot->PcItem->Code == 21560 || src_slot->PcItem->Code == 21561
			|| src_slot->PcItem->Code == 21562 || src_slot->PcItem->Code == 21563 )
		{
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_EQUIP_THERE ) );
			return;
		}
#endif // PREMIUMZONE_KEY

#ifdef ITEMLIMITFUNC_NOTQUICKSLOT			//아이템 사용제한
					WORD aItemLimitFunc = src_slot->PcItem->ItemTable->theItemLimitFunc;
					if(CHECK_ITEMFUNC( aItemLimitFunc, n_ItemFunc_NotQuickSlot ))
					{
						nRui->ResetCursorItem();
						SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_EQUIP_THERE ) );
						return;
					}
#endif
					
					
					for( i = 0; i < RP_MAX_QUICK_SLOT; ++i )
					{
						if( PcInven->QuickSlot[i] == pcitem )
						{
							g_RockClient.Send_RpCsUnLinkQuickSlot( i );
							//						PcInven->QuickSlot[i] = NULL;
							MainMenuWnd.UpdateQuickSlotItem();
							break;
						}
					}
					
					//				dest_slot->SetSlotItem( pcitem );
					//				PcInven->QuickSlot[StartSlotNum + dest_slot->SlotNum] = pcitem;
					
					SRpdQuickSlot	Qslot;
					Qslot.theID.theItemID	= pcitem->ItemUID;
					Qslot.theType			= n_ITEM;
					Qslot.thePosition		= StartSlotNum + dest_slot->SlotNum;
					g_RockClient.Send_RpCsLinkQuickSlot( Qslot );
				}
				else
				{
					SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_EQUIP_THERE ) );
					//				nRui->TextOutChat( _RT("SYSTEM_MSG : 소모품만 링크할수 있습니다."), D3DCOLOR_XRGB( 255, 255, 0 ) );
					return;
				}
			}
			break;
		case n_SGROUP_SKILL:
			{
				if( pcitem->SkillTable->theSkillType1 != n_PasiveSkill )
				{
					for( i = 0; i < RP_MAX_QUICK_SLOT; ++i )
					{
						if( PcInven->QuickSlot[i] == pcitem )
						{
							g_RockClient.Send_RpCsUnLinkQuickSlot( i );
							//						PcInven->QuickSlot[i] = NULL;
							MainMenuWnd.UpdateQuickSlotItem();
							break;
						}
					}
					
					//퀵슬롯 등록
					//				dest_slot->SetSlotItem( pcitem );
					//				PcInven->QuickSlot[StartSlotNum + dest_slot->SlotNum] = pcitem;
					
					//스킬코드채우기....
					if( dest_slot->PcItem )
					{
						SkillSystem->theQuickSlot_SkillCode[dest_slot->SlotNum] = dest_slot->PcItem->Code;
					}
					
					//서버에 등록하기
					SRpdQuickSlot	Qslot;
					Qslot.theID.theSkillID	= pcitem->Code;
					Qslot.theType			= n_SKILL;
					Qslot.thePosition		= StartSlotNum + dest_slot->SlotNum;
					g_RockClient.Send_RpCsLinkQuickSlot( Qslot );
				}
				else
				{
					SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_EQUIP_THERE ) );
					//				nRui->TextOutChat( _RT("SYSTEM_MSG : 액티브 스킬만 등록할 수 있습니다."), D3DCOLOR_XRGB( 255, 255, 0 ) );
				}
			}
			break;
		default:
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_EQUIP_THERE ) );
//				nRui->TextOutChat( _RT("SYSTEM_MSG : 링크 할 수 없습니다."), D3DCOLOR_XRGB( 255, 255, 0 ) );
			}
			return;
		}

		nRui->ResetCursorItem();
	}
	else
	{
		//커서에 아이템 붙이기 메세지.
		dest_slot = (CSlot*)Container->Get( (WNDID)EMsg->FocusWnd );
		nRui->SetCursorItem( dest_slot->PcItem );
		nRui->SetItemSelectedSlot( dest_slot );
	}
}

//-----------------------------------------------------------------------------
void CMainMenuWnd::QuickSlotDBClick( SEventMessage* EMsg )
{
	//스킬 or 소모아이템 사용하기
	CSlot* cslot = (CSlot*)Container->Get( EMsg->FocusWnd );

	if( cslot->PcItem != NULL )
	{
		if( cslot->PcItem->IType == n_ITYPE_USE && cslot->PcItem->ItemTable )
		{
			bool result = InventoryWnd.CheckUseItemPassiveSkill( cslot->PcItem->ItemTable );
			
			if( result )
			{				
				if( !UseItem(cslot))
				{
					return;
				}
		
			}
			else
			{
				SSkillBaseInfo* skill_info = NULL;
				skill_info = g_Pc_Manager.GetSkillBaseInfoPtr( cslot->PcItem->ItemTable->theNeedSkill );
				
				if( skill_info )
				{
					SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NEED_EQUIP ), skill_info->theSkillName );				
				}
			}				
		}
		else if( cslot->PcItem->IType == n_ITYPE_SKILL )
		{
			//라이딩 중이면 스킬을 사용할수 없다
			if(g_Pc.GetPlayer()->m_State.bRide)
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANNOT_USESKILL_INRIDING ) );									
				return;
			}
			
		
			//스킬 관련 프로토콜...
			//clso->SlotNum -> DIK_F1 변환....
			if( !(cslot->PcItem->IsDelay) )		//딜레이 적용중이 아닐경우
			{	
				DWORD dSkillCode = 0;

				///--JAPAN_BUG_MODIFY				
				if( !(cslot->PcItem->SkillTable) )
				{
					return;
				}	

				dSkillCode = cslot->PcItem->SkillTable->theSkillCode;		
				
				if( cslot->SlotNum < c_MaxMainMenuItemDrawNum )
				{
					SkillSystem->UseSkillMgr( ( cslot->SlotNum + DIK_1 ), dSkillCode );
				}
				else if(cslot->SlotNum >= c_MaxMainMenuItemDrawNum  && cslot->SlotNum < c_MaxMainMenuItemDrawNum*2 )
				{
					SkillSystem->UseSkillMgr( ( (cslot->SlotNum - c_MaxMainMenuItemDrawNum) + DIK_F1 ), dSkillCode  );
				}
				else if(cslot->SlotNum >= 20  && cslot->SlotNum < 30 )
				{
					SkillSystem->UseSkillMgr( (cslot->SlotNum - c_MaxMainMenuItemDrawNum*2) + DIK_F1 + 0xff , dSkillCode  );
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
void CMainMenuWnd::QuickSlotDrag( SEventMessage* EMsg )
{
	CSlot* slot = (CSlot*)Container->Get( EMsg->FocusWnd );
	
	if( ( slot->PcItem != NULL ) && ( EMsg->FocusWnd != (WNDID)EMsg->fParam ) )
	{
		//커서에 아이템 붙이기
		if( nRui->GetCursorItem() == NULL )
		{
			nRui->SetCursorItem( slot->PcItem );
			nRui->SetItemSelectedSlot( slot );
		}
	}
}

//-----------------------------------------------------------------------------
void CMainMenuWnd::QuickSlotDrop( SEventMessage* EMsg )
{
	CSlot*	src_slot		= NULL;			// Drag Slot 
	CSlot*	dest_slot		= NULL;			// Drop Slot 
	WNDID	wndid		= (WNDID)EMsg->fParam;
	BYTE	position	= 0;

	src_slot = (CSlot*)Container->Get( EMsg->FocusWnd );
	dest_slot = (CSlot*)Container->Get( wndid );

	if( ( src_slot->PcItem != NULL ) && 
		( ( ( wndid >= WID_MainMenu_QSlot0 ) && ( wndid <= WID_MainMenu_QSlot9 ) ) ||
		  ( ( wndid >= WID_MainMenu_EQSlot0 ) && ( wndid <= WID_MainMenu_EQSlot19 ) ) ) )		
	{
		//---------------------------------------------------------------------
		//MessageBox( NULL, "퀵슬롯 사이의 이동 -> 나중에 추가", "QUICKSLOT", MB_OK );
		//nRui->TextOutChat( _RT("SYSTEM_MSG : 아직 퀵슬롯 사이의 이동은 지원하지 않습니다."), D3DCOLOR_XRGB( 255, 255, 0 ) );
		//return;
		//---------------------------------------------------------------------
		//옮길 퀵슬롯에 아이템이 있다면 링크 끊기
//		if( dest_slot->PcItem != NULL )
//		{
//			position = StartSlotNum + dest_slot->SlotNum;
//			PcInven->QuickSlot[position] = NULL;
//			dest_slot->PcItem = NULL;

			//링크 끊기
//			g_RockClient.Send_RpCsUnLinkQuickSlot( position );
//		}

		//다른 퀵슬롯으로 링크
//		position = StartSlotNum + dest_slot->SlotNum;
//		PcInven->QuickSlot[position] = nRui->GetCursorItem();
//		dest_slot->SetSlotItem( nRui->GetCursorItem() );
		
		//기존 슬롯 링크 끊기
		if( dest_slot->PcItem == NULL )			// 빈슬롯으로 이동된것이므로 이동임 
		{
			position = StartSlotNum + src_slot->SlotNum;
//			PcInven->QuickSlot[position] = NULL;
			g_RockClient.Send_RpCsUnLinkQuickSlot( position );
//			src_slot->SetSlotItem( NULL );
		}

		//새로운 자리로 링크
		SRpdQuickSlot	Qslot;
		if( nRui->GetCursorItem() != NULL )
		{
			if( nRui->GetCursorItem()->ItemUID != 0 )
			{
				Qslot.theID.theItemID	= nRui->GetCursorItem()->ItemUID;
				Qslot.theType			= n_ITEM;
			}
			else
			{
				Qslot.theID.theSkillID	= nRui->GetCursorItem()->Code;
				Qslot.theType			= n_SKILL;

				//스킬이면...스킬코드 갱신, 스킬코드채우기....
				SkillSystem->theQuickSlot_SkillCode[dest_slot->SlotNum] = nRui->GetCursorItem()->Code;
			}
			
			Qslot.thePosition		= StartSlotNum + dest_slot->SlotNum;
			g_RockClient.Send_RpCsLinkQuickSlot( Qslot ); 
		}
		else
		{
//			nRui->TextOutChat( _RT("SYSTEM_MSG : tslot == NULL"), D3DCOLOR_XRGB( 255, 255, 0 ) );
		}

		nRui->ResetCursorItem();
		
	}
	else if( EMsg->FocusWnd != wndid )
	{
//		PcInven->QuickSlot[StartSlotNum + src_slot->SlotNum] = NULL;

		//링크 끊기
		position = StartSlotNum + src_slot->SlotNum;
		g_RockClient.Send_RpCsUnLinkQuickSlot( position );
//		src_slot->SetSlotItem( NULL );

		nRui->ResetCursorItem();
	}
}

//-----------------------------------------------------------------------------
int CMainMenuWnd::GetStartSlotNum()
{
	return( StartSlotNum );
}

// 퀵슬롯 키 입력---------------------------------------------------------------------------
void CMainMenuWnd::UseFunctionKey( N_FUNCTIONKEY fk )
{
	//퀵슬롯에 정보를 교환할때는 서버에서 오기전까지는 키 입력 처리를 받지 않는다 . 
	
	if( !nRui->m_KeyLock.QuickSlotKey.GetCheckTime(g_nowTime))
		return;
/*	
	if( nRui->m_KeyLock.QuickSlotKey.bflag ) 
	{
		
		if( SAFE_TIME_COMPARE( g_nowTime , > , SAFE_TIME_ADD( nRui->m_KeyLock.QuickSlotKey.dTime , 3000 ) ) ) 
		{
			nRui->m_KeyLock.QuickSlotKey.bflag = false ; 
			nRui->m_KeyLock.QuickSlotKey.dTime  = 0 ; 
		}

		return;
	}
*/

	UseFunctionKeyNew(fk);	
}

void CMainMenuWnd::UseFunctionKeyOld( N_FUNCTIONKEY fk )
{

}


BOOL	CMainMenuWnd::CheckItemState(SPcItem* PcItem)
{

	//By simwoosung 아이템이 귀환 아이템일시에
	if( ( PcItem->ItemTable->theType == n_Type_Scroll )&& ( PcItem->ItemTable->theReHP == n_Scroll_Move ))
	{
		if(!g_DelayBarWnd.IsStart())
		{
			g_RockClient.Send_RpCs10SecDelay( n_DelayWarp );
			nRui->SendEvent( WID_DelayBar_Wnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
			g_DelayBarWnd.ResetDelay( DELAY_MAIN_RETURN , PcItem->ItemUID , FALSE );
		}
	}
	else
	{
		// 부가효과 (아이템 사용 불가) 처리 수정필요^^ 
		if( g_Pc.GetPlayer()->GetAddEffectState( n_TypeNotItem ) )
		{
			return FALSE;
		}

		// 상용 아이템 = 스크롤 아이템으로 처리 되어 있음 
		// 스크롤 아이템중 남녀 구분하여 써야 하는 아이템 처리 ( 헤어변경, 얼굴변경 )
		if( PcItem->ItemTable->theType == n_Type_Scroll &&
			( PcItem->ItemTable->theReHP == n_Scroll_Hair || PcItem->ItemTable->theReHP == n_Scroll_Face ) 
			)
		{
			if( PcItem->ItemTable->theReSP != g_Pc.m_MainPC.char_info.theGender )
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOTSEX_WEAR ) ); 
				return FALSE;
			}
		}

		
		//은신감지비약 과 은신비약은 사용을 못하게 ... 
		if( (g_Pc.GetPlayer()->m_State.bRide && (PcItem->Code >= 6119 && PcItem->Code <= 6120)) ||
			(  g_DelayBarWnd.GetDelay_Type() == DELAY_RIDE_ON && (PcItem->Code >= 6119 && PcItem->Code <= 6120) ) )
		{												
							
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_HIDE_OR_SENSOR) );								
			return FALSE;				
		}

		g_RockClient.Send_RpCsUse( PcItem->ItemUID );
		nRui->ResetCursorItem();

	}
	return TRUE;
}


void CMainMenuWnd::UseFunctionKeyNew( N_FUNCTIONKEY fk )
{
	if( !g_Pc.GetPlayer()->IsMyPlayerRender )
	{
		return;
	}
	
	CSlot *pSlot = NULL;

	if( fk < n_FKF1 )
	{
		pSlot = QSlot[fk];
	}
	else
	{
		pSlot = EQSlot[fk - n_FKF1];
	}

	if(NULL == pSlot)
	{
		return;
	}
	
	if( ( !g_ExchangeWnd.m_bIsExchangeLock && !g_UserStoreSellWnd.ISUserSellMode() ) || 
		   g_UserStoreSellWnd.m_UserStoreSellOpenMode )
	{
		if( pSlot->PcItem != NULL )
		{
			if( g_UserStoreSellWnd.m_UserStoreSellOpenMode )
			{
				if( pSlot->PcItem->SkillTable == NULL )
				{
					return;
				}

				if( pSlot->PcItem->SkillTable->theSkillCode != c_SkillCode_UserSellStore )
				{
					return;
				}
			}

			if( pSlot->PcItem->IType == n_ITYPE_USE )
			{
				///--소환 출두 아이템일 경우
				if( !UseItem(pSlot))
				{
					return;
				}
				
			}
			else if( pSlot->PcItem->IType == n_ITYPE_SKILL )
			{
				//라이딩 중이면 스킬을 사용할수 없다
				if(g_Pc.GetPlayer()->m_State.bRide)
				{
					SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANNOT_USESKILL_INRIDING ) );	
					return;
				}				
				
				pSlot->CheckDelay();
				
				if( !(pSlot->PcItem->IsDelay) )		//딜레이 적용중이 아닐경우
				{	
					
					DWORD dSkillCode = 0;
					///--JAPAN_BUG_MODIFY				
					if( !(pSlot->PcItem->SkillTable) )
					{
						return;
					}	
					
					dSkillCode = pSlot->PcItem->SkillTable->theSkillCode;
					
					// 퀵슬롯 기능키로 스킬 시전
					if(pSlot->SlotNum < c_MaxMainMenuItemDrawNum )
					{
						SkillSystem->UseSkillMgr( pSlot->SlotNum + DIK_1, dSkillCode );
					}
					else if(pSlot->SlotNum >= c_MaxMainMenuItemDrawNum  && pSlot->SlotNum < c_MaxMainMenuItemDrawNum*2  )
					{
						SkillSystem->UseSkillMgr( ( pSlot->SlotNum - c_MaxMainMenuItemDrawNum ) + DIK_F1, dSkillCode );
					}
					else if(pSlot->SlotNum >= 20  && pSlot->SlotNum < 30 )
					{
						SkillSystem->UseSkillMgr( (pSlot->SlotNum - c_MaxMainMenuItemDrawNum*2 ) + DIK_F1 + 0xff , dSkillCode  );
					}

				}
			}
#ifdef QUICKSLOT_MOVE_ITEM_WEAR
			else if(pSlot->PcItem->IType == n_ITYPE_WEAR )
			{
				//장비템이면
				//장비창어느자리에 있는지 찾아내고 
				//찾았으면 교환을 시도한다.. 
				// 교환이 되면 퀵슬롯에 바뀐 장비코드로 새로 들어온다.
				// 인벤에서 일어나는 상황 서로교체를 하고 교체된 장비자리값으로 맞바꾼다..
				// 이거면 되낭 ㅋㅋ 
			}
#endif

		}
	}
	return;
}

BOOL CMainMenuWnd::UseItem(CSlot* pSlot)
{
	///--소환
  
	static long delay_time = false;

	if( SAFE_TIME_COMPARE( g_nowTime , > , SAFE_TIME_ADD( delay_time , 1000 ) ) )
	{
		///--PRINT_DLG_LOG("아이템 사용 g_nowTime = %u delay_time + 1000 = %u [Result %d] ", 
		///--g_nowTime, delay_time + 1000 ,(int)(g_nowTime - (delay_time + 1000)) );
		delay_time = g_nowTime;		
	}			
	else
	{
		
		///--PRINT_DLG_LOG("아이템 사용불가 g_nowTime = %u delay_time + 1000 = %u [Result %d] ", 
		///--	g_nowTime, delay_time + 1000 ,(int)(g_nowTime - (delay_time + 1000)) );
		return FALSE;
	}
	
	///-- 공성병기 소환 아이템일경우 
	if( g_Pc_Manager.IsColonyItem(pSlot->PcItem->ItemTable) )
	{
		///-- 공성병기 소환 가능할 경우
		if( !g_Pc_Manager.IsCanCallColonyItem() )
		{
			return FALSE;
		}			
		
		if( !g_ColMapPositionWnd->IsVisible )
		{
			UINT nCallType = pSlot->PcItem->ItemTable->theReHPSpeed;
			g_ColMapPositionWnd->Open();
			g_ColMapPositionWnd->SetColItem(pSlot->PcItem, nCallType);
		}
		
		return FALSE;
	}

	//소환  
	if( pSlot->PcItem->ItemTable && pSlot->PcItem->ItemTable->theType == n_Type_Scroll && 
		pSlot->PcItem->ItemTable->theReHP == n_Scroll_Summon )
	{
		g_UIMsgWnd.SetItemID(pSlot->PcItem->ItemUID);
		g_UIMsgWnd.SetMsgType(MSG_CALL_DEST);
		nRui->SendEvent( WID_UIMsgWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
		nRui->SendEvent( WID_UIMsgInEdit, n_emSetFocus, 0, 0, 0, 0 );
		return FALSE;
	}
	
	///--출두
	if( pSlot->PcItem->ItemTable && pSlot->PcItem->ItemTable->theType == n_Type_Scroll && 
		pSlot->PcItem->ItemTable->theReHP == n_Scroll_Appear )
	{
		g_UIMsgWnd.SetItemID(pSlot->PcItem->ItemUID);
		g_UIMsgWnd.SetMsgType(MSG_MOVE_DEST);
		nRui->SendEvent( WID_UIMsgWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
		nRui->SendEvent( WID_UIMsgInEdit, n_emSetFocus, 0, 0, 0, 0 );
		return FALSE;
	}
	
	///--캐릭터 이름변환
	if( pSlot->PcItem->ItemTable && pSlot->PcItem->ItemTable->theType == n_Type_Scroll && 
		pSlot->PcItem->ItemTable->theReHP == n_Scroll_PCName )
	{
		g_UIMsgWnd.SetItemID(pSlot->PcItem->ItemUID);
		g_UIMsgWnd.SetMsgType(MSG_RE_PCNAME);
		nRui->SendEvent( WID_UIMsgWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
		nRui->SendEvent( WID_UIMsgInEdit, n_emSetFocus, 0, 0, 0, 0 );
		return FALSE;
	}
	
	///-- 프론티어 이름변환
	if( pSlot->PcItem->ItemTable && pSlot->PcItem->ItemTable->theType == n_Type_Scroll && 
		pSlot->PcItem->ItemTable->theReHP == n_Scroll_FrontierName )
	{
		//1차적으로 프론티어장인지를 먼저체크
		//1차적으로 프론티어에 소속되어있는지
		if( ( nRui->GetPcInvenInfo()->FrontierMyInfo == NULL ) || 
			( nRui->GetPcInvenInfo()->FrontierMyInfo->FrontierID == 0 ) || 
		    ( nRui->GetPcInvenInfo()->FrontierMyInfo->FrontierGrade != n_FT_Master ) )
		{
			SystemPrint( n_DarkGreen ,n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOT_AUTHORITY ) );				 
		    return FALSE;
		}		
		
		g_UIMsgWnd.SetItemID(pSlot->PcItem->ItemUID);
		g_UIMsgWnd.SetMsgType(MSG_RE_FRONTIER_NAME);
		nRui->SendEvent( WID_UIMsgWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );		
		return FALSE;
	}
	
	///-- 성별 전환
	if( pSlot->PcItem->ItemTable && pSlot->PcItem->ItemTable->theType == n_Type_Scroll && 
		pSlot->PcItem->ItemTable->theReHP == n_Scroll_Sex )
	{
		if( !InventoryWnd.IsEmptyWearSlot() )
		{
			SystemPrint( n_DarkGreen ,n_SystemMsg, G_STRING_CHAR( IDS_SYSMUST_TAKEOFFFORCGENDER2 ) );
			return TRUE;
		}		
		
		g_UIMsgWnd.SetItemID(pSlot->PcItem->ItemUID);
		g_UIMsgWnd.SetMsgType(MSG_RE_GENDER);
		nRui->SendEvent( WID_UIMsgWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );		
		return FALSE;
	}
	
	if( pSlot->PcItem->ItemTable && pSlot->PcItem->ItemTable->theType == n_Type_Scroll && 
		pSlot->PcItem->ItemTable->theReHP == n_Scroll_ChangeEldaColor )
		///-- 엘다스톤 변경 관련 아이템 클릭시
	{
		g_pChangeEldaColorWnd->SetPayPcItem(pSlot->PcItem);
		g_pChangeEldaColorWnd->Open();

		return FALSE;
	}

#ifdef RENEWAL_QUEST1030		
		
	///-- 스크롤 퀘스트	
	if( pSlot->PcItem->ItemTable && pSlot->PcItem->ItemTable->theType == n_Type_Scroll && 
		pSlot->PcItem->ItemTable->theReHP == n_Scroll_Quest && 
		pSlot->PcItem->ItemTable->theReMP )
	{
		if( pSlot->PcItem->ItemTable->theReMP != g_Pc.m_MainPC.char_info.theMainClass )
		{
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANNOTBYUANDUSEJOP ) ); 
			return FALSE;
		}

		///-- 소드맨일 경우
		if( g_Pc.m_MainPC.char_info.theMainClass == Swordman )
		{
			///-- 한손검인지 두손검인지 체크해서 비교하는 루틴이 필요
			if( pSlot->PcItem->ItemTable->theNeedSkill )
			{
				WORD aNeedSkill = pSlot->PcItem->ItemTable->theNeedSkill;
				// 장비 착용 (무기) ---------------------------------------------------------------------------------- 
				if( aNeedSkill >= c_SkillCode_OndHandSword_Min && 
					aNeedSkill <= c_SkillCode_OndHandSword_Max )					
				{
					if( !InventoryWnd.theSkillCode_Sword )
					{
						SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANNOTBYUANDUSEJOP ) ); 
						return FALSE;
					}
				}
				else if( aNeedSkill >= c_SkillCode_TwoHandSword_Min && 
						 aNeedSkill <= c_SkillCode_TwoHandSword_Max )		
				{
					if( !InventoryWnd.theSkillCode_TwohandSword )
					{
						SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANNOTBYUANDUSEJOP ) ); 
						return FALSE;
					}
				}	
			}
		}
	}

#endif

	///-- 춤 중복쓰기 방지 
	if( (pSlot->PcItem->ItemTable->theReHP == n_Scroll_Expression) && 
		g_Pc.GetPlayer()->m_curt_event.type == SM_ACT )
	{
		LPACTION_DATA	pData = NULL;
		
		if(	( pData = g_RockClient.m_Action.GetData( pSlot->PcItem->ItemTable->theReSP ) ) == NULL )
			return FALSE;
		
		if( g_Pc.GetPlayer()->m_ani_index == pData->StartAni[0])
		{
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_EMOTION_NOT_USE ) );
			return FALSE;
		}
		
	}

	///-- 사교성 아이템 일경우		
	if( pSlot->PcItem->ItemTable && pSlot->PcItem->ItemTable->theType == n_Type_Scroll && 
		pSlot->PcItem->ItemTable->theReHP == n_Scroll_Expression )
	{
		BOOL IsUseExpression = TRUE;
		
		if( g_Pc.GetSkillSystem()->theIsUsedSkill )
		{
			IsUseExpression = FALSE;
		}		
		
		if( g_DelayBarWnd.IsVisible && DELAY_RIDE_ON == g_DelayBarWnd.GetDelay_Type() )
		{
			IsUseExpression = FALSE;
		}

		if(g_Pc.GetPlayer()->m_State.bRide)
		{
			IsUseExpression = FALSE;
		}
		
		switch( g_Pc.GetPlayer()->m_curt_event.type )
		{
			case NULL:
			case SM_SIMSIM:
			case SM_ACT:				
				break;
			default:
				 IsUseExpression = FALSE;
				break;
		}
		
		if( FALSE == IsUseExpression )
		{
			return FALSE;
		}		
	}	

	// 소모성 아이템의 딜레이 시간 체크 
	//			WORD	theDelayTime;		// 아이템 쿨타임 
	//			BYTE	theDelayGroup;		// 아이템 쿨타임 그룹 (최대 50)
	for( int i = 0; i < c_Max_ItemDelayInfo; ++i )
	{
		if( pSlot->PcItem->ItemTable->theDelayGroup == i )
		{						
			if( (nRui->enableDelayTime[i]) && !(nRui->useCheckDelayTime[i]) )
			{
				if(!CheckItemState(pSlot->PcItem))
				{
					return FALSE;
				}
				
				nRui->useCheckDelayTime[i] = true;
			}						
			else if( !nRui->useCheckDelayTime[i] )
			{							
				int time = ( nRui->realItemDelayTime[i] - nRui->cumItemDelayTime[i] ) / 1000.0f;
				if( time >= 0 )
				{
					SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_WAIT_FOR_USE ), time + 1 );
				}
			}
		}
	}	
		
	return TRUE;

}

//-----------------------------------------------------------------------------
void CMainMenuWnd::SetSkillDelayTime( WORD _skillcode )
{
   SPcItem ** pSkillItem = nRui->GetPcInvenInfo()->SkillItem;

   for( int i = 0; i < RP_MAX_USER_SKILL ; i++)
   {
      if( ( pSkillItem[i] != NULL) && ( pSkillItem[i]->IType == n_ITYPE_SKILL) )
	  {
	     if( pSkillItem[i]->Code == _skillcode)
		 {
			pSkillItem[i]->IsDelay = true;
			DWORD dDelayTime = pSkillItem[i]->SkillTable->theDelayTime;
			// 테이블 값은 100ms단위 -> ms단위로 바꿈
			pSkillItem[i]->DelayTime	=  (float)dDelayTime * 100.0f; // ms
			pSkillItem[i]->ProgressTime	=  (float)dDelayTime * 100.0f;

			pSkillItem[i]->CumulativeTime = 0.0f;
			break;			
		 }	
	  }
   }
}

//-----------------------------------------------------------------------------
void CMainMenuWnd::UnLinkQuickSlot( CSlot* slot )
{
	if( slot->PcItem != NULL )
	{	
//		slot->SetSlotItem( NULL );

		BYTE position = StartSlotNum + slot->SlotNum;
//		PcInven->QuickSlot[position] = NULL;

		//링크 끊기
		g_RockClient.Send_RpCsUnLinkQuickSlot( position );

		nRui->ResetCursorItem();
	}
}

//----------------------------------------------------------------------------
bool CMainMenuWnd::CompareSlot( CSlot* cslot, CSlot* tslot )
{
	if( ( cslot != NULL ) && ( tslot != NULL ) )
	{
		if( ( cslot->Group == tslot->Group ) &&
			( cslot->SlotNum == tslot->SlotNum ) &&
			( cslot->PcItem->ItemUID == tslot->PcItem->ItemUID ) &&
			( cslot->PcItem->Code == tslot->PcItem->Code ) )
		{
			return( true );
		}
	}

	return( false );
}

void CMainMenuWnd::LogOff()
{
	SlotLine = 0;
#ifdef EXPANSION_QUICK_SLOT
	if(iExBtnCount == 1 )
	{
		theQExFrame->SetVisible(false);
		theQExFrame2->SetVisible(false);
		g_ChatMainProc.ReRPosition(0, 46);		
	}
	else if( iExBtnCount == 2 )
	{
		theQExFrame->SetVisible(false);
		theQExFrame2->SetVisible(false);
		g_ChatMainProc.ReRPosition(0, 46*2);		
	}

	iExBtnCount = 0 ; 
#else
	bool bVisible = theQExFrame->IsVisible();		
	if(bVisible)
	{
		theQExFrame->SetVisible(false);
		g_ChatMainProc.ReRPosition(0, 46);		
	}
#endif
}

//-----------------------------------------------------------------------------





///---------------------------------------------------------------------------
///-- Composition
///---------------------------------------------------------------------------
void CMainEXPWnd::Composition()
{
	CompositionNew();	
}

void CMainEXPWnd::CompositionOld()
{

}

void CMainEXPWnd::CompositionNew()
{
	CFrameWnd*		fw		= NULL;
	CButton*		btn		= NULL;
	CImageBox*		img		= NULL;

#ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/22)

	fw = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_MainExp_Wnd, WID_None, 0,
		nRui->Desktop.Height - (768 - 756), nRui->Desktop.Width, 12, false);

#else

	fw = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_MainExp_Wnd, WID_None, 0, 756, 1024, 12, false );	

#endif// #ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/22)
	
	fw->SetClientImage( TID_None );	
	fw->SetWndProc( this );
	fw->SetTitle( false );
	fw->SetVisible( false );
	fw->SetMove( false );
	fw->SetRegion( false );
	fw->SetZorder( n_zoTop );		
	fw->Initialize();

#ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/22)

	img= (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_MainExp_Wnd, 0, 0, nRui->Desktop.Width, 12, false ); 

#else

	img= (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_MainExp_Wnd, 0, 0, 1024, 12, false );  

#endif// #ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/22)

	img->SetClientImage( TID_MAIN_EXP_BG ); 
	
#ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/22)

	ExpGauge = (CProgressBar*)nRui->CreateWnd( n_wtProgressBar, WID_None, WID_MainExp_Wnd, 0, 0, nRui->Desktop.Width, 12, false );  

#else

	ExpGauge = (CProgressBar*)nRui->CreateWnd( n_wtProgressBar, WID_None, WID_MainExp_Wnd, 0, 0, 1024, 12, false );  

#endif// #ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/22)

	ExpGauge ->SetClientImage( TID_None );
	ExpGauge ->SetClientColor( 255, 255, 255 );
	ExpGauge ->SetBarImage( TID_MAIN_EXP_GAUGE );	
	ExpGauge ->InitProgress( 100 );	
	
#ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/22)

	ExpText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_MainExp_Wnd, 0, 0, nRui->Desktop.Width, 12, false );  

#else

	ExpText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_MainExp_Wnd, 0, 0, 1024, 12, false );  

#endif// #ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/22)

	ExpText->SetFontg( n_ftGulimChe, 5, n_fwNormal, n_fdNormal );
	ExpText->SetImageNum( true );
	ExpText->SetClientImage( TID_None );	
	ExpText->SetAlignText( n_atCenter, n_atCenter );
	ExpText->SetText( _RT("100/100") );

	img= (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_MainExp_Wnd, 7, 4, 31, 7, false );  
	img->SetClientImage( TID_CLIENT );
	
	img= (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_MainExp_Wnd, 8, 5, 11, 5, false );  
	img->SetClientImage( TID_skill_exp );

	ExpPercent = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_MainExp_Wnd, 17, 0, 15, 12, false );  
	ExpPercent->SetFontg( n_ftGulimChe, 5, n_fwNormal, n_fdNormal );
	ExpPercent->SetImageNum( true );
	ExpPercent->SetClientImage( TID_None );	
	ExpPercent->SetAlignText( n_atRight, n_atCenter );
	ExpPercent->SetText( _RT("0") );

	img= (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_MainExp_Wnd, 32, 5, 5, 5, false );  
	img->SetClientImage( TID_skill_percent );
}

///---------------------------------------------------------------------------
///-- Init
///---------------------------------------------------------------------------
void CMainEXPWnd::Init()
{
	thePcParam	= nRui->GetPcParamInfo();
}
///---------------------------------------------------------------------------
///-- Update
///---------------------------------------------------------------------------
void CMainEXPWnd::Update()
{	
	UpdateNew();
}

void CMainEXPWnd::UpdateOld()
{
	
}

void CMainEXPWnd::UpdateNew()
{
	static TCHAR tmpStr[128] = {0,};
#ifdef EXP_INT64
	LONGLONG NeedExp	= g_Pc_Manager.GetStatNeedExp64( thePcParam->MainClass, thePcParam->Level );
	LONGLONG CurExp	= thePcParam->Exp;
	int cur_percent	= (int)(100 * ( (double)CurExp / (double)NeedExp ));
#else 
	
	DWORD NeedExp	= g_Pc_Manager.GetStatNeedExp( thePcParam->MainClass, thePcParam->Level );
	DWORD CurExp	= thePcParam->Exp;
	int cur_percent	= (int)(100 * ( (double)CurExp / NeedExp ));

#endif 

	int size = sizeof(ULONGLONG);

	if( NeedExp == 0 )
	{
//		nRui->TextOutChat( "SYSTEM_MSG : 잘못된 필요경험치입니다.", 0xffffff00 );
		return;
	}

	ExpGauge->SetPercent( cur_percent );

	//	__int64
#ifdef EXP_INT64
	Rsprintf( RWCHAR(tmpStr), _RT( " %I64u  /  %I64u " ), CurExp, NeedExp);

#else 
	Rsprintf( RWCHAR(tmpStr), _RT( " %u  /  %u " ), CurExp, NeedExp);

#endif 

	ExpText->SetText( tmpStr );
	
	Rsprintf( RWCHAR(tmpStr), _RT("%d"), cur_percent );
	ExpPercent->SetText( tmpStr );
}

///---------------------------------------------------------------------------
///-- Composition
///---------------------------------------------------------------------------
void CMainEnduranceWarning::Composition()
{	
	int i;
	CFrameWnd*		fw		= NULL;
	CButton*		btn		= NULL;
	CImageBox*		img		= NULL;

	FrameWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_MAIN_ENDU_WARN, WID_None, 985, 280, 45, 400, false );	
	FrameWnd->SetClientImage( TID_None );	
	FrameWnd->SetWndProc( this );
	FrameWnd->SetTitle( false );
	FrameWnd->SetVisible( false );
	FrameWnd->SetMove( false );
	FrameWnd->SetRegion( false );
	FrameWnd->SetZorder( n_zoTop );	
	FrameWnd->SetFocusCheck( false );	
	
	FrameWnd->Initialize();

	mi_index = 0;

	for( i = 0; i< RP_MAX_EQUIP; ++i )
	{
		mcp_slots[i] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, WID_MAIN_ENDU_WARN, 0, 0 + 38 * i, 35, 35, false );
		mcp_slots[i]->PcItem = NULL;
		mcp_slots[i]->SetSlotGroup( n_SGROUP_NONE );	
		mcp_slots[i]->SetHID( HID_ITEMTIP );
		mcp_slots[i]->SetFocusCheck( false );	
	
	}
	///-- 아이템 Slot
	
	/*
	///-- eld text
	img= (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, FrameWnd->GetID(), 0, 0, 1024, 12, false );  
	img->SetClientImage( TID_MAIN_EXP_BG ); 

	ExpGauge = (CProgressBar*)nRui->CreateWnd( n_wtProgressBar, WID_None, FrameWnd->GetID(), 0, 0, 1024, 12, false );  
	ExpGauge ->SetClientImage( TID_None );
	ExpGauge ->SetClientColor( 255, 255, 255 );
	ExpGauge ->SetBarImage( TID_MAIN_EXP_GAUGE );	
	ExpGauge ->InitProgress( 100 );
	*/
	
}
///---------------------------------------------------------------------------
///-- Update
///---------------------------------------------------------------------------
void CMainEnduranceWarning::Update()
{
	int k;
	SPcItem* PcItem;
	mi_index = 0;
	
	///-----------------------------------------------------------------------
	///-- 장착 인벤토리 Access
	for( k = 0; k < RP_MAX_EQUIP; ++k )
	{	
		if( nRui->GetPcInvenInfo()->WearItem[k] == NULL )
		{
			continue;
		}					
		///-- 내구도 검사
		PcItem = nRui->GetPcInvenInfo()->WearItem[k]; 		

		if( PcItem->Code >= c_NotRepairitem_Start &&  
			(PcItem->ItemTable->theClass == n_Class_Weapon || PcItem->ItemTable->theClass == n_Class_Accessory ) &&
			PcItem->ItemTable->theType != n_Type_ShortWeapon ) 
		{
			continue;			
		}

		if( PcItem && PcItem->ItemTable->theMaxEndurance && 
				PcItem->WearTable.theEndurance * 100 / PcItem->ItemTable->theMaxEndurance <= 50 )				
		{
			assert(mi_index < RP_MAX_EQUIP);

			mcp_slots[mi_index]->PcItem = PcItem;			
			++mi_index;
		}
	}
	///-- 다른 슬롯 제거
	for( k = mi_index ; k < RP_MAX_EQUIP; ++k )
	{
		mcp_slots[k]->PcItem = NULL;	
		mcp_slots[k]->SetFocusCheck( false );
	}
				
}

#ifdef TAIWAN30MINEVENT
void CMainButtonWnd::GetSignalStart( BYTE eStart) //유료인벤확장
{
	n_eState = (N_CEVENTSTATE)eStart;
	if(n_eState == n_eSuccess )
	{
		AllTime = m_Cumulative ;
		SystemPrint( n_DarkGreen, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_TIWAN30EVENTSTART ));
	}
	if(n_eState == n_eComplete  )
	{
		event_Buttons->SetFocusCheck( false );
		SystemPrint( n_DarkGreen, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_TIWAN30EVENTEND ));
	}			
}
void CMainButtonWnd::GetSignalEnd( BYTE eEnd) //유료인벤확장
{
	E_End = eEnd;
	n_eState = n_eComplete  ;
	event_Buttons->SetBtnImage( TID_OPT_QUEST_BT, TID_OPT_QUEST_BT_OVR, TID_OPT_QUEST_BT_CLK, TID_None );
	SystemPrint( n_LightGreen, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_TIWAN30EVENTCOMPLETE ));
}

///---------------------------------------------------------------------------
///-- Init
///---------------------------------------------------------------------------
void CMainButtonWnd::Init()
{
	m_CurTime = timeGetTime();
	m_NowTime = timeGetTime();
	m_Cumulative = 0;
	m_MilliSec = 0 ; 
	OnTwinkle = false;
	e_State = n_EVENT_OPEN;
	n_eState = n_eFail ;
	event_Buttons->SetFocusCheck( true );
	str[258]= NULL;
}
#endif //TAIWAN30MINEVENT

///---------------------------------------------------------------------------
///-- Composition
///---------------------------------------------------------------------------
void CMainButtonWnd::Composition()
{
	CompositionNew();
}

void CMainButtonWnd::CompositionOld()
{

}

void CMainButtonWnd::CompositionNew()
{
	CFrameWnd*		fw		= NULL;
	CButton*		btn		= NULL;

	SRect MainButtonWnd;
#ifdef TAIWAN30MINEVENT

	CFrameWnd*		Ew		= NULL;
	SRect MainEventButtonWnd;

	MainEventButtonWnd.x = 848;
	MainEventButtonWnd.y = 310; 		
	MainEventButtonWnd.w = 100;
	MainEventButtonWnd.h = 60; 
#endif //TAIWAN30MINEVENT

	int nButtonXPos[11] = { 8, 45, 82, 119, 156, 193, 230, 267, 304, 341 , 341+37 };

#if defined( C_PAY_SYSTEM ) 

	#ifdef APPLY_GUIDE
		
		#ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/22)
			
			MainButtonWnd.x = nRui->Desktop.Width - (1024 - 648);
			MainButtonWnd.y = nRui->Desktop.Height - (768 - 710);
			
		#else
			
			MainButtonWnd.x = 648;
			MainButtonWnd.y = 710;
			
		#endif // #ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/22)
			
			MainButtonWnd.w = 384;
			MainButtonWnd.h = 47;	

	#else// #ifdef APPLY_GUIDE
		
		#ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/22)
			
			MainButtonWnd.x = nRui->Desktop.Width - (1024 - 685);
			MainButtonWnd.y = nRui->Desktop.Height - (768 - 710);
			
		#else
			
			MainButtonWnd.x = 685;
			MainButtonWnd.y = 710;
			
		#endif // #ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/22)
			
			MainButtonWnd.w = 347;
			MainButtonWnd.h = 47;

	#endif// #ifdef APPLY_GUIDE	

	
	#ifdef CHINA_VERSION
		
		#ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/22)
			
			MainButtonWnd.x = nRui->Desktop.Width - (1024 - 648 - 37);
			MainButtonWnd.y = nRui->Desktop.Height - (768 - 710);
			
		#else
			
			MainButtonWnd.x = 648-37 ;
			MainButtonWnd.y = 710;
			
		#endif // #ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/22)
			
			MainButtonWnd.w = 384 +37 ;
			MainButtonWnd.h = 47;
		
	#endif// #ifdef CHINA_VERSION
	
#else// #if defined( C_PAY_SYSTEM ) && !defined( GLOBAL_VERSION )

	#ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/22)
			
		MainButtonWnd.x = nRui->Desktop.Width - (1024 - 685);
		MainButtonWnd.y = nRui->Desktop.Height - (768 - 710);
			
	#else
			
		MainButtonWnd.x = 685;
		MainButtonWnd.y = 710;
			
	#endif // #ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/22)
			
		MainButtonWnd.w = 347;
		MainButtonWnd.h = 47;
			
#endif// #if defined( C_PAY_SYSTEM ) && !defined( GLOBAL_VERSION )
	
#ifdef TAIWAN30MINEVENT
	Ew = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_MainEventButtons_Wnd, WID_None, 
									  MainEventButtonWnd.x, MainEventButtonWnd.y, MainEventButtonWnd.w, MainEventButtonWnd.h, false );	
	Ew->SetClientImage( TID_None );	
	Ew->SetWndProc( this );
	Ew->SetTitle( false );
	Ew->SetVisible( false );
	Ew->SetMove( false );
	Ew->SetRegion( false );
	Ew->SetZorder( n_zoTop );		
	Ew->Initialize();
#endif //TAIWAN30MINEVENT

#ifdef TAIWAN30MINEVENT //대만 출석이벤트
	event_Buttons = (CButton*)nRui->CreateWnd( n_wtButton, WID_MainMenu_EventWndBtn, WID_MainEventButtons_Wnd, 
					   0, 0, 84, 50, false );     
	//event_Buttons->SetBtnImage( TID_OPT_CASHOP_BT, TID_OPT_CASHOP_BT_OVR, TID_OPT_CASHOP_BT_CLK, TID_OPT_FRONTIER_BT );
	event_Buttons->SetBtnImage( TID_OPT_QUEST_BT, TID_OPT_QUEST_BT_OVR, TID_OPT_QUEST_BT_CLK, TID_None );
	event_Buttons->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	event_Buttons->SetHID( HID_TIWAN30EVENT );
	event_Buttons->SetAlignText( n_atCenter, n_atBottom );
	event_Buttons->SetFocusCheck( true );

	event_Text = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_MainEventButtons_Wnd,  0, 0 + 16 , 84, 50, false );  
	event_Text->SetFontg( n_ftGulimChe, 12, n_fwBold, n_fdNormal );
	event_Text->SetClientImage( TID_None );	
	event_Text->SetAlignText( n_atCenter, n_atBottom );
	event_Text->SetText( _RT("99") );
	event_Text->SetFocusCheck(false);

#endif //TAIWAN30MINEVENT
#if defined( C_PAY_SYSTEM )
	
	fw = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_MainButtons_Wnd, WID_None, 
									  MainButtonWnd.x, MainButtonWnd.y, MainButtonWnd.w, MainButtonWnd.h, false );	
	fw->SetClientImage( TID_None );	
	fw->SetWndProc( this );
	fw->SetTitle( false );
	fw->SetVisible( false );
	fw->SetMove( false );
	fw->SetRegion( false );
	fw->SetZorder( n_zoTop );		
	fw->Initialize();


	/*
	mapD_BtnImage[0] = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_MainButtons_Wnd, 
						nButtonXPos[0], 7, 34, 34, false );
	mapD_BtnImage[0]->SetClientImage( TID_None );
	
	*/
	///---------------------------------------------------------------------
	///-- Buttons

	///-- 아이템 샵창
	mapD_Buttons[0] = (CButton*)nRui->CreateWnd( n_wtButton, WID_MainMenu_CashWndBtn, WID_MainButtons_Wnd, 
					   nButtonXPos[0], 7, 34, 34, false );     
	mapD_Buttons[0]->SetBtnImage( TID_OPT_CASHOP_BT, TID_OPT_CASHOP_BT_OVR, TID_OPT_CASHOP_BT_CLK, TID_None );
	mapD_Buttons[0]->SetHID( HD_CASH_SHOP );

	///-- 길드창
	mapD_Buttons[1] = (CButton*)nRui->CreateWnd( n_wtButton, WID_MainMenu_GuildWndBtn, WID_MainButtons_Wnd, 
					   nButtonXPos[1], 7, 34, 34, false );   
	mapD_Buttons[1]->SetBtnImage( TID_OPT_FRONTIER_BT, TID_OPT_FRONTIER_BT_OVR, TID_OPT_FRONTIER_BT_CLK, TID_None );
	mapD_Buttons[1]->SetHID( HID_GUILD );

	///-- 로그창
	mapD_Buttons[2] = (CButton*)nRui->CreateWnd( n_wtButton, WID_MainMenu_LogWndBtn, WID_MainButtons_Wnd, 
					   nButtonXPos[2], 7, 34, 34, false );   
	mapD_Buttons[2]->SetBtnImage( TID_OPT_LOG_BT, TID_OPT_LOG_BT_OVR, TID_OPT_LOG_BT_CLK, TID_None );
	mapD_Buttons[2]->SetHID( HID_LOGWND );
	
	///-- 스탯창
	mapD_Buttons[3] = (CButton*)nRui->CreateWnd( n_wtButton, WID_MainMenu_StatWndBtn, WID_MainButtons_Wnd,
					  nButtonXPos[3], 7, 34, 34, false ); 
	mapD_Buttons[3]->SetBtnImage( TID_OPT_STATE_BT, TID_OPT_STATE_BT_OVR, TID_OPT_STATE_BT_CLK, TID_None );
	mapD_Buttons[3]->SetHID( HID_CHARACTER );
	
	///-- 옵션창
	mapD_Buttons[4] = (CButton*)nRui->CreateWnd( n_wtButton, WID_MainMenu_SkillWndBtn, WID_MainButtons_Wnd, 
					  nButtonXPos[4], 7, 34, 34, false );  
	mapD_Buttons[4]->SetBtnImage( TID_OPT_SKILL_BT, TID_OPT_SKILL_BT_OVR, TID_OPT_SKILL_BT_CLK, TID_None );
	mapD_Buttons[4]->SetHID( HID_SKILL );
	
	///-- 파티창
	mapD_Buttons[5] = (CButton*)nRui->CreateWnd( n_wtButton, WID_MainMenu_MessengerWndBtn, WID_MainButtons_Wnd, 
					  nButtonXPos[5], 7, 34, 34, false );
	mapD_Buttons[5]->SetBtnImage( TID_OPT_PARTY_BT, TID_OPT_PARTY_BT_OVR, TID_OPT_PARTY_BT_CLK, TID_None );
	mapD_Buttons[5]->SetHID( HID_FRIENDS );
	
	///-- 인벤창
	InVenButton = (CItemTransButton*)nRui->CreateWnd( n_wtItemTransButton, WID_MainMenu_InvenWndBtn, WID_MainButtons_Wnd,
					  nButtonXPos[6], 7, 34, 34, false ); 
	InVenButton->SetBtnImage( TID_OPT_INVEN_BT, TID_OPT_INVEN_BT_OVR, TID_OPT_INVEN_BT_CLK, TID_None );
	InVenButton->SetHID(HID_INVENTORY );
	
	///-- 퀘스트창
	QuestButton = (CItemTransButton*)nRui->CreateWnd( n_wtItemTransButton, WID_MainMenu_QuestWndBtn, WID_MainButtons_Wnd,
					  nButtonXPos[7], 7, 34, 34, false );
	QuestButton->SetBtnImage( TID_OPT_QUEST_BT, TID_OPT_QUEST_BT_OVR, TID_OPT_QUEST_BT_CLK, TID_None );
	QuestButton->SetHID( HID_QUEST );

	///-- 가이드창 - 모험백서
#ifdef APPLY_GUIDE
	mapD_Buttons[6] = (CButton*)nRui->CreateWnd( n_wtButton, WID_MainMenu_HelpWndBtn, WID_MainButtons_Wnd, 
					   nButtonXPos[8], 7, 36, 36, false );     
	mapD_Buttons[6]->SetBtnImage( TID_OPT_HELP_BT, TID_OPT_HELP_BT_OVR, TID_OPT_HELP_BT_CLK, TID_None );
	mapD_Buttons[6]->SetHID( HID_HELP );
#endif


	#ifdef CHINA_VERSION
	mapD_Buttons[8] = (CButton*)nRui->CreateWnd( n_wtButton, WID_MainMenu_RadioBroadBtn, WID_MainButtons_Wnd, 
					  nButtonXPos[9], 7, 36, 36, false );     
	mapD_Buttons[8]->SetBtnImage( TID_OPT_RADIO_BROAD_BT, TID_OPT_RADIO_BROAD_BT_OVE, TID_OPT_RADIO_BROAD_BT_CLK, TID_None );
	mapD_Buttons[8]->SetHID( HID_MAP );

	///-- 시스템창
	mapD_Buttons[7] = (CButton*)nRui->CreateWnd( n_wtButton, WID_MainMenu_SystemWndBtn, WID_MainButtons_Wnd, 
					  nButtonXPos[10], 7, 34, 34, false );     
	mapD_Buttons[7]->SetBtnImage( TID_OPT_SYSTEM_BT, TID_OPT_SYSTEM_BT_OVR, TID_OPT_SYSTEM_BT_CLK, TID_None );
	mapD_Buttons[7]->SetHID( HID_SYSTEM );

	#else

#ifdef APPLY_GUIDE

	///-- 시스템창
	mapD_Buttons[7] = (CButton*)nRui->CreateWnd( n_wtButton, WID_MainMenu_SystemWndBtn, WID_MainButtons_Wnd, 
					  nButtonXPos[9], 7, 34, 34, false );     
	mapD_Buttons[7]->SetBtnImage( TID_OPT_SYSTEM_BT, TID_OPT_SYSTEM_BT_OVR, TID_OPT_SYSTEM_BT_CLK, TID_None );
	mapD_Buttons[7]->SetHID( HID_SYSTEM );

#else 
	mapD_Buttons[6] = NULL;

	///-- 시스템창
	mapD_Buttons[7] = (CButton*)nRui->CreateWnd( n_wtButton, WID_MainMenu_SystemWndBtn, WID_MainButtons_Wnd, 
					  nButtonXPos[8], 7, 34, 34, false );     
	mapD_Buttons[7]->SetBtnImage( TID_OPT_SYSTEM_BT, TID_OPT_SYSTEM_BT_OVR, TID_OPT_SYSTEM_BT_CLK, TID_None );
	mapD_Buttons[7]->SetHID( HID_SYSTEM );


#endif




	#endif 
	
	
#else
	
	fw = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_MainButtons_Wnd, WID_None, 
									  MainButtonWnd.x, MainButtonWnd.y, MainButtonWnd.w, MainButtonWnd.h, false );
	fw->SetClientImage( TID_None );	
	fw->SetWndProc( this );
	fw->SetTitle( false );
	fw->SetVisible( false );
	fw->SetMove( false );
	fw->SetRegion( false );
	fw->SetZorder( n_zoTop );		
	fw->Initialize();
	
	///---------------------------------------------------------------------
	///-- Buttons	
		
	///-- 캐쉬창은 안보인다.
	mapD_Buttons[0] = (CButton*)nRui->CreateWnd( n_wtButton, WID_MainMenu_CashWndBtn, WID_MainButtons_Wnd, 
					   0, 0, 0, 0, false );     
	mapD_Buttons[0]->SetBtnImage( TID_OPT_CASHOP_BT, TID_OPT_CASHOP_BT_OVR, TID_OPT_CASHOP_BT_CLK, TID_None );
	mapD_Buttons[0]->SetVisible(false);


	///-- 길드창
	mapD_Buttons[1] = (CButton*)nRui->CreateWnd( n_wtButton, WID_MainMenu_GuildWndBtn, WID_MainButtons_Wnd, 
					   nButtonXPos[0], 7, 34, 34, false );     
	mapD_Buttons[1]->SetBtnImage( TID_OPT_FRONTIER_BT, TID_OPT_FRONTIER_BT_OVR, TID_OPT_FRONTIER_BT_CLK, TID_None );	
	mapD_Buttons[1]->SetHID( HID_GUILD );

	///-- 로그창
	mapD_Buttons[2] = (CButton*)nRui->CreateWnd( n_wtButton, WID_MainMenu_LogWndBtn, WID_MainButtons_Wnd, 
					   nButtonXPos[1], 7, 34, 34, false );     
	mapD_Buttons[2]->SetBtnImage( TID_OPT_LOG_BT, TID_OPT_LOG_BT_OVR, TID_OPT_LOG_BT_CLK, TID_None );
	mapD_Buttons[2]->SetHID( HID_LOGWND );

	
	///-- 스탯창
	mapD_Buttons[3] = (CButton*)nRui->CreateWnd( n_wtButton, WID_MainMenu_StatWndBtn, WID_MainButtons_Wnd, 
					   nButtonXPos[2], 7, 34, 34, false ); 
	mapD_Buttons[3]->SetBtnImage( TID_OPT_STATE_BT, TID_OPT_STATE_BT_OVR, TID_OPT_STATE_BT_CLK, TID_None );
	mapD_Buttons[3]->SetHID( HID_CHARACTER );

	
	///-- 옵션창
	mapD_Buttons[4] = (CButton*)nRui->CreateWnd( n_wtButton, WID_MainMenu_SkillWndBtn, WID_MainButtons_Wnd, 
					   nButtonXPos[3], 7, 34, 34, false ); 
	mapD_Buttons[4]->SetBtnImage( TID_OPT_SKILL_BT, TID_OPT_SKILL_BT_OVR, TID_OPT_SKILL_BT_CLK, TID_None );
	mapD_Buttons[4]->SetHID( HID_SKILL );

	
	///-- 파티창
	mapD_Buttons[5] = (CButton*)nRui->CreateWnd( n_wtButton, WID_MainMenu_MessengerWndBtn, WID_MainButtons_Wnd, 
						nButtonXPos[4], 7, 34, 34, false ); 
	mapD_Buttons[5]->SetBtnImage( TID_OPT_PARTY_BT, TID_OPT_PARTY_BT_OVR, TID_OPT_PARTY_BT_CLK, TID_None );
	mapD_Buttons[5]->SetHID( HID_FRIENDS );

	
	///-- 인벤창
	InVenButton = (CItemTransButton*)nRui->CreateWnd( n_wtItemTransButton, WID_MainMenu_InvenWndBtn, WID_MainButtons_Wnd, 
					    nButtonXPos[5], 7, 34, 34, false );
	InVenButton->SetBtnImage( TID_OPT_INVEN_BT, TID_OPT_INVEN_BT_OVR, TID_OPT_INVEN_BT_CLK, TID_None );
	InVenButton->SetHID( HID_INVENTORY );
		
	///-- 퀘스트창
	QuestButton = (CItemTransButton*)nRui->CreateWnd( n_wtItemTransButton, WID_MainMenu_QuestWndBtn, WID_MainButtons_Wnd, 
						nButtonXPos[6], 7, 34, 34, false );
	QuestButton->SetBtnImage( TID_OPT_QUEST_BT, TID_OPT_QUEST_BT_OVR, TID_OPT_QUEST_BT_CLK, TID_None );
	QuestButton->SetHID( HID_QUEST );

	///-- 가이드창 - 모험백서
	mapD_Buttons[6] = (CButton*)nRui->CreateWnd( n_wtButton, WID_MainMenu_HelpWndBtn, WID_MainButtons_Wnd, 
					   nButtonXPos[7], 7, 36, 36, false );     
	mapD_Buttons[6]->SetBtnImage( TID_OPT_HELP_BT, TID_OPT_HELP_BT_OVR, TID_OPT_HELP_BT_CLK, TID_None );
	mapD_Buttons[6]->SetHID( HID_HELP );
	
	///-- 시스템창
	mapD_Buttons[7] = (CButton*)nRui->CreateWnd( n_wtButton, WID_MainMenu_SystemWndBtn, WID_MainButtons_Wnd, 
					  nButtonXPos[8], 7, 34, 34, false );     
	mapD_Buttons[7]->SetBtnImage( TID_OPT_SYSTEM_BT, TID_OPT_SYSTEM_BT_OVR, TID_OPT_SYSTEM_BT_CLK, TID_None );
	mapD_Buttons[7]->SetHID( HID_SYSTEM );	

#endif		

}

///---------------------------------------------------------------------------
///-- Proc
///---------------------------------------------------------------------------
void CMainButtonWnd::Proc( SEventMessage* EMsg )
{
	bool IsWnd = false;
	
	switch( EMsg->FocusWnd )
	{
#ifdef TAIWAN30MINEVENT
	case WID_MainMenu_EventWndBtn:
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			switch(e_State)
			{
				case n_EVENT_OPEN:
					g_RockClient.Send_SRpCsTaiwan30minEventStart();
				break;
				case n_EVENT_PLAY:
					SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_TIWAN30EVENTPLAY ));
				break;
				case n_EVENT_CLOSE:
					g_RockClient.Send_SRpCsTaiwan30minEvent();
				break;
			}
		}
		break;
#endif //TAIWAN30MINEVENT
	case WID_MainMenu_RadioBroadBtn:
		{
			if( EMsg->Notify == NM_BTN_CLICK )
			{
				if( !g_pRadioBroadcast->IsVisible )
				{
					g_pRadioBroadcast->Open();
				}
				else
				{
					g_pRadioBroadcast->Close();
				}
			}
		}
		break;
	case WID_MainMenu_GuildWndBtn:
		{
			if( EMsg->Notify == NM_BTN_CLICK )
			{
				if(g_Pc.GetPlayer()->IsMyPlayerRender)
				{
					if( !g_FrontierMainWnd.IsVisible )
					{
						g_FrontierMainWnd.Open();
					}
					else
					{
						g_FrontierMainWnd.Close();
					}
				}
			}
		}
		break;
	case WID_MainMenu_CashWndBtn:
		{
			if( EMsg->Notify == NM_BTN_CLICK )
			{
				if( !g_WebShopWnd.IsVisible )
				{
					if(g_RockClient.GetLanguage() == eLang_Korea)
					{
						g_WebShopWnd.OpenURL( L"http://itemmall.sigongon.co.kr/index.asp" , "" );
					}    
					else if(g_RockClient.GetLanguage() == eLang_Indonesia)	
					{
						g_WebShopWnd.OpenURL( L"http://202.78.197.87:8080/" , "" );
					}
					else if(g_RockClient.GetLanguage() == eLang_Singapore)	
					{
						g_WebShopWnd.OpenURL( L"http://203.116.79.36:8080/index.asp" , "" );
					}
					else if(g_RockClient.GetLanguage() == eLang_China)	
					{
						g_WebShopWnd.OpenURL( L"http://shop.guanghei.com/" , "" );
					}
					else if(g_RockClient.GetLanguage() == eLang_Japan)
					{
						g_WebShopWnd.OpenURL( L"http://decoitem.gameleon.jp/" , "" );
					}
					else if(g_RockClient.GetLanguage() == eLang_Taiwan)
					{
//#ifdef _DEBUG
//						g_WebShopWnd.OpenURL( L"http://decomalltest.unalis.com.tw:8080/index.asp" , "" );
//#else 
						g_WebShopWnd.OpenURL( L"http://decomall.unalis.com.tw:8080/index.asp" , "" );
//#endif 
					}
					else if(g_RockClient.GetLanguage() == eLang_HongKong )
					{
						g_WebShopWnd.OpenURL( L"http://runupsoweb.runup.com.my/itemmall" , "");
						//g_WebShopWnd.OpenURL( L"http://so.runup.com.hk/itemmall" , "");
					}
				}
				else
				{
					g_WebShopWnd.Close();
				}
			}
		}
		break;
	case WID_MainMenu_LogWndBtn:
		if( EMsg->Notify == NM_BTN_CLICK )
		{	
			IsWnd = !g_LogWnd.IsVisible;
			nRui->SendEvent( g_LogWnd.m_pFrame->GetID(), n_emSetVisible, (EPARAM)IsWnd, 0, 0, 0 );
		}
		break;
	case WID_MainMenu_StatWndBtn:
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			IsWnd = !CharStateWnd.IsVisible;
			nRui->SendEvent( WID_StatWnd, n_emSetVisible, (EPARAM)IsWnd, 0, 0, 0 );
		}
		break;
	case WID_MainMenu_SkillWndBtn:
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			if(g_Pc.GetPlayer()->IsMyPlayerRender)
			{
				IsWnd = !(g_SkillWnd.IsVisible);
				nRui->SendEvent( WID_SkillWnd, n_emSetVisible, (EPARAM)IsWnd, 0, 0, 0 );
				
				if( IsWnd == false )
				{
					//				nRui->SendEvent( WID_SkillRegistWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
					//				nRui->SendEvent( WID_SkillOverWriteWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
					nRui->SendEvent( WID_SkillLevelUpWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
					//				nRui->SendEvent( WID_SkillInfoWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
				}
			}
		}
		break;
	case WID_MainMenu_MessengerWndBtn:
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			if(g_Pc.GetPlayer()->theMapType != n_FightZone)
			{
				IsWnd = !g_MCommunityWnd.IsVisible;
				g_MCommunityWnd.SetVisible(IsWnd);
			}
		}
		break;
	case WID_MainMenu_InvenWndBtn:
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			if(g_Pc.GetPlayer()->IsMyPlayerRender)
			{
				IsWnd = !InventoryWnd.IsVisible;
#ifdef WIDE_PAY_INVENTORY //유료인벤확장			
				InventoryWnd.Open();
#else
				nRui->SendEvent( WID_InventoryWnd, n_emSetVisible, (EPARAM)IsWnd, 0, 0, 0 );
#endif
			}		
		}
		break;
	case WID_MainMenu_QuestWndBtn:
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			IsWnd = !QuestWnd.IsVisible;
			if( IsWnd )				
			{
				QuestWnd.OpenQuestWnd();
			}
			else
			{
				nRui->SendEvent( WID_Quest_Wnd, n_emSetVisible, (EPARAM)IsWnd, 0, 0, 0 );
			}
		}
		break;
	case WID_MainMenu_SystemWndBtn:
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			IsWnd = !SystemMenuWnd.IsVisible;
			if(IsWnd)
			{
				SystemMenuWnd.Open(g_Pc.GetPlayer()->IsMyPlayerRender);
			}
			else
			{
				SystemMenuWnd.Close();
			}			
		}
		break;

#ifdef APPLY_GUIDE
	case WID_MainMenu_HelpWndBtn:
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			IsWnd = !g_pGuideWnd->IsVisible;
			if(IsWnd)
			{
				g_pGuideWnd->Open();
			}
			else
			{
				g_pGuideWnd->Close();
			}		
		}
		break;
#endif
	}

}

#ifdef TAIWAN30MINEVENT
void CMainButtonWnd::Update()
{
	UpdateNew();
}
void CMainButtonWnd::UpdateOld()
{
	
}

void CMainButtonWnd::UpdateNew()
{
	m_NowTime = timeGetTime();
	m_Cumulative +=  m_NowTime - m_CurTime;
	m_CurTime = m_NowTime;
	event_Text->SetText( ( G_STRING_CHAR( IDS_WND_TIWAN30EVENT ) ) );

	DWORD dw = ( (m_Cumulative-AllTime) / 1000 ) / 60;
	BaseTime = 30 - dw;// 대만 출석 이벤트 제한시간 입력

	if(n_eState == n_eSuccess )
	{	
		if(BaseTime >0)
		{
			e_State = n_EVENT_PLAY;
			Rsprintf(RWCHAR(str) , _RT("%s %d") ,  G_STRING_CHAR(IDS_HELPTIP_REMAIN_TIME) ,BaseTime); 
			event_Text->SetText(RWCHAR(str));
		}	
		else
		{
			e_State = n_EVENT_CLOSE;
			static long quest_time = timeGetTime();
		
			if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( timeGetTime() , quest_time ) , > , 500 ) )
			{
				quest_time = timeGetTime();
				
				OnTwinkle = !OnTwinkle;
			}	
		
			if( OnTwinkle == true )
			{
				event_Buttons->SetBtnImage( TID_OPT_QUEST_BT_OVR, TID_OPT_QUEST_BT_OVR, TID_OPT_QUEST_BT_CLK, TID_None );
			}
			else
			{
				event_Buttons->SetBtnImage( TID_OPT_QUEST_BT, TID_OPT_QUEST_BT_OVR, TID_OPT_QUEST_BT_CLK, TID_None );			
			}	
			event_Text->SetText(( G_STRING_CHAR( IDS_WND_TIWAN30EVENT_COMPLETE ) ) );
		}
	}
	else 
	{
		e_State = n_EVENT_OPEN;
		//event_Buttons->SetFocusCheck( true );
	}

}
#endif //TAIWAN30MINEVENT

///---------------------------------------------------------------------------
///-- LogOff
///---------------------------------------------------------------------------
void CMainButtonWnd::LogOff()
{
	InVenButton->InitData();
}


/*
//-----------------------------------------------------------------------------
void CMainMenuLVgageWnd::Init()
{
	PcParam = nRui->GetPcParamInfo();
	theFontg = Fontman->GetFont( n_ftGulimChe, 12, n_fwNormal, n_fdShadow );
}

//-----------------------------------------------------------------------------
void CMainMenuLVgageWnd::Composition()
{
	
	
	//-------------------------------------------------------------------------
	//
	//	메인메뉴...레벨업 게이지....
	//
	//-------------------------------------------------------------------------
	if( nRui->GetResolution() == n_800X600 )
	{
		CFrameWnd*fw = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_MainMenuLv_Wnd, WID_None, 0, (600 - 128), 97, 128, false );
		fw->SetClientImage( TID_MainMenu_1024X768_Wnd_1 );
		fw->SetCaption( _RT("MainMenu_LV") );
		fw->SetWndProc( this );

		fw->SetTitle( false );
		fw->SetCloseBtn( false );
		fw->SetVisible( false );
		fw->SetMove( false );
		fw->SetRegion( false );
		fw->SetZorder( n_zoTop );
		fw->Initialize();
	}
	else if( nRui->GetResolution() == n_1024X768 )
	{
		//CFrameWnd*fw = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_MainMenuLv_Wnd, WID_None, 0, (768 - 116), 97, 116, false );
		CFrameWnd*fw = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_MainMenuLv_Wnd, WID_None, 0, (768 - 128), 97, 128, false );
		fw->SetClientImage( TID_MainMenu_1024X768_Wnd_1 );
		fw->SetCaption( _RT("MainMenu_LV") );
		fw->SetWndProc( this );
		
		fw->SetTitle( false );
		fw->SetCloseBtn( false );
		fw->SetVisible( false );
		fw->SetMove( false );
		fw->SetRegion( false );
		fw->SetZorder( n_zoTop );
		fw->Initialize();
	}
	
	

	UpGage = (CUpGauge*)nRui->CreateWnd( n_wtUpGauge, WID_MainMenu_LVgage, WID_MainMenuLv_Wnd, 18, 29, 70, 70, false );
	UpGage->SetClientImage( TID_None );
	UpGage->SetBarImage( TID_MainMenu_LVgage );
	UpGage->SetProgressDirection( n_PD_UP );
	UpGage->InitProgress( 0 );
	
	pStateOpenBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_MainMenuLvBtn_Wnd, WID_MainMenuLv_Wnd, 10, 22 , 87, 87, false );
	pStateOpenBtn->SetClientImage( TID_None );
	pStateOpenBtn->SetBtnImage( TID_None, TID_None, TID_None , TID_None );
	pStateOpenBtn->SetVisible( FALSE );

	
	//HighlightImg = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_MainMenu_LVHighlight, WID_MainMenuLv_Wnd, 19, 17, 62, 81, false );
	//HighlightImg->SetClientImage( TID_None );//TID_MainMenu_LVHighlight
	//HighlightImg->SetVisible( false );
}

//-----------------------------------------------------------------------------
void CMainMenuLVgageWnd::Update()
{
	if( (nRui->GetFocusWnd() != WID_MainMenu_LVgage) && 
		(nRui->GetFocusWnd() != WID_MainMenuLvBtn_Wnd) )
	{
		SetToolTipDestWnd(WID_None);
	}
}

//-----------------------------------------------------------------------------
void CMainMenuLVgageWnd::Proc( SEventMessage* EMsg )
{
	
	switch( EMsg->FocusWnd )
	{
		case WID_MainMenu_LVgage:
			{
				SetToolTipDestWnd(WID_MainMenu_LVgage);
			}
			break;
			
		case WID_MainMenuLvBtn_Wnd:
			if( EMsg->Notify == NM_BTN_FOCUS )
			{
				SetToolTipDestWnd(WID_MainMenu_LVgage);		
			}

			if( EMsg->Notify == NM_BTN_CLICK )
			{
				SEventMessage Msg;
				
				memset( &Msg , 0 , sizeof( SEventMessage ) );
				
				Msg.FocusWnd = WID_Stat_LVBtn;
				Msg.Notify   = NM_BTN_CLICK;
				
				CharStateWnd.Proc( &Msg );
			}
			
			break;	
	}
	
}

void CMainMenuLVgageWnd::SetToolTipDestWnd( UINT WIDToolTipDestWnd )
{
	if( m_WIDToolTipDestWnd != WIDToolTipDestWnd )
	{
		m_WIDToolTipDestWnd = WIDToolTipDestWnd;
		m_fCumulTime = 0.0f;
		m_fRatio = 0.0f;
	}
}

void CMainMenuLVgageWnd::RenderToolTipName()
{
	static TCHAR strTemp[256] = _T("Exp");
	static TCHAR strToolTip[512] = _T("");
	float fFadingTime = 0.5f;
	int iTempXpos = 0;
	int iTempYpos = 0;
	int iToolTipXpos = 0;
	int iToolTipYpos = 0;
	
	if( m_WIDToolTipDestWnd != WID_None )
	{
		CWnd *pFDestWnd = Container->Get( WID_MainMenu_LVgage );
		SRect WcSize;
		pFDestWnd->GetClientSize( &WcSize );

		int iNeedExp	= g_Pc_Manager.GetStatNeedExp( PcParam->MainClass, PcParam->Level );
		int iCurExp	= PcParam->Exp;
		
		sprintf(strToolTip, _T("%d/%d"), iCurExp, iNeedExp);
		
		D3DCOLOR tColor = D3DCOLOR_ARGB( (int)(m_fRatio * 255), 255, 200, 0 );	

		SStringInfo StrInfoTemp, StrInfoToolTip;		
		theFontg->GetStringInfo( strTemp, &StrInfoTemp );
		theFontg->GetStringInfo( strToolTip, &StrInfoToolTip );

		iTempXpos = WcSize.x + ((WcSize.w - StrInfoTemp.Width) / 2 );
		iTempYpos = WcSize.y + ((WcSize.h - StrInfoTemp.Height) / 2 ) - (StrInfoTemp.Height / 2);
		
		iToolTipXpos = WcSize.x + ((WcSize.w - StrInfoToolTip.Width) / 2 );
		iToolTipYpos = WcSize.y + ((WcSize.h - StrInfoToolTip.Height) / 2 ) + (StrInfoToolTip.Height / 2);		
		
		Render->DrawText( theFontg, strTemp , iTempXpos, iTempYpos, tColor);	
		Render->DrawText( theFontg, strToolTip , iToolTipXpos, iToolTipYpos, tColor);		

		if( m_fRatio <= 1.0f )
		{
			m_fCumulTime += g_GTimer.GetFrameElapsedSecTime();
			m_fRatio = ( m_fCumulTime / fFadingTime );		
			if( m_fRatio > 1.0f )
		 	  m_fRatio = 1.0f;
		}
	}
}

//-----------------------------------------------------------------------------
*/