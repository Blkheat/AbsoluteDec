#include "Rui.h"
#include "Define.h"
#include "Util.h"
#include "Render.h"
#include "Fontman.h"
#include "Resource.h"
#include "Chain.h"
#include "Container.h"
#include "..\\..\\stringmanager.h"

#include "..\\..\\brother.h"
#include "..\\..\\global.h"
#include "..\\..\\RockClient.h"

#include "..\\..\\quadlist.h"
#include "..\\..\\bravolibs\\network\\Protocol.h"
#include "..\\..\\Pc.h"
#include "PlayerInfoWnd.h"

#include "CWebShopWnd.h"

CWebShopWnd	g_WebShopWnd;

CWebShopWnd::CWebShopWnd()
{
	m_pFrame    = NULL;
	m_pCloseBtn = NULL;
	m_iWebMode =  0; 

	m_bFlag = false;
}

CWebShopWnd::~CWebShopWnd()
{
}

void CWebShopWnd::Init()
{
	
}

void CWebShopWnd::SetWebMode(int mode)
{
	m_iWebMode = mode ; 
}


void CWebShopWnd::Composition()
{
	int center_x	= nRui->Desktop.Width   / 2.0f;
	int center_y	= nRui->Desktop.Height  / 2.0f;

	m_WndRect.w = 900 + 4;
	m_WndRect.h = 600 + 4 + 27;
	m_WndRect.x = center_x - ( m_WndRect.w / 2.0f );
	m_WndRect.y = center_y - ( m_WndRect.h / 2.0f );

	//...........................................................................................................
	// 메인 프레임
	//...........................................................................................................
	m_pCullFrame = (CFrameWnd *)nRui->CreateWnd( n_wtFrameWnd, WID_None , WID_None , 0, 0 ,
		nRui->Desktop.Width , nRui->Desktop.Height , false );

	m_pCullFrame->SetClientImage( TID_None );
	m_pCullFrame->SetMove( FALSE );
	m_pCullFrame->SetZorder( n_zoTop );

	m_pCullFrame->SetCaption( _RT("") );
	m_pCullFrame->SetWndProc( this );
	m_pCullFrame->SetTitle( false );
	m_pCullFrame->SetVisible( false );
	m_pCullFrame->Initialize();

	m_pFrame = (CFrameWnd *)nRui->CreateWnd( n_wtFrameWnd, WID_None , m_pCullFrame->GetID() , m_WndRect.x, m_WndRect.y, m_WndRect.w, m_WndRect.h, false );
	m_pFrame->SetWndTexStyle( fwfTex_Style_Tile9 );
	m_pFrame->SetClientImage( TID_CTRL_WND_L1 );
	m_pFrame->SetIconTID( TID_CTRL_ICON_NONE );	
	m_pFrame->SetWndTileSizeLR( 40, 29 );
	m_pFrame->SetWndTileSizeTB( 31, 11 );
	m_pFrame->SetCaption( G_STRING_CHAR( IDS_ITEMMALL ) );	 
	m_pFrame->SetTitle( true );
	m_pFrame->SetMove( FALSE );
	m_pFrame->SetZorder( n_zoTop );

	m_pFrame->SetWndProc( this );	
	m_pFrame->SetVisible( false );
	m_pFrame->Initialize();
	
	m_pCloseBtn = (CButton*)nRui->CreateWnd( n_wtButton , WID_None , m_pFrame->GetID(), 880, 6, 13, 13, false );   
	m_pCloseBtn->SetBtnImage( TID_CTRL_BTN_X, TID_CTRL_BTN_X_OVR, TID_CTRL_BTN_X_CLK, TID_None );
	
	//중국 때문에 들어간다... 완전 새로운 구역때문에 
	m_iWebBuildMode = g_RockClient.GetRockProfileInt("Updater","ServerNew", 0 );

	

}

void CWebShopWnd::LogOff()
{
	m_WebDialog.InitWebDialog();
}


void CWebShopWnd::Update()
{

}

void CWebShopWnd::Proc( SEventMessage* EMsg )
{
	if( EMsg->FocusWnd == m_pCloseBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			Close();	
		}
	}
}

int CWebShopWnd::OpenURL(wchar_t *addr, TCHAR *post)
{

	RTCHAR rtAddress[1024] = { 0, };

#ifdef C_HACK_SHIELD
	g_RockClient.HS_PauseService();
#endif

	if (!nRui->m_IsSurpportHCursor)
	{
		ShowCursor(TRUE);
	}
	else
	{
		if (!m_bFlag)
		{
			nRui->m_IsSurpportHCursor = FALSE;
			ShowCursor(FALSE);
			m_bFlag = true;
		}
	}

	nRui->SendEvent(m_pCullFrame->GetID(), n_emSetVisible, (EPARAM)true, 0, 0, 0);
	nRui->SendEvent(m_pFrame->GetID(), n_emSetVisible, (EPARAM)true, 0, 0, 0);

	int AddY;

	AddY = 27;

	if (m_WebDialog.SetDialog(m_WndRect.x, m_WndRect.y + AddY, m_WndRect.w, m_WndRect.h - AddY,
		g_RockClient.m_hInstance, g_RockClient.m_hWnd))
	{
		m_rcTop.left = 0;
		m_rcTop.top = 0;
		m_rcTop.right = nRui->Desktop.Width;
		m_rcTop.bottom = m_WebDialog.m_Rect.top;

		m_rcBottom.left = 0;
		m_rcBottom.top = m_WebDialog.m_Rect.bottom;
		m_rcBottom.right = nRui->Desktop.Width;
		m_rcBottom.bottom = nRui->Desktop.Height;

		m_rcLeft.left = 0;
		m_rcLeft.top = m_WebDialog.m_Rect.top;
		m_rcLeft.right = m_WebDialog.m_Rect.left;
		m_rcLeft.bottom = m_WebDialog.m_Rect.bottom;

		m_rcRight.left = m_WebDialog.m_Rect.right;
		m_rcRight.top = m_WebDialog.m_Rect.top;
		m_rcRight.right = nRui->Desktop.Width;
		m_rcRight.bottom = m_WebDialog.m_Rect.bottom;

		char TempStr[1024] = { 0, };

		wsprintf(TempStr, "id=%s&check=%s", g_RockClient.m_UserName, m_md5);

		

		//m_WebDialog.SetURL(const_cast< wchar_t* >(test.c_str()),"");
		m_WebDialog.SetURL(addr, TempStr);
		m_WebDialog.Show();

		g_RockClient.m_SceneManager.SetKeyInput(FALSE);

	}
	else
	{
		Close();
	}

	return TRUE;

}

int CWebShopWnd::Close(void)
{

#ifdef C_HACK_SHIELD
	g_RockClient.HS_ResumeService();
#endif

	if(!nRui->m_IsSurpportHCursor)
	{
		if(m_bFlag)
		{
			nRui->m_IsSurpportHCursor = TRUE;
			ShowCursor(TRUE);
			m_bFlag = false;
			nRui->SetCursorType(n_CT_NORMAL);
		}
		else
		{
			ShowCursor(FALSE);
		}
	}
	
	nRui->SendEvent( m_pFrame->GetID() , n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	nRui->SendEvent( m_pCullFrame->GetID() , n_emSetVisible, (EPARAM)false, 0, 0, 0 );

	m_WebDialog.Hide();
	m_WebDialog.Destroy();
	
	g_RockClient.m_SceneManager.SetKeyInput( TRUE );
	
	::SetFocus(g_RockClient.GetApp());

	return TRUE;
}

void CWebShopWnd::Present(void)
{
	g_RenderManager.Present(&m_rcTop    , &m_rcTop    , g_RockClient.m_hWnd, NULL);
	g_RenderManager.Present(&m_rcBottom , &m_rcBottom , g_RockClient.m_hWnd, NULL);
	g_RenderManager.Present(&m_rcLeft   , &m_rcLeft   , g_RockClient.m_hWnd, NULL);	  
	g_RenderManager.Present(&m_rcRight  , &m_rcRight  , g_RockClient.m_hWnd, NULL);		
}

