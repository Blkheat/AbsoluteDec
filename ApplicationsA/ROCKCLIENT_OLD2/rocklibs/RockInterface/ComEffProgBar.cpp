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

#include "ComEffProgBar.h"

#include "..\\..\\CRenderManager.h"

CComEffProgBar::CComEffProgBar( SDesktop* DT ) : CProgressBar(DT)
{
	pComEffImgBox[0] = NULL;
	pComEffImgBox[1] = NULL;
	pComEffImgBox[2] = NULL;
	pComEffImgBox[3] = NULL;
	pComEffImgBox[4] = NULL;

	m_iNowLevel = 0;
	m_iPrevGPercent = 0;

	m_bSwitchComLevel = false;
	m_bRenderGlow = false;
	m_fGlowTime = 0.1f;
	m_fGCumulTime = 0.0f;

	m_IsInit = false;
}

CComEffProgBar::~CComEffProgBar()
{

}

void CComEffProgBar::InitData()
{
	m_IsInit = true;
	
	m_iNowLevel = 0;
	m_iPrevGPercent = 0;

	m_bSwitchComLevel = false;
	m_bRenderGlow = false;
	m_fGlowTime = 0.1f;
	m_fGCumulTime = 0.0f;
	
	m_bFlashFlag = true;
	m_fFCumulTime = 0.0f;

	m_si = 0;
	m_ei = 0;
}
	
void CComEffProgBar::RenderProc()
{
	if( !m_IsInit )
	{
		InitData();	
	}

	m_fFInterTime = 0.1f;
	
	DWORD SrcBlend, DestBlend;
	g_RenderManager.GetRenderState( D3DRS_SRCBLEND, &SrcBlend );
	g_RenderManager.GetRenderState( D3DRS_DESTBLEND, &DestBlend );

	DWORD ColorOP, ColorARG1, ColorARG2; 

    g_RenderManager.GetTextureStageState( 0, D3DTSS_COLOROP		, &ColorOP );
    g_RenderManager.GetTextureStageState( 0, D3DTSS_COLORARG1	, &ColorARG1   );
    g_RenderManager.GetTextureStageState( 0, D3DTSS_COLORARG2	, &ColorARG2   );

	int iCenX = Cx + Cw/2;
	int iCenY = Cy + Ch/2;

	int iPosX = iCenX - ( 169/2 );
	int iPosY = iCenY - ( 17/2 );
		
	if(m_bSwitchComLevel && !m_bRenderGlow)  //콤보 레벨이 전환되는 시점
	{
		if( m_bFlashFlag )
		{
			g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_MODULATE4X );
			g_RenderManager.SetTextureColorArg1( 0 , D3DTA_DIFFUSE );
			g_RenderManager.SetTextureColorArg2( 0 , D3DTA_TEXTURE );

			Render->Draw( TID_None, iPosX, iPosY, 169, 17, 
				D3DCOLOR_ARGB( 125, 0, 125, 255 ) );

			g_RenderManager.SetTextureColorOP  ( 0 , ColorOP );
			g_RenderManager.SetTextureColorArg1( 0 , ColorARG1 );
			g_RenderManager.SetTextureColorArg2( 0 , ColorARG2 );
		}

		g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
		g_RenderManager.SetDestBlend( D3DBLEND_ONE );


		SetPercent( 100 );
		Bsize[2] = ProgressRate;
		Render->Draw( Bimg, Bsize[0], Bsize[1], Bsize[2], Bsize[3], Bcolor );

		g_RenderManager.SetSrcBlend( SrcBlend );
		g_RenderManager.SetDestBlend( DestBlend );

		m_fFCumulTime += g_GTimer.GetFrameElapsedSecTime();

		if( m_fFCumulTime > m_fFInterTime )
		{
			m_bFlashFlag = !m_bFlashFlag;
			m_fFCumulTime -= m_fFInterTime;			
		}	
	}
	else
	{
		//번쩍이는 효과적용
		if(m_bRenderGlow)
		{
			if( m_bSwitchComLevel )
			{
				SetPercent( 100 );
			}
			else
			{
				SetPercent( m_iNowGPercent );
			}

			Bsize[2] = ProgressRate;			
		
			g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
			g_RenderManager.SetDestBlend( D3DBLEND_ONE );

			Render->Draw( Bimg, Bsize[0], Bsize[1], Bsize[2], Bsize[3], Bcolor );

			g_RenderManager.SetSrcBlend( SrcBlend );
			g_RenderManager.SetDestBlend( DestBlend );

			SetPercent( m_iPrevGPercent );
			Bsize[2] = ProgressRate;
		
			if( !m_bSwitchComLevel )
			Render->Draw( Bimg, Bsize[0], Bsize[1], Bsize[2], Bsize[3], Bcolor );

			m_fGCumulTime += g_GTimer.GetFrameElapsedSecTime();
			if(m_fGCumulTime > m_fGlowTime)
			{
				m_fGCumulTime = 0.0f;
				m_bRenderGlow = false;
				m_iPrevGPercent = m_iNowGPercent;
				m_bSwitchComLevel = false;
			}	
		}	
		else
		{
			SetPercent( m_iNowGPercent );
			Bsize[2] = ProgressRate;
			Render->Draw( Bimg, Bsize[0], Bsize[1], Bsize[2], Bsize[3], Bcolor );	
			m_iPrevGPercent = m_iNowGPercent;
		}	
	}	
}

