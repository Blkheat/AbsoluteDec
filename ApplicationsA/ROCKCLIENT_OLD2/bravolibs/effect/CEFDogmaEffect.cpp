#include "CEFDogmaEffect.h"
#include "..\\Effect.h"

extern long		 g_nowTime;

//----------------------------------------------------------------------------------------------------------------------
CEFDogmaEffect::CEFDogmaEffect()
{
	m_InitTime = g_nowTime;
	
	m_IsConEffect = TRUE;
	
	m_ISShot = FALSE;	
	m_ISDamageProcess = FALSE;


#ifdef HHM_SEALED_TEMPLE_BOSSMOB_EFFECT_LIMIT_20091207

	m_iFireRainCount = 5;
	m_bWalkDust = false;

	m_fMasterFireSize = 2.0f;

	m_aiMasterFireBone[0] = 33;
	m_aiMasterFireBone[1] = 37;

#else

	m_iFireRainCount = 15;
	m_bWalkDust = true;

	m_fMasterFireSize = 4.0f;
	
	// 좌어깨
	m_aiMasterFireBone[0] = 7;
	m_aiMasterFireBone[1] = 8;
	
	// 우어깨
	m_aiMasterFireBone[2] = 18;
	m_aiMasterFireBone[3] = 20;

	m_aiMasterFireBone[4] = 33;// 좌무릎
	m_aiMasterFireBone[5] = 37;// 우무릎


#endif// #ifdef HHM_SEALED_TEMPLE_BOSSMOB_EFFECT_LIMIT_20091207
}

//----------------------------------------------------------------------------------------------------------------------
CEFDogmaEffect::~CEFDogmaEffect()
{
	if(m_pSrcChr)
		m_pSrcChr->SetApplyConAniSkill(FALSE);
}

//----------------------------------------------------------------------------------------------------------------------
HRESULT	CEFDogmaEffect::InvalidateDeviceObject()
{
	m_SwordTail.InvalidateDeviceObject();
	return S_OK;
}

//----------------------------------------------------------------------------------------------------------------------
#ifdef DIRECT_VERSION_9_MJH
HRESULT	CEFDogmaEffect::RestoreDeviceObject (LPDIRECT3DDEVICE9 pDevice)
#else
HRESULT	CEFDogmaEffect::RestoreDeviceObject (LPDIRECT3DDEVICE8 pDevice)
#endif // DIRECT_VERSION_9_MJH
{

	m_SwordTail.InitDevice(g_lpDevice);	

	CStringToCode	HashString("afterimage3.tga");
	m_SwordTail.m_HashCode = HashString.GetHashCode();

	return S_OK;
}

//----------------------------------------------------------------------------------------------------------------------
void CEFDogmaEffect::Init(void)
{
	
	if(m_ID == EF_MASTER_DOGMA_APPEAR_EFFECT)
	{
		if(!g_Particle.m_EffectManager.CheckCharacter(m_pSrcChr))
		{
			SetDestroy();	
			return;
		}
	}
	else
	{
		if(m_ID != EF_DOGMA_L1_FIRERAIN)
		{
			if(!g_Particle.m_EffectManager.CheckCharacter(m_pSrcChr))
			{
				SetDestroy(TRUE);	
				return;
			}
		}
	}	

	m_SwordTail.InitDevice(g_lpDevice);
	CStringToCode	HashString("afterimage3.tga");
	m_SwordTail.m_HashCode = HashString.GetHashCode();
	
	if(m_pSrcChr)
		m_Src_Ani_Index = m_pSrcChr->m_ani_index;

	//................................................................................	
	// 발동 사운드
	//................................................................................	
	if(m_pExtraInfo != NULL)
		g_Particle.m_EffectManager.GetSkillImfactSound(m_pExtraInfo->SkillCode , m_pSrcChr);


	switch(m_ID)
	{
		case EF_DOGMA_L1_FIRERAIN:
			InitDogmaFireRain();
			break;

		case EF_DOGMA_L1_FRAME_STORM:
			InitDogmaFrameStorm();
			break;

		case EF_DOGMA_L2_WRATH_ONECUT:
			InitDogmaWrathOneCut();
			break;

		case EF_DOGMA_L2_SWORD_FLASH:
			InitDogmaSwordFlash();	
			break;

		case EF_DOGMA_L3_FOOT_QUAKE:
			InitDogmaFootQuake();
			break;

		case EF_DOGMA_L3_WRATH_ONECUT2:
			InitDogmaWrathOneCut2();
			break;

		case EF_DOGMA_L4_CIRCLE_SWORD:
			InitDogmaCircleSword();
			break;

		case EF_MASTER_DOGMA_APPEAR_EFFECT:
			InitMasterDogmaAEffect();
			break;
	}
}

//----------------------------------------------------------------------------------------------------------------------
int CEFDogmaEffect::CheckFrame(void)
{
	switch(m_ID)
	{
		case EF_DOGMA_L1_FIRERAIN:
			return CheckDogmaFireRain();

		case EF_DOGMA_L1_FRAME_STORM:
		    return CheckDogmaFrameStorm();

		case EF_DOGMA_L2_WRATH_ONECUT:
			return CheckDogmaWrathOneCut();

		case EF_DOGMA_L2_SWORD_FLASH:
			return CheckDogmaSwordFlash();

		case EF_DOGMA_L3_FOOT_QUAKE:
			return CheckDogamFootQuake();

		case EF_DOGMA_L3_WRATH_ONECUT2:
			return CheckDogmaWrathOneCut2();

		case EF_DOGMA_L4_CIRCLE_SWORD:
			return CheckDogmaCircleSword();
	}	
		
	return TRUE;	
}

//----------------------------------------------------------------------------------------------------------------------
int CEFDogmaEffect::Update(float dtime)
{
	switch(m_ID)
	{
		case EF_DOGMA_L1_FIRERAIN:
			return UpdateDogmaFireRain(dtime);

		case EF_DOGMA_L2_WRATH_ONECUT:
			return UpdateDogmaWrathOneCut(dtime);

		case EF_DOGMA_L2_SWORD_FLASH:
			return UpdateDogmaSwordFlash(dtime);

		case EF_DOGMA_L3_FOOT_QUAKE:
			return UpdateDogmaFootQuake(dtime);

		case EF_DOGMA_L3_WRATH_ONECUT2:
			return UpdateDogmaWrathOneCut2(dtime);

		case EF_DOGMA_L4_CIRCLE_SWORD:
			return UpdateDogmaCircleSword(dtime);

		case EF_MASTER_DOGMA_APPEAR_EFFECT:
			return UpdateMasterDogmaAEffect(dtime);
	}	

	return TRUE;
}

