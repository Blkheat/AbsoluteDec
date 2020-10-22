#include "RockPCH.h"
#include "VFileSystem.h"

#include "CEFEmblemFlag.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include <SOUND\Sound.H>
#include <NETWORK\\Protocol.h>
#include <Base\\D3DBase.h>
#include "..\\RockClient.h"
#include "..\\quadlist.h"
#include <Map\\Field.h>
#include "..\\Map.H"
#include "..\\PC.H"
#include "..\\Effect.H"
#include "..\\Camera.H"
#include "..\\RockClient.h"

#include "..\\obj\\EffItemManager.h"
#include "..\\..\\CRenderManager.h"
#include "..\\..\\rocklibs\\RockInterface\\EmblemTexManager.h"

extern CBravoMap g_Map;
extern long		 g_nowTime;

int CEFEmblemFlag::InitDevice(void)
{
	return TRUE;
}

int	CEFEmblemFlag::DeleteDevice(void)
{
	return TRUE;
}


CEFEmblemFlag::CEFEmblemFlag()
{
	m_bEmblemFlag = true;
	m_dTexCode = 0;
}

CEFEmblemFlag::~CEFEmblemFlag()
{
}

void CEFEmblemFlag::Init(void)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr ) )
	{
		SetDestroy();

		return;
	}

	SetLinkItem();
	int Code = -1; 

	Code = g_Particle.m_EffectManager.GetWearCode( m_pSrcChr , n_WearSlot_SKIN_Serve );
	if(-1  == Code)
	{
		Code = g_Particle.m_EffectManager.GetWearCode( m_pSrcChr , n_WearSlot_Serve );
	}

	SEffItemInfo ItemInfo = g_EffItemManager.FindItemInfo(Code);

	if( ItemInfo.nCode != (DWORD)Code )
	{
		SetDestroy();
		return;
	}
	
	m_EffItemInfo = ItemInfo;
	
	//모델전환되지 않는 아이템이라면
	if(!ItemInfo.isChange)
	{
		SetDestroy();
		return;
	}
	
#ifdef NEW_EFFITEMTYPE
	//버텍스 애니메이션이 아니라면
	if(!ItemInfo.isVtxAni)
	{
		SetDestroy();
		return;
	}
#endif
	
	///--엠블렌 플래그 적용
	m_bEmblemFlag = (bool)ItemInfo.isEmblem;

	SModelList sModel = g_EffItemManager.FindModelList( m_EffItemInfo );

	///-- 모델데이터가 없으면
	if(!sModel.pModel || !sModel.pMoveModel)
	{
		SetDestroy();
		return;
	}

	m_pModel = sModel.pModel;
	m_pMoveModel = sModel.pMoveModel;
	
	m_pModel->m_lPrevTime = g_nowTime;
	
	m_VAniModel.Create( m_pModel, g_nowTime );
	m_VAniMoveModel.Create( m_pMoveModel, g_nowTime );
	
	m_dTexCode = ItemInfo.TexCode;
}


int CEFEmblemFlag::Render(float dtime)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr ) )
	{
		SetDestroy();
		return TRUE;
	}

	if( m_pSrcChr->m_CharAlpha.GetState() == CHAR_NONE_DISPLAY || !m_pSrcChr->m_bIsVisible )
		return TRUE;

	if( m_pSrcChr == g_Pc.GetPlayer() && !(g_Pc.GetPlayer()->IsMyPlayerRender) )
	{
		return TRUE;		
	}
	
	if( !m_pSrcChr->m_IsAllLTexture )
		return TRUE;
	
	if( m_pSrcChr->m_curt_event.IsHideWing )
	{
		return TRUE;
	}

	SModelList sModel = g_EffItemManager.FindModelList( m_EffItemInfo );
	if( sModel.pModel == NULL )
	{
		m_pModel = NULL;
		m_pMoveModel = NULL;
		return TRUE;
	}
	else
	{
		m_pModel = sModel.pModel;
		m_pMoveModel = sModel.pMoveModel;
		m_pModel->m_lPrevTime = g_nowTime;
	}	
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DTEXTURE9	pTexture1 = NULL;
#else
	LPDIRECT3DTEXTURE8	pTexture1 = NULL;
