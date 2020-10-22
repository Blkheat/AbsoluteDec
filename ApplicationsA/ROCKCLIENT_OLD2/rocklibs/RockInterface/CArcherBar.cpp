#include "..\\RockPCH.h"
#include "CArcherBar.h"
#include "Rui.h"
#include "..\\..\\global.h"
#include "..\\..\\RockClient.h"

#include "..\\..\\quadlist.h"
#include "..\\..\\bravolibs\\network\\Protocol.h"
#include "..\\..\\Pc.h"
#include "..\\..\\GTimer.h"
#include "..\\..\\CRenderManager.h"
#include "..\\..\\bravolibs\\Effect\\CTextCommand.h"

#include "..\\..\\bravolibs\\obj\\player.h"
#include "..\\..\\bravolibs\\obj\\nonplayer.h"

CArcherBar	g_ArcherBar;

#define RIGHT_EXTRA_WIDTH	30							// 오른쪽 공백 크기

#define CRITICAL_HIT_WIDTH	   	   14					// 크리티컬 히트 크기
#define HIT_WIDTH				   14					// 히트 크기
#define FIX_ADD_Y				   4					// 바에서 히트 까지 높이
#define FIX_ADD_X				   13					// 바에서 프로그래스 시작까지의 거리
#define PRGRESS_BAR_SIZE		   304

#define ARCHER_MAX_TIME			   800		
//#define ARCHER_MAX_TIME			   5000		

CArcherBar::CArcherBar()
{
	m_ISHIT = FALSE;
	m_Count	= 0;

	m_nCriticalWidth = 0;
	m_nHitWidth = 0;
	m_ArcherMaxTime = 0;
	
	m_Percent = 0;	
	
	// 레벨 1 일때 컨디션 게이지 히트 테이블
	{ 
		HIT_ZONE_TABLE TempTable;
		TempTable.List.push_back( HIT_ZONE      );
		TempTable.List.push_back( HIT_ZONE      );
		TempTable.List.push_back( CRITICAL_ZONE );
		TempTable.List.push_back( CRITICAL_ZONE );
		TempTable.List.push_back( HIT_ZONE      );
		TempTable.List.push_back( HIT_ZONE      );
		
		m_HitTable.push_back( TempTable );
	}
	
	// 레벨 2 일때 컨디션 게이지 히트 테이블
	{ 
		HIT_ZONE_TABLE TempTable;
		TempTable.List.push_back( HIT_ZONE      );
		TempTable.List.push_back( HIT_ZONE      );
		TempTable.List.push_back( CRITICAL_ZONE );
		TempTable.List.push_back( CRITICAL_ZONE );
		TempTable.List.push_back( HIT_ZONE      );
		TempTable.List.push_back( HIT_ZONE      );
		TempTable.List.push_back( HIT_ZONE      );
		
		m_HitTable.push_back( TempTable );
	}
	
	// 레벨 3 일때 컨디션 게이지 히트 테이블
	{ 
		HIT_ZONE_TABLE TempTable;
		TempTable.List.push_back( HIT_ZONE      );
		TempTable.List.push_back( HIT_ZONE      );
		TempTable.List.push_back( HIT_ZONE      );
		TempTable.List.push_back( CRITICAL_ZONE );
		TempTable.List.push_back( CRITICAL_ZONE );
		TempTable.List.push_back( HIT_ZONE      );
		TempTable.List.push_back( HIT_ZONE      );
		TempTable.List.push_back( HIT_ZONE      );
		
		m_HitTable.push_back( TempTable );
	}
	
	// 레벨 4 일때 컨디션 게이지 히트 테이블
	{ 
		HIT_ZONE_TABLE TempTable;
		TempTable.List.push_back( HIT_ZONE      );
		TempTable.List.push_back( HIT_ZONE      );
		TempTable.List.push_back( HIT_ZONE      );
		TempTable.List.push_back( CRITICAL_ZONE );
		TempTable.List.push_back( CRITICAL_ZONE );
		TempTable.List.push_back( HIT_ZONE      );
		TempTable.List.push_back( HIT_ZONE      );
		TempTable.List.push_back( HIT_ZONE      );
		TempTable.List.push_back( HIT_ZONE      );
		
		m_HitTable.push_back( TempTable );
	}
	
	// 레벨 5 일때 컨디션 게이지 히트 테이블
	{ 
		HIT_ZONE_TABLE TempTable;
		TempTable.List.push_back( HIT_ZONE      );
		TempTable.List.push_back( HIT_ZONE      );
		TempTable.List.push_back( HIT_ZONE      );
		TempTable.List.push_back( HIT_ZONE      );
		TempTable.List.push_back( CRITICAL_ZONE );
		TempTable.List.push_back( CRITICAL_ZONE );
		TempTable.List.push_back( HIT_ZONE      );
		TempTable.List.push_back( HIT_ZONE      );
		TempTable.List.push_back( HIT_ZONE      );
		TempTable.List.push_back( HIT_ZONE      );
		
		m_HitTable.push_back( TempTable );
	}
	
	// 레벨 6 일때 컨디션 게이지 히트 테이블
	{ 
		HIT_ZONE_TABLE TempTable;
		TempTable.List.push_back( HIT_ZONE      );
		TempTable.List.push_back( HIT_ZONE      );
		TempTable.List.push_back( HIT_ZONE      );
		TempTable.List.push_back( HIT_ZONE      );
		TempTable.List.push_back( CRITICAL_ZONE );
		TempTable.List.push_back( CRITICAL_ZONE );
		TempTable.List.push_back( HIT_ZONE      );
		TempTable.List.push_back( HIT_ZONE      );
		TempTable.List.push_back( HIT_ZONE      );
		TempTable.List.push_back( HIT_ZONE      );
		
		m_HitTable.push_back( TempTable );
	}
	
	// 레벨 7 일때 컨디션 게이지 히트 테이블
	{ 
		HIT_ZONE_TABLE TempTable;
		TempTable.List.push_back( HIT_ZONE      );
		TempTable.List.push_back( HIT_ZONE      );
		TempTable.List.push_back( HIT_ZONE      );
		TempTable.List.push_back( HIT_ZONE      );
		TempTable.List.push_back( CRITICAL_ZONE );
		TempTable.List.push_back( CRITICAL_ZONE );
		TempTable.List.push_back( HIT_ZONE      );
		TempTable.List.push_back( HIT_ZONE      );
		TempTable.List.push_back( HIT_ZONE      );
		TempTable.List.push_back( HIT_ZONE      );
		
		m_HitTable.push_back( TempTable );
	}
	
	m_fFlashTime = 0.25f;
	m_fCumulativeTime = 0.0f;
	m_bIsCritical = false;
	m_fGlowTime = 0.15f;
	m_bIsGlow = false;
	m_fGowCumtime = 0.0f;
}

