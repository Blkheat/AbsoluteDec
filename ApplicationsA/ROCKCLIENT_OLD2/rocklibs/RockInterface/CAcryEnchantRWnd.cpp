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

#include "..\\..\\global.h"
#include "..\\..\\RockClient.h"

#include "StringManager.h"
#include "CAcryEnchantRWnd.h"
#include "..\\..\\StringManager.h"
#include "SkillLevelUpWnd.h"
CAcryEnchantRWnd	* g_pAcryEnchantRWnd = NULL;

CAcryEnchantRWnd::CAcryEnchantRWnd()
{
	m_pARFrameWnd = NULL;

	m_pAccessarySlot = NULL;
	memset( &m_AccessaryItem, 0, sizeof(SPcItem) );	
	
	m_pResultTxt = NULL;
	m_pAddAttriTxt = NULL;		

	m_pCloseBtn = NULL;			
	m_pConfirmBtn = NULL;

	m_pIConX = NULL;
	
	m_IsSuccess = FALSE;
}

CAcryEnchantRWnd::~CAcryEnchantRWnd()
{


}

void CAcryEnchantRWnd::RePosition( int nXPos, int nYPos )
{
	m_pARFrameWnd->Reposition( nXPos, nYPos );
}

void CAcryEnchantRWnd::Open(BOOL IsSuccess)
{
	m_IsSuccess = IsSuccess;

	if( m_IsSuccess )
	{
		m_pResultTxt->SetTextColor( 50, 50, 255 );
		m_pResultTxt->SetText( G_STRING_CHAR(IDS_SUCCESS_ADDACCESARY) );	

		m_pIConX->SetVisible( false );
		m_pAddAttriTxt->SetText( G_STRING_CHAR(IDS_NONE) );
	}
	else
	{
		m_pResultTxt->SetTextColor( 255, 50, 50 );
		m_pResultTxt->SetText( G_STRING_CHAR(IDS_FAIL_ADDACCESARY) );	

		m_pIConX->SetVisible( true );
		m_pAddAttriTxt->SetText( G_STRING_CHAR(IDS_NONE) );
	}

	nRui->SendEvent( m_pARFrameWnd->GetID(), n_emSetVisible, (EPARAM)true, 0, 0, 0 );
}

void CAcryEnchantRWnd::Close()
{
	nRui->SendEvent( m_pARFrameWnd->GetID(), n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	m_IsSuccess = FALSE;
}

void CAcryEnchantRWnd::SetPcItem( SPcItem * pPcItem )
{
	static char strString[256] = "";	
	
	if( pPcItem )
	{
		memcpy( &m_AccessaryItem, pPcItem, sizeof(SPcItem) );

		///-- 이곳에서 결과가 성공시에 주문서 가공 결과를 출력해준다.
		if( IsVisible && m_IsSuccess )
		{
			///--	m_pAddAttriTxt
			SItemBaseInfo* item_Info = NULL;
	
			WORD aScrollCode = 0;
			aScrollCode =  m_AccessaryItem.WearTable.theEldaStone;

			BYTE aSkillLevel = 0;

			if( aScrollCode )
			{
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
							aSkillLevel =  g_SkillLevelUpWnd.GetActiveSKillLevel(skill_Info);
															
							if( skill_Info->theAttackAttri == 12 )
							{
								Rsprintf( RWCHAR(strString), G_STRING_CHAR(IDS_RES_OF_PARALYSIS_PER), aSkillLevel );		
							}
							else if( skill_Info->theAttackAttri == 13 )
							{
								Rsprintf( RWCHAR(strString), G_STRING_CHAR(IDS_RES_OF_DISPEL_PER), aSkillLevel );		///-- HAN_MSG
							}	
							
							m_pAddAttriTxt->SetText( strString );
						}
					}
				}
			}
		}
	}
}

void CAcryEnchantRWnd::Init()
{


}

