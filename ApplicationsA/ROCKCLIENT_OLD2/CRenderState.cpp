#include "CRenderState.h"
#include "CRenderManager.h"

#ifdef DIRECT_VERSION_9_MJH
extern IDirect3DDevice9*	g_lpDevice;
#else
extern IDirect3DDevice8*	g_lpDevice;
#endif // DIRECT_VERSION_9_MJH

CRenderState::CRenderState()
{
	//.......................................................................................................
	// SetRender State 관련
	//.......................................................................................................
	m_ZEnable           = D3DZB_TRUE;
	m_FillMode			= D3DFILL_SOLID;
	m_ShadeMode			= D3DSHADE_GOURAUD;
	m_ZWriteEnable		= TRUE;
	
	m_AlphaTestEnable	= FALSE;
	m_AlphaFunc			= 0;

	m_AlphaBlendEnable  = FALSE;
	m_SrcBlend			= D3DBLEND_ONE;
	m_DestBlend			= D3DBLEND_ONE;
	
	m_CullMode			= D3DCULL_CCW;
	m_ZFunc				= D3DCMP_LESSEQUAL;
	
	m_FogEnable			= FALSE;
	m_FogColor			= 0;
	m_FogTableMode		= D3DFOG_NONE;
	
	m_FogStart			= 0.0f;
	m_FogEnd			= 0.0f;
	
	m_ZBias				= 0;
	m_EdgeAntialias		= FALSE;
	m_StencilEnable		= FALSE;
	m_StencilRef		= 0;
	m_StencilFunc		= D3DCMP_ALWAYS;
	m_StencilPass		= D3DSTENCILOP_KEEP;

	m_Lighting			= FALSE;
	m_Ambient			= 0;
	m_AlphaRef			= 0;
	
	m_FogVertexMode		= D3DFOG_NONE;
	m_FogRangeEnable	= FALSE;
	m_FogDensity		= 1.0f;
	m_TFactorColor		= 0x00000000;

	m_Clipping			= TRUE;
	m_ClipPlaneEnable	= FALSE;
	m_VertexBlend		= D3DVBF_DISABLE;
	m_IndexedVertexBlendEnable = FALSE;
	m_StencilZFail      = D3DSTENCILOP_KEEP;
	m_StencilFail       = D3DSTENCILOP_KEEP;
	m_StencilMask       = 0xFFFFFFFF;
	m_StencilWriteMask  = 0xFFFFFFFF; 
	m_PointSize		    = 1.0f;
}

CRenderState::~CRenderState()
{
}

HRESULT CRenderState::SetAlphaRef(DWORD Value)
{
	if( m_AlphaRef == Value )
		return S_OK;

	m_AlphaRef = Value;
	
	HRESULT hResult = S_FALSE;

	EnterCriticalSection( &g_cDeviceFlag );
	hResult = g_lpDevice->SetRenderState( D3DRS_ALPHAREF , Value );
	LeaveCriticalSection( &g_cDeviceFlag );

	return hResult;
}

HRESULT CRenderState::SetZEnable(DWORD Value)
{
	if( m_ZEnable == Value )
		return S_OK;

	m_ZEnable = Value;

	HRESULT hResult = S_FALSE;

	EnterCriticalSection( &g_cDeviceFlag );
	hResult = g_lpDevice->SetRenderState( D3DRS_ZENABLE , Value );
	LeaveCriticalSection( &g_cDeviceFlag );

	return hResult;
}

HRESULT CRenderState::SetFillMode(DWORD Value)
{
	if( m_FillMode == Value )
		return S_OK;

	m_FillMode = Value;

	HRESULT hResult = S_FALSE;

	EnterCriticalSection( &g_cDeviceFlag );
	hResult = g_lpDevice->SetRenderState( D3DRS_FILLMODE , Value );
	LeaveCriticalSection( &g_cDeviceFlag );

	return hResult;
}

HRESULT CRenderState::SetShadeMode(DWORD Value)
{
	if( m_ShadeMode == Value )
		return S_OK;

	m_ShadeMode = Value;

	HRESULT hResult = S_FALSE;

	EnterCriticalSection( &g_cDeviceFlag );
	hResult = g_lpDevice->SetRenderState( D3DRS_SHADEMODE , Value );
	LeaveCriticalSection( &g_cDeviceFlag );

	return hResult;
}

