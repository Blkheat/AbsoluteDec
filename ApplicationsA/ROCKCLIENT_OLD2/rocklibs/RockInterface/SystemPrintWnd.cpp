#include <wchar.h>
#include "Rui.h"

#include "..\\..\\brother.h"
#include "..\\RockPCH.h"

#include "Define.h"
#include "Util.h"
#include "Render.h"
#include "Fontman.h"
#include "Resource.h"
#include "Chain.h"
#include "Container.h"

#include "..\\..\\GTimer.h"
#include "..\\..\\global.h"
#include "..\\..\\RockClient.h"
#include "..\\..\\bravolibs\\obj\\player.h"

#include "UiUtil.h"
#include "EditBox.h"
#include "SystemPrintWnd.h"

CSystemPrintWnd g_SystemPrintWnd;



CSystemPrintWnd::CSystemPrintWnd()
{
	m_pSystemBgWnd = NULL;
	m_pSystemOutList = NULL;
	m_pSystemScrollBarBg = NULL;	
	m_pSystemScrollBar = NULL;

	m_bFocusSysWnd = FALSE;
	m_bViewScroll = FALSE;	
	m_fCumTime = 0.0f;

	Init();
}

CSystemPrintWnd::~CSystemPrintWnd()
{

}

void CSystemPrintWnd::Init()
{
	m_nInsertIndex = 0;
	memset( m_SystemTxtData, 0, sizeof(SSystemTxtData) * (MAX_SYSTEMDATA_NUM + 1) );
	
	Rstrcpy(RWCHAR(m_strTempBuf1), _RT(""));
	Rstrcpy(RWCHAR(m_strTempBuf2), _RT(""));

	m_bEnableWeel = false;
}

void CSystemPrintWnd::LogOff()
{
	nRui->SendEvent( WID_SystemPrintFrame, n_emSetVisible, (EPARAM)false, 0, 0, 0 );

	Init();
}


void CSystemPrintWnd::Composition()
{	
	//693 
	// 1024 -1 - 354 = 669
	// 24 
	int FrameWidthEx = 0; 
#ifdef APPLY_GUIDE
	FrameWidthEx = 36;
#else 
	FrameWidthEx = 0;
#endif

#ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/24)
	
	m_nLeftDownPos = SPoint2( (nRui->Desktop.Width - (1024 - 693)) - FrameWidthEx  , nRui->Desktop.Height - 59 );	
	
#else
	
	m_nLeftDownPos = SPoint2( 693 - FrameWidthEx  , nRui->Desktop.Height - 59 );	
	
#endif// #ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/24)

	m_pSystemBgWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_SystemPrintFrame, WID_None, 
	m_nLeftDownPos.x, m_nLeftDownPos.y , 330 + FrameWidthEx  , 0, false );
	m_pSystemBgWnd->SetClientImage( TID_None );
	m_pSystemBgWnd->SetWndProc( this );
	m_pSystemBgWnd->SetTitle( false );
	m_pSystemBgWnd->SetVisible( false );
	m_pSystemBgWnd->SetZorder( n_zoBottom );
	m_pSystemBgWnd->SetMove(false);	
	m_pSystemBgWnd->Initialize();

	m_pSystemOutList = (CSystemOutList*)nRui->CreateWnd( n_wtSystemOutList, WID_None,
						WID_SystemPrintFrame, 0, 0, 313 + FrameWidthEx , 0, false );	
	m_pSystemOutList->SetOutputLevel(ChatWndSizeMin);	
	m_pSystemOutList->SetRefpViewSystemTxtData(m_SystemTxtData);
	m_pSystemOutList->SetFocusCheck(false);
	m_pSystemOutList->InitSystemOutList();

	m_pSystemScrollBarBg = (CWnd*)nRui->CreateWnd( n_wtWnd, WID_None, WID_SystemPrintFrame, 
		314  + FrameWidthEx , 0, 17, 0, false );	
	m_pSystemScrollBarBg->SetClientImage(TID_CLIENT);
	m_pSystemScrollBarBg->SetClientColor(50, 50, 50);
	m_pSystemScrollBarBg->SetClientAlpah(200);
	m_pSystemScrollBarBg->SetVisible(false);
	
	m_pSystemScrollBar =  (CScrollBar*)nRui->CreateWnd( n_wtScrollBar, WID_None, WID_SystemPrintFrame, 
						316 + FrameWidthEx, 0, 13, 0, false );
	m_pSystemScrollBar->SetClientImage( TID_None );
	m_pSystemScrollBar->SetThumbImage( TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR, 
					  TID_CTRL_SCR_BAR, TID_None );
	m_pSystemScrollBar->InitSlide( stVertSld, 0, (MAX_CHATDATA_NUM - 0), 0, 13 );//Type, min, max, value, 썸의 폭
	m_pSystemScrollBar->SetScrollBtnA( TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT, 
					  TID_CTRL_SCR2_UBT_CLK, TID_None );
	m_pSystemScrollBar->SetScrollBtnB( TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT, 
					  TID_CTRL_SCR2_DBT_CLK, TID_None );
	m_pSystemScrollBar->SetVisible(false);
	
	m_pSystemOutList->SetLinkScroll(m_pSystemScrollBar, m_pSystemScrollBarBg);

	ResetSizeFrame();	
}