void CAcryEnchantRWnd::Composition()
{
	CButton*		btn = NULL;
	CTextBox*		tbox = NULL;
	CImageBox*		img = NULL;
	CImageBox9*		black_img9 = NULL;
	
	int center_x	= nRui->Desktop.Width / 2;
	int center_y	= nRui->Desktop.Height / 2;	
	
	SRect enchantwnd;
	enchantwnd.w	= 204;
	enchantwnd.h	= 220;
	enchantwnd.x	= center_x - ( enchantwnd.w / 2 );
	enchantwnd.y	= 349;	

	m_pARFrameWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_None, WID_None, 
		enchantwnd.x, enchantwnd.y, enchantwnd.w, enchantwnd.h, false );
	m_pARFrameWnd->SetWndTexStyle( fwfTex_Style_Tile9 );
	m_pARFrameWnd->SetClientImage( TID_CTRL_WND_L1 );
	m_pARFrameWnd->SetIconTID( TID_CTRL_ICON_INVEN );	
	m_pARFrameWnd->SetWndTileSizeLR( 40, 29 );
	m_pARFrameWnd->SetWndTileSizeTB( 31, 11 );

	m_pARFrameWnd->SetCaption( G_STRING_CHAR(IDS_WND_ENCHANT_RESULT) );			
	m_pARFrameWnd->SetTitle( true );	
	m_pARFrameWnd->SetWndProc( this );
	m_pARFrameWnd->SetVisible( false );
	m_pARFrameWnd->Initialize();

	m_pCloseBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, m_pARFrameWnd->GetID(), 
									 185, 6, 13, 13, false );	
	m_pCloseBtn->SetBtnImage( TID_CTRL_BTN_X, TID_CTRL_BTN_X_OVR, TID_CTRL_BTN_X_CLK, TID_None );

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pARFrameWnd->GetID(), 1, 31, 202, 65, false );
	img->SetClientImage( TID_CTRL_BACK );
	img->SetFocusCheck( false );
	
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pARFrameWnd->GetID(), 1, 30, 202, 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );
	
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pARFrameWnd->GetID(), 1, 96, 202, 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );

	m_pResultTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pARFrameWnd->GetID(), 
											   1, 45, 202, 17, false );
	m_pResultTxt->SetFontR( n_fRsvFontWndStatic );	
	m_pResultTxt->SetAlignText( n_atCenter, n_atTop );
	m_pResultTxt->SetTextColor( 255, 50, 50 ); 
	m_pResultTxt->SetMultiLine( true );
	m_pResultTxt->SetLineInterval( 5 );
	m_pResultTxt->SetMultiLineAsign(true);   
	m_pResultTxt->SetText( G_STRING_CHAR(IDS_FAIL_ADDACCESARY) );	

	///-- Slot
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pARFrameWnd->GetID(), 84, 106, 35, 35, false );         
	img ->SetClientImage( TID_CTRL_SLOT_SUNKEN );	
	
	m_pAccessarySlot = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, m_pARFrameWnd->GetID(), 
		84, 106, 35, 35, false );	
	m_pAccessarySlot->PcItem = &m_AccessaryItem;	
	m_pAccessarySlot->SetSlotGroup( n_SGROUP_TUNING );
	m_pAccessarySlot->SetHID( HID_ITEMTIP );

	m_pIConX = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pARFrameWnd->GetID(), 
				88, 110, 27, 27, false );
	m_pIConX->SetClientImage( TID_C_WND_ELDA_SB_X );
	m_pIConX->SetVisible( true );
	
	///-- 부가속성( Static Text )
	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pARFrameWnd->GetID(), 0, 146, 202, 17, false );
	tbox->SetFontR( n_fRsvFontWndStatic );	
	tbox->SetText( G_STRING_CHAR(IDS_ADD_ATTRI) );	

	m_pAddAttriTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pARFrameWnd->GetID(), 
												 0, 161, 202, 17, false );
	m_pAddAttriTxt->SetFontR( n_fRsvFontWndStatic );	
	m_pAddAttriTxt->SetText( G_STRING_CHAR(IDS_NONE) );	

	m_pConfirmBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, m_pARFrameWnd->GetID(), 
											   74, 186, 58, 25, false );
	m_pConfirmBtn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_CTRL_BTN58_DIS );
	m_pConfirmBtn->SetFontR( n_fRsvFontWndButton );   
	m_pConfirmBtn->SetText( G_STRING_CHAR( IDS_WND_COM_OK ) );
}

void CAcryEnchantRWnd::Update()
{


}

void CAcryEnchantRWnd::Proc( SEventMessage* EMsg )
{
	if( EMsg->FocusWnd == m_pCloseBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			Close();
		}
	}

	if( EMsg->FocusWnd == m_pConfirmBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			Close();
		}
	}
}
