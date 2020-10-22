#include "CEFBonWarSkill.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include	<SOUND\Sound.H>
#include "..\\..\RockClient.h"
#include "..\\..\\map.h"
#include "..\\..\\CRenderManager.h"

extern long		 g_nowTime;

CEFBonWarSkill::CEFBonWarSkill()
{
	m_IsConEffect = TRUE;
	m_ISDamageProcess = FALSE;
}

CEFBonWarSkill::~CEFBonWarSkill()
{

}

HRESULT	CEFBonWarSkill::InvalidateDeviceObject()
{
	m_SwordTail.InvalidateDeviceObject();

	return S_OK;
}

#ifdef DIRECT_VERSION_9_MJH
HRESULT	CEFBonWarSkill::RestoreDeviceObject (LPDIRECT3DDEVICE9 pDevice)
#else
HRESULT	CEFBonWarSkill::RestoreDeviceObject (LPDIRECT3DDEVICE8 pDevice)
#endif // DIRECT_VERSION_9_MJH
{

	m_SwordTail.InitDevice( g_lpDevice );	

	CStringToCode	HashString( "afterimage3.tga" );
	m_SwordTail.m_HashCode = HashString.GetHashCode();

	return S_OK;
}

int	CEFBonWarSkill::CheckFrame(void)
{
	if(m_State == ES_SELF_DESTROY)
	{
		return TRUE;
	}

	if( m_Src_Ani_Index != m_pSrcChr->m_ani_index )
	{
		SetAfterEffect();
		SetDestroy();
		return TRUE;
	}
	
	switch(m_ID)
	{
		case EF_B_M_SOLDIER_THRUST:
			{
				return CheckFrameBonSTrust();
			}
			break;
		case EF_B_M_SOLDIER_WEAK_CUT:
			{
				return CheckFrameBonSWCut();
			}
			break;
		case EF_B_M_SOLDIER_THRUST_CUT:
			{
				return CheckFrameBonSTCut();
			}
			break;
		case EF_B_M_WARRIOR_TURN_CUT:
			{
				return CheckFrameBonSTurnCut();
			}
			break;
		case EF_B_M_WARRIOR_X1_CUT:
			{
				return  CheckFrameBoneWX1Cut();
			}
			break;
		case EF_B_M_WARRIOR_X2_CUT:
			{
				return  CheckFrameBoneWX2Cut();
			}
			break;
		case EF_B_M_BOSS_FIRE_STORM:
			{
				return CheckFrameBoneBFStorm();
			}
			break;
		case EF_B_JAHE_CRAZY_FEN_BEFORE:
			{
				return CheckFrameBonJCraFen();
			}
			break;
		case EF_B_SKEL_BAIN_DARK_STORM:
			{
				return CheckFrameSkelDStorm();
			}
			break;
	}	
	
	return TRUE;
}

void CEFBonWarSkill::Init()
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr ) )
	{
		SetDestroy(TRUE);
		return;
	}	
	
	m_Src_Ani_Index = m_pSrcChr->m_ani_index;

	//................................................................................	
	// 발동 사운드
	//................................................................................	
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( m_pExtraInfo->SkillCode , m_pSrcChr );	
	
	switch(m_ID)
	{
		case EF_B_M_SOLDIER_THRUST:
			{
				InitBonSTrust();
			}
			break;
		case EF_B_M_SOLDIER_WEAK_CUT:
			{
				InitBonSWCut();
			}
			break;
		case EF_B_M_SOLDIER_THRUST_CUT:
			{
				InitBonSTCut();
			}
			break;
		case EF_B_M_WARRIOR_TURN_CUT:
			{
				InitBonSTurnCut();
			}
			break;
		case EF_B_M_WARRIOR_X1_CUT:
			{
				InitBonWX1Cut();
			}
			break;
		case EF_B_M_WARRIOR_X2_CUT:
			{
				InitBonWX2Cut();
			}
			break;
		case EF_B_M_BOSS_FIRE_STORM:
			{
				InitBonBFStorm();
			}
			break;
		case EF_B_JAHE_CRAZY_FEN_BEFORE:
			{
				 InitBonJCraFen();
			}
			break;
		case EF_B_SKEL_BAIN_DARK_STORM:
			{
				 InitSkelDStorm();
			}
			break;
	}
}

int CEFBonWarSkill::Update(float dtime)
{
	switch(m_ID)
	{
		case EF_B_M_SOLDIER_THRUST:
			{
				return UpdateBonSTrust(dtime);
			}
			break;
		case EF_B_M_SOLDIER_WEAK_CUT:
			{
				return UpdateBonSWCut(dtime);
			}
			break;
		case EF_B_M_SOLDIER_THRUST_CUT:
			{
				return UpdateBonSTCut(dtime);
			}
			break;
		case EF_B_M_WARRIOR_TURN_CUT:
			{
				return UpdateBonSTurnCut(dtime);
			}
			break;
		case EF_B_M_WARRIOR_X1_CUT:
			{
				return UpdateBonWX1Cut(dtime);
			}
			break;
		case EF_B_M_WARRIOR_X2_CUT:
			{
				return UpdateBonWX2Cut(dtime);
			}
			break;
		case EF_B_M_BOSS_FIRE_STORM:
			{
				return UpdateBonBFStorm(dtime);
			}
			break;
		case EF_B_JAHE_CRAZY_FEN_BEFORE:
			{
				return UpdateBonJCraFen(dtime);
			}
			break;
		case EF_B_SKEL_BAIN_DARK_STORM:
			{
				return UpdateSkelDStorm(dtime);
			}
			break;
	}	
	
	return TRUE;
}

int	CEFBonWarSkill::Render(float dtime)
{
	switch(m_ID)
	{
		case EF_B_M_SOLDIER_THRUST:
			{
				return RenderBonSTrust(dtime);
			}
			break;
		case EF_B_M_SOLDIER_WEAK_CUT:
			{
				return RenderBonSWCut(dtime);
			}
			break;
		case EF_B_M_SOLDIER_THRUST_CUT:
			{
				return RenderBonSTCut(dtime);
			}
			break;
		case EF_B_M_WARRIOR_TURN_CUT:
			{
				return RenderBonSTurnCut(dtime);
			}
			break;
		case EF_B_M_WARRIOR_X1_CUT:
			{
				return RenderBonWX1Cut(dtime);
			}
			break;
		case EF_B_M_WARRIOR_X2_CUT:
			{
				return RenderBonWX2Cut(dtime);
			}
			break;	
		case EF_B_M_BOSS_FIRE_STORM:
			{
				return RenderBonBFStorm(dtime);
			}
			break;
		case EF_B_JAHE_CRAZY_FEN_BEFORE:
			{
				return RenderBonJCraFen(dtime);
			}
			break;
		case EF_B_SKEL_BAIN_DARK_STORM:
			{
				return RenderSkelDStorm(dtime);
			}
			break;
	}	
	
	return TRUE;	
}