void CSystemPrintWnd::ResetSizeFrame()
{
	SRect SystemBgWndRect;	
	SRect OutListRect;
	
	SRect ScrollBarRect;
	SRect ScrollBarBgRect;

	IPOS2 ScrollBarRPos;
	IPOS2 ScrollBarBgRPos;
	IPOS2 OutListRPos;

	m_pSystemBgWnd->GetClientSize(&SystemBgWndRect);	
	
	m_pSystemScrollBar->GetWndPosition(ScrollBarRPos);	
	m_pSystemScrollBarBg->GetWndPosition(ScrollBarBgRPos);

	m_pSystemOutList->GetClientSize(&OutListRect);
	m_pSystemOutList->GetWndPosition(OutListRPos);	

	SystemBgWndRect.y = m_nLeftDownPos.y - OutListRect.h;
	SystemBgWndRect.h = OutListRect.h;	

	m_pSystemBgWnd->ResetSize(&SystemBgWndRect);
	
	m_pSystemOutList->SetWndRepositionR(OutListRPos[0], 0);
	
	m_pSystemScrollBarBg->SetWndRepositionR(ScrollBarBgRPos[0], 0);
	m_pSystemScrollBarBg->GetClientSize(&ScrollBarBgRect);
	ScrollBarBgRect.h = OutListRect.h;
	m_pSystemScrollBarBg->ResetSize(&ScrollBarBgRect);	
	
	m_pSystemScrollBar->GetClientSize(&ScrollBarRect);	
	ScrollBarRect.h = OutListRect.h;
	m_pSystemScrollBar->ResetSize(&ScrollBarRect);	
	m_pSystemScrollBar->SetWndRepositionR(ScrollBarRPos[0], 0);

	ResetScroll();
	
}

void CSystemPrintWnd::ResetScroll()
{
	int nLastValue = ( MAX_SYSTEMDATA_NUM - m_pSystemOutList->GetOutPutLine() );
	m_pSystemScrollBar->ReSetScroll(0, nLastValue, nLastValue);
	m_pSystemOutList->SetDelValue(0);
}

