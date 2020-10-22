#include "..\\..\\rocklibs\RockPCH.h"
#include "CFade.h"
#include <stdio.h>

#include "..\..\CRenderManager.h"

CFade::CFade()
{
	m_Color      = D3DCOLOR_ARGB( 0 , 0 , 0 , 0 );
	m_EndColor   = D3DCOLOR_ARGB( 0 , 0 , 0 , 0 );
	m_OldTime    = 0;
	m_State      = FADE_STATE_NONE;
	m_ISContinue = FALSE;

	//InitializeCriticalSection( &csFadeFlag );
}

CFade::~CFade()
{
	//DeleteCriticalSection( &csFadeFlag );
}


//................................................................................................................
// 화면 사이즈 설정
//................................................................................................................
void CFade::Init(int xsize,int ysize)
{
	//EnterCriticalSection( &csFadeFlag );
	
	m_VertexList[ 0 ].vpos = D3DXVECTOR4(			0.0f  ,			  0.0f  , 0.0f , 1.0f );
	m_VertexList[ 1 ].vpos = D3DXVECTOR4( ( float ) xsize ,			  0.0f	, 0.0f , 1.0f );
	m_VertexList[ 2 ].vpos = D3DXVECTOR4(			0.0f  , ( float ) ysize , 0.0f , 1.0f );
	m_VertexList[ 3 ].vpos = D3DXVECTOR4( ( float ) xsize , ( float ) ysize , 0.0f , 1.0f );

	//LeaveCriticalSection( &csFadeFlag );
}


//................................................................................................................
// 페이드 시작
//................................................................................................................
void CFade::Start(POST_FADE_STATE Type,D3DXCOLOR scolor,D3DXCOLOR ecolor,DWORD Time,BOOL ISContinue)
{
	m_OldTime    = timeGetTime();
	m_RangeTime  = Time;
	m_ISContinue = ISContinue;
	m_State      = Type;

	SetColor( scolor );
	SetEndColor( ecolor );
}

void CFade::Start(POST_FADE_STATE Type,D3DXCOLOR scolor,D3DXCOLOR mcolor,D3DXCOLOR ecolor,DWORD Time,DWORD Time2,BOOL ISContinue)
{
	m_OldTime    = timeGetTime();
	m_RangeTime  = Time;
	m_ISContinue = ISContinue;
	m_State      = Type;

	SetColor( scolor );
	SetEndColor( mcolor );

	m_MiddleColor = ecolor;
	m_MiddleTime  = Time2;
}


//................................................................................................................
// 끝 값 지정
//................................................................................................................
void CFade::SetEndColor(D3DXCOLOR color)
{
	m_EndColor = color;
}

//................................................................................................................
// 현재 칼라값 설정
//................................................................................................................
void CFade::SetColor(D3DXCOLOR color)
{
	m_Color = color;
}

//................................................................................................................
// 페이드 처리 안함
//................................................................................................................
void CFade::ResetFade(void)
{
	m_State  = FADE_STATE_NONE;
}

//................................................................................................................
// 범위
//................................................................................................................
float CFade::GetRange(void)
{
	
	DWORD SubTime = SAFE_TIME_SUB( timeGetTime() , m_OldTime );

	if( SubTime == 0 ) 
		return 0.0f;

	return ( float )( ( float ) SubTime / ( float ) m_RangeTime );
}

//................................................................................................................
// 칼라 지정
//................................................................................................................
void CFade::SetVertexColor(D3DXCOLOR color)
{
	m_VertexList[ 0 ].color =
	m_VertexList[ 1 ].color =
	m_VertexList[ 2 ].color =
	m_VertexList[ 3 ].color = color;
}

