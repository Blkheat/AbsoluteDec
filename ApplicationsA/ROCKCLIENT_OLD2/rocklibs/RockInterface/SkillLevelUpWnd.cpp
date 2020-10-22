// SkillLevelUpWnd.cpp: implementation of the CSkillLevelUpWnd class.
//
//////////////////////////////////////////////////////////////////////

#include "..\\RockPCH.h"

#include "Rui.h"

#include "Define.h"
#include "Util.h"
#include "Render.h"
#include "Fontman.h"
#include "Resource.h"
#include "Chain.h"
#include "Container.h"



#include "..\\..\\global.h"
#include "..\\..\\RockClient.h"

#include "..\\..\\bravolibs\\obj\\SkillSystem.h"

//#include "pc.h"
#include "SkillLevelUpWnd.h"
#include "stringmanager.h"

CSkillLevelUpWnd	g_SkillLevelUpWnd;


//-----------------------------------------------------------------------------
void CSkillLevelUpWnd::Init()
{
	m_pPcItem = NULL;
	m_pFontg = NULL;

	return;
}


void CSkillLevelUpWnd::CompositionOld()
{
	
}
//-----------------------------------------------------------------------------

void CSkillLevelUpWnd::CompositionNew()
{

	
	CImageBox * img;
	CImageBox9 * img9;
	int center_x	= nRui->Desktop.Width / 2;
	int center_y	= nRui->Desktop.Height / 2;

	int ReSizeFrmX = 0 ;
	int ReSizeFrmY = 0 ; 
	
#ifdef APPLY_BOLD_FONT	
	int ReSizeW = 259 ;
#else
	int ReSizeW = 209 ;
#endif
	int ReSizeH = 260 ;
	int AddSizeW = 30 ; 


	SRect skillwnd;
	skillwnd.w	= ReSizeW + ReSizeW + 50 ;
	skillwnd.h	= 405 ;
	skillwnd.x	= center_x - ( skillwnd.w / 2 );
	skillwnd.y	= center_y - ( skillwnd.h / 2 );
	int i=0;
	///-- Frame Wnd
	m_pSkillLevelUpFWnd = (CFrameWnd *)nRui->CreateWnd( n_wtFrameWnd, WID_SkillLevelUpWnd, WID_None, skillwnd.x, skillwnd.y, skillwnd.w, skillwnd.h, false );
	m_pSkillLevelUpFWnd->SetWndTexStyle( fwfTex_Style_Tile9 );	
	m_pSkillLevelUpFWnd->SetClientImage( TID_CTRL_WND_L1 );	
	m_pSkillLevelUpFWnd->SetIconTID( TID_CTRL_ICON_SKILL );	
	m_pSkillLevelUpFWnd->SetWndTileSizeLR( 40, 29 );
	m_pSkillLevelUpFWnd->SetWndTileSizeTB( 31, 11 );
	m_pSkillLevelUpFWnd->SetCaption( G_STRING_CHAR( IDS_WND_SKILL_LEVUP ) ); 
	m_pSkillLevelUpFWnd->SetTitle( true );

	m_pSkillLevelUpFWnd->SetWndProc( this );	

	m_pSkillLevelUpFWnd->SetVisible( false );
	m_pSkillLevelUpFWnd->Initialize();
	
	///---------------------------------------------------------------------------	
	///-- Static
	///---------------------------------------------------------------------------		
	///-- Back	
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_SkillLevelUpWnd, 1, 82, skillwnd.w - 2  , ReSizeH + 20 , false );
	img ->SetClientImage( TID_CTRL_BACK );
	img ->SetFocusCheck( false );
	//윗선 
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_SkillLevelUpWnd, 1, 82, skillwnd.w - 2, 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );
	img ->SetFocusCheck( false );

	//아랫선 
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_SkillLevelUpWnd, 1,  82 + ReSizeH + 20 , skillwnd.w -2  , 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );
	img ->SetFocusCheck( false );

	///-- Slot
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_SkillLevelUpWnd, 29, 34, 41, 41, false );       
	img ->SetClientImage( TID_CTRL_SLOT_BIG );
	img ->SetFocusCheck( false );

	///-- arrow	
