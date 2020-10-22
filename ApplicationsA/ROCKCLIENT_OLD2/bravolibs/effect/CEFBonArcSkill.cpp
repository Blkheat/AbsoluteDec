#include "CEFBowAttack.h"
#include "CEFArrow.h"
#include "CEFBonArcSkill.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include	<SOUND\Sound.H>
#include "..\\..\RockClient.h"
#include "..\\..\\map.h"
#include "..\\..\\CRenderManager.h"

extern long		 g_nowTime;

CEFBonArcSkill::CEFBonArcSkill()
{
	m_ISShot = FALSE;
	m_IsConEffect = TRUE;
	m_fXRadian = 0.0f;
	m_fYRadian = 0.0f;
	
	m_dNpcRealFileCode = 0;
}

CEFBonArcSkill::~CEFBonArcSkill()
{

}

int	CEFBonArcSkill::CheckFrame(void)
{
	
	switch(m_ID)
	{
		case EF_B_ARCHER_BASE_ATTACK:
			{
				return CheckFrameBonABaseAttack();
			}
			break;
		case EF_B_ARCHER_FIRE_ARROW:
			{
				return CheckFrameBonAFireArrow();
			}
			break;
		case EF_B_ARCHER_DECAY_ARROW:
			{
				return CheckFrameBonADecayArrow();
			}
			break;
		case EF_B_ARCHER_PAIN_ARROW:
			{
				return CheckFrameBonAPainArrow();
			}
			break;
		case EF_B_ARCHER_DARK_ARROW:
			{
				return CheckFrameBonADarkArrow();
			}
			break;
		case EF_B_ARCHER_CURSE_ARROW:
			{
				return CheckFrameBonACurseArrow();
			}
			break;
		case EF_B_ARCHER_PARALYSIS_ARROW:
			{
				return CheckFrameBonAParalysisArrow();
			}
			break;
		case EF_B_BOS_ARCHER_RANDOM_ARROW:
			{
				return CheckFrameBonBRandomArrow();
			}
			break;
	}	
	
	return TRUE;
}

void CEFBonArcSkill::Init()
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr , m_pDestChr ) )
	{
		if(m_ID == EF_B_ARCHER_BASE_ATTACK)
		{
			SetDestroy(TRUE, TRUE);
		}
		else
		{
			SetDestroy(TRUE);
		}
		return;
	}

	m_Src_Ani_Index = m_pSrcChr->m_ani_index;

	switch(m_ID)
	{
		case EF_B_ARCHER_BASE_ATTACK:
			{
				InitBonABaseAttack();
			}
			break;
		case EF_B_ARCHER_FIRE_ARROW:
			{
				InitBonAFireArrow();
			}
			break;
		case EF_B_ARCHER_DECAY_ARROW:
			{
				InitBonADecayArrow();
			}
			break;
		case EF_B_ARCHER_PAIN_ARROW:
			{
				InitBonAPainArrow();
			}
			break;
		case EF_B_ARCHER_DARK_ARROW:
			{
				InitBonADarkArrow();
			}
			break;
		case EF_B_ARCHER_CURSE_ARROW:
			{
				InitBonACurseArrow();
			}
			break;
		case EF_B_ARCHER_PARALYSIS_ARROW:
			{
				InitBonAParalysisArrow();
			}
			break;
		case EF_B_BOS_ARCHER_RANDOM_ARROW:
			{
				InitBonBRandomArrow();
			}
			break;
	}
	
	if( !m_pSrcChr->m_bPC && m_pSrcChr->m_State.nCharacterType != nCharacterTypePlayerNPC )
	{
		m_dNpcRealFileCode = g_Pc_Manager.GetNPCRealFileCode( g_Map.m_NpcTable[ m_pSrcChr->m_nTableIndex ].code );
	}
}

int CEFBonArcSkill::Update(float dtime)
{
	switch(m_ID)
	{
		case EF_B_ARCHER_BASE_ATTACK:
			{
				return UpdateBonABaseAttack(dtime);
			}
			break;
		case EF_B_ARCHER_FIRE_ARROW:
			{
				return UpdateBonAFireArrow(dtime);
			}
			break;
		case EF_B_ARCHER_DECAY_ARROW:
			{
				return UpdateBonADecayArrow(dtime);
			}
			break;
		case EF_B_ARCHER_PAIN_ARROW:
			{
				return UpdateBonAPainArrow(dtime);
			}
			break;
		case EF_B_ARCHER_DARK_ARROW:
			{
				return UpdateBonADarkArrow(dtime);
			}
			break;
		case EF_B_ARCHER_CURSE_ARROW:
			{
				return UpdateBonACurseArrow(dtime);
			}
			break;
		case EF_B_ARCHER_PARALYSIS_ARROW:
			{
				return UpdateBonAParalysisArrow(dtime);
			}
			break;
		case EF_B_BOS_ARCHER_RANDOM_ARROW:
			{
				return UpdateBonBRandomArrow(dtime);
			}
			break;
	}	
	
	return TRUE;
}

int	CEFBonArcSkill::Render(float dtime)
{
	switch(m_ID)
	{
		case EF_B_ARCHER_BASE_ATTACK:
			{
				return RenderBonABaseAttack(dtime);
			}
			break;
		case EF_B_ARCHER_FIRE_ARROW:
			{
				return RenderBonAFireArrow(dtime);
			}
			break;
		case EF_B_ARCHER_DECAY_ARROW:
			{
				return RenderBonADecayArrow(dtime);
			}
			break;
		case EF_B_ARCHER_PAIN_ARROW:
			{
				return RenderBonAPainArrow(dtime);
			}
			break;
		case EF_B_ARCHER_DARK_ARROW:
			{
				return RenderBonADarkArrow(dtime);
			}
			break;
		case EF_B_ARCHER_CURSE_ARROW:
			{
				return RenderBonACurseArrow(dtime);
			}
			break;
		case EF_B_ARCHER_PARALYSIS_ARROW:
			{
				return RenderBonAParalysisArrow(dtime);
			}
			break;
		case EF_B_BOS_ARCHER_RANDOM_ARROW:
			{
				return RenderBonBRandomArrow(dtime);
			}
			break;
	}	
	
	return TRUE;	
}

int	CEFBonArcSkill::SetAfterEffect()
{
	switch(m_ID)
	{
		case EF_B_ARCHER_BASE_ATTACK:
			{
				return SetAEffectBonABaseAttack();
			}
			break;
		case EF_B_ARCHER_FIRE_ARROW:
			{
				return SetAEffectBonAFireArrow();
			}
			break;
		case EF_B_ARCHER_DECAY_ARROW:
			{
				return SetAEffectBonADecayArrow();
			}
			break;
		case EF_B_ARCHER_PAIN_ARROW:
			{
				return SetAEffectBonAPainArrow();
			}
			break;
		case EF_B_ARCHER_DARK_ARROW:
			{
				return SetAEffectBonADarkArrow();
			}
			break;
		case EF_B_ARCHER_CURSE_ARROW:
			{
				return SetAEffectBonACurseArrow();
			}
			break;
		case EF_B_ARCHER_PARALYSIS_ARROW:
			{
				return SetAEffectBonAParalysisArrow();
			}
			break;
		case EF_B_BOS_ARCHER_RANDOM_ARROW:
			{
				return SetAEffectBonBRandomArrow();
			}
			break;
	}	

	return TRUE;
}

void CEFBonArcSkill::InitBonABaseAttack()
{
	m_vStartPos   = D3DXVECTOR3( m_pSrcChr->GetRealLHandTM()._41 , 
								 m_pSrcChr->GetRealLHandTM()._42 , 	
								 m_pSrcChr->GetRealLHandTM()._43   ); 
			
	D3DXMATRIX matBone;
	m_pDestChr->GetBoneLinkTM( 2 , matBone );

	D3DXVECTOR3 vDestPos( matBone._41 ,
						  matBone._42 ,
						  matBone._43 );

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

	m_RangeTime	  = PositionToTimer( 50 , m_vStartPos , vDestPos );
	m_InitTime    = g_nowTime;
}

int CEFBonArcSkill::CheckFrameBonABaseAttack()
{
	return TRUE;
}

int CEFBonArcSkill::UpdateBonABaseAttack(float dtime)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr , m_pDestChr ) )
	{
		SetDestroy(TRUE, TRUE);
		return TRUE;
	}

	D3DXMATRIX matBone;
	m_pDestChr->GetBoneLinkTM( 2 , matBone );

	D3DXVECTOR3 vDestPos( matBone._41 ,
						  matBone._42 ,
						  matBone._43 );
	
	D3DXVECTOR3 vDistance = vDestPos - m_vStartPos;	
	float       Range     = ( ( ( float ) ( SAFE_TIME_SUB( g_nowTime , m_InitTime ) ) / ( float ) m_RangeTime ) );

	//............................................................................................................
	// 캐릭터에 충돌 처리 ( 이펙트 .... T.T )
	//............................................................................................................
	if( Range >= 1.0f ) 
	{
		Range = 1.0f;
		///-- 화살 붙이기...
		g_Particle.m_EffectManager.CreateArrowStuck(
			m_pDestChr,
			m_pSrcChr->m_radian,
			D3DXVECTOR3( RANDOM_NUM , RANDOM_NUM , RANDOM_NUM ) ,
			2 );

		SetDestroy();	
		SetAfterEffect();		
		return TRUE;
	}

	D3DXVECTOR3 vPos = m_vStartPos + ( vDistance * Range );	
	
	GetXAndYRadian(m_fXRadian, m_fYRadian, vPos, vDestPos);

	D3DXMATRIX matRotX, matRotY;

	D3DXMatrixRotationX( &matRotX , m_fXRadian );
	D3DXMatrixRotationY( &matRotY , m_fYRadian );
	D3DXMatrixMultiply( &m_matLocal , &matRotX , &matRotY );	
	
	m_matLocal._41 = vPos.x;
	m_matLocal._42 = vPos.y;
	m_matLocal._43 = vPos.z;

	return TRUE;	
}

int CEFBonArcSkill::RenderBonABaseAttack(float dtime)
{
	g_RenderManager.SetTransform( D3DTS_WORLD , &m_matLocal );
	
	g_RenderManager.SetAlphaBlendEnable( FALSE );
	g_RenderManager.SetZWriteEnable( TRUE );
	g_RenderManager.SetZEnable( TRUE );
	g_RenderManager.SetCullMode( D3DCULL_CCW );

	CEFArrow::Render( 0 );
	
	
	return TRUE;	
}

int CEFBonArcSkill::SetAEffectBonABaseAttack()
{
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillAttackedSound( m_pExtraInfo->SkillCode , m_pDestChr );	
	
	g_Particle.m_EffectManager.StandTargetHit( m_pSrcChr , m_pDestChr , m_pExtraInfo );
	
	return TRUE;
}

void CEFBonArcSkill::InitBonAFireArrow()
{
	m_Src_Ani_Index = m_pSrcChr->m_ani_index;

	g_Particle.SetEffSetCon( 0.0f , EF_B_ARCHER_FIRE_ARROW, ESLT_CHARPOS , 
							 EFFECT_SET_LOOP_TIME , m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex() );
}

int	CEFBonArcSkill::CheckFrameBonAFireArrow()
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

	int nCheckFrame = 49;

	if( m_dNpcRealFileCode )
	{
		if( m_dNpcRealFileCode == 21100019 )
		{
			nCheckFrame = 25;
		}
	}

	BOOL bIsAction = FALSE;
	
	if( m_Src_Ani_Index != m_pSrcChr->m_ani_index )
	{
		bIsAction = TRUE;
	}

	if( m_pSrcChr->m_ani_frame == nCheckFrame )
	{
		bIsAction = TRUE;
	}
	
	// 임펙트 프레임 화살 나가는 시점
	if( bIsAction )
	{
		g_Particle.DelEffSetCon( EF_B_ARCHER_FIRE_ARROW , ESLT_CHARPOS , 
								 m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex(), TRUE );
		
		m_pSrcChr->m_IsApplyVA = TRUE;
		m_pSrcChr->m_IsNextApplyVA = TRUE;
		m_pSrcChr->BoneRender();
		
		m_vStartPos = D3DXVECTOR3( m_pSrcChr->GetRealLHandTM()._41 , 
								   m_pSrcChr->GetRealLHandTM()._42 , 	
								   m_pSrcChr->GetRealLHandTM()._43   );	
			
		D3DXMATRIX matBone;
		m_pDestChr->GetBoneLinkTM( 2 , matBone );

		D3DXVECTOR3 vDestPos( matBone._41 ,
							  matBone._42 ,
							  matBone._43 );

		D3DXVECTOR3 vDistance = vDestPos - m_vStartPos;
		
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

		m_RangeTime	  = PositionToTimer( 50 , m_vStartPos , vDestPos );

		m_InitTime    = g_nowTime;
		
		m_ISShot = TRUE;

		//................................................................................	
		// 발동 사운드
		//................................................................................	
		if( m_pExtraInfo != NULL )
			g_Particle.m_EffectManager.GetSkillImfactSound( m_pExtraInfo->SkillCode , m_pSrcChr );
	}
	
	return TRUE;
}

int	CEFBonArcSkill::UpdateBonAFireArrow(float dtime)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr , m_pDestChr ) )
	{
		g_Particle.DelEffSetCon( EF_B_ARCHER_FIRE_ARROW , ESLT_CHARPOS , 
								 m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex() );
		SetDestroy(TRUE);
		return TRUE;
	}

	if( !m_ISShot )
	{
		return TRUE;
	}

	D3DXMATRIX matBone;
	m_pDestChr->GetBoneLinkTM( 2 , matBone );

	D3DXVECTOR3 vDestPos( matBone._41 ,
						  matBone._42 ,
						  matBone._43 );
	
	D3DXVECTOR3 vDistance = vDestPos - m_vStartPos;	
	float   Range = ( ( ( float ) ( SAFE_TIME_SUB( g_nowTime , m_InitTime ) ) / ( float ) m_RangeTime ) );

	//............................................................................................................
	// 캐릭터에 충돌 처리 ( 이펙트 .... T.T )
	//............................................................................................................
	if( Range >= 1.0f ) 
	{
		Range = 1.0f;
		SetDestroy();	
		SetAfterEffect();		
		return TRUE;
	}

	D3DXVECTOR3 vPos      = m_vStartPos + ( vDistance * Range );
	GetXAndYRadian(m_fXRadian, m_fYRadian, vPos, vDestPos);

	D3DXMATRIX matRotX, matRotY;

	D3DXMatrixRotationX( &matRotX , m_fXRadian );
	D3DXMatrixRotationY( &matRotY , m_fYRadian );
	D3DXMatrixMultiply( &m_matLocal , &matRotX , &matRotY );

	m_matLocal._41 = vPos.x;
	m_matLocal._42 = vPos.y;
	m_matLocal._43 = vPos.z;

	g_Particle.DisplayUpdate( 24566 , vPos );

	return TRUE;	
}

int	CEFBonArcSkill::RenderBonAFireArrow(float dtime)
{
	// 활 날라 갈때
	if( m_ISShot )
	{
		g_RenderManager.SetTransform( D3DTS_WORLD , &m_matLocal );

		g_RenderManager.SetCullMode( D3DCULL_CCW );
		g_RenderManager.SetZWriteEnable( TRUE );
		g_RenderManager.SetZEnable( TRUE );	
		g_RenderManager.SetAlphaBlendEnable( FALSE );
		CEFArrow::Render( 0 );
	}
	
	return TRUE;
}

int	CEFBonArcSkill::SetAEffectBonAFireArrow()
{
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillAttackedSound( m_pExtraInfo->SkillCode , m_pDestChr );
	
	g_Particle.SetEffSetCon( 0.0f , EF_B_ARCHER_FIRE_ARROWED , ESLT_SRC_RAD , 
								 3.1f , m_pDestChr->GetUnique() , m_pDestChr->GetIndex(), 
								 ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr), 
								 m_pSrcChr->GetDirection() );

	if(m_pExtraInfo)
	{
		m_pExtraInfo->AfterEffect = EF_B_ARCHER_FIRE_ARROWED;
		m_pExtraInfo->DestType = ESLT_SRC_RAD;
		m_pExtraInfo->lifeTime = 3.1f;
		m_pExtraInfo->fRadian = m_pSrcChr->GetDirection();
	}
	
	g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , m_pDestChr , m_pExtraInfo );	
	
	return TRUE;
}

void CEFBonArcSkill::InitBonADecayArrow()
{
	m_Src_Ani_Index = m_pSrcChr->m_ani_index;

	g_Particle.SetEffSetCon( 0.0f , EF_B_ARCHER_DECAY_ARROW, ESLT_CHARPOS , 
							 EFFECT_SET_LOOP_TIME , m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex() );
}

int CEFBonArcSkill::CheckFrameBonADecayArrow()
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
	
	int nCheckFrame = 49;

	if( m_dNpcRealFileCode )
	{
		if( m_dNpcRealFileCode == 21100019 )
		{
			nCheckFrame = 25;
		}
	}

	BOOL bIsAction = FALSE;
	
	if( m_Src_Ani_Index != m_pSrcChr->m_ani_index )
	{
		bIsAction = TRUE;
	}

	if( m_pSrcChr->m_ani_frame == nCheckFrame )
	{
		bIsAction = TRUE;
	}

	// 임펙트 프레임 화살 나가는 시점
	if( bIsAction )
	{
		g_Particle.DelEffSetCon( EF_B_ARCHER_DECAY_ARROW , ESLT_CHARPOS , 
								 m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex(), TRUE );
		
		m_pSrcChr->m_IsApplyVA = TRUE;
		m_pSrcChr->m_IsNextApplyVA = TRUE;
		m_pSrcChr->BoneRender();
		
		m_vStartPos = D3DXVECTOR3( m_pSrcChr->GetRealLHandTM()._41 , 
								   m_pSrcChr->GetRealLHandTM()._42 , 	
								   m_pSrcChr->GetRealLHandTM()._43   );	
			
		D3DXMATRIX matBone;
		m_pDestChr->GetBoneLinkTM( 2 , matBone );

		D3DXVECTOR3 vDestPos( matBone._41 ,
							  matBone._42 ,
							  matBone._43 );

		D3DXVECTOR3 vDistance = vDestPos - m_vStartPos;
		
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

		m_RangeTime	  = PositionToTimer( 50 , m_vStartPos , vDestPos );

		m_InitTime    = g_nowTime;
		
		m_ISShot = TRUE;

		//................................................................................	
		// 발동 사운드
		//................................................................................	
		if( m_pExtraInfo != NULL )
			g_Particle.m_EffectManager.GetSkillImfactSound( m_pExtraInfo->SkillCode , m_pSrcChr );
	}
	
	return TRUE;	
}

int CEFBonArcSkill::UpdateBonADecayArrow(float dtime)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr , m_pDestChr ) )
	{
		g_Particle.DelEffSetCon( EF_B_ARCHER_DECAY_ARROW , ESLT_CHARPOS , 
								 m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex() );
		SetDestroy(TRUE);

		return TRUE;
	}

	if( !m_ISShot )
	{
		return TRUE;
	}

	D3DXMATRIX matBone;
	m_pDestChr->GetBoneLinkTM( 2 , matBone );

	D3DXVECTOR3 vDestPos( matBone._41 ,
						  matBone._42 ,
						  matBone._43 );
	
	D3DXVECTOR3 vDistance = vDestPos - m_vStartPos;	
	float   Range = ( ( ( float ) ( SAFE_TIME_SUB( g_nowTime , m_InitTime ) ) / ( float ) m_RangeTime ) );

	//............................................................................................................
	// 캐릭터에 충돌 처리 ( 이펙트 .... T.T )
	//............................................................................................................
	if( Range >= 1.0f ) 
	{
		Range = 1.0f;
		SetDestroy();	
		SetAfterEffect();		
		return TRUE;
	}

	D3DXVECTOR3 vPos      = m_vStartPos + ( vDistance * Range );
	GetXAndYRadian(m_fXRadian, m_fYRadian, vPos, vDestPos);

	D3DXMATRIX matRotX, matRotY;

	D3DXMatrixRotationX( &matRotX , m_fXRadian );
	D3DXMatrixRotationY( &matRotY , m_fYRadian );
	D3DXMatrixMultiply( &m_matLocal , &matRotX , &matRotY );

	m_matLocal._41 = vPos.x;
	m_matLocal._42 = vPos.y;
	m_matLocal._43 = vPos.z;

	g_Particle.DisplayUpdate( 24567 , vPos );	
	
	return TRUE;
}

int CEFBonArcSkill::RenderBonADecayArrow(float dtime)
{
	// 활 날라 갈때
	if( m_ISShot )
	{
		g_RenderManager.SetTransform( D3DTS_WORLD , &m_matLocal );

		g_RenderManager.SetCullMode( D3DCULL_CCW );
		g_RenderManager.SetZWriteEnable( TRUE );
		g_RenderManager.SetZEnable( TRUE );	
		g_RenderManager.SetAlphaBlendEnable( FALSE );
		CEFArrow::Render( 0 );
	}
	
	return TRUE;
}

int CEFBonArcSkill::SetAEffectBonADecayArrow()
{
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillAttackedSound( m_pExtraInfo->SkillCode , m_pDestChr );
	
	g_Particle.SetEffSetCon( 0.0f , EF_B_ARCHER_DECAY_ARROWED , ESLT_SRC_RAD , 
								 3.1f , m_pDestChr->GetUnique() , m_pDestChr->GetIndex(), 
								 ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr), 
								 m_pSrcChr->GetDirection() ); 

	if(m_pExtraInfo)
	{
		m_pExtraInfo->AfterEffect = EF_B_ARCHER_DECAY_ARROWED;
		m_pExtraInfo->DestType = ESLT_SRC_RAD;
		m_pExtraInfo->lifeTime = 3.1f;
		m_pExtraInfo->fRadian = m_pSrcChr->GetDirection();
	}
	
	g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , m_pDestChr , m_pExtraInfo );	
	
	return TRUE;
}

void CEFBonArcSkill::InitBonAPainArrow()
{
	m_Src_Ani_Index = m_pSrcChr->m_ani_index;
	
	g_Particle.SetEffSetCon( 0.0f , EF_B_ARCHER_PAIN_ARROW, ESLT_CHARWHOLELINK , 
							 EFFECT_SET_LOOP_TIME , m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex() );
}

int CEFBonArcSkill::CheckFrameBonAPainArrow()
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

	BOOL bIsAction = FALSE;
	
	if( m_Src_Ani_Index != m_pSrcChr->m_ani_index )
	{
		bIsAction = TRUE;
	}

	if( m_pSrcChr->m_ani_frame == 29 )
	{
		bIsAction = TRUE;
	}
	
	if( bIsAction )
	{
		g_Particle.DelEffSetCon( EF_B_ARCHER_PAIN_ARROW , ESLT_CHARWHOLELINK , 
								 m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex(), TRUE );
		
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
		
		m_InitTime    = g_nowTime;

		if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( m_pExtraInfo->SkillCode , m_pSrcChr );
	}

	return TRUE;
}

int CEFBonArcSkill::UpdateBonAPainArrow(float dtime)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr , m_pDestChr ) )
	{
		g_Particle.DelEffSetCon( EF_B_ARCHER_PAIN_ARROW , ESLT_CHARWHOLELINK , 
								 m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex() );
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
	
	float       Range       = ( ( ( float ) ( SAFE_TIME_SUB( g_nowTime , m_InitTime ) ) / ( float ) m_RangeTime ) );
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

	g_Particle.DisplayUpdate( 24568 , vPos );	
	
	return TRUE;
}

int CEFBonArcSkill::RenderBonAPainArrow(float dtime)
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
	
	return TRUE;
}

int CEFBonArcSkill::SetAEffectBonAPainArrow()
{
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillAttackedSound( m_pExtraInfo->SkillCode , m_pDestChr );	

	g_Particle.SetEffSetCon( 0.0f , EF_B_ARCHER_PAIN_ARROWED , ESLT_SRC_RAD , 
							 3.1f , m_pDestChr->GetUnique() , m_pDestChr->GetIndex(), 
							 ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr), 
							 m_pSrcChr->GetDirection() ); 

	if(m_pExtraInfo)
	{
		m_pExtraInfo->AfterEffect = EF_B_ARCHER_PAIN_ARROWED;
		m_pExtraInfo->DestType = ESLT_SRC_RAD;
		m_pExtraInfo->lifeTime = 3.1f;
		m_pExtraInfo->fRadian = m_pSrcChr->GetDirection();
	}
	
	g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , m_pDestChr , m_pExtraInfo );

	return TRUE;
}

void CEFBonArcSkill::InitBonADarkArrow()
{
	m_Src_Ani_Index = m_pSrcChr->m_ani_index;

	g_Particle.SetEffSetCon( 0.0f , EF_B_ARCHER_DARK_ARROW, ESLT_CHARPOS , 
							 EFFECT_SET_LOOP_TIME , m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex() );

}

int CEFBonArcSkill::CheckFrameBonADarkArrow()
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

	BOOL bIsAction = FALSE;
	
	if( m_Src_Ani_Index != m_pSrcChr->m_ani_index )
	{
		bIsAction = TRUE;
	}

	if( m_pSrcChr->m_ani_frame == 25 )
	{
		bIsAction = TRUE;
	}

	// 임펙트 프레임 화살 나가는 시점
	if( bIsAction )
	{
		g_Particle.DelEffSetCon( EF_B_ARCHER_DARK_ARROW , ESLT_CHARPOS , 
								 m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex(), TRUE );
		
		m_pSrcChr->m_IsApplyVA = TRUE;
		m_pSrcChr->m_IsNextApplyVA = TRUE;
		m_pSrcChr->BoneRender();
		
		m_vStartPos = D3DXVECTOR3( m_pSrcChr->GetRealLHandTM()._41 , 
								   m_pSrcChr->GetRealLHandTM()._42 , 	
								   m_pSrcChr->GetRealLHandTM()._43   );	
			
		D3DXMATRIX matBone;
		m_pDestChr->GetBoneLinkTM( 2 , matBone );

		D3DXVECTOR3 vDestPos( matBone._41 ,
							  matBone._42 ,
							  matBone._43 );

		D3DXVECTOR3 vDistance = vDestPos - m_vStartPos;
		
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

		m_RangeTime	  = PositionToTimer( 50 , m_vStartPos , vDestPos );

		m_InitTime    = g_nowTime;
		
		m_ISShot = TRUE;

		//................................................................................	
		// 발동 사운드
		//................................................................................	
		if( m_pExtraInfo != NULL )
			g_Particle.m_EffectManager.GetSkillImfactSound( m_pExtraInfo->SkillCode , m_pSrcChr );
	}
	
	return TRUE;
}

int CEFBonArcSkill::UpdateBonADarkArrow(float dtime)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr , m_pDestChr ) )
	{
		g_Particle.DelEffSetCon( EF_B_ARCHER_DARK_ARROW , ESLT_CHARPOS , 
								 m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex() );
		SetDestroy(TRUE);

		return TRUE;
	}

	if( !m_ISShot )
	{
		return TRUE;
	}

	D3DXMATRIX matBone;
	m_pDestChr->GetBoneLinkTM( 2 , matBone );

	D3DXVECTOR3 vDestPos( matBone._41 ,
						  matBone._42 ,
						  matBone._43 );
	
	D3DXVECTOR3 vDistance = vDestPos - m_vStartPos;	
	float   Range = ( ( ( float ) ( SAFE_TIME_SUB( g_nowTime , m_InitTime ) ) / ( float ) m_RangeTime ) );

	//............................................................................................................
	// 캐릭터에 충돌 처리 ( 이펙트 .... T.T )
	//............................................................................................................
	if( Range >= 1.0f ) 
	{
		Range = 1.0f;
		SetDestroy();	
		SetAfterEffect();		
		return TRUE;
	}

	D3DXVECTOR3 vPos      = m_vStartPos + ( vDistance * Range );
	GetXAndYRadian(m_fXRadian, m_fYRadian, vPos, vDestPos);

	D3DXMATRIX matRotX, matRotY;

	D3DXMatrixRotationX( &matRotX , m_fXRadian );
	D3DXMatrixRotationY( &matRotY , m_fYRadian );
	D3DXMatrixMultiply( &m_matLocal , &matRotX , &matRotY );

	m_matLocal._41 = vPos.x;
	m_matLocal._42 = vPos.y;
	m_matLocal._43 = vPos.z;

	g_Particle.DisplayUpdate( 24569 , vPos );	
	
	return TRUE;
}

int	CEFBonArcSkill::RenderBonADarkArrow(float dtime)
{
	// 활 날라 갈때
	if( m_ISShot )
	{
		g_RenderManager.SetTransform( D3DTS_WORLD , &m_matLocal );

		g_RenderManager.SetCullMode( D3DCULL_CCW );
		g_RenderManager.SetZWriteEnable( TRUE );
		g_RenderManager.SetZEnable( TRUE );	
		g_RenderManager.SetAlphaBlendEnable( FALSE );
		CEFArrow::Render( 0 );
	}
	
	return TRUE;
}

int CEFBonArcSkill::SetAEffectBonADarkArrow()
{
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillAttackedSound( m_pExtraInfo->SkillCode , m_pDestChr );	

	g_Particle.SetEffSetCon( 0.0f , EF_B_ARCHER_DARK_ARROWED , ESLT_SRC_RAD , 
							 3.1f , m_pDestChr->GetUnique() , m_pDestChr->GetIndex(), 
							 ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr), 
							 m_pSrcChr->GetDirection() ); 

	if(m_pExtraInfo)
	{
		m_pExtraInfo->AfterEffect = EF_B_ARCHER_DARK_ARROWED;
		m_pExtraInfo->DestType = ESLT_SRC_RAD;
		m_pExtraInfo->lifeTime = 3.1f;
		m_pExtraInfo->fRadian = m_pSrcChr->GetDirection();
	}
	
	g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , m_pDestChr , m_pExtraInfo );

	return TRUE;
}

void CEFBonArcSkill::InitBonACurseArrow()
{
	m_Src_Ani_Index = m_pSrcChr->m_ani_index;
	g_Particle.SetEffSetCon( 0.0f , EF_B_ARCHER_CURSE_ARROW, ESLT_CHARPOS , 
							 EFFECT_SET_LOOP_TIME , m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex() );
	
	m_pSrcChr->SetDirection( m_EffectTargetInfo.vPos.x, m_EffectTargetInfo.vPos.z );
}

int CEFBonArcSkill::CheckFrameBonACurseArrow()
{
	WORD aSkillCode = 0;

	if( m_pExtraInfo != NULL )
	{
		aSkillCode = m_pExtraInfo->SkillCode;
	}	
	
	if( m_ISShot == TRUE )
	{
		return TRUE;
	}	
	
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr ) )
	{
		g_Particle.DelEffSetCon( EF_B_ARCHER_CURSE_ARROW , ESLT_CHARPOS , 
								 m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex(), TRUE );
		m_ISShot = TRUE;

		m_InitTime = g_nowTime;

		// 스킬 범위
		m_EffectDrop.Init( m_EffectTargetInfo.vPos.x , m_EffectTargetInfo.vPos.z , 
						   150.0f , 0.0f , 800 , 1000 , 10 , 
						   g_Particle.m_EffectManager.GetAttackRange( aSkillCode ) , 
						   g_nowTime , 2000 );

		m_ISDamageProcess = TRUE;
	}

	BOOL bIsAction = FALSE;
	
	if( m_Src_Ani_Index != m_pSrcChr->m_ani_index )
	{
		bIsAction = TRUE;
	}

	if( m_pSrcChr->m_ani_frame == 29 )
	{
		bIsAction = TRUE;
	}

	// 임펙트 프레임 화살 나가는 시점
	if( bIsAction )
	{
		g_Particle.DelEffSetCon( EF_B_ARCHER_CURSE_ARROW , ESLT_CHARPOS , 
								 m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex(), TRUE );
		m_ISShot = TRUE;

		//................................................................................	
		// 발동 사운드
		//................................................................................	
		if( m_pExtraInfo != NULL )
			g_Particle.m_EffectManager.GetSkillImfactSound( m_pExtraInfo->SkillCode , m_pSrcChr );

		m_InitTime = g_nowTime;

		// 스킬 범위
		m_EffectDrop.Init( m_EffectTargetInfo.vPos.x , m_EffectTargetInfo.vPos.z , 
						   150.0f , 0.0f , 800 , 1000 , 10 , 
						   g_Particle.m_EffectManager.GetAttackRange( aSkillCode ) , 
						   g_nowTime , 2000 );

		m_ISDamageProcess = TRUE;
	}

	return TRUE;
}

int CEFBonArcSkill::UpdateBonACurseArrow(float dtime)
{
	if( FALSE == m_ISShot )
	{
		return TRUE;
	}	
	
	if( g_Particle.m_EffectManager.CheckDie( m_pSrcChr ) )
	{
		///-- 공격 주체가 죽는다해도 2.1초후까지는 데미지처리를 해준다.	
	}
	
	if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( g_nowTime , m_InitTime ) , > , 2100 ) )
	{
		SetDestroy();
		return TRUE;
	}

	Character			*pTempDest = NULL;
	EFFECT_EXTRA_INFO    Info;
	
	if( m_ISDamageProcess )
	{
		if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( g_nowTime , m_InitTime ) , > , 1000 ) )
		{			
			if( m_pExtraInfo != NULL )
			{
				// 1:N 데미지
				int Count = m_pExtraInfo->GetTargetCount();		
				
				for( int i = 0 ; i < Count ; ++i )
				{
					if( m_pExtraInfo->GetTargetInfo( &pTempDest , &Info , i ) )
					{
						g_Particle.SetEffSetCon( 0.0f , EF_B_ARCHER_CURSE_ARROWED , ESLT_SRC_RAD , 
						 3.1f , pTempDest->GetUnique() , pTempDest->GetIndex(), 
						 ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr), 
						 m_pSrcChr->GetDirection() );				
						
						Info.AfterEffect = EF_B_ARCHER_CURSE_ARROWED;
				        Info.DestType = ESLT_SRC_RAD;
				        Info.lifeTime = 3.1f;
						Info.fRadian = m_pSrcChr->GetDirection();
						
						g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , pTempDest , &Info );
					}

					Info.Clear();
				}
			}
		
			m_ISDamageProcess = FALSE;
		}
	}

	m_EffectDrop.Update( ( DWORD )g_nowTime );	
	
	return TRUE;
}

int CEFBonArcSkill::RenderBonACurseArrow(float dtime)
{
	
	if( m_ISShot )
	{	
		if( m_EffectDrop.GetDataSize() )
		{
			DROP_EFFECT_DATA *pData = NULL;
			D3DXMATRIX		  matLocal;

			D3DXMatrixIdentity( &matLocal );

			m_EffectDrop.StartPosition();

			while( ( pData = m_EffectDrop.GetDropData() ) != NULL )
			{
				if( pData->State == DROPST_DROP	|| pData->State == DROPST_END )
				{
					if( pData->State == DROPST_END )
					{
						g_Particle.SetEffSetCon( 0.0f , EF_B_ARCHER_CURSE_ARROW_LAND , 
												 100 , 2.6f , -1 , -1 , pData->vPos );
					}
					
					matLocal._41 = pData->vPos.x;
					matLocal._42 = pData->vPos.y;
					matLocal._43 = pData->vPos.z;
				
					g_Particle.DisplayUpdate( 24570 , pData->vPos );
				
					g_RenderManager.SetTransform( D3DTS_WORLD , &matLocal );
				
					g_RenderManager.SetCullMode( D3DCULL_CCW );
					g_RenderManager.SetZWriteEnable( TRUE );
					g_RenderManager.SetZEnable( TRUE );
					g_RenderManager.SetAlphaBlendEnable( FALSE );
				
					CEFArrow::Render( 2 );
				}
			}
		}
	}
	
	return TRUE;
}

int CEFBonArcSkill::SetAEffectBonACurseArrow()
{
	return TRUE;
}

void CEFBonArcSkill::InitBonAParalysisArrow()
{
	m_Src_Ani_Index = m_pSrcChr->m_ani_index;
	g_Particle.SetEffSetCon( 0.0f , EF_B_ARCHER_PARALYSIS_ARROW, ESLT_CHARPOS , 
							 EFFECT_SET_LOOP_TIME , m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex() );
}

int	CEFBonArcSkill::CheckFrameBonAParalysisArrow()
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

	BOOL bIsAction = FALSE;
	
	if( m_Src_Ani_Index != m_pSrcChr->m_ani_index )
	{
		bIsAction = TRUE;
	}

	if( m_pSrcChr->m_ani_frame == 49 )
	{
		bIsAction = TRUE;
	}

	if( bIsAction )
	{
		g_Particle.DelEffSetCon( EF_B_ARCHER_PARALYSIS_ARROW , ESLT_CHARPOS , 
								 m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex(), TRUE );
		
		m_pSrcChr->m_IsApplyVA = TRUE;
		m_pSrcChr->m_IsNextApplyVA = TRUE;
		m_pSrcChr->BoneRender();
		
		m_vStartPos = D3DXVECTOR3( m_pSrcChr->GetRealLHandTM()._41 , 
								   m_pSrcChr->GetRealLHandTM()._42 , 	
								   m_pSrcChr->GetRealLHandTM()._43   );	
			
		D3DXMATRIX matBone;
		m_pDestChr->GetBoneLinkTM( 2 , matBone );

		D3DXVECTOR3 vDestPos( matBone._41 ,
							  matBone._42 ,
							  matBone._43 );

		D3DXVECTOR3 vDistance = vDestPos - m_vStartPos;
		
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

		m_RangeTime	  = PositionToTimer( 50 , m_vStartPos , vDestPos );

		m_InitTime    = g_nowTime;
		
		m_ISShot = TRUE;

		//................................................................................	
		// 발동 사운드
		//................................................................................	
		if( m_pExtraInfo != NULL )
			g_Particle.m_EffectManager.GetSkillImfactSound( m_pExtraInfo->SkillCode , m_pSrcChr );
	}
	
	return TRUE;
}

int CEFBonArcSkill::UpdateBonAParalysisArrow(float dtime)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr , m_pDestChr ) )
	{
		g_Particle.DelEffSetCon( EF_B_ARCHER_PARALYSIS_ARROW , ESLT_CHARPOS , 
								 m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex(), TRUE );
		SetDestroy(TRUE);
		return TRUE;
	}

	if( !m_ISShot )
	{
		return TRUE;
	}

	D3DXMATRIX matBone;
	m_pDestChr->GetBoneLinkTM( 2 , matBone );

	D3DXVECTOR3 vDestPos( matBone._41 ,
						  matBone._42 ,
						  matBone._43 );
	
	D3DXVECTOR3 vDistance = vDestPos - m_vStartPos;	
	float   Range = ( ( ( float ) ( SAFE_TIME_SUB( g_nowTime , m_InitTime ) ) / ( float ) m_RangeTime ) );

	//............................................................................................................
	// 캐릭터에 충돌 처리 ( 이펙트 .... T.T )
	//............................................................................................................
	if( Range >= 1.0f ) 
	{
		Range = 1.0f;
		SetDestroy();	
		SetAfterEffect();		
		return TRUE;
	}

	D3DXVECTOR3 vPos      = m_vStartPos + ( vDistance * Range );
	GetXAndYRadian(m_fXRadian, m_fYRadian, vPos, vDestPos);

	D3DXMATRIX matRotX, matRotY;

	D3DXMatrixRotationX( &matRotX , m_fXRadian );
	D3DXMatrixRotationY( &matRotY , m_fYRadian );
	D3DXMatrixMultiply( &m_matLocal , &matRotX , &matRotY );

	m_matLocal._41 = vPos.x;
	m_matLocal._42 = vPos.y;
	m_matLocal._43 = vPos.z;

	g_Particle.DisplayUpdate( 24571 , vPos );	
	
	return TRUE;
}

int CEFBonArcSkill::RenderBonAParalysisArrow(float dtime)
{
	// 활 날라 갈때
	if( m_ISShot )
	{
		g_RenderManager.SetTransform( D3DTS_WORLD , &m_matLocal );

		g_RenderManager.SetCullMode( D3DCULL_CCW );
		g_RenderManager.SetZWriteEnable( TRUE );
		g_RenderManager.SetZEnable( TRUE );	
		g_RenderManager.SetAlphaBlendEnable( FALSE );
		CEFArrow::Render( 0 );
	}
	
	return TRUE;
}

int CEFBonArcSkill::SetAEffectBonAParalysisArrow()
{
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillAttackedSound( m_pExtraInfo->SkillCode , m_pDestChr );	

	g_Particle.SetEffSetCon( 0.0f , EF_B_ARCHER_PARALYSIS_ARROWED , ESLT_SRC_RAD , 
							 3.1f , m_pDestChr->GetUnique() , m_pDestChr->GetIndex(), 
							 ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr), 
							 m_pSrcChr->GetDirection() );
	if(m_pExtraInfo)
	{
		m_pExtraInfo->AfterEffect = EF_B_ARCHER_PARALYSIS_ARROWED;
		m_pExtraInfo->DestType = ESLT_SRC_RAD;
		m_pExtraInfo->lifeTime = 3.1f;
		m_pExtraInfo->fRadian = m_pSrcChr->GetDirection();
	}
	
	g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , m_pDestChr , m_pExtraInfo );

	return TRUE;
}
	
void CEFBonArcSkill::InitBonBRandomArrow()
{
	m_Src_Ani_Index = m_pSrcChr->m_ani_index;
	g_Particle.SetEffSetCon( 0.0f , EF_B_BOS_ARCHER_RANDOM_ARROW, ESLT_CHARPOS , 
							 EFFECT_SET_LOOP_TIME , m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex() );
}

int CEFBonArcSkill::CheckFrameBonBRandomArrow()
{
	if( m_ISShot == TRUE )
	{
		return TRUE;
	}	
	
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr ) )
	{
		g_Particle.DelEffSetCon( EF_B_BOS_ARCHER_RANDOM_ARROW, ESLT_CHARPOS , 
								 m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex(), TRUE );
		SetDestroy(TRUE);
		return TRUE;
	}

	BOOL bIsAction = FALSE;
	
	if( m_Src_Ani_Index != m_pSrcChr->m_ani_index )
	{
		bIsAction = TRUE;
	}

	if( m_pSrcChr->m_ani_frame == 25 )
	{
		bIsAction = TRUE;
	}

	if( bIsAction )
	{
		g_Particle.DelEffSetCon( EF_B_BOS_ARCHER_RANDOM_ARROW, ESLT_CHARPOS , 
								 m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex(), TRUE );
		
		m_pSrcChr->m_IsApplyVA = TRUE;
		m_pSrcChr->m_IsNextApplyVA = TRUE;
		m_pSrcChr->BoneRender();		
		//................................................................................	
		// 발동 사운드
		//................................................................................	
		if( m_pExtraInfo != NULL )
			g_Particle.m_EffectManager.GetSkillImfactSound( m_pExtraInfo->SkillCode , m_pSrcChr );	
		
		m_InitTime = g_nowTime;
		m_PrevTime = g_nowTime;
		m_GapTime = 0;

		// 스킬 범위
		m_InitTime = g_nowTime;

		WORD aSkillCode = 0;
		
		if(m_pExtraInfo)
		{
		   aSkillCode = m_pExtraInfo->SkillCode;
		   m_fRadius = g_Particle.m_EffectManager.GetAttackRange( m_pExtraInfo->SkillCode );	   
		}	

		D3DXVECTOR3 vDir, vStart, vEnd;
		vStart = D3DXVECTOR3(  m_pSrcChr->GetRealLHandTM()._41 , 
			     			   m_pSrcChr->GetRealLHandTM()._42 , 	
							   m_pSrcChr->GetRealLHandTM()._43   ); 		
		
		vEnd.x = m_EffectTargetInfo.vPos.x;
		vEnd.z = m_EffectTargetInfo.vPos.z;
		vEnd.y = 0.0f;	

		float fDelHeight = vStart.y - g_Map.PHF_GetHeight( vStart.x , vStart.z );
		D3DXVECTOR3 vTemp = vStart;
		vTemp.y = 0.0f;

		vDir = vEnd - vTemp;
		D3DXVec3Normalize(&vDir, &vDir);
		vEnd += (vDir * 20.0f);	

		float fLenght = D3DXVec3Length(&(vEnd - vStart));

		D3DXVECTOR3 vRightV;
		D3DXVECTOR3 vUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		D3DXVec3Cross(&vRightV, &vUp, &vDir);

		D3DXVECTOR3 vLeft, vRight;

		vLeft = vEnd - (vRightV * m_fRadius);
		vRight = vEnd + (vRightV * m_fRadius);
		
		D3DXVECTOR3 vDistance = vLeft - vTemp;
		D3DXVec3Normalize(&vDistance, &vDistance);
		float fsRadian =   D3DXVec3Dot( &vDistance , &D3DXVECTOR3(0.0f, 0.0f, -1.0f) );
		fsRadian = acos((double)fsRadian);
		if(vDistance.x > 0.0f)
		{
		   fsRadian = (2.0f * D3DX_PI) - fsRadian;
		}
			
		D3DXVECTOR3 vDistance2 = vRight - vTemp;
		D3DXVec3Normalize(&vDistance2, &vDistance2);
		float fDelRadian = D3DXVec3Dot( &vDistance , &vDistance2 );
		fDelRadian = acos((double)fDelRadian);

		D3DXMATRIX matRot;

		for(int i = 0; i < 7 ; i++)
		{
			//실제가 각도는 = 스케일 + 왼쪽과 오른쪽 사이 각 도  * (총 7발 / 6.0?)
			float fRadian = fsRadian + fDelRadian * (i / 6.0f);
			//Tops 왼쪽부터 - 스타트 지점 
			D3DXVECTOR3 vTPos = vLeft - vTemp;
			// Y 축으로 회전하는데 얼마만큼 회전할것이냐 ? 
			D3DXMatrixRotationY( &matRot , fDelRadian * (i / 6.0f) );
			//시작점 에서 최대한 움직이는 Y행열 축 값 ? 
			D3DXVec3TransformCoord( &vTPos , &vTPos , &matRot );
			// 벡터 거리 + 스타트 값 최정 End 값인가 ? 
			vTPos += vTemp;
			//각 화살의 스타 지점은 = 캐릭터의 스타트 지점이다 . 
			m_vStartPoses[i] = vStart;
			//각 화살의 마지막 지점은 vTPos
			m_vEndPos[i] = vTPos;
			//맵의 높이값 + (맵과  + 캐릭터 왼손의 높이를 증가 시켜준다 . 
			m_vEndPos[i].y = g_Map.PHF_GetHeight( vEnd.x , vEnd.z ) + fDelHeight;
			//왜 방향을 다시 구하지 ? = 왼쪽 점 - 스타트 지점 
			D3DXVECTOR3 vDir = m_vEndPos[i] - m_vStartPoses[i];
			//방향을 다시 단위로 만든다 왜 ?
			D3DXVec3Normalize(&vDir, &vDir);
			//시작 	+  각지점까지의 방향을 *스케일값 
			m_vStartPoses[i] += (vDir * 11.0f);
			//w전역 방향값은 = 현재 각화살의 방향이다 .
			m_vDir[i] = vDir;
			// Y축 각도는 현재 어디인다 .. ?
			m_fYRadians[i] = fRadian;
		}

		vEnd.y = g_Map.PHF_GetHeight( vEnd.x , vEnd.z ) + fDelHeight;
		m_RangeTime	= PositionToTimer( 50 , vStart , vEnd );
		m_ISShot = TRUE;	

		float fYRadian = 0.0f;
		m_fXRadian = 0.0f;
		
		GetXAndYRadian(m_fXRadian, fYRadian, vStart, vEnd);
	}	
	
	return TRUE;
}

int CEFBonArcSkill::UpdateBonBRandomArrow(float dtime)
{
	Character			*pTempDest = NULL;
	EFFECT_EXTRA_INFO    Info;
	
	if( !m_ISShot )
	{
		if( g_Particle.m_EffectManager.CheckDie( m_pSrcChr ) )
		{
			g_Particle.DelEffSetCon( EF_B_BOS_ARCHER_RANDOM_ARROW, ESLT_CHARPOS , 
									 m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex(), TRUE );
			
			SetDestroy(TRUE);
			return TRUE;
		}
	}	
	
	return TRUE;
}

int CEFBonArcSkill::RenderBonBRandomArrow(float dtime)
{
	if( m_ISShot )
	{
		D3DXMATRIX		  matLocal;
		D3DXMatrixIdentity( &matLocal );
		D3DXMATRIX		  matIden;
		D3DXMatrixIdentity( &matIden );

		g_RenderManager.SetZEnable( D3DZB_TRUE );	
		g_RenderManager.SetAlphaBlendEnable( TRUE );

		float  Range = ( ( ( float ) ( SAFE_TIME_SUB( g_nowTime , m_InitTime ) ) / ( float ) m_RangeTime ) );

		if( Range >= 1.0f )
		{
			Range = 1.0f;
			SetAfterEffect();
			SetDestroy();		
			return TRUE;
		}

		D3DXMATRIX	m_matRotX, m_matRotY;
		
		for(int i = 0; i < 7 ; i++)
		{
			D3DXMatrixRotationX( &m_matRotX , m_fXRadian );
			D3DXMatrixRotationY( &m_matRotY , m_fYRadians[i] );
			D3DXMatrixMultiply( &matLocal , &m_matRotX , &m_matRotY );

			D3DXVECTOR3 vPos;
			D3DXVec3Lerp( &vPos , &m_vStartPoses[i] , &m_vEndPos[i] , Range );
			matLocal._41 = vPos.x;
			matLocal._42 = vPos.y;
			matLocal._43 = vPos.z;

			g_RenderManager.SetCullMode( D3DCULL_CCW );
			g_RenderManager.SetZWriteEnable( TRUE );			
			g_RenderManager.SetTransform( D3DTS_WORLD , &matLocal );

			CEFArrow::Render( 0 );

			g_RenderManager.SetCullMode( D3DCULL_NONE );
			g_RenderManager.SetZWriteEnable( FALSE );

			g_RenderManager.SetTransform( D3DTS_WORLD , &matIden );		
			g_Particle.DisplayUpdate( 24571 , (vPos- (m_vDir[i] * 5.0f)) );
		}

		g_RenderManager.SetCullMode( D3DCULL_NONE );
		g_RenderManager.SetZWriteEnable( FALSE );
	}
	
	g_RenderManager.SetCullMode( D3DCULL_NONE );
	g_RenderManager.SetZWriteEnable( FALSE );
	g_RenderManager.SetAlphaBlendEnable( FALSE );
	g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
	g_RenderManager.SetDestBlend( D3DBLEND_INVSRCALPHA );
	
	return TRUE;
}

int CEFBonArcSkill::SetAEffectBonBRandomArrow()
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
				g_Particle.SetEffSetCon( 0.0f , EF_B_BOS_ARCHER_RANDOM_ARROWED , ESLT_SRC_RAD , 
						 3.1f , pTempDest->GetUnique() , pTempDest->GetIndex(), 
						 ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr), 
						 m_pSrcChr->GetDirection() );				
				
				Info.AfterEffect = EF_B_BOS_ARCHER_RANDOM_ARROWED;
				Info.DestType = ESLT_SRC_RAD;
				Info.lifeTime = 3.1f;
				Info.fRadian = m_pSrcChr->GetDirection();
				
				g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , pTempDest , &Info );	
			}

			Info.Clear();
		}
	}

	return TRUE;
}
