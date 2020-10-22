///////////////////////////////////////////////////////////////////////////////
///
///		File		: HelpItemTip.cpp
///		Desc		:
///
///		Author		: Park Soo-Hyun
///		Team		: Program - Client Team
///		Date		: 2004-05-24
///
///		Copyright(c) 2004, Rocksoft Co., Ltd. ALL RIGHTS RESERVED
///
///////////////////////////////////////////////////////////////////////////////

#include "..\\RockPCH.h"
#include "Container.h"
#include "Rui.h"
#include "HelpItemTip.h"


#include "..\\..\\RockClient.h"

#include ".\\..\\bravolibs\\obj\\player_manager.h"

#include "ItemRepairWndproc.h"
#include "StoreWndProc.h"
#include "UiUtil.h"
#include "stringmanager.h"

#include "CUserStoreSellWnd.h"
#include "CUserStoreBuyWnd.h"
#include "ExchangeWndProc.h"

#include "SkillLevelUpWnd.h"
#include "ListTime.h"
#include "PetInfoWnd.h"

//D3DCOLOR g_StrColor = D3DCOLOR_XRGB( 120, 110, 75 );
D3DCOLOR g_StrColor = D3DCOLOR_XRGB( 0, 0, 0 );


//-----------------------------------------------------------------------------
CHelpItem::CHelpItem( SDesktop* DT )
{
	pDT				= DT;
	
	theSlotWnd		= NULL;
	theSlotWndID	= WID_None;
	thePcItem		= NULL;
	
	theTipType		= n_TIP_NONE;
	
	theClient.x		= 0;
	theClient.y		= 0;
	theClient.w		= MAX_ITEMTIP_WIDTH;
	theClient.h		= 0;
	
	theFrameWidth	= 2;
	
	theSlotWndID	= HID_None;
	//theFontg		= Fontman->GetFont( n_ftGulimChe, 12, n_fwMedium, n_fdNormal );
	theFontg = NULL;
	
	SkillNumLine	= 0;
	DescStartpos	= 0;
	
	memset( &theStrInfo, 0, sizeof( SStringInfo ) );
	
	theTColor = D3DCOLOR_XRGB( 221, 225, 237 );
	
	InitWeapon();
	InitWear();
	InitEtc();
	InitSkill();
}

//-----------------------------------------------------------------------------
CHelpItem::~CHelpItem()
{
}

//-----------------------------------------------------------------------------
void CHelpItem::Initialize()
{
}
///---------------------------------------------------------------------------
///-- Refresh
///---------------------------------------------------------------------------
///-- 같은 위치를 가르키는 Item 이 data 가 변경되었을 경우 이 함수를 통해 새로 읽음을 설정
void CHelpItem::Refresh()
{
	theSlotWndID = HID_None;
}

//-----------------------------------------------------------------------------
void CHelpItem::Set( WNDID wndid, SRect* wc_size, SPcItem* pcitem )
{
	//id와 code가 같다면..같은 툴팁
	if( ( theSlotWndID == wndid ) && ( thePcItem->Code == pcitem->Code ) && ( thePreSetPcItem.Code == pcitem->Code ) )
	{
		//그래도 위치 값은 계산....
		CalcClientSize( wc_size );
		CalcFrameSize();
		return;
	}
	
	theSlotWndID	= wndid;	
	theSlotWnd		= Container->Get( theSlotWndID );	
	thePcItem		= pcitem;
	thePreSetPcItem = *( pcitem );			// 포인터 자체 pcitem이 수정되면 thePcItem 값도 변화 되므로 변화 여부를 알 수 없다. 그래서 한번 더 체크!!! 
	
	//n_TIP_ITEM_WEAPON -> 무기( 제1분류 Class값이 1인 경우 )
	//n_TIP_ITEM_WEAR -> 장비( 제1분류 Class값이 2~8일 경우 )
	//n_TIP_ITEM_ETC -> 기타( 제1분류 Class값이 9~11일 경우 )
	//n_TIP_SKILL -> 스킬일때....
	
    BYTE bClass = 1;
	
	if(thePcItem->ItemTable)
		bClass = thePcItem->ItemTable->theClass;
	
	if(thePcItem->SkillTable)
		bClass = n_ICLASS_NONE;
	
	InitExtraInfo();
	
	switch( bClass )
	{
	case n_Class_Weapon:		//1  무기
		InitWeapon();
		SetTipWeapon();
		break;
	case n_Class_Helmet:		//0  머리	
	case n_Class_CoatArmor:		//2  상의
	case n_Class_PantsArmor:	//3  하의
	case n_Class_Gloves:		//4  장갑
	case n_Class_Shoes:			//5  부츠
	case n_Class_Serve:			//6  서브
	case n_Class_Accessory:		//7  장신구
	case n_Class_Shield:		//8  방패
	case n_Class_Riding:		// 말 에는 말굽 내구도가 있다 ㅠㅠ 
	case n_Class_RidingItem:	//말 아이템 
	case n_Class_Skin:			//스킨 
		InitWear();		
		SetTipWear();
		break;
	case n_Class_Use:			//9  소모품( 사용가능 )
	case n_Class_Useless:		//10 비소모품( 사용 불가능 )
	case n_Class_Special:		//11 특수 아이템
	case n_Class_Face:			//얼굴
	case n_Class_Quest:			//퀘스트
	case n_Class_Eldastone:		// 14 엘다스톤 
	case n_Class_FrontierPet:
	case n_Class_PersonalPet:	//펫
	case n_Class_Weapon_StrengthStone:	//무기 강화석 
	case n_Class_Armor_StrengthStone:	//방어구 강화석
		InitEtc();
		SetTipEtc();
		break;
	case n_ICLASS_NONE://SKILL
		InitSkill();
		SetTipSkill();
		break;
	}
	
	CalcClientSize( wc_size );
	CalcFrameSize();
}

//-----------------------------------------------------------------------------
void CHelpItem::CalcClientSize( SRect* wc_size )
{
	CWnd* Fwnd	= NULL;
	
	int sc_x = 0;
	int sc_y = 0;
	
	if( ( nRui->GetFocusWnd() != WID_None ) && ( nRui->GetFocusWnd() != WID_DeskTop ) )
	{
		Fwnd = Container->Get( nRui->GetFocusWnd() );
		WNDID Pid = Fwnd->GetParentID();
		
		if(Pid == WID_InventoryWnd)
		{
			Fwnd = 	Container->Get( WID_InventoryWnd );
			SRect sRect;
			Fwnd->GetClientSize(&sRect);
			
			sc_x = sRect.x + (sRect.w / 2);   
			sc_y = pDT->Height / 2;
			
		}
		else
		{
			sc_x = pDT->Width / 2;
			sc_y = pDT->Height / 2;
		}
	}
	
	int wc_x = wc_size->x;
	int wc_y = wc_size->y;
	int wc_w = wc_size->x + wc_size->w;
	int wc_h = wc_size->y + wc_size->h;
	
	//wndclient_center
	int wc_cnt = ( wc_size->w / 2 ) +  wc_size->x;
	
	if( wc_cnt <= sc_x )
	{
		//왼쪽
		if( ( wc_y - theClient.h ) > 0 )
		{
			theClient.x = wc_w;
			theClient.y = wc_y - theClient.h;
		}
		else
		{
			theClient.x = wc_w;
			theClient.y = wc_h;
		}
		
	}
	else if( wc_cnt > sc_x )
	{
		//오른쪽
		if( ( wc_y - theClient.h ) > 0 )
		{
			theClient.x = wc_x - theClient.w;
			theClient.y = wc_y - theClient.h;
		}
		else
		{
			theClient.x = wc_x - theClient.w;
			theClient.y = wc_h;
		}
	}
}

//-----------------------------------------------------------------------------
void CHelpItem::CalcFrameSize()
{
	//가로 프레임 이미지는 괜찮은데..세로 프레임 이미지가 원본 크기보다 작으면 뭉개져서 나온다....
	//일단...크기를 줄여서 해결....원인은?
	
	//위
	theNorthFrm.x = theClient.x;
	theNorthFrm.y = theClient.y - theFrameWidth;
	theNorthFrm.w = theClient.w;
	theNorthFrm.h = theFrameWidth;
	//
	theNorthEastFrm.x = theClient.x + theClient.w;
	theNorthEastFrm.y = theClient.y - theFrameWidth;
	theNorthEastFrm.w = theFrameWidth;
	theNorthEastFrm.h = theFrameWidth;
	//오른쪽
	theEastFrm.x = theClient.x + theClient.w;
	theEastFrm.y = theClient.y;
	theEastFrm.w = theFrameWidth;
	theEastFrm.h = theClient.h;
	//
	theSouthEastFrm.x = theClient.x + theClient.w;
	theSouthEastFrm.y = theClient.y + theClient.h;
	theSouthEastFrm.w = theFrameWidth;
	theSouthEastFrm.h = theFrameWidth;
	//아래
	theSouthFrm.x = theClient.x;
	theSouthFrm.y = theClient.y + theClient.h;
	theSouthFrm.w = theClient.w;
	theSouthFrm.h = theFrameWidth;
	//
	theSouthWestFrm.x = theClient.x - theFrameWidth;
	theSouthWestFrm.y = theClient.y + theClient.h;
	theSouthWestFrm.w = theFrameWidth;
	theSouthWestFrm.h = theFrameWidth;
	//왼쪽
	theWestFrm.x = theClient.x - theFrameWidth;
	theWestFrm.y = theClient.y;
	theWestFrm.w = theFrameWidth;
	theWestFrm.h = theClient.h;
	//
	theNorthWestFrm.x = theClient.x - theFrameWidth;
	theNorthWestFrm.y = theClient.y - theFrameWidth;
	theNorthWestFrm.w = theFrameWidth;
	theNorthWestFrm.h = theFrameWidth;
}

//-----------------------------------------------------------------------------
int CHelpItem::GetAlignCenterx( SStringInfo* strInfo )
{
	int pos_x( 0 );
	float PosBoxCenterX( 0.0f );
	float PosFontCenterX( 0.0f );
	
	//무조건...문자열이 제대로 들어온다고 보고 처리....
	/*
	if( ( TextLength == 0 ) || ( strInfo.Width >= MAX_ITEMTIP_WIDTH ) )
	{	
	pos_x = 0;
	}
	//*/
	
    PosBoxCenterX = (float)( MAX_ITEMTIP_WIDTH / 2 );
    PosFontCenterX = (float)( strInfo->Width / 2 );
	
    pos_x	= (int)( PosBoxCenterX - PosFontCenterX );
	//pos_x	+= 0;
    
    return( pos_x );
}

//-----------------------------------------------------------------------------
void CHelpItem::RenderProc()
{
	//	Render->Draw( TID_CLIENT, theClient.x, theClient.y, theClient.w, theClient.h, D3DCOLOR_ARGB( 195, 113, 119, 97 ) );	
	Render->Draw( TID_CLIENT, theClient.x, theClient.y, theClient.w, theClient.h, D3DCOLOR_ARGB( 225, 57, 61, 72 ) );	
	
	Render->Draw( TID_Tooltip2_Img, theNorthFrm.x, theNorthFrm.y, theNorthFrm.w, theNorthFrm.h, D3DCOLOR_ARGB( 225, 220, 220, 255 ) );
	Render->Draw( TID_Tooltip2_Img, theNorthEastFrm.x, theNorthEastFrm.y, theNorthEastFrm.w, theNorthEastFrm.h, D3DCOLOR_ARGB( 225, 220, 220, 255 ) );
	Render->Draw( TID_Tooltip2_Img, theEastFrm.x, theEastFrm.y, theEastFrm.w, theEastFrm.h, D3DCOLOR_ARGB( 225, 220, 220, 255 ) );
	Render->Draw( TID_Tooltip2_Img, theSouthEastFrm.x, theSouthEastFrm.y, theSouthEastFrm.w, theSouthEastFrm.h, D3DCOLOR_ARGB( 225, 220, 220, 255 ) );
	Render->Draw( TID_Tooltip2_Img, theSouthFrm.x, theSouthFrm.y, theSouthFrm.w, theSouthFrm.h, D3DCOLOR_ARGB( 225, 220, 220, 255 ) );
	Render->Draw( TID_Tooltip2_Img, theSouthWestFrm.x, theSouthWestFrm.y, theSouthWestFrm.w, theSouthWestFrm.h, D3DCOLOR_ARGB( 225, 220, 220, 255 ) );
	Render->Draw( TID_Tooltip2_Img, theWestFrm.x, theWestFrm.y, theWestFrm.w, theWestFrm.h, D3DCOLOR_ARGB( 225, 220, 220, 255 ) );
	Render->Draw( TID_Tooltip2_Img, theNorthWestFrm.x, theNorthWestFrm.y, theNorthWestFrm.w, theNorthWestFrm.h, D3DCOLOR_ARGB( 225, 220, 220, 255 ) );
	
    switch( theTipType )
	{
	case n_TIP_ITEM_WEAPON:	DrawTipWeapon();	break;
	case n_TIP_ITEM_WEAR:	DrawTipWear();		break;
	case n_TIP_ITEM_ETC:	DrawTipEtc();		break;
	case n_TIP_SKILL:		DrawTipSkill();		break;
	}
}

void CHelpItem::InitExtraInfo()
{
	memset( &theAppEldTemp, 0, sizeof( STipString ) );
	memset( &theAppEldInfo, 0, sizeof( STipString ) );	
	memset( &theAppMixEldTemp, 0, sizeof( STipString ) );	
	memset( &theAppMixEldInfo, 0, sizeof( STipString ) );	



}

//-----------------------------------------------------------------------------
void CHelpItem::InitWeapon()
{
	WeaponNumLine = 0;
	
	memset( &theWeaponName, 0, sizeof( STipString ) );
	memset( &theWeaponGrade, 0, sizeof( STipString ) );
	memset( &theWeaponNeed, 0, sizeof( STipString ) );
	
	memset( theWeaponDesc, '\0', sizeof( STipString ) * 10 );
	
	memset( &theWeaponAttack, 0, sizeof( STipString ) );
	memset( &theWeaponMagic, 0, sizeof( STipString ) );
	memset( &theWeaponAttackAttri, 0, sizeof( STipString ) );
	memset( &theWeaponType, 0, sizeof( STipString ) );
	memset( &theWeaponproperty, 0, sizeof( STipString ) );
	
	memset( &theWeaponAttackSpeed, 0, sizeof( STipString ) );
	memset( &theWeaponMagicSpeed, 0, sizeof( STipString ) );
	memset( &theWeaponSkillName, 0, sizeof( STipString ) );
	memset( &theWeaponAddEffect1, 0, sizeof( STipString ) );
	memset( &theWeaponAddEffect2, 0, sizeof( STipString ) );
	memset( &theWeaponEBW, 0, sizeof( STipString ) );
	
	memset( &theWeaponPrice, 0, sizeof( STipString ) );	
	memset( &theNeedFame , 0 , sizeof(STipString));
	
	theWeaponGrade.color = D3DCOLOR_XRGB( 100, 255, 100 );
	theWeaponNeed.color = D3DCOLOR_XRGB( 100, 255, 100 );	
    for( int i = 0; i < 10; ++i )
	{
		theWeaponDesc[i].color = theTColor;
	}
	theWeaponAttack.color = D3DCOLOR_XRGB( 255, 150, 150 );
	theWeaponMagic.color = D3DCOLOR_XRGB( 255, 150, 150 );
    theWeaponAttackAttri.color = D3DCOLOR_XRGB( 255, 150, 150 );
	theWeaponType.color = theTColor;
	theWeaponproperty.color = theTColor;
	theWeaponAttackSpeed.color = D3DCOLOR_XRGB( 255, 255, 100 );
	theWeaponMagicSpeed.color = D3DCOLOR_XRGB( 255, 255, 100 );
	theWeaponSkillName.color = theTColor;
	
	theNeedFame.color = theTColor;
	
	theWeaponAddEffect1.color = D3DCOLOR_XRGB( 240, 120, 142 );
	theWeaponAddEffect2.color = D3DCOLOR_XRGB( 240, 120, 142 );
	
	theWeaponEBW.color = D3DCOLOR_XRGB( 100, 255, 100 );
	theWeaponPrice.color = D3DCOLOR_XRGB( 255, 255, 100 );
}

//-----------------------------------------------------------------------------
void CHelpItem::InitWear()
{
	WearNumLine = 0;
	
	memset( &theWearName, 0, sizeof( STipString ) );
	memset( &theWearGrade, 0, sizeof( STipString ) );
	memset( &theWearNeed, 0, sizeof( STipString ) );
	
	memset( theWearDesc, '\0', sizeof( STipString ) * 10 );
	
	memset( &theWearAttackDef, 0, sizeof( STipString ) );
	memset( &theWearMagicRes, 0, sizeof( STipString ) );
	memset( &theWearMentalRes, 0, sizeof( STipString ) );
	memset( &theWearCurseRes, 0, sizeof( STipString ) );
	memset( &theWearVitalRes, 0, sizeof( STipString ) );
	
	memset( &theWearDefensAttri, 0, sizeof( STipString ) );
	memset( &theWearPoint, 0, sizeof( STipString ) );
	memset( &theAccAddEffect, 0, sizeof( STipString ) );	///-- 장신구 부가효과

	memset( &theWearproperty, 0, sizeof( STipString ) );
	memset( &theWearAttackEvade, 0, sizeof( STipString ) );
	memset( &theWearSkillName, 0, sizeof( STipString ) );
	
	memset( &theWearAddEffect1, 0, sizeof( STipString ) );
	memset( &theWearAddEffect2, 0, sizeof( STipString ) );
	
	memset( &theWearEBW, 0, sizeof( STipString ) );
	
	memset( &theWearPrice, 0, sizeof( STipString ) );
	
	memset(&theWearSetItem, 0 , sizeof(STipString)*4 );
	
	
    theWearGrade.color = D3DCOLOR_XRGB( 100, 255, 100 );
	theWearNeed.color = D3DCOLOR_XRGB( 100, 255, 100 );
	for( int i = 0; i < 10; ++i )
	{
		theWearDesc[i].color = theTColor;
	}
	theWearAttackDef.color = D3DCOLOR_XRGB( 150, 150, 255 );
	theWearMagicRes.color = D3DCOLOR_XRGB( 150, 150, 255 );
	theWearMentalRes.color = D3DCOLOR_XRGB( 150, 150, 255 );
	theWearCurseRes.color = D3DCOLOR_XRGB( 150, 150, 255 );
	theWearVitalRes.color = D3DCOLOR_XRGB( 150, 150, 255 );
	
	theWearDefensAttri.color = theTColor;
	theWearPoint.color = theTColor;
	theAccAddEffect.color = D3DCOLOR_XRGB( 100, 255, 100 );
	theWearproperty.color = theTColor;
	theWearAttackEvade.color = D3DCOLOR_XRGB( 255, 255, 100 );
	theWearSkillName.color = theTColor;
	
	theWearAddEffect1.color = D3DCOLOR_XRGB( 240, 120, 142 );
	theWearAddEffect2.color = D3DCOLOR_XRGB( 240, 120, 142 );
	
	theWearEBW.color = D3DCOLOR_XRGB( 100, 255, 100 );
	theWearPrice.color = D3DCOLOR_XRGB( 255, 255, 100 );	
	
	for(short i = 0; i < 4; ++i )
	{
		theWearSetItem[i].color = D3DCOLOR_XRGB( 150, 150, 150);;
	}
}

//-----------------------------------------------------------------------------
void CHelpItem::InitEtc()
{
	EtcNumLine = 0;
	
	memset( &theEtcName, 0, sizeof( STipString ) );
	memset( &theEtcGrade, 0, sizeof( STipString ) );
	memset( &theEtcNeed, 0, sizeof( STipString ) );
	
	memset( theEtcDesc, '\0', sizeof( STipString ) * 10 );
	memset( &theEtcBWP, 0, sizeof( STipString ) );
	memset( &theEtcTime, 0, sizeof( STipString ) );

	memset( &theEtcPrice, 0, sizeof( STipString ) );
	
	theEtcGrade.color = D3DCOLOR_XRGB( 100, 255, 100 );
	theEtcNeed.color = D3DCOLOR_XRGB( 100, 255, 100 );		
	theEtcTime.color = D3DCOLOR_XRGB( 100, 255, 100 );

	for( int i = 0; i < 10; ++i )
	{
		theEtcDesc[i].color = theTColor;
	}
	theEtcBWP.color = theTColor;
	theEtcPrice.color = D3DCOLOR_XRGB( 255, 255, 100 );
}

//-----------------------------------------------------------------------------
void CHelpItem::InitSkill()
{
	SkillNumLine = 0;
	
	memset( &theSkillName, 0, sizeof( STipString ) );
	memset( &theSkillNeed, 0, sizeof( STipString ) );
	memset( &theSkillToggle, 0, sizeof( STipString ) );
	
	memset( theSkillDesc, '\0', sizeof( STipString ) * 10 );	
	
	memset( &theSkillItem, 0, sizeof( STipString ) );
	memset( &theSkillGage, 0, sizeof( STipString ) );
	memset( &theSkillWastePoint, 0, sizeof( STipString ) );
	memset( &theSkillKeepupTime, 0, sizeof( STipString ) );
	memset( &theSkillProimaType, 0, sizeof( STipString ) );
	memset( &theSkillUpLevel, 0, sizeof( STipString ) );
	
	memset( &theNeedPoint, 0, sizeof( STipString ) );
	
	memset( &theDelay , 0 , sizeof( STipString)) ; 
	memset( &theNeedDelay  , 0 , sizeof( STipString));	
	memset( &theEffect[0]  , 0 , sizeof( STipString));
	memset( &theEffect[1]  , 0 , sizeof( STipString));
	memset( &theEffect[2]  , 0 , sizeof( STipString));
	memset ( &theAddEfect ,  0 , sizeof( STipString )) ;
	memset ( &theTarget ,  0 , sizeof( STipString ));
	
	
	theDelay.color  = theTColor;
	theNeedDelay.color = theTColor;
	theEffect[0].color = D3DCOLOR_XRGB( 100, 255, 100 ) ; 
	theEffect[1].color = D3DCOLOR_XRGB( 100, 255, 100 ) ; 
	theEffect[2].color = D3DCOLOR_XRGB( 100, 255, 100 ) ; 
	theAddEfect.color = D3DCOLOR_XRGB( 100, 255, 100 ) ; ; 
	
	theTarget.color = theTColor;
	
	
	theSkillName.color = theTColor;
	theSkillNeed.color = D3DCOLOR_XRGB( 100, 255, 100 );
	theSkillToggle.color = D3DCOLOR_XRGB( 100, 255, 100 );
	for( int i = 0; i < 10; ++i )
	{
		theSkillDesc[i].color = theTColor;
	}
	theSkillItem.color = D3DCOLOR_XRGB( 255, 255, 100 );
	theSkillGage.color = D3DCOLOR_XRGB( 255, 255, 100 );
	theSkillWastePoint.color = D3DCOLOR_XRGB( 255, 255, 100 );
	theSkillKeepupTime.color = theTColor;
	theSkillProimaType.color = theTColor;
	
	theSkillUpLevel.color = D3DCOLOR_XRGB( 255, 180, 180 );
	theNeedPoint.color = D3DCOLOR_XRGB( 255, 180, 180 );
	
}

void CHelpItem::SetItemAddEffectInfo(TCHAR * pStr , SItemAddEffectInfo * pItemAddEffectInfo)
{
	if(NULL == pItemAddEffectInfo)
		return;
	
	TCHAR strtemp[128] = {0,};
	
	switch(pItemAddEffectInfo->theAddEffectType)
	{
	case 1:
		{
			Rsprintf( RWCHAR(strtemp), G_STRING_CHAR( IDS_RISE_OF_MOVEVEOCITY ) , pItemAddEffectInfo->theParam[0]-100 );		
			Rstrcat( RWCHAR(pStr), RWCHAR(strtemp) );
		}
		break;
	case 2:
		{
			Rsprintf( RWCHAR(strtemp), G_STRING_CHAR( IDS_RECOVER_HP_2SECOND ) , pItemAddEffectInfo->theParam[0]);		
			Rstrcat( RWCHAR(pStr), RWCHAR(strtemp) );
		}
		break;
	case 3:
		{
			Rsprintf( RWCHAR(strtemp), G_STRING_CHAR( IDS_RECOVER_SP_2SECOND ) , pItemAddEffectInfo->theParam[0]);		
			Rstrcat( RWCHAR(pStr), RWCHAR(strtemp) );
		}
		break;
	case 4:
		{
			Rsprintf( RWCHAR(strtemp), G_STRING_CHAR( IDS_RECOVER_MP_2SECOND ) , pItemAddEffectInfo->theParam[0]);		
			Rstrcat( RWCHAR(pStr), RWCHAR(strtemp) );
		}
		break;
	case 5:
		{
			if(1 == pItemAddEffectInfo->theParam[0])
			{
				Rsprintf( RWCHAR(strtemp), G_STRING_CHAR( IDS_RISE_MAX_HP ) , pItemAddEffectInfo->theParam[1]);			
				Rstrcat( RWCHAR(pStr), RWCHAR(strtemp) );
			}
			
			if(2 == pItemAddEffectInfo->theParam[0])
			{
				Rsprintf( RWCHAR(strtemp), G_STRING_CHAR( IDS_RISE_MAX_HP_PERCENT ) , pItemAddEffectInfo->theParam[1] -100);		
				Rstrcat( RWCHAR(pStr), RWCHAR(strtemp) );
			}
		}
		break;
	case 6:
		{
			if(1 == pItemAddEffectInfo->theParam[0])
			{
				Rsprintf( RWCHAR(strtemp), G_STRING_CHAR( IDS_RISE_MAX_SP ) , pItemAddEffectInfo->theParam[1]);			
				Rstrcat( RWCHAR(pStr), RWCHAR(strtemp) );
			}
			
			if(2 == pItemAddEffectInfo->theParam[0])
			{
				Rsprintf( RWCHAR(strtemp), G_STRING_CHAR( IDS_RISE_MAX_SP_PERCENT ) , pItemAddEffectInfo->theParam[1] - 100);		
				Rstrcat( RWCHAR(pStr), RWCHAR(strtemp) );
			}
		}
		break;
	case 7:
		{
			if(1 == pItemAddEffectInfo->theParam[0])
			{
				Rsprintf( RWCHAR(strtemp), G_STRING_CHAR( IDS_RISE_MAX_MP ), pItemAddEffectInfo->theParam[1]);			
				Rstrcat( RWCHAR(pStr), RWCHAR(strtemp) );
			}
			
			if(2 == pItemAddEffectInfo->theParam[0])
			{
				Rsprintf( RWCHAR(strtemp), G_STRING_CHAR( IDS_RISE_MAX_MP_PERCENT ), pItemAddEffectInfo->theParam[1] -100);		
				Rstrcat( RWCHAR(pStr), RWCHAR(strtemp) );
			}
		}
		break;
	case 8:
		{
			float fValue = (float)(pItemAddEffectInfo->theParam[0] - 100.0f);
			
			Rsprintf( RWCHAR(strtemp), G_STRING_CHAR( IDS_ENCHANT_DEATH ) , (int)fValue);		
			Rstrcat( RWCHAR(pStr), RWCHAR(strtemp) );
		}
		break;
		//		case 9:
		//			{
		//				Rsprintf( RWCHAR(strtemp), "SP,MP 회복 2초당 %d씩" , pItemAddEffectInfo->theParam[0] );		
		//				Rstrcat( RWCHAR(pStr), RWCHAR(strtemp) );
		//			}
		
		
	case 10:
		{
			if(2 == pItemAddEffectInfo->theParam[0])
			{
				Rsprintf( RWCHAR(strtemp), G_STRING_CHAR( IDS_HELP_PHY_ATK_INC ), pItemAddEffectInfo->theParam[1] -100);		
				Rstrcat( RWCHAR(pStr), RWCHAR(strtemp) );
			}
			
		}
		break;
	case 11:
		{
			
			if(2 == pItemAddEffectInfo->theParam[0])
			{
				Rsprintf( RWCHAR(strtemp), G_STRING_CHAR( IDS_HELP_PHY_DEF_INC ), pItemAddEffectInfo->theParam[1] -100);		
				Rstrcat( RWCHAR(pStr), RWCHAR(strtemp) );
			}
			
		}
		break;
	case 12:
		{
			
			if(2 == pItemAddEffectInfo->theParam[0])
			{
				Rsprintf( RWCHAR(strtemp), G_STRING_CHAR( IDS_HELP_MAG_ATK_INC ), pItemAddEffectInfo->theParam[1] -100);		
				Rstrcat( RWCHAR(pStr), RWCHAR(strtemp) );
			}
			
		}
		break;
	case 13:
		{
			
			if(2 == pItemAddEffectInfo->theParam[0])
			{
				Rsprintf( RWCHAR(strtemp),G_STRING_CHAR( IDS_HELP_MAG_DEF_INC ), pItemAddEffectInfo->theParam[1] -100);		
				Rstrcat( RWCHAR(pStr), RWCHAR(strtemp) );
			}
			
		}
		
		break;
		
		
	}
}

