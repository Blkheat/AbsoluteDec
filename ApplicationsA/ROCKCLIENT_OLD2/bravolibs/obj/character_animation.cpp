     #include "RockPCH.h"

#include	<NETWORK\Protocol.h>
#include	<Base\\D3DBase.h>
#include	"..\\RockClient.h"
#include	"..\\quadlist.h"
#include	<Map\Field.h>
#include	"..\\Map.H"
#include	"..\\PC.H"
//#include	"..\\Menu_Interface.h"
#include	"..\\Effect.h"
#include	"ObjBase.H"
#include	<SOUND\Sound.H>


#include "..\\global.h"
#include "..\\..\\rocklibs\\RockInterface\\CArcherBar.h"


/*
n_Attack5_ChantReady,					// �غ� ���� 
n_Attack5_ChantShoot,					// �߻� ����
n_Attack5_ChantGrade1,					// ������ ��â 1�ܰ� (���⺰ ������ ��â �ܰ� ����)
n_Attack5_ChantGrade2,					// ������ ��â 2�ܰ� 
n_Attack5_ChantGrade3,					// ������ ��â 3�ܰ� 
n_Attack5_ChantGrade4,					// ������ ��â 4�ܰ� 
n_Attack5_ChantGrade5,					// ������ ��â 5�ܰ� 
n_Attack6_ChantReady,					// �غ� ���� 
n_Attack6_ChantShoot,					// �߻� ����
n_Attack6_ChantGrade1,					// ������ ��â 1�ܰ� 
n_Attack6_ChantGrade2,					// ������ ��â 2�ܰ� 
n_Attack6_ChantGrade3,					// ������ ��â 3�ܰ� 
n_Attack6_ChantGrade4,					// ������ ��â 4�ܰ� 
n_Attack6_ChantGrade5,					// ������ ��â 5�ܰ� 
*/

//-----------------------------------------------------------------------------
// Desc: �ִϸ��̼� ������
//-----------------------------------------------------------------------------
#define		RUN_FRAME				6				// �޸���
#define		WALK_FRAME				12				// �ȱ�

//-----------------------------------------------------------------------------
// Desc: �ִϸ��̼� Ÿ��
//-----------------------------------------------------------------------------
/*#define		STANDARD_SPEED			30.0f			// ���ֱ�(0.3��)
#define		PCWALK_SPEED			30.0f			// ���� �ȱ�
#define		PCRUN_SPEED				30.0f			// ���� �޸���
#define		NPCWALK_SPEED			30.0f			// NPC �ȱ�
#define		NPCRUN_SPEED			30.0f			// NPC �޸���
#define		HORSERUN_SPEED			30.0f			// ���ӵ�*/



const int c_AttackTime_VFast( 6 );		///-- �ſ� ����
const int c_AttackTime_Fast( 4 );		///-- ��     ��
const int c_AttackTime_LFast( 2 );		///-- �ణ ����
const int c_AttackTime_Normal( 0 );		///-- ��     ��
const int c_AttackTime_LSlow( -2 );		///-- ���� ����
const int c_AttackTime_Slow( -4 );		///-- ��     ��
const int c_AttackTime_VSlow( -6 );		///-- �ſ� ����	


//-----------------------------------------------------------------------------
// Desc: �ִϸ��̼Ǻ����ϱ� ���� ���ϱ� ( By wxywxy )
//-----------------------------------------------------------------------------
int Character::AnimationPlayNoneBlend(int index,int AniFrame)
{	
	int	ani_index = index;
	float ani_time = c_Stand_AniTime;
	

	// �ϴ� ���� ����, �����뿡 ���� �ִϸ��̼� �з� 
	// Time �� 30.0f �� ����, Index �� ������ �ִϸ��̼� ����
	if( m_bPC || (!m_bPC && m_State.nCharacterType == nCharacterTypePlayerNPC ) )		// PC Animation Set
	{		
		if( m_ISApproachWeapon )
		{
			switch( index )
			{
				case n_Stand_Battle9:
					ani_index = n_SI_OneHand_Wait1;
					break;
				case n_Stand_Peace9:
					ani_index = n_SI_OneHand_Wait0;
					break;
				case n_Stand_Select9:
					ani_index = n_SI_OneHand_Wait2;
					break;
				case n_Run9:
				case n_Walk9:
					if( m_State.bWalk == true )
						ani_index = n_SI_OneHand_Walk;
					else
						ani_index = n_SI_OneHand_Run;
					break;
				case n_Suddenly9:
					ani_index = n_SI_OneHand_Suddenly;
					break;	
				case n_Attack9_Basic0:
				case n_Attack9_Basic4:
					ani_index = m_AttackMotion;
					break;
				case n_ReAction_LightDmg9:
					ani_index = n_SI_OneHand_ReAction0;
					break;
				case n_ReAction_StrongDmg9:		
					ani_index = n_SI_OneHand_ReAction1;
					break;
				case n_ReAction_Critical9:
					ani_index = n_SI_OneHand_ReAction2;
					break;
				case n_Die9:
					ani_index = n_SI_OneHand_Die;
					break;	
				default:
					break;
			}
		}
		else
		{
			if( m_pUserInfo->bAttack == false )		// ���� ������ 
			{	
				switch( index )
				{
				case n_Run9:
				case n_Walk9:
					{
						if( m_State.bWalk == true )
						{
							ani_index = n_Walk9;
						}
					}
					break;
				case n_Stand_Peace9:
				case n_Stand_Battle9:
				case n_Suddenly9:
					break;
				case n_Attack9_Basic0:
				case n_Attack9_Basic4:
					{
						// As Weapon Type
						ani_index = SetWeaponAttackMotionIndex( index );
					}
					break;
				default:
					break;
				}
			}
			else									// ���� ����
			{
				switch( index )
				{
				case n_Run9:
				case n_Walk9:
					{
						if( m_State.bWalk == true )
						{
							ani_index = SetWeaponMotionIndex( n_Walk9 );
						}
						else
						{
							ani_index = SetWeaponMotionIndex( index );
						}					
					}
					break;
				
				case n_Stand_Peace9:
				case n_Stand_Battle9:
				case n_Stand_Select9:
				case n_Suddenly9:
					{
						ani_index = SetWeaponMotionIndex( index );
					}
					break;	
					// Attack
				case n_Attack9_Basic0:
				case n_Attack9_Basic4:
					{
						// As Weapon Type
						ani_index = SetWeaponAttackMotionIndex( index );
					}
					break;										
				default:
					break;
				}			
			}
			
			// ReAction < Damage ( normal, strong, critical ), Die >
			switch( index )
			{
			case n_ReAction_LightDmg9:
			case n_ReAction_StrongDmg9:		
			case n_Die9:
				{
					ani_index = SetWeaponMotionIndex( index );
				}
				break;	
			case n_ReAction_Critical9:
				{
					ani_index = SetWeaponMotionIndex( index );
					ani_time = 15.0f;
				}
				break;
			default:
				break;
			}

			//...................................................................................................
			// ��Ÿ ���� ���
			//...................................................................................................
			switch( index )
			{
				case n_Skill_KeepUp_Action:
					{
						ani_index = SetKeepUpAction( index );
					}
					break;
			}
			//...................................................................................................
		}
	}
	else			// NPC Animation Set
	{
		switch( index )
		{
		case n_NPC_Walk0:
			{
				if( m_State.bWalk == false && g_Pc_Manager.GetAniNumber( this, n_NPC_Run0 ) != -1 )
				{
					ani_index = n_NPC_Run0;			
				}
			}	
			break;
		case n_NPC_ReAction2:
			{
				ani_time = 15.0f;
			}
			break;
		default:
			break;
		}
	}

	//-----------------------------------------------------------------------------
	// �ִϸ��̼�ã��
	//-----------------------------------------------------------------------------
//	if( AniSearch( ani_index ) == true )
	{		
/*
		m_step_time	  = 0.0f;
		m_middle_time = 0.0f;
		
		// �ִϸ��̼� ���� ������				
		m_pre_end_frame = m_ani_frame;		
		m_ani_frame	= 0;
		m_ani_index = ani_index;
		m_ani_time = ani_time;
		m_pre_ani_number = m_ani_number;
		m_ani_number = g_Pc_Manager.GetAniNumber( this, ani_index );								
		if( m_pre_end_frame > m_end_frame )
			m_pre_end_frame = m_end_frame;	
		m_blend_frame = 0;
		m_end_frame = g_Pc_Manager.GetEndFrame( m_ani_number ) - 1;		
*/
		m_step_time	  = 0.0f;
		m_middle_time = 0.0f;
		
		// �ִϸ��̼� ���� ������				
		m_pre_end_frame = 0;		
		
		m_ani_frame	= AniFrame;

		m_ani_index = ani_index;
		m_ani_time = ani_time;
		m_ani_number = g_Pc_Manager.GetAniNumber( this, ani_index );								
		m_pre_ani_number = m_ani_number;
		
		if( m_pre_end_frame > m_end_frame )
			m_pre_end_frame = m_end_frame;	

		m_blend_frame = 0;
//		m_end_frame = g_Pc_Manager.GetEndFrame( m_ani_number ) - 1;		
		m_end_frame = g_Pc_Manager.GetEndFrame( this, ani_index ) - 1;				

		if( AniFrame == -1 )
		{
			m_ani_frame   = g_Pc_Manager.GetEndFrame( this, ani_index ) - 4;
			m_blend_frame = 8;
		}

		return 0;
	}
	return -1;
}

