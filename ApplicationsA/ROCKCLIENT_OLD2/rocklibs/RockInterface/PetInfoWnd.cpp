// PetInfoWnd.cpp: implementation of the CPetInfoWnd class.
//
//////////////////////////////////////////////////////////////////////
#include "Rui.h"
//////////////////////////////////////////////////////////////////////////
#include "FrameWnd.h"
#include "Button.h"
#include "ImageBox.h"
#include "TextBox.h"
#include "EditBox.h"
#include "ProgressBar.h"
#include "UpGauge.h"
#include "SlideBar.h"
#include "CheckButton.h"
#include "RadioButton.h"
#include "ListBox.h"
#include "ScrollBar.h"
#include "ComboBox.h"
#include "Spin.h"
#include "Wnd.h"


//////////////////////////////////////////////////////////////////////////

#include "Define.h"
#include "PetInventoryWnd.h"

#include "PetMainWndProc.h"
#include "PetInfoWnd.h"

#include "..\\..\\RockClient.h"


//////////////////////////////////////////////////////////////////////////

extern long			g_nowTime;


const DWORD c_Max_DrawKeepupItemTime( 1000.0f * 60.0f * 60.0f * 24.0f );		// 24 Hours 

const DWORD c_PetBaseItemTime(  60 * 60 * 24 );		// 24시간 기준점


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CPetInfoWnd g_PetInfoWnd;


void  CPetInfoWnd::Composition()
{
	CompositionNew();
}

void CPetInfoWnd::CompositionOld()
{

}

void CPetInfoWnd::CompositionNew()
{
	//-------------------------------------------------------------------------
	//
	// 다른 유저 및 NPC 정보창 
	//
	//-------------------------------------------------------------------------
	SRect infownd;	
	
	infownd.x = 20;
	infownd.y = 104 ; 
	infownd.w	= 188; 
	infownd.h	= 64;

	FlashFrame = (CFlashFrameWnd*)nRui->CreateWnd( n_wtFlashFrameWnd, WID_PETINFO_WND, WID_None, 
												 	infownd.x, infownd.y, infownd.w,  infownd.h, false );
	FlashFrame->SetClientImage( TID_MAIN_PETINFO );	
	FlashFrame->SetCaption( _RT("Pet") );
	FlashFrame->SetWndProc( this );
	FlashFrame->SetTitle( false );
	FlashFrame->SetMove( true );
	FlashFrame->SetVisible( false );
	FlashFrame->SetZorder( n_zoBottom );
	FlashFrame->Initialize();
	FlashFrame->SetIntervalTime(0.2f);
	FlashFrame->SetFlashImage(TID_petInfo_Warning);	
    FlashFrame->SetFlashColorOption(D3DTOP_MODULATE);
    FlashFrame->SetFlashColor(D3DCOLOR_ARGB(63, 255, 128, 128 ));
    FlashFrame->SetActiveFlash(false);
    FlashFrame->SetFlashImgSize(0, 0, 188, 64);  

    GlowImg = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_PETINFO_WND, 
											0, 0, 188, 64, false );
	GlowImg->SetClientImage(TID_PetInfo_GlowWnd);	
	GlowImg->SetGlow(TRUE);
	GlowImg->SetVisible(false);
	GlowImg->SetZorder( n_zoBottom );
	
	m_pChaImgBox = (CCharImgBox*)nRui->CreateWnd( n_wtCharImgBox, WID_None, WID_PETINFO_WND, 
											-25, 3, 64, 64, false );
	m_pChaImgBox->SetClientImage(TID_None);
	m_pChaImgBox->m_IsMask = true;
	m_pChaImgBox->SetFocusCheck(false);

#ifdef APPLY_BOLD_FONT	
	Level = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_PETINFO_WND, 52, 48, 19, 15, false );
#else
	Level = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_PETINFO_WND, 54, 48, 19, 15, false );
