#include "CEFCannonEffect.h"
#include "CEFArrow.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include	<SOUND\Sound.H>
#include "..\\..\\CRenderManager.h"


CEFCannonEffect::CEFCannonEffect()
{
	m_IsConEffect = TRUE;
	m_ISShot = FALSE;
	
	m_nImpacFrame = 0;
	m_uEnergyGetEffId = 0;
	m_uEnergyGetType = 0;
	
	m_nSrcBoneNum = 0;
	m_uBillType = 0;
	m_uTaleType = 0;
	m_uPercuEffId = 0;
	m_uPercuEffType = 0;
	m_uExploEffId = 0;
	m_uExploEffType = 0;
	m_uAttackedEffect = 0;
	m_uAttackedType = 0;

	m_fXRadian = 0.0f;
	m_fYRadian = 0.0f;
	m_RangeTime = 0;
	m_InitTime = 0;
}

CEFCannonEffect::~CEFCannonEffect()
{
	

}

int	CEFCannonEffect::CheckFrame(void)
{
	if( m_ISShot == TRUE )
	{
		return TRUE;
	}	
	
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr ) )
	{
		SetDestroy(TRUE);
		return TRUE;
	}
	
	// 임펙트 프레임 화살 나가는 시점
	if( m_Src_Ani_Index != m_pSrcChr->m_ani_index || m_pSrcChr->m_ani_frame == m_nImpacFrame )
	{
		///-- 에너지 모으는 이펙트 해제
		if(m_uEnergyGetEffId)
		{
			g_Particle.DelEffSetCon( m_uEnergyGetEffId , m_uEnergyGetType , 
									  m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex() );
		}		
		
		CBillboardData *pData = NULL;
		///-- 빌보드 데이터 가져옴 
		if( NULL == ( pData = g_Billboard.m_BillboardData.GetKeyDataPointer( m_uBillType ) ) )
		{
			SetDestroy(TRUE);
			return TRUE;
		}		
		
		m_pSrcChr->m_IsApplyVA = TRUE;
		m_pSrcChr->m_IsNextApplyVA = TRUE;
		m_pSrcChr->BoneRender();
		
		m_LinkBillboard.MakeLink(pData);
		m_LinkBillboard.m_vLoc = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		m_InitTime = g_nowTime;

		D3DXMATRIX matBone;
		m_pSrcChr->GetBoneLinkTM( m_nSrcBoneNum , matBone, FALSE );
		m_vStartPos = D3DXVECTOR3(matBone._41, matBone._42, matBone._43);

		m_pDestChr->GetBoneLinkTM( 2 , matBone );
		D3DXVECTOR3 vDestPos( matBone._41 ,
							  matBone._42 ,
							  matBone._43 );

		m_RangeTime = PositionToTimer( 50 , m_vStartPos , vDestPos );
		m_fXRadian = 0.0f;
		m_fYRadian = m_pSrcChr->m_radian;
		GetXAndYRadian(m_fXRadian, m_fYRadian, m_vStartPos, vDestPos);

		D3DXMATRIX matRotX, matRotY;

		D3DXMatrixRotationX( &matRotX , m_fXRadian );	
		D3DXMatrixRotationY( &matRotY , m_fYRadian );
		D3DXMatrixMultiply( &m_matLocal , &matRotX , &matRotY );

		m_matLocal._41 = m_vStartPos.x;
		m_matLocal._42 = m_vStartPos.y;
		m_matLocal._43 = m_vStartPos.z;

		///-- 격발 이펙트 시전
		if(m_uPercuEffId)
		{
			g_Particle.SetEffSetCon( 0.0f , m_uPercuEffId , m_uPercuEffType , 2.1f , m_pSrcChr->GetUnique() , 
												 m_pSrcChr->GetIndex(), D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr->GetDirection() );
		}

		m_ISShot = TRUE;
		
		//................................................................................	
		// 발동 사운드
		//................................................................................	
		if( m_pExtraInfo != NULL )
			g_Particle.m_EffectManager.GetSkillImfactSound( m_pExtraInfo->SkillCode , m_pSrcChr );		
	
	}	
	
	return TRUE;
}

int	CEFCannonEffect::Render(float dtime)
{
	if( !m_ISShot )
	{
		return TRUE;
	}	
	
	D3DXMATRIX matIdent;
	D3DXMatrixIdentity( &matIdent );
	D3DXVECTOR3 vPos = D3DXVECTOR3(m_matLocal._41, m_matLocal._42, m_matLocal._43);
		
	///-- 포탄 그리기
	g_RenderManager.SetTransform( D3DTS_WORLD , &matIdent );
	g_RenderManager.SetCullMode( D3DCULL_CCW );
	g_RenderManager.SetZWriteEnable( TRUE );
	
	m_LinkBillboard.m_vLoc = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_LinkBillboard.m_matPostWorld = m_matLocal;

	g_Billboard.DisplayManager( m_LinkBillboard , dtime );	
#ifdef BATCH_RENDER
	m_LinkBillboard.DrawBatchBillboard(TRUE);
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
	m_LinkBillboard.DrawBillboard(TRUE);
#endif	//	BATCH_RENDER
	
	
	///-- 포탄 꼬리 그리기
	if(m_uTaleType)
	{
		g_RenderManager.SetTransform( D3DTS_WORLD , &matIdent );
		g_RenderManager.SetCullMode( D3DCULL_NONE );
		g_RenderManager.SetZWriteEnable( FALSE );
		
		g_Particle.DisplayUpdate( m_uTaleType , vPos );

		g_RenderManager.SetCullMode( D3DCULL_CCW );		
		g_RenderManager.SetZWriteEnable( TRUE );
	}
	
	return TRUE;
}	
						
int CEFCannonEffect::Update(float dtime)
{
	if( !m_ISShot )
	{
		return TRUE;
	}	
	
	///-- 공격 주체가 사라졌을때
	Character			*pTempDest = NULL;
	EFFECT_EXTRA_INFO    Info;
	
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr ) )
	{
		SetDestroy(TRUE);
		return TRUE;
	}

	D3DXMATRIX matBone;
	m_pDestChr->GetBoneLinkTM( 2 , matBone );

	D3DXVECTOR3 vDestPos( matBone._41 ,
						  matBone._42 ,
						  matBone._43 );
	
	D3DXVECTOR3 vDistance = vDestPos - m_vStartPos;	
	float Range = ( ( ( float ) ( SAFE_TIME_SUB( g_nowTime , m_InitTime ) ) / ( float ) m_RangeTime ) );
	D3DXVECTOR3 vPos      = m_vStartPos + ( vDistance * Range );	
	//............................................................................................................
	// 캐릭터에 충돌 처리 ( 이펙트 .... T.T )
	//............................................................................................................
	if( Range >= 1.0f ) 
	{
		Range = 1.0f;		
		SetDestroy();	
		
		///-- 주변 광역 폭팔 이펙트 시전
		if(m_uExploEffId)
		{
			D3DXVECTOR3 vTemPos = D3DXVECTOR3( m_pDestChr->GetPosTM()._41, 
											   0.0f,
											   m_pDestChr->GetPosTM()._43);		
			vTemPos.y = g_Map.PHF_GetHeight( vTemPos.x , vTemPos.z );
			g_Particle.SetEffSetCon( 0.0f , m_uExploEffId , ESLT_POS , 2.5f , -1 , -1 , vTemPos, 0.0f );
		}		
		
		SetAfterEffect();		
		return TRUE;
	}

	GetXAndYRadian(m_fXRadian, m_fYRadian, vPos, vDestPos);
	D3DXMATRIX matRotX, matRotY;

	D3DXMatrixRotationX( &matRotX , m_fXRadian + D3DXToRadian( 90 ) );
	D3DXMatrixRotationY( &matRotY , m_fYRadian );
	D3DXMatrixMultiply( &m_matLocal , &matRotX , &matRotY );	
	
	m_matLocal._41 = vPos.x;
	m_matLocal._42 = vPos.y;
	m_matLocal._43 = vPos.z;
	
	return TRUE;
}

int CEFCannonEffect::SetAfterEffect(void)
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
				
				if(m_uAttackedType == ESLT_SRC_RAD)
				{
					g_Particle.SetEffSetCon( 0.0f , m_uAttackedEffect , m_uAttackedType , 2.1f , pTempDest->GetUnique() , 
										 pTempDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr ), m_pSrcChr->GetDirection() + D3DXToRadian( 180 ) );
				}
				else
				{	
					g_Particle.SetEffSetCon( 0.0f , m_uAttackedEffect , m_uAttackedType , 2.1f , pTempDest->GetUnique() , 
										 pTempDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr) );
				}		
				
				Info.AfterEffect = m_uAttackedEffect;
				Info.DestType = m_uAttackedType;
				Info.lifeTime = 5.1f;
					
				g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , pTempDest , &Info );		
			}

			Info.Clear();
		}
	}	
	
	return TRUE;
}

void CEFCannonEffect::Init(void)
{
	if( m_pExtraInfo == NULL )
	{
		SetDestroy();
		return;
	}
	
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr , m_pDestChr ) )
	{
		SetDestroy(TRUE);
		return;
	}

	m_Src_Ani_Index = m_pSrcChr->m_ani_index;

	///-- 에너지 모으는 이펙트 연동
	if(m_uEnergyGetEffId)
	{
		g_Particle.m_IsGUnitLinkBoneAndSizeFix = TRUE;
		g_Particle.SetEffSetCon( 0.0f , m_uEnergyGetEffId , m_uEnergyGetType , 
			EFFECT_SET_LOOP_TIME , m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex() );
	}
}
