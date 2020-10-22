#include "..\\RockPCH.h"

#include "..\\..\\RockClient.h"
#include "Rui.h"
#include "Define.h"
#include "Util.h"
#include "Render.h"
#include "Fontman.h"
#include "Resource.h"
#include "Chain.h"
#include "Container.h"

#include "UiUtil.h"

#include "CGuideWnd.h"

#include "..\\..\\bravolibs\obj\etcstruct.h"
#include "..\\..\\VFileSystem.h"

CGuideWnd * g_pGuideWnd = NULL;

CGuideWnd::CGuideWnd()
{
	m_pGuideFrameWnd = NULL;
	memset( m_GuideField, 0, sizeof(CTextBox *) * 17 );
	m_pCloseBtn = NULL;
	m_pScrollBar = NULL;

	m_nStartListNum = 0;

	for(int i = 0; i < 120 ; i++)
	{
		memset( m_strLine[i], 0, sizeof(char) * 100 );
	}
}

CGuideWnd::~CGuideWnd()
{
	AllClearData();
}

void CGuideWnd::AllClearData()
{
	GuideDataListPos i = m_GuideDataList.begin();

    for( ; i != m_GuideDataList.end() ; ++i )
    {
        SGuideData * pData = (*i);
		SAFE_DELETE( pData );
		(*i) = NULL;
    }

    m_GuideDataList.clear();

	for(int j = 0; j < 120 ; j++)
	{
		memset( m_strLine[j], 0, sizeof(char) * 100 );
	}
}

void CGuideWnd::Init()
{


}

void CGuideWnd::Composition()
{
	CImageBox9*		img9 = NULL;
	
	int center_x	= nRui->Desktop.Width / 2;
	int center_y	= nRui->Desktop.Height / 2;

	SRect GuideWnd;
	GuideWnd.w	= 600;
	GuideWnd.h	= 400;
	GuideWnd.x	= center_x - ( GuideWnd.w / 2 );
	GuideWnd.y	= center_y - ( GuideWnd.h / 2 );

	m_pGuideFrameWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_None, WID_None, 
						GuideWnd.x, GuideWnd.y, GuideWnd.w, GuideWnd.h, false );
	m_pGuideFrameWnd->SetWndTexStyle( fwfTex_Style_Tile9 );
	m_pGuideFrameWnd->SetClientImage( TID_CTRL_WND_L1 );		
	m_pGuideFrameWnd->SetIconTID( TID_CTRL_ICON_HELP );
	m_pGuideFrameWnd->SetCaption( G_STRING_CHAR( IDS_ADVENTURE_BIBLE ) );						
	m_pGuideFrameWnd->SetWndTileSizeLR( 40, 29 );
	m_pGuideFrameWnd->SetWndTileSizeTB( 31, 11 );
	m_pGuideFrameWnd->SetWndProc( this );
	m_pGuideFrameWnd->SetTitle( true );
	m_pGuideFrameWnd->SetRegion(false);
	m_pGuideFrameWnd->SetVisible( false );
	m_pGuideFrameWnd->Initialize();

	img9 = (CImageBox9*)nRui->CreateWnd( n_wtImageBox9, WID_None, 
										m_pGuideFrameWnd->GetID(), 16, 42, 568, 342, false );       	
	img9->SetWndTileSizeLR( 9, 9 );
	img9->SetWndTileSizeTB( 8, 8 );
	img9->SetFocusCheck( false );
	img9->SetClientImage( TID_CTRL_CONTBACK_L1 );
	img9->SetAdjustPosition( 0, 0 );

	m_pCloseBtn = (CButton*)nRui->CreateWnd( n_wtButton, 
				  WID_None, m_pGuideFrameWnd->GetID(), 570, 6, 13, 13, false );
	m_pCloseBtn->SetBtnImage(TID_CTRL_BTN_X, TID_CTRL_BTN_X_OVR, 
							TID_CTRL_BTN_X_CLK, TID_None );

	//...........................................................................................................
	// 스크롤
	//...........................................................................................................
	m_pScrollBar = (CScrollBar*)nRui->CreateWnd( n_wtScrollBar, WID_None , m_pGuideFrameWnd->GetID() , 571, 43, 13, 340, false );
	m_pScrollBar->InitSlide( stVertSld, 0, 120, 0, 13 );//Type, min, max, value, 썸의 폭
	m_pScrollBar->SetClientImage( TID_CTRL_SCR_BG );
	m_pScrollBar->SetThumbImage( TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR_CLK, TID_None );
	m_pScrollBar->SetScrollBtnA( TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT_CLK, TID_None );
	m_pScrollBar->SetScrollBtnB( TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT_CLK, TID_None );

	int i = 0;
	int iAddPosY = 19;

	for( i = 0 ; i < 17 ; i++ )
	{
		m_GuideField[i] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pGuideFrameWnd->GetID(), 
		18, ( iAddPosY * i ) + 50, 550, 18, false );
		m_GuideField[i]->SetClientImage(TID_None);
		m_GuideField[i]->SetAlignText( n_atLeft, n_atCenter );
		m_GuideField[i]->SetBlank( 10 , 0, 0, 0 );	
		m_GuideField[i]->SetText(_RT(""));
	}	
}

