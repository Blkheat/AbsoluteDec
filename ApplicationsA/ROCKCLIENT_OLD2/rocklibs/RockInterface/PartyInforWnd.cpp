#include "..\\..\\RockClient.h"
#include "..\\RockPCH.h"
#include "Rui.h"

#include "Define.h"
#include "Util.h"
#include "Render.h"
#include "Fontman.h"
#include "Resource.h"
#include "Chain.h"
#include "Container.h"

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
#include "Wnd.h"

#include "..\\..\\global.h"

#include "..\\..\\quadlist.h"
#include "..\\..\\bravolibs\\network\\Protocol.h"
#include "..\\..\\Pc.h"

#include "UIMsgWnd.h"
#include "PartyPopupWnd.h"
#include "PartyInforWnd.h"
#include "NonPlayerInfoWnd.h"
#include "StringManager.h"
#include "LeagueInfoWnd.h"

extern long			g_nowTime;
#ifdef DIRECT_VERSION_9_MJH
extern IDirect3DDevice9*	g_lpDevice;
#else
extern IDirect3DDevice8*	g_lpDevice;
#endif // DIRECT_VERSION_9_MJH

CPartyInforWnd g_PartyInforWnd;

///--JAPAN_BUG_MODIFY
BOOL IsMyPartyAndLeagueMember( UINT dPCID )
{
	if( g_PartyInforWnd.FindPartyMember( dPCID ) )
	{
		return TRUE;
	}
	
	if( g_LeagueInfoWnd.FindLeagueMember( dPCID ) )
	{
		return TRUE;
	}
	
	return FALSE;
}

CPartyInforWnd::CPartyInforWnd()
{
	memset(&m_PartyInfo, 0 , sizeof(SPARTY_INFO));	
	
	m_nSelectMemberIndex = -1;
	
	m_bIsMemberParty = FALSE;
	m_bIsMinimize = false;
	m_bViewItemHowMenu = false;

	m_pPartyInforFrame = NULL;

	//상단 컨트롤 윈도우 프레임
    m_pPartyTitleWnd = NULL;
	m_pGetItemViewBtn = NULL;
	m_pPartyNameTxt = NULL;
	m_pPartyMinBtn = NULL;

	//하단 컨트롤 윈도우 프레임
	m_pPartyDownWnd = NULL;
	m_pLeagueViewBtn = NULL;			
	m_pPartyAddMemberBtn = NULL;		
    m_pPartyBreakBtn = NULL;
	
    //상단 윈도우 위쪽 아이템 정책프레임
	m_pPartyGetItemMenuWnd = NULL;
	m_pPartyGetItemFreeBtn = NULL;		
	m_pPartyGetItemSequenceBtn = NULL;	
	m_pPartyGetCountryBtn = NULL;
	
	memset(m_tmpStr , NULL , MAX_SIZE_128 );
	memset(m_strToolTipArea , NULL , MAX_SIZE_256 );
		
	m_WIDToolTipDestWnd = WID_None;
	m_uPartyType = NORMAL_PARTY;
		
	ClearPartyInfo();
}

CPartyInforWnd::~CPartyInforWnd()
{
	ClearAllPartyMemberList();
}

void CPartyInforWnd::Init()
{
	
}

void CPartyInforWnd::SetPartyType(UINT uType)
{
	m_uPartyType = uType;
	if(LEAGUE_PARTY == m_uPartyType)
	{
		m_pPartyNameTxt->SetHID(HD_LEAGUE_RENAME);
		m_pPartyAddMemberBtn->SetHID(HD_LEAGUE_IN);
		m_pPartyBreakBtn->SetHID(HD_LPBREAKBTN);
		
		m_pLeagueViewBtn->SetVisible(true);
	}
	else
	{	
		m_pPartyNameTxt->SetHID(HD_PARTY_RNAME);
		m_pPartyAddMemberBtn->SetHID(HD_PARTY_IN);
		m_pPartyBreakBtn->SetHID(HD_PBREAKBTN);
	}
}