int CEFBonWarSkill::SetAfterEffect()
{
	switch(m_ID)
	{
		case EF_B_M_SOLDIER_THRUST:
			{
				return SetAEffectBonSTrust();
			}
			break;
		case EF_B_M_SOLDIER_WEAK_CUT:
			{
				return SetAEffectBonSWCut();
			}
			break;
		case EF_B_M_SOLDIER_THRUST_CUT:
			{
				return SetAEffectBonSTCut();
			}
			break;
		case EF_B_M_WARRIOR_TURN_CUT:
			{
				return SetAEffectBonSTurnCut();
			}
			break;
		case EF_B_M_WARRIOR_X1_CUT:
			{
				return SetAEffectBonWX1Cut();
			}
			break;
		case EF_B_M_WARRIOR_X2_CUT:
			{
				return SetAEffectBonWX2Cut();
			}
			break;
		case EF_B_M_BOSS_FIRE_STORM:
			{
				return SetAEffectBonBFStorm();
			}
			break;
		case EF_B_JAHE_CRAZY_FEN_BEFORE:
			{
				return SetAEffectBonJCraFen();
			}
			break;
		case EF_B_SKEL_BAIN_DARK_STORM:
			{
				return SetAEffectSkelDStorm();
			}
			break;
	}	

	return TRUE;
}

void CEFBonWarSkill::InitBonSTrust()
{
	CBillboardData *pData = NULL ;
	
	///-- 칼끝에 붙는 빌보드 데이타 가져옴 
	if( NULL == ( pData = g_Billboard.m_BillboardData.GetKeyDataPointer( 24523 ) ) )
	{
		SetDestroy(TRUE);
		return;
	}

	///-- 빌보드 렌더러에 링크
	m_LinkBillboard.MakeLink( pData );
	m_LinkBillboard.m_nDirectEffType = ESLT_CHARWHOLELINK;
	m_nPrevFrame = m_pSrcChr->m_ani_frame;

	m_SwordTail.InitDevice( g_lpDevice );
}
	
int	CEFBonWarSkill::CheckFrameBonSTrust()
{
	if( m_pSrcChr->m_ani_frame == 62 )
	{
		m_pSrcChr->m_IsApplyVA = TRUE;
		m_pSrcChr->m_IsNextApplyVA = TRUE;
	}

	m_pSrcChr->BoneRender();

	if( m_pSrcChr->m_ani_frame == 62 )
	{
		SetAfterEffect();		
		return TRUE;
	}	

	if(m_pSrcChr->m_ani_frame + c_MotionBlendHalfFrame > (int)m_pSrcChr->m_end_frame)
	{
		SetDestroy();
		return TRUE;
	}	
	
	return TRUE;
}

int	CEFBonWarSkill::UpdateBonSTrust(float dtime)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr, m_pDestChr ) )
	{
		SetDestroy(TRUE);
		return TRUE;
	}
	
	BOOL IsUpdateEffeectCon = FALSE;

	if( m_pSrcChr->m_ani_frame <= 75 )	
	{
		IsUpdateEffeectCon = TRUE;
	}	
	
	if(IsUpdateEffeectCon)
	{
		int nNowFrame = m_pSrcChr->m_ani_frame;
		float fDelTime = max( 0.0f, ( nNowFrame - m_nPrevFrame ) * 0.03f );
		m_nPrevFrame = nNowFrame;

		D3DXMATRIX  matTemp;
		D3DXVECTOR3 vTemp1, vTemp2, vStart, vDir;

		m_pSrcChr->GetDirectBoneLinkTM( 16 , matTemp );
		vTemp1.x = matTemp._41;
		vTemp1.y = matTemp._42;
		vTemp1.z = matTemp._43;	

		m_pSrcChr->GetDirectBoneLinkTM( 18 , matTemp );
		vTemp2.x = matTemp._41;
		vTemp2.y = matTemp._42;
		vTemp2.z = matTemp._43;
		
		vStart = ( vTemp1 + vTemp2 ) / 2.0f;
		vDir = vTemp1 - vTemp2;
		D3DXVec3Normalize( &vDir , &vDir );
		vStart += vDir * 15.0f * ( m_LinkBillboard.m_fCurLife );	

		matTemp._41 = vStart.x;
		matTemp._42 = vStart.y;
		matTemp._43 = vStart.z;

		m_LinkBillboard.m_mLinkBoneMatrix = matTemp;
		m_LinkBillboard.UpdateBillboardNoneSet(fDelTime);
	}

	return TRUE;
}

int	CEFBonWarSkill::RenderBonSTrust(float dtime)
{
	BOOL IsRenderEffeectCon = FALSE;

	if( (30 <= m_pSrcChr->m_ani_frame) && (m_pSrcChr->m_ani_frame <= 75) )	
	{
		IsRenderEffeectCon = TRUE;
	}	
	
	D3DXMATRIX matLocal;	

	D3DXMatrixIdentity( &matLocal );

	g_RenderManager.SetTransform( D3DTS_WORLD , &matLocal );

	g_RenderManager.SetCullMode( D3DCULL_NONE );
	
	g_RenderManager.SetZWriteEnable( FALSE );
	g_RenderManager.SetZEnable( D3DZB_TRUE );
	
	g_RenderManager.SetAlphaBlendEnable( TRUE );
	g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
	g_RenderManager.SetDestBlend(  D3DBLEND_DESTALPHA );
	
	g_RenderManager.SetFillMode( D3DFILL_SOLID );
	
	if( IsRenderEffeectCon )	
	{
#ifdef BATCH_RENDER
		m_LinkBillboard.DrawBatchBillboard();
#ifdef DIRECT_VERSION_9_MJH
		LPDIRECT3DTEXTURE9	prevBillboardTexture = g_RenderManager.GetPrevBillboardTexture();
#else
		LPDIRECT3DTEXTURE8	prevBillboardTexture = g_RenderManager.GetPrevBillboardTexture();
#endif // DIRECT_VERSION_9_MJH
		if(prevBillboardTexture)
		{
			g_RenderManager.SetTexture( 0, prevBillboardTexture );
			g_RenderManager.SetSrcBlend( g_RenderManager.m_PrevSrcBlend );
			g_RenderManager.SetDestBlend( g_RenderManager.m_PrevDestBlend );
			g_RenderManager.EndBatchPrimitive();
			g_RenderManager.SetBeginBatchRendering(FALSE);
	}
#else	//	BATCH_RENDER
		m_LinkBillboard.DrawBillboard();
#endif	//	BATCH_RENDER
		
	}

	g_RenderManager.SetCullMode( D3DCULL_CCW );
	
	g_RenderManager.SetZWriteEnable( TRUE );
	g_RenderManager.SetZEnable( TRUE );
	
	g_RenderManager.SetAlphaBlendEnable( FALSE );

	return TRUE;
}

