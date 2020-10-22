//.......................................................................................................
// 
// Set Texture State Header																	By wxywxy
//
//.......................................................................................................

#ifndef __CTextureState_H
#define __CTextureState_H

#ifdef DIRECT_VERSION_9_MJH
#include <d3d9.h>
#else
#include <d3d8.h>
#endif // DIRECT_VERSION_9_MJH

#define TEXTURE_STATE_STAGE_MAXCOUNT	3

typedef struct s_Texture_State_Data
{
	DWORD	ColorOP;
	DWORD	ColorArg1;
	DWORD	ColorArg2;
    
	DWORD	AlphaOP;
	DWORD	AlphaArg1;
	DWORD	AlphaArg2;

	DWORD	TexCoordIndex;
	
	DWORD	AddressU;
	DWORD	AddressV;
	DWORD	AddressW;

	DWORD	BorderColor;

	DWORD	MAGFilter;
	DWORD	MINFilter;
	DWORD	MIPFilter;

	DWORD	TextureTransFormFlags;
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DTEXTURE9 pTexture;
#else
	LPDIRECT3DTEXTURE8 pTexture;
#endif // DIRECT_VERSION_9_MJH	 

	s_Texture_State_Data()
	{
		memset(this, 0, sizeof(s_Texture_State_Data));
	}

}TEXTURE_STATE_DATA,*LPTEXTURE_STATE_DATA;

class CTextureState
{
public:
	 
	 CTextureState();
	~CTextureState();

public:

	void InitTextureState(void);

public:

	DWORD	GetTextureColorOP(DWORD Stage);
	DWORD   GetTextureColorArg1(DWORD Stage);
	DWORD   GetTextureColorArg2(DWORD Stage);

	DWORD	GetTextureAlphaOP(DWORD Stage);
	DWORD	GetTextureAlphaArg1(DWORD Stage);
	DWORD   GetTextureAlphaArg2(DWORD Stage);

	DWORD   GetTextureMagFilter(DWORD Stage);
	DWORD   GetTextureMinFilter(DWORD Stage);
	DWORD   GetTextureMipFilter(DWORD Stage);
	
	HRESULT SetTextureColorOP(DWORD Stage,DWORD Value);
	HRESULT SetTextureColorArg1(DWORD Stage,DWORD Value);
	HRESULT SetTextureColorArg2(DWORD Stage,DWORD Value);
	HRESULT SetTextureAlphaOP(DWORD Stage,DWORD Value);
	HRESULT SetTextureAlphaArg1(DWORD Stage,DWORD Value);
	HRESULT SetTextureAlphaArg2(DWORD Stage,DWORD Value);
	HRESULT SetTextureCoordIndex(DWORD Stage,DWORD Value);
	HRESULT SetTextureAddressU(DWORD Stage,DWORD Value);
	HRESULT SetTextureAddressV(DWORD Stage,DWORD Value);
	HRESULT SetTextureAddressW(DWORD Stage,DWORD Value);
	HRESULT SetTextureBorderColor(DWORD Stage,DWORD Value);
	HRESULT SetTextureMagFilter(DWORD Stage,DWORD Value);
	HRESULT SetTextureMinFilter(DWORD Stage,DWORD Value);
	HRESULT SetTextureMipFilter(DWORD Stage,DWORD Value);
	HRESULT SetTextureTransFormFlags(DWORD Stage,DWORD Value);
#ifdef DIRECT_VERSION_9_MJH
	HRESULT SetTexture(DWORD Stage,LPDIRECT3DTEXTURE9 pTexture);
#else
	HRESULT SetTexture(DWORD Stage,LPDIRECT3DTEXTURE8 pTexture);
#endif // DIRECT_VERSION_9_MJH
	HRESULT DisableTexture(DWORD Stage);
	
	HRESULT GetTextureStageState( DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD* pValue );
#ifdef DIRECT_VERSION_9_MJH
	HRESULT GetSamplerState( DWORD Stage, D3DSAMPLERSTATETYPE Type, DWORD* pValue );
#endif // DIRECT_VERSION_9_MJH

	void	DisableTextureStageState(DWORD Stage);

protected:

	TEXTURE_STATE_DATA	m_StateList[ TEXTURE_STATE_STAGE_MAXCOUNT ];
};

#endif