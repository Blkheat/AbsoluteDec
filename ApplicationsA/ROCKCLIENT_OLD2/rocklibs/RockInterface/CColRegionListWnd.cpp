#include "..\\..\\RockClient.h"
#include "Rui.h"
#include "CColRegionListWnd.h"
#include "CColRegionInfoWnd.h"

CColRegionListWnd g_ColRegionListWnd;

#define TEXT_COLOR_TITLE 255, 255, 200
#define TEXT_COLOR_STATIC 225, 225, 225
#define TEXT_COLOR_VALUE1 225, 225, 255
#define TEXT_COLOR_VALUE2 225, 255, 225

CColRegionListWnd::CColRegionListWnd()
{
	m_nStartListNum = 0;	
	m_nSelectedBtn = -1;		   
}

CColRegionListWnd::~CColRegionListWnd()
{

}

void CColRegionListWnd::Init()
{

}

void CColRegionListWnd::Composition()
{
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
	m_pColRListFrame = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_None, WID_None, 
									sCRLRect.x, sCRLRect.y, sCRLRect.w, sCRLRect.h, false );
	m_pColRListFrame->SetWndTexStyle( fwfTex_Style_Tile9 );
	m_pColRListFrame->SetClientImage( TID_CTRL_WND_L1 );
	m_pColRListFrame->SetWndTileSizeLR( 40, 29 );
	m_pColRListFrame->SetWndTileSizeTB( 31, 11 );

	m_pColRListFrame->SetCaption( "콜로니 지역안내" );	 //HAN_MSG
	m_pColRListFrame->SetTitle( true );
	m_pColRListFrame->SetWndProc( this );
	m_pColRListFrame->SetMove( false );
	m_pColRListFrame->SetVisible( false );
	m_pColRListFrame->Initialize();

	img9 = (CImageBox9*)nRui->CreateWnd( n_wtImageBox9, WID_None, m_pColRListFrame->GetID(), 
										 31, 40, 296, 226, false );       	
	img9->SetWndTileSizeLR( 9, 9 );					// 윈도우 생성시와 동일 
	img9->SetWndTileSizeTB( 8, 8 );					// 윈도우 생성시와 동일 
	img9->SetFocusCheck( false );					// 이미지 사용시 필수 
	img9->SetClientImage( TID_CTRL_CONTBACK_L1 );
	img9->SetAdjustPosition( 0, 0 );

	m_pScrollBar = (CScrollBar*)nRui->CreateWnd( n_wtScrollBar, WID_None, m_pColRListFrame->GetID(), 
												 313, 41, 13, 224, false );
	m_pScrollBar->SetClientImage( TID_CTRL_SCR_BG );
	m_pScrollBar->SetThumbImage( TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR_CLK, TID_None );
	m_pScrollBar->InitSlide( stVertSld, 0, 0, 0, 13 );//Type, min, max, value, 썸의 폭
	m_pScrollBar->SetScrollBtnA( TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT_CLK, TID_None );
	m_pScrollBar->SetScrollBtnB( TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT_CLK, TID_None );

	int i = 0;
	
	for( i = 0 ; i < 14 ; i++ )
	{
		m_pColRName[i].pColAreaData = &m_sCAreaData[i];
		m_pColRName[i].pColRName = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, 
									m_pColRListFrame->GetID(), 31, 42 + (i * 16), 281, 16, false );
		m_pColRName[i].pColRName->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
		m_pColRName[i].pColRName->SetClientImage( TID_None );
		m_pColRName[i].pColRName->SetAlignText( n_atCenter, n_atCenter );	
		m_pColRName[i].pColRName->SetTextColor( TEXT_COLOR_VALUE1 );
		m_pColRName[i].pColRName->SetText(_RT(""));
		m_pColRName[i].pColRName->SetHID( HID_None );
	}

	for( i = 0 ; i < 14 ; i++ )
	{
		m_pColRSelBtn[i] = 	(CButton*)nRui->CreateWnd( n_wtButton, WID_None, m_pColRListFrame->GetID(), 
			32, 42 + (i * 16), 281, 16, false );
		m_pColRSelBtn[i]->SetClientImage(TID_None);
		m_pColRSelBtn[i]->SetClientAlpah(128);
		m_pColRSelBtn[i]->SetBtnImage(TID_None, TID_None, TID_MFriendSelFocus, TID_None);
	}

	m_pCloseBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, m_pColRListFrame->GetID(), 
											 336, 6, 13, 13, false );   
	m_pCloseBtn->SetBtnImage( TID_CTRL_BTN_X, TID_CTRL_BTN_X_OVR, TID_CTRL_BTN_X_CLK, TID_None );
}

