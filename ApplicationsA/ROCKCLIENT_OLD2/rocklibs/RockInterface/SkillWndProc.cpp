#include "..\\RockPCH.h"

#include "Rui.h"

#include "Define.h"
#include "Util.h"
#include "Render.h"
#include "Fontman.h"
#include "Resource.h"
#include "Chain.h"
#include "Container.h"

#include "FrameWnd.h"
#include "Button.h"
#include "ImageBox.h"
#include "StateButton.h"
#include "TextBox.h"
#include "EditBox.h"
#include "ProgressBar.h"
#include "SlideBar.h"
#include "CheckButton.h"
#include "RadioButton.h"
#include "ListBox.h"
#include "ScrollBar.h"
#include "ComboBox.h"
#include "Wnd.h"


#include "..\\..\\global.h"
#include "..\\..\\RockClient.h"


#include "..\\..\\quadlist.h"
#include "..\\..\\bravolibs\\network\\Protocol.h"
#include "..\\..\\bravolibs\\obj\\SkillSystem.h"
#include "..\\..\\Pc.h"


#include "SkillWndProc.h"
#include "MainMenuWndProc.h"

#include "SkillRegistWnd.h"
#include "SkillLevelUpWnd.h"

#include "InventoryWndProc.h"
#include "ExchangeWndProc.h"
#include "CUserStoreSellWnd.h"
#include "stringmanager.h"

CSkillWnd		g_SkillWnd;

extern long		g_nowTime;


/*
 
#define MaxStrNum 10000 
#define SID_SkillWnd_Title 0 
#define SID_Combat_Skill 1 
#define SID_NonCombat_Skill 2 
#define SID_SubClass_Skill 3 
#define SID_Active 4 
#define SID_Passive 5 
#define SID_Skill_Point 6 

char g_strKorean[MaxStrNum][256] = {0,};

///sprintf( g_strKorean[SID_SkillWnd_Title], "스킬" );
//sprintf( g_strKorean[SID_Combat_Skill], "전투 기술" );
//sprintf( g_strKorean[SID_NonCombat_Skill], "비전투 기술" );
//sprintf( g_strKorean[SID_SubClass_Skill], "직업 기술" );
//sprintf( g_strKorean[SID_Active], "액티브" );
//sprintf( g_strKorean[SID_Passive], "패시브" );
//sprintf( g_strKorean[SID_Skill_Point], "스킬 포인트" );

pCommon_Control->SetText( _T( g_strKorean[SID_SkillWnd_Title] ) );
*/

int SortSkillNameList( const void *arg1, const void *arg2 );

int SortSkillNameList( const void *arg1, const void *arg2 )
{
	SPcItem** a1 = ( SPcItem** ) arg1;
	SPcItem** a2 = ( SPcItem** ) arg2;


	SSkillBaseInfo* skill_Info_a1;
	skill_Info_a1 = g_Pc_Manager.GetSkillBaseInfoPtr( (*a1)->Code ); 

	SSkillBaseInfo* skill_Info_a2;
	skill_Info_a2 = g_Pc_Manager.GetSkillBaseInfoPtr( (*a2)->Code ); 

	if( skill_Info_a1 == NULL || skill_Info_a2 == NULL )
	{
		return 0;
	}
	
	int comp_result = 0;

	comp_result = Rstrcmp( skill_Info_a1->theSkillName, skill_Info_a2->theSkillName );
	
	if( comp_result < 0 )
	{
		return -1;
	}
	else if( comp_result > 0 )
	{
		return 1;
	}
	else
	{
		return 0;
	}

	return 0;
}


//-----------------------------------------------------------------------------
void CSkillWnd::Init()
{
	m_pPcParam = nRui->GetPcParamInfo();
	m_pPcInven = nRui->GetPcInvenInfo();

	m_SkillTabState = nSkillTab_Active;

	m_SkillPointNum = nRui->GetPcParamInfo()->SkillPoint;
	m_TotSkillPointNum = 0;

	m_InsertHighlight = FALSE;

	SetSkillItem();													// 현재 보유 스킬을 분류한다 
	SetSkillTabState( nSkillTab_Active );							// 액티브 기술, 패시브 기술, 직업 기술 
//	SetSkillSubTabState( nSkillSubTab_Active );						
	/*
	for( int idx = 0; idx < c_Max_SkillSlot; ++idx )
	{
		m_SkillPack[idx].LevelBtn->SetBtnImage( TID_None, TID_Skill_LevelUpBtn_Focus_Active, TID_Skill_LevelUpBtn_Select_Active, TID_Skill_LevelUpBtn_Select_Active );
	}*/

	return;
}


