// ActionSystem.cpp: implementation of the CActionSystem class.
//
//////////////////////////////////////////////////////////////////////
#include "RockPCH.h"
#include "VFileSystem.h"

#include "Stdlib.h"
#include "..\\RockClient.h"
#include <NETWORK\\Protocol.h>
#include "..\\quadlist.h"
#include "..\\Pc.h"
#include "ActionSystem.h"

#include "RockInterface\\NonPlayerInfoWnd.h"
#include "RockInterface\\Slot.h"
#include "RockInterface\\SkillWndProc.h"
#include "RockInterface\\CArcherBar.h"
#include "RockInterface\\CLogWnd.h"
#include "RockInterface\\ChantHitBarWnd.h"

#include "Effect.h"
#include "stringmanager.h"



extern	long			g_nowTime;
extern	CBravoPC		g_Pc;

extern CBravoParticle		g_Particle;

// 150
const int c_Level2_ChantTime_Point1_Min( 40 );		
const int c_Level2_ChantTime_Point1_Max( 60 );

// 99, 198
const int c_Level3_ChantTime_Point1_Min( 23 );
const int c_Level3_ChantTime_Point1_Max( 43 );
const int c_Level3_ChantTime_Point2_Min( 56 );
const int c_Level3_ChantTime_Point2_Max( 76 );

// 75, 150, 225
const int c_Level4_ChantTime_Point1_Min( 15 );
const int c_Level4_ChantTime_Point1_Max( 35 );
const int c_Level4_ChantTime_Point2_Min( 40 );
const int c_Level4_ChantTime_Point2_Max( 60 );
const int c_Level4_ChantTime_Point3_Min( 65 );
const int c_Level4_ChantTime_Point3_Max( 85 );

// 60, 120, 180, 240
const int c_Level5_ChantTime_Point1_Min( 10 );
const int c_Level5_ChantTime_Point1_Max( 30 );
const int c_Level5_ChantTime_Point2_Min( 30 );
const int c_Level5_ChantTime_Point2_Max( 50 );
const int c_Level5_ChantTime_Point3_Min( 50 );
const int c_Level5_ChantTime_Point3_Max( 70 );
const int c_Level5_ChantTime_Point4_Min( 70 );
const int c_Level5_ChantTime_Point4_Max( 90 );

// 50, 100, 150, 200, 250
const int c_Level6_ChantTime_Point1_Min( 7 );
const int c_Level6_ChantTime_Point1_Max( 27 );
const int c_Level6_ChantTime_Point2_Min( 23 );
const int c_Level6_ChantTime_Point2_Max( 43 );
const int c_Level6_ChantTime_Point3_Min( 40 );
const int c_Level6_ChantTime_Point3_Max( 60 );
const int c_Level6_ChantTime_Point4_Min( 56 );
const int c_Level6_ChantTime_Point4_Max( 76 );
const int c_Level6_ChantTime_Point5_Min( 73 );
const int c_Level6_ChantTime_Point5_Max( 93 );



const float c_ChantHitTime( 30.0f );

const WORD c_MaxComboItemLevel( 5 );	// 0 ~ 4, S ~ D 

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CActionSystem::CActionSystem()
{
	theCurtComboType = 0;	
	theSuccessComboKeyType = 0;
	theSuccessComboCnt = 0;	
	theSuccessComboStatus = n_NotCombo;
	theSuccessComboSeq = 0;

	theCurtChantType = 0;
	theSuccessChantCnt = 0; 
//	theSuccessChantStatus = n_Chant_Cancel;
	theSuccessChantStatus = n_Chant_Start;		// test �ϱ�...���߿��� ��ų�� ���� start !!!	
	
	theComboLevel = 0;
	theComboGauge = 0;

	theProimaLevel = 0;
	theProimaGauge = 0;
	
	theSpellingTime = 500;
	theChantTime = 0;	

	theArcherTime = 0;
	theSuccessArcherCnt = 0;
	theArcherSpellTime = 500;

//	theComboGaugeInfo_gauge = NULL;
	theComboStepInfo_gauge = NULL;	

	theChantTimeBar = NULL;
	theChantCompleteImg = NULL;	
	theChantPoint1 = NULL;	
	theChantPoint2 = NULL;	
	theChantPoint3 = NULL;	
	theChantPoint4 = NULL;	
	theChantPoint5 = NULL;	

	InitComboEnable();

	thePreTime = g_nowTime;

	theChantSkillCode = 0;
	theComboSkillCode0 = 0;
	theComboSkillCode1 = 0;
	theComboSkillCode2 = 0;
	theComboSkillCode7 = 0;
	
	m_bActAutoCombo = FALSE;

	m_vPlayerPos = D3DXVECTOR3(0.0f , 0.0f , 0.0f );
	m_vOldPlayerPos = m_vPlayerPos;
	D3DXMatrixIdentity(&m_Matchar);

	theTotComboStep = 0;
	theNowRandomKey = 0;

	theTotComboStep = 5;

	theUseSkillCode = 0;
}

CActionSystem::~CActionSystem()
{

}

void CActionSystem::InitData()
{
	InitComboEnable();
	
	theChantSkillCode = 0;
	theComboSkillCode0 = 0;
	theComboSkillCode1 = 0;
	theComboSkillCode2 = 0;
	theComboSkillCode7 = 0;
}

//-----------------------------------------------------------------------------
// Desc: NPC �Ӽ��ֱ�
//-----------------------------------------------------------------------------
bool CActionSystem::ReadComboSeqeunce( const char* file_pos )
{
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined( LOAD_VF ) /////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	char	file_path[256] = {0,};
	char	file_name[256] = {0,};
	
	strcpy( file_path, file_pos, 256 );
	sprintf( file_name, "\\Chr\\Combo_Seq.dat");
	strcat( file_path, file_name);		

	FILE*	file;
	file = fopen( file_path, "rt" );
	if( file == NULL )
		return false;
	
	char* token;				// ��ū
	char string [256];			// ���� �ٴ��� ���� ����
	bool bRead  = true;			// ���� �� �о����� üũ			
	char splitter[] = " \n\t";	// ������ : /, ĳ��������, �� 
	int	 seq_cnt = 0;			// ������ ī��Ʈ 
	int	 type_cnt = 0;			// �޺� Ÿ�� ī��Ʈ 
	int	 num = -1;				// �޺� Ÿ�� ī��Ʈ 
			
	// �� ������ �б�
	if( fgets( string, 256, file ) == NULL )				
			return false;				
	do 
	{	
		// �ּ��� �ִٸ� ���� �� ����
		while( string[0] == '/' && string[1] == '/' )	
		{				
			// �� �̻� ������ ���ٸ� ���� ����
			if( fgets( string, 256 , file ) == NULL )		
			{
				bRead = false;
				break;
			}
		}

		if( seq_cnt >= 20 )
		{
			++type_cnt;
			seq_cnt = 0;			
		}

		// �ε��� 
		token = strtok( string, splitter );
		if( token == NULL )			
			break;	

		theCombo_Info[type_cnt].combo_index[seq_cnt] = (int)atoi(token);
		
		// 1st
		token = strtok( NULL, splitter );
		theCombo_Info[type_cnt].combo_sequence[seq_cnt][++num] = (int)atoi(token);
		
		// 2st
		token = strtok( NULL, splitter );
		theCombo_Info[type_cnt].combo_sequence[seq_cnt][++num] = (int)atoi(token);

		// 3st
		token = strtok( NULL, splitter );
		theCombo_Info[type_cnt].combo_sequence[seq_cnt][++num] = (int)atoi(token);

		// 4st
		token = strtok( NULL, splitter );
		theCombo_Info[type_cnt].combo_sequence[seq_cnt][++num] = (int)atoi(token);

		// 5st
		token = strtok( NULL, splitter );
		theCombo_Info[type_cnt].combo_sequence[seq_cnt][++num] = (int)atoi(token);

		// 6st
		token = strtok( NULL, splitter );
		theCombo_Info[type_cnt].combo_sequence[seq_cnt][++num] = (int)atoi(token);

		// 7st
		token = strtok( NULL, splitter );
		theCombo_Info[type_cnt].combo_sequence[seq_cnt][++num] = (int)atoi(token);

		// 8st
		token = strtok( NULL, splitter );
		theCombo_Info[type_cnt].combo_sequence[seq_cnt][++num] = (int)atoi(token);
		
//		token = strtok( NULL, splitter );
//		sprintf( &theCombo_Info[type_cnt].combo_sequence[seq_cnt][++num], "%s", token );
		
		++seq_cnt;
		num = -1;
		
		// ���� �� �б�, ������ ���ٸ� string�� 0����									
		if( fgets( string, 256 , file ) == NULL )		
			bRead = false;			
	}
	while( bRead != false ); // �� �̻� ������ ���ٸ� ���� ����	
	fclose( file );	


///////////////////////////////////////////////////////////////////////////////////////////////////	
#else  ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
	
	VFileHandle* pFH = g_VFChrRoot.OpenFile( "Combo_Seq.dat" );
	if( pFH == NULL ) return false;
	
	char* token;				// ��ū
	char string [256];			// ���� �ٴ��� ���� ����
	bool bRead  = true;			// ���� �� �о����� üũ			
	char splitter[] = " \n\t";	// ������ : /, ĳ��������, �� 
	int	 seq_cnt = 0;			// ������ ī��Ʈ 
	int	 type_cnt = 0;			// �޺� Ÿ�� ī��Ʈ 
	int	 num = -1;				// �޺� Ÿ�� ī��Ʈ 
			
	// �� ������ �б�
	if( pFH->Gets( string, sizeof( string ) ) == 0 )
	{
		g_VFChrRoot.CloseFile( pFH );
		return false; 
	}			
	do 
	{	
		// �ּ��� �ִٸ� ���� �� ����
		while( string[0] == '/' && string[1] == '/' )	
		{	
			// �� �̻� ������ ���ٸ� ���� ����
			if( pFH->Gets( string, sizeof( string ) ) == 0 )
			{
				bRead = false;
				break;
			}
		}

		if( seq_cnt >= 20 )
		{
			++type_cnt;
			seq_cnt = 0;			
		}

		// �ε��� 
		token = strtok( string, splitter );
		if( token == NULL )			
			break;	

		theCombo_Info[type_cnt].combo_index[seq_cnt] = (int)atoi(token);

		// 1st
		token = strtok( NULL, splitter );
		theCombo_Info[type_cnt].combo_sequence[seq_cnt][++num] = (int)atoi(token);
		
		// 2st
		token = strtok( NULL, splitter );
		theCombo_Info[type_cnt].combo_sequence[seq_cnt][++num] = (int)atoi(token);

		// 3st
		token = strtok( NULL, splitter );
		theCombo_Info[type_cnt].combo_sequence[seq_cnt][++num] = (int)atoi(token);

		// 4st
		token = strtok( NULL, splitter );
		theCombo_Info[type_cnt].combo_sequence[seq_cnt][++num] = (int)atoi(token);

		// 5st
		token = strtok( NULL, splitter );
		theCombo_Info[type_cnt].combo_sequence[seq_cnt][++num] = (int)atoi(token);

		// 6st
		token = strtok( NULL, splitter );
		theCombo_Info[type_cnt].combo_sequence[seq_cnt][++num] = (int)atoi(token);

		// 7st
		token = strtok( NULL, splitter );
		theCombo_Info[type_cnt].combo_sequence[seq_cnt][++num] = (int)atoi(token);

		// 8st
		token = strtok( NULL, splitter );
		theCombo_Info[type_cnt].combo_sequence[seq_cnt][++num] = (int)atoi(token);
		
//		token = strtok( NULL, splitter );
//		sprintf( &theCombo_Info[type_cnt].combo_sequence[seq_cnt][++num], "%s", token );
		
		++seq_cnt;
		num = -1;
		
		// ���� �� �б�, ������ ���ٸ� string�� 0����									
		if( pFH->Gets( string, sizeof( string ) ) == 0 )
			bRead = false;			
	}
	while( bRead != false ); // �� �̻� ������ ���ٸ� ���� ����	

	g_VFChrRoot.CloseFile( pFH );
	
///////////////////////////////////////////////////////////////////////////////////////////////////	
#endif ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

	return true;
}



