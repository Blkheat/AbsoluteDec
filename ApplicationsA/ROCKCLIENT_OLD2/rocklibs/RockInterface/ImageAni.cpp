///////////////////////////////////////////////////////////////////////////////
///
///		File		: ImageAni.cpp
///		Desc		:
///
///		Author		: Park Soo-Hyun
///		Team		: Program - Client Team
///		Date		: 
///
///		Copyright(c) 2004, Rocksoft Co., Ltd. ALL RIGHTS RESERVED
///
///////////////////////////////////////////////////////////////////////////////
#include "ImageAni.h"
#include "..\\..\\CRenderManager.h"
#include "Rui.h"

#ifdef DIRECT_VERSION_9_MJH
extern IDirect3DDevice9*	g_lpDevice;
#else
extern IDirect3DDevice8*	g_lpDevice;
#endif // DIRECT_VERSION_9_MJH

extern long		g_nowTime;

//-----------------------------------------------------------------------------
CImageAni::CImageAni( SDesktop* DT ) : CTextBox( DT )
{

	m_pNextImageAni = NULL;
	m_bNextLink = false;
	m_ImgCount = 0 ; 
	m_iMaxImgCount = 0 ; 

	Clear();
}

//-----------------------------------------------------------------------------
CImageAni::~CImageAni()
{
}

void CImageAni::Clear()
{
	m_IntervalTime = 1;
	m_LimitTime = -1;
	m_dwLimitOldTime = timeGetTime();
	m_dwOldTimer = timeGetTime();
	m_Play = false;
	m_NextImg =0;
	m_fSize = 0.0f;
	m_fScale = 0.0f ;
	m_bRepeat = true; 
	m_EndSprite = false;
#ifdef ELDA_ANIFRAME_SYNC
	m_nAnimTime	= 0;
#endif	//	ELDA_ANIFRAME_SYNC
	
}

//기본 정보 초기화
void	CImageAni::SetInfoSprite(_EFFMODE Mode , int MaxImgNum, DWORD LimitTime , DWORD Interval , int MaxImgCount , bool bStopImg /* =false  */)
{
	m_bStopImg = bStopImg;
	m_effMode = Mode;
	m_MaxImgNum = MaxImgNum;
	m_LimitTime = LimitTime;
	m_IntervalTime = Interval;

	m_iMaxImgCount =  MaxImgCount;

	if( m_LimitTime == -1)
	{
		m_LimitTime = 999999999;
	}
	if(MaxImgCount == -1 )	
	{
		m_iMaxImgCount =999999999;
	}


}
//기본 사이즈 보다 크게 하거나 축소 시킬때.. 
void CImageAni::SetSize(int MaxSize , float Scale) 
{
	m_iMaxSize = MaxSize;
	m_fScale = Scale;
};


#ifdef ELDA_ANIFRAME_SYNC
void CImageAni::RenderProc()
{
	
	//총 로테이션 시간 
	// 몇초만 플레이 하고 사라져라 
	
	
	int RealCx = Cx;
	int RealCy = Cy;
	int RealCw = Cw;
	int RealCh = Ch;
	int RealCimg = Cimg;

	int nAddFrame = 0;

	if( m_Play  && (m_ImgCount < m_iMaxImgCount) )
	{
		if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( g_nowTime , m_dwLimitOldTime ) , <= , m_LimitTime  ) ) 
		{
			if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( g_nowTime , m_dwOldTimer ) , >= , m_IntervalTime ) )
			{
				m_nAnimTime	+= SAFE_TIME_SUB( g_nowTime , m_dwOldTimer );
				nAddFrame = m_nAnimTime / ANIMFRAME_NUM;
				int test = m_nAnimTime;
				//FLOAT testTemp;

				m_nAnimTime %= ANIMFRAME_NUM;
				
				m_dwOldTimer = g_nowTime;

				switch(m_effMode)
				{
				case DEFAULT_BASE:
					
					if(m_NextImg > m_MaxImgNum -1  )
					{
						m_NextImg = 0;	
						m_ImgCount += nAddFrame;
					}
					
					break; 
				case SIZE_DOWN:
					
					//시간이 무한데... 
					if(m_NextImg > m_MaxImgNum-1 )
					{
						m_NextImg = 0;	
					}
					
					m_fSize += m_fScale*nAddFrame;
					
					if( m_fSize > m_iMaxSize  )
					{
						m_fSize = 0;
						m_ImgCount += nAddFrame;
						m_EndSprite = true; 
					}
					
					RealCx = Cx + (m_fSize / 2 );
					RealCy = Cy + (m_fSize / 2 );
					RealCw = Cw - m_fSize;
					RealCh = Ch - m_fSize;
					
					break;
				case SIZE_UP:
					
					//시간이 무한데... 
					if(m_NextImg >= m_MaxImgNum-1 )
					{
						m_NextImg = 0;	
					}
					
					m_fSize += m_fScale*nAddFrame;
					
					if( m_fSize >= m_iMaxSize)
					{
						m_fSize = 0;
						m_ImgCount += nAddFrame;
						m_EndSprite = true; 
					}
					
					RealCx = Cx - (m_fSize / 2 );
					RealCy = Cy - (m_fSize / 2 );
					RealCw = Cw + m_fSize;
					RealCh = Ch + m_fSize;
					
					break;
				case REPEAT_ANI:
					{
						if(m_NextImg >= m_MaxImgNum )
						{
							m_NextImg = m_MaxImgNum-1;
							m_bRepeat = false ; 
						}
						
						if( m_NextImg < 0 )
						{
							m_ImgCount += nAddFrame;
							m_NextImg = 0;
							m_bRepeat = true; 
							m_EndSprite = true; 
						}
					}	
				}
			}
			
			RealCimg = Cimg + m_NextImg; 

			if(m_ImgCount < m_iMaxImgCount)
			{
				Render->ScretchDraw( RealCimg , RealCx , RealCy, RealCw , RealCh , Ccolor );	
			}
									
			if( !m_bStopImg )
			{
				if(m_bRepeat)
				{
					m_NextImg += nAddFrame;
				}
				else 
				{
					m_NextImg -= nAddFrame;
				}
			}
		}
	}
}

