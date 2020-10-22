#include "CEFWrathKniteAttack.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include	<SOUND\Sound.H>
#include "..\\..\RockClient.h"
#include "..\\..\\map.h"
#include "..\\..\\CRenderManager.h"

extern long		 g_nowTime;
	 
CEFWrathKniteAttack::CEFWrathKniteAttack()
{
	m_dNpcRealFileCode = 0;
}

CEFWrathKniteAttack::~CEFWrathKniteAttack()
{
	m_pSrcChr->SetAniTime(c_Stand_AniTime);
}

void CEFWrathKniteAttack::Init(void)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr , m_pDestChr ) )
	{
		SetDestroy();
		return;
	}
	
	float fAniTime = c_Stand_AniTime * 0.6f;
	m_pSrcChr->SetAniTime(fAniTime);

	m_Src_Ani_Index = m_pSrcChr->m_ani_index;

	//................................................................................	
	// 발동 사운드
	//................................................................................	
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( m_pExtraInfo->SkillCode , m_pSrcChr );
	
	if( !m_pSrcChr->m_bPC && m_pSrcChr->m_State.nCharacterType != nCharacterTypePlayerNPC )
	{
		m_dNpcRealFileCode = g_Pc_Manager.GetNPCRealFileCode( g_Map.m_NpcTable[ m_pSrcChr->m_nTableIndex ].code );
	}
}

int CEFWrathKniteAttack::CheckFrame(void)
{
	if(m_State == ES_SELF_DESTROY)
	{
		return TRUE;
	}

	if( m_Src_Ani_Index != m_pSrcChr->m_ani_index )
	{
		g_Particle.SetEffSetCon( 0.0f , EF_WRATH_KNIFE_ATTACK_ED , ESLT_CHARPOS , 
								 6.1f , m_pDestChr->GetUnique() , m_pDestChr->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr) );
		SetAfterEffect();
		SetDestroy();
		return TRUE;
	}
	
	int nCheckFrame = 28;
	D3DXVECTOR3 vLinkPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	if( m_dNpcRealFileCode )
	{
		if( m_dNpcRealFileCode == 21100020 )
		{
			nCheckFrame = 69;		
			
			if( GetSrcAniCount() == nCheckFrame )
			{
				D3DXMATRIX  mat;		
				m_pSrcChr->GetBoneLinkTM( 4 , mat );			///-- 왼손
				vLinkPos = D3DXVECTOR3(mat._41, mat._42, mat._43);

				D3DXMATRIX matRotY;
				D3DXVECTOR3 vDir = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
				D3DXMatrixRotationY( &matRotY , m_pSrcChr->GetDirection() + D3DXToRadian( 8 ) );
				D3DXVec3TransformCoord( &vDir , &vDir , &matRotY );

				vLinkPos += ( 20.0f * vDir );
			}
		}
	}
	else
	{
		nCheckFrame = 28;
		
		if( GetSrcAniCount() == nCheckFrame )
		{
			D3DXMATRIX  mat;		
			m_pSrcChr->GetBoneLinkTM( 8 , mat );				///--칼끝
			vLinkPos = D3DXVECTOR3(mat._41, mat._42, mat._43);
		}
	}

	if( m_dNpcRealFileCode )
	{
		if( m_dNpcRealFileCode == 21100020 )
		{
			BOOL IsEffect = FALSE;
			
			if( GetSrcAniCount() == 15 )
			{
				IsEffect = TRUE;
			}
			
			if( GetSrcAniCount() == 36 )
			{
				IsEffect = TRUE;
			}

			if( IsEffect )
			{
				g_Particle.SetEffSetCon( 0.0f , EF_WRATH_KNIFE_ATTACK_ED , ESLT_CHARPOS , 
										 6.1f , m_pDestChr->GetUnique() , m_pDestChr->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr) );

				//................................................................................	
				// 어택 사운드
				//................................................................................	
				if( m_pExtraInfo != NULL )
					g_Particle.m_EffectManager.GetSkillAttackedSound( m_pExtraInfo->SkillCode , m_pDestChr );
			}
		}
	}
	
	if( GetSrcAniCount() == nCheckFrame )
	{
		m_pSrcChr->m_IsApplyVA = TRUE;
		m_pSrcChr->m_IsNextApplyVA = TRUE;
		m_pSrcChr->BoneRender();	
		
		g_Particle.SetEffSetCon( 0.0f , EF_WRATH_KNIFE_ATTACK , ESLT_POS , 6.1f , 
								-1 , -1 , vLinkPos,	m_pSrcChr->GetDirection() + D3DXToRadian( 8 ) );
	}
	
	if( GetSrcAniCount() == nCheckFrame )
	{
		g_Particle.SetEffSetCon( 0.0f , EF_WRATH_KNIFE_ATTACK_ED , ESLT_CHARPOS , 
								 6.1f , m_pDestChr->GetUnique() , m_pDestChr->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr) );

		//................................................................................	
		// 어택 사운드
		//................................................................................	
		if( m_pExtraInfo != NULL )
			g_Particle.m_EffectManager.GetSkillAttackedSound( m_pExtraInfo->SkillCode , m_pDestChr );


		SetAfterEffect();
		SetDestroy();
	}	
	
	return TRUE;
}

int CEFWrathKniteAttack::Update(float dtime)
{
	return TRUE;
}

int	CEFWrathKniteAttack::Render(float dtime)
{
	return TRUE;
}

int CEFWrathKniteAttack::SetAfterEffect(void)
{	
	if( m_pExtraInfo )
	{
		m_pExtraInfo->AfterEffect = EF_WRATH_KNIFE_ATTACK_ED;
		m_pExtraInfo->DestType = ESLT_CHARPOS;
		m_pExtraInfo->lifeTime = 6.1f;
	}
	
	g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , m_pDestChr , m_pExtraInfo );

	return TRUE;
}