//----------------------------------------------------------------------------------------------------------------------
int CEFDogmaEffect::Render(float dtime)
{
	switch(m_ID)
	{
		case EF_DOGMA_L1_FIRERAIN:
			return RenderDogmaFireRain(dtime);

		case EF_DOGMA_L1_FRAME_STORM:
		    return RenderDogmaFrameStorm(dtime);

		case EF_MASTER_DOGMA_APPEAR_EFFECT:
			return RenderMasterDogmaAEffect(dtime);
	}	

	return TRUE;
}

//----------------------------------------------------------------------------------------------------------------------
void CEFDogmaEffect::InitDogmaFireRain()
{
	if(m_pSrcChr == NULL)
	{
		WORD aSkillCode = 0;	
		float fRadius = 0.0f;

		if(m_pExtraInfo != NULL)
		{
			aSkillCode = m_pExtraInfo->SkillCode;
			fRadius = g_Particle.m_EffectManager.GetAttackRange(m_pExtraInfo->SkillCode);
		}		
		
	
		m_InitTime = g_nowTime;

		// 스킬 범위
		m_EffectDrop.Init(m_EffectTargetInfo.vPos.x , m_EffectTargetInfo.vPos.z , 
							150.0f , D3DXToRadian(-25) , 800 , 1200 , m_iFireRainCount,
							fRadius , g_nowTime , 2000, TRUE);

		m_ISDamageProcess = TRUE;

		if(m_ISShot)
		{
			//파티클 파이볼구조의 파이어 이미지와 파이어 꼬리가 필요하다 .. 
			CParticleData *pData = NULL; 

			if((pData = g_Particle.m_ParticleData.GetKeyDataPointer(PT_FIREBALL)) == NULL)// 파이어볼
			{
				return;
				SetDestroy();
			}

			m_ParticleData.m_pReference = pData; 
		
			m_ParticleData.m_vMoveLocation	= D3DXVECTOR3(0.0f , 0.0f , 0.0f);
			m_ParticleData.m_bIsSuppressed	= false;
			m_ParticleData.m_bIsCreate		= true;
			m_ParticleData.m_lUnique		= -1;
			m_ParticleData.m_iIndex			= -1;
			m_ParticleData.m_fSize			= 1.5f;	
		}
	
		return;
	}	
	
	CParticleData *  pData = NULL;

	if((pData = g_Particle.m_ParticleData.GetKeyDataPointer(24553)) == NULL)// 불타버린 본 아처 - 불화살 - 불 꼬리필요함
	{
		SetDestroy();
		return;
	}	

	m_ParticleLink[0].MakeLink(pData);
	m_ParticleLink[0].m_fSize = 10.0f;
	m_ParticleLink[1].MakeLink(pData);
	m_ParticleLink[1].m_fSize = 10.0f;
	
	m_Src_Ani_Index = m_pSrcChr->m_ani_index;	
	m_pSrcChr->SetApplyConAniSkill(TRUE);
	m_pSrcChr->SetConAniTime(80);
	
	m_pSrcChr->SetDirection(m_EffectTargetInfo.vPos.x, m_EffectTargetInfo.vPos.z);

	g_Particle.m_IsGUnitLinkBoneAndSizeFix = TRUE;
	g_Particle.m_fAddSize = 8.0f;
	g_Particle.SetEffSetCon(0.0f , EF_DOGMA_L1_FIRERAIN, ESLT_CHARPOS , // 도그마베인 - 1단계 파이어레인
							 EFFECT_SET_LOOP_TIME , m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex());

	g_Particle.m_IsGUnitLinkBoneAndSizeFix = TRUE;
	g_Particle.m_fAddSize = 8.0f;
	g_Particle.SetEffSetCon(0.0f , EF_B_ARCHER_FIRE_ARROW, ESLT_CHARPOS ,// // 불타버린 본 아처 - 불화살 - 불 꼬리필요함
							 EFFECT_SET_LOOP_TIME , m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex());
}

//----------------------------------------------------------------------------------------------------------------------
int	CEFDogmaEffect::CheckDogmaFireRain()
{
	if(m_pSrcChr == NULL)
		return TRUE;

	if(m_ISShot)
		return TRUE;
	
	WORD aSkillCode = 0;	
	float fRadius = 0.0f;

	if(m_pExtraInfo != NULL)
	{
		aSkillCode = m_pExtraInfo->SkillCode;
		fRadius = g_Particle.m_EffectManager.GetAttackRange(m_pExtraInfo->SkillCode);
	}	
	
	if(!g_Particle.m_EffectManager.CheckCharacter(m_pSrcChr))
	{
		if(m_pSrcChr)
		{
			g_Particle.DelEffSetCon(EF_B_ARCHER_CURSE_ARROW , ESLT_CHARPOS , // 저주받은 본아처 - 저주의 화살 저주관련꼬리 필요함
								 m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex(), TRUE);
		}

		m_ISShot = TRUE;
		m_InitTime = g_nowTime;

		// 스킬 범위
		m_EffectDrop.Init(m_EffectTargetInfo.vPos.x , m_EffectTargetInfo.vPos.z , 
						   150.0f , D3DXToRadian(-25) , 800 , 1200 , m_iFireRainCount,
						   fRadius , g_nowTime , 2000, TRUE);

		m_ISDamageProcess = TRUE;		
	}

	BOOL bIsAction = FALSE;	
	
	if(m_pSrcChr)
	{
		if(m_Src_Ani_Index != m_pSrcChr->m_ani_index || m_pSrcChr->m_ani_frame == 77)
			bIsAction = TRUE;
	}

	// 임펙트 프레임 화살 나가는 시점
	if(bIsAction)
	{
		g_Particle.DelEffSetCon(EF_DOGMA_L1_FIRERAIN , ESLT_CHARPOS , 
								 m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex());
		
		g_Particle.DelEffSetCon(EF_B_ARCHER_FIRE_ARROW , ESLT_CHARPOS , 
								 m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex());
		
		
		m_ISShot = TRUE;
		m_InitTime = g_nowTime;

		// 스킬 범위
		m_EffectDrop.Init(m_EffectTargetInfo.vPos.x , m_EffectTargetInfo.vPos.z , 
						   150.0f , D3DXToRadian(-25) , 800 , 1200, m_iFireRainCount,
						   fRadius , g_nowTime , 2000, TRUE);

		m_ISDamageProcess = TRUE;		
	}

	if(m_ISShot)
	{
		//파티클 파이볼구조의 파이어 이미지와 파이어 꼬리가 필요하다 .. 
		CParticleData *pData = NULL; 

		if((pData = g_Particle.m_ParticleData.GetKeyDataPointer(PT_FIREBALL)) == NULL)
		{
			return FALSE;
			SetDestroy();
		}

		m_ParticleData.m_pReference = pData; 
	
		m_ParticleData.m_vMoveLocation	= D3DXVECTOR3(0.0f , 0.0f , 0.0f);
		m_ParticleData.m_bIsSuppressed	= false;
		m_ParticleData.m_bIsCreate		= true;
		m_ParticleData.m_lUnique		= -1;
		m_ParticleData.m_iIndex			= -1;
		m_ParticleData.m_fSize			= 1.5f;	
	}

	return TRUE;
}