//-----------------------------------------------------------------------------
// Desc: �ִϸ��̼Ǻ����ϱ� 
//-----------------------------------------------------------------------------
int Character::AnimationPlay( int index )
{	
	int	ani_index = index;
	float ani_time = c_Stand_AniTime;	

	// �ϴ� ���� ����, �����뿡 ���� �ִϸ��̼� �з� 
	// Time �� 30.0f �� ����, Index �� ������ �ִϸ��̼� ����
	if( m_bPC || ( (!m_bPC) && m_State.nCharacterType == nCharacterTypePlayerNPC ) )		// PC Animation Set
	{		
		if( m_pUserInfo == NULL )
		{
			return -1;
		}
		
		if( m_ISApproachWeapon )
		{
			switch( index )
			{
				case n_Stand_Battle9:
					ani_index = n_SI_OneHand_Wait1;
					break;
				case n_Stand_Peace9:
					ani_index = n_SI_OneHand_Wait0;
					break;
				case n_Stand_Select9:
					ani_index = n_SI_OneHand_Wait2;
					break;
				case n_Run9:
				case n_Walk9:
					if( m_State.bWalk == true )
						ani_index = n_SI_OneHand_Walk;
					else
						ani_index = n_SI_OneHand_Run;
					break;
				case n_Suddenly9:
					ani_index = n_SI_OneHand_Suddenly;
					break;	
				case n_Attack9_Basic0:
				case n_Attack9_Basic4:
					ani_index = m_AttackMotion;
					break;
				case n_ReAction_LightDmg9:
					ani_index = n_SI_OneHand_ReAction0;
					break;
				case n_ReAction_StrongDmg9:		
					ani_index = n_SI_OneHand_ReAction1;
					break;
				case n_ReAction_Critical9:
					ani_index = n_SI_OneHand_ReAction2;
					break;
				case n_Die9:
					ani_index = n_SI_OneHand_Die;
					break;	
				default:
					break;
			}
		}
		else
		{
			if( g_RockClient.GetGameMode() == GMODE_SELECT )
			{
				switch( index )
				{
				case n_Stand_Peace9:
				case n_Stand_Battle9:
					{
						index = n_Stand_Select9;
					}
					break;			
				}		
			}		
			
			if( m_pUserInfo->bAttack == false )		// ���� ������ 
			{	
				switch( index )
				{
				case n_Run9:
				case n_Walk9:
					{
						if( m_State.bWalk == true )
						{
							ani_index = n_Walk9;
						}
						else
						{
							ani_index = n_Run9;
						}
					}
					break;
				case n_Stand_Peace9:
				case n_Stand_Battle9:
				case n_Stand_Select9:
				case n_Suddenly9:
					break;
				case n_Attack9_Basic0:
				case n_Attack9_Basic4:
					{
						// As Weapon Type
						ani_index = SetWeaponAttackMotionIndex( index );
					}
					break;
				default:
					break;
				}
			}
			else									// ���� ����
			{
				switch( index )
				{
				case n_Run9:
				case n_Walk9:
					{
						if( m_State.bWalk == true )
						{
							ani_index = SetWeaponMotionIndex( n_Walk9 );
						}
						else
						{
							ani_index = SetWeaponMotionIndex( n_Run9 );
						}		
					}
					break;
				case n_Stand_Peace9:
				case n_Stand_Battle9:
				case n_Stand_Select9:
				case n_Suddenly9:
					{
						ani_index = SetWeaponMotionIndex( index );
					}
					break;	
					// Attack
				case n_Attack9_Basic0:
				case n_Attack9_Basic4:
					{
						// As Weapon Type
						ani_index = SetWeaponAttackMotionIndex( index );
					}
					break;										
				default:
					break;
				}			
			}
			// ReAction < Damage ( normal, strong, critical ), Die >
			switch( index )
			{
			case n_ReAction_LightDmg9:
			case n_ReAction_StrongDmg9:		
			case n_Die9:
				{
					ani_index = SetWeaponMotionIndex( index );
				}
				break;	
			case n_ReAction_Critical9:
				{
					ani_index = SetWeaponMotionIndex( index );
					ani_time = 15.0f;
				}
				break;
			default:
				break;
			}

			//...................................................................................................
			// ��Ÿ ���� ���
			//...................................................................................................
			switch( index )
			{
				case n_Skill_KeepUp_Action:
					{
						ani_index = SetKeepUpAction( index );
					}
					break;
			}
			//...................................................................................................

		}

	}
	else			// NPC Animation Set
	{
		switch( index )
		{
		case n_NPC_Walk0:
			{
				if( m_State.bWalk == false && g_Pc_Manager.GetAniNumber( this, n_NPC_Run0 ) != -1 )
				{
					ani_index = n_NPC_Run0;			
				}
			}	
			break;
		case n_NPC_ReAction2:
			{
				ani_time = 15.0f;
			}
			break;
		
		//.....................................................................................................
		// NPC ����Ʈ ������ ã�� ������ ����Ʈ�� 14 ������ by wxywxy
		//.....................................................................................................
		case n_NPC_Attack0:// ���� 0		-> �Ϲݰ��� 
		case n_NPC_Attack1:// ���� 1		-> ũ��Ƽ�� ���� 
		case n_NPC_Attack2:// ���� 2		-> ��ų���� 
		case n_NPC_Attack3:// ���� 3
		case n_NPC_Attack4:// ���� 4
		case n_NPC_Attack5:// ���� 5
		case n_NPC_Attack6:// ���� 6
		case n_NPC_Attack7:// ���� 7
		case n_NPC_Attack8:// ���� 8
			{
				int AniIndex = index - n_NPC_Attack0;
				
				///-- Critical Error Check : Array Index
				assert( MAX_NPC_INTABLE > m_nTableIndex);	
				
				DWORD dFileCode = g_Pc_Manager.GetNPCRealFileCode( g_Map.m_NpcTable[ m_nTableIndex ].code );					
				m_NPCImpactFrame = g_Pc_Manager.m_NPCImpactTable.GetFrame( dFileCode , AniIndex );
				m_bNotImpact = false ; 
				
				int Ani_End_Frame = g_Pc_Manager.GetEndFrame( this, ani_index );
				
				if( m_NPCImpactFrame < c_MotionBlendFrame )
				{
					m_NPCImpactFrame = c_MotionBlendFrame;
				}			
				
				// ����Ʈ �������� ã�� ���ϸ� ����Ʈ �Ǵ� ����Ʈ �������� �ص� �����Ӻ��� Ŭ��
				if( m_NPCImpactFrame == -1 || (m_NPCImpactFrame + c_MotionBlendHalfFrame) > Ani_End_Frame )
				{
					m_NPCImpactFrame = Ani_End_Frame - c_MotionBlendHalfFrame;			
				}
			}

			break;
		//.....................................................................................................


		default:
			break;
		}
	}

	//-----------------------------------------------------------------------------
	// �ִϸ��̼�ã��
	//-----------------------------------------------------------------------------
	if( AniSearch( ani_index ) == true )
	{
		m_step_time	  = 0.0f;
		m_middle_time = 0.0f;
		
		// �ִϸ��̼� ���� ������				
		m_pre_end_frame = m_ani_frame;		
		m_ani_frame	= 0;
		m_ani_index = ani_index;

		if( m_curt_event.type == SM_ANI_SQUENCE )
		{
			if( m_bPC )
			{
				// �÷��̾��϶�
				if( this == g_Pc.GetPlayer() )
				{
					m_ani_time = ( c_Stand_AniTime * g_RockClient.m_SceneManager.GetSquencePlayerAniTime() );
				}
				// �ٸ� �÷��̾� �϶�
				else
				{
					m_ani_time = ( c_Stand_AniTime * g_RockClient.m_SceneManager.GetSquenceOtherPlayerAniTime() );
				}
			}
		}
		else
		{
			m_ani_time = ani_time;
		}

		if(m_IsApplyConAniSkill)
		{
			m_ani_time = m_aConAniTime;
		}

		m_pre_ani_number = m_ani_number;
		m_ani_number = g_Pc_Manager.GetAniNumber( this, ani_index );								
		if( m_pre_end_frame > m_end_frame )
			m_pre_end_frame = m_end_frame;	
		m_blend_frame = 0;
//		m_end_frame = g_Pc_Manager.GetEndFrame( m_ani_number ) - 1;	
		m_end_frame = g_Pc_Manager.GetEndFrame( this, ani_index ) - 1;	
//		m_fMaxHeight = -100.0f;
//		m_fMinHeight = 100.0f;
		return 0;
	}
	
	return -1;
}