//-----------------------------------------------------------------------------
void CSkillWnd::Composition()
{
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 스킬 윈도우 
// CSkillWnd

// 레벨업 윈도우 
// CSkillLevelUpWnd
// CSkillLevelUpMsgWnd

// 스킬 이름 윈도우
// CSkillInfoWnd
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	int i,j;
	CImageBox*		img;
	int center_x	= nRui->Desktop.Width / 2;
	int center_y	= nRui->Desktop.Height / 2;

	//-------------------------------------------------------------------------
	//
	//	스킬
	//	
	//-------------------------------------------------------------------------
	SRect skillwnd;
	skillwnd.w	= 308;
	skillwnd.h	= 383;
	skillwnd.x	= center_x - ( skillwnd.w / 2 );
	skillwnd.y	= center_y - ( skillwnd.h / 2 );
	
	//스킬 윈도우 프레임
	m_pSkillFWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_SkillWnd, WID_None, skillwnd.x, skillwnd.y, 
												skillwnd.w, skillwnd.h, false );			
	m_pSkillFWnd->SetWndTexStyle( fwfTex_Style_Tile9 );
	m_pSkillFWnd->SetClientImage( TID_CTRL_WND_L1 );
	m_pSkillFWnd->SetIconTID( TID_CTRL_ICON_SKILL );
 	m_pSkillFWnd->SetWndTileSizeLR( 40, 29 );
	m_pSkillFWnd->SetWndTileSizeTB( 31, 11 ); 

	m_pSkillFWnd->SetCaption( G_STRING_CHAR( IDS_WND_SKILL ) );
	m_pSkillFWnd->SetTitle( true );
	
	m_pSkillFWnd->SetWndProc( this );
	m_pSkillFWnd->SetClientColor( 255, 0, 0 );
	m_pSkillFWnd->SetVisible( false );
	m_pSkillFWnd->Initialize();

	//스킬창 내부 배경이미지
	m_pInBgImg = (CImageBox9*)nRui->CreateWnd( n_wtImageBox9, WID_None, WID_SkillWnd, 6 ,  59, 290, 260, false );
	m_pInBgImg->SetWndTileSizeLR( 9, 9 );
	m_pInBgImg->SetWndTileSizeTB( 8, 8 );
	m_pInBgImg->SetFocusCheck( false );
	m_pInBgImg->SetClientImage( TID_CTRL_CONTBACK_L1 );
	m_pInBgImg->SetAdjustPosition( 0, 0 );
	

	///---------------------------------------------------------------------------	
	///-- Static
	///---------------------------------------------------------------------------		
	///-- 슬롯	배경이미지
	slotImg[0] = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_SkillWnd, 9,  64, 137, 51, false );  
	slotImg[1] = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_SkillWnd, 149, 64, 137, 51, false ); 
	slotImg[2] = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_SkillWnd, 9, 114, 137, 51, false );     
	slotImg[3] = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_SkillWnd, 149, 114, 137, 51, false ); 
	slotImg[4] = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_SkillWnd, 9, 164, 137, 51, false );   
	slotImg[5] = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_SkillWnd, 149, 164, 137, 51, false ); 
	slotImg[6] = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_SkillWnd, 9, 214, 137, 51, false );   
	slotImg[7] = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_SkillWnd, 149, 214, 137, 51, false ); 
	slotImg[8] = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_SkillWnd, 9, 264, 137, 51, false );    
	slotImg[9] = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_SkillWnd, 149, 264, 137, 51, false ); 
	
	slotImg[0] ->SetClientImage( TID_C_SKILL_SLOT_A );
	slotImg[1] ->SetClientImage( TID_C_SKILL_SLOT_A );
	slotImg[2] ->SetClientImage( TID_C_SKILL_SLOT_A );
	slotImg[3] ->SetClientImage( TID_C_SKILL_SLOT_A );
	slotImg[4] ->SetClientImage( TID_C_SKILL_SLOT_A );
	slotImg[5] ->SetClientImage( TID_C_SKILL_SLOT_A );
	slotImg[6] ->SetClientImage( TID_C_SKILL_SLOT_A );
	slotImg[7] ->SetClientImage( TID_C_SKILL_SLOT_A );
	slotImg[8] ->SetClientImage( TID_C_SKILL_SLOT_A );
	slotImg[9] ->SetClientImage( TID_C_SKILL_SLOT_A );

	///마름모 이미지
	m_pDiaImg[0] = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_SkillWnd, 143,  110, 9, 9, false );
	m_pDiaImg[1] = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_SkillWnd, 143,  160, 9, 9, false );
	m_pDiaImg[2] = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_SkillWnd, 143,  210, 9, 9, false );
	m_pDiaImg[3] = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_SkillWnd, 143,  260, 9, 9, false );

	m_pDiaImg[0]->SetClientImage( TID_C_SKILL_DIA_A );
	m_pDiaImg[1]->SetClientImage( TID_C_SKILL_DIA_A );
	m_pDiaImg[2]->SetClientImage( TID_C_SKILL_DIA_A );
	m_pDiaImg[3]->SetClientImage( TID_C_SKILL_DIA_A );	
	
	
	///-- 스킬 포인트 배경이미지
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_SkillWnd, 253, 330, 39, 17, false );  
	img->SetClientImage( TID_CTRL_IMG_ELDTEXT35 ); 

	///-- 남은 스킬 포인트 글자
	m_pSkillPointTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_Skill_SkillPointText, WID_SkillWnd, 174, 330, 70, 17, false );
	m_pSkillPointTxt->SetFontR( n_fRsvFontWndStatic );	
	m_pSkillPointTxt->SetAlignText( n_atRight, n_atCenter );
	m_pSkillPointTxt->SetText( G_STRING_CHAR( IDS_REMAIN_SKILL_POINT ) );				
	
	///-- 남은 스킬 포인트 넘버
	m_pSkillPointNumTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_Skill_SkillPointNumText, WID_SkillWnd, 253, 330, 39, 17, false );
	m_pSkillPointNumTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pSkillPointNumTxt->SetClientImage( TID_None );
	m_pSkillPointNumTxt->SetClientColor( 50, 50, 50 );
	m_pSkillPointNumTxt->SetAlignText( n_atCenter, n_atCenter );
	m_pSkillPointNumTxt->SetText( _RT("0") );
	m_pSkillPointNumTxt->SetTextColor( 255, 255, 255 );
	m_pSkillPointNumTxt->SetHID( HID_None );

	///-- 스킬 포인트 배경이미지
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_SkillWnd, 253, 350, 39, 17, false );  
	img->SetClientImage( TID_CTRL_IMG_ELDTEXT35 );
	

	///-- 총 스킬 포인트 글자
	m_pTotSkillPointTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_SkillWnd, 174, 350, 70, 17, false );
	m_pTotSkillPointTxt->SetFontR( n_fRsvFontWndStatic );	
	m_pTotSkillPointTxt->SetAlignText( n_atRight, n_atCenter );
	m_pTotSkillPointTxt->SetText( G_STRING_CHAR( IDS_TOTAL_SKILL_POINT )  );				

	///-- 총 스킬 포인트 넘버
	m_pTotSkillPointNumTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_SkillWnd, 253, 350, 39, 17, false );
	m_pTotSkillPointNumTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pTotSkillPointNumTxt->SetClientImage( TID_None );
	m_pTotSkillPointNumTxt->SetClientColor( 50, 50, 50 );
	m_pTotSkillPointNumTxt->SetAlignText( n_atCenter, n_atCenter );
	m_pTotSkillPointNumTxt->SetText( _RT("0") );
	m_pTotSkillPointNumTxt->SetTextColor( 255, 255, 255 );
	m_pTotSkillPointNumTxt->SetHID( HID_None );

	///---------------------------------------------------------------------------	
	///-- Control
	///---------------------------------------------------------------------------	
	///-- 닫기버튼	
	m_pCloseBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_Skill_CloseBtn, WID_SkillWnd, 285, 7, 13, 13, false );        
	m_pCloseBtn->SetBtnImage( TID_CTRL_BTN_X, TID_CTRL_BTN_X_OVR, TID_CTRL_BTN_X_CLK, TID_None );
	
	///-- 스킬 탭
	m_pActiveSBtn = (CStateButton*)nRui->CreateWnd( n_wtStateButton, WID_Skill_ActiveSBtn, WID_SkillWnd, 
													9, 33, 65, 27, false ); 
	m_pActiveSBtn->SetClientImage( TID_None );
	m_pActiveSBtn->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pActiveSBtn->SetText( G_STRING_CHAR( IDS_ACTIVE ) );	 
	m_pActiveSBtn->SetAlignText( n_atCenter, n_atCenter );
	m_pActiveSBtn->SetTextColor( 0, 0, 0 );
	m_pActiveSBtn->SetBtnImage( TID_C_SKILL_NACT_TAB_A, TID_C_SKILL_NACT_TAB_A, 
								TID_C_SKILL_ACT_TAB_A, TID_None );
	
	m_pPassiveSBtn = (CStateButton*)nRui->CreateWnd( n_wtStateButton, WID_Skill_PassiveSBtn, WID_SkillWnd, 
													 75, 33, 65, 27, false );  
	m_pPassiveSBtn->SetClientImage( TID_None );
	m_pPassiveSBtn->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pPassiveSBtn->SetText( G_STRING_CHAR( IDS_PASSIVE ) );						 
	m_pPassiveSBtn->SetAlignText( n_atCenter, n_atCenter );
	m_pPassiveSBtn->SetTextColor( 0, 0, 0 );
	m_pPassiveSBtn->SetBtnImage( TID_C_SKILL_NACT_TAB_P, TID_C_SKILL_NACT_TAB_P,
								 TID_C_SKILL_ACT_TAB_P, TID_None );	

	m_pSubClassSBtn = (CStateButton*)nRui->CreateWnd( n_wtStateButton, WID_Skill_SubClassSBtn, WID_SkillWnd, 
													141, 33, 65, 27, false );
	m_pSubClassSBtn->SetClientImage( TID_None );
	m_pSubClassSBtn->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pSubClassSBtn->SetText( G_STRING_CHAR( IDS_SKILL_SUB ) );	
	m_pSubClassSBtn->SetAlignText( n_atCenter, n_atCenter );
	m_pSubClassSBtn->SetTextColor( 0, 0, 0 );
	m_pSubClassSBtn->SetBtnImage( TID_C_SKILL_NACT_TAB_J, TID_C_SKILL_NACT_TAB_J, 
								  TID_C_SKILL_ACT_TAB_J, TID_None );
#ifdef C_SUBJOB_RENEW
	m_pSubClassSBtn->SetVisible(false);
#endif 
	nRui->SetGroupWnd( 3, WID_Skill_ActiveSBtn, 
						WID_Skill_ActiveSBtn, WID_Skill_PassiveSBtn, WID_Skill_SubClassSBtn );	
	
	///-- 스크롤바
	m_pScrollBar = (CScrollBar*)nRui->CreateWnd( n_wtScrollBar, WID_Skill_Scroll, WID_SkillWnd, 293, 60, 15, 258, false );
	m_pScrollBar->InitSlide( stVertSld, 0, 45, 0, 13 );//Type, min, max, value, 썸의 폭
	m_pScrollBar->SetClientImage( TID_CTRL_SCR_BG );
	m_pScrollBar->SetThumbImage( TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR_CLK, TID_None );
	m_pScrollBar->SetScrollBtnA( TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT_CLK, TID_None );
	m_pScrollBar->SetScrollBtnB( TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT_CLK, TID_None );
	
	///-- 스킬 슬롯 구성요소 
	///-- 스킬 슬롯 
	for( i = 0 ; i < 5; ++i )
	{
		for( j = 0 ; j < 2; ++j )
		{	
			m_SkillPack[(i * 2) + j].Slot = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Skill_Slot0 + (i * 2) + j, 
											WID_SkillWnd, 17 + j*140,  72 + i * 50, 35, 35, false );
			m_SkillPack[(i * 2) + j].Slot->SetSlotGroup( n_SGROUP_SKILL );
			m_SkillPack[(i * 2) + j].Slot->SetSlotNum( 0 );	
			m_SkillPack[(i * 2) + j].Slot->SetHID( HID_ITEMTIP );	
		}
	}	
	
	///-- 스킬 레벨업 버튼
	for( i = 0 ; i < 5; ++i )
	{
		for( j = 0 ; j < 2; ++j )
		{	
			m_SkillPack[(i * 2) + j].LevelBtn = (CButton*)nRui->CreateWnd( n_wtButton, 
												WID_Skill_SkillLevelBtn0 + (i * 2) + j , 
												WID_SkillWnd, 115 + j*140,  72 + i * 50, 23, 35, false );  
			m_SkillPack[(i * 2) + j].LevelBtn->SetBtnImage( TID_C_SKILL_LUPBTN_IMG1, TID_C_SKILL_LUPBTN_IMG2, 
											   TID_C_SKILL_LUPBTN_IMG2, TID_None );
			m_SkillPack[(i * 2) + j].LevelBtn->SetHID( HID_None );
		}
	}	
	
	///-- 스킬 레벨바
	for( i = 0 ; i < 5; ++i )
	{
		for( j = 0 ; j < 2; ++j )
		{	
			m_SkillPack[(i * 2) + j].LevelBar = (CProgressBar*)nRui->CreateWnd( n_wtProgressBar, WID_None, 
												WID_SkillWnd, 57 + j*140, 72 + i * 50, 53, 19, false );
			m_SkillPack[(i * 2) + j].LevelBar->SetClientImage( TID_None );
			m_SkillPack[(i * 2) + j].LevelBar->SetBarImage( TID_C_SKILL_GAUGE_BAR );
			m_SkillPack[(i * 2) + j].LevelBar->SetBarBlank( 0, 0 );
			m_SkillPack[(i * 2) + j].LevelBar->InitProgress( 100 );
		}
	}	
	
	///-- 스킬 레벨 텍스트	
	for( i = 0 ; i < 5; ++i )
	{
		for( j = 0 ; j < 2; ++j )
		{
			m_SkillPack[(i * 2) + j].LevelTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, 
												WID_SkillWnd, 56 + j*140, 94 + i * 50, 55, 14, false );
			m_SkillPack[(i * 2) + j].LevelTxt->SetFontR( n_fRsvFontWndStatic );	
			m_SkillPack[(i * 2) + j].LevelTxt->SetHID( HID_None );
			m_SkillPack[(i * 2) + j].LevelTxt->SetTextColor( 255, 255, 255);
		}
	}	

	return;
}