//----------------------------------------------------------------------------------------------------------------------
int	CEFDogmaEffect::UpdateDogmaFireRain(float dtime)
{
	if(FALSE == m_ISShot)
		return TRUE;
	
	m_EffectDrop.Update((DWORD)g_nowTime);	

	Character			*pTempDest = NULL;
	EFFECT_EXTRA_INFO    Info;
	
	if(m_ISDamageProcess)
	{
		if(SAFE_TIME_COMPARE(SAFE_TIME_SUB(g_nowTime , m_InitTime) , > , 1000))
		{			
			if(m_pExtraInfo != NULL)
			{
				// 1:N 데미지
				int Count = m_pExtraInfo->GetTargetCount();		
				
				for(int i = 0; i < Count; ++i)
				{
					if(m_pExtraInfo->GetTargetInfo(&pTempDest , &Info , i))
					{
						g_Particle.SetEffSetCon(0.0f , EF_FIREBALL_ATTACKED0 , ESLT_CHARPOS , 
												 5.1f , pTempDest->GetUnique() , pTempDest->GetIndex(), 
												 ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr));				
						
						Info.AfterEffect = EF_FIREBALL_ATTACKED0;
						Info.DestType = ESLT_CHARPOS;
						Info.lifeTime = 5.1f;											
						g_Particle.m_EffectManager.EffectTargetHit(m_pSrcChr , pTempDest , &Info);
					}

					Info.Clear();
				}
			}
		
			m_ISDamageProcess = FALSE;
		}
	}
	else
	{
		if(m_pExtraInfo != NULL)
		{
			// 1:N 데미지
			int Count = m_pExtraInfo->GetTargetCount();		
				
			for(int i = 0; i < Count; ++i)
			{
				if(m_pExtraInfo->GetTargetInfo(&pTempDest , &Info , i))
				{
					if(pTempDest)
					{
						pTempDest->m_lPreTime = g_nowTime;
					}
				}
			}
		}
	}

	///-- 해제하는 루틴이 없네... 무한반복이라 
	///-- 이렇게 하지 않으면 해제되지 않음	
	///-- 일단 복구시킴 By simwoosung
	if(SAFE_TIME_COMPARE(SAFE_TIME_SUB(g_nowTime , m_InitTime) , > , 5100))
	{
		m_EffectDrop.SetIsLoop(FALSE);
		return TRUE;
	}
	
	return TRUE;
}

//----------------------------------------------------------------------------------------------------------------------
int	CEFDogmaEffect::RenderDogmaFireRain(float dtime)
{
	if(FALSE == m_ISShot && m_pSrcChr)
	{
		D3DXVECTOR3 vPos = D3DXVECTOR3(m_pSrcChr->GetRealRHandTM()._41,
		   								m_pSrcChr->GetRealRHandTM()._42,
										m_pSrcChr->GetRealRHandTM()._43);
		
		m_ParticleLink[0].SetLinkPos(vPos);
		m_ParticleLink[0].UpdateLink(dtime);
		m_ParticleLink[0].Render();

		vPos = D3DXVECTOR3(m_pSrcChr->GetRealLHandTM()._41,
		   					m_pSrcChr->GetRealLHandTM()._42,
							m_pSrcChr->GetRealLHandTM()._43);
		
		m_ParticleLink[1].SetLinkPos(vPos);
		m_ParticleLink[1].UpdateLink(dtime);
		m_ParticleLink[1].Render();	
		
		
		return TRUE;
	}

	DWORD dCullMode = g_RenderManager.GetCullMode();
	DWORD dZWriteEnable = g_RenderManager.GetZWriteEnable();
	DWORD dZEnable = g_RenderManager.GetZEnable();
	DWORD dAlphaBlendEnable = g_RenderManager.GetAlphaBlendEnable();
	DWORD dSrcBlend = g_RenderManager.GetSrcBlend();
	DWORD dDestBlend = g_RenderManager.GetDestBlend();
	
	if(m_EffectDrop.GetDataSize())
	{
		DROP_EFFECT_DATA *pData = NULL;
		D3DXMATRIX		  matLocal;

		D3DXMatrixIdentity(&matLocal);

		m_EffectDrop.StartPosition();		
		D3DXMatrixIdentity(&matLocal);
		
		g_RenderManager.SetTransform(D3DTS_WORLD , &matLocal);		
		g_RenderManager.SetCullMode(D3DCULL_NONE);
		
		g_RenderManager.SetZWriteEnable(FALSE);
		g_RenderManager.SetZEnable(D3DZB_TRUE);
		
		g_RenderManager.SetAlphaBlendEnable(TRUE);
		g_RenderManager.SetSrcBlend(D3DBLEND_ONE);
		g_RenderManager.SetDestBlend(D3DBLEND_ONE);

		while((pData = m_EffectDrop.GetDropData()) != NULL)
		{			
			if(pData->State == DROPST_DROP	|| 	pData->State == DROPST_END)
			{					
				//바닥에 떨어 졌을때 .. 
				if(pData->State == DROPST_END)
					g_Particle.SetEffSetCon(0.0f , EF_FOOT_QUAKE0 , 100 , 2.6f , -1 , -1 , pData->vPos);

				g_RenderManager.SetTransform(D3DTS_WORLD , &matLocal);

				m_ParticleData.SetLinkPos(pData->vPos);
				m_ParticleData.UpdateLink(dtime);
				m_ParticleData.Render();

				//파이어볼 꼬리를 달고 간다 .. 
				g_Particle.DisplayUpdate(PT_FIREBALL_TAIL , pData->vPos);
			}			
		}

		g_RenderManager.SetCullMode(D3DCULL_CCW);
		g_RenderManager.SetZWriteEnable(TRUE);
		g_RenderManager.SetZEnable(TRUE);
		g_RenderManager.SetAlphaBlendEnable(FALSE);

	}
	else
	{
		g_RenderManager.SetCullMode(dCullMode);
		g_RenderManager.SetZWriteEnable(dZWriteEnable);
		g_RenderManager.SetZEnable(dZEnable);
		g_RenderManager.SetAlphaBlendEnable(dAlphaBlendEnable);
		g_RenderManager.SetSrcBlend(dSrcBlend);
		g_RenderManager.SetDestBlend(dDestBlend);
		
		SetDestroy();
		return TRUE;
	}

	g_RenderManager.SetCullMode(dCullMode);
	g_RenderManager.SetZWriteEnable(dZWriteEnable);
	g_RenderManager.SetZEnable(dZEnable);
	g_RenderManager.SetAlphaBlendEnable(dAlphaBlendEnable);
	g_RenderManager.SetSrcBlend(dSrcBlend);
	g_RenderManager.SetDestBlend(dDestBlend);

	return TRUE;
}

