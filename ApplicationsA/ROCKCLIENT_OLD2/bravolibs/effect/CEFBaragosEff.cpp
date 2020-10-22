#include "CEFBaragosEff.h"
#include "..\\Effect.h"

extern long		 g_nowTime;

//----------------------------------------------------------------------------------------------------------------------
CEFBaragosEff::CEFBaragosEff()
{
	m_InitTime = 0;
	m_RangeTime = 0;
	m_PrevTime = 0;
	m_GapTime = 0;

	m_ParticleTime = 0; 

	m_fRadius = 200.0f;

	m_ISShot = FALSE;
	m_ISDamageProcess = FALSE;

	m_nBressState = CEFBaragosEff::BRESS_STATE_NONE;
	m_nThunderState = CEFBaragosEff::THUNDER_STATE_NONE;

	m_vPreStartPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vMoveDir = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vTempPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		
	memset(m_aiBerserkFireBone, 0, sizeof(m_aiBerserkFireBone));

#ifdef HHM_SEALED_TEMPLE_BOSSMOB_EFFECT_LIMIT_20091207
	
	m_bDragonBressMouseFire = false;
	m_iDragonBress1132Count = 10;

	m_bHowlOfDevilBlur = false;

	m_iThunderBreak1129Count = 5;

	m_fBerserkFireEffSize = 2.0f;

	m_aiBerserkFireBone[0] = 23;// 좌앞발
	m_aiBerserkFireBone[1] = 28;// 우앞발
	m_aiBerserkFireBone[2] = 36;// 좌뒷발
	m_aiBerserkFireBone[3] = 40;// 우뒷발


#else
	
	m_bDragonBressMouseFire = true;
	m_iDragonBress1132Count = 20;

	m_bHowlOfDevilBlur = true;

	m_iThunderBreak1129Count = 35;

	m_fBerserkFireEffSize = 4.0f;
	
	// 목
	m_aiBerserkFireBone[0] = 6;
	m_aiBerserkFireBone[1] = 7;
	m_aiBerserkFireBone[2] = 8;
	m_aiBerserkFireBone[3] = 9;

	m_aiBerserkFireBone[4] = 23;// 좌앞발
	m_aiBerserkFireBone[5] = 28;// 우앞발
	m_aiBerserkFireBone[6] = 36;// 좌뒷발
	m_aiBerserkFireBone[7] = 40;// 우뒷발
	
	// 꼬리(몸통에 가까운 순서)
	m_aiBerserkFireBone[8] = 42;
	m_aiBerserkFireBone[9] = 43;
	m_aiBerserkFireBone[10] = 44;
	m_aiBerserkFireBone[11] = 45;

#endif// #ifdef HHM_SEALED_TEMPLE_BOSSMOB_EFFECT_LIMIT_20091207
}

//----------------------------------------------------------------------------------------------------------------------
CEFBaragosEff::~CEFBaragosEff()
{
	m_BressParPosInfoList.clear();
	g_Particle.m_IsRenderScreenBlur = FALSE;

#ifndef HHM_SEALED_TEMPLE_BOSSMOB_EFFECT_LIMIT_20091207

	g_Particle.DelEffSetCon(EF_BARAGOS_DRAGON_BRESS1132 , ESLT_CHARPOS , 
							 m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex());

#endif
}

//----------------------------------------------------------------------------------------------------------------------
void CEFBaragosEff::Init(void)
{
	if(!g_Particle.m_EffectManager.CheckCharacter(m_pSrcChr))
	{
		SetDestroy(TRUE);	
		return;
	}	
	m_ParticleTime = g_nowTime;
	
	m_Src_Ani_Index = m_pSrcChr->m_ani_index;
	//................................................................................	
	// 스킬 발동 사운드
	//................................................................................	
	if(m_pExtraInfo != NULL)
		g_Particle.m_EffectManager.GetSkillImfactSound(m_pExtraInfo->SkillCode , m_pSrcChr);

	switch(m_ID)
	{
		case EF_BARAGOS_HOWL_OF_DEVIL:
			InitHowlOfDevil();
			break;

		case EF_BARAGOS_THUNDERBREAK:
			InitThunderBreak();
			break;

		case EF_BARAGOS_BERSERK:
			InitBerSerk();
			break;
	}
}

//----------------------------------------------------------------------------------------------------------------------
int CEFBaragosEff::CheckFrame(void)
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

	if((m_ID != EF_BARAGOS_BERSERK) && (m_Src_Ani_Index != m_pSrcChr->m_ani_index))
		SetDestroy(TRUE);
	
	switch(m_ID)
	{
		case EF_BARAGOS_DRAGON_BRESS:
			return CheckDragonBress();

		case EF_BARAGOS_HOWL_OF_DEVIL:
			return CheckHowlOfDevil();

		case EF_BARAGOS_THUNDERBREAK:
			return CheckThunderBreak();

		case EF_BARAGOS_BERSERK:
			return CheckBerserk();
	}	
	
	return TRUE;
}

//----------------------------------------------------------------------------------------------------------------------
int CEFBaragosEff::Update(float dtime)
{
	switch(m_ID)
	{
		case EF_BARAGOS_DRAGON_BRESS:
			return UpdateDragonBress(dtime);

		case EF_BARAGOS_HOWL_OF_DEVIL:
			return UpdateHowlOfDevil(dtime);

		case EF_BARAGOS_THUNDERBREAK:
			return UpdateThunderBreak(dtime);
	}	
	
	return TRUE;
}

//----------------------------------------------------------------------------------------------------------------------
int CEFBaragosEff::Render(float dtime)
{
	switch(m_ID)
	{
		case EF_BARAGOS_DRAGON_BRESS:
			return RenderDragonBress(dtime);

		case EF_BARAGOS_HOWL_OF_DEVIL:
			return RenderHowlOfDevil(dtime);

		case EF_BARAGOS_BERSERK:
			return RenderBerserk(dtime);
	}	
	
	return TRUE;
}

//----------------------------------------------------------------------------------------------------------------------
int CEFBaragosEff::CheckDragonBress()
{
	if(m_pSrcChr->m_ani_frame == 33)
		m_nBressState = CEFBaragosEff::BRESS_STATE_SHOT;		

	BOOL IsCheck = FALSE;
	if(m_pSrcChr->m_ani_frame == 76)
		IsCheck = TRUE;		
	
	if(IsCheck && !m_ISShot)
	{
		m_ISShot = TRUE;
		m_ISDamageProcess = TRUE;
	}
	
	if(m_pSrcChr->m_ani_frame == 122)
		m_nBressState = CEFBaragosEff::BRESS_STATE_CLOSE;		

	return TRUE;
}

//----------------------------------------------------------------------------------------------------------------------
int CEFBaragosEff::UpdateDragonBress(float dtime)
{
	if(m_ISDamageProcess)
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
					
					g_Particle.SetEffSetCon(0.0f , EF_BARAGOS_DRAGON_BRESSED , ESLT_CPOS_ICAMERA , 
											 5.1f , pTempDest->GetUnique() , pTempDest->GetIndex(), 
											 ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr));				
					
					Info.AfterEffect = EF_BARAGOS_DRAGON_BRESSED;
					Info.DestType = ESLT_CPOS_ICAMERA;
					Info.lifeTime = 5.1f;											
					g_Particle.m_EffectManager.EffectTargetHit(m_pSrcChr , pTempDest , &Info);
				}

				Info.Clear();
			}
		}

		///-- 라운드 파이어 땅에 관련된 이펙트를 추가(드래곤 브레스(바닥))
		D3DXVECTOR3 vCenterPos = m_vTempPos;
		vCenterPos.y = g_Map.PHF_GetHeight(vCenterPos.x , vCenterPos.z);		
		g_Particle.SetEffSetCon(0.0f , EF_BARAGOS_DRAGON_BRESS1134 , ESLT_POS , 4.1f , -1 , -1 , vCenterPos, 0.0f);

		// 드래곤 브레스(검은연기), 드래곤 브레스(불-여러곳에 뿌려주세요)
		for(int i = 0; i < m_iDragonBress1132Count; ++i)
		{
			float fR = Random(-m_fRadius , m_fRadius);
			D3DXVECTOR3  vPos = D3DXVECTOR3(fR , 0.0f , 0.0f);
			D3DXMATRIX   matRot;
			D3DXMatrixRotationY(&matRot , D3DXToRadian(Random(0 , 360)));
			///-- 반경Radius 이내로 360 회전된 위치를 구한다.	
			D3DXVec3TransformCoord(&vPos , &vPos , &matRot);
			vPos += D3DXVECTOR3(vCenterPos.x , 0.0f , vCenterPos.z);		
			vPos.y = g_Map.PHF_GetHeight(vPos.x , vPos.z);

			int EffId = Random(0, 1) + (int)EF_BARAGOS_DRAGON_BRESS1132;
			//int EffId = EF_BARAGOS_DRAGON_BRESS1132 + 1;
			g_Particle.SetEffSetCon(0.0f , (EFFECT_ID)EffId , ESLT_POS , 2.1f , -1 , -1 , vPos, 0.0f);
		}

		m_ISDamageProcess = FALSE;
	}
	
	return TRUE;
}

//----------------------------------------------------------------------------------------------------------------------
int CEFBaragosEff::RenderDragonBress(float dtime)
{
	DWORD dCullMode = g_RenderManager.GetCullMode();
	DWORD dZWriteEnable = g_RenderManager.GetZWriteEnable();
	DWORD dZEnable = g_RenderManager.GetZEnable();
	DWORD dAlphaBlendEnable = g_RenderManager.GetAlphaBlendEnable();
	DWORD dSrcBlend = g_RenderManager.GetSrcBlend();
	DWORD dDestBlend = g_RenderManager.GetDestBlend();
	DWORD dFillMode = g_RenderManager.GetFillMode();	
	
	D3DXMATRIX   matTemp;		
	
	// 이펙트들의 위치를 지정
	D3DXVECTOR3 vStartPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vTempPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vEndPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_pSrcChr->GetDirectBoneLinkTM(10 , matTemp);	
	vStartPos.x = matTemp._41;
	vStartPos.y = matTemp._42;
	vStartPos.z = matTemp._43;

	m_vMoveDir = vStartPos - m_vPreStartPos;
	D3DXVec3Normalize(&m_vMoveDir , &m_vMoveDir);

	m_pSrcChr->GetDirectBoneLinkTM(18 , matTemp);	
	vTempPos.x = matTemp._41;
	vTempPos.y = matTemp._42;
	vTempPos.z = matTemp._43;

	vTempPos.y = vStartPos.y - 70.0f;

	D3DXVECTOR3 vDir = vTempPos - vStartPos;
	D3DXVec3Normalize(&vDir , &vDir);
	vEndPos = vTempPos  + 50.0f * vDir;
	vEndPos = vEndPos + 20.0f * m_vMoveDir;	

	m_vPreStartPos = vStartPos;

	D3DXVECTOR3 vCDir = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	D3DXMATRIX matRotY;
	D3DXMatrixRotationY(&matRotY , m_pSrcChr->GetDirection());
	D3DXVec3TransformCoord(&vCDir , &vCDir , &matRotY);	

	m_vTempPos = vStartPos + (100.0f * vCDir);
	
	if(SAFE_TIME_COMPARE(SAFE_TIME_SUB(g_nowTime , m_ParticleTime) , > , 50))
	{
		if(m_nBressState != CEFBaragosEff::BRESS_STATE_NONE)
		{
			BRESS_POS BressPos;
			BressPos.vStartPos = vStartPos;
			BressPos.vEndPos = vEndPos;
			BressPos.dInitTime = g_nowTime;
			
	
			m_BressParPosInfoList.push_back(BressPos);
	
		}
		m_ParticleTime = g_nowTime;
	}

	
	
	vector < list < BRESS_POS >::iterator > templist;	
	if(!m_BressParPosInfoList.empty())
	{
	   list	< BRESS_POS >::iterator	pi = m_BressParPosInfoList.begin();
	   BRESS_POS BressPos;
	   BRESS_POS * pBressPos = NULL;

	   for(; pi != m_BressParPosInfoList.end(); ++pi)
	   {   
		  BressPos = (BRESS_POS)(*pi);
		  pBressPos = (BRESS_POS *)(&(*pi));

		  pBressPos->vEndPos += 10.0f * m_vMoveDir;

		  if(SAFE_TIME_COMPARE(SAFE_TIME_SUB(g_nowTime , BressPos.dInitTime) , > , 1000))
			  templist.push_back(pi);	  
	   }	
	}


	for(int i = 0; i < templist.size(); ++i)
		m_BressParPosInfoList.erase(templist[i]);

	D3DXVECTOR3 vPos;

	if(!m_BressParPosInfoList.empty())
	{
	   int nSize = m_BressParPosInfoList.size();

	   list	< BRESS_POS >::iterator	pi = m_BressParPosInfoList.begin();
	   BRESS_POS BressPos;

	   for(; pi != m_BressParPosInfoList.end(); ++pi)
	   {   
		  BressPos = (BRESS_POS)(*pi);	
		  float fRange =  (float)(SAFE_TIME_SUB(g_nowTime , BressPos.dInitTime) / 1000.0f);
		  if(fRange > 1.0f)
		  	 fRange = 1.0f;

		  int nLevel = (int)(fRange * 15.0f);
		  if(nLevel > 6)
		  {
			  D3DXVec3Lerp(&vPos , &BressPos.vStartPos , &BressPos.vEndPos , fRange);	
			  float fSize = 1.0f + pow((float)nLevel , (float)2) / 50.0f;

			  if(m_bDragonBressMouseFire)
				g_Particle.DisplayUpdate(24706 ,vPos, -1, fSize);
		  }
	   }	
	
	   for(int i = 0; i < 6; ++i)
	   {
		  float fRange =  (float)(i / 15.0f);
		  D3DXVec3Lerp(&vPos , &vStartPos , &vEndPos , fRange);	
		  float fLevel = (float)(fRange * 15.0f);
		  float fSize = 0.15f + fLevel/3.0f;
	   	  g_Particle.DisplayUpdate(24706 ,vPos, -1, fSize);	
	   }
	}

	g_RenderManager.SetCullMode(dCullMode);	
	g_RenderManager.SetZWriteEnable(dZWriteEnable);
	g_RenderManager.SetZEnable(dZEnable);	
	g_RenderManager.SetAlphaBlendEnable(dAlphaBlendEnable);
	g_RenderManager.SetSrcBlend(dSrcBlend);
	g_RenderManager.SetDestBlend( dDestBlend);	
	g_RenderManager.SetFillMode(dFillMode);
	
	if(m_nBressState == CEFBaragosEff::BRESS_STATE_CLOSE)
		SetDestroy();
	
	return TRUE;
}

//----------------------------------------------------------------------------------------------------------------------
void CEFBaragosEff::InitHowlOfDevil()
{
	CParticleData *  pData = NULL;
	
	if((pData = g_Particle.m_ParticleData.GetKeyDataPointer(24760)) == NULL)// 악룡의 포효(포효하는입에 붙음)
	{
		SetDestroy();
		return;
	}

	m_AddParticleLink[0].MakeLink(pData);
	m_AddParticleLink[0].m_fSize = 2.5f;

	if((pData = g_Particle.m_ParticleData.GetKeyDataPointer(347)) == NULL)
	{
		SetDestroy();
		return;
	}

	m_AddParticleLink[1].MakeLink(pData);
	m_AddParticleLink[1].m_fSize = 2.5f;
	
}

//----------------------------------------------------------------------------------------------------------------------
int CEFBaragosEff::CheckHowlOfDevil()
{
	BOOL IsCheck = FALSE;

	if(m_pSrcChr->m_ani_frame == 49)
	{
		D3DXMATRIX matBone;			
		m_pSrcChr->GetBoneLinkTM(4 , matBone , FALSE);
		D3DXVECTOR3	Pos1(matBone._41, 0.0f, matBone._43);	
		Pos1.y = g_Map.PHF_GetHeight(Pos1.x , Pos1.z);
		g_Particle.SetEffSetCon(0.0f , EF_BARAGOS_HOWL_OF_DEVIL1131 , // 악룡의 포효(한쪽발 먼지)
								ESLT_POS , 1.5f , -1 , -1 , Pos1, 0.0f);		
	}
	
	if(m_pSrcChr->m_ani_frame == 65)
	{
		IsCheck = TRUE;	
		g_Particle.m_IsRenderScreenBlur = m_bHowlOfDevilBlur;		
	
	}	
	
	if(IsCheck && !m_ISShot)
	{
		m_ISShot = TRUE;
		m_ISDamageProcess = TRUE;
	}
	
	if(m_pSrcChr->m_ani_frame == 165)
	{
		g_Particle.m_IsRenderScreenBlur = FALSE;
		SetDestroy();
	}
	
	return TRUE;
}

//----------------------------------------------------------------------------------------------------------------------
int	CEFBaragosEff::UpdateHowlOfDevil(float dtime)
{
	if(m_ISDamageProcess)
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
					
					g_Particle.SetEffSetCon(0.0f , EF_BARAGOS_HOWL_OF_DEVILED , ESLT_CPOS_ICAMERA , 
											 5.1f , pTempDest->GetUnique() , pTempDest->GetIndex(), 
											 ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr));				
					
					Info.AfterEffect = EF_BARAGOS_DRAGON_BRESSED;
					Info.DestType = ESLT_CPOS_ICAMERA;
					Info.lifeTime = 5.1f;											
					g_Particle.m_EffectManager.EffectTargetHit(m_pSrcChr , pTempDest , &Info);
				}

				Info.Clear();
			}
		}

		m_ISDamageProcess = FALSE;
	}	
	
	return TRUE;
}

//----------------------------------------------------------------------------------------------------------------------
int	CEFBaragosEff::RenderHowlOfDevil(float dtime)
{
	
	DWORD dCullMode = g_RenderManager.GetCullMode();
	DWORD dZWriteEnable = g_RenderManager.GetZWriteEnable();
	DWORD dZEnable = g_RenderManager.GetZEnable();
	DWORD dAlphaBlendEnable = g_RenderManager.GetAlphaBlendEnable();
	DWORD dSrcBlend = g_RenderManager.GetSrcBlend();
	DWORD dDestBlend = g_RenderManager.GetDestBlend();
	DWORD dFillMode = g_RenderManager.GetFillMode();	
	
	if(!m_ISShot)
	{
		D3DXMATRIX   matTemp;
		D3DXVECTOR3  vPos1, vPos2, vPos;

		m_pSrcChr->GetDirectBoneLinkTM(17 , matTemp);	
		vPos1.x = matTemp._41;
		vPos1.y = matTemp._42;
		vPos1.z = matTemp._43;

		m_pSrcChr->GetDirectBoneLinkTM(12 , matTemp);	
		vPos2.x = matTemp._41;
		vPos2.y = matTemp._42;
		vPos2.z = matTemp._43;

		vPos = (vPos1 + vPos2) / 2.0f;
	
		m_AddParticleLink[0].SetLinkPos(vPos);
		m_AddParticleLink[0].UpdateLink(dtime);
		m_AddParticleLink[0].Render();		

		m_AddParticleLink[1].SetLinkPos(vPos);
		m_AddParticleLink[1].UpdateLink(dtime);
		m_AddParticleLink[1].Render();	
		
		g_RenderManager.SetCullMode(dCullMode);	
		g_RenderManager.SetZWriteEnable(dZWriteEnable);
		g_RenderManager.SetZEnable(dZEnable);	
		g_RenderManager.SetAlphaBlendEnable(dAlphaBlendEnable);
		g_RenderManager.SetSrcBlend(dSrcBlend);
		g_RenderManager.SetDestBlend( dDestBlend);	
		g_RenderManager.SetFillMode(dFillMode);
		
		return TRUE;
	}
	
	return TRUE;
}

// 선더 브레이크(기모음)------------------------------------------------------------------------------------------------
void CEFBaragosEff::InitThunderBreak()
{
	g_Particle.m_aDirectLinkBone = 18;
	g_Particle.SetEffSetCon(0.0f , EF_BARAGOS_THUNDERBREAK, ESLT_CHARPOS , 
						 EFFECT_SET_LOOP_TIME , m_pSrcChr->GetUnique() , 
						 m_pSrcChr->GetIndex());	
}

//----------------------------------------------------------------------------------------------------------------------
int	CEFBaragosEff::CheckThunderBreak()
{
	BOOL IsCheck = FALSE;	
	
	if(m_pSrcChr->m_ani_frame == 111)
	{
		IsCheck = TRUE;			
		g_Particle.DelEffSetCon(EF_BARAGOS_THUNDERBREAK , ESLT_CHARPOS , 
								 m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex());	
	}
	
	if(IsCheck)
	{
		m_ISShot = TRUE;
		m_PrevTime = g_nowTime;
		///-- 뿔 부분에서 큰 번개 하나 떨어짐
		D3DXMATRIX   matTemp;
		D3DXVECTOR3 vPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	

		m_pSrcChr->GetDirectBoneLinkTM(18 , matTemp);	
		vPos.x = matTemp._41;
		vPos.y = matTemp._42;
		vPos.z = matTemp._43;
	
		D3DXVECTOR3 vDir = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		D3DXMATRIX matRotY;
		D3DXMatrixRotationY(&matRotY , m_pSrcChr->GetDirection());
		D3DXVec3TransformCoord(&vDir , &vDir , &matRotY);	

		vPos -= (150.0f * vDir);

		vPos.y = g_Map.PHF_GetHeight(vPos.x , vPos.z);
		g_Particle.m_fAddSize = 5.0f;// 5
		g_Particle.SetEffSetCon(0.0f , EF_BARAGOS_THUNDERBREAK1128 , ESLT_POS , 5.1f , -1 , -1 , 
					vPos, 0.0f, -1, -10);
		g_Particle.m_fAddSize = 1.0f;
		g_Particle.SetEffSetCon(0.0f , EF_BARAGOS_THUNDERBREAK1128 , ESLT_POS , 5.1f , -1 , -1 , 
					vPos, 0.0f, -1, 0);		

		m_vPreStartPos = vPos;
	}	
	
	return TRUE;
}

//----------------------------------------------------------------------------------------------------------------------
int	CEFBaragosEff::UpdateThunderBreak(float dtime)
{
	if(m_ISShot)
	{
		if(SAFE_TIME_COMPARE(SAFE_TIME_SUB(g_nowTime , m_PrevTime) , > , 1000))
		{
			m_ISDamageProcess = TRUE;
		}	
	}	
	
	if(m_ISDamageProcess)
	{
		///-- 데미지 처리
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
					
					
					g_Particle.SetEffSetCon(0.0f , EF_BARAGOS_THUNDERBREAKED , ESLT_CPOS_ICAMERA , 
											 5.1f , pTempDest->GetUnique() , pTempDest->GetIndex(), 
											 ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr));				
					
					Info.AfterEffect = EF_BARAGOS_THUNDERBREAKED;
					Info.DestType = ESLT_CPOS_ICAMERA;
					Info.lifeTime = 5.1f;											
					g_Particle.m_EffectManager.EffectTargetHit(m_pSrcChr , pTempDest , &Info);
				}

				Info.Clear();
			}
		}		
		
		///-- 랜덤 반경으로 번개 이펙트
		for(int i = 0; i < m_iThunderBreak1129Count; ++i)
		{
			float fR = Random(0.0f , m_fRadius);
			
			D3DXVECTOR3  vPos = D3DXVECTOR3(fR , 0.0f , 0.0f);
			D3DXMATRIX   matRot;
			D3DXMatrixRotationY(&matRot , D3DXToRadian(Random(0 , 360)));
			///-- 반경Radius 이내로 360 회전된 위치를 구한다.	
			D3DXVec3TransformCoord(&vPos , &vPos , &matRot);
			vPos += D3DXVECTOR3(m_vPreStartPos.x , 0.0f , m_vPreStartPos.z);	

			vPos.y = g_Map.PHF_GetHeight(vPos.x , vPos.z);

			int nRndHeight = Random(-30, -10);
			float fRndSize = Random(0.0f, 2.0f);

			g_Particle.m_fAddSize = fRndSize;
			g_Particle.SetEffSetCon(0.0f , EF_BARAGOS_THUNDERBREAK1129 , ESLT_POS , 5.1f , -1 , -1 , 
				vPos, 0.0f, -1, nRndHeight);
		}
		
		m_ISDamageProcess = FALSE;

		SetDestroy();
	}	
	
	return TRUE;
}

// 광폭화 초기화--------------------------------------------------------------------------------------------------------
void CEFBaragosEff::InitBerSerk()
{
	m_IsConEffect = FALSE;
	CParticleData *  pData = NULL;

	if((pData = g_Particle.m_ParticleData.GetKeyDataPointer(24378)) == NULL)
	{
		SetDestroy();
		return;
	}

	for(int i = 0; i < ME_BERSERK_COUNT; ++i)
	{
		m_ParticleLink[i].MakeLink(pData);
		m_ParticleLink[i].m_fSize = m_fBerserkFireEffSize;
	}	
	
	if((pData = g_Particle.m_ParticleData.GetKeyDataPointer(24755)) == NULL)// 광폭화(포효하는 입에 붙음)
	{
		SetDestroy();
		return;
	}

	m_AddParticleLink[0].MakeLink(pData);
	m_AddParticleLink[0].m_fSize = 2.5f;

	if((pData = g_Particle.m_ParticleData.GetKeyDataPointer(24756)) == NULL)// 광폭화(포효하는 입에 붙음)
	{
		SetDestroy();
		return;
	}

	m_AddParticleLink[1].MakeLink(pData);
	m_AddParticleLink[1].m_fSize = 2.5f;
}

// 광폭화 이펙트 프레임 체크--------------------------------------------------------------------------------------------
int	CEFBaragosEff::CheckBerserk()
{
#ifdef HHM_SEALED_TEMPLE_BOSSMOB_EFFECT_LIMIT_20091207
	
	m_ISShot = TRUE;
	return TRUE;

#endif// #ifdef HHM_SEALED_TEMPLE_BOSSMOB_EFFECT_LIMIT_20091207

	if(m_ISShot)
		return TRUE;
	
	if(m_pSrcChr->m_ani_frame == 66)
	{
		g_Particle.m_IsRenderScreenBlur = TRUE;
		g_Particle.RockCamera((rand() % 200) + 300);

		g_Particle.m_aDirectLinkBone = 28;
		g_Particle.SetEffSetCon(0.0f , EF_BARAGOS_DRAGON_BRESS1132, ESLT_CHARPOS , 
							 EFFECT_SET_LOOP_TIME , m_pSrcChr->GetUnique() , 
							 m_pSrcChr->GetIndex());
		g_Particle.m_aDirectLinkBone = 23;
		g_Particle.SetEffSetCon(0.0f , EF_BARAGOS_DRAGON_BRESS1132, ESLT_CHARPOS , 
							 EFFECT_SET_LOOP_TIME , m_pSrcChr->GetUnique() , 
							 m_pSrcChr->GetIndex());
	}

	if(m_pSrcChr->m_ani_frame == 76)
	{
		g_Particle.DelEffSetCon(EF_BARAGOS_DRAGON_BRESS1132 , ESLT_CHARPOS , 
								 m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex());
	}

	if(m_pSrcChr->m_ani_frame == 157)
	{
		g_Particle.m_IsRenderScreenBlur = FALSE;
		m_ISShot = TRUE;
	}

	return TRUE;
}

// 광폭화 그리기--------------------------------------------------------------------------------------------------------
int	CEFBaragosEff::RenderBerserk(float dtime)
{
	DWORD dCullMode = g_RenderManager.GetCullMode();
	DWORD dZWriteEnable = g_RenderManager.GetZWriteEnable();
	DWORD dZEnable = g_RenderManager.GetZEnable();
	DWORD dAlphaBlendEnable = g_RenderManager.GetAlphaBlendEnable();
	DWORD dSrcBlend = g_RenderManager.GetSrcBlend();
	DWORD dDestBlend = g_RenderManager.GetDestBlend();
	DWORD dFillMode = g_RenderManager.GetFillMode();	
	
	if(!m_ISShot)
	{
		D3DXMATRIX   matTemp;
		D3DXVECTOR3  vPos1, vPos2, vPos;

		m_pSrcChr->GetDirectBoneLinkTM(17 , matTemp);	
		vPos1.x = matTemp._41;
		vPos1.y = matTemp._42;
		vPos1.z = matTemp._43;

		m_pSrcChr->GetDirectBoneLinkTM(12 , matTemp);	
		vPos2.x = matTemp._41;
		vPos2.y = matTemp._42;
		vPos2.z = matTemp._43;

		vPos = (vPos1 + vPos2) / 2.0f;
	
		m_AddParticleLink[0].SetLinkPos(vPos);
		m_AddParticleLink[0].UpdateLink(dtime);
		m_AddParticleLink[0].Render();		

		m_AddParticleLink[1].SetLinkPos(vPos);
		m_AddParticleLink[1].UpdateLink(dtime);
		m_AddParticleLink[1].Render();	
		
		g_RenderManager.SetCullMode(dCullMode);	
		g_RenderManager.SetZWriteEnable(dZWriteEnable);
		g_RenderManager.SetZEnable(dZEnable);	
		g_RenderManager.SetAlphaBlendEnable(dAlphaBlendEnable);
		g_RenderManager.SetSrcBlend(dSrcBlend);
		g_RenderManager.SetDestBlend( dDestBlend);	
		g_RenderManager.SetFillMode(dFillMode);
		
		return TRUE;
	}

	D3DXMATRIX   matTemp;
	D3DXVECTOR3  vPos;

	for(int i = 0; i < ME_BERSERK_COUNT; ++i)
	{
		if(!m_aiBerserkFireBone[i])
			continue;

		m_pSrcChr->GetDirectBoneLinkTM(m_aiBerserkFireBone[i] , matTemp);	
		vPos.x = matTemp._41;
		vPos.y = matTemp._42;
		vPos.z = matTemp._43;
	
		m_ParticleLink[i].SetLinkPos(vPos);
		m_ParticleLink[i].UpdateLink(dtime);
		m_ParticleLink[i].Render();
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