//-----------------------------------------------------------------------------
void CSkillWnd::Update()
{	
	static DWORD skillbtn_time = g_nowTime;
	static bool IsDrawEnableState = true;

	//보여질때...
	if( IsVisible )
	{
		//스킬 버튼의 이미지 전환
		if( SAFE_TIME_COMPARE( g_nowTime , > , SAFE_TIME_ADD( skillbtn_time , 500 ) ) )
		{
			IsDrawEnableState = !IsDrawEnableState;
			skillbtn_time = g_nowTime;
		}

		for( int i = 0; i < c_Max_SkillSlot; ++i )
		{
			if( g_ExchangeWnd.m_bIsExchangeLock )
			{
				m_SkillPack[i].Slot->theSlotState = n_Slot_Disable;
			}
			//.................................................................................................
			// 유료 아이템 뿅망치류를 들고 있을때
			//.................................................................................................
			else if( g_Pc.GetPlayer()->m_ISApproachWeapon )
			{
				if( m_SkillPack[i].Slot->PcItem != NULL )
				{
					if( m_SkillPack[i].Slot->PcItem->SkillTable != NULL )
					{
						if( g_Pc_Manager.ISUseSkillOnApproachWeapon( m_SkillPack[i].Slot->PcItem->SkillTable->theSkillCode ) )
						{
							m_SkillPack[i].Slot->theSlotState = n_Slot_Enable;
						}
						else
						{
							m_SkillPack[i].Slot->theSlotState = n_Slot_Disable;
						}
					}
					else
						m_SkillPack[i].Slot->theSlotState = n_Slot_Disable;
				}
				else
					m_SkillPack[i].Slot->theSlotState = n_Slot_Disable;
				
			}
			else if( g_UserStoreSellWnd.ISUserSellMode() )
			{
				if( g_UserStoreSellWnd.m_UserStoreSellOpenMode && m_SkillPack[i].Slot->PcItem != NULL )
				{
					if( m_SkillPack[i].Slot->PcItem->SkillTable != NULL )
					{
						if( m_SkillPack[i].Slot->PcItem->SkillTable->theSkillCode == c_SkillCode_UserSellStore )
						{
							m_SkillPack[i].Slot->theSlotState = n_Slot_Enable;
						}
						else
						{
							m_SkillPack[i].Slot->theSlotState = n_Slot_Disable;
						}
					}
					else
					{
						m_SkillPack[i].Slot->theSlotState = n_Slot_Disable;
					}
				}
				else
					m_SkillPack[i].Slot->theSlotState = n_Slot_Disable;
			}
			else
			{
				m_SkillPack[i].Slot->theSlotState = n_Slot_Enable;
				
				// 구입할 수 없는 아이템들은 Disable 시킨다 
				if( m_SkillPack[i].Slot->PcItem && m_SkillPack[i].Slot->PcItem->SkillTable )
				{			
					if( m_SkillPack[i].Slot->PcItem->SkillTable->theSkillType1 != n_PasiveSkill )
					{
						if( m_SkillPack[i].Slot->PcItem->SkillTable->theSkillCode == c_SkillCode_BasicAttack_Millena || 
							m_SkillPack[i].Slot->PcItem->SkillTable->theSkillCode == c_SkillCode_BasicAttack_Rain )
						{
							m_SkillPack[i].LevelBtn->SetBtnImage( TID_None, TID_None, 
													  TID_None, TID_None );							
							continue;
						}

						bool bAttackTarget = g_Pc.theSkillSystem.CheckAttackTarget( m_SkillPack[i].Slot->PcItem->SkillTable ); 
						bool bTargetStatus = g_Pc.theSkillSystem.CheckTargetStatus( m_SkillPack[i].Slot->PcItem->SkillTable );
						bool bAttackRange = g_Pc.theSkillSystem.CheckAttackRange( m_SkillPack[i].Slot->PcItem->SkillTable );
						bool bNeedStatus = g_Pc.theSkillSystem.CheckNeedStatus( m_SkillPack[i].Slot->PcItem->SkillTable );
						bool bNeedItem = g_Pc.theSkillSystem.CheckNeedItem( m_SkillPack[i].Slot->PcItem->SkillTable );
						
						if( false == bAttackTarget || false == bTargetStatus || 		
							//g_Pc.theSkillSystem.CheckAttackFrame( m_SkillPack[i].Slot->PcItem->SkillTable ) == false || 
							//g_Pc.theSkillSystem.CheckDelayTime( m_SkillPack[i].Slot->PcItem->SkillTable ) == false ||
							false == bAttackRange || false == bNeedStatus || false == bNeedItem )
						{
							m_SkillPack[i].Slot->theSlotState = n_Slot_Disable;
						}
					}
				}		
			}

			//레벨버튼이 홀딩상태가 아니면...
			if( m_SkillPack[i].LevelBtn->GetBtnState() != n_bsHold )
			{
				if( IsDrawEnableState )
				{				
					m_SkillPack[i].LevelBtn->SetBtnImage( TID_C_SKILL_LUPBTN_IMG1, TID_C_SKILL_LUPBTN_IMG2, 
														  TID_C_SKILL_LUPBTN_IMG1, TID_C_SKILL_LUPBTN_IMG1 );
				}		
				else
				{
					m_SkillPack[i].LevelBtn->SetBtnImage( TID_C_SKILL_LUPBTN_IMG2, TID_C_SKILL_LUPBTN_IMG2, 
														  TID_C_SKILL_LUPBTN_IMG1, TID_C_SKILL_LUPBTN_IMG2 );
				}
				
			}
			else
			{
				m_SkillPack[i].LevelBtn->SetBtnImage( TID_None, TID_None, 
													  TID_None, TID_None );
			}
			
		}

		m_SkillPointNum = nRui->GetPcParamInfo()->SkillPoint;
		
		char str_point[10];
		Rsprintf( RWCHAR(str_point), _RT("%d"), m_SkillPointNum );
		m_pSkillPointNumTxt->SetText(  str_point  );
		
		m_TotSkillPointNum = nRui->GetPcParamInfo()->TotalSkillPoint;
		
		Rsprintf( RWCHAR(str_point), _RT("%d"), m_TotSkillPointNum );
		m_pTotSkillPointNumTxt->SetText( str_point  );

		// Skill 습득시 생성 표시 
		if( m_InsertHighlight )
		{
			m_InsertHighlight = false;
			g_main_buttons.mapD_Buttons[4]->SetBtnImage( TID_OPT_SKILL_BT, TID_OPT_SKILL_BT_OVR, TID_OPT_SKILL_BT_CLK, TID_None );
		}
	}

	if( m_InsertHighlight )
	{		
		static long draw_time = timeGetTime();
		static bool OnTwinkle = false;

		if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( timeGetTime() , draw_time ) , > , 500 ) )
		{
			draw_time = timeGetTime();
			OnTwinkle = !OnTwinkle;
		}	
		
		if( OnTwinkle == true )
		{
			g_main_buttons.mapD_Buttons[4]->SetBtnImage( TID_OPT_SKILL_BT_OVR, TID_OPT_SKILL_BT_OVR, TID_OPT_SKILL_BT_CLK, TID_None );
		}
		else
		{
			g_main_buttons.mapD_Buttons[4]->SetBtnImage( TID_OPT_SKILL_BT, TID_OPT_SKILL_BT_OVR, TID_OPT_SKILL_BT_CLK, TID_None );
		}		
	}
	
	return;	
}