void CActionSystem::ActionProc()
{
	int chrAttrIndex = g_Pc.GetPlayer()->GetAttrIndex();
	int chrClassType = nRui->thePcParam.MainClass;	

	if( 
		(	// �з��� ��-��, ������ ������, �ҵ��, ����Ʈ, �ӽóʸ�, �����̾�  
			( chrAttrIndex == n_Millena_Man || chrAttrIndex == n_Millena_Woman ) &&
			( chrClassType == Wanderer ||
			  chrClassType == Swordman || chrClassType == Knight || chrClassType == Mercenary || 
			  chrClassType == Slayer ) 
		) ||
		(	// ���� ��-��, ������ ��Ʈ������, �Ҽ���, ���ӳ�
			// ��Ʈ�������� ��쿡�� ���� �������� �������϶��� �����Ų��.
			( chrAttrIndex == n_Rain_Man || chrAttrIndex == n_Rain_Woman ) &&
			( ( chrClassType == Stranger ) ||			     		 
			  chrClassType == Shaman || chrClassType == Psyche ) 
		)
	  )	
	{
		//////////////////////////////////////////////////////////////////////////////////////
		// �޺� ������ ���� Ű ���μ��� Z, X, C
		//////////////////////////////////////////////////////////////////////////////////////
		if( g_Pc.GetPlayer()->m_curt_event.type == SM_ATTACK_NPC ||
			g_Pc.GetPlayer()->m_curt_event.type == SM_USE_SKILL )
		{			
			if( g_Pc.GetPlayer()->m_next_event.type != SM_USE_SKILL )
			{			
				///-- �ڵ��޺��� �ƴҰ��
				if(m_bActAutoCombo == FALSE)
				{
					if( g_RockClient.theKey[DIK_Z].Action == n_baDown )
					{			
						ComboSystem(DIK_Z);					
					}
					else if( g_RockClient.theKey[DIK_X].Action == n_baDown )
					{
						ComboSystem(DIK_X);						
					}
					else if( g_RockClient.theKey[DIK_C].Action == n_baDown )
					{
						ComboSystem(DIK_C);						
					}
				}
				else
				{
					ComboAutoSystem();
				}
			}
		}		
	}
	
    // ������ ���� ������ ����  
	ActionProcChantCombo();	
	
	return;
}

