//-----------------------------------------------------------------------------
// File: D3DFont.h
//
// Desc: Texture-based font class
//
// Copyright (c) 1999-2000 Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------
#ifndef D3DFONT_H
#define D3DFONT_H
#include <tchar.h>
#ifdef DIRECT_VERSION_9_MJH
#include <D3D9.h>
#else
#include <D3D8.h>
#endif // DIRECT_VERSION_9_MJH


// Font creation flags
#define D3DFONT_BOLD        0x0001
#define D3DFONT_ITALIC      0x0002

// Font rendering flags
#define D3DFONT_CENTERED    0x0001
#define D3DFONT_TWOSIDED    0x0002
#define D3DFONT_FILTERED    0x0004




//-----------------------------------------------------------------------------
// Name: class CD3DFont
// Desc: Texture-based font class for doing text in a 3D scene.
//-----------------------------------------------------------------------------
class CD3DFont
{
    TCHAR   m_strFontName[80];            // Font properties
    DWORD   m_dwFontHeight;
    DWORD   m_dwFontFlags;

#ifdef DIRECT_VERSION_9_MJH
    LPDIRECT3DDEVICE9       m_pd3dDevice; // A D3DDevice used for rendering
    LPDIRECT3DTEXTURE9      m_pTexture;   // The d3d texture for this font
    LPDIRECT3DVERTEXBUFFER9 m_pVB;        // VertexBuffer for rendering text
#else
	LPDIRECT3DDEVICE8       m_pd3dDevice; // A D3DDevice used for rendering
    LPDIRECT3DTEXTURE8      m_pTexture;   // The d3d texture for this font
    LPDIRECT3DVERTEXBUFFER8 m_pVB;        // VertexBuffer for rendering text
#endif // DIRECT_VERSION_9_MJH
    DWORD   m_dwTexWidth;                 // Texture dimensions
    DWORD   m_dwTexHeight;
    FLOAT   m_fTextScale;
    FLOAT   m_fTexCoords[128-32][4];

    // Stateblocks for setting and restoring render states
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DSTATEBLOCK9 m_pStateBlockSaved;
    LPDIRECT3DSTATEBLOCK9 m_pStateBlockDrawText;
#else
	DWORD   m_dwSavedStateBlock;
    DWORD   m_dwDrawTextStateBlock;
#endif // DIRECT_VERSION_9_MJH

public:
    // 2D and 3D text drawing functions
    HRESULT DrawText( FLOAT x, FLOAT y, DWORD dwColor, 
                      TCHAR* strText, DWORD dwFlags=0L );
    HRESULT DrawTextScaled( FLOAT x, FLOAT y, FLOAT z, 
                            FLOAT fXScale, FLOAT fYScale, DWORD dwColor, 
                            TCHAR* strText, DWORD dwFlags=0L );
    HRESULT Render3DText( TCHAR* strText, DWORD dwFlags=0L );
    
    // Function to get extent of text
    HRESULT GetTextExtent( TCHAR* strText, SIZE* pSize );

    // Initializing and destroying device-dependent objects
#ifdef DIRECT_VERSION_9_MJH
	HRESULT InitDeviceObjects( LPDIRECT3DDEVICE9 pd3dDevice );
#else
	HRESULT InitDeviceObjects( LPDIRECT3DDEVICE8 pd3dDevice );
#endif // DIRECT_VERSION_9_MJH
    HRESULT RestoreDeviceObjects();
    HRESULT InvalidateDeviceObjects();
    HRESULT DeleteDeviceObjects();

    // Constructor / destructor
    CD3DFont( TCHAR* strFontName, DWORD dwHeight, DWORD dwFlags=0L );
    ~CD3DFont();
};




#endif