int CEFBonWarSkill::SetAEffectBonSTrust()
{
	//................................................................................	
	// 어택 사운드
	//................................................................................	
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillAttackedSound( m_pExtraInfo->SkillCode , m_pDestChr );

	g_Particle.SetEffSetCon( 0.0f , EF_STAND_SWORD_ATTACKED2 , ESLT_CHARPOS , 
							 6.1f , m_pDestChr->GetUnique() , m_pDestChr->GetIndex(), 
							 ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr) );

	if(m_pExtraInfo)
	{
		m_pExtraInfo->AfterEffect = EF_STAND_SWORD_ATTACKED2;
		m_pExtraInfo->DestType = ESLT_CHARPOS;
		m_pExtraInfo->lifeTime = 6.1f;
	}
	
	g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , m_pDestChr , m_pExtraInfo );

	return TRUE;
}

void CEFBonWarSkill::InitBonSWCut()
{
	m_SwordTail.InitDevice( g_lpDevice );
	CStringToCode	HashString( "afterimage3.tga" );
	m_SwordTail.m_HashCode = HashString.GetHashCode();
}

int	CEFBonWarSkill::CheckFrameBonSWCut()
{
	if( m_pSrcChr->m_ani_frame == 45 )
	{
		m_pSrcChr->m_IsApplyVA = TRUE;
		m_pSrcChr->m_IsNextApplyVA = TRUE;
	}

	m_pSrcChr->BoneRender();

	if( m_pSrcChr->m_ani_frame == 45 )
	{
		if( !m_ISDamageProcess )
		{
			SetAfterEffect();
			m_ISDamageProcess = TRUE;
		}
	}

	if(m_pSrcChr->m_ani_frame + c_MotionBlendHalfFrame > (int)m_pSrcChr->m_end_frame)
	{
		SetDestroy();
		return TRUE;
	}
	
	return TRUE;
}

int	CEFBonWarSkill::UpdateBonSWCut(float dtime)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr, m_pDestChr ) )
	{
		SetDestroy(TRUE);
		return TRUE;
	}	

	D3DXMATRIX   matTemp;
	D3DXVECTOR3  vHead;
	D3DXVECTOR3  vCenter;

	m_pSrcChr->GetDirectBoneLinkTM( 16 , matTemp );	
	
	vHead.x = matTemp._41;
	vHead.y = matTemp._42;
	vHead.z = matTemp._43;

	m_pSrcChr->GetDirectBoneLinkTM( 18 , matTemp );
	vCenter.x = matTemp._41;
	vCenter.y = matTemp._42;
	vCenter.z = matTemp._43;

	m_SwordTail.AddPoint( vCenter , vHead , g_lpDevice );
	
	return TRUE;
}

int	CEFBonWarSkill::RenderBonSWCut(float dtime)
{
	D3DXMATRIX matLocal;	

	D3DXMatrixIdentity( &matLocal );

	g_RenderManager.SetTransform( D3DTS_WORLD , &matLocal );

	g_RenderManager.SetCullMode( D3DCULL_NONE );
	
	g_RenderManager.SetZWriteEnable( FALSE );
	g_RenderManager.SetZEnable( D3DZB_TRUE );
	
	g_RenderManager.SetAlphaBlendEnable( TRUE );
	g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
	g_RenderManager.SetDestBlend(  D3DBLEND_DESTALPHA );
	
	g_RenderManager.SetFillMode( D3DFILL_SOLID );
	
	m_SwordTail.Render( g_lpDevice );
	
	g_RenderManager.SetCullMode( D3DCULL_CCW );
	
	g_RenderManager.SetZWriteEnable( TRUE );
	g_RenderManager.SetZEnable( TRUE );
	
	g_RenderManager.SetAlphaBlendEnable( FALSE );	
	
	return TRUE;
}

int CEFBonWarSkill::SetAEffectBonSWCut()
{
	//................................................................................	
	// 어택 사운드
	//................................................................................	
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillAttackedSound( m_pExtraInfo->SkillCode , m_pDestChr );

	g_Particle.SetEffSetCon( 0.0f , EF_B_M_SOLDIER_WEAK_CUTED , ESLT_CPOS_ICAMERA , 
							 6.1f , m_pDestChr->GetUnique() , m_pDestChr->GetIndex(), 
							 ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr) );

	if(m_pExtraInfo)
	{
		m_pExtraInfo->AfterEffect = EF_B_M_SOLDIER_WEAK_CUTED;
		m_pExtraInfo->DestType = ESLT_CPOS_ICAMERA;
		m_pExtraInfo->lifeTime = 6.1f;
	}
	
	g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , m_pDestChr , m_pExtraInfo );	
	
	return TRUE;
}

void CEFBonWarSkill::InitBonSTCut()
{
	CBillboardData *pData = NULL ;
	
	///-- 칼끝에 붙는 빌보드 데이타 가져옴 
	if( NULL == ( pData = g_Billboard.m_BillboardData.GetKeyDataPointer( 24523 ) ) )
	{
		SetDestroy(TRUE);
		return;
	}

	///-- 빌보드 렌더러에 링크
	m_LinkBillboard.MakeLink( pData );
	m_LinkBillboard.m_nDirectEffType = ESLT_CHARWHOLELINK;
	m_nPrevFrame = m_pSrcChr->m_ani_frame;	
	
	m_SwordTail.InitDevice( g_lpDevice );
	CStringToCode	HashString( "afterimage3.tga" );
	m_SwordTail.m_HashCode = HashString.GetHashCode();
}

int	CEFBonWarSkill::CheckFrameBonSTCut()
{
	if( m_pSrcChr->m_ani_frame == 62 )
	{
		m_pSrcChr->m_IsApplyVA = TRUE;
		m_pSrcChr->m_IsNextApplyVA = TRUE;
	}

	if( m_pSrcChr->m_ani_frame == 115 )
	{
		m_pSrcChr->m_IsApplyVA = TRUE;
		m_pSrcChr->m_IsNextApplyVA = TRUE;
	}

	m_pSrcChr->BoneRender();

	if( m_pSrcChr->m_ani_frame == 62 )
	{
		//................................................................................	
		// 어택 사운드
		//................................................................................	
		if( m_pExtraInfo != NULL )
			g_Particle.m_EffectManager.GetSkillAttackedSound( m_pExtraInfo->SkillCode , m_pDestChr );

		g_Particle.SetEffSetCon( 0.0f , EF_B_M_SOLDIER_THRUST_CUTED , ESLT_SRC_RAD , 
								 6.1f , m_pDestChr->GetUnique() , m_pDestChr->GetIndex(), 
								 ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr), 
								 m_pSrcChr->GetDirection() + D3DXToRadian( 180 ) );
	}

	if( m_pSrcChr->m_ani_frame == 115 )
	{
		if( !m_ISDamageProcess )
		{
			SetAfterEffect();
			m_ISDamageProcess = TRUE;
		}
	}

	if(m_pSrcChr->m_ani_frame + c_MotionBlendHalfFrame > (int)m_pSrcChr->m_end_frame)
	{
		SetDestroy();
		return TRUE;
	}	
	
	return TRUE;
}