#endif
	Level->SetFontR( n_fRsvFontWndStatic2 );
	Level->SetAlignText( n_atCenter, n_atCenter );	
	Level->SetClientColor( 240, 240, 200 );
	Level->SetBlank( 0, 0 );
	Level->SetText(_RT(""));

	Name = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_PETINFO_WND,  76, 48, 144, 13 ,  false );
	Name->SetFontR( n_fRsvFontWndStatic );
	Name->SetAlignText( n_atLeft, n_atCenter );		
	Name->SetBlank( 0, 0 );
	Name->SetTextColor( 0, 0, 0 );
	Name->SetText(_RT(""));
	
	HP = (CProgressBar*)nRui->CreateWnd( n_wtProgressBar, WID_None, WID_PETINFO_WND,56, 21 , 110, 9, false );   
	HP->SetClientImage( TID_None );
	HP->SetBarImage( TID_Pet_hp_red );
	HP->SetBarBlank( 0, 0 );
	HP->InitProgress( 100 );
	
	CP = (CProgressBar*)nRui->CreateWnd( n_wtProgressBar, WID_None, WID_PETINFO_WND,56, 33, 110, 9, false );  
	CP->SetClientImage( TID_None );
	CP->SetBarImage( TID_Pet_cp_green );
	CP->SetBarBlank( 0, 0 );
	CP->InitProgress( 100 );
	//-------------------------------------------------------------------------
	// HP
//	HPText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_PlayerInfo_HPText, WID_PlayerInfo_Wnd, 110, 42, 100, 12, false );
	HPText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_PETINFO_WND, 58, 22, 110, 5, false );  
	HPText->SetFontg( n_ftGulimChe, 5, n_fwNormal, n_fdNormal );
	HPText->SetImageNum( true );
	HPText->SetClientImage( TID_None );
	HPText->SetClientColor( 180, 175, 160 );
	HPText->SetAlignText( n_atCenter, n_atCenter );
	HPText->SetText( _RT("") );
	HPText->SetTextColor( 255, 220, 220 );
	HPText->SetHID( HD_S_PET_HP );
 
	// CP