#ifdef APPLY_BOLD_FONT
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_SkillLevelUpWnd, 270, 215 , 30, 37, false );
#else	
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_SkillLevelUpWnd, 220, 230 , 30, 37, false );
#endif	
	img ->SetClientImage( TID_C_SKILL_ARROW_IMG );
	img ->SetFocusCheck( false );

		
	img9 = (CImageBox9*)nRui->CreateWnd( n_wtImageBox9, WID_None, WID_SkillLevelUpWnd, 6, 92, ReSizeW , ReSizeH , false );
	img9->SetWndTileSizeLR( 9, 9 );
	img9->SetWndTileSizeTB( 8, 8 );
	img9->SetFocusCheck( false );
	img9->SetClientImage( TID_CTRL_CONTBACK_L1 );
	img9->SetAdjustPosition( 0, 0 );


	img9 = (CImageBox9*)nRui->CreateWnd( n_wtImageBox9, WID_None, WID_SkillLevelUpWnd, skillwnd.w - ReSizeW - 6  , 92, ReSizeW, ReSizeH , false );
	img9->SetWndTileSizeLR( 9, 9 );
	img9->SetWndTileSizeTB( 8, 8 );
	img9->SetFocusCheck( false );
	img9->SetClientImage( TID_CTRL_CONTBACK_L1 );
	img9->SetAdjustPosition( 0, 0 );
	
	///---------------------------------------------------------------------------	
	///-- Control
	///---------------------------------------------------------------------------	
	///-- X	
	m_pCloseBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_SkillLevelUp_CloseBtn, WID_SkillLevelUpWnd, skillwnd.w - 25 , 6, 13, 13, false );
	m_pCloseBtn->SetBtnImage( TID_CTRL_BTN_X, TID_CTRL_BTN_X_OVR, TID_CTRL_BTN_X_CLK, TID_None );
	
	m_pOKBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_SkillLevelUp_OKBtn, WID_SkillLevelUpWnd, 6 + ReSizeW - 58 , skillwnd.h - 33 , 58, 25, false );  
	m_pOKBtn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_None );
	m_pOKBtn->SetFontR( n_fRsvFontWndButton );   
	m_pOKBtn->SetText( G_STRING_CHAR( IDS_WND_COM_OK ) );
	
	m_pCancelBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_SkillLevelUp_CancelBtn, WID_SkillLevelUpWnd, skillwnd.w - ReSizeW - 6  , skillwnd.h- 33, 58, 25, false );  
	m_pCancelBtn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_None );
	m_pCancelBtn->SetFontR( n_fRsvFontWndButton );   	
	m_pCancelBtn->SetText( G_STRING_CHAR( IDS_WND_COM_CANCEL ) );
		
	m_pSkillIcon = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_SkillLevelUp_SkillImg, WID_SkillLevelUpWnd, 32, 37, 35, 35, false );
	m_pSkillIcon->SetClientImage( TID_None );// 해당하는 스킬의 아이콘 표시 
	m_pSkillIcon->SetClientColor( 255, 255, 255 );
	
	m_pLevelUpDesc = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_SkillLevelUp_LevelUpDescText, 
												WID_SkillLevelUpWnd, 105, 39, ReSizeW + ReSizeW/2 , 33, false );
	m_pLevelUpDesc->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pLevelUpDesc->SetClientImage( TID_None );
	m_pLevelUpDesc->SetMultiLine( true );
	m_pLevelUpDesc->SetAlignText( n_atLeft, n_atCenter );
	m_pLevelUpDesc->SetText( _RT("") );
	m_pLevelUpDesc->SetTextColor( 80, 80, 80 );
	m_pLevelUpDesc->SetHID( HID_None );
	m_pLevelUpDesc->SetFocusCheck( false );

	m_CurtSkillName = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_SkillLevelUp_CurtSkillNameText, 
												  WID_SkillLevelUpWnd, 14, 108, ReSizeW, 13, false );
	m_CurtSkillName->SetFontg( n_ftGulimChe, 12, n_fwBold, n_fdNormal );
	m_CurtSkillName->SetClientImage( TID_None );
	m_CurtSkillName->SetAlignText( n_atCenter, n_atCenter );
	m_CurtSkillName->SetText( _RT("") );
	m_CurtSkillName->SetTextColor( 222, 222, 255 );
	m_CurtSkillName->SetHID( HID_None );
	m_CurtSkillName->SetFocusCheck( false );

	m_CurtSkillNeedItem = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_SkillLevelUp_CurtSkillNeedItemText, WID_SkillLevelUpWnd, 14, 110, ReSizeW, 13, false );
	m_CurtSkillNeedItem->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_CurtSkillNeedItem->SetClientImage( TID_None );
	m_CurtSkillNeedItem->SetAlignText( n_atCenter, n_atCenter );
	m_CurtSkillNeedItem->SetText( _RT("") );
	m_CurtSkillNeedItem->SetTextColor( 255, 222, 222 );
	m_CurtSkillNeedItem->SetHID( HID_None );
	m_CurtSkillNeedItem->SetFocusCheck( false );
	
	m_CurtSkillLevel = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_SkillLevelUp_CurtSkillLevelText, WID_SkillLevelUpWnd, 14, 110, ReSizeW, 13, false );
	m_CurtSkillLevel->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_CurtSkillLevel->SetClientImage( TID_None );
	m_CurtSkillLevel->SetAlignText( n_atCenter, n_atCenter );
	m_CurtSkillLevel->SetText( _RT("") );
	m_CurtSkillLevel->SetTextColor( 255, 255, 255 );
	m_CurtSkillLevel->SetHID( HID_None );
	m_CurtSkillLevel->SetFocusCheck( false );

	m_CurtSkillGageType = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_SkillLevelUp_CurtSkillGaugeTypeText, WID_SkillLevelUpWnd, 14, 110, ReSizeW, 13, false );
	m_CurtSkillGageType->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_CurtSkillGageType->SetClientImage( TID_None );
	m_CurtSkillGageType->SetAlignText( n_atCenter, n_atCenter );
	m_CurtSkillGageType->SetText( _RT("") );
	m_CurtSkillGageType->SetTextColor( 255, 234, 234 );
	m_CurtSkillGageType->SetHID( HID_None );
	m_CurtSkillGageType->SetFocusCheck( false );

	m_CurtSkillGageValue = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_SkillLevelUp_CurtSkillGaugeValueText, WID_SkillLevelUpWnd, 14, 110, ReSizeW, 13, false );
	m_CurtSkillGageValue->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_CurtSkillGageValue->SetClientImage( TID_None );
	m_CurtSkillGageValue->SetAlignText( n_atCenter, n_atCenter );
	m_CurtSkillGageValue->SetText( _RT("") );
	m_CurtSkillGageValue->SetTextColor( 255, 234, 234 );
	m_CurtSkillGageValue->SetHID( HID_None );
	m_CurtSkillGageValue->SetFocusCheck( false );

	m_CurtSkillWastePoint = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_SkillLevelUp_CurtSkillWastePointText, WID_SkillLevelUpWnd, 14, 110, ReSizeW, 13, false );
	m_CurtSkillWastePoint->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_CurtSkillWastePoint->SetClientImage( TID_None );
	m_CurtSkillWastePoint->SetAlignText( n_atCenter, n_atCenter );
	m_CurtSkillWastePoint->SetText( _RT("") );
	m_CurtSkillWastePoint->SetTextColor( 222, 255, 222 );
	m_CurtSkillWastePoint->SetHID( HID_None );
	m_CurtSkillWastePoint->SetFocusCheck( false );

	m_CurtSkillDelay = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_SkillLevelUpWnd, 14, 110, ReSizeW, 13, false );
	m_CurtSkillDelay->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_CurtSkillDelay->SetClientImage( TID_None );
	m_CurtSkillDelay->SetAlignText( n_atCenter, n_atCenter );
	m_CurtSkillDelay->SetText( _RT("") );
	m_CurtSkillDelay->SetTextColor( 255, 234, 234 );
	m_CurtSkillDelay->SetHID( HID_None );
	m_CurtSkillDelay->SetFocusCheck( false );

	for(i = 0 ; i < 3 ; ++i)
	{
		m_CurtSkillEffect[i] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_SkillLevelUpWnd, 14, 110, ReSizeW, 13, false );
		m_CurtSkillEffect[i]->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
		m_CurtSkillEffect[i]->SetClientImage( TID_None );
		m_CurtSkillEffect[i]->SetAlignText( n_atCenter, n_atCenter );
		m_CurtSkillEffect[i]->SetText( _RT("") );
		m_CurtSkillEffect[i]->SetTextColor( 255, 234, 234 );
		m_CurtSkillEffect[i]->SetHID( HID_None );
		m_CurtSkillEffect[i]->SetFocusCheck( false );
	}

	m_CurtSkillAddEffect = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_SkillLevelUpWnd, 14, 110, ReSizeW, 13, false );
	m_CurtSkillAddEffect->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_CurtSkillAddEffect->SetClientImage( TID_None );
	m_CurtSkillAddEffect->SetAlignText( n_atCenter, n_atCenter );
	m_CurtSkillAddEffect->SetText( _RT("") );
	m_CurtSkillAddEffect->SetTextColor( 255, 234, 234 );
	m_CurtSkillAddEffect->SetHID( HID_None );
	m_CurtSkillAddEffect->SetFocusCheck( false );

	//설명문 .. 
	m_CurtSkillDesc = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_SkillLevelUp_CurtSkillDescText, WID_SkillLevelUpWnd, 
												  16, 130, ReSizeW - 30  , 117, false );
	m_CurtSkillDesc->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_CurtSkillDesc->SetClientImage( TID_None );
	m_CurtSkillDesc->SetAlignText( n_atCenter, n_atCenter );
	m_CurtSkillDesc->SetText( _RT("") );
	m_CurtSkillDesc->SetTextColor( 255, 255, 255 );
	m_CurtSkillDesc->SetMultiLine( true );
	m_CurtSkillDesc->SetHID( HID_None );
	m_CurtSkillDesc->SetFocusCheck( false );

	int nXPos = ReSizeW + 10;	
	
	m_NextSkillName = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_SkillLevelUp_NextSkillNameText, WID_SkillLevelUpWnd, nXPos, 108, ReSizeW, 13, false );
	m_NextSkillName->SetFontg( n_ftGulimChe, 12, n_fwBold, n_fdNormal );
	m_NextSkillName->SetClientImage( TID_None );
	m_NextSkillName->SetAlignText( n_atCenter, n_atCenter );
	m_NextSkillName->SetText( _RT("") );
	m_NextSkillName->SetTextColor( 222, 222, 255 );
	m_NextSkillName->SetHID( HID_None );
	m_NextSkillName->SetFocusCheck( false );

	m_NextSkillLeveOrPoint = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_SkillLevelUpWnd, nXPos, 130, ReSizeW, 13, false );
	m_NextSkillLeveOrPoint->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_NextSkillLeveOrPoint->SetClientImage( TID_None );
	m_NextSkillLeveOrPoint->SetAlignText( n_atCenter, n_atCenter );
	m_NextSkillLeveOrPoint->SetText( _RT("") );
	m_NextSkillLeveOrPoint->SetTextColor( 255, 222, 222 );
	m_NextSkillLeveOrPoint->SetHID( HID_None );
	m_NextSkillLeveOrPoint->SetFocusCheck( false );

	m_NextSkillNeedItem = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_SkillLevelUp_NextSkillNeedItemText, WID_SkillLevelUpWnd, nXPos, 130, ReSizeW, 13, false );
	m_NextSkillNeedItem->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_NextSkillNeedItem->SetClientImage( TID_None );
	m_NextSkillNeedItem->SetAlignText( n_atCenter, n_atCenter );
	m_NextSkillNeedItem->SetText( _RT("") );
	m_NextSkillNeedItem->SetTextColor( 255, 222, 222 );
	m_NextSkillNeedItem->SetHID( HID_None );
	m_NextSkillNeedItem->SetFocusCheck( false );
	
	m_NextSkillLevel = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_SkillLevelUp_NextSkillLevelText, WID_SkillLevelUpWnd, nXPos, 130, ReSizeW, 13, false );
	m_NextSkillLevel->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_NextSkillLevel->SetClientImage( TID_None );
	m_NextSkillLevel->SetAlignText( n_atCenter, n_atCenter );
	m_NextSkillLevel->SetText( _RT("") );
	m_NextSkillLevel->SetTextColor( 255, 255, 255 );
	m_NextSkillLevel->SetHID( HID_None );
	m_NextSkillLevel->SetFocusCheck( false );

	m_NextSkillGageType = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_SkillLevelUp_NextSkillGaugeTypeText, WID_SkillLevelUpWnd, nXPos, 130, ReSizeW, 13, false );
	m_NextSkillGageType->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_NextSkillGageType->SetClientImage( TID_None );
	m_NextSkillGageType->SetAlignText( n_atCenter, n_atCenter );
	m_NextSkillGageType->SetText( _RT("") );
	m_NextSkillGageType->SetTextColor( 255, 234, 234 );
	m_NextSkillGageType->SetHID( HID_None );
	m_NextSkillGageType->SetFocusCheck( false );

	m_NextSkillGageValue = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_SkillLevelUp_NextSkillGaugeValueText, WID_SkillLevelUpWnd, nXPos, 130, ReSizeW, 13, false );
	m_NextSkillGageValue->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_NextSkillGageValue->SetClientImage( TID_None );
	m_NextSkillGageValue->SetAlignText( n_atCenter, n_atCenter );
	m_NextSkillGageValue->SetText( _RT("") );
	m_NextSkillGageValue->SetTextColor( 255, 234, 234 );
	m_NextSkillGageValue->SetHID( HID_None );
	m_NextSkillGageValue->SetFocusCheck( false );

	m_NextSkillWastePoint = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_SkillLevelUp_NextSkillWastePointText, WID_SkillLevelUpWnd, nXPos, 130, ReSizeW, 13, false );
	m_NextSkillWastePoint->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_NextSkillWastePoint->SetClientImage( TID_None );
	m_NextSkillWastePoint->SetAlignText( n_atCenter, n_atCenter );
	m_NextSkillWastePoint->SetText( _RT("") );
	m_NextSkillWastePoint->SetTextColor( 222, 255, 222 );
	m_NextSkillWastePoint->SetHID( HID_None );
	m_NextSkillWastePoint->SetFocusCheck( false );

	m_NextSkillDelay = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_SkillLevelUpWnd, 14, 110, ReSizeW, 13, false );
	m_NextSkillDelay->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_NextSkillDelay->SetClientImage( TID_None );
	m_NextSkillDelay->SetAlignText( n_atCenter, n_atCenter );
	m_NextSkillDelay->SetText( _RT("") );
	m_NextSkillDelay->SetTextColor( 255, 234, 234 );
	m_NextSkillDelay->SetHID( HID_None );
	m_NextSkillDelay->SetFocusCheck( false );

	for(i = 0 ; i < 3 ; ++i)
	{
		m_NextSkillEffect[i] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_SkillLevelUpWnd, 14, 110, ReSizeW, 13, false );
		m_NextSkillEffect[i]->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
		m_NextSkillEffect[i]->SetClientImage( TID_None );
		m_NextSkillEffect[i]->SetAlignText( n_atCenter, n_atCenter );
		m_NextSkillEffect[i]->SetText( _RT("") );
		m_NextSkillEffect[i]->SetTextColor( 255, 234, 234 );
		m_NextSkillEffect[i]->SetHID( HID_None );
		m_NextSkillEffect[i]->SetFocusCheck( false );
	}

	m_NextSkillAddEffect = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_SkillLevelUpWnd, 14, 110, ReSizeW, 13, false );
	m_NextSkillAddEffect->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_NextSkillAddEffect->SetClientImage( TID_None );
	m_NextSkillAddEffect->SetAlignText( n_atCenter, n_atCenter );
	m_NextSkillAddEffect->SetText( _RT("") );
	m_NextSkillAddEffect->SetTextColor( 255, 234, 234 );
	m_NextSkillAddEffect->SetHID( HID_None );
	m_NextSkillAddEffect->SetFocusCheck( false );

	m_NextSkillDesc = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_SkillLevelUp_NextSkillDescText, WID_SkillLevelUpWnd, 
												  nXPos + 3, 130, ReSizeW - 30 , 117, false );
	m_NextSkillDesc->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_NextSkillDesc->SetClientImage( TID_None );
	m_NextSkillDesc->SetAlignText( n_atCenter, n_atCenter );
	m_NextSkillDesc->SetText( _RT("") );
	m_NextSkillDesc->SetTextColor( 255, 255, 255 );
	m_NextSkillDesc->SetMultiLine( true );
	m_NextSkillDesc->SetHID( HID_None );
	m_NextSkillDesc->SetFocusCheck( false );	

	Init();

}

