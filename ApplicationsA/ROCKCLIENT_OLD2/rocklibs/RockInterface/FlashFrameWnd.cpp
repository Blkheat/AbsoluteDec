#include "..\\..\\RockClient.h"
#include "..\\RockPCH.h"

#include "Rui.h"
#include "..\\..\\GTimer.h"
#include "FlashFrameWnd.h"

#include "..\\..\\CRenderManager.h"

CFlashFrameWnd::CFlashFrameWnd( SDesktop* DT ) : CFrameWnd(DT)
{
	m_bIsActiveFlash = false;
	m_FlashImg = TID_None;
	m_FlashColorOp = D3DTOP_MODULATE;
	m_SrcBlend = D3DBLEND_SRCALPHA;
	m_DestBlend = D3DBLEND_INVSRCALPHA;

	m_FlashColor = D3DCOLOR_ARGB(255, 255, 255, 255 );

	
	m_nRposX = 0;
	m_nRposY = 0;	
	m_nFImgWidth = 0;
	m_nFImgHeight = 0;

	m_bSwitchFlag = true;

	m_fIntervalTime = 0.2f;	
	m_bNoRender = false;
}

CFlashFrameWnd::~CFlashFrameWnd()
{


}

void CFlashFrameWnd::RenderProc()
{
	
	CFrameWnd::RenderProc();
	
	if(CExtraimg != TID_None)
	{
		Render->Draw( CExtraimg, Cx, Cy, Cw, Ch, Ccolor );	
	}

	if(m_bIsActiveFlash)
	{
		m_bSwitchFlag = true;
		
		int ni_time = ( timeGetTime() % (int)(m_fIntervalTime * 2000) ) 
					   / (m_fIntervalTime * 1000.0f);
		
		if( 1 == ni_time )
		{
			m_bSwitchFlag = false;		
		}				
		
		DWORD ColorOP, ColorARG1, ColorARG2;
		g_RenderManager.GetTextureStageState( 0, D3DTSS_COLOROP		, &ColorOP );
		g_RenderManager.GetTextureStageState( 0, D3DTSS_COLORARG1	, &ColorARG1   );
		g_RenderManager.GetTextureStageState( 0, D3DTSS_COLORARG2	, &ColorARG2   );	
		
		g_RenderManager.SetTextureColorOP  ( 0 ,m_FlashColorOp  );
		g_RenderManager.SetTextureColorArg1( 0 ,D3DTA_DIFFUSE  );
		g_RenderManager.SetTextureColorArg2( 0 ,D3DTA_TEXTURE  );

		DWORD SrcBlend, DestBlend;
		g_RenderManager.GetRenderState( D3DRS_SRCBLEND, &SrcBlend );
		g_RenderManager.GetRenderState( D3DRS_DESTBLEND, &DestBlend );

		g_RenderManager.SetSrcBlend( m_SrcBlend );
		g_RenderManager.SetDestBlend( m_DestBlend );
	
		if(m_bSwitchFlag && !m_bNoRender)
		{
			Render->ScretchDraw( m_FlashImg, (Cx + m_nRposX), (Cy + m_nRposY), 
						m_nFImgWidth, m_nFImgHeight, m_FlashColor );
		}

		g_RenderManager.SetSrcBlend( SrcBlend );
		g_RenderManager.SetDestBlend( DestBlend );

		g_RenderManager.SetTextureColorOP  ( 0 , ColorOP );
		g_RenderManager.SetTextureColorArg1( 0 , ColorARG1 );
		g_RenderManager.SetTextureColorArg2( 0 , ColorARG2 );
	}
}

void CFlashFrameWnd::SetFlashImage(UINT fImg)
{
	m_FlashImg = fImg;
}

void CFlashFrameWnd::SetFlashColor(D3DCOLOR color)
{
	m_FlashColor = color;
}

void CFlashFrameWnd::SetFlashColorOption(UINT ColorOp)
{
	m_FlashColorOp = ColorOp;	
}

void CFlashFrameWnd::SetIntervalTime(float fTime)
{
	m_fIntervalTime = fTime;
}

void CFlashFrameWnd::SetFlashBlendOption(DWORD SrcBlend, DWORD DestBlend)
{
	m_SrcBlend = SrcBlend;
	m_DestBlend = DestBlend;
}

void CFlashFrameWnd::SetFlashImgSize(int nRposX, int nRposY, int width, int height)
{
	m_nRposX = nRposX;
	m_nRposY = nRposY;
	m_nFImgWidth = width;	
	m_nFImgHeight = height;	
}

void CFlashFrameWnd::SetActiveFlash(bool bflag)
{
	m_bIsActiveFlash = bflag;

	if(!m_bIsActiveFlash)
	{
		m_bSwitchFlag = true;		
	}	
}