//-----------------------------------------------------------------------------
// Desc: �ִϸ��̼� �ٲٱ�
//-----------------------------------------------------------------------------
bool Character::AniSearch( unsigned int _nIndex )
{
	if( _nIndex == ANIMATION_NONE )
		return false;

	// �ִϵ���Ÿ ���� ��� ����
	// �Ȱ��� �ִϸ��̼��� ȣ���� ��� ����
	int nAniNumber = g_Pc_Manager.GetAniNumber( this, _nIndex );

	if( m_curt_event.type == SM_ATTACK_NPC || m_curt_event.type == SM_USE_SKILL )
	{
//		DebugPrint("\nSM_ATTACK_NPC AniSearch Index = %d, number = %d\n", _nIndex, nAniNumber);

		if( nAniNumber != -1 )
			return true;
	}
	else
	{
		if( nAniNumber != -1 && nAniNumber != m_ani_number )
			return true;
	}
	return false;
}

//-----------------------------------------------------------------------------
// Desc: ������ ����
//-----------------------------------------------------------------------------
int Character::GetRunFrame()
{	
	WORD			runframe = 0;

	// PC
	if( m_bPC == true )
	{
		// �ȱ� ���
		if( m_State.bWalk == true )
		{
			runframe = WALK_FRAME;
		}
		else
		{
			// MoveSpeed = 1Cell ������ �� �ɸ� �ð�;
			// runframe = MoveSpeed / 30ms;
			// MoveSpeed = 180; 
//			runframe = ( c_Pc_MoveSpeed * m_ani_percent ) / c_MoveTime; 
//			runframe = c_Pc_MoveSpeed / ( c_MoveTime * m_move_percent ); 		
			runframe = RUN_FRAME;		
		}
	}
	else if(m_State.nPetState == nPetState_Pet)
	{
		// �ȱ� ���
		if( m_State.bWalk == true )
		{
			runframe = WALK_FRAME;
		}
		else
		{
			runframe = RUN_FRAME;		
		}
	}
#ifdef NPC_MOVE_TO_SKILL
	else if(m_State.nActiveState == nActiveTypeMove)
	{
		// �ȱ� ���
		if( m_State.bWalk == true )
		{
			runframe = WALK_FRAME;
		}
		else
		{
			runframe = RUN_FRAME;		
		}
	}
#endif 
	// NPC
	else
	{
		// ���̺� �ִ� ������ �̵��ӵ��� �°� ������ 
		SNPCBaseInfo*	npc_Info = NULL;
		
		///-- Critical Error Check : Array Index
		assert( MAX_NPC_INTABLE > m_nTableIndex);	
		
		npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( g_Map.m_NpcTable[m_nTableIndex].code ); 
		if( npc_Info->theMoveSpeed )
		{
			runframe = ( npc_Info->theMoveSpeed * 10 ) / c_MoveTime;
		}
		else
		{
			runframe = WALK_FRAME;
		}
//		runframe = ( ( npc_Info->theMoveSpeed * 10 ) * m_ani_percent ) / c_MoveTime;	
//		runframe = ( npc_Info->theMoveSpeed * 10 ) / ( c_MoveTime * m_move_percent );	
//		runframe = WALK_FRAME;

		// �ٴ� ����� �� ���������� ó�� 
		if( m_State.bWalk == false )
		{
			runframe = runframe * 0.5f;
		}

		// �ִϸ��̼��ε���
/*		int ani_number;
		//if( m_State.bWalk == false && m_pAniInfo[NPC_WALK1].ani_number != -1 )
		if( m_State.bWalk == false && g_Pc_Manager.GetAniNumber( this, NPC_WALK1 ) != -1  )
			//ani_number = g_Pc_Manager.m_ani_count+m_pAniInfo[NPC_WALK1].ani_number;
			ani_number = g_Pc_Manager.GetAniNumber( this, NPC_WALK1 );
		else			
			//ani_number = g_Pc_Manager.m_ani_count+m_pAniInfo[NPC_WALK0].ani_number;
			ani_number = g_Pc_Manager.GetAniNumber( this, NPC_WALK0 );
		
		// �ȱ�
		if( m_State.bWalk == true )
			return g_Pc_Manager.GetEndFrame( ani_number );
		// �ٱ�
		else
			return g_Pc_Manager.GetEndFrame( ani_number )/2;
*/
	}	
	return runframe;
}