//----------------------------------------------------------------------------------------------------------------------
void CEFDogmaEffect::InitDogmaFrameStorm()
{
	if(!g_Particle.m_EffectManager.CheckCharacter(m_pSrcChr , m_pDestChr))
	{
		SetDestroy(TRUE);	
		return;
	}	

	m_pSrcChr->SetDirection(m_EffectTargetInfo.vPos.x, m_EffectTargetInfo.vPos.z);
	m_pSrcChr->SetApplyConAniSkill(TRUE);
	m_pSrcChr->SetConAniTime(80);

	CParticleData *  pData = NULL;

	if((pData = g_Particle.m_ParticleData.GetKeyDataPointer(24553)) == NULL)
	{
		SetDestroy();
		return;
	}	

	m_ParticleLink[0].MakeLink(pData);
	m_ParticleLink[0].m_fSize = 10.0f;
    
}

//----------------------------------------------------------------------------------------------------------------------
int CEFDogmaEffect::CheckDogmaFrameStorm()
{
	if(m_State == ES_SELF_DESTROY)
		return TRUE;
	
	if(!g_Particle.m_EffectManager.CheckCharacter(m_pSrcChr))
	{
		SetDestroy(TRUE);
		return TRUE;
	}

	BOOL bIsAction = FALSE;
	
	if(m_Src_Ani_Index != m_pSrcChr->m_ani_index)
		bIsAction = TRUE;

	if(m_pSrcChr->m_ani_frame == 139)
		bIsAction = TRUE;
	
	if(m_pExtraInfo != NULL)
	{
		Character			*pTempDest = NULL;
		EFFECT_EXTRA_INFO    Info;
		
		// 1:N 데미지
		int Count = m_pExtraInfo->GetTargetCount();		
			
		for(int i = 0; i < Count; ++i)
		{
			if(m_pExtraInfo->GetTargetInfo(&pTempDest , &Info , i))
			{
				if(pTempDest)
					pTempDest->m_lPreTime = g_nowTime;
			}
		}
	}

	// 임펙트 프레임 화살 나가는 시점
	if(bIsAction)
	{		
		g_Particle.DelEffSetCon(EF_DOGMA_L1_FIRERAIN , ESLT_CHARPOS , 
								 m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex());
		
		D3DXVECTOR3 vPos;
	
		vPos.x = m_EffectTargetInfo.vPos.x;
		vPos.z = m_EffectTargetInfo.vPos.z;
		vPos.y = g_Map.PHF_GetHeight(vPos.x , vPos.z);
		
		g_Particle.SetEffSetCon(0.0f , EF_FRAME_STORM , 100 , 5.1f , -1 , -1 , vPos);
		
		Character			*pTempDest = NULL;
		EFFECT_EXTRA_INFO    Info;

		if(m_pExtraInfo != NULL)
		{
			// 1:N 데미지
			int Count = m_pExtraInfo->GetTargetCount();			
			
			for(int i = 0; i < Count; ++i)
			{
				if(m_pExtraInfo->GetTargetInfo(&pTempDest , &Info , i))
				{
					//................................................................................	
					// 어택 사운드
					//................................................................................	
					if(m_pExtraInfo != NULL)
						g_Particle.m_EffectManager.GetSkillAttackedSound(m_pExtraInfo->SkillCode , pTempDest);				
					

					g_Particle.SetEffSetCon(0.0f , EF_FRAME_STORMED , ESLT_CHARPOS , 5.1f , pTempDest->GetUnique() , 
											 pTempDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 
											 m_pSrcChr));
						
					Info.AfterEffect = EF_FRAME_STORMED;
					Info.DestType = ESLT_CHARPOS;
					Info.lifeTime = 5.1f;
						
					g_Particle.m_EffectManager.EffectTargetHit(m_pSrcChr , pTempDest , &Info);		
				}

				Info.Clear();
			}
		}	

		SetDestroy();	
	}
	
	return TRUE;
}

//----------------------------------------------------------------------------------------------------------------------
int	CEFDogmaEffect::RenderDogmaFrameStorm(float dtime)
{
	D3DXVECTOR3 vPos = D3DXVECTOR3(m_pSrcChr->GetRealRHandTM()._41,
		   							m_pSrcChr->GetRealRHandTM()._42,
									m_pSrcChr->GetRealRHandTM()._43);
		
	m_ParticleLink[0].SetLinkPos(vPos);
	m_ParticleLink[0].UpdateLink(dtime);
	m_ParticleLink[0].Render();	
	
	return TRUE;
}

//----------------------------------------------------------------------------------------------------------------------
void CEFDogmaEffect::InitDogmaWrathOneCut()
{
	if(!g_Particle.m_EffectManager.CheckCharacter(m_pSrcChr , m_pDestChr))
	{
		SetDestroy(TRUE);	
		return;
	}
}

