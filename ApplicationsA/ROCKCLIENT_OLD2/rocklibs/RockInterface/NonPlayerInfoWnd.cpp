// NonPlayerInfoWnd.cpp: implementation of the CNonPlayerInfoWnd class.
//
//////////////////////////////////////////////////////////////////////
#include "..\\RockPCH.h"

#include "Define.h"
#include "Util.h"
#include "Render.h"
#include "Fontman.h"
#include "Resource.h"
#include "Chain.h"
#include "Container.h"
#include "Rui.h"

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
#include "Spin.h"
#include "StateButton.h"
#include "ChatOutput.h"
#include "Slot.h"

#include "NonPlayerInfoWnd.h"
#include "NonPlayerInfoAddWnd.h"
#include "PartyInforWnd.h"

#include "..\\..\\quadlist.h"
#include "..\\..\\bravolibs\\network\\Protocol.h"
#include "..\\..\\Pc.h"

#include "..\\..\\Map.h"

#include "..\\..\\RockClient.h"
#include "..\\..\\PC.h"
#include "..\\..\\Camera.h"
#include "..\\..\\Map.h"
#include "..\\..\\bravolibs\\obj\\Character_Item.H"
#include "..\\..\\bravolibs\\obj\\Player_Manager.H"
#include "..\\..\\bravolibs\\obj\\Line_Vertex.H"
#include "..\\..\\Effect.h"

#include "BattleZoneWndProc.h"
#include "stringmanager.h"
#include "defineMap_ID.h"



extern long			g_nowTime;

CNonPlayerInfoWnd NonPlayerInfoWnd;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNonPlayerInfoWnd::CNonPlayerInfoWnd()
{
	thePickedNpcID = -1;
	thePickedUserID = -1;
	
	theFrame	  = NULL;
	theGlowImg	  = NULL;
	
	theLevel	  = NULL;
	theName	      = NULL;	
	thePrevHP	  = NULL;
	theHP		  = NULL;
	theGlowHPImg	  = NULL;
	theSMP		  = NULL;
	
	theHPText		=NULL;
	theSMPText		=NULL;


	m_ISDisplay   = FALSE;
	m_pSelectSrc  = NULL;
	m_pSrc        = NULL;

	m_pAddButton = NULL;	

	m_BStartEffect = FALSE;
	m_lPrevTime = g_nowTime;
	
	m_iPrevHP	  = 0;
	m_iDamage	  = 0;

	m_fGlowTime	  = 0.05f;    //점멸 타임
	m_fFadeTime	  = 1.0f; 	 //페이딩 타임
	m_fBarAlpha   = 0.0f;	 //이전 HP알파값	 
	m_fCumulTime  = 0.0f;	 //페이딩 누적 타임

	for( int i = 0 ; i < RP_MAX_KEEPUP_SKILL ; ++i )
	{
		theKeepupSkillField[ i ].SkillCode = 0;
		theKeepupSkillField[ i ].KeepupSkillPos = -1;		 
	}
}

CNonPlayerInfoWnd::~CNonPlayerInfoWnd()
{
	
}

//-----------------------------------------------------------------------------
void CNonPlayerInfoWnd::Init()
{
}

//-----------------------------------------------------------------------------
void CNonPlayerInfoWnd::Composition()
{

	CompositionNew();	

	return;
}

void CNonPlayerInfoWnd::CompositionOld()
{

}

void CNonPlayerInfoWnd::CompositionNew()
{
	//CButton*		btn;		

	int center_x	= nRui->Desktop.Width / 2;
//	int center_y	= nRui->Desktop.Height / 2;
	
	//-------------------------------------------------------------------------
	//
	// 다른 유저 및 NPC 정보창 
	//
	//-------------------------------------------------------------------------
	SRect infownd;	
	infownd.x	= center_x - ( 193 / 2 );
//	infownd.y	= center_y - ( infownd.h * 2 );
	infownd.y	= 7; //2;
	infownd.w	= 193; 
	infownd.h	= 48;
	
	theFrame = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_NonPlayerInfo_Wnd, WID_None, 
		infownd.x, infownd.y, infownd.w, infownd.h, false );
	theFrame->SetClientImage( TID_MAIN_PC_INFO );
	theFrame->SetCaption( _RT("NonPlayerInfo") );
	theFrame->SetWndProc( this );
	theFrame->SetTitle( false );
	theFrame->SetMove( false );
	theFrame->SetVisible( false );
	
	
	theFrame->SetZorder( n_zoTop );
	///--theFrame->SetRegion( true );
	theFrame->Initialize();
	m_pOldPos.x = infownd.x;
	m_pOldPos.y = infownd.y;
	
	theGlowImg = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_NonPlayerInfo_GlowWnd, 
					WID_NonPlayerInfo_Wnd, 0, 0, infownd.w, infownd.h, false);
	theGlowImg->SetClientImage(TID_NonPlayerInfo_GlowWnd);
	theGlowImg->SetGlow(TRUE);
	theGlowImg->SetVisible(false);
	theGlowImg->SetZorder( n_zoBottom );
	
	theLevel = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_NonPlayerInfo_LevelText, 
					WID_NonPlayerInfo_Wnd, 7, 7, 17, 13, false );
	theLevel->SetFontR( n_fRsvFontWndStatic2 );
	theLevel->SetClientColor( 240, 240, 200 );
	theLevel->SetBlank( 0, 0 );
	
	theName = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_NonPlayerInfo_NameText, 
					WID_NonPlayerInfo_Wnd, 36, 7, 144, 13, false );
	theName->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	theName->SetClientColor( 180, 175, 160 );
	theName->SetClientImage( TID_None );
	//theName->SetFontDecoration( dfOutlineFont );
	theName->SetAlignText( n_atLeft, n_atCenter );
	//theName->SetText( _RT("나이스가이 ^ ^V") );		// 나중에 정보를 받아서 처리 
	theName->SetTextColor( 0, 0, 0 );
	theName->SetBlank( 0, 0 );
	
	thePrevHP = (CProgressBar*)nRui->CreateWnd( n_wtProgressBar, WID_NonPlayerInfo_PrevHPGauge, 
					WID_NonPlayerInfo_Wnd, 1, 30, 166, 7, false );
	thePrevHP->SetClientImage( TID_None );
	thePrevHP->SetBarImage( TID_NonPCInfo_PrevHP_Gauge );
	thePrevHP->SetBarBlank( 0, 0 );
	thePrevHP->InitProgress( 50 );
	thePrevHP->SetZorder( n_zoBottom );	

	theHP = (CProgressBar*)nRui->CreateWnd( n_wtProgressBar, WID_NonPlayerInfo_HPGauge, 
					WID_NonPlayerInfo_Wnd, 1, 30, 166, 7, false );
	theHP->SetClientImage( TID_None );
	theHP->SetBarImage( TID_MAIN_PC_GAUGE_HP );
	theHP->SetBarBlank( 0, 0 );
	theHP->InitProgress( 50 );

	theGlowHPImg = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_NonPlayerInfo_GlowHPGauge, 
					WID_NonPlayerInfo_Wnd, 1, 30, 166, 7, false );
	theGlowHPImg->SetClientImage( TID_NonPCInfo_GlowHP_Gauge );
	theGlowHPImg->SetGlow(TRUE);
	theGlowHPImg->SetZorder( n_zoTop );
	theGlowHPImg->SetVisible(false);
	
	theSMP = (CProgressBar*)nRui->CreateWnd( n_wtProgressBar, WID_NonPlayerInfo_SPMPGauge, 
					WID_NonPlayerInfo_Wnd, 1, 40, 166, 7, false );
	theSMP->SetClientImage( TID_None );
	theSMP->SetBarImage( TID_MAIN_PC_GAUGE_MP );
	theSMP->SetBarBlank( 0, 0 );
	theSMP->InitProgress( 50 );



	theHPText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_NonPlayerInfo_Wnd, 
		1, 30, 166, 5, false );  
	theHPText->SetFontg( n_ftGulimChe, 5, n_fwNormal, n_fdNormal );
	theHPText->SetImageNum( true );
	theHPText->SetClientImage( TID_None );
	theHPText->SetClientColor( 180, 175, 160 );
	theHPText->SetAlignText( n_atRight, n_atCenter );
	theHPText->SetText( _RT("1000/1000") );
	theHPText->SetTextColor( 255, 220, 220 );
	theHPText->SetHID( HD_S_HP );
	
	
	//	SMPText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_PlayerInfo_SMPText, WID_PlayerInfo_Wnd, 110, 57, 100, 12, false );
	theSMPText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_NonPlayerInfo_Wnd, 
		1, 40, 166, 5, false );   
	theSMPText->SetFontg( n_ftGulimChe, 5, n_fwNormal, n_fdNormal );
	theSMPText->SetImageNum( true );
	theSMPText->SetClientImage( TID_None );
	theSMPText->SetClientColor( 180, 175, 160 );
	theSMPText->SetAlignText( n_atRight, n_atCenter );
	theSMPText->SetText( _RT("100/100") );
	theSMPText->SetTextColor( 220, 220, 255 );
	theSMPText->SetHID( HD_S_MP );

	m_pAddButton = (CButton*)nRui->CreateWnd( n_wtButton, WID_NonPlayerInfo_AddBtn, WID_NonPlayerInfo_Wnd, 
		  171, 28, 22, 21, false );
	m_pAddButton->SetBtnImage( TID_MAIN_MENUBT, TID_MAIN_MENUBT_OVR, TID_MAIN_MENUBT_CLK, TID_None );
	m_pAddButton->SetAlignText( n_atCenter, n_atCenter );	
	m_pAddButton->SetTextColor(0, 0, 0);
	m_pAddButton->SetHID( HD_MENU ); 