void CGuideWnd::Update()
{


}

void CGuideWnd::Proc( SEventMessage* EMsg )
{
	if( EMsg->FocusWnd == m_pCloseBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			Close();
		}
	}
	else if( EMsg->FocusWnd == m_pScrollBar->GetID() ) 
	{
		if( ( EMsg->Notify == NM_SLD_MOVEBAR ) || 
			( EMsg->Notify == NM_SLD_CLICKMOVEBAR ) || 
			( EMsg->Notify == NM_SLD_BTNMOVERBAR ) )
		{
			SetListScroll( (int)EMsg->fParam );			
		}
	}	
}

void CGuideWnd::SetWheelValue( short _wheel )
{
	int value = 0;
	int Max   = 0;	

	Max = m_pScrollBar->GetSldMax();
	m_pScrollBar->GetValue( &value );
	
	if( ( _wheel < 0 ) && ( value < Max ) )
	{
		value += 1;
	}
	else if( ( _wheel > 0 ) && ( value > 0 ) )
	{
		value -= 1;
	}
	
	m_pScrollBar->SetValue( value );
	SetListScroll( value );
}

///-- 메인 클래스가 변화할때 해당 함수를 호출해서 가이드 데이타를 재갱신한다.
void CGuideWnd::RefreshGuideDataList()
{
	AllClearData();
	///-- 현재 클래스에 연관된 가이드문을 벡터 m_GuideDataList에 추가한다.
	
	char FileName[ 1024 ] = "";		
	wsprintf( FileName , "%s" , "Guide.dat" );

	VFileHandle* pFH = g_VFInterface.OpenFile( FileName );
	VFileHandle* pTFH = NULL;

	if( pFH == NULL ) 
	{
		return;
	}

	int nfile_size = pFH->GetSize();

	///-- 사이즈 정규성 Check
	if( nfile_size % sizeof( SGuideData ) != 0 )
	{
		g_VFInterface.CloseFile( pFH );
		return;
	}
	///-- Set Count
	int n_count = nfile_size / sizeof( SGuideData );

	for( int i = 0; i < n_count ; ++i )
	{
		SGuideData * pGuideData = SAFE_NEW( SGuideData );
		ZeroMemory(pGuideData , sizeof(SGuideData));

		pFH->Read( ( void * )pGuideData, sizeof( SGuideData ));
		 
		pGuideData->strDesc[299] = '\0';		
		pGuideData->strDesc[298] = '\0';		

		///-- 같은 클래스인지 체크해서
		///-- 집어넣는다.
		if( pGuideData->dClassNum == nRui->thePcParam.MainClass )
		{
			m_GuideDataList.push_back(pGuideData);
		}
		else
		{
			SAFE_DELETE( pGuideData );
		}
	}	

	g_VFInterface.CloseFile( pFH );	
	
	///-- m_GuideDataList의 내용을 문자열라인에 넣는다.
	InsertStrLine();
	///-- 문자열 라인을 GuideField 텍스트박스에 집어넣는다.
	SetGuideField();
}

///-- 레벨업시 - 현재 레벨에 맞는 가이드 문을 시스템 메시지로 출력한다.
void CGuideWnd::PrintNowLevelGuide()
{
	GuideDataListPos iPos = m_GuideDataList.begin();
	
	for( ; iPos != m_GuideDataList.end() ; ++iPos )
	{
		SGuideData * pData = (*iPos);
		if( pData->dLevel == nRui->thePcParam.Level )
		{
			SystemPrint( n_LightYellow, n_SystemMsg, pData->strDesc );
		}
	}
}

///-- 램덤하게 현재 클래스에 맞는 가이드문 데이터를 가져온다.
char * CGuideWnd::GetRandomHelpGuide()
{
	int nMax = m_GuideDataList.size();
	int nRnd = RandomNum(0, nMax - 1);
	
	if( !m_GuideDataList.empty() )
	{
		return m_GuideDataList[nRnd]->strDesc;
	}	
	
	return NULL;
}

void CGuideWnd::InsertStrLine()
{  
	int nCount = 0;
	int nStrLineCount = 0;

	RTCHAR nTempStr1[256] = _RT("");
	RTCHAR nTempStr2[256] = _RT("");
		
	GuideDataListPos iPos = m_GuideDataList.begin();	
	
	for( ; iPos != m_GuideDataList.end() ; ++iPos )
	{
		SGuideData * pData = (*iPos);
		nCount++;
		Rsprintf(RWCHAR(nTempStr1), _RT("- %s"), RWCHAR(pData->strDesc) );
		
		RTCHAR * pTempBuf = nTempStr1;
		RTCHAR * pTempOut = nTempStr2;

		int nLineCount = 0;	
		int nLineSize = 70;
		  
		while(pTempBuf)
		{
			if(nLineCount == 0)
			{
				nLineSize = 70;

#ifdef UNICODE_ROCK
				pTempBuf = ::GetStringReturnInStringW('\n', false, nLineSize, pTempBuf, pTempOut);
#else
				pTempBuf = ::GetStringReturnInString('\n', false, nLineSize, pTempBuf, pTempOut);
#endif

			}
			else
			{
				nLineSize = 64;
#ifdef UNICODE_ROCK
				pTempBuf = ::GetStringReturnInStringW('\n', false, nLineSize, pTempBuf, pTempOut);				
#else
				pTempBuf = ::GetStringReturnInString('\n', false, nLineSize, pTempBuf, pTempOut);				
#endif
			}		
		
			if(nLineCount)
			{
				Rsprintf( RWCHAR(m_strLine[nStrLineCount]), _RT("%s%s"), _RT("   "), pTempOut);
			}
			else
			{
				Rsprintf( RWCHAR(m_strLine[nStrLineCount]), _RT("%s"), pTempOut);
			}

			ZeroMemory(pTempOut , MAX_SIZE_256);

			
			nLineCount++;

			if( nStrLineCount >= 120 )
			{
				MessageBox(NULL , "Guide Over Max 120 string" , "", MB_OK);
				break;
			}

			nStrLineCount++;
		}
	}
}

void CGuideWnd::SetGuideField()
{
	for( int i = 0; i < 17 ; ++i )
	{		
		int nLine =  m_nStartListNum + i;
		if( nLine < 120 )
		{
			m_GuideField[i]->SetText(m_strLine[nLine]);
		}
		else
		{
			m_GuideField[i]->SetText(_RT(""));
		}
	}
}

void  CGuideWnd::SetListScroll( int _linenum )
{
	m_nStartListNum = _linenum * 1;	
	SetGuideField();
}

void  CGuideWnd::Open()
{
	nRui->SendEvent( m_pGuideFrameWnd->GetID(), n_emSetVisible, (EPARAM)true, 0, 0, 0 );
	m_pScrollBar->SetValue( 0 );
	SetListScroll(0);
}

void  CGuideWnd::Close()
{
	nRui->SendEvent( m_pGuideFrameWnd->GetID(), n_emSetVisible, (EPARAM)false, 0, 0, 0 );
}