//----------------------------------------------------------------------------------------------------------------------
int	CEFDogmaEffect::CheckDogmaWrathOneCut()
{
	if(m_State == ES_SELF_DESTROY)
		return TRUE;
	
	if(!g_Particle.m_EffectManager.CheckCharacter(m_pSrcChr))
	{
		SetDestroy(TRUE);
		return TRUE;
	}

	BOOL bIsAction = FALSE;
	
	if(m_Src_Ani_Index != m_pSrcChr->m_ani_index)
		bIsAction = TRUE;

	if(m_pSrcChr->m_ani_frame == 77)
		bIsAction = TRUE;

	if((m_pSrcChr->m_ani_frame % 5) == 0 && m_bWalkDust)
	{
		D3DXMATRIX matBone;			
		m_pSrcChr->GetBoneLinkTM(6 , matBone , FALSE);
		D3DXVECTOR3	Pos1(matBone._41, 0.0f, matBone._43);	
		Pos1.y = g_Map.PHF_GetHeight(Pos1.x , Pos1.z);
		g_Particle.DisplayUpdate(PT_WALK_DUST , Pos1, -1, 5.0f);
		
		m_pSrcChr->GetBoneLinkTM(7 , matBone , FALSE);
		D3DXVECTOR3	Pos2(matBone._41, 0.0f, matBone._43);	
		Pos2.y = g_Map.PHF_GetHeight(Pos2.x , Pos2.z);
		g_Particle.DisplayUpdate(PT_WALK_DUST , Pos2, -1, 5.0f);
	}

	if(m_pExtraInfo != NULL)
	{
		Character			*pTempDest = NULL;
		EFFECT_EXTRA_INFO    Info;
		
		// 1:N 데미지
		int Count = m_pExtraInfo->GetTargetCount();		
			
		for(int i = 0; i < Count; ++i)
		{
			if(m_pExtraInfo->GetTargetInfo(&pTempDest , &Info , i))
			{
				if(pTempDest)
				{
					pTempDest->m_lPreTime = g_nowTime;
				}
			}
		}
	}
	
	// 임펙트 프레임 화살 나가는 시점
	if(bIsAction)
	{		
		D3DXVECTOR3 vPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 vTempPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 vDir = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		D3DXMATRIX matRotY;		
		
		vPos.x = m_pSrcChr->GetPosTM()._41;
		vPos.y = m_pSrcChr->GetPosTM()._42;
		vPos.z = m_pSrcChr->GetPosTM()._43;		
		
		vTempPos = vPos;							
							
		D3DXMatrixRotationY(&matRotY , m_pSrcChr->GetDirection());
		D3DXVec3TransformCoord(&vDir , &vDir , &matRotY);	
		
		vTempPos += (75.0f * vDir);
		vTempPos.y = g_Map.Get__MapHeightF(vTempPos.x , vTempPos.z);
		g_Particle.SetEffSetCon(0.0f , EF_GIANT_GOLEM_ATTACK , 
								ESLT_POS , 1.5f , -1 , -1 , vTempPos, 0.0f);

		vTempPos += (75.0f * vDir);
		vTempPos.y = g_Map.Get__MapHeightF(vTempPos.x , vTempPos.z);
		g_Particle.SetEffSetCon(0.0f , EF_GIANT_GOLEM_ATTACK , 
								ESLT_POS , 1.5f , -1 , -1 , vTempPos, 0.0f);	

		vPos += (50.0f * vDir);

		g_Particle.m_fAddSize = 2.0f;
		g_Particle.SetEffSetCon(0.0f , EF_DOGMA_L2_WRATH_ONECUT , ESLT_POS , 5.1f , -1 , -1 , 
								 vPos, m_pSrcChr->GetDirection());
		
		vPos += (50.0f * vDir);
		g_Particle.m_fAddSize = 1.5f;
		g_Particle.SetEffSetCon(0.0f , EF_DOGMA_L2_WRATH_ONECUT , ESLT_POS , 5.1f , -1 , -1 , 
								 vPos, m_pSrcChr->GetDirection());

		vPos += (50.0f * vDir);
		g_Particle.m_fAddSize = 1.0f;
		g_Particle.SetEffSetCon(0.0f , EF_DOGMA_L2_WRATH_ONECUT , ESLT_POS , 5.1f , -1 , -1 , 
								 vPos, m_pSrcChr->GetDirection());
		
		Character			*pTempDest = NULL;
		EFFECT_EXTRA_INFO    Info;

		if(m_pExtraInfo != NULL)
		{
			// 1:N 데미지
			int Count = m_pExtraInfo->GetTargetCount();			
			
			for(int i = 0; i < Count; ++i)
			{
				if(m_pExtraInfo->GetTargetInfo(&pTempDest , &Info , i))
				{
					//................................................................................	
					// 어택 사운드
					//................................................................................	
					if(m_pExtraInfo != NULL)
						g_Particle.m_EffectManager.GetSkillAttackedSound(m_pExtraInfo->SkillCode , pTempDest);				
					

					g_Particle.SetEffSetCon(0.0f , EF_GIANT_GOLEM_ATTACKED , ESLT_CHARPOS , 5.1f , pTempDest->GetUnique() , 
											 pTempDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 
											 m_pSrcChr));
						
					Info.AfterEffect = EF_GIANT_GOLEM_ATTACKED;
					Info.DestType = ESLT_CHARPOS;
					Info.lifeTime = 5.1f;
						
					g_Particle.m_EffectManager.EffectTargetHit(m_pSrcChr , pTempDest , &Info);		
				}

				Info.Clear();
			}
		}
		
		SetDestroy();
	}	
	
	return TRUE;
}

//----------------------------------------------------------------------------------------------------------------------
int	CEFDogmaEffect::UpdateDogmaWrathOneCut(float dtime)
{
	///-- 공격 주체가 사라졌을때
	Character			*pTempDest = NULL;
	EFFECT_EXTRA_INFO    Info;
	
	if(!g_Particle.m_EffectManager.CheckCharacter(m_pSrcChr))
	{
		SetDestroy(TRUE);
		return TRUE;
	}

	return TRUE;
}

//----------------------------------------------------------------------------------------------------------------------
void CEFDogmaEffect::InitDogmaSwordFlash()
{
	if(!g_Particle.m_EffectManager.CheckCharacter(m_pSrcChr , m_pDestChr))
	{
		SetDestroy(TRUE);	
		return;
	}
}

//----------------------------------------------------------------------------------------------------------------------
int	CEFDogmaEffect::CheckDogmaSwordFlash()
{
	if(m_State == ES_SELF_DESTROY)
		return TRUE;
	
	if(!g_Particle.m_EffectManager.CheckCharacter(m_pSrcChr))
	{
		SetDestroy(TRUE);
		return TRUE;
	}

	BOOL bIsAction = FALSE;
	if(m_Src_Ani_Index != m_pSrcChr->m_ani_index)
		bIsAction = TRUE;

	if(m_pSrcChr->m_ani_frame == 44)
		bIsAction = TRUE;

	if(m_pExtraInfo != NULL)
	{
		Character			*pTempDest = NULL;
		EFFECT_EXTRA_INFO    Info;
		
		// 1:N 데미지
		int Count = m_pExtraInfo->GetTargetCount();		
			
		for(int i = 0; i < Count; ++i)
		{
			if(m_pExtraInfo->GetTargetInfo(&pTempDest , &Info , i))
			{
				if(pTempDest)
					pTempDest->m_lPreTime = g_nowTime;
			}
		}
	}

	if((m_pSrcChr->m_ani_frame % 5) == 0 && m_bWalkDust)
	{
		D3DXMATRIX matBone;			
		m_pSrcChr->GetBoneLinkTM(6 , matBone , FALSE);
		D3DXVECTOR3	Pos1(matBone._41, 0.0f, matBone._43);	
		Pos1.y = g_Map.PHF_GetHeight(Pos1.x , Pos1.z);
		g_Particle.DisplayUpdate(PT_WALK_DUST , Pos1, -1, 5.0f);
		
		m_pSrcChr->GetBoneLinkTM(7 , matBone , FALSE);
		D3DXVECTOR3	Pos2(matBone._41, 0.0f, matBone._43);	
		Pos2.y = g_Map.PHF_GetHeight(Pos2.x , Pos2.z);
		g_Particle.DisplayUpdate(PT_WALK_DUST , Pos2, -1, 5.0f);
	}
	
	// 임펙트 프레임 화살 나가는 시점
	if(bIsAction)
	{		
		D3DXVECTOR3 vPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 vTempPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 vDir = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		D3DXMATRIX matRotY;		
		
		vPos.x = m_pSrcChr->GetPosTM()._41;
		vPos.y = m_pSrcChr->GetPosTM()._42;
		vPos.z = m_pSrcChr->GetPosTM()._43;	

		D3DXMatrixRotationY(&matRotY , m_pSrcChr->GetDirection());
		D3DXVec3TransformCoord(&vDir , &vDir , &matRotY);	
		
		vPos += (150.0f * vDir);
		vPos.y = g_Map.Get__MapHeightF(vPos.x , vPos.z);		
		
		g_Particle.m_IsGUnitLinkBoneAndSizeFix = TRUE;
		g_Particle.m_fAddSize = 2.5f;
		g_Particle.SetEffSetCon(0.0f , EF_DOGMA_L2_SWORD_FLASH , ESLT_POS , 
								 10.1f , -1 , -1, vPos, 0.0f);
		
		Character			*pTempDest = NULL;
		EFFECT_EXTRA_INFO    Info;

		if(m_pExtraInfo != NULL)
		{
			// 1:N 데미지
			int Count = m_pExtraInfo->GetTargetCount();			
			
			for(int i = 0; i < Count; ++i)
			{
				if(m_pExtraInfo->GetTargetInfo(&pTempDest , &Info , i))
				{
					//................................................................................	
					// 어택 사운드
					//................................................................................	
					if(m_pExtraInfo != NULL)
						g_Particle.m_EffectManager.GetSkillAttackedSound(m_pExtraInfo->SkillCode , pTempDest);				
					

					g_Particle.SetEffSetCon(0.0f , EF_GIANT_GOLEM_ATTACKED , ESLT_CHARPOS , 5.1f , pTempDest->GetUnique() , 
											 pTempDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 
											 m_pSrcChr));
						
					Info.AfterEffect = EF_GIANT_GOLEM_ATTACKED;
					Info.DestType = ESLT_CHARPOS;
					Info.lifeTime = 5.1f;
						
					g_Particle.m_EffectManager.EffectTargetHit(m_pSrcChr , pTempDest , &Info);		
				}

				Info.Clear();
			}
		}
		
		SetDestroy();
	
	}	
	
	return TRUE;
}