/*
	theKeepupSkillImg[0] = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_NonPlayerInfo_KeepupSkillImg0, WID_NonPlayerInfo_Wnd, 392, 112, 24, 24, false );
	theKeepupSkillImg[0]->SetClientImage( TID_None );
	theKeepupSkillImg[1] = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_NonPlayerInfo_KeepupSkillImg1, WID_NonPlayerInfo_Wnd, 416, 112, 24, 24, false );
	theKeepupSkillImg[1]->SetClientImage( TID_None );
	theKeepupSkillImg[2] = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_NonPlayerInfo_KeepupSkillImg2, WID_NonPlayerInfo_Wnd, 440, 112, 24, 24, false );
	theKeepupSkillImg[2]->SetClientImage( TID_None );
	theKeepupSkillImg[3] = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_NonPlayerInfo_KeepupSkillImg3, WID_NonPlayerInfo_Wnd, 464, 112, 24, 24, false );
	theKeepupSkillImg[3]->SetClientImage( TID_None );
	theKeepupSkillImg[4] = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_NonPlayerInfo_KeepupSkillImg4, WID_NonPlayerInfo_Wnd, 488, 112, 24, 24, false );
	theKeepupSkillImg[4]->SetClientImage( TID_None );
	theKeepupSkillImg[5] = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_NonPlayerInfo_KeepupSkillImg5, WID_NonPlayerInfo_Wnd, 512, 112, 24, 24, false );
	theKeepupSkillImg[5]->SetClientImage( TID_None );
	theKeepupSkillImg[6] = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_NonPlayerInfo_KeepupSkillImg6, WID_NonPlayerInfo_Wnd, 536, 112, 24, 24, false );
	theKeepupSkillImg[6]->SetClientImage( TID_None );
	theKeepupSkillImg[7] = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_NonPlayerInfo_KeepupSkillImg7, WID_NonPlayerInfo_Wnd, 560, 112, 24, 24, false );
	theKeepupSkillImg[7]->SetClientImage( TID_None );
	theKeepupSkillImg[8] = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_NonPlayerInfo_KeepupSkillImg8, WID_NonPlayerInfo_Wnd, 584, 112, 24, 24, false );
	theKeepupSkillImg[8]->SetClientImage( TID_None );
	theKeepupSkillImg[9] = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_NonPlayerInfo_KeepupSkillImg9, WID_NonPlayerInfo_Wnd, 608, 112, 24, 24, false );
	theKeepupSkillImg[9]->SetClientImage( TID_None );
*/
//	theKeepupSkillSlot[0] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_NonPlayerInfo_KeepupSkillImg0, WID_None, 253, 94, 24, 24, false );
	
#ifdef COMBO_UPGRADE
	int i = 0 , layer = 0 ;    
	int sPosX = infownd.x ;		
	int sPosY = 62 ;
	int Xindex = 0 ; 
	int Yindex = 0 ;

	// Y 증가는 size 25
	
	for(i = 0; i < RP_MAX_KEEPUP_SKILL; ++i )
	{
	
		theKeepupSkillSlot[i] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, 
			WID_None, sPosX+(Xindex*24), sPosY+(Yindex*25), 24, 24, false );
		theKeepupSkillSlot[i]->SetSlotGroup( n_SGROUP_KEEPUPSKILL );
		//	theKeepupSkillSlot[j]->SetHightlightImg( TID_None, TID_None );
		theKeepupSkillSlot[i]->SetHID( HID_ITEMTIP );
		theKeepupSkillSlot[i]->SetVisible( FALSE );
		theKeepupSkillSlot[i]->SetScaleTrans(true);
		
		++Xindex;

		if( i == 9 )
		{
			Xindex = 0 ;
			++Yindex;
		}
	}