#ifdef DIRECT_VERSION_9_MJH
void CFade::Process(LPDIRECT3DDEVICE9 pDevice)
#else
void CFade::Process(LPDIRECT3DDEVICE8 pDevice)
#endif // DIRECT_VERSION_9_MJH
{
	switch( m_State )
	{
		//........................................................................................................
		// 페이드 적용 안함
		//........................................................................................................
		case FADE_STATE_NONE:
			return;

		//........................................................................................................
		// 페이드 적용
		//........................................................................................................
		case FADE_STATE_LOOP:
		case FADE_STATE_FADE:
			{
				float Range = GetRange();
			
				if( Range >= 1.0f )
				{
					m_Color = m_EndColor;

					SetVertexColor( m_EndColor );

					if( FADE_STATE_LOOP == m_State )
					{
						Start( FADE_STATE_FADE , m_EndColor , m_MiddleColor , m_MiddleTime , m_ISContinue );
					}
					else if( m_ISContinue )
					{
						m_State = FADE_STATE_CONTINUE;
					}
					else
					{
						m_State = FADE_STATE_NONE;
					}
				}
				else
				{
					D3DXCOLOR Color;
					D3DXCOLOR ResultColor;

					Color = m_EndColor - m_Color;

					Color *= Range;
					
					ResultColor = m_Color + Color;

					SetVertexColor( ResultColor );
				}
			}
			
			break;

		//........................................................................................................
		// 현재 칼라로
		//........................................................................................................
		case FADE_STATE_CONTINUE:			
			break;
	}
}

void CFade::Render()
{
	if( m_State == FADE_STATE_NONE )
	{
		return;
	}
	
	DWORD dFVF = g_RenderManager.GetFVF();
	DWORD dZEnable = g_RenderManager.GetZEnable();
	DWORD dFogEnable = g_RenderManager.GetFogEnable();
	DWORD dLighting = g_RenderManager.GetLighting();
	
	DWORD dColOp = g_RenderManager.GetTextureColorOP( 0 );
	DWORD dColArg1 = g_RenderManager.GetTextureColorArg1( 0 );
	DWORD dColArg2 = g_RenderManager.GetTextureColorArg2( 0 );

	DWORD dAlphaBlendEnable = g_RenderManager.GetAlphaBlendEnable();
	DWORD dSrcBlend = g_RenderManager.GetSrcBlend();
	DWORD dDestBlend = g_RenderManager.GetDestBlend();
	
	g_RenderManager.SetFVF( D3DFVF_VERTEX_FADE_2D );

	g_RenderManager.SetZEnable( FALSE );
	g_RenderManager.SetFogEnable( FALSE );
	g_RenderManager.SetLighting( FALSE );

	g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_SELECTARG1 );
	g_RenderManager.SetTextureColorArg1( 0 , D3DTA_DIFFUSE );
	g_RenderManager.SetTextureColorArg2( 0 , D3DTOP_DISABLE );

	g_RenderManager.DisableTextureStageState( 1 );
	g_RenderManager.DisableTextureStageState( 2 );

	g_RenderManager.SetAlphaBlendEnable( TRUE );
	g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
	g_RenderManager.SetDestBlend( D3DBLEND_INVSRCALPHA );

	g_RenderManager.DisableTexture( 0 );

	g_RenderManager.DrawPrimitiveUP( D3DPT_TRIANGLESTRIP , 2 , m_VertexList , sizeof( VERTEX_FADE_2D ) );
		
	g_RenderManager.SetFVF( dFVF );
	g_RenderManager.SetZEnable( dZEnable );
	g_RenderManager.SetFogEnable( dFogEnable );
	g_RenderManager.SetLighting( dLighting );

	g_RenderManager.SetTextureColorOP  ( 0 , dColOp );
	g_RenderManager.SetTextureColorArg1( 0 , dColArg1 );
	g_RenderManager.SetTextureColorArg2( 0 , dColArg2 );

	g_RenderManager.SetAlphaBlendEnable( dAlphaBlendEnable );
	g_RenderManager.SetSrcBlend( dSrcBlend );
	g_RenderManager.SetDestBlend( dDestBlend );
	
}