int	CEFBonWarSkill::UpdateBonSTCut(float dtime)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr, m_pDestChr ) )
	{
		SetDestroy(TRUE);
		return TRUE;
	}	

	D3DXMATRIX   matTemp;
	D3DXVECTOR3  vHead;
	D3DXVECTOR3  vCenter;
	D3DXVECTOR3 vStart, vDir;	

	BOOL IsUpdateEffeectCon = FALSE;
	
	if( m_pSrcChr->m_ani_frame <= 75 )	
	{
		IsUpdateEffeectCon = TRUE;
	}

	m_pSrcChr->GetDirectBoneLinkTM( 16 , matTemp );	
	
	vHead.x = matTemp._41;
	vHead.y = matTemp._42;
	vHead.z = matTemp._43;

	m_pSrcChr->GetDirectBoneLinkTM( 18 , matTemp );
	vCenter.x = matTemp._41;
	vCenter.y = matTemp._42;
	vCenter.z = matTemp._43;	

	m_SwordTail.AddPoint( vCenter , vHead , g_lpDevice );
	
	if( IsUpdateEffeectCon )
	{
		int nNowFrame = m_pSrcChr->m_ani_frame;
		float fDelTime = max( 0.0f, ( nNowFrame - m_nPrevFrame ) * 0.03f );
		m_nPrevFrame = nNowFrame;
		
		vStart = ( vHead + vCenter ) / 2.0f;
		vDir = vHead - vCenter;
		D3DXVec3Normalize( &vDir , &vDir );
		vStart += vDir * 15.0f * ( m_LinkBillboard.m_fCurLife );

		matTemp._41 = vStart.x;
		matTemp._42 = vStart.y;
		matTemp._43 = vStart.z;

		m_LinkBillboard.m_mLinkBoneMatrix = matTemp;
		m_LinkBillboard.UpdateBillboardNoneSet(fDelTime);
	}
	
	return TRUE;
}

int	CEFBonWarSkill::RenderBonSTCut(float dtime)
{
	D3DXMATRIX matLocal;	

	D3DXMatrixIdentity( &matLocal );

	BOOL IsRenderEffeectCon = FALSE;

	if( (30 <= m_pSrcChr->m_ani_frame) && (m_pSrcChr->m_ani_frame <= 75) )	
	{
		IsRenderEffeectCon = TRUE;
	}

	g_RenderManager.SetTransform( D3DTS_WORLD , &matLocal );

	g_RenderManager.SetCullMode( D3DCULL_NONE );
	
	g_RenderManager.SetZWriteEnable( FALSE );
	g_RenderManager.SetZEnable( D3DZB_TRUE );
	
	g_RenderManager.SetAlphaBlendEnable( TRUE );
	g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
	g_RenderManager.SetDestBlend(  D3DBLEND_DESTALPHA );
	
	g_RenderManager.SetFillMode( D3DFILL_SOLID );
	
	m_SwordTail.Render( g_lpDevice );

	if( IsRenderEffeectCon )	
	{
#ifdef BATCH_RENDER
		m_LinkBillboard.DrawBatchBillboard();
#ifdef DIRECT_VERSION_9_MJH
		LPDIRECT3DTEXTURE9	prevBillboardTexture = g_RenderManager.GetPrevBillboardTexture();
#else
		LPDIRECT3DTEXTURE8	prevBillboardTexture = g_RenderManager.GetPrevBillboardTexture();
#endif // DIRECT_VERSION_9_MJH
		if(prevBillboardTexture)
		{
			g_RenderManager.SetTexture( 0, prevBillboardTexture );
			g_RenderManager.SetSrcBlend( g_RenderManager.m_PrevSrcBlend );
			g_RenderManager.SetDestBlend( g_RenderManager.m_PrevDestBlend );
			g_RenderManager.EndBatchPrimitive();
			g_RenderManager.SetBeginBatchRendering(FALSE);
	}
#else	//	BATCH_RENDER
		m_LinkBillboard.DrawBillboard();
#endif	//	BATCH_RENDER
	}
	
	g_RenderManager.SetCullMode( D3DCULL_CCW );
	
	g_RenderManager.SetZWriteEnable( TRUE );
	g_RenderManager.SetZEnable( TRUE );
	
	g_RenderManager.SetAlphaBlendEnable( FALSE );	
	
	return TRUE;
}

int	CEFBonWarSkill::SetAEffectBonSTCut()
{
	//................................................................................	
	// 어택 사운드
	//................................................................................	
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillAttackedSound( m_pExtraInfo->SkillCode , m_pDestChr );

	g_Particle.SetEffSetCon( 0.0f , EF_B_M_SOLDIER_THRUST_CUTED , ESLT_SRC_RAD , 
							 6.1f , m_pDestChr->GetUnique() , m_pDestChr->GetIndex(), 
							 ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr), 
							 m_pSrcChr->GetDirection() + D3DXToRadian( 180 ) );

	if(m_pExtraInfo)
	{
		m_pExtraInfo->AfterEffect = EF_B_M_SOLDIER_THRUST_CUTED;
		m_pExtraInfo->DestType = ESLT_SRC_RAD;
		m_pExtraInfo->lifeTime = 6.1f;
	}
	
	g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , m_pDestChr , m_pExtraInfo );	
	
	return TRUE;	
}


void CEFBonWarSkill::InitBonSTurnCut()
{
	m_SwordTail.InitDevice( g_lpDevice );
	CStringToCode	HashString( "afterimage3.tga" );
	m_SwordTail.m_HashCode = HashString.GetHashCode();
}

int	CEFBonWarSkill::CheckFrameBonSTurnCut()
{
	if( m_pSrcChr->m_ani_frame == 35 )
	{
		m_pSrcChr->m_IsApplyVA = TRUE;
		m_pSrcChr->m_IsNextApplyVA = TRUE;
	}

	m_pSrcChr->BoneRender();

	if( m_pSrcChr->m_ani_frame == 35 )
	{
		if( !m_ISDamageProcess )
		{
			SetAfterEffect();
			m_ISDamageProcess = TRUE;
		}
	}

	if(m_pSrcChr->m_ani_frame + c_MotionBlendHalfFrame > (int)m_pSrcChr->m_end_frame)
	{
		SetDestroy();
		return TRUE;
	}	
	
	return TRUE;
}	

