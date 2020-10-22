///////////////////////////////////////////////////////////////////////////////
///
///		File		: Render.cpp
///		Desc		:
///
///		Author		: Park Soo-Hyun
///		Team		: Program - Client Team
///		Date		: 
///
///		Copyright(c) 2004, Rocksoft Co., Ltd. ALL RIGHTS RESERVED
///
///////////////////////////////////////////////////////////////////////////////


#include "Render.h"

#include "..\\..\\global.h"
#include "..\\..\\RockClient.h"
#include "..\\..\\CRenderManager.h"

CRender*		Render;


//-----------------------------------------------------------------------------
#ifdef DIRECT_VERSION_9_MJH
CRender::CRender( LPDIRECT3DDEVICE9 D3DDevice )
#else
CRender::CRender( LPDIRECT3DDEVICE8 D3DDevice )
#endif // DIRECT_VERSION_9_MJH
{
	Init();
	
	theD3DDevice = D3DDevice;	
}

//-----------------------------------------------------------------------------
CRender::~CRender()
{
	theD3DDevice = NULL;
}

void CRender::Init()
{
	InitMarkUp();	
}

void CRender::InitMarkUp()
{
	m_IsDblStrt = false;
	m_IsDblPlus = false;
	m_IsDblMinus = false;
	m_IsDblDivide = false;
	m_IsDblDallar = false;
	m_IsDblAnd = false;;
}

