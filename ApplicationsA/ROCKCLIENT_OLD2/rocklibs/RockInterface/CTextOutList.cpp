#include "..\\..\\RockClient.h"
#include "CTextOutList.h"

#include "..\\..\\bravolibs\\Sound\\Sound.h"
#include "SystemPrintWnd.h"
#include "ChatMainProc.h"

#include "..\\..\\CRenderManager.h"

#ifdef DIRECT_VERSION_9_MJH
extern IDirect3DDevice9*	g_lpDevice;
#else
extern IDirect3DDevice8*	g_lpDevice;
#endif // DIRECT_VERSION_9_MJH

//------------------------------------------------------------------------------
// CChatOutList  채팅관련 출력 컴포넌트
//------------------------------------------------------------------------------

CChatOutList::CChatOutList( SDesktop* DT ) : CWnd(DT)
{
	m_pRITextBox = SAFE_NEW( CRITextBox(DT) );	
	m_pTextBox = SAFE_NEW( CTextBox(DT) );	
	m_pTextBox2 = SAFE_NEW( CTextBox(DT) );	
	
	m_pLinkScrollBarBg = NULL;
	m_pLinkScrollbar = NULL;

	m_nLHeight = 16;
	
	m_IsFocus = false;
	
	m_uChatOutActState = COutState_None;
	m_uFilterState = COutTotalView;

	m_pTempFocusChatData = NULL;
	
	m_ppRefChatTxtData = NULL;
	memset( m_pViewChatTxtData, 0, sizeof(SChatTxtData*) * (MAX_CHATDATA_NUM + 1) );	

	m_nOutPutLine = 0;
	m_nLineCount = 0;

	m_nDelValue = 0;	
}

CChatOutList::~CChatOutList()
{
	SAFE_DELETE(m_pRITextBox);
	SAFE_DELETE(m_pTextBox);
	SAFE_DELETE(m_pTextBox2);
}

void CChatOutList::SetLinkScroll(CScrollBar* pLinkScrollbar, CWnd * pLinkScrollBarBg)
{
	m_pLinkScrollBarBg = pLinkScrollBarBg;
	m_pLinkScrollbar = pLinkScrollbar;
}

void CChatOutList::GetStringInfo( TCHAR *strTemp , SStringInfo * StrInfo )
{
	if(m_pRITextBox)
	{
		m_pRITextBox->GetFong()->GetRIStringInfo( strTemp , StrInfo );
	}
}

void CChatOutList::SetDelValue(int nValue)
{
	m_nDelValue = nValue;
}

void CChatOutList::SetChatOutActState(UINT uState)
{
	m_uChatOutActState = uState;
}

void CChatOutList::SetFilterState(UINT uState)
{
	m_uFilterState = uState;
}

void CChatOutList::SetRefpViewChatTxtData(SChatTxtData **ppRefChatTxtData)
{
	m_ppRefChatTxtData = ppRefChatTxtData;
}

void CChatOutList::SetOutputLevel(UINT uState)
{
	switch(uState)
	{
	   case ChatWndSizeMin:
		   {
				m_nOutPutLine = 8 + 1;
		   }
		   break;
	   case ChatWndSizeMid:
		   {
				m_nOutPutLine = 16 + 1;
		   }
		   break;
	   case ChatWndSizeMax:
		   {
				m_nOutPutLine = 24 + 1;
		   }
		   break;
	   default:
		   {
				m_nOutPutLine = 16 + 1;
		   }
		   break;
	}
	
	ResetSize();
}

void CChatOutList::ResetSize()
{
	Ch = ( m_nOutPutLine * m_nLHeight );	
	Csize[3] = Ch;
}