int	CEFBonWarSkill::UpdateBonSTurnCut(float dtime)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr, m_pDestChr ) )
	{
		SetDestroy(TRUE);
		return TRUE;
	}	

	D3DXMATRIX   matTemp;
	D3DXVECTOR3  vHead;
	D3DXVECTOR3  vCenter;

	m_pSrcChr->GetDirectBoneLinkTM( 16 , matTemp );	
	
	vHead.x = matTemp._41;
	vHead.y = matTemp._42;
	vHead.z = matTemp._43;

	m_pSrcChr->GetDirectBoneLinkTM( 18 , matTemp );
	vCenter.x = matTemp._41;
	vCenter.y = matTemp._42;
	vCenter.z = matTemp._43;

	m_SwordTail.AddPoint( vCenter , vHead , g_lpDevice );		
	
	return TRUE;
}

int	CEFBonWarSkill::RenderBonSTurnCut(float dtime)
{

	D3DXMATRIX matLocal;	
	
	D3DXMatrixIdentity( &matLocal );

	g_RenderManager.SetTransform( D3DTS_WORLD , &matLocal );

	g_RenderManager.SetCullMode( D3DCULL_NONE );
	
	g_RenderManager.SetZWriteEnable( FALSE );
	g_RenderManager.SetZEnable( D3DZB_TRUE );
	
	g_RenderManager.SetAlphaBlendEnable( TRUE );
	g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
	g_RenderManager.SetDestBlend(  D3DBLEND_DESTALPHA );
	
	g_RenderManager.SetFillMode( D3DFILL_SOLID );
	
	m_SwordTail.Render( g_lpDevice );
	
	g_RenderManager.SetCullMode( D3DCULL_CCW );
	
	g_RenderManager.SetZWriteEnable( TRUE );
	g_RenderManager.SetZEnable( TRUE );
	
	g_RenderManager.SetAlphaBlendEnable( FALSE );	
	
	return TRUE;
}

int	CEFBonWarSkill::SetAEffectBonSTurnCut()
{
	//................................................................................	
	// 어택 사운드
	//................................................................................	
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillAttackedSound( m_pExtraInfo->SkillCode , m_pDestChr );

	g_Particle.SetEffSetCon( 0.0f , EF_B_M_WAR_TURN_FIRST_QUARTER , ESLT_CHARPOS , 
							 6.1f , m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex(), 
							 D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr->GetDirection() );

	g_Particle.SetEffSetCon( 0.0f , EF_B_M_WARRIOR_TURN_CUTED , ESLT_CHARPOS , 
							 6.1f , m_pDestChr->GetUnique() , m_pDestChr->GetIndex(), 
							 ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr) );

	if(m_pExtraInfo)
	{
		m_pExtraInfo->AfterEffect = EF_B_M_WARRIOR_TURN_CUTED;
		m_pExtraInfo->DestType = ESLT_CHARPOS;
		m_pExtraInfo->lifeTime = 6.1f;
	}
	
	g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , m_pDestChr , m_pExtraInfo );	
	
	return TRUE;
}

void CEFBonWarSkill::InitBonWX1Cut()
{
	m_SwordTail.InitDevice( g_lpDevice );
	CStringToCode	HashString( "afterimage3.tga" );
	m_SwordTail.m_HashCode = HashString.GetHashCode();		
}

int	CEFBonWarSkill::CheckFrameBoneWX1Cut()
{
	if( m_pSrcChr->m_ani_frame == 27 )
	{
		m_pSrcChr->m_IsApplyVA = TRUE;
		m_pSrcChr->m_IsNextApplyVA = TRUE;
	}
	

	if( m_pSrcChr->m_ani_frame == 61 )
	{
		m_pSrcChr->m_IsApplyVA = TRUE;
		m_pSrcChr->m_IsNextApplyVA = TRUE;
	}

	m_pSrcChr->BoneRender();

	if( m_pSrcChr->m_ani_frame == 27 )
	{
		//................................................................................	
		// 어택 사운드
		//................................................................................	
		if( m_pExtraInfo != NULL )
			g_Particle.m_EffectManager.GetSkillAttackedSound( m_pExtraInfo->SkillCode , m_pDestChr );

		g_Particle.SetEffSetCon( 0.0f , EF_B_M_WARRIOR_X1_CUTED , ESLT_CHARPOS , 
							 10.1f , m_pDestChr->GetUnique() , m_pDestChr->GetIndex(), 
							 ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr) );
	}

	if( m_pSrcChr->m_ani_frame == 61 )
	{
		if( !m_ISDamageProcess )
		{
			SetAfterEffect();
			m_ISDamageProcess = TRUE;
		}
	}

	if(m_pSrcChr->m_ani_frame + c_MotionBlendHalfFrame > (int)m_pSrcChr->m_end_frame)
	{
		SetDestroy();
		return TRUE;
	}			
	
	return TRUE;
}

int CEFBonWarSkill::UpdateBonWX1Cut(float dtime)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr, m_pDestChr ) )
	{
		SetDestroy(TRUE);
		return TRUE;
	}	

	D3DXMATRIX   matTemp;
	D3DXVECTOR3  vHead;
	D3DXVECTOR3  vCenter;

	m_pSrcChr->GetDirectBoneLinkTM( 16 , matTemp );	
	
	vHead.x = matTemp._41;
	vHead.y = matTemp._42;
	vHead.z = matTemp._43;

	m_pSrcChr->GetDirectBoneLinkTM( 18 , matTemp );
	vCenter.x = matTemp._41;
	vCenter.y = matTemp._42;
	vCenter.z = matTemp._43;

	m_SwordTail.AddPoint( vCenter , vHead , g_lpDevice );	
	
	return TRUE;
}

int CEFBonWarSkill::RenderBonWX1Cut(float dtime)
{
	D3DXMATRIX matLocal;	
	
	D3DXMatrixIdentity( &matLocal );

	g_RenderManager.SetTransform( D3DTS_WORLD , &matLocal );

	g_RenderManager.SetCullMode( D3DCULL_NONE );
	
	g_RenderManager.SetZWriteEnable( FALSE );
	g_RenderManager.SetZEnable( D3DZB_TRUE );
	
	g_RenderManager.SetAlphaBlendEnable( TRUE );
	g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
	g_RenderManager.SetDestBlend(  D3DBLEND_DESTALPHA );
	
	g_RenderManager.SetFillMode( D3DFILL_SOLID );
	
	m_SwordTail.Render( g_lpDevice );
	
	g_RenderManager.SetCullMode( D3DCULL_CCW );
	
	g_RenderManager.SetZWriteEnable( TRUE );
	g_RenderManager.SetZEnable( TRUE );
	
	g_RenderManager.SetAlphaBlendEnable( FALSE );
	
	return TRUE;
}