//-----------------------------------------------------------------------------
void CSkillWnd::Proc( SEventMessage* EMsg )
{
	int pack_index = 0;

	SRect btn_size;

	switch( EMsg->FocusWnd )
	{
	case WID_Skill_CloseBtn:
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			nRui->SendEvent( WID_SkillWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
			IsVisible = false;
			
			if( IsVisible == false )
			{
				nRui->SendEvent( WID_SkillLevelUpWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
			}
		}
		break;
	// 탭을 누르면 각각의 스킬 표시를 바꾸어 준다
	///-- 엑티브
	case WID_Skill_ActiveSBtn:
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			if( m_SkillTabState != nSkillTab_Active )
			{
				SetSkillTabState( nSkillTab_Active );
			}				
		}
		break;
	///-- 패시브 버튼
	case WID_Skill_PassiveSBtn:
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			if( m_SkillTabState != nSkillTab_Passive )
			{
				SetSkillTabState( nSkillTab_Passive );	
			}	
		}
		break;	
	///-- 서브직업 버튼
	case WID_Skill_SubClassSBtn:
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			if( m_SkillTabState != nSkillTab_SubClass )
			{
				SetSkillTabState( nSkillTab_SubClass );				
			}	
		}
		break;
	// 탭마다 표시되어 있는 스킬들을 스크롤 시킨다 
	case WID_Skill_Scroll:
		if( ( EMsg->Notify == NM_SLD_MOVEBAR ) || ( EMsg->Notify == NM_SLD_CLICKMOVEBAR ) || 
			( EMsg->Notify == NM_SLD_BTNMOVERBAR ) )
		{
			SetSlotScroll( (int)EMsg->fParam );
		}
		break;	
	case WID_Skill_SkillLevelBtn0:
	case WID_Skill_SkillLevelBtn1:
	case WID_Skill_SkillLevelBtn2:
	case WID_Skill_SkillLevelBtn3:
	case WID_Skill_SkillLevelBtn4:
	case WID_Skill_SkillLevelBtn5:
	case WID_Skill_SkillLevelBtn6:
	case WID_Skill_SkillLevelBtn7:
	case WID_Skill_SkillLevelBtn8:
	case WID_Skill_SkillLevelBtn9:
		{
			if( EMsg->Notify == NM_BTN_CLICK )
			{
				pack_index = EMsg->FocusWnd - WID_Skill_SkillLevelBtn0;

/*				static SPcItem testPcItem;
				
				testPcItem.IType = n_ITYPE_SKILL;
				testPcItem.Class = n_ICLASS_NONE;
				testPcItem.ItemUID = 0;
				testPcItem.Code = 84;
				testPcItem.Tid = (TEXID)g_Pc_Manager.GetSkillIconTID( 84 );
				testPcItem.TidEx = (TEXID)g_Pc_Manager.GetSkillAttriIconTID( 84 );
				testPcItem.Amount = 1;
				testPcItem.SkillTable = g_Pc_Manager.GetSkillBaseInfoPtr( 84 );
				
				g_SkillLevelUpWnd.Open( &testPcItem );
*/				
				if( m_SkillPack[pack_index].Slot->PcItem != NULL )
				{
					g_SkillLevelUpWnd.Open( m_SkillPack[pack_index].Slot->PcItem );					
				}
			}
		}
		break;	
	case WID_Skill_Slot0:
	case WID_Skill_Slot1:
	case WID_Skill_Slot2:
	case WID_Skill_Slot3:
	case WID_Skill_Slot4:
	case WID_Skill_Slot5:
	case WID_Skill_Slot6:
	case WID_Skill_Slot7:
	case WID_Skill_Slot8:
	case WID_Skill_Slot9:
		if( EMsg->Notify == NM_SLOT_CLICK )		{	SkillSlotClick( EMsg );		}
		if( EMsg->Notify == NM_SLOT_DBCLICK )	{	SkillSlotDBClick( EMsg );	}
		if( EMsg->Notify == NM_SLOT_DRAG )		{	SkillSlotDrag( EMsg );		}
		if( EMsg->Notify == NM_SLOT_DROP )		{	SkillSlotDrop( EMsg );		}
		break;
	default:
		break;
	}
}

//-----------------------------------------------------------------------------
void CSkillWnd::SetWheelValue( short _wheel )
{
	int value = 0;
	int nMax = 0;

	m_pScrollBar->GetValue( &value );
	nMax = m_pScrollBar->GetSldMax();

	if( ( _wheel < 0 ) && ( value < nMax ) )
	{
		value += 1;
	}
	else if( ( _wheel > 0 ) && ( value > 0 ) )
	{
		value -= 1;
	}
	
	m_pScrollBar->SetValue( value );
	SetSlotScroll( value );
}

//-----------------------------------------------------------------------------
void CSkillWnd::UpdateSkillSlotItem()
{
	SPcItem* tmpPcItem = NULL;
	char str[40] = "";
	
	m_SkillPointNum = nRui->GetPcParamInfo()->SkillPoint;

	SetSkillItem();

	for( int i = 0; i < c_Max_SkillSlot; ++i )
	{
		switch( m_SkillTabState )
		{
		case nSkillTab_Active:
			{				
				tmpPcItem = m_pSkill_Active[m_StartSlotNum + i];
				m_SkillPack[i].Slot->PcItem = tmpPcItem;				
			}
			break;
		case nSkillTab_Passive:
			{
				tmpPcItem = m_pSkill_Passive[i];				
				m_SkillPack[i].Slot->PcItem = tmpPcItem;
			}
			break;
		case nSkillTab_SubClass:
			{
				tmpPcItem = m_pSkill_SubSkill[i];
				m_SkillPack[i].Slot->PcItem = tmpPcItem;
			}
			break;
		}

		if( tmpPcItem != NULL && tmpPcItem->SkillTable != NULL )
		{
			int nSkillLevel = g_SkillLevelUpWnd.GetActiveSKillLevel( tmpPcItem->SkillTable );
			int nMaxLevel = g_SkillLevelUpWnd.GetActiveSKillLevel( tmpPcItem->SkillTable , 1 );
			
			Rsprintf( RWCHAR(str), _RT("Level %d"), nSkillLevel );
			m_SkillPack[i].LevelTxt->SetText( str );
			
			float percent = (float)nSkillLevel/(float)nMaxLevel;			
			m_SkillPack[i].LevelBar->SetPercent( percent * 100 );

			if( nSkillLevel == nMaxLevel) //스킬레벨이 맥스레벨일 경우 
			{
				m_SkillPack[i].LevelBar->SetBarImage(TID_C_SKILL_GAUGE_MASTER);
			}
			else
			{
				m_SkillPack[i].LevelBar->SetBarImage(TID_C_SKILL_GAUGE_BAR);
			}		
			
			if( CheckEnableLevelUpBtn( tmpPcItem ) )
			{
				m_SkillPack[i].LevelBtn->SetBtnHold( false );	
			}
			else
			{
				m_SkillPack[i].LevelBtn->SetBtnHold( true );	
			}	
		}
		else
		{
			m_SkillPack[i].LevelTxt->SetText( _RT( "" ) );
			m_SkillPack[i].LevelBar->SetPercent( 0 );
			m_SkillPack[i].LevelBtn->SetBtnHold( true );
		}
	}	

	return;
}

//-----------------------------------------------------------------------------
void CSkillWnd::SetSkillSystem( CSkillSystem* _skillsystem )
{
	m_pSkillSystem = _skillsystem;
}