void CPartyInforWnd::Composition()
{
	//파티창 전체 프레임
	m_pPartyInforFrame = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_PartyInforWnd, WID_None, 
	13, 197, 152, 0, false );
	m_pPartyInforFrame->SetClientImage( TID_None );
    m_pPartyInforFrame->SetCaption( _RT("PartyMsg Window") );
    m_pPartyInforFrame->SetWndProc( this );
    m_pPartyInforFrame->SetTitle( false );
    m_pPartyInforFrame->SetVisible( false );
    m_pPartyInforFrame->SetMove( true );
	m_pPartyInforFrame->SetZorder( n_zoTop );
    m_pPartyInforFrame->Initialize();

	//상단 컨트롤 윈도우 프레임
	m_pPartyTitleWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_None, WID_PartyInforWnd, 
		3, 0, 145, 27, false );
	m_pPartyTitleWnd->SetClientImage( TID_PARTY_TITLE_WND );
	m_pPartyTitleWnd->SetCaption( _RT("") );
	m_pPartyTitleWnd->SetWndProc( this );
	m_pPartyTitleWnd->SetTitle( false );
	m_pPartyTitleWnd->SetVisible( false );
	m_pPartyTitleWnd->SetMove( false );
	m_pPartyTitleWnd->SetZorder( n_zoBottom );
	m_pPartyTitleWnd->Initialize();

	m_pGetItemViewBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, m_pPartyTitleWnd->GetID(), 
		3, 2, 13, 13, false );	
	m_pGetItemViewBtn->SetBtnImage( TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT_OVR, 
									TID_CTRL_SCR2_UBT_CLK, TID_None );
	m_pGetItemViewBtn->SetHID(HD_ITEM_VIEW);
		
	m_pPartyNameTxt = (CLimitLenTextBox*)nRui->CreateWnd( n_wtLimitLenTextBox, WID_None, m_pPartyTitleWnd->GetID(), 
		19, 3, 96, 12, false );
	m_pPartyNameTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pPartyNameTxt->SetClientImage( TID_None );
	m_pPartyNameTxt->SetAlignText( n_atLeft, n_atCenter );	
	m_pPartyNameTxt->SetText(_RT(""));	
	m_pPartyNameTxt->SetLimitLength(18);	
	m_pPartyNameTxt->SetTextColor( 255, 255, 255 );
	
	m_pPartyNameTxt->SetHID(HD_PARTY_RNAME);

	m_pPartyMinBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, m_pPartyTitleWnd->GetID(), 
		129, 2, 13, 13, false );	
	m_pPartyMinBtn->SetBtnImage( TID_CTRL_BTNMIN, TID_CTRL_BTNMIN_OVR, 
								 TID_CTRL_BTNMIN_CLK, TID_None );

	//상단 윈도우 위쪽 아이템 정책프레임
	m_pPartyGetItemMenuWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_None, WID_None, 
		0, 0, 72, 20, false );
	m_pPartyGetItemMenuWnd->SetClientImage( TID_PARTY_GETITEM_HOWWND );
    m_pPartyGetItemMenuWnd->SetCaption( _RT("") );
    m_pPartyGetItemMenuWnd->SetWndProc( this );
    m_pPartyGetItemMenuWnd->SetTitle( false );
    m_pPartyGetItemMenuWnd->SetVisible( false );
    m_pPartyGetItemMenuWnd->SetMove( false );
	m_pPartyGetItemMenuWnd->SetZorder( n_zoTop );
    m_pPartyGetItemMenuWnd->Initialize();

	m_pPartyGetItemFreeBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None,
								m_pPartyGetItemMenuWnd->GetID(), 10, 3, 16, 16, false );
	m_pPartyGetItemFreeBtn->SetBtnImage( TID_None, TID_None, 
		TID_None, TID_None );
	m_pPartyGetItemFreeBtn->SetHID(HD_ITEM_FREE);
	
	m_pPartyGetItemSequenceBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, 
								m_pPartyGetItemMenuWnd->GetID(), 29, 3, 16, 16, false );
	m_pPartyGetItemSequenceBtn->SetBtnImage( TID_None, TID_None, 
		TID_None, TID_None );
	m_pPartyGetItemSequenceBtn->SetHID(HD_ITEM_SEQ);
	
	m_pPartyGetCountryBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, 
								m_pPartyGetItemMenuWnd->GetID(), 48, 3, 16, 16, false );
	m_pPartyGetCountryBtn->SetBtnImage( TID_None, TID_None, 
		TID_None, TID_None );
	m_pPartyGetCountryBtn->SetHID(HD_ITEM_CON);
	
	SRect PartyTitleWndRect;
	SRect PartyGetItemMenuWndRect;
	
	m_pPartyTitleWnd->GetClientSize(&PartyTitleWndRect);
	m_pPartyGetItemMenuWnd->GetClientSize(&PartyGetItemMenuWndRect);

	int nXpos = PartyTitleWndRect.x;
	int nYpos = PartyTitleWndRect.y - PartyGetItemMenuWndRect.h - 1;

	m_pPartyGetItemMenuWnd->SetAbsolutePosition(nXpos, nYpos);
	m_pPartyInforFrame->SetMoveLinkFrameWnd(m_pPartyGetItemMenuWnd);
	
	//파티멤버창 구성화
	CompositionPartyMemberWnd();

	//하단 컨트롤 윈도우 프레임
	m_pPartyDownWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_None, WID_PartyInforWnd, 
		3, 0, 145, 21, false );
	m_pPartyDownWnd->SetClientImage( TID_PARTY_DOWN_WND );
	m_pPartyDownWnd->SetCaption( _RT("") );
	m_pPartyDownWnd->SetWndProc( this );
	m_pPartyDownWnd->SetTitle( false );
	m_pPartyDownWnd->SetVisible( false );
	m_pPartyDownWnd->SetMove( false );
	m_pPartyDownWnd->SetZorder( n_zoBottom );
	m_pPartyDownWnd->Initialize();

	m_pLeagueViewBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, 
								m_pPartyDownWnd->GetID(), 54, 2, 16, 16, false );
	m_pLeagueViewBtn->SetBtnImage( TID_PARTY_LEAGUE_BTN_NORMAL, TID_PARTY_LEAGUE_BTN_FOCUS, 
								   TID_PARTY_LEAGUE_BTN_CLICK, TID_None );
	m_pLeagueViewBtn->SetVisible(false);
	m_pLeagueViewBtn->SetHID(HD_VIEW_LMEM);

	SRect sTempRect;

	sTempRect.x	= 81;
	sTempRect.y	= 5;
	sTempRect.w	= 25;
	sTempRect.h	= 13;

