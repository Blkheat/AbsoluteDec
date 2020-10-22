#include "Rui.h"
#include "CColRegionInfoWnd.h"
#include "CColRegionListWnd.h"
#include "..\\..\\StringManager.h"
#include "..\\..\\MapDataManager.h"
#include "UiUtil.h"

CColRegionInfoWnd g_ColRegionInfoWnd;

#define TEXT_COLOR_TITLE 255, 255, 200
#define TEXT_COLOR_STATIC 225, 225, 225
#define TEXT_COLOR_VALUE1 225, 225, 255
#define TEXT_COLOR_VALUE2 225, 255, 225

CColRegionInfoWnd::CColRegionInfoWnd()
{
	m_pColRInfoFrame = NULL;
	m_pCloseBtn = NULL;
	m_pColRegionName = NULL;
	m_pColOccuFronName = NULL;
	m_pColFronMasterName = NULL;
	m_pColFronMemNumber = NULL;
	m_pColDeclWarCanDate = NULL;

	m_pPrevBtn = NULL;
	m_pDeclWar = NULL;
	
	m_pLinkColAreaData = NULL;
}

CColRegionInfoWnd::~CColRegionInfoWnd()
{

}

void CColRegionInfoWnd::Init()
{

}

void CColRegionInfoWnd::Composition()
{
	char temp[100] = {0,};
	
	int center_x	= nRui->Desktop.Width / 2;
	int center_y	= nRui->Desktop.Height / 2;

	SRect sCRLRect;
	CImageBox *	 img = NULL;
	CImageBox9*  img9 = NULL;
	CTextBox  *  tbox = NULL;

	sCRLRect.w	= 355;
	sCRLRect.h	= 290;
	sCRLRect.x	= center_x - ( sCRLRect.w / 2 );
	sCRLRect.y	= center_y - ( sCRLRect.h / 2 );
	
	///-- Frame Wnd
	m_pColRInfoFrame = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_None, WID_None, 
									sCRLRect.x, sCRLRect.y, sCRLRect.w, sCRLRect.h, false );
	m_pColRInfoFrame->SetWndTexStyle( fwfTex_Style_Tile9 );
	m_pColRInfoFrame->SetClientImage( TID_CTRL_WND_L1 );
	m_pColRInfoFrame->SetWndTileSizeLR( 40, 29 );
	m_pColRInfoFrame->SetWndTileSizeTB( 31, 11 );

	m_pColRInfoFrame->SetCaption( "콜로니 지역정보" );	 //HAN_MSG
	m_pColRInfoFrame->SetTitle( true );
	m_pColRInfoFrame->SetWndProc( this );
	m_pColRInfoFrame->SetMove( false );
	m_pColRInfoFrame->SetVisible( false );
	m_pColRInfoFrame->Initialize();

	img9 = (CImageBox9*)nRui->CreateWnd( n_wtImageBox9, WID_None, m_pColRInfoFrame->GetID(), 
										 11, 35, 331, 205, false );       	
	img9->SetWndTileSizeLR( 9, 9 );					// 윈도우 생성시와 동일 
	img9->SetWndTileSizeTB( 8, 8 );					// 윈도우 생성시와 동일 
	img9->SetFocusCheck( false );					// 이미지 사용시 필수 
	img9->SetClientImage( TID_CTRL_CONTBACK_L1 );
	img9->SetAdjustPosition( 0, 0 );
	
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pColRInfoFrame->GetID(), 
									   131, 35, 1, 205, false );   
	img->SetClientImage( TID_CLIENT );
	img->SetClientColor( 200, 200, 225 );
	img->SetScaleTrans( true );

	int i = 0;
	int yPos = 67;
	
	for( i = 0; i < 4; ++i )
	{
		img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pColRInfoFrame->GetID(), 
										11, yPos, 331, 1, false );   
		img->SetClientImage( TID_CLIENT );
		img->SetClientColor( 200, 200, 225 );
		img->SetScaleTrans( true );		
		yPos += 32;
	}

	///-- Static
	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pColRInfoFrame->GetID(), 20, 46, 110, 13, false );
	tbox->SetFontR( n_fRsvFontWndStatic );
	tbox->SetTextColor( TEXT_COLOR_STATIC );	
	tbox->SetAlignText( n_atLeft, n_atCenter );	
	sprintf( temp, "%s", "지역정보" );				// HAN_MSG  
	tbox->SetText( temp );

	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pColRInfoFrame->GetID(), 20, 46 + 32, 110, 13, false );
	tbox->SetFontR( n_fRsvFontWndStatic );
	tbox->SetTextColor( TEXT_COLOR_STATIC );	
	tbox->SetAlignText( n_atLeft, n_atCenter );	
	sprintf( temp, "%s", "점령 프론티어" );				// HAN_MSG  
	tbox->SetText( temp );

	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pColRInfoFrame->GetID(), 20, 46 + (32 * 2), 110, 13, false );
	tbox->SetFontR( n_fRsvFontWndStatic );
	tbox->SetTextColor( TEXT_COLOR_STATIC );	
	tbox->SetAlignText( n_atLeft, n_atCenter );	
	sprintf( temp, "%s", "프론티어 마스터" );				// HAN_MSG  
	tbox->SetText( temp );

	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pColRInfoFrame->GetID(), 20, 46 + (32 * 3), 110, 13, false );
	tbox->SetFontR( n_fRsvFontWndStatic );
	tbox->SetTextColor( TEXT_COLOR_STATIC );	
	tbox->SetAlignText( n_atLeft, n_atCenter );	
	sprintf( temp, "%s", "전투인원" );				// HAN_MSG  
	tbox->SetText( temp );

	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pColRInfoFrame->GetID(), 20, 46 + (32 * 4), 110, 40, false );
	tbox->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	tbox->SetClientImage( TID_None );
	tbox->SetTextColor( TEXT_COLOR_VALUE1 );	
	tbox->SetAlignText( n_atLeft, n_atCenter );
	tbox->SetMultiLine( true );	
	tbox->SetLineInterval( 2 );
	sprintf( temp, "%s", "선전포고##가능시간" );				// HAN_MSG  
	tbox->SetText( temp );

	m_pColRegionName = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pColRInfoFrame->GetID(), 132, 46 , 216, 13, false );
	m_pColRegionName->SetFontR( n_fRsvFontWndStatic );
	m_pColRegionName->SetTextColor( TEXT_COLOR_STATIC );	
	m_pColRegionName->SetAlignText( n_atCenter, n_atCenter );	
	sprintf( temp, "%s", "일리시안 콜로니" );				// HAN_MSG  
	m_pColRegionName->SetText( temp );

	m_pColOccuFronName = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pColRInfoFrame->GetID(), 132, 46 + 32 , 216, 13, false );
	m_pColOccuFronName->SetFontR( n_fRsvFontWndStatic );
	m_pColOccuFronName->SetTextColor( TEXT_COLOR_STATIC );	
	m_pColOccuFronName->SetAlignText( n_atCenter, n_atCenter );	
	sprintf( temp, "%s", "미소 & 오리온 프론티어" );				// HAN_MSG  
	m_pColOccuFronName->SetText( temp );

	m_pColFronMasterName = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pColRInfoFrame->GetID(), 132, 46 + (32 * 2) , 216, 13, false );
	m_pColFronMasterName->SetFontR( n_fRsvFontWndStatic );
	m_pColFronMasterName->SetTextColor( TEXT_COLOR_STATIC );	
	m_pColFronMasterName->SetAlignText( n_atCenter, n_atCenter );	
	sprintf( temp, "%s", "별이햇님" );				// HAN_MSG  
	m_pColFronMasterName->SetText( temp );

	m_pColFronMemNumber = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pColRInfoFrame->GetID(), 132, 46 + (32 * 3) , 216, 13, false );
	m_pColFronMemNumber->SetFontR( n_fRsvFontWndStatic );
	m_pColFronMemNumber->SetTextColor( TEXT_COLOR_STATIC );	
	m_pColFronMemNumber->SetAlignText( n_atCenter, n_atCenter );	
	sprintf( temp, "%s", "60명" );				// HAN_MSG  
	m_pColFronMemNumber->SetText( temp );

	m_pColDeclWarCanDate = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pColRInfoFrame->GetID(), 132, 46 + (32 * 4) , 216, 13, false );
	m_pColDeclWarCanDate->SetFontR( n_fRsvFontWndStatic );
	m_pColDeclWarCanDate->SetTextColor( TEXT_COLOR_STATIC );	
	m_pColDeclWarCanDate->SetAlignText( n_atCenter, n_atCenter );	
	sprintf( temp, "%s", "04월03일 오후 10시" );				// HAN_MSG  
	m_pColDeclWarCanDate->SetText( temp );

	m_pColAttckFName = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pColRInfoFrame->GetID(), 132, 46 + (32 * 4) + 20 , 216, 50, false );
	m_pColAttckFName->SetFontR( n_fRsvFontWndStatic );
	m_pColAttckFName->SetTextColor( 255, 100, 100 );	
	m_pColAttckFName->SetAlignText( n_atCenter, n_atTop ); 
    m_pColAttckFName->SetMultiLine( true );
    m_pColAttckFName->SetLineInterval( 5 );
    m_pColAttckFName->SetMultiLineAsign(true);   
	sprintf( temp, "%s", "시공찬가 프론티어##선전포고완료" );				// HAN_MSG  
	m_pColAttckFName->SetText( temp );

	m_pPrevBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None , m_pColRInfoFrame->GetID() , 73, 251, 74, 25, false );        
	m_pPrevBtn->SetBtnImage( TID_CTRL_BTN58 , TID_CTRL_BTN58_OVR , TID_CTRL_BTN58_CLK , TID_CTRL_BTN58_DIS );
	m_pPrevBtn->SetFontR( n_fRsvFontWndButton );   
	m_pPrevBtn->SetText( _RT("이전 단계") );				// HAN_MSG  
	

	m_pDeclWar = (CButton*)nRui->CreateWnd( n_wtButton, WID_None , m_pColRInfoFrame->GetID() , 216, 251, 74, 25, false );        
	m_pDeclWar->SetBtnImage( TID_CTRL_BTN58 , TID_CTRL_BTN58_OVR , TID_CTRL_BTN58_CLK , TID_CTRL_BTN58_DIS );
	m_pDeclWar->SetFontR( n_fRsvFontWndButton );   
	m_pDeclWar->SetText( _RT("선전 포고") );				// HAN_MSG 

	m_pCloseBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, m_pColRInfoFrame->GetID(), 
											 336, 6, 13, 13, false );   
	m_pCloseBtn->SetBtnImage( TID_CTRL_BTN_X, TID_CTRL_BTN_X_OVR, TID_CTRL_BTN_X_CLK, TID_None );
}