//-----------------------------------------------------------------------------
void CSkillWnd::SetSkillItem()
{
	int i;
	int m_NumSkill_Active;
	int	m_NumSkill_Passive;
	int	m_NumSkill_SubClass;
	
	m_NumSkill_Active = 0;
	m_NumSkill_Passive = 0;	
	m_NumSkill_SubClass = 0;

	int skill_num = 0;

	
	InitPassiveSkillCode();

	for( i = 0; i < RP_MAX_USER_SKILL; ++i )
	{
		if( m_pPcInven->SkillItem[i] != NULL )
		{
			++skill_num;
		}
		else
		{
			break;
		}
	}

	qsort( (void *) &m_pPcInven->SkillItem[0], (size_t) skill_num, sizeof( SPcItem* ), SortSkillNameList );	
	
	for( i = 0; i < RP_MAX_USER_SKILL; ++i )
	{
		m_pSkill_Active[i] = NULL;
		m_pSkill_Passive[i] = NULL;
		m_pSkill_SubSkill[i] = NULL;

		if( m_pPcInven->SkillItem[i] != NULL && m_pPcInven->SkillItem[i]->SkillTable != NULL )
		{
			if( m_pPcInven->SkillItem[i]->SkillTable->theSkillType2 == n_Produce )
			{
				if( m_pPcInven->SkillItem[i]->SkillTable->theSkillType1 == n_PasiveSkill )
				{
					SetPassiveSkillCode( m_pPcInven->SkillItem[i]->Code );
				}	
				
				m_pSkill_SubSkill[m_NumSkill_SubClass] = m_pPcInven->SkillItem[i];
				m_NumSkill_SubClass++;
			}
			else 
			{
				if( m_pPcInven->SkillItem[i]->SkillTable->theSkillType1 == n_PasiveSkill )
				{
					SetPassiveSkillCode( m_pPcInven->SkillItem[i]->Code );

					m_pSkill_Passive[m_NumSkill_Passive] = m_pPcInven->SkillItem[i];
					m_NumSkill_Passive++;
				}
				else //if( m_pPcInven->SkillItem[i]->SkillTable->theSkillType1 == n_ActiveSkill )
				{
					m_pSkill_Active[m_NumSkill_Active] = m_pPcInven->SkillItem[i];
					m_NumSkill_Active++;		
				}		
			}

		}
	}

	return;
}

//-----------------------------------------------------------------------------
void CSkillWnd::SetSkillTabState( NSkillTabState state )
{
	SPcItem* tmpPcItem = NULL;
	char str[40] = "";
	
	m_SkillTabState = state;

	m_StartSlotNum	= 0;
	m_pScrollBar ->SetValue( m_StartSlotNum );

	int nCount = 0;
	
	for( int i = 0; i < c_Max_SkillSlot; ++i )
	{
		switch( m_SkillTabState )
		{
		case nSkillTab_Active:
			{
				nRui->SendEvent( WID_Skill_ActiveSBtn, n_emSetChecked, 0, 0, 0, 0 );
				
				tmpPcItem = m_pSkill_Active[i];			
				m_SkillPack[i].Slot->PcItem = tmpPcItem;
				
				m_pActiveSBtn->SetTextColor(255, 255, 255);
				m_pPassiveSBtn->SetTextColor(200, 200, 200);
				m_pSubClassSBtn->SetTextColor(200, 200, 200);

				for( nCount = 0 ; nCount < 10 ; nCount++ )
				{
					slotImg[nCount]->SetClientImage( TID_C_SKILL_SLOT_A );		
				}
				
				for( nCount = 0 ; nCount < 4 ; nCount++ )
				{
					m_pDiaImg[nCount]->SetClientImage( TID_C_SKILL_DIA_A );		
				}
			}
			break;
		case nSkillTab_Passive:
			{
				nRui->SendEvent( WID_Skill_PassiveSBtn, n_emSetChecked, 0, 0, 0, 0 );
				
				tmpPcItem = m_pSkill_Passive[i];		
				m_SkillPack[i].Slot->PcItem = tmpPcItem;

				m_pActiveSBtn->SetTextColor(200, 200, 200);
				m_pPassiveSBtn->SetTextColor(255, 255, 255);
				m_pSubClassSBtn->SetTextColor(200, 200, 200);

				for( nCount = 0 ; nCount < 10 ; nCount++ )
				{
					slotImg[nCount]->SetClientImage( TID_C_SKILL_SLOT_P );		
				}
				
				for( nCount = 0 ; nCount < 4 ; nCount++ )
				{
					m_pDiaImg[nCount]->SetClientImage( TID_C_SKILL_DIA_P );		
				}
			}
			break;
		case nSkillTab_SubClass:
			{
				nRui->SendEvent( WID_Skill_SubClassSBtn, n_emSetChecked, 0, 0, 0, 0 );
				
				tmpPcItem = m_pSkill_SubSkill[i];			
				m_SkillPack[i].Slot->PcItem = tmpPcItem;

				m_pActiveSBtn->SetTextColor(200, 200, 200);
				m_pPassiveSBtn->SetTextColor(200, 200, 200);
				m_pSubClassSBtn->SetTextColor(255, 255, 255);

				for( nCount = 0 ; nCount < 10 ; nCount++ )
				{
					slotImg[nCount]->SetClientImage( TID_C_SKILL_SLOT_J );		
				}
				
				for( nCount = 0 ; nCount < 4 ; nCount++ )
				{
					m_pDiaImg[nCount]->SetClientImage( TID_C_SKILL_DIA_J );		
				}
			}
			break;
		}

		if( tmpPcItem != NULL && tmpPcItem->SkillTable != NULL )
		{
			int nSkillLevel = g_SkillLevelUpWnd.GetActiveSKillLevel( tmpPcItem->SkillTable );
			int nMaxLevel = g_SkillLevelUpWnd.GetActiveSKillLevel( tmpPcItem->SkillTable , 1 );
			
			Rsprintf( RWCHAR(str), _RT("Level %d"), nSkillLevel );
			m_SkillPack[i].LevelTxt->SetText( str);
			
			float percent = 0.0f;
			
			if(0 != nMaxLevel)
			{
				percent = (float)nSkillLevel/(float)nMaxLevel;			
			}
				
			m_SkillPack[i].LevelBar->SetPercent( percent * 100 );

			if( nSkillLevel == nMaxLevel) //스킬레벨이 맥스레벨일 경우 
			{
				m_SkillPack[i].LevelBar->SetBarImage(TID_C_SKILL_GAUGE_MASTER);
			}
			else
			{
				m_SkillPack[i].LevelBar->SetBarImage(TID_C_SKILL_GAUGE_BAR);
			}		
			
			if( CheckEnableLevelUpBtn( tmpPcItem ) )
			{
				m_SkillPack[i].LevelBtn->SetBtnHold( false );	
			}
			else
			{
				m_SkillPack[i].LevelBtn->SetBtnHold( true );	
			}	
		}
		else
		{
			m_SkillPack[i].LevelTxt->SetText( _RT( "" ) );
			m_SkillPack[i].LevelBar->SetPercent( 0 );
			m_SkillPack[i].LevelBtn->SetBtnHold( true );
		}
	}

	return;
}

//-----------------------------------------------------------------------------
void CSkillWnd::SetSlotScroll( int linenum )
{
	SPcItem* tmpPcItem = NULL;
	char str[60] = {0,};

	m_StartSlotNum = linenum * 2;								// 한 슬롯당 스킬 2개씩. 
	
	for( int i = 0; i < c_Max_SkillSlot; ++i )
	{
		switch( m_SkillTabState )
		{
		case nSkillTab_Active:			
			{
				tmpPcItem = m_pSkill_Active[m_StartSlotNum + i];			
				m_SkillPack[i].Slot->PcItem = tmpPcItem;
			}	
			break;
		case nSkillTab_Passive:
			{				

				tmpPcItem = m_pSkill_Passive[m_StartSlotNum + i];			
				m_SkillPack[i].Slot->PcItem = tmpPcItem;
			}
			break;
		case nSkillTab_SubClass:
			{				
				tmpPcItem = m_pSkill_SubSkill[m_StartSlotNum + i];			
				m_SkillPack[i].Slot->PcItem = tmpPcItem;
			}
			break;
		}

		if( tmpPcItem != NULL && tmpPcItem->SkillTable != NULL )
		{
			int nSkillLevel = g_SkillLevelUpWnd.GetActiveSKillLevel( tmpPcItem->SkillTable );
			int nMaxLevel = g_SkillLevelUpWnd.GetActiveSKillLevel( tmpPcItem->SkillTable , 1 );

			Rsprintf( RWCHAR(str), _RT("Level %d"), nSkillLevel );
			m_SkillPack[i].LevelTxt->SetText(  str );
			
			float percent = (float)nSkillLevel/(float)nMaxLevel;			
			m_SkillPack[i].LevelBar->SetPercent( percent * 100 );

			if( nSkillLevel == nMaxLevel) //스킬레벨이 맥스레벨일 경우 
			{
				m_SkillPack[i].LevelBar->SetBarImage(TID_C_SKILL_GAUGE_MASTER);
			}
			else
			{
				m_SkillPack[i].LevelBar->SetBarImage(TID_C_SKILL_GAUGE_BAR);
			}		
			
			if( CheckEnableLevelUpBtn( tmpPcItem ) )
			{
				m_SkillPack[i].LevelBtn->SetBtnHold( false );	
			}
			else
			{
				m_SkillPack[i].LevelBtn->SetBtnHold( true );	
			}	
		}
		else
		{
			m_SkillPack[i].LevelTxt->SetText( _RT( "" ) );
			m_SkillPack[i].LevelBar->SetPercent( 0 );
			m_SkillPack[i].LevelBtn->SetBtnHold( true );
		}
	}
	return;
}

