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
	//��ų ���� 
	//��ų�� ������ ���� ���·� �ǵ��� 
	std::list	<CEffectBase *> Effectlist = g_Particle.m_EffectManager.GetEffectList();
	std::list	<CEffectBase *> ::iterator	i = Effectlist.begin();

	CEffectBase *pData = NULL;
	
	if( g_RockClient.m_SceneManager.GetEffectDisplayMode() == SCENE_NORMAL )
	{
		for( ; i != Effectlist.end() ; ++i )
		{					
			if( NULL != ( pData = ( CEffectBase * )( *i ) ) ) 
			{
				//�������� ������ ���ų� ������ ������ - GM ������ �����Ѵ�.
				if( (*i)->GetSrcChar() != NULL && 
					( (*i)->GetID() == EF_HIDE || (*i)->GetID() == EF_MOMENT_HIDE ) &&
					(*i)->GetSkillCode() != 8003 )
				{
					///-- �ѹ��̶�� ������ ���� ���̳�?
					if( (*i)->GetSrcChar()->m_CharAlpha.GetShow() )
					{
						if(( (*i)->GetSrcChar()->GetUnique()  != m_pSrcChr->GetUnique() ) )
						{
							//�׳��� ������ �޾ƿ´� . 
							//�Ⱥ��� �� ���̾� ���� �̳� ? 
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

	//���� �ִ� ���� ã�� ? 
	//�״��� �̳��� �� �����Ÿ��� ��� �Գ�
	if( m_pSrcChr->m_bPC )
	{
		//���� ��������� ... ? 
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
				//�������� ������ ���ų� ������ ������ - GM ������ �����Ѵ�.
				//�� ��Ƽ���̳� ? �����϶� . 
				if( (*i)->GetSrcChar() != NULL && 
					( (*i)->GetID() == EF_HIDE || (*i)->GetID() == EF_MOMENT_HIDE ) &&
					(*i)->GetSkillCode() != 8003 )
				{
					//���ڽ��� ���� �ٸ��ѵ� �˻�
					//�з����� ������ �����ؾ� �Ѵ�.
					//Ȧ���� �������� �����ؾ� �Ѵ�. 
					if( (*i)->GetSrcChar()->GetUnique() != g_Pc.GetPlayer()->GetUnique()  )
					{
						//����Ƽ�� �̳� ? 
						if( ::IsMyPartyAndLeagueMember( (*i)->GetSrcChar()->GetUnique() ) )
						{
							(*i)->GetSrcChar()->m_CharAlpha.SetShow(false);
							//���� �Ⱥ��̸� ����ȭ ���Ѷ�.. 
							if( (*i)->GetSrcChar()->m_CharAlpha.GetState() == CHAR_NONE_DISPLAY )
							{
								//����ȭ ���Ѷ� 
								(*i)->GetSrcChar()->m_CharAlpha.Set(CHAR_HIDE ,0.3f);							
							}							
						}
						else 	
						{							
							///-- �� ��Ƽ���� �ƴѰ��
							D3DXVECTOR3 vLen;
							
							///-- �Ÿ����� �����ϸ�
							if( DistanceIn(g_Pc.GetPlayer()->GetPosTM() , (*i)->GetSrcChar()->GetPosTM()) )
							{								
								if( (*i)->GetSrcChar()->m_CharAlpha.GetState() == CHAR_NONE_DISPLAY	)
								{
									//����ȭ ��Ű�� 
									//����������� ���� ���ߴ�.. 
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
							//���� �ۿ� �ְ� 
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
				//�������� ������ ���ų� ������ ������ - GM ������ �����Ѵ�.
				if( (*i)->GetSrcChar() != NULL && 
					( (*i)->GetID() == EF_HIDE || (*i)->GetID() == EF_MOMENT_HIDE ) &&
					(*i)->GetSkillCode() != 8003 )
				{
					//���ڽ��� ���� �ٸ��ѵ� �˻� 
					if( ( (*i)->GetSrcChar()->GetUnique()  != m_pSrcChr->GetUnique() ) &&
						( (*i)->GetSrcChar()->GetUnique()  != g_Pc.GetPlayer()->GetUnique() ) )
					{
						//�׳��� ������ �޾ƿ´�. 
						//�Ⱥ��̴� ���̾� �����̳�?						
						if( DistanceIn(m_pSrcChr->GetPosTM() , (*i)->GetSrcChar()->GetPosTM()) )
						{
							if( (*i)->GetSrcChar()->m_CharAlpha.GetState() == CHAR_NONE_DISPLAY	)
							{
								//����ȭ ��Ű�� 
								(*i)->GetSrcChar()->m_CharAlpha.SetShow(true);								
							}							
						}
						//���� �ۿ� �ְ� 
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