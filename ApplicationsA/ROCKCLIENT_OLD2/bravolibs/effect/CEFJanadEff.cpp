#include "CEFJanadEff.h"
#include "..\\Effect.h"

extern long		 g_nowTime;

//----------------------------------------------------------------------------------------------------------------------
CEFJanadEff::CEFJanadEff() : m_fSwordFlashOfDevilRadius(10.0f)
{
	m_dDestCell = 0;
	
	m_InitTime = 0;
	m_RangeTime = 0;
	m_PrevTime = 0;
	m_GapTime = 0;
	m_RunningTime = 300;

	m_fRadius = 126.0f;

	m_vTempPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vCenterPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

    m_ISShot = FALSE;;
	m_ISDamageProcess = FALSE;

#ifdef HHM_SEALED_TEMPLE_BOSSMOB_EFFECT_LIMIT_20091207

	m_iSwordFlashOfDevilCount = 2;

	m_iMargmaBustCount = 2;
	m_bMargmaBust = false;

	m_bDispel = false;
	
	m_bDarkSwordStorm = false;
	m_iDarkSwordStormCount = 0;

#else

	m_iSwordFlashOfDevilCount = 25;

	m_iMargmaBustCount = 20;
	m_bMargmaBust = false;
	
	m_bDispel = false;
	
	m_bDarkSwordStorm = true;
	m_iDarkSwordStormCount = 18;

#endif// #ifdef HHM_SEALED_TEMPLE_BOSSMOB_EFFECT_LIMIT_20091207
}

//----------------------------------------------------------------------------------------------------------------------
void CEFJanadEff::Init(void)
{
	if(!g_Particle.m_EffectManager.CheckCharacter(m_pSrcChr))
	{
		SetDestroy(TRUE);	
		return;
	}	

	m_Src_Ani_Index = m_pSrcChr->m_ani_index;
	//................................................................................	
	// 스킬 발동 사운드
	//................................................................................	
	if(m_pExtraInfo != NULL)
		g_Particle.m_EffectManager.GetSkillImfactSound(m_pExtraInfo->SkillCode , m_pSrcChr);		
	
	switch(m_ID)
	{
		case EF_JANAD_DASHTRIPLE_ATTACK:
			InitDashTripleAttack();
			break;

		case EF_JANAD_CALLDEVILSOUL:
			InitCallDevilSoul();
			break;

		case EF_JANAD_DARKSWORDSTORM:
			InitDarkSwordStorm();
			break;

		case EF_JANAD_DISPEL:
			InitDispel();
			break;

		case EF_JANAD_MAGMA_BUST:
			InitMargmaBust();
			break;
	}
}

//----------------------------------------------------------------------------------------------------------------------
int CEFJanadEff::CheckFrame(void)
{
	if(m_State == ES_SELF_DESTROY)
		return TRUE;
	
	if(!g_Particle.m_EffectManager.CheckCharacter(m_pSrcChr))
	{
		SetDestroy(TRUE);
		return TRUE;
	}
	
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

	if( m_Src_Ani_Index != m_pSrcChr->m_ani_index)
		SetDestroy(TRUE);
	
	switch(m_ID)
	{
		case EF_JANAD_DASHTRIPLE_ATTACK:
			return CheckDashTripleAttack();

		case EF_JANAD_CALLDEVILSOUL:
			return CheckCallDevilSoul();

		case EF_JANAD_DARKSWORDSTORM:
			return CheckDarkSwordStorm();

		case EF_JANAD_DISPEL:
			return CheckDispel();

		case EF_JANAD_MAGMA_BUST:
			return CheckMargmaBust();

		case EF_JANAD_SWORD_FLASH_OF_DEVIL:
			return CheckSwordFlashOfDevil();
	}	
	
	return TRUE;
}

//----------------------------------------------------------------------------------------------------------------------
int CEFJanadEff::Update(float dtime)
{
	switch(m_ID)
	{
		case EF_JANAD_DARKSWORDSTORM:
			return UpdateDarkSwordStorm(dtime);

		case EF_JANAD_MAGMA_BUST:
			return UpdateMargmaBust(dtime);

		case EF_JANAD_SWORD_FLASH_OF_DEVIL:
			return UpdateSwordFlashOfDevil(dtime);
	}	
	
	return TRUE;
}

//----------------------------------------------------------------------------------------------------------------------
int CEFJanadEff::Render(float dtime)
{
	switch(m_ID)
	{
		case EF_JANAD_CALLDEVILSOUL:
			return RenderCallDevilSoul(dtime);

		case EF_JANAD_DARKSWORDSTORM:
			return RenderDarkSwordStorm(dtime);

		case EF_JANAD_DISPEL:
			return RenderDispel(dtime);

		case EF_JANAD_MAGMA_BUST:
			return RenderMargmaBust(dtime);

		case EF_JANAD_SWORD_FLASH_OF_DEVIL:
			return RednerSwordFlashOfDevil(dtime);
	}	
	
	return TRUE;
}

//----------------------------------------------------------------------------------------------------------------------
int CEFJanadEff::CheckDashTripleAttack()
{
	if(m_pSrcChr->m_ani_frame == 40)
	{
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
					
					Info.AfterEffect = EF_JANAD_MAGMA_BUST;
					Info.DestType = ESLT_CPOS_ICAMERA;
					Info.lifeTime = 5.1f;
					
					g_Particle.m_EffectManager.EffectTargetHit(m_pSrcChr , pTempDest , &Info);					
					
					g_Particle.SetEffSetCon(0.0f , EF_JANAD_DARKSWORDSTORMED , ESLT_CPOS_ICAMERA , 5.1f , 
						pTempDest->GetUnique() , pTempDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr));
				}
				
				Info.Clear();
			}
		}		
	}
	return TRUE;
}

//----------------------------------------------------------------------------------------------------------------------
void CEFJanadEff::InitCallDevilSoul()
{
	///-- 이거 소환 애니메이션 들어가면.. 딜레이 주고
	///-- 일정 시간 딜레이 후에... 소환 된 대상이 나오게 해야 할듯... 

	CParticleData *  pData = NULL;
	
	if((pData = g_Particle.m_ParticleData.GetKeyDataPointer(24767)) == NULL)
	{
		SetDestroy();
		return;
	}

	m_ParticleLink[0].MakeLink(pData);
	m_ParticleLink[0].m_fSize = 2.5f;
}

//----------------------------------------------------------------------------------------------------------------------
int	CEFJanadEff::CheckCallDevilSoul()
{
	if(m_pSrcChr->m_ani_frame == 80)
		SetDestroy();	
	
	return TRUE;
}

//----------------------------------------------------------------------------------------------------------------------
int	CEFJanadEff::RenderCallDevilSoul(float dtime)
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

	m_pSrcChr->GetDirectBoneLinkTM(12 , matTemp);	
	vPos.x = matTemp._41;
	vPos.y = matTemp._42;
	vPos.z = matTemp._43;
	
	m_ParticleLink[0].SetLinkPos(vPos);
	m_ParticleLink[0].UpdateLink(dtime);
	m_ParticleLink[0].Render();		
	
	g_RenderManager.SetCullMode(dCullMode);	
	g_RenderManager.SetZWriteEnable(dZWriteEnable);
	g_RenderManager.SetZEnable(dZEnable);	
	g_RenderManager.SetAlphaBlendEnable(dAlphaBlendEnable);
	g_RenderManager.SetSrcBlend(dSrcBlend);
	g_RenderManager.SetDestBlend( dDestBlend);	
	g_RenderManager.SetFillMode(dFillMode);	
	
	return TRUE;
}

//----------------------------------------------------------------------------------------------------------------------
void CEFJanadEff::InitDarkSwordStorm()
{
	//파티클 파이볼구조의 파이어 이미지와 파이어 꼬리가 필요하다 .. 
	CParticleData *pData = NULL;
	CParticleData *pData2 = NULL; 

	if((pData = g_Particle.m_ParticleData.GetKeyDataPointer(24782)) == NULL)
		return;
	
	if((pData2 = g_Particle.m_ParticleData.GetKeyDataPointer(24783)) == NULL)
		return;

	m_vCenterPos.x = m_pSrcChr->GetPosTM()._41;
	m_vCenterPos.y = m_pSrcChr->GetPosTM()._42;
	m_vCenterPos.z = m_pSrcChr->GetPosTM()._43;

	D3DXVECTOR3 vDir = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	D3DXMATRIX matRotY;
	D3DXMatrixRotationY(&matRotY , m_pSrcChr->GetDirection());
	D3DXVec3TransformCoord(&vDir , &vDir , &matRotY);	

	m_vCenterPos += (50.0f * vDir);
	float fMinRadius = 10.0f;

	D3DXVECTOR3 vMaxRad(m_fRadius, 0.0f, 0.0f);
	D3DXVECTOR3 vMinRad(fMinRadius, 0.0f, 0.0f);	

	for(int i = 0; i < m_iDarkSwordStormCount; ++i)
	{
		m_ParticleLink[i].MakeLink(pData2);			
		m_ParticleLink2[i].MakeLink(pData);
		m_ParticleLink[i].m_IsPosFix = FALSE;
		m_ParticleLink2[i].m_IsPosFix = FALSE;
		
		float fRad = (2 * D3DX_PI) * (i / 18.0f);
		D3DXMATRIX	matRot;
		D3DXMatrixRotationY(&matRot, fRad);
		D3DXVec3TransformCoord(&m_vStartPos[i] , &vMinRad , &matRot);
		D3DXVec3TransformCoord(&m_vEndPos[i] , &vMaxRad , &matRot);
	}
}

//----------------------------------------------------------------------------------------------------------------------
int	CEFJanadEff::CheckDarkSwordStorm()
{
	if(m_ISShot)
		return TRUE;
	
	if(m_pSrcChr->m_ani_frame == 41)
	{
		m_ISShot = TRUE;
		m_InitTime = g_nowTime;
		
		D3DXVECTOR3 vPos;
		
		vPos = m_vCenterPos;
		vPos.y += 30.0f;			
		
		if(m_bDarkSwordStorm)
		{
			g_Particle.m_fAddSize = 2.0f;
			g_Particle.SetEffSetCon(0.0f , EF_JANAD_DARKSWORDSTORM , ESLT_POS , 
				10.1f , -1 , -1 , vPos, m_pSrcChr->GetDirection());
			
			g_Particle.SetEffSetCon(0.0f , EF_B_SKEL_BAIN_DARK_STORM , ESLT_POS , 
				10.1f , -1 , -1 , vPos);
		}
	
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
					
					Info.AfterEffect = EF_JANAD_DARKSWORDSTORMED;
					Info.DestType = ESLT_CPOS_ICAMERA;
					Info.lifeTime = 5.1f;
					
					g_Particle.m_EffectManager.EffectTargetHit(m_pSrcChr , pTempDest , &Info);					
					
					g_Particle.SetEffSetCon(0.0f , EF_JANAD_DARKSWORDSTORMED , ESLT_CPOS_ICAMERA , 5.1f , 
						pTempDest->GetUnique() , pTempDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr));
				}

				Info.Clear();
			}
		}	
	}	

	return TRUE;
}

//----------------------------------------------------------------------------------------------------------------------
int	CEFJanadEff::UpdateDarkSwordStorm(float dtime)
{
	if(!m_ISShot)
		return TRUE;

	if(SAFE_TIME_COMPARE(SAFE_TIME_SUB(g_nowTime , m_InitTime) , > , m_RunningTime))
	{
		D3DXVECTOR3 vPos;
		
		vPos = m_vCenterPos;
		vPos.y += 30.0f;			
	
		g_Particle.m_fAddSize = 3.0f;
		g_Particle.SetEffSetCon(0.0f , EF_JANAD_DARKSWORDSTORM , ESLT_POS , 
					10.1f , -1 , -1 , vPos, m_pSrcChr->GetDirection());
		
		SetDestroy();		
		return TRUE;
	}	

	return TRUE;
}

//----------------------------------------------------------------------------------------------------------------------
int	CEFJanadEff::RenderDarkSwordStorm(float dtime)
{
	if(!m_ISShot)
		return TRUE;

	D3DXMATRIX		  matLocal;
	D3DXMatrixIdentity(&matLocal);		
	g_RenderManager.SetTransform(D3DTS_WORLD , &matLocal);	
	
	D3DXVECTOR3 vPos;
	float fRange = SAFE_TIME_SUB(g_nowTime , m_InitTime)/(float)m_RunningTime;	
	if(fRange > 1.0f)
	    fRange = 1.0f;

	g_RenderManager.DisableTextureStageState(1);

	DWORD dFVF = g_RenderManager.GetFVF();
	DWORD dAlphaBlendEnable = g_RenderManager.GetAlphaBlendEnable();
	DWORD dCullMode = g_RenderManager.GetCullMode();
	DWORD dZEnable = g_RenderManager.GetZEnable();
	DWORD dZWriteEnable = g_RenderManager.GetZWriteEnable();
	DWORD dLighting = g_RenderManager.GetLighting();
	DWORD dSrcBlend = g_RenderManager.GetSrcBlend();
	DWORD dDestBlend = g_RenderManager.GetDestBlend(); 

	DWORD dColOp = g_RenderManager.GetTextureColorOP(0);
	DWORD dColArg1 = g_RenderManager.GetTextureColorArg1(0);
	DWORD dColArg2 = g_RenderManager.GetTextureColorArg2(0);

	DWORD dAlpOp = g_RenderManager.GetTextureAlphaOP(0);
	DWORD dAlpArg1 = g_RenderManager.GetTextureAlphaArg1(0);
	DWORD dAlpArg2 = g_RenderManager.GetTextureAlphaArg2(0);
	
	g_RenderManager.SetAlphaBlendEnable(TRUE);	
	g_RenderManager.SetCullMode(D3DCULL_NONE);
	
	g_RenderManager.SetZEnable(D3DZB_TRUE);
	g_RenderManager.SetZWriteEnable(FALSE);

	float fHeight = g_Map.PHF_GetHeight(m_vCenterPos.x , m_vCenterPos.z);

	for(int i = 0; i < m_iDarkSwordStormCount; ++i)
	{
		D3DXVec3Lerp(&vPos , &m_vStartPos[i] , &m_vEndPos[i] , fRange);
		vPos += D3DXVECTOR3(m_vCenterPos.x , 0.0f , m_vCenterPos.z);		
		vPos.y = fHeight + 5.0f;

		m_ParticleLink[i].SetLinkPos(vPos);		
		m_ParticleLink[i].UpdateLink(dtime);
		m_ParticleLink[i].Render();

		m_ParticleLink2[i].SetLinkPos(vPos);		
		m_ParticleLink2[i].UpdateLink(dtime);
		m_ParticleLink2[i].Render();
	}

	g_RenderManager.SetFVF(dFVF);
	g_RenderManager.SetAlphaBlendEnable(dAlphaBlendEnable);
	g_RenderManager.SetCullMode(dCullMode);
	g_RenderManager.SetZEnable(dZEnable);
	g_RenderManager.SetZWriteEnable(dZWriteEnable);
	g_RenderManager.SetLighting(dLighting);
	g_RenderManager.SetSrcBlend(dSrcBlend);
	g_RenderManager.SetDestBlend(dDestBlend);

	g_RenderManager.SetTextureColorOP  (0 , dColOp);
	g_RenderManager.SetTextureColorArg1(0 , dColArg1);
	g_RenderManager.SetTextureColorArg2(0 , dColArg2);
	
	g_RenderManager.SetTextureAlphaOP  (0 , dAlpOp);
	g_RenderManager.SetTextureAlphaArg1(0 , dAlpArg1);
	g_RenderManager.SetTextureAlphaArg2(0 , dAlpArg2);

	return TRUE;
}

//----------------------------------------------------------------------------------------------------------------------
void CEFJanadEff::InitDispel()
{
	if(!m_bDispel)
		return;

	CParticleData *  pData = NULL;
	if((pData = g_Particle.m_ParticleData.GetKeyDataPointer(24769)) == NULL)
	{
		SetDestroy();
		return;
	}

	m_ParticleLink[0].MakeLink(pData);
	m_ParticleLink[0].m_fSize = 2.5f;
}

//----------------------------------------------------------------------------------------------------------------------
int	CEFJanadEff::CheckDispel()
{
	if(m_pSrcChr->m_ani_frame == 82)
	{
		D3DXVECTOR3 vPos;

		vPos.x = m_pSrcChr->GetPosTM()._41;
		vPos.y = m_pSrcChr->GetPosTM()._42;
		vPos.z = m_pSrcChr->GetPosTM()._43;
		
		g_Particle.m_fAddSize = 1.5f;
		g_Particle.SetEffSetCon(0.0f , EF_JANAD_DISPEL1136 , ESLT_POS , 
						 10.0f , -1 , -1, vPos, m_pSrcChr->GetDirection() );		
		
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
					
					Info.AfterEffect = EF_JANAD_DISPELED;
					Info.DestType = ESLT_CPOS_ICAMERA;
					Info.lifeTime = 5.1f;
					
					g_Particle.m_EffectManager.EffectTargetHit(m_pSrcChr , pTempDest , &Info);					
					
					g_Particle.SetEffSetCon(0.0f , EF_JANAD_DARKSWORDSTORMED , ESLT_CPOS_ICAMERA , 5.1f , 
						pTempDest->GetUnique() , pTempDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr));
				}

				Info.Clear();
			}
		}		
		
		SetDestroy();	
	}	
	
	return TRUE;
}

//----------------------------------------------------------------------------------------------------------------------
int	CEFJanadEff::RenderDispel(float dtime)
{
	if(!m_bDispel)
		return TRUE;

	DWORD dCullMode = g_RenderManager.GetCullMode();
	DWORD dZWriteEnable = g_RenderManager.GetZWriteEnable();
	DWORD dZEnable = g_RenderManager.GetZEnable();
	DWORD dAlphaBlendEnable = g_RenderManager.GetAlphaBlendEnable();
	DWORD dSrcBlend = g_RenderManager.GetSrcBlend();
	DWORD dDestBlend = g_RenderManager.GetDestBlend();
	DWORD dFillMode = g_RenderManager.GetFillMode();

	D3DXMATRIX   matTemp;
	D3DXVECTOR3  vPos;

	m_pSrcChr->GetDirectBoneLinkTM(12 , matTemp);	
	vPos.x = matTemp._41;
	vPos.y = matTemp._42;
	vPos.z = matTemp._43;
	
	m_ParticleLink[0].SetLinkPos(vPos);
	m_ParticleLink[0].UpdateLink(dtime);
	m_ParticleLink[0].Render();		
	
	g_RenderManager.SetCullMode(dCullMode);	
	g_RenderManager.SetZWriteEnable(dZWriteEnable);
	g_RenderManager.SetZEnable(dZEnable);	
	g_RenderManager.SetAlphaBlendEnable(dAlphaBlendEnable);
	g_RenderManager.SetSrcBlend(dSrcBlend);
	g_RenderManager.SetDestBlend( dDestBlend);	
	g_RenderManager.SetFillMode(dFillMode);	
	
	return TRUE;
}

//----------------------------------------------------------------------------------------------------------------------
void CEFJanadEff::InitMargmaBust()
{
	CParticleData *  pData = NULL;
	
	if((pData = g_Particle.m_ParticleData.GetKeyDataPointer(24776)) == NULL)
	{
		SetDestroy();
		return;
	}

	m_ParticleLink[0].MakeLink(pData);
	m_ParticleLink[0].m_fSize = 2.5f;
}

//----------------------------------------------------------------------------------------------------------------------
int	CEFJanadEff::CheckMargmaBust()
{
	if(m_pSrcChr->m_ani_frame == 41)
	{
		D3DXVECTOR3  vPos;
		
		vPos.x = m_pSrcChr->GetPosTM()._41;
		vPos.y = m_pSrcChr->GetPosTM()._42;
		vPos.z = m_pSrcChr->GetPosTM()._43;

		D3DXVECTOR3 vDir = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		D3DXMATRIX matRotY;
		D3DXMatrixRotationY(&matRotY , m_pSrcChr->GetDirection());
		D3DXVec3TransformCoord(&vDir , &vDir , &matRotY);	

		vPos += (30.0f * vDir);
		m_vCenterPos = vPos;

		m_fRadius = 150.0f;
		
		if(m_bMargmaBust)
			g_Particle.SetEffSetCon(0.0f , EF_BARAGOS_DRAGON_BRESS1134 , ESLT_POS , 
				4.1f , -1 , -1 , m_vCenterPos, 0.0f);

		for(int i = 0; i < m_iMargmaBustCount; ++i)
		{
			float fR = Random(-m_fRadius , m_fRadius);
			D3DXVECTOR3  vPos = D3DXVECTOR3(fR , 0.0f , 0.0f);
			D3DXMATRIX   matRot;
			D3DXMatrixRotationY(&matRot , D3DXToRadian(Random(0 , 360)));
			///-- 반경Radius 이내로 360 회전된 위치를 구한다.	
			D3DXVec3TransformCoord(&vPos , &vPos , &matRot);
			vPos += D3DXVECTOR3(m_vCenterPos.x , 0.0f , m_vCenterPos.z);
			vPos.y = g_Map.PHF_GetHeight(vPos.x , vPos.z);
			vPos.y += 10.0f;

			g_Particle.m_fAddSize = 1.0f;
			g_Particle.SetEffSetCon(0.0f , EF_JANAD_MAGMA_BUSTED , ESLT_POS , 5.1f , -1 , -1 , vPos, 0.0f);
		}			
		
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
					
					Info.AfterEffect = EF_JANAD_MAGMA_BUST;
					Info.DestType = ESLT_CPOS_ICAMERA;
					Info.lifeTime = 5.1f;
					
					g_Particle.m_EffectManager.EffectTargetHit(m_pSrcChr , pTempDest , &Info);					
					
					g_Particle.SetEffSetCon(0.0f , EF_JANAD_DARKSWORDSTORMED , ESLT_CPOS_ICAMERA , 5.1f , 
						pTempDest->GetUnique() , pTempDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr));
				}

				Info.Clear();
			}
		}		
		
		SetDestroy();
	
		m_ISShot = TRUE;
	}
	
	return TRUE;
}

//----------------------------------------------------------------------------------------------------------------------
int	CEFJanadEff::UpdateMargmaBust(float dtime)
{
	if(m_ISShot)
		return TRUE;
	return TRUE;
}

//----------------------------------------------------------------------------------------------------------------------
int	CEFJanadEff::RenderMargmaBust(float dtime)
{
	if(m_ISShot)
		return TRUE;
	
	DWORD dCullMode = g_RenderManager.GetCullMode();
	DWORD dZWriteEnable = g_RenderManager.GetZWriteEnable();
	DWORD dZEnable = g_RenderManager.GetZEnable();
	DWORD dAlphaBlendEnable = g_RenderManager.GetAlphaBlendEnable();
	DWORD dSrcBlend = g_RenderManager.GetSrcBlend();
	DWORD dDestBlend = g_RenderManager.GetDestBlend();
	DWORD dFillMode = g_RenderManager.GetFillMode();

	D3DXMATRIX   matTemp;

	D3DXVECTOR3 vPos;

	vPos.x = m_pSrcChr->GetPosTM()._41;
	vPos.y = m_pSrcChr->GetPosTM()._42;
	vPos.z = m_pSrcChr->GetPosTM()._43;

	D3DXVECTOR3 vDir = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	D3DXMATRIX matRotY;
	D3DXMatrixRotationY(&matRotY , m_pSrcChr->GetDirection());
	D3DXVec3TransformCoord(&vDir , &vDir , &matRotY);	

	vPos += (30.0f * vDir);
	vPos.y += 20.0f;
	
	if(m_bMargmaBust)
	{
		m_ParticleLink[0].SetLinkPos(vPos);
		m_ParticleLink[0].UpdateLink(dtime);
		m_ParticleLink[0].Render();
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

//----------------------------------------------------------------------------------------------------------------------
int	CEFJanadEff::CheckSwordFlashOfDevil()
{
	if(m_pSrcChr->m_ani_frame == 66)
	{
		D3DXMATRIX   matTemp;
		D3DXVECTOR3  vPos;

		m_pSrcChr->GetDirectBoneLinkTM(23 , matTemp);	
		vPos.x = matTemp._41;
		vPos.y = matTemp._42;
		vPos.z = matTemp._43;
		vPos.y = g_Map.PHF_GetHeight(vPos.x , vPos.z);

		m_vCenterPos = vPos;

		g_Particle.m_fAddSize = 1.0f;
		g_Particle.SetEffSetCon(0.0f , EF_JANAD_SWORD_FLASH_OF_DEVILED1171 , 
								 ESLT_POS , 5.1f , -1 , -1 , vPos, 0.0f);
		
		g_Particle.m_fAddSize = 1.0f;
		g_Particle.SetEffSetCon(0.0f , EF_JANAD_SWORD_FLASH_OF_DEVILED1127 , 
								 ESLT_POS , 5.1f , -1 , -1 , vPos, 0.0f);

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
					
					Info.AfterEffect = EF_JANAD_SWORD_FLASH_OF_DEVILED1126;
					Info.DestType = ESLT_CPOS_ICAMERA;
					Info.lifeTime = 5.1f;
					
					g_Particle.m_EffectManager.EffectTargetHit(m_pSrcChr , pTempDest , &Info);					
					
					g_Particle.SetEffSetCon(0.0f , EF_JANAD_SWORD_FLASH_OF_DEVILED1126 , ESLT_CPOS_ICAMERA , 5.1f , 
						pTempDest->GetUnique() , pTempDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr));
				}

				Info.Clear();
			}
		}

		m_InitTime = g_nowTime;
		m_ISShot = TRUE;
	}

	return TRUE;
}

//----------------------------------------------------------------------------------------------------------------------
int	CEFJanadEff::UpdateSwordFlashOfDevil(float dtime)
{
	if(!m_ISShot)
		return TRUE;
	return TRUE;
}

//----------------------------------------------------------------------------------------------------------------------
int	CEFJanadEff::RednerSwordFlashOfDevil(float dtime)
{
	if(!m_ISShot)
		return TRUE;

	if(SAFE_TIME_COMPARE(SAFE_TIME_SUB(g_nowTime , m_InitTime) , > , m_RunningTime))
	{
		g_Particle.SetEffSetCon(0.0f , EF_BARAGOS_DRAGON_BRESS1134 , ESLT_POS , 
			4.1f , -1 , -1 , m_vCenterPos, 0.0f);
		
		for(int i = 0; i < m_iSwordFlashOfDevilCount; ++i)
		{
			float fR = Random(-m_fSwordFlashOfDevilRadius , m_fSwordFlashOfDevilRadius);
			D3DXVECTOR3  vPos = D3DXVECTOR3(fR , 0.0f , 0.0f);
			D3DXMATRIX   matRot;
			D3DXMatrixRotationY(&matRot , D3DXToRadian(Random(0 , 360)));
			///-- 반경Radius 이내로 360 회전된 위치를 구한다.	
			D3DXVec3TransformCoord(&vPos , &vPos , &matRot);
			vPos += D3DXVECTOR3(m_vCenterPos.x , 0.0f , m_vCenterPos.z);
			vPos.y = g_Map.PHF_GetHeight(vPos.x , vPos.z);

			D3DXVECTOR3  vTempPos = vPos;
			
			vPos.y -= 40.0f;

			g_Particle.m_fAddSize = -0.5f;
			g_Particle.SetEffSetCon(0.0f , EF_JANAD_SWORD_FLASH_OF_DEVIL , ESLT_POS , 5.1f , -1 , -1 , vPos, 0.0f);
			g_Particle.SetEffSetCon(0.0f , EF_JANAD_MAGMA_BUSTED , ESLT_POS , 5.1f , -1 , -1 , vTempPos, 0.0f);
			g_Particle.SetEffSetCon(0.0f , EF_BARAGOS_DRAGON_BRESS1133 , ESLT_POS , 5.1f , -1 , -1 , vTempPos, 0.0f);			
		}
		
		SetDestroy();
	}

	return TRUE;
}