//----------------------------------------------------------------------------------------------------------------------
int CEFDogmaEffect::UpdateDogmaSwordFlash(float dtime)
{
	///-- 공격 주체가 사라졌을때
	Character			*pTempDest = NULL;
	EFFECT_EXTRA_INFO    Info;
	
	if(!g_Particle.m_EffectManager.CheckCharacter(m_pSrcChr))
	{
		SetDestroy(TRUE);
		return TRUE;
	}	
	
	return TRUE;
}

//----------------------------------------------------------------------------------------------------------------------
void CEFDogmaEffect::InitDogmaWrathOneCut2()
{
	if(!g_Particle.m_EffectManager.CheckCharacter(m_pSrcChr , m_pDestChr))
	{
		SetDestroy(TRUE);	
		return;
	}
}

//----------------------------------------------------------------------------------------------------------------------
int CEFDogmaEffect::CheckDogmaWrathOneCut2()
{
	if(m_State == ES_SELF_DESTROY)
		return TRUE;
	
	if(!g_Particle.m_EffectManager.CheckCharacter(m_pSrcChr))
	{
		SetDestroy(TRUE);
		return TRUE;
	}

	BOOL bIsAction = FALSE;
	
	if(m_Src_Ani_Index != m_pSrcChr->m_ani_index)
		bIsAction = TRUE;

	if(m_pSrcChr->m_ani_frame == 99)
		bIsAction = TRUE;

	if(m_pExtraInfo != NULL)
	{
		Character			*pTempDest = NULL;
		EFFECT_EXTRA_INFO    Info;
		
		// 1:N 데미지
		int Count = m_pExtraInfo->GetTargetCount();		
			
		for(int i = 0; i < Count; ++i)
		{
			if(m_pExtraInfo->GetTargetInfo(&pTempDest , &Info , i))
			{
				if(pTempDest)
					pTempDest->m_lPreTime = g_nowTime;
			}
		}
	}

	if((m_pSrcChr->m_ani_frame % 5) == 0 && m_bWalkDust)
	{
		D3DXMATRIX matBone;			
		m_pSrcChr->GetBoneLinkTM(6 , matBone , FALSE);
		D3DXVECTOR3	Pos1(matBone._41, 0.0f, matBone._43);	
		Pos1.y = g_Map.PHF_GetHeight(Pos1.x , Pos1.z);
		g_Particle.DisplayUpdate(PT_WALK_DUST , Pos1, -1, 7.0f);
		
		m_pSrcChr->GetBoneLinkTM(7 , matBone , FALSE);
		D3DXVECTOR3	Pos2(matBone._41, 0.0f, matBone._43);	
		Pos2.y = g_Map.PHF_GetHeight(Pos2.x , Pos2.z);
		g_Particle.DisplayUpdate(PT_WALK_DUST , Pos2, -1, 7.0f);
	}
	
	// 임펙트 프레임 화살 나가는 시점
	if(bIsAction)
	{		
		D3DXVECTOR3 vPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 vTempPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 vDir = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		D3DXMATRIX matRotY;		
		
		vPos.x = m_pSrcChr->GetPosTM()._41;
		vPos.y = m_pSrcChr->GetPosTM()._42;
		vPos.z = m_pSrcChr->GetPosTM()._43;		
		
		vTempPos = vPos;							
							
		D3DXMatrixRotationY(&matRotY , m_pSrcChr->GetDirection());
		D3DXVec3TransformCoord(&vDir , &vDir , &matRotY);	

		vPos += (50.0f * vDir);

		g_Particle.m_fAddSize = 2.0f;
		g_Particle.SetEffSetCon(0.0f , EF_DOGMA_L3_WRATH_ONECUT2 , ESLT_POS , 5.1f , -1 , -1 , 
								 vPos, m_pSrcChr->GetDirection() - D3DXToRadian(21));		
	
		
		Character			*pTempDest = NULL;
		EFFECT_EXTRA_INFO    Info;

		if(m_pExtraInfo != NULL)
		{
			// 1:N 데미지
			int Count = m_pExtraInfo->GetTargetCount();			
			
			for(int i = 0; i < Count; ++i)
			{
				if(m_pExtraInfo->GetTargetInfo(&pTempDest , &Info , i))
				{
					//................................................................................	
					// 어택 사운드
					//................................................................................	
					if(m_pExtraInfo != NULL)
						g_Particle.m_EffectManager.GetSkillAttackedSound(m_pExtraInfo->SkillCode , pTempDest);				
					

					g_Particle.SetEffSetCon(0.0f , EF_DOGMA_L3_WRATH_ONECUT2ED , ESLT_CHARPOS , 5.1f , pTempDest->GetUnique() , 
											 pTempDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 
											 m_pSrcChr));
						
					Info.AfterEffect = EF_DOGMA_L3_WRATH_ONECUT2ED;
					Info.DestType = ESLT_CHARPOS;
					Info.lifeTime = 5.1f;
						
					g_Particle.m_EffectManager.EffectTargetHit(m_pSrcChr , pTempDest , &Info);		
				}

				Info.Clear();
			}
		}
		
		SetDestroy();
	}
	
	return TRUE;
}

//----------------------------------------------------------------------------------------------------------------------
int	CEFDogmaEffect::UpdateDogmaWrathOneCut2(float dtime)
{
	///-- 공격 주체가 사라졌을때
	Character			*pTempDest = NULL;
	EFFECT_EXTRA_INFO    Info;
	
	if(!g_Particle.m_EffectManager.CheckCharacter(m_pSrcChr))
	{
		SetDestroy(TRUE);
		return TRUE;
	}	
	
	return TRUE;	
}

// 발구르기-------------------------------------------------------------------------------------------------------------
void CEFDogmaEffect::InitDogmaFootQuake()
{
	if(!g_Particle.m_EffectManager.CheckCharacter(m_pSrcChr , m_pDestChr))
	{
		SetDestroy(TRUE);	
		return;
	}
}

//----------------------------------------------------------------------------------------------------------------------
int CEFDogmaEffect::CheckDogamFootQuake()
{
	if(m_State == ES_SELF_DESTROY)
		return TRUE;
	
	if(!g_Particle.m_EffectManager.CheckCharacter(m_pSrcChr))
	{
		SetDestroy(TRUE);
		return TRUE;
	}

	BOOL bIsAction = FALSE;
	
	if(m_Src_Ani_Index != m_pSrcChr->m_ani_index)
		bIsAction = TRUE;

	if(m_pSrcChr->m_ani_frame == 111)
		bIsAction = TRUE;
	
	if(m_pExtraInfo != NULL)
	{
		Character			*pTempDest = NULL;
		EFFECT_EXTRA_INFO    Info;
		
		// 1:N 데미지
		int Count = m_pExtraInfo->GetTargetCount();		
			
		for(int i = 0; i < Count; ++i)
			if(m_pExtraInfo->GetTargetInfo(&pTempDest , &Info , i))
			{
				if(pTempDest)
					pTempDest->m_lPreTime = g_nowTime;
			}
	}

	if((m_pSrcChr->m_ani_frame % 5) == 0 && m_bWalkDust)
	{
		D3DXMATRIX matBone;			
		m_pSrcChr->GetBoneLinkTM(6 , matBone , FALSE);
		D3DXVECTOR3	Pos1(matBone._41, 0.0f, matBone._43);	
		Pos1.y = g_Map.PHF_GetHeight(Pos1.x , Pos1.z);
		g_Particle.DisplayUpdate(PT_WALK_DUST , Pos1, -1, 8.0f);
		
		m_pSrcChr->GetBoneLinkTM(7 , matBone , FALSE);
		D3DXVECTOR3	Pos2(matBone._41, 0.0f, matBone._43);	
		Pos2.y = g_Map.PHF_GetHeight(Pos2.x , Pos2.z);
		g_Particle.DisplayUpdate(PT_WALK_DUST , Pos2, -1, 8.0f);
	}
	
	// 임펙트 프레임 화살 나가는 시점
	if(bIsAction)
	{		
		D3DXVECTOR3 vPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 vTempPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 vDir = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		D3DXMATRIX matRotY;		
		
		vPos.x = m_pSrcChr->GetPosTM()._41;
		vPos.y = m_pSrcChr->GetPosTM()._42;
		vPos.z = m_pSrcChr->GetPosTM()._43;		
		
		vTempPos = vPos;							
							
		D3DXMatrixRotationY(&matRotY , m_pSrcChr->GetDirection());
		D3DXVec3TransformCoord(&vDir , &vDir , &matRotY);	

		vPos += (10.0f * vDir);

		g_Particle.m_fAddSize = 2.0f;
		g_Particle.SetEffSetCon(0.0f , EF_DOGMA_L3_FOOT_QUAKE , ESLT_POS , 5.1f , -1 , -1 , 
								 vPos, m_pSrcChr->GetDirection());	
		
		D3DXMATRIX matBone;
		m_pSrcChr->GetBoneLinkTM(6 , matBone , FALSE);
		D3DXVECTOR3	Pos2(matBone._41, 0.0f, matBone._43);	
		Pos2.y = g_Map.PHF_GetHeight(Pos2.x , Pos2.z);		

		g_Particle.m_fAddSize = 2.0f;
		g_Particle.SetEffSetCon(0.0f , EF_GIANT_GOLEM_ATTACK , 
								ESLT_POS , 1.5f , -1 , -1 , Pos2, 0.0f);

		g_Particle.RockCamera((rand() % 200) + 300);
	

		Character			*pTempDest = NULL;
		EFFECT_EXTRA_INFO    Info;

		if(m_pExtraInfo != NULL)
		{
			// 1:N 데미지
			int Count = m_pExtraInfo->GetTargetCount();			
			
			for(int i = 0; i < Count; ++i)
			{
				if(m_pExtraInfo->GetTargetInfo(&pTempDest , &Info , i))
				{
					//................................................................................	
					// 어택 사운드
					//................................................................................	
					if(m_pExtraInfo != NULL)
						g_Particle.m_EffectManager.GetSkillAttackedSound(m_pExtraInfo->SkillCode , pTempDest);				
					

					g_Particle.SetEffSetCon(0.0f , EF_GIANT_GOLEM_ATTACKED , ESLT_CHARPOS , 5.1f , pTempDest->GetUnique() , 
											 pTempDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 
											 m_pSrcChr));
						
					Info.AfterEffect = EF_GIANT_GOLEM_ATTACKED;
					Info.DestType = ESLT_CHARPOS;
					Info.lifeTime = 5.1f;
						
					g_Particle.m_EffectManager.EffectTargetHit(m_pSrcChr , pTempDest , &Info);		
				}

				Info.Clear();
			}
		}
		
		SetDestroy();	
	}	
	
	return TRUE;
}

//----------------------------------------------------------------------------------------------------------------------
int CEFDogmaEffect::UpdateDogmaFootQuake(float dtime)
{
	///-- 공격 주체가 사라졌을때
	Character			*pTempDest = NULL;
	EFFECT_EXTRA_INFO    Info;
	
	if(!g_Particle.m_EffectManager.CheckCharacter(m_pSrcChr))
	{
		SetDestroy(TRUE);
		return TRUE;
	}		
	
	return TRUE;
}

//----------------------------------------------------------------------------------------------------------------------
void CEFDogmaEffect::InitDogmaCircleSword()
{
	if(!g_Particle.m_EffectManager.CheckCharacter(m_pSrcChr , m_pDestChr))
	{
		SetDestroy(TRUE);	
		return;
	}
}	

//----------------------------------------------------------------------------------------------------------------------
int CEFDogmaEffect::CheckDogmaCircleSword()
{
	if(m_State == ES_SELF_DESTROY)
		return TRUE;
	
	if(!g_Particle.m_EffectManager.CheckCharacter(m_pSrcChr))
	{
		SetDestroy(TRUE);
		return TRUE;
	}

	BOOL bIsAction = FALSE;
	
	if(m_Src_Ani_Index != m_pSrcChr->m_ani_index)
		bIsAction = TRUE;

	if(m_pSrcChr->m_ani_frame == 90)
		bIsAction = TRUE;

	if(m_pExtraInfo != NULL)
	{
		Character			*pTempDest = NULL;
		EFFECT_EXTRA_INFO    Info;
		
		// 1:N 데미지
		int Count = m_pExtraInfo->GetTargetCount();		
			
		for(int i = 0; i < Count; ++i)
		{
			if(m_pExtraInfo->GetTargetInfo(&pTempDest , &Info , i))
			{
				if(pTempDest)
				{
					pTempDest->m_lPreTime = g_nowTime;
				}
			}
		}
	}

	if((m_pSrcChr->m_ani_frame % 5) == 0 && m_bWalkDust)
	{
		D3DXMATRIX matBone;			
		m_pSrcChr->GetBoneLinkTM(6 , matBone , FALSE);
		D3DXVECTOR3	Pos1(matBone._41, 0.0f, matBone._43);	
		Pos1.y = g_Map.PHF_GetHeight(Pos1.x , Pos1.z);
		g_Particle.DisplayUpdate(PT_WALK_DUST , Pos1, -1, 8.0f);
		
		m_pSrcChr->GetBoneLinkTM(7 , matBone , FALSE);
		D3DXVECTOR3	Pos2(matBone._41, 0.0f, matBone._43);	
		Pos2.y = g_Map.PHF_GetHeight(Pos2.x , Pos2.z);
		g_Particle.DisplayUpdate(PT_WALK_DUST , Pos2, -1, 8.0f);
	}	
	
	// 임펙트 프레임 화살 나가는 시점
	if(bIsAction)
	{		
		D3DXVECTOR3 vPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 vTempPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 vDir = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		D3DXMATRIX matRotY;		
		
		vPos.x = m_pSrcChr->GetPosTM()._41;
		vPos.y = m_pSrcChr->GetPosTM()._42;
		vPos.z = m_pSrcChr->GetPosTM()._43;	
		
		vPos.y += 30.0f;
		
		g_Particle.m_fAddSize = 6.0f;
		g_Particle.SetEffSetCon(0.0f , EF_DOGMA_L4_CIRCLE_SWORD , 
								ESLT_POS , 5.1f , -1 , -1, vPos, m_pSrcChr->GetDirection());
	
		Character			*pTempDest = NULL;
		EFFECT_EXTRA_INFO    Info;

		if(m_pExtraInfo != NULL)
		{
			// 1:N 데미지
			int Count = m_pExtraInfo->GetTargetCount();			
			
			for(int i = 0; i < Count; ++i)
			{
				if(m_pExtraInfo->GetTargetInfo(&pTempDest , &Info , i))
				{
					//................................................................................	
					// 어택 사운드
					//................................................................................	
					if(m_pExtraInfo != NULL)
						g_Particle.m_EffectManager.GetSkillAttackedSound(m_pExtraInfo->SkillCode , pTempDest);				
					

					g_Particle.SetEffSetCon(0.0f , EF_GIANT_GOLEM_ATTACKED , ESLT_CHARPOS , 5.1f , pTempDest->GetUnique() , 
											 pTempDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 
											 m_pSrcChr));
						
					Info.AfterEffect = EF_GIANT_GOLEM_ATTACKED;
					Info.DestType = ESLT_CHARPOS;
					Info.lifeTime = 5.1f;
						
					g_Particle.m_EffectManager.EffectTargetHit(m_pSrcChr , pTempDest , &Info);		
				}

				Info.Clear();
			}
		}
		
		SetDestroy();	
	}	
	
	return TRUE;
}	

//----------------------------------------------------------------------------------------------------------------------
int CEFDogmaEffect::UpdateDogmaCircleSword(float dtime)
{
	///-- 공격 주체가 사라졌을때
	Character			*pTempDest = NULL;
	EFFECT_EXTRA_INFO    Info;
	
	if(!g_Particle.m_EffectManager.CheckCharacter(m_pSrcChr))
	{
		SetDestroy(TRUE);
		return TRUE;
	}	
	
	return TRUE;
}

// 변신초기화-----------------------------------------------------------------------------------------------------------
void CEFDogmaEffect::InitMasterDogmaAEffect()
{
	m_IsConEffect = FALSE;

	CParticleData *  pData = NULL;

	if((pData = g_Particle.m_ParticleData.GetKeyDataPointer(24378)) == NULL)// 콜로니코어 레벨2
	{
		SetDestroy();
		return;
	}

	for(int i = 0; i < ME_MASTER_FIRE_EFFECT_COUNT; ++i)
	{
		m_kMasterFireLink[i].MakeLink(pData);
		m_kMasterFireLink[i].m_fSize = m_fMasterFireSize;
	}
}

//----------------------------------------------------------------------------------------------------------------------
int	CEFDogmaEffect::UpdateMasterDogmaAEffect(float dtime)
{
	if(g_Particle.m_EffectManager.CheckDie(m_pSrcChr))
	{
		SetDestroy();
		return TRUE;
	}	
	
	if(!g_Particle.m_EffectManager.CheckCharacter(m_pSrcChr))
	{
		SetDestroy();	
		return TRUE;
	}	
	
	return TRUE;
}

// 변신 후 화염이펙트 그린다--------------------------------------------------------------------------------------------
int	CEFDogmaEffect::RenderMasterDogmaAEffect(float dtime)
{
	DWORD dCullMode = g_RenderManager.GetCullMode();
	DWORD dZWriteEnable = g_RenderManager.GetZWriteEnable();
	DWORD dZEnable = g_RenderManager.GetZEnable();
	DWORD dAlphaBlendEnable = g_RenderManager.GetAlphaBlendEnable();
	DWORD dSrcBlend = g_RenderManager.GetSrcBlend();
	DWORD dDestBlend = g_RenderManager.GetDestBlend();
	DWORD dFillMode = g_RenderManager.GetFillMode();

	
	D3DXMATRIX   matTemp;
	D3DXVECTOR3  vPos;
	
	for(int i = 0; i < ME_MASTER_FIRE_EFFECT_COUNT; ++i)
	{
		m_pSrcChr->GetDirectBoneLinkTM(m_aiMasterFireBone[i] , matTemp);	
		vPos.x = matTemp._41;
		vPos.y = matTemp._42;
		vPos.z = matTemp._43;
		
		m_kMasterFireLink[i].SetLinkPos(vPos);
		m_kMasterFireLink[i].UpdateLink(dtime);
		m_kMasterFireLink[i].Render();
	}

	g_RenderManager.SetCullMode(dCullMode);	
	g_RenderManager.SetZWriteEnable(dZWriteEnable);
	g_RenderManager.SetZEnable(dZEnable);	
	g_RenderManager.SetAlphaBlendEnable(dAlphaBlendEnable);
	g_RenderManager.SetSrcBlend(dSrcBlend);
	g_RenderManager.SetDestBlend( dDestBlend);	
	g_RenderManager.SetFillMode(dFillMode);

	return TRUE;
}