//-----------------------------------------------------------------------------
// Desc: ���� �÷���
//-----------------------------------------------------------------------------
void Character::SoundPlay()
{ 	
	int ISLeftFoot = TRUE;
	
	if( m_CharAlpha.GetState() == CHAR_NONE_DISPLAY )
		return;
/*
	//....................................................................................................
	// ���� �׽�Ʈ
	//....................................................................................................
	if( ( m_ani_frame == 6 ) && m_bPC == FALSE )
	{
		if( g_Map.m_NpcTable[ m_nTableIndex ].code == 5005 )
		{
			g_Sound.Play( 56 , m_matChr._41 , m_matChr._42 , m_matChr._43 );
		}
	}
*/
	static int  AttrIndex   = -1;
	static BYTE MapAttrType =  0;
	UINT uMoveEff = -1;

	AttrIndex   = GetAttrIndex();

	if( -1 == AttrIndex )
		return;

	MapAttrType = g_Map.GetAttrType( m_Mov.curt.y , m_Mov.curt.x );
	BYTE aAttrType = MapAttrType;

	DWORD dNowType = m_curt_event.type;

	if( !m_bPC && m_State.nRideState == nRideState_Ride )
	{
		SNPCBaseInfo* npc_Info = NULL;
		DWORD dNPCCode = g_Map.m_NpcTable[ m_nTableIndex ].code;
		npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( dNPCCode );
		
		if( npc_Info->theTargetRange != 0 )
		{
			if( m_State.TransIndex == MAX_USER_INTABLE )
			{
				dNowType = g_Pc.GetPlayer()->m_curt_event.type;
			}
			else
			{
				dNowType = g_Pc.m_Pc[m_State.TransIndex].m_curt_event.type;
			}

			if( dNowType != SM_MOVE_BROADCAST )
			{
				dNowType = 0;
			}
		}
	}

	if( g_RockClient.GetGameMode() == GMODE_NORMAL && dNowType != NULL )
	{		
		//-----------------------------------------------------------------------------		
		// Desc: ���� �÷��� �Ǵ� Ű(������)���� �����ȣã��
		//
		// ����ĳ���ʹ� �߰����尪�� ���� �˻��ؾ��Ѵ�
		//
		// Copyright (c) 2003.11.13 By KStaR79
		//-----------------------------------------------------------------------------
		int key1 = -1, key2 = -1, sound1 = -1, sound2 = -1;		

		key1	= g_Pc_Manager.GetSoundkey( this, m_ani_index );
		sound1	= g_Pc_Manager.GetSoundNumber( this, m_ani_index );
		
		if( m_bPC == true )
		{			
			key2	= g_Pc_Manager.m_PcAddSound[AttrIndex][m_ani_index].sound_key;
			sound2	= g_Pc_Manager.m_PcAddSound[AttrIndex][m_ani_index].sound_number;
		}

		int nEndFrame = 0;		

		BOOL bRideHorse = FALSE;
		BOOL bIsRideUnit = FALSE;
		int nRideType = 0;

		//-----------------------------------------------------------------------------
		// ���� Ÿ�Կ� ���� ó��
		//-----------------------------------------------------------------------------
		switch( dNowType )
		{		
			case SM_ACT:
			{
				//�չ� ��� .. 
				if(m_ani_index  == n_Ride_Sudden2)
				{

					if( m_ani_frame == ((m_end_frame+1)/3)+key2 )
					{
						key2 = m_ani_frame;
						m_bSoundPlay2 = FALSE;
					}
					sound2 = 297;
				}
				break;
			}
			case SM_MOVE_BROADCAST:	// �̵���
			{
				//-----------------------------------------------------------------------------
				// �ٰ��������� �߰������ӿ����� ����ؾ� �Ѵ�
				//-----------------------------------------------------------------------------
				//DebugPrint( "m_Ani_Frame %d ( %d - %d ) \n" , m_ani_frame , key1 , ((m_end_frame+1)/2)+key1 );
			
				switch( MapAttrType )
				{
					case MATYPE_WATER:
						{
							if( ( m_ani_frame % 4 ) )
								g_Particle.m_EffectManager.Create( EF_RUN_WAVE  , this , NULL );
						}
						break;
				}

				//���� �ӵ��� Ʋ���� ���� ���� �ؾ���  dongs 
				if(m_State.bRide == true)
				{
					bRideHorse = TRUE;
					
					SNPCBaseInfo* npc_Info = NULL;
					DWORD dNPCCode = g_Map.m_NpcTable[ m_State.TransIndex ].code;
					npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( dNPCCode );

					if( npc_Info )
					{
						if( npc_Info->theTargetRange == 0 )
						{
							nRideType = 0;
							
							if( m_ani_frame == ((m_end_frame+1)/3)+key1 )
							{
								key1 = m_ani_frame;
								m_bSoundPlay1 = FALSE;
								
								ISLeftFoot = FALSE;
							}
						}
						else 
						{
							return;
						}
					}								
				}
				else if( !m_bPC && m_State.nRideState == nRideState_Ride )
				{
					SNPCBaseInfo* npc_Info = NULL;
					DWORD dNPCCode = g_Map.m_NpcTable[ m_nTableIndex ].code;
					npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( dNPCCode );					
					
					bIsRideUnit = TRUE;
					
					if( npc_Info->theTargetRange == 1 )
					{
						nRideType = 1;
						key1 = 9;

						if( m_ani_frame == ((m_end_frame+1)/2)+key1 )
						{
							key1 = m_ani_frame;
							m_bSoundPlay1 = FALSE;								
							ISLeftFoot = FALSE;
						}						
					}
					else if( npc_Info->theTargetRange == 2 )
					{
						nRideType = 2;
						key1 = 8;

						if( m_ani_frame == ((m_end_frame+1)/2)+key1 )
						{
							key1 = m_ani_frame;
							m_bSoundPlay1 = FALSE;								
							ISLeftFoot = FALSE;
						}
					}
				}
				else 
				{
					if( m_State.bWalk == false && m_ani_frame == ((m_end_frame+1)/2)+key1 )	
					{
						if( m_ani_frame == ((m_end_frame+1)/2)+key1 )
						{
							key1 = m_ani_frame;
							m_bSoundPlay1 = FALSE;
							
							ISLeftFoot = FALSE;
						}						
					}
				}
			
				//-----------------------------------------------------------------------------
				// Desc: ����Ÿ�� �Ӽ��� ���� ���� ���
				//
				// 1. ���� ����ٴϱ⶧���� �����Ѵ�
				// 2. �ȱ�, �ٱ⿡ ���� �����ؾ� �Ѵ�
				//				
				// Copyright (c) 2003.11.13 By KStaR79
				//-----------------------------------------------------------------------------
				if( m_bPC == true && GetAttrIndex() != UNION_FAIRY )
				{
					// ��Ÿ�� �̵�
					if( bRideHorse )
					{	
						if(nRideType == 0)
						{
							sound1 = 296;
						}					
					} 
					// �Ϲ��̵�
					else
					{
						switch( MapAttrType )
						{
						case MATYPE_LAND:	// ��
							sound1 = 0;
							break;
						case MATYPE_GRASS:	// Ǯ
							sound1 = 1;
							break;
						case MATYPE_WATER:	// ��
							sound1 = 2;
							break;
						case MATYPE_STONE:	// ��
							sound1 = 54;
							break;
						case MATYPE_WOOD:	// ����
							sound1 = 77;
							break;
						case MATYPE_SNOW:	// ��
							sound1 = 128;
							break;

						case MATYPE_SAND:   // ��
							sound1 = 129;
							break;

						}
					}
				}
				else
				{
					///-- �Ŵ�� �ϰ��
					if( m_State.nCharacterType == nCharacterTypeBigMoster )
					{
						SNPCBaseInfo* npc_Info = NULL;
						npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( g_Map.m_NpcTable[m_nTableIndex].code );
						if( npc_Info )
						{
							if( (m_ani_frame % 10) == 0 )
							{
								D3DXMATRIX matBone;			
								GetBoneLinkTM(6 , matBone , FALSE);
								D3DXVECTOR3	Pos1( matBone._41, 0.0f, matBone._43 );	
								Pos1.y = g_Map.PHF_GetHeight( Pos1.x , Pos1.z );
								g_Particle.DisplayUpdate( PT_WALK_DUST , Pos1, -1, 3.0f );
								
								GetBoneLinkTM(7 , matBone , FALSE);
								D3DXVECTOR3	Pos2( matBone._41, 0.0f, matBone._43 );	
								Pos2.y = g_Map.PHF_GetHeight( Pos2.x , Pos2.z );
								g_Particle.DisplayUpdate( PT_WALK_DUST , Pos2, -1, 3.0f );
							}
						}
					}
					else if( bIsRideUnit )
					{
						if(nRideType == 1)
						{	
							sound1 = 342;													
						}
						else if(nRideType == 2)
						{
							sound1 = 344;
						}
					}
				}
			}
			break;

		case SM_ATTACK_NPC:		// ������3
			{		
				// �ڱ�ĳ���� -> ���ݿ�Ÿ�� �����ٸ� ���ݻ��� ����ϸ� �ȵȴ�
				if( m_nTableIndex == MAX_USER_INTABLE && g_Pc.GetPlayer()->m_bWait == true )
						sound1 = -1;
			}
			break;
		}

		//-----------------------------------------------------------------------------
		// sound1 ���
		//-----------------------------------------------------------------------------

		BOOL IsSyncKeyFrame = FALSE;


		if( ( !m_bPC || sound1 != -1) && m_ani_frame == key1 && m_bSoundPlay1 == false )
		{
			m_bSoundPlay1 = true;		

			if(sound1 != -1)
			{
				g_Sound.Play( sound1 , m_matChr._41 , m_matChr._42 , m_matChr._43 );
			}

//			if( m_curt_event.type == SM_ATTACK_NPC )			
//				RLG3( "-=##### g_Sound.Play() m_ani_frame = %d, sound1 = %d m_ani_index = %d #####=-", m_ani_frame, sound1, m_ani_index );					

			//-----------------------------------------------------------------------------
			// ��ƼŬ �Ѹ���
			//-----------------------------------------------------------------------------
			switch( m_curt_event.type )
			{
			case SM_MOVE_BROADCAST:
				{
					// ���� ��ƾ ( �ϴ� �ٴ��̸� �ٳ��� By wxywxy )
					if( m_bPC == true && GetAttrIndex() != UNION_FAIRY && !m_State.bRide )
					{
/*
						if( t1g_Map.m_pAttrMap[m_Mov.curt.y][m_Mov.curt.x].byType == MATYPE_WATER )
						{
							D3DXVECTOR3 tempv = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
			
							if( m_nTableIndex == MAX_USER_INTABLE )
								tempv = D3DXVECTOR3( g_Pc.GetPlayer()->GetPosTM()._41, g_Map.m_fBaseHeightWater, g_Pc.GetPlayer()->GetPosTM()._43 );
							else
								tempv = D3DXVECTOR3( g_Pc.m_Pc[m_nTableIndex].GetPosTM()._41, g_Map.m_fBaseHeightWater, g_Pc.m_Pc[m_nTableIndex].GetPosTM()._43 );

							g_Particle.DisplayUpdate( 55, tempv );
						}
						else*/
						{
							// ���� ����Ʈ
							switch( MapAttrType )
							{
							
							case MATYPE_LAND:	// ��
								g_Particle.m_EffectManager.Create( EF_WALK_DUST , this , NULL );
								break;
							case MATYPE_GRASS:	// Ǯ
								g_Particle.m_EffectManager.Create( EF_WALK_DUST , this , NULL );
								break;
							case MATYPE_WATER:	// ��
								/*
								DebugPrint( "Frame %d IS Left %d \n" , m_ani_frame , ISLeftFoot );

								if( ISLeftFoot )
									g_Particle.m_EffectManager.Create( EF_RUN_WAVE  , this , NULL );
								else
									g_Particle.m_EffectManager.Create( EF_WALK_WAVE  , this , NULL );
								*/
								break;
							case MATYPE_STONE:	// ��
								g_Particle.m_EffectManager.Create( EF_WALK_DUST , this , NULL );
								break;
							case MATYPE_WOOD:	// ����
								g_Particle.m_EffectManager.Create( EF_WALK_DUST , this , NULL );
								break;

							case MATYPE_SNOW:	// ��
								{
									float fAngle = D3DXToDegree( GetDirection() );
/*									
									if( fAngle < 0 )
									{
										int Num = ( int ) ( -fAngle / 360.0f ); 
										
										fAngle = fAngle + ( ( Num + 1 ) * 360.0f );
									}
									else if( fAngle >= 360 )
									{
										int Num = ( int ) ( fAngle / 360.0f ); 
										
										fAngle = fAngle - ( Num * 360.0f );
									}
*/									

									D3DXVECTOR3	Pos( GetPosTM()._41 , GetPosTM()._42 , GetPosTM()._43 );
									
									int  Num = ( int ) ( ( ( int ) fAngle ) / 30 );
									
									Num = BB_SNOW0 + Num;
									
									if( ISLeftFoot )
									{
										D3DXMATRIX   matRotY;
										D3DXVECTOR3  vDir( 1.0f , 0.0f , 0.0f );
		
										D3DXMatrixRotationY( &matRotY , GetDirection() );
										D3DXVec3TransformCoord( &vDir , &vDir , &matRotY );

										Pos += vDir * 1.0f; 
									}
									else
									{
										D3DXMATRIX   matRotY;
										D3DXVECTOR3  vDir( -1.0f , 0.0f , 0.0f );
		
										D3DXMatrixRotationY( &matRotY , GetDirection() );
										D3DXVec3TransformCoord( &vDir , &vDir , &matRotY );

										Pos += vDir * 1.0f; 
									}

									g_Billboard.SetManager( 0 , Num , -1 , -1 , Pos );
								}
								break;

							case MATYPE_SAND:   // ��
							{
								D3DXVECTOR3	Pos( GetPosTM()._41 , GetPosTM()._42 , GetPosTM()._43 );
	
								g_Particle.DisplayUpdate( PT_FOOT_SAND , Pos );
							}

								break;

							default:
								g_Particle.m_EffectManager.Create( EF_WALK_DUST , this , NULL );
								break;

							}
						}
					}
					else
					{
						int a = 0;
					}
				}
				break;
			}

		}

		//-----------------------------------------------------------------------------
		// sound2 ���
		//-----------------------------------------------------------------------------
		if( sound2 != -1 && m_ani_frame == key2 && m_bSoundPlay2 == false )
		{
			m_bSoundPlay2 = true;
			
			g_Sound.Play( sound2 , GetPosTM()._41 , 
								   GetPosTM()._42 ,
								   GetPosTM()._43 );

		}		
	}
	else if( g_RockClient.GetGameMode() == GMODE_NORMAL && m_curt_event.type == NULL)
	{
		if( m_ani_frame == ((m_end_frame+1)/2) ||
			m_ani_frame == 1 )	
		{
			switch( MapAttrType )
			{
				
			case MATYPE_LAND:	// ��
				break;
			case MATYPE_GRASS:	// Ǯ
				break;
			case MATYPE_WATER:	// ��
				g_Particle.m_EffectManager.Create( EF_WALK_WAVE  , this , NULL );
				break;
			case MATYPE_STONE:	// ��
				break;
			case MATYPE_WOOD:	// ����
				break;
				
			case MATYPE_SNOW:	// ��
				{
				}
				break;
				
			case MATYPE_SAND:   // ��
				{
				}
				
				break;
				
			default:
				break;
				
			}
		}
		
	}
}