HRESULT CRenderState::SetZWriteEnable(DWORD Value)
{
	if( m_ZWriteEnable == Value )
		return S_OK;

	m_ZWriteEnable = Value;

	HRESULT hResult = S_FALSE;

	EnterCriticalSection( &g_cDeviceFlag );
	hResult = g_lpDevice->SetRenderState( D3DRS_ZWRITEENABLE , Value );
	LeaveCriticalSection( &g_cDeviceFlag );

	return hResult;
}

HRESULT CRenderState::SetAlphaTestEnable(DWORD Value)
{
	if( m_AlphaTestEnable == Value )
		return S_OK;

	m_AlphaTestEnable = Value;

	HRESULT hResult = S_FALSE;

	EnterCriticalSection( &g_cDeviceFlag );
	hResult = g_lpDevice->SetRenderState( D3DRS_ALPHATESTENABLE , Value );
	LeaveCriticalSection( &g_cDeviceFlag );

	return hResult;
}

HRESULT CRenderState::SetAlphaFunc(DWORD Value)
{
	if( m_AlphaFunc == Value )
		return S_OK;

	m_AlphaFunc = Value;

	HRESULT hResult = S_FALSE;

	EnterCriticalSection( &g_cDeviceFlag );
	hResult = g_lpDevice->SetRenderState( D3DRS_ALPHAFUNC , Value );
	LeaveCriticalSection( &g_cDeviceFlag );

	return hResult;
}

HRESULT CRenderState::SetAlphaBlendEnable(DWORD Value)
{
	if( m_AlphaBlendEnable == Value )
		return S_OK;

	m_AlphaBlendEnable = Value;

	HRESULT hResult = S_FALSE;

	EnterCriticalSection( &g_cDeviceFlag );
	hResult = g_lpDevice->SetRenderState( D3DRS_ALPHABLENDENABLE , Value );
	LeaveCriticalSection( &g_cDeviceFlag );

	return hResult;
}

HRESULT CRenderState::SetSrcBlend(DWORD Value)
{
	if( m_SrcBlend == Value )
		return S_OK;

	m_SrcBlend = Value;

	HRESULT hResult = S_FALSE;

	EnterCriticalSection( &g_cDeviceFlag );
	hResult = g_lpDevice->SetRenderState( D3DRS_SRCBLEND , Value );
	LeaveCriticalSection( &g_cDeviceFlag );
	
	return hResult;
}

HRESULT CRenderState::SetDestBlend(DWORD Value)
{
	///--JAPAN_BUG_MODIFY
	///-- Intel (R) G965 Express Chipset Family 같은 내장 그래픽카드에서는
	///-- 두번 알파블렌딩이 먹는곳에서 
	///-- DEST Blend Option을 D3DBLEND_DESTALPHA 로 하면 이상현상이 일어난다.
	if( Value == D3DBLEND_DESTALPHA )
	{
		Value = D3DBLEND_ONE;
	}
	
	if( m_DestBlend == Value )
		return S_OK;

	m_DestBlend = Value;

	HRESULT hResult = S_FALSE;

	EnterCriticalSection( &g_cDeviceFlag );
	hResult = g_lpDevice->SetRenderState( D3DRS_DESTBLEND , Value );
	LeaveCriticalSection( &g_cDeviceFlag );

	return hResult;
}

HRESULT CRenderState::SetCullMode(DWORD Value)
{
	if( m_CullMode == Value )
		return S_OK;

	m_CullMode = Value;

	HRESULT hResult = S_FALSE;

	EnterCriticalSection( &g_cDeviceFlag );
	hResult = g_lpDevice->SetRenderState( D3DRS_CULLMODE , Value );
	LeaveCriticalSection( &g_cDeviceFlag );

	return hResult;
}

HRESULT CRenderState::SetZFunc(DWORD Value)
{
	if( m_ZFunc == Value )
		return S_OK;

	m_ZFunc = Value;

	HRESULT hResult = S_FALSE;

	EnterCriticalSection( &g_cDeviceFlag );
	hResult = g_lpDevice->SetRenderState( D3DRS_ZFUNC , Value );
	LeaveCriticalSection( &g_cDeviceFlag );

	return hResult;
}
	
HRESULT CRenderState::SetFogEnable(DWORD Value)
{
	if( m_FogEnable == Value )
		return S_OK;

	m_FogEnable = Value;

	HRESULT hResult = S_FALSE;

	EnterCriticalSection( &g_cDeviceFlag );
	hResult = g_lpDevice->SetRenderState( D3DRS_FOGENABLE , Value );
	LeaveCriticalSection( &g_cDeviceFlag );
	
	return hResult;
}

