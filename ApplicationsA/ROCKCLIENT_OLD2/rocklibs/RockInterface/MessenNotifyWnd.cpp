#include "..\\..\\RockClient.h"
#include "..\\..\\brother.h"
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

#include "..\\..\\GTimer.h"
#include "..\\..\\global.h"

#include "..\\..\\quadlist.h"
#include "..\\..\\bravolibs\\network\\Protocol.h"
#include "..\\..\\Pc.h"

#include "MCommunityWnd.h"
#include "MessenNotifyWnd.h"

CMessenNotifyWnd::CMessenNotifyWnd()
{
	m_nIndex = -1;
	
	m_pFrameWnd = NULL;
	m_pNotifyFlashImg = NULL;
	m_pCloseBtn = NULL;
	m_pDestNameTxt = NULL;
	m_pMouseOverBtn = NULL;
	m_FocusOriginTxt = NULL;

	m_uFrameID = WID_None;
	m_uFlashImgID = WID_None;
	m_uCloseBtnID = WID_None;
	m_uDestNameTxtID = WID_None;
    m_uMouseOverBtnID = WID_None;
	m_uFocusOriginTxtID = WID_None;
	
	memset(m_strTemp , NULL, MAX_SIZE_512);
	m_WndState = NState_None;

	m_SAlpha = 255;
	m_EAlPha = 255;

	m_fUpActionTime = 0.5f;
	m_fDownActionTime = 0.5f;
	m_fCumulativeTime = 0.0f;
}

CMessenNotifyWnd::~CMessenNotifyWnd()
{
	

}

void CMessenNotifyWnd::Init()
{

	
}

void CMessenNotifyWnd::SetInterDestName(void * pstrName)
{
	char*StrName = RCHAR(pstrName);

	Rstrcpy( RWCHAR(m_strInterDestName), RWCHAR(StrName) );
	m_pDestNameTxt->SetText(m_strInterDestName);
	m_pDestNameTxt->SetLimitLength(RP_MAX_PC_NAME-1);	
}

TCHAR * CMessenNotifyWnd::GetInterDestName()
{
	return m_strInterDestName;
}

void CMessenNotifyWnd::Composition()
{
	

}

void CMessenNotifyWnd::Composition(int nIndex, int nXpos, int nYpos)
{
	m_nIndex = nIndex;
	
	m_pFrameWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_None, WID_None, 
		nXpos, nYpos, 153, 31, false );
	m_pFrameWnd->SetClientImage( TID_MessenNotifyBgUI );
	m_pFrameWnd->SetCaption( _RT("MessenNotify Window") );
	m_pFrameWnd->SetWndProc( this );
	m_pFrameWnd->SetTitle( false );
	m_pFrameWnd->SetMove(false);
	m_pFrameWnd->SetZorder( n_zoTop );
	m_pFrameWnd->SetVisible( false );
	m_pFrameWnd->Initialize();
	m_uFrameID = m_pFrameWnd->GetID();	

	m_pNotifyFlashImg = (CFlashWnd*)nRui->CreateWnd( n_wtFlashWnd, WID_None, m_uFrameID, 
		4, 4, 9, 9, false );
	m_pNotifyFlashImg->SetClientImage(TID_MNotifyFlashImg);
	m_pNotifyFlashImg->SetActiveFlash(true);
	m_uFlashImgID = m_pNotifyFlashImg->GetID();	

	m_pCloseBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, m_uFrameID, 
	                                132, 9, 13, 13, false );
    m_pCloseBtn->SetClientImage( TID_None );
    m_pCloseBtn->SetBtnImage( TID_None, TID_MNotifyCloseBtnFocus, 
							   TID_MNotifyCloseBtnClick, TID_None );
	m_uCloseBtnID = m_pCloseBtn->GetID();

	m_pDestNameTxt = (CLimitLenTextBox*)nRui->CreateWnd( n_wtLimitLenTextBox, WID_None, m_uFrameID, 
		26, 6, 101, 19, false );
	m_pDestNameTxt->SetClientImage(TID_None);
	m_pDestNameTxt->SetAlignText( n_atLeft, n_atCenter );
	m_pDestNameTxt->SetBlank( 2, 0, 0, 0 );		
	m_pDestNameTxt->SetText(_RT(""));
	m_pDestNameTxt->SetLimitLength(RP_MAX_PC_NAME-1);
	m_pDestNameTxt->SetTextColor(255, 255, 255);
	m_pDestNameTxt->SetBtnImage( TID_None, TID_MNotifyMouseOver, 
							   TID_MNotifyMouseClick, TID_None );
	m_uDestNameTxtID = m_pDestNameTxt->GetID(); 

	
	m_FocusOriginTxt = (CFrameTextBox*)nRui->CreateWnd( n_wtFrameTextBox, WID_None, m_uFrameID, 
	0, 0, 0, 0, false );	
	m_FocusOriginTxt->SetAlignText( n_atLeft, n_atCenter);
	m_FocusOriginTxt->SetBlank( 5, 0, 0, 0 );
	m_FocusOriginTxt->SetText(_RT(""));			
	m_FocusOriginTxt->SetClientImage(TID_CLIENT);
	m_FocusOriginTxt->SetClientColor(55, 55, 74);
	m_FocusOriginTxt->SetFrameColor( D3DCOLOR_ARGB( 255, 241, 226, 182 ) );
	m_FocusOriginTxt->SetVisible(false);
	m_uFocusOriginTxtID = m_FocusOriginTxt->GetID();

}

void CMessenNotifyWnd::Update()
{
	m_FocusOriginTxt->SetVisible(false);

	POINT	ptCursor;
	GetCursorPos(&ptCursor);
	::ScreenToClient(g_RockClient.GetApp(), &ptCursor);

	memset(m_strTemp, NULL , MAX_SIZE_512);

	SRect temRect;
	SStringInfo *pStringInfo = NULL;

	if( m_pDestNameTxt->FocusCheck(ptCursor.x, ptCursor.y, false) )
	{
		/*if(!m_pDestNameTxt->IsEqualString())
		{
			m_pDestNameTxt->GetClientSize(&temRect);
			pStringInfo = m_pDestNameTxt->GetStringInfo();
			temRect.x = (temRect.x) - (pStringInfo->Width + 20);			
			temRect.w = pStringInfo->Width + 10;			
			
			m_FocusOriginTxt->ResetSize(&temRect);				
			
			m_pDestNameTxt->GetText(m_strTemp, 512);
			m_FocusOriginTxt->SetText(m_strTemp);
			m_FocusOriginTxt->SetVisible(true);					
		}*/
	}

	if( m_WndState != NState_None )
	{
		MoveAction();
	}
}

void CMessenNotifyWnd::Proc( SEventMessage* EMsg )
{
	CMessenOneChatWnd *pMessenOneChatWnd = NULL;
	
	if( EMsg->FocusWnd == m_uDestNameTxtID )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			//연관된 채팅창을 열어준다.
			pMessenOneChatWnd = g_MCommunityWnd.FindMessenOneChatWnd(m_strInterDestName);
			if(pMessenOneChatWnd == NULL)
			{
				return;
			}
			CFrameWnd * pFrameWnd = pMessenOneChatWnd->GetFrameWnd();
			POINT pos;
			g_MCommunityWnd.GetMessenOneChatWndPos(pos);
			pFrameWnd->SetAbsolutePosition(pos.x, pos.y);
			pMessenOneChatWnd->SetVisible(true);					
			AfterActionThisWndClick();
		}
	}

	if( EMsg->FocusWnd == m_uCloseBtnID )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			//연관된 채팅창을 초기화한다.
			pMessenOneChatWnd = g_MCommunityWnd.FindMessenOneChatWnd(m_strInterDestName);
			if(pMessenOneChatWnd == NULL)
			{
				return;
			}
			pMessenOneChatWnd->Init();			
			AfterActionThisWndClick();
		}
	}
}

CFlashWnd * CMessenNotifyWnd::GetNotifyFlashImg()
{
	return m_pNotifyFlashImg;
}

CFrameWnd * CMessenNotifyWnd::GetFrameWnd()
{
	return m_pFrameWnd; 
}

UINT CMessenNotifyWnd::GetFrameID()
{
	return m_uFrameID;
}

void CMessenNotifyWnd::MoveAction(UINT uState, POINT sPoint, POINT ePoint)
{
	m_fCumulativeTime = 0.0f;
	m_WndState = uState;
	
	if(m_WndState == NState_UpAction)
	{
		m_SAlpha = 128;
		m_EAlPha = 255;
	}

	if(m_WndState == NState_DownAction)
	{
		m_SAlpha = 255;
		m_EAlPha = 255;
	}		

	m_Spoint = sPoint;
	m_Epoint = ePoint;
}

void CMessenNotifyWnd::MoveAction()
{
	float fElapsedtime = g_GTimer.GetFrameElapsedSecTime();	
	m_fCumulativeTime += fElapsedtime;

	float fIntevalTime = 0.0f;
	if(m_WndState == NState_UpAction)
	{
		fIntevalTime = m_fUpActionTime;
	}

	if(m_WndState == NState_DownAction)
	{
		fIntevalTime = m_fUpActionTime;
	}

	float fRatio = m_fCumulativeTime / fIntevalTime;
	if( m_fCumulativeTime > fIntevalTime )
	{
		m_WndState = NState_None;
		m_fCumulativeTime = 0.0f;
		fRatio = 1.0f;	
	}

	POINT pos;

	pos.x = m_Spoint.x  + ( fRatio * (m_Epoint.x - m_Spoint.x) );
	pos.y = m_Spoint.y  + ( fRatio * (m_Epoint.y - m_Spoint.y) );

	int nAlpah = m_SAlpha + ( fRatio * (m_EAlPha - m_SAlpha ) );

	m_pFrameWnd->SetAbsolutePosition(pos.x, pos.y);
	m_pFrameWnd->SetClientAlpah(nAlpah);
	m_pNotifyFlashImg->SetClientAlpah(nAlpah);
	m_pCloseBtn->SetClientAlpah(nAlpah);
	m_pDestNameTxt->SetClientAlpah(nAlpah);
	m_pDestNameTxt->SetTextAlpha(nAlpah);
}

void CMessenNotifyWnd::AfterActionThisWndClick()
{
	int nViewNotifyWndNum = g_MCommunityWnd.GetViewNotifyWndNum();
	int nDestNameNum  = g_MCommunityWnd.GetDestNameNum();
	
	SAFE_DELETE_ARRAY(g_MCommunityWnd.m_MessenNotifyDestName[m_nIndex]);

	TCHAR **pDestNameList = g_MCommunityWnd.m_MessenNotifyDestName;
	CMessenNotifyWnd *pNotifyWndList = g_MCommunityWnd.m_MessenNotifyWnd;
	POINT *pWndPosList = g_MCommunityWnd.m_MessenNotifyWndPos;

	for(int i = m_nIndex ; i < (nDestNameNum - 1) ; i++)
	{
		pDestNameList[i] = pDestNameList[i + 1];
	}
	
	for(int i = m_nIndex ; i < (nViewNotifyWndNum - 1) ; i++)
	{
		pNotifyWndList[i].SetInterDestName(pDestNameList[i]);
		pNotifyWndList[i].MoveAction(NState_DownAction, pWndPosList[i+2], pWndPosList[i+1]);
	}

	pNotifyWndList[(nViewNotifyWndNum - 1)].SetInterDestName(_T(""));
	nRui->SendEvent( pNotifyWndList[(nViewNotifyWndNum - 1)].GetFrameID(), n_emSetVisible, 
			(EPARAM)false, 0, 0, 0 );	
	
	nViewNotifyWndNum--;
	nDestNameNum--;
	g_MCommunityWnd.SetViewNotifyWndNum(nViewNotifyWndNum);	
	g_MCommunityWnd.SetDestNameNum(nDestNameNum);

	if( nDestNameNum >= MAX_MESSEN_NOTIFYWND_NUM )
	{
		pNotifyWndList[nViewNotifyWndNum].MoveAction(NState_UpAction, 
		pWndPosList[nViewNotifyWndNum], pWndPosList[nViewNotifyWndNum + 1]);
		pNotifyWndList[nViewNotifyWndNum].SetInterDestName(pDestNameList[nViewNotifyWndNum]);
		nRui->SendEvent( pNotifyWndList[nViewNotifyWndNum].GetFrameID(), n_emSetVisible, 
			(EPARAM)true, 0, 0, 0 );
		nViewNotifyWndNum++;
		g_MCommunityWnd.SetViewNotifyWndNum(nViewNotifyWndNum);
	}	
}
