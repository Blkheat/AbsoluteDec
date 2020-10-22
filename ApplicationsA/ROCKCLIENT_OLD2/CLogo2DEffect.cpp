
#include "CLogo2DEffect.h"
#include "CRenderManager.h"
#include	<SOUND\\Sound.H>
#include "bravolibs\\obj\\texture.h"

#ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/23)
	
#include "RockClient.h"
float g_fExtentW = 0.0f, g_fExtentH = 0.0f;

#endif// #ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/23)


extern Texture				g_LogoTex;
extern Texture				g_EffTex1;
extern Texture				g_EffTex2;
extern Texture				g_EffTex3;
extern Texture				g_LoadingTex;

STexture	g_tmpTex;

CLogo2DEffect::CLogo2DEffect()
{
	//int nCenX = 1024 / 2;
	//int nCenY = 768 / 2;	
	
	m_uNowEffStep = n_LogoEffNone;

	m_dGapTime = 0;
	m_dPrevTime = 0;

#ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/23)
	
	float fResolutionRateW = (g_RockClient.m_d3dsdBackBuffer.Width / 1024.0f) - 1.0f;// 폭 변동율
	float fResolutionRateH = (g_RockClient.m_d3dsdBackBuffer.Height / 768.0f) - 1.0f;// 높이 변동율
	
	g_fExtentW = 425.0f * fResolutionRateW;// 이동 폭
	g_fExtentH = 318.0f * fResolutionRateH;// 이동 높이
	
	m_sEff1Rect = SFRect(425.0f + g_fExtentW, 318.0f + g_fExtentH, 200.0f, 119.0f);
	m_sEff2Rect = SFRect(528.0f + g_fExtentW, 367.0f + g_fExtentH, 125.0f, 79.0f);
	m_sEff3Rect = SFRect(339.0f + g_fExtentW, 302.0f + g_fExtentH, 390.0f, 164.0f);
	m_sEffAddRect = SFRect(528.0f + g_fExtentW, 367.0f + g_fExtentH, 125.0f, 79.0f);	///-- 2번째 이미지
	m_sLoaingRect = SFRect(396.0f + g_fExtentW, 500.0f + g_fExtentH, 270.0f, 18.0f);
	
#else
	
	m_sEff1Rect = SFRect(425.0f, 318.0f, 200.0f, 119.0f);
	m_sEff2Rect = SFRect(528.0f, 367.0f, 125.0f, 79.0f);
	m_sEff3Rect = SFRect(339.0f, 302.0f, 390.0f, 164.0f);
	m_sEffAddRect = SFRect(528.0f, 367.0f, 125.0f, 79.0f);	///-- 2번째 이미지
	m_sLoaingRect = SFRect(396.0f, 500.0f, 270.0f, 18.0f);
	
#endif// #ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/23)

	m_sEff1RectUV = SFRect(0.0f, 0.0f, 200.0f/256.0f, 119.0f/128.0f);
	m_sEff2RectUV = SFRect(0.0f, 0.0f, 125.0f/128.0f, 79.0f/128.0f);
	m_sEff3RectUV = SFRect(0.0f, 0.0f, 390.0f/512.0f, 164.0f/256.0f);
	m_sLoaingRectUV = SFRect(0.0f, 0.0f, 270.0f/512.0f, 18.0f/32.0f);


#ifdef KOREA_VERSION

#ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/23)
	
	g_fExtentW = 373.0f * fResolutionRateW;// 이동 폭
	g_fExtentH = 315.0f * fResolutionRateH;// 이동 높이
	
	m_sEff1Rect = SFRect(373.0f + g_fExtentW, 315.0f + g_fExtentH, 250.0f, 123.0f);
	m_sEff2Rect = SFRect(478.0f + g_fExtentW, 423.0f + g_fExtentH, 142.0f, 32.0f);
	m_sEff3Rect = SFRect(412.0f + g_fExtentW, 265.0f + g_fExtentH, 289.0f, 186.0f);
	m_sEffAddRect = SFRect(478.0f + g_fExtentW, 423.0f + g_fExtentH, 142.0f, 32.0f);	///-- 2번째 이미지
	m_sLoaingRect = SFRect(386.0f + g_fExtentW, 500.0f + g_fExtentH, 270.0f, 18.0f);
	
#else
	
	m_sEff1Rect = SFRect(373.0f, 315.0f, 250.0f, 123.0f);
	m_sEff2Rect = SFRect(478.0f, 423.0f, 142.0f, 32.0f);
	m_sEff3Rect = SFRect(412.0f, 265.0f, 289.0f, 186.0f);
	m_sEffAddRect = SFRect(478.0f, 423.0f, 142.0f, 32.0f);	///-- 2번째 이미지
	m_sLoaingRect = SFRect(386.0f, 500.0f, 270.0f, 18.0f);
	
#endif// #ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/23)
	
	m_sEff1RectUV = SFRect(0.0f, 0.0f, 250.0f/256.0f, 123.0f/128.0f);
	m_sEff2RectUV = SFRect(0.0f, 0.0f, 142.0f/256.0f, 32.0f/32.0f);
	m_sEff3RectUV = SFRect(0.0f, 0.0f, 289.0f/512.0f, 186.0f/256.0f);
	m_sLoaingRectUV = SFRect(0.0f, 0.0f, 270.0f/512.0f, 18.0f/32.0f);
	
#endif	
	
	m_nEff1Alpha = 0;
	m_nEff2Alpha = 0;
	m_nEff3Alpha = 0;

	m_nEffAddAlpha = 0;

	m_bIsFlashEff = FALSE;
	m_dFlashPrevTime = 0;

	m_nLoadingAlpah = 0;

}