HRESULT CRenderState::SetFogColor(DWORD Value)
{
	if( m_FogColor == Value )
		return S_OK;

	m_FogColor = Value;

	HRESULT hResult = S_FALSE;

	EnterCriticalSection( &g_cDeviceFlag );
	hResult = g_lpDevice->SetRenderState( D3DRS_FOGCOLOR , Value );
	LeaveCriticalSection( &g_cDeviceFlag );

	return hResult;
}

HRESULT CRenderState::SetFogTableMode(DWORD Value)
{
	if( m_FogTableMode == Value )
		return S_OK;

	m_FogTableMode = Value;

	HRESULT hResult = S_FALSE;

	EnterCriticalSection( &g_cDeviceFlag );
	hResult = g_lpDevice->SetRenderState( D3DRS_FOGTABLEMODE , Value );
	LeaveCriticalSection( &g_cDeviceFlag );

	return hResult;
}

HRESULT CRenderState::SetFogStart(float Value)
{
	if( m_FogStart == Value )
		return S_OK;

	m_FogStart = Value;

	HRESULT hResult = S_FALSE;

	EnterCriticalSection( &g_cDeviceFlag );
	hResult = g_lpDevice->SetRenderState( D3DRS_FOGSTART , *((DWORD*) &Value));
	LeaveCriticalSection( &g_cDeviceFlag );

	return hResult;
}

HRESULT CRenderState::SetFogEnd(float Value)
{
	if( m_FogEnd == Value )
		return S_OK;

	m_FogEnd = Value;

	HRESULT hResult = S_FALSE;

	EnterCriticalSection( &g_cDeviceFlag );
	hResult = g_lpDevice->SetRenderState( D3DRS_FOGEND , *((DWORD*) &Value));
	LeaveCriticalSection( &g_cDeviceFlag );

	return hResult;
}

HRESULT CRenderState::SetZBias(DWORD Value)
{
	if( m_ZBias == Value )
		return S_OK;

	m_ZBias = Value;

	HRESULT hResult = S_FALSE;

	EnterCriticalSection( &g_cDeviceFlag ); 
#ifdef DIRECT_VERSION_9_MJH
	hResult = g_lpDevice->SetRenderState( D3DRS_DEPTHBIAS , Value );
#else
	hResult = g_lpDevice->SetRenderState( D3DRS_ZBIAS , Value );
#endif // DIRECT_VERSION_9_MJH
	LeaveCriticalSection( &g_cDeviceFlag );

	return hResult;
}

HRESULT CRenderState::SetEdgeAntialias(DWORD Value)
{
	if( m_EdgeAntialias == Value )
		return S_OK;

	m_EdgeAntialias = Value;

	HRESULT hResult = S_FALSE;

	EnterCriticalSection( &g_cDeviceFlag );
#ifdef DIRECT_VERSION_9_MJH
	hResult = g_lpDevice->SetRenderState( D3DRS_ANTIALIASEDLINEENABLE , Value );
#else
	hResult = g_lpDevice->SetRenderState( D3DRS_EDGEANTIALIAS , Value );
#endif // DIRECT_VERSION_9_MJH
	LeaveCriticalSection( &g_cDeviceFlag );

	return hResult;
}

HRESULT CRenderState::SetStencilEnable(DWORD Value)
{
	if( m_StencilEnable == Value )
		return S_OK;

	m_StencilEnable = Value;

	HRESULT hResult = S_FALSE;

	EnterCriticalSection( &g_cDeviceFlag );
	hResult = g_lpDevice->SetRenderState( D3DRS_STENCILENABLE , Value );
	LeaveCriticalSection( &g_cDeviceFlag );

	return hResult;
}

HRESULT CRenderState::SetStencilRef(DWORD Value)
{
	if( m_StencilRef == Value )
		return S_OK;

	m_StencilRef = Value;

	HRESULT hResult = S_FALSE;

	EnterCriticalSection( &g_cDeviceFlag );
	hResult = g_lpDevice->SetRenderState( D3DRS_STENCILREF , Value );
	LeaveCriticalSection( &g_cDeviceFlag );

	return hResult;
}

HRESULT CRenderState::SetStencilFunc(DWORD Value)
{
	if( m_StencilFunc == Value )
		return S_OK;

	m_StencilFunc = Value;

	HRESULT hResult = S_FALSE;

	EnterCriticalSection( &g_cDeviceFlag );
	hResult = g_lpDevice->SetRenderState( D3DRS_STENCILFUNC , Value );
	LeaveCriticalSection( &g_cDeviceFlag );

	return hResult;
}