void CSystemPrintWnd::Update()
{
	
	POINT	ptCursor;
	GetCursorPos(&ptCursor);
	::ScreenToClient(g_RockClient.GetApp(), &ptCursor);

	SRect SystemBgWndRect;
	m_pSystemBgWnd->GetClientSize(&SystemBgWndRect);

	m_bFocusSysWnd = FALSE;
	
	if( ( ptCursor.x > SystemBgWndRect.x ) && ( ptCursor.y > SystemBgWndRect.y ) && 
		( ptCursor.x < SystemBgWndRect.x + SystemBgWndRect.w ) && 
		( ptCursor.y < SystemBgWndRect.y + SystemBgWndRect.h ) )
	{
       m_bFocusSysWnd = TRUE;

	   BOOL IsMustViewScroll = FALSE;
	   
	   if( ( m_pSystemOutList->GetOutPutLine() - 1) >= m_pSystemOutList->GetLineCount() )
	   {
		  IsMustViewScroll = TRUE;
	   }
	
	   if( !m_bViewScroll && IsMustViewScroll )
	   {
		  m_fCumTime += g_GTimer.GetFrameElapsedSecTime();
		  float fRatio = m_fCumTime/SSystemTxtData::FadingTime;
			  
		  if( m_fCumTime >= SSystemTxtData::FadingTime)
		  {
			 m_bViewScroll = TRUE;
			 fRatio = 1.0f;
		  }					
			
		  UINT uAlpha = (int)(fRatio * 255.0f);
		  m_pSystemScrollBarBg->SetClientAlpah(uAlpha);
		  m_pSystemScrollBar->SetClientAlpah(uAlpha);		  
	   }
	   else
	   {
		  m_fCumTime = 0.0f;
		  m_pSystemScrollBarBg->SetClientAlpah(255);
		  m_pSystemScrollBar->SetClientAlpah(255);
	   }

	   m_pSystemScrollBarBg->SetVisible(true);
	   m_pSystemScrollBar->SetVisible(true);

	   m_pSystemOutList->SetAutoScroll(FALSE);
	}
	else
	{
	   m_bFocusSysWnd = FALSE;
	   m_bViewScroll = FALSE;	
	   m_fCumTime = 0.0f;

	   m_pSystemScrollBarBg->SetVisible(false);
	   m_pSystemScrollBar->SetVisible(false);

	   m_pSystemOutList->SetAutoScroll(TRUE);
	}
	
	if( nRui->GetEMsg()->Mouse.Action == MA_LUP)
	{	
		if( nRui->GetFocusWnd() != m_pSystemScrollBar->GetID() )
		{
			m_bEnableWeel = false;		
		}
	}
	
	UpdateSystemTxtData();
}

void CSystemPrintWnd::UpdateSystemTxtData()
{
	for(int i = 0 ; i < m_nInsertIndex ; i++)
	{
		switch(m_SystemTxtData[i].ViewState)
		{
			case NTxtView:
				{
					m_SystemTxtData[i].CumTime += g_GTimer.GetFrameElapsedSecTime();
					if(m_SystemTxtData[i].CumTime >= SSystemTxtData::ViewTime)
					{
						m_SystemTxtData[i].CumTime = 0.0f;
						m_SystemTxtData[i].ViewState = NTxtFadeout;
					}
				}
				break;
			case NTxtFadeout:
				{
					m_SystemTxtData[i].CumTime += g_GTimer.GetFrameElapsedSecTime();
					float fRatio = m_SystemTxtData[i].CumTime/SSystemTxtData::FadingTime;
					if(m_SystemTxtData[i].CumTime >= SSystemTxtData::FadingTime)
					{
						fRatio = 1.0f;
						m_SystemTxtData[i].CumTime = 0.0f;
						m_SystemTxtData[i].ViewState = NTxtNone;
					}					
					UINT uAlpha = (int)(255.0f - fRatio * 255.0f);
					m_SystemTxtData[i].Rcolor[A] = uAlpha;		
				}
				break;
		}
	}		
}

bool CSystemPrintWnd::SetWheelValue(SMouse & theMouse)
{
	if(!m_pSystemScrollBar->IsVisible() || !m_bEnableWeel)
	{
		return false;
	}	
	
	if( theMouse.State == n_msWheel && theMouse.WheelValue != 0 )
	{
		POINT curtMousePos;
		GetCursorPos( &curtMousePos );
		ScreenToClient( g_RockClient.GetApp(), &curtMousePos );	
		
		if( IsVisible && 
		  GetFrameWnd()->FocusCheck( curtMousePos.x, curtMousePos.y, false ) )
		{
			SetWheelValue( theMouse.WheelValue );
			return true;
		}	
	}
	
	return false;
}

void CSystemPrintWnd::SetWheelValue( short _wheel )
{
	if(m_pSystemScrollBar->IsVisible())
	{
		int value = 0;
		int nMax = 0;

		m_pSystemScrollBar->GetValue( &value );
		nMax = m_pSystemScrollBar->GetSldMax();
		
		if( ( _wheel < 0 ) && ( value < nMax ) )
		{
			value += 1;
		}
		else if( ( _wheel > 0 ) && ( value > 0 ) )
		{
			value -= 1;
		}
		
		m_pSystemScrollBar->SetValue( value );
		SetListScroll( value );
	}
}