CLogo2DEffect::~CLogo2DEffect()
{


}

void CLogo2DEffect::Update2DEffect()
{
	DWORD dNowTime = timeGetTime();

	static BOOL IsTemp = FALSE;
	
	switch( m_uNowEffStep )
	{
		case n_LogoEffStart:
			{
				if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( dNowTime , m_dPrevTime ) , >= , 1400 ) )
				{
					if( !IsTemp )
					{
						g_Sound.Play( 60, 0.0f, 0.0f, 0.0f, SCENE_NORMAL, TRUE );
						IsTemp = TRUE; 
					}
				}				
				
				if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( dNowTime , m_dPrevTime ) , >= , 1500 ) )
				{
					IsTemp = FALSE;					
					m_uNowEffStep = n_LogoEffStep1;
					m_dPrevTime = dNowTime;					
				}			
			}
			break;

		case n_LogoEffStep1:
			{
				if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( dNowTime , m_dPrevTime ) , >= , 500 ) )
				{
					m_uNowEffStep = n_LogoEffStep2;
					m_dPrevTime = dNowTime;
					
				#ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/23)
					
					m_sEff1Rect.x = 425.0f + g_fExtentW;
					
				#else
					
					m_sEff1Rect.x = 425.0f;
					
				#endif// #ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/23)
					
					
#ifdef KOREA_VERSION
					
				#ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/23)
					
					m_sEff1Rect.x = 373.0f + g_fExtentW;
					
				#else
					
					m_sEff1Rect.x = 373.0f;
					
				#endif// #ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/23)
					
#endif
					m_nEff1Alpha = 255;
					m_AImageList.clear();					
				}
				else
				{
					DWORD dGapTime = SAFE_TIME_SUB( dNowTime, m_dPrevTime );
					float fRatio = (float)dGapTime / 500.0f;
					
				#ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/23)
					
					int nNowPosX = 15 + ( fRatio * ( (425.0f + g_fExtentW) - 15.0f) );	
					
				#else
					
					int nNowPosX = 15 + ( fRatio * (425 - 15) );	
					
				#endif// #ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/23)
					
					int nAlpha = 128 + ( fRatio * ( 255 - 128) );
					
#ifdef KOREA_VERSION
					
				#ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/23)
					
					nNowPosX = 15 + ( fRatio * ( (373.0f + g_fExtentW) - 15.0f) );
					
				#else
					
					nNowPosX = 15 + ( fRatio * (373 - 15) );
					
				#endif// #ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/23)
					
#endif
					
					m_sEff1Rect.x = (float)nNowPosX;
					m_nEff1Alpha = nAlpha;
					
					nAlpha = ( fRatio * 128 );	
					
					SLogoAImage sLogoImage;
					sLogoImage.nEffAlpha = nAlpha;
					sLogoImage.sEffRect.x = m_sEff1Rect.x;
					sLogoImage.sEffRect.y = m_sEff1Rect.y;
					sLogoImage.sEffRect.w = m_sEff1Rect.w;
					sLogoImage.sEffRect.h = m_sEff1Rect.h;
					
					m_AImageList.push_front( sLogoImage );
					
					if( m_AImageList.size() > 3 )
						m_AImageList.pop_back();
				}
			}
			break;
		case n_LogoEffStep2:
			{
				if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( dNowTime , m_dPrevTime ) , >= , 200 ) )
				{
					if( !IsTemp )
					{
						g_Sound.Play( 61, 0.0f, 0.0f, 0.0f, SCENE_NORMAL, TRUE );
						IsTemp = TRUE; 
					}
				}				
				
				if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( dNowTime , m_dPrevTime ) , >= , 300 ) )
				{
					IsTemp = FALSE;	
					m_uNowEffStep = n_LogoEffStep3;
					m_dPrevTime = dNowTime;					
				}
			}
			break;

		case n_LogoEffStep3:
			{
				if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( dNowTime , m_dPrevTime ) , >= , 400 ) )
				{
					m_uNowEffStep = n_LogoEffStep4;
					m_dPrevTime = dNowTime;
					
				#ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/23)
					
					m_sEff2Rect.x = 528.0f + g_fExtentW;
					
				#else
					
					m_sEff2Rect.x = 528.0f;
					
				#endif// #ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/23)
					
					
#ifdef KOREA_VERSION
					
				#ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/23)
					
					m_sEff2Rect.x = 478.0 + g_fExtentW;
					
				#else
					
					m_sEff2Rect.x = 478.0f;
					
				#endif// #ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/23)
#endif					
					m_nEff2Alpha = 255;
					m_bIsFlashEff = TRUE;
					m_dFlashPrevTime = dNowTime;
					m_AImageList2.clear();
				}
				else
				{
					DWORD dGapTime = SAFE_TIME_SUB( dNowTime, m_dPrevTime );
					float fRatio = (float)dGapTime / 400.0f;
					
				#ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/23)
					
					int nNowPosX = 930 - ( fRatio * 400.0f + g_fExtentW);	
					
				#else
					
					int nNowPosX = 930 - ( fRatio * 400.0f );	
					
				#endif// #ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/23)
					
					
#ifdef KOREA_VERSION
					
				#ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/23)
					
					nNowPosX = 880.0f + g_fExtentW - ( fRatio * 400.0f );	
					
				#else
					
					nNowPosX = 880.0f - ( fRatio * 400.0f );
					
				#endif// #ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/23)
					
#endif					
					
					int nAlpha = 128 + ( fRatio * ( 255 - 128) );
					
					m_sEff2Rect.x = (float)nNowPosX;
					m_nEff2Alpha = nAlpha;
					
					nAlpha = ( fRatio * 128 );
					
					SLogoAImage sLogoImage;
					sLogoImage.nEffAlpha = nAlpha;
					sLogoImage.sEffRect.x = m_sEff2Rect.x;
					sLogoImage.sEffRect.y = m_sEff2Rect.y;
					sLogoImage.sEffRect.w = m_sEff2Rect.w;
					sLogoImage.sEffRect.h = m_sEff2Rect.h;
					
					m_AImageList2.push_front( sLogoImage );
					
					if( m_AImageList2.size() > 3 )
						m_AImageList2.pop_back();
				}
			}
			break;

		case n_LogoEffStep4:
			{
				if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( dNowTime , m_dPrevTime ) , >= , 600 ) )
				{
					if( !IsTemp )
					{
						g_Sound.Play( 283, 0.0f, 0.0f, 0.0f, SCENE_NORMAL, TRUE );
						IsTemp = TRUE; 
					}
				}	
				
				if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( dNowTime , m_dPrevTime ) , >= , 700 ) )
				{
					IsTemp = FALSE; 					
					m_uNowEffStep = n_LogoEffStep5;
					m_dPrevTime = dNowTime;
				}
			}
			break;
		case n_LogoEffStep5:
			{
				if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( dNowTime , m_dPrevTime ) , >= , 1600 ) )
				{
					m_uNowEffStep = n_LogoEffStep6;
					m_dPrevTime = dNowTime;					
					m_nEff3Alpha = 255;
				}
				else
				{
					DWORD dGapTime = SAFE_TIME_SUB( dNowTime, m_dPrevTime );
					float fRatio = (float)dGapTime / 1600.0f;				
					int nAlpha = fRatio * 255;					
					m_nEff3Alpha = nAlpha;
				}
			}
			break;
		case n_LogoEffStep6:
			{
				if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( dNowTime , m_dPrevTime ) , >= , 200 ) )
				{
					m_uNowEffStep = n_LogoEffEnd;
					m_dPrevTime = dNowTime;
				}
			}
			break;
	}
	
	if( m_bIsFlashEff )
	{
		if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( dNowTime , m_dFlashPrevTime ) , >= , 1000 ) )
		{
			m_nEffAddAlpha = 0;
			m_bIsFlashEff = FALSE;
		}
		else
		{
			float fCenPointX = 528.0f + 125.0f / 2.0f;
			float fCenPointY = 367.0f + 79.0f / 2.0f;	
			
			DWORD dGapTime = SAFE_TIME_SUB( dNowTime, m_dFlashPrevTime );
			float fRatio = (float)dGapTime / 1000.0f;				
			int nAlpha = 200 - ( fRatio * 200 );					
			m_nEffAddAlpha = nAlpha;

			m_sEffAddRect.w = 125.0f + (fRatio * 0);
			m_sEffAddRect.h = 79.0f + (fRatio * 0);
		
			if((((int)m_sEffAddRect.w) % 2) != 0)
				m_sEffAddRect.w += 1.0f;
			if((((int)m_sEffAddRect.h) % 2) != 0)
				m_sEffAddRect.h += 1.0f;

		#ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/23)
			
			m_sEffAddRect.x = 528.0f + g_fExtentW;
			m_sEffAddRect.y = 367.0f + g_fExtentH;
			m_sEffAddRect.w = 125.0f; 
			m_sEffAddRect.h = 79.0f;
			
		#else
			
			m_sEffAddRect.x = 528.0f;
			m_sEffAddRect.y = 367.0f;
			m_sEffAddRect.w = 125.0f; 
			m_sEffAddRect.h = 79.0f;
			
		#endif// #ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/23)
			
#ifdef KOREA_VERSION
			
		#ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/23)
			
			m_sEffAddRect.x = 478.0f + g_fExtentW;
			m_sEffAddRect.y = 393.0f + g_fExtentH;
			m_sEffAddRect.w = 142.0f; 
			m_sEffAddRect.h = 32.0f;
			
		#else
			
			m_sEffAddRect.x = 478.0f;
			m_sEffAddRect.y = 393.0f;
			m_sEffAddRect.w = 142.0f; 
			m_sEffAddRect.h = 32.0f;
			
		#endif// #ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/23)
#endif
	
		}				
	}

	if( m_uNowEffStep == n_LogoEffEnd )
	{
		DWORD dGapTime =  SAFE_TIME_SUB( dNowTime , m_dPrevTime ); 
		
		int ni_time = ( dGapTime % (int)( (1.0f + 0.5f) * 1000.0f) ) 
					   / (1.0f * 1000.0f);
		
		m_nLoadingAlpah = 255;
		
		if( 1 == ni_time )
		{
			m_nLoadingAlpah = 0;		
		}	
	}
}