//-----------------------------------------------------------------------------
void CHelpItem::SetTipWeapon()
{
	bool blank_a = false;
	bool blank_b = false;
	
	
	theTipType = n_TIP_ITEM_WEAPON;
	
	int ClWidth = 0;
	int nMaxStrWidthNum = 28;
	
	theClient.h = BV;
	//@@@@ char	theName[20];		// 아이템명....
	Rsprintf( RWCHAR(theWeaponName.str), _RT("%s"), RWCHAR(thePcItem->ItemTable->theName) );
	
	if(theFontg)
		theFontg->GetStringInfo( theWeaponName.str, &theStrInfo );
	if( ClWidth < theStrInfo.Width )
		ClWidth = theStrInfo.Width;	
	if( nMaxStrWidthNum < theStrInfo.nCLen )
		nMaxStrWidthNum = theStrInfo.nCLen;
	
	theWeaponName.x = 5;
	//theWeaponName.x = GetAlignCenterx( &theStrInfo );
	theWeaponName.y = theClient.h;
	//by simwoosung	
	SItemBaseInfo * pEldaInfo = NULL;
	if(thePcItem->WearTable.theCode) //추가정보가 있을경우 
	{
		pEldaInfo = SetTipApplyEldastone( theWeaponName.x, theWeaponName.y, 
			(int)theStrInfo.Width, ClWidth ); //적용엘다스톤 정보
	}
	theClient.h += RV;
	
	//-------------------------------------------------------------------------	
	theClient.h += BV;
	//@@@@ 장비품 레벨 -- theGrade
	Rsprintf(RWCHAR(theWeaponGrade.str), _RT("%s %s (%d)"), G_STRING_CHAR( IDS_ITEM_EQUIP ), G_STRING_CHAR( IDS_CHARSTATE_LEVEL ), thePcItem->ItemTable->theGrade / 100 );    
	theWeaponGrade.color = D3DCOLOR_XRGB( 100, 255, 100 );
	
	if(theFontg)
		theFontg->GetStringInfo( theWeaponGrade.str, &theStrInfo );
	if( ClWidth < theStrInfo.Width )
		ClWidth = theStrInfo.Width;
	if( nMaxStrWidthNum < theStrInfo.nCLen )
		nMaxStrWidthNum = theStrInfo.nCLen;
	
	theWeaponGrade.x = 5;
	//theWeaponGrade.x = GetAlignCenterx( &theStrInfo );
	theWeaponGrade.y = theClient.h;
	theClient.h += RV;
	//-----------------------------------------------------------------------------		
	if( (thePcItem->ItemTable->theNeedSkill) || (thePcItem->ItemTable->theLimitLevel) )
	{	
		//@@@@ 장비품 필요 스킬, 레벨 -- theNeed
		TCHAR NeedSkill[256] = {0,};
		TCHAR NeedLevel[256] = {0,};
		
		if(thePcItem->ItemTable->theNeedSkill)
		{
			SSkillBaseInfo *pSkill_Info = NULL;
			pSkill_Info = g_Pc_Manager.GetSkillBaseInfoPtr((DWORD)thePcItem->ItemTable->theNeedSkill);
			if(pSkill_Info)
			{
				Rsprintf( RWCHAR(NeedSkill), _RT("%s (%s) "), G_STRING_CHAR( IDS_WND_NEED_SKILL ), RWCHAR(pSkill_Info->theSkillName) );
				Rstrcat( RWCHAR(theWeaponNeed.str), RWCHAR(NeedSkill) );
			}		
		}
		
		if(thePcItem->ItemTable->theLimitLevel)
		{
			Rsprintf( RWCHAR(NeedSkill), _RT("%s (%d) "), G_STRING_CHAR( IDS_WND_NEED_LEVEL ), thePcItem->ItemTable->theLimitLevel );		
			Rstrcat( RWCHAR(theWeaponNeed.str), RWCHAR(NeedSkill) );					
		}		
		
		if(theFontg)
			theFontg->GetStringInfo( theWeaponNeed.str, &theStrInfo );
		if( ClWidth < theStrInfo.Width )
			ClWidth = theStrInfo.Width;
		if( nMaxStrWidthNum < theStrInfo.nCLen )
			nMaxStrWidthNum = theStrInfo.nCLen;
		
		theWeaponNeed.x = 5;
		//theWeaponNeed.x = GetAlignCenterx( &theStrInfo );
		theWeaponNeed.y = theClient.h;
		theClient.h += RV;		
	}	
	//-------------------------------------------------------------------------	
	
	theClient.h += BV;
	
	//@@@@ char	theDesc[100];		// 설명
	/*int NumStr		= theFontg->GetStringCount( thePcItem->ItemTable->theDesc );
	WeaponNumLine	= GetNumLine( NumStr, 20 );
	DescStartpos	= 0;	
	
	  for( int i = 0; i < WeaponNumLine; ++i )
	  {
	  if(theFontg)
	  theFontg->GetStringDistribute( theWeaponDesc[i].str, &thePcItem->ItemTable->theDesc[DescStartpos] );
	  
		DescStartpos += _tcslen( theWeaponDesc[i].str );
		
		  if(theFontg)
		  theFontg->GetStringInfo( theWeaponDesc[i].str, &theStrInfo );
		  if( ClWidth < theStrInfo.Width )
		  ClWidth = theStrInfo.Width;
		  theWeaponDesc[i].x = 5;
		  //theWeaponDesc[i].x = GetAlignCenterx( &theStrInfo );
		  theWeaponDesc[i].y = theClient.h;
		  theClient.h += RV;		
		  }
	//-------------------------------------------------------------------------*/	
	//@@@@ BYTE	theMinAttackBonus;	// 추가 물리 공격력 범위
	//@@@@ BYTE	theMaxAttackBonus;		
	//@@@@ BYTE	theMinMagicBonus;	// 추가 마법 공격력 범위 
	//@@@@ BYTE	theMaxMagicBonus;
	
	int theMinAttackBonus = thePcItem->ItemTable->theMinAttackBonus;
	int theMaxAttackBonus = thePcItem->ItemTable->theMaxAttackBonus;
	if( pEldaInfo )
	{
		theMinAttackBonus += pEldaInfo->theMinAttackBonus;
		theMaxAttackBonus += pEldaInfo->theMaxAttackBonus;
	}		
	if( (theMinAttackBonus != 0) || (theMinAttackBonus != 0) )
	{
		if( thePcItem->Code >= c_NotRepairitem_Start &&  
			(thePcItem->ItemTable->theClass == n_Class_Weapon || thePcItem->ItemTable->theClass == n_Class_Accessory ) &&
			thePcItem->ItemTable->theType != n_Type_ShortWeapon ) 
		{
			Rsprintf( RWCHAR(theWeaponAttack.str), _RT("%s %d - %d"), G_STRING_CHAR( IDS_CHARSTATE_PHYATT ), theMinAttackBonus, theMaxAttackBonus );
		}
		else
		{
			///-- 내구력에 따른 공격력 감소
			if( thePcItem->ItemTable->theMaxEndurance && 
				thePcItem->WearTable.theEndurance * 100 / thePcItem->ItemTable->theMaxEndurance <= 50 )
			{
				int li_pen_min = theMinAttackBonus * thePcItem->WearTable.theEndurance * 2 / thePcItem->ItemTable->theMaxEndurance ;
				int li_pen_max = theMaxAttackBonus * thePcItem->WearTable.theEndurance * 2 / thePcItem->ItemTable->theMaxEndurance;
				Rsprintf( RWCHAR(theWeaponAttack.str), _RT("%s %d - %d"), G_STRING_CHAR( IDS_CHARSTATE_PHYATT ), li_pen_min, li_pen_max );
			}
			else
			{
				Rsprintf( RWCHAR(theWeaponAttack.str), _RT("%s %d - %d"), G_STRING_CHAR( IDS_CHARSTATE_PHYATT ), theMinAttackBonus, theMaxAttackBonus );
			}
		}
		
		if(theFontg)
			theFontg->GetStringInfo( theWeaponAttack.str, &theStrInfo );
		if( ClWidth < theStrInfo.Width )
			ClWidth = theStrInfo.Width;
		if( nMaxStrWidthNum < theStrInfo.nCLen )
			nMaxStrWidthNum = theStrInfo.nCLen;
		
		theWeaponAttack.x = 5;
		//theWeaponAttack.x = GetAlignCenterx( &theStrInfo );
		theWeaponAttack.y = theClient.h;
		theClient.h += RV;
		//-------------------------------------------------------------------------
	}	
	int theMinMagicBonus = thePcItem->ItemTable->theMinMagicBonus;
	int theMaxMagicBonus = thePcItem->ItemTable->theMaxMagicBonus;
	if( pEldaInfo )
	{
		theMinMagicBonus += pEldaInfo->theMinMagicBonus;
		theMaxMagicBonus += pEldaInfo->theMaxMagicBonus;		
	}
	if( (theMinMagicBonus != 0) || (theMaxMagicBonus != 0) )
	{
		if( thePcItem->Code >= c_NotRepairitem_Start &&  
			(thePcItem->ItemTable->theClass == n_Class_Weapon || thePcItem->ItemTable->theClass == n_Class_Accessory ) &&
			thePcItem->ItemTable->theType != n_Type_ShortWeapon ) 
			
		{
			Rsprintf( RWCHAR(theWeaponMagic.str), _RT("%s %d - %d"), G_STRING_CHAR( IDS_CHARSTATE_MAGATT ), theMinMagicBonus, theMaxMagicBonus );
		}
		else
		{
			///-- 내구력에 따른 마법 공격력 감소
			if( thePcItem->ItemTable->theMaxEndurance && 
				thePcItem->WearTable.theEndurance * 100 / thePcItem->ItemTable->theMaxEndurance <= 50 )
			{
				int li_pen_min = theMinMagicBonus * thePcItem->WearTable.theEndurance * 2 / thePcItem->ItemTable->theMaxEndurance ;
				int li_pen_max = theMaxMagicBonus * thePcItem->WearTable.theEndurance * 2 / thePcItem->ItemTable->theMaxEndurance;
				Rsprintf( RWCHAR(theWeaponMagic.str), _RT("%s %d - %d"), G_STRING_CHAR( IDS_CHARSTATE_MAGATT ), li_pen_min, li_pen_max );
			}
			else
			{	
				Rsprintf( RWCHAR(theWeaponMagic.str), _RT("%s %d - %d"), G_STRING_CHAR( IDS_CHARSTATE_MAGATT ), theMinMagicBonus, theMaxMagicBonus );
			}
		}
		
		if(theFontg)
			theFontg->GetStringInfo( theWeaponMagic.str, &theStrInfo );
		if( ClWidth < theStrInfo.Width )
			ClWidth = theStrInfo.Width;
		if( nMaxStrWidthNum < theStrInfo.nCLen )
			nMaxStrWidthNum = theStrInfo.nCLen;
		
		theWeaponMagic.x = 5;
		//theWeaponMagic.x = GetAlignCenterx( &theStrInfo );
		theWeaponMagic.y = theClient.h;
		theClient.h += RV;
		//-------------------------------------------------------------------------	
	}	
	
	//@@@@ BYTE	theAttackAttri;		// 공격속성
	switch( thePcItem->ItemTable->theAttackAttri )
	{
	case 0:		Rsprintf( RWCHAR(theWeaponAttackAttri.str), _RT("%s:%s"), G_STRING_CHAR( IDS_ATTACK_ATTR ), G_STRING_CHAR( IDS_ATTR_NONE ) );	break;
	case 1:		Rsprintf( RWCHAR(theWeaponAttackAttri.str), _RT("%s:%s"), G_STRING_CHAR( IDS_ATTACK_ATTR ), G_STRING_CHAR( IDS_ATTR_FIRE ) );		break;
	case 2:		Rsprintf( RWCHAR(theWeaponAttackAttri.str), _RT("%s:%s"), G_STRING_CHAR( IDS_ATTACK_ATTR ), G_STRING_CHAR( IDS_ATTR_ICE ));		break;
	case 3:		Rsprintf( RWCHAR(theWeaponAttackAttri.str), _RT("%s:%s"), G_STRING_CHAR( IDS_ATTACK_ATTR ), G_STRING_CHAR( IDS_ATTR_EARTH ));	break;
	case 4:		Rsprintf( RWCHAR(theWeaponAttackAttri.str), _RT("%s:%s"), G_STRING_CHAR( IDS_ATTACK_ATTR ), G_STRING_CHAR( IDS_ATTR_LIGHTNING ));		break;
	case 5:		Rsprintf( RWCHAR(theWeaponAttackAttri.str), _RT("%s:%s"), G_STRING_CHAR( IDS_ATTACK_ATTR ), G_STRING_CHAR( IDS_ATTR_WATER ));		break;
	case 6:		Rsprintf( RWCHAR(theWeaponAttackAttri.str), _RT("%s:%s"), G_STRING_CHAR( IDS_ATTACK_ATTR ), G_STRING_CHAR( IDS_ATTR_NATURE ));	break;
	case 7:		Rsprintf( RWCHAR(theWeaponAttackAttri.str), _RT("%s:%s"), G_STRING_CHAR( IDS_ATTACK_ATTR ), G_STRING_CHAR( IDS_ATTR_HOLY ));	break;
	case 8:		Rsprintf( RWCHAR(theWeaponAttackAttri.str), _RT("%s:%s"), G_STRING_CHAR( IDS_ATTACK_ATTR ), G_STRING_CHAR( IDS_ATTR_DARK ));	break;
	case 9:		Rsprintf( RWCHAR(theWeaponAttackAttri.str), _RT("%s:%s"), G_STRING_CHAR( IDS_ATTACK_ATTR ), G_STRING_CHAR( IDS_ATTR_SPRIT ));	break;
	case 10:	Rsprintf( RWCHAR(theWeaponAttackAttri.str), _RT("%s:%s"), G_STRING_CHAR( IDS_ATTACK_ATTR ), G_STRING_CHAR( IDS_ATTR_CURSE ));	break;
	case 11:	Rsprintf( RWCHAR(theWeaponAttackAttri.str), _RT("%s:%s"), G_STRING_CHAR( IDS_ATTACK_ATTR ), G_STRING_CHAR( IDS_ATTR_BIONIC ));	break;
	case 12:	Rsprintf( RWCHAR(theWeaponAttackAttri.str), _RT("%s:-"),  G_STRING_CHAR( IDS_ATTACK_ATTR ));		break;
	}
	
	if(theFontg)
		theFontg->GetStringInfo( theWeaponAttackAttri.str, &theStrInfo );
	if( ClWidth < theStrInfo.Width )
		ClWidth = theStrInfo.Width;
	if( nMaxStrWidthNum < theStrInfo.nCLen )
		nMaxStrWidthNum = theStrInfo.nCLen;
	
	theWeaponAttackAttri.x = 5;
	//theWeaponAttackAttri.x = GetAlignCenterx( &theStrInfo );
	theWeaponAttackAttri.y = theClient.h;
	theClient.h += RV;
	//-------------------------------------------------------------------------
	
	theClient.h += BV;
	//@@@@ 검(1), 대검(2)만.... BYTE	theType;			// 제 2 분류
	char zattack[20] = "";
	char xattack[20] = "";
	char cattack[20] = "";
	if( ( thePcItem->ItemTable->theType == 1 ) || ( thePcItem->ItemTable->theType == 2 ) )
	{
		//@@@@ BYTE	theZAttack;			// Z버튼 공격 성능  0 - 6
		//@@@@ BYTE	theXAttack;			// X버튼 공격 성능
		//@@@@ BYTE	theCAttack;			// C버튼 공격 성능
		if( thePcItem->ItemTable->theZAttack == 0 )	{	Rsprintf(RWCHAR(zattack), _RT("%s(S)"), G_STRING_CHAR( IDS_HORIZON_SLASH ) );	}
		else if( thePcItem->ItemTable->theZAttack == 1 )	{	Rsprintf(RWCHAR(zattack), _RT("%s(A)"), G_STRING_CHAR( IDS_HORIZON_SLASH ) );	}
		else if( thePcItem->ItemTable->theZAttack == 2 )	{	Rsprintf(RWCHAR(zattack), _RT("%s(B)"), G_STRING_CHAR( IDS_HORIZON_SLASH ) );	}
		else if( thePcItem->ItemTable->theZAttack == 3 )	{	Rsprintf(RWCHAR(zattack), _RT("%s(C)"), G_STRING_CHAR( IDS_HORIZON_SLASH ) );	}
		else if( thePcItem->ItemTable->theZAttack == 4 )	{	Rsprintf(RWCHAR(zattack), _RT("%s(D)"), G_STRING_CHAR( IDS_HORIZON_SLASH ) );	}
		else if( thePcItem->ItemTable->theZAttack == 5 )	{	Rsprintf(RWCHAR(zattack), _RT("%s(E)"), G_STRING_CHAR( IDS_HORIZON_SLASH ) );	}
		else if( thePcItem->ItemTable->theZAttack == 6 )	{	Rsprintf(RWCHAR(zattack), _RT("%s(F)"), G_STRING_CHAR( IDS_HORIZON_SLASH ) );	}
		
		if( thePcItem->ItemTable->theXAttack == 0 )	{	Rsprintf(RWCHAR(xattack), _RT("%s(S)"), G_STRING_CHAR( IDS_VERTICAL_SLASH ) );	}
		else if( thePcItem->ItemTable->theXAttack == 1 )	{	Rsprintf(RWCHAR(xattack), _RT("%s(A)"), G_STRING_CHAR( IDS_VERTICAL_SLASH ) );	}
		else if( thePcItem->ItemTable->theXAttack == 2 )	{	Rsprintf(RWCHAR(xattack), _RT("%s(B)"), G_STRING_CHAR( IDS_VERTICAL_SLASH ) );	}
		else if( thePcItem->ItemTable->theXAttack == 3 )	{	Rsprintf(RWCHAR(xattack), _RT("%s(C)"), G_STRING_CHAR( IDS_VERTICAL_SLASH ) );	}
		else if( thePcItem->ItemTable->theXAttack == 4 )	{	Rsprintf(RWCHAR(xattack), _RT("%s(D)"), G_STRING_CHAR( IDS_VERTICAL_SLASH ) );	}
		else if( thePcItem->ItemTable->theXAttack == 5 )	{	Rsprintf(RWCHAR(xattack), _RT("%s(E)"), G_STRING_CHAR( IDS_VERTICAL_SLASH ) );	}
		else if( thePcItem->ItemTable->theXAttack == 6 )	{	Rsprintf(RWCHAR(xattack), _RT("%s(F)"), G_STRING_CHAR( IDS_VERTICAL_SLASH ) );	}
		
		if( thePcItem->ItemTable->theCAttack == 0 )	{	Rsprintf(RWCHAR(cattack), _RT("%s(S)"), G_STRING_CHAR( IDS_THRUST ) );	}
		else if( thePcItem->ItemTable->theCAttack == 1 )	{	Rsprintf(RWCHAR(cattack), _RT("%s(A)"), G_STRING_CHAR( IDS_THRUST ) );	}
		else if( thePcItem->ItemTable->theCAttack == 2 )	{	Rsprintf(RWCHAR(cattack), _RT("%s(B)"), G_STRING_CHAR( IDS_THRUST ) );	}
		else if( thePcItem->ItemTable->theCAttack == 3 )	{	Rsprintf(RWCHAR(cattack), _RT("%s(C)"), G_STRING_CHAR( IDS_THRUST ) );	}
		else if( thePcItem->ItemTable->theCAttack == 4 )	{	Rsprintf(RWCHAR(cattack), _RT("%s(D)"), G_STRING_CHAR( IDS_THRUST ) );	}
		else if( thePcItem->ItemTable->theCAttack == 5 )	{	Rsprintf(RWCHAR(cattack), _RT("%s(E)"), G_STRING_CHAR( IDS_THRUST ) );	}
		else if( thePcItem->ItemTable->theCAttack == 6 )	{	Rsprintf(RWCHAR(cattack), _RT("%s(F)"), G_STRING_CHAR( IDS_THRUST ) );	}
		
		Rsprintf( RWCHAR(theWeaponType.str), _RT("%s, %s, %s"), RWCHAR(zattack), RWCHAR(xattack), RWCHAR(cattack) );
		if(theFontg)
			theFontg->GetStringInfo( theWeaponType.str, &theStrInfo );
		if( ClWidth < theStrInfo.Width )
			ClWidth = theStrInfo.Width;
		if( nMaxStrWidthNum < theStrInfo.nCLen )
			nMaxStrWidthNum = theStrInfo.nCLen;
		
		theWeaponType.x = 5;
		//theWeaponType.x = GetAlignCenterx( &theStrInfo );
		theWeaponType.y = theClient.h;
		theClient.h += RV;
		//-------------------------------------------------------------------------
		
		theClient.h += BV;
	}
	
	SetItemProperty( pEldaInfo, theWeaponproperty );
	if( Rstrcmp( theWeaponproperty.str, _RT("")) != 0 )
	{
		if(theFontg)
			theFontg->GetStringInfo( theWeaponproperty.str, &theStrInfo );
		if( ClWidth < theStrInfo.Width )
			ClWidth = theStrInfo.Width;
		if( nMaxStrWidthNum < theStrInfo.nCLen )
			nMaxStrWidthNum = theStrInfo.nCLen;
		
		theWeaponproperty.x = 5;
		//theWeaponproperty.x = GetAlignCenterx( &theStrInfo );
		theWeaponproperty.y = theClient.h;
		theClient.h += RV;
		//-------------------------------------------------------------------------		
		blank_a = true;
	}	
	
	//@@@@ short	theAttackSpeed;		// 공격 속도 보너스 -50 - 50
	//@@@@ short	theAttackHit;		// 공격 명중 보너스
	//@@@@ short	theAttackEvade;		// 공격 회피 보너스
	char attackspeed[20] = "";
	char attackhit[20] = "";
	char attackevade[20] = "";
	if( ( thePcItem->ItemTable->theAttackSpeed != 0 ) ) // || ( thePcItem->ItemTable->theAttackHit != 0 ) || ( thePcItem->ItemTable->theAttackEvade != 0 ) )
	{
		if( thePcItem->ItemTable->theAttackSpeed != 0 )
		{	
			Rsprintf( RWCHAR(attackspeed), _RT("%s+%d "), G_STRING_CHAR( IDS_CHARSTATE_ATTSPEED ), thePcItem->ItemTable->theAttackSpeed );  
			Rstrcat( RWCHAR(theWeaponAttackSpeed.str), RWCHAR(attackspeed) );   }
		//		if( thePcItem->ItemTable->theAttackHit != 0 )
		//		{	_stprintf( attackhit, "명중+%d ", thePcItem->ItemTable->theAttackHit );  _tcscat( theWeaponAttackSpeed.str, attackhit );	}
		//		if( thePcItem->ItemTable->theAttackEvade != 0 )
		//		{	_stprintf( attackevade, "회피+%d ", thePcItem->ItemTable->theAttackEvade );  _tcscat( theWeaponAttackSpeed.str, attackevade );	}
		
		//_stprintf( theWeaponAttackSpeed.str, "%s %s %s", attackspeed, attackhit, attackevade );
		if(theFontg)
			theFontg->GetStringInfo( theWeaponAttackSpeed.str, &theStrInfo );
		if( ClWidth < theStrInfo.Width )
			ClWidth = theStrInfo.Width;
		if( nMaxStrWidthNum < theStrInfo.nCLen )
			nMaxStrWidthNum = theStrInfo.nCLen;
		
		theWeaponAttackSpeed.x = 5;
		//theWeaponAttackSpeed.x = GetAlignCenterx( &theStrInfo );
		theWeaponAttackSpeed.y = theClient.h;
		theClient.h += RV;
		//-------------------------------------------------------------------------
		
		blank_a = true;
	}
	
	
	//@@@@ short	theMagicSpeed;		// 마법 시전속도 보너스
	//@@@@ short	theMagicHit;		// 마법 성공률 보너스
	/*		char magicspeed[20] = "";
	char magichit[20] = "";
	if( ( thePcItem->ItemTable->theMagicSpeed != 0 ) || ( thePcItem->ItemTable->theMagicHit != 0 ) )
	{
	if( thePcItem->ItemTable->theMagicSpeed != 0 )
	{	_stprintf( magicspeed, "!속도+%d ", thePcItem->ItemTable->theMagicSpeed );  _tcscat( theWeaponMagicSpeed.str, magicspeed );   }
	if( thePcItem->ItemTable->theMagicHit != 0 )
	{	_stprintf( magichit, "!명중+%d ", thePcItem->ItemTable->theMagicHit );  _tcscat( theWeaponMagicSpeed.str, magichit );   }
	
	  //_stprintf( theWeaponMagicSpeed.str, "%s %s", magicspeed, magichit );
	  if(theFontg)
	  theFontg->GetStringInfo( theWeaponMagicSpeed.str, &theStrInfo );
	  if( ClWidth < theStrInfo.Width )
	  ClWidth = theStrInfo.Width;
	  theWeaponMagicSpeed.x = 5;
	  //theWeaponMagicSpeed.x = GetAlignCenterx( &theStrInfo );
	  theWeaponMagicSpeed.y = theClient.h;
	  theClient.h += RV;
	  //-------------------------------------------------------------------------
	  
		blank_a = true;
		}
	*/		
	
	if( blank_a ){	theClient.h += BV;	}
	//@@@@ WORD	theOwnSkillCode;	// 고유 스킬
	if( thePcItem->ItemTable->theOwnSkillCode != 0 )
	{
		SSkillBaseInfo* skilltable = g_Pc_Manager.GetSkillBaseInfoPtr( thePcItem->ItemTable->theOwnSkillCode );
		if( skilltable )
		{
			Rsprintf( RWCHAR(theWeaponSkillName.str), _RT("%s:%s"), G_STRING_CHAR( IDS_SKILL_NAME ), RWCHAR(skilltable->theSkillName) );
			if(theFontg)
				theFontg->GetStringInfo( theWeaponSkillName.str, &theStrInfo );
			if( ClWidth < theStrInfo.Width )
				ClWidth = theStrInfo.Width;
			if( nMaxStrWidthNum < theStrInfo.nCLen )
				nMaxStrWidthNum = theStrInfo.nCLen;
			
			theWeaponSkillName.x = 5;
			//theWeaponSkillName.x = GetAlignCenterx( &theStrInfo );
			theWeaponSkillName.y = theClient.h;
			theClient.h += RV;
			//-------------------------------------------------------------------------
			blank_b = true;
		}
	}
	
	//@@@@ BYTE	theAddEffect1;		// 장비효과 1,2 
	//@@@@ BYTE	theAddEffect2;
	if( thePcItem->ItemTable->theAddEffect1 != 0 ) 
	{
		SItemAddEffectInfo * pItemAddEffectInfo = NULL;
		pItemAddEffectInfo = g_Pc_Manager.GetItemAddEffectInfo(thePcItem->ItemTable->theAddEffect1);
		
		if(pItemAddEffectInfo && pItemAddEffectInfo->theAddEffectType)
		{
			Rsprintf( RWCHAR(theWeaponAddEffect1.str), _RT("%s: "), G_STRING_CHAR( IDS_EQUIPMENT_EFFECT ));								
			SetItemAddEffectInfo(theWeaponAddEffect1.str, pItemAddEffectInfo);
			
			if(theFontg)
				theFontg->GetStringInfo( theWeaponAddEffect1.str, &theStrInfo );
			if( ClWidth < theStrInfo.Width )
				ClWidth = theStrInfo.Width;
			if( nMaxStrWidthNum < theStrInfo.nCLen )
				nMaxStrWidthNum = theStrInfo.nCLen;
			
			theWeaponAddEffect1.x = 5;
			//theWeaponAddEffect.x = GetAlignCenterx( &theStrInfo );
			theWeaponAddEffect1.y = theClient.h;
			theClient.h += RV;
			//-------------------------------------------------------------------------		
			blank_b = true;		
		}		
	}	
	if( thePcItem->ItemTable->theAddEffect2 != 0 ) 
	{
		SItemAddEffectInfo * pItemAddEffectInfo = NULL;
		pItemAddEffectInfo = g_Pc_Manager.GetItemAddEffectInfo(thePcItem->ItemTable->theAddEffect2);
		
		if(pItemAddEffectInfo && pItemAddEffectInfo->theAddEffectType)
		{
			Rsprintf( RWCHAR(theWeaponAddEffect2.str), _RT("%s: "), G_STRING_CHAR( IDS_EQUIPMENT_EFFECT ));								
			SetItemAddEffectInfo(theWeaponAddEffect2.str, pItemAddEffectInfo);
			
			if(theFontg)
				theFontg->GetStringInfo( theWeaponAddEffect2.str, &theStrInfo );
			if( ClWidth < theStrInfo.Width )
				ClWidth = theStrInfo.Width;
			if( nMaxStrWidthNum < theStrInfo.nCLen )
				nMaxStrWidthNum = theStrInfo.nCLen;
			
			theWeaponAddEffect2.x = 5;
			//theWeaponAddEffect.x = GetAlignCenterx( &theStrInfo );
			theWeaponAddEffect2.y = theClient.h;
			theClient.h += RV;
			//-------------------------------------------------------------------------		
			blank_b = true;		
		}		
	}
	
	if( blank_b ){	theClient.h += BV;	}
	//@@@@ WORD	theMaxEndurance;	// 최대 내구력
	//@@@@ BYTE	theBundle;			// 번들가능 갯수, 1=번들불가
	//@@@@ BYTE	theWeight;			// 무게
	//_stprintf( theWeaponEBW.str, " 내구력:%d, 무게:%d, 가격:%dEld", thePcItem->ItemTable->theMaxEndurance, thePcItem->ItemTable->theWeight, thePcItem->ItemTable->thePrice );
	//		_stprintf( theWeaponEBW.str, "내구력:%d 무게:%d", thePcItem->ItemTable->theMaxEndurance, thePcItem->ItemTable->theWeight);
	
#ifdef C_TIME_SUIT_UPDATE
	//시간제 아이템 조건  시간이 0 이고  
	if(thePcItem->ItemTable->theMaxEndurance == 0  && thePcItem->ItemTable->theRare == 255 )
	{	
		g_ListTime.GetTimeRemainStr(thePcItem->ItemUID , theWeaponEBW.str );	
		//남은 시간 을 표시한다.  일 시 분 까지 표기한다. 일단은 분으로 표기하자 .. 
		
	}
	else
	{
		Rsprintf( RWCHAR(theWeaponEBW.str), _RT("%s: %d / %d"), G_STRING_CHAR( IDS_CHARSTATE_ENDURANCE ), thePcItem->WearTable.theEndurance, thePcItem->ItemTable->theMaxEndurance );
	}
	
#else
	Rsprintf( RWCHAR(theWeaponEBW.str), _RT("%s: %d / %d"), G_STRING_CHAR( IDS_CHARSTATE_ENDURANCE ), thePcItem->WearTable.theEndurance, thePcItem->ItemTable->theMaxEndurance );
#endif
	
	if(theFontg)
		theFontg->GetStringInfo( theWeaponEBW.str, &theStrInfo );
	if( ClWidth < theStrInfo.Width )
		ClWidth = theStrInfo.Width;
	if( nMaxStrWidthNum < theStrInfo.nCLen )
		nMaxStrWidthNum = theStrInfo.nCLen;
	
	theWeaponEBW.x = 5;
	//theWeaponEBW.x = GetAlignCenterx( &theStrInfo );
	theWeaponEBW.y = theClient.h;
	theClient.h += RV;
	//-------------------------------------------------------------------------
	char StrEld[256]="";
	if( ( StoreWnd.theDCRate > 0 ) && ( ((CSlot*)theSlotWnd)->Group == n_SGROUP_STORE ) )
	{
		
		int price = thePcItem->ItemTable->thePrice - ( (int)( thePcItem->ItemTable->thePrice * ( StoreWnd.theDCRate * 0.01f ) ) + 1 );
		Rsprintf( RWCHAR(StrEld), G_STRING_CHAR( IDS_WND_REPAIR_ELDA ), price );
		Rsprintf( RWCHAR(theWeaponPrice.str), _RT("%s:%s Sale:%d%%"), G_STRING_CHAR( IDS_ITEM_COST ),RWCHAR(StrEld ), StoreWnd.theDCRate );	
	}
	else
	{
		Rsprintf( RWCHAR(StrEld), G_STRING_CHAR( IDS_WND_REPAIR_ELDA ), thePcItem->ItemTable->thePrice );
		Rsprintf( RWCHAR(theWeaponPrice.str), _RT("%s:%s"), G_STRING_CHAR( IDS_ITEM_COST ), RWCHAR(StrEld));
	}
	if(theFontg)
		theFontg->GetStringInfo( theWeaponPrice.str, &theStrInfo );
	if( ClWidth < theStrInfo.Width )
		ClWidth = theStrInfo.Width;
	if( nMaxStrWidthNum < theStrInfo.nCLen )
		nMaxStrWidthNum = theStrInfo.nCLen;
	
	theWeaponPrice.x = 5;
	//theWeaponPrice.x = GetAlignCenterx( &theStrInfo );
	theWeaponPrice.y = theClient.h;
	theClient.h += RV;
	
	//--필요 명성치 
	if(thePcItem->ItemTable->theLimitFame > 0 )
	{
		Rsprintf( RWCHAR(theNeedFame.str), _RT("%s: %d"), G_STRING_CHAR(IDS_WND_NEED_FAME), thePcItem->ItemTable->theLimitFame ); 
		
		if(theFontg)
			theFontg->GetStringInfo(theNeedFame.str , &theStrInfo );
		if(ClWidth < theStrInfo.Width )
			ClWidth = theStrInfo.Width; 
		if(nMaxStrWidthNum < theStrInfo.nCLen)
			nMaxStrWidthNum = theStrInfo.nCLen;
		
		theNeedFame.x = 5; 
		theNeedFame.y = theClient.h;
		theClient.h += RV;
	}
	
	//--설명문 
	
	bool IsDesc = false;
	
	theClient.h += BV;	
	WeaponNumLine = 0;
#ifdef UNICODE_ROCK
	RTCHAR *pStrIn = RWCHAR(thePcItem->ItemTable->theDesc);
	while(pStrIn)
	{
		TCHAR TempBuf[256];
		memset(TempBuf, NULL, sizeof(TCHAR) * 256 );
		
		pStrIn = GetStringReturnInStringW('\\', true, nMaxStrWidthNum, pStrIn, TempBuf);
		if( Rstrcmp(TempBuf, _RT("") ) != 0 )
		{
			SAFE_STR_CPY(theWeaponDesc[WeaponNumLine].str, TempBuf, 256 );
			if(theFontg)
				theFontg->GetStringInfo( theWeaponDesc[WeaponNumLine].str, &theStrInfo );
			if( ClWidth < theStrInfo.Width )
				ClWidth = theStrInfo.Width;
			theWeaponDesc[WeaponNumLine].x = 5;
			//theWeaponDesc[i].x = GetAlignCenterx( &theStrInfo );
			theWeaponDesc[WeaponNumLine].y = theClient.h;
			theClient.h += RV;
			
			WeaponNumLine++;
			IsDesc = true;
		}
	}	
#else

	TCHAR *pStrIn = (TCHAR *)thePcItem->ItemTable->theDesc;
	while(pStrIn)
	{
		TCHAR TempBuf[256];
		memset(TempBuf, NULL, sizeof(TCHAR) * 256 );
		
		pStrIn = GetStringReturnInString('\\', true, nMaxStrWidthNum, pStrIn, TempBuf);
		if( Rstrcmp(TempBuf, _RT("") ) != 0 )
		{
			SAFE_STR_CPY(theWeaponDesc[WeaponNumLine].str, TempBuf, 256 );
			if(theFontg)
				theFontg->GetStringInfo( theWeaponDesc[WeaponNumLine].str, &theStrInfo );
			if( ClWidth < theStrInfo.Width )
				ClWidth = theStrInfo.Width;
			theWeaponDesc[WeaponNumLine].x = 5;
			//theWeaponDesc[i].x = GetAlignCenterx( &theStrInfo );
			theWeaponDesc[WeaponNumLine].y = theClient.h;
			theClient.h += RV;
			
			WeaponNumLine++;
			IsDesc = true;
		}
	}	
#endif
	if(IsDesc)
	{
		theClient.h += BV;
	}
	
	if( (ClWidth + 10) >= MAX_ITEMTIP_WIDTH )
		ClWidth = MAX_ITEMTIP_WIDTH;
	else
		ClWidth += 10;
	
	theClient.w = ClWidth;
}

