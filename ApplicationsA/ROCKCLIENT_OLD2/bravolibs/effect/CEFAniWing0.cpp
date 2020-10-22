#include "RockPCH.h"
#include "VFileSystem.h"

#include "CEFAniWing0.h"
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

int CEFAniWing0::InitDevice(void)
{
	return TRUE;
}

int	CEFAniWing0::DeleteDevice(void)
{
	return TRUE;
}


CEFAniWing0::CEFAniWing0()
{
	m_pModel = NULL;
	m_pBone = NULL;
	
	m_dTexCode = 0;
	m_nItemCode = -1;
	m_nLinkBone = -1;

	m_IsHighLight = FALSE;
	m_IsSemiOpacity = FALSE;
	m_IsMotionSync = FALSE;

	m_next_frame = 0;
	m_curt_frame = 0;
	m_ani_frame = 0;
	m_end_frame = 0;
	m_middle_time = 0.0f;

	m_PreTime = 0;

	m_uFrame = 30;
}

CEFAniWing0::~CEFAniWing0()
{
}

void CEFAniWing0::Init(void)
{
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

		Code = g_Particle.m_EffectManager.GetWearCode( m_pSrcChr , n_WearSlot_SKIN_Serve );
		if(-1  == Code)
		{
			Code = g_Particle.m_EffectManager.GetWearCode( m_pSrcChr , n_WearSlot_Serve );
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
	//버텍스 애니메이션 이라면
	if(ItemInfo.isVtxAni)
	{
		SetDestroy();
		return;
	}

	m_IsSemiOpacity = ItemInfo.isSemiTrans;
	m_IsMotionSync = ItemInfo.isMotionSync;

#endif

	SModelList sModel = g_EffItemManager.FindModelList( m_EffItemInfo );

	///-- 모델데이터가 없으면
	if(!sModel.pModel)
	{
		SetDestroy();
		return;
	}	

	m_pModel = sModel.pModel;
	m_pModel->m_lPrevTime = g_nowTime;
	m_dTexCode = ItemInfo.TexCode;

#ifdef NEW_EFFITEMTYPE
	m_pBone = g_EffItemManager.FineAniList(m_EffItemInfo);

	if(!m_pBone)
	{
		SetDestroy();
		return;
	}

	m_pBone->m_lPrevTime = g_nowTime;

#endif

	m_VtxBuffer.CreateBuffer( m_pModel->m_header.m_num_vertex );

	// 버퍼에넣기					
	OBJECTVERTEX*	pOBJVERTEX = NULL;
#ifdef DIRECT_VERSION_9_MJH
	if( FAILED( m_VtxBuffer.NormalVB->Lock( 0, 0, (void**)&pOBJVERTEX, 0 ) ) )
#else
	if( FAILED( m_VtxBuffer.NormalVB->Lock( 0, 0, (BYTE**)&pOBJVERTEX, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH		
	{
		return;
	}
			
	for( int j = 0; j < m_pModel->m_header.m_num_vertex; ++j )
	{
		pOBJVERTEX[j].Normal = m_pModel->m_pVertex[j].m_normal;
		pOBJVERTEX[j].u		 = m_pModel->m_pVertex[j].m_u;
		pOBJVERTEX[j].v	     = m_pModel->m_pVertex[j].m_v;
	}

	m_VtxBuffer.NormalVB->Unlock();
	
	m_PreTime = g_nowTime;

#ifdef NEW_EFFITEMTYPE	
	m_end_frame = m_pBone->m_ani_frame;
#endif
}


int CEFAniWing0::Render(float dtime)
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
		return TRUE;
	}
	else
	{
		m_pModel = sModel.pModel;	
		m_pModel->m_lPrevTime = g_nowTime;
	}

	m_pBone->m_lPrevTime = g_nowTime;

	D3DXMATRIX	matWorld, matWorld2;
	
	D3DXMatrixIdentity( &matWorld );
	D3DXMatrixIdentity( &matWorld2 );
	
	if(m_nLinkBone == -1)
	{
		m_pSrcChr->GetBoneLinkTM( 2, matWorld2, FALSE );
	}
	else
	{
		m_pSrcChr->GetBoneLinkTM( m_nLinkBone , matWorld2, FALSE );	
	}

	matWorld = matWorld2;
	
	if( m_IsMotionSync )
	{
		matWorld = m_pSrcChr->GetPosTM();
		if( m_pSrcChr->m_State.bRide )
		{
			matWorld._41 = m_pSrcChr->GetBonCenterTM()._41;
			matWorld._42 += 12.5f + m_pSrcChr->m_fAddRideHeight + 2.3f;
			matWorld._43 = m_pSrcChr->GetBonCenterTM()._43;
		}
		else
		{
			matWorld._42 += 12.5f;
		}
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
			if( m_IsSemiOpacity )
			{
				mtrl.Diffuse.a = 0.5f;
			}
			
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
	
		RenderAniWing();
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

	g_RenderManager.SetLight( 0, &preLight );

	return TRUE;
}

int CEFAniWing0::RenderAniWing()
{
	g_RenderManager.SetFVF( D3DFVF_OBJECTVERTEX );
		
	g_RenderManager.SetStreamSource( 0, m_VtxBuffer.NormalVB, sizeof(OBJECTVERTEX) );		
	// 렌더링			
	g_RenderManager.SetIndices( m_pModel->m_pIB, 0 );
		
	g_RenderManager.DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, m_pModel->m_header.m_num_vertex, 
															  0, m_pModel->m_header.m_num_face );
	return TRUE;
}

//.............................................................................................................
// 링크이펙트 경우 그림자를 표시해야할부분에서 렌더링 해준다 By wxywxy
//.............................................................................................................
int CEFAniWing0::RenderShadow(void)
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
	
	D3DXMATRIX	matWorld, matWorld2;
	
	D3DXMatrixIdentity( &matWorld );
	D3DXMatrixIdentity( &matWorld2 );
	
	if(m_nLinkBone == -1)
	{
		m_pSrcChr->GetBoneLinkTM( 2, matWorld2, FALSE );
	}
	else
	{
		m_pSrcChr->GetBoneLinkTM( m_nLinkBone , matWorld2, FALSE );	
	}

	matWorld = matWorld2;
	
	if( m_IsMotionSync )
	{
		matWorld = m_pSrcChr->GetPosTM();
		if( m_pSrcChr->m_State.bRide )
		{
			matWorld._41 = m_pSrcChr->GetBonCenterTM()._41;
			matWorld._42 += 12.5f + m_pSrcChr->m_fAddRideHeight + 2.3f;
			matWorld._43 = m_pSrcChr->GetBonCenterTM()._43;
		}
		else
		{
			matWorld._42 += 12.5f;
		}
	}

	g_RenderManager.SetTransform( D3DTS_WORLD,	&matWorld );

	RenderAniWing();

	return TRUE;
}

void CEFAniWing0::ApplyShaowProj()
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

	D3DXMATRIX	matWorld, matWorld2;
	
	D3DXMatrixIdentity( &matWorld );
	D3DXMatrixIdentity( &matWorld2 );
	
	if(m_nLinkBone == -1)
	{
		m_pSrcChr->GetBoneLinkTM( 2, matWorld2, FALSE );
	}
	else
	{
		m_pSrcChr->GetBoneLinkTM( m_nLinkBone , matWorld2, FALSE );	
	}

	matWorld = matWorld2;
	
	if( m_IsMotionSync )
	{
		matWorld = m_pSrcChr->GetPosTM();
		if( m_pSrcChr->m_State.bRide )
		{
			matWorld._41 = m_pSrcChr->GetBonCenterTM()._41;
			matWorld._42 += 12.5f + m_pSrcChr->m_fAddRideHeight + 2.3f;
			matWorld._43 = m_pSrcChr->GetBonCenterTM()._43;
		}
		else
		{
			matWorld._42 += 12.5f;
		}
	}	

	D3DXMATRIX  matLinkItemLocalToLight;

	// 현재라이트위치
	D3DXMatrixMultiply( &matLinkItemLocalToLight, &matWorld, &m_pSrcChr->m_Shadow.matWorldToLight );

	D3DXVECTOR4 vOut;
	
	for( int j = 0; j < m_pModel->m_header.m_num_vertex ; ++j )
	{									
		D3DXVec3Transform( &vOut, &m_VtxBuffer.pVertex[j] , &matLinkItemLocalToLight );
		
		if( (float)fabs(vOut.x/vOut.z) > m_pSrcChr->m_Shadow.fXMax )
		{
			m_pSrcChr->m_Shadow.fXMax = (float)fabs(vOut.x/vOut.z);
		}
		
		if( (float)fabs(vOut.y/vOut.z) > m_pSrcChr->m_Shadow.fYMax )
		{
			m_pSrcChr->m_Shadow.fYMax = (float)fabs(vOut.y/vOut.z);
		}
	}	
}