//-----------------------------------------------------------------------------
void CSkillWnd::SkillSlotClick( SEventMessage* EMsg )
{
	CSlot*	cslot	= NULL;//현재 장비 슬롯
	CSlot*	pslot	= NULL;//선택된(pick) 슬롯

	if( nRui->GetCursorItem() != NULL )
	{
		//스킬 창에는 다른 아이템이 올 수 없다....Skip
		cslot = (CSlot*)Container->Get( (WNDID)EMsg->FocusWnd );
		pslot = nRui->GetItemSelectedSlot();
		if( cslot->PcItem == NULL || pslot->PcItem == NULL )
		{
			return;
		}
		
		if( cslot->PcItem == pslot->PcItem )
		{
			nRui->ResetCursorItem();
		}
	}
	else
	{
		cslot = (CSlot*)Container->Get( EMsg->FocusWnd );
		if( cslot->PcItem == NULL )
		{
			return;
		}

		if( cslot->PcItem->SkillTable->theSkillType1 != n_PasiveSkill )
		{
			//커서에 아이템 붙이기 메세지.
			nRui->SetCursorItem( cslot->PcItem );
			nRui->SetItemSelectedSlot( cslot );
		}
	}	
}

//-----------------------------------------------------------------------------
void CSkillWnd::SkillSlotDBClick( SEventMessage* EMsg )
{
	//스킬 사용하기....
	CSlot* slot = (CSlot*)Container->Get( EMsg->FocusWnd );
	if( slot->PcItem )
	{
		//라이딩 중이면 스킬을 사용할수 없다
		if(g_Pc.GetPlayer()->m_State.bRide)
		{
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANNOT_USESKILL_INRIDING ) );									
			return;
		}
				
		if( !(slot->PcItem->IsDelay) )
		{		
			m_pSkillSystem->UseSkillWndMgr( slot->PcItem->Code );
		}
	}
}

//-----------------------------------------------------------------------------
void CSkillWnd::SkillSlotDrag( SEventMessage* EMsg )
{
	CSlot* slot = (CSlot*)Container->Get( EMsg->FocusWnd );
	
	if( ( slot->PcItem != NULL ) && ( EMsg->FocusWnd != (WNDID)EMsg->fParam ) )
	{
		if( slot->PcItem->SkillTable->theSkillType1 != n_PasiveSkill )
		{
			//커서에 아이템 붙이기
			if( nRui->GetCursorItem() == NULL )
			{
				nRui->SetCursorItem( slot->PcItem );
				nRui->SetItemSelectedSlot( slot );
			}
		}
	}
}

//-----------------------------------------------------------------------------
void CSkillWnd::SkillSlotDrop( SEventMessage* EMsg )
{
	
	//현재 선택된 슬롯
	CSlot* tslot = (CSlot*)Container->Get( (WNDID)EMsg->FocusWnd );	
	//현재 포커스가 잡힌 슬롯
	CSlot* sslot = (CSlot*)Container->Get( (WNDID)EMsg->fParam );	

	if( ((WNDID)EMsg->fParam != WID_None ) && ( sslot->GetWindowType() == n_wtSlot ) )
	{
		if( tslot == sslot )	//같으면...	
		{
			nRui->ResetCursorItem();
		}		
		else if( ( tslot->PcItem != NULL ) && ( sslot->Group == n_SGROUP_QUICK ) )
		{
			//퀵슬롯에 아이템이 있다면...삭제...
			if( sslot->PcItem != NULL )
			{
				BYTE position = m_StartSlotNum + sslot->SlotNum;
//				m_pPcInven->QuickSlot[position] = NULL;
				sslot->PcItem = NULL;

				//링크 끊기
				g_RockClient.Send_RpCsUnLinkQuickSlot( position );
			}

			//퀵슬롯이 비어있을때만....링크....
			if( sslot->PcItem == NULL )
			{
				if( tslot->PcItem == NULL )
				{
					return;
				}
				
				//패시브 스킬이 아니면...
				if( tslot->PcItem->SkillTable->theSkillType1 != n_PasiveSkill )
				{
					for( int i = 0; i < RP_MAX_QUICK_SLOT; ++i )
					{
						if( m_pPcInven->QuickSlot[i] == tslot->PcItem )
						{
							g_RockClient.Send_RpCsUnLinkQuickSlot( i );
//							m_pPcInven->QuickSlot[i] = NULL;
							MainMenuWnd.UpdateQuickSlotItem();
							break;
						}
					}

					int StartSlotNum = MainMenuWnd.GetStartSlotNum();

					//퀵슬롯 등록
//					sslot->SetSlotItem( tslot->GetSlotItem() );
//					m_pPcInven->QuickSlot[StartSlotNum + sslot->SlotNum] = tslot->GetSlotItem();

					//스킬코드채우기....
//					m_pSkillSystem->theQuickSlot_SkillCode[sslot->SlotNum] = sslot->PcItem->Code;

					//인벤에서 퀵슬롯으로 링크
					SRpdQuickSlot qslot;
					qslot.theID.theSkillID	= tslot->GetSlotItem()->Code;
					qslot.theType			= n_SKILL;
					qslot.thePosition		= StartSlotNum + sslot->SlotNum;
					g_RockClient.Send_RpCsLinkQuickSlot( qslot ); 

					//커서 리셋
					nRui->ResetCursorItem();
				}
				else
				{
					SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_WEAR_THERE ) );
//					nRui->TextOutChat( _T("SYSTEM_MSG : 액티브 스킬만 등록할 수 있습니다."), D3DCOLOR_XRGB( 255, 255, 0 ) );
				}
			}
		}
	}	
}