void CActionSystem::ActionProcChantCombo()
{
	int chrAttrIndex = g_Pc.GetPlayer()->GetAttrIndex();
	int chrClassType = nRui->thePcParam.MainClass;	
	
	// ������ 
	if(		// ���� ��-��, ������ ��Ʈ������, �Ҽ���, ���ӳ� 
			( chrAttrIndex == n_Rain_Man || chrAttrIndex == n_Rain_Woman ) &&
			( chrClassType == Stranger ||
			  chrClassType == Magician || chrClassType == Prominas || 
			  chrClassType == Priest || chrClassType == Holy_Avenger ) 		 
		)
	{	
	//	PRINT_DLG_LOG("ProcChantCombo event.status %d " , g_Pc.GetPlayer()->m_curt_event.status );
		if( g_Pc.GetPlayer()->m_curt_event.status == SM_CHANT_SPELLING )
		{	
			bool bChant_Combo = false;

			if( chrClassType == Priest || chrClassType == Holy_Avenger 
				|| chrClassType == Magician || chrClassType == Prominas	)
			{
				bChant_Combo = true;
			}

			g_Pc.GetPlayer()->theActionSystem.ChantComboSystem( bChant_Combo );			
		}		
	}

	if(	( chrAttrIndex == n_Millena_Man || chrAttrIndex == n_Millena_Woman ) &&
		( chrClassType == Wanderer || chrClassType == Archer || chrClassType == Sniper ) )
	{	
		if( g_Pc.GetPlayer()->m_curt_event.status == SM_CHANT_SPELLING )
		{	
			bool bChant_Combo = true;
			
			g_Pc.GetPlayer()->theActionSystem.ChantComboSystem( bChant_Combo );			
		}
		else if( g_Pc.GetPlayer()->m_curt_event.type == SM_ATTACK_NPC )
		{
			if( g_ChantHitBarWnd.IsVisible )
			{
				nRui->SendEvent( WID_ChantHitBar_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
			}
		}
	}
}

char CActionSystem::ApllyAndGetRandomKey()
{
	int nKey = 0;

	int nCutType = RandomNum(1, 3);	///-- ���� Ÿ�� ���� - ���κ���, ���κ���, ���
	nKey = nCutType * 10;

	int nCutMotion = RandomNum(1, 5); ///-- ���� ��� ����
	nKey += nCutMotion;
	
	theNowRandomKey = nKey;

	switch( theNowRandomKey / 10 )
	{
		case 1:
			return 'Z';
			break;
		case 2:
			return 'X';
			break;
		case 3:
			return 'C';
			break;
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
// RockClient ActionSystem ////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
// �⺻ ���� �����Ŀ� �޺� ����� ����.
// ���� �Ǵ� �������� ���� ���� �ý����� �޶�����. 
// Ű�� ������ �޺� Ÿ�̹� �����ȿ� �ִ��� �˻縦 �Ͽ� �޺� ���� ���� Ȯ�� 
// => �ϴ� ���� ���� �ɷ�ġ�� �´� �޺� ������ Ű�� ���������� �˻� �Ѵ� 
//			(����Ǵ� ���� �޺��� ������ ���� ������ ���� ������ �޺� ����) 
//			(��ó�� ������ Ű Ÿ�Կ� ���� �ش�Ǵ� ���� �޺� �������� ������ ����)
// => �޺� ������ �����ȿ� �ִ��� ���� �ִ� ������ �˻��ؼ� �޺� Ÿ�̹����� Ȯ�� 
// ===> ���� �̺�Ʈ�� ����Ǿ� �ִ��� Ȯ���� ���ٸ� ���� �̺�Ʈ�� ���� �̺�Ʈ�� �����Ѵ� 
// ===> ���� �̺�Ʈ�� ����Ǿ� �ִ� ���¿��� ���� Ű �Է��� ���´ٸ� ���� �̺�Ʈ ����Ѵ� 
// ======> �̺�Ʈ ����ÿ� �޺��� ���Ǵ� �޺������� �� SP ��ġ�� ���ڸ��� ���� ����Ѵ� 
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

// if( chrAttrIndex == �з��� ) && if( chrClass == �ҵ��, ����Ʈ, �ӽóʸ�, �����̾� (��-��) )
// if( chrAttrIndex == ���� )  && if( chrClass == �Ҽ���, ���ӳ� (��-��) (���� ����) )
void CActionSystem::ComboSystem( int aPressKey )
{	
	// �ΰ�ȿ�� (�޺��Ұ�) ó�� 
	
	if( g_Pc.GetPlayer()->GetAddEffectState(n_TypeNotCombo))
	{
		return ;
	}
	/*
	if( g_Pc.GetPlayer()->theAddEffect_State[n_TypeNotCombo] == 1 )
	{
		return;
	}
*/

//	DebugPrint("\n�޺� ���� theCombo_State = %d\n", g_Pc.GetPlayer()->theCombo_State);	

	if( g_Pc.GetPlayer()->theCombo_State == n_Combo_Success || 			// ���� ������ �� ����� �ʿ� ���ݾ�?!!
		g_Pc.GetPlayer()->theCombo_State == n_Combo_Disable || 			// �ѹ� ���д� ��...!!
		g_Pc.GetPlayer()->theCombo_State == n_Combo_Skill_Enable )		// �޺��� �Ұ���, ��ų�� ����!! 
	{
		return;
	}

	int curt_frame = g_Pc.GetPlayer()->m_ani_frame;
	int curt_motion = g_Pc.GetPlayer()->m_ani_index;
	int range_startframe = g_Pc_Manager.GetRangeStartFrame( g_Pc.GetPlayer(), curt_motion );
	int range_endframe = g_Pc_Manager.GetRangeEndFrame( g_Pc.GetPlayer(), curt_motion );
	int end_frame = g_Pc.GetPlayer()->m_end_frame;
	
	if( range_endframe + c_MotionBlendHalfFrame > end_frame )
	{
		range_endframe = end_frame - c_MotionBlendHalfFrame;
	}
	
	///--PRINT_DLG_LOG( "g_Pc.GetPlayer()->m_curt_event.type = %d", g_Pc.GetPlayer()->m_curt_event.type );
	
	if( g_Pc.GetPlayer()->m_curt_event.type == SM_ATTACK_NPC )
	{
		if( curt_frame >= range_startframe && curt_frame <= range_endframe )
		{
			bool combo_result = true;
			
			bool bFlag = SetComboType();								
			// �÷��̾ ���� �������� ���� �������� �˻��ؼ� �޺� Ÿ�� ���� (���� �κ��丮 �ϼ��� �����)
			if(!bFlag)
			{
				return;
			}

			int combo_motion = -1;
					
			//���� �Ͽ��ٸ� ��ϵ� Ű���� ������
			if( (nRui->thePcParam.MainClass != Wanderer) && ( nRui->thePcParam.MainClass != Stranger) &&
				g_Pc.GetPlayer()->theActionSystem.theTotComboStep > 4 )
			{
				combo_motion = GetRandomComboAniIndex( theSuccessComboCnt, aPressKey );
			}
			else
			{
				combo_motion = GetComboAniIndex( theSuccessComboCnt, aPressKey ); 
			}			
		
			if( combo_motion == -1 )						// �޺� ���� 
				combo_result = false;		
			
			if( combo_result )
			{
				EVENT_DATA_INFO attack_event;
				attack_event = g_Pc.GetPlayer()->m_curt_event;
				attack_event.motion = combo_motion;
				attack_event.critical = theSuccessComboKeyType;		// �޺� Ű Ÿ�� 
				attack_event.count = theSuccessComboCnt;			// �޺� ī��Ʈ 
				attack_event.status = theSuccessComboStatus;		// �޺� ������ �ϼ� 
				attack_event.number = g_Pc.theSkillSystem.theShortCut_SkillCode[2];
				
#ifdef AUTO_COMBO
				if( theSuccessComboStatus == n_ComboComplete && 
				    theSuccessComboCnt == c_Max_ComboSeqNum )
				{
					m_bActAutoCombo = TRUE;
				}
#endif
				////-- �޺����߿� �޺������ �ٲ��.. �޺��� �ʱ�ȭ�ȴ�
				if( nRui->m_Combo2DTimingEffect.m_nComCount )
				{
					if( attack_event.race != nRui->m_Combo2DTimingEffect.m_nRace ||
						attack_event.unique != nRui->m_Combo2DTimingEffect.m_lDestID )
					{
						g_Pc.GetPlayer()->theCombo_State = n_Combo_Disable;
						InitComboEnable();
						return;
					}
				}				
				

				//by simwoosung
				nRui->m_Combo2DTimingEffect.SetComboState(STATE_COMBO_SUCCESS);

				if( g_Pc.GetPlayer()->m_curt_event.type ==SM_ATTACK_NPC  && 
					g_Pc.GetPlayer()->m_curt_event.kill)
				{
					//PRINT_DLG_LOG(" �̳��� ���� ���ε�...");
					nRui->m_Combo2DTimingEffect.SetComboState( STATE_COMBO_FAIL );
					g_Pc.GetPlayer()->theCombo_State = n_Combo_Disable;
					InitComboEnable();
				
					//SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_DEAD_TARGET ) );
					return ;
					
				}


#ifdef CONTROL_ATTACK_VELOCITY 
				g_Pc.GetPlayer()->m_ComBoAttackList.push_back( attack_event );

#else
				g_Pc.GetPlayer()->CM__ATTACK__NPC( attack_event );
				g_Pc.GetPlayer()->theCombo_State = n_Combo_Success;
#endif					
			
			}
			else
			{
				//by simwoosung
				nRui->m_Combo2DTimingEffect.SetComboState(STATE_COMBO_FAIL);			
				
				g_Pc.GetPlayer()->theCombo_State = n_Combo_Disable;
				InitComboEnable();
			}
		}	
	}	

	return;
}

void CActionSystem::ComboAutoSystem()
{
	// �ΰ�ȿ�� (�޺��Ұ�) ó�� 
	if( g_Pc.GetPlayer()->GetAddEffectState(n_TypeNotCombo))
	{
		m_bActAutoCombo = FALSE;
		return;
	}
	/*
	if( g_Pc.GetPlayer()->theAddEffect_State[n_TypeNotCombo] == 1 )
	{
		m_bActAutoCombo = FALSE;
		return;
	}
	*/
	if( g_Pc.GetPlayer()->theCombo_State == n_Combo_Success || 			// ���� ������ �� ����� �ʿ� ���ݾ�?!!
		g_Pc.GetPlayer()->theCombo_State == n_Combo_Disable || 			// �ѹ� ���д� ��...!!
		g_Pc.GetPlayer()->theCombo_State == n_Combo_Skill_Enable )		// �޺��� �Ұ���, ��ų�� ����!! 
	{
		m_bActAutoCombo = FALSE;
		return;
	}

	int curt_frame = g_Pc.GetPlayer()->m_ani_frame;
	int curt_motion = g_Pc.GetPlayer()->m_ani_index;
	int range_startframe = g_Pc_Manager.GetRangeStartFrame( g_Pc.GetPlayer(), curt_motion );
	int range_endframe = g_Pc_Manager.GetRangeEndFrame( g_Pc.GetPlayer(), curt_motion );

	if( g_Pc.GetPlayer()->m_curt_event.type == SM_ATTACK_NPC )
	{
		if( curt_frame >= range_startframe + 1 )	///-- ���� �������� ���������Ӻ��� 1�̻��̸� �ڵ��޺� ����	
		{
			bool combo_result = true;
			
			bool bFlag = SetComboType();	// �÷��̾ ���� �������� ���� �������� �˻��ؼ� �޺� Ÿ�� ���� (���� �κ��丮 �ϼ��� �����)
			if(!bFlag)
			{
				m_bActAutoCombo = FALSE;
				return;
			}

			int aPressKey = 0;

			///-- ���� ��ϵ� Ű���� ������ Ű�� �ڵ������� �����ش�.
			char comKey	= nRui->m_Combo2DTimingEffect.GetNowComboKey();
			
			switch(comKey)
			{
				case 'Z':
					{
						aPressKey = DIK_Z;
					}
					break;
				case 'X':
					{
						aPressKey = DIK_X;	
					}
					break;
				case 'C':
					{
						aPressKey = DIK_C;
					}
					break;
			}			
			
			int combo_motion = GetComboAniIndex( theSuccessComboCnt, aPressKey ); // �޺� ���� ���� �Ǵ� �� ���� �ִϸ��̼� �ε��� 
			
			if( combo_motion == -1 )						// �޺� ���� 
				combo_result = false;		
			
			if( combo_result )
			{
				EVENT_DATA_INFO attack_event;
				attack_event = g_Pc.GetPlayer()->m_curt_event;
				attack_event.motion = combo_motion;
				attack_event.critical = theSuccessComboKeyType;		// �޺� Ű Ÿ�� 
				attack_event.count = theSuccessComboCnt;			// �޺� ī��Ʈ 
				attack_event.status = theSuccessComboStatus;		// �޺� ������ �ϼ� 
				if( theSuccessComboStatus == n_ComboComplete )
				{
					attack_event.number = theSuccessComboSeq + 1;
					m_bActAutoCombo = FALSE;
				}
				
				//by simwoosung
				nRui->m_Combo2DTimingEffect.SetComboState(STATE_COMBO_SUCCESS);
#ifdef CONTROL_ATTACK_VELOCITY 
				g_Pc.GetPlayer()->m_ComBoAttackList.push_back( attack_event );

#else

				g_Pc.GetPlayer()->CM__ATTACK__NPC( attack_event );
#endif				
				g_Pc.GetPlayer()->theCombo_State = n_Combo_Success;			
			}
			else
			{
				//by simwoosung
				nRui->m_Combo2DTimingEffect.SetComboState(STATE_COMBO_FAIL);
				
				g_Pc.GetPlayer()->theCombo_State = n_Combo_Disable;
				InitComboEnable();
				m_bActAutoCombo = FALSE;
			}
		}
	}

}

// ���� ������ 
// if( chrAttrIndex == �з��� ) && if( chrClass == ��ó, �ҿ� �극��Ŀ ) (���� ����)
void CActionSystem::TimingHitSystem()
{
	// ����� ������ 
	return;
}

// ��â ��ų �����Ҷ� ������ �˷��ְ� �������� ���� (��â ��� ����)
// ��â �޺� ����Ʈ ��Ʈ ������ ���� ������ �˷��ش� (����� �ٲ��ָ� �ٸ�����鿡�� ���)
// ��â ��ų ������ ������ �˷��ְ� �������� ��â �������� �÷��ش� (��â ����)
// ������ 
// if( chrAttrIndex == ���� ) && if( chrClass == ������, ���ι̳���, ������Ʈ, ȣ�� ��� ) (���� ����)
void CActionSystem::ChantComboSystem( bool aChantCombo )
{
	// �ΰ�ȿ�� (�޺��Ұ�) ó�� 

	//if( g_Pc.GetPlayer()->theAddEffect_State[n_TypeNotSpelling] == 1 )
	if( g_Pc.GetPlayer()->GetAddEffectState(n_TypeNotSpelling))
	{
		g_Pc.GetPlayer()->m_curt_event.Clear();
		nRui->SendEvent( WID_ChantHitBar_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
		g_Pc.GetPlayer()->m_curt_event.loop = false;
		g_Pc.GetPlayer()->m_SpellEffect.Clear();
		g_Pc.GetSkillSystem()->theUseSpellSkill = FALSE;
		return;
	}

	if( g_Pc.GetPlayer()->m_curt_event.type == SM_STATE_DIE ) 
	{
		nRui->SendEvent( WID_ChantHitBar_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
		g_Pc.GetPlayer()->m_curt_event.loop = false;
		g_Pc.GetPlayer()->m_SpellEffect.Clear();
		g_Pc.GetSkillSystem()->theUseSpellSkill = FALSE;
		return;
	}

	if( !g_Pc.GetPlayer()->m_curt_event.loop ) 
	{
		nRui->SendEvent( WID_ChantHitBar_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
		g_Pc.GetPlayer()->m_curt_event.loop = false;
		g_Pc.GetPlayer()->m_SpellEffect.Clear();
		g_Pc.GetSkillSystem()->theUseSpellSkill = FALSE;
		return;
	}

	SSkillBaseInfo* skill_Info = NULL;
	skill_Info = g_Pc_Manager.GetSkillBaseInfoPtr( g_Pc.GetPlayer()->m_curt_event.count );		
	if( skill_Info == NULL )
	{
		g_Pc.GetPlayer()->m_curt_event.Clear();
		nRui->SendEvent( WID_ChantHitBar_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
		g_Pc.GetPlayer()->m_curt_event.loop = false;
		g_Pc.GetPlayer()->m_SpellEffect.Clear();
		g_Pc.GetSkillSystem()->theUseSpellSkill = FALSE;
		return;
	}

	int SpellingTime = skill_Info->theNeedSpelling * 100;	

	if( SpellingTime == 0 )
	{
		SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_SPELL_SKILL ) );
		g_Pc.GetPlayer()->m_curt_event.Clear();
		g_Pc.GetPlayer()->m_SpellEffect.Clear();
		nRui->SendEvent( WID_ChantHitBar_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
		g_Pc.GetPlayer()->m_curt_event.loop = false;	
		g_Pc.GetSkillSystem()->theUseSpellSkill = FALSE;
	}


	//ĳ���Ͱ� �����̸� ��â ���
	
	m_Matchar = g_Pc.GetPlayer()->GetPosTM();

	m_vPlayerPos = D3DXVECTOR3(m_Matchar._41 ,m_Matchar._42 ,m_Matchar._43);
	float fDel = D3DXVec3Length(&(m_vOldPlayerPos - m_vPlayerPos));

	if( fDel > 0.2f)
	{

		g_Pc.GetPlayer()->m_curt_event.Clear();
		g_Pc.GetPlayer()->m_SpellEffect.Clear();				
		nRui->SendEvent( WID_ChantHitBar_Wnd , n_emSetVisible, (EPARAM)false, 0 , 0 , 0 );
		g_Pc.GetPlayer()->m_curt_event.loop = false;	
		g_Pc.GetSkillSystem()->theUseSpellSkill = FALSE;
		return;
	}	

	int chant_level = 1;	

	chant_level = GetChantLevel( SpellingTime );

	if( chant_level == -1 )
	{ 
		chant_level = 0;
	}

	chant_level = 0;	
	
	theChantTime += SAFE_TIME_SUB( g_nowTime , thePreTime );

	///-- ���ݼӵ��� ���� ��â�������� ��ȭ
	SpellingTime *= ( g_Pc.GetPlayer()->m_realani_time / 27.0f ); 
	
	float time_rate = (float)theChantTime / SpellingTime;	
		
	int chant_percent = 0;

	chant_percent = time_rate * 100;
	
	chant_percent = SetRandomTypeChantPercent( chant_percent, time_rate );

	if( chant_percent > 100 )
		chant_percent = 100;

	if( chant_percent > 95 )
	{
		theChantTimeBar->SetPercent( 100 );
	}
	else
	{
		theChantTimeBar->SetPercent( chant_percent );
	}

	if( chant_percent == 0 )
	{
		g_Pc.GetPlayer()->m_SpellEffect.SetSpellEffect( g_Pc.GetPlayer()->m_curt_event.count , 0 , 
			g_Pc.GetPlayer()->GetUnique() , g_Pc.GetPlayer()->GetIndex() );
	}

	if( chant_percent >= 100 && theUseSkillCode != 0 )
	{		
		theUseSkillCode = 0;
		
		///-- �ϵ� �÷��̽�... Send_RpCsSpelling �Լ��� �ι�������.
		
		++theSuccessChantCnt;	// ���� ��Ų ī��Ʈ�� �����ش� 
		g_RockClient.Send_RpCsSpelling( theSuccessChantCnt ); 
		theSuccessChantStatus = n_Chant_Complete;
		g_Pc.GetPlayer()->m_curt_event.loop = false;

		nRui->SendEvent( WID_ChantHitBar_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );

		g_Pc.GetPlayer()->m_SpellEffect.Clear();
		g_Pc.GetSkillSystem()->theUseSpellSkill = FALSE;
	}
	
	thePreTime = g_nowTime;
	
	// ��â �ý��� -> �޺� �ý������� ���� 
	if( aChantCombo )
	{
		SetChantState( chant_level, chant_percent );
	}
		
	return;
}

void CActionSystem::ArcherComboSystem()
{
	// �ΰ�ȿ�� (�޺��Ұ�) ó�� 	
	if( g_Pc.GetPlayer()->GetAddEffectState(n_TypeNotAttack))
	{
		g_Pc.GetPlayer()->m_curt_event.Clear();
		nRui->SendEvent( WID_ChantHitBar_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
		return;
	}

	if( g_Pc.GetPlayer()->m_curt_event.type == SM_STATE_DIE ) 
	{
		return;
	}

	int SpellingTime = theArcherTime * g_ArcherBar.m_fVelRatio;

	theArcherTime += SAFE_TIME_SUB( g_nowTime , thePreTime );

	///-- ���ݼӵ��� ���� ��â�������� ��ȭ
	SpellingTime *= ( g_Pc.GetPlayer()->m_realani_time / 27.0f ); 
	
	float time_rate = (float)theChantTime / SpellingTime;	
		
	int chant_percent = 0;

	chant_percent = time_rate * 100;

	chant_percent = SetRandomTypeChantPercent( chant_percent, time_rate );

	if( chant_percent > 100 )
		chant_percent = 100;

	if( chant_percent > 95 )
	{
		theChantTimeBar->SetPercent( 100 );
	}
	else
	{
		theChantTimeBar->SetPercent( chant_percent );
	}

	if( chant_percent >= 100 )
	{		
		nRui->SendEvent( WID_ChantHitBar_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );	
	}
	
	thePreTime = g_nowTime;
}

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
// ComboSystem
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
int CActionSystem::InitComboEnable()
{
//	theCurtComboType = 0;

#ifdef _DEBUG
	PRINT_DLG_LOG("InitComboEnable");
#endif 
	theSuccessComboCnt = 0;
	theSuccessComboKeyType = 0;
	theSuccessComboStatus = 0;

	int num = 0;
	int i = 0;

	for( i = 0; i < c_Max_ComboType; ++i )
	{
		for( num = 0; num < c_Max_ComboNum; ++num )
		{
			theCombo_Info[i].combo_enable[num] = true;
		}
	}
	
	return -1;
}

int CActionSystem::GetComboLevel( int aComboType, bool IsArcher )
{
	int combo_level = -1;

	switch( aComboType )
	{
	case nComboType_TwoHand_Sword:	
		{
			combo_level = GetSkillLevel( theComboSkillCode0 );
		}
		break;
	case nComboType_OneHand_Sword:
	case nComboType_Sword_Shield:
		{
			if(IsArcher)
			{
				combo_level = GetSkillLevel( theComboSkillCode1 );
			}
			else
			{
				combo_level = GetSkillLevel( theComboSkillCode2 );
			}				
		}
		break;
	case nComboType_CrystalBall:
		{
			combo_level = GetSkillLevel( theComboSkillCode7 );
		}
		break;
	case nComboType_MagicBook:
		{			
			combo_level = GetSkillLevel( theChantSkillCode );	
		}
		break;
	case nComboType_Staff:
		{
			combo_level = GetSkillLevel( theChantSkillCode );	
		}
		break;
	default:
		break;
	}

	return combo_level;

}

int CActionSystem::GetComboLevel(void)
{
	theCurtComboType = nComboType_OneHand_Sword; 

	int RHand_index = g_Pc.GetPlayer()->m_pUserInfo->ArmorInfo.index[0];
	int LHand_index = g_Pc.GetPlayer()->m_pUserInfo->ArmorInfo.index[1];

	bool IsArcher = false;

	if( RHand_index != -1 && LHand_index == -1 )				// ������ ���⸸ ����
	{		
		if( g_Pc_Manager.m_pItm[RHand_index]->m_type == 0 )		// ��� 
		{
			theCurtComboType = nComboType_TwoHand_Sword; 
		}
		else if( g_Pc_Manager.m_pItm[RHand_index]->m_type == 2 ) // �Ѽհ� 
		{
			theCurtComboType = nComboType_OneHand_Sword; 
		}
		else if( g_Pc_Manager.m_pItm[RHand_index]->m_type == 6 ) // ������ 
		{
			theCurtComboType = nComboType_Staff; 
		}
		else if( g_Pc_Manager.m_pItm[RHand_index]->m_type == 7 ) // �����
		{
			theCurtComboType = nComboType_CrystalBall;	
		}
	}
	else if( RHand_index == -1 && LHand_index != -1 )			// �޼� ���⸸ ����
	{
		if( g_Pc_Manager.m_pItm[LHand_index]->m_type == 1 )		// Ȱ 
		{
			theCurtComboType = nComboType_OneHand_Sword;
			IsArcher = true;
		}
		else if( g_Pc_Manager.m_pItm[LHand_index]->m_type == 3 ) // ���� 
		{
			theCurtComboType = nComboType_OneHand_Sword;		// ���и� ������ ��ȭ��� 
		}
		else if( g_Pc_Manager.m_pItm[LHand_index]->m_type == 5 ) // ������ 
		{
			theCurtComboType = nComboType_MagicBook; 
		}
	}
	else if( RHand_index != -1 && LHand_index != -1 )			// ��տ� ���� ����
	{
		if( g_Pc_Manager.m_pItm[RHand_index]->m_type == 2 && 
			g_Pc_Manager.m_pItm[LHand_index]->m_type == 3 )		// �Ѽհ� + ����
		{
			theCurtComboType = nComboType_Sword_Shield; 
		}
	}
	else														// ���� ���� (�Ǽ� ����)
	{
		theCurtComboType = nComboType_OneHand_Sword; 						
	}

	int combo_level = -1;

	switch( theCurtComboType )
	{
	case nComboType_TwoHand_Sword:	
		{
			if( nRui->thePcInven.WearItem[n_WearSlot_Weapon] )
			{
				combo_level = GetSkillLevel( theComboSkillCode0 );
			}				
		}
		break;
	case nComboType_OneHand_Sword:
	case nComboType_Sword_Shield:
		{
			if( nRui->thePcInven.WearItem[n_WearSlot_Weapon] )
			{
				if(IsArcher)
				{
					combo_level = GetSkillLevel( theComboSkillCode1 );
				}
				else
				{
					combo_level = GetSkillLevel( theComboSkillCode2 );
				}
			}
		}
		break;
	case nComboType_CrystalBall:
		{
			if( nRui->thePcInven.WearItem[n_WearSlot_Weapon] )
			{
				combo_level = GetSkillLevel( theComboSkillCode7 );
			}
		}
		break;
	case nComboType_MagicBook:
		{			
			if( nRui->thePcInven.WearItem[n_WearSlot_Shield] )
			{
				combo_level = GetSkillLevel( theChantSkillCode );	
			}
		}
		break;
	case nComboType_Staff:
		{
			if( nRui->thePcInven.WearItem[n_WearSlot_Weapon] )
			{
				combo_level = GetSkillLevel( theChantSkillCode );	
			}
		}
		break;
	default:
		break;
	}

	return combo_level;
}

int CActionSystem::GetComboAniIndex( int &aSequenceNum, int aPressKey )
{
	int ani_index = -1;
	int num = 0;
	int index = 0;
	int combo_key = 0;
	int success_combo_num = 0;
	int success_combo_key = 0;	
	bool success_combo = false;
	int weapon_skill = 0;				// ������ ��ų �ɷ�ġ�� �¾ƾ� �޺� ��� ���� 
	SItemBaseInfo* weapon_info = NULL;	
	int combo_level = 1;	
	

	switch( theCurtComboType )
	{
	case nComboType_TwoHand_Sword:	
		{
			if( nRui->thePcInven.WearItem[n_WearSlot_Weapon] )
			{
				weapon_info = g_Pc_Manager.GetItemBaseInfoPtr( nRui->thePcInven.WearItem[n_WearSlot_Weapon]->Code );
				combo_level = GetSkillLevel( theComboSkillCode0 );
				if( combo_level == -1 )
				{
					SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_LEARN_BIGSWORD_SK ) );						
					return ani_index;
				}
			}
			else
			{
				return ani_index;
			}
		}
		break;
	case nComboType_OneHand_Sword:
	case nComboType_Sword_Shield:
		{
			if( nRui->thePcInven.WearItem[n_WearSlot_Weapon] )
			{
				weapon_info = g_Pc_Manager.GetItemBaseInfoPtr( nRui->thePcInven.WearItem[n_WearSlot_Weapon]->Code );
				combo_level = GetSkillLevel( theComboSkillCode2 );
				if( combo_level == -1 )
				{
					SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_LEARN_SWORD_SK ) );
					return ani_index;
				}
			}
			else
			{
				return ani_index;
			}
		}
		break;
	case nComboType_CrystalBall:
		{
			if( nRui->thePcInven.WearItem[n_WearSlot_Weapon] )
			{
				weapon_info = g_Pc_Manager.GetItemBaseInfoPtr( nRui->thePcInven.WearItem[n_WearSlot_Weapon]->Code );
				combo_level = GetSkillLevel( theComboSkillCode7 );
				if( combo_level == -1 )
				{
					SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_LEARN_SYNC_SK ) );						
					return ani_index;
				}
			}
			else
			{
				return ani_index;
			}
		}
		break;
	case nComboType_MagicBook:
		{			
			if( nRui->thePcInven.WearItem[n_WearSlot_Shield] )
			{
				weapon_info = g_Pc_Manager.GetItemBaseInfoPtr( nRui->thePcInven.WearItem[n_WearSlot_Shield]->Code );
				combo_level = GetSkillLevel( theChantSkillCode );	
				if( combo_level == -1 )
				{
					SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_LEARN_PROIMA_SK ) );						
					return ani_index;
				}
			}
			else
			{
				return ani_index;
			}
		}
		break;
	case nComboType_Staff:
		{
			if( nRui->thePcInven.WearItem[n_WearSlot_Weapon] )
			{
				weapon_info = g_Pc_Manager.GetItemBaseInfoPtr( nRui->thePcInven.WearItem[n_WearSlot_Weapon]->Code );
				combo_level = GetSkillLevel( theChantSkillCode );	
				if( combo_level == -1 )
				{
					SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_LEARN_PROIMA_SK ) );						
					return ani_index;
				}
			}
			else
			{
				return ani_index;
			}
		}
		break;
	default:
		break;
	}

	if( weapon_info == NULL )
		return ani_index;

	
	int chrClassType = nRui->thePcParam.MainClass;

	if( ( chrClassType == Wanderer ) || ( chrClassType == Stranger ) ) 
	{
		for( num = 0; num < c_Max_ComboNum; ++num )
		{
			theCombo_Info[theCurtComboType].combo_enable[num] = false;
		}

		theCombo_Info[theCurtComboType].combo_enable[0] = true;
	}
	else
	{
		// Log�� ��ϵǾ� �ִ� �������� ����� �� �ִ�. 
		for( num = 0; num < c_Max_ComboNum; ++num )
		{
			if( theCombo_Info[theCurtComboType].combo_enable[num] )
			{
				theCombo_Info[theCurtComboType].combo_enable[num] = false;
				
				int nRegNum = g_Pc.theSkillSystem.theShortCut_SkillCode[2] - 1;
				
				///-- ���� ��ϵ� �޺��� ��밡���ϴ�.
				if( nRegNum == num )
				{
					theCombo_Info[theCurtComboType].combo_enable[num] = true;
				}
			}
		}
	}

	for( num = 0; num < c_Max_ComboNum; ++num )
	{
		if( theCombo_Info[theCurtComboType].combo_enable[num] )
		{
			combo_key = theCombo_Info[theCurtComboType].combo_sequence[num][aSequenceNum];

			success_combo = false;

			if( aPressKey == DIK_Z )
			{		
				weapon_skill = c_ComboZKey1 + (c_MaxComboItemLevel - weapon_info->theZAttack);		// ������ Z ��� �ɷ�ġ�� �¾ƾ� �޺� ���� 

				if( combo_key >= c_ComboZKey1 && combo_key <= weapon_skill )
//				if( combo_key >= c_ComboZKey1 && combo_key <= c_ComboZKey5 )
				{
					success_combo = true;
				}
			}
			else if( aPressKey == DIK_X )
			{
				weapon_skill = c_ComboXKey1 + (c_MaxComboItemLevel - weapon_info->theXAttack);		// ������ X ��� �ɷ�ġ�� �¾ƾ� �޺� ���� 

				if( combo_key >= c_ComboXKey1 && combo_key <= weapon_skill )
				//if( combo_key >= c_ComboXKey1 && combo_key <= c_ComboXKey5 )
				{
					success_combo = true;
				}
			}
			else if( aPressKey == DIK_C )
			{
				weapon_skill = c_ComboCKey1 + (c_MaxComboItemLevel - weapon_info->theCAttack);		// ������ C ��� �ɷ�ġ�� �¾ƾ� �޺� ���� 

				if( combo_key >= c_ComboCKey1 && combo_key <= weapon_skill )
				//if( combo_key >= c_ComboCKey1 && combo_key <= c_ComboCKey5 )
				{
					success_combo = true;
				}
			}

			if( success_combo )				// ���� �� �������� ���� �޺� ���� 
			{
				///-- By simwoosung
				///-- �ϴ� �ִϸ��̼� ������ ��ϵ� Ű�� �켱������ ���д�.				
				if( !success_combo_key )  ///-- ���� �޺�Ű�� �����Ǿ� ���� ������
				{
					success_combo_key = combo_key;		///-- ����					
					success_combo_num = num;
				}
				else	///-- �޺�Ű�� ���� �����Ǿ��ִٸ�
				{
					///-- ���� ��ϵ� �޺�Ű�ĸ� ������ �ִϸ��̼��� �����޺�Ű�� �ٲ��ش�.
					if( g_Pc.theSkillSystem.theShortCut_SkillCode[2] ==  (num + 1) )
					{
					   	success_combo_key = combo_key;		///-- ������					
						success_combo_num = num;
					}
				}

				RLG1( "ComboSequenceNum = %d", success_combo_num );				
			}
			else
			{
				theCombo_Info[theCurtComboType].combo_enable[num] = false;
			}
		}
	}

	///-- By simwoosung - �޺��� �б�Ǵ°��� ���⿡ �ξ�� ���డ���ϰԵ�...
	aPressKey = 0;
	

	if( success_combo_key )				
	{	
		if( aSequenceNum >= (combo_level + 3) )
		{				
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_SHORT_COMBOLEVEL ) );
			return ani_index;
		}			
		
		int nowSP = GetNowSP();
		int useSP = GetUseSP();
