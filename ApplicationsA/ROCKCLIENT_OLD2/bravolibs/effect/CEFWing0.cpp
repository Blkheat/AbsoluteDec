#include "RockPCH.h"
#include "VFileSystem.h"

#include "CEFWing0.h"
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

int CEFWing0::InitDevice(void)
{
	return TRUE;
}

int	CEFWing0::DeleteDevice(void)
{
	return TRUE;
}


CEFWing0::CEFWing0()
{
	m_dTexCode = 0;
	m_bEmblemFlag = false;
	m_nItemCode = -1;
	m_nLinkBone = -1;

	m_IsHighLight = FALSE;
	m_IsSemiOpacity = FALSE;
}

CEFWing0::~CEFWing0()
{
}

void CEFWing0::SetDestroy(BOOL ISDamageAct, BOOL ISBasicAttack)
{
	CEffectBase::SetDestroy( ISDamageAct, ISBasicAttack );
}

void CEFWing0::Init(void)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr ) )
	{
		SetDestroy();
		return;
	}
	
	///-- 펫에 왜 날개가 붙는거지;;;;
	if( !m_pSrcChr->m_bPC )
	{
		if( m_pSrcChr->m_State.nCharacterType != nCharacterTypePlayerNPC )
		{	
			SetDestroy();
			return;
		}
	}

	SetLinkItem();

	int Code = -1;
	if(-1 == m_nItemCode)
	{
		if(m_pSrcChr == g_Pc.GetPlayer() )
		{

			Code = g_Particle.m_EffectManager.GetWearCode( m_pSrcChr , n_WearSlot_SKIN_Serve );
			if(-1  == Code)
			{
				Code = g_Particle.m_EffectManager.GetWearCode( m_pSrcChr , n_WearSlot_Serve );
			}
		}
		else
		{

			Code = g_Particle.m_EffectManager.GetWearCode( m_pSrcChr , n_WearSlot_SKIN_Serve );
			if(-1  == Code)
			{
				Code = g_Particle.m_EffectManager.GetWearCode( m_pSrcChr , n_WearSlot_Serve );
			}
		}
	}
	else
	{
		Code = m_nItemCode;
	}	
	
	SEffItemInfo ItemInfo = g_EffItemManager.FindItemInfo(Code);
	
	if( ItemInfo.nCode != (DWORD)Code )
	{
		SetDestroy();
		return;
	}
	
	m_EffItemInfo = ItemInfo;
	m_Light = g_Pc_Manager.theCharacter_Light;

	//모델전환되는 아이템이라면
	if(ItemInfo.isChange)
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
	if(!sModel.pModel)
	{
		SetDestroy();
		return;
	}	

	m_pModel = sModel.pModel;
	m_pModel->m_lPrevTime = g_nowTime;

	m_VAniModel.Create( m_pModel , g_nowTime );

	m_dTexCode = ItemInfo.TexCode;
}


int CEFWing0::Render(float dtime)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr ) )
	{
		SetDestroy();
		return TRUE;
	}	

	if( m_pSrcChr->m_CharAlpha.GetState() == CHAR_NONE_DISPLAY || !m_pSrcChr->m_bIsVisible )
		return TRUE;
	
	if( !m_pSrcChr->m_IsAllLTexture )
		return TRUE;

	if( m_pSrcChr == g_Pc.GetPlayer() && !(g_Pc.GetPlayer()->IsMyPlayerRender) )
	{
		return TRUE;		
	}
	
	if( m_pSrcChr->m_curt_event.IsHideWing )
	{
		return TRUE;
	}

	SModelList sModel = g_EffItemManager.FindModelList( m_EffItemInfo );
	if( sModel.pModel == NULL )
	{
		m_pModel = NULL;
		return TRUE;
	}
	else
	{
		m_pModel = sModel.pModel;	
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
		}
	}

	D3DXMATRIX	matWorld;
	
	D3DXMatrixIdentity( &matWorld );
	
	if(m_nLinkBone == -1)
	{
		m_pSrcChr->GetBoneLinkTM( 2, matWorld, FALSE );
	}
	else
	{
		m_pSrcChr->GetBoneLinkTM( m_nLinkBone , matWorld, FALSE );	
	}
	
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

	mtrl.Specular.r = 1.0f;
	mtrl.Specular.g = 1.0f;
	mtrl.Specular.b = 1.0f;
	mtrl.Specular.a = 1.0f;
	mtrl.Power = 20;

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

	DWORD dSpecularEnable = 0;
	DWORD dSpecularMaterialSource = 0;
#ifdef DIRECT_VERSION_9_MJH
	D3DLIGHT9 Light;
#else
	D3DLIGHT8 Light;
#endif // DIRECT_VERSION_9_MJH
	
	if( m_IsHighLight )
	{
		g_RenderManager.GetRenderState( D3DRS_SPECULARENABLE, &dSpecularEnable);	
		g_RenderManager.GetRenderState( D3DRS_SPECULARMATERIALSOURCE, &dSpecularMaterialSource);	
		g_RenderManager.SetRenderState( D3DRS_SPECULARENABLE, TRUE );
		g_RenderManager.SetRenderState( D3DRS_SPECULARMATERIALSOURCE, D3DMCS_MATERIAL );		
		g_RenderManager.GetLight(0, &Light);
	}

	int uAlphaState = m_pSrcChr->m_CharAlpha.GetState();

	if(m_IsSemiOpacity)
	{
		uAlphaState = CHAR_HIDE;
	}
	
	switch( uAlphaState )
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

	if( m_IsHighLight )
	{
		m_Light.Position = g_Camera.m_vEye;
		m_Light.Direction = g_Camera.m_vDir;
		g_RenderManager.SetLight( 0, &m_Light );
	}

	g_RenderManager.SetMaterial( &mtrl );		
	g_RenderManager.LightEnable( 0, TRUE );
	g_RenderManager.LightEnable( 1, TRUE );		
	g_RenderManager.SetLighting( TRUE );
	g_EffItemManager.SetTexture( m_dTexCode );
	
	DWORD ColorOP, ColorARG1, ColorARG2; 
	DWORD AlphaOP, AlphaARG1, AlphaARG2;
	ColorOP = ColorARG1 = ColorARG2 = 0;
	AlphaOP = AlphaARG1 = AlphaARG2 = 0;

	g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureColorArg1( 0 , D3DTA_DIFFUSE );
	g_RenderManager.SetTextureColorArg1( 0 , D3DTA_TEXTURE );

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
		
		g_RenderManager.SetTextureAlphaOP  ( 0 , D3DTOP_SELECTARG1 );
		g_RenderManager.SetTextureAlphaArg1( 0 , D3DTA_TEXTURE );		
		
		g_RenderManager.SetTextureCoordIndex ( 1 , 0 );
		
		g_RenderManager.SetTextureColorOP  ( 1 , D3DTOP_ADD );
		g_RenderManager.SetTextureColorArg1( 1 , D3DTA_CURRENT   );
		g_RenderManager.SetTextureColorArg2( 1 , D3DTA_TEXTURE   );
		
		g_RenderManager.SetTextureAlphaOP  ( 1 , D3DTOP_SELECTARG1 );
		g_RenderManager.SetTextureAlphaArg1( 1 , D3DTA_CURRENT );

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
		if( uAlphaState == CHAR_HIDE )
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
	g_RenderManager.SetTexture( 1, NULL );	
	g_RenderManager.LightEnable( 0, TRUE );	
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

	if( m_IsHighLight )
	{
		g_RenderManager.SetLight(0, &Light);	
		g_RenderManager.SetRenderState( D3DRS_SPECULARMATERIALSOURCE, dSpecularMaterialSource );
		g_RenderManager.SetRenderState( D3DRS_SPECULARENABLE, dSpecularEnable );
	}

	g_RenderManager.SetLight( 0, &preLight );

	return TRUE;
}

//.............................................................................................................
// 링크이펙트 경우 그림자를 표시해야할부분에서 렌더링 해준다 By wxywxy
//.............................................................................................................
int CEFWing0::RenderShadow(void)
{
	if( NULL == m_pSrcChr )
		return FALSE;

	if( m_pSrcChr->m_curt_event.IsHideWing )
	{
		return TRUE;
	}

	SModelList sModel = g_EffItemManager.FindModelList( m_EffItemInfo );
	if( sModel.pModel == NULL )
	{
		m_pModel = NULL;
		return TRUE;
	}
	else
	{
		m_pModel = sModel.pModel;	
		m_pModel->m_lPrevTime = g_nowTime;
	}	
	
	D3DXMATRIX	matWorld;
	D3DXMatrixIdentity( &matWorld );
	
	if(m_nLinkBone == -1)
	{
		m_pSrcChr->GetBoneLinkTM( 2, matWorld, FALSE );
	}
	else
	{
		m_pSrcChr->GetBoneLinkTM( m_nLinkBone , matWorld, FALSE );	
	}
	
	D3DXMATRIX  matShadow = g_Particle.m_EffectManager.MakeShadowMatrix( 
		m_pSrcChr , &m_VAniModel , matWorld );

	g_RenderManager.SetTransform( D3DTS_WORLD,	&matWorld );

	m_VAniModel.Draw( m_pModel );

	return TRUE;
}

void CEFWing0::ApplyShaowProj()
{
	if( NULL == m_pSrcChr )
		return;

	if( m_pSrcChr->m_curt_event.IsHideWing )
	{
		return;
	}

	SModelList sModel = g_EffItemManager.FindModelList( m_EffItemInfo );
	if( sModel.pModel == NULL )
	{
		m_pModel = NULL;
		return;
	}
	else
	{
		m_pModel = sModel.pModel;	
		m_pModel->m_lPrevTime = g_nowTime;
	}

	D3DXMATRIX	matWorld;
	D3DXMatrixIdentity( &matWorld );
	
	if(m_nLinkBone == -1)
	{
		m_pSrcChr->GetBoneLinkTM( 2, matWorld, FALSE );
	}
	else
	{
		m_pSrcChr->GetBoneLinkTM( m_nLinkBone , matWorld, FALSE );	
	}	
	
	g_Particle.m_EffectManager.ApplyShaowProj( m_pSrcChr , 
				&m_VAniModel , matWorld );
}

int CEFWing0::CheckFrame(void)
{
	return TRUE;
}

int CEFWing0::Update(float dtime)
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
		return TRUE;
	}
	else
	{
		m_pModel = sModel.pModel;	
		m_pModel->m_lPrevTime = g_nowTime;
	}

	m_VAniModel.Update( m_pModel, g_nowTime, m_pSrcChr );	

	return TRUE;
}