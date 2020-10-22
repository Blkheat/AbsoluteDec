
#include "RockClient.h"
#include "CTextureState.h"
#include "CRenderManager.h"

#ifdef DIRECT_VERSION_9_MJH
extern IDirect3DDevice9*	g_lpDevice;
#else
extern IDirect3DDevice8*	g_lpDevice;
#endif // DIRECT_VERSION_9_MJH


#define CHECK_TEXTURE_STAGE_MAXCOUNT() \
	if( (Stage) >= TEXTURE_STATE_STAGE_MAXCOUNT ) \
		return S_FALSE;

#define PROC_TEXTURE_STATE_CHANGE(x) \
	if( (x) == (Value) ) \
		return S_OK; \
	\
	(x) = (Value);

#define CHECK_SAME_VALUE(n,v) \
	if( (n) == (v) ) \
		return S_OK;	

CTextureState::CTextureState()
{
	for( int i = 0 ; i < TEXTURE_STATE_STAGE_MAXCOUNT ; ++i )
	{
		m_StateList[ i ].ColorOP       = D3DTOP_DISABLE;
		m_StateList[ i ].ColorArg1     = D3DTA_TEXTURE;
		m_StateList[ i ].ColorArg2     = D3DTA_CURRENT;

		m_StateList[ i ].AlphaOP	   = D3DTOP_DISABLE;
		m_StateList[ i ].AlphaArg1	   = D3DTA_TEXTURE;
		m_StateList[ i ].AlphaArg2	   = D3DTA_CURRENT;

		m_StateList[ i ].TexCoordIndex = D3DTSS_TCI_PASSTHRU;

		m_StateList[ i ].AddressU	   = D3DTADDRESS_WRAP;
		m_StateList[ i ].AddressV	   = D3DTADDRESS_WRAP;
		m_StateList[ i ].AddressW      = D3DTADDRESS_WRAP;

		m_StateList[ i ].BorderColor   = 0x00000000;

		m_StateList[ i ].MAGFilter     = D3DTEXF_POINT;
		m_StateList[ i ].MINFilter     = D3DTEXF_POINT;
		m_StateList[ i ].MIPFilter     = D3DTEXF_POINT;

		m_StateList[ i ].TextureTransFormFlags = D3DTTFF_DISABLE;
	}
}

CTextureState::~CTextureState()
{

}

#ifdef DIRECT_VERSION_9_MJH
HRESULT CTextureState::SetTexture(DWORD Stage,LPDIRECT3DTEXTURE9 pTexture)
#else
HRESULT CTextureState::SetTexture(DWORD Stage,LPDIRECT3DTEXTURE8 pTexture)
#endif // DIRECT_VERSION_9_MJH
{
	
	CHECK_SAME_VALUE( m_StateList[ Stage ].pTexture , pTexture );	
	
	HRESULT hResult = S_FALSE;
	EnterCriticalSection( &g_cDeviceFlag );
	m_StateList[ Stage ].pTexture = pTexture;
	hResult = g_lpDevice->SetTexture( Stage , pTexture );
	LeaveCriticalSection( &g_cDeviceFlag );

	return hResult;
}

HRESULT CTextureState::DisableTexture(DWORD Stage)
{
	CHECK_SAME_VALUE( m_StateList[ Stage ].pTexture , NULL );
	
	HRESULT hResult = S_FALSE;
	
	EnterCriticalSection( &g_cDeviceFlag );
	m_StateList[ Stage ].pTexture = NULL;
	hResult = g_lpDevice->SetTexture( Stage , NULL );
	LeaveCriticalSection( &g_cDeviceFlag );

	return hResult;
}

DWORD CTextureState::GetTextureColorOP(DWORD Stage)
{
	return m_StateList[ Stage ].ColorOP;	
}

DWORD CTextureState::GetTextureColorArg1(DWORD Stage)
{
	return m_StateList[ Stage ].ColorArg1;	
}

DWORD CTextureState::GetTextureColorArg2(DWORD Stage)
{
	return m_StateList[ Stage ].ColorArg2;	
}

HRESULT CTextureState::SetTextureColorOP(DWORD Stage,DWORD Value)
{
	//CHECK_TEXTURE_STAGE_MAXCOUNT();
	
	///-- 해당 텍스쳐 옵션을 지원하지 않는다면.. 이루틴이 맞는건가?
	if( !( g_RockClient.GetCaps()->TextureOpCaps & Value ) )
	{
		//Value = D3DTOP_MODULATE; ///-- 그냥 모듈레이트 - 일단보류
		int a = 0;
	}	
	
	PROC_TEXTURE_STATE_CHANGE( m_StateList[ Stage ].ColorOP );
	
	HRESULT hResult = S_FALSE;

	EnterCriticalSection( &g_cDeviceFlag );
	hResult = g_lpDevice->SetTextureStageState( Stage , D3DTSS_COLOROP   , Value   );
	LeaveCriticalSection( &g_cDeviceFlag );

	return hResult;
}

HRESULT CTextureState::SetTextureColorArg1(DWORD Stage,DWORD Value)
{
	//CHECK_TEXTURE_STAGE_MAXCOUNT();
	
	PROC_TEXTURE_STATE_CHANGE( m_StateList[ Stage ].ColorArg1 );

	HRESULT hResult = S_FALSE;

	EnterCriticalSection( &g_cDeviceFlag );
	hResult = g_lpDevice->SetTextureStageState( Stage , D3DTSS_COLORARG1 , Value );
	LeaveCriticalSection( &g_cDeviceFlag );

	return hResult;
}

HRESULT CTextureState::SetTextureColorArg2(DWORD Stage,DWORD Value)
{
	//CHECK_TEXTURE_STAGE_MAXCOUNT();
	
	PROC_TEXTURE_STATE_CHANGE( m_StateList[ Stage ].ColorArg2 );

	HRESULT hResult = S_FALSE;

	EnterCriticalSection( &g_cDeviceFlag );
	hResult = g_lpDevice->SetTextureStageState( Stage , D3DTSS_COLORARG2 , Value );
	LeaveCriticalSection( &g_cDeviceFlag );

	return hResult;
}

DWORD CTextureState::GetTextureAlphaOP(DWORD Stage)
{
	return m_StateList[ Stage ].AlphaOP;	
}

DWORD CTextureState::GetTextureAlphaArg1(DWORD Stage)
{
	return m_StateList[ Stage ].AlphaArg1;	
}

DWORD CTextureState::GetTextureAlphaArg2(DWORD Stage)
{
	return m_StateList[ Stage ].AlphaArg2;	
}

HRESULT CTextureState::SetTextureAlphaOP(DWORD Stage,DWORD Value)
{
	//CHECK_TEXTURE_STAGE_MAXCOUNT();
	
	PROC_TEXTURE_STATE_CHANGE( m_StateList[ Stage ].AlphaOP );

	HRESULT hResult = S_OK;

	EnterCriticalSection( &g_cDeviceFlag );
	hResult = g_lpDevice->SetTextureStageState( Stage , D3DTSS_ALPHAOP , Value );
	LeaveCriticalSection( &g_cDeviceFlag );

	return hResult;
}

HRESULT CTextureState::SetTextureAlphaArg1(DWORD Stage,DWORD Value)
{
	//CHECK_TEXTURE_STAGE_MAXCOUNT();
	
	PROC_TEXTURE_STATE_CHANGE( m_StateList[ Stage ].AlphaArg1 );

	HRESULT hResult = S_FALSE;

	EnterCriticalSection( &g_cDeviceFlag );
	hResult = g_lpDevice->SetTextureStageState( Stage , D3DTSS_ALPHAARG1 , Value );
	LeaveCriticalSection( &g_cDeviceFlag );

	return hResult;
}

HRESULT CTextureState::SetTextureAlphaArg2(DWORD Stage,DWORD Value)
{
	//CHECK_TEXTURE_STAGE_MAXCOUNT();
	
	PROC_TEXTURE_STATE_CHANGE( m_StateList[ Stage ].AlphaArg2 );

	HRESULT hResult = S_OK;

	EnterCriticalSection( &g_cDeviceFlag );
	hResult = g_lpDevice->SetTextureStageState( Stage , D3DTSS_ALPHAARG2 , Value );
	LeaveCriticalSection( &g_cDeviceFlag );

	return hResult;
}

HRESULT CTextureState::SetTextureCoordIndex(DWORD Stage,DWORD Value)
{
	//CHECK_TEXTURE_STAGE_MAXCOUNT();
	
	PROC_TEXTURE_STATE_CHANGE( m_StateList[ Stage ].TexCoordIndex );

	HRESULT hResult = S_FALSE;

	EnterCriticalSection( &g_cDeviceFlag );
	hResult = g_lpDevice->SetTextureStageState( Stage , D3DTSS_TEXCOORDINDEX , Value );
	LeaveCriticalSection( &g_cDeviceFlag );

	return hResult;
}

HRESULT CTextureState::SetTextureAddressU(DWORD Stage,DWORD Value)
{
	//CHECK_TEXTURE_STAGE_MAXCOUNT();
	
	PROC_TEXTURE_STATE_CHANGE( m_StateList[ Stage ].AddressU );

	HRESULT hResult = S_FALSE;

	EnterCriticalSection( &g_cDeviceFlag );
#ifdef DIRECT_VERSION_9_MJH
	hResult = g_lpDevice->SetSamplerState( Stage , D3DSAMP_ADDRESSU , Value );
#else
	hResult = g_lpDevice->SetTextureStageState( Stage , D3DTSS_ADDRESSU , Value );
#endif // DIRECT_VERSION_9_MJH
	LeaveCriticalSection( &g_cDeviceFlag );

	return hResult;
}

HRESULT CTextureState::SetTextureAddressV(DWORD Stage,DWORD Value)
{
	//CHECK_TEXTURE_STAGE_MAXCOUNT();
	
	PROC_TEXTURE_STATE_CHANGE( m_StateList[ Stage ].AddressV );

	HRESULT hResult = S_FALSE;

	EnterCriticalSection( &g_cDeviceFlag );
#ifdef DIRECT_VERSION_9_MJH
	hResult = g_lpDevice->SetSamplerState( Stage , D3DSAMP_ADDRESSV , Value );
#else
	hResult = g_lpDevice->SetTextureStageState( Stage , D3DTSS_ADDRESSV , Value );
#endif // DIRECT_VERSION_9_MJH
	LeaveCriticalSection( &g_cDeviceFlag );

	return hResult;
}

HRESULT CTextureState::SetTextureAddressW(DWORD Stage,DWORD Value)
{
	//CHECK_TEXTURE_STAGE_MAXCOUNT();
	
	PROC_TEXTURE_STATE_CHANGE( m_StateList[ Stage ].AddressW );

	HRESULT hResult = S_FALSE;

	EnterCriticalSection( &g_cDeviceFlag );
#ifdef DIRECT_VERSION_9_MJH
	hResult = g_lpDevice->SetSamplerState( Stage , D3DSAMP_ADDRESSW , Value );
#else
	hResult = g_lpDevice->SetTextureStageState( Stage , D3DTSS_ADDRESSW , Value );
#endif // DIRECT_VERSION_9_MJH
	LeaveCriticalSection( &g_cDeviceFlag );

	return hResult;
}

HRESULT CTextureState::SetTextureBorderColor(DWORD Stage,DWORD Value)
{
	//CHECK_TEXTURE_STAGE_MAXCOUNT();
	
	PROC_TEXTURE_STATE_CHANGE( m_StateList[ Stage ].BorderColor );

	HRESULT hResult = S_FALSE;

	EnterCriticalSection( &g_cDeviceFlag );
#ifdef DIRECT_VERSION_9_MJH
	hResult = g_lpDevice->SetSamplerState( Stage , D3DSAMP_BORDERCOLOR , Value );
#else
	hResult = g_lpDevice->SetTextureStageState( Stage , D3DTSS_BORDERCOLOR , Value );
#endif // DIRECT_VERSION_9_MJH
	LeaveCriticalSection( &g_cDeviceFlag );

	return hResult;
}

DWORD CTextureState::GetTextureMagFilter(DWORD Stage)
{
	return m_StateList[ Stage ].MAGFilter;
}

DWORD CTextureState::GetTextureMinFilter(DWORD Stage)
{
	return m_StateList[ Stage ].MINFilter;
}

DWORD CTextureState::GetTextureMipFilter(DWORD Stage)
{
	return m_StateList[ Stage ].MIPFilter;
}

HRESULT CTextureState::SetTextureMagFilter(DWORD Stage,DWORD Value)
{
	//CHECK_TEXTURE_STAGE_MAXCOUNT();
	
	PROC_TEXTURE_STATE_CHANGE( m_StateList[ Stage ].MAGFilter );

	HRESULT hResult = S_FALSE;

	EnterCriticalSection( &g_cDeviceFlag );
#ifdef DIRECT_VERSION_9_MJH
	hResult = g_lpDevice->SetSamplerState( Stage , D3DSAMP_MAGFILTER , Value );
#else
	hResult = g_lpDevice->SetTextureStageState( Stage , D3DTSS_MAGFILTER , Value );
#endif // DIRECT_VERSION_9_MJH
	LeaveCriticalSection( &g_cDeviceFlag );

	return hResult;
}

HRESULT CTextureState::SetTextureMinFilter(DWORD Stage,DWORD Value)
{
	//CHECK_TEXTURE_STAGE_MAXCOUNT();
	
	PROC_TEXTURE_STATE_CHANGE( m_StateList[ Stage ].MINFilter );

	HRESULT hResult = S_FALSE;

	EnterCriticalSection( &g_cDeviceFlag );
#ifdef DIRECT_VERSION_9_MJH
	hResult = g_lpDevice->SetSamplerState( Stage , D3DSAMP_MINFILTER , Value );
#else
	hResult = g_lpDevice->SetTextureStageState( Stage , D3DTSS_MINFILTER , Value );
#endif // DIRECT_VERSION_9_MJH
	LeaveCriticalSection( &g_cDeviceFlag );

	return hResult;
}

HRESULT CTextureState::SetTextureMipFilter(DWORD Stage,DWORD Value)
{
	//CHECK_TEXTURE_STAGE_MAXCOUNT();
	
	PROC_TEXTURE_STATE_CHANGE( m_StateList[ Stage ].MIPFilter );

	HRESULT hResult = S_FALSE;

	EnterCriticalSection( &g_cDeviceFlag );
#ifdef DIRECT_VERSION_9_MJH
	hResult = g_lpDevice->SetSamplerState( Stage , D3DSAMP_MIPFILTER , Value );
#else
	hResult = g_lpDevice->SetTextureStageState( Stage , D3DTSS_MIPFILTER , Value );
#endif // DIRECT_VERSION_9_MJH
	LeaveCriticalSection( &g_cDeviceFlag );

	return hResult;
}

HRESULT CTextureState::SetTextureTransFormFlags(DWORD Stage,DWORD Value)
{
	//CHECK_TEXTURE_STAGE_MAXCOUNT();
	
	PROC_TEXTURE_STATE_CHANGE( m_StateList[ Stage ].TextureTransFormFlags );

	HRESULT hResult = S_FALSE;

	EnterCriticalSection( &g_cDeviceFlag );
	hResult = g_lpDevice->SetTextureStageState( Stage , D3DTSS_TEXTURETRANSFORMFLAGS , Value );
	LeaveCriticalSection( &g_cDeviceFlag );

	return hResult;
}

HRESULT CTextureState::GetTextureStageState( DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD* pValue )
{
	HRESULT hResult = S_FALSE;
	
	EnterCriticalSection( &g_cDeviceFlag );
	hResult = g_lpDevice->GetTextureStageState( Stage , Type , pValue );
	LeaveCriticalSection( &g_cDeviceFlag );
	
	return hResult;
}
#ifdef DIRECT_VERSION_9_MJH

HRESULT CTextureState::GetSamplerState( DWORD Stage, D3DSAMPLERSTATETYPE Type, DWORD* pValue )
{
	HRESULT hResult = S_FALSE;
	
	EnterCriticalSection( &g_cDeviceFlag );
	hResult = g_lpDevice->GetSamplerState( Stage , Type , pValue );
	LeaveCriticalSection( &g_cDeviceFlag );
	
	return hResult;
} // SamplerStage     MJH

#endif // DIRECT_VERSION_9_MJH
void CTextureState::InitTextureState(void)
{
	
	EnterCriticalSection( &g_cDeviceFlag );
	for( int i = 0 ; i < TEXTURE_STATE_STAGE_MAXCOUNT ; ++i )
	{
		g_lpDevice->SetTextureStageState( i , D3DTSS_COLOROP   , m_StateList[ i ].ColorOP   );
		g_lpDevice->SetTextureStageState( i , D3DTSS_COLORARG1 , m_StateList[ i ].ColorArg1 );
		g_lpDevice->SetTextureStageState( i , D3DTSS_COLORARG2 , m_StateList[ i ].ColorArg2 );

		g_lpDevice->SetTextureStageState( i , D3DTSS_ALPHAOP   , m_StateList[ i ].AlphaOP   );
		g_lpDevice->SetTextureStageState( i , D3DTSS_ALPHAARG1 , m_StateList[ i ].AlphaArg1 );
		g_lpDevice->SetTextureStageState( i , D3DTSS_ALPHAARG2 , m_StateList[ i ].AlphaArg2 );

		g_lpDevice->SetTextureStageState( i , D3DTSS_TEXCOORDINDEX , m_StateList[ i ].TexCoordIndex );
#ifdef DIRECT_VERSION_9_MJH
		g_lpDevice->SetSamplerState( i , D3DSAMP_ADDRESSU , m_StateList[ i ].AddressU );
		g_lpDevice->SetSamplerState( i , D3DSAMP_ADDRESSV , m_StateList[ i ].AddressV );
		g_lpDevice->SetSamplerState( i , D3DSAMP_ADDRESSW , m_StateList[ i ].AddressW );
		g_lpDevice->SetSamplerState( i , D3DSAMP_BORDERCOLOR , m_StateList[ i ].BorderColor );
#else
		g_lpDevice->SetTextureStageState( i , D3DTSS_ADDRESSU , m_StateList[ i ].AddressU );
		g_lpDevice->SetTextureStageState( i , D3DTSS_ADDRESSV , m_StateList[ i ].AddressV );
		g_lpDevice->SetTextureStageState( i , D3DTSS_ADDRESSW , m_StateList[ i ].AddressW );
		g_lpDevice->SetTextureStageState( i , D3DTSS_BORDERCOLOR , m_StateList[ i ].BorderColor );
#endif // DIRECT_VERSION_9_MJH

#ifdef DIRECT_VERSION_9_MJH
		g_lpDevice->SetSamplerState( i , D3DSAMP_MAGFILTER , m_StateList[ i ].MAGFilter );
		g_lpDevice->SetSamplerState( i , D3DSAMP_MINFILTER , m_StateList[ i ].MINFilter );
		g_lpDevice->SetSamplerState( i , D3DSAMP_MIPFILTER , m_StateList[ i ].MIPFilter );
#else
		g_lpDevice->SetTextureStageState( i , D3DTSS_MAGFILTER , m_StateList[ i ].MAGFilter );
		g_lpDevice->SetTextureStageState( i , D3DTSS_MINFILTER , m_StateList[ i ].MINFilter );
		g_lpDevice->SetTextureStageState( i , D3DTSS_MIPFILTER , m_StateList[ i ].MIPFilter );
#endif // DIRECT_VERSION_9_MJH

		g_lpDevice->SetTextureStageState( i , D3DTSS_TEXTURETRANSFORMFLAGS , m_StateList[ i ].TextureTransFormFlags );
	}
	LeaveCriticalSection( &g_cDeviceFlag );
}

void CTextureState::DisableTextureStageState(DWORD Stage)
{
	SetTextureColorOP( Stage , D3DTOP_DISABLE );
	SetTextureAlphaOP( Stage , D3DTOP_DISABLE );
}