//		if( nRui->thePcParam.Stat1.theSP >= useSP )		// �Ҹ�Ǵ� SP ��ġ�� �´� SP �� �ִٸ� �޺� ����!!!
		if( nowSP >= useSP )		// �Ҹ�Ǵ� SP ��ġ�� �´� SP �� �ִٸ� �޺� ����!!!			
		{
			ani_index = GetAnimationIndex( success_combo_key );
			theSuccessComboKeyType = success_combo_key;
			++aSequenceNum;				
			theSuccessComboStatus = n_Combo; 
			RLG1( "ComboCnt(aSequenceNum) = %d", aSequenceNum );
		}
		else 
		{
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_SHORTAGE_ANUM ) );
		}
	}

	//niceg	RLG1("aSequenceNum = %d\n", aSequenceNum);
	// 8���� �޺� �������� �ϼ��߰ų� ������ ������ ���� �������� 0�� ��쿡 �޺� �ϼ�!!!
	if( (aSequenceNum >= c_Max_ComboSeqNum) || 
		(theCombo_Info[theCurtComboType].combo_sequence[success_combo_num][aSequenceNum] == 0) ||
		( ( aSequenceNum >= (combo_level + 3) ) &&  
		( (nRui->thePcParam.MainClass != Wanderer) && ( nRui->thePcParam.MainClass != Stranger) ) )
	  )	//by simwoosung - �޺� ���ɽ������� �ϼ��ɶ���(�� ������ ������쿡��...�� �ϵ��ڵ� ��.��) ���ø�Ʈ�� ����
	{
		theSuccessComboStatus = n_ComboComplete; 
		theSuccessComboSeq = success_combo_num;
	}
	
	return ani_index;
}