void CSkillLevelUpWnd::Composition()
{
	CompositionNew();

	return;
}


//-----------------------------------------------------------------------------
void CSkillLevelUpWnd::Update()
{	
	return;
}

void CSkillLevelUpWnd::DrawCurtLvSkillInfo()
{
	char str_info[255] = {0,};

	m_pSkillIcon->SetClientImage( g_Pc_Manager.GetSkillIconTID( m_pPcItem->Code ) );

	SSkillBaseInfo* curt_skill_info = NULL;
	curt_skill_info = m_pPcItem->SkillTable;


	int info_posX =  6 ;
	int info_posY = 108;
	int add_posY = 13 + 3;

	Rsprintf( RWCHAR(str_info), _RT("%s"), RWCHAR(m_pPcItem->SkillTable->theSkillName) );
	m_CurtSkillName->SetText( str_info  );
	m_CurtSkillName->SetWndRepositionR( info_posX, info_posY );
//다음 스킬과 줄을 맞추기 위해 

	/*
	int info_posX = 14;
	int info_posY = 130;
	int add_posY = 13 + 3;

	_stprintf( str_info, "%s", m_pPcItem->SkillTable->theSkillName );
	m_CurtSkillName->SetText( _RT( str_info ) );

	_stprintf( str_info, "%s", m_pPcItem->SkillTable->theDesc );
	m_CurtSkillDesc->SetText( _RT( str_info ) );
	info_posY += add_posY;
	m_CurtSkillDesc->SetWndRepositionR( info_posX, info_posY );	

	m_pFontg = Fontman->GetFont( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	int str_num = m_pFontg->GetStringCount( m_pPcItem->SkillTable->theDesc );
	int line_num = GetNumLine( str_num, 24 );
	info_posY += ( line_num * add_posY );

	*/
	

	

	if( m_pPcItem->SkillTable->theNeedItem )
	{
		switch( m_pPcItem->SkillTable->theNeedItem )
		{
		case 1: Rsprintf( RWCHAR(str_info), _RT("%s:%s"), G_STRING_CHAR( IDS_WND_HT_NEEDITEM ),  G_STRING_CHAR( IDS_ITEM_TYPE_SWORD ) ); break;
		case 2: Rsprintf( RWCHAR(str_info), _RT("%s:%s"), G_STRING_CHAR( IDS_WND_HT_NEEDITEM ),G_STRING_CHAR( IDS_ITEM_TYPE_GSWORD ) ); break;
		case 3: Rsprintf( RWCHAR(str_info), _RT("%s:%s"), G_STRING_CHAR( IDS_WND_HT_NEEDITEM ),  G_STRING_CHAR( IDS_ITEM_TYPE_BOW ) ); break;
		case 4: Rsprintf( RWCHAR(str_info), _RT("%s:%s"), G_STRING_CHAR( IDS_WND_HT_NEEDITEM ), G_STRING_CHAR( IDS_ITEM_TYPE_MBOOK )); break;
		case 5: Rsprintf( RWCHAR(str_info), _RT("%s:%s"), G_STRING_CHAR( IDS_WND_HT_NEEDITEM ), G_STRING_CHAR( IDS_ITEM_TYPE_STAFF )); break;
		case 6: Rsprintf( RWCHAR(str_info), _RT("%s:%s"), G_STRING_CHAR( IDS_WND_HT_NEEDITEM ), G_STRING_CHAR( IDS_ITEM_TYPE_CRYBALL )); break;
		case 100: Rsprintf( RWCHAR(str_info), _RT("%s:%s"), G_STRING_CHAR( IDS_WND_HT_NEEDITEM ), G_STRING_CHAR( IDS_ITEM_TYPE_SHIELD )); break;
		}
		m_CurtSkillNeedItem->SetText( str_info );
		info_posY += add_posY;
		m_CurtSkillNeedItem->SetWndRepositionR( info_posX, info_posY );
	}
	else
	{
		m_CurtSkillNeedItem->SetText( _RT( "" ) );	
	}

	Rsprintf( RWCHAR(str_info), _RT("%s:%d"), G_STRING_CHAR( IDS_WND_SKILL_AB ), 
		g_SkillLevelUpWnd.GetActiveSKillLevel(m_pPcItem->SkillTable));  
	m_CurtSkillLevel->SetText( str_info );
	info_posY += add_posY;
	m_CurtSkillLevel->SetWndRepositionR( info_posX, info_posY );
	/*
#ifndef SKILL_HELP_TIP
	
	if( nRui->thePcParam.GageType )
	{
		switch( nRui->thePcParam.GageType )
		{
		case n_ComboGage: Rsprintf( RWCHAR(str_info), _RT("%s:%s"), G_STRING_CHAR( IDS_WND_NEED_GAUGE ), G_STRING_CHAR( IDS_WND_GAUGE_COMBO )); break;
		case n_ConditionGage: Rsprintf( RWCHAR(str_info), _RT("%s:%s"), G_STRING_CHAR( IDS_WND_NEED_GAUGE ), G_STRING_CHAR( IDS_WND_GAUGE_CONDITION ) ); break;
		case n_ProimaGage: Rsprintf( RWCHAR(str_info), _RT("%s:%s"), G_STRING_CHAR( IDS_WND_NEED_GAUGE ), G_STRING_CHAR( IDS_WND_GAUGE_PROIMA ) ); break;
		}		
		m_CurtSkillGageType->SetText( _RT( str_info ) );		
		info_posY += add_posY;
		m_CurtSkillGageType->SetWndRepositionR( info_posX, info_posY );
	}
	else
	{
		m_CurtSkillGageType->SetText( _RT( "" ) );	
	}

#endif
  */


	if( m_pPcItem->SkillTable->theGageValue )
	{
		int Value = m_pPcItem->SkillTable->theGageValue / 5;

		Rsprintf( RWCHAR(str_info) , _RT("%s:%d%s") , 	G_STRING_CHAR( IDS_WND_USE_GAUGE ), 
										Value, G_STRING_CHAR( IDS_PARTITION_UNIT ) ); 

		m_CurtSkillGageValue->SetText( str_info );	
		info_posY += add_posY;
		m_CurtSkillGageValue->SetWndRepositionR( info_posX, info_posY );	
	}
	else
	{
		m_CurtSkillGageValue->SetText( _RT( "" ) );	
	}


	str_info[0] = '\0';

	char add_str_info[255] = {0, };

	if( ( m_pPcItem->SkillTable->theWasteHP ) || ( m_pPcItem->SkillTable->theWasteMP ) || ( m_pPcItem->SkillTable->theWasteSP ) )
	{
		if( m_pPcItem->SkillTable->theWasteHP )
		{	
			Rsprintf( RWCHAR(str_info), _RT("%s:%d "), G_STRING_CHAR( IDS_WND_USE_HP ), m_pPcItem->SkillTable->theWasteHP ); 
		}
		if( m_pPcItem->SkillTable->theWasteMP )
		{
			if( m_pPcItem->SkillTable->theWasteMP )
			{
				Rsprintf( RWCHAR(add_str_info), _RT("%s:%d "), G_STRING_CHAR( IDS_WND_USE_MP ), m_pPcItem->SkillTable->theWasteMP );
				SAFE_STR_CAT( str_info, add_str_info, 255 );
			}
			else
			{
				Rsprintf( RWCHAR(str_info), _RT("%s:%d "), G_STRING_CHAR( IDS_WND_USE_MP ), m_pPcItem->SkillTable->theWasteMP ); 
			}
		}
		if( m_pPcItem->SkillTable->theWasteSP )
		{
			if( m_pPcItem->SkillTable->theWasteHP || m_pPcItem->SkillTable->theWasteMP )
			{
				Rsprintf( RWCHAR(add_str_info), _RT("%s:%d "), G_STRING_CHAR( IDS_WND_USE_SP ), m_pPcItem->SkillTable->theWasteSP ); 
				SAFE_STR_CAT( str_info, add_str_info, 255 ); 
			}
			else
			{
				Rsprintf( RWCHAR(str_info), _RT("%s:%d "), G_STRING_CHAR( IDS_WND_USE_SP ), m_pPcItem->SkillTable->theWasteSP ); 
			}
		}
		m_CurtSkillWastePoint->SetText( str_info) ;
		info_posY += add_posY;
		m_CurtSkillWastePoint->SetWndRepositionR( info_posX, info_posY );
	}
	else
	{
		m_CurtSkillWastePoint->SetText( _RT( "" ) );	
	}
		

	//딜레이 
	if( m_pPcItem->SkillTable->theDelayTime)
	{  
		Rsprintf( RWCHAR(str_info) , _RT("%s:%d %s"), G_STRING_CHAR(IDS_HELP_KEEP_UP_TIME), 
			m_pPcItem->SkillTable->theDelayTime/10 , G_STRING_CHAR(IDS_WND_CLOCK_SEC) );
		m_CurtSkillDelay->SetText( str_info );
		info_posY += add_posY;
		m_CurtSkillDelay->SetWndRepositionR( info_posX, info_posY );
	}
	else
	{
		m_CurtSkillDelay->SetText( _RT(""));
	}

//	SetHideHelpTip(ClWidth , nMaxStrWidthNum);


	int effectType[3];
	ZeroMemory(&effectType,sizeof(effectType));

	for(int i =0 ; i < 3 ; ++i)
	{
	
		if( m_pPcItem->SkillTable->theEffectInfo[i].theEffectType  && 
			m_pPcItem->SkillTable->theEffectInfo[i].theEffectValue != 0) 
		{
		
			if( GetEffectStrInfo(m_pPcItem , m_pPcItem->SkillTable ,  str_info , effectType , i ))
			{		
				m_CurtSkillEffect[i]->SetText( str_info );
				info_posY += add_posY;
				m_CurtSkillEffect[i]->SetWndRepositionR( info_posX, info_posY );
				
			}
			else
			{
				m_CurtSkillEffect[i]->SetText(_RT(""));
			}
		}
		else
		{
			m_CurtSkillEffect[i]->SetText(_RT(""));	
		}
	

	}


	//속성증폭은 뺀다
	if( m_pPcItem->SkillTable->theAddEffect > 0  && 
		m_pPcItem->SkillTable->theAddEffect != n_AmplifyAttri )
	{

		if( GetAddEffectInfo( m_pPcItem ,m_pPcItem->SkillTable  ,  str_info ))
		{
			m_CurtSkillAddEffect->SetText(str_info);
			info_posY += add_posY;
			m_CurtSkillAddEffect->SetWndRepositionR( info_posX, info_posY );	
		}
		else
		{
			m_CurtSkillAddEffect->SetText(_RT(""));
		}
		
	}
	else
	{
		m_CurtSkillAddEffect->SetText(_RT(""));
	}


	Rsprintf( RWCHAR(str_info), _RT("%s"), m_pPcItem->SkillTable->theDesc );
	m_CurtSkillDesc->SetText(  str_info  );
	info_posY += add_posY + (add_posY/2) ;

	m_CurtSkillDesc->SetWndRepositionR( info_posX + 16, info_posY );	

	//m_pFontg = Fontman->GetFont( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	//int str_num = m_pFontg->GetStringCount( m_pPcItem->SkillTable->theDesc );
	//int line_num = GetNumLine( str_num, 24 );
//	info_posY += ( line_num * add_posY );



/*	if( m_pPcItem->SkillTable->theProimaType )
	{
		switch( m_pPcItem->SkillTable->theProimaType )
		{
//		case 1:		_stprintf( str_info, "필요 프로이마:프리" ); break;
//		case 2:		_stprintf( str_info, "필요 프로이마:세스" ); break;
//		case 3:		_stprintf( str_info, "필요 프로이마:란트" ); break;
//		case 4:		_stprintf( str_info, "필요 프로이마:에아" ); break;
//		case 5:		_stprintf( str_info, "필요 프로이마:바크" ); break;
//		case 6:		_stprintf( str_info, "필요 프로이마:프리트" ); break;
//		case 7:		_stprintf( str_info, "필요 프로이마:세아스" ); break;
//		case 8:		_stprintf( str_info, "필요 프로이마:라이트" ); break;
//		case 9:		_stprintf( str_info, "필요 프로이마:에아르" ); break;
//		case 10:	_stprintf( str_info, "필요 프로이마:바사크" ); break;
//		case 11:	_stprintf( str_info, "필요 프로이마:프리세트" ); break;
//		case 12:	_stprintf( str_info, "필요 프로이마:세아시스" ); break;
//		case 13:	_stprintf( str_info, "필요 프로이마:라이아트" ); break;
//		case 14:	_stprintf( str_info, "필요 프로이마:에아리아" ); break;
//		case 15:	_stprintf( str_info, "필요 프로이마:바라사크" ); break;
//		case 16:	_stprintf( str_info, "필요 프로이마:파라세트" ); break;
//		case 17:	_stprintf( str_info, "필요 프로이마:아이아스" ); break;
//		case 18:	_stprintf( str_info, "필요 프로이마:그리아드" ); break;
//		case 19:	_stprintf( str_info, "필요 프로이마:브리너스" ); break;
//		case 20:	_stprintf( str_info, "필요 프로이마:케이아스" ); break;
//		case 21:	_stprintf( str_info, "필요 프로이마:샤크텐" ); break;
//		case 22:	_stprintf( str_info, "필요 프로이마:마하브" ); break;
//		case 23:	_stprintf( str_info, "필요 프로이마:가이아" ); break;
//		case 24:	_stprintf( str_info, "필요 프로이마:프리랏사" ); break;
//		case 25:	_stprintf( str_info, "필요 프로이마:테사라트" ); break;
//		case 30:	_stprintf( str_info, "필요 프로이마:카실라트" ); break;
//		case 31:	_stprintf( str_info, "필요 프로이마:알바트론" ); break;
		}
		m_CurtSkillProimaType->SetText( _T( str_info ) );	
		info_posY += add_posY;
		m_CurtSkillProimaType->SetWndRepositionR( info_posX, info_posY );
	}
	else
	{
		m_CurtSkillProimaType->SetText( _T( "" ) );	
	}
*/



	return;
}
//현재의 StateLevel 0 이면 현재 스킬레벨 검사 1이면 Max 레벨 검사 
//액티브 스킬이어야 하고 보조직업스킬을 제외한 스킬들은 -5씩하여 레벨 1로 보여주게 한다.. 
int CSkillLevelUpWnd::GetActiveSKillLevel(SSkillBaseInfo* SkillTable , int StateLevel /* = 0 */)
{
	if( SkillTable == NULL)
		return 0; 
	
	if(StateLevel == 0 )
	{
		
		if( SkillTable->theSkillType1 == 1 && SkillTable->theSkillType2 != 7 && 
			SkillTable->theSkillLevel > 5 && SkillTable->theMaxLevel <= 10 )
		{
			return SkillTable->theSkillLevel - 5; 
		}


		return SkillTable->theSkillLevel;
	}
	else
	{
		if( SkillTable->theSkillType1 == 1 && SkillTable->theSkillType2 != 7 && 
			SkillTable->theSkillLevel > 5 && SkillTable->theMaxLevel <= 10 )
		{
			return SkillTable->theMaxLevel - 5; 
		}

		return SkillTable->theMaxLevel;
	}

}



