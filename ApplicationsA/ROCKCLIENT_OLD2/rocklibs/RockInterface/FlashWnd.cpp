#include "..\\..\\RockClient.h"
#include "..\\RockPCH.h"

#include "Rui.h"
#include "..\\..\\GTimer.h"

#include "FlashWnd.h"

#include "..\\..\\CRenderManager.h"

CFlashWnd::CFlashWnd( SDesktop* DT ) : CWnd(DT)
{
	m_bIsActiveFlash = false;
	m_bSwitchFlag = true;

	m_fViewIntTime = 0.5f;
	m_fNoViewIntTime = 0.2f;
	m_fCumulativeTime = 0.0f;	

	m_FlashColor = D3DCOLOR_ARGB(255, 255, 255, 255 );
	m_FlashColorOp = D3DTOP_MODULATE;
	m_SrcBlend = D3DBLEND_SRCALPHA;
	m_DestBlend = D3DBLEND_INVSRCALPHA;
}

CFlashWnd::~CFlashWnd()
{

}

void CFlashWnd::SetFlashColorOption(UINT ColorOp)
{
	m_FlashColorOp = ColorOp;	
}

void CFlashWnd::SetFlashBlendOption(DWORD SrcBlend, DWORD DestBlend)
{
	m_SrcBlend = SrcBlend;
	m_DestBlend = DestBlend;
}

void CFlashWnd::RenderProc()
{
	if(m_bIsActiveFlash)
	{
		m_bSwitchFlag = true;
		
		int ni_time = ( timeGetTime() % (int)( (m_fViewIntTime + m_fNoViewIntTime) * 1000.0f) ) 
					   / (m_fViewIntTime * 1000.0f);
		
		if( 1 == ni_time )
		{
			m_bSwitchFlag = false;		
		}
	}

	DWORD ColorOP, ColorARG1, ColorARG2;
	g_RenderManager.GetTextureStageState( 0, D3DTSS_COLOROP	 , &ColorOP );
	g_RenderManager.GetTextureStageState( 0, D3DTSS_COLORARG1	, &ColorARG1   );
	g_RenderManager.GetTextureStageState( 0, D3DTSS_COLORARG2	, &ColorARG2   );	
	
	g_RenderManager.SetTextureColorOP  ( 0 , m_FlashColorOp );
	g_RenderManager.SetTextureColorArg1( 0 , D3DTA_DIFFUSE );
	g_RenderManager.SetTextureColorArg2( 0 , D3DTA_TEXTURE );

	DWORD SrcBlend, DestBlend;
	g_RenderManager.GetRenderState( D3DRS_SRCBLEND, &SrcBlend );
	g_RenderManager.GetRenderState( D3DRS_DESTBLEND, &DestBlend );

	g_RenderManager.SetSrcBlend( m_SrcBlend );
	g_RenderManager.SetDestBlend( m_DestBlend );

	if(m_bSwitchFlag)
	{
		Render->Draw( Cimg, Cx, Cy, Cw, Ch, Ccolor );
	}

	g_RenderManager.SetSrcBlend( SrcBlend );
	g_RenderManager.SetDestBlend( DestBlend );

	g_RenderManager.SetTextureColorOP  ( 0 , ColorOP );
	g_RenderManager.SetTextureColorArg1( 0 , ColorARG1 );
	g_RenderManager.SetTextureColorArg2( 0 , ColorARG2 );
}

void CFlashWnd::SetIntervalTime(float fTime)
{
	m_fViewIntTime  = fTime;
}

void CFlashWnd::SetActiveFlash(bool bflag)
{
	m_bIsActiveFlash = bflag;

	if(!m_bIsActiveFlash)
	{
		m_bSwitchFlag = true;
		m_fCumulativeTime = 0.0f;
	}	
}