#ifdef APPLY_ENGLISH_ORIGIN_SIZE
	
	sTempRect.x	= 66;
	sTempRect.w	= 40;

#endif		

	m_pPartyAddMemberBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, 
								m_pPartyDownWnd->GetID(), sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );
	m_pPartyAddMemberBtn->SetBtnImage( TID_None, TID_PARTY_MEMBER_BTN_NORMAL, 
		TID_PARTY_MEMBER_BTN_FOCUS, TID_None );
	m_pPartyAddMemberBtn->SetText( G_STRING_CHAR( IDS_ADDITION  ) );
#ifdef APPLY_ENGLISH_ORIGIN_SIZE
	m_pPartyAddMemberBtn->SetAlignText( n_atCenter, n_atCenter );
#endif
	m_pPartyAddMemberBtn->SetHID(HD_PARTY_IN);

	sTempRect.x	= 112;
	sTempRect.y	= 5;
	sTempRect.w	= 25;
	sTempRect.h	= 13;

#ifdef APPLY_ENGLISH_ORIGIN_SIZE
	
	sTempRect.x	= 107;
	sTempRect.w	= 40;

#endif

	m_pPartyBreakBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, 
								m_pPartyDownWnd->GetID(), sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );
	m_pPartyBreakBtn->SetBtnImage( TID_None, TID_PARTY_MEMBER_BTN_NORMAL, 
		TID_PARTY_MEMBER_BTN_FOCUS, TID_None );
	m_pPartyBreakBtn->SetText( G_STRING_CHAR( IDS_SECESSION ) );
#ifdef APPLY_ENGLISH_ORIGIN_SIZE
	m_pPartyBreakBtn->SetAlignText( n_atCenter, n_atCenter );
#endif	
	m_pPartyBreakBtn->SetHID(HD_PBREAKBTN);

	
	//SetPartyType(LEAGUE_PARTY);
	//SetVisible(TRUE);	
	
	SetItemMgr( n_PartyItem_Free );
}

void CPartyInforWnd::CompositionPartyMemberWnd()
{
	int nXpos = 0;
	int nYpos = 0;
	
	nYpos = 23;
		
	for( int i = 0 ; i < MAX_PARTY_MEMBERNUM ; i++ )
	{
		m_PartyMemberWnd[i].Composition( i, nXpos, nYpos );
		nYpos += 49;
	}
}

