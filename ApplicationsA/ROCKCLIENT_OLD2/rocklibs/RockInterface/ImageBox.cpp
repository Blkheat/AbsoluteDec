///////////////////////////////////////////////////////////////////////////////
///
///		File		: ImageBox.cpp
///		Desc		:
///
///		Author		: Park Soo-Hyun
///		Team		: Program - Client Team
///		Date		: 
///
///		Copyright(c) 2004, Rocksoft Co., Ltd. ALL RIGHTS RESERVED
///
///////////////////////////////////////////////////////////////////////////////
#include "ImageBox.h"
#include "..\\..\\CRenderManager.h"
#include "Rui.h"
#include "EmblemTexManager.h"

#ifdef DIRECT_VERSION_9_MJH
extern IDirect3DDevice9*	g_lpDevice;
#else
extern IDirect3DDevice8*	g_lpDevice;
#endif // DIRECT_VERSION_9_MJH
#ifndef _LEFT 

#define _LEFT 0
#define _TOP 1
#define _WIDTH 2
#define _HEIGHT 3
#define _TILE_SIZE 30
#define _TILE_SIZEX2 60

#endif

//-----------------------------------------------------------------------------
CImageBox::CImageBox( SDesktop* DT ) : CTextBox( DT )
{
	m_bGlow = FALSE;
	m_bClip = FALSE; 
}

//-----------------------------------------------------------------------------
CImageBox::~CImageBox()
{
}

void CImageBox::RenderProc()
{
	DWORD SrcBlend, DestBlend;
	
	if(m_bGlow)
	{
	   g_RenderManager.GetRenderState( D3DRS_SRCBLEND, &SrcBlend );
	   g_RenderManager.GetRenderState( D3DRS_DESTBLEND, &DestBlend );		
	   
  	   g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
 	   g_RenderManager.SetDestBlend( D3DBLEND_ONE );
	}
		
	if( CBgImage != TID_None )
	{
		Render->Draw( CBgImage, Cx, Cy, Cw, Ch, Ccolor );
	}
	
	CTextBox::RenderProc();

	if(m_bGlow)
	{
  	   g_RenderManager.SetSrcBlend( SrcBlend );
 	   g_RenderManager.SetDestBlend( DestBlend );
	}
}

///---------------------------------------------------------------------------
///-- CImageBox9
///---------------------------------------------------------------------------

//-----------------------------------------------------------------------------
CImageBox9::CImageBox9( SDesktop* DT ) : CTextBox( DT )
{
	m_bGlow = FALSE;
}

//-----------------------------------------------------------------------------
CImageBox9::~CImageBox9()
{
}

///---------------------------------------------------------------------------
///-- RenderProc
///---------------------------------------------------------------------------
void CImageBox9::RenderProc()
{
	int i;
	DWORD SrcBlend, DestBlend;
	
	if(m_bGlow)
	{
	   g_RenderManager.GetRenderState( D3DRS_SRCBLEND, &SrcBlend );
	   g_RenderManager.GetRenderState( D3DRS_DESTBLEND, &DestBlend );		
	   
  	   g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
 	   g_RenderManager.SetDestBlend( D3DBLEND_ONE );
	}
	
	for( i = 0 ; i < 9; ++i )
	{
		Render->Draw( Cimg + i, maal_tile[i][_LEFT], maal_tile[i][_TOP], maal_tile[i][_WIDTH], maal_tile[i][_HEIGHT], Ccolor );
	}

	//CTextBox::RenderProc();

	if(m_bGlow)
	{
  	   g_RenderManager.SetSrcBlend( SrcBlend );
 	   g_RenderManager.SetDestBlend( DestBlend );
	}
}