#else	
	
	theKeepupSkillSlot[0] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_NonPlayerInfo_KeepupSkillImg0, WID_None, 
		infownd.x, 62, 24, 24, false );
	theKeepupSkillSlot[0]->SetSlotGroup( n_SGROUP_KEEPUPSKILL );
//	theKeepupSkillSlot[0]->SetHightlightImg( TID_None, TID_None );
	theKeepupSkillSlot[0]->SetHID( HID_ITEMTIP );
	theKeepupSkillSlot[0]->SetScaleTrans(true);

	theKeepupSkillSlot[1] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_NonPlayerInfo_KeepupSkillImg1, WID_None, 
		infownd.x + 24, 62, 24, 24, false );
	theKeepupSkillSlot[1]->SetSlotGroup( n_SGROUP_KEEPUPSKILL );
//	theKeepupSkillSlot[1]->SetHightlightImg( TID_None, TID_None );
	theKeepupSkillSlot[1]->SetHID( HID_ITEMTIP );
	theKeepupSkillSlot[1]->SetScaleTrans(true);

	theKeepupSkillSlot[2] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_NonPlayerInfo_KeepupSkillImg2, WID_None, 
		infownd.x + 48, 62, 24, 24, false );
	theKeepupSkillSlot[2]->SetSlotGroup( n_SGROUP_KEEPUPSKILL );
//	theKeepupSkillSlot[2]->SetHightlightImg( TID_None, TID_None );
	theKeepupSkillSlot[2]->SetHID( HID_ITEMTIP );
	theKeepupSkillSlot[2]->SetScaleTrans(true);

	theKeepupSkillSlot[3] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_NonPlayerInfo_KeepupSkillImg3, WID_None, 
		infownd.x + 72, 62, 24, 24, false );
	theKeepupSkillSlot[3]->SetSlotGroup( n_SGROUP_KEEPUPSKILL );
//	theKeepupSkillSlot[3]->SetHightlightImg( TID_None, TID_None );
	theKeepupSkillSlot[3]->SetHID( HID_ITEMTIP );
	theKeepupSkillSlot[3]->SetScaleTrans(true);

	theKeepupSkillSlot[4] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_NonPlayerInfo_KeepupSkillImg4, WID_None, 
		infownd.x + 96, 62, 24, 24, false );
	theKeepupSkillSlot[4]->SetSlotGroup( n_SGROUP_KEEPUPSKILL );
//	theKeepupSkillSlot[4]->SetHightlightImg( TID_None, TID_None );
	theKeepupSkillSlot[4]->SetHID( HID_ITEMTIP );
	theKeepupSkillSlot[4]->SetScaleTrans(true);

	theKeepupSkillSlot[5] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_NonPlayerInfo_KeepupSkillImg5, WID_None, 
		infownd.x, 87, 24, 24, false );
	theKeepupSkillSlot[5]->SetSlotGroup( n_SGROUP_KEEPUPSKILL );
//	theKeepupSkillSlot[5]->SetHightlightImg( TID_None, TID_None );
	theKeepupSkillSlot[5]->SetHID( HID_ITEMTIP );
	theKeepupSkillSlot[5]->SetScaleTrans(true);

	theKeepupSkillSlot[6] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_NonPlayerInfo_KeepupSkillImg6, WID_None, 
		infownd.x + 24, 87, 24, 24, false );
	theKeepupSkillSlot[6]->SetSlotGroup( n_SGROUP_KEEPUPSKILL );
//	theKeepupSkillSlot[6]->SetHightlightImg( TID_None, TID_None );
	theKeepupSkillSlot[6]->SetHID( HID_ITEMTIP );
	theKeepupSkillSlot[6]->SetScaleTrans(true);

	theKeepupSkillSlot[7] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_NonPlayerInfo_KeepupSkillImg7, WID_None, 
		infownd.x + 48, 87, 24, 24, false );
	theKeepupSkillSlot[7]->SetSlotGroup( n_SGROUP_KEEPUPSKILL );
//	theKeepupSkillSlot[7]->SetHightlightImg( TID_None, TID_None );
	theKeepupSkillSlot[7]->SetHID( HID_ITEMTIP );
	theKeepupSkillSlot[7]->SetScaleTrans(true);

	theKeepupSkillSlot[8] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_NonPlayerInfo_KeepupSkillImg8, WID_None, 
		infownd.x + 72, 87, 24, 24, false );
	theKeepupSkillSlot[8]->SetSlotGroup( n_SGROUP_KEEPUPSKILL );
//	theKeepupSkillSlot[8]->SetHightlightImg( TID_None, TID_None );
	theKeepupSkillSlot[8]->SetHID( HID_ITEMTIP );
	theKeepupSkillSlot[8]->SetScaleTrans(true);

	theKeepupSkillSlot[9] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_NonPlayerInfo_KeepupSkillImg9, WID_None, 
		infownd.x + 96, 87, 24, 24, false );
	theKeepupSkillSlot[9]->SetSlotGroup( n_SGROUP_KEEPUPSKILL );
//	theKeepupSkillSlot[9]->SetHightlightImg( TID_None, TID_None );
	theKeepupSkillSlot[9]->SetHID( HID_ITEMTIP );
	theKeepupSkillSlot[9]->SetScaleTrans(true);

#endif

	DeleteAllKeepUpSkill();

}

void CNonPlayerInfoWnd::DeleteAllKeepUpSkill(void)
{
	for( int i = 0 ; i < RP_MAX_KEEPUP_SKILL ; ++i )
	{
		theKeepupSkillField[ i ].SkillCode = 0;
		theKeepupSkillField[ i ].KeepupSkillPos = -1;
		
		theKeepupSkillSlot[ i ]->SetVisible( FALSE );
		theKeepupSkillSlot[ i ]->PcItem = NULL;		 
	}
}