void CPartyInforWnd::Update()
{
	m_pPartyNameTxt->SetEnable(true);	
	m_pPartyGetItemFreeBtn->SetEnable(true);
	m_pPartyGetItemSequenceBtn->SetEnable(true);
    m_pPartyGetCountryBtn->SetEnable(true);	
	
	if(LEAGUE_PARTY == m_uPartyType)
	{
		m_pPartyNameTxt->SetHID(HD_LEAGUE_RENAME);
	}
	else
	{	
		m_pPartyNameTxt->SetHID(HD_PARTY_RNAME);
	}
	
	m_pPartyAddMemberBtn->SetVisible(true);
	
	m_pPartyBreakBtn->SetVisible(true);
	
	//방장이 아니면
	if(m_PartyInfo.m_dLeaderID != g_Pc.m_MainPC.lUnique)
	{
		m_pPartyNameTxt->SetEnable(false);		
		m_pPartyGetItemFreeBtn->SetEnable(false);
		m_pPartyGetItemSequenceBtn->SetEnable(false);
		m_pPartyGetCountryBtn->SetEnable(false);
		m_pPartyAddMemberBtn->SetVisible(false);
		m_pPartyNameTxt->SetHID(HID_None);
	}
	
	if( n_FightZone == g_Pc.GetPlayer()->theMapType )	//내가 배틀존에 있다면
	{
		m_pPartyBreakBtn->SetVisible(false);
	}	
	
	for(int i = 0; i < m_PartyMemberList.size() ; i++)
	{
        m_PartyMemberWnd[i].Update();
	}
	
	if( ( -1 == NonPlayerInfoWnd.thePickedUserID || MAX_USER_INTABLE == NonPlayerInfoWnd.thePickedUserID )
		|| (!FindPartyMember(g_Pc.m_PcTable[NonPlayerInfoWnd.thePickedUserID].lUnique) ) )
	{
		 for(int i = 0 ; i < g_PartyInforWnd.GetTotPartyMemNumber() ; i++)
		 {		
		    g_PartyInforWnd.m_PartyMemberWnd[i].m_pPartyMemSelectImg->SetVisible(false);	
		 }
		 g_PartyInforWnd.m_nSelectMemberIndex = -1;
	}
	
	
	//툴팁 이름표시 - 뷰 체크
	if(m_WIDToolTipDestWnd != nRui->GetFocusWnd())
	{
		SetFocusMemberWnd(WID_None, _RT(""));
	}
	
	//파티-팝업 윈도우 - 뷰 체크
	if( nRui->GetEMsg()->Mouse.Action == MA_LDOWN )
	{
		if( g_PartyPopupWnd.IsVisible && 
			!g_PartyPopupWnd.CheckFocusWnd(nRui->GetFocusWnd()) )
			nRui->SendEvent( WID_Party_PopupWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	}
}

void CPartyInforWnd::Proc( SEventMessage* EMsg )
{
	
	//상단 컨트롤 윈도우 프레임
	if( m_pGetItemViewBtn->GetID() == EMsg->FocusWnd )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{		
			m_bViewItemHowMenu = !m_bViewItemHowMenu;
			if(!m_bViewItemHowMenu)
			{
				m_pGetItemViewBtn->SetHID(HD_ITEM_VIEW);
			}
			else
			{
				m_pGetItemViewBtn->SetHID(HID_None);
			}

			nRui->SendEvent( m_pPartyGetItemMenuWnd->GetID(), n_emSetVisible, 
				(EPARAM)m_bViewItemHowMenu, 0, 0, 0 );
		}
	}

	if( m_pPartyNameTxt->GetID() == EMsg->FocusWnd )
	{
		if( EMsg->Notify == NM_BTN_RCLICK )
		{
			g_UIMsgWnd.Init();
			
			if(NORMAL_PARTY == m_uPartyType)
			{
				g_UIMsgWnd.SetMsgType( MSG_PARTYRENAME );
			}
			else
			{	
				g_UIMsgWnd.SetMsgType( MSG_LEAGUENAME );
			}
			
			if( !g_UIMsgWnd.IsVisible )
			{
				nRui->SendEvent( WID_UIMsgWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
				nRui->SendEvent( WID_UIMsgInEdit, n_emSetFocus, 0, 0, 0, 0 );		
			}
		}
	}

	if( m_pPartyMinBtn->GetID() == EMsg->FocusWnd )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			m_bIsMinimize = !m_bIsMinimize;
			SetMinimize(m_bIsMinimize);
		}
	}	
	
	//상단 윈도우 위쪽 아이템 정책프레임
	if( m_pPartyGetItemFreeBtn->GetID() == EMsg->FocusWnd )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			g_RockClient.Send_RpCsPartyItemMgr((BYTE)n_PartyItem_Free);
		}
	}

	if( m_pPartyGetItemSequenceBtn->GetID() == EMsg->FocusWnd )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			g_RockClient.Send_RpCsPartyItemMgr((BYTE)n_PartyItem_Seq);
		}
	}

	if( m_pPartyGetCountryBtn->GetID() == EMsg->FocusWnd )
	{

	}
	
	//하단 컨트롤 윈도우 프레임
	//이건 방장이 아니면 나오지않는다.
	if( m_pPartyAddMemberBtn->GetID() == EMsg->FocusWnd )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			g_UIMsgWnd.Init();
			
			if(NORMAL_PARTY == m_uPartyType)
			{
				g_UIMsgWnd.SetMsgType( MSG_PARTYIN );
			}
			else
			{	
				g_UIMsgWnd.SetMsgType( MSG_LEAGUEIN );
			}			
			
			if( !g_UIMsgWnd.IsVisible )
			{
				nRui->SendEvent( WID_UIMsgWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
				nRui->SendEvent( WID_UIMsgInEdit, n_emSetFocus, 0, 0, 0, 0 );
			}
		}
	}

	if( m_pPartyBreakBtn->GetID() == EMsg->FocusWnd )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			g_UIMsgWnd.Init();				
			g_UIMsgWnd.SetParam(nRui->GetPcParamInfo()->PCName);				
			
			if(NORMAL_PARTY == m_uPartyType)
			{
				g_UIMsgWnd.SetMsgType( MSG_PARTYOUT );
			}
			else
			{	
				g_UIMsgWnd.SetMsgType( MSG_LEAGUEOUT );
			}

			if( !g_UIMsgWnd.IsVisible )
				nRui->SendEvent( WID_UIMsgWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );	
		}		
	}

	//리그 구성멤버 버튼동작
	if( m_pLeagueViewBtn->GetID() == EMsg->FocusWnd )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			bool bIsVisible = g_LeagueInfoWnd.IsVisible;
			bIsVisible = !bIsVisible;
			if(bIsVisible)
			{
				g_LeagueInfoWnd.Open();
			}
			else
			{
				g_LeagueInfoWnd.Close();	
			}		
		}
	}
}