#endif // DIRECT_VERSION_9_MJH
	
	if( m_bEmblemFlag )
	{
		DWORD FrontierID = 0;	
		
		if(m_pSrcChr->m_bPC)
		{
			// 플레이어
			if( m_pSrcChr->GetUnique() == g_Pc.GetPlayer()->GetUnique() )
			{
				FrontierID = nRui->GetPcInvenInfo()->FrontierMyInfo->FrontierID;
			}
			// 다른 유저
			else
			{
				if( !m_pSrcChr->m_bPC )
				{
					SetDestroy();
					return TRUE;
				}

				FrontierID = g_Pc.m_PcTable[ m_pSrcChr->m_nTableIndex ].char_info.theFrontierID;
			}
		}

		BOOL IsApplNationEmblem = FALSE; 
		
		if( FrontierID )
		{
			VESIONTEX *pData = g_EmblemTexManager.FindForntierVersionTex( FrontierID );

			if( NULL != pData )
			{
				if( NULL != pData->pRenderTexture )
				{
					pTexture1 = pData->pRenderTexture->GetTexture();
				}
			}
			else
			{
				IsApplNationEmblem = TRUE;
			}
		}
		else
		{
			///-- 프론티어에 소속되어있지 않을 경우
			if( m_pSrcChr->m_bPC == true )
			{
				IsApplNationEmblem = TRUE;			
			}
		}

		if( IsApplNationEmblem )
		{
			if( m_pSrcChr->GetAttrIndex() == n_Millena_Man ||
				m_pSrcChr->GetAttrIndex() == n_Millena_Woman )
			{
				pTexture1 = g_EmblemTexManager.m_MEmbTexture;	
			}
			else
			{
				pTexture1 = g_EmblemTexManager.m_REmbTexture;
			}		
		}
	}	
	
	D3DXMATRIX	matWorld;
	
	D3DXMatrixIdentity( &matWorld );
	
	m_pSrcChr->GetBoneLinkTM(2, matWorld, FALSE);	

	g_RenderManager.SetTransform( D3DTS_WORLD, &matWorld );	
#ifdef DIRECT_VERSION_9_MJH
	D3DLIGHT9 preLight;
#else
	D3DLIGHT8 preLight;
#endif // DIRECT_VERSION_9_MJH	
	g_RenderManager.GetLight( 0, &preLight );
	g_RenderManager.SetLight( 0, &g_Pc_Manager.theCharacter_Light );

#ifdef DIRECT_VERSION_9_MJH
	D3DMATERIAL9	mtrl;		
	ZeroMemory( &mtrl, sizeof(D3DMATERIAL9) );
#else
	D3DMATERIAL8	mtrl;		
	ZeroMemory( &mtrl, sizeof(D3DMATERIAL8) );
