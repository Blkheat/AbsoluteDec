#include "CEffectBase.h"
#include "Windows.h"
#include "..\\..\\Effect.h"

extern long g_nowTime;

D3DXVECTOR3 GetCharPos(D3DXVECTOR3 vPos, Character * pSrc)
{
	if(pSrc)
	{
		if( pSrc->m_NowAInfo.dAttackType == SM_ATTACK_NPC )
		{
			g_Particle.m_pAttackUnit = pSrc;
			g_Particle.m_IsDirAttatack = TRUE;		

			return vPos;
		}
		else if( pSrc->m_NowAInfo.dAttackType == SM_USE_SKILL )
		{
			SSkillBaseInfo* skill_Info;			
			skill_Info = g_Pc_Manager.GetSkillBaseInfoPtr( pSrc->m_NowAInfo.dSkillCode );			
			if( skill_Info )	
			{
				if( skill_Info->theActTarget == n_AT_TARGETLAND )
				{
					g_Particle.m_pAttackUnit = pSrc;
					return vPos;
				}				
				else if( skill_Info->theSkillType2 == n_DeathBlow || 
						 skill_Info->theSkillType2 == n_AttackMagic || 
						 skill_Info->theSkillType2 == n_NoFrameDeathBlow )
				{
					g_Particle.m_pAttackUnit = pSrc;
					g_Particle.m_IsDirAttatack = TRUE;
					return vPos;
				}
			}
		}		
	}
	
	return vPos;
}

//................................................................................................................
// Create class
//................................................................................................................
CEffectBase::CEffectBase()
{
	m_State      = ES_PLAY;

	m_pSrcChr    = NULL;
	m_pDestChr   = NULL;

	m_pExtraInfo = NULL;
	m_ISLandLink = FALSE;
	m_ISLinkItem = TRUE;
	
	m_SkillUnique    = 0;
	m_SrcImpactFrame = -1;
	m_SrcAniFrame    = -1;

	m_SceneMode		 = g_RockClient.m_SceneManager.GetEffectCreateMode();

	m_ISLinkItem  = FALSE;
	m_IsConEffect = FALSE;
	
	m_Src_Ani_Index = 0;
	m_HashCode = 0;

	m_ISDamageAct = FALSE;
	m_ISBasicAttack = FALSE;
	m_SkillCode = 0;

	m_dNpcRealFileCode = 0;	
	m_IsAfterSortRender = FALSE;
}

//................................................................................................................
// Destroy class
//................................................................................................................
CEffectBase::~CEffectBase()
{
	m_pSrcChr  = NULL;
	m_pDestChr = NULL;
	
	SAFE_DELETE( m_pExtraInfo );

	Destroy();
}

//................................................................................................................
// Check Frame 시 에니메이션 카운트
//................................................................................................................
void CEffectBase::AddSrcAniFrameCount(void)
{
	// PushEffect 에서 캐릭터 쪽 임팩트 프레임 얻어오지 못한것은 카운트 하지 않하기
	if( m_SrcImpactFrame < 0 )
		return;
	
	// 지속 스킬시 빡 방지
	if( m_SrcAniFrame >= 10000 )
		return;

	m_SrcAniFrame++;
}

//................................................................................................................
// 소스의 에니메이션 카운트
//................................................................................................................
int CEffectBase::GetSrcAniCount(void)
{
	return m_SrcAniFrame;
}

//................................................................................................................
// 소스의 임펙트 프레임
//................................................................................................................
int CEffectBase::GetSrcImpactFrame(void)
{
	return m_SrcImpactFrame;
}


//................................................................................................................
// Setup
//................................................................................................................
int CEffectBase::Setup(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo)
{
	m_pSrcChr  = pSrc;
	m_pDestChr = pDest;
	m_ID	   = id;

	///--JAPAN_BUG_MODIFY
	if( pSrc )
	{
		m_SkillCode = pSrc->m_curt_event.dKeepSkillCode;
	}
	
	if( pExtraInfo != NULL )
	{
		m_SkillCode = pExtraInfo->SkillCode;
		
		m_pExtraInfo = SAFE_NEW( EFFECT_EXTRA_INFO );		

		// memcpy( m_pExtraInfo , pExtraInfo , sizeof( EFFECT_EXTRA_INFO ) );		
		m_pExtraInfo->pDest		 = pDest;
		m_pExtraInfo->EventType  = pExtraInfo->EventType;
		m_pExtraInfo->Critical   = pExtraInfo->Critical;
		m_pExtraInfo->SkillCode  = pExtraInfo->SkillCode;
		m_pExtraInfo->lStartTime = pExtraInfo->lStartTime;		
		
		memcpy( &m_pExtraInfo->TargetInfo , &pExtraInfo->TargetInfo , sizeof( struct _STargetInfo ) );

		for( int i = 0 ; i < pExtraInfo->SubExtraInfoList.size() ; ++i )
		{
			m_pExtraInfo->SubInit( pExtraInfo->SubExtraInfoList[ i ].pDest     , 
				     pExtraInfo->SubExtraInfoList[ i ].EventType ,
					 pExtraInfo->SubExtraInfoList[ i ].Critical  ,
					 pExtraInfo->SubExtraInfoList[ i ].SkillCode ,
					 pExtraInfo->SubExtraInfoList[ i ].TargetInfo  );
		}

		m_SkillUnique = pExtraInfo->SkillUnique;
	}
	
	//.......................................................................................................
	// 이펙트 매니저에 스킬 타입및 셀 값을 입력 
	// 광역 마법시 아이디로 사용함 -> Unique , Race , Skill Code , Cell
	//.......................................................................................................
	switch( g_Particle.m_EffectManager.m_TargetInfo.TargetType )
	{
		case n_PC:
			SetPC();	
			break;
		case n_NPC:
			SetNPC();	
			break;
		case n_Cell:
			SetCell( g_Particle.m_EffectManager.m_TargetInfo.CellX , g_Particle.m_EffectManager.m_TargetInfo.CellY , g_Particle.m_EffectManager.m_TargetInfo.vPos );
			break;
	}
	//.......................................................................................................

	Init();

	return TRUE;
}

void CEffectBase::SetDestroy(BOOL ISDamageAct, BOOL ISBasicAttack)
{ 
	m_State = ES_SELF_DESTROY;

	if( m_ID == EF_DOGMA_L1_FIRERAIN ||
		m_ID == EF_DOGMA_L1_FRAME_STORM )
	{
		int a = 0;
	}

	if( ISDamageAct && !m_ISDamageAct )
	{
		if( m_pExtraInfo )
		{
			if(ISBasicAttack)
			{
				g_Particle.m_EffectManager.DestStandTargetHit( m_pDestChr , m_pExtraInfo );	
			}
			else
			{
				Character			*pTempDest = NULL;
				EFFECT_EXTRA_INFO    Info;
			
				int Count = m_pExtraInfo->GetTargetCount();
				for( int i = 0 ; i < Count ; ++i )
				{
					if( m_pExtraInfo->GetTargetInfo( &pTempDest , &Info , i ) )
					{
						g_Particle.m_EffectManager.DestTargetHit(pTempDest , &Info );
					}

					Info.Clear();
				}
			}
		}
		
		m_ISDamageAct = TRUE;
	}	
}

//................................................................................................................
// Render
//................................................................................................................
int CEffectBase::Render(float dtime)
{

	return TRUE;
}

//................................................................................................................
// CheckFrame
//................................................................................................................
int	CEffectBase::CheckFrame(void)
{

	return TRUE;
}

//................................................................................................................
// SortRender
//................................................................................................................
int	CEffectBase::SortRender(float dtime)
{

	return TRUE;
}

//................................................................................................................
// Update
//................................................................................................................
int CEffectBase::Update(float dtime)
{
	return TRUE;
}

//................................................................................................................
// Init
//................................................................................................................
void CEffectBase::Init(void)
{

}

//................................................................................................................
// Destroy
//................................................................................................................
void CEffectBase::Destroy(void)
{
}

//................................................................................................................
// 이펙트 처리후 뒤로올 이펙트 셋업
//................................................................................................................
int CEffectBase::SetAfterEffect(void)
{

	return TRUE;
}


HRESULT	CEffectBase::InvalidateDeviceObject()
{
	return S_OK;	


}

HRESULT	CEffectBase::RestoreDeviceObject()
{
	return S_OK;
}

void CEffectBase::GetXAndYRadian(float & fXRadian, float & fYRadian, D3DXVECTOR3 vStartPos, D3DXVECTOR3 vDestPos)
{
	/// 시작점과 도착점의 거리체크
	float fDistance = D3DXVec3Length( &(vDestPos - vStartPos) );
	if(fDistance < 0.1f)
	{
		return;	
	}	
	
	D3DXVECTOR3 vDir1, vDir2;	
	
	D3DXVECTOR3 vXDestPos = vDestPos;
	vXDestPos.y = vStartPos.y;
	vDir1 = vXDestPos - vStartPos;
	D3DXVec3Normalize( &vDir1 , &vDir1 );
	
	vDir2 = vDestPos - vStartPos;
	D3DXVec3Normalize( &vDir2 , &vDir2 );

	if(ISZERO(vDir1.y - vDir2.y))
	{
		fXRadian = 0.0f;
		return;
	}

	fXRadian = D3DXVec3Dot( &vDir1 , &vDir2 );	
	fXRadian = acos((double)fXRadian);

	if(vDir2.y < 0.0f)
	{
	    fXRadian = (2.0f * D3DX_PI) - fXRadian;
	}
	
	vDir1 = vDestPos - vStartPos;
	vDir1.y = 0.0f;
	D3DXVec3Normalize(&vDir1, &vDir1);

	if(ISZERO(vDir1.z - (-1.0f)))
	{
		fYRadian = 0.0f;
		return;
	}

	fYRadian = D3DXVec3Dot( &vDir1 , &D3DXVECTOR3(0.0f, 0.0f, -1.0f) );
	fYRadian = acos((double)fYRadian);

	if(vDir1.x > 0.0f)
	{
	    fYRadian = (2.0f * D3DX_PI) - fYRadian;
	}
}