void CColRegionInfoWnd::Update()
{

}

void CColRegionInfoWnd::Proc( SEventMessage* EMsg )
{
	if( EMsg->FocusWnd == m_pCloseBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			Close();
			g_ColRegionListWnd.ClearColAreaDataList();
		}
	}
	else if( EMsg->FocusWnd == m_pPrevBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			OpenColListWnd();			
		}	
	}
	else if( EMsg->FocusWnd == m_pDeclWar->GetID() && !m_IsButton_lock )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			///-- 이곳에서 선전포고 공포를 서버측으로 보내준다.
			///-- 두번 클릭하는 경우 - 패킷이 두번가는경우를 체크
			WORD aColonyCode = 0;
			if( m_pLinkColAreaData && m_pLinkColAreaData->pColAreaDetailInfo )
			{
				aColonyCode = m_pLinkColAreaData->pColAreaDetailInfo->aColonyCode;
			}			
			
			if(aColonyCode)
			{
				g_RockClient.Send_CsDeclareWar(aColonyCode);
			}
			
			m_IsButton_lock = TRUE;
		}	
	}
}

void CColRegionInfoWnd::OpenColListWnd()
{
	Close();
	g_ColRegionListWnd.Open();
}

void CColRegionInfoWnd::Open()
{
	nRui->SendEvent( m_pColRInfoFrame->GetID() , n_emSetVisible, (EPARAM)true, 0, 0, 0 );
	m_IsButton_lock = FALSE;
}