int Character::SetWeaponMotionIndex( int index )
{
	int motion_index = index;
	int subtract_index = 0;
	int RHand_index = m_pUserInfo->ArmorInfo.index[0];
	int LHand_index = m_pUserInfo->ArmorInfo.index[1];	
	int RHand_Code = GetWearItemCode( n_WearSlot_Weapon );
	int LHand_Code = GetWearItemCode( n_WearSlot_Shield );


	SItemBaseInfo* item_Info = NULL;	
		
	if( RHand_index != -1 && LHand_index == -1 )					// ������ ���⸸ ����
	{		
//		item_Info = g_Pc_Manager.GetItemBaseInfoPtr( RHand_Code );	// �и�ġ�� �����տ��� ���� ������...^ ^? 
//		if( item_Info )
//		{
//			if( item_Info->theType == n_Type_TwohandSword )			// ��� ���� ����� (�и�ġ) 
///			{
//				return n_Attack9_Basic0;							// ���� ���� �ִϸ��̼� 
//			}
//		}


/*		item_Info = g_Pc_Manager.GetItemBaseInfoPtr( RHand_Code );
		if( item_Info )
		{
			if( item_Info->theType == n_Type_TwohandSword )			// ��� 
			{
				subtract_index = 7;
			}
			else if( item_Info->theType == n_Type_Sword )			// �Ѽհ� 
			{
				subtract_index = 5;
			}
			else if( item_Info->theType == n_Type_Staff )			// ������ 
			{
				subtract_index = 2;
			}
			else if( item_Info->theType == n_Type_CrystalBall )		// �����
			{
				subtract_index = 1;
			}
		}
*/
		if( g_Pc_Manager.m_pItm[RHand_index]->m_type == 0 )		// ��� 
		{
			subtract_index = 7;
		}
		else if( g_Pc_Manager.m_pItm[RHand_index]->m_type == 2 ) // �Ѽհ� 
		{
			subtract_index = 5;
		}
		else if( g_Pc_Manager.m_pItm[RHand_index]->m_type == 6 ) // ������ 
		{
			subtract_index = 2;
		}
		else if( g_Pc_Manager.m_pItm[RHand_index]->m_type == 7 ) // �����
		{
			subtract_index = 1;
		}
		
		motion_index = index - subtract_index;
	}
	else if( RHand_index == -1 && LHand_index != -1 )			// �޼� ���⸸ ����
	{
/*		item_Info = g_Pc_Manager.GetItemBaseInfoPtr( LHand_Code );
		if( item_Info )
		{
			if( item_Info->theType == n_Type_Bow )					// Ȱ 
			{
				subtract_index = 6;
			}
			else if( item_Info->theType == n_Type_Sword )			// ���� 
			{
				subtract_index = 0;
			}
			else if( item_Info->theType == n_Type_Staff )			// ������ 
			{
				subtract_index = 3;
			}
		}
*/
		if( g_Pc_Manager.m_pItm[LHand_index]->m_type == 1 )			// Ȱ 
		{
			subtract_index = 6;
		}
		else if( g_Pc_Manager.m_pItm[LHand_index]->m_type == 3 )	// ���� 
		{
			subtract_index = 0;			// ���и� ������ ��ȭ��� 
		}
		else if( g_Pc_Manager.m_pItm[LHand_index]->m_type == 5 )	// ������ 
		{
			subtract_index = 3;
		}

		motion_index = index - subtract_index;
	}
	else if( RHand_index != -1 && LHand_index != -1 )				// ��տ� ���� ����
	{
/*	
		item_Info = g_Pc_Manager.GetItemBaseInfoPtr( RHand_Code );
		if( item_Info )
		{
			if( item_Info->theType == n_Type_Sword )				// �Ѽհ�
			{
				item_Info = g_Pc_Manager.GetItemBaseInfoPtr( LHand_Code );
				if( item_Info )
				{
					if( item_Info->theType == n_Type_Shield )		// ����
					{
						subtract_index = 4;
					}
				}
			}
		}
		
*/
		if( g_Pc_Manager.m_pItm[RHand_index]->m_type == 2 && 
			g_Pc_Manager.m_pItm[LHand_index]->m_type == 3 )		// �Ѽհ� + ����
		{
			subtract_index = 4;
		}
		motion_index = index - subtract_index;
	}
	
	return motion_index;
}

