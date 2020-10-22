 // SkillSystem.cpp: implementation of the CSkillSystem class.
//
//////////////////////////////////////////////////////////////////////

#include "..\\RockClient.h"
#include <NETWORK\\Protocol.h>
#include "..\\quadlist.h"
#include "..\\Pc.h"
#include "ActionSystem.h"
#include "SkillSystem.h"
#include "..\\Effect.h"
//#include "character.h"
//#include "..\\..\\bravolibs\\obj\\player.h"


#include "stdio.h"

#include "RockInterface\\PlayerInfoWnd.h"
#include "RockInterface\\NonPlayerInfoWnd.h"
#include "RockInterface\\MainMenuWndProc.h"
#include "RockInterface\\ExchangeWndProc.h"
#include "RockInterface\\PartyInforWnd.h"
#include "RockInterface\\CUserStoreSellWnd.h"
#include "RockInterface\\CUserStoreTitleWnd.h"
#include "RockInterface\\CUserStoreSellWnd.h"
#include "RockInterface\\CDelayBarWnd.h"
#include "..\\..\\MapDataManager.h"
#include "stringmanager.h"
#include "RockInterface\\AddEffectMsg.h"
#include "RockInterface\\PetMainWndProc.h"
#include "RockInterface\\PetInfoWnd.h"


const int c_Max_PerSkill_EffectNum( 3 );		// 스킬당 효과 갯수 
const int c_Max_PerSkill_SkillAddEffectNum( 1 );		// 스킬당 부가효과 갯수 (상태이상 처리)

extern		long				g_nowTime;

extern bool g_IsSkEffSys;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSkillSystem::CSkillSystem()
{	
	memset( theShortCut_SkillCode, 0, sizeof(WORD)*2 );
	
	for( int i = 0; i < c_Max_SkillKey_Num; ++i )
	{
		theQuickSlot_SkillCode[i] = 0;
		theQuickSlot_DelayTime[i] = true;	
	}

	theTargetIsPlayer = FALSE;
	theTargetIsPlayerCell = FALSE;
	theQuickSlotIndex = 0;

	theTargetLand_SkillCode = 0;

	theIsUsedSkill = FALSE;
	theUseSpellSkill = FALSE;
	theUseSPellTime = timeGetTime();

	theSkillDelay_time = (long)timeGetTime();

	//LoadShortcutSkill();
}

CSkillSystem::~CSkillSystem()
{
	//SaveShortcutSkill();
	UseSkillList.clear();
}


// 스킬 패시브 및 현재 가지고 있는 스킬 목록 및 스킬 테이블 사용 정보 클래스 생성 필요 (CSkill_Info)
//- CSkillSystem
//----- SSkill_Info					// 각각의 스킬 테이블 정보 (RockBase - basestructure.h) 표시 등
//----- CSkillControl(Interface)	// 사용가능 스킬 표시, 스킬 단축키 지정 및 설정 기능 등 
//----- CSkill_Action				// 스킬 사용 및 키 조작 
//---------- CSkill_Action::UseSkill( int aPressKey );	// 단축키 및 퀵슬롯 조작 
//----- SM_USE_SKILL				// 스킬 사용에 맞는 모션처리 및 각각의 수치 표현, 이펙트, 사운드 

///--JAPAN_BUG_MODIFY
bool CSkillSystem::UseSkillMgr( int aPressKey, DWORD dSkillCode )
{
	static DWORD delay_time = 0;
	
	DWORD nowTime = (DWORD)g_nowTime;

	if( SAFE_TIME_COMPARE( nowTime , > , SAFE_TIME_ADD( 1000 , delay_time ) ) )
	{
		///--PRINT_DLG_LOG("스킬 사용 g_nowTime = %u delay_time + 1000 = %u [Result %d] ", 
		///--g_nowTime, delay_time + 1000 ,(int)(g_nowTime - delay_time) );
		delay_time = nowTime;		
	}			
	else
	{
		///--PRINT_DLG_LOG("스킬 불가 g_nowTime = %u delay_time + 1000 = %u [Result %d] ", 
		///--g_nowTime, delay_time + 1000 ,(int)(g_nowTime - (delay_time + 1000)) );
		return false;
	}
	
	// 교환중에는 스킬 사용 불가 
	if( g_ExchangeWnd.m_bIsExchangeLock )
	{
		SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_SKILL_ON_EX ) );
		return false;
	}

	if( g_DelayBarWnd.IsVisible && DELAY_RIDE_ON == g_DelayBarWnd.GetDelay_Type() )
	{
		SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANNOT_USESKILL_INRIDING  ) );
		return false;
	}
	
	
	if( g_DelayBarWnd.IsVisible && DELAY_PVP_ROOM == g_DelayBarWnd.GetDelay_Type() )
	{
		SystemPrint( n_DarkRed, n_SystemMsg,G_STRING_CHAR(IDS_SYSPR_GO_PVPROOM_NOT_SKILL)); 
		return false;
	}
	DWORD skill_code = 0;

	skill_code = GetSkillCode( aPressKey );			// 스킬 코드값 
	if( skill_code == 0 )//--PRINT_DLG_LOG("스킬 코드 값이 없습니다.. 사용 불가 ");
		return false;
		
	///--JAPAN_BUG_MODIFY		
	if( dSkillCode != skill_code )
		return false;

	//...........................................................................................................
	// 유료아이템 스킬 사용 유무
	//...........................................................................................................
	if( g_Pc.GetPlayer()->m_ISApproachWeapon )
	{
		if( !g_Pc_Manager.ISUseSkillOnApproachWeapon( skill_code ) )
		{
			return false;
		}
	}

	///-- By simwoosung
	///-- 현재 상태가 스킬-스펠 중이면 스킬을 사용하지 못한다.
	if( g_Pc.GetPlayer()->m_curt_event.type == SM_USE_SKILL && 
	   ( g_Pc.GetPlayer()->m_curt_event.status == SM_CHANT_READY || 
	     g_Pc.GetPlayer()->m_curt_event.status == SM_CHANT_SPELLING ) )
	{
		return false;
	}	

	//...........................................................................................................
	// 개인 상점 루틴
	//...........................................................................................................
	if( skill_code == c_SkillCode_UserSellStore )
	{
		if( g_Pc.GetPlayer()->m_curt_event.status == SM_RESTSKILL_SITDOWN ||
			g_Pc.GetPlayer()->m_curt_event.status == SM_RESTSKILL_KEEPING ||
			g_Pc.GetPlayer()->m_curt_event.status == SM_RESTSKILL_STANDUP   )
		{
			///--PRINT_DLG_LOG("개인 상점 휴식 스킬을 사용중입니다 .. ");
			return false;
		}
		
		///--PRINT_DLG_LOG("개인 상점이 아닐시 서버에 보냄 ");
		if( !g_UserStoreSellWnd.ISUserSellMode() )
		{
			g_RockClient.Send_RpCsSkill( g_Pc.GetPlayer()->GetUnique() , g_Pc.GetPlayer()->GetUnique() ,
				n_PC , c_SkillCode_UserSellStore );
		}
		else if( g_UserStoreSellWnd.m_UserStoreSellOpenMode )
		{
			g_RockClient.Send_CsClosePCShop();
		}

		return true;
	}

	if( g_UserStoreSellWnd.ISUserSellMode() )
	{
		SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_USERSTORE_USE ) );
		return FALSE;
	}

	///--현재 맵이 콜로니 맵일 경우
	if( g_Map.m_uMapType == MAP_TYPE_COLONY )
	{
		///-- 스킬 코드가 텔레포트 일경우
		if( skill_code >= c_SkillCode_Teleport_Min && skill_code <= c_SkillCode_Teleport_Max )
		{
			///-- 도착 예상 지점과 현재 위치 사이에 건물이 존재하는지 여부를 확인
			///-- 확인되면... 텔레포트를 막아준다.
			D3DXVECTOR3 vNowPos = D3DXVECTOR3( g_Pc.GetPlayer()->GetPosTM()._41,
											   g_Pc.GetPlayer()->GetPosTM()._42,
											   g_Pc.GetPlayer()->GetPosTM()._43 );

			D3DXVECTOR3 vDir = g_Pc.GetPlayer()->m_vDir;
			D3DXVec3Normalize(&vDir, &vDir);
			
			BOOL bFind = FALSE;

			float fLength = 110.0f;
			
			if( !g_Map.IsNoBuildingUnit(vNowPos, vDir, fLength) )
			{
				bFind = TRUE;						
			}
			
			if(bFind)
			{
				SystemPrint( n_DarkRed, n_SystemMsg, "전방에 장애물이 있어서 텔레포트가 불가능합니다." );	///--HAN_MSG 
				return FALSE;
			}
			else
			{
				g_RockClient.Send_RpCsDirection( g_Pc.GetPlayer()->m_Mov.now_Yrad );
			}
		}
	}

	// aPressKey -> DIK_1 ~ DIK_12, DIK_LCONTROL, DIK_LSHIFT
	// 대상의 선택 되어 있어야 모든 스킬 사용 가능 ( 에러 : 선택된 대상 없음 ) 
	// 범위안에 있어야 사용 가능 ( 에러 : 범위안에 대상 없음 )
	// 피타격이 설정되어 있는 상태에서 해당 스킬에 맞는 거리 및 콤보게이지, SP, MP 수치 등에 맞는지 고려후 성공여부 판단 
	// 서버에 프로토콜에 맞게 스킬 사용함을 알림 
	// 스킬 사용 성공에 맞게 클라이언트 처리 
	// 물리, 마법 공격 스킬 처리 (이펙트, 사운드)
	if( g_Map.m_NpcTable[NonPlayerInfoWnd.thePickedNpcID].race == c_NpcRace_Item )
		return false;

	// 선택한 NPC 가 신수일경우 공격 하지 않음

	SSkillBaseInfo* skill_Info;
	skill_Info = g_Pc_Manager.GetSkillBaseInfoPtr( skill_code ); 
	if( skill_Info == NULL )
		return false;

	if( NonPlayerInfoWnd.thePickedNpcID != -1 )
	{
		///--JAPAN_BUG_MODIFY
		if( g_Map.m_Npc[NonPlayerInfoWnd.thePickedNpcID].m_State.nHolyState == nHolyState_Holy && 
			( ( skill_Info->theTarget == n_T_OTHERCHAR ) || 
			  ( skill_code == c_SkillCode_BasicAttack_Millena ) || 
			  ( skill_code == c_SkillCode_BasicAttack_Rain ) )		
			)
		{
			SystemPrint( n_DarkRed, n_SystemMsg,G_STRING_CHAR(IDS_SYSPR_INVALID_TARGET)); 
			return S_OK;
		}

		if( g_Map.m_Npc[NonPlayerInfoWnd.thePickedNpcID].m_State.nPetState == nPetState_Pet && 
			( ( skill_Info->theTarget == n_T_OTHERCHAR ) || 
			  ( skill_code == c_SkillCode_BasicAttack_Millena ) || 
			  ( skill_code == c_SkillCode_BasicAttack_Rain ) )	
			)
		{
			SystemPrint( n_DarkRed, n_SystemMsg,G_STRING_CHAR(IDS_SYSPR_INVALID_TARGET)); 
			return S_OK;
		}	

		if(  IsSameNationUnit( (void *)(&g_Map.m_Npc[NonPlayerInfoWnd.thePickedNpcID]) ) && 
			( ( skill_Info->theTarget == n_T_OTHERCHAR ) || 
			  ( skill_code == c_SkillCode_BasicAttack_Millena ) || 
			  ( skill_code == c_SkillCode_BasicAttack_Rain ) )	
			)
		{
			SystemPrint( n_DarkRed, n_SystemMsg,G_STRING_CHAR(IDS_SYSPR_INVALID_TARGET)); 
			return S_OK;
		}
	}

	///-- 아이템 자동 루팅 
	if( skill_code == c_SkillCode_Basic_PickUp )
	{
		nRui->ItemPickUp();
		///--PRINT_DLG_LOG("아이템 자동 루팅");
		return true;		
	}
	
	// 스킬 테이블을 참조하여 사용 여부를 판단한다 	
	if( CheckEnableUseSkill( skill_code ) == false )
	{
		///--PRINT_DLG_LOG("스킬 테이블을 참조하여 사용여부 판단 skillcode %d", skill_code);
		return false;
	}
	
	///--PRINT_DLG_LOG("theTargetIsPlayer %d ,  theTargetIsPlayerCell %d ", theTargetIsPlayer ,  theTargetIsPlayerCell );

	// 스킬 사용 가능하면 서버에 알린다 
	if( theTargetIsPlayer )
	{
		if( g_Pc.GetPlayer()->m_curt_event.type == SM_STATE_DIE )
		{
//			SystemPrint( n_DarkRed, n_SystemMsg, "이미 죽어있는 타겟입니다" );
			return false;
		}
		///--PRINT_DLG_LOG("theTargetIsPlayer 서버에 보낸다" );
		g_Pc.GetPlayer()->Illusion_CsSkill( g_Pc.m_MainPC.lUnique, n_PC, skill_code ); 
	}
	else if( theTargetIsPlayerCell )
	{
		if( g_Pc.GetPlayer()->m_curt_event.type == SM_STATE_DIE )
		{
//			SystemPrint( n_DarkRed, n_SystemMsg, "이미 죽어있는 타겟입니다" );
			return false;
		}
		///--PRINT_DLG_LOG("theTargetIsPlayerCell 서버에 보낸다" );
		g_Pc.GetPlayer()->Illusion_CsSkill( g_Pc.m_MainPC.lDestBlock, n_Cell, skill_code ); 
	}
	else if( NonPlayerInfoWnd.thePickedNpcID != -1 )
	{
		if( g_Map.m_Npc[NonPlayerInfoWnd.thePickedNpcID].m_curt_event.type == SM_STATE_DIE )
		{
//			SystemPrint( n_DarkRed, n_SystemMsg, "이미 죽어있는 타겟입니다" );
			return false;
		}

		g_Pc.GetPlayer()->Illusion_CsSkill( g_Map.m_NpcTable[NonPlayerInfoWnd.thePickedNpcID].lUnique, n_NPC, skill_code ); 
		g_Pc.GetPlayer()->SetDirection( g_Map.m_Npc[NonPlayerInfoWnd.thePickedNpcID].GetPosTM()._41, g_Map.m_Npc[NonPlayerInfoWnd.thePickedNpcID].GetPosTM()._43 );
	}
	else if( NonPlayerInfoWnd.thePickedUserID != -1 && NonPlayerInfoWnd.thePickedUserID != MAX_USER_INTABLE )
	{
//		if( g_Pc.m_Pc[NonPlayerInfoWnd.thePickedUserID].m_curt_event.type == SM_STATE_DIE )
//		{
//			SystemPrint( n_DarkRed, n_SystemMsg, "이미 죽어있는 타겟입니다" );
//			return false;
//		}

		g_Pc.GetPlayer()->Illusion_CsSkill( g_Pc.m_PcTable[NonPlayerInfoWnd.thePickedUserID].lUnique, n_PC, skill_code ); 
		g_Pc.GetPlayer()->SetDirection( g_Pc.m_Pc[NonPlayerInfoWnd.thePickedUserID].GetPosTM()._41, g_Pc.m_Pc[NonPlayerInfoWnd.thePickedUserID].GetPosTM()._43 );
	}
	
	theIsUsedSkill = TRUE;
	UseSkillList.insert(map <DWORD , long>::value_type(skill_code, g_nowTime));
	if(skill_Info->theNeedSpelling)
	{
		theUseSpellSkill = TRUE;
		theUseSPellTime = g_nowTime;
	}

	if( // 아처 기본 공격시 컨디션 게이지 나온 상태
		g_Pc.GetPlayer()->m_curt_event.status == SM_BOW_ATTACK ||	
		g_Pc.GetPlayer()->m_curt_event.status == SM_BOW_READY  ||
		g_Pc.GetPlayer()->m_curt_event.status == SM_BOW_WAIT 
		)
	{
		g_Pc.GetPlayer()->m_curt_event.Clear();

		g_ArcherBar.End();
	}

	return true;
}


bool CSkillSystem::UseSkillWndMgr( int aSkillCode )
{
	static long delay_time = false;

	if( SAFE_TIME_COMPARE( g_nowTime , > , SAFE_TIME_ADD( delay_time , 1000 ) ) )
	{
		delay_time = g_nowTime;		
	}			
	else
	{
		return false;
	}

	if( g_ExchangeWnd.m_bIsExchangeLock )
	{
		SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_SKILL_ON_EX ) );		
		return false;
	}
	
	if( g_DelayBarWnd.IsVisible && DELAY_RIDE_ON == g_DelayBarWnd.GetDelay_Type() )
	{
		SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANNOT_USESKILL_INRIDING  ) );
		return false;
	}

	if( g_DelayBarWnd.IsVisible && DELAY_PVP_ROOM == g_DelayBarWnd.GetDelay_Type() )
	{
		SystemPrint( n_DarkRed, n_SystemMsg,G_STRING_CHAR(IDS_SYSPR_GO_PVPROOM_NOT_SKILL)); 
		return false;
	}

	//...........................................................................................................
	// 유료아이템 스킬 사용 유무
	//...........................................................................................................
	if( g_Pc.GetPlayer()->m_ISApproachWeapon )
	{
		if( !g_Pc_Manager.ISUseSkillOnApproachWeapon( aSkillCode ) )
		{
			return false;
		}
	}

	//...........................................................................................................
	// 개인 상점 루틴
	//...........................................................................................................
	if( aSkillCode == c_SkillCode_UserSellStore )
	{
		if( g_Pc.GetPlayer()->m_curt_event.status == SM_RESTSKILL_SITDOWN ||
			g_Pc.GetPlayer()->m_curt_event.status == SM_RESTSKILL_KEEPING ||
			g_Pc.GetPlayer()->m_curt_event.status == SM_RESTSKILL_STANDUP   )
		{
			return false;
		}


		if( !g_UserStoreSellWnd.ISUserSellMode() )
		{
			g_RockClient.Send_RpCsSkill( g_Pc.GetPlayer()->GetUnique() , g_Pc.GetPlayer()->GetUnique() ,
				n_PC , c_SkillCode_UserSellStore );

		}
		else if( g_UserStoreSellWnd.m_UserStoreSellOpenMode )
		{
			g_RockClient.Send_CsClosePCShop();
		}
		else
		{
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_USERSTORE_USE ) );
		}

		return true;
	}

	// aPressKey -> DIK_1 ~ DIK_12, DIK_LCONTROL, DIK_LSHIFT
	// 대상의 선택 되어 있어야 모든 스킬 사용 가능 ( 에러 : 선택된 대상 없음 ) 
	// 범위안에 있어야 사용 가능 ( 에러 : 범위안에 대상 없음 )
	// 피타격이 설정되어 있는 상태에서 해당 스킬에 맞는 거리 및 콤보게이지, SP, MP 수치 등에 맞는지 고려후 성공여부 판단 
	// 서버에 프로토콜에 맞게 스킬 사용함을 알림 
	// 스킬 사용 성공에 맞게 클라이언트 처리 
	// 물리, 마법 공격 스킬 처리 (이펙트, 사운드)	
	if( g_Map.m_NpcTable[NonPlayerInfoWnd.thePickedNpcID].race == c_NpcRace_Item )
		return false;

	
	SSkillBaseInfo* skill_Info;
	skill_Info = g_Pc_Manager.GetSkillBaseInfoPtr( aSkillCode ); 
	if( skill_Info == NULL )
		return false;	

	if( NonPlayerInfoWnd.thePickedNpcID != -1 )
	{
		//dongs 수정 자기자신이면 통과 .. 
		if( g_Map.m_Npc[NonPlayerInfoWnd.thePickedNpcID].m_State.nHolyState == nHolyState_Holy &&
				skill_Info->theTarget == n_T_OTHERCHAR ) 
				//	&& !(skill_Info->theTarget == n_T_SELF ) && !(skill_Info->theActTarget == n_AT_TARGETLAND) )
		{
			return S_OK;
		}
   
		if( g_Map.m_Npc[NonPlayerInfoWnd.thePickedNpcID].m_State.nPetState == nPetState_Pet && 
					skill_Info->theTarget == n_T_OTHERCHAR ) 
			//!( skill_Info->theTarget == n_T_SELF ) && !(skill_Info->theActTarget == n_AT_TARGETLAND)  )
		{
			SystemPrint( n_DarkRed, n_SystemMsg,G_STRING_CHAR(IDS_SYSPR_INVALID_TARGET)); 
			return S_OK;
		}
	
		if( IsSameNationUnit( (void *)(&g_Map.m_Npc[NonPlayerInfoWnd.thePickedNpcID]) ) && 
					skill_Info->theTarget == n_T_OTHERCHAR ) 
			//!( skill_Info->theTarget == n_T_SELF ) && !(skill_Info->theActTarget == n_AT_TARGETLAND)  )
		{
			SystemPrint( n_DarkRed, n_SystemMsg,G_STRING_CHAR(IDS_SYSPR_INVALID_TARGET)); 
			return S_OK;
		}
	}
	
	///--현재 맵이 콜로니 맵일 경우
	if( g_Map.m_uMapType == MAP_TYPE_COLONY )
	{
		///-- 스킬 코드가 텔레포트 일경우
		if( aSkillCode >= c_SkillCode_Teleport_Min && aSkillCode <= c_SkillCode_Teleport_Max )
		{
			///-- 도착 예상 지점과 현재 위치 사이에 건물이 존재하는지 여부를 확인
			///-- 확인되면... 텔레포트를 막아준다.
			D3DXVECTOR3 vNowPos = D3DXVECTOR3( g_Pc.GetPlayer()->GetPosTM()._41,
											   g_Pc.GetPlayer()->GetPosTM()._42,
											   g_Pc.GetPlayer()->GetPosTM()._43 );

			D3DXVECTOR3 vDir = g_Pc.GetPlayer()->m_vDir;
			D3DXVec3Normalize(&vDir, &vDir);
			
			BOOL bFind = FALSE;

			float fLength = 110.0f;
			
			if( !g_Map.IsNoBuildingUnit(vNowPos, vDir, fLength) )
			{
				bFind = TRUE;						
			}
			
			if(bFind)
			{
				SystemPrint( n_DarkRed, n_SystemMsg, "전방에 장애물이 있어서 텔레포트가 불가능합니다." );	///--HAN_MSG 
				return FALSE;
			}
			else
			{
				g_RockClient.Send_RpCsDirection( g_Pc.GetPlayer()->m_Mov.now_Yrad );
			}
		}
	}
	
	///-- 아이템 자동 루팅 
	if( aSkillCode == c_SkillCode_Basic_PickUp )
	{
		nRui->ItemPickUp();
		return true;		
	}	
	
	// 스킬 테이블을 참조하여 사용 여부를 판단한다 	
	if( CheckEnableUseSkill( aSkillCode ) == false )
	{
		return false;
	}
	
	// 스킬 사용 가능하면 서버에 알린다 
	if( theTargetIsPlayer )
	{
		if( g_Pc.GetPlayer()->m_curt_event.type == SM_STATE_DIE )
		{
//			SystemPrint( n_DarkRed, n_SystemMsg, "이미 죽어있는 타겟입니다" );
			return false;
		}
	
		g_Pc.GetPlayer()->Illusion_CsSkill( g_Pc.m_MainPC.lUnique, n_PC, aSkillCode ); 
	}
	else if( theTargetIsPlayerCell )
	{
		if( g_Pc.GetPlayer()->m_curt_event.type == SM_STATE_DIE )
		{
//			SystemPrint( n_DarkRed, n_SystemMsg, "이미 죽어있는 타겟입니다" );
			return false;
		}
	
		g_Pc.GetPlayer()->Illusion_CsSkill( g_Pc.m_MainPC.lDestBlock, n_Cell, aSkillCode ); 
	}
	else if( NonPlayerInfoWnd.thePickedNpcID != -1 )
	{
		if( g_Map.m_Npc[NonPlayerInfoWnd.thePickedNpcID].m_curt_event.type == SM_STATE_DIE )
		{
//			SystemPrint( n_DarkRed, n_SystemMsg, "이미 죽어있는 타겟입니다" );
			return false;
		}
		
		g_Pc.GetPlayer()->Illusion_CsSkill( g_Map.m_NpcTable[NonPlayerInfoWnd.thePickedNpcID].lUnique, n_NPC, aSkillCode ); 
		g_Pc.GetPlayer()->SetDirection( g_Map.m_Npc[NonPlayerInfoWnd.thePickedNpcID].GetPosTM()._41, g_Map.m_Npc[NonPlayerInfoWnd.thePickedNpcID].GetPosTM()._43 );
	}
	else if( NonPlayerInfoWnd.thePickedUserID != -1 && NonPlayerInfoWnd.thePickedUserID != MAX_USER_INTABLE )
	{
		if( g_Pc.m_Pc[NonPlayerInfoWnd.thePickedUserID].m_curt_event.type == aSkillCode )
		{			
//			SystemPrint( n_DarkRed, n_SystemMsg, "이미 죽어있는 타겟입니다" );
			return false;
		}		
	
		g_Pc.GetPlayer()->Illusion_CsSkill( g_Pc.m_PcTable[NonPlayerInfoWnd.thePickedUserID].lUnique, n_PC, aSkillCode ); 
		g_Pc.GetPlayer()->SetDirection( g_Pc.m_Pc[NonPlayerInfoWnd.thePickedUserID].GetPosTM()._41, g_Pc.m_Pc[NonPlayerInfoWnd.thePickedUserID].GetPosTM()._43 );
	}
	
	theIsUsedSkill = TRUE;
	UseSkillList.insert(map <DWORD , long>::value_type(aSkillCode, g_nowTime));
	
	if(skill_Info->theNeedSpelling)
	{
		theUseSpellSkill = TRUE;
		theUseSPellTime = g_nowTime;
	}

	if( // 아처 기본 공격시 컨디션 게이지 나온 상태
		g_Pc.GetPlayer()->m_curt_event.status == SM_BOW_ATTACK ||	
		g_Pc.GetPlayer()->m_curt_event.status == SM_BOW_READY  ||
		g_Pc.GetPlayer()->m_curt_event.status == SM_BOW_WAIT 
		)
	{
		g_Pc.GetPlayer()->m_curt_event.Clear();

		g_ArcherBar.End();
	}

	return false;
}


void CSkillSystem::UseSkillTargetMgr( Character* pSrcChr, EVENT_DATA_INFO& aSkillEvent, _STargetInfo aTargetInfo )
{
	// 흠...때리는 것도 있고 도와주는 것도 있으니까 부과효과에 따라서 처리해야 될듯..일단은 걍 가구...
	// 부과효과 타입에 맞게 임펙트시에 따라 맞는 모션과 특수효과를 그려준다 	
	// tg_Info.theEffectType1 각각의 타입에 맞게 theEffectValue1 를 적용시키고...
	// theAddEffectType 각각의 타입에 맞게 상태 변경을 시키고
	// theKeepupTimeType에 맞게 theAddEffectTime 으로 남은 시간을 표시하여 준다 
	// 해당 스킬에 맞았을때에도 정해진 맞는 모션을 애니메이션 시켜준다 
	EVENT_DATA_INFO underattack_event;	

	if( aTargetInfo.thePushedCell )
	{
		underattack_event.type = SM_SLIDE_NPC;
		underattack_event.cell = aTargetInfo.thePushedCell;
	}				
	else
	{
//		if( aSkillEvent.loop == false )		// 죽음
		if( aTargetInfo.theDieFlag )		
		{
			underattack_event.type = SM_STATE_DIE;
			underattack_event.loop = !(aTargetInfo.theDieFlag);
		}
		else
		{
			underattack_event.type = SM_UNDERATTACK;
		}
	}
	
	//.......................................................................................................
	// 이펙트 매니저에 스킬 타입및 셀 값을 입력
	//.......................................................................................................
	switch( aSkillEvent.race )
	{
		case n_NPC:
			g_Particle.m_EffectManager.SetNPC();	
			break;
		case n_PC:
			g_Particle.m_EffectManager.SetPC();	
			break;
		case n_Cell:
			g_Particle.m_EffectManager.SetCell( aSkillEvent.x , aSkillEvent.y , aSkillEvent.vPos );
			break;

	}
	//.......................................................................................................


	// (필살기계 1, 공격 마법계 2)외에는 맞는 모션이 생기지 않음  
	SSkillBaseInfo* skill_Info;
	skill_Info = g_Pc_Manager.GetSkillBaseInfoPtr( aSkillEvent.count ); 
	if( skill_Info == NULL )
		return;

	int skilledIdx = 0;

	if( aTargetInfo.theTargetType == n_PC )
	{
		//........................................................................................................
		// 1. 내 캐릭터가 맞았을때 처리 by wxywxy
		//........................................................................................................
		if( g_Pc.GetMainPC().lUnique == aTargetInfo.theTargetID )
		{
//			if( g_Pc.GetPlayer()->m_bIsLoaded == true )
			{
				CreateSkill( pSrcChr , g_Pc.GetPlayer() , MAX_USER_INTABLE , skill_Info , underattack_event , aSkillEvent , aTargetInfo );

//				g_Pc.GetPlayer()->SetDirection( pSrcChr->GetPosTM()._41, pSrcChr->GetPosTM()._43 );
				
				#ifdef REAPPEAR_DIFFICULT_BUG_LOG
				///-- 내 파티가 나를 죽였다??
				if( pSrcChr->m_bPC && aTargetInfo.theDieFlag )
				{
					if( ::IsMyPartyAndLeagueMember( g_Pc.m_PcTable[ pSrcChr->m_nTableIndex ].lUnique ) )
					{
						ClientFilelog("My Party Kill Me!!!");
					}
				}
				#endif
			}			
		}
		//........................................................................................................
		// 2. 다른 유저가 맞았을때 처리 맞았을때 처리 by wxywxy
		//........................................................................................................
		else
		{
			skilledIdx = g_Pc.FindUser( aTargetInfo.theTargetID );
			
			if( skilledIdx == MAX_USER_INTABLE )
				return;
			
			// 맞는 모션 처리...이것은 효과 끝날때 맞추기가 어렵군...흠...
			// 흠...n_DeathBlow 일때는 지금 처리해 주고, n_AttackMagic 일때는 마법 특수효과가 끝날때 처리?!!...
			if( g_Pc.m_Pc[skilledIdx].m_bIsLoaded == true )
			{		
				CreateSkill( pSrcChr , &g_Pc.m_Pc[skilledIdx] , skilledIdx , skill_Info , underattack_event , aSkillEvent , aTargetInfo );

//				g_Pc.m_Pc[skilledIdx].SetDirection( pSrcChr->GetPosTM()._41, pSrcChr->GetPosTM()._43 );
			
			}
		}
		
	}
	//............................................................................................................
	// 3. NPC 맞았을때 처리 맞았을때 처리 by wxywxy
	//............................................................................................................
	else if( aTargetInfo.theTargetType == n_NPC )
	{
		skilledIdx = g_Map.FindNpc( aTargetInfo.theTargetID );
		
		if( skilledIdx == MAX_NPC_INTABLE )
			return;
		
		if( g_Map.m_Npc[skilledIdx].m_bIsLoaded == true )
		{
			CreateSkill( pSrcChr , &g_Map.m_Npc[skilledIdx] , skilledIdx , skill_Info , underattack_event , aSkillEvent , aTargetInfo );

//			g_Map.m_Npc[skilledIdx].SetDirection( pSrcChr->GetPosTM()._41, pSrcChr->GetPosTM()._43 );
		}
	}

	return;
}


// 현재 버튼에 선택되어 있는 스킬을 알아내어 사용한다. 
int CSkillSystem::GetSkillCode( int aPressKey )
{
	int skill_index = 0;
	
	switch( aPressKey )
	{
	case DIK_1:
		skill_index = n_SkillKey_1;
		break;
	case DIK_2:
		skill_index = n_SkillKey_2;
		break;
	case DIK_3:
		skill_index = n_SkillKey_3;
		break;
	case DIK_4:
		skill_index = n_SkillKey_4;
		break;
	case DIK_5:
		skill_index = n_SkillKey_5;
		break;
	case DIK_6:
		skill_index = n_SkillKey_6;
		break;
	case DIK_7:
		skill_index = n_SkillKey_7;
		break;
	case DIK_8:
		skill_index = n_SkillKey_8;
		break;
	case DIK_9:
		skill_index = n_SkillKey_9;
		break;
	case DIK_0:
		skill_index = n_SkillKey_0;
		break;
	case DIK_F1:
		skill_index = n_SkillKey_F1;
		break;
	case DIK_F2:
		skill_index = n_SkillKey_F2;
		break;
	case DIK_F3:
		skill_index = n_SkillKey_F3;
		break;
	case DIK_F4:
		skill_index = n_SkillKey_F4;
		break;
	case DIK_F5:
		skill_index = n_SkillKey_F5;
		break;
	case DIK_F6:
		skill_index = n_SkillKey_F6;
		break;
	case DIK_F7:
		skill_index = n_SkillKey_F7;
		break;
	case DIK_F8:
		skill_index = n_SkillKey_F8;
		break;
	case DIK_F9:
		skill_index = n_SkillKey_F9;
		break;
	case DIK_F10:
		skill_index = n_SkillKey_F0;
		break;
	case DIK_LCONTROL:
		skill_index = n_SkillKey_LCONTROL;
		break;		
	default:
		{
		
		}
		break;
	}


	if( aPressKey > 0xff )
	{
		int PressKey = aPressKey - 0xff; 
		
		switch(PressKey)
		{
		case DIK_F1:
			skill_index = n_SkillKey_SF1;
			break;
		case DIK_F2:
			skill_index = n_SkillKey_SF2;
			break;
		case DIK_F3:
			skill_index = n_SkillKey_SF3;
			break;
		case DIK_F4:
			skill_index = n_SkillKey_SF4;
			break;
		case DIK_F5:
			skill_index = n_SkillKey_SF5;
			break;
		case DIK_F6:
			skill_index = n_SkillKey_SF6;
			break;
		case DIK_F7:
			skill_index = n_SkillKey_SF7;
			break;
		case DIK_F8:
			skill_index = n_SkillKey_SF8;
			break;
		case DIK_F9:
			skill_index = n_SkillKey_SF9;
			break;
		case DIK_F10:
			skill_index = n_SkillKey_SF0;
			break;
		case DIK_LCONTROL:
			skill_index = n_SkillKey_LCONTROL;
			break;		

		}

		
	}

	int skill_code = 0;

	// 현재 눌려진 키에 해당하는 스킬을 리턴한다 
	if( aPressKey == DIK_LCONTROL ) 
	{
		skill_code = theShortCut_SkillCode[skill_index];		
	}
	else
	{
		skill_code = theQuickSlot_SkillCode[skill_index];		
	}


	theQuickSlotIndex = skill_index;

	return skill_code;	
}

	
bool CSkillSystem::CheckEnableUseSkill( DWORD aSkillCode )
{
	SSkillBaseInfo* skill_Info;
	skill_Info = g_Pc_Manager.GetSkillBaseInfoPtr( aSkillCode ); 
	if( skill_Info == NULL )
		return false;

	if( ( aSkillCode >= c_SkillCode_BreathControl_Min && aSkillCode <= c_SkillCode_BreathControl_Max ) || 
		( aSkillCode >= c_SkillCode_Meditation_Min && aSkillCode <= c_SkillCode_Meditation_Max ) )
	{
		if( g_Pc.GetPlayer()->m_curt_event.type == SM_USE_SKILL && 
			g_Pc.GetPlayer()->m_curt_event.status == SM_RESTSKILL_KEEPING )
		{
			///-- 이동 불가	- 이상황은 현재 휴식 스킬중인지를 체크하는 루틴...;;;
			///-- 여기에서만은 예외적인 상황인가...;;;
			g_RockClient.Send_RpCsStartMove( g_Pc.m_MainPC.lUnique, g_Pc.m_MainPC.lDestBlock, 0 );		
		
			return false;
		}		
	}
	// 기본 공격 스킬을 좀 다르게 처리 ^ ^;;
	else if( aSkillCode == c_SkillCode_BasicAttack_Millena || aSkillCode == c_SkillCode_BasicAttack_Rain )
	{
		// 기본 공격도 딜레이가 있다!!!
		if( CheckDelayTime( skill_Info ) == false )
		{			
		}
		// 일반 공격 스킬을 써도 상태에 따라서 가능하게 설정 
		else if( g_Pc.GetPlayer()->m_curt_event.type == SM_SLIDE_NPC ||				// 크리티컬에 맞아 밀려나거나 죽었을땐 이동 및 공격 불가 
				g_Pc.GetPlayer()->m_curt_event.type == SM_STATE_DIE ||
				g_Pc.GetPlayer()->m_curt_event.type == SM_ATTACK_NPC || 
				( g_Pc.GetPlayer()->m_curt_event.type == SM_USE_SKILL && g_Pc.GetPlayer()->m_curt_event.status != SM_RESTSKILL_KEEPING ) ||			
				g_Pc.GetPlayer()->m_curt_event.status == SM_RESTSKILL_SITDOWN ||
				g_Pc.GetPlayer()->m_curt_event.status == SM_RESTSKILL_STANDUP ) 			
		{			
		}
		// 개인 상점시 공격 안되게.....
		else if( g_Pc.GetPlayer()->m_curt_event.type == SM_USE_SKILL && 
			     ( g_Pc.GetPlayer()->m_curt_event.status == SM_SELLSKILL_KEEPING ||			
				   g_Pc.GetPlayer()->m_curt_event.status == SM_SELLSKILL_SITDOWN ||
				   g_Pc.GetPlayer()->m_curt_event.status == SM_SELLSKILL_STANDUP ) 	)
		{
		}

		// 기본 공격 스킬이니까 기본 공격으로 처리 ^ ^V
		else
		{	
			if( g_Pc.GetPlayer()->GetAddEffectState(n_TypeNotAttack))
			{
				return S_OK;
			}

			if( NonPlayerInfoWnd.thePickedNpcID != -1 )
			{
				if( g_Pc.GetPlayer()->m_curt_event.type == SM_ATTACK_DELAY && 
					g_Pc.GetPlayer()->m_curt_event.tbl_Idx == NonPlayerInfoWnd.thePickedNpcID )
				{
					return false;
				}
				else if( g_Pc.GetPlayer()->m_curt_event.type == SM_BOW_ATTACK && 
						 g_Pc.GetPlayer()->m_save_event.tbl_Idx == NonPlayerInfoWnd.thePickedNpcID )
				{
					return false;
				}
				
				int nRace = g_Pc.GetPlayer()->m_curt_event.race;
				int nIndex = g_Pc.GetPlayer()->m_curt_event.tbl_Idx;

				int nGapTime = c_PickSameAttack_DelayTime;

				if( nRace != g_Map.m_NpcTable[NonPlayerInfoWnd.thePickedNpcID].race || nIndex != NonPlayerInfoWnd.thePickedNpcID )
				{
					nGapTime = c_PickOtherAttack_DelayTime;
				}				

				// 콤보 아닌 공격은 짧은시간에 여러번 발생안됨 
				if( SAFE_TIME_COMPARE( SAFE_TIME_ADD( g_RockClient.m_PickAttack_DelayTime , nGapTime ) , > , g_nowTime ) )
				{
					return false;
				}

				EVENT_DATA_INFO pickNPC_event;
				pickNPC_event.type = CM_MOVE_ATTACK;
				pickNPC_event.motion = n_Attack9_Basic0;
				pickNPC_event.tbl_Idx = NonPlayerInfoWnd.thePickedNpcID;
				pickNPC_event.race	 = g_Map.m_NpcTable[NonPlayerInfoWnd.thePickedNpcID].race;
				pickNPC_event.index	 = g_Map.m_NpcTable[NonPlayerInfoWnd.thePickedNpcID].index;
				pickNPC_event.unique = g_Map.m_NpcTable[NonPlayerInfoWnd.thePickedNpcID].lUnique;
				pickNPC_event.x		 = g_Map.m_NpcTable[NonPlayerInfoWnd.thePickedNpcID].lDestBlock%(g_Map.m_cSize*2);
				pickNPC_event.y		 = g_Map.m_NpcTable[NonPlayerInfoWnd.thePickedNpcID].lDestBlock/(g_Map.m_cSize*2);
				pickNPC_event.vPos.x = g_Map.m_Npc[NonPlayerInfoWnd.thePickedNpcID].GetPosTM()._41;
				pickNPC_event.vPos.y = g_Map.m_Npc[NonPlayerInfoWnd.thePickedNpcID].GetPosTM()._42;
				pickNPC_event.vPos.z = g_Map.m_Npc[NonPlayerInfoWnd.thePickedNpcID].GetPosTM()._43;
				pickNPC_event.cell = g_Map.m_NpcTable[NonPlayerInfoWnd.thePickedNpcID].lDestBlock;
				g_Pc.GetPlayer()->PushQueue( pickNPC_event, TRUE );

				MainMenuWnd.SetSkillDelayTime( aSkillCode );
			}
			else if( NonPlayerInfoWnd.thePickedUserID != -1 && NonPlayerInfoWnd.thePickedUserID != MAX_USER_INTABLE )
			{
				if( g_Pc.GetPlayer()->m_curt_event.type == SM_ATTACK_DELAY && 
					g_Pc.GetPlayer()->m_curt_event.tbl_Idx == NonPlayerInfoWnd.thePickedUserID )
				{
					return false;
				}
				else if( g_Pc.GetPlayer()->m_curt_event.type == SM_BOW_ATTACK && 
					g_Pc.GetPlayer()->m_save_event.tbl_Idx == NonPlayerInfoWnd.thePickedUserID )
				{
					return false;
				}

				if( g_Pc.GetPlayer()->m_ISApproachWeapon )
				{
					switch( g_Pc.m_Pc[ NonPlayerInfoWnd.thePickedUserID ].m_curt_event.status )
					{
					case SM_SELLSKILL_SITDOWN:
					case SM_SELLSKILL_KEEPING:
					case SM_SELLSKILL_STANDUP:
						return false;
					}
				}
				else if( g_Pc.GetPlayer()->theMapType == n_FightZone )
				{
					if( g_Pc.m_MainPC.nFlag == g_Pc.m_PcTable[NonPlayerInfoWnd.thePickedUserID].nFlag )
					{
						return false;
					}
				}
				else
				{
					if( !g_RockClient.CanAttackPlayer( &g_Pc.m_Pc[NonPlayerInfoWnd.thePickedUserID] ) )					
					{
						return false;
					}
				}

				int nRace = g_Pc.GetPlayer()->m_curt_event.race;
				int nIndex = g_Pc.GetPlayer()->m_curt_event.tbl_Idx;

				int nGapTime = c_PickSameAttack_DelayTime;

				if( nRace != g_Pc.m_PcTable[NonPlayerInfoWnd.thePickedUserID].char_info.race || nIndex != NonPlayerInfoWnd.thePickedUserID )
				{
					nGapTime = c_PickOtherAttack_DelayTime;
				}				

				// 콤보 아닌 공격은 짧은시간에 여러번 발생안됨 
				if( SAFE_TIME_COMPARE( SAFE_TIME_ADD( g_RockClient.m_PickAttack_DelayTime , nGapTime ) , > , g_nowTime ) )
				{
					return false;
				}	

				EVENT_DATA_INFO pickPC_event;
				pickPC_event.type	= CM_MOVE_ATTACK;
				pickPC_event.motion = n_Attack9_Basic0;
				pickPC_event.tbl_Idx = NonPlayerInfoWnd.thePickedUserID;
				pickPC_event.race	 = g_Pc.m_PcTable[NonPlayerInfoWnd.thePickedUserID].char_info.race;
				pickPC_event.index	 = g_Pc.m_PcTable[NonPlayerInfoWnd.thePickedUserID].nIndex;
				pickPC_event.unique	 = g_Pc.m_PcTable[NonPlayerInfoWnd.thePickedUserID].lUnique;
				pickPC_event.x		 = g_Pc.m_PcTable[NonPlayerInfoWnd.thePickedUserID].lDestBlock%(g_Map.m_cSize*2);
				pickPC_event.y		 = g_Pc.m_PcTable[NonPlayerInfoWnd.thePickedUserID].lDestBlock/(g_Map.m_cSize*2);
				pickPC_event.vPos.x = g_Pc.m_Pc[NonPlayerInfoWnd.thePickedUserID].GetPosTM()._41;
				pickPC_event.vPos.y = g_Pc.m_Pc[NonPlayerInfoWnd.thePickedUserID].GetPosTM()._42;
				pickPC_event.vPos.z = g_Pc.m_Pc[NonPlayerInfoWnd.thePickedUserID].GetPosTM()._43;
				pickPC_event.cell = g_Pc.m_PcTable[NonPlayerInfoWnd.thePickedUserID].lDestBlock;
				g_Pc.GetPlayer()->PushQueue( pickPC_event, TRUE );

				MainMenuWnd.SetSkillDelayTime( aSkillCode );
			}

//			RLG1( "g_Pc.GetPlayer()->m_curt_event.type = %d", g_Pc.GetPlayer()->m_curt_event.type );
		}
		return false;
	}


	// 대상, 타이밍(필살기계), 사정 거리, 콤보게이지, 프로이마게이지, 컨디션 게이지, HP-SP-MP, 무기 유무...체크!!!
	// 필요장비, 딜레이(100ms), 
	// 1. 대상 비교 (때려도 되는 눔이냐?!!!)
	// 2. 필살기 계 일때 타이밍에 맞게 눌렀는지 판단 
	// 3. 딜레이 시간(쿨타임)이 지났는지 확인 
	// 4. 스킬 사정거리에 들어왔는지 체크!!
	// 5. 콤보게이지, 프로이마게이지, 컨디션 게이지, HP-SP-MP 등 필요한 능력치에 맞는 상태인지 확인!!!
	// 6. 필요 장비 착용 여부 확인
	
	//by dongs 임시 다 ..
	/*
	if( CheckAttackTarget( skill_Info ) == false || 
		CheckTargetStatus( skill_Info ) == false || 		
		CheckAttackFrame( skill_Info ) == false || 
		CheckDelayTime( skill_Info ) == false ||
		CheckAttackRange( skill_Info ) == false || 
		CheckNeedStatus( skill_Info ) == false ||
		CheckNeedItem( skill_Info ) == false )
	{
		return false;
	}
	*/
	
	if( CheckAttackTarget( skill_Info ) == false )
	{
		///--PRINT_DLG_LOG( "CheckAttackTarget Error " );
		return false; 

	}

	if( CheckTargetStatus( skill_Info ) == false )
	{
		///--PRINT_DLG_LOG( "CheckTargetStatus Error " );
		return false; 

	}

	if( CheckAttackFrame( skill_Info ) == false )
	{
		///--PRINT_DLG_LOG( "CheckAttackFrame Error " );
		return false; 

	}

	if( CheckAttackRange( skill_Info ) == false )
	{
		///--PRINT_DLG_LOG( "CheckAttackRange Error " );
		return false; 

	}

	if( CheckNeedStatus( skill_Info ) == false )
	{
		///--PRINT_DLG_LOG( "CheckNeedStatus Error " );
		return false; 

	}

	if( CheckNeedItem( skill_Info ) == false )
	{
		///--PRINT_DLG_LOG( "CheckNeedItem Error " );
		return false; 

	}

	// 지속성 지역 타겟 마법일 경우 마우스를 클릭해야 스킬 사용. 지역 타겟인지 체크.  
	if( skill_Info->theActTarget == n_AT_TARGETLAND )
//	if( skill_Info->theActTarget == n_AT_SELFLAND )			// test용 코드 
	{	
		// UserInputProc() 제어 해준다. 
		g_RockClient.m_MouseControlType = n_MouseType_UseSkill;
		
		theTargetLand_SkillCode = aSkillCode;

		g_Particle.m_EffectManager.SetLandMark( TRUE, g_Particle.m_EffectManager.GetAttackRange( aSkillCode ) * 2.0f + 16.0f , skill_Info->theRange * 8.0f + 8.0f );		
	

		return false;
	}

	return true;
}
	
bool CSkillSystem::CheckTargetStatus( SSkillBaseInfo* aSkill_Info )
{
	if( g_RockClient.m_MouseControlType == n_MouseType_UseSkill )
	{
		///--PRINT_DLG_LOG("스킬불가 - g_RockClient.m_MouseControlType == n_MouseType_UseSkill");
		return false;
	}

	// 부가효과 (스킬불가) 처리 
	if( g_Pc.GetPlayer()->GetAddEffectState(n_TypeNotSkill))
	{
		return false;
	}
	
	map < DWORD, long >::iterator iPos = UseSkillList.find((DWORD)aSkill_Info->theSkillCode);	
	if(iPos != UseSkillList.end())
	{
		long lTime = (*iPos).second;
		if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( g_nowTime , lTime ) , > , 1000 ) )
		{
			UseSkillList.erase(iPos);
		}
		else
		{		
			return false;
		}
	}

	if(theUseSpellSkill)
	{
		if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( g_nowTime , theUseSPellTime ) , > , 2500 ) )
		{
			theUseSpellSkill = FALSE;
		}
		else
		{
			return false;
		}
	}

	// 호흡조절, 명상 토클처럼 사용 
	if( g_Pc.GetPlayer()->m_curt_event.status == SM_RESTSKILL_KEEPING )
	{
		if( ( aSkill_Info->theSkillCode >= c_SkillCode_BreathControl_Min && aSkill_Info->theSkillCode <= c_SkillCode_BreathControl_Max ) || 
			( aSkill_Info->theSkillCode >= c_SkillCode_Meditation_Min && aSkill_Info->theSkillCode <= c_SkillCode_Meditation_Max ) )
		{
			return true;
		}
	}

	int chrAttrIndex = g_Pc.GetPlayer()->GetAttrIndex();
	int chrClassType = nRui->thePcParam.MainClass;

	if(	// 레인 남-녀, 직업은 스트레인저, 소서러, 서머너 
		( chrAttrIndex == n_Rain_Man || chrAttrIndex == n_Rain_Woman ) &&
		( chrClassType == Stranger || chrClassType == Magician || chrClassType == Prominas || chrClassType == Priest || chrClassType == Holy_Avenger ) 		 
	  )			
	{
		// 레인은 기본 공격이나 스킬이 끝난다음에 사용할 수 있다 
		if( g_Pc.GetPlayer()->m_curt_event.type == SM_ATTACK_NPC ||
			g_Pc.GetPlayer()->m_curt_event.type == SM_USE_SKILL )
		{
//			SystemPrint( n_DarkRed, n_SystemMsg, "스킬을 사용할 수 있는 상태가 아닙니다" );
					
			// 스킬 우선 사용 정책에 따라 레인도 언제든지 스킬 한번은 가능하게... 
			if( theIsUsedSkill == TRUE )
			{
				return false;
			}
		}
	}
	else if( ( chrAttrIndex == n_Millena_Man || chrAttrIndex == n_Millena_Woman ) )
	{
		if( aSkill_Info->theSkillCode >= c_SkillCode_BreathControl_Min && 
			aSkill_Info->theSkillCode <= c_SkillCode_BreathControl_Max ) 
		{
			// 호흡 조절 동안은 다른 스킬은 사용할 수가 없다. 취소한 후 사용 가능.
			if( g_Pc.GetPlayer()->m_curt_event.type == SM_ATTACK_NPC ||
				g_Pc.GetPlayer()->m_curt_event.type == SM_USE_SKILL )
			{
				///PRINT_DLG_LOG("호흡 조절 동안은 다른 스킬은 사용할 수가 없다 Line 918 " );

				return false;
			}
		}
		else if( aSkill_Info->theSkillType2 != n_DeathBlow )
		{
			if( g_Pc.GetPlayer()->m_curt_event.type == SM_ATTACK_NPC ||
				g_Pc.GetPlayer()->m_curt_event.type == SM_USE_SKILL )
			{

				if( theIsUsedSkill == TRUE )
				{				
					return false;
				}
			}
		}
	}	
	
	// 밀레나, 레인 공통으로 체크할 상태 
	if( g_Pc.GetPlayer()->m_curt_event.type == SM_SLIDE_NPC ||				
		g_Pc.GetPlayer()->m_curt_event.type == SM_STATE_DIE ||
		g_Pc.GetPlayer()->m_curt_event.status == SM_RESTSKILL_SITDOWN ||
		g_Pc.GetPlayer()->m_curt_event.status == SM_RESTSKILL_KEEPING ||
		g_Pc.GetPlayer()->m_curt_event.status == SM_RESTSKILL_STANDUP 		
		) 			
	{		
//		SystemPrint( n_DarkRed, n_SystemMsg, "스킬을 사용할 수 있는 상태가 아닙니다" );
		return false;
	}	

	if( // 아처 기본 공격시 컨디션 게이지 나온 상태
		g_Pc.GetPlayer()->m_curt_event.status == SM_BOW_ATTACK ||	
		g_Pc.GetPlayer()->m_curt_event.status == SM_BOW_READY  ||
		g_Pc.GetPlayer()->m_curt_event.status == SM_BOW_WAIT 
		)
	{
		if( theIsUsedSkill == TRUE )
		{
			return false;
		}		
	}
	
	if( theTargetIsPlayer || theTargetIsPlayerCell )
	{
		if( g_Pc.GetPlayer()->m_curt_event.type == SM_STATE_DIE )
		{
//			SystemPrint( n_DarkRed, n_SystemMsg, "이미 죽어있는 타겟입니다" );
			return false;
		}
	}
	else if( NonPlayerInfoWnd.thePickedNpcID != -1 )
	{
		if( g_Map.m_Npc[NonPlayerInfoWnd.thePickedNpcID].m_curt_event.type == SM_STATE_DIE )
		{
//			SystemPrint( n_DarkRed, n_SystemMsg, "이미 죽어있는 타겟입니다" );
			return false;
		}
	}
	else if( NonPlayerInfoWnd.thePickedUserID != -1 && NonPlayerInfoWnd.thePickedUserID != MAX_USER_INTABLE )
	{
//		if( g_Pc.m_Pc[NonPlayerInfoWnd.thePickedUserID].m_curt_event.type == SM_STATE_DIE )
//		{
//			SystemPrint( n_DarkRed, n_SystemMsg, "이미 죽어있는 타겟입니다" );
//			return false;
//		}
	}
	
	bool EnableSkill = true;
	bool CheckSkill = true;
	int need_skill_max = 0;
	
	int i = 0;
	if( aSkill_Info->thePreSkill1 )
	{
		for( i = 0; i < RP_MAX_KEEPUP_SKILL; ++i )
		{
			need_skill_max = ( ( aSkill_Info->thePreSkill1 / 10 ) * 10 ) + 10;			
			
			 if( aSkill_Info->thePreSkill1 >= c_SkillCode_Possess_Shaman_Min &&
				 aSkill_Info->thePreSkill1 <= c_SkillCode_Possess_Shaman_Max )
			 {
				 need_skill_max = aSkill_Info->thePreSkill1;
			 }			
			
			if( ( nRui->thePcInven.KeepupSkill[i] >= aSkill_Info->thePreSkill1 ) && 
				( nRui->thePcInven.KeepupSkill[i] <= need_skill_max ) ) 
			{				
				CheckSkill = false;
				break;
			}
		}

		if( i == RP_MAX_KEEPUP_SKILL )
		{
			EnableSkill = false;

//			SSkillBaseInfo* NeedSkill_Info = NULL;
//			SystemPrint( n_DarkRed, n_SystemMsg, "%s 스킬을 쓰여야 사용 가능합니다", NeedSkill_Info->theSkillName );
//			return false;
		}
	}
	if( CheckSkill && aSkill_Info->thePreSkill2 )
	{
		
		for(  i = 0; i < RP_MAX_KEEPUP_SKILL; ++i )
		{
			need_skill_max = ( ( aSkill_Info->thePreSkill2 / 10 ) * 10 ) + 10;			

			if( aSkill_Info->thePreSkill2 >= c_SkillCode_Possess_Shaman_Min &&
				aSkill_Info->thePreSkill2 <= c_SkillCode_Possess_Shaman_Max )
			{
				need_skill_max = aSkill_Info->thePreSkill2;
			}			
			
			if( ( nRui->thePcInven.KeepupSkill[i] >= aSkill_Info->thePreSkill2 ) && 
				( nRui->thePcInven.KeepupSkill[i] <= need_skill_max ) ) 
			{	
				EnableSkill = true;
				break;
			}
		}

		if( i == RP_MAX_KEEPUP_SKILL )
		{
			EnableSkill = false;
			
//			SSkillBaseInfo* NeedSkill_Info = NULL;
//			SystemPrint( n_DarkRed, n_SystemMsg, "%s 스킬을 쓰여야 사용 가능합니다", NeedSkill_Info->theSkillName );
//			return false;
		}
	}

	if( EnableSkill == false )
	{
		return false;
	}

	return true;
}


bool CSkillSystem::CheckAttackTarget( SSkillBaseInfo* aSkill_Info )
{
/*
enum NTarget
{ 
	n_T_NONE				 = 0,		//  
	n_T_SELF				 = 1,		//  자기자신					몬스터일 경우, 몬스터자신
	n_T_SAMECHAR_NOTSELF	 = 2,		//  문명이 같은 유저			몬스터일 경우, 다른 몬스터
	n_T_SAMECHAR			 = 3,		//	동문명 유저	& 본인			몬스터일 경우 자신과 자신의 동족들
	n_T_NPC					 = 4,		//  비전투형							-
	n_T_OTHERCHAR			 = 5,		//	타문명 유저 & 몬스터		몬스터일 경우, 모든 PC들
	n_T_ALL_CHAR			 = 6,		//  전체 
	n_T_LANDTARGET			 = 7,
	n_T_PARTY				 = 8,		// 파티원들을 대상
	n_T_LAND_SAMECHAR		 = 9,		// 지역범위
	n_T_LAND_OTHERCHAR		 = 10,
	n_T_LAND_PARTY			 = 11,
};

enum NActTarget
{
	n_AT_NONE				= 0,		// 대상이 없음. (본인)
	n_AT_SELF				= 1,
	n_AT_TARGET				= 2,		// 캐릭터 대상 (목표 설정후 스킬 시전)
	n_AT_SELFLAND			= 3,		// 본인이 서있는 좌표 
	n_AT_TARGETLAND			= 4,		// 다른 좌표 (스킬 시전후 목표 설정)
};
*/
	theTargetIsPlayer = FALSE;
	theTargetIsPlayerCell = FALSE;

	if( aSkill_Info->theActTarget == n_AT_NONE )
	{
		theTargetIsPlayer = TRUE;
		return true;
	}
	else if( aSkill_Info->theActTarget == n_AT_SELF )
	{
		theTargetIsPlayer = TRUE;
		return true;
	}
	else if( aSkill_Info->theActTarget == n_AT_TARGET )
	{
		if( aSkill_Info->theTarget == n_T_NONE )				// 대상이 없당...ㅡㅡ;
		{
		}
		else if( aSkill_Info->theTarget == n_T_SELF )
		{
			theTargetIsPlayer = TRUE;
			return true;
		}
		else if( aSkill_Info->theTarget == n_T_SAMECHAR_NOTSELF )
		{
			if( NonPlayerInfoWnd.thePickedUserID != -1 )	
			{				
				if( NonPlayerInfoWnd.thePickedUserID == MAX_USER_INTABLE )
				{
					return false;
				}
				
				if( g_Pc.GetPlayer()->theMapType == n_FightZone )
				{
					if( g_Pc.m_MainPC.nFlag == g_Pc.m_PcTable[NonPlayerInfoWnd.thePickedUserID].nFlag )
					{
						return true;
					}
				}
				else
				{
					if( !g_RockClient.CanAttackPlayer( &g_Pc.m_Pc[NonPlayerInfoWnd.thePickedUserID] ) )	
					{
						return true;
					}
				}	
			}
		}
		else if( aSkill_Info->theTarget == n_T_SAMECHAR )				// 애매한 대상은 처리 하지 않기로 결정 ^ ^/ 서버의 판단에 맡기자...
		{	
			if( NonPlayerInfoWnd.thePickedUserID != -1 )	
			{
				if( NonPlayerInfoWnd.thePickedUserID == MAX_USER_INTABLE )
				{
					theTargetIsPlayer = TRUE;
					return true;
				}				
				
				if( g_Pc.GetPlayer()->theMapType == n_FightZone )
				{
					if( g_Pc.m_MainPC.nFlag == g_Pc.m_PcTable[NonPlayerInfoWnd.thePickedUserID].nFlag )
					{
						return true;
					}
				}
				else
				{
					if( !g_RockClient.CanAttackPlayer( &g_Pc.m_Pc[NonPlayerInfoWnd.thePickedUserID] ) )	
					{
						return true;
					}
				}	
			}
		}
		else if( aSkill_Info->theTarget == n_T_NPC )
		{
			if( NonPlayerInfoWnd.thePickedNpcID != -1 )	
			{
				return true;
			}
		}
		else if( aSkill_Info->theTarget == n_T_OTHERCHAR )
		{
			if( NonPlayerInfoWnd.thePickedUserID != -1 )	
			{
				if( NonPlayerInfoWnd.thePickedUserID == MAX_USER_INTABLE )
				{
					return false;
				}				
				
				if( g_Pc.GetPlayer()->theMapType == n_FightZone )
				{
					if( g_Pc.m_MainPC.nFlag != g_Pc.m_PcTable[NonPlayerInfoWnd.thePickedUserID].nFlag )
					{
						return true;
					}
				}
				else
				{
					if( g_RockClient.CanAttackPlayer( &g_Pc.m_Pc[NonPlayerInfoWnd.thePickedUserID] ) )	
					{
						return true;
					}
				}				
			}
			
			if( NonPlayerInfoWnd.thePickedNpcID != -1 )	
			{
				return true;
			}
		}
		else if( aSkill_Info->theTarget == n_T_ALL_CHAR )	
		{	
			if( NonPlayerInfoWnd.thePickedUserID == MAX_USER_INTABLE )
			{
				theTargetIsPlayer = TRUE;
				return true;
			}			
			
			if( NonPlayerInfoWnd.thePickedUserID != -1 )	
			{
				return true;
			}
			else if( NonPlayerInfoWnd.thePickedNpcID != -1 )	
			{
				return true;
			}
		}	
		else if( aSkill_Info->theTarget == n_T_PARTY )		
		{	
			if( g_PartyInforWnd.IsMemberParty() )
			{
				return true;
			}
		}	
		else if( aSkill_Info->theTarget == n_T_LANDTARGET )		
		{	
		}
		else if( aSkill_Info->theTarget == n_T_LAND_SAMECHAR )		
		{	
		}
		else if( aSkill_Info->theTarget == n_T_LAND_OTHERCHAR )	
		{
		}
		else if( aSkill_Info->theTarget == n_T_LAND_PARTY )	
		{	
			if( g_PartyInforWnd.IsMemberParty() )
			{
				return true;
			}
		}
	}
	else if( aSkill_Info->theActTarget == n_AT_SELFLAND )
	{
		theTargetIsPlayerCell = TRUE;
		return true;
	}
	else if( aSkill_Info->theActTarget == n_AT_TARGETLAND )
	{	
		return true;
	}
		
//	SystemPrint( n_DarkRed, n_SystemMsg, "스킬을 사용할 수 있는 대상이 아닙니다" );
	return false;
}

bool CSkillSystem::CheckAttackFrame( SSkillBaseInfo* aSkill_Info )
{
	return true;

	if( aSkill_Info->theSkillType2 == n_DeathBlow )			// 필살기계 스킬일 경우에만 프레임 판정을 한다 
	{
		int chrAttrIndex = g_Pc.GetPlayer()->GetAttrIndex();
		int chrClassType = nRui->thePcParam.MainClass;
				

		// 아처일때는 프레임 불필요 By wxywxy
		if( ( chrAttrIndex == n_Millena_Man || chrAttrIndex == n_Millena_Woman ) &&
			( chrClassType == Wanderer || 
			  chrClassType == Archer || chrClassType == Sniper ) ) 
		{
			return TRUE;
		}
		else if( 
				(	// 밀레나 남-녀, 직업은 원더러, 소드맨, 나이트, 머시너리, 슬레이어  
					( chrAttrIndex == n_Millena_Man || chrAttrIndex == n_Millena_Woman ) &&
					( chrClassType == Wanderer || 
					  chrClassType == Swordman || chrClassType == Knight || chrClassType == Mercenary || chrClassType == Slayer ) 
				) ||
				(	// 레인 남-녀, 직업은 스트레인저, 소서러, 서머너 
					( chrAttrIndex == n_Rain_Man || chrAttrIndex == n_Rain_Woman ) &&
					( chrClassType == Stranger || 
					  chrClassType == Shaman || chrClassType == Psyche ) 
				)
			  )
		{
			int skill_state = g_Pc.GetPlayer()->theCombo_State;
			
			// 콤보 중일때는 스킬 중복 사용 금지 
			if( skill_state == n_Combo_Success ||
				skill_state == n_Combo_Disable ) 
			{
				return false; 
			}
			
			int curt_frame = g_Pc.GetPlayer()->m_ani_frame;
			int curt_motion = g_Pc.GetPlayer()->m_ani_index;
			int range_startframe = g_Pc_Manager.GetRangeStartFrame( g_Pc.GetPlayer(), curt_motion );
			int range_endframe = g_Pc_Manager.GetRangeEndFrame( g_Pc.GetPlayer(), curt_motion );
						
			if( skill_state != n_Combo_Disable )
			{
				if( curt_frame >= range_startframe && curt_frame <= range_endframe )
				{
					g_Pc.GetPlayer()->theCombo_State = n_Combo_Success;
					return true;			
				}
				else if( curt_frame < range_startframe || curt_frame > range_endframe )
				{				
 					g_Pc.GetPlayer()->theCombo_State = n_Combo_Disable;
				}
				else if( range_startframe == range_endframe )
				{
//					SystemPrint( n_DarkRed, n_SystemMsg, "스킬을 사용할 수 있는 타이밍이 아닙니다" );
				}
			}
		}
		// 마법계 
		else if(	// 레인 남-녀, 직업은 스트레인저, 소서러, 서머너 
				( chrAttrIndex == n_Rain_Man || chrAttrIndex == n_Rain_Woman ) &&
				( chrClassType == Stranger || 
				  chrClassType == Magician || chrClassType == Prominas || chrClassType == Priest || chrClassType == Holy_Avenger ) 		 
			   )		
		{
			return true;
		}
	}
	else
	{		
		return true;
	}

	return false;
}


bool CSkillSystem::CheckDelayTime( SSkillBaseInfo* aSkill_Info )
{
	if( theQuickSlot_DelayTime[theQuickSlotIndex] == false )
	{
		return false;
	}

	return true;
}

///--JAPAN_BUG_MODIFY 
bool CSkillSystem::CheckAttackRange( SSkillBaseInfo* aSkill_Info )
{	
	int AttackRange = aSkill_Info->theRange;
	int distance = 0;

	int nSPosX = 0;
	int nSPosY = 0;
	
	int nDPosX = 0;
	int nDPosY = 0;
	
	nSPosX = g_Pc.GetPlayer()->m_Mov.curt.x;
	nSPosY = g_Pc.GetPlayer()->m_Mov.curt.y;

	D3DXVECTOR3 vPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	if( theTargetIsPlayer || theTargetIsPlayerCell ||
		aSkill_Info->theActTarget == n_AT_TARGETLAND )
	{
		return true;
	}

	if( NonPlayerInfoWnd.thePickedNpcID != -1 )
	{
		vPos.x = g_Map.m_Npc[NonPlayerInfoWnd.thePickedNpcID].GetPosTM()._41;
		vPos.z = g_Map.m_Npc[NonPlayerInfoWnd.thePickedNpcID].GetPosTM()._43;		
		
		nDPosX = (DWORD)( vPos.x / (float)CLIENT_TILESIZE );
		nDPosY = (DWORD)( vPos.z / (float)CLIENT_TILESIZE );		

		distance = GetMax( abs( nSPosX - nDPosX ), abs( nSPosY - nDPosY ) );		
	}
	else if( NonPlayerInfoWnd.thePickedUserID != -1 )
	{
		if( NonPlayerInfoWnd.thePickedUserID == MAX_USER_INTABLE )
		{
			return true;
		}

		vPos.x = g_Pc.m_Pc[NonPlayerInfoWnd.thePickedUserID].GetPosTM()._41;
		vPos.z = g_Pc.m_Pc[NonPlayerInfoWnd.thePickedUserID].GetPosTM()._43;
		
		nDPosX = (DWORD)( vPos.x / (float)CLIENT_TILESIZE );
		nDPosY = (DWORD)( vPos.z / (float)CLIENT_TILESIZE );		

		distance = GetMax( abs( nSPosX - nDPosX ), abs( nSPosY - nDPosY ) );
	}

	if( distance <= AttackRange )
	{
		return true;
	}
	else
	{
//		SystemPrint( n_DarkRed, n_SystemMsg, "스킬의 사정거리에서 벗어났습니다" );
	}
	
	return false;
}


bool CSkillSystem::CheckNeedStatus( SSkillBaseInfo* aSkill_Info )
{
	if( aSkill_Info->theToggleFlag )
	{
		for( int i = 0; i < RP_MAX_KEEPUP_SKILL; ++i )
		{
			// 이미 발동된 토글형 스킬에 관해서는 다시 스킬을 쓰면 해제가 된다...해제 스킬은 게이지 소모 안되니까...^^			
			if( nRui->thePcInven.KeepupSkill[i] == aSkill_Info->theSkillCode )
			{				
				return true;
			}
		}
	}

	// 4. 콤보게이지, 프로이마게이지, 컨디션 게이지, HP-SP-MP, 무기 유무 등 필요한 능력치에 맞는 상태인지 확인!!!

	int gauge_type = nRui->GetPcParamInfo()->GageType;
	int gauge_value = aSkill_Info->theGageValue;

	// 필요 게이지 종류 ( 0:none, 1:comboG, 2:conditionG, 3:proimaG )
	switch( gauge_type )
	{
	case n_ComboGage:
		{
			if( gauge_value > nRui->thePcParam.Stat1.theGage )
			{
//				SystemPrint( n_DarkRed, n_SystemMsg, "콤보게이지가 부족합니다" );
				return false;
			}
		}
		break;
	case n_ConditionGage:
		{
			if( gauge_value > nRui->thePcParam.Stat1.theGage )
			{
				return false;
			}
		}
		break;
	case n_ProimaGage:
		{
			if( gauge_value > nRui->thePcParam.Stat1.theGage )
			{
//				SystemPrint( n_DarkRed, n_SystemMsg, "프로이마 게이지가 부족합니다" );
				return false;
			}
		}
		break;
	default:
		break;
	}

	if( aSkill_Info->theWasteHP > nRui->thePcParam.Stat1.theHP )
	{
//		SystemPrint( n_DarkRed, n_SystemMsg, "필요 체력이 부족합니다" );
		return false;
	}
	if( aSkill_Info->theWasteMP > nRui->thePcParam.Stat1.theMP )
	{
//		SystemPrint( n_DarkRed, n_SystemMsg, "필요 마력이 부족합니다" );
		return false;
	}
	if( aSkill_Info->theWasteSP > nRui->thePcParam.Stat1.theSP )
	{
//		SystemPrint( n_DarkRed, n_SystemMsg, "필요 기력이 부족합니다" );
		return false;
	}

	return true;
}


bool CSkillSystem::CheckNeedItem( SSkillBaseInfo* aSkill_Info )
{
/*
	n_Type_Sword		= 1,		// 1  검
	n_Type_TwohandSword	= 2,		// 2  대검
	n_Type_Bow			= 3,		// 3  활
	n_Type_MagicBook	= 4,		// 4  마법서
	n_Type_Staff		= 5,		// 5  스태프
	n_Type_CrystalBall	= 6,		// 6  수정구
	n_Type_Shield		= 100,		// 100 방패
*/
	SItemBaseInfo* item_Info;
	

	switch( aSkill_Info->theNeedItem )
	{
	case 0:				// 불필요 무조건 사용 가능 
		{
			return true;
		}
		break;
	case n_Type_Sword:
	case n_Type_TwohandSword:
	case n_Type_Staff:
	case n_Type_CrystalBall:
		{
			if( nRui->thePcInven.WearItem[n_WearSlot_Weapon] )
			{
				item_Info = g_Pc_Manager.GetItemBaseInfoPtr( nRui->thePcInven.WearItem[n_WearSlot_Weapon]->Code ); 

				if( item_Info->theType == aSkill_Info->theNeedItem )
				{
					return true;
				}		
			}
		}
		break;
	case n_Type_Bow:		
	case n_Type_MagicBook:
	case n_Type_Shield:
		{
			if( nRui->thePcInven.WearItem[n_WearSlot_Shield] )
			{
				item_Info = g_Pc_Manager.GetItemBaseInfoPtr( nRui->thePcInven.WearItem[n_WearSlot_Shield]->Code ); 

				if( item_Info->theType == aSkill_Info->theNeedItem )
				{
					return true;
				}		
			}			
		}
		break;
	default:
		break;
	}

//	SystemPrint( n_DarkRed, n_SystemMsg, "스킬 사용에 필요한 장비를 착용하셔야 합니다 " );
	return false;
}


//-----------------------------------------------------------------------------
void CSkillSystem::SaveShortcutSkill()
{
	FILE* fp = _tfopen( _T("Chr\\Shortcut.dat"), _T("w+t") );
	if( fp == NULL )
	{
		//MessageBox( NULL, _T("Shortcut 파일 열기 실패!!!"), _T("알림"), MB_OK );
		return;
	}

	fprintf( fp, "%d %d", theShortCut_SkillCode[0], theShortCut_SkillCode[1] );	

	fclose( fp );
}

//-----------------------------------------------------------------------------
void CSkillSystem::LoadShortcutSkill()
{
	FILE* fp = _tfopen( _T("Chr\\Shortcut.dat"), _T("rt") );
	if( fp == NULL )
	{
		//MessageBox( NULL, _T("Shortcut 파일 열기 실패!!!"), _T("알림"), MB_OK );
		return;
	}

	fscanf( fp, "%d %d", &theShortCut_SkillCode[0], &theShortCut_SkillCode[1] );	

	fclose( fp );
}


void CSkillSystem::UseSkillTargetInfoMgr( Character* pSrcChr, _STargetInfo aTargetInfo, WORD aSkillCode, BOOL bCreateEffect )
{
//<Before>
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//	DWORD		theTargetID;
//	BYTE		theTargetType;			// n_PC, n_NPC
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//	BYTE		theEffectType1;			// 1(HPAdd) ~ 30(LuckAdd) 까지 
//	short		theEffectValue1;		// Damage(-) 또는 회복정도(+) (HP 관련)
//	BYTE		theAddEffectType;		// 부가효과
//	DWORD		theKeepupTimeType:1;	// n_EffectTime, n_AddEffectTime
//	DWORD		theKeepupTime:31;		// 지속시간
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//	DWORD		thePushedCell;			// 밀기 공격으로 인해 물러 나는 경우 0:안 밀린 경우
//	BYTE		theDieFlag:1;			// (1)n_Success(죽음) : 죽음 모션처리를 위해 수정됨 
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
typedef struct _Effect {
	BYTE	theEffectType;			// 효력수치처리계산방법1	 NCalculate 참조 
	short	theEffectValue;			// 효력수치1
	long	theKeepupTime;			// 지속시간(초)		0 ~ 86400	-1:마법을 재사용시 기존 마법이 풀리는 처리
	BYTE	theSuccRate;			// 성공확률1		// 1~100(%)
}Effect, *EffectPtr;

_Effect	theEffectInfo[3];			// 효력 * 3

struct _STargetInfo
{
	DWORD		theTargetID;	
	
	DWORD		theTargetType:1;		// n_PC, n_NPC
	DWORD		theDieFlag:1;			// (1)n_Success(죽음) : 죽음 모션처리를 위해 수정됨
	DWORD		thePushedCell:30;		// 밀기 공격으로 인해 물러 나는 경우 0:안 밀린 경우( 1 ~ 262,144 )
	BYTE		theKeepupFlag[3];		// n_Success(지속성 적용)
	DWORD		theEffectValue[3];		// 효력처리 값
	WORD		theEffectMaxValue;		// 효력처리 Max값( theMaxAttackPower or theMaxMagicPower )
	BYTE		theAddEfectFlag;		// n_Success, n_Fail
};  
*/	
	SSkillBaseInfo* skill_Info;
	skill_Info = g_Pc_Manager.GetSkillBaseInfoPtr( aSkillCode ); 
	if( skill_Info == NULL )
		return;
 
	int i = 0;
	int j = 0;
	BYTE effect_type = 0;
	DWORD effect_value = 0;
	WORD keepup_flag = 0;
	int keepup_time = 0;
	BOOL isKeepUpSave = FALSE;
	BYTE addeffect_flag = 0;
	BOOL ISFind = FALSE;

	static char temp_msgKS[256] = {0,};
	static char log_msgKS[256] = {0,};

	if( bCreateEffect )
	{
		int Index;
		
		if( aTargetInfo.theTargetType == n_PC )
		{
			if( g_Pc.m_MainPC.lUnique == aTargetInfo.theTargetID )
			{
				if( g_Pc.GetPlayer()->m_KeepUpChecker.FindCode( aSkillCode ) == -1 )
				{
					bCreateEffect = FALSE;
				}
			}
			else
			{
				Index = g_Pc.FindUser( aTargetInfo.theTargetID );				
				
				///--JAPAN_BUG_MODIFY
				if( Index == MAX_USER_INTABLE )
					return;
				
				if(g_Pc.m_Pc[ Index ].m_KeepUpChecker.FindCode( aSkillCode ) == -1 )
				{
					bCreateEffect = FALSE;
				}
			}
		}
		else if( aTargetInfo.theTargetType == n_NPC )
		{
			Index = g_Map.FindNpc( aTargetInfo.theTargetID );
			
			if( Index == MAX_NPC_INTABLE )
				return;

			if(g_Map.m_Npc[ Index ].m_KeepUpChecker.FindCode( aSkillCode ) == -1 )
			{
				bCreateEffect = FALSE;
			}
		}
	}

	BOOL bIsSkillFail = TRUE;	

	bool bIsDrawSystemMsg = false; 

	if( ( pSrcChr && ( pSrcChr->m_bPC == true ) && ( pSrcChr->m_nTableIndex == MAX_USER_INTABLE ) ) ||
		( ( aTargetInfo.theTargetType == n_PC ) && ( g_Pc.m_MainPC.lUnique == aTargetInfo.theTargetID )  )
	  )
	{
		bIsDrawSystemMsg = true;
	}
	
	for( i = 0; i < c_Max_PerSkill_EffectNum; ++i )			// 스킬 효력 처리 3가지 처리  
	{
		ISFind = FALSE;

		if( skill_Info->theEffectInfo[i].theEffectType == n_ZERO )
		{
			continue;
		}

		effect_value = aTargetInfo.theEffectValue[i];		// aTargetInfo.theEffectMaxValue
		keepup_flag = aTargetInfo.theKeepupFlag[i];		
		effect_type = skill_Info->theEffectInfo[i].theEffectType;
		keepup_time = skill_Info->theEffectInfo[i].theKeepupTime;

		int skillcase = effect_type / 30;
		if( skillcase == 1 )
		{
			effect_type -= 30;
		}
		else if( skillcase == 2 )
		{
			effect_type -= 60;
		}		

		// 지속 스킬 성공 여부 처리 
		// 인터페이스 및 이펙트 처리를 위해서...
		if( keepup_time == 0 )				// 지속 스킬이 아닌 것들...제외 
		{		
		}
		else if( keepup_time != 0 && keepup_flag == 0 )		// 지속 스킬 이지만 실패 했을 경우...제외 
		{		
			if( bCreateEffect && bIsDrawSystemMsg && skill_Info )
			{
				SSkillField * pSkillField = NULL;							
				pSkillField = g_Pc_Manager.m_SourceDataManager.FindSkillFieldPtr( (DWORD)effect_type );
							
				if( pSkillField && g_IsSkEffSys )
				{
					Rsprintf( RWCHAR(temp_msgKS), _RT("[%s] "), RWCHAR(skill_Info->theSkillName) );
					Rsprintf( RWCHAR(log_msgKS), G_STRING_CHAR( IDS_SYSPR_KEEP_SKILL_FAIL ), 
							 G_STRING_CHAR( pSkillField->dStringIndex ) );
					SAFE_STR_CAT( temp_msgKS, log_msgKS, 256 );
					SystemPrint( n_DarkRed, n_SystemMsg, temp_msgKS );
				}
			}

			continue;
		}
		else													// 지속 스킬 성공
		{
			if( bCreateEffect == TRUE )
			{
				WORD tableIndex = 0;				
				
				if( aTargetInfo.theTargetType == n_PC )
				{
					if( g_Pc.m_MainPC.lUnique == aTargetInfo.theTargetID )
					{										
						if( bIsDrawSystemMsg && skill_Info )
						{
							SSkillField * pSkillField = NULL;							
							pSkillField = g_Pc_Manager.m_SourceDataManager.FindSkillFieldPtr( (DWORD)effect_type );
											
							if( pSkillField && g_IsSkEffSys )
							{							
								Rsprintf( RWCHAR(temp_msgKS), _RT("[%s] "), RWCHAR(skill_Info->theSkillName ) );
								Rsprintf( RWCHAR(log_msgKS), G_STRING_CHAR( IDS_SYSPR_KEEP_SKILL_ON ), 
										 RWCHAR(g_Pc.m_MainPC.char_info.thePCName), 
										 G_STRING_CHAR( pSkillField->dStringIndex ) );
								SAFE_STR_CAT( temp_msgKS, log_msgKS, 256 );
								SystemPrint( n_DarkGreen, n_SystemMsg, temp_msgKS );	
							}
						}


						if(isKeepUpSave == FALSE)
						{
							for( j = 0; j < RP_MAX_KEEPUP_SKILL; ++j )
							{
								if( nRui->thePcInven.KeepupSkill[j] == aSkillCode )
								{
									ISFind = TRUE;								
									break;
								}
							}
							
							if( FALSE == ISFind )
							{
								for( j = 0; j < RP_MAX_KEEPUP_SKILL; ++j )
								{
									if( nRui->thePcInven.KeepupSkill[j] == 0 )
									{
										nRui->thePcInven.KeepupSkill[j] = aSkillCode;
										
										PlayerInfoWnd.SetKeepUpSkillCode( j , aSkillCode );							
										
										// 지속성 스킬 이펙트 추가 [상준]
										g_Particle.m_EffectManager.CreateKeepupSkill( aSkillCode , g_Pc.GetPlayer() , NULL , TRUE );
										g_Particle.m_EffectManager.CreateAddEffectStart( aSkillCode, g_Pc.GetPlayer() );
										
										break;
									}
								}
							}
						}
					}
					else
					{
						tableIndex = g_Pc.FindUser( aTargetInfo.theTargetID );
						if( tableIndex == MAX_USER_INTABLE )
							return;
					
						if( bIsDrawSystemMsg && skill_Info )
						{
							SSkillField * pSkillField = NULL;							
							pSkillField = g_Pc_Manager.m_SourceDataManager.FindSkillFieldPtr( (DWORD)effect_type );
								
							if( pSkillField && g_IsSkEffSys )
							{							
								Rsprintf( RWCHAR(temp_msgKS), _RT("[%s] "), RWCHAR(skill_Info->theSkillName ) );
								Rsprintf( RWCHAR(log_msgKS), G_STRING_CHAR( IDS_SYSPR_KEEP_SKILL_ON ), 
										 RWCHAR(g_Pc.m_PcTable[tableIndex].char_info.thePCName) , 
										 G_STRING_CHAR( pSkillField->dStringIndex ) );
								SAFE_STR_CAT( temp_msgKS, log_msgKS, 256 );
								SystemPrint( n_DarkGreen, n_SystemMsg, temp_msgKS );	
							}
						}
						
						if(isKeepUpSave == FALSE)
						{
							for( j = 0; j < RP_MAX_KEEPUP_SKILL; ++j )
							{
								if( g_Pc.m_PcTable[tableIndex].char_info.theKeepupSkill[j] == aSkillCode )
								{
									ISFind = TRUE;									
									break;
								}
							}
							
							if( FALSE == ISFind )
							{
								for( j = 0; j < RP_MAX_KEEPUP_SKILL; ++j )
								{
									if( g_Pc.m_PcTable[tableIndex].char_info.theKeepupSkill[j] == 0 )
									{
										g_Pc.m_PcTable[tableIndex].char_info.theKeepupSkill[j] = aSkillCode;
										
										// 지속성 스킬 이펙트 추가 [상준]
										g_Particle.m_EffectManager.CreateKeepupSkill( aSkillCode , &g_Pc.m_Pc[ tableIndex ] , NULL , TRUE );
										g_Particle.m_EffectManager.CreateAddEffectStart( aSkillCode, &g_Pc.m_Pc[ tableIndex ] );

										break;
									}
								}
							}
						}
					}						

				}
				else if( aTargetInfo.theTargetType == n_NPC )
				{
					tableIndex = g_Map.FindNpc( aTargetInfo.theTargetID );
					if( tableIndex == MAX_NPC_INTABLE )
						return;
					
						if( bIsDrawSystemMsg && skill_Info )
						{
							SSkillField * pSkillField = NULL;							
							pSkillField = g_Pc_Manager.m_SourceDataManager.FindSkillFieldPtr( (DWORD)effect_type );
							
							if( pSkillField && g_IsSkEffSys )
							{							
								Rsprintf( RWCHAR(temp_msgKS), _RT("[%s] "), RWCHAR(skill_Info->theSkillName ) );
								Rsprintf( RWCHAR(log_msgKS), G_STRING_CHAR( IDS_SYSPR_KEEP_SKILL_ON ), 
										 RWCHAR(g_Map.m_NpcTable[tableIndex].szName), 
										 G_STRING_CHAR( pSkillField->dStringIndex ) );
								SAFE_STR_CAT( temp_msgKS, log_msgKS, 256 );
								SystemPrint( n_DarkGreen, n_SystemMsg, temp_msgKS );	
							}
						}
					
					if(isKeepUpSave == FALSE)
					{
						
						for( j = 0; j < RP_MAX_KEEPUP_SKILL; ++j )
						{
							if( g_Map.m_NpcTable[tableIndex].theKeepupSkill[j] == aSkillCode )
							{
								ISFind = TRUE;
								
								break;
							}
						}
						
						if( FALSE == ISFind )
						{
							for( j = 0; j < RP_MAX_KEEPUP_SKILL; ++j )
							{
								if( g_Map.m_NpcTable[tableIndex].theKeepupSkill[j] == 0 )
								{
									g_Map.m_NpcTable[tableIndex].theKeepupSkill[j] = aSkillCode;
									
									// 지속성 스킬 이펙트 추가 [상준]
									g_Particle.m_EffectManager.CreateKeepupSkill( aSkillCode , &g_Map.m_Npc[ tableIndex ] , NULL , TRUE );
									g_Particle.m_EffectManager.CreateAddEffectStart( aSkillCode, &g_Map.m_Npc[ tableIndex ] );
									
									break;
								}
							}
						}
					}
				}

				isKeepUpSave = TRUE;
			}
		}

		UseSkillEffectValueMgr( aTargetInfo.theTargetID, aTargetInfo.theTargetType, aSkillCode, 
								effect_type, effect_value, aTargetInfo.theEffectMaxValue, aTargetInfo.theDieFlag );
	}
	
	return;
}


void CSkillSystem::UseSkillEffectValueMgr( DWORD aTargetID, BYTE aTargetType, WORD aSkillCode, Byte aEffectType, long aEffectValue, WORD aEffectMaxValue, BOOL aDieFlag )
{
	SSkillBaseInfo* skill_Info;
	skill_Info = g_Pc_Manager.GetSkillBaseInfoPtr( aSkillCode ); 
	if( skill_Info == NULL )
		return;

	//LONG_MAX

	WORD aTableIndex = 0;
	WORD i = 0;

	if( aTargetType == n_PC )
	{
		if( g_Pc.m_MainPC.lUnique != aTargetID )
		{
			aTableIndex = g_Pc.FindUser( aTargetID );
			if( aTableIndex == MAX_USER_INTABLE )
					return;
		}
	}
	else if( aTargetType == n_NPC )
	{
		aTableIndex = g_Map.FindNpc( aTargetID );		
		if( aTableIndex == MAX_NPC_INTABLE )
			return;
	}
		
	// 실제 스킬에 따른 효력치 적용 
	switch( aEffectType )
	{
	case n_HPAdd:
//	case n_HPMultiply:
//	case n_HPPercent:
		{
#ifdef ROCKCLIENT_DEV
			RLG1( "=========================UseSkillEffectValueMgr n_HPdd = %d", aEffectValue );
#endif
			

//			// 서버에서 수정해주기 전까지만...임시 작업 위에다 정의 함 .. 
			if( aEffectValue >= c_DamageFlag_Subtract )
			{
				aEffectValue -= c_DamageFlag_Subtract;
				aEffectValue *= -1;
			}

			if( aTargetType == n_PC )
			{
				// 주인공에게 적용되는 스킬일 경우...
				if( g_Pc.m_MainPC.lUnique == aTargetID )
				{
					if( aDieFlag )
					{
						nRui->thePcParam.Stat1.theHP = 0;
						g_Pc.GetPlayer()->m_lPreTime = g_nowTime;
					}
					else
					{
						///--JAPAN_BUG_MODIFY
						///-- by simwoosung 현재 죽어있지 않는경우에만 적용시킨다.
						if( !(g_Pc.GetPlayer()->IsRealDie()) )
						{
							nRui->thePcParam.Stat1.theHP -= aEffectValue;
							//PRINT_DLG_LOG("나 스킬로 damage - %d theHP - %d 적용", aEffectValue, 
							//		           g_Pc.m_MainPC.char_info.theRealHP);
							g_Pc.GetPlayer()->m_lPreTime = g_nowTime;													
						}

						if( nRui->thePcParam.Stat1.theHP >= nRui->GetPcParamInfo()->MaxHP )
						{
							//이전꺼 
							nRui->thePcParam.Stat1.theHP = nRui->GetPcParamInfo()->MaxHP;
							g_Pc.m_MainPC.char_info.theRealHP = nRui->GetPcParamInfo()->MaxHP;
							g_Pc.GetPlayer()->m_lPreTime = g_nowTime;
						}

						//dongs캐릭 피버그 수정 .. 
						if( (  (int)g_Pc.m_MainPC.char_info.theRealHP > 0 ) && g_Pc.m_MainPC.char_info.theRealHP >= nRui->thePcParam.Stat1.theHP ) 
						{
							nRui->thePcParam.Stat1.theHP = g_Pc.m_MainPC.char_info.theRealHP;
							g_Pc.GetPlayer()->m_lPreTime = g_nowTime;
						}
					}
					
				}
				// 다른 유저에게 적용되는 스킬일 경우...
				else
				{
					if( aDieFlag )
					{
						g_Pc.m_PcTable[aTableIndex].char_info.theHP = 0;
						g_Pc.m_Pc[aTableIndex].m_lPreTime = g_nowTime;
					}
					else
					{
						///--JAPAN_BUG_MODIFY
						///-- by simwoosung 현재 죽어있지 않는경우에만 적용시킨다.
						if( !(g_Pc.m_Pc[aTableIndex].IsRealDie()) )
						{
							g_Pc.m_PcTable[aTableIndex].char_info.theHP -= aEffectValue;
							g_Pc.m_Pc[aTableIndex].m_lPreTime = g_nowTime;
						}

						if( g_Pc.m_PcTable[aTableIndex].char_info.theHP >= 
							g_Pc.m_PcTable[aTableIndex].char_info.theMaxHP )
						{
							//이전꺼 
							g_Pc.m_PcTable[aTableIndex].char_info.theHP = 
												g_Pc.m_PcTable[aTableIndex].char_info.theMaxHP;
							g_Pc.m_PcTable[aTableIndex].char_info.theRealHP  = 
												g_Pc.m_PcTable[aTableIndex].char_info.theMaxHP;
							g_Pc.m_Pc[aTableIndex].m_lPreTime = g_nowTime;	
						}
						
						// dongs 추가 캐릭터 피버그 .. 
						if( ( (int)g_Pc.m_PcTable[aTableIndex].char_info.theRealHP > 0 ) && ( g_Pc.m_PcTable[aTableIndex].char_info.theRealHP >= g_Pc.m_PcTable[aTableIndex].char_info.theHP) )
						{
							g_Pc.m_PcTable[aTableIndex].char_info.theHP = g_Pc.m_PcTable[aTableIndex].char_info.theRealHP;
							g_Pc.m_Pc[aTableIndex].m_lPreTime = g_nowTime;
						}
					}
				}
			}
			else if( aTargetType == n_NPC )
			{
				if( aDieFlag )
				{
					g_Map.m_NpcTable[aTableIndex].theHP = 0;
				}
				else
				{
					///--JAPAN_BUG_MODIFY
					///-- by simwoosung 현재 죽어있지 않는경우에만 적용시킨다.
					if( !(g_Map.m_Npc[aTableIndex].IsRealDie()) )
					{
						g_Map.m_NpcTable[aTableIndex].theHP -= aEffectValue;
						g_Map.m_Npc[aTableIndex].m_lPreTime = g_nowTime;
					}
					
					if( g_Map.m_NpcTable[aTableIndex].theHP >= 
							g_Map.m_NpcTable[aTableIndex].theMaxHP )
					{
						g_Map.m_NpcTable[aTableIndex].theHP = 
												g_Map.m_NpcTable[aTableIndex].theMaxHP;
						g_Map.m_NpcTable[aTableIndex].theRealHP  = 
												g_Map.m_NpcTable[aTableIndex].theMaxHP;
						g_Map.m_Npc[aTableIndex].m_lPreTime = g_nowTime;
					}
				}
			}
		}
		break;
	case n_SPAdd:
//	case n_SPMultiply:
//	case n_SPPercent:
		{
#ifdef ROCKCLIENT_DEV
		
//	SystemPrint( n_DarkRed, n_SystemMsg, "Skill Value :: n_SPAdd = %d", aEffectValue );

#endif

			if( aTargetType == n_PC )
			{
				// 주t1인공에게 적용되는 스킬일 경우...
				if( g_Pc.m_MainPC.lUnique == aTargetID && g_Pc.m_MainPC.char_info.theCitizen == MILLENA  )
				{
					nRui->thePcParam.Stat1.theSP = aEffectValue;
				}
				// 다른 유저에게 적용되는 스킬일 경우...
				else
				{

					if( g_Pc.m_PcTable[aTableIndex].char_info.theCitizen  ==  MILLENA)
					{
						g_Pc.m_PcTable[aTableIndex].char_info.theSMP = aEffectValue;

					}
				
				}
			}
			else if( aTargetType == n_NPC )
			{
				// 몹 또는 상인 NPC 들에게 적용되는 스킬일 경우...
				//				g_Map.m_NpcTable[aTableIndex].theSP = aEffectValue;
			}
		}
		break;
	case n_MPAdd:
//	case n_MPMultiply:
//	case n_MPPercent:
		{
			if( aTargetType == n_PC )
			{
				
				// 주인공에게 적용되는 스킬일 경우...
				if( g_Pc.m_MainPC.lUnique == aTargetID && g_Pc.m_MainPC.char_info.theCitizen == RAIN )
				{
					if(!aEffectValue)
					{
						int a = 0;
					}
					
					nRui->thePcParam.Stat1.theMP = aEffectValue;	
				}
				// 다른 유저에게 적용되는 스킬일 경우...
				else
				{
					if( g_Pc.m_PcTable[aTableIndex].char_info.theCitizen  ==  RAIN )
					{
						g_Pc.m_PcTable[aTableIndex].char_info.theSMP = aEffectValue;
					}
				}
			}
			else if( aTargetType == n_NPC )
			{
				// 몹 또는 상인 NPC 들에게 적용되는 스킬일 경우...
				//				g_Map.m_NpcTable[aTableIndex].theMP = aEffectValue;
			}
		}
		break;
//	case n_MaxHPAdd:
//	case n_MaxHPMultiply:
//	case n_MaxHPPercent:
	case n_VitalAdd:		
		{
			if( aTargetType == n_PC )
			{
				// 주인공에게 적용되는 스킬일 경우...
				if( g_Pc.m_MainPC.lUnique == aTargetID )
				{
					nRui->thePcParam.Vital = aEffectValue;
//					nRui->thePcParam.MaxHP = aEffectValue * 5;
				}
				// 다른 유저에게 적용되는 스킬일 경우...
				else
				{
//					g_Pc.m_PcTable[aTableIndex].char_info.theMaxHP = aEffectValue * 5;
				}
			}
			else if( aTargetType == n_NPC )
			{
				// 몹 또는 상인 NPC 들에게 적용되는 스킬일 경우...
//				g_Map.m_NpcTable[aTableIndex].theMaxHP = aEffectValue * 5;
			}
		}
		break;
//	case n_MaxSPAdd:
//	case n_MaxSPMultiply:
//	case n_MaxSPPercent:
	case n_StaminaAdd:
		{
			if( aTargetType == n_PC )
			{
				// 주인공에게 적용되는 스킬일 경우...
				if( g_Pc.m_MainPC.lUnique == aTargetID )
				{
					nRui->thePcParam.Stamina = aEffectValue;
//					nRui->thePcParam.MaxSP = aEffectValue * 3;
				}
				// 다른 유저에게 적용되는 스킬일 경우...
				else
				{
//					g_Pc.m_PcTable[aTableIndex].char_info.theMaxSMP = aEffectValue * 3;
				}
			}
			else if( aTargetType == n_NPC )
			{
				// 몹 또는 상인 NPC 들에게 적용되는 스킬일 경우...
				//	g_Map.m_NpcTable[aTableIndex].theSMP = aEffectValue * 3;
			}
		}
		break;
//	case n_MaxMPAdd:
//	case n_MaxMPMultiply:
//	case n_MaxMPPercent:	
	case n_IntAdd:	
		{
			if( aTargetType == n_PC )
			{
				// 주인공에게 적용되는 스킬일 경우...
				if( g_Pc.m_MainPC.lUnique == aTargetID )
				{
					nRui->thePcParam.Intel = aEffectValue;
//					nRui->thePcParam.MaxMP = aEffectValue * 3;
				}
				// 다른 유저에게 적용되는 스킬일 경우...
				else
				{
//					g_Pc.m_PcTable[aTableIndex].char_info.theMaxSMP = aEffectValue * 3;
				}
			}
			else if( aTargetType == n_NPC )
			{
				// 몹 또는 상인 NPC 들에게 적용되는 스킬일 경우...
				// g_Map.m_NpcTable[aTableIndex].theSMP = aEffectValue * 3;
			}
		}
		break;
	case n_DexAdd:	
		{
			if( aTargetType == n_PC )
			{				
				if( g_Pc.m_MainPC.lUnique == aTargetID )
				{
					// 다른 유저에게 적용되는 스킬일 경우...// 주인공에게 적용되는 스킬일 경우...
					nRui->thePcParam.Dex = aEffectValue;
				}				
				else
				{	// 다른 유저에게 적용되는 스킬일 경우...
				}
			}			
			else if( aTargetType == n_NPC )
			{	// 몹 또는 상인 NPC 들에게 적용되는 스킬일 경우...
			}
		}
		break;	
	case n_HPSpeedAdd:					// HP 회복속도
//	case n_HPSpeedMultiply:
//	case n_HPSpeedPercent:
	case n_SPSpeedAdd:					// SP 회복속도
//	case n_SPSpeedMultiply:					
//	case n_SPSpeedPercent:					
	case n_MPSpeedAdd:					// MP 회복속도 
//	case n_MPSpeedMultiply:				
//	case n_MPSpeedPercent:				
		break;
	case n_AttackSpeedAdd:				// 공격속도(물리)
//	case n_AttackSpeedMultiply:				
//	case n_AttackSpeedPercent:				
		{
			if( aTargetType == n_PC )
			{
				// 주인공에게 적용되는 스킬일 경우...
				if( g_Pc.m_MainPC.lUnique == aTargetID )
				{
					///-- By simwoosung
					///-- 홀리 어벤져의 치유의 빛 사용시... 이곳으로 현재 HP값이 들어온다.
					///-- 서버에서 아직 원인을 모르고있는듯하지만.. 상당히 크리티컬한 버그이다.
					///-- 해서 임시적으로 막아주는 코드를 넣어준다.
					///-- 들어온값이 HP와 동일하면... 리턴 시킨다.					
					if( aEffectValue == g_Pc.m_MainPC.char_info.theRealHP )
					{
						///--PRINT_DLG_LOG( "aEffectValue = %d g_Pc.GetPlayer()->m_Weapon_DelayTime = %d ", 
						///--				aEffectValue , g_Pc.GetPlayer()->m_Weapon_DelayTime );
						return;
					}
					
					nRui->thePcParam.Stat2.theAttackSpeed = aEffectValue;
					g_Pc.GetPlayer()->m_Weapon_DelayTime = aEffectValue * 100;		// 100ms단위로 보내주므로 컨버팅 필요 
								
				}
				// 다른 유저에게 적용되는 스킬일 경우...
				else
				{
					//					g_Pc.m_PcTable[aTableIndex].char_info.theMaxSMP = aEffectValue;
				}
			}
			else if( aTargetType == n_NPC )
			{
				// 몹 또는 상인 NPC 들에게 적용되는 스킬일 경우...
				//				g_Map.m_NpcTable[aTableIndex].theSMP = aEffectValue;
			}
		}
		break;
	case n_MoveSpeedAdd:				// 이동속도
//	case n_MoveSpeedMultiply:		
//	case n_MoveSpeedPercent:		
		{
#ifdef ROCKCLIENT_DEV
//			SystemPrint( n_DarkRed, n_SystemMsg, _RT("Skill Value :: n_MoveSpeed = %d"), aEffectValue );
#endif

			if( aTargetType == n_PC )
			{
				// 주인공에게 적용되는 스킬일 경우...
				if( g_Pc.m_MainPC.lUnique == aTargetID )
				{
					nRui->thePcParam.Stat2.theMoveSpeed = aEffectValue;					
					g_Pc.GetPlayer()->m_ani_percent = ( (float)aEffectValue / c_Pc_MoveSpeed );					
					g_Pc.GetPlayer()->m_move_percent = c_Pc_MoveSpeed / ( c_Pc_MoveSpeed * g_Pc.GetPlayer()->m_ani_percent );					
				}
				// 다른 유저에게 적용되는 스킬일 경우...
				else
				{
					g_Pc.m_Pc[aTableIndex].m_ani_percent = ( (float)aEffectValue / c_Pc_MoveSpeed );					
					g_Pc.m_Pc[aTableIndex].m_move_percent = c_Pc_MoveSpeed / ( c_Pc_MoveSpeed * g_Pc.m_Pc[aTableIndex].m_ani_percent );					
				}
			}
			else if( aTargetType == n_NPC )
			{
				// 몹 또는 상인 NPC 들에게 적용되는 스킬일 경우...
				SNPCBaseInfo*	npc_Info = NULL;
				npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( g_Map.m_NpcTable[aTableIndex].code ); 
				g_Map.m_Npc[aTableIndex].m_ani_percent = ( (float)aEffectValue / npc_Info->theMoveSpeed );					
				g_Map.m_Npc[aTableIndex].m_move_percent = npc_Info->theMoveSpeed / ( npc_Info->theMoveSpeed * g_Map.m_Npc[aTableIndex].m_ani_percent );
				RLG3("aEffectValue = %d, m_ani_percent = %3.3f, m_move_percent = %3.3f", aEffectValue, g_Map.m_Npc[aTableIndex].m_ani_percent, g_Map.m_Npc[aTableIndex].m_move_percent );
			}
		}
		break;
	case n_AttackPowerAdd:				// 물리공격(최소, 최대치 모두 증가)
//	case n_AttackPowerMultiply:				
//	case n_AttackPowerPercent:				
		{

/*
enum NSkillType2
{
	n_DeathBlow		 = 1,		// 필살기계
	n_AttackMagic	 = 2,		// 공격마법계
	n_Reserved		 = 3,		// 예약
	n_GoodSupport	 = 4,		// 보조계 - Good
	n_Regain		 = 5,		// 회복계
	n_ItemSkill		 = 6,		// 아이템계
	n_Produce		 = 7,		// 생산계
	n_BadSupport	 = 8,		// 보조계 - Good
};
*/
/*			if( skill_Info->theSkillType2 == n_DeathBlow || skill_Info->theSkillType2 == n_AttackMagic )
			{
				if( aTargetType == n_PC )
				{
					// 주인공에게 적용되는 스킬일 경우...
					if( g_Pc.m_MainPC.lUnique == aTargetID )
					{
						nRui->thePcParam.Stat1.theHP = aEffectValue;
						g_Pc.m_MainPC.char_info.theRealHP = aEffectValue; 
					}
					// 다른 유저에게 적용되는 스킬일 경우...
					else
					{
						g_Pc.m_PcTable[aTableIndex].char_info.theHP = aEffectValue;
						g_Pc.m_PcTable[aTableIndex].char_info.theRealHP = aEffectValue; 
					}
				}
				else if( aTargetType == n_NPC )
				{
					// 몹 또는 상인 NPC 들에게 적용되는 스킬일 경우...
					g_Map.m_NpcTable[aTableIndex].theHP = aEffectValue;
					g_Map.m_NpcTable[aTableIndex].theRealHP = aEffectValue; 
				}
			}
			else */
			{
				if( aTargetType == n_PC )
				{
					// 주인공에게 적용되는 스킬일 경우...
					if( g_Pc.m_MainPC.lUnique == aTargetID )
					{
						nRui->thePcParam.Stat2.theMinAttackPower = aEffectValue;
						nRui->thePcParam.Stat2.theMaxAttackPower = aEffectMaxValue;
					}
					// 다른 유저에게 적용되는 스킬일 경우...
					else
					{
						//					g_Pc.m_PcTable[aTableIndex].char_info.theMaxSMP = aEffectValue;
					}
				}
				else if( aTargetType == n_NPC )
				{
					// 몹 또는 상인 NPC 들에게 적용되는 스킬일 경우...
					//				g_Map.m_NpcTable[aTableIndex].theSMP = aEffectValue;
				}
			}
		}
		break;
	case n_AttackDefenseAdd:			// 물리방어
//	case n_AttackDefenseMultiply:		
//	case n_AttackDefensePercent:		
		{
			if( aTargetType == n_PC )
			{
				// 주인공에게 적용되는 스킬일 경우...
				if( g_Pc.m_MainPC.lUnique == aTargetID )
				{
					nRui->thePcParam.Stat2.theAttackDef = aEffectValue;
				}
				// 다른 유저에게 적용되는 스킬일 경우...
				else
				{
					//					g_Pc.m_PcTable[aTableIndex].char_info.theMaxSMP = aEffectValue;
				}
			}
			else if( aTargetType == n_NPC )
			{
				// 몹 또는 상인 NPC 들에게 적용되는 스킬일 경우...
				//				g_Map.m_NpcTable[aTableIndex].theSMP = aEffectValue;
			}
		}
		break;
//	case n_AttackAttriAdd:				// 방어속성(방어속서이 0인 평범한 상의 아이템에만 적용되어 속성값을 부여한다)
//	case n_AttackAttriMultiply:	
//	case n_AttackAttriPercent:	
//		break;
	case n_MagicResAdd:
		{
			if( aTargetType == n_PC )
			{
				// 주인공에게 적용되는 스킬일 경우...
				if( g_Pc.m_MainPC.lUnique == aTargetID )
				{
					nRui->thePcParam.Stat2.theMagicRes = aEffectValue;					
				}
				// 다른 유저에게 적용되는 스킬일 경우...
				else
				{
				}
			}
			else if( aTargetType == n_NPC )
			{
			}
		}
		break;
	case n_MagicPowerAdd:				// 마법공격
//	case n_MagicPowerMultiply:		
//	case n_MagicPowerPercent:		
		{
/*			if( skill_Info->theSkillType2 == n_DeathBlow || skill_Info->theSkillType2 == n_AttackMagic )
			{
				if( aTargetType == n_PC )
				{
					// 주인공에게 적용되는 스킬일 경우...
					if( g_Pc.m_MainPC.lUnique == aTargetID )
					{
						nRui->thePcParam.Stat1.theHP = aEffectValue;
						g_Pc.m_MainPC.char_info.theRealHP = aEffectValue; 
					}
					// 다른 유저에게 적용되는 스킬일 경우...
					else
					{
						g_Pc.m_PcTable[aTableIndex].char_info.theHP = aEffectValue;
						g_Pc.m_PcTable[aTableIndex].char_info.theRealHP = aEffectValue; 
					}
				}
				else if( aTargetType == n_NPC )
				{
					// 몹 또는 상인 NPC 들에게 적용되는 스킬일 경우...
					g_Map.m_NpcTable[aTableIndex].theHP = aEffectValue;
					g_Map.m_NpcTable[aTableIndex].theRealHP = aEffectValue; 
				}
			}
			else */
			{
				if( aTargetType == n_PC )
				{
					// 주인공에게 적용되는 스킬일 경우...
					if( g_Pc.m_MainPC.lUnique == aTargetID )
					{
						nRui->thePcParam.Stat2.theMinMagicPower = aEffectValue;
						nRui->thePcParam.Stat2.theMaxMagicPower = aEffectMaxValue;
					}
					// 다른 유저에게 적용되는 스킬일 경우...
					else
					{
						//					g_Pc.m_PcTable[aTableIndex].char_info.theMaxSMP = aEffectValue;
					}
				}
				else if( aTargetType == n_NPC )
				{
					// 몹 또는 상인 NPC 들에게 적용되는 스킬일 경우...
					//				g_Map.m_NpcTable[aTableIndex].theSMP = aEffectValue;
				}
			}
		}
		break;
/*	case n_PowerAdd:					// 파워
		{
			if( aTargetType == n_PC )
			{
				// 주인공에게 적용되는 스킬일 경우...
				if( g_Pc.m_MainPC.lUnique == aTargetID )
				{
					nRui->thePcParam.Power = aEffectValue;
				}
				// 다른 유저에게 적용되는 스킬일 경우...
				else
				{
					//					g_Pc.m_PcTable[aTableIndex].char_info.theMaxSMP = aEffectValue;
				}
			}
			else if( aTargetType == n_NPC )
			{
				// 몹 또는 상인 NPC 들에게 적용되는 스킬일 경우...
				//				g_Map.m_NpcTable[aTableIndex].theSMP = aEffectValue;
			}
		}
		break;
	case n_DefenseAdd:					// 방어
		{
			if( aTargetType == n_PC )
			{
				// 주인공에게 적용되는 스킬일 경우...
				if( g_Pc.m_MainPC.lUnique == aTargetID )
				{
					nRui->thePcParam.Defense = aEffectValue;
				}
				// 다른 유저에게 적용되는 스킬일 경우...
				else
				{
					//					g_Pc.m_PcTable[aTableIndex].char_info.theMaxSMP = aEffectValue;
				}
			}
			else if( aTargetType == n_NPC )
			{
				// 몹 또는 상인 NPC 들에게 적용되는 스킬일 경우...
				//				g_Map.m_NpcTable[aTableIndex].theSMP = aEffectValue;
			}
		}
		break;
	case n_SympathyAdd:					// 교감
		{
			if( aTargetType == n_PC )
			{
				// 주인공에게 적용되는 스킬일 경우...
				if( g_Pc.m_MainPC.lUnique == aTargetID )
				{
					nRui->thePcParam.Sympathy = aEffectValue;
				}
				// 다른 유저에게 적용되는 스킬일 경우...
				else
				{
					//					g_Pc.m_PcTable[aTableIndex].char_info.theMaxSMP = aEffectValue;
				}
			}
			else if( aTargetType == n_NPC )
			{
				// 몹 또는 상인 NPC 들에게 적용되는 스킬일 경우...
				//				g_Map.m_NpcTable[aTableIndex].theSMP = aEffectValue;
			}
		}
		break;
	case n_ResistAdd:					// 저항
		{
			if( aTargetType == n_PC )
			{
				// 주인공에게 적용되는 스킬일 경우...
				if( g_Pc.m_MainPC.lUnique == aTargetID )
				{
					nRui->thePcParam.Resist = aEffectValue;
				}
				// 다른 유저에게 적용되는 스킬일 경우...
				else
				{
					//					g_Pc.m_PcTable[aTableIndex].char_info.theMaxSMP = aEffectValue;
				}
			}
			else if( aTargetType == n_NPC )
			{
				// 몹 또는 상인 NPC 들에게 적용되는 스킬일 경우...
				//				g_Map.m_NpcTable[aTableIndex].theSMP = aEffectValue;
			}
		}
		break;
	case n_FocusAdd:					// 집중
		{
			if( aTargetType == n_PC )
			{
				// 주인공에게 적용되는 스킬일 경우...
				if( g_Pc.m_MainPC.lUnique == aTargetID )
				{
					nRui->thePcParam.Focus = aEffectValue;
				}
				// 다른 유저에게 적용되는 스킬일 경우...
				else
				{
					//					g_Pc.m_PcTable[aTableIndex].char_info.theMaxSMP = aEffectValue;
				}
			}
			else if( aTargetType == n_NPC )
			{
				// 몹 또는 상인 NPC 들에게 적용되는 스킬일 경우...
				//				g_Map.m_NpcTable[aTableIndex].theSMP = aEffectValue;
			}
		}
		break;
	case n_ReactionAdd:					// 반응
		{
			if( aTargetType == n_PC )
			{
				// 주인공에게 적용되는 스킬일 경우...
				if( g_Pc.m_MainPC.lUnique == aTargetID )
				{
					nRui->thePcParam.Reaction = aEffectValue;
				}
				// 다른 유저에게 적용되는 스킬일 경우...
				else
				{
					//					g_Pc.m_PcTable[aTableIndex].char_info.theMaxSMP = aEffectValue;
				}
			}
			else if( aTargetType == n_NPC )
			{
				// 몹 또는 상인 NPC 들에게 적용되는 스킬일 경우...
				//				g_Map.m_NpcTable[aTableIndex].theSMP = aEffectValue;
			}
		}
		break;
	case n_CharismaAdd:					// 카리스마, 매력
		{
			if( aTargetType == n_PC )
			{
				// 주인공에게 적용되는 스킬일 경우...
				if( g_Pc.m_MainPC.lUnique == aTargetID )
				{
					nRui->thePcParam.Charisma = aEffectValue;
				}
				// 다른 유저에게 적용되는 스킬일 경우...
				else
				{
					//					g_Pc.m_PcTable[aTableIndex].char_info.theMaxSMP = aEffectValue;
				}
			}
			else if( aTargetType == n_NPC )
			{
				// 몹 또는 상인 NPC 들에게 적용되는 스킬일 경우...
				//				g_Map.m_NpcTable[aTableIndex].theSMP = aEffectValue;
			}
		}
		break;
*/
	case n_AttackBlowAdd:			// 물리 필살기 
//	case n_AttackBlowMultiply:
//	case n_AttackBlowPercent:
	case n_MagicBlowAdd:			// 마법 필살기 
//	case n_MagicBlowMultiply:
//	case n_MagicBlowPercent:
		{
#ifdef ROCKCLIENT_DEV
			RLG1( "=========================UseSkillEffectValueMgr n_AttackBlow = %d", aEffectValue );
#endif
			// 서버에서 수정해주기 전까지만...임시 작업 
			if( aEffectValue >= c_DamageFlag_Subtract )
			{
				aEffectValue -= c_DamageFlag_Subtract;
				aEffectValue *= -1;
			}

			if( aTargetType == n_PC )
			{
				// 주인공에게 적용되는 스킬일 경우...
				if( g_Pc.m_MainPC.lUnique == aTargetID )
				{
					if( aDieFlag )
					{
						nRui->thePcParam.Stat1.theHP = 0;
						g_Pc.m_MainPC.char_info.theHP = 0;
						g_Pc.m_MainPC.char_info.theRealHP = 0;
						g_Pc.GetPlayer()->m_lPreTime = g_nowTime;
					}
					else
					{						
						///--JAPAN_BUG_MODIFY
						///-- by simwoosung 현재 죽어있지 않는경우에만 적용시킨다.
						if( !(g_Pc.GetPlayer()->IsRealDie()) )
						{
							nRui->thePcParam.Stat1.theHP -= aEffectValue;
							//PRINT_DLG_LOG("나 스킬로 damage - %d theHP - %d 적용 스킬코드 - %d", aEffectValue, 
							//		           nRui->thePcParam.Stat1.theHP, aSkillCode);
							g_Pc.GetPlayer()->m_lPreTime = g_nowTime;
							///SAFE_DELETE_ARRAY( target_list );
						}					
					}
				}
				// 다른 유저에게 적용되는 스킬일 경우...
				else
				{
					if( aDieFlag )
					{
						g_Pc.m_PcTable[aTableIndex].char_info.theHP = 0;
						g_Pc.m_PcTable[aTableIndex].char_info.theRealHP = 0;
						g_Pc.m_Pc[aTableIndex].m_lPreTime = g_nowTime;
					}
					else
					{
						///--JAPAN_BUG_MODIFY
						///-- by simwoosung 현재 죽어있지 않는경우에만 적용시킨다.
						if( !(g_Pc.m_Pc[aTableIndex].IsRealDie()) )
						{
							g_Pc.m_PcTable[aTableIndex].char_info.theHP -= aEffectValue;						
							g_Pc.m_Pc[aTableIndex].m_lPreTime = g_nowTime;
						}
					}
				}
			}
			else if( aTargetType == n_NPC )
			{
				if( aDieFlag )
				{
					g_Map.m_NpcTable[aTableIndex].theHP = 0;
					g_Map.m_NpcTable[aTableIndex].theRealHP = 0;
					g_Map.m_Npc[aTableIndex].m_lPreTime = g_nowTime;
				}
				else
				{
					
					if( NonPlayerInfoWnd.thePickedNpcID != -1 && 
						NonPlayerInfoWnd.thePickedNpcID == aTableIndex )
					{
						//--PRINT_DLG_LOG("대상HP 적용루틴으로 들어옴");
					}			
					
					///--JAPAN_BUG_MODIFY
					///-- by simwoosung 현재 죽어있지 않는경우에만 적용시킨다.
					if( !(g_Map.m_Npc[aTableIndex].IsRealDie()) )
					{
						g_Map.m_NpcTable[aTableIndex].theHP -= aEffectValue;
						g_Map.m_Npc[aTableIndex].m_lPreTime = g_nowTime;

						if( NonPlayerInfoWnd.thePickedNpcID != -1 && 
							NonPlayerInfoWnd.thePickedNpcID == aTableIndex )
						{
							///--PRINT_DLG_LOG("대상HP 적용 Damage = %d  theRealHP = %d theHP = %d",
							///--			   aEffectValue, g_Map.m_NpcTable[aTableIndex].theRealHP, 
							///--			   g_Map.m_NpcTable[aTableIndex].theHP );
						}	
					}
				}
			}
		}
		break;
	case n_LuckAdd:						// 행운
//	case n_LuckMultiply:		
//	case n_LuckPercent:		
		{
			if( aTargetType == n_PC )
			{
				// 주인공에게 적용되는 스킬일 경우...
				if( g_Pc.m_MainPC.lUnique == aTargetID )
				{
					nRui->thePcParam.Luck = (char)aEffectValue;
				}
				// 다른 유저에게 적용되는 스킬일 경우...
				else
				{
					// g_Pc.m_PcTable[aTableIndex].char_info.theMaxSMP = aEffectValue;
				}
			}
			else if( aTargetType == n_NPC )
			{
				// 몹 또는 상인 NPC 들에게 적용되는 스킬일 경우...
				//	g_Map.m_NpcTable[aTableIndex].theSMP = aEffectValue;
			}
		}
		break;
	case n_CommonPowerAdd:
		{
			if( aTargetType == n_PC )
			{
				// 주인공에게 적용되는 스킬일 경우...
				if( g_Pc.m_MainPC.lUnique == aTargetID )
				{
					if( g_Pc.m_MainPC.char_info.theCitizen == MILLENA)
					{
						nRui->thePcParam.Stat2.theMinAttackPower = aEffectValue;
						nRui->thePcParam.Stat2.theMaxAttackPower = aEffectMaxValue;
					}
					else
					{
						nRui->thePcParam.Stat2.theMinMagicPower = aEffectValue;
						nRui->thePcParam.Stat2.theMaxMagicPower = aEffectMaxValue;
					}
				}
				// 다른 유저에게 적용되는 스킬일 경우...
				else
				{
					//					g_Pc.m_PcTable[aTableIndex].char_info.theMaxSMP = aEffectValue;
				}
			}
			else if( aTargetType == n_NPC )
			{
				// 몹 또는 상인 NPC 들에게 적용되는 스킬일 경우...
				//				g_Map.m_NpcTable[aTableIndex].theSMP = aEffectValue;
			}
		}
		break;
	case n_MaxHPAdd:
		{
#ifdef ROCKCLIENT_DEV
//			SystemPrint( n_DarkRed, n_SystemMsg, "Skill Value :: n_MaxHPAdd = %d", aEffectValue );
#endif
			if( aTargetType == n_PC )
			{
				// 주t1인공에게 적용되는 스킬일 경우...
				if( g_Pc.m_MainPC.lUnique == aTargetID )
				{
					nRui->thePcParam.MaxHP = aEffectValue;
				}
				// 다른 유저에게 적용되는 스킬일 경우...
				else
				{
					g_Pc.m_PcTable[aTableIndex].char_info.theMaxHP = aEffectValue;
				}
			}
			else if( aTargetType == n_NPC )
			{
				// 몹 또는 상인 NPC 들에게 적용되는 스킬일 경우...
			//	g_Map.m_NpcTable[aTableIndex].theMaxHP = aEffectValue;
			}
		}
		break;
	case n_MaxSMPAdd:
		{

#ifdef ROCKCLIENT_DEV
//			SystemPrint( n_DarkRed, n_SystemMsg, "Skill Value :: n_MaxSMPAdd = %d", aEffectValue );
#endif
			if( aTargetType == n_PC )
			{
				// 주t1인공에게 적용되는 스킬일 경우...
				if( g_Pc.m_MainPC.lUnique == aTargetID )
				{
					if( g_Pc.m_MainPC.char_info.theCitizen == MILLENA)
					{
						nRui->thePcParam.MaxSP = aEffectValue;
					}
					else
					{
						nRui->thePcParam.MaxMP = aEffectValue;
					}
				}
				// 다른 유저에게 적용되는 스킬일 경우...
				else
				{
					g_Pc.m_PcTable[aTableIndex].char_info.theMaxSMP = aEffectValue;
				}
			}
			else if( aTargetType == n_NPC )
			{
				// 몹 또는 상인 NPC 들에게 적용되는 스킬일 경우...
				//g_Map.m_NpcTable[aTableIndex].theSMP = aEffectValue;
			}
		}
		break;			
	default:
		break;
	}
	
	return;
}

void CSkillSystem::InitCharDamage( Character * pChar )
{
	pChar->m_curt_event.damage = 0;
	pChar->m_curt_event.number = 0;
	SAFE_DELETE_ARRAY(pChar->m_curt_event.target_list);
}

void CSkillSystem::UseItemEffectValueMgr( DWORD aTargetID, BYTE aTargetType, WORD aItemCode, Byte aEffectType, long aEffectValue, WORD aEffectMaxValue, BOOL aDieFlag )
{
	SItemBaseInfo* item_Info = NULL;
	item_Info = g_Pc_Manager.GetItemBaseInfoPtr( aItemCode );	
	if( item_Info == NULL )
		return;

	WORD aTableIndex = 0;
	WORD i = 0;

	if( aTargetType == n_PC )
	{
		if( g_Pc.m_MainPC.lUnique != aTargetID )
		{
			aTableIndex = g_Pc.FindUser( aTargetID );
			if( aTableIndex == MAX_USER_INTABLE )
					return;
		}
	}
	else if( aTargetType == n_NPC )
	{
		aTableIndex = g_Map.FindNpc( aTargetID );		
		if( aTableIndex == MAX_NPC_INTABLE )
			return;
	}
/*
	n_Scroll_Exp			= 51,
	n_Scroll_MaxHP			= 52,
	n_Scroll_MaxSP			= 53,
	n_Scroll_MaxMP			= 54,
	n_Scroll_AttackPow		= 55,
	n_Scroll_MagicPow		= 56,
	n_Scroll_AttackDef		= 57,
	n_Scroll_MagicRes		= 58,
	n_Scroll_AttackSpeed	= 59, 
	n_Scroll_MoveSpeed		= 60,
	n_Scroll_HitRate		= 61,
	n_Scroll_AvoidRate		= 62,
	n_Scroll_DropRate		= 63,
*/
	
	// 실제 스킬에 따른 효력치 적용 
	switch( aEffectType )
	{
	case n_Scroll_Exp:
		{

		}
		break;

	case n_Scroll_MaxHP:
		{
#ifdef ROCKCLIENT_DEV
			RLG1( "=========================UseItemEffectValueMgr n_Scroll_MaxHP = %d", aEffectValue );
#endif
			if( aTargetType == n_PC )
			{
				// 주인공에게 적용되는 스킬일 경우...
				if( g_Pc.m_MainPC.lUnique == aTargetID )
				{
					nRui->thePcParam.MaxHP = aEffectValue; 

//					if( nRui->thePcParam.Stat1.theHP > nRui->thePcParam.MaxHP )
//					{
//						nRui->thePcParam.Stat1.theHP = nRui->thePcParam.MaxHP;
//					}					
				}
				// 다른 유저에게 적용되는 스킬일 경우...
				else
				{
					g_Pc.m_PcTable[aTableIndex].char_info.theMaxHP = aEffectValue;

//					if( g_Pc.m_PcTable[aTableIndex].char_info.theHP > g_Pc.m_PcTable[aTableIndex].char_info.theMaxHP )
//					{
//						g_Pc.m_PcTable[aTableIndex].char_info.theHP = g_Pc.m_PcTable[aTableIndex].char_info.theMaxHP;
//					}					
				}
			}
			else if( aTargetType == n_NPC )
			{
				// 몹 또는 상인 NPC 들에게 적용되는 스킬일 경우...
				g_Map.m_NpcTable[aTableIndex].theMaxHP = aEffectValue;

//				if( g_Map.m_NpcTable[aTableIndex].theHP > g_Map.m_NpcTable[aTableIndex].theMaxHP )
//				{
//					g_Map.m_NpcTable[aTableIndex].theHP = g_Map.m_NpcTable[aTableIndex].theMaxHP;
//				}	
			}
		}
		break;

	case n_Scroll_MaxSP:
		{
#ifdef ROCKCLIENT_DEV
			RLG1( "=========================UseItemEffectValueMgr n_Scroll_MaxSP = %d", aEffectValue );
#endif
			if( aTargetType == n_PC )
			{
				// 주인공에게 적용되는 스킬일 경우...
				if( g_Pc.m_MainPC.lUnique == aTargetID )
				{
					nRui->thePcParam.MaxSP = aEffectValue; 

//					if( nRui->thePcParam.Stat1.theSP > nRui->thePcParam.MaxSP )
//					{
//						nRui->thePcParam.Stat1.theSP = nRui->thePcParam.MaxSP;
//					}	
				}
				// 다른 유저에게 적용되는 스킬일 경우...
				else
				{
					g_Pc.m_PcTable[aTableIndex].char_info.theMaxSMP = aEffectValue;

//					if( g_Pc.m_PcTable[aTableIndex].char_info.theSMP > g_Pc.m_PcTable[aTableIndex].char_info.theMaxSMP )
///					{
//						g_Pc.m_PcTable[aTableIndex].char_info.theSMP = g_Pc.m_PcTable[aTableIndex].char_info.theMaxSMP;
//					}	
				}
			}
			else if( aTargetType == n_NPC )
			{
				// 몹 또는 상인 NPC 들에게 적용되는 스킬일 경우...
				g_Map.m_NpcTable[aTableIndex].theSMP = aEffectValue;
			}
		}
		break;

	case n_Scroll_MaxMP:
		{
#ifdef ROCKCLIENT_DEV
			RLG1( "=========================UseItemEffectValueMgr n_Scroll_MaxMP = %d", aEffectValue );
#endif
			if( aTargetType == n_PC )
			{
				// 주인공에게 적용되는 스킬일 경우...
				if( g_Pc.m_MainPC.lUnique == aTargetID )
				{
					nRui->thePcParam.MaxMP = aEffectValue; 

//					if( nRui->thePcParam.Stat1.theMP > nRui->thePcParam.MaxMP )
//					{
//						nRui->thePcParam.Stat1.theMP = nRui->thePcParam.MaxMP;
//					}	
				}
				// 다른 유저에게 적용되는 스킬일 경우...
				else
				{
					g_Pc.m_PcTable[aTableIndex].char_info.theMaxSMP = aEffectValue;

//					if( g_Pc.m_PcTable[aTableIndex].char_info.theSMP > g_Pc.m_PcTable[aTableIndex].char_info.theMaxSMP )
//					{
//						g_Pc.m_PcTable[aTableIndex].char_info.theSMP = g_Pc.m_PcTable[aTableIndex].char_info.theMaxSMP;
//					}	
				}
			}
			else if( aTargetType == n_NPC )
			{
				// 몹 또는 상인 NPC 들에게 적용되는 스킬일 경우...
				g_Map.m_NpcTable[aTableIndex].theSMP = aEffectValue;
			}
		}
		break;

	case n_Scroll_AttackPow:				// 물리공격(최소, 최대치 모두 증가)
		{
			if( aTargetType == n_PC )
			{
				// 주인공에게 적용되는 스킬일 경우...
				if( g_Pc.m_MainPC.lUnique == aTargetID )
				{
					nRui->thePcParam.Stat2.theMinAttackPower = aEffectValue;
					nRui->thePcParam.Stat2.theMaxAttackPower = aEffectMaxValue;
				}
			}
		}
		break;

	case n_Scroll_MagicPow:				// 마법공격
		{
			if( aTargetType == n_PC )
			{
				// 주인공에게 적용되는 스킬일 경우...
				if( g_Pc.m_MainPC.lUnique == aTargetID )
				{
					nRui->thePcParam.Stat2.theMinMagicPower = aEffectValue;
					nRui->thePcParam.Stat2.theMaxMagicPower = aEffectMaxValue;
				}
			}
		}
		break;
	
	case n_Scroll_AttackDef:			// 물리방어
		{
			if( aTargetType == n_PC )
			{
				// 주인공에게 적용되는 스킬일 경우...
				if( g_Pc.m_MainPC.lUnique == aTargetID )
				{
					nRui->thePcParam.Stat2.theAttackDef = aEffectValue;
				}
			}
		}
		break;
	
	case n_Scroll_MagicRes:
		{
			if( aTargetType == n_PC )
			{
				// 주인공에게 적용되는 스킬일 경우...
				if( g_Pc.m_MainPC.lUnique == aTargetID )
				{
					nRui->thePcParam.Stat2.theMagicRes = aEffectValue;					
				}
			}
		}
		break;

	case n_Scroll_AttackSpeed:				// 공격속도(물리)
		{
			if( aTargetType == n_PC )
			{
				// 주인공에게 적용되는 스킬일 경우...
				if( g_Pc.m_MainPC.lUnique == aTargetID )
				{
					nRui->thePcParam.Stat2.theAttackSpeed = aEffectValue;
					g_Pc.GetPlayer()->m_Weapon_DelayTime = aEffectValue * 100;		// 100ms단위로 보내주므로 컨버팅 필요 					
				}
			}
		}
		break;

	case n_Scroll_MoveSpeed:				// 이동속도
		{
			if( aTargetType == n_PC )
			{
				// 주인공에게 적용되는 스킬일 경우...
				if( g_Pc.m_MainPC.lUnique == aTargetID )
				{
					nRui->thePcParam.Stat2.theMoveSpeed = aEffectValue;					
					g_Pc.GetPlayer()->m_ani_percent = ( (float)aEffectValue / c_Pc_MoveSpeed );					
					g_Pc.GetPlayer()->m_move_percent = c_Pc_MoveSpeed / ( c_Pc_MoveSpeed * g_Pc.GetPlayer()->m_ani_percent );					
				}
				// 다른 유저에게 적용되는 스킬일 경우...
				else
				{
					g_Pc.m_Pc[aTableIndex].m_ani_percent = ( (float)aEffectValue / c_Pc_MoveSpeed );					
					g_Pc.m_Pc[aTableIndex].m_move_percent = c_Pc_MoveSpeed / ( c_Pc_MoveSpeed * g_Pc.m_Pc[aTableIndex].m_ani_percent );					
				}
			}
			else if( aTargetType == n_NPC )
			{
				// 몹 또는 상인 NPC 들에게 적용되는 스킬일 경우...
				SNPCBaseInfo*	npc_Info = NULL;
				npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( g_Map.m_NpcTable[aTableIndex].code ); 
				g_Map.m_Npc[aTableIndex].m_ani_percent = ( (float)aEffectValue / npc_Info->theMoveSpeed );					
				g_Map.m_Npc[aTableIndex].m_move_percent = npc_Info->theMoveSpeed / ( npc_Info->theMoveSpeed * g_Map.m_Npc[aTableIndex].m_ani_percent );
				RLG3("aEffectValue = %d, m_ani_percent = %3.3f, m_move_percent = %3.3f", aEffectValue, g_Map.m_Npc[aTableIndex].m_ani_percent, g_Map.m_Npc[aTableIndex].m_move_percent );
			}
		}
		break;
	
	case n_Scroll_HitRate:
		{
			if( aTargetType == n_PC )
			{
				// 주인공에게 적용되는 스킬일 경우...
				if( g_Pc.m_MainPC.lUnique == aTargetID )
				{
//					nRui->thePcParam.Stat2.theAttackHit = aEffectValue;
				}
			}
		}
		break;

	case n_Scroll_AvoidRate:
		{
			if( aTargetType == n_PC )
			{
				// 주인공에게 적용되는 스킬일 경우...
				if( g_Pc.m_MainPC.lUnique == aTargetID )
				{
//					nRui->thePcParam.Stat2.theAttackEvade = aEffectValue;
				}
			}
		}
		break;

	case n_Scroll_DropRate: 
		{
			if( aTargetType == n_PC )
			{
				// 주인공에게 적용되는 스킬일 경우...
				if( g_Pc.m_MainPC.lUnique == aTargetID )
				{
					//nRui->thePcParam.Stat2.theAttackEvade = aEffectValue;
				}
			}
		}
		break;
	
	default:
		break;
	}

	return;
}


// 부가효과는 이펙트나 데미지와 상관없이 처리된다 
void CSkillSystem::UseSkillTargetInfoMgr_AddEffect( Character* pSrcChr, _STargetInfo aTargetInfo, WORD aSkillCode, BOOL bCreateEffect )
{	

	BYTE addeffect_flag = 0;

	// 부과효과 처리
	addeffect_flag = aTargetInfo.theAddEfectFlag;

	static char temp_msg[256] = {0,};
	static char log_msg[256] = {0,};

	SSkillBaseInfo* skill_Info;
	skill_Info = g_Pc_Manager.GetSkillBaseInfoPtr( aSkillCode ); 
	if( skill_Info == NULL )
		return;

	bool bIsDrawSystemMsg = false; 

	if( ( pSrcChr && ( pSrcChr->m_bPC == true ) && ( pSrcChr->m_nTableIndex == MAX_USER_INTABLE ) ) ||
		( ( aTargetInfo.theTargetType == n_PC ) && ( g_Pc.m_MainPC.lUnique == aTargetInfo.theTargetID ) )
	  )
	{
		bIsDrawSystemMsg = true;
		///-- 주체가 본체가 아닌 분신일 경우
		if( pSrcChr )
		{
			if( pSrcChr->m_bPC )
			{
				if( pSrcChr->m_State.nCharState != nCharState_Normal &&
					pSrcChr->m_State.nCharState != nCharState_Master )
				{
					bIsDrawSystemMsg = false;
					return;	
				}
			}
		}
	}

	if( bCreateEffect )
	{
		int Index;
		
		if( aTargetInfo.theTargetType == n_PC )
		{
			if( g_Pc.m_MainPC.lUnique == aTargetInfo.theTargetID )
			{
				if( g_Pc.GetPlayer()->m_KeepUpChecker.FindCode( aSkillCode ) == -1 )
				{
					bCreateEffect = FALSE;
				}
			}
			else
			{
				Index = g_Pc.FindUser( aTargetInfo.theTargetID );
				
				///--JAPAN_BUG_MODIFY
				if( Index == MAX_USER_INTABLE )
					return;
				
				if(g_Pc.m_Pc[ Index ].m_KeepUpChecker.FindCode( aSkillCode ) == -1 )
				{
					bCreateEffect = FALSE;
				}
			}
		}
		else if( aTargetInfo.theTargetType == n_NPC )
		{
			Index = g_Map.FindNpc( aTargetInfo.theTargetID );
			
			if( Index == MAX_NPC_INTABLE )
				return;
			
			if(g_Map.m_Npc[ Index ].m_KeepUpChecker.FindCode( aSkillCode ) == -1 )
			{
				bCreateEffect = FALSE;
			}
		}
	}

	
	if( addeffect_flag )
	{		
		if( skill_Info->theAddEffectTime == 0 )
		{
			return;
		}

		if( bCreateEffect ) 
		{
			int i = 0;
			int j = 0;
			WORD tableIndex = 0;
			BOOL ISFind = FALSE;
			
			// 부가효과도 지속 스킬 아이콘 표시 해줘야 하는데...아직 그려주지 않았다면 지금이라두 처리한다 
			if( aTargetInfo.theTargetType == n_PC )
			{
				if( g_Pc.m_MainPC.lUnique == aTargetInfo.theTargetID )
				{
					for( j = 0; j < RP_MAX_KEEPUP_SKILL; ++j )
					{
						if( nRui->thePcInven.KeepupSkill[j] == aSkillCode )
						{
							ISFind = TRUE;
							
							break;
						}
					}
					
					if( FALSE == ISFind )
					{
						for( j = 0; j < RP_MAX_KEEPUP_SKILL; ++j )
						{
							if( nRui->thePcInven.KeepupSkill[j] == 0  )
							{									
								nRui->thePcInven.KeepupSkill[j] = aSkillCode;
								
								PlayerInfoWnd.SetKeepUpSkillCode( j , aSkillCode );
								
								// 지속성 스킬 이펙트 추가 [상준]
								g_Particle.m_EffectManager.CreateKeepupSkill( aSkillCode , g_Pc.GetPlayer() , NULL , TRUE );					
								g_Particle.m_EffectManager.CreateAddEffectStart( aSkillCode, g_Pc.GetPlayer() );

								if( bIsDrawSystemMsg && skill_Info && skill_Info->theAddEffect )
								{
									Rsprintf( RWCHAR(temp_msg), _RT("[%s] "), G_STRING_CHAR( IDS_ADDEFFECT )  );
									Rsprintf( RWCHAR(log_msg), G_STRING_CHAR( IDS_SYSPR_KEEP_SKILL_ON ), RWCHAR(g_Pc.m_MainPC.char_info.thePCName), RWCHAR(g_Pc_Manager.theAddEffectInfoTable[skill_Info->theAddEffect - 1].theName) );
									SAFE_STR_CAT( temp_msg, log_msg, 256 );
									SystemPrint( n_DarkGreen, n_SystemMsg, temp_msg );		
								}
								
								break;
							}
						}
					}
					else
					{
						///-- 앞서 스킬의 기본효과 자체도 성공한 케이스
						if( bIsDrawSystemMsg && skill_Info && skill_Info->theAddEffect )
						{
							Rsprintf( RWCHAR(temp_msg), _RT("[%s] "), G_STRING_CHAR( IDS_ADDEFFECT )  );
							Rsprintf( RWCHAR(log_msg), G_STRING_CHAR( IDS_SYSPR_KEEP_SKILL_ON ), RWCHAR(g_Pc.m_MainPC.char_info.thePCName), RWCHAR(g_Pc_Manager.theAddEffectInfoTable[skill_Info->theAddEffect - 1].theName) );
							SAFE_STR_CAT( temp_msg, log_msg, 256 );
							SystemPrint( n_DarkGreen, n_SystemMsg, temp_msg );		
						}
					}
					
					for( j = 0; j < RP_MAX_KEEPUP_SKILL; ++j )
					{
						if( g_Pc.GetPlayer()->theAddEffect_SkillCode[j] == 0 || 
							g_Pc.GetPlayer()->theAddEffect_SkillCode[j] == aSkillCode )
						{
							g_Pc.GetPlayer()->theAddEffect_SkillCode[j] = aSkillCode;
							break; 
						}
					}
				}
				else
				{
					tableIndex = g_Pc.FindUser( aTargetInfo.theTargetID );
					if( tableIndex == MAX_USER_INTABLE )
						return;
					
					for( j = 0; j < RP_MAX_KEEPUP_SKILL; ++j )
					{
						if( g_Pc.m_PcTable[tableIndex].char_info.theKeepupSkill[j] == aSkillCode )
						{
							ISFind = TRUE;
							
							break;
						}
					}
					
					if( FALSE == ISFind )
					{
						for( j = 0; j < RP_MAX_KEEPUP_SKILL; ++j )
						{
							if( g_Pc.m_PcTable[tableIndex].char_info.theKeepupSkill[j] == 0 )
							{
								g_Pc.m_PcTable[tableIndex].char_info.theKeepupSkill[j] = aSkillCode;
								
								// 지속성 스킬 이펙트 추가 [상준]
								g_Particle.m_EffectManager.CreateKeepupSkill( aSkillCode , &g_Pc.m_Pc[ tableIndex ] , NULL , TRUE );					
								g_Particle.m_EffectManager.CreateAddEffectStart( aSkillCode, &g_Pc.m_Pc[tableIndex] );
								
								if( bIsDrawSystemMsg && skill_Info && skill_Info->theAddEffect )
								{
									Rsprintf( RWCHAR(temp_msg), _RT("[%s] "), G_STRING_CHAR( IDS_ADDEFFECT )  );
									Rsprintf( RWCHAR(log_msg), G_STRING_CHAR( IDS_SYSPR_KEEP_SKILL_ON ), RWCHAR(g_Pc.m_PcTable[ tableIndex ].char_info.thePCName), RWCHAR(g_Pc_Manager.theAddEffectInfoTable[skill_Info->theAddEffect - 1].theName) );
									SAFE_STR_CAT( temp_msg, log_msg, 256 );
									SystemPrint( n_DarkGreen, n_SystemMsg, temp_msg );			
								}
								break;
							}
						}
					}
					else
					{
						///-- 앞서 스킬의 기본효과 자체도 성공한 케이스
						if( bIsDrawSystemMsg && skill_Info && skill_Info->theAddEffect )
						{
							Rsprintf( RWCHAR(temp_msg), _RT("[%s] "), G_STRING_CHAR( IDS_ADDEFFECT )  );
							Rsprintf( RWCHAR(log_msg), G_STRING_CHAR( IDS_SYSPR_KEEP_SKILL_ON ), RWCHAR(g_Pc.m_PcTable[ tableIndex ].char_info.thePCName), RWCHAR(g_Pc_Manager.theAddEffectInfoTable[skill_Info->theAddEffect - 1].theName) );
							SAFE_STR_CAT( temp_msg, log_msg, 256 );
							SystemPrint( n_DarkGreen, n_SystemMsg, temp_msg );			
						}
					}
				}						
				
			}
			else if( aTargetInfo.theTargetType == n_NPC )
			{
				tableIndex = g_Map.FindNpc( aTargetInfo.theTargetID );
				if( tableIndex == MAX_NPC_INTABLE )
					return;
				
				for( j = 0; j < RP_MAX_KEEPUP_SKILL; ++j )
				{
					if( g_Map.m_NpcTable[tableIndex].theKeepupSkill[j] == aSkillCode )
					{
						ISFind = TRUE;
						
						break;
					}
				}
				
				if( FALSE == ISFind )
				{
					for( j = 0; j < RP_MAX_KEEPUP_SKILL; ++j )
					{
						if( g_Map.m_NpcTable[tableIndex].theKeepupSkill[j] == 0 )
						{
							g_Map.m_NpcTable[tableIndex].theKeepupSkill[j] = aSkillCode;
							
							// 지속성 스킬 이펙트 추가 [상준]
							g_Particle.m_EffectManager.CreateKeepupSkill( aSkillCode , &g_Map.m_Npc[ tableIndex ] , NULL , TRUE );					
							g_Particle.m_EffectManager.CreateAddEffectStart( aSkillCode, &g_Map.m_Npc[tableIndex] );

							if( bIsDrawSystemMsg && skill_Info && skill_Info->theAddEffect )
							{
								Rsprintf( RWCHAR(temp_msg), _RT("[%s] "), G_STRING_CHAR( IDS_ADDEFFECT )  );
								Rsprintf( RWCHAR(log_msg), G_STRING_CHAR( IDS_SYSPR_KEEP_SKILL_ON ), RWCHAR(g_Map.m_NpcTable[ tableIndex ].szName), 
											RWCHAR(g_Pc_Manager.theAddEffectInfoTable[skill_Info->theAddEffect - 1].theName) );
								SAFE_STR_CAT( temp_msg, log_msg, 256 );
								SystemPrint( n_DarkGreen, n_SystemMsg, temp_msg );			
							}
							
							break;
						}
					}
				}
				else
				{
					///-- 앞서 스킬의 기본효과 자체도 성공한 케이스
					if( bIsDrawSystemMsg && skill_Info && skill_Info->theAddEffect )
					{
						Rsprintf( RWCHAR(temp_msg), _RT("[%s] "), G_STRING_CHAR( IDS_ADDEFFECT )  );
						Rsprintf( RWCHAR(log_msg), G_STRING_CHAR( IDS_SYSPR_KEEP_SKILL_ON ), RWCHAR(g_Map.m_NpcTable[ tableIndex ].szName), 
										RWCHAR(g_Pc_Manager.theAddEffectInfoTable[skill_Info->theAddEffect - 1].theName) );
						SAFE_STR_CAT( temp_msg, log_msg, 256 );
						SystemPrint( n_DarkGreen, n_SystemMsg, temp_msg );		
					}
				}
			}
		}
				
		///-- By simwoosung 부가효과가 실제 생성될때만 적용한다. 
		if( bCreateEffect ) 
		{
			UseSkillAddEffectValueMgr( pSrcChr, TRUE, aTargetInfo.theTargetID, aTargetInfo.theTargetType, aSkillCode );
		}
	}
	else
	{
		if( bCreateEffect && bIsDrawSystemMsg && skill_Info && skill_Info->theAddEffect )
		{
			Rsprintf( RWCHAR(temp_msg), _RT("[%s] "), G_STRING_CHAR( IDS_ADDEFFECT )  );
			Rsprintf( RWCHAR(log_msg), G_STRING_CHAR( IDS_SYSPR_KEEP_SKILL_FAIL ), RWCHAR(g_Pc_Manager.theAddEffectInfoTable[skill_Info->theAddEffect - 1].theName) );
			SAFE_STR_CAT( temp_msg, log_msg, 256 );
			SystemPrint( n_DarkRed, n_SystemMsg, temp_msg );			
		}
	}
	
	return;
}


void CSkillSystem::UseSkillAddEffectValueMgr( Character* pSrcChr, BOOL aIsStart, DWORD aTargetID, BYTE aTargetType, WORD aSkillCode )
{
	SSkillBaseInfo* skill_Info;
	skill_Info = g_Pc_Manager.GetSkillBaseInfoPtr( aSkillCode ); 
	if( skill_Info == NULL )
		return;

	WORD aTableIndex = 0;

	if( aTargetType == n_PC )
	{
		if( g_Pc.m_MainPC.lUnique != aTargetID )
		{
			aTableIndex = g_Pc.FindUser( aTargetID );
			if( aTableIndex == MAX_USER_INTABLE )
					return;
		}
	}
	else if( aTargetType == n_NPC )
	{
		aTableIndex = g_Map.FindNpc( aTargetID );		
		if( aTableIndex == MAX_NPC_INTABLE )
			return;
	}

	TCHAR strTemp[256] = {0,};
	CWnd* Wnd = NULL;

	// 부가효과 발동된 것이면 무조건 효력 적용 
	if( aIsStart )
	{
		if( aTargetType == n_PC )
		{
			// 주인공에게 적용되는 스킬일 경우...
			if( g_Pc.m_MainPC.lUnique == aTargetID )
			{
				///-- 주체가 본체가 아닌 분신일 경우
				if( pSrcChr )
				{
					if( pSrcChr->m_bPC )
					{
						if( pSrcChr->m_State.nCharState != nCharState_Normal &&
							pSrcChr->m_State.nCharState != nCharState_Master )
						{
							return;	
						}
					}
				}				
				
				for( int idx = 0; idx < MAX_ADDEFFECT_NUM; ++idx )
				{
					if( g_Pc.GetPlayer()->theAddEffect_State[idx] == 0 )
					{	//유져의 행동에 제약을 걸기 ... 

						g_Pc.GetPlayer()->theAddEffect_State[idx] = g_Pc_Manager.theAddEffectInfoTable[skill_Info->theAddEffect - 1].theAddeffect[idx];
						if(n_TypeNotMove == idx && (skill_Info->theAddEffect == n_AddEff_Panic)) 
						{
							g_Pc.GetPlayer()->theAddEffect_State[idx] = 1;
						}
		
					}
				}

				g_pAddEffectMsg->UpdateEffectMessage(g_Pc.GetPlayer()->theAddEffect_State);
				
				// 강제 타겟일 경우 타겟을 고정시킨다 
				if( g_Pc_Manager.theAddEffectInfoTable[skill_Info->theAddEffect - 1].theAddeffect[n_TypeProvoke] )
				{
					if( pSrcChr )
					{
						if( pSrcChr->m_bPC )
						{							
							g_Pc.m_nPickedUser = pSrcChr->m_nTableIndex;						
						
							nRui->SendEvent( WID_NonPlayerInfo_Wnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );				
							Wnd = Container->Get( WID_NonPlayerInfo_LevelText );
							Rsprintf( RWCHAR(strTemp), _RT("%d"), g_Pc.m_PcTable[g_Pc.m_nPickedUser].char_info.theLevel );
							Wnd->ReceiveEvent(n_emSetText, (EPARAM)strTemp, 240, 240, 200 );		
							Wnd = Container->Get( WID_NonPlayerInfo_NameText );
							Wnd->ReceiveEvent(n_emSetText, (EPARAM)g_Pc.m_PcTable[g_Pc.m_nPickedUser].char_info.thePCName, 240, 240, 200 );				

							NonPlayerInfoWnd.thePickedNpcID = -1;
							NonPlayerInfoWnd.thePickedUserID = g_Pc.m_nPickedUser;				
							NonPlayerInfoWnd.SetPickCharacter( ( Character * )&g_Pc.m_Pc[g_Pc.m_nPickedUser] );							
						}
						else
						{
							g_Map.m_nPickedNpc = pSrcChr->m_nTableIndex;
							
							nRui->SendEvent( WID_NonPlayerInfo_Wnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );				
							Wnd = Container->Get( WID_NonPlayerInfo_LevelText );
							Rsprintf( RWCHAR(strTemp), _RT("%d"), g_Map.m_NpcTable[g_Map.m_nPickedNpc].theLevel );
							Wnd->ReceiveEvent(n_emSetText, (EPARAM)strTemp, 240, 240, 200 );			
							Wnd = Container->Get( WID_NonPlayerInfo_NameText );
							Wnd->ReceiveEvent(n_emSetText, (EPARAM)g_Map.m_NpcTable[g_Map.m_nPickedNpc].szName, 240, 240, 200 );			

							NonPlayerInfoWnd.thePickedNpcID = g_Map.m_nPickedNpc;
							NonPlayerInfoWnd.SetPickCharacter( ( Character * )&g_Map.m_Npc[g_Map.m_nPickedNpc] );												
						}					
					}
				}
			}
			// 다른 유저에게 적용되는 스킬일 경우...
			else
			{		
			}
		}
		else if( aTargetType == n_NPC )
		{
		}		
	}
	// 부가효과 종료된 것이면 다른 스킬에 비추어서 효력 적용
	else
	{
		if( aTargetType == n_PC )
		{
			// 주인공에게 적용되는 스킬일 경우...
			if( g_Pc.m_MainPC.lUnique == aTargetID )
			{
				//부과 효과 초기화 
				ZeroMemory( g_Pc.GetPlayer()->theAddEffect_State, sizeof( BYTE ) * MAX_ADDEFFECT_NUM );

				for(int num =0 ; num < RP_MAX_KEEPUP_SKILL; ++num)
				{
					if( g_Pc.GetPlayer()->theAddEffect_SkillCode[num] == aSkillCode )
					{
						g_Pc.GetPlayer()->theAddEffect_SkillCode[num] = 0;
					}
					else if( g_Pc.GetPlayer()->theAddEffect_SkillCode[num])
					{
						SSkillBaseInfo* effect_skill_Info;
						effect_skill_Info = g_Pc_Manager.GetSkillBaseInfoPtr( g_Pc.GetPlayer()->theAddEffect_SkillCode[num] ); 
												
						if( effect_skill_Info )
						{
							for(int idx = 0; idx < MAX_ADDEFFECT_NUM; ++idx )
							{
								if( g_Pc_Manager.theAddEffectInfoTable[effect_skill_Info->theAddEffect - 1].theAddeffect[idx] )
								{
									g_Pc.GetPlayer()->theAddEffect_State[idx] = 1;
								}
								//랜덤 포지션을 위해서...
								//이동불가 로 셋팅하면 , 서버에서 이동자체를 막아버리기 때문에 .. 
								//패닉 은 이동불가를 추가로 넣어줘야 한다 . 
								if(n_TypeNotMove == idx && (effect_skill_Info->theAddEffect == n_AddEff_Panic)) 
								{
									g_Pc.GetPlayer()->theAddEffect_State[idx] = 1;
								}					

							}
						}
					}				
				}
				g_pAddEffectMsg->UpdateEffectMessage(g_Pc.GetPlayer()->theAddEffect_State);

			}
			// 다른 유저에게 적용되는 스킬일 경우...
			else
			{		
			}
		}
		else if( aTargetType == n_NPC )
		{
		}					
	}

	return;
}

//................................................................................................................
// 스킬 이펙트 생성 By wxywxy
//................................................................................................................
void CSkillSystem::CreateSkill( Character *pSrc, Character *pDest, int skilledIdx, SSkillBaseInfo* pSkillBaseInfo, 
								EVENT_DATA_INFO &underattack_event, EVENT_DATA_INFO& aSkillEvent, _STargetInfo aTargetInfo )
{
	if( NULL == pSkillBaseInfo )
		return;

	// Src 발동 이펙트 처리 ( 효력 처리 없이 이펙트만 생성 )
	if( pSkillBaseInfo->theEffectInfo[0].theEffectType == n_ZERO && 
		pSkillBaseInfo->theEffectInfo[1].theEffectType == n_ZERO && 
		pSkillBaseInfo->theEffectInfo[2].theEffectType == n_ZERO && 
		pSkillBaseInfo->theAddEffect == n_ZERO )
	{	
		// Create() -> Ing() -> End()
		g_Particle.m_EffectManager.CreateKeepupSkill( pSkillBaseInfo->theSkillCode , pSrc , NULL , TRUE );					
	}	
	// 광역 지속 스킬시
	else if( pSkillBaseInfo->theActionKeepupTime == TRUE )
	{
		// Create() <1>-> Ing() -> End() -> EffectTargetHit() : 부가 효과 걸리지 않는다 데미지만 있음
		g_Particle.m_EffectManager.Create( (EFFECT_ID)pSkillBaseInfo->theKeepupEffect , 
			pSrc                 , pDest ,
			underattack_event.type  , aSkillEvent.critical , aSkillEvent.count, &aTargetInfo , 
			aSkillEvent.lStartTime, aSkillEvent.skill_unique );
	}
	// Src 발동 이펙트 처리 + Dest 발동 이펙트 처리 + Dest 지속 이펙트 처리 + Dest 이벤트 처리 + Dest 지속 아이콘 처리 + Dest 효력 처리 
	else if( pSkillBaseInfo->theActionEffect != 0 )
	{
		// Create() <1>-> Ing() -> End() -> EffectTargetHit() -> UseSkillTargetInfoMgr & UseSkillTargetInfoMgr_AddEffect
		//			<2>-> EffectTargetHit() -> UseSkillTargetInfoMgr & UseSkillTargetInfoMgr_AddEffect -> Ing() -> End() 
		g_Particle.m_EffectManager.Create( (EFFECT_ID)pSkillBaseInfo->theActionEffect , 
		pSrc , pDest , underattack_event.type  , aSkillEvent.critical , aSkillEvent.count, 
		&aTargetInfo, aSkillEvent.lStartTime );
	}
	// Dest 지속 이펙트 처리 + Dest 지속 아이콘 처리 + Dest 효력 처리 
	else
	{
	
		// 일종의 EffectTargetHit() -> UseSkillTargetInfoMgr & UseSkillTargetInfoMgr_AddEffect
		UseSkillTargetInfoMgr( pSrc, aTargetInfo, aSkillEvent.count, TRUE );
		UseSkillTargetInfoMgr_AddEffect( pSrc, aTargetInfo, aSkillEvent.count, TRUE );
	}

	return;
}

//---------------------------------------------------------------------------------------------------------
// before : .theHP = .theRealHP;
// after  : .theHP -= Damage;
// => Damage = theRealHP - DrawHP; 
// 서버 메세지를 받자마자 스킬 타입에 따라 데미지 계산을 먼저 한 다음, 클라이언트에서 각 모션에 맞게 이펙트 및 효과를 처리한다. 
//---------------------------------------------------------------------------------------------------------
void CSkillSystem::SetDamageValue( WORD aSkillCode, _STargetInfo *aTargetInfo, BYTE aEffectType, DWORD aAttackID )
{
	
	///--JAPAN_BUG_MODIFY
	int i = 0;
	int skillcase = 0;
	int	skilledIdx = -1;
	int damage = 0;
	BYTE effect_type = 0;
	DWORD effect_value = 0;
	WORD keepup_flag = 0;
	int keepup_time = 0;
	
	int same_effect_type[c_Max_PerSkill_EffectNum];	//마법필살기,물리필살기 같은종류이면 첫번째 배열에 몰아준다 . 
	memset( same_effect_type, 0 , sizeof(int) * c_Max_PerSkill_EffectNum );

	DWORD dDamageTotal = 0;
	BOOL  IsDie = FALSE;	
	
	SSkillBaseInfo* skill_Info = NULL;
	skill_Info = g_Pc_Manager.GetSkillBaseInfoPtr( aSkillCode );	
	
	if( aTargetInfo == NULL || skill_Info == NULL )
		return;
	
	for( i = 0; i < c_Max_PerSkill_EffectNum; ++i )			// 스킬 효력 처리 3가지 처리  
	{
		if( skill_Info->theEffectInfo[i].theEffectType == n_ZERO )
		{
			continue;
		}

		effect_value = aTargetInfo->theEffectValue[i];	
		keepup_flag = aTargetInfo->theKeepupFlag[i];		
		effect_type = skill_Info->theEffectInfo[i].theEffectType;
		keepup_time = skill_Info->theEffectInfo[i].theKeepupTime;

		skillcase = effect_type / 30;

		if( skillcase == 1 )
		{
			effect_type -= 30;
		}
		else if( skillcase == 2 )
		{
			effect_type -= 60;
		}		
 
		if( keepup_time != 0 && keepup_flag == 0 )		// 지속 스킬 이지만 실패 했을 경우...제외 
		{			
			///--JAPAN_BUG_MODIFY
			if( !ISSkipExtraSkill( aSkillCode ) )
			{
				switch( effect_type )
				{
					case n_HPAdd:					// 체력 관련 처리 
					case n_AttackBlowAdd:			// 물리 필살기 
					case n_MagicBlowAdd:
						{
							aTargetInfo->theEffectValue[i] = 0;
						}
				}		
				
				continue;
			}
		}		
		
		switch( effect_type )
		{
		case n_HPAdd:					// 체력 관련 처리 
		case n_AttackBlowAdd:			// 물리 필살기 
		case n_MagicBlowAdd:
			{
				if( aTargetInfo->theTargetType == n_PC )
				{
					if( aTargetInfo->theTargetID == g_Pc.m_MainPC.lUnique )				
					{
						skilledIdx = MAX_USER_INTABLE;
						
						if( aTargetInfo->theDieFlag )
						{
							damage = aTargetInfo->theEffectValue[i];					
							
							g_Pc.m_MainPC.char_info.theRealHP = 0;							
							g_Pc.GetPlayer()->m_lPreTime = g_nowTime;
							g_Pc.GetPlayer()->SetRealDie(TRUE, aEffectType, aAttackID);

							aTargetInfo->theEffectValue[i] = damage;
							dDamageTotal += damage;							
						}
						else
						{
							damage = 0;

							if( aTargetInfo->theEffectValue[i] )
							{
								damage = g_Pc.m_MainPC.char_info.theRealHP - aTargetInfo->theEffectValue[i];							
								g_Pc.m_MainPC.char_info.theRealHP = aTargetInfo->theEffectValue[i];
								///--PRINT_DLG_LOG("나 스킬로 damage - %d theRealHP - %d 들어옴 스킬코드 - %d ", 
								///--	damage, g_Pc.m_MainPC.char_info.theRealHP, aSkillCode );
								g_Pc.GetPlayer()->m_lPreTime = g_nowTime;								
							}
							else
							{
								///--PRINT_DLG_LOG("나 스킬 HP 0 들어옴");
							}							

							if( aEffectType == n_PET && g_Pc.GetPlayer()->m_State.nPetState == nPetState_Master )
							{
								//내 펫이면
								if( aAttackID == g_Map.m_NpcTable[g_Pc.GetPlayer()->m_State.PetIndex].lUnique )
								{
									if( damage < 0 )
									{
										SystemPrint( n_DarkGreen, n_SystemMsg, 
										G_STRING_CHAR( IDS_PET_RECALL_MASTER_HP ), -damage );		
									}
								}	
							}
							
							// 서버에서 아직 DWORD => long 작업 전이므로 임시로 1000000000 의 값을 더해서 사용한다. 							
							// 마이너스 데미지는 즉 회복을 뜻한다. (HP 상승)
							if( damage < 0 )
							{
//								if( skill_Info->theActTarget == n_AT_TARGET )
//								{
									aTargetInfo->theEffectValue[i] = abs( damage ) + c_DamageFlag_Subtract;									
//								}
//								else
//								{
//									aTargetInfo->theEffectValue[i] = 0; 									
//								}
							}
							else
							{
								aTargetInfo->theEffectValue[i] = damage;
								dDamageTotal += damage;
							}
						}		
					}	
					else
					{
						///--JAPAN_BUG_MODIFY
						skilledIdx = g_Pc.FindUser( aTargetInfo->theTargetID );		
						if( skilledIdx != MAX_USER_INTABLE )
						{
							if( aTargetInfo->theDieFlag )
							{								
							
								damage = aTargetInfo->theEffectValue[i];
								
								g_Pc.m_PcTable[skilledIdx].char_info.theRealHP = 0;
								g_Pc.m_Pc[skilledIdx].m_lPreTime = g_nowTime;
								g_Pc.m_Pc[skilledIdx].SetRealDie(TRUE);

								aTargetInfo->theEffectValue[i] = damage;
								dDamageTotal += damage;
							}
							else
							{
								damage = 0;
								
								if( aTargetInfo->theEffectValue[i] )
								{
									damage = g_Pc.m_PcTable[skilledIdx].char_info.theRealHP - aTargetInfo->theEffectValue[i]; 
									g_Pc.m_PcTable[skilledIdx].char_info.theRealHP = aTargetInfo->theEffectValue[i];
									g_Pc.m_Pc[skilledIdx].m_lPreTime = g_nowTime;
								}
								else
								{
									///--PRINT_DLG_LOG("타 유저 스킬 HP 0 들어옴");
								}
								
								if( damage < 0 )
								{
//									if( skill_Info->theActTarget == n_AT_TARGET )
//									{
										aTargetInfo->theEffectValue[i] = abs( damage ) + c_DamageFlag_Subtract;									
//									}
//									else
//									{
//										aTargetInfo->theEffectValue[i] = 0; 									
//									}
								}
								else
								{
									aTargetInfo->theEffectValue[i] = damage;
									dDamageTotal += damage;
								}
							}
						}
					}		
				}
				else if( aTargetInfo->theTargetType == n_NPC )
				{
					skilledIdx = g_Map.FindNpc( aTargetInfo->theTargetID );							
					if( skilledIdx != MAX_NPC_INTABLE )
					{	
						if( aTargetInfo->theDieFlag )
						{							
							damage = aTargetInfo->theEffectValue[i];							
							
							g_Map.m_NpcTable[skilledIdx].theRealHP = 0; 
							g_Map.m_Npc[skilledIdx].m_lPreTime = g_nowTime;
							g_Map.m_Npc[skilledIdx].SetRealDie(TRUE);

							aTargetInfo->theEffectValue[i] = damage;
							dDamageTotal += damage;
							
#ifdef ROCKCLIENT_DEV
							RLG3( "%s Damage = %d, HP = %d", skill_Info->theSkillName, damage, 
															 g_Map.m_NpcTable[skilledIdx].theRealHP );
#endif
						}
						else
						{
							damage = 0;							
							
							if( aTargetInfo->theEffectValue[i] )
							{
								damage = g_Map.m_NpcTable[skilledIdx].theRealHP - aTargetInfo->theEffectValue[i]; 
								g_Map.m_NpcTable[skilledIdx].theRealHP = aTargetInfo->theEffectValue[i];
								g_Map.m_Npc[skilledIdx].m_lPreTime = g_nowTime;

								if( NonPlayerInfoWnd.thePickedNpcID != -1 && 
									NonPlayerInfoWnd.thePickedNpcID == skilledIdx )
								{
									///--PRINT_DLG_LOG("대상HP 들어옴 Damage = %d  theRealHP = %d theHP = %d",
									///--			   damage, g_Map.m_NpcTable[skilledIdx].theRealHP, 
									///--			   g_Map.m_NpcTable[skilledIdx].theHP );
								}
							}
							else
							{
								///--PRINT_DLG_LOG("몹 스킬 HP 0 들어옴");
							}					
						
							if( damage < 0 )
							{
//								if( skill_Info->theActTarget == n_AT_TARGET )
//								{
									aTargetInfo->theEffectValue[i] = abs( damage ) + c_DamageFlag_Subtract;									
//								}
//								else
//								{
//									aTargetInfo->theEffectValue[i] = 0; 									
//								}
							}
							else
							{
								aTargetInfo->theEffectValue[i] = damage;
								dDamageTotal += damage;
							}
						}
					}
				}	
			}
			break;
		default:
			break;
		}

		same_effect_type[i] = effect_type;
	}


	if( aEffectType == n_PET && g_Pc.GetPlayer()->m_State.nPetState == nPetState_Master )
	{
		//내 펫이면
		if( aAttackID == g_Map.m_NpcTable[g_Pc.GetPlayer()->m_State.PetIndex].lUnique )
		{
			if( dDamageTotal > 0 )
			{
				if( g_PetInfoWnd.m_CurPetNum == MERCE_SLOT_NUM )							
				{
					SystemPrint( n_DarkGreen, n_SystemMsg, 
					 G_STRING_CHAR( IDS_SOLDIER_ATTACK_DAMAGE ) , dDamageTotal );		
				}
				else
				{
					SystemPrint( n_DarkGreen, n_SystemMsg, 
					 G_STRING_CHAR( IDS_PET_ATTACK_DAMAGE ), dDamageTotal );
				}										
			}
		}	
	}

	BOOL bFindDamageEffect = FALSE;

	for( i = 0; i < c_Max_PerSkill_EffectNum; ++i )	 // 스킬 효력 처리 3가지 처리  
	{
		switch( same_effect_type[i] )
		{
			case n_AttackBlowAdd:			// 물리 필살기 
			case n_MagicBlowAdd:
				{
					if( !bFindDamageEffect )
					{
						aTargetInfo->theEffectValue[i] = dDamageTotal;
						bFindDamageEffect = TRUE;
					}
					else
					{
						aTargetInfo->theEffectValue[i] = 0;
					}
				}		
		}	
	}

	return;
}


void CSkillSystem::SetDamageValue( DWORD aTargetID, BYTE aTargetType, WORD aSkillCode, WORD aEffectType, DWORD& aEffectValue )
{
	if( aEffectType == n_ZERO )
	{
		return;
	}
	
	int i = 0;
	int skillcase = 0;
	int	skilledIdx = -1;
	int damage = 0;
	BYTE effect_type = 0;
	

	SSkillBaseInfo* skill_Info = NULL;
	skill_Info = g_Pc_Manager.GetSkillBaseInfoPtr( aSkillCode ); 
		
	effect_type = aEffectType;

	skillcase = aEffectType / 30;
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
	case n_HPAdd:					// 체력 관련 처리 
	case n_AttackBlowAdd:			// 물리 필살기 
	case n_MagicBlowAdd:
		{
			if( aTargetType == n_PC )
			{
				if( aTargetID == g_Pc.m_MainPC.lUnique )				
				{
					skilledIdx = MAX_USER_INTABLE;
					
					damage = g_Pc.m_MainPC.char_info.theRealHP - aEffectValue; 					
					
					if( aEffectValue )
					{
						g_Pc.m_MainPC.char_info.theRealHP = aEffectValue;
					}					
					g_Pc.GetPlayer()->m_lPreTime = g_nowTime;
					
					// 서버에서 아직 DWORD => long 작업 전이므로 임시로 1000000000 의 값을 더해서 사용한다. 							
					// 마이너스 데미지는 즉 회복을 뜻한다. (HP 상승)
					if( damage < 0 )
					{
//						if( skill_Info->theActTarget == n_AT_TARGET )
//						{
							aEffectValue = abs( damage ) + c_DamageFlag_Subtract;									
//						}
//						else
//						{
//							aEffectValue = 0; 									
//						}
					}
					else
					{
						aEffectValue = damage;
					}		
				}	
				else
				{
					skilledIdx = g_Pc.FindUser( aTargetID );		
					if( skilledIdx != MAX_USER_INTABLE )
					{
						damage = g_Pc.m_PcTable[skilledIdx].char_info.theRealHP - aEffectValue; 
						g_Pc.m_PcTable[skilledIdx].char_info.theRealHP = aEffectValue;
						g_Pc.m_Pc[skilledIdx].m_lPreTime = g_nowTime;
						
						if( damage < 0 )
						{
							// if( skill_Info->theActTarget == n_AT_TARGET )
							// {
							aEffectValue = abs( damage ) + c_DamageFlag_Subtract;									
							// }
							// else
							// {
							//		aEffectValue = 0; 									
							// }
						}
						else
						{
							aEffectValue = damage;
						}
					}
				}		
			}
			else if( aTargetType == n_NPC )
			{
				skilledIdx = g_Map.FindNpc( aTargetID );							
				if( skilledIdx != MAX_NPC_INTABLE )
				{	
					damage = g_Map.m_NpcTable[skilledIdx].theRealHP - aEffectValue; 
					g_Map.m_NpcTable[skilledIdx].theRealHP = aEffectValue;
					g_Map.m_Npc[skilledIdx].m_lPreTime = g_nowTime;
					
					if( damage < 0 )
					{
						// if( skill_Info->theActTarget == n_AT_TARGET )
						// {
						aEffectValue = abs( damage ) + c_DamageFlag_Subtract;									
						// }
						// else
						// {
						//		aEffectValue = 0; 									
						// }
					}
					else
					{
						aEffectValue = damage;
					}
				}
			}	
			break;
		default:
			break;
		}
		
		RLG1( "=========================aEffectValue = %d ", aEffectValue );
	}

	return;
}


bool CSkillSystem::SetMultiAttackDamageValue( EVENT_DATA_INFO& aEvent, BYTE theAttackType, DWORD theAttackID)
{
	int tgNum = 0;
	int damage = 0; 
	int attackedIdx = 0;
	SAttacked* tg_Info = (SAttacked*) aEvent.target_list;
	bool bIsAttackBack = false;
	
	if( tg_Info )
	{
		for( tgNum = 0; tgNum < aEvent.number; ++tgNum )
		{			
//			DWORD		theAttackedID;		// 맞은 객체
//			DWORD		theAttackedHP;		// 맞은 객체의 남은 HP (유저는 WORD로 변환해야 한다. )
//			BYTE		theAttackedType:1;	// (1)맞은 객체의 타입 PC, NPC
//			BYTE		theDieFlag:2;		// (1)n_Success(죽음) : 죽음 모션처리를 위해 수정됨 
//			BYTE		theCritical:2;		// (1)크리티컬 성공 여부 (n_Success/n_Fail)
						
			if( tg_Info[tgNum].theAttackedType == n_PC )
			{
				if( tg_Info[tgNum].theAttackedID == g_Pc.m_MainPC.lUnique )				
				{
					attackedIdx = MAX_USER_INTABLE;
					
					if( tg_Info[tgNum].theDieFlag )
					{
//						damage = tg_Info[tgNum].theAttackedHP;
						g_Pc.m_MainPC.char_info.theRealHP = 0;
						g_Pc.GetPlayer()->m_lPreTime = g_nowTime;
						g_Pc.GetPlayer()->SetRealDie(TRUE, theAttackType, theAttackID);
					}
					else
					{
						damage = g_Pc.m_MainPC.char_info.theRealHP - tg_Info[tgNum].theAttackedHP; 
						if( tg_Info[tgNum].theAttackedHP )
						{
							g_Pc.m_MainPC.char_info.theRealHP = tg_Info[tgNum].theAttackedHP;
						}
						g_Pc.GetPlayer()->m_lPreTime = g_nowTime;
						tg_Info[tgNum].theAttackedHP = damage;
					}
				}				
				else
				{
					attackedIdx = g_Pc.FindUser( tg_Info[tgNum].theAttackedID );		
					if( attackedIdx != MAX_USER_INTABLE )
					{
						if( tg_Info[tgNum].theDieFlag )
						{
//							damage = tg_Info[tgNum].theAttackedHP; 
							g_Pc.m_PcTable[attackedIdx].char_info.theRealHP = 0;
							g_Pc.m_Pc[attackedIdx].m_lPreTime = g_nowTime;
							g_Pc.m_Pc[attackedIdx].SetRealDie(TRUE);
						}
						else
						{
							damage = g_Pc.m_PcTable[attackedIdx].char_info.theRealHP - tg_Info[tgNum].theAttackedHP; 
							g_Pc.m_PcTable[attackedIdx].char_info.theRealHP = tg_Info[tgNum].theAttackedHP; 
							g_Pc.m_Pc[attackedIdx].m_lPreTime = g_nowTime;
							tg_Info[tgNum].theAttackedHP = damage;
						}
					}
				}				
			}
			else if( tg_Info[tgNum].theAttackedType == n_NPC )
			{
				attackedIdx = g_Map.FindNpc( tg_Info[tgNum].theAttackedID );							
				if( attackedIdx != MAX_NPC_INTABLE )
				{			
					if( tg_Info[tgNum].theDieFlag )
					{
//						damage = tg_Info[tgNum].theAttackedHP; 
						g_Map.m_NpcTable[attackedIdx].theRealHP = 0;
						g_Map.m_Npc[attackedIdx].m_lPreTime = g_nowTime;
						g_Map.m_Npc[attackedIdx].SetRealDie(TRUE);
					}
					else
					{
						damage = g_Map.m_NpcTable[attackedIdx].theRealHP - tg_Info[tgNum].theAttackedHP; 
						g_Map.m_NpcTable[attackedIdx].theRealHP = tg_Info[tgNum].theAttackedHP; 
						g_Map.m_Npc[attackedIdx].m_lPreTime = g_nowTime;
						tg_Info[tgNum].theAttackedHP = damage;
					}
				}
			}
		}
	}		

	return true;
}


//..............................................................................................................
// 아이스 레인과 같은 지속시간은 있지만 단발성 스킬 처리 할것들 정리 by wxywxy
//..............................................................................................................
BOOL CSkillSystem::ISSkipExtraSkill(WORD SkillCode)
{
	
	SSkillBaseInfo* skill_Info = NULL;
	skill_Info = g_Pc_Manager.GetSkillBaseInfoPtr( SkillCode ); 
	
	if(skill_Info->theTarget == n_T_LAND_OTHERCHAR &&
		(skill_Info->theSkillType2 == n_AttackMagic)|| (skill_Info->theSkillType2 == n_DeathBlow) )	
	{
		return TRUE;
	}
	/*
	switch( SkillCode )
	{
		// 아이스 레인
		case 1921:	
		case 1922:	
		case 1923:	
		case 1924:	
		case 1925:	

		// 콜로스 아이스 레인
		case 5509:
		// 필로그 아이스 레인
		case 5529:
			return TRUE;
	}
*/

	return FALSE;
}