//-----------------------------------------------------------------------------
void CHelpItem::SetTipWear()
{
	if( thePcItem == NULL || thePcItem->ItemTable == NULL )
	{
		return;
	}
	
	bool blank_a = false;
	bool blank_b = false;
	bool blank_c = false;
	
	
	theTipType = n_TIP_ITEM_WEAR;
	
	int ClWidth = 0;
	int nMaxStrWidthNum = 28;
	
	theClient.h = BV;
	//@@@@ char	theName[20];		// 아이템명....
	Rsprintf( RWCHAR(theWearName.str), _RT("%s"), RWCHAR(thePcItem->ItemTable->theName) );
	if(theFontg)
		theFontg->GetStringInfo( theWearName.str, &theStrInfo );
	if( ClWidth < theStrInfo.Width )
		ClWidth = theStrInfo.Width;
	if( nMaxStrWidthNum < theStrInfo.nCLen )
		nMaxStrWidthNum = theStrInfo.nCLen;
	
	theWearName.x = 5;
	//theWearName.x = GetAlignCenterx( &theStrInfo );
	theWearName.y = theClient.h;
	//by simwoosung	
	SItemBaseInfo * pEldaInfo = NULL;
	if(thePcItem->WearTable.theCode) //추가정보가 있을경우 
	{
		pEldaInfo = SetTipApplyEldastone( theWearName.x, theWearName.y, 
			(int)theStrInfo.Width, ClWidth ); //적용엘다스톤 정보
	}
	theClient.h += RV;
	//-------------------------------------------------------------------------
	
	
	theClient.h += BV;
	
	//@@@@ 장비품 레벨 -- theGrade
	if(thePcItem->ItemTable->theClass != n_Class_Riding )
	{
		Rsprintf( RWCHAR(theWearGrade.str), _RT("%s %s %d"), G_STRING_CHAR( IDS_ITEM_EQUIP ), G_STRING_CHAR( IDS_CHARSTATE_LEVEL ), thePcItem->ItemTable->theGrade / 100 );
		if(theFontg)
			theFontg->GetStringInfo( theWearGrade.str, &theStrInfo );
		if( ClWidth < theStrInfo.Width )
			ClWidth = theStrInfo.Width;
		if( nMaxStrWidthNum < theStrInfo.nCLen )
			nMaxStrWidthNum = theStrInfo.nCLen;
		
		theWearGrade.x = 5;
		//theWearGrade.x = GetAlignCenterx( &theStrInfo );
		theWearGrade.y = theClient.h;
		theClient.h += RV;
	}
	
	//-------------------------------------------------------------------------	
	if( (thePcItem->ItemTable->theNeedSkill) || (thePcItem->ItemTable->theLimitLevel) )
	{	
		//@@@@ 장비품 필요 스킬, 레벨 -- theNeed
		TCHAR NeedSkill[256] = {0,};
		TCHAR NeedLevel[256] = {0,};
  		
		
		if(thePcItem->ItemTable->theNeedSkill)
		{
			SSkillBaseInfo *pSkill_Info = NULL;
			pSkill_Info = g_Pc_Manager.GetSkillBaseInfoPtr((DWORD)thePcItem->ItemTable->theNeedSkill);
			if(pSkill_Info)
			{
				Rsprintf( RWCHAR(NeedSkill), _RT("%s (%s) "), G_STRING_CHAR( IDS_WND_NEED_SKILL ), RWCHAR(pSkill_Info->theSkillName) );		        
				Rstrcat( RWCHAR(theWearNeed.str), RWCHAR(NeedSkill) );
			}		
		}

		if(thePcItem->ItemTable->theLimitLevel)
		{
			Rsprintf( RWCHAR(NeedSkill), _RT("%s (%d) "), G_STRING_CHAR( IDS_WND_NEED_LEVEL ), thePcItem->ItemTable->theLimitLevel );	
			Rstrcat( RWCHAR(theWearNeed.str), RWCHAR(NeedSkill) );
		}		
		
		if(theFontg)
			theFontg->GetStringInfo( theWearNeed.str, &theStrInfo );
		if( ClWidth < theStrInfo.Width )
			ClWidth = theStrInfo.Width;
		if( nMaxStrWidthNum < theStrInfo.nCLen )
			nMaxStrWidthNum = theStrInfo.nCLen;
		
		theWearNeed.x = 5;
		//theWearNeed.x = GetAlignCenterx( &theStrInfo );
		theWearNeed.y = theClient.h;
		theClient.h += RV;		
	}	
	//-------------------------------------------------------------------------	
	
	theClient.h += BV;
	//@@@@ char	theDesc[100];		// 설명
	/*int NumStr = 0;
	if(theFontg)
	NumStr	= theFontg->GetStringCount( thePcItem->ItemTable->theDesc );
	WearNumLine		= GetNumLine( NumStr, 20 );
	DescStartpos	= 0;
	
	  for( int i = 0; i < WearNumLine; ++i )
	  {
	  if(theFontg)
	  theFontg->GetStringDistribute( theWearDesc[i].str, &thePcItem->ItemTable->theDesc[DescStartpos] );
	  DescStartpos += _tcslen( theWearDesc[i].str );
	  
		if(theFontg)
		theFontg->GetStringInfo( theWearDesc[i].str, &theStrInfo );
		if( ClWidth < theStrInfo.Width )
		ClWidth = theStrInfo.Width;
		theWearDesc[i].x = 5;
		//theWearDesc[i].x = GetAlignCenterx( &theStrInfo );
		theWearDesc[i].y = theClient.h;
		theClient.h += RV;
	}*/
	//-------------------------------------------------------------------------

	int theMinAttackBonus = thePcItem->ItemTable->theMinAttackBonus;
	int theMaxAttackBonus = thePcItem->ItemTable->theMaxAttackBonus;
	if( pEldaInfo )
	{
		theMinAttackBonus += pEldaInfo->theMinAttackBonus;
		theMaxAttackBonus += pEldaInfo->theMaxAttackBonus;
	}		

	if( (theMinAttackBonus != 0) || (theMinAttackBonus != 0) )
	{
		if( thePcItem->Code >= c_NotRepairitem_Start &&  
			(thePcItem->ItemTable->theClass == n_Class_Weapon || thePcItem->ItemTable->theClass == n_Class_Accessory ) &&
			thePcItem->ItemTable->theType != n_Type_ShortWeapon ) 
		{
			Rsprintf( RWCHAR(theWeaponAttack.str), _RT("%s %d - %d"), G_STRING_CHAR( IDS_CHARSTATE_PHYATT ), theMinAttackBonus, theMaxAttackBonus );
		}
		else
		{
			///-- 내구력에 따른 공격력 감소
			if( thePcItem->ItemTable->theMaxEndurance && 
				thePcItem->WearTable.theEndurance * 100 / thePcItem->ItemTable->theMaxEndurance <= 50 )
			{
				int li_pen_min = theMinAttackBonus * thePcItem->WearTable.theEndurance * 2 / thePcItem->ItemTable->theMaxEndurance ;
				int li_pen_max = theMaxAttackBonus * thePcItem->WearTable.theEndurance * 2 / thePcItem->ItemTable->theMaxEndurance;
				Rsprintf( RWCHAR(theWeaponAttack.str), _RT("%s %d - %d"), G_STRING_CHAR( IDS_CHARSTATE_PHYATT ), li_pen_min, li_pen_max );
			}
			else
			{
				Rsprintf( RWCHAR(theWeaponAttack.str), _RT("%s %d - %d"), G_STRING_CHAR( IDS_CHARSTATE_PHYATT ), theMinAttackBonus, theMaxAttackBonus );
			}
		}
		
		if(theFontg)
			theFontg->GetStringInfo( theWeaponAttack.str, &theStrInfo );
		if( ClWidth < theStrInfo.Width )
			ClWidth = theStrInfo.Width;
		if( nMaxStrWidthNum < theStrInfo.nCLen )
			nMaxStrWidthNum = theStrInfo.nCLen;
		
		theWeaponAttack.x = 5;
		//theWeaponAttack.x = GetAlignCenterx( &theStrInfo );
		theWeaponAttack.y = theClient.h;
		theClient.h += RV;
		//-------------------------------------------------------------------------
	}	
	
	int theMinMagicBonus = thePcItem->ItemTable->theMinMagicBonus;
	int theMaxMagicBonus = thePcItem->ItemTable->theMaxMagicBonus;
	if( pEldaInfo )
	{
		theMinMagicBonus += pEldaInfo->theMinMagicBonus;
		theMaxMagicBonus += pEldaInfo->theMaxMagicBonus;		
	}
	if( (theMinMagicBonus != 0) || (theMaxMagicBonus != 0) )
	{
		if( thePcItem->Code >= c_NotRepairitem_Start &&  
			(thePcItem->ItemTable->theClass == n_Class_Weapon || thePcItem->ItemTable->theClass == n_Class_Accessory ) &&
			thePcItem->ItemTable->theType != n_Type_ShortWeapon ) 
		{
			Rsprintf( RWCHAR(theWeaponMagic.str), _RT("%s %d - %d"), G_STRING_CHAR( IDS_CHARSTATE_MAGATT ), theMinMagicBonus, theMaxMagicBonus );
		}
		else
		{
			///-- 내구력에 따른 마법 공격력 감소
			if( thePcItem->ItemTable->theMaxEndurance && 
				thePcItem->WearTable.theEndurance * 100 / thePcItem->ItemTable->theMaxEndurance <= 50 )
			{
				int li_pen_min = theMinMagicBonus * thePcItem->WearTable.theEndurance * 2 / thePcItem->ItemTable->theMaxEndurance ;
				int li_pen_max = theMaxMagicBonus * thePcItem->WearTable.theEndurance * 2 / thePcItem->ItemTable->theMaxEndurance;
				Rsprintf( RWCHAR(theWeaponMagic.str), _RT("%s %d - %d"), G_STRING_CHAR( IDS_CHARSTATE_MAGATT ), li_pen_min, li_pen_max );
			}
			else
			{	
				Rsprintf( RWCHAR(theWeaponMagic.str), _RT("%s %d - %d"), G_STRING_CHAR( IDS_CHARSTATE_MAGATT ), theMinMagicBonus, theMaxMagicBonus );
			}
		}
		
		if(theFontg)
			theFontg->GetStringInfo( theWeaponMagic.str, &theStrInfo );
		if( ClWidth < theStrInfo.Width )
			ClWidth = theStrInfo.Width;
		if( nMaxStrWidthNum < theStrInfo.nCLen )
			nMaxStrWidthNum = theStrInfo.nCLen;
		
		theWeaponMagic.x = 5;
		//theWeaponMagic.x = GetAlignCenterx( &theStrInfo );
		theWeaponMagic.y = theClient.h;
		theClient.h += RV;
		//-------------------------------------------------------------------------	
	}	
	
	// 방어. 저항 필드
	//@@@@ BYTE	theAttackDef;		// 대 물리 방어력	0 - 200	
	int theAttackDef = thePcItem->ItemTable->theAttackDef;
	if( pEldaInfo )
	{
		theAttackDef += pEldaInfo->theAttackDef;
	}
	if( theAttackDef != 0 )
	{
		if( thePcItem->Code >= c_NotRepairitem_Start &&  
			(thePcItem->ItemTable->theClass == n_Class_Weapon || thePcItem->ItemTable->theClass == n_Class_Accessory ) &&
			thePcItem->ItemTable->theType != n_Type_ShortWeapon ) 
		{
			Rsprintf( RWCHAR(theWearAttackDef.str), _RT("%s:%d"), G_STRING_CHAR( IDS_CHARSTATE_PHYDEF ), theAttackDef );
		}
		else
		{
			///-- 내구력에 따른 물리 방어력 감소 
			if( thePcItem->ItemTable->theMaxEndurance && 
				thePcItem->WearTable.theEndurance * 100 / thePcItem->ItemTable->theMaxEndurance <= 50 )
			{
				int li_pen = theAttackDef * thePcItem->WearTable.theEndurance * 2 / thePcItem->ItemTable->theMaxEndurance ;			
				Rsprintf( RWCHAR(theWearAttackDef.str), _RT("%s:%d"), G_STRING_CHAR( IDS_CHARSTATE_PHYDEF ), li_pen );	
			}
			else
			{	
				Rsprintf( RWCHAR(theWearAttackDef.str), _RT("%s:%d"), G_STRING_CHAR( IDS_CHARSTATE_PHYDEF ), theAttackDef );
			}
		}
		
		if(theFontg)
			theFontg->GetStringInfo( theWearAttackDef.str, &theStrInfo );
		if( ClWidth < theStrInfo.Width )
			ClWidth = theStrInfo.Width;
		if( nMaxStrWidthNum < theStrInfo.nCLen )
			nMaxStrWidthNum = theStrInfo.nCLen;
		
		theWearAttackDef.x = 5;
		//theWearAttackDef.x = GetAlignCenterx( &theStrInfo );
		theWearAttackDef.y = theClient.h;
		theClient.h += RV;
		//-------------------------------------------------------------------------
		
		blank_a = true;
	}
	//@@@@ BYTE	theMagicRes;		// 대 공격 마법 저항력
	int theMagicRes = thePcItem->ItemTable->theMagicRes;
	if( pEldaInfo )
	{
		theMagicRes += pEldaInfo->theMagicRes;
	}
	if( theMagicRes != 0 )
	{
		if( thePcItem->Code >= c_NotRepairitem_Start &&  
			(thePcItem->ItemTable->theClass == n_Class_Weapon || thePcItem->ItemTable->theClass == n_Class_Accessory ) &&
			thePcItem->ItemTable->theType != n_Type_ShortWeapon ) 
		{
			Rsprintf( RWCHAR(theWearMagicRes.str), _RT("%s:%d"), G_STRING_CHAR( IDS_CHARSTATE_MAGDEF ), theMagicRes );
		}
		else
		{
			///-- 내구력에 따른 마법 방어력 감소
			if( thePcItem->ItemTable->theMaxEndurance && 
				thePcItem->WearTable.theEndurance * 100 / thePcItem->ItemTable->theMaxEndurance <= 50 )
			{
				int li_pen = theMagicRes * thePcItem->WearTable.theEndurance * 2 / thePcItem->ItemTable->theMaxEndurance ;			
				Rsprintf( RWCHAR(theWearMagicRes.str), _RT("%s:%d"), G_STRING_CHAR( IDS_CHARSTATE_MAGDEF ), li_pen );	
			}
			else
			{	
				Rsprintf( RWCHAR(theWearMagicRes.str), _RT("%s:%d"), G_STRING_CHAR( IDS_CHARSTATE_MAGDEF ), theMagicRes );
			}
		}
		
		if(theFontg)
			theFontg->GetStringInfo( theWearMagicRes.str, &theStrInfo );
		if( ClWidth < theStrInfo.Width )
			ClWidth = theStrInfo.Width;
		if( nMaxStrWidthNum < theStrInfo.nCLen )
			nMaxStrWidthNum = theStrInfo.nCLen;
		
		theWearMagicRes.x = 5;
		//theWearMagicRes.x = GetAlignCenterx( &theStrInfo );
		theWearMagicRes.y = theClient.h;
		theClient.h += RV;
		//-------------------------------------------------------------------------
		
		blank_a = true;
	}
	//@@@@ BYTE	theMentalRes;		// 대 정신 마법 저항력
	/*	if( thePcItem->ItemTable->theMentalRes != 0 )
	{
	_stprintf( theWearMentalRes.str, "!정신마법 방어력:%d", thePcItem->ItemTable->theMentalRes );
	if(theFontg)
	theFontg->GetStringInfo( theWearMentalRes.str, &theStrInfo );
	if( ClWidth < theStrInfo.Width )
	ClWidth = theStrInfo.Width;
	theWearMentalRes.x = 5;
	//theWearMentalRes.x = GetAlignCenterx( &theStrInfo );
	theWearMentalRes.y = theClient.h;
	theClient.h += RV;
	//-------------------------------------------------------------------------
	
	  blank_a = true;
	  }
	  //@@@@ BYTE	theCurseRes;		// 대 저주 마법 저항력
	  if( thePcItem->ItemTable->theCurseRes != 0 )
	  {
	  _stprintf( theWearCurseRes.str, "!저주마법 방어력:%d", thePcItem->ItemTable->theCurseRes );
	  if(theFontg)
	  theFontg->GetStringInfo( theWearCurseRes.str, &theStrInfo );
	  if( ClWidth < theStrInfo.Width )
	  ClWidth = theStrInfo.Width;
	  theWearCurseRes.x = 5;
	  //theWearCurseRes.x = GetAlignCenterx( &theStrInfo );
	  theWearCurseRes.y = theClient.h;
	  theClient.h += RV;
	  //-------------------------------------------------------------------------
	  
		blank_a = true;
		}
		//@@@@ BYTE	theVitalRes;		// 대 생체 마법 저항력
		if( thePcItem->ItemTable->theVitalRes != 0 )
		{
		_stprintf( theWearVitalRes.str, "!생체마법 방어력:%d", thePcItem->ItemTable->theVitalRes );
		if(theFontg)
		theFontg->GetStringInfo( theWearVitalRes.str, &theStrInfo );
		if( ClWidth < theStrInfo.Width )
		ClWidth = theStrInfo.Width;
		theWearVitalRes.x = 5;
		//theWearVitalRes.x = GetAlignCenterx( &theStrInfo );
		theWearVitalRes.y = theClient.h;
		theClient.h += RV;
		//-------------------------------------------------------------------------
		
		  blank_a = true;
		  }
	*/	
	
	if( blank_a ){	theClient.h += BV;	}
	//@@@@ BYTE	theDefensAttri;		// 방어속성 
	
	if(thePcItem->ItemTable->theClass != n_Class_Riding )
	{
		switch( thePcItem->ItemTable->theDefensAttri )
		{
		case 0:		Rsprintf( RWCHAR(theWearDefensAttri.str),_RT( "%s:%s"), G_STRING_CHAR( IDS_DEFENSE_ATTR ), G_STRING_CHAR( IDS_ATTR_NONE ) );	break;
		case 1:		Rsprintf( RWCHAR(theWearDefensAttri.str),_RT( "%s:%s"), G_STRING_CHAR( IDS_DEFENSE_ATTR ), G_STRING_CHAR( IDS_ATTR_FIRE ) );		break;
		case 2:		Rsprintf( RWCHAR(theWearDefensAttri.str),_RT( "%s:%s"), G_STRING_CHAR( IDS_DEFENSE_ATTR ), G_STRING_CHAR( IDS_ATTR_ICE ));		break;
		case 3:		Rsprintf( RWCHAR(theWearDefensAttri.str),_RT( "%s:%s"), G_STRING_CHAR( IDS_DEFENSE_ATTR ), G_STRING_CHAR( IDS_ATTR_EARTH ));	break;
		case 4:		Rsprintf( RWCHAR(theWearDefensAttri.str),_RT( "%s:%s"), G_STRING_CHAR( IDS_DEFENSE_ATTR ), G_STRING_CHAR( IDS_ATTR_LIGHTNING ));		break;
		case 5:		Rsprintf( RWCHAR(theWearDefensAttri.str),_RT( "%s:%s"), G_STRING_CHAR( IDS_DEFENSE_ATTR ), G_STRING_CHAR( IDS_ATTR_WATER ));		break;
		case 6:		Rsprintf( RWCHAR(theWearDefensAttri.str),_RT( "%s:%s"), G_STRING_CHAR( IDS_DEFENSE_ATTR ), G_STRING_CHAR( IDS_ATTR_NATURE ));	break;
		case 7:		Rsprintf( RWCHAR(theWearDefensAttri.str),_RT( "%s:%s"), G_STRING_CHAR( IDS_DEFENSE_ATTR ), G_STRING_CHAR( IDS_ATTR_HOLY ));	break;
		case 8:		Rsprintf( RWCHAR(theWearDefensAttri.str),_RT( "%s:%s"), G_STRING_CHAR( IDS_DEFENSE_ATTR ), G_STRING_CHAR( IDS_ATTR_DARK ));	break;
		case 9:		Rsprintf( RWCHAR(theWearDefensAttri.str),_RT( "%s:%s"), G_STRING_CHAR( IDS_DEFENSE_ATTR ), G_STRING_CHAR( IDS_ATTR_SPRIT ));	break;
		case 10:	Rsprintf( RWCHAR(theWearDefensAttri.str),_RT( "%s:%s"), G_STRING_CHAR( IDS_DEFENSE_ATTR ), G_STRING_CHAR( IDS_ATTR_CURSE ));	break;
		case 11:	Rsprintf( RWCHAR(theWearDefensAttri.str),_RT( "%s:%s"), G_STRING_CHAR( IDS_DEFENSE_ATTR ), G_STRING_CHAR( IDS_ATTR_BIONIC ));	break;
		case 12:	Rsprintf( RWCHAR(theWearDefensAttri.str),_RT( "%s:-"),  G_STRING_CHAR( IDS_DEFENSE_ATTR ));		break;
		}
		if(theFontg)
			theFontg->GetStringInfo( theWearDefensAttri.str, &theStrInfo );
		if( ClWidth < theStrInfo.Width )
			ClWidth = theStrInfo.Width;
		if( nMaxStrWidthNum < theStrInfo.nCLen )
			nMaxStrWidthNum = theStrInfo.nCLen;
		
		theWearDefensAttri.x = 5;
		//theWearDefensAttri.x = GetAlignCenterx( &theStrInfo );
		theWearDefensAttri.y = theClient.h;
		theClient.h += RV;
		//-------------------------------------------------------------------------
		theClient.h += BV;		
	}
	
	///-- 이쪽에다가 악세사리 관련 저항 헬프팁 추가
	BYTE aClass = thePcItem->ItemTable->theClass;
	if( aClass == n_Class_Accessory )
	{
		WORD aScrollCode = 0;
		aScrollCode =  thePcItem->WearTable.theEldaStone;
		if( aScrollCode )
		{
			SetAccAddEffect( aScrollCode, theAccAddEffect );
			if( Rstrcmp( theAccAddEffect.str, _RT("")) != 0 )
			{
				if(theFontg)
					theFontg->GetStringInfo( theAccAddEffect.str, &theStrInfo );
				if( ClWidth < theStrInfo.Width )
					ClWidth = theStrInfo.Width;
				if( nMaxStrWidthNum < theStrInfo.nCLen )
					nMaxStrWidthNum = theStrInfo.nCLen;

				theAccAddEffect.x = 5;		
				theAccAddEffect.y = theClient.h;
				theClient.h += RV;
			}
		}
	}
	
	SetItemProperty( pEldaInfo, theWearproperty );
	if( Rstrcmp( theWearproperty.str, _RT("")) != 0 )
	{
		if(theFontg)
			theFontg->GetStringInfo( theWearproperty.str, &theStrInfo );
		if( ClWidth < theStrInfo.Width )
			ClWidth = theStrInfo.Width;
		if( nMaxStrWidthNum < theStrInfo.nCLen )
			nMaxStrWidthNum = theStrInfo.nCLen;
		
		theWearproperty.x = 5;
		//theWearproperty.x = GetAlignCenterx( &theStrInfo );
		theWearproperty.y = theClient.h;
		theClient.h += RV;
		//-------------------------------------------------------------------------		
		blank_a = true;
	}	
	
	//@@@@ short	theAttackEvade;		// 공격 회피 보너스
	/*	if( thePcItem->ItemTable->theAttackEvade != 0 )
	{
	_stprintf( theWearAttackEvade.str, "!회피+%d", thePcItem->ItemTable->theAttackEvade );
	if(theFontg)
	theFontg->GetStringInfo( theWearAttackEvade.str, &theStrInfo );
	if( ClWidth < theStrInfo.Width )
	ClWidth = theStrInfo.Width;
	theWearAttackEvade.x = 5;
	//theWearAttackEvade.x = GetAlignCenterx( &theStrInfo );
	theWearAttackEvade.y = theClient.h;
	theClient.h += RV;
	//-------------------------------------------------------------------------
	
	  blank_b = true;
	  }
	*/	
	
	if( blank_b ){	theClient.h += BV;	}
	//@@@@ WORD	theOwnSkillCode;	// 고유 스킬....
	if( thePcItem->ItemTable->theOwnSkillCode != 0 )
	{
		SSkillBaseInfo* skilltable = g_Pc_Manager.GetSkillBaseInfoPtr( thePcItem->ItemTable->theOwnSkillCode );
		
		if( skilltable )
		{
			Rsprintf( RWCHAR(theWearSkillName.str), _RT("%s:%s") , G_STRING_CHAR( IDS_SKILL_NAME ), RWCHAR(skilltable->theSkillName) );
			if(theFontg)
				theFontg->GetStringInfo( theWearSkillName.str, &theStrInfo );
			if( ClWidth < theStrInfo.Width )
				ClWidth = theStrInfo.Width;
			if( nMaxStrWidthNum < theStrInfo.nCLen )
				nMaxStrWidthNum = theStrInfo.nCLen;
			
			theWearSkillName.x = 5;
			//theWearSkillName.x = GetAlignCenterx( &theStrInfo );
			theWearSkillName.y = theClient.h;
			theClient.h += RV;
			//-------------------------------------------------------------------------
			
			blank_c = true;
		}
	}	
	
	
	//@@@@ BYTE	theAddEffect1;		// 장비효과 1,2 
	//@@@@ BYTE	theAddEffect2;
	if(  thePcItem->ItemTable->theAddEffect1 != 0  )
	{
		SItemAddEffectInfo * pItemAddEffectInfo = NULL;
		pItemAddEffectInfo = g_Pc_Manager.GetItemAddEffectInfo(thePcItem->ItemTable->theAddEffect1);
		
		if(pItemAddEffectInfo && pItemAddEffectInfo->theAddEffectType)
		{
			Rsprintf( RWCHAR(theWeaponAddEffect1.str), _RT("%s: "), G_STRING_CHAR( IDS_EQUIPMENT_EFFECT ));								
			SetItemAddEffectInfo(theWearAddEffect1.str, pItemAddEffectInfo);
			
			if(theFontg)
				theFontg->GetStringInfo( theWearAddEffect1.str, &theStrInfo );
			if( ClWidth < theStrInfo.Width )
				ClWidth = theStrInfo.Width;
			if( nMaxStrWidthNum < theStrInfo.nCLen )
				nMaxStrWidthNum = theStrInfo.nCLen;
			
			theWearAddEffect1.x = 5;
			//theWeaponAddEffect.x = GetAlignCenterx( &theStrInfo );
			theWearAddEffect1.y = theClient.h;
			theClient.h += RV;
			//-------------------------------------------------------------------------		
			blank_c = true;		
		}
	}
	if(  thePcItem->ItemTable->theAddEffect2 != 0  )
	{
		SItemAddEffectInfo * pItemAddEffectInfo = NULL;
		pItemAddEffectInfo = g_Pc_Manager.GetItemAddEffectInfo(thePcItem->ItemTable->theAddEffect2);
		
		if(pItemAddEffectInfo && pItemAddEffectInfo->theAddEffectType)
		{
			Rsprintf( RWCHAR(theWeaponAddEffect1.str), _RT("%s: "), G_STRING_CHAR( IDS_EQUIPMENT_EFFECT ));			
			SetItemAddEffectInfo(theWearAddEffect2.str, pItemAddEffectInfo);
			
			if(theFontg)
				theFontg->GetStringInfo( theWearAddEffect2.str, &theStrInfo );
			if( ClWidth < theStrInfo.Width )
				ClWidth = theStrInfo.Width;
			if( nMaxStrWidthNum < theStrInfo.nCLen )
				nMaxStrWidthNum = theStrInfo.nCLen;
			
			theWearAddEffect2.x = 5;
			//theWeaponAddEffect.x = GetAlignCenterx( &theStrInfo );
			theWearAddEffect2.y = theClient.h;
			theClient.h += RV;
			//-------------------------------------------------------------------------		
			blank_c = true;		
		}
	}
	
	
	if( blank_c ){	theClient.h += BV;	}
	//@@@@ WORD	theMaxEndurance;	// 최대 내구력
	//@@@@ BYTE	theBundle;			// 번들가능 갯수, 1=번들불가
	//@@@@ BYTE	theWeight;			// 무게
	//	_stprintf( theWearEBW.str, "내구력:%d 무게:%d", thePcItem->ItemTable->theMaxEndurance, thePcItem->ItemTable->theWeight);
	
	
#ifdef C_TIME_SUIT_UPDATE
	//시간제 아이템 조건  시간이 0 이고  
	if(thePcItem->ItemTable->theMaxEndurance == 0  && thePcItem->ItemTable->theRare == 255 )
	{	
		g_ListTime.GetTimeRemainStr(thePcItem->ItemUID , theWearEBW.str );
	}
	else
	{
		if(thePcItem->ItemTable->theClass == n_Class_Riding )
		{
			Rsprintf( RWCHAR(theWearEBW.str),_RT( "%s: %d / %d"), G_STRING_CHAR(IDS_CHARSTATE_ENDURANCE_FOREFOOT)	, thePcItem->WearTable.theEndurance, thePcItem->ItemTable->theMaxEndurance );	 
		}
		else 
		{
			Rsprintf( RWCHAR(theWearEBW.str),_RT( "%s: %d / %d"), G_STRING_CHAR( IDS_CHARSTATE_ENDURANCE ), thePcItem->WearTable.theEndurance, thePcItem->ItemTable->theMaxEndurance );	
		}
	}
	
#else
	
	if(thePcItem->ItemTable->theClass == n_Class_Riding )
	{
		Rsprintf( RWCHAR(theWearEBW.str),_RT( "%s: %d / %d"), G_STRING_CHAR(IDS_CHARSTATE_ENDURANCE_FOREFOOT)	, thePcItem->WearTable.theEndurance, thePcItem->ItemTable->theMaxEndurance );	 
	}
	else 
	{
		Rsprintf( RWCHAR(theWearEBW.str),_RT( "%s: %d / %d"), G_STRING_CHAR( IDS_CHARSTATE_ENDURANCE ), thePcItem->WearTable.theEndurance, thePcItem->ItemTable->theMaxEndurance );	
	}
	
	
#endif
	
	
	//_stprintf( theWearEBW.str, " 내구력:%d, 무게:%d, 가격:%dEld", thePcItem->ItemTable->theMaxEndurance, thePcItem->ItemTable->theWeight, thePcItem->ItemTable->thePrice );
	if(theFontg)
		theFontg->GetStringInfo( theWearEBW.str, &theStrInfo );
	if( ClWidth < theStrInfo.Width )
		ClWidth = theStrInfo.Width;
	if( nMaxStrWidthNum < theStrInfo.nCLen )
		nMaxStrWidthNum = theStrInfo.nCLen;
	
	theWearEBW.x = 5;
	//theWearEBW.x = GetAlignCenterx( &theStrInfo );
	theWearEBW.y = theClient.h;
	theClient.h += RV;
	//-------------------------------------------------------------------------
	TCHAR StrEld[256]="";
	if( ( StoreWnd.theDCRate > 0 ) && ( ((CSlot*)theSlotWnd)->Group == n_SGROUP_STORE ) )
	{
		int price = thePcItem->ItemTable->thePrice - ( (int)( thePcItem->ItemTable->thePrice * ( StoreWnd.theDCRate * 0.01f ) ) + 1 );
#ifdef CPK_SYSTEM	
		if( ((CSlot*)theSlotWnd)->Group == n_SGROUP_STORE )
		{
			price *= GetBuyShopRatioFromPKPoint( nRui->GetPcParam()->thePKPoint );
		}
#endif	
		Rsprintf( RWCHAR(StrEld), G_STRING_CHAR( IDS_WND_REPAIR_ELDA ), price );
		Rsprintf( RWCHAR(theWearPrice.str), _RT("%s:%s Sale:%d%%"), G_STRING_CHAR( IDS_ITEM_COST  ), RWCHAR(StrEld ), StoreWnd.theDCRate );	
	}
	else
	{
		int price = thePcItem->ItemTable->thePrice;
#ifdef CPK_SYSTEM	
		if( ((CSlot*)theSlotWnd)->Group == n_SGROUP_STORE )
		{
			price *= GetBuyShopRatioFromPKPoint( nRui->GetPcParam()->thePKPoint );
		}
#endif	
		Rsprintf( RWCHAR(StrEld), G_STRING_CHAR( IDS_WND_REPAIR_ELDA ), price );
		Rsprintf( RWCHAR(theWearPrice.str), _RT("%s:%s"), G_STRING_CHAR( IDS_ITEM_COST ),RWCHAR(StrEld ));
	}
	
	if(theFontg)
		theFontg->GetStringInfo( theWearPrice.str, &theStrInfo );
	if( ClWidth < theStrInfo.Width )
        ClWidth = theStrInfo.Width;
	if( nMaxStrWidthNum < theStrInfo.nCLen )
		nMaxStrWidthNum = theStrInfo.nCLen;
	
	theWearPrice.x = 5;
	//theWearPrice.x = GetAlignCenterx( &theStrInfo );
	theWearPrice.y = theClient.h;
	theClient.h += RV;
	
	//--필요 명성치 
	if(thePcItem->ItemTable->theLimitFame > 0 )
	{
		Rsprintf( RWCHAR(theNeedFame.str), _RT("%s: %d"), G_STRING_CHAR(IDS_WND_NEED_FAME), 
				  thePcItem->ItemTable->theLimitFame ); 
		
		if(theFontg)
			theFontg->GetStringInfo(theNeedFame.str , &theStrInfo );
		if(ClWidth < theStrInfo.Width )
			ClWidth = theStrInfo.Width; 
		if(nMaxStrWidthNum < theStrInfo.nCLen)
			nMaxStrWidthNum = theStrInfo.nCLen;
		
		theNeedFame.x = 5; 
		theNeedFame.y = theClient.h;
		theClient.h += RV;
		
	}	
	
	bool IsDesc = false;
	theClient.h += BV;		
	
	char CoatArmor[128]= {0,};
	char PantsArmor[128]= {0,};
	char Gloves[128] = {0,};
	char Shoes[128]= {0,};
	
	//--------------------------------------------------------
	// 세트 아이템일경우 헬프팁에 관련된 세트아이템 보여준다 .. 
	//-----------------------------------------------------------
	
	SItemBaseInfo* Item_Coat = NULL ; 
	SItemBaseInfo* Item_Pants = NULL ; 
	SItemBaseInfo* Item_Gloves = NULL ; 
	SItemBaseInfo* Item_Shoes = NULL ; 
	
	//셋 아이템 일경우 
	if( (thePcItem->ItemTable->theSetID  !=0 ) && 
		(thePcItem->ItemTable->theType >= n_Type_Coat && thePcItem->ItemTable->theType <= n_Type_SpiritShoes ) 
		)
	{
		switch(thePcItem->Class) 
		{
			
			//상의 일경우 
			//아이템 코드값이 처음 것이다 순차적으로 +1 +2 +3 
		case n_Class_CoatArmor:		
			{
				Item_Coat = 	g_Pc_Manager.GetItemBaseInfoPtr(  thePcItem->Code	);	//상의 
				Item_Pants =	g_Pc_Manager.GetItemBaseInfoPtr(  thePcItem->Code+1 ); //하의 
				Item_Gloves =   g_Pc_Manager.GetItemBaseInfoPtr(  thePcItem->Code+2 ); //장갑 			
				Item_Shoes =    g_Pc_Manager.GetItemBaseInfoPtr(  thePcItem->Code+3 );	//부츠 	
			}
			break;
		case n_Class_PantsArmor:	
			{		// 10 11 12 13 
				Item_Coat = 	g_Pc_Manager.GetItemBaseInfoPtr(  thePcItem->Code-1	);	//상의 
				Item_Pants =	g_Pc_Manager.GetItemBaseInfoPtr(  thePcItem->Code ); //하의 
				Item_Gloves =   g_Pc_Manager.GetItemBaseInfoPtr(  thePcItem->Code+1 ); //장갑 			
				Item_Shoes =    g_Pc_Manager.GetItemBaseInfoPtr(  thePcItem->Code+2 );	//부츠 	
			}
			break;
		case n_Class_Gloves:		
			{		// 10 11 12 13 
				Item_Coat = 	g_Pc_Manager.GetItemBaseInfoPtr(  thePcItem->Code-2	);	//상의 
				Item_Pants =	g_Pc_Manager.GetItemBaseInfoPtr(  thePcItem->Code-1 ); //하의 
				Item_Gloves =   g_Pc_Manager.GetItemBaseInfoPtr(  thePcItem->Code ); //장갑 			
				Item_Shoes =    g_Pc_Manager.GetItemBaseInfoPtr(  thePcItem->Code+1 );	//부츠 	
			}
			break;
		case n_Class_Shoes:			
			{		// 10 11 12 13 
				Item_Coat = 	g_Pc_Manager.GetItemBaseInfoPtr(  thePcItem->Code-3	);	//상의 
				Item_Pants =	g_Pc_Manager.GetItemBaseInfoPtr(  thePcItem->Code-2 ); //하의 
				Item_Gloves =   g_Pc_Manager.GetItemBaseInfoPtr(  thePcItem->Code-1 ); //장갑 			
				Item_Shoes =    g_Pc_Manager.GetItemBaseInfoPtr(  thePcItem->Code );	//부츠 	
			}
			break; 
		default: break; 
		}
		
		//---------------------------------------------------------------
		// 4개의 아이템을 구한것을 화면에 보여준다 ..  
		//-----------------------------------------------------------------
		
		for(int i =0 ; i < 4 ; ++i)
		{
			
			switch(i) 
			{
			case 0:SAFE_STR_CPY(theWearSetItem[0].str,Item_Coat->theName, 256); break;
			case 1:SAFE_STR_CPY(theWearSetItem[1].str,Item_Pants->theName,256); break;
			case 2:SAFE_STR_CPY(theWearSetItem[2].str,Item_Gloves->theName, 256); break;
			case 3:SAFE_STR_CPY(theWearSetItem[3].str,Item_Shoes->theName, 256); break;
			default: break;
			}
			
			if(theFontg)
				theFontg->GetStringInfo( theWearSetItem[i].str, &theStrInfo );
			if( ClWidth < theStrInfo.Width )
				ClWidth = theStrInfo.Width;
			if( nMaxStrWidthNum < theStrInfo.nCLen )
				nMaxStrWidthNum = theStrInfo.nCLen;
			
			theWearSetItem[i].x = 5;
			//theWearEBW.x = GetAlignCenterx( &theStrInfo );
			theWearSetItem[i].y = theClient.h;
			theClient.h += RV;
			
			
		}
		
		theClient.h += BV ; 
	}
	
	//-- 아이템 설명 
	
	WearNumLine = 0;
#ifdef UNICODE_ROCK
	RTCHAR *pStrIn = RWCHAR(thePcItem->ItemTable->theDesc);
	while(pStrIn)
	{
		TCHAR TempBuf[256];
		memset(TempBuf, NULL, sizeof(TCHAR) * 256 );
		
		pStrIn = GetStringReturnInStringW('\\', false, nMaxStrWidthNum, pStrIn, TempBuf);
		
		if( Rstrcmp(TempBuf, _RT("") ) != 0 )
		{
			SAFE_STR_CPY(theWearDesc[WearNumLine].str, TempBuf, 256);
			if(theFontg)
				theFontg->GetStringInfo( theWearDesc[WearNumLine].str, &theStrInfo );
			if( ClWidth < theStrInfo.Width )
				ClWidth = theStrInfo.Width;
			theWearDesc[WearNumLine].x = 5;
			//theWeaponDesc[i].x = GetAlignCenterx( &theStrInfo );
			theWearDesc[WearNumLine].y = theClient.h;
			theClient.h += RV;
			
			WearNumLine++;
			IsDesc = false;
		}
	}	
#else
	
	TCHAR *pStrIn = (TCHAR *)thePcItem->ItemTable->theDesc;
	while(pStrIn)
	{
		TCHAR TempBuf[256];
		memset(TempBuf, NULL, sizeof(TCHAR) * 256 );
		
		pStrIn = GetStringReturnInString('\\', false, nMaxStrWidthNum, pStrIn, TempBuf);
		
		if( Rstrcmp(TempBuf, _RT("") ) != 0 )
		{
			SAFE_STR_CPY(theWearDesc[WearNumLine].str, TempBuf, 256);
			if(theFontg)
				theFontg->GetStringInfo( theWearDesc[WearNumLine].str, &theStrInfo );
			if( ClWidth < theStrInfo.Width )
				ClWidth = theStrInfo.Width;
			theWearDesc[WearNumLine].x = 5;
			//theWeaponDesc[i].x = GetAlignCenterx( &theStrInfo );
			theWearDesc[WearNumLine].y = theClient.h;
			theClient.h += RV;
			
			WearNumLine++;
			IsDesc = false;
		}
	}	
#endif
	if(IsDesc)
	{
		theClient.h += BV;
	}
	
	if( (ClWidth + 10) >= MAX_ITEMTIP_WIDTH )
		ClWidth = MAX_ITEMTIP_WIDTH;
	else
		ClWidth += 10;
    
	theClient.w = ClWidth;
	//by dongs 부가 설명 색깔을 바꾼다 .. 
	// 4개의 셋트 아이템일 경우 . 빨강색으로 변경 .
	/*
	
	  n_WearSlot_CoatArmor = 0,
	  n_WearSlot_PantsArmor,
	  n_WearSlot_Gloves,
	  n_WearSlot_Shoes,
	*/
	
	
	
	// bFoucs == 0  ; -> 장비슬롯 1이면 인벤토리슬롯이다 .. 
	int  iFocus =  0 ; 
	CWnd* Fwnd = NULL;
	
	
	for( int i = 0; i < WearNumLine; ++i )
	{
		//	theWearDesc[i].color = D3DCOLOR_XRGB( 240, 120, 142 );
		theWearDesc[i].SetMode  = 0 ;
	}
	
	
	if( thePcItem->ItemTable->theSetID  ==0 )
	{
		return;
	}
	
	
	switch(thePcItem->ItemTable->theClass) 
	{
	case n_Class_CoatArmor:		//2  상의
	case n_Class_PantsArmor:	//3  하의
	case n_Class_Gloves:		//4  장갑
	case n_Class_Shoes:			//5  부츠
		{
			//셋트 아이템 경우일때만 통과한다 .. 
			if( thePcItem->ItemTable->theSetID != 0 )
			{
				break; 
			}
			else
			{
				return;
			}
		}	
		
	default:	return;
	}
	
	for(short i = 0 ; i < 4 ; ++i)
	{
		theWearSetItem[i].color = D3DCOLOR_XRGB( 150, 150, 150);
	}
	
	//-- 아이템 색 표시 
	if( ( nRui->GetFocusWnd() != WID_None ) && ( nRui->GetFocusWnd() != WID_DeskTop ) )
	{
		
		Fwnd = Container->Get( nRui->GetFocusWnd() );
		switch(Fwnd->GetID())
		{
		case WID_Inven_WearSlot0:
		case WID_Inven_WearSlot1:
		case WID_Inven_WearSlot2:
		case WID_Inven_WearSlot3:
		case WID_Inven_WearSlot4:
		case WID_Inven_WearSlot5:
		case WID_Inven_WearSlot6:
		case WID_Inven_WearSlot7:
		case WID_Inven_WearSlot8:
		case WID_Inven_WearSlot9:
		case WID_Inven_WearSlot10:
		case WID_Inven_WearSlot11:
		case WID_Inven_WearSlot12:
		case WID_Inven_WearSlot13:
			{
				//iFocus = 0 ; 
				SetItemWear();
			}
			break;
			//-------------------------------------------------------------------------
		case WID_Inven_ItemSlot0:
		case WID_Inven_ItemSlot1:
		case WID_Inven_ItemSlot2:
		case WID_Inven_ItemSlot3:
		case WID_Inven_ItemSlot4:
		case WID_Inven_ItemSlot5:
		case WID_Inven_ItemSlot6:
		case WID_Inven_ItemSlot7:
		case WID_Inven_ItemSlot8:
		case WID_Inven_ItemSlot9:
		case WID_Inven_ItemSlot10:
		case WID_Inven_ItemSlot11:
		case WID_Inven_ItemSlot12:
		case WID_Inven_ItemSlot13:
		case WID_Inven_ItemSlot14:
		case WID_Inven_ItemSlot15:
		case WID_Inven_ItemSlot16:
		case WID_Inven_ItemSlot17:
		case WID_Inven_ItemSlot18:
		case WID_Inven_ItemSlot19:
		case WID_Inven_ItemSlot20:
		case WID_Inven_ItemSlot21:
		case WID_Inven_ItemSlot22:
		case WID_Inven_ItemSlot23:
		case WID_Inven_ItemSlot24:
			{
				SetItemInventory();
			}
			break;
		case WID_Exchange_MineItemSlot0: 
		case WID_Exchange_MineItemSlot1:
		case WID_Exchange_MineItemSlot2:
		case WID_Exchange_MineItemSlot3:
		case WID_Exchange_MineItemSlot4:
		case WID_Exchange_MineItemSlot5:
		case WID_Exchange_MineItemSlot6:
		case WID_Exchange_MineItemSlot7:
		case WID_Exchange_MineItemSlot8:
		case WID_Exchange_MineItemSlot9:
			{
				SetMyItem();
				break; 
			}
			
		case WID_Exchange_YoursItemSlot0:
		case WID_Exchange_YoursItemSlot1:
		case WID_Exchange_YoursItemSlot2:
		case WID_Exchange_YoursItemSlot3:
		case WID_Exchange_YoursItemSlot4:
		case WID_Exchange_YoursItemSlot5:
		case WID_Exchange_YoursItemSlot6:
		case WID_Exchange_YoursItemSlot7:
		case WID_Exchange_YoursItemSlot8:
		case WID_Exchange_YoursItemSlot9:		
			{
				SetYourItem();
				break;
			}
			
			//인벤도 아니고 장비슬롯 도 아닌경우
			//셋트 아이템 일 경우 .. 
		default:  
			{
			}		
			break; 
			
		}
		
		if( g_UserStoreSellWnd.IsVisible)
		{
			//개인상점  Focus
			for( int i = 0 ; i < USER_STORE_SELL_ITEM_SLOT_MAX_COUNT ; ++i )
			{
				if( g_UserStoreSellWnd.m_pItemSlotList[ i ]->GetID() == Fwnd->GetID() )
				{
					SetItemUserStore();
					break;
				}
			}
		}
		else if( g_UserStoreBuyWnd.IsVisible )
		{
			//다른 상점 보기 .. 
			for( int i = 0 ; i < USER_STORE_SELL_ITEM_SLOT_MAX_COUNT ; ++i )
			{
				if( g_UserStoreBuyWnd.m_pItemSlotList[ i ]->GetID() == Fwnd->GetID() )
				{
					SetItemUserStoreBuy();
					break;
				}
			}
			
		}
	}
}
void CHelpItem::SetMyItem()
{
	
	
	if (!g_ExchangeWnd.IsVisible)
	{	
		return;
	}
	
	//	g_ExchangeWnd.m_MinePcItem[c_MaxExchangeNum];
	if( thePcItem->ItemTable->theSetID != 0 )
	{
		for( int i = 0; i < WearNumLine; ++i )
		{
			theWearDesc[i].SetMode = 1 ;
		}
	}
	
	SPcItem* MinePcItem =  g_ExchangeWnd.GetMyItemPcItem();
	
	for(int i =0 ; i < c_MaxExchangeNum ; ++i )
	{
		if( MinePcItem[i].ItemTable == NULL )
		{
			continue;
		}
		
		if( MinePcItem[i].ItemTable && 
			MinePcItem[i].ItemTable->theSetID == thePcItem->ItemTable->theSetID )
		{
			
			switch(MinePcItem[i].Class) 
			{
			case n_Class_CoatArmor:	
				theWearSetItem[0].color = GetRareColor( thePcItem->ItemTable->theRare);
				break;
			case n_Class_PantsArmor:
				theWearSetItem[1].color = GetRareColor( thePcItem->ItemTable->theRare);
				break;
			case n_Class_Gloves:
				theWearSetItem[2].color = GetRareColor( thePcItem->ItemTable->theRare);
				break;
			case n_Class_Shoes:
				theWearSetItem[3].color = GetRareColor( thePcItem->ItemTable->theRare);
				break;
			default:
				break;
			}
		}
	}
	
	
}