void CLogo2DEffect::Render2DEffect(int nAlpha)
{
	
	g_RenderManager.SetTextureColorOP( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureColorArg1( 0 , D3DTA_DIFFUSE );
	g_RenderManager.SetTextureColorArg2( 0 , D3DTA_TEXTURE );	
	
	g_RenderManager.SetTextureAlphaOP( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureAlphaArg1( 0 , D3DTA_DIFFUSE );
	g_RenderManager.SetTextureAlphaArg2( 0 , D3DTA_TEXTURE );	
	
	DWORD dAlphaBlend = g_RenderManager.GetAlphaBlendEnable();
	DWORD dSrcBlend = g_RenderManager.GetSrcBlend();
	DWORD dDestBlend = g_RenderManager.GetDestBlend();

	g_RenderManager.SetAlphaBlendEnable( TRUE );
	g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
	g_RenderManager.SetDestBlend( D3DBLEND_INVSRCALPHA );

	if( nAlpha == 255 )
	{
		Render2DEff3(m_nEff3Alpha);
		Render2DEff1(m_nEff1Alpha);
		Render2DEff2(m_nEff2Alpha);
		Render2DEffAdd(m_nEffAddAlpha);

		Render2DEffLoading(m_nLoadingAlpah);
	}
	else
	{
		//Render2DEff3(nAlpha);
		//Render2DEff1(nAlpha);
		//Render2DEff2(nAlpha);	
	}

	g_RenderManager.SetAlphaBlendEnable( dAlphaBlend );
	g_RenderManager.SetSrcBlend( dSrcBlend );
	g_RenderManager.SetDestBlend( dDestBlend );
}

void CLogo2DEffect::Render2DEff1(int nAlpha)
{
	int nCount = m_AImageList.size();

	for(int i = 0 ; i < nCount ; i++)
	{
		SLogoAImage sLogoImage = m_AImageList[i];
		g_tmpTex.vt[0].diffuse	= D3DCOLOR_ARGB( sLogoImage.nEffAlpha , 255, 255, 255 );
		g_tmpTex.vt[1].diffuse	= D3DCOLOR_ARGB( sLogoImage.nEffAlpha , 255, 255, 255 );
		g_tmpTex.vt[2].diffuse	= D3DCOLOR_ARGB( sLogoImage.nEffAlpha , 255, 255, 255 );
		g_tmpTex.vt[3].diffuse	= D3DCOLOR_ARGB( sLogoImage.nEffAlpha , 255, 255, 255 );
		
		g_tmpTex.vt[0].x = sLogoImage.sEffRect.x - 0.5f;
		g_tmpTex.vt[0].y = sLogoImage.sEffRect.y - 0.5f;		
		g_tmpTex.vt[1].x = g_tmpTex.vt[0].x + sLogoImage.sEffRect.w;
		g_tmpTex.vt[1].y = g_tmpTex.vt[0].y;
		g_tmpTex.vt[2].x = g_tmpTex.vt[0].x + sLogoImage.sEffRect.w;
		g_tmpTex.vt[2].y = g_tmpTex.vt[0].y + sLogoImage.sEffRect.h;
		g_tmpTex.vt[3].x = g_tmpTex.vt[0].x;
		g_tmpTex.vt[3].y = g_tmpTex.vt[0].y + sLogoImage.sEffRect.h;
		
		g_tmpTex.vt[0].tu = 0.0f;
		g_tmpTex.vt[1].tu = m_sEff1RectUV.w;
		g_tmpTex.vt[2].tu = m_sEff1RectUV.w;
		g_tmpTex.vt[3].tu = 0.0f;
		
		g_tmpTex.vt[0].tv = 0.0f;
		g_tmpTex.vt[1].tv = 0.0f;
		g_tmpTex.vt[2].tv = m_sEff1RectUV.h;
		g_tmpTex.vt[3].tv = m_sEff1RectUV.h;
		
		g_RenderManager.SetTexture( 0 , g_EffTex1.m_pD3DTex );
		g_RenderManager.DrawPrimitiveUP( D3DPT_TRIANGLEFAN , 2, g_tmpTex.vt, sizeof( SVertex ) );
	}	
	
	////-- 배경 이미지를 그려준다.
	g_tmpTex.vt[0].diffuse	= D3DCOLOR_ARGB( nAlpha, 255, 255, 255 );
	g_tmpTex.vt[1].diffuse	= D3DCOLOR_ARGB( nAlpha, 255, 255, 255 );
	g_tmpTex.vt[2].diffuse	= D3DCOLOR_ARGB( nAlpha, 255, 255, 255 );
	g_tmpTex.vt[3].diffuse	= D3DCOLOR_ARGB( nAlpha, 255, 255, 255 );
	
	g_tmpTex.vt[0].x = m_sEff1Rect.x - 0.5f;
	g_tmpTex.vt[0].y = m_sEff1Rect.y - 0.5f;		
	g_tmpTex.vt[1].x = g_tmpTex.vt[0].x + m_sEff1Rect.w;
	g_tmpTex.vt[1].y = g_tmpTex.vt[0].y;
	g_tmpTex.vt[2].x = g_tmpTex.vt[0].x + m_sEff1Rect.w;
	g_tmpTex.vt[2].y = g_tmpTex.vt[0].y + m_sEff1Rect.h;
	g_tmpTex.vt[3].x = g_tmpTex.vt[0].x;
	g_tmpTex.vt[3].y = g_tmpTex.vt[0].y + m_sEff1Rect.h;
	
	g_tmpTex.vt[0].tu = 0.0f;
	g_tmpTex.vt[1].tu = m_sEff1RectUV.w;
	g_tmpTex.vt[2].tu = m_sEff1RectUV.w;
	g_tmpTex.vt[3].tu = 0.0f;
	
	g_tmpTex.vt[0].tv = 0.0f;
	g_tmpTex.vt[1].tv = 0.0f;
	g_tmpTex.vt[2].tv = m_sEff1RectUV.h;
	g_tmpTex.vt[3].tv = m_sEff1RectUV.h;
	
	g_RenderManager.SetTexture( 0 , g_EffTex1.m_pD3DTex );
	g_RenderManager.DrawPrimitiveUP( D3DPT_TRIANGLEFAN , 2, g_tmpTex.vt, sizeof( SVertex ) );	

}

void CLogo2DEffect::Render2DEff2(int nAlpha)
{
	int nCount = m_AImageList2.size();

	for(int i = 0 ; i < nCount ; i++)
	{
		SLogoAImage sLogoImage = m_AImageList2[i];
		g_tmpTex.vt[0].diffuse	= D3DCOLOR_ARGB( sLogoImage.nEffAlpha , 255, 255, 255 );
		g_tmpTex.vt[1].diffuse	= D3DCOLOR_ARGB( sLogoImage.nEffAlpha , 255, 255, 255 );
		g_tmpTex.vt[2].diffuse	= D3DCOLOR_ARGB( sLogoImage.nEffAlpha , 255, 255, 255 );
		g_tmpTex.vt[3].diffuse	= D3DCOLOR_ARGB( sLogoImage.nEffAlpha , 255, 255, 255 );
		
		g_tmpTex.vt[0].x = sLogoImage.sEffRect.x - 0.5f;
		g_tmpTex.vt[0].y = sLogoImage.sEffRect.y - 0.5f;		
		g_tmpTex.vt[1].x = g_tmpTex.vt[0].x + sLogoImage.sEffRect.w;
		g_tmpTex.vt[1].y = g_tmpTex.vt[0].y;
		g_tmpTex.vt[2].x = g_tmpTex.vt[0].x + sLogoImage.sEffRect.w;
		g_tmpTex.vt[2].y = g_tmpTex.vt[0].y + sLogoImage.sEffRect.h;
		g_tmpTex.vt[3].x = g_tmpTex.vt[0].x;
		g_tmpTex.vt[3].y = g_tmpTex.vt[0].y + sLogoImage.sEffRect.h;
		
		g_tmpTex.vt[0].tu = 0.0f;
		g_tmpTex.vt[1].tu = m_sEff2RectUV.w;
		g_tmpTex.vt[2].tu = m_sEff2RectUV.w;
		g_tmpTex.vt[3].tu = 0.0f;
		
		g_tmpTex.vt[0].tv = 0.0f;
		g_tmpTex.vt[1].tv = 0.0f;
		g_tmpTex.vt[2].tv = m_sEff2RectUV.h;
		g_tmpTex.vt[3].tv = m_sEff2RectUV.h;
		
		g_RenderManager.SetTexture( 0 , g_EffTex2.m_pD3DTex );
		g_RenderManager.DrawPrimitiveUP( D3DPT_TRIANGLEFAN , 2, g_tmpTex.vt, sizeof( SVertex ) );
	}	
	
	////-- 배경 이미지를 그려준다.
	g_tmpTex.vt[0].diffuse	= D3DCOLOR_ARGB( nAlpha, 255, 255, 255 );
	g_tmpTex.vt[1].diffuse	= D3DCOLOR_ARGB( nAlpha, 255, 255, 255 );
	g_tmpTex.vt[2].diffuse	= D3DCOLOR_ARGB( nAlpha, 255, 255, 255 );
	g_tmpTex.vt[3].diffuse	= D3DCOLOR_ARGB( nAlpha, 255, 255, 255 );
	
	g_tmpTex.vt[0].x = m_sEff2Rect.x - 0.5f;
	g_tmpTex.vt[0].y = m_sEff2Rect.y - 0.5f;		
	g_tmpTex.vt[1].x = g_tmpTex.vt[0].x + m_sEff2Rect.w;
	g_tmpTex.vt[1].y = g_tmpTex.vt[0].y;
	g_tmpTex.vt[2].x = g_tmpTex.vt[0].x + m_sEff2Rect.w;
	g_tmpTex.vt[2].y = g_tmpTex.vt[0].y + m_sEff2Rect.h;
	g_tmpTex.vt[3].x = g_tmpTex.vt[0].x;
	g_tmpTex.vt[3].y = g_tmpTex.vt[0].y + m_sEff2Rect.h;
	
	g_tmpTex.vt[0].tu = 0.0f;
	g_tmpTex.vt[1].tu = m_sEff2RectUV.w;
	g_tmpTex.vt[2].tu = m_sEff2RectUV.w;
	g_tmpTex.vt[3].tu = 0.0f;
	
	g_tmpTex.vt[0].tv = 0.0f;
	g_tmpTex.vt[1].tv = 0.0f;
	g_tmpTex.vt[2].tv = m_sEff2RectUV.h;
	g_tmpTex.vt[3].tv = m_sEff2RectUV.h;
	
	g_RenderManager.SetTexture( 0 , g_EffTex2.m_pD3DTex );
	g_RenderManager.DrawPrimitiveUP( D3DPT_TRIANGLEFAN , 2, g_tmpTex.vt, sizeof( SVertex ) );
}

void CLogo2DEffect::Render2DEff3(int nAlpha)
{
	////-- 배경 이미지를 그려준다.
	g_tmpTex.vt[0].diffuse	= D3DCOLOR_ARGB( nAlpha, 255, 255, 255 );
	g_tmpTex.vt[1].diffuse	= D3DCOLOR_ARGB( nAlpha, 255, 255, 255 );
	g_tmpTex.vt[2].diffuse	= D3DCOLOR_ARGB( nAlpha, 255, 255, 255 );
	g_tmpTex.vt[3].diffuse	= D3DCOLOR_ARGB( nAlpha, 255, 255, 255 );
	
	g_tmpTex.vt[0].x = m_sEff3Rect.x - 0.5f;
	g_tmpTex.vt[0].y = m_sEff3Rect.y - 0.5f;		
	g_tmpTex.vt[1].x = g_tmpTex.vt[0].x + m_sEff3Rect.w;
	g_tmpTex.vt[1].y = g_tmpTex.vt[0].y;
	g_tmpTex.vt[2].x = g_tmpTex.vt[0].x + m_sEff3Rect.w;
	g_tmpTex.vt[2].y = g_tmpTex.vt[0].y + m_sEff3Rect.h;
	g_tmpTex.vt[3].x = g_tmpTex.vt[0].x;
	g_tmpTex.vt[3].y = g_tmpTex.vt[0].y + m_sEff3Rect.h;
	
	g_tmpTex.vt[0].tu = 0.0f;
	g_tmpTex.vt[1].tu = m_sEff3RectUV.w;
	g_tmpTex.vt[2].tu = m_sEff3RectUV.w;
	g_tmpTex.vt[3].tu = 0.0f;
	
	g_tmpTex.vt[0].tv = 0.0f;
	g_tmpTex.vt[1].tv = 0.0f;
	g_tmpTex.vt[2].tv = m_sEff3RectUV.h;
	g_tmpTex.vt[3].tv = m_sEff3RectUV.h;
	
	g_RenderManager.SetTexture( 0 , g_EffTex3.m_pD3DTex );
	g_RenderManager.DrawPrimitiveUP( D3DPT_TRIANGLEFAN , 2, g_tmpTex.vt, sizeof( SVertex ) );
}

void CLogo2DEffect::Render2DEffAdd(int nAlpha)
{
	g_RenderManager.SetTextureColorOP( 0 , D3DTOP_ADD );
	g_RenderManager.SetTextureColorArg1( 0 , D3DTA_DIFFUSE );
	g_RenderManager.SetTextureColorArg2( 0 , D3DTA_TEXTURE );	
	
	DWORD dAlphaBlend = g_RenderManager.GetAlphaBlendEnable();
	DWORD dSrcBlend = g_RenderManager.GetSrcBlend();
	DWORD dDestBlend = g_RenderManager.GetDestBlend();
	
	g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
	g_RenderManager.SetDestBlend( D3DBLEND_ONE );	
	
	////-- 배경 이미지를 그려준다.
	g_tmpTex.vt[0].diffuse	= D3DCOLOR_ARGB( nAlpha, 255, 255, 255 );
	g_tmpTex.vt[1].diffuse	= D3DCOLOR_ARGB( nAlpha, 255, 255, 255 );
	g_tmpTex.vt[2].diffuse	= D3DCOLOR_ARGB( nAlpha, 255, 255, 255 );
	g_tmpTex.vt[3].diffuse	= D3DCOLOR_ARGB( nAlpha, 255, 255, 255 );
	
	g_tmpTex.vt[0].x = m_sEffAddRect.x - 0.5f;
	g_tmpTex.vt[0].y = m_sEffAddRect.y - 0.5f;		
	g_tmpTex.vt[1].x = g_tmpTex.vt[0].x + m_sEffAddRect.w;
	g_tmpTex.vt[1].y = g_tmpTex.vt[0].y;
	g_tmpTex.vt[2].x = g_tmpTex.vt[0].x + m_sEffAddRect.w;
	g_tmpTex.vt[2].y = g_tmpTex.vt[0].y + m_sEffAddRect.h;
	g_tmpTex.vt[3].x = g_tmpTex.vt[0].x;
	g_tmpTex.vt[3].y = g_tmpTex.vt[0].y + m_sEffAddRect.h;
	
	g_tmpTex.vt[0].tu = 0.0f;
	g_tmpTex.vt[1].tu = m_sEff2RectUV.w;
	g_tmpTex.vt[2].tu = m_sEff2RectUV.w;
	g_tmpTex.vt[3].tu = 0.0f;
	
	g_tmpTex.vt[0].tv = 0.0f;
	g_tmpTex.vt[1].tv = 0.0f;
	g_tmpTex.vt[2].tv = m_sEff2RectUV.h;
	g_tmpTex.vt[3].tv = m_sEff2RectUV.h;
	
	g_RenderManager.SetTexture( 0 , g_EffTex2.m_pD3DTex );
	g_RenderManager.DrawPrimitiveUP( D3DPT_TRIANGLEFAN , 2, g_tmpTex.vt, sizeof( SVertex ) );

	g_RenderManager.SetSrcBlend( dSrcBlend );
	g_RenderManager.SetDestBlend( dDestBlend );	

	g_RenderManager.SetTextureColorOP( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureColorArg1( 0 , D3DTA_DIFFUSE );
	g_RenderManager.SetTextureColorArg2( 0 , D3DTA_TEXTURE );		
}

void CLogo2DEffect::Render2DEffLoading(int nAlpha)
{
	////-- 배경 이미지를 그려준다.
	g_tmpTex.vt[0].diffuse	= D3DCOLOR_ARGB( nAlpha, 255, 255, 255 );
	g_tmpTex.vt[1].diffuse	= D3DCOLOR_ARGB( nAlpha, 255, 255, 255 );
	g_tmpTex.vt[2].diffuse	= D3DCOLOR_ARGB( nAlpha, 255, 255, 255 );
	g_tmpTex.vt[3].diffuse	= D3DCOLOR_ARGB( nAlpha, 255, 255, 255 );
	
	g_tmpTex.vt[0].x = m_sLoaingRect.x - 0.5f;
	g_tmpTex.vt[0].y = m_sLoaingRect.y - 0.5f;		
	g_tmpTex.vt[1].x = g_tmpTex.vt[0].x + m_sLoaingRect.w;
	g_tmpTex.vt[1].y = g_tmpTex.vt[0].y;
	g_tmpTex.vt[2].x = g_tmpTex.vt[0].x + m_sLoaingRect.w;
	g_tmpTex.vt[2].y = g_tmpTex.vt[0].y + m_sLoaingRect.h;
	g_tmpTex.vt[3].x = g_tmpTex.vt[0].x;
	g_tmpTex.vt[3].y = g_tmpTex.vt[0].y + m_sLoaingRect.h;
	
	g_tmpTex.vt[0].tu = 0.0f;
	g_tmpTex.vt[1].tu = m_sLoaingRectUV.w;
	g_tmpTex.vt[2].tu = m_sLoaingRectUV.w;
	g_tmpTex.vt[3].tu = 0.0f;
	
	g_tmpTex.vt[0].tv = 0.0f;
	g_tmpTex.vt[1].tv = 0.0f;
	g_tmpTex.vt[2].tv = m_sLoaingRectUV.h;
	g_tmpTex.vt[3].tv = m_sLoaingRectUV.h;
	
	g_RenderManager.SetTexture( 0 , g_LoadingTex.m_pD3DTex );
	g_RenderManager.DrawPrimitiveUP( D3DPT_TRIANGLEFAN , 2, g_tmpTex.vt, sizeof( SVertex ) );
}