//-----------------------------------------------------------------------------
void CRender::SetState()
{
	if( theD3DDevice == NULL )
	{
		return;
	}

	g_RenderManager.SetFVF( RUI_VERTEX_FORMAT );
	g_RenderManager.SetZEnable( FALSE );
	g_RenderManager.SetLighting( FALSE );
	g_RenderManager.SetFogEnable( FALSE );

	g_RenderManager.SetTextureMipFilter( 0 , D3DTEXF_NONE );
	g_RenderManager.SetTextureMinFilter( 0 , D3DTEXF_LINEAR );
	g_RenderManager.SetTextureMagFilter( 0 , D3DTEXF_LINEAR );

	g_RenderManager.SetTextureColorOP( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureColorArg1( 0 , D3DTA_TEXTURE );
	g_RenderManager.SetTextureColorArg2( 0 , D3DTA_DIFFUSE );
	
	g_RenderManager.SetTextureAlphaOP( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureAlphaArg1( 0 , D3DTA_DIFFUSE );
	g_RenderManager.SetTextureAlphaArg2( 0 , D3DTA_TEXTURE );
	
	g_RenderManager.SetAlphaTestEnable( TRUE );
	g_RenderManager.SetAlphaFunc( D3DCMP_NOTEQUAL );
	g_RenderManager.SetAlphaRef( 0 );

	g_RenderManager.SetAlphaBlendEnable( TRUE );
	g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
	g_RenderManager.SetDestBlend( D3DBLEND_INVSRCALPHA );

	g_RenderManager.DisableTextureStageState( 1 );
	g_RenderManager.DisableTextureStageState( 2 );
}

//-----------------------------------------------------------------------------
void CRender::ReleaseState()
{
	g_RenderManager.SetFogEnable( TRUE );
	g_RenderManager.SetLighting( TRUE );
	g_RenderManager.SetZEnable( TRUE );
}

//-----------------------------------------------------------------------------
/*
bool CRender::Begin()
{	
	if( SUCCEEDED( g_RockClient.BeginScene() ) ) 
	{
		return true;
	}

	return false;
}

//-----------------------------------------------------------------------------
void CRender::End()
{
	g_RockClient.EndScene();
}
*/

//-----------------------------------------------------------------------------
void CRender::DrawState()
{
}

//-----------------------------------------------------------------------------
void CRender::SetScissorTest( int x, int y, int w, int h )
{

}

//-----------------------------------------------------------------------------
void CRender::SetScissorTest()
{
	
}


///-----------------------------------------------------------------------------
///-- Draw_CloneImg
///-----------------------------------------------------------------------------
///-- 기존의 TID의 Texel 좌표를 쓰며 다른 Img 를 Render 한다.
///-- Minigame 때문에 추가.
void CRender::Draw_CloneImg( TEXID Tid, int CloneTID, int x, int y, int w, int h, D3DCOLOR Color )
{
	if( Tid == TID_None ){	return;	}

	///-- Tid 가 범위를 벗어났다면
	if( Tid >= MAX_TEX_AMOUNT ) 
	{
		RLG1(_T( "!!!!!! Tid값이 잘못 되었습니다 : %d "),Tid );
		Tid = MAX_TEX_AMOUNT  -1;			
	}

	STexture*	Tex = NULL; 
	STexture*	CloneTex = NULL;	
	STexture	tmpTex;
	bool		Clip( true );

	Tex = Resource->GetTexture( Tid );
	if( Tex == NULL )
	{
		return;
	}	

	CloneTex = Resource->GetTexture( CloneTID );
	if( CloneTex == NULL )
	{
		return;
	}
	
	///-- Texture 없음 -> Dynamic Loading_M
	if( Tex && Resource->Img[CloneTex->ImgId].pTexture == NULL )
	{		
		Resource->LoadTextureDynamic( CloneTID );	
		///Tex = Resource->GetTexture( CloneTID );		
	}
	
	if( Tex != NULL)
	{
		memcpy( &tmpTex, Tex, sizeof( STexture ) );
		
		if( w == RD_DEFAULT ){	w = tmpTex.w;	}
		if( h == RD_DEFAULT ){	h = tmpTex.h;	}
		
		tmpTex.vt[0].diffuse	= Color;
		tmpTex.vt[1].diffuse	= Color;
		tmpTex.vt[2].diffuse	= Color;
		tmpTex.vt[3].diffuse	= Color;
		
		tmpTex.vt[0].x = (float)x - 0.5f;
		tmpTex.vt[0].y = (float)y - 0.5f;		
		tmpTex.vt[1].x = tmpTex.vt[0].x + w;
		tmpTex.vt[1].y = tmpTex.vt[0].y;
		tmpTex.vt[2].x = tmpTex.vt[0].x + w;
		tmpTex.vt[2].y = tmpTex.vt[0].y + h;
		tmpTex.vt[3].x = tmpTex.vt[0].x;
		tmpTex.vt[3].y = tmpTex.vt[0].y + h;
		
		if( (w < tmpTex.w) && Clip )
		{
			SImage* Img = Resource->GetImageInfo( tmpTex.ImgId );
			
			tmpTex.vt[1].tu = ( Tex->x + (float)w ) / Img->Width;
			tmpTex.vt[2].tu = ( Tex->x + (float)w ) / Img->Width;
		}
		
		Resource->Img[CloneTex->ImgId].m_lPreTime = timeGetTime();
		g_RenderManager.SetTexture( 0, Resource->Img[CloneTex->ImgId].pTexture );	
		g_RenderManager.DrawPrimitiveUP( D3DPT_TRIANGLEFAN , 2, tmpTex.vt, sizeof( SVertex ) );
	}
}

void CRender::DrawClip(TEXID Tid, int x, int y, int w, int h, D3DCOLOR Color , bool Clip /* = false */)
{
	if( Tid == TID_None ){	return;	}
	
	///-- Tid 가 범위를 벗어났다면
	if( Tid >= MAX_TEX_AMOUNT ) 
	{
		RLG1(_T( "!!!!!! Tid값이 잘못 되었습니다 : %d "),Tid );
		Tid = MAX_TEX_AMOUNT  -1;			
	}
	STexture*	Tex = NULL;	
	STexture	tmpTex;
	// bool		Clip( true );
	
	Tex = Resource->GetTexture( Tid );
	if( Tex == NULL )
	{
		return;
	}	
	
	///-- Texture 없음 -> Dynamic Loading_M
	if( Tex && Resource->Img[Tex->ImgId].pTexture == NULL )
	{		
		Resource->LoadTextureDynamic( Tid );	
		Tex = Resource->GetTexture( Tid );		
	}
	
	if( Tex != NULL)
	{
		memcpy( &tmpTex, Tex, sizeof( STexture ) );
		
		if( w == RD_DEFAULT ){	w = tmpTex.w;	}
		if( h == RD_DEFAULT ){	h = tmpTex.h;	}
		
		tmpTex.vt[0].diffuse	= Color;
		tmpTex.vt[1].diffuse	= Color;
		tmpTex.vt[2].diffuse	= Color;
		tmpTex.vt[3].diffuse	= Color;
		
		tmpTex.vt[0].x = (float)x - 0.5f;
		tmpTex.vt[0].y = (float)y - 0.5f;		
		tmpTex.vt[1].x = tmpTex.vt[0].x + w;
		tmpTex.vt[1].y = tmpTex.vt[0].y;
		tmpTex.vt[2].x = tmpTex.vt[0].x + w;
		tmpTex.vt[2].y = tmpTex.vt[0].y + h;
		tmpTex.vt[3].x = tmpTex.vt[0].x;
		tmpTex.vt[3].y = tmpTex.vt[0].y + h;
		
		if( (w < tmpTex.w) && Clip )
		{
			SImage* Img = Resource->GetImageInfo( tmpTex.ImgId );
			
			tmpTex.vt[1].tu = ( Tex->x + (float)w ) / Img->Width;
			tmpTex.vt[2].tu = ( Tex->x + (float)w ) / Img->Width;
		}
		
		Resource->Img[tmpTex.ImgId].m_lPreTime = timeGetTime();
		g_RenderManager.SetTexture( 0, tmpTex.pTexture );	
		g_RenderManager.DrawPrimitiveUP( D3DPT_TRIANGLEFAN , 2, tmpTex.vt, sizeof( SVertex ) );
	}

}



///-----------------------------------------------------------------------------
///-- Draw
///-----------------------------------------------------------------------------
///-- 각 Interface 를 Draw 한다.
///-- Texture 가 준비되지 않았을 경우 실시간 로딩을 수행한다
void CRender::Draw( TEXID Tid, int x, int y, int w, int h, D3DCOLOR Color )
{
	if( Tid == TID_None ){	return;	}

	///-- Tid 가 범위를 벗어났다면
	if( Tid >= MAX_TEX_AMOUNT ) 
	{
		RLG1(_T( "!!!!!! Tid값이 잘못 되었습니다 : %d "),Tid );
		Tid = MAX_TEX_AMOUNT  -1;			
	}
	STexture*	Tex = NULL;	
	STexture	tmpTex;
	bool		Clip( true );

	Tex = Resource->GetTexture( Tid );
	if( Tex == NULL )
	{
		return;
	}	
	
	///-- Texture 없음 -> Dynamic Loading_M
	if( Tex && Resource->Img[Tex->ImgId].pTexture == NULL )
	{		
		Resource->LoadTextureDynamic( Tid );	
		Tex = Resource->GetTexture( Tid );		
	}
	
	if( Tex != NULL)
	{
		memcpy( &tmpTex, Tex, sizeof( STexture ) );
		
		if( w == RD_DEFAULT ){	w = tmpTex.w;	}
		if( h == RD_DEFAULT ){	h = tmpTex.h;	}
		
		tmpTex.vt[0].diffuse	= Color;
		tmpTex.vt[1].diffuse	= Color;
		tmpTex.vt[2].diffuse	= Color;
		tmpTex.vt[3].diffuse	= Color;
		
		tmpTex.vt[0].x = (float)x - 0.5f;
		tmpTex.vt[0].y = (float)y - 0.5f;		
		tmpTex.vt[1].x = tmpTex.vt[0].x + w;
		tmpTex.vt[1].y = tmpTex.vt[0].y;
		tmpTex.vt[2].x = tmpTex.vt[0].x + w;
		tmpTex.vt[2].y = tmpTex.vt[0].y + h;
		tmpTex.vt[3].x = tmpTex.vt[0].x;
		tmpTex.vt[3].y = tmpTex.vt[0].y + h;
		
		if( (w < tmpTex.w) && Clip )
		{
			SImage* Img = Resource->GetImageInfo( tmpTex.ImgId );
			
			tmpTex.vt[1].tu = ( Tex->x + (float)w ) / Img->Width;
			tmpTex.vt[2].tu = ( Tex->x + (float)w ) / Img->Width;
		}

		Resource->Img[tmpTex.ImgId].m_lPreTime = timeGetTime();
		g_RenderManager.SetTexture( 0, tmpTex.pTexture );	
		g_RenderManager.DrawPrimitiveUP( D3DPT_TRIANGLEFAN , 2, tmpTex.vt, sizeof( SVertex ) );
	}
}


void CRender::DrawFitImg( TEXID Tid, int x, int y, int w, int h, D3DCOLOR Color )
{
	if( Tid == TID_None ){	return;	}

	///-- Tid 가 범위를 벗어났다면
	if( Tid >= MAX_TEX_AMOUNT ) 
	{
		RLG1(_T( "!!!!!! Tid값이 잘못 되었습니다 : %d "),Tid );
		Tid = MAX_TEX_AMOUNT  -1;			
	}
	STexture*	Tex = NULL;	
	STexture	tmpTex;
	bool		Clip( true );

	Tex = Resource->GetTexture( Tid );
	if( Tex == NULL )
	{
		return;
	}	
	
	///-- Texture 없음 -> Dynamic Loading_M
	if( Tex && Resource->Img[Tex->ImgId].pTexture == NULL )
	{		
		Resource->LoadTextureDynamic( Tid );	
		Tex = Resource->GetTexture( Tid );		
	}
	
	if( Tex != NULL)
	{
		memcpy( &tmpTex, Tex, sizeof( STexture ) );
		
		if( w == RD_DEFAULT ){	w = tmpTex.w;	}
		if( h == RD_DEFAULT ){	h = tmpTex.h;	}
		
		tmpTex.vt[0].diffuse	= Color;
		tmpTex.vt[1].diffuse	= Color;
		tmpTex.vt[2].diffuse	= Color;
		tmpTex.vt[3].diffuse	= Color;
		
		tmpTex.vt[0].x = (float)x - 0.5f;
		tmpTex.vt[0].y = (float)y - 0.5f;		
		tmpTex.vt[1].x = tmpTex.vt[0].x + w;
		tmpTex.vt[1].y = tmpTex.vt[0].y;
		tmpTex.vt[2].x = tmpTex.vt[0].x + w;
		tmpTex.vt[2].y = tmpTex.vt[0].y + h;
		tmpTex.vt[3].x = tmpTex.vt[0].x;
		tmpTex.vt[3].y = tmpTex.vt[0].y + h;
		
		tmpTex.vt[1].tu = 1.0f;
		tmpTex.vt[2].tu = 1.0f;

		tmpTex.vt[2].tv = 1.0f;
		tmpTex.vt[3].tv = 1.0f;

		Resource->Img[tmpTex.ImgId].m_lPreTime = timeGetTime();
		g_RenderManager.SetTexture( 0, tmpTex.pTexture );	
		g_RenderManager.DrawPrimitiveUP( D3DPT_TRIANGLEFAN , 2, tmpTex.vt, sizeof( SVertex ) );
	}
}


void CRender::ScretchDraw( TEXID Tid, int x, int y, int w, int h, D3DCOLOR Color )
{
	if( Tid == TID_None ){	return;	}

	///-- Tid 가 범위를 벗어났다면
	if( Tid >= MAX_TEX_AMOUNT ) 
	{
		RLG1(_T( "!!!!!! Tid값이 잘못 되었습니다 : %d "),Tid );
		Tid = MAX_TEX_AMOUNT  -1;			
	}
	STexture*	Tex = NULL;	
	STexture	tmpTex;
	bool		Clip( true );

	Tex = Resource->GetTexture( Tid );
	if( Tex == NULL )
	{
		return;
	}
		
	///-- Texture 없음 -> Dynamic Loading_M
	if( Resource->Img[Tex->ImgId].pTexture == NULL )
	{		
		Resource->LoadTextureDynamic( Tid );	
		Tex = Resource->GetTexture( Tid );		
	}
	
	if( Tex != NULL)
	{
		memcpy( &tmpTex, Tex, sizeof( STexture ) );
		
		if( w == RD_DEFAULT ){	w = tmpTex.w;	}
		if( h == RD_DEFAULT ){	h = tmpTex.h;	}
		
		tmpTex.vt[0].diffuse	= Color;
		tmpTex.vt[1].diffuse	= Color;
		tmpTex.vt[2].diffuse	= Color;
		tmpTex.vt[3].diffuse	= Color;
		
		tmpTex.vt[0].x = (float)x - 0.5f;
		tmpTex.vt[0].y = (float)y - 0.5f;		
		tmpTex.vt[1].x = tmpTex.vt[0].x + w;
		tmpTex.vt[1].y = tmpTex.vt[0].y;
		tmpTex.vt[2].x = tmpTex.vt[0].x + w;
		tmpTex.vt[2].y = tmpTex.vt[0].y + h;
		tmpTex.vt[3].x = tmpTex.vt[0].x;
		tmpTex.vt[3].y = tmpTex.vt[0].y + h;
		
		Resource->Img[tmpTex.ImgId].m_lPreTime = timeGetTime();
		g_RenderManager.SetTexture( 0, tmpTex.pTexture );	
		g_RenderManager.DrawPrimitiveUP( D3DPT_TRIANGLEFAN , 2, tmpTex.vt, sizeof( SVertex ) );
	}
}

void CRender::ScretchDraw( TEXID Tid, int x, int y, int w, int h, D3DCOLOR Color, float z )
{
	if( Tid == TID_None ){	return;	}

	///-- Tid 가 범위를 벗어났다면
	if( Tid >= MAX_TEX_AMOUNT ) 
	{
		RLG1(_T( "!!!!!! Tid값이 잘못 되었습니다 : %d "),Tid );
		Tid = MAX_TEX_AMOUNT  -1;			
	}
	STexture*	Tex = NULL;	
	STexture	tmpTex;
	bool		Clip( true );

	Tex = Resource->GetTexture( Tid );
	if( Tex == NULL )
	{
		return;
	}
		
	///-- Texture 없음 -> Dynamic Loading_M
	if( Resource->Img[Tex->ImgId].pTexture == NULL )
	{		
		Resource->LoadTextureDynamic( Tid );	
		Tex = Resource->GetTexture( Tid );		
	}
	
	if( Tex != NULL)
	{
		memcpy( &tmpTex, Tex, sizeof( STexture ) );
		
		if( w == RD_DEFAULT ){	w = tmpTex.w;	}
		if( h == RD_DEFAULT ){	h = tmpTex.h;	}
		
		tmpTex.vt[0].diffuse	= Color;
		tmpTex.vt[1].diffuse	= Color;
		tmpTex.vt[2].diffuse	= Color;
		tmpTex.vt[3].diffuse	= Color;
		
		tmpTex.vt[0].x = (float)x - 0.5f;
		tmpTex.vt[0].y = (float)y - 0.5f;		
		tmpTex.vt[1].x = tmpTex.vt[0].x + w;
		tmpTex.vt[1].y = tmpTex.vt[0].y;
		tmpTex.vt[2].x = tmpTex.vt[0].x + w;
		tmpTex.vt[2].y = tmpTex.vt[0].y + h;
		tmpTex.vt[3].x = tmpTex.vt[0].x;
		tmpTex.vt[3].y = tmpTex.vt[0].y + h;

		tmpTex.vt[3].z = tmpTex.vt[2].z = tmpTex.vt[1].z = tmpTex.vt[0].z = z;
		
		Resource->Img[tmpTex.ImgId].m_lPreTime = timeGetTime();
		g_RenderManager.SetTexture( 0, tmpTex.pTexture );	
		g_RenderManager.DrawPrimitiveUP( D3DPT_TRIANGLEFAN , 2, tmpTex.vt, sizeof( SVertex ) );

		tmpTex.vt[3].z = tmpTex.vt[2].z = tmpTex.vt[1].z = tmpTex.vt[0].z = 0;
	}
}

void CRender::DrawAllDir(TEXID Tid, int x , int y, int w ,int h , D3DCOLOR Color , int Direction)
{

/////////
	if( Tid == TID_None ){	return;	}

	///-- Tid 가 범위를 벗어났다면
	if( Tid >= MAX_TEX_AMOUNT ) 
	{
		RLG1(_T( "!!!!!! Tid값이 잘못 되었습니다 : %d "),Tid );
		Tid = MAX_TEX_AMOUNT  -1;			
	}
	STexture*	Tex = NULL;	
	STexture	tmpTex;
	bool		Clip( true );

	Tex = Resource->GetTexture( Tid );
	if( Tex == NULL )
	{
		return;
	}	
	
	///-- Texture 없음 -> Dynamic Loading_M
	if( Tex && Resource->Img[Tex->ImgId].pTexture == NULL )
	{		
		Resource->LoadTextureDynamic( Tid );	
		Tex = Resource->GetTexture( Tid );		
	}
	
	if( Tex != NULL)
	{
		memcpy( &tmpTex, Tex, sizeof( STexture ) );
		
		if( w == RD_DEFAULT ){	w = tmpTex.w;	}
		if( h == RD_DEFAULT ){	h = tmpTex.h;	}
	
	
		tmpTex.vt[0].diffuse	= Color;
		tmpTex.vt[1].diffuse	= Color;
		tmpTex.vt[2].diffuse	= Color;
		tmpTex.vt[3].diffuse	= Color;
		
		tmpTex.vt[0].x = (float)x - 0.5f;
		tmpTex.vt[0].y = (float)y - 0.5f;		
		tmpTex.vt[1].x = tmpTex.vt[0].x + w;
		tmpTex.vt[1].y = tmpTex.vt[0].y;
	
		tmpTex.vt[2].x = tmpTex.vt[0].x + w;
		tmpTex.vt[2].y = tmpTex.vt[0].y + h;
	
		tmpTex.vt[3].x = tmpTex.vt[0].x;
		tmpTex.vt[3].y = tmpTex.vt[0].y + h;
		

		
		if( (w < tmpTex.w) && Clip )
		{
			SImage* Img = Resource->GetImageInfo( tmpTex.ImgId );
			
			tmpTex.vt[0].tu = (float)(Tex->x + (Img->Width - w)) /Img->Width;
			tmpTex.vt[0].tv = (float)(Tex->y + (Img->Height - h)) /Img->Height;	
			
			tmpTex.vt[1].tv = (float)(Tex->y + (Img->Height - h)) /Img->Height;	
			
			tmpTex.vt[3].tu = (float)(Tex->x + (Img->Width - w)) /Img->Width;


//			tmpTex.vt[1].tv = ( Tex->x + (float)h ) / Img->Height;
//			tmpTex.vt[2].tv = ( Tex->x + (float)h ) / Img->Height;
//
		}
		
		Resource->Img[tmpTex.ImgId].m_lPreTime = timeGetTime();
		g_RenderManager.SetTexture( 0, tmpTex.pTexture );	
		g_RenderManager.DrawPrimitiveUP( D3DPT_TRIANGLEFAN , 2, tmpTex.vt, sizeof( SVertex ) );
	}

}


void CRender::Draw( TEXID Tid, int x, int y, int w, int h, D3DCOLOR Color , float z)
{
	if( Tid == TID_None ){	return;	}

	///-- Tid 가 범위를 벗어났다면
	if( Tid >= MAX_TEX_AMOUNT ) 
	{
		RLG1(_T( "!!!!!! Tid값이 잘못 되었습니다 : %d "),Tid );
		Tid = MAX_TEX_AMOUNT  -1;			
	}

	STexture*	Tex = NULL;	
	STexture	tmpTex;
	bool		Clip( true );

	Tex = Resource->GetTexture( Tid );
	if( Tex == NULL )
	{
		return;
	}
	
	///-- Texture 없음 -> Dynamic Loading_M	
	if( Tex && Resource->Img[Tex->ImgId].pTexture == NULL )
	{		
		Resource->LoadTextureDynamic( Tid );	
		Tex = Resource->GetTexture( Tid );		
	}
	
	if( Tex != NULL)
	{
		memcpy( &tmpTex, Tex, sizeof( STexture ) );
		
		if( w == RD_DEFAULT ){	w = tmpTex.w;	}
		if( h == RD_DEFAULT ){	h = tmpTex.h;	}
		
		tmpTex.vt[0].diffuse	= Color;
		tmpTex.vt[1].diffuse	= Color;
		tmpTex.vt[2].diffuse	= Color;
		tmpTex.vt[3].diffuse	= Color;
		
		tmpTex.vt[0].x = (float)x - 0.5f;
		tmpTex.vt[0].y = (float)y - 0.5f;		
		tmpTex.vt[1].x = tmpTex.vt[0].x + w;
		tmpTex.vt[1].y = tmpTex.vt[0].y;
	
		tmpTex.vt[2].x = tmpTex.vt[0].x + w;
		tmpTex.vt[2].y = tmpTex.vt[0].y + h;
	
		tmpTex.vt[3].x = tmpTex.vt[0].x;
		tmpTex.vt[3].y = tmpTex.vt[0].y + h;

		tmpTex.vt[3].z = tmpTex.vt[2].z = tmpTex.vt[1].z = tmpTex.vt[0].z = z;
		
		if( (w < tmpTex.w) && Clip )
		{
			SImage* Img = Resource->GetImageInfo( tmpTex.ImgId );
			
			tmpTex.vt[1].tu = ( Tex->x + (float)w ) / Img->Width;
			tmpTex.vt[2].tu = ( Tex->x + (float)w ) / Img->Width;
		}			
		
		Resource->Img[tmpTex.ImgId].m_lPreTime = timeGetTime();
		g_RenderManager.SetTexture( 0, tmpTex.pTexture );	
		g_RenderManager.DrawPrimitiveUP( D3DPT_TRIANGLEFAN , 2, tmpTex.vt, sizeof( SVertex ) );

		tmpTex.vt[3].z = tmpTex.vt[2].z = tmpTex.vt[1].z = tmpTex.vt[0].z = 0;
	}	
}

void CRender::DrawFitImg( TEXID Tid, int x, int y, int w, int h, D3DCOLOR Color , float z)
{
	if( Tid == TID_None ){	return;	}

	///-- Tid 가 범위를 벗어났다면
	if( Tid >= MAX_TEX_AMOUNT ) 
	{
		RLG1(_T( "!!!!!! Tid값이 잘못 되었습니다 : %d "),Tid );
		Tid = MAX_TEX_AMOUNT  -1;			
	}
	STexture*	Tex = NULL;	
	STexture	tmpTex;
	bool		Clip( true );

	Tex = Resource->GetTexture( Tid );
	if( Tex == NULL )
	{
		return;
	}	
	
	///-- Texture 없음 -> Dynamic Loading_M	
	if( Tex && Resource->Img[Tex->ImgId].pTexture == NULL )
	{		
		Resource->LoadTextureDynamic( Tid );	
		Tex = Resource->GetTexture( Tid );		
	}
	
	if( Tex != NULL)
	{
		memcpy( &tmpTex, Tex, sizeof( STexture ) );
		
		if( w == RD_DEFAULT ){	w = tmpTex.w;	}
		if( h == RD_DEFAULT ){	h = tmpTex.h;	}
		
		tmpTex.vt[0].diffuse	= Color;
		tmpTex.vt[1].diffuse	= Color;
		tmpTex.vt[2].diffuse	= Color;
		tmpTex.vt[3].diffuse	= Color;
		
		tmpTex.vt[0].x = (float)x - 0.5f;
		tmpTex.vt[0].y = (float)y - 0.5f;		
		tmpTex.vt[1].x = tmpTex.vt[0].x + w;
		tmpTex.vt[1].y = tmpTex.vt[0].y;
		tmpTex.vt[2].x = tmpTex.vt[0].x + w;
		tmpTex.vt[2].y = tmpTex.vt[0].y + h;
		tmpTex.vt[3].x = tmpTex.vt[0].x;
		tmpTex.vt[3].y = tmpTex.vt[0].y + h;

		tmpTex.vt[3].z = tmpTex.vt[2].z = tmpTex.vt[1].z = tmpTex.vt[0].z = z;
		
		tmpTex.vt[1].tu = 1.0f;
		tmpTex.vt[2].tu = 1.0f;

		tmpTex.vt[2].tv = 1.0f;
		tmpTex.vt[3].tv = 1.0f;
		
		Resource->Img[tmpTex.ImgId].m_lPreTime = timeGetTime();
		g_RenderManager.SetTexture( 0, tmpTex.pTexture );	
		g_RenderManager.DrawPrimitiveUP( D3DPT_TRIANGLEFAN , 2, tmpTex.vt, sizeof( SVertex ) );

		tmpTex.vt[3].z = tmpTex.vt[2].z = tmpTex.vt[1].z = tmpTex.vt[0].z = 0;
	}	
}


//-----------------------------------------------------------------------------
void CRender::DrawUpGaugeDown( TEXID Tid, int x, int y, int w, int h, D3DCOLOR color )
{
	if( Tid == TID_None ){	return;	}

	STexture*	Tex = NULL;	
	STexture	tmpTex;
	bool		Clip( true );

	Tex = Resource->GetTexture( Tid );
	if( Tex == NULL )
	{
		return;
	}
	
	if( Tex != NULL )
	{
		memcpy( &tmpTex, Tex, sizeof( STexture ) );

		if( w == RD_DEFAULT ){	w = tmpTex.w;	}
		if( h == RD_DEFAULT ){	h = tmpTex.h;	}

		tmpTex.vt[0].diffuse	= color;
		tmpTex.vt[1].diffuse	= color;
		tmpTex.vt[2].diffuse	= color;
		tmpTex.vt[3].diffuse	= color;

		tmpTex.vt[0].x = (float)x - 0.5f;
		tmpTex.vt[0].y = (float)y - 0.5f;
		tmpTex.vt[1].x = tmpTex.vt[0].x + w;
		tmpTex.vt[1].y = tmpTex.vt[0].y;
		tmpTex.vt[2].x = tmpTex.vt[0].x + w;
		tmpTex.vt[2].y = tmpTex.vt[0].y + h;
		tmpTex.vt[3].x = tmpTex.vt[0].x;
		tmpTex.vt[3].y = tmpTex.vt[0].y + h;

		if( (h < tmpTex.h) && Clip )
		{
			SImage* Img = Resource->GetImageInfo( tmpTex.ImgId );

			tmpTex.vt[2].tv = ( Tex->y + (float)h ) / Img->Height;
			tmpTex.vt[3].tv = ( Tex->y + (float)h ) / Img->Height;
		}

		g_RenderManager.SetTexture( 0, tmpTex.pTexture );	
		g_RenderManager.DrawPrimitiveUP( D3DPT_TRIANGLEFAN , 2, tmpTex.vt, sizeof( SVertex ) );
	}
}

//-----------------------------------------------------------------------------
void CRender::DrawUpGaugeUp( TEXID Tid, int x, int y, int w, int h, D3DCOLOR color, int _delta )
{
	if( Tid == TID_None ){	return;	}

	STexture*	Tex = NULL;	
	STexture	tmpTex;
	bool		Clip( true );

	Tex = Resource->GetTexture( Tid );
	if( Tex == NULL )
	{
		return;
	}
	
	if( Tex != NULL )
	{
		memcpy( &tmpTex, Tex, sizeof( STexture ) );

		if( w == RD_DEFAULT ){	w = tmpTex.w;	}
		if( h == RD_DEFAULT ){	h = tmpTex.h;	}

		tmpTex.vt[0].diffuse	= color;
		tmpTex.vt[1].diffuse	= color;
		tmpTex.vt[2].diffuse	= color;
		tmpTex.vt[3].diffuse	= color;

		tmpTex.vt[0].x = (float)x - 0.5f;
		tmpTex.vt[0].y = (float)y - 0.5f;
		tmpTex.vt[1].x = tmpTex.vt[0].x + w;
		tmpTex.vt[1].y = tmpTex.vt[0].y;
		tmpTex.vt[2].x = tmpTex.vt[0].x + w;
		tmpTex.vt[2].y = tmpTex.vt[0].y + h;
		tmpTex.vt[3].x = tmpTex.vt[0].x;
		tmpTex.vt[3].y = tmpTex.vt[0].y + h;

		
		SImage* Img = Resource->GetImageInfo( tmpTex.ImgId );
		tmpTex.vt[0].tv = ( Tex->y + (float)_delta ) / Img->Height;
		tmpTex.vt[1].tv = ( Tex->y + (float)_delta ) / Img->Height;
		

		g_RenderManager.SetTexture( 0, tmpTex.pTexture );	
		g_RenderManager.DrawPrimitiveUP( D3DPT_TRIANGLEFAN , 2, tmpTex.vt, sizeof( SVertex ) );
	}
}

///-----------------------------------------------------------------------------
///-- DrawMiniMap
///-----------------------------------------------------------------------------
///-- Minimap 은 모두 Dynamic Texture 로 설정하길 
void CRender::DrawMiniMap( TEXID Tid, SRect* _map_rect, SPoint2* _tex_vtx_pos, int _tex_map_size, D3DCOLOR _color )
{
	if( Tid == TID_None ){	return;	}

	STexture*	Tex = NULL;	
	STexture	tmpTex;

	Tex = Resource->GetTexture( Tid );
	if( Tex == NULL )
	{
		return;
	}
	
	///-- Texture 없음 -> Dynamic Loading_M
	if( Tex && Resource->Img[Tex->ImgId].pTexture == NULL )
	{		
		Resource->LoadTextureDynamic( Tid );	
		Tex = Resource->GetTexture( Tid );		
	}
	
	if( Tex != NULL )
	{
		memcpy( &tmpTex, Tex, sizeof( STexture ) );

		tmpTex.vt[0].diffuse	= _color;
		tmpTex.vt[1].diffuse	= _color;
		tmpTex.vt[2].diffuse	= _color;
		tmpTex.vt[3].diffuse	= _color;

		tmpTex.vt[0].x = (float)_map_rect->x - 0.5f;
		tmpTex.vt[0].y = (float)_map_rect->y - 0.5f;
		tmpTex.vt[1].x = tmpTex.vt[0].x + _map_rect->w;
		tmpTex.vt[1].y = tmpTex.vt[0].y;
		tmpTex.vt[2].x = tmpTex.vt[0].x + _map_rect->w;
		tmpTex.vt[2].y = tmpTex.vt[0].y + _map_rect->h;
		tmpTex.vt[3].x = tmpTex.vt[0].x;
		tmpTex.vt[3].y = tmpTex.vt[0].y + _map_rect->h;

		//위치와 크기를 이용해서 Image의 UV좌표를 계산한다.
		tmpTex.vt[0].tu = (float)_tex_vtx_pos[0].x / _tex_map_size;//가로, 세로 폭이 똑같다는 전제
		tmpTex.vt[0].tv = (float)_tex_vtx_pos[0].y / _tex_map_size;

		tmpTex.vt[1].tu = (float)_tex_vtx_pos[1].x / _tex_map_size;
		tmpTex.vt[1].tv = (float)_tex_vtx_pos[1].y / _tex_map_size;

		tmpTex.vt[2].tu = (float)_tex_vtx_pos[2].x / _tex_map_size;
		tmpTex.vt[2].tv = (float)_tex_vtx_pos[2].y / _tex_map_size;

		tmpTex.vt[3].tu = (float)_tex_vtx_pos[3].x / _tex_map_size;
		tmpTex.vt[3].tv = (float)_tex_vtx_pos[3].y / _tex_map_size;
		

		Resource->Img[tmpTex.ImgId].m_lPreTime = timeGetTime();
		g_RenderManager.SetTexture( 0, tmpTex.pTexture );	
		g_RenderManager.DrawPrimitiveUP( D3DPT_TRIANGLEFAN , 2, tmpTex.vt, sizeof( SVertex ) );
	}
}

//-----------------------------------------------------------------------------
int CRender::DrawAlignText( CFontg* Fontg, TCHAR* Str, int Ax, int Ay, int x, int y, 
							 int w, int h, D3DCOLOR Color )
{
	if( (Str == NULL) || (Rstrlen( Str ) == 0) ){	return 0;	}

	static FONTID IdxBuf[MAX_INDEX_BUFFER];
	int NumStr( 0 );

	int nFonth = 0;
	
	memset( IdxBuf, 0, sizeof(FONTID) * MAX_INDEX_BUFFER );
	SFont* RFont = NULL;

	NumStr = Fontg->Confirm( Str, IdxBuf );

	for( int i = 0; i < NumStr; ++i  )
	{
		RFont = Fontg->GetFontasm( IdxBuf[i] );
		
		if( RFont != NULL )
		{
			RFont->vt[0].diffuse	= Color;
			RFont->vt[1].diffuse	= Color;
			RFont->vt[2].diffuse	= Color;
			RFont->vt[3].diffuse	= Color;

			RFont->vt[0].x = (float)Ax - 0.5f;
			RFont->vt[0].y = (float)Ay - 0.5f;
			RFont->vt[1].x = RFont->vt[0].x + RFont->w;
			RFont->vt[1].y = RFont->vt[0].y;
			RFont->vt[2].x = RFont->vt[0].x + RFont->w;
			RFont->vt[2].y = RFont->vt[0].y + RFont->h;
			RFont->vt[3].x = RFont->vt[0].x;
			RFont->vt[3].y = RFont->vt[0].y + RFont->h;

			Ax = Ax + RFont->w;

			nFonth = RFont->h;

			if( IdxBuf[i] != 32 )
			{
				//" " 렌더링 하지 않는다.
				//"\n"는 그대로 찍는다( Line 출력이기 때문에 개행은 처리되지 않는다. )				
				g_RenderManager.SetTexture( 0, RFont->pTexture );
				g_RenderManager.DrawPrimitiveUP( D3DPT_TRIANGLEFAN , 2, RFont->vt, sizeof( SVertex ) );				
			}
		}
	}
	
	return nFonth;
}

int CRender::GetColor( DWORD *Color , FONTID *IdxBuf , int &Num )
{
//	C1 n_DarkRed  D3DCOLOR_XRGB(255,0,0);
//	C2 n_DarkGreen D3DCOLOR_XRGB(0,215,0);
//	C3 n_LightGreen D3DCOLOR_XRGB(128, 255,128);
//	C4 n_LightBlue D3DCOLOR_XRGB(128, 128 ,255);
//	C5 n_LightYellow D3DCOLOR_XRGB(255, 255 , 128);
//	C6 n_LightOrange  D3DCOLOR_XRGB(255, 150 , 100 );

	DWORD RetRGB = 0 ; 
	static bool bFirstInLoop = false  ;
	static bool IsColor = false;
	static DWORD SaveColor = 0 ; 
	
	if ( IdxBuf[Num]	== '<')
	{
		SaveColor = *Color;
		IsColor = true;
	}
	if(IdxBuf[Num]	== '>')
	{
		*Color = SaveColor ;
		IsColor =false ;
		bFirstInLoop =false ;
		return 1;
	}
	
	if(IsColor == true && bFirstInLoop == false )
	{
		bFirstInLoop = true ; 
		//색상선택 C1 n_DarkRed  D3DCOLOR_XRGB(255,0,0); 
		if( IdxBuf[Num+1] == 'C' && IdxBuf[Num+2] == '1')
		{
			*Color = D3DCOLOR_XRGB(255,0,0);
		}
		//  n_DarkGreen D3DCOLOR_XRGB(0,215,0);
		else if(IdxBuf[Num+1] == 'C' && IdxBuf[Num+2] == '2')
		{
			*Color = D3DCOLOR_XRGB(0,255 , 0 );
		}
		//	n_LightGreen D3DCOLOR_XRGB(128, 255,128);
		else if(IdxBuf[Num+1] == 'C' && IdxBuf[Num+2] == '3')
		{
			*Color = D3DCOLOR_XRGB(128 ,255 , 128 );
		}
		// n_LightBlue D3DCOLOR_XRGB(128, 128 ,255);
		else if(IdxBuf[Num+1] == 'C' && IdxBuf[Num+2] == '4')
		{
			*Color = D3DCOLOR_XRGB(128, 128 , 255 );
		}
		// n_LightYellow D3DCOLOR_XRGB(255, 255 , 128);
		else if(IdxBuf[Num+1] == 'C' && IdxBuf[Num+2] == '5')
		{
			*Color = D3DCOLOR_XRGB(255 , 255 , 128 );
		
		}
		// n_LightOrange  D3DCOLOR_XRGB(255, 150 , 100 );
		else if(IdxBuf[Num+1] == 'C' && IdxBuf[Num+2] == '6')
		{
			*Color = D3DCOLOR_XRGB(255, 150 , 100 );
		}

		Num+=2;
		return 1;
	}


	

	


	return 0;
}
//-----------------------------------------------------------------------------
void CRender::DrawText( CFontg* Fontg, TCHAR* Str, int x, int y, int w, int h, 
					    D3DCOLOR Color, bool Integral, int start_line, int LineInterval )
{
	if( ( Str == NULL ) || ( Rstrlen( Str ) == 0 ) )
	{
		return;
	}

	int PosX		= x;
	int PosY		= y;
	int Width		= x + w;
	int Height		= y + h;
	int	curline 	= 0;

	static FONTID IdxBuf[MAX_INDEX_BUFFER];
	int NumStr( 0 );
	
	memset( IdxBuf, 0, sizeof(FONTID) * MAX_INDEX_BUFFER );
	SFont* RFont = NULL;
	
	
	NumStr = Fontg->Confirm( Str, IdxBuf );
	//SetScissorTest( x, y, w, h );
	
//색상 정보를 알아 냅시다
	// C1 Red C2 Bule C3 Green C4 Yellow
	
//	DWORD SaveColor = Color ; 

	for(int i = 0; i < NumStr; ++i  )
	{
		RFont = Fontg->GetFontasm( IdxBuf[i]  );
		int temp = IdxBuf[i];

		 if( GetColor(&Color , IdxBuf , i ) )
			 continue;
		
		if( RFont != NULL )
		{
			RFont->vt[0].diffuse	= Color;
			RFont->vt[1].diffuse	= Color;
			RFont->vt[2].diffuse	= Color;
			RFont->vt[3].diffuse	= Color;

			RFont->vt[0].x = (float)PosX - 0.5f;
			RFont->vt[0].y = (float)PosY - 0.5f;
			RFont->vt[1].x = RFont->vt[0].x + RFont->w;
			RFont->vt[1].y = RFont->vt[0].y;
			RFont->vt[2].x = RFont->vt[0].x + RFont->w;
			RFont->vt[2].y = RFont->vt[0].y + RFont->h;
			RFont->vt[3].x = RFont->vt[0].x;
			RFont->vt[3].y = RFont->vt[0].y + RFont->h;
			
			PosX = PosX + RFont->w;

			//더블 샵 개행으로 전환
			bool IsDoubleShap = false;
			if( (i < NumStr - 1) && (35 == IdxBuf[i]) && (35 == IdxBuf[i + 1]) )
			{
				IsDoubleShap = true;
			}
			//-----------------------------------------------------------------
			//Integral -> 문자열을 폭과 높이에 맞춘다
			if( ( ( Integral && (PosX > Width) ) || (IdxBuf[i] == 10) || (IdxBuf[i] == 92) || IsDoubleShap ) || 
				( (IdxBuf[i] == 32) ))
			{
				if( IdxBuf[i] == 32 )	///스페이스
				{
					//다음 라인이 위드값을 초과하면....
					int j = i;
					int nPosX = PosX;
					while( IdxBuf[++j] != 32 && j < NumStr )
					{
						SFont* RtFont = NULL;
						RtFont =  Fontg->GetFontasm( IdxBuf[j] );
						if(RtFont == NULL)
						{
							return;
						}
						
						nPosX += ( RtFont )->w;	
						int niIdx = (int)IdxBuf[j];
						if( 47 > niIdx || niIdx > 122) //아스키코드가 아닐 경우에만
						{
							if(Integral && nPosX > Width)
							{
								PosX = x;
								goto tTerm;
							}
						}
						
						if(Integral && nPosX > Width)
						{
							PosX = x;
							goto tTerm;
						}
					}
					
					if( !( Integral && (PosX > Width) ) )
					{
						goto tEnd;
					}
				}
				
				if(Integral && (PosX > Width) )
				{
					int niIdx = (int)IdxBuf[i];
					if( 47 <= niIdx && niIdx <= 122) //아스키코드일 경우에만
					{
						goto tTerm;
					}
				}				
tTerm:				
				PosX = x;

				if( curline >= start_line )
				{
					PosY = PosY + RFont->h + LineInterval;
				}
				
				RFont->vt[0].x = (float)PosX - 0.5f;
				RFont->vt[0].y = (float)PosY - 0.5f;
				RFont->vt[1].x = RFont->vt[0].x + RFont->w;
				RFont->vt[1].y = RFont->vt[0].y;
				RFont->vt[2].x = RFont->vt[0].x + RFont->w;
				RFont->vt[2].y = RFont->vt[0].y + RFont->h;
				RFont->vt[3].x = RFont->vt[0].x;
				RFont->vt[3].y = RFont->vt[0].y + RFont->h;

				if( (IdxBuf[i] != 10) && (IdxBuf[i] != 92) && !IsDoubleShap)
				{
					PosX = PosX + RFont->w;
				}
				if( Integral && ( (PosY + RFont->h) > Height ) )
				{
					//Render->SetScissorTest();
					return;
				}
				
				curline++;
			}
			//-----------------------------------------------------------------
tEnd:			
			if( curline >= start_line )
			{
				if( (IdxBuf[i] != 32) && (IdxBuf[i] != 10) && (IdxBuf[i] != 92) && !IsDoubleShap) 
				//  " " 32 와 "\n" 10 은"\\" 92  더블샵 렌더링 하지 않는다.
				{					
					g_RenderManager.SetTexture( 0, RFont->pTexture );
					g_RenderManager.DrawPrimitiveUP( D3DPT_TRIANGLEFAN , 2, RFont->vt, sizeof( SVertex ) );					
				}
			}

			if(IsDoubleShap)
			{
				i++;
			}
		}
	}	
}

void CRender::DrawToolText( CFontg* Fontg, TCHAR* Str, int x, int y, int w, int h, 
							D3DCOLOR Color, bool Integral, int start_line, int LineInterval )
{	
	if( ( Str == NULL ) || ( Rstrlen( Str ) == 0 ) )
	{
		return;
	}

	int PosX		= x;
	int PosY		= y;
	int Width		= x + w;
	int Height		= y + h;
	int	curline 	= 0;

	static FONTID IdxBuf[MAX_INDEX_BUFFER];
	int NumStr( 0 );
	
	memset( IdxBuf, 0, sizeof(FONTID) * MAX_INDEX_BUFFER );
	SFont* RFont = NULL;

 
	NumStr = Fontg->Confirm( Str, IdxBuf );
	//SetScissorTest( x, y, w, h );

	for( int i = 0; i < NumStr; ++i  )
	{
		RFont = Fontg->GetFontasm( IdxBuf[i] );
		
		if( RFont != NULL )
		{
			RFont->vt[0].diffuse	= Color;
			RFont->vt[1].diffuse	= Color;
			RFont->vt[2].diffuse	= Color;
			RFont->vt[3].diffuse	= Color;

			RFont->vt[0].x = (float)PosX - 0.5f;
			RFont->vt[0].y = (float)PosY - 0.5f;
			RFont->vt[1].x = RFont->vt[0].x + RFont->w;
			RFont->vt[1].y = RFont->vt[0].y;
			RFont->vt[2].x = RFont->vt[0].x + RFont->w;
			RFont->vt[2].y = RFont->vt[0].y + RFont->h;
			RFont->vt[3].x = RFont->vt[0].x;
			RFont->vt[3].y = RFont->vt[0].y + RFont->h;
			
			PosX = PosX + RFont->w;

			//더블 샵 개행으로 전환
			bool IsDoubleShap = false;
			if( (i < NumStr - 1) && (35 == IdxBuf[i]) && (35 == IdxBuf[i + 1]) )
			{
				IsDoubleShap = true;
			}
			//-----------------------------------------------------------------
			//Integral -> 문자열을 폭과 높이에 맞춘다
			if( ( ( Integral && (PosX > Width) ) || (IdxBuf[i] == 10) || (IdxBuf[i] == 92) || IsDoubleShap ) ) 
				
			{	
				PosX = x;

				if( curline >= start_line )
				{
					PosY = PosY + RFont->h + LineInterval;
				}
				
				RFont->vt[0].x = (float)PosX - 0.5f;
				RFont->vt[0].y = (float)PosY - 0.5f;
				RFont->vt[1].x = RFont->vt[0].x + RFont->w;
				RFont->vt[1].y = RFont->vt[0].y;
				RFont->vt[2].x = RFont->vt[0].x + RFont->w;
				RFont->vt[2].y = RFont->vt[0].y + RFont->h;
				RFont->vt[3].x = RFont->vt[0].x;
				RFont->vt[3].y = RFont->vt[0].y + RFont->h;

				if( (IdxBuf[i] != 10) && (IdxBuf[i] != 92) && !IsDoubleShap)
				{
					PosX = PosX + RFont->w;
				}
				if( Integral && ( (PosY + RFont->h) > Height ) )
				{
					//Render->SetScissorTest();
					return;
				}
				
				curline++;
			}
			//-----------------------------------------------------------------
			
			if( curline >= start_line )
			{
				if( (IdxBuf[i] != 32) && (IdxBuf[i] != 10) && (IdxBuf[i] != 92) && !IsDoubleShap) 
				//  " " 32 와 "\n" 10 은"\\" 92  더블샵 렌더링 하지 않는다.
				{					
					g_RenderManager.SetTexture( 0, RFont->pTexture );
					g_RenderManager.DrawPrimitiveUP( D3DPT_TRIANGLEFAN , 2, RFont->vt, sizeof( SVertex ) );					
				}
			}

			if(IsDoubleShap)
			{
				i++;
			}
		}
	}

	//Render->SetScissorTest();
}

void CRender::DrawText( CFontg* Fontg, TCHAR* Str, int x, int y, int w, int h, D3DCOLOR Color, bool Integral, int start_line, float z )
{
	if( ( Str == NULL ) || ( Rstrlen( Str ) == 0 ) )
	{
		return;
	}

	int PosX		= x;
	int PosY		= y;
	int Width		= x + w;
	int Height		= y + h;
	int	curline 	= 0;

	static FONTID IdxBuf[MAX_INDEX_BUFFER];
	int NumStr( 0 );
	
	memset( IdxBuf, 0, sizeof(FONTID) * MAX_INDEX_BUFFER );
	SFont* RFont = NULL;

	NumStr = Fontg->Confirm( Str, IdxBuf );
	//SetScissorTest( x, y, w, h );

	for( int i = 0; i < NumStr; ++i  )
	{
		RFont = Fontg->GetFontasm( IdxBuf[i] );
		
		if( RFont != NULL )
		{
			RFont->vt[0].diffuse	= Color;
			RFont->vt[1].diffuse	= Color;
			RFont->vt[2].diffuse	= Color;
			RFont->vt[3].diffuse	= Color;

			RFont->vt[0].x = (float)PosX - 0.5f;
			RFont->vt[0].y = (float)PosY - 0.5f;
			RFont->vt[1].x = RFont->vt[0].x + RFont->w;
			RFont->vt[1].y = RFont->vt[0].y;
			RFont->vt[2].x = RFont->vt[0].x + RFont->w;
			RFont->vt[2].y = RFont->vt[0].y + RFont->h;
			RFont->vt[3].x = RFont->vt[0].x;
			RFont->vt[3].y = RFont->vt[0].y + RFont->h;
			
			PosX = PosX + RFont->w;

			RFont->vt[3].z = RFont->vt[2].z = RFont->vt[1].z = RFont->vt[0].z = z;
			//-----------------------------------------------------------------
			//Integral -> 문자열을 폭과 높이에 맞춘다
			if( ( ( Integral && (PosX > Width) ) || (IdxBuf[i] == 10) || (IdxBuf[i] == 92)  ) || 
				( (IdxBuf[i] == 32) ))
			{
				if( IdxBuf[i] == 32 )	///스페이스
				{
					//다음 라인이 위드값을 초과하면....
					int j = i;
					int nPosX = PosX;
					while( IdxBuf[++j] != 32  && j < NumStr )
					{
						SFont* RtFont = NULL;
						RtFont =  Fontg->GetFontasm( IdxBuf[j] );
						if(RtFont == NULL)
						{
							return;
						}
						
						nPosX += ( RtFont )->w;	
						int niIdx = (int)IdxBuf[j];
						if( 47 > niIdx || niIdx > 122) //아스키코드가 아닐 경우에만
						{
							if(Integral && nPosX > Width)
							{
								PosX = x;
								goto tTerm;
							}
						}
						if(Integral && nPosX > Width)
						{
							PosX = x;
							goto tTerm;
						}
					}
					
					if( !( Integral && (PosX > Width) ) )
					{
						goto tEnd;
					}
				}
				
				if(Integral && (PosX > Width) )
				{
					int niIdx = (int)IdxBuf[i];
					if( 47 <= niIdx && niIdx <= 122) //아스키코드일 경우에만
					{
						goto tTerm;
					}
				}			
tTerm:
				PosX = x;

				if( curline >= start_line )
				{
					PosY = PosY + RFont->h;

					RFont->vt[0].x = (float)PosX - 0.5f;
					RFont->vt[0].y = (float)PosY - 0.5f;
					RFont->vt[1].x = RFont->vt[0].x + RFont->w;
					RFont->vt[1].y = RFont->vt[0].y;
					RFont->vt[2].x = RFont->vt[0].x + RFont->w;
					RFont->vt[2].y = RFont->vt[0].y + RFont->h;
					RFont->vt[3].x = RFont->vt[0].x;
					RFont->vt[3].y = RFont->vt[0].y + RFont->h;

					if( (IdxBuf[i] != 10) && (IdxBuf[i] != 92) )
					{
						PosX = PosX + RFont->w;
					}
					if( Integral && ( (PosY + RFont->h) > Height ) )
					{
						//Render->SetScissorTest();
						return;
					}
				}
				
				curline++;
			}
			//-----------------------------------------------------------------
tEnd:			
			if( curline >= start_line )
			{
				if( (IdxBuf[i] != 32) && (IdxBuf[i] != 10) && (IdxBuf[i] != 92) ) //  " "와 "\n"은 렌더링 하지 않는다.
				{					
					g_RenderManager.SetTexture( 0, RFont->pTexture );
					g_RenderManager.DrawPrimitiveUP( D3DPT_TRIANGLEFAN , 2, RFont->vt, sizeof( SVertex ) );
				}
			}

			RFont->vt[3].z = RFont->vt[2].z = RFont->vt[1].z = RFont->vt[0].z = 0;
		}
	}

	//Render->SetScissorTest();
}

//-----------------------------------------------------------------------------
bool CRender::DrawText_StepCnt( CFontg* Fontg, TCHAR* Str, int x, int y, int w, int h, D3DCOLOR Color, 
							   bool Integral, int start_line, int aStepCnt, int LineInterval )
{
	bool draw_result = true;

	if( ( Str == NULL ) || ( Rstrlen( Str ) == 0 ) )
	{
		return false;
	}

	int PosX		= x;
	int PosY		= y;
	int Width		= x + w;
	int Height		= y + h;
	int	curline 	= 0;

	static FONTID IdxBuf[MAX_INDEX_BUFFER];
	int NumStr( 0 );
	
	memset( IdxBuf, 0, sizeof(FONTID) * MAX_INDEX_BUFFER );
	SFont* RFont = NULL;
 
	NumStr = Fontg->Confirm( Str, IdxBuf );
	//SetScissorTest( x, y, w, h );

	if( aStepCnt > NumStr )
	{
		aStepCnt = NumStr;
		draw_result = false;
	}

//	for( int i = 0; i < NumStr; ++i  )
	for( int i = 0; i < aStepCnt; ++i  )		
	{
		RFont = Fontg->GetFontasm( IdxBuf[i] );
		
		if( i == ( aStepCnt - 1 ) )
		{
			//	((D3DCOLOR)((((a) &0xff) <<24)|(((r) &0xff) <<16)|(((g) &0xff) <<8)|((b) &0xff)))
			
			int alpha = 0;		// Color >> 24;
			int red = 0;		// ( Color & ( 0xff << 16 ) ) >> 16;
			int green = 0;		// ( Color & ( 0xff << 8 ) ) >> 8;
			int blue = 0;		// ( Color & 0xff );
			
			alpha = 255 * 0.25f;
			red = ( Color & ( 0xff << 16 ) ) >> 16;
			green = ( Color & ( 0xff << 8 ) ) >> 8;
			blue = ( Color & 0xff );
			
			Color = D3DCOLOR_ARGB( alpha, red, green, blue );			
		}

		if( RFont != NULL )
		{
			RFont->vt[0].diffuse	= Color;
			RFont->vt[1].diffuse	= Color;
			RFont->vt[2].diffuse	= Color;
			RFont->vt[3].diffuse	= Color;

			RFont->vt[0].x = (float)PosX - 0.5f;
			RFont->vt[0].y = (float)PosY - 0.5f;
			RFont->vt[1].x = RFont->vt[0].x + RFont->w;
			RFont->vt[1].y = RFont->vt[0].y;
			RFont->vt[2].x = RFont->vt[0].x + RFont->w;
			RFont->vt[2].y = RFont->vt[0].y + RFont->h;
			RFont->vt[3].x = RFont->vt[0].x;
			RFont->vt[3].y = RFont->vt[0].y + RFont->h;
			
			PosX = PosX + RFont->w;

			//더블 샵 개행으로 전환
			bool IsDoubleShap = false;
			if( (i < NumStr - 1) && (35 == IdxBuf[i]) && (35 == IdxBuf[i + 1]) )
			{
				IsDoubleShap = true;
			}
//-----------------------------------------------------------------
			//Integral -> 문자열을 폭과 높이에 맞춘다
			if( ( ( Integral && (PosX > Width) ) || (IdxBuf[i] == 10) || (IdxBuf[i] == 92) || IsDoubleShap ) || 
				( (IdxBuf[i] == 32) ))
			{
				if( IdxBuf[i] == 32 )	///스페이스
				{
					//다음 라인이 위드값을 초과하면....
					int j = i;
					int nPosX = PosX;
					while( IdxBuf[++j] != 32  && j < NumStr)
					{
						SFont* RtFont = NULL;
						RtFont =  Fontg->GetFontasm( IdxBuf[j] );
						if(RtFont == NULL)
						{
							return true;
						}
						
						nPosX += ( RtFont )->w;	
						int niIdx = (int)IdxBuf[j];
						if( 47 > niIdx || niIdx > 122) //아스키코드가 아닐 경우에만
						{
							if(Integral && nPosX > Width)
							{
								PosX = x;
								goto tTerm;
							}
						}
						if(Integral && nPosX > Width)
						{
							PosX = x;
							goto tTerm;
						}
					}
					
					if( !( Integral && (PosX > Width) ) )
					{
						goto tEnd;
					}
				}
				
				if(Integral && (PosX > Width) )
				{
					int niIdx = (int)IdxBuf[i];
					if( 47 <= niIdx && niIdx <= 122) //아스키코드일 경우에만
					{
						goto tTerm;
					}
				}				
tTerm:				
				PosX = x;

				if( curline >= start_line )
				{
					PosY = PosY + RFont->h + LineInterval;
				}
				
				RFont->vt[0].x = (float)PosX - 0.5f;
				RFont->vt[0].y = (float)PosY - 0.5f;
				RFont->vt[1].x = RFont->vt[0].x + RFont->w;
				RFont->vt[1].y = RFont->vt[0].y;
				RFont->vt[2].x = RFont->vt[0].x + RFont->w;
				RFont->vt[2].y = RFont->vt[0].y + RFont->h;
				RFont->vt[3].x = RFont->vt[0].x;
				RFont->vt[3].y = RFont->vt[0].y + RFont->h;

				if( (IdxBuf[i] != 10) && (IdxBuf[i] != 92) && !IsDoubleShap)
				{
					PosX = PosX + RFont->w;
				}
				if( Integral && ( (PosY + RFont->h) > Height ) )
				{
					//Render->SetScissorTest();
					return true;
				}
				
				curline++;
			}
			//-----------------------------------------------------------------
tEnd:			
			if( curline >= start_line )
			{
				if( (IdxBuf[i] != 32) && (IdxBuf[i] != 10) && (IdxBuf[i] != 92) && !IsDoubleShap) 
				//  " " 32 와 "\n" 10 은"\\" 92  더블샵 렌더링 하지 않는다.
				{					
					g_RenderManager.SetTexture( 0, RFont->pTexture );
					g_RenderManager.DrawPrimitiveUP( D3DPT_TRIANGLEFAN , 2, RFont->vt, sizeof( SVertex ) );					
				}
			}

			if(IsDoubleShap)
			{
				i++;
			}
		}
	}
	
	//Render->SetScissorTest();

	return draw_result;
}

///---------------------------------------------------------------------------
///-- DrawTextureNUM
///---------------------------------------------------------------------------
///-- Image Numeric 을 그린다.
void CRender::DrawImageNUM( TCHAR* pStr, int x, int y, D3DCOLOR Color )
{
	int ll_len = Rstrlen( pStr );
	int i;
	int ll_code;
#ifdef UNICODE_ROCK
	WCHAR* Str = RWCHAR(pStr);
#else
	CHAR* Str = RWCHAR(pStr);
#endif

	for( i = 0 ; i < ll_len; ++i )
	{
		///-- Render
		if( Str[i] > 46 && Str[i]< 58 )
		{
			ll_code = TID_CTRL_NUM_SLASH + ( int(Str[i]) - 47 );
			Draw( TEXID(ll_code), x, y, 5, 7, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			x+= 5;
		}
	}
}


//-----------------------------------------------------------------------------
void CRender::DrawBGText( CFontg* Fontg, TCHAR* Str, int x, int y, D3DCOLOR Color, D3DCOLOR BGColor )
{
	if( ( Str == NULL ) || ( Rstrlen( Str ) == 0 ) )
	{
		return;
	}
	if( theD3DDevice == NULL )
	{
		return;
	}
	
	int PosX = x;
	int PosY = y;
	
	static FONTID IdxBuf[256];
	int NumStr( 0 );
	
	memset( IdxBuf, 0, sizeof(FONTID) * 256 );
	SFont* tmpFont = NULL;

	NumStr = Fontg->Confirm( Str, IdxBuf );

	//SetScissorTest( x, y, w, h );
	
	int i = 0;

	//if( SUCCEEDED( g_RenderManager.BeginScene() ) )
    //{		
		int str_width = 0;
		int str_height = 0;

		for( i = 0; i < NumStr; ++i  )
		{
			tmpFont = Fontg->GetFontasm( IdxBuf[ i ] );
		
			if( tmpFont != NULL )
			{
				str_width += tmpFont->w;

				if( tmpFont->h > str_height )
				{
					str_height = tmpFont->h;
				}
			}
		}

		// Draw Text BG 
		tmpFont->vt[0].diffuse = tmpFont->vt[1].diffuse = tmpFont->vt[2].diffuse = tmpFont->vt[3].diffuse = BGColor;
		
		tmpFont->vt[0].x = (float)PosX - 1.5f;
		tmpFont->vt[0].y = (float)PosY - 1.5f;
		tmpFont->vt[1].x = tmpFont->vt[0].x + str_width + 3.0f;
		tmpFont->vt[1].y = tmpFont->vt[0].y;
		tmpFont->vt[2].x = tmpFont->vt[0].x + str_width + 3.0f;
		tmpFont->vt[2].y = tmpFont->vt[0].y + str_height + 3.0f;
		tmpFont->vt[3].x = tmpFont->vt[0].x;
		tmpFont->vt[3].y = tmpFont->vt[0].y + str_height + 3.0f;
		
		g_RenderManager.SetTexture( 0, NULL );
		g_RenderManager.DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, tmpFont->vt, sizeof( SVertex ) );					

		
		for( i = 0; i < NumStr; ++i  )
		{
			tmpFont = Fontg->GetFontasm( IdxBuf[i] );

			if( tmpFont != NULL )
			{
				// Draw Text 				
				tmpFont->vt[0].diffuse	= tmpFont->vt[1].diffuse = tmpFont->vt[2].diffuse	= tmpFont->vt[3].diffuse = Color;
				
				tmpFont->vt[0].x = (float)PosX - 0.5f;
				tmpFont->vt[0].y = (float)PosY - 0.5f;
				tmpFont->vt[1].x = tmpFont->vt[0].x + tmpFont->w;
				tmpFont->vt[1].y = tmpFont->vt[0].y;
				tmpFont->vt[2].x = tmpFont->vt[0].x + tmpFont->w;
				tmpFont->vt[2].y = tmpFont->vt[0].y + tmpFont->h;
				tmpFont->vt[3].x = tmpFont->vt[0].x;
				tmpFont->vt[3].y = tmpFont->vt[0].y + tmpFont->h;

				PosX = PosX + tmpFont->w;

				if( ( IdxBuf[i] != 32 ) && ( IdxBuf[i] != 10 ) ) //  " "와 "\n"은 렌더링 하지 않는다.
				{	
					g_RenderManager.SetTexture( 0, tmpFont->pTexture );
					g_RenderManager.DrawPrimitiveUP( D3DPT_TRIANGLEFAN , 2, tmpFont->vt, sizeof( SVertex ) );					
				}
			}
		}

	//	g_RenderManager.EndScene();
	//}
}
//--------------------------------
void CRender::DrawText2( CFontg* Fontg, TCHAR* Str, int x, int y, D3DCOLOR Color , bool &StateColor , int bSetID /* = 0  */)
{

	if( ( Str == NULL ) || ( Rstrlen( Str ) == 0 ) )
	{
		return;
	}
	if( theD3DDevice == NULL )
	{
		return;
	}
	
	int PosX = x;
	int PosY = y;
	
	static FONTID IdxBuf[256];
	int NumStr( 0 );
	
	memset( IdxBuf, 0, sizeof(FONTID) * 256 );
	SFont* tmpFont = NULL;

	NumStr = Fontg->Confirm( Str, IdxBuf );
	static bool bState1  = false;

	//SetScissorTest( x, y, w, h );
	//if( SUCCEEDED( g_RenderManager.BeginScene() ) )
    //{
		for( int i = 0; i < NumStr; ++i  )
		{
			tmpFont = Fontg->GetFontasm( IdxBuf[i] );

			if( tmpFont != NULL )
			{

				if((bSetID == 1 ) && StateColor)
				{
					bState1 =true; 
				}
								
				if((bSetID == 1) && IdxBuf[i] == '(' )
				{
					bState1 =false; 
					StateColor = false; 
					
				}
				
				if(!StateColor)
				{
					bState1 = false ; 
				}

				
				if((bSetID == 2)  && IdxBuf[i-1] == '(' )
				{
					bState1 =true; 
				}
				else if((bSetID == 2) && IdxBuf[i] == ')' )
				{
					bState1 =false; 
				}

				if(bState1 )
				{
					
					tmpFont->vt[0].diffuse	= tmpFont->vt[1].diffuse = tmpFont->vt[2].diffuse	=  tmpFont->vt[3].diffuse = Color;	
				}
				else
				{
					tmpFont->vt[0].diffuse	= tmpFont->vt[1].diffuse = tmpFont->vt[2].diffuse	= tmpFont->vt[3].diffuse = D3DCOLOR_XRGB( 150, 150, 150);
				}
				
				tmpFont->vt[0].x = (float)PosX - 0.5f;
				tmpFont->vt[0].y = (float)PosY - 0.5f;
				tmpFont->vt[1].x = tmpFont->vt[0].x + tmpFont->w;
				tmpFont->vt[1].y = tmpFont->vt[0].y;
				tmpFont->vt[2].x = tmpFont->vt[0].x + tmpFont->w;
				tmpFont->vt[2].y = tmpFont->vt[0].y + tmpFont->h;
				tmpFont->vt[3].x = tmpFont->vt[0].x;
				tmpFont->vt[3].y = tmpFont->vt[0].y + tmpFont->h;

				PosX = PosX + tmpFont->w;

				if( ( IdxBuf[i] != 32 ) && ( IdxBuf[i] != 10 ) ) //  " "와 "\n"은 렌더링 하지 않는다.
				{	
					g_RenderManager.SetTexture( 0, tmpFont->pTexture );
					g_RenderManager.DrawPrimitiveUP( D3DPT_TRIANGLEFAN , 2, tmpFont->vt, sizeof( SVertex ) );					
				}
			}
		}

	//	g_RenderManager.EndScene();
	//}
}



//-----------------------------------------------------------------------------
void CRender::DrawText( CFontg* Fontg, TCHAR* Str, int x, int y,  D3DCOLOR Color )
{
	if( ( Str == NULL ) || ( Rstrlen( Str ) == 0 ) )
	{
		return;
	}
	if( theD3DDevice == NULL )
	{
		return;
	}
	
	int PosX = x;
	int PosY = y;
	
	static FONTID IdxBuf[256];
	int NumStr( 0 );
	
	memset( IdxBuf, 0, sizeof(FONTID) * 256 );
	SFont* tmpFont = NULL;

	NumStr = Fontg->Confirm( Str, IdxBuf );

	//SetScissorTest( x, y, w, h );
	
	//if( SUCCEEDED( g_RenderManager.BeginScene() ) )
    //{
		for( int i = 0; i < NumStr; ++i  )
		{
			tmpFont = Fontg->GetFontasm( IdxBuf[i] );

			if( tmpFont != NULL )
			{
				tmpFont->vt[0].diffuse	= tmpFont->vt[1].diffuse = tmpFont->vt[2].diffuse	= tmpFont->vt[3].diffuse = Color;
				
				tmpFont->vt[0].x = (float)PosX - 0.5f;
				tmpFont->vt[0].y = (float)PosY - 0.5f;
				tmpFont->vt[1].x = tmpFont->vt[0].x + tmpFont->w;
				tmpFont->vt[1].y = tmpFont->vt[0].y;
				tmpFont->vt[2].x = tmpFont->vt[0].x + tmpFont->w;
				tmpFont->vt[2].y = tmpFont->vt[0].y + tmpFont->h;
				tmpFont->vt[3].x = tmpFont->vt[0].x;
				tmpFont->vt[3].y = tmpFont->vt[0].y + tmpFont->h;

				PosX = PosX + tmpFont->w;

				if( ( IdxBuf[i] != 32 ) && ( IdxBuf[i] != 10 ) ) //  " "와 "\n"은 렌더링 하지 않는다.
				{	
					g_RenderManager.SetTexture( 0, tmpFont->pTexture );
					g_RenderManager.DrawPrimitiveUP( D3DPT_TRIANGLEFAN , 2, tmpFont->vt, sizeof( SVertex ) );					
				}
			}
		}

	//	g_RenderManager.EndScene();
	//}
}

void CRender::DrawText( CFontg* Fontg, TCHAR* Str, int x, int y,  D3DCOLOR Color, float z)
{
	if( ( Str == NULL ) || ( Rstrlen( Str ) == 0 ) )
	{
		return;
	}
	if( theD3DDevice == NULL )
	{
		return;
	}
	
	int PosX = x;
	int PosY = y;
	
	static FONTID IdxBuf[256];
	int NumStr( 0 );
	
	memset( IdxBuf, 0, sizeof(FONTID) * 256 );
	SFont* tmpFont = NULL;

	NumStr = Fontg->Confirm( Str, IdxBuf );

	for( int i = 0; i < NumStr; ++i  )
	{
		tmpFont = Fontg->GetFontasm( IdxBuf[i] );

		if( tmpFont != NULL )
		{
			tmpFont->vt[0].diffuse	= tmpFont->vt[1].diffuse = tmpFont->vt[2].diffuse = tmpFont->vt[3].diffuse = Color;
			
			tmpFont->vt[0].x = (float)PosX - 0.5f;
			tmpFont->vt[0].y = (float)PosY - 0.5f;
			tmpFont->vt[1].x = tmpFont->vt[0].x + tmpFont->w;
			tmpFont->vt[1].y = tmpFont->vt[0].y;
			tmpFont->vt[2].x = tmpFont->vt[0].x + tmpFont->w;
			tmpFont->vt[2].y = tmpFont->vt[0].y + tmpFont->h;
			tmpFont->vt[3].x = tmpFont->vt[0].x;
			tmpFont->vt[3].y = tmpFont->vt[0].y + tmpFont->h;

			tmpFont->vt[3].z = tmpFont->vt[2].z = tmpFont->vt[1].z = tmpFont->vt[0].z = z;

			PosX = PosX + tmpFont->w;

			if( ( IdxBuf[i] != 32 ) && ( IdxBuf[i] != 10 ) ) //  " "와 "\n"은 렌더링 하지 않는다.
			{				
				g_RenderManager.SetTexture( 0, tmpFont->pTexture );
				g_RenderManager.DrawPrimitiveUP( D3DPT_TRIANGLEFAN , 2, tmpFont->vt, sizeof( SVertex ) );				
			}

			tmpFont->vt[3].z = tmpFont->vt[2].z = tmpFont->vt[1].z = tmpFont->vt[0].z = 0;
		}
	}
}

///---------------------------------------------------------------------------
///-- RIDrawText
///---------------------------------------------------------------------------
///-- 사용하지 않는 함수
void CRender::RIDrawText(CFontg* Fontg, TCHAR* Str, int x, int y,  D3DCOLOR Color)
{

}

//-----------------------------------------------------------------------------
void CRender::RIDrawAlignText( CFontg* Fontg, TCHAR* Str, int Ax, int Ay, int x, int y, int w, int h, D3DCOLOR Color, BOOL IsMarkUp )
{
	if( (RWCHAR(Str) == NULL) || (Rstrlen( Str ) == 0) ){	return;	}

	static FONTID IdxBuf[MAX_INDEX_BUFFER];
	int NumStr( 0 );
	
	memset( IdxBuf, 0, sizeof(FONTID) * MAX_INDEX_BUFFER );
	SFont* RFont = NULL;

	NumStr = Fontg->Confirm( Str, IdxBuf );

	if( Fontg->GetDecoration() == n_fdOutline )
	{
		Ax -= 1;
	}

	D3DCOLOR RnderColor = Color;

	for( int i = 0; i < NumStr; ++i  )
	{
		RFont = Fontg->GetFontasm( IdxBuf[i] );
		
		if( RFont != NULL )
		{
			if( IsMarkUp )
			{
				if( ( i <= (NumStr - 2) ) && (60 == IdxBuf[i]) && (42 == IdxBuf[i + 1]) )
				{
					m_IsDblStrt = true;
					i++;
					continue;
				}

				if( ( i <= (NumStr - 2) ) && (42 == IdxBuf[i]) && (62 == IdxBuf[i + 1]) )
				{
					m_IsDblStrt = false;
					i++;
					continue;
				}

				//플러스			
				if( ( i <= (NumStr - 2) ) && (60 == IdxBuf[i]) && (43 == IdxBuf[i + 1]) )
				{
					m_IsDblPlus = true;
					i++;
					continue;
				}
							
				if( ( i <= (NumStr - 2) ) && (43 == IdxBuf[i]) && (62 == IdxBuf[i + 1]) )
				{
					m_IsDblPlus = false;
					i++;
					continue;
				}


				//마이너스			
				if( ( i <= (NumStr - 2) ) && (60 == IdxBuf[i]) && (45 == IdxBuf[i + 1]) )
				{
					m_IsDblMinus = true;
					i++;
					continue;
				}
					
				if( ( i <= (NumStr - 2) ) && (45 == IdxBuf[i]) && (62 == IdxBuf[i + 1]) )
				{
					m_IsDblMinus = false;
					i++;
					continue;
				}

				//더블 나누기			
				if( ( i <= (NumStr - 2) ) && (60 == IdxBuf[i]) && (47 == IdxBuf[i + 1]) )
				{
					m_IsDblDivide = true;
					i++;
					continue;
				}
						
				if( ( i <= (NumStr - 2) ) && (47 == IdxBuf[i]) && (62 == IdxBuf[i + 1]) )
				{
					m_IsDblDivide = false;
					i++;
					continue;
				}

				//더블 달라			
				if( ( i <= (NumStr - 2) ) && (60 == IdxBuf[i]) && (36 == IdxBuf[i + 1]) )
				{
					m_IsDblDallar = true;
					i++;
					continue;
				}

				if( ( i <= (NumStr - 2) ) && (36 == IdxBuf[i]) && (62 == IdxBuf[i + 1]) )
				{
					m_IsDblDallar = false;
					i++;
					continue;
				}

				//더블 앤드			
				if( ( i <= (NumStr - 2) ) && (60 == IdxBuf[i]) && (38 == IdxBuf[i + 1]) )
				{
					m_IsDblAnd = true;
					i++;
					continue;
				}
						
				if( ( i <= (NumStr - 2) ) && (38 == IdxBuf[i]) && (62 == IdxBuf[i + 1]) )
				{
					m_IsDblAnd = false;
					i++;
					continue;
				}
				
				RnderColor = Color;
				
				///-- n_LightYellow
				if( m_IsDblStrt )
				{
					RnderColor = D3DCOLOR_ARGB( ((BYTE)((Color)>>24)), 255, 255, 128);
				}		

				///-- n_LightGreen
				if( m_IsDblPlus )
				{
					RnderColor = D3DCOLOR_ARGB( ((BYTE)((Color)>>24)), 128, 255, 128);
				}

				///-- n_DarkRed
				if( m_IsDblMinus )
				{
					RnderColor = D3DCOLOR_ARGB( ((BYTE)((Color)>>24)), 255, 0, 0);
				}

				///-- n_LightBlue
				if( m_IsDblDivide )
				{
					RnderColor = D3DCOLOR_ARGB( ((BYTE)((Color)>>24)), 128, 128, 255);
				}

				///-- n_DarkGreen
				if( m_IsDblDallar )
				{
					RnderColor = D3DCOLOR_ARGB( ((BYTE)((Color)>>24)), 0, 255, 0);
				}

				///-- n_LightOrange
				if( m_IsDblAnd )
				{
					RnderColor = D3DCOLOR_ARGB( ((BYTE)((Color)>>24)), 255, 150, 100);
				}
			}
			
			RFont->vt[0].diffuse	= RnderColor;
			RFont->vt[1].diffuse	= RnderColor;
			RFont->vt[2].diffuse	= RnderColor;
			RFont->vt[3].diffuse	= RnderColor;

			RFont->vt[0].x = (float)Ax - 0.5f;
			RFont->vt[0].y = (float)Ay - 0.5f;
			RFont->vt[1].x = RFont->vt[0].x + RFont->w;
			RFont->vt[1].y = RFont->vt[0].y;
			RFont->vt[2].x = RFont->vt[0].x + RFont->w;
			RFont->vt[2].y = RFont->vt[0].y + RFont->h;
			RFont->vt[3].x = RFont->vt[0].x;
			RFont->vt[3].y = RFont->vt[0].y + RFont->h;

			Ax = Ax + RFont->Riw;

			if( IdxBuf[i] != 32 )
			{
				//" " 렌더링 하지 않는다.
				//"\n"는 그대로 찍는다( Line 출력이기 때문에 개행은 처리되지 않는다. )				
				g_RenderManager.SetTexture( 0, RFont->pTexture );
				g_RenderManager.DrawPrimitiveUP( D3DPT_TRIANGLEFAN , 2, RFont->vt, sizeof( SVertex ) );				
			}
		}
	}
}

//-----------------------------------------------------------------------------
void CRender::DrawFont( SFont* RFont )
{	
	g_RenderManager.SetTexture( 0, RFont->pTexture );
	g_RenderManager.DrawPrimitiveUP( D3DPT_TRIANGLEFAN , 2, RFont->vt, sizeof( SVertex ) );	
}

//-----------------------------------------------------------------------------
void CRender::DrawPoint( int x, int y, D3DCOLOR color, float pointSize )
{
	SVertex vt;

	vt.x = (float)x;
	vt.y = (float)y;
	vt.z = 0.0f;
	vt.w = 1.0f;
	vt.diffuse = color;

	g_RenderManager.SetTexture( 0, NULL );

	g_RenderManager.SetPointSize( pointSize );

	g_RenderManager.DrawPrimitiveUP( D3DPT_POINTLIST, 1, &vt, sizeof( SVertex ) );
}

//-----------------------------------------------------------------------------