void CSystemPrintWnd::Proc( SEventMessage* EMsg )
{
	if( EMsg->FocusWnd == m_pSystemScrollBar->GetID()  )
	{
		if( ( EMsg->Notify == NM_SLD_MOVEBAR ) || ( EMsg->Notify == NM_SLD_CLICKMOVEBAR ) 
			|| ( EMsg->Notify == NM_SLD_BTNMOVERBAR ) )
		{
			m_bEnableWeel = true;
			SetListScroll( (int)EMsg->fParam );	
			
			///-- 이쪽에서 사라진 이전의 시스템 메시지들 다시 다 보여주게한다.
			for(int i = 0 ; i < m_nInsertIndex ; i++)
			{
				m_SystemTxtData[i].ViewState = NTxtView;
				m_SystemTxtData[i].Rcolor[A] = 255;
				m_SystemTxtData[i].CumTime = 0.0f;
			}
		}	
	}
}

void CSystemPrintWnd::SetListScroll( int _linenum )
{
	int nDelValue = m_pSystemScrollBar->GetSldMax() - _linenum;
	m_pSystemOutList->SetDelValue(nDelValue);
}

void CSystemPrintWnd::InsertSystemMsg(void *pstrMsg, UINT uSysColor)
{
	RTCHAR* strMsg = (RTCHAR*)pstrMsg;

	switch(uSysColor)
	{
		case n_DarkRed:
			{
				m_TempSystemData.Rcolor[A] = 255;
				m_TempSystemData.Rcolor[R] = 255;
				m_TempSystemData.Rcolor[G] = 0;
				m_TempSystemData.Rcolor[B] = 0;
			}
			break;
		case n_DarkGreen:
			{
				m_TempSystemData.Rcolor[A] = 255;
				m_TempSystemData.Rcolor[R] = 0;
				m_TempSystemData.Rcolor[G] = 215;
				m_TempSystemData.Rcolor[B] = 5;
			}
			break;
		case n_LightGreen:
			{
				m_TempSystemData.Rcolor[A] = 255;
				m_TempSystemData.Rcolor[R] = 128;
				m_TempSystemData.Rcolor[G] = 255;				
				m_TempSystemData.Rcolor[B] = 128;
			}
			break;
		case n_LightBlue:
			{
				m_TempSystemData.Rcolor[A] = 255;
				m_TempSystemData.Rcolor[R] = 128;
				m_TempSystemData.Rcolor[G] = 128;
				m_TempSystemData.Rcolor[B] = 255;
			}
			break;
		case n_LightYellow:
			{
				m_TempSystemData.Rcolor[A] = 255;
				m_TempSystemData.Rcolor[R] = 255;
				m_TempSystemData.Rcolor[G] = 255;
				m_TempSystemData.Rcolor[B] = 128;
			}
			break;
		case n_LightOrange:
			{
				m_TempSystemData.Rcolor[A] = 255;
				m_TempSystemData.Rcolor[R] = 255;
				m_TempSystemData.Rcolor[G] = 150;
				m_TempSystemData.Rcolor[B] = 100;
			}
			break;		
	}

	SAFE_STR_CPY(m_strTempBuf1 , strMsg , MAX_SIZE_1024);
	m_TempSystemData.ViewState = NTxtView;	
	
	RTCHAR * pTempBuf = RWCHAR(m_strTempBuf1);	
	RTCHAR * pTempOut = RWCHAR(m_strTempBuf2);
	
	int nLine = 48;

#ifdef  APPLY_BOLD_FONT
	nLine = 42;
#endif
	
	while(pTempBuf)
	{
#ifdef UNICODE_ROCK
			pTempBuf = ::GetStringReturnInStringW('\\', false, nLine, pTempBuf, pTempOut, TRUE);
#else
			pTempBuf = ::GetStringReturnInString('\\', false, nLine, pTempBuf, pTempOut, TRUE);
#endif
	
		m_TempSystemData.ViewState = NTxtView;		
		Rstrcpy(m_TempSystemData.Text ,pTempOut );
		Rstrcpy(pTempOut, _RT("") );
		InsertSystemData();		
	}
	
	//ResetScroll();
}

void CSystemPrintWnd::InsertSystemData()
{
	if( m_nInsertIndex >= MAX_SYSTEMDATA_NUM )
	{
		for(int i = 0 ; i < MAX_SYSTEMDATA_NUM ; i++)
		{
			m_SystemTxtData[i] = m_SystemTxtData[i + 1];			
		}
		m_SystemTxtData[m_nInsertIndex-1] = m_TempSystemData;
	}
	else
	{
		m_SystemTxtData[m_nInsertIndex] = m_TempSystemData;
		m_nInsertIndex++;
	}
}