CArcherBar::~CArcherBar()
{
	DeleteMemory();
}

void CArcherBar::DeleteMemory(void)
{
	m_TimerList.clear();
}

void CArcherBar::Init()
{
}

void CArcherBar::Composition()
{
	SRect FrameRect;
	
	FrameRect.w	= nRui->Desktop.Width;
	FrameRect.h	= nRui->Desktop.Height;
	
	FrameRect.x	= 0;
	FrameRect.y	= 0;
	
	//..........................................................................................................
	// 선택창
	//..........................................................................................................
	m_pFrameWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_ArcherBar_Wnd, WID_None, 
		0 , 0 , 0 , 0 , false );
	
	m_pFrameWnd->SetClientImage( TID_None );
	m_pFrameWnd->SetCaption( _RT("Archer Window") );
	m_pFrameWnd->SetWndProc( this );
	m_pFrameWnd->SetTitle( false );	
	m_pFrameWnd->Initialize();
	m_pFrameWnd->SetVisible( false );
	m_pFrameWnd->SetMove( FALSE );
	
	m_BarPX = ( 1024 - 330 ) / 2;
	m_BarPY = 600;

	CTextCommand TextCmd;

	if( TextCmd.OpenCharVF( "archer.txt" ) == NULL )
    {
        return;
    }

	int nCount = 0;

	while( TextCmd.GetLine() != NULL )
	{
		if( TextCmd.ISArgEmpty() )
        {
            continue;
        }

		if(nCount == 0)
		{
			TextCmd.GetArgValue( 0 , m_nCriticalWidth );
		}
		else if(nCount == 1)
		{
			TextCmd.GetArgValue( 0 , m_nHitWidth );
		}
		else if(nCount == 2)
		{
			TextCmd.GetArgValue( 0 , m_Percent );
		}

		nCount++;
	}

	TextCmd.CloseChar();

	m_ArcherMaxTime = (int)( ARCHER_MAX_TIME * ( m_Percent / 100.0f ) );
	m_fVelRatio = ( m_Percent / 100.0f );
}