int	CActionSystem::GetRandomComboAniIndex( int &aSequenceNum, int aPressKey )
{
	int ani_index = -1;
	int num = 0;
	int index = 0;
	int combo_key = 0;
	int success_combo_num = 0;
	int success_combo_key = 0;	
	bool success_combo = false;
	int weapon_skill = 0;				// ������ ��ų �ɷ�ġ�� �¾ƾ� �޺� ��� ���� 
	SItemBaseInfo* weapon_info = NULL;	
	int combo_level = 1;	
	

	switch( theCurtComboType )
	{
	case nComboType_TwoHand_Sword:	
		{
			if( nRui->thePcInven.WearItem[n_WearSlot_Weapon] )
			{
				weapon_info = g_Pc_Manager.GetItemBaseInfoPtr( nRui->thePcInven.WearItem[n_WearSlot_Weapon]->Code );
				combo_level = GetSkillLevel( theComboSkillCode0 );
				if( combo_level == -1 )
				{
					SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_LEARN_BIGSWORD_SK ) );						
					return ani_index;
				}
			}
			else
			{
				return ani_index;
			}
		}
		break;
	case nComboType_OneHand_Sword:
	case nComboType_Sword_Shield:
		{
			if( nRui->thePcInven.WearItem[n_WearSlot_Weapon] )
			{
				weapon_info = g_Pc_Manager.GetItemBaseInfoPtr( nRui->thePcInven.WearItem[n_WearSlot_Weapon]->Code );
				combo_level = GetSkillLevel( theComboSkillCode2 );
				if( combo_level == -1 )
				{
					SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_LEARN_SWORD_SK ) );
					return ani_index;
				}
			}
			else
			{
				return ani_index;
			}
		}
		break;
	case nComboType_CrystalBall:
		{
			if( nRui->thePcInven.WearItem[n_WearSlot_Weapon] )
			{
				weapon_info = g_Pc_Manager.GetItemBaseInfoPtr( nRui->thePcInven.WearItem[n_WearSlot_Weapon]->Code );
				combo_level = GetSkillLevel( theComboSkillCode7 );
				if( combo_level == -1 )
				{
					SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_LEARN_SYNC_SK ) );						
					return ani_index;
				}
			}
			else
			{
				return ani_index;
			}
		}
		break;
	case nComboType_MagicBook:
		{			
			if( nRui->thePcInven.WearItem[n_WearSlot_Shield] )
			{
				weapon_info = g_Pc_Manager.GetItemBaseInfoPtr( nRui->thePcInven.WearItem[n_WearSlot_Shield]->Code );
				combo_level = GetSkillLevel( theChantSkillCode );	
				if( combo_level == -1 )
				{
					SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_LEARN_PROIMA_SK ) );						
					return ani_index;
				}
			}
			else
			{
				return ani_index;
			}
		}
		break;
	case nComboType_Staff:
		{
			if( nRui->thePcInven.WearItem[n_WearSlot_Weapon] )
			{
				weapon_info = g_Pc_Manager.GetItemBaseInfoPtr( nRui->thePcInven.WearItem[n_WearSlot_Weapon]->Code );
				combo_level = GetSkillLevel( theChantSkillCode );	
				if( combo_level == -1 )
				{
					SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_LEARN_PROIMA_SK ) );						
					return ani_index;
				}
			}
			else
			{
				return ani_index;
			}
		}
		break;
	default:
		break;
	}

	if( weapon_info == NULL )
		return ani_index;	

	
	success_combo = false;
	combo_key = theNowRandomKey;

	if( aPressKey == DIK_Z )
	{		
		weapon_skill = c_ComboZKey1 + (c_MaxComboItemLevel);		// ������ Z ��� �ɷ�ġ�� �¾ƾ� �޺� ���� 

		if( combo_key >= c_ComboZKey1 && combo_key <= weapon_skill )
		{
			success_combo = true;
		}
	}
	else if( aPressKey == DIK_X )
	{
		weapon_skill = c_ComboXKey1 + (c_MaxComboItemLevel);		// ������ X ��� �ɷ�ġ�� �¾ƾ� �޺� ���� 

		if( combo_key >= c_ComboXKey1 && combo_key <= weapon_skill )
		{
			success_combo = true;
		}
	}
	else if( aPressKey == DIK_C )
	{
		weapon_skill = c_ComboCKey1 + (c_MaxComboItemLevel);		// ������ C ��� �ɷ�ġ�� �¾ƾ� �޺� ���� 

		if( combo_key >= c_ComboCKey1 && combo_key <= weapon_skill )
		{
			success_combo = true;
		}
	}

	if( success_combo )				// ���� �� �������� ���� �޺� ���� 
	{
		success_combo_key = combo_key;		///-- ����					
		success_combo_num = g_Pc.theSkillSystem.theShortCut_SkillCode[2] - 1;				
	}	

	if( success_combo_key )				
	{	
		if( aSequenceNum >= (combo_level + 3) )
		{				
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_SHORT_COMBOLEVEL ) );
			return ani_index;
		}			
		
		int nowSP = GetNowSP();
		int useSP = GetUseSP();
		if( nowSP >= useSP )		// �Ҹ�Ǵ� SP ��ġ�� �´� SP �� �ִٸ� �޺� ����!!!			
		{
			ani_index = GetAnimationIndex( success_combo_key );
			theSuccessComboKeyType = success_combo_key;
			++aSequenceNum;				
			theSuccessComboStatus = n_Combo; 
		}
		else 
		{
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_SHORTAGE_ANUM ) );
		}
	}

	// 8���� �޺� �������� �ϼ��߰ų� ������ ������ ���� �������� 0�� ��쿡 �޺� �ϼ�!!!
	if( aSequenceNum == theTotComboStep ) 
	{
		theSuccessComboStatus = n_ComboComplete; 
		theSuccessComboSeq = success_combo_num;
	}

	return ani_index;
}

bool CActionSystem::SetComboType()
{
	// ���� ���⿡ ���� ���� 
	theCurtComboType = nComboType_OneHand_Sword;
	SItemBaseInfo* weapon_info = NULL;

	int RHand_index = g_Pc.GetPlayer()->m_pUserInfo->ArmorInfo.index[0];
	int LHand_index = g_Pc.GetPlayer()->m_pUserInfo->ArmorInfo.index[1];

	///--���⸦ �������⸦ �����ϰ� ������
	if(g_Pc.GetPlayer()->m_pUserInfo->ArmorInfo.Code[0] != -1)
	{
		weapon_info = g_Pc_Manager.GetItemBaseInfoPtr( g_Pc.GetPlayer()->m_pUserInfo->ArmorInfo.Code[0] );
		if(weapon_info)
		{
			if( n_Type_ShortWeapon == weapon_info->theType )
			{
				return false;
			}
		}
	}	
	if(g_Pc.GetPlayer()->m_pUserInfo->ArmorInfo.Code[1] != -1)
	{
		weapon_info = g_Pc_Manager.GetItemBaseInfoPtr( g_Pc.GetPlayer()->m_pUserInfo->ArmorInfo.Code[1] );
		if(weapon_info)
		{
			if( n_Type_ShortWeapon == weapon_info->theType )
			{
				return false;
			}
		}
	}	

	if( RHand_index != -1 && LHand_index == -1 )				// ������ ���⸸ ����
	{		
		if( g_Pc_Manager.m_pItm[RHand_index]->m_type == 0 )		// ��� 
		{
			theCurtComboType = nComboType_TwoHand_Sword; 
		}
		else if( g_Pc_Manager.m_pItm[RHand_index]->m_type == 2 ) // �Ѽհ� 
		{
			theCurtComboType = nComboType_OneHand_Sword; 
		}
		else if( g_Pc_Manager.m_pItm[RHand_index]->m_type == 6 ) // ������ 
		{
			theCurtComboType = nComboType_Staff; 
		}
		else if( g_Pc_Manager.m_pItm[RHand_index]->m_type == 7 ) // �����
		{
			theCurtComboType = nComboType_CrystalBall;	
		}
	}
	else if( RHand_index == -1 && LHand_index != -1 )			// �޼� ���⸸ ����
	{
		if( g_Pc_Manager.m_pItm[LHand_index]->m_type == 1 )		// Ȱ 
		{
			theCurtComboType = nComboType_OneHand_Sword; 
		}
		else if( g_Pc_Manager.m_pItm[LHand_index]->m_type == 3 ) // ���� 
		{
			theCurtComboType = nComboType_OneHand_Sword;		// ���и� ������ ��ȭ��� 
		}
		else if( g_Pc_Manager.m_pItm[LHand_index]->m_type == 5 ) // ������ 
		{
			theCurtComboType = nComboType_MagicBook; 
		}
	}
	else if( RHand_index != -1 && LHand_index != -1 )			// ��տ� ���� ����
	{
		if( g_Pc_Manager.m_pItm[RHand_index]->m_type == 2 && 
			g_Pc_Manager.m_pItm[LHand_index]->m_type == 3 )		// �Ѽհ� + ����
		{
			theCurtComboType = nComboType_Sword_Shield; 
		}
	}
	else														// ���� ���� (�Ǽ� ����)
	{
		theCurtComboType = nComboType_OneHand_Sword; 						
	}

	return true;
}

int CActionSystem::GetAnimationIndex( int aComboKey )
{
	int ani_index = -1;
	int key_type = 0;
	int key_index = 0;

	if( aComboKey < c_ComboXKey1 )				// ZKey Press
	{
		key_type = 0;						
		key_index = aComboKey - 11;
	}
	else if( aComboKey < c_ComboCKey1 )			// XKey Press
	{
		key_index = aComboKey - 21;
		key_type = 5;						
	}
	else										// CKey Press
	{
		key_index = aComboKey - 31;
		key_type = 10;						
	}

	switch( theCurtComboType )
	{
	case nComboType_TwoHand_Sword:	
		ani_index = n_Attack0_KeyZ0 + key_type + key_index;
		break;
	case nComboType_OneHand_Sword:
		ani_index = n_Attack2_KeyZ0 + key_type + key_index;
		break;
	case nComboType_Sword_Shield:
		ani_index = n_Attack3_KeyZ0 + key_type + key_index;
		break;
	case nComboType_CrystalBall:
		ani_index = n_Attack7_KeyZ0 + key_type + key_index;
		break;
	case nComboType_MagicBook:
		ani_index = n_Attack5_KeyZ0 + key_type + key_index;
		break;
	case nComboType_Staff:
		ani_index = n_Attack6_KeyZ0 + key_type + key_index;
		break;	
	default:
		break;
	}

	return ani_index;
}

int CActionSystem::GetUseSP()
{
	int useSP = 0;

	if( theSuccessComboCnt >= 2 )
	{
		useSP = theSuccessComboCnt - 2;		// �޺� �� - 2 ��ŭ �Ҹ�
	}

	return useSP;
}

int CActionSystem::GetNowSP()
{
	int chrAttrIndex = g_Pc.GetPlayer()->GetAttrIndex();
	int chrClassType = nRui->thePcParam.MainClass;
	int nowSP = 0;


	if(	// �з��� ��-��, ������ ������, �ҵ��, ����Ʈ, �ӽóʸ�, �����̾�  
		( chrAttrIndex == n_Millena_Man || chrAttrIndex == n_Millena_Woman ) &&
		( chrClassType == Wanderer || 
		  chrClassType == Swordman || chrClassType == Knight || chrClassType == Mercenary || chrClassType == Slayer ) 
	  ) 
	{
		nowSP = nRui->thePcParam.Stat1.theSP;
	}
	else if( // ���� ��-��, ������ ��Ʈ������, �Ҽ���, ���ӳ� 
		( chrAttrIndex == n_Rain_Man || chrAttrIndex == n_Rain_Woman ) &&
		( chrClassType == Stranger || 
		  chrClassType == Magician || chrClassType == Prominas || chrClassType == Priest || chrClassType == Holy_Avenger ||
		  chrClassType == Shaman || chrClassType == Psyche ) 
	  )
	{
		nowSP = nRui->thePcParam.Stat1.theMP;
	}
	
	return nowSP;
}


///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
// ChantComboSystem
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
void CActionSystem::InitChantEnable()
{
	thePreTime = g_nowTime;
	theChantTime = 0;		
	theSuccessChantCnt = 0;
	theSuccessChantStatus = n_Chant_Start;
//	theChantCompleteImg->SetVisible(false);
	theChantTimeBar->SetPercent( 0 );
	
	//��â�ҋ� ĳ���� �̵��ϸ� .. ����϶� .. ���� 
	m_Matchar = g_Pc.GetPlayer()->GetPosTM();
	m_vOldPlayerPos = D3DXVECTOR3(m_Matchar._41,m_Matchar._42,m_Matchar._43 );
	

	return;
}

void CActionSystem::InitArcherEnable()
{
	thePreTime = g_nowTime;
	theArcherTime = 0;
	theSuccessArcherCnt = 0;
	
	theChantTimeBar->SetPercent( 0 );
}