void CSkillLevelUpWnd::DrawNextLvSkillInfo()
{
	SSkillBaseInfo* next_skill_info = NULL;
	next_skill_info = g_Pc_Manager.GetSkillBaseInfoPtr( m_pPcItem->SkillTable->theSkillCode + 1);
	
	if( next_skill_info == NULL )
		return; 

	TCHAR str_info[255] = {0,};
	Rsprintf( RWCHAR(str_info), G_STRING_CHAR( IDS_WND_SKILL_UP_ALRAM ), next_skill_info->theNeedPoint );

	m_pLevelUpDesc->SetText(  str_info );

	
#ifdef APPLY_BOLD_FONT
	int info_posX =  302;
#else
	int info_posX =  257;
#endif
	
	int info_posY = 108;
	int add_posY = 13 + 3;

	Rsprintf( RWCHAR(str_info), _RT("%s"), RWCHAR(next_skill_info->theSkillName) );
	m_NextSkillName->SetText(  str_info  );
	m_NextSkillName->SetWndRepositionR( info_posX, info_posY );


/*

	int NeedLevel = next_skill_info->theNeedLevel;
	if(NeedLevel)
	{
		_stprintf( str_info, _T("%s: %s(%d) , %s(%d)"),G_STRING_CHAR( IDS_HELP_SKILL_UP_NEED_LEVEL ), G_STRING_CHAR(IDS_CHARSTATE_LEVEL),NeedLevel ,
					G_STRING_CHAR( IDS_HELP_POINT ), next_skill_info->theNeedPoint  );		//HAN_MSG
		
		//_stprintf( str_info, _T("%s:%d  %s:%d"),G_STRING_CHAR( IDS_WND_NEED_LEVEL_SKILL_UP ), NeedLevel ,"포인트",next_skill_info->theNeedPoint  );		//HAN_MSG
		m_NextSkillLeveOrPoint->SetText(_T(str_info));
		info_posY += add_posY;
		m_NextSkillLeveOrPoint->SetWndRepositionR( info_posX, info_posY );
	}
*/

	if( next_skill_info->theNeedItem )
	{
		switch( next_skill_info->theNeedItem )
		{
		case 1: Rsprintf( RWCHAR(str_info), _RT("%s:%s"), G_STRING_CHAR( IDS_WND_HT_NEEDITEM ), G_STRING_CHAR( IDS_ITEM_TYPE_SWORD ) ); break;
		case 2: Rsprintf( RWCHAR(str_info), _RT("%s:%s"), G_STRING_CHAR( IDS_WND_HT_NEEDITEM ), G_STRING_CHAR( IDS_ITEM_TYPE_GSWORD ) ); break;
		case 3: Rsprintf( RWCHAR(str_info), _RT("%s:%s"), G_STRING_CHAR( IDS_WND_HT_NEEDITEM ), G_STRING_CHAR( IDS_ITEM_TYPE_BOW ) ); break;
		case 4: Rsprintf( RWCHAR(str_info), _RT("%s:%s"), G_STRING_CHAR( IDS_WND_HT_NEEDITEM ), G_STRING_CHAR( IDS_ITEM_TYPE_MBOOK ) ); break;
		case 5: Rsprintf( RWCHAR(str_info), _RT("%s:%s"), G_STRING_CHAR( IDS_WND_HT_NEEDITEM ), G_STRING_CHAR( IDS_ITEM_TYPE_STAFF ) ); break;
		case 6: Rsprintf( RWCHAR(str_info), _RT("%s:%s"), G_STRING_CHAR( IDS_WND_HT_NEEDITEM ), G_STRING_CHAR( IDS_ITEM_TYPE_CRYBALL ) ); break;
		case 100: Rsprintf( RWCHAR(str_info), _RT("%s:%s"), G_STRING_CHAR( IDS_WND_HT_NEEDITEM ), G_STRING_CHAR( IDS_ITEM_TYPE_SHIELD ) ); break;
		}
		m_NextSkillNeedItem->SetText( str_info  );
		info_posY += add_posY;
		m_NextSkillNeedItem->SetWndRepositionR( info_posX, info_posY );
	}
	else
	{
		m_NextSkillNeedItem->SetText( _RT( "" ) );	
	}

	Rsprintf( RWCHAR(str_info), _RT("%s:%d"), G_STRING_CHAR( IDS_WND_SKILL_AB ), g_SkillLevelUpWnd.GetActiveSKillLevel( next_skill_info ) );
	m_NextSkillLevel->SetText( str_info );
	info_posY += add_posY;
	m_NextSkillLevel->SetWndRepositionR( info_posX, info_posY );
/*
#ifndef SKILL_HELP_TIP
	if( nRui->thePcParam.GageType )
	{
		switch( nRui->thePcParam.GageType )
		{
		case 1: Rsprintf( RWCHAR(str_info), _RT("%s:%s"), G_STRING_CHAR( IDS_WND_NEED_GAUGE ), G_STRING_CHAR( IDS_WND_GAUGE_COMBO )); break;
		case 2: Rsprintf( RWCHAR(str_info), _RT("%s:%s"), G_STRING_CHAR( IDS_WND_NEED_GAUGE ), G_STRING_CHAR( IDS_WND_GAUGE_CONDITION )); break;
		case 3: Rsprintf( RWCHAR(str_info), _RT("%s:%s"), G_STRING_CHAR( IDS_WND_NEED_GAUGE ), G_STRING_CHAR( IDS_WND_GAUGE_PROIMA )); break;
		}		
		m_NextSkillGageType->SetText( _RT( str_info ) );	
		info_posY += add_posY;
		m_NextSkillGageType->SetWndRepositionR( info_posX, info_posY );
	}
	else
	{
		m_NextSkillGageType->SetText( _RT( "" ) );	
	}
#endif 
*/
	if( next_skill_info->theGageValue )
	{
		int Value = next_skill_info->theGageValue / 5;
		
		Rsprintf( RWCHAR(str_info) , _RT("%s:%d%s"), 	G_STRING_CHAR( IDS_WND_USE_GAUGE ), 
										Value, G_STRING_CHAR( IDS_PARTITION_UNIT ) ); 
		m_NextSkillGageValue->SetText(  str_info  );	
		info_posY += add_posY;
		m_NextSkillGageValue->SetWndRepositionR( info_posX, info_posY );	
	}
	else
	{
		m_NextSkillGageValue->SetText( _RT( "" ) );	
	}

	str_info[0] = '\0';
	
	char add_str_info[255] = {0, };
	
	if( ( next_skill_info->theWasteHP ) || ( next_skill_info->theWasteMP ) || ( next_skill_info->theWasteSP ) )
	{
		if( next_skill_info->theWasteHP )
		{	
			Rsprintf( RWCHAR(str_info), _RT("%s:%d "), G_STRING_CHAR( IDS_WND_USE_HP ), next_skill_info->theWasteHP ); 
		}
		if( next_skill_info->theWasteMP )
		{
			if( next_skill_info->theWasteMP )
			{
				Rsprintf( RWCHAR(add_str_info), _RT("%s:%d "), G_STRING_CHAR( IDS_WND_USE_MP ), next_skill_info->theWasteMP );
				SAFE_STR_CAT( str_info, add_str_info, 255 );
			}
			else
			{
				Rsprintf( RWCHAR(str_info), _RT("%s:%d "), G_STRING_CHAR( IDS_WND_USE_MP ), next_skill_info->theWasteMP ); 
			}
		}
		if( next_skill_info->theWasteSP )
		{
			if( next_skill_info->theWasteHP || next_skill_info->theWasteMP )
			{
				Rsprintf( RWCHAR(add_str_info), _RT("%s:%d "), G_STRING_CHAR( IDS_WND_USE_SP ), next_skill_info->theWasteSP ); 
				SAFE_STR_CAT( str_info, add_str_info, 255 ); 
			}
			else
			{
				Rsprintf( RWCHAR(str_info), _RT("%s:%d "), G_STRING_CHAR( IDS_WND_USE_SP ), next_skill_info->theWasteSP ); 
			}
		}
		m_NextSkillWastePoint->SetText( str_info);
		info_posY += add_posY;
		m_NextSkillWastePoint->SetWndRepositionR( info_posX, info_posY );
	}
	else
	{
		m_NextSkillWastePoint->SetText( _RT( "" ) );	
	}
		
		//딜레이 
	if( next_skill_info->theDelayTime)
	{  
		Rsprintf( RWCHAR(str_info) , _RT("%s:%d%s"),G_STRING_CHAR(IDS_HELP_KEEP_UP_TIME) ,  
			next_skill_info->theDelayTime/10 , G_STRING_CHAR(IDS_WND_CLOCK_SEC));
		m_NextSkillDelay->SetText( str_info );
		info_posY += add_posY;
		m_NextSkillDelay->SetWndRepositionR( info_posX, info_posY );
	}
	else
	{
		m_NextSkillDelay->SetText( _RT(""));
	}

//	SetHideHelpTip(ClWidth , nMaxStrWidthNum);


	int effectType[3];
	ZeroMemory(&effectType,sizeof(effectType));

	for(int i =0 ; i < 3 ; ++i)
	{
	
		if( next_skill_info->theEffectInfo[i].theEffectType  && 
			next_skill_info->theEffectInfo[i].theEffectValue != 0) 
		{
			
			if( GetEffectStrInfo( m_pPcItem , next_skill_info , str_info , effectType , i ))
			{
				m_NextSkillEffect[i]->SetText( str_info );
				info_posY += add_posY;
				m_NextSkillEffect[i]->SetWndRepositionR( info_posX, info_posY );
			}
			else
			{
				m_NextSkillEffect[i]->SetText(_RT(""));	
			}
			
		}
		else
		{
			m_NextSkillEffect[i]->SetText(_RT(""));	
		}
	

	}


	//속성증폭은 뺀다
	if( next_skill_info->theAddEffect > 0  && 
		next_skill_info->theAddEffect != n_AmplifyAttri )
	{
				
		if( GetAddEffectInfo( NULL , next_skill_info,str_info)) 
		{
			m_NextSkillAddEffect->SetText(str_info);
			info_posY += add_posY;
			m_NextSkillAddEffect->SetWndRepositionR( info_posX, info_posY );	
		}
		else
		{
			m_NextSkillAddEffect->SetText(_RT(""));
		}
		
	}
	else
	{
		m_NextSkillAddEffect->SetText(_RT(""));
	}


	Rsprintf( RWCHAR(str_info), _RT("%s") , RWCHAR(next_skill_info->theDesc) );
	m_NextSkillDesc->SetText(  str_info );
	info_posY += add_posY+ (add_posY/2);
	m_NextSkillDesc->SetWndRepositionR( info_posX + 16, info_posY );	


/*	if( next_skill_info->theProimaType )
	{
		switch( next_skill_info->theProimaType )
		{
//		case 1:		_stprintf( str_info, "필요 프로이마:프리" ); break;
//		case 2:		_stprintf( str_info, "필요 프로이마:세스" ); break;
//		case 3:		_stprintf( str_info, "필요 프로이마:란트" ); break;
//		case 4:		_stprintf( str_info, "필요 프로이마:에아" ); break;
//		case 5:		_stprintf( str_info, "필요 프로이마:바크" ); break;
//		case 6:		_stprintf( str_info, "필요 프로이마:프리트" ); break;
//		case 7:		_stprintf( str_info, "필요 프로이마:세아스" ); break;
//		case 8:		_stprintf( str_info, "필요 프로이마:라이트" ); break;
//		case 9:		_stprintf( str_info, "필요 프로이마:에아르" ); break;
//		case 10:	_stprintf( str_info, "필요 프로이마:바사크" ); break;
//		case 11:	_stprintf( str_info, "필요 프로이마:프리세트" ); break;
//		case 12:	_stprintf( str_info, "필요 프로이마:세아시스" ); break;
//		case 13:	_stprintf( str_info, "필요 프로이마:라이아트" ); break;
//		case 14:	_stprintf( str_info, "필요 프로이마:에아리아" ); break;
//		case 15:	_stprintf( str_info, "필요 프로이마:바라사크" ); break;
//		case 16:	_stprintf( str_info, "필요 프로이마:파라세트" ); break;
//		case 17:	_stprintf( str_info, "필요 프로이마:아이아스" ); break;
//		case 18:	_stprintf( str_info, "필요 프로이마:그리아드" ); break;
//		case 19:	_stprintf( str_info, "필요 프로이마:브리너스" ); break;
//		case 20:	_stprintf( str_info, "필요 프로이마:케이아스" ); break;
//		case 21:	_stprintf( str_info, "필요 프로이마:샤크텐" ); break;
//		case 22:	_stprintf( str_info, "필요 프로이마:마하브" ); break;
//		case 23:	_stprintf( str_info, "필요 프로이마:가이아" ); break;
//		case 24:	_stprintf( str_info, "필요 프로이마:프리랏사" ); break;
//		case 25:	_stprintf( str_info, "필요 프로이마:테사라트" ); break;
//		case 30:	_stprintf( str_info, "필요 프로이마:카실라트" ); break;
//		case 31:	_stprintf( str_info, "필요 프로이마:알바트론" ); break;
//		}
//		m_NextSkillProimaType->SetText( _T( str_info ) );	
//		info_posY += add_posY;
		m_NextSkillProimaType->SetWndRepositionR( info_posX, info_posY );
	}
	else
	{
		m_NextSkillProimaType->SetText( _T( "" ) );	
	}
*/	
	return;
}



