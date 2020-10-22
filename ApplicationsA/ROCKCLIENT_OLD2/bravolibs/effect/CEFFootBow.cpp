#include "CEFFootBow.h"
#include "CEFArrow.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include	<SOUND\Sound.H>
#include "..\\..\\CRenderManager.h"

extern long g_nowTime;

CEFFootBow::CEFFootBow()
{
	m_IsConEffect = TRUE;
	m_ISShot = FALSE;	

	m_dNpcRealFileCode = 0;
}


CEFFootBow::~CEFFootBow()
{
}

int	CEFFootBow::CheckFrame(void)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr , m_pDestChr ) )
	{
		SetDestroy(TRUE);
		return TRUE;
	}
	
	if( m_ISShot == TRUE )
	{
		return TRUE;
	}
	
	if( m_SrcAniFrame < m_pSrcChr->m_ani_frame )
	{
		m_SrcAniFrame = m_pSrcChr->m_ani_frame;	
	}
	
	if(m_Src_Ani_Index != m_pSrcChr->m_ani_index)
	{
		m_pSrcChr->m_start_blend_frame = 0;
	}

	int nCheckFrame = 44;

	if( m_dNpcRealFileCode )
	{
		if( m_dNpcRealFileCode == 21100019 )
		{
			nCheckFrame = 28;
		}
	}
	
	if( m_Src_Ani_Index != m_pSrcChr->m_ani_index || GetSrcAniCount() == nCheckFrame )
	{
		m_pSrcChr->m_IsApplyVA = TRUE;
		m_pSrcChr->m_IsNextApplyVA = TRUE;
		m_pSrcChr->BoneRender();		
		
		m_ISShot = TRUE;
		m_vStartPos   = D3DXVECTOR3( m_pSrcChr->GetRealLHandTM()._41 , 
									 m_pSrcChr->GetRealLHandTM()._42 , 	
									 m_pSrcChr->GetRealLHandTM()._43   ); 		
		D3DXMATRIX matBone;
		m_pDestChr->GetBoneLinkTM( 7 , matBone );
		
		D3DXVECTOR3 vDestPos( matBone._41 ,
							  matBone._42 ,
							  matBone._43  );
		
		D3DXVECTOR3 vDistance = vDestPos - m_vStartPos;
		
		D3DXMatrixRotationY( &m_matRotY , m_pSrcChr->m_radian );

		m_matLocal._41 = m_vStartPos.x;
		m_matLocal._42 = m_vStartPos.y;
		m_matLocal._43 = m_vStartPos.z;
		
		m_RangeTime	  = PositionToTimer( 50 , m_vStartPos , vDestPos );
		
		m_OldTimer    = g_nowTime;

		if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( m_pExtraInfo->SkillCode , m_pSrcChr );
	}

	return TRUE;
}

void CEFFootBow::Init(void)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr , m_pDestChr ) )
	{
		SetDestroy(TRUE);
		return;
	}

	m_Src_Ani_Index = m_pSrcChr->m_ani_index;

	if( !m_pSrcChr->m_bPC && m_pSrcChr->m_State.nCharacterType != nCharacterTypePlayerNPC )
	{
		m_dNpcRealFileCode = g_Pc_Manager.GetNPCRealFileCode( g_Map.m_NpcTable[ m_pSrcChr->m_nTableIndex ].code );
	}
	
	if( m_dNpcRealFileCode )
	{
		return;
	}

	///-- 이곳에서 스킬의 정보를 알아옴
	SSkillBaseInfo* skill_Info;
	skill_Info = g_Pc_Manager.GetSkillBaseInfoPtr( m_pSrcChr->m_curt_event.count );
	if( skill_Info != NULL )
	{
		if(skill_Info->theNeedSpelling)
		{
#ifdef SNIPER_MOD_MOTION			
			m_pSrcChr->m_ani_frame = 44 - 10;
#else
			m_pSrcChr->m_ani_frame = 20;
#endif								
		}
	}
}

int	CEFFootBow::Render(float dtime)
{
	if( m_ISShot )
	{		
		g_RenderManager.SetTransform( D3DTS_WORLD , &m_matLocal );
			
		g_RenderManager.SetCullMode( D3DCULL_CCW );
		
		g_RenderManager.SetZWriteEnable( TRUE );
		g_RenderManager.SetZEnable( TRUE );
		
		g_RenderManager.SetAlphaBlendEnable( FALSE );

		CEFArrow::Render( 0 );
	}
	else
	{	
		if(m_pSrcChr->m_pre_ani_number != -1)
		{
			return TRUE;
		}	
		
		// 임펙트 프레임이 1 이라서 여기서 찍어준다 된장
		g_RenderManager.SetTransform( D3DTS_WORLD, &m_pSrcChr->GetRHandTM() );
								
		g_RenderManager.SetCullMode( D3DCULL_CCW );
		
		g_RenderManager.SetZWriteEnable( TRUE );
		g_RenderManager.SetZEnable( TRUE );
		
		g_RenderManager.SetAlphaBlendEnable( FALSE );

		g_Particle.m_ArrowDisplay.Render( g_lpDevice );
	}

	return TRUE;
}

int CEFFootBow::Update(float dtime)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr , m_pDestChr ) )
	{
		SetDestroy(TRUE);
		return TRUE;
	}

	if( FALSE == m_ISShot )
	{
		return TRUE;
	}	

	D3DXMATRIX matBone;
	m_pDestChr->GetBoneLinkTM( 7 , matBone );
	
	D3DXVECTOR3 vDestPos( matBone._41 ,
						  matBone._42 ,
						  matBone._43 );
	
	D3DXVECTOR3 vDistance   = vDestPos - m_vStartPos;
	
	float       Range       = ( ( ( float ) SAFE_TIME_SUB( g_nowTime , m_OldTimer ) / ( float ) m_RangeTime ) );
	float		RangeRadius = D3DXToRadian( 180 ) * Range;
	
	D3DXMATRIX matRot;

	D3DXMatrixRotationX( &matRot , D3DXToRadian( 90.0f ) - RangeRadius );
	D3DXMatrixMultiply( &m_matLocal , &matRot , &m_matRotY );

	//............................................................................................................
	// 캐릭터에 충돌 처리 ( 이펙트 .... T.T )
	//............................................................................................................
	if( Range >= 1.0f ) 
	{
		Range = 1.0f;
		
		SetAfterEffect();
		SetDestroy();
		
		return TRUE;
	}
	
	D3DXVECTOR3 vPos      = m_vStartPos + ( vDistance * Range );  
	
	vPos.y = vPos.y + sinf( RangeRadius ) * 30.0f;

	m_matLocal._41 = vPos.x;
	m_matLocal._42 = vPos.y;
	m_matLocal._43 = vPos.z;

	g_Particle.DisplayUpdate( PT_FOOT_BOW_TAIL , vPos );

	return TRUE;
}

int CEFFootBow::SetAfterEffect(void)

{
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillAttackedSound( m_pExtraInfo->SkillCode , m_pDestChr );

	int EffectID = 0;
	
	switch( m_ID )
	{
		case EF_FOOT_BOW_0:
		
			EffectID = EF_FOOT_BOW_ATTACKED_0;
				
			break;

		case EF_FOOT_BOW_1:

			EffectID = EF_FOOT_BOW_ATTACKED_1;

			break;

		case EF_FOOT_BOW_2:

			EffectID = EF_FOOT_BOW_ATTACKED_2;

			break;
	}

	g_Particle.SetEffSetCon( 0.0f , EffectID , ESLT_CHARPOS , 2.0f , 
		m_pDestChr->GetUnique() , m_pDestChr->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr) );

	if(m_pExtraInfo)
	{
		m_pExtraInfo->AfterEffect = EffectID;
		m_pExtraInfo->DestType = ESLT_CHARPOS;
		m_pExtraInfo->lifeTime = 2.0f;
	}
	
	g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , m_pDestChr , m_pExtraInfo );

	return TRUE;
}