void CColRegionListWnd::Update()
{

}
void CColRegionListWnd::ClearColAreaDataList()
{
	int i = 0;

	for( i = 0 ; i < RP_MAX_LINKED_COLONY ; i++ )
	{
		m_sCAreaData[i].aColonyCode = 0;
		SAFE_DELETE(m_sCAreaData[i].pColAreaDetailInfo)
	}

	m_nLinkColonyCount = 0;
}

void CColRegionListWnd::Proc( SEventMessage* EMsg )
{

	for(int i = 0 ; i < 14 ; i++)
	{
		if(m_pColRSelBtn[i]->GetID() == EMsg->FocusWnd)
		{			
			if( EMsg->Notify == NM_BTN_CLICK )
			{
				if( m_nSelectedBtn != i )
				{
					FieldBntSelected( i );
				}
				else
				{
					if(m_pColRName[i].pColAreaData->aColonyCode)  ///-- 콜로니 코드가 존재할 경우
					{
						if(!m_pColRName[i].pColAreaData->pColAreaDetailInfo)
						{
							///-- 콜로니 코드에 관한 정확한 정보가 없을 경우
							///-- 콜로니에 대한 정확한 정보를 요청한다.
							g_RockClient.Send_CsReqColonyInfo(m_pColRName[i].pColAreaData->aColonyCode);
						}
						
						OpenColInfoWnd(m_pColRName[i].pColAreaData);						
					}				
				}
			}
		}
	}	
	
	if( EMsg->FocusWnd == m_pScrollBar->GetID() )
	{
		if( ( EMsg->Notify == NM_SLD_MOVEBAR ) || ( EMsg->Notify == NM_SLD_CLICKMOVEBAR ) 
				|| ( EMsg->Notify == NM_SLD_BTNMOVERBAR ) )
		{
			SetListScroll( (int)EMsg->fParam );				
		}
	}
	else if( EMsg->FocusWnd == m_pCloseBtn->GetID())
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			Close();
		}
	}	
}

void CColRegionListWnd::OpenColInfoWnd( SColAreaData *pColAreaData )
{
	nRui->SendEvent( m_pColRListFrame->GetID() , n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	g_ColRegionInfoWnd.SetLinkColAreaData( pColAreaData );
	g_ColRegionInfoWnd.Open();
	g_ColRegionInfoWnd.RefreshColRegionInfo();	
}

void CColRegionListWnd::Open()
{
	nRui->SendEvent( m_pColRListFrame->GetID() , n_emSetVisible, (EPARAM)true, 0, 0, 0 );
	m_nStartListNum = 0;	
	m_nSelectedBtn = -1;
}

void CColRegionListWnd::Close()
{
	ClearColAreaDataList();
	nRui->SendEvent( m_pColRListFrame->GetID() , n_emSetVisible, (EPARAM)false, 0, 0, 0 );
}

void CColRegionListWnd::LogOff()
{
	ClearColAreaDataList();
	nRui->SendEvent( m_pColRListFrame->GetID() , n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	m_nStartListNum = 0;	
	m_nSelectedBtn = -1;
}

void CColRegionListWnd::SetListScroll( int _linenum )
{
	FieldBntSelected(-1);
	m_nStartListNum = _linenum * 1;

	SetColAreaField();
}

void CColRegionListWnd::FieldBntSelected( int _num_btn )
{
	m_nSelectedBtn = _num_btn;
	
	for(int i = 0; i < 14 ; i++)
	{
		m_pColRSelBtn[i]->SetClientImage( TID_None );
	}

	if(m_nSelectedBtn != -1)
	{
		m_pColRSelBtn[m_nSelectedBtn]->SetClientImage( TID_MFriendSelFocus );
	}
}

void CColRegionListWnd::SetColAreaField()
{	
	FieldBntSelected(-1);
	
	TCHAR tempStr[512] = {0,};

	for( int i = 0; i < 14; ++i )
	{
		if( m_nStartListNum + i < 14)
		{
			m_pColRName[i].pColAreaData = &m_sCAreaData[m_nStartListNum + i];
		}
		else
		{
			m_pColRName[i].pColAreaData = NULL;
		}	

		if(m_pColRName[i].pColAreaData)
		{
			m_pColRName[i].pColRName->SetText(_RT(""));
		}
		else
		{
			m_pColRName[i].pColRName->SetText(_RT(""));
		}
	}
}