void CChatOutList::InitChatOutList()
{
	m_pRITextBox->SetClientImage(TID_None);
	m_pTextBox->SetClientImage(TID_None);
	m_pTextBox2->SetClientImage(TID_None);

#if defined (ONLY_ENGLISH_SET) 	
	m_pRITextBox->SetFontg(n_ftGulimChe, 13, n_fwNormal, n_fdShadow);
	m_pTextBox->SetFontg(n_ftGulimChe, 13, n_fwNormal, n_fdNormal);
	m_pTextBox2->SetFontg(n_ftGulimChe, 13, n_fwNormal, n_fdNormal);
#else
	m_pRITextBox->SetFontg(n_ftGulimChe, 12, n_fwNormal, n_fdShadow);
	m_pTextBox->SetFontg(n_ftGulimChe, 12, n_fwNormal, n_fdNormal);
	m_pTextBox2->SetFontg(n_ftGulimChe, 12, n_fwNormal, n_fdNormal);
#endif	
	m_pRITextBox->SetAlignText(n_atLeft, n_atCenter);
	m_pTextBox->SetAlignText(n_atLeft, n_atCenter);
	m_pTextBox2->SetAlignText(n_atLeft, n_atCenter);
	
	m_pRITextBox->CreateWnd( n_wtRITextBox, WID_None, WID_None, 0, 0, (Cw - 10), m_nLHeight, false );
	m_pTextBox->CreateWnd( n_wtRITextBox, WID_None, WID_None, 0, 0, (Cw - 10), m_nLHeight, false );
	m_pTextBox2->CreateWnd( n_wtRITextBox, WID_None, WID_None, 0, 0, (Cw - 10), m_nLHeight, false );

}

SEventMessage* CChatOutList::EventProc( SEventMessage* EMsg )
{
	if(!isEnable)
	  return EMsg;	
	
	int Mx = EMsg->Mouse.Aposition.x;
	int My = EMsg->Mouse.Aposition.y;	
	
	switch( EMsg->Mouse.Action )
	{
	case MA_FOCUS:
		{
			m_IsFocus = true;
		}
		break;
	case MA_LUP:
		{
			EMsg->FocusWnd	= Iid;
			EMsg->Notify	= NM_BTN_CLICK;
			g_Sound.Play( 3 );
		}
		break;
	case MA_RUP:
		{
			EMsg->FocusWnd	= Iid;
			EMsg->Notify	= NM_BTN_RCLICK;
			g_Sound.Play( 3 );
		}
		break;
	}
	
	return( EMsg );
}

WNDID CChatOutList::FocusCheck( int Mx, int My, bool IsChildCheck )
{
	IsChildCheck;

	m_pTempFocusChatData = NULL;
	
	if(!isFocusCheck)
	{
		return WID_None;
	}	
	
	if(m_uChatOutActState == COutState_None)
	{
		return WID_None;
	}

	if(m_ppRefChatTxtData == NULL)
	{
		return WID_None;
	}
	
	int Bx, By, Bw, Bh;
	Bx = By = Bw = Bh = 0;
	
	int nX = Cx + 4;
	int nY = Cy + Ch - 21;
	int nLine = (m_nLineCount - 1) - m_nDelValue;
	
	int nCount = 0;
	SChatTxtData *pTempChatTxtData = NULL;	
	
	for(int i = nLine ; i >= 0; i-- )
	{
		pTempChatTxtData = m_pViewChatTxtData[i];		
		
		if( (pTempChatTxtData->ChatState == n_AnnounChat) ||
			(pTempChatTxtData->ChatState == n_HelpChat) )
		{
			nCount++;		
			if(nCount >= (m_nOutPutLine - 1) )
			{
				break;
			}

			continue;
		}
		
		int nPosX = nX;
		int nPosY = nY - ( nCount * m_nLHeight );		
		
		if( Rstrcmp(pTempChatTxtData->Dest, _RT("") ) != 0 )
		{
			Bx = nPosX; //BoundingBox
			By = nPosY;
			Bw = nPosX + pTempChatTxtData->TSize.tWidth + 1;
			Bh = nPosY + pTempChatTxtData->TSize.tHeight + 1;

			if( ( (Bx <= Mx) && (Bw >= Mx) ) && ( (By <= My) && (Bh >= My) ) )
			{
				m_pTempFocusChatData = pTempChatTxtData;
				m_FocusRect.x = Bx;
				m_FocusRect.y = By;
				m_FocusRect.w = pTempChatTxtData->TSize.tWidth + 1;
				m_FocusRect.h = pTempChatTxtData->TSize.tHeight + 3;
				
				return Iid;				
			}
		}
		
		nCount++;		
		if(nCount >= (m_nOutPutLine - 1) )
		{
			break;
		}
	}
	
	return WID_None;
}

void CChatOutList::UpdateViewChatTxtData()
{
	if(m_ppRefChatTxtData == NULL)
	{
		return;
	}

	m_nLineCount = 0;

	SChatTxtData *pTempChatTxtData = NULL;

	int nCount = 0;
	
	while(m_ppRefChatTxtData[nCount])
	{
		pTempChatTxtData = m_ppRefChatTxtData[nCount];
		
		UINT uTAlpha = pTempChatTxtData->Rcolor[A];
		UINT uRed =	   pTempChatTxtData->Rcolor[R];
		UINT uGreen =  pTempChatTxtData->Rcolor[G];
		UINT uBlue =   pTempChatTxtData->Rcolor[B];

		if( ( (m_uChatOutActState == COutState_NonAct) && (uTAlpha == 0) ) || 
			( Rstrcmp(pTempChatTxtData->Text, _RT("") ) == 0) ) 
		{
			nCount++;
			continue;
		}
		
		m_pViewChatTxtData[m_nLineCount] = pTempChatTxtData;
		
		nCount++;
		m_nLineCount++;
	}

	if(m_pLinkScrollbar)
	{
		if((m_nOutPutLine - 1) < m_nLineCount)
		{
			m_pLinkScrollbar->SetVisible(true);
			if(m_pLinkScrollBarBg)
			{
				m_pLinkScrollBarBg->SetVisible(true);
			}
		}
		else
		{			
			if(m_pLinkScrollbar->IsVisible())
			{
				int nLastValue = ( MAX_CHATDATA_NUM - GetOutPutLine() );
				m_pLinkScrollbar->ReSetScroll(0, nLastValue, nLastValue);			
				m_nDelValue = 0;
			}

			m_pLinkScrollbar->SetVisible(false);
			if(m_pLinkScrollBarBg)
			{
				m_pLinkScrollBarBg->SetVisible(false);
			}
		}
	}
}

void CChatOutList::RenderProc()
{
	if(m_uChatOutActState == COutState_None)
	{
		return;
	}

	UpdateViewChatTxtData();
	
	if(m_uChatOutActState == COutState_NonAct)
	{
		CalcRenderOutBound();
	}
	else
	{
		int a = 0;
	}

	int nX = Cx + 4;
	int nY = Cy + Ch - 21;
	int nLine = (m_nLineCount - 1) - m_nDelValue;

	int nCount = 0;
	SChatTxtData *pTempChatTxtData = NULL;
	
	CTextBox *pTextBox = NULL;

	for(int i = nLine ; i >= 0; i-- )
	{
		pTempChatTxtData = m_pViewChatTxtData[i];		
		
		UINT uTAlpha = pTempChatTxtData->Rcolor[A];
		UINT uRed =	   pTempChatTxtData->Rcolor[R];
		UINT uGreen =  pTempChatTxtData->Rcolor[G];
		UINT uBlue =   pTempChatTxtData->Rcolor[B];


		if( m_uChatOutActState == COutState_Act )
		{
			uTAlpha = 255;
		}

		int nPosX = nX;
		int nPosY = nY - ( nCount * m_nLHeight );

		SRect RITextBoxRect;		
		
		pTextBox = (CTextBox *)m_pRITextBox;
		
		if( n_WholeServerChat == pTempChatTxtData->ChatState ||
			n_WholeChannelChat == pTempChatTxtData->ChatState ||
			n_WholeAreaChat == pTempChatTxtData->ChatState 	)
		{
			pTextBox = m_pRITextBox;
		}		
		
		if( Rstrcmp(pTempChatTxtData->Dest, _RT("") ) != 0 )
		{
			pTextBox->SetBlank(0, 0, 0, 0);
		}
		else
		{
			pTextBox->SetBlank(20, 0, 0, 0);		
		}
		
		pTextBox->GetClientSize(&RITextBoxRect);		
			
		RITextBoxRect.x = nPosX;
		RITextBoxRect.y = nPosY;		
		pTextBox->ResetSize(&RITextBoxRect);
		pTextBox->SetText(pTempChatTxtData->Text);

		RITextBoxRect.x = nPosX - 24;
		RITextBoxRect.y = nPosY;		

		if(m_uChatOutActState == COutState_NonAct)
		{
			RITextBoxRect.w += 40;
		}
		else
		{
			RITextBoxRect.w += 11;
		}

		m_pTextBox->ResetSize(&RITextBoxRect);
		
		m_pRITextBox->SetClientImage(TID_None);
		m_pTextBox->SetClientImage(TID_None);
		m_pTextBox2->SetClientImage(TID_None);

		pTextBox->SetTextColor(uRed, uGreen, uBlue);
		
		if(n_WholeServerChat == pTempChatTxtData->ChatState)
		{
			m_pTextBox->SetClientImage(TID_CLIENT);
			m_pTextBox->SetClientColor(80, 0, 103);
			m_pTextBox->SetClientAlpah((int)(uTAlpha * 0.3f));
			m_pTextBox->RenderProc();

			int li_time = ( timeGetTime() % 2000 ) / 1000;
			if(li_time)
			{
				pTextBox->SetTextColor(235, 86, 216);
			}
		}
		
		if(n_WholeChannelChat == pTempChatTxtData->ChatState)
		{
			m_pTextBox->SetClientImage(TID_CLIENT);
			m_pTextBox->SetClientColor(80, 0, 103);
			m_pTextBox->SetClientAlpah((int)(uTAlpha * 0.3f));
			m_pTextBox->RenderProc();

			int li_time = ( timeGetTime() % 2000 ) / 1000;
			if(li_time)
			{
				pTextBox->SetTextColor(65, 168, 240);
			}			
		}		
		
		if(n_WholeAreaChat == pTempChatTxtData->ChatState)
		{
			m_pTextBox->SetClientImage(TID_CLIENT);
			m_pTextBox->SetClientColor(80, 0, 103);
			m_pTextBox->SetClientAlpah((int)(uTAlpha * 0.3f));
			m_pTextBox->RenderProc();

			int li_time = ( timeGetTime() % 2000 ) / 1000;
			if(li_time)
			{
				pTextBox->SetTextColor(255 ,122 ,  3 );
			}			
		}		


		pTextBox->SetTextAlpha(uTAlpha);
		pTextBox->RenderProc();

		nCount++;
		
		if(nCount >= (m_nOutPutLine - 1) )
		{
			break;
		}
	}	
	
	D3DCOLOR Ccolor = D3DCOLOR_ARGB( 100, 200, 200, 200 );	
	
	if(m_IsFocus)
	{
		Render->Draw( TID_CLIENT, m_FocusRect.x, m_FocusRect.y, 
								  m_FocusRect.w, m_FocusRect.h, Ccolor );		
		m_IsFocus = false;
	}
}


void CChatOutList::CalcRenderOutBound()
{	
	int nCount = min( m_nLineCount, (m_nOutPutLine - 1) );

	int nX = Cx + 4;
	int nY = Cy + Ch;	
	
	if(nCount == 0)
	{
		return;
	}
	
	int nPosX = Cx - 29;
	int nPosY = nY - ( ( (nCount - 1) * m_nLHeight ) + 30); 
	int nWidth = Cw + 29;
	int nHeight = (Cy + Ch) - nPosY;
	
	SVertex vt[4];	
	
	vt[0].diffuse =	D3DCOLOR_ARGB( 100, 50, 50, 50 );
	vt[1].diffuse = D3DCOLOR_ARGB( 0, 50, 50, 50 );
    vt[2].diffuse = D3DCOLOR_ARGB( 155, 50, 50, 50 );		
	vt[3].diffuse = D3DCOLOR_ARGB( 0, 50, 50, 50 );
	
	vt[0].x = (float)nPosX - 0.5f;
    vt[0].y = (float)nPosY - 0.5f;		
    vt[0].w = 1.0f;

	vt[1].x = vt[0].x + nWidth;
    vt[1].y = vt[0].y;
    vt[1].w = 1.0f;

	vt[2].x = vt[0].x;
    vt[2].y = vt[0].y + nHeight;	
    vt[2].w = 1.0f;
	
    
	vt[3].x = vt[0].x + nWidth;
    vt[3].y = vt[0].y + nHeight;
    vt[3].w = 1.0f;

	DWORD Col0Op, Col0ARG1, Col0ARG2;
	DWORD Col1Op, Col1ARG1, Col1ARG2;
	DWORD Col2Op, Col2ARG1, Col2ARG2;
	
	g_RenderManager.GetTextureStageState( 0, D3DTSS_COLOROP,   &Col0Op );
	g_RenderManager.GetTextureStageState( 0, D3DTSS_COLORARG1, &Col0ARG1 );
	g_RenderManager.GetTextureStageState( 0, D3DTSS_COLORARG2, &Col0ARG2 );

	g_RenderManager.GetTextureStageState( 1, D3DTSS_COLOROP,   &Col1Op );
	g_RenderManager.GetTextureStageState( 1, D3DTSS_COLORARG1, &Col1ARG1 );
	g_RenderManager.GetTextureStageState( 1, D3DTSS_COLORARG2, &Col1ARG2 );

	g_RenderManager.GetTextureStageState( 2, D3DTSS_COLOROP,   &Col2Op );
	g_RenderManager.GetTextureStageState( 2, D3DTSS_COLORARG1, &Col2ARG1 );
	g_RenderManager.GetTextureStageState( 2, D3DTSS_COLORARG2, &Col2ARG2 );

	g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_SELECTARG1 );
	g_RenderManager.SetTextureColorArg1( 0 , D3DTA_DIFFUSE );
	g_RenderManager.SetTextureColorArg2( 0 , D3DTA_TEXTURE );

	g_RenderManager.SetTextureColorOP  ( 1 , D3DTOP_DISABLE );
	g_RenderManager.SetTextureColorArg1( 1 , D3DTOP_DISABLE );
	g_RenderManager.SetTextureColorArg2( 1 , D3DTOP_DISABLE );

	g_RenderManager.SetTextureColorOP  ( 2 , D3DTOP_DISABLE );
	g_RenderManager.SetTextureColorArg1( 2 , D3DTOP_DISABLE );
	g_RenderManager.SetTextureColorArg2( 2 , D3DTOP_DISABLE );

	g_RenderManager.SetFVF( RUI_VERTEX_FORMAT );
	g_RenderManager.SetTexture( 0, NULL );
	g_RenderManager.DrawPrimitiveUP( D3DPT_TRIANGLESTRIP , 2, vt, sizeof( SVertex ) );
	
	g_RenderManager.SetTextureColorOP  ( 0 , Col0Op );
	g_RenderManager.SetTextureColorArg1( 0 , Col0ARG1 );
	g_RenderManager.SetTextureColorArg2( 0 , Col0ARG2 );

	g_RenderManager.SetTextureColorOP  ( 1 , Col1Op );
	g_RenderManager.SetTextureColorArg1( 1 , Col1ARG1 );
	g_RenderManager.SetTextureColorArg2( 1 , Col1ARG2 );

	g_RenderManager.SetTextureColorOP  ( 2 , Col2Op );
	g_RenderManager.SetTextureColorArg1( 2 , Col2ARG1 );
	g_RenderManager.SetTextureColorArg2( 2 , Col2ARG2 );
}


CSystemOutList::CSystemOutList( SDesktop* DT ) : CWnd(DT)
{	
	m_IsAutoScroll = TRUE;
	
	m_pRITextBox = SAFE_NEW( CRITextBox(DT) );
	m_pRITextBox->m_IsMarkUp = TRUE;
	
	m_pLinkScrollBarBg = NULL;
	m_pLinkScrollbar = NULL;

	m_nLHeight = 16;

	m_pRefSystemTxtData = NULL;
	memset( m_pViewSystemTxtData, 0, sizeof(SSystemTxtData*) * (MAX_SYSTEMDATA_NUM + 1) );

	m_nOutPutLine = 0;
	m_nLineCount = 0;
	m_nDelValue = 0;
}


CSystemOutList::~CSystemOutList()
{
	SAFE_DELETE(m_pRITextBox);
}

void CSystemOutList::SetDelValue(int nValue)
{
	m_nDelValue = nValue;
}

void CSystemOutList::SetRefpViewSystemTxtData(SSystemTxtData	*pRefSystemTxtData)
{
	m_pRefSystemTxtData = pRefSystemTxtData;
}

void CSystemOutList::SetLinkScroll(CScrollBar* pLinkScrollbar, CWnd * pLinkScrollBarBg)
{
	m_pLinkScrollBarBg = pLinkScrollBarBg;
	m_pLinkScrollbar = pLinkScrollbar;
}

void CSystemOutList::GetStringInfo( TCHAR *strTemp , SStringInfo * StrInfo )
{
	if(m_pRITextBox)
	{
		m_pRITextBox->GetFong()->GetRIStringInfo( strTemp , StrInfo );
	}
}

void CSystemOutList::SetOutputLevel(UINT uState)
{
	switch(uState)
	{
	   case ChatWndSizeMin:
		   {
				m_nOutPutLine = 8 + 1;
		   }
		   break;
	   case ChatWndSizeMid:
		   {
				m_nOutPutLine = 16 + 1;
		   }
		   break;
	   case ChatWndSizeMax:
		   {
				m_nOutPutLine = 24 + 1;
		   }
		   break;
	   default:
		   {
				m_nOutPutLine = 16 + 1;
		   }
		   break;
	}
	
	ResetSize();
}

void CSystemOutList::ResetSize()
{
	Ch = ( m_nOutPutLine * m_nLHeight );	
	Csize[3] = Ch;
}

void CSystemOutList::InitSystemOutList()
{
	m_pRITextBox->SetClientImage(TID_None);

#if defined (ONLY_ENGLISH_SET) 		
	m_pRITextBox->SetFontg(n_ftGulimChe, 13, n_fwNormal, n_fdShadow);	
#else
	m_pRITextBox->SetFontg(n_ftGulimChe, 12, n_fwNormal, n_fdShadow);
#endif	

	m_pRITextBox->SetAlignText(n_atLeft, n_atCenter);
	m_pRITextBox->CreateWnd( n_wtRITextBox, WID_None, WID_None, 0, 0, (Cw - 10), m_nLHeight, false );
}

void CSystemOutList::UpdateViewSystemTxtData()
{
	if(m_pRefSystemTxtData == NULL)
	{
		return;
	}
	
	memset( m_pViewSystemTxtData, 0, sizeof(SSystemTxtData*) * (MAX_SYSTEMDATA_NUM + 1) );
	m_nLineCount = 0;

	SSystemTxtData *pTempSystemTxtData = NULL;
	for(int i = 0 ; i < MAX_SYSTEMDATA_NUM ; i++)
	{
		pTempSystemTxtData = &m_pRefSystemTxtData[i];
		UINT uTAlpha = pTempSystemTxtData->Rcolor[A];
	
		if( ( uTAlpha == 0 ) || ( Rstrcmp(pTempSystemTxtData->Text, _RT("") ) == 0) ) 
		{
			continue;
		}

		m_pViewSystemTxtData[m_nLineCount] = pTempSystemTxtData;
		m_nLineCount++;
	}

	if( m_pLinkScrollbar && m_IsAutoScroll )
	{
		if((m_nOutPutLine - 1) < m_nLineCount)
		{
			m_pLinkScrollbar->SetVisible(true);
			if(m_pLinkScrollBarBg)
			{
				m_pLinkScrollBarBg->SetVisible(true);
			}
		}
		else
		{
			if(m_pLinkScrollbar->IsVisible())
			{
				int nLastValue = ( MAX_SYSTEMDATA_NUM - GetOutPutLine() );
				m_pLinkScrollbar->ReSetScroll(0, nLastValue, nLastValue);			
				m_nDelValue = 0;
			}
			
			m_pLinkScrollbar->SetVisible(false);
			if(m_pLinkScrollBarBg)
			{
				m_pLinkScrollBarBg->SetVisible(false);
			}
		}
	}
}

void CSystemOutList::RenderProc()
{
	UpdateViewSystemTxtData();
	
	CalcRenderOutBound();	
	
	int nX = Cx + 4;
	int nY = Cy + Ch - 21;
	int nLine = (m_nLineCount - 1) - m_nDelValue;

	int nCount = 0;
	SSystemTxtData *pTempSystemTxtData = NULL;
	
	Render->InitMarkUp();
	
	for(int i = nLine ; i >= 0 ; i-- )
	{
		/*pTempSystemTxtData = m_pViewSystemTxtData[i];		
		
		UINT uTAlpha = pTempSystemTxtData->Rcolor[A];
		UINT uRed =	   pTempSystemTxtData->Rcolor[R];
		UINT uGreen =  pTempSystemTxtData->Rcolor[G];
		UINT uBlue =   pTempSystemTxtData->Rcolor[B];

		int nPosX = nX;
		int nPosY = nY - ( nCount * m_nLHeight );

		SRect RITextBoxRect;		
		
		m_pRITextBox->SetBlank(0, 0, 0, 0);		
		m_pRITextBox->GetClientSize(&RITextBoxRect);		
			
		RITextBoxRect.x = nPosX;
		RITextBoxRect.y = nPosY;
		m_pRITextBox->ResetSize(&RITextBoxRect);
		m_pRITextBox->SetText( pTempSystemTxtData->Text );
		
		m_pRITextBox->SetTextColor(uRed, uGreen, uBlue);
		m_pRITextBox->SetTextAlpha(uTAlpha);
		m_pRITextBox->RenderProc();*/

		nCount++;
		
		if(nCount >= (m_nOutPutLine - 1) )
		{
			break;
		}
	}

	Render->InitMarkUp();	

	int j = nCount - 1;

	for(int i = 0 ; i < nCount ; i++, j-- )
	{
		pTempSystemTxtData = m_pViewSystemTxtData[ (nLine+1) - nCount + i ];
			
		UINT uTAlpha = pTempSystemTxtData->Rcolor[A];
		UINT uRed =	   pTempSystemTxtData->Rcolor[R];
		UINT uGreen =  pTempSystemTxtData->Rcolor[G];
		UINT uBlue =   pTempSystemTxtData->Rcolor[B];

		int nPosX = nX;
		int nPosY = nY - ( j * m_nLHeight );

		SRect RITextBoxRect;		
		
		m_pRITextBox->SetBlank(0, 0, 0, 0);		
		m_pRITextBox->GetClientSize(&RITextBoxRect);		
			
		RITextBoxRect.x = nPosX;
		RITextBoxRect.y = nPosY;
		m_pRITextBox->ResetSize(&RITextBoxRect);
		m_pRITextBox->SetText( pTempSystemTxtData->Text );
		
		m_pRITextBox->SetTextColor(uRed, uGreen, uBlue);
		m_pRITextBox->SetTextAlpha(uTAlpha);
		m_pRITextBox->RenderProc();
	}

	Render->InitMarkUp();
}

void CSystemOutList::CalcRenderOutBound()
{
	int nCount = min( m_nLineCount, (m_nOutPutLine - 1) );

	int nX = Cx + 4;
	int nY = Cy + Ch - 21;	
	
	if(nCount == 0)
	{
		return;
	}
	
	int nPosX = Cx;
	int nPosY = nY - ( ( (nCount - 1) * m_nLHeight ) + 8); 
	int nWidth = Cw + 29;
	int nHeight = (Cy + Ch) - nPosY;
	
	SVertex vt[4];
/*	
	vt[0].diffuse =	D3DCOLOR_ARGB( 0, 50, 50, 50 );
	vt[1].diffuse = D3DCOLOR_ARGB( 155, 50, 50, 50 );
    vt[2].diffuse = D3DCOLOR_ARGB( 155, 50, 50, 50 );		
	vt[3].diffuse = D3DCOLOR_ARGB( 0, 50, 50, 50 );
	
	vt[0].x = (float)nPosX - 0.5f;
    vt[0].y = (float)nPosY - 0.5f;		
    vt[1].x = vt[0].x + nWidth;
    vt[1].y = vt[0].y;
    vt[2].x = vt[0].x + nWidth;
    vt[2].y = vt[0].y + nHeight;
    vt[3].x = vt[0].x - 20;
    vt[3].y = vt[0].y + nHeight;
*/
/*
	vt[0].diffuse =	D3DCOLOR_ARGB( 0, 50, 50, 50 );
	vt[1].diffuse = D3DCOLOR_ARGB( 155, 50, 50, 50 );
    vt[2].diffuse = D3DCOLOR_ARGB( 155, 50, 50, 50 );		
	vt[3].diffuse = D3DCOLOR_ARGB( 0, 50, 50, 50 );
*/	
	vt[0].diffuse =	D3DCOLOR_ARGB( 155, 50, 50, 50 );
	vt[1].diffuse = D3DCOLOR_ARGB( 0, 50, 50, 50 );
    vt[2].diffuse = D3DCOLOR_ARGB( 155, 50, 50, 50 );		
	vt[3].diffuse = D3DCOLOR_ARGB( 0, 50, 50, 50 );

	vt[0].x = (float)nPosX - 0.5f;
    vt[0].y = (float)nPosY - 0.5f;		
	vt[0].w = 1.0f;

	vt[1].x = vt[0].x + nWidth;
    vt[1].y = vt[0].y;
	vt[1].w = 1.0f;

	vt[2].x = vt[0].x;
    vt[2].y = vt[0].y + nHeight;
	vt[2].w = 1.0f;
    
	vt[3].x = vt[0].x + nWidth;
    vt[3].y = vt[0].y + nHeight;
	vt[3].w = 1.0f;
    

	DWORD Col0Op, Col0ARG1, Col0ARG2;
	DWORD Alp0Op, Alp0ARG1, Alp0ARG2;

	DWORD Col1Op, Col1ARG1, Col1ARG2;
	//DWORD Alp1Op, Alp1ARG1, Alp1ARG2;

	DWORD Col2Op, Col2ARG1, Col2ARG2;
	//DWORD Alp2Op, Alp2ARG1, Alp2ARG2;

	g_RenderManager.GetTextureStageState( 0, D3DTSS_COLOROP,   &Col0Op );
	g_RenderManager.GetTextureStageState( 0, D3DTSS_COLORARG1, &Col0ARG1 );
	g_RenderManager.GetTextureStageState( 0, D3DTSS_COLORARG2, &Col0ARG2 );

	g_RenderManager.GetTextureStageState( 0, D3DTSS_ALPHAOP,   &Alp0Op );
	g_RenderManager.GetTextureStageState( 0, D3DTSS_ALPHAARG1, &Alp0ARG1 );
	g_RenderManager.GetTextureStageState( 0, D3DTSS_ALPHAARG2, &Alp0ARG2 );

	g_RenderManager.GetTextureStageState( 1, D3DTSS_COLOROP,   &Col1Op );
	g_RenderManager.GetTextureStageState( 1, D3DTSS_COLORARG1, &Col1ARG1 );
	g_RenderManager.GetTextureStageState( 1, D3DTSS_COLORARG2, &Col1ARG2 );

	g_RenderManager.GetTextureStageState( 2, D3DTSS_COLOROP,   &Col2Op );
	g_RenderManager.GetTextureStageState( 2, D3DTSS_COLORARG1, &Col2ARG1 );
	g_RenderManager.GetTextureStageState( 2, D3DTSS_COLORARG2, &Col2ARG2 );

	g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_SELECTARG1 );
	g_RenderManager.SetTextureColorArg1( 0 , D3DTA_DIFFUSE );
	g_RenderManager.SetTextureColorArg2( 0 , D3DTOP_DISABLE );
	
	g_RenderManager.SetTextureAlphaOP  ( 0 , D3DTOP_SELECTARG1 );
	g_RenderManager.SetTextureAlphaArg1( 0 , D3DTA_DIFFUSE );
	g_RenderManager.SetTextureAlphaArg2( 0 , D3DTOP_DISABLE );

	g_RenderManager.SetTextureColorOP  ( 1 , D3DTOP_DISABLE );
	g_RenderManager.SetTextureColorArg1( 1 , D3DTOP_DISABLE );
	g_RenderManager.SetTextureColorArg2( 1 , D3DTOP_DISABLE );

	g_RenderManager.SetTextureColorOP  ( 2 , D3DTOP_DISABLE );
	g_RenderManager.SetTextureColorArg1( 2 , D3DTOP_DISABLE );
	g_RenderManager.SetTextureColorArg2( 2 , D3DTOP_DISABLE );

	g_RenderManager.SetFVF( RUI_VERTEX_FORMAT );
	g_RenderManager.SetTexture( 0, NULL );
	g_RenderManager.DrawPrimitiveUP( D3DPT_TRIANGLESTRIP , 2, vt, sizeof( SVertex ) );

	g_RenderManager.SetTextureColorOP  ( 0 , Col0Op );
	g_RenderManager.SetTextureColorArg1( 0 , Col0ARG1 );
	g_RenderManager.SetTextureColorArg2( 0 , Col0ARG2 );
	
	g_RenderManager.SetTextureAlphaOP  ( 0 , Alp0Op );
	g_RenderManager.SetTextureAlphaArg1( 0 , Alp0ARG1 );
	g_RenderManager.SetTextureAlphaArg2( 0 , Alp0ARG2 );

	g_RenderManager.SetTextureColorOP  ( 1 , Col1Op );
	g_RenderManager.SetTextureColorArg1( 1 , Col1ARG1 );
	g_RenderManager.SetTextureColorArg2( 1 , Col1ARG2 );

	g_RenderManager.SetTextureColorOP  ( 2 , Col2Op );
	g_RenderManager.SetTextureColorArg1( 2 , Col2ARG1 );
	g_RenderManager.SetTextureColorArg2( 2 , Col2ARG2 );
}