void CArcherBar::Start(int Level)
{
	if( g_Pc.GetPlayer()->m_curt_event.type == SM_STATE_DIE )
		return;
	
	DeleteMemory();

	
	// 컨디션 게이지 테이블보다 큰지 검사 ( 크면 게이지 띄우지 않기 )
	if( m_HitTable.size() < Level )
	{
		return;
	}

	m_ArcherMaxTime = (int)( ARCHER_MAX_TIME * 
					  ( g_Pc.GetPlayer()->m_realani_time / 27.0f ) * m_fVelRatio );

	m_pFrameWnd->SetVisible( TRUE );
	
	m_ISHIT   = FALSE;

	m_OldTime = timeGetTime();	
	
	m_SelectHit = NONE_HIT_ZONE;
	m_fCumulativeTime = 0.0f;
	m_fGowCumtime = 0.0f;
	m_bIsCritical = false;
	m_bIsGlow = false;
}

void CArcherBar::End(void)
{
	DeleteMemory();
	
	if( m_pFrameWnd != NULL )
	{
		nRui->SendEvent( WID_ArcherBar_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	}
}

void CArcherBar::Update()
{
	if( ISVisible() == FALSE )
	{
		return;
	}
	
	if( g_Pc.GetPlayer()->m_curt_event.type == SM_STATE_DIE )
		End();
	
	float Per = 0.0f;
	
	DWORD RangeTime = SAFE_TIME_SUB( timeGetTime() , m_OldTime );
	
	if( RangeTime == 0 )
	{
		m_GageWidth = 0;
	}
	else
	{
		Per			= ( float ) RangeTime / ( float ) m_ArcherMaxTime;
		m_GageWidth = PRGRESS_BAR_SIZE * Per;
	}
	
	//.........................................................................................................
	// 시간 다 지났을때
	//.........................................................................................................
	if( Per >= 1.0f )
	{
		End();
		
		if( m_ISHIT == FALSE )
			m_Count = 0;
		
		g_Pc.GetPlayer()->CM__ATTACK__BOW( m_Count , m_SelectHit );
		
		return;
	}
	
	return;
	
}

int CArcherBar::Hit(void)
{
	if( ISVisible() == FALSE || m_ISHIT == TRUE )
	{
		return 0;
	}
	
	m_ISHIT			= TRUE;

	DWORD Time = SAFE_TIME_SUB( timeGetTime() , m_OldTime );
	///--PRINT_DLG_LOG("Click Time = %u", Time);
	
	std::vector<ARCHER_TIMER>::iterator i = m_TimerList.begin();
	
	for( ; i != m_TimerList.end() ; ++i )
	{
		if( i->StartTime < Time  &&
			i->EndTime   >= Time    )
		{
			m_SelectHit = i->Type;
			
			//크리티컬존 일경우
			if(CRITICAL_ZONE == m_SelectHit)
			{
				m_bIsCritical = true;			
			}

			m_bIsGlow = true;

			m_rGlowRect.w = 9;
			m_rGlowRect.h = 18;
				
			m_pCenPoint.x = i->PosX + 9/2;
			m_pCenPoint.y = m_BarPY + 18/2;
		
			
			if( i->Type == CRITICAL_ZONE )
			{
				i->TID = TID_ArcherBar_Hit_Check;
				
				m_Count++;
				
				if( m_Count > 10 )
					m_Count = 10;
				
				break;
			}
			else
			{
				i->TID = TID_ArcherBar_Hit_Check;
				
				m_Count = 0;
				
				break;
			}
		}
	}
	
	if( Time == 0 )
	{
		m_GageWidth = 0;
	}
	else
	{
		float Per			= ( float ) Time / ( float ) m_ArcherMaxTime;
		m_GageWidth = PRGRESS_BAR_SIZE * Per;
	}
	
	m_StopGageWidth = m_GageWidth;
	
	return 0;
}

void CArcherBar::Proc( SEventMessage* EMsg )
{
}

void CArcherBar::RenderProc()
{
	
}

int CArcherBar::ISVisible(void)
{
	if( m_pFrameWnd == NULL )
		return FALSE;
	
	if( m_pFrameWnd->IsVisible() )
		return TRUE;
	
	return FALSE;
}