void CNonPlayerInfoWnd::SetKeepUpSkillCode( int nNum, DWORD SkillCode, int HolyLevel )
{
	int Num = 0;

	BOOL IsFind = FALSE;

	for( int i = 0; i < RP_MAX_KEEPUP_SKILL; ++i )
	{
		if( !IsFind )
		{
			if( theKeepupSkillField[i].SkillCode == SkillCode )
			{
				theKeepupSkillField[i].KeepupSkillPos = nNum;			
				Num = i;
				IsFind = TRUE;		
			}
		}
		else
		{
			if( theKeepupSkillField[i].SkillCode == SkillCode )
			{
				theKeepupSkillField[i].SkillCode = 0;
				theKeepupSkillField[i].KeepupSkillPos = -1;					
			}
		}
	}

	if( FALSE == IsFind )
	{
		for( int i = 0; i < RP_MAX_KEEPUP_SKILL; ++i )
		{
			if( theKeepupSkillField[i].KeepupSkillPos == -1 )
			{
				theKeepupSkillField[i].KeepupSkillPos = nNum;
				theKeepupSkillField[i].SkillCode = SkillCode;
				Num = i;				
				break;
			}	
		}
	}
	
	pcitem[Num].IType		= n_ITYPE_SKILL;
	pcitem[Num].Class		= n_ICLASS_NONE;
	pcitem[Num].ItemUID		= 0;
	pcitem[Num].Code		= SkillCode;
	
#ifdef SKILLICON_TEST	//중복 스킬 사용시 같은 아이콘의 스킬검색처리
	
	for( int j = 0; j < RP_MAX_KEEPUP_SKILL; ++j )
	{
		if(pcitem[j].Tid == (TEXID)g_Pc_Manager.GetSkillContinualIconTID( SkillCode ))
		{
			if( theKeepupSkillField[j].SkillCode > SkillCode)
			{
				DeleteKeepUpSkillCode( Num );   // 다음스킬의 스킬코드가 작으면 그대로 둔다  
				pcitem[j].Tid = (TEXID)g_Pc_Manager.GetSkillContinualIconTID( SkillCode );
				
			}
			if( theKeepupSkillField[j].SkillCode < SkillCode)
			{
				DeleteKeepUpSkillCode( Num );   // 다음스킬의 스킬코드가 크면 바꿔준다
				pcitem[Num].Tid	= (TEXID)g_Pc_Manager.GetSkillContinualIconTID( SkillCode );
			}
		}
		else
		{
			pcitem[Num].Tid	= (TEXID)g_Pc_Manager.GetSkillContinualIconTID( SkillCode );
		}
	}
#else
	pcitem[Num].Tid	= (TEXID)g_Pc_Manager.GetSkillContinualIconTID( SkillCode );
#endif //SKILLICON_TEST
	pcitem[Num].TidEx		= (TEXID)g_Pc_Manager.GetSkillAttriIconTID( SkillCode );
	pcitem[Num].Amount		= 1;
	pcitem[Num].SkillTable	= g_Pc_Manager.GetSkillBaseInfoPtr( SkillCode );
	
	
//신수 스킬 이 들어가면 이곳을 통합한다 .. 이것은 현재 상대국가 정보를 알아야한다 . 
	if(thePickedUserID == MAX_USER_INTABLE)
	{
		pcitem[Num].Nation = g_Pc.m_MainPC.char_info.theCitizen;
	}
	else if(thePickedNpcID != -1 )
	{
		pcitem[Num].Nation = 2;
	}
	else if(thePickedUserID != -1 && thePickedUserID != MAX_USER_INTABLE )
	{
		pcitem[Num].Nation = g_Pc.m_PcTable[thePickedUserID].char_info.theCitizen;
	}

#ifdef HOLY_SKILLICON_MODIFY

	if( g_RockClient.IsHolySkill(SkillCode) )
	{
		if(thePickedUserID == MAX_USER_INTABLE)
		{
			if(nRui->GetPcInvenInfo()->FrontierPetInfo)
			{
				pcitem[Num].HolyLevel = nRui->GetPcInvenInfo()->FrontierPetInfo->thePetLevel;	
			}
		}
		
		if(thePickedNpcID != -1 )
		{
			pcitem[Num].HolyLevel = g_Map.m_NpcTable[thePickedNpcID].theHolyLevel;
		}

		if(thePickedUserID != -1 && thePickedUserID != MAX_USER_INTABLE )
		{
			pcitem[Num].HolyLevel = g_Pc.m_PcTable[thePickedUserID].char_info.theHolyLevel;
		}
		
		if( ( SkillCode == 5551 || SkillCode == 5552 ) && pcitem[Num].HolyLevel != 0 )
		{
			int Level = pcitem[Num].HolyLevel; 
			pcitem[Num].HolyLevel = Level*0.7f;
		}
	}

#endif

	theKeepupSkillSlot[Num]->PcItem = &pcitem[Num];
	theKeepupSkillSlot[Num]->SetVisible( TRUE );
}

void CNonPlayerInfoWnd::DeleteKeepUpSkillCode(int nNum)
{
	int Num = 0;	
	
	BOOL IsFind = FALSE;
	
	for( int i = 0; i < RP_MAX_KEEPUP_SKILL; ++i )
	{
		if( theKeepupSkillField[i].KeepupSkillPos == nNum )
		{
			theKeepupSkillField[i].KeepupSkillPos = -1;
			theKeepupSkillField[i].SkillCode = 0;
			Num = i;
			IsFind = TRUE;			
		}	
	}
	
	if( FALSE == IsFind )
	{
		return;
	}
	
	theKeepupSkillSlot[Num]->PcItem = NULL;
	theKeepupSkillSlot[Num]->SetVisible( FALSE );
}

BOOL CNonPlayerInfoWnd::IsItemKeepSkill(DWORD SkillCode)
{
	///-- 아이템을 장착한 후에 관련스킬은 내것이 아닐경우 볼수 없다.
	///-- 마비 저항, 디스펠 저항
	if( 8501 <= SkillCode && SkillCode <= 8540 )
	{
		return TRUE;
	}

	if( 8601 <= SkillCode && SkillCode <= 8640 )
	{
		return TRUE;		
	}
	
	return FALSE;
}

BOOL CNonPlayerInfoWnd::IsNotViewKeepSkill(DWORD SkillCode)
{
	if( SkillCode == 5541 )
	{
		return TRUE;
	}

	if( SkillCode == 5543 )
	{
		return TRUE;
	}

	if( SkillCode == 5545 )
	{
		return TRUE;
	}

	if( SkillCode == 5547 )
	{
		return TRUE;
	}

	if( SkillCode == 5549 )
	{
		return TRUE;
	}

	if( SkillCode == 5551 )
	{
		return TRUE;
	}
	
	return FALSE;
}