///---------------------------------------------------------------------------
///-- SetWndTileSizeLR
///---------------------------------------------------------------------------
void CImageBox9::SetWndTileSizeLR( const int al_left, const int al_right )
{
	ml_Wnd_width[0] = al_left;
	ml_Wnd_width[1] = al_right;
}
///---------------------------------------------------------------------------
///-- SetWndTileSizeTB
///---------------------------------------------------------------------------
void CImageBox9::SetWndTileSizeTB( const int al_top, const int al_bottom )
{
	ml_Wnd_height[0] = al_top;
	ml_Wnd_height[1] = al_bottom;
}
///---------------------------------------------------------------------------
///-- SetAdjustPosition
///---------------------------------------------------------------------------
void CImageBox9::SetAdjustPosition( int PosX, int PosY )//OR
{
 	// 원 루틴

	Tx = Cx + BlankLeft;
	Ty = Cy + BlankTop;

	Tw = Cw - ( BlankRight + BlankLeft );
	Th = Ch - ( BlankBottom + BlankTop );

	Ax = GetAlignPosX();
	Ay = GetAlignPosY();
	
	int ll_stretch_width = Cw - ml_Wnd_width[0] - ml_Wnd_width[1];
	int ll_stretch_height= Ch - ml_Wnd_height[0] - ml_Wnd_height[1]; 
	
	
	maal_tile[0][_LEFT] = maal_tile[3][_LEFT] = maal_tile[6][_LEFT] = Cx;
	maal_tile[1][_LEFT] = maal_tile[4][_LEFT] = maal_tile[7][_LEFT] = Cx + ml_Wnd_width[0];
	maal_tile[2][_LEFT] = maal_tile[5][_LEFT] = maal_tile[8][_LEFT] = maal_tile[1][_LEFT] + ll_stretch_width;
	
	maal_tile[0][_TOP] = maal_tile[1][_TOP] = maal_tile[2][_TOP] = Cy;
	maal_tile[3][_TOP] = maal_tile[4][_TOP] = maal_tile[5][_TOP] = Cy + ml_Wnd_height[0];
	maal_tile[6][_TOP] = maal_tile[7][_TOP] = maal_tile[8][_TOP] = maal_tile[3][_TOP] + ll_stretch_height;
	
	maal_tile[0][_WIDTH] = maal_tile[3][_WIDTH] = maal_tile[6][_WIDTH] = ml_Wnd_width[0];
	maal_tile[1][_WIDTH] = maal_tile[4][_WIDTH] = maal_tile[7][_WIDTH] = ll_stretch_width ;
	maal_tile[2][_WIDTH] = maal_tile[5][_WIDTH] = maal_tile[8][_WIDTH] = ml_Wnd_width[1];
	
	maal_tile[0][_HEIGHT] = maal_tile[1][_HEIGHT] = maal_tile[2][_HEIGHT] = ml_Wnd_height[0];
	maal_tile[3][_HEIGHT] = maal_tile[4][_HEIGHT] = maal_tile[5][_HEIGHT] = ll_stretch_height ;
	maal_tile[6][_HEIGHT] = maal_tile[7][_HEIGHT] = maal_tile[8][_HEIGHT] = ml_Wnd_height[1];
	
}

CEmblemImageBox::CEmblemImageBox( SDesktop* DT ) : CTextBox( DT )
{

}

CEmblemImageBox::~CEmblemImageBox()
{

}
	
void CEmblemImageBox::RenderProc()
{
	if(nRui->GetPcInvenInfo()->FrontierMyInfo)
	{
		if(nRui->GetPcInvenInfo()->FrontierMyInfo->FrontierID)
		{
			VESIONTEX * pVesion 
			= g_EmblemTexManager.FindForntierVersionTex(nRui->GetPcInvenInfo()->FrontierMyInfo->FrontierID);
			if(pVesion)
			{
				g_EmblemTexManager.RenderFrontierImg(pVesion, Cx, Cy, Cw, Ch);
			}
		}
	}
}

//-----------------------------------------------------------------------------
//텍스쳐 이미지 없이 .. 박스 만들기 
//그라인더 효과이다 .. 
//-----------------------------------------------------------------------------
CNoImageBox::CNoImageBox( SDesktop* DT ) : CTextBox( DT )
{
	m_nLHeight = 16; 
	m_LineCount = 0 ; 
	m_LimitCount = 0;
}

//-----------------------------------------------------------------------------
CNoImageBox::~CNoImageBox()
{
}

void CNoImageBox::SetShowCount(int Count)
{
	m_LimitCount = Count;
}

void CNoImageBox::SetMaxCount(int Count )
{
	m_LineCount = Count;
}