//................................................................................................................
// ���� ��ų �ߵ� ��� ���� ���� ����
//................................................................................................................
int Character::SetKeepUpAction( int index )
{
	int motion_index = index;
	int select_index = 0;
	int RHand_index  = m_pUserInfo->ArmorInfo.index[0];
	int LHand_index  = m_pUserInfo->ArmorInfo.index[1];	
	int RHand_Code   = GetWearItemCode( n_WearSlot_Weapon );
	int LHand_Code   = GetWearItemCode( n_WearSlot_Shield );


	SItemBaseInfo* item_Info = NULL;	
		
	if( RHand_index != -1 && LHand_index == -1 )					// ������ ���⸸ ����
	{		
		if( g_Pc_Manager.m_pItm[RHand_index]->m_type == 0 )		// ��� 
		{
			select_index = n_Skill_KeepUp_Action_0;
		}
		else if( g_Pc_Manager.m_pItm[RHand_index]->m_type == 2 ) // �Ѽհ� 
		{
			select_index = n_Skill_KeepUp_Action_2;
		}
		else if( g_Pc_Manager.m_pItm[RHand_index]->m_type == 6 ) // ������ 
		{
			select_index = n_Skill_KeepUp_Action_6;
		}
		else if( g_Pc_Manager.m_pItm[RHand_index]->m_type == 7 ) // �����
		{
			select_index = n_Skill_KeepUp_Action_7;
		}
	}
	else if( RHand_index == -1 && LHand_index != -1 )			// �޼� ���⸸ ����
	{
		if( g_Pc_Manager.m_pItm[LHand_index]->m_type == 1 )			// Ȱ 
		{
			select_index = n_Skill_KeepUp_Action_2;
		}
		else if( g_Pc_Manager.m_pItm[LHand_index]->m_type == 3 )	// ���� 
		{
			select_index = n_Skill_KeepUp_Action_3;					// ���и� ������ ��ȭ��� 
		}
		else if( g_Pc_Manager.m_pItm[LHand_index]->m_type == 5 )	// ������ 
		{
			select_index = n_Skill_KeepUp_Action_5;
		}
	}
	else if( RHand_index != -1 && LHand_index != -1 )				// ��տ� ���� ����
	{
		if( g_Pc_Manager.m_pItm[RHand_index]->m_type == 2 && 
			g_Pc_Manager.m_pItm[LHand_index]->m_type == 3 )		// �Ѽհ� + ����
		{
			select_index = n_Skill_KeepUp_Action_3;
		}
	}
	
	return select_index;
}
//................................................................................................................