void CActionSystem::SetChantState( int aLevel, int aChantTime )
{
	// ��â ����Ʈ�� �°� theChantTimeBar�� Ȯ���Ͽ� DIK_SPACE Ű�� ������ ��â�������� ä��� 
	if( g_RockClient.theKey[DIK_SPACE].Action == n_baDown && theSuccessChantStatus != n_Chant_Fail ) 
	{
//		DebugPrint("aChantTime = %d\n", aChantTime);
		switch( aLevel )
		{
		case 0:
			{
				theChantPoint1->SetBtnState( n_bsHold );
				theChantPoint2->SetBtnState( n_bsHold );
				theChantPoint3->SetBtnState( n_bsHold );
				theChantPoint4->SetBtnState( n_bsHold );
				theChantPoint5->SetBtnState( n_bsHold );
			}
			break;
		case 1:
			{
				if( theSuccessChantStatus == n_Chant_Start && 
					aChantTime >= c_Level2_ChantTime_Point1_Min && aChantTime <= c_Level2_ChantTime_Point1_Max )
				{	
					++theSuccessChantCnt;
					theChantPoint1->SetBtnState( n_bsSelect );					
					theSuccessChantStatus = n_Chant_Complete;
//					g_Pc.GetPlayer()->AnimationPlay( g_Pc.GetPlayer()->SetChantMotion( SM_CHANT_SPELLING, theSuccessChantCnt ) );		// ��â ����� ��� �׽�Ʈ...	
					
					g_Pc.GetPlayer()->m_SpellEffect.SetSpellEffect( g_Pc.GetPlayer()->m_curt_event.count , 1 , g_Pc.GetPlayer()->GetUnique() , g_Pc.GetPlayer()->GetIndex() );
				}		
				else
				{
					theSuccessChantStatus = n_Chant_Fail;
				}
			}
			break;
		case 2:
			{
				if( theSuccessChantStatus == n_Chant_Start && 
					aChantTime >= c_Level3_ChantTime_Point1_Min && aChantTime <= c_Level3_ChantTime_Point1_Max )
				{
					++theSuccessChantCnt;
					theChantPoint1->SetBtnState( n_bsSelect );					
					theSuccessChantStatus = n_Chant_Success;
//					g_Pc.GetPlayer()->AnimationPlay( g_Pc.GetPlayer()->SetChantMotion( SM_CHANT_SPELLING, theSuccessChantCnt ) );			

					g_Pc.GetPlayer()->m_SpellEffect.SetSpellEffect( g_Pc.GetPlayer()->m_curt_event.count , 1 , g_Pc.GetPlayer()->GetUnique() , g_Pc.GetPlayer()->GetIndex() );
				}
				else if( theSuccessChantStatus == n_Chant_Start && 
						 aChantTime >= c_Level3_ChantTime_Point2_Min && aChantTime <= c_Level3_ChantTime_Point2_Max )
				{
					++theSuccessChantCnt;
					theChantPoint2->SetBtnState( n_bsSelect );					
					theSuccessChantStatus = n_Chant_Complete;
//					g_Pc.GetPlayer()->AnimationPlay( g_Pc.GetPlayer()->SetChantMotion( SM_CHANT_SPELLING, theSuccessChantCnt ) );			

					g_Pc.GetPlayer()->m_SpellEffect.SetSpellEffect( g_Pc.GetPlayer()->m_curt_event.count , 2 , g_Pc.GetPlayer()->GetUnique() , g_Pc.GetPlayer()->GetIndex() );
				}	
				else
				{
					theSuccessChantStatus = n_Chant_Fail;
				}
			}
			break;
		case 3:
			{
				if( theSuccessChantStatus == n_Chant_Start && theSuccessChantCnt == 0 && 
					aChantTime >= c_Level4_ChantTime_Point1_Min && aChantTime <= c_Level4_ChantTime_Point1_Max )
				{
					++theSuccessChantCnt;
					theChantPoint1->SetBtnState( n_bsSelect );					
					theSuccessChantStatus = n_Chant_Success;
//					g_Pc.GetPlayer()->AnimationPlay( g_Pc.GetPlayer()->SetChantMotion( SM_CHANT_SPELLING, theSuccessChantCnt ) );			

					g_Pc.GetPlayer()->m_SpellEffect.SetSpellEffect( g_Pc.GetPlayer()->m_curt_event.count , 1 , g_Pc.GetPlayer()->GetUnique() , g_Pc.GetPlayer()->GetIndex() );
				}
				else if( theSuccessChantStatus == n_Chant_Start && theSuccessChantCnt == 1 && 
						 aChantTime >= c_Level4_ChantTime_Point2_Min && aChantTime <= c_Level4_ChantTime_Point2_Max )
				{
					++theSuccessChantCnt;
					theChantPoint2->SetBtnState( n_bsSelect );					
					theSuccessChantStatus = n_Chant_Success;
//					g_Pc.GetPlayer()->AnimationPlay( g_Pc.GetPlayer()->SetChantMotion( SM_CHANT_SPELLING, theSuccessChantCnt ) );			

					g_Pc.GetPlayer()->m_SpellEffect.SetSpellEffect( g_Pc.GetPlayer()->m_curt_event.count , 2 , g_Pc.GetPlayer()->GetUnique() , g_Pc.GetPlayer()->GetIndex() );
				}
				else if( theSuccessChantStatus == n_Chant_Start && theSuccessChantCnt == 2 && 
						 aChantTime >= c_Level4_ChantTime_Point3_Min && aChantTime <= c_Level4_ChantTime_Point3_Max )
				{
					++theSuccessChantCnt;
					theChantPoint3->SetBtnState( n_bsSelect );					
					theSuccessChantStatus = n_Chant_Complete;
//					g_Pc.GetPlayer()->AnimationPlay( g_Pc.GetPlayer()->SetChantMotion( SM_CHANT_SPELLING, theSuccessChantCnt ) );			

					g_Pc.GetPlayer()->m_SpellEffect.SetSpellEffect( g_Pc.GetPlayer()->m_curt_event.count , 3 , g_Pc.GetPlayer()->GetUnique() , g_Pc.GetPlayer()->GetIndex() );
				}
				else
				{
					theSuccessChantStatus = n_Chant_Fail;
				}
			}
			break;
		case 4:
			{
				if( theSuccessChantStatus == n_Chant_Start && theSuccessChantCnt == 0 && 
					aChantTime >= c_Level5_ChantTime_Point1_Min && aChantTime <= c_Level5_ChantTime_Point1_Max )
				{
					++theSuccessChantCnt;
					theChantPoint1->SetBtnState( n_bsSelect );					
					theSuccessChantStatus = n_Chant_Success;
//					g_Pc.GetPlayer()->AnimationPlay( g_Pc.GetPlayer()->SetChantMotion( SM_CHANT_SPELLING, theSuccessChantCnt ) );			

					g_Pc.GetPlayer()->m_SpellEffect.SetSpellEffect( g_Pc.GetPlayer()->m_curt_event.count , 1 , g_Pc.GetPlayer()->GetUnique() , g_Pc.GetPlayer()->GetIndex() );
				}
				else if( theSuccessChantStatus == n_Chant_Start && theSuccessChantCnt == 1 && 
						 aChantTime >= c_Level5_ChantTime_Point2_Min && aChantTime <= c_Level5_ChantTime_Point2_Max )
				{
					++theSuccessChantCnt;
					theChantPoint2->SetBtnState( n_bsSelect );					
					theSuccessChantStatus = n_Chant_Success;
//					g_Pc.GetPlayer()->AnimationPlay( g_Pc.GetPlayer()->SetChantMotion( SM_CHANT_SPELLING, theSuccessChantCnt ) );			

					g_Pc.GetPlayer()->m_SpellEffect.SetSpellEffect( g_Pc.GetPlayer()->m_curt_event.count , 2 , g_Pc.GetPlayer()->GetUnique() , g_Pc.GetPlayer()->GetIndex() );
				}
				else if( theSuccessChantStatus == n_Chant_Start && theSuccessChantCnt == 2 && 
						 aChantTime >= c_Level5_ChantTime_Point3_Min && aChantTime <= c_Level5_ChantTime_Point3_Max )
				{
					++theSuccessChantCnt;
					theChantPoint3->SetBtnState( n_bsSelect );					
					theSuccessChantStatus = n_Chant_Success;
//					g_Pc.GetPlayer()->AnimationPlay( g_Pc.GetPlayer()->SetChantMotion( SM_CHANT_SPELLING, theSuccessChantCnt ) );			

					g_Pc.GetPlayer()->m_SpellEffect.SetSpellEffect( g_Pc.GetPlayer()->m_curt_event.count , 3 , g_Pc.GetPlayer()->GetUnique() , g_Pc.GetPlayer()->GetIndex() );
				}
				else if( theSuccessChantStatus == n_Chant_Start && theSuccessChantCnt == 3 && 
						 aChantTime >= c_Level5_ChantTime_Point4_Min && aChantTime <= c_Level5_ChantTime_Point4_Max )
				{
					++theSuccessChantCnt;
					theChantPoint4->SetBtnState( n_bsSelect );					
					theSuccessChantStatus = n_Chant_Complete;
//					g_Pc.GetPlayer()->AnimationPlay( g_Pc.GetPlayer()->SetChantMotion( SM_CHANT_SPELLING, theSuccessChantCnt ) );			

					g_Pc.GetPlayer()->m_SpellEffect.SetSpellEffect( g_Pc.GetPlayer()->m_curt_event.count , 4 , g_Pc.GetPlayer()->GetUnique() , g_Pc.GetPlayer()->GetIndex() );
				}
				else
				{
					theSuccessChantStatus = n_Chant_Fail;
				}
			}
			break;
		case 5:
			{
 				if( theSuccessChantStatus == n_Chant_Start && theSuccessChantCnt == 0 && 
					aChantTime >= c_Level6_ChantTime_Point1_Min && aChantTime <= c_Level6_ChantTime_Point1_Max )
				{
					++theSuccessChantCnt;
					theChantPoint1->SetBtnState( n_bsSelect );					
					theSuccessChantStatus = n_Chant_Success;
//					g_Pc.GetPlayer()->AnimationPlay( g_Pc.GetPlayer()->SetChantMotion( SM_CHANT_SPELLING, theSuccessChantCnt ) );			

					g_Pc.GetPlayer()->m_SpellEffect.SetSpellEffect( g_Pc.GetPlayer()->m_curt_event.count , 1 , g_Pc.GetPlayer()->GetUnique() , g_Pc.GetPlayer()->GetIndex() );
				}
				else if( theSuccessChantStatus == n_Chant_Start && theSuccessChantCnt == 1 && 
						 aChantTime >= c_Level6_ChantTime_Point2_Min && aChantTime <= c_Level6_ChantTime_Point2_Max )
				{
					++theSuccessChantCnt;
					theChantPoint2->SetBtnState( n_bsSelect );					
					theSuccessChantStatus = n_Chant_Success;
//					g_Pc.GetPlayer()->AnimationPlay( g_Pc.GetPlayer()->SetChantMotion( SM_CHANT_SPELLING, theSuccessChantCnt ) );			

					g_Pc.GetPlayer()->m_SpellEffect.SetSpellEffect( g_Pc.GetPlayer()->m_curt_event.count , 2 , g_Pc.GetPlayer()->GetUnique() , g_Pc.GetPlayer()->GetIndex() );
				}
				else if( theSuccessChantStatus == n_Chant_Start && theSuccessChantCnt == 2 && 
						 aChantTime >= c_Level6_ChantTime_Point3_Min && aChantTime <= c_Level6_ChantTime_Point3_Max )
				{
					++theSuccessChantCnt;
					theChantPoint3->SetBtnState( n_bsSelect );					
					theSuccessChantStatus = n_Chant_Success;
//					g_Pc.GetPlayer()->AnimationPlay( g_Pc.GetPlayer()->SetChantMotion( SM_CHANT_SPELLING, theSuccessChantCnt ) );			

					g_Pc.GetPlayer()->m_SpellEffect.SetSpellEffect( g_Pc.GetPlayer()->m_curt_event.count , 3 , g_Pc.GetPlayer()->GetUnique() , g_Pc.GetPlayer()->GetIndex() );
				}
				else if( theSuccessChantStatus == n_Chant_Start && theSuccessChantCnt == 3 && 
						 aChantTime >= c_Level6_ChantTime_Point4_Min && aChantTime <= c_Level6_ChantTime_Point4_Max )
				{
					++theSuccessChantCnt;
					theChantPoint4->SetBtnState( n_bsSelect );					
					theSuccessChantStatus = n_Chant_Success;
//					g_Pc.GetPlayer()->AnimationPlay( g_Pc.GetPlayer()->SetChantMotion( SM_CHANT_SPELLING, theSuccessChantCnt ) );			

					g_Pc.GetPlayer()->m_SpellEffect.SetSpellEffect( g_Pc.GetPlayer()->m_curt_event.count , 4 , g_Pc.GetPlayer()->GetUnique() , g_Pc.GetPlayer()->GetIndex() );
				}
				else if( theSuccessChantStatus == n_Chant_Start && theSuccessChantCnt == 4 && 
						 aChantTime >= c_Level6_ChantTime_Point5_Min && aChantTime <= c_Level6_ChantTime_Point5_Max )
				{
					++theSuccessChantCnt;
					theChantPoint5->SetBtnState( n_bsSelect );					
					theSuccessChantStatus = n_Chant_Complete;
//					g_Pc.GetPlayer()->AnimationPlay( g_Pc.GetPlayer()->SetChantMotion( SM_CHANT_SPELLING, theSuccessChantCnt ) );			

					g_Pc.GetPlayer()->m_SpellEffect.SetSpellEffect( g_Pc.GetPlayer()->m_curt_event.count , 5 , g_Pc.GetPlayer()->GetUnique() , g_Pc.GetPlayer()->GetIndex() );
				}
				else
				{
					theSuccessChantStatus = n_Chant_Fail;
				}
			}
			break;
		
		default:
			break; 


		}		
		return;
	}
	

	if( theSuccessChantStatus == n_Chant_Start || theSuccessChantStatus == n_Chant_Success )
	{
//		theChantCompleteImg->SetVisible(false);

		switch( aLevel )
		{
		case 0:
			{
				theChantPoint1->SetBtnState( n_bsHold );
				theChantPoint2->SetBtnState( n_bsHold );
				theChantPoint3->SetBtnState( n_bsHold );
				theChantPoint4->SetBtnState( n_bsHold );
				theChantPoint5->SetBtnState( n_bsHold );
			}
			break;
		case 1:
			{
				theChantPoint1->SetBtnPosWnd( 165, 5 );

				if( aChantTime >= c_Level2_ChantTime_Point1_Min && aChantTime <= c_Level2_ChantTime_Point1_Max )
				{
					if( theChantPoint1->GetBtnState() == n_bsNormal )
					{
						theChantPoint1->SetBtnState( n_bsFocus );
						theSuccessChantStatus = n_Chant_Start;
					}
				}

				if( theSuccessChantStatus == n_Chant_Start && 
						( theSuccessChantCnt < 1 ) && 
						 aChantTime > c_Level2_ChantTime_Point1_Max )
				{
					theSuccessChantStatus = n_Chant_Fail;
				}
				else
				{
					if( theSuccessChantStatus == n_Chant_Start && 
						( theSuccessChantCnt == 0 ) && 
						theChantPoint1->GetBtnState() != n_bsNormal )
					{
						theChantPoint1->SetBtnState( n_bsNormal );
					}
				}				
			}
			break;
		case 2:
			{
				theChantPoint1->SetBtnPosWnd( 114, 5 );
				theChantPoint2->SetBtnPosWnd( 213, 5 );

				if( aChantTime >= c_Level3_ChantTime_Point1_Min && aChantTime <= c_Level3_ChantTime_Point1_Max )
				{
					if( theChantPoint1->GetBtnState() == n_bsNormal )
					{
						theChantPoint1->SetBtnState( n_bsFocus );
						theSuccessChantStatus = n_Chant_Start;
					}
				}				
				else if( aChantTime >= c_Level3_ChantTime_Point2_Min && aChantTime <= c_Level3_ChantTime_Point2_Max )
				{
					if( theChantPoint2->GetBtnState() == n_bsNormal )
					{
						theChantPoint2->SetBtnState( n_bsFocus );
						theSuccessChantStatus = n_Chant_Start;
					}
				}

				if( theSuccessChantStatus == n_Chant_Start && 
						 ( theSuccessChantCnt < 1 ) && 
						 ( aChantTime > c_Level3_ChantTime_Point1_Max ) ) // && aChantTime < c_Level3_ChantTime_Point2_Min ) )
				{
					theSuccessChantStatus = n_Chant_Fail;
				}
				else if( theSuccessChantStatus == n_Chant_Start && 
						 ( theSuccessChantCnt < 2 ) && 
						 aChantTime > c_Level3_ChantTime_Point2_Max )
				{
					theSuccessChantStatus = n_Chant_Fail;
				}
				else
				{
					if( theSuccessChantStatus == n_Chant_Start && 
						( theSuccessChantCnt == 0 ) && 
						theChantPoint1->GetBtnState() != n_bsNormal )
					{
						theChantPoint1->SetBtnState( n_bsNormal );
						theChantPoint2->SetBtnState( n_bsNormal );
					}
				}
			}
			break;
		case 3:
			{
				theChantPoint1->SetBtnPosWnd( 90, 5 );
				theChantPoint2->SetBtnPosWnd( 165, 5 );
				theChantPoint3->SetBtnPosWnd( 240, 5 );

				if( aChantTime >= c_Level4_ChantTime_Point1_Min && aChantTime <= c_Level4_ChantTime_Point1_Max )
				{
					if( theChantPoint1->GetBtnState() == n_bsNormal )
					{
						theChantPoint1->SetBtnState( n_bsFocus );
						theSuccessChantStatus = n_Chant_Start;
					}
				}				
				else if( aChantTime >= c_Level4_ChantTime_Point2_Min && aChantTime <= c_Level4_ChantTime_Point2_Max )
				{
					if( theChantPoint2->GetBtnState() == n_bsNormal )
					{
						theChantPoint2->SetBtnState( n_bsFocus );
						theSuccessChantStatus = n_Chant_Start;
					}
				}				
				else if( aChantTime >= c_Level4_ChantTime_Point3_Min && aChantTime <= c_Level4_ChantTime_Point3_Max )
				{
					if( theChantPoint3->GetBtnState() == n_bsNormal )
					{
						theChantPoint3->SetBtnState( n_bsFocus );
						theSuccessChantStatus = n_Chant_Start;
					}
				}

				if( theSuccessChantStatus == n_Chant_Start && 
						 ( theSuccessChantCnt < 1 ) && 
						 ( aChantTime > c_Level4_ChantTime_Point1_Max ) ) // && aChantTime < c_Level4_ChantTime_Point2_Min ) )
				{
					theSuccessChantStatus = n_Chant_Fail;
				}
				else if( theSuccessChantStatus == n_Chant_Start && 
						 ( theSuccessChantCnt < 2 ) && 
						 ( aChantTime > c_Level4_ChantTime_Point2_Max ) ) // && aChantTime < c_Level4_ChantTime_Point3_Min ) )
				{
					theSuccessChantStatus = n_Chant_Fail;
				}				
				else if( theSuccessChantStatus == n_Chant_Start && 
						 ( theSuccessChantCnt < 3 ) && 
						 ( aChantTime > c_Level4_ChantTime_Point3_Max ) )
				{
					theSuccessChantStatus = n_Chant_Fail;
				}
				else
				{
					if( theSuccessChantStatus == n_Chant_Start && 
						( theSuccessChantCnt == 0 ) && 
						theChantPoint1->GetBtnState() != n_bsNormal )
					{
						theChantPoint1->SetBtnState( n_bsNormal );
						theChantPoint2->SetBtnState( n_bsNormal );
						theChantPoint3->SetBtnState( n_bsNormal );
					}
				}
			}
			break;
		case 4:
			{
				theChantPoint1->SetBtnPosWnd( 75, 5 );
				theChantPoint2->SetBtnPosWnd( 135, 5 );
				theChantPoint3->SetBtnPosWnd( 195, 5 );
				theChantPoint4->SetBtnPosWnd( 255, 5 );

				if( aChantTime >= c_Level5_ChantTime_Point1_Min && aChantTime <= c_Level5_ChantTime_Point1_Max )
				{
					if( theChantPoint1->GetBtnState() == n_bsNormal )
					{
						theChantPoint1->SetBtnState( n_bsFocus );
						theSuccessChantStatus = n_Chant_Start;
					}
				}				
				else if( aChantTime >= c_Level5_ChantTime_Point2_Min && aChantTime <= c_Level5_ChantTime_Point2_Max )
				{
					if( theChantPoint2->GetBtnState() == n_bsNormal )
					{
						theChantPoint2->SetBtnState( n_bsFocus );
						theSuccessChantStatus = n_Chant_Start;
					}					
				}				
				else if( aChantTime >= c_Level5_ChantTime_Point3_Min && aChantTime <= c_Level5_ChantTime_Point3_Max )
				{
					if( theChantPoint3->GetBtnState() == n_bsNormal )
					{
						theChantPoint3->SetBtnState( n_bsFocus );
						theSuccessChantStatus = n_Chant_Start;
					}
				}				
				else if( aChantTime >= c_Level5_ChantTime_Point4_Min && aChantTime <= c_Level5_ChantTime_Point4_Max )
				{
					if( theChantPoint4->GetBtnState() == n_bsNormal )
					{
						theChantPoint4->SetBtnState( n_bsFocus );
						theSuccessChantStatus = n_Chant_Start;
					}
				}

				if( theSuccessChantStatus == n_Chant_Start && 
						 ( theSuccessChantCnt < 1 ) && 
						 ( aChantTime > c_Level5_ChantTime_Point1_Max ) ) // && aChantTime < c_Level5_ChantTime_Point2_Min ) )
				{
					theSuccessChantStatus = n_Chant_Fail;
				}
				else if( theSuccessChantStatus == n_Chant_Start && 
						 ( theSuccessChantCnt < 2 ) && 
						 ( aChantTime > c_Level5_ChantTime_Point2_Max ) ) // && aChantTime < c_Level5_ChantTime_Point3_Min ) )
				{
					theSuccessChantStatus = n_Chant_Fail;
				}
				else if( theSuccessChantStatus == n_Chant_Start && 
						 ( theSuccessChantCnt < 3 ) && 
						 ( aChantTime > c_Level5_ChantTime_Point3_Max ) ) // && aChantTime < c_Level5_ChantTime_Point4_Min ) )
				{
					theSuccessChantStatus = n_Chant_Fail;
				}
				else if( theSuccessChantStatus == n_Chant_Start && 
						 ( theSuccessChantCnt < 4 ) && 
						 ( aChantTime > c_Level5_ChantTime_Point4_Max ) )
				{
					theSuccessChantStatus = n_Chant_Fail;
				}
				else
				{
					if( theSuccessChantStatus == n_Chant_Start && 
						( theSuccessChantCnt == 0 ) && 
						theChantPoint1->GetBtnState() != n_bsNormal )
					{
						theChantPoint1->SetBtnState( n_bsNormal );
						theChantPoint2->SetBtnState( n_bsNormal );
						theChantPoint3->SetBtnState( n_bsNormal );
						theChantPoint4->SetBtnState( n_bsNormal );
					}
				}
			}
			break;
		case 5:
			{
				theChantPoint1->SetBtnPosWnd( 65, 5 );
				theChantPoint2->SetBtnPosWnd( 115, 5 );
				theChantPoint3->SetBtnPosWnd( 165, 5 );
				theChantPoint4->SetBtnPosWnd( 215, 5 );
				theChantPoint5->SetBtnPosWnd( 265, 5 );

				if( aChantTime >= c_Level6_ChantTime_Point1_Min && aChantTime <= c_Level6_ChantTime_Point1_Max )
				{
					if( theChantPoint1->GetBtnState() == n_bsNormal )
					{
						theChantPoint1->SetBtnState( n_bsFocus );
						theSuccessChantStatus = n_Chant_Start;
					}
				}
				else if( aChantTime >= c_Level6_ChantTime_Point2_Min && aChantTime <= c_Level6_ChantTime_Point2_Max )
				{
					if( theChantPoint2->GetBtnState() == n_bsNormal )
					{
						theChantPoint2->SetBtnState( n_bsFocus );
						theSuccessChantStatus = n_Chant_Start;
					}					
				}
				else if( aChantTime >= c_Level6_ChantTime_Point3_Min && aChantTime <= c_Level6_ChantTime_Point3_Max )
				{
					if( theChantPoint3->GetBtnState() == n_bsNormal )
					{
						theChantPoint3->SetBtnState( n_bsFocus );
						theSuccessChantStatus = n_Chant_Start;
					}
				}
				else if( aChantTime >= c_Level6_ChantTime_Point4_Min && aChantTime <= c_Level6_ChantTime_Point4_Max )
				{
					if( theChantPoint4->GetBtnState() == n_bsNormal )
					{
						theChantPoint4->SetBtnState( n_bsFocus );
						theSuccessChantStatus = n_Chant_Start;
					}
				}
				else if( aChantTime >= c_Level6_ChantTime_Point5_Min && aChantTime <= c_Level6_ChantTime_Point5_Max )
				{
					if( theChantPoint5->GetBtnState() == n_bsNormal )
					{
						theChantPoint5->SetBtnState( n_bsFocus );
						theSuccessChantStatus = n_Chant_Start;
					}
				}

				if( theSuccessChantStatus == n_Chant_Start && 
						 ( theSuccessChantCnt < 1 ) && 
						 ( aChantTime > c_Level6_ChantTime_Point1_Max ) ) // && aChantTime < c_Level6_ChantTime_Point2_Min ) )
				{
					theSuccessChantStatus = n_Chant_Fail;
				}
				else if( theSuccessChantStatus == n_Chant_Start && 
						 ( theSuccessChantCnt < 2 ) && 
						 ( aChantTime > c_Level6_ChantTime_Point2_Max ) ) // && aChantTime < c_Level6_ChantTime_Point3_Min ) )
				{
					theSuccessChantStatus = n_Chant_Fail;
				}				
				else if( theSuccessChantStatus == n_Chant_Start && 
						 ( theSuccessChantCnt < 3 ) && 
						 ( aChantTime > c_Level6_ChantTime_Point3_Max ) ) // && aChantTime < c_Level6_ChantTime_Point4_Min ) )
				{
					theSuccessChantStatus = n_Chant_Fail;
				}				
				else if( theSuccessChantStatus == n_Chant_Start && 
						 ( theSuccessChantCnt < 4 ) && 
						 ( aChantTime > c_Level6_ChantTime_Point4_Max ) ) // && aChantTime < c_Level6_ChantTime_Point5_Min ) )
				{
					theSuccessChantStatus = n_Chant_Fail;
				}								
				else if( theSuccessChantStatus == n_Chant_Start && 
						 ( theSuccessChantCnt < 5 ) && 
						 ( aChantTime > c_Level6_ChantTime_Point5_Max ) ) 
				{
					theSuccessChantStatus = n_Chant_Fail;
				}
				else
				{
					if( theSuccessChantStatus == n_Chant_Start && 
						( theSuccessChantCnt == 0 ) && 
						theChantPoint1->GetBtnState() != n_bsNormal )
					{
						theChantPoint1->SetBtnState( n_bsNormal );
						theChantPoint2->SetBtnState( n_bsNormal );
						theChantPoint3->SetBtnState( n_bsNormal );
						theChantPoint4->SetBtnState( n_bsNormal );
						theChantPoint5->SetBtnState( n_bsNormal );
					}
				}
			}
			break;
		
		default:
			break;
		}
	}
	else if( theSuccessChantStatus == n_Chant_Fail )
	{
		theChantPoint1->SetBtnState( n_bsHold );
		theChantPoint2->SetBtnState( n_bsHold );
		theChantPoint3->SetBtnState( n_bsHold );
		theChantPoint4->SetBtnState( n_bsHold );
		theChantPoint5->SetBtnState( n_bsHold );
	}
	
	return;
}