void CPartyInforWnd::SetItemMgr(UINT ItemMgr)
{
	m_PartyInfo.m_bItemMgr = ItemMgr;
	
	switch( ItemMgr )
	{
	   case n_PartyItem_Free:
		   {
			   m_pPartyGetItemFreeBtn->SetClientImage( TID_PARTY_GETITEM_FREENOWSEL );
			   m_pPartyGetItemSequenceBtn->SetClientImage( TID_PARTY_GETITEM_SEQUEENABLESEL );	
			   m_pPartyGetCountryBtn->SetClientImage( TID_PARTY_GETITEM_COUNTENABLESEL );	
			   
			   if(m_PartyInfo.m_dLeaderID != g_Pc.m_MainPC.lUnique)
			   {
			      m_pPartyGetItemSequenceBtn->SetClientImage( TID_PARTY_GETITEM_SEQUEDISABLESEL );	
			      m_pPartyGetCountryBtn->SetClientImage( TID_PARTY_GETITEM_COUNTDISABLESEL );
			   }
		   }
		   break;
	   case n_PartyItem_Seq:
		   {
			   m_pPartyGetItemFreeBtn->SetClientImage( TID_PARTY_GETITEM_FREEENABLESEL );
			   m_pPartyGetItemSequenceBtn->SetClientImage( TID_PARTY_GETITEM_SEQUENOWSEL );	
			   m_pPartyGetCountryBtn->SetClientImage( TID_PARTY_GETITEM_COUNTENABLESEL );
			   
			   if(m_PartyInfo.m_dLeaderID != g_Pc.m_MainPC.lUnique)
			   {
				   m_pPartyGetItemFreeBtn->SetClientImage( TID_PARTY_GETITEM_FREEDISABLESEL );
				   m_pPartyGetCountryBtn->SetClientImage( TID_PARTY_GETITEM_COUNTDISABLESEL );
			   }
		   }		   
		   break;	
	}
}