// ���⺰ ���� �⺻ ����
int Character::SetWeaponAttackMotionIndex( int index )
{
	int motion_index = index;
	int weaponType_BasicMotion = n_Attack9_Basic0;
	int add_index = 0;
	int RHand_index = m_pUserInfo->ArmorInfo.index[0];
	int LHand_index = m_pUserInfo->ArmorInfo.index[1];
	int RHand_Code = GetWearItemCode( n_WearSlot_Weapon );
//	int LHand_Code = GetWearItemCode( n_WearSlot_Shield );


	SItemBaseInfo* item_Info = NULL;	

	if( RHand_index != -1 && LHand_index == -1 )				// ������ ���⸸ ����
	{	
//		item_Info = g_Pc_Manager.GetItemBaseInfoPtr( RHand_Code );	// �и�ġ�� �����տ��� ���� ������...^ ^? 
//		if( item_Info )
//		{
//			if( item_Info->theType == n_Type_TwohandSword )			// ��� ���� ����� (�и�ġ) 
///			{
//				return n_Attack9_Basic0;							// ���� ���� �ִϸ��̼� 
//			}
//		}

		if( g_Pc_Manager.m_pItm[RHand_index]->m_type == 0 )		// ��� 
		{
			weaponType_BasicMotion = n_Attack0_Basic0;
		}
		else if( g_Pc_Manager.m_pItm[RHand_index]->m_type == 2 ) // �Ѽհ� 
		{
			weaponType_BasicMotion = n_Attack2_Basic0;
		}
		else if( g_Pc_Manager.m_pItm[RHand_index]->m_type == 6 ) // ������ 
		{
			weaponType_BasicMotion = n_Attack6_Basic0;
		}
		else if( g_Pc_Manager.m_pItm[RHand_index]->m_type == 7 ) // �����
		{
			weaponType_BasicMotion = n_Attack7_Basic0;
		}
	}
	else if( RHand_index == -1 && LHand_index != -1 )			// �޼� ���⸸ ����
	{
		if( g_Pc_Manager.m_pItm[LHand_index]->m_type == 1 )		// Ȱ 
		{
			weaponType_BasicMotion = n_Attack1_Basic0;
		}
		else if( g_Pc_Manager.m_pItm[LHand_index]->m_type == 3 ) // ���� 
		{
			weaponType_BasicMotion = n_Attack9_Basic0;			// ���и� ������ ��ȭ��� 
		}
		else if( g_Pc_Manager.m_pItm[LHand_index]->m_type == 5 ) // ������ 
		{
			weaponType_BasicMotion = n_Attack5_Basic0;			
		}
	}
	else if( RHand_index != -1 && LHand_index != -1 )			// ��տ� ���� ����
	{
		if( g_Pc_Manager.m_pItm[RHand_index]->m_type == 2 && 
			g_Pc_Manager.m_pItm[LHand_index]->m_type == 3 )		// �Ѽհ� + ����
		{
			weaponType_BasicMotion = n_Attack3_Basic0;	
		}
	}
	else														// ���� ���� (�Ǽ� ����)
	{
		weaponType_BasicMotion = n_Attack9_Basic0;	
	}

	if( index == n_Attack9_Basic4 )			// ũ��Ƽ���� ������ 5��° ����� ����Ѵ� 
	{
		motion_index = weaponType_BasicMotion + 4;
	}
	else
	{
		// ���� ��� ������ ���� ������ �⺻ ���� ����
		add_index = rand()%4;
		
		motion_index = weaponType_BasicMotion + add_index;
	}

	return motion_index;
}


int Character::SetChantMotion( int aChant_State, int aChant_grade )
{
	int motion_index = -1;
	int weaponType_BasicMotion = n_Attack9_Basic0;
	int add_index = 0;
	int RHand_index = m_pUserInfo->ArmorInfo.index[0];
	int LHand_index = m_pUserInfo->ArmorInfo.index[1];

	if( aChant_State == SM_CHANT_READY )
	{
		if( RHand_index != -1 && LHand_index == -1 )				// ������ ���⸸ ����
		{		
			if( g_Pc_Manager.m_pItm[RHand_index]->m_type == 6 )		// ������ 
			{
				weaponType_BasicMotion = n_Attack6_ChantReady0;			
			}
			else if( g_Pc_Manager.m_pItm[RHand_index]->m_type == 7 ) // �����
			{
				weaponType_BasicMotion = n_Attack7_ChantReady0;			
			}
		}
		else if( RHand_index == -1 && LHand_index != -1 )			// �޼� ���⸸ ����
		{		
			if( g_Pc_Manager.m_pItm[LHand_index]->m_type == 5 )		// ������ 
			{
				weaponType_BasicMotion = n_Attack5_ChantReady0;			
			}
			else if( g_Pc_Manager.m_pItm[LHand_index]->m_type == 1 )	//Ȱ
			{
				return n_Attack10_BowReady0;
			}
		}

		motion_index = weaponType_BasicMotion;
	}
	else if( aChant_State == SM_CHANT_SPELLING )
	{
		if( RHand_index != -1 && LHand_index == -1 )				// ������ ���⸸ ����
		{		
			if( g_Pc_Manager.m_pItm[RHand_index]->m_type == 6 )		// ������ 
			{
				weaponType_BasicMotion = n_Attack6_ChantGrade0;	
			}
			else if( g_Pc_Manager.m_pItm[RHand_index]->m_type == 7 ) // �����
			{
				weaponType_BasicMotion = n_Attack7_ChantGrade0;	
			}
		}
		else if( RHand_index == -1 && LHand_index != -1 )			// �޼� ���⸸ ����
		{		
			if( g_Pc_Manager.m_pItm[LHand_index]->m_type == 5 )		// ������ 
			{
				weaponType_BasicMotion = n_Attack5_ChantGrade0;			
			}
			else if( g_Pc_Manager.m_pItm[LHand_index]->m_type == 1 )	//Ȱ
			{
				return n_Attack10_BowWait0;
			}
		}

		motion_index = weaponType_BasicMotion + aChant_grade;
	}
	else if( aChant_State == SM_USE_SKILL )
	{
		if( RHand_index != -1 && LHand_index == -1 )				// ������ ���⸸ ����
		{		
			if( g_Pc_Manager.m_pItm[RHand_index]->m_type == 6 )		// ������ 
			{
				weaponType_BasicMotion = n_Attack6_ChantShoot0;			
			}
			else if( g_Pc_Manager.m_pItm[RHand_index]->m_type == 7 ) // �����
			{
				weaponType_BasicMotion = n_Attack7_ChantShoot0;			
			}
		}
		else if( RHand_index == -1 && LHand_index != -1 )			// �޼� ���⸸ ����
		{		
			if( g_Pc_Manager.m_pItm[LHand_index]->m_type == 5 )		// ������ 
			{
				weaponType_BasicMotion = n_Attack5_ChantShoot0;			
			}			
		}

		motion_index = weaponType_BasicMotion;
	}


	return motion_index;
}


