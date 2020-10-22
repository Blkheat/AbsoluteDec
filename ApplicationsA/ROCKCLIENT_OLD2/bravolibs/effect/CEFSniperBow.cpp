#include "CEFSniperBow.h"
#include "CEFArrow.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include	<SOUND\Sound.H>
#include "..\\..\\CRenderManager.h"

extern long g_nowTime;

CEFSniperBow::CEFSniperBow()
{
	m_ISShot = FALSE;
	m_IsConEffect = TRUE;

	m_fXRadian = 0.0f;
	m_fYRadian = 0.0f;
}


CEFSniperBow::~CEFSniperBow()
{

}

int CEFSniperBow::CheckFrame(void)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr ) )
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

	// ����Ʈ ������ ȭ�� ������ ����
	if( m_Src_Ani_Index != m_pSrcChr->m_ani_index || GetSrcAniCount() == 56 )
	{
		m_pSrcChr->m_IsApplyVA = TRUE;
		m_pSrcChr->m_IsNextApplyVA = TRUE;
		m_pSrcChr->BoneRender();
		
		m_vStartPos   = D3DXVECTOR3( m_pSrcChr->GetRealLHandTM()._41 , 
									 m_pSrcChr->GetRealLHandTM()._42 , 	
									 m_pSrcChr->GetRealLHandTM()._43   ); 
		
		D3DXMATRIX matBone;
		m_pDestChr->GetBoneLinkTM( 2 , matBone );
		
		D3DXVECTOR3 vDestPos( matBone._41 ,
							  matBone._42 ,
							  matBone._43   );
		
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
		
		m_OldTimer    = g_nowTime;
		
		g_Particle.SetEffSetCon( 0.0f , EF_BOW_SPOT , 0 , 2.0f , m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex() );
		
		m_ISShot	  = TRUE;
		
		if( m_pExtraInfo != NULL )
			g_Particle.m_EffectManager.GetSkillImfactSound( m_pExtraInfo->SkillCode , m_pSrcChr );
	}

	return TRUE;
}


void CEFSniperBow::Init(void)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr , m_pDestChr ) )
	{
		SetDestroy(TRUE);
		return;
	}
	
	m_Src_Ani_Index = m_pSrcChr->m_ani_index;
	
	///-- �̰����� ��ų�� ������ �˾ƿ�
	SSkillBaseInfo* skill_Info;
	skill_Info = g_Pc_Manager.GetSkillBaseInfoPtr( m_pSrcChr->m_curt_event.count );
	if( skill_Info != NULL )
	{
		if(skill_Info->theNeedSpelling)
		{
#ifdef SNIPER_MOD_MOTION			
			m_pSrcChr->m_ani_frame = 56 - 10;
#else
			m_pSrcChr->m_ani_frame = 36;
#endif			
		}
	}

	g_Particle.SetEffSetCon( 0.0f , EF_SNIPER_BOW , ESLT_CHARPOS , 
		EFFECT_SET_LOOP_TIME , m_pDestChr->GetUnique() , m_pDestChr->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr) );
}

int	CEFSniperBow::Render(float dtime)
{
	// Ȱ ���� ����
	if( m_ISShot )
	{
		g_RenderManager.SetTransform( D3DTS_WORLD , &m_matLocal );
		
		g_RenderManager.SetCullMode( D3DCULL_CCW );
		
		g_RenderManager.SetZWriteEnable( TRUE );
		g_RenderManager.SetZEnable( TRUE );
		
		g_RenderManager.SetAlphaBlendEnable(  FALSE );

		CEFArrow::Render( 0 );
	}
	// ��� ���۽� ǥ�� ���
	else
	{
		if(m_pSrcChr->m_pre_ani_number != -1)
		{
			return TRUE;
		}		
		
		// ����Ʈ �������� 1 �̶� ���⼭ ����ش� ����
		g_RenderManager.SetTransform( D3DTS_WORLD, &m_pSrcChr->GetRHandTM() );
								
		g_RenderManager.SetCullMode( D3DCULL_CCW );
		
		g_RenderManager.SetZWriteEnable( TRUE );
		g_RenderManager.SetZEnable( TRUE );
		
		g_RenderManager.SetAlphaBlendEnable(  FALSE );

		g_Particle.m_ArrowDisplay.Render( g_lpDevice );
	}

	return TRUE;
}

int CEFSniperBow::Update(float dtime)
{
	if( g_Particle.m_EffectManager.CheckDie( m_pSrcChr ) )
	{
		SetDestroy(TRUE);
		return TRUE;
	}
	
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr , m_pDestChr ) )
	{
		SetDestroy(TRUE);
		return TRUE;
	}

	if( m_ISShot )
	{
		D3DXMATRIX matBone;
		m_pDestChr->GetBoneLinkTM( 2 , matBone );
		
		D3DXVECTOR3 vDestPos( matBone._41 ,
							  matBone._42 ,
							  matBone._43 );
		
		D3DXVECTOR3 vDistance = vDestPos - m_vStartPos;
		
		float       Range     = ( ( ( float ) SAFE_TIME_SUB( g_nowTime , m_OldTimer ) / ( float ) m_RangeTime ) );
		
		//............................................................................................................
		// ĳ���Ϳ� �浹 ó�� ( ����Ʈ .... T.T )
		//............................................................................................................
		if( Range >= 1.0f ) 
		{
			Range = 1.0f;
			
			g_Particle.DelEffSetCon( EF_SNIPER_BOW , ESLT_CHARPOS , 
				m_pDestChr->GetUnique() , m_pDestChr->GetIndex() );

			g_Particle.m_EffectManager.CreateArrowStuck(
				m_pDestChr,
				m_pSrcChr->m_radian,
				D3DXVECTOR3( RANDOM_NUM , RANDOM_NUM , RANDOM_NUM ) ,
				2 );
			
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
	}

	return TRUE;
}

int CEFSniperBow::SetAfterEffect(void)

{
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillAttackedSound( m_pExtraInfo->SkillCode , m_pDestChr );

	g_Particle.SetEffSetCon( 0.0f , EF_SNIPER_BOW_ED , 0 , 2.0f , 
		m_pDestChr->GetUnique() , m_pDestChr->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr) );

	if(m_pExtraInfo)
	{
		m_pExtraInfo->AfterEffect = EF_SNIPER_BOW_ED;
		m_pExtraInfo->DestType = 0;
		m_pExtraInfo->lifeTime = 2.0f;
	}
	
	g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , m_pDestChr , m_pExtraInfo );

	return TRUE;
}