int CEFAniWing0::CheckFrame(void)
{
	return TRUE;
}

int CEFAniWing0::Update(float dtime)
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
	
	m_pBone->m_lPrevTime = g_nowTime;

	long	RangeTime   = SAFE_TIME_SUB( g_nowTime , m_PreTime );
	int		SkipFrame   = RangeTime / m_uFrame;
	float	fRangeFrame = 0.0f;
	int		NextFrame   = 0;

	if( SkipFrame )
		m_PreTime = g_nowTime;

	if( ( m_ani_frame + SkipFrame ) >= m_end_frame )
	{
		int AddCount = ( m_ani_frame + SkipFrame ) / m_end_frame;
	
		m_ani_frame   = ( SkipFrame + m_ani_frame ) % m_end_frame;
		RangeTime   -= AddCount * m_uFrame;
	}
	else
	{
		m_ani_frame  += SkipFrame;
		RangeTime   -= ( SkipFrame * m_uFrame );
	}

	if( m_pSrcChr && !(m_pSrcChr->m_IsApplyVA) )
	{
		return TRUE;
	}
	
	m_middle_time = ( float ) ( ( float ) RangeTime / ( float ) m_uFrame );	

	m_curt_frame = m_ani_frame;		
	m_next_frame = m_ani_frame + 1;
	
	if( m_curt_frame >= (m_end_frame) )
	{
		m_curt_frame = m_end_frame - 1;
	}
	
	if( m_next_frame >= (m_end_frame) )
	{
		m_next_frame = 0;
	}
	
	int id = 0;
	D3DXMATRIX matTemp;
	D3DXQUATERNION qtRot;	

	for( int j = 0; j < m_pModel->m_header.m_num_vertex; ++j )
	{		
		id = m_pModel->m_pNodeID[j].ID;

		if( m_pBone->m_num_node <= id )
		{
			return false;
		}
		
		if( NULL == m_pBone->m_pNode[id].m_pAnimation )
		{
			return false;
		}

		D3DXQuaternionSlerp( &qtRot, 
								&m_pBone->m_pNode[id].m_pAnimation[m_curt_frame].m_qtRot,
								&m_pBone->m_pNode[id].m_pAnimation[m_next_frame].m_qtRot,
								m_middle_time );		
			
		// 쿼터니온 -> 메트릭스
		D3DXMatrixRotationQuaternion( &matTemp, &qtRot );
							
		//-----------------------------------------------------------------------------
		// 위치값
		//-----------------------------------------------------------------------------
		matTemp._41 = (( m_pBone->m_pNode[id].m_pAnimation[m_next_frame].m_vPos.x -
						 m_pBone->m_pNode[id].m_pAnimation[m_curt_frame].m_vPos.x ) * m_middle_time ) +
						 m_pBone->m_pNode[id].m_pAnimation[m_curt_frame].m_vPos.x;
		
		matTemp._42 = (( m_pBone->m_pNode[id].m_pAnimation[m_next_frame].m_vPos.y -
						 m_pBone->m_pNode[id].m_pAnimation[m_curt_frame].m_vPos.y ) * m_middle_time ) +
						 m_pBone->m_pNode[id].m_pAnimation[m_curt_frame].m_vPos.y;
		
		matTemp._43 = (( m_pBone->m_pNode[id].m_pAnimation[m_next_frame].m_vPos.z -
						 m_pBone->m_pNode[id].m_pAnimation[m_curt_frame].m_vPos.z ) * m_middle_time ) +
						 m_pBone->m_pNode[id].m_pAnimation[m_curt_frame].m_vPos.z;

		
		//-----------------------------------------------------------------------------
		// 버텍스 x 메트릭스 = 애니 버텍스
		//-----------------------------------------------------------------------------
		D3DXVec3TransformCoord( &m_VtxBuffer.pVertex[j], &m_pModel->m_pVertex[j].m_vertex, &matTemp );
	}	
	
	// 버퍼에넣기					
	OBJECTVERTEX*	pOBJVERTEX = NULL;
#ifdef DIRECT_VERSION_9_MJH
	if( FAILED( m_VtxBuffer.NormalVB->Lock( 0, 0, (void**)&pOBJVERTEX, 0 ) ) )
#else
	if( FAILED( m_VtxBuffer.NormalVB->Lock( 0, 0, (BYTE**)&pOBJVERTEX, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH		
	{
		return FALSE;
	}
			
	for(unsigned int j = 0; j < m_pModel->m_header.m_num_vertex; ++j )
	{
		memcpy( pOBJVERTEX[j].Vertex, m_VtxBuffer.pVertex[j], sizeof(m_VtxBuffer.pVertex[j]) );						
	}

	m_VtxBuffer.NormalVB->Unlock();

	return TRUE;
}