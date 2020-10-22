///////////////////////////////////////////////////////////////////////////////
///
///		File		: Render.h
///		Desc		:
///
///		Author		: Park Soo-Hyun
///		Team		: Program - Client Team
///		Date		: 
///
///		Copyright(c) 2004, Rocksoft Co., Ltd. ALL RIGHTS RESERVED
///
///////////////////////////////////////////////////////////////////////////////
#ifndef __RENDER_H__
#define __RENDER_H__
//-----------------------------------------------------------------------------
#include "..\\RockPCH.h"
#ifdef DIRECT_VERSION_9_MJH
#include <d3dx9.h>
#else
#include <d3dx8.h>
#endif // DIRECT_VERSION_9_MJH

#include "Define.h"
#include "Fontman.h"
#include "Resource.h"


#define	RD_DEFAULT	-1
#define RUI_VERTEX_FORMAT ( D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1 )


class CRender
{
public:
	CRender(){ Init(); }
#ifdef DIRECT_VERSION_9_MJH
	CRender( LPDIRECT3DDEVICE9 D3DDevice );
#else
	CRender( LPDIRECT3DDEVICE8 D3DDevice );
#endif // DIRECT_VERSION_9_MJH
	~CRender();

	void Init();

	void SetState();
	void ReleaseState();

	//bool Begin();
	//void End();

	void DrawState();//블랜딩, ....
	void SetScissorTest( int x, int y, int w, int h );
	void SetScissorTest();
	
	void Draw( TEXID Tid, int x, int y, int w, int h, D3DCOLOR color );
	void DrawClip( TEXID Tid, int x, int y, int w, int h, D3DCOLOR color  , bool bClip = true);
	void DrawFitImg( TEXID Tid, int x, int y, int w, int h, D3DCOLOR Color );

	void DrawPoint( int x, int y, D3DCOLOR color, float pointSize );
	
	void DrawMiniMap( TEXID Tid, SRect* _map_rect, SPoint2* _tex_vtx_pos, int _tex_map_size, D3DCOLOR _color );
	void DrawUpGaugeUp( TEXID Tid, int x, int y, int w, int h, D3DCOLOR color, int _delta );
	void DrawUpGaugeDown( TEXID Tid, int x, int y, int w, int h, D3DCOLOR color );


	int  DrawAlignText( CFontg* Fontg, TCHAR* Str, int Ax, int Ay, int x, int y,  int w, int h, D3DCOLOR Color );
	void DrawText( CFontg* Fontg, TCHAR* Str, int x, int y,  int w, int h, D3DCOLOR Color, 
				   bool Integral, int start_line, int LineInterval = 0 );
	bool DrawText_StepCnt( CFontg* Fontg, TCHAR* Str, int x, int y, int w, int h, 
						   D3DCOLOR Color, bool Integral, int start_line, int aStepCnt, int LineInterval = 0 );		// by NiceguY 
	void DrawText( CFontg* Fontg, TCHAR* Str, int x, int y,  D3DCOLOR Color );
	void DrawBGText( CFontg* Fontg, TCHAR* Str, int x, int y, D3DCOLOR Color, D3DCOLOR BGColor );
	void DrawFont( SFont* RFont );
	void DrawImageNUM( TCHAR* Str, int x, int y, D3DCOLOR Color );


	//by dongs 
	void DrawAllDir(TEXID Tid, int x , int y, int w ,int h , D3DCOLOR Color , int Direction = 0 );
	void DrawText2( CFontg* Fontg, TCHAR* Str, int x, int y,  D3DCOLOR Color ,  bool &StateColor , int bSetID = 0 );
	int  GetColor( DWORD *Color , FONTID *IdxBuf , int &Num );
	

	//by simwoosung
	void ScretchDraw( TEXID Tid, int x, int y, int w, int h, D3DCOLOR Color );
	void ScretchDraw( TEXID Tid, int x, int y, int w, int h, D3DCOLOR Color, float z );
	void Draw( TEXID Tid, int x, int y, int w, int h, D3DCOLOR color , float z);
	void DrawFitImg( TEXID Tid, int x, int y, int w, int h, D3DCOLOR color , float z);
	void Draw_CloneImg( TEXID Tid, int CloneTID, int x, int y, int w, int h, D3DCOLOR Color );
	void DrawText( CFontg* Fontg, TCHAR* Str, int x, int y,  int w, int h, D3DCOLOR Color, bool Integral, int start_line , float z);
	void DrawText( CFontg* Fontg, TCHAR* Str, int x, int y,  D3DCOLOR Color, float z);
	void DrawToolText( CFontg* Fontg, TCHAR* Str, int x, int y, int w, int h, 
					    D3DCOLOR Color, bool Integral, int start_line, int LineInterval = 0 );	
	
	//by simwoosung	- 실제 글자의 폭과너비만큼 글자를 렌더링한다.
	void RIDrawText(CFontg* Fontg, TCHAR* Str, int x, int y,  D3DCOLOR Color);
	void RIDrawAlignText( CFontg* Fontg, TCHAR* Str, int Ax, int Ay, int x, int y, int w, int h, D3DCOLOR Color,  BOOL IsMarkUp = FALSE );

	void InitMarkUp();

protected:

#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DDEVICE9       theD3DDevice;
#else
	LPDIRECT3DDEVICE8       theD3DDevice;
#endif // DIRECT_VERSION_9_MJH
	
	bool m_IsDblStrt;
	bool m_IsDblPlus;
	bool m_IsDblMinus;
	bool m_IsDblDivide;
	bool m_IsDblDallar;
	bool m_IsDblAnd;
};


extern CRender*		Render;
//-----------------------------------------------------------------------------
#endif  __RENDER_H__