//-----------------------------------------------------------------------------
void CNonPlayerInfoWnd::Update()
{
//	nRui->SendEvent( WID_NonPlayerInfo_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );				
//	return;

	static TCHAR tmpStr[128] = {0,};
	WORD i = 0;

	int percent = 0;
    long lElapseTime = SAFE_TIME_SUB( g_nowTime , m_lPrevTime );
	float fElapsetime = (lElapseTime * 0.001);	

	float fTime = 1.0f;

	m_pAddButton->SetVisible(TRUE);

	//연출효과가 있을경우
	if( m_BStartEffect )
	{
        theName->SetTextColor( 255, 58, 27);
		
		m_fCumulTime += fElapsetime;
		
		if(m_fCumulTime < m_fGlowTime)
		{
		    theGlowImg->SetVisible(true);
			theGlowHPImg->SetVisible(true);
			
			int PosX, PosY;
			PosX = RandomNum(-3, 3);
			PosY = RandomNum(-3, 3);

			theFrame->SetAbsolutePosition(m_pOldPos.x + PosX, m_pOldPos.y + PosY);	
		}
		else
		{
			theGlowImg->SetVisible(false);
			theGlowHPImg->SetVisible(false);

			theFrame->SetAbsolutePosition(m_pOldPos.x, m_pOldPos.y);
		}
		
		m_fBarAlpha -= ( (fElapsetime / m_fFadeTime ) * 255 );
		if( m_fBarAlpha <= 0 )
			m_fBarAlpha = 0;
		thePrevHP->SetBarAlpha((int)m_fBarAlpha);
		
		m_iPrevHP -= ( (fElapsetime / fTime ) );		

		if( m_fCumulTime >= m_fFadeTime )
		{
		    m_fCumulTime = 0;
			m_BStartEffect = FALSE;
		}
	}
	else//연출효과가 없을경우
	{
		//연출효과가 없을시 m_iPrevHP를 현재 HP값과 동일화시킨다.		
		if(thePickedUserID == MAX_USER_INTABLE)
		{
			m_iPrevHP = (float)nRui->GetPcParamInfo()->Stat1.theHP;
		}
		
		if(thePickedNpcID != -1 )
		{
			m_iPrevHP = (float)g_Map.m_NpcTable[thePickedNpcID].theHP;
		}

		if(thePickedUserID != -1 && thePickedUserID != MAX_USER_INTABLE )
		{
			m_iPrevHP = (float)g_Pc.m_PcTable[thePickedUserID].char_info.theHP;
		}	

		theName->SetTextColor( 0, 0, 0 );
	}
	
	theFrame->SetClientImage( TID_MAIN_PC_INFO );

	int DrawHP = 0;

	//몹 픽킹 
	if( thePickedNpcID != -1 )		
	{
	
		if(g_RockClient.m_EtcInfo.DevMode == 1)
		{
			theHPText->SetVisible(true);
		}
		else
		{
			theHPText->SetVisible(false);
			theSMPText->SetVisible(false);
		}

		DrawHP = g_Map.m_NpcTable[thePickedNpcID].theHP;

		if( DrawHP > g_Map.m_NpcTable[thePickedNpcID].theMaxHP )
		{
			DrawHP = g_Map.m_NpcTable[thePickedNpcID].theMaxHP;
		}

		if( g_Map.m_Npc[thePickedNpcID].m_curt_event.type == SM_STATE_DIE )
			DrawHP = 0;
		
		if( g_Map.m_NpcTable[ thePickedNpcID ].lUnique == -1 )
		{
			thePickedNpcID = -1;			
			nRui->SendEvent( WID_NonPlayerInfo_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
			nRui->SendEvent( WID_NonPlayerInfoAddWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
		}
		
		SNPCBaseInfo* npc_info = NULL;		
		npc_info = g_Pc_Manager.GetNPCBaseInfoPtr( g_Map.m_NpcTable[thePickedNpcID].code ); 

		if( npc_info && npc_info->theGender == 3 )	
		{
			theFrame->SetClientImage( TID_MAIN_PC_INFO_BOSS );
		}		

		if( npc_info && npc_info->theRace == NPC_RACE_MYSTERY )
		{			
			theLevel->SetText( _RT("???") );
		}
		else
		{
	
			if( g_Map.m_NowMapID == c_RockMap_Valley_Ruddy49 || 
				g_Map.m_NowMapID == c_RockMap_Valley_Ruddy50 ) 
			{			
				if( g_Map.m_Npc[thePickedNpcID].m_State.nPetState == nPetState_Pet )
				{
					Rsprintf( RWCHAR(tmpStr), _RT("%d"), g_Map.m_NpcTable[thePickedNpcID].theLevel );
					theLevel->SetText( tmpStr );
				}
				else
				{
					theLevel->SetText((CHAR*)_RT("???"));
				}				
			}
			else 
			{
				Rsprintf( RWCHAR(tmpStr), _RT("%d"), g_Map.m_NpcTable[thePickedNpcID].theLevel );
				theLevel->SetText( tmpStr );
			}			
		}	

		Rsprintf( RWCHAR(tmpStr), _RT("%s"), RWCHAR(g_Map.m_NpcTable[thePickedNpcID].szName) );
		theName->SetText( tmpStr );
		
		if(m_iPrevHP < (float)DrawHP)
			m_iPrevHP = (float)DrawHP;	

		percent = ((float)m_iPrevHP / (float)g_Map.m_NpcTable[thePickedNpcID].theMaxHP) * 100;
		thePrevHP->SetPercent(percent);			

		if(g_RockClient.m_EtcInfo.DevMode == 1)
		{
			Rsprintf( RWCHAR(tmpStr), _RT("%d/%d"), DrawHP , g_Map.m_NpcTable[thePickedNpcID].theMaxHP );
			theHPText->SetText(tmpStr);
		}	

		if(m_BStartEffect)
		{
			int a = 0;
		}
		
		percent = ((float)DrawHP / (float)g_Map.m_NpcTable[thePickedNpcID].theMaxHP) * 100;
		theHP->SetPercent( percent );

//		theSMP->SetPercent( ((float)g_Map.m_NpcTable[thePickedNpcID].theMP / (float)g_Map.m_NpcTable[thePickedNpcID].theMaxMP) * 100  );
		if( g_Map.m_Npc[thePickedNpcID].m_State.nHolyState == nHolyState_Holy  )
			theSMP->SetPercent( 0 );		
		else
			theSMP->SetPercent( 100 );		
		

		if(g_Map.m_Npc[thePickedNpcID].m_State.nPetState == nPetState_Pet )
		{
			theSMP->SetPercent( ((float)g_Map.m_NpcTable[thePickedNpcID].theSMP / (float)npc_info->theMaxSP) * 100  );	
		}

//		RLG3( "Percent = %d, HP = %d, MaxHP = %d", percent, DrawHP, g_Map.m_NpcTable[thePickedNpcID].theMaxHP );

		//*	Keepup_Skill 지금은 주석, 추후 리소스 작업이 완료되면...처리
		for( i = 0; i < RP_MAX_KEEPUP_SKILL; ++i )
		{
			DWORD SkillCode = g_Map.m_NpcTable[thePickedNpcID].theKeepupSkill[i];
			///-- 아이템을 장착한 후에 관련스킬은 내것이 아닐경우 볼수 없다.
			///-- 마비 저항, 디스펠 저항
			BOOL IsItemSkill = IsItemKeepSkill( SkillCode );
			BOOL IsNotViewSkill = IsNotViewKeepSkill( SkillCode );
			
			if( ( g_Map.m_NpcTable[thePickedNpcID].theKeepupSkill[i] <= 0 ) || 
				( g_Map.m_NpcTable[thePickedNpcID].theKeepupSkill[i] >= c_Max_SkillCode_Num ) ||
				IsItemSkill || IsNotViewSkill )
			{
				DeleteKeepUpSkillCode( i );
			}
			else
			{

#ifdef HOLY_SKILLICON_MODIFY
			SetKeepUpSkillCode( i , g_Map.m_NpcTable[thePickedNpcID].theKeepupSkill[i] , g_Map.m_NpcTable[thePickedNpcID].theHolyLevel );
#else
			SetKeepUpSkillCode( i , g_Map.m_NpcTable[thePickedNpcID].theKeepupSkill[i] );
#endif	
			}
		}

	}
	//유져 픽킹 
	else if( thePickedUserID != -1 && thePickedUserID != MAX_USER_INTABLE )		
	{
	
		DrawHP = g_Pc.m_PcTable[thePickedUserID].char_info.theHP;

		if( DrawHP > g_Pc.m_PcTable[thePickedUserID].char_info.theMaxHP )
			DrawHP = g_Pc.m_PcTable[thePickedUserID].char_info.theMaxHP;

		if( g_Pc.m_Pc[thePickedUserID].m_curt_event.type == SM_STATE_DIE )
			DrawHP = 0;

		if( g_Pc.m_PcTable[ thePickedUserID ].lUnique == -1 )
		{
			thePickedUserID = -1;	
			nRui->SendEvent( WID_NonPlayerInfo_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
			nRui->SendEvent( WID_NonPlayerInfoAddWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
		}

		if( g_Pc.m_Pc[ thePickedUserID ].m_CharAlpha.State == CHAR_NONE_DISPLAY )
		{
			thePickedUserID = -1;
			nRui->SendEvent( WID_NonPlayerInfo_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
			nRui->SendEvent( WID_NonPlayerInfoAddWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
		}


		//같은 나라는 레벨이 모이가 표시 
		//다른 나라는 ??? 표시 ...  //dongs 종족		

		if( g_Pc.m_MainPC.char_info.theCitizen == g_Pc.m_PcTable[thePickedUserID].char_info.theCitizen)
		{

			theHPText->SetVisible(true);
			theSMPText->SetVisible(true);
	
			Rsprintf( RWCHAR(tmpStr), _RT("%d"), g_Pc.m_PcTable[thePickedUserID].char_info.theLevel );
			theLevel->SetText( tmpStr );
			
			Rsprintf( RWCHAR(tmpStr), _RT("%d/%d"), DrawHP , g_Pc.m_PcTable[thePickedUserID].char_info.theMaxHP );
			theHPText->SetText(tmpStr);
			
			Rsprintf( RWCHAR(tmpStr), _RT("%d/%d"), g_Pc.m_PcTable[thePickedUserID].char_info.theSMP, g_Pc.m_PcTable[thePickedUserID].char_info.theMaxSMP );
			theSMPText->SetText(tmpStr);

		}
		else 
		{
			//	Rsprintf(RWCHAR(tmpStr), _T("%d"), g_Pc.m_PcTable[thePickedUserID].char_info.theLevel );
			theLevel->SetText( RCHAR(_RT("???")));
			
			theHPText->SetVisible(false);
			theSMPText->SetVisible(false);
			
		}		

		theFrame->SetClientColor(255,255,255);
		
#ifdef NEW_PVP_DECO

		if( g_Pc.m_PcTable[thePickedUserID].char_info.theCitizen != g_Pc.m_MainPC.char_info.theCitizen)
		{
			//상대가 나보다 20 레벨이 높으면 .. 빨강으로 뿌려준다 .. 
			if( (g_Pc.m_PcTable[thePickedUserID].char_info.theLevel - nRui->GetPcParamInfo()->Level ) >= 20 )	
			{
				theFrame->SetClientImage( TID_MAIN_PC_INFO_BOSS );
				
			}
			//상대가 나보다 10레벨이하가 차이 나면 
			if( (nRui->GetPcParamInfo()->Level - g_Pc.m_PcTable[thePickedUserID].char_info.theLevel) >= 20 )	
			{
				theFrame->SetClientColor(200,200,255);
			}				
		}		

#endif		

		Rsprintf( RWCHAR(tmpStr), _RT("%s"), RWCHAR(g_Pc.m_PcTable[thePickedUserID].char_info.thePCName) );
		theName->SetText( tmpStr );
		
		if(m_iPrevHP < (float)DrawHP )
			m_iPrevHP = (float)DrawHP;		
		
		percent = ((float)m_iPrevHP / (float)g_Pc.m_PcTable[thePickedUserID].char_info.theMaxHP) * 100;
		thePrevHP->SetPercent( percent );
		
		percent = ((float)DrawHP / (float)g_Pc.m_PcTable[thePickedUserID].char_info.theMaxHP) * 100;
		theHP->SetPercent( percent );		

//		if( g_Pc.m_PcTable[thePickedUserID].nIndex == n_Millena_Man || g_Pc.m_PcTable[thePickedUserID].nIndex == n_Millena_Woman )
//		{
			theSMP->SetPercent( ((float)g_Pc.m_PcTable[thePickedUserID].char_info.theSMP / (float)g_Pc.m_PcTable[thePickedUserID].char_info.theMaxSMP) * 100 );
//		}
//		else if( g_Pc.m_PcTable[thePickedUserID].nIndex == n_Rain_Man || g_Pc.m_PcTable[thePickedUserID].nIndex == n_Rain_Woman )
//		{
//			theSMP->SetPercent( ((float)g_Pc.m_PcTable[thePickedUserID].char_info.theSMP / (float)g_Pc.m_PcTable[thePickedUserID].char_info.theMaxSMP) * 100 );
//		}	
		
		//*	Keepup_Skill 지금은 주석, 추후 리소스 작업이 완료되면...처리
		for( i = 0; i < RP_MAX_KEEPUP_SKILL; ++i )
		{
			DWORD SkillCode = g_Pc.m_PcTable[thePickedUserID].char_info.theKeepupSkill[i];
			///-- 아이템을 장착한 후에 관련스킬은 내것이 아닐경우 볼수 없다.
			///-- 마비 저항, 디스펠 저항
			BOOL IsItemSkill = IsItemKeepSkill( SkillCode );
			BOOL IsNotViewSkill = IsNotViewKeepSkill( SkillCode );
			
			if( ( g_Pc.m_PcTable[thePickedUserID].char_info.theKeepupSkill[i] <= 0 ) || 
				( g_Pc.m_PcTable[thePickedUserID].char_info.theKeepupSkill[i] >= c_Max_SkillCode_Num ) ||
				IsItemSkill || IsNotViewSkill )
			{
				DeleteKeepUpSkillCode( i );
			}
			else
			{
				SetKeepUpSkillCode( i , g_Pc.m_PcTable[thePickedUserID].char_info.theKeepupSkill[i] );
			}
		}
	}
	else if(thePickedUserID == MAX_USER_INTABLE)	//자기자신 픽킹
	{
		m_pAddButton->SetVisible(FALSE);
		
		DrawHP = nRui->GetPcParamInfo()->Stat1.theHP;

		if( DrawHP > nRui->GetPcParamInfo()->MaxHP )
			DrawHP = nRui->GetPcParamInfo()->MaxHP;

		if( g_Pc.GetPlayer()->m_curt_event.type == SM_STATE_DIE )
			DrawHP = 0;
		
		theHPText->SetVisible(true);
		theSMPText->SetVisible(true);
	
		Rsprintf( RWCHAR(tmpStr) ,  _RT("%d"), nRui->GetPcParamInfo()->Level );
		theLevel->SetText( tmpStr );
			
		Rsprintf( RWCHAR(tmpStr), _RT("%d/%d"), DrawHP , nRui->GetPcParamInfo()->MaxHP );
		theHPText->SetText(tmpStr);
			
		if( g_Pc.GetPlayer()->GetAttrIndex() == n_Millena_Man || g_Pc.GetPlayer()->GetAttrIndex() == n_Millena_Woman )
		{

			theSMP->SetPercent( ((float)nRui->GetPcParamInfo()->Stat1.theSP / (float)nRui->GetPcParamInfo()->MaxSP) * 100 );			
			Rsprintf(RWCHAR(tmpStr), _RT("%d/%d"), (int)nRui->GetPcParamInfo()->Stat1.theSP, nRui->GetPcParamInfo()->MaxSP );
			theSMPText->SetText( tmpStr );
			
		}
		else if( g_Pc.GetPlayer()->GetAttrIndex() == n_Rain_Man || g_Pc.GetPlayer()->GetAttrIndex() == n_Rain_Woman )
		{
						
			theSMP->SetPercent( ((float)nRui->GetPcParamInfo()->Stat1.theMP / (float)nRui->GetPcParamInfo()->MaxMP) * 100  );			
			Rsprintf(RWCHAR(tmpStr), _RT("%d/%d"), (int)nRui->GetPcParamInfo()->Stat1.theMP, nRui->GetPcParamInfo()->MaxMP );
			theSMPText->SetText( tmpStr );			
		}

		Rsprintf(RWCHAR(tmpStr), _RT("%s"), RWCHAR(nRui->GetPcParamInfo()->PCName) );
		theName->SetText( tmpStr );
		
		if(m_iPrevHP < (float)DrawHP )
			m_iPrevHP = (float)DrawHP;		
		
		percent = ((float)m_iPrevHP / (float)nRui->GetPcParamInfo()->MaxHP) * 100;
		thePrevHP->SetPercent( percent );
		
		percent = ((float)DrawHP / (float)nRui->GetPcParamInfo()->MaxHP) * 100;
		theHP->SetPercent( percent );

		for( i = 0; i < RP_MAX_KEEPUP_SKILL; ++i )
		{
			DWORD SkillCode = nRui->thePcInven.KeepupSkill[i];
			///-- 아이템을 장착한 후에 관련스킬은 내것이 아닐경우 볼수 없다.
			///-- 마비 저항, 디스펠 저항
			BOOL IsItemSkill = IsItemKeepSkill( SkillCode );
			BOOL IsNotViewSkill = IsNotViewKeepSkill( SkillCode );
			
			if( ( nRui->thePcInven.KeepupSkill[i] <= 0 ) ||  
				( nRui->thePcInven.KeepupSkill[i] >= c_Max_SkillCode_Num ) ||
				 IsItemSkill || IsNotViewSkill )
			{
				DeleteKeepUpSkillCode( i );
			}
			else
			{
				SetKeepUpSkillCode( i , nRui->thePcInven.KeepupSkill[i] );
			}
		}
	}
	else
	{
		for( i = 0; i < RP_MAX_KEEPUP_SKILL; ++i )
		{
			DeleteKeepUpSkillCode( i );
		}

		NonPlayerInfoWnd.IsVisible = false;		
	}

/*
	int i = 0;
	int	skill_index = 0;
	WORD tmpKeepUpSkill[RP_MAX_KEEPUP_SKILL] = {0,};

	
	for( i = 0; i < RP_MAX_KEEPUP_SKILL; ++i )
	{
		if( g_Pc.m_PcTable[tableIndex].char_info.theKeepupSkill[i] != 0 )
		{
			tmpKeepUpSkill[skill_index] = g_Pc.m_PcTable[tableIndex].char_info.theKeepupSkill[i];			
			++skill_index;
			
			 g_Pc.m_PcTable[tableIndex].char_info.theKeepupSkill[i] = 0;			
		}		
	}
	
	for( i = 0; i < skill_index; ++i )
	{
		g_Pc.m_PcTable[tableIndex].char_info.theKeepupSkill[i] = tmpKeepUpSkill[i];
	}

  

	for( i = 0; i < RP_MAX_KEEPUP_SKILL; ++i )
	{
		if( g_Map.m_NpcTable[tableIndex].theKeepupSkill[i] != 0 )
		{
			tmpKeepUpSkill[skill_index] = g_Map.m_NpcTable[tableIndex].theKeepupSkill[i];			
			++skill_index;
			
			 g_Map.m_NpcTable[tableIndex].theKeepupSkill[i] = 0;			
		}		
	}
	
	for( i = 0; i < skill_index; ++i )
	{
		g_Map.m_NpcTable[tableIndex].theKeepupSkill[i] = tmpKeepUpSkill[i];
	}

*/

	
	// 캐릭터 선택 이펙트
	if( NonPlayerInfoWnd.IsVisible )
	{
		if( m_pSelectSrc != m_pSrc )
		{
			CBillboardData *pData0 = NULL ;
			CBillboardData *pData1 = NULL ;
			
			D3DXVECTOR3 vMax = m_pSelectSrc->GetBoundMaxPos();
			D3DXVECTOR3 vMin = m_pSelectSrc->GetBoundMinPos();
			D3DXVECTOR3 vTemp = vMax - vMin;

			float fSize = D3DXVec3Length( &vTemp );

			if( NULL != ( pData0 = g_Billboard.m_BillboardData.GetKeyDataPointer( 228 ) ) )
			{
				pData0->m_vStartSize.x = fSize;
				pData0->m_vStartSize.z = fSize;

				pData0->m_vEndSize.x   = fSize * 0.88f;
				pData0->m_vEndSize.z   = fSize * 0.88f;
			}
			
			if( NULL != ( pData1 = g_Billboard.m_BillboardData.GetKeyDataPointer( 319 ) ) )
			{
				pData1->m_vStartSize.x = fSize * 0.3f;
				pData1->m_vStartSize.z = fSize * 0.3f;
				pData1->m_vStartSize.y = 0.0f;

				pData1->m_vEndSize.x   = fSize * 0.3f;
				pData1->m_vEndSize.z   = fSize * 0.3f;
				pData1->m_vEndSize.y = 0.0f;
			}
			
			if( m_ISDisplay )
			{
				g_Particle.DelEffSetCon_All( EF_CHARACTER_SELECT , 0 );
				
				g_Particle.SetEffSetCon( 0.0f , EF_CHARACTER_SELECT , 0 , EFFECT_SET_LOOP_TIME , m_pSelectSrc->GetUnique() , m_pSelectSrc->GetIndex() );
			}
			else
			{
				g_Particle.SetEffSetCon( 0.0f , EF_CHARACTER_SELECT , 0 , EFFECT_SET_LOOP_TIME , m_pSelectSrc->GetUnique() , m_pSelectSrc->GetIndex() );
			}

			m_pSrc	  = m_pSelectSrc;
			m_ISDisplay = TRUE;
		}
	}
	else
	{
		if( m_ISDisplay )
		{
			g_Particle.DelEffSetCon_All( EF_CHARACTER_SELECT , 0 );
		}

		m_ISDisplay = FALSE;
		m_pSrc = m_pSelectSrc = NULL;
	}

	m_lPrevTime = g_nowTime;

	return;
}

void CNonPlayerInfoWnd::SetPickCharacter(Character *pChar)
{
	if( pChar == NULL )
	{
		if( m_ISDisplay )
		{
			g_Particle.DelEffSetCon_All( EF_CHARACTER_SELECT , 0 );
		}

		m_ISDisplay = FALSE;
		m_pSrc = m_pSelectSrc = NULL;
	}	
	
	if( m_pSelectSrc != pChar )
	{
		DeleteAllKeepUpSkill();
	}

	m_pSelectSrc = pChar;

	//by simwoosung
	if(thePickedUserID != -1 && thePickedUserID != MAX_USER_INTABLE)
	{
        g_PartyInforWnd.SelectPartyMemberWnd(thePickedUserID);
		m_iPrevHP = g_Pc.m_PcTable[thePickedUserID].char_info.theHP;
	}
	else if(thePickedNpcID != -1)
	{
		m_iPrevHP = g_Map.m_NpcTable[thePickedNpcID].theHP;
	}
	else if(thePickedUserID == MAX_USER_INTABLE)
	{
		m_iPrevHP = nRui->GetPcParamInfo()->Stat1.theHP;
	}
}

//-----------------------------------------------------------------------------
void CNonPlayerInfoWnd::Proc( SEventMessage* EMsg )
{
	switch( EMsg->FocusWnd )
	{	
	// 각각의 컨트롤에 따라서 처리 	
//	case WID_NonPlayerInfo_StatICONImg1:			// 혹시 툴팁 처리???
//	case WID_NonPlayerInfo_StatICONImg2:
//	case WID_NonPlayerInfo_StatICONImg3:
//	case WID_NonPlayerInfo_StatICONImg4:
//	case WID_NonPlayerInfo_StatICONImg5:
//	case WID_NonPlayerInfo_StatICONImg6:
//		break;

	case WID_NonPlayerInfo_AddBtn:
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			NonPlayerInfoAddWnd.IsVisible = !NonPlayerInfoAddWnd.IsVisible;
			if( NonPlayerInfoAddWnd.IsVisible )
			{
				nRui->SendEvent( WID_NonPlayerInfoAddWnd, n_emSetVisible, (EPARAM)NonPlayerInfoAddWnd.IsVisible, 0, 0, 0 );
				//nRui->Chain.Ordering( Container->Get( WID_NonPlayerInfoAddWnd ) );			
			}
			else
			{
				nRui->SendEvent( WID_NonPlayerInfoAddWnd, n_emSetVisible, (EPARAM)NonPlayerInfoAddWnd.IsVisible, 0, 0, 0 );
				if(g_NonPlayerInfoAdd2Wnd.IsVisible)
				{
					g_NonPlayerInfoAdd2Wnd.Close();
				}
			} 
		}
		break;		

/*	case WID_NonPlayerInfo_ChatBtn:
		break;
	case WID_NonPlayerInfo_PartyBtn:
		break;
	case WID_NonPlayerInfo_FriendBtn:
		break;
	case WID_NonPlayerInfo_TradeBtn:
		break;
	case WID_NonPlayerInfo_ChaseBtn:
		break;
	case WID_NonPlayerInfo_AttackBtn:
		break; */
	default:
		break; 
	}
}

void CNonPlayerInfoWnd::Start2DEffect(int iHP, int iRealHP)
{
	if(iHP == iRealHP)  //미스처리
		return;
	
	m_iDamage = iHP - iRealHP;
	
	if(!m_BStartEffect)
	 m_iPrevHP = iHP;		
	
	m_fCumulTime = 0.0f;

	m_fBarAlpha = 255.0f;
	thePrevHP->SetBarAlpha((int)m_fBarAlpha);
	m_BStartEffect = TRUE;

	m_lPrevTime = g_nowTime;
}

void CNonPlayerInfoWnd::Start2DEffect(int iHP, _STargetInfo aTargetInfo, WORD aSkillCode)
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
		effect_type  = skill_Info->theEffectInfo[i].theEffectType;

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
				 Start2DEffect(iHP, effect_value);
				 break;
		}	
	}
}

//-----------------------------------------------------------------------------
