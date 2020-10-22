#include "CEFHideSensor.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include	<SOUND\Sound.H>
#include "..\\..\\\rocklibs\RockInterface\PartyInforWnd.h"


#define HIDE_ALPHA_VALUE	0.3f


CEFHideSensor::CEFHideSensor()
{
}

CEFHideSensor::~CEFHideSensor()
{
	//스킬 종료 
	//스킬이 끝나면 원래 상태로 되돌림 
	std::list	<CEffectBase *> Effectlist = g_Particle.m_EffectManager.GetEffectList();
	std::list	<CEffectBase *> ::iterator	i = Effectlist.begin();

	CEffectBase *pData = NULL;
	
	if( g_RockClient.m_SceneManager.GetEffectDisplayMode() == SCENE_NORMAL )
	{
		for( ; i != Effectlist.end() ; ++i )
		{					
			if( NULL != ( pData = ( CEffectBase * )( *i ) ) ) 
			{
				//누군가가 은신을 쓰거나 월영을 썻을때 - GM 은신은 제외한다.
				if( (*i)->GetSrcChar() != NULL && 
					( (*i)->GetID() == EF_HIDE || (*i)->GetID() == EF_MOMENT_HIDE ) &&
					(*i)->GetSkillCode() != 8003 )
				{
					///-- 한번이라두 감지를 당한 넘이냐?
					if( (*i)->GetSrcChar()->m_CharAlpha.GetShow() )
					{
						if(( (*i)->GetSrcChar()->GetUnique()  != m_pSrcChr->GetUnique() ) )
						{
							//그넘의 정보를 받아온다 . 
							//안보이 는 넘이야 투명 이냐 ? 
							if( DistanceIn(m_pSrcChr->GetPosTM(), (*i)->GetSrcChar()->GetPosTM()) )
							{
								if( (*i)->GetSrcChar()->m_CharAlpha.GetState() == CHAR_HIDE )
								{
									(*i)->GetSrcChar()->m_CharAlpha.Set( CHAR_NONE_DISPLAY , 0.0f);

									BOOL IsOwnerPet = FALSE;

									Character *pSrcChr = (*i)->GetSrcChar();
									
									if( pSrcChr->m_bPC && pSrcChr->m_State.nPetState == nPetState_Master &&
										pSrcChr->m_State.PetIndex != -1 )
									{
										IsOwnerPet = TRUE;
									}
									
									if(IsOwnerPet)
									{
										int nPetIndex = m_pSrcChr->m_State.PetIndex;
										g_Map.m_Npc[nPetIndex].m_CharAlpha.Set( CHAR_NONE_DISPLAY ,0.0f);
									}
					
								}
							}
							else 
							{
								(*i)->GetSrcChar()->m_CharAlpha.Set( CHAR_NONE_DISPLAY , 0.0f );

								BOOL IsOwnerPet = FALSE;

								Character *pSrcChr = (*i)->GetSrcChar();
								
								if( pSrcChr->m_bPC && pSrcChr->m_State.nPetState == nPetState_Master &&
									pSrcChr->m_State.PetIndex != -1 )
								{
									IsOwnerPet = TRUE;
								}
								
								if(IsOwnerPet)
								{
									int nPetIndex = m_pSrcChr->m_State.PetIndex;
									g_Map.m_Npc[nPetIndex].m_CharAlpha.Set( CHAR_NONE_DISPLAY ,0.0f);
								}
							}
						}
					}
				}
			}
		}
	}

	
}
BOOL CEFHideSensor::DistanceIn(D3DXMATRIX &SrcPos ,D3DXMATRIX &DestPos)
{
	D3DXVECTOR3 vLength = D3DXVECTOR3(0.0f,0.0f,0.0f) ;

	vLength.x = SrcPos._41 - DestPos._41;
	vLength.y = 0.0f;
	vLength.y = SrcPos._43 - DestPos._43; 
	
	if(D3DXVec3Length(&vLength) < g_Particle.m_EffectManager.GetAttackRange(8004)  )
	{
		return TRUE;
	}

	return FALSE;
}

void CEFHideSensor::Init(void)
{

}

int CEFHideSensor::Update(float dtime)
{
	if( g_Particle.m_EffectManager.CheckDie( m_pSrcChr ) )
	{
		SetDestroy();

		return TRUE;
	}

	if( g_Particle.m_EffectManager.CheckDie( m_pSrcChr ) )
	{
		SetDestroy();

		return TRUE;
	}

	//숨어 있는 넘을 찾자 ? 
	//그다음 이넘이 내 사정거리에 들어 왔냐
	if( m_pSrcChr->m_bPC )
	{
		//내가 사용했으면 ... ? 
		if( m_pSrcChr->GetUnique() == g_Pc.GetPlayer()->GetUnique() ) 
		{
			MyUseHideSensor();
		}		
	}

	return TRUE;
}
BOOL CEFHideSensor::MyUseHideSensor()
{

	std::list	<CEffectBase *> Effectlist = g_Particle.m_EffectManager.GetEffectList();
	std::list	<CEffectBase *> ::iterator	i = Effectlist.begin();
	CEffectBase *pData;
	
	if( g_RockClient.m_SceneManager.GetEffectDisplayMode() == SCENE_NORMAL )
	{
		for( ; i != Effectlist.end() ; ++i )
		{					
			if( NULL != ( pData = ( CEffectBase * )( *i ) ) ) 
			{
				//누군가가 은신을 쓰거나 월영을 썻을때 - GM 은신은 제외한다.
				//내 파티원이냐 ? 무시하라 . 
				if( (*i)->GetSrcChar() != NULL && 
					( (*i)->GetID() == EF_HIDE || (*i)->GetID() == EF_MOMENT_HIDE ) &&
					(*i)->GetSkillCode() != 8003 )
				{
					//나자신은 제외 다른넘들 검색
					//밀레나의 은신을 생각해야 한다.
					//홀리의 디텍팅을 생각해야 한다. 
					if( (*i)->GetSrcChar()->GetUnique() != g_Pc.GetPlayer()->GetUnique()  )
					{
						//내파티원 이냐 ? 
						if( ::IsMyPartyAndLeagueMember( (*i)->GetSrcChar()->GetUnique() ) )
						{
							(*i)->GetSrcChar()->m_CharAlpha.SetShow(false);
							//만약 안보이면 투명화 시켜라.. 
							if( (*i)->GetSrcChar()->m_CharAlpha.GetState() == CHAR_NONE_DISPLAY )
							{
								//투명화 시켜라 
								(*i)->GetSrcChar()->m_CharAlpha.Set(CHAR_HIDE ,0.3f);							
							}							
						}
						else 	
						{							
							///-- 내 파티원이 아닌경우
							D3DXVECTOR3 vLen;
							
							///-- 거리내에 존재하면
							if( DistanceIn(g_Pc.GetPlayer()->GetPosTM() , (*i)->GetSrcChar()->GetPosTM()) )
							{								
								if( (*i)->GetSrcChar()->m_CharAlpha.GetState() == CHAR_NONE_DISPLAY	)
								{
									//투명화 시키고 
									//비약으로인해 감지 당했다.. 
									(*i)->GetSrcChar()->m_CharAlpha.SetShow(true);
									(*i)->GetSrcChar()->m_CharAlpha.Set(CHAR_HIDE ,0.3f);
								
									BOOL IsOwnerPet = FALSE;

									Character *pSrcChr = (*i)->GetSrcChar();
									
									if( pSrcChr->m_bPC && pSrcChr->m_State.nPetState == nPetState_Master &&
										pSrcChr->m_State.PetIndex != -1 )
									{
										IsOwnerPet = TRUE;
									}
									
									if(IsOwnerPet)
									{
										int nPetIndex = m_pSrcChr->m_State.PetIndex;
										g_Map.m_Npc[nPetIndex].m_CharAlpha.Set(CHAR_HIDE ,0.3f);
									}
								}								
							}
							//범위 밖에 있고 
							else 
							{
								if(	(*i)->GetSrcChar()->m_CharAlpha.GetState() == CHAR_HIDE )
								{								
									(*i)->GetSrcChar()->m_CharAlpha.SetShow(false);
									(*i)->GetSrcChar()->m_CharAlpha.Set(CHAR_NONE_DISPLAY ,0.0f);

									BOOL IsOwnerPet = FALSE;

									Character *pSrcChr = (*i)->GetSrcChar();
									
									if( pSrcChr->m_bPC && pSrcChr->m_State.nPetState == nPetState_Master &&
										pSrcChr->m_State.PetIndex != -1 )
									{
										IsOwnerPet = TRUE;
									}
									
									if(IsOwnerPet)
									{
										int nPetIndex = m_pSrcChr->m_State.PetIndex;
										g_Map.m_Npc[nPetIndex].m_CharAlpha.Set(CHAR_NONE_DISPLAY ,0.0f);
									}
								}								
							}
						}
					}
				}
			}
		}
	}

	return TRUE;
}


BOOL CEFHideSensor::MyPartyHideSensor()
{
		
	std::list	<CEffectBase *> Effectlist = g_Particle.m_EffectManager.GetEffectList();
	std::list	<CEffectBase *> ::iterator	i = Effectlist.begin();
	CEffectBase * pData = NULL;	
	
	if( g_RockClient.m_SceneManager.GetEffectDisplayMode() == SCENE_NORMAL )
	{
		for( ; i != Effectlist.end() ; ++i )
		{					
			if( NULL != ( pData = ( CEffectBase * )( *i ) ) ) 
			{
				//누군가가 은신을 쓰거나 월영을 썻을때 - GM 은신은 제외한다.
				if( (*i)->GetSrcChar() != NULL && 
					( (*i)->GetID() == EF_HIDE || (*i)->GetID() == EF_MOMENT_HIDE ) &&
					(*i)->GetSkillCode() != 8003 )
				{
					//나자신은 제외 다른넘들 검색 
					if( ( (*i)->GetSrcChar()->GetUnique()  != m_pSrcChr->GetUnique() ) &&
						( (*i)->GetSrcChar()->GetUnique()  != g_Pc.GetPlayer()->GetUnique() ) )
					{
						//그넘의 정보를 받아온다. 
						//안보이는 넘이야 투명이냐?						
						if( DistanceIn(m_pSrcChr->GetPosTM() , (*i)->GetSrcChar()->GetPosTM()) )
						{
							if( (*i)->GetSrcChar()->m_CharAlpha.GetState() == CHAR_NONE_DISPLAY	)
							{
								//투명화 시키고 
								(*i)->GetSrcChar()->m_CharAlpha.SetShow(true);								
							}							
						}
						//범위 밖에 있고 
						else 
						{
							if(	(*i)->GetSrcChar()->m_CharAlpha.GetState() == CHAR_HIDE)
							{								
								(*i)->GetSrcChar()->m_CharAlpha.SetShow(false);								
							}							
						}
					}					
				}
			}
		}
	}

	return TRUE;
}