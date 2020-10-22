//-----------------------------------------------------------------------------
// File: D3DFont.cpp
//
// Desc: Texture-based font class
//
// Copyright (c) 1999-2000 Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------
//#define STRICT
#include "RockPCH.h"

#include <stdio.h>
#include <tchar.h>
#ifdef DIRECT_VERSION_9_MJH
#include <D3DX9.h>
#else
#include <D3DX8.h>
#endif // DIRECT_VERSION_9_MJH
#include "D3DFont.h"
#include "D3DUtil.h"
#include "DXUtil.h"

#include "..\\..\\CRenderManager.h"



//-----------------------------------------------------------------------------
// Custom vertex types for rendering text
//-----------------------------------------------------------------------------
#define MAX_NUM_VERTICES 50*6

struct FONT2DVERTEX { D3DXVECTOR4 p;   DWORD color;     FLOAT tu, tv; };
struct FONT3DVERTEX { D3DXVECTOR3 p;   D3DXVECTOR3 n;   FLOAT tu, tv; };

#define D3DFVF_FONT2DVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)
#define D3DFVF_FONT3DVERTEX (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)

inline FONT2DVERTEX InitFont2DVertex( const D3DXVECTOR4& p, D3DCOLOR color,
                                      FLOAT tu, FLOAT tv )
{
    FONT2DVERTEX v;   v.p = p;   v.color = color;   v.tu = tu;   v.tv = tv;
    return v;
}

inline FONT3DVERTEX InitFont3DVertex( const D3DXVECTOR3& p, const D3DXVECTOR3& n,
                                      FLOAT tu, FLOAT tv )
{
    FONT3DVERTEX v;   v.p = p;   v.n = n;   v.tu = tu;   v.tv = tv;
    return v;
}




//-----------------------------------------------------------------------------
// Name: CD3DFont()
// Desc: Font class constructor
//-----------------------------------------------------------------------------
CD3DFont::CD3DFont( TCHAR* strFontName, DWORD dwHeight, DWORD dwFlags )
{
    strcpy( m_strFontName, strFontName);
    m_dwFontHeight         = dwHeight;
    m_dwFontFlags          = dwFlags;

    m_pd3dDevice           = NULL;
    m_pTexture             = NULL;
    m_pVB                  = NULL;
#ifdef DIRECT_VERSION_9_MJH
	m_pStateBlockSaved     = NULL;
    m_pStateBlockDrawText  = NULL;
#else 
	m_dwSavedStateBlock    = 0L;
    m_dwDrawTextStateBlock = 0L;
#endif // DIRECT_VERSION_9_MJH

}




//-----------------------------------------------------------------------------
// Name: ~CD3DFont()
// Desc: Font class destructor
//-----------------------------------------------------------------------------
CD3DFont::~CD3DFont()
{
    InvalidateDeviceObjects();
    DeleteDeviceObjects();
}