void CActionSystem::SetComboGauge()
{
	int gauge = nRui->thePcParam.Stat1.theGage;
		theComboStepInfo_gauge->SetSkillGauge(gauge);
	theComboLevel = theComboStepInfo_gauge->GetNowLevel();

	// ���⼭ ��ó�� �׷����� �ʱ� ( ������ ������ ��Ʈ �Ǹ� �׷��ش� )
	if( nRui->thePcParam.MainClass != Archer ||
		nRui->thePcParam.MainClass != Sniper   )
		g_Pc.GetPlayer()->m_ComboEffectSet.SetComboEffect( theComboLevel );

/*	// ���� Progress Bar�� �׷��ֱ� ���ؼ�...
	int level_percent = 16.6 * level;	// (16.6 = 100 / 6)
	int gauge_percent = 0;

	if( gauge <= 15 ) gauge_percent = 50 * (gauge / 15);   // 50 �ۼ�Ʈ
	else if( gauge > 15 && gauge <= 35 )  gauge_percent = 50 + ( 33 * (gauge / 20) );	// 33 �ۼ�Ʈ 
	else if( gauge > 35 && gauge <= 50 )  gauge_percent = 83 + ( 17 * (gauge / 15) );	// 17 �ۼ�Ʈ 

	theComboLevel = level;
//	theComboGaugeInfo_level->SetPercent( level_percent );
	theComboGaugeInfo_gauge->SetPercent( gauge_percent );
*/

	return;
}