void CHelpItem::SetYourItem()
{
	
	if (!g_ExchangeWnd.IsVisible)
	{	
		return;
	}
	
	if( thePcItem->ItemTable->theSetID != 0 )
	{
		for( int i = 0; i < WearNumLine; ++i )
		{
			theWearDesc[i].SetMode = 1 ;
		}
	}
	SPcItem* YoursPcItem = g_ExchangeWnd.GetYoursItemPcItem();
	
	
	for(int i =0 ; i < c_MaxExchangeNum ; ++i )
	{
		if( YoursPcItem[i].ItemTable == NULL )
		{
			continue;
		}
		
		if( YoursPcItem[i].ItemTable && 
			YoursPcItem[i].ItemTable->theSetID == thePcItem->ItemTable->theSetID )
		{
			
			switch(YoursPcItem[i].Class) 
			{
			case n_Class_CoatArmor:	
				theWearSetItem[0].color = GetRareColor( thePcItem->ItemTable->theRare);
				break;
			case n_Class_PantsArmor:
				theWearSetItem[1].color = GetRareColor( thePcItem->ItemTable->theRare);
				break;
			case n_Class_Gloves:
				theWearSetItem[2].color = GetRareColor( thePcItem->ItemTable->theRare);
				break;
			case n_Class_Shoes:
				theWearSetItem[3].color = GetRareColor( thePcItem->ItemTable->theRare);
				break;
			default:
				break;
			}
		}
	}
	
}
void CHelpItem::SetItemUserStoreBuy()
{
	
	if( thePcItem->ItemTable->theSetID != 0 )
	{
		for( int i = 0; i < WearNumLine; ++i )
		{
			theWearDesc[i].SetMode = 1 ;
		}
	}
	
	
	for(int i =0 ; i <USER_STORE_SELL_ITEM_SAVE_MAX_COUNT ; ++i )
	{
		if( g_UserStoreBuyWnd.m_WarehousePcItem[i].ItemTable == NULL )
		{
			continue;
		}
		
		if( g_UserStoreBuyWnd.m_WarehousePcItem[i].ItemTable && 
			g_UserStoreBuyWnd.m_WarehousePcItem[i].ItemTable->theSetID == thePcItem->ItemTable->theSetID )
		{
			
			switch(g_UserStoreBuyWnd.m_WarehousePcItem[i].Class) 
			{
			case n_Class_CoatArmor:	
				theWearSetItem[0].color = GetRareColor( thePcItem->ItemTable->theRare);
				break;
			case n_Class_PantsArmor:
				theWearSetItem[1].color = GetRareColor( thePcItem->ItemTable->theRare);
				break;
			case n_Class_Gloves:
				theWearSetItem[2].color = GetRareColor( thePcItem->ItemTable->theRare);
				break;
			case n_Class_Shoes:
				theWearSetItem[3].color = GetRareColor( thePcItem->ItemTable->theRare);
				break;
			default:
				break;
			}
		}
	}
	
	
	
	
}