//----------------------------------------------------------------------------
bool CSkillWnd::CompareSlot( CSlot* cslot, CSlot* tslot )
{
	if( ( cslot != NULL ) && ( tslot != NULL ) )
	{
		if( cslot->PcItem == NULL )
		{
			return false;
		}

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


void CSkillWnd::InitPassiveSkillCode()
{
	InventoryWnd.theSkillCode_BasicCure = 0;
	InventoryWnd.theSkillCode_MasicPharmacy = 0;
	InventoryWnd.theSkillCode_Sword = 0;
	InventoryWnd.theSkillCode_TwohandSword = 0;
	InventoryWnd.theSkillCode_Bow = 0;
	InventoryWnd.theSkillCode_Shield = 0;
	InventoryWnd.theSkillCode_MagicBook = 0;
	InventoryWnd.theSkillCode_Staff = 0;
	InventoryWnd.theSkillCode_CrystalBall = 0;
	InventoryWnd.theSkillCode_LightArmor = 0;
	InventoryWnd.theSkillCode_MiddleArmor = 0;
	InventoryWnd.theSkillCode_HeavyArmor = 0;
	InventoryWnd.theSkillCode_ServeArmor = 0;
	InventoryWnd.theSkillCode_MasicArmor = 0;
	InventoryWnd.theSkillCode_PriestArmor = 0;
	InventoryWnd.theSkillCode_SpiritArmor = 0;
	InventoryWnd.theSkillCode_CitizenBless = 0;
	g_Pc.GetPlayer()->theActionSystem.theComboSkillCode0 = 0;
	g_Pc.GetPlayer()->theActionSystem.theComboSkillCode1 = 0;
	g_Pc.GetPlayer()->theActionSystem.theComboSkillCode2 = 0;		
	g_Pc.GetPlayer()->theActionSystem.theComboSkillCode7 = 0;
	g_Pc.GetPlayer()->theActionSystem.theChantSkillCode = 0;

	return;
}

// 스킬 창 세팅할때, 스킬 레벨업 할때 등 패시브 스킬 코드 저장해서 사용한다. 
void CSkillWnd::SetPassiveSkillCode( WORD aSkillCode )
{
	// 패시브 스킬 설정 
	// 각각 사용하는 패시브 스킬을 매번 가지고 있는거에서 찾기보다 걍 하나씩 찾아서 가지고 쓰자...ㅋ 	
	
	// 콤보 및 영창 패시브 -------------------------------------------------------------------------------
	if( aSkillCode >= c_SkillCode_ProimaOfUnderstand_Min && aSkillCode <= c_SkillCode_ProimaOfUnderstand_Max )			// 프로이마의 이해 1 ~ 5 (영창 콤보 사용)
	{
		// 패시브 스킬들은 낮은건 안쓰니까..
		if( g_Pc.GetPlayer()->theActionSystem.theChantSkillCode < aSkillCode ||
			g_Pc.GetPlayer()->theActionSystem.theChantSkillCode == c_SkillCode_BasicProimaOfUnderstand )
		{
			g_Pc.GetPlayer()->theActionSystem.theChantSkillCode = aSkillCode;
			g_Pc.GetPlayer()->theActionSystem.theComboSkillCode7 = 0;
			g_Pc.GetPlayer()->theActionSystem.theComboSkillCode1 = 0;
			g_Pc.GetPlayer()->theActionSystem.theComboSkillCode2 = 0;
			g_Pc.GetPlayer()->theActionSystem.theComboSkillCode0 = 0;
		}
	}
	else if( aSkillCode >= c_SkillCode_Synchronization_Min && aSkillCode <= c_SkillCode_Synchronization_Max )			// 동기화 (샤먼 영창 콤보 사용)
	{
		if( g_Pc.GetPlayer()->theActionSystem.theComboSkillCode7 < aSkillCode ||
			g_Pc.GetPlayer()->theActionSystem.theComboSkillCode7 == c_SkillCode_BasicProimaOfUnderstand )
		{
			g_Pc.GetPlayer()->theActionSystem.theChantSkillCode = 0;
			g_Pc.GetPlayer()->theActionSystem.theComboSkillCode7 = aSkillCode;
			g_Pc.GetPlayer()->theActionSystem.theComboSkillCode1 = 0;
			g_Pc.GetPlayer()->theActionSystem.theComboSkillCode2 = 0;
			g_Pc.GetPlayer()->theActionSystem.theComboSkillCode0 = 0;
		}
	}	
	else if( aSkillCode >= c_SkillCode_ArcherOfMind_Min && aSkillCode <= c_SkillCode_ArcherOfMind_Max )			// 아처의 마음가짐 (아처 컨디션 시스템 사용)
	{
		if( g_Pc.GetPlayer()->theActionSystem.theComboSkillCode1 < aSkillCode ||
			g_Pc.GetPlayer()->theActionSystem.theComboSkillCode1 == c_SkillCode_BasicOndHandSwordShip )
		{
			g_Pc.GetPlayer()->theActionSystem.theChantSkillCode = 0;
			g_Pc.GetPlayer()->theActionSystem.theComboSkillCode7 = 0;
			g_Pc.GetPlayer()->theActionSystem.theComboSkillCode1 = aSkillCode;
			g_Pc.GetPlayer()->theActionSystem.theComboSkillCode2 = 0;
			g_Pc.GetPlayer()->theActionSystem.theComboSkillCode0 = 0;
		}
	}		
	else if( aSkillCode >= c_SkillCode_OndHandSwordShip_Min && aSkillCode <= c_SkillCode_OndHandSwordShip_Max )		// 검술 1 ~ 5 (검 콤보 사용)
	{
		if( g_Pc.GetPlayer()->theActionSystem.theComboSkillCode2 < aSkillCode ||
			g_Pc.GetPlayer()->theActionSystem.theComboSkillCode2 == c_SkillCode_BasicOndHandSwordShip )
		{
			g_Pc.GetPlayer()->theActionSystem.theChantSkillCode = 0;
			g_Pc.GetPlayer()->theActionSystem.theComboSkillCode7 = 0;
			g_Pc.GetPlayer()->theActionSystem.theComboSkillCode1 = 0;
			g_Pc.GetPlayer()->theActionSystem.theComboSkillCode2 = aSkillCode;
			g_Pc.GetPlayer()->theActionSystem.theComboSkillCode0 = 0;
		}
	}
	else if( aSkillCode >= c_SkillCode_TwoHandSwordShip_Min && aSkillCode <= c_SkillCode_TwoHandSwordShip_Max )		// 대검술 1 ~ 5 (대검 콤보 사용)
	{
		if( g_Pc.GetPlayer()->theActionSystem.theComboSkillCode0 < aSkillCode ||
			g_Pc.GetPlayer()->theActionSystem.theComboSkillCode0 == c_SkillCode_BasicOndHandSwordShip )
		{
			g_Pc.GetPlayer()->theActionSystem.theChantSkillCode = 0;
			g_Pc.GetPlayer()->theActionSystem.theComboSkillCode7 = 0;
			g_Pc.GetPlayer()->theActionSystem.theComboSkillCode1 = 0;
			g_Pc.GetPlayer()->theActionSystem.theComboSkillCode2 = 0;
			g_Pc.GetPlayer()->theActionSystem.theComboSkillCode0 = aSkillCode;
		}
	}
	// 아이템 사용 ---------------------------------------------------------------------------------------
	else if( aSkillCode >= c_SkillCode_BasicCure_Min && aSkillCode <= c_SkillCode_BasicCure_Max )		// 기초 치료 1 ~ 5 (지속성 치료제 사용)
	{
		if( InventoryWnd.theSkillCode_BasicCure < aSkillCode )
		{
			InventoryWnd.theSkillCode_BasicCure = aSkillCode;
		}
	}
	else if( aSkillCode >= c_SkillCode_MasicPharmacy_Min && aSkillCode <= c_SkillCode_MasicPharmacy_Max )		// 마법약 지식 1 ~ 5 (즉효성 치료제 사용)
	{	
		if( InventoryWnd.theSkillCode_MasicPharmacy < aSkillCode )
		{
			InventoryWnd.theSkillCode_MasicPharmacy = aSkillCode;
		}
	}
	// 장비 착용 (무기) ---------------------------------------------------------------------------------- 
	else if( aSkillCode >= c_SkillCode_OndHandSword_Min && aSkillCode <= c_SkillCode_OndHandSword_Max )		// 검 1 ~ 10 (검 사용)
	{
		if( InventoryWnd.theSkillCode_Sword < aSkillCode )
		{
			InventoryWnd.theSkillCode_Sword = aSkillCode;
		}
	}
	else if( aSkillCode >= c_SkillCode_TwoHandSword_Min && aSkillCode <= c_SkillCode_TwoHandSword_Max )		// 대검 1 ~ 10 (대검 사용)
	{
		if( InventoryWnd.theSkillCode_TwohandSword = aSkillCode )
		{
			InventoryWnd.theSkillCode_TwohandSword = aSkillCode;
		}
	}
	else if( aSkillCode >= c_SkillCode_Bow_Min && aSkillCode <= c_SkillCode_Bow_Max )		// 활 1 ~ 10 (활 사용)
	{
		if( InventoryWnd.theSkillCode_Bow < aSkillCode )
		{
			InventoryWnd.theSkillCode_Bow = aSkillCode;
		}
	}
	else if( aSkillCode >= c_SkillCode_Shield_Min && aSkillCode <= c_SkillCode_Shield_Max )		// 방패 1 ~ 10 (방패 사용)
	{
		if( InventoryWnd.theSkillCode_Shield < aSkillCode )
		{
			InventoryWnd.theSkillCode_Shield = aSkillCode;
		}
	}
	else if( aSkillCode >= c_SkillCode_MagicBook_Min && aSkillCode <= c_SkillCode_MagicBook_Max )		// 마법서 1 ~ 10 (마법서 사용)
	{
		if( InventoryWnd.theSkillCode_MagicBook < aSkillCode )
		{
			InventoryWnd.theSkillCode_MagicBook = aSkillCode;
		}
	}
	else if( aSkillCode >= c_SkillCode_Staff_Min && aSkillCode <= c_SkillCode_Staff_Max )		// 지팡이 1 ~ 10 (지팡이 사용)
	{
		if( InventoryWnd.theSkillCode_Staff < aSkillCode )
		{
			InventoryWnd.theSkillCode_Staff = aSkillCode;
		}
	}
	else if( aSkillCode >= c_SkillCode_CrystalBall_Min && aSkillCode <= c_SkillCode_CrystalBall_Max )		// 수정구 1 ~ 10 (수정구 사용)
	{
		if( InventoryWnd.theSkillCode_CrystalBall < aSkillCode )
		{
			InventoryWnd.theSkillCode_CrystalBall = aSkillCode;
		}
	}
	// 장비 착용 (의복) ---------------------------------------------------------------------------------- 
	else if( aSkillCode >= c_SkillCode_LightArmor_Min && aSkillCode <= c_SkillCode_LightArmor_Max )		// 경장비 1 ~ 10 (경장비 사용)
	{
		if( InventoryWnd.theSkillCode_LightArmor < aSkillCode )
		{
			InventoryWnd.theSkillCode_LightArmor = aSkillCode;
		}
	}
	else if( aSkillCode >= c_SkillCode_MiddleArmor_Min && aSkillCode <= c_SkillCode_MiddleArmor_Max )		// 중장비  1 ~ 10 (중장비 사용)
	{
		if( InventoryWnd.theSkillCode_MiddleArmor < aSkillCode )
		{
			InventoryWnd.theSkillCode_MiddleArmor = aSkillCode;
		}
	}	
	else if( aSkillCode >= c_SkillCode_HeavyArmor_Min && aSkillCode <= c_SkillCode_HeavyArmor_Max )		// 헤비장비  1 ~ 10 (헤비장비 사용)
	{
		if( InventoryWnd.theSkillCode_HeavyArmor < aSkillCode )
		{
			InventoryWnd.theSkillCode_HeavyArmor = aSkillCode;
		}
	}	
	else if( aSkillCode >= c_SkillCode_ServeArmor_Min && aSkillCode <= c_SkillCode_ServeArmor_Max )		// 서브장비  1 ~ 10 (서브장비 사용)
	{
		if( InventoryWnd.theSkillCode_ServeArmor < aSkillCode )
		{
			InventoryWnd.theSkillCode_ServeArmor = aSkillCode;
		}
	}	
	else if( aSkillCode >= c_SkillCode_MasicArmor_Min && aSkillCode <= c_SkillCode_MasicArmor_Max )		// 마도복 1 ~ 10 (마도복 사용)
	{
		if( InventoryWnd.theSkillCode_MasicArmor < aSkillCode )
		{
			InventoryWnd.theSkillCode_MasicArmor = aSkillCode;
		}
	}
	else if( aSkillCode >= c_SkillCode_PriestArmor_Min && aSkillCode <= c_SkillCode_PriestArmor_Max )		// 사제복 1 ~ 10 (사제복 사용)
	{
		if( InventoryWnd.theSkillCode_PriestArmor < aSkillCode )
		{
			InventoryWnd.theSkillCode_PriestArmor = aSkillCode;
		}
	}
	else if( aSkillCode >= c_SkillCode_SpiritArmor_Min && aSkillCode <= c_SkillCode_SpiritArmor_Max )		// 강신복 1 ~ 10 (강신복 사용)
	{
		if( InventoryWnd.theSkillCode_SpiritArmor < aSkillCode )
		{
			InventoryWnd.theSkillCode_SpiritArmor = aSkillCode;
		}
	}	
	// 오오라 스킬 
	else if( aSkillCode == c_SkillCode_MillenaBlessing )		// 밀레나의 가호 
	{
		if( InventoryWnd.theSkillCode_CitizenBless < aSkillCode )
		{
			InventoryWnd.theSkillCode_CitizenBless = aSkillCode;
		}
	}	
	else if( aSkillCode == c_SkillCode_RainBlessing )		// 레인의 가호 
	{
		if( InventoryWnd.theSkillCode_CitizenBless < aSkillCode )
		{
			InventoryWnd.theSkillCode_CitizenBless = aSkillCode;
		}
	}		
	else if( aSkillCode == c_SkillCode_BasicOndHandSwordShip )					// 초급 검술 
	{
		g_Pc.GetPlayer()->theActionSystem.theComboSkillCode0 = aSkillCode;
		g_Pc.GetPlayer()->theActionSystem.theComboSkillCode1 = aSkillCode;
		g_Pc.GetPlayer()->theActionSystem.theComboSkillCode2 = aSkillCode;		
	}
	else if( aSkillCode == c_SkillCode_BasicProimaOfUnderstand )				// 초급 프로이마의 이해 
	{	
		g_Pc.GetPlayer()->theActionSystem.theComboSkillCode7 = aSkillCode;
		g_Pc.GetPlayer()->theActionSystem.theChantSkillCode = aSkillCode;
	}

	return;
}


//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/*void CSkillWnd::SkillShortCutSlotClick( SEventMessage* EMsg )
{
	CSlot*	tslot	= NULL;//현재 장비 슬롯
	CSlot*	sslot	= NULL;//선택된(pick) 슬롯

	if( nRui->GetCursorItem() != NULL )
	{
		//스킬 창에는 다른 아이템이 올 수 없다....Skip
		tslot = (CSlot*)Container->Get( (WNDID)EMsg->FocusWnd );
		sslot = nRui->GetItemSelectedSlot();
		if( sslot->PcItem == NULL )
		{
			return;
		}
		
		if( tslot->PcItem == sslot->PcItem )
		{
			nRui->ResetCursorItem();
		}
		else
		{
			if( sslot->Group )
			{
				switch( sslot->Group )
				{				
				case n_SGROUP_SKILL:
					{		
						if( tslot->PcItem != sslot->PcItem )
						{
							tslot->SetSlotItem( sslot->PcItem );				

							if( tslot->GetID() == WID_Skill_ShortCutSlot_Shift )
							{
								m_pSkillSystem->theShortCut_SkillCode[0] = tslot->PcItem->Code;
								g_RockClient.Send_RpCsHotKey( 0, tslot->PcItem->Code );
							}
							else if( tslot->GetID() == WID_Skill_ShortCutSlot_Ctrl )
							{
								m_pSkillSystem->theShortCut_SkillCode[1] = tslot->PcItem->Code;
								g_RockClient.Send_RpCsHotKey( 1, tslot->PcItem->Code );
							}
						}
						nRui->ResetCursorItem();					
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
	
	return;
}

//-----------------------------------------------------------------------------
void CSkillWnd::SkillShortCutSlotDBClick( SEventMessage* EMsg )
{
	CSlot*		tslot	= NULL;	
	CSlot*		sslot	= NULL;
	

	tslot = (CSlot*)Container->Get( (WNDID)EMsg->FocusWnd );				
	sslot = nRui->GetItemSelectedSlot();
		
	if( tslot->PcItem )
	{
		if( sslot->PcItem )
		{
			if( tslot->PcItem->ItemUID == sslot->PcItem->ItemUID )
			{
				nRui->ResetCursorItem();
			}
		}

		tslot->PcItem = NULL;

		if( tslot->GetID() == WID_Skill_ShortCutSlot_Ctrl )
		{
			m_pSkillSystem->theShortCut_SkillCode[0] = 0;
			g_RockClient.Send_RpCsHotKey( 0, 0 );
		}
		else if( tslot->GetID() == WID_Skill_ShortCutSlot_Shift )
		{
			m_pSkillSystem->theShortCut_SkillCode[1] = 0;
			g_RockClient.Send_RpCsHotKey( 1, 0 );
		}
	}

	return;
}

//-----------------------------------------------------------------------------
void CSkillWnd::SkillShortCutSlotDrag( SEventMessage* EMsg )
{
	return;
}

//-----------------------------------------------------------------------------
void CSkillWnd::SkillShortCutSlotDrop( SEventMessage* EMsg )
{
	return;
}*/

bool CSkillWnd::CheckEnableLevelUpBtn( SPcItem* pPcItem )
{
	bool enable_btn = false;
	SPcItem* tmpPcItem = NULL;
	
	int nSkillLevel = g_SkillLevelUpWnd.GetActiveSKillLevel( pPcItem->SkillTable );
	int nMaxLevel = g_SkillLevelUpWnd.GetActiveSKillLevel( pPcItem->SkillTable , 1 );

	if( nMaxLevel == nSkillLevel )
	{
		return false;
	}
	
	SSkillBaseInfo* next_skill_Info;
	next_skill_Info = g_Pc_Manager.GetSkillBaseInfoPtr( pPcItem->SkillTable->theSkillCode + 1 ); 
	
	if( next_skill_Info == NULL )
		return false;

	if( next_skill_Info->theNeedPoint == 0 ) 
	{
		return false;
	}

	if( next_skill_Info->theNeedLevel > nRui->GetPcParamInfo()->Level ) 
	{
		return false;
	}	
	
	if( m_SkillPointNum >= next_skill_Info->theNeedPoint )	
	{
		enable_btn = true;

		for( int i = 0; i < RP_MAX_USER_SKILL; ++i )
		{
			tmpPcItem = NULL;
			
			switch( m_SkillTabState )
			{
			case nSkillTab_Active:
				{				
					tmpPcItem = m_pSkill_Active[i];				
				}
				break;
			case nSkillTab_Passive:
				{
					tmpPcItem = m_pSkill_Passive[i];
				}
				break;
			case nSkillTab_SubClass:
				{
					tmpPcItem = m_pSkill_SubSkill[i];				
				}
				break;
			}

			if( tmpPcItem == NULL )
				break;

			int nSkillLevel = g_SkillLevelUpWnd.GetActiveSKillLevel( pPcItem->SkillTable ); 
			int nMaxLevel = g_SkillLevelUpWnd.GetActiveSKillLevel( pPcItem->SkillTable, 1 );

			int level_range = nMaxLevel - nSkillLevel; 
			
			if( ( tmpPcItem->SkillTable->theSkillCode > pPcItem->SkillTable->theSkillCode ) && 
				( tmpPcItem->SkillTable->theSkillCode <= ( pPcItem->SkillTable->theSkillCode + level_range ) ) )
			{
				enable_btn = false;
			}			
		}

	}

	return enable_btn;
}