int CEFBonWarSkill::SetAEffectBonWX1Cut()
{
	//................................................................................	
	// 어택 사운드
	//................................................................................	
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillAttackedSound( m_pExtraInfo->SkillCode , m_pDestChr );

	g_Particle.SetEffSetCon( 0.0f , EF_B_M_WARRIOR_X1_CUTED2 , ESLT_CHARPOS , 
							 8.1f , m_pDestChr->GetUnique() , m_pDestChr->GetIndex(), 
							 ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr) );

	if(m_pExtraInfo)
	{
		m_pExtraInfo->AfterEffect = EF_B_M_WARRIOR_X1_CUTED2;
		m_pExtraInfo->DestType = ESLT_CHARPOS;
		m_pExtraInfo->lifeTime = 8.1f;
	}
	
	g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , m_pDestChr , m_pExtraInfo );	
	
	return TRUE;
}

void CEFBonWarSkill::InitBonWX2Cut()
{
	m_SwordTail.InitDevice( g_lpDevice );
	CStringToCode	HashString( "afterimage3.tga" );
	m_SwordTail.m_HashCode = HashString.GetHashCode();		
}

int	CEFBonWarSkill::CheckFrameBoneWX2Cut()
{
	if( m_pSrcChr->m_ani_frame == 27 )
	{
		m_pSrcChr->m_IsApplyVA = TRUE;
		m_pSrcChr->m_IsNextApplyVA = TRUE;
	}
	

	if( m_pSrcChr->m_ani_frame == 61 )
	{
		m_pSrcChr->m_IsApplyVA = TRUE;
		m_pSrcChr->m_IsNextApplyVA = TRUE;
	}

	m_pSrcChr->BoneRender();

	if( m_pSrcChr->m_ani_frame == 27 )
	{
		//................................................................................	
		// 어택 사운드
		//................................................................................	
		if( m_pExtraInfo != NULL )
			g_Particle.m_EffectManager.GetSkillAttackedSound( m_pExtraInfo->SkillCode , m_pDestChr );

		g_Particle.SetEffSetCon( 0.0f , EF_B_M_WARRIOR_X2_CUTED , ESLT_CHARPOS , 
							 10.1f , m_pDestChr->GetUnique() , m_pDestChr->GetIndex(), 
							 ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr) );
	}

	if( m_pSrcChr->m_ani_frame == 61 )
	{
		if( !m_ISDamageProcess )
		{
			SetAfterEffect();
			m_ISDamageProcess = TRUE;
		}
	}

	if(m_pSrcChr->m_ani_frame + c_MotionBlendHalfFrame > (int)m_pSrcChr->m_end_frame)
	{
		SetDestroy();
		return TRUE;
	}
	
	return TRUE;
}

int CEFBonWarSkill::UpdateBonWX2Cut(float dtime)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr, m_pDestChr ) )
	{
		SetDestroy(TRUE);
		return TRUE;
	}	

	D3DXMATRIX   matTemp;
	D3DXVECTOR3  vHead;
	D3DXVECTOR3  vCenter;

	m_pSrcChr->GetDirectBoneLinkTM( 16 , matTemp );	
	
	vHead.x = matTemp._41;
	vHead.y = matTemp._42;
	vHead.z = matTemp._43;

	m_pSrcChr->GetDirectBoneLinkTM( 18 , matTemp );
	vCenter.x = matTemp._41;
	vCenter.y = matTemp._42;
	vCenter.z = matTemp._43;

	m_SwordTail.AddPoint( vCenter , vHead , g_lpDevice );	
	
	return TRUE;
}

int CEFBonWarSkill::RenderBonWX2Cut(float dtime)
{
	D3DXMATRIX matLocal;	
	
	D3DXMatrixIdentity( &matLocal );

	g_RenderManager.SetTransform( D3DTS_WORLD , &matLocal );

	g_RenderManager.SetCullMode( D3DCULL_NONE );
	
	g_RenderManager.SetZWriteEnable( FALSE );
	g_RenderManager.SetZEnable( D3DZB_TRUE );
	
	g_RenderManager.SetAlphaBlendEnable( TRUE );
	g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
	g_RenderManager.SetDestBlend(  D3DBLEND_DESTALPHA );
	
	g_RenderManager.SetFillMode( D3DFILL_SOLID );
	
	m_SwordTail.Render( g_lpDevice );
	
	g_RenderManager.SetCullMode( D3DCULL_CCW );
	
	g_RenderManager.SetZWriteEnable( TRUE );
	g_RenderManager.SetZEnable( TRUE );
	
	g_RenderManager.SetAlphaBlendEnable( FALSE );	
	
	return TRUE;
}

int CEFBonWarSkill::SetAEffectBonWX2Cut()
{
	//................................................................................	
	// 어택 사운드
	//................................................................................	
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillAttackedSound( m_pExtraInfo->SkillCode , m_pDestChr );

	g_Particle.SetEffSetCon( 0.0f , EF_B_M_WARRIOR_X2_CUTED2 , ESLT_CHARPOS , 
							 8.1f , m_pDestChr->GetUnique() , m_pDestChr->GetIndex(), 
							 ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr) );

	if(m_pExtraInfo)
	{
		m_pExtraInfo->AfterEffect = EF_B_M_WARRIOR_X2_CUTED2;
		m_pExtraInfo->DestType = ESLT_CHARPOS;
		m_pExtraInfo->lifeTime = 8.1f;
	}
	
	g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , m_pDestChr , m_pExtraInfo );	
	
	return TRUE;
}

void CEFBonWarSkill::InitBonBFStorm()
{
	m_SwordTail.InitDevice( g_lpDevice );
	CStringToCode	HashString( "afterimage3.tga" );
	m_SwordTail.m_HashCode = HashString.GetHashCode();	
	
	///-- 이곳에 파이어 스톰 이펙트를 붙일수있을듯....
	g_Particle.SetEffSetCon( 1.0f , EF_B_M_BOSS_FIRE_STORM , ESLT_CHARPOS , 10.1f , 
		m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex() );
}	
	
int	CEFBonWarSkill::CheckFrameBoneBFStorm()
{
	if( m_pSrcChr->m_ani_frame == 45 )
	{
		m_pSrcChr->m_IsApplyVA = TRUE;
		m_pSrcChr->m_IsNextApplyVA = TRUE;
	}

	m_pSrcChr->BoneRender();

	if( m_pSrcChr->m_ani_frame == 45 )
	{
		if( !m_ISDamageProcess )
		{
			SetAfterEffect();
			m_ISDamageProcess = TRUE;
		}
	}

	if(m_pSrcChr->m_ani_frame + c_MotionBlendHalfFrame > (int)m_pSrcChr->m_end_frame)
	{
		SetDestroy();
		return TRUE;
	}
	
	return TRUE;
}
	