void CHelpItem::SetItemUserStore()
{
	
	if( thePcItem->ItemTable->theSetID != 0 )
	{
		for( int i = 0; i < WearNumLine; ++i )
		{
			theWearDesc[i].SetMode = 1 ;
		}
	}
	
	
	for(int i =0 ; i <USER_STORE_SELL_ITEM_SAVE_MAX_COUNT ; ++i )
	{
		if( g_UserStoreSellWnd.m_WarehousePcItem[i].ItemTable == NULL )
		{
			continue;
		}
		
		if( g_UserStoreSellWnd.m_WarehousePcItem[i].ItemTable && 
			g_UserStoreSellWnd.m_WarehousePcItem[i].ItemTable->theSetID == thePcItem->ItemTable->theSetID )
		{
			
			switch(g_UserStoreSellWnd.m_WarehousePcItem[i].Class) 
			{
			case n_Class_CoatArmor:	
				theWearSetItem[0].color = GetRareColor( thePcItem->ItemTable->theRare);
				break;
			case n_Class_PantsArmor:
				theWearSetItem[1].color = GetRareColor( thePcItem->ItemTable->theRare);
				break;
			case n_Class_Gloves:
				theWearSetItem[2].color = GetRareColor( thePcItem->ItemTable->theRare);
				break;
			case n_Class_Shoes:
				theWearSetItem[3].color = GetRareColor( thePcItem->ItemTable->theRare);
				break;
			default:
				break;
			}
		}
	}
}

void CHelpItem::SetItemWear()
{
	
	int SetItemCnt = 0  ; 
	int	SetID = 0 ; 
	//
	
	//세트 아이템이고 슈트이면..
	if( thePcItem->ItemTable->theSetID  !=0  && thePcItem->ItemTable->theType == n_Type_SetCloth)
	{		
		for( int i = 0; i < WearNumLine; ++i )
		{
			theWearDesc[i].SetMode = 2 ;
		}
		return;
	}
	
	
	
	
	//상의 
	if(nRui->thePcInven.WearItem[n_WearSlot_CoatArmor] )
	{
		//	Item_Coat = 	g_Pc_Manager.GetItemBaseInfoPtr(  nRui->thePcInven.WearItem[n_WearSlot_CoatArmor]->Code  );
		//셋아이템일  경우  카운를 증가 시킨다 . 
		if( nRui->thePcInven.WearItem[n_WearSlot_CoatArmor]->ItemTable->theSetID == thePcItem->ItemTable->theSetID) 
		{
			++SetItemCnt;
			theWearSetItem[0].color = GetRareColor( thePcItem->ItemTable->theRare);
		}
		
	}
	//하의 
	if(nRui->thePcInven.WearItem[n_WearSlot_PantsArmor] )
	{
		//Item_Pants = 	g_Pc_Manager.GetItemBaseInfoPtr(  nRui->thePcInven.WearItem[n_WearSlot_PantsArmor]->Code  );
		if( nRui->thePcInven.WearItem[n_WearSlot_PantsArmor]->ItemTable->theSetID == thePcItem->ItemTable->theSetID )
		{
			++SetItemCnt; 
			theWearSetItem[1].color = GetRareColor( thePcItem->ItemTable->theRare);	
		}
		
	}
	
	//장갑 
	if(nRui->thePcInven.WearItem[n_WearSlot_Gloves] )
	{
		//	Item_Gloves = 	g_Pc_Manager.GetItemBaseInfoPtr(  nRui->thePcInven.WearItem[n_WearSlot_Gloves]->Code  );
		if( nRui->thePcInven.WearItem[n_WearSlot_Gloves]->ItemTable->theSetID == thePcItem->ItemTable->theSetID )	
		{
			++SetItemCnt; 
			theWearSetItem[2].color = GetRareColor( thePcItem->ItemTable->theRare);	
		}
		
	}
	//신발 
	if(nRui->thePcInven.WearItem[n_WearSlot_Shoes] )
	{
		//Item_Shoes = 	g_Pc_Manager.GetItemBaseInfoPtr(  nRui->thePcInven.WearItem[n_WearSlot_Shoes]->Code  );
		
		if( nRui->thePcInven.WearItem[n_WearSlot_Shoes]->ItemTable->theSetID == thePcItem->ItemTable->theSetID )
		{
			++SetItemCnt;
			theWearSetItem[3].color = GetRareColor( thePcItem->ItemTable->theRare);	
		}
		
	}
	
	//SetID 명성탬 4개 셋을 보유 했을경우 . 
	//SetID 명성탬 1개 셋을 보유 했을경우 . 
	//SetID 일반 템 구분 어떻게 ??? 
	if(SetItemCnt == 0 )
	{
		return; 
	}
	
	//명성셋 1개 이상 보유 했을시 .. 
	else if(SetItemCnt > 0 && SetItemCnt < 4 )
	{
		for( int i = 0; i < WearNumLine; ++i )
		{
			//	theWearDesc[i].color = D3DCOLOR_XRGB( 240, 120, 142 );
			theWearDesc[i].SetMode = 1 ;
		}
	}
	
	//명성셋 4개 보유 ..  
	else if(SetItemCnt == 4 )
	{
		//설명 
		for( int i = 0; i < WearNumLine; ++i )
		{
			theWearDesc[i].SetMode = 2 ;
		}
	}
	
}

//보통 인벤토리 에 있을때는 앞부분 만 .. 색깔 이 변한다 . 
//인벤토리 있는 세트 아이템만 .. ?? 
void CHelpItem::SetItemInventory()
{
	
	SPcDataInven* m_PcInven = NULL;
	m_PcInven = nRui->GetPcInvenInfo();
	
	if( thePcItem->ItemTable->theSetID != 0 )
	{
		for( int i = 0; i < WearNumLine; ++i )
		{
			theWearDesc[i].SetMode = 1 ;
		}
	}
	
	for( int i = 0; i < c_MaxInvenItemNum; ++i )
	{
		if( m_PcInven->InvenItem[i] == NULL )
		{
			continue;
		}
		
		if ( m_PcInven->InvenItem[i] && 
			(m_PcInven->InvenItem[i]->ItemTable->theSetID == thePcItem->ItemTable->theSetID ) ) 
		{
			switch(m_PcInven->InvenItem[i]->Class) 
			{
			case n_Class_CoatArmor:	
				theWearSetItem[0].color = GetRareColor( thePcItem->ItemTable->theRare);
				break;
			case n_Class_PantsArmor:
				theWearSetItem[1].color = GetRareColor( thePcItem->ItemTable->theRare);
				break;
			case n_Class_Gloves:
				theWearSetItem[2].color = GetRareColor( thePcItem->ItemTable->theRare);
				break;
			case n_Class_Shoes:
				theWearSetItem[3].color = GetRareColor( thePcItem->ItemTable->theRare);
				break;
			default:
				break;
			}
			
		}
		
	}
	
}

//-----------------------------------------------------------------------------
void CHelpItem::SetTipEtc()
{
	theTipType = n_TIP_ITEM_ETC;
	
	/*
	if( ((CSlot*)theSlotWnd)->Group == n_SGROUP_ELDA_MIX_PRE0 )
	{
		if( EldaStoneMix(0))
		{
			return;
		}
	}
	else if( ((CSlot*)theSlotWnd)->Group == n_SGROUP_ELDA_MIX_PRE1 )
	{
		
		if( EldaStoneMix(1))
		{
			return;
		}
		
	}
	*/

	
	int ClWidth = 0;
	int nMaxStrWidthNum = 28;
	
	theClient.h = BV;
	//@@@@ char	theName[20];		// 아이템명....
	//-------------------------------------------------------------------------
	
	Rsprintf( RWCHAR(theEtcName.str), _RT("%s"), RWCHAR(thePcItem->ItemTable->theName) );
	
	if(theFontg)
		theFontg->GetStringInfo( theEtcName.str, &theStrInfo );
	if( ClWidth < theStrInfo.Width )
        ClWidth = theStrInfo.Width;
	if( nMaxStrWidthNum < theStrInfo.nCLen )
		nMaxStrWidthNum = theStrInfo.nCLen;
	
	theEtcName.x = 5;
	//theEtcName.x = GetAlignCenterx( &theStrInfo );
	theEtcName.y = theClient.h;
	//by simwoosung
	
	if(thePcItem->WearTable.theCode || (thePcItem->Class == n_Class_Eldastone &&  (thePcItem->Code > 60000))) //추가정보가 있을경우 
	{
		SetTipApplyEldastone( theEtcName.x, theEtcName.y, 
			(int)theStrInfo.Width, ClWidth ); //적용엘다스톤 정보
	}
	theClient.h += RV;
	//-------------------------------------------------------------------------		
	theClient.h += BV;
	//@@@@ 장비품 레벨 -- theGrade
	if( thePcItem->Class == n_Class_Use )
	{
		Rsprintf( RWCHAR(theEtcGrade.str), _RT("%s %s %d"), G_STRING_CHAR( IDS_ITEM_USE ), G_STRING_CHAR( IDS_CHARSTATE_LEVEL ), thePcItem->ItemTable->theGrade / 100 );
		if(theFontg)
			theFontg->GetStringInfo( theEtcGrade.str, &theStrInfo );
		if( ClWidth < theStrInfo.Width )
            ClWidth = theStrInfo.Width;
		if( nMaxStrWidthNum < theStrInfo.nCLen )
			nMaxStrWidthNum = theStrInfo.nCLen;
		
		theEtcGrade.x = 5;
		//theEtcGrade.x = GetAlignCenterx( &theStrInfo );
		theEtcGrade.y = theClient.h;
		theClient.h += RV;
	}
	
	//-----------------------------------------------------------------------------		
	if( (thePcItem->ItemTable->theNeedSkill) || (thePcItem->ItemTable->theLimitLevel) )
	{	
		//@@@@ 장비품 필요 스킬, 레벨 -- theNeed
		TCHAR NeedSkill[256] = {0,};
		TCHAR NeedLevel[256] = {0,};
		
		if(thePcItem->ItemTable->theNeedSkill)
		{
			SSkillBaseInfo *pSkill_Info = NULL;
			pSkill_Info = g_Pc_Manager.GetSkillBaseInfoPtr((DWORD)thePcItem->ItemTable->theNeedSkill);
			if(pSkill_Info)
			{
				Rsprintf( RWCHAR(NeedSkill), _RT("%s: (%s) "), G_STRING_CHAR( IDS_WND_NEED_SKILL ), RWCHAR(pSkill_Info->theSkillName) );		        
				Rstrcat( RWCHAR(theEtcNeed.str), RWCHAR(NeedSkill) );
			}		
		}
		
		if(thePcItem->ItemTable->theLimitLevel)
		{
			Rsprintf( RWCHAR(NeedSkill), _RT("%s (%d) "), G_STRING_CHAR( IDS_WND_NEED_LEVEL ), thePcItem->ItemTable->theLimitLevel );		
			Rstrcat( RWCHAR(theEtcNeed.str), RWCHAR(NeedSkill) );
		}		
		
		if(theFontg)
			theFontg->GetStringInfo( theEtcNeed.str, &theStrInfo );
		if( ClWidth < theStrInfo.Width )
			ClWidth = theStrInfo.Width;
		if( nMaxStrWidthNum < theStrInfo.nCLen )
			nMaxStrWidthNum = theStrInfo.nCLen;
		
		theEtcNeed.x = 5;
		//theEtcNeed.x = GetAlignCenterx( &theStrInfo );
		theEtcNeed.y = theClient.h;
		theClient.h += RV;		
	}			
	
	//if(thePcItem->ItemTable->theMaxEndurance == 0  && thePcItem->ItemTable->theRare == 255 ) 쓰지않아서 주석 처리함
#ifdef KEEPUP_PAYITEM_RENEWAL

	if(thePcItem->ItemTable->theClass == n_Class_Use && thePcItem->ItemTable->theType == n_Type_Scroll && thePcItem->IType == n_ITYPE_SPECIAL 
		&& (thePcItem->ItemTable->theReGage || thePcItem->ItemTable->theReMPSpeed || thePcItem->ItemTable->theReSPSpeed ||thePcItem->ItemTable->theReHPSpeed ) )
	{	
		g_ListTime.GetTimeEffectStr( thePcItem->Code , theEtcTime.str );
		if(theFontg)
			theFontg->GetStringInfo( theEtcTime.str, &theStrInfo );
		if( ClWidth < theStrInfo.Width )
			ClWidth = theStrInfo.Width;
		if( nMaxStrWidthNum < theStrInfo.nCLen )
			nMaxStrWidthNum = theStrInfo.nCLen;
		
		theEtcTime.x = 5;
		//theEtcNeed.x = GetAlignCenterx( &theStrInfo );
		theEtcTime.y = theClient.h;
		theClient.h += RV;		
	}
#endif
#ifdef PREMIUMZONE_KEY_HELPTIP
	if(thePcItem->ItemTable->theMaxEndurance == 0  && thePcItem->ItemTable->theRare == 255 )
	{	
		g_ListTime.GetTimeEffectStr( thePcItem->Code , theEtcTime.str );
		//g_ListTime.GetTimeRemainStr(thePcItem->ItemUID , theEtcTime.str );
		if(theFontg)
			theFontg->GetStringInfo( theEtcTime.str, &theStrInfo );
		if( ClWidth < theStrInfo.Width )
			ClWidth = theStrInfo.Width;
		if( nMaxStrWidthNum < theStrInfo.nCLen )
			nMaxStrWidthNum = theStrInfo.nCLen;
		
		theEtcTime.x = 5;
		//theEtcNeed.x = GetAlignCenterx( &theStrInfo );
		theEtcTime.y = theClient.h;
		theClient.h += RV;		
	}
#endif //PREMIUMZONE_KEY_HELPTIP	


#ifdef PET_PAY_USEITEM_HELPTIP ////펫 유료 아이템 지속시간 표시(헬프팁)			
	if(thePcItem->ItemTable->theClass == n_Class_PersonalPet && thePcItem->ItemTable->theType == n_Type_Scroll && thePcItem->IType == n_ITYPE_SPECIAL 
		&& (thePcItem->ItemTable->theReGage || thePcItem->ItemTable->theReMPSpeed || thePcItem->ItemTable->theReSPSpeed ||thePcItem->ItemTable->theReHPSpeed ) )
	{	

		g_PetInfoWnd.GetTimePetEffectStr(thePcItem->Code , theEtcTime.str );
		if(theFontg)
			theFontg->GetStringInfo( theEtcTime.str, &theStrInfo );
		if( ClWidth < theStrInfo.Width )
			ClWidth = theStrInfo.Width;
		if( nMaxStrWidthNum < theStrInfo.nCLen )
			nMaxStrWidthNum = theStrInfo.nCLen;
		
		theEtcTime.x = 5;
		//theEtcNeed.x = GetAlignCenterx( &theStrInfo );
		theEtcTime.y = theClient.h;
		theClient.h += RV;		
	}

#endif// PET_PAY_USEITEM_HELPTIP	



	//-------------------------------------------------------------------------	
	//@@@@ char	theDesc[100];		// 설명
	/*int NumStr = 0;
	if(theFontg)
	NumStr		= theFontg->GetStringCount( thePcItem->ItemTable->theDesc );
	EtcNumLine		= GetNumLine( NumStr, 20 );
	DescStartpos	= 0;
	
	  for( int i = 0; i < EtcNumLine; ++i )
	  {
	  if(theFontg)
	  theFontg->GetStringDistribute( theEtcDesc[i].str, &thePcItem->ItemTable->theDesc[DescStartpos] );
	  DescStartpos += _tcslen( theEtcDesc[i].str );
	  
		if(theFontg)
		theFontg->GetStringInfo( theEtcDesc[i].str, &theStrInfo );
		if( ClWidth < theStrInfo.Width )
		ClWidth = theStrInfo.Width;
		theEtcDesc[i].x = 5;
		//theEtcDesc[i].x = GetAlignCenterx( &theStrInfo );
		theEtcDesc[i].y = theClient.h;
		theClient.h += RV;
	}*/
	//-------------------------------------------------------------------------
	
	theClient.h += BV;
	//@@@@ DWORD	thePrice;		// 가격 
	//@@@@ BYTE		theBundle;		// 번들가능 갯수, 1=번들불가
	//@@@@ BYTE		theWeight;		// 무게
	//	_stprintf( theEtcBWP.str, "최대묶음수:%d, 무게:%d", thePcItem->ItemTable->theBundle, thePcItem->ItemTable->theWeight );
	Rsprintf( RWCHAR(theEtcBWP.str), _RT("%s:%d"), G_STRING_CHAR( IDS_ITEM_MAX_BUNDLE ), thePcItem->ItemTable->theBundle);
	//_stprintf( theEtcBWP.str, "최대묶음수:%d, 무게:%d, 가격:%dEld", thePcItem->ItemTable->theBundle, thePcItem->ItemTable->theWeight, thePcItem->ItemTable->thePrice );
	if(theFontg)
		theFontg->GetStringInfo( theEtcBWP.str, &theStrInfo );
	if( ClWidth < theStrInfo.Width )
        ClWidth = theStrInfo.Width;
	if( nMaxStrWidthNum < theStrInfo.nCLen )
		nMaxStrWidthNum = theStrInfo.nCLen;
	
	theEtcBWP.x = 5;
	//theEtcBWP.x = GetAlignCenterx( &theStrInfo );
	theEtcBWP.y = theClient.h;
	theClient.h += RV;
	//-------------------------------------------------------------------------
	TCHAR strEld[256]="";
	if( ( StoreWnd.theDCRate > 0 ) && ( ((CSlot*)theSlotWnd)->Group == n_SGROUP_STORE ) )
	{
		int price = thePcItem->ItemTable->thePrice - ( (int)( thePcItem->ItemTable->thePrice * ( StoreWnd.theDCRate * 0.01f ) ) + 1 );
		
		Rsprintf( RWCHAR(strEld), G_STRING_CHAR( IDS_WND_REPAIR_ELDA ), price );
		Rsprintf( RWCHAR(theEtcPrice.str), _RT("%s:%s Sale:%d%%"),  G_STRING_CHAR( IDS_ITEM_COST ), RWCHAR(strEld ), StoreWnd.theDCRate );	
		
	}
	else
	{
		Rsprintf( RWCHAR(strEld), G_STRING_CHAR( IDS_WND_REPAIR_ELDA ), thePcItem->ItemTable->thePrice );
		Rsprintf( RWCHAR(theEtcPrice.str), _RT("%s:%s"),G_STRING_CHAR( IDS_ITEM_COST ), RWCHAR(strEld ));
	}
	
	if(theFontg)
		theFontg->GetStringInfo( theEtcPrice.str, &theStrInfo );
	if( ClWidth < theStrInfo.Width )
        ClWidth = theStrInfo.Width;
	if( nMaxStrWidthNum < theStrInfo.nCLen )
		nMaxStrWidthNum = theStrInfo.nCLen;
	
	theEtcPrice.x = 5;
	//theWearPrice.x = GetAlignCenterx( &theStrInfo );
	theEtcPrice.y = theClient.h;
	theClient.h += RV;
	

	bool IsDesc = false;	
	theClient.h += BV;
	
	EtcNumLine = 0;


#ifdef UNICODE_ROCK
	RTCHAR *pStrIn = RWCHAR(thePcItem->ItemTable->theDesc);
	while(pStrIn)
	{
		TCHAR TempBuf[256];
		memset(TempBuf, NULL, sizeof(TCHAR) * 256 );
		
		pStrIn = GetStringReturnInStringW('\\', true, nMaxStrWidthNum, pStrIn, TempBuf);
		if( Rstrcmp(TempBuf, _RT("") ) != 0 )
		{
			SAFE_STR_CPY(theEtcDesc[EtcNumLine].str, TempBuf, 256);
			if(theFontg)
				theFontg->GetStringInfo( theEtcDesc[EtcNumLine].str, &theStrInfo );
			if( ClWidth < theStrInfo.Width )
				ClWidth = theStrInfo.Width;
			theEtcDesc[EtcNumLine].x = 5;
			//theWeaponDesc[i].x = GetAlignCenterx( &theStrInfo );
			theEtcDesc[EtcNumLine].y = theClient.h;
			theClient.h += RV;
			
			EtcNumLine++;
			IsDesc = true;
		}
	}

#else

	TCHAR *pStrIn = (TCHAR *)thePcItem->ItemTable->theDesc;
	while(pStrIn)
	{
		TCHAR TempBuf[256];
		memset(TempBuf, NULL, sizeof(TCHAR) * 256 );
		
		pStrIn = GetStringReturnInString('\\', true, nMaxStrWidthNum, pStrIn, TempBuf);
		if( Rstrcmp(TempBuf, _RT("") ) != 0 )
		{
			SAFE_STR_CPY(theEtcDesc[EtcNumLine].str, TempBuf, 256);
			if(theFontg)
				theFontg->GetStringInfo( theEtcDesc[EtcNumLine].str, &theStrInfo );
			if( ClWidth < theStrInfo.Width )
				ClWidth = theStrInfo.Width;
			theEtcDesc[EtcNumLine].x = 5;
			//theWeaponDesc[i].x = GetAlignCenterx( &theStrInfo );
			theEtcDesc[EtcNumLine].y = theClient.h;
			theClient.h += RV;
			
			EtcNumLine++;
			IsDesc = true;
		}
	}
#endif	
	if(IsDesc)
	{
		theClient.h += BV;
	}
	
	if( (ClWidth + 10) >= MAX_ITEMTIP_WIDTH )
		ClWidth = MAX_ITEMTIP_WIDTH;
	else
		ClWidth += 10;
    
	theClient.w = ClWidth;
}