//	SMPText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_PlayerInfo_SMPText, WID_PlayerInfo_Wnd, 110, 57, 100, 12, false );
	CPText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_PETINFO_WND, 58, 34, 110, 5, false );   
	CPText->SetFontg( n_ftGulimChe, 5, n_fwNormal, n_fdNormal );
	CPText->SetImageNum( true );
	CPText->SetClientImage( TID_None );
	CPText->SetClientColor( 180, 175, 160 );
	CPText->SetAlignText( n_atCenter, n_atCenter );
	CPText->SetText( _RT("") );
	CPText->SetTextColor( 220, 220, 255 );
	CPText->SetHID( HD_S_PET_CP );
	
	//펫상태창 매뉴 .. 
	m_PetMenubtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_PETINFO_WND,169 , 20 , 22, 21, false );
	m_PetMenubtn->SetBtnImage( TID_MAIN_MENUBT, TID_MAIN_MENUBT_OVR, TID_MAIN_MENUBT_CLK, TID_None );
	m_PetMenubtn->SetAlignText( n_atCenter, n_atCenter );	
	m_PetMenubtn->SetTextColor(0, 0, 0);
	m_PetMenubtn->SetHID( HD_MENU ); 

	m_MenuFrame = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_None ,WID_None,218, 122 , 86, 28, false );
	m_MenuFrame->SetClientImage( TID_icon_boxbarS );
	m_MenuFrame->SetCaption( _RT("") );
	m_MenuFrame->SetWndProc( this );
	m_MenuFrame->SetTitle( false );
	m_MenuFrame->SetMove( false );
	m_MenuFrame->SetVisible( false );
	m_MenuFrame->Initialize();
	
	FlashFrame->SetMoveLinkFrameWnd(m_MenuFrame);

	m_pCallOffBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, m_MenuFrame->GetID(),  6, 3, 24, 22, false );
	m_pCallOffBtn->SetBtnImage( TID_icon_pet_cancel, TID_icon_pet_cancel_bright, 	TID_icon_pet_cancel_dark, TID_None );
	m_pCallOffBtn->SetAlignText( n_atCenter, n_atCenter );	
	m_pCallOffBtn->SetHID(HID_PET_CALLOFF_BTN);

	m_pPetInvenBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, 	m_MenuFrame->GetID(),   32, 3, 21, 22, false );
	m_pPetInvenBtn->SetBtnImage( TID_icon_pet_bag, TID_icon_pet_bag_bright, 	TID_icon_pet_bag_dark	, TID_None );
	m_pPetInvenBtn->SetAlignText( n_atCenter, n_atCenter );	
	m_pPetInvenBtn->SetHID(HID_PET_INVEN_BTN);
	
	m_pPetEmotionBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, 	m_MenuFrame->GetID(),   55, 3, 26, 20, false );
	m_pPetEmotionBtn->SetBtnImage( TID_icon_pet_emotion, TID_icon_pet_emotion_bright, 	TID_icon_pet_emotion_dark	, TID_None );
	m_pPetEmotionBtn->SetAlignText( n_atCenter, n_atCenter );	
	m_pPetEmotionBtn->SetHID(HID_PET_EMOTION_BTN);
	/********************************************************************
	-유료 아이템 사용 정보 표시 
	*********************************************************************/
	m_KeepUpFrame = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_None ,WID_None,69, 169 , 72, 28, false );
	m_KeepUpFrame->SetClientImage( WID_None );
	m_KeepUpFrame->SetCaption( _RT("") );
	m_KeepUpFrame->SetWndProc( this );
	m_KeepUpFrame->SetTitle( false );
	m_KeepUpFrame->SetMove( false );
	m_KeepUpFrame->SetVisible( false );
	m_KeepUpFrame->Initialize();
	m_KeepUpFrame->SetZorder( n_zoBottom );
	FlashFrame->SetMoveLinkFrameWnd2(m_KeepUpFrame);
	
	for(int i  = 0; i < RP_MAX_PET_USEDITEM; ++i )
	{
		m_pKeepupItemSlot[i] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, m_KeepUpFrame->GetID() , (24*i), 0 , 24, 24, false );
		m_pKeepupItemSlot[i]->SetSlotGroup( n_SGROUP_KEEPUP_COMMERCIALITEM );
		m_pKeepupItemSlot[i]->SetHID( HID_ITEMTIP );
		m_pKeepupItemSlot[i]->SetVisible( true );
		m_pKeepupItemSlot[i]->SetScaleTrans( true );	
		m_pKeepupItemSlot[i]->SetDelayShow(false);

		m_pKeepupItemText[i] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None,  m_KeepUpFrame->GetID(),  (24*i) , 0, 24, 22, false );  
		m_pKeepupItemText[i]->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
		m_pKeepupItemText[i]->SetClientImage( TID_None );	
		m_pKeepupItemText[i]->SetAlignText( n_atCenter, n_atBottom );
		m_pKeepupItemText[i]->SetText( _RT("") );
		m_pKeepupItemText[i]->SetFocusCheck(false);
		m_pKeepupItemText[i]->SetVisible(false);
	}

	Init();
}

void CPetInfoWnd::ClearPetInfo()
{
		
	Level->SetText(_RT(""));
	Name->SetText(_RT(""));
	HPText->SetText(_RT(""));
	CPText->SetText(_RT(""));
	HP->SetPercent( 0 );
	CP->SetPercent( 0 );
//	Exp->SetPercent( 0 ); 

	m_CurPetNum = -1  ;
	for(int PetInc = 0 ; PetInc < RP_MAX_PET_USEDITEM ; ++PetInc)
	{
		if( m_pKeepupItemSlot[PetInc]->IsVisible() == false )
		{
			break;
		}

		m_pKeepupItemSlot[PetInc]->SetVisible(false);
		m_pKeepupItemText[PetInc]->SetVisible(false);		
	}
	
}