float Character::GetAniTime()
{
//	float ani_time = c_Stand_AniTime;
	
	///-- �Ϲ� �ִϸ��̼��� 0.03�ʸ��� 1�������� �����Ѵ�.
	float ani_time = m_ani_time;
	
	///-- ��ų ���� �ִϸ��̼� �ӵ��� �������� ���� - ���� �ľ��� �����ؾ��� �ʿ�����
	
	///-- ���� ���ӾִϽ�ų�� ����ÿ��� ����	
	if( m_IsApplyConAniSkill || m_curt_event.type == SM_ATTACK_NPC ||
		m_curt_event.status == SM_BOW_READY  ||
		( m_curt_event.type == SM_USE_SKILL && ( m_curt_event.status == SM_CHANT_READY || m_curt_event.status == SM_CHANT_SPELLING ) ) 
		///-- �з����������� �̺κ��� �����ϸ� ������ ��û���� ������.... ������ �ľ��ؼ� 
		///-- ���� ����غ��� �����ؾ��ҵ� - �ִϸ��̼� �κ��� �������� �Ǽ�.. �ִϸ��̼� ���ý�ų�� 
		///-- ���� �з��� �������� ������ ��ü������ �������� ������ ����...
#ifdef APPLY_SKILL_VELOCITY		
		|| ( m_curt_event.type == SM_USE_SKILL && m_curt_event.status == SM_USE_SKILL ) 
#endif		
		)
	{	
		int AttackTime = 0;
		
		if( m_bPC )
		{
			float rate = 1.0f; 
			
			long org_speed = c_WeaponDelayTime / 2;

			if( ( org_speed - m_Weapon_DelayTime ) >= 0 )
			{
				rate = ( org_speed - m_Weapon_DelayTime ) / (float) org_speed;				
				AttackTime = ( c_Stand_AniTime / 2 ) * rate; 
			}
			else
			{
				rate = ( org_speed - m_Weapon_DelayTime ) / (float) org_speed;			
				AttackTime = ( c_Stand_AniTime / 2 )  * rate;
			}
		
			ani_time = ( m_ani_time - AttackTime );
		}
		else
		{
			SNPCBaseInfo*	npc_Info = NULL;
			npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( g_Map.m_NpcTable[m_nTableIndex].code ); 

			long org_speed = npc_Info->theAttackSpeed * 100;
			float rate = 1.0f; 

			// test code
//			m_Weapon_DelayTime = npc_Info->theAttackSpeed * 100 * 2;
//			m_Weapon_DelayTime = npc_Info->theAttackSpeed * 0.5f;

			if( ( org_speed - m_Weapon_DelayTime ) >= 0 )
			{
				rate = ( org_speed - m_Weapon_DelayTime ) / (float) org_speed;
				AttackTime = ( c_Stand_AniTime / 2 ) * rate; 
			}
			else
			{
				rate = ( org_speed - m_Weapon_DelayTime ) / (float) org_speed;
				AttackTime = ( c_Stand_AniTime / 2 )  * rate; 
			}

			ani_time = ( m_ani_time - AttackTime );
		}
		
//		RLG2( "m_Weapon_DelayTime = %d, ani_time = %3.3f", m_Weapon_DelayTime, ani_time );
	}
	// ������ �����϶� ���ϸ��̼� �ӵ��� ������ Boa��  by wxywxy
	else if( m_curt_event.type == SM_ANI_SQUENCE )
	{
		if( m_bPC )
		{
			// �÷��̾��϶�
			if( this == g_Pc.GetPlayer() )
			{
				m_realani_time = c_Stand_AniTime * g_RockClient.m_SceneManager.GetSquencePlayerAniTime();
				return ( c_Stand_AniTime * g_RockClient.m_SceneManager.GetSquencePlayerAniTime() );
			}
			// �ٸ� �÷��̾� �϶�
			else
			{
				m_realani_time = c_Stand_AniTime * g_RockClient.m_SceneManager.GetSquenceOtherPlayerAniTime(); 
				return ( c_Stand_AniTime * g_RockClient.m_SceneManager.GetSquenceOtherPlayerAniTime() );
			}
		}
	}

	switch( m_ani_index )
	{
	case n_Attack10_BowReady0:
		{
			if(m_bPC)
			{
				ani_time *= g_ArcherBar.m_fVelRatio;
			}
			else
			{
				ani_time *= 0.25f;
			}
		}
		break;	
	case n_Run0:
	case n_Run1:
	case n_Run2:
	case n_Run3:
	case n_Run5:
	case n_Run6:
	case n_Run7:
	case n_Run9:
	case n_Walk0:
	case n_Walk1:
	case n_Walk2:
	case n_Walk3:
	case n_Walk5:
	case n_Walk6:
	case n_Walk7:
	case n_Walk9:
	case n_SI_OneHand_Walk:
	case n_SI_OneHand_Run:
	case n_Ride_Walk:	
	case n_Ride_Run:
		{
#ifdef ROCKCLIENT_DEV			
//			m_ani_percent = 0.25f;
//			m_move_percent = 1.0f;
#endif
//			ani_time = ( c_Stand_AniTime * m_ani_percent );
			ani_time = ( m_ani_time * m_ani_percent );		

//			RLG1( "ani_time = %3.3f", ani_time );
		}
		break;
	case n_NPC_Run0:
	case n_NPC_Run1:
	case n_NPC_Walk0:
	case n_NPC_Walk1:
		{
//			m_ani_percent = 1.0f;
//			m_move_percent = 1.0f;
//			ani_time = ( c_Stand_AniTime * m_ani_percent );
			
			ani_time = ( m_ani_time * m_ani_percent );	
		}
		break;
	default:
		break;
	}
	
	if( ani_time > 90.0f )
	{
		ClientFilelog( "Stop Motion Bug ani_time = %f m_ani_time = %f m_ani_percent = %f", 
						ani_time, m_ani_time, m_ani_percent);		
		ani_time = 90.0f;		
	}

	m_realani_time = ani_time;	
	
	return ani_time;
}
/*
int Character::GetAttackSpeedTime()
{

//	���ݼӵ� ǥ��.
//	0~2 = �ſ� ����
//	3~5 = �ణ ����
//	6~8 = ����
//	9~12 = ����
//	13~15 = ���� ����
//	16~20 = ����

//const int c_AttackTime_VFast( 6 );		///-- �ſ� ����
//const int c_AttackTime_Fast( 4 );		///-- ��     ��
//const int c_AttackTime_LFast( 2 );		///-- �ణ ����
//const int c_AttackTime_Normal( 0 );		///-- ��     ��
//const int c_AttackTime_LSlow( -2 );		///-- ���� ����
//const int c_AttackTime_Slow( -4 );		///-- ��     ��
//const int c_AttackTime_VSlow( -6 );		///-- �ſ� ����


	if( m_Weapon_DelayTime >= 0 && m_Weapon_DelayTime <= 2 )
	{
		return c_AttackTime_VFast;
	}
	else if( m_Weapon_DelayTime >= 3 && m_Weapon_DelayTime <= 5 )
	{
		return c_AttackTime_Fast;
	}
	else if( m_Weapon_DelayTime >= 6 && m_Weapon_DelayTime <= 8 )
	{
		return c_AttackTime_LFast;
	}
	else if( m_Weapon_DelayTime >= 9 && m_Weapon_DelayTime <= 12 )
	{
		return c_AttackTime_Normal;
	}
	else if( m_Weapon_DelayTime >= 13 && m_Weapon_DelayTime <= 15 )
	{
		return c_AttackTime_LSlow;
	}
	else if( m_Weapon_DelayTime >= 16 && m_Weapon_DelayTime <= 18 )
	{
		return c_AttackTime_Slow;
	}	
	else if( m_Weapon_DelayTime >= 19 && m_Weapon_DelayTime <= 20 )
	{
		return c_AttackTime_VSlow;
	}	
	else 
	{
		return c_AttackTime_Normal;
	}

	return c_AttackTime_Normal;
}
*/