HRESULT CRenderState::SetStencilPass(DWORD Value)
{
	if( m_StencilPass == Value )
		return S_OK;

	m_StencilPass = Value;

	HRESULT hResult = S_FALSE;

	EnterCriticalSection( &g_cDeviceFlag );
	hResult = g_lpDevice->SetRenderState( D3DRS_STENCILPASS , Value );
	LeaveCriticalSection( &g_cDeviceFlag );

	return hResult;
}

HRESULT CRenderState::SetLighting(DWORD Value)
{
	if( m_Lighting == Value )
		return S_OK;

	m_Lighting = Value;

	HRESULT hResult = S_FALSE;

	EnterCriticalSection( &g_cDeviceFlag );
	hResult = g_lpDevice->SetRenderState( D3DRS_LIGHTING , Value );
	LeaveCriticalSection( &g_cDeviceFlag );

	return hResult;
}

HRESULT CRenderState::SetAmbient(DWORD Value)
{
	if( m_Ambient == Value )
		return S_OK;

	m_Ambient = Value;

	HRESULT hResult = S_FALSE;

	EnterCriticalSection( &g_cDeviceFlag );
	hResult = g_lpDevice->SetRenderState( D3DRS_AMBIENT , Value );
	LeaveCriticalSection( &g_cDeviceFlag );

	return hResult;
}

HRESULT CRenderState::SetFogVertexMode(DWORD Value)
{
	if( m_FogVertexMode == Value )
		return S_OK;

	m_FogVertexMode = Value;

	HRESULT hResult = S_FALSE;

	EnterCriticalSection( &g_cDeviceFlag );
	hResult = g_lpDevice->SetRenderState( D3DRS_FOGVERTEXMODE,  Value );
	LeaveCriticalSection( &g_cDeviceFlag );
	
	return hResult;	
}

HRESULT CRenderState::SetFogRangeEnable(DWORD Value)
{
	if( m_FogRangeEnable == Value )
		return S_OK;

	m_FogRangeEnable = Value;

	HRESULT hResult = S_FALSE;

	EnterCriticalSection( &g_cDeviceFlag );
	hResult = g_lpDevice->SetRenderState( D3DRS_RANGEFOGENABLE,  Value );
	LeaveCriticalSection( &g_cDeviceFlag );
	
	return hResult;	
}

HRESULT CRenderState::SetFogDensity(float Value)
{
	if( m_FogDensity == Value )
		return S_OK;

	m_FogDensity = Value;

	HRESULT hResult = S_FALSE;

	EnterCriticalSection( &g_cDeviceFlag );
	hResult = g_lpDevice->SetRenderState( D3DRS_FOGDENSITY,  *((DWORD*) &Value) );
	LeaveCriticalSection( &g_cDeviceFlag );
	
	return hResult;	
}

HRESULT CRenderState::SetTFactorColor(DWORD Value)
{
	if( m_TFactorColor == Value )
		return S_OK;

	m_TFactorColor = Value;

	HRESULT hResult = S_FALSE;

	EnterCriticalSection( &g_cDeviceFlag );
	hResult = g_lpDevice->SetRenderState( D3DRS_TEXTUREFACTOR, m_TFactorColor );
	LeaveCriticalSection( &g_cDeviceFlag );

	return hResult;	
}

HRESULT CRenderState::SetClipping(DWORD Value)
{
	if( m_Clipping == Value )
		return S_OK;

	m_Clipping = Value;

	HRESULT hResult = S_FALSE;

	EnterCriticalSection( &g_cDeviceFlag );
	hResult = g_lpDevice->SetRenderState( D3DRS_CLIPPING, Value );
	LeaveCriticalSection( &g_cDeviceFlag );

	return hResult;	
}

HRESULT CRenderState::SetClipPlaneEnable(DWORD Value)
{
	if( m_ClipPlaneEnable == Value )
		return S_OK;

	m_ClipPlaneEnable = Value;

	HRESULT hResult = S_FALSE;

	EnterCriticalSection( &g_cDeviceFlag );
	hResult = g_lpDevice->SetRenderState( D3DRS_CLIPPLANEENABLE, Value );
	LeaveCriticalSection( &g_cDeviceFlag );

	return hResult;
}

HRESULT CRenderState::SetVertexBlend(DWORD Value)
{
	if( m_VertexBlend == Value )
		return S_OK;

	m_VertexBlend = Value;

	HRESULT hResult = S_FALSE;

	EnterCriticalSection( &g_cDeviceFlag );
	hResult = g_lpDevice->SetRenderState( D3DRS_VERTEXBLEND, Value );
	LeaveCriticalSection( &g_cDeviceFlag );

	return hResult;
}

HRESULT CRenderState::SetIndexedVertexBlendEnable(DWORD Value)
{
	if( m_IndexedVertexBlendEnable == Value )
		return S_OK;

	m_IndexedVertexBlendEnable = Value;

	HRESULT hResult = S_FALSE;

	EnterCriticalSection( &g_cDeviceFlag );
	hResult = g_lpDevice->SetRenderState( D3DRS_INDEXEDVERTEXBLENDENABLE, Value );
	LeaveCriticalSection( &g_cDeviceFlag );

	return hResult;
}
 
HRESULT CRenderState::SetStencilZFail(DWORD Value)
{
	if( m_StencilZFail == Value )
		return S_OK;

	m_StencilZFail = Value;

	HRESULT hResult = S_FALSE;

	EnterCriticalSection( &g_cDeviceFlag );
	hResult = g_lpDevice->SetRenderState( D3DRS_STENCILZFAIL, Value );
	LeaveCriticalSection( &g_cDeviceFlag );

	return hResult;
}

HRESULT CRenderState::SetStencilFail(DWORD Value)
{
	if( m_StencilFail == Value )
		return S_OK;

	m_StencilFail = Value;

	HRESULT hResult = S_FALSE;

	EnterCriticalSection( &g_cDeviceFlag );
	hResult = g_lpDevice->SetRenderState( D3DRS_STENCILFAIL, Value );
	LeaveCriticalSection( &g_cDeviceFlag );

	return hResult;
}

HRESULT CRenderState::SetStencilMask(DWORD Value)
{
	if( m_StencilMask == Value )
		return S_OK;

	m_StencilMask = Value;

	HRESULT hResult = S_FALSE;

	EnterCriticalSection( &g_cDeviceFlag );
	hResult = g_lpDevice->SetRenderState( D3DRS_STENCILMASK, Value );
	LeaveCriticalSection( &g_cDeviceFlag );

	return hResult;
}

HRESULT CRenderState::SetStencilWriteMask(DWORD Value)
{
	if( m_StencilWriteMask == Value )
		return S_OK;

	m_StencilWriteMask = Value;

	HRESULT hResult = S_FALSE;

	EnterCriticalSection( &g_cDeviceFlag );
	hResult = g_lpDevice->SetRenderState( D3DRS_STENCILWRITEMASK, Value );
	LeaveCriticalSection( &g_cDeviceFlag );

	return hResult;
}

HRESULT CRenderState::SetPointSize(float Value)
{
	if( m_PointSize == Value )
		return S_OK;

	m_PointSize = Value;

	HRESULT hResult = S_FALSE;

	EnterCriticalSection( &g_cDeviceFlag );
	hResult = g_lpDevice->SetRenderState(D3DRS_POINTSIZE, *((DWORD*) &Value));
	LeaveCriticalSection( &g_cDeviceFlag );

	return hResult;
}


void CRenderState::InitRenderState(void)
{
	EnterCriticalSection( &g_cDeviceFlag );
	g_lpDevice->SetRenderState( D3DRS_ZENABLE          , m_ZEnable				  );
	g_lpDevice->SetRenderState( D3DRS_FILLMODE         , m_FillMode				  );
	g_lpDevice->SetRenderState( D3DRS_SHADEMODE        , m_ShadeMode			  );
	g_lpDevice->SetRenderState( D3DRS_ZWRITEENABLE     , m_ZWriteEnable		      );
	g_lpDevice->SetRenderState( D3DRS_ALPHATESTENABLE  , m_AlphaTestEnable		  );
	g_lpDevice->SetRenderState( D3DRS_ALPHAFUNC		   , m_AlphaFunc			  );
	g_lpDevice->SetRenderState( D3DRS_ALPHABLENDENABLE , m_AlphaBlendEnable		  );
	g_lpDevice->SetRenderState( D3DRS_SRCBLEND		   , m_SrcBlend				  );
	g_lpDevice->SetRenderState( D3DRS_DESTBLEND		   , m_DestBlend			  );
	g_lpDevice->SetRenderState( D3DRS_CULLMODE		   , m_CullMode				  );
	g_lpDevice->SetRenderState( D3DRS_ZFUNC			   , m_ZFunc				  );
	g_lpDevice->SetRenderState( D3DRS_FOGENABLE		   , m_FogEnable			  );
	g_lpDevice->SetRenderState( D3DRS_FOGCOLOR		   , m_FogColor				  );
	g_lpDevice->SetRenderState( D3DRS_FOGTABLEMODE     , m_FogTableMode			  );
	g_lpDevice->SetRenderState( D3DRS_FOGSTART		   , *((DWORD*) &m_FogStart)  );
	g_lpDevice->SetRenderState( D3DRS_FOGEND		   , *((DWORD*) &m_FogEnd)    );
#ifdef DIRECT_VERSION_9_MJH
	g_lpDevice->SetRenderState( D3DRS_DEPTHBIAS			   , m_ZBias		      );
	g_lpDevice->SetRenderState( D3DRS_ANTIALIASEDLINEENABLE    , m_EdgeAntialias  );
#else
	g_lpDevice->SetRenderState( D3DRS_ZBIAS			   , m_ZBias				  );
	g_lpDevice->SetRenderState( D3DRS_EDGEANTIALIAS    , m_EdgeAntialias		  );
#endif // DIRECT_VERSION_9_MJH
	g_lpDevice->SetRenderState( D3DRS_STENCILENABLE    , m_StencilEnable		  );
	g_lpDevice->SetRenderState( D3DRS_STENCILREF       , m_StencilRef			  );
	g_lpDevice->SetRenderState( D3DRS_STENCILFUNC      , m_StencilFunc			  );
	g_lpDevice->SetRenderState( D3DRS_STENCILPASS      , m_StencilPass			  );
	g_lpDevice->SetRenderState( D3DRS_LIGHTING		   , m_Lighting				  );
	g_lpDevice->SetRenderState( D3DRS_AMBIENT		   , m_Ambient				  );	           
	g_lpDevice->SetRenderState( D3DRS_ALPHAREF		   , m_AlphaRef				  );
	g_lpDevice->SetRenderState( D3DRS_FOGVERTEXMODE	   ,  m_FogVertexMode         );	
	g_lpDevice->SetRenderState( D3DRS_RANGEFOGENABLE   ,  m_FogRangeEnable        );	
	g_lpDevice->SetRenderState( D3DRS_FOGDENSITY       ,  *((DWORD*) &m_FogDensity ));	
	g_lpDevice->SetRenderState( D3DRS_TEXTUREFACTOR    , m_TFactorColor			  );

	g_lpDevice->SetRenderState( D3DRS_CLIPPING,         m_Clipping );
	g_lpDevice->SetRenderState( D3DRS_CLIPPLANEENABLE,  m_ClipPlaneEnable );
	g_lpDevice->SetRenderState( D3DRS_VERTEXBLEND,      m_VertexBlend );
	g_lpDevice->SetRenderState( D3DRS_INDEXEDVERTEXBLENDENABLE, m_IndexedVertexBlendEnable );
	

	g_lpDevice->SetRenderState( D3DRS_STENCILZFAIL     , m_StencilZFail );
	g_lpDevice->SetRenderState( D3DRS_STENCILFAIL      , m_StencilFail );
	g_lpDevice->SetRenderState( D3DRS_STENCILMASK      , m_StencilMask );
	g_lpDevice->SetRenderState( D3DRS_STENCILWRITEMASK , m_StencilWriteMask  );
	
	g_lpDevice->SetRenderState(D3DRS_POINTSIZE, *((DWORD*) &m_PointSize ));
	LeaveCriticalSection( &g_cDeviceFlag );
}

HRESULT CRenderState::GetRenderState( D3DRENDERSTATETYPE State, DWORD* pValue )
{
	HRESULT hResult = S_FALSE;

	EnterCriticalSection( &g_cDeviceFlag );
	hResult = g_lpDevice->GetRenderState( State, pValue );
	LeaveCriticalSection( &g_cDeviceFlag );
	
	return hResult;		
}

HRESULT CRenderState::SetRenderState( D3DRENDERSTATETYPE State, DWORD Value )
{
	HRESULT hResult = S_FALSE;

	EnterCriticalSection( &g_cDeviceFlag );
	hResult = g_lpDevice->SetRenderState( State, Value );
	LeaveCriticalSection( &g_cDeviceFlag );
	
	return hResult;		
}