bool CSkillLevelUpWnd::GetAddEffectInfo(SPcItem* PcItem ,SSkillBaseInfo* SkillTable , TCHAR *StrInfo)
{
	TCHAR StrKeepupTime[64] = {0,};
	TCHAR StrSuccRate[64]= {0,};
	TCHAR StrEffectValue[64]= {0,};
	TCHAR StrName[64]= {0,};
	
	if(SkillTable->theAddEffectTime > 0 )
	{
		Rsprintf(RWCHAR(StrKeepupTime) , _RT("(%d %s)"),SkillTable->theAddEffectTime/10,G_STRING_CHAR(IDS_WND_CLOCK_SEC));	//HAN_MSG
	}
	
	GetAddEffectName(StrName , SkillTable->theAddEffect);
				
	
	switch(SkillTable->theAddEffect)
	{				
		
		
		//초도 필요 없다 
	case	n_Aura:						// 오라 
	case	n_Kill:						// 즉사
	case	n_Teleport:					// 전방으로 순간 이동 (거리적용가능)
	case	n_WideAttack:				// 광역 공격 
	case	n_AmplifyAttri:				// 속성 증폭 (속성 정보 추가)
	case	n_AliveEffect:		// 부활
	case	n_Death_Potion:  					//즉사
	case 	n_Indomitable:		// 불굴의 의지
	case	n_DetectCombeBase:			// 콤보 이동불가 면역
		return FALSE;					
		
		//지속시간 부과효과 이름 보여주는것 
	case	n_NotMove:		// 이동불가
	case	n_Paralysis:		// 마비
	case	n_ReflexAttack:	// 받아치기 
	case 	n_ReflaxSkill:	
	case 	n_Provoke:		 	// 도발
	case	n_Invisible:			// 투명효과( 마지막 공격, 공격받은 시점으로부터 30초 후 경과한 후 적용가능)
	case	n_InvisibleShort:		// 순간 투명효과 (언제든 사용가능)
		
	case	n_Sleep:				// 공격받으면 깨어남.
	case	n_ReturnTown:			// 마을로 돌아감
	case	n_OtherSelf:			// 분신
	case	n_NotSkill:				// 스킬 불가 
	case	n_ImpossibleCombo :		// 콤보 봉인 
		
		if(SkillTable->theAddEffectRate != 100 )
		{
			Rsprintf(RWCHAR(StrSuccRate) , _RT("[%s %d%%]") , G_STRING_CHAR(IDS_HELP_SUCCESS) , SkillTable->theAddEffectRate );
		}
		Rsprintf( RWCHAR(StrInfo), _RT("%s %s%s%s") , RWCHAR(StrName) , RWCHAR(StrEffectValue) , RWCHAR(StrKeepupTime) , RWCHAR(StrSuccRate));
		break;
		
	case	n_Dispell_Immunity_Potion: 		//디스펠 면역
	case	n_Paralysis_Immunity_Potion: 	//마비 면역
	case	n_Move_Cancle_Potion: 			//자유(이동불가 해제)
	case	n_Magic_Immunity_Potion: 	//마법 면역
	case	n_Attack_Immunity_Potion: 	//물리 공격 면역
	case	n_Detect_Potion: 				//은신 감지
		if(SkillTable->theAddEffectRate != 100 )
		{
			Rsprintf(RWCHAR(StrSuccRate) , _RT("[%s %d%%]") , G_STRING_CHAR(IDS_HELP_SUCCESS) , SkillTable->theAddEffectRate );
		}
		Rsprintf( RWCHAR(StrInfo), _RT("%s %s") , RWCHAR(StrKeepupTime ), RWCHAR(StrSuccRate));
		break;
		//디스펠은 100% 도 표기한다.
	case	n_BadDispel:				// 디스펠 (필살기계, 비연속적필살기계, 마법공격계. 적용 스킬 갯수 추가)
	case	n_GoodDispel:				// 디스펠 (회복계, 좋은 보조계.  적용 스킬 갯수 추가)
	case	n_Detect:					// 투명상태 유저 검색 기능 	
		if(SkillTable->theAddEffectRate)
		{
			Rsprintf(RWCHAR(StrSuccRate) , _RT("[%s %d%%]") , G_STRING_CHAR(IDS_HELP_SUCCESS) , SkillTable->theAddEffectRate );
			Rsprintf( RWCHAR(StrInfo), _RT("%s") , RWCHAR(StrSuccRate));
		}
		break;
		
	case	n_ReduceDamage:				// 데미지 흡수 
		Rsprintf( RWCHAR(StrEffectValue) , _RT("+%d%%") ,SkillTable->theAddEffectValue);
		Rsprintf( RWCHAR(StrInfo), _RT("%s %s %s%s") , RWCHAR(StrName) , RWCHAR(StrEffectValue) , RWCHAR(StrKeepupTime) , RWCHAR(StrSuccRate));
		break;
		
		
		
	default:break;
		
	}
	


	return TRUE;

}