void CNoImageBox::RenderProc()
{

	int nCount = min( m_LimitCount , m_LineCount );

	int nX = Cx + 4;
	int nY = Cy + Ch - 17;	
	
	if(nCount == 0)
	{
		return;
	}
	
	int nPosX = Cx;
	int nPosY = nY ;
	int nWidth = Cw + 29;
	int nHeight = ( ( (nCount - 1) * m_nLHeight ) + 18); 

	SVertex vt[4];


	vt[0].diffuse =	D3DCOLOR_ARGB( 155, 50, 50, 50 );
	vt[1].diffuse = D3DCOLOR_ARGB( 0, 50, 50, 50 );
    vt[2].diffuse = D3DCOLOR_ARGB( 155, 50, 50, 50 );		
	vt[3].diffuse = D3DCOLOR_ARGB( 0, 50, 50, 50 );

	vt[0].x = (float)nPosX - 0.5f;
    vt[0].y = (float)nPosY - 0.5f;		
	vt[0].w = 1.0f;

	vt[1].x = vt[0].x + nWidth;
    vt[1].y = vt[0].y;
	vt[1].w = 1.0f;

	vt[2].x = vt[0].x;
    vt[2].y = vt[0].y + nHeight;
	vt[2].w = 1.0f;
    
	vt[3].x = vt[0].x + nWidth;
    vt[3].y = vt[0].y + nHeight;
	vt[3].w = 1.0f;
    

	DWORD Col0Op, Col0ARG1, Col0ARG2;
	DWORD Alp0Op, Alp0ARG1, Alp0ARG2;

	DWORD Col1Op, Col1ARG1, Col1ARG2;
	//DWORD Alp1Op, Alp1ARG1, Alp1ARG2;

	DWORD Col2Op, Col2ARG1, Col2ARG2;
	//DWORD Alp2Op, Alp2ARG1, Alp2ARG2;

	g_RenderManager.GetTextureStageState( 0, D3DTSS_COLOROP,   &Col0Op );
	g_RenderManager.GetTextureStageState( 0, D3DTSS_COLORARG1, &Col0ARG1 );
	g_RenderManager.GetTextureStageState( 0, D3DTSS_COLORARG2, &Col0ARG2 );

	g_RenderManager.GetTextureStageState( 0, D3DTSS_ALPHAOP,   &Alp0Op );
	g_RenderManager.GetTextureStageState( 0, D3DTSS_ALPHAARG1, &Alp0ARG1 );
	g_RenderManager.GetTextureStageState( 0, D3DTSS_ALPHAARG2, &Alp0ARG2 );

	g_RenderManager.GetTextureStageState( 1, D3DTSS_COLOROP,   &Col1Op );
	g_RenderManager.GetTextureStageState( 1, D3DTSS_COLORARG1, &Col1ARG1 );
	g_RenderManager.GetTextureStageState( 1, D3DTSS_COLORARG2, &Col1ARG2 );

	g_RenderManager.GetTextureStageState( 2, D3DTSS_COLOROP,   &Col2Op );
	g_RenderManager.GetTextureStageState( 2, D3DTSS_COLORARG1, &Col2ARG1 );
	g_RenderManager.GetTextureStageState( 2, D3DTSS_COLORARG2, &Col2ARG2 );

	g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_SELECTARG1 );
	g_RenderManager.SetTextureColorArg1( 0 , D3DTA_DIFFUSE );
	g_RenderManager.SetTextureColorArg2( 0 , D3DTOP_DISABLE );
	
	g_RenderManager.SetTextureAlphaOP  ( 0 , D3DTOP_SELECTARG1 );
	g_RenderManager.SetTextureAlphaArg1( 0 , D3DTA_DIFFUSE );
	g_RenderManager.SetTextureAlphaArg2( 0 , D3DTOP_DISABLE );

	g_RenderManager.SetTextureColorOP  ( 1 , D3DTOP_DISABLE );
	g_RenderManager.SetTextureColorArg1( 1 , D3DTOP_DISABLE );
	g_RenderManager.SetTextureColorArg2( 1 , D3DTOP_DISABLE );

	g_RenderManager.SetTextureColorOP  ( 2 , D3DTOP_DISABLE );
	g_RenderManager.SetTextureColorArg1( 2 , D3DTOP_DISABLE );
	g_RenderManager.SetTextureColorArg2( 2 , D3DTOP_DISABLE );

	g_RenderManager.SetFVF( RUI_VERTEX_FORMAT );
	g_RenderManager.SetTexture( 0, NULL );
	g_RenderManager.DrawPrimitiveUP( D3DPT_TRIANGLESTRIP , 2, vt, sizeof( SVertex ) );

	g_RenderManager.SetTextureColorOP  ( 0 , Col0Op );
	g_RenderManager.SetTextureColorArg1( 0 , Col0ARG1 );
	g_RenderManager.SetTextureColorArg2( 0 , Col0ARG2 );
	
	g_RenderManager.SetTextureAlphaOP  ( 0 , Alp0Op );
	g_RenderManager.SetTextureAlphaArg1( 0 , Alp0ARG1 );
	g_RenderManager.SetTextureAlphaArg2( 0 , Alp0ARG2 );

	g_RenderManager.SetTextureColorOP  ( 1 , Col1Op );
	g_RenderManager.SetTextureColorArg1( 1 , Col1ARG1 );
	g_RenderManager.SetTextureColorArg2( 1 , Col1ARG2 );

	g_RenderManager.SetTextureColorOP  ( 2 , Col2Op );
	g_RenderManager.SetTextureColorArg1( 2 , Col2ARG1 );
	g_RenderManager.SetTextureColorArg2( 2 , Col2ARG2 );



	

}