//-----------------------------------------------------------------------------
// Name: InitDeviceObjects()
// Desc: Initializes device-dependent objects, including the vertex buffer used
//       for rendering text and the texture map which stores the font image.
//-----------------------------------------------------------------------------
#ifdef DIRECT_VERSION_9_MJH
HRESULT CD3DFont::InitDeviceObjects( LPDIRECT3DDEVICE9 pd3dDevice )
#else
HRESULT CD3DFont::InitDeviceObjects( LPDIRECT3DDEVICE8 pd3dDevice )
#endif // DIRECT_VERSION_9_MJH
{
    HRESULT hr;

    // Keep a local copy of the device
    m_pd3dDevice = pd3dDevice;

    // Establish the font and texture size
    m_fTextScale  = 1.0f; // Draw fonts into texture without scaling

    // Large fonts need larger textures
    if( m_dwFontHeight > 40 )
        m_dwTexWidth = m_dwTexHeight = 1024;
    else if( m_dwFontHeight > 20 )
        m_dwTexWidth = m_dwTexHeight = 512;
    else
        m_dwTexWidth  = m_dwTexHeight = 256;

    // If requested texture is too big, use a smaller texture and smaller font,
    // and scale up when rendering.
#ifdef DIRECT_VERSION_9_MJH
	D3DCAPS9 d3dCaps;
#else
	D3DCAPS8 d3dCaps;
#endif // DIRECT_VERSION_9_MJH
    m_pd3dDevice->GetDeviceCaps( &d3dCaps );

    if( m_dwTexWidth > d3dCaps.MaxTextureWidth )
    {
        m_fTextScale = (FLOAT)d3dCaps.MaxTextureWidth / (FLOAT)m_dwTexWidth;
        m_dwTexWidth = m_dwTexHeight = d3dCaps.MaxTextureWidth;
    }

    // Create a new texture for the font
#ifdef DIRECT_VERSION_9_MJH
    hr = m_pd3dDevice->CreateTexture( m_dwTexWidth, m_dwTexHeight, 1,
                                      0, D3DFMT_A4R4G4B4,
                                      D3DPOOL_MANAGED, &m_pTexture , NULL );
#else
	hr = m_pd3dDevice->CreateTexture( m_dwTexWidth, m_dwTexHeight, 1,
                                      0, D3DFMT_A4R4G4B4,
                                      D3DPOOL_MANAGED, &m_pTexture );
#endif // DIRECT_VERSION_9_MJH
    if( FAILED(hr) )
        return hr;

    // Prepare to create a bitmap
    DWORD*      pBitmapBits;
    BITMAPINFO bmi;
    ZeroMemory( &bmi.bmiHeader,  sizeof(BITMAPINFOHEADER) );
    bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth       =  (int)m_dwTexWidth;
    bmi.bmiHeader.biHeight      = -(int)m_dwTexHeight;
    bmi.bmiHeader.biPlanes      = 1;
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biBitCount    = 32;

    // Create a DC and a bitmap for the font
    HDC     hDC       = CreateCompatibleDC( NULL );
    HBITMAP hbmBitmap = CreateDIBSection( hDC, &bmi, DIB_RGB_COLORS,
                                          (VOID**)&pBitmapBits, NULL, 0 );
    SetMapMode( hDC, MM_TEXT );

    // Create a font.  By specifying ANTIALIASED_QUALITY, we might get an
    // antialiased font, but this is not guaranteed.
    INT nHeight    = -MulDiv( m_dwFontHeight, 
        (INT)(GetDeviceCaps(hDC, LOGPIXELSY) * m_fTextScale), 72 );
    DWORD dwBold   = (m_dwFontFlags&D3DFONT_BOLD)   ? FW_BOLD : FW_NORMAL;
    DWORD dwItalic = (m_dwFontFlags&D3DFONT_ITALIC) ? TRUE    : FALSE;
    HFONT hFont    = CreateFont( nHeight, 0, 0, 0, dwBold, dwItalic,
                          FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                          CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
                          VARIABLE_PITCH, m_strFontName );
    if( NULL==hFont )
        return E_FAIL;

    SelectObject( hDC, hbmBitmap );
    SelectObject( hDC, hFont );

    // Set text properties
    SetTextColor( hDC, RGB(255,255,255) );
    SetBkColor(   hDC, 0x00000000 );
    SetTextAlign( hDC, TA_TOP );

    // Loop through all printable character and output them to the bitmap..
    // Meanwhile, keep track of the corresponding tex coords for each character.
    DWORD x = 0;
    DWORD y = 0;
    TCHAR str[2] = _T("x");
    SIZE size;

    for( TCHAR c=32; c<127; c++ )
    {
        str[0] = c;
        GetTextExtentPoint32( hDC, str, 1, &size );

        if( (DWORD)(x+size.cx+1) > m_dwTexWidth )
        {
            x  = 0;
            y += size.cy+1;
        }

        ExtTextOut( hDC, x+0, y+0, ETO_OPAQUE, NULL, str, 1, NULL );

        m_fTexCoords[c-32][0] = ((FLOAT)(x+0))/m_dwTexWidth;
        m_fTexCoords[c-32][1] = ((FLOAT)(y+0))/m_dwTexHeight;
        m_fTexCoords[c-32][2] = ((FLOAT)(x+0+size.cx))/m_dwTexWidth;
        m_fTexCoords[c-32][3] = ((FLOAT)(y+0+size.cy))/m_dwTexHeight;

        x += size.cx+1;
    }

    // Lock the surface and write the alpha values for the set pixels
    D3DLOCKED_RECT d3dlr;
    m_pTexture->LockRect( 0, &d3dlr, 0, 0 );
    WORD* pDst16 = (WORD*)d3dlr.pBits;
    BYTE bAlpha; // 4-bit measure of pixel intensity

    for( y=0; y < m_dwTexHeight; y++ )
    {
        for( x=0; x < m_dwTexWidth; x++ )
        {
            bAlpha = (BYTE)((pBitmapBits[m_dwTexWidth*y + x] & 0xff) >> 4);
            if (bAlpha > 0)
            {
                *pDst16++ = (bAlpha << 12) | 0x0fff;
            }
            else
            {
                *pDst16++ = 0x0000;
            }
        }
    }

    // Done updating texture, so clean up used objects
    m_pTexture->UnlockRect(0);
    DeleteObject( hbmBitmap );
    DeleteDC( hDC );
    DeleteObject( hFont );

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: RestoreDeviceObjects()
// Desc:
//-----------------------------------------------------------------------------
HRESULT CD3DFont::RestoreDeviceObjects()
{
    HRESULT hr;

    // Create vertex buffer for the letters
#ifdef DIRECT_VERSION_9_MJH
	if( FAILED( hr = m_pd3dDevice->CreateVertexBuffer( MAX_NUM_VERTICES*sizeof(FONT2DVERTEX),
                                                       D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, 0,
                                                       D3DPOOL_DEFAULT, &m_pVB , NULL) ) )
#else
	if( FAILED( hr = m_pd3dDevice->CreateVertexBuffer( MAX_NUM_VERTICES*sizeof(FONT2DVERTEX),
                                                       D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, 0,
                                                       D3DPOOL_DEFAULT, &m_pVB ) ) )
#endif // DIRECT_VERSION_9_MJH													   
    {
        return hr;
    }

    // Create the state blocks for rendering text
    for( UINT which=0; which<2; which++ )
    {
        m_pd3dDevice->BeginStateBlock();
        m_pd3dDevice->SetTexture( 0, m_pTexture );
		
		g_RenderManager.SetAlphaBlendEnable( TRUE );
		g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
		g_RenderManager.SetDestBlend( D3DBLEND_INVSRCALPHA );

		g_RenderManager.SetAlphaTestEnable( TRUE );
		g_RenderManager.SetAlphaRef( 0x08 );
		g_RenderManager.SetAlphaFunc( D3DCMP_GREATEREQUAL );
        g_RenderManager.SetFillMode( D3DFILL_SOLID );
		g_RenderManager.SetCullMode( D3DCULL_CCW );

		g_RenderManager.SetZEnable( FALSE );
		g_RenderManager.SetStencilEnable( FALSE );
	
		g_RenderManager.SetClipping( TRUE );
		g_RenderManager.SetVertexBlend( FALSE );
		g_RenderManager.SetClipPlaneEnable( FALSE );
        g_RenderManager.SetIndexedVertexBlendEnable( FALSE );
		g_RenderManager.SetEdgeAntialias( FALSE );
		g_RenderManager.SetFogEnable( FALSE );

		g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_MODULATE );
		g_RenderManager.SetTextureColorArg1( 0 , D3DTA_TEXTURE );
		g_RenderManager.SetTextureColorArg2( 0 , D3DTA_DIFFUSE );
		
		
		g_RenderManager.SetTextureAlphaOP  ( 0 , D3DTOP_MODULATE );
		g_RenderManager.SetTextureAlphaArg1( 0 , D3DTA_TEXTURE );
		g_RenderManager.SetTextureAlphaArg2( 0 , D3DTA_DIFFUSE );

		g_RenderManager.SetTextureMipFilter ( 0 , D3DTEXF_NONE );
		g_RenderManager.SetTextureMagFilter ( 0 , D3DTEXF_POINT );
		g_RenderManager.SetTextureMinFilter ( 0 , D3DTEXF_POINT );

		g_RenderManager.SetTextureCoordIndex (  0 , 0 );
		g_RenderManager.SetTextureTransFormFlags( 0 , D3DTTFF_DISABLE );

		g_RenderManager.DisableTextureStageState( 1 );
#ifdef DIRECT_VERSION_9_MJH
		if( which==0 )
            m_pd3dDevice->EndStateBlock( &m_pStateBlockSaved );
        else
            m_pd3dDevice->EndStateBlock( &m_pStateBlockDrawText );
#else
		if( which==0 )
            m_pd3dDevice->EndStateBlock( &m_dwSavedStateBlock );
        else
            m_pd3dDevice->EndStateBlock( &m_dwDrawTextStateBlock );
#endif // DIRECT_VERSION_9_MJH
    }

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: InvalidateDeviceObjects()
// Desc: Destroys all device-dependent objects
//-----------------------------------------------------------------------------
HRESULT CD3DFont::InvalidateDeviceObjects()
{
    SAFE_RELEASE( m_pVB );

    // Delete the state blocks
#ifdef DIRECT_VERSION_9_MJH
    SAFE_RELEASE( m_pStateBlockSaved );
    SAFE_RELEASE( m_pStateBlockDrawText );
#else 
	if( m_pd3dDevice )
    {
        if( m_dwSavedStateBlock )
            m_pd3dDevice->DeleteStateBlock( m_dwSavedStateBlock );
        if( m_dwDrawTextStateBlock )
            m_pd3dDevice->DeleteStateBlock( m_dwDrawTextStateBlock );
    }

    m_dwSavedStateBlock    = 0L;
    m_dwDrawTextStateBlock = 0L;
#endif // DIRECT_VERSION_9_MJH


    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: DeleteDeviceObjects()
// Desc: Destroys all device-dependent objects
//-----------------------------------------------------------------------------
HRESULT CD3DFont::DeleteDeviceObjects()
{
    SAFE_RELEASE( m_pTexture );
    m_pd3dDevice = NULL;

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: GetTextExtent()
// Desc: Get the dimensions of a text string
//-----------------------------------------------------------------------------
HRESULT CD3DFont::GetTextExtent( TCHAR* strText, SIZE* pSize )
{
    if( NULL==strText || NULL==pSize )
        return E_FAIL;

    FLOAT fRowWidth  = 0.0f;
    FLOAT fRowHeight = (m_fTexCoords[0][3]-m_fTexCoords[0][1])*m_dwTexHeight;
    FLOAT fWidth     = 0.0f;
    FLOAT fHeight    = fRowHeight;

    while( *strText )
    {
        TCHAR c = *strText++;

        if( c == _T('\n') )
        {
            fRowWidth = 0.0f;
            fHeight  += fRowHeight;
        }
        if( c < _T(' ') )
            continue;

        FLOAT tx1 = m_fTexCoords[c-32][0];
        FLOAT tx2 = m_fTexCoords[c-32][2];

        fRowWidth += (tx2-tx1)*m_dwTexWidth;

        if( fRowWidth > fWidth )
            fWidth = fRowWidth;
    }

    pSize->cx = (int)fWidth;
    pSize->cy = (int)fHeight;

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: DrawTextScaled()
// Desc: Draws scaled 2D text.  Note that x and y are in viewport coordinates
//       (ranging from -1 to +1).  fXScale and fYScale are the size fraction 
//       relative to the entire viewport.  For example, a fXScale of 0.25 is
//       1/8th of the screen width.  This allows you to output text at a fixed
//       fraction of the viewport, even if the screen or window size changes.
//-----------------------------------------------------------------------------
HRESULT CD3DFont::DrawTextScaled( FLOAT x, FLOAT y, FLOAT z,
                                  FLOAT fXScale, FLOAT fYScale, DWORD dwColor,
                                  TCHAR* strText, DWORD dwFlags )
{
    if( m_pd3dDevice == NULL )
        return E_FAIL;

    // Set up renderstate
#ifdef DIRECT_VERSION_9_MJH
	m_pStateBlockSaved->Capture();
    m_pStateBlockDrawText->Apply();
#else 
	m_pd3dDevice->CaptureStateBlock( m_dwSavedStateBlock );
    m_pd3dDevice->ApplyStateBlock( m_dwDrawTextStateBlock );
#endif // DIRECT_VERSION_9_MJH
    g_RenderManager.SetFVF( D3DFVF_FONT2DVERTEX );
#ifdef DIRECT_VERSION_9_MJH
	m_pd3dDevice->SetStreamSource( 0, m_pVB, 0, sizeof(FONT2DVERTEX) );
#else
	m_pd3dDevice->SetStreamSource( 0, m_pVB, sizeof(FONT2DVERTEX) );
#endif // DIRECT_VERSION_9_MJH
    

    // Set filter states
    if( dwFlags & D3DFONT_FILTERED )
    {
		g_RenderManager.SetTextureMagFilter ( 0 , D3DTEXF_LINEAR );
		g_RenderManager.SetTextureMinFilter ( 0 , D3DTEXF_LINEAR );
    }
#ifdef DIRECT_VERSION_9_MJH
	D3DVIEWPORT9 vp;
#else
	D3DVIEWPORT8 vp;
#endif // DIRECT_VERSION_9_MJH
    m_pd3dDevice->GetViewport( &vp );
    FLOAT sx  = (x+1.0f)*vp.Width/2;
    FLOAT sy  = (y-1.0f)*vp.Height/2;
    FLOAT sz  = z;
    FLOAT rhw = 1.0f;
    FLOAT fStartX = sx;

    FLOAT fLineHeight = ( m_fTexCoords[0][3] - m_fTexCoords[0][1] ) * m_dwTexHeight;

    // Fill vertex buffer
    FONT2DVERTEX* pVertices;
    DWORD         dwNumTriangles = 0L;
#ifdef DIRECT_VERSION_9_MJH
	if( FAILED( m_pVB->Lock( 0, 0, (void**)&pVertices, D3DLOCK_DISCARD ) ) )
#else
	if( FAILED( m_pVB->Lock( 0, 0, (BYTE**)&pVertices, D3DLOCK_DISCARD ) ) )
#endif // DIRECT_VERSION_9_MJH
	{
		return E_FAIL;		
	}   

    while( *strText )
    {
        TCHAR c = *strText++;

        if( c == _T('\n') )
        {
            sx  = fStartX;
            sy += fYScale*vp.Height;
        }
        if( c < _T(' ') )
            continue;

        FLOAT tx1 = m_fTexCoords[c-32][0];
        FLOAT ty1 = m_fTexCoords[c-32][1];
        FLOAT tx2 = m_fTexCoords[c-32][2];
        FLOAT ty2 = m_fTexCoords[c-32][3];

        FLOAT w = (tx2-tx1)*m_dwTexWidth;
        FLOAT h = (ty2-ty1)*m_dwTexHeight;

        w *= (fXScale*vp.Height)/fLineHeight;
        h *= (fYScale*vp.Height)/fLineHeight;

        *pVertices++ = InitFont2DVertex( D3DXVECTOR4(sx+0-0.5f,sy+h-0.5f,sz,rhw), dwColor, tx1, ty2 );
        *pVertices++ = InitFont2DVertex( D3DXVECTOR4(sx+0-0.5f,sy+0-0.5f,sz,rhw), dwColor, tx1, ty1 );
        *pVertices++ = InitFont2DVertex( D3DXVECTOR4(sx+w-0.5f,sy+h-0.5f,sz,rhw), dwColor, tx2, ty2 );
        *pVertices++ = InitFont2DVertex( D3DXVECTOR4(sx+w-0.5f,sy+0-0.5f,sz,rhw), dwColor, tx2, ty1 );
        *pVertices++ = InitFont2DVertex( D3DXVECTOR4(sx+w-0.5f,sy+h-0.5f,sz,rhw), dwColor, tx2, ty2 );
        *pVertices++ = InitFont2DVertex( D3DXVECTOR4(sx+0-0.5f,sy+0-0.5f,sz,rhw), dwColor, tx1, ty1 );
        dwNumTriangles += 2;

        if( dwNumTriangles*3 > (MAX_NUM_VERTICES-6) )
        {
            // Unlock, render, and relock the vertex buffer
            m_pVB->Unlock();
            m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, dwNumTriangles );
#ifdef DIRECT_VERSION_9_MJH
			if( FAILED( m_pVB->Lock( 0, 0, (void**)&pVertices, D3DLOCK_DISCARD ) ) )
#else
			if( FAILED( m_pVB->Lock( 0, 0, (BYTE**)&pVertices, D3DLOCK_DISCARD ) ) )
#endif // DIRECT_VERSION_9_MJH
			{
				return E_FAIL;		
			}
            
            dwNumTriangles = 0L;
        }

        sx += w;
    }

    // Unlock and render the vertex buffer
    m_pVB->Unlock();
    if( dwNumTriangles > 0 )
        m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, dwNumTriangles );

    // Restore the modified renderstates
#ifdef DIRECT_VERSION_9_MJH
	m_pStateBlockSaved->Apply();
#else
	m_pd3dDevice->ApplyStateBlock( m_dwSavedStateBlock );
#endif // DIRECT_VERSION_9_MJH
    

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: DrawText()
// Desc: Draws 2D text
//-----------------------------------------------------------------------------
HRESULT CD3DFont::DrawText( FLOAT sx, FLOAT sy, DWORD dwColor,
                            TCHAR* strText, DWORD dwFlags )
{

    if( m_pd3dDevice == NULL )
        return E_FAIL;

    // Setup renderstate
#ifdef DIRECT_VERSION_9_MJH
	m_pStateBlockSaved->Capture();
    m_pStateBlockDrawText->Apply();
#else
	m_pd3dDevice->CaptureStateBlock( m_dwSavedStateBlock );
    m_pd3dDevice->ApplyStateBlock( m_dwDrawTextStateBlock );
#endif // DIRECT_VERSION_9_MJH
    g_RenderManager.SetFVF( D3DFVF_FONT2DVERTEX );
#ifdef DIRECT_VERSION_9_MJH
	m_pd3dDevice->SetStreamSource( 0, m_pVB, 0, sizeof(FONT2DVERTEX) );
#else
	m_pd3dDevice->SetStreamSource( 0, m_pVB, sizeof(FONT2DVERTEX) );
#endif // DIRECT_VERSION_9_MJH

    // Set filter states
    if( dwFlags & D3DFONT_FILTERED )
    {
		g_RenderManager.SetTextureMagFilter ( 0 , D3DTEXF_LINEAR );
		g_RenderManager.SetTextureMinFilter ( 0 , D3DTEXF_LINEAR );
    }

    FLOAT fStartX = sx;

    // Fill vertex buffer
    FONT2DVERTEX* pVertices = NULL;
    DWORD         dwNumTriangles = 0;
#ifdef DIRECT_VERSION_9_MJH
	if( FAILED( m_pVB->Lock( 0, 0, (void**)&pVertices, D3DLOCK_DISCARD ) ) )
#else
	if( FAILED( m_pVB->Lock( 0, 0, (BYTE**)&pVertices, D3DLOCK_DISCARD ) ) )
#endif // DIRECT_VERSION_9_MJH
	{
		return E_FAIL;		
	}

    while( *strText )
    {
        TCHAR c = *strText++;

        if( c == _T('\n') )
        {
            sx = fStartX;
            sy += (m_fTexCoords[0][3]-m_fTexCoords[0][1])*m_dwTexHeight;
        }
        if( c < _T(' ') )
            continue;

        FLOAT tx1 = m_fTexCoords[c-32][0];
        FLOAT ty1 = m_fTexCoords[c-32][1];
        FLOAT tx2 = m_fTexCoords[c-32][2];
        FLOAT ty2 = m_fTexCoords[c-32][3];

        FLOAT w = (tx2-tx1) *  m_dwTexWidth / m_fTextScale;
        FLOAT h = (ty2-ty1) * m_dwTexHeight / m_fTextScale;

        *pVertices++ = InitFont2DVertex( D3DXVECTOR4(sx+0-0.5f,sy+h-0.5f,0.9f,1.0f), dwColor, tx1, ty2 );
        *pVertices++ = InitFont2DVertex( D3DXVECTOR4(sx+0-0.5f,sy+0-0.5f,0.9f,1.0f), dwColor, tx1, ty1 );
        *pVertices++ = InitFont2DVertex( D3DXVECTOR4(sx+w-0.5f,sy+h-0.5f,0.9f,1.0f), dwColor, tx2, ty2 );
        *pVertices++ = InitFont2DVertex( D3DXVECTOR4(sx+w-0.5f,sy+0-0.5f,0.9f,1.0f), dwColor, tx2, ty1 );
        *pVertices++ = InitFont2DVertex( D3DXVECTOR4(sx+w-0.5f,sy+h-0.5f,0.9f,1.0f), dwColor, tx2, ty2 );
        *pVertices++ = InitFont2DVertex( D3DXVECTOR4(sx+0-0.5f,sy+0-0.5f,0.9f,1.0f), dwColor, tx1, ty1 );
        dwNumTriangles += 2;

        if( dwNumTriangles*3 > (MAX_NUM_VERTICES-6) )
        {
            // Unlock, render, and relock the vertex buffer
            m_pVB->Unlock();
            m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, dwNumTriangles );
            pVertices = NULL;
#ifdef DIRECT_VERSION_9_MJH
			if( FAILED( m_pVB->Lock( 0, 0, (void**)&pVertices, D3DLOCK_DISCARD ) ) )
#else
			if( FAILED( m_pVB->Lock( 0, 0, (BYTE**)&pVertices, D3DLOCK_DISCARD ) ) )
#endif // DIRECT_VERSION_9_MJH
			{
				return E_FAIL;		
			}
            
            dwNumTriangles = 0L;
        }

        sx += w;
    }

    // Unlock and render the vertex buffer
    m_pVB->Unlock();
    if( dwNumTriangles > 0 )
        m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, dwNumTriangles );

    // Restore the modified renderstates
#ifdef DIRECT_VERSION_9_MJH
	 m_pStateBlockSaved->Apply();
#else
	 m_pd3dDevice->ApplyStateBlock( m_dwSavedStateBlock );
#endif // DIRECT_VERSION_9_MJH

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: Render3DText()
// Desc: Renders 3D text
//-----------------------------------------------------------------------------
HRESULT CD3DFont::Render3DText( TCHAR* strText, DWORD dwFlags )
{
    if( m_pd3dDevice == NULL )
        return E_FAIL;

    // Setup renderstate
#ifdef DIRECT_VERSION_9_MJH
	m_pStateBlockSaved->Capture();
    m_pStateBlockDrawText->Apply();
#else
	m_pd3dDevice->CaptureStateBlock( m_dwSavedStateBlock );
    m_pd3dDevice->ApplyStateBlock( m_dwDrawTextStateBlock );
#endif // DIRECT_VERSION_9_MJH
    g_RenderManager.SetFVF( D3DFVF_FONT3DVERTEX );
#ifdef DIRECT_VERSION_9_MJH
	m_pd3dDevice->SetStreamSource( 0, m_pVB, 0, sizeof(FONT3DVERTEX) );
#else
	m_pd3dDevice->SetStreamSource( 0, m_pVB, sizeof(FONT3DVERTEX) );
#endif // DIRECT_VERSION_9_MJH

    // Set filter states
    if( dwFlags & D3DFONT_FILTERED )
    {
		g_RenderManager.SetTextureMagFilter ( 0 , D3DTEXF_LINEAR );
		g_RenderManager.SetTextureMinFilter ( 0 , D3DTEXF_LINEAR );
    }

    // Position for each text element
    FLOAT x = 0.0f;
    FLOAT y = 0.0f;

    // Center the text block at the origin
    if( dwFlags & D3DFONT_CENTERED )
    {
        SIZE sz;
        GetTextExtent( strText, &sz );
        x = -(((FLOAT)sz.cx)/10.0f)/2.0f;
        y = -(((FLOAT)sz.cy)/10.0f)/2.0f;
    }

    // Turn off culling for two-sided text
    if( dwFlags & D3DFONT_TWOSIDED )
		g_RenderManager.SetCullMode( D3DCULL_NONE );

    FLOAT fStartX = x;
    TCHAR c;

    // Fill vertex buffer
    FONT3DVERTEX* pVertices;
    DWORD         dwVertex       = 0L;
    DWORD         dwNumTriangles = 0L;
#ifdef DIRECT_VERSION_9_MJH
	if( FAILED( m_pVB->Lock( 0, 0, (void**)&pVertices, D3DLOCK_DISCARD ) ) )
#else
	if( FAILED( m_pVB->Lock( 0, 0, (BYTE**)&pVertices, D3DLOCK_DISCARD ) ) )
#endif // DIRECT_VERSION_9_MJH
	{
		return E_FAIL;		
	}    

    while( c = *strText++ )
    {
        if( c == '\n' )
        {
            x = fStartX;
            y -= (m_fTexCoords[0][3]-m_fTexCoords[0][1])*m_dwTexHeight/10.0f;
        }
        if( c < 32 )
            continue;

        FLOAT tx1 = m_fTexCoords[c-32][0];
        FLOAT ty1 = m_fTexCoords[c-32][1];
        FLOAT tx2 = m_fTexCoords[c-32][2];
        FLOAT ty2 = m_fTexCoords[c-32][3];

        FLOAT w = (tx2-tx1) * m_dwTexWidth  / ( 10.0f * m_fTextScale );
        FLOAT h = (ty2-ty1) * m_dwTexHeight / ( 10.0f * m_fTextScale );

        *pVertices++ = InitFont3DVertex( D3DXVECTOR3(x+0,y+0,0), D3DXVECTOR3(0,0,-1), tx1, ty2 );
        *pVertices++ = InitFont3DVertex( D3DXVECTOR3(x+0,y+h,0), D3DXVECTOR3(0,0,-1), tx1, ty1 );
        *pVertices++ = InitFont3DVertex( D3DXVECTOR3(x+w,y+0,0), D3DXVECTOR3(0,0,-1), tx2, ty2 );
        *pVertices++ = InitFont3DVertex( D3DXVECTOR3(x+w,y+h,0), D3DXVECTOR3(0,0,-1), tx2, ty1 );
        *pVertices++ = InitFont3DVertex( D3DXVECTOR3(x+w,y+0,0), D3DXVECTOR3(0,0,-1), tx2, ty2 );
        *pVertices++ = InitFont3DVertex( D3DXVECTOR3(x+0,y+h,0), D3DXVECTOR3(0,0,-1), tx1, ty1 );
        dwNumTriangles += 2;

        if( dwNumTriangles*3 > (MAX_NUM_VERTICES-6) )
        {
            // Unlock, render, and relock the vertex buffer
            m_pVB->Unlock();
            m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, dwNumTriangles );
#ifdef DIRECT_VERSION_9_MJH
			if( FAILED( m_pVB->Lock( 0, 0, (void**)&pVertices, D3DLOCK_DISCARD ) ) )
#else
			if( FAILED( m_pVB->Lock( 0, 0, (BYTE**)&pVertices, D3DLOCK_DISCARD ) ) )
#endif // DIRECT_VERSION_9_MJH
			{
				return E_FAIL;		
			}
            
            dwNumTriangles = 0L;
        }

        x += w;
    }

    // Unlock and render the vertex buffer
    m_pVB->Unlock();
    if( dwNumTriangles > 0 )
        m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, dwNumTriangles );

    // Restore the modified renderstates
#ifdef DIRECT_VERSION_9_MJH
	m_pStateBlockSaved->Apply();
#else
	m_pd3dDevice->ApplyStateBlock( m_dwSavedStateBlock );
#endif // DIRECT_VERSION_9_MJH
    

    return S_OK;
}