void CActionSystem::SetConditionGauge()
{
	int gauge = nRui->thePcParam.Stat1.theGage;

	theComboStepInfo_gauge->SetSkillGauge(gauge);
	theComboLevel = theComboStepInfo_gauge->GetNowLevel();

	g_Pc.GetPlayer()->m_ComboEffectSet.SetComboEffect( theComboLevel );

	return;
}

void CActionSystem::SetProimaGauge()
{
	int gauge = nRui->thePcParam.Stat1.theGage;

	theComboStepInfo_gauge->SetSkillGauge(gauge);
	theProimaLevel = theComboStepInfo_gauge->GetNowLevel();	

	g_Pc.GetPlayer()->m_ComboEffectSet.SetComboEffect( theProimaLevel );

	return;
}

// �ڵ尪�� ������ ������ ����. 
int	CActionSystem::GetSkillLevel( WORD aSkillCode )
{	
	if( aSkillCode == 0 )
		return -1;

	if( aSkillCode == c_SkillCode_BasicOndHandSwordShip || 
		aSkillCode == c_SkillCode_BasicProimaOfUnderstand )	
	{
		return 1;			// �ʱ� �˼��̳� �ʱ� �����̸��� ��� �޺� 1�������� ��� ���� ( ������ 3������ ���� )
	}

	int skill_level = -1;

	skill_level = aSkillCode % 10;	

	if( skill_level == 0 )
		skill_level = 10;

	return skill_level;
}


int	CActionSystem::GetNeedGageLevel( BYTE aNeedGage )
{
	if( aNeedGage == 0 )
		return -1;

	int gage_level = -1;

	switch( aNeedGage )
	{
	case 5:
		{
			gage_level = 1;
		}
		break;
	case 10:
		{
			gage_level = 2;
		}
		break;
	case 15:
		{
			gage_level = 3;
		}
		break;
	case 20:
		{
			gage_level = 4;
		}
		break;
	case 25:
		{
			gage_level = 5;
		}
		break;
	case 30:
		{
			gage_level = 5;
		}
		break;
	default:
		{
			gage_level = 1;
		}
		break;
	}

	return gage_level;
}

//by simwoosung - �޺���ų�� ���� �������� ���� �Ǵ�
bool CActionSystem::IsEnableComboSkill()
{
	int chrAttrIndex = g_Pc.GetPlayer()->GetAttrIndex();
	int chrClassType = nRui->thePcParam.MainClass;		

	if( 
		(	// �з��� ��-��, ������ ������, �ҵ��, ����Ʈ, �ӽóʸ�, �����̾�  
			( chrAttrIndex == n_Millena_Man || chrAttrIndex == n_Millena_Woman ) &&
			( chrClassType == Wanderer || 
			  chrClassType == Swordman || chrClassType == Knight || 
			  chrClassType == Mercenary || chrClassType == Slayer ) 
		) ||

		(	// ���� ��-��, ������ ��Ʈ������, �Ҽ���, ���ӳ�
			// ��Ʈ�������� ��쿡�� ���� �������� �������϶��� �����Ų��.
			( chrAttrIndex == n_Rain_Man || chrAttrIndex == n_Rain_Woman ) &&
			( ( chrClassType == Stranger ) ||			     		 
			  chrClassType == Shaman || chrClassType == Psyche ) 
		)
	 );
	 else
	 {
		return false;	
	 }	 
	
	if(!SetComboType())
		return false;

	int weapon_skill = 0;				// ������ ��ų �ɷ�ġ�� �¾ƾ� �޺� ��� ���� 
	SItemBaseInfo* weapon_info = NULL;	
	int combo_level = 1;		

	switch( theCurtComboType )
	{
	case nComboType_TwoHand_Sword:	
		{
			if( nRui->thePcInven.WearItem[n_WearSlot_Weapon] )
			{
				weapon_info = g_Pc_Manager.GetItemBaseInfoPtr( nRui->thePcInven.WearItem[n_WearSlot_Weapon]->Code );
				combo_level = GetSkillLevel( theComboSkillCode0 );
				if( combo_level == -1 )
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		break;
	case nComboType_OneHand_Sword:
	case nComboType_Sword_Shield:
		{
			if( nRui->thePcInven.WearItem[n_WearSlot_Weapon] )
			{
				weapon_info = g_Pc_Manager.GetItemBaseInfoPtr( nRui->thePcInven.WearItem[n_WearSlot_Weapon]->Code );
				combo_level = GetSkillLevel( theComboSkillCode2 );
				if( combo_level == -1 )
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		break;
	case nComboType_CrystalBall:
		{
			if( nRui->thePcInven.WearItem[n_WearSlot_Weapon] )
			{
				weapon_info = g_Pc_Manager.GetItemBaseInfoPtr( nRui->thePcInven.WearItem[n_WearSlot_Weapon]->Code );
				combo_level = GetSkillLevel( theComboSkillCode7 );
				if( combo_level == -1 )
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		break;
	case nComboType_MagicBook:
		{			
			if( nRui->thePcInven.WearItem[n_WearSlot_Shield] )
			{
				weapon_info = g_Pc_Manager.GetItemBaseInfoPtr( nRui->thePcInven.WearItem[n_WearSlot_Shield]->Code );
				combo_level = GetSkillLevel( theChantSkillCode );	
				if( combo_level == -1 )
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		break;
	case nComboType_Staff:
		{
			if( nRui->thePcInven.WearItem[n_WearSlot_Weapon] )
			{
				weapon_info = g_Pc_Manager.GetItemBaseInfoPtr( nRui->thePcInven.WearItem[n_WearSlot_Weapon]->Code );
				combo_level = GetSkillLevel( theChantSkillCode );	
				if( combo_level == -1 )
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		break;
	default:
		break;
	}
	
	if( theSuccessComboCnt >= (combo_level + 3) )
	{				
		return false;
	}			
	 
	int nowSP = GetNowSP();
	int useSP = GetUseSP();
	if( ((nowSP < useSP) || (nowSP <= 2)) && !(g_Pc.GetPlayer()->m_curt_event.count) )
	{
		return false;	
	}

	return true;
}

int	CActionSystem::GetEnableSkillLevel()		// ��밡���� ��ų���� ����Լ�
{
	static int nlevel = -1;
	
	if( theComboSkillCode0 == c_SkillCode_BasicOndHandSwordShip ||
		theComboSkillCode1 == c_SkillCode_BasicOndHandSwordShip ||
		theComboSkillCode2 == c_SkillCode_BasicOndHandSwordShip )
	{
		nlevel = 1;
	}
	else if( theChantSkillCode == c_SkillCode_BasicProimaOfUnderstand ||
			 theComboSkillCode7 == c_SkillCode_BasicProimaOfUnderstand )	
	{
		nlevel = 1;
	}    
	
	//���⼱ ����� ���°� ������
	if(theComboSkillCode0)	
	{
		nlevel = GetSkillLevel( theComboSkillCode0 );
	}	

	if(theComboSkillCode1)	
	{
		nlevel = GetSkillLevel( theComboSkillCode1 );
	}
	
	if(theComboSkillCode2)	
	{
		nlevel = GetSkillLevel( theComboSkillCode2 );	
	}

	if(theComboSkillCode7)
	{
		nlevel = GetSkillLevel( theComboSkillCode7 );
	}

	if(theChantSkillCode)
	{
		nlevel = GetSkillLevel( theChantSkillCode );
	}
	
	if( nlevel != -1 )
	{
		return nlevel;
	}

	return 0;
}				 


int CActionSystem::SetRandomTypeChantPercent( int aChantPercent, float aTimeRate )
{
	int chant_percent = aChantPercent;

	if( chant_percent == 0 )
	{
//		theCurtChantType = RandomNum( 0, 4 );
		++theCurtChantType;
		
		if( theCurtChantType >= 5 )
		{
			theCurtChantType = 0;
		}
	}	

	switch( theCurtChantType )
	{
	// type 1		���� ������ ���� 
	case 1: 
		{			
			chant_percent *= aTimeRate;		
		}
		break;

	// type 2		���� ������ ���� 
	case 2: 
		{			
			chant_percent *= aTimeRate * aTimeRate;		
		}
		break;
	
	// type 3		���� �������� ���̻� ���� �������� ���� 
	case 3: 
		{
			chant_percent *= aTimeRate * 2.0f;

			if( chant_percent >= 50 )
			{
				chant_percent /= aTimeRate * 2.0f;
			}
		}
		break;

	// type 4		�ݱ��� ���� �������ٰ� �ٽ� ���� ��Ȳ�������� ���� �������� ���� 
	case 4: 
		{
			chant_percent *= aTimeRate * 2.0f;

			if( chant_percent >= 50 )
			{
				chant_percent = 50 + ( ( chant_percent - 50 ) * ( aTimeRate - 0.5f ) * 2.0f );
			}
		}
		break;

	default:
		break;
	}

	return chant_percent;
}


int CActionSystem::GetChantLevel( int aSpellingTime )
{
	if( aSpellingTime >= 500 && aSpellingTime < 1000 )
	{
		return 1;
	}	
	else if( aSpellingTime >= 1000 && aSpellingTime < 1500 )
	{
		return 2;
	}
	else if( aSpellingTime >= 1500 && aSpellingTime < 2000 )
	{
		return 3;
	}
	else if( aSpellingTime >= 2000 && aSpellingTime < 2500 )
	{
		return 4;
	}
	else if( aSpellingTime >= 2500 && aSpellingTime < 3000 )
	{
		return 5;
	}
	else 
	{
		return 0;
	}

	return 0;
}