void CPartyInforWnd::SetVisible(BOOL bFlag)
{
    m_bIsMemberParty = bFlag;
	
	SRect rect;
	
	//뷰 플래그가 TRUE이면 처음부터 크게 나온다.
	if(bFlag)
	{
		nRui->SendEvent( WID_PartyInforWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
		nRui->SendEvent( m_pPartyTitleWnd->GetID(), n_emSetVisible, (EPARAM)true, 0, 0, 0 );
		SetMinimize(false);
	}
	else
	{
		m_uPartyType = NORMAL_PARTY;
		m_pLeagueViewBtn->SetVisible(false);
		
		nRui->SendEvent( WID_PartyInforWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
		nRui->SendEvent( m_pPartyTitleWnd->GetID(), n_emSetVisible, (EPARAM)false, 0, 0, 0 );

		for(int i = 0 ; i < MAX_PARTY_MEMBERNUM ; i++)
		{		
			nRui->SendEvent( m_PartyMemberWnd[i].m_pPartyMemWnd->GetID(), 
				n_emSetVisible, (EPARAM)false, 0, 0, 0 );		
		}

		nRui->SendEvent( m_pPartyDownWnd->GetID(), n_emSetVisible, (EPARAM)false, 0, 0, 0 );		
		
		//모든 다른윈도우도 안보이게 한다.
		if(g_PartyPopupWnd.IsVisible)
		{
			nRui->SendEvent( WID_Party_PopupWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
		}

		if(m_bViewItemHowMenu)
		{
			m_bViewItemHowMenu = false;
			nRui->SendEvent( m_pPartyGetItemMenuWnd->GetID(), n_emSetVisible, (EPARAM)false, 0, 0, 0 );
		}
	}
}

void CPartyInforWnd::SetMinimize(bool bFlag)
{
	for(int i = 0 ; i < MAX_PARTY_MEMBERNUM ; i++)
	{		
		nRui->SendEvent( m_PartyMemberWnd[i].m_pPartyMemWnd->GetID(), 
			n_emSetVisible, (EPARAM)false, 0, 0, 0 );		
	}	
	
	m_bIsMinimize = bFlag;	
	SRect rect;

	SRect PartyInforFrameRect;
	SRect PartyTitleWndRect;
	SRect PartyDownWndRect;

	IPOS2 PartyTitleWndRPos;
	IPOS2 PartyDownWndRPos;

	m_pPartyInforFrame->GetClientSize(&PartyInforFrameRect);
	m_pPartyTitleWnd->GetClientSize(&PartyTitleWndRect);

	m_pPartyTitleWnd->GetWndPosition(PartyTitleWndRPos);	

	PartyInforFrameRect.h = PartyTitleWndRect.h;	

	if(m_bIsMinimize)	//최소화 
	{
		m_pPartyMinBtn->SetBtnImage( TID_CTRL_BTNBOX, TID_CTRL_BTNBOX_OVR, 
								 TID_CTRL_BTNBOX_CLK, TID_None );
		m_pPartyMinBtn->SetHID(HD_PMAX);	
		
		m_pPartyInforFrame->ResetSize(&PartyInforFrameRect);
		m_pPartyTitleWnd->SetWndRepositionR(PartyTitleWndRPos[0], 0);		

		if(g_PartyPopupWnd.IsVisible)
		{
			nRui->SendEvent( WID_Party_PopupWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
		}

		nRui->SendEvent( m_pPartyDownWnd->GetID(), n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	}
	else  //최대화 	
	{
		m_pPartyMinBtn->SetBtnImage( TID_CTRL_BTNMIN, TID_CTRL_BTNMIN_OVR, 
								 TID_CTRL_BTNMIN_CLK, TID_None );
		m_pPartyMinBtn->SetHID(HD_PMIN);
		
		SRect PartyMemWndRect;		
		
		for(int i = 0 ; i < m_PartyMemberList.size() ; i++)
		{
			m_PartyMemberWnd[i].m_pPartyMemWnd->GetClientSize(&PartyMemWndRect);
			PartyInforFrameRect.h += PartyMemWndRect.h;
		}
		
		m_pPartyDownWnd->GetClientSize(&PartyDownWndRect);
		PartyInforFrameRect.h += PartyDownWndRect.h;

		m_pPartyDownWnd->GetWndPosition(PartyDownWndRPos);		
		
		m_pPartyInforFrame->ResetSize(&PartyInforFrameRect);
		m_pPartyTitleWnd->SetWndRepositionR(PartyTitleWndRPos[0], 0);

		int nXpos = 0;
		int nYpos = 0;		
		nYpos = 23;
	
		for(int i = 0 ; i < m_PartyMemberList.size() ; i++)
		{
			m_PartyMemberWnd[i].m_pPartyMemWnd->SetWndRepositionR(nXpos, nYpos);
			nRui->SendEvent( m_PartyMemberWnd[i].m_pPartyMemWnd->GetID(), 
				n_emSetVisible, (EPARAM)true, 0, 0, 0 );
			nYpos += 49; 
		}

		m_pPartyDownWnd->SetWndRepositionR(PartyDownWndRPos[0], nYpos);
		nRui->SendEvent( m_pPartyDownWnd->GetID(), n_emSetVisible, (EPARAM)true, 0, 0, 0 );
	}
}


void CPartyInforWnd::ClearAllInfo()
{
	if(m_bIsMemberParty)
	{ 
		ClearPartyInfo();		
		ClearAllPartyMemberList();
		
		UpdatePartyMemberWnd();
	}
}

void CPartyInforWnd::ClearAllPartyMemberList()
{
	list< SPARTYMEMBER_INFO * >::iterator i = m_PartyMemberList.begin();
	
	for( ; i != m_PartyMemberList.end() ; ++i )
	{
		SPARTYMEMBER_INFO *pMemberInfo = (*i);
		
		if( pMemberInfo->m_bIsSee )
		{
			DeletePCharImage( pMemberInfo->m_dPCID );
		}
		
		SAFE_DELETE(pMemberInfo);
	}
	
	m_PartyMemberList.clear();
}

void CPartyInforWnd::ClearPartyInfo()
{
    memset(&m_PartyInfo, 0, sizeof(SPARTY_INFO));
	
	m_bIsMemberParty = FALSE;	
	SetFocusMemberWnd(WID_None, _RT(""));
}

void CPartyInforWnd::SetFocusMemberWnd( UINT WIDToolTipDestWnd, void *pstrToolTipArea )
{
	if( m_WIDToolTipDestWnd != WIDToolTipDestWnd )  
	{
		CHAR* strToolTipArea = (CHAR*)pstrToolTipArea;

		m_WIDToolTipDestWnd = WIDToolTipDestWnd;
		Rstrcpy(RWCHAR(m_strToolTipArea) , RWCHAR(strToolTipArea) );
	}
}

void CPartyInforWnd::RenderToolTipArea()
{
	if( m_WIDToolTipDestWnd != WID_None )
	{
    	CWnd *pFDestWnd = Container->Get( m_WIDToolTipDestWnd );
		SRect WcSize;
		pFDestWnd->GetClientSize( &WcSize );
		
		WcSize.x += 1;
		WcSize.y -= 1;
		
		if( pFDestWnd->GetWindowType() == n_wtChaImgButton )
		{
			WcSize.x += 1;
			WcSize.y += 1;
		}	
		
		nRui->GetHelpTip()->SetUpPosString(&WcSize, m_strToolTipArea, D3DCOLOR_XRGB(255, 255, 255 ));
		nRui->GetHelpTip()->RenderProc(1.0f, D3DCOLOR_XRGB(252, 230, 170) );  
	}	
}

SPARTYMEMBER_INFO * CPartyInforWnd::InsertPartyMember( UINT dPCID )
{
	list< SPARTYMEMBER_INFO * >::iterator i;
	
	if( m_PartyMemberList.size() < MAX_PARTY_MEMBERNUM )
	{
		
		SPARTYMEMBER_INFO * psPartymemInfo = SAFE_NEW( SPARTYMEMBER_INFO );		
		memset(psPartymemInfo, 0, sizeof(SPARTYMEMBER_INFO));
		psPartymemInfo->m_bWasMember = TRUE;
		
		psPartymemInfo->m_dPCID = dPCID;		 
		i = m_PartyMemberList.insert(m_PartyMemberList.end(), psPartymemInfo);		
		
		UpdatePartyMemberWnd();
		SetMinimize(m_bIsMinimize);
		
		return (*i);
	}	
	
	return NULL;
}

BOOL CPartyInforWnd::DeletePartyMember( UINT dPCID )
{
	list< SPARTYMEMBER_INFO * >::iterator i = m_PartyMemberList.begin();
	
	for( ; i != m_PartyMemberList.end() ; ++i )
	{
		SPARTYMEMBER_INFO *pMemberInfo = (*i);
		if( pMemberInfo->m_dPCID == dPCID )
		{
			if(pMemberInfo->m_bIsSee)
			{
				DeletePCharImage(pMemberInfo->m_dPCID);
			}
			
			SAFE_DELETE(pMemberInfo);
			(*i) = NULL;

			m_PartyMemberList.erase( i );
			
			UpdatePartyMemberWnd();
			SetMinimize(m_bIsMinimize);
			
			return TRUE;
		}
	}
	
	return FALSE;
}

void CPartyInforWnd::UpdatePartyMemberWnd()
{
	list< SPARTYMEMBER_INFO * >::iterator i = m_PartyMemberList.begin();
	int j = 0;	
	
	for( ; ( (i!=m_PartyMemberList.end())&&(j< MAX_PARTY_MEMBERNUM) ) ; i++ )
	{
		m_PartyMemberWnd[j].SetPartyMemberInfo(*i);		
		j++;
	}
	for( ; j < MAX_PARTY_MEMBERNUM; j++ )
	{		
		m_PartyMemberWnd[j].SetPartyMemberInfo(NULL);	
	}
}

SPARTYMEMBER_INFO * CPartyInforWnd::FindPartyMember( UINT dPCID )
{
	list< SPARTYMEMBER_INFO * >::iterator i = m_PartyMemberList.begin();	
	
	for( ; i != m_PartyMemberList.end() ; ++i )
	{
		SPARTYMEMBER_INFO *pMemberInfo = (*i);
		if( pMemberInfo->m_dPCID == dPCID )
		{
			return pMemberInfo;
		}
	}
	
	return NULL;
}

SPARTYMEMBER_INFO * CPartyInforWnd::FindPartyMember( char * strPCName )
{
	list< SPARTYMEMBER_INFO * >::iterator i = m_PartyMemberList.begin();	
	
	for( ; i != m_PartyMemberList.end() ; ++i )
	{
		SPARTYMEMBER_INFO *pMemberInfo = (*i);
		
		if( Rstrcmp(pMemberInfo->m_strPCName, strPCName) == 0 )
		{
			return pMemberInfo;
		}
	}
	
	return NULL;
}

// 캐릭터 얼굴 생성
BOOL CPartyInforWnd::MakePCharImage( UINT dPCID )
{
	
	bool bflag = g_Pc_Manager.m_CharImage.Create( dPCID ,  c_PcRace_User , g_lpDevice );
	//생성이 안됐다면 다시한번 호출 .. 
	if( bflag == false )
	{
		g_Pc_Manager.m_CharImage.Create( dPCID ,  c_PcRace_User , g_lpDevice );
	}


	if( g_Pc_Manager.m_CharImage.Find(dPCID, c_PcRace_User) )
		return TRUE;
	
	return FALSE;
}

void CPartyInforWnd::DeletePCharImage( UINT dPCID )
{
	g_Pc_Manager.m_CharImage.Delete( dPCID , 
		c_PcRace_User);
}

void CPartyInforWnd::ChangePCharImage( UINT dPCID )
{
	g_Pc_Manager.m_CharImage.Change( dPCID , 
		c_PcRace_User , g_lpDevice );

}

void CPartyInforWnd::UpdatePartyWnd()
{
	m_pPartyNameTxt->SetText( m_PartyInfo.m_strPartyName );
	SetItemMgr( m_PartyInfo.m_bItemMgr );

	UpdatePartyMemberWnd();
}

list< SPARTYMEMBER_INFO * >::iterator CPartyInforWnd::GetBeginMember()
{
	return m_PartyMemberList.begin();
}

list< SPARTYMEMBER_INFO * >::iterator CPartyInforWnd::GetEndMember()
{
	return m_PartyMemberList.end();	
}

void CPartyInforWnd::GiveGagePartyMember(int index)
{
	SPARTYMEMBER_INFO *pMemberInfo = m_PartyMemberWnd[index].GetPartyMemberInfo();
	
	if( pMemberInfo == NULL )
	{
		return;
	}
	
	GiveGagePartyMember(pMemberInfo);
}

void CPartyInforWnd::SelectPartyMember(int index)
{
	SPARTYMEMBER_INFO *pMemberInfo = m_PartyMemberWnd[index].GetPartyMemberInfo();
	
	if( pMemberInfo == NULL )
	{
		return;
	}

	static SEventMessage Message;
	
	Message.FocusWnd = m_PartyMemberWnd[index].m_pPartyMemWnd->GetID();
	Message.Notify = NM_FWND_CLICK;

	m_PartyMemberWnd[index].Proc( &Message );

}

void CPartyInforWnd::SelectPartyMemberWnd(int nTableindex)
{
	if( -1 == nTableindex )
	{
		return;
	}
	
	if( FindPartyMember(g_Pc.m_PcTable[nTableindex].lUnique ) ) 
	{
		for(int i = 0 ; i < g_PartyInforWnd.GetTotPartyMemNumber() ; i++)
		{	
			g_PartyInforWnd.m_PartyMemberWnd[i].m_pPartyMemSelectImg->SetVisible(false);	
		    SPARTYMEMBER_INFO *pMemberInfo  = g_PartyInforWnd.m_PartyMemberWnd[i].GetPartyMemberInfo();
		    if(pMemberInfo)
			{
				if( pMemberInfo->m_dPCID == g_Pc.m_PcTable[nTableindex].lUnique )
				{
					g_PartyInforWnd.m_PartyMemberWnd[i].m_pPartyMemSelectImg->SetVisible(true);					
				}
			}
		}	
	}
}

void CPartyInforWnd::GiveGagePartyMember(SPARTYMEMBER_INFO *pMemberInfo)
{
	if(pMemberInfo == NULL)
		return;
	
	BYTE GageType = nRui->thePcParam.GageType;
	TCHAR tstrTemp[256] = {0,};
	memset(tstrTemp , NULL , MAX_SIZE_256);
	
	int iGage = 0;
	
	switch(GageType)
	{
	case n_ComboGage: 
		{
			iGage = nRui->thePcParam.Stat1.theGage;
			Rstrcpy( RWCHAR(tstrTemp), G_STRING_CHAR( IDS_COMBO ) );
		}
		break;
	case n_ConditionGage:
		{
			iGage = nRui->thePcParam.Stat1.theGage;
			Rstrcpy( RWCHAR(tstrTemp), G_STRING_CHAR( IDS_CONDITION ) );
		}
		break;
	case n_ProimaGage:
		{
			iGage = nRui->thePcParam.Stat1.theGage;
			Rstrcpy( RWCHAR(tstrTemp), G_STRING_CHAR( IDS_PROIMA ));
		}
		break;
	}
	
	//현재 게이지 전송을 사용할수 없을 경우
	/*if( nRui->thePcParam.GetMaxGage() == 0 )
	{
		SystemPrint( n_LightYellow, n_SystemMsg, "!현재 게이지전송을 사용할 수 없습니다.", tstrTemp);
		return;
	}*/   
	
	if( pMemberInfo->m_bIsLogOut )
	{
		SystemPrint( n_LightYellow, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_PARTY_LOGOUT ),
			pMemberInfo->m_strPCName );
		return;
	}
	
	if( !(pMemberInfo->m_bIsViewRange) )
	{
		SystemPrint( n_LightYellow, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_PT_OUTOFVIEW ),
			pMemberInfo->m_strPCName );
		return;
	}
	
	if( iGage < 5 )
	{
		SystemPrint( n_LightYellow, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_SHORTAGE_GAUGE ), tstrTemp);
		return;
	}
	
	//전송대상이 게이지를 사용할수 없을경우
	/*if( pMemberInfo->m_bMaxGage == 0 )
	{
		SystemPrint( n_LightYellow, n_SystemMsg, "!게이지를 사용할수 없는 대상입니다.", tstrTemp);
		return;
	}*/
	
	//풀게이지 전송이 불가능 할경우
	/*if( (pMemberInfo->m_bMaxGage - pMemberInfo->m_bGage) < 5 )
	{
	SystemPrint( n_LightYellow, n_SystemMsg, "!대상에 풀게이지 전송이 불가능합니다.", tstrTemp);
	return;	
}*/	
	
	g_RockClient.Send_RpCsPartyGiveGage(pMemberInfo->m_dPCID, (BYTE)5);
	
}

void CPartyInforWnd::LogOff()
{
	ClearAllInfo();
	SetVisible( FALSE );
}

int  CPartyInforWnd::GetTotPartyMemNumber()
{
	return m_PartyMemberList.size();
}

void CPartyInforWnd::RestorePMemberCharImg()
{
	if(m_bIsMemberParty)
	{
		list< SPARTYMEMBER_INFO * >::iterator i = m_PartyMemberList.begin();
		
		for( ; i != m_PartyMemberList.end() ; ++i )
		{
			SPARTYMEMBER_INFO *pMemberInfo = (*i);
			if(pMemberInfo->m_dPCID == g_Pc.m_MainPC.lUnique)
				continue;
			
			pMemberInfo->m_bIsViewRange = FALSE;
			pMemberInfo->m_bIsSee = FALSE;
			
			long lunique = pMemberInfo->m_dPCID;
			PCTABLE *pPcTable = g_Pc.FindPcTable(lunique);
			
			if(pPcTable)
			{
				pMemberInfo->m_bIsViewRange = TRUE;
				if(MakePCharImage(pMemberInfo->m_dPCID))
				{
					pMemberInfo->m_bIsSee = TRUE;
				}				
			}		
		}
	}
}

void CPartyInforWnd::ReFreshPMemberCharImg()
{
	if(m_bIsMemberParty)
	{
		list< SPARTYMEMBER_INFO * >::iterator i = m_PartyMemberList.begin();
		
		for( ; i != m_PartyMemberList.end() ; ++i )
		{
			SPARTYMEMBER_INFO *pMemberInfo = (*i);
			if(pMemberInfo->m_dPCID == g_Pc.m_MainPC.lUnique)
				continue;
			
			long lunique = pMemberInfo->m_dPCID;
			PCTABLE *pPcTable = g_Pc.FindPcTable(lunique);
			
			if(pPcTable)
			{
				ChangePCharImage(pMemberInfo->m_dPCID);						
			}		
		}
	}
}