SItemBaseInfo * CHelpItem::SetTipApplyEldastone(int iPosX, int iPosY, int iNameWidth, int & ClWidth)
{
	TCHAR strCategory[256] = {0,};
	TCHAR strMixCategory[256] = {0,};
	TCHAR strTemp[256] = {0,};
	
	WORD iEldaCode = 0;//[0];	
	
	SItemBaseInfo *pItemBaseInfo  = NULL;
	SItemBaseInfo *pItemBaseInfoMixElda = NULL ; 
	thebMixEldaStone = false; 
	bool baseMixEldaStone = true ; 
		
	//엘다스톤만 체크 했을시 
	if(thePcItem->Code > 60000 )
	{		
		pItemBaseInfoMixElda = g_Pc_Manager.GetItemBaseInfoPtr( thePcItem->Code );
		pItemBaseInfo = g_Pc_Manager.GetItemBaseInfoPtr( pItemBaseInfoMixElda->theAttackSuccSnd3 );
				
	}
	//무기나 방어구 체크시 
	else 
	{
		
		iEldaCode = thePcItem->WearTable.theEldaStone;
		if(iEldaCode > 60000 )
		{
			thebMixEldaStone = true; 
			pItemBaseInfoMixElda = g_Pc_Manager.GetItemBaseInfoPtr( iEldaCode );
			pItemBaseInfo = g_Pc_Manager.GetItemBaseInfoPtr( pItemBaseInfoMixElda->theAttackSuccSnd3 );
			if(pItemBaseInfo == NULL )
			{
				pItemBaseInfo = pItemBaseInfoMixElda;
				baseMixEldaStone = false; 
			}
			
		}
		else 
		{
			pItemBaseInfo = g_Pc_Manager.GetItemBaseInfoPtr( iEldaCode );
		}
		
	}
		

	if( NULL == pItemBaseInfo )
	{
		return NULL;
	}
	
	if( n_Class_Eldastone != pItemBaseInfo->theClass )
	{
		return NULL;
	}
	
	D3DCOLOR  InfoColor;
	D3DCOLOR  MixInfoColor;

	if(pItemBaseInfo->theVital)
	{
		//녹색
		Rstrcpy(RWCHAR(strCategory), _RT("GR"));						
		theEldaColor = D3DCOLOR_XRGB( 0, 255, 0 );
		InfoColor = D3DCOLOR_XRGB( 0, 0, 0);
	}
	else if(pItemBaseInfo->theMinMagicBonus && pItemBaseInfo->theMaxMagicBonus)
	{
		//푸른색
		Rstrcpy(RWCHAR(strCategory), _RT("BL"));						
		theEldaColor = D3DCOLOR_XRGB( 0, 0, 255 );
		InfoColor = D3DCOLOR_XRGB( 255, 255, 255 );
	}
	else if(pItemBaseInfo->theSympathy)
	{
		//노란색
		Rstrcpy(RWCHAR(strCategory), _RT("YE"));						
		theEldaColor = D3DCOLOR_XRGB( 255, 255, 0 );
		InfoColor = D3DCOLOR_XRGB( 0, 0, 0);
	}
	else if(pItemBaseInfo->theMinAttackBonus && pItemBaseInfo->theMaxAttackBonus)
	{
		//붉은색
		Rstrcpy(RWCHAR(strCategory), _RT("RD"));							
		theEldaColor = D3DCOLOR_XRGB( 255, 0, 0 );
		InfoColor = D3DCOLOR_XRGB( 255, 255, 255);	
	}
	else if(pItemBaseInfo->theInt)
	{
		//보라색
		Rstrcpy(RWCHAR(strCategory), _RT("PP"));						
		theEldaColor = D3DCOLOR_XRGB( 74, 0, 127 );
		InfoColor = D3DCOLOR_XRGB( 255, 255, 255);
	}
	else if(pItemBaseInfo->theMagicRes)
	{
		//자주색
		Rstrcpy(RWCHAR(strCategory), _RT("VO"));						
		theEldaColor = D3DCOLOR_XRGB( 255, 42, 134);
		InfoColor = D3DCOLOR_XRGB( 255, 255, 255);
	}
	else if(pItemBaseInfo->theStamina)
	{
		//남색
		Rstrcpy(RWCHAR(strCategory), _RT("IN"));						
		theEldaColor = D3DCOLOR_XRGB( 0, 13, 104);
		InfoColor = D3DCOLOR_XRGB( 255, 255, 255);
	}
	else if(pItemBaseInfo->theDex)
	{
		//은색
		Rstrcpy(RWCHAR(strCategory), _RT("SL"));						
		theEldaColor = D3DCOLOR_XRGB( 181, 181, 181);
		InfoColor = D3DCOLOR_XRGB( 0, 0, 0);
	}
	else if(pItemBaseInfo->theAttackDef)
	{
		//연두색	
		Rstrcpy(RWCHAR(strCategory), _RT("LG"));						
		theEldaColor = D3DCOLOR_XRGB( 148, 230, 68);
		InfoColor = D3DCOLOR_XRGB( 0, 0, 0);
	}
	else if(pItemBaseInfo->thePower)
	{
		//주황색
		Rstrcpy(RWCHAR(strCategory), _RT("OR"));						
		theEldaColor = D3DCOLOR_XRGB( 255, 125, 0);
		InfoColor = D3DCOLOR_XRGB( 0, 0, 0);
	}
	
	int iLevel = pItemBaseInfo->theGrade/100;

	if(thebMixEldaStone)
	{
		
		int iWidthTemp = 0 ; 
		int iWidthStr  = 0 ; 
		//붉은 
		//연두
		//푸른
		//자주 

		Rstrcpy(RWCHAR(strMixCategory), _RT("MIX"));
			
		if( pItemBaseInfoMixElda->theCode > 60000 &&  pItemBaseInfoMixElda->theCode < 61000)
		{
			theMixEldaColor = D3DCOLOR_XRGB( 255, 0, 0 );
			MixInfoColor = D3DCOLOR_XRGB( 255, 255, 255);	
		}
		else if( pItemBaseInfoMixElda->theCode > 61000 &&  pItemBaseInfoMixElda->theCode < 62000)
		{
			theMixEldaColor = D3DCOLOR_XRGB( 148, 230, 68);
			MixInfoColor = D3DCOLOR_XRGB( 255, 255, 255);
		}
		else if( pItemBaseInfoMixElda->theCode > 62000 &&  pItemBaseInfoMixElda->theCode < 63000)
		{
			theMixEldaColor =D3DCOLOR_XRGB( 0, 0, 255 );
			MixInfoColor = D3DCOLOR_XRGB( 255, 255, 255 );
		}
		else if( pItemBaseInfoMixElda->theCode > 62000 &&  pItemBaseInfoMixElda->theCode < 63000)
		{
			theMixEldaColor = D3DCOLOR_XRGB( 255, 42, 134);
			MixInfoColor = D3DCOLOR_XRGB( 255, 255, 255);
		}
			
		Rsprintf(RWCHAR(strTemp), _RT("%s%d"), RWCHAR(strMixCategory), iLevel);
		Rstrcpy(RWCHAR(theAppMixEldTemp.str), _RT(" + "));

		theAppMixEldTemp.x = (iPosX + iNameWidth);
		theAppMixEldTemp.y = iPosY;
		theAppMixEldTemp.color = D3DCOLOR_XRGB( 255, 255, 255);
		
		if(theFontg)
			theFontg->GetStringInfo( theAppMixEldTemp.str, &theStrInfo );
		
		int itotWidth = (iNameWidth) + theStrInfo.Width;
		
		iWidthTemp = theStrInfo.Width;

		Rstrcpy(RWCHAR(theAppMixEldInfo.str), RWCHAR(strTemp));
		theAppMixEldInfo.x = (iPosX + itotWidth);
		theAppMixEldInfo.y = iPosY;
		theAppMixEldInfo.color = MixInfoColor;
		
		if(theFontg)
			theFontg->GetStringInfo( theAppMixEldInfo.str, &theStrInfo );
		
		itotWidth += theStrInfo.Width;
		iWidthStr =  theStrInfo.Width;


		if( ClWidth < itotWidth )
			ClWidth = itotWidth;
		//////////////////////////////////////////////////////////////////////////
		// 보통 엘다스톤 
		//////////////////////////////////////////////////////////////////////////
		if( baseMixEldaStone )
		{
			Rsprintf(RWCHAR(strTemp), _RT("%s%d"), RWCHAR(strCategory), iLevel);
			Rstrcpy(RWCHAR(theAppEldTemp.str), _RT(" + "));
			
			//theAppEldTemp.str   :  +
			//theAppEldinfo.str   : OR 색깔표시 
			
			theAppEldTemp.x = (iPosX + itotWidth  );
			theAppEldTemp.y = iPosY;
			theAppEldTemp.color = D3DCOLOR_XRGB( 255, 255, 255);
			
			if(theFontg)
				theFontg->GetStringInfo( theAppEldTemp.str, &theStrInfo );
			
			itotWidth += theStrInfo.Width; 
			
			Rstrcpy(RWCHAR(theAppEldInfo.str), RWCHAR(strTemp));
			theAppEldInfo.x = (iPosX + itotWidth );
			theAppEldInfo.y = iPosY;
			theAppEldInfo.color = InfoColor;
			
			if(theFontg)
				theFontg->GetStringInfo( theAppEldInfo.str, &theStrInfo );
			
			itotWidth += theStrInfo.Width;
			
			if( ClWidth < itotWidth )
				ClWidth = itotWidth;
		}
		
		DWORD itemCode = iEldaCode;
		return g_Pc_Manager.GetItemBaseInfoPtr( itemCode );
	}


	Rsprintf(RWCHAR(strTemp), _RT("%s%d"), RWCHAR(strCategory), iLevel);
	Rstrcpy(RWCHAR(theAppEldTemp.str), _RT(" + "));

	//theAppEldTemp.str   :  +
	//theAppEldinfo.str   : OR 색깔표시 
	
	theAppEldTemp.x = (iPosX + iNameWidth);
	theAppEldTemp.y = iPosY;
	theAppEldTemp.color = D3DCOLOR_XRGB( 255, 255, 255);
	
	if(theFontg)
		theFontg->GetStringInfo( theAppEldTemp.str, &theStrInfo );
	
	int itotWidth = (iNameWidth) + theStrInfo.Width;
	
	Rstrcpy(RWCHAR(theAppEldInfo.str), RWCHAR(strTemp));
	theAppEldInfo.x = (iPosX + itotWidth);
	theAppEldInfo.y = iPosY;
	theAppEldInfo.color = InfoColor;
	
	if(theFontg)
		theFontg->GetStringInfo( theAppEldInfo.str, &theStrInfo );
	
	itotWidth += theStrInfo.Width;
	
	if( ClWidth < itotWidth )
		ClWidth = itotWidth;
	
	DWORD itemCode = iEldaCode;
	return g_Pc_Manager.GetItemBaseInfoPtr( itemCode );
}