void CPetInfoWnd::Open(int PetSlot)
{
	m_CurPetNum =PetSlot;
	if( m_CurPetNum != -1 )
	{
		nRui->SendEvent( WID_PETINFO_WND  , n_emSetVisible, (EPARAM)true, 0, 0, 0 );
		nRui->SendEvent( m_KeepUpFrame->GetID(), n_emSetVisible, (EPARAM)true, 0, 0, 0 );		
	
		if( m_CurPetNum == MERCE_SLOT_NUM )
		{
			m_PetMenubtn->SetVisible( false );
		}
		else
		{
			m_PetMenubtn->SetVisible( true );
		}	
	}
}

void CPetInfoWnd::Close()
{
	
	nRui->SendEvent( m_MenuFrame->GetID(), n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	nRui->SendEvent( WID_PETINFO_WND  , n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	nRui->SendEvent( m_KeepUpFrame->GetID(), n_emSetVisible, (EPARAM)false, 0, 0, 0 );		
	ClearPetInfo();

}


void CPetInfoWnd::MenuComPosition()
{
	if( m_MenuFrame != NULL)
	{
		return ;
	}
}

void CPetInfoWnd::Init()
{

	PcInven  = nRui->GetPcInvenInfo();
	m_IsMenuVisible = false;
	m_CurPetNum = -1;
}

void CPetInfoWnd::Update()
{
//	RLGS("PetInfoWnd Update");

	static TCHAR tmpStr[128] = {0,};
	
	int Percent = 0 ; 
	int i =0 ;

	if( NULL == PcInven->thePetData[m_CurPetNum] )
	{
		return;
	}	

	for( i = 0 ; i < 3 ; ++i )
	{

		m_pKeepupItemSlot[i]->PcItem = g_PetMainWndProc.m_PetText[m_CurPetNum].KeepupItemSlot[i]->PcItem ;

		if ( g_PetMainWndProc.m_PetText[m_CurPetNum].KeepupItemSlot[i]->IsVisible()) 
		{
#ifdef PET_PAY_USEITEM_RENEWAL //잔상버그 그리전에 없애준다

			m_pKeepupItemSlot[i]->SetVisible(false);
			m_pKeepupItemText[i]->SetVisible(false);

#endif //PET_PAY_USEITEM_RENEWAL
			if((int)g_PetMainWndProc.m_PetText[m_CurPetNum].fRemainTime[i] > 0 )
			{

#ifdef PET_PAY_USEITEM_RENEWAL 

				m_pKeepupItemSlot[i]->SetVisible(true); //그린후 다시 체크
				m_pKeepupItemText[i]->SetVisible(true);				
			
				if((int)g_PetMainWndProc.m_PetText[m_CurPetNum].fRemainTime[i] > c_PetBaseItemTime)//Day
				{ 
					Rsprintf(RWCHAR(tmpStr) , _RT("%d"), (int)g_PetMainWndProc.m_PetText[m_CurPetNum].fRemainTime[i] /60 /60 /24 );	
					m_pKeepupItemText[i]->SetText(tmpStr);
					m_pKeepupItemText[i]->SetTextColor( 0 , 255 , 255 );
				}
				else if((int)g_PetMainWndProc.m_PetText[m_CurPetNum].fRemainTime[i] < c_PetBaseItemTime 
					    && (int)g_PetMainWndProc.m_PetText[m_CurPetNum].fRemainTime[i] > c_PetBaseItemTime/24) //Hour
				{
					Rsprintf(RWCHAR(tmpStr) , _RT("%d"), (int)g_PetMainWndProc.m_PetText[m_CurPetNum].fRemainTime[i] /60 /60 );	
					m_pKeepupItemText[i]->SetText(tmpStr);
					m_pKeepupItemText[i]->SetTextColor( 0 , 255 , 0 );
				}
				else //Min
				{ 
					Rsprintf(RWCHAR(tmpStr) , _RT("%d"), (int)g_PetMainWndProc.m_PetText[m_CurPetNum].fRemainTime[i] /60 );	
					m_pKeepupItemText[i]->SetText(tmpStr);
					m_pKeepupItemText[i]->SetTextColor( 255 , 255 , 255 );
				}
#else
				Rsprintf( RWCHAR(tmpStr), _RT("%d"), (int)g_PetMainWndProc.m_PetText[m_CurPetNum].fRemainTime[i]);
				m_pKeepupItemText[i]->SetText(tmpStr);

#endif //PET_PAY_USEITEM_RENEWAL
			}
			else
			{
				m_pKeepupItemText[i]->SetText(_RT(""));
			}						
		}
		else 
		{
			m_pKeepupItemSlot[i]->SetVisible(false);
			m_pKeepupItemText[i]->SetVisible(false);			
		}
	}
	
	int nNpcCode = PcInven->thePetData[m_CurPetNum]->NpcCode;
	m_pChaImgBox->SetRenderCharImgProperty( nNpcCode, c_NpcRace_Mob, 
									        CHARIMAGE_64SIZE, false, true );

	Rsprintf( RWCHAR(tmpStr), _RT("%s"), RWCHAR(PcInven->thePetData[m_CurPetNum]->Name));
	Name->SetText(tmpStr);
	
	Rsprintf( RWCHAR(tmpStr), _RT("%d"), PcInven->thePetData[m_CurPetNum]->Level);
	Level->SetText(tmpStr);


	Rsprintf( RWCHAR(tmpStr), _RT("%d/%d"), PcInven->thePetData[m_CurPetNum]->HP, 
								   PcInven->thePetData[m_CurPetNum]->MaxHP);
	HPText->SetText(tmpStr);
	
	
	Rsprintf( RWCHAR(tmpStr), _RT("%d/%d"), PcInven->thePetData[m_CurPetNum]->CP, 
								   PcInven->thePetData[m_CurPetNum]->MaxCP);
	CPText->SetText(tmpStr);

	Percent = ( (float)PcInven->thePetData[m_CurPetNum]->HP /  
					   PcInven->thePetData[m_CurPetNum]->MaxHP ) * 100;
	HP->SetPercent(Percent);
	
	if( ( 0 < Percent) &&  ( Percent< 30 ) )
	{
		FlashFrame->SetActiveFlash(true);
	}
	else
	{
		FlashFrame->SetActiveFlash(false);
	}
	
	Percent = ((float)PcInven->thePetData[m_CurPetNum]->CP  / 
			  (float)PcInven->thePetData[m_CurPetNum]->MaxCP) * 100;
	
	CP->SetPercent( Percent );

//	Percent = ((float)PcInven->thePetData[m_CurPetNum]->Exp  / (float)PcInven->thePetData[m_CurPetNum]->MaxExp) * 100;
//	Exp->SetPercent(Percent);

	//NPC 정보도 갱신해준다..	
	
	int npc_tbl_index = g_Map.FindNpc(PcInven->thePetData[m_CurPetNum]->PetID);
	if( npc_tbl_index != MAX_NPC_INTABLE)
	{
		g_Map.m_NpcTable[npc_tbl_index].theLevel = PcInven->thePetData[m_CurPetNum]->Level;
		g_Map.m_NpcTable[npc_tbl_index].theHP = PcInven->thePetData[m_CurPetNum]->HP;
		g_Map.m_NpcTable[npc_tbl_index].theMaxHP	= PcInven->thePetData[m_CurPetNum]->MaxHP;
		g_Map.m_NpcTable[npc_tbl_index].theSMP	= PcInven->thePetData[m_CurPetNum]->CP;
		//CP 갱신이 없어 .. 
		//g_Map.m_NpcTable[npc_tbl_index]

	}

}

#ifdef PET_PAY_USEITEM_HELPTIP
DWORD CPetInfoWnd::GetTimePetPayEffect(DWORD PetPayCode) //유료아이템의 코드를 비교해서 지속 시간 얻어온다
{
	int i, j =0;
	
	for( i = 0 ; i < MAX_PET_SLOT_NUM; i++ )
	{
		for( j = 0 ; j < RP_MAX_PET_USEDITEM ; j++ ) //펫 과 슬롯이이템 비교
		{
			if( PcInven->thePetData[i]->KeepupItem[j].Code == PetPayCode )
			{
				return g_PetMainWndProc.m_PetText[i].fRemainTime[j];
			}
		}
	}

	return 0;
}
void CPetInfoWnd::GetTimePetEffectStr(DWORD Code , TCHAR *str)//얻어온 시간을 변환
{
	int PetTime = GetTimePetPayEffect(Code) ;
	
	if( PetTime > c_PetBaseItemTime ) //Day
	{ 
		Rsprintf(RWCHAR(str) , _RT("%s : %d%s/%d%s/%d%s") , G_STRING_CHAR(IDS_HELPTIP_REMAIN_TIME) , 
		(PetTime/60 /60 /24)%30 , G_STRING_CHAR(IDS_HELPTIP_DAY), (PetTime/60 /60)%24 ,G_STRING_CHAR(IDS_WND_CLOCK_HOUR) ,(PetTime/60 )%60 , G_STRING_CHAR( IDS_WND_CLOCK_MIN) );
	}
	else if( PetTime < c_PetBaseItemTime && PetTime > c_PetBaseItemTime/24 ) //Hour
	{ 
		Rsprintf(RWCHAR(str) , _RT("%s : %d%s/%d%s") , G_STRING_CHAR(IDS_HELPTIP_REMAIN_TIME) , 
		(PetTime/60 /60)%24 ,G_STRING_CHAR(IDS_WND_CLOCK_HOUR) ,(PetTime/60 )%60 , G_STRING_CHAR( IDS_WND_CLOCK_MIN) );
	}
	else //Min
	{ 
		Rsprintf(RWCHAR(str) , _RT("%s : %d%s") , G_STRING_CHAR(IDS_HELPTIP_REMAIN_TIME) , 
		(PetTime/60 )%60 , G_STRING_CHAR( IDS_WND_CLOCK_MIN) ); 
	}

}
#endif //PET_PAY_USEITEM_HELPTIP


void CPetInfoWnd::Proc( SEventMessage* EMsg )
{
	
	if( EMsg->FocusWnd == m_PetMenubtn->GetID() ) 
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			if(!m_IsMenuVisible)
			{		
				nRui->SendEvent( m_MenuFrame->GetID(), n_emSetVisible, (EPARAM)true, 0, 0, 0 );
				m_IsMenuVisible = true;
			}
			else
			{
				nRui->SendEvent( m_MenuFrame->GetID(), n_emSetVisible, (EPARAM)false, 0, 0, 0 );					
				m_IsMenuVisible	 = false;
			}
		}
	}	


	if( EMsg->FocusWnd  == m_pCallOffBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			//소환 해제 창 
			nRui->SendEvent( g_PetMainWndProc.m_pFrameOffWnd->GetID(), n_emSetVisible, (EPARAM)true, 0, 0, 0 );		
			return;
			
			//초기화 값이 아니면 
			//g_RockClient.Send_CsCancelCallPet();
		}	
	}

	if(EMsg->FocusWnd == m_pPetInvenBtn->GetID())
	{
		if(EMsg->Notify == NM_BTN_CLICK)
		{

			if(g_PetInventoryWnd.IsVisible == true)
			{
				g_PetInventoryWnd.Close();	
				
			}
			else 
			{
#ifdef PET_TIME_TEST //시간제 펫의경우는 인벤토리 사용불가
				if( PcInven->thePetData[m_CurPetNum]->NpcCode > 6190  && PcInven->thePetData[m_CurPetNum]->NpcCode < 6197 )
					{
						SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_INVENTORY_NOTOPEN)) ;
						return ;
					}
#else 
#endif // PET_TIME_TEST
				//마우스 좌표에 인벤토리가 나타남 .. 
				g_PetInventoryWnd.FrameWnd->Reposition(EMsg->Mouse.Aposition.x,EMsg->Mouse.Aposition.y);
					
				g_PetInventoryWnd.m_CurSlotIndex = m_CurPetNum;
				g_PetInventoryWnd.Open();
			}							
		}

	}

}