#endif // DIRECT_VERSION_9_MJH
		
	mtrl.Power = 1.0f;
		
	mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
	mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
	mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;
	mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;	

	int RenderMaxCount = 1;
	
	DWORD dFVF = g_RenderManager.GetFVF();
	DWORD dCullMode = g_RenderManager.GetCullMode();
	DWORD dZWriteEnable = g_RenderManager.GetZWriteEnable();
	DWORD dZEnable = g_RenderManager.GetZEnable();
	DWORD dAlphaBlendEnable = g_RenderManager.GetAlphaBlendEnable();
	DWORD dSrcBlend = g_RenderManager.GetSrcBlend();
	DWORD dDestBlend = g_RenderManager.GetDestBlend(); 
	DWORD dZFunc = g_RenderManager.GetZFunc();
	DWORD dLighting = g_RenderManager.GetLighting();
	///--JAPAN_BUG_MODIFY
	DWORD dFogEnable = g_RenderManager.GetFogEnable();
	g_RenderManager.SetFogEnable( TRUE );

	switch( m_pSrcChr->m_CharAlpha.GetState() )
	{
		case CHAR_HIDE:	
			
			RenderMaxCount = 2;
			
			mtrl.Diffuse.a = m_pSrcChr->m_CharAlpha.fAlpha;
			
			g_RenderManager.SetCullMode( D3DCULL_NONE );
			
			g_RenderManager.SetZWriteEnable( TRUE );
			g_RenderManager.SetZEnable( TRUE );
			
			g_RenderManager.SetAlphaBlendEnable( TRUE );

			break;

		case CHAR_NORMAL:
			
			RenderMaxCount = 1;
			
			g_RenderManager.SetCullMode( D3DCULL_NONE );
			
			g_RenderManager.SetZWriteEnable( TRUE );
			g_RenderManager.SetZEnable( TRUE );
			
			g_RenderManager.SetAlphaBlendEnable( TRUE );
			g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
			g_RenderManager.SetDestBlend( D3DBLEND_INVSRCALPHA );

			g_RenderManager.SetZFunc( D3DCMP_LESSEQUAL );

			break;
	}

	g_RenderManager.SetMaterial( &mtrl );

	g_RenderManager.LightEnable(0,TRUE);
	g_RenderManager.LightEnable(1,TRUE);
		
	g_RenderManager.SetLighting( TRUE );

	g_EffItemManager.SetTexture( m_dTexCode );
	
	DWORD ColorOP, ColorARG1, ColorARG2; 
	DWORD AlphaOP, AlphaARG1, AlphaARG2;
	ColorOP = ColorARG1 = ColorARG2 = 0;
	AlphaOP = AlphaARG1 = AlphaARG2 = 0;

	if( pTexture1 )
	{
		g_RenderManager.SetTexture( 1 , pTexture1 );
		
		g_RenderManager.GetTextureStageState( 0, D3DTSS_COLOROP		, &ColorOP );
		g_RenderManager.GetTextureStageState( 0, D3DTSS_COLORARG1	, &ColorARG1   );
		g_RenderManager.GetTextureStageState( 0, D3DTSS_COLORARG2	, &ColorARG2   );

		g_RenderManager.GetTextureStageState( 0, D3DTSS_ALPHAOP		, &AlphaOP );
		g_RenderManager.GetTextureStageState( 0, D3DTSS_ALPHAARG1	, &AlphaARG1   );
		g_RenderManager.GetTextureStageState( 0, D3DTSS_ALPHAARG2	, &AlphaARG2   );

		g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_SELECTARG1 );
		g_RenderManager.SetTextureColorArg1( 0 , D3DTA_TEXTURE );
		
		g_RenderManager.SetTextureAlphaOP  ( 0 , D3DTOP_MODULATE );
		g_RenderManager.SetTextureAlphaArg1( 0 , D3DTA_CURRENT );
		g_RenderManager.SetTextureAlphaArg2( 0 , D3DTA_TEXTURE );	
		
		g_RenderManager.SetTextureCoordIndex ( 1 , 0 );
		
		g_RenderManager.SetTextureColorOP  ( 1 , D3DTOP_ADD );
		g_RenderManager.SetTextureColorArg1( 1 , D3DTA_CURRENT   );
		g_RenderManager.SetTextureColorArg2( 1 , D3DTA_TEXTURE   );
		
		g_RenderManager.SetTextureAlphaOP  ( 1 , D3DTOP_MODULATE );
		g_RenderManager.SetTextureAlphaArg1( 1 , D3DTA_CURRENT );
		g_RenderManager.SetTextureAlphaArg2( 1 , D3DTA_TEXTURE );

		g_RenderManager.SetTextureMipFilter ( 1 , D3DTEXF_NONE );
		g_RenderManager.SetTextureMagFilter ( 1 , D3DTEXF_LINEAR );
		g_RenderManager.SetTextureMinFilter ( 1 , D3DTEXF_POINT );
	}
	else
	{
		g_RenderManager.DisableTextureStageState( 1 );
	}

	for( int TwoPass = 0 ; TwoPass < RenderMaxCount ; ++TwoPass )
	{
		if( m_pSrcChr->m_CharAlpha.GetState() == CHAR_HIDE )
		{
			switch( TwoPass )
			{
			case 0 :
				g_RenderManager.SetSrcBlend( D3DBLEND_ZERO );
				g_RenderManager.SetDestBlend( D3DBLEND_ONE  );
				g_RenderManager.SetZFunc( D3DCMP_LESSEQUAL );
				break;
			case 1:
				g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
				g_RenderManager.SetDestBlend( D3DBLEND_INVSRCALPHA  );
				g_RenderManager.SetZFunc( D3DCMP_EQUAL );
				break;
			}
		}

		if( m_pSrcChr->m_curt_event.type == SM_MOVE_BROADCAST )
		{
			m_VAniMoveModel.Draw( m_pMoveModel );
		}
		else
			m_VAniModel.Draw( m_pModel );
		
	}

	if( pTexture1 )
	{	
		g_RenderManager.SetTextureColorOP  ( 0 , ColorOP );
		g_RenderManager.SetTextureColorArg1( 0 , ColorARG1 );			
		g_RenderManager.SetTextureColorArg2( 0 , ColorARG2 );
		
		g_RenderManager.SetTextureAlphaOP  ( 0 , AlphaOP );
		g_RenderManager.SetTextureAlphaArg1( 0 , AlphaARG1 );
		g_RenderManager.SetTextureAlphaArg1( 0 , AlphaARG2 );
	}

	g_RenderManager.DisableTextureStageState( 1 );
	g_RenderManager.DisableTextureStageState( 2 );
	g_RenderManager.SetTexture( 0, NULL );
	g_RenderManager.SetTexture( 1 , NULL );	
	g_RenderManager.LightEnable( 0 , TRUE );	
	g_RenderManager.LightEnable( 1, TRUE );

	g_RenderManager.SetFVF(dFVF);
	g_RenderManager.SetCullMode( dCullMode );			
	g_RenderManager.SetZWriteEnable( dZWriteEnable );
	g_RenderManager.SetZEnable( dZEnable );			
	g_RenderManager.SetAlphaBlendEnable( dAlphaBlendEnable );
	g_RenderManager.SetSrcBlend( dSrcBlend );
	g_RenderManager.SetDestBlend( dDestBlend );
	g_RenderManager.SetZFunc( dZFunc );
	g_RenderManager.SetLighting( dLighting );
	///--JAPAN_BUG_MODIFY
	g_RenderManager.SetFogEnable( dFogEnable );

	g_RenderManager.SetLight( 0, &preLight );

	return TRUE;
}