//-----------------------------------------------------------------------------
void CHelpItem::SetTipSkill()
{
	
	
	
	theTipType = n_TIP_SKILL;
	
	int ClWidth = 0;
	int nMaxStrWidthNum = 28;
	
	theClient.h = BV;
	//@@@@ char	theSkillName[20];		// 스킬명
	Rsprintf( RWCHAR(theSkillName.str), _RT("%s"), RWCHAR(thePcItem->SkillTable->theSkillName) );
	if(theFontg)
		theFontg->GetStringInfo( theSkillName.str, &theStrInfo );
	if( ClWidth < theStrInfo.Width )
        ClWidth = theStrInfo.Width;
	if( nMaxStrWidthNum < theStrInfo.nCLen )
		nMaxStrWidthNum = theStrInfo.nCLen;
	
	theSkillName.x = 5;
	//theSkillName.x = GetAlignCenterx( &theStrInfo );
	theSkillName.y = theClient.h;
	theClient.h += RV;
	
	bool bTempFlag = false;
	//-----------------------------------------------------------------------------		
	if( (thePcItem->SkillTable->thePreSkill1) || (thePcItem->SkillTable->thePreSkill2) )
	{	
		theClient.h += BV;
		
		bTempFlag = true;
		
		TCHAR NeedSkill[256] = {0,};
		
		if(thePcItem->SkillTable->thePreSkill1)
		{
			SSkillBaseInfo *pSkill_Info = NULL;
			pSkill_Info = g_Pc_Manager.GetSkillBaseInfoPtr((DWORD)thePcItem->SkillTable->thePreSkill1);
			if(pSkill_Info)
			{
				Rsprintf( RWCHAR(NeedSkill), _RT("%s (%s) "), G_STRING_CHAR( IDS_WND_NEED_SKILL ), RWCHAR(pSkill_Info->theSkillName) );
				Rstrcat( RWCHAR(theSkillNeed.str), RWCHAR(NeedSkill) );
				
				if(thePcItem->SkillTable->thePreSkill2)
				{
					Rstrcat( RWCHAR(theSkillNeed.str), G_STRING_CHAR( IDS_WND_OR ) );
				}
			}		
		}
		
		if(thePcItem->SkillTable->thePreSkill2)
		{
			SSkillBaseInfo *pSkill_Info = NULL;
			pSkill_Info = g_Pc_Manager.GetSkillBaseInfoPtr((DWORD)thePcItem->SkillTable->thePreSkill2);
			if(pSkill_Info)
			{
				if(thePcItem->SkillTable->thePreSkill1)
				{
					Rsprintf( RWCHAR(NeedSkill), _RT("(%s) "), RWCHAR(pSkill_Info->theSkillName) );				
					Rstrcat( RWCHAR(theSkillNeed.str), RWCHAR(NeedSkill) );
				}
				else
				{
					Rsprintf( RWCHAR(NeedSkill), G_STRING_CHAR( IDS_WND_NEED_SKILL ), RWCHAR(pSkill_Info->theSkillName ));		       
					Rstrcat( RWCHAR(theSkillNeed.str), RWCHAR(NeedSkill) );
				}
			}
		}
		
		if(theFontg)
			theFontg->GetStringInfo( theSkillNeed.str, &theStrInfo );
		if( ClWidth < theStrInfo.Width )
			ClWidth = theStrInfo.Width;
		if( nMaxStrWidthNum < theStrInfo.nCLen )
			nMaxStrWidthNum = theStrInfo.nCLen;
		
		theSkillNeed.x = 5;
		//theSkillNeed.x = GetAlignCenterx( &theStrInfo );
		theSkillNeed.y = theClient.h;		
	}
	
	if( thePcItem->SkillTable->theToggleFlag )
	{
		theClient.h += BV;		
		bTempFlag = true;
		
		Rstrcpy(RWCHAR(theSkillToggle.str), G_STRING_CHAR( IDS_WND_TOGGLE_SKILL ) );
		if(theFontg)
			theFontg->GetStringInfo( theSkillToggle.str, &theStrInfo );
		if( ClWidth < theStrInfo.Width )
			ClWidth = theStrInfo.Width;
		if( nMaxStrWidthNum < theStrInfo.nCLen )
			nMaxStrWidthNum = theStrInfo.nCLen;
		
		theSkillToggle.x = 5;
		//theSkillToggle.x = GetAlignCenterx( &theStrInfo );
		theSkillToggle.y = theClient.h;
		
	}
	
	if(bTempFlag)
	{
		theClient.h += RV;
	}
	
	//@@@@ char	theDesc[100];			// 설명
	/*int NumStr = 0;
	if(theFontg)
	NumStr		= theFontg->GetStringCount( thePcItem->SkillTable->theDesc );
	SkillNumLine	= GetNumLine( NumStr, 20 );
	DescStartpos	= 0;
	
	  for( int i = 0; i < SkillNumLine; ++i )
	  {
	  if(theFontg)
	  theFontg->GetStringDistribute( theSkillDesc[i].str, &thePcItem->SkillTable->theDesc[DescStartpos] );
	  DescStartpos += _tcslen( theSkillDesc[i].str );
	  
		if(theFontg)
		theFontg->GetStringInfo( theSkillDesc[i].str, &theStrInfo );
		if( ClWidth < theStrInfo.Width )
		ClWidth = theStrInfo.Width;
		theSkillDesc[i].x = 5;
		//theSkillDesc[i].x = GetAlignCenterx( &theStrInfo );
		theSkillDesc[i].y = theClient.h;
		theClient.h += RV;
	}*/
	//-------------------------------------------------------------------------	
	
	theClient.h += BV;
	//@@@@ BYTE	theNeedItem;// 필요장비(마법도구, 아이템테이블 중 Type 참조 )
	//@@@@ BYTE	theSkillLevel;			// 스킬숙련도	1 ~ 10
	int iSkillLevel = g_SkillLevelUpWnd.GetActiveSKillLevel(thePcItem->SkillTable);		
		
	switch( thePcItem->SkillTable->theNeedItem )
	{
	case 0:	Rsprintf( RWCHAR(theSkillItem.str), _RT("%s:%d"),G_STRING_CHAR( IDS_WND_SKILL_AB ), iSkillLevel ); break;								
	case 1: Rsprintf( RWCHAR(theSkillItem.str), _RT("%s:%s, %s:%d"), G_STRING_CHAR( IDS_WND_HT_NEEDITEM ), G_STRING_CHAR( IDS_ITEM_TYPE_SWORD ), G_STRING_CHAR( IDS_WND_SKILL_AB ), iSkillLevel ); break;					
	case 2: Rsprintf( RWCHAR(theSkillItem.str), _RT("%s:%s, %s:%d"),G_STRING_CHAR( IDS_WND_HT_NEEDITEM ), G_STRING_CHAR( IDS_ITEM_TYPE_GSWORD ), G_STRING_CHAR( IDS_WND_SKILL_AB ), iSkillLevel ); break;				
	case 3: Rsprintf( RWCHAR(theSkillItem.str), _RT("%s:%s, %s:%d"), G_STRING_CHAR( IDS_WND_HT_NEEDITEM ), G_STRING_CHAR( IDS_ITEM_TYPE_BOW ), G_STRING_CHAR( IDS_WND_SKILL_AB ), iSkillLevel ); break;					
	case 4: Rsprintf( RWCHAR(theSkillItem.str), _RT("%s:%s, %s:%d"), G_STRING_CHAR( IDS_WND_HT_NEEDITEM ), G_STRING_CHAR( IDS_ITEM_TYPE_MBOOK ), G_STRING_CHAR( IDS_WND_SKILL_AB ), iSkillLevel ); break;				
	case 5: Rsprintf( RWCHAR(theSkillItem.str), _RT("%s:%s, %s:%d"), G_STRING_CHAR( IDS_WND_HT_NEEDITEM ), G_STRING_CHAR( IDS_ITEM_TYPE_STAFF ), G_STRING_CHAR( IDS_WND_SKILL_AB ), iSkillLevel ); break;				
	case 6: Rsprintf( RWCHAR(theSkillItem.str), _RT("%s:%s, %s:%d"), G_STRING_CHAR( IDS_WND_HT_NEEDITEM ), G_STRING_CHAR( IDS_ITEM_TYPE_CRYBALL ), G_STRING_CHAR( IDS_WND_SKILL_AB ), iSkillLevel ); break;				
	case 7: Rsprintf( RWCHAR(theSkillItem.str), _RT("%s:%s, %s:%d"), G_STRING_CHAR( IDS_WND_HT_NEEDITEM ), G_STRING_CHAR( IDS_ITEM_TYPE_SHIELD ), G_STRING_CHAR( IDS_WND_SKILL_AB ),iSkillLevel ); break;				
	}	
	if(Rstrcmp(theSkillItem.str, _RT("") ) != 0 )
	{
		if(theFontg)
			theFontg->GetStringInfo( theSkillItem.str, &theStrInfo );
		if( ClWidth < theStrInfo.Width )
			ClWidth = theStrInfo.Width;
		if( nMaxStrWidthNum < theStrInfo.nCLen )
			nMaxStrWidthNum = theStrInfo.nCLen;
		
		theSkillItem.x = 5;
		//theSkillItem.x = GetAlignCenterx( &theStrInfo );
		theSkillItem.y = theClient.h;
		theClient.h += RV;
	}
	//-------------------------------------------------------------------------	
	
	//@@@@ BYTE	theGageType;			// 필요 게이지 종류 ( 0:none, 1:comboG, 2:conditionG, 3:proimaG )
	//@@@@ BYTE	theGageValue;			// 소모게이지 수치	1 ~ 50	
	if(  thePcItem->SkillTable->theGageValue != 0  )
	{
		int nStep = (thePcItem->SkillTable->theGageValue)/5;
		if(nStep)
		{
			
			Rsprintf( RWCHAR(theSkillGage.str), _RT("%s:%d%s"),	G_STRING_CHAR( IDS_WND_USE_GAUGE ), 
				nStep, G_STRING_CHAR( IDS_PARTITION_UNIT ) ); 
				/*			 
				switch( nRui->thePcParam.GageType )
				{
				case 0: _stprintf( theSkillGage.str, "%s:%d%s", 
				G_STRING_CHAR( IDS_WND_USE_GAUGE ), 
				nStep, G_STRING_CHAR( IDS_PARTITION_UNIT ) ); break;					
				case n_ComboGage: _stprintf( theSkillGage.str, "%s:%s, %s:%d%s",
				G_STRING_CHAR( IDS_WND_NEED_GAUGE ), 
				G_STRING_CHAR( IDS_WND_GAUGE_COMBO ), 
				G_STRING_CHAR( IDS_WND_USE_GAUGE ), 
				nStep, G_STRING_CHAR( IDS_PARTITION_UNIT ) ); break;						
				case n_ConditionGage: _stprintf( theSkillGage.str, "%s:%s, %s:%d%s",
				G_STRING_CHAR( IDS_WND_NEED_GAUGE ), 
				G_STRING_CHAR( IDS_WND_GAUGE_CONDITION ), 
				G_STRING_CHAR( IDS_WND_USE_GAUGE ), 
				nStep, G_STRING_CHAR( IDS_PARTITION_UNIT ) ); break;						
				case n_ProimaGage: _stprintf( theSkillGage.str, "%s:%s, %s:%d%s",
				G_STRING_CHAR( IDS_WND_NEED_GAUGE ), 
				G_STRING_CHAR( IDS_WND_GAUGE_PROIMA ), 
				G_STRING_CHAR( IDS_WND_USE_GAUGE ), 
				nStep, G_STRING_CHAR( IDS_PARTITION_UNIT ) ); break;						
				}
			*/
			
			if(theFontg)
				theFontg->GetStringInfo( theSkillGage.str, &theStrInfo );
			if( ClWidth < theStrInfo.Width )
				ClWidth = theStrInfo.Width;
			if( nMaxStrWidthNum < theStrInfo.nCLen )
				nMaxStrWidthNum = theStrInfo.nCLen;
			
			theSkillGage.x = 5;
			//theSkillGage.x = GetAlignCenterx( &theStrInfo );
			theSkillGage.y = theClient.h;
			theClient.h += RV;
			//---------------------------------------------------------------------
		}
	}
	
	
	//@@@@ WORD	theWasteHP;				// 소모 HP
	//@@@@ WORD	theWasteMP;				// 소모 MP
	//@@@@ WORD	theWasteSP;				// 소모 SP
	char wastehp[40] = {0,};
	char wastemp[40] = {0,};
	char wastesp[40] = {0,};
	if( ( thePcItem->SkillTable->theWasteHP != 0 ) || ( thePcItem->SkillTable->theWasteMP != 0 ) || ( thePcItem->SkillTable->theWasteSP != 0 ) )
	{
		if( thePcItem->SkillTable->theWasteHP != 0 )
		{	
			Rsprintf( RWCHAR(wastehp), _RT("%s:%d "), G_STRING_CHAR( IDS_WND_USE_HP ), thePcItem->SkillTable->theWasteHP ); 
			Rstrcat(RWCHAR( theSkillWastePoint.str), RWCHAR(wastehp) ); 
		}		
		if( thePcItem->SkillTable->theWasteMP != 0 )
		{
			Rsprintf( RWCHAR(wastemp), _RT("%s:%d "), G_STRING_CHAR( IDS_WND_USE_MP ),thePcItem->SkillTable->theWasteMP ); 
			Rstrcat(RWCHAR( theSkillWastePoint.str), RWCHAR(wastemp) ); 
		}		
		if( thePcItem->SkillTable->theWasteSP != 0 )
		{
			Rsprintf( RWCHAR(wastesp), _RT("%s:%d "), G_STRING_CHAR( IDS_WND_USE_SP ),thePcItem->SkillTable->theWasteSP ); 
			Rstrcat(RWCHAR( theSkillWastePoint.str), RWCHAR(wastesp) );
		}		
		
		//_stprintf( theSkillWastePoint.str, "%s %s %s ", wastehp, wastemp, wastesp );
		if(theFontg)
			theFontg->GetStringInfo( theSkillWastePoint.str, &theStrInfo );
		if( ClWidth < theStrInfo.Width )
            ClWidth = theStrInfo.Width;
		if( nMaxStrWidthNum < theStrInfo.nCLen )
			nMaxStrWidthNum = theStrInfo.nCLen;
		
		theSkillWastePoint.x = 5;
		//theSkillWastePoint.x = GetAlignCenterx( &theStrInfo );
		theSkillWastePoint.y = theClient.h;
		theClient.h += RV;
		//---------------------------------------------------------------------
	}
	
	//스킬 코드 추출
	int nSkillCode = thePcItem->SkillTable->theSkillCode;
	int nNextSkillCode = nSkillCode + 1;
	if( ( nNextSkillCode % 10 ) != 0 )	//다음 카테고리로 넘어가지 않는다면
	{
		SSkillBaseInfo *pSkill_Info = NULL;
		pSkill_Info = g_Pc_Manager.GetSkillBaseInfoPtr(nNextSkillCode);
		
		if( pSkill_Info	&& ( nNextSkillCode == pSkill_Info->theSkillCode ) )		//다음 스킬이 존재한다면..
		{
			
			int nNeedLevel = pSkill_Info->theNeedLevel;
			
			if(nNeedLevel)	//다음 스킬 필요레벨이 0이 아니라면
			{
				theClient.h += BV;

				Rsprintf( RWCHAR(theSkillUpLevel.str), _RT("%s: %s(%d) , %s(%d)"),G_STRING_CHAR( IDS_HELP_SKILL_UP_NEED_LEVEL ), G_STRING_CHAR(IDS_CHARSTATE_LEVEL),nNeedLevel ,
					G_STRING_CHAR( IDS_HELP_POINT ), pSkill_Info->theNeedPoint  );		
				if(theFontg)
					theFontg->GetStringInfo( theSkillUpLevel.str, &theStrInfo );
				if( ClWidth < theStrInfo.Width )
					ClWidth = theStrInfo.Width;
				if( nMaxStrWidthNum < theStrInfo.nCLen )
					nMaxStrWidthNum = theStrInfo.nCLen;
				theSkillUpLevel.x = 5;			
				theSkillUpLevel.y = theClient.h;
				theClient.h += RV;			
				
			}
			
		}
		
	}	
	
	// 주석 처리....
	/*
	//@@@@ long	theKeepupTime;			// 지속시간(초)		0 ~ 86400	-1:마법을 재사용시 기존 마법이 풀리는 처리
	if( thePcItem->SkillTable->theEffectInfo[0].theKeepupTime != 0 )
	{
	_stprintf( theSkillKeepupTime.str, "!지속시간:%d초", thePcItem->SkillTable->theEffectInfo[0].theKeepupTime );
	theFontg->GetStringInfo( theSkillKeepupTime.str, &theStrInfo );
	theSkillKeepupTime.x = GetAlignCenterx( &theStrInfo );
	theSkillKeepupTime.y = theClient.h;
	theClient.h += RV;
	//---------------------------------------------------------------------
	}
	//*/	
	/*
	//@@@@ BYTE	theProimaType;			// 27가지 프로이마계열 중 하나 (스킬사용시 착용한 마법도구와 연계)
	if( thePcItem->SkillTable->theProimaType != 0 )
	{
	switch( thePcItem->SkillTable->theProimaType )
	{
	case 1:		_stprintf( theSkillProimaType.str, "!필요 프로이마:프리" ); break;
	case 2:		_stprintf( theSkillProimaType.str, "!필요 프로이마:세스" ); break;
	case 3:		_stprintf( theSkillProimaType.str, "!필요 프로이마:란트" ); break;
	case 4:		_stprintf( theSkillProimaType.str, "!필요 프로이마:에아" ); break;
	case 5:		_stprintf( theSkillProimaType.str, "!필요 프로이마:바크" ); break;
	case 6:		_stprintf( theSkillProimaType.str, "!필요 프로이마:프리트" ); break;
	case 7:		_stprintf( theSkillProimaType.str, "!필요 프로이마:세아스" ); break;
	case 8:		_stprintf( theSkillProimaType.str, "!필요 프로이마:라이트" ); break;
	case 9:		_stprintf( theSkillProimaType.str, "!필요 프로이마:에아르" ); break;
	case 10:	_stprintf( theSkillProimaType.str, "!필요 프로이마:바사크" ); break;
	case 11:	_stprintf( theSkillProimaType.str, "!필요 프로이마:프리세트" ); break;
	case 12:	_stprintf( theSkillProimaType.str, "!필요 프로이마:세아시스" ); break;
	case 13:	_stprintf( theSkillProimaType.str, "!필요 프로이마:라이아트" ); break;
	case 14:	_stprintf( theSkillProimaType.str, "!필요 프로이마:에아리아" ); break;
	case 15:	_stprintf( theSkillProimaType.str, "!필요 프로이마:바라사크" ); break;
	case 16:	_stprintf( theSkillProimaType.str, "!필요 프로이마:파라세트" ); break;
	case 17:	_stprintf( theSkillProimaType.str, "!필요 프로이마:아이아스" ); break;
	case 18:	_stprintf( theSkillProimaType.str, "!필요 프로이마:그리아드" ); break;
	case 19:	_stprintf( theSkillProimaType.str, "!필요 프로이마:브리너스" ); break;
	case 20:	_stprintf( theSkillProimaType.str, "!필요 프로이마:케이아스" ); break;
	case 21:	_stprintf( theSkillProimaType.str, "!필요 프로이마:샤크텐" ); break;
	case 22:	_stprintf( theSkillProimaType.str, "!필요 프로이마:마하브" ); break;
	case 23:	_stprintf( theSkillProimaType.str, "!필요 프로이마:가이아" ); break;
	case 24:	_stprintf( theSkillProimaType.str, "!필요 프로이마:프리랏사" ); break;
	case 25:	_stprintf( theSkillProimaType.str, "!필요 프로이마:테사라트" ); break;
	case 30:	_stprintf( theSkillProimaType.str, "!필요 프로이마:카실라트" ); break;
	case 31:	_stprintf( theSkillProimaType.str, "!필요 프로이마:알바트론" ); break;
	}
	
	  if(theFontg)
	  theFontg->GetStringInfo( theSkillProimaType.str, &theStrInfo );
	  if( ClWidth < theStrInfo.Width )
	  ClWidth = theStrInfo.Width;
	  theSkillProimaType.x = 5;
	  //theSkillProimaType.x = GetAlignCenterx( &theStrInfo );
	  theSkillProimaType.y = theClient.h;
	  theClient.h += RV;
	  //---------------------------------------------------------------------
	  }
	*/
	
	
	//-------------------------------------------------------------------------
	theClient.h += BV;
	
	SetAddHelpTip(ClWidth, nMaxStrWidthNum );
	
	SkillNumLine = 0;
#ifdef UNICODE_ROCK
	RTCHAR *pStrIn = RWCHAR(thePcItem->SkillTable->theDesc);
	while(pStrIn)
	{
		TCHAR TempBuf[256];
		memset(TempBuf, NULL, sizeof(TCHAR) * 256 );
		
		pStrIn = GetStringReturnInStringW('\\', true, nMaxStrWidthNum, pStrIn, TempBuf);
	
		if( Rstrcmp(TempBuf, _RT("") ) != 0 )
		{
			Rstrcpy(RWCHAR(theSkillDesc[SkillNumLine].str), RWCHAR(TempBuf));
			if(theFontg)
				theFontg->GetStringInfo( theSkillDesc[SkillNumLine].str, &theStrInfo );
			if( ClWidth < theStrInfo.Width )
				ClWidth = theStrInfo.Width;
			
			theSkillDesc[SkillNumLine].x = 5;
			//theSkillDesc[i].x = GetAlignCenterx( &theStrInfo );
			theSkillDesc[SkillNumLine].y = theClient.h;
			theClient.h += RV;
			
			SkillNumLine++;
		}
	}
#else
	TCHAR *pStrIn = (TCHAR *)thePcItem->SkillTable->theDesc;
	while(pStrIn)
	{
		TCHAR TempBuf[256];
		memset(TempBuf, NULL, sizeof(TCHAR) * 256 );
		
		pStrIn = GetStringReturnInString('\\', true, nMaxStrWidthNum, pStrIn, TempBuf);
		if( Rstrcmp(TempBuf, _RT("") ) != 0 )
		{
			Rstrcpy(RWCHAR(theSkillDesc[SkillNumLine].str), RWCHAR(TempBuf));
			if(theFontg)
				theFontg->GetStringInfo( theSkillDesc[SkillNumLine].str, &theStrInfo );
			if( ClWidth < theStrInfo.Width )
				ClWidth = theStrInfo.Width;
			
			theSkillDesc[SkillNumLine].x = 5;
			//theSkillDesc[i].x = GetAlignCenterx( &theStrInfo );
			theSkillDesc[SkillNumLine].y = theClient.h;
			theClient.h += RV;
			
			SkillNumLine++;
		}
	}


#endif
	
	theClient.h += BV;
	
	if( (ClWidth + 10) >= MAX_ITEMTIP_WIDTH )
		ClWidth = MAX_ITEMTIP_WIDTH;
	else
		ClWidth += 10;
    
	theClient.w = ClWidth;
	
}


void CHelpItem::SetAddHelpTip(int &ClWidth , int &nMaxStrWidthNum )
{
	
	
	//딜레이 
	if( thePcItem->SkillTable->theDelayTime)
	{
		Rsprintf( RWCHAR(theDelay.str), _RT("%s: %d%s"),G_STRING_CHAR(IDS_HELP_KEEP_UP_TIME), 
			thePcItem->SkillTable->theDelayTime/10 , G_STRING_CHAR(IDS_WND_CLOCK_SEC) );
		if(theFontg)
			theFontg->GetStringInfo( theDelay.str, &theStrInfo );
		if( ClWidth < theStrInfo.Width )
            ClWidth = theStrInfo.Width;
		if( nMaxStrWidthNum < theStrInfo.nCLen )
			nMaxStrWidthNum = theStrInfo.nCLen;
		
		theDelay.x = 5;
		//theSkillWastePoint.x = GetAlignCenterx( &theStrInfo );
		theDelay.y = theClient.h;
		theClient.h += RV;
		//---------------------------------------------------------------------
		
	}
	
	
	//	SetHideHelpTip(ClWidth , nMaxStrWidthNum);
	
	//효과 3개?
	
	int effectType[3];
	ZeroMemory(&effectType,sizeof(effectType));
	for(int i =0 ; i < 3 ; ++i)
	{
		
		if( thePcItem->SkillTable->theEffectInfo[i].theEffectType  && 
			((thePcItem->SkillTable->theEffectInfo[i].theEffectValue != 0)||  
			g_RockClient.IsHolySkill(thePcItem->Code)  )) 
		{
			
			if (g_SkillLevelUpWnd.GetEffectStrInfo( thePcItem ,thePcItem->SkillTable , theEffect[i].str , effectType ,i  ))
			{
				
				if(theFontg)
					theFontg->GetStringInfo( theEffect[i].str, &theStrInfo );
				if( ClWidth < theStrInfo.Width )
					ClWidth = theStrInfo.Width;
				if( nMaxStrWidthNum < theStrInfo.nCLen )
					nMaxStrWidthNum = theStrInfo.nCLen;
				
				theEffect[i].x = 5;
				//theSkillWastePoint.x = GetAlignCenterx( &theStrInfo );
				theEffect[i].y = theClient.h;
				theClient.h += RV;
				//---------------------------------------------------------------------
				
			}
			
		}
		
		
	}
	
	
	//속성증폭은 뺀다
	if( thePcItem->SkillTable->theAddEffect > 0  && 
		thePcItem->SkillTable->theAddEffect != n_AmplifyAttri )
	{
		
		
		if( g_SkillLevelUpWnd.GetAddEffectInfo(thePcItem , thePcItem->SkillTable , theAddEfect.str ))
		{
			
			if(theFontg)
				theFontg->GetStringInfo( theAddEfect.str, &theStrInfo );
			if( ClWidth < theStrInfo.Width )
				ClWidth = theStrInfo.Width;
			if( nMaxStrWidthNum < theStrInfo.nCLen )
				nMaxStrWidthNum = theStrInfo.nCLen;
			
			theAddEfect.x = 5;
			//theSkillWastePoint.x = GetAlignCenterx( &theStrInfo );
			theAddEfect.y = theClient.h;
			theClient.h += RV;
			
		}
		
	}
	theClient.h += RV;
	
}


void CHelpItem::SetHideHelpTip(int &ClWidth , int &nMaxStrWidthNum )
{
	
	//영창딜레이 
	if( thePcItem->SkillTable->theNeedSpelling)
	{
		
		sprintf( theNeedDelay.str, "시전 시간 %.1f초 ",  (float)thePcItem->SkillTable->theNeedSpelling/10.f );
		if(theFontg)
			theFontg->GetStringInfo( theNeedDelay.str, &theStrInfo );
		if( ClWidth < theStrInfo.Width )
            ClWidth = theStrInfo.Width;
		if( nMaxStrWidthNum < theStrInfo.nCLen )
			nMaxStrWidthNum = theStrInfo.nCLen;
		
		theNeedDelay.x = 5;
		//theSkillWastePoint.x = GetAlignCenterx( &theStrInfo );
		theNeedDelay.y = theClient.h;
		theClient.h += RV;
		//---------------------------------------------------------------------
		
	}
	
	
	//공격 속성  
	char strAttri[20] = "";
	if( thePcItem->SkillTable->theAttackAttri)
	{
		switch(thePcItem->SkillTable->theAttackAttri)
		{
		case 0:
			Rstrcpy(RWCHAR(strAttri) ,_RT("무") ); break ;
		case 1:
			Rstrcpy(RWCHAR(strAttri) ,_RT("화") ); break; 
		case 2:
			Rstrcpy(RWCHAR(strAttri) ,_RT("빙") ); break; 
		case 3:
			Rstrcpy(RWCHAR(strAttri) ,_RT("대지") ); break; 
		case 4:
			Rstrcpy(RWCHAR(strAttri) ,_RT("뢰") ); break; 
		case 5:
			Rstrcpy(RWCHAR(strAttri) ,_RT("수") ); break; 
		case 6:
			Rstrcpy(RWCHAR(strAttri) ,_RT("자연")); break; 
		case 7:
			Rstrcpy(RWCHAR(strAttri) ,_RT("성")); break; 
		case 8:
			Rstrcpy(RWCHAR(strAttri) ,_RT("마")); break; 
		case 9:
			Rstrcpy(RWCHAR(strAttri) ,_RT("정신")); break; 
		case 10:
			Rstrcpy(RWCHAR(strAttri) ,_RT("저주")); break; 
		case 11:
			Rstrcpy(RWCHAR(strAttri) ,_RT("생체")); break; 
		}
		
		sprintf( theAttackAttri.str, "%s속성",strAttri  );
		if(theFontg)
			theFontg->GetStringInfo( theNeedDelay.str, &theStrInfo );
		if( ClWidth < theStrInfo.Width )
            ClWidth = theStrInfo.Width;
		if( nMaxStrWidthNum < theStrInfo.nCLen )
			nMaxStrWidthNum = theStrInfo.nCLen;
		
		theAttackAttri.x = 5;
		//theSkillWastePoint.x = GetAlignCenterx( &theStrInfo );
		theAttackAttri.y = theClient.h;
		theClient.h += RV;
		//---------------------------------------------------------------------
		
	}
	
	
	const TCHAR strSelf[20]="자신";
	const TCHAR strTarget[20]="대상";
	const TCHAR strLand[20]="일정영역";
	const TCHAR strParty[20]="파티원";
	const TCHAR strUseSkillType[20]="시전 타입";
	TCHAR strBuff[40]="";
	
	//시전대상
	if( thePcItem->SkillTable->theTarget  > 0 )
	{
		
		switch(thePcItem->SkillTable->theTarget)
		{
			
		case n_T_SELF:	
			Rsprintf(RWCHAR(strBuff), _RT("%s:%s") , strUseSkillType , strSelf ); 
			break;
			
		case n_T_SAMECHAR_NOTSELF:
		case n_T_SAMECHAR:	
		case n_T_NPC:					
		case n_T_OTHERCHAR:
		case n_T_ALL_CHAR:
			Rsprintf(RWCHAR(strBuff), _RT("%s:%s") , strUseSkillType , strTarget ); 
			break; 
		case n_T_PARTY:
		case n_T_LAND_PARTY:	
		case n_T_AURA_PARTY:	
			Rsprintf(RWCHAR(strBuff), _RT("%s:%s") , strUseSkillType , strParty ); 					
			break;	
			
		case n_T_LANDTARGET:
		case n_T_LAND_SAMECHAR:
		case n_T_LAND_OTHERCHAR:
		case n_T_AURA_SAMECHAR:	
		case n_T_AURA_OTHERCHAR:
		case n_T_AURA_FRONTIER:	
			Rsprintf(RWCHAR(strBuff), _RT("%s:%s") , strUseSkillType , strLand ); 											
			break;	
			
		default: break;
			
		}
		
		
		Rsprintf( RWCHAR(theTarget.str), _RT("%s"), RWCHAR(strBuff) );
		if(theFontg)
			theFontg->GetStringInfo( theTarget.str, &theStrInfo );
		if( ClWidth < theStrInfo.Width )
            ClWidth = theStrInfo.Width;
		if( nMaxStrWidthNum < theStrInfo.nCLen )
			nMaxStrWidthNum = theStrInfo.nCLen;
		
		theTarget.x = 5;
		//theSkillWastePoint.x = GetAlignCenterx( &theStrInfo );
		theTarget.y = theClient.h;
		theClient.h += RV;
		
	}
	
	
}

//-----------------------------------------------------------------------------
D3DCOLOR CHelpItem::GetRareColor( BYTE rare )//Rare( 희귀도 )에 따른 컬러값....
{
	if( rare == 0 ){	return( D3DCOLOR_XRGB( 120, 110, 75 ) );	}//초급 아이템 - 회색
	else if( ( rare >= 1 ) && ( rare <= 30 ) )		{	return( D3DCOLOR_XRGB( 64, 64, 64 ) );	}		//일반 아이템	- 검정색
	else if( ( rare >= 31 ) && ( rare <= 70 ) )		{	return( D3DCOLOR_XRGB( 180, 35, 255 ) );	}	//중급 아이템	- 보라색
	else if( ( rare >= 71 ) && ( rare <= 150 ) )	{	return( D3DCOLOR_XRGB( 90, 85, 250 ) );	}	//고급 아이템	- 파란색
	else if( ( rare >= 151 ) && ( rare <= 200 ) )	{	return( D3DCOLOR_XRGB( 255, 175, 0 ) );	}	//귀중품		- 노란색
	else if( ( rare >= 201 ) && ( rare <= 230 ) )	{	return( D3DCOLOR_XRGB( 255, 30, 200 ) );	}	//보물			- 분홍색
	else if( ( rare >= 231 ) && ( rare <= 255 ) )	{	return( D3DCOLOR_XRGB( 250, 52, 52 ) );	}	//레전드리		- 빨간색
	
	//예외상황일때..흰색 리턴
	return( D3DCOLOR_XRGB( 0, 0, 0 ) );
}

void CHelpItem::SetAccAddEffect( WORD aScrollCode, STipString & tipString )
{
	if( aScrollCode )
	{
		SItemBaseInfo* item_Info = NULL;
		BYTE aSkillLevel = 0;

		item_Info = g_Pc_Manager.GetItemBaseInfoPtr( aScrollCode );

		if( item_Info )
		{
			WORD aOwnSkillCode = 0;
			aOwnSkillCode = item_Info->theOwnSkillCode;				
			if( aOwnSkillCode )
			{
				SSkillBaseInfo* skill_Info = NULL;
				skill_Info = g_Pc_Manager.GetSkillBaseInfoPtr( aOwnSkillCode ); 

				if( skill_Info )
				{
					///-- 스킬 숙련도
					aSkillLevel = g_SkillLevelUpWnd.GetActiveSKillLevel( skill_Info );
			
					if( skill_Info->theAttackAttri == 12 )
					{
						Rsprintf( RWCHAR(tipString.str), G_STRING_CHAR( IDS_RES_OF_PARALYSIS_PER ) , aSkillLevel );	
					}
					else if( skill_Info->theAttackAttri == 13 )
					{
						Rsprintf( RWCHAR(tipString.str), G_STRING_CHAR( IDS_RES_OF_DISPEL_PER ) , aSkillLevel );		
					}	
				}			
			}		
		}	
	}	
}

void CHelpItem::SetItemProperty(SItemBaseInfo * pEldaInfo, STipString & tipString)
{
	//@@@@ short	thePower;			// 파워	-50 - 50
	//@@@@ short	theDefense;			// 체력	 
	//@@@@ short	theSympathy;		// 교감 
	//@@@@ short	theInt;			// 지능 
	//@@@@ short	theStamina;			// 지구력 
	//@@@@ short	theDex;		// 반응
	
	char power[20] = {0,};
	char vital[20] = {0,};
	char sympathy[20] = {0,};
	char intelligence[20] = {0,};
	char stamina[20] = {0,};	
	char dex[20] = {0,};
	
	
	//-------------------파워------------------------------
	int thePower = thePcItem->ItemTable->thePower;
	if( pEldaInfo )
	{
		thePower += pEldaInfo->thePower;
	}	
	
	if( thePower != 0 )
	{
		Rsprintf( RWCHAR(power), _RT("%s+%d "),G_STRING_CHAR( IDS_CHARSTATE_POWER2 ), thePower );					
		Rstrcat( RWCHAR(tipString.str),RWCHAR(power) );
	}

	//-------------------체력------------------------------
	int theVital = thePcItem->ItemTable->theVital;
	if( pEldaInfo )
	{
		theVital += pEldaInfo->theVital;
	}
	if( theVital != 0 )
	{
		Rsprintf( RWCHAR(vital), _RT("%s+%d "),G_STRING_CHAR( IDS_CHARSTATE_VITAL2 ),  theVital );					
		Rstrcat( RWCHAR(tipString.str), RWCHAR(vital) );
	}
	//-------------------교감------------------------------
	int theSympathy = thePcItem->ItemTable->theSympathy;
	if( pEldaInfo )
	{
		theSympathy += pEldaInfo->theSympathy;
	}
	if( theSympathy != 0 )
	{
		Rsprintf( RWCHAR(sympathy), _RT("%s+%d "),G_STRING_CHAR( IDS_CHARSTATE_SYMPATHY2 ), theSympathy );			
		Rstrcat( RWCHAR(tipString.str), RWCHAR(sympathy) ); 
	}
	//-------------------지능------------------------------
	int theInt = thePcItem->ItemTable->theInt;
	if( pEldaInfo )
	{
		theInt += pEldaInfo->theInt;
	}
	if( theInt != 0 )
	{
		Rsprintf( RWCHAR(intelligence), _RT("%s+%d "),G_STRING_CHAR( IDS_CHARSTATE_INTELLI2 ), theInt );				
		Rstrcat( RWCHAR(tipString.str), RWCHAR(intelligence) );	
	}
	//-------------------지구력------------------------------
	int theStamina = thePcItem->ItemTable->theStamina;
	if( pEldaInfo )
	{
		theStamina += pEldaInfo->theStamina;		
	}
	if( theStamina != 0 )
	{
		Rsprintf( RWCHAR(stamina), _RT("%s+%d "),G_STRING_CHAR( IDS_CHARSTATE_STAMINA2 ), theStamina );			
		Rstrcat( RWCHAR(tipString.str), RWCHAR(stamina) );
	}
	//-------------------민첩------------------------------
	int theDex = thePcItem->ItemTable->theDex;
	if( pEldaInfo )
	{
		theDex += pEldaInfo->theDex;
	}
	if( theDex != 0 )
	{
		Rsprintf( RWCHAR(dex), _RT("%s+%d "),G_STRING_CHAR( IDS_CHARSTATE_DEX2 ), theDex );						
		Rstrcat( RWCHAR(tipString.str), RWCHAR(dex) ); 
	}
}

//-----------------------------------------------------------------------------
void CHelpItem::DrawTipWeapon()
{
	if(theFontg)
	{
		theFontg->GetStringInfo( theWeaponName.str, &theStrInfo );
		Render->Draw(TID_Tooltip2_Img, (theWeaponName.x + theClient.x - 5), (theWeaponName.y + theClient.y - 5), 
			theClient.w, (theStrInfo.Height + 10), D3DCOLOR_ARGB( 225, 220, 220, 255 ));
		
		if(thePcItem->ItemTable)
			Render->DrawText( theFontg, theWeaponName.str, (theWeaponName.x + theClient.x), (theWeaponName.y + theClient.y), GetRareColor( thePcItem->ItemTable->theRare ) );
		
		if((thePcItem->WearTable.theCode) && ( (Rstrcmp(theAppEldInfo.str, _RT("")) != 0) ||(Rstrcmp(theAppMixEldInfo.str, _RT("")) != 0) ) )			
		{

			if(thebMixEldaStone)
			{
				theFontg->GetStringInfo( theAppMixEldInfo.str, &theStrInfo );
				
				Render->Draw(TID_CLIENT, (theAppMixEldInfo.x + theClient.x - 3), (theAppMixEldInfo.y + theClient.y - 3), 
					(theStrInfo.Width + 6), (theStrInfo.Height + 6), theMixEldaColor);	
				
				Render->DrawText( theFontg, theAppMixEldTemp.str, (theAppMixEldTemp.x + theClient.x), 
					(theAppMixEldTemp.y + theClient.y), theAppMixEldTemp.color);
				Render->DrawText( theFontg, theAppMixEldInfo.str, (theAppMixEldInfo.x + theClient.x), 
					(theAppMixEldInfo.y + theClient.y), theAppMixEldInfo.color);
				
			}

			if( Rstrcmp(theAppEldInfo.str, _RT("")) != 0)
			{
				theFontg->GetStringInfo( theAppEldInfo.str, &theStrInfo );
				
				Render->Draw(TID_CLIENT, (theAppEldInfo.x + theClient.x - 3), (theAppEldInfo.y + theClient.y - 3), 
					(theStrInfo.Width + 6), (theStrInfo.Height + 6), theEldaColor);	
				
				Render->DrawText( theFontg, theAppEldTemp.str, (theAppEldTemp.x + theClient.x), 
					(theAppEldTemp.y + theClient.y), theAppEldTemp.color);
				Render->DrawText( theFontg, theAppEldInfo.str, (theAppEldInfo.x + theClient.x), 
				(theAppEldInfo.y + theClient.y), theAppEldInfo.color);
			}

		}		
		
		Render->DrawText( theFontg, theWeaponGrade.str, (theWeaponGrade.x + theClient.x), (theWeaponGrade.y + theClient.y),  theWeaponGrade.color );
		Render->DrawText( theFontg, theWeaponNeed.str, (theWeaponNeed.x + theClient.x), (theWeaponNeed.y + theClient.y),  theWeaponNeed.color );
		
		for( int i = 0; i < WeaponNumLine; ++i )
		{
			Render->DrawText( theFontg, theWeaponDesc[i].str, (theWeaponDesc[i].x + theClient.x), (theWeaponDesc[i].y + theClient.y), theWeaponDesc[i].color );
		}
		
		Render->DrawText( theFontg, theWeaponAttack.str, (theWeaponAttack.x + theClient.x), (theWeaponAttack.y + theClient.y),  theWeaponAttack.color );
		Render->DrawText( theFontg, theWeaponMagic.str, (theWeaponMagic.x + theClient.x), (theWeaponMagic.y + theClient.y),  theWeaponMagic.color );
		Render->DrawText( theFontg, theWeaponAttackAttri.str, (theWeaponAttackAttri.x + theClient.x), (theWeaponAttackAttri.y + theClient.y),  theWeaponAttackAttri.color );
		Render->DrawText( theFontg, theWeaponType.str, (theWeaponType.x + theClient.x), (theWeaponType.y + theClient.y),  theWeaponType.color );
		Render->DrawText( theFontg, theWeaponproperty.str, (theWeaponproperty.x + theClient.x), (theWeaponproperty.y + theClient.y),  theWeaponproperty.color );
		
		Render->DrawText( theFontg, theWeaponAttackSpeed.str, (theWeaponAttackSpeed.x + theClient.x), (theWeaponAttackSpeed.y + theClient.y),  theWeaponAttackSpeed.color );
		Render->DrawText( theFontg, theWeaponMagicSpeed.str, (theWeaponMagicSpeed.x + theClient.x), (theWeaponMagicSpeed.y + theClient.y),  theWeaponMagicSpeed.color );
		Render->DrawText( theFontg, theWeaponSkillName.str, (theWeaponSkillName.x + theClient.x), (theWeaponSkillName.y + theClient.y),  theWeaponSkillName.color );
		Render->DrawText( theFontg, theWeaponAddEffect1.str, (theWeaponAddEffect1.x + theClient.x), (theWeaponAddEffect1.y + theClient.y),  theWeaponAddEffect1.color );
		Render->DrawText( theFontg, theWeaponAddEffect2.str, (theWeaponAddEffect2.x + theClient.x), (theWeaponAddEffect2.y + theClient.y),  theWeaponAddEffect2.color );
#ifdef C_TIME_SUIT_UPDATE
		
		if(thePcItem->ItemTable->theMaxEndurance == 0  && thePcItem->ItemTable->theRare == 255 )
		{	
			g_ListTime.GetTimeRemainStr(thePcItem->ItemUID , theWeaponEBW.str );
			Render->DrawText( theFontg, theWeaponEBW.str, (theWeaponEBW.x + theClient.x), (theWeaponEBW.y + theClient.y),  theWeaponEBW.color );
		}
		else
		{
			Render->DrawText( theFontg, theWeaponEBW.str, (theWeaponEBW.x + theClient.x), (theWeaponEBW.y + theClient.y),  theWeaponEBW.color );
			
		}
		
#else
		
		Render->DrawText( theFontg, theWeaponEBW.str, (theWeaponEBW.x + theClient.x), (theWeaponEBW.y + theClient.y),  theWeaponEBW.color );
		
#endif
		
		
		Render->DrawText( theFontg, theWeaponEBW.str, (theWeaponEBW.x + theClient.x), (theWeaponEBW.y + theClient.y),  theWeaponEBW.color );
		Render->DrawText( theFontg, theWeaponPrice.str, (theWeaponPrice.x + theClient.x), (theWeaponPrice.y + theClient.y),  theWeaponPrice.color );
		
		if( thePcItem->ItemTable->theLimitFame > 0 )
		{
			Render->DrawText( theFontg, theNeedFame.str, (theNeedFame.x + theClient.x), (theNeedFame.y + theClient.y),  theNeedFame.color );
		}
	}
}

//-----------------------------------------------------------------------------
void CHelpItem::DrawTipWear()
{
	if(theFontg)
	{
		theFontg->GetStringInfo( theWearName.str, &theStrInfo );
		Render->Draw(TID_Tooltip2_Img, (theWearName.x + theClient.x - 5), (theWearName.y + theClient.y - 5), 
			theClient.w , (theStrInfo.Height + 10), D3DCOLOR_ARGB( 180, 220, 220, 255 ));	
		
		if(thePcItem->ItemTable)
			Render->DrawText( theFontg, theWearName.str, (theWearName.x + theClient.x), (theWearName.y + theClient.y), GetRareColor( thePcItem->ItemTable->theRare ) );
		
		if((thePcItem->WearTable.theCode) &&  ( (Rstrcmp(theAppEldInfo.str, _RT("")) != 0) ||(Rstrcmp(theAppMixEldInfo.str, _RT("")) != 0) ) )	
		{	
			if(thebMixEldaStone)
			{
				theFontg->GetStringInfo( theAppMixEldInfo.str, &theStrInfo );
				
				Render->Draw(TID_CLIENT, (theAppMixEldInfo.x + theClient.x - 3), (theAppMixEldInfo.y + theClient.y - 3), 
					(theStrInfo.Width + 6), (theStrInfo.Height + 6), theMixEldaColor);	
				
				Render->DrawText( theFontg, theAppMixEldTemp.str, (theAppMixEldTemp.x + theClient.x), 
					(theAppMixEldTemp.y + theClient.y), theAppMixEldTemp.color);
				Render->DrawText( theFontg, theAppMixEldInfo.str, (theAppMixEldInfo.x + theClient.x), 
					(theAppMixEldInfo.y + theClient.y), theAppMixEldInfo.color);
				
			}

			if( Rstrcmp(theAppEldInfo.str, _RT("")) != 0) 
			{
				theFontg->GetStringInfo( theAppEldInfo.str, &theStrInfo );
				
				Render->Draw(TID_CLIENT, (theAppEldInfo.x + theClient.x - 3), (theAppEldInfo.y + theClient.y - 3), 
					(theStrInfo.Width + 6), (theStrInfo.Height + 6), theEldaColor);	
				
				Render->DrawText( theFontg, theAppEldTemp.str, (theAppEldTemp.x + theClient.x), 
					(theAppEldTemp.y + theClient.y), theAppEldTemp.color);
				Render->DrawText( theFontg, theAppEldInfo.str, (theAppEldInfo.x + theClient.x), 
				(theAppEldInfo.y + theClient.y), theAppEldInfo.color);

			}
		
		}		
		
		Render->DrawText( theFontg, theWearGrade.str, (theWearGrade.x + theClient.x), (theWearGrade.y + theClient.y),  theWearGrade.color );
		Render->DrawText( theFontg, theWearNeed.str, (theWearNeed.x + theClient.x), (theWearNeed.y + theClient.y),  theWearNeed.color );
		
		bool ChangeColor = true ; 			
		for( int i = 0; i < WearNumLine; ++i )
		{
			
			if(theWearDesc[i].SetMode == 0 )
			{
				Render->DrawText( theFontg, theWearDesc[i].str, (theWearDesc[i].x + theClient.x), (theWearDesc[i].y + theClient.y), theWearDesc[i].color );			
			}
			else 
			{
				Render->DrawText2( theFontg, theWearDesc[i].str, (theWearDesc[i].x + theClient.x), (theWearDesc[i].y + theClient.y), theWearNeed.color , ChangeColor, theWearDesc[i].SetMode );
			}
		}
		
		
		//세트 아이템일경우 
		if( thePcItem->ItemTable->theSetID !=0 )
		{
			for(int i =0 ; i < 4 ; ++i)
			{
				Render->DrawText( theFontg, theWearSetItem[i].str, (theWearSetItem[i].x + theClient.x), (theWearSetItem[i].y + theClient.y),  theWearSetItem[i].color );
			}
		}
		
		//윙만 능력치 표시한다 
		if(thePcItem->ItemTable->theClass == n_Class_Serve )
		{
			if(thePcItem->ItemTable->theMinAttackBonus !=0 || thePcItem->ItemTable->theMaxAttackBonus !=0 )
			{
				Render->DrawText( theFontg, theWeaponAttack.str, (theWeaponAttack.x + theClient.x), (theWeaponAttack.y + theClient.y),  theWeaponAttack.color );
				
			}
			if(thePcItem->ItemTable->theMinMagicBonus !=0  || thePcItem->ItemTable->theMaxMagicBonus !=0 )
			{
				Render->DrawText( theFontg, theWeaponMagic.str, (theWeaponMagic.x + theClient.x), (theWeaponMagic.y + theClient.y),  theWeaponMagic.color );
			}
		}
		
		
		Render->DrawText( theFontg, theWearAttackDef.str, (theWearAttackDef.x + theClient.x), (theWearAttackDef.y + theClient.y),  theWearAttackDef.color );
		Render->DrawText( theFontg, theWearMagicRes.str, (theWearMagicRes.x + theClient.x), (theWearMagicRes.y + theClient.y),  theWearMagicRes.color );
		Render->DrawText( theFontg, theWearMentalRes.str, (theWearMentalRes.x + theClient.x), (theWearMentalRes.y + theClient.y),  theWearMentalRes.color );
		Render->DrawText( theFontg, theWearCurseRes.str, (theWearCurseRes.x + theClient.x), (theWearCurseRes.y + theClient.y),  theWearCurseRes.color );
		Render->DrawText( theFontg, theWearVitalRes.str, (theWearVitalRes.x + theClient.x), (theWearVitalRes.y + theClient.y),  theWearVitalRes.color );
		
		Render->DrawText( theFontg, theWearDefensAttri.str, (theWearDefensAttri.x + theClient.x), (theWearDefensAttri.y + theClient.y),  theWearDefensAttri.color );
		Render->DrawText( theFontg, theWearPoint.str, (theWearPoint.x + theClient.x), (theWearPoint.y + theClient.y),  theWearPoint.color );
		Render->DrawText( theFontg, theAccAddEffect.str, (theAccAddEffect.x + theClient.x), (theAccAddEffect.y + theClient.y),  theAccAddEffect.color );	///-- 장신구 부가효과
		Render->DrawText( theFontg, theWearproperty.str, (theWearproperty.x + theClient.x), (theWearproperty.y + theClient.y),  theWearproperty.color );
		Render->DrawText( theFontg, theWearAttackEvade.str, (theWearAttackEvade.x + theClient.x), (theWearAttackEvade.y + theClient.y),  theWearAttackEvade.color );
		Render->DrawText( theFontg, theWearSkillName.str, (theWearSkillName.x + theClient.x), (theWearSkillName.y + theClient.y),  theWearSkillName.color );
		
		Render->DrawText( theFontg, theWearAddEffect1.str, (theWearAddEffect1.x + theClient.x), (theWearAddEffect1.y + theClient.y),  theWearAddEffect1.color );
		Render->DrawText( theFontg, theWearAddEffect2.str, (theWearAddEffect2.x + theClient.x), (theWearAddEffect2.y + theClient.y),  theWearAddEffect2.color );
#ifdef C_TIME_SUIT_UPDATE
		
		if(thePcItem->ItemTable->theMaxEndurance == 0  && thePcItem->ItemTable->theRare == 255 )
		{	
			g_ListTime.GetTimeRemainStr(thePcItem->ItemUID , theWearEBW.str );
			Render->DrawText( theFontg, theWearEBW.str, (theWearEBW.x + theClient.x), (theWearEBW.y + theClient.y), theWearEBW.color );
		}
		else
		{
			Render->DrawText( theFontg, theWearEBW.str, (theWearEBW.x + theClient.x), (theWearEBW.y + theClient.y), theWearEBW.color );
			
		}

#else
		
		Render->DrawText( theFontg, theWearEBW.str, (theWearEBW.x + theClient.x), (theWearEBW.y + theClient.y), theWearEBW.color );
		
#endif
		
		
		
		Render->DrawText( theFontg, theWearPrice.str, (theWearPrice.x + theClient.x), (theWearPrice.y + theClient.y),  theWearPrice.color );
		
		if( thePcItem->ItemTable->theLimitFame > 0 )
		{
			Render->DrawText( theFontg, theNeedFame.str, (theNeedFame.x + theClient.x), (theNeedFame.y + theClient.y),  theNeedFame.color );
		}
		
		
	}
}

//-----------------------------------------------------------------------------
void CHelpItem::DrawTipEtc()
{
	if(theFontg)
	{
		theFontg->GetStringInfo( theEtcName.str, &theStrInfo );
		Render->Draw(TID_Tooltip2_Img, (theEtcName.x + theClient.x - 5), (theEtcName.y + theClient.y - 5), 
			theClient.w , (theStrInfo.Height + 10), D3DCOLOR_ARGB( 180, 220, 220, 255 ));
		
		if( (Rstrcmp(theAppEldInfo.str, _RT("")) != 0) )			
		{
			theFontg->GetStringInfo( theAppEldInfo.str, &theStrInfo );
			
			Render->Draw(TID_CLIENT, (theAppEldInfo.x + theClient.x - 3), (theAppEldInfo.y + theClient.y - 3), 
				(theStrInfo.Width + 6), (theStrInfo.Height + 6), theEldaColor);	
			
			Render->DrawText( theFontg, theAppEldTemp.str, (theAppEldTemp.x + theClient.x), 
				(theAppEldTemp.y + theClient.y), theAppEldTemp.color);
			Render->DrawText( theFontg, theAppEldInfo.str, (theAppEldInfo.x + theClient.x), 
				(theAppEldInfo.y + theClient.y), theAppEldInfo.color);
		}
		
		/*
		if( ((CSlot*)theSlotWnd)->Group == n_SGROUP_ELDA_MIX_PRE0 || 
			((CSlot*)theSlotWnd)->Group == n_SGROUP_ELDA_MIX_PRE1  )
		{
			if(thePcItem->ItemTable)
				Render->DrawText( theFontg, theEtcName.str, (theEtcName.x + theClient.x), (theEtcName.y + theClient.y), GetRareColor( thePcItem->ItemTable->theRare ) );
			return;
		}
		*/
		if(thePcItem->ItemTable)
			Render->DrawText( theFontg, theEtcName.str, (theEtcName.x + theClient.x), (theEtcName.y + theClient.y), GetRareColor( thePcItem->ItemTable->theRare ) );
		
		Render->DrawText( theFontg, theEtcGrade.str, (theEtcGrade.x + theClient.x), (theEtcGrade.y + theClient.y),  theEtcGrade.color );
		Render->DrawText( theFontg, theEtcNeed.str, (theEtcNeed.x + theClient.x), (theEtcNeed.y + theClient.y),  theEtcNeed.color );		
		
#ifdef KEEPUP_PAYITEM_RENEWAL
		
		if(thePcItem->ItemTable->theClass == n_Class_Use && thePcItem->ItemTable->theType == n_Type_Scroll && thePcItem->IType == n_ITYPE_SPECIAL 
			&& (thePcItem->ItemTable->theReGage || thePcItem->ItemTable->theReMPSpeed || thePcItem->ItemTable->theReSPSpeed ||thePcItem->ItemTable->theReHPSpeed ) )
		{
			Render->DrawText( theFontg, theEtcTime.str, (theEtcTime.x + theClient.x), (theEtcTime.y + theClient.y),  theEtcTime.color );		
		}
#endif	
#ifdef PREMIUMZONE_KEY_HELPTIP
		if(thePcItem->ItemTable->theMaxEndurance == 0  && thePcItem->ItemTable->theRare == 255 )
		{	
			g_ListTime.GetTimeRemainStr(thePcItem->ItemUID , theEtcTime.str );
			Render->DrawText( theFontg, theEtcTime.str, (theEtcTime.x + theClient.x), (theEtcTime.y + theClient.y), theEtcTime.color );
		}
#endif //PREMIUMZONE_KEY_HELPTIP		

#ifdef PET_PAY_USEITEM_HELPTIP //펫 유료 아이템 지속시간 표시(헬프팁)
		
		if(thePcItem->ItemTable->theClass == n_Class_PersonalPet && thePcItem->ItemTable->theType == n_Type_Scroll && thePcItem->IType == n_ITYPE_SPECIAL 
			&& (thePcItem->ItemTable->theReGage || thePcItem->ItemTable->theReMPSpeed || thePcItem->ItemTable->theReSPSpeed ||thePcItem->ItemTable->theReHPSpeed ) )
		{ //그리는 부분
			Render->DrawText( theFontg, theEtcTime.str, (theEtcTime.x + theClient.x), (theEtcTime.y + theClient.y),  theEtcTime.color );		
		}
#endif //PET_PAY_USEITEM_HELPTIP
		for( int i = 0; i < EtcNumLine; ++i )
		{
			Render->DrawText( theFontg, theEtcDesc[i].str, (theEtcDesc[i].x + theClient.x), (theEtcDesc[i].y + theClient.y), theEtcDesc[i].color );
		}
		Render->DrawText( theFontg, theEtcBWP.str, (theEtcBWP.x + theClient.x), (theEtcBWP.y + theClient.y),  theEtcBWP.color );
		Render->DrawText( theFontg, theEtcPrice.str, (theEtcPrice.x + theClient.x), (theEtcPrice.y + theClient.y),  theEtcPrice.color );
	}
}

//-----------------------------------------------------------------------------
void CHelpItem::DrawTipSkill()
{
	if(theFontg)
	{
		

		theFontg->GetStringInfo( theSkillName.str, &theStrInfo );
		Render->Draw(TID_Tooltip2_Img, (theSkillName.x + theClient.x - 5), (theSkillName.y + theClient.y - 5), 
			theClient.w , (theStrInfo.Height + 10), D3DCOLOR_ARGB( 180, 220, 220, 255 ));		
		
		Render->DrawText( theFontg, theSkillName.str, (theSkillName.x + theClient.x), (theSkillName.y + theClient.y), theSkillName.color );
		Render->DrawText( theFontg, theSkillNeed.str, (theSkillNeed.x + theClient.x), (theSkillNeed.y + theClient.y), theSkillNeed.color );
		Render->DrawText( theFontg, theSkillToggle.str, (theSkillToggle.x + theClient.x), (theSkillToggle.y + theClient.y), theSkillToggle.color );
		
		for( int i = 0; i < SkillNumLine; ++i )
		{
			Render->DrawText( theFontg, theSkillDesc[i].str, (theSkillDesc[i].x + theClient.x), (theSkillDesc[i].y + theClient.y), theSkillDesc[i].color );
		}
		Render->DrawText( theFontg, theSkillItem.str, (theSkillItem.x + theClient.x), (theSkillItem.y + theClient.y), theSkillItem.color );
		Render->DrawText( theFontg, theSkillGage.str, (theSkillGage.x + theClient.x), (theSkillGage.y + theClient.y), theSkillGage.color );
		Render->DrawText( theFontg, theSkillWastePoint.str, (theSkillWastePoint.x + theClient.x), (theSkillWastePoint.y + theClient.y), theSkillWastePoint.color );
		Render->DrawText( theFontg, theSkillKeepupTime.str, (theSkillKeepupTime.x + theClient.x), (theSkillKeepupTime.y + theClient.y), theSkillKeepupTime.color );
		Render->DrawText( theFontg, theSkillProimaType.str, (theSkillProimaType.x + theClient.x), (theSkillProimaType.y + theClient.y), theSkillProimaType.color );
		Render->DrawText( theFontg, theSkillUpLevel.str, (theSkillUpLevel.x + theClient.x), (theSkillUpLevel.y + theClient.y), theSkillUpLevel.color );
		Render->DrawText( theFontg, theNeedPoint.str, (theNeedPoint.x + theClient.x), (theNeedPoint.y + theClient.y), theNeedPoint.color );
		
		
		if( thePcItem->SkillTable == NULL  ) return;

		if( thePcItem->SkillTable->theDelayTime ) 
		{
			Render->DrawText( theFontg, theDelay.str, (theDelay.x + theClient.x), (theDelay.y + theClient.y), theDelay.color );
		}
		if( thePcItem->SkillTable->theNeedSpelling)
		{
			Render->DrawText( theFontg, theNeedDelay.str, (theNeedDelay.x + theClient.x), (theNeedDelay.y + theClient.y), theNeedDelay.color );
		}
		if( thePcItem->SkillTable->theAttackAttri)
		{
			Render->DrawText( theFontg, theAttackAttri.str, (theAttackAttri.x + theClient.x), (theAttackAttri.y + theClient.y), theAttackAttri.color );
		}
		for(short i =0 ; i < 3 ; ++i)
		{
			
			if( thePcItem->SkillTable->theEffectInfo[i].theEffectType ) 
			{
				
				Render->DrawText( theFontg, theEffect[i].str, (theEffect[i].x + theClient.x), (theEffect[i].y + theClient.y), theEffect[i].color );
				
			}
		}
		if( thePcItem->SkillTable->theAddEffect ) 
		{
			Render->DrawText( theFontg, theAddEfect.str, (theAddEfect.x + theClient.x), (theAddEfect.y + theClient.y), theAddEfect.color );
		}
		
		if( thePcItem->SkillTable->theTarget ) 
		{
			Render->DrawText( theFontg, theTarget.str, (theTarget.x + theClient.x), (theTarget.y + theClient.y), theTarget.color );
		}
	}
}

//-----------------------------------------------------------------------------
int CHelpItem::GetNumLine( int NumStr, int Count )
{
	int NumChar = 0;
	int NumLine = 1;
	
	if( NumStr == 0 )
	{
		return( 0 );
	}
	
	for( int i = 0; i < NumStr; ++i )
	{
		NumChar++;
		
		if( NumChar == Count )
		{
			NumChar = 0;
			NumLine++;
		}
	}
	
	return( NumLine );
}

//-----------------------------------------------------------------------------

BOOL CHelpItem::EldaStoneMix(int Select)
{
	
	char theName[64] ={0,};
	char AfterName[64] = {0,};
	
	if( Select == 0 )
	{
		Rsprintf( RWCHAR(AfterName ), _RT("%s"),_RT("") );		
	}
	else if(Select == 1 )
	{
		Rsprintf( RWCHAR(AfterName) , _RT("%s"),G_STRING_CHAR(IDS_WND_NEED));
	}
	
	int ClWidth = 0;
	int nMaxStrWidthNum = 28;
	theClient.h = BV;
	
	
	int Len = Rstrlen(thePcItem->ItemTable->theName); 
	
	while(1)
	{
		if((thePcItem->ItemTable->theName[Len] == _RT(' ') ) || Len == 0)
		{
			break; 
		}
		Len--;
		
	}
	

	if(!Len )  return FALSE;

	Rstrncat(RWCHAR(theName),RWCHAR(thePcItem->ItemTable->theName) , Len+1 );
	Rstrcat( RWCHAR(theName) , RWCHAR(AfterName) );	
	Rstrcpy( RWCHAR(theName)+(Rstrlen(RWCHAR(theName))+1)  , _RT("") );
	Rsprintf( RWCHAR(theEtcName.str), _RT("%s"), RWCHAR(theName) );
	
	if(theFontg)
		theFontg->GetStringInfo( theEtcName.str, &theStrInfo );
	if( ClWidth < theStrInfo.Width )
		ClWidth = theStrInfo.Width;
	if( nMaxStrWidthNum < theStrInfo.nCLen )
		nMaxStrWidthNum = theStrInfo.nCLen;
	
	theEtcName.x = 5;
	//theEtcName.x = GetAlignCenterx( &theStrInfo );
	theEtcName.y = theClient.h;
	theClient.h += RV;
	//-------------------------------------------------------------------------		
	theClient.h += BV;
	
	if( (ClWidth + 10) >= MAX_ITEMTIP_WIDTH )
		ClWidth = MAX_ITEMTIP_WIDTH;
	else
		ClWidth += 10;
	
	theClient.w = ClWidth;
	
	return TRUE;
	
	//정보가 팁정보인가 진짜 정보인가 구분짓기 위해 ?
	
}
//-----------------------------------------------------------------------------