void CComEffProgBar::SetSkillGauge(int Gauge)
{
	int Level = 0;
	
	// 총 게이지 칸 => 5(1) + 5(2) + 5(3) + 5(4) + 5(5) + 5(Ex) = 30 
	if( Gauge >= 5 && Gauge < 10 ) Level = 1;
	else if( Gauge >= 10 && Gauge < 15 ) Level = 2;
	else if( Gauge >= 15 && Gauge < 20 ) Level = 3;
	else if( Gauge >= 20 && Gauge < 25 ) Level = 4;
	else if( Gauge >= 25 && Gauge < 30 ) Level = 5;
	else if( Gauge >= 30 ) Level = 6;	// Ex Level
	
	m_iNowGPercent = 0;

	switch( Level )
	{
	case 0:
		{
			m_iNowGPercent = Gauge * 20;						// 0 - 5
		}
		break;
	case 1:
		{
			m_iNowGPercent = (Gauge - 5) * 20;					// 5 - 10
		}
		break;
	case 2:
		{
			m_iNowGPercent = (Gauge - 10) * 20;					// 10 - 15
		}
		break;
	case 3:
		{
			m_iNowGPercent = (Gauge - 15) * 20;					// 15 - 20
		}
		break;
	case 4:
		{
			m_iNowGPercent = (Gauge - 20) * 20;					// 20 - 25
		}
		break;
	case 5:
		{
			m_iNowGPercent = (Gauge - 25) * 20;				    // 25 - 30
		}
		break;
	case 6:
		{
			m_iNowGPercent = 100;								// 30 - 
		}
		break;
	default:
		break;
	}	
	
	int InterLevel = Level - m_iNowLevel;	
	int i = 0;
	
	if( InterLevel > 0 )		//콤보레벨이 올라갈때
	{
		if(Level == 6)
		{
			m_ComEffInfo.IsMax = true;
			m_ComEffInfo.IsPushEffect = true;			
			m_si = 0;
			m_ei = 5;

			for(int i = m_si ; i < m_ei ; i++)
			{
				pComEffImgBox[i]->InsertComEffInfo(m_ComEffInfo);
			}			

			m_bRenderGlow = true;
			m_bSwitchComLevel = true;
		}
		else
		{
			m_ComEffInfo.IsMax = false;
			m_ComEffInfo.IsPushEffect = true;
			m_si = m_iNowLevel;
			m_ei = Level;
			
			for(int i = m_si ; i < m_ei ; i++)
			{
				pComEffImgBox[i]->InsertComEffInfo(m_ComEffInfo);
			}				
			
			m_bSwitchComLevel = true;			
		}	
	}
	else if( InterLevel < 0 )	//콤보레벨이 떨어질때
	{
		if( m_iNowLevel == 6 )
		{
			m_ComEffInfo.IsMax = true;
			m_ComEffInfo.IsPushEffect = false;
			m_si = 0;
			m_ei = 5;			
			
			for(i = m_si ; i < m_ei ; i++)
			{
				pComEffImgBox[i]->InsertComEffInfo(m_ComEffInfo);
			}
			
		}
		else
		{
			m_ComEffInfo.IsMax = false;
			m_ComEffInfo.IsPushEffect = false;
			m_si = Level;
			m_ei = m_iNowLevel;
			
			for(i = m_si ; i < m_ei ; i++)
			{
				pComEffImgBox[i]->InsertComEffInfo(m_ComEffInfo);
			}
		}	
	}
	else	//콤보 레벨이 같을 경우
	{
		if( m_iPrevGPercent < m_iNowGPercent)
			m_bRenderGlow = true;				
	}
	
	m_iNowLevel = Level;
}