#else	//	ELDA_ANIFRAME_SYNC

void CImageAni::RenderProc()
{
	
	//총 로테이션 시간 
	// 몇초만 플레이 하고 사라져라 
	
	
	int RealCx = Cx;
	int RealCy = Cy;
	int RealCw = Cw;
	int RealCh = Ch;
	int RealCimg = Cimg;

	if( m_Play  && (m_ImgCount < m_iMaxImgCount) )
	{
		if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( g_nowTime , m_dwLimitOldTime ) , <= , m_LimitTime  ) ) 
		{
			if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( g_nowTime , m_dwOldTimer ) , >= , m_IntervalTime ) )
			{
				m_dwOldTimer = g_nowTime;
				
				switch(m_effMode)
				{
				case DEFAULT_BASE:
					
					if(m_NextImg > m_MaxImgNum -1  )
					{
						m_NextImg = 0;	
						m_ImgCount++;
					}
					
					break; 
				case SIZE_DOWN:
					
					//시간이 무한데... 
					if(m_NextImg > m_MaxImgNum-1 )
					{
						m_NextImg = 0;	
					}
					
					m_fSize += m_fScale;
					
					if( m_fSize > m_iMaxSize  )
					{
						m_fSize = 0;
						m_ImgCount++;
						m_EndSprite = true; 
					}
					
					RealCx = Cx + (m_fSize / 2 );
					RealCy = Cy + (m_fSize / 2 );
					RealCw = Cw - m_fSize;
					RealCh = Ch - m_fSize;
					
					break;
				case SIZE_UP:
					
					//시간이 무한데... 
					if(m_NextImg >= m_MaxImgNum-1 )
					{
						m_NextImg = 0;	
					}
					
					m_fSize += m_fScale;
					
					if( m_fSize >= m_iMaxSize)
					{
						m_fSize = 0;
						m_ImgCount++;
						m_EndSprite = true; 
					}
					
					RealCx = Cx - (m_fSize / 2 );
					RealCy = Cy - (m_fSize / 2 );
					RealCw = Cw + m_fSize;
					RealCh = Ch + m_fSize;
					
					break;
				case REPEAT_ANI:
					{
						if(m_NextImg >= m_MaxImgNum )
						{
							m_NextImg = m_MaxImgNum-1;
							m_bRepeat = false ; 
						}
						
						if( m_NextImg < 0 )
						{
							m_ImgCount++;
							m_NextImg = 0;
							m_bRepeat = true; 
							m_EndSprite = true; 
						}
					}	
				}
			}
			
			RealCimg = Cimg + m_NextImg; 

			if(m_ImgCount < m_iMaxImgCount)
			{
				Render->ScretchDraw( RealCimg , RealCx , RealCy, RealCw , RealCh , Ccolor );	
			}
									
			if( !m_bStopImg )
			{
				if(m_bRepeat)
				{
					m_NextImg++;
				}
				else 
				{
					m_NextImg--;
				}
			}
		}
	}
}

#endif	//	ELDA_ANIFRAME_SYNC

void CImageAni::SetPlay(bool play)
{
	m_dwLimitOldTime = g_nowTime;
	m_dwOldTimer = g_nowTime;
	m_NextImg = 0;
	m_EndSprite = false;
	m_ImgCount = 0;

	m_Play = play;

}