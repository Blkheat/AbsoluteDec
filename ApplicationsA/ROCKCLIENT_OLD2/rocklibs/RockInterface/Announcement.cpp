#include "RockPCH.h"
#include "Rui.h"
#include "Render.h"

#include "..\\..\\GTimer.h"
#include "..\\..\\bravolibs\\Sound\\Sound.h"

#include "LoadingWndProc.h"
#include "Announcement.h"
#include "stringmanager.h"
CAnnouncement	g_Announcement;

CAnnouncement::CAnnouncement()
{
	m_pFontg	 = NULL;
	m_pDesktop   = NULL;

	m_fTransTime = 0.5f;
	m_fRndertime = 30.0f;

	m_fACumTime[AIN] = 0.0f;
	m_fACumTime[AOUT] = 0.0f;
}

CAnnouncement::~CAnnouncement()
{

}

void CAnnouncement::Init(SDesktop *pDesktop)
{
	m_pDesktop = pDesktop;
	
#ifdef CHINA_VERSION
	m_pFontg = Fontman->GetFont( n_ftDotum , 15 , n_fwBold, n_fdUserColor , true , 7 );
#else
	m_pFontg = Fontman->GetFont( n_ftDotum, 20, n_fwBold, n_fdOutline , true , 5  );
#endif
	
#ifdef CHINA_VERSION
	m_ATxtData[AIN].EPos.y = m_ATxtData[AOUT].SPos.y = 73;
#else 
	m_ATxtData[AIN].EPos.y = m_ATxtData[AOUT].SPos.y = 150;
#endif

	m_ATxtData[AIN].SPos.y = m_ATxtData[AIN].EPos.y + 30;
	m_ATxtData[AOUT].EPos.y = m_ATxtData[AOUT].SPos.y - 30;
}

void  CAnnouncement::UpdateProc()
{
	float fRatio = 0.0f;	
	
	if( m_ATxtData[AIN].IsRender && (m_ATxtData[AIN].NowState == AMOVE) )
	{
		m_fACumTime[AIN] += g_GTimer.GetFrameElapsedSecTime();
		float fRatio = m_fACumTime[AIN] / m_fTransTime;
		if( m_fACumTime[AIN] > m_fTransTime )
		{
			fRatio = 1.0f;
			m_fACumTime[AIN] = 0.0f;
			m_ATxtData[AIN].NowState = AVIEW;
		}

		m_ATxtData[AIN].TextData.Rcolor[A] = (int)( fRatio * 255 );
		m_ATxtData[AIN].NPos.y = m_ATxtData[AIN].SPos.y 
								 + ( fRatio * (m_ATxtData[AIN].EPos.y - m_ATxtData[AIN].SPos.y ) );
	}

	if( m_ATxtData[AIN].IsRender && (m_ATxtData[AIN].NowState == AVIEW) )
	{
		m_fACumTime[AIN] += g_GTimer.GetFrameElapsedSecTime();		
		if( m_fACumTime[AIN] > m_fRndertime )
		{
			m_ATxtData[AOUT].TextData = m_ATxtData[AIN].TextData;
			m_ATxtData[AOUT].NPos = m_ATxtData[AIN].NPos;		
			m_ATxtData[AOUT].IsRender = true;
			m_ATxtData[AOUT].NowState = AMOVE;
			m_fACumTime[AOUT] = 0.0f;

			m_ATxtData[AIN].IsRender = false;
			m_ATxtData[AIN].NowState = ANONE;
		}		
	}

	if( m_ATxtData[AOUT].IsRender && (m_ATxtData[AOUT].NowState == AMOVE) )
	{
		m_fACumTime[AOUT] += g_GTimer.GetFrameElapsedSecTime();
		float fRatio = m_fACumTime[AOUT] / m_fTransTime;
		if( m_fACumTime[AOUT] > m_fTransTime )
		{
			fRatio = 1.0f;
			m_fACumTime[AOUT] = 0.0f;
			m_ATxtData[AOUT].IsRender = false;
			m_ATxtData[AOUT].NowState = ANONE;
		}

		m_ATxtData[AOUT].TextData.Rcolor[A] = 255 - (int)( fRatio * 255 );
		m_ATxtData[AOUT].NPos.y = m_ATxtData[AOUT].SPos.y 
								 + ( fRatio * (m_ATxtData[AOUT].EPos.y - m_ATxtData[AOUT].SPos.y ) );
	}


}

void  CAnnouncement::RenderProc()
{
	if( LoadingBarWnd.IsProgress == true )
	{
		return;
	}
	
	UpdateProc();
	
	D3DCOLOR Ccolor;
	unsigned char * Rcolor = NULL;
	
	if(m_ATxtData[AOUT].IsRender)
	{
		Rcolor = m_ATxtData[AOUT].TextData.Rcolor;
		Ccolor = D3DCOLOR_ARGB( Rcolor[A], Rcolor[R], Rcolor[G], Rcolor[B] );
		Render->DrawText( m_pFontg, m_ATxtData[AOUT].TextData.Text, 
						  m_ATxtData[AOUT].NPos.x, m_ATxtData[AOUT].NPos.y, Ccolor );
	}
	
	if(m_ATxtData[AIN].IsRender)
	{
		Rcolor = m_ATxtData[AIN].TextData.Rcolor;
		Ccolor = D3DCOLOR_ARGB( Rcolor[A], Rcolor[R], Rcolor[G], Rcolor[B] );
		Render->DrawText( m_pFontg, m_ATxtData[AIN].TextData.Text, 
						  m_ATxtData[AIN].NPos.x, m_ATxtData[AIN].NPos.y, Ccolor );
	}
}

void  CAnnouncement::InsertAnnouncement(TCHAR *pStr ,bool bFlag /* = TRUE */)
{
	
	TCHAR strTemp[256] = {0,};
	
	if( bFlag == true)
	{
		Rsprintf( RWCHAR(strTemp),G_STRING_CHAR( IDS_NOTICE ), RWCHAR(pStr));
		//이 부분에서 효과음 호출
#ifndef TAIWAN_VERSION
		g_Sound.Play( 257 );
#endif 

	}
	else 
	{
		Rsprintf(RWCHAR(strTemp), _RT("%s"), RWCHAR(pStr));
	}

	if(m_ATxtData[AIN].IsRender)	//이미 공지사항이 렌더링되고 있으면
	{
		m_ATxtData[AOUT].TextData = m_ATxtData[AIN].TextData;
		m_ATxtData[AOUT].NPos = m_ATxtData[AIN].NPos;		
		m_ATxtData[AOUT].IsRender = true;
		m_ATxtData[AOUT].NowState = AMOVE;
		m_fACumTime[AOUT] = 0.0f;
		m_ATxtData[AIN].IsRender = false;
		m_ATxtData[AIN].NowState = ANONE;
	}
	
	SAFE_STR_CPY( m_ATxtData[AIN].TextData.Text, strTemp, 256 );
	m_pFontg->GetStringInfo(m_ATxtData[AIN].TextData.Text, &m_StrInfo);	
	m_ATxtData[AIN].NPos.x = (m_pDesktop->Width - m_StrInfo.Width) / 2;
	m_ATxtData[AIN].IsRender = true;
	m_ATxtData[AIN].NowState = AMOVE;
	
	m_fACumTime[AIN] = 0.0f;
}

void  CAnnouncement::LogOff()
{
	m_ATxtData[AIN].IsRender = false;
	m_ATxtData[AOUT].IsRender = false; 	
}

bool  CAnnouncement::IsRenderProc()
{
	if( m_ATxtData[AIN].IsRender || m_ATxtData[AOUT].IsRender )
	{
		return true;
	}

	return false;
}