void CColRegionInfoWnd::Close()
{
	m_pLinkColAreaData = NULL;
	nRui->SendEvent( m_pColRInfoFrame->GetID() , n_emSetVisible, (EPARAM)false, 0, 0, 0 );
}

void CColRegionInfoWnd::LogOff()
{
	m_pLinkColAreaData = NULL;
	nRui->SendEvent( m_pColRInfoFrame->GetID() , n_emSetVisible, (EPARAM)false, 0, 0, 0 );	
}

void CColRegionInfoWnd::SetLinkColAreaData(SColAreaData * pLinkColAreaData)
{
	m_pLinkColAreaData = pLinkColAreaData;
}

void CColRegionInfoWnd::RefreshColRegionInfo()
{
	m_pColRegionName->SetText(_RT("") );
	m_pColOccuFronName->SetText(_RT("") );
	m_pColFronMasterName->SetText(_RT("") );
	m_pColFronMemNumber->SetText(_RT("") );
	m_pColDeclWarCanDate->SetText(_RT("") );
	m_pColAttckFName->SetText(_RT("") );
	m_pDeclWar->SetBtnHold( true );
	
	char strTemp[128] = {0,};
	
	if(m_pLinkColAreaData)
	{
		if(m_pLinkColAreaData->pColAreaDetailInfo)
		{
			m_pColRegionName->SetText( G_STRING_CHAR( G_MAP_STRID(m_pLinkColAreaData->pColAreaDetailInfo->aColonyCode) ) );

			if( strcmp(m_pLinkColAreaData->pColAreaDetailInfo->strFrontierName,"") != 0 )
			{
				Rsprintf(RWCHAR(strTemp), _RT("%s "), m_pLinkColAreaData->pColAreaDetailInfo->strFrontierName, _RT("프론티어"));	///--HAN_MSG
			}
			else
			{
				Rsprintf(RWCHAR(strTemp),_RT(""));
			}

			m_pColOccuFronName->SetText(strTemp);
			m_pColFronMasterName->SetText(m_pLinkColAreaData->pColAreaDetailInfo->strMasterPCName);

			Rsprintf(RWCHAR(strTemp), _RT("%u 명"), m_pLinkColAreaData->pColAreaDetailInfo->aMemberNum);		///HAN_MSG
			m_pColFronMemNumber->SetText( strTemp );

			::ConvertTimeToString(m_pLinkColAreaData->pColAreaDetailInfo->aWarTime, strTemp);
			m_pColDeclWarCanDate->SetText( strTemp );

			if( strcmp(m_pLinkColAreaData->pColAreaDetailInfo->strFrontierName,"") != 0 )
			{
				Rsprintf(RWCHAR(strTemp), _RT("%s %s"), m_pLinkColAreaData->pColAreaDetailInfo->strAttackFName , "프론티어##선전포고완료");	///-- HAN_MSG
				m_pColAttckFName->SetText( strTemp );
				m_pDeclWar->SetBtnHold( false );
			}
			else
			{
				m_pDeclWar->SetBtnHold( true );
			}		
		}
	}
}


