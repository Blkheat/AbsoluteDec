#include "CEFBeeLineBow.h"
#include "CEFArrow.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include	<SOUND\Sound.H>
#include "..\\..\\CRenderManager.h"

extern long g_nowTime;

CEFBeeLineBow::CEFBeeLineBow()
{
	m_ISShot = FALSE;
	m_IsConEffect = TRUE;
	m_DamageEffectType = ESLT_CHARPOS;

	m_fXRadian = 0.0f;
	m_fYRadian = 0.0f;

	m_dNpcRealFileCode = 0;
}


CEFBeeLineBow::~CEFBeeLineBow()
{

}

void CEFBeeLineBow::SetExtra(int TailNum,int DamageNum,DWORD Time,float DelayTime,int DamageType)
{
	m_TailParticleNum = TailNum;
	m_DamageEffectNum = DamageNum;
	m_SpeedToOneTime  = Time;			
	m_EffectDelayTime = DelayTime;
	m_DamageEffectType = DamageType;
}

int CEFBeeLineBow::CheckFrame(void)
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
			nCheckFrame = 25;
		}
	}

	// 임펙트 프레임 화살 나가는 시점
	if( m_Src_Ani_Index != m_pSrcChr->m_ani_index || GetSrcAniCount() == nCheckFrame )
	{
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

		m_RangeTime	  = PositionToTimer( m_SpeedToOneTime , m_vStartPos , vDestPos );

		m_OldTimer    = g_nowTime;
		
		m_ISShot = TRUE;

		//................................................................................	
		// 발동 사운드
		//................................................................................	
		if( m_pExtraInfo != NULL )
			g_Particle.m_EffectManager.GetSkillImfactSound( m_pExtraInfo->SkillCode , m_pSrcChr );
	}

	return TRUE;
}

void CEFBeeLineBow::Init(void)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr , m_pDestChr ) )
	{
		SetDestroy(TRUE);		
		return;
	}

	m_Src_Ani_Index = m_pSrcChr->m_ani_index;

	///--PRINT_DLG_LOG( "스킬 발동 m_pSrcChr->m_ani_frame = %d ", m_pSrcChr->m_ani_frame);

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
			m_pSrcChr->m_ani_frame = 15;
#endif
		}
	}
}

int	CEFBeeLineBow::Render(float dtime)
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

int CEFBeeLineBow::Update(float dtime)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr , m_pDestChr ) )
	{
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
	
	float       Range     = ( ( ( float ) ( SAFE_TIME_SUB( g_nowTime , m_OldTimer ) ) / ( float ) m_RangeTime ) );

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

	if( m_TailParticleNum != -1 )
		g_Particle.DisplayUpdate( m_TailParticleNum , vPos );

	return TRUE;
}

int CEFBeeLineBow::SetAfterEffect(void)
{
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillAttackedSound( m_pExtraInfo->SkillCode , m_pDestChr );
	
	g_Particle.SetEffSetCon( 0.0f , m_DamageEffectNum , m_DamageEffectType , 
		m_EffectDelayTime , m_pDestChr->GetUnique() , m_pDestChr->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr) ); 

	if(m_pExtraInfo)
	{
		m_pExtraInfo->AfterEffect = m_DamageEffectNum;
		m_pExtraInfo->DestType = 0;
		m_pExtraInfo->lifeTime = m_EffectDelayTime;
	}
	
	g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , m_pDestChr , m_pExtraInfo );

	return TRUE;
}
