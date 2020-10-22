// DelayBarWnd.cpp: implementation of the CDelayBarWnd class.
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


#include "CDelayBarWnd.h"
#include "..\\RockClient.h"
#include "..\\..\\global.h"
#include "..\\..\\Effect.h"

#include "ChatBoxWndProc.h"
#include "SysMsgWndProc.h"
#include "SystemMenuWndProc.h"
#include "ChatOutput.h"
#include "SysMsgOutput.h"
#include "QuestProc.h"
#include "PlayerInfoWnd.h"
#include "NonPlayerInfoWnd.h"
#include "stringmanager.h"


CDelayBarWnd g_DelayBarWnd;
extern	long			g_nowTime;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDelayBarWnd::CDelayBarWnd()
{
	m_BStart = FALSE;
	m_BActivate =  FALSE;
	m_BIsDie = FALSE;
	
	m_TDelay_Type = DELAY_NONE;
	m_pProgressBar = NULL;
	m_pTextBox = NULL;
	
	m_fDelay_Time = 10.0f;
	m_fProgress_Time = 0.0f;
	m_lPrevTime = g_nowTime;
	
	SAFE_STR_CPY( m_tstrNofify, _RT(""), 512 );
	
	m_DwTempItemUID = 0;
	
	m_vPlayerPos = D3DXVECTOR3(0.0f, 0.0f,0.0f);
	m_vOldPlayerPos = m_vPlayerPos;
}

CDelayBarWnd::~CDelayBarWnd()
{
	
}

void CDelayBarWnd::Init()
{
	
}

BOOL CDelayBarWnd::IsStart()
{   
	
	///-- 이경우에만 특수하게 막아준다.
	if( m_TDelay_Type == DELAY_RIDE_ON)
	{
		return TRUE;
	}
	
	return m_BStart;
}

void CDelayBarWnd::Clear()
{
	m_BStart = FALSE;
	nRui->SendEvent( WID_DelayBar_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	m_TDelay_Type = DELAY_NONE; 
	m_BActivate = FALSE;
}

void CDelayBarWnd::Composition()
{
	CFrameWnd*		fw;
	CProgressBar*	pb;
	CTextBox*        tb;
	
	
	int center_x	= nRui->Desktop.Width / 2;
	
	SRect Delaywnd;
	Delaywnd.w	= 393;
	Delaywnd.h	= 25;
	Delaywnd.x	= center_x - ( Delaywnd.w / 2 );
	Delaywnd.y	= nRui->Desktop.Height - 150;
	
	fw = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_DelayBar_Wnd, WID_None, Delaywnd.x, Delaywnd.y, Delaywnd.w, Delaywnd.h, false );
	fw->SetClientImage( TID_LogoutClient );
	fw->SetCaption( _RT("DelayBar") );
	fw->SetWndProc( this );
	fw->SetTitle( false );   
	fw->SetMove( false );
	fw->SetVisible( false );
	fw->SetZorder( n_zoTop );
	fw->Initialize(); 
	
	pb = (CProgressBar*)nRui->CreateWnd( n_wtProgressBar, WID_DelayBar_TimeBar, WID_DelayBar_Wnd, 24, 0, 345, 25, false );
	pb->SetClientImage( TID_None );
	pb->SetBarImage( TID_LogoutBar );
	pb->SetBarBlank( 0, 0 );
	pb->InitProgress( 0 );
	
	m_pProgressBar = pb;
	
	tb = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_DelayBar_NotifyTex, WID_DelayBar_Wnd, 24, 1, 345, 25, false );
	tb->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	tb->SetClientImage( TID_None );
	tb->SetClientColor( 180, 175, 160 );
	tb->SetAlignText( n_atCenter, n_atCenter );
	tb->SetText( _RT("") );
	tb->SetTextColor( 230, 230, 255 );
	tb->SetZorder( n_zoTop );   
	m_pTextBox = tb;
}