int CEFBonWarSkill::UpdateBonBFStorm(float dtime)
{
	///-- 공격 주체가 사라졌을때
	Character			*pTempDest = NULL;
	EFFECT_EXTRA_INFO    Info;
	
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr ) )
	{
		if(!m_ISDamageProcess)
		{
			SetDestroy(TRUE);		
			m_ISDamageProcess = TRUE;
		}
		else
		{
			SetDestroy();
		}

		return TRUE;
	}

	D3DXMATRIX   matTemp;
	D3DXVECTOR3  vHead;
	D3DXVECTOR3  vCenter;

	m_pSrcChr->GetDirectBoneLinkTM( 16 , matTemp );	
	
	vHead.x = matTemp._41;
	vHead.y = matTemp._42;
	vHead.z = matTemp._43;

	m_pSrcChr->GetDirectBoneLinkTM( 18 , matTemp );
	vCenter.x = matTemp._41;
	vCenter.y = matTemp._42;
	vCenter.z = matTemp._43;

	m_SwordTail.AddPoint( vCenter , vHead , g_lpDevice );		
	
	return TRUE;
}

int CEFBonWarSkill::RenderBonBFStorm(float dtime)
{
	D3DXMATRIX matLocal;	
	
	D3DXMatrixIdentity( &matLocal );

	g_RenderManager.SetTransform( D3DTS_WORLD , &matLocal );

	g_RenderManager.SetCullMode( D3DCULL_NONE );
	
	g_RenderManager.SetZWriteEnable( FALSE );
	g_RenderManager.SetZEnable( D3DZB_TRUE );
	
	g_RenderManager.SetAlphaBlendEnable( TRUE );
	g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
	g_RenderManager.SetDestBlend(  D3DBLEND_DESTALPHA );
	
	g_RenderManager.SetFillMode( D3DFILL_SOLID );
	
	m_SwordTail.Render( g_lpDevice );
	
	g_RenderManager.SetCullMode( D3DCULL_CCW );
	
	g_RenderManager.SetZWriteEnable( TRUE );
	g_RenderManager.SetZEnable( TRUE );
	
	g_RenderManager.SetAlphaBlendEnable( FALSE );		
	
	return TRUE;
}

int CEFBonWarSkill::SetAEffectBonBFStorm()
{
	Character			*pTempDest = NULL;
	EFFECT_EXTRA_INFO    Info;

	if( m_pExtraInfo != NULL )
	{
		// 1:N 데미지
		int Count = m_pExtraInfo->GetTargetCount();			
		
		for( int i = 0 ; i < Count ; ++i )
		{
			if( m_pExtraInfo->GetTargetInfo( &pTempDest , &Info , i ) )
			{
				//................................................................................	
				// 어택 사운드
				//................................................................................	
				if( m_pExtraInfo != NULL )
					g_Particle.m_EffectManager.GetSkillAttackedSound( m_pExtraInfo->SkillCode , pTempDest );				
				

				g_Particle.SetEffSetCon( 0.0f , EF_FRAME_STORMED , ESLT_CHARPOS , 2.1f , pTempDest->GetUnique() , 
										 pTempDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 
										 m_pSrcChr) );
					
				Info.AfterEffect = EF_FRAME_STORMED;
				Info.DestType = ESLT_CHARPOS;
				Info.lifeTime = 5.1f;
					
				g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , pTempDest , &Info );		
			}

			Info.Clear();
		}
	}	
	
	return TRUE;	
}

void CEFBonWarSkill::InitBonJCraFen()
{
	m_SwordTail.InitDevice( g_lpDevice );
	CStringToCode	HashString( "afterimage3.tga" );
	m_SwordTail.m_HashCode = HashString.GetHashCode();
}	

int	CEFBonWarSkill::CheckFrameBonJCraFen()
{
	if( m_pSrcChr->m_ani_frame == 30 )
	{
		m_pSrcChr->m_IsApplyVA = TRUE;
		m_pSrcChr->m_IsNextApplyVA = TRUE;

		g_Particle.m_aDirectLinkBone = 18;
		g_Particle.SetEffSetCon( 0.0f , EF_B_JAHE_CRAZY_FEN_BEFORE, ESLT_CHARPOS , 
							 EFFECT_SET_LOOP_TIME , m_pSrcChr->GetUnique() , 
							 m_pSrcChr->GetIndex() );
	}	
	
	if( m_pSrcChr->m_ani_frame == 60 )
	{
		m_pSrcChr->m_IsApplyVA = TRUE;
		m_pSrcChr->m_IsNextApplyVA = TRUE;
		g_Particle.DelEffSetCon( EF_B_JAHE_CRAZY_FEN_BEFORE , ESLT_CHARPOS , 
								 m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex(), TRUE );	
	}

	m_pSrcChr->BoneRender();

	if( m_pSrcChr->m_ani_frame == 60 )
	{
		D3DXMATRIX  mat;		
		m_pSrcChr->GetDirectBoneLinkTM( 18 , mat );
		D3DXVECTOR3 vPos = D3DXVECTOR3(mat._41, mat._42, mat._43);
		
		g_Particle.SetEffSetCon( 0.0f , EF_B_JAHE_CRAZY_FEN_AFTER , ESLT_POS , 6.1f , 
								-1 , -1 , vPos,	m_pSrcChr->GetDirection() );
		
		if( !m_ISDamageProcess )
		{
			SetAfterEffect();
			m_ISDamageProcess = TRUE;
		}
	}

	if(m_pSrcChr->m_ani_frame + c_MotionBlendHalfFrame > (int)m_pSrcChr->m_end_frame)
	{
		SetDestroy();
		return TRUE;
	}	
	
	return TRUE;
}

int	CEFBonWarSkill::UpdateBonJCraFen(float dtime)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr, m_pDestChr ) )
	{
		SetDestroy(TRUE);
		return TRUE;
	}	

	D3DXMATRIX   matTemp;
	D3DXVECTOR3  vHead;
	D3DXVECTOR3  vCenter;

	m_pSrcChr->GetDirectBoneLinkTM( 16 , matTemp );	
	
	vHead.x = matTemp._41;
	vHead.y = matTemp._42;
	vHead.z = matTemp._43;

	m_pSrcChr->GetDirectBoneLinkTM( 18 , matTemp );
	vCenter.x = matTemp._41;
	vCenter.y = matTemp._42;
	vCenter.z = matTemp._43;	

	m_SwordTail.AddPoint( vCenter , vHead , g_lpDevice );
	
	return TRUE;
}

int	CEFBonWarSkill::RenderBonJCraFen(float dtime)
{
	D3DXMATRIX matLocal;	

	D3DXMatrixIdentity( &matLocal );

	g_RenderManager.SetTransform( D3DTS_WORLD , &matLocal );

	g_RenderManager.SetCullMode( D3DCULL_NONE );
	
	g_RenderManager.SetZWriteEnable( FALSE );
	g_RenderManager.SetZEnable( D3DZB_TRUE );
	
	g_RenderManager.SetAlphaBlendEnable( TRUE );
	g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
	g_RenderManager.SetDestBlend(  D3DBLEND_DESTALPHA );
	
	g_RenderManager.SetFillMode( D3DFILL_SOLID );
	
	m_SwordTail.Render( g_lpDevice );
	
	g_RenderManager.SetCullMode( D3DCULL_CCW );
	
	g_RenderManager.SetZWriteEnable( TRUE );
	g_RenderManager.SetZEnable( TRUE );
	
	g_RenderManager.SetAlphaBlendEnable( FALSE );	
	
	return TRUE;
}

int	CEFBonWarSkill::SetAEffectBonJCraFen()
{
	//................................................................................	
	// 어택 사운드
	//................................................................................	
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillAttackedSound( m_pExtraInfo->SkillCode , m_pDestChr );

	g_Particle.SetEffSetCon( 0.0f , EF_B_JAHE_CRAZY_FENED , ESLT_CHARPOS , 
							 8.1f , m_pDestChr->GetUnique() , m_pDestChr->GetIndex(), 
							 ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr) );

	if(m_pExtraInfo)
	{
		m_pExtraInfo->AfterEffect = EF_B_JAHE_CRAZY_FENED;
		m_pExtraInfo->DestType = ESLT_CHARPOS;
		m_pExtraInfo->lifeTime = 8.1f;
	}
	
	g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , m_pDestChr , m_pExtraInfo );	
	
	return TRUE;
}
	
void CEFBonWarSkill::InitSkelDStorm()
{
	m_SwordTail.InitDevice( g_lpDevice );
	CStringToCode	HashString( "afterimage3.tga" );
	m_SwordTail.m_HashCode = HashString.GetHashCode();	
}	

int	CEFBonWarSkill::CheckFrameSkelDStorm()
{
	if( m_pSrcChr->m_ani_frame == 39 )
	{
		m_pSrcChr->m_IsApplyVA = TRUE;
		m_pSrcChr->m_IsNextApplyVA = TRUE;
	}

	m_pSrcChr->BoneRender();

	if( m_pSrcChr->m_ani_frame == 39 )
	{
		if( !m_ISDamageProcess )
		{
			g_Particle.SetEffSetCon( 0.0f , EF_B_SKEL_BAIN_DARK_STORM , ESLT_CHARPOS , 10.1f , 
								 	 m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex() );

			SetAfterEffect();
			m_ISDamageProcess = TRUE;
		}
	}

	if(m_pSrcChr->m_ani_frame + c_MotionBlendHalfFrame > (int)m_pSrcChr->m_end_frame)
	{
		SetDestroy();
		return TRUE;
	}	
	
	return TRUE;
}

int	CEFBonWarSkill::UpdateSkelDStorm(float dtime)
{
	///-- 공격 주체가 사라졌을때
	Character			*pTempDest = NULL;
	EFFECT_EXTRA_INFO    Info;
	
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr ) )
	{
		if(!m_ISDamageProcess)
		{
			SetDestroy(TRUE);		
			m_ISDamageProcess = TRUE;
		}
		else
		{
			SetDestroy();
		}

		return TRUE;
	}

	D3DXMATRIX   matTemp;
	D3DXVECTOR3  vHead;
	D3DXVECTOR3  vCenter;

	m_pSrcChr->GetDirectBoneLinkTM( 17 , matTemp );	
	vHead.x = matTemp._41;
	vHead.y = matTemp._42;
	vHead.z = matTemp._43;

	m_pSrcChr->GetDirectBoneLinkTM( 18 , matTemp );
	vCenter.x = matTemp._41;
	vCenter.y = matTemp._42;
	vCenter.z = matTemp._43;

	m_SwordTail.AddPoint( vCenter , vHead , g_lpDevice );		
	
	return TRUE;
}

int	CEFBonWarSkill::RenderSkelDStorm(float dtime)
{
	D3DXMATRIX matLocal;	
	
	D3DXMatrixIdentity( &matLocal );

	g_RenderManager.SetTransform( D3DTS_WORLD , &matLocal );

	g_RenderManager.SetCullMode( D3DCULL_NONE );
	
	g_RenderManager.SetZWriteEnable( FALSE );
	g_RenderManager.SetZEnable( D3DZB_TRUE );
	
	g_RenderManager.SetAlphaBlendEnable( TRUE );
	g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
	g_RenderManager.SetDestBlend(  D3DBLEND_DESTALPHA );
	
	g_RenderManager.SetFillMode( D3DFILL_SOLID );
	
	m_SwordTail.Render( g_lpDevice );
	
	g_RenderManager.SetCullMode( D3DCULL_CCW );
	
	g_RenderManager.SetZWriteEnable( TRUE );
	g_RenderManager.SetZEnable( TRUE );
	
	g_RenderManager.SetAlphaBlendEnable( FALSE );	
	
	return TRUE;
}

int	CEFBonWarSkill::SetAEffectSkelDStorm()
{

	Character			*pTempDest = NULL;
	EFFECT_EXTRA_INFO    Info;

	if( m_pExtraInfo != NULL )
	{
		// 1:N 데미지
		int Count = m_pExtraInfo->GetTargetCount();			
		
		for( int i = 0 ; i < Count ; ++i )
		{
			if( m_pExtraInfo->GetTargetInfo( &pTempDest , &Info , i ) )
			{
				//................................................................................	
				// 어택 사운드
				//................................................................................	
				if( m_pExtraInfo != NULL )
					g_Particle.m_EffectManager.GetSkillAttackedSound( m_pExtraInfo->SkillCode , pTempDest );				
				

				g_Particle.SetEffSetCon( 0.0f , EF_B_SKEL_BAIN_DARK_STORMED , ESLT_CHARPOS , 2.1f , pTempDest->GetUnique() , 
										 pTempDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr) );
					
				Info.AfterEffect = EF_B_SKEL_BAIN_DARK_STORMED;
				Info.DestType = ESLT_CHARPOS;
				Info.lifeTime = 5.1f;
					
				g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , pTempDest , &Info );		
			}

			Info.Clear();
		}
	}	
	
	return TRUE;
}


