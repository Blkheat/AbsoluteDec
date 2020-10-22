#include "CEffectManager.h"
#include "..\\Effect.h"

#include "..\\RockClient.h"
#include <NETWORK\\Protocol.h>
#include "..\\quadlist.h"
#include "..\\Pc.h"
#include "..\\Npc.h"
#include <SOUND\Sound.H>
#include "..\\rocklibs\RockInterface\NonPlayerInfoWnd.h"
#include "..\\rocklibs\RockInterface\PlayerInfoWnd.h"
#include "..\\rocklibs\RockInterface\CDelayBarWnd.h"
#include "stringmanager.h"
extern long					g_nowTime;
extern Sound_Manager		g_Sound;
#include "..\\..\\CRenderManager.h"
#include "..\\obj\\EffItemManager.h"

//................................................................................................................
// ����Ʈ�� ��� ����
//................................................................................................................
#include "CEFLightning.h"										// ����Ʈ��
#include "CEFFireBall.h"										// ȭ�̾
#include "CEFStickAttack.h"										// ������ �⺻ ����
#include "CEFBookAttack.h"										// å �⺻ ����
#include "CEFRockPress.h"										// �� ������
#include "CEFWoodenSword.h"										// ��� �ҵ�
#include "CEFRockShield.h"										// �� �ǵ� ( ���� ��ų )
#include "CEFRefreshSword.h"									// ��� ( ���� ��ų )
#include "CEFStrongSword.h"										// ���� ( ���� ��ų )
#include "CEFGatherCut.h"										// ��� ����
#include "CEFIceSprie.h"										// ���̽� ���Ǿ�
#include "CEFBubbleCrash.h"										// ���� ũ����
#include "CEFRecoveryItem2.h"									// ȸ�� ������2 ( ���Ӽ� )
#include "CEFAvoid.h"											// ȸ��
#include "CEFBookCriticalAttack.h"								// å �⺻ ���� ( ũ��Ƽ�� )
#include "CEFBowAttack.h"										// Ȱ ���� �⺻
#include "CEFArrow.h"											// Ȱ
#include "CArrowStuck.h"										// Ȱ ���� �ٴ°�
#include "CEFGlareBow.h"										// ��� ���
#include "CEFFootBow.h"											// �ߵ� ���
#include "CEFFootBowLink.h"										// �ߵ� ��� ȭ�� ��ũ �Ȱ�
#include "CEFPossessed.h"										// ����
#include "CEFPossessedAppear.h"									// ���� ��Ÿ ����
#include "CEFRefreshBow.h"										// ġ���� ȭ��
#include "CEFFootStrength.h"									// ������ ����
#include "CEFFireSmack.h"										// ȭ�� ��Ÿ
#include "CEFFireArmor.h"										// ���̾� �Ƹ�
#include "CEFIceShiled.h"										// ���̽� �ǵ�
#include "CEFPriestStandAttack.h"								// ������ �⺻ ����
#include "CEFNpcFireBall.h"										// NPC ȭ�� ����
#include "CEFSrcLink.h"											// ���� �ٴ� ����Ʈ
#include "CEFKeepUpPump.h"										// ���� ����Ʈ���� �ߵ��ϰ� -> ��������Ʈ�� ����(EffectTargetHit()ó��)  
#include "CEFNPCBubble.h"										// NPC ���� ����
#include "CEFMouthFire.h"										// NPC ��->�� ����
#include "CEFFruitAttack.h"										// NPC ���� ����
#include "CEFNpcStoneAttack.h"									// NPC ¯�� ����
#include "CEFGageTrans.h"										// ������ ����
#include "CNPCStickAttack.h"									// NPC ������ ����
#include "CEFAddEffectBase.h"									// �ΰ� ȿ�� ����Ʈ
#include "CEFHide.h"											// ����
#include "CEFScapmper.h"										// ����
#include "CEFMomentHide.h"										// ����
#include "CEFBowRain.h"											// ������ ��
#include "CEFSniperBow.h"										// ����
#include "CEFHealRainBow.h"										// ġ���� ��
#include "CEFBeeLineBow.h"										// �������� ���󰡴� ȭ���
#include "CEFIceRain.h"											// ���̽� ����
#include "CEFHandPar.h"											// �⿭��
#include "CEFTyneeWave.h"										// Ÿ�̴� ���̺�
#include "CEFBigPress.h"										// ��������
#include "CEFSonBrush.h"										// Son Brush
#include "CEFEnergy.h"											// ������
#include "CEFDetect.h"											// Detect
#include "CEFLandDamage.h"										// ���� ������
#include "CEFSwordUp.h"											// Į ��þƮ
#include "CEFBowUp.h"											// Ȱ ��þƮ
#include "CEFBookUp.h"											// å ��þƮ
#include "CEFBallUp.h"											// ������ ��þƮ
#include "CEFStickUp.h"											// ������ ��þƮ
#include "CEFHealBubble.h"										// ���� �� ( NPC )
#include "CEFBigBubble.h"										// �չ�� ���� ( NPC )
#include "CEFHPSuction.h"										// HP ��� ( NPC )
#include "CEFSPSuction.h"										// SP ��� ( NPC )
#include "CEFIceRipBlow.h"										// ������ �Ա� 
#include "CEFMagicDelete.h"										// ����ȭ 
#include "CEFBigNpc0.h"
#include "CEFBigNpcSkill0.h"									// �Ŵ� ���� ��ų
#include "CEFWing0.h"											// ���� �׽�Ʈ
#include "CEFFireWork.h"										// �Ҳ� 
#include "CEFHolyAppear.h"										// �ż� ��Ÿ���� ����Ʈ
#include "CEFCommonFlySill.h"									// ���󰡴� ��ų ����Ʈ ����
#include "CEFBizEyeBeam.h"										// ���̺�
#include "CEFFastBow.h"											// �ż��� ȭ��
#include "CEFEmblemFlag.h"
#include "CEFFPopoiAttack.h"									// ������ ��Ÿ����
#include "CEFTigerAttack.h"										// ȣ���� �������� ���Ӱ��ݽ�ų
#include "CEFFameItem.h"										// ���� ���� ����Ʈ 
#include "CEFFameItemSet.h"										// ���� ����Ʈ 
#include "CEFFireRain.h"										// ���̾� ����

#include "CEFIceField.h"										// ���̽� �ʵ�
#include "CEFIceSpear.h"										// ���̽� ���Ǿ�
#include "CEFLightStorm.h"										// ����Ʈ ����
#include "CEFStoneRian.h"										// ��� ��
#include "CEFWindBush.h"										// ���� ����

#include "CEFRoundFire.h"										// ���� ���̾�
#include "CEFRoundLight.h"										// ���� ����Ʈ
#include "CEFCastStone.h"										// ĳ��Ʈ ����

#include "CEFEarthBind.h"										// ������ �ູ
#include "CEFRandomArrow.h"										// ����

#include "CEFConAniSkill.h"										// ���� �ִϸ��̼� ��ų

#include "CEFFireStormSmash.h"									// ��ǳ��Ÿ
#include "CEFSpiritFireSmack.h"									// �⿭��Ÿ
#include "CEFWrathKniteAttack.h"								// �г��� ����
#include "CEFCircleCrash.h"										// ��Ŭũ����
#include "CEFIceBowRain.h"										// ������ ȭ���

#include "CEFDarkness.h"
#include "CEFTripleDarkness.h"
#include "CEFTripleFire.h"
#include "CEFGloveDestruction.h"
#include "CEFHolyBomb.h"
#include "CEFDarkBomb.h"

#include "CEFWrath_Bomb.h"
#include "CEFBowDoubleShot.h"
#include "CEFHideSensor.h"

#include "CEFGTEffect.h"										// ����Ÿ�� ��������Ʈ
#include "CEFCannonEffect.h"									// �Ŵ���� ��������Ʈ
#include "CEFGolemEffect.h"										// �Žź� ���� ����Ʈ

#include "CEFBunchStrength.h"									// ������ �Ǽ�
#include "CEFBonWarSkill.h"										// �ذ������ ��ų
#include "CEFBonArcSkill.h"										// �ذ���Ŀ� ��ų

#include "CEFAniWing0.h"										// �ִ����� ����

#include "CEFSpeacialItem.h"									// Ŭ���� ���������� - ���� ����Ʈ

#include "CEFEldaWeaponItem.h"									// Ŭ���� ���ٽ��� ��������� ����Ʈ

#include "CEFDogmaEffect.h"										// ���׸� ����Ʈ
#include "CEFBaragosEff.h"										// �ٶ�� ����Ʈ
#include "CEFJanadEff.h"										// �𳪵� ����Ʈ 

#include "CEFRugalSkill.h"										// �簥 ����Ʈ

#include "CEFUrobosEff.h"										// ��κ��ν� ����Ʈ



float g_fRatio = 50.0f;

//................................................................................................................
// Create class 
//................................................................................................................
CEffectManager::CEffectManager()
{
	m_EffectState = CREATE_EFFECT;
	m_ISLandMarkDisplay = FALSE;
}

//................................................................................................................
// Destroy class 
//................................................................................................................
CEffectManager::~CEffectManager()
{
	Destroy();
	
	DeleteDevice();
}

//................................................................................................................
// InitDevice ( �� Ŭ���� ���� �ε��� �����͵� )
//................................................................................................................
int CEffectManager::InitDevice(void)
{
	CEFFireBall::InitDevice();
	CEFRockPress::InitDevice();
	CEFRockShield::InitDevice();
	CEFArrow::InitDevice();
	CEFFruitAttack::InitDevice();
	CEFNpcStoneAttack::InitDevice();
	CEFAddEffectBase::InitDevice();
	CEFWing0::InitDevice();
	CEFFireWork::InitDevice();
	CEFHolyAppear::InitDevice();
	CEFEmblemFlag::InitDevice();
	
	g_EffItemManager.LoadData();
	
	return TRUE;
}

//................................................................................................................
// DeleteDevice ( �� Ŭ���� ���� ���� �����͵� )
//................................................................................................................
int CEffectManager::DeleteDevice(void)
{
	CEFFireBall::DeleteDevice();
	CEFRockPress::DeleteDevice();
	CEFRockShield::DeleteDevice();
	CEFArrow::DeleteDevice();
	CEFFruitAttack::DeleteDevice();
	CEFNpcStoneAttack::DeleteDevice();
	CEFAddEffectBase::DeleteDevice();
	CEFWing0::DeleteDevice();
	CEFFireWork::DeleteDevice();
	CEFHolyAppear::DeleteDevice();
	CEFEmblemFlag::DeleteDevice();	
	
	return TRUE;
}

//................................................................................................................
// Destroy
//................................................................................................................
void CEffectManager::Destroy(void)
{
	std::list	<CEffectBase *> ::iterator	i = m_EffectList.begin();
	
	for( ; i != m_EffectList.end() ; )
	{
		CEffectBase * pData = (*i);
		SAFE_DELETE( pData );
		(*i) = NULL;
		
		m_EffectList.erase( i++ );
	}
	
}

std::list <CEffectBase *> CEffectManager::GetEffectList()
{
	return m_EffectList;
}

HRESULT CEffectManager::InvalidateDeviceObject()
{
	std::list	<CEffectBase *> ::iterator	i = m_EffectList.begin();
	
	for( ; i != m_EffectList.end() ; ++i )
	{
		(*i)->InvalidateDeviceObject();
	}
	return S_OK;
}

HRESULT CEffectManager::RestoreDeviceObject()
{
	std::list <CEffectBase *> ::iterator i = m_EffectList.begin();
	
	for( ; i != m_EffectList.end() ; ++i  )
	{
		(*i)->RestoreDeviceObject();
	}
	
	return S_OK;
}


//................................................................................................................
// Destroy
//................................................................................................................
void CEffectManager::AllClear(void)
{
	//.......................................................................................................
	// ��ü ����Ʈ ����
	//.......................................................................................................
	{
		std::list	<CEffectBase *> ::iterator	i = m_EffectList.begin();
		
		for( ; i != m_EffectList.end() ; )
		{
			
			if( NULL != *i )
			{
				Character    *pSrcChar = (*i)->GetSrcChar();
				
				if( pSrcChar != NULL )
				{
					if( !pSrcChar->PopEffectBase( (CEffectBase **)&(*i) ) )
					{
						DebugPrint( "\n Not Find PopEffectBase \n" );
					}
				}
				
				CEffectBase * pData = (*i);
				SAFE_DELETE( pData );
				(*i) = NULL;
			}
			
			m_EffectList.erase( i++ );
		}
	}
	
	//.......................................................................................................
	// ���� ����Ʈ ����
	//.......................................................................................................
	{
		std::list	<CEffectBase *> ::iterator	i = m_LandEffectList.begin();
		
		for( ; i != m_LandEffectList.end() ; )
		{
			m_LandEffectList.erase( i++ );
			
		}
	}
	
	m_ReadyLandEffectList.clear();
}


//................................................................................................................
// ���� ���� ����Ʈ �ߵ� ��� ����Ʈ�� �ֱ�
//................................................................................................................
int CEffectManager::PushReadyLandEffect(DWORD SkillUnique)
{
	if( ISFindReadLandEffect( SkillUnique ) )
	{
		DebugPrint( "already ready land effect\n");
		
		return FALSE;
	}
	
	m_ReadyLandEffectList.push_back( SkillUnique );
	
	DebugPrint( "PushReadyLandEffect\n");
	
	return TRUE;
}

//................................................................................................................
// ���� ���� ����Ʈ �ߵ��� ����Ʈ���� ����
//................................................................................................................
int CEffectManager::PopReadyLandEffect(DWORD SkillUnique)
{
	std::list  <DWORD> ::iterator i = m_ReadyLandEffectList.begin();
	
	for( ; i != m_ReadyLandEffectList.end() ; ++i )
	{
		if( (*i) == SkillUnique )
		{
			m_ReadyLandEffectList.erase( i++ );
			
			DebugPrint( "PopReadyLandEffect\n");
		}
	}
	
	return FALSE;
}

//................................................................................................................
// ���� ���� ����Ʈ �ߵ� ����Ʈ �̹� ����Ʈ�� �ֳ�?
//................................................................................................................
int CEffectManager::ISFindReadLandEffect(DWORD SkillUnique)
{
	std::list  <DWORD> ::iterator i = m_ReadyLandEffectList.begin();
	
	for( ; i != m_ReadyLandEffectList.end() ; ++i )
	{
		if( (*i) == SkillUnique )
		{
			return TRUE;
		}
	}
	
	return FALSE;
}

bool CEffectManager::IsFindEffectID(Character *pChar , EFFECT_ID id )
{
	if( pChar == NULL )
		return FALSE;
	
	std::list	<CEffectBase *>::iterator i = pChar->m_EffectList.begin();

	for( ; i != pChar->m_EffectList.end() ; ++i )
	{
		CEffectBase * pEffect = ((CEffectBase *)*i);
		
		if( pEffect->GetID() == id  )
		{
			return TRUE;
		}
	}

	return FALSE;
}

//................................................................................................................
// Update
//................................................................................................................
int CEffectManager::Update(float dtime)
{
	
	std::vector < std::list	<CEffectBase *> ::iterator > vTempVector;
	std::list	<CEffectBase *> ::iterator	i = m_EffectList.begin();
	
	for( ; i != m_EffectList.end() ; i++ )
	{
		CEffectBase *pEFBase = (*i);
		
		// Cell ����Ʈ�� �ҽ��� �˻����� ����
		if( pEFBase->m_ISLandLink != TRUE )
		{
			if( NULL == pEFBase->GetSrcChar() )
			{
				pEFBase->SetDestroy();
			}
			else if( pEFBase->GetSrcChar()->m_nTableIndex == -1 )
			{
				pEFBase->SetDestroy();
			}
			
			if( NULL != pEFBase->GetDestChar() )
			{
				if( pEFBase->GetDestChar()->m_nTableIndex == -1 )
				{
					pEFBase->SetDestroy();
				}
			}
		}
		
		if( pEFBase->GetState() != ES_SELF_DESTROY )
			pEFBase->Update( dtime );
		
		//........................................................................................................
		// ����Ʈ ����� 
		//........................................................................................................
		if( pEFBase->GetState() == ES_SELF_DESTROY )
		{
			if( NULL != pEFBase )
			{
				// Cell ����Ʈ
				if( pEFBase->m_ISLandLink == TRUE )
				{
					if( !PopLandEffectBase( (CEffectBase **)&pEFBase ) )
					{
					//	DebugPrint( "\n Not Find PopLandEffectBase \n" );
					}
				}
				else
				{
					//....................................................................................................
					// ( ĳ���ͳ� ����Ʈ�� ���� ������ )
					//....................................................................................................
					Character    *pSrcChar = pEFBase->GetSrcChar();
					
					if( pSrcChar != NULL )
					{
						if( !pSrcChar->PopEffectBase( (CEffectBase **)&pEFBase ) )
						{
						//	DebugPrint( "\n Not Find PopEffectBase \n" );
						}
					}
				}
				
				SAFE_DELETE( pEFBase );
				pEFBase = NULL;				
			}	

			vTempVector.push_back( i );			
		}
	}

	for( int j = 0 ; j < vTempVector.size() ; j++ )
	{
		m_EffectList.erase( vTempVector[j] );
	}
	
	return TRUE;
}

//................................................................................................................
// Render
//................................................................................................................
int CEffectManager::Render(float dtime)
{
	// �ٴڿ� ��ũ ��� Boa ��
	//	RenderToLandMark(dtime);
	
	std::list	<CEffectBase *> ::iterator	i = m_EffectList.begin();
	
	CEffectBase *pData;
	
	if( g_RockClient.m_SceneManager.GetEffectDisplayMode() == SCENE_NORMAL )
	{
		for( ; i != m_EffectList.end() ; ++i )
		{
			pData = ( CEffectBase * )( *i );
			
			if( NULL != pData ) 
			{
				if( pData->GetState() != ES_SELF_DESTROY )
				{
					if( pData->GetSrcChar() != NULL )
					{
						if( pData->GetSrcChar()->m_CharAlpha.GetState() == CHAR_NONE_DISPLAY ||
							!(pData->GetSrcChar()->m_IsAllLTexture) )
						{
							continue;
						}
					}
					
					pData->Render( dtime );
				}
			}
		}
	}
	else if( g_RockClient.m_SceneManager.GetEffectDisplayMode() == SCENE_SQUENCE )
	{
		for( ; i != m_EffectList.end() ; ++i )
		{
			if( NULL != ( pData = ( CEffectBase * )( *i ) ) ) 
			{
				if( (*i)->GetState() != ES_SELF_DESTROY )
				{
					if( (*i)->GetSrcChar() != NULL )
					{
						if( (*i)->GetSrcChar()->m_CharAlpha.GetState() == CHAR_NONE_DISPLAY ||
							!((*i)->GetSrcChar()->m_IsAllLTexture) )
						{
							continue;
						}
					}
					
					if( (*i)->m_SceneMode == !SCENE_SQUENCE )
						continue;
					
					(*i)->Render( dtime );
				}
			}
		}
	}
	
	
	return TRUE;
}

//................................................................................................................
// Push Effect
//................................................................................................................
int CEffectManager::PushEffect(CEffectBase **ppEffectBase)
{
	m_EffectList.push_back( *ppEffectBase );
	
	Character *pSrcChar = (*ppEffectBase)->GetSrcChar();
	
	(*ppEffectBase)->m_ISLandLink = FALSE;
	/*
	//.......................................................................................................
	// ����Ʈ �Ŵ����� ��ų Ÿ�Թ� �� ���� �Է� 
	// ���� ������ ���̵�� ����� -> Unique , Race , Skill Code , Cell
	//.......................................................................................................
	switch( m_TargetInfo.TargetType )
	{
	case n_PC:
	(*ppEffectBase)->SetPC();	
	break;
	case n_NPC:
	(*ppEffectBase)->SetNPC();	
	break;
	case n_Cell:
	(*ppEffectBase)->SetCell( m_TargetInfo.CellX , m_TargetInfo.CellY , m_TargetInfo.vPos );
	break;
	}
	//.......................................................................................................
	*/
	if( pSrcChar != NULL )
	{
		pSrcChar->PushEffectBase( ppEffectBase );	
		
		(*ppEffectBase)->m_SrcAniFrame = (*ppEffectBase)->m_SrcImpactFrame = 
			g_Pc_Manager.GetImpactFrame( pSrcChar , pSrcChar->m_ani_index);
	}
	else
	{
		(*ppEffectBase)->SetDestroy();
		
		DebugPrint( "------ Not Found Src Pointer PushEffect ( Delete ) ------\n" );
	}
	
	return TRUE;
}

//................................................................................................................
// Create ( Ȯ�� ���� ������ )
//................................................................................................................
int CEffectManager::Create(EFFECT_ID id,Character *pSrc,Character *pDest,DWORD event,
						   BYTE critical, WORD skillcode, struct _STargetInfo *pTargetInfo,
						   long lStartTime , DWORD SkillUnique )
{
	switch( m_EffectState )
	{
		// ����Ʈ ����
	case	CREATE_EFFECT:
		{
			EFFECT_EXTRA_INFO ExtraInfo;
			
			ExtraInfo.SkillUnique = SkillUnique;
			ExtraInfo.lStartTime = lStartTime;
			
			ExtraInfo.Init( pDest , event , critical , skillcode, pTargetInfo );
			
			CreateEffect( id , pSrc , pDest , &ExtraInfo );
		}
		break;
		
		// ����Ʈ ���� ó�� ����
	case CREATE_EFFECT_LOCK_ONETIME:
		{
			m_ExtraEffectQueue.Init( pDest , event , critical , skillcode, pTargetInfo );
			m_ExtraEffectQueue.lStartTime = lStartTime;
			
			m_EffectState = CREATE_EFFECT_LOCK;
		}
		break;
		
		// ����Ʈ �� 
	case CREATE_EFFECT_LOCK:
		{
			m_ExtraEffectQueue.SubInit(  pDest , event , critical , skillcode, pTargetInfo );
		}
		break;
		
		// ����Ʈ �� Ǯ���� ����Ʈ ������ -> CREATE_EFFECT ��
	case CREATE_EFFECT_UNLOCK:
		{
			m_ExtraEffectQueue.SubInit(  pDest , event , critical , skillcode, pTargetInfo );
			
			m_ExtraEffectQueue.SkillUnique = SkillUnique;
			
			CreateEffect( id , pSrc , m_ExtraEffectQueue.pDest , &m_ExtraEffectQueue );
			
			m_EffectState = CREATE_EFFECT;
		}
		break;
	}
	
	SetNPC();	
	
	return TRUE;
}

//................................................................................................................
// Create ( Ȯ�� ���� ������ )
//................................................................................................................
int CEffectManager::Create(EFFECT_ID id,Character *pSrc,Character *pDest)
{
	if( pSrc != NULL && pDest == NULL )
	{
		if( CheckCharacter( pSrc ) == FALSE )
		{
			return FALSE;
		}
	}
	else if( pSrc != NULL && pDest != NULL )
	{
		if( CheckCharacter( pSrc , pDest ) == FALSE )
		{
			return FALSE;
		}
	}
	else if( pSrc == NULL && pDest != NULL )
	{
		if( CheckCharacter( pDest ) == FALSE )
		{
			return FALSE;
		}
	}
	
	CreateEffect( id , pSrc , pDest , NULL );
	
	return TRUE;
}

//................................................................................................................
// ��� ����Ʈ ����
//................................................................................................................
int CEffectManager::CreateEffect(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo)
{

	if( pSrc != NULL && pDest == NULL )
	{
		if( CheckCharacter( pSrc ) == FALSE )
		{
			return FALSE;
		}
	}
	else if( pSrc != NULL && pDest != NULL )
	{
		if( CheckCharacter( pSrc , pDest ) == FALSE )
		{
			return FALSE;
		}
	}
	else if( pSrc == NULL && pDest != NULL )
	{
		if( CheckCharacter( pDest ) == FALSE )
		{
			return FALSE;
		}
	}
	
	if( pExtraInfo )
	{
		if( pExtraInfo->TargetInfo.theDieFlag )
		{
			RLGS("pExtraInfo.TargetInfo.theDieFlag CreateEffect()");
		}
	}
	
	switch( id )
	{
		// �ȱ�
		//		MAKE_EFFECT(EF_WALK);
		// �ȱ� ( �� )
		//		MAKE_EFFECT(EF_WATER_WAVE);
		//		MAKE_EFFECT(EF_LIGHTNING);
		// ������� ( �ӽ� ���̾�� )
		//		MAKE_OTHER(EF_GLARE_CUTTING4)
		// ȭ�̾� ��
		//		MAKE_EFFECT(EF_FIREBALL);
		// �����
		//		MAKE_EFFECT(EF_ENERGY_SPREAD);
		
		//........................................................................................................ 
		// �ȱ� ����
		//........................................................................................................ 
		MAKE_EFFECT(EF_RUN_DUST,EF_WALK_DUST);
		MAKE_EFFECT(EF_WALK_DUST,EF_WALK_DUST);
		
		//........................................................................................................ 
		// �ȱ� �� ����
		//........................................................................................................ 
		MAKE_EFFECT(EF_RUN_WAVE,EF_WALK_WAVE);
		MAKE_EFFECT(EF_WALK_WAVE,EF_WALK_WAVE);
		
		//........................................................................................................ 
		// ȭ�̾
		//........................................................................................................ 
		MAKE_EFFECT(EF_FIREBALL4,EF_FIREBALL0);
		MAKE_EFFECT(EF_FIREBALL3,EF_FIREBALL0);
		MAKE_EFFECT(EF_FIREBALL2,EF_FIREBALL0);
		MAKE_EFFECT(EF_FIREBALL1,EF_FIREBALL0);
		MAKE_EFFECT(EF_FIREBALL0,EF_FIREBALL0);
		
		//........................................................................................................ 
		// ����Ʈ ��
		//........................................................................................................ 
		MAKE_EFFECT(EF_LIGHTNING4,EF_LIGHTNING0);
		MAKE_EFFECT(EF_LIGHTNING3,EF_LIGHTNING0);
		MAKE_EFFECT(EF_LIGHTNING2,EF_LIGHTNING0);
		MAKE_EFFECT(EF_LIGHTNING1,EF_LIGHTNING0);
		MAKE_EFFECT(EF_LIGHTNING0,EF_LIGHTNING0);
		
		
		//........................................................................................................
		// �⺻ ���� ����Ʈ
		//........................................................................................................
		
		// �⺻ Į ����
		MAKE_EFFECT(EF_STAND_SWORD_ATTACK,EF_STAND_SWORD_ATTACK);
		
		// �⺻ å ����
		MAKE_EFFECT(EF_STAND_BOOK_ATTACK,EF_STAND_BOOK_ATTACK);
		
		// �⺻ ������ ����
		MAKE_EFFECT(EF_STAND_STICK_ATTACK,EF_STAND_STICK_ATTACK);
		
		// �⺻ �̻� ����
		MAKE_EFFECT(EF_STAND_TOOTH_ATTACK,EF_STAND_TOOTH_ATTACK);
		
		// �⺻ �б� ����
		MAKE_EFFECT(EF_STAND_DULL_ATTACK,EF_STAND_DULL_ATTACK);
		
		// �⺻ ���� ����
		MAKE_EFFECT(EF_STAND_NAIL_ATTACK,EF_STAND_NAIL_ATTACK);
		
		MAKE_EFFECT(EF_STAND_SWORD_ATTACKED0,EF_STAND_SWORD_ATTACKED0); // 	Į�� �⺻ ������ ( �� )
		MAKE_EFFECT(EF_STAND_SWORD_ATTACKED1,EF_STAND_SWORD_ATTACKED1); // 	Į�� �⺻ ������ ( �� )
		MAKE_EFFECT(EF_STAND_SWORD_ATTACKED2,EF_STAND_SWORD_ATTACKED2); // 	Į�� �⺻ ������ ( �� )
		
		MAKE_EFFECT(EF_STAND_BOOK_ATTACKED0,EF_STAND_BOOK_ATTACKED0);  // 	�����⺻����(å) ������ ( �� ) 
		MAKE_EFFECT(EF_STAND_BOOK_ATTACKED1,EF_STAND_BOOK_ATTACKED1);  // 	�����⺻����(å) ������ ( �� ) 
		MAKE_EFFECT(EF_STAND_BOOK_ATTACKED2,EF_STAND_BOOK_ATTACKED2);  // 	�����⺻����(å) ������ ( �� ) 
		
		MAKE_EFFECT(EF_STAND_STICK_ATTACKED0,EF_STAND_STICK_ATTACKED0); // 	�����⺻����(������) ������ ( �� ) 
		MAKE_EFFECT(EF_STAND_STICK_ATTACKED1,EF_STAND_STICK_ATTACKED1); // 	�����⺻����(������) ������ ( �� ) 
		MAKE_EFFECT(EF_STAND_STICK_ATTACKED2,EF_STAND_STICK_ATTACKED2); // 	�����⺻����(������) ������ ( �� ) 
		
		MAKE_EFFECT(EF_STAND_TOOTH_ATTACKED0,EF_STAND_TOOTH_ATTACKED0); // 	�̻� �⺻ ����   ������ ( �� ) 
		MAKE_EFFECT(EF_STAND_TOOTH_ATTACKED1,EF_STAND_TOOTH_ATTACKED1); // 	�̻� �⺻ ����   ������ ( �� ) 
		MAKE_EFFECT(EF_STAND_TOOTH_ATTACKED2,EF_STAND_TOOTH_ATTACKED2); // 	�̻� �⺻ ����   ������ ( �� ) 
		
		MAKE_EFFECT(EF_STAND_DULL_ATTACKED0,EF_STAND_DULL_ATTACKED0);  // 	�б� �⺻ ����   ������ ( �� ) 
		MAKE_EFFECT(EF_STAND_DULL_ATTACKED1,EF_STAND_DULL_ATTACKED1);  // 	�б� �⺻ ����   ������ ( �� ) 
		MAKE_EFFECT(EF_STAND_DULL_ATTACKED2,EF_STAND_DULL_ATTACKED2);  // 	�б� �⺻ ����   ������ ( �� ) 
		
		MAKE_EFFECT(EF_STAND_NAIL_ATTACKED0,EF_STAND_NAIL_ATTACKED0);  //	���� �⺻ ����  ������ ( �� )
		MAKE_EFFECT(EF_STAND_NAIL_ATTACKED1,EF_STAND_NAIL_ATTACKED1);  //	���� �⺻ ����  ������ ( �� )
		MAKE_EFFECT(EF_STAND_NAIL_ATTACKED2,EF_STAND_NAIL_ATTACKED2);  //	���� �⺻ ����  ������ ( �� )
		
//		MAKE_EFFECT(EF_RUGAL_BASIC_ATTACKED,EF_RUGAL_BASIC_ATTACKED);  //	�簥 �⺻ ����  ������ ( �� )
//		MAKE_EFFECT(EF_RUGAL_CRI_ATTACKED,EF_RUGAL_CRI_ATTACKED);  //	�簥 �⺻ ����  ������ ( ��  )
//		MAKE_EFFECT(EF_RUGALSOLDART_BASIC_ATTACKED,EF_RUGALSOLDART_BASIC_ATTACKED);  //	�簥 �⺻ ����  ������ ( �� )
//		MAKE_EFFECT(EF_RUGALSOLDART_CRI_ATTACKED,EF_RUGALSOLDART_CRI_ATTACKED);  //	�簥 �⺻ ����  ������ ( �� )
		MAKE_EFFECT(EF_RUGALTEMPEST_CRI_ATTACKED,EF_RUGALTEMPEST_CRI_ATTACKED);  //	�簥 �⺻ ����  ������ (  )




		// �� ������
		MAKE_EFFECT(EF_ROCKPRESS4,EF_ROCKPRESS0);
		MAKE_EFFECT(EF_ROCKPRESS3,EF_ROCKPRESS0);
		MAKE_EFFECT(EF_ROCKPRESS2,EF_ROCKPRESS0);
		MAKE_EFFECT(EF_ROCKPRESS1,EF_ROCKPRESS0);
		MAKE_EFFECT(EF_ROCKPRESS0,EF_ROCKPRESS0);
		
		// ��� �ҵ�
		MAKE_EFFECT(EF_WOODENSWORD4,EF_WOODENSWORD0);
		MAKE_EFFECT(EF_WOODENSWORD3,EF_WOODENSWORD0);
		MAKE_EFFECT(EF_WOODENSWORD2,EF_WOODENSWORD0);
		MAKE_EFFECT(EF_WOODENSWORD1,EF_WOODENSWORD0);
		MAKE_EFFECT(EF_WOODENSWORD0,EF_WOODENSWORD0);
		
		// �� �ǵ� ( ���� ��ų )
		MAKE_EFFECT(EF_ROCK_SHIELD0,EF_ROCK_SHIELD0);
		MAKE_EFFECT(EF_ROCK_SHIELD1,EF_ROCK_SHIELD0);
		MAKE_EFFECT(EF_ROCK_SHIELD2,EF_ROCK_SHIELD0);
		MAKE_EFFECT(EF_ROCK_SHIELD3,EF_ROCK_SHIELD0);
		MAKE_EFFECT(EF_ROCK_SHIELD4,EF_ROCK_SHIELD0);
		
		// ��� ( ���� ��ų )
		MAKE_EFFECT(EF_REFRESH_SWORD0,EF_REFRESH_SWORD0);
		MAKE_EFFECT(EF_REFRESH_SWORD1,EF_REFRESH_SWORD0);
		MAKE_EFFECT(EF_REFRESH_SWORD2,EF_REFRESH_SWORD0);
		MAKE_EFFECT(EF_REFRESH_SWORD3,EF_REFRESH_SWORD0);
		MAKE_EFFECT(EF_REFRESH_SWORD4,EF_REFRESH_SWORD0);
		
		// ���� ( ���� ��ų )
		MAKE_EFFECT(EF_STRONG_SWORD0,EF_STRONG_SWORD0);
		MAKE_EFFECT(EF_STRONG_SWORD1,EF_STRONG_SWORD0);
		MAKE_EFFECT(EF_STRONG_SWORD2,EF_STRONG_SWORD0);
		MAKE_EFFECT(EF_STRONG_SWORD3,EF_STRONG_SWORD0);
		MAKE_EFFECT(EF_STRONG_SWORD4,EF_STRONG_SWORD0);
		
		// ��� ����
		MAKE_EFFECT(EF_V_CUT_ATTACK0,EF_V_CUT_ATTACK0);
		MAKE_EFFECT(EF_V_CUT_ATTACK1,EF_V_CUT_ATTACK0);
		MAKE_EFFECT(EF_V_CUT_ATTACK2,EF_V_CUT_ATTACK0);
		MAKE_EFFECT(EF_V_CUT_ATTACK3,EF_V_CUT_ATTACK0);
		MAKE_EFFECT(EF_V_CUT_ATTACK4,EF_V_CUT_ATTACK0);
		
		MAKE_EFFECT(EF_CAP_SWORD_ATTACK0,EF_CAP_SWORD_ATTACK0);
		MAKE_EFFECT(EF_CAP_SWORD_ATTACK1,EF_CAP_SWORD_ATTACK0);
		MAKE_EFFECT(EF_CAP_SWORD_ATTACK2,EF_CAP_SWORD_ATTACK0);
		MAKE_EFFECT(EF_CAP_SWORD_ATTACK3,EF_CAP_SWORD_ATTACK0);
		MAKE_EFFECT(EF_CAP_SWORD_ATTACK4,EF_CAP_SWORD_ATTACK0);
		
		// ��� ����
		MAKE_EFFECT(EF_GATHER_CUT0,EF_GATHER_CUT0);
		MAKE_EFFECT(EF_GATHER_CUT1,EF_GATHER_CUT0);
		MAKE_EFFECT(EF_GATHER_CUT2,EF_GATHER_CUT0);
		MAKE_EFFECT(EF_GATHER_CUT3,EF_GATHER_CUT0);
		MAKE_EFFECT(EF_GATHER_CUT4,EF_GATHER_CUT0);
		
		// ���̽� ���Ǿ�
		MAKE_EFFECT(EF_ICESPIRE0,EF_ICESPIRE0);
		MAKE_EFFECT(EF_ICESPIRE1,EF_ICESPIRE0);
		MAKE_EFFECT(EF_ICESPIRE2,EF_ICESPIRE0);
		MAKE_EFFECT(EF_ICESPIRE3,EF_ICESPIRE0);
		MAKE_EFFECT(EF_ICESPIRE4,EF_ICESPIRE0);
		
		// ���� ����
		MAKE_EFFECT(EF_WEAPON_CRASH0,EF_WEAPON_CRASH0);
		MAKE_EFFECT(EF_WEAPON_CRASH1,EF_WEAPON_CRASH0);
		MAKE_EFFECT(EF_WEAPON_CRASH2,EF_WEAPON_CRASH0);
		MAKE_EFFECT(EF_WEAPON_CRASH3,EF_WEAPON_CRASH0);
		MAKE_EFFECT(EF_WEAPON_CRASH4,EF_WEAPON_CRASH0);
		
		// ȸ��
		MAKE_EFFECT(EF_AVOID0,EF_AVOID0);
		MAKE_EFFECT(EF_AVOID1,EF_AVOID0);
		MAKE_EFFECT(EF_AVOID2,EF_AVOID0);
		MAKE_EFFECT(EF_AVOID3,EF_AVOID0);
		MAKE_EFFECT(EF_AVOID4,EF_AVOID0);
		
		// ���� ũ����
		MAKE_EFFECT(EF_BUBBLE_CRASH0,EF_BUBBLE_CRASH0);
		MAKE_EFFECT(EF_BUBBLE_CRASH1,EF_BUBBLE_CRASH0);
		MAKE_EFFECT(EF_BUBBLE_CRASH2,EF_BUBBLE_CRASH0);
		MAKE_EFFECT(EF_BUBBLE_CRASH3,EF_BUBBLE_CRASH0);
		MAKE_EFFECT(EF_BUBBLE_CRASH4,EF_BUBBLE_CRASH0);
		
		// ȸ�� ������1 ��ȿ�� 
		MAKE_EFFECT(EF_RECOVERY_ITEM1_0,EF_RECOVERY_ITEM1_0);
		MAKE_EFFECT(EF_RECOVERY_ITEM1_1,EF_RECOVERY_ITEM1_0);
		MAKE_EFFECT(EF_RECOVERY_ITEM1_2,EF_RECOVERY_ITEM1_0);
		MAKE_EFFECT(EF_RECOVERY_ITEM1_3,EF_RECOVERY_ITEM1_0);
		MAKE_EFFECT(EF_RECOVERY_ITEM1_4,EF_RECOVERY_ITEM1_0);
		
		// ȸ�� ������2 ���Ӽ� 
		MAKE_EFFECT(EF_RECOVERY_ITEM2_0,EF_RECOVERY_ITEM2_0);
		MAKE_EFFECT(EF_RECOVERY_ITEM2_1,EF_RECOVERY_ITEM2_0);
		MAKE_EFFECT(EF_RECOVERY_ITEM2_2,EF_RECOVERY_ITEM2_0);
		MAKE_EFFECT(EF_RECOVERY_ITEM2_3,EF_RECOVERY_ITEM2_0);
		MAKE_EFFECT(EF_RECOVERY_ITEM2_4,EF_RECOVERY_ITEM2_0);
		
		// ȸ�� ������2 ���Ӽ� 
		MAKE_EFFECT(EF_BOW_ATTACK0,EF_BOW_ATTACK0);
		
		// �⺻ Ȱ���� ������
		MAKE_EFFECT(EF_BOW_ATTACKED_0,EF_BOW_ATTACKED0);
		MAKE_EFFECT(EF_BOW_ATTACKED_1,EF_BOW_ATTACKED0);
		MAKE_EFFECT(EF_BOW_ATTACKED_2,EF_BOW_ATTACKED0);
		
		// ��� ���
		MAKE_EFFECT(EF_GLAER_BOW_0,EF_GLAER_BOW_ATTACK);
		MAKE_EFFECT(EF_GLAER_BOW_1,EF_GLAER_BOW_ATTACK);
		MAKE_EFFECT(EF_GLAER_BOW_2,EF_GLAER_BOW_ATTACK);
		
		MAKE_EFFECT(EF_DOUBLESHOT , EF_DOUBLESHOT);
		
		// �ߵ� ���
		MAKE_EFFECT(EF_FOOT_BOW_0,EF_FOOT_BOW_ATTACK);
		MAKE_EFFECT(EF_FOOT_BOW_1,EF_FOOT_BOW_ATTACK);
		MAKE_EFFECT(EF_FOOT_BOW_2,EF_FOOT_BOW_ATTACK);
		
		// �ߵ� ȭ�� �ٴ°� 
		MAKE_EFFECT(EF_FOOT_BOW_LINK,EF_FOOT_BOW_LINK);
		
		// ȭ�� ��Ÿ
		MAKE_EFFECT(EF_FIRE_SMACK_0,EF_FIRE_SMACK);
		MAKE_EFFECT(EF_FIRE_SMACK_1,EF_FIRE_SMACK);
		MAKE_EFFECT(EF_FIRE_SMACK_2,EF_FIRE_SMACK);
		
		// ����
		MAKE_EFFECT(EF_POSSESSED_0,EF_POSSESSED);
		MAKE_EFFECT(EF_POSSESSED_1,EF_POSSESSED);
		MAKE_EFFECT(EF_POSSESSED_2,EF_POSSESSED);
		MAKE_EFFECT(EF_POSSESSED_3,EF_POSSESSED);
		MAKE_EFFECT(EF_POSSESSED_4,EF_POSSESSED);
		
		// ���� ��Ÿ����
		MAKE_EFFECT(EF_APPEAR_BING0,EF_APPEAR_BING);
		MAKE_EFFECT(EF_APPEAR_BING1,EF_APPEAR_BING);
		MAKE_EFFECT(EF_APPEAR_BING2,EF_APPEAR_BING);
		MAKE_EFFECT(EF_APPEAR_BING3,EF_APPEAR_BING);
		MAKE_EFFECT(EF_APPEAR_BING4,EF_APPEAR_BING);
		
		// ���� ����
		MAKE_EFFECT(EF_POSSESSED_CANCEL,EF_POSSESSED_CANCEL);
		
		// �ӻ�
		MAKE_EFFECT(EF_QUICK_BOW_0,EF_QUICK_BOW);
		MAKE_EFFECT(EF_QUICK_BOW_1,EF_QUICK_BOW);
		MAKE_EFFECT(EF_QUICK_BOW_2,EF_QUICK_BOW);
		
		// ����
		MAKE_EFFECT(EF_FOCUS_BOW_0,EF_FOCUS_BOW);
		MAKE_EFFECT(EF_FOCUS_BOW_1,EF_FOCUS_BOW);
		MAKE_EFFECT(EF_FOCUS_BOW_2,EF_FOCUS_BOW);
		
		// ġ���� ȭ��
		MAKE_EFFECT(EF_REFRESH_BOW_0,EF_REFRESH_BOW);
		MAKE_EFFECT(EF_REFRESH_BOW_1,EF_REFRESH_BOW);
		MAKE_EFFECT(EF_REFRESH_BOW_2,EF_REFRESH_BOW);
		
		// ���ڰ�
		MAKE_EFFECT(EF_FOOT_LIGHTNING_0,EF_FOOT_LIGHTNING);
		MAKE_EFFECT(EF_FOOT_LIGHTNING_1,EF_FOOT_LIGHTNING);
		MAKE_EFFECT(EF_FOOT_LIGHTNING_2,EF_FOOT_LIGHTNING);
		
		// ���� ���
		MAKE_EFFECT(EF_ABSOLUTE_DP_0,EF_ABSOLUTE_DP);
		MAKE_EFFECT(EF_ABSOLUTE_DP_1,EF_ABSOLUTE_DP);
		MAKE_EFFECT(EF_ABSOLUTE_DP_2,EF_ABSOLUTE_DP);
		
		// ������ ����
		MAKE_EFFECT(EF_FOOT_STRENGTH_0,EF_FOOT_STRENGTH);
		MAKE_EFFECT(EF_FOOT_STRENGTH_1,EF_FOOT_STRENGTH);
		MAKE_EFFECT(EF_FOOT_STRENGTH_2,EF_FOOT_STRENGTH);
		
		MAKE_EFFECT(EF_HAND_ATTACK,EF_HAND_ATTACK0);
		
		// �� �⺻ ����
		MAKE_EFFECT(EF_HAND_ATTACKED0,EF_HAND_ATTACKED);
		MAKE_EFFECT(EF_HAND_ATTACKED1,EF_HAND_ATTACKED);
		MAKE_EFFECT(EF_HAND_ATTACKED2,EF_HAND_ATTACKED);
		
		// ���� �⺻ ������ ����
		MAKE_EFFECT(EF_BALL_ATTACK,EF_BALL_ATTACK);
		MAKE_EFFECT(EF_BALL_ATTACKED0,EF_BALL_ATTACKED);
		MAKE_EFFECT(EF_BALL_ATTACKED1,EF_BALL_ATTACKED);
		MAKE_EFFECT(EF_BALL_ATTACKED2,EF_BALL_ATTACKED);
		
		//		// �޾�ġ��
		//		MAKE_EFFECT(EF_BOUND_ATTACK,EF_BOUND_ATTACK);
		
		// ���̾� �Ƹ�
		MAKE_EFFECT(EF_FIRE_ARMOR0,EF_FIRE_ARMOR);
		MAKE_EFFECT(EF_FIRE_ARMOR1,EF_FIRE_ARMOR);
		MAKE_EFFECT(EF_FIRE_ARMOR2,EF_FIRE_ARMOR);
		
		// ���̽� �ǵ�
		MAKE_EFFECT(EF_ICE_SHILED0,EF_ICE_SHILED);
		MAKE_EFFECT(EF_ICE_SHILED1,EF_ICE_SHILED);
		MAKE_EFFECT(EF_ICE_SHILED2,EF_ICE_SHILED);
		
		//........................................................................................................
		// 3�� �߰� ����Ʈ
		//........................................................................................................
		// ����Ʈ �⺻ ����
		MAKE_EFFECT(EF_NIGHT_STAND_ATTACK,EF_NIGHT_STAND_ATTACK);
		MAKE_EFFECT(EF_NIGHT_STAND_ATTACKED0,EF_NIGHT_STAND_ATTACKED);
		MAKE_EFFECT(EF_NIGHT_STAND_ATTACKED1,EF_NIGHT_STAND_ATTACKED);
		MAKE_EFFECT(EF_NIGHT_STAND_ATTACKED2,EF_NIGHT_STAND_ATTACKED);
		
		// ���� ����
		MAKE_EFFECT(EF_SHILED_ATTACK0,EF_SHILED_ATTACKED);
		MAKE_EFFECT(EF_SHILED_ATTACK1,EF_SHILED_ATTACKED);
		MAKE_EFFECT(EF_SHILED_ATTACK2,EF_SHILED_ATTACKED);
		// ��� �¼�
		MAKE_EFFECT(EF_DEFEND_SETUP,EF_DEFEND_SETUP);
		
		// �����̾� �⺻ ����
		MAKE_EFFECT(EF_SLAYER_STAND_ATTACK,EF_SLAYER_STAND_ATTACK);
		MAKE_EFFECT(EF_SLAYER_STAND_ATTACKED0,EF_SLAYER_STAND_ATTACKED);
		MAKE_EFFECT(EF_SLAYER_STAND_ATTACKED1,EF_SLAYER_STAND_ATTACKED);
		MAKE_EFFECT(EF_SLAYER_STAND_ATTACKED2,EF_SLAYER_STAND_ATTACKED);
		
		// ���� ����
		MAKE_EFFECT(EF_BLOOD_RUNAWAY,EF_BLOOD_RUNAWAY);
		
		// �ұ��� ����
		MAKE_EFFECT(EF_LIVE_MIND,EF_LIVE_MIND);
		
		// ������Ʈ �⺻ ����
		MAKE_EFFECT(EF_PRIEST_STAND_ATTACK,EF_PRIEST_STAND_ATTACK);
		MAKE_EFFECT(EF_PRIEST_STAND_ATTACKED0,EF_PRIEST_STAND_ATTACKED);
		MAKE_EFFECT(EF_PRIEST_STAND_ATTACKED1,EF_PRIEST_STAND_ATTACKED);
		MAKE_EFFECT(EF_PRIEST_STAND_ATTACKED2,EF_PRIEST_STAND_ATTACKED);
		
		// ������Ʈ ���� 
		MAKE_EFFECT(EF_HEAL0,EF_PRIEST_HEAL);
		MAKE_EFFECT(EF_HEAL1,EF_PRIEST_HEAL);
		MAKE_EFFECT(EF_HEAL2,EF_PRIEST_HEAL);
		
		// ������Ʈ ����
		MAKE_EFFECT(EF_DISPELL,EF_PRIEST_DISPELL);
		
		// ������Ʈ ����� ����
		MAKE_EFFECT(EF_LUCKY_GOD,EF_LUCKY_GOD);
		// ������Ʈ ����� ���� ���ϴ���
		MAKE_EFFECT(EF_LUCKY_GODED,EF_LUCKY_GODED);
		
		// ������Ʈ ��Ȱ
		MAKE_EFFECT(EF_REVIVAL,EF_REVIVAL);
		
		// �ӷϽ� ��������
		MAKE_EFFECT(EF_NPC_WOOD_ATTACK,EF_NPC_WOOD_ATTACK);
		MAKE_EFFECT(EF_NPC_WOOD_ATTACKED0,EF_NPC_WOOD_ATTACKED);
		MAKE_EFFECT(EF_NPC_WOOD_ATTACKED1,EF_NPC_WOOD_ATTACKED);
		MAKE_EFFECT(EF_NPC_WOOD_ATTACKED2,EF_NPC_WOOD_ATTACKED);
		
		// ���� ȭ�̾
		MAKE_EFFECT(EF_NPC_FIRE,EF_NPC_FIRE);					// �⺻ ����
		MAKE_EFFECT(EF_NPC_FIRE_SKILL,EF_NPC_FIRE);				// ��ų
		
		MAKE_EFFECT(EF_NPC_FIREED0,EF_NPC_FIREED);				// �⺻ ���� ������ ����Ʈ
		MAKE_EFFECT(EF_NPC_FIREED1,EF_NPC_FIREED);
		MAKE_EFFECT(EF_NPC_FIREED2,EF_NPC_FIREED);
		
		// NPC ���̸� ��θӸ� ������ ( �⺻ ���� )
		MAKE_EFFECT(EF_NPC_HEAVY_STICK,EF_NPC_HEAVY_STICK);
		MAKE_EFFECT(EF_NPC_HEAVY_STICKED0,EF_NPC_HEAVY_STICKED);
		MAKE_EFFECT(EF_NPC_HEAVY_STICKED1,EF_NPC_HEAVY_STICKED);
		MAKE_EFFECT(EF_NPC_HEAVY_STICKED2,EF_NPC_HEAVY_STICKED);
		
		// �߱�����
		MAKE_EFFECT(EF_FOOT_QUAKE0,EF_FOOT_QUAKE);
		MAKE_EFFECT(EF_FOOT_QUAKE1,EF_FOOT_QUAKE);
		MAKE_EFFECT(EF_FOOT_QUAKE2,EF_FOOT_QUAKE);
		
		// ����
		MAKE_EFFECT(EF_ATTACKED_FOCUS,EF_ATTACKED_FOCUS);
		
		// ����
		MAKE_EFFECT(EF_SWORD_FLASH0,EF_SWORD_FLASH);
		MAKE_EFFECT(EF_SWORD_FLASH1,EF_SWORD_FLASH);
		MAKE_EFFECT(EF_SWORD_FLASH2,EF_SWORD_FLASH);
		
		// ��Ƽ ����
		MAKE_EFFECT(EF_PARTY_HEAL,EF_PARTY_HEAL);
		
		// ���̸� ��θӸ� �ٴ� ��� ( ��ų )
		MAKE_EFFECT(EF_NPC_QUAKE_HEAVY_STICK,EF_NPC_QUAKE_HEAVY_STICK);
		
		// �����
		MAKE_EFFECT(EF_CIRCLE_SWORD,EF_CIRCLE_SWORD);
		
		MAKE_EFFECT_TIMER(EF_PRESSURE_AURA,EF_KEEPUP_PUMP,3.1f);
		MAKE_EFFECT_TIMER(EF_WARM_LIGHT,EF_KEEPUP_PUMP,3.1f);
		
		MAKE_EFFECT(EF_WARM_LIGHT1,EF_SRC_LINK);
		MAKE_EFFECT(EF_PRESSURE_AURA1,EF_SRC_LINK);
		
		
		MAKE_EFFECT(EF_PRESSURE_AURAED,EF_SRC_LINK);
		MAKE_EFFECT(EF_WARM_LIGHTED,EF_SRC_LINK);
		//MAKE_EFFECT(EF_WARM_LIGHTED,EF_WARM_LIGHTED);
		
		// NPC ����� ���� ��ų
		MAKE_EFFECT(EF_BUBBLE_ATTACK,EF_NPC_BUBBLE_ATTACK);
		
		// NPC ������ �ٴ����
		MAKE_EFFECT(EF_AXE_LAND_HIT,EF_AXE_LAND_HIT);
		
		// �����佺 �� -> ��
		MAKE_EFFECT(EF_MOUTH_FIRE,EF_MOUTH_FIRE);
		MAKE_EFFECT(EF_MOUTH_FIRE1,EF_MOUTH_FIRE);
		
		// NPC ���� ����
		MAKE_EFFECT(EF_FRUIT_ATTACK,EF_FRUIT_ATTACK);
		
		MAKE_EFFECT(EF_ROCK_SHOT,EF_ROCK_SHOT);
		MAKE_EFFECT(EF_ROCK_SHOT1,EF_ROCK_SHOT);
		
		// ���� ����
		MAKE_EFFECT(EF_GAS_SHOT,EF_GAS_SHOT);
		
		// ������ ����
		MAKE_EFFECT(EF_GAGE_TRANS,EF_GAGE_TRANS);
		
		// NPC ������ ���� ( �⺻ ���� )
		MAKE_EFFECT(EF_NPC_STICK,EF_NPC_STICK);
		// NPC ������ ���� ������ ( �⺻ ���� )
		MAKE_EFFECT(EF_NPC_STICK_ATTACKED0,EF_NPC_STICK_ATTACKED);
		MAKE_EFFECT(EF_NPC_STICK_ATTACKED1,EF_NPC_STICK_ATTACKED);
		MAKE_EFFECT(EF_NPC_STICK_ATTACKED2,EF_NPC_STICK_ATTACKED);
		
		// �ΰ� ȿ��
		MAKE_EFFECT(EF_MOVE_OBSTRUCTION,EF_ADDEFFECT_BASE);	 			// �̵����
		MAKE_EFFECT(EF_FAINT,EF_ADDEFFECT_BASE);						// ���� or ����
		MAKE_EFFECT(EF_SLEEP,EF_ADDEFFECT_BASE);						// ����
		MAKE_EFFECT(EF_POISONING,EF_ADDEFFECT_BASE);					// �ߵ�
		MAKE_EFFECT(EF_PROVOCATION,EF_ADDEFFECT_BASE);					// ����
		MAKE_EFFECT(EF_COMBO_OBSTRUCTION,EF_ADDEFFECT_BASE);			// �޺����		
		MAKE_EFFECT(EF_FREEZING,EF_ADDEFFECT_BASE);						// ����			
		MAKE_EFFECT(EF_STONY,EF_ADDEFFECT_BASE);						// ��ȭ			
		MAKE_EFFECT(EF_WHISPER,EF_ADDEFFECT_BASE);						// ħ��
		MAKE_EFFECT(EF_PARALYSYS,EF_ADDEFFECT_BASE);
		MAKE_EFFECT(EF_SKILL_DISABLE,EF_ADDEFFECT_BASE);
		MAKE_EFFECT(EF_ADD_CURSE,EF_ADDEFFECT_BASE);
		MAKE_EFFECT(EF_ADD_UP,EF_ADDEFFECT_BASE);
		
		//...........................................................................................................
		// �̱״ϼ�
		//...........................................................................................................
		MAKE_EFFECT(EF_EGNEETION0,EF_EGNEETION);						// �̱״ϼ�
		MAKE_EFFECT(EF_EGNEETION1,EF_EGNEETION);
		MAKE_EFFECT(EF_EGNEETION2,EF_EGNEETION);
		//...........................................................................................................
		// �̱״ϼ� ������
		//...........................................................................................................
		MAKE_EFFECT(EF_EGNEETION_ED0,EF_EGNEETION_ED);					// �̱״ϼ� ������ ( ���� )
		MAKE_EFFECT(EF_EGNEETION_ED1,EF_EGNEETION_ED);
		MAKE_EFFECT(EF_EGNEETION_ED2,EF_EGNEETION_ED);
		//...........................................................................................................
		// ����
		//...........................................................................................................
		MAKE_EFFECT(EF_CHARGE,EF_CHARGE);								// ���� ( �� �� )
		//...........................................................................................................
		// ĥ�� ��ġ
		//...........................................................................................................
		MAKE_EFFECT(EF_CR_TOUCH0,EF_CR_TOUCH);
		MAKE_EFFECT(EF_CR_TOUCH1,EF_CR_TOUCH);
		MAKE_EFFECT(EF_CR_TOUCH2,EF_CR_TOUCH);
		
		//...........................................................................................................
		// ���� ���̺�
		//...........................................................................................................
		MAKE_EFFECT(EF_HEAL_WAVE0,EF_HEAL_WAVE);
		MAKE_EFFECT(EF_HEAL_WAVE1,EF_HEAL_WAVE);
		MAKE_EFFECT(EF_HEAL_WAVE2,EF_HEAL_WAVE);
		
		//...........................................................................................................
		// �޸��� ����Ʈ
		//...........................................................................................................
		MAKE_EFFECT(EF_PEAL_DUST0,EF_PEAL_DUST);
		MAKE_EFFECT(EF_PEAL_DUST1,EF_PEAL_DUST);
		MAKE_EFFECT(EF_PEAL_DUST2,EF_PEAL_DUST);
		
		MAKE_EFFECT(EF_PEAL_DUST_ED0,EF_SRC_LINK);
		MAKE_EFFECT(EF_PEAL_DUST_ED1,EF_SRC_LINK);
		MAKE_EFFECT(EF_PEAL_DUST_ED2,EF_SRC_LINK);
		
		//...........................................................................................................
		// ������ �õ�
		//...........................................................................................................
		MAKE_EFFECT(EF_POISON_SEED0,EF_POISON_SEED);
		MAKE_EFFECT(EF_POISON_SEED1,EF_POISON_SEED);
		MAKE_EFFECT(EF_POISON_SEED2,EF_POISON_SEED);
		
		//...........................................................................................................
		// �� ��
		//...........................................................................................................
		MAKE_EFFECT(EF_SELF_SHOUT,EF_SELF_SHOUT);
		
		//...........................................................................................................
		// �� ��
		//...........................................................................................................
		MAKE_EFFECT(EF_HIDE,EF_HIDE);
		
		//...........................................................................................................
		// �Ͽ���
		//...........................................................................................................
		MAKE_EFFECT(EF_MAYUNSAL,EF_MAYUNSAL);
		
		//...........................................................................................................
		// ����
		//...........................................................................................................
		MAKE_EFFECT(EF_SCAPMPER,EF_SCAPMPER);
		
		//...........................................................................................................
		// ����
		//...........................................................................................................
		MAKE_EFFECT(EF_MOMENT_HIDE,EF_MOMENT_HIDE);
		
		//...........................................................................................................
		// �˱�
		//...........................................................................................................
		MAKE_EFFECT(EF_SWORD_GHOST,EF_SRC_LINK);
		
		//...........................................................................................................
		// �޾�ġ��
		//...........................................................................................................
		MAKE_EFFECT(EF_BOUND_ATTACK,EF_SRC_LINK);
		
		//..............................................................................................................
		// ������ ��
		//..............................................................................................................
		MAKE_EFFECT(EF_BOW_RAIN,EF_BOW_RAIN);
		
		//..............................................................................................................
		// �� ��
		//..............................................................................................................
		MAKE_EFFECT(EF_SNIPER_BOW,EF_SNIPER_BOW);
		
		//..............................................................................................................
		// ġ���� ��
		//..............................................................................................................
		MAKE_EFFECT(EF_HEAL_RAIN_BOW,EF_HEAL_RAIN_BOW);
		
		//..............................................................................................................
		// ħ���� ȭ��
		//..............................................................................................................
		MAKE_EFFECT(EF_WHISPER_BOW,EF_WHISPER_BOW);
		
		//..............................................................................................................
		// ��ȭ�� ȭ��
		//..............................................................................................................
		MAKE_EFFECT(EF_CLEAR_BOW,EF_CLEAR_BOW);
		
		//..............................................................................................................
		// ������ ȭ��
		//..............................................................................................................
		MAKE_EFFECT(EF_CANCEL_BOW,EF_CANCEL_BOW);
		
		//..............................................................................................................
		// ������ ȭ��
		//..............................................................................................................
		MAKE_EFFECT(EF_COMBO_ADD_BOW,EF_COMBO_ADD_BOW);
		
		//..............................................................................................................
		// ���̽� ����
		//..............................................................................................................
		MAKE_EFFECT(EF_ICE_RAIN,EF_ICE_RAIN);
		MAKE_EFFECT(EF_ICE_RAINED,EF_ICE_RAINED);
		
		//..............................................................................................................
		// Fire ���� 
		//..............................................................................................................
		MAKE_EFFECT(EF_FIRE_RAIN, EF_FIRE_RAIN );
		MAKE_SKILL_SPOT_SRC(EF_FIRE_RAINED,EF_FIREBALL_ATTACKED0,ESLT_CHARPOS,5.1f);	
		
		//..............................................................................................................
		// �⿭��
		//..............................................................................................................
		MAKE_EFFECT(EF_HAND_PAR,EF_HAND_PAR);
		
		//..............................................................................................................
		// ������ ����
		//..............................................................................................................
		MAKE_EFFECT(EF_FRAME_STORM,EF_FRAME_STORM);
		
		//..............................................................................................................
		// ����Ʈ�� ���Ǿ�
		//..............................................................................................................
		MAKE_EFFECT(EF_LIGHTNIG_SPIRE0,EF_LIGHTNIG_SPIRE);
		MAKE_EFFECT(EF_LIGHTNIG_SPIRE1,EF_LIGHTNIG_SPIRE);
		MAKE_EFFECT(EF_LIGHTNIG_SPIRE2,EF_LIGHTNIG_SPIRE);
		
		//..............................................................................................................
		// Ÿ�̴� ���̺�
		//..............................................................................................................
		MAKE_EFFECT(EF_TYNEE_WAVE,EF_TYNEE_WAVE);
		
		//..............................................................................................................
		// ��������
		//..............................................................................................................
		MAKE_EFFECT(EF_BIG_PRESS,EF_BIG_PRESS);
		
		//..............................................................................................................
		// �� ����
		//..............................................................................................................
		MAKE_EFFECT(EF_SON_BURSH,EF_SON_BURSH);
		
		//..............................................................................................................
		// �ڷ���Ʈ
		//..............................................................................................................
		MAKE_EFFECT(EF_TELEPORT,EF_TELEPORT);
		
		//..............................................................................................................
		// ����
		//..............................................................................................................
		MAKE_EFFECT(EF_SLEEP_STICK,EF_SLEEP_STICK);
		
		//..............................................................................................................
		// ���� ���۸�
		//..............................................................................................................
		MAKE_EFFECT(EF_MANA_BUFFERING,EF_MANA_BUFFERING);
		
		//..............................................................................................................
		// ����
		//..............................................................................................................
		MAKE_EFFECT(EF_RETURN_HOME,EF_RETURN_HOME);
		
		//..............................................................................................................
		// ���� ����
		//..............................................................................................................
		MAKE_EFFECT(EF_SELF_HEAL,EF_SELF_HEAL);
		
		//..............................................................................................................
		// ���� ���
		//..............................................................................................................
		MAKE_EFFECT(EF_DISPELL_BAD,EF_DISPELL_BAD);
		
		//..............................................................................................................
		// ����
		//..............................................................................................................
		MAKE_EFFECT(EF_BRESS,EF_BRESS);
		MAKE_EFFECT(EF_BRESS_ED,EF_SRC_LINK);		// ���� ���ϴ� �� �ҽ��ʿ� �޾��ֱ�
		
		//..............................................................................................................
		// ����Ʈ
		//..............................................................................................................
		MAKE_EFFECT(EF_DETECT,EF_DETECT);
		
		//..............................................................................................................
		// ���潺 �ǵ�
		//..............................................................................................................
		MAKE_EFFECT(EF_DEPENCE_SHILED,EF_DEPENCE_SHILED);
		MAKE_EFFECT(EF_DEPENCE_SHILED_ED,EF_SRC_LINK);
		
		//..............................................................................................................
		// ���潺 ���ζ�
		//..............................................................................................................
		MAKE_EFFECT(EF_DEFENSE_AURA,EF_DEFENSE_AURA);
		MAKE_EFFECT(EF_DEFENSE_AURA_ED,EF_SRC_LINK);
		
		//..............................................................................................................
		// Ȧ�� ����Ʈ
		//..............................................................................................................
		MAKE_EFFECT(EF_HOLY_LIGHT,EF_HOLY_LIGHT);
		
		//..............................................................................................................
		// ������ ��
		//..............................................................................................................
		MAKE_EFFECT(EF_UNCOMBO,EF_UNCOMBO);
		
		//..............................................................................................................
		// ��Ȥ
		//..............................................................................................................
		MAKE_EFFECT(EF_FAINT_SELF,EF_SRC_LINK);
		
		//..............................................................................................................
		// �Ѱ赹��
		//..............................................................................................................
		MAKE_EFFECT(EF_LIMIT_RUSH,EF_SRC_LINK);
		
		//..............................................................................................................
		// �н�
		//..............................................................................................................
		MAKE_EFFECT(EF_SELF_COPY,EF_SELF_COPY);
		
		//..............................................................................................................
		// ������
		//..............................................................................................................
		MAKE_EFFECT(EF_ENERGY0,EF_ENERGY);
		MAKE_EFFECT(EF_ENERGY1,EF_ENERGY);
		MAKE_EFFECT(EF_ENERGY2,EF_ENERGY);
		
		//..............................................................................................................
		// ��ǳ����
		//..............................................................................................................
		MAKE_EFFECT(EF_CIRCLE_KICK,EF_CIRCLE_KICK);
		
		//..............................................................................................................
		// ġ���� ��
		//..............................................................................................................
		MAKE_EFFECT(EF_HEAL_LIGHT,EF_SRC_LINK);
		MAKE_EFFECT(EF_HEAL_LIGHT_ED,EF_SRC_LINK);
		
		//..............................................................................................................
		// �ǵ�����
		//..............................................................................................................
		MAKE_EFFECT(EF_CROSS_COUNTOR,EF_SRC_LINK);
		
		//..............................................................................................................
		// NPC ���� ������ ������ ��~ ( ���� �� )
		//..............................................................................................................
		MAKE_EFFECT(EF_BOX_DIE,EF_SRC_LINK);
		
		//..............................................................................................................
		// ����
		//..............................................................................................................
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_SHOUT,EF_SHOUT,ESLT_CHARPOS,2.1f,EF_SHOUTED,ESLT_CHARPOS,2.1f);
		//..............................................................................................................
		// ������ ��Ÿ
		//..............................................................................................................
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_STRONG_STICK,EF_STRONG_STICK,ESLT_CHARPOS,2.1f,EF_STRONG_STICKED,ESLT_CHARPOS,2.1f);
		//..............................................................................................................
		// �����ؼ� �ٴڿ� ��������
		//..............................................................................................................
		MAKE_SKILL_SPOT_SRC_AND_LIST(EF_HIT_LAND,EF_HIT_LAND,ESLT_CHARPOS,2.1f,EF_HIT_LANDED,ESLT_CHARPOS,2.1f);
		//..............................................................................................................
		// ���ʱ�
		//..............................................................................................................
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_NPC_ENTANGLE,EF_NPC_ENTANGLE,ESLT_CHARPOS,2.1f,EF_NPC_ENTANGLEED,ESLT_CHARPOS,5.1f);
		//..............................................................................................................
		// ����Ʈ��Ʈ�� ����
		//..............................................................................................................
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_NPC_REDTRENT,EF_NPC_REDTRENT,ESLT_CHARPOS,2.1f,EF_NPC_REDTRENT_ED,ESLT_CHARPOS,2.1f);
		//..............................................................................................................
		// ��ȭ
		//..............................................................................................................
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_NPC_STONY,EF_NPC_STONY,ESLT_CHARPOS,2.1f,EF_NPC_STONYED,ESLT_CHARPOS,2.1f);
		//..............................................................................................................
		// ����ȭ
		//..............................................................................................................
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_NPC_TREE,EF_NPC_TREE,ESLT_CHARPOS,2.1f,EF_NPC_TREEED,ESLT_CHARPOS,2.1f);
		//..............................................................................................................
		// ������ ��Ʈ����ũ
		//..............................................................................................................
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_FRAME_STRIKE,EF_FRAME_STRIKE,ESLT_CHARPOS,2.1f,EF_FRAME_STRIKE_ED,ESLT_CHARPOS,6.1f);
		//..............................................................................................................
		// ����ȭ
		//..............................................................................................................
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_NEUTRALIZE,EF_NEUTRALIZE,ESLT_CHARPOS,2.1f,EF_NEUTRALIZE_ED,ESLT_CHARPOS,2.1f);
		//..............................................................................................................
		// ��������� �帱ó�� ����
		//..............................................................................................................
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_HAND_DRILL,EF_HAND_DRILL,ESLT_CHARPOS,2.1f,EF_HAND_DRILLED,ESLT_CHARPOS,2.1f);
		//..............................................................................................................
		// �޺��� ����
		//..............................................................................................................
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_MOON_CURSE,EF_MOON_CURSE,ESLT_CHARPOS,2.1f,EF_MOON_CURSE_ED,ESLT_CHARPOS,2.1f);
		//..............................................................................................................
		// ���� ����
		//..............................................................................................................
		MAKE_SKILL_SPOT_SRC_AND_LIST(EF_LAND_CURSE,EF_LAND_CURSE,ESLT_CHARPOS,2.1f,EF_LAND_CURSE_ED,ESLT_CHARPOS,2.1f);
		//..............................................................................................................
		// ������ ��ȿ
		//..............................................................................................................
		MAKE_SKILL_SPOT_SRC_AND_LIST(EF_ROAR,EF_ROAR,ESLT_CHARPOS,2.1f,EF_ROARED,ESLT_CHARPOS,2.1f);
		//..............................................................................................................
		// ���⸦ ���� �ȾƼ� ����
		//..............................................................................................................
		MAKE_SKILL_SPOT_SRC_AND_LIST(EF_LAND_PUT_STICK,EF_LAND_PUT_STICK,ESLT_CHARPOS,2.1f,EF_LAND_PUT_STICKED,ESLT_CHARPOS,4.1f);
		//..............................................................................................................
		// ü�� ����Ʈ��
		//..............................................................................................................
		MAKE_SKILL_SPOT_SRC_AND_LIST(EF_CHAIN_LIGHTNING,EF_CHAIN_LIGHTNING,ESLT_CHARPOS,2.1f,EF_CHAIN_LIGHTNING_ED,ESLT_CHARPOS,4.1f);
		//..............................................................................................................
		// ������ 0
		//..............................................................................................................
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_NPC_SOUND_PAR0,EF_NPC_SOUND_PAR0,ESLT_CHARPOS,2.1f,EF_NPC_SOUND_PAR0_ED,ESLT_CHARPOS,2.1f);
		//..............................................................................................................
		// ������ 1
		//..............................................................................................................
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_NPC_SOUND_PAR1,EF_NPC_SOUND_PAR1,ESLT_CHARPOS,2.1f,EF_NPC_SOUND_PAR1_ED,ESLT_CHARPOS,2.1f);
		//..............................................................................................................
		// ������ 2
		//..............................................................................................................
		MAKE_SKILL_SPOT_SRC_AND_LIST(EF_NPC_SOUND_PAR2,EF_NPC_SOUND_PAR2,ESLT_CHARPOS,2.1f,EF_NPC_SOUND_PAR2_ED,ESLT_CHARPOS,2.1f);
		//..............................................................................................................
		// �߻��� ��ȿ
		//..............................................................................................................
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_NPC_NATURAL_CRY,EF_NPC_NATURAL_CRY,ESLT_CHARPOS,2.1f,EF_NPC_NATURAL_CRYED,ESLT_CHARPOS,2.1f);
		//..............................................................................................................
		// �г��� �ϰ�
		//..............................................................................................................
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_NPC_ONE_CUT,EF_NPC_ONE_CUT,ESLT_CHARPOS,2.1f,EF_NPC_ONE_CUTED,ESLT_CHARPOS,2.1f);
		//..............................................................................................................
		// ��ġ �ϰ�
		//..............................................................................................................
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_NPC_HAMER_CRASH,EF_NPC_HAMER_CRASH,ESLT_CHARPOS,2.1f,EF_NPC_HAMER_CRASH_ED,ESLT_CHARPOS,2.1f);
		//..............................................................................................................
		// �縯���� ����
		//..............................................................................................................
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_PEL_CURSE,EF_PEL_CURSE,ESLT_CHARPOS,2.1f,EF_PEL_CURSE_ED,ESLT_CHARPOS,2.1f);
		//..............................................................................................................
		// ���� ���
		//..............................................................................................................
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_ELECT_CRASH,EF_ELECT_CRASH,ESLT_CHARPOS,2.1f,EF_ELECT_CRASH_ED,ESLT_CHARPOS,2.1f);
		//..............................................................................................................
		// ��ȿ - �ɷ»��(�ڱ����� �Ŵ°�)
		//..............................................................................................................
		MAKE_SKILL_SPOT_SRC(EF_NPC_CRY_UP,EF_NPC_CRY_UP,ESLT_CHARPOS,2.1f);
		//..............................................................................................................
		// ������ ��� �ɱ�
		//..............................................................................................................
		MAKE_SKILL_SPOT_SRC_AND_LIST(EF_NPC_JUMP_ATTACK,EF_NPC_JUMP_ATTACK,ESLT_CHARPOS,2.1f,EF_NPC_JUMP_ATTACKED,ESLT_CHARPOS,2.1f);
		//..............................................................................................................
		// ��մ�����
		//..............................................................................................................
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_THROW_STICK,EF_THROW_STICK,ESLT_CHARPOS,2.1f,EF_THROW_STICKED,ESLT_CHARPOS,4.1f);
		//..............................................................................................................
		// ���� ����~(��)
		//..............................................................................................................
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_NPC_UPPER_CUT,EF_NPC_UPPER_CUT,ESLT_CHARPOS,2.1f,EF_NPC_UPPER_CUTED,ESLT_CHARPOS,2.1f);
		//..............................................................................................................
		// �߰�ȭ ( �ڱ����� )
		//..............................................................................................................
		MAKE_SKILL_SPOT_SRC(EF_NPC_STRONY,EF_NPC_STRONY,ESLT_CHARPOS,2.1f);
		//..............................................................................................................
		// �����¼�(�ڱ�����)
		//..............................................................................................................
		MAKE_SKILL_SPOT_SRC(EF_FIGHT_STAND,EF_FIGHT_STAND,ESLT_CHARPOS,2.1f);
		//..............................................................................................................
		// ���� ������
		//..............................................................................................................
		MAKE_SKILL_SPOT_SRC_AND_LIST(EF_LAND_SLEEP,EF_LAND_SLEEP,ESLT_CHARPOS,2.1f,EF_LAND_SLEEP_ED,ESLT_CHARPOS,2.1f);
		//..............................................................................................................
		// ������ �뷡
		//..............................................................................................................
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_FROG_SONG,EF_FROG_SONG,ESLT_CHARPOS,2.1f,EF_FROG_SONG_ED,ESLT_CHARPOS,2.1f);
		//..............................................................................................................
		// 3�� ���Ӻ���
		//..............................................................................................................
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_NPC_SKILL_CUT,EF_NPC_SKILL_CUT,ESLT_CHARPOS,2.1f,EF_NPC_SKILL_CUT_ED,ESLT_CHARPOS,3.1f);
		//..............................................................................................................
		// ���� �� �ٴ� �����ļ� ���� ����
		//..............................................................................................................
		MAKE_SKILL_SPOT_SRC_AND_LIST(EF_JUMP_HIT_HAND_LAND,EF_JUMP_HIT_HAND_LAND,ESLT_CHARPOS,2.1f,EF_JUMP_HIT_HAND_LANDED,ESLT_CHARPOS,2.1f);
		//..............................................................................................................
		// ���� ������ 3�� ���
		//..............................................................................................................
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_NPC_TWO_HIT,EF_NPC_TWO_HIT,ESLT_CHARPOS,2.1f,EF_NPC_TWO_HITED,ESLT_CHARPOS,2.1f);
		//..............................................................................................................
		// ���� ��
		//..............................................................................................................
		MAKE_EFFECT(EF_HEAL_BUBBLE,EF_HEAL_BUBBLE);						
		//..............................................................................................................
		// �չ�� ���� ( NPC )
		//..............................................................................................................
		MAKE_EFFECT(EF_BIG_BUBBLE,EF_BIG_BUBBLE);
		//..............................................................................................................
		// SP ���
		//..............................................................................................................
		MAKE_EFFECT(EF_SP_SUCTION,EF_SP_SUCTION);
		//..............................................................................................................
		// HP ���
		//..............................................................................................................
		MAKE_EFFECT(EF_HP_SUCTION,EF_HP_SUCTION);
		//..............................................................................................................
		// ������ �Ա�
		//..............................................................................................................
		MAKE_EFFECT(EF_ICE_RIP_BLOW,EF_ICE_RIP_BLOW);
		//..............................................................................................................
		// ��ȭ
		//..............................................................................................................
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_STONY_LINE,EF_STONY_LINE,ESLT_CHARPOS,3.1f,EF_STONY_LINE_ED,ESLT_CHARPOS,3.1f);
		//..............................................................................................................
		// ���̺�
		//..............................................................................................................
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_EYE_BEAM,EF_EYE_BEAM,ESLT_CHARPOS,3.1f,EF_EYE_BEAMED,ESLT_CHARPOS,3.1f);
		//..............................................................................................................
		// ���̺� ( 360 )
		//..............................................................................................................
		MAKE_SKILL_SPOT_SRC_AND_LIST(EF_EYE_BEAM_LAND,EF_EYE_BEAM_LAND,ESLT_CHARPOS,3.1f,EF_EYE_BEAM_LAND_ED,ESLT_CHARPOS,3.1f);
		//..............................................................................................................
		// ����
		//..............................................................................................................
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_NPC_AB_HPMP,EF_NPC_AB_HPMP,ESLT_CHARPOS,3.1f,EF_NPC_AB_HPMP_ED,ESLT_CHARPOS,3.1f);
		//..............................................................................................................
		// ����ȭ
		//..............................................................................................................
		MAKE_EFFECT(EF_MAGIC_DELETE,EF_MAGIC_DELETE);
		
		//..............................................................................................................
		// ��� ����
		//..............................................................................................................
		MAKE_EFFECT(EF_BIG_SWORD,EF_BIG_SWORD);
		MAKE_EFFECT(EF_BIG_SWORD0_ED,EF_BIG_SWORD0_ED);
		MAKE_EFFECT(EF_BIG_SWORD1_ED,EF_BIG_SWORD1_ED);
		MAKE_EFFECT(EF_BIG_SWORD2_ED,EF_BIG_SWORD2_ED);
		
		//..............................................................................................................
		// ���� ��ų ( ����Ʈ )
		//..............................................................................................................
		MAKE_EFFECT(EF_MIRACLE,EF_MIRACLE);
		MAKE_EFFECT(EF_MIRACLE_ED,EF_SRC_LINK);
		
		//..............................................................................................................
		// �Ŵ���� �⺻ ����
		//..............................................................................................................
		MAKE_EFFECT(EF_BIG_NPC_ATTACK0,EF_BIG_NPC_ATTACK0);			// �⺻ ����
		MAKE_EFFECT(EF_BIG_NPC_ATTACK0ED,EF_BIG_NPC_ATTACK0ED);		// �⺻ ���� ������
		
		//..............................................................................................................
		// �Ŵ���� ��Ÿ����
		//..............................................................................................................
		MAKE_EFFECT(EF_BIG_NPC_APPEAR0,EF_BIG_NPC_APPEAR0);			// �󱼿� �ٿ����ƿ�
		
		//..............................................................................................................
		// �Ŵ� ���� ��ų ���� 
		//..............................................................................................................
		MAKE_EFFECT(EF_BIG_NPC_SKILL0,EF_BIG_NPC_SKILL0);			// ��
		MAKE_EFFECT(EF_BIG_NPC_SKILL1,EF_BIG_NPC_SKILL0);			// �ǹ�
        
		//..............................................................................................................
		// ���� �׽�Ʈ
		//..............................................................................................................
		MAKE_EFFECT(EF_WING0,EF_WING0);								// ���� �׽�Ʈ
		
		//..............................................................................................................
		// ���
		//..............................................................................................................
		MAKE_EFFECT(EF_EMBLEM_FLAG,EF_EMBLEM_FLAG);
		
		//..............................................................................................................
		// ���� ����
		//..............................................................................................................
		MAKE_EFFECT(EF_BLOOD_RUNAWAY1,EF_SRC_LINK);
		
		//..............................................................................................................
		// �ұ��� ����
		//..............................................................................................................
		MAKE_EFFECT(EF_LIVE_MIND1,EF_SRC_LINK);
		
		//..............................................................................................................
		// �Ҳ�
		//..............................................................................................................
		MAKE_EFFECT(EF_FIREFLOWER0,EF_FIREFLOWER0);
		MAKE_EFFECT(EF_FIREFLOWER1,EF_FIREFLOWER0);
		
		MAKE_EFFECT(EF_SI_ITEM_ATTACK0,EF_SI_ITEM_ATTACK0); // �и�ġ ����
		MAKE_EFFECT(EF_SI_ITEM_ATTACK1,EF_SI_ITEM_ATTACK1); // ������ ����
		MAKE_EFFECT(EF_SI_ITEM_ATTACK2,EF_SI_ITEM_ATTACK2); // �ϸ��� ����
		MAKE_EFFECT(EF_SI_ITEM_ATTACK3,EF_SI_ITEM_ATTACK3); // ����� ����
		
		MAKE_EFFECT(EF_SI_ITEM_ATTACKED0,EF_SI_ITEM_ATTACKED0); // �и�ġ ����
		MAKE_EFFECT(EF_SI_ITEM_ATTACKED1,EF_SI_ITEM_ATTACKED1); // ������ ����
		MAKE_EFFECT(EF_SI_ITEM_ATTACKED2,EF_SI_ITEM_ATTACKED2); // �ϸ��� ����
		MAKE_EFFECT(EF_SI_ITEM_ATTACKED3,EF_SI_ITEM_ATTACKED3); // ����� ����
		
		//..............................................................................................................
		// �ż� ����
		//..............................................................................................................
		//MAKE_EFFECT(EF_HOLY_APPEAR,EF_HOLY_APPEAR);
		//MAKE_EFFECT_SET_SRC_SPOT(EF_HOLY_APPEAR,EF_HOLY_APPEAR,ESLT_CHARPOS,5.0f);
		//MAKE_EFFECT_SET_SRC_SPOT(EF_HOLY_DISAPPEAR,EF_HOLY_DISAPPEAR,ESLT_CHARPOS,5.0f);
		//MAKE_EFFECT_SET_SRC_SPOT(EF_HOLY_LEVEL_UP,EF_HOLY_LEVEL_UP,ESLT_CHARPOS,5.0f);
		
		MAKE_EFFECT(EF_HOLY_AURA,EF_HOLY_AURA);
		
		MAKE_EFFECT(EF_HOLY_AURAED0,EF_SRC_LINK);
		MAKE_EFFECT(EF_HOLY_AURAED1,EF_SRC_LINK);
		MAKE_EFFECT(EF_HOLY_AURAED2,EF_SRC_LINK);
		MAKE_EFFECT(EF_HOLY_AURAED3,EF_SRC_LINK);
		
		MAKE_EFFECT(EF_HOLY_CREATE00,EF_SRC_LINK);
		MAKE_EFFECT(EF_HOLY_CREATE01,EF_SRC_LINK);
		MAKE_EFFECT(EF_HOLY_CREATE02,EF_SRC_LINK);
		MAKE_EFFECT(EF_HOLY_CREATE03,EF_SRC_LINK);
		MAKE_EFFECT(EF_HOLY_CREATE04,EF_SRC_LINK);
		
		MAKE_EFFECT(EF_HOLY_CREATE10,EF_SRC_LINK);
		MAKE_EFFECT(EF_HOLY_CREATE11,EF_SRC_LINK);
		MAKE_EFFECT(EF_HOLY_CREATE12,EF_SRC_LINK);
		MAKE_EFFECT(EF_HOLY_CREATE13,EF_SRC_LINK);
		MAKE_EFFECT(EF_HOLY_CREATE14,EF_SRC_LINK);
		
		MAKE_EFFECT(EF_HOLY_CREATE20,EF_SRC_LINK);
		MAKE_EFFECT(EF_HOLY_CREATE21,EF_SRC_LINK);
		MAKE_EFFECT(EF_HOLY_CREATE22,EF_SRC_LINK);
		MAKE_EFFECT(EF_HOLY_CREATE23,EF_SRC_LINK);
		MAKE_EFFECT(EF_HOLY_CREATE24,EF_SRC_LINK);
		
		MAKE_EFFECT(EF_HOLY_CREATE30,EF_SRC_LINK);
		MAKE_EFFECT(EF_HOLY_CREATE31,EF_SRC_LINK);
		MAKE_EFFECT(EF_HOLY_CREATE32,EF_SRC_LINK);
		MAKE_EFFECT(EF_HOLY_CREATE33,EF_SRC_LINK);
		MAKE_EFFECT(EF_HOLY_CREATE34,EF_SRC_LINK);
		
		MAKE_EFFECT(EF_HOLY_CREATE40,EF_SRC_LINK);
		MAKE_EFFECT(EF_HOLY_CREATE41,EF_SRC_LINK);
		MAKE_EFFECT(EF_HOLY_CREATE42,EF_SRC_LINK);
		MAKE_EFFECT(EF_HOLY_CREATE43,EF_SRC_LINK);
		MAKE_EFFECT(EF_HOLY_CREATE44,EF_SRC_LINK);
		
		//..............................................................................................................
		// ��� NPC
		//..............................................................................................................
		MAKE_STAND_ATTACK_DEST(EF_MN_GUAD_ATTACK0,-1,0,0,EF_MN_GUAD_ATTACK0,ESLT_CHARPOS,5.0f);
		
		MAKE_EFFECT_SET_SRC_SPOT(EF_MN_GUAD_ATTACKED00,EF_MN_GUAD_ATTACKED00,ESLT_CHARPOS,5.0f);
		MAKE_EFFECT_SET_SRC_SPOT(EF_MN_GUAD_ATTACKED01,EF_MN_GUAD_ATTACKED01,ESLT_CHARPOS,5.0f);
		
		MAKE_STAND_ATTACK_DEST(EF_RI_GUAD_ATTACK0,-1,0,0,EF_RI_GUAD_ATTACK0,ESLT_CHARPOS,5.0f);
		
		MAKE_EFFECT_SET_SRC_SPOT(EF_RI_GUAD_ATTACKED00,EF_RI_GUAD_ATTACKED00,ESLT_CHARPOS,5.0f);
		MAKE_EFFECT_SET_SRC_SPOT(EF_RI_GUAD_ATTACKED01,EF_RI_GUAD_ATTACKED01,ESLT_CHARPOS,5.0f);
		
		MAKE_SKILL_FLY(EF_MIN_GUAD_SKILL,
			21728,CEFCommonFlySkill::CommonFlySkill_Effect_Type::BILLBOARD_LINK,
			25710,CEFCommonFlySkill::CommonFlySkill_Effect_Type::PARTICLE_SPOT,
			EF_MIN_GUAD_SKILLED,70,5);
		
		MAKE_SKILL_FLY(EF_RI_GUAD_SKILL,
			21728,CEFCommonFlySkill::CommonFlySkill_Effect_Type::BILLBOARD_LINK,
			25710,CEFCommonFlySkill::CommonFlySkill_Effect_Type::PARTICLE_SPOT,
			EF_RI_GUAD_SKILLED,70,5);
		
		
		MAKE_STAND_ATTACK_DEST(EF_LAND_PUT_STICKED,-1,0,0,EF_LAND_PUT_STICKED,0,4.1f);
		MAKE_STAND_ATTACK_DEST(EF_HIT_LANDED,-1,0,0,EF_HIT_LANDED,0,2.1f);
		MAKE_STAND_ATTACK_DEST(EF_CIRCLE_KICK_ED,-1,0,0,EF_CIRCLE_KICK_ED,0,4.1f);
		MAKE_STAND_ATTACK_DEST(EF_SWORD_FLASHED2,-1,0,0,EF_SWORD_FLASHED2,0,4.1f);
		MAKE_STAND_ATTACK_DEST(EF_AXE_LAND_HITED,-1,0,0,EF_AXE_LAND_HITED,0,3.1f);
		

		MAKE_ATTACK_FLY(EF_NPC_FLY_ATTACK0,
			PT_FIREBALL,CEFCommonFlySkill::CommonFlySkill_Effect_Type::PARTICLE_LINK,
			PT_FIREBALL_TAIL,CEFCommonFlySkill::CommonFlySkill_Effect_Type::PARTICLE_SPOT,
			EF_FIREBALL_ATTACKED4,70,5);
		
		MAKE_ATTACK_FLY(EF_NPC_FLY_ATTACK1,
			PT_FIREBALL,CEFCommonFlySkill::CommonFlySkill_Effect_Type::PARTICLE_LINK,
			PT_FIREBALL_TAIL,CEFCommonFlySkill::CommonFlySkill_Effect_Type::PARTICLE_SPOT,
			EF_FIREBALL_ATTACKED4,70,4);
		
		MAKE_EFFECT(EF_NPC_FLY_HAUPAI_LR,EF_NPC_FLY_HAUPAI_LR);
		MAKE_EFFECT(EF_NPC_FLY_HAUPAI_RR,EF_NPC_FLY_HAUPAI_LR);
		
		//........................................................................................................
		// NPC ��ų ( ������ )
		//........................................................................................................
		// �׷ν�ĭ ���� �߻�
		MAKE_SKILL_FLY(EF_NPC_NERO_GAS,
			324,CEFCommonFlySkill::CommonFlySkill_Effect_Type::BILLBOARD_LINK,
			325,CEFCommonFlySkill::CommonFlySkill_Effect_Type::PARTICLE_SPOT,
			EF_NPC_NERO_GASED,70,1);
		
		// �׷ν�ĭ �߰�ȭ
		MAKE_SKILL_SPOT_SRC(EF_NPC_NERO_STRONG,EF_NPC_NERO_STRONGED,ESLT_CHARPOS,4.1f);
		
		// ����XP-900 �����¼�
		MAKE_SKILL_SPOT_SRC(EF_NPC_BIZ_AMOTION,EF_NPC_BIZ_AMOTIONED,ESLT_CHARPOS,4.1f);
		
		// �׷ν�ĭ ������ �ϰ�
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_NPC_NERO_DIE_PUNCH,EF_NPC_NERO_DIE_PUNCH,ESLT_CHARPOS,4.1f,EF_NPC_NERO_DIE_PUNCHED,ESLT_CHARPOS,4.1f);
		
		// �ϸ��Ǿ� ����ȭ
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_NPC_HARU_MU,EF_NPC_HARU_MU,ESLT_CHARPOS,4.1f,EF_NPC_HARU_MUED,ESLT_CHARPOS,4.1f);
		
		// �ϸ��Ǿ� HP ���
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_NPC_HARU_HP,EF_NPC_HARU_HP,ESLT_CHARPOS,4.1f,EF_NPC_HARU_HPED,ESLT_CHARPOS,4.1f);
		
		// �ϸ��Ǿ� ��Ƽ ����Ʈ��
		MAKE_SKILL_SPOT_SRC_AND_LIST(EF_NPC_HARU_LIGHT,EF_NPC_HARU_LIGHT,ESLT_CHARPOS,4.1f,EF_NPC_HARU_LIGHTED,ESLT_CHARPOS,4.1f);
		
		// ����XP-300 �������
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_NPC_BIZ_ELEC,EF_NPC_BIZ_ELEC,ESLT_CHARPOS,4.1f,EF_NPC_BIZ_ELECED,ESLT_CHARPOS,4.1f);
		
		// ��ī�� 3�� ����
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_NPC_BIS_THREE,EF_NPC_BIS_THREE,ESLT_CHARPOS,4.1f,EF_NPC_BIS_THREEED,ESLT_CHARPOS,4.1f);
		
		// ��ī�� ���� ����
		MAKE_SKILL_SPOT_SRC_AND_LIST(EF_NPC_BIS_JUMP_AT,EF_NPC_BIS_JUMP_AT,ESLT_CHARPOS,4.1f,EF_NPC_BIS_JUMP_ATED,ESLT_CHARPOS,4.1f);
		
		// ��ī�� ���� ����
		MAKE_SKILL_SPOT_SRC_AND_LIST(EF_NPC_BIS_LAND_AT,EF_NPC_BIS_LAND_AT,ESLT_CHARPOS,4.1f,EF_NPC_BIS_LAND_ATED,ESLT_CHARPOS,4.1f);
		
		// ����XP-610 ���� ��
		MAKE_EFFECT(EF_NPC_BIZ_BEAM,EF_NPC_BIZ_BEAM);
		
		MAKE_SPOT_EFFECT_DEST(EF_FIREBALL_ATTACKED4,EF_FIREBALL_ATTACKED4,ESLT_CHARPOS,4.1f,0.0f);
		
		// �ż��Ǵ��
		MAKE_SKILL_SPOT_SRC(EF_FAST_SWORD,EF_FAST_SWORD,ESLT_CHARPOS,4.1f);
		// ȸ��
		MAKE_SKILL_SPOT_SRC(EF_FAST_AWAY,EF_FAST_AWAY,ESLT_CHARPOS,4.1f);
		// �ż��� ȭ�� ( ���α׷����� ó�� )
		MAKE_EFFECT(EF_FAST_BOW,EF_FAST_BOW);
		
		MAKE_SPOT_EFFECT(EF_MN_POINT_DEAD,EF_MN_POINT_DEAD,ESLT_CHARPOS,3.1f,0.0f);
		MAKE_SPOT_EFFECT(EF_RN_POINT_EDAD,EF_RN_POINT_EDAD,ESLT_CHARPOS,3.1f,0.0f);
		
		// �׸��� �⺻����
		MAKE_EFFECT(EF_GRIZZL_BAS_ATT,EF_GRIZZL_BAS_ATT);
		MAKE_EFFECT(EF_F_GRIZZL_BAS_ATT,EF_GRIZZL_BAS_ATT);
		MAKE_EFFECT(EF_M_GRIZZL_BAS_ATT,EF_GRIZZL_BAS_ATT);	
		// �׸��� �⺻������ ��, ��
		MAKE_EFFECT(EF_GRIZZL_BAS_ATTED_0,EF_GRIZZL_BAS_ATTED_0);
		MAKE_EFFECT(EF_F_GRIZZL_BAS_ATTED_0,EF_GRIZZL_BAS_ATTED_0);
		MAKE_EFFECT(EF_M_GRIZZL_BAS_ATTED_0,EF_GRIZZL_BAS_ATTED_0);
		MAKE_EFFECT(EF_GRIZZL_BAS_ATTED_2,EF_GRIZZL_BAS_ATTED_2);
		MAKE_EFFECT(EF_F_GRIZZL_BAS_ATTED_2,EF_GRIZZL_BAS_ATTED_2);
		MAKE_EFFECT(EF_M_GRIZZL_BAS_ATTED_2,EF_GRIZZL_BAS_ATTED_2);
		
		// Ű�޶� ��Ʈ �⺻����
		MAKE_EFFECT(EF_CHIME_ANT_BAS_ATT,EF_CHIME_ANT_BAS_ATT);
		// Ű�޶� ��Ʈ �⺻������ ��, ��
		MAKE_EFFECT(EF_CHIME_ANT_BAS_ATTED_0,EF_CHIME_ANT_BAS_ATTED_0);
		MAKE_EFFECT(EF_CHIME_ANT_BAS_ATTED_2,EF_CHIME_ANT_BAS_ATTED_2);
		
		// �׸����� �뷡 
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_SONG_GRIZZL, EF_SONG_GRIZZL, ESLT_CHARPOS, 2.1f, EF_SONG_GRIZZL_ED, ESLT_CHARPOS, 2.1f);
		// �׸����� �ϰ�
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_HIT_GRIZZL, EF_HIT_GRIZZL, ESLT_CHARPOS, 2.1f, EF_HIT_GRIZZL_ED, ESLT_CHARPOS, 2.1f);
		// �׸����� ȸ��
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_RECOVER_GRIZZL, EF_RECOVER_GRIZZL, ESLT_CHARPOS, 2.1f, EF_RECOVER_GRIZZL_ED, ESLT_CHARPOS, 2.1f);
		// Ű�޶� ��Ʈ�� ���� ����
		MAKE_SKILL_SPOT_SRC_AND_LIST(EF_BIG_SHOCK_C_ANT,EF_BIG_SHOCK_C_ANT,ESLT_CHARPOS,4.1f,EF_BIG_SHOCK_C_ANT_ED,ESLT_CHARPOS,4.1f);
		
		MAKE_EFFECT(EF_DEFENSE_PET_BMOTION,EF_DEFENSE_PET_BMOTION);
		
		//������ ��Ÿ����
		MAKE_EFFECT(EF_POPOI_ATTACK_0, EF_POPOI_ATTACK);
		MAKE_EFFECT(EF_POPOI_ATTACK_1, EF_POPOI_ATTACK);
		MAKE_EFFECT(EF_POPOI_ATTACK_2, EF_POPOI_ATTACK);
		
		//���׺� ��1 - �Ϲ� ��	
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_EGGBIL_HILL0, -1, 0, 2.1f, EF_EGGBIL_HILLING, ESLT_CHARPOS, 2.1f);
		//���׺� ��2 - 3�ܰ������ ��
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_EGGBIL_HILL1, EF_EGGBIL_HILLMOTION , 0, 2.1f, EF_EGGBIL_HILLING, ESLT_CHARPOS, 2.1f);
		
		//�ǳ����� ���ɷ�
		MAKE_EFFECT(EF_PINAPOA_HEART,EF_RECOVERY_ITEM1_0);		
		
		// ȣ���� �⺻����
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_TIGER_ATTACKED_1, -1, 0, 2.1f, EF_TIGER_ATTACKED_1, ESLT_CHARPOS, 2.1f);
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_TIGER_ATTACKED_2, -1, 0, 2.1f, EF_TIGER_ATTACKED_2, ESLT_CHARPOS, 2.1f);
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_TIGER_ATTACKED_3, -1, 0, 2.1f, EF_TIGER_ATTACKED_3, ESLT_CHARPOS, 2.1f);
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_TIGER_ATTACKED_4, -1, 0, 2.1f, EF_TIGER_ATTACKED_4, ESLT_CHARPOS, 2.1f);
		
		// ȣ���� ��ų
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_TIGER_SKILL_2, -1, 0, 2.1f, EF_TIGER_SKILL_2, ESLT_CHARPOS, 2.1f);
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_TIGER_SKILL_3, -1, 0, 2.1f, EF_TIGER_SKILL_3, ESLT_CHARPOS, 2.1f);
		
		// ȣ���� �������� ���Ӱ��ݽ�ų
		MAKE_EFFECT(EF_TIGER_SKILL_4, EF_TIGET_ATTACK);
		
		//�縷���� �⺻���1
		MAKE_DEFENCE_PET_MOTION(EF_FOX_DEFENSED_1, 1);
		//�縷���� �⺻���2
		MAKE_DEFENCE_PET_MOTION(EF_FOX_DEFENSED_2, 1);
		//�縷���� �⺻���3
		MAKE_DEFENCE_PET_MOTION(EF_FOX_DEFENSED_3, 1);
		//�縷���� �⺻���4
		MAKE_DEFENCE_PET_MOTION(EF_FOX_DEFENSED_4, 1);
		
		//�縷���� ��ų2
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_FOX_SKILL_1, -1, 0, 2.1f, EF_FOX_SKILL_1, ESLT_CHARPOS, 2.1f);
		//�縷���� ��ų3
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_FOX_SKILL_2, -1, 0, 2.1f, EF_FOX_SKILL_2, ESLT_CHARPOS, 2.1f);
		//�縷���� ��ų4
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_FOX_SKILL_3, -1, 0, 2.1f, EF_FOX_SKILL_3, ESLT_CHARPOS, 2.1f);
		
		//���̽� �ʵ�
		MAKE_EFFECT(EF_ICE_FIELD,EF_ICE_FIELD);
		//���̽� ���Ǿ�
		MAKE_EFFECT(EF_ICE_SPEAR,EF_ICE_SPEAR);
		
		//����Ʈ ����
		MAKE_EFFECT(EF_LIGHT_STORM,EF_LIGHT_STORM);
		MAKE_SKILL_SPOT_SRC(EF_LIGHT_STORMED,EF_LIGHTNING_ATTACKED2,ESLT_CHARPOS,5.1f);
		
		//��� ��
		MAKE_EFFECT(EF_STONE_RAIN,EF_STONE_RAIN);
		MAKE_SKILL_SPOT_SRC(EF_STONE_RAINED,EF_STONE_RAINED,ESLT_CHARPOS,5.1f);
		
		//���� ����
		MAKE_EFFECT(EF_WIND_BUSH,EF_WIND_BUSH);
		MAKE_SKILL_SPOT_SRC(EF_WIND_BUSHED,EF_WIND_BUSHED,ESLT_CHARPOS,5.1f);
		
		//���� ���̾�	
		MAKE_EFFECT(EF_ROUND_FIRE,EF_ROUND_FIRE);
		//���� ����Ʈ	
		MAKE_EFFECT(EF_ROUND_LIGHT,EF_ROUND_LIGHT);
		//ĳ��Ʈ ����
		MAKE_EFFECT(EF_CAST_STONE,EF_CAST_STONE);
		
		// ������ �ӹ�
		MAKE_EFFECT(EF_EARTH_BIND,EF_EARTH_BIND);
		// ����
		MAKE_EFFECT(EF_RANDOM_ARROW,EF_RANDOM_ARROW);		
		
		//����� ����
		MAKE_SKILL_SPOT_SRC(EF_INDONESIA_EVENT,EF_INDONESIA_EVENT,ESLT_CHARPOS,2.1f);
		
		//�ż��� ��
		MAKE_EFFECT(EF_QUICK_SWORD,EF_CONANISKILL);
		//���� Ⱦ����
		MAKE_EFFECT(EF_CON_CROSSCUT,EF_CONANISKILL);
		
		//������ ����
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_PAIN_EARTH, -1, 0, 2.1f, EF_PAIN_EARTHED, ESLT_CHARPOS, 3.1f);
		
		//��ǳ��Ÿ
		MAKE_EFFECT(EF_FIRESTORM_SMASH,EF_FIRESTORM_SMASH);
		//�⿭��Ÿ
		MAKE_EFFECT(EF_SPIRIT_FIRE_SMACK,EF_SPIRIT_FIRE_SMACK);
		//ȸ������
		MAKE_EFFECT(EF_SPIN_FIRE_KICK,EF_CONANISKILL);
		
		///-- ��Ŭ ü����
		MAKE_SKILL_SPOT_SRC(EF_CIRCLE_CHANGE,EF_CIRCLE_SHIELD,ESLT_CHARPOS,3.1f);	
		///-- ����
		MAKE_EFFECT(EF_REVENGE_SHIELD,EF_SRC_LINK);
		
		///--�г��� ����
		MAKE_EFFECT(EF_WRATH_KNIFE_ATTACK,EF_WRATH_KNIFE_ATTACK);
		///--��Ŭũ����
		MAKE_EFFECT(EF_CIRCLE_CRASH,EF_CIRCLE_CRASH);
		///-- ������ ȭ��
		MAKE_BEELINEBOW(EF_PARALYSIS_BOW,PT_PARALYSIS_BOW_TAIL,EF_PARALYSIS_BOWED,70,3.1f,ESLT_CHARPOS);
		///-- ��ȭ��
		MAKE_BEELINEBOW(EF_POISON_BOW,PT_POISON_BOW_TAIL,EF_POISON_BOW,70,3.1f,ESLT_CPOS_ICAMERA);
		///-- �￬��
		MAKE_EFFECT(EF_TREE_CON_KICK,EF_CONANISKILL);
		///-- ������ �ٿ�
		MAKE_EFFECT(EF_SLASH_DOWN,EF_CONANISKILL);
		///-- ������ ��
		MAKE_EFFECT(EF_SLASH_UP,EF_CONANISKILL);
		///-- Ʈ���� ī��Ʈ
		MAKE_EFFECT(EF_TRIPLE_COUNT,EF_CONANISKILL);
		
		///-- ������ ȭ���
		MAKE_EFFECT(EF_ICE_BOW_RAIN,EF_ICE_BOW_RAIN);
		
		///-- ���� ���� 
		MAKE_EFFECT(EF_HIDE_SENSOR , EF_HIDE_SENSOR);
		
		//�г� ����
		MAKE_EFFECT(EF_WRATH_BOMB , EF_WRATH_BOMB );
		//�ż��� ����
		MAKE_EFFECT(EF_HOLY_BOMB_BALL , EF_HOLY_BOMB );	
		//����� ����
		MAKE_EFFECT(EF_DARK_BOMB_BALL , EF_DARK_BOMB );	
		//�尩 �ı� 
		MAKE_EFFECT(EF_GLOVE_DESTRUCTION ,EF_CONANISKILL);
		//��ũ�Ͻ� 
		MAKE_EFFECT(EF_ONE_DARKNESS,EF_ONE_DARKNESS);		
		//Ʈ���� ��ũ�Ͻ�
		MAKE_EFFECT(EF_TRIPLE_DARKNESS,EF_TRIPLE_DARKNESS);	
		//Ʈ���� ���̾�
		MAKE_EFFECT(EF_TRIPLE_FIRE,EF_TRIPLE_FIRE);
		
		///-- �Ŵ����� ���� ��ũ ����Ʈ
		MAKE_EFFECT(EF_COLONYCORE_L1,EF_GIANTUNIT_SRC_LINK);
		MAKE_EFFECT(EF_COLONYSUBCORE_L1,EF_GIANTUNIT_SRC_LINK);
		MAKE_EFFECT(EF_COLONYCORE_L2,EF_GIANTUNIT_SRC_LINK);
		MAKE_EFFECT(EF_COLONYSUBCORE_L2,EF_GIANTUNIT_SRC_LINK);
		
		///-- ����Ÿ�� ��������Ʈ
		MAKE_GTOWEREFFECT(EF_COLONYGTOWER_L1_SKILL, 1, 0, PT_FIREBALL_TAIL, EF_BOW_ATTACKED_2, ESLT_SRC_RAD);
		MAKE_GTOWEREFFECT(EF_COLONYGTOWER_L2_SKILL, 1, BB_GT_L2_BALL, PT_FIREBALL_TAIL, EF_COLONYGTOWER_L2_SKILLED, ESLT_CHARPOS);
		MAKE_GTOWEREFFECT(EF_COLONYGTOWER_L3_SKILL, 1, BB_GT_L3_BEAM, PT_VIOLET_TAIL , EF_COLONYGTOWER_L3_SKILLED, ESLT_CHARPOS);
		
		///-- �Ŵ���� ��������Ʈ
		MAKE_CANNONEFECT(EF_GUST_PAN_ATTACK, 126, 2, BB_CANNON_BALL, PT_FIREBALL_TAIL, 0, 0, EF_GUST_PAN_ATTACK_PERCUSSION, ESLT_CHARPOS, EF_GUST_PAN_ATTACK_EXPLOSION, ESLT_CHARPOS, EF_GUST_PAN_ATTACKED, ESLT_CHARPOS);
		MAKE_CANNONEFECT(EF_GUST_PAN_SKILL, 126, 2, BB_CANNON_BALL, PT_FIREBALL_TAIL, EF_GUST_PAN_GET_POWER, ESLT_CHARPOS, EF_GUST_PAN_SKILL_PERCUSSION, ESLT_CHARPOS, EF_GUST_PAN_SKILL_EXPLOSION, ESLT_CHARPOS, EF_GUST_PAN_SKILLKED, ESLT_CHARPOS);
		
		///-- �Žź� ���� ����Ʈ
		MAKE_GOLEMEFFECT(EF_GIANT_GOLEM_ATTACK, 99, 164, 0, 0, ESLT_CHARPOS, 0, ESLT_CHARPOS, EF_GIANT_GOLEM_ATTACK_EXPLOSION, ESLT_CHARPOS, EF_GIANT_GOLEM_ATTACKED, ESLT_CHARPOS);
		MAKE_GOLEMEFFECT(EF_GIANT_GOLEM_SKILL, 99, 164, PT_GGOLEM_SKILLHAND_TAIL1, EF_GIANT_GOLEM_RIGHT_HAND, ESLT_CHARPOS, EF_GIANT_GOLEM_LEFT_HAND, ESLT_CHARPOS, EF_GIANT_GOLEM_SKILL_EXPLOSION, ESLT_CHARPOS, EF_GIANT_GOLEM_SKILLED, ESLT_CHARPOS);
		
		///-- �ż��� ����
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_PAIN_HOLLY, -1, 0, 2.1f, EF_PAIN_HOLLYED, ESLT_CHARPOS, 3.1f);	
		
		///-- ������ �Ǽ�
		MAKE_EFFECT(EF_BUNCH_STRENGTH,EF_BUNCH_STRENGTH);
		
		//�� �Ӵ� ���˷� - �⺻���� ��������
		MAKE_STAND_ATTACK_DEST(EF_B_M_BASE_ATTACK,-1,0,0,-1,0,0);		
		//�� �Ӵ� ���� - ���
		MAKE_EFFECT(EF_B_M_SOLDIER_THRUST, EF_BONWARSKILL);
		//�� �Ӵ� ���� - �޼Һ���
		MAKE_EFFECT(EF_B_M_SOLDIER_WEAK_CUT, EF_BONWARSKILL);
		//�� �Ӵ� ���� - �񷯺���
		MAKE_EFFECT(EF_B_M_SOLDIER_THRUST_CUT, EF_BONWARSKILL);
		//�� �Ӵ� ���� - ȸ������
		MAKE_EFFECT(EF_B_M_WARRIOR_TURN_CUT, EF_BONWARSKILL);
		//�� �Ӵ� ���� - X����1
		MAKE_EFFECT(EF_B_M_WARRIOR_X1_CUT, EF_BONWARSKILL);
		//�� �Ӵ� ���� - X����2
		MAKE_EFFECT(EF_B_M_WARRIOR_X2_CUT, EF_BONWARSKILL);
		//�� �Ӵ� ������ - ��������
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_B_M_GUARD_RESURRECTION, EF_REVIVAL, 0, 3.1f, -1, 0, 3.1f);
		//�� �Ӵ� ���� - ȭ����ǳ
		MAKE_EFFECT(EF_B_M_BOSS_FIRE_STORM, EF_BONWARSKILL);
		
		//�� ��ó�� - �⺻���� ��������
		MAKE_EFFECT(EF_B_ARCHER_BASE_ATTACK, EF_BONARCSKILL);
		//��Ÿ���� �� ��ó - ��ȭ��
		MAKE_EFFECT(EF_B_ARCHER_FIRE_ARROW, EF_BONARCSKILL);		
		//�� ��ó ġ��� - ����ġ�� ����Ʈ
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_B_ARCHER_FIRST_AID, EF_B_ARCHER_FIRST_AID, 0, 3.1f, -1, 0, 3.1f);
		//������ ����ó - ������ ȭ��
		MAKE_EFFECT(EF_B_ARCHER_DECAY_ARROW, EF_BONARCSKILL);
		//�����ɸ� ����ó - ������ ȭ��
		MAKE_EFFECT(EF_B_ARCHER_PAIN_ARROW, EF_BONARCSKILL);
		//���ָ� ����ó - ������ ȭ��
		MAKE_EFFECT(EF_B_ARCHER_DARK_ARROW, EF_BONARCSKILL);
		//���ֹ��� ����ó - ������ ȭ��
		MAKE_EFFECT(EF_B_ARCHER_CURSE_ARROW, EF_BONARCSKILL);
		//������ ����ó - ������ ȭ��
		MAKE_EFFECT(EF_B_ARCHER_PARALYSIS_ARROW, EF_BONARCSKILL);
		//����ó ���� - ����
		MAKE_EFFECT(EF_B_BOS_ARCHER_RANDOM_ARROW, EF_BONARCSKILL);
		
		//�� ����� - ������ �˼�
		MAKE_EFFECT(EF_B_JAHE_CRAZY_FEN_BEFORE, EF_BONWARSKILL);
		//���ú��� - ������ǳ
		MAKE_EFFECT(EF_B_SKEL_BAIN_DARK_STORM, EF_BONWARSKILL);	
		
		// �ִ����� ����
		MAKE_EFFECT(EF_ANIWING0,EF_ANIWING0);		
		
		

		//
	//	MAKE_SKILL_SPOT_SRC_AND_DEST(EF_RUGAL_BASIC_ATTACK,EF_RUGAL_BASIC_ATTACK,ESLT_CHARPOS,2.1f,EF_RUGAL_BASIC_ATTACKED,ESLT_CHARPOS,2.1f);
	//	MAKE_SKILL_SPOT_SRC_AND_DEST(EF_RUGAL_CRI_ATTACK,EF_RUGAL_CRI_ATTACK,ESLT_CHARPOS,2.1f,EF_RUGAL_CRI_ATTACKED,ESLT_CHARPOS,2.1f);

		//�߱� - ���������� ǥ�� ����Ʈ
		MAKE_EFFECT(EF_BEFORE_ATTACKER_MARK, EF_SRC_LINK );		
		
		//��Ʈ��� �Ҳ�
		MAKE_EFFECT(EF_FIREHEARTFLOWER,EF_FIREFLOWER0);		
		//���̷����� �Ҳ�
		MAKE_EFFECT(EF_FIREENGLOVEFLOWER,EF_FIREFLOWER0);
		//����� �Ҳ�
		MAKE_EFFECT(EF_FIRENATLOVEFLOWER,EF_FIREFLOWER0);		
		
		//Ŭ���� ���������� - ���� ����Ʈ - ���ι̳���
		MAKE_EFFECT( EF_SPECIAL_ITEM_EFFECT_PROMINAS, EF_SPECIAL_ITEM_EFFECT );
		//Ŭ���� ���������� - ���� ����Ʈ - ����Ű
		MAKE_EFFECT( EF_SPECIAL_ITEM_EFFECT_PSYCHE, EF_SPECIAL_ITEM_EFFECT );
		//Ŭ���� ���������� - ���� ����Ʈ - �ӽóʸ�
		MAKE_EFFECT( EF_SPECIAL_ITEM_EFFECT_MERCENARY, EF_SPECIAL_ITEM_EFFECT );
		//Ŭ���� ���������� - ���� ����Ʈ - ����Ʈ
		MAKE_EFFECT( EF_SPECIAL_ITEM_EFFECT_KNIGHT, EF_SPECIAL_ITEM_EFFECT );
		//Ŭ���� ���������� - ���� ����Ʈ - �����̾�
		MAKE_EFFECT( EF_SPECIAL_ITEM_EFFECT_SLAYER, EF_SPECIAL_ITEM_EFFECT );
		//Ŭ���� ���������� - ���� ����Ʈ - ��������
		MAKE_EFFECT( EF_SPECIAL_ITEM_EFFECT_SNIPER, EF_SPECIAL_ITEM_EFFECT );
		//Ŭ���� ���������� - ���� ����Ʈ - ������Ʈ
		MAKE_EFFECT( EF_SPECIAL_ITEM_EFFECT_PRIEST, EF_SPECIAL_ITEM_EFFECT );
		//Ŭ���� ���������� - ���� ����Ʈ - Ȧ�����
		MAKE_EFFECT( EF_SPECIAL_ITEM_EFFECT_HOLY, EF_SPECIAL_ITEM_EFFECT );

		//Ŭ���� ���������� - ���� ����Ʈ - ���ι̳���
		MAKE_EFFECT( EF_BASE_ITEM_EFFECT_PROMINAS, EF_BASE_ITEM_EFFECT );
		MAKE_EFFECT( EF_BASE_ITEM_EFFECT_PSYCHE, EF_BASE_ITEM_EFFECT );
		MAKE_EFFECT( EF_BASE_ITEM_EFFECT_MERCENARY, EF_BASE_ITEM_EFFECT );
		MAKE_EFFECT( EF_BASE_ITEM_EFFECT_KNIGHT, EF_BASE_ITEM_EFFECT );
		MAKE_EFFECT( EF_BASE_ITEM_EFFECT_SLAYER, EF_BASE_ITEM_EFFECT );
		MAKE_EFFECT( EF_BASE_ITEM_EFFECT_SNIPER, EF_BASE_ITEM_EFFECT );
		MAKE_EFFECT( EF_BASE_ITEM_EFFECT_PRIEST, EF_BASE_ITEM_EFFECT );
		MAKE_EFFECT( EF_BASE_ITEM_EFFECT_HOLY, EF_BASE_ITEM_EFFECT );



		///���׸� ���� - 1�ܰ� �⺻����  
		MAKE_EFFECT( EF_DOGMA_L1_BASIC_ATTACK, EF_DOGMAEFFECT );	
		///���׸� ���� - 1�ܰ� ���̾��
		MAKE_EFFECT( EF_DOGMA_L1_FIRERAIN, EF_DOGMAEFFECT );
		///���׸� ���� - 1�ܰ� �����ӽ���
		MAKE_EFFECT( EF_DOGMA_L1_FRAME_STORM, EF_DOGMAEFFECT );

		///���׸� ���� - 2�ܰ� �⺻����  
		MAKE_EFFECT( EF_DOGMA_L2_BASIC_ATTACK, EF_DOGMAEFFECT );	
		///���׸� ���� - 2�ܰ� �г��� �ϰ�
		MAKE_EFFECT( EF_DOGMA_L2_WRATH_ONECUT, EF_DOGMAEFFECT );
		///���׸� ���� - 2�ܰ� ����
		MAKE_EFFECT( EF_DOGMA_L2_SWORD_FLASH, EF_DOGMAEFFECT );

		///���׸� ���� - 3�ܰ� �⺻����  
		MAKE_EFFECT( EF_DOGMA_L3_BASIC_ATTACK, EF_DOGMAEFFECT );	
		///���׸� ���� - 3�ܰ� �߱�����
		MAKE_EFFECT( EF_DOGMA_L3_FOOT_QUAKE, EF_DOGMAEFFECT );
		///���׸� ���� - 3�ܰ� �г��� �ϰ�2
		MAKE_EFFECT( EF_DOGMA_L3_WRATH_ONECUT2, EF_DOGMAEFFECT );

		///���׸� ���� - 4�ܰ� �����
		MAKE_EFFECT( EF_DOGMA_L4_CIRCLE_SWORD, EF_DOGMAEFFECT );

		///���׸� ���� - 4�ܰ� ���� ����Ʈ
		MAKE_EFFECT( EF_MASTER_DOGMA_APPEAR_EFFECT, EF_DOGMAEFFECT );		

	
		//�簥 �⺻ ���� 
		MAKE_EFFECT(EF_RUGAL_BASIC_ATTACK , EF_RUGALEFFECT );

		//�ִ�Ʈ �⺻���� 
		MAKE_EFFECT(EF_RUGALSOLDART_BASIC_ATTACK , EF_RUGALEFFECT );

		MAKE_EFFECT(EF_RUGAL_HOWLLING_1 , EF_RUGALEFFECT );
		MAKE_EFFECT(EF_RUGAL_CURSE_1 , EF_RUGALEFFECT );
		MAKE_EFFECT(EF_RUGAL_NAIL_OFDECAY , EF_RUGALEFFECT );
		MAKE_EFFECT(EF_RUGAL_ONECUTOFRUIN , EF_RUGALEFFECT );
		MAKE_EFFECT(EF_RUGAL_ILLUSIONTREE_1 , EF_RUGALEFFECT );
		MAKE_EFFECT(EF_RUGAL_RECALL_SOLTAR , EF_RUGALEFFECT );
		MAKE_EFFECT(EF_RUGAL_CLO_CYCLON , EF_RUGALEFFECT );
		MAKE_EFFECT(EF_RUGAL_DRIL_TEMPEST , EF_RUGALEFFECT );
		MAKE_EFFECT(EF_RUGAL_DRIL_HURRICANE , EF_RUGALEFFECT );	
			
		MAKE_EFFECT(EF_RUGAL_RECALL_SOLTARED , EF_RUGALEFFECT );
	



		///�ٶ�� - �⺻����
		MAKE_EFFECT( EF_BARAGOS_BASIC_ATTACK, EF_BARAGOSEFFECT );
		///�ٶ�� - �巡�� �극��(��)
		MAKE_EFFECT( EF_BARAGOS_DRAGON_BRESS, EF_BARAGOSEFFECT );
		///�ٶ�� - �Ƿ��� ��ȿ
		MAKE_EFFECT( EF_BARAGOS_HOWL_OF_DEVIL, EF_BARAGOSEFFECT );
		///�ٶ�� - ���� �극��ũ
		MAKE_EFFECT( EF_BARAGOS_THUNDERBREAK, EF_BARAGOSEFFECT );
		///�ٶ�� - ����ȭ
		MAKE_EFFECT( EF_BARAGOS_BERSERK, EF_BARAGOSEFFECT );

		///�𳪵� - �⺻����
		MAKE_EFFECT( EF_JANAD_BASIC_ATTACK, EF_JANADEFFECT );
		///�𳪵� - ���� 3�� ����
		MAKE_EFFECT( EF_JANAD_DASHTRIPLE_ATTACK, EF_JANADEFFECT );
		///�𳪵� - ���ɼ�ȯ
		MAKE_EFFECT( EF_JANAD_CALLDEVILSOUL, EF_JANADEFFECT );	
		///�𳪵� - ���� ��ǳ
		MAKE_EFFECT( EF_JANAD_DARKSWORDSTORM, EF_JANADEFFECT );
		///�𳪵� - ����
		MAKE_EFFECT( EF_JANAD_DISPEL, EF_JANADEFFECT );
		///�𳪵� - ���׸� ����Ʈ
		MAKE_EFFECT( EF_JANAD_MAGMA_BUST, EF_JANADEFFECT );
		///�𳪵� - ��Ȳ�� ����
		MAKE_EFFECT( EF_JANAD_SWORD_FLASH_OF_DEVIL, EF_JANADEFFECT );
		///�𳪵� - ��ȯ
		MAKE_EFFECT( EF_JANAD_CALLDEVILSOULED, EF_JANADEFFECT );


		//��κ��� �⺻���� 
		MAKE_EFFECT ( EF_UROBOROS_BASIC_ATTACK ,  EF_UROBOROSEFFECT );
		MAKE_EFFECT( EF_UROBOROS_POISON ,  EF_UROBOROSEFFECT );
		MAKE_EFFECT( EF_UROBOROS_BALKAN ,  EF_UROBOROSEFFECT );
		MAKE_SPOT_EFFECT_DEST(EF_UROBOROS_BALKAN_ED , EF_UROBOROS_BALKAN_ED , ESLT_CHARPOS ,3.1f , 0.0f );


		MAKE_EFFECT( EF_UROBOROS_DRAG_HOWLLING ,  EF_UROBOROSEFFECT );
		MAKE_EFFECT( EF_UROBOROS_RUN ,  EF_UROBOROSEFFECT );

		
		MAKE_EFFECT(EF_DONG_TEST_0 , EF_DONGS_TEST);
		//........................................................................................................
		// ��ų ����Ʈ ���� �ɷ�ġ�� ó��
		//........................................................................................................
		case EF_SKILL_ACTION:
			{
				if( !CheckCharacter( pSrc , pDest ) )
					return FALSE;
				
				if( NULL == pExtraInfo )
				{
					return FALSE;
				}
				
				EffectTargetHit( pSrc , pDest , pExtraInfo );
				
			}
			return TRUE;
			
		case EF_NPC_STAND_ATTACK:
			{
				if( CheckCharacter( pSrc , pDest ) == FALSE )
					return TRUE;
				
				if( NULL == pExtraInfo )
					return TRUE;
				
				
				StandTargetHit( pSrc , pDest , pExtraInfo );
			}
			return TRUE;
			
			
		default:
			{
				if( NULL != pExtraInfo )
				{
					DebugPrint( "!\r\n--- ����Ʈ ���� Ÿ�� ���� ---\r\n" );
				}
			}
			
			return FALSE;
	}
	
	return FALSE;
}

//................................................................................................................
// Delete Effect
//................................................................................................................

///--JAPAN_BUG_MODIFY	
int CEffectManager::Delete(EFFECT_ID id,Character *pSrc,Character *pDest, DWORD dSkillCode)
{
	std::list	<CEffectBase *> ::iterator	i = m_EffectList.begin();
	
	for( ; i != m_EffectList.end() ; ++i )
	{
		//........................................................................................................
		// ����Ʈ ����� 
		//........................................................................................................
		if( (*i)->GetID()       == id &&
			(*i)->GetSrcChar()  == pSrc &&
			(*i)->GetDestChar() == pDest &&
			(*i)->GetSkillCode() == dSkillCode )
		{
			///-- By simwoosung ���õ� ������ �����.
			(*i)->SetDestroy();			
		}
	}
	
	return FALSE;
}

//................................................................................................................
// ���� ��� ����Ʈ ���� ( ����Ʈ ó�� ) By wxywxy 
//................................................................................................................
int CEffectManager::PopLandEffectBase(CEffectBase **ppEffectBase)
{
	std::list	<CEffectBase *> ::iterator	i = m_LandEffectList.begin();
	
	for( ; i != m_LandEffectList.end() ; )
	{
		if( (*i) == *ppEffectBase )
		{
			m_LandEffectList.erase( i++ );
			
			return TRUE;
		}
		else
			i++;
	}
	
	return FALSE;
}

void CEffectManager::DeleteAllLandEffect(void)
{
	std::list	<CEffectBase *> ::iterator	i = m_LandEffectList.begin();
	
	for( ; i != m_LandEffectList.end() ; ++i )
	{
		(*i)->SetDestroy();
	}
	
	RLGS( "------------- DeleteAllLandEffect ---------------------------" );
}


//..............................................................................................................
// �ϸ��Ǿ� ���� ���ݽ� ���� �ٸ��� 
//..............................................................................................................
int CEffectManager::MAKE_EF_NPC_FLY_HAUPAI_LR(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( !CheckCharacter( pSrc , pDest ) )
		return FALSE;
	
	if( NULL == pExtraInfo )
	{
		return FALSE;
	}
	
	if( pExtraInfo->Critical )
	{
		CreateEffect( EF_NPC_FLY_ATTACK0 , pSrc , pDest , pExtraInfo );
	}
	else
	{
		switch( id )
		{
		case EF_NPC_FLY_HAUPAI_LR:
			CreateEffect( EF_NPC_FLY_ATTACK1 , pSrc , pDest , pExtraInfo );
			break;
			
		case EF_NPC_FLY_HAUPAI_RR:
			CreateEffect( EF_NPC_FLY_ATTACK0 , pSrc , pDest , pExtraInfo );
			break;
		}
	}
	
	return TRUE;
}

//................................................................................................................
// ���� �������Ʈ �߰� ( ����Ʈ ó�� ) By wxywxy 
//................................................................................................................
int CEffectManager::PushLandEffectBase(CEffectBase **ppEffectBase)
{
	m_EffectList.push_back( *ppEffectBase );
	
	Character *pSrcChar = (*ppEffectBase)->GetSrcChar();
	/*
	//.......................................................................................................
	// ����Ʈ �Ŵ����� ��ų Ÿ�Թ� �� ���� �Է� 
	// ���� ������ ���̵�� ����� -> Unique , Race , Skill Code , Cell
	//.......................................................................................................
	switch( m_TargetInfo.TargetType )
	{
	case n_PC:
	(*ppEffectBase)->SetPC();	
	break;
	case n_NPC:
	(*ppEffectBase)->SetNPC();	
	break;
	case n_Cell:
	(*ppEffectBase)->SetCell( m_TargetInfo.CellX , m_TargetInfo.CellY , m_TargetInfo.vPos );
	break;
	}
	//.......................................................................................................
	*/
	//if( pSrcChar != NULL )
	{
		m_LandEffectList.push_back( *ppEffectBase );
	}
	
	(*ppEffectBase)->m_ISLandLink = TRUE;
	
	PopReadyLandEffect( (*ppEffectBase)->GetSkillUnique() );
	
	return TRUE;
}


//................................................................................................................
// Delete Effect ( ������ų ����Ʈ ����� )
//................................................................................................................
int CEffectManager::Delete(DWORD SkillUnique)
{
	DebugPrint( "-------------Process Delete Land Skill %d----------------\n",SkillUnique);
	
	std::list	<CEffectBase *> ::iterator	i = m_EffectList.begin();
	
	for( ; i != m_EffectList.end() ; )
	{
		//........................................................................................................
		// ����Ʈ ����� 
		//........................................................................................................
		if( (*i)->GetSkillUnique() == SkillUnique )
		{
			DebugPrint( "-------------Delete Land Skill %d----------------\n",SkillUnique);
			
			(*i)->SetDestroy();
			
			i++;
		}
		else
		{
			i++;
		}
	}
	return FALSE;
}

//................................................................................................................
// ����Ʈ �Ŵ��� �������� �����
//................................................................................................................
int CEffectManager::DeleteOnManager(Character *pSrc)
{
	std::list	<CEffectBase *> ::iterator	i = m_EffectList.begin();
	
	for( ; i != m_EffectList.end() ; )
	{
		//........................................................................................................
		// ����Ʈ ����� 
		//........................................................................................................
		if( (*i)->GetSrcChar()  == pSrc ||
			(*i)->GetDestChar() == pSrc   )
		{
			m_EffectList.erase( i++ );
		}
		else
		{
			i++;
		}
	}
	
	return TRUE;
}

//................................................................................................................
// ����Ʈ �Ŵ��� ĳ���� ���� ť�� �����
//................................................................................................................
int CEffectManager::DeleteNow(Character *pSrc)
{
	std::list	<CEffectBase *> ::iterator	i = m_EffectList.begin();
	
	for( ; i != m_EffectList.end() ; )
	{
		//........................................................................................................
		// ����Ʈ ����� 
		//........................................................................................................
		
		if( (*i)->GetSrcChar()  == pSrc ||
			(*i)->GetDestChar() == pSrc  )
		{
			if( (*i) && (*i)->m_ISLandLink == FALSE && 
				(*i)->ISLinkItem() == FALSE )	
			{
				//....................................................................................................
				// ( ĳ���ͳ� ����Ʈ�� ���� ������ )
				//....................................................................................................
				Character    *pSrcChar = (*i)->GetSrcChar();
				
				if( pSrcChar != NULL )
				{
					pSrcChar->PopEffectBase( (CEffectBase **)&(*i) );
				}
				else
				{
					return false;
				}
				
				CEffectBase * pData = (*i);
				SAFE_DELETE( pData )
					(*i) = NULL;
				
				m_EffectList.erase( i++ );
			}
			///--JAPAN_BUG_MODIFY
			///-- ������ ĳ���� ���� ������� ���ֱ�
			else if( !(pSrc->m_bPC) && pSrc->m_State.nCharacterType == nCharacterTypePlayerNPC )
			{
				///--��ũ �����۵� �����ش�.
				if( (*i) && (*i)->m_ISLandLink == FALSE )
				{
					Character    *pSrcChar = (*i)->GetSrcChar();
				
					if( pSrcChar != NULL )
					{
						pSrcChar->PopEffectBase( (CEffectBase **)&(*i) );
					}
					else
					{
						return false;
					}
					
					CEffectBase * pData = (*i);
					SAFE_DELETE( pData )
					(*i) = NULL;
					
					m_EffectList.erase( i++ );
				}
				else
				{
					i++;
				}
			}			
			else
			{
				i++;
			}
		}
		else
		{
			i++;
		}
	}
	
	return TRUE;
}

//................................................................................................................
// ����Ʈ SetDestroy();
//................................................................................................................
int CEffectManager::Delete(Character *pSrc)
{
	std::list	<CEffectBase *> ::iterator	i = m_EffectList.begin();
	
	for( ; i != m_EffectList.end() ; ++i )
	{
		//........................................................................................................
		// ����Ʈ ����� 
		//........................................................................................................
		
		if( (*i)->GetSrcChar()  == pSrc ||
			(*i)->GetDestChar() == pSrc  )
		{
			
			if( (*i)->m_ISLandLink == FALSE )	
				(*i)->SetDestroy();
		}
	}
	
	return TRUE;
}

//...............................................................................................................
// ������ ����
//...............................................................................................................
int CEffectManager::MAKE_EF_GAGE_TRANS(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// ����Ʈ ������ �ҽ��ʸ� ����Ʈ ���
	if( pSrc == NULL || pDest == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFGageTrans );  					// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );												// �޴����� �ֱ�
	
	return TRUE;
}

//..............................................................................................................
// �ż��� ȭ��
//..............................................................................................................
int CEffectManager::MAKE_EF_FAST_BOW(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// ����Ʈ ������ �ҽ��ʸ� ����Ʈ ���
	if( pSrc == NULL || pDest == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFFastBow );						// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );												// �޴����� �ֱ�
	
	return TRUE;
}

//...............................................................................................................
// 	�ż� ����
//...............................................................................................................
int CEffectManager::MAKE_EF_HOLY_AURA(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	// ����Ʈ ������ �ҽ��ʸ� ����Ʈ ���
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	// 1:N ������
	int Count = pExtraInfo->GetTargetCount();
	
	Character			*pTempDest = NULL;
	EFFECT_EXTRA_INFO    Info;
	
	for( int i = 0 ; i < Count ; ++i )
	{
		if( pExtraInfo->GetTargetInfo( &pTempDest , &Info , i ) )
		{
			EffectTargetHit( pSrc , pTempDest , &Info );
		}
	}
	
	return TRUE;
}

//..............................................................................................................
// �ż� ��Ÿ����
//..............................................................................................................
int CEffectManager::MAKE_EF_HOLY_APPEAR(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// ����Ʈ ������ �ҽ��ʸ� ����Ʈ ���
	if( pSrc == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFHolyAppear );						// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );												// �޴����� �ֱ�
	
	return TRUE;
}

//..............................................................................................................
// ���� �׽�Ʈ
//..............................................................................................................
int CEffectManager::MAKE_EF_WING0(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFWing0 );					// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );				// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );										// �޴����� �ֱ�
	
	return TRUE;
}

//..............................................................................................................
// �ִ����� ���� �׽�Ʈ
//..............................................................................................................
int CEffectManager::MAKE_EF_ANIWING0(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFAniWing0 );				// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );				// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );										// �޴����� �ֱ�
	
	return TRUE;
}

//..............................................................................................................
// ���� ���
//..............................................................................................................
int CEffectManager::MAKE_EF_EMBLEM_FLAG(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFEmblemFlag );				// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );				// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );										// �޴����� �ֱ�
	
	return TRUE;
}

//..............................................................................................................
// �׸��� �⺻����
//..............................................................................................................
int CEffectManager::MAKE_EF_GRIZZL_BAS_ATT(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	StandTargetHit( pSrc , pDest , pExtraInfo );
	
	return TRUE;
}

//..............................................................................................................
// �׸��� �⺻������ ��, ��
//..............................................................................................................
int CEffectManager::MAKE_EF_GRIZZL_BAS_ATTED_0(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pDest == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 2.0f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos( D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
	
	return TRUE;
}

int CEffectManager::MAKE_EF_GRIZZL_BAS_ATTED_2(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pDest == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 2.0f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos( D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
	
	return TRUE;
}

//..............................................................................................................
// Ű�޶� ��Ʈ �⺻����
//..............................................................................................................
int CEffectManager::MAKE_EF_CHIME_ANT_BAS_ATT(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	StandTargetHit( pSrc , pDest , pExtraInfo );
	
	return TRUE;
}

//..............................................................................................................
// Ű�޶� ��Ʈ �⺻������ ��, ��
//..............................................................................................................
int CEffectManager::MAKE_EF_CHIME_ANT_BAS_ATTED_0(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pDest == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 2.0f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
	
	return TRUE;
}

int CEffectManager::MAKE_EF_CHIME_ANT_BAS_ATTED_2(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pDest == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 2.0f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
	
	return TRUE;
}

//..............................................................................................................
// �Ҳ�
//..............................................................................................................
int CEffectManager::MAKE_EF_FIREFLOWER0(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFFireWork );				// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );				// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );										// �޴����� �ֱ�
	
	return TRUE;
}


//................................................................................................................
// ����Ʈ
//................................................................................................................
int CEffectManager::MAKE_EF_DETECT(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// ����Ʈ ������ �ҽ��ʸ� ����Ʈ ���
	if( pSrc == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFDetect );							// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );												// �޴����� �ֱ�
	
	return TRUE;
}


//...............................................................................................................
// �⿭��
//...............................................................................................................
int CEffectManager::MAKE_EF_HAND_PAR(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// ����Ʈ ������ �ҽ��ʸ� ����Ʈ ���
	if( pSrc == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFHandPar );						// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );												// �޴����� �ֱ�
	
	return TRUE;
}

//..............................................................................................................
// Son Brush
//..............................................................................................................
int CEffectManager::MAKE_EF_SON_BURSH(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// ����Ʈ ������ �ҽ��ʸ� ����Ʈ ���
	if( pSrc == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFSonBrush );						// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );												// �޴����� �ֱ�
	
	return TRUE;
}

//................................................................................................................
// ����Ʈ �Ŵ��� ĳ���� ���� ť�� �����
//................................................................................................................
int CEffectManager::DeleteOnlySrc(Character *pSrc)
{
	std::list	<CEffectBase *> ::iterator	i = m_EffectList.begin();
	
	for( ; i != m_EffectList.end() ; ++i )
	{
		//........................................................................................................
		// ����Ʈ ����� 
		//........................................................................................................
		
		if( (*i)->GetSrcChar() == pSrc )
		{
			// ��ũ ������ üũ
			if( !(*i)->ISLinkItem() && !(*i)->ISConEffect() )
			{
				(*i)->SetDestroy();
				
				RLGS( "-----  DeleteOnlySrc ---- " );
			}
		}
	}
	
	return TRUE;
}

//================================================================================================================
//
// ����Ʈ ���� �Լ�	
//
//================================================================================================================

//................................................................................................................
// �ȱ� ����
//................................................................................................................
int CEffectManager::MAKE_EF_WALK_DUST(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	D3DXVECTOR3	Pos( pSrc->GetPosTM()._41 , pSrc->GetPosTM()._42 , pSrc->GetPosTM()._43 );	
	
	g_Particle.DisplayUpdate( PT_WALK_DUST , Pos );
	
	return TRUE;
}

//...............................................................................................................
// 	��� �¼�
//...............................................................................................................
int CEffectManager::MAKE_EF_DEFEND_SETUP(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 2.1f , pSrc->GetUnique() , pSrc->GetIndex() );
	//................................................................................	
	// ��ų �ߵ� ����
	//................................................................................	
	if( NULL != pExtraInfo )
		g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	
	return TRUE;
}

//...............................................................................................................
// 	���� ����
//...............................................................................................................
int CEffectManager::MAKE_EF_BLOOD_RUNAWAY(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 2.1f , pSrc->GetUnique() , pSrc->GetIndex() );
	
	//................................................................................	
	// ��ų �ߵ� ����
	//................................................................................	
	if( NULL != pExtraInfo )
	{
		EffectTargetHit( pSrc , pDest , pExtraInfo );
		
		g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	}
	
	
	return TRUE;
}

//...............................................................................................................
// 	�ұ��� ����
//...............................................................................................................
int CEffectManager::MAKE_EF_LIVE_MIND(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 2.1f , pSrc->GetUnique() , pSrc->GetIndex() );
	
	
	//................................................................................	
	// ��ų �ߵ� ����
	//................................................................................	
	if( NULL != pExtraInfo )
	{
		g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
		EffectTargetHit( pSrc , pDest , pExtraInfo );
	}
	
	return TRUE;
}

//..............................................................................................................
// ����Ʈ�� ���Ǿ�
//..............................................................................................................
int CEffectManager::MAKE_EF_LIGHTNIG_SPIRE(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL || pDest == NULL )
	{
		return -1;
	}
	
	int DamageID;
	
	switch( id )
	{
	case EF_LIGHTNIG_SPIRE0:
		DamageID = EF_LIGHTNIG_SPIREED0;
		break;
		
	case EF_LIGHTNIG_SPIRE1:
		DamageID = EF_LIGHTNIG_SPIREED1;
		break;
		
	case EF_LIGHTNIG_SPIRE2:
		DamageID = EF_LIGHTNIG_SPIREED2;
		break;
	}
	
	pExtraInfo->AfterEffect = DamageID;
	pExtraInfo->DestType = 0;
	pExtraInfo->lifeTime = 2.1f;
	
	g_Particle.SetEffSetCon( 0.0f , DamageID , 0 , 2.1f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
	
	
	//................................................................................	
	// ��ų �ߵ� ����
	//................................................................................	
	if( NULL != pExtraInfo )
		g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );	
	
	EffectTargetHit( pSrc , pDest , pExtraInfo );
	
	return TRUE;
}

//..............................................................................................................
// �ڷ���Ʈ
//..............................................................................................................
int CEffectManager::MAKE_EF_TELEPORT(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	D3DXVECTOR3 vPos( pSrc->GetPosTM()._41 , 
		pSrc->GetPosTM()._42 + 10.0f,
		pSrc->GetPosTM()._43 );
	
	g_Particle.SetEffSetCon( 0.0f , id , 100 , 1.1f , -1 , -1 , vPos );
	
	
	//................................................................................	
	// ��ų �ߵ� ����
	//................................................................................	
	if( NULL != pExtraInfo )
		g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	
	EffectTargetHit( pSrc , pDest , pExtraInfo );
	
	return TRUE;
}

//................................................................................................................
// Ȧ�� ����Ʈ
//................................................................................................................
int CEffectManager::MAKE_EF_HOLY_LIGHT(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{	
	if( CheckCharacter( pSrc , pDest ) == FALSE )
		return TRUE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 3.1f , pSrc->GetUnique() , pSrc->GetIndex() );
	
	//................................................................................	
	// ��ų �ߵ� ����
	//................................................................................	
	if( NULL != pExtraInfo )
		g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	
	g_Particle.SetEffSetCon( 0.0f , EF_HOLY_LIGHT_ED , 0 , 3.1f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
	
	pExtraInfo->AfterEffect = EF_HOLY_LIGHT_ED;
	pExtraInfo->DestType = 0;
	pExtraInfo->lifeTime = 3.1f;
	
	EffectTargetHit( pSrc , pDest , pExtraInfo );
	
	return TRUE;
}

//...............................................................................................................
// ���� ��ų ( ����Ʈ )
//...............................................................................................................
int CEffectManager::MAKE_EF_MIRACLE(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 3.1f , pSrc->GetUnique() , pSrc->GetIndex() );
	
	//................................................................................	
	// ��ų �ߵ� ����
	//................................................................................	
	if( NULL != pExtraInfo )
	{
		g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
		
		EffectTargetHit( pSrc , pDest , pExtraInfo );
	}
	
	return TRUE;
}

//................................................................................................................
// ������ ��
//................................................................................................................
int CEffectManager::MAKE_EF_UNCOMBO(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL || pDest == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 5.1f , pSrc->GetUnique() , pSrc->GetIndex() );
	
	//................................................................................	
	// ��ų �ߵ� ����
	//................................................................................	
	if( NULL != pExtraInfo )
		g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	
	g_Particle.SetEffSetCon( 0.0f , EF_UNCOMBO_ED , 0 , 5.1f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
	
	pExtraInfo->AfterEffect = EF_UNCOMBO_ED;
	pExtraInfo->DestType = 0;
	pExtraInfo->lifeTime = 5.1f;
	
	EffectTargetHit( pSrc , pDest , pExtraInfo );
	
	return TRUE;
}

//................................................................................................................
// �н�
//................................................................................................................
int CEffectManager::MAKE_EF_SELF_COPY(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 3.1f , pSrc->GetUnique() , pSrc->GetIndex() );
	
	//................................................................................	
	// ��ų �ߵ� ����
	//................................................................................	
	if( NULL != pExtraInfo )
		g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	
	EffectTargetHit( pSrc , pDest , pExtraInfo );
	
	return TRUE;
}

//................................................................................................................
// ���潺 ���ζ�
//................................................................................................................
int CEffectManager::MAKE_EF_DEFENSE_AURA(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 2.1f , pSrc->GetUnique() , pSrc->GetIndex() );
	//................................................................................	
	// ��ų �ߵ� ����
	//................................................................................	
	g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	
	// ����Ʈ ������ �ҽ��ʸ� ����Ʈ ���
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	// 1:N ������
	int Count = pExtraInfo->GetTargetCount();
	
	Character			*pTempDest = NULL;
	EFFECT_EXTRA_INFO    Info;
	
	for( int i = 0 ; i < Count ; ++i )
	{
		if( pExtraInfo->GetTargetInfo( &pTempDest , &Info , i ) )
		{
			//g_Particle.SetEffSetCon( 0.0f , EF_DEFENSE_AURA_ED , 0 , 2.6f , pTempDest->GetUnique() , pTempDest->GetIndex() );
			EffectTargetHit( pSrc , pTempDest , &Info );
		}
	}
	
	//................................................................................	
	// ���� ����
	//................................................................................	
	if( pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillAttackedSound( pExtraInfo->SkillCode , pDest );
	
	if( pSrc == g_Pc.GetPlayer() )
		g_Particle.RockCamera( ( rand() % 200 ) + 300 );
	
	return TRUE;
}


//................................................................................................................
// ���潺 �ǵ�
//................................................................................................................
int CEffectManager::MAKE_EF_DEPENCE_SHILED(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 2.1f , pSrc->GetUnique() , pSrc->GetIndex() );
	
	//................................................................................	
	// ��ų �ߵ� ����
	//................................................................................	
	if( NULL != pExtraInfo )
		g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	
	EffectTargetHit( pSrc , pDest , pExtraInfo );
	
	return TRUE;
}

//..............................................................................................................
// ���� ���۸�
//..............................................................................................................
int CEffectManager::MAKE_EF_MANA_BUFFERING(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 2.1f , pSrc->GetUnique() , pSrc->GetIndex() );
	
	//................................................................................	
	// ��ų �ߵ� ����
	//................................................................................	
	if( NULL != pExtraInfo )
		g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	
	EffectTargetHit( pSrc , pDest , pExtraInfo );
	
	return TRUE;
}

//..............................................................................................................
// ���� ����
//..............................................................................................................
int CEffectManager::MAKE_EF_SELF_HEAL(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 5.1f , pSrc->GetUnique() , pSrc->GetIndex() );
	
	//................................................................................	
	// ��ų �ߵ� ����
	//................................................................................	
	if( NULL != pExtraInfo )
		g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	
	EffectTargetHit( pSrc , pDest , pExtraInfo );
	
	return TRUE;
}

//..............................................................................................................
// ����
//..............................................................................................................
int CEffectManager::MAKE_EF_RETURN_HOME(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	D3DXVECTOR3 vPos( pSrc->GetPosTM()._41 , 
		pSrc->GetPosTM()._42 ,
		pSrc->GetPosTM()._43  );
	
	g_Particle.SetEffSetCon( 0.0f , id , 100 , 5.1f , -1 , -1 , vPos );
	
	//................................................................................	
	// ��ų �ߵ� ����
	//................................................................................	
	if( NULL != pExtraInfo )
		g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	
	EffectTargetHit( pSrc , pDest , pExtraInfo );
	
	return TRUE;
}


//..............................................................................................................
// ����
//..............................................................................................................
int CEffectManager::MAKE_EF_SLEEP_STICK(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL || pDest == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , EF_SLEEP_STICK    , 0 , 2.1f , pSrc->GetUnique() , pSrc->GetIndex() );
	
	g_Particle.SetEffSetCon( 0.0f , EF_SLEEP_STICK_ED , 0 , 1.1f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
	
	//................................................................................	
	// ��ų �ߵ� ����
	//................................................................................	
	if( NULL != pExtraInfo )
		g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	
	pExtraInfo->AfterEffect = EF_SLEEP_STICK_ED;
	pExtraInfo->DestType = 0;
	pExtraInfo->lifeTime = 1.1f;
	
	EffectTargetHit( pSrc , pDest , pExtraInfo );
	
	return TRUE;
}

//................................................................................................................
// �ȱ� ��
//................................................................................................................
int CEffectManager::MAKE_EF_WALK_WAVE(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	D3DXVECTOR3	Pos( pSrc->GetPosTM()._41 , pSrc->GetPosTM()._42 , pSrc->GetPosTM()._43 );
	
	Pos.y = g_Map.GetWaterHeight( Pos.x , Pos.z );
	
	if( id == EF_RUN_WAVE )
	{
		Pos.x += pSrc->m_vDir.x;
		Pos.z += pSrc->m_vDir.z;
	}
	
	g_Particle.SetEffSetCon( 0.0f , id , ESLT_POS , 2.1f , -1 , -1 , Pos , pSrc->GetDirection() );
	
	//DebugPrint( "Player Y Rad %f\n" , pSrc->m_Mov.now_Yrad );
	
	return TRUE;
}

//..............................................................................................................
// �Ŵ���� �⺻ ����
//..............................................................................................................
int CEffectManager::MAKE_EF_BIG_NPC_ATTACK0(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// ����Ʈ ������ �ҽ��ʸ� ����Ʈ ���
	if( pSrc == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFBigNpc0 );						// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );												// �޴����� �ֱ�
	
																		/*
																		g_Particle.SetEffSetCon( 0.0f , id , ESLT_CHARPOS , 8.1f , pSrc->GetUnique() , pSrc->GetIndex() );
																		
																		  StandTargetHit( pSrc , pDest , pExtraInfo );
	*/
	return TRUE;
}

//...........................................................................................................
// �⺻���� 1:1 ������ �ִ°� ó��
//...........................................................................................................
int CEffectManager::MakeStandAttackProcess(Character *pSrc, Character *pDest,int SrcSetNum  , int SrcSetType  , float SrcTimer,
										   int DestSetNum  , int DestSetType  , float DestTimer,LPEFFECT_EXTRA_INFO pExtraInfo)
										   
{
	if( !CheckCharacter( pSrc , pDest ) || pExtraInfo == NULL )
	{
		return TRUE;
	}
	
	if( SrcSetNum != -1 )
	{
		g_Particle.SetEffSetCon( 0.0f , SrcSetNum , SrcSetType , SrcTimer , pSrc->GetUnique() , pSrc->GetIndex() );
	}
	
	if( DestSetNum != -1 )
	{
		if( DestSetType == ESLT_SRC_RAD )
		{
			g_Particle.SetEffSetCon( 0.0f , DestSetNum , DestSetType , DestTimer , 
				pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc),
				pSrc->GetDirection() + D3DXToRadian( 180 ) );	
		}
		else
		{
			g_Particle.SetEffSetCon( 0.0f , DestSetNum , DestSetType , DestTimer , 
				pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
		}
	}
	
	StandTargetHit( pSrc , pDest , pExtraInfo );
	
	return TRUE;
}

int CEffectManager::MakeEffectSetSrcLinkProcess(Character *pSrc,int SrcSetNum  , int SrcSetType  , float SrcTimer)
{
	if( SrcSetNum != -1 )
	{
		g_Particle.SetEffSetCon( 0.0f , SrcSetNum , SrcSetType , SrcTimer , pSrc->GetUnique() , pSrc->GetIndex() );
	}
	
	
	return TRUE;
}


int CEffectManager::MAKE_EF_BIG_NPC_ATTACK0ED(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , ESLT_CHARPOS , 8.1f , pSrc->GetUnique() , pSrc->GetIndex() );
	
	return TRUE;
}

//..............................................................................................................
// �Ŵ���� ��Ÿ����
//..............................................................................................................
int CEffectManager::MAKE_EF_BIG_NPC_APPEAR0(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , ESLT_CHARPOS , EFFECT_SET_LOOP_TIME , pSrc->GetUnique() , pSrc->GetIndex() );
	
	return TRUE;
}


//..............................................................................................................
// ���� ������ 
//..............................................................................................................
int CEffectManager::MAKE_EF_SI_ITEM_ATTACK0(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	StandTargetHit( pSrc , pDest , pExtraInfo );
	
	return TRUE;
}

int CEffectManager::MAKE_EF_SI_ITEM_ATTACK1(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	StandTargetHit( pSrc , pDest , pExtraInfo );
	
	return TRUE;
}

int CEffectManager::MAKE_EF_SI_ITEM_ATTACK2(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	StandTargetHit( pSrc , pDest , pExtraInfo );
	
	return TRUE;
}

int CEffectManager::MAKE_EF_SI_ITEM_ATTACK3(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	StandTargetHit( pSrc , pDest , pExtraInfo );
	
	return TRUE;
}

int CEffectManager::MAKE_EF_SI_ITEM_ATTACKED0(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pDest == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 2.1f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
	
	return TRUE;
}

int CEffectManager::MAKE_EF_SI_ITEM_ATTACKED1(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pDest == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 2.1f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
	
	return TRUE;
}

int CEffectManager::MAKE_EF_SI_ITEM_ATTACKED2(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pDest == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 2.1f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
	
	return TRUE;
}

int CEffectManager::MAKE_EF_SI_ITEM_ATTACKED3(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pDest == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 2.1f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
	
	return TRUE;
}

//...............................................................................................................
// ���̸� ��θӸ� ������ 
//...............................................................................................................
int CEffectManager::MAKE_EF_NPC_HEAVY_STICK(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	StandTargetHit( pSrc , pDest , pExtraInfo );
	
	return TRUE;
}

//...............................................................................................................
// ���̸� ��θӸ� ������ ������
//...............................................................................................................
int CEffectManager::MAKE_EF_NPC_HEAVY_STICKED(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pDest == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 2.1f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
	
	return TRUE;
}


//................................................................................................................
// ���� ����
//................................................................................................................
int CEffectManager::MAKE_EF_WEAPON_CRASH0(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// ����Ʈ ������ �ҽ��ʸ� ����Ʈ ���
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	g_Particle.SetEffSetCon( 0.0f , EF_WEAPON_CRASH0 , 0 , 1.1f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
	//................................................................................	
	// ��ų �ߵ� ����
	//................................................................................	
	g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	
	pExtraInfo->AfterEffect = EF_WEAPON_CRASH0;
	pExtraInfo->DestType = 0;
	pExtraInfo->lifeTime = 1.1f;
	
	EffectTargetHit( pSrc , pDest , pExtraInfo );
	//................................................................................	
	// ���� ����
	//................................................................................	
	if( pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillAttackedSound( pExtraInfo->SkillCode , pDest );
	
	return TRUE;
}

//...............................................................................................................
// NPC ������ �ٴ� ���
//...............................................................................................................
int CEffectManager::MAKE_EF_AXE_LAND_HIT(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , EF_AXE_LAND_HIT , 0 , 2.6f , pSrc->GetUnique() , pSrc->GetIndex() );
	//................................................................................	
	// ��ų �ߵ� ����
	//................................................................................	
	g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	
	// ����Ʈ ������ �ҽ��ʸ� ����Ʈ ���
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	// 1:N ������
	int Count = pExtraInfo->GetTargetCount();
	
	Character			*pTempDest = NULL;
	EFFECT_EXTRA_INFO    Info;
	
	for( int i = 0 ; i < Count ; ++i )
	{
		if( pExtraInfo->GetTargetInfo( &pTempDest , &Info , i ) )
		{
			g_Particle.SetEffSetCon( 0.0f , EF_AXE_LAND_HITED , 0 , 2.6f , pTempDest->GetUnique() , pTempDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
			
			Info.AfterEffect = EF_AXE_LAND_HITED;
			Info.DestType = 0;
			Info.lifeTime = 2.6f;
			
			EffectTargetHit( pSrc , pTempDest , &Info );
		}
	}
	
	//................................................................................	
	// ���� ����
	//................................................................................	
	if( pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillAttackedSound( pExtraInfo->SkillCode , pDest );
	
	return TRUE;
}

//..............................................................................................................
// �Ŵ���� ��ų
//..............................................................................................................
int CEffectManager::MAKE_EF_BIG_NPC_SKILL0(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFBigNpcSkill0 );					// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );												// �޴����� �ֱ�	
	
	return TRUE;
}

//..............................................................................................................
// ����XP-610 ���� ��
//..............................................................................................................
int CEffectManager::MAKE_EF_NPC_BIZ_BEAM(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFBizEyeBeam );						// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );												// �޴����� �ֱ�	
	
	return TRUE;
}


//...............................................................................................................
// ������Ʈ ��Ȱ
//...............................................................................................................
int CEffectManager::MAKE_EF_REVIVAL(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 2.1f , pSrc->GetUnique() , pSrc->GetIndex() );
	//................................................................................	
	// ��ų �ߵ� ����
	//................................................................................	
	g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	
	// ����Ʈ ������ �ҽ��ʸ� ����Ʈ ���
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	EffectTargetHit( pSrc , pDest , pExtraInfo );
	//................................................................................	
	// ���� ����
	//................................................................................	
	if( pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillAttackedSound( pExtraInfo->SkillCode , pDest );
	
	return TRUE;
}

//..............................................................................................................
// ������ ����
//..............................................................................................................
int CEffectManager::MAKE_EF_FRAME_STORM(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	D3DXVECTOR3 vPos;
	
	vPos.x = m_TargetInfo.vPos.x;
	vPos.z = m_TargetInfo.vPos.z;
	vPos.y = g_Map.PHF_GetHeight( vPos.x , vPos.z );
	
	g_Particle.SetEffSetCon( 0.0f , id , 100 , 5.1f , -1 , -1 , vPos );
	//................................................................................	
	// ��ų �ߵ� ����
	//................................................................................	
	g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	
	// ����Ʈ ������ �ҽ��ʸ� ����Ʈ ���
	if( pExtraInfo == NULL || pSrc == NULL )
		return TRUE;
	
	// 1:N ������
	int Count = pExtraInfo->GetTargetCount();
	
	Character			*pTempDest = NULL;
	EFFECT_EXTRA_INFO    Info;
	
	for( int i = 0 ; i < Count ; ++i )
	{
		if( pExtraInfo->GetTargetInfo( &pTempDest , &Info , i ) )
		{
			//................................................................................	
			// ���� ����
			//................................................................................	
			if( pExtraInfo != NULL )
				g_Particle.m_EffectManager.GetSkillAttackedSound( pExtraInfo->SkillCode , pTempDest );		
			
			
			g_Particle.SetEffSetCon( 0.0f , EF_FRAME_STORMED , 0 , 5.1f , pTempDest->GetUnique() , 
									 pTempDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
			
			Info.AfterEffect = EF_FRAME_STORMED;
			Info.DestType = 0;
			Info.lifeTime = 5.1f;
			
			EffectTargetHit( pSrc , pTempDest , &Info );
		}
	}
	
	return TRUE;
}

//................................................................................................................
// ġ���� ��
//................................................................................................................
int CEffectManager::MAKE_EF_HEAL_LIGHT(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 3.1f , pSrc->GetUnique() , pSrc->GetIndex() );
	//................................................................................	
	// ��ų �ߵ� ����
	//................................................................................	
	g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	
	// ����Ʈ ������ �ҽ��ʸ� ����Ʈ ���
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	// 1:N ������
	int Count = pExtraInfo->GetTargetCount();
	
	Character			*pTempDest = NULL;
	EFFECT_EXTRA_INFO    Info;
	
	for( int i = 0 ; i < Count ; ++i )
	{
		if( pExtraInfo->GetTargetInfo( &pTempDest , &Info , i ) )
		{
			g_Particle.SetEffSetCon( 0.0f , EF_HEAL_LIGHT_ED , 0 , 5.1f , pTempDest->GetUnique() , pTempDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
			
			Info.AfterEffect = EF_HEAL_LIGHT_ED;
			Info.DestType = 0;
			Info.lifeTime = 5.1f;
			
			EffectTargetHit( pSrc , pTempDest , &Info );
		}
	}
	
	//................................................................................	
	// ���� ����
	//................................................................................	
	if( pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillAttackedSound( pExtraInfo->SkillCode , pDest );
	
	return TRUE;
}

//...............................................................................................................
// �߱�����
//...............................................................................................................
int CEffectManager::MAKE_EF_FOOT_QUAKE(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 2.1f , pSrc->GetUnique() , pSrc->GetIndex() );
	//................................................................................	
	// ��ų �ߵ� ����
	//................................................................................	
	g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	
	//................................................................................	
	// ���� ����
	//................................................................................	
	if( pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillAttackedSound( pExtraInfo->SkillCode , pDest );
	
	// ����Ʈ ������ �ҽ��ʸ� ����Ʈ ���
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	int AttackID = 0;
	
	switch( id )
	{
	case EF_FOOT_QUAKE0:
		AttackID = EF_FOOT_QUAKE_ATTACKED0;
		break;
	case EF_FOOT_QUAKE1:
		AttackID = EF_FOOT_QUAKE_ATTACKED1;
		break;
	case EF_FOOT_QUAKE2:
		AttackID = EF_FOOT_QUAKE_ATTACKED2;
		break;
	}
	
	// 1:N ������
	int Count = pExtraInfo->GetTargetCount();
	
	Character			*pTempDest = NULL;
	EFFECT_EXTRA_INFO    Info;
	
	for( int i = 0 ; i < Count ; ++i )
	{
		if( pExtraInfo->GetTargetInfo( &pTempDest , &Info , i ) )
		{
			g_Particle.SetEffSetCon( 0.0f , AttackID , 0 , 2.6f , pTempDest->GetUnique() , pTempDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
			
			Info.AfterEffect = AttackID;
			Info.DestType = 0;
			Info.lifeTime = 2.6f;
			
			EffectTargetHit( pSrc , pTempDest , &Info );
		}
	}
	
	if( pSrc == g_Pc.GetPlayer() )
		g_Particle.RockCamera( ( rand() % 200 ) + 300 );
	
	return TRUE;
}


//................................................................................................................
// ��ǳ����
//................................................................................................................
int CEffectManager::MAKE_EF_CIRCLE_KICK(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 2.1f , pSrc->GetUnique() , pSrc->GetIndex() );
	//................................................................................	
	// ��ų �ߵ� ����
	//................................................................................	
	g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	
	// ����Ʈ ������ �ҽ��ʸ� ����Ʈ ���
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	// 1:N ������
	int Count = pExtraInfo->GetTargetCount();
	
	Character			*pTempDest = NULL;
	EFFECT_EXTRA_INFO    Info;
	
	for( int i = 0 ; i < Count ; ++i )
	{
		if( pExtraInfo->GetTargetInfo( &pTempDest , &Info , i ) )
		{
			g_Particle.SetEffSetCon( 0.0f , EF_CIRCLE_KICK_ED , 0 , 2.6f , pTempDest->GetUnique() , pTempDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
			
			Info.AfterEffect = EF_CIRCLE_KICK_ED;
			Info.DestType = 0;
			Info.lifeTime = 2.6f;
			
			EffectTargetHit( pSrc , pTempDest , &Info );
		}
	}
	
	//................................................................................	
	// ���� ����
	//................................................................................	
	if( pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillAttackedSound( pExtraInfo->SkillCode , pDest );
	
	if( pSrc == g_Pc.GetPlayer() )
		g_Particle.RockCamera( ( rand() % 200 ) + 300 );
	
	return TRUE;
}


//..............................................................................................................
// Ÿ�̴� ���̺�
//..............................................................................................................
int CEffectManager::MAKE_EF_TYNEE_WAVE(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// ����Ʈ ������ �ҽ��ʸ� ����Ʈ ���
	if( pSrc == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFTyneeWave );						// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );												// �޴����� �ֱ�
	
	return TRUE;
}

//................................................................................................................
// ������
//................................................................................................................
int CEffectManager::MAKE_EF_ENERGY(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// ����Ʈ ������ �ҽ��ʸ� ����Ʈ ���
	if( pSrc == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFEnergy );							// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );												// �޴����� �ֱ�
	
	return TRUE;
}


//...............................................................................................................
// ����
//...............................................................................................................
int CEffectManager::MAKE_EF_SWORD_FLASH(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , ESLT_SRC_RAD , 2.1f , pSrc->GetUnique() , pSrc->GetIndex() , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) , pSrc->GetDirection() );
	//................................................................................	
	// ��ų �ߵ� ����
	//................................................................................	
	g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	
	// ����Ʈ ������ �ҽ��ʸ� ����Ʈ ���
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	int AttackID = 0;
	
	switch( id )
	{
	case EF_SWORD_FLASH0:
		AttackID = EF_SWORD_FLASHED0;
		break;
	case EF_SWORD_FLASH1:
		AttackID = EF_SWORD_FLASHED1;
		break;
	case EF_SWORD_FLASH2:
		AttackID = EF_SWORD_FLASHED2;
		break;
	}
	
	// 1:N ������
	int Count = pExtraInfo->GetTargetCount();
	
	Character			*pTempDest = NULL;
	EFFECT_EXTRA_INFO    Info;
	
	for( int i = 0 ; i < Count ; ++i )
	{
		if( pExtraInfo->GetTargetInfo( &pTempDest , &Info , i ) )
		{
			g_Particle.SetEffSetCon( 0.0f , AttackID , 0 , 2.6f , pTempDest->GetUnique() , pTempDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
			
			Info.AfterEffect = AttackID;
			Info.DestType = 0;
			Info.lifeTime = 2.6f;
			
			EffectTargetHit( pSrc , pTempDest , &Info );
		}
	}
	
	//................................................................................	
	// ���� ����
	//................................................................................	
	if( pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillAttackedSound( pExtraInfo->SkillCode , pDest );
	
	return TRUE;
}

//...............................................................................................................
// ��Ƽ ����
//...............................................................................................................
int CEffectManager::MAKE_EF_PARTY_HEAL(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 4.1f , pSrc->GetUnique() , pSrc->GetIndex() );
	//................................................................................	
	// ��ų �ߵ� ����
	//................................................................................	
	g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	
	// ����Ʈ ������ �ҽ��ʸ� ����Ʈ ���
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	// 1:N ������
	int Count = pExtraInfo->GetTargetCount();
	
	Character			*pTempDest = NULL;
	EFFECT_EXTRA_INFO    Info;
	
	for( int i = 0 ; i < Count ; ++i )
	{
		if( pExtraInfo->GetTargetInfo( &pTempDest , &Info , i ) )
		{
			g_Particle.SetEffSetCon( 0.0f , EF_PARTY_HEALED , 0 , 4.6f , pTempDest->GetUnique() , pTempDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
			
			Info.AfterEffect = EF_PARTY_HEALED;
			Info.DestType = 0;
			Info.lifeTime = 4.6f;
			
			EffectTargetHit( pSrc , pTempDest , &Info );
		}
	}
	
	//................................................................................	
	// ���� ����
	//................................................................................	
	if( pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillAttackedSound( pExtraInfo->SkillCode , pDest );
	
	
	return TRUE;
}


//...............................................................................................................
// ���̸� �ٴ� ��� ��ų
//...............................................................................................................
int CEffectManager::MAKE_EF_NPC_QUAKE_HEAVY_STICK(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , EF_NPC_QUAKE_HEAVY_STICKED2 , 0 , 2.1f , pSrc->GetUnique() , pSrc->GetIndex() );
	//................................................................................	
	// ��ų �ߵ� ����
	//................................................................................	
	g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	
	
	// ����Ʈ ������ �ҽ��ʸ� ����Ʈ ���
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	// 1:N ������
	int Count = pExtraInfo->GetTargetCount();
	
	Character			*pTempDest = NULL;
	EFFECT_EXTRA_INFO    Info;
	
	for( int i = 0 ; i < Count ; ++i )
	{
		if( pExtraInfo->GetTargetInfo( &pTempDest , &Info , i ) )
		{
			g_Particle.SetEffSetCon( 0.0f , id , 0 , 2.6f , pTempDest->GetUnique() , pTempDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
			
			Info.AfterEffect = id;
			Info.DestType = 0;
			Info.lifeTime = 2.6f;
			
			EffectTargetHit( pSrc , pTempDest , &Info );
		}
	}
	
	return TRUE;
}

//...............................................................................................................
// ���� 
//...............................................................................................................
int CEffectManager::MAKE_EF_ATTACKED_FOCUS(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id			    , 0 , 2.1f , pSrc->GetUnique()  , pSrc->GetIndex() );
	
	// ����Ʈ ������ �ҽ��ʸ� ����Ʈ ���
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	g_Particle.SetEffSetCon( 0.0f , EF_ATTACK_FOCUS , 0 , 2.1f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
	
	pExtraInfo->AfterEffect = EF_ATTACK_FOCUS;
	pExtraInfo->DestType = 0;
	pExtraInfo->lifeTime = 2.1f;
	
	EffectTargetHit( pSrc , pDest , pExtraInfo );
	
	//................................................................................	
	// �ߵ� ����
	//................................................................................	
	if( pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	
	return TRUE;
}

//..............................................................................................................
// ���̽� ����
//..............................................................................................................
int CEffectManager::MAKE_EF_ICE_RAIN(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	CEffectBase *pData = SAFE_NEW( CEFIceRain );					// Ŭ���� ����	
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );					// �ҽ� , Ÿ�� ����
	
	PushLandEffectBase( &pData );									// �޴����� �ֱ�
	
	return TRUE;
}


int CEffectManager::MAKE_EF_FIRE_RAIN(EFFECT_ID id, Character *pSrc , Character *pDest , LPEFFECT_EXTRA_INFO pExtraInfo , float Timer)
{
	CEffectBase *pData = SAFE_NEW( CEFFireRain );
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );
	
	PushLandEffectBase( &pData ) ; 
	
	return TRUE;
}





//..............................................................................................................
// ��������
//..............................................................................................................
int CEffectManager::MAKE_EF_BIG_PRESS(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	CEffectBase *pData = SAFE_NEW( CEFBigPress );			// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );			// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );									// �޴����� �ֱ�
	
	return TRUE;
}

//................................................................................................................
// ������Ʈ ����
//................................................................................................................
int CEffectManager::MAKE_EF_PRIEST_DISPELL(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id			 , 0 , 4.1f , pSrc->GetUnique() , pSrc->GetIndex() );
	
	// ����Ʈ ������ �ҽ��ʸ� ����Ʈ ���
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	g_Particle.SetEffSetCon( 0.0f , EF_DISPELLED , 0 , 4.1f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
	
	pExtraInfo->AfterEffect = EF_DISPELLED;
	pExtraInfo->DestType = 0;
	pExtraInfo->lifeTime = 4.1f;
	
	EffectTargetHit( pSrc , pDest , pExtraInfo );
	
	return TRUE;
}

//................................................................................................................
// ���� ����
//................................................................................................................
int CEffectManager::MAKE_EF_DISPELL_BAD(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , EF_DISPELL_BAD , 0 , 2.1f , pSrc->GetUnique() , pSrc->GetIndex() );
	
	// ����Ʈ ������ �ҽ��ʸ� ����Ʈ ���
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	g_Particle.SetEffSetCon( 0.0f , EF_DISPELL_BAD_ED , 0 , 3.1f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
	
	pExtraInfo->AfterEffect = EF_DISPELL_BAD_ED;
	pExtraInfo->DestType = 0;
	pExtraInfo->lifeTime = 3.1f;
	
	EffectTargetHit( pSrc , pDest , pExtraInfo );
	
	g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	
	return TRUE;
}

//................................................................................................................
// ����
//................................................................................................................
int CEffectManager::MAKE_EF_BRESS(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 2.1f , pSrc->GetUnique() , pSrc->GetIndex() );
	
	// ����Ʈ ������ �ҽ��ʸ� ����Ʈ ���
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	EffectTargetHit( pSrc , pDest , pExtraInfo );
	
	g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	
	return TRUE;
}

//...............................................................................................................
// ���� ȭ�̾ ����
//...............................................................................................................
int CEffectManager::MAKE_EF_NPC_FIRE(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// ����Ʈ ������ �ҽ��ʸ� ����Ʈ ���
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFNpcFireBall );					// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );												// �޴����� �ֱ�
	
	return TRUE;
}

//...............................................................................................................
// NPC ¯�� ����
//...............................................................................................................
int CEffectManager::MAKE_EF_ROCK_SHOT(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// ����Ʈ ������ �ҽ��ʸ� ����Ʈ ���
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFNpcStoneAttack );					// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );												// �޴����� �ֱ�
	
	return TRUE;
}

//..............................................................................................................
// ĥ�� ��ġ
//..............................................................................................................
int CEffectManager::MAKE_EF_CR_TOUCH(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pDest == NULL )
	{
		return TRUE;
	}
	
	//................................................................................	
	// ��ų �ߵ� ����
	//................................................................................	
	if( pExtraInfo != NULL && pSrc != NULL  )
		g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	
	switch( id ) 
	{
	case EF_CR_TOUCH0:
		g_Particle.SetEffSetCon( 0.0f , EF_CR_TOUCH_ED0, 0 , 4.1f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
		pExtraInfo->AfterEffect = EF_CR_TOUCH_ED0;
		pExtraInfo->DestType = 0;
		pExtraInfo->lifeTime = 4.1f;
		break;
	case EF_CR_TOUCH1:
		g_Particle.SetEffSetCon( 0.0f , EF_CR_TOUCH_ED1, 0 , 4.1f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
		pExtraInfo->AfterEffect = EF_CR_TOUCH_ED1;
		pExtraInfo->DestType = 0;
		pExtraInfo->lifeTime = 4.1f;
		break;
	case EF_CR_TOUCH2:
		g_Particle.SetEffSetCon( 0.0f , EF_CR_TOUCH_ED2, 0 , 4.1f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
		pExtraInfo->AfterEffect = EF_CR_TOUCH_ED2;
		pExtraInfo->DestType = 0;
		pExtraInfo->lifeTime = 4.1f;
		break;
	}
	
	EffectTargetHit( pSrc , pDest , pExtraInfo );
	
	//................................................................................	
	// ���� ����
	//................................................................................	
	if( pExtraInfo != NULL && pDest != NULL )
		g_Particle.m_EffectManager.GetSkillAttackedSound( pExtraInfo->SkillCode , pDest );
	
	return TRUE;
}

//..............................................................................................................
// ���� ���̺�
//..............................................................................................................
int CEffectManager::MAKE_EF_HEAL_WAVE(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( CheckCharacter( pSrc , pDest ) == FALSE )
		return TRUE;
	
	//................................................................................	
	// ��ų �ߵ� ����
	//................................................................................	
	if( pExtraInfo != NULL && pSrc != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	
	switch( id )
	{
	case EF_HEAL_WAVE0:
		g_Particle.SetEffSetCon( 0.0f , EF_HEAL_WAVE_ED0 , 0 , 4.1f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
		pExtraInfo->AfterEffect = EF_HEAL_WAVE_ED0;
		pExtraInfo->DestType = 0;
		pExtraInfo->lifeTime = 4.1f;
		break;
	case EF_HEAL_WAVE1:
		g_Particle.SetEffSetCon( 0.0f , EF_HEAL_WAVE_ED1 , 0 , 4.1f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
		pExtraInfo->AfterEffect = EF_HEAL_WAVE_ED1;
		pExtraInfo->DestType = 0;
		pExtraInfo->lifeTime = 4.1f;			
		break;
	case EF_HEAL_WAVE2:
		g_Particle.SetEffSetCon( 0.0f , EF_HEAL_WAVE_ED2 , 0 , 4.1f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
		pExtraInfo->AfterEffect = EF_HEAL_WAVE_ED2;
		pExtraInfo->DestType = 0;
		pExtraInfo->lifeTime = 4.1f;
		break;
	}
	
	EffectTargetHit( pSrc , pDest , pExtraInfo );
	
	//................................................................................	
	// ���� ����
	//................................................................................	
	if( pExtraInfo != NULL && pDest != NULL )
		g_Particle.m_EffectManager.GetSkillAttackedSound( pExtraInfo->SkillCode , pDest );
	
	return TRUE;
}

//..............................................................................................................
// ����
//..............................................................................................................
int CEffectManager::MAKE_EF_SELF_SHOUT(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	//................................................................................	
	// ��ų �ߵ� ����
	//................................................................................	
	if( pExtraInfo != NULL && pSrc != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	
	if( pSrc == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 4.1f , pSrc->GetUnique() , pSrc->GetIndex() );
	EffectTargetHit( pSrc , pDest , pExtraInfo );
	
	//................................................................................	
	// �ߵ� ����
	//................................................................................	
	if( pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	
	return TRUE;
}

//..............................................................................................................
// ���� 
//..............................................................................................................
int CEffectManager::MAKE_EF_HIDE(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	CEffectBase *pData = SAFE_NEW( CEFHide );							// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );												// �޴����� �ֱ�
	
	return TRUE;
}

//..............................................................................................................
// ������ ��
//..............................................................................................................
int CEffectManager::MAKE_EF_BOW_RAIN(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	CEffectBase *pData = SAFE_NEW( CEFBowRain );						// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );												// �޴����� �ֱ�
	
	return TRUE;
}

//..............................................................................................................
// ġ���� ��
//..............................................................................................................
int CEffectManager::MAKE_EF_HEAL_RAIN_BOW(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	CEffectBase *pData = SAFE_NEW( CEFHealRainBow );					// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );												// �޴����� �ֱ�
	
	return TRUE;
}


//..............................................................................................................
// ����
//..............................................................................................................
int CEffectManager::MAKE_EF_MOMENT_HIDE	(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	CEffectBase *pData = SAFE_NEW( CEFMomentHide );						// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );												// �޴����� �ֱ�
	
	return TRUE;
}

//..............................................................................................................
// ����
//..............................................................................................................
int CEffectManager::MAKE_EF_SCAPMPER(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	CEffectBase *pData = SAFE_NEW( CEFScapmper );						// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );												// �޴����� �ֱ�
	
	return TRUE;
}

//..............................................................................................................
// ������ �õ�
//..............................................................................................................
int CEffectManager::MAKE_EF_POISON_SEED(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	//................................................................................	
	// ��ų �ߵ� ����
	//................................................................................	
	if( pExtraInfo != NULL && pSrc != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	
	if( pDest == NULL )
		return TRUE;
	
	switch( id )
	{
	case EF_POISON_SEED0:
		g_Particle.SetEffSetCon( 0.0f , EF_POISON_SEED_ED0 , 0 , 4.1f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
		pExtraInfo->AfterEffect = EF_POISON_SEED_ED0;
		pExtraInfo->DestType = 0;
		pExtraInfo->lifeTime = 4.1f;
		break;
	case EF_POISON_SEED1:
		g_Particle.SetEffSetCon( 0.0f , EF_POISON_SEED_ED1 , 0 , 4.1f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
		pExtraInfo->AfterEffect = EF_POISON_SEED_ED1;
		pExtraInfo->DestType = 0;
		pExtraInfo->lifeTime = 4.1f;
		break;
	case EF_POISON_SEED2:
		g_Particle.SetEffSetCon( 0.0f , EF_POISON_SEED_ED2 , 0 , 4.1f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
		pExtraInfo->AfterEffect = EF_POISON_SEED_ED2;
		pExtraInfo->DestType = 0;
		pExtraInfo->lifeTime = 4.1f;
		break;
	}
	
	EffectTargetHit( pSrc , pDest , pExtraInfo );
	
	//................................................................................	
	// ���� ����
	//................................................................................	
	if( pExtraInfo != NULL && pDest != NULL )
		g_Particle.m_EffectManager.GetSkillAttackedSound( pExtraInfo->SkillCode , pDest );
	
	return TRUE;
}

//..............................................................................................................
// �Ͽ���
//..............................................................................................................
int CEffectManager::MAKE_EF_MAYUNSAL(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	//................................................................................	
	// ��ų �ߵ� ����
	//................................................................................	
	if( pExtraInfo != NULL && pSrc != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	
	if( pSrc != NULL )
		g_Particle.SetEffSetCon( 0.0f , id			   , 0 , 4.1f , pSrc->GetUnique()  , pSrc->GetIndex() );
	
	if( pDest != NULL )
	{
		g_Particle.SetEffSetCon( 0.0f , EF_MAYUNSAL_ED , 0 , 4.1f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
		pExtraInfo->AfterEffect = EF_MAYUNSAL_ED;
		pExtraInfo->DestType = 0;
		pExtraInfo->lifeTime = 4.1f;
	}
	//................................................................................	
	// ���� ����
	//................................................................................	
	if( pExtraInfo != NULL && pDest != NULL )
		g_Particle.m_EffectManager.GetSkillAttackedSound( pExtraInfo->SkillCode , pDest );
	
	EffectTargetHit( pSrc , pDest , pExtraInfo );
	
	return TRUE;
}

//..............................................................................................................
// �޸��� ����Ʈ
//..............................................................................................................
int CEffectManager::MAKE_EF_PEAL_DUST(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	//................................................................................	
	// ��ų �ߵ� ����
	//................................................................................	
	if( pExtraInfo != NULL && pSrc != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	
	EffectTargetHit( pSrc , pDest , pExtraInfo );
	
	//................................................................................	
	// ���� ����
	//................................................................................	
	if( pExtraInfo != NULL && pDest != NULL )
		g_Particle.m_EffectManager.GetSkillAttackedSound( pExtraInfo->SkillCode , pDest );
	
	return TRUE;
}

//...............................................................................................................
// NPC ���� ����
//...............................................................................................................
int CEffectManager::MAKE_EF_FRUIT_ATTACK(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// ����Ʈ ������ �ҽ��ʸ� ����Ʈ ���
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFFruitAttack );					// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );												// �޴����� �ֱ�
	
	return TRUE;
}

//...............................................................................................................
// NPC �� -> ��
//...............................................................................................................
int CEffectManager::MAKE_EF_MOUTH_FIRE(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// ����Ʈ ������ �ҽ��ʸ� ����Ʈ ���
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFMouthFire );  					// Ŭ���� ����
	
	switch( id )
	{
	case EF_MOUTH_FIRE:
		(( CEFMouthFire * )pData)->SetInit( PT_NPC_MOUTH_FIRE_BALL, PT_NPC_MOUTH_FIRE_BALL_TAIL,
			70,1,EF_MOUTH_FIREED);
		break;
		
	case EF_MOUTH_FIRE1:
		(( CEFMouthFire * )pData)->SetInit( PT_NPC_MOUTH_FIRE_BALL1, PT_NPC_MOUTH_FIRE_BALL_TAIL1,
			70,1,EF_MOUTH_FIREED1);
		break;
	}
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );												// �޴����� �ֱ�
	
	return TRUE;
}

//...............................................................................................................
// ���󰡴� ��ų ����Ʈ ����
//...............................................................................................................
int CEffectManager::MakeCommonFlySkill(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,
									   int FrontNum,int FrontType,
									   int TailNum ,int TailType,
									   int AttackedNum,DWORD RangeTime,int BoneNum,BOOL ISSKILL)
{
	if( pExtraInfo == NULL || pDest == NULL || pSrc == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFCommonFlySkill );					// Ŭ���� ����
	
	(( CEFCommonFlySkill * )pData)->SetInit(
		FrontNum , ( CEFCommonFlySkill::CommonFlySkill_Effect_Type ) FrontType , 
		TailNum  , ( CEFCommonFlySkill::CommonFlySkill_Effect_Type ) TailType  ,
		AttackedNum,RangeTime,pSrc,BoneNum , ISSKILL );
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );												// �޴����� �ֱ�
	
	return TRUE;
}


//...............................................................................................................
// NPC ���� ����
//...............................................................................................................
int CEffectManager::MAKE_EF_GAS_SHOT(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// ����Ʈ ������ �ҽ��ʸ� ����Ʈ ���
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFMouthFire );						// Ŭ���� ����
	
	(( CEFMouthFire * )pData)->SetInit( PT_NPC_MOUTH_GAS, PT_NPC_MOUTH_GAS_TAIL ,
		70,1,EF_GAS_SHOTED);
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );												// �޴����� �ֱ�
	
	return TRUE;
}

//...............................................................................................................
// NPC ����� �ձ�
//...............................................................................................................
int CEffectManager::MAKE_EF_NPC_BUBBLE_ATTACK(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// ����Ʈ ������ �ҽ��ʸ� ����Ʈ ���
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFNPCBubble );						// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );												// �޴����� �ֱ�
	
	return TRUE;
}

//...............................................................................................................
// NPC ������ ���� ( �⺻ ���� )
//...............................................................................................................
int CEffectManager::MAKE_EF_NPC_STICK(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// ����Ʈ ������ �ҽ��ʸ� ����Ʈ ���
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFNPCStickAttack );					// Ŭ���� ����	
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );												// �޴����� �ֱ�
	
	return TRUE;
}

//...............................................................................................................
// NPC ������ ���� ( �⺻ ���� )
//...............................................................................................................
int CEffectManager::MAKE_EF_NPC_STICK_ATTACKED(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pDest == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 4.1f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
	
	return TRUE;
}


//...............................................................................................................
// 	ŵ�� ���� 
//...............................................................................................................
int CEffectManager::MAKE_EF_KEEPUP_PUMP(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// ����Ʈ ������ �ҽ��ʸ� ����Ʈ ���
	CEffectBase *pData = SAFE_NEW( CEFKeepUpPump );						// Ŭ���� ����
	
	( ( CEFKeepUpPump * ) pData )->SetTimer( Timer );
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );												// �޴����� �ֱ�
	
	//................................................................................	
	// �ߵ� ����
	//................................................................................	
	if( pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	
	return TRUE;
}


//................................................................................................................
// ���� �ٴ� ����Ʈ
//................................................................................................................
int CEffectManager::MAKE_EF_SRC_LINK(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// ����Ʈ ������ �ҽ��ʸ� ����Ʈ ���
	CEffectBase *pData = SAFE_NEW( CEFSrcLink );						// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );												// �޴����� �ֱ�
	
	return TRUE;
}


//................................................................................................................
// �Ŵ����� ���� �ٴ� ����Ʈ
//................................................................................................................
int CEffectManager::MAKE_EF_GIANTUNIT_SRC_LINK(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// ����Ʈ ������ �ҽ��ʸ� ����Ʈ ���
	CEffectBase *pData = SAFE_NEW( CEFSrcLink );						// Ŭ���� ����
	
	((CEFSrcLink *)pData)->m_IsGiantUnit = TRUE;
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );	
	
	return TRUE;
}

///-- ����Ÿ�� ���� ��������Ʈ
int CEffectManager::MAKE_G_TOWER_EFFECT(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo, int nSrcBonNum , UINT uBillType, 
										UINT uTailType, UINT uAttackedEffId, UINT uAttackedType)
{
	CEffectBase *pData = SAFE_NEW( CEFGTEffect );	
	
	((CEFGTEffect *)pData)->m_nSrcBoneNum = nSrcBonNum;
	((CEFGTEffect *)pData)->m_uBillType = uBillType;	
	((CEFGTEffect *)pData)->m_uTaleType = uTailType;
	((CEFGTEffect *)pData)->m_uAttackedEffect = uAttackedEffId;
	((CEFGTEffect *)pData)->m_uAttackedType = uAttackedType;
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );	
	
	return TRUE;
}

///-- �Ŵ���� ��������Ʈ
int CEffectManager::MAKE_CANNON_EFECT(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo, int nImpacFrame, int nSrcBonNum,
									  UINT uBillType, UINT uTailType, UINT uGetEffId, UINT uGetEffType, UINT uPercuEffId, UINT uPercuEffType, UINT uExploEffId, UINT uExploEffType, 
									  UINT uAttackedEffId, UINT uAttackedType)
{
	CEffectBase *pData = SAFE_NEW( CEFCannonEffect );
	
	((CEFCannonEffect *)pData)->m_nImpacFrame = nImpacFrame;
	((CEFCannonEffect *)pData)->m_nSrcBoneNum = nSrcBonNum;
	((CEFCannonEffect *)pData)->m_uBillType = uBillType;	
	((CEFCannonEffect *)pData)->m_uTaleType = uTailType;
	((CEFCannonEffect *)pData)->m_uEnergyGetEffId = uGetEffId;
	((CEFCannonEffect *)pData)->m_uEnergyGetType = uGetEffType;
	((CEFCannonEffect *)pData)->m_uPercuEffId = uPercuEffId;
	((CEFCannonEffect *)pData)->m_uPercuEffType = uPercuEffType;
	((CEFCannonEffect *)pData)->m_uExploEffId = uExploEffId;
	((CEFCannonEffect *)pData)->m_uExploEffType = uExploEffType;
	((CEFCannonEffect *)pData)->m_uAttackedEffect = uAttackedEffId;
	((CEFCannonEffect *)pData)->m_uAttackedType = uAttackedType;
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );
	
	return TRUE;
}

///-- �Žź� ���� ����Ʈ
int CEffectManager::MAKE_GOLEM_EFFECT(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo, int nMImpacFrame, int nImpacFrame,
									  UINT uTailType, UINT uRightEffId, UINT uRightEffType, UINT uLeftEffId, UINT uLeftEffType, INT uExploEffId, 
									  UINT uExploEffType, UINT uAttackedEffId, UINT uAttackedType)
{
	CEffectBase *pData = SAFE_NEW( CEFGolemEffect );	
	
	((CEFGolemEffect *)pData)->m_nMiddleImpacFrame = nMImpacFrame;
	((CEFGolemEffect *)pData)->m_nLastImpacFrame = nImpacFrame;	
	((CEFGolemEffect *)pData)->m_uTaleType = uTailType;
	((CEFGolemEffect *)pData)->m_uRightHandEffId = uRightEffId;
	((CEFGolemEffect *)pData)->m_uRightHandType = uRightEffType;
	((CEFGolemEffect *)pData)->m_uLeftHandEffId = uLeftEffId;
	((CEFGolemEffect *)pData)->m_uLeftHandType = uLeftEffType;
	((CEFGolemEffect *)pData)->m_uExploEffId = uExploEffId;
	((CEFGolemEffect *)pData)->m_uExploEffType = uExploEffType;
	((CEFGolemEffect *)pData)->m_uAttackedEffect = uAttackedEffId;
	((CEFGolemEffect *)pData)->m_uAttackedType = uAttackedType;
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );	
	
	return TRUE;
}

//................................................................................................................
// ������ ��
//................................................................................................................
int CEffectManager::MAKE_EF_WARM_LIGHTED(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 4.1f , pSrc->GetUnique() , pSrc->GetIndex() );
	
	return TRUE;
}

int CEffectManager::MAKE_EF_NPC_FIREED(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 2.1f , pSrc->GetUnique() , pSrc->GetIndex() );
	
	return TRUE;
}

//...............................................................................................................
// 	������Ʈ ����
//...............................................................................................................
int CEffectManager::MAKE_EF_PRIEST_HEAL(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 4.1f , pSrc->GetUnique() , pSrc->GetIndex() );
	
	//................................................................................	
	// ��ų �ߵ� ����
	//................................................................................	
	g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	
	// ����Ʈ ������ �ҽ��ʸ� ����Ʈ ���
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	switch( id )
	{
	case EF_HEAL0:
		g_Particle.SetEffSetCon( 0.0f , EF_HEALED0 , 0 , 4.1f , pDest->GetUnique() , pDest->GetIndex(),
			::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
		pExtraInfo->AfterEffect = EF_HEALED0;
		pExtraInfo->DestType = 0;
		pExtraInfo->lifeTime = 4.1f;
		
		break;
		
	case EF_HEAL1:
		g_Particle.SetEffSetCon( 0.0f , EF_HEALED1 , 0 , 4.1f , pDest->GetUnique() , pDest->GetIndex(),
			::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc));
		pExtraInfo->AfterEffect = EF_HEALED1;
		pExtraInfo->DestType = 0;
		pExtraInfo->lifeTime = 4.1f;
		
		break;
		
	case EF_HEAL2:
		g_Particle.SetEffSetCon( 0.0f , EF_HEALED2 , 0 , 4.1f , pDest->GetUnique() , pDest->GetIndex(),
			::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
		pExtraInfo->AfterEffect = EF_HEALED2;
		pExtraInfo->DestType = 0;
		pExtraInfo->lifeTime = 4.1f;
		
		break;
	}
	
	EffectTargetHit( pSrc , pDest , pExtraInfo );
	
	//................................................................................	
	// ���� ����
	//................................................................................	
	if( pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillAttackedSound( pExtraInfo->SkillCode , pDest );
	
	return TRUE;
}

//...............................................................................................................
// �����
//...............................................................................................................
int CEffectManager::MAKE_EF_CIRCLE_SWORD(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 2.1f , pSrc->GetUnique() , pSrc->GetIndex() );
	
	//................................................................................	
	// ��ų �ߵ� ����
	//................................................................................	
	g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	
	
	// ����Ʈ ������ �ҽ��ʸ� ����Ʈ ���
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	// 1:N ������
	int Count = pExtraInfo->GetTargetCount();
	
	Character			*pTempDest = NULL;
	EFFECT_EXTRA_INFO    Info;
	
	for( int i = 0 ; i < Count ; ++i )
	{
		if( pExtraInfo->GetTargetInfo( &pTempDest , &Info , i ) )
		{
			g_Particle.SetEffSetCon( 0.0f , EF_CIRCLE_SWORDED , 0 , 2.6f , 
				pTempDest->GetUnique() , pTempDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
			
			Info.AfterEffect = EF_CIRCLE_SWORDED;
			Info.DestType = 0;
			Info.lifeTime = 2.6f;
			
			EffectTargetHit( pSrc , pTempDest , &Info );
		}
	}
	
	//................................................................................	
	// ���� ����
	//................................................................................	
	if( pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillAttackedSound( pExtraInfo->SkillCode , pDest );
	
	return TRUE;
}

//...............................................................................................................
// 	������Ʈ ����� ����
//...............................................................................................................
int CEffectManager::MAKE_EF_LUCKY_GOD(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id			    , 0 , 2.1f , pSrc->GetUnique()  , pSrc->GetIndex() );
	//................................................................................	
	// ��ų �ߵ� ����
	//................................................................................	
	g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	
	// ����Ʈ ������ �ҽ��ʸ� ����Ʈ ���
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	
	g_Particle.SetEffSetCon( 0.0f , EF_LUCKY_GODED   , ESLT_CHARPOS , 4.1f , pDest->GetUnique()  , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
	
	pExtraInfo->AfterEffect = EF_LUCKY_GODED;
	pExtraInfo->DestType = ESLT_CHARPOS;
	pExtraInfo->lifeTime = 4.1f;
	
	EffectTargetHit( pSrc , pDest , pExtraInfo );
	//................................................................................	
	// ���� ����
	//................................................................................	
	if( pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillAttackedSound( pExtraInfo->SkillCode , pDest );
	
	
	return TRUE;
}

//...............................................................................................................
// 	������Ʈ ����� ���� ( ���ϴ� �� )
//...............................................................................................................
int CEffectManager::MAKE_EF_LUCKY_GODED(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id			   , 0 , 2.1f , pSrc->GetUnique()  , pSrc->GetIndex() );
	
	return TRUE;
}



//................................................................................................................
// ȸ��
//................................................................................................................
int CEffectManager::MAKE_EF_AVOID0(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// ����Ʈ ������ �ҽ��ʸ� ����Ʈ ���
	if( pSrc == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFAvoid );							// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );												// �޴����� �ֱ�
	
	//................................................................................	
	// �ߵ� ����
	//................................................................................	
	if( pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	
	return TRUE;
}

//................................................................................................................
// ����Ʈ��
//................................................................................................................
int CEffectManager::MAKE_EF_LIGHTNING0(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// ����Ʈ ������ �ҽ��ʸ� ����Ʈ ���
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFLightning );						// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );												// �޴����� �ֱ�
	
	return TRUE;
}

//................................................................................................................
// ȭ�̾� ��
//................................................................................................................
int CEffectManager::MAKE_EF_FIREBALL0(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// ����Ʈ ������ �ҽ��ʸ� ����Ʈ ���
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFFireBall );						// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );												// �޴����� �ֱ�
	
	return TRUE;
}


//................................................................................................................
// ȸ�� ������ 2 ( ���Ӽ� )
//................................................................................................................
int CEffectManager::MAKE_EF_RECOVERY_ITEM2_0(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	CEffectBase *pData = SAFE_NEW( CEFRecoveryItem2 );					// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );												// �޴����� �ֱ�
	
	return TRUE;
}

//................................................................................................................
// ���̾� �Ƹ� ( ���Ӽ� )
//................................................................................................................
int CEffectManager::MAKE_EF_FIRE_ARMOR(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	CEffectBase *pData = SAFE_NEW( CEFFireArmor );						// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );												// �޴����� �ֱ�
	
	return TRUE;
}

//................................................................................................................
// ���̽� �ǵ� ( ���Ӽ� )
//................................................................................................................
int CEffectManager::MAKE_EF_ICE_SHILED(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	CEffectBase *pData = SAFE_NEW( CEFIceShiled );						// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );												// �޴����� �ֱ�
	
	return TRUE;
}

//................................................................................................................
// �� ������
//................................................................................................................
int CEffectManager::MAKE_EF_ROCKPRESS0(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// ����Ʈ ������ �ҽ��ʸ� ����Ʈ ���
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFRockPress );						// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );												// �޴����� �ֱ�
	
	return TRUE;
}

//................................................................................................................
// ��� �ҵ�
//................................................................................................................
int CEffectManager::MAKE_EF_WOODENSWORD0(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// ����Ʈ ������ �ҽ��ʸ� ����Ʈ ���
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFWoodenSword );					// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );												// �޴����� �ֱ�
	
	return TRUE;
}

//................................................................................................................
// �� �ǵ�
//................................................................................................................
int CEffectManager::MAKE_EF_ROCK_SHIELD0(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	CEffectBase *pData = SAFE_NEW( CEFRockShield );						// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );												// �޴����� �ֱ�
	
	return TRUE;
}

//................................................................................................................
// ���
//................................................................................................................
int CEffectManager::MAKE_EF_REFRESH_SWORD0(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	CEffectBase *pData = SAFE_NEW( CEFRefreshSword );					// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );												// �޴����� �ֱ�
	
	//................................................................................	
	// �ߵ� ����
	//................................................................................	
	if( pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	
	return TRUE;
}

//...............................................................................................................
// 	�޾�ġ��
//...............................................................................................................
int CEffectManager::MAKE_EF_BOUND_ATTACK(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	// ����Ʈ ������ �ҽ��ʸ� ����Ʈ ���
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	g_Particle.SetEffSetCon( 0.0f , id , ESLT_CHARPOS , 2.6f , pSrc->GetUnique() , pSrc->GetIndex() );
	
	pExtraInfo->AfterEffect = id;
	pExtraInfo->DestType = ESLT_CHARPOS;
	pExtraInfo->lifeTime = 2.6f;
	
	EffectTargetHit( pSrc , pDest , pExtraInfo );
	
	return TRUE;
}

//................................................................................................................
// ����
//................................................................................................................
int CEffectManager::MAKE_EF_STRONG_SWORD0(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	CEffectBase *pData = SAFE_NEW( CEFStrongSword );					// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );												// �޴����� �ֱ�
	
	return TRUE;
}

//................................................................................................................
// �������
//................................................................................................................
int CEffectManager::MAKE_EF_V_CUT_ATTACK0(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	//g_Particle.SetEffSetCon( 0.0f , EF_V_CUT_ATTACK0  , 0 , 1.9f , pSrc->GetUnique() , pSrc->GetIndex() );
	
	// ����Ʈ ������ �ҽ��ʸ� ����Ʈ ���
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	int nCount = pExtraInfo->GetTargetCount();
	
	g_Particle.SetEffSetCon( 0.0f , EF_V_CUT_ATTACKED , ESLT_SRC_RAD , 2.6f , pDest->GetUnique() , 
		pDest->GetIndex() , ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) , pSrc->GetDirection() + D3DXToRadian( 180 ) );
	
	pExtraInfo->AfterEffect = EF_V_CUT_ATTACKED;
	pExtraInfo->DestType = ESLT_SRC_RAD;
	pExtraInfo->lifeTime = 2.6f;
	
	EffectTargetHit( pSrc , pDest , pExtraInfo );
	
	return TRUE;
}

//................................................................................................................
// �޼� ����
//................................................................................................................
int CEffectManager::MAKE_EF_CAP_SWORD_ATTACK0(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	// ����Ʈ ������ �ҽ��ʸ� ����Ʈ ���
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	g_Particle.SetEffSetCon( 0.0f , EF_CAP_SWORD_ATTTACKED , ESLT_SRC_RAD , 2.5f , 
		pDest->GetUnique() , pDest->GetIndex() , ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) , pSrc->GetDirection() + D3DXToRadian( 180 ) );
	
	pExtraInfo->AfterEffect = EF_CAP_SWORD_ATTTACKED;
	pExtraInfo->DestType = ESLT_SRC_RAD;
	pExtraInfo->lifeTime = 2.5f;
	
	EffectTargetHit( pSrc , pDest , pExtraInfo );
	
	return TRUE;
}

//............................................................................................................
// ���� ����
//............................................................................................................
int CEffectManager::MAKE_EF_POSSESSED_CANCEL(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , EF_POSSESSED_CANCEL , 0 , 2.1f , pSrc->GetUnique() , pSrc->GetIndex() );
	
	Delete( EF_APPEAR_BING0 , g_Pc.GetPlayer() , NULL );
	Delete( EF_APPEAR_BING1 , g_Pc.GetPlayer() , NULL );
	Delete( EF_APPEAR_BING2 , g_Pc.GetPlayer() , NULL );
	Delete( EF_APPEAR_BING3 , g_Pc.GetPlayer() , NULL );
	Delete( EF_APPEAR_BING4 , g_Pc.GetPlayer() , NULL );
	
	return TRUE;
}

//................................................................................................................
// ȭ�� ��Ÿ
//................................................................................................................
int CEffectManager::MAKE_EF_FIRE_SMACK(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// ����Ʈ ������ �ҽ��ʸ� ����Ʈ ���
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFFireSmack );						// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );												// �޴����� �ֱ�
	
	return TRUE;
}


//................................................................................................................
// ��� ����
//................................................................................................................
int CEffectManager::MAKE_EF_GATHER_CUT0(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// ����Ʈ ������ �ҽ��ʸ� ����Ʈ ���
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFGatherCut );						// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );												// �޴����� �ֱ�
	
	return TRUE;
}

//................................................................................................................
// ���� ũ����
//................................................................................................................
int CEffectManager::MAKE_EF_BUBBLE_CRASH0(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// ����Ʈ ������ �ҽ��ʸ� ����Ʈ ���
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFBubbleCrash );					// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );												// �޴����� �ֱ�
	
	return TRUE;
}


//................................................................................................................
// ���̽� ���Ǿ�
//................................................................................................................
int CEffectManager::MAKE_EF_ICESPIRE0(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// ����Ʈ ������ �ҽ��ʸ� ����Ʈ ���
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFIceSpire );						// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );												// �޴����� �ֱ�
	
	return TRUE;
}

//................................................................................................................
// ȸ�� ������ 1 ( ��ȿ�� )
//................................................................................................................
int CEffectManager::MAKE_EF_RECOVERY_ITEM1_0(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 4.1f , pSrc->GetUnique() , pSrc->GetIndex() );
	
	return TRUE;
}

//............................................................................................................
// �⺻ Ȱ ���� ������ ����Ʈ
//............................................................................................................
int CEffectManager::MAKE_EF_BOW_ATTACKED0(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL || pDest == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , ESLT_SRC_RAD , 3.1f , pDest->GetUnique() , 
		pDest->GetIndex() , ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) , pSrc->GetDirection() + D3DXToRadian( 180 ) );
	
	return TRUE;
}

//............................................................................................................
// ������Ʈ �⺻ ���� 
//............................................................................................................
int CEffectManager::MAKE_EF_PRIEST_STAND_ATTACK(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	CEffectBase *pData = SAFE_NEW( CEFPriestStandAttack );				// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );												// �޴����� �ֱ�
	
	return TRUE;
}

//............................................................................................................
// ������Ʈ �⺻ ���� ������
//............................................................................................................
int CEffectManager::MAKE_EF_PRIEST_STAND_ATTACKED(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL || pDest == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 2.1f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
	
	return TRUE;
}


//............................................................................................................
// ������
//............................................................................................................
int CEffectManager::MAKE_EF_GLAER_BOW_ATTACK(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// ����Ʈ ������ �ҽ��ʸ� ����Ʈ ���
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFGlareBow );						// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );												// �޴����� �ֱ�
	
	
	return TRUE;
}


//............................................................................................................
// ���� 
//............................................................................................................
int CEffectManager::MAKE_EF_DOUBLESHOT(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// ����Ʈ ������ �ҽ��ʸ� ����Ʈ ���
	if(  pDest == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFBowDoubleShot );					// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );												// �޴����� �ֱ�
	
	
	return TRUE;
}


//..............................................................................................................
// ����
//..............................................................................................................
int CEffectManager::MAKE_EF_SNIPER_BOW(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// ����Ʈ ������ �ҽ��ʸ� ����Ʈ ���
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFSniperBow );						// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );												// �޴����� �ֱ�
	
	return TRUE;
}

//............................................................................................................
// �ߵ� ���
//............................................................................................................
int CEffectManager::MAKE_EF_FOOT_BOW_ATTACK(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	CEffectBase *pData = SAFE_NEW( CEFFootBow );						// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );												// �޴����� �ֱ�
	
	
	return TRUE;
}

//............................................................................................................
// ����
//............................................................................................................
int CEffectManager::MAKE_EF_POSSESSED(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	CEffectBase *pData = SAFE_NEW( CEFPossessed );						// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );												// �޴����� �ֱ�
	
	return TRUE;
}

//............................................................................................................
// ���� ��Ÿ����
//............................................................................................................
int CEffectManager::MAKE_EF_APPEAR_BING(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	CEffectBase *pData = SAFE_NEW( CEFPossessedAppear );				// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );												// �޴����� �ֱ�
	
	
	return TRUE;
}

//............................................................................................................
// �ߵ� ��� ( ȭ�� ) ��ũ �Ȱ�
//............................................................................................................
int CEffectManager::MAKE_EF_FOOT_BOW_LINK(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	CEffectBase *pData = SAFE_NEW( CEFFootBowLink );							// Ŭ���� ����
	
	pData->Setup( ( EFFECT_ID ) EF_FOOT_BOW_LINK , pSrc , NULL , NULL );					// �ҽ� , Ÿ�� ����
	
	float fRad = D3DXToRadian( ( rand() % 360 ) );
	
	((CEFFootBowLink *)pData)->SetExtraInfo( fRad , D3DXVECTOR3( 0.0f , 1.0f , 0.0f ) , 7 );
	
	PushEffect( &pData );										// �޴����� �ֱ�
	
	return TRUE;
}

//...............................................................................................................
// 	����Ʈ ���� ���� ������
//...............................................................................................................
int CEffectManager::MAKE_EF_SHILED_ATTACKED(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// ����Ʈ ������ �ҽ��ʸ� ����Ʈ ���
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	int AttackedID = -1;
	
	switch( id )
	{
	case EF_SHILED_ATTACK0:
		AttackedID = EF_SHILED_ATTACEKD0;
		break;
		
	case EF_SHILED_ATTACK1:
		AttackedID = EF_SHILED_ATTACEKD1;
		break;
		
	case EF_SHILED_ATTACK2:
		AttackedID = EF_SHILED_ATTACEKD2;
		break;
	}
	
	g_Particle.SetEffSetCon( 0.0f , AttackedID , 0 , 2.1f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
	
	pExtraInfo->AfterEffect = AttackedID;
	pExtraInfo->DestType = 0;
	pExtraInfo->lifeTime = 2.1f;
	
	EffectTargetHit( pSrc , pDest , pExtraInfo );
	
	return TRUE;
}

//...............................................................................................................
// 	����� �� �⺻���� ����Ʈ
//...............................................................................................................
int	CEffectManager::MAKE_EF_DEFENSE_PET_BMOTION(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer, int nBonNum)
{
	
	CEffectBase *pData = SAFE_NEW( CEFCommonFlySkill );					// Ŭ���� ����
	
	(( CEFCommonFlySkill * )pData)->SetInit(
		PT_DEFENSE_PET , CEFCommonFlySkill::CommonFlySkill_Effect_Type::PARTICLE_LINK , 
		PT_FIREBALL_TAIL  , CEFCommonFlySkill::CommonFlySkill_Effect_Type::PARTICLE_SPOT  ,
		id ,70, pSrc, nBonNum , TRUE, FALSE );
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );
	//................................................................................	
	// �ߵ� ����
	//................................................................................	
	//	if( pExtraInfo != NULL )
	//		g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	
	int nDamage = pExtraInfo->TargetInfo.theEffectValue[ 0 ];
	
	// ������ ���� ���
	D3DXVECTOR3 vHeight = D3DXVECTOR3( 
		pSrc->GetPosTM()._41 ,
		pSrc->GetPosTM()._42 + ( pSrc->GetBoundMaxPos().y * 1.2f ) ,
		pSrc->GetPosTM()._43 );
	
	if(nDamage && ( pDest == g_Pc.GetPlayer()) )
		g_Particle.m_DamageNum.Add( vHeight  , nDamage , false, pSrc );
	
	return TRUE;
}

//..............................................................................................................
// ������ ��Ÿ����
//..............................................................................................................
int	CEffectManager::MAKE_EF_POPOI_ATTACK(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// ����Ʈ ������ �ҽ��ʸ� ����Ʈ ���
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFFPopoiAttack );					// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );
	
	
	return TRUE;
}

//..............................................................................................................
// ȣ���� �������� ���Ӱ��ݽ�ų
//..............................................................................................................
int	CEffectManager::MAKE_EF_TIGET_ATTACK(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// ����Ʈ ������ �ҽ��ʸ� ����Ʈ ���
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFTigerAttack );					// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );
	
	return TRUE;
}

//..............................................................................................................
// ���̽� �ʵ�
//..............................................................................................................
int	CEffectManager::MAKE_EF_ICE_FIELD(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	CEffectBase *pData = SAFE_NEW( CEFIceField );						// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// �ҽ� , Ÿ�� ����
	
	PushLandEffectBase( &pData );
	
	return TRUE;
}

//..............................................................................................................
// ���̽� ���Ǿ�
//..............................................................................................................
int	CEffectManager::MAKE_EF_ICE_SPEAR(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	//-- ����Ʈ ������ �ҽ��ʸ� ����Ʈ ���
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFIceSpear );						// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );
	
	return TRUE;
}

//..............................................................................................................
// ����Ʈ ����
//..............................................................................................................
int	CEffectManager::MAKE_EF_LIGHT_STORM(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	
	CEffectBase *pData = SAFE_NEW( CEFLightStorm );						// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// �ҽ� , Ÿ�� ����
	
	PushLandEffectBase( &pData );
	
	return TRUE;
}

//..............................................................................................................
// ��� ��
//..............................................................................................................
int	CEffectManager::MAKE_EF_STONE_RAIN(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	
	CEffectBase *pData = SAFE_NEW( CEFStoneRian );						// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// �ҽ� , Ÿ�� ����
	
	PushLandEffectBase( &pData );
	
	return TRUE;
}

//..............................................................................................................
// ���� ����
//..............................................................................................................
int	CEffectManager::MAKE_EF_WIND_BUSH(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	CEffectBase *pData = SAFE_NEW( CEFWindBush );						// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// �ҽ� , Ÿ�� ����
	
	PushLandEffectBase( &pData );
	
	return TRUE;
}

//..............................................................................................................
// ���� ���̾�
//..............................................................................................................
int	CEffectManager::MAKE_EF_ROUND_FIRE(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	CEffectBase *pData = SAFE_NEW( CEFRoundFire );						// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// �ҽ� , Ÿ�� ����
	
	PushLandEffectBase( &pData );	
	
	return TRUE;
}

//..............................................................................................................
// ���� ����Ʈ
//..............................................................................................................
int	CEffectManager::MAKE_EF_ROUND_LIGHT(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	CEffectBase *pData = SAFE_NEW( CEFRoundLight );						// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// �ҽ� , Ÿ�� ����
	
	PushLandEffectBase( &pData );
	
	return TRUE;
}

//..............................................................................................................
// ĳ��Ʈ ����
//..............................................................................................................
int	CEffectManager::MAKE_EF_CAST_STONE(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// ����Ʈ ������ �ҽ��ʸ� ����Ʈ ���
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFCastStone );						// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );
	
	return TRUE;
}

//..............................................................................................................
// ������ �ӹ�
//..............................................................................................................
int	CEffectManager::MAKE_EF_EARTH_BIND(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	CEffectBase *pData = SAFE_NEW( CEFEarthBind );						// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );
	
	return TRUE;
}

//..............................................................................................................
// ����
//..............................................................................................................
int	CEffectManager::MAKE_EF_RANDOM_ARROW(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{	
	CEffectBase *pData = SAFE_NEW( CEFRandomArrow );					// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );
	
	return TRUE;
}


//..............................................................................................................
// ���� �ִϸ��̼� ��ų
//..............................................................................................................
int CEffectManager::MAKE_EF_CONANISKILL(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// ����Ʈ ������ �ҽ��ʸ� ����Ʈ ���
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFConAniSkill );					// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );
	
	return TRUE;
}

//..............................................................................................................
// ��ǳ��Ÿ
//..............................................................................................................
int CEffectManager::MAKE_EF_FIRESTORM_SMASH(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// ����Ʈ ������ �ҽ��ʸ� ����Ʈ ���
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFFireStormSmash );					// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );	
	
	return TRUE;
}

//..............................................................................................................
// �⿭��Ÿ
//..............................................................................................................
int CEffectManager::MAKE_EF_SPIRIT_FIRE_SMACK(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// ����Ʈ ������ �ҽ��ʸ� ����Ʈ ���
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFSpiritFireSmack );				// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );
	
	return TRUE;
}

//..............................................................................................................
// �г��� ����
//..............................................................................................................
int CEffectManager::MAKE_EF_WRATH_KNIFE_ATTACK(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// ����Ʈ ������ �ҽ��ʸ� ����Ʈ ���
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFWrathKniteAttack );				// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );
	
	return TRUE;
}

//..............................................................................................................
// ��Ŭũ����
//..............................................................................................................
int CEffectManager::MAKE_EF_CIRCLE_CRASH(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// ����Ʈ ������ �ҽ��ʸ� ����Ʈ ���
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFCircleCrash );					// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );
	
	return TRUE;
}

//..............................................................................................................
// ���� ȭ��� 
//..............................................................................................................
int CEffectManager::MAKE_EF_ICE_BOW_RAIN(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	CEffectBase *pData = SAFE_NEW( CEFIceBowRain );					// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );					// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );											// �޴����� �ֱ�
	
	return TRUE;
}

//..............................................................................................................
// ���� ����
//..............................................................................................................
int CEffectManager::MAKE_EF_HIDE_SENSOR(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	CEffectBase *pData = SAFE_NEW( CEFHideSensor );					// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );					// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );											// �޴����� �ֱ�
	
	return TRUE;
}



//................................................................................................................
// ����Ʈ Ÿ�� ó��
//................................................................................................................
void CEffectManager::EffectTargetHit(Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo)
{
	if( pSrc == NULL )
	{
		return;
	}
	else
	{
		if( pSrc->m_nTableIndex == -1 )
		{
			return;
		}
	}
	
	if( pDest == NULL )
	{
		return;
	}
	else
	{
		if( pDest->m_nTableIndex == -1 )
		{
			return;
		}
	}
	
	if( NULL == pExtraInfo )
	{
		return;
	}	
	
	BOOL bGoodSkill = ISGoodSkill( pExtraInfo->SkillCode );	
	SSkillBaseInfo* skill_Info;
	
	skill_Info = g_Pc_Manager.GetSkillBaseInfoPtr( pExtraInfo->SkillCode ); 
	
	if( skill_Info == NULL )
		return;
	
	///--JAPAN_BUG_MODIFY
	DWORD dTotalDamage = 0;
	BOOL  bFindTotalDamage = FALSE; 
	BOOL  ISDisplayDamageNum = FALSE;
	BOOL  bFindDamageEffect = FALSE;
	
	if( skill_Info->theSkillType2 == n_DeathBlow || 
		skill_Info->theSkillType2 == n_AttackMagic || 
		skill_Info->theSkillType2 == n_BadSupport || 
		skill_Info->theSkillType2 == n_NoFrameDeathBlow )
	{
		ISDisplayDamageNum = TRUE;
	}

	for( int i = 0; i < 3; ++i )	// ��ų ȿ�� ó�� 3����  
	{
		if( skill_Info->theEffectInfo[i].theEffectType == n_ZERO )
		{
			continue;
		}

		int skillcase = 0;
		int effect_type = skill_Info->theEffectInfo[i].theEffectType;

		skillcase = effect_type / 30;

		if( skillcase == 1 )
		{
			effect_type -= 30;
		}
		else if( skillcase == 2 )
		{
			effect_type -= 60;
		}		

		switch( effect_type )
		{
			case n_HPAdd:
			case n_AttackBlowAdd:			// ���� �ʻ�� 
			case n_MagicBlowAdd:
				{
					bFindDamageEffect = TRUE;
					
					if(!bFindTotalDamage)
					{
						dTotalDamage = pExtraInfo->TargetInfo.theEffectValue[i];
						bFindTotalDamage = TRUE;
					}	
				}
				break;
		}

		if(bFindTotalDamage)
		{
			break;
		}
	}

	if( ISDisplayDamageNum )
	{
		if( !bFindDamageEffect )
		{
			ISDisplayDamageNum = FALSE;
		}
	}

	DWORD DamageRange = dTotalDamage;
	
	// �������� 0�϶� �׾������� ����Ʈ ���̱�
	if( DamageRange <= 0 && !pExtraInfo->TargetInfo.theDieFlag && ISDisplayDamageNum ) 
	{
		switch( pExtraInfo->SkillCode )
		{
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
			break;
			
		default:
			// ������ ������ �÷��̾�� �����ִ°͸� ���
			if( IsMyRelationUnit( pSrc , pDest ) ) 
			{
				g_Particle.SetEffSetCon( 0.0f , EF_ATTACK_MISS , 0 , 0.3f , pDest->GetUnique() , pDest->GetIndex() );
			}

			break;
		}
		// return;
	}
	
	// ������ ���� ���
	D3DXVECTOR3 vHeight = D3DXVECTOR3(0.0f,0.0f,0.0f);
	
	if( g_Map.IsGiantUnit( pDest ) )
	{
		vHeight = D3DXVECTOR3( pDest->GetBonCenterTM()._41 ,
			pDest->GetBonCenterTM()._42 + 38.0f, //(pDest->GetBoundMaxPos().y - pDest->GetBoundMinVec().y) * 0.3f, 
			pDest->GetBonCenterTM()._43 );
	}
	else
	{
		vHeight = D3DXVECTOR3( pDest->GetPosTM()._41 ,
			pDest->GetPosTM()._42 + ( pDest->GetBoundMaxPos().y * 1.2f ) ,
			pDest->GetPosTM()._43 );	
	}
	
	// ������ ������ �÷��̾�� �����ִ°͸� ���
	if( IsMyRelationUnit( pSrc , pDest ) && DamageRange > 0 && ISDisplayDamageNum )
	{
		int nDamage = 0;
		
		// �������� �������ֱ� ��������...�ӽ� �۾� ������ ���� �� .. 
		if( DamageRange >= c_DamageFlag_Subtract )
		{			
			DamageRange -= c_DamageFlag_Subtract;
			nDamage = DamageRange;
			
			nDamage *= -1;			
		}
		else
		{
			nDamage = DamageRange;
		}
		
		g_Particle.m_DamageNum.Add( vHeight  , nDamage , pExtraInfo->Critical, pDest );
	}	
	
	
	EVENT_DATA_INFO underattack_event;	
	
	underattack_event.type = pExtraInfo->EventType;
	underattack_event.cell = pExtraInfo->TargetInfo.thePushedCell;
	
	int skilledIdx = 0;
	
	if( !pExtraInfo->TargetInfo.theTargetType != n_PC && underattack_event.type != SM_STATE_DIE )
	{
		EDamageLevel Level = DamageLevel( pDest , DamageRange , pExtraInfo->Critical );
		
		MonsterAttackedSoundPlay( pDest , Level );
	}
	
	Character * pDestC = NULL;
	
	if( pExtraInfo->TargetInfo.theTargetType == n_PC )
	{
		//........................................................................................................
		// 1. �� ĳ���Ͱ� �¾����� ó�� by wxywxy
		//........................................................................................................
		if( pExtraInfo->TargetInfo.theTargetID == g_Pc.GetMainPC().lUnique )
		{
			//			
			pDestC = g_Pc.GetPlayer();
			
			if( g_Pc.GetPlayer()->m_bIsVisible == true ) //&& g_Pc.GetPlayer()->m_bIsLoaded == true )
			{
				
				if( PlayerInfoWnd.IsVisible )
				{
					PlayerInfoWnd.Start2DEffect(nRui->thePcParam.Stat1.theHP, pExtraInfo->TargetInfo,
						pExtraInfo->SkillCode);
				}				
				
				g_Pc.theSkillSystem.UseSkillTargetInfoMgr( pSrc, pExtraInfo->TargetInfo, pExtraInfo->SkillCode, TRUE );
				g_Pc.theSkillSystem.UseSkillTargetInfoMgr_AddEffect( pSrc, pExtraInfo->TargetInfo, pExtraInfo->SkillCode, TRUE );
				
				if( underattack_event.type == SM_UNDERATTACK )
				{
					g_Sound.CheckBattleMode(TRUE);
					
					if( g_Pc.GetPlayer()->ReadyUnderAttackMotion( pExtraInfo->Critical ) )
					{
						//						underattack_event.motion = g_Pc.GetPlayer()->SetUnderAttackMotion( pExtraInfo->TargetInfo.theEffectValue1 );
						if( bGoodSkill )
							underattack_event.motion = ANIMATION_NONE;						
						else
						{
							if( g_DelayBarWnd.IsVisible && DELAY_RIDE_ON == g_DelayBarWnd.GetDelay_Type() )
							{
								g_DelayBarWnd.Clear();	
							}							
							
							if( !g_Pc.GetPlayer()->m_State.bRide )	//���̵� ���� �ƴ϶��
							{
								underattack_event.motion = g_Pc.GetPlayer()->SetUnderAttackMotion( rand()%100 );
								g_Pc.GetPlayer()->PushQueue( underattack_event );
							}
							else
							{
								g_Pc.GetPlayer()->theVibrationTime = g_nowTime;
								g_Pc.GetPlayer()->theIsVibration = true;
								g_Map.m_Npc[g_Pc.GetPlayer()->m_State.TransIndex].theVibrationTime = g_nowTime;
								g_Map.m_Npc[g_Pc.GetPlayer()->m_State.TransIndex].theIsVibration = true;
							}
						}
						
						g_Pc.GetPlayer()->PushQueue( underattack_event, TRUE );	
					}
				}
				else if( underattack_event.type == SM_SLIDE_NPC )
				{
					if( !g_Pc.GetPlayer()->m_State.bRide )	//���̵� ���� �ƴ϶��
					{
						underattack_event.motion = n_NPC_ReAction2;
						g_Pc.GetPlayer()->PushQueue( underattack_event, TRUE );	
						g_Pc.GetPlayer()->SetDirection( pSrc->GetPosTM()._41, pSrc->GetPosTM()._43 );
					}
					else
					{
						g_Pc.GetPlayer()->theVibrationTime = g_nowTime;
						g_Pc.GetPlayer()->theIsVibration = true;
						g_Map.m_Npc[g_Pc.GetPlayer()->m_State.TransIndex].theVibrationTime = g_nowTime;
						g_Map.m_Npc[g_Pc.GetPlayer()->m_State.TransIndex].theIsVibration = true;
						g_Pc.GetPlayer()->SetDirection( pSrc->GetPosTM()._41, pSrc->GetPosTM()._43 );
						g_Map.m_Npc[g_Pc.GetPlayer()->m_State.TransIndex].SetDirection(pSrc->GetPosTM()._41, pSrc->GetPosTM()._43);
					}
				}			
				else if( underattack_event.type == SM_STATE_DIE )
				{
					RLGS("NonPlaer SM_ATTACK_PC kill = true");
					
					if( pSrc->m_curt_event.type != SM_STATE_DIE )
					{
						if( pSrc->m_bPC ) 
						{	
							SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_PC_DIE_BYUSER ), 
										((PCTABLE *)g_Pc.FindPcTable( pSrc ))->char_info.thePCName );
						}
						else
						{
							SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_PC_DIE_BYMOB ), 
										( g_Map.GetNpcTable( pSrc->GetUnique() ))->szName );
						}
					}
					else
					{
						///-- �׾��µ� �� ������� �޽����� ó���ؾ��Ҷ�
						return;
					}

					underattack_event.motion = n_Die9;
					
					if( pSrc == pDest )
					{
						underattack_event.type = SM_SELF_DIE;
					}
					
					g_Particle.DelEffSetCon( EF_REST_BREATH , 0 , g_Pc.GetPlayer()->GetUnique() , g_Pc.GetPlayer()->GetIndex() );
					
					g_Pc.GetPlayer()->PushQueue( underattack_event, TRUE );
					
					nRui->thePcParam.Stat1.theHP = 0;
					g_Pc.m_MainPC.char_info.theHP = 0;
					g_Pc.m_MainPC.char_info.theRealHP = 0;
					g_Pc.GetPlayer()->m_lPreTime = g_nowTime;					
					
					g_Pc.GetPlayer()->SetDirection( pSrc->GetPosTM()._41, pSrc->GetPosTM()._43 );
					
					// �÷��̾� �׾����� ����Ʈ �����
					DeletePlayerEffect( pDest );
				}				
			}
			
			
			// �ΰ�ȿ���� ���� ������ ���� �� ������ ��ġ �� ���� ��ȭ ó�� 
		}
		//........................................................................................................
		// 2. �ٸ� ������ �¾����� ó�� �¾����� ó�� by wxywxy
		//........................................................................................................
		else
		{
			skilledIdx = g_Pc.FindUser( pExtraInfo->TargetInfo.theTargetID );
			
			if( skilledIdx == MAX_USER_INTABLE )
				return;			
			
			//pDestC = &g_Pc.m_Pc[skilledIdx];
			
			PCTABLE *pPcTable = ((PCTABLE *)g_Pc.FindPcTable( pDest ));
			
			if( g_Pc.m_Pc[skilledIdx].m_bIsLoaded == true )
			{
				if( skilledIdx == NonPlayerInfoWnd.thePickedUserID )
				{
					long unique = pExtraInfo->TargetInfo.theTargetID;
					PCTABLE *plTable = g_Pc.FindPcTable(unique);					
					
					///-- ���� ��ü�� ���ϰ�쿡�� 2D ����Ʈ�� ���´�.				  
					if( plTable && NonPlayerInfoWnd.IsVisible && pSrc == g_Pc.GetPlayer() )
					{
						NonPlayerInfoWnd.Start2DEffect(plTable->char_info.theHP, 
							pExtraInfo->TargetInfo, pExtraInfo->SkillCode);	   		
					}				
				}				
				
				g_Pc.theSkillSystem.UseSkillTargetInfoMgr( pSrc, pExtraInfo->TargetInfo, pExtraInfo->SkillCode, TRUE );
				g_Pc.theSkillSystem.UseSkillTargetInfoMgr_AddEffect( pSrc, pExtraInfo->TargetInfo, pExtraInfo->SkillCode, TRUE );
				
				g_Pc.m_Pc[skilledIdx].theVibrationTime = g_nowTime;
				g_Pc.m_Pc[skilledIdx].theIsVibration = true;
				
				if( underattack_event.type == SM_UNDERATTACK )
				{
					
					if( g_Pc.m_Pc[skilledIdx].ReadyUnderAttackMotion( pExtraInfo->Critical ) )
					{
						//						underattack_event.motion = g_Pc.m_Pc[skilledIdx].SetUnderAttackMotion( pExtraInfo->TargetInfo.theEffectValue1 );
						
						if( bGoodSkill )
							underattack_event.motion = ANIMATION_NONE;
						else
						{
							if( !pDest->m_State.bRide )	//���̵� ���� �ƴ϶��
							{
								underattack_event.motion = (( NonPlayer * )pDest)->SetUnderAttackMotion( rand()%100 );
								(( NonPlayer * )pDest)->PushQueue(underattack_event, TRUE);
							}
							else
							{
								pDest->theVibrationTime = g_nowTime;
								pDest->theIsVibration = true;
								g_Map.m_Npc[pDest->m_State.TransIndex].theVibrationTime = g_nowTime;
								g_Map.m_Npc[pDest->m_State.TransIndex].theIsVibration = true;
							}						
						}
						
						g_Pc.m_Pc[skilledIdx].PushQueue( underattack_event, TRUE );
					}					
				}
				else if( underattack_event.type == SM_SLIDE_NPC )
				{
					if( !pDest->m_State.bRide )	//���̵� ���� �ƴ϶��
					{
						underattack_event.motion = n_ReAction_Critical9;
						g_Pc.m_Pc[skilledIdx].PushQueue( underattack_event, TRUE );	
						g_Pc.m_Pc[skilledIdx].SetDirection( pSrc->GetPosTM()._41, pSrc->GetPosTM()._43 );
					}
					else
					{
						pDest->theVibrationTime = g_nowTime;
						pDest->theIsVibration = true;
						(( NonPlayer * )pDest)->SetDirection( pSrc->GetPosTM()._41, pSrc->GetPosTM()._43 );
						g_Map.m_Npc[pDest->m_State.TransIndex].theVibrationTime = g_nowTime;
						g_Map.m_Npc[pDest->m_State.TransIndex].theIsVibration = true;
						g_Map.m_Npc[pDest->m_State.TransIndex].SetDirection(pSrc->GetPosTM()._41, pSrc->GetPosTM()._43);
					}
				}										
				else if( underattack_event.type == SM_STATE_DIE )
				{
					underattack_event.motion = n_Die9;
					
					if( pSrc == pDest )
					{
						underattack_event.type = SM_SELF_DIE;
					}					
					
					if(pPcTable)
					{
						pPcTable->char_info.theHP = 0;
						pPcTable->char_info.theRealHP = 0;
						pDest->m_lPreTime = g_nowTime;
					}
					
					g_Pc.m_Pc[skilledIdx].PushQueue(underattack_event, TRUE);	
					g_Pc.m_Pc[skilledIdx].SetDirection( pSrc->GetPosTM()._41, pSrc->GetPosTM()._43 );
					
					DeleteOtherPlayerEffect( pDest );
				}
				
				// �ٸ� ���� HP �������̽� ������ ó�� 
				
			}
		}
		
		// �ΰ�ȿ���� ���� ������ ���� �� ������ ��ġ �� ���� ��ȭ ó�� 
		// g_Pc.m_Pc[skilledIdx] <-- ����� �ش��ϴ� ĳ���� ��ġ�� �־ ������Ʈ �����ش� 
	}
	else 
		//............................................................................................................
		// 3. NPC �¾����� ó�� �¾����� ó�� by wxywxy
		//............................................................................................................
		if( pExtraInfo->TargetInfo.theTargetType == n_NPC )
		{
			skilledIdx = g_Map.FindNpc( pExtraInfo->TargetInfo.theTargetID );
			
			if( skilledIdx == MAX_NPC_INTABLE )
				return;
			
			NPCTABLE *pNpcTable;
			pNpcTable = g_Map.GetNpcTable( pDest->GetUnique() );
			if( pNpcTable == NULL ) return;
			
			if( g_Map.m_Npc[skilledIdx].m_bIsLoaded == true )
			{
				if( skilledIdx == NonPlayerInfoWnd.thePickedNpcID )
				{
					int iunique = pExtraInfo->TargetInfo.theTargetID;
					NPCTABLE *pNpcTable = g_Map.GetNpcTable(iunique);				
					
					///-- ���� ��ü�� ���ϰ�쿡�� 2D ����Ʈ�� ���´�.
					if( pNpcTable && NonPlayerInfoWnd.IsVisible && pSrc == g_Pc.GetPlayer() )
					{
						NonPlayerInfoWnd.Start2DEffect(pNpcTable->theHP, 
							pExtraInfo->TargetInfo, pExtraInfo->SkillCode);	   		
					}			
				}			
				
				g_Pc.theSkillSystem.UseSkillTargetInfoMgr( pSrc, pExtraInfo->TargetInfo, pExtraInfo->SkillCode, TRUE );					
				g_Pc.theSkillSystem.UseSkillTargetInfoMgr_AddEffect( pSrc, pExtraInfo->TargetInfo, pExtraInfo->SkillCode, TRUE );
				
				//...................................................................................................
				// ������ ó���� ��鸮�� ���ƾ� �Ұ͵� By wxywxy
				//...................................................................................................
				switch( g_Map.m_NpcTable[skilledIdx].code )
				{
				case c_Milena_Guad_Point_Npc:
				case c_Rain_Gaud_Point_Npc:
					break;
					
				default:
					g_Map.m_Npc[skilledIdx].theVibrationTime = g_nowTime;
					g_Map.m_Npc[skilledIdx].theIsVibration = true;
					break;
				}
				//...................................................................................................
				
				if( underattack_event.type == SM_UNDERATTACK )
				{
					if( g_Map.m_Npc[skilledIdx].ReadyUnderAttackMotion( pExtraInfo->Critical ) )
					{
						//					underattack_event.motion = g_Map.m_Npc[skilledIdx].SetUnderAttackMotion( pExtraInfo->TargetInfo.theEffectValue1 );
						if( bGoodSkill )
							underattack_event.motion = ANIMATION_NONE;
						else
							underattack_event.motion = g_Map.m_Npc[skilledIdx].SetUnderAttackMotion( rand()%100 );					
						
						g_Map.m_Npc[skilledIdx].PushQueue( underattack_event, TRUE );	
					}
				}
				else if( underattack_event.type == SM_SLIDE_NPC )
				{
					underattack_event.motion = n_NPC_ReAction2;
					g_Map.m_Npc[skilledIdx].PushQueue( underattack_event, TRUE );	
					g_Map.m_Npc[skilledIdx].SetDirection( pSrc->GetPosTM()._41, pSrc->GetPosTM()._43 );
				}	
				else if( underattack_event.type == SM_STATE_DIE )
				{
					if( pDest->m_State.nCharacterType == nCharacterTypePlayerNPC )
					{
						underattack_event.motion = n_Die9;
					}
					else
					{
						underattack_event.motion = n_NPC_Die0 + rand()%2;
					}			
					
					if( pSrc == pDest )
					{
						underattack_event.type = SM_SELF_DIE;
					}
					
					// ������ ������ �ִ� ����Ʈ �����
					DeleteNPCEffect( pDest );
					g_Map.m_Npc[skilledIdx].PushQueue(underattack_event, TRUE);	
					g_Map.m_Npc[skilledIdx].SetDirection( pSrc->GetPosTM()._41, pSrc->GetPosTM()._43 );
					
					pNpcTable->theHP = 0;
					pNpcTable->theRealHP = 0;
					pDest->m_lPreTime = g_nowTime;
					
					SNPCBaseInfo* npc_Info;
					
					if( pDest != NULL )
					{
						long ID = pDest->GetUnique();
						
						if( ID == -1 )
							return;
						
						NPCTABLE *pTable = g_Map.GetNpcTable( ID );
						
						if( pTable == NULL ) 
							return;
						
						npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( pTable->code ); 
						
						if( npc_Info == NULL )
							return;
						
						if( -1 != npc_Info->theDieSnd )
							g_Sound.Play( npc_Info->theDieSnd , NULL );
					}
				}
				
				
				// ���� HP �������̽� ������ ó��
				
		}
		
		
		
		// �ΰ�ȿ���� ���� ������ ���� �� ������ ��ġ �� ���� ��ȭ ó�� 
		// g_Pc.m_Pc[skilledIdx] <-- ����� �ش��ϴ� ĳ���� ��ġ�� �־ ������Ʈ �����ش� 
		
	}
	
	if( pDest->m_bPC && !bGoodSkill && pDest->m_State.nPetState == nPetState_Master)
	{
		SNPCBaseInfo* npc_Info = NULL;
		npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( g_Map.m_NpcTable[pDest->m_State.PetIndex].code );
		if( npc_Info && PET_AI_REDUCEDAMAGE == npc_Info->theAttackType )
		{
			if(ISZERO(pExtraInfo->fRadian))
			{
				g_Particle.SetEffSetCon( 0.0f , pExtraInfo->AfterEffect , pExtraInfo->DestType 
					, pExtraInfo->lifeTime , g_Map.m_Npc[pDest->m_State.PetIndex].GetUnique() , 
					g_Map.m_Npc[pDest->m_State.PetIndex].GetIndex() );
			}
			else
			{
				g_Particle.SetEffSetCon( 0.0f , pExtraInfo->AfterEffect , pExtraInfo->DestType 
					, pExtraInfo->lifeTime , g_Map.m_Npc[pDest->m_State.PetIndex].GetUnique() , 
					g_Map.m_Npc[pDest->m_State.PetIndex].GetIndex(), D3DXVECTOR3(0.0f, 0.0f, 0.0f)
					, pExtraInfo->fRadian );
			}
		}		
	}
	
	return;
}

//................................................................................................................
// ���� ���� ���� ( ���� �޼� : ���� ������ )
//................................................................................................................
int CEffectManager::IsNoneWeapon(Character *pSrc)
{
	if( pSrc == NULL )
		return FALSE;
	
	int RHand_index = pSrc->m_pUserInfo->ArmorInfo.index[0];
	int LHand_index = pSrc->m_pUserInfo->ArmorInfo.index[1];
	
	int HandType    = 0;
	
	
	if( RHand_index != -1 && LHand_index == -1 )				// ������ ���⸸ ����
	{		
		if( g_Pc_Manager.m_pItm[RHand_index]->m_type == 0 )		// ��� 
		{
			// ������
			HandType = n_WearSlot_Weapon;
		}
		else if( g_Pc_Manager.m_pItm[RHand_index]->m_type == 2 ) // �Ѽհ� 
		{
			// ������
			HandType = n_WearSlot_Weapon;
		}
		else if( g_Pc_Manager.m_pItm[RHand_index]->m_type == 6 ) // ������ 
		{
			// ������
			HandType = n_WearSlot_Weapon;
		}
		else if( g_Pc_Manager.m_pItm[RHand_index]->m_type == 7 ) // �����
		{
			// ������
			HandType = n_WearSlot_Weapon;
		}
	}
	else if( RHand_index == -1 && LHand_index != -1 )			// �޼� ���⸸ ����
	{
		if( g_Pc_Manager.m_pItm[LHand_index]->m_type == 1 )		// Ȱ 
		{
			// �޼�
			HandType = n_WearSlot_Shield;
		}
		else if( g_Pc_Manager.m_pItm[LHand_index]->m_type == 3 ) // ���� 
		{
			// �޼�
			HandType = n_WearSlot_Shield;
		}
		else if( g_Pc_Manager.m_pItm[LHand_index]->m_type == 5 ) // ������ 
		{
			// �޼�
			HandType = n_WearSlot_Shield;
		}
	}
	else if( RHand_index != -1 && LHand_index != -1 )			// ��տ� ���� ����
	{
		if( g_Pc_Manager.m_pItm[RHand_index]->m_type == 2 && 
			g_Pc_Manager.m_pItm[LHand_index]->m_type == 3 )		// �Ѽհ� + ����
		{
			//������
			HandType = n_WearSlot_Weapon;
		}
		else
		{
			//������
			HandType = n_WearSlot_Weapon;
		}
	}
	else
	{
		return -1;
	}
	
	if( pSrc->GetUnique() == g_Pc.GetMainPCUniqe() )	
	{
		if( nRui->thePcInven.WearItem[HandType] )
		{
			return nRui->thePcInven.WearItem[HandType]->Code;
		}
		else
		{ 
			return -1;
		}
	}
	else
	{
		if(pSrc->m_bPC)
		{
			return g_Pc.m_PcTable[ pSrc->m_nTableIndex ].char_info.theWearItem[HandType].theCode;	
		}
		else if(!pSrc->m_bPC && pSrc->m_State.nCharacterType == nCharacterTypePlayerNPC )
		{
			return g_Map.m_NpcTable[ pSrc->m_nTableIndex ].theExPcInfo.theWearItem[HandType];
		}	
	}
	
	return -1;
}

//................................................................................................................
// �⺻ ���ݽ� ó�� ��ƾ 
//................................................................................................................
void CEffectManager::StandAttack( Character *pSrc, Character *pDest, DWORD EventType,
								  DWORD Cell, BOOL IsKill, DWORD Damage, BOOL IsCritical,
								  long lStartTime )
{
	if( IsKill )
	{
		RLGS("IsKill StandAttack()");
	}
	
	if( pSrc == NULL || pDest == NULL )
		return;
	
	if( pDest->m_nTableIndex == -1 || pSrc->m_nTableIndex == -1 )
		return;
	
	//............................................................................................................
	// ���� ��ü ����
	//............................................................................................................
	if( pSrc->m_bPC == TRUE || ( !pSrc->m_bPC && pSrc->m_State.nCharacterType == nCharacterTypePlayerNPC ))
	{
		// ���� ������ �ڵ�
		DWORD WeaponCode       = -1;
		
		// �÷��̾ ��տ� ���� ���� ���°� �ƴϸ� ���� ����Ʈ ���
		if( -1 == ( WeaponCode = IsNoneWeapon( pSrc ) ) )
		{
			StandAttackHand( pSrc , pDest , EventType , Cell , IsKill , Damage , IsCritical );
			
			return;
		}
		
		//.......................................................................................................
		// ����Ʈ ���̽� �����
		//.......................................................................................................
		
		int   AttackEffectCode = g_Pc_Manager.theItemInfoTable[ g_Pc_Manager.theItemCodeTable[ WeaponCode ] ].theAttackEff;
		
		// ���� ����Ʈ ��/�� üũ
		if( -1 == AttackEffectCode )
			return;
		
		short AttackSound = g_Pc_Manager.theItemInfoTable[ g_Pc_Manager.theItemCodeTable[ WeaponCode ] ].theAttackSnd;
		
		// ���� ���� ��/�� üũ
		if( -1 != AttackSound )
			g_Sound.Play( AttackSound , NULL );
		
		EFFECT_EXTRA_INFO	TempExtraInfo;
		
		TempExtraInfo.EventType						   = EventType;
		TempExtraInfo.SkillCode						   = WeaponCode;
		TempExtraInfo.Critical						   = IsCritical;
		TempExtraInfo.lStartTime					   = lStartTime;	
		
		TempExtraInfo.TargetInfo.theTargetID           = pDest->GetUnique();
		
		if( pDest->m_bPC )
			TempExtraInfo.TargetInfo.theTargetType     = n_PC;
		else
			TempExtraInfo.TargetInfo.theTargetType     = n_NPC;
		
		TempExtraInfo.TargetInfo.theDieFlag            = IsKill;
		TempExtraInfo.TargetInfo.thePushedCell         = Cell;
		
		TempExtraInfo.TargetInfo.theEffectValue[ 0 ]   = Damage;					// ������
		
		
		CreateEffect( ( EFFECT_ID ) AttackEffectCode , pSrc , pDest , &TempExtraInfo );
	}
	//............................................................................................................
	// ���� ��ü NPC
	//............................................................................................................
	else
	{
		SNPCBaseInfo* npc_Info;
		int           AttackEffectCode;
		short		  AttackSound;
		
		long Unique = pSrc->GetUnique();
		
		NPCTABLE *pTable = g_Map.GetNpcTable( Unique );
		
		if( pTable == NULL )
			return;
		
		DWORD Code = pTable->code;
		
		npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( Code ); 
		
		AttackEffectCode = npc_Info->theAttackEff;
		AttackSound      = npc_Info->theAttackSnd0;
		
		// ���� ���� ��/��üũ
		if( -1 != AttackSound )
			g_Sound.Play( AttackSound , NULL );
		
		// ���� ����Ʈ ��/�� üũ
		if( -1 == AttackEffectCode )
			return;
		
		//....................................................................................................
		// NPC ���ݽ� ���� ������ ���� ( 0 , 1 , 2 : �Ҹ� �ȳ� )
		//....................................................................................................
		int RandomSound = rand() % 3;
		
		switch( RandomSound )
		{
		case 0:
			
			RandomSound = npc_Info->theAttackVoc1;
			
			break;
			
		case 1:
			
			RandomSound = npc_Info->theAttackVoc2;
			
			break;
			
		case 2:
			
			RandomSound = -1;
			
			break;
		}
		
		if( -1 != RandomSound )
			g_Sound.Play( RandomSound , pSrc->GetPosTM()._41 , pSrc->GetPosTM()._42 , pSrc->GetPosTM()._43 );
		
		//....................................................................................................
		
		// ����Ʈ ����
		EFFECT_EXTRA_INFO	TempExtraInfo;
		
		TempExtraInfo.EventType = EventType;
		TempExtraInfo.SkillCode = ( g_Map.GetNpcTable( pSrc->GetUnique() ))->code; // NPC �ڵ�
		TempExtraInfo.Critical  = IsCritical;
		TempExtraInfo.lStartTime = lStartTime;
		
		TempExtraInfo.TargetInfo.theTargetID         = pDest->GetUnique();
		
		if( pDest->m_bPC )
			TempExtraInfo.TargetInfo.theTargetType   = n_PC;
		else
			TempExtraInfo.TargetInfo.theTargetType   = n_NPC;
		
		TempExtraInfo.TargetInfo.theDieFlag          = IsKill;
		TempExtraInfo.TargetInfo.thePushedCell       = Cell;
		
		TempExtraInfo.TargetInfo.theEffectValue[ 0 ] = Damage;					// ������
		
		CreateEffect( ( EFFECT_ID ) AttackEffectCode , pSrc , pDest , &TempExtraInfo );
		
	}
}

//.................................................................................................................
// ���� HP ������
//.................................................................................................................
DWORD	CEffectManager::GetRealHP(Character *pSrc)
{
	if( pSrc == NULL )
		return FALSE;
	
	// PC �϶�
	if( TRUE == pSrc->m_bPC )
	{
		return ((PCTABLE *)g_Pc.FindPcTable( pSrc ))->char_info.theRealHP;
	}
	
	// NPC �϶�
	return ( g_Map.GetNpcTable( pSrc->GetUnique() ))->theRealHP;
}

//.................................................................................................................
// ȭ�� ǥ������ HP ������
//.................................................................................................................
DWORD	CEffectManager::GetHP(Character *pSrc)
{
	if( !pSrc )
	{
		RLGS("Error : WORD CEffectManager::GetHP(Character *pSrc)");
		return 1;
	}
	
	if( pSrc->GetUnique() == g_Pc.GetMainPCUniqe() )
	{
		return nRui->thePcParam.Stat1.theHP;
	}
	
	// PC �϶�
	if( TRUE == pSrc->m_bPC )
	{
		if( pSrc->m_nTableIndex == MAX_USER_INTABLE )
		{
			return nRui->thePcParam.Stat1.theHP;
		}
		else
		{
			PCTABLE* pcTable = NULL;
			pcTable = g_Pc.FindPcTable( pSrc );
			if( !pcTable )
			{
				RLGS("Error : pcTable = g_Pc.FindPcTable( pSrc );");
				return 1;
			}
			return pcTable->char_info.theHP;
		}
	}
	
	// NPC �϶�
	NPCTABLE* npcTable = NULL;
	npcTable = g_Map.GetNpcTable( pSrc->GetUnique() ); 
	if( !npcTable )
	{
		RLGS("Error : pcTable = g_Pc.FindPcTable( pSrc );");
		return 1;
	}
	return npcTable->theHP;
}

//.................................................................................................................
// ������ ���� ( 0 : ������ , 1 : �߰� , 2 : ũ��Ƽ�� )
//.................................................................................................................
EDamageLevel CEffectManager::DamageLevel(Character *pSrc,DWORD Damage,BOOL IsCritical)
{
	if( IsCritical ) 
	{
		return DL_CRITICAL;
	}
	
	DWORD theHP = 100.0f;							// �ӽ� ���߿� ���� ������ ���� By wxywxy
	
	float DamageLevel = (float) Damage / ( float ) GetHP( pSrc );
	
	if( DamageLevel > 0.3f )
	{
		return DL_STRONG;
	}
	
	return DL_WEAK;
}

//................................................................................................................
// �⺻ �� ���� ����Ʈ ó�� ��ƾ ( ������ ���� )
//................................................................................................................
void CEffectManager::StandAttackHand(Character *pSrc,Character *pDest,DWORD EventType,DWORD Cell,BOOL IsKill,DWORD Damage,BOOL IsCritical)	
{
	if( pSrc == NULL || pDest == NULL )
		return;
	
	// ���� �̽� ó��
	if( Damage <= 0 )
	{
		// ������ ������ �÷��̾�� �����ִ°͸� ���
		if( IsMyRelationUnit( pSrc , pDest ) )
		{
			g_Particle.SetEffSetCon( 0.0f , EF_ATTACK_MISS , 0 , 0.3f , pDest->GetUnique() , pDest->GetIndex() );
		}
		// RLGS( "\r--- �� ���� ( ���� ) ---\r" );
		
		return;
	}
	
	EDamageLevel Level = DamageLevel( pDest , Damage , IsCritical );
	
	// ���� ����Ʈ ���
	
	// ������ ���� ���
	D3DXVECTOR3 vHeight = D3DXVECTOR3(0.0f,0.0f,0.0f);
	
	
	if( g_Map.IsGiantUnit( pDest ) )
	{
		vHeight = D3DXVECTOR3( pDest->GetBonCenterTM()._41 ,
			pDest->GetBonCenterTM()._42 + 38.0f, //(pDest->GetBoundMaxPos().y - pDest->GetBoundMinVec().y) * 0.3f, 
			pDest->GetBonCenterTM()._43 );
	}
	else
	{
		vHeight = D3DXVECTOR3( pDest->GetPosTM()._41 ,
			pDest->GetPosTM()._42 + ( pDest->GetBoundMaxPos().y * 1.2f ) ,
			pDest->GetPosTM()._43 );	
	}
	
	// ������ ������ �÷��̾�� �����ִ°͸� ���
	if( IsMyRelationUnit( pSrc , pDest ) )
	{
		g_Particle.m_DamageNum.Add( vHeight  , Damage , IsCritical, pDest );
	}

	// ������ ����Ʈ ���
	switch( Level )
	{
		// ������ ��
	case DL_WEAK:
		
		//			RLGS( "\r--- �� ���� ( �� ) ---\r" );
		g_Particle.SetEffSetCon( 0.0f , EF_HAND_ATTACKED0 , 0 , 0.4f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
		
		break;
		
	case DL_STRONG:
		
		//			RLGS( "\r--- �� ���� ( �� ) ---\r" );
		g_Particle.SetEffSetCon( 0.0f , EF_HAND_ATTACKED1 , 0 , 0.4f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
		
		break;
		
	case DL_CRITICAL:
		
		//			RLGS( "\r--- �� ���� ( �� ) ---\r" );
		g_Particle.SetEffSetCon( 0.0f , EF_HAND_ATTACKED2 , 0 , 0.4f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
		
		break;
	}
	
	// ������ ó��
	EVENT_DATA_INFO underattack_event;	
	
	//............................................................................................................
	// �̺�Ʈ ó��
	//............................................................................................................
	
	// �����̵�
	if( Cell )
	{
		underattack_event.type = SM_SLIDE_NPC;
		underattack_event.cell = Cell;
	}
	else
	{
		// �ױ�
		if( IsKill )
		{					
			underattack_event.type = SM_STATE_DIE;
			underattack_event.loop = false;
		}
		// �⺻ ��Ÿ��
		else
		{
			underattack_event.type = SM_UNDERATTACK;
		}
	}
	
	// ���� ó��
	if( pDest->m_bPC )
	{
		// �÷��̾� ó��
		if( pDest->GetUnique() == g_Pc.GetMainPCUniqe() )
		{
			if( PlayerInfoWnd.IsVisible )
			{
				int RealHP = nRui->thePcParam.Stat1.theHP - Damage;
				PlayerInfoWnd.Start2DEffect(nRui->thePcParam.Stat1.theHP, RealHP);
			}			
			
			if( g_DelayBarWnd.IsVisible && DELAY_RIDE_ON == g_DelayBarWnd.GetDelay_Type() )
			{
				g_DelayBarWnd.Clear();	
			}
			
			///-- by simwoosung ������� �����ϰ� �ǰ��� �ٸ���
			///--JAPAN_BUG_MODIFY
			///-- by simwoosung ���� �׾����� �ʴ°�쿡�� �����Ų��.
			if( pDest->IsRealDie() )
			{
				nRui->thePcParam.Stat1.theHP -= Damage;
				g_Pc.GetPlayer()->m_lPreTime = g_nowTime;
			}
			
			if( underattack_event.type == SM_UNDERATTACK )
			{				
				g_Sound.CheckBattleMode(TRUE);
				
				if( g_Pc.GetPlayer()->ReadyUnderAttackMotion( IsCritical ) )
				{					
					if( !g_Pc.GetPlayer()->m_State.bRide )	//���̵� ���� �ƴ϶��
					{
						underattack_event.motion = g_Pc.GetPlayer()->SetUnderAttackMotion( Damage );
						g_Pc.GetPlayer()->PushQueue( underattack_event );
					}
					else
					{
						g_Pc.GetPlayer()->theVibrationTime = g_nowTime;
						g_Pc.GetPlayer()->theIsVibration = true;
						g_Map.m_Npc[g_Pc.GetPlayer()->m_State.TransIndex].theVibrationTime = g_nowTime;
						g_Map.m_Npc[g_Pc.GetPlayer()->m_State.TransIndex].theIsVibration = true;
					}
				}	
			}
			else if( underattack_event.type == SM_SLIDE_NPC )
			{
				
				if( !g_Pc.GetPlayer()->m_State.bRide )	//���̵� ���� �ƴ϶��
				{
					underattack_event.motion = n_ReAction_Critical9;
					g_Pc.GetPlayer()->PushQueue( underattack_event, TRUE );
					g_Pc.GetPlayer()->SetDirection( pSrc->GetPosTM()._41, pSrc->GetPosTM()._43 );
				}
				else
				{
					g_Pc.GetPlayer()->theVibrationTime = g_nowTime;
					g_Pc.GetPlayer()->theIsVibration = true;
					g_Map.m_Npc[g_Pc.GetPlayer()->m_State.TransIndex].theVibrationTime = g_nowTime;
					g_Map.m_Npc[g_Pc.GetPlayer()->m_State.TransIndex].theIsVibration = true;
					g_Pc.GetPlayer()->SetDirection( pSrc->GetPosTM()._41, pSrc->GetPosTM()._43 );
					g_Map.m_Npc[g_Pc.GetPlayer()->m_State.TransIndex].SetDirection(pSrc->GetPosTM()._41, pSrc->GetPosTM()._43);
				}
			}
			else if( underattack_event.type == SM_STATE_DIE )
			{
				RLGS("NonPlaer SM_ATTACK_PC kill = true");
				
				if( pSrc->m_curt_event.type != SM_STATE_DIE )
				{
					if( pSrc->m_bPC )
					{
						SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_PC_DIE_BYUSER ), 
															 ((PCTABLE *)g_Pc.FindPcTable( pSrc ))->char_info.thePCName );
					}
					else
					{	
						SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_PC_DIE_BYMOB ), 
															 ( g_Map.GetNpcTable( pSrc->GetUnique() ))->szName );
					}
				}
				else
				{
					///-- �׾��µ� �� ������� �޽����� ó���ؾ��Ҷ�
					return;
				}

				underattack_event.motion = n_Die9;
				
				g_Pc.GetPlayer()->PushQueue( underattack_event, TRUE );
				
				nRui->thePcParam.Stat1.theHP = 0;
				g_Pc.m_MainPC.char_info.theHP = 0;
				g_Pc.m_MainPC.char_info.theRealHP = 0;
				g_Pc.GetPlayer()->m_lPreTime = g_nowTime;
				
				// ������ ������ �ִ� ����Ʈ ��� �����
				DeletePlayerEffect( pDest );
				
				// ������ ȿ��
				g_Particle.SetEffSetCon( 0.0f , EF_PLAYER_DIE , 0 , 2.4f , pDest->GetUnique() , -1 );
				
				g_Pc.GetPlayer()->SetDirection( pSrc->GetPosTM()._41, pSrc->GetPosTM()._43 );
			}
			
			// ���� ũ��Ƽ�� ���� �������� ȭ�� ���� Boa ��
			if( IsCritical )
				g_Particle.RockCamera( ( rand() % 200 ) + 400 );
			
		}
		// �ٸ� ���� ó��
		else
		{
			PCTABLE *pTable = ((PCTABLE *)g_Pc.FindPcTable( pDest ));
			
			if( pDest->m_bIsLoaded == true )
			{
				//by simwoosung
				int index = g_Pc.FindUser(pDest->GetUnique());
				
				if( index == NonPlayerInfoWnd.thePickedUserID )
				{
					///���� ��ü�� ���ϰ�쿡�� 2D ����Ʈ�� ���´�.
					if( NonPlayerInfoWnd.IsVisible && pSrc == g_Pc.GetPlayer() )
					{
						int RealHP = pTable->char_info.theHP - Damage;	
						NonPlayerInfoWnd.Start2DEffect(pTable->char_info.theHP, 
							RealHP);	   		
					}
				}				
				
				///--JAPAN_BUG_MODIFY
				///-- by simwoosung ���� �׾����� �ʴ°�쿡�� �����Ų��.
				if( !(pDest->IsRealDie()) )
				{
					pDest->m_lPreTime = g_nowTime;
					pTable->char_info.theHP -= Damage;
				}										
				
				(( NonPlayer * )pDest)->theVibrationTime = g_nowTime;
				(( NonPlayer * )pDest)->theIsVibration = true;
				
				if( underattack_event.type == SM_UNDERATTACK )
				{
					if( (( NonPlayer * )pDest)->ReadyUnderAttackMotion( IsCritical ) )
					{
						if( !pDest->m_State.bRide )	//���̵� ���� �ƴ϶��
						{
							underattack_event.motion = (( NonPlayer * )pDest)->SetUnderAttackMotion( Damage );
							(( NonPlayer * )pDest)->PushQueue(underattack_event, TRUE);
						}
						else
						{
							pDest->theVibrationTime = g_nowTime;
							pDest->theIsVibration = true;
							g_Map.m_Npc[pDest->m_State.TransIndex].theVibrationTime = g_nowTime;
							g_Map.m_Npc[pDest->m_State.TransIndex].theIsVibration = true;
						}
					}					
				}				
				else if( underattack_event.type == SM_SLIDE_NPC )
				{
					if( !pDest->m_State.bRide )	//���̵� ���� �ƴ϶��
					{
						underattack_event.motion = n_ReAction_Critical9;
						(( NonPlayer * )pDest)->PushQueue(underattack_event, TRUE);	
						(( NonPlayer * )pDest)->SetDirection( pSrc->GetPosTM()._41, pSrc->GetPosTM()._43 );
					}
					else
					{
						pDest->theVibrationTime = g_nowTime;
						pDest->theIsVibration = true;
						(( NonPlayer * )pDest)->SetDirection( pSrc->GetPosTM()._41, pSrc->GetPosTM()._43 );
						g_Map.m_Npc[pDest->m_State.TransIndex].theVibrationTime = g_nowTime;
						g_Map.m_Npc[pDest->m_State.TransIndex].theIsVibration = true;
						g_Map.m_Npc[pDest->m_State.TransIndex].SetDirection(pSrc->GetPosTM()._41, pSrc->GetPosTM()._43);
					}
				}
				else if( underattack_event.type == SM_STATE_DIE )
				{
					underattack_event.motion = n_Die9;
					(( NonPlayer * )pDest)->PushQueue(underattack_event, TRUE);	
					pTable->char_info.theHP = 0;
					pTable->char_info.theRealHP = 0;
					pDest->m_lPreTime = g_nowTime;
					
					// ������ ������ �ִ� ����Ʈ ��� �����
					DeleteOtherPlayerEffect( pDest );
					
					// ������ ȿ��
					g_Particle.SetEffSetCon( 0.0f , EF_PLAYER_DIE , 0 , 2.4f , pDest->GetUnique() , -1 );
					(( NonPlayer * )pDest)->SetDirection( pSrc->GetPosTM()._41, pSrc->GetPosTM()._43 );
				}				
			} 			
		}
		
		//PC�� �¾����� �� �������ϰ�� - ����� ���� ������ �����ÿ� 
		//����� �꿡�� �´� ����� �־��ش�.
		if( underattack_event.type == SM_UNDERATTACK || underattack_event.type == SM_SLIDE_NPC )
		{
			if(pDest->m_State.nPetState == nPetState_Master)
			{
				SNPCBaseInfo* npc_Info = NULL;
				npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( g_Map.m_NpcTable[pDest->m_State.PetIndex].code );
				if( npc_Info && PET_AI_REDUCEDAMAGE == npc_Info->theAttackType )
				{
					g_Map.m_Npc[pDest->m_State.PetIndex].theVibrationTime = g_nowTime;
					g_Map.m_Npc[pDest->m_State.PetIndex].theIsVibration = true;	
					
					if( underattack_event.type == SM_UNDERATTACK )
					{
						if( g_Map.m_Npc[pDest->m_State.PetIndex].ReadyUnderAttackMotion( IsCritical ) )
						{
							underattack_event.motion = g_Map.m_Npc[pDest->m_State.PetIndex].SetUnderAttackMotion( Damage );							
							g_Map.m_Npc[pDest->m_State.PetIndex].PushQueue( underattack_event, TRUE );	
						}	
					}
					else if( underattack_event.type == SM_SLIDE_NPC )
					{
						underattack_event.motion = n_NPC_ReAction2;
						
						g_Map.m_Npc[pDest->m_State.PetIndex].PushQueue(underattack_event, TRUE);	
						g_Map.m_Npc[pDest->m_State.PetIndex].SetDirection( pSrc->GetPosTM()._41, pSrc->GetPosTM()._43 );
					}
				}			
			}		
		}
	}
	// NPC ó��
	else
	{
		if( TRUE == pDest->m_bIsLoaded )
		{
			NPCTABLE *pNpcTable;
			
			pNpcTable = g_Map.GetNpcTable( pDest->GetUnique() );
			
			//by simwoosung
			int index = g_Map.FindNpc(pDest->GetUnique());
			if( index == NonPlayerInfoWnd.thePickedNpcID )
			{
				///���� ��ü�� ���ϰ�쿡�� 2D ����Ʈ�� ���´�.
				if( NonPlayerInfoWnd.IsVisible &&  pSrc == g_Pc.GetPlayer() )
				{
					int RealHP = pNpcTable->theHP - Damage;	
					NonPlayerInfoWnd.Start2DEffect(pNpcTable->theHP, RealHP);	   		
				}
			}			
			
			///--JAPAN_BUG_MODIFY
			///-- by simwoosung ���� �׾����� �ʴ°�쿡�� �����Ų��.
			if( !(pDest->IsRealDie()) )
			{
				pDest->m_lPreTime = g_nowTime;
				pNpcTable->theHP -= Damage;
			}	
			
			//...................................................................................................
			// ������ ó���� ��鸮�� ���ƾ� �Ұ͵� By wxywxy
			//...................................................................................................
			switch( pNpcTable->code )
			{
			case c_Milena_Guad_Point_Npc:
			case c_Rain_Gaud_Point_Npc:
				break;
				
			default:
				(( NonPlayer * )pDest)->theVibrationTime = g_nowTime;
				(( NonPlayer * )pDest)->theIsVibration = true;
				break;
			}
			//...................................................................................................
			
			
			if( underattack_event.type == SM_UNDERATTACK )
			{
				if( pDest->ReadyUnderAttackMotion( IsCritical ) )
				{
					underattack_event.motion = pDest->SetUnderAttackMotion( Damage );
					
					(( NonPlayer * )pDest)->PushQueue( underattack_event, TRUE );	
				}
			}
			else if( underattack_event.type == SM_SLIDE_NPC )
			{
				underattack_event.motion = n_NPC_ReAction2;
				
				(( NonPlayer * )pDest)->PushQueue(underattack_event, TRUE);	
				(( NonPlayer * )pDest)->SetDirection( pSrc->GetPosTM()._41, pSrc->GetPosTM()._43 );
			}	
			else if( underattack_event.type == SM_STATE_DIE )
			{
				RLGS("Player SM_ATTACK_NPC kill = true");
				
				// ������ ������ �ִ� ����Ʈ ��� �����
				DeleteNPCEffect( pDest );
				
				if( pDest->m_State.nCharacterType == nCharacterTypePlayerNPC )
				{
					underattack_event.motion = n_Die9;
				}
				else
				{
					underattack_event.motion = n_NPC_Die0 + rand()%2;
				}			
				
				(( NonPlayer * )pDest)->PushQueue(underattack_event, TRUE);	
				(( NonPlayer * )pDest)->SetDirection( pSrc->GetPosTM()._41, pSrc->GetPosTM()._43 );
				
				pNpcTable->theHP = 0;
				pNpcTable->theRealHP = 0;
				pDest->m_lPreTime = g_nowTime;
				
				SNPCBaseInfo* npc_Info;
				
				npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( ( g_Map.GetNpcTable( pDest->GetUnique() ))->code ); 
				
				if( -1 != npc_Info->theDieSnd )
					g_Sound.Play( npc_Info->theDieSnd , NULL );
			}
			
		}
	}
	
}

void CEffectManager::DestTargetHit(Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo)
{
	if( pDest == NULL )
		return;
	
	if( pDest->m_nTableIndex == -1 )
		return;
	
	if( pExtraInfo == NULL )
	{
		return;
	}

	if( pExtraInfo->TargetInfo.theDieFlag )
	{
		RLGS("pExtraInfo.TargetInfo.theDieFlag DestTargetHit()");
	}	

	SSkillBaseInfo* skill_Info;	
	skill_Info = g_Pc_Manager.GetSkillBaseInfoPtr( pExtraInfo->SkillCode ); 	
	if( skill_Info == NULL )
		return;

	///--JAPAN_BUG_MODIFY
	DWORD dTotalDamage = 0;
	BOOL  bFindTotalDamage = FALSE; 
	BOOL  ISDisplayDamageNum = FALSE;
	BOOL  bFindDamageEffect = FALSE;
	
	if( skill_Info->theSkillType2 == n_DeathBlow || 
		skill_Info->theSkillType2 == n_AttackMagic || 
		skill_Info->theSkillType2 == n_BadSupport || 
		skill_Info->theSkillType2 == n_NoFrameDeathBlow )
	{
		ISDisplayDamageNum = TRUE;
	}

	for( int i = 0; i < 3; ++i )	// ��ų ȿ�� ó�� 3����  
	{
		if( skill_Info->theEffectInfo[i].theEffectType == n_ZERO )
		{
			continue;
		}

		int skillcase = 0;
		int effect_type = skill_Info->theEffectInfo[i].theEffectType;

		skillcase = effect_type / 30;

		if( skillcase == 1 )
		{
			effect_type -= 30;
		}
		else if( skillcase == 2 )
		{
			effect_type -= 60;
		}		

		switch( effect_type )
		{
			case n_HPAdd:
			case n_AttackBlowAdd:			// ���� �ʻ�� 
			case n_MagicBlowAdd:
				{
					bFindDamageEffect = TRUE;
					
					if(!bFindTotalDamage)
					{
						dTotalDamage = pExtraInfo->TargetInfo.theEffectValue[i];
						bFindTotalDamage = TRUE;
					}	
				}
				break;
		}

		if(bFindTotalDamage)
		{
			break;
		}
	}
	
	if( ISDisplayDamageNum )
	{
		if( !bFindDamageEffect )
		{
			ISDisplayDamageNum = FALSE;
		}
	}
	
	// ���� �̽� ó��
	if( !(pExtraInfo->TargetInfo.theDieFlag) && dTotalDamage <= 0 && ISDisplayDamageNum )
	{
		// ������ ������ �÷��̾�� �����ִ°͸� ���
		if( IsMyRelationUnit( NULL , pDest ) )
		{
			g_Particle.SetEffSetCon( 0.0f , EF_ATTACK_MISS , 0 , 0.3f , 
									 pDest->GetUnique() , pDest->GetIndex() );
		}
	}
	
	EDamageLevel Level = DamageLevel( pDest , dTotalDamage , pExtraInfo->Critical );
	
	// ������ ���� ���
	
	// ������ ���� ���
	D3DXVECTOR3 vHeight = D3DXVECTOR3(0.0f,0.0f,0.0f);		
	
	if( g_Map.IsGiantUnit( pDest ) )
	{
		vHeight = D3DXVECTOR3( pDest->GetBonCenterTM()._41 ,
			pDest->GetBonCenterTM()._42 + 38.0f,//(pDest->GetBoundMaxPos().y - pDest->GetBoundMinVec().y) * 0.3f, 
			pDest->GetBonCenterTM()._43 );
	}
	else
	{
		vHeight = D3DXVECTOR3( pDest->GetPosTM()._41 ,
			pDest->GetPosTM()._42 + ( pDest->GetBoundMaxPos().y * 1.2f ) ,
			pDest->GetPosTM()._43 );	
	}
	
	BOOL bGoodSkill = ISGoodSkill( pExtraInfo->SkillCode );	
	
	// ������ ������ �÷��̾�� �����ִ°͸� ���
	if( IsMyRelationUnit( NULL , pDest ) && dTotalDamage > 0 && ISDisplayDamageNum )
	{
		DWORD DamageRange = dTotalDamage;		
		int nDamage = 0;
		
		// �������� �������ֱ� ��������...�ӽ� �۾� ������ ���� �� .. 
		if( DamageRange >= c_DamageFlag_Subtract )
		{			
			DamageRange -= c_DamageFlag_Subtract;
			nDamage = DamageRange;
			
			nDamage *= -1;			
		}
		else
		{
			nDamage = DamageRange;
		}		
		
		g_Particle.m_DamageNum.Add( vHeight  , nDamage , pExtraInfo->Critical, pDest );
		
	}
	
	EVENT_DATA_INFO underattack_event;
	
	underattack_event.type = pExtraInfo->EventType;
	underattack_event.cell = pExtraInfo->TargetInfo.thePushedCell;
	
	int skilledIdx = 0;
	
	if( !pExtraInfo->TargetInfo.theTargetType != n_PC && underattack_event.type != SM_STATE_DIE &&
		ISDisplayDamageNum )
	{
		EDamageLevel Level = DamageLevel( pDest , dTotalDamage , pExtraInfo->Critical );		
		MonsterAttackedSoundPlay( pDest , Level );
	}
	
	Character * pDestC = NULL;
	
	if( pExtraInfo->TargetInfo.theTargetType == n_PC )
	{
		//........................................................................................................
		// 1. �� ĳ���Ͱ� �¾����� ó�� by wxywxy
		//........................................................................................................
		if( pExtraInfo->TargetInfo.theTargetID == g_Pc.GetMainPC().lUnique )
		{
			pDestC = g_Pc.GetPlayer();
			
			if( g_Pc.GetPlayer()->m_bIsVisible == true ) 
			{
				if( PlayerInfoWnd.IsVisible )
				{
					PlayerInfoWnd.Start2DEffect(nRui->thePcParam.Stat1.theHP, pExtraInfo->TargetInfo,
						pExtraInfo->SkillCode);
				}				
				
				g_Pc.theSkillSystem.UseSkillTargetInfoMgr( NULL, pExtraInfo->TargetInfo, pExtraInfo->SkillCode, TRUE );			
				
				if( underattack_event.type == SM_UNDERATTACK )
				{
					g_Sound.CheckBattleMode(TRUE);
					
					if( g_Pc.GetPlayer()->ReadyUnderAttackMotion( pExtraInfo->Critical ) )
					{
						if( bGoodSkill )
						{
							underattack_event.motion = ANIMATION_NONE;
						}
						else
						{
							if( g_DelayBarWnd.IsVisible && DELAY_RIDE_ON == g_DelayBarWnd.GetDelay_Type() )
							{
								g_DelayBarWnd.Clear();	
							}							
							
							if( !g_Pc.GetPlayer()->m_State.bRide )	//���̵� ���� �ƴ϶��
							{
								underattack_event.motion = g_Pc.GetPlayer()->SetUnderAttackMotion( rand()%100 );
								g_Pc.GetPlayer()->PushQueue( underattack_event );
							}
							else
							{
								g_Pc.GetPlayer()->theVibrationTime = g_nowTime;
								g_Pc.GetPlayer()->theIsVibration = true;
								g_Map.m_Npc[g_Pc.GetPlayer()->m_State.TransIndex].theVibrationTime = g_nowTime;
								g_Map.m_Npc[g_Pc.GetPlayer()->m_State.TransIndex].theIsVibration = true;
							}
						}
						
						g_Pc.GetPlayer()->PushQueue( underattack_event, TRUE );	
					}
				}
				else if( underattack_event.type == SM_SLIDE_NPC )
				{
					if( !g_Pc.GetPlayer()->m_State.bRide )	//���̵� ���� �ƴ϶��
					{
						underattack_event.motion = n_NPC_ReAction2;
						g_Pc.GetPlayer()->PushQueue( underattack_event, TRUE );						
					}
					else
					{
						g_Pc.GetPlayer()->theVibrationTime = g_nowTime;
						g_Pc.GetPlayer()->theIsVibration = true;
						g_Map.m_Npc[g_Pc.GetPlayer()->m_State.TransIndex].theVibrationTime = g_nowTime;
						g_Map.m_Npc[g_Pc.GetPlayer()->m_State.TransIndex].theIsVibration = true;				
					}
				}			
				else if( underattack_event.type == SM_STATE_DIE )
				{
					RLGS("NonPlaer SM_ATTACK_PC kill = true");
					
					underattack_event.motion = n_Die9;
					
					g_Particle.DelEffSetCon( EF_REST_BREATH , 0 , g_Pc.GetPlayer()->GetUnique() , g_Pc.GetPlayer()->GetIndex() );
					
					g_Pc.GetPlayer()->PushQueue( underattack_event, TRUE );
					
					nRui->thePcParam.Stat1.theHP = 0;
					g_Pc.m_MainPC.char_info.theHP = 0;
					g_Pc.m_MainPC.char_info.theRealHP = 0;
					g_Pc.GetPlayer()->m_lPreTime = g_nowTime;				
					
					// �÷��̾� �׾����� ����Ʈ �����
					DeletePlayerEffect( pDest );
				}				
			}
			
			
			// �ΰ�ȿ���� ���� ������ ���� �� ������ ��ġ �� ���� ��ȭ ó�� 
		}
		//........................................................................................................
		// 2. �ٸ� ������ �¾����� ó�� �¾����� ó�� by wxywxy
		//........................................................................................................
		else
		{
			skilledIdx = g_Pc.FindUser( pExtraInfo->TargetInfo.theTargetID );
			
			if( skilledIdx == MAX_USER_INTABLE )
				return;				
			
			PCTABLE *pPcTable = ((PCTABLE *)g_Pc.FindPcTable( pDest ));
			
			if( g_Pc.m_Pc[skilledIdx].m_bIsLoaded == true )
			{
				if( skilledIdx == NonPlayerInfoWnd.thePickedUserID )
				{
					long unique = pExtraInfo->TargetInfo.theTargetID;
					PCTABLE *plTable = g_Pc.FindPcTable(unique);			
				}				
				
				g_Pc.theSkillSystem.UseSkillTargetInfoMgr( NULL, pExtraInfo->TargetInfo, pExtraInfo->SkillCode, TRUE );			
				
				g_Pc.m_Pc[skilledIdx].theVibrationTime = g_nowTime;
				g_Pc.m_Pc[skilledIdx].theIsVibration = true;
				
				if( underattack_event.type == SM_UNDERATTACK )
				{
					if( g_Pc.m_Pc[skilledIdx].ReadyUnderAttackMotion( pExtraInfo->Critical ) )
					{
						if( bGoodSkill )
						{
							underattack_event.motion = ANIMATION_NONE;
						}
						else
						{
							if( !pDest->m_State.bRide )	//���̵� ���� �ƴ϶��
							{
								underattack_event.motion = (( NonPlayer * )pDest)->SetUnderAttackMotion( rand()%100 );
								(( NonPlayer * )pDest)->PushQueue(underattack_event, TRUE);
							}
							else
							{
								pDest->theVibrationTime = g_nowTime;
								pDest->theIsVibration = true;
								g_Map.m_Npc[pDest->m_State.TransIndex].theVibrationTime = g_nowTime;
								g_Map.m_Npc[pDest->m_State.TransIndex].theIsVibration = true;
							}						
						}
						
						g_Pc.m_Pc[skilledIdx].PushQueue( underattack_event, TRUE );
					}					
				}
				else if( underattack_event.type == SM_SLIDE_NPC )
				{
					if( !pDest->m_State.bRide )	//���̵� ���� �ƴ϶��
					{
						underattack_event.motion = n_ReAction_Critical9;
						g_Pc.m_Pc[skilledIdx].PushQueue( underattack_event, TRUE );						
					}
					else
					{
						pDest->theVibrationTime = g_nowTime;
						pDest->theIsVibration = true;					
						g_Map.m_Npc[pDest->m_State.TransIndex].theVibrationTime = g_nowTime;
						g_Map.m_Npc[pDest->m_State.TransIndex].theIsVibration = true;				
					}
				}										
				else if( underattack_event.type == SM_STATE_DIE )
				{
					underattack_event.motion = n_Die9;				
					
					if(pPcTable)
					{
						pPcTable->char_info.theHP = 0;
						pPcTable->char_info.theRealHP = 0;
						pDest->m_lPreTime = g_nowTime;
					}
					
					g_Pc.m_Pc[skilledIdx].PushQueue(underattack_event, TRUE);					
					DeleteOtherPlayerEffect( pDest );
				}				
				// �ٸ� ���� HP �������̽� ������ ó�� 				
			}
		}
	}
	else 
	{
		//............................................................................................................
		// 3. NPC �¾����� ó�� �¾����� ó�� by wxywxy
		//............................................................................................................
		if( pExtraInfo->TargetInfo.theTargetType == n_NPC )
		{
			skilledIdx = g_Map.FindNpc( pExtraInfo->TargetInfo.theTargetID );
			
			if( skilledIdx == MAX_NPC_INTABLE )
				return;
			
			NPCTABLE *pNpcTable;
			pNpcTable = g_Map.GetNpcTable( pDest->GetUnique() );
			if( pNpcTable == NULL ) return;
			
			if( g_Map.m_Npc[skilledIdx].m_bIsLoaded == true )
			{
				if( skilledIdx == NonPlayerInfoWnd.thePickedNpcID )
				{
					int iunique = pExtraInfo->TargetInfo.theTargetID;
					NPCTABLE *pNpcTable = g_Map.GetNpcTable(iunique);			
				}				
							
				g_Pc.theSkillSystem.UseSkillTargetInfoMgr( NULL, pExtraInfo->TargetInfo, pExtraInfo->SkillCode, TRUE );
					
				//...................................................................................................
				// ������ ó���� ��鸮�� ���ƾ� �Ұ͵� By wxywxy
				//...................................................................................................
				switch( g_Map.m_NpcTable[skilledIdx].code )
				{
				case c_Milena_Guad_Point_Npc:
				case c_Rain_Gaud_Point_Npc:
					break;
					
				default:
					g_Map.m_Npc[skilledIdx].theVibrationTime = g_nowTime;
					g_Map.m_Npc[skilledIdx].theIsVibration = true;
					break;
				}
				//...................................................................................................
				
				if( underattack_event.type == SM_UNDERATTACK )
				{
					if( g_Map.m_Npc[skilledIdx].ReadyUnderAttackMotion( pExtraInfo->Critical ) )
					{
						if( bGoodSkill )
							underattack_event.motion = ANIMATION_NONE;
						else
							underattack_event.motion = g_Map.m_Npc[skilledIdx].SetUnderAttackMotion( rand()%100 );					
						
						g_Map.m_Npc[skilledIdx].PushQueue( underattack_event, TRUE );	
					}
				}
				else if( underattack_event.type == SM_SLIDE_NPC )
				{
					underattack_event.motion = n_NPC_ReAction2;
					g_Map.m_Npc[skilledIdx].PushQueue( underattack_event, TRUE );				
				}	
				else if( underattack_event.type == SM_STATE_DIE )
				{
					if( pDest->m_State.nCharacterType == nCharacterTypePlayerNPC )
					{
						underattack_event.motion = n_Die9;
					}
					else
					{
						underattack_event.motion = n_NPC_Die0 + rand()%2;
					}

					// ������ ������ �ִ� ����Ʈ �����
					DeleteNPCEffect( pDest );
					
					g_Map.m_Npc[skilledIdx].PushQueue(underattack_event, TRUE);			
					
					pNpcTable->theHP = 0;
					pNpcTable->theRealHP = 0;
					pDest->m_lPreTime = g_nowTime;
					
					SNPCBaseInfo* npc_Info;
					
					if( pDest != NULL )
					{
						long ID = pDest->GetUnique();
						
						if( ID == -1 )
							return;
						
						NPCTABLE *pTable = g_Map.GetNpcTable( ID );
						
						if( pTable == NULL ) 
							return;
						
						npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( pTable->code ); 
						
						if( npc_Info == NULL )
							return;
						
						if( -1 != npc_Info->theDieSnd )
							g_Sound.Play( npc_Info->theDieSnd , NULL );
					}
				}
			}
		}			
	}
}

//................................................................................................................
// �⺻ ���� Ÿ�� ó��
//................................................................................................................
void CEffectManager::StandTargetHit(Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo)
{
	if( pSrc == NULL || pDest == NULL )
		return;
	
	if( pSrc->m_nTableIndex == -1 || pDest->m_nTableIndex == -1 )
		return;	
	
	if( pExtraInfo->TargetInfo.theDieFlag )
	{
		RLGS("pExtraInfo.TargetInfo.theDieFlag StandTargetHit()");
		/*
		RLG1("pExtraInfo.TargetInfo.theDieFlag StandTargetHit() pExtraInfo->EventType = %d", pExtraInfo->EventType );
		RLG1("pExtraInfo.TargetInfo.theDieFlag StandTargetHit() pExtraInfo->TargetInfo.theEffectValue[ 0 ] = %d", pExtraInfo->TargetInfo.theEffectValue[ 0 ] );
		*/
	}	
	
	if( SM_ATTACK_NPC != pExtraInfo->EventType )
		return;

	// ���� �̽� ó��
	if( !(pExtraInfo->TargetInfo.theDieFlag) && pExtraInfo->TargetInfo.theEffectValue[ 0 ] <= 0 )
	{
		// ������ ������ �÷��̾�� �����ִ°͸� ���
		if( IsMyRelationUnit( pSrc , pDest ) )
		{
			g_Particle.SetEffSetCon( 0.0f , EF_ATTACK_MISS , 0 , 0.3f , pDest->GetUnique() , pDest->GetIndex() );
		}	
	}
	
	EDamageLevel Level = DamageLevel( pDest , pExtraInfo->TargetInfo.theEffectValue[ 0 ] , pExtraInfo->Critical );
	
	// ������ ���� ���
	
	D3DXVECTOR3 vHeight = D3DXVECTOR3(0.0f,0.0f,0.0f);		
	
	if( g_Map.IsGiantUnit( pDest ) )
	{
		vHeight = D3DXVECTOR3( pDest->GetBonCenterTM()._41 ,
			pDest->GetBonCenterTM()._42 + 38.0f,//(pDest->GetBoundMaxPos().y - pDest->GetBoundMinVec().y) * 0.3f, 
			pDest->GetBonCenterTM()._43 );	
	}
	else
	{
		vHeight = D3DXVECTOR3( pDest->GetPosTM()._41 ,
			pDest->GetPosTM()._42 + ( pDest->GetBoundMaxPos().y * 1.2f ) ,
			pDest->GetPosTM()._43 );	
	}
	
	// ������ ������ �÷��̾�� �����ִ°͸� ���
	if( IsMyRelationUnit( pSrc , pDest ) && 
		pExtraInfo->TargetInfo.theEffectValue[ 0 ] > 0 )
	{
		g_Particle.m_DamageNum.Add( vHeight  , pExtraInfo->TargetInfo.theEffectValue[ 0 ] , 
		pExtraInfo->Critical, pDest );
	}	
	
	EVENT_DATA_INFO underattack_event;	
	
	//............................................................................................................
	// �̺�Ʈ ó��
	//............................................................................................................
	
	// �����̵�
	if( pExtraInfo->TargetInfo.thePushedCell )
	{
		underattack_event.type = SM_SLIDE_NPC;
		underattack_event.cell = pExtraInfo->TargetInfo.thePushedCell;
	}
	else
	{
		// �ױ�
		if( pExtraInfo->TargetInfo.theDieFlag )
		{					
			//			RLGS("pExtraInfo.TargetInfo.theDieFlag underattack_event.type = SM_STATE_DIE;");
			underattack_event.type = SM_STATE_DIE;
			underattack_event.loop = false;
		}
		// �⺻ ��Ÿ��
		else
		{
			underattack_event.type = SM_UNDERATTACK;
		}
	}
	
	if( !pDest->m_bPC && underattack_event.type != SM_STATE_DIE )
	{
		MonsterAttackedSoundPlay( pDest , Level );
	}
	
	//............................................................................................................
	// Ÿ�� ó��
	//............................................................................................................
	int   AttackedEffectCode = -1;
	short AttackedSound      = -1;
	
	// ���� ó��
	if( pDest->m_bPC )
	{
		// �÷��̾� ó��
		if( pDest->GetUnique() == g_Pc.GetMainPCUniqe() )
		{
			if( PlayerInfoWnd.IsVisible )
			{
				int RealHP = nRui->thePcParam.Stat1.theHP - pExtraInfo->TargetInfo.theEffectValue[ 0 ];
				PlayerInfoWnd.Start2DEffect(nRui->thePcParam.Stat1.theHP, RealHP);
			}		
			
			if( g_DelayBarWnd.IsVisible && DELAY_RIDE_ON == g_DelayBarWnd.GetDelay_Type() )
			{
				g_DelayBarWnd.Clear();	
			}
			
			///--JAPAN_BUG_MODIFY
			///-- by simwoosung ���� �׾����� �ʴ°�쿡�� �����Ų��.
			if( !(pDest->IsRealDie()) )
			{
				nRui->thePcParam.Stat1.theHP -= pExtraInfo->TargetInfo.theEffectValue[ 0 ];
				
				//PRINT_DLG_LOG("�� �׳� ���� ���� theHP - %d ����� - %d ������ ���̵� - %d", 
				//nRui->thePcParam.Stat1.theHP, pExtraInfo->TargetInfo.theEffectValue[ 0 ], 
				//g_Map.m_NpcTable[ pSrc->m_nTableIndex ].lUnique );
				
				g_Pc.GetPlayer()->m_lPreTime = g_nowTime;
			}		
			
			// dongs ��Ʀ ���� ���� ���� .. 
			if( nRui->thePcParam.Stat1.theHP >  nRui->thePcParam.MaxHP && (g_Pc.m_MainPC.char_info.theRealHP > 0 ) )
			{
				nRui->thePcParam.Stat1.theHP = g_Pc.m_MainPC.char_info.theRealHP;
				g_Pc.GetPlayer()->m_lPreTime = g_nowTime;
			}
			
			if( underattack_event.type == SM_UNDERATTACK )
			{				
				g_Sound.CheckBattleMode(TRUE);
				
				if( g_Pc.GetPlayer()->ReadyUnderAttackMotion( pExtraInfo->Critical ) )
				{					
					if( !g_Pc.GetPlayer()->m_State.bRide )	//���̵� ���� �ƴ϶��
					{
						underattack_event.motion = g_Pc.GetPlayer()->SetUnderAttackMotion( pExtraInfo->TargetInfo.theEffectValue[ 0 ] );
						g_Pc.GetPlayer()->PushQueue( underattack_event );
					}
					else
					{
						g_Pc.GetPlayer()->theVibrationTime = g_nowTime;
						g_Pc.GetPlayer()->theIsVibration = true;
						g_Map.m_Npc[g_Pc.GetPlayer()->m_State.TransIndex].theVibrationTime = g_nowTime;
						g_Map.m_Npc[g_Pc.GetPlayer()->m_State.TransIndex].theIsVibration = true;
					}
				}	
			}
			else if( underattack_event.type == SM_SLIDE_NPC )
			{
				if( !g_Pc.GetPlayer()->m_State.bRide )	//���̵� ���� �ƴ϶��
				{
					underattack_event.motion = n_ReAction_Critical9;
					g_Pc.GetPlayer()->PushQueue( underattack_event, TRUE );
					g_Pc.GetPlayer()->SetDirection( pSrc->GetPosTM()._41, pSrc->GetPosTM()._43 );
				}
				else
				{
					g_Pc.GetPlayer()->theVibrationTime = g_nowTime;
					g_Pc.GetPlayer()->theIsVibration = true;
					g_Map.m_Npc[g_Pc.GetPlayer()->m_State.TransIndex].theVibrationTime = g_nowTime;
					g_Map.m_Npc[g_Pc.GetPlayer()->m_State.TransIndex].theIsVibration = true;
					g_Pc.GetPlayer()->SetDirection( pSrc->GetPosTM()._41, pSrc->GetPosTM()._43 );
					g_Map.m_Npc[g_Pc.GetPlayer()->m_State.TransIndex].SetDirection(pSrc->GetPosTM()._41, pSrc->GetPosTM()._43);
				}
			}
			else if( underattack_event.type == SM_STATE_DIE )
			{
				RLGS("StandTargetHit NonPlaer SM_ATTACK_PC kill = true");
				
				if( pSrc->m_curt_event.type != SM_STATE_DIE )
				{
					if( pSrc->m_bPC )
					{
						SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_PC_DIE_BYUSER ), 
															 ((PCTABLE *)g_Pc.FindPcTable( pSrc ))->char_info.thePCName );
					}
					else
					{
						SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_PC_DIE_BYMOB ), 
															 ( g_Map.GetNpcTable( pSrc->GetUnique() ))->szName );
					}
				}
				else
				{
					///-- �׾��µ� �� ������� �޽����� ó���ؾ��Ҷ�
					return;
				}

				underattack_event.motion = n_Die9;			
				
				if( pSrc == pDest )
				{
					underattack_event.type = SM_SELF_DIE;
				}
				
				g_Particle.DelEffSetCon( EF_REST_BREATH , 0 , g_Pc.GetPlayer()->GetUnique() , g_Pc.GetPlayer()->GetIndex() );
				g_Pc.GetPlayer()->PushQueue( underattack_event, TRUE );									
				
				nRui->thePcParam.Stat1.theHP = 0;
				g_Pc.m_MainPC.char_info.theHP = 0;
				g_Pc.m_MainPC.char_info.theRealHP = 0;
				g_Pc.GetPlayer()->m_lPreTime = g_nowTime;
				
				// ������ ������ �ִ� ����Ʈ ��� �����
				DeletePlayerEffect( pDest );
				
				// ������ ȿ��
				g_Particle.SetEffSetCon( 0.0f , EF_PLAYER_DIE , 0 , 2.4f , pDest->GetUnique() , -1 );
				g_Pc.GetPlayer()->SetDirection( pSrc->GetPosTM()._41, pSrc->GetPosTM()._43 );
			}			
			
			// ���� ũ��Ƽ�� ���� �������� ȭ�� ���� Boa ��
			if( pExtraInfo->Critical )
				g_Particle.RockCamera( ( rand() % 200 ) + 400 );
			
		}
		// �ٸ� ���� ó��
		else
		{
			PCTABLE *pTable = ((PCTABLE *)g_Pc.FindPcTable( pDest ));
			
			if( pTable == NULL ) return;
			
			if( pDest->m_bIsLoaded == true )
			{
				
				//by simwoosung
				int index = g_Pc.FindUser(pDest->GetUnique());
				if( index == NonPlayerInfoWnd.thePickedUserID )
				{
					///���� ��ü�� ���ϰ�쿡�� 2D ����Ʈ�� ���´�.
					if( NonPlayerInfoWnd.IsVisible &&  pSrc == g_Pc.GetPlayer() )
					{
						int RealHP = pTable->char_info.theHP - pExtraInfo->TargetInfo.theEffectValue[ 0 ];
						NonPlayerInfoWnd.Start2DEffect(pTable->char_info.theHP, RealHP);	   		
					}
				}		
				
				///--JAPAN_BUG_MODIFY
				///-- by simwoosung ���� �׾����� �ʴ°�쿡�� �����Ų��.
				if( !(pDest->IsRealDie()) )
				{
					pDest->m_lPreTime = g_nowTime;
					pTable->char_info.theHP -= pExtraInfo->TargetInfo.theEffectValue[ 0 ];	
				}								
				
				//dongs �ǹ��� ���� ... ��Ʀ ���� .. ���� .. 
				if(pTable->char_info.theHP  > pTable->char_info.theMaxHP && (pTable->char_info.theRealHP > 0) )
				{
					pDest->m_lPreTime = g_nowTime;
					pTable->char_info.theHP = pTable->char_info.theRealHP;
				}
				
				
				(( NonPlayer * )pDest)->theVibrationTime = g_nowTime;
				(( NonPlayer * )pDest)->theIsVibration = true;
				
				if( underattack_event.type == SM_UNDERATTACK )
				{
					if( (( NonPlayer * )pDest)->ReadyUnderAttackMotion( pExtraInfo->Critical ) )
					{
						if( !pDest->m_State.bRide )	//���̵� ���� �ƴ϶��
						{
							underattack_event.motion = (( NonPlayer * )pDest)->SetUnderAttackMotion( pExtraInfo->TargetInfo.theEffectValue[ 0 ] );
							(( NonPlayer * )pDest)->PushQueue(underattack_event, TRUE);
						}
						else
						{
							pDest->theVibrationTime = g_nowTime;
							pDest->theIsVibration = true;
							g_Map.m_Npc[pDest->m_State.TransIndex].theVibrationTime = g_nowTime;
							g_Map.m_Npc[pDest->m_State.TransIndex].theIsVibration = true;
						}
					}					
				}
				else if( underattack_event.type == SM_SLIDE_NPC )
				{
					if( !pDest->m_State.bRide )	//���̵� ���� �ƴ϶��
					{
						underattack_event.motion = n_ReAction_Critical9;
						(( NonPlayer * )pDest)->PushQueue(underattack_event, TRUE);	
						(( NonPlayer * )pDest)->SetDirection( pSrc->GetPosTM()._41, pSrc->GetPosTM()._43 );
					}
					else
					{
						pDest->theVibrationTime = g_nowTime;
						pDest->theIsVibration = true;
						(( NonPlayer * )pDest)->SetDirection( pSrc->GetPosTM()._41, pSrc->GetPosTM()._43 );
						g_Map.m_Npc[pDest->m_State.TransIndex].theVibrationTime = g_nowTime;
						g_Map.m_Npc[pDest->m_State.TransIndex].theIsVibration = true;
						g_Map.m_Npc[pDest->m_State.TransIndex].SetDirection(pSrc->GetPosTM()._41, pSrc->GetPosTM()._43);
					}
				}
				else if( underattack_event.type == SM_STATE_DIE )
				{
					underattack_event.motion = n_Die9;
					
					if( pSrc == pDest )
					{
						underattack_event.type = SM_SELF_DIE;
					}
					
					(( NonPlayer * )pDest)->PushQueue(underattack_event, TRUE);	
					pTable->char_info.theHP = 0;
					pTable->char_info.theRealHP = 0;
					pDest->m_lPreTime = g_nowTime;
					
					// ������ ������ �ִ� ����Ʈ ��� �����
					DeleteOtherPlayerEffect( pDest );
					
					// ������ ȿ��
					g_Particle.SetEffSetCon( 0.0f , EF_PLAYER_DIE , 0 , 2.4f , pDest->GetUnique() , -1 );
					(( NonPlayer * )pDest)->SetDirection( pSrc->GetPosTM()._41, pSrc->GetPosTM()._43 );
				}
				
			} 			
		}
		
		//PC�� �¾����� �� �������ϰ�� - ����� ���� ������ �����ÿ� 
		//����� �꿡�� �´� ����� �־��ش�.
		if( underattack_event.type == SM_UNDERATTACK || underattack_event.type == SM_SLIDE_NPC )
		{
			if(pDest->m_State.nPetState == nPetState_Master)
			{
				SNPCBaseInfo* npc_Info = NULL;
				npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( g_Map.m_NpcTable[pDest->m_State.PetIndex].code );
				if( npc_Info && PET_AI_REDUCEDAMAGE == npc_Info->theAttackType )
				{
					g_Map.m_Npc[pDest->m_State.PetIndex].theVibrationTime = g_nowTime;
					g_Map.m_Npc[pDest->m_State.PetIndex].theIsVibration = true;					
					
					if( underattack_event.type == SM_UNDERATTACK )
					{
						if( g_Map.m_Npc[pDest->m_State.PetIndex].ReadyUnderAttackMotion( pExtraInfo->Critical ) )
						{
							underattack_event.motion = g_Map.m_Npc[pDest->m_State.PetIndex].SetUnderAttackMotion( pExtraInfo->TargetInfo.theEffectValue[ 0 ] );							
							g_Map.m_Npc[pDest->m_State.PetIndex].PushQueue( underattack_event, TRUE );	
						}	
					}
					else if( underattack_event.type == SM_SLIDE_NPC )
					{
						underattack_event.motion = n_NPC_ReAction2;
						
						g_Map.m_Npc[pDest->m_State.PetIndex].PushQueue(underattack_event, TRUE);	
						g_Map.m_Npc[pDest->m_State.PetIndex].SetDirection( pSrc->GetPosTM()._41, pSrc->GetPosTM()._43 );
					}
				}			
			}		
		}
	}
	// NPC ó��
	else
	{
		if( TRUE == pDest->m_bIsLoaded )
		{
			NPCTABLE *pNpcTable;
			
			pNpcTable = g_Map.GetNpcTable( pDest->GetUnique() );
			
			if( pNpcTable == NULL ) return;
			
			//by simwoosung
			int index = g_Map.FindNpc(pDest->GetUnique());
			if( index == NonPlayerInfoWnd.thePickedNpcID )
			{
				///���� ��ü�� ���ϰ�쿡�� 2D ����Ʈ�� ���´�.
				if( NonPlayerInfoWnd.IsVisible &&  pSrc == g_Pc.GetPlayer() )
				{
					int RealHP = pNpcTable->theHP - pExtraInfo->TargetInfo.theEffectValue[ 0 ];
					NonPlayerInfoWnd.Start2DEffect(pNpcTable->theHP, RealHP);	   		
				}
			}
			
			///--JAPAN_BUG_MODIFY
			///-- by simwoosung ���� �׾����� �ʴ°�쿡�� �����Ų��.
			if( !(pDest->IsRealDie()) )
			{
				pDest->m_lPreTime = g_nowTime;
				pNpcTable->theHP -= pExtraInfo->TargetInfo.theEffectValue[ 0 ];
			}	
			
			//...................................................................................................
			// ������ ó���� ��鸮�� ���ƾ� �Ұ͵� By wxywxy
			//...................................................................................................
			switch( g_Map.m_NpcTable[index].code )
			{
			case c_Milena_Guad_Point_Npc:
			case c_Rain_Gaud_Point_Npc:
				break;
				
			default:
				(( NonPlayer * )pDest)->theVibrationTime = g_nowTime;
				(( NonPlayer * )pDest)->theIsVibration = true;
				break;
			}
			//...................................................................................................
			
			
			if( underattack_event.type == SM_UNDERATTACK )
			{
				if( pDest->ReadyUnderAttackMotion( pExtraInfo->Critical ) )
				{
					underattack_event.motion = pDest->SetUnderAttackMotion( pExtraInfo->TargetInfo.theEffectValue[ 0 ] );
					
					(( NonPlayer * )pDest)->PushQueue( underattack_event, TRUE );	
				}
			}
			else if( underattack_event.type == SM_SLIDE_NPC )
			{
				underattack_event.motion = n_NPC_ReAction2;
				
				(( NonPlayer * )pDest)->PushQueue(underattack_event, TRUE);	
				(( NonPlayer * )pDest)->SetDirection( pSrc->GetPosTM()._41, pSrc->GetPosTM()._43 );
			}	
			else if( underattack_event.type == SM_STATE_DIE )
			{
				SNPCBaseInfo* npc_Info;
				
				npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( ( g_Map.GetNpcTable( pDest->GetUnique() ))->code ); 
				
				if( -1 != npc_Info->theDieSnd )
					g_Sound.Play( npc_Info->theDieSnd , pDest->GetPosTM()._41 ,
					pDest->GetPosTM()._42 ,
					pDest->GetPosTM()._43  );
				
				//				RLGS("Player SM_ATTACK_NPC kill = true");
				
				// ������ ������ �ִ� ����Ʈ ��� ����� ( NPC�� ������ ��� �� )
				//DeleteNPCEffect( pDest );
				
				if( pDest->m_State.nCharacterType == nCharacterTypePlayerNPC )
				{
					underattack_event.motion = n_Die9;
				}
				else
				{
					underattack_event.motion = n_NPC_Die0 + rand()%2;
				}				
				
				if( pSrc == pDest )
				{
					underattack_event.type = SM_SELF_DIE;
				}
				
				(( NonPlayer * )pDest)->PushQueue(underattack_event, TRUE);	
				(( NonPlayer * )pDest)->SetDirection( pSrc->GetPosTM()._41, pSrc->GetPosTM()._43 );
				
				pNpcTable->theHP = 0;
				pNpcTable->theRealHP = 0;
				pDest->m_lPreTime = g_nowTime;
			}
		}
	}
	
	//............................................................................................................
	// Ÿ�� ����Ʈ
	//............................................................................................................
	
	// ���� �� ����
	if( pSrc->m_bPC || ( !pSrc->m_bPC && pSrc->m_State.nCharacterType == nCharacterTypePlayerNPC ) )
	{
		switch( Level )
		{
			// ������ ��
		case DL_WEAK:
			
			AttackedEffectCode = g_Pc_Manager.theItemInfoTable[ g_Pc_Manager.theItemCodeTable[ pExtraInfo->SkillCode ] ].theAttackSuccEff1;
			AttackedSound      = g_Pc_Manager.theItemInfoTable[ g_Pc_Manager.theItemCodeTable[ pExtraInfo->SkillCode ] ].theAttackSuccSnd1;
			
			//			RLGS( "\r--- ���� �⺻ ���ݿ� ���� ( �� ) ---\r" );
			
			
			break;
			
		case DL_STRONG:
			
			AttackedEffectCode = g_Pc_Manager.theItemInfoTable[ g_Pc_Manager.theItemCodeTable[ pExtraInfo->SkillCode ] ].theAttackSuccEff2;
			AttackedSound      = g_Pc_Manager.theItemInfoTable[ g_Pc_Manager.theItemCodeTable[ pExtraInfo->SkillCode ] ].theAttackSuccSnd2;
			
			//			RLGS( "\r--- ���� �⺻ ���ݿ� ���� ( �� ) ---\r" );
			
			break;
			
		case DL_CRITICAL:
			
			AttackedEffectCode = g_Pc_Manager.theItemInfoTable[ g_Pc_Manager.theItemCodeTable[ pExtraInfo->SkillCode ] ].theAttackSuccEff3;
			AttackedSound      = g_Pc_Manager.theItemInfoTable[ g_Pc_Manager.theItemCodeTable[ pExtraInfo->SkillCode ] ].theAttackSuccSnd3;
			
			//			RLGS( "\r--- ���� �⺻ ���ݿ� ���� ( �� ) ---\r" );
			
			break;
		}
	}
	else
	{
		// ������ ����Ʈ ����
		SNPCBaseInfo* npc_Info = NULL;
		DWORD dNpcCode = 0;
		
		dNpcCode = g_Map.m_NpcTable[pSrc->m_nTableIndex].code;
		npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( dNpcCode ); 	
		
		if( NULL == npc_Info )
		{
			return;
		}
		
		switch( Level )
		{
			// ������ ��
		case DL_WEAK:
			
			AttackedEffectCode = npc_Info->theAttackedEff1;
			AttackedSound      = npc_Info->theAttackedSnd1;
			
			//				RLGS( "\r--- NPC �⺻ ���ݿ� ���� ( �� ) ---\r" );
			
			break;
			
		case DL_STRONG:
			
			AttackedEffectCode = npc_Info->theAttackedEff2;
			AttackedSound      = npc_Info->theAttackedSnd2;
			
			//				RLGS( "\r--- NPC �⺻ ���ݿ� ���� ( �� ) ---\r" );
			
			break;
			
		case DL_CRITICAL:
			
			AttackedEffectCode = npc_Info->theAttackedEff3;
			AttackedSound      = npc_Info->theAttackedSnd3;
			
			//				RLGS( "\r--- NPC �⺻ ���ݿ� ���� ( �� ) ---\r" );
			
			break;
		}
	}
	
	if( -1 != AttackedSound )
		g_Sound.Play( AttackedSound , pDest->GetPosTM()._41 , pDest->GetPosTM()._42 , pDest->GetPosTM()._43 );
	
	if( -1 == AttackedEffectCode )
		return;
	
	Create( ( EFFECT_ID ) AttackedEffectCode , pSrc , pDest );
	
	if(pDest->m_bPC && pDest->m_State.nPetState == nPetState_Master)
	{
		SNPCBaseInfo* npc_Info = NULL;
		npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( g_Map.m_NpcTable[pDest->m_State.PetIndex].code );
		if( npc_Info && PET_AI_REDUCEDAMAGE == npc_Info->theAttackType )
		{
			Create( ( EFFECT_ID ) AttackedEffectCode , pSrc , &g_Map.m_Npc[pDest->m_State.PetIndex] );
		}		
	}
}

//................................................................................................................
// �⺻ ���� Ÿ�� ó�� ( ������ ���� & ȿ�� ���� )
//................................................................................................................
void CEffectManager::StandTargetHitNoDamage(Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo)
{
	if( pSrc == NULL || pDest == NULL )
		return;
	
	if( pSrc->m_nTableIndex == -1 || pDest->m_nTableIndex == -1 )
		return;
	
	if( pExtraInfo->TargetInfo.theDieFlag )
	{
		RLGS("pExtraInfo.TargetInfo.theDieFlag StandTargetHit()");
		RLG1("pExtraInfo.TargetInfo.theDieFlag StandTargetHit() pExtraInfo->EventType = %d", pExtraInfo->EventType );
		RLG1("pExtraInfo.TargetInfo.theDieFlag StandTargetHit() pExtraInfo->TargetInfo.theEffectValue[ 0 ] = %d", pExtraInfo->TargetInfo.theEffectValue[ 0 ] );
	}
	
	if( SM_ATTACK_NPC != pExtraInfo->EventType )
		return;
	
	// ���� �̽� ó��
	if( !(pExtraInfo->TargetInfo.theDieFlag) && pExtraInfo->TargetInfo.theEffectValue[ 0 ] <= 0 )
	{
		//		RLGS( "\r--- �⺻ ���� ( ���� ) ---\r" );
		
		return;
	}
	
	//............................................................................................................
	// Ÿ�� ����Ʈ
	//............................................................................................................
	EDamageLevel Level = DamageLevel( pDest , pExtraInfo->TargetInfo.theEffectValue[ 0 ] , pExtraInfo->Critical );
	int   AttackedEffectCode = -1;
	short AttackedSound      = -1;
	
	// ���� �� ����
	if( pSrc->m_bPC )
	{
		switch( Level )
		{
			// ������ ��
		case DL_WEAK:
			
			AttackedEffectCode = g_Pc_Manager.theItemInfoTable[ g_Pc_Manager.theItemCodeTable[ pExtraInfo->SkillCode ] ].theAttackSuccEff1;
			AttackedSound      = g_Pc_Manager.theItemInfoTable[ g_Pc_Manager.theItemCodeTable[ pExtraInfo->SkillCode ] ].theAttackSuccSnd1;
			
			//			RLGS( "\r--- ���� �⺻ ���ݿ� ���� ( �� ) ---\r" );
			
			
			break;
			
		case DL_STRONG:
			
			AttackedEffectCode = g_Pc_Manager.theItemInfoTable[ g_Pc_Manager.theItemCodeTable[ pExtraInfo->SkillCode ] ].theAttackSuccEff2;
			AttackedSound      = g_Pc_Manager.theItemInfoTable[ g_Pc_Manager.theItemCodeTable[ pExtraInfo->SkillCode ] ].theAttackSuccSnd2;
			
			//			RLGS( "\r--- ���� �⺻ ���ݿ� ���� ( �� ) ---\r" );
			
			break;
			
		case DL_CRITICAL:
			
			AttackedEffectCode = g_Pc_Manager.theItemInfoTable[ g_Pc_Manager.theItemCodeTable[ pExtraInfo->SkillCode ] ].theAttackSuccEff3;
			AttackedSound      = g_Pc_Manager.theItemInfoTable[ g_Pc_Manager.theItemCodeTable[ pExtraInfo->SkillCode ] ].theAttackSuccSnd3;
			
			//			RLGS( "\r--- ���� �⺻ ���ݿ� ���� ( �� ) ---\r" );
			
			break;
		}
	}
	// NPC �� ����
	else
	{
		// ������ ����Ʈ ����
		SNPCBaseInfo* npc_Info = NULL;
		DWORD dNpcCode = 0;
		
		dNpcCode = g_Map.m_NpcTable[pSrc->m_nTableIndex].code;
		npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( dNpcCode );
		
		if( NULL == npc_Info )
		{
			return;
		}
		
		switch( Level )
		{
			// ������ ��
		case DL_WEAK:
			
			AttackedEffectCode = npc_Info->theAttackedEff1;
			AttackedSound      = npc_Info->theAttackedSnd1;
			
			//				RLGS( "\r--- NPC �⺻ ���ݿ� ���� ( �� ) ---\r" );
			
			break;
			
		case DL_STRONG:
			
			AttackedEffectCode = npc_Info->theAttackedEff2;
			AttackedSound      = npc_Info->theAttackedSnd2;
			
			//				RLGS( "\r--- NPC �⺻ ���ݿ� ���� ( �� ) ---\r" );
			
			break;
			
		case DL_CRITICAL:
			
			AttackedEffectCode = npc_Info->theAttackedEff3;
			AttackedSound      = npc_Info->theAttackedSnd3;
			
			//				RLGS( "\r--- NPC �⺻ ���ݿ� ���� ( �� ) ---\r" );
			
			break;
		}
	}
	
	if( -1 == AttackedEffectCode )
		return;
	
	if( -1 != AttackedSound )
		g_Sound.Play( AttackedSound , NULL );
	
	Create( ( EFFECT_ID ) AttackedEffectCode , pSrc , pDest );
	if(pDest->m_bPC && pDest->m_State.nPetState == nPetState_Master)
	{
		SNPCBaseInfo* npc_Info = NULL;
		npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( g_Map.m_NpcTable[pDest->m_State.PetIndex].code );
		if( npc_Info && PET_AI_REDUCEDAMAGE == npc_Info->theAttackType )
		{
			Create( ( EFFECT_ID ) AttackedEffectCode , pSrc , &g_Map.m_Npc[pDest->m_State.PetIndex] );
		}		
	}
}

void CEffectManager::DestStandTargetHit(Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo)
{
	if( pDest == NULL )
	{
		return;
	}
	
	if( pExtraInfo == NULL )
	{
		return;
	}
	
	if( pDest->m_nTableIndex == -1 )
		return;
	
	if( SM_ATTACK_NPC != pExtraInfo->EventType )
		return;	
	
	// ���� �̽� ó��
	if( !(pExtraInfo->TargetInfo.theDieFlag) && pExtraInfo->TargetInfo.theEffectValue[ 0 ] <= 0 )
	{
		return;
	}
	
	EDamageLevel Level = DamageLevel( pDest , pExtraInfo->TargetInfo.theEffectValue[ 0 ] , pExtraInfo->Critical );
	
	EVENT_DATA_INFO underattack_event;	
	
	//............................................................................................................
	// �̺�Ʈ ó��
	//............................................................................................................
	
	// �����̵�
	if( pExtraInfo->TargetInfo.thePushedCell )
	{
		underattack_event.type = SM_SLIDE_NPC;
		underattack_event.cell = pExtraInfo->TargetInfo.thePushedCell;
	}
	else
	{
		// �ױ�
		if( pExtraInfo->TargetInfo.theDieFlag )
		{					
			//			RLGS("pExtraInfo.TargetInfo.theDieFlag underattack_event.type = SM_STATE_DIE;");
			underattack_event.type = SM_STATE_DIE;
			underattack_event.loop = false;
		}
		// �⺻ ��Ÿ��
		else
		{
			underattack_event.type = SM_UNDERATTACK;
		}
	}
	
	if( !pDest->m_bPC && underattack_event.type != SM_STATE_DIE )
	{
		MonsterAttackedSoundPlay( pDest , Level );
	}
	
	//............................................................................................................
	// Ÿ�� ó��
	//............................................................................................................
	int   AttackedEffectCode = -1;
	short AttackedSound      = -1;
	
	// ���� ó��
	if( pDest->m_bPC )
	{
		// �÷��̾� ó��
		if( pDest->GetUnique() == g_Pc.GetMainPCUniqe() )
		{
			
			if( PlayerInfoWnd.IsVisible )
			{
				int RealHP = nRui->thePcParam.Stat1.theHP - pExtraInfo->TargetInfo.theEffectValue[ 0 ];
				PlayerInfoWnd.Start2DEffect(nRui->thePcParam.Stat1.theHP, RealHP);
			}
			
			if( g_DelayBarWnd.IsVisible && DELAY_RIDE_ON == g_DelayBarWnd.GetDelay_Type() )
			{
				g_DelayBarWnd.Clear();	
			}
			
			///--JAPAN_BUG_MODIFY
			///-- by simwoosung ���� �׾����� �ʴ°�쿡�� �����Ų��.
			if( !(pDest->IsRealDie()) )
			{
				nRui->thePcParam.Stat1.theHP -= pExtraInfo->TargetInfo.theEffectValue[ 0 ];
				g_Pc.GetPlayer()->m_lPreTime = g_nowTime;
			}
			
			// dongs ��Ʀ ���� ���� ���� .. 
			if( nRui->thePcParam.Stat1.theHP >  nRui->thePcParam.MaxHP && (g_Pc.m_MainPC.char_info.theRealHP > 0) )
			{
				nRui->thePcParam.Stat1.theHP = g_Pc.m_MainPC.char_info.theRealHP ;
				g_Pc.GetPlayer()->m_lPreTime = g_nowTime;
			}			
			
			if( underattack_event.type == SM_UNDERATTACK )
			{
				g_Sound.CheckBattleMode(TRUE);
				
				if( g_Pc.GetPlayer()->ReadyUnderAttackMotion( pExtraInfo->Critical ) )
				{					
					if( !g_Pc.GetPlayer()->m_State.bRide )	//���̵� ���� �ƴ϶��
					{
						underattack_event.motion = g_Pc.GetPlayer()->SetUnderAttackMotion( pExtraInfo->TargetInfo.theEffectValue[ 0 ] );
						g_Pc.GetPlayer()->PushQueue( underattack_event );
					}
					else
					{
						g_Pc.GetPlayer()->theVibrationTime = g_nowTime;
						g_Pc.GetPlayer()->theIsVibration = true;
						g_Map.m_Npc[g_Pc.GetPlayer()->m_State.TransIndex].theVibrationTime = g_nowTime;
						g_Map.m_Npc[g_Pc.GetPlayer()->m_State.TransIndex].theIsVibration = true;
					}
				}	
			}
			else if( underattack_event.type == SM_SLIDE_NPC )
			{
				if( !g_Pc.GetPlayer()->m_State.bRide )	//���̵� ���� �ƴ϶��
				{
					underattack_event.motion = n_ReAction_Critical9;
					g_Pc.GetPlayer()->PushQueue( underattack_event, TRUE );				
				}
				else
				{
					g_Pc.GetPlayer()->theVibrationTime = g_nowTime;
					g_Pc.GetPlayer()->theIsVibration = true;
					g_Map.m_Npc[g_Pc.GetPlayer()->m_State.TransIndex].theVibrationTime = g_nowTime;
					g_Map.m_Npc[g_Pc.GetPlayer()->m_State.TransIndex].theIsVibration = true;					
				}
			}
			else if( underattack_event.type == SM_STATE_DIE )
			{
				RLGS("StandTargetHit NonPlaer SM_ATTACK_PC kill = true");				
				underattack_event.motion = n_Die9;			
				
				g_Pc.GetPlayer()->PushQueue( underattack_event, TRUE );									
				
				nRui->thePcParam.Stat1.theHP = 0;
				g_Pc.m_MainPC.char_info.theHP = 0;
				g_Pc.m_MainPC.char_info.theRealHP = 0;
				g_Pc.GetPlayer()->m_lPreTime = g_nowTime;
				
				// ������ ������ �ִ� ����Ʈ ��� �����
				DeletePlayerEffect( pDest );
				
				// ������ ȿ��
				g_Particle.SetEffSetCon( 0.0f , EF_PLAYER_DIE , 0 , 2.4f , pDest->GetUnique() , -1 );			
			}			
			
			// ���� ũ��Ƽ�� ���� �������� ȭ�� ���� Boa ��
			if( pExtraInfo->Critical )
				g_Particle.RockCamera( ( rand() % 200 ) + 400 );
			
		}
		// �ٸ� ���� ó��
		else
		{
			PCTABLE *pTable = ((PCTABLE *)g_Pc.FindPcTable( pDest ));
			
			if( pDest->m_bIsLoaded == true )
			{
				///--JAPAN_BUG_MODIFY
				///-- by simwoosung ���� �׾����� �ʴ°�쿡�� �����Ų��.
				if( !(pDest->IsRealDie()) )
				{
					pDest->m_lPreTime = g_nowTime;
					pTable->char_info.theHP -= pExtraInfo->TargetInfo.theEffectValue[ 0 ];	
				}	
				
				//dongs �ǹ��� ���� ... ��Ʀ ���� .. ���� .. 
				if(pTable->char_info.theHP  > pTable->char_info.theMaxHP && (pTable->char_info.theRealHP > 0) )
				{
					pDest->m_lPreTime = g_nowTime;
					pTable->char_info.theHP = pTable->char_info.theRealHP;
				}
				
				
				(( NonPlayer * )pDest)->theVibrationTime = g_nowTime;
				(( NonPlayer * )pDest)->theIsVibration = true;
				
				if( underattack_event.type == SM_UNDERATTACK )
				{
					if( (( NonPlayer * )pDest)->ReadyUnderAttackMotion( pExtraInfo->Critical ) )
					{
						if( !pDest->m_State.bRide )	//���̵� ���� �ƴ϶��
						{
							underattack_event.motion = (( NonPlayer * )pDest)->SetUnderAttackMotion( pExtraInfo->TargetInfo.theEffectValue[ 0 ] );
							(( NonPlayer * )pDest)->PushQueue(underattack_event, TRUE);
						}
						else
						{
							pDest->theVibrationTime = g_nowTime;
							pDest->theIsVibration = true;
							g_Map.m_Npc[pDest->m_State.TransIndex].theVibrationTime = g_nowTime;
							g_Map.m_Npc[pDest->m_State.TransIndex].theIsVibration = true;
						}
					}					
				}
				else if( underattack_event.type == SM_SLIDE_NPC )
				{
					if( !pDest->m_State.bRide )	//���̵� ���� �ƴ϶��
					{
						underattack_event.motion = n_ReAction_Critical9;
						(( NonPlayer * )pDest)->PushQueue(underattack_event, TRUE);						
					}
					else
					{
						pDest->theVibrationTime = g_nowTime;
						pDest->theIsVibration = true;					
						g_Map.m_Npc[pDest->m_State.TransIndex].theVibrationTime = g_nowTime;
						g_Map.m_Npc[pDest->m_State.TransIndex].theIsVibration = true;						
					}
				}
				else if( underattack_event.type == SM_STATE_DIE )
				{
					underattack_event.motion = n_Die9;
					(( NonPlayer * )pDest)->PushQueue(underattack_event, TRUE);	
					pTable->char_info.theHP = 0;
					pTable->char_info.theRealHP = 0;
					pDest->m_lPreTime = g_nowTime;
					
					// ������ ������ �ִ� ����Ʈ ��� �����
					DeleteOtherPlayerEffect( pDest );
					
					// ������ ȿ��
					g_Particle.SetEffSetCon( 0.0f , EF_PLAYER_DIE , 0 , 2.4f , pDest->GetUnique() , -1 );				
				}
				
			} 			
		}
		
		//PC�� �¾����� �� �������ϰ�� - ����� ���� ������ �����ÿ� 
		//����� �꿡�� �´� ����� �־��ش�.
		if( underattack_event.type == SM_UNDERATTACK || underattack_event.type == SM_SLIDE_NPC )
		{
			if(pDest->m_State.nPetState == nPetState_Master)
			{			
				SNPCBaseInfo* npc_Info = NULL;
				npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( g_Map.m_NpcTable[pDest->m_State.PetIndex].code );
				if( npc_Info && PET_AI_REDUCEDAMAGE == npc_Info->theAttackType )
				{
					g_Map.m_Npc[pDest->m_State.PetIndex].theVibrationTime = g_nowTime;
					g_Map.m_Npc[pDest->m_State.PetIndex].theIsVibration = true;						
					
					if( underattack_event.type == SM_UNDERATTACK )
					{
						if( g_Map.m_Npc[pDest->m_State.PetIndex].ReadyUnderAttackMotion( pExtraInfo->Critical ) )
						{
							underattack_event.motion = g_Map.m_Npc[pDest->m_State.PetIndex].SetUnderAttackMotion( pExtraInfo->TargetInfo.theEffectValue[ 0 ] );							
							g_Map.m_Npc[pDest->m_State.PetIndex].PushQueue( underattack_event, TRUE );	
						}	
					}
					else if( underattack_event.type == SM_SLIDE_NPC )
					{
						underattack_event.motion = n_NPC_ReAction2;						
						g_Map.m_Npc[pDest->m_State.PetIndex].PushQueue(underattack_event, TRUE);							
					}
				}			
			}		
		}
	}
	// NPC ó��
	else
	{
		if( TRUE == pDest->m_bIsLoaded )
		{
			NPCTABLE *pNpcTable;
			
			pNpcTable = g_Map.GetNpcTable( pDest->GetUnique() );
			
			if( pNpcTable != NULL )
			{
				///--JAPAN_BUG_MODIFY
				///-- by simwoosung ���� �׾����� �ʴ°�쿡�� �����Ų��.
				if( !(pDest->IsRealDie()) )
				{
					pDest->m_lPreTime = g_nowTime;
					pNpcTable->theHP -= pExtraInfo->TargetInfo.theEffectValue[ 0 ];
				}	
			}
			//...................................................................................................
			// ������ ó���� ��鸮�� ���ƾ� �Ұ͵� By wxywxy
			//...................................................................................................
			switch( pNpcTable->code )
			{
			case c_Milena_Guad_Point_Npc:
			case c_Rain_Gaud_Point_Npc:
				break;
				
			default:
				(( NonPlayer * )pDest)->theVibrationTime = g_nowTime;
				(( NonPlayer * )pDest)->theIsVibration = true;
				break;
			}
			//...................................................................................................
			
			
			if( underattack_event.type == SM_UNDERATTACK )
			{
				if( pDest->ReadyUnderAttackMotion( pExtraInfo->Critical ) )
				{
					underattack_event.motion = pDest->SetUnderAttackMotion( pExtraInfo->TargetInfo.theEffectValue[ 0 ] );
					
					(( NonPlayer * )pDest)->PushQueue( underattack_event, TRUE );	
				}
			}
			else if( underattack_event.type == SM_SLIDE_NPC )
			{
				underattack_event.motion = n_NPC_ReAction2;
				
				(( NonPlayer * )pDest)->PushQueue(underattack_event, TRUE);					
			}	
			else if( underattack_event.type == SM_STATE_DIE )
			{
				SNPCBaseInfo* npc_Info;
				
				npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( ( g_Map.GetNpcTable( pDest->GetUnique() ))->code ); 
				
				if( -1 != npc_Info->theDieSnd )
					g_Sound.Play( npc_Info->theDieSnd , pDest->GetPosTM()._41 , 
					pDest->GetPosTM()._42 ,
					pDest->GetPosTM()._43 );
				
				//				RLGS("Player SM_ATTACK_NPC kill = true");
				
				// ������ ������ �ִ� ����Ʈ ��� ����� ( NPC�� ������ ��� �� )
				DeleteNPCEffect( pDest );
				
				if( pDest->m_State.nCharacterType == nCharacterTypePlayerNPC )
				{
					underattack_event.motion = n_Die9;
				}
				else
				{
					underattack_event.motion = n_NPC_Die0 + rand()%2;
				}	
				
				(( NonPlayer * )pDest)->PushQueue(underattack_event, TRUE);				
				pNpcTable->theHP = 0;
				pNpcTable->theRealHP = 0;
				pDest->m_lPreTime = g_nowTime;
			}			
		}
	}
}

// �⺻ ���� Ÿ�� ó�� ( ������ ���� & ȿ�� ���� )
void CEffectManager::StandTargetHitOnlyDamage(Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo)
{
	if( pSrc == NULL || pDest == NULL )
		return;
	
	if( pSrc->m_nTableIndex == -1 || pDest->m_nTableIndex == -1 )
		return;	
	
	if( pExtraInfo->TargetInfo.theDieFlag )
	{
		RLGS("pExtraInfo.TargetInfo.theDieFlag StandTargetHit()");
		RLG1("pExtraInfo.TargetInfo.theDieFlag StandTargetHit() pExtraInfo->EventType = %d", pExtraInfo->EventType );
		RLG1("pExtraInfo.TargetInfo.theDieFlag StandTargetHit() pExtraInfo->TargetInfo.theEffectValue[ 0 ] = %d", pExtraInfo->TargetInfo.theEffectValue[ 0 ] );
	}
	
	if( SM_ATTACK_NPC != pExtraInfo->EventType )
		return;	
	
	// ���� �̽� ó��
	if( !(pExtraInfo->TargetInfo.theDieFlag) && pExtraInfo->TargetInfo.theEffectValue[ 0 ] <= 0 )
	{
		// ������ ������ �÷��̾�� �����ִ°͸� ���
		if( IsMyRelationUnit( pSrc , pDest ) )
			g_Particle.SetEffSetCon( 0.0f , EF_ATTACK_MISS , 0 , 0.3f , pDest->GetUnique() , pDest->GetIndex() );
		
		//		RLGS( "\r--- �⺻ ���� ( ���� ) ---\r" );
		
		return;
	}
	
	EDamageLevel Level = DamageLevel( pDest , pExtraInfo->TargetInfo.theEffectValue[ 0 ] , pExtraInfo->Critical );
	
	// ������ ���� ���
	D3DXVECTOR3 vHeight = D3DXVECTOR3(0.0f,0.0f,0.0f);		
	
	if( g_Map.IsGiantUnit( pDest ) )
	{
		vHeight = D3DXVECTOR3( pDest->GetBonCenterTM()._41 ,
			pDest->GetBonCenterTM()._42 + 38.0f, //(pDest->GetBoundMaxPos().y - pDest->GetBoundMinVec().y) * 0.3f, 
			pDest->GetBonCenterTM()._43 );
	}
	else
	{
		
		vHeight = D3DXVECTOR3( pDest->GetPosTM()._41 ,
			pDest->GetPosTM()._42 + ( pDest->GetBoundMaxPos().y * 1.2f ) ,
			pDest->GetPosTM()._43 );
	}
	
	// ������ ������ �÷��̾�� �����ִ°͸� ���
	
	if( IsMyRelationUnit( pSrc , pDest ) )
		g_Particle.m_DamageNum.Add( vHeight  , pExtraInfo->TargetInfo.theEffectValue[ 0 ] , 
		pExtraInfo->Critical, pDest );
	
	
	
	
	EVENT_DATA_INFO underattack_event;	
	
	//............................................................................................................
	// �̺�Ʈ ó��
	//............................................................................................................
	
	// �����̵�
	if( pExtraInfo->TargetInfo.thePushedCell )
	{
		underattack_event.type = SM_SLIDE_NPC;
		underattack_event.cell = pExtraInfo->TargetInfo.thePushedCell;
	}
	else
	{
		// �ױ�
		if( pExtraInfo->TargetInfo.theDieFlag )
		{					
			//			RLGS("pExtraInfo.TargetInfo.theDieFlag underattack_event.type = SM_STATE_DIE;");
			underattack_event.type = SM_STATE_DIE;
			underattack_event.loop = false;
		}
		// �⺻ ��Ÿ��
		else
		{
			underattack_event.type = SM_UNDERATTACK;
		}
	}
	
	if( !pDest->m_bPC && underattack_event.type != SM_STATE_DIE )
	{
		MonsterAttackedSoundPlay( pDest , Level );
	}
	
	//............................................................................................................
	// Ÿ�� ó��
	//............................................................................................................
	int   AttackedEffectCode = -1;
	short AttackedSound      = -1;
	
	// ���� ó��
	if( pDest->m_bPC )
	{
		// �÷��̾� ó��
		if( pDest->GetUnique() == g_Pc.GetMainPCUniqe() )
		{
			
			if( PlayerInfoWnd.IsVisible )
			{
				int RealHP = nRui->thePcParam.Stat1.theHP - pExtraInfo->TargetInfo.theEffectValue[ 0 ];
				PlayerInfoWnd.Start2DEffect(nRui->thePcParam.Stat1.theHP, RealHP);
			}
			
			if( g_DelayBarWnd.IsVisible && DELAY_RIDE_ON == g_DelayBarWnd.GetDelay_Type() )
			{
				g_DelayBarWnd.Clear();	
			}
			
			///--JAPAN_BUG_MODIFY
			///-- by simwoosung ���� �׾����� �ʴ°�쿡�� �����Ų��.
			if( !(pDest->IsRealDie()) )
			{
				nRui->thePcParam.Stat1.theHP -= pExtraInfo->TargetInfo.theEffectValue[ 0 ];
				g_Pc.GetPlayer()->m_lPreTime = g_nowTime;
			}
			
			// dongs ��Ʀ ���� ���� ���� .. 
			if( nRui->thePcParam.Stat1.theHP >  nRui->thePcParam.MaxHP && (g_Pc.m_MainPC.char_info.theRealHP > 0) )
			{
				nRui->thePcParam.Stat1.theHP = g_Pc.m_MainPC.char_info.theRealHP ;
				g_Pc.GetPlayer()->m_lPreTime = g_nowTime;
			}			
			
			if( underattack_event.type == SM_UNDERATTACK )
			{
				g_Sound.CheckBattleMode(TRUE);
				
				if( g_Pc.GetPlayer()->ReadyUnderAttackMotion( pExtraInfo->Critical ) )
				{					
					if( !g_Pc.GetPlayer()->m_State.bRide )	//���̵� ���� �ƴ϶��
					{
						underattack_event.motion = g_Pc.GetPlayer()->SetUnderAttackMotion( pExtraInfo->TargetInfo.theEffectValue[ 0 ] );
						g_Pc.GetPlayer()->PushQueue( underattack_event );
					}
					else
					{
						g_Pc.GetPlayer()->theVibrationTime = g_nowTime;
						g_Pc.GetPlayer()->theIsVibration = true;
						g_Map.m_Npc[g_Pc.GetPlayer()->m_State.TransIndex].theVibrationTime = g_nowTime;
						g_Map.m_Npc[g_Pc.GetPlayer()->m_State.TransIndex].theIsVibration = true;
					}
				}	
			}
			else if( underattack_event.type == SM_SLIDE_NPC )
			{
				if( !g_Pc.GetPlayer()->m_State.bRide )	//���̵� ���� �ƴ϶��
				{
					underattack_event.motion = n_ReAction_Critical9;
					g_Pc.GetPlayer()->PushQueue( underattack_event, TRUE );
					g_Pc.GetPlayer()->SetDirection( pSrc->GetPosTM()._41, pSrc->GetPosTM()._43 );
				}
				else
				{
					g_Pc.GetPlayer()->theVibrationTime = g_nowTime;
					g_Pc.GetPlayer()->theIsVibration = true;
					g_Map.m_Npc[g_Pc.GetPlayer()->m_State.TransIndex].theVibrationTime = g_nowTime;
					g_Map.m_Npc[g_Pc.GetPlayer()->m_State.TransIndex].theIsVibration = true;
					g_Pc.GetPlayer()->SetDirection( pSrc->GetPosTM()._41, pSrc->GetPosTM()._43 );
					g_Map.m_Npc[g_Pc.GetPlayer()->m_State.TransIndex].SetDirection(pSrc->GetPosTM()._41, pSrc->GetPosTM()._43);
				}
			}
			else if( underattack_event.type == SM_STATE_DIE )
			{
				RLGS("StandTargetHit NonPlaer SM_ATTACK_PC kill = true");
				
				if( pSrc->m_curt_event.type != SM_STATE_DIE )
				{
					if( pSrc->m_bPC )
					{
						SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_PC_DIE_BYUSER ), 
															 ((PCTABLE *)g_Pc.FindPcTable( pSrc ))->char_info.thePCName );
					}
					else
					{
						SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_PC_DIE_BYMOB ), 
															 ( g_Map.GetNpcTable( pSrc->GetUnique() ))->szName );
					}
				}
				else
				{
					///-- �׾��µ� �� ������� �޽����� ó���ؾ��Ҷ�
					return;
				}

				underattack_event.motion = n_Die9;			
				
				g_Pc.GetPlayer()->PushQueue( underattack_event, TRUE );									
				
				nRui->thePcParam.Stat1.theHP = 0;
				g_Pc.m_MainPC.char_info.theHP = 0;
				g_Pc.m_MainPC.char_info.theRealHP = 0;
				g_Pc.GetPlayer()->m_lPreTime = g_nowTime;
				
				// ������ ������ �ִ� ����Ʈ ��� �����
				DeletePlayerEffect( pDest );
				
				// ������ ȿ��
				g_Particle.SetEffSetCon( 0.0f , EF_PLAYER_DIE , 0 , 2.4f , pDest->GetUnique() , -1 );
				g_Pc.GetPlayer()->SetDirection( pSrc->GetPosTM()._41, pSrc->GetPosTM()._43 );
			}
			
			
			
			// ���� ũ��Ƽ�� ���� �������� ȭ�� ���� Boa ��
			if( pExtraInfo->Critical )
				g_Particle.RockCamera( ( rand() % 200 ) + 400 );
			
		}
		// �ٸ� ���� ó��
		else
		{
			PCTABLE *pTable = ((PCTABLE *)g_Pc.FindPcTable( pDest ));
			
			if( pDest->m_bIsLoaded == true )
			{
				//by simwoosung
				int index = g_Pc.FindUser(pDest->GetUnique());
				if( index == NonPlayerInfoWnd.thePickedUserID )
				{
					///-- ���� ��ü�� ���ϰ�쿡�� 2D ����Ʈ�� ���´�.
					if( NonPlayerInfoWnd.IsVisible &&  pSrc == g_Pc.GetPlayer() )
					{
						int RealHP = pTable->char_info.theHP - pExtraInfo->TargetInfo.theEffectValue[ 0 ];
						NonPlayerInfoWnd.Start2DEffect(pTable->char_info.theHP, RealHP);	   		
					}
				}		
				
				///--JAPAN_BUG_MODIFY
				///-- by simwoosung ���� �׾����� �ʴ°�쿡�� �����Ų��.
				if( !(pDest->IsRealDie()) )
				{
					pDest->m_lPreTime = g_nowTime;
					pTable->char_info.theHP -= pExtraInfo->TargetInfo.theEffectValue[ 0 ];	
				}	
				
				//dongs �ǹ��� ���� ... ��Ʀ ���� .. ���� .. 
				if(pTable->char_info.theHP  > pTable->char_info.theMaxHP && (pTable->char_info.theRealHP > 0) )
				{
					pDest->m_lPreTime = g_nowTime;
					pTable->char_info.theHP = pTable->char_info.theRealHP;
				}
				
				
				(( NonPlayer * )pDest)->theVibrationTime = g_nowTime;
				(( NonPlayer * )pDest)->theIsVibration = true;
				
				if( underattack_event.type == SM_UNDERATTACK )
				{
					if( (( NonPlayer * )pDest)->ReadyUnderAttackMotion( pExtraInfo->Critical ) )
					{
						if( !pDest->m_State.bRide )	//���̵� ���� �ƴ϶��
						{
							underattack_event.motion = (( NonPlayer * )pDest)->SetUnderAttackMotion( pExtraInfo->TargetInfo.theEffectValue[ 0 ] );
							(( NonPlayer * )pDest)->PushQueue(underattack_event, TRUE);
						}
						else
						{
							pDest->theVibrationTime = g_nowTime;
							pDest->theIsVibration = true;
							g_Map.m_Npc[pDest->m_State.TransIndex].theVibrationTime = g_nowTime;
							g_Map.m_Npc[pDest->m_State.TransIndex].theIsVibration = true;
						}
					}					
				}
				else if( underattack_event.type == SM_SLIDE_NPC )
				{
					if( !pDest->m_State.bRide )	//���̵� ���� �ƴ϶��
					{
						underattack_event.motion = n_ReAction_Critical9;
						(( NonPlayer * )pDest)->PushQueue(underattack_event, TRUE);	
						(( NonPlayer * )pDest)->SetDirection( pSrc->GetPosTM()._41, pSrc->GetPosTM()._43 );
					}
					else
					{
						pDest->theVibrationTime = g_nowTime;
						pDest->theIsVibration = true;
						(( NonPlayer * )pDest)->SetDirection( pSrc->GetPosTM()._41, pSrc->GetPosTM()._43 );
						g_Map.m_Npc[pDest->m_State.TransIndex].theVibrationTime = g_nowTime;
						g_Map.m_Npc[pDest->m_State.TransIndex].theIsVibration = true;
						g_Map.m_Npc[pDest->m_State.TransIndex].SetDirection(pSrc->GetPosTM()._41, pSrc->GetPosTM()._43);
					}
				}
				else if( underattack_event.type == SM_STATE_DIE )
				{
					underattack_event.motion = n_Die9;
					(( NonPlayer * )pDest)->PushQueue(underattack_event, TRUE);	
					pTable->char_info.theHP = 0;
					pTable->char_info.theRealHP = 0;
					pDest->m_lPreTime = g_nowTime;
					
					// ������ ������ �ִ� ����Ʈ ��� �����
					DeleteOtherPlayerEffect( pDest );
					
					// ������ ȿ��
					g_Particle.SetEffSetCon( 0.0f , EF_PLAYER_DIE , 0 , 2.4f , pDest->GetUnique() , -1 );
					(( NonPlayer * )pDest)->SetDirection( pSrc->GetPosTM()._41, pSrc->GetPosTM()._43 );
				}
				
			} 			
		}
		
		//PC�� �¾����� �� �������ϰ�� - ����� ���� ������ �����ÿ� 
		//����� �꿡�� �´� ����� �־��ش�.
		if( underattack_event.type == SM_UNDERATTACK || underattack_event.type == SM_SLIDE_NPC )
		{
			if(pDest->m_State.nPetState == nPetState_Master)
			{			
				SNPCBaseInfo* npc_Info = NULL;
				npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( g_Map.m_NpcTable[pDest->m_State.PetIndex].code );
				if( npc_Info && PET_AI_REDUCEDAMAGE == npc_Info->theAttackType )
				{
					g_Map.m_Npc[pDest->m_State.PetIndex].theVibrationTime = g_nowTime;
					g_Map.m_Npc[pDest->m_State.PetIndex].theIsVibration = true;						
					
					if( underattack_event.type == SM_UNDERATTACK )
					{
						if( g_Map.m_Npc[pDest->m_State.PetIndex].ReadyUnderAttackMotion( pExtraInfo->Critical ) )
						{
							underattack_event.motion = g_Map.m_Npc[pDest->m_State.PetIndex].SetUnderAttackMotion( pExtraInfo->TargetInfo.theEffectValue[ 0 ] );							
							g_Map.m_Npc[pDest->m_State.PetIndex].PushQueue( underattack_event, TRUE );	
						}	
					}
					else if( underattack_event.type == SM_SLIDE_NPC )
					{
						underattack_event.motion = n_NPC_ReAction2;
						
						g_Map.m_Npc[pDest->m_State.PetIndex].PushQueue(underattack_event, TRUE);	
						g_Map.m_Npc[pDest->m_State.PetIndex].SetDirection( pSrc->GetPosTM()._41, pSrc->GetPosTM()._43 );
					}
				}			
			}		
		}
	}
	// NPC ó��
	else
	{
		if( TRUE == pDest->m_bIsLoaded )
		{
			NPCTABLE *pNpcTable;
			
			pNpcTable = g_Map.GetNpcTable( pDest->GetUnique() );
			
			if( pNpcTable != NULL )
			{
				//by simwoosung
				int index = g_Map.FindNpc(pDest->GetUnique());
				if( index == NonPlayerInfoWnd.thePickedNpcID )
				{
					///-- ���� ��ü�� ���ϰ�쿡�� 2D ����Ʈ�� ���´�.
					if( NonPlayerInfoWnd.IsVisible &&  pSrc == g_Pc.GetPlayer() )
					{
						int RealHP = pNpcTable->theHP - pExtraInfo->TargetInfo.theEffectValue[ 0 ];
						NonPlayerInfoWnd.Start2DEffect(pNpcTable->theHP, RealHP);	   		
					}
				}
				
				///--JAPAN_BUG_MODIFY
				///-- by simwoosung ���� �׾����� �ʴ°�쿡�� �����Ų��.
				if( !(pDest->IsRealDie()) )
				{
					pDest->m_lPreTime = g_nowTime;
					pNpcTable->theHP -= pExtraInfo->TargetInfo.theEffectValue[ 0 ];
				}	
			}
			//...................................................................................................
			// ������ ó���� ��鸮�� ���ƾ� �Ұ͵� By wxywxy
			//...................................................................................................
			switch( pNpcTable->code )
			{
			case c_Milena_Guad_Point_Npc:
			case c_Rain_Gaud_Point_Npc:
				break;
				
			default:
				(( NonPlayer * )pDest)->theVibrationTime = g_nowTime;
				(( NonPlayer * )pDest)->theIsVibration = true;
				break;
			}
			//...................................................................................................
			
			
			if( underattack_event.type == SM_UNDERATTACK )
			{
				if( pDest->ReadyUnderAttackMotion( pExtraInfo->Critical ) )
				{
					underattack_event.motion = pDest->SetUnderAttackMotion( pExtraInfo->TargetInfo.theEffectValue[ 0 ] );
					
					(( NonPlayer * )pDest)->PushQueue( underattack_event, TRUE );	
				}
			}
			else if( underattack_event.type == SM_SLIDE_NPC )
			{
				underattack_event.motion = n_NPC_ReAction2;
				
				(( NonPlayer * )pDest)->PushQueue(underattack_event, TRUE);	
				(( NonPlayer * )pDest)->SetDirection( pSrc->GetPosTM()._41, pSrc->GetPosTM()._43 );
			}	
			else if( underattack_event.type == SM_STATE_DIE )
			{
				SNPCBaseInfo* npc_Info;
				
				npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( ( g_Map.GetNpcTable( pDest->GetUnique() ))->code ); 
				
				if( -1 != npc_Info->theDieSnd )
					g_Sound.Play( npc_Info->theDieSnd , pDest->GetPosTM()._41 , 
					pDest->GetPosTM()._42 ,
					pDest->GetPosTM()._43 );
				
				//				RLGS("Player SM_ATTACK_NPC kill = true");
				
				// ������ ������ �ִ� ����Ʈ ��� ����� ( NPC�� ������ ��� �� )
				DeleteNPCEffect( pDest );
				
				if( pDest->m_State.nCharacterType == nCharacterTypePlayerNPC )
				{
					underattack_event.motion = n_Die9;
				}
				else
				{
					underattack_event.motion = n_NPC_Die0 + rand()%2;
				}	
				
				(( NonPlayer * )pDest)->PushQueue(underattack_event, TRUE);	
				(( NonPlayer * )pDest)->SetDirection( pSrc->GetPosTM()._41, pSrc->GetPosTM()._43 );
				pNpcTable->theHP = 0;
				pNpcTable->theRealHP = 0;
				pDest->m_lPreTime = g_nowTime;
			}
			
			
		}
	}
	
}



//...............................................................................................................
// �⺻ ��� ����
//...............................................................................................................
int CEffectManager::MAKE_EF_BIG_SWORD(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	StandTargetHit( pSrc , pDest , pExtraInfo );
	
	return TRUE;
}

//...............................................................................................................
// �⺻ Į ����
//...............................................................................................................
int CEffectManager::MAKE_EF_STAND_SWORD_ATTACK(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	StandTargetHit( pSrc , pDest , pExtraInfo );
	
	return TRUE;
}

//...............................................................................................................
// �ӷϽ� ���� ���� ( ���� )
//...............................................................................................................
int CEffectManager::MAKE_EF_NPC_WOOD_ATTACK(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	StandTargetHit( pSrc , pDest , pExtraInfo );
	
	return TRUE;
}


//...............................................................................................................
// �ӷϽ� ���� ���� ( ������ )
//...............................................................................................................
int CEffectManager::MAKE_EF_NPC_WOOD_ATTACKED(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pDest == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 0.3f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
	
	return TRUE;
}



//...............................................................................................................
// �⺻ å ����
//...............................................................................................................
int CEffectManager::MAKE_EF_STAND_BOOK_ATTACK(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{	
	if( pSrc == NULL )
		return FALSE;
	
	if( pExtraInfo->TargetInfo.theDieFlag )
	{
		RLGS("pExtraInfo.TargetInfo.theDieFlag MAKE_EF_STAND_BOOK_ATTACK()");
		RLG1("pExtraInfo.TargetInfo.theDieFlag MAKE_EF_STAND_BOOK_ATTACK() pExtraInfo->EventType = %d", pExtraInfo->EventType );
		RLG1("pExtraInfo.TargetInfo.theDieFlag MAKE_EF_STAND_BOOK_ATTACK() pExtraInfo->TargetInfo.theEffectValue[ 0 ] = %d", pExtraInfo->TargetInfo.theEffectValue[ 0 ] );
	}
	
	CEffectBase	*pData = NULL;
	
	switch( pSrc->m_ani_index )
	{
	case n_Attack5_Basic0:
	case n_Attack5_Basic1:
	case n_Attack5_Basic3:
	case n_Attack5_Basic2:
		pData = SAFE_NEW( CEFBookAttack );		
		break;
	case n_Attack5_Basic4:
		pData = SAFE_NEW( CEFBookCriticalAttack );		
		break;
	default:
		pData = SAFE_NEW( CEFBookAttack );		
		break;
	}	
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );												// �޴����� �ֱ�
	
	return TRUE;
}

//...............................................................................................................
// �⺻ ������ ����
//...............................................................................................................
int CEffectManager::MAKE_EF_STAND_STICK_ATTACK(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	CEffectBase	*pData = SAFE_NEW( CEFStickAttack );					// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );												// �޴����� �ֱ�
	
	return TRUE;
}

//...............................................................................................................
// �⺻ �̻� ����
//...............................................................................................................
int CEffectManager::MAKE_EF_STAND_TOOTH_ATTACK(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	StandTargetHit( pSrc , pDest , pExtraInfo );
	
	return TRUE;
}

//...............................................................................................................
// �⺻ �б� ����
//...............................................................................................................
int CEffectManager::MAKE_EF_STAND_DULL_ATTACK(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	StandTargetHit( pSrc , pDest , pExtraInfo );
	
	return TRUE;
}

//...............................................................................................................
// �⺻ ���� ����
//...............................................................................................................
int CEffectManager::MAKE_EF_STAND_NAIL_ATTACK(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	StandTargetHit( pSrc , pDest , pExtraInfo );
	
	return TRUE;
}

//...............................................................................................................
// 	��� Į�� �⺻ ������ ( �� )
//...............................................................................................................
int CEffectManager::MAKE_EF_BIG_SWORD0_ED(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL || pDest == NULL )
		return FALSE;
	
	switch( pSrc->m_ani_index )
	{
	case n_Attack0_Basic0:
		g_Particle.SetEffSetCon( 0.0f , 698 , ESLT_SRC_RAD , 0.5f , pDest->GetUnique() , 
			pDest->GetIndex() , ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) , pSrc->GetDirection() + D3DXToRadian( 180 ) );
		break;
	default:
		g_Particle.SetEffSetCon( 0.0f , id , ESLT_SRC_RAD , 0.5f , pDest->GetUnique() , pDest->GetIndex() , 
			::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) , pSrc->GetDirection() + D3DXToRadian( 180 ) );
	}
	
	return TRUE;
}

//...............................................................................................................
// 	��� �⺻ ������ ( �� )
//...............................................................................................................
int CEffectManager::MAKE_EF_BIG_SWORD1_ED(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL || pDest == NULL )
		return FALSE;
	
	switch( pSrc->m_ani_index )
	{
	case n_Attack0_Basic0:
		g_Particle.SetEffSetCon( 0.0f , 698 , ESLT_SRC_RAD , 0.5f , pDest->GetUnique() , 
			pDest->GetIndex() , ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) , pSrc->GetDirection() + D3DXToRadian( 180 ) );
		break;
	default:
		g_Particle.SetEffSetCon( 0.0f , id , ESLT_SRC_RAD , 0.5f , pDest->GetUnique() , pDest->GetIndex() , 
			::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) , pSrc->GetDirection() + D3DXToRadian( 180 ) );
	}
	
	return TRUE;
}

//...............................................................................................................
// 	��� �⺻ ������ ( �� )
//...............................................................................................................
int CEffectManager::MAKE_EF_BIG_SWORD2_ED(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL || pDest == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , ESLT_SRC_RAD , 0.2f , pDest->GetUnique() , 
		pDest->GetIndex() , ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) , pSrc->GetDirection() + D3DXToRadian( 180 ) );
	
	return TRUE;
}


//...............................................................................................................
// 	Į�� �⺻ ������ ( �� )
//...............................................................................................................
int CEffectManager::MAKE_EF_STAND_SWORD_ATTACKED0(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL || pDest == NULL )
		return FALSE;
	
	switch( pSrc->m_ani_index )
	{
	case n_Attack2_Basic0:
		g_Particle.SetEffSetCon( 0.0f , 16 , ESLT_SRC_RAD , 0.5f , pDest->GetUnique() , pDest->GetIndex() , 
			::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) , pSrc->GetDirection() + D3DXToRadian( 180 ) );
		break;
	default:
		g_Particle.SetEffSetCon( 0.0f , id , ESLT_SRC_RAD , 0.5f , pDest->GetUnique() , pDest->GetIndex() , 
			::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) , pSrc->GetDirection() + D3DXToRadian( 180 ) );
	}
	
	return TRUE;
}

//...............................................................................................................
// 	Į�� �⺻ ������ ( �� )
//...............................................................................................................
int CEffectManager::MAKE_EF_STAND_SWORD_ATTACKED1(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL || pDest == NULL )
		return FALSE;
	
	switch( pSrc->m_ani_index )
	{
	case n_Attack2_Basic0:
		g_Particle.SetEffSetCon( 0.0f , 16 , ESLT_SRC_RAD , 0.5f , pDest->GetUnique() , pDest->GetIndex() , 
			::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) , pSrc->GetDirection() + D3DXToRadian( 180 ) );
		break;
	default:
		g_Particle.SetEffSetCon( 0.0f , id , ESLT_SRC_RAD , 0.5f , pDest->GetUnique() , pDest->GetIndex() , 
			::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) , pSrc->GetDirection() + D3DXToRadian( 180 ) );
	}
	
	return TRUE;
}

//...............................................................................................................
// 	Į�� �⺻ ������ ( �� )
//...............................................................................................................
int CEffectManager::MAKE_EF_STAND_SWORD_ATTACKED2(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL || pDest == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , ESLT_SRC_RAD , 0.2f , pDest->GetUnique() , pDest->GetIndex() , 
		::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) , pSrc->GetDirection() + D3DXToRadian( 180 ) );
	
		/*
		if( pSrc->m_ani_index == �ι��� )
		{
		g_Particle.SetEffSetCon( 0.5f , id , ESLT_SRC_RAD , 0.3f , pDest->GetUnique() , pDest->GetIndex() , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) , pSrc->GetDirection() + D3DXToRadian( 180 ) );
		}
	*/
	
	return TRUE;
}

//...............................................................................................................
// 	�����⺻����(å) ������ ( �� ) 
//...............................................................................................................
int CEffectManager::MAKE_EF_STAND_BOOK_ATTACKED0(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pDest == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 0.3f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
	
	return TRUE;
}

//...............................................................................................................
// 	�����⺻����(å) ������ ( �� ) 
//...............................................................................................................
int CEffectManager::MAKE_EF_STAND_BOOK_ATTACKED1(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pDest == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 0.3f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
	
	return TRUE;
}

//...............................................................................................................
// 	�����⺻����(å) ������ ( �� ) 
//...............................................................................................................
int CEffectManager::MAKE_EF_STAND_BOOK_ATTACKED2(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pDest == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 0.3f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
	
	return TRUE;
}

//...............................................................................................................
// 	�⺻ �� ���� ( ������ )
//...............................................................................................................
int CEffectManager::MAKE_EF_HAND_ATTACKED(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pDest == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 2.0f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
	
	return TRUE;
}

//...............................................................................................................
// 	�⺻ �� ���� 
//...............................................................................................................
int CEffectManager::MAKE_EF_HAND_ATTACK0(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	StandTargetHit( pSrc , pDest , pExtraInfo );
	
	return TRUE;
}

//...............................................................................................................
// 	����Ʈ �⺻ ����
//...............................................................................................................
int CEffectManager::MAKE_EF_NIGHT_STAND_ATTACK(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	StandTargetHit( pSrc , pDest , pExtraInfo );
	
	return TRUE;
}

//...............................................................................................................
// 	����Ʈ �⺻ ���� ������
//...............................................................................................................
int CEffectManager::MAKE_EF_NIGHT_STAND_ATTACKED(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pDest == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 2.0f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
	
	return TRUE;
}

//...............................................................................................................
// 	�����̾� �⺻ ����
//...............................................................................................................
int CEffectManager::MAKE_EF_SLAYER_STAND_ATTACK(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	StandTargetHit( pSrc , pDest , pExtraInfo );
	
	return TRUE;
}

//...............................................................................................................
// 	�����̾� �⺻ ���� ������
//...............................................................................................................
int CEffectManager::MAKE_EF_SLAYER_STAND_ATTACKED(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pDest == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 2.0f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
	
	return TRUE;
}

//...............................................................................................................
// 	�����⺻����(������) ������ ( �� ) 
//...............................................................................................................
int CEffectManager::MAKE_EF_STAND_STICK_ATTACKED0(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pDest == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 1.1f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
	
	return TRUE;
}

//...............................................................................................................
// 	�����⺻����(������) ������ ( �� )
//...............................................................................................................
int CEffectManager::MAKE_EF_STAND_STICK_ATTACKED1(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer) 
{
	if( pDest == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 1.1f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
	
	return TRUE;
}

//...............................................................................................................
// 	�����⺻����(������) ������ ( �� )
//...............................................................................................................
int CEffectManager::MAKE_EF_STAND_STICK_ATTACKED2(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer) 
{
	if( pDest == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 1.1f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
	
	return TRUE;
}

//...............................................................................................................
// 	�̻� �⺻ ����   ������ ( �� ) 
//...............................................................................................................
int CEffectManager::MAKE_EF_STAND_TOOTH_ATTACKED0(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pDest == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 1.1f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
	
	return TRUE;
}

//...............................................................................................................
// 	�̻� �⺻ ����   ������ ( �� )
//...............................................................................................................
int CEffectManager::MAKE_EF_STAND_TOOTH_ATTACKED1(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pDest == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 1.1f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
	
	return TRUE;
}

//...............................................................................................................
// 	�̻� �⺻ ����   ������ ( �� )
//...............................................................................................................
int CEffectManager::MAKE_EF_STAND_TOOTH_ATTACKED2(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer) 
{
	if( pDest == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 1.1f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
	
	return TRUE;
}

//...............................................................................................................
// 	�б� �⺻ ����   ������ ( �� ) 
//...............................................................................................................
int CEffectManager::MAKE_EF_STAND_DULL_ATTACKED0(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pDest == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 1.1f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
	
	return TRUE;
}

//...............................................................................................................
// 	�б� �⺻ ����   ������ ( �� )
//...............................................................................................................
int CEffectManager::MAKE_EF_STAND_DULL_ATTACKED1(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer) 
{
	if( pDest == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 1.1f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
	
	return TRUE;
}

//...............................................................................................................
// 	�б� �⺻ ����   ������ ( �� )
//...............................................................................................................
int CEffectManager::MAKE_EF_STAND_DULL_ATTACKED2(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer) 
{
	if( pDest == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 1.1f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
	
	return TRUE;
}

//...............................................................................................................
//	���� �⺻ ����  ������ ( �� )
//...............................................................................................................
int CEffectManager::MAKE_EF_STAND_NAIL_ATTACKED0(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pDest == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 1.1f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
	
	return TRUE;
}

//...............................................................................................................
//	���� �⺻ ����  ������ ( �� )
//...............................................................................................................
int CEffectManager::MAKE_EF_STAND_NAIL_ATTACKED1(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pDest == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 1.1f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
	
	return TRUE;
}

int CEffectManager::MAKE_EF_RUGAL_BASIC_ATTACKED(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pDest == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 1.1f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
	return TRUE;
}

int CEffectManager::MAKE_EF_RUGAL_CRI_ATTACKED(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pDest == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 1.1f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
	return TRUE;
}

int CEffectManager::MAKE_EF_RUGALSOLDART_BASIC_ATTACKED(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pDest == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 1.1f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
	return TRUE;
}

int CEffectManager::MAKE_EF_RUGALSOLDART_CRI_ATTACKED(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pDest == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 1.1f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
	return TRUE;
}

int CEffectManager::MAKE_EF_RUGALTEMPEST_CRI_ATTACKED(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pDest == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 1.1f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
	return TRUE;
}

//...............................................................................................................
//	���� �⺻ ����  ������ ( �� )
//...............................................................................................................
int CEffectManager::MAKE_EF_STAND_NAIL_ATTACKED2(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pDest == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 1.1f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
	
	return TRUE;
}

//...............................................................................................................
//	�ӻ� ( ��ó )
//...............................................................................................................
int CEffectManager::MAKE_EF_QUICK_BOW(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , EF_QUICK_BOW_0 , ESLT_CHARPOS , 2.1f , pSrc->GetUnique() , pSrc->GetIndex() );
	
	//................................................................................	
	// �ߵ� ����
	//................................................................................	
	if( pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	
	return TRUE;
}

//...............................................................................................................
// 	�⺻ ���� ������ ����
//...............................................................................................................
int CEffectManager::MAKE_EF_BALL_ATTACK(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	StandTargetHit( pSrc , pDest , pExtraInfo );
	
	return TRUE;
}

//...............................................................................................................
// 	�⺻ ���� ������ ���� ������
//...............................................................................................................
int CEffectManager::MAKE_EF_BALL_ATTACKED(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pDest == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , ESLT_CHARPOS , 2.1f , 
		pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
	
	return TRUE;
}

//...............................................................................................................
//	ġ���� ȭ�� ( ��ó )
//...............................................................................................................
int CEffectManager::MAKE_EF_REFRESH_BOW(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// ����Ʈ ������ �ҽ��ʸ� ����Ʈ ���
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	CEffectBase	*pData = SAFE_NEW( CEFRefreshBow );				// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );				// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );										// �޴����� �ֱ�
	
	return TRUE;
}

//...............................................................................................................
//	������ ȭ��
//...............................................................................................................
int CEffectManager::MAKE_EF_COMBO_ADD_BOW(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// ����Ʈ ������ �ҽ��ʸ� ����Ʈ ���
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	CEffectBase	*pData = SAFE_NEW( CEFBeeLineBow );				// Ŭ���� ����
	
	((CEFBeeLineBow *)pData)->SetExtra( PT_COMBO_ADD_BOW_TAIL , EF_COMBO_ADD_BOWED , 70 , 3.1f );
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );				// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );										// �޴����� �ֱ�
	
	return TRUE;
}

//...............................................................................................................
//	���� ��
//...............................................................................................................
int CEffectManager::MAKE_EF_HEAL_BUBBLE(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// ����Ʈ ������ �ҽ��ʸ� ����Ʈ ���
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	CEffectBase	*pData = SAFE_NEW( CEFHealBubble );				// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );				// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );										// �޴����� �ֱ�
	
	return TRUE;
}

//..............................................................................................................
// �չ�� ���� ( NPC )
//..............................................................................................................
int CEffectManager::MAKE_EF_BIG_BUBBLE(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// ����Ʈ ������ �ҽ��ʸ� ����Ʈ ���
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	CEffectBase	*pData = SAFE_NEW( CEFBigBubble );				// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );				// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );										// �޴����� �ֱ�
	
	return TRUE;
}

//..............................................................................................................
// SP ���
//..............................................................................................................
int CEffectManager::MAKE_EF_SP_SUCTION(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// ����Ʈ ������ �ҽ��ʸ� ����Ʈ ���
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	CEffectBase	*pData = SAFE_NEW( CEFSPSuction );				// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );				// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );										// �޴����� �ֱ�
	
	return TRUE;
}

//..............................................................................................................
// ������ �Ա�
//..............................................................................................................
int CEffectManager::MAKE_EF_ICE_RIP_BLOW(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// ����Ʈ ������ �ҽ��ʸ� ����Ʈ ���
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	CEffectBase	*pData = SAFE_NEW( CEFIceRipBlow );				// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );				// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );										// �޴����� �ֱ�
	
	return TRUE;	
}

//..............................................................................................................
// ����ȭ
//..............................................................................................................
int CEffectManager::MAKE_EF_MAGIC_DELETE(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// ����Ʈ ������ �ҽ��ʸ� ����Ʈ ���
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	CEffectBase	*pData = SAFE_NEW( CEFMagicDelete );			// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );				// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );										// �޴����� �ֱ�
	
	return TRUE;	
}


//..............................................................................................................
// HP ���
//..............................................................................................................
int CEffectManager::MAKE_EF_HP_SUCTION(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// ����Ʈ ������ �ҽ��ʸ� ����Ʈ ���
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	CEffectBase	*pData = SAFE_NEW( CEFHPSuction );				// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );				// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );										// �޴����� �ֱ�
	
	return TRUE;
}


//..............................................................................................................
// ħ���� ȭ��
//..............................................................................................................
int CEffectManager::MAKE_EF_WHISPER_BOW(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// ����Ʈ ������ �ҽ��ʸ� ����Ʈ ���
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	CEffectBase	*pData = SAFE_NEW( CEFBeeLineBow );				// Ŭ���� ����
	
	((CEFBeeLineBow *)pData)->SetExtra( PT_WHISPER_BOW_TAIL , EF_WHISPER_BOWED , 70 , 3.1f );
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );				// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );										// �޴����� �ֱ�
	
	return TRUE;
}

//..............................................................................................................
// ������ ȭ��
//..............................................................................................................
int CEffectManager::MAKE_EF_CANCEL_BOW(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// ����Ʈ ������ �ҽ��ʸ� ����Ʈ ���
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	CEffectBase	*pData = SAFE_NEW( CEFBeeLineBow );				// Ŭ���� ����
	
	((CEFBeeLineBow *)pData)->SetExtra( PT_CANCEL_BOW_TAIL , EF_CANCEL_BOWED , 70 , 3.1f );
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );				// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );										// �޴����� �ֱ�
	
	return TRUE;
}

//..............................................................................................................
// ��ȭ�� ȭ��
//..............................................................................................................
int CEffectManager::MAKE_EF_CLEAR_BOW(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// ����Ʈ ������ �ҽ��ʸ� ����Ʈ ���
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	CEffectBase	*pData = SAFE_NEW( CEFBeeLineBow );						// Ŭ���� ����
	
	((CEFBeeLineBow *)pData)->SetExtra( PT_CLEAR_BOW_TAIL , EF_CLEAR_BOWED , 70 , 3.1f );
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );				// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );										// �޴����� �ֱ�
	
	return TRUE;
}

//..............................................................................................................
// ����, �������� Ȱ-���뽺ų
//..............................................................................................................
int CEffectManager::MAKE_BEE_LINE_BOW(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo, int nTailNum, int nDNum, 
									  float fDTime, float fDelTime, int nDType)
{
	// ����Ʈ ������ �ҽ��ʸ� ����Ʈ ���
	//if( pExtraInfo == NULL || pDest == NULL )
	//	return TRUE;
	
	CEffectBase	*pData = SAFE_NEW( CEFBeeLineBow );						// Ŭ���� ����
	
	((CEFBeeLineBow *)pData)->SetExtra( nTailNum , nDNum , fDTime , fDelTime, nDType );
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );				// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );										// �޴����� �ֱ�
	
	return TRUE;	
}

//...............................................................................................................
//	���̽� ���� ����
//...............................................................................................................
int CEffectManager::MAKE_EF_ICE_RAINED(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , EF_ICE_RAINED , 0 , 5.1f , pSrc->GetUnique() , pSrc->GetIndex() );
	
	return TRUE;
}

//...............................................................................................................
//	FireRain ���� 
//...............................................................................................................
int CEffectManager::MAKE_EF_FIRE_RAINED(EFFECT_ID id , Character *pSrc , Character *pDest, LPEFFECT_EXTRA_INFO pExtraInfo , float Timer )
{
	
	if( pSrc == NULL)
		return FALSE ;
	
	g_Particle.SetEffSetCon( 0.0f , EF_FIRE_RAINED , 0 , 5.1f , pSrc->GetUnique() , pSrc->GetIndex() );
	
	return TRUE;
	
}



//...............................................................................................................
//	���� ( ��ó )
//...............................................................................................................
int CEffectManager::MAKE_EF_FOCUS_BOW(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , EF_FOCUS_BOW_0 , ESLT_CHARPOS , 2.1f , pSrc->GetUnique() , pSrc->GetIndex() );
	
	//................................................................................	
	// �ߵ� ����
	//................................................................................	
	if( pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	
	return TRUE;
}

//..............................................................................................................
// �̱״ϼ�
//..............................................................................................................
int CEffectManager::MAKE_EF_EGNEETION(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	//................................................................................	
	// �ߵ� ����
	//................................................................................	
	if( pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	
	
	if( pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillAttackedSound( pExtraInfo->SkillCode , pDest );
	
	EffectTargetHit( pSrc , pDest , pExtraInfo );
	
	return TRUE;
}

//..............................................................................................................
// �̱״ϼ� ������ ( ���� )
//..............................................................................................................
int CEffectManager::MAKE_EF_EGNEETION_ED(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	CEffectBase *pData = SAFE_NEW( CEFSrcLink );	
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );				// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );										// �޴����� �ֱ�
	
	return TRUE;
}

//..............................................................................................................
// ����
//..............................................................................................................
int CEffectManager::MAKE_EF_CHARGE(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	CEffectBase *pData = SAFE_NEW( CEFSrcLink );	
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );				// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );										// �޴����� �ֱ�
	
	//................................................................................	
	// �ߵ� ����
	//................................................................................	
	if( pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	
	return TRUE;
}

//...............................................................................................................
//	���� ���
//...............................................................................................................
int CEffectManager::MAKE_EF_ABSOLUTE_DP(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , EF_ABSOLUTE_DP_0 , ESLT_CHARPOS , 2.1f , pSrc->GetUnique() , pSrc->GetIndex() );
	
	//................................................................................	
	// �ߵ� ����
	//................................................................................	
	if( pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );	
	
	return TRUE;
}

//...............................................................................................................
//	������ ����
//...............................................................................................................
int CEffectManager::MAKE_EF_FOOT_STRENGTH(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	CEffectBase	*pData = SAFE_NEW( CEFFootStrength );			// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );				// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );										// �޴����� �ֱ�
	
	return TRUE;
}

//...............................................................................................................
//	������ �Ǽ�
//...............................................................................................................
int CEffectManager::MAKE_EF_BUNCH_STRENGTH(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	CEffectBase	*pData = SAFE_NEW( CEFBunchStrength );			// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );				// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );										// �޴����� �ֱ�
	
	return TRUE;
}

//...............................................................................................................
//	�ذ������ ��ų
//...............................................................................................................
int CEffectManager::MAKE_EF_BONWARSKILL(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	CEffectBase	*pData = SAFE_NEW( CEFBonWarSkill );			// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );				// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );										// �޴����� �ֱ�	
	
	return TRUE;
}

//...............................................................................................................
//	�ذ���Ŀ� ��ų
//...............................................................................................................
int CEffectManager::MAKE_EF_BONARCSKILL(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	CEffectBase	*pData = SAFE_NEW( CEFBonArcSkill );			// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );				// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );										// �޴����� �ֱ�	
	
	return TRUE;
}



//...............................................................................................................
//	Ŭ���� ���������� - ���� ����Ʈ
//...............................................................................................................
int CEffectManager::MAKE_EF_SPECIAL_ITEM_EFFECT(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	CEffectBase	*pData = SAFE_NEW( CEFSpeacialItem );			// Ŭ���� ����

	pData->Setup( id , pSrc , pDest , pExtraInfo );				// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );										// �޴����� �ֱ�	
	
	return TRUE;
}

//...............................................................................................................
//	�ռ� ���پ����� - ���� ����Ʈ
//...............................................................................................................
int CEffectManager::MAKE_EF_BASE_ITEM_EFFECT(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{

	CEffectBase	*pData = SAFE_NEW( CEFEldaWeaponItem );			// Ŭ���� ����

	pData->Setup( id , pSrc , pDest , pExtraInfo );				// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );										// �޴����� �ֱ�	

	return TRUE;
}




//...............................................................................................................
//	�簥 ��ų
//...............................................................................................................
int CEffectManager::MAKE_EF_RUGALEFFECT(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{

	D3DXVECTOR3 vPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vTempPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vDir = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	D3DXMATRIX matRotY;

	int DestSetNum = 0;
	int DestSetType = 0; 
	float DestTimer = 5.1f;
	

	
	if( id == EF_RUGAL_BASIC_ATTACK ||		
		id == EF_RUGALSOLDART_BASIC_ATTACK )		
	{
		if( CheckCharacter( pSrc ) )
		{
			switch( id )
			{
				case EF_RUGAL_BASIC_ATTACK:
					{
						if( pExtraInfo->Critical )
						{							
						//	g_Particle.m_IsGUnitLinkBoneAndSizeFix = TRUE;
							g_Particle.m_fAddSize = -0.5f;
							g_Particle.SetEffSetCon( 0.0f , EF_RUGAL_CRI_ATTACK , ESLT_CHARPOS , 
							2.1f , pSrc->GetUnique() , pSrc->GetIndex());

							DestSetNum = EF_RUGAL_CRI_ATTACKED;
							DestSetType = ESLT_CHARPOS;
						}
						else
						{

							//g_Particle.m_IsGUnitLinkBoneAndSizeFix = TRUE;
							//g_Particle.m_fAddSize = 0.8f;
							g_Particle.SetEffSetCon( 0.0f , EF_RUGAL_BASIC_ATTACK , ESLT_CHARPOS , 2.1f , pSrc->GetUnique() , pSrc->GetIndex() 
								, D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc->GetDirection()  );
											
							DestSetNum = EF_RUGAL_BASIC_ATTACKED;
							DestSetType = ESLT_CHARPOS;
						}
					}
					break;
				case EF_RUGALSOLDART_BASIC_ATTACK:
					{
						if( pExtraInfo->Critical )
						{							
							vPos.x = pSrc->GetPosTM()._41;
							vPos.y = pSrc->GetPosTM()._42;
							vPos.z = pSrc->GetPosTM()._43;							
							/*
							vTempPos = vPos;							
							
							D3DXMatrixRotationY( &matRotY , pSrc->GetDirection() );
							D3DXVec3TransformCoord( &vDir , &vDir , &matRotY );
							vPos += ( 100.0f * vDir );
							
							vTempPos += ( 75.0f * vDir );
							vTempPos.y = g_Map.Get__MapHeightF( vTempPos.x , vTempPos.z );
							g_Particle.SetEffSetCon( 0.0f , EF_GIANT_GOLEM_ATTACK , 
													ESLT_POS , 1.5f , -1 , -1 , vTempPos, 0.0f );

							vTempPos += ( 75.0f * vDir );
							vTempPos.y = g_Map.Get__MapHeightF( vTempPos.x , vTempPos.z );
							g_Particle.SetEffSetCon( 0.0f , EF_GIANT_GOLEM_ATTACK , 
													ESLT_POS , 1.5f , -1 , -1 , vTempPos, 0.0f );							
							*/
							g_Particle.m_fAddSize = -0.5f;
							g_Particle.SetEffSetCon( 0.0f , EF_RUGALSOLDART_CRI_ATTACK , ESLT_POS , 
							5.1f , pSrc->GetUnique() , pSrc->GetIndex(), vPos, pSrc->GetDirection() );							
							
							DestSetNum = EF_RUGALSOLDART_CRI_ATTACKED;
							DestSetType = ESLT_CHARPOS;
						}
						else
						{
						//	g_Particle.m_IsGUnitLinkBoneAndSizeFix = TRUE;
							g_Particle.m_fAddSize = -0.5f;
							g_Particle.SetEffSetCon( 0.0f , EF_RUGALSOLDART_BASIC_ATTACK , ESLT_CHARPOS , 2.1f , pSrc->GetUnique() , pSrc->GetIndex() 
								, D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc->GetDirection()  );
						
							DestSetNum = EF_RUGALSOLDART_BASIC_ATTACKED;
							DestSetType = ESLT_CHARPOS;
						}
					}
					break;
			
			}		
			
			if( NULL != pExtraInfo )
			{
				//................................................................................	
				// ��ų �ߵ� ����
				//................................................................................	
				g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
			}

			if( CheckCharacter( pSrc , pDest ) && NULL != pExtraInfo )
			{
				StandTargetHit( pSrc , pDest , pExtraInfo );
				
				if(-1 != DestSetNum)
				{
					g_Particle.SetEffSetCon( 0.0f , DestSetNum , DestSetType , DestTimer , 
						pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );				
				}
			}
		}		
		
		return TRUE;
	}	
	else if(id == EF_RUGAL_RECALL_SOLTARED)
	{

		vPos.x = pSrc->GetPosTM()._41;
		vPos.y = pSrc->GetPosTM()._42;
		vPos.z = pSrc->GetPosTM()._43;						
		
		g_Particle.SetEffSetCon( 0.0f , EF_RUGAL_RECALL_SOLTARED , ESLT_POS , 
			5.1f , -1 , -1 , vPos, pSrc->GetDirection() );


		return TRUE;
	}


	CEffectBase	*pData = SAFE_NEW( CEFRugalSkill );			// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );				// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );
	
	return TRUE;
}

int CEffectManager::MAKE_EF_DOGMAEFFECT(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	
	D3DXVECTOR3 vPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vTempPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vDir = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	D3DXMATRIX matRotY;

	int DestSetNum = 0;
	int DestSetType = 0; 
	float DestTimer = 5.1f;
	
	if( id == EF_DOGMA_L1_BASIC_ATTACK ||		
		id == EF_DOGMA_L2_BASIC_ATTACK ||		
		id == EF_DOGMA_L3_BASIC_ATTACK  )
	{
		if( CheckCharacter( pSrc ) )
		{
			switch( id )
			{
				case EF_DOGMA_L1_BASIC_ATTACK:
					{
						if( pExtraInfo->Critical )
						{							
							g_Particle.m_IsGUnitLinkBoneAndSizeFix = TRUE;
							g_Particle.m_fAddSize = 1.5f;
							g_Particle.SetEffSetCon( 0.0f , EF_DOGMA_L1_CRI_ATTACK , ESLT_CHARPOS , 
							10.1f , pSrc->GetUnique() , pSrc->GetIndex(), 
							D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc->GetDirection() );

							DestSetNum = EF_DOGMA_L1_CRI_ATTACKED;
							DestSetType = ESLT_CHARPOS;
						}
						else
						{
							vPos.x = pSrc->GetPosTM()._41;
							vPos.y = pSrc->GetPosTM()._42;
							vPos.z = pSrc->GetPosTM()._43;							
							
							D3DXMatrixRotationY( &matRotY , pSrc->GetDirection() );
							D3DXVec3TransformCoord( &vDir , &vDir , &matRotY );
							vPos += ( 55.0f * vDir );
							
							vPos.y = g_Map.Get__MapHeightF( vPos.x , vPos.z );
							g_Particle.m_fAddSize = 2.0f;
							g_Particle.SetEffSetCon( 0.0f , EF_DOGMA_L1_BASIC_ATTACK , ESLT_POS , 
							10.1f , -1 , -1 , vPos , 0.0f );

							DestSetNum = EF_GIANT_GOLEM_ATTACKED;
							DestSetType = ESLT_CHARPOS;
						}
					}
					break;
				case EF_DOGMA_L2_BASIC_ATTACK:
					{
						if( pExtraInfo->Critical )
						{							
							vPos.x = pSrc->GetPosTM()._41;
							vPos.y = pSrc->GetPosTM()._42;
							vPos.z = pSrc->GetPosTM()._43;							
							
							vTempPos = vPos;							
							
							D3DXMatrixRotationY( &matRotY , pSrc->GetDirection() );
							D3DXVec3TransformCoord( &vDir , &vDir , &matRotY );
							vPos += ( 100.0f * vDir );
							
							vTempPos += ( 75.0f * vDir );
							vTempPos.y = g_Map.Get__MapHeightF( vTempPos.x , vTempPos.z );
							g_Particle.SetEffSetCon( 0.0f , EF_GIANT_GOLEM_ATTACK , 
													ESLT_POS , 1.5f , -1 , -1 , vTempPos, 0.0f );

							vTempPos += ( 75.0f * vDir );
							vTempPos.y = g_Map.Get__MapHeightF( vTempPos.x , vTempPos.z );
							g_Particle.SetEffSetCon( 0.0f , EF_GIANT_GOLEM_ATTACK , 
													ESLT_POS , 1.5f , -1 , -1 , vTempPos, 0.0f );							
							
							g_Particle.m_fAddSize = 2.0f;
							g_Particle.SetEffSetCon( 0.0f , EF_DOGMA_L2_CRI_ATTACK , ESLT_POS , 
							10.1f , -1 , -1 , vPos, pSrc->GetDirection() );							
							
							DestSetNum = EF_DOGMA_L2_CRI_ATTACKED;
							DestSetType = ESLT_CHARPOS;
						}
						else
						{
							g_Particle.m_IsGUnitLinkBoneAndSizeFix = TRUE;
							g_Particle.m_fAddSize = 2.0f;
							g_Particle.SetEffSetCon( 0.0f , EF_DOGMA_L2_BASIC_ATTACK , ESLT_CHARPOS , 
							10.1f , pSrc->GetUnique() , pSrc->GetIndex(), 
							D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc->GetDirection() );										
							
							DestSetNum = EF_GIANT_GOLEM_ATTACKED;
							DestSetType = ESLT_CHARPOS;
						}
					}
					break;
				case EF_DOGMA_L3_BASIC_ATTACK:
					{
						if( pExtraInfo->Critical )
						{							
							vPos.x = pSrc->GetPosTM()._41;
							vPos.y = pSrc->GetPosTM()._42;
							vPos.z = pSrc->GetPosTM()._43;							
							
							vTempPos = vPos;							
							
							D3DXMatrixRotationY( &matRotY , pSrc->GetDirection() );
							D3DXVec3TransformCoord( &vDir , &vDir , &matRotY );
							vPos += ( 100.0f * vDir );						
							
							g_Particle.m_fAddSize = 3.5f;
							g_Particle.SetEffSetCon( 0.0f , EF_DOGMA_L3_CRI_ATTACK , ESLT_POS , 
							10.1f , -1 , -1 , vPos, pSrc->GetDirection() );							
							
							DestSetNum = EF_GIANT_GOLEM_ATTACKED;
							DestSetType = ESLT_CHARPOS;
						}
						else
						{						
							g_Particle.m_IsGUnitLinkBoneAndSizeFix = TRUE;
							g_Particle.m_fAddSize = 0.5f;
							g_Particle.SetEffSetCon( 0.0f , EF_DOGMA_L3_BASIC_ATTACK , ESLT_CHARPOS , 
							10.1f , pSrc->GetUnique() , pSrc->GetIndex(), 
							D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc->GetDirection() );							
							
							g_Particle.m_IsGUnitLinkBoneAndSizeFix = TRUE;
							g_Particle.m_fAddSize = 1.5f;
							g_Particle.SetEffSetCon( 0.0f , EF_DOGMA_L3_BASIC_ATTACK , ESLT_CHARPOS , 
							10.1f , pSrc->GetUnique() , pSrc->GetIndex(), 
							D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc->GetDirection() );								
							
							g_Particle.m_IsGUnitLinkBoneAndSizeFix = TRUE;
							g_Particle.m_fAddSize = 2.5f;
							g_Particle.SetEffSetCon( 0.0f , EF_DOGMA_L3_BASIC_ATTACK , ESLT_CHARPOS , 
							10.1f , pSrc->GetUnique() , pSrc->GetIndex(), 
							D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc->GetDirection() );											
							
							DestSetNum = EF_GIANT_GOLEM_ATTACKED;
							DestSetType = ESLT_CHARPOS;
						}
					}
					break;
			}		
			
			if( NULL != pExtraInfo )
			{
				//................................................................................	
				// ��ų �ߵ� ����
				//................................................................................	
				g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
			}

			if( CheckCharacter( pSrc , pDest ) && NULL != pExtraInfo )
			{
				StandTargetHit( pSrc , pDest , pExtraInfo );
				
				if(-1 != DestSetNum)
				{
					g_Particle.SetEffSetCon( 0.0f , DestSetNum , DestSetType , DestTimer , 
						pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );				
				}
			}
		}		
		
		return TRUE;
	}	
	
	CEffectBase	*pData = SAFE_NEW( CEFDogmaEffect );			// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );				// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );										// �޴����� �ֱ�	
	
	return TRUE;
}

//	�ٶ�� ���� ����Ʈ
int CEffectManager::MAKE_EF_BARAGOSEFFECT(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	D3DXVECTOR3 vPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	
	int DestSetNum = 0;
	int DestSetType = 0; 
	float DestTimer = 5.1f;
	
	if( id == EF_BARAGOS_BASIC_ATTACK )
	{
		if( CheckCharacter( pSrc ) )
		{
			if( pExtraInfo->Critical )
			{
				
				vPos.x = pSrc->GetPosTM()._41;
				vPos.y = pSrc->GetPosTM()._42;
				vPos.z = pSrc->GetPosTM()._43;		
				
				g_Particle.m_IsGUnitLinkBoneAndSizeFix = TRUE;	
				g_Particle.m_fAddSize = 1.5f;
				g_Particle.SetEffSetCon( 0.0f , EF_BARAGOS_CRI_ATTACK , ESLT_POS , 
					10.1f , -1 , -1 , vPos, pSrc->GetDirection() );

				DestSetNum = EF_BARAGOS_CRI_ATTACKED;
				DestSetType = ESLT_CPOS_ICAMERA;
			}
			else
			{
				vPos.x = pSrc->GetPosTM()._41;
				vPos.y = pSrc->GetPosTM()._42;
				vPos.z = pSrc->GetPosTM()._43;		
				
				g_Particle.m_IsGUnitLinkBoneAndSizeFix = TRUE;	
				g_Particle.m_fAddSize = 1.0f;
				g_Particle.SetEffSetCon( 0.0f , EF_BARAGOS_BASIC_ATTACK , ESLT_POS , 
					10.1f , -1 , -1 , vPos, pSrc->GetDirection() );

				DestSetNum = EF_BARAGOS_BASIC_ATTACKED;
				DestSetType = ESLT_CPOS_ICAMERA;
			}
			
			if( NULL != pExtraInfo )
			{
				//................................................................................	
				// ��ų �ߵ� ����
				//................................................................................	
				g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
			}

			if( CheckCharacter( pSrc , pDest ) && NULL != pExtraInfo )
			{
				StandTargetHit( pSrc , pDest , pExtraInfo );
				
				if(-1 != DestSetNum)
				{
					g_Particle.SetEffSetCon( 0.0f , DestSetNum , DestSetType , DestTimer , 
						pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );				
				}
			}
		}
	
		return TRUE;
	}	
	
	CEffectBase	*pData = SAFE_NEW( CEFBaragosEff );				// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );				// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );										// �޴����� �ֱ�

	return TRUE;
}




int CEffectManager::MAKE_EF_JANADEFFECT(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{

	D3DXVECTOR3 vPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	
	int DestSetNum = 0;
	int DestSetType = 0; 
	float DestTimer = 5.1f;
	
	//��ȯ ����Ʈ
	if( EF_JANAD_CALLDEVILSOULED == id )
	{
		vPos.x = pSrc->GetPosTM()._41;
		vPos.y = pSrc->GetPosTM()._42;
		vPos.z = pSrc->GetPosTM()._43;						
		
		g_Particle.SetEffSetCon( 0.0f , EF_JANAD_CALLDEVILSOULED , ESLT_POS , 
			5.1f , -1 , -1 , vPos, pSrc->GetDirection() );
		return TRUE;
	}
	else if( EF_JANAD_BASIC_ATTACK == id )
	{
		if( CheckCharacter( pSrc ) )
		{
			if( pExtraInfo->Critical )
			{				
				vPos.x = pSrc->GetPosTM()._41;
				vPos.y = pSrc->GetPosTM()._42;
				vPos.z = pSrc->GetPosTM()._43;						
				
				g_Particle.SetEffSetCon( 0.0f , EF_JANAD_CRI_ATTACK , ESLT_POS , 
					10.1f , -1 , -1 , vPos, pSrc->GetDirection() );

				DestSetNum = EF_JANAD_CRI_ATTACKED;
				DestSetType = ESLT_CPOS_ICAMERA;
			}
			else
			{
				vPos.x = pSrc->GetPosTM()._41;
				vPos.y = pSrc->GetPosTM()._42;
				vPos.z = pSrc->GetPosTM()._43;		
				
				g_Particle.m_IsGUnitLinkBoneAndSizeFix = TRUE;	
				g_Particle.m_fAddSize = 1.0f;
				g_Particle.SetEffSetCon( 0.0f , EF_JANAD_BASIC_ATTACK , ESLT_POS , 
					10.1f , -1 , -1 , vPos, pSrc->GetDirection() );

				DestSetNum = EF_JANAD_BASIC_ATTACKED;
				DestSetType = ESLT_CPOS_ICAMERA;
			}
			
			if( NULL != pExtraInfo )
			{
				//................................................................................	
				// ��ų �ߵ� ����
				//................................................................................	
				g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
			}

			if( CheckCharacter( pSrc , pDest ) && NULL != pExtraInfo )
			{
				StandTargetHit( pSrc , pDest , pExtraInfo );
				
				if(-1 != DestSetNum)
				{
					g_Particle.SetEffSetCon( 0.0f , DestSetNum , DestSetType , DestTimer , 
						pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );				
				}
			}
		}
	
		return TRUE;
	}		
	
	CEffectBase	*pData = SAFE_NEW( CEFJanadEff );				// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );				// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );


	return TRUE;
}


int CEffectManager::MAKE_EF_UROBOROSEFFECT(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	
	D3DXVECTOR3 vPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	
	int DestSetNum = 0;
	int DestSetType = 0; 
	float DestTimer = 5.1f;
	
	if( EF_UROBOROS_BASIC_ATTACK == id )
	{
		if( CheckCharacter( pSrc ) )
		{
			if( pExtraInfo->Critical )
			{				
				vPos.x = pSrc->GetPosTM()._41;
				vPos.y = pSrc->GetPosTM()._42;
				vPos.z = pSrc->GetPosTM()._43;						
				
				g_Particle.SetEffSetCon( 0.0f , EF_UROBOROS_CRI_ATTACK , ESLT_POS , 
					5.1f , -1 , -1 , vPos );
				
				DestSetNum = EF_UROBOROS_CRI_ATTACKED;
				DestSetType = ESLT_CHARPOS;
			}
			else
			{
				//��κ��� �⺻ ���� ���� .. 
			/*
				vPos.x = pSrc->GetPosTM()._41;
				vPos.y = pSrc->GetPosTM()._42;
				vPos.z = pSrc->GetPosTM()._43;		
				
				g_Particle.m_IsGUnitLinkBoneAndSizeFix = TRUE;	
				g_Particle.m_fAddSize = 1.0f;
				g_Particle.SetEffSetCon( 0.0f , EF_UROBOROS_BASIC_ATTACK , ESLT_POS , 
					10.1f , -1 , -1 , vPos, pSrc->GetDirection() );
			*/
				
				DestSetNum = EF_UROBOROS_BASIC_ATTACKED;
				DestSetType = ESLT_CHARPOS;
			}
			
			if( NULL != pExtraInfo )
			{
				//................................................................................	
				// ��ų �ߵ� ����
				//................................................................................	
				g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
			}
			
			if( CheckCharacter( pSrc , pDest ) && NULL != pExtraInfo )
			{
				StandTargetHit( pSrc , pDest , pExtraInfo );
				
				if(-1 != DestSetNum)
				{
					g_Particle.SetEffSetCon( 0.0f , DestSetNum , DestSetType , DestTimer , 
						pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );				
				}
			}
		}
		
		return TRUE;
	}		
	
	CEffectBase	*pData = SAFE_NEW( CEFUrobosEff );				// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );				// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );
	
	
	return TRUE;
}



//dongtest 

int CEffectManager::MAKE_EF_DONGS_TEST(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	
	CEffectBase	*pData = SAFE_NEW( CEFFameItem );				// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );				// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );										// �޴����� �ֱ�
	
	return TRUE;
}


//..............................................................................................................
// �ż��� ���� 
//..............................................................................................................
int CEffectManager::MAKE_EF_HOLY_BOMB(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE ;
	
	CEffectBase	*pData = SAFE_NEW( CEFHolyBomb );				// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );				// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );										// �޴����� �ֱ�
	return TRUE;	
}


//..............................................................................................................
// ����� ���� 
//..............................................................................................................
int CEffectManager::MAKE_EF_DARK_BOMB(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	CEffectBase	*pData = SAFE_NEW( CEFDarkBomb );				// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );				// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );										// �޴����� �ֱ�
	return TRUE;	
}


//..............................................................................................................
// �г� ���� 
//..............................................................................................................
int CEffectManager::MAKE_EF_WRATH_BOMB(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	
	g_Particle.SetEffSetCon( 0.0f , EF_WRATH_BOMB , ESLT_SRC_RAD , 2.1f , pSrc->GetUnique() , pSrc->GetIndex() , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) , pSrc->GetDirection() );
	
	//................................................................................	
	// ��ų �ߵ� ����
	//................................................................................	
	g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	
	// ����Ʈ ������ �ҽ��ʸ� ����Ʈ ���
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	// 1:N ������
	int Count = pExtraInfo->GetTargetCount();
	
	Character			*pTempDest = NULL;
	EFFECT_EXTRA_INFO    Info;
	
	for( int i = 0 ; i < Count ; ++i )
	{
		if( pExtraInfo->GetTargetInfo( &pTempDest , &Info , i ) )
		{
			g_Particle.SetEffSetCon( 0.0f , EF_WRATH_BOMB_ED , 0 , 2.6f , 
				pTempDest->GetUnique() , pTempDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
			
			Info.AfterEffect = 	EF_WRATH_BOMB_ED	;
			Info.DestType = 0;
			Info.lifeTime = 2.6f;
			
			EffectTargetHit( pSrc , pTempDest , &Info );
		}
	}
	
	//................................................................................	
	// ���� ����
	//................................................................................	
	if( pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillAttackedSound( pExtraInfo->SkillCode , pDest );		
	
	return TRUE;
}
//..............................................................................................................
// �尩 �ı� 
//..............................................................................................................
int CEffectManager::MAKE_EF_GLOVE_DESTRUCTION(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	
	if( pSrc == NULL || pDest == NULL)
		return FALSE ;
	
	CEffectBase	*pData = SAFE_NEW( CEFGloveDestruction );		// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );				// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );										// �޴����� �ֱ�
	
	return TRUE;
}
//..............................................................................................................
// ��ũ�Ͻ� ���Ǿ�  
//..............................................................................................................
int CEffectManager::MAKE_EF_ONE_DARKNESS(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	
	if( pSrc == NULL || pDest == NULL)
		return FALSE ;
	
	CEffectBase	*pData = SAFE_NEW( CEFDarkness );				// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );				// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );										// �޴����� �ֱ�
	
	return TRUE;
}
//..............................................................................................................
// Ʈ���� ��ũ�Ͻ� 
//..............................................................................................................
int CEffectManager::MAKE_EF_TRIPLE_DARKNESS(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	
	if( pSrc == NULL || pDest == NULL)
		return FALSE ;
	
	CEffectBase	*pData = SAFE_NEW( CEFTripleDarkness );			// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );				// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );										// �޴����� �ֱ�
	return TRUE;
}
//..............................................................................................................
// Ʈ���� ���̾� 
//..............................................................................................................
int CEffectManager::MAKE_EF_TRIPLE_FIRE(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	
	if( pSrc == NULL || pDest == NULL)
		return FALSE ;
	
	CEffectBase	*pData = SAFE_NEW( CEFTripleFire );				// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );				// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );										// �޴����� �ֱ�
	
	return TRUE;
}







//...............................................................................................................
//	���ڰ� ( ���� )
//...............................................................................................................
int CEffectManager::MAKE_EF_FOOT_LIGHTNING(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// ����Ʈ ������ �ҽ��ʸ� ����Ʈ ���
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	g_Particle.SetEffSetCon( 0.0f , id , ESLT_CHARPOS , 2.1f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
	//	g_Particle.SetEffSetCon( 0.0f , id , 100 , 2.1f , pSrc->GetUnique() , pSrc->GetIndex() , D3DXVECTOR3( pSrc->GetPosTM()._41 , pSrc->GetPosTM()._42 , pSrc->GetPosTM()._43 ) );
	
	//................................................................................	
	// �ߵ� ����
	//................................................................................	
	if( pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	
	
	switch( id )
	{
	case EF_FOOT_LIGHTNING_0:
		g_Particle.SetEffSetCon( 0.0f , EF_FOOT_LIGHTNING_ATTACKED_0 , ESLT_CHARPOS , 
			2.1f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
		pExtraInfo->AfterEffect = EF_FOOT_LIGHTNING_ATTACKED_0;
		pExtraInfo->DestType = ESLT_CHARPOS;
		pExtraInfo->lifeTime = 2.1f;
		break;
		
	case EF_FOOT_LIGHTNING_1:
		g_Particle.SetEffSetCon( 0.0f , EF_FOOT_LIGHTNING_ATTACKED_1 , ESLT_CHARPOS , 
			2.1f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
		pExtraInfo->AfterEffect = EF_FOOT_LIGHTNING_ATTACKED_1;
		pExtraInfo->DestType = ESLT_CHARPOS;
		pExtraInfo->lifeTime = 2.1f;
		break;
		
	case EF_FOOT_LIGHTNING_2:
		g_Particle.SetEffSetCon( 0.0f , EF_FOOT_LIGHTNING_ATTACKED_2 , ESLT_CHARPOS , 
			2.1f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
		pExtraInfo->AfterEffect = EF_FOOT_LIGHTNING_ATTACKED_2;
		pExtraInfo->DestType = ESLT_CHARPOS;
		pExtraInfo->lifeTime = 2.1f;
		break;
	}
	
	if( pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillAttackedSound( pExtraInfo->SkillCode , pDest );
	
	EffectTargetHit( pSrc , pDest , pExtraInfo );
	
	return TRUE;
}

//...............................................................................................................
// �⺻ Ȱ ����
//...............................................................................................................
int CEffectManager::MAKE_EF_BOW_ATTACK0(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	CEffectBase	*pData = SAFE_NEW( CEFBowAttack );				// Ŭ���� ����
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );				// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );										// �޴����� �ֱ�
	
	return TRUE;
}

//...............................................................................................................
//	������ ���� ����Ʈ ȿ��
//...............................................................................................................
int CEffectManager::UseItemEffect(long Unique,DWORD ItemCode)
{
	
	
	
	//...........................................................................................................
	// �ߵ� ����Ʈ ���� �˻�
	//...........................................................................................................
	int AttackedEffectCode = g_Pc_Manager.theItemInfoTable[ g_Pc_Manager.theItemCodeTable[ ItemCode ] ].theAttackEff;
	
	short AttackSound = g_Pc_Manager.theItemInfoTable[ g_Pc_Manager.theItemCodeTable[ ItemCode ] ].theAttackSnd;
	
	//...........................................................................................................
	// ĳ���� ã��
	//...........................................................................................................
	Character	*pSrc     = NULL;
	BOOL		 ISPlayer = FALSE;
	
	if( g_Pc.GetPlayer()->GetUnique() == Unique )
	{
		pSrc = ( Character * ) g_Pc.GetPlayer();
		
		ISPlayer = TRUE;
	}
	else
	{
		pSrc = ( Character * ) g_Pc.GetUser( Unique );
		
		ISPlayer = FALSE;
	}
	
	//...........................................................................................................
	// ĳ���� ������
	//...........................................................................................................
	if( NULL == pSrc )
	{
		DebugPrint( "!\n == CEffectManager::UseItemEffect ( ����ڸ� ã���� ���� ) == \n" );
		
		return FALSE;
	}
	
	// ������ ���� �Ҹ�
	if( -1 != AttackSound )
	{
		if( ISPlayer )
			g_Sound.Play( AttackSound , NULL );
		else
		{
			D3DXVECTOR3 vPos( pSrc->GetPosTM()._41 , pSrc->GetPosTM()._42 , pSrc->GetPosTM()._43 );
			
			g_Sound.Play( AttackSound , &vPos );
		}
	}
	
	if( -1 == AttackedEffectCode )
	{
		return false;
	}
	
	//...........................................................................................................
	// ����Ʈ ����
	//...........................................................................................................
	EFFECT_EXTRA_INFO	TempExtraInfo;
	
	TempExtraInfo.SkillCode	= ItemCode;					// ������ �ڵ� �ֱ� ( T.T ) ���߿� ��ų�� ���� ���� ���ߵǼ���...
	
	CreateEffect( ( EFFECT_ID ) AttackedEffectCode , pSrc , NULL , &TempExtraInfo );
	
	return TRUE;
}

//............................................................................................................
// ���� ���� ������ 0 ���� ó���Һκ� ( T.T )
//............................................................................................................
int CEffectManager::AttackStartFrame(Character *pSrc)
{
	if( pSrc == NULL )
		return FALSE;
	
	if( nRui->thePcInven.WearItem[n_WearSlot_Weapon] == NULL )
		return FALSE;
	
	SItemBaseInfo* item_Info = NULL;
	
	item_Info = g_Pc_Manager.GetItemBaseInfoPtr( nRui->thePcInven.WearItem[n_WearSlot_Weapon]->Code );
	
	if( item_Info == NULL )
		return FALSE;
	
	switch( item_Info->theType )
	{
		// ������ 
	case 4:
		{	
			switch( pSrc->m_ani_index )
			{
			case n_Attack5_Basic0:
			case n_Attack5_Basic1:
			case n_Attack5_Basic3:
				g_Particle.SetEffSetCon( 0.0f , EF_RAIN_GATHER  , 0 , 0.5f , pSrc->GetUnique() , pSrc->GetIndex() );
				break;
			case n_Attack5_Basic2:
				g_Particle.SetEffSetCon( 0.0f , EF_RAIN_GATHER2 , 0 , 0.5f , pSrc->GetUnique() , pSrc->GetIndex() );
				break;
			case n_Attack5_Basic4:
				g_Particle.SetEffSetCon( 0.0f , EF_RAIN_GATHER3 , 0 , 0.5f , pSrc->GetUnique() , pSrc->GetIndex() );
				break;
			default:
				g_Particle.SetEffSetCon( 0.0f , EF_RAIN_GATHER  , 0 , 0.5f , pSrc->GetUnique() , pSrc->GetIndex() );
				break;
			}
		}
		break;
	}
	
	return TRUE;
}

// ���� ������ ���� �Ҹ�
int CEffectManager::MonsterAttackedSoundPlay(Character *pSrc,EDamageLevel level)
{
	if( NULL == pSrc )
	{
		return FALSE;
	}
	
	if( pSrc->m_nTableIndex == -1 )
	{
		return FALSE;
	}
	
	NPCTABLE* npc_table = g_Map.GetNpcTable( pSrc->GetUnique() );
	if( npc_table == NULL )
	{
		return false;
	}
	
	SNPCBaseInfo* npc_Info = NULL;				
	npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( npc_table->code ); 			
	if( npc_Info == NULL )
		return FALSE;
	
	int SoundNum = -1;
	
	switch( level )
	{
		// ������ ��
	case DL_WEAK:
		
		SoundNum = npc_Info->theAttackSnd1;
		
		break;
		
	case DL_STRONG:
		
		SoundNum = npc_Info->theAttackSnd2;
		
		break;
		
	case DL_CRITICAL:
		
		SoundNum = npc_Info->theAttackSnd3;
		
		break;
	}
	
	if( -1 != SoundNum )
		g_Sound.Play( SoundNum , pSrc->GetPosTM()._41 , pSrc->GetPosTM()._42 , pSrc->GetPosTM()._43 );
	
	return TRUE;
}

//..............................................................................................................
// �ΰ� ȿ��
//..............................................................................................................
int CEffectManager::MAKE_EF_ADDEFFECT_BASE(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// �ҽ��ʸ� ����Ʈ ���
	if( pSrc == NULL )
		return TRUE;
	
	std::list	<CEffectBase *>::iterator i = pSrc->m_EffectList.begin();
	
	for( ; i != pSrc->m_EffectList.end() ; ++i )
	{
		if( (*i)->GetID() == EF_ADDEFFECT_BASE )
		{
			((CEFAddEffectBase *)*i)->InsertAddSkill( id );
			
			return TRUE;
		}
	}
	
	CEffectBase	*pData = SAFE_NEW( CEFAddEffectBase );						// Ŭ���� ����
	
	pData->Setup( EF_ADDEFFECT_BASE , pSrc , NULL , NULL );					// �ҽ� , Ÿ�� ����
	
	((CEFAddEffectBase *)pData)->InsertAddSkill( id );
	
	PushEffect( &pData );													// �޴����� �ֱ�
	
	return TRUE;
}

//..............................................................................................................
// �÷��̾� �׾����� ����Ʈ �����
//..............................................................................................................
void CEffectManager::DeletePlayerEffect(Character *pSrc)
{
	if( pSrc == NULL )
		return;
	
	g_Particle.m_EffectManager.DeleteOnlySrc( pSrc );
	
	pSrc->m_SpellEffect.Clear();
}

//..............................................................................................................
// �ٸ� �÷��̾� �׾����� ����Ʈ �����
//..............................................................................................................
void CEffectManager::DeleteOtherPlayerEffect(Character *pSrc)
{
	if( pSrc == NULL )
		return;
	
	g_Particle.m_EffectManager.DeleteOnlySrc( pSrc );
	pSrc->m_SpellEffect.Clear();
}

//..............................................................................................................
// NPC �׾����� ����Ʈ �����
//..............................................................................................................
void CEffectManager::DeleteNPCEffect(Character *pSrc)
{
	g_Particle.m_EffectManager.DeleteOnlySrc( pSrc );
}


//..............................................................................................................
// ��ų ����Ʈ �����ӽ� ���� �Ҹ�
//..............................................................................................................
int	 CEffectManager::GetSkillImfactSound(DWORD aSkillCode,Character *pSrc)
{
	SSkillBaseInfo* skill_Info;
	
	skill_Info = g_Pc_Manager.GetSkillBaseInfoPtr( aSkillCode ); 
	
	if( skill_Info == NULL )
		return -1;
	
	if( NULL != pSrc )
		g_Sound.Play( skill_Info->theMagicShot , pSrc->GetPosTM()._41 , pSrc->GetPosTM()._42 , pSrc->GetPosTM()._43 );
	else
		g_Sound.Play( skill_Info->theMagicShot );
	
	return skill_Info->theMagicShot;
}

//..............................................................................................................
// ��ų ���۽� ���� ( �ϴ� ��â�� ���� ��û ������ CRockClientNet���� ó�� )
//..............................................................................................................
int	 CEffectManager::GetSkillStartSound(DWORD aSkillCode,Character *pSrc,BOOL ISCheck)
{
	SSkillBaseInfo* skill_Info;
	
	skill_Info = g_Pc_Manager.GetSkillBaseInfoPtr( aSkillCode ); 
	
	if( skill_Info == NULL )
		return -1;
	
	if( skill_Info->theMagicShotSount == -1 )	
		return -1;
	
	// ��â ��ƾ üũ
	if( ISCheck )
	{
		if( skill_Info->theNeedSpelling )
			return -1;
	}
	
	if( NULL != pSrc )
		g_Sound.Play( skill_Info->theMagicShotSount , pSrc->GetPosTM()._41 , pSrc->GetPosTM()._42 , pSrc->GetPosTM()._43 );
	else
		g_Sound.Play( skill_Info->theMagicShotSount );
	
	return skill_Info->theMagicShotSount;
}

//..............................................................................................................
// ��ų ���� �����ӽ� ���� �Ҹ�
//..............................................................................................................
int	 CEffectManager::GetSkillAttackedSound(DWORD aSkillCode,Character *pSrc)
{
	SSkillBaseInfo* skill_Info;
	
	skill_Info = g_Pc_Manager.GetSkillBaseInfoPtr( aSkillCode ); 
	
	if( skill_Info == NULL )
		return -1;
	
	if( NULL != pSrc )
		g_Sound.Play( skill_Info->theMagicHit , pSrc->GetPosTM()._41 , pSrc->GetPosTM()._42 , pSrc->GetPosTM()._43 );
	else
		g_Sound.Play( skill_Info->theMagicHit );
	
	return skill_Info->theMagicHit;
}

//..............................................................................................................
// ���� ����Ʈ
//..............................................................................................................
int	 CEffectManager::SetRestSkill(Character *pSrc)
{
	if( pSrc == NULL )
		return FALSE;
	
	int Index    = -1;
	int EffectID = -1;
	
	// �÷��̾� �϶�	
	if( pSrc == g_Pc.GetPlayer() )
	{
		Index = g_Pc.GetAttrIndex( g_Pc.m_MainPC.char_info.theCitizen , g_Pc.m_MainPC.char_info.theGender );
	}
	else
	{
		Index = g_Pc.GetAttrIndex( g_Pc.m_PcTable[ pSrc->m_nTableIndex ].char_info.theCitizen , g_Pc.m_PcTable[ pSrc->m_nTableIndex ].char_info.theGender );
	}
	
	switch( Index )
	{
		// �з���
	case 0:
	case 1:
		
		EffectID = EF_REST_BREATH;
		
		break;
		
	case 2:
	case 3:
		
		EffectID = EF_REST_RAIN;
		
		break;
	}
	
	if( EffectID != -1 )
	{
		g_Particle.SetEffSetCon( 0.0f , EffectID , 0 , EFFECT_SET_LOOP_TIME ,  pSrc->GetUnique() , pSrc->GetIndex() );
	}
	
	return TRUE;
}

//..............................................................................................................
// ���� ����Ʈ �����
//..............................................................................................................
int	 CEffectManager::DeleteRestSkill(Character *pSrc)
{
	if( pSrc == NULL )
		return FALSE;
	
	int Index    = -1;
	int EffectID = -1;
	
	// �÷��̾� �϶�	
	if( pSrc == g_Pc.GetPlayer() )
	{
		Index = g_Pc.GetAttrIndex( g_Pc.m_MainPC.char_info.theCitizen , g_Pc.m_MainPC.char_info.theGender );
	}
	else
	{
		Index = g_Pc.GetAttrIndex( g_Pc.m_PcTable[ pSrc->m_nTableIndex ].char_info.theCitizen , g_Pc.m_PcTable[ pSrc->m_nTableIndex ].char_info.theGender );
	}
	
	switch( Index )
	{
		// �з���
	case 0:
	case 1:
		
		EffectID = EF_REST_BREATH;
		
		break;
		
	case 2:
	case 3:
		
		EffectID = EF_REST_RAIN;
		
		break;
	}
	
	if( EffectID != -1 )
	{
		g_Particle.DelEffSetCon( EffectID , 0 , pSrc->GetUnique() , pSrc->GetIndex() );
	}
	
	
	return TRUE;
}


//.............................................................................................................
// ȭ�� ���̱�
//.............................................................................................................
int CEffectManager::CreateArrowStuck(Character *pSrc,float AddRad,D3DXVECTOR3 vAddPos,int LinkBone)
{
	CEffectBase	*pData = SAFE_NEW( CArrowStuck );							// Ŭ���� ����
	
	pData->Setup( ( EFFECT_ID ) -1 , pSrc , NULL , NULL );					// �ҽ� , Ÿ�� ����
	
	((CArrowStuck *)pData)->SetExtraInfo( AddRad , vAddPos , LinkBone );
	
	PushEffect( &pData );										// �޴����� �ֱ�
	
	return TRUE;
}

//.............................................................................................................
// ������ ĳ�� �˻�
//.............................................................................................................
int CEffectManager::CheckCharacter(Character *pSrc)
{
	if( pSrc == NULL )
		return FALSE;	
	
	if( pSrc->m_nTableIndex == -1 )
		return FALSE;
	
	if( pSrc->GetUnique() == -1 )
		return FALSE;
	
	return TRUE;
}

//.............................................................................................................
// ������ ĳ�� �˻�
//.............................................................................................................
int CEffectManager::CheckCharacter(Character *pSrc,Character *pDest)
{
	if( pSrc == NULL || pDest == NULL )
		return FALSE;	
	
	if( pSrc->m_nTableIndex == -1 || pDest->m_nTableIndex == -1 )
		return FALSE;
	
	if( pDest->GetUnique() == - 1 || pDest->GetUnique() == - 1 )
		return FALSE;
	
	return TRUE;
}

//.............................................................................................................
// �׾����� �˻� ( ������ ĳ���� �˻� )
//.............................................................................................................
int CEffectManager::CheckDie(Character *pSrc)
{
	if( !CheckCharacter( pSrc ) )
	{
		return TRUE;
	}
	
	/*	
	if( pSrc->m_curt_event.type == SM_STATE_DIE )
	{
	return TRUE;
	}
	*/
	
	return FALSE;
}

//.............................................................................................................
// ���Ӽ� ��ų�� �ִٸ� ����Ʈ �޾��ֱ�
//.............................................................................................................
void CEffectManager::CreateKeepupSkill(int skillcode,Character *pSrc,Character *pDest,BOOL ISExcept)
{
	SSkillBaseInfo* skill_Info;
				
	skill_Info = g_Pc_Manager.GetSkillBaseInfoPtr( skillcode ); 
				
	if( skill_Info != NULL )
	{
		// ���Ӽ� ��ų�̳�?
		if( skill_Info->theEffectFlag || ISExcept )
		{
			// �ߵ� ��ų ( ���� )
			// g_Particle.m_EffectManager.Create( ( EFFECT_ID ) skill_Info->theActionEffect , &g_Map.m_Npc[npc_tbl_index] , NULL );
			
			// ���Ӽ� ��ų
			if( skill_Info->theKeepupEffect != -1 )
			{
				EFFECT_EXTRA_INFO ExtraInfo;
				
				ExtraInfo.SkillCode = skillcode;
				
				g_Particle.m_EffectManager.CreateEffect( ( EFFECT_ID ) skill_Info->theKeepupEffect , pSrc , pDest, &ExtraInfo );
			}
		}
	}
}


//.............................................................................................................
// ���Ӽ� ��ų ����Ʈ �����
//.............................................................................................................
void CEffectManager::DeleteKeepupSkill(int skillcode,Character *pSrc)
{
	SSkillBaseInfo* skill_Info;
	skill_Info = g_Pc_Manager.GetSkillBaseInfoPtr( skillcode ); 
	
	// �ߵ� ��ų ����
	// g_Particle.m_EffectManager.Delete( ( EFFECT_ID ) skill_Info->theActionEffect , &g_Pc.m_Pc[ tableIndex ] , NULL );
	
	if( skill_Info != NULL )
	{
		if( skill_Info->theKeepupEffect != -1 )
		{
			///--JAPAN_BUG_MODIFY
			Delete( ( EFFECT_ID ) skill_Info->theKeepupEffect , pSrc , NULL, skillcode );	
		}
	}
}

//.............................................................................................................
// �÷��̾� ���Ӽ� ��ų ����Ʈ ��� �����
//.............................................................................................................
void CEffectManager::DeleteAllKeepupSkillOnPlayer(void)
{
	for( int i = 0; i < RP_MAX_KEEPUP_SKILL; ++i )
	{
		if( nRui->thePcInven.KeepupSkill[i] != 0 )
		{
			WORD aSkillCode = nRui->thePcInven.KeepupSkill[i];
			
			//.............................................................................................................
			// ���Ӽ� ��ų ����Ʈ �����
			//.............................................................................................................
			g_Particle.m_EffectManager.DeleteKeepupSkill( nRui->thePcInven.KeepupSkill[i] , g_Pc.GetPlayer() );
			g_Particle.m_EffectManager.DeleteAddEffectEnd( nRui->thePcInven.KeepupSkill[i] , g_Pc.GetPlayer() );
			
			PlayerInfoWnd.DeleteKeepUpSkillCode( i, aSkillCode );
			
			nRui->thePcInven.KeepupSkill[i] = 0;
		}
	}
}


//.............................................................................................................
// 1:N ����Ʈ ���� 
//.............................................................................................................
void CEffectManager::EffectCreateLock(void)
{
	m_EffectState = CREATE_EFFECT_LOCK_ONETIME;
	
	m_ExtraEffectQueue.SubExtraInfoList.clear();
}

//.............................................................................................................
// 1:N ����Ʈ �������� ȣ�� ( ����Ʈ �ߵ� )
//.............................................................................................................
void CEffectManager::EffectCreateUnLock(void)
{
	m_EffectState = CREATE_EFFECT_UNLOCK;
}

//.............................................................................................................
// ��ų �ڵ� �����ų�? ( ��� ������ ���� )
//.............................................................................................................
BOOL CEffectManager::ISGoodSkill(DWORD SkillCode)
{
	SSkillBaseInfo* skill_Info;
	
	skill_Info = g_Pc_Manager.GetSkillBaseInfoPtr( SkillCode ); 
	
	if( skill_Info == NULL )	
		return FALSE;
	
	if( skill_Info->theSkillType2 == n_GoodSupport || 
		skill_Info->theSkillType2 == n_Regain        )
	{
		return TRUE;
	}
	
	return FALSE;
}


//.............................................................................................................
// �ǽð� ����Ʈ ( ������ ) ó�� ( �÷��̾� , ������ ������ ���� ���� )
//.............................................................................................................
void CEffectManager::CreateKeepupSkillStart(DWORD SkillCode,WORD *pEffectType,DWORD *pEffectValue,WORD EffectMaxValue)
{
	_STargetInfo TInfo;
	
	TInfo.theTargetID   = g_Pc.GetPlayer()->GetUnique();
	TInfo.theTargetType = n_PC;
	TInfo.theDieFlag    = 0;
	TInfo.thePushedCell = 0;
	
	TInfo.theKeepupFlag[ 0 ] = TInfo.theKeepupFlag[ 1 ] = TInfo.theKeepupFlag[ 2 ] = 1;
	
	memcpy( TInfo.theEffectValue    , pEffectValue    , sizeof( DWORD ) * 3 );
	
	TInfo.theEffectMaxValue = EffectMaxValue;
	
	TInfo.theAddEfectFlag = FALSE;
	
	g_Pc.theSkillSystem.SetDamageValue( SkillCode, &TInfo );
	g_Pc.theSkillSystem.UseSkillTargetInfoMgr( NULL, TInfo , SkillCode, TRUE );
	
	//	if( n_Success == TInfo.theAddEfectFlag )
	//		g_Pc.theSkillSystem.UseSkillTargetInfoMgr_AddEffect( NULL, &TInfo, SkillCode, TRUE );
	
}


void CEffectManager::CreateAddEffectStart(DWORD SkillCode,Character *pSrc)
{
	if( pSrc == NULL )
		return;
	
	// ���� �̻� ����Ʈ ��ȣ ���
	SSkillBaseInfo* skill_Info;
	
	skill_Info = g_Pc_Manager.GetSkillBaseInfoPtr( SkillCode ); 
	
	if( skill_Info == NULL )
		return;
	
	
	if( skill_Info->theStatusEffect == -1 )
		return;
	
	Create( ( EFFECT_ID ) skill_Info->theStatusEffect , pSrc , NULL );
}

void CEffectManager::DeleteAddEffectEnd(DWORD SkillCode,Character *pSrc)
{
	if( pSrc == NULL )
		return;
	
	// ���� �̻� ����Ʈ ��ȣ ���
	SSkillBaseInfo* skill_Info;
	
	skill_Info = g_Pc_Manager.GetSkillBaseInfoPtr( SkillCode ); 
	
	if( skill_Info == NULL )
		return;
	
	
	if( skill_Info->theStatusEffect == -1 )
		return;
	
	std::list	<CEffectBase *>::iterator i = pSrc->m_EffectList.begin();
	
	for( ; i != pSrc->m_EffectList.end() ; ++i )
	{
		if( (*i)->GetID() == EF_ADDEFFECT_BASE )
		{
			((CEFAddEffectBase *)*i)->DeleteAddSkill( skill_Info->theStatusEffect );
			
			return;
		}
	}
}

//.............................................................................................................
// �ǽð� ����Ʈ ( ������ ) ó�� ( �ڱ�� ���� , ������ ������ ���� ���� )
//.............................................................................................................
void CEffectManager::CreateAuroraEffect(DWORD SkillCode,BYTE TargetNum,SObjectIDPtr TargetList , int FrontierHolyLevel /* = 0  */)
{
	BOOL ISFind = FALSE;
	
	for( int i = 0 ; i < ( int ) TargetNum ; ++i )
	{
		ISFind = FALSE;
		
		//.....................................................................................................
		// �����϶�
		//.....................................................................................................
		if( n_PC == TargetList[ i ].theType )
		{
			//.................................................................................................
			// �÷��̾��϶� ( ���� ) KeepUpStart ���� ó���Ѵ�
			//.................................................................................................
			if( TargetList[ i ].theID == g_Pc.GetPlayer()->GetUnique() )
			{
				continue;
			}
			//.................................................................................................
			// �ٸ� �����϶�
			//.................................................................................................
			else
			{
				int skilledIdx;
				
				skilledIdx = g_Pc.FindUser( TargetList[ i ].theID );
				
				if( skilledIdx == MAX_USER_INTABLE )
					return;
				
				if( g_Pc.m_Pc[skilledIdx].m_bIsLoaded == true )
				{
					for( int i = 0; i < RP_MAX_KEEPUP_SKILL; ++i )
					{
						if( g_Pc.m_PcTable[ g_Pc.m_Pc[ skilledIdx ].m_nTableIndex ].char_info.theKeepupSkill[ i ] == SkillCode )
						{
							ISFind = TRUE;
							
							break;
						}
					}
					
					if( FALSE == ISFind )
					{
						CreateKeepupSkill( SkillCode , &g_Pc.m_Pc[skilledIdx] , NULL , TRUE );
						
						
						// ���ӽ�ų ������ �ޱ�
						for( i = 0; i < RP_MAX_KEEPUP_SKILL; ++i )
						{
							if( g_Pc.m_PcTable[ g_Pc.m_Pc[ skilledIdx ].m_nTableIndex ].char_info.theKeepupSkill[ i ] == 0 )
							{
								g_Pc.m_PcTable[ g_Pc.m_Pc[ skilledIdx ].m_nTableIndex ].char_info.theKeepupSkill[ i ] = SkillCode;
#ifdef HOLY_SKILLICON_MODIFY
							
								if( g_RockClient.IsHolySkill(SkillCode))
								{
									g_Pc.m_PcTable[ g_Pc.m_Pc[ skilledIdx ].m_nTableIndex ].char_info.theHolyLevel = FrontierHolyLevel; 
								}
							
								
#endif
								

								break;
							}
						}
					}
				}
				
			}
		}
		//.....................................................................................................
		// �� �϶�
		//.....................................................................................................
		else if( n_NPC == TargetList[ i ].theType )
		{
			int skilledIdx;
			
			skilledIdx = g_Map.FindNpc( TargetList[ i ].theID );
			
			if( skilledIdx == MAX_NPC_INTABLE )
				return;
			
			if( g_Map.m_Npc[skilledIdx].m_bIsLoaded == true )
			{
				for( int i = 0; i < RP_MAX_KEEPUP_SKILL; ++i )
				{
					if( g_Map.m_NpcTable[ g_Map.m_Npc[skilledIdx].m_nTableIndex ].theKeepupSkill[i] == SkillCode )
					{
						ISFind = TRUE;
						
						break;
					}
				}
				
				if( ISFind == FALSE )
				{
					CreateKeepupSkill( SkillCode , &g_Map.m_Npc[skilledIdx] , NULL , TRUE );
					
					// ���ӽ�ų ������ �ޱ�
					for( int i = 0; i < RP_MAX_KEEPUP_SKILL; ++i )
					{
						if( g_Map.m_NpcTable[ g_Map.m_Npc[skilledIdx].m_nTableIndex ].theKeepupSkill[i] == 0 )
						{
							g_Map.m_NpcTable[ g_Map.m_Npc[skilledIdx].m_nTableIndex ].theKeepupSkill[i] = SkillCode;
#ifdef HOLY_SKILLICON_MODIFY
							if( g_RockClient.IsHolySkill(SkillCode))
								{
									g_Map.m_NpcTable[ g_Map.m_Npc[skilledIdx].m_nTableIndex ].theHolyLevel = FrontierHolyLevel; 
								}
								
				
#endif
							
							break;
						}
						
					}
				}
			}
		}
		
		
	}
}

void CEffectManager::SetLandMark(BOOL ISDisplay,float Size,float AttackRange)
{ 
	m_ISLandMarkDisplay = ISDisplay; 
	
	if( TRUE == m_ISLandMarkDisplay )
	{
		CBillboardData *pData = NULL ;
		
		if( NULL == ( pData = g_Billboard.m_BillboardData.GetKeyDataPointer( 1023 ) ) )
		{
			m_ISLandMarkDisplay = FALSE;
			
			return;
		}
		
		pData->m_vStartSize.x = Size;
		pData->m_vStartSize.z = Size;
		
		pData->m_vEndSize.x = Size;
		pData->m_vEndSize.z = Size;
		
		m_LandMark.MakeLink( pData );
		
		if( NULL == ( pData = g_Billboard.m_BillboardData.GetKeyDataPointer( 1024 ) ) )
		{
			m_ISLandMarkDisplay = FALSE;
			
			return;
		}
		
		m_LandMarkRange = AttackRange;
		
		pData->m_vStartSize.x = Size;
		pData->m_vStartSize.z = Size;
		
		pData->m_vEndSize.x = Size;
		pData->m_vEndSize.z = Size;
		
		m_DisableLandMark.MakeLink( pData );
		
		m_ISLandMarkDisplay = TRUE;
	}
	
}

//...................................................................................................................
// ���� ������ �ٴڿ� ����ִ� ����Ʈ
//...................................................................................................................
int CEffectManager::RenderToLandMark(float dtime)
{
	if( FALSE == m_ISLandMarkDisplay )
		return FALSE;
	
	static int OldMouseX = -1;
	static int OldMouseY = -1;
	static D3DXVECTOR3 vPos = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
	
	// ���콺 üũ�ؼ� ���� üũ�ϴ°�
	if( nRui->GetCursor()->pos.x != OldMouseX ||
		nRui->GetCursor()->pos.y != OldMouseY )
	{
		OldMouseX = nRui->GetCursor()->pos.x;
		OldMouseY = nRui->GetCursor()->pos.y;
		
		if( g_Map.PHF_PickDest( g_RockClient.GetApp() ) == S_OK )
		{
			vPos.x = g_Map.m_RealPickedX;
			vPos.z = g_Map.m_RealPickedY;
			vPos.y = 0.0f;
			
			D3DXVECTOR3 vDistance;		
			
			vDistance = D3DXVECTOR3( g_Pc.GetPlayer()->GetPosTM()._41 , 0.0f , g_Pc.GetPlayer()->GetPosTM()._43 );
			
			vDistance = vDistance - vPos;
			
			if( m_LandMarkRange < D3DXVec3Length( &vDistance ) )
				m_DisplayLandMarkType = 0;
			else
				m_DisplayLandMarkType = 1;
		}
		else
		{
			return FALSE;
		}
	}
	
	// ��� ��ƾ
	m_LandMark.m_vLoc        = vPos;
	m_DisableLandMark.m_vLoc = vPos;
	
	g_Billboard.DisplayManager( m_LandMark        , dtime );
	g_Billboard.DisplayManager( m_DisableLandMark , dtime );
	
	D3DXMATRIX	matIdentity;
	
	D3DXMatrixIdentity( &matIdentity );
	
	g_RenderManager.SetTransform( D3DTS_WORLD , &matIdentity );
	
	g_RenderManager.SetCullMode( D3DCULL_CCW );
	
	g_RenderManager.SetZWriteEnable( TRUE );
	g_RenderManager.SetZEnable( TRUE );
	g_RenderManager.SetAlphaBlendEnable(  FALSE );
	
	switch( m_DisplayLandMarkType )
	{
	case 0:
#ifdef BATCH_RENDER
		{
			m_DisableLandMark.DrawBatchBillboard();
#ifdef DIRECT_VERSION_9_MJH
			LPDIRECT3DTEXTURE9	prevBillboardTexture = g_RenderManager.GetPrevBillboardTexture();
#else
			LPDIRECT3DTEXTURE8	prevBillboardTexture = g_RenderManager.GetPrevBillboardTexture();
#endif // DIRECT_VERSION_9_MJH
			if(prevBillboardTexture)
			{
				g_RenderManager.SetTexture( 0, prevBillboardTexture );
				g_RenderManager.SetSrcBlend( g_RenderManager.m_PrevSrcBlend );
				g_RenderManager.SetDestBlend( g_RenderManager.m_PrevDestBlend );
				g_RenderManager.EndBatchPrimitive();
				g_RenderManager.SetBeginBatchRendering(FALSE);
			}
		}
		
#else	//	BATCH_RENDER
		m_DisableLandMark.DrawBillboard();
#endif	//	BATCH_RENDER
		
		break;
		
	case 1:
#ifdef BATCH_RENDER
		{
			m_LandMark.DrawBatchBillboard();
#ifdef DIRECT_VERSION_9_MJH
			LPDIRECT3DTEXTURE9	prevBillboardTexture = g_RenderManager.GetPrevBillboardTexture();
#else
			LPDIRECT3DTEXTURE8	prevBillboardTexture = g_RenderManager.GetPrevBillboardTexture();
#endif // DIRECT_VERSION_9_MJH
			if(prevBillboardTexture)
			{
				g_RenderManager.SetTexture( 0, prevBillboardTexture );
				g_RenderManager.SetSrcBlend( g_RenderManager.m_PrevSrcBlend );
				g_RenderManager.SetDestBlend( g_RenderManager.m_PrevDestBlend );
				g_RenderManager.EndBatchPrimitive();
				g_RenderManager.SetBeginBatchRendering(FALSE);
			}
		}
		
#else	//	BATCH_RENDER
		m_LandMark.DrawBillboard();
#endif	//	BATCH_RENDER
		
		break;
	}
	
	g_RenderManager.SetCullMode( D3DCULL_CCW );
	
	g_RenderManager.SetZWriteEnable( TRUE );
	g_RenderManager.SetZEnable( TRUE );
	g_RenderManager.SetAlphaBlendEnable(  FALSE );
	
	return TRUE;
}

//...................................................................................................................
// ���� ���� Appear �� Ÿ�� ������
//...................................................................................................................
int CEffectManager::AddLandEffect(WORD SkillCode,DWORD SkillUnique,DWORD CellX,DWORD CellY)
{
	DebugPrint( "-------------AddLandEffect %d----------------\n",SkillUnique);
	
	if( ISFindReadLandEffect( SkillUnique ) )
	{
		return FALSE;
	}
	
	std::list	<CEffectBase *> ::iterator	i = m_EffectList.begin();
	
	for( ; i != m_EffectList.end() ; )
	{
		if( (*i)->m_pExtraInfo != NULL )
		{
			//........................................................................................................
			// ����Ʈ ����� 
			//........................................................................................................
			if( (*i)->GetSkillUnique() == SkillUnique )
			{
				DebugPrint( "-------------Found AddLandEffect Return %d----------------\n",SkillUnique);
				
				return TRUE;
			}
			else
			{
				i++;
			}
		}
		else
		{
			i++;
		}
	}
	
	EFFECT_EXTRA_INFO TempInfo;
	
	TempInfo.SkillCode   = SkillCode;
	TempInfo.SkillUnique = SkillUnique;
	
	SSkillBaseInfo* skill_Info;
	
	skill_Info = g_Pc_Manager.GetSkillBaseInfoPtr( SkillCode ); 
	
	if( skill_Info == NULL )
		return FALSE;
	
	D3DXVECTOR3 vPos( 0.0f , 0.0f , 0.0f );
	
	vPos.x = CellX * (TILE_SIZE/2);
	vPos.y = g_Pc.GetMapHeight((int)CellX, (int)CellY );
	vPos.z = CellY * (TILE_SIZE/2);
	
	g_Particle.m_EffectManager.SetCell( CellX , CellY , vPos );
	
	g_Particle.m_EffectManager.CreateEffect( ( EFFECT_ID ) skill_Info->theKeepupEffect , NULL , NULL , &TempInfo );
	
	DebugPrint( "-------------Insert AddLandEffect %d----------------\n",SkillUnique);
	
	return TRUE;
}

float CEffectManager::GetAttackRange(int SkillCode)
{
	SSkillBaseInfo* skill_Info;
	
	skill_Info = g_Pc_Manager.GetSkillBaseInfoPtr( SkillCode ); 
	
	if( skill_Info == NULL )
	{
		return 0.0f;
	}
	else if( 100 == skill_Info->theSphere1 )
	{
		return 0.0f;
	}
	else if( 100 < skill_Info->theSphere1 && skill_Info->theSphere1 < 200 )
	{	
		return ( ( ( skill_Info->theSphere1 - 100 ) / 10 ) * 8.0f );
	}
	else
	{
		return ( skill_Info->theSphere1 * 8.0f );
	}
	
	return 0.0f;
}

//...................................................................................................................
// ���� ���� ������ 
//...................................................................................................................
int CEffectManager::DamageLandEffect(Character *pDest,int DamageValue,int SkillCode,BYTE ISDie)
{
	if( pDest == NULL )
		return FALSE;
	
	SSkillBaseInfo* skill_Info;
	
	skill_Info = g_Pc_Manager.GetSkillBaseInfoPtr( SkillCode ); 
	
	if( skill_Info == NULL )
		return FALSE;
	
	BOOL bGoodSkill = ISGoodSkill( SkillCode );
	BOOL ISDisplayDamageNum = FALSE;
	
	if( skill_Info->theSkillType2 == n_DeathBlow || 
		skill_Info->theSkillType2 == n_AttackMagic || 
		skill_Info->theSkillType2 == n_NoFrameDeathBlow )
	{
		ISDisplayDamageNum = TRUE;
	}
	
	// �������� 0�϶� �׾������� ����Ʈ ���̱�
	if( DamageValue <= 0 && ISDisplayDamageNum ) 
	{
		switch( SkillCode )
		{
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
			break;
			
		default:
			// ������ ������ �÷��̾�� �����ִ°͸� ���
			//if( pSrc == g_Pc.GetPlayer() || pDest == g_Pc.GetPlayer() )
			g_Particle.SetEffSetCon( 0.0f , EF_ATTACK_MISS , 0 , 0.3f , pDest->GetUnique() , pDest->GetIndex() );
			
			break;
		}
		// return;
	}
	
	// ������ ���� ���
	D3DXVECTOR3 vHeight = D3DXVECTOR3(0.0f,0.0f,0.0f);		
	
	if( g_Map.IsGiantUnit( pDest ) )
	{
		vHeight = D3DXVECTOR3( pDest->GetBonCenterTM()._41 ,
			pDest->GetBonCenterTM()._42 + 38.0f,//(pDest->GetBoundMaxPos().y - pDest->GetBoundMinVec().y) * 0.3f, 
			pDest->GetBonCenterTM()._43 );
	}
	else
	{
		
		vHeight = D3DXVECTOR3( pDest->GetPosTM()._41 ,
			pDest->GetPosTM()._42 + ( pDest->GetBoundMaxPos().y * 1.2f ) ,
			pDest->GetPosTM()._43 );
	}	
	
	g_Particle.m_DamageNum.Add( vHeight  , DamageValue , 0, pDest );
	
	
	// dongs Test ����Ʈ �� .. �ּ� ó�� .. 
	g_Particle.m_EffectManager.CreateEffect( ( EFFECT_ID ) skill_Info->theActionEffect , pDest , NULL , NULL );
	
	if( ISDie == TRUE )
	{
		int skilledIdx = 0;
		EVENT_DATA_INFO underattack_event;	
		
		underattack_event.type = SM_STATE_DIE;
		
		//......................................................................................................
		// ����
		//......................................................................................................
		if( pDest->m_bPC )
		{
#ifdef ROCKCLIENT_DEV					
//			SystemPrint( n_DarkRed, n_SystemMsg, "���� �׿��� �޷դ�����~~~ SM_STATE_DIE" );				
#endif
			underattack_event.motion = n_Die9;
			
			g_Particle.DelEffSetCon( EF_REST_BREATH , 0 , g_Pc.GetPlayer()->GetUnique() , g_Pc.GetPlayer()->GetIndex() );
			
			// �÷��̾� �׾����� ����Ʈ �����
			DeletePlayerEffect( pDest );
			
			//...............................................................................................
			// ����
			//...............................................................................................
			if( pDest->GetUnique() == g_Pc.GetPlayer()->GetUnique() )
			{
				g_Pc.GetPlayer()->PushQueue( underattack_event, TRUE );
			}
			//...............................................................................................
			// ����
			//...............................................................................................
			else
			{
				skilledIdx = g_Pc.FindUser( pDest->GetUnique() );
				
				if( skilledIdx == MAX_USER_INTABLE )
					return FALSE;
				
				g_Pc.m_Pc[skilledIdx].PushQueue( underattack_event, TRUE );
			}
			
		}
		//......................................................................................................
		// NPC
		//......................................................................................................
		else
		{
			if( pDest->m_State.nCharacterType == nCharacterTypePlayerNPC )
			{
				underattack_event.motion = n_Die9;
			}
			else
			{
				underattack_event.motion = n_NPC_Die0 + rand()%2;
			}
			
			skilledIdx = g_Map.FindNpc( pDest->GetUnique() );
			
			if( skilledIdx == MAX_NPC_INTABLE )
				return FALSE;
			
			// ������ ������ �ִ� ����Ʈ �����
			DeleteNPCEffect( pDest );
			
			g_Map.m_Npc[skilledIdx].PushQueue(underattack_event, TRUE);	
			
			
			SNPCBaseInfo* npc_Info;
			
			npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( ( g_Map.GetNpcTable( pDest->GetUnique() ))->code ); 
			
			if( -1 != npc_Info->theDieSnd )
				g_Sound.Play( npc_Info->theDieSnd , NULL );
			
		}
	}
	
	return TRUE;
}


int CEffectManager::DamageSkillEffect( Character *pDest, int DamageValue, int SkillCode, BYTE ISDie )
{
	if( pDest == NULL )
		return FALSE;
	
	SSkillBaseInfo* skill_Info;
	
	skill_Info = g_Pc_Manager.GetSkillBaseInfoPtr( SkillCode ); 
	
	if( skill_Info == NULL )
		return FALSE;
	
	BOOL bGoodSkill = ISGoodSkill( SkillCode );
	BOOL ISDisplayDamageNum = FALSE;
	
	if( skill_Info->theSkillType2 == n_DeathBlow || 
		skill_Info->theSkillType2 == n_AttackMagic || 
		skill_Info->theSkillType2 == n_NoFrameDeathBlow )
	{
		ISDisplayDamageNum = TRUE;
	}
	
	// �������� 0�϶� �׾������� ����Ʈ ���̱�
	if( DamageValue <= 0 && ISDisplayDamageNum ) 
	{
		switch( SkillCode )
		{
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
			break;
			
		default:
			// ������ ������ �÷��̾�� �����ִ°͸� ���
			//if( pSrc == g_Pc.GetPlayer() || pDest == g_Pc.GetPlayer() )
			g_Particle.SetEffSetCon( 0.0f , EF_ATTACK_MISS , 0 , 0.3f , pDest->GetUnique() , pDest->GetIndex() );
			
			break;
		}
		// return;
	}
	
	
	// ������ ���� ���
	D3DXVECTOR3 vHeight = D3DXVECTOR3(0.0f,0.0f,0.0f);		
	
	if( g_Map.IsGiantUnit( pDest ) )
	{
		vHeight = D3DXVECTOR3( pDest->GetBonCenterTM()._41 ,
			pDest->GetBonCenterTM()._42 + 38.0f,//(pDest->GetBoundMaxPos().y - pDest->GetBoundMinVec().y) * 0.3f, 
			pDest->GetBonCenterTM()._43 );
	}
	else
	{
		
		vHeight = D3DXVECTOR3( pDest->GetPosTM()._41 ,
			pDest->GetPosTM()._42 + ( pDest->GetBoundMaxPos().y * 1.2f ) ,
			pDest->GetPosTM()._43 );
	}	
	
	g_Particle.m_DamageNum.Add( vHeight  , DamageValue , 0, pDest );
	
	if( ISDie == TRUE )
	{
		int skilledIdx = 0;
		EVENT_DATA_INFO underattack_event;	
		
		underattack_event.type = SM_STATE_DIE;
		
		//......................................................................................................
		// ����
		//......................................................................................................
		if( pDest->m_bPC )
		{
			underattack_event.motion = n_Die9;
			
			g_Particle.DelEffSetCon( EF_REST_BREATH , 0 , g_Pc.GetPlayer()->GetUnique() , g_Pc.GetPlayer()->GetIndex() );
			
			// �÷��̾� �׾����� ����Ʈ �����
			DeletePlayerEffect( pDest );
			
			//...............................................................................................
			// ����
			//...............................................................................................
			if( pDest->GetUnique() == g_Pc.GetPlayer()->GetUnique() )
			{
				g_Pc.GetPlayer()->PushQueue( underattack_event, TRUE );
			}
			//...............................................................................................
			// ����
			//...............................................................................................
			else
			{
				skilledIdx = g_Pc.FindUser( pDest->GetUnique() );
				
				if( skilledIdx == MAX_USER_INTABLE )
					return FALSE;
				
				g_Pc.m_Pc[skilledIdx].PushQueue( underattack_event, TRUE );
			}
			
		}
		//......................................................................................................
		// NPC
		//......................................................................................................
		else
		{
			if( pDest->m_State.nCharacterType == nCharacterTypePlayerNPC )
			{
				underattack_event.motion = n_Die9;
			}
			else
			{
				underattack_event.motion = n_NPC_Die0 + rand()%2;
			}
			
			skilledIdx = g_Map.FindNpc( pDest->GetUnique() );
			
			if( skilledIdx == MAX_NPC_INTABLE )
				return FALSE;
			
			// ������ ������ �ִ� ����Ʈ �����
			DeleteNPCEffect( pDest );
			
			g_Map.m_Npc[skilledIdx].PushQueue(underattack_event, TRUE);	
			
			
			SNPCBaseInfo* npc_Info;
			
			npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( ( g_Map.GetNpcTable( pDest->GetUnique() ))->code ); 
			
			if( -1 != npc_Info->theDieSnd )
				g_Sound.Play( npc_Info->theDieSnd , NULL );
			
		}
	}
	
	return TRUE;
}


//...................................................................................................................
// ���ο� ��Ƽ �ɹ��� ���� / ���� �����̸� ó��
//...................................................................................................................
int CEffectManager::CheckPartiHide(UINT dPCID)
{
	DWORD HideSkillCodeList[] = {
		471 ,
			472 ,
			473 ,
			474 ,
			475 ,
			481 ,
			482 ,
			483 ,
			484 ,
			485
	};
	
	
	int skilledIdx = g_Pc.FindUser( dPCID );
	
	if( skilledIdx == MAX_USER_INTABLE )
		return FALSE;
	
	for( int j = 0; j < RP_MAX_KEEPUP_SKILL; ++j )
	{
		for( int i = 0 ; i < 10 ; ++i )
		{
			if( g_Pc.m_PcTable[skilledIdx].char_info.theKeepupSkill[j] == HideSkillCodeList[ i ] )
			{
				g_Pc.m_Pc[skilledIdx].m_CharAlpha.Set( CHAR_HIDE , 0.3f );
				
				if( g_Pc.m_Pc[skilledIdx].m_bPC && 
					g_Pc.m_Pc[skilledIdx].m_State.nPetState == nPetState_Master &&
					g_Pc.m_Pc[skilledIdx].m_State.PetIndex != -1 )
				{
					int PetIndex =  g_Pc.m_Pc[skilledIdx].m_State.PetIndex;
					g_Map.m_Npc[PetIndex].m_CharAlpha.Set( CHAR_HIDE , 0.3f );
				}

			}
		}
	}	
	
	return TRUE;
}

//...................................................................................................................
// ���� ��Ƽ �ɹ��� ���� / ���� �����̸� ó��
//...................................................................................................................
int CEffectManager::CheckPartiDeleteHide( UINT dPCID )
{
	DWORD HideSkillCodeList[] = {
		471 ,
			472 ,
			473 ,
			474 ,
			475 ,
			481 ,
			482 ,
			483 ,
			484 ,
			485
	};
	
	
	int skilledIdx = g_Pc.FindUser( dPCID );
	
	if( skilledIdx == MAX_USER_INTABLE )
		return FALSE;
	
	for( int j = 0; j < RP_MAX_KEEPUP_SKILL; ++j )
	{
		for( int i = 0 ; i < 10 ; ++i )
		{
			if( g_Pc.m_PcTable[skilledIdx].char_info.theKeepupSkill[j] == HideSkillCodeList[ i ] )
			{
				g_Pc.m_Pc[skilledIdx].m_CharAlpha.Set( CHAR_NONE_DISPLAY , 0.0f );
				
				if( g_Pc.m_Pc[skilledIdx].m_bPC && 
					g_Pc.m_Pc[skilledIdx].m_State.nPetState == nPetState_Master &&
					g_Pc.m_Pc[skilledIdx].m_State.PetIndex != -1 )
				{
					int PetIndex =  g_Pc.m_Pc[skilledIdx].m_State.PetIndex;
					g_Map.m_Npc[PetIndex].m_CharAlpha.Set( CHAR_NONE_DISPLAY , 0.0f );
				}
			}
		}
	}
	return TRUE;
}

//...................................................................................................................
// �� ��þƮ�� ����Ʈ �ޱ�
//...............................................................................................................
int CEffectManager::CreateDefenceEnChantEffect(Character *pChar, BYTE Level)
{
	
#ifndef DEFENSE_EFFECT
	return FALSE; 
#endif 
	
	
	// �÷��̾� �ƴϸ� �ȵ�
	if( !pChar->m_bPC )
		return FALSE;
	
	if( !CheckCharacter( pChar ) )
		return FALSE;
	
	DeleteDefenceEnChantEffect( pChar );
	
	int EldaStoneCode = 0;
	int EldaColor = 0 ; 
	
	
	if( Level == 255 )
	{
		int  position  = n_WearSlot_CoatArmor;
		
		if( pChar->GetUnique() == g_Pc.GetPlayer()->GetUnique() )
		{
			if( g_RockClient.m_PcInven->WearItem[position] != NULL )
			{
				EldaStoneCode = g_RockClient.m_PcInven->WearItem[position]->WearTable.theEldaStone;
				
				//����� �ΰ� ���� , ���� , ��� , ���� , ���� , ���� , ��� 
				EldaColor = GetDefenceEldaStoneInfo(EldaStoneCode);
				
				if(EldaColor != -1 )
				{
					//����
					int GetLevel = GetDefenceEldaStoneLevel(EldaStoneCode);
					
					if( GetLevel > 4 && GetLevel <= 10 )
					{
						Level =  GetLevel ;				
					}
					else 
					{
						return FALSE;
					}
				}
				else 
				{
					return FALSE; 
				}
			}
			else
			{
				return FALSE;
			}
		}
		else
		{
			// ���� ������ �ٸ������� ����Ʈ �ޱ� 
			if( g_Pc.m_PcTable[pChar->m_nTableIndex].char_info.theWearItem[position].theCode != 0 )
			{
				EldaStoneCode = g_Pc.m_PcTable[pChar->m_nTableIndex].char_info.theWearItem[position].theEldaStone;
				
				//����� �ΰ� ���� , ���� , ��� , ���� , ���� , ���� 
				EldaColor = GetDefenceEldaStoneInfo(EldaStoneCode);
				
				if(EldaColor != -1 )
				{
					//����
					int GetLevel = GetDefenceEldaStoneLevel(EldaStoneCode);
					
					if( GetLevel > 4 && GetLevel <= 10 )
					{
						Level =  GetLevel ;				
					}
					else 
					{
						return FALSE;
					}
				}
				else 
				{
					return FALSE; 
				}
			}
			else
			{
				return FALSE;
			}
			
		}
		
	}
	
	CEffectBase *pData = NULL ; 
	int				ID = 0 ; 
	
	pData = SAFE_NEW( CEFFameItemSet );
	ID	= EF_DEFENSED_ENCHANT;
	
	((CEFFameItemSet*)pData)->m_Level = Level ; 
	((CEFFameItemSet*)pData)->m_EldaType = EldaColor;
	
	
	if( pData == NULL )
		return FALSE;
	
	pData->Setup( (EFFECT_ID)ID , pChar , NULL , NULL );							// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );												// �޴����� �ֱ�
	
	return TRUE;
}

int CEffectManager::GetDefenceEldaStoneInfo(WORD Code)
{
	SItemBaseInfo *pItemBaseInfo = g_Pc_Manager.GetItemBaseInfoPtr( Code );
	
	if( NULL == pItemBaseInfo )
	{
		return -1;
	}
	
	if( n_Class_Eldastone != pItemBaseInfo->theClass )
	{
		return -1;
	}
	
	if(pItemBaseInfo->theMagicRes)
	{
		//���ֻ�
		return 1;
	}
	
	if(pItemBaseInfo->theAttackDef)
	{
		//���λ�	
		return 2;
	}
	
	if(pItemBaseInfo->theVital)
	{
		//���
		return 3;
	}
	
	if(pItemBaseInfo->theInt)
	{
		//�����
		return 4;
	}
	
	if(pItemBaseInfo->theStamina)
	{
		//����
		return 5;
	}
	
	if(pItemBaseInfo->theDex)
	{
		//����
		return 6;
	}
	
	if(pItemBaseInfo->theSympathy)
	{
		//�����
		return 7;
	}
	
	
	return -1;
	
}

int CEffectManager::GetDefenceEldaStoneLevel(WORD Code)
{
	SItemBaseInfo *pItemBaseInfo = g_Pc_Manager.GetItemBaseInfoPtr( Code );
	
	if( NULL == pItemBaseInfo )
	{
		return -1;
	}
	
	if( n_Class_Eldastone != pItemBaseInfo->theClass )
	{
		return -1;
	}
	
	if(pItemBaseInfo->thePower)
	{
		//��Ȳ��
		return -1;
	}
	
	if(pItemBaseInfo->theMinMagicBonus && pItemBaseInfo->theMaxMagicBonus )	
	{
		//Ǫ���� 
		return -1;
		
	}
	
	if(pItemBaseInfo->theMinAttackBonus && pItemBaseInfo->theMaxAttackBonus )
	{
		//������ 
		return -1; 
		
	}
	
	
	int iLevel = pItemBaseInfo->theGrade/100;
	
	return iLevel;	
	
}

//...................................................................................................................
// ��þƮ�� ����Ʈ �ޱ�
//...................................................................................................................
int CEffectManager::CreateWeaponEnChantEffect(Character *pChar,BYTE Level)
{
	// �÷��̾� �ƴϸ� �ȵ�
	if( !pChar->m_bPC )
		return FALSE;
	
	if( !CheckCharacter( pChar ) )
		return FALSE;
	
	DeleteWeaponEnChantEffect( pChar );
	
	int WeaponType = -1;
	
	int RHand_index = 0;
	int LHand_index = 0;
	
	//..............................................................................................................
	// ���� ���� üũ
	//..............................................................................................................
	if( pChar->GetUnique() == g_Pc.GetPlayer()->GetUnique() )
	{
		RHand_index = g_Pc.GetPlayer()->m_pUserInfo->ArmorInfo.index[0];
		LHand_index = g_Pc.GetPlayer()->m_pUserInfo->ArmorInfo.index[1];
		
	}
	else
	{
		RHand_index = g_Pc.m_Pc[ pChar->m_nTableIndex ].m_pUserInfo->ArmorInfo.index[0];
		LHand_index = g_Pc.m_Pc[ pChar->m_nTableIndex ].m_pUserInfo->ArmorInfo.index[1];
	}
	
	
	if( RHand_index != -1 && LHand_index == -1 )				// ������ ���⸸ ����
	{		
		if( g_Pc_Manager.m_pItm[RHand_index]->m_type == 0 )		// ��� 
		{
			WeaponType = 0;
		}
		else if( g_Pc_Manager.m_pItm[RHand_index]->m_type == 2 ) // �Ѽհ� 
		{
			WeaponType = 2;
		}
		else if( g_Pc_Manager.m_pItm[RHand_index]->m_type == 6 ) // ������ 
		{
			WeaponType = 6;
		}
		else if( g_Pc_Manager.m_pItm[RHand_index]->m_type == 7 ) // �����
		{
			WeaponType = 7;
		}
	}
	else if( RHand_index == -1 && LHand_index != -1 )			// �޼� ���⸸ ����
	{
		if( g_Pc_Manager.m_pItm[LHand_index]->m_type == 1 )		// Ȱ 
		{
			WeaponType = 1;
		}
		else if( g_Pc_Manager.m_pItm[LHand_index]->m_type == 5 ) // ������ 
		{
			WeaponType = 5;
		}
#ifdef ELDA_WEAPON_EFFECT
		else if( g_Pc_Manager.m_pItm[LHand_index]->m_type == 3 ) // ����
		{
			WeaponType = 2;
		}
#endif 
	}
	else if( RHand_index != -1 && LHand_index != -1 )			// ��տ� ���� ����
	{
		if( g_Pc_Manager.m_pItm[RHand_index]->m_type == 2 && 
			g_Pc_Manager.m_pItm[LHand_index]->m_type == 3 )		// �Ѽհ� + ����
		{
#ifdef ELDA_WEAPON_EFFECT
			//����Ʈ �̰� Į������ �ִ��� ? ? 
			if(IsFindEffectID(pChar , EF_BASE_ITEM_EFFECT_MERCENARY ))
			{
				WeaponType = 2;
			}
			else 
#endif 
			{
				WeaponType = 2;
			}
		}
	}
	else														// ���� ���� (�Ǽ� ����)
	{
		WeaponType = -1;
		return FALSE;
	}

	if( WeaponType == 5 ) 
	{
		if( !GetMagicBookLevelEffect(pChar , Level ))
		{
			return FALSE;
		}
	}
	
	int GetLevel = 0 ; 
	bool bIsMixEldaStone =false; 

	if( Level == 255 )
	{
		int  position  = n_WearSlot_Weapon;
		WORD EldaStone = 0;
		
		if( WeaponType == 1 ||	//Ȱ 
			WeaponType == 5 ||	// ������ 
			WeaponType == 3 )	// ���� 
		{
			position = n_WearSlot_Shield;
			
			//�������� ���� ����Ʈ�� �ٸ��� ������ ..  �ϵ��ڵ� ... 	
		}
		
		if( pChar->GetUnique() == g_Pc.GetPlayer()->GetUnique() )
		{
			if( g_RockClient.m_PcInven->WearItem[position] != NULL )
			{
				EldaStone = g_RockClient.m_PcInven->WearItem[position]->WearTable.theEldaStone;
				 
#ifdef ELDA_WEAPON_EFFECT
				if(EldaStone > MIXELDA_ITEMCODE_MIN )
				{
					bIsMixEldaStone = true; 
					GetLevel = GetWeaponMixEldaStoneLevel(EldaStone);
					
					if( GetLevel > 0 && GetLevel <= 20 )
					{
						Level = (GetLevel-1) /2 ; 
						if( Level >= 6 )
						{
							Level = 6 ; 
						}
					}
					else 
						return FALSE; 

				}
				else 
#endif 
				{
					int GetLevel = GetWeaponEldaStoneLevel(EldaStone);
					
					if( GetLevel > 0 && GetLevel <= 20 )
					{
						Level = ( GetLevel - 1 ) / 2;	
						if( Level > 4 )
							Level = 4 ; 
						
					}
					else
						return FALSE;
				}	
			}
			else
			{
				return FALSE;
			}
		}
		else
		{
			// ���� ������ �ٸ������� ����Ʈ �ޱ� 
			
			if( g_Pc.m_PcTable[pChar->m_nTableIndex].char_info.theWearItem[position].theCode  != 0 )
			{
				EldaStone = g_Pc.m_PcTable[pChar->m_nTableIndex].char_info.theWearItem[position].theEldaStone;
				
		
#ifdef ELDA_WEAPON_EFFECT
				if(EldaStone > MIXELDA_ITEMCODE_MIN )
				{
					bIsMixEldaStone = true; 
					GetLevel = GetWeaponMixEldaStoneLevel(EldaStone);
				
					if( GetLevel > 0 && GetLevel <= 20 )
					{
						Level = (GetLevel-1) /2 ; 
						if( Level >= 5 )
						{
							Level = 5 ; 
						}
					}
					else 
						return FALSE; 
				}
				else 
#endif 
				{
					int GetLevel = GetWeaponEldaStoneLevel(EldaStone);
					
					if( GetLevel > 0 && GetLevel <= 20 )
					{
						Level = ( GetLevel - 1 ) / 2;	
						if( Level > 4 )
							Level = 4 ; 
					}
					else
						return FALSE;
				}
			}
			else
			{
				return FALSE;
			}
		}
		
	}
	
	
	
	CEffectBase *pData = NULL;								// Ŭ���� ����
	int			 ID    = 0;
#ifdef ELDA_WEAPON_EFFECT
	if( bIsMixEldaStone )
	{
		pData = SAFE_NEW( CEFEldaWeaponItem );		
		
		switch( WeaponType )
		{
			// ���
		case 0:	ID    = EF_BASE_ITEM_EFFECT_SLAYER;	break;
			// �Ѽհ� 
		case 2:	ID    = EF_BASE_ITEM_EFFECT_MERCENARY;break;
			//���� 
		case 3: ID	  = EF_BASE_ITEM_EFFECT_KNIGHT;	break;
			// ������
		case 6:	ID    = EF_BASE_ITEM_EFFECT_HOLY;	break;
			// ����
		case 7:ID    =	EF_BASE_ITEM_EFFECT_PSYCHE;	break;
			// Ȱ
		case 1: ID    = EF_BASE_ITEM_EFFECT_SNIPER;	break;
			// ������
		case 5: ID    = EF_BASE_ITEM_EFFECT_PROMINAS;break;
			// �Ǽ�
		case -1: 	return FALSE;
			
		}

		((CEFEldaWeaponItem*)pData)->m_Level      = Level;
		((CEFEldaWeaponItem*)pData)->m_WeaponType = WeaponType;
	}
	else 
#endif 
	{
		
		switch( WeaponType )
		{
			//...................................................................................................
			// ���
			//...................................................................................................
		case 0:
			//CEFSwordUp 
			pData = SAFE_NEW( CEFSwordUp );		
			ID    = EF_SWORD_UP;
			
			((CEFSwordUp*)pData)->m_Level      = Level;
			((CEFSwordUp*)pData)->m_WeaponType = WeaponType;
			
			break;
			
			//...................................................................................................
			// �Ѽհ� 
			//...................................................................................................
		case 2:
			
			//CEFSwordUp 
			pData = SAFE_NEW(CEFSwordUp);	
			
			ID    = EF_SWORD_UP;
			
			((CEFSwordUp*)pData)->m_Level      = Level;
			((CEFSwordUp*)pData)->m_WeaponType = WeaponType;
			
			break;
			
			//...................................................................................................
			// ������
			//...................................................................................................
		case 6:
			
			pData = SAFE_NEW( CEFStickUp );		
			ID    = EF_STICK_UP;
			
			((CEFStickUp*)pData)->m_Level      = Level;
			((CEFStickUp*)pData)->m_WeaponType = WeaponType;
			
			break;
			
			//...................................................................................................
			// ����
			//...................................................................................................
		case 7:
			
			pData = SAFE_NEW( CEFBallUp );		
			ID    = EF_BALL_UP;
			((CEFBallUp*)pData)->m_Level      = Level;
			((CEFBallUp*)pData)->m_WeaponType = WeaponType;
			
			break;
			
			//...................................................................................................
			// Ȱ
			//...................................................................................................
		case 1:
			
			ID    = EF_BOW_UP;
			pData = SAFE_NEW( CEFBowUp );	
			
			((CEFBowUp*)pData)->m_Level      = Level;
			((CEFBowUp*)pData)->m_WeaponType = WeaponType;
			
			break;
			
			//...................................................................................................
			// ������
			//...................................................................................................
		case 5:
			
			ID    = EF_BOOK_UP;
			pData = SAFE_NEW( CEFBookUp );	
			
			((CEFBookUp*)pData)->m_Level      = Level;
			((CEFBookUp*)pData)->m_WeaponType = WeaponType;
			
			break;
			
			//...................................................................................................
			// �Ǽ�
			//...................................................................................................
		case -1:
			return FALSE;
			
		}
	}
	
	if( pData == NULL )
		return FALSE;
	
	pData->Setup( (EFFECT_ID)ID , pChar , NULL , NULL );							// �ҽ� , Ÿ�� ����
	
	PushEffect( &pData );												// �޴����� �ֱ�
	
	
	return TRUE;
}

int CEffectManager::GetMagicBookLevelEffect(Character *pChar,BYTE &Level )
{	

	int Code = 0 ;
	
	if( pChar->GetUnique() == g_Pc.GetPlayer()->GetUnique() )
	{
		if( g_RockClient.m_PcInven->WearItem[n_WearSlot_Shield] != NULL ) 
		{
			Code  = g_RockClient.m_PcInven->WearItem[n_WearSlot_Shield]->Code ;
		}
		
	}
	else 
	{
		Code = g_Pc.m_PcTable[pChar->m_nTableIndex].char_info.theWearItem[n_WearSlot_Shield].theCode ;
		
	}
#ifdef PAYITEM_EFFECT_TEST
#else
	switch(Code)
	{
	case 0: return FALSE;
	case 16601:
	case 16602:
	case 16603:
	case 16604:
	case 16605:
		
		Level = 5 ; 
		break; 
		
	case 16606:
	case 16607:
	case 16608:
	case 16609:
	case 16610:
		
		Level = 6 ; 	
		break; 
	default:break; 
		
	}
#endif //PAYITEM_EFFECT_TEST
	

	return TRUE;
}


int CEffectManager::GetWeaponMixEldaStoneLevel(WORD Code)
{
	SItemBaseInfo *pItemBaseInfo = g_Pc_Manager.GetItemBaseInfoPtr( Code );
	
	if( NULL == pItemBaseInfo )
	{
		return -1;
	}
	
	if( n_Class_Eldastone != pItemBaseInfo->theClass )
	{
		return -1;
	}
	
	int iLevel = pItemBaseInfo->theGrade/100;
	
	return iLevel;	
}
//...................................................................................................................
// ���ٽ��� ���� ��� ( ���⸸ , ����ƴϸ� -1 )
//...................................................................................................................
int CEffectManager::GetWeaponEldaStoneLevel(WORD Code)
{
	SItemBaseInfo *pItemBaseInfo = g_Pc_Manager.GetItemBaseInfoPtr( Code );
	
	if( NULL == pItemBaseInfo )
	{
		return -1;
	}
	
	if( n_Class_Eldastone != pItemBaseInfo->theClass )
	{
		return -1;
	}
	
	if(pItemBaseInfo->theSympathy)
	{
		//�����
		return -1;
	}
	
	if(pItemBaseInfo->theVital)
	{
		//���
		return -1;
	}
	
	if(pItemBaseInfo->theInt)
	{
		//�����
		return -1;
	}
	
	if(pItemBaseInfo->theMagicRes)
	{
		//���ֻ�
		return -1;
	}
	
	if(pItemBaseInfo->theStamina)
	{
		//����
		return -1;
	}
	
	if(pItemBaseInfo->theDex)
	{
		//����
		return -1;
	}
	
	if(pItemBaseInfo->theAttackDef)
	{
		//���λ�	
		return -1;
	}
	
	if(pItemBaseInfo->thePower)
	{
		//��Ȳ��
		return -1;
	}
	
	//Ǫ������ �������� �ƴ϶��..
	if( !(pItemBaseInfo->theMinMagicBonus && pItemBaseInfo->theMaxMagicBonus) &&  
		!(pItemBaseInfo->theMinAttackBonus && pItemBaseInfo->theMaxAttackBonus) )
	{
		return -1;
	}
	
	int iLevel = pItemBaseInfo->theGrade/100;
	
	return iLevel;	
}


int CEffectManager::DeleteDefenceEnChantEffect(Character *pSrc)	
{
	
	if( pSrc == NULL )
		return FALSE;
	
	std::list	<CEffectBase *>::iterator i = pSrc->m_EffectList.begin();
	
	for( ; i != pSrc->m_EffectList.end() ; ++i )
	{
		if( (*i)->GetID() == EF_DEFENSED_ENCHANT )
		{
			((CEFAddEffectBase *)*i)->SetDestroy();
			
			return TRUE;
		}
	}
	
	
	
	return FALSE;
}




//...................................................................................................................
// ��þƮ ����Ʈ ����
//...................................................................................................................
int CEffectManager::DeleteWeaponEnChantEffect(Character *pSrc)
{
	if( pSrc == NULL )
		return FALSE;
	
	std::list	<CEffectBase *>::iterator i = pSrc->m_EffectList.begin();
	
	for( ; i != pSrc->m_EffectList.end() ; ++i )
	{
		CEffectBase * pEffect = ((CEffectBase *)*i);
		
		if( pEffect->GetID() == EF_SWORD_UP  ||
			pEffect->GetID() == EF_BOW_UP	 ||
			pEffect->GetID() == EF_BOOK_UP	 ||	
			pEffect->GetID() == EF_BALL_UP	 ||
			pEffect->GetID() == EF_AXE_UP	 ||
			pEffect->GetID() == EF_STICK_UP  
#ifdef ELDA_WEAPON_EFFECT
			||	pEffect->GetID() == EF_BASE_ITEM_EFFECT_PROMINAS   ||
			pEffect->GetID() == EF_BASE_ITEM_EFFECT_PSYCHE   ||
			pEffect->GetID() == EF_BASE_ITEM_EFFECT_MERCENARY   ||
			pEffect->GetID() == EF_BASE_ITEM_EFFECT_KNIGHT   ||
			pEffect->GetID() == EF_BASE_ITEM_EFFECT_SLAYER   ||
			pEffect->GetID() == EF_BASE_ITEM_EFFECT_SNIPER   ||
			pEffect->GetID() == EF_BASE_ITEM_EFFECT_PRIEST   ||
			pEffect->GetID() == EF_BASE_ITEM_EFFECT_HOLY 			
#endif 
			)
		{
			if( pEffect )
			{
				pEffect->SetDestroy();
			}
		}
	}
	
	return FALSE;	 
}

//...........................................................................................................
// ��ų ����Ʈ 1 : 1 �� ����Ʈ�� �������ִ°�
//...........................................................................................................
int CEffectManager::MakeSkillSpotSrcAndDest(Character *pSrc, Character *pDest,
											int SrcSetNum  , int SrcSetType  , float SrcTimer,
											int DestSetNum , int DestSetType , float DestTimer,
											LPEFFECT_EXTRA_INFO pExtraInfo,float Timer )
{
	if( CheckCharacter( pSrc ) )
	{
		if(-1 != SrcSetNum)
		{
			g_Particle.SetEffSetCon( 0.0f , SrcSetNum , SrcSetType , SrcTimer , pSrc->GetUnique() , pSrc->GetIndex() );
			
			if( NULL != pExtraInfo )
			{
				//................................................................................	
				// ��ų �ߵ� ����
				//................................................................................	
				g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
			}
		}
	}
	
	
	if( CheckCharacter( pSrc , pDest ) && NULL != pExtraInfo )
	{
		pExtraInfo->AfterEffect = DestSetNum;
		pExtraInfo->DestType = DestSetType;
		pExtraInfo->lifeTime = DestTimer;
		
		EffectTargetHit( pSrc , pDest , pExtraInfo );
		
		if(-1 != DestSetNum)
		{
			g_Particle.SetEffSetCon( 0.0f , DestSetNum , DestSetType , DestTimer , 
				pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
			
			//................................................................................	
			// ���� ����
			//................................................................................	
			if( pExtraInfo != NULL )
				g_Particle.m_EffectManager.GetSkillAttackedSound( pExtraInfo->SkillCode , pDest );
		}
	}
	else if( CheckCharacter( pSrc , pDest ) && NULL == pExtraInfo)
	{
		if(-1 != DestSetNum)
		{
			g_Particle.SetEffSetCon( 0.0f , DestSetNum , DestSetType , DestTimer , 
				pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
			
			//................................................................................	
			// ���� ����
			//................................................................................	
			if( pExtraInfo != NULL )
				g_Particle.m_EffectManager.GetSkillAttackedSound( pExtraInfo->SkillCode , pDest );
		}
	}
	
	return TRUE;
}


//...........................................................................................................
// ��ų ����Ʈ 1 : N �� ����Ʈ�� �������ִ°�
//...........................................................................................................
int CEffectManager::MakeSkillSpotSrcAndList(Character *pSrc, Character *pDest,
											int SrcSetNum  , int SrcSetType  , float SrcTimer,
											int DestSetNum , int DestSetType , float DestTimer,
											LPEFFECT_EXTRA_INFO pExtraInfo,float Timer )
{
	if( CheckCharacter( pSrc ) )																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																								
	{
		g_Particle.SetEffSetCon( 0.0f , SrcSetNum , SrcSetType , SrcTimer , pSrc->GetUnique() , pSrc->GetIndex() );
		
		if( NULL != pExtraInfo )
		{
			//................................................................................	
			// ��ų �ߵ� ����
			//................................................................................	
			g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
		}
	}
	
	
	if( CheckCharacter( pSrc , pDest ) && NULL != pExtraInfo )
	{
		// 1:N ������
		int Count = pExtraInfo->GetTargetCount();
		
		Character			*pTempDest = NULL;
		EFFECT_EXTRA_INFO    Info;
		
		for( int i = 0 ; i < Count ; ++i )
		{
			if( pExtraInfo->GetTargetInfo( &pTempDest , &Info , i ) )
			{
				g_Particle.SetEffSetCon( 0.0f , DestSetNum , DestSetType , DestTimer , 
					pTempDest->GetUnique() , pTempDest->GetIndex(), ::GetCharPos( D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
				
				Info.AfterEffect = DestSetNum;
				Info.DestType = DestSetType;
				Info.lifeTime = DestTimer;
				EffectTargetHit( pSrc , pTempDest , &Info );
			}
		}
		
		//................................................................................	
		// ���� ����
		//................................................................................	
		if( pExtraInfo != NULL )
			g_Particle.m_EffectManager.GetSkillAttackedSound( pExtraInfo->SkillCode , pDest );
	}
	
	
	return TRUE;
}

//...........................................................................................................
// ��ų ����Ʈ �ҽ� �ʸ� 
//...........................................................................................................
int CEffectManager::MakeSkillSpotSrc(Character *pSrc, Character *pDest,
									 int SrcSetNum  , int SrcSetType  , float SrcTimer,
									 LPEFFECT_EXTRA_INFO pExtraInfo,float Timer )
{
	if( CheckCharacter( pSrc ) )
	{		
		g_Particle.SetEffSetCon( 0.0f , SrcSetNum , SrcSetType , SrcTimer , pSrc->GetUnique() , pSrc->GetIndex() );
		
		if( NULL != pExtraInfo )
		{
			//................................................................................	
			// ��ų �ߵ� ����
			//................................................................................	
			g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
			EffectTargetHit( pSrc , pDest , pExtraInfo );
		}
	}
	
	return TRUE;
}

//...........................................................................................................
// ��ų ����Ʈ �ҽ� �ʸ� 
//...........................................................................................................
int CEffectManager::MakeSkilledSrc(Character *pSrc,int SrcSetNum,int SrcSetType,float SrcTimer)
{
	if( CheckCharacter( pSrc ) )
	{
		g_Particle.SetEffSetCon( 0.0f , SrcSetNum , SrcSetType , SrcTimer , pSrc->GetUnique() , pSrc->GetIndex() );
	}
	
	return TRUE;
}

//...........................................................................................................
// ����Ʈ�� ���
//...........................................................................................................
int CEffectManager::MakeSpotEffect(EFFECT_ID id,Character *pSrc,int EffectNum,int SrcSetType,float STime,float DTimer)
{
	if( CheckCharacter( pSrc ) )
	{
		g_Particle.SetEffSetCon( DTimer , EffectNum , SrcSetType , STime , pSrc->GetUnique() , pSrc->GetIndex() );
	}
	
	return TRUE;
}


//...........................................................................................................
// �÷��̾���� �Ÿ�
//...........................................................................................................
BOOL CEffectManager::ISOverLength(Character *pSrc, float fDist )
{
	if( pSrc == NULL )
		return TRUE;

	if( g_RockClient.GetGameMode() != GMODE_NORMAL )
	{
		return FALSE;
	}
	
	static D3DXVECTOR3 vLength( 0.0f , 0.0f , 0.0f );
	
	vLength.x = g_Pc.GetPlayer()->GetPosTM()._41 - pSrc->GetPosTM()._41; 
	vLength.y = 0.0f; 
	vLength.z = g_Pc.GetPlayer()->GetPosTM()._43 - pSrc->GetPosTM()._43;
	
	if( D3DXVec3Length( &vLength ) >= fDist )
	{
		return TRUE;
	}
	
	return FALSE;
}

//...........................................................................................................
// ������ ��ũ ����Ʈ �߰� by wxywxy
//...........................................................................................................
void CEffectManager::CreateItemLinkEffect(DWORD Code,Character *pSrc)
{
	if( NULL == pSrc )
		return;
	
	if( Code == -1 )
		return;
	
	SItemBaseInfo *item_Info = NULL;
	
	item_Info = g_Pc_Manager.GetItemBaseInfoPtr( Code );
	
	if( NULL != item_Info )
	{
		if( -1 != item_Info->theLinkEff )
		{
			g_Particle.m_EffectManager.CreateEffect( ( EFFECT_ID ) item_Info->theLinkEff , pSrc , NULL , NULL );
		}
	}
}

void CEffectManager::DeleteItemLinkEffect(int pos,Character *pSrc)
{
	if( NULL == pSrc )
		return;
	
	SItemBaseInfo *item_Info = NULL;
	WORD		   Code = -1;
	
	if( pSrc->GetUnique() == g_Pc.GetPlayer()->GetUnique() )
	{
		if( nRui->thePcInven.WearItem[ pos ] )
		{
			Code = nRui->thePcInven.WearItem[ pos ]->Code;
		}
	}
	else
	{
		if( g_Pc.m_PcTable[ pSrc->m_nTableIndex].char_info.theWearItem[ pos ].theCode )
		{
			Code = g_Pc.m_PcTable[ pSrc->m_nTableIndex ].char_info.theWearItem[ pos ].theCode;
		}
	}
	
	if( Code == -1 )
		return;
	
	item_Info = g_Pc_Manager.GetItemBaseInfoPtr( Code );
	
	if( NULL != item_Info )
	{
		if( -1 != item_Info->theLinkEff )
		{
			g_Particle.m_EffectManager.Delete( ( EFFECT_ID ) item_Info->theLinkEff , ( Character * ) pSrc , NULL );
		}
	}
}

//................................................................................................................
// ����Ʈ SetDestroy();
//................................................................................................................
int CEffectManager::DeleteLink(Character *pSrc)
{
	std::list	<CEffectBase *> ::iterator	i = m_EffectList.begin();
	
	for( ; i != m_EffectList.end() ; ++i )
	{
		//........................................................................................................
		// ����Ʈ ����� 
		//........................................................................................................
		
		if( (*i)->GetSrcChar()  == pSrc )
		{
			if( (*i)->ISLinkItem() == TRUE )	
				(*i)->SetDestroy();
		}
	}
	
	return TRUE;
}

D3DXMATRIX CEffectManager::MakeShadowMatrix(Character *pSrc,CVAniModel *pAniModel,D3DXMATRIX matWorld)
{
	D3DXMATRIX  matShadow;
	
	D3DXMatrixIdentity( &matShadow );
	
	if( NULL == pSrc || NULL == pAniModel )
		return matShadow;
	
	D3DXMATRIX  matLocalToLight;
	
	// �������Ʈ��ġ
	D3DXMatrixMultiply( &matLocalToLight, &matWorld, &pSrc->m_Shadow.matWorldToLight );
	
	// ����Ʈ x �������� = �׸������
	D3DXMatrixMultiply( &matShadow, &matLocalToLight, &pSrc->m_Shadow.matShadowProj );
	
	return matShadow;
}

void  CEffectManager::ApplyShaowProj(Character *pSrc,CVAniModel *pAniModel,D3DXMATRIX matWorld)
{	
	if( NULL == pSrc || NULL == pAniModel )
	{
		return;
	}
	
	D3DXMATRIX  matLinkItemLocalToLight;
	
	// �������Ʈ��ġ
	D3DXMatrixMultiply( &matLinkItemLocalToLight, &matWorld, &pSrc->m_Shadow.matWorldToLight );
	
	D3DXVECTOR4 vOut;
	
	for( int j = 0; j < pAniModel->GetVertexCount() ; ++j )
	{									
		D3DXVec3Transform( &vOut, &pAniModel->GetVertex( j ) , &matLinkItemLocalToLight );
		
		if( (float)fabs(vOut.x/vOut.z) > pSrc->m_Shadow.fXMax )
		{
			pSrc->m_Shadow.fXMax = (float)fabs(vOut.x/vOut.z);
		}
		
		if( (float)fabs(vOut.y/vOut.z) > pSrc->m_Shadow.fYMax )
		{
			pSrc->m_Shadow.fYMax = (float)fabs(vOut.y/vOut.z);
		}
	}	
}

int CEffectManager::GetWearCode(Character *pChr,int Pos)
{
	if( NULL == pChr )
		return -1;
	
	if( !pChr->m_bPC && pChr->m_State.nCharacterType != nCharacterTypePlayerNPC )
		return -1;
	
	if( pChr == g_Pc.GetPlayer() )
	{
		SPcItem *pItem = NULL;
		
		pItem = g_RockClient.m_PcInven->WearItem[ Pos ];
		
		if( NULL == pItem )
			return -1;
		
		return ( int ) pItem->Code;
	}
	else
	{
		WORD Code = 0;
		
		if(pChr->m_bPC)
		{
			Code = g_Pc.m_PcTable[ pChr->m_nTableIndex ].char_info.theWearItem[ Pos ].theCode;
		}
		else if(!pChr->m_bPC && pChr->m_State.nCharacterType == nCharacterTypePlayerNPC )
		{
			Code = g_Map.m_NpcTable[ pChr->m_nTableIndex ].theExPcInfo.theWearItem[ Pos ];
		}
		
		if( Code == 0 )
			return -1;
		
		return ( int ) Code;
	}
	
	return -1;
}

BOOL CEffectManager::IsMyRelationUnit( Character *pSrc, Character *pDest )
{
	if( pSrc )
	{
		if( pSrc == g_Pc.GetPlayer() )
		{
			return TRUE;
		}

		if( !pSrc->m_bPC && pSrc->m_State.nPetState == nPetState_Pet )
		{
			///-- �ڽ��� ���̰�... ���������̶��...
			if( pSrc->m_State.PetIndex == MAX_USER_INTABLE )
			{
				SNPCBaseInfo* npc_Info = NULL;
				npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( g_Map.m_NpcTable[pSrc->m_nTableIndex].code );		
				
				if( npc_Info && ( PET_AI_ATTACK == npc_Info->theAttackType ) )
				{
					return TRUE;
				}
			}
		}
	}	
	
	if( pDest )
	{
		if( pDest == g_Pc.GetPlayer() )
		{
			return TRUE;
		}
	}
	
	return FALSE;
}