#include "..\\..\\RockClient.h"
#include "..\\RockPCH.h"

#include "Rui.h"
#include "..\\..\\GTimer.h"

#include "ComboStepBar.h"

#include "..\\..\\CRenderManager.h"

CComboStepBar::CComboStepBar( SDesktop* DT ) : CWnd(DT)
{
	m_nMaxStep = 0;
	m_pIsStepGlow = NULL;
	m_fStepGlowtime = 0.15f;
	m_fStepGlowCumtime = 0.0f;

	m_IsSwitchComLevel = false;
	m_fSwitchComGlowtime = 0.15f;
	m_fSwitchComCumtime = 0.0f;

	m_nNowStep = 0;
	m_nGabPixelW = 0;

	m_nNowLevel = 0;

	m_StepTID = TID_None;
	m_StepEffTID = TID_None;

	m_StepCw = 0;
	m_StepCh = 0;

	m_EffTID = TID_None;
	m_EffCx = 0;
	m_EffCy = 0;
	m_EffCw = 0;
	m_EffCh = 0;

	pComEffImgBox[0] = NULL;
	pComEffImgBox[1] = NULL;
	pComEffImgBox[2] = NULL;
	pComEffImgBox[3] = NULL;
	pComEffImgBox[4] = NULL;
	pComEffImgBox[5] = NULL;
}

CComboStepBar::~CComboStepBar()
{
	SAFE_DELETE_ARRAY(m_pIsStepGlow);
}

void CComboStepBar::InitData()
{
	m_nNowLevel = 0;

	memset(m_pIsStepGlow, 0, sizeof(bool)*m_nMaxStep);
	m_fStepGlowCumtime = 0.0f;

	m_IsSwitchComLevel = false;
	m_fSwitchComCumtime = 0.0f;

	m_si = 0;
	m_ei = 0;
}

void CComboStepBar::RenderProc()
{
	
	if(m_IsSwitchComLevel)
	{
		RenderSwitchComLevel();
	}	
	
	int nPosX = Cx;
	int nPosY = Cy;

	bool bStepGlowFlag = false;
	bool bStepGlowEndFlag = false;	
	
	for(int i = 0; i < m_nNowStep ; i++)
	{
	   if(m_pIsStepGlow[i])
	   {
		   Render->Draw( m_StepEffTID, nPosX, nPosY, m_StepCw, m_StepCh, Ccolor );
		   bStepGlowFlag = true;
		   
		   if(m_fStepGlowCumtime > m_fStepGlowtime)
		   {
			   m_pIsStepGlow[i] = false;
			   bStepGlowEndFlag = true;
		   }
	   }
	   else	
	   {
		   Render->Draw( m_StepTID, nPosX, nPosY, m_StepCw, m_StepCh, Ccolor );
	   }

	   nPosX += (m_nGabPixelW + m_StepCw);
	}

	if(bStepGlowFlag)
	{
		m_fStepGlowCumtime += g_GTimer.GetFrameElapsedSecTime();
	}

	if(bStepGlowEndFlag)
	{
		m_fStepGlowCumtime = 0.0f;
	}
}

void CComboStepBar::RenderSwitchComLevel()
{
	int nWdith = 0;
	m_fSwitchComCumtime += g_GTimer.GetFrameElapsedSecTime();	

	float fRatio = m_fSwitchComCumtime / m_fSwitchComGlowtime;
	if(m_fSwitchComCumtime > m_fSwitchComGlowtime)
	{
		m_fSwitchComCumtime = 0.0f;
		fRatio = 0.0f;
		m_IsSwitchComLevel = false;
	}

	D3DCOLOR Color = D3DCOLOR_ARGB((int)( (1.0f - fRatio) * 255), 255, 255, 255 );
	nWdith = m_EffCw + ( fRatio * (100));
	//nWdith = m_EffCw;

	DWORD SrcBlend, DestBlend;
	g_RenderManager.GetRenderState( D3DRS_SRCBLEND, &SrcBlend );
	g_RenderManager.GetRenderState( D3DRS_DESTBLEND, &DestBlend );
	
	g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
	g_RenderManager.SetDestBlend( D3DBLEND_DESTALPHA );
	
	Render->ScretchDraw( m_EffTID , m_EffCx, m_EffCy, nWdith, m_EffCh, Color );

	g_RenderManager.SetSrcBlend( SrcBlend );
	g_RenderManager.SetDestBlend( DestBlend );
}

void CComboStepBar::SetMaxStep(int nStep)
{
	m_nMaxStep = nStep;
	m_pIsStepGlow = SAFE_NEW_ARRAY( bool , m_nMaxStep );	
	memset(m_pIsStepGlow, 0, sizeof(bool)*m_nMaxStep);
}

void CComboStepBar::SetNowStep(int nStep)
{
	if( nStep > m_nMaxStep )
	{
		nStep = m_nMaxStep; 
	}
	
	m_nNowStep = nStep;
}

void CComboStepBar::SetGabPixelW(int nCw)
{
	m_nGabPixelW = nCw;
}

void CComboStepBar::SetStepTid(TEXID uTid)
{
	m_StepTID = uTid;
}

void CComboStepBar::SetStepEffTid(TEXID uTid)
{
	m_StepEffTID = uTid;
}

void CComboStepBar::SetStepSize(int nCw, int nCh)
{
    m_StepCw = nCw;
	m_StepCh = nCh;
}

void CComboStepBar::SetEffTID(TEXID uTid)
{
	m_EffTID = uTid;
}

void CComboStepBar::SetSwitchComLevel(bool bFlag)
{
	m_IsSwitchComLevel = bFlag;
	m_fSwitchComCumtime = 0.0f;
}

void CComboStepBar::SetEffSize(int nCx, int nCy, int nCw, int nCh)
{
	m_EffCx = Cx + nCx;
	m_EffCy = Cy + nCy;
	m_EffCw = nCw;
	m_EffCh = nCh;	
}

void CComboStepBar::SetSkillGauge(int nGauge)
{
	int nLevel = 0;
	int nNowStep = 0;
	
	// 총 게이지 칸 => 5(1) + 5(2) + 5(3) + 5(4) + 5(5) + 5(Ex) = 30 
	if( nGauge >= 5 && nGauge < 10 ) nLevel = 1;
	else if( nGauge >= 10 && nGauge < 15 ) nLevel = 2;
	else if( nGauge >= 15 && nGauge < 20 ) nLevel = 3;
	else if( nGauge >= 20 && nGauge < 25 ) nLevel = 4;
	else if( nGauge >= 25 && nGauge < 30 ) nLevel = 5;
	else if( nGauge >= 30 ) nLevel = 6;	// Ex Level

	switch( nLevel )
	{
	case 0:
		{
			nNowStep = nGauge;						// 0 - 5
		}
		break;
	case 1:
		{
			nNowStep = nGauge - 5;					// 5 - 10
		}
		break;
	case 2:
		{
			nNowStep = nGauge - 10;					// 10 - 15
		}
		break;
	case 3:
		{
			nNowStep = nGauge - 15;					// 15 - 20
		}
		break;
	case 4:
		{
			nNowStep = nGauge - 20;					// 20 - 25
		}
		break;
	case 5:
		{
			nNowStep = nGauge - 25;				    // 25 - 30
		}
		break;
	case 6:
		{
			nNowStep = 5;							// 30 - 
		}
		break;
	default:
		break;
	}

	int nInterLevel = nLevel - m_nNowLevel;	
	int i = 0;

	if( nInterLevel > 0 )		//콤보레벨이 올라갈때
	{
		m_ComEffInfo.IsMax = false;
		m_ComEffInfo.IsPushEffect = true;
		m_si = m_nNowLevel;
		m_ei = nLevel;
			
		for(int i = m_si ; i < m_ei ; i++)
		{
			pComEffImgBox[i]->InsertComEffInfo(m_ComEffInfo);
		}			
	}
	else if( nInterLevel < 0 )	//콤보레벨이 떨어질때
	{
		m_ComEffInfo.IsMax = false;
		m_ComEffInfo.IsPushEffect = false;
		m_si = nLevel;
		m_ei = m_nNowLevel;
			
		for(i = m_si ; i < m_ei ; i++)
		{
			pComEffImgBox[i]->InsertComEffInfo(m_ComEffInfo);
		}
	}
	else	//콤보 레벨이 같을 경우
	{
		if( m_nNowStep < nNowStep)
		{
			for( i = m_nNowStep; i <= nNowStep ; i++ )
			{
				if(i == m_nNowStep)
				{
					continue;
				}				
				m_pIsStepGlow[i-1] = true;				
			}
		}		   				
	}

	SetNowStep(nNowStep);
	m_nNowLevel = nLevel;
}