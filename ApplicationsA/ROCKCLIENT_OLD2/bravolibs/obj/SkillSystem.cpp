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


const int c_Max_PerSkill_EffectNum( 3 );		// ��ų�� ȿ�� ���� 
const int c_Max_PerSkill_SkillAddEffectNum( 1 );		// ��ų�� �ΰ�ȿ�� ���� (�����̻� ó��)

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


// ��ų �нú� �� ���� ������ �ִ� ��ų ��� �� ��ų ���̺� ��� ���� Ŭ���� ���� �ʿ� (CSkill_Info)
//- CSkillSystem
//----- SSkill_Info					// ������ ��ų ���̺� ���� (RockBase - basestructure.h) ǥ�� ��
//----- CSkillControl(Interface)	// ��밡�� ��ų ǥ��, ��ų ����Ű ���� �� ���� ��� �� 
//----- CSkill_Action				// ��ų ��� �� Ű ���� 
//---------- CSkill_Action::UseSkill( int aPressKey );	// ����Ű �� ������ ���� 
//----- SM_USE_SKILL				// ��ų ��뿡 �´� ���ó�� �� ������ ��ġ ǥ��, ����Ʈ, ���� 

///--JAPAN_BUG_MODIFY
bool CSkillSystem::UseSkillMgr( int aPressKey, DWORD dSkillCode )
{
	static DWORD delay_time = 0;
	
	DWORD nowTime = (DWORD)g_nowTime;

	if( SAFE_TIME_COMPARE( nowTime , > , SAFE_TIME_ADD( 1000 , delay_time ) ) )
	{
		///--PRINT_DLG_LOG("��ų ��� g_nowTime = %u delay_time + 1000 = %u [Result %d] ", 
		///--g_nowTime, delay_time + 1000 ,(int)(g_nowTime - delay_time) );
		delay_time = nowTime;		
	}			
	else
	{
		///--PRINT_DLG_LOG("��ų �Ұ� g_nowTime = %u delay_time + 1000 = %u [Result %d] ", 
		///--g_nowTime, delay_time + 1000 ,(int)(g_nowTime - (delay_time + 1000)) );
		return false;
	}
	
	// ��ȯ�߿��� ��ų ��� �Ұ� 
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

	skill_code = GetSkillCode( aPressKey );			// ��ų �ڵ尪 
	if( skill_code == 0 )//--PRINT_DLG_LOG("��ų �ڵ� ���� �����ϴ�.. ��� �Ұ� ");
		return false;
		
	///--JAPAN_BUG_MODIFY		
	if( dSkillCode != skill_code )
		return false;

	//...........................................................................................................
	// ��������� ��ų ��� ����
	//...........................................................................................................
	if( g_Pc.GetPlayer()->m_ISApproachWeapon )
	{
		if( !g_Pc_Manager.ISUseSkillOnApproachWeapon( skill_code ) )
		{
			return false;
		}
	}

	///-- By simwoosung
	///-- ���� ���°� ��ų-���� ���̸� ��ų�� ������� ���Ѵ�.
	if( g_Pc.GetPlayer()->m_curt_event.type == SM_USE_SKILL && 
	   ( g_Pc.GetPlayer()->m_curt_event.status == SM_CHANT_READY || 
	     g_Pc.GetPlayer()->m_curt_event.status == SM_CHANT_SPELLING ) )
	{
		return false;
	}	

	//...........................................................................................................
	// ���� ���� ��ƾ
	//...........................................................................................................
	if( skill_code == c_SkillCode_UserSellStore )
	{
		if( g_Pc.GetPlayer()->m_curt_event.status == SM_RESTSKILL_SITDOWN ||
			g_Pc.GetPlayer()->m_curt_event.status == SM_RESTSKILL_KEEPING ||
			g_Pc.GetPlayer()->m_curt_event.status == SM_RESTSKILL_STANDUP   )
		{
			///--PRINT_DLG_LOG("���� ���� �޽� ��ų�� ������Դϴ� .. ");
			return false;
		}
		
		///--PRINT_DLG_LOG("���� ������ �ƴҽ� ������ ���� ");
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

	///--���� ���� �ݷδ� ���� ���
	if( g_Map.m_uMapType == MAP_TYPE_COLONY )
	{
		///-- ��ų �ڵ尡 �ڷ���Ʈ �ϰ��
		if( skill_code >= c_SkillCode_Teleport_Min && skill_code <= c_SkillCode_Teleport_Max )
		{
			///-- ���� ���� ������ ���� ��ġ ���̿� �ǹ��� �����ϴ��� ���θ� Ȯ��
			///-- Ȯ�εǸ�... �ڷ���Ʈ�� �����ش�.
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
				SystemPrint( n_DarkRed, n_SystemMsg, "���濡 ��ֹ��� �־ �ڷ���Ʈ�� �Ұ����մϴ�." );	///--HAN_MSG 
				return FALSE;
			}
			else
			{
				g_RockClient.Send_RpCsDirection( g_Pc.GetPlayer()->m_Mov.now_Yrad );
			}
		}
	}

	// aPressKey -> DIK_1 ~ DIK_12, DIK_LCONTROL, DIK_LSHIFT
	// ����� ���� �Ǿ� �־�� ��� ��ų ��� ���� ( ���� : ���õ� ��� ���� ) 
	// �����ȿ� �־�� ��� ���� ( ���� : �����ȿ� ��� ���� )
	// ��Ÿ���� �����Ǿ� �ִ� ���¿��� �ش� ��ų�� �´� �Ÿ� �� �޺�������, SP, MP ��ġ � �´��� ����� �������� �Ǵ� 
	// ������ �������ݿ� �°� ��ų ������� �˸� 
	// ��ų ��� ������ �°� Ŭ���̾�Ʈ ó�� 
	// ����, ���� ���� ��ų ó�� (����Ʈ, ����)
	if( g_Map.m_NpcTable[NonPlayerInfoWnd.thePickedNpcID].race == c_NpcRace_Item )
		return false;

	// ������ NPC �� �ż��ϰ�� ���� ���� ����

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

	///-- ������ �ڵ� ���� 
	if( skill_code == c_SkillCode_Basic_PickUp )
	{
		nRui->ItemPickUp();
		///--PRINT_DLG_LOG("������ �ڵ� ����");
		return true;		
	}
	
	// ��ų ���̺��� �����Ͽ� ��� ���θ� �Ǵ��Ѵ� 	
	if( CheckEnableUseSkill( skill_code ) == false )
	{
		///--PRINT_DLG_LOG("��ų ���̺��� �����Ͽ� ��뿩�� �Ǵ� skillcode %d", skill_code);
		return false;
	}
	
	///--PRINT_DLG_LOG("theTargetIsPlayer %d ,  theTargetIsPlayerCell %d ", theTargetIsPlayer ,  theTargetIsPlayerCell );

	// ��ų ��� �����ϸ� ������ �˸��� 
	if( theTargetIsPlayer )
	{
		if( g_Pc.GetPlayer()->m_curt_event.type == SM_STATE_DIE )
		{
//			SystemPrint( n_DarkRed, n_SystemMsg, "�̹� �׾��ִ� Ÿ���Դϴ�" );
			return false;
		}
		///--PRINT_DLG_LOG("theTargetIsPlayer ������ ������" );
		g_Pc.GetPlayer()->Illusion_CsSkill( g_Pc.m_MainPC.lUnique, n_PC, skill_code ); 
	}
	else if( theTargetIsPlayerCell )
	{
		if( g_Pc.GetPlayer()->m_curt_event.type == SM_STATE_DIE )
		{
//			SystemPrint( n_DarkRed, n_SystemMsg, "�̹� �׾��ִ� Ÿ���Դϴ�" );
			return false;
		}
		///--PRINT_DLG_LOG("theTargetIsPlayerCell ������ ������" );
		g_Pc.GetPlayer()->Illusion_CsSkill( g_Pc.m_MainPC.lDestBlock, n_Cell, skill_code ); 
	}
	else if( NonPlayerInfoWnd.thePickedNpcID != -1 )
	{
		if( g_Map.m_Npc[NonPlayerInfoWnd.thePickedNpcID].m_curt_event.type == SM_STATE_DIE )
		{
//			SystemPrint( n_DarkRed, n_SystemMsg, "�̹� �׾��ִ� Ÿ���Դϴ�" );
			return false;
		}

		g_Pc.GetPlayer()->Illusion_CsSkill( g_Map.m_NpcTable[NonPlayerInfoWnd.thePickedNpcID].lUnique, n_NPC, skill_code ); 
		g_Pc.GetPlayer()->SetDirection( g_Map.m_Npc[NonPlayerInfoWnd.thePickedNpcID].GetPosTM()._41, g_Map.m_Npc[NonPlayerInfoWnd.thePickedNpcID].GetPosTM()._43 );
	}
	else if( NonPlayerInfoWnd.thePickedUserID != -1 && NonPlayerInfoWnd.thePickedUserID != MAX_USER_INTABLE )
	{
//		if( g_Pc.m_Pc[NonPlayerInfoWnd.thePickedUserID].m_curt_event.type == SM_STATE_DIE )
//		{
//			SystemPrint( n_DarkRed, n_SystemMsg, "�̹� �׾��ִ� Ÿ���Դϴ�" );
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

	if( // ��ó �⺻ ���ݽ� ����� ������ ���� ����
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
	// ��������� ��ų ��� ����
	//...........................................................................................................
	if( g_Pc.GetPlayer()->m_ISApproachWeapon )
	{
		if( !g_Pc_Manager.ISUseSkillOnApproachWeapon( aSkillCode ) )
		{
			return false;
		}
	}

	//...........................................................................................................
	// ���� ���� ��ƾ
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
	// ����� ���� �Ǿ� �־�� ��� ��ų ��� ���� ( ���� : ���õ� ��� ���� ) 
	// �����ȿ� �־�� ��� ���� ( ���� : �����ȿ� ��� ���� )
	// ��Ÿ���� �����Ǿ� �ִ� ���¿��� �ش� ��ų�� �´� �Ÿ� �� �޺�������, SP, MP ��ġ � �´��� ����� �������� �Ǵ� 
	// ������ �������ݿ� �°� ��ų ������� �˸� 
	// ��ų ��� ������ �°� Ŭ���̾�Ʈ ó�� 
	// ����, ���� ���� ��ų ó�� (����Ʈ, ����)	
	if( g_Map.m_NpcTable[NonPlayerInfoWnd.thePickedNpcID].race == c_NpcRace_Item )
		return false;

	
	SSkillBaseInfo* skill_Info;
	skill_Info = g_Pc_Manager.GetSkillBaseInfoPtr( aSkillCode ); 
	if( skill_Info == NULL )
		return false;	

	if( NonPlayerInfoWnd.thePickedNpcID != -1 )
	{
		//dongs ���� �ڱ��ڽ��̸� ��� .. 
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
	
	///--���� ���� �ݷδ� ���� ���
	if( g_Map.m_uMapType == MAP_TYPE_COLONY )
	{
		///-- ��ų �ڵ尡 �ڷ���Ʈ �ϰ��
		if( aSkillCode >= c_SkillCode_Teleport_Min && aSkillCode <= c_SkillCode_Teleport_Max )
		{
			///-- ���� ���� ������ ���� ��ġ ���̿� �ǹ��� �����ϴ��� ���θ� Ȯ��
			///-- Ȯ�εǸ�... �ڷ���Ʈ�� �����ش�.
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
				SystemPrint( n_DarkRed, n_SystemMsg, "���濡 ��ֹ��� �־ �ڷ���Ʈ�� �Ұ����մϴ�." );	///--HAN_MSG 
				return FALSE;
			}
			else
			{
				g_RockClient.Send_RpCsDirection( g_Pc.GetPlayer()->m_Mov.now_Yrad );
			}
		}
	}
	
	///-- ������ �ڵ� ���� 
	if( aSkillCode == c_SkillCode_Basic_PickUp )
	{
		nRui->ItemPickUp();
		return true;		
	}	
	
	// ��ų ���̺��� �����Ͽ� ��� ���θ� �Ǵ��Ѵ� 	
	if( CheckEnableUseSkill( aSkillCode ) == false )
	{
		return false;
	}
	
	// ��ų ��� �����ϸ� ������ �˸��� 
	if( theTargetIsPlayer )
	{
		if( g_Pc.GetPlayer()->m_curt_event.type == SM_STATE_DIE )
		{
//			SystemPrint( n_DarkRed, n_SystemMsg, "�̹� �׾��ִ� Ÿ���Դϴ�" );
			return false;
		}
	
		g_Pc.GetPlayer()->Illusion_CsSkill( g_Pc.m_MainPC.lUnique, n_PC, aSkillCode ); 
	}
	else if( theTargetIsPlayerCell )
	{
		if( g_Pc.GetPlayer()->m_curt_event.type == SM_STATE_DIE )
		{
//			SystemPrint( n_DarkRed, n_SystemMsg, "�̹� �׾��ִ� Ÿ���Դϴ�" );
			return false;
		}
	
		g_Pc.GetPlayer()->Illusion_CsSkill( g_Pc.m_MainPC.lDestBlock, n_Cell, aSkillCode ); 
	}
	else if( NonPlayerInfoWnd.thePickedNpcID != -1 )
	{
		if( g_Map.m_Npc[NonPlayerInfoWnd.thePickedNpcID].m_curt_event.type == SM_STATE_DIE )
		{
//			SystemPrint( n_DarkRed, n_SystemMsg, "�̹� �׾��ִ� Ÿ���Դϴ�" );
			return false;
		}
		
		g_Pc.GetPlayer()->Illusion_CsSkill( g_Map.m_NpcTable[NonPlayerInfoWnd.thePickedNpcID].lUnique, n_NPC, aSkillCode ); 
		g_Pc.GetPlayer()->SetDirection( g_Map.m_Npc[NonPlayerInfoWnd.thePickedNpcID].GetPosTM()._41, g_Map.m_Npc[NonPlayerInfoWnd.thePickedNpcID].GetPosTM()._43 );
	}
	else if( NonPlayerInfoWnd.thePickedUserID != -1 && NonPlayerInfoWnd.thePickedUserID != MAX_USER_INTABLE )
	{
		if( g_Pc.m_Pc[NonPlayerInfoWnd.thePickedUserID].m_curt_event.type == aSkillCode )
		{			
//			SystemPrint( n_DarkRed, n_SystemMsg, "�̹� �׾��ִ� Ÿ���Դϴ�" );
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

	if( // ��ó �⺻ ���ݽ� ����� ������ ���� ����
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
	// ��...������ �͵� �ְ� �����ִ� �͵� �����ϱ� �ΰ�ȿ���� ���� ó���ؾ� �ɵ�..�ϴ��� �� ����...
	// �ΰ�ȿ�� Ÿ�Կ� �°� ����Ʈ�ÿ� ���� �´� ��ǰ� Ư��ȿ���� �׷��ش� 	
	// tg_Info.theEffectType1 ������ Ÿ�Կ� �°� theEffectValue1 �� �����Ű��...
	// theAddEffectType ������ Ÿ�Կ� �°� ���� ������ ��Ű��
	// theKeepupTimeType�� �°� theAddEffectTime ���� ���� �ð��� ǥ���Ͽ� �ش� 
	// �ش� ��ų�� �¾��������� ������ �´� ����� �ִϸ��̼� �����ش� 
	EVENT_DATA_INFO underattack_event;	

	if( aTargetInfo.thePushedCell )
	{
		underattack_event.type = SM_SLIDE_NPC;
		underattack_event.cell = aTargetInfo.thePushedCell;
	}				
	else
	{
//		if( aSkillEvent.loop == false )		// ����
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
	// ����Ʈ �Ŵ����� ��ų Ÿ�Թ� �� ���� �Է�
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


	// (�ʻ��� 1, ���� ������ 2)�ܿ��� �´� ����� ������ ����  
	SSkillBaseInfo* skill_Info;
	skill_Info = g_Pc_Manager.GetSkillBaseInfoPtr( aSkillEvent.count ); 
	if( skill_Info == NULL )
		return;

	int skilledIdx = 0;

	if( aTargetInfo.theTargetType == n_PC )
	{
		//........................................................................................................
		// 1. �� ĳ���Ͱ� �¾����� ó�� by wxywxy
		//........................................................................................................
		if( g_Pc.GetMainPC().lUnique == aTargetInfo.theTargetID )
		{
//			if( g_Pc.GetPlayer()->m_bIsLoaded == true )
			{
				CreateSkill( pSrcChr , g_Pc.GetPlayer() , MAX_USER_INTABLE , skill_Info , underattack_event , aSkillEvent , aTargetInfo );

//				g_Pc.GetPlayer()->SetDirection( pSrcChr->GetPosTM()._41, pSrcChr->GetPosTM()._43 );
				
				#ifdef REAPPEAR_DIFFICULT_BUG_LOG
				///-- �� ��Ƽ�� ���� �׿���??
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
		// 2. �ٸ� ������ �¾����� ó�� �¾����� ó�� by wxywxy
		//........................................................................................................
		else
		{
			skilledIdx = g_Pc.FindUser( aTargetInfo.theTargetID );
			
			if( skilledIdx == MAX_USER_INTABLE )
				return;
			
			// �´� ��� ó��...�̰��� ȿ�� ������ ���߱Ⱑ ��Ʊ�...��...
			// ��...n_DeathBlow �϶��� ���� ó���� �ְ�, n_AttackMagic �϶��� ���� Ư��ȿ���� ������ ó��?!!...
			if( g_Pc.m_Pc[skilledIdx].m_bIsLoaded == true )
			{		
				CreateSkill( pSrcChr , &g_Pc.m_Pc[skilledIdx] , skilledIdx , skill_Info , underattack_event , aSkillEvent , aTargetInfo );

//				g_Pc.m_Pc[skilledIdx].SetDirection( pSrcChr->GetPosTM()._41, pSrcChr->GetPosTM()._43 );
			
			}
		}
		
	}
	//............................................................................................................
	// 3. NPC �¾����� ó�� �¾����� ó�� by wxywxy
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


// ���� ��ư�� ���õǾ� �ִ� ��ų�� �˾Ƴ��� ����Ѵ�. 
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

	// ���� ������ Ű�� �ش��ϴ� ��ų�� �����Ѵ� 
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
			///-- �̵� �Ұ�	- �̻�Ȳ�� ���� �޽� ��ų�������� üũ�ϴ� ��ƾ...;;;
			///-- ���⿡������ �������� ��Ȳ�ΰ�...;;;
			g_RockClient.Send_RpCsStartMove( g_Pc.m_MainPC.lUnique, g_Pc.m_MainPC.lDestBlock, 0 );		
		
			return false;
		}		
	}
	// �⺻ ���� ��ų�� �� �ٸ��� ó�� ^ ^;;
	else if( aSkillCode == c_SkillCode_BasicAttack_Millena || aSkillCode == c_SkillCode_BasicAttack_Rain )
	{
		// �⺻ ���ݵ� �����̰� �ִ�!!!
		if( CheckDelayTime( skill_Info ) == false )
		{			
		}
		// �Ϲ� ���� ��ų�� �ᵵ ���¿� ���� �����ϰ� ���� 
		else if( g_Pc.GetPlayer()->m_curt_event.type == SM_SLIDE_NPC ||				// ũ��Ƽ�ÿ� �¾� �з����ų� �׾����� �̵� �� ���� �Ұ� 
				g_Pc.GetPlayer()->m_curt_event.type == SM_STATE_DIE ||
				g_Pc.GetPlayer()->m_curt_event.type == SM_ATTACK_NPC || 
				( g_Pc.GetPlayer()->m_curt_event.type == SM_USE_SKILL && g_Pc.GetPlayer()->m_curt_event.status != SM_RESTSKILL_KEEPING ) ||			
				g_Pc.GetPlayer()->m_curt_event.status == SM_RESTSKILL_SITDOWN ||
				g_Pc.GetPlayer()->m_curt_event.status == SM_RESTSKILL_STANDUP ) 			
		{			
		}
		// ���� ������ ���� �ȵǰ�.....
		else if( g_Pc.GetPlayer()->m_curt_event.type == SM_USE_SKILL && 
			     ( g_Pc.GetPlayer()->m_curt_event.status == SM_SELLSKILL_KEEPING ||			
				   g_Pc.GetPlayer()->m_curt_event.status == SM_SELLSKILL_SITDOWN ||
				   g_Pc.GetPlayer()->m_curt_event.status == SM_SELLSKILL_STANDUP ) 	)
		{
		}

		// �⺻ ���� ��ų�̴ϱ� �⺻ �������� ó�� ^ ^V
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

				// �޺� �ƴ� ������ ª���ð��� ������ �߻��ȵ� 
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

				// �޺� �ƴ� ������ ª���ð��� ������ �߻��ȵ� 
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


	// ���, Ÿ�̹�(�ʻ���), ���� �Ÿ�, �޺�������, �����̸�������, ����� ������, HP-SP-MP, ���� ����...üũ!!!
	// �ʿ����, ������(100ms), 
	// 1. ��� �� (������ �Ǵ� ���̳�?!!!)
	// 2. �ʻ�� �� �϶� Ÿ�ֿ̹� �°� �������� �Ǵ� 
	// 3. ������ �ð�(��Ÿ��)�� �������� Ȯ�� 
	// 4. ��ų �����Ÿ��� ���Դ��� üũ!!
	// 5. �޺�������, �����̸�������, ����� ������, HP-SP-MP �� �ʿ��� �ɷ�ġ�� �´� �������� Ȯ��!!!
	// 6. �ʿ� ��� ���� ���� Ȯ��
	
	//by dongs �ӽ� �� ..
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

	// ���Ӽ� ���� Ÿ�� ������ ��� ���콺�� Ŭ���ؾ� ��ų ���. ���� Ÿ������ üũ.  
	if( skill_Info->theActTarget == n_AT_TARGETLAND )
//	if( skill_Info->theActTarget == n_AT_SELFLAND )			// test�� �ڵ� 
	{	
		// UserInputProc() ���� ���ش�. 
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
		///--PRINT_DLG_LOG("��ų�Ұ� - g_RockClient.m_MouseControlType == n_MouseType_UseSkill");
		return false;
	}

	// �ΰ�ȿ�� (��ų�Ұ�) ó�� 
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

	// ȣ������, ��� ��Ŭó�� ��� 
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

	if(	// ���� ��-��, ������ ��Ʈ������, �Ҽ���, ���ӳ� 
		( chrAttrIndex == n_Rain_Man || chrAttrIndex == n_Rain_Woman ) &&
		( chrClassType == Stranger || chrClassType == Magician || chrClassType == Prominas || chrClassType == Priest || chrClassType == Holy_Avenger ) 		 
	  )			
	{
		// ������ �⺻ �����̳� ��ų�� ���������� ����� �� �ִ� 
		if( g_Pc.GetPlayer()->m_curt_event.type == SM_ATTACK_NPC ||
			g_Pc.GetPlayer()->m_curt_event.type == SM_USE_SKILL )
		{
//			SystemPrint( n_DarkRed, n_SystemMsg, "��ų�� ����� �� �ִ� ���°� �ƴմϴ�" );
					
			// ��ų �켱 ��� ��å�� ���� ���ε� �������� ��ų �ѹ��� �����ϰ�... 
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
			// ȣ�� ���� ������ �ٸ� ��ų�� ����� ���� ����. ����� �� ��� ����.
			if( g_Pc.GetPlayer()->m_curt_event.type == SM_ATTACK_NPC ||
				g_Pc.GetPlayer()->m_curt_event.type == SM_USE_SKILL )
			{
				///PRINT_DLG_LOG("ȣ�� ���� ������ �ٸ� ��ų�� ����� ���� ���� Line 918 " );

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
	
	// �з���, ���� �������� üũ�� ���� 
	if( g_Pc.GetPlayer()->m_curt_event.type == SM_SLIDE_NPC ||				
		g_Pc.GetPlayer()->m_curt_event.type == SM_STATE_DIE ||
		g_Pc.GetPlayer()->m_curt_event.status == SM_RESTSKILL_SITDOWN ||
		g_Pc.GetPlayer()->m_curt_event.status == SM_RESTSKILL_KEEPING ||
		g_Pc.GetPlayer()->m_curt_event.status == SM_RESTSKILL_STANDUP 		
		) 			
	{		
//		SystemPrint( n_DarkRed, n_SystemMsg, "��ų�� ����� �� �ִ� ���°� �ƴմϴ�" );
		return false;
	}	

	if( // ��ó �⺻ ���ݽ� ����� ������ ���� ����
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
//			SystemPrint( n_DarkRed, n_SystemMsg, "�̹� �׾��ִ� Ÿ���Դϴ�" );
			return false;
		}
	}
	else if( NonPlayerInfoWnd.thePickedNpcID != -1 )
	{
		if( g_Map.m_Npc[NonPlayerInfoWnd.thePickedNpcID].m_curt_event.type == SM_STATE_DIE )
		{
//			SystemPrint( n_DarkRed, n_SystemMsg, "�̹� �׾��ִ� Ÿ���Դϴ�" );
			return false;
		}
	}
	else if( NonPlayerInfoWnd.thePickedUserID != -1 && NonPlayerInfoWnd.thePickedUserID != MAX_USER_INTABLE )
	{
//		if( g_Pc.m_Pc[NonPlayerInfoWnd.thePickedUserID].m_curt_event.type == SM_STATE_DIE )
//		{
//			SystemPrint( n_DarkRed, n_SystemMsg, "�̹� �׾��ִ� Ÿ���Դϴ�" );
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
//			SystemPrint( n_DarkRed, n_SystemMsg, "%s ��ų�� ������ ��� �����մϴ�", NeedSkill_Info->theSkillName );
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
//			SystemPrint( n_DarkRed, n_SystemMsg, "%s ��ų�� ������ ��� �����մϴ�", NeedSkill_Info->theSkillName );
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
	n_T_SELF				 = 1,		//  �ڱ��ڽ�					������ ���, �����ڽ�
	n_T_SAMECHAR_NOTSELF	 = 2,		//  ������ ���� ����			������ ���, �ٸ� ����
	n_T_SAMECHAR			 = 3,		//	������ ����	& ����			������ ��� �ڽŰ� �ڽ��� ������
	n_T_NPC					 = 4,		//  ��������							-
	n_T_OTHERCHAR			 = 5,		//	Ÿ���� ���� & ����		������ ���, ��� PC��
	n_T_ALL_CHAR			 = 6,		//  ��ü 
	n_T_LANDTARGET			 = 7,
	n_T_PARTY				 = 8,		// ��Ƽ������ ���
	n_T_LAND_SAMECHAR		 = 9,		// ��������
	n_T_LAND_OTHERCHAR		 = 10,
	n_T_LAND_PARTY			 = 11,
};

enum NActTarget
{
	n_AT_NONE				= 0,		// ����� ����. (����)
	n_AT_SELF				= 1,
	n_AT_TARGET				= 2,		// ĳ���� ��� (��ǥ ������ ��ų ����)
	n_AT_SELFLAND			= 3,		// ������ ���ִ� ��ǥ 
	n_AT_TARGETLAND			= 4,		// �ٸ� ��ǥ (��ų ������ ��ǥ ����)
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
		if( aSkill_Info->theTarget == n_T_NONE )				// ����� ����...�Ѥ�;
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
		else if( aSkill_Info->theTarget == n_T_SAMECHAR )				// �ָ��� ����� ó�� ���� �ʱ�� ���� ^ ^/ ������ �Ǵܿ� �ñ���...
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
		
//	SystemPrint( n_DarkRed, n_SystemMsg, "��ų�� ����� �� �ִ� ����� �ƴմϴ�" );
	return false;
}

bool CSkillSystem::CheckAttackFrame( SSkillBaseInfo* aSkill_Info )
{
	return true;

	if( aSkill_Info->theSkillType2 == n_DeathBlow )			// �ʻ��� ��ų�� ��쿡�� ������ ������ �Ѵ� 
	{
		int chrAttrIndex = g_Pc.GetPlayer()->GetAttrIndex();
		int chrClassType = nRui->thePcParam.MainClass;
				

		// ��ó�϶��� ������ ���ʿ� By wxywxy
		if( ( chrAttrIndex == n_Millena_Man || chrAttrIndex == n_Millena_Woman ) &&
			( chrClassType == Wanderer || 
			  chrClassType == Archer || chrClassType == Sniper ) ) 
		{
			return TRUE;
		}
		else if( 
				(	// �з��� ��-��, ������ ������, �ҵ��, ����Ʈ, �ӽóʸ�, �����̾�  
					( chrAttrIndex == n_Millena_Man || chrAttrIndex == n_Millena_Woman ) &&
					( chrClassType == Wanderer || 
					  chrClassType == Swordman || chrClassType == Knight || chrClassType == Mercenary || chrClassType == Slayer ) 
				) ||
				(	// ���� ��-��, ������ ��Ʈ������, �Ҽ���, ���ӳ� 
					( chrAttrIndex == n_Rain_Man || chrAttrIndex == n_Rain_Woman ) &&
					( chrClassType == Stranger || 
					  chrClassType == Shaman || chrClassType == Psyche ) 
				)
			  )
		{
			int skill_state = g_Pc.GetPlayer()->theCombo_State;
			
			// �޺� ���϶��� ��ų �ߺ� ��� ���� 
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
//					SystemPrint( n_DarkRed, n_SystemMsg, "��ų�� ����� �� �ִ� Ÿ�̹��� �ƴմϴ�" );
				}
			}
		}
		// ������ 
		else if(	// ���� ��-��, ������ ��Ʈ������, �Ҽ���, ���ӳ� 
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
//		SystemPrint( n_DarkRed, n_SystemMsg, "��ų�� �����Ÿ����� ������ϴ�" );
	}
	
	return false;
}


bool CSkillSystem::CheckNeedStatus( SSkillBaseInfo* aSkill_Info )
{
	if( aSkill_Info->theToggleFlag )
	{
		for( int i = 0; i < RP_MAX_KEEPUP_SKILL; ++i )
		{
			// �̹� �ߵ��� ����� ��ų�� ���ؼ��� �ٽ� ��ų�� ���� ������ �ȴ�...���� ��ų�� ������ �Ҹ� �ȵǴϱ�...^^			
			if( nRui->thePcInven.KeepupSkill[i] == aSkill_Info->theSkillCode )
			{				
				return true;
			}
		}
	}

	// 4. �޺�������, �����̸�������, ����� ������, HP-SP-MP, ���� ���� �� �ʿ��� �ɷ�ġ�� �´� �������� Ȯ��!!!

	int gauge_type = nRui->GetPcParamInfo()->GageType;
	int gauge_value = aSkill_Info->theGageValue;

	// �ʿ� ������ ���� ( 0:none, 1:comboG, 2:conditionG, 3:proimaG )
	switch( gauge_type )
	{
	case n_ComboGage:
		{
			if( gauge_value > nRui->thePcParam.Stat1.theGage )
			{
//				SystemPrint( n_DarkRed, n_SystemMsg, "�޺��������� �����մϴ�" );
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
//				SystemPrint( n_DarkRed, n_SystemMsg, "�����̸� �������� �����մϴ�" );
				return false;
			}
		}
		break;
	default:
		break;
	}

	if( aSkill_Info->theWasteHP > nRui->thePcParam.Stat1.theHP )
	{
//		SystemPrint( n_DarkRed, n_SystemMsg, "�ʿ� ü���� �����մϴ�" );
		return false;
	}
	if( aSkill_Info->theWasteMP > nRui->thePcParam.Stat1.theMP )
	{
//		SystemPrint( n_DarkRed, n_SystemMsg, "�ʿ� ������ �����մϴ�" );
		return false;
	}
	if( aSkill_Info->theWasteSP > nRui->thePcParam.Stat1.theSP )
	{
//		SystemPrint( n_DarkRed, n_SystemMsg, "�ʿ� ����� �����մϴ�" );
		return false;
	}

	return true;
}


bool CSkillSystem::CheckNeedItem( SSkillBaseInfo* aSkill_Info )
{
/*
	n_Type_Sword		= 1,		// 1  ��
	n_Type_TwohandSword	= 2,		// 2  ���
	n_Type_Bow			= 3,		// 3  Ȱ
	n_Type_MagicBook	= 4,		// 4  ������
	n_Type_Staff		= 5,		// 5  ������
	n_Type_CrystalBall	= 6,		// 6  ������
	n_Type_Shield		= 100,		// 100 ����
*/
	SItemBaseInfo* item_Info;
	

	switch( aSkill_Info->theNeedItem )
	{
	case 0:				// ���ʿ� ������ ��� ���� 
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

//	SystemPrint( n_DarkRed, n_SystemMsg, "��ų ��뿡 �ʿ��� ��� �����ϼž� �մϴ� " );
	return false;
}


//-----------------------------------------------------------------------------
void CSkillSystem::SaveShortcutSkill()
{
	FILE* fp = _tfopen( _T("Chr\\Shortcut.dat"), _T("w+t") );
	if( fp == NULL )
	{
		//MessageBox( NULL, _T("Shortcut ���� ���� ����!!!"), _T("�˸�"), MB_OK );
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
		//MessageBox( NULL, _T("Shortcut ���� ���� ����!!!"), _T("�˸�"), MB_OK );
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
//	BYTE		theEffectType1;			// 1(HPAdd) ~ 30(LuckAdd) ���� 
//	short		theEffectValue1;		// Damage(-) �Ǵ� ȸ������(+) (HP ����)
//	BYTE		theAddEffectType;		// �ΰ�ȿ��
//	DWORD		theKeepupTimeType:1;	// n_EffectTime, n_AddEffectTime
//	DWORD		theKeepupTime:31;		// ���ӽð�
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//	DWORD		thePushedCell;			// �б� �������� ���� ���� ���� ��� 0:�� �и� ���
//	BYTE		theDieFlag:1;			// (1)n_Success(����) : ���� ���ó���� ���� ������ 
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
typedef struct _Effect {
	BYTE	theEffectType;			// ȿ�¼�ġó�������1	 NCalculate ���� 
	short	theEffectValue;			// ȿ�¼�ġ1
	long	theKeepupTime;			// ���ӽð�(��)		0 ~ 86400	-1:������ ����� ���� ������ Ǯ���� ó��
	BYTE	theSuccRate;			// ����Ȯ��1		// 1~100(%)
}Effect, *EffectPtr;

_Effect	theEffectInfo[3];			// ȿ�� * 3

struct _STargetInfo
{
	DWORD		theTargetID;	
	
	DWORD		theTargetType:1;		// n_PC, n_NPC
	DWORD		theDieFlag:1;			// (1)n_Success(����) : ���� ���ó���� ���� ������
	DWORD		thePushedCell:30;		// �б� �������� ���� ���� ���� ��� 0:�� �и� ���( 1 ~ 262,144 )
	BYTE		theKeepupFlag[3];		// n_Success(���Ӽ� ����)
	DWORD		theEffectValue[3];		// ȿ��ó�� ��
	WORD		theEffectMaxValue;		// ȿ��ó�� Max��( theMaxAttackPower or theMaxMagicPower )
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
	
	for( i = 0; i < c_Max_PerSkill_EffectNum; ++i )			// ��ų ȿ�� ó�� 3���� ó��  
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

		// ���� ��ų ���� ���� ó�� 
		// �������̽� �� ����Ʈ ó���� ���ؼ�...
		if( keepup_time == 0 )				// ���� ��ų�� �ƴ� �͵�...���� 
		{		
		}
		else if( keepup_time != 0 && keepup_flag == 0 )		// ���� ��ų ������ ���� ���� ���...���� 
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
		else													// ���� ��ų ����
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
										
										// ���Ӽ� ��ų ����Ʈ �߰� [����]
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
										
										// ���Ӽ� ��ų ����Ʈ �߰� [����]
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
									
									// ���Ӽ� ��ų ����Ʈ �߰� [����]
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
		
	// ���� ��ų�� ���� ȿ��ġ ���� 
	switch( aEffectType )
	{
	case n_HPAdd:
//	case n_HPMultiply:
//	case n_HPPercent:
		{
#ifdef ROCKCLIENT_DEV
			RLG1( "=========================UseSkillEffectValueMgr n_HPdd = %d", aEffectValue );
#endif
			

//			// �������� �������ֱ� ��������...�ӽ� �۾� ������ ���� �� .. 
			if( aEffectValue >= c_DamageFlag_Subtract )
			{
				aEffectValue -= c_DamageFlag_Subtract;
				aEffectValue *= -1;
			}

			if( aTargetType == n_PC )
			{
				// ���ΰ����� ����Ǵ� ��ų�� ���...
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
						///-- by simwoosung ���� �׾����� �ʴ°�쿡�� �����Ų��.
						if( !(g_Pc.GetPlayer()->IsRealDie()) )
						{
							nRui->thePcParam.Stat1.theHP -= aEffectValue;
							//PRINT_DLG_LOG("�� ��ų�� damage - %d theHP - %d ����", aEffectValue, 
							//		           g_Pc.m_MainPC.char_info.theRealHP);
							g_Pc.GetPlayer()->m_lPreTime = g_nowTime;													
						}

						if( nRui->thePcParam.Stat1.theHP >= nRui->GetPcParamInfo()->MaxHP )
						{
							//������ 
							nRui->thePcParam.Stat1.theHP = nRui->GetPcParamInfo()->MaxHP;
							g_Pc.m_MainPC.char_info.theRealHP = nRui->GetPcParamInfo()->MaxHP;
							g_Pc.GetPlayer()->m_lPreTime = g_nowTime;
						}

						//dongsĳ�� �ǹ��� ���� .. 
						if( (  (int)g_Pc.m_MainPC.char_info.theRealHP > 0 ) && g_Pc.m_MainPC.char_info.theRealHP >= nRui->thePcParam.Stat1.theHP ) 
						{
							nRui->thePcParam.Stat1.theHP = g_Pc.m_MainPC.char_info.theRealHP;
							g_Pc.GetPlayer()->m_lPreTime = g_nowTime;
						}
					}
					
				}
				// �ٸ� �������� ����Ǵ� ��ų�� ���...
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
						///-- by simwoosung ���� �׾����� �ʴ°�쿡�� �����Ų��.
						if( !(g_Pc.m_Pc[aTableIndex].IsRealDie()) )
						{
							g_Pc.m_PcTable[aTableIndex].char_info.theHP -= aEffectValue;
							g_Pc.m_Pc[aTableIndex].m_lPreTime = g_nowTime;
						}

						if( g_Pc.m_PcTable[aTableIndex].char_info.theHP >= 
							g_Pc.m_PcTable[aTableIndex].char_info.theMaxHP )
						{
							//������ 
							g_Pc.m_PcTable[aTableIndex].char_info.theHP = 
												g_Pc.m_PcTable[aTableIndex].char_info.theMaxHP;
							g_Pc.m_PcTable[aTableIndex].char_info.theRealHP  = 
												g_Pc.m_PcTable[aTableIndex].char_info.theMaxHP;
							g_Pc.m_Pc[aTableIndex].m_lPreTime = g_nowTime;	
						}
						
						// dongs �߰� ĳ���� �ǹ��� .. 
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
					///-- by simwoosung ���� �׾����� �ʴ°�쿡�� �����Ų��.
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
				// ��t1�ΰ����� ����Ǵ� ��ų�� ���...
				if( g_Pc.m_MainPC.lUnique == aTargetID && g_Pc.m_MainPC.char_info.theCitizen == MILLENA  )
				{
					nRui->thePcParam.Stat1.theSP = aEffectValue;
				}
				// �ٸ� �������� ����Ǵ� ��ų�� ���...
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
				// �� �Ǵ� ���� NPC �鿡�� ����Ǵ� ��ų�� ���...
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
				
				// ���ΰ����� ����Ǵ� ��ų�� ���...
				if( g_Pc.m_MainPC.lUnique == aTargetID && g_Pc.m_MainPC.char_info.theCitizen == RAIN )
				{
					if(!aEffectValue)
					{
						int a = 0;
					}
					
					nRui->thePcParam.Stat1.theMP = aEffectValue;	
				}
				// �ٸ� �������� ����Ǵ� ��ų�� ���...
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
				// �� �Ǵ� ���� NPC �鿡�� ����Ǵ� ��ų�� ���...
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
				// ���ΰ����� ����Ǵ� ��ų�� ���...
				if( g_Pc.m_MainPC.lUnique == aTargetID )
				{
					nRui->thePcParam.Vital = aEffectValue;
//					nRui->thePcParam.MaxHP = aEffectValue * 5;
				}
				// �ٸ� �������� ����Ǵ� ��ų�� ���...
				else
				{
//					g_Pc.m_PcTable[aTableIndex].char_info.theMaxHP = aEffectValue * 5;
				}
			}
			else if( aTargetType == n_NPC )
			{
				// �� �Ǵ� ���� NPC �鿡�� ����Ǵ� ��ų�� ���...
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
				// ���ΰ����� ����Ǵ� ��ų�� ���...
				if( g_Pc.m_MainPC.lUnique == aTargetID )
				{
					nRui->thePcParam.Stamina = aEffectValue;
//					nRui->thePcParam.MaxSP = aEffectValue * 3;
				}
				// �ٸ� �������� ����Ǵ� ��ų�� ���...
				else
				{
//					g_Pc.m_PcTable[aTableIndex].char_info.theMaxSMP = aEffectValue * 3;
				}
			}
			else if( aTargetType == n_NPC )
			{
				// �� �Ǵ� ���� NPC �鿡�� ����Ǵ� ��ų�� ���...
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
				// ���ΰ����� ����Ǵ� ��ų�� ���...
				if( g_Pc.m_MainPC.lUnique == aTargetID )
				{
					nRui->thePcParam.Intel = aEffectValue;
//					nRui->thePcParam.MaxMP = aEffectValue * 3;
				}
				// �ٸ� �������� ����Ǵ� ��ų�� ���...
				else
				{
//					g_Pc.m_PcTable[aTableIndex].char_info.theMaxSMP = aEffectValue * 3;
				}
			}
			else if( aTargetType == n_NPC )
			{
				// �� �Ǵ� ���� NPC �鿡�� ����Ǵ� ��ų�� ���...
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
					// �ٸ� �������� ����Ǵ� ��ų�� ���...// ���ΰ����� ����Ǵ� ��ų�� ���...
					nRui->thePcParam.Dex = aEffectValue;
				}				
				else
				{	// �ٸ� �������� ����Ǵ� ��ų�� ���...
				}
			}			
			else if( aTargetType == n_NPC )
			{	// �� �Ǵ� ���� NPC �鿡�� ����Ǵ� ��ų�� ���...
			}
		}
		break;	
	case n_HPSpeedAdd:					// HP ȸ���ӵ�
//	case n_HPSpeedMultiply:
//	case n_HPSpeedPercent:
	case n_SPSpeedAdd:					// SP ȸ���ӵ�
//	case n_SPSpeedMultiply:					
//	case n_SPSpeedPercent:					
	case n_MPSpeedAdd:					// MP ȸ���ӵ� 
//	case n_MPSpeedMultiply:				
//	case n_MPSpeedPercent:				
		break;
	case n_AttackSpeedAdd:				// ���ݼӵ�(����)
//	case n_AttackSpeedMultiply:				
//	case n_AttackSpeedPercent:				
		{
			if( aTargetType == n_PC )
			{
				// ���ΰ����� ����Ǵ� ��ų�� ���...
				if( g_Pc.m_MainPC.lUnique == aTargetID )
				{
					///-- By simwoosung
					///-- Ȧ�� ����� ġ���� �� ����... �̰����� ���� HP���� ���´�.
					///-- �������� ���� ������ �𸣰��ִµ�������.. ����� ũ��Ƽ���� �����̴�.
					///-- �ؼ� �ӽ������� �����ִ� �ڵ带 �־��ش�.
					///-- ���°��� HP�� �����ϸ�... ���� ��Ų��.					
					if( aEffectValue == g_Pc.m_MainPC.char_info.theRealHP )
					{
						///--PRINT_DLG_LOG( "aEffectValue = %d g_Pc.GetPlayer()->m_Weapon_DelayTime = %d ", 
						///--				aEffectValue , g_Pc.GetPlayer()->m_Weapon_DelayTime );
						return;
					}
					
					nRui->thePcParam.Stat2.theAttackSpeed = aEffectValue;
					g_Pc.GetPlayer()->m_Weapon_DelayTime = aEffectValue * 100;		// 100ms������ �����ֹǷ� ������ �ʿ� 
								
				}
				// �ٸ� �������� ����Ǵ� ��ų�� ���...
				else
				{
					//					g_Pc.m_PcTable[aTableIndex].char_info.theMaxSMP = aEffectValue;
				}
			}
			else if( aTargetType == n_NPC )
			{
				// �� �Ǵ� ���� NPC �鿡�� ����Ǵ� ��ų�� ���...
				//				g_Map.m_NpcTable[aTableIndex].theSMP = aEffectValue;
			}
		}
		break;
	case n_MoveSpeedAdd:				// �̵��ӵ�
//	case n_MoveSpeedMultiply:		
//	case n_MoveSpeedPercent:		
		{
#ifdef ROCKCLIENT_DEV
//			SystemPrint( n_DarkRed, n_SystemMsg, _RT("Skill Value :: n_MoveSpeed = %d"), aEffectValue );
#endif

			if( aTargetType == n_PC )
			{
				// ���ΰ����� ����Ǵ� ��ų�� ���...
				if( g_Pc.m_MainPC.lUnique == aTargetID )
				{
					nRui->thePcParam.Stat2.theMoveSpeed = aEffectValue;					
					g_Pc.GetPlayer()->m_ani_percent = ( (float)aEffectValue / c_Pc_MoveSpeed );					
					g_Pc.GetPlayer()->m_move_percent = c_Pc_MoveSpeed / ( c_Pc_MoveSpeed * g_Pc.GetPlayer()->m_ani_percent );					
				}
				// �ٸ� �������� ����Ǵ� ��ų�� ���...
				else
				{
					g_Pc.m_Pc[aTableIndex].m_ani_percent = ( (float)aEffectValue / c_Pc_MoveSpeed );					
					g_Pc.m_Pc[aTableIndex].m_move_percent = c_Pc_MoveSpeed / ( c_Pc_MoveSpeed * g_Pc.m_Pc[aTableIndex].m_ani_percent );					
				}
			}
			else if( aTargetType == n_NPC )
			{
				// �� �Ǵ� ���� NPC �鿡�� ����Ǵ� ��ų�� ���...
				SNPCBaseInfo*	npc_Info = NULL;
				npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( g_Map.m_NpcTable[aTableIndex].code ); 
				g_Map.m_Npc[aTableIndex].m_ani_percent = ( (float)aEffectValue / npc_Info->theMoveSpeed );					
				g_Map.m_Npc[aTableIndex].m_move_percent = npc_Info->theMoveSpeed / ( npc_Info->theMoveSpeed * g_Map.m_Npc[aTableIndex].m_ani_percent );
				RLG3("aEffectValue = %d, m_ani_percent = %3.3f, m_move_percent = %3.3f", aEffectValue, g_Map.m_Npc[aTableIndex].m_ani_percent, g_Map.m_Npc[aTableIndex].m_move_percent );
			}
		}
		break;
	case n_AttackPowerAdd:				// ��������(�ּ�, �ִ�ġ ��� ����)
//	case n_AttackPowerMultiply:				
//	case n_AttackPowerPercent:				
		{

/*
enum NSkillType2
{
	n_DeathBlow		 = 1,		// �ʻ���
	n_AttackMagic	 = 2,		// ���ݸ�����
	n_Reserved		 = 3,		// ����
	n_GoodSupport	 = 4,		// ������ - Good
	n_Regain		 = 5,		// ȸ����
	n_ItemSkill		 = 6,		// �����۰�
	n_Produce		 = 7,		// �����
	n_BadSupport	 = 8,		// ������ - Good
};
*/
/*			if( skill_Info->theSkillType2 == n_DeathBlow || skill_Info->theSkillType2 == n_AttackMagic )
			{
				if( aTargetType == n_PC )
				{
					// ���ΰ����� ����Ǵ� ��ų�� ���...
					if( g_Pc.m_MainPC.lUnique == aTargetID )
					{
						nRui->thePcParam.Stat1.theHP = aEffectValue;
						g_Pc.m_MainPC.char_info.theRealHP = aEffectValue; 
					}
					// �ٸ� �������� ����Ǵ� ��ų�� ���...
					else
					{
						g_Pc.m_PcTable[aTableIndex].char_info.theHP = aEffectValue;
						g_Pc.m_PcTable[aTableIndex].char_info.theRealHP = aEffectValue; 
					}
				}
				else if( aTargetType == n_NPC )
				{
					// �� �Ǵ� ���� NPC �鿡�� ����Ǵ� ��ų�� ���...
					g_Map.m_NpcTable[aTableIndex].theHP = aEffectValue;
					g_Map.m_NpcTable[aTableIndex].theRealHP = aEffectValue; 
				}
			}
			else */
			{
				if( aTargetType == n_PC )
				{
					// ���ΰ����� ����Ǵ� ��ų�� ���...
					if( g_Pc.m_MainPC.lUnique == aTargetID )
					{
						nRui->thePcParam.Stat2.theMinAttackPower = aEffectValue;
						nRui->thePcParam.Stat2.theMaxAttackPower = aEffectMaxValue;
					}
					// �ٸ� �������� ����Ǵ� ��ų�� ���...
					else
					{
						//					g_Pc.m_PcTable[aTableIndex].char_info.theMaxSMP = aEffectValue;
					}
				}
				else if( aTargetType == n_NPC )
				{
					// �� �Ǵ� ���� NPC �鿡�� ����Ǵ� ��ų�� ���...
					//				g_Map.m_NpcTable[aTableIndex].theSMP = aEffectValue;
				}
			}
		}
		break;
	case n_AttackDefenseAdd:			// �������
//	case n_AttackDefenseMultiply:		
//	case n_AttackDefensePercent:		
		{
			if( aTargetType == n_PC )
			{
				// ���ΰ����� ����Ǵ� ��ų�� ���...
				if( g_Pc.m_MainPC.lUnique == aTargetID )
				{
					nRui->thePcParam.Stat2.theAttackDef = aEffectValue;
				}
				// �ٸ� �������� ����Ǵ� ��ų�� ���...
				else
				{
					//					g_Pc.m_PcTable[aTableIndex].char_info.theMaxSMP = aEffectValue;
				}
			}
			else if( aTargetType == n_NPC )
			{
				// �� �Ǵ� ���� NPC �鿡�� ����Ǵ� ��ų�� ���...
				//				g_Map.m_NpcTable[aTableIndex].theSMP = aEffectValue;
			}
		}
		break;
//	case n_AttackAttriAdd:				// ���Ӽ�(���Ӽ��� 0�� ����� ���� �����ۿ��� ����Ǿ� �Ӽ����� �ο��Ѵ�)
//	case n_AttackAttriMultiply:	
//	case n_AttackAttriPercent:	
//		break;
	case n_MagicResAdd:
		{
			if( aTargetType == n_PC )
			{
				// ���ΰ����� ����Ǵ� ��ų�� ���...
				if( g_Pc.m_MainPC.lUnique == aTargetID )
				{
					nRui->thePcParam.Stat2.theMagicRes = aEffectValue;					
				}
				// �ٸ� �������� ����Ǵ� ��ų�� ���...
				else
				{
				}
			}
			else if( aTargetType == n_NPC )
			{
			}
		}
		break;
	case n_MagicPowerAdd:				// ��������
//	case n_MagicPowerMultiply:		
//	case n_MagicPowerPercent:		
		{
/*			if( skill_Info->theSkillType2 == n_DeathBlow || skill_Info->theSkillType2 == n_AttackMagic )
			{
				if( aTargetType == n_PC )
				{
					// ���ΰ����� ����Ǵ� ��ų�� ���...
					if( g_Pc.m_MainPC.lUnique == aTargetID )
					{
						nRui->thePcParam.Stat1.theHP = aEffectValue;
						g_Pc.m_MainPC.char_info.theRealHP = aEffectValue; 
					}
					// �ٸ� �������� ����Ǵ� ��ų�� ���...
					else
					{
						g_Pc.m_PcTable[aTableIndex].char_info.theHP = aEffectValue;
						g_Pc.m_PcTable[aTableIndex].char_info.theRealHP = aEffectValue; 
					}
				}
				else if( aTargetType == n_NPC )
				{
					// �� �Ǵ� ���� NPC �鿡�� ����Ǵ� ��ų�� ���...
					g_Map.m_NpcTable[aTableIndex].theHP = aEffectValue;
					g_Map.m_NpcTable[aTableIndex].theRealHP = aEffectValue; 
				}
			}
			else */
			{
				if( aTargetType == n_PC )
				{
					// ���ΰ����� ����Ǵ� ��ų�� ���...
					if( g_Pc.m_MainPC.lUnique == aTargetID )
					{
						nRui->thePcParam.Stat2.theMinMagicPower = aEffectValue;
						nRui->thePcParam.Stat2.theMaxMagicPower = aEffectMaxValue;
					}
					// �ٸ� �������� ����Ǵ� ��ų�� ���...
					else
					{
						//					g_Pc.m_PcTable[aTableIndex].char_info.theMaxSMP = aEffectValue;
					}
				}
				else if( aTargetType == n_NPC )
				{
					// �� �Ǵ� ���� NPC �鿡�� ����Ǵ� ��ų�� ���...
					//				g_Map.m_NpcTable[aTableIndex].theSMP = aEffectValue;
				}
			}
		}
		break;
/*	case n_PowerAdd:					// �Ŀ�
		{
			if( aTargetType == n_PC )
			{
				// ���ΰ����� ����Ǵ� ��ų�� ���...
				if( g_Pc.m_MainPC.lUnique == aTargetID )
				{
					nRui->thePcParam.Power = aEffectValue;
				}
				// �ٸ� �������� ����Ǵ� ��ų�� ���...
				else
				{
					//					g_Pc.m_PcTable[aTableIndex].char_info.theMaxSMP = aEffectValue;
				}
			}
			else if( aTargetType == n_NPC )
			{
				// �� �Ǵ� ���� NPC �鿡�� ����Ǵ� ��ų�� ���...
				//				g_Map.m_NpcTable[aTableIndex].theSMP = aEffectValue;
			}
		}
		break;
	case n_DefenseAdd:					// ���
		{
			if( aTargetType == n_PC )
			{
				// ���ΰ����� ����Ǵ� ��ų�� ���...
				if( g_Pc.m_MainPC.lUnique == aTargetID )
				{
					nRui->thePcParam.Defense = aEffectValue;
				}
				// �ٸ� �������� ����Ǵ� ��ų�� ���...
				else
				{
					//					g_Pc.m_PcTable[aTableIndex].char_info.theMaxSMP = aEffectValue;
				}
			}
			else if( aTargetType == n_NPC )
			{
				// �� �Ǵ� ���� NPC �鿡�� ����Ǵ� ��ų�� ���...
				//				g_Map.m_NpcTable[aTableIndex].theSMP = aEffectValue;
			}
		}
		break;
	case n_SympathyAdd:					// ����
		{
			if( aTargetType == n_PC )
			{
				// ���ΰ����� ����Ǵ� ��ų�� ���...
				if( g_Pc.m_MainPC.lUnique == aTargetID )
				{
					nRui->thePcParam.Sympathy = aEffectValue;
				}
				// �ٸ� �������� ����Ǵ� ��ų�� ���...
				else
				{
					//					g_Pc.m_PcTable[aTableIndex].char_info.theMaxSMP = aEffectValue;
				}
			}
			else if( aTargetType == n_NPC )
			{
				// �� �Ǵ� ���� NPC �鿡�� ����Ǵ� ��ų�� ���...
				//				g_Map.m_NpcTable[aTableIndex].theSMP = aEffectValue;
			}
		}
		break;
	case n_ResistAdd:					// ����
		{
			if( aTargetType == n_PC )
			{
				// ���ΰ����� ����Ǵ� ��ų�� ���...
				if( g_Pc.m_MainPC.lUnique == aTargetID )
				{
					nRui->thePcParam.Resist = aEffectValue;
				}
				// �ٸ� �������� ����Ǵ� ��ų�� ���...
				else
				{
					//					g_Pc.m_PcTable[aTableIndex].char_info.theMaxSMP = aEffectValue;
				}
			}
			else if( aTargetType == n_NPC )
			{
				// �� �Ǵ� ���� NPC �鿡�� ����Ǵ� ��ų�� ���...
				//				g_Map.m_NpcTable[aTableIndex].theSMP = aEffectValue;
			}
		}
		break;
	case n_FocusAdd:					// ����
		{
			if( aTargetType == n_PC )
			{
				// ���ΰ����� ����Ǵ� ��ų�� ���...
				if( g_Pc.m_MainPC.lUnique == aTargetID )
				{
					nRui->thePcParam.Focus = aEffectValue;
				}
				// �ٸ� �������� ����Ǵ� ��ų�� ���...
				else
				{
					//					g_Pc.m_PcTable[aTableIndex].char_info.theMaxSMP = aEffectValue;
				}
			}
			else if( aTargetType == n_NPC )
			{
				// �� �Ǵ� ���� NPC �鿡�� ����Ǵ� ��ų�� ���...
				//				g_Map.m_NpcTable[aTableIndex].theSMP = aEffectValue;
			}
		}
		break;
	case n_ReactionAdd:					// ����
		{
			if( aTargetType == n_PC )
			{
				// ���ΰ����� ����Ǵ� ��ų�� ���...
				if( g_Pc.m_MainPC.lUnique == aTargetID )
				{
					nRui->thePcParam.Reaction = aEffectValue;
				}
				// �ٸ� �������� ����Ǵ� ��ų�� ���...
				else
				{
					//					g_Pc.m_PcTable[aTableIndex].char_info.theMaxSMP = aEffectValue;
				}
			}
			else if( aTargetType == n_NPC )
			{
				// �� �Ǵ� ���� NPC �鿡�� ����Ǵ� ��ų�� ���...
				//				g_Map.m_NpcTable[aTableIndex].theSMP = aEffectValue;
			}
		}
		break;
	case n_CharismaAdd:					// ī������, �ŷ�
		{
			if( aTargetType == n_PC )
			{
				// ���ΰ����� ����Ǵ� ��ų�� ���...
				if( g_Pc.m_MainPC.lUnique == aTargetID )
				{
					nRui->thePcParam.Charisma = aEffectValue;
				}
				// �ٸ� �������� ����Ǵ� ��ų�� ���...
				else
				{
					//					g_Pc.m_PcTable[aTableIndex].char_info.theMaxSMP = aEffectValue;
				}
			}
			else if( aTargetType == n_NPC )
			{
				// �� �Ǵ� ���� NPC �鿡�� ����Ǵ� ��ų�� ���...
				//				g_Map.m_NpcTable[aTableIndex].theSMP = aEffectValue;
			}
		}
		break;
*/
	case n_AttackBlowAdd:			// ���� �ʻ�� 
//	case n_AttackBlowMultiply:
//	case n_AttackBlowPercent:
	case n_MagicBlowAdd:			// ���� �ʻ�� 
//	case n_MagicBlowMultiply:
//	case n_MagicBlowPercent:
		{
#ifdef ROCKCLIENT_DEV
			RLG1( "=========================UseSkillEffectValueMgr n_AttackBlow = %d", aEffectValue );
#endif
			// �������� �������ֱ� ��������...�ӽ� �۾� 
			if( aEffectValue >= c_DamageFlag_Subtract )
			{
				aEffectValue -= c_DamageFlag_Subtract;
				aEffectValue *= -1;
			}

			if( aTargetType == n_PC )
			{
				// ���ΰ����� ����Ǵ� ��ų�� ���...
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
						///-- by simwoosung ���� �׾����� �ʴ°�쿡�� �����Ų��.
						if( !(g_Pc.GetPlayer()->IsRealDie()) )
						{
							nRui->thePcParam.Stat1.theHP -= aEffectValue;
							//PRINT_DLG_LOG("�� ��ų�� damage - %d theHP - %d ���� ��ų�ڵ� - %d", aEffectValue, 
							//		           nRui->thePcParam.Stat1.theHP, aSkillCode);
							g_Pc.GetPlayer()->m_lPreTime = g_nowTime;
							///SAFE_DELETE_ARRAY( target_list );
						}					
					}
				}
				// �ٸ� �������� ����Ǵ� ��ų�� ���...
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
						///-- by simwoosung ���� �׾����� �ʴ°�쿡�� �����Ų��.
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
						//--PRINT_DLG_LOG("���HP �����ƾ���� ����");
					}			
					
					///--JAPAN_BUG_MODIFY
					///-- by simwoosung ���� �׾����� �ʴ°�쿡�� �����Ų��.
					if( !(g_Map.m_Npc[aTableIndex].IsRealDie()) )
					{
						g_Map.m_NpcTable[aTableIndex].theHP -= aEffectValue;
						g_Map.m_Npc[aTableIndex].m_lPreTime = g_nowTime;

						if( NonPlayerInfoWnd.thePickedNpcID != -1 && 
							NonPlayerInfoWnd.thePickedNpcID == aTableIndex )
						{
							///--PRINT_DLG_LOG("���HP ���� Damage = %d  theRealHP = %d theHP = %d",
							///--			   aEffectValue, g_Map.m_NpcTable[aTableIndex].theRealHP, 
							///--			   g_Map.m_NpcTable[aTableIndex].theHP );
						}	
					}
				}
			}
		}
		break;
	case n_LuckAdd:						// ���
//	case n_LuckMultiply:		
//	case n_LuckPercent:		
		{
			if( aTargetType == n_PC )
			{
				// ���ΰ����� ����Ǵ� ��ų�� ���...
				if( g_Pc.m_MainPC.lUnique == aTargetID )
				{
					nRui->thePcParam.Luck = (char)aEffectValue;
				}
				// �ٸ� �������� ����Ǵ� ��ų�� ���...
				else
				{
					// g_Pc.m_PcTable[aTableIndex].char_info.theMaxSMP = aEffectValue;
				}
			}
			else if( aTargetType == n_NPC )
			{
				// �� �Ǵ� ���� NPC �鿡�� ����Ǵ� ��ų�� ���...
				//	g_Map.m_NpcTable[aTableIndex].theSMP = aEffectValue;
			}
		}
		break;
	case n_CommonPowerAdd:
		{
			if( aTargetType == n_PC )
			{
				// ���ΰ����� ����Ǵ� ��ų�� ���...
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
				// �ٸ� �������� ����Ǵ� ��ų�� ���...
				else
				{
					//					g_Pc.m_PcTable[aTableIndex].char_info.theMaxSMP = aEffectValue;
				}
			}
			else if( aTargetType == n_NPC )
			{
				// �� �Ǵ� ���� NPC �鿡�� ����Ǵ� ��ų�� ���...
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
				// ��t1�ΰ����� ����Ǵ� ��ų�� ���...
				if( g_Pc.m_MainPC.lUnique == aTargetID )
				{
					nRui->thePcParam.MaxHP = aEffectValue;
				}
				// �ٸ� �������� ����Ǵ� ��ų�� ���...
				else
				{
					g_Pc.m_PcTable[aTableIndex].char_info.theMaxHP = aEffectValue;
				}
			}
			else if( aTargetType == n_NPC )
			{
				// �� �Ǵ� ���� NPC �鿡�� ����Ǵ� ��ų�� ���...
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
				// ��t1�ΰ����� ����Ǵ� ��ų�� ���...
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
				// �ٸ� �������� ����Ǵ� ��ų�� ���...
				else
				{
					g_Pc.m_PcTable[aTableIndex].char_info.theMaxSMP = aEffectValue;
				}
			}
			else if( aTargetType == n_NPC )
			{
				// �� �Ǵ� ���� NPC �鿡�� ����Ǵ� ��ų�� ���...
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
	
	// ���� ��ų�� ���� ȿ��ġ ���� 
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
				// ���ΰ����� ����Ǵ� ��ų�� ���...
				if( g_Pc.m_MainPC.lUnique == aTargetID )
				{
					nRui->thePcParam.MaxHP = aEffectValue; 

//					if( nRui->thePcParam.Stat1.theHP > nRui->thePcParam.MaxHP )
//					{
//						nRui->thePcParam.Stat1.theHP = nRui->thePcParam.MaxHP;
//					}					
				}
				// �ٸ� �������� ����Ǵ� ��ų�� ���...
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
				// �� �Ǵ� ���� NPC �鿡�� ����Ǵ� ��ų�� ���...
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
				// ���ΰ����� ����Ǵ� ��ų�� ���...
				if( g_Pc.m_MainPC.lUnique == aTargetID )
				{
					nRui->thePcParam.MaxSP = aEffectValue; 

//					if( nRui->thePcParam.Stat1.theSP > nRui->thePcParam.MaxSP )
//					{
//						nRui->thePcParam.Stat1.theSP = nRui->thePcParam.MaxSP;
//					}	
				}
				// �ٸ� �������� ����Ǵ� ��ų�� ���...
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
				// �� �Ǵ� ���� NPC �鿡�� ����Ǵ� ��ų�� ���...
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
				// ���ΰ����� ����Ǵ� ��ų�� ���...
				if( g_Pc.m_MainPC.lUnique == aTargetID )
				{
					nRui->thePcParam.MaxMP = aEffectValue; 

//					if( nRui->thePcParam.Stat1.theMP > nRui->thePcParam.MaxMP )
//					{
//						nRui->thePcParam.Stat1.theMP = nRui->thePcParam.MaxMP;
//					}	
				}
				// �ٸ� �������� ����Ǵ� ��ų�� ���...
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
				// �� �Ǵ� ���� NPC �鿡�� ����Ǵ� ��ų�� ���...
				g_Map.m_NpcTable[aTableIndex].theSMP = aEffectValue;
			}
		}
		break;

	case n_Scroll_AttackPow:				// ��������(�ּ�, �ִ�ġ ��� ����)
		{
			if( aTargetType == n_PC )
			{
				// ���ΰ����� ����Ǵ� ��ų�� ���...
				if( g_Pc.m_MainPC.lUnique == aTargetID )
				{
					nRui->thePcParam.Stat2.theMinAttackPower = aEffectValue;
					nRui->thePcParam.Stat2.theMaxAttackPower = aEffectMaxValue;
				}
			}
		}
		break;

	case n_Scroll_MagicPow:				// ��������
		{
			if( aTargetType == n_PC )
			{
				// ���ΰ����� ����Ǵ� ��ų�� ���...
				if( g_Pc.m_MainPC.lUnique == aTargetID )
				{
					nRui->thePcParam.Stat2.theMinMagicPower = aEffectValue;
					nRui->thePcParam.Stat2.theMaxMagicPower = aEffectMaxValue;
				}
			}
		}
		break;
	
	case n_Scroll_AttackDef:			// �������
		{
			if( aTargetType == n_PC )
			{
				// ���ΰ����� ����Ǵ� ��ų�� ���...
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
				// ���ΰ����� ����Ǵ� ��ų�� ���...
				if( g_Pc.m_MainPC.lUnique == aTargetID )
				{
					nRui->thePcParam.Stat2.theMagicRes = aEffectValue;					
				}
			}
		}
		break;

	case n_Scroll_AttackSpeed:				// ���ݼӵ�(����)
		{
			if( aTargetType == n_PC )
			{
				// ���ΰ����� ����Ǵ� ��ų�� ���...
				if( g_Pc.m_MainPC.lUnique == aTargetID )
				{
					nRui->thePcParam.Stat2.theAttackSpeed = aEffectValue;
					g_Pc.GetPlayer()->m_Weapon_DelayTime = aEffectValue * 100;		// 100ms������ �����ֹǷ� ������ �ʿ� 					
				}
			}
		}
		break;

	case n_Scroll_MoveSpeed:				// �̵��ӵ�
		{
			if( aTargetType == n_PC )
			{
				// ���ΰ����� ����Ǵ� ��ų�� ���...
				if( g_Pc.m_MainPC.lUnique == aTargetID )
				{
					nRui->thePcParam.Stat2.theMoveSpeed = aEffectValue;					
					g_Pc.GetPlayer()->m_ani_percent = ( (float)aEffectValue / c_Pc_MoveSpeed );					
					g_Pc.GetPlayer()->m_move_percent = c_Pc_MoveSpeed / ( c_Pc_MoveSpeed * g_Pc.GetPlayer()->m_ani_percent );					
				}
				// �ٸ� �������� ����Ǵ� ��ų�� ���...
				else
				{
					g_Pc.m_Pc[aTableIndex].m_ani_percent = ( (float)aEffectValue / c_Pc_MoveSpeed );					
					g_Pc.m_Pc[aTableIndex].m_move_percent = c_Pc_MoveSpeed / ( c_Pc_MoveSpeed * g_Pc.m_Pc[aTableIndex].m_ani_percent );					
				}
			}
			else if( aTargetType == n_NPC )
			{
				// �� �Ǵ� ���� NPC �鿡�� ����Ǵ� ��ų�� ���...
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
				// ���ΰ����� ����Ǵ� ��ų�� ���...
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
				// ���ΰ����� ����Ǵ� ��ų�� ���...
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
				// ���ΰ����� ����Ǵ� ��ų�� ���...
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


// �ΰ�ȿ���� ����Ʈ�� �������� ������� ó���ȴ� 
void CSkillSystem::UseSkillTargetInfoMgr_AddEffect( Character* pSrcChr, _STargetInfo aTargetInfo, WORD aSkillCode, BOOL bCreateEffect )
{	

	BYTE addeffect_flag = 0;

	// �ΰ�ȿ�� ó��
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
		///-- ��ü�� ��ü�� �ƴ� �н��� ���
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
			
			// �ΰ�ȿ���� ���� ��ų ������ ǥ�� ����� �ϴµ�...���� �׷����� �ʾҴٸ� �����̶�� ó���Ѵ� 
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
								
								// ���Ӽ� ��ų ����Ʈ �߰� [����]
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
						///-- �ռ� ��ų�� �⺻ȿ�� ��ü�� ������ ���̽�
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
								
								// ���Ӽ� ��ų ����Ʈ �߰� [����]
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
						///-- �ռ� ��ų�� �⺻ȿ�� ��ü�� ������ ���̽�
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
							
							// ���Ӽ� ��ų ����Ʈ �߰� [����]
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
					///-- �ռ� ��ų�� �⺻ȿ�� ��ü�� ������ ���̽�
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
				
		///-- By simwoosung �ΰ�ȿ���� ���� �����ɶ��� �����Ѵ�. 
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

	// �ΰ�ȿ�� �ߵ��� ���̸� ������ ȿ�� ���� 
	if( aIsStart )
	{
		if( aTargetType == n_PC )
		{
			// ���ΰ����� ����Ǵ� ��ų�� ���...
			if( g_Pc.m_MainPC.lUnique == aTargetID )
			{
				///-- ��ü�� ��ü�� �ƴ� �н��� ���
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
					{	//������ �ൿ�� ������ �ɱ� ... 

						g_Pc.GetPlayer()->theAddEffect_State[idx] = g_Pc_Manager.theAddEffectInfoTable[skill_Info->theAddEffect - 1].theAddeffect[idx];
						if(n_TypeNotMove == idx && (skill_Info->theAddEffect == n_AddEff_Panic)) 
						{
							g_Pc.GetPlayer()->theAddEffect_State[idx] = 1;
						}
		
					}
				}

				g_pAddEffectMsg->UpdateEffectMessage(g_Pc.GetPlayer()->theAddEffect_State);
				
				// ���� Ÿ���� ��� Ÿ���� ������Ų�� 
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
			// �ٸ� �������� ����Ǵ� ��ų�� ���...
			else
			{		
			}
		}
		else if( aTargetType == n_NPC )
		{
		}		
	}
	// �ΰ�ȿ�� ����� ���̸� �ٸ� ��ų�� ���߾ ȿ�� ����
	else
	{
		if( aTargetType == n_PC )
		{
			// ���ΰ����� ����Ǵ� ��ų�� ���...
			if( g_Pc.m_MainPC.lUnique == aTargetID )
			{
				//�ΰ� ȿ�� �ʱ�ȭ 
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
								//���� �������� ���ؼ�...
								//�̵��Ұ� �� �����ϸ� , �������� �̵���ü�� ���ƹ����� ������ .. 
								//�д� �� �̵��Ұ��� �߰��� �־���� �Ѵ� . 
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
			// �ٸ� �������� ����Ǵ� ��ų�� ���...
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
// ��ų ����Ʈ ���� By wxywxy
//................................................................................................................
void CSkillSystem::CreateSkill( Character *pSrc, Character *pDest, int skilledIdx, SSkillBaseInfo* pSkillBaseInfo, 
								EVENT_DATA_INFO &underattack_event, EVENT_DATA_INFO& aSkillEvent, _STargetInfo aTargetInfo )
{
	if( NULL == pSkillBaseInfo )
		return;

	// Src �ߵ� ����Ʈ ó�� ( ȿ�� ó�� ���� ����Ʈ�� ���� )
	if( pSkillBaseInfo->theEffectInfo[0].theEffectType == n_ZERO && 
		pSkillBaseInfo->theEffectInfo[1].theEffectType == n_ZERO && 
		pSkillBaseInfo->theEffectInfo[2].theEffectType == n_ZERO && 
		pSkillBaseInfo->theAddEffect == n_ZERO )
	{	
		// Create() -> Ing() -> End()
		g_Particle.m_EffectManager.CreateKeepupSkill( pSkillBaseInfo->theSkillCode , pSrc , NULL , TRUE );					
	}	
	// ���� ���� ��ų��
	else if( pSkillBaseInfo->theActionKeepupTime == TRUE )
	{
		// Create() <1>-> Ing() -> End() -> EffectTargetHit() : �ΰ� ȿ�� �ɸ��� �ʴ´� �������� ����
		g_Particle.m_EffectManager.Create( (EFFECT_ID)pSkillBaseInfo->theKeepupEffect , 
			pSrc                 , pDest ,
			underattack_event.type  , aSkillEvent.critical , aSkillEvent.count, &aTargetInfo , 
			aSkillEvent.lStartTime, aSkillEvent.skill_unique );
	}
	// Src �ߵ� ����Ʈ ó�� + Dest �ߵ� ����Ʈ ó�� + Dest ���� ����Ʈ ó�� + Dest �̺�Ʈ ó�� + Dest ���� ������ ó�� + Dest ȿ�� ó�� 
	else if( pSkillBaseInfo->theActionEffect != 0 )
	{
		// Create() <1>-> Ing() -> End() -> EffectTargetHit() -> UseSkillTargetInfoMgr & UseSkillTargetInfoMgr_AddEffect
		//			<2>-> EffectTargetHit() -> UseSkillTargetInfoMgr & UseSkillTargetInfoMgr_AddEffect -> Ing() -> End() 
		g_Particle.m_EffectManager.Create( (EFFECT_ID)pSkillBaseInfo->theActionEffect , 
		pSrc , pDest , underattack_event.type  , aSkillEvent.critical , aSkillEvent.count, 
		&aTargetInfo, aSkillEvent.lStartTime );
	}
	// Dest ���� ����Ʈ ó�� + Dest ���� ������ ó�� + Dest ȿ�� ó�� 
	else
	{
	
		// ������ EffectTargetHit() -> UseSkillTargetInfoMgr & UseSkillTargetInfoMgr_AddEffect
		UseSkillTargetInfoMgr( pSrc, aTargetInfo, aSkillEvent.count, TRUE );
		UseSkillTargetInfoMgr_AddEffect( pSrc, aTargetInfo, aSkillEvent.count, TRUE );
	}

	return;
}

//---------------------------------------------------------------------------------------------------------
// before : .theHP = .theRealHP;
// after  : .theHP -= Damage;
// => Damage = theRealHP - DrawHP; 
// ���� �޼����� ���ڸ��� ��ų Ÿ�Կ� ���� ������ ����� ���� �� ����, Ŭ���̾�Ʈ���� �� ��ǿ� �°� ����Ʈ �� ȿ���� ó���Ѵ�. 
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
	
	int same_effect_type[c_Max_PerSkill_EffectNum];	//�����ʻ��,�����ʻ�� ���������̸� ù��° �迭�� �����ش� . 
	memset( same_effect_type, 0 , sizeof(int) * c_Max_PerSkill_EffectNum );

	DWORD dDamageTotal = 0;
	BOOL  IsDie = FALSE;	
	
	SSkillBaseInfo* skill_Info = NULL;
	skill_Info = g_Pc_Manager.GetSkillBaseInfoPtr( aSkillCode );	
	
	if( aTargetInfo == NULL || skill_Info == NULL )
		return;
	
	for( i = 0; i < c_Max_PerSkill_EffectNum; ++i )			// ��ų ȿ�� ó�� 3���� ó��  
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
 
		if( keepup_time != 0 && keepup_flag == 0 )		// ���� ��ų ������ ���� ���� ���...���� 
		{			
			///--JAPAN_BUG_MODIFY
			if( !ISSkipExtraSkill( aSkillCode ) )
			{
				switch( effect_type )
				{
					case n_HPAdd:					// ü�� ���� ó�� 
					case n_AttackBlowAdd:			// ���� �ʻ�� 
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
		case n_HPAdd:					// ü�� ���� ó�� 
		case n_AttackBlowAdd:			// ���� �ʻ�� 
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
								///--PRINT_DLG_LOG("�� ��ų�� damage - %d theRealHP - %d ���� ��ų�ڵ� - %d ", 
								///--	damage, g_Pc.m_MainPC.char_info.theRealHP, aSkillCode );
								g_Pc.GetPlayer()->m_lPreTime = g_nowTime;								
							}
							else
							{
								///--PRINT_DLG_LOG("�� ��ų HP 0 ����");
							}							

							if( aEffectType == n_PET && g_Pc.GetPlayer()->m_State.nPetState == nPetState_Master )
							{
								//�� ���̸�
								if( aAttackID == g_Map.m_NpcTable[g_Pc.GetPlayer()->m_State.PetIndex].lUnique )
								{
									if( damage < 0 )
									{
										SystemPrint( n_DarkGreen, n_SystemMsg, 
										G_STRING_CHAR( IDS_PET_RECALL_MASTER_HP ), -damage );		
									}
								}	
							}
							
							// �������� ���� DWORD => long �۾� ���̹Ƿ� �ӽ÷� 1000000000 �� ���� ���ؼ� ����Ѵ�. 							
							// ���̳ʽ� �������� �� ȸ���� ���Ѵ�. (HP ���)
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
									///--PRINT_DLG_LOG("Ÿ ���� ��ų HP 0 ����");
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
									///--PRINT_DLG_LOG("���HP ���� Damage = %d  theRealHP = %d theHP = %d",
									///--			   damage, g_Map.m_NpcTable[skilledIdx].theRealHP, 
									///--			   g_Map.m_NpcTable[skilledIdx].theHP );
								}
							}
							else
							{
								///--PRINT_DLG_LOG("�� ��ų HP 0 ����");
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
		//�� ���̸�
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

	for( i = 0; i < c_Max_PerSkill_EffectNum; ++i )	 // ��ų ȿ�� ó�� 3���� ó��  
	{
		switch( same_effect_type[i] )
		{
			case n_AttackBlowAdd:			// ���� �ʻ�� 
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
	case n_HPAdd:					// ü�� ���� ó�� 
	case n_AttackBlowAdd:			// ���� �ʻ�� 
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
					
					// �������� ���� DWORD => long �۾� ���̹Ƿ� �ӽ÷� 1000000000 �� ���� ���ؼ� ����Ѵ�. 							
					// ���̳ʽ� �������� �� ȸ���� ���Ѵ�. (HP ���)
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
//			DWORD		theAttackedID;		// ���� ��ü
//			DWORD		theAttackedHP;		// ���� ��ü�� ���� HP (������ WORD�� ��ȯ�ؾ� �Ѵ�. )
//			BYTE		theAttackedType:1;	// (1)���� ��ü�� Ÿ�� PC, NPC
//			BYTE		theDieFlag:2;		// (1)n_Success(����) : ���� ���ó���� ���� ������ 
//			BYTE		theCritical:2;		// (1)ũ��Ƽ�� ���� ���� (n_Success/n_Fail)
						
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
// ���̽� ���ΰ� ���� ���ӽð��� ������ �ܹ߼� ��ų ó�� �Ұ͵� ���� by wxywxy
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
		// ���̽� ����
		case 1921:	
		case 1922:	
		case 1923:	
		case 1924:	
		case 1925:	

		// �ݷν� ���̽� ����
		case 5509:
		// �ʷα� ���̽� ����
		case 5529:
			return TRUE;
	}
*/

	return FALSE;
}
