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
// 이펙트별 헤더 선언
//................................................................................................................
#include "CEFLightning.h"										// 라이트닝
#include "CEFFireBall.h"										// 화이어볼
#include "CEFStickAttack.h"										// 지팡이 기본 공격
#include "CEFBookAttack.h"										// 책 기본 공격
#include "CEFRockPress.h"										// 락 프레스
#include "CEFWoodenSword.h"										// 우든 소드
#include "CEFRockShield.h"										// 락 실드 ( 지속 스킬 )
#include "CEFRefreshSword.h"									// 쾌검 ( 지속 스킬 )
#include "CEFStrongSword.h"										// 강검 ( 지속 스킬 )
#include "CEFGatherCut.h"										// 모아 베기
#include "CEFIceSprie.h"										// 아이스 스피어
#include "CEFBubbleCrash.h"										// 버블 크래쉬
#include "CEFRecoveryItem2.h"									// 회복 아이템2 ( 지속성 )
#include "CEFAvoid.h"											// 회피
#include "CEFBookCriticalAttack.h"								// 책 기본 공격 ( 크리티컬 )
#include "CEFBowAttack.h"										// 활 공격 기본
#include "CEFArrow.h"											// 활
#include "CArrowStuck.h"										// 활 몸에 붙는거
#include "CEFGlareBow.h"										// 노려 쏘기
#include "CEFFootBow.h"											// 발등 쏘기
#include "CEFFootBowLink.h"										// 발등 쏘기 화살 링크 된거
#include "CEFPossessed.h"										// 빙의
#include "CEFPossessedAppear.h"									// 빙신 나타 나기
#include "CEFRefreshBow.h"										// 치유의 화살
#include "CEFFootStrength.h"									// 마인의 각력
#include "CEFFireSmack.h"										// 화염 연타
#include "CEFFireArmor.h"										// 파이어 아머
#include "CEFIceShiled.h"										// 아이스 실드
#include "CEFPriestStandAttack.h"								// 프리스 기본 공격
#include "CEFNpcFireBall.h"										// NPC 화염 공격
#include "CEFSrcLink.h"											// 몸에 붙는 이펙트
#include "CEFKeepUpPump.h"										// 지속 이펙트지만 발동하고 -> 지속이펙트로 변경(EffectTargetHit()처리)  
#include "CEFNPCBubble.h"										// NPC 버블 공격
#include "CEFMouthFire.h"										// NPC 입->불 공격
#include "CEFFruitAttack.h"										// NPC 과일 공격
#include "CEFNpcStoneAttack.h"									// NPC 짱돌 공격
#include "CEFGageTrans.h"										// 게이지 전송
#include "CNPCStickAttack.h"									// NPC 지팡이 공격
#include "CEFAddEffectBase.h"									// 부가 효과 이펙트
#include "CEFHide.h"											// 은신
#include "CEFScapmper.h"										// 질주
#include "CEFMomentHide.h"										// 월영
#include "CEFBowRain.h"											// 오월의 비
#include "CEFSniperBow.h"										// 저격
#include "CEFHealRainBow.h"										// 치유의 비
#include "CEFBeeLineBow.h"										// 직선으로 날라가는 화살류
#include "CEFIceRain.h"											// 아이스 레인
#include "CEFHandPar.h"											// 기열파
#include "CEFTyneeWave.h"										// 타이니 웨이브
#include "CEFBigPress.h"										// 빅프레스
#include "CEFSonBrush.h"										// Son Brush
#include "CEFEnergy.h"											// 연기파
#include "CEFDetect.h"											// Detect
#include "CEFLandDamage.h"										// 광역 데미지
#include "CEFSwordUp.h"											// 칼 인첸트
#include "CEFBowUp.h"											// 활 인첸트
#include "CEFBookUp.h"											// 책 인첸트
#include "CEFBallUp.h"											// 수정구 인첸트
#include "CEFStickUp.h"											// 지팡이 인첸트
#include "CEFHealBubble.h"										// 버블 힐 ( NPC )
#include "CEFBigBubble.h"										// 왕방울 공격 ( NPC )
#include "CEFHPSuction.h"										// HP 흡수 ( NPC )
#include "CEFSPSuction.h"										// SP 흡수 ( NPC )
#include "CEFIceRipBlow.h"										// 차가운 입김 
#include "CEFMagicDelete.h"										// 무력화 
#include "CEFBigNpc0.h"
#include "CEFBigNpcSkill0.h"									// 거대 몬스터 스킬
#include "CEFWing0.h"											// 날개 테스트
#include "CEFFireWork.h"										// 불꽃 
#include "CEFHolyAppear.h"										// 신수 나타날때 이펙트
#include "CEFCommonFlySill.h"									// 날라가는 스킬 이펙트 공통
#include "CEFBizEyeBeam.h"										// 아이빔
#include "CEFFastBow.h"											// 신속의 화살
#include "CEFEmblemFlag.h"
#include "CEFFPopoiAttack.h"									// 포포이 연타공격
#include "CEFTigerAttack.h"										// 호랑이 최종형태 연속공격스킬
#include "CEFFameItem.h"										// 명성셋 지속 이펙트 
#include "CEFFameItemSet.h"										// 명성셋 이펙트 
#include "CEFFireRain.h"										// 파이어 레인

#include "CEFIceField.h"										// 아이스 필드
#include "CEFIceSpear.h"										// 아이스 스피어
#include "CEFLightStorm.h"										// 라이트 스톰
#include "CEFStoneRian.h"										// 운석의 비
#include "CEFWindBush.h"										// 윈드 버쉬

#include "CEFRoundFire.h"										// 라운드 파이어
#include "CEFRoundLight.h"										// 라운드 라이트
#include "CEFCastStone.h"										// 캐스트 스톤

#include "CEFEarthBind.h"										// 대지의 축복
#include "CEFRandomArrow.h"										// 난사

#include "CEFConAniSkill.h"										// 연속 애니메이션 스킬

#include "CEFFireStormSmash.h"									// 열풍강타
#include "CEFSpiritFireSmack.h"									// 기열연타
#include "CEFWrathKniteAttack.h"								// 분노의 검진
#include "CEFCircleCrash.h"										// 서클크래쉬
#include "CEFIceBowRain.h"										// 얼음의 화살비

#include "CEFDarkness.h"
#include "CEFTripleDarkness.h"
#include "CEFTripleFire.h"
#include "CEFGloveDestruction.h"
#include "CEFHolyBomb.h"
#include "CEFDarkBomb.h"

#include "CEFWrath_Bomb.h"
#include "CEFBowDoubleShot.h"
#include "CEFHideSensor.h"

#include "CEFGTEffect.h"										// 가드타워 연관이펙트
#include "CEFCannonEffect.h"									// 거대대포 연관이펙트
#include "CEFGolemEffect.h"										// 거신병 연관 이펙트

#include "CEFBunchStrength.h"									// 마인의 권술
#include "CEFBonWarSkill.h"										// 해골전사용 스킬
#include "CEFBonArcSkill.h"										// 해골아쳐용 스킬

#include "CEFAniWing0.h"										// 애니적용 날개

#include "CEFSpeacialItem.h"									// 클래스 지존아이템 - 연결 이펙트

#include "CEFEldaWeaponItem.h"									// 클래스 엘다스톤 무기아이템 이펙트

#include "CEFDogmaEffect.h"										// 도그마 이펙트
#include "CEFBaragosEff.h"										// 바라고스 이펙트
#include "CEFJanadEff.h"										// 쟈나드 이펙트 

#include "CEFRugalSkill.h"										// 루갈 이펙트

#include "CEFUrobosEff.h"										// 우로보로스 이펙트



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
// InitDevice ( 각 클래스 별로 로딩할 데이터들 )
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
// DeleteDevice ( 각 클래스 별로 지울 데이터들 )
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
	// 전체 이펙트 삭제
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
	// 지역 이펙트 삭제
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
// 지속 광역 이펙트 발동 대기 리스트에 넣기
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
// 지속 광역 이펙트 발동된 리스트에서 빼기
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
// 지속 광역 이펙트 발동 이펙트 이미 리스트에 있남?
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
		
		// Cell 이펙트는 소스를 검사하지 않음
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
		// 이펙트 지우기 
		//........................................................................................................
		if( pEFBase->GetState() == ES_SELF_DESTROY )
		{
			if( NULL != pEFBase )
			{
				// Cell 이펙트
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
					// ( 캐릭터내 리스트도 같이 지워요 )
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
	// 바닥에 마크 찍어 Boa 요
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
	// 이펙트 매니저에 스킬 타입및 셀 값을 입력 
	// 광역 마법시 아이디로 사용함 -> Unique , Race , Skill Code , Cell
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
// Create ( 확장 정보 있을때 )
//................................................................................................................
int CEffectManager::Create(EFFECT_ID id,Character *pSrc,Character *pDest,DWORD event,
						   BYTE critical, WORD skillcode, struct _STargetInfo *pTargetInfo,
						   long lStartTime , DWORD SkillUnique )
{
	switch( m_EffectState )
	{
		// 이펙트 생성
	case	CREATE_EFFECT:
		{
			EFFECT_EXTRA_INFO ExtraInfo;
			
			ExtraInfo.SkillUnique = SkillUnique;
			ExtraInfo.lStartTime = lStartTime;
			
			ExtraInfo.Init( pDest , event , critical , skillcode, pTargetInfo );
			
			CreateEffect( id , pSrc , pDest , &ExtraInfo );
		}
		break;
		
		// 이펙트 락후 처음 실행
	case CREATE_EFFECT_LOCK_ONETIME:
		{
			m_ExtraEffectQueue.Init( pDest , event , critical , skillcode, pTargetInfo );
			m_ExtraEffectQueue.lStartTime = lStartTime;
			
			m_EffectState = CREATE_EFFECT_LOCK;
		}
		break;
		
		// 이펙트 락 
	case CREATE_EFFECT_LOCK:
		{
			m_ExtraEffectQueue.SubInit(  pDest , event , critical , skillcode, pTargetInfo );
		}
		break;
		
		// 리펙트 락 풀린후 이펙트 생성후 -> CREATE_EFFECT 로
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
// Create ( 확장 정보 없을때 )
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
// 모든 이펙트 생성
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
		// 걷기
		//		MAKE_EFFECT(EF_WALK);
		// 걷기 ( 물 )
		//		MAKE_EFFECT(EF_WATER_WAVE);
		//		MAKE_EFFECT(EF_LIGHTNING);
		// 노려베기 ( 임시 파이어볼로 )
		//		MAKE_OTHER(EF_GLARE_CUTTING4)
		// 화이어 볼
		//		MAKE_EFFECT(EF_FIREBALL);
		// 대원참
		//		MAKE_EFFECT(EF_ENERGY_SPREAD);
		
		//........................................................................................................ 
		// 걷기 먼지
		//........................................................................................................ 
		MAKE_EFFECT(EF_RUN_DUST,EF_WALK_DUST);
		MAKE_EFFECT(EF_WALK_DUST,EF_WALK_DUST);
		
		//........................................................................................................ 
		// 걷기 물 파장
		//........................................................................................................ 
		MAKE_EFFECT(EF_RUN_WAVE,EF_WALK_WAVE);
		MAKE_EFFECT(EF_WALK_WAVE,EF_WALK_WAVE);
		
		//........................................................................................................ 
		// 화이어볼
		//........................................................................................................ 
		MAKE_EFFECT(EF_FIREBALL4,EF_FIREBALL0);
		MAKE_EFFECT(EF_FIREBALL3,EF_FIREBALL0);
		MAKE_EFFECT(EF_FIREBALL2,EF_FIREBALL0);
		MAKE_EFFECT(EF_FIREBALL1,EF_FIREBALL0);
		MAKE_EFFECT(EF_FIREBALL0,EF_FIREBALL0);
		
		//........................................................................................................ 
		// 라이트 닝
		//........................................................................................................ 
		MAKE_EFFECT(EF_LIGHTNING4,EF_LIGHTNING0);
		MAKE_EFFECT(EF_LIGHTNING3,EF_LIGHTNING0);
		MAKE_EFFECT(EF_LIGHTNING2,EF_LIGHTNING0);
		MAKE_EFFECT(EF_LIGHTNING1,EF_LIGHTNING0);
		MAKE_EFFECT(EF_LIGHTNING0,EF_LIGHTNING0);
		
		
		//........................................................................................................
		// 기본 공격 이펙트
		//........................................................................................................
		
		// 기본 칼 공격
		MAKE_EFFECT(EF_STAND_SWORD_ATTACK,EF_STAND_SWORD_ATTACK);
		
		// 기본 책 공격
		MAKE_EFFECT(EF_STAND_BOOK_ATTACK,EF_STAND_BOOK_ATTACK);
		
		// 기본 지팡이 공격
		MAKE_EFFECT(EF_STAND_STICK_ATTACK,EF_STAND_STICK_ATTACK);
		
		// 기본 이빨 공격
		MAKE_EFFECT(EF_STAND_TOOTH_ATTACK,EF_STAND_TOOTH_ATTACK);
		
		// 기본 둔기 공격
		MAKE_EFFECT(EF_STAND_DULL_ATTACK,EF_STAND_DULL_ATTACK);
		
		// 기본 손톱 공격
		MAKE_EFFECT(EF_STAND_NAIL_ATTACK,EF_STAND_NAIL_ATTACK);
		
		MAKE_EFFECT(EF_STAND_SWORD_ATTACKED0,EF_STAND_SWORD_ATTACKED0); // 	칼날 기본 데미지 ( 약 )
		MAKE_EFFECT(EF_STAND_SWORD_ATTACKED1,EF_STAND_SWORD_ATTACKED1); // 	칼날 기본 데미지 ( 중 )
		MAKE_EFFECT(EF_STAND_SWORD_ATTACKED2,EF_STAND_SWORD_ATTACKED2); // 	칼날 기본 데미지 ( 강 )
		
		MAKE_EFFECT(EF_STAND_BOOK_ATTACKED0,EF_STAND_BOOK_ATTACKED0);  // 	마법기본공격(책) 데미지 ( 약 ) 
		MAKE_EFFECT(EF_STAND_BOOK_ATTACKED1,EF_STAND_BOOK_ATTACKED1);  // 	마법기본공격(책) 데미지 ( 중 ) 
		MAKE_EFFECT(EF_STAND_BOOK_ATTACKED2,EF_STAND_BOOK_ATTACKED2);  // 	마법기본공격(책) 데미지 ( 강 ) 
		
		MAKE_EFFECT(EF_STAND_STICK_ATTACKED0,EF_STAND_STICK_ATTACKED0); // 	마법기본공격(지팡이) 데미지 ( 약 ) 
		MAKE_EFFECT(EF_STAND_STICK_ATTACKED1,EF_STAND_STICK_ATTACKED1); // 	마법기본공격(지팡이) 데미지 ( 중 ) 
		MAKE_EFFECT(EF_STAND_STICK_ATTACKED2,EF_STAND_STICK_ATTACKED2); // 	마법기본공격(지팡이) 데미지 ( 강 ) 
		
		MAKE_EFFECT(EF_STAND_TOOTH_ATTACKED0,EF_STAND_TOOTH_ATTACKED0); // 	이빨 기본 공격   데미지 ( 약 ) 
		MAKE_EFFECT(EF_STAND_TOOTH_ATTACKED1,EF_STAND_TOOTH_ATTACKED1); // 	이빨 기본 공격   데미지 ( 중 ) 
		MAKE_EFFECT(EF_STAND_TOOTH_ATTACKED2,EF_STAND_TOOTH_ATTACKED2); // 	이빨 기본 공격   데미지 ( 강 ) 
		
		MAKE_EFFECT(EF_STAND_DULL_ATTACKED0,EF_STAND_DULL_ATTACKED0);  // 	둔기 기본 공격   데미지 ( 약 ) 
		MAKE_EFFECT(EF_STAND_DULL_ATTACKED1,EF_STAND_DULL_ATTACKED1);  // 	둔기 기본 공격   데미지 ( 중 ) 
		MAKE_EFFECT(EF_STAND_DULL_ATTACKED2,EF_STAND_DULL_ATTACKED2);  // 	둔기 기본 공격   데미지 ( 강 ) 
		
		MAKE_EFFECT(EF_STAND_NAIL_ATTACKED0,EF_STAND_NAIL_ATTACKED0);  //	발톱 기본 공격  데미지 ( 약 )
		MAKE_EFFECT(EF_STAND_NAIL_ATTACKED1,EF_STAND_NAIL_ATTACKED1);  //	발톱 기본 공격  데미지 ( 중 )
		MAKE_EFFECT(EF_STAND_NAIL_ATTACKED2,EF_STAND_NAIL_ATTACKED2);  //	발톱 기본 공격  데미지 ( 강 )
		
//		MAKE_EFFECT(EF_RUGAL_BASIC_ATTACKED,EF_RUGAL_BASIC_ATTACKED);  //	루갈 기본 공격  데미지 ( 약 )
//		MAKE_EFFECT(EF_RUGAL_CRI_ATTACKED,EF_RUGAL_CRI_ATTACKED);  //	루갈 기본 공격  데미지 ( 강  )
//		MAKE_EFFECT(EF_RUGALSOLDART_BASIC_ATTACKED,EF_RUGALSOLDART_BASIC_ATTACKED);  //	루갈 기본 공격  데미지 ( 약 )
//		MAKE_EFFECT(EF_RUGALSOLDART_CRI_ATTACKED,EF_RUGALSOLDART_CRI_ATTACKED);  //	루갈 기본 공격  데미지 ( 강 )
		MAKE_EFFECT(EF_RUGALTEMPEST_CRI_ATTACKED,EF_RUGALTEMPEST_CRI_ATTACKED);  //	루갈 기본 공격  데미지 (  )




		// 락 프레스
		MAKE_EFFECT(EF_ROCKPRESS4,EF_ROCKPRESS0);
		MAKE_EFFECT(EF_ROCKPRESS3,EF_ROCKPRESS0);
		MAKE_EFFECT(EF_ROCKPRESS2,EF_ROCKPRESS0);
		MAKE_EFFECT(EF_ROCKPRESS1,EF_ROCKPRESS0);
		MAKE_EFFECT(EF_ROCKPRESS0,EF_ROCKPRESS0);
		
		// 우든 소드
		MAKE_EFFECT(EF_WOODENSWORD4,EF_WOODENSWORD0);
		MAKE_EFFECT(EF_WOODENSWORD3,EF_WOODENSWORD0);
		MAKE_EFFECT(EF_WOODENSWORD2,EF_WOODENSWORD0);
		MAKE_EFFECT(EF_WOODENSWORD1,EF_WOODENSWORD0);
		MAKE_EFFECT(EF_WOODENSWORD0,EF_WOODENSWORD0);
		
		// 락 실드 ( 지속 스킬 )
		MAKE_EFFECT(EF_ROCK_SHIELD0,EF_ROCK_SHIELD0);
		MAKE_EFFECT(EF_ROCK_SHIELD1,EF_ROCK_SHIELD0);
		MAKE_EFFECT(EF_ROCK_SHIELD2,EF_ROCK_SHIELD0);
		MAKE_EFFECT(EF_ROCK_SHIELD3,EF_ROCK_SHIELD0);
		MAKE_EFFECT(EF_ROCK_SHIELD4,EF_ROCK_SHIELD0);
		
		// 쾌검 ( 지속 스킬 )
		MAKE_EFFECT(EF_REFRESH_SWORD0,EF_REFRESH_SWORD0);
		MAKE_EFFECT(EF_REFRESH_SWORD1,EF_REFRESH_SWORD0);
		MAKE_EFFECT(EF_REFRESH_SWORD2,EF_REFRESH_SWORD0);
		MAKE_EFFECT(EF_REFRESH_SWORD3,EF_REFRESH_SWORD0);
		MAKE_EFFECT(EF_REFRESH_SWORD4,EF_REFRESH_SWORD0);
		
		// 강검 ( 지속 스킬 )
		MAKE_EFFECT(EF_STRONG_SWORD0,EF_STRONG_SWORD0);
		MAKE_EFFECT(EF_STRONG_SWORD1,EF_STRONG_SWORD0);
		MAKE_EFFECT(EF_STRONG_SWORD2,EF_STRONG_SWORD0);
		MAKE_EFFECT(EF_STRONG_SWORD3,EF_STRONG_SWORD0);
		MAKE_EFFECT(EF_STRONG_SWORD4,EF_STRONG_SWORD0);
		
		// 노려 베기
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
		
		// 모아 베기
		MAKE_EFFECT(EF_GATHER_CUT0,EF_GATHER_CUT0);
		MAKE_EFFECT(EF_GATHER_CUT1,EF_GATHER_CUT0);
		MAKE_EFFECT(EF_GATHER_CUT2,EF_GATHER_CUT0);
		MAKE_EFFECT(EF_GATHER_CUT3,EF_GATHER_CUT0);
		MAKE_EFFECT(EF_GATHER_CUT4,EF_GATHER_CUT0);
		
		// 아이스 스피어
		MAKE_EFFECT(EF_ICESPIRE0,EF_ICESPIRE0);
		MAKE_EFFECT(EF_ICESPIRE1,EF_ICESPIRE0);
		MAKE_EFFECT(EF_ICESPIRE2,EF_ICESPIRE0);
		MAKE_EFFECT(EF_ICESPIRE3,EF_ICESPIRE0);
		MAKE_EFFECT(EF_ICESPIRE4,EF_ICESPIRE0);
		
		// 무장 해제
		MAKE_EFFECT(EF_WEAPON_CRASH0,EF_WEAPON_CRASH0);
		MAKE_EFFECT(EF_WEAPON_CRASH1,EF_WEAPON_CRASH0);
		MAKE_EFFECT(EF_WEAPON_CRASH2,EF_WEAPON_CRASH0);
		MAKE_EFFECT(EF_WEAPON_CRASH3,EF_WEAPON_CRASH0);
		MAKE_EFFECT(EF_WEAPON_CRASH4,EF_WEAPON_CRASH0);
		
		// 회피
		MAKE_EFFECT(EF_AVOID0,EF_AVOID0);
		MAKE_EFFECT(EF_AVOID1,EF_AVOID0);
		MAKE_EFFECT(EF_AVOID2,EF_AVOID0);
		MAKE_EFFECT(EF_AVOID3,EF_AVOID0);
		MAKE_EFFECT(EF_AVOID4,EF_AVOID0);
		
		// 버블 크래쉬
		MAKE_EFFECT(EF_BUBBLE_CRASH0,EF_BUBBLE_CRASH0);
		MAKE_EFFECT(EF_BUBBLE_CRASH1,EF_BUBBLE_CRASH0);
		MAKE_EFFECT(EF_BUBBLE_CRASH2,EF_BUBBLE_CRASH0);
		MAKE_EFFECT(EF_BUBBLE_CRASH3,EF_BUBBLE_CRASH0);
		MAKE_EFFECT(EF_BUBBLE_CRASH4,EF_BUBBLE_CRASH0);
		
		// 회복 아이템1 즉효성 
		MAKE_EFFECT(EF_RECOVERY_ITEM1_0,EF_RECOVERY_ITEM1_0);
		MAKE_EFFECT(EF_RECOVERY_ITEM1_1,EF_RECOVERY_ITEM1_0);
		MAKE_EFFECT(EF_RECOVERY_ITEM1_2,EF_RECOVERY_ITEM1_0);
		MAKE_EFFECT(EF_RECOVERY_ITEM1_3,EF_RECOVERY_ITEM1_0);
		MAKE_EFFECT(EF_RECOVERY_ITEM1_4,EF_RECOVERY_ITEM1_0);
		
		// 회복 아이템2 지속성 
		MAKE_EFFECT(EF_RECOVERY_ITEM2_0,EF_RECOVERY_ITEM2_0);
		MAKE_EFFECT(EF_RECOVERY_ITEM2_1,EF_RECOVERY_ITEM2_0);
		MAKE_EFFECT(EF_RECOVERY_ITEM2_2,EF_RECOVERY_ITEM2_0);
		MAKE_EFFECT(EF_RECOVERY_ITEM2_3,EF_RECOVERY_ITEM2_0);
		MAKE_EFFECT(EF_RECOVERY_ITEM2_4,EF_RECOVERY_ITEM2_0);
		
		// 회복 아이템2 지속성 
		MAKE_EFFECT(EF_BOW_ATTACK0,EF_BOW_ATTACK0);
		
		// 기본 활공격 데미지
		MAKE_EFFECT(EF_BOW_ATTACKED_0,EF_BOW_ATTACKED0);
		MAKE_EFFECT(EF_BOW_ATTACKED_1,EF_BOW_ATTACKED0);
		MAKE_EFFECT(EF_BOW_ATTACKED_2,EF_BOW_ATTACKED0);
		
		// 노려 쏘기
		MAKE_EFFECT(EF_GLAER_BOW_0,EF_GLAER_BOW_ATTACK);
		MAKE_EFFECT(EF_GLAER_BOW_1,EF_GLAER_BOW_ATTACK);
		MAKE_EFFECT(EF_GLAER_BOW_2,EF_GLAER_BOW_ATTACK);
		
		MAKE_EFFECT(EF_DOUBLESHOT , EF_DOUBLESHOT);
		
		// 발등 쏘기
		MAKE_EFFECT(EF_FOOT_BOW_0,EF_FOOT_BOW_ATTACK);
		MAKE_EFFECT(EF_FOOT_BOW_1,EF_FOOT_BOW_ATTACK);
		MAKE_EFFECT(EF_FOOT_BOW_2,EF_FOOT_BOW_ATTACK);
		
		// 발등 화살 붙는거 
		MAKE_EFFECT(EF_FOOT_BOW_LINK,EF_FOOT_BOW_LINK);
		
		// 화염 연타
		MAKE_EFFECT(EF_FIRE_SMACK_0,EF_FIRE_SMACK);
		MAKE_EFFECT(EF_FIRE_SMACK_1,EF_FIRE_SMACK);
		MAKE_EFFECT(EF_FIRE_SMACK_2,EF_FIRE_SMACK);
		
		// 빙의
		MAKE_EFFECT(EF_POSSESSED_0,EF_POSSESSED);
		MAKE_EFFECT(EF_POSSESSED_1,EF_POSSESSED);
		MAKE_EFFECT(EF_POSSESSED_2,EF_POSSESSED);
		MAKE_EFFECT(EF_POSSESSED_3,EF_POSSESSED);
		MAKE_EFFECT(EF_POSSESSED_4,EF_POSSESSED);
		
		// 빙신 나타나기
		MAKE_EFFECT(EF_APPEAR_BING0,EF_APPEAR_BING);
		MAKE_EFFECT(EF_APPEAR_BING1,EF_APPEAR_BING);
		MAKE_EFFECT(EF_APPEAR_BING2,EF_APPEAR_BING);
		MAKE_EFFECT(EF_APPEAR_BING3,EF_APPEAR_BING);
		MAKE_EFFECT(EF_APPEAR_BING4,EF_APPEAR_BING);
		
		// 빙의 해제
		MAKE_EFFECT(EF_POSSESSED_CANCEL,EF_POSSESSED_CANCEL);
		
		// 속사
		MAKE_EFFECT(EF_QUICK_BOW_0,EF_QUICK_BOW);
		MAKE_EFFECT(EF_QUICK_BOW_1,EF_QUICK_BOW);
		MAKE_EFFECT(EF_QUICK_BOW_2,EF_QUICK_BOW);
		
		// 집중
		MAKE_EFFECT(EF_FOCUS_BOW_0,EF_FOCUS_BOW);
		MAKE_EFFECT(EF_FOCUS_BOW_1,EF_FOCUS_BOW);
		MAKE_EFFECT(EF_FOCUS_BOW_2,EF_FOCUS_BOW);
		
		// 치유의 화살
		MAKE_EFFECT(EF_REFRESH_BOW_0,EF_REFRESH_BOW);
		MAKE_EFFECT(EF_REFRESH_BOW_1,EF_REFRESH_BOW);
		MAKE_EFFECT(EF_REFRESH_BOW_2,EF_REFRESH_BOW);
		
		// 벽뢰각
		MAKE_EFFECT(EF_FOOT_LIGHTNING_0,EF_FOOT_LIGHTNING);
		MAKE_EFFECT(EF_FOOT_LIGHTNING_1,EF_FOOT_LIGHTNING);
		MAKE_EFFECT(EF_FOOT_LIGHTNING_2,EF_FOOT_LIGHTNING);
		
		// 절대 방어
		MAKE_EFFECT(EF_ABSOLUTE_DP_0,EF_ABSOLUTE_DP);
		MAKE_EFFECT(EF_ABSOLUTE_DP_1,EF_ABSOLUTE_DP);
		MAKE_EFFECT(EF_ABSOLUTE_DP_2,EF_ABSOLUTE_DP);
		
		// 마인의 각력
		MAKE_EFFECT(EF_FOOT_STRENGTH_0,EF_FOOT_STRENGTH);
		MAKE_EFFECT(EF_FOOT_STRENGTH_1,EF_FOOT_STRENGTH);
		MAKE_EFFECT(EF_FOOT_STRENGTH_2,EF_FOOT_STRENGTH);
		
		MAKE_EFFECT(EF_HAND_ATTACK,EF_HAND_ATTACK0);
		
		// 손 기본 공격
		MAKE_EFFECT(EF_HAND_ATTACKED0,EF_HAND_ATTACKED);
		MAKE_EFFECT(EF_HAND_ATTACKED1,EF_HAND_ATTACKED);
		MAKE_EFFECT(EF_HAND_ATTACKED2,EF_HAND_ATTACKED);
		
		// 샤먼 기본 수정구 공격
		MAKE_EFFECT(EF_BALL_ATTACK,EF_BALL_ATTACK);
		MAKE_EFFECT(EF_BALL_ATTACKED0,EF_BALL_ATTACKED);
		MAKE_EFFECT(EF_BALL_ATTACKED1,EF_BALL_ATTACKED);
		MAKE_EFFECT(EF_BALL_ATTACKED2,EF_BALL_ATTACKED);
		
		//		// 받아치기
		//		MAKE_EFFECT(EF_BOUND_ATTACK,EF_BOUND_ATTACK);
		
		// 파이어 아머
		MAKE_EFFECT(EF_FIRE_ARMOR0,EF_FIRE_ARMOR);
		MAKE_EFFECT(EF_FIRE_ARMOR1,EF_FIRE_ARMOR);
		MAKE_EFFECT(EF_FIRE_ARMOR2,EF_FIRE_ARMOR);
		
		// 아이스 실드
		MAKE_EFFECT(EF_ICE_SHILED0,EF_ICE_SHILED);
		MAKE_EFFECT(EF_ICE_SHILED1,EF_ICE_SHILED);
		MAKE_EFFECT(EF_ICE_SHILED2,EF_ICE_SHILED);
		
		//........................................................................................................
		// 3차 추가 이펙트
		//........................................................................................................
		// 나이트 기본 공격
		MAKE_EFFECT(EF_NIGHT_STAND_ATTACK,EF_NIGHT_STAND_ATTACK);
		MAKE_EFFECT(EF_NIGHT_STAND_ATTACKED0,EF_NIGHT_STAND_ATTACKED);
		MAKE_EFFECT(EF_NIGHT_STAND_ATTACKED1,EF_NIGHT_STAND_ATTACKED);
		MAKE_EFFECT(EF_NIGHT_STAND_ATTACKED2,EF_NIGHT_STAND_ATTACKED);
		
		// 방패 가격
		MAKE_EFFECT(EF_SHILED_ATTACK0,EF_SHILED_ATTACKED);
		MAKE_EFFECT(EF_SHILED_ATTACK1,EF_SHILED_ATTACKED);
		MAKE_EFFECT(EF_SHILED_ATTACK2,EF_SHILED_ATTACKED);
		// 방어 태세
		MAKE_EFFECT(EF_DEFEND_SETUP,EF_DEFEND_SETUP);
		
		// 슬레이어 기본 공격
		MAKE_EFFECT(EF_SLAYER_STAND_ATTACK,EF_SLAYER_STAND_ATTACK);
		MAKE_EFFECT(EF_SLAYER_STAND_ATTACKED0,EF_SLAYER_STAND_ATTACKED);
		MAKE_EFFECT(EF_SLAYER_STAND_ATTACKED1,EF_SLAYER_STAND_ATTACKED);
		MAKE_EFFECT(EF_SLAYER_STAND_ATTACKED2,EF_SLAYER_STAND_ATTACKED);
		
		// 피의 폭주
		MAKE_EFFECT(EF_BLOOD_RUNAWAY,EF_BLOOD_RUNAWAY);
		
		// 불굴의 의지
		MAKE_EFFECT(EF_LIVE_MIND,EF_LIVE_MIND);
		
		// 프리스트 기본 공격
		MAKE_EFFECT(EF_PRIEST_STAND_ATTACK,EF_PRIEST_STAND_ATTACK);
		MAKE_EFFECT(EF_PRIEST_STAND_ATTACKED0,EF_PRIEST_STAND_ATTACKED);
		MAKE_EFFECT(EF_PRIEST_STAND_ATTACKED1,EF_PRIEST_STAND_ATTACKED);
		MAKE_EFFECT(EF_PRIEST_STAND_ATTACKED2,EF_PRIEST_STAND_ATTACKED);
		
		// 프리스트 힐링 
		MAKE_EFFECT(EF_HEAL0,EF_PRIEST_HEAL);
		MAKE_EFFECT(EF_HEAL1,EF_PRIEST_HEAL);
		MAKE_EFFECT(EF_HEAL2,EF_PRIEST_HEAL);
		
		// 프리스트 디스펠
		MAKE_EFFECT(EF_DISPELL,EF_PRIEST_DISPELL);
		
		// 프리스트 행운의 여신
		MAKE_EFFECT(EF_LUCKY_GOD,EF_LUCKY_GOD);
		// 프리스트 행운의 여신 당하는쪽
		MAKE_EFFECT(EF_LUCKY_GODED,EF_LUCKY_GODED);
		
		// 프리스트 부활
		MAKE_EFFECT(EF_REVIVAL,EF_REVIVAL);
		
		// 머록스 나무정령
		MAKE_EFFECT(EF_NPC_WOOD_ATTACK,EF_NPC_WOOD_ATTACK);
		MAKE_EFFECT(EF_NPC_WOOD_ATTACKED0,EF_NPC_WOOD_ATTACKED);
		MAKE_EFFECT(EF_NPC_WOOD_ATTACKED1,EF_NPC_WOOD_ATTACKED);
		MAKE_EFFECT(EF_NPC_WOOD_ATTACKED2,EF_NPC_WOOD_ATTACKED);
		
		// 블러켄 화이어볼
		MAKE_EFFECT(EF_NPC_FIRE,EF_NPC_FIRE);					// 기본 공격
		MAKE_EFFECT(EF_NPC_FIRE_SKILL,EF_NPC_FIRE);				// 스킬
		
		MAKE_EFFECT(EF_NPC_FIREED0,EF_NPC_FIREED);				// 기본 공격 데미지 이펙트
		MAKE_EFFECT(EF_NPC_FIREED1,EF_NPC_FIREED);
		MAKE_EFFECT(EF_NPC_FIREED2,EF_NPC_FIREED);
		
		// NPC 파이몬 우두머리 때리기 ( 기본 공격 )
		MAKE_EFFECT(EF_NPC_HEAVY_STICK,EF_NPC_HEAVY_STICK);
		MAKE_EFFECT(EF_NPC_HEAVY_STICKED0,EF_NPC_HEAVY_STICKED);
		MAKE_EFFECT(EF_NPC_HEAVY_STICKED1,EF_NPC_HEAVY_STICKED);
		MAKE_EFFECT(EF_NPC_HEAVY_STICKED2,EF_NPC_HEAVY_STICKED);
		
		// 발구르기
		MAKE_EFFECT(EF_FOOT_QUAKE0,EF_FOOT_QUAKE);
		MAKE_EFFECT(EF_FOOT_QUAKE1,EF_FOOT_QUAKE);
		MAKE_EFFECT(EF_FOOT_QUAKE2,EF_FOOT_QUAKE);
		
		// 도발
		MAKE_EFFECT(EF_ATTACKED_FOCUS,EF_ATTACKED_FOCUS);
		
		// 검진
		MAKE_EFFECT(EF_SWORD_FLASH0,EF_SWORD_FLASH);
		MAKE_EFFECT(EF_SWORD_FLASH1,EF_SWORD_FLASH);
		MAKE_EFFECT(EF_SWORD_FLASH2,EF_SWORD_FLASH);
		
		// 파티 힐링
		MAKE_EFFECT(EF_PARTY_HEAL,EF_PARTY_HEAL);
		
		// 파이몬 우두머리 바닥 찍기 ( 스킬 )
		MAKE_EFFECT(EF_NPC_QUAKE_HEAVY_STICK,EF_NPC_QUAKE_HEAVY_STICK);
		
		// 대원참
		MAKE_EFFECT(EF_CIRCLE_SWORD,EF_CIRCLE_SWORD);
		
		MAKE_EFFECT_TIMER(EF_PRESSURE_AURA,EF_KEEPUP_PUMP,3.1f);
		MAKE_EFFECT_TIMER(EF_WARM_LIGHT,EF_KEEPUP_PUMP,3.1f);
		
		MAKE_EFFECT(EF_WARM_LIGHT1,EF_SRC_LINK);
		MAKE_EFFECT(EF_PRESSURE_AURA1,EF_SRC_LINK);
		
		
		MAKE_EFFECT(EF_PRESSURE_AURAED,EF_SRC_LINK);
		MAKE_EFFECT(EF_WARM_LIGHTED,EF_SRC_LINK);
		//MAKE_EFFECT(EF_WARM_LIGHTED,EF_WARM_LIGHTED);
		
		// NPC 물방울 공격 스킬
		MAKE_EFFECT(EF_BUBBLE_ATTACK,EF_NPC_BUBBLE_ATTACK);
		
		// NPC 도끼로 바닥찍기
		MAKE_EFFECT(EF_AXE_LAND_HIT,EF_AXE_LAND_HIT);
		
		// 포스토스 입 -> 불
		MAKE_EFFECT(EF_MOUTH_FIRE,EF_MOUTH_FIRE);
		MAKE_EFFECT(EF_MOUTH_FIRE1,EF_MOUTH_FIRE);
		
		// NPC 과일 공격
		MAKE_EFFECT(EF_FRUIT_ATTACK,EF_FRUIT_ATTACK);
		
		MAKE_EFFECT(EF_ROCK_SHOT,EF_ROCK_SHOT);
		MAKE_EFFECT(EF_ROCK_SHOT1,EF_ROCK_SHOT);
		
		// 볼락 가스
		MAKE_EFFECT(EF_GAS_SHOT,EF_GAS_SHOT);
		
		// 게이지 전송
		MAKE_EFFECT(EF_GAGE_TRANS,EF_GAGE_TRANS);
		
		// NPC 지팡이 공격 ( 기본 공격 )
		MAKE_EFFECT(EF_NPC_STICK,EF_NPC_STICK);
		// NPC 지팡이 공격 데미지 ( 기본 공격 )
		MAKE_EFFECT(EF_NPC_STICK_ATTACKED0,EF_NPC_STICK_ATTACKED);
		MAKE_EFFECT(EF_NPC_STICK_ATTACKED1,EF_NPC_STICK_ATTACKED);
		MAKE_EFFECT(EF_NPC_STICK_ATTACKED2,EF_NPC_STICK_ATTACKED);
		
		// 부가 효과
		MAKE_EFFECT(EF_MOVE_OBSTRUCTION,EF_ADDEFFECT_BASE);	 			// 이동장애
		MAKE_EFFECT(EF_FAINT,EF_ADDEFFECT_BASE);						// 마비 or 기절
		MAKE_EFFECT(EF_SLEEP,EF_ADDEFFECT_BASE);						// 수면
		MAKE_EFFECT(EF_POISONING,EF_ADDEFFECT_BASE);					// 중독
		MAKE_EFFECT(EF_PROVOCATION,EF_ADDEFFECT_BASE);					// 도발
		MAKE_EFFECT(EF_COMBO_OBSTRUCTION,EF_ADDEFFECT_BASE);			// 콤보장애		
		MAKE_EFFECT(EF_FREEZING,EF_ADDEFFECT_BASE);						// 빙결			
		MAKE_EFFECT(EF_STONY,EF_ADDEFFECT_BASE);						// 석화			
		MAKE_EFFECT(EF_WHISPER,EF_ADDEFFECT_BASE);						// 침묵
		MAKE_EFFECT(EF_PARALYSYS,EF_ADDEFFECT_BASE);
		MAKE_EFFECT(EF_SKILL_DISABLE,EF_ADDEFFECT_BASE);
		MAKE_EFFECT(EF_ADD_CURSE,EF_ADDEFFECT_BASE);
		MAKE_EFFECT(EF_ADD_UP,EF_ADDEFFECT_BASE);
		
		//...........................................................................................................
		// 이그니션
		//...........................................................................................................
		MAKE_EFFECT(EF_EGNEETION0,EF_EGNEETION);						// 이그니션
		MAKE_EFFECT(EF_EGNEETION1,EF_EGNEETION);
		MAKE_EFFECT(EF_EGNEETION2,EF_EGNEETION);
		//...........................................................................................................
		// 이그니션 데미지
		//...........................................................................................................
		MAKE_EFFECT(EF_EGNEETION_ED0,EF_EGNEETION_ED);					// 이그니션 데미지 ( 지속 )
		MAKE_EFFECT(EF_EGNEETION_ED1,EF_EGNEETION_ED);
		MAKE_EFFECT(EF_EGNEETION_ED2,EF_EGNEETION_ED);
		//...........................................................................................................
		// 차지
		//...........................................................................................................
		MAKE_EFFECT(EF_CHARGE,EF_CHARGE);								// 차지 ( 지 속 )
		//...........................................................................................................
		// 칠링 터치
		//...........................................................................................................
		MAKE_EFFECT(EF_CR_TOUCH0,EF_CR_TOUCH);
		MAKE_EFFECT(EF_CR_TOUCH1,EF_CR_TOUCH);
		MAKE_EFFECT(EF_CR_TOUCH2,EF_CR_TOUCH);
		
		//...........................................................................................................
		// 힐링 웨이브
		//...........................................................................................................
		MAKE_EFFECT(EF_HEAL_WAVE0,EF_HEAL_WAVE);
		MAKE_EFFECT(EF_HEAL_WAVE1,EF_HEAL_WAVE);
		MAKE_EFFECT(EF_HEAL_WAVE2,EF_HEAL_WAVE);
		
		//...........................................................................................................
		// 펄리닉 더스트
		//...........................................................................................................
		MAKE_EFFECT(EF_PEAL_DUST0,EF_PEAL_DUST);
		MAKE_EFFECT(EF_PEAL_DUST1,EF_PEAL_DUST);
		MAKE_EFFECT(EF_PEAL_DUST2,EF_PEAL_DUST);
		
		MAKE_EFFECT(EF_PEAL_DUST_ED0,EF_SRC_LINK);
		MAKE_EFFECT(EF_PEAL_DUST_ED1,EF_SRC_LINK);
		MAKE_EFFECT(EF_PEAL_DUST_ED2,EF_SRC_LINK);
		
		//...........................................................................................................
		// 포이즌 시드
		//...........................................................................................................
		MAKE_EFFECT(EF_POISON_SEED0,EF_POISON_SEED);
		MAKE_EFFECT(EF_POISON_SEED1,EF_POISON_SEED);
		MAKE_EFFECT(EF_POISON_SEED2,EF_POISON_SEED);
		
		//...........................................................................................................
		// 기 합
		//...........................................................................................................
		MAKE_EFFECT(EF_SELF_SHOUT,EF_SELF_SHOUT);
		
		//...........................................................................................................
		// 은 신
		//...........................................................................................................
		MAKE_EFFECT(EF_HIDE,EF_HIDE);
		
		//...........................................................................................................
		// 암연살
		//...........................................................................................................
		MAKE_EFFECT(EF_MAYUNSAL,EF_MAYUNSAL);
		
		//...........................................................................................................
		// 질주
		//...........................................................................................................
		MAKE_EFFECT(EF_SCAPMPER,EF_SCAPMPER);
		
		//...........................................................................................................
		// 월영
		//...........................................................................................................
		MAKE_EFFECT(EF_MOMENT_HIDE,EF_MOMENT_HIDE);
		
		//...........................................................................................................
		// 검기
		//...........................................................................................................
		MAKE_EFFECT(EF_SWORD_GHOST,EF_SRC_LINK);
		
		//...........................................................................................................
		// 받아치기
		//...........................................................................................................
		MAKE_EFFECT(EF_BOUND_ATTACK,EF_SRC_LINK);
		
		//..............................................................................................................
		// 오월의 비
		//..............................................................................................................
		MAKE_EFFECT(EF_BOW_RAIN,EF_BOW_RAIN);
		
		//..............................................................................................................
		// 저 격
		//..............................................................................................................
		MAKE_EFFECT(EF_SNIPER_BOW,EF_SNIPER_BOW);
		
		//..............................................................................................................
		// 치유의 비
		//..............................................................................................................
		MAKE_EFFECT(EF_HEAL_RAIN_BOW,EF_HEAL_RAIN_BOW);
		
		//..............................................................................................................
		// 침묵의 화살
		//..............................................................................................................
		MAKE_EFFECT(EF_WHISPER_BOW,EF_WHISPER_BOW);
		
		//..............................................................................................................
		// 정화의 화살
		//..............................................................................................................
		MAKE_EFFECT(EF_CLEAR_BOW,EF_CLEAR_BOW);
		
		//..............................................................................................................
		// 해제의 화살
		//..............................................................................................................
		MAKE_EFFECT(EF_CANCEL_BOW,EF_CANCEL_BOW);
		
		//..............................................................................................................
		// 각성의 화살
		//..............................................................................................................
		MAKE_EFFECT(EF_COMBO_ADD_BOW,EF_COMBO_ADD_BOW);
		
		//..............................................................................................................
		// 아이스 레인
		//..............................................................................................................
		MAKE_EFFECT(EF_ICE_RAIN,EF_ICE_RAIN);
		MAKE_EFFECT(EF_ICE_RAINED,EF_ICE_RAINED);
		
		//..............................................................................................................
		// Fire 레인 
		//..............................................................................................................
		MAKE_EFFECT(EF_FIRE_RAIN, EF_FIRE_RAIN );
		MAKE_SKILL_SPOT_SRC(EF_FIRE_RAINED,EF_FIREBALL_ATTACKED0,ESLT_CHARPOS,5.1f);	
		
		//..............................................................................................................
		// 기열파
		//..............................................................................................................
		MAKE_EFFECT(EF_HAND_PAR,EF_HAND_PAR);
		
		//..............................................................................................................
		// 프레임 스톰
		//..............................................................................................................
		MAKE_EFFECT(EF_FRAME_STORM,EF_FRAME_STORM);
		
		//..............................................................................................................
		// 라이트닝 스피어
		//..............................................................................................................
		MAKE_EFFECT(EF_LIGHTNIG_SPIRE0,EF_LIGHTNIG_SPIRE);
		MAKE_EFFECT(EF_LIGHTNIG_SPIRE1,EF_LIGHTNIG_SPIRE);
		MAKE_EFFECT(EF_LIGHTNIG_SPIRE2,EF_LIGHTNIG_SPIRE);
		
		//..............................................................................................................
		// 타이니 웨이브
		//..............................................................................................................
		MAKE_EFFECT(EF_TYNEE_WAVE,EF_TYNEE_WAVE);
		
		//..............................................................................................................
		// 빅프레스
		//..............................................................................................................
		MAKE_EFFECT(EF_BIG_PRESS,EF_BIG_PRESS);
		
		//..............................................................................................................
		// 쏜 버쉬
		//..............................................................................................................
		MAKE_EFFECT(EF_SON_BURSH,EF_SON_BURSH);
		
		//..............................................................................................................
		// 텔레포트
		//..............................................................................................................
		MAKE_EFFECT(EF_TELEPORT,EF_TELEPORT);
		
		//..............................................................................................................
		// 슬립
		//..............................................................................................................
		MAKE_EFFECT(EF_SLEEP_STICK,EF_SLEEP_STICK);
		
		//..............................................................................................................
		// 마나 버퍼링
		//..............................................................................................................
		MAKE_EFFECT(EF_MANA_BUFFERING,EF_MANA_BUFFERING);
		
		//..............................................................................................................
		// 리턴
		//..............................................................................................................
		MAKE_EFFECT(EF_RETURN_HOME,EF_RETURN_HOME);
		
		//..............................................................................................................
		// 셀프 힐링
		//..............................................................................................................
		MAKE_EFFECT(EF_SELF_HEAL,EF_SELF_HEAL);
		
		//..............................................................................................................
		// 디스펠 배드
		//..............................................................................................................
		MAKE_EFFECT(EF_DISPELL_BAD,EF_DISPELL_BAD);
		
		//..............................................................................................................
		// 블레스
		//..............................................................................................................
		MAKE_EFFECT(EF_BRESS,EF_BRESS);
		MAKE_EFFECT(EF_BRESS_ED,EF_SRC_LINK);		// 블래스 당하는 쪽 소스쪽에 달아주기
		
		//..............................................................................................................
		// 디텍트
		//..............................................................................................................
		MAKE_EFFECT(EF_DETECT,EF_DETECT);
		
		//..............................................................................................................
		// 디펜스 실드
		//..............................................................................................................
		MAKE_EFFECT(EF_DEPENCE_SHILED,EF_DEPENCE_SHILED);
		MAKE_EFFECT(EF_DEPENCE_SHILED_ED,EF_SRC_LINK);
		
		//..............................................................................................................
		// 디펜스 오로라
		//..............................................................................................................
		MAKE_EFFECT(EF_DEFENSE_AURA,EF_DEFENSE_AURA);
		MAKE_EFFECT(EF_DEFENSE_AURA_ED,EF_SRC_LINK);
		
		//..............................................................................................................
		// 홀리 라이트
		//..............................................................................................................
		MAKE_EFFECT(EF_HOLY_LIGHT,EF_HOLY_LIGHT);
		
		//..............................................................................................................
		// 봉인의 빛
		//..............................................................................................................
		MAKE_EFFECT(EF_UNCOMBO,EF_UNCOMBO);
		
		//..............................................................................................................
		// 현혹
		//..............................................................................................................
		MAKE_EFFECT(EF_FAINT_SELF,EF_SRC_LINK);
		
		//..............................................................................................................
		// 한계돌파
		//..............................................................................................................
		MAKE_EFFECT(EF_LIMIT_RUSH,EF_SRC_LINK);
		
		//..............................................................................................................
		// 분신
		//..............................................................................................................
		MAKE_EFFECT(EF_SELF_COPY,EF_SELF_COPY);
		
		//..............................................................................................................
		// 연기파
		//..............................................................................................................
		MAKE_EFFECT(EF_ENERGY0,EF_ENERGY);
		MAKE_EFFECT(EF_ENERGY1,EF_ENERGY);
		MAKE_EFFECT(EF_ENERGY2,EF_ENERGY);
		
		//..............................................................................................................
		// 선풍열각
		//..............................................................................................................
		MAKE_EFFECT(EF_CIRCLE_KICK,EF_CIRCLE_KICK);
		
		//..............................................................................................................
		// 치유의 빛
		//..............................................................................................................
		MAKE_EFFECT(EF_HEAL_LIGHT,EF_SRC_LINK);
		MAKE_EFFECT(EF_HEAL_LIGHT_ED,EF_SRC_LINK);
		
		//..............................................................................................................
		// 되돌리기
		//..............................................................................................................
		MAKE_EFFECT(EF_CROSS_COUNTOR,EF_SRC_LINK);
		
		//..............................................................................................................
		// NPC 상자 죽을때 나오는 거~ ( 낭심 본 )
		//..............................................................................................................
		MAKE_EFFECT(EF_BOX_DIE,EF_SRC_LINK);
		
		//..............................................................................................................
		// 괴성
		//..............................................................................................................
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_SHOUT,EF_SHOUT,ESLT_CHARPOS,2.1f,EF_SHOUTED,ESLT_CHARPOS,2.1f);
		//..............................................................................................................
		// 몽둥이 강타
		//..............................................................................................................
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_STRONG_STICK,EF_STRONG_STICK,ESLT_CHARPOS,2.1f,EF_STRONG_STICKED,ESLT_CHARPOS,2.1f);
		//..............................................................................................................
		// 점프해서 바닥에 떨어지기
		//..............................................................................................................
		MAKE_SKILL_SPOT_SRC_AND_LIST(EF_HIT_LAND,EF_HIT_LAND,ESLT_CHARPOS,2.1f,EF_HIT_LANDED,ESLT_CHARPOS,2.1f);
		//..............................................................................................................
		// 인탱글
		//..............................................................................................................
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_NPC_ENTANGLE,EF_NPC_ENTANGLE,ESLT_CHARPOS,2.1f,EF_NPC_ENTANGLEED,ESLT_CHARPOS,5.1f);
		//..............................................................................................................
		// 레드트렌트의 저주
		//..............................................................................................................
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_NPC_REDTRENT,EF_NPC_REDTRENT,ESLT_CHARPOS,2.1f,EF_NPC_REDTRENT_ED,ESLT_CHARPOS,2.1f);
		//..............................................................................................................
		// 석화
		//..............................................................................................................
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_NPC_STONY,EF_NPC_STONY,ESLT_CHARPOS,2.1f,EF_NPC_STONYED,ESLT_CHARPOS,2.1f);
		//..............................................................................................................
		// 나무화
		//..............................................................................................................
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_NPC_TREE,EF_NPC_TREE,ESLT_CHARPOS,2.1f,EF_NPC_TREEED,ESLT_CHARPOS,2.1f);
		//..............................................................................................................
		// 프레임 스트라이크
		//..............................................................................................................
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_FRAME_STRIKE,EF_FRAME_STRIKE,ESLT_CHARPOS,2.1f,EF_FRAME_STRIKE_ED,ESLT_CHARPOS,6.1f);
		//..............................................................................................................
		// 무력화
		//..............................................................................................................
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_NEUTRALIZE,EF_NEUTRALIZE,ESLT_CHARPOS,2.1f,EF_NEUTRALIZE_ED,ESLT_CHARPOS,2.1f);
		//..............................................................................................................
		// 손을맞잡고 드릴처럼 공격
		//..............................................................................................................
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_HAND_DRILL,EF_HAND_DRILL,ESLT_CHARPOS,2.1f,EF_HAND_DRILLED,ESLT_CHARPOS,2.1f);
		//..............................................................................................................
		// 달빛의 저주
		//..............................................................................................................
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_MOON_CURSE,EF_MOON_CURSE,ESLT_CHARPOS,2.1f,EF_MOON_CURSE_ED,ESLT_CHARPOS,2.1f);
		//..............................................................................................................
		// 광역 저주
		//..............................................................................................................
		MAKE_SKILL_SPOT_SRC_AND_LIST(EF_LAND_CURSE,EF_LAND_CURSE,ESLT_CHARPOS,2.1f,EF_LAND_CURSE_ED,ESLT_CHARPOS,2.1f);
		//..............................................................................................................
		// 광분의 포효
		//..............................................................................................................
		MAKE_SKILL_SPOT_SRC_AND_LIST(EF_ROAR,EF_ROAR,ESLT_CHARPOS,2.1f,EF_ROARED,ESLT_CHARPOS,2.1f);
		//..............................................................................................................
		// 무기를 땅에 꽂아서 광역
		//..............................................................................................................
		MAKE_SKILL_SPOT_SRC_AND_LIST(EF_LAND_PUT_STICK,EF_LAND_PUT_STICK,ESLT_CHARPOS,2.1f,EF_LAND_PUT_STICKED,ESLT_CHARPOS,4.1f);
		//..............................................................................................................
		// 체인 라이트닝
		//..............................................................................................................
		MAKE_SKILL_SPOT_SRC_AND_LIST(EF_CHAIN_LIGHTNING,EF_CHAIN_LIGHTNING,ESLT_CHARPOS,2.1f,EF_CHAIN_LIGHTNING_ED,ESLT_CHARPOS,4.1f);
		//..............................................................................................................
		// 초음파 0
		//..............................................................................................................
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_NPC_SOUND_PAR0,EF_NPC_SOUND_PAR0,ESLT_CHARPOS,2.1f,EF_NPC_SOUND_PAR0_ED,ESLT_CHARPOS,2.1f);
		//..............................................................................................................
		// 초음파 1
		//..............................................................................................................
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_NPC_SOUND_PAR1,EF_NPC_SOUND_PAR1,ESLT_CHARPOS,2.1f,EF_NPC_SOUND_PAR1_ED,ESLT_CHARPOS,2.1f);
		//..............................................................................................................
		// 초음파 2
		//..............................................................................................................
		MAKE_SKILL_SPOT_SRC_AND_LIST(EF_NPC_SOUND_PAR2,EF_NPC_SOUND_PAR2,ESLT_CHARPOS,2.1f,EF_NPC_SOUND_PAR2_ED,ESLT_CHARPOS,2.1f);
		//..............................................................................................................
		// 야생의 포효
		//..............................................................................................................
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_NPC_NATURAL_CRY,EF_NPC_NATURAL_CRY,ESLT_CHARPOS,2.1f,EF_NPC_NATURAL_CRYED,ESLT_CHARPOS,2.1f);
		//..............................................................................................................
		// 분노의 일격
		//..............................................................................................................
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_NPC_ONE_CUT,EF_NPC_ONE_CUT,ESLT_CHARPOS,2.1f,EF_NPC_ONE_CUTED,ESLT_CHARPOS,2.1f);
		//..............................................................................................................
		// 망치 일격
		//..............................................................................................................
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_NPC_HAMER_CRASH,EF_NPC_HAMER_CRASH,ESLT_CHARPOS,2.1f,EF_NPC_HAMER_CRASH_ED,ESLT_CHARPOS,2.1f);
		//..............................................................................................................
		// 펠릭스의 저주
		//..............................................................................................................
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_PEL_CURSE,EF_PEL_CURSE,ESLT_CHARPOS,2.1f,EF_PEL_CURSE_ED,ESLT_CHARPOS,2.1f);
		//..............................................................................................................
		// 전기 충격
		//..............................................................................................................
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_ELECT_CRASH,EF_ELECT_CRASH,ESLT_CHARPOS,2.1f,EF_ELECT_CRASH_ED,ESLT_CHARPOS,2.1f);
		//..............................................................................................................
		// 포효 - 능력상승(자기한테 거는거)
		//..............................................................................................................
		MAKE_SKILL_SPOT_SRC(EF_NPC_CRY_UP,EF_NPC_CRY_UP,ESLT_CHARPOS,2.1f);
		//..............................................................................................................
		// 점프후 깔고 앉기
		//..............................................................................................................
		MAKE_SKILL_SPOT_SRC_AND_LIST(EF_NPC_JUMP_ATTACK,EF_NPC_JUMP_ATTACK,ESLT_CHARPOS,2.1f,EF_NPC_JUMP_ATTACKED,ESLT_CHARPOS,2.1f);
		//..............................................................................................................
		// 기둥던지기
		//..............................................................................................................
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_THROW_STICK,EF_THROW_STICK,ESLT_CHARPOS,2.1f,EF_THROW_STICKED,ESLT_CHARPOS,4.1f);
		//..............................................................................................................
		// 마라 어퍼~(필)
		//..............................................................................................................
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_NPC_UPPER_CUT,EF_NPC_UPPER_CUT,ESLT_CHARPOS,2.1f,EF_NPC_UPPER_CUTED,ESLT_CHARPOS,2.1f);
		//..............................................................................................................
		// 견고화 ( 자기한테 )
		//..............................................................................................................
		MAKE_SKILL_SPOT_SRC(EF_NPC_STRONY,EF_NPC_STRONY,ESLT_CHARPOS,2.1f);
		//..............................................................................................................
		// 전투태세(자기한테)
		//..............................................................................................................
		MAKE_SKILL_SPOT_SRC(EF_FIGHT_STAND,EF_FIGHT_STAND,ESLT_CHARPOS,2.1f);
		//..............................................................................................................
		// 광역 잠재우기
		//..............................................................................................................
		MAKE_SKILL_SPOT_SRC_AND_LIST(EF_LAND_SLEEP,EF_LAND_SLEEP,ESLT_CHARPOS,2.1f,EF_LAND_SLEEP_ED,ESLT_CHARPOS,2.1f);
		//..............................................................................................................
		// 개구리 노래
		//..............................................................................................................
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_FROG_SONG,EF_FROG_SONG,ESLT_CHARPOS,2.1f,EF_FROG_SONG_ED,ESLT_CHARPOS,2.1f);
		//..............................................................................................................
		// 3단 연속베기
		//..............................................................................................................
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_NPC_SKILL_CUT,EF_NPC_SKILL_CUT,ESLT_CHARPOS,2.1f,EF_NPC_SKILL_CUT_ED,ESLT_CHARPOS,3.1f);
		//..............................................................................................................
		// 점프 후 바닥 내리쳐서 광역 공격
		//..............................................................................................................
		MAKE_SKILL_SPOT_SRC_AND_LIST(EF_JUMP_HIT_HAND_LAND,EF_JUMP_HIT_HAND_LAND,ESLT_CHARPOS,2.1f,EF_JUMP_HIT_HAND_LANDED,ESLT_CHARPOS,2.1f);
		//..............................................................................................................
		// 방패 가격후 3단 찌르기
		//..............................................................................................................
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_NPC_TWO_HIT,EF_NPC_TWO_HIT,ESLT_CHARPOS,2.1f,EF_NPC_TWO_HITED,ESLT_CHARPOS,2.1f);
		//..............................................................................................................
		// 버블 힐
		//..............................................................................................................
		MAKE_EFFECT(EF_HEAL_BUBBLE,EF_HEAL_BUBBLE);						
		//..............................................................................................................
		// 왕방울 공격 ( NPC )
		//..............................................................................................................
		MAKE_EFFECT(EF_BIG_BUBBLE,EF_BIG_BUBBLE);
		//..............................................................................................................
		// SP 흡수
		//..............................................................................................................
		MAKE_EFFECT(EF_SP_SUCTION,EF_SP_SUCTION);
		//..............................................................................................................
		// HP 흡수
		//..............................................................................................................
		MAKE_EFFECT(EF_HP_SUCTION,EF_HP_SUCTION);
		//..............................................................................................................
		// 차가운 입김
		//..............................................................................................................
		MAKE_EFFECT(EF_ICE_RIP_BLOW,EF_ICE_RIP_BLOW);
		//..............................................................................................................
		// 석화
		//..............................................................................................................
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_STONY_LINE,EF_STONY_LINE,ESLT_CHARPOS,3.1f,EF_STONY_LINE_ED,ESLT_CHARPOS,3.1f);
		//..............................................................................................................
		// 아이빔
		//..............................................................................................................
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_EYE_BEAM,EF_EYE_BEAM,ESLT_CHARPOS,3.1f,EF_EYE_BEAMED,ESLT_CHARPOS,3.1f);
		//..............................................................................................................
		// 아이빔 ( 360 )
		//..............................................................................................................
		MAKE_SKILL_SPOT_SRC_AND_LIST(EF_EYE_BEAM_LAND,EF_EYE_BEAM_LAND,ESLT_CHARPOS,3.1f,EF_EYE_BEAM_LAND_ED,ESLT_CHARPOS,3.1f);
		//..............................................................................................................
		// 흡혈
		//..............................................................................................................
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_NPC_AB_HPMP,EF_NPC_AB_HPMP,ESLT_CHARPOS,3.1f,EF_NPC_AB_HPMP_ED,ESLT_CHARPOS,3.1f);
		//..............................................................................................................
		// 무력화
		//..............................................................................................................
		MAKE_EFFECT(EF_MAGIC_DELETE,EF_MAGIC_DELETE);
		
		//..............................................................................................................
		// 대검 공격
		//..............................................................................................................
		MAKE_EFFECT(EF_BIG_SWORD,EF_BIG_SWORD);
		MAKE_EFFECT(EF_BIG_SWORD0_ED,EF_BIG_SWORD0_ED);
		MAKE_EFFECT(EF_BIG_SWORD1_ED,EF_BIG_SWORD1_ED);
		MAKE_EFFECT(EF_BIG_SWORD2_ED,EF_BIG_SWORD2_ED);
		
		//..............................................................................................................
		// 기적 스킬 ( 나이트 )
		//..............................................................................................................
		MAKE_EFFECT(EF_MIRACLE,EF_MIRACLE);
		MAKE_EFFECT(EF_MIRACLE_ED,EF_SRC_LINK);
		
		//..............................................................................................................
		// 거대몬스터 기본 공격
		//..............................................................................................................
		MAKE_EFFECT(EF_BIG_NPC_ATTACK0,EF_BIG_NPC_ATTACK0);			// 기본 공격
		MAKE_EFFECT(EF_BIG_NPC_ATTACK0ED,EF_BIG_NPC_ATTACK0ED);		// 기본 공격 데미지
		
		//..............................................................................................................
		// 거대몬스터 나타날때
		//..............................................................................................................
		MAKE_EFFECT(EF_BIG_NPC_APPEAR0,EF_BIG_NPC_APPEAR0);			// 얼굴에 붙여보아요
		
		//..............................................................................................................
		// 거대 몬스터 스킬 공격 
		//..............................................................................................................
		MAKE_EFFECT(EF_BIG_NPC_SKILL0,EF_BIG_NPC_SKILL0);			// 용
		MAKE_EFFECT(EF_BIG_NPC_SKILL1,EF_BIG_NPC_SKILL0);			// 건물
        
		//..............................................................................................................
		// 날개 테스트
		//..............................................................................................................
		MAKE_EFFECT(EF_WING0,EF_WING0);								// 날개 테스트
		
		//..............................................................................................................
		// 깃발
		//..............................................................................................................
		MAKE_EFFECT(EF_EMBLEM_FLAG,EF_EMBLEM_FLAG);
		
		//..............................................................................................................
		// 피의 폭주
		//..............................................................................................................
		MAKE_EFFECT(EF_BLOOD_RUNAWAY1,EF_SRC_LINK);
		
		//..............................................................................................................
		// 불굴의 의지
		//..............................................................................................................
		MAKE_EFFECT(EF_LIVE_MIND1,EF_SRC_LINK);
		
		//..............................................................................................................
		// 불꽃
		//..............................................................................................................
		MAKE_EFFECT(EF_FIREFLOWER0,EF_FIREFLOWER0);
		MAKE_EFFECT(EF_FIREFLOWER1,EF_FIREFLOWER0);
		
		MAKE_EFFECT(EF_SI_ITEM_ATTACK0,EF_SI_ITEM_ATTACK0); // 뿅망치 공격
		MAKE_EFFECT(EF_SI_ITEM_ATTACK1,EF_SI_ITEM_ATTACK1); // 지적봉 공격
		MAKE_EFFECT(EF_SI_ITEM_ATTACK2,EF_SI_ITEM_ATTACK2); // 하리센 공격
		MAKE_EFFECT(EF_SI_ITEM_ATTACK3,EF_SI_ITEM_ATTACK3); // 물고기 공격
		
		MAKE_EFFECT(EF_SI_ITEM_ATTACKED0,EF_SI_ITEM_ATTACKED0); // 뿅망치 공격
		MAKE_EFFECT(EF_SI_ITEM_ATTACKED1,EF_SI_ITEM_ATTACKED1); // 지적봉 공격
		MAKE_EFFECT(EF_SI_ITEM_ATTACKED2,EF_SI_ITEM_ATTACKED2); // 하리센 공격
		MAKE_EFFECT(EF_SI_ITEM_ATTACKED3,EF_SI_ITEM_ATTACKED3); // 물고기 공격
		
		//..............................................................................................................
		// 신수 관련
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
		// 방어 NPC
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
		// NPC 스킬 ( 수정요 )
		//........................................................................................................
		// 네로쉬칸 가스 발사
		MAKE_SKILL_FLY(EF_NPC_NERO_GAS,
			324,CEFCommonFlySkill::CommonFlySkill_Effect_Type::BILLBOARD_LINK,
			325,CEFCommonFlySkill::CommonFlySkill_Effect_Type::PARTICLE_SPOT,
			EF_NPC_NERO_GASED,70,1);
		
		// 네로쉬칸 견고화
		MAKE_SKILL_SPOT_SRC(EF_NPC_NERO_STRONG,EF_NPC_NERO_STRONGED,ESLT_CHARPOS,4.1f);
		
		// 비즈XP-900 전투태세
		MAKE_SKILL_SPOT_SRC(EF_NPC_BIZ_AMOTION,EF_NPC_BIZ_AMOTIONED,ESLT_CHARPOS,4.1f);
		
		// 네로쉬칸 죽음의 일격
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_NPC_NERO_DIE_PUNCH,EF_NPC_NERO_DIE_PUNCH,ESLT_CHARPOS,4.1f,EF_NPC_NERO_DIE_PUNCHED,ESLT_CHARPOS,4.1f);
		
		// 하르피아 무력화
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_NPC_HARU_MU,EF_NPC_HARU_MU,ESLT_CHARPOS,4.1f,EF_NPC_HARU_MUED,ESLT_CHARPOS,4.1f);
		
		// 하르피아 HP 흡수
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_NPC_HARU_HP,EF_NPC_HARU_HP,ESLT_CHARPOS,4.1f,EF_NPC_HARU_HPED,ESLT_CHARPOS,4.1f);
		
		// 하르피아 멀티 라이트닝
		MAKE_SKILL_SPOT_SRC_AND_LIST(EF_NPC_HARU_LIGHT,EF_NPC_HARU_LIGHT,ESLT_CHARPOS,4.1f,EF_NPC_HARU_LIGHTED,ESLT_CHARPOS,4.1f);
		
		// 비즈XP-300 전기충격
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_NPC_BIZ_ELEC,EF_NPC_BIZ_ELEC,ESLT_CHARPOS,4.1f,EF_NPC_BIZ_ELECED,ESLT_CHARPOS,4.1f);
		
		// 비스카그 3단 베기
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_NPC_BIS_THREE,EF_NPC_BIS_THREE,ESLT_CHARPOS,4.1f,EF_NPC_BIS_THREEED,ESLT_CHARPOS,4.1f);
		
		// 비스카그 점프 광역
		MAKE_SKILL_SPOT_SRC_AND_LIST(EF_NPC_BIS_JUMP_AT,EF_NPC_BIS_JUMP_AT,ESLT_CHARPOS,4.1f,EF_NPC_BIS_JUMP_ATED,ESLT_CHARPOS,4.1f);
		
		// 비스카그 광역 공격
		MAKE_SKILL_SPOT_SRC_AND_LIST(EF_NPC_BIS_LAND_AT,EF_NPC_BIS_LAND_AT,ESLT_CHARPOS,4.1f,EF_NPC_BIS_LAND_ATED,ESLT_CHARPOS,4.1f);
		
		// 비즈XP-610 아이 빔
		MAKE_EFFECT(EF_NPC_BIZ_BEAM,EF_NPC_BIZ_BEAM);
		
		MAKE_SPOT_EFFECT_DEST(EF_FIREBALL_ATTACKED4,EF_FIREBALL_ATTACKED4,ESLT_CHARPOS,4.1f,0.0f);
		
		// 신속의대검
		MAKE_SKILL_SPOT_SRC(EF_FAST_SWORD,EF_FAST_SWORD,ESLT_CHARPOS,4.1f);
		// 회피
		MAKE_SKILL_SPOT_SRC(EF_FAST_AWAY,EF_FAST_AWAY,ESLT_CHARPOS,4.1f);
		// 신속의 화살 ( 프로그램에서 처리 )
		MAKE_EFFECT(EF_FAST_BOW,EF_FAST_BOW);
		
		MAKE_SPOT_EFFECT(EF_MN_POINT_DEAD,EF_MN_POINT_DEAD,ESLT_CHARPOS,3.1f,0.0f);
		MAKE_SPOT_EFFECT(EF_RN_POINT_EDAD,EF_RN_POINT_EDAD,ESLT_CHARPOS,3.1f,0.0f);
		
		// 그리즐리 기본공격
		MAKE_EFFECT(EF_GRIZZL_BAS_ATT,EF_GRIZZL_BAS_ATT);
		MAKE_EFFECT(EF_F_GRIZZL_BAS_ATT,EF_GRIZZL_BAS_ATT);
		MAKE_EFFECT(EF_M_GRIZZL_BAS_ATT,EF_GRIZZL_BAS_ATT);	
		// 그리즐리 기본데미지 약, 강
		MAKE_EFFECT(EF_GRIZZL_BAS_ATTED_0,EF_GRIZZL_BAS_ATTED_0);
		MAKE_EFFECT(EF_F_GRIZZL_BAS_ATTED_0,EF_GRIZZL_BAS_ATTED_0);
		MAKE_EFFECT(EF_M_GRIZZL_BAS_ATTED_0,EF_GRIZZL_BAS_ATTED_0);
		MAKE_EFFECT(EF_GRIZZL_BAS_ATTED_2,EF_GRIZZL_BAS_ATTED_2);
		MAKE_EFFECT(EF_F_GRIZZL_BAS_ATTED_2,EF_GRIZZL_BAS_ATTED_2);
		MAKE_EFFECT(EF_M_GRIZZL_BAS_ATTED_2,EF_GRIZZL_BAS_ATTED_2);
		
		// 키메라 엔트 기본공격
		MAKE_EFFECT(EF_CHIME_ANT_BAS_ATT,EF_CHIME_ANT_BAS_ATT);
		// 키메라 엔트 기본데미지 약, 강
		MAKE_EFFECT(EF_CHIME_ANT_BAS_ATTED_0,EF_CHIME_ANT_BAS_ATTED_0);
		MAKE_EFFECT(EF_CHIME_ANT_BAS_ATTED_2,EF_CHIME_ANT_BAS_ATTED_2);
		
		// 그리즐리의 노래 
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_SONG_GRIZZL, EF_SONG_GRIZZL, ESLT_CHARPOS, 2.1f, EF_SONG_GRIZZL_ED, ESLT_CHARPOS, 2.1f);
		// 그리즐리의 일격
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_HIT_GRIZZL, EF_HIT_GRIZZL, ESLT_CHARPOS, 2.1f, EF_HIT_GRIZZL_ED, ESLT_CHARPOS, 2.1f);
		// 그리즐리의 회복
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_RECOVER_GRIZZL, EF_RECOVER_GRIZZL, ESLT_CHARPOS, 2.1f, EF_RECOVER_GRIZZL_ED, ESLT_CHARPOS, 2.1f);
		// 키메라 엔트의 광역 공격
		MAKE_SKILL_SPOT_SRC_AND_LIST(EF_BIG_SHOCK_C_ANT,EF_BIG_SHOCK_C_ANT,ESLT_CHARPOS,4.1f,EF_BIG_SHOCK_C_ANT_ED,ESLT_CHARPOS,4.1f);
		
		MAKE_EFFECT(EF_DEFENSE_PET_BMOTION,EF_DEFENSE_PET_BMOTION);
		
		//포포이 연타공격
		MAKE_EFFECT(EF_POPOI_ATTACK_0, EF_POPOI_ATTACK);
		MAKE_EFFECT(EF_POPOI_ATTACK_1, EF_POPOI_ATTACK);
		MAKE_EFFECT(EF_POPOI_ATTACK_2, EF_POPOI_ATTACK);
		
		//에그빌 힐1 - 일반 힐	
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_EGGBIL_HILL0, -1, 0, 2.1f, EF_EGGBIL_HILLING, ESLT_CHARPOS, 2.1f);
		//에그빌 힐2 - 3단계부터의 힐
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_EGGBIL_HILL1, EF_EGGBIL_HILLMOTION , 0, 2.1f, EF_EGGBIL_HILLING, ESLT_CHARPOS, 2.1f);
		
		//피나포어 오믈렛
		MAKE_EFFECT(EF_PINAPOA_HEART,EF_RECOVERY_ITEM1_0);		
		
		// 호랑이 기본공격
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_TIGER_ATTACKED_1, -1, 0, 2.1f, EF_TIGER_ATTACKED_1, ESLT_CHARPOS, 2.1f);
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_TIGER_ATTACKED_2, -1, 0, 2.1f, EF_TIGER_ATTACKED_2, ESLT_CHARPOS, 2.1f);
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_TIGER_ATTACKED_3, -1, 0, 2.1f, EF_TIGER_ATTACKED_3, ESLT_CHARPOS, 2.1f);
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_TIGER_ATTACKED_4, -1, 0, 2.1f, EF_TIGER_ATTACKED_4, ESLT_CHARPOS, 2.1f);
		
		// 호랑이 스킬
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_TIGER_SKILL_2, -1, 0, 2.1f, EF_TIGER_SKILL_2, ESLT_CHARPOS, 2.1f);
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_TIGER_SKILL_3, -1, 0, 2.1f, EF_TIGER_SKILL_3, ESLT_CHARPOS, 2.1f);
		
		// 호랑이 최종형태 연속공격스킬
		MAKE_EFFECT(EF_TIGER_SKILL_4, EF_TIGET_ATTACK);
		
		//사막여우 기본방어1
		MAKE_DEFENCE_PET_MOTION(EF_FOX_DEFENSED_1, 1);
		//사막여우 기본방어2
		MAKE_DEFENCE_PET_MOTION(EF_FOX_DEFENSED_2, 1);
		//사막여우 기본방어3
		MAKE_DEFENCE_PET_MOTION(EF_FOX_DEFENSED_3, 1);
		//사막여우 기본방어4
		MAKE_DEFENCE_PET_MOTION(EF_FOX_DEFENSED_4, 1);
		
		//사막여우 스킬2
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_FOX_SKILL_1, -1, 0, 2.1f, EF_FOX_SKILL_1, ESLT_CHARPOS, 2.1f);
		//사막여우 스킬3
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_FOX_SKILL_2, -1, 0, 2.1f, EF_FOX_SKILL_2, ESLT_CHARPOS, 2.1f);
		//사막여우 스킬4
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_FOX_SKILL_3, -1, 0, 2.1f, EF_FOX_SKILL_3, ESLT_CHARPOS, 2.1f);
		
		//아이스 필드
		MAKE_EFFECT(EF_ICE_FIELD,EF_ICE_FIELD);
		//아이스 스피어
		MAKE_EFFECT(EF_ICE_SPEAR,EF_ICE_SPEAR);
		
		//라이트 스톰
		MAKE_EFFECT(EF_LIGHT_STORM,EF_LIGHT_STORM);
		MAKE_SKILL_SPOT_SRC(EF_LIGHT_STORMED,EF_LIGHTNING_ATTACKED2,ESLT_CHARPOS,5.1f);
		
		//운석의 비
		MAKE_EFFECT(EF_STONE_RAIN,EF_STONE_RAIN);
		MAKE_SKILL_SPOT_SRC(EF_STONE_RAINED,EF_STONE_RAINED,ESLT_CHARPOS,5.1f);
		
		//윈드 버쉬
		MAKE_EFFECT(EF_WIND_BUSH,EF_WIND_BUSH);
		MAKE_SKILL_SPOT_SRC(EF_WIND_BUSHED,EF_WIND_BUSHED,ESLT_CHARPOS,5.1f);
		
		//라운드 파이어	
		MAKE_EFFECT(EF_ROUND_FIRE,EF_ROUND_FIRE);
		//라운드 라이트	
		MAKE_EFFECT(EF_ROUND_LIGHT,EF_ROUND_LIGHT);
		//캐스트 스톤
		MAKE_EFFECT(EF_CAST_STONE,EF_CAST_STONE);
		
		// 대지의 속박
		MAKE_EFFECT(EF_EARTH_BIND,EF_EARTH_BIND);
		// 난사
		MAKE_EFFECT(EF_RANDOM_ARROW,EF_RANDOM_ARROW);		
		
		//사랑의 물약
		MAKE_SKILL_SPOT_SRC(EF_INDONESIA_EVENT,EF_INDONESIA_EVENT,ESLT_CHARPOS,2.1f);
		
		//신속의 검
		MAKE_EFFECT(EF_QUICK_SWORD,EF_CONANISKILL);
		//연속 횡베기
		MAKE_EFFECT(EF_CON_CROSSCUT,EF_CONANISKILL);
		
		//대지의 고통
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_PAIN_EARTH, -1, 0, 2.1f, EF_PAIN_EARTHED, ESLT_CHARPOS, 3.1f);
		
		//열풍강타
		MAKE_EFFECT(EF_FIRESTORM_SMASH,EF_FIRESTORM_SMASH);
		//기열연타
		MAKE_EFFECT(EF_SPIRIT_FIRE_SMACK,EF_SPIRIT_FIRE_SMACK);
		//회전열각
		MAKE_EFFECT(EF_SPIN_FIRE_KICK,EF_CONANISKILL);
		
		///-- 서클 체인지
		MAKE_SKILL_SPOT_SRC(EF_CIRCLE_CHANGE,EF_CIRCLE_SHIELD,ESLT_CHARPOS,3.1f);	
		///-- 복수
		MAKE_EFFECT(EF_REVENGE_SHIELD,EF_SRC_LINK);
		
		///--분노의 검진
		MAKE_EFFECT(EF_WRATH_KNIFE_ATTACK,EF_WRATH_KNIFE_ATTACK);
		///--서클크래쉬
		MAKE_EFFECT(EF_CIRCLE_CRASH,EF_CIRCLE_CRASH);
		///-- 마비의 화살
		MAKE_BEELINEBOW(EF_PARALYSIS_BOW,PT_PARALYSIS_BOW_TAIL,EF_PARALYSIS_BOWED,70,3.1f,ESLT_CHARPOS);
		///-- 독화살
		MAKE_BEELINEBOW(EF_POISON_BOW,PT_POISON_BOW_TAIL,EF_POISON_BOW,70,3.1f,ESLT_CPOS_ICAMERA);
		///-- 삼연각
		MAKE_EFFECT(EF_TREE_CON_KICK,EF_CONANISKILL);
		///-- 슬래쉬 다운
		MAKE_EFFECT(EF_SLASH_DOWN,EF_CONANISKILL);
		///-- 슬래쉬 업
		MAKE_EFFECT(EF_SLASH_UP,EF_CONANISKILL);
		///-- 트리플 카운트
		MAKE_EFFECT(EF_TRIPLE_COUNT,EF_CONANISKILL);
		
		///-- 얼음의 화살비
		MAKE_EFFECT(EF_ICE_BOW_RAIN,EF_ICE_BOW_RAIN);
		
		///-- 은신 감지 
		MAKE_EFFECT(EF_HIDE_SENSOR , EF_HIDE_SENSOR);
		
		//분노 폭발
		MAKE_EFFECT(EF_WRATH_BOMB , EF_WRATH_BOMB );
		//신성한 폭발
		MAKE_EFFECT(EF_HOLY_BOMB_BALL , EF_HOLY_BOMB );	
		//어둠의 폭발
		MAKE_EFFECT(EF_DARK_BOMB_BALL , EF_DARK_BOMB );	
		//장갑 파괴 
		MAKE_EFFECT(EF_GLOVE_DESTRUCTION ,EF_CONANISKILL);
		//다크니스 
		MAKE_EFFECT(EF_ONE_DARKNESS,EF_ONE_DARKNESS);		
		//트리플 다크니스
		MAKE_EFFECT(EF_TRIPLE_DARKNESS,EF_TRIPLE_DARKNESS);	
		//트리플 파이어
		MAKE_EFFECT(EF_TRIPLE_FIRE,EF_TRIPLE_FIRE);
		
		///-- 거대유닛 관련 링크 이펙트
		MAKE_EFFECT(EF_COLONYCORE_L1,EF_GIANTUNIT_SRC_LINK);
		MAKE_EFFECT(EF_COLONYSUBCORE_L1,EF_GIANTUNIT_SRC_LINK);
		MAKE_EFFECT(EF_COLONYCORE_L2,EF_GIANTUNIT_SRC_LINK);
		MAKE_EFFECT(EF_COLONYSUBCORE_L2,EF_GIANTUNIT_SRC_LINK);
		
		///-- 가드타워 공격이펙트
		MAKE_GTOWEREFFECT(EF_COLONYGTOWER_L1_SKILL, 1, 0, PT_FIREBALL_TAIL, EF_BOW_ATTACKED_2, ESLT_SRC_RAD);
		MAKE_GTOWEREFFECT(EF_COLONYGTOWER_L2_SKILL, 1, BB_GT_L2_BALL, PT_FIREBALL_TAIL, EF_COLONYGTOWER_L2_SKILLED, ESLT_CHARPOS);
		MAKE_GTOWEREFFECT(EF_COLONYGTOWER_L3_SKILL, 1, BB_GT_L3_BEAM, PT_VIOLET_TAIL , EF_COLONYGTOWER_L3_SKILLED, ESLT_CHARPOS);
		
		///-- 거대대포 공격이펙트
		MAKE_CANNONEFECT(EF_GUST_PAN_ATTACK, 126, 2, BB_CANNON_BALL, PT_FIREBALL_TAIL, 0, 0, EF_GUST_PAN_ATTACK_PERCUSSION, ESLT_CHARPOS, EF_GUST_PAN_ATTACK_EXPLOSION, ESLT_CHARPOS, EF_GUST_PAN_ATTACKED, ESLT_CHARPOS);
		MAKE_CANNONEFECT(EF_GUST_PAN_SKILL, 126, 2, BB_CANNON_BALL, PT_FIREBALL_TAIL, EF_GUST_PAN_GET_POWER, ESLT_CHARPOS, EF_GUST_PAN_SKILL_PERCUSSION, ESLT_CHARPOS, EF_GUST_PAN_SKILL_EXPLOSION, ESLT_CHARPOS, EF_GUST_PAN_SKILLKED, ESLT_CHARPOS);
		
		///-- 거신병 공격 이펙트
		MAKE_GOLEMEFFECT(EF_GIANT_GOLEM_ATTACK, 99, 164, 0, 0, ESLT_CHARPOS, 0, ESLT_CHARPOS, EF_GIANT_GOLEM_ATTACK_EXPLOSION, ESLT_CHARPOS, EF_GIANT_GOLEM_ATTACKED, ESLT_CHARPOS);
		MAKE_GOLEMEFFECT(EF_GIANT_GOLEM_SKILL, 99, 164, PT_GGOLEM_SKILLHAND_TAIL1, EF_GIANT_GOLEM_RIGHT_HAND, ESLT_CHARPOS, EF_GIANT_GOLEM_LEFT_HAND, ESLT_CHARPOS, EF_GIANT_GOLEM_SKILL_EXPLOSION, ESLT_CHARPOS, EF_GIANT_GOLEM_SKILLED, ESLT_CHARPOS);
		
		///-- 신성한 고통
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_PAIN_HOLLY, -1, 0, 2.1f, EF_PAIN_HOLLYED, ESLT_CHARPOS, 3.1f);	
		
		///-- 마인의 권술
		MAKE_EFFECT(EF_BUNCH_STRENGTH,EF_BUNCH_STRENGTH);
		
		//본 머더 도검류 - 기본공격 당했을때
		MAKE_STAND_ATTACK_DEST(EF_B_M_BASE_ATTACK,-1,0,0,-1,0,0);		
		//본 머더 병사 - 찌르기
		MAKE_EFFECT(EF_B_M_SOLDIER_THRUST, EF_BONWARSKILL);
		//본 머더 병사 - 급소베기
		MAKE_EFFECT(EF_B_M_SOLDIER_WEAK_CUT, EF_BONWARSKILL);
		//본 머더 병사 - 찔러베기
		MAKE_EFFECT(EF_B_M_SOLDIER_THRUST_CUT, EF_BONWARSKILL);
		//본 머더 전사 - 회전베기
		MAKE_EFFECT(EF_B_M_WARRIOR_TURN_CUT, EF_BONWARSKILL);
		//본 머더 전사 - X베기1
		MAKE_EFFECT(EF_B_M_WARRIOR_X1_CUT, EF_BONWARSKILL);
		//본 머더 전사 - X베기2
		MAKE_EFFECT(EF_B_M_WARRIOR_X2_CUT, EF_BONWARSKILL);
		//본 머더 근위병 - 리저렉션
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_B_M_GUARD_RESURRECTION, EF_REVIVAL, 0, 3.1f, -1, 0, 3.1f);
		//본 머더 보스 - 화염폭풍
		MAKE_EFFECT(EF_B_M_BOSS_FIRE_STORM, EF_BONWARSKILL);
		
		//본 아처류 - 기본공격 당했을때
		MAKE_EFFECT(EF_B_ARCHER_BASE_ATTACK, EF_BONARCSKILL);
		//불타버린 본 아처 - 불화살
		MAKE_EFFECT(EF_B_ARCHER_FIRE_ARROW, EF_BONARCSKILL);		
		//본 아처 치료사 - 응급치료 이펙트
		MAKE_SKILL_SPOT_SRC_AND_DEST(EF_B_ARCHER_FIRST_AID, EF_B_ARCHER_FIRST_AID, 0, 3.1f, -1, 0, 3.1f);
		//부패한 본아처 - 부패의 화살
		MAKE_EFFECT(EF_B_ARCHER_DECAY_ARROW, EF_BONARCSKILL);
		//역병걸린 본아처 - 고통의 화살
		MAKE_EFFECT(EF_B_ARCHER_PAIN_ARROW, EF_BONARCSKILL);
		//굶주린 본아처 - 암흑의 화살
		MAKE_EFFECT(EF_B_ARCHER_DARK_ARROW, EF_BONARCSKILL);
		//저주받은 본아처 - 저주의 화살
		MAKE_EFFECT(EF_B_ARCHER_CURSE_ARROW, EF_BONARCSKILL);
		//광포한 본아처 - 마비의 화살
		MAKE_EFFECT(EF_B_ARCHER_PARALYSIS_ARROW, EF_BONARCSKILL);
		//본아처 보스 - 난사
		MAKE_EFFECT(EF_B_BOS_ARCHER_RANDOM_ARROW, EF_BONARCSKILL);
		
		//본 자헤딘 - 광폭의 검술
		MAKE_EFFECT(EF_B_JAHE_CRAZY_FEN_BEFORE, EF_BONWARSKILL);
		//스컬베인 - 암흑폭풍
		MAKE_EFFECT(EF_B_SKEL_BAIN_DARK_STORM, EF_BONWARSKILL);	
		
		// 애니적용 날개
		MAKE_EFFECT(EF_ANIWING0,EF_ANIWING0);		
		
		

		//
	//	MAKE_SKILL_SPOT_SRC_AND_DEST(EF_RUGAL_BASIC_ATTACK,EF_RUGAL_BASIC_ATTACK,ESLT_CHARPOS,2.1f,EF_RUGAL_BASIC_ATTACKED,ESLT_CHARPOS,2.1f);
	//	MAKE_SKILL_SPOT_SRC_AND_DEST(EF_RUGAL_CRI_ATTACK,EF_RUGAL_CRI_ATTACK,ESLT_CHARPOS,2.1f,EF_RUGAL_CRI_ATTACKED,ESLT_CHARPOS,2.1f);

		//중국 - 선제공격자 표시 이펙트
		MAKE_EFFECT(EF_BEFORE_ATTACKER_MARK, EF_SRC_LINK );		
		
		//하트모양 불꽃
		MAKE_EFFECT(EF_FIREHEARTFLOWER,EF_FIREFLOWER0);		
		//아이러브유 불꽃
		MAKE_EFFECT(EF_FIREENGLOVEFLOWER,EF_FIREFLOWER0);
		//사랑해 불꽃
		MAKE_EFFECT(EF_FIRENATLOVEFLOWER,EF_FIREFLOWER0);		
		
		//클래스 지존아이템 - 연결 이펙트 - 프로미나스
		MAKE_EFFECT( EF_SPECIAL_ITEM_EFFECT_PROMINAS, EF_SPECIAL_ITEM_EFFECT );
		//클래스 지존아이템 - 연결 이펙트 - 싸이키
		MAKE_EFFECT( EF_SPECIAL_ITEM_EFFECT_PSYCHE, EF_SPECIAL_ITEM_EFFECT );
		//클래스 지존아이템 - 연결 이펙트 - 머시너리
		MAKE_EFFECT( EF_SPECIAL_ITEM_EFFECT_MERCENARY, EF_SPECIAL_ITEM_EFFECT );
		//클래스 지존아이템 - 연결 이펙트 - 나이트
		MAKE_EFFECT( EF_SPECIAL_ITEM_EFFECT_KNIGHT, EF_SPECIAL_ITEM_EFFECT );
		//클래스 지존아이템 - 연결 이펙트 - 슬레이어
		MAKE_EFFECT( EF_SPECIAL_ITEM_EFFECT_SLAYER, EF_SPECIAL_ITEM_EFFECT );
		//클래스 지존아이템 - 연결 이펙트 - 스나이퍼
		MAKE_EFFECT( EF_SPECIAL_ITEM_EFFECT_SNIPER, EF_SPECIAL_ITEM_EFFECT );
		//클래스 지존아이템 - 연결 이펙트 - 프리스트
		MAKE_EFFECT( EF_SPECIAL_ITEM_EFFECT_PRIEST, EF_SPECIAL_ITEM_EFFECT );
		//클래스 지존아이템 - 연결 이펙트 - 홀리어벤져
		MAKE_EFFECT( EF_SPECIAL_ITEM_EFFECT_HOLY, EF_SPECIAL_ITEM_EFFECT );

		//클래스 지존아이템 - 연결 이펙트 - 프로미나스
		MAKE_EFFECT( EF_BASE_ITEM_EFFECT_PROMINAS, EF_BASE_ITEM_EFFECT );
		MAKE_EFFECT( EF_BASE_ITEM_EFFECT_PSYCHE, EF_BASE_ITEM_EFFECT );
		MAKE_EFFECT( EF_BASE_ITEM_EFFECT_MERCENARY, EF_BASE_ITEM_EFFECT );
		MAKE_EFFECT( EF_BASE_ITEM_EFFECT_KNIGHT, EF_BASE_ITEM_EFFECT );
		MAKE_EFFECT( EF_BASE_ITEM_EFFECT_SLAYER, EF_BASE_ITEM_EFFECT );
		MAKE_EFFECT( EF_BASE_ITEM_EFFECT_SNIPER, EF_BASE_ITEM_EFFECT );
		MAKE_EFFECT( EF_BASE_ITEM_EFFECT_PRIEST, EF_BASE_ITEM_EFFECT );
		MAKE_EFFECT( EF_BASE_ITEM_EFFECT_HOLY, EF_BASE_ITEM_EFFECT );



		///도그마 베인 - 1단계 기본공격  
		MAKE_EFFECT( EF_DOGMA_L1_BASIC_ATTACK, EF_DOGMAEFFECT );	
		///도그마 베인 - 1단계 파이어레인
		MAKE_EFFECT( EF_DOGMA_L1_FIRERAIN, EF_DOGMAEFFECT );
		///도그마 베인 - 1단계 프레임스톰
		MAKE_EFFECT( EF_DOGMA_L1_FRAME_STORM, EF_DOGMAEFFECT );

		///도그마 베인 - 2단계 기본공격  
		MAKE_EFFECT( EF_DOGMA_L2_BASIC_ATTACK, EF_DOGMAEFFECT );	
		///도그마 베인 - 2단계 분노의 일격
		MAKE_EFFECT( EF_DOGMA_L2_WRATH_ONECUT, EF_DOGMAEFFECT );
		///도그마 베인 - 2단계 검진
		MAKE_EFFECT( EF_DOGMA_L2_SWORD_FLASH, EF_DOGMAEFFECT );

		///도그마 베인 - 3단계 기본공격  
		MAKE_EFFECT( EF_DOGMA_L3_BASIC_ATTACK, EF_DOGMAEFFECT );	
		///도그마 베인 - 3단계 발구르기
		MAKE_EFFECT( EF_DOGMA_L3_FOOT_QUAKE, EF_DOGMAEFFECT );
		///도그마 베인 - 3단계 분노의 일격2
		MAKE_EFFECT( EF_DOGMA_L3_WRATH_ONECUT2, EF_DOGMAEFFECT );

		///도그마 베인 - 4단계 대원참
		MAKE_EFFECT( EF_DOGMA_L4_CIRCLE_SWORD, EF_DOGMAEFFECT );

		///도그마 베인 - 4단계 외형 이펙트
		MAKE_EFFECT( EF_MASTER_DOGMA_APPEAR_EFFECT, EF_DOGMAEFFECT );		

	
		//루갈 기본 공격 
		MAKE_EFFECT(EF_RUGAL_BASIC_ATTACK , EF_RUGALEFFECT );

		//솔다트 기본공격 
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
	



		///바라고스 - 기본공격
		MAKE_EFFECT( EF_BARAGOS_BASIC_ATTACK, EF_BARAGOSEFFECT );
		///바라고스 - 드래곤 브레스(볼)
		MAKE_EFFECT( EF_BARAGOS_DRAGON_BRESS, EF_BARAGOSEFFECT );
		///바라고스 - 악룡의 포효
		MAKE_EFFECT( EF_BARAGOS_HOWL_OF_DEVIL, EF_BARAGOSEFFECT );
		///바라고스 - 선더 브레이크
		MAKE_EFFECT( EF_BARAGOS_THUNDERBREAK, EF_BARAGOSEFFECT );
		///바라고스 - 광폭화
		MAKE_EFFECT( EF_BARAGOS_BERSERK, EF_BARAGOSEFFECT );

		///쟈나드 - 기본공격
		MAKE_EFFECT( EF_JANAD_BASIC_ATTACK, EF_JANADEFFECT );
		///쟈나드 - 돌진 3단 공격
		MAKE_EFFECT( EF_JANAD_DASHTRIPLE_ATTACK, EF_JANADEFFECT );
		///쟈나드 - 마령소환
		MAKE_EFFECT( EF_JANAD_CALLDEVILSOUL, EF_JANADEFFECT );	
		///쟈나드 - 암흑 검풍
		MAKE_EFFECT( EF_JANAD_DARKSWORDSTORM, EF_JANADEFFECT );
		///쟈나드 - 디스펠
		MAKE_EFFECT( EF_JANAD_DISPEL, EF_JANADEFFECT );
		///쟈나드 - 마그마 버스트
		MAKE_EFFECT( EF_JANAD_MAGMA_BUST, EF_JANADEFFECT );
		///쟈나드 - 마황의 검진
		MAKE_EFFECT( EF_JANAD_SWORD_FLASH_OF_DEVIL, EF_JANADEFFECT );
		///쟈나드 - 소환
		MAKE_EFFECT( EF_JANAD_CALLDEVILSOULED, EF_JANADEFFECT );


		//우로보스 기본공격 
		MAKE_EFFECT ( EF_UROBOROS_BASIC_ATTACK ,  EF_UROBOROSEFFECT );
		MAKE_EFFECT( EF_UROBOROS_POISON ,  EF_UROBOROSEFFECT );
		MAKE_EFFECT( EF_UROBOROS_BALKAN ,  EF_UROBOROSEFFECT );
		MAKE_SPOT_EFFECT_DEST(EF_UROBOROS_BALKAN_ED , EF_UROBOROS_BALKAN_ED , ESLT_CHARPOS ,3.1f , 0.0f );


		MAKE_EFFECT( EF_UROBOROS_DRAG_HOWLLING ,  EF_UROBOROSEFFECT );
		MAKE_EFFECT( EF_UROBOROS_RUN ,  EF_UROBOROSEFFECT );

		
		MAKE_EFFECT(EF_DONG_TEST_0 , EF_DONGS_TEST);
		//........................................................................................................
		// 스킬 이펙트 없이 능력치만 처리
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
					DebugPrint( "!\r\n--- 이펙트 생성 타입 없음 ---\r\n" );
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
		// 이펙트 지우기 
		//........................................................................................................
		if( (*i)->GetID()       == id &&
			(*i)->GetSrcChar()  == pSrc &&
			(*i)->GetDestChar() == pDest &&
			(*i)->GetSkillCode() == dSkillCode )
		{
			///-- By simwoosung 관련된 모든것을 지운다.
			(*i)->SetDestroy();			
		}
	}
	
	return FALSE;
}

//................................................................................................................
// 셀에 찍는 이펙트 삭제 ( 이펙트 처리 ) By wxywxy 
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
// 하르피아 종류 공격시 손이 다르삼 
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
// 셀에 찍는이펙트 추가 ( 이펙트 처리 ) By wxywxy 
//................................................................................................................
int CEffectManager::PushLandEffectBase(CEffectBase **ppEffectBase)
{
	m_EffectList.push_back( *ppEffectBase );
	
	Character *pSrcChar = (*ppEffectBase)->GetSrcChar();
	/*
	//.......................................................................................................
	// 이펙트 매니저에 스킬 타입및 셀 값을 입력 
	// 광역 마법시 아이디로 사용함 -> Unique , Race , Skill Code , Cell
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
// Delete Effect ( 광역스킬 이펙트 지우기 )
//................................................................................................................
int CEffectManager::Delete(DWORD SkillUnique)
{
	DebugPrint( "-------------Process Delete Land Skill %d----------------\n",SkillUnique);
	
	std::list	<CEffectBase *> ::iterator	i = m_EffectList.begin();
	
	for( ; i != m_EffectList.end() ; )
	{
		//........................................................................................................
		// 이펙트 지우기 
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
// 이펙트 매니저 내에서만 지우기
//................................................................................................................
int CEffectManager::DeleteOnManager(Character *pSrc)
{
	std::list	<CEffectBase *> ::iterator	i = m_EffectList.begin();
	
	for( ; i != m_EffectList.end() ; )
	{
		//........................................................................................................
		// 이펙트 지우기 
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
// 이펙트 매니저 캐릭터 내에 큐도 지우기
//................................................................................................................
int CEffectManager::DeleteNow(Character *pSrc)
{
	std::list	<CEffectBase *> ::iterator	i = m_EffectList.begin();
	
	for( ; i != m_EffectList.end() ; )
	{
		//........................................................................................................
		// 이펙트 지우기 
		//........................................................................................................
		
		if( (*i)->GetSrcChar()  == pSrc ||
			(*i)->GetDestChar() == pSrc  )
		{
			if( (*i) && (*i)->m_ISLandLink == FALSE && 
				(*i)->ISLinkItem() == FALSE )	
			{
				//....................................................................................................
				// ( 캐릭터내 리스트도 같이 지워요 )
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
			///-- 유저형 캐릭터 날개 사라지게 해주기
			else if( !(pSrc->m_bPC) && pSrc->m_State.nCharacterType == nCharacterTypePlayerNPC )
			{
				///--링크 아이템도 지워준다.
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
// 이펙트 SetDestroy();
//................................................................................................................
int CEffectManager::Delete(Character *pSrc)
{
	std::list	<CEffectBase *> ::iterator	i = m_EffectList.begin();
	
	for( ; i != m_EffectList.end() ; ++i )
	{
		//........................................................................................................
		// 이펙트 지우기 
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
// 게이지 전송
//...............................................................................................................
int CEffectManager::MAKE_EF_GAGE_TRANS(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// 데스트 없으면 소스쪽만 이펙트 출력
	if( pSrc == NULL || pDest == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFGageTrans );  					// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// 소스 , 타켓 지정
	
	PushEffect( &pData );												// 메니저에 넣기
	
	return TRUE;
}

//..............................................................................................................
// 신속의 화살
//..............................................................................................................
int CEffectManager::MAKE_EF_FAST_BOW(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// 데스트 없으면 소스쪽만 이펙트 출력
	if( pSrc == NULL || pDest == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFFastBow );						// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// 소스 , 타켓 지정
	
	PushEffect( &pData );												// 메니저에 넣기
	
	return TRUE;
}

//...............................................................................................................
// 	신수 오라
//...............................................................................................................
int CEffectManager::MAKE_EF_HOLY_AURA(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	// 데스트 없으면 소스쪽만 이펙트 출력
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	// 1:N 데미지
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
// 신수 나타날때
//..............................................................................................................
int CEffectManager::MAKE_EF_HOLY_APPEAR(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// 데스트 없으면 소스쪽만 이펙트 출력
	if( pSrc == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFHolyAppear );						// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// 소스 , 타켓 지정
	
	PushEffect( &pData );												// 메니저에 넣기
	
	return TRUE;
}

//..............................................................................................................
// 날개 테스트
//..............................................................................................................
int CEffectManager::MAKE_EF_WING0(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFWing0 );					// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );				// 소스 , 타켓 지정
	
	PushEffect( &pData );										// 메니저에 넣기
	
	return TRUE;
}

//..............................................................................................................
// 애니적용 날개 테스트
//..............................................................................................................
int CEffectManager::MAKE_EF_ANIWING0(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFAniWing0 );				// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );				// 소스 , 타켓 지정
	
	PushEffect( &pData );										// 메니저에 넣기
	
	return TRUE;
}

//..............................................................................................................
// 엠블렘 깃발
//..............................................................................................................
int CEffectManager::MAKE_EF_EMBLEM_FLAG(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFEmblemFlag );				// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );				// 소스 , 타켓 지정
	
	PushEffect( &pData );										// 메니저에 넣기
	
	return TRUE;
}

//..............................................................................................................
// 그리즐리 기본공격
//..............................................................................................................
int CEffectManager::MAKE_EF_GRIZZL_BAS_ATT(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	StandTargetHit( pSrc , pDest , pExtraInfo );
	
	return TRUE;
}

//..............................................................................................................
// 그리즐리 기본데미지 약, 강
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
// 키메라 엔트 기본공격
//..............................................................................................................
int CEffectManager::MAKE_EF_CHIME_ANT_BAS_ATT(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	StandTargetHit( pSrc , pDest , pExtraInfo );
	
	return TRUE;
}

//..............................................................................................................
// 키메라 엔트 기본데미지 약, 강
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
// 불꽃
//..............................................................................................................
int CEffectManager::MAKE_EF_FIREFLOWER0(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFFireWork );				// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );				// 소스 , 타켓 지정
	
	PushEffect( &pData );										// 메니저에 넣기
	
	return TRUE;
}


//................................................................................................................
// 디텍트
//................................................................................................................
int CEffectManager::MAKE_EF_DETECT(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// 데스트 없으면 소스쪽만 이펙트 출력
	if( pSrc == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFDetect );							// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// 소스 , 타켓 지정
	
	PushEffect( &pData );												// 메니저에 넣기
	
	return TRUE;
}


//...............................................................................................................
// 기열파
//...............................................................................................................
int CEffectManager::MAKE_EF_HAND_PAR(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// 데스트 없으면 소스쪽만 이펙트 출력
	if( pSrc == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFHandPar );						// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// 소스 , 타켓 지정
	
	PushEffect( &pData );												// 메니저에 넣기
	
	return TRUE;
}

//..............................................................................................................
// Son Brush
//..............................................................................................................
int CEffectManager::MAKE_EF_SON_BURSH(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// 데스트 없으면 소스쪽만 이펙트 출력
	if( pSrc == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFSonBrush );						// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// 소스 , 타켓 지정
	
	PushEffect( &pData );												// 메니저에 넣기
	
	return TRUE;
}

//................................................................................................................
// 이펙트 매니저 캐릭터 내에 큐도 지우기
//................................................................................................................
int CEffectManager::DeleteOnlySrc(Character *pSrc)
{
	std::list	<CEffectBase *> ::iterator	i = m_EffectList.begin();
	
	for( ; i != m_EffectList.end() ; ++i )
	{
		//........................................................................................................
		// 이펙트 지우기 
		//........................................................................................................
		
		if( (*i)->GetSrcChar() == pSrc )
		{
			// 링크 아이템 체크
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
// 이펙트 생성 함수	
//
//================================================================================================================

//................................................................................................................
// 걷기 먼지
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
// 	방어 태세
//...............................................................................................................
int CEffectManager::MAKE_EF_DEFEND_SETUP(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 2.1f , pSrc->GetUnique() , pSrc->GetIndex() );
	//................................................................................	
	// 스킬 발동 사운드
	//................................................................................	
	if( NULL != pExtraInfo )
		g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	
	return TRUE;
}

//...............................................................................................................
// 	피의 폭주
//...............................................................................................................
int CEffectManager::MAKE_EF_BLOOD_RUNAWAY(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 2.1f , pSrc->GetUnique() , pSrc->GetIndex() );
	
	//................................................................................	
	// 스킬 발동 사운드
	//................................................................................	
	if( NULL != pExtraInfo )
	{
		EffectTargetHit( pSrc , pDest , pExtraInfo );
		
		g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	}
	
	
	return TRUE;
}

//...............................................................................................................
// 	불굴의 의지
//...............................................................................................................
int CEffectManager::MAKE_EF_LIVE_MIND(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 2.1f , pSrc->GetUnique() , pSrc->GetIndex() );
	
	
	//................................................................................	
	// 스킬 발동 사운드
	//................................................................................	
	if( NULL != pExtraInfo )
	{
		g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
		EffectTargetHit( pSrc , pDest , pExtraInfo );
	}
	
	return TRUE;
}

//..............................................................................................................
// 라이트닝 스피어
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
	// 스킬 발동 사운드
	//................................................................................	
	if( NULL != pExtraInfo )
		g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );	
	
	EffectTargetHit( pSrc , pDest , pExtraInfo );
	
	return TRUE;
}

//..............................................................................................................
// 텔레포트
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
	// 스킬 발동 사운드
	//................................................................................	
	if( NULL != pExtraInfo )
		g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	
	EffectTargetHit( pSrc , pDest , pExtraInfo );
	
	return TRUE;
}

//................................................................................................................
// 홀리 라이트
//................................................................................................................
int CEffectManager::MAKE_EF_HOLY_LIGHT(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{	
	if( CheckCharacter( pSrc , pDest ) == FALSE )
		return TRUE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 3.1f , pSrc->GetUnique() , pSrc->GetIndex() );
	
	//................................................................................	
	// 스킬 발동 사운드
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
// 기적 스킬 ( 나이트 )
//...............................................................................................................
int CEffectManager::MAKE_EF_MIRACLE(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 3.1f , pSrc->GetUnique() , pSrc->GetIndex() );
	
	//................................................................................	
	// 스킬 발동 사운드
	//................................................................................	
	if( NULL != pExtraInfo )
	{
		g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
		
		EffectTargetHit( pSrc , pDest , pExtraInfo );
	}
	
	return TRUE;
}

//................................................................................................................
// 봉인의 빛
//................................................................................................................
int CEffectManager::MAKE_EF_UNCOMBO(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL || pDest == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 5.1f , pSrc->GetUnique() , pSrc->GetIndex() );
	
	//................................................................................	
	// 스킬 발동 사운드
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
// 분신
//................................................................................................................
int CEffectManager::MAKE_EF_SELF_COPY(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 3.1f , pSrc->GetUnique() , pSrc->GetIndex() );
	
	//................................................................................	
	// 스킬 발동 사운드
	//................................................................................	
	if( NULL != pExtraInfo )
		g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	
	EffectTargetHit( pSrc , pDest , pExtraInfo );
	
	return TRUE;
}

//................................................................................................................
// 디펜스 오로라
//................................................................................................................
int CEffectManager::MAKE_EF_DEFENSE_AURA(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 2.1f , pSrc->GetUnique() , pSrc->GetIndex() );
	//................................................................................	
	// 스킬 발동 사운드
	//................................................................................	
	g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	
	// 데스트 없으면 소스쪽만 이펙트 출력
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	// 1:N 데미지
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
	// 어택 사운드
	//................................................................................	
	if( pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillAttackedSound( pExtraInfo->SkillCode , pDest );
	
	if( pSrc == g_Pc.GetPlayer() )
		g_Particle.RockCamera( ( rand() % 200 ) + 300 );
	
	return TRUE;
}


//................................................................................................................
// 디펜스 실드
//................................................................................................................
int CEffectManager::MAKE_EF_DEPENCE_SHILED(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 2.1f , pSrc->GetUnique() , pSrc->GetIndex() );
	
	//................................................................................	
	// 스킬 발동 사운드
	//................................................................................	
	if( NULL != pExtraInfo )
		g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	
	EffectTargetHit( pSrc , pDest , pExtraInfo );
	
	return TRUE;
}

//..............................................................................................................
// 마나 버퍼링
//..............................................................................................................
int CEffectManager::MAKE_EF_MANA_BUFFERING(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 2.1f , pSrc->GetUnique() , pSrc->GetIndex() );
	
	//................................................................................	
	// 스킬 발동 사운드
	//................................................................................	
	if( NULL != pExtraInfo )
		g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	
	EffectTargetHit( pSrc , pDest , pExtraInfo );
	
	return TRUE;
}

//..............................................................................................................
// 셀프 힐링
//..............................................................................................................
int CEffectManager::MAKE_EF_SELF_HEAL(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 5.1f , pSrc->GetUnique() , pSrc->GetIndex() );
	
	//................................................................................	
	// 스킬 발동 사운드
	//................................................................................	
	if( NULL != pExtraInfo )
		g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	
	EffectTargetHit( pSrc , pDest , pExtraInfo );
	
	return TRUE;
}

//..............................................................................................................
// 리턴
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
	// 스킬 발동 사운드
	//................................................................................	
	if( NULL != pExtraInfo )
		g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	
	EffectTargetHit( pSrc , pDest , pExtraInfo );
	
	return TRUE;
}


//..............................................................................................................
// 슬립
//..............................................................................................................
int CEffectManager::MAKE_EF_SLEEP_STICK(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL || pDest == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , EF_SLEEP_STICK    , 0 , 2.1f , pSrc->GetUnique() , pSrc->GetIndex() );
	
	g_Particle.SetEffSetCon( 0.0f , EF_SLEEP_STICK_ED , 0 , 1.1f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
	
	//................................................................................	
	// 스킬 발동 사운드
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
// 걷기 물
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
// 거대몬스터 기본 공격
//..............................................................................................................
int CEffectManager::MAKE_EF_BIG_NPC_ATTACK0(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// 데스트 없으면 소스쪽만 이펙트 출력
	if( pSrc == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFBigNpc0 );						// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// 소스 , 타켓 지정
	
	PushEffect( &pData );												// 메니저에 넣기
	
																		/*
																		g_Particle.SetEffSetCon( 0.0f , id , ESLT_CHARPOS , 8.1f , pSrc->GetUnique() , pSrc->GetIndex() );
																		
																		  StandTargetHit( pSrc , pDest , pExtraInfo );
	*/
	return TRUE;
}

//...........................................................................................................
// 기본공격 1:1 데미지 주는것 처리
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
// 거대몬스터 나타날때
//..............................................................................................................
int CEffectManager::MAKE_EF_BIG_NPC_APPEAR0(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , ESLT_CHARPOS , EFFECT_SET_LOOP_TIME , pSrc->GetUnique() , pSrc->GetIndex() );
	
	return TRUE;
}


//..............................................................................................................
// 유료 아이템 
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
// 파이몬 우두머리 때리기 
//...............................................................................................................
int CEffectManager::MAKE_EF_NPC_HEAVY_STICK(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	StandTargetHit( pSrc , pDest , pExtraInfo );
	
	return TRUE;
}

//...............................................................................................................
// 파이몬 우두머리 때리기 데미지
//...............................................................................................................
int CEffectManager::MAKE_EF_NPC_HEAVY_STICKED(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pDest == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 2.1f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
	
	return TRUE;
}


//................................................................................................................
// 무장 해제
//................................................................................................................
int CEffectManager::MAKE_EF_WEAPON_CRASH0(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// 데스트 없으면 소스쪽만 이펙트 출력
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	g_Particle.SetEffSetCon( 0.0f , EF_WEAPON_CRASH0 , 0 , 1.1f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
	//................................................................................	
	// 스킬 발동 사운드
	//................................................................................	
	g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	
	pExtraInfo->AfterEffect = EF_WEAPON_CRASH0;
	pExtraInfo->DestType = 0;
	pExtraInfo->lifeTime = 1.1f;
	
	EffectTargetHit( pSrc , pDest , pExtraInfo );
	//................................................................................	
	// 어택 사운드
	//................................................................................	
	if( pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillAttackedSound( pExtraInfo->SkillCode , pDest );
	
	return TRUE;
}

//...............................................................................................................
// NPC 도끼로 바닥 찍기
//...............................................................................................................
int CEffectManager::MAKE_EF_AXE_LAND_HIT(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , EF_AXE_LAND_HIT , 0 , 2.6f , pSrc->GetUnique() , pSrc->GetIndex() );
	//................................................................................	
	// 스킬 발동 사운드
	//................................................................................	
	g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	
	// 데스트 없으면 소스쪽만 이펙트 출력
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	// 1:N 데미지
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
	// 어택 사운드
	//................................................................................	
	if( pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillAttackedSound( pExtraInfo->SkillCode , pDest );
	
	return TRUE;
}

//..............................................................................................................
// 거대몬스터 스킬
//..............................................................................................................
int CEffectManager::MAKE_EF_BIG_NPC_SKILL0(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFBigNpcSkill0 );					// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// 소스 , 타켓 지정
	
	PushEffect( &pData );												// 메니저에 넣기	
	
	return TRUE;
}

//..............................................................................................................
// 비즈XP-610 아이 빔
//..............................................................................................................
int CEffectManager::MAKE_EF_NPC_BIZ_BEAM(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFBizEyeBeam );						// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// 소스 , 타켓 지정
	
	PushEffect( &pData );												// 메니저에 넣기	
	
	return TRUE;
}


//...............................................................................................................
// 프리스트 부활
//...............................................................................................................
int CEffectManager::MAKE_EF_REVIVAL(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 2.1f , pSrc->GetUnique() , pSrc->GetIndex() );
	//................................................................................	
	// 스킬 발동 사운드
	//................................................................................	
	g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	
	// 데스트 없으면 소스쪽만 이펙트 출력
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	EffectTargetHit( pSrc , pDest , pExtraInfo );
	//................................................................................	
	// 어택 사운드
	//................................................................................	
	if( pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillAttackedSound( pExtraInfo->SkillCode , pDest );
	
	return TRUE;
}

//..............................................................................................................
// 프레임 스톰
//..............................................................................................................
int CEffectManager::MAKE_EF_FRAME_STORM(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	D3DXVECTOR3 vPos;
	
	vPos.x = m_TargetInfo.vPos.x;
	vPos.z = m_TargetInfo.vPos.z;
	vPos.y = g_Map.PHF_GetHeight( vPos.x , vPos.z );
	
	g_Particle.SetEffSetCon( 0.0f , id , 100 , 5.1f , -1 , -1 , vPos );
	//................................................................................	
	// 스킬 발동 사운드
	//................................................................................	
	g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	
	// 데스트 없으면 소스쪽만 이펙트 출력
	if( pExtraInfo == NULL || pSrc == NULL )
		return TRUE;
	
	// 1:N 데미지
	int Count = pExtraInfo->GetTargetCount();
	
	Character			*pTempDest = NULL;
	EFFECT_EXTRA_INFO    Info;
	
	for( int i = 0 ; i < Count ; ++i )
	{
		if( pExtraInfo->GetTargetInfo( &pTempDest , &Info , i ) )
		{
			//................................................................................	
			// 어택 사운드
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
// 치유의 빛
//................................................................................................................
int CEffectManager::MAKE_EF_HEAL_LIGHT(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 3.1f , pSrc->GetUnique() , pSrc->GetIndex() );
	//................................................................................	
	// 스킬 발동 사운드
	//................................................................................	
	g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	
	// 데스트 없으면 소스쪽만 이펙트 출력
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	// 1:N 데미지
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
	// 어택 사운드
	//................................................................................	
	if( pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillAttackedSound( pExtraInfo->SkillCode , pDest );
	
	return TRUE;
}

//...............................................................................................................
// 발구르기
//...............................................................................................................
int CEffectManager::MAKE_EF_FOOT_QUAKE(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 2.1f , pSrc->GetUnique() , pSrc->GetIndex() );
	//................................................................................	
	// 스킬 발동 사운드
	//................................................................................	
	g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	
	//................................................................................	
	// 어택 사운드
	//................................................................................	
	if( pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillAttackedSound( pExtraInfo->SkillCode , pDest );
	
	// 데스트 없으면 소스쪽만 이펙트 출력
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
	
	// 1:N 데미지
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
// 선풍열각
//................................................................................................................
int CEffectManager::MAKE_EF_CIRCLE_KICK(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 2.1f , pSrc->GetUnique() , pSrc->GetIndex() );
	//................................................................................	
	// 스킬 발동 사운드
	//................................................................................	
	g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	
	// 데스트 없으면 소스쪽만 이펙트 출력
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	// 1:N 데미지
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
	// 어택 사운드
	//................................................................................	
	if( pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillAttackedSound( pExtraInfo->SkillCode , pDest );
	
	if( pSrc == g_Pc.GetPlayer() )
		g_Particle.RockCamera( ( rand() % 200 ) + 300 );
	
	return TRUE;
}


//..............................................................................................................
// 타이니 웨이브
//..............................................................................................................
int CEffectManager::MAKE_EF_TYNEE_WAVE(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// 데스트 없으면 소스쪽만 이펙트 출력
	if( pSrc == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFTyneeWave );						// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// 소스 , 타켓 지정
	
	PushEffect( &pData );												// 메니저에 넣기
	
	return TRUE;
}

//................................................................................................................
// 연기파
//................................................................................................................
int CEffectManager::MAKE_EF_ENERGY(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// 데스트 없으면 소스쪽만 이펙트 출력
	if( pSrc == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFEnergy );							// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// 소스 , 타켓 지정
	
	PushEffect( &pData );												// 메니저에 넣기
	
	return TRUE;
}


//...............................................................................................................
// 검진
//...............................................................................................................
int CEffectManager::MAKE_EF_SWORD_FLASH(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , ESLT_SRC_RAD , 2.1f , pSrc->GetUnique() , pSrc->GetIndex() , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) , pSrc->GetDirection() );
	//................................................................................	
	// 스킬 발동 사운드
	//................................................................................	
	g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	
	// 데스트 없으면 소스쪽만 이펙트 출력
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
	
	// 1:N 데미지
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
	// 어택 사운드
	//................................................................................	
	if( pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillAttackedSound( pExtraInfo->SkillCode , pDest );
	
	return TRUE;
}

//...............................................................................................................
// 파티 힐링
//...............................................................................................................
int CEffectManager::MAKE_EF_PARTY_HEAL(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 4.1f , pSrc->GetUnique() , pSrc->GetIndex() );
	//................................................................................	
	// 스킬 발동 사운드
	//................................................................................	
	g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	
	// 데스트 없으면 소스쪽만 이펙트 출력
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	// 1:N 데미지
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
	// 어택 사운드
	//................................................................................	
	if( pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillAttackedSound( pExtraInfo->SkillCode , pDest );
	
	
	return TRUE;
}


//...............................................................................................................
// 파이몬 바닥 찍기 스킬
//...............................................................................................................
int CEffectManager::MAKE_EF_NPC_QUAKE_HEAVY_STICK(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , EF_NPC_QUAKE_HEAVY_STICKED2 , 0 , 2.1f , pSrc->GetUnique() , pSrc->GetIndex() );
	//................................................................................	
	// 스킬 발동 사운드
	//................................................................................	
	g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	
	
	// 데스트 없으면 소스쪽만 이펙트 출력
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	// 1:N 데미지
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
// 도발 
//...............................................................................................................
int CEffectManager::MAKE_EF_ATTACKED_FOCUS(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id			    , 0 , 2.1f , pSrc->GetUnique()  , pSrc->GetIndex() );
	
	// 데스트 없으면 소스쪽만 이펙트 출력
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	g_Particle.SetEffSetCon( 0.0f , EF_ATTACK_FOCUS , 0 , 2.1f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
	
	pExtraInfo->AfterEffect = EF_ATTACK_FOCUS;
	pExtraInfo->DestType = 0;
	pExtraInfo->lifeTime = 2.1f;
	
	EffectTargetHit( pSrc , pDest , pExtraInfo );
	
	//................................................................................	
	// 발동 사운드
	//................................................................................	
	if( pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	
	return TRUE;
}

//..............................................................................................................
// 아이스 레인
//..............................................................................................................
int CEffectManager::MAKE_EF_ICE_RAIN(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	CEffectBase *pData = SAFE_NEW( CEFIceRain );					// 클래스 생성	
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );					// 소스 , 타켓 지정
	
	PushLandEffectBase( &pData );									// 메니저에 넣기
	
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
// 빅프레스
//..............................................................................................................
int CEffectManager::MAKE_EF_BIG_PRESS(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	CEffectBase *pData = SAFE_NEW( CEFBigPress );			// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );			// 소스 , 타켓 지정
	
	PushEffect( &pData );									// 메니저에 넣기
	
	return TRUE;
}

//................................................................................................................
// 프리스트 디스펠
//................................................................................................................
int CEffectManager::MAKE_EF_PRIEST_DISPELL(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id			 , 0 , 4.1f , pSrc->GetUnique() , pSrc->GetIndex() );
	
	// 데스트 없으면 소스쪽만 이펙트 출력
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
// 디스펠 베드
//................................................................................................................
int CEffectManager::MAKE_EF_DISPELL_BAD(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , EF_DISPELL_BAD , 0 , 2.1f , pSrc->GetUnique() , pSrc->GetIndex() );
	
	// 데스트 없으면 소스쪽만 이펙트 출력
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
// 블레스
//................................................................................................................
int CEffectManager::MAKE_EF_BRESS(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 2.1f , pSrc->GetUnique() , pSrc->GetIndex() );
	
	// 데스트 없으면 소스쪽만 이펙트 출력
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	EffectTargetHit( pSrc , pDest , pExtraInfo );
	
	g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	
	return TRUE;
}

//...............................................................................................................
// 블러켄 화이어볼 공격
//...............................................................................................................
int CEffectManager::MAKE_EF_NPC_FIRE(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// 데스트 없으면 소스쪽만 이펙트 출력
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFNpcFireBall );					// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// 소스 , 타켓 지정
	
	PushEffect( &pData );												// 메니저에 넣기
	
	return TRUE;
}

//...............................................................................................................
// NPC 짱돌 공격
//...............................................................................................................
int CEffectManager::MAKE_EF_ROCK_SHOT(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// 데스트 없으면 소스쪽만 이펙트 출력
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFNpcStoneAttack );					// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// 소스 , 타켓 지정
	
	PushEffect( &pData );												// 메니저에 넣기
	
	return TRUE;
}

//..............................................................................................................
// 칠링 터치
//..............................................................................................................
int CEffectManager::MAKE_EF_CR_TOUCH(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pDest == NULL )
	{
		return TRUE;
	}
	
	//................................................................................	
	// 스킬 발동 사운드
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
	// 어택 사운드
	//................................................................................	
	if( pExtraInfo != NULL && pDest != NULL )
		g_Particle.m_EffectManager.GetSkillAttackedSound( pExtraInfo->SkillCode , pDest );
	
	return TRUE;
}

//..............................................................................................................
// 힐링 웨이브
//..............................................................................................................
int CEffectManager::MAKE_EF_HEAL_WAVE(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( CheckCharacter( pSrc , pDest ) == FALSE )
		return TRUE;
	
	//................................................................................	
	// 스킬 발동 사운드
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
	// 어택 사운드
	//................................................................................	
	if( pExtraInfo != NULL && pDest != NULL )
		g_Particle.m_EffectManager.GetSkillAttackedSound( pExtraInfo->SkillCode , pDest );
	
	return TRUE;
}

//..............................................................................................................
// 기합
//..............................................................................................................
int CEffectManager::MAKE_EF_SELF_SHOUT(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	//................................................................................	
	// 스킬 발동 사운드
	//................................................................................	
	if( pExtraInfo != NULL && pSrc != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	
	if( pSrc == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 4.1f , pSrc->GetUnique() , pSrc->GetIndex() );
	EffectTargetHit( pSrc , pDest , pExtraInfo );
	
	//................................................................................	
	// 발동 사운드
	//................................................................................	
	if( pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	
	return TRUE;
}

//..............................................................................................................
// 은신 
//..............................................................................................................
int CEffectManager::MAKE_EF_HIDE(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	CEffectBase *pData = SAFE_NEW( CEFHide );							// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// 소스 , 타켓 지정
	
	PushEffect( &pData );												// 메니저에 넣기
	
	return TRUE;
}

//..............................................................................................................
// 오월의 비
//..............................................................................................................
int CEffectManager::MAKE_EF_BOW_RAIN(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	CEffectBase *pData = SAFE_NEW( CEFBowRain );						// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// 소스 , 타켓 지정
	
	PushEffect( &pData );												// 메니저에 넣기
	
	return TRUE;
}

//..............................................................................................................
// 치유의 비
//..............................................................................................................
int CEffectManager::MAKE_EF_HEAL_RAIN_BOW(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	CEffectBase *pData = SAFE_NEW( CEFHealRainBow );					// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// 소스 , 타켓 지정
	
	PushEffect( &pData );												// 메니저에 넣기
	
	return TRUE;
}


//..............................................................................................................
// 월영
//..............................................................................................................
int CEffectManager::MAKE_EF_MOMENT_HIDE	(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	CEffectBase *pData = SAFE_NEW( CEFMomentHide );						// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// 소스 , 타켓 지정
	
	PushEffect( &pData );												// 메니저에 넣기
	
	return TRUE;
}

//..............................................................................................................
// 질주
//..............................................................................................................
int CEffectManager::MAKE_EF_SCAPMPER(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	CEffectBase *pData = SAFE_NEW( CEFScapmper );						// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// 소스 , 타켓 지정
	
	PushEffect( &pData );												// 메니저에 넣기
	
	return TRUE;
}

//..............................................................................................................
// 포이즌 시드
//..............................................................................................................
int CEffectManager::MAKE_EF_POISON_SEED(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	//................................................................................	
	// 스킬 발동 사운드
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
	// 어택 사운드
	//................................................................................	
	if( pExtraInfo != NULL && pDest != NULL )
		g_Particle.m_EffectManager.GetSkillAttackedSound( pExtraInfo->SkillCode , pDest );
	
	return TRUE;
}

//..............................................................................................................
// 암연살
//..............................................................................................................
int CEffectManager::MAKE_EF_MAYUNSAL(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	//................................................................................	
	// 스킬 발동 사운드
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
	// 어택 사운드
	//................................................................................	
	if( pExtraInfo != NULL && pDest != NULL )
		g_Particle.m_EffectManager.GetSkillAttackedSound( pExtraInfo->SkillCode , pDest );
	
	EffectTargetHit( pSrc , pDest , pExtraInfo );
	
	return TRUE;
}

//..............................................................................................................
// 펄리닉 더스트
//..............................................................................................................
int CEffectManager::MAKE_EF_PEAL_DUST(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	//................................................................................	
	// 스킬 발동 사운드
	//................................................................................	
	if( pExtraInfo != NULL && pSrc != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	
	EffectTargetHit( pSrc , pDest , pExtraInfo );
	
	//................................................................................	
	// 어택 사운드
	//................................................................................	
	if( pExtraInfo != NULL && pDest != NULL )
		g_Particle.m_EffectManager.GetSkillAttackedSound( pExtraInfo->SkillCode , pDest );
	
	return TRUE;
}

//...............................................................................................................
// NPC 과일 공격
//...............................................................................................................
int CEffectManager::MAKE_EF_FRUIT_ATTACK(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// 데스트 없으면 소스쪽만 이펙트 출력
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFFruitAttack );					// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// 소스 , 타켓 지정
	
	PushEffect( &pData );												// 메니저에 넣기
	
	return TRUE;
}

//...............................................................................................................
// NPC 입 -> 불
//...............................................................................................................
int CEffectManager::MAKE_EF_MOUTH_FIRE(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// 데스트 없으면 소스쪽만 이펙트 출력
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFMouthFire );  					// 클래스 생성
	
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
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// 소스 , 타켓 지정
	
	PushEffect( &pData );												// 메니저에 넣기
	
	return TRUE;
}

//...............................................................................................................
// 날라가는 스킬 이펙트 공통
//...............................................................................................................
int CEffectManager::MakeCommonFlySkill(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,
									   int FrontNum,int FrontType,
									   int TailNum ,int TailType,
									   int AttackedNum,DWORD RangeTime,int BoneNum,BOOL ISSKILL)
{
	if( pExtraInfo == NULL || pDest == NULL || pSrc == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFCommonFlySkill );					// 클래스 생성
	
	(( CEFCommonFlySkill * )pData)->SetInit(
		FrontNum , ( CEFCommonFlySkill::CommonFlySkill_Effect_Type ) FrontType , 
		TailNum  , ( CEFCommonFlySkill::CommonFlySkill_Effect_Type ) TailType  ,
		AttackedNum,RangeTime,pSrc,BoneNum , ISSKILL );
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// 소스 , 타켓 지정
	
	PushEffect( &pData );												// 메니저에 넣기
	
	return TRUE;
}


//...............................................................................................................
// NPC 가스 공격
//...............................................................................................................
int CEffectManager::MAKE_EF_GAS_SHOT(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// 데스트 없으면 소스쪽만 이펙트 출력
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFMouthFire );						// 클래스 생성
	
	(( CEFMouthFire * )pData)->SetInit( PT_NPC_MOUTH_GAS, PT_NPC_MOUTH_GAS_TAIL ,
		70,1,EF_GAS_SHOTED);
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// 소스 , 타켓 지정
	
	PushEffect( &pData );												// 메니저에 넣기
	
	return TRUE;
}

//...............................................................................................................
// NPC 물방울 뿜기
//...............................................................................................................
int CEffectManager::MAKE_EF_NPC_BUBBLE_ATTACK(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// 데스트 없으면 소스쪽만 이펙트 출력
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFNPCBubble );						// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// 소스 , 타켓 지정
	
	PushEffect( &pData );												// 메니저에 넣기
	
	return TRUE;
}

//...............................................................................................................
// NPC 지팡이 공격 ( 기본 공격 )
//...............................................................................................................
int CEffectManager::MAKE_EF_NPC_STICK(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// 데스트 없으면 소스쪽만 이펙트 출력
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFNPCStickAttack );					// 클래스 생성	
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// 소스 , 타켓 지정
	
	PushEffect( &pData );												// 메니저에 넣기
	
	return TRUE;
}

//...............................................................................................................
// NPC 지팡이 공격 ( 기본 공격 )
//...............................................................................................................
int CEffectManager::MAKE_EF_NPC_STICK_ATTACKED(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pDest == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 4.1f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
	
	return TRUE;
}


//...............................................................................................................
// 	킵업 펌프 
//...............................................................................................................
int CEffectManager::MAKE_EF_KEEPUP_PUMP(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// 데스트 없으면 소스쪽만 이펙트 출력
	CEffectBase *pData = SAFE_NEW( CEFKeepUpPump );						// 클래스 생성
	
	( ( CEFKeepUpPump * ) pData )->SetTimer( Timer );
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// 소스 , 타켓 지정
	
	PushEffect( &pData );												// 메니저에 넣기
	
	//................................................................................	
	// 발동 사운드
	//................................................................................	
	if( pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	
	return TRUE;
}


//................................................................................................................
// 몸에 붙는 이펙트
//................................................................................................................
int CEffectManager::MAKE_EF_SRC_LINK(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// 데스트 없으면 소스쪽만 이펙트 출력
	CEffectBase *pData = SAFE_NEW( CEFSrcLink );						// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// 소스 , 타켓 지정
	
	PushEffect( &pData );												// 메니저에 넣기
	
	return TRUE;
}


//................................................................................................................
// 거대유닛 몸에 붙는 이펙트
//................................................................................................................
int CEffectManager::MAKE_EF_GIANTUNIT_SRC_LINK(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// 데스트 없으면 소스쪽만 이펙트 출력
	CEffectBase *pData = SAFE_NEW( CEFSrcLink );						// 클래스 생성
	
	((CEFSrcLink *)pData)->m_IsGiantUnit = TRUE;
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// 소스 , 타켓 지정
	
	PushEffect( &pData );	
	
	return TRUE;
}

///-- 가드타워 관련 공격이펙트
int CEffectManager::MAKE_G_TOWER_EFFECT(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo, int nSrcBonNum , UINT uBillType, 
										UINT uTailType, UINT uAttackedEffId, UINT uAttackedType)
{
	CEffectBase *pData = SAFE_NEW( CEFGTEffect );	
	
	((CEFGTEffect *)pData)->m_nSrcBoneNum = nSrcBonNum;
	((CEFGTEffect *)pData)->m_uBillType = uBillType;	
	((CEFGTEffect *)pData)->m_uTaleType = uTailType;
	((CEFGTEffect *)pData)->m_uAttackedEffect = uAttackedEffId;
	((CEFGTEffect *)pData)->m_uAttackedType = uAttackedType;
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// 소스 , 타켓 지정
	
	PushEffect( &pData );	
	
	return TRUE;
}

///-- 거대대포 공격이펙트
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
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// 소스 , 타켓 지정
	
	PushEffect( &pData );
	
	return TRUE;
}

///-- 거신병 공격 이펙트
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
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// 소스 , 타켓 지정
	
	PushEffect( &pData );	
	
	return TRUE;
}

//................................................................................................................
// 따뜻한 빛
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
// 	프리스트 힐링
//...............................................................................................................
int CEffectManager::MAKE_EF_PRIEST_HEAL(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 4.1f , pSrc->GetUnique() , pSrc->GetIndex() );
	
	//................................................................................	
	// 스킬 발동 사운드
	//................................................................................	
	g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	
	// 데스트 없으면 소스쪽만 이펙트 출력
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
	// 어택 사운드
	//................................................................................	
	if( pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillAttackedSound( pExtraInfo->SkillCode , pDest );
	
	return TRUE;
}

//...............................................................................................................
// 대원참
//...............................................................................................................
int CEffectManager::MAKE_EF_CIRCLE_SWORD(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 2.1f , pSrc->GetUnique() , pSrc->GetIndex() );
	
	//................................................................................	
	// 스킬 발동 사운드
	//................................................................................	
	g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	
	
	// 데스트 없으면 소스쪽만 이펙트 출력
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	// 1:N 데미지
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
	// 어택 사운드
	//................................................................................	
	if( pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillAttackedSound( pExtraInfo->SkillCode , pDest );
	
	return TRUE;
}

//...............................................................................................................
// 	프리스트 행운의 여신
//...............................................................................................................
int CEffectManager::MAKE_EF_LUCKY_GOD(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id			    , 0 , 2.1f , pSrc->GetUnique()  , pSrc->GetIndex() );
	//................................................................................	
	// 스킬 발동 사운드
	//................................................................................	
	g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	
	// 데스트 없으면 소스쪽만 이펙트 출력
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	
	g_Particle.SetEffSetCon( 0.0f , EF_LUCKY_GODED   , ESLT_CHARPOS , 4.1f , pDest->GetUnique()  , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
	
	pExtraInfo->AfterEffect = EF_LUCKY_GODED;
	pExtraInfo->DestType = ESLT_CHARPOS;
	pExtraInfo->lifeTime = 4.1f;
	
	EffectTargetHit( pSrc , pDest , pExtraInfo );
	//................................................................................	
	// 어택 사운드
	//................................................................................	
	if( pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillAttackedSound( pExtraInfo->SkillCode , pDest );
	
	
	return TRUE;
}

//...............................................................................................................
// 	프리스트 행운의 여신 ( 당하는 쪽 )
//...............................................................................................................
int CEffectManager::MAKE_EF_LUCKY_GODED(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id			   , 0 , 2.1f , pSrc->GetUnique()  , pSrc->GetIndex() );
	
	return TRUE;
}



//................................................................................................................
// 회피
//................................................................................................................
int CEffectManager::MAKE_EF_AVOID0(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// 데스트 없으면 소스쪽만 이펙트 출력
	if( pSrc == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFAvoid );							// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// 소스 , 타켓 지정
	
	PushEffect( &pData );												// 메니저에 넣기
	
	//................................................................................	
	// 발동 사운드
	//................................................................................	
	if( pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	
	return TRUE;
}

//................................................................................................................
// 라이트닝
//................................................................................................................
int CEffectManager::MAKE_EF_LIGHTNING0(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// 데스트 없으면 소스쪽만 이펙트 출력
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFLightning );						// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// 소스 , 타켓 지정
	
	PushEffect( &pData );												// 메니저에 넣기
	
	return TRUE;
}

//................................................................................................................
// 화이어 볼
//................................................................................................................
int CEffectManager::MAKE_EF_FIREBALL0(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// 데스트 없으면 소스쪽만 이펙트 출력
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFFireBall );						// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// 소스 , 타켓 지정
	
	PushEffect( &pData );												// 메니저에 넣기
	
	return TRUE;
}


//................................................................................................................
// 회복 아이템 2 ( 지속성 )
//................................................................................................................
int CEffectManager::MAKE_EF_RECOVERY_ITEM2_0(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	CEffectBase *pData = SAFE_NEW( CEFRecoveryItem2 );					// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// 소스 , 타켓 지정
	
	PushEffect( &pData );												// 메니저에 넣기
	
	return TRUE;
}

//................................................................................................................
// 파이어 아머 ( 지속성 )
//................................................................................................................
int CEffectManager::MAKE_EF_FIRE_ARMOR(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	CEffectBase *pData = SAFE_NEW( CEFFireArmor );						// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// 소스 , 타켓 지정
	
	PushEffect( &pData );												// 메니저에 넣기
	
	return TRUE;
}

//................................................................................................................
// 아이스 실드 ( 지속성 )
//................................................................................................................
int CEffectManager::MAKE_EF_ICE_SHILED(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	CEffectBase *pData = SAFE_NEW( CEFIceShiled );						// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// 소스 , 타켓 지정
	
	PushEffect( &pData );												// 메니저에 넣기
	
	return TRUE;
}

//................................................................................................................
// 락 프레스
//................................................................................................................
int CEffectManager::MAKE_EF_ROCKPRESS0(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// 데스트 없으면 소스쪽만 이펙트 출력
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFRockPress );						// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// 소스 , 타켓 지정
	
	PushEffect( &pData );												// 메니저에 넣기
	
	return TRUE;
}

//................................................................................................................
// 우든 소드
//................................................................................................................
int CEffectManager::MAKE_EF_WOODENSWORD0(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// 데스트 없으면 소스쪽만 이펙트 출력
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFWoodenSword );					// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// 소스 , 타켓 지정
	
	PushEffect( &pData );												// 메니저에 넣기
	
	return TRUE;
}

//................................................................................................................
// 락 실드
//................................................................................................................
int CEffectManager::MAKE_EF_ROCK_SHIELD0(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	CEffectBase *pData = SAFE_NEW( CEFRockShield );						// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// 소스 , 타켓 지정
	
	PushEffect( &pData );												// 메니저에 넣기
	
	return TRUE;
}

//................................................................................................................
// 쾌검
//................................................................................................................
int CEffectManager::MAKE_EF_REFRESH_SWORD0(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	CEffectBase *pData = SAFE_NEW( CEFRefreshSword );					// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// 소스 , 타켓 지정
	
	PushEffect( &pData );												// 메니저에 넣기
	
	//................................................................................	
	// 발동 사운드
	//................................................................................	
	if( pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	
	return TRUE;
}

//...............................................................................................................
// 	받아치기
//...............................................................................................................
int CEffectManager::MAKE_EF_BOUND_ATTACK(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	// 데스트 없으면 소스쪽만 이펙트 출력
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
// 강검
//................................................................................................................
int CEffectManager::MAKE_EF_STRONG_SWORD0(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	CEffectBase *pData = SAFE_NEW( CEFStrongSword );					// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// 소스 , 타켓 지정
	
	PushEffect( &pData );												// 메니저에 넣기
	
	return TRUE;
}

//................................................................................................................
// 노려베기
//................................................................................................................
int CEffectManager::MAKE_EF_V_CUT_ATTACK0(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	//g_Particle.SetEffSetCon( 0.0f , EF_V_CUT_ATTACK0  , 0 , 1.9f , pSrc->GetUnique() , pSrc->GetIndex() );
	
	// 데스트 없으면 소스쪽만 이펙트 출력
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
// 급소 베기
//................................................................................................................
int CEffectManager::MAKE_EF_CAP_SWORD_ATTACK0(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	// 데스트 없으면 소스쪽만 이펙트 출력
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
// 빙의 해제
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
// 화염 연타
//................................................................................................................
int CEffectManager::MAKE_EF_FIRE_SMACK(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// 데스트 없으면 소스쪽만 이펙트 출력
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFFireSmack );						// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// 소스 , 타켓 지정
	
	PushEffect( &pData );												// 메니저에 넣기
	
	return TRUE;
}


//................................................................................................................
// 모아 베기
//................................................................................................................
int CEffectManager::MAKE_EF_GATHER_CUT0(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// 데스트 없으면 소스쪽만 이펙트 출력
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFGatherCut );						// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// 소스 , 타켓 지정
	
	PushEffect( &pData );												// 메니저에 넣기
	
	return TRUE;
}

//................................................................................................................
// 버블 크래쉬
//................................................................................................................
int CEffectManager::MAKE_EF_BUBBLE_CRASH0(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// 데스트 없으면 소스쪽만 이펙트 출력
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFBubbleCrash );					// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// 소스 , 타켓 지정
	
	PushEffect( &pData );												// 메니저에 넣기
	
	return TRUE;
}


//................................................................................................................
// 아이스 스피어
//................................................................................................................
int CEffectManager::MAKE_EF_ICESPIRE0(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// 데스트 없으면 소스쪽만 이펙트 출력
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFIceSpire );						// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// 소스 , 타켓 지정
	
	PushEffect( &pData );												// 메니저에 넣기
	
	return TRUE;
}

//................................................................................................................
// 회복 아이템 1 ( 즉효성 )
//................................................................................................................
int CEffectManager::MAKE_EF_RECOVERY_ITEM1_0(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 4.1f , pSrc->GetUnique() , pSrc->GetIndex() );
	
	return TRUE;
}

//............................................................................................................
// 기본 활 공격 데미지 이펙트
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
// 프리스트 기본 공격 
//............................................................................................................
int CEffectManager::MAKE_EF_PRIEST_STAND_ATTACK(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	CEffectBase *pData = SAFE_NEW( CEFPriestStandAttack );				// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// 소스 , 타켓 지정
	
	PushEffect( &pData );												// 메니저에 넣기
	
	return TRUE;
}

//............................................................................................................
// 프리스트 기본 공격 데미지
//............................................................................................................
int CEffectManager::MAKE_EF_PRIEST_STAND_ATTACKED(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL || pDest == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 2.1f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
	
	return TRUE;
}


//............................................................................................................
// 노려쏘기
//............................................................................................................
int CEffectManager::MAKE_EF_GLAER_BOW_ATTACK(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// 데스트 없으면 소스쪽만 이펙트 출력
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFGlareBow );						// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// 소스 , 타켓 지정
	
	PushEffect( &pData );												// 메니저에 넣기
	
	
	return TRUE;
}


//............................................................................................................
// 더블샷 
//............................................................................................................
int CEffectManager::MAKE_EF_DOUBLESHOT(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// 데스트 없으면 소스쪽만 이펙트 출력
	if(  pDest == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFBowDoubleShot );					// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// 소스 , 타켓 지정
	
	PushEffect( &pData );												// 메니저에 넣기
	
	
	return TRUE;
}


//..............................................................................................................
// 저격
//..............................................................................................................
int CEffectManager::MAKE_EF_SNIPER_BOW(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// 데스트 없으면 소스쪽만 이펙트 출력
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFSniperBow );						// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// 소스 , 타켓 지정
	
	PushEffect( &pData );												// 메니저에 넣기
	
	return TRUE;
}

//............................................................................................................
// 발등 쏘기
//............................................................................................................
int CEffectManager::MAKE_EF_FOOT_BOW_ATTACK(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	CEffectBase *pData = SAFE_NEW( CEFFootBow );						// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// 소스 , 타켓 지정
	
	PushEffect( &pData );												// 메니저에 넣기
	
	
	return TRUE;
}

//............................................................................................................
// 빙의
//............................................................................................................
int CEffectManager::MAKE_EF_POSSESSED(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	CEffectBase *pData = SAFE_NEW( CEFPossessed );						// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// 소스 , 타켓 지정
	
	PushEffect( &pData );												// 메니저에 넣기
	
	return TRUE;
}

//............................................................................................................
// 빙신 나타나기
//............................................................................................................
int CEffectManager::MAKE_EF_APPEAR_BING(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	CEffectBase *pData = SAFE_NEW( CEFPossessedAppear );				// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// 소스 , 타켓 지정
	
	PushEffect( &pData );												// 메니저에 넣기
	
	
	return TRUE;
}

//............................................................................................................
// 발등 쏘기 ( 화살 ) 링크 된거
//............................................................................................................
int CEffectManager::MAKE_EF_FOOT_BOW_LINK(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	CEffectBase *pData = SAFE_NEW( CEFFootBowLink );							// 클래스 생성
	
	pData->Setup( ( EFFECT_ID ) EF_FOOT_BOW_LINK , pSrc , NULL , NULL );					// 소스 , 타켓 지정
	
	float fRad = D3DXToRadian( ( rand() % 360 ) );
	
	((CEFFootBowLink *)pData)->SetExtraInfo( fRad , D3DXVECTOR3( 0.0f , 1.0f , 0.0f ) , 7 );
	
	PushEffect( &pData );										// 메니저에 넣기
	
	return TRUE;
}

//...............................................................................................................
// 	나이트 방패 가격 데미지
//...............................................................................................................
int CEffectManager::MAKE_EF_SHILED_ATTACKED(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// 데스트 없으면 소스쪽만 이펙트 출력
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
// 	방어형 펫 기본동작 이펙트
//...............................................................................................................
int	CEffectManager::MAKE_EF_DEFENSE_PET_BMOTION(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer, int nBonNum)
{
	
	CEffectBase *pData = SAFE_NEW( CEFCommonFlySkill );					// 클래스 생성
	
	(( CEFCommonFlySkill * )pData)->SetInit(
		PT_DEFENSE_PET , CEFCommonFlySkill::CommonFlySkill_Effect_Type::PARTICLE_LINK , 
		PT_FIREBALL_TAIL  , CEFCommonFlySkill::CommonFlySkill_Effect_Type::PARTICLE_SPOT  ,
		id ,70, pSrc, nBonNum , TRUE, FALSE );
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// 소스 , 타켓 지정
	
	PushEffect( &pData );
	//................................................................................	
	// 발동 사운드
	//................................................................................	
	//	if( pExtraInfo != NULL )
	//		g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	
	int nDamage = pExtraInfo->TargetInfo.theEffectValue[ 0 ];
	
	// 데미지 숫자 찍기
	D3DXVECTOR3 vHeight = D3DXVECTOR3( 
		pSrc->GetPosTM()._41 ,
		pSrc->GetPosTM()._42 + ( pSrc->GetBoundMaxPos().y * 1.2f ) ,
		pSrc->GetPosTM()._43 );
	
	if(nDamage && ( pDest == g_Pc.GetPlayer()) )
		g_Particle.m_DamageNum.Add( vHeight  , nDamage , false, pSrc );
	
	return TRUE;
}

//..............................................................................................................
// 포포이 연타공격
//..............................................................................................................
int	CEffectManager::MAKE_EF_POPOI_ATTACK(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// 데스트 없으면 소스쪽만 이펙트 출력
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFFPopoiAttack );					// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// 소스 , 타켓 지정
	
	PushEffect( &pData );
	
	
	return TRUE;
}

//..............................................................................................................
// 호랑이 최종형태 연속공격스킬
//..............................................................................................................
int	CEffectManager::MAKE_EF_TIGET_ATTACK(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// 데스트 없으면 소스쪽만 이펙트 출력
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFTigerAttack );					// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// 소스 , 타켓 지정
	
	PushEffect( &pData );
	
	return TRUE;
}

//..............................................................................................................
// 아이스 필드
//..............................................................................................................
int	CEffectManager::MAKE_EF_ICE_FIELD(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	CEffectBase *pData = SAFE_NEW( CEFIceField );						// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// 소스 , 타켓 지정
	
	PushLandEffectBase( &pData );
	
	return TRUE;
}

//..............................................................................................................
// 아이스 스피어
//..............................................................................................................
int	CEffectManager::MAKE_EF_ICE_SPEAR(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	//-- 데스트 없으면 소스쪽만 이펙트 출력
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFIceSpear );						// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// 소스 , 타켓 지정
	
	PushEffect( &pData );
	
	return TRUE;
}

//..............................................................................................................
// 라이트 스톰
//..............................................................................................................
int	CEffectManager::MAKE_EF_LIGHT_STORM(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	
	CEffectBase *pData = SAFE_NEW( CEFLightStorm );						// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// 소스 , 타켓 지정
	
	PushLandEffectBase( &pData );
	
	return TRUE;
}

//..............................................................................................................
// 운석의 비
//..............................................................................................................
int	CEffectManager::MAKE_EF_STONE_RAIN(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	
	CEffectBase *pData = SAFE_NEW( CEFStoneRian );						// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// 소스 , 타켓 지정
	
	PushLandEffectBase( &pData );
	
	return TRUE;
}

//..............................................................................................................
// 윈드 버쉬
//..............................................................................................................
int	CEffectManager::MAKE_EF_WIND_BUSH(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	CEffectBase *pData = SAFE_NEW( CEFWindBush );						// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// 소스 , 타켓 지정
	
	PushLandEffectBase( &pData );
	
	return TRUE;
}

//..............................................................................................................
// 라운드 파이어
//..............................................................................................................
int	CEffectManager::MAKE_EF_ROUND_FIRE(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	CEffectBase *pData = SAFE_NEW( CEFRoundFire );						// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// 소스 , 타켓 지정
	
	PushLandEffectBase( &pData );	
	
	return TRUE;
}

//..............................................................................................................
// 라운드 라이트
//..............................................................................................................
int	CEffectManager::MAKE_EF_ROUND_LIGHT(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	CEffectBase *pData = SAFE_NEW( CEFRoundLight );						// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// 소스 , 타켓 지정
	
	PushLandEffectBase( &pData );
	
	return TRUE;
}

//..............................................................................................................
// 캐스트 스톤
//..............................................................................................................
int	CEffectManager::MAKE_EF_CAST_STONE(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// 데스트 없으면 소스쪽만 이펙트 출력
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFCastStone );						// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// 소스 , 타켓 지정
	
	PushEffect( &pData );
	
	return TRUE;
}

//..............................................................................................................
// 대지의 속박
//..............................................................................................................
int	CEffectManager::MAKE_EF_EARTH_BIND(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	CEffectBase *pData = SAFE_NEW( CEFEarthBind );						// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// 소스 , 타켓 지정
	
	PushEffect( &pData );
	
	return TRUE;
}

//..............................................................................................................
// 난사
//..............................................................................................................
int	CEffectManager::MAKE_EF_RANDOM_ARROW(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{	
	CEffectBase *pData = SAFE_NEW( CEFRandomArrow );					// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// 소스 , 타켓 지정
	
	PushEffect( &pData );
	
	return TRUE;
}


//..............................................................................................................
// 연속 애니메이션 스킬
//..............................................................................................................
int CEffectManager::MAKE_EF_CONANISKILL(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// 데스트 없으면 소스쪽만 이펙트 출력
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFConAniSkill );					// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// 소스 , 타켓 지정
	
	PushEffect( &pData );
	
	return TRUE;
}

//..............................................................................................................
// 열풍강타
//..............................................................................................................
int CEffectManager::MAKE_EF_FIRESTORM_SMASH(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// 데스트 없으면 소스쪽만 이펙트 출력
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFFireStormSmash );					// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// 소스 , 타켓 지정
	
	PushEffect( &pData );	
	
	return TRUE;
}

//..............................................................................................................
// 기열연타
//..............................................................................................................
int CEffectManager::MAKE_EF_SPIRIT_FIRE_SMACK(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// 데스트 없으면 소스쪽만 이펙트 출력
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFSpiritFireSmack );				// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// 소스 , 타켓 지정
	
	PushEffect( &pData );
	
	return TRUE;
}

//..............................................................................................................
// 분노의 검진
//..............................................................................................................
int CEffectManager::MAKE_EF_WRATH_KNIFE_ATTACK(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// 데스트 없으면 소스쪽만 이펙트 출력
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFWrathKniteAttack );				// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// 소스 , 타켓 지정
	
	PushEffect( &pData );
	
	return TRUE;
}

//..............................................................................................................
// 서클크래쉬
//..............................................................................................................
int CEffectManager::MAKE_EF_CIRCLE_CRASH(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// 데스트 없으면 소스쪽만 이펙트 출력
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	CEffectBase *pData = SAFE_NEW( CEFCircleCrash );					// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// 소스 , 타켓 지정
	
	PushEffect( &pData );
	
	return TRUE;
}

//..............................................................................................................
// 얼음 화살비 
//..............................................................................................................
int CEffectManager::MAKE_EF_ICE_BOW_RAIN(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	CEffectBase *pData = SAFE_NEW( CEFIceBowRain );					// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );					// 소스 , 타켓 지정
	
	PushEffect( &pData );											// 메니저에 넣기
	
	return TRUE;
}

//..............................................................................................................
// 은신 감지
//..............................................................................................................
int CEffectManager::MAKE_EF_HIDE_SENSOR(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	CEffectBase *pData = SAFE_NEW( CEFHideSensor );					// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );					// 소스 , 타켓 지정
	
	PushEffect( &pData );											// 메니저에 넣기
	
	return TRUE;
}



//................................................................................................................
// 이펙트 타격 처리
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

	for( int i = 0; i < 3; ++i )	// 스킬 효력 처리 3가지  
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
			case n_AttackBlowAdd:			// 물리 필살기 
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
	
	// 데미지가 0일때 죽었을때도 이펙트 보이기
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
			// 데미지 찍을때 플레이어랑 연관있는것만 찍기
			if( IsMyRelationUnit( pSrc , pDest ) ) 
			{
				g_Particle.SetEffSetCon( 0.0f , EF_ATTACK_MISS , 0 , 0.3f , pDest->GetUnique() , pDest->GetIndex() );
			}

			break;
		}
		// return;
	}
	
	// 데미지 숫자 찍기
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
	
	// 데미지 찍을때 플레이어랑 연관있는것만 찍기
	if( IsMyRelationUnit( pSrc , pDest ) && DamageRange > 0 && ISDisplayDamageNum )
	{
		int nDamage = 0;
		
		// 서버에서 수정해주기 전까지만...임시 작업 위에다 정의 함 .. 
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
		// 1. 내 캐릭터가 맞았을때 처리 by wxywxy
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
							
							if( !g_Pc.GetPlayer()->m_State.bRide )	//라이딩 중이 아니라면
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
					if( !g_Pc.GetPlayer()->m_State.bRide )	//라이딩 중이 아니라면
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
						///-- 죽었는데 또 죽으라고 메시지를 처리해야할때
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
					
					// 플레이어 죽었을때 이펙트 지우기
					DeletePlayerEffect( pDest );
				}				
			}
			
			
			// 부가효과에 따른 데미지 적용 및 각각의 수치 및 상태 변화 처리 
		}
		//........................................................................................................
		// 2. 다른 유저가 맞았을때 처리 맞았을때 처리 by wxywxy
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
					
					///-- 공격 주체가 나일경우에만 2D 이펙트가 나온다.				  
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
							if( !pDest->m_State.bRide )	//라이딩 중이 아니라면
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
					if( !pDest->m_State.bRide )	//라이딩 중이 아니라면
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
				
				// 다른 유저 HP 인터페이스 데미지 처리 
				
			}
		}
		
		// 부가효과에 따른 데미지 적용 및 각각의 수치 및 상태 변화 처리 
		// g_Pc.m_Pc[skilledIdx] <-- 요놈의 해당하는 캐릭터 수치에 넣어서 업데이트 시켜준다 
	}
	else 
		//............................................................................................................
		// 3. NPC 맞았을때 처리 맞았을때 처리 by wxywxy
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
					
					///-- 공격 주체가 나일경우에만 2D 이펙트가 나온다.
					if( pNpcTable && NonPlayerInfoWnd.IsVisible && pSrc == g_Pc.GetPlayer() )
					{
						NonPlayerInfoWnd.Start2DEffect(pNpcTable->theHP, 
							pExtraInfo->TargetInfo, pExtraInfo->SkillCode);	   		
					}			
				}			
				
				g_Pc.theSkillSystem.UseSkillTargetInfoMgr( pSrc, pExtraInfo->TargetInfo, pExtraInfo->SkillCode, TRUE );					
				g_Pc.theSkillSystem.UseSkillTargetInfoMgr_AddEffect( pSrc, pExtraInfo->TargetInfo, pExtraInfo->SkillCode, TRUE );
				
				//...................................................................................................
				// 데미지 처리시 흔들리지 말아야 할것들 By wxywxy
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
					
					// 죽을때 가지고 있던 이펙트 지우기
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
				
				
				// 몬스터 HP 인터페이스 데미지 처리
				
		}
		
		
		
		// 부가효과에 따른 데미지 적용 및 각각의 수치 및 상태 변화 처리 
		// g_Pc.m_Pc[skilledIdx] <-- 요놈의 해당하는 캐릭터 수치에 넣어서 업데이트 시켜준다 
		
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
// 무기 장착 유무 ( 무기 왼손 : 무기 오른손 )
//................................................................................................................
int CEffectManager::IsNoneWeapon(Character *pSrc)
{
	if( pSrc == NULL )
		return FALSE;
	
	int RHand_index = pSrc->m_pUserInfo->ArmorInfo.index[0];
	int LHand_index = pSrc->m_pUserInfo->ArmorInfo.index[1];
	
	int HandType    = 0;
	
	
	if( RHand_index != -1 && LHand_index == -1 )				// 오른손 무기만 장착
	{		
		if( g_Pc_Manager.m_pItm[RHand_index]->m_type == 0 )		// 대검 
		{
			// 오른손
			HandType = n_WearSlot_Weapon;
		}
		else if( g_Pc_Manager.m_pItm[RHand_index]->m_type == 2 ) // 한손검 
		{
			// 오른손
			HandType = n_WearSlot_Weapon;
		}
		else if( g_Pc_Manager.m_pItm[RHand_index]->m_type == 6 ) // 지팡이 
		{
			// 오른손
			HandType = n_WearSlot_Weapon;
		}
		else if( g_Pc_Manager.m_pItm[RHand_index]->m_type == 7 ) // 흑수정
		{
			// 오른손
			HandType = n_WearSlot_Weapon;
		}
	}
	else if( RHand_index == -1 && LHand_index != -1 )			// 왼손 무기만 장착
	{
		if( g_Pc_Manager.m_pItm[LHand_index]->m_type == 1 )		// 활 
		{
			// 왼손
			HandType = n_WearSlot_Shield;
		}
		else if( g_Pc_Manager.m_pItm[LHand_index]->m_type == 3 ) // 방패 
		{
			// 왼손
			HandType = n_WearSlot_Shield;
		}
		else if( g_Pc_Manager.m_pItm[LHand_index]->m_type == 5 ) // 마법서 
		{
			// 왼손
			HandType = n_WearSlot_Shield;
		}
	}
	else if( RHand_index != -1 && LHand_index != -1 )			// 양손에 무기 장착
	{
		if( g_Pc_Manager.m_pItm[RHand_index]->m_type == 2 && 
			g_Pc_Manager.m_pItm[LHand_index]->m_type == 3 )		// 한손검 + 방패
		{
			//오른손
			HandType = n_WearSlot_Weapon;
		}
		else
		{
			//오른손
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
// 기본 공격시 처리 루틴 
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
	// 공격 주체 유저
	//............................................................................................................
	if( pSrc->m_bPC == TRUE || ( !pSrc->m_bPC && pSrc->m_State.nCharacterType == nCharacterTypePlayerNPC ))
	{
		// 공격 아이템 코드
		DWORD WeaponCode       = -1;
		
		// 플레이어가 양손에 무기 장착 상태가 아니면 공통 이펙트 사용
		if( -1 == ( WeaponCode = IsNoneWeapon( pSrc ) ) )
		{
			StandAttackHand( pSrc , pDest , EventType , Cell , IsKill , Damage , IsCritical );
			
			return;
		}
		
		//.......................................................................................................
		// 이펙트 베이스 만들기
		//.......................................................................................................
		
		int   AttackEffectCode = g_Pc_Manager.theItemInfoTable[ g_Pc_Manager.theItemCodeTable[ WeaponCode ] ].theAttackEff;
		
		// 공격 이펙트 유/무 체크
		if( -1 == AttackEffectCode )
			return;
		
		short AttackSound = g_Pc_Manager.theItemInfoTable[ g_Pc_Manager.theItemCodeTable[ WeaponCode ] ].theAttackSnd;
		
		// 공격 사운드 유/무 체크
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
		
		TempExtraInfo.TargetInfo.theEffectValue[ 0 ]   = Damage;					// 데미지
		
		
		CreateEffect( ( EFFECT_ID ) AttackEffectCode , pSrc , pDest , &TempExtraInfo );
	}
	//............................................................................................................
	// 공격 주체 NPC
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
		
		// 공격 사운드 유/무체크
		if( -1 != AttackSound )
			g_Sound.Play( AttackSound , NULL );
		
		// 공격 이펙트 유/무 체크
		if( -1 == AttackEffectCode )
			return;
		
		//....................................................................................................
		// NPC 공격시 기합 세개중 렌덤 ( 0 , 1 , 2 : 소리 안남 )
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
		
		// 이펙트 생성
		EFFECT_EXTRA_INFO	TempExtraInfo;
		
		TempExtraInfo.EventType = EventType;
		TempExtraInfo.SkillCode = ( g_Map.GetNpcTable( pSrc->GetUnique() ))->code; // NPC 코드
		TempExtraInfo.Critical  = IsCritical;
		TempExtraInfo.lStartTime = lStartTime;
		
		TempExtraInfo.TargetInfo.theTargetID         = pDest->GetUnique();
		
		if( pDest->m_bPC )
			TempExtraInfo.TargetInfo.theTargetType   = n_PC;
		else
			TempExtraInfo.TargetInfo.theTargetType   = n_NPC;
		
		TempExtraInfo.TargetInfo.theDieFlag          = IsKill;
		TempExtraInfo.TargetInfo.thePushedCell       = Cell;
		
		TempExtraInfo.TargetInfo.theEffectValue[ 0 ] = Damage;					// 데미지
		
		CreateEffect( ( EFFECT_ID ) AttackEffectCode , pSrc , pDest , &TempExtraInfo );
		
	}
}

//.................................................................................................................
// 실제 HP 얻어오기
//.................................................................................................................
DWORD	CEffectManager::GetRealHP(Character *pSrc)
{
	if( pSrc == NULL )
		return FALSE;
	
	// PC 일때
	if( TRUE == pSrc->m_bPC )
	{
		return ((PCTABLE *)g_Pc.FindPcTable( pSrc ))->char_info.theRealHP;
	}
	
	// NPC 일때
	return ( g_Map.GetNpcTable( pSrc->GetUnique() ))->theRealHP;
}

//.................................................................................................................
// 화면 표시중인 HP 얻어오기
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
	
	// PC 일때
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
	
	// NPC 일때
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
// 데미지 레벨 ( 0 : 작은거 , 1 : 중간 , 2 : 크리티컬 )
//.................................................................................................................
EDamageLevel CEffectManager::DamageLevel(Character *pSrc,DWORD Damage,BOOL IsCritical)
{
	if( IsCritical ) 
	{
		return DL_CRITICAL;
	}
	
	DWORD theHP = 100.0f;							// 임시 나중에 정보 있을때 수정 By wxywxy
	
	float DamageLevel = (float) Damage / ( float ) GetHP( pSrc );
	
	if( DamageLevel > 0.3f )
	{
		return DL_STRONG;
	}
	
	return DL_WEAK;
}

//................................................................................................................
// 기본 손 공격 이펙트 처리 루틴 ( 유저만 적용 )
//................................................................................................................
void CEffectManager::StandAttackHand(Character *pSrc,Character *pDest,DWORD EventType,DWORD Cell,BOOL IsKill,DWORD Damage,BOOL IsCritical)	
{
	if( pSrc == NULL || pDest == NULL )
		return;
	
	// 공격 미스 처리
	if( Damage <= 0 )
	{
		// 데미지 찍을때 플레이어랑 연관있는것만 찍기
		if( IsMyRelationUnit( pSrc , pDest ) )
		{
			g_Particle.SetEffSetCon( 0.0f , EF_ATTACK_MISS , 0 , 0.3f , pDest->GetUnique() , pDest->GetIndex() );
		}
		// RLGS( "\r--- 손 공격 ( 실패 ) ---\r" );
		
		return;
	}
	
	EDamageLevel Level = DamageLevel( pDest , Damage , IsCritical );
	
	// 공격 이펙트 찍기
	
	// 데미지 숫자 찍기
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
	
	// 데미지 찍을때 플레이어랑 연관있는것만 찍기
	if( IsMyRelationUnit( pSrc , pDest ) )
	{
		g_Particle.m_DamageNum.Add( vHeight  , Damage , IsCritical, pDest );
	}

	// 데미지 이펙트 찍기
	switch( Level )
	{
		// 데미지 약
	case DL_WEAK:
		
		//			RLGS( "\r--- 손 공격 ( 약 ) ---\r" );
		g_Particle.SetEffSetCon( 0.0f , EF_HAND_ATTACKED0 , 0 , 0.4f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
		
		break;
		
	case DL_STRONG:
		
		//			RLGS( "\r--- 손 공격 ( 중 ) ---\r" );
		g_Particle.SetEffSetCon( 0.0f , EF_HAND_ATTACKED1 , 0 , 0.4f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
		
		break;
		
	case DL_CRITICAL:
		
		//			RLGS( "\r--- 손 공격 ( 강 ) ---\r" );
		g_Particle.SetEffSetCon( 0.0f , EF_HAND_ATTACKED2 , 0 , 0.4f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
		
		break;
	}
	
	// 데미지 처리
	EVENT_DATA_INFO underattack_event;	
	
	//............................................................................................................
	// 이벤트 처리
	//............................................................................................................
	
	// 슬라이딩
	if( Cell )
	{
		underattack_event.type = SM_SLIDE_NPC;
		underattack_event.cell = Cell;
	}
	else
	{
		// 죽기
		if( IsKill )
		{					
			underattack_event.type = SM_STATE_DIE;
			underattack_event.loop = false;
		}
		// 기본 피타격
		else
		{
			underattack_event.type = SM_UNDERATTACK;
		}
	}
	
	// 유저 처리
	if( pDest->m_bPC )
	{
		// 플레이어 처리
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
			
			///-- by simwoosung 대미지가 존재하고 피값이 다르며
			///--JAPAN_BUG_MODIFY
			///-- by simwoosung 현재 죽어있지 않는경우에만 적용시킨다.
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
					if( !g_Pc.GetPlayer()->m_State.bRide )	//라이딩 중이 아니라면
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
				
				if( !g_Pc.GetPlayer()->m_State.bRide )	//라이딩 중이 아니라면
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
					///-- 죽었는데 또 죽으라고 메시지를 처리해야할때
					return;
				}

				underattack_event.motion = n_Die9;
				
				g_Pc.GetPlayer()->PushQueue( underattack_event, TRUE );
				
				nRui->thePcParam.Stat1.theHP = 0;
				g_Pc.m_MainPC.char_info.theHP = 0;
				g_Pc.m_MainPC.char_info.theRealHP = 0;
				g_Pc.GetPlayer()->m_lPreTime = g_nowTime;
				
				// 죽을때 가지고 있는 이펙트 모두 지우기
				DeletePlayerEffect( pDest );
				
				// 죽을때 효과
				g_Particle.SetEffSetCon( 0.0f , EF_PLAYER_DIE , 0 , 2.4f , pDest->GetUnique() , -1 );
				
				g_Pc.GetPlayer()->SetDirection( pSrc->GetPosTM()._41, pSrc->GetPosTM()._43 );
			}
			
			// 유저 크리티컬 공격 당했을때 화면 흔들어 Boa 요
			if( IsCritical )
				g_Particle.RockCamera( ( rand() % 200 ) + 400 );
			
		}
		// 다른 유저 처리
		else
		{
			PCTABLE *pTable = ((PCTABLE *)g_Pc.FindPcTable( pDest ));
			
			if( pDest->m_bIsLoaded == true )
			{
				//by simwoosung
				int index = g_Pc.FindUser(pDest->GetUnique());
				
				if( index == NonPlayerInfoWnd.thePickedUserID )
				{
					///공격 주체가 나일경우에만 2D 이펙트가 나온다.
					if( NonPlayerInfoWnd.IsVisible && pSrc == g_Pc.GetPlayer() )
					{
						int RealHP = pTable->char_info.theHP - Damage;	
						NonPlayerInfoWnd.Start2DEffect(pTable->char_info.theHP, 
							RealHP);	   		
					}
				}				
				
				///--JAPAN_BUG_MODIFY
				///-- by simwoosung 현재 죽어있지 않는경우에만 적용시킨다.
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
						if( !pDest->m_State.bRide )	//라이딩 중이 아니라면
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
					if( !pDest->m_State.bRide )	//라이딩 중이 아니라면
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
					
					// 죽을때 가지고 있는 이펙트 모두 지우기
					DeleteOtherPlayerEffect( pDest );
					
					// 죽을때 효과
					g_Particle.SetEffSetCon( 0.0f , EF_PLAYER_DIE , 0 , 2.4f , pDest->GetUnique() , -1 );
					(( NonPlayer * )pDest)->SetDirection( pSrc->GetPosTM()._41, pSrc->GetPosTM()._43 );
				}				
			} 			
		}
		
		//PC가 맞았을때 펫 마스터일경우 - 방어형 펫을 가지고 있을시에 
		//방어형 펫에도 맞는 모션을 넣어준다.
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
	// NPC 처리
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
				///공격 주체가 나일경우에만 2D 이펙트가 나온다.
				if( NonPlayerInfoWnd.IsVisible &&  pSrc == g_Pc.GetPlayer() )
				{
					int RealHP = pNpcTable->theHP - Damage;	
					NonPlayerInfoWnd.Start2DEffect(pNpcTable->theHP, RealHP);	   		
				}
			}			
			
			///--JAPAN_BUG_MODIFY
			///-- by simwoosung 현재 죽어있지 않는경우에만 적용시킨다.
			if( !(pDest->IsRealDie()) )
			{
				pDest->m_lPreTime = g_nowTime;
				pNpcTable->theHP -= Damage;
			}	
			
			//...................................................................................................
			// 데미지 처리시 흔들리지 말아야 할것들 By wxywxy
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
				
				// 죽을때 가지고 있는 이펙트 모두 지우기
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

	for( int i = 0; i < 3; ++i )	// 스킬 효력 처리 3가지  
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
			case n_AttackBlowAdd:			// 물리 필살기 
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
	
	// 공격 미스 처리
	if( !(pExtraInfo->TargetInfo.theDieFlag) && dTotalDamage <= 0 && ISDisplayDamageNum )
	{
		// 데미지 찍을때 플레이어랑 연관있는것만 찍기
		if( IsMyRelationUnit( NULL , pDest ) )
		{
			g_Particle.SetEffSetCon( 0.0f , EF_ATTACK_MISS , 0 , 0.3f , 
									 pDest->GetUnique() , pDest->GetIndex() );
		}
	}
	
	EDamageLevel Level = DamageLevel( pDest , dTotalDamage , pExtraInfo->Critical );
	
	// 데미지 숫자 찍기
	
	// 데미지 숫자 찍기
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
	
	// 데미지 찍을때 플레이어랑 연관있는것만 찍기
	if( IsMyRelationUnit( NULL , pDest ) && dTotalDamage > 0 && ISDisplayDamageNum )
	{
		DWORD DamageRange = dTotalDamage;		
		int nDamage = 0;
		
		// 서버에서 수정해주기 전까지만...임시 작업 위에다 정의 함 .. 
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
		// 1. 내 캐릭터가 맞았을때 처리 by wxywxy
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
							
							if( !g_Pc.GetPlayer()->m_State.bRide )	//라이딩 중이 아니라면
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
					if( !g_Pc.GetPlayer()->m_State.bRide )	//라이딩 중이 아니라면
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
					
					// 플레이어 죽었을때 이펙트 지우기
					DeletePlayerEffect( pDest );
				}				
			}
			
			
			// 부가효과에 따른 데미지 적용 및 각각의 수치 및 상태 변화 처리 
		}
		//........................................................................................................
		// 2. 다른 유저가 맞았을때 처리 맞았을때 처리 by wxywxy
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
							if( !pDest->m_State.bRide )	//라이딩 중이 아니라면
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
					if( !pDest->m_State.bRide )	//라이딩 중이 아니라면
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
				// 다른 유저 HP 인터페이스 데미지 처리 				
			}
		}
	}
	else 
	{
		//............................................................................................................
		// 3. NPC 맞았을때 처리 맞았을때 처리 by wxywxy
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
				// 데미지 처리시 흔들리지 말아야 할것들 By wxywxy
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

					// 죽을때 가지고 있던 이펙트 지우기
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
// 기본 공격 타격 처리
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

	// 공격 미스 처리
	if( !(pExtraInfo->TargetInfo.theDieFlag) && pExtraInfo->TargetInfo.theEffectValue[ 0 ] <= 0 )
	{
		// 데미지 찍을때 플레이어랑 연관있는것만 찍기
		if( IsMyRelationUnit( pSrc , pDest ) )
		{
			g_Particle.SetEffSetCon( 0.0f , EF_ATTACK_MISS , 0 , 0.3f , pDest->GetUnique() , pDest->GetIndex() );
		}	
	}
	
	EDamageLevel Level = DamageLevel( pDest , pExtraInfo->TargetInfo.theEffectValue[ 0 ] , pExtraInfo->Critical );
	
	// 데미지 숫자 찍기
	
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
	
	// 데미지 찍을때 플레이어랑 연관있는것만 찍기
	if( IsMyRelationUnit( pSrc , pDest ) && 
		pExtraInfo->TargetInfo.theEffectValue[ 0 ] > 0 )
	{
		g_Particle.m_DamageNum.Add( vHeight  , pExtraInfo->TargetInfo.theEffectValue[ 0 ] , 
		pExtraInfo->Critical, pDest );
	}	
	
	EVENT_DATA_INFO underattack_event;	
	
	//............................................................................................................
	// 이벤트 처리
	//............................................................................................................
	
	// 슬라이딩
	if( pExtraInfo->TargetInfo.thePushedCell )
	{
		underattack_event.type = SM_SLIDE_NPC;
		underattack_event.cell = pExtraInfo->TargetInfo.thePushedCell;
	}
	else
	{
		// 죽기
		if( pExtraInfo->TargetInfo.theDieFlag )
		{					
			//			RLGS("pExtraInfo.TargetInfo.theDieFlag underattack_event.type = SM_STATE_DIE;");
			underattack_event.type = SM_STATE_DIE;
			underattack_event.loop = false;
		}
		// 기본 피타격
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
	// 타격 처리
	//............................................................................................................
	int   AttackedEffectCode = -1;
	short AttackedSound      = -1;
	
	// 유저 처리
	if( pDest->m_bPC )
	{
		// 플레이어 처리
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
			///-- by simwoosung 현재 죽어있지 않는경우에만 적용시킨다.
			if( !(pDest->IsRealDie()) )
			{
				nRui->thePcParam.Stat1.theHP -= pExtraInfo->TargetInfo.theEffectValue[ 0 ];
				
				//PRINT_DLG_LOG("나 그냥 맞음 적용 theHP - %d 대미지 - %d 때린놈 아이디 - %d", 
				//nRui->thePcParam.Stat1.theHP, pExtraInfo->TargetInfo.theEffectValue[ 0 ], 
				//g_Map.m_NpcTable[ pSrc->m_nTableIndex ].lUnique );
				
				g_Pc.GetPlayer()->m_lPreTime = g_nowTime;
			}		
			
			// dongs 피튐 현상 버그 수정 .. 
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
					if( !g_Pc.GetPlayer()->m_State.bRide )	//라이딩 중이 아니라면
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
				if( !g_Pc.GetPlayer()->m_State.bRide )	//라이딩 중이 아니라면
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
					///-- 죽었는데 또 죽으라고 메시지를 처리해야할때
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
				
				// 죽을때 가지고 있는 이펙트 모두 지우기
				DeletePlayerEffect( pDest );
				
				// 죽을때 효과
				g_Particle.SetEffSetCon( 0.0f , EF_PLAYER_DIE , 0 , 2.4f , pDest->GetUnique() , -1 );
				g_Pc.GetPlayer()->SetDirection( pSrc->GetPosTM()._41, pSrc->GetPosTM()._43 );
			}			
			
			// 유저 크리티컬 공격 당했을때 화면 흔들어 Boa 요
			if( pExtraInfo->Critical )
				g_Particle.RockCamera( ( rand() % 200 ) + 400 );
			
		}
		// 다른 유저 처리
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
					///공격 주체가 나일경우에만 2D 이펙트가 나온다.
					if( NonPlayerInfoWnd.IsVisible &&  pSrc == g_Pc.GetPlayer() )
					{
						int RealHP = pTable->char_info.theHP - pExtraInfo->TargetInfo.theEffectValue[ 0 ];
						NonPlayerInfoWnd.Start2DEffect(pTable->char_info.theHP, RealHP);	   		
					}
				}		
				
				///--JAPAN_BUG_MODIFY
				///-- by simwoosung 현재 죽어있지 않는경우에만 적용시킨다.
				if( !(pDest->IsRealDie()) )
				{
					pDest->m_lPreTime = g_nowTime;
					pTable->char_info.theHP -= pExtraInfo->TargetInfo.theEffectValue[ 0 ];	
				}								
				
				//dongs 피버그 수정 ... 피튐 현상 .. 가능 .. 
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
						if( !pDest->m_State.bRide )	//라이딩 중이 아니라면
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
					if( !pDest->m_State.bRide )	//라이딩 중이 아니라면
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
					
					// 죽을때 가지고 있는 이펙트 모두 지우기
					DeleteOtherPlayerEffect( pDest );
					
					// 죽을때 효과
					g_Particle.SetEffSetCon( 0.0f , EF_PLAYER_DIE , 0 , 2.4f , pDest->GetUnique() , -1 );
					(( NonPlayer * )pDest)->SetDirection( pSrc->GetPosTM()._41, pSrc->GetPosTM()._43 );
				}
				
			} 			
		}
		
		//PC가 맞았을때 펫 마스터일경우 - 방어형 펫을 가지고 있을시에 
		//방어형 펫에도 맞는 모션을 넣어준다.
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
	// NPC 처리
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
				///공격 주체가 나일경우에만 2D 이펙트가 나온다.
				if( NonPlayerInfoWnd.IsVisible &&  pSrc == g_Pc.GetPlayer() )
				{
					int RealHP = pNpcTable->theHP - pExtraInfo->TargetInfo.theEffectValue[ 0 ];
					NonPlayerInfoWnd.Start2DEffect(pNpcTable->theHP, RealHP);	   		
				}
			}
			
			///--JAPAN_BUG_MODIFY
			///-- by simwoosung 현재 죽어있지 않는경우에만 적용시킨다.
			if( !(pDest->IsRealDie()) )
			{
				pDest->m_lPreTime = g_nowTime;
				pNpcTable->theHP -= pExtraInfo->TargetInfo.theEffectValue[ 0 ];
			}	
			
			//...................................................................................................
			// 데미지 처리시 흔들리지 말아야 할것들 By wxywxy
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
				
				// 죽을때 가지고 있는 이펙트 모두 지우기 ( NPC는 지금은 없어도 됨 )
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
	// 타격 이펙트
	//............................................................................................................
	
	// 유저 가 공격
	if( pSrc->m_bPC || ( !pSrc->m_bPC && pSrc->m_State.nCharacterType == nCharacterTypePlayerNPC ) )
	{
		switch( Level )
		{
			// 데미지 약
		case DL_WEAK:
			
			AttackedEffectCode = g_Pc_Manager.theItemInfoTable[ g_Pc_Manager.theItemCodeTable[ pExtraInfo->SkillCode ] ].theAttackSuccEff1;
			AttackedSound      = g_Pc_Manager.theItemInfoTable[ g_Pc_Manager.theItemCodeTable[ pExtraInfo->SkillCode ] ].theAttackSuccSnd1;
			
			//			RLGS( "\r--- 유저 기본 공격에 맞음 ( 약 ) ---\r" );
			
			
			break;
			
		case DL_STRONG:
			
			AttackedEffectCode = g_Pc_Manager.theItemInfoTable[ g_Pc_Manager.theItemCodeTable[ pExtraInfo->SkillCode ] ].theAttackSuccEff2;
			AttackedSound      = g_Pc_Manager.theItemInfoTable[ g_Pc_Manager.theItemCodeTable[ pExtraInfo->SkillCode ] ].theAttackSuccSnd2;
			
			//			RLGS( "\r--- 유저 기본 공격에 맞음 ( 중 ) ---\r" );
			
			break;
			
		case DL_CRITICAL:
			
			AttackedEffectCode = g_Pc_Manager.theItemInfoTable[ g_Pc_Manager.theItemCodeTable[ pExtraInfo->SkillCode ] ].theAttackSuccEff3;
			AttackedSound      = g_Pc_Manager.theItemInfoTable[ g_Pc_Manager.theItemCodeTable[ pExtraInfo->SkillCode ] ].theAttackSuccSnd3;
			
			//			RLGS( "\r--- 유저 기본 공격에 맞음 ( 강 ) ---\r" );
			
			break;
		}
	}
	else
	{
		// 데미지 이펙트 생성
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
			// 데미지 약
		case DL_WEAK:
			
			AttackedEffectCode = npc_Info->theAttackedEff1;
			AttackedSound      = npc_Info->theAttackedSnd1;
			
			//				RLGS( "\r--- NPC 기본 공격에 맞음 ( 약 ) ---\r" );
			
			break;
			
		case DL_STRONG:
			
			AttackedEffectCode = npc_Info->theAttackedEff2;
			AttackedSound      = npc_Info->theAttackedSnd2;
			
			//				RLGS( "\r--- NPC 기본 공격에 맞음 ( 중 ) ---\r" );
			
			break;
			
		case DL_CRITICAL:
			
			AttackedEffectCode = npc_Info->theAttackedEff3;
			AttackedSound      = npc_Info->theAttackedSnd3;
			
			//				RLGS( "\r--- NPC 기본 공격에 맞음 ( 강 ) ---\r" );
			
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
// 기본 공격 타격 처리 ( 데미지 없음 & 효과 있음 )
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
	
	// 공격 미스 처리
	if( !(pExtraInfo->TargetInfo.theDieFlag) && pExtraInfo->TargetInfo.theEffectValue[ 0 ] <= 0 )
	{
		//		RLGS( "\r--- 기본 공격 ( 실패 ) ---\r" );
		
		return;
	}
	
	//............................................................................................................
	// 타격 이펙트
	//............................................................................................................
	EDamageLevel Level = DamageLevel( pDest , pExtraInfo->TargetInfo.theEffectValue[ 0 ] , pExtraInfo->Critical );
	int   AttackedEffectCode = -1;
	short AttackedSound      = -1;
	
	// 유저 가 공격
	if( pSrc->m_bPC )
	{
		switch( Level )
		{
			// 데미지 약
		case DL_WEAK:
			
			AttackedEffectCode = g_Pc_Manager.theItemInfoTable[ g_Pc_Manager.theItemCodeTable[ pExtraInfo->SkillCode ] ].theAttackSuccEff1;
			AttackedSound      = g_Pc_Manager.theItemInfoTable[ g_Pc_Manager.theItemCodeTable[ pExtraInfo->SkillCode ] ].theAttackSuccSnd1;
			
			//			RLGS( "\r--- 유저 기본 공격에 맞음 ( 약 ) ---\r" );
			
			
			break;
			
		case DL_STRONG:
			
			AttackedEffectCode = g_Pc_Manager.theItemInfoTable[ g_Pc_Manager.theItemCodeTable[ pExtraInfo->SkillCode ] ].theAttackSuccEff2;
			AttackedSound      = g_Pc_Manager.theItemInfoTable[ g_Pc_Manager.theItemCodeTable[ pExtraInfo->SkillCode ] ].theAttackSuccSnd2;
			
			//			RLGS( "\r--- 유저 기본 공격에 맞음 ( 중 ) ---\r" );
			
			break;
			
		case DL_CRITICAL:
			
			AttackedEffectCode = g_Pc_Manager.theItemInfoTable[ g_Pc_Manager.theItemCodeTable[ pExtraInfo->SkillCode ] ].theAttackSuccEff3;
			AttackedSound      = g_Pc_Manager.theItemInfoTable[ g_Pc_Manager.theItemCodeTable[ pExtraInfo->SkillCode ] ].theAttackSuccSnd3;
			
			//			RLGS( "\r--- 유저 기본 공격에 맞음 ( 강 ) ---\r" );
			
			break;
		}
	}
	// NPC 가 공격
	else
	{
		// 데미지 이펙트 생성
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
			// 데미지 약
		case DL_WEAK:
			
			AttackedEffectCode = npc_Info->theAttackedEff1;
			AttackedSound      = npc_Info->theAttackedSnd1;
			
			//				RLGS( "\r--- NPC 기본 공격에 맞음 ( 약 ) ---\r" );
			
			break;
			
		case DL_STRONG:
			
			AttackedEffectCode = npc_Info->theAttackedEff2;
			AttackedSound      = npc_Info->theAttackedSnd2;
			
			//				RLGS( "\r--- NPC 기본 공격에 맞음 ( 중 ) ---\r" );
			
			break;
			
		case DL_CRITICAL:
			
			AttackedEffectCode = npc_Info->theAttackedEff3;
			AttackedSound      = npc_Info->theAttackedSnd3;
			
			//				RLGS( "\r--- NPC 기본 공격에 맞음 ( 강 ) ---\r" );
			
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
	
	// 공격 미스 처리
	if( !(pExtraInfo->TargetInfo.theDieFlag) && pExtraInfo->TargetInfo.theEffectValue[ 0 ] <= 0 )
	{
		return;
	}
	
	EDamageLevel Level = DamageLevel( pDest , pExtraInfo->TargetInfo.theEffectValue[ 0 ] , pExtraInfo->Critical );
	
	EVENT_DATA_INFO underattack_event;	
	
	//............................................................................................................
	// 이벤트 처리
	//............................................................................................................
	
	// 슬라이딩
	if( pExtraInfo->TargetInfo.thePushedCell )
	{
		underattack_event.type = SM_SLIDE_NPC;
		underattack_event.cell = pExtraInfo->TargetInfo.thePushedCell;
	}
	else
	{
		// 죽기
		if( pExtraInfo->TargetInfo.theDieFlag )
		{					
			//			RLGS("pExtraInfo.TargetInfo.theDieFlag underattack_event.type = SM_STATE_DIE;");
			underattack_event.type = SM_STATE_DIE;
			underattack_event.loop = false;
		}
		// 기본 피타격
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
	// 타격 처리
	//............................................................................................................
	int   AttackedEffectCode = -1;
	short AttackedSound      = -1;
	
	// 유저 처리
	if( pDest->m_bPC )
	{
		// 플레이어 처리
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
			///-- by simwoosung 현재 죽어있지 않는경우에만 적용시킨다.
			if( !(pDest->IsRealDie()) )
			{
				nRui->thePcParam.Stat1.theHP -= pExtraInfo->TargetInfo.theEffectValue[ 0 ];
				g_Pc.GetPlayer()->m_lPreTime = g_nowTime;
			}
			
			// dongs 피튐 현상 버그 수정 .. 
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
					if( !g_Pc.GetPlayer()->m_State.bRide )	//라이딩 중이 아니라면
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
				if( !g_Pc.GetPlayer()->m_State.bRide )	//라이딩 중이 아니라면
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
				
				// 죽을때 가지고 있는 이펙트 모두 지우기
				DeletePlayerEffect( pDest );
				
				// 죽을때 효과
				g_Particle.SetEffSetCon( 0.0f , EF_PLAYER_DIE , 0 , 2.4f , pDest->GetUnique() , -1 );			
			}			
			
			// 유저 크리티컬 공격 당했을때 화면 흔들어 Boa 요
			if( pExtraInfo->Critical )
				g_Particle.RockCamera( ( rand() % 200 ) + 400 );
			
		}
		// 다른 유저 처리
		else
		{
			PCTABLE *pTable = ((PCTABLE *)g_Pc.FindPcTable( pDest ));
			
			if( pDest->m_bIsLoaded == true )
			{
				///--JAPAN_BUG_MODIFY
				///-- by simwoosung 현재 죽어있지 않는경우에만 적용시킨다.
				if( !(pDest->IsRealDie()) )
				{
					pDest->m_lPreTime = g_nowTime;
					pTable->char_info.theHP -= pExtraInfo->TargetInfo.theEffectValue[ 0 ];	
				}	
				
				//dongs 피버그 수정 ... 피튐 현상 .. 가능 .. 
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
						if( !pDest->m_State.bRide )	//라이딩 중이 아니라면
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
					if( !pDest->m_State.bRide )	//라이딩 중이 아니라면
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
					
					// 죽을때 가지고 있는 이펙트 모두 지우기
					DeleteOtherPlayerEffect( pDest );
					
					// 죽을때 효과
					g_Particle.SetEffSetCon( 0.0f , EF_PLAYER_DIE , 0 , 2.4f , pDest->GetUnique() , -1 );				
				}
				
			} 			
		}
		
		//PC가 맞았을때 펫 마스터일경우 - 방어형 펫을 가지고 있을시에 
		//방어형 펫에도 맞는 모션을 넣어준다.
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
	// NPC 처리
	else
	{
		if( TRUE == pDest->m_bIsLoaded )
		{
			NPCTABLE *pNpcTable;
			
			pNpcTable = g_Map.GetNpcTable( pDest->GetUnique() );
			
			if( pNpcTable != NULL )
			{
				///--JAPAN_BUG_MODIFY
				///-- by simwoosung 현재 죽어있지 않는경우에만 적용시킨다.
				if( !(pDest->IsRealDie()) )
				{
					pDest->m_lPreTime = g_nowTime;
					pNpcTable->theHP -= pExtraInfo->TargetInfo.theEffectValue[ 0 ];
				}	
			}
			//...................................................................................................
			// 데미지 처리시 흔들리지 말아야 할것들 By wxywxy
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
				
				// 죽을때 가지고 있는 이펙트 모두 지우기 ( NPC는 지금은 없어도 됨 )
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

// 기본 공격 타격 처리 ( 데미지 있음 & 효과 없음 )
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
	
	// 공격 미스 처리
	if( !(pExtraInfo->TargetInfo.theDieFlag) && pExtraInfo->TargetInfo.theEffectValue[ 0 ] <= 0 )
	{
		// 데미지 찍을때 플레이어랑 연관있는것만 찍기
		if( IsMyRelationUnit( pSrc , pDest ) )
			g_Particle.SetEffSetCon( 0.0f , EF_ATTACK_MISS , 0 , 0.3f , pDest->GetUnique() , pDest->GetIndex() );
		
		//		RLGS( "\r--- 기본 공격 ( 실패 ) ---\r" );
		
		return;
	}
	
	EDamageLevel Level = DamageLevel( pDest , pExtraInfo->TargetInfo.theEffectValue[ 0 ] , pExtraInfo->Critical );
	
	// 데미지 숫자 찍기
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
	
	// 데미지 찍을때 플레이어랑 연관있는것만 찍기
	
	if( IsMyRelationUnit( pSrc , pDest ) )
		g_Particle.m_DamageNum.Add( vHeight  , pExtraInfo->TargetInfo.theEffectValue[ 0 ] , 
		pExtraInfo->Critical, pDest );
	
	
	
	
	EVENT_DATA_INFO underattack_event;	
	
	//............................................................................................................
	// 이벤트 처리
	//............................................................................................................
	
	// 슬라이딩
	if( pExtraInfo->TargetInfo.thePushedCell )
	{
		underattack_event.type = SM_SLIDE_NPC;
		underattack_event.cell = pExtraInfo->TargetInfo.thePushedCell;
	}
	else
	{
		// 죽기
		if( pExtraInfo->TargetInfo.theDieFlag )
		{					
			//			RLGS("pExtraInfo.TargetInfo.theDieFlag underattack_event.type = SM_STATE_DIE;");
			underattack_event.type = SM_STATE_DIE;
			underattack_event.loop = false;
		}
		// 기본 피타격
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
	// 타격 처리
	//............................................................................................................
	int   AttackedEffectCode = -1;
	short AttackedSound      = -1;
	
	// 유저 처리
	if( pDest->m_bPC )
	{
		// 플레이어 처리
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
			///-- by simwoosung 현재 죽어있지 않는경우에만 적용시킨다.
			if( !(pDest->IsRealDie()) )
			{
				nRui->thePcParam.Stat1.theHP -= pExtraInfo->TargetInfo.theEffectValue[ 0 ];
				g_Pc.GetPlayer()->m_lPreTime = g_nowTime;
			}
			
			// dongs 피튐 현상 버그 수정 .. 
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
					if( !g_Pc.GetPlayer()->m_State.bRide )	//라이딩 중이 아니라면
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
				if( !g_Pc.GetPlayer()->m_State.bRide )	//라이딩 중이 아니라면
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
					///-- 죽었는데 또 죽으라고 메시지를 처리해야할때
					return;
				}

				underattack_event.motion = n_Die9;			
				
				g_Pc.GetPlayer()->PushQueue( underattack_event, TRUE );									
				
				nRui->thePcParam.Stat1.theHP = 0;
				g_Pc.m_MainPC.char_info.theHP = 0;
				g_Pc.m_MainPC.char_info.theRealHP = 0;
				g_Pc.GetPlayer()->m_lPreTime = g_nowTime;
				
				// 죽을때 가지고 있는 이펙트 모두 지우기
				DeletePlayerEffect( pDest );
				
				// 죽을때 효과
				g_Particle.SetEffSetCon( 0.0f , EF_PLAYER_DIE , 0 , 2.4f , pDest->GetUnique() , -1 );
				g_Pc.GetPlayer()->SetDirection( pSrc->GetPosTM()._41, pSrc->GetPosTM()._43 );
			}
			
			
			
			// 유저 크리티컬 공격 당했을때 화면 흔들어 Boa 요
			if( pExtraInfo->Critical )
				g_Particle.RockCamera( ( rand() % 200 ) + 400 );
			
		}
		// 다른 유저 처리
		else
		{
			PCTABLE *pTable = ((PCTABLE *)g_Pc.FindPcTable( pDest ));
			
			if( pDest->m_bIsLoaded == true )
			{
				//by simwoosung
				int index = g_Pc.FindUser(pDest->GetUnique());
				if( index == NonPlayerInfoWnd.thePickedUserID )
				{
					///-- 공격 주체가 나일경우에만 2D 이펙트가 나온다.
					if( NonPlayerInfoWnd.IsVisible &&  pSrc == g_Pc.GetPlayer() )
					{
						int RealHP = pTable->char_info.theHP - pExtraInfo->TargetInfo.theEffectValue[ 0 ];
						NonPlayerInfoWnd.Start2DEffect(pTable->char_info.theHP, RealHP);	   		
					}
				}		
				
				///--JAPAN_BUG_MODIFY
				///-- by simwoosung 현재 죽어있지 않는경우에만 적용시킨다.
				if( !(pDest->IsRealDie()) )
				{
					pDest->m_lPreTime = g_nowTime;
					pTable->char_info.theHP -= pExtraInfo->TargetInfo.theEffectValue[ 0 ];	
				}	
				
				//dongs 피버그 수정 ... 피튐 현상 .. 가능 .. 
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
						if( !pDest->m_State.bRide )	//라이딩 중이 아니라면
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
					if( !pDest->m_State.bRide )	//라이딩 중이 아니라면
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
					
					// 죽을때 가지고 있는 이펙트 모두 지우기
					DeleteOtherPlayerEffect( pDest );
					
					// 죽을때 효과
					g_Particle.SetEffSetCon( 0.0f , EF_PLAYER_DIE , 0 , 2.4f , pDest->GetUnique() , -1 );
					(( NonPlayer * )pDest)->SetDirection( pSrc->GetPosTM()._41, pSrc->GetPosTM()._43 );
				}
				
			} 			
		}
		
		//PC가 맞았을때 펫 마스터일경우 - 방어형 펫을 가지고 있을시에 
		//방어형 펫에도 맞는 모션을 넣어준다.
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
	// NPC 처리
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
					///-- 공격 주체가 나일경우에만 2D 이펙트가 나온다.
					if( NonPlayerInfoWnd.IsVisible &&  pSrc == g_Pc.GetPlayer() )
					{
						int RealHP = pNpcTable->theHP - pExtraInfo->TargetInfo.theEffectValue[ 0 ];
						NonPlayerInfoWnd.Start2DEffect(pNpcTable->theHP, RealHP);	   		
					}
				}
				
				///--JAPAN_BUG_MODIFY
				///-- by simwoosung 현재 죽어있지 않는경우에만 적용시킨다.
				if( !(pDest->IsRealDie()) )
				{
					pDest->m_lPreTime = g_nowTime;
					pNpcTable->theHP -= pExtraInfo->TargetInfo.theEffectValue[ 0 ];
				}	
			}
			//...................................................................................................
			// 데미지 처리시 흔들리지 말아야 할것들 By wxywxy
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
				
				// 죽을때 가지고 있는 이펙트 모두 지우기 ( NPC는 지금은 없어도 됨 )
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
// 기본 대검 공격
//...............................................................................................................
int CEffectManager::MAKE_EF_BIG_SWORD(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	StandTargetHit( pSrc , pDest , pExtraInfo );
	
	return TRUE;
}

//...............................................................................................................
// 기본 칼 공격
//...............................................................................................................
int CEffectManager::MAKE_EF_STAND_SWORD_ATTACK(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	StandTargetHit( pSrc , pDest , pExtraInfo );
	
	return TRUE;
}

//...............................................................................................................
// 머록스 나무 정령 ( 공격 )
//...............................................................................................................
int CEffectManager::MAKE_EF_NPC_WOOD_ATTACK(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	StandTargetHit( pSrc , pDest , pExtraInfo );
	
	return TRUE;
}


//...............................................................................................................
// 머록스 나무 정령 ( 데미지 )
//...............................................................................................................
int CEffectManager::MAKE_EF_NPC_WOOD_ATTACKED(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pDest == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 0.3f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
	
	return TRUE;
}



//...............................................................................................................
// 기본 책 공격
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
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// 소스 , 타켓 지정
	
	PushEffect( &pData );												// 메니저에 넣기
	
	return TRUE;
}

//...............................................................................................................
// 기본 지팡이 공격
//...............................................................................................................
int CEffectManager::MAKE_EF_STAND_STICK_ATTACK(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	CEffectBase	*pData = SAFE_NEW( CEFStickAttack );					// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );						// 소스 , 타켓 지정
	
	PushEffect( &pData );												// 메니저에 넣기
	
	return TRUE;
}

//...............................................................................................................
// 기본 이빨 공격
//...............................................................................................................
int CEffectManager::MAKE_EF_STAND_TOOTH_ATTACK(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	StandTargetHit( pSrc , pDest , pExtraInfo );
	
	return TRUE;
}

//...............................................................................................................
// 기본 둔기 공격
//...............................................................................................................
int CEffectManager::MAKE_EF_STAND_DULL_ATTACK(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	StandTargetHit( pSrc , pDest , pExtraInfo );
	
	return TRUE;
}

//...............................................................................................................
// 기본 발톱 공격
//...............................................................................................................
int CEffectManager::MAKE_EF_STAND_NAIL_ATTACK(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	StandTargetHit( pSrc , pDest , pExtraInfo );
	
	return TRUE;
}

//...............................................................................................................
// 	대검 칼날 기본 데미지 ( 약 )
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
// 	대검 기본 데미지 ( 중 )
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
// 	대검 기본 데미지 ( 강 )
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
// 	칼날 기본 데미지 ( 약 )
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
// 	칼날 기본 데미지 ( 중 )
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
// 	칼날 기본 데미지 ( 강 )
//...............................................................................................................
int CEffectManager::MAKE_EF_STAND_SWORD_ATTACKED2(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL || pDest == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , ESLT_SRC_RAD , 0.2f , pDest->GetUnique() , pDest->GetIndex() , 
		::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) , pSrc->GetDirection() + D3DXToRadian( 180 ) );
	
		/*
		if( pSrc->m_ani_index == 두방모션 )
		{
		g_Particle.SetEffSetCon( 0.5f , id , ESLT_SRC_RAD , 0.3f , pDest->GetUnique() , pDest->GetIndex() , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) , pSrc->GetDirection() + D3DXToRadian( 180 ) );
		}
	*/
	
	return TRUE;
}

//...............................................................................................................
// 	마법기본공격(책) 데미지 ( 약 ) 
//...............................................................................................................
int CEffectManager::MAKE_EF_STAND_BOOK_ATTACKED0(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pDest == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 0.3f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
	
	return TRUE;
}

//...............................................................................................................
// 	마법기본공격(책) 데미지 ( 중 ) 
//...............................................................................................................
int CEffectManager::MAKE_EF_STAND_BOOK_ATTACKED1(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pDest == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 0.3f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
	
	return TRUE;
}

//...............................................................................................................
// 	마법기본공격(책) 데미지 ( 강 ) 
//...............................................................................................................
int CEffectManager::MAKE_EF_STAND_BOOK_ATTACKED2(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pDest == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 0.3f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
	
	return TRUE;
}

//...............................................................................................................
// 	기본 손 공격 ( 데미지 )
//...............................................................................................................
int CEffectManager::MAKE_EF_HAND_ATTACKED(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pDest == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 2.0f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
	
	return TRUE;
}

//...............................................................................................................
// 	기본 손 공격 
//...............................................................................................................
int CEffectManager::MAKE_EF_HAND_ATTACK0(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	StandTargetHit( pSrc , pDest , pExtraInfo );
	
	return TRUE;
}

//...............................................................................................................
// 	나이트 기본 공격
//...............................................................................................................
int CEffectManager::MAKE_EF_NIGHT_STAND_ATTACK(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	StandTargetHit( pSrc , pDest , pExtraInfo );
	
	return TRUE;
}

//...............................................................................................................
// 	나이트 기본 공격 데미지
//...............................................................................................................
int CEffectManager::MAKE_EF_NIGHT_STAND_ATTACKED(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pDest == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 2.0f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
	
	return TRUE;
}

//...............................................................................................................
// 	슬레이어 기본 공격
//...............................................................................................................
int CEffectManager::MAKE_EF_SLAYER_STAND_ATTACK(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	StandTargetHit( pSrc , pDest , pExtraInfo );
	
	return TRUE;
}

//...............................................................................................................
// 	슬레이어 기본 공격 데미지
//...............................................................................................................
int CEffectManager::MAKE_EF_SLAYER_STAND_ATTACKED(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pDest == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 2.0f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
	
	return TRUE;
}

//...............................................................................................................
// 	마법기본공격(지팡이) 데미지 ( 약 ) 
//...............................................................................................................
int CEffectManager::MAKE_EF_STAND_STICK_ATTACKED0(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pDest == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 1.1f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
	
	return TRUE;
}

//...............................................................................................................
// 	마법기본공격(지팡이) 데미지 ( 중 )
//...............................................................................................................
int CEffectManager::MAKE_EF_STAND_STICK_ATTACKED1(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer) 
{
	if( pDest == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 1.1f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
	
	return TRUE;
}

//...............................................................................................................
// 	마법기본공격(지팡이) 데미지 ( 강 )
//...............................................................................................................
int CEffectManager::MAKE_EF_STAND_STICK_ATTACKED2(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer) 
{
	if( pDest == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 1.1f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
	
	return TRUE;
}

//...............................................................................................................
// 	이빨 기본 공격   데미지 ( 약 ) 
//...............................................................................................................
int CEffectManager::MAKE_EF_STAND_TOOTH_ATTACKED0(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pDest == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 1.1f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
	
	return TRUE;
}

//...............................................................................................................
// 	이빨 기본 공격   데미지 ( 중 )
//...............................................................................................................
int CEffectManager::MAKE_EF_STAND_TOOTH_ATTACKED1(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pDest == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 1.1f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
	
	return TRUE;
}

//...............................................................................................................
// 	이빨 기본 공격   데미지 ( 강 )
//...............................................................................................................
int CEffectManager::MAKE_EF_STAND_TOOTH_ATTACKED2(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer) 
{
	if( pDest == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 1.1f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
	
	return TRUE;
}

//...............................................................................................................
// 	둔기 기본 공격   데미지 ( 약 ) 
//...............................................................................................................
int CEffectManager::MAKE_EF_STAND_DULL_ATTACKED0(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pDest == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 1.1f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
	
	return TRUE;
}

//...............................................................................................................
// 	둔기 기본 공격   데미지 ( 중 )
//...............................................................................................................
int CEffectManager::MAKE_EF_STAND_DULL_ATTACKED1(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer) 
{
	if( pDest == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 1.1f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
	
	return TRUE;
}

//...............................................................................................................
// 	둔기 기본 공격   데미지 ( 강 )
//...............................................................................................................
int CEffectManager::MAKE_EF_STAND_DULL_ATTACKED2(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer) 
{
	if( pDest == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 1.1f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
	
	return TRUE;
}

//...............................................................................................................
//	발톱 기본 공격  데미지 ( 약 )
//...............................................................................................................
int CEffectManager::MAKE_EF_STAND_NAIL_ATTACKED0(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pDest == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 1.1f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
	
	return TRUE;
}

//...............................................................................................................
//	발톱 기본 공격  데미지 ( 중 )
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
//	발톱 기본 공격  데미지 ( 강 )
//...............................................................................................................
int CEffectManager::MAKE_EF_STAND_NAIL_ATTACKED2(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pDest == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , id , 0 , 1.1f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
	
	return TRUE;
}

//...............................................................................................................
//	속사 ( 아처 )
//...............................................................................................................
int CEffectManager::MAKE_EF_QUICK_BOW(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , EF_QUICK_BOW_0 , ESLT_CHARPOS , 2.1f , pSrc->GetUnique() , pSrc->GetIndex() );
	
	//................................................................................	
	// 발동 사운드
	//................................................................................	
	if( pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	
	return TRUE;
}

//...............................................................................................................
// 	기본 샤먼 수정구 공격
//...............................................................................................................
int CEffectManager::MAKE_EF_BALL_ATTACK(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	StandTargetHit( pSrc , pDest , pExtraInfo );
	
	return TRUE;
}

//...............................................................................................................
// 	기본 샤먼 수정구 공격 데미지
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
//	치유의 화살 ( 아처 )
//...............................................................................................................
int CEffectManager::MAKE_EF_REFRESH_BOW(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// 데스트 없으면 소스쪽만 이펙트 출력
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	CEffectBase	*pData = SAFE_NEW( CEFRefreshBow );				// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );				// 소스 , 타켓 지정
	
	PushEffect( &pData );										// 메니저에 넣기
	
	return TRUE;
}

//...............................................................................................................
//	각성의 화살
//...............................................................................................................
int CEffectManager::MAKE_EF_COMBO_ADD_BOW(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// 데스트 없으면 소스쪽만 이펙트 출력
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	CEffectBase	*pData = SAFE_NEW( CEFBeeLineBow );				// 클래스 생성
	
	((CEFBeeLineBow *)pData)->SetExtra( PT_COMBO_ADD_BOW_TAIL , EF_COMBO_ADD_BOWED , 70 , 3.1f );
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );				// 소스 , 타켓 지정
	
	PushEffect( &pData );										// 메니저에 넣기
	
	return TRUE;
}

//...............................................................................................................
//	버블 힐
//...............................................................................................................
int CEffectManager::MAKE_EF_HEAL_BUBBLE(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// 데스트 없으면 소스쪽만 이펙트 출력
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	CEffectBase	*pData = SAFE_NEW( CEFHealBubble );				// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );				// 소스 , 타켓 지정
	
	PushEffect( &pData );										// 메니저에 넣기
	
	return TRUE;
}

//..............................................................................................................
// 왕방울 공격 ( NPC )
//..............................................................................................................
int CEffectManager::MAKE_EF_BIG_BUBBLE(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// 데스트 없으면 소스쪽만 이펙트 출력
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	CEffectBase	*pData = SAFE_NEW( CEFBigBubble );				// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );				// 소스 , 타켓 지정
	
	PushEffect( &pData );										// 메니저에 넣기
	
	return TRUE;
}

//..............................................................................................................
// SP 흡수
//..............................................................................................................
int CEffectManager::MAKE_EF_SP_SUCTION(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// 데스트 없으면 소스쪽만 이펙트 출력
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	CEffectBase	*pData = SAFE_NEW( CEFSPSuction );				// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );				// 소스 , 타켓 지정
	
	PushEffect( &pData );										// 메니저에 넣기
	
	return TRUE;
}

//..............................................................................................................
// 차가운 입김
//..............................................................................................................
int CEffectManager::MAKE_EF_ICE_RIP_BLOW(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// 데스트 없으면 소스쪽만 이펙트 출력
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	CEffectBase	*pData = SAFE_NEW( CEFIceRipBlow );				// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );				// 소스 , 타켓 지정
	
	PushEffect( &pData );										// 메니저에 넣기
	
	return TRUE;	
}

//..............................................................................................................
// 무력화
//..............................................................................................................
int CEffectManager::MAKE_EF_MAGIC_DELETE(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// 데스트 없으면 소스쪽만 이펙트 출력
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	CEffectBase	*pData = SAFE_NEW( CEFMagicDelete );			// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );				// 소스 , 타켓 지정
	
	PushEffect( &pData );										// 메니저에 넣기
	
	return TRUE;	
}


//..............................................................................................................
// HP 흡수
//..............................................................................................................
int CEffectManager::MAKE_EF_HP_SUCTION(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// 데스트 없으면 소스쪽만 이펙트 출력
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	CEffectBase	*pData = SAFE_NEW( CEFHPSuction );				// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );				// 소스 , 타켓 지정
	
	PushEffect( &pData );										// 메니저에 넣기
	
	return TRUE;
}


//..............................................................................................................
// 침묵의 화살
//..............................................................................................................
int CEffectManager::MAKE_EF_WHISPER_BOW(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// 데스트 없으면 소스쪽만 이펙트 출력
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	CEffectBase	*pData = SAFE_NEW( CEFBeeLineBow );				// 클래스 생성
	
	((CEFBeeLineBow *)pData)->SetExtra( PT_WHISPER_BOW_TAIL , EF_WHISPER_BOWED , 70 , 3.1f );
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );				// 소스 , 타켓 지정
	
	PushEffect( &pData );										// 메니저에 넣기
	
	return TRUE;
}

//..............................................................................................................
// 해제의 화살
//..............................................................................................................
int CEffectManager::MAKE_EF_CANCEL_BOW(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// 데스트 없으면 소스쪽만 이펙트 출력
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	CEffectBase	*pData = SAFE_NEW( CEFBeeLineBow );				// 클래스 생성
	
	((CEFBeeLineBow *)pData)->SetExtra( PT_CANCEL_BOW_TAIL , EF_CANCEL_BOWED , 70 , 3.1f );
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );				// 소스 , 타켓 지정
	
	PushEffect( &pData );										// 메니저에 넣기
	
	return TRUE;
}

//..............................................................................................................
// 정화의 화살
//..............................................................................................................
int CEffectManager::MAKE_EF_CLEAR_BOW(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// 데스트 없으면 소스쪽만 이펙트 출력
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	CEffectBase	*pData = SAFE_NEW( CEFBeeLineBow );						// 클래스 생성
	
	((CEFBeeLineBow *)pData)->SetExtra( PT_CLEAR_BOW_TAIL , EF_CLEAR_BOWED , 70 , 3.1f );
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );				// 소스 , 타켓 지정
	
	PushEffect( &pData );										// 메니저에 넣기
	
	return TRUE;
}

//..............................................................................................................
// 아쳐, 스나이퍼 활-범용스킬
//..............................................................................................................
int CEffectManager::MAKE_BEE_LINE_BOW(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo, int nTailNum, int nDNum, 
									  float fDTime, float fDelTime, int nDType)
{
	// 데스트 없으면 소스쪽만 이펙트 출력
	//if( pExtraInfo == NULL || pDest == NULL )
	//	return TRUE;
	
	CEffectBase	*pData = SAFE_NEW( CEFBeeLineBow );						// 클래스 생성
	
	((CEFBeeLineBow *)pData)->SetExtra( nTailNum , nDNum , fDTime , fDelTime, nDType );
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );				// 소스 , 타켓 지정
	
	PushEffect( &pData );										// 메니저에 넣기
	
	return TRUE;	
}

//...............................................................................................................
//	아이스 레인 맞음
//...............................................................................................................
int CEffectManager::MAKE_EF_ICE_RAINED(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , EF_ICE_RAINED , 0 , 5.1f , pSrc->GetUnique() , pSrc->GetIndex() );
	
	return TRUE;
}

//...............................................................................................................
//	FireRain 맞음 
//...............................................................................................................
int CEffectManager::MAKE_EF_FIRE_RAINED(EFFECT_ID id , Character *pSrc , Character *pDest, LPEFFECT_EXTRA_INFO pExtraInfo , float Timer )
{
	
	if( pSrc == NULL)
		return FALSE ;
	
	g_Particle.SetEffSetCon( 0.0f , EF_FIRE_RAINED , 0 , 5.1f , pSrc->GetUnique() , pSrc->GetIndex() );
	
	return TRUE;
	
}



//...............................................................................................................
//	집중 ( 아처 )
//...............................................................................................................
int CEffectManager::MAKE_EF_FOCUS_BOW(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , EF_FOCUS_BOW_0 , ESLT_CHARPOS , 2.1f , pSrc->GetUnique() , pSrc->GetIndex() );
	
	//................................................................................	
	// 발동 사운드
	//................................................................................	
	if( pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	
	return TRUE;
}

//..............................................................................................................
// 이그니션
//..............................................................................................................
int CEffectManager::MAKE_EF_EGNEETION(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	//................................................................................	
	// 발동 사운드
	//................................................................................	
	if( pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	
	
	if( pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillAttackedSound( pExtraInfo->SkillCode , pDest );
	
	EffectTargetHit( pSrc , pDest , pExtraInfo );
	
	return TRUE;
}

//..............................................................................................................
// 이그니션 데미지 ( 지속 )
//..............................................................................................................
int CEffectManager::MAKE_EF_EGNEETION_ED(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	CEffectBase *pData = SAFE_NEW( CEFSrcLink );	
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );				// 소스 , 타켓 지정
	
	PushEffect( &pData );										// 메니저에 넣기
	
	return TRUE;
}

//..............................................................................................................
// 차지
//..............................................................................................................
int CEffectManager::MAKE_EF_CHARGE(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	CEffectBase *pData = SAFE_NEW( CEFSrcLink );	
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );				// 소스 , 타켓 지정
	
	PushEffect( &pData );										// 메니저에 넣기
	
	//................................................................................	
	// 발동 사운드
	//................................................................................	
	if( pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	
	return TRUE;
}

//...............................................................................................................
//	절대 방어
//...............................................................................................................
int CEffectManager::MAKE_EF_ABSOLUTE_DP(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	g_Particle.SetEffSetCon( 0.0f , EF_ABSOLUTE_DP_0 , ESLT_CHARPOS , 2.1f , pSrc->GetUnique() , pSrc->GetIndex() );
	
	//................................................................................	
	// 발동 사운드
	//................................................................................	
	if( pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );	
	
	return TRUE;
}

//...............................................................................................................
//	마인의 각력
//...............................................................................................................
int CEffectManager::MAKE_EF_FOOT_STRENGTH(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	CEffectBase	*pData = SAFE_NEW( CEFFootStrength );			// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );				// 소스 , 타켓 지정
	
	PushEffect( &pData );										// 메니저에 넣기
	
	return TRUE;
}

//...............................................................................................................
//	마인의 권술
//...............................................................................................................
int CEffectManager::MAKE_EF_BUNCH_STRENGTH(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	CEffectBase	*pData = SAFE_NEW( CEFBunchStrength );			// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );				// 소스 , 타켓 지정
	
	PushEffect( &pData );										// 메니저에 넣기
	
	return TRUE;
}

//...............................................................................................................
//	해골전사용 스킬
//...............................................................................................................
int CEffectManager::MAKE_EF_BONWARSKILL(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	CEffectBase	*pData = SAFE_NEW( CEFBonWarSkill );			// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );				// 소스 , 타켓 지정
	
	PushEffect( &pData );										// 메니저에 넣기	
	
	return TRUE;
}

//...............................................................................................................
//	해골아쳐용 스킬
//...............................................................................................................
int CEffectManager::MAKE_EF_BONARCSKILL(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	CEffectBase	*pData = SAFE_NEW( CEFBonArcSkill );			// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );				// 소스 , 타켓 지정
	
	PushEffect( &pData );										// 메니저에 넣기	
	
	return TRUE;
}



//...............................................................................................................
//	클래스 지존아이템 - 연결 이펙트
//...............................................................................................................
int CEffectManager::MAKE_EF_SPECIAL_ITEM_EFFECT(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	CEffectBase	*pData = SAFE_NEW( CEFSpeacialItem );			// 클래스 생성

	pData->Setup( id , pSrc , pDest , pExtraInfo );				// 소스 , 타켓 지정
	
	PushEffect( &pData );										// 메니저에 넣기	
	
	return TRUE;
}

//...............................................................................................................
//	합성 엘다아이템 - 연결 이펙트
//...............................................................................................................
int CEffectManager::MAKE_EF_BASE_ITEM_EFFECT(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{

	CEffectBase	*pData = SAFE_NEW( CEFEldaWeaponItem );			// 클래스 생성

	pData->Setup( id , pSrc , pDest , pExtraInfo );				// 소스 , 타켓 지정
	
	PushEffect( &pData );										// 메니저에 넣기	

	return TRUE;
}




//...............................................................................................................
//	루갈 스킬
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
				// 스킬 발동 사운드
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


	CEffectBase	*pData = SAFE_NEW( CEFRugalSkill );			// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );				// 소스 , 타켓 지정
	
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
				// 스킬 발동 사운드
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
	
	CEffectBase	*pData = SAFE_NEW( CEFDogmaEffect );			// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );				// 소스 , 타켓 지정
	
	PushEffect( &pData );										// 메니저에 넣기	
	
	return TRUE;
}

//	바라고스 관련 이펙트
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
				// 스킬 발동 사운드
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
	
	CEffectBase	*pData = SAFE_NEW( CEFBaragosEff );				// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );				// 소스 , 타켓 지정
	
	PushEffect( &pData );										// 메니저에 넣기

	return TRUE;
}




int CEffectManager::MAKE_EF_JANADEFFECT(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{

	D3DXVECTOR3 vPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	
	int DestSetNum = 0;
	int DestSetType = 0; 
	float DestTimer = 5.1f;
	
	//소환 이펙트
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
				// 스킬 발동 사운드
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
	
	CEffectBase	*pData = SAFE_NEW( CEFJanadEff );				// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );				// 소스 , 타켓 지정
	
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
				//우로보스 기본 공격 빼자 .. 
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
				// 스킬 발동 사운드
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
	
	CEffectBase	*pData = SAFE_NEW( CEFUrobosEff );				// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );				// 소스 , 타켓 지정
	
	PushEffect( &pData );
	
	
	return TRUE;
}



//dongtest 

int CEffectManager::MAKE_EF_DONGS_TEST(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	
	CEffectBase	*pData = SAFE_NEW( CEFFameItem );				// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );				// 소스 , 타켓 지정
	
	PushEffect( &pData );										// 메니저에 넣기
	
	return TRUE;
}


//..............................................................................................................
// 신성한 폭발 
//..............................................................................................................
int CEffectManager::MAKE_EF_HOLY_BOMB(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE ;
	
	CEffectBase	*pData = SAFE_NEW( CEFHolyBomb );				// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );				// 소스 , 타켓 지정
	
	PushEffect( &pData );										// 메니저에 넣기
	return TRUE;	
}


//..............................................................................................................
// 어둠의 폭발 
//..............................................................................................................
int CEffectManager::MAKE_EF_DARK_BOMB(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	CEffectBase	*pData = SAFE_NEW( CEFDarkBomb );				// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );				// 소스 , 타켓 지정
	
	PushEffect( &pData );										// 메니저에 넣기
	return TRUE;	
}


//..............................................................................................................
// 분노 폭발 
//..............................................................................................................
int CEffectManager::MAKE_EF_WRATH_BOMB(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	if( pSrc == NULL )
		return FALSE;
	
	
	g_Particle.SetEffSetCon( 0.0f , EF_WRATH_BOMB , ESLT_SRC_RAD , 2.1f , pSrc->GetUnique() , pSrc->GetIndex() , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) , pSrc->GetDirection() );
	
	//................................................................................	
	// 스킬 발동 사운드
	//................................................................................	
	g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
	
	// 데스트 없으면 소스쪽만 이펙트 출력
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	// 1:N 데미지
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
	// 어택 사운드
	//................................................................................	
	if( pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillAttackedSound( pExtraInfo->SkillCode , pDest );		
	
	return TRUE;
}
//..............................................................................................................
// 장갑 파괴 
//..............................................................................................................
int CEffectManager::MAKE_EF_GLOVE_DESTRUCTION(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	
	if( pSrc == NULL || pDest == NULL)
		return FALSE ;
	
	CEffectBase	*pData = SAFE_NEW( CEFGloveDestruction );		// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );				// 소스 , 타켓 지정
	
	PushEffect( &pData );										// 메니저에 넣기
	
	return TRUE;
}
//..............................................................................................................
// 다크니스 스피어  
//..............................................................................................................
int CEffectManager::MAKE_EF_ONE_DARKNESS(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	
	if( pSrc == NULL || pDest == NULL)
		return FALSE ;
	
	CEffectBase	*pData = SAFE_NEW( CEFDarkness );				// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );				// 소스 , 타켓 지정
	
	PushEffect( &pData );										// 메니저에 넣기
	
	return TRUE;
}
//..............................................................................................................
// 트리플 다크니스 
//..............................................................................................................
int CEffectManager::MAKE_EF_TRIPLE_DARKNESS(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	
	if( pSrc == NULL || pDest == NULL)
		return FALSE ;
	
	CEffectBase	*pData = SAFE_NEW( CEFTripleDarkness );			// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );				// 소스 , 타켓 지정
	
	PushEffect( &pData );										// 메니저에 넣기
	return TRUE;
}
//..............................................................................................................
// 트리플 파이어 
//..............................................................................................................
int CEffectManager::MAKE_EF_TRIPLE_FIRE(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	
	if( pSrc == NULL || pDest == NULL)
		return FALSE ;
	
	CEffectBase	*pData = SAFE_NEW( CEFTripleFire );				// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );				// 소스 , 타켓 지정
	
	PushEffect( &pData );										// 메니저에 넣기
	
	return TRUE;
}







//...............................................................................................................
//	벽뢰각 ( 샤먼 )
//...............................................................................................................
int CEffectManager::MAKE_EF_FOOT_LIGHTNING(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// 데스트 없으면 소스쪽만 이펙트 출력
	if( pExtraInfo == NULL || pDest == NULL )
		return TRUE;
	
	g_Particle.SetEffSetCon( 0.0f , id , ESLT_CHARPOS , 2.1f , pDest->GetUnique() , pDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSrc) );
	//	g_Particle.SetEffSetCon( 0.0f , id , 100 , 2.1f , pSrc->GetUnique() , pSrc->GetIndex() , D3DXVECTOR3( pSrc->GetPosTM()._41 , pSrc->GetPosTM()._42 , pSrc->GetPosTM()._43 ) );
	
	//................................................................................	
	// 발동 사운드
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
// 기본 활 공격
//...............................................................................................................
int CEffectManager::MAKE_EF_BOW_ATTACK0(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	CEffectBase	*pData = SAFE_NEW( CEFBowAttack );				// 클래스 생성
	
	pData->Setup( id , pSrc , pDest , pExtraInfo );				// 소스 , 타켓 지정
	
	PushEffect( &pData );										// 메니저에 넣기
	
	return TRUE;
}

//...............................................................................................................
//	아이템 사용시 이펙트 효과
//...............................................................................................................
int CEffectManager::UseItemEffect(long Unique,DWORD ItemCode)
{
	
	
	
	//...........................................................................................................
	// 발동 이펙트 유무 검사
	//...........................................................................................................
	int AttackedEffectCode = g_Pc_Manager.theItemInfoTable[ g_Pc_Manager.theItemCodeTable[ ItemCode ] ].theAttackEff;
	
	short AttackSound = g_Pc_Manager.theItemInfoTable[ g_Pc_Manager.theItemCodeTable[ ItemCode ] ].theAttackSnd;
	
	//...........................................................................................................
	// 캐릭터 찾기
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
	// 캐릭터 없을때
	//...........................................................................................................
	if( NULL == pSrc )
	{
		DebugPrint( "!\n == CEffectManager::UseItemEffect ( 사용자를 찾을수 없네 ) == \n" );
		
		return FALSE;
	}
	
	// 먹을때 나는 소리
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
	// 이펙트 생성
	//...........................................................................................................
	EFFECT_EXTRA_INFO	TempExtraInfo;
	
	TempExtraInfo.SkillCode	= ItemCode;					// 아이템 코드 넣기 ( T.T ) 나중에 스킬에 대한 정보 얻어야되서리...
	
	CreateEffect( ( EFFECT_ID ) AttackedEffectCode , pSrc , NULL , &TempExtraInfo );
	
	return TRUE;
}

//............................................................................................................
// 공격 시작 프레임 0 에서 처리할부분 ( T.T )
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
		// 마법서 
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

// 몬스터 맞을때 나는 소리
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
		// 데미지 약
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
// 부가 효과
//..............................................................................................................
int CEffectManager::MAKE_EF_ADDEFFECT_BASE(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo,float Timer)
{
	// 소스쪽만 이펙트 출력
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
	
	CEffectBase	*pData = SAFE_NEW( CEFAddEffectBase );						// 클래스 생성
	
	pData->Setup( EF_ADDEFFECT_BASE , pSrc , NULL , NULL );					// 소스 , 타켓 지정
	
	((CEFAddEffectBase *)pData)->InsertAddSkill( id );
	
	PushEffect( &pData );													// 메니저에 넣기
	
	return TRUE;
}

//..............................................................................................................
// 플레이어 죽었을때 이펙트 지우기
//..............................................................................................................
void CEffectManager::DeletePlayerEffect(Character *pSrc)
{
	if( pSrc == NULL )
		return;
	
	g_Particle.m_EffectManager.DeleteOnlySrc( pSrc );
	
	pSrc->m_SpellEffect.Clear();
}

//..............................................................................................................
// 다른 플레이어 죽었을때 이펙트 지우기
//..............................................................................................................
void CEffectManager::DeleteOtherPlayerEffect(Character *pSrc)
{
	if( pSrc == NULL )
		return;
	
	g_Particle.m_EffectManager.DeleteOnlySrc( pSrc );
	pSrc->m_SpellEffect.Clear();
}

//..............................................................................................................
// NPC 죽었을때 이펙트 지우기
//..............................................................................................................
void CEffectManager::DeleteNPCEffect(Character *pSrc)
{
	g_Particle.m_EffectManager.DeleteOnlySrc( pSrc );
}


//..............................................................................................................
// 스킬 임펙트 프레임시 나는 소리
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
// 스킬 시작시 사운드 ( 일단 영창만 차후 요청 있을시 CRockClientNet에서 처리 )
//..............................................................................................................
int	 CEffectManager::GetSkillStartSound(DWORD aSkillCode,Character *pSrc,BOOL ISCheck)
{
	SSkillBaseInfo* skill_Info;
	
	skill_Info = g_Pc_Manager.GetSkillBaseInfoPtr( aSkillCode ); 
	
	if( skill_Info == NULL )
		return -1;
	
	if( skill_Info->theMagicShotSount == -1 )	
		return -1;
	
	// 영창 루틴 체크
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
// 스킬 어택 프레임시 나는 소리
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
// 명상시 이펙트
//..............................................................................................................
int	 CEffectManager::SetRestSkill(Character *pSrc)
{
	if( pSrc == NULL )
		return FALSE;
	
	int Index    = -1;
	int EffectID = -1;
	
	// 플레이어 일때	
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
		// 밀레나
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
// 명상시 이펙트 지우기
//..............................................................................................................
int	 CEffectManager::DeleteRestSkill(Character *pSrc)
{
	if( pSrc == NULL )
		return FALSE;
	
	int Index    = -1;
	int EffectID = -1;
	
	// 플레이어 일때	
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
		// 밀레나
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
// 화살 붙이기
//.............................................................................................................
int CEffectManager::CreateArrowStuck(Character *pSrc,float AddRad,D3DXVECTOR3 vAddPos,int LinkBone)
{
	CEffectBase	*pData = SAFE_NEW( CArrowStuck );							// 클래스 생성
	
	pData->Setup( ( EFFECT_ID ) -1 , pSrc , NULL , NULL );					// 소스 , 타켓 지정
	
	((CArrowStuck *)pData)->SetExtraInfo( AddRad , vAddPos , LinkBone );
	
	PushEffect( &pData );										// 메니저에 넣기
	
	return TRUE;
}

//.............................................................................................................
// 없어진 캐릭 검사
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
// 없어진 캐릭 검사
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
// 죽었는지 검사 ( 없어진 캐릭도 검사 )
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
// 지속성 스킬이 있다면 이펙트 달아주기
//.............................................................................................................
void CEffectManager::CreateKeepupSkill(int skillcode,Character *pSrc,Character *pDest,BOOL ISExcept)
{
	SSkillBaseInfo* skill_Info;
				
	skill_Info = g_Pc_Manager.GetSkillBaseInfoPtr( skillcode ); 
				
	if( skill_Info != NULL )
	{
		// 지속성 스킬이냐?
		if( skill_Info->theEffectFlag || ISExcept )
		{
			// 발동 스킬 ( 이전 )
			// g_Particle.m_EffectManager.Create( ( EFFECT_ID ) skill_Info->theActionEffect , &g_Map.m_Npc[npc_tbl_index] , NULL );
			
			// 지속성 스킬
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
// 지속성 스킬 이펙트 지우기
//.............................................................................................................
void CEffectManager::DeleteKeepupSkill(int skillcode,Character *pSrc)
{
	SSkillBaseInfo* skill_Info;
	skill_Info = g_Pc_Manager.GetSkillBaseInfoPtr( skillcode ); 
	
	// 발동 스킬 이전
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
// 플레이어 지속성 스킬 이펙트 모두 지우기
//.............................................................................................................
void CEffectManager::DeleteAllKeepupSkillOnPlayer(void)
{
	for( int i = 0; i < RP_MAX_KEEPUP_SKILL; ++i )
	{
		if( nRui->thePcInven.KeepupSkill[i] != 0 )
		{
			WORD aSkillCode = nRui->thePcInven.KeepupSkill[i];
			
			//.............................................................................................................
			// 지속성 스킬 이펙트 지우기
			//.............................................................................................................
			g_Particle.m_EffectManager.DeleteKeepupSkill( nRui->thePcInven.KeepupSkill[i] , g_Pc.GetPlayer() );
			g_Particle.m_EffectManager.DeleteAddEffectEnd( nRui->thePcInven.KeepupSkill[i] , g_Pc.GetPlayer() );
			
			PlayerInfoWnd.DeleteKeepUpSkillCode( i, aSkillCode );
			
			nRui->thePcInven.KeepupSkill[i] = 0;
		}
	}
}


//.............................................................................................................
// 1:N 이펙트 시작 
//.............................................................................................................
void CEffectManager::EffectCreateLock(void)
{
	m_EffectState = CREATE_EFFECT_LOCK_ONETIME;
	
	m_ExtraEffectQueue.SubExtraInfoList.clear();
}

//.............................................................................................................
// 1:N 이펙트 마지막에 호출 ( 이펙트 발동 )
//.............................................................................................................
void CEffectManager::EffectCreateUnLock(void)
{
	m_EffectState = CREATE_EFFECT_UNLOCK;
}

//.............................................................................................................
// 스킬 코드 좋은거냐? ( 모션 데이터 없음 )
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
// 실시간 이펙트 ( 오라형 ) 처리 ( 플레이어 , 데미지 동작은 하지 않음 )
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
	
	// 상태 이상 이펙트 번호 얻기
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
	
	// 상태 이상 이펙트 번호 얻기
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
// 실시간 이펙트 ( 오라형 ) 처리 ( 자기는 제외 , 데미지 동작은 하지 않음 )
//.............................................................................................................
void CEffectManager::CreateAuroraEffect(DWORD SkillCode,BYTE TargetNum,SObjectIDPtr TargetList , int FrontierHolyLevel /* = 0  */)
{
	BOOL ISFind = FALSE;
	
	for( int i = 0 ; i < ( int ) TargetNum ; ++i )
	{
		ISFind = FALSE;
		
		//.....................................................................................................
		// 유저일때
		//.....................................................................................................
		if( n_PC == TargetList[ i ].theType )
		{
			//.................................................................................................
			// 플레이어일때 ( 무시 ) KeepUpStart 에서 처리한다
			//.................................................................................................
			if( TargetList[ i ].theID == g_Pc.GetPlayer()->GetUnique() )
			{
				continue;
			}
			//.................................................................................................
			// 다른 유저일때
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
						
						
						// 지속스킬 아이콘 달기
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
		// 몹 일때
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
					
					// 지속스킬 아이콘 달기
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
// 광역 마법시 바닥에 찍어주는 이펙트
//...................................................................................................................
int CEffectManager::RenderToLandMark(float dtime)
{
	if( FALSE == m_ISLandMarkDisplay )
		return FALSE;
	
	static int OldMouseX = -1;
	static int OldMouseY = -1;
	static D3DXVECTOR3 vPos = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
	
	// 마우스 체크해서 지형 체크하는곳
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
	
	// 출력 루틴
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
// 광역 마법 Appear 시 타겟 없을때
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
			// 이펙트 지우기 
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
// 광역 마법 데미지 
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
	
	// 데미지가 0일때 죽었을때도 이펙트 보이기
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
			// 데미지 찍을때 플레이어랑 연관있는것만 찍기
			//if( pSrc == g_Pc.GetPlayer() || pDest == g_Pc.GetPlayer() )
			g_Particle.SetEffSetCon( 0.0f , EF_ATTACK_MISS , 0 , 0.3f , pDest->GetUnique() , pDest->GetIndex() );
			
			break;
		}
		// return;
	}
	
	// 데미지 숫자 찍기
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
	
	
	// dongs Test 이펙트 끔 .. 주석 처리 .. 
	g_Particle.m_EffectManager.CreateEffect( ( EFFECT_ID ) skill_Info->theActionEffect , pDest , NULL , NULL );
	
	if( ISDie == TRUE )
	{
		int skilledIdx = 0;
		EVENT_DATA_INFO underattack_event;	
		
		underattack_event.type = SM_STATE_DIE;
		
		//......................................................................................................
		// 유저
		//......................................................................................................
		if( pDest->m_bPC )
		{
#ifdef ROCKCLIENT_DEV					
//			SystemPrint( n_DarkRed, n_SystemMsg, "내가 죽였다 메롱ㅋㅋㅋ~~~ SM_STATE_DIE" );				
#endif
			underattack_event.motion = n_Die9;
			
			g_Particle.DelEffSetCon( EF_REST_BREATH , 0 , g_Pc.GetPlayer()->GetUnique() , g_Pc.GetPlayer()->GetIndex() );
			
			// 플레이어 죽었을때 이펙트 지우기
			DeletePlayerEffect( pDest );
			
			//...............................................................................................
			// 내꺼
			//...............................................................................................
			if( pDest->GetUnique() == g_Pc.GetPlayer()->GetUnique() )
			{
				g_Pc.GetPlayer()->PushQueue( underattack_event, TRUE );
			}
			//...............................................................................................
			// 남꺼
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
			
			// 죽을때 가지고 있던 이펙트 지우기
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
	
	// 데미지가 0일때 죽었을때도 이펙트 보이기
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
			// 데미지 찍을때 플레이어랑 연관있는것만 찍기
			//if( pSrc == g_Pc.GetPlayer() || pDest == g_Pc.GetPlayer() )
			g_Particle.SetEffSetCon( 0.0f , EF_ATTACK_MISS , 0 , 0.3f , pDest->GetUnique() , pDest->GetIndex() );
			
			break;
		}
		// return;
	}
	
	
	// 데미지 숫자 찍기
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
		// 유저
		//......................................................................................................
		if( pDest->m_bPC )
		{
			underattack_event.motion = n_Die9;
			
			g_Particle.DelEffSetCon( EF_REST_BREATH , 0 , g_Pc.GetPlayer()->GetUnique() , g_Pc.GetPlayer()->GetIndex() );
			
			// 플레이어 죽었을때 이펙트 지우기
			DeletePlayerEffect( pDest );
			
			//...............................................................................................
			// 내꺼
			//...............................................................................................
			if( pDest->GetUnique() == g_Pc.GetPlayer()->GetUnique() )
			{
				g_Pc.GetPlayer()->PushQueue( underattack_event, TRUE );
			}
			//...............................................................................................
			// 남꺼
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
			
			// 죽을때 가지고 있던 이펙트 지우기
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
// 새로운 파티 맴버가 은신 / 월영 상태이면 처리
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
// 기존 파티 맴버가 은신 / 월영 상태이면 처리
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
// 방어구 인첸트시 이펙트 달기
//...............................................................................................................
int CEffectManager::CreateDefenceEnChantEffect(Character *pChar, BYTE Level)
{
	
#ifndef DEFENSE_EFFECT
	return FALSE; 
#endif 
	
	
	// 플레이어 아니면 안됨
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
				
				//어떤엘다 인가 자주 , 연두 , 녹색 , 보라 , 남색 , 은색 , 노랑 
				EldaColor = GetDefenceEldaStoneInfo(EldaStoneCode);
				
				if(EldaColor != -1 )
				{
					//레벨
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
			// 서버 수정후 다른유저도 이펙트 달기 
			if( g_Pc.m_PcTable[pChar->m_nTableIndex].char_info.theWearItem[position].theCode != 0 )
			{
				EldaStoneCode = g_Pc.m_PcTable[pChar->m_nTableIndex].char_info.theWearItem[position].theEldaStone;
				
				//어떤엘다 인가 자주 , 연두 , 녹색 , 보라 , 남색 , 은색 
				EldaColor = GetDefenceEldaStoneInfo(EldaStoneCode);
				
				if(EldaColor != -1 )
				{
					//레벨
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
	
	pData->Setup( (EFFECT_ID)ID , pChar , NULL , NULL );							// 소스 , 타켓 지정
	
	PushEffect( &pData );												// 메니저에 넣기
	
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
		//자주색
		return 1;
	}
	
	if(pItemBaseInfo->theAttackDef)
	{
		//연두색	
		return 2;
	}
	
	if(pItemBaseInfo->theVital)
	{
		//녹색
		return 3;
	}
	
	if(pItemBaseInfo->theInt)
	{
		//보라색
		return 4;
	}
	
	if(pItemBaseInfo->theStamina)
	{
		//남색
		return 5;
	}
	
	if(pItemBaseInfo->theDex)
	{
		//은색
		return 6;
	}
	
	if(pItemBaseInfo->theSympathy)
	{
		//노란색
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
		//주황색
		return -1;
	}
	
	if(pItemBaseInfo->theMinMagicBonus && pItemBaseInfo->theMaxMagicBonus )	
	{
		//푸른색 
		return -1;
		
	}
	
	if(pItemBaseInfo->theMinAttackBonus && pItemBaseInfo->theMaxAttackBonus )
	{
		//붉은색 
		return -1; 
		
	}
	
	
	int iLevel = pItemBaseInfo->theGrade/100;
	
	return iLevel;	
	
}

//...................................................................................................................
// 인첸트시 이펙트 달기
//...................................................................................................................
int CEffectManager::CreateWeaponEnChantEffect(Character *pChar,BYTE Level)
{
	// 플레이어 아니면 안됨
	if( !pChar->m_bPC )
		return FALSE;
	
	if( !CheckCharacter( pChar ) )
		return FALSE;
	
	DeleteWeaponEnChantEffect( pChar );
	
	int WeaponType = -1;
	
	int RHand_index = 0;
	int LHand_index = 0;
	
	//..............................................................................................................
	// 무기 종류 체크
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
	
	
	if( RHand_index != -1 && LHand_index == -1 )				// 오른손 무기만 장착
	{		
		if( g_Pc_Manager.m_pItm[RHand_index]->m_type == 0 )		// 대검 
		{
			WeaponType = 0;
		}
		else if( g_Pc_Manager.m_pItm[RHand_index]->m_type == 2 ) // 한손검 
		{
			WeaponType = 2;
		}
		else if( g_Pc_Manager.m_pItm[RHand_index]->m_type == 6 ) // 지팡이 
		{
			WeaponType = 6;
		}
		else if( g_Pc_Manager.m_pItm[RHand_index]->m_type == 7 ) // 흑수정
		{
			WeaponType = 7;
		}
	}
	else if( RHand_index == -1 && LHand_index != -1 )			// 왼손 무기만 장착
	{
		if( g_Pc_Manager.m_pItm[LHand_index]->m_type == 1 )		// 활 
		{
			WeaponType = 1;
		}
		else if( g_Pc_Manager.m_pItm[LHand_index]->m_type == 5 ) // 마법서 
		{
			WeaponType = 5;
		}
#ifdef ELDA_WEAPON_EFFECT
		else if( g_Pc_Manager.m_pItm[LHand_index]->m_type == 3 ) // 방패
		{
			WeaponType = 2;
		}
#endif 
	}
	else if( RHand_index != -1 && LHand_index != -1 )			// 양손에 무기 장착
	{
		if( g_Pc_Manager.m_pItm[RHand_index]->m_type == 2 && 
			g_Pc_Manager.m_pItm[LHand_index]->m_type == 3 )		// 한손검 + 방패
		{
#ifdef ELDA_WEAPON_EFFECT
			//나이트 이고 칼이펙이 있느냐 ? ? 
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
	else														// 무기 없음 (맨손 공격)
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
		
		if( WeaponType == 1 ||	//활 
			WeaponType == 5 ||	// 마법서 
			WeaponType == 3 )	// 방패 
		{
			position = n_WearSlot_Shield;
			
			//마법서만 무기 이펙트가 다르기 때문에 ..  하드코딩 ... 	
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
			// 서버 수정후 다른유저도 이펙트 달기 
			
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
	
	
	
	CEffectBase *pData = NULL;								// 클래스 생성
	int			 ID    = 0;
#ifdef ELDA_WEAPON_EFFECT
	if( bIsMixEldaStone )
	{
		pData = SAFE_NEW( CEFEldaWeaponItem );		
		
		switch( WeaponType )
		{
			// 대검
		case 0:	ID    = EF_BASE_ITEM_EFFECT_SLAYER;	break;
			// 한손검 
		case 2:	ID    = EF_BASE_ITEM_EFFECT_MERCENARY;break;
			//방패 
		case 3: ID	  = EF_BASE_ITEM_EFFECT_KNIGHT;	break;
			// 지팡이
		case 6:	ID    = EF_BASE_ITEM_EFFECT_HOLY;	break;
			// 수정
		case 7:ID    =	EF_BASE_ITEM_EFFECT_PSYCHE;	break;
			// 활
		case 1: ID    = EF_BASE_ITEM_EFFECT_SNIPER;	break;
			// 마법서
		case 5: ID    = EF_BASE_ITEM_EFFECT_PROMINAS;break;
			// 맨손
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
			// 대검
			//...................................................................................................
		case 0:
			//CEFSwordUp 
			pData = SAFE_NEW( CEFSwordUp );		
			ID    = EF_SWORD_UP;
			
			((CEFSwordUp*)pData)->m_Level      = Level;
			((CEFSwordUp*)pData)->m_WeaponType = WeaponType;
			
			break;
			
			//...................................................................................................
			// 한손검 
			//...................................................................................................
		case 2:
			
			//CEFSwordUp 
			pData = SAFE_NEW(CEFSwordUp);	
			
			ID    = EF_SWORD_UP;
			
			((CEFSwordUp*)pData)->m_Level      = Level;
			((CEFSwordUp*)pData)->m_WeaponType = WeaponType;
			
			break;
			
			//...................................................................................................
			// 지팡이
			//...................................................................................................
		case 6:
			
			pData = SAFE_NEW( CEFStickUp );		
			ID    = EF_STICK_UP;
			
			((CEFStickUp*)pData)->m_Level      = Level;
			((CEFStickUp*)pData)->m_WeaponType = WeaponType;
			
			break;
			
			//...................................................................................................
			// 수정
			//...................................................................................................
		case 7:
			
			pData = SAFE_NEW( CEFBallUp );		
			ID    = EF_BALL_UP;
			((CEFBallUp*)pData)->m_Level      = Level;
			((CEFBallUp*)pData)->m_WeaponType = WeaponType;
			
			break;
			
			//...................................................................................................
			// 활
			//...................................................................................................
		case 1:
			
			ID    = EF_BOW_UP;
			pData = SAFE_NEW( CEFBowUp );	
			
			((CEFBowUp*)pData)->m_Level      = Level;
			((CEFBowUp*)pData)->m_WeaponType = WeaponType;
			
			break;
			
			//...................................................................................................
			// 마법서
			//...................................................................................................
		case 5:
			
			ID    = EF_BOOK_UP;
			pData = SAFE_NEW( CEFBookUp );	
			
			((CEFBookUp*)pData)->m_Level      = Level;
			((CEFBookUp*)pData)->m_WeaponType = WeaponType;
			
			break;
			
			//...................................................................................................
			// 맨손
			//...................................................................................................
		case -1:
			return FALSE;
			
		}
	}
	
	if( pData == NULL )
		return FALSE;
	
	pData->Setup( (EFFECT_ID)ID , pChar , NULL , NULL );							// 소스 , 타켓 지정
	
	PushEffect( &pData );												// 메니저에 넣기
	
	
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
// 엘다스톤 레벨 얻기 ( 무기만 , 무기아니면 -1 )
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
		//노란색
		return -1;
	}
	
	if(pItemBaseInfo->theVital)
	{
		//녹색
		return -1;
	}
	
	if(pItemBaseInfo->theInt)
	{
		//보라색
		return -1;
	}
	
	if(pItemBaseInfo->theMagicRes)
	{
		//자주색
		return -1;
	}
	
	if(pItemBaseInfo->theStamina)
	{
		//남색
		return -1;
	}
	
	if(pItemBaseInfo->theDex)
	{
		//은색
		return -1;
	}
	
	if(pItemBaseInfo->theAttackDef)
	{
		//연두색	
		return -1;
	}
	
	if(pItemBaseInfo->thePower)
	{
		//주황색
		return -1;
	}
	
	//푸른색도 붉은색도 아니라면..
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
// 인첸트 이펙트 삭제
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
// 스킬 이펙트 1 : 1 로 임팩트시 데미지주는것
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
				// 스킬 발동 사운드
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
			// 어택 사운드
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
			// 어택 사운드
			//................................................................................	
			if( pExtraInfo != NULL )
				g_Particle.m_EffectManager.GetSkillAttackedSound( pExtraInfo->SkillCode , pDest );
		}
	}
	
	return TRUE;
}


//...........................................................................................................
// 스킬 이펙트 1 : N 로 임팩트시 데미지주는것
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
			// 스킬 발동 사운드
			//................................................................................	
			g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
		}
	}
	
	
	if( CheckCharacter( pSrc , pDest ) && NULL != pExtraInfo )
	{
		// 1:N 데미지
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
		// 어택 사운드
		//................................................................................	
		if( pExtraInfo != NULL )
			g_Particle.m_EffectManager.GetSkillAttackedSound( pExtraInfo->SkillCode , pDest );
	}
	
	
	return TRUE;
}

//...........................................................................................................
// 스킬 이펙트 소스 쪽만 
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
			// 스킬 발동 사운드
			//................................................................................	
			g_Particle.m_EffectManager.GetSkillImfactSound( pExtraInfo->SkillCode , pSrc );
			EffectTargetHit( pSrc , pDest , pExtraInfo );
		}
	}
	
	return TRUE;
}

//...........................................................................................................
// 스킬 이펙트 소스 쪽만 
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
// 이펙트만 출력
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
// 플레이어와의 거리
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
// 아이템 링크 이펙트 추가 by wxywxy
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
// 이펙트 SetDestroy();
//................................................................................................................
int CEffectManager::DeleteLink(Character *pSrc)
{
	std::list	<CEffectBase *> ::iterator	i = m_EffectList.begin();
	
	for( ; i != m_EffectList.end() ; ++i )
	{
		//........................................................................................................
		// 이펙트 지우기 
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
	
	// 현재라이트위치
	D3DXMatrixMultiply( &matLocalToLight, &matWorld, &pSrc->m_Shadow.matWorldToLight );
	
	// 라이트 x 프로젝션 = 그림자행렬
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
	
	// 현재라이트위치
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
			///-- 자신의 펫이고... 공격형펫이라면...
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