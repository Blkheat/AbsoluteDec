//.......................................................................................................
// 
// Set Render State	Header																	By wxywxy
//
//.......................................................................................................

#ifndef __CRenderState_H
#define __CRenderState_H

#include <RockPCH.h>

#ifdef DIRECT_VERSION_9_MJH
#include <d3d9.h>
#else
#include <d3d8.h>
#endif // DIRECT_VERSION_9_MJH

class CRenderState
{
public:
	
	 CRenderState();
	~CRenderState();

public:
		DWORD	GetZEnable()	{ return m_ZEnable; }
		DWORD	GetFogEnable()	{ return m_FogEnable; }
		DWORD	GetLighting()	{ return m_Lighting; }
		DWORD	GetAlphaBlendEnable() { return m_AlphaBlendEnable; }
		DWORD	GetSrcBlend()	{ return m_SrcBlend; }
		DWORD	GetDestBlend()	{ return m_DestBlend; }
		DWORD   GetAlphaTestEnable() { return m_AlphaTestEnable; }
		DWORD	GetZFunc()	{ return m_ZFunc; }
		DWORD   GetZWriteEnable() { return m_ZWriteEnable; }
		DWORD   GetZBias() { return m_ZBias; }
		DWORD   GetCullMode() { return m_CullMode; }
		DWORD	GetAlphaRef() { return m_AlphaRef; }
		DWORD	GetAlphaFunc()	{ return m_AlphaFunc; }
		DWORD   GetFillMode() { return m_FillMode; }
	
		HRESULT SetLighting(DWORD Value);
		HRESULT SetAmbient(DWORD Value);
		HRESULT SetStencilPass(DWORD Value);
		HRESULT SetStencilFunc(DWORD Value);
		HRESULT SetStencilRef(DWORD Value);
		HRESULT SetStencilEnable(DWORD Value);
		HRESULT SetEdgeAntialias(DWORD Value);
		HRESULT SetZBias(DWORD Value);
		HRESULT SetFogEnd(float Value);
		HRESULT SetFogStart(float Value);
		HRESULT SetFogTableMode(DWORD Value);
		HRESULT SetFogColor(DWORD Value);
		HRESULT SetFogEnable(DWORD Value);
		HRESULT SetZFunc(DWORD Value);
		HRESULT SetCullMode(DWORD Value);
		HRESULT SetDestBlend(DWORD Value);
		HRESULT SetSrcBlend(DWORD Value);
		HRESULT SetAlphaBlendEnable(DWORD Value);
		HRESULT SetAlphaFunc(DWORD Value);
		HRESULT SetAlphaTestEnable(DWORD Value);
		HRESULT SetZWriteEnable(DWORD Value);
		HRESULT SetShadeMode(DWORD Value);
		HRESULT SetFillMode(DWORD Value);
		HRESULT SetZEnable(DWORD Value);
		HRESULT SetAlphaRef(DWORD Value);

		HRESULT SetFogDensity(float Value);
		HRESULT SetFogRangeEnable(DWORD Value);
		HRESULT SetFogVertexMode(DWORD Value);
		HRESULT SetTFactorColor(DWORD Value);
		
		HRESULT SetClipping(DWORD Value);
		HRESULT SetClipPlaneEnable(DWORD Value);
		HRESULT SetVertexBlend(DWORD Value);
		HRESULT SetIndexedVertexBlendEnable(DWORD Value);
		HRESULT SetStencilZFail(DWORD Value);
		HRESULT SetStencilFail(DWORD Value);
		HRESULT SetStencilMask(DWORD Value);
		HRESULT SetStencilWriteMask(DWORD Value);
		HRESULT SetPointSize(float Value);

		void	InitRenderState(void);

		HRESULT GetRenderState( D3DRENDERSTATETYPE State, DWORD* pValue );
		HRESULT SetRenderState( D3DRENDERSTATETYPE State, DWORD Value );

protected:

	DWORD	m_ZEnable;
	DWORD	m_FillMode;
	DWORD   m_ShadeMode;
	DWORD	m_ZWriteEnable;
	DWORD   m_AlphaTestEnable;
	DWORD	m_AlphaBlendEnable;
	DWORD   m_SrcBlend;
	DWORD	m_DestBlend;
	DWORD	m_CullMode;
	DWORD	m_ZFunc;
	DWORD	m_AlphaFunc;
	DWORD	m_FogEnable;
	DWORD	m_FogColor;
	DWORD	m_FogTableMode;
	float   m_FogStart;
	float	m_FogEnd;
	DWORD	m_ZBias;
	DWORD	m_EdgeAntialias;
	DWORD	m_StencilEnable;
	DWORD	m_StencilRef;
	DWORD	m_StencilFunc;
	DWORD   m_StencilPass;

	DWORD	m_Lighting;
	DWORD   m_Ambient;

	DWORD	m_AlphaRef;

	DWORD	m_FogVertexMode;
	DWORD	m_FogRangeEnable;
	float	m_FogDensity;
	DWORD	m_TFactorColor;

	DWORD	m_Clipping;
	DWORD	m_ClipPlaneEnable;
	DWORD	m_VertexBlend;
	DWORD	m_IndexedVertexBlendEnable;

	DWORD	m_StencilZFail;
	DWORD	m_StencilFail;
	DWORD	m_StencilMask;
	DWORD	m_StencilWriteMask; 

	float	m_PointSize;
};

#endif
//.......................................................................................................