//.............................................................................................................
// 링크이펙트 경우 그림자를 표시해야할부분에서 렌더링 해준다 By wxywxy
//.............................................................................................................
int CEFEmblemFlag::RenderShadow(void)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr ) )
		return FALSE;

	if( m_pSrcChr->m_curt_event.IsHideWing )
	{
		return TRUE;
	}

	SModelList sModel = g_EffItemManager.FindModelList( m_EffItemInfo );
	if( sModel.pModel == NULL )
	{
		m_pModel = NULL;
		m_pMoveModel = NULL;
		return TRUE;
	}

	D3DXMATRIX	matWorld;
	D3DXMatrixIdentity( &matWorld );	
	m_pSrcChr->GetBoneLinkTM( 2, matWorld, FALSE );

	if( m_pSrcChr->m_curt_event.type == SM_MOVE_BROADCAST )
	{
		D3DXMATRIX  matShadow = g_Particle.m_EffectManager.MakeShadowMatrix( 
			m_pSrcChr , &m_VAniMoveModel , matWorld );
	
		g_RenderManager.SetTransform( D3DTS_WORLD,	&matWorld );		

	//	m_VAniMoveModel.Draw( m_pMoveModel );
	}
	else
	{
		D3DXMATRIX  matShadow = g_Particle.m_EffectManager.MakeShadowMatrix( 
			m_pSrcChr , &m_VAniModel , matWorld );	
		
		g_RenderManager.SetTransform( D3DTS_WORLD,	&matWorld );	
	
		m_VAniModel.Draw( m_pModel );
	}

	return TRUE;
}

void CEFEmblemFlag::ApplyShaowProj()
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr ) )
		return;

	if( m_pSrcChr->m_curt_event.IsHideWing )
	{
		return;
	}

	SModelList sModel = g_EffItemManager.FindModelList( m_EffItemInfo );
	if( sModel.pModel == NULL )
	{
		m_pModel = NULL;
		m_pMoveModel = NULL;
		return;
	}

	D3DXMATRIX	matWorld;
	D3DXMatrixIdentity( &matWorld );	
	m_pSrcChr->GetBoneLinkTM( 2, matWorld, FALSE );

	if( m_pSrcChr->m_curt_event.type == SM_MOVE_BROADCAST )
	{
		g_Particle.m_EffectManager.ApplyShaowProj( 
			m_pSrcChr , &m_VAniMoveModel , matWorld );
	}
	else
	{
		g_Particle.m_EffectManager.ApplyShaowProj( 
			m_pSrcChr , &m_VAniModel , matWorld );
	}
}

int CEFEmblemFlag::CheckFrame(void)
{
	return TRUE;
}

int CEFEmblemFlag::Update(float dtime)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr ) )
	{
		SetDestroy();

		return TRUE;
	}

	SModelList sModel = g_EffItemManager.FindModelList( m_EffItemInfo );
	if( sModel.pModel == NULL )
	{
		m_pModel = NULL;
		m_pMoveModel = NULL;
		return TRUE;
	}
	else
	{
		m_pModel = sModel.pModel;
		m_pMoveModel = sModel.pMoveModel;
		m_pModel->m_lPrevTime = g_nowTime;
	}	

	m_VAniModel.Update( m_pModel, g_nowTime, m_pSrcChr );
	m_VAniMoveModel.Update( m_pMoveModel, g_nowTime, m_pSrcChr );

	return TRUE;
}