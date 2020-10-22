#include "..\\..\\RockClient.h"
#include "..\\RockPCH.h"

#include "Rui.h"
#include "..\\..\\GTimer.h"

#include	"..\\..\\quadlist.h"
#include	<NETWORK\\Protocol.h>
#include	<Base\\D3DBase.h>
#include	<Obj\\ObjBase.h>
#include	<Map\\Field.h>

#include	"..\\..\\Pc.h"
#include "..\\..\\global.h"

#include "ComEffImgBox.h"

#include "..\\..\\CRenderManager.h"

#ifdef DIRECT_VERSION_9_MJH
extern IDirect3DDevice9*	g_lpDevice;
#else
extern IDirect3DDevice8*	g_lpDevice;
#endif // DIRECT_VERSION_9_MJH

CComEffImgBox::CComEffImgBox( SDesktop* DT ) : CImageBox( DT )
{
	m_RenderTexImg = TID_None;
	m_IsRenderImg = false;	
	
	m_uiNowEffStep = ComboEffectStep_None;
	m_fCumulativeTime = 0.0f;
	m_IsInit = false;
	m_bPosInit = false;

	m_fGlowTime = 0.15f;
	m_fGCumulTime = 0.0f;
	m_bRenderGlow = false;

	m_uTransImg = TID_None;
	m_uTimgCw = 0;
	m_uTimgCh = 0;

	m_pControlStepBar = NULL;
}

CComEffImgBox::~CComEffImgBox()
{


}

void CComEffImgBox::InitData()
{
	if(m_ComEffQueue.GetCurQueueSize())
	{
		m_ComEffQueue.PopData(m_ComEffInfo);
	}
	
	m_IsRenderImg = false;	
	m_fCumulativeTime = 0.0f;	
	
	m_fGlowTime = 0.1f;
	m_fGCumulTime = 0.0f;
	m_bRenderGlow = false;	
	
	m_IsPushEffect = true;
	m_uiNowEffStep = ComboEffectStep_None;
	m_RenderTexImg = TID_None;

	m_fMaxIntervalTime = 0.25f;		 
	m_fMaxFlashCumulTime = 0.0f;	 
	m_bMaxFlashSwitchFlag = false; 	 	
	
	m_bStartFlash = false;
	m_bFlashFlag = true;			 
	m_fFCumulTime = 0.0f;			 
	m_fFInterTime = 0.15f;
	
	m_PushStep_01ExTime = 0.25f;

	m_PushStep_03ExTime = 0.01f;
	m_PushStep_04ExTime = 0.25f;
	m_PushStep_05ExTime = 0.1f;

	m_PopStep_01ExTime = 0.5f;
	m_PopStep_02ExTime = 1.0f;

	m_IsRenderCross = false;
	m_fCrossRenTime = 0.1f;
	m_fCrossCumTime = 0.0f;
	m_fCrossStep = Cross_VerRenderStep;

	m_IsInit = true;
	m_bPosInit = false;
}

void CComEffImgBox::SetTransImgID(UINT uImg)
{
	m_uTransImg = uImg;
}

void CComEffImgBox::SetTransImgIDSize(int nCw, int nCh)
{
	m_uTimgCw = nCw;
	m_uTimgCh = nCh;
}

void CComEffImgBox::GetTransInfo(bool bFlag, int & nCw, int & nCh)
{	
	if( (m_uTransImg != TID_None) && bFlag )
	{
		nCw = m_uTimgCw;
		nCh = m_uTimgCh;
		
		m_RenderTexImg = m_uTransImg;
	}
	else
	{
		nCw = Cw;
		nCh = Ch;
		
		m_RenderTexImg = Cimg;
	}
}
	
void CComEffImgBox::RenderProc()
{
	if(!m_IsInit)
	{
		InitData();
	}
	
	if( m_IsRenderCross )
	{
		RenderCrossFlash();
	}

	DWORD SrcBlend, DestBlend;
	g_RenderManager.GetRenderState( D3DRS_SRCBLEND, &SrcBlend );
	g_RenderManager.GetRenderState( D3DRS_DESTBLEND, &DestBlend );
	
	if( CBgImage != TID_None )
	{
		Render->Draw(CBgImage , Cx, Cy, Cw, Ch, Ccolor );
	}
	
	if(m_IsRenderImg)
	{
		CTextBox::RenderProc();
	}	
	
	if( m_uiNowEffStep == ComboEffectStep_None )
	{
		if(m_ComEffQueue.GetCurQueueSize())
		{
			m_ComEffQueue.PopData(m_ComEffInfo);
			m_uiNowEffStep = ComboEffectStep_01;		
		}
		else
		{
			return;
		}		
	}
	
	switch(m_uiNowEffStep)
	{
	case ComboEffectStep_01:
		{
			if(m_ComEffInfo.IsPushEffect)
			{
				RenderPushEffStep_01();
			}
			else
			{
				RenderPopEffStep_01();
			}
		}
		break;
	case ComboEffectStep_02:
		{
			if(m_ComEffInfo.IsPushEffect)
			{
				RenderPushEffStep_02();
			}
			else
			{
				RenderPopEffStep_02();
			}
		}
		break;
	case ComboEffectStep_03:
		{
			RenderPushEffStep_03();
		}
		break;
	case ComboEffectStep_04:
		{
			RenderPushEffStep_04();
		}
		break;
	}	
	
	if( m_bStartFlash || m_bRenderGlow )
	{
		if(m_bRenderGlow)
		{
			g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
			g_RenderManager.SetDestBlend( D3DBLEND_DESTALPHA );
		}		
		
		if( m_bStartFlash )
		{
			if(m_bFlashFlag)
			{
				g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
				g_RenderManager.SetDestBlend( D3DBLEND_DESTALPHA );
			}

			m_fFCumulTime += g_GTimer.GetFrameElapsedSecTime();
			if( m_fFCumulTime > m_fFInterTime )
			{
				m_fFCumulTime -= m_fFInterTime;
				m_bFlashFlag = !m_bFlashFlag;				
			}
		}
	}
	
	Render->ScretchDraw( m_RenderTexImg , m_iCx, m_iCy, m_iCw, m_iCh, m_iColor );
	
	if( m_bStartFlash || m_bRenderGlow )
	{
		g_RenderManager.SetSrcBlend( SrcBlend );
		g_RenderManager.SetDestBlend( DestBlend );
		
		if(m_bRenderGlow)
		{
			m_fGCumulTime += g_GTimer.GetFrameElapsedSecTime();
			if(m_fGCumulTime > m_fGlowTime)
			{
				m_fGCumulTime = 0.0f;
				m_bRenderGlow = false; 
			}
		}
	}
	
	m_fCumulativeTime += g_GTimer.GetFrameElapsedSecTime();
	
}

void CComEffImgBox::UpdateFlashSwitchFlag()
{	
	float fElapsedtime = g_GTimer.GetFrameElapsedSecTime();	
	m_fMaxFlashCumulTime += fElapsedtime;
	
	if( m_fMaxFlashCumulTime >= m_fMaxIntervalTime )
	{
		m_bMaxFlashSwitchFlag = !m_bMaxFlashSwitchFlag; 
		m_fMaxFlashCumulTime -= fElapsedtime;
		m_fMaxFlashCumulTime = (m_fMaxIntervalTime - m_fMaxFlashCumulTime);		
	}
}

/*void CComEffImgBox::RenderPushEffStep_01()
{
	//점점커지는 단계
	if(!m_bPosInit)
	{
		CWnd *pFDestWnd = Container->Get( WID_ComboGauge_Gauge );
		SRect WcSize;
		pFDestWnd->GetClientSize( &WcSize );

		m_sPos.x = WcSize.x + (WcSize.w/2);// + 20;
		m_sPos.y = WcSize.y + WcSize.h;
	
		m_bStartFlash = true;
		m_IsRenderImg = false;
		m_bPosInit = true;

		CWnd *pParentWnd = Container->Get( Pid );
		if( pParentWnd && 
		    ( (pParentWnd->GetWindowType() == n_wtFlashFrameWnd) ||
			  (pParentWnd->GetWindowType() == n_wtFrameWnd) ) )
		{
			CWnd *pWnd = Container->Get( Iid );
			((CFrameWnd*)pParentWnd)->GetChain()->Ordering( pWnd );
		}
	}

	float fRatio = m_fCumulativeTime / m_PushStep_01ExTime;
	if(m_fCumulativeTime > m_PushStep_01ExTime)
	{
		m_fCumulativeTime -= m_PushStep_01ExTime;
		fRatio = 1.0f;		
		m_uiNowEffStep = ComboEffectStep_02;

		m_TempPos.x = m_sPos.x;
		m_TempPos.y = m_sPos.y;

		m_bPosInit = false;
	}
	
	int nCw, nCh;
	GetTransInfo(true, nCw, nCh );

	m_iCw = (nCw + 2) - 5 + ( fRatio * 5 );
	m_iCh = (nCh + 2) - 5 + ( fRatio * 5 );
	
	if(((m_iCw % 2) != 0) && (m_iCw != nCw) )
		m_iCw += 1;
	if(((m_iCh % 2) != 0) && (m_iCh != nCh))
		m_iCh += 1;

	m_iCx = m_sPos.x - ( m_iCw/2 );
	m_iCy = m_sPos.y - ( m_iCh/2 );

	m_RealCenPos = m_sPos;
	m_scaleRatio =  (float)(m_iCw)/(float)nCw;

	m_iColor = D3DCOLOR_ARGB( (int)(fRatio * 255), 255, 255, 255 );	 
}*/

void CComEffImgBox::RenderPushEffStep_01()
{
	if(!m_bPosInit)
	{
		/*m_sPos.x = m_TempPos.x;
		m_sPos.y = m_TempPos.y;*/
		
		CWnd *pFDestWnd = Container->Get( WID_ComboGauge_Gauge );
		SRect WcSize;
		pFDestWnd->GetClientSize( &WcSize );

		m_sPos.x = WcSize.x + (WcSize.w/2);// + 20;
		m_sPos.y = WcSize.y + WcSize.h;
	
		m_bStartFlash = true;
		m_IsRenderImg = false;
		m_bPosInit = true;

		CWnd *pParentWnd = Container->Get( Pid );
		if( pParentWnd && 
		    ( (pParentWnd->GetWindowType() == n_wtFlashFrameWnd) ||
			  (pParentWnd->GetWindowType() == n_wtFrameWnd) ) )
		{
			CWnd *pWnd = Container->Get( Iid );
			((CFrameWnd*)pParentWnd)->GetChain()->Ordering( pWnd );
		}
		
		m_bPosInit = true;
		m_bRenderGlow = true;
		
		if(m_pControlStepBar)
		{
			m_pControlStepBar->SetSwitchComLevel(true);
		}
	}

	float fRatio = m_fCumulativeTime / m_PushStep_01ExTime;
	if(m_fCumulativeTime > m_PushStep_01ExTime)
	{
		m_fCumulativeTime -= m_PushStep_01ExTime;			
		m_uiNowEffStep = ComboEffectStep_02;

		m_TempPos.x = m_sPos.x;
		m_TempPos.y = m_sPos.y;
		m_bPosInit = false;		
	}

	int nCw, nCh;
	GetTransInfo(true, nCw, nCh );

	m_iCw = nCw;
	m_iCh = nCh;

	m_iCx = m_sPos.x - ( m_iCw/2 );
	m_iCy = m_sPos.y - ( m_iCh/2 );

	m_RealCenPos = m_sPos;
	m_scaleRatio = 1.0f;
	
	m_iColor = D3DCOLOR_ARGB( 255, 255, 255, 255 );
}

void CComEffImgBox::RenderPushEffStep_02()
{
	if(!m_bPosInit)
	{
		m_sPos.x = m_TempPos.x;
		m_sPos.y = m_TempPos.y;
		
		m_ePos.x = Cx + (Cw/2);
		m_ePos.y = Cy + (Ch/2);

		m_cenPos.x = (m_sPos.x + m_ePos.x) / 2;
		m_cenPos.y = (m_sPos.y + m_ePos.y) / 2;
		
		m_bPosInit = true;
	}
	
	float radian = m_fCumulativeTime * 13.0f;

	if( radian >= D3DX_PI )
	{
		radian = D3DX_PI;				
		
		m_fCumulativeTime = 0.0f;				
		m_uiNowEffStep = ComboEffectStep_03;

		m_TempPos.x = m_ePos.x;
		m_TempPos.y = m_ePos.y;

		m_bPosInit = false;		
	}

	POINT imPos;
	imPos.x = m_sPos.x - m_cenPos.x;
	imPos.y = m_sPos.y - m_cenPos.y;
	
	POINT temPos;
	
	temPos.x = cos(radian)*imPos.x - sin(radian)*imPos.y;
	temPos.y = sin(radian)*imPos.x + cos(radian)*imPos.y;

	imPos.x = temPos.x + m_cenPos.x;
	imPos.y = temPos.y + m_cenPos.y;

	if( m_uiNowEffStep == ComboEffectStep_03 )
	{
		imPos.x = m_ePos.x;
		imPos.y = m_ePos.y;
		m_bStartFlash = false;
	}

	m_RealCenPos =  imPos;
	m_scaleRatio =  1.0f;

	int nCw, nCh;
	GetTransInfo(true, nCw, nCh );

	m_iCx = imPos.x - ( nCw/2 );
	m_iCy = imPos.y - ( nCh/2 );

	m_iCw = nCw;
	m_iCh = nCh;

	m_iColor = D3DCOLOR_ARGB( 255, 255, 255, 255 );
}

void CComEffImgBox::RenderPushEffStep_03()
{
	if(!m_bPosInit)
	{
		m_RenderTexImg = Cimg;
		
		m_sPos.x = Cx + (Cw/2);
		m_sPos.y = Cy + (Ch/2);
		
		m_bPosInit = true;
	}

	float fRatio = m_fCumulativeTime / m_PushStep_03ExTime;
	if(m_fCumulativeTime > m_PushStep_03ExTime)
	{
		m_fCumulativeTime -= m_PushStep_03ExTime;
		fRatio = 1.0f;		
		m_uiNowEffStep = ComboEffectStep_04;

		m_TempPos.x = m_sPos.x;
		m_TempPos.y = m_sPos.y;

		m_bPosInit = false;		
	}

	m_iCw = Cw + ( fRatio * 16 );
	m_iCh = Ch + ( fRatio * 16 );
	if( (m_iCw % 2) != 0 )
		m_iCw += 1;
	if( (m_iCh % 2) != 0 )
		m_iCh += 1;

	m_iCx = m_sPos.x - ( m_iCw/2 ) + 1;
	m_iCy = m_sPos.y - ( m_iCh/2 );

	if( m_uiNowEffStep == ComboEffectStep_04 )
	{
		m_iCx = Cx;
		m_iCy = Cy;
		m_bRenderGlow = true;
	}

	m_RealCenPos =  m_sPos;
	m_scaleRatio =  (float)(m_iCw)/(float)Cw;

	m_iColor = D3DCOLOR_ARGB( 255, 255, 255, 255 );
}

void CComEffImgBox::RenderPushEffStep_04()
{
	m_PushStep_04ExTime = 0.1f;
	
	if(!m_bPosInit)
	{
		m_RenderTexImg = Cimg;
		
		m_sPos.x = Cx + (Cw/2);
		m_sPos.y = Cy + (Ch/2);
		
		m_bPosInit = true;
	}

	float fRatio = m_fCumulativeTime / m_PushStep_04ExTime;
	if(m_fCumulativeTime > m_PushStep_04ExTime)
	{
		m_fCumulativeTime = 0.0f;
		fRatio = 1.0f;
		m_bPosInit = false;
		m_IsRenderImg = true;

		m_uiNowEffStep = ComboEffectStep_None;		
	}
	
	m_iCw = Cw + 16 - ( fRatio * 16 );
	m_iCh = Ch + 16 - ( fRatio * 16 );
	
	if( (m_iCw % 2) != 0) 
		m_iCw += 1;
	if( (m_iCh % 2) != 0)
		m_iCh += 1;	

	m_iCx = m_sPos.x - ( m_iCw/2 ) + 1;
	m_iCy = m_sPos.y - ( m_iCh/2 );

	if( m_uiNowEffStep == ComboEffectStep_None )
	{
		m_iCx = Cx;
		m_iCy = Cy;	
		m_fFCumulTime = 0.0f;		
		m_IsRenderCross = true;
	}

	m_RealCenPos =  m_sPos;
	m_scaleRatio =  (float)(m_iCw)/(float)11.0f;

	m_iColor = D3DCOLOR_ARGB( 255, 255, 255, 255 );
}

void CComEffImgBox::RenderPopEffStep_01()
{
	if(!m_bPosInit)
	{
		m_RenderTexImg = Cimg;
		m_bPosInit = true;
		m_IsRenderCross = true;

		CWnd *pParentWnd = Container->Get( Pid );
		if( pParentWnd && 
		    ( (pParentWnd->GetWindowType() == n_wtFlashFrameWnd) ||
			  (pParentWnd->GetWindowType() == n_wtFrameWnd) ) )
		{
			CWnd *pWnd = Container->Get( Iid );
			((CFrameWnd*)pParentWnd)->GetChain()->Ordering( pWnd );
		}
	}

	float fRatio = m_fCumulativeTime / m_PopStep_01ExTime;
	if(m_fCumulativeTime > m_PopStep_01ExTime)
	{
		m_fCumulativeTime -= m_PopStep_01ExTime;
		fRatio = 1.0f;
			
		m_bPosInit = false;
		m_IsRenderImg = false;
		m_uiNowEffStep = ComboEffectStep_02;	
	}

	m_iCw = Cw;
	m_iCh = Ch;

	m_iCx = Cx;
	m_iCy = Cy;

	m_iColor = D3DCOLOR_ARGB( (int)(fRatio * 255), 255, 255, 255 );	
}

void CComEffImgBox::RenderPopEffStep_02()
{
	float fYVel = 0.0f;
	float fYAccel = 600.0f;

	if(!m_bPosInit)
	{		
		m_RenderTexImg = Cimg;
		m_bPosInit = true;
	}

	float fRatio = m_fCumulativeTime / m_PopStep_02ExTime;
	if(m_fCumulativeTime > m_PopStep_01ExTime)
	{
		m_fCumulativeTime = 0.0f;
		fRatio = 1.0f;
			
		m_bPosInit = false;
		
		m_uiNowEffStep = ComboEffectStep_None;
		m_RenderTexImg = TID_None;
	}

	int xPos = m_sPos.x;
	float fd = pow( m_fCumulativeTime, 2 );
	int yPos = m_sPos.y + 0.5f * fYAccel * fd;

	m_iCw = Cw;
	m_iCh = Ch;

	m_iCx = Cx;
	m_iCy = yPos - ( m_iCh/2 );

	m_iColor = D3DCOLOR_ARGB( (int)((1 - fRatio) * 255), 255, 255, 255 );
}

void CComEffImgBox::InsertComEffInfo( SComboEffectInfo ComEffInfo )
{
	//여기선 큐가 필요없는듯 흠...
	InitData();	
	m_ComEffQueue.PushData( ComEffInfo );
}

void CComEffImgBox::RenderCrossFlash()
{
	POINT sPos;
	sPos.x = Cx + (Cw/2);
	sPos.y = Cy + (Ch/2);	
	
	float fRatio = m_fCrossCumTime / m_fCrossRenTime;
	D3DCOLOR Color = D3DCOLOR_ARGB((int)( (1.0f - fRatio) * 255), 255, 255, 255 );
	int iPosX, iPosY;
	int iWdith, iHeight;
	iPosX = iPosY = iWdith = iHeight = 0;

	m_fCrossCumTime += g_GTimer.GetFrameElapsedSecTime();

	int nCw, nCh;
	GetTransInfo(true, nCw, nCh );

	switch( m_fCrossStep )
	{
		case Cross_VerRenderStep:
			{
			  iWdith = nCw;
			  iHeight = nCh + ( fRatio * (nCh + 100) );
			  if( (iHeight % 2) != 0 )
				  iHeight += 1;			
			  			
			  if(m_fCrossCumTime > m_fCrossRenTime)
			  {
			    m_fCrossCumTime -= m_fCrossRenTime;
				m_fCrossStep = Cross_HorRenderStep;
			  }
			}
			break;
		case Cross_HorRenderStep:
			{
			  iWdith = nCw + ( fRatio * (nCw + 100) );
			  if( (iWdith % 2) != 0 )
				  iWdith += 1;
			  iHeight = nCh;
			  
			  if(m_fCrossCumTime > m_fCrossRenTime)
			  {
				m_fCrossCumTime = 0.0f;
				m_fCrossStep = Cross_VerRenderStep;
				m_IsRenderCross = false;
			  }
			}
			break;
	}

	iPosX = sPos.x - ( iWdith/2 );
	iPosY = sPos.y - ( iHeight/2 );

	DWORD SrcBlend, DestBlend;
	
	g_RenderManager.GetRenderState( D3DRS_SRCBLEND, &SrcBlend );
	g_RenderManager.GetRenderState( D3DRS_DESTBLEND, &DestBlend );

	g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
	g_RenderManager.SetDestBlend( D3DBLEND_DESTALPHA );

	Render->ScretchDraw( m_RenderTexImg , iPosX, iPosY, iWdith, iHeight, Color );	

	g_RenderManager.SetSrcBlend( SrcBlend );
	g_RenderManager.SetDestBlend( DestBlend );

	m_RenderTexImg = Cimg;
}
