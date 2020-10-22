#include "CEFConAniSkill.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include	<SOUND\Sound.H>

extern long		 g_nowTime;

#define AIMAGE_MAX_COUNT	3

CEFConAniSkill::CEFConAniSkill()
{
	m_nAniStep = 0;
	m_nNowAniIndex = 0;	
	m_pConSkillData = NULL;
	m_dPrevTime = 0;

	m_IsRAImage = FALSE;	
	m_IsRWAImage = FALSE;	
}

CEFConAniSkill::~CEFConAniSkill()
{
	if(m_pSrcChr)
	{
		m_pSrcChr->SetApplyConAniSkill(FALSE);
	}	
	
	m_nAniStep = 0;
	m_nNowAniIndex = 0;
	m_pConSkillData = NULL;
}

void CEFConAniSkill::Init(void)
{
	if( NULL == m_pSrcChr )
	{
		SetDestroy(TRUE);
		return;
	}
	
	m_pConSkillData = (void *)g_Pc_Manager.m_SourceDataManager.FindConSkillDataPtr((DWORD)m_ID);

	if(NULL == m_pConSkillData)
	{
		SetDestroy(TRUE);
		return;
	}

	m_dPrevTime = g_nowTime;
	
	///-- 주체에 연속 애니메이션 스킬 적용
	m_pSrcChr->SetApplyConAniSkill(TRUE);

	m_nAniStep = 0;
	SConSkillData * pConSkillData = (SConSkillData *)m_pConSkillData;
	
	m_nNowAniIndex = pConSkillData->sAniList[m_nAniStep].dAniNum;
	int nTime = (int)pConSkillData->sAniList[m_nAniStep].bVelRatio;
	///-- 처음 애니메이션 적용
	m_pSrcChr->SetConAniTime(nTime);
	m_pSrcChr->AnimationPlay(m_nNowAniIndex);
	
	m_IsRAImage = (BOOL)(pConSkillData->bIsAfterImage);
	m_IsRWAImage = (BOOL)(pConSkillData->bIsAfterWImage);
	
	//................................................................................	
	// 발동 사운드
	//................................................................................	
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( m_pExtraInfo->SkillCode , m_pSrcChr );

	UINT uImpactId = (UINT)(pConSkillData->sAniList[m_nAniStep].dImpEff);
	if(uImpactId)
	{
		int nType = ESLT_CHARPOS;
		
		switch(uImpactId)
		{	
			case EF_ROTATEKICK:
				{
					nType = ESLT_CPOS_ICAMERA;
				}
				break;			
		}			
		
		g_Particle.SetEffSetCon( 0.0f , uImpactId , nType , 5.1f , 
				        		 m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex() );
	}		
}

int	CEFConAniSkill::CheckFrame(void)
{
	if(m_State == ES_SELF_DESTROY)
	{
		return TRUE;
	}	
	
	if( g_Particle.m_EffectManager.CheckDie( m_pSrcChr ) )
	{
		SetDestroy(TRUE);
		return TRUE;
	}
	
	SConSkillData * pConSkillData = NULL;
	
	if( m_pSrcChr->m_ani_frame + c_MotionBlendHalfFrame > (int)m_pSrcChr->m_end_frame )
	{
		///-- 다음 스텝으로 이동
		m_nAniStep++;
		pConSkillData = (SConSkillData *)m_pConSkillData;
		
		///-- 다음 스텝이 15단계이면 마지막이다.
		if(m_nAniStep == 15)
		{
			m_nNowAniIndex = 0;
			m_pSrcChr->SetApplyConAniSkill(FALSE);					
			SetDestroy();
			return TRUE;
		}

		///-- 다음 스텝 애니메이션이 0이면 마지막이다.
		m_nNowAniIndex = pConSkillData->sAniList[m_nAniStep].dAniNum;
		if(0 == m_nNowAniIndex)
		{
			m_nNowAniIndex = 0;
			m_pSrcChr->SetApplyConAniSkill(FALSE);					
			SetDestroy();
			return TRUE;
		}

		int nTime = (int)pConSkillData->sAniList[m_nAniStep].bVelRatio;
		m_pSrcChr->SetConAniTime(nTime);
		m_pSrcChr->AnimationPlay(m_nNowAniIndex);

		UINT uImpactId = (UINT)(pConSkillData->sAniList[m_nAniStep].dImpEff);
		if(uImpactId)
		{
			int nType = ESLT_CHARPOS;
			
			switch(uImpactId)
			{	
				case EF_ROTATEKICK:
					{
						nType = ESLT_CPOS_ICAMERA;
					}
					break;			
			}			
			
			g_Particle.SetEffSetCon( 0.0f , uImpactId , nType , 5.1f , 
				        		     m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex() );
		}		
	}

	if( m_pSrcChr->m_ani_frame == g_Pc_Manager.GetImpactFrame(m_pSrcChr, m_nNowAniIndex) )
	{		
		UINT uAttackId;
		pConSkillData = (SConSkillData *)m_pConSkillData;			
		
		uAttackId = (UINT)(pConSkillData->sAniList[m_nAniStep].dDamEff);		
		
		if(m_nAniStep == 14)
		{
			SetAfterEffect(uAttackId, TRUE);
			return TRUE;
		}

		int nNextAniNum = pConSkillData->sAniList[m_nAniStep+1].dAniNum;
		if(0 == nNextAniNum)
		{
			SetAfterEffect(uAttackId, TRUE);
			return TRUE;
		}
		
		SetAfterEffect(uAttackId, FALSE);
	}

	return TRUE;
}

int CEFConAniSkill::Update(float dtime)
{
	Character			*pTempDest = NULL;
	EFFECT_EXTRA_INFO    Info;
	
	if( g_Particle.m_EffectManager.CheckDie( m_pSrcChr ) )
	{
		SetDestroy(TRUE);
		return TRUE;
	}

	if(!m_IsRAImage)
	{
		return TRUE;
	}
	
	if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( g_nowTime , m_dPrevTime ) , > , 70 ) )
	{
		SAImageData sData;
		sData.n_end_frame = m_pSrcChr->m_end_frame;
		sData.n_blend_frame = m_pSrcChr->m_blend_frame;
		sData.n_start_blend_frame = m_pSrcChr->m_start_blend_frame;
		sData.n_ani_frame = m_pSrcChr->m_ani_frame;
		sData.n_ani_number = m_pSrcChr->m_ani_number;
		sData.n_pre_end_frame = m_pSrcChr->m_pre_end_frame;
		sData.n_pre_ani_number = m_pSrcChr->m_pre_ani_number;
		sData.f_middle_time = m_pSrcChr->m_middle_time;

		sData.IsAfterWImage = (bool)m_IsRWAImage;
		
		sData.sPos.x = m_pSrcChr->GetPosTM()._41;
		sData.sPos.y = m_pSrcChr->GetPosTM()._42;
		sData.sPos.z = m_pSrcChr->GetPosTM()._43;

		sData.sDiffuse.r = 0.0f;
		sData.sDiffuse.g = 0.0f;
		sData.sDiffuse.b = 0.15f;
		
		m_AImageList.push_front( sData );
		
		if( m_AImageList.size() > AIMAGE_MAX_COUNT )
			m_AImageList.pop_back();

		m_dPrevTime = g_nowTime;
	}	
	
	return TRUE;
}

int	CEFConAniSkill::Render(float dtime)
{
	if(!m_IsRAImage)
	{
		return TRUE;
	}	
	
	D3DXMATRIX matLocal;	

	D3DXMatrixIdentity( &matLocal );

	g_RenderManager.SetTransform( D3DTS_WORLD , &matLocal );

	float AlphaList[] = {
		0.2f,
		0.3f,
		0.4f,
		0.5f,
		0.6f,
		0.7f	
	};
	
	float Alpha = 0.0f;

	g_RenderManager.SetFVF( D3DFVF_OBJECTVERTEX );
#ifdef DIRECT_VERSION_9_MJH
	D3DLIGHT9 lLight;
#else
	D3DLIGHT8 lLight;
#endif // DIRECT_VERSION_9_MJH	
	lLight = g_Pc_Manager.theCharacter_Light;
	g_RenderManager.SetLight( 0, &lLight );
	g_RenderManager.LightEnable( 0, TRUE );

	g_RenderManager.SetAlphaBlendEnable( TRUE );
	g_RenderManager.SetLighting( TRUE );

	g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_ADD );
	g_RenderManager.SetTextureColorArg1( 0 , D3DTA_DIFFUSE );
	g_RenderManager.SetTextureColorArg2( 0 , D3DTA_TEXTURE );
	
	
	g_RenderManager.SetTextureAlphaOP  ( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureAlphaArg1( 0 , D3DTA_CURRENT );
	g_RenderManager.SetTextureAlphaArg2( 0 , D3DTA_TEXTURE );

	g_RenderManager.DisableTextureStageState( 1 );
	g_RenderManager.DisableTextureStageState( 2 );

	long lDelAtime = (long)m_pSrcChr->m_nDelVATime;
	if(lDelAtime < 30)
	{
		lDelAtime = 30;	
	}	
	float fRatio =  30.0f / (float)lDelAtime;	
	int nCount = (int)(m_AImageList.size() * fRatio);

	if(m_AImageList.size() && nCount < 1)
	{
		nCount = 1;
	}

	for( int i = 0 ; i < nCount ; ++i )
	{
		 m_AImageList[i].fAlpha = AlphaList[i];
		 m_pSrcChr->DisplayAfterImage(m_AImageList[i]);
	}

	m_pSrcChr->AniRender();
	m_pSrcChr->Display(0, 0, false, FALSE);

	g_RenderManager.SetLighting( FALSE );
	g_RenderManager.LightEnable( 0, FALSE );

	g_RenderManager.SetAlphaBlendEnable( FALSE );
	
	return TRUE;
}

int CEFConAniSkill::SetAfterEffect(UINT uAttackId, BOOL IsLast)
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
				
				if(IsLast)
				{				
					Info.AfterEffect = uAttackId;
					Info.DestType = 0;
					Info.lifeTime = 5.1f;
					
					g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , pTempDest , &Info );
				}
				
				if(uAttackId == 0)
				{
					continue;
				}				
				
				switch(uAttackId)
				{		
					case EF_STAND_SWORD_ATTACKED0:
					case EF_STAND_SWORD_ATTACKED1:
					case EF_STAND_SWORD_ATTACKED2:
						{
							g_Particle.SetEffSetCon( 0.0f , uAttackId , ESLT_SRC_RAD , 0.5f , 
							pTempDest->GetUnique() , pTempDest->GetIndex() , 
							::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr) , 
							m_pSrcChr->GetDirection() + D3DXToRadian( 180 ) );
						}
						break;
					default:
						{
							g_Particle.SetEffSetCon( 0.0f , uAttackId , 0 , 5.1f , 
										pTempDest->GetUnique() , pTempDest->GetIndex(),
										::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr) );
						}
						break;
				}	
				
			}

			Info.Clear();
		}
	}

	if(uAttackId == 0)
	{
		return TRUE;
	}				

	return TRUE;
}