bool CSkillLevelUpWnd::GetEffectStrInfo(SPcItem* pcItem , SSkillBaseInfo* SkillTable , TCHAR *StrInfo , int *effectType , int Num)
{
	//SSkillBaseInfo* SkillTable = pcItem->SkillTable;

	_Effect EffectInfo;
	memcpy(&EffectInfo , &SkillTable->theEffectInfo[Num], sizeof(EffectInfo) );
	

	TCHAR StrEffect[256]= {0,};
	TCHAR StrAfter[64] = {0,};
	TCHAR StrKeepupTime[64] = {0,};
	TCHAR StrSuccRate[64]= {0,};
	TCHAR StrTypeName[64]="";
	TCHAR StrPlusAndMinus[64]={0,};
	TCHAR StrAdd[64]= {0,};

	

	int skillcase = 0;
	BYTE effect_type = 0;
	effect_type = EffectInfo.theEffectType;
	
	skillcase = effect_type / 30;
	int DamageState = 0 ; 
	int iSeeType = 0 ; 
				
	int EffectValue = EffectInfo.theEffectValue;
#ifdef HOLY_SKILLICON_MODIFY
	if(pcItem && g_RockClient.IsHolySkill(pcItem->Code) ) 
	{
		EffectValue = EffectValue + pcItem->HolyLevel ; 
	}
#endif

	switch(skillcase) {
	case 0:
		{
			
			if(EffectValue >= -1)
			{
				//증가 
				DamageState = 1;
			}
			else
			{
				//감소 
				EffectValue = abs(EffectValue);
				DamageState = 0;
			
				switch(effect_type)
				{
					case n_MagicResAdd:
					case n_AttackDefenseAdd:
						EffectValue *= 10;
					break;	
				}
			}
			
			Rsprintf( RWCHAR(StrAfter) , _RT("%d"),EffectValue);
			
			break;
		}
		
	case 1:
		{
			//배수값 인식 
			effect_type -= 30;
			DamageState = 1; 
			Rsprintf( RWCHAR(StrAfter) , _RT("%d%s"),EffectValue/10 , G_STRING_CHAR(IDS_HELP_CRITICAL));
			break;
		}
	case 2:
		{
			//%비율로 100 이하면 감소 증가 
			effect_type -= 60;
			
			if( EffectValue < 100)
			{
				EffectValue = 100 - EffectValue;
				DamageState = 0;
			}
			else
			{
				switch(effect_type) {
				case n_MoveSpeedAdd:
				case n_AttackPowerAdd:
				case n_AttackDefenseAdd:
				case n_MagicPowerAdd:
				case n_MagicResAdd:
				case n_CommonPowerAdd:	
					EffectValue = EffectValue - 100 ;
					break;
				default: break;
				}
				EffectValue = EffectValue;
				DamageState = 1;
				
			}
			
			Rsprintf( RWCHAR(StrAfter) , _RT("%d%%"),EffectValue);
			
			break;
		}
		
	default: break; 
	}
	
	ZeroMemory(&StrAdd , sizeof(StrAdd));
	if((effect_type == n_AttackBlowAdd ||
		effect_type == n_MagicBlowAdd ) &&
		Num > 0 )
	{
		//	effectType[i] = effect_type;	
		Rstrcpy( RWCHAR(StrAdd) , G_STRING_CHAR(IDS_HELP_ADD_DEMAGE));
	}
	int iNation  =0  ; 
	if( pcItem ) 
	{
		iNation = pcItem->Nation;
	}
	
	GetEffectName(effect_type , StrTypeName ,iSeeType  , iNation );
	
	if( iSeeType == 0 ) 
	{
		return FALSE;
	}
	
	if( DamageState == 0 ) 
	{
		Rstrcpy(RWCHAR(StrPlusAndMinus) ,_RT("-"));
	}
	else if(DamageState == 1 )
	{
		Rstrcpy(RWCHAR(StrPlusAndMinus) , _RT("+"));
	}
	
	//지속시간 

	if(EffectInfo.theKeepupTime > 0 )
	{
		Rsprintf(RWCHAR(StrKeepupTime) , _RT("(%d %s)"),EffectInfo.theKeepupTime/10 ,G_STRING_CHAR(IDS_WND_CLOCK_SEC));
	}

	
	//성공률
	if(EffectInfo.theSuccRate != 100 )
	{
		Rsprintf(RWCHAR(StrSuccRate) , _RT("[%s %d%%]") , G_STRING_CHAR(IDS_HELP_SUCCESS), EffectInfo.theSuccRate);
	}
	
	//추가데미지 값이 있느냐 ? 
	if( StrAdd[0] != NULL ) 
	{
		Rsprintf( RWCHAR(StrEffect) ,_RT("%s %s%s"),RWCHAR(StrAdd), RWCHAR(StrPlusAndMinus) , RWCHAR(StrAfter) );
	}
	else
	{
		Rsprintf( RWCHAR(StrEffect) , _RT("%s%s %s%s"),RWCHAR(StrAdd), RWCHAR(StrTypeName) , RWCHAR(StrPlusAndMinus) , RWCHAR(StrAfter) );
	}
		
	// HP + 10 /2초(10초) 
	if(iSeeType == 1 )
	{
		//지속시간이 있냐? 
		if( (EffectInfo.theKeepupTime > 0 ) || (EffectInfo.theKeepupTime == -1)  )
		{
			//한국이나일본은 같고 그외는 ㅋㅋ 
			if( g_RockClient.GetLanguage() == eLang_Korea || 
				g_RockClient.GetLanguage() == eLang_Japan ||
				g_RockClient.GetLanguage() == eLang_China || 
				g_RockClient.GetLanguage() == eLang_Taiwan || 
				g_RockClient.GetLanguage() == eLang_HongKong )
			{
				Rsprintf( RWCHAR(StrInfo), _RT("%s %s %s %s"),G_STRING_CHAR(IDS_HELP_EVERY_TWO_SEC), RWCHAR(StrEffect) ,		//HAN_MSG
					RWCHAR(StrKeepupTime), RWCHAR(StrSuccRate) );
			}
			else
			{
				Rsprintf( RWCHAR(StrInfo), _RT(" %s %s %s %s"), RWCHAR(StrEffect) ,G_STRING_CHAR(IDS_HELP_EVERY_TWO_SEC),
					RWCHAR(StrKeepupTime), RWCHAR(StrSuccRate) );
			}
			
		}
		else
		{
			Rsprintf( RWCHAR(StrInfo), _RT("%s %s %s"), RWCHAR(StrEffect) , 
				RWCHAR(StrKeepupTime), RWCHAR(StrSuccRate) );
		}
	}
	// HP + 10 
	else if(iSeeType == 2 )
	{
		Rsprintf( RWCHAR(StrInfo), _RT("%s %s %s"), RWCHAR(StrEffect) , 
			RWCHAR(StrKeepupTime), RWCHAR(StrSuccRate) );
	}
	
	return TRUE;
}



bool CSkillLevelUpWnd::GetEffectName(int Type , TCHAR *StrTypeName , int &iSeeType , int Nation)
{

	switch(Type)
		{
		
		case n_HPAdd:	
			Rstrcpy( RWCHAR(StrTypeName), G_STRING_CHAR(IDS_CHARSTATE_HP));	
			iSeeType = 1; 
			break;
		case n_SPAdd:			
			Rstrcpy( RWCHAR(StrTypeName), G_STRING_CHAR(IDS_CHARSTATE_SP));
			iSeeType = 1; 
			break;	
		case n_MPAdd:				
			Rstrcpy( RWCHAR(StrTypeName), G_STRING_CHAR(IDS_CHARSTATE_MP));
			iSeeType = 1; 
			break;
		case n_VitalAdd:	// 체력 (HP)		
			Rstrcpy( RWCHAR(StrTypeName), G_STRING_CHAR(IDS_CHARSTATE_VITAL2));
			iSeeType = 2; 
			break;
		case n_StaminaAdd: // 지구력 (SP)
			Rstrcpy( RWCHAR(StrTypeName), G_STRING_CHAR(IDS_CHARSTATE_STAMINA2));
			iSeeType = 2; 
			break;					
		case n_IntAdd:						// 지능 (MP)
			Rstrcpy( RWCHAR(StrTypeName), G_STRING_CHAR(IDS_CHARSTATE_INTELLI2));
			iSeeType = 2; 
			break;
		case n_DexAdd:						// 민첩 
			Rstrcpy( RWCHAR(StrTypeName), G_STRING_CHAR(IDS_CHARSTATE_DEX2));
			iSeeType = 2; 
			break;
		case n_LuckAdd:						// 행운
			Rstrcpy( RWCHAR(StrTypeName), G_STRING_CHAR(IDS_HELP_LUCK2));
			iSeeType = 2; 
			break;
		case n_HPSpeedAdd:					// HP 회복속도
			Rstrcpy( RWCHAR(StrTypeName), _RT("HP"));
			iSeeType = 0 ; 
			break;
		case n_SPSpeedAdd:			// SP 회복속도
			Rstrcpy( RWCHAR(StrTypeName), _RT("SP"));
			iSeeType = 0 ; 
			break;
		case n_MPSpeedAdd:					// MP 회복속도 
			Rstrcpy( RWCHAR(StrTypeName), _RT("MP"));
			iSeeType = 0 ; 
			break;
		case n_AttackSpeedAdd:				// 12 공격속도(물리)
			Rstrcpy( RWCHAR(StrTypeName), G_STRING_CHAR(IDS_CHARSTATE_ATTSPEED));
			iSeeType = 2 ; 
			break;
		case n_MoveSpeedAdd	:				// 13 이동속도
			Rstrcpy( RWCHAR(StrTypeName), G_STRING_CHAR(IDS_CHARSTATE_MOVSPEED));
			iSeeType = 2 ; 
			break;
		case n_AttackPowerAdd:				// 물리공격(최소, 최대치 모두 증가)
			Rstrcpy( RWCHAR(StrTypeName),G_STRING_CHAR(IDS_CHARSTATE_PHYATT));	
			iSeeType = 2 ; 
			break;
		case n_AttackDefenseAdd:				// 물리방어
			Rstrcpy( RWCHAR(StrTypeName), G_STRING_CHAR(IDS_CHARSTATE_PHYDEF));
			iSeeType = 2 ; 
			break;
		case n_DefenseAttriAdd:				// 방어속성
			Rstrcpy( RWCHAR(StrTypeName), _RT(""));
			iSeeType = 0 ; 
			break;
		case n_MagicPowerAdd:			// 마법공격
			Rstrcpy( RWCHAR(StrTypeName), G_STRING_CHAR(IDS_CHARSTATE_MAGATT));
			iSeeType = 2 ; 
			break;
		case n_MagicResAdd:				// 마법방어
			Rstrcpy( RWCHAR(StrTypeName), G_STRING_CHAR(IDS_CHARSTATE_MAGDEF));
			iSeeType = 2 ; 
			break;
		case n_AttackBlowAdd:			// 물리 필살기
			Rstrcpy( RWCHAR(StrTypeName), G_STRING_CHAR(IDS_HELP_PHY_DAMAGE));
			iSeeType = 2 ; 
			break;
		case n_MagicBlowAdd:				// 마법 필살기
			Rstrcpy( RWCHAR(StrTypeName), G_STRING_CHAR(IDS_HELP_MAG_DAMAGE));
			iSeeType = 2 ; 
			break;
		case n_GageAdd:				// 게이지 증가량 추가 (획득시 추가획득), only add
			Rstrcpy( RWCHAR(StrTypeName), G_STRING_CHAR(IDS_WND_GAUGE_COMBO));
			iSeeType = 2 ; 
			break;
		case n_HPtoMP:		// 체력을 마나로 전환, 소모 HP 기준으로 회복량 계산 
			Rstrcpy( RWCHAR(StrTypeName), _RT("MP"));
			iSeeType = 2 ; 
			break;
		case n_HPtoSP:				
			iSeeType = 2 ; 
			Rstrcpy( RWCHAR(StrTypeName), _RT("SP"));
			break;
		// 물공 or 마공
		case n_CommonPowerAdd:
			if( Nation  == RAIN)
			{
				Rstrcpy( RWCHAR(StrTypeName), G_STRING_CHAR(IDS_CHARSTATE_MAGATT));
			}
			else if(Nation  == MILLENA  ) 
			{
				Rstrcpy( RWCHAR(StrTypeName),G_STRING_CHAR(IDS_CHARSTATE_PHYATT));	
			}
			else
			{
				Rstrcpy( RWCHAR(StrTypeName),G_STRING_CHAR(IDS_HELP_ATTACK));	
			}
			iSeeType = 2 ; 
			break;
		case n_MaxHPAdd:
			Rstrcpy( RWCHAR(StrTypeName), G_STRING_CHAR(IDS_HELP_MAX_HP));	
			iSeeType = 2 ; 
			break;
		case n_MaxSMPAdd:
			if( Nation  == RAIN)
			{
				Rstrcpy( RWCHAR(StrTypeName), G_STRING_CHAR(IDS_HELP_MAX_MP));	
			}
			else
			{
				Rstrcpy( RWCHAR(StrTypeName), G_STRING_CHAR(IDS_HELP_MAX_SP));	
			}
			iSeeType = 2 ; 
			break;
		default: break; 
		}	

	return TRUE;
}


bool CSkillLevelUpWnd::GetAddEffectName(TCHAR *Str ,int iAddEffect)
{
	/*
	n_NotMove		= 1,	// 이동불가
	n_Paralysis		= 2,	// 마비
	n_ReflexAttack	= 3,	// 받아치기 
	n_ReflaxSkill	= 4,
	n_Provoke		= 5,	// 도발
	n_Indomitable	= 6,	// 불굴의 의지
	n_AliveEffect	= 7,	// 부활
	n_BadDispel		= 8,	// 디스펠 (필살기계, 비연속적필살기계, 마법공격계. 적용 스킬 갯수 추가)
	n_Aura			= 9,	// 
	n_GoodDispel	= 10,	// 디스펠 (회복계, 좋은 보조계.  적용 스킬 갯수 추가)
	n_Invisible		= 11,	// 투명효과( 마지막 공격, 공격받은 시점으로부터 30초 후 경과한 후 적용가능)
	n_InvisibleShort = 12,	// 순간 투명효과 (언제든 사용가능)
	n_Detect		= 13,	// 투명상태 유저 검색 기능 
	n_Sleep			= 14,	// 공격받으면 깨어남.
	n_ReturnTown	= 15,	// 마을로 돌아감
	n_Teleport		= 16,	// 전방으로 순간 이동 (거리적용가능)
	n_Kill			= 17,	// 즉사
	n_WideAttack	= 18,	// 광역 공격 
	n_AmplifyAttri	= 19,	// 속성 증폭 (속성 정보 추가)
	n_OtherSelf		= 20,	// 분신
	n_NotSkill		= 21,	// 스킬 불가 
	n_ImpossibleCombo = 22,	// 콤보 봉인 
	n_DetectCombeBase = 23,	// 콤보 이동불가 면역
	n_ReduceDamage	= 24,	// 데미지 흡수 
	n_Dispell_Immunity_Potion = 25,		//디스펠 면역
	n_Paralysis_Immunity_Potion = 26,	//마비 면역
	n_Move_Cancle_Potion = 27,			//자유(이동불가 해제)
	n_Death_Potion = 28,					//즉사
	n_Magic_Immunity_Potion = 29,		//마법 면역
	n_Attack_Immunity_Potion = 30,		//물리 공격 면역
	n_Detect_Potion = 31,				//은신 감지

  
*/

	//g_Pc_Manager.theAddEffectInfoTable[skill_Info->theAddEffect - 1].theAddeffect

	switch(iAddEffect)
	{	
		case	n_NotMove:		// 이동불가
			Rsprintf(RWCHAR(Str) ,_RT( "%s"), G_STRING_CHAR(IDS_HELP_NO_MOVEMENT));
			break;
		case	n_Paralysis:		// 마비
			Rsprintf(RWCHAR(Str) ,_RT( "%s"), G_STRING_CHAR(IDS_HELP_PRALYSIS));
			break;
		case	n_ReflexAttack:	// 받아치기 
			Rsprintf(RWCHAR(Str) ,_RT( "%s"), G_STRING_CHAR(IDS_HELP_COUNTER_ATTACK));
			break;
		case 	n_ReflaxSkill:	// 되돌리기 
			Rsprintf(RWCHAR(Str) ,_RT( "%s"), G_STRING_CHAR(IDS_HELP_HIT_BACK));
			break;
		case 	n_Provoke:		 	// 도발
			Rsprintf(RWCHAR(Str) ,_RT( "%s"), G_STRING_CHAR(IDS_HELP_PROVOKE));
			break;
		case	n_Invisible:			// 투명효과( 마지막 공격, 공격받은 시점으로부터 30초 후 경과한 후 적용가능)
		case	n_InvisibleShort:		// 순간 투명효과 (언제든 사용가능)
			Rsprintf(RWCHAR(Str) ,_RT( "%s"), G_STRING_CHAR(IDS_HELP_TRANSPARAN));
			break;
		case	n_Sleep:				// 공격받으면 깨어남.
			Rsprintf(RWCHAR(Str) ,_RT( "%s"), G_STRING_CHAR(IDS_HELP_FAINT));
			break;
	//	case	n_ReturnTown:			// 마을로 돌아감
	//		Rsprintf(RWCHAR(Str) ,_RT( "%s"), G_STRING_CHAR(귀환));
	//		break;
		case	n_OtherSelf:			// 분신
			Rsprintf(RWCHAR(Str) ,_RT( "%s"), G_STRING_CHAR(IDS_HELP_DUPLICATE));
			break;
		case	n_NotSkill:				// 스킬 불가 
			Rsprintf(RWCHAR(Str) ,_RT( "%s"), G_STRING_CHAR(IDS_HELP_SEAL_SKILL));
			break;
		case	n_ImpossibleCombo :		// 콤보 봉인 
			Rsprintf(RWCHAR(Str) ,_RT( "%s"), G_STRING_CHAR(IDS_HELP_SEAL_COMBO));
			break;
		case n_ReduceDamage:				//--데미지흡수 
			Rsprintf(RWCHAR(Str) ,_RT( "%s"), G_STRING_CHAR(IDS_HELP_DAMAGES_ABSORPTION));
			break;
/*

		case	n_Dispell_Immunity_Potion: 		//디스펠 면역
			Rsprintf(RWCHAR(Str) ,_RT( "%s"), G_STRING_CHAR(IDS_HELP_IMM_DISPEL));
			break;
		case	n_Paralysis_Immunity_Potion: 	//마비 면역
			Rsprintf(RWCHAR(Str) ,_RT( "%s"), G_STRING_CHAR(IDS_HELP_IMM_MAGIC));
			break;
		case	n_Move_Cancle_Potion: 			//자유(이동불가 해제)
			Rsprintf(RWCHAR(Str) ,_RT( "%s"), G_STRING_CHAR(IDS_HELP_COUNTER_ATTACK));
			break;
		case	n_Magic_Immunity_Potion: 	//마법 면역
			Rsprintf(RWCHAR(Str) ,_RT( "%s"), G_STRING_CHAR(IDS_HELP_IMM_MAGIC));
			break;
		case	n_Attack_Immunity_Potion: 	//물리 공격 면역
			Rsprintf(RWCHAR(Str) ,_RT( "%s"), G_STRING_CHAR(IDS_HELP_IMM_PA));
			break;
		case	n_Detect_Potion: 				//은신 감지
			Rsprintf(RWCHAR(Str) ,_RT( "%s"), G_STRING_CHAR(IDS_HELP_HIDDEN_DETECTION));
			break;
*/
		
	default: 
			Rstrcpy(RWCHAR(Str) , _RT(""));
			break;

	}

	return TRUE;
}

void CSkillLevelUpWnd::Open( SPcItem* pPcItem )
{
	m_pPcItem = pPcItem; 

	nRui->SendEvent( WID_SkillLevelUpWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
	
	DrawCurtLvSkillInfo();	
	DrawNextLvSkillInfo();	

	return;
}

//-----------------------------------------------------------------------------
void CSkillLevelUpWnd::Proc( SEventMessage* EMsg )
{
	switch( EMsg->FocusWnd )
	{
	case WID_SkillLevelUpWnd:
		{
			if( EMsg->Keys[DIK_RETURN].Action == n_baDown )
			{
				g_RockClient.Send_CsSkillUp( m_pPcItem->SkillTable->theSkillCode );
				nRui->SetEnterFlag(n_nfEditEnter);
				nRui->SendEvent( WID_SkillLevelUpWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
			}
		}
	case WID_SkillLevelUp_CloseBtn:
		{
			if( EMsg->Notify == NM_BTN_CLICK )
			{
				nRui->SendEvent( WID_SkillLevelUpWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
			}
		}
		break;
	case WID_SkillLevelUp_OKBtn:
		{
			if( EMsg->Notify == NM_BTN_CLICK )
			{
				g_RockClient.Send_CsSkillUp( m_pPcItem->SkillTable->theSkillCode );
					
				nRui->SendEvent( WID_SkillLevelUpWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
			}
		}
		break;
	case WID_SkillLevelUp_CancelBtn:
		{
			if( EMsg->Notify == NM_BTN_CLICK )
			{
				nRui->SendEvent( WID_SkillLevelUpWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
			}
		}
		break;
	}

	return;
}


//-----------------------------------------------------------------------------
int CSkillLevelUpWnd::GetNumLine( int aNumStr, int aCount )
{
	int NumChar = 0;
	int NumLine = 1;
	
	if( aNumStr == 0 )
	{
		return( 0 );
	}
	
	for( int i = 0; i < aNumStr; ++i )
	{
		NumChar++;
		
		if( NumChar == aCount )
		{
			NumChar = 0;
			NumLine++;
		}
	}
	
	return( NumLine );
}

//-----------------------------------------------------------------------------