void CDelayBarWnd::Update()
{
	if(m_BActivate)
	{ 
		m_fProgress_Time += SAFE_TIME_SUB( g_nowTime , m_lPrevTime ) * 0.001;
		int DelayPercent = (int)(((double)m_fProgress_Time / m_fDelay_Time) * 100.0f);
		int RemainTime = (int)(m_fDelay_Time - m_fProgress_Time) + 1;	
		
		Player*	user_pc = g_Pc.GetPlayer();
		D3DXMATRIX		matchar;
		
		//캐릭터가 죽으면 딜레이바를 없애고 초기화
		if( ( m_TDelay_Type != DELAY_LOG_OFF ) && 
			( m_TDelay_Type != DELAY_GAME_OUT ) &&
			( user_pc->IsRealDie() ) )
		{
			m_BStart = FALSE;
			nRui->SendEvent( WID_DelayBar_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
			m_TDelay_Type = DELAY_NONE; 
			m_BActivate = FALSE;
		}	  
		
		// 이 모션 중에는 라이딩을 소환 할수 없습니다 
		if ((m_TDelay_Type == DELAY_RIDE_ON ) && g_Pc.GetPlayer()->m_curt_event.type  == SM_ACT) 
		{
			m_BStart = FALSE;
			nRui->SendEvent( WID_DelayBar_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
			m_TDelay_Type = DELAY_NONE; 
			m_BActivate = FALSE;
		}	  
		
		if ( (m_TDelay_Type == DELAY_PVP_ROOM ) || 
			(m_TDelay_Type == DELAY_LOG_OFF) ||
			(m_TDelay_Type == DELAY_GAME_OUT) ||
			m_TDelay_Type == DELAY_MAIN_RETURN )
		{
			if( g_Pc.GetPlayer()->m_curt_event.type == SM_MOVE_ATTACK || 
				g_Pc.GetPlayer()->m_curt_event.type == SM_ATTACK_NPC ||
				g_Pc.GetPlayer()->m_curt_event.type == SM_ATTACK_DELAY ||
				g_Pc.GetPlayer()->m_curt_event.type == SM_UNDERATTACK ||
				g_Pc.GetPlayer()->m_curt_event.type == SM_USE_SKILL )
			{
				// ResetDelay(DELAY_PVP_ROOM, 0, 0 );
				Clear();
			}
		}
		
		matchar = user_pc->GetPosTM();
		m_vPlayerPos = D3DXVECTOR3(matchar._41, matchar._42, matchar._43);
		
		//캐릭터가 이동하면 딜레이바를 없애고 초기화
		float fDel = D3DXVec3Length(&(m_vOldPlayerPos - m_vPlayerPos));
		if(fDel > 0.2f)
		{
			m_BStart = FALSE;
			nRui->SendEvent( WID_DelayBar_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
			m_TDelay_Type = DELAY_NONE; 
			m_BActivate = FALSE;
			return; 
		}
		
		//딜레이바가 100%이면 각 상황에 따른 후처리
		if( (DelayPercent > 100) && m_BStart )
		{
			DelayPercent = 100; 
			m_BStart = FALSE;	
			
			ProcessDelayAfter();
			
			if( m_TDelay_Type != DELAY_RIDE_ON)
			{
				m_TDelay_Type = DELAY_NONE;
				nRui->SendEvent( WID_DelayBar_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );			
			}		
			
			return;		
		}
		
		RTCHAR* pTStr = NULL;
		switch(m_TDelay_Type)
		{
		case DELAY_LOG_OFF:			pTStr = G_STRING_CHAR( IDS_WND_LOGOFF );	  break;
		case DELAY_GAME_OUT:		pTStr = G_STRING_CHAR( IDS_WND_GAME_EXIT );	  break;
		case DELAY_INVEN_RETURN:     
		case DELAY_MAIN_RETURN:		pTStr = G_STRING_CHAR( IDS_WND_WARP );	  break;
		case DELAY_RIDE_ON:			pTStr = G_STRING_CHAR( IDS_RIDING );		  break;					
		case DELAY_RIDE_OFF:			pTStr = G_STRING_CHAR( IDS_RIDING );		  break;					
		case DELAY_PVP_ROOM:		pTStr = G_STRING_CHAR(IDS_WND_PVP_ROOM);	  break;
		default:					 pTStr = _RT("");	  break; 		  
		}	  
		
		Rsprintf(RWCHAR(m_tstrNofify),G_STRING_CHAR( IDS_WND_TIME_REMAIN ),  RWCHAR(pTStr), RemainTime);
		
		if(m_pProgressBar)
			m_pProgressBar->SetPercent(DelayPercent);	  
		
		if(m_pTextBox)
			m_pTextBox->SetText( m_tstrNofify );
	}
	
	m_lPrevTime = g_nowTime;
}

void CDelayBarWnd::Proc( SEventMessage* EMsg )
{
	
	
	
}

void CDelayBarWnd::Close()
{
	m_TDelay_Type = DELAY_NONE;
	nRui->SendEvent( WID_DelayBar_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );			
}


void CDelayBarWnd::ResetDelay(NDELAY_TYPE DType, DWORD DwTempId, BOOL IsDie)
{   
	
	if(m_TDelay_Type==DType)return;
	
	switch(DType)
	{
	case DELAY_RIDE_ON: m_fDelay_Time = 3.0f;
		break;
	case DELAY_RIDE_OFF: m_fDelay_Time = 3.0f;
		break;
	default: m_fDelay_Time = 10.0f;
		break;
	}
	
	m_TDelay_Type = DType;
	m_DwTempItemUID = DwTempId;
	
	m_BActivate = TRUE;
	m_BStart = TRUE;
	m_BIsDie = IsDie;   
	
	m_lPrevTime = g_nowTime;
	m_fProgress_Time = 0.0f;
	
	if(m_pProgressBar)
		m_pProgressBar->SetPercent(0);
	
	Player*	user_pc = g_Pc.GetPlayer();
	D3DXMATRIX		matchar;
	
	matchar = user_pc->GetPosTM();
	m_vPlayerPos = D3DXVECTOR3(matchar._41, matchar._42, matchar._43);
	m_vOldPlayerPos = m_vPlayerPos;   
}

void CDelayBarWnd::ProcessDelayAfter()
{
	switch(m_TDelay_Type)
	{
	case DELAY_LOG_OFF:           ProcessDelayLogOff();		                            
		break;
	case DELAY_GAME_OUT:			ProcessGameOut();
		break;
	case DELAY_INVEN_RETURN:      ProcessDelayInven_Return();
		break;
	case DELAY_MAIN_RETURN:       ProcessDelayMain_Return();
		break;
	case DELAY_RIDE_ON:			ProcessRideOn();							
		break;	
	case DELAY_RIDE_OFF:			ProcessRideOff();
		break;
	case DELAY_PVP_ROOM:		ProcessPvPRoom();	
		break; 
	default:                      break;
	}
	
   	m_BActivate = FALSE;
	m_BIsDie = FALSE;
}

void CDelayBarWnd::ProcessDelayLogOff()
{
	SEventMessage EMsg;
	
	EMsg.FocusWnd = WID_System_LogOffBtn;
	EMsg.Notify = NM_BTN_CLICK;
	SystemMenuWnd.Proc(&EMsg);   
}

void CDelayBarWnd::ProcessGameOut()
{
	SEventMessage EMsg;
	
	EMsg.FocusWnd = WID_System_GameExitBtn;
	EMsg.Notify = NM_BTN_CLICK;
	SystemMenuWnd.Proc(&EMsg);
}

void CDelayBarWnd::ProcessDelayInven_Return()
{	
	// 부가효과 (아이템 사용 불가) 처리 
	if( g_Pc.GetPlayer()->GetAddEffectState(n_TypeNotItem))
	{
		return;
	}
	
    g_RockClient.Send_RpCsUse( m_DwTempItemUID );							
    nRui->ResetCursorItem();
	
	return;
}

void CDelayBarWnd::ProcessDelayMain_Return()
{
	// 부가효과 (아이템 사용 불가) 처리
	if( g_Pc.GetPlayer()->GetAddEffectState(n_TypeNotItem))
	{
		return;
	}
	
	g_Particle.m_EffectManager.DeleteRestSkill( g_Pc.GetPlayer());
	g_RockClient.Send_RpCsUse( m_DwTempItemUID );
	
	return;
}

void CDelayBarWnd::ProcessRideOn()
{
	if( FALSE == g_RockClient.CanRideOnDest() )
	{
		return;
	}
	
	g_RockClient.Send_CsRideOn();
	
	return;
}
void CDelayBarWnd::ProcessRideOff()
{

	g_RockClient.Send_CsRideOff();

	return;
}
void CDelayBarWnd::ProcessPvPRoom()
{
	nRui->SendEvent( WID_BZ_SELECTZONE_WND, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	g_RockClient.Send_CsSelectFightGameType( n_FreeBattleGame );
	
	return;
}