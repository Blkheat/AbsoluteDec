#include "..\\..\\RockClient.h"
#include "..\\RockPCH.h"

#include "..\\..\\quadlist.h"
#include "..\\..\\bravolibs\\network\\Protocol.h"
#include "..\\..\\Pc.h"

#include "..\\..\\GTimer.h"

#include "Rui.h"
#include "BZoneIN2DEffect.h"

#ifdef DIRECT_VERSION_9_MJH
extern IDirect3DDevice9*	g_lpDevice;
#else
extern IDirect3DDevice8*	g_lpDevice;
#endif // DIRECT_VERSION_9_MJH

CBZoneIn2DEffect::CBZoneIn2DEffect()
{
	//레디 연출 관련
	m_uReadyState = N2DEffect_None;
	m_uRedayImg = TID_None;
	m_sReadyMoveRect = SRect(0, 0, 276, 44);
	m_sReadyNomalRect = SRect(0, 0, 238, 44);
	m_fMovetime = 0.15f;
	m_fMCumtime = 0.0f;
	m_nReadyAlpha = 255;
	
	m_fReadyFadetime = 0.25f;
	m_fRedayFCumtime = 0.0f;
	

	//스타트 연출 관련
	m_uStartState = N2DEffect_None;			
	m_uStartImg = TID_None;			
	m_sStartMiniRect = SRect(0, 0, 442, 87);		
	m_sStartNormalRect = SRect(0, 0, 396, 78);
	m_fSizetime = 0.3f;			
	m_fSCumtime = 0.0f;			
	m_nStartAlpha = 255;

	m_fStartViewtime = 0.9f;
	m_fStartVCumtime = 0.0f;

	m_fStartFadetime = 0.25f;
	m_fStartFCumtime = 0.0f;
	
	m_bIsGlow = false;				
	m_fGlowtime = 0.1f;			
	m_fGCumtime = 0.0f;

}

CBZoneIn2DEffect::~CBZoneIn2DEffect()
{

}

void CBZoneIn2DEffect::InitEffect()
{
	InitReadyEffect();
	
	InitStartEffect();
}

void CBZoneIn2DEffect::InitReadyEffect()
{
	m_uReadyState = N2DEffect_None;
	m_uRedayImg = TID_None;
	m_fMCumtime = 0.0f;
	m_nReadyAlpha = 255;

	m_fRedayFCumtime = 0.0f;
}

void CBZoneIn2DEffect::InitStartEffect()
{
	m_uStartState = N2DEffect_None;
	m_uStartImg = TID_None;
	m_fSCumtime = 0.0f;			
	m_nStartAlpha = 255;

	m_bIsGlow = false;
	m_fGCumtime = 0.0f;

	m_fStartFCumtime = 0.0f;

	m_sStartCenPoint.x = nRui->Desktop.Width/2;
	m_sStartCenPoint.y = 384;
}

void CBZoneIn2DEffect::SetReadyEffect(UINT uEffectType)
{
	InitReadyEffect();
	m_uReadyState = uEffectType;
}

void CBZoneIn2DEffect::SetStartEffect(UINT uEffectType)
{
	InitStartEffect();
	m_uStartState = uEffectType;
}
	
void CBZoneIn2DEffect::UpdateProc()
{
	if( m_uReadyState != N2DEffect_None )
	{
		UpdateReadyProc();	
	}

	if( m_uStartState != N2DEffect_None )
	{
		UpdateStartProc();	
	}
}

void CBZoneIn2DEffect::RenderProc()
{
	if( m_uReadyState != N2DEffect_None )
	{
		RenderReadyProc();	
	}

	if( m_uStartState != N2DEffect_None )
	{
		RenderStartProc();	
	}
}

void CBZoneIn2DEffect::UpdateReadyProc()
{
	switch(m_uReadyState)
	{
		case N2DEffect_Trans:
			{
				UpdateReadyTransProc();
			}
			break;
		case N2DEffect_View:
			{
				UpdateReadyViewProc();
			}
			break;
		case N2DEffect_Hide:
			{	
				UpdateReadyHideProc();
			}
			break;
	}
}

void CBZoneIn2DEffect::UpdateReadyTransProc()
{
	//레디 이동부
	SPoint2 sPos = SPoint2(10, 295);
	int nEndx = (nRui->Desktop.Width - 276)/2 + 10;		
	SPoint2 ePos = SPoint2(nEndx, 295);

	m_fMCumtime += g_GTimer.GetFrameElapsedSecTime();

	float fRatio = m_fMCumtime / m_fMovetime;
	
	if( m_fMCumtime > m_fMovetime )
	{
		m_fMCumtime = 0.0f;
		m_uReadyState= N2DEffect_View;				
	}

	m_sReadyRenderRect = m_sReadyMoveRect;
	m_sReadyRenderRect.x = sPos.x + ( fRatio * (ePos.x - sPos.x) );
	m_sReadyRenderRect.y = sPos.y;

	m_uRedayImg = TID_GAMEREADY_MOVING_IMG;
}

void CBZoneIn2DEffect::UpdateReadyViewProc()
{
	m_sReadyRenderRect = m_sReadyNomalRect;
	m_sReadyRenderRect.x = (nRui->Desktop.Width - m_sReadyRenderRect.w)/2;
	m_sReadyRenderRect.y = 295;

	m_uRedayImg = TID_GAMEREADY_NORMAL_IMG;
}

void CBZoneIn2DEffect::UpdateReadyHideProc()
{
	m_fRedayFCumtime += g_GTimer.GetFrameElapsedSecTime();
	float fRatio = m_fRedayFCumtime/m_fReadyFadetime;
	
	if(m_fRedayFCumtime > m_fReadyFadetime)
	{
		m_uReadyState = N2DEffect_None;
		m_fRedayFCumtime = 0;
		fRatio = 0.0f;
	}	
	
	m_nReadyAlpha = (int)((1.0f - fRatio) * 255);
	m_uRedayImg = TID_GAMEREADY_NORMAL_IMG;
}

void CBZoneIn2DEffect::UpdateStartProc()
{
	switch(m_uStartState)
	{
		case N2DEffect_Trans:
			{
				UpdateStartTransProc();
			}
			break;
		case N2DEffect_View:
			{
				UpdateStartViewProc();
			}
			break;
		case N2DEffect_Hide:
			{
				UpdateStartHideProc();	
			}
			break;
	}
}

void CBZoneIn2DEffect::UpdateStartTransProc()
{
	//스타트 초기 
	int nsWidth = 766 * 2;
	int nsHeight = 151 * 2;
	int neWidth = (int)(442 / 1.5f);
	int neHeight = (int)(87 / 1.5f);	

	m_fSCumtime += g_GTimer.GetFrameElapsedSecTime();

	float fRatio = m_fSCumtime / m_fSizetime;
	
	if( m_fSCumtime > m_fSizetime )
	{
		m_fSCumtime = 0.0f;
		m_uStartState= N2DEffect_View;
		m_bIsGlow = true;
	}

	m_sStartRenderRect.w  =  nsWidth - ( fRatio * (nsWidth - neWidth) );
	m_sStartRenderRect.h =  nsHeight - ( fRatio * (nsHeight - neHeight) );

	if((m_sStartRenderRect.w % 2) != 0)
		m_sStartRenderRect.w += 1;
	if((m_sStartRenderRect.h % 2) != 0)
		m_sStartRenderRect.h += 1;

	m_sStartRenderRect.x = m_sStartCenPoint.x - ( m_sStartRenderRect.w/2 );
	m_sStartRenderRect.y = m_sStartCenPoint.y - ( m_sStartRenderRect.h/2 );

	m_uStartImg = TID_GAMESTART_MINIMIZE_IMG;	
}

void CBZoneIn2DEffect::UpdateStartViewProc()
{
	m_sStartRenderRect = m_sStartNormalRect;
	m_sStartRenderRect.x = m_sStartCenPoint.x - ( m_sStartRenderRect.w/2 );
	m_sStartRenderRect.y = m_sStartCenPoint.y - ( m_sStartRenderRect.h/2 );
	
	if(m_bIsGlow)
	{
		m_uStartImg = TID_GAMESTART_GLOW_IMG;
		m_fGCumtime += g_GTimer.GetFrameElapsedSecTime();

		if(m_fGCumtime > m_fGlowtime)
		{
			m_fGCumtime = 0.0f;
			m_bIsGlow = false;
		}
	}
	else
	{
		m_uStartImg = TID_GAMESTART_NORMAL_IMG;
	}
	
	m_fStartVCumtime += g_GTimer.GetFrameElapsedSecTime();
	
	if(m_fStartVCumtime > m_fStartViewtime)
	{
		m_fStartVCumtime = 0.0f;
		m_uStartState= N2DEffect_Hide;
		m_uReadyState= N2DEffect_Hide;
	}
}

void CBZoneIn2DEffect::UpdateStartHideProc()
{
	m_fStartFCumtime += g_GTimer.GetFrameElapsedSecTime();
	float fRatio = m_fStartFCumtime/m_fStartFadetime;
	
	if(m_fStartFCumtime > m_fStartFadetime)
	{
		m_uStartState = N2DEffect_None;
		m_fStartFCumtime = 0;
		fRatio = 0.0f;
	}	
	
	m_nStartAlpha = (int)((1.0f - fRatio) * 255);
	m_uStartImg = TID_GAMESTART_NORMAL_IMG;
}

void CBZoneIn2DEffect::RenderReadyProc()
{
	D3DCOLOR uColor = D3DCOLOR_ARGB( m_nReadyAlpha, 255, 255, 255 );
	
	Render->Draw( m_uRedayImg, m_sReadyRenderRect.x, m_sReadyRenderRect.y, 
				  m_sReadyRenderRect.w, m_sReadyRenderRect.h, uColor);	
}

void CBZoneIn2DEffect::RenderStartProc()
{
	D3DCOLOR uColor = D3DCOLOR_ARGB( m_nStartAlpha, 255, 255, 255 );
	
	Render->ScretchDraw( m_uStartImg, m_sStartRenderRect.x, m_sStartRenderRect.y, 
				  m_sStartRenderRect.w, m_sStartRenderRect.h, uColor);
}