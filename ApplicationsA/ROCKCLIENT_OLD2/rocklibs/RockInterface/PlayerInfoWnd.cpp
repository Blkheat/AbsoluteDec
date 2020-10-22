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
#include "..\\..\\Pc.h"
#include "..\\..\\MapDataManager.h"

#include "stringmanager.h"
#include "PetMainWndProc.h"



#include "PlayerInfoWnd.h"
#include "PlayerInfoAddWnd.h"
#include "NonPlayerInfoWnd.h"
#include "NonPlayerInfoAddWnd.h"

#include "AliveWnd.h"
#include "BattleZoneWndProc.h"
#include "..\\..\\ListTime.h"

CPlayerInfoWnd	PlayerInfoWnd;

extern long			g_nowTime;


const DWORD c_Max_DrawKeepupItemTime( 1000.0f * 60.0f * 60.0f * 24.0f );		// 24 Hours 

const DWORD c_PetBaseItemTime(  60 * 60 * 24 );		// 24시간이상일경우 기준점


CPlayerInfoWnd::CPlayerInfoWnd()
{
	FlashFrame	= NULL;
	GlowImg		= NULL;
	GlowHPImg	= NULL;
	
	CharImg		= NULL;
	Level		= NULL;
	Name		= NULL;
	PrevHP		= NULL;
	HP			= NULL;
	SPMP		= NULL;
	
	pComboStepBar = NULL;

	pComEffimg[0] = NULL;
	pComEffimg[1] = NULL;
	pComEffimg[2] = NULL;
	pComEffimg[3] = NULL;
	pComEffimg[4] = NULL;
	pComEffimg[5] = NULL;

	pMyMenuBtn = NULL;

	m_pFocusBox = NULL;
	
	memset(PayImgBg , 0 , sizeof(PayImgBg));
	
	m_iPrevHP =0;			
	m_iDamage =0;			

	m_BStartEffect = FALSE; 	
	m_lPrevTime = g_nowTime;
	
	m_fGlowTime = 0.05f;	
	m_fFadeTime = 1.0f;		
	m_fBarAlpha = 0;		
	m_fCumulTime = 0;

	
}


//-----------------------------------------------------------------------------
void CPlayerInfoWnd::Init()
{
	thePcParam	= nRui->GetPcParamInfo();
	thePcInven	= nRui->GetPcInvenInfo();
	
	m_iPrevHP = (float)thePcParam->Stat1.theHP;


	ZeroMemory(m_KeepupItem , sizeof(m_KeepupItem));
	
	if( m_BStartEffect )
	{
		GlowImg->SetVisible(false);
		GlowHPImg->SetVisible(false);
	    FlashFrame->SetAbsolutePosition(m_pOldPos.x, m_pOldPos.y);

		m_fBarAlpha = 255.0f;
	    PrevHP->SetBarAlpha((int)m_fBarAlpha);	  

	    m_lPrevTime = g_nowTime;

		m_fCumulTime = 0;
		m_BStartEffect  = FALSE;
	}

	//*	Keepup_Skill 지금은 주석, 추후 리소스 작업이 완료되면...처리
	for( int i = 0; i < RP_MAX_KEEPUP_SKILL; ++i )
	{
		if( thePcInven->KeepupSkill[i] != 0 )
		{
			SetKeepUpSkillCode( i , thePcInven->KeepupSkill[i] );
			theKeepupSkillSlot[i]->SetVisible( TRUE );
			//theKeepupSkillImg[i]->SetClientImage( (TEXID)g_Pc_Manager.GetSkillContinualIconTID( thePcInven->KeepupSkill[i] ) );
		}
		else
		{
			theKeepupSkillSlot[i]->SetVisible( FALSE );
		}
	}

	for(short i = 0; i < RP_MAX_PAY_CONTENTS; ++i )
	{
		if( thePcInven->PayEffect[i] != 0 )
		{
			SetKeepUpItemCode( i , *(thePcInven->PayEffect[i]) );
			m_pKeepupItemSlot[i]->SetVisible( TRUE );
			m_pKeepupItemText[i]->SetVisible( TRUE );

		}
		else
		{
			m_pKeepupItemSlot[i]->SetVisible( FALSE );
			m_pKeepupItemText[i]->SetVisible( FALSE );
		}
	}

#ifdef KEEPUP_PAYITEM_RENEWAL
	RemainTimeShow();
#endif

	//명성치 초기화 
	InitFameInDun();
	
	//*/
	m_lPrevTime = g_nowTime;
}

//-----------------------------------------------------------------------------
void CPlayerInfoWnd::Composition()
{
	CompositionNew();
}

//
void CPlayerInfoWnd::CompositionNew()
{
	int i = 0;

	FlashFrame = (CFlashFrameWnd*)nRui->CreateWnd( n_wtFlashFrameWnd, WID_PlayerInfo_Wnd, WID_None, 
												   4, 0, 268, 89, false );
	FlashFrame->SetClientImage( TID_MAIN_CHRINFO );	
	FlashFrame->SetCaption( _RT("PlayerInfo") );
	FlashFrame->SetWndProc( this );
	FlashFrame->SetTitle( false );
	FlashFrame->SetMove( false );
	FlashFrame->SetVisible( false );
	FlashFrame->SetZorder( n_zoBottom );
	///--이걸 true로 하면 포커스잡을때 감도가 떨어진다.
	///-- 시간이 오래걸린다는 의미
	///--FlashFrame->SetRegion( true );		
	FlashFrame->Initialize();
	FlashFrame->SetIntervalTime(0.2f);
	FlashFrame->SetFlashImage(TID_PlayerInfo_Warning);	
    FlashFrame->SetFlashColorOption(D3DTOP_MODULATE);
    FlashFrame->SetFlashColor(D3DCOLOR_ARGB(63, 255, 128, 128 ));
    FlashFrame->SetActiveFlash(false);
    FlashFrame->SetFlashImgSize(0, 0, 268, 89);  

	m_pOldPos.x = 0;
	m_pOldPos.y = 0;
	
    GlowImg = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_PlayerInfo_GlowWnd, WID_PlayerInfo_Wnd, 
											0, 0, 268, 84, false );
	GlowImg->SetClientImage(TID_PlayerInfo_GlowWnd);	
	GlowImg->SetGlow(TRUE);
	GlowImg->SetVisible(false);
	GlowImg->SetZorder( n_zoBottom );
	GlowImg->SetFocusCheck(false);

	Level = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_PlayerInfo_LevelText, WID_PlayerInfo_Wnd, 
							80, 14, 25, 13, false );
	Level->SetFontR( n_fRsvFontWndStatic2 );
	Level->SetClientColor( 240, 240, 200 );
	Level->SetBlank( 0, 0 );
	Level->SetFocusCheck(false);

	Name = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_PlayerInfo_NameText, WID_PlayerInfo_Wnd, 
							112, 14, 149, 13, false );
	Name->SetFontR( n_fRsvFontWndStatic );
	Name->SetAlignText( n_atLeft, n_atCenter );		
	Name->SetBlank( 0, 0 );
	Name->SetTextColor( 0, 0, 0 );
	Name->SetFocusCheck(false);

	PrevHP = (CProgressBar*)nRui->CreateWnd( n_wtProgressBar, WID_PlayerInfo_PrevHPGauge, WID_PlayerInfo_Wnd,  
								83, 35, 160, 9, false );   
	PrevHP->SetClientImage( TID_None );
	PrevHP->SetBarImage( TID_PlayerInfo_PrevHP_Gauge );
	PrevHP->SetBarBlank( 0, 0 );
	PrevHP->InitProgress( 100 );
	PrevHP->SetZorder( n_zoBottom );
	PrevHP->SetFocusCheck(false);
			
	HP = (CProgressBar*)nRui->CreateWnd( n_wtProgressBar, WID_PlayerInfo_HPGauge, WID_PlayerInfo_Wnd,  
										83, 35, 160, 9, false );   
	HP->SetClientImage( TID_None );
	HP->SetBarImage( TID_MAIN_GAUGE_HP );
	HP->SetBarBlank( 0, 0 );
	HP->InitProgress( 100 );
	HP->SetFocusCheck(false);

	GlowHPImg = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_PlayerInfo_GlowHPGauge, WID_PlayerInfo_Wnd, 
											83, 35, 160, 9, false );
	GlowHPImg->SetClientImage( TID_None );
	GlowHPImg->SetGlow(TRUE);
	GlowHPImg->SetVisible(false);
	GlowHPImg->SetFocusCheck(false);

	SPMP = (CProgressBar*)nRui->CreateWnd( n_wtProgressBar, WID_PlayerInfo_SPMPGauge, WID_PlayerInfo_Wnd, 
										81, 47, 162, 9, false );  
	SPMP->SetClientImage( TID_None );
	SPMP->SetBarImage( TID_MAIN_GAUGE_MP );
	SPMP->SetBarBlank( 0, 0 );
	SPMP->InitProgress( 100 );
	SPMP->SetFocusCheck(false);
	
	//-------------------------------------------------------------------------
	// HP
//	HPText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_PlayerInfo_HPText, WID_PlayerInfo_Wnd, 110, 42, 100, 12, false );
	HPText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_PlayerInfo_HPText, WID_PlayerInfo_Wnd, 
							  147, 36, 100, 5, false );  
	HPText->SetFontg( n_ftGulimChe, 5, n_fwNormal, n_fdNormal );
	HPText->SetImageNum( true );
	HPText->SetClientImage( TID_None );
	HPText->SetClientColor( 180, 175, 160 );
	HPText->SetAlignText( n_atRight, n_atCenter );
	HPText->SetText( _RT("100/100") );
	HPText->SetTextColor( 255, 220, 220 );
	HPText->SetHID( HD_S_HP );
	HPText->SetFocusCheck(false);

	// MP
//	SMPText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_PlayerInfo_SMPText, WID_PlayerInfo_Wnd, 110, 57, 100, 12, false );
	SMPText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_PlayerInfo_SMPText, WID_PlayerInfo_Wnd, 
							147, 48, 100, 5, false );   
	SMPText->SetFontg( n_ftGulimChe, 5, n_fwNormal, n_fdNormal );
	SMPText->SetImageNum( true );
	SMPText->SetClientImage( TID_None );
	SMPText->SetClientColor( 180, 175, 160 );
	SMPText->SetAlignText( n_atRight, n_atCenter );
	SMPText->SetText( _RT("100/100") );
	SMPText->SetTextColor( 220, 220, 255 );
	SMPText->SetHID( HD_S_MP );
	SMPText->SetFocusCheck(false);
	
	pComboStepBar = (CComboStepBar*)nRui->CreateWnd( n_wtComboStepBar, WID_ComboGauge_Gauge, 
					WID_PlayerInfo_Wnd, 78, 59, 134, 3, false );
	pComboStepBar->SetMaxStep(5);
	pComboStepBar->SetNowStep(0);
	pComboStepBar->SetGabPixelW(1);
	pComboStepBar->SetStepTid(TID_MAIN_COMBO_STEP);
	pComboStepBar->SetStepEffTid(TID_MAIN_COMBO_STEP_EFF);
	pComboStepBar->SetStepSize(26, 3);
	pComboStepBar->SetEffTID(TID_MAIN_COMBO_UP_EFF);
	pComboStepBar->SetEffSize(-2, -2, 138, 7);
	pComboStepBar->SetFocusCheck(false);

	g_Pc.GetPlayer()->theActionSystem.theComboStepInfo_gauge = pComboStepBar;	

	pComEffimg[0] = (CComEffImgBox*)nRui->CreateWnd( n_wtComEffImgBox, WID_PlayerInfo_Level1Img, 
		WID_PlayerInfo_Wnd, 18, 73, 12, 7, false );
	pComEffimg[0]->SetClientImage( TID_MAIN_COMBO_LEVEL1 );
	pComEffimg[0]->SetTransImgID(TID_MAIN_COMBO_TRANSIMG);
	pComEffimg[0]->SetTransImgIDSize(10, 10);
	pComEffimg[0]->SetZorder( n_zoTop );
	pComEffimg[0]->m_pControlStepBar = pComboStepBar;
	pComboStepBar->pComEffImgBox[0] = pComEffimg[0];
	pComEffimg[0]->SetFocusCheck(false);
	
	pComEffimg[1] = (CComEffImgBox*)nRui->CreateWnd( n_wtComEffImgBox, WID_PlayerInfo_Level2Img, 
		WID_PlayerInfo_Wnd, 31, 74, 15, 6, false );
	pComEffimg[1]->SetClientImage( TID_MAIN_COMBO_LEVEL2 );
	pComEffimg[1]->SetTransImgID(TID_MAIN_COMBO_TRANSIMG);
	pComEffimg[1]->SetTransImgIDSize(10, 10);
	pComEffimg[1]->SetZorder( n_zoTop );
	pComEffimg[1]->m_pControlStepBar = pComboStepBar;
	pComboStepBar->pComEffImgBox[1] = pComEffimg[1];
	pComEffimg[1]->SetFocusCheck(false);

	
	pComEffimg[2] = (CComEffImgBox*)nRui->CreateWnd( n_wtComEffImgBox, WID_PlayerInfo_Level3Img, 
		WID_PlayerInfo_Wnd, 46, 66, 18, 13, false );
	pComEffimg[2]->SetClientImage( TID_MAIN_COMBO_LEVEL3 );
	pComEffimg[2]->SetTransImgID(TID_MAIN_COMBO_TRANSIMG);
	pComEffimg[2]->SetTransImgIDSize(10, 10);
	pComEffimg[2]->SetZorder( n_zoTop );
	pComEffimg[2]->m_pControlStepBar = pComboStepBar;
	pComboStepBar->pComEffImgBox[2] = pComEffimg[2];
	pComEffimg[2]->SetFocusCheck(false);

	
	pComEffimg[3] = (CComEffImgBox*)nRui->CreateWnd( n_wtComEffImgBox, WID_PlayerInfo_Level4Img, 
		WID_PlayerInfo_Wnd, 60, 52, 16, 18, false );
	pComEffimg[3]->SetClientImage( TID_MAIN_COMBO_LEVEL4 );
	pComEffimg[3]->SetTransImgID(TID_MAIN_COMBO_TRANSIMG);
	pComEffimg[3]->SetTransImgIDSize(10, 10);
	pComEffimg[3]->SetZorder( n_zoTop );
	pComEffimg[3]->m_pControlStepBar = pComboStepBar;
	pComboStepBar->pComEffImgBox[3] = pComEffimg[3];
	pComEffimg[3]->SetFocusCheck(false);

	
	pComEffimg[4] = (CComEffImgBox*)nRui->CreateWnd( n_wtComEffImgBox, WID_PlayerInfo_Level5Img, 
		WID_PlayerInfo_Wnd, 70, 29,  8, 24, false );
	pComEffimg[4]->SetClientImage( TID_MAIN_COMBO_LEVEL5 );
	pComEffimg[4]->SetTransImgID(TID_MAIN_COMBO_TRANSIMG);
	pComEffimg[4]->SetTransImgIDSize(10, 10);
	pComEffimg[4]->SetZorder( n_zoTop );
	pComEffimg[4]->m_pControlStepBar = pComboStepBar;
	pComboStepBar->pComEffImgBox[4] = pComEffimg[4];
	pComEffimg[4]->SetFocusCheck(false);


	pComEffimg[5] = (CComEffImgBox*)nRui->CreateWnd( n_wtComEffImgBox, WID_None, 
		WID_PlayerInfo_Wnd, 60,  9, 17, 19, false );
	pComEffimg[5]->SetClientImage( TID_MAIN_COMBO_LEVEL6 );
	pComEffimg[5]->SetTransImgID(TID_MAIN_COMBO_TRANSIMG);
	pComEffimg[5]->SetTransImgIDSize(10, 10);
	pComEffimg[5]->SetZorder( n_zoTop );
	pComEffimg[5]->m_pControlStepBar = pComboStepBar;
	pComboStepBar->pComEffImgBox[5] = pComEffimg[5];
	pComEffimg[5]->SetFocusCheck(false);

	m_pFocusBox = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_PlayerInfo_Wnd, 
		  15, 5, 252, 56, false );
	m_pFocusBox->SetClientImage( TID_None );
	m_pFocusBox->SetBtnImage( TID_None, TID_None, TID_None, TID_None );
	m_pFocusBox->SetAlignText( n_atCenter, n_atCenter );	
	m_pFocusBox->SetTextColor(0, 0, 0);

	pMyMenuBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_PlayerInfo_Wnd, 
		  246, 34, 22, 20, false );
	pMyMenuBtn->SetBtnImage( TID_MAIN_MENUBT, TID_MAIN_MENUBT_OVR, TID_MAIN_MENUBT_CLK, TID_None );
	pMyMenuBtn->SetAlignText( n_atCenter, n_atCenter );	
	pMyMenuBtn->SetTextColor(0, 0, 0);
	pMyMenuBtn->SetZorder( n_zoTop );
	pMyMenuBtn->SetHID( HD_MENU );


#ifdef COMBO_UPGRADE
	int j = 0 , layer = 0 ;    
	int sPosX = 78 ;		
	int sPosY = 73 ;
	int Xindex = 0 ; 
	int Yindex = 0 ;

	// Y 증가는 size 25
	
	for(j = 0 , i = 0; j < RP_MAX_KEEPUP_SKILL; ++j )
	{
	
		theKeepupSkillSlot[j] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, 
			WID_None, sPosX+(Xindex*24), sPosY+(Yindex*25), 24, 24, false );
		theKeepupSkillSlot[j]->SetSlotGroup( n_SGROUP_KEEPUPSKILL );
		//	theKeepupSkillSlot[j]->SetHightlightImg( TID_None, TID_None );
		theKeepupSkillSlot[j]->SetHID( HID_ITEMTIP );
		theKeepupSkillSlot[j]->SetVisible( FALSE );
		theKeepupSkillSlot[j]->SetScaleTrans(true);
	
		++Xindex;

		if( j == 9 )
		{
			Xindex = 0 ;
			++Yindex;
		}
				

	}

#else	

	theKeepupSkillSlot[0] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_PlayerInfo_KeepupSkillImg0, WID_None, 78, 73, 24, 24, false );
	theKeepupSkillSlot[0]->SetSlotGroup( n_SGROUP_KEEPUPSKILL );
//	theKeepupSkillSlot[0]->SetHightlightImg( TID_None, TID_None );
	theKeepupSkillSlot[0]->SetHID( HID_ITEMTIP );
	theKeepupSkillSlot[0]->SetVisible( FALSE );
	theKeepupSkillSlot[0]->SetScaleTrans(true);

	theKeepupSkillSlot[1] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_PlayerInfo_KeepupSkillImg1, WID_None, 102, 73, 24, 24, false );
	theKeepupSkillSlot[1]->SetSlotGroup( n_SGROUP_KEEPUPSKILL );
//	theKeepupSkillSlot[1]->SetHightlightImg( TID_None, TID_None );
	theKeepupSkillSlot[1]->SetHID( HID_ITEMTIP );
	theKeepupSkillSlot[1]->SetVisible( FALSE );
	theKeepupSkillSlot[1]->SetScaleTrans(true);
	
	theKeepupSkillSlot[2] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_PlayerInfo_KeepupSkillImg2, WID_None, 126, 73, 24, 24, false );
	theKeepupSkillSlot[2]->SetSlotGroup( n_SGROUP_KEEPUPSKILL );
//	theKeepupSkillSlot[2]->SetHightlightImg( TID_None, TID_None );
	theKeepupSkillSlot[2]->SetHID( HID_ITEMTIP );
	theKeepupSkillSlot[2]->SetVisible( FALSE );
	theKeepupSkillSlot[2]->SetScaleTrans(true);
	
	theKeepupSkillSlot[3] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_PlayerInfo_KeepupSkillImg3, WID_None, 150, 73, 24, 24, false );
	theKeepupSkillSlot[3]->SetSlotGroup( n_SGROUP_KEEPUPSKILL );
//	theKeepupSkillSlot[3]->SetHightlightImg( TID_None, TID_None );
	theKeepupSkillSlot[3]->SetHID( HID_ITEMTIP );
	theKeepupSkillSlot[3]->SetVisible( FALSE );
	theKeepupSkillSlot[3]->SetScaleTrans(true);
	
	theKeepupSkillSlot[4] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_PlayerInfo_KeepupSkillImg4, WID_None, 174, 73, 24, 24, false );
	theKeepupSkillSlot[4]->SetSlotGroup( n_SGROUP_KEEPUPSKILL );
//	theKeepupSkillSlot[4]->SetHightlightImg( TID_None, TID_None );
	theKeepupSkillSlot[4]->SetHID( HID_ITEMTIP );
	theKeepupSkillSlot[4]->SetVisible( FALSE );
	theKeepupSkillSlot[4]->SetScaleTrans(true);
	
	theKeepupSkillSlot[5] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_PlayerInfo_KeepupSkillImg5, WID_None, 78, 98, 24, 24, false );
	theKeepupSkillSlot[5]->SetSlotGroup( n_SGROUP_KEEPUPSKILL );
//	theKeepupSkillSlot[5]->SetHightlightImg( TID_None, TID_None );
	theKeepupSkillSlot[5]->SetHID( HID_ITEMTIP );
	theKeepupSkillSlot[5]->SetVisible( FALSE );
	theKeepupSkillSlot[5]->SetScaleTrans(true);
	
	theKeepupSkillSlot[6] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_PlayerInfo_KeepupSkillImg6, WID_None, 102, 98, 24, 24, false );
	theKeepupSkillSlot[6]->SetSlotGroup( n_SGROUP_KEEPUPSKILL );
//	theKeepupSkillSlot[6]->SetHightlightImg( TID_None, TID_None );
	theKeepupSkillSlot[6]->SetHID( HID_ITEMTIP );
	theKeepupSkillSlot[6]->SetVisible( FALSE );
	theKeepupSkillSlot[6]->SetScaleTrans(true);
	
	theKeepupSkillSlot[7] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_PlayerInfo_KeepupSkillImg7, WID_None, 126, 98, 24, 24, false );
	theKeepupSkillSlot[7]->SetSlotGroup( n_SGROUP_KEEPUPSKILL );
//	theKeepupSkillSlot[7]->SetHightlightImg( TID_None, TID_None );
	theKeepupSkillSlot[7]->SetHID( HID_ITEMTIP );
	theKeepupSkillSlot[7]->SetVisible( FALSE );
	theKeepupSkillSlot[7]->SetScaleTrans(true);
	
	theKeepupSkillSlot[8] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_PlayerInfo_KeepupSkillImg8, WID_None, 150, 98, 24, 24, false );
	theKeepupSkillSlot[8]->SetSlotGroup( n_SGROUP_KEEPUPSKILL );
//	theKeepupSkillSlot[8]->SetHightlightImg( TID_None, TID_None );
	theKeepupSkillSlot[8]->SetHID( HID_ITEMTIP );
	theKeepupSkillSlot[8]->SetVisible( FALSE );
	theKeepupSkillSlot[8]->SetScaleTrans(true);
	
	theKeepupSkillSlot[9] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_PlayerInfo_KeepupSkillImg9, WID_None, 174, 98, 24, 24, false );
	theKeepupSkillSlot[9]->SetSlotGroup( n_SGROUP_KEEPUPSKILL );
//	theKeepupSkillSlot[9]->SetHightlightImg( TID_None, TID_None );
	theKeepupSkillSlot[9]->SetHID( HID_ITEMTIP );
	theKeepupSkillSlot[9]->SetVisible( FALSE );
	theKeepupSkillSlot[9]->SetScaleTrans(true);
#endif


	int xpos = 280;
	int ypos = 10;

	// 유료 아이템 사용 정보 표시 

	int BgXpos = 280;
	int BgYpos = 24 +10; 
#ifdef	KEEPUP_PAYITEM_RENEWAL
	for( i = 0 ; i < 6 ; ++i)
	{
		PayImgBg[i] = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_None, 
			BgXpos, BgYpos , 120, 12 , false );
		PayImgBg[i]->SetClientImage(TID_BLCK_BG);
		PayImgBg[i]->SetClientAlpah(100);
		//PayImgBg[i]->SetGlow(TRUE);
		PayImgBg[i]->SetVisible(false);
		PayImgBg[i]->SetZorder( n_zoBottom );
		PayImgBg[i]->SetFocusCheck(false);
		
		BgYpos += 36;
		
	}
#endif

	for( i = 0; i < RP_MAX_PAY_CONTENTS; ++i )
	{
		m_pKeepupItemSlot[i] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, WID_None, xpos, ypos, 24, 24, false );
		m_pKeepupItemSlot[i]->SetSlotGroup( n_SGROUP_KEEPUP_COMMERCIALITEM );
		m_pKeepupItemSlot[i]->SetHID( HID_ITEMTIP );
		m_pKeepupItemSlot[i]->SetVisible( false );
		m_pKeepupItemSlot[i]->SetScaleTrans( true );
		m_pKeepupItemSlot[i]->SetZorder(n_zoBottom);
		m_pKeepupItemSlot[i]->SetDelayShow(false);

#ifdef	KEEPUP_PAYITEM_RENEWAL
		m_pKeepupItemText[i] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_None,  xpos, ypos + 16 , 24, 22, false );  
#else
		m_pKeepupItemText[i] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_None,  xpos, ypos, 24, 22, false );  
#endif
		m_pKeepupItemText[i]->SetFontg( n_ftGulimChe, 12, n_fwBold, n_fdNormal );
		m_pKeepupItemText[i]->SetClientImage( TID_None );	
		m_pKeepupItemText[i]->SetAlignText( n_atCenter, n_atBottom );
		m_pKeepupItemText[i]->SetText( _RT("99") );
		m_pKeepupItemText[i]->SetFocusCheck(false);
		m_pKeepupItemText[i]->SetVisible(false);

		
		xpos += 24;
		
		if( xpos >= 400 )		// 24 * 5 
		{
			xpos = 280;
#ifdef KEEPUP_PAYITEM_RENEWAL
		ypos += 24 + 12;
#else
		ypos += 24;
#endif
		
		}		
	}

	//-------------------------------------------------------------------------
	// 토템 스코어 
	//--------------------------------------------------------------------------

	int center_x	= nRui->Desktop.Width / 2;
	//0   128   203   67
		///-- Frame Wnd
	FrameWndTotem = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_TOTEMSCORE_WND, WID_None,  711 , 5 , 174 , 50 , false );
	FrameWndTotem->SetClientImage( TID_GAMERESULT_TOTEM_SCORE );
	FrameWndTotem->SetCaption( _RT("") );	 
	FrameWndTotem->SetTitle( false );
	FrameWndTotem->SetWndProc( this );
	FrameWndTotem->SetMove(false);
	FrameWndTotem->SetVisible( false );
	FrameWndTotem->SetZorder( n_zoBottom );
	FrameWndTotem->SetRegion( true );

	FrameWndTotem->SetClientAlpah(190);
	FrameWndTotem->Initialize();
//	FrameWndTotem->SetHID(470);


//	m_pFontg = Fontman->GetFont( n_ftDotum, 20, n_fwBold, n_fdOutline, true, 5 );
	// 글자 
	//SMPText->SetFontg( n_ftGulimChe, 5, n_fwNormal, n_fdNormal );
	Score = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_TOTEMSCORE_WND, 0, 2, 172 , 50, false );
	Score->SetFontg5( n_ftDotum, 26 , n_fwBold, n_fdShadow , true , 10 );
//	Score->SetFontR( n_fRsvNotice );
	Score->SetAlignText( n_atCenter, n_atCenter );	
	Score->SetClientAlpah( 0 );
	Score->SetTextColor( 10, 240, 10 );
	Score->SetTextAlpha(190);
	Score->SetBlank( 0, 0 );
	Score->SetText(_RT("5    3"));

	//-------------------------------------------------------------------------
	// Fame InDunTime 
	//--------------------------------------------------------------------------
	

	
	FrameWndFameInDunTime = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_None, WID_None,  536 , 713 , 0 , 0 , false );
	FrameWndFameInDunTime->SetClientImage( WID_None );
	FrameWndFameInDunTime->SetCaption( _RT("") );	 
	FrameWndFameInDunTime->SetTitle( false );
	FrameWndFameInDunTime->SetWndProc( this );
	FrameWndFameInDunTime->SetMove(false);
	FrameWndFameInDunTime->SetVisible( false );
	FrameWndFameInDunTime->SetZorder( n_zoBottom );
	FrameWndFameInDunTime->SetRegion( false );
	FrameWndFameInDunTime->Initialize();


	FameInDunTimeText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, FrameWndFameInDunTime->GetID() , 0, 2, 124 , 34 , false );
	FameInDunTimeText->SetFontg5( n_ftDotum, 26 , n_fwBold, n_fdOutline , true , 5 );
	//FameInDunTimeText->SetFontR( n_fRsvNotice );
	FameInDunTimeText->SetAlignText( n_atCenter, n_atCenter );	
	FameInDunTimeText->SetClientAlpah( 0 );
	//FameInDunTimeText->SetTextAlpha(190);
	FameInDunTimeText->SetTextColor( 255 , 255 , 255 );
	FameInDunTimeText->SetBlank( 0, 0 );
	FameInDunTimeText->SetText(_RT("45분50초")); 
}



//------------------------------
void CPlayerInfoWnd::Update()
{
	UpdateNew();
}

//
void CPlayerInfoWnd::UpdateNew()
{
	static TCHAR tmpStr[128] = {0,};
	static UINT	uEnLevelID[6] = 
	{
		TID_MAIN_COMBO_ENLEVEL1,
		TID_MAIN_COMBO_ENLEVEL2,
		TID_MAIN_COMBO_ENLEVEL3,
		TID_MAIN_COMBO_ENLEVEL4,
		TID_MAIN_COMBO_ENLEVEL5,
		TID_MAIN_COMBO_ENLEVEL6
	};

	int percent = 0;
    long lElapseTime = SAFE_TIME_SUB( timeGetTime() , m_lPrevTime );
	float fElapsetime = (lElapseTime * 0.001);	

	float fTime = 1.0f;	

	if( m_BStartEffect )
	{
        Name->SetTextColor( 255, 58, 27);		
		m_fCumulTime += fElapsetime;

		if(m_fCumulTime < m_fGlowTime)
		{
		    GlowImg->SetVisible(true);
			GlowHPImg->SetVisible(true);
			
			int PosX, PosY;
			PosX = RandomNum(-3, 3);
			PosY = RandomNum(-3, 3);

			FlashFrame->SetAbsolutePosition(m_pOldPos.x + PosX, m_pOldPos.y + PosY);	
		}
		else
		{
			GlowImg->SetVisible(false);
			GlowHPImg->SetVisible(false);

			FlashFrame->SetAbsolutePosition(m_pOldPos.x, m_pOldPos.y);
		}

		m_fBarAlpha -= ( (fElapsetime / m_fFadeTime ) * 255 );
		if( m_fBarAlpha <= 0 )
			m_fBarAlpha = 0;
		PrevHP->SetBarAlpha((int)m_fBarAlpha);
		
		m_iPrevHP -= ( (fElapsetime / fTime ) );
		
		if( m_fCumulTime >= m_fFadeTime )
		{
		    m_fCumulTime = 0;
			m_BStartEffect = FALSE;
		}
	}
	else
	{
		//연출효과가 없을시 m_iPrevHP를 현재 HP값과 동일화시킨다.
		m_iPrevHP = (float)thePcParam->Stat1.theHP;		
		Name->SetTextColor( 0, 0, 0 );
	}
	int DrawHP  =0 ; 

	DrawHP = thePcParam->Stat1.theHP;
	
	
	if( thePcParam )
	{
		Rsprintf( RWCHAR(tmpStr), _RT("%d"), thePcParam->Level );
		Level->SetText( tmpStr );

		Rsprintf( RWCHAR(tmpStr), _RT("%s"), RWCHAR(thePcParam->PCName) );
		Name->SetText( tmpStr );

		// HP 와 RealHP 가 다른 이유는 Damage를 이벤트에서 가지고 있다가 이펙트 부분에서 처리를 해줘야 하므로 
		// 추후 ScStatus등 HP 갱신 해주는 부분에서 동기가 안맞아서 MaxHP를 넘어서는 등의 처리가 생길 수 있다.
		// MaxHP 가 100이라는 가정하에...
		// ScAttack() thePcParam->Stat1.theHP = 100; realHP = 90; damage = 10; 
		// => ScStatus Start thePcParam->Stat1.theHP = 100, RealHP = 90, aMsg->HP = 100
		// => ScStatus End thePcParam->Stat1.theHP = 110, RealHP = 100, aMsg->HP = 100
		// => ImpactFrm SetUnderAttackEventMgr 에서 Damage 10 처리 thePcParam->Stat1.theHP = 100 = 110 - damage 10; 
		// 그러므로 MaxHP 값은 Over해서 표현될 수 있는 부분에서 눈속임이 필요하다. 
		// 정말 지저분하다...ㅜ  ㅜ 슬프다...흑...
		if( DrawHP > thePcParam->MaxHP )
			DrawHP = thePcParam->MaxHP;

		if( g_Pc.GetPlayer()->m_curt_event.type == SM_STATE_DIE )
		{
			DrawHP = 0;
			//배틀존이 아닐경우에만 죽을때나오는 윈도우를 뛰워준다.
			if( !g_AliveWnd.IsVisible )
			{
				if( g_Pc.GetPlayer()->theMapType == n_FightZone )
				{
					///--JAPAN_BUG_MODIFY
					if( !g_BZ_GameResultWnd.IsVisible && ( g_AliveWnd.m_AliveType != n_AnswerType_Observer ) )
					{
						g_AliveWnd.Open( n_AnswerWndType_Observer_RoomOut );				
					}
				}
				else
				{
					g_AliveWnd.Open( n_AnswerWndType_Alive );				
				}
			}
		}

		if(m_iPrevHP < (float)DrawHP)
			m_iPrevHP = (float)DrawHP;
			
		percent = ( (float)m_iPrevHP / (float)thePcParam->MaxHP ) * 100;
		PrevHP->SetPercent(percent);
		
		percent = ((float)DrawHP / (float)thePcParam->MaxHP) * 100;
		if( ( 0 < percent) &&  ( percent< 30 ) )
		{
			FlashFrame->SetActiveFlash(true);
		}
		else
		{
			FlashFrame->SetActiveFlash(false);
		}
		HP->SetPercent( percent );

		Rsprintf( RWCHAR(tmpStr), _RT("%d/%d"), DrawHP, thePcParam->MaxHP );
		HPText->SetText( tmpStr );

		if( g_Pc.GetPlayer()->GetAttrIndex() == n_Millena_Man || g_Pc.GetPlayer()->GetAttrIndex() == n_Millena_Woman )
		{
			if( thePcParam->Stat1.theSP > thePcParam->MaxSP )
			{
				thePcParam->Stat1.theSP = thePcParam->MaxSP;
			}
			
			SPMP->SetPercent( ((float)thePcParam->Stat1.theSP / (float)thePcParam->MaxSP) * 100 );

			Rsprintf( RWCHAR(tmpStr), _RT("%d/%d"), (int)thePcParam->Stat1.theSP, thePcParam->MaxSP );
			SMPText->SetText( tmpStr );
			SMPText->SetHID( HD_S_SP );
		}
		else if( g_Pc.GetPlayer()->GetAttrIndex() == n_Rain_Man || g_Pc.GetPlayer()->GetAttrIndex() == n_Rain_Woman )
		{
			if( thePcParam->Stat1.theMP > thePcParam->MaxMP)
			{
				thePcParam->Stat1.theMP = thePcParam->MaxMP;
			}

			SPMP->SetPercent( ((float)thePcParam->Stat1.theMP / (float)thePcParam->MaxMP) * 100  );
			
			Rsprintf( RWCHAR(tmpStr), _RT("%d/%d"), (int)thePcParam->Stat1.theMP, thePcParam->MaxMP );
			SMPText->SetText( tmpStr );
			SMPText->SetHID( HD_S_MP );
		}
	}
	
	// 캐릭터 얼굴 그린다.
	if( nRui->m_bIsShowUI && SUCCEEDED( g_RockClient.BeginScene() ) )
	{		
 		g_Pc_Manager.m_CharImage.Render( 6 , 10 , g_Pc.GetPlayer()->GetUnique() , 
 			g_Pc.m_MainPC.char_info.race , CHARIMAGE_64SIZE, g_lpDevice, false );		

		g_RockClient.EndScene();
	}

	m_lPrevTime = timeGetTime();
	
	//사용가능한 콤보레벨 표시하기
	int nEnableSkillLevel = g_Pc.GetPlayer()->theActionSystem.GetEnableSkillLevel();
	
	for(int i = 0 ; i < 6 ; i++)
	{
		if( i < nEnableSkillLevel )
		{
			pComEffimg[i]->SetClientBgImage(uEnLevelID[i]);
		}
		else
		{
			pComEffimg[i]->SetClientBgImage(TID_None);	
		}
	}

	for(short i = 0; i < RP_MAX_PAY_CONTENTS; ++i )
	{
		if( m_pKeepupItemSlot[i]->PcItem == NULL )
		{
			break;
		}

		if( NULL == nRui->GetPcInvenInfo()->PayEffect[i] )
		{
			continue;
		}

#ifdef KEEPUP_PAYITEM_RENEWAL		
		
		tm Time = g_ListTime.GetTimeTm(nRui->GetPcInvenInfo()->PayEffect[i]->theTime);
		if (Time.tm_mday != 0  )
		{
			Rsprintf(RWCHAR(tmpStr) , _RT("%d"), Time.tm_mday );
			m_pKeepupItemText[i]->SetTextColor(0,255,255) ;
		}
		else if(Time.tm_hour != 0  )
		{
			Rsprintf(RWCHAR(tmpStr) , _RT("%d"), Time.tm_hour );
			m_pKeepupItemText[i]->SetTextColor(0,255,0) ;
		}
		else if(Time.tm_min	!= 0 )
		{
			Rsprintf(RWCHAR(tmpStr) , _RT("%d"), Time.tm_min );
			m_pKeepupItemText[i]->SetTextColor(255,255,255) ;
		}
		else 
		{
			Rsprintf(RWCHAR(tmpStr) , _RT("%d"), 1 );
			m_pKeepupItemText[i]->SetTextColor(255,255,255) ;
		}

		m_pKeepupItemText[i]->SetText(tmpStr);
#else
		m_pKeepupItemSlot[i]->PcItem->CumulativeTime += lElapseTime;
		//분만 표시해 준다. 
		int WaitTime = (int)m_pKeepupItemSlot[i]->PcItem->ProgressTime - (int)m_pKeepupItemSlot[i]->PcItem->CumulativeTime;
		int iTime = (WaitTime / 1000)/60;
	
		if(iTime > 0 )
		{
			Rsprintf(RWCHAR(tmpStr) , _RT("%d"), iTime  );
			m_pKeepupItemText[i]->SetText(tmpStr);

		}
		else
		{
			m_pKeepupItemText[i]->SetText(_RT(""));	
		}
		
		if( ( m_pKeepupItemSlot[i]->PcItem->DelayTime * 1000.0f ) > c_Max_DrawKeepupItemTime )
		{
		//	m_pKeepupItemSlot[i]->PcItem->Amount = ( ( ( m_pKeepupItemSlot[i]->PcItem->DelayTime * 1000.0f ) - m_pKeepupItemSlot[i]->PcItem->CumulativeTime ) / c_Max_DrawKeepupItemTime ) + 1;

			if( m_pKeepupItemSlot[i]->PcItem->CumulativeTime > c_Max_DrawKeepupItemTime &&
				m_pKeepupItemSlot[i]->PcItem->IsDelay == false )
			{
				m_pKeepupItemSlot[i]->PcItem->IsDelay = true;				
				m_pKeepupItemSlot[i]->PcItem->ProgressTime *= 1000.0f;
				m_pKeepupItemSlot[i]->PcItem->ProgressTime -= c_Max_DrawKeepupItemTime;		
				m_pKeepupItemSlot[i]->PcItem->CumulativeTime -= c_Max_DrawKeepupItemTime;
			}
		}

#endif


	
	}
	
	/********************************************************************
	- Pet 지속 아이템 
	*********************************************************************/
	
	for(short i =0 ;  i < 3 ; ++i )
	{

		for(int  j = 0; j < RP_MAX_PET_USEDITEM; ++j )
		{
			SPetText &m_PetText = g_PetMainWndProc.m_PetText[i];

			if( m_PetText.KeepupItemSlot[j]->PcItem == NULL )
			{
				break;
			}
			
			m_PetText.KeepupItemSlot[j]->PcItem->CumulativeTime += lElapseTime;
#ifdef PET_PAY_USEITEM_RENEWAL
			float WaitTime = (((m_PetText.KeepupItemSlot[j]->PcItem->CumulativeTime/m_PetText.KeepupItemSlot[j]->PcItem->ProgressTime)
								*m_PetText.KeepupItemSlot[j]->PcItem->DelayTime)); // 유료아이템 시작일부터 지나간 시간
			m_PetText.fRemainTime[j] = (m_PetText.KeepupItemSlot[j]->PcItem->DelayTime) - WaitTime;

			m_PetText.KeepupItemSlot[j]->SetVisible(false);// 그리기전 없애주고
			m_PetText.KeepupItemText[j]->SetVisible(false);
#else 		

			float WaitTime = (((m_PetText.KeepupItemSlot[j]->PcItem->CumulativeTime/m_PetText.KeepupItemSlot[j]->PcItem->ProgressTime)
								*m_PetText.KeepupItemSlot[j]->PcItem->DelayTime)/60);
			m_PetText.fRemainTime[j] = (m_PetText.KeepupItemSlot[j]->PcItem->DelayTime/60) - WaitTime;


#endif //PET_PAY_USEITEM_RENEWAL
			if((int)m_PetText.fRemainTime[j] > 0 )			
			{
#ifdef PET_PAY_USEITEM_RENEWAL
				m_PetText.KeepupItemSlot[j]->SetVisible(true); // 확인후 다시체크
				m_PetText.KeepupItemText[j]->SetVisible(true);

				if((int)m_PetText.fRemainTime[j] > c_PetBaseItemTime) //24시간기준으로 표시결정
				{
					Rsprintf(RWCHAR(tmpStr) , _RT("%d"), (int)m_PetText.fRemainTime[j] /60 /60 /24 ); //Day	
					m_PetText.KeepupItemText[j]->SetText(tmpStr);
					m_PetText.KeepupItemText[j]->SetTextColor( 0 , 255 , 255 );
				}
				else if((int)m_PetText.fRemainTime[j] < c_PetBaseItemTime && (int)m_PetText.fRemainTime[j] > 3600) 
				{
					Rsprintf(RWCHAR(tmpStr) , _RT("%d"), (int)m_PetText.fRemainTime[j] /60 /60 );//Hour	
					m_PetText.KeepupItemText[j]->SetText(tmpStr);
					m_PetText.KeepupItemText[j]->SetTextColor( 0 , 255 , 0 );
				}
				else
				{
					Rsprintf(RWCHAR(tmpStr) , _RT("%d"), (int)m_PetText.fRemainTime[j] /60 ); //Min	
					m_PetText.KeepupItemText[j]->SetText(tmpStr);
					m_PetText.KeepupItemText[j]->SetTextColor( 255 , 255 , 255 );
				}
#else
				Rsprintf(RWCHAR(tmpStr) , _RT("%d"), (int)m_PetText.fRemainTime[j] );	
				m_PetText.KeepupItemText[j]->SetText(tmpStr);
#endif // PET_PAY_USEITEM_RENEWAL
			}
			else
			{
				m_PetText.KeepupItemText[j]->SetText(_RT(""));
			}

			if( ( m_PetText.KeepupItemSlot[j]->PcItem->DelayTime * 1000.0f ) > c_Max_DrawKeepupItemTime )
			{
			//	m_PetText.KeepupItemSlot[j]->PcItem->Amount = ( ( ( m_PetText.KeepupItemSlot[j]->PcItem->DelayTime * 1000.0f ) - m_pKeepupItemSlot[i]->PcItem->CumulativeTime ) / c_Max_DrawKeepupItemTime ) + 1;
				
				if( m_PetText.KeepupItemSlot[j]->PcItem->CumulativeTime > c_Max_DrawKeepupItemTime &&
					m_PetText.KeepupItemSlot[j]->PcItem->IsDelay == false )
				{
					m_PetText.KeepupItemSlot[j]->PcItem->IsDelay = true;				
					m_PetText.KeepupItemSlot[j]->PcItem->ProgressTime *= 1000.0f;
					m_PetText.KeepupItemSlot[j]->PcItem->ProgressTime -= c_Max_DrawKeepupItemTime;		
					m_PetText.KeepupItemSlot[j]->PcItem->CumulativeTime -= c_Max_DrawKeepupItemTime;
				}
			}			
		}	
	}

	//한번이라두 정보가 들어오면 ... 
	//by dongs 레텔 토템 갯수 인것이다 .. 
	if(nRui->GetPcInvenInfo()->TotemScore.bShow)	
	{
		//토템 갯수를 출력한다 .. 
		if( g_Map.m_NowMapID == c_RockMap_LetteField_ForgetOfPlain43 ) 
		{
			if( FrameWndTotem->IsVisible() == false)
			{
				FrameWndTotem->SetVisible(true);
			}
			Rsprintf( RWCHAR(tmpStr), _RT("%d      %d"), nRui->GetPcInvenInfo()->TotemScore.Millena , nRui->GetPcInvenInfo()->TotemScore.Rain );
			Score->SetText(tmpStr);
			
		}
		else
		{
			
			if( FrameWndTotem->IsVisible() == true)
			{
				FrameWndTotem->SetVisible(false);
			}

			//nRui->SendEvent( FrameWndTotem->GetID() , n_emSetVisible, (EPARAM)false, 0, 0, 0 );
		}		
	}

	//명성치 인던시간 
	ShowFameInDun();	
	return;
}

//-----------------------------------------------------------------------------
void CPlayerInfoWnd::Proc( SEventMessage* EMsg )
{
	if( EMsg->FocusWnd == m_pFocusBox->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{	
			if( NonPlayerInfoWnd.thePickedUserID != MAX_USER_INTABLE )
			{
				// 부가효과 (강제타겟) 처리 
				if( g_Pc.GetPlayer()->GetAddEffectState(n_TypeProvoke))
				{
					return;
				}				
				
				CWnd *Wnd = NULL;			
				TCHAR strTemp[256] = {0,};
				nRui->SendEvent( WID_NonPlayerInfo_Wnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
				Wnd = Container->Get( WID_NonPlayerInfo_LevelText );
				Rsprintf( RWCHAR(strTemp), _RT("%d"), nRui->GetPcParamInfo()->Level );
				Wnd->ReceiveEvent(n_emSetText, (EPARAM)strTemp, 240, 240, 200 );		
				Wnd = Container->Get( WID_NonPlayerInfo_NameText );
				Wnd->ReceiveEvent(n_emSetText, (EPARAM)thePcParam->PCName, 240, 240, 200 );

				NonPlayerInfoWnd.thePickedNpcID = -1;
				NonPlayerInfoWnd.thePickedUserID = MAX_USER_INTABLE;
				NonPlayerInfoWnd.SetPickCharacter( ( Character * )g_Pc.GetPlayer() );
				
				if( NonPlayerInfoAddWnd.IsVisible )
				{
					nRui->SendEvent( WID_NonPlayerInfoAddWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
					if(g_NonPlayerInfoAdd2Wnd.IsVisible)
					{
						g_NonPlayerInfoAdd2Wnd.Close();
					}			
				}	
			}

		}
	}	
	
	if( EMsg->FocusWnd == pMyMenuBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			PlayerInfoAddWnd.IsVisible = !PlayerInfoAddWnd.IsVisible;
			if( PlayerInfoAddWnd.IsVisible )
			{
				PlayerInfoAddWnd.Open();
			}
			else
			{
				PlayerInfoAddWnd.Close();			
			}	
		}	
	}	
	
	return;
}


void CPlayerInfoWnd::SortKeepupSkill()
{
	int i = 0;
	int	skill_index = 0;
	WORD tmpKeepUpSkill[RP_MAX_KEEPUP_SKILL] = {0,};
	
	for( i = 0; i < RP_MAX_KEEPUP_SKILL; ++i )
	{
		if( thePcInven->KeepupSkill[i] != 0 )
		{
			WORD aSkillCode = tmpKeepUpSkill[skill_index] = thePcInven->KeepupSkill[i];							
				++skill_index;			 
			thePcInven->KeepupSkill[i] = 0;			
			DeleteKeepUpSkillCode( i, aSkillCode );
		}		
	}
	
	for( i = 0; i < skill_index; ++i )
	{
		PlayerInfoWnd.SetKeepupSkill( tmpKeepUpSkill[i] );
	}
		
	return; 
}

void CPlayerInfoWnd::SetKeepUpSkillCode(int Num, DWORD SkillCode)
{
	int nIndex = Num;
	
	pcitem[nIndex].IType		= n_ITYPE_SKILL;
	pcitem[nIndex].Class		= n_ICLASS_NONE;
	pcitem[nIndex].ItemUID		= 0;
	pcitem[nIndex].Code		= SkillCode;
	
#ifdef SKILLICON_TEST	//중복 스킬 사용시 같은 아이콘의 스킬검색처리

	
	for( int j = 0; j < RP_MAX_KEEPUP_SKILL; ++j )
	{
		if(pcitem[j].Tid == (TEXID)g_Pc_Manager.GetSkillContinualIconTID( SkillCode ) 
			&& ( SkillCode != 1811 ) && ( SkillCode != 1812 ) && ( SkillCode != 1813 )) 
		{
			
			if( pcitem[j].Code > SkillCode)
			{
				DeleteKeepUpDirectSkillCode( SkillCode );
				pcitem[j].Tid = (TEXID)g_Pc_Manager.GetSkillContinualIconTID( SkillCode );
			}
			else
			{
				DeleteKeepUpDirectSkillCode( pcitem[j].Code );
				pcitem[nIndex].Tid	= (TEXID)g_Pc_Manager.GetSkillContinualIconTID( SkillCode );
			}
		}
		else
		{
			pcitem[nIndex].Tid	= (TEXID)g_Pc_Manager.GetSkillContinualIconTID( SkillCode );
		}
	}
	

#else
	
	pcitem[nIndex].Tid			= (TEXID)g_Pc_Manager.GetSkillContinualIconTID( SkillCode );
#endif //SKILLICON_TEST
	
	pcitem[nIndex].TidEx		= (TEXID)g_Pc_Manager.GetSkillAttriIconTID( SkillCode );
	pcitem[nIndex].Amount		= 1;
	pcitem[nIndex].SkillTable	= g_Pc_Manager.GetSkillBaseInfoPtr( SkillCode );
	pcitem[nIndex].Nation		= g_Pc.m_MainPC.char_info.theCitizen;

#ifdef HOLY_SKILLICON_MODIFY
	if( thePcInven->FrontierPetInfo != NULL ) 
	{
		pcitem[nIndex].HolyLevel	= thePcInven->FrontierPetInfo->thePetLevel;
		if( ( SkillCode == 5551 || SkillCode == 5552 ) && pcitem[nIndex].HolyLevel != 0 )
		{
			int Level = pcitem[nIndex].HolyLevel;
			pcitem[nIndex].HolyLevel = Level*0.7f;
		}
	}
#endif

	theKeepupSkillSlot[nIndex]->PcItem = &pcitem[nIndex];
	theKeepupSkillSlot[nIndex]->SetVisible( TRUE );

}

//-----------------------------------------------------------------------------
void CPlayerInfoWnd::SetKeepupSkill( WORD _skillcode )
{
//	SortKeepupSkill();

	//*	Keepup_Skill 지금은 주석, 추후 리소스 작업이 완료되면...처리
	for( int i = 0; i < RP_MAX_KEEPUP_SKILL; ++i )
	{
		if( thePcInven->KeepupSkill[i] == 0 )
		{
			thePcInven->KeepupSkill[i] = _skillcode;			
			SetKeepUpSkillCode( i , _skillcode );
			break;
		}
	}

	return;
}

//-----------------------------------------------------------------------------
void CPlayerInfoWnd::DelKeepupSkill( WORD _skillcode )
{	
	//*	Keepup_Skill 지금은 주석, 추후 리소스 작업이 완료되면...처리
	for( int i = 0; i < RP_MAX_KEEPUP_SKILL; ++i )
	{
		if( thePcInven->KeepupSkill[i] == _skillcode )
		{
			thePcInven->KeepupSkill[i] = 0;		
			DeleteKeepUpSkillCode( i, _skillcode );
			break;
		}
	}

	SortKeepupSkill();

	return;
}

void CPlayerInfoWnd::DeleteKeepUpSkillCode(int Num, WORD aSkillCode)
{
	theKeepupSkillSlot[Num]->PcItem = NULL;
	theKeepupSkillSlot[Num]->SetVisible( FALSE );
}

void CPlayerInfoWnd::DeleteAllKeepUpSkillCode()
{
	for( int i = 0; i < RP_MAX_KEEPUP_SKILL; ++i )
	{
		theKeepupSkillSlot[i]->PcItem = NULL;
		theKeepupSkillSlot[i]->SetVisible( FALSE );
	}
}

void CPlayerInfoWnd::DeleteKeepUpDirectSkillCode( WORD aSkillCode )
{
	for( int i = 0; i < RP_MAX_KEEPUP_SKILL; ++i )
	{
		if( theKeepupSkillSlot[i]->PcItem != NULL )
		{
			if( theKeepupSkillSlot[i]->PcItem->SkillTable != NULL &&
				theKeepupSkillSlot[i]->PcItem->SkillTable->theSkillCode == aSkillCode )
			{
				theKeepupSkillSlot[i]->PcItem = NULL;
				theKeepupSkillSlot[i]->SetVisible( FALSE );
				break;
			}
		}
	}
}

void CPlayerInfoWnd::SetCheckPayTime(int aNum , SPayEffect aPayEffect )
{

	//시간 갱신만한다.. 
/*	
	m_KeepupItem[aNum].IType		= n_ITYPE_SPECIAL;
	m_KeepupItem[aNum].Class		= n_Class_Use;
	m_KeepupItem[aNum].ItemUID		= 0;
	m_KeepupItem[aNum].Code			= aPayEffect.theItemCode;
	m_KeepupItem[aNum].Tid			= (TEXID) g_Pc_Manager.GetItemTID( aPayEffect.theItemCode );
	m_KeepupItem[aNum].TidEx		= (TEXID) g_Pc_Manager.GetItemTID( aPayEffect.theItemCode );
	m_KeepupItem[aNum].Amount		= 1;


	m_KeepupItem[aNum].ItemTable	= g_Pc_Manager.GetItemBaseInfoPtr( aPayEffect.theItemCode );
	
	m_pKeepupItemSlot[aNum]->PcItem = &m_KeepupItem[aNum];
	m_pKeepupItemSlot[aNum]->SetVisible( TRUE );
	m_pKeepupItemText[aNum]->SetVisible( TRUE );
*/

	// 남은 시간 표시 
	SItemBaseInfo* item_Info = NULL;
	item_Info = m_pKeepupItemSlot[aNum]->PcItem->ItemTable;

#ifdef KEEPUP_PAYITEM_RENEWAL
#else
	DWORD AllUseTime = item_Info->theReGage * 1000.0f;							// Sec
	AllUseTime += item_Info->theReMPSpeed * 1000.0f * 60.0f;					// Min
	AllUseTime += item_Info->theReSPSpeed * 1000.0f * 60.0f * 60.0f;			// Hour
	AllUseTime += item_Info->theReHPSpeed * 1000.0f * 60.0f * 60.0f * 24.0f;	// Day 
		
	if( AllUseTime <= c_Max_DrawKeepupItemTime )
	{
		m_pKeepupItemSlot[aNum]->PcItem->IsDelay = true;
		m_pKeepupItemSlot[aNum]->PcItem->DelayTime = AllUseTime / 1000.0f;
		m_pKeepupItemSlot[aNum]->PcItem->ProgressTime = AllUseTime;	
		m_pKeepupItemSlot[aNum]->PcItem->CumulativeTime = AllUseTime - aPayEffect.theTime;	// 사용 
	}
	else
	{
		m_pKeepupItemSlot[aNum]->PcItem->IsDelay = false;
		m_pKeepupItemSlot[aNum]->PcItem->DelayTime = AllUseTime / 1000.0f;			// 원본 사용 시간 저장 
		m_pKeepupItemSlot[aNum]->PcItem->ProgressTime = AllUseTime / 1000.0f;		// 초단위로 가지고 있다가 남은 시간이 1시간 밑으로 남았을때만 보여주기 위해서 * 1000.0f 후 사용한다. 
		m_pKeepupItemSlot[aNum]->PcItem->CumulativeTime = AllUseTime - aPayEffect.theTime;	// 사용 
		m_pKeepupItemSlot[aNum]->PcItem->Amount = ( aPayEffect.theTime / c_Max_DrawKeepupItemTime ) + 1;
	}
#endif


	return;
}

void CPlayerInfoWnd::RemainTimeShow()
{
	int Count = -1 ; 
	int i=0;
	int MaxSecter = 5; 
	int BgCount = 0; 

	for(i = 0 ; i< 6 ; i++)
	{
		PayImgBg[i]->SetVisible(false);
	}

	for(i = 0 ; i< RP_MAX_PAY_CONTENTS ;++i)
	{
		if( thePcInven->PayEffect[i] != 0 )
		{
			if( i > MaxSecter-1 )
			{
				BgCount++;
				MaxSecter+=5;
			}
			//Width+=24;
			Count++;
			PayImgBg[BgCount]->SetClientSize(((Count%5)+1)*24 , 12);
			PayImgBg[BgCount]->SetVisible(true);
		
		}
		else
		{
			break;
		}
	}
	
}


//========================================================================================================
void CPlayerInfoWnd::SetKeepUpItemCode( int aNum, SPayEffect aPayEffect )
{
	m_KeepupItem[aNum].IType		= n_ITYPE_SPECIAL;
	m_KeepupItem[aNum].Class		= n_Class_Use;
	m_KeepupItem[aNum].ItemUID		= 0;
	m_KeepupItem[aNum].Code			= aPayEffect.theItemCode;
	m_KeepupItem[aNum].Tid			= (TEXID) g_Pc_Manager.GetItemTID( aPayEffect.theItemCode );
	m_KeepupItem[aNum].TidEx		= (TEXID) g_Pc_Manager.GetItemTID( aPayEffect.theItemCode );
	m_KeepupItem[aNum].Amount		= 1;


	m_KeepupItem[aNum].ItemTable	= g_Pc_Manager.GetItemBaseInfoPtr( aPayEffect.theItemCode );
	
	m_pKeepupItemSlot[aNum]->PcItem = &m_KeepupItem[aNum];
	m_pKeepupItemSlot[aNum]->SetVisible( TRUE );
	m_pKeepupItemText[aNum]->SetVisible( TRUE );
SItemBaseInfo* item_Info = NULL;
	item_Info = m_pKeepupItemSlot[aNum]->PcItem->ItemTable;


#ifdef KEEPUP_PAYITEM_RENEWAL
	

#else 
	DWORD AllUseTime = item_Info->theReGage * 1000.0f;							// Sec
	AllUseTime += item_Info->theReMPSpeed * 1000.0f * 60.0f;					// Min
	AllUseTime += item_Info->theReSPSpeed * 1000.0f * 60.0f * 60.0f;			// Hour
	AllUseTime += item_Info->theReHPSpeed * 1000.0f * 60.0f * 60.0f * 24.0f;	// Day 

	if( AllUseTime <= c_Max_DrawKeepupItemTime )
	{
		m_pKeepupItemSlot[aNum]->PcItem->IsDelay = true;
		m_pKeepupItemSlot[aNum]->PcItem->DelayTime = AllUseTime / 1000.0f;
		m_pKeepupItemSlot[aNum]->PcItem->ProgressTime = AllUseTime;	
		m_pKeepupItemSlot[aNum]->PcItem->CumulativeTime = SAFE_TIME_SUB( AllUseTime, aPayEffect.theTime );	// 사용 
	}
	else
	{
		m_pKeepupItemSlot[aNum]->PcItem->IsDelay = false;
		m_pKeepupItemSlot[aNum]->PcItem->DelayTime = AllUseTime / 1000.0f;			// 원본 사용 시간 저장 
		m_pKeepupItemSlot[aNum]->PcItem->ProgressTime = AllUseTime / 1000.0f;		// 초단위로 가지고 있다가 남은 시간이 1시간 밑으로 남았을때만 보여주기 위해서 * 1000.0f 후 사용한다. 
		m_pKeepupItemSlot[aNum]->PcItem->CumulativeTime = SAFE_TIME_SUB( AllUseTime, aPayEffect.theTime );	// 사용 
		m_pKeepupItemSlot[aNum]->PcItem->Amount = ( aPayEffect.theTime / c_Max_DrawKeepupItemTime ) + 1;
	}
	
#endif
	

	return;
}

void CPlayerInfoWnd::DeleteKeepUpItemCode( int aNum )
{
	m_pKeepupItemSlot[aNum]->PcItem = NULL;
	m_pKeepupItemSlot[aNum]->SetVisible( FALSE );
	m_pKeepupItemText[aNum]->SetVisible( FALSE );
	
}


void CPlayerInfoWnd::SortKeepupItem()
{
	int i = 0;
	int	idx = 0;
	SPayEffect tmpPayEffect[ RP_MAX_PAY_CONTENTS ];

	
	memset( tmpPayEffect, 0, sizeof( SPayEffect ) * RP_MAX_PAY_CONTENTS );

	for( i = 0; i < RP_MAX_PAY_CONTENTS; ++i )
	{
		if( thePcInven->PayEffect[i] )
		{
			SItemBaseInfo* item_Info = NULL;
		
#ifdef KEEPUP_PAYITEM_RENEWAL
			tmpPayEffect[idx] = *( thePcInven->PayEffect[i] );			
			++idx;
			
			thePcInven->PayEffect[i]->theItemCode = 0;			
			thePcInven->PayEffect[i]->theTime  = 0;			
			
			DeleteKeepUpItemCode( i );
			SAFE_DELETE( thePcInven->PayEffect[i] );
#else
			item_Info = m_pKeepupItemSlot[i]->PcItem->ItemTable;
			
			float AllUseTime = item_Info->theReGage * 1000.0f;							// Sec
			AllUseTime += item_Info->theReMPSpeed * 1000.0f * 60.0f;					// Min
			AllUseTime += item_Info->theReSPSpeed * 1000.0f * 60.0f * 60.0f;			// Hour
			AllUseTime += item_Info->theReHPSpeed * 1000.0f * 60.0f * 60.0f * 24.0f;	// Day 
			
			if( AllUseTime > c_Max_DrawKeepupItemTime && m_pKeepupItemSlot[i]->PcItem->IsDelay == true )
			{
				thePcInven->PayEffect[i]->theTime = AllUseTime - (m_pKeepupItemSlot[i]->PcItem->CumulativeTime + c_Max_DrawKeepupItemTime);
			}
			else
			{
				thePcInven->PayEffect[i]->theTime = AllUseTime - m_pKeepupItemSlot[i]->PcItem->CumulativeTime;
			}
			
			tmpPayEffect[idx] = *( thePcInven->PayEffect[i] );			
			++idx;
			
			thePcInven->PayEffect[i]->theItemCode = 0;			
			thePcInven->PayEffect[i]->theTime  = 0;			
			
			DeleteKeepUpItemCode( i );
			SAFE_DELETE( thePcInven->PayEffect[i] );
#endif
		}
	}
	
	for( i = 0; i < idx; ++i )
	{
		PlayerInfoWnd.SetPayEffect( tmpPayEffect[i] );
	}
		
	return; 
}

//-----------------------------------------------------------------------------
void CPlayerInfoWnd::SetPayEffect( SPayEffect aPayEffect )
{
	for( int i = 0; i < RP_MAX_PAY_CONTENTS; ++i )
	{
		if( thePcInven->PayEffect[i] == NULL )
		{
			thePcInven->PayEffect[i] = SAFE_NEW( SPayEffect );
			thePcInven->PayEffect[i]->theItemCode = aPayEffect.theItemCode;
			thePcInven->PayEffect[i]->theTime  = aPayEffect.theTime;
				
			SetKeepUpItemCode( i , aPayEffect );

			break;
		}
	}

#ifdef KEEPUP_PAYITEM_RENEWAL
	RemainTimeShow();
#endif
	

	return;
}

//-----------------------------------------------------------------------------
void CPlayerInfoWnd::DelPayEffect( SPayEffect aPayEffect )
{		
	for( int i = 0; i < RP_MAX_PAY_CONTENTS; ++i )
	{
		if( thePcInven->PayEffect[i] )
		{
			if( thePcInven->PayEffect[i]->theItemCode == aPayEffect.theItemCode )
			{
				thePcInven->PayEffect[i]->theItemCode = 0;
				thePcInven->PayEffect[i]->theTime  = 0;

				DeleteKeepUpItemCode( i );
				break;
			}
		}
	}

	SortKeepupItem();
	
#ifdef KEEPUP_PAYITEM_RENEWAL
	
	RemainTimeShow();
	
#endif

	return;
}


//-----------------------------------------------------------------------------

void CPlayerInfoWnd::KeepVisible(BOOL IsVisible)
{

	for(int i = 0 ; i < RP_MAX_KEEPUP_SKILL ; ++i)
	{
		theKeepupSkillSlot[i]->SetVisible( IsVisible );
	}

}

void CPlayerInfoWnd::Start2DEffect(int iHP, int iRealHP)
{
	if(iHP == iRealHP)  //미스처리
		return;
	
	m_iDamage = iHP - iRealHP;
		
	if(!m_BStartEffect)
	 m_iPrevHP = iHP;	
	
	m_fCumulTime = 0.0f;

	m_fBarAlpha = 255.0f;
	PrevHP->SetBarAlpha((int)m_fBarAlpha);
	m_BStartEffect = TRUE;

	m_lPrevTime = g_nowTime;
}

void CPlayerInfoWnd::Start2DEffect(int iHP, _STargetInfo aTargetInfo, WORD aSkillCode)
{
	SSkillBaseInfo* skill_Info;
	skill_Info = g_Pc_Manager.GetSkillBaseInfoPtr( aSkillCode );

	if( skill_Info == NULL )
		return;

	int skillcase = 0;
	BYTE effect_type = 0;
	DWORD effect_value = 0;

	for(int i = 0; i < 3; ++i )
	{
		effect_value = aTargetInfo.theEffectValue[i];
		effect_type = skill_Info->theEffectInfo[i].theEffectType;

		if( aTargetInfo.theDieFlag )	// 죽었을 경우엔 현재 남아있는 HP 모두 감소 
		{
			effect_value = iHP;
		}
		
		skillcase = effect_type / 30;
		if( skillcase == 1 )
		{
			effect_type -= 30;
		}
		else if( skillcase == 2 )
		{
			effect_type -= 60;
		}	
		
		switch(effect_type)
		{
			case n_HPAdd:
//			case n_AttackPowerAdd:
//			case n_MagicPowerAdd:
			case n_AttackBlowAdd:
			case n_MagicBlowAdd:
				 Start2DEffect( iHP, iHP - effect_value );	
				 break;
		}	
	}
}

void CPlayerInfoWnd::LogOff()
{
	pComboStepBar->InitData();
	pComEffimg[0]->InitData();
	pComEffimg[1]->InitData();
	pComEffimg[2]->InitData();
	pComEffimg[3]->InitData();
	pComEffimg[4]->InitData();
	pComEffimg[5]->InitData();
	int i=0;

	for( i = 0; i < RP_MAX_PAY_CONTENTS; ++i )
	{
		if( m_pKeepupItemSlot[i]->IsVisible() == false )
		{
			break;
		}
		m_pKeepupItemText[i]->SetVisible( FALSE );
		m_pKeepupItemSlot[i]->SetVisible( FALSE );
	}


	FrameWndFameInDunTime->SetVisible(false);
#ifdef KEEPUP_PAYITEM_RENEWAL
	for(i =0; i < 6 ; ++i)
		PayImgBg[i]->SetVisible(false);
#endif
	
	return;
}
void CPlayerInfoWnd::InitFameInDun()
{
	FrameWndFameInDunTime->SetVisible(false);
	FameInDunTimeText->SetTextColor( 255 , 255 , 255 );
	memset(&thePcInven->InDunData , 0 , sizeof(SInDunData));


}

float tempTime = 0 ; 


void CPlayerInfoWnd::ShowFameInDun()
{

	//명성치 인던에 시간을 준다.
	//맵 49,50번맵에만 카운트 표시 ... 

	if(!thePcInven->InDunData.iStartCount)
	{
		FrameWndFameInDunTime->SetVisible(false);
	//	FameInDunTimeText->SetTextColor( 255 , 255 , 255 );
		
		return;
	}

	if( (g_Map.m_NowMapID == c_RockMap_Valley_Ruddy49 ||
		g_Map.m_NowMapID == c_RockMap_Valley_Ruddy50 ||
		g_Map.m_NowMapID == c_RockMap_Armis_Temple53  )&& 
		thePcInven->InDunData.iStartCount )
	{	

		FrameWndFameInDunTime->SetVisible(true);
		
		int CurTime	 = SAFE_TIME_SUB( g_nowTime , thePcInven->InDunData.dwStartTime );
		
		if(thePcInven->InDunData.iStartCount < 0 )
		{
			thePcInven->InDunData.iStartCount = 0 ; 
		}
		
		int Cnt =   thePcInven->InDunData.iStartCount - CurTime;
			
		if( Cnt <= 0 )
		{
			thePcInven->InDunData.iStartCount  = 0 ; 
			Cnt = 0 ; 
			return ; 
		}

		int Min = (Cnt/1000) / 60 ;
		int Sec = (Cnt/1000) % 60 ; 
		
		char tmpStr[128] = {0,};

		if(Min)
		{	
			Rsprintf( RWCHAR(tmpStr), _RT("%d "), Min); 
			Rstrcat(RWCHAR(tmpStr),G_STRING_CHAR(IDS_WND_CLOCK_MIN));
		}
		else
		{
			Rsprintf( RWCHAR(tmpStr), _RT("%d "),Sec );
			Rstrcat(RWCHAR(tmpStr),G_STRING_CHAR(IDS_WND_CLOCK_SEC));
		}
		
		FameInDunTimeText->SetText(tmpStr);
		
	}
	else
	{
		InitFameInDun();
		FrameWndFameInDunTime->SetVisible(false);
	}
	
}
