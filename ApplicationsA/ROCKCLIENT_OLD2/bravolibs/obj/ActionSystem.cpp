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
	theSuccessChantStatus = n_Chant_Start;		// test 니까...나중에는 스킬을 쓰면 start !!!	
	
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
// Desc: NPC 속성주기
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
	
	char* token;				// 토큰
	char string [256];			// 문서 줄단위 저장 변수
	bool bRead  = true;			// 문서 다 읽었는지 체크			
	char splitter[] = " \n\t";	// 구분자 : /, 캐리지리턴, 탭 
	int	 seq_cnt = 0;			// 시퀀스 카운트 
	int	 type_cnt = 0;			// 콤보 타입 카운트 
	int	 num = -1;				// 콤보 타입 카운트 
			
	// 줄 단위로 읽기
	if( fgets( string, 256, file ) == NULL )				
			return false;				
	do 
	{	
		// 주석이 있다면 다음 줄 읽음
		while( string[0] == '/' && string[1] == '/' )	
		{				
			// 더 이상 읽은게 없다면 루프 종료
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

		// 인덱스 
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
		
		// 다음 줄 읽기, 읽은게 없다면 string은 0으로									
		if( fgets( string, 256 , file ) == NULL )		
			bRead = false;			
	}
	while( bRead != false ); // 더 이상 읽은게 없다면 루프 종료	
	fclose( file );	


///////////////////////////////////////////////////////////////////////////////////////////////////	
#else  ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
	
	VFileHandle* pFH = g_VFChrRoot.OpenFile( "Combo_Seq.dat" );
	if( pFH == NULL ) return false;
	
	char* token;				// 토큰
	char string [256];			// 문서 줄단위 저장 변수
	bool bRead  = true;			// 문서 다 읽었는지 체크			
	char splitter[] = " \n\t";	// 구분자 : /, 캐리지리턴, 탭 
	int	 seq_cnt = 0;			// 시퀀스 카운트 
	int	 type_cnt = 0;			// 콤보 타입 카운트 
	int	 num = -1;				// 콤보 타입 카운트 
			
	// 줄 단위로 읽기
	if( pFH->Gets( string, sizeof( string ) ) == 0 )
	{
		g_VFChrRoot.CloseFile( pFH );
		return false; 
	}			
	do 
	{	
		// 주석이 있다면 다음 줄 읽음
		while( string[0] == '/' && string[1] == '/' )	
		{	
			// 더 이상 읽은게 없다면 루프 종료
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

		// 인덱스 
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
		
		// 다음 줄 읽기, 읽은게 없다면 string은 0으로									
		if( pFH->Gets( string, sizeof( string ) ) == 0 )
			bRead = false;			
	}
	while( bRead != false ); // 더 이상 읽은게 없다면 루프 종료	

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
		(	// 밀레나 남-녀, 직업은 원더러, 소드맨, 나이트, 머시너리, 슬레이어  
			( chrAttrIndex == n_Millena_Man || chrAttrIndex == n_Millena_Woman ) &&
			( chrClassType == Wanderer ||
			  chrClassType == Swordman || chrClassType == Knight || chrClassType == Mercenary || 
			  chrClassType == Slayer ) 
		) ||
		(	// 레인 남-녀, 직업은 스트레인저, 소서러, 서머너
			// 스트레인져일 경우에는 가진 아이템이 수정구일때만 적용시킨다.
			( chrAttrIndex == n_Rain_Man || chrAttrIndex == n_Rain_Woman ) &&
			( ( chrClassType == Stranger ) ||			     		 
			  chrClassType == Shaman || chrClassType == Psyche ) 
		)
	  )	
	{
		//////////////////////////////////////////////////////////////////////////////////////
		// 콤보 공격을 위한 키 프로세스 Z, X, C
		//////////////////////////////////////////////////////////////////////////////////////
		if( g_Pc.GetPlayer()->m_curt_event.type == SM_ATTACK_NPC ||
			g_Pc.GetPlayer()->m_curt_event.type == SM_USE_SKILL )
		{			
			if( g_Pc.GetPlayer()->m_next_event.type != SM_USE_SKILL )
			{			
				///-- 자동콤보가 아닐경우
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
	
    // 마법계 시전 딜레이 동작  
	ActionProcChantCombo();	
	
	return;
}

void CActionSystem::ActionProcChantCombo()
{
	int chrAttrIndex = g_Pc.GetPlayer()->GetAttrIndex();
	int chrClassType = nRui->thePcParam.MainClass;	
	
	// 마법계 
	if(		// 레인 남-녀, 직업은 스트레인저, 소서러, 서머너 
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

	int nCutType = RandomNum(1, 3);	///-- 베기 타입 결정 - 가로베기, 세로베기, 찌르기
	nKey = nCutType * 10;

	int nCutMotion = RandomNum(1, 5); ///-- 베기 모션 결정
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
// 기본 공격 성공후에 콤보 기술이 들어간다.
// 종족 또는 직업군에 따라 전투 시스템이 달라진다. 
// 키가 눌려진 콤보 타이밍 범위안에 있는지 검사를 하여 콤보 생성 여부 확인 
// => 일단 현재 무기 능력치에 맞는 콤보 시퀀스 키가 눌려졌는지 검사 한다 
//			(연결되는 다음 콤보의 레벨이 무기 레벨에 맞지 않으면 콤보 실패) 
//			(맨처음 눌려진 키 타입에 따라서 해당되는 여러 콤보 시퀀스로 나뉘어 진다)
// => 콤보 프레임 범위안에 있는지 현재 애니 프레임 검사해서 콤보 타이밍인지 확인 
// ===> 다음 이벤트에 저장되어 있는지 확인후 없다면 다음 이벤트에 공격 이벤트를 저장한다 
// ===> 다음 이벤트가 저장되어 있는 상태에서 엉망 키 입력이 들어온다면 공격 이벤트 취소한다 
// ======> 이벤트 저장시에 콤보에 사용되는 콤보게이지 및 SP 수치가 모자르면 공격 취소한다 
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

// if( chrAttrIndex == 밀레나 ) && if( chrClass == 소드맨, 나이트, 머시너리, 슬레이어 (남-여) )
// if( chrAttrIndex == 레인 )  && if( chrClass == 소서러, 서머너 (남-여) (추후 지원) )
void CActionSystem::ComboSystem( int aPressKey )
{	
	// 부가효과 (콤보불가) 처리 
	
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

//	DebugPrint("\n콤보 상태 theCombo_State = %d\n", g_Pc.GetPlayer()->theCombo_State);	

	if( g_Pc.GetPlayer()->theCombo_State == n_Combo_Success || 			// 성공 했으면 더 계산할 필요 없잖아?!!
		g_Pc.GetPlayer()->theCombo_State == n_Combo_Disable || 			// 한번 실패는 끝...!!
		g_Pc.GetPlayer()->theCombo_State == n_Combo_Skill_Enable )		// 콤보는 불가능, 스킬은 가능!! 
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
			// 플레이어가 현재 착용중인 무기 아이템을 검사해서 콤보 타입 결정 (추후 인벤토리 완성후 재수정)
			if(!bFlag)
			{
				return;
			}

			int combo_motion = -1;
					
			//전직 하였다면 등록된 키값을 보여줌
			if( (nRui->thePcParam.MainClass != Wanderer) && ( nRui->thePcParam.MainClass != Stranger) &&
				g_Pc.GetPlayer()->theActionSystem.theTotComboStep > 4 )
			{
				combo_motion = GetRandomComboAniIndex( theSuccessComboCnt, aPressKey );
			}
			else
			{
				combo_motion = GetComboAniIndex( theSuccessComboCnt, aPressKey ); 
			}			
		
			if( combo_motion == -1 )						// 콤보 실패 
				combo_result = false;		
			
			if( combo_result )
			{
				EVENT_DATA_INFO attack_event;
				attack_event = g_Pc.GetPlayer()->m_curt_event;
				attack_event.motion = combo_motion;
				attack_event.critical = theSuccessComboKeyType;		// 콤보 키 타입 
				attack_event.count = theSuccessComboCnt;			// 콤보 카운트 
				attack_event.status = theSuccessComboStatus;		// 콤보 시퀀스 완성 
				attack_event.number = g_Pc.theSkillSystem.theShortCut_SkillCode[2];
				
#ifdef AUTO_COMBO
				if( theSuccessComboStatus == n_ComboComplete && 
				    theSuccessComboCnt == c_Max_ComboSeqNum )
				{
					m_bActAutoCombo = TRUE;
				}
#endif
				////-- 콤보도중에 콤보대상이 바뀌면.. 콤보는 초기화된다
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
					//PRINT_DLG_LOG(" 이놈은 죽은 놈인데...");
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
	// 부가효과 (콤보불가) 처리 
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
	if( g_Pc.GetPlayer()->theCombo_State == n_Combo_Success || 			// 성공 했으면 더 계산할 필요 없잖아?!!
		g_Pc.GetPlayer()->theCombo_State == n_Combo_Disable || 			// 한번 실패는 끝...!!
		g_Pc.GetPlayer()->theCombo_State == n_Combo_Skill_Enable )		// 콤보는 불가능, 스킬은 가능!! 
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
		if( curt_frame >= range_startframe + 1 )	///-- 현재 프레임이 시작프레임보다 1이상이면 자동콤보 시작	
		{
			bool combo_result = true;
			
			bool bFlag = SetComboType();	// 플레이어가 현재 착용중인 무기 아이템을 검사해서 콤보 타입 결정 (추후 인벤토리 완성후 재수정)
			if(!bFlag)
			{
				m_bActAutoCombo = FALSE;
				return;
			}

			int aPressKey = 0;

			///-- 현재 등록된 키값을 프레스 키로 자동적으로 보내준다.
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
			
			int combo_motion = GetComboAniIndex( theSuccessComboCnt, aPressKey ); // 콤보 성공 여부 판단 할 실제 애니메이션 인덱스 
			
			if( combo_motion == -1 )						// 콤보 실패 
				combo_result = false;		
			
			if( combo_result )
			{
				EVENT_DATA_INFO attack_event;
				attack_event = g_Pc.GetPlayer()->m_curt_event;
				attack_event.motion = combo_motion;
				attack_event.critical = theSuccessComboKeyType;		// 콤보 키 타입 
				attack_event.count = theSuccessComboCnt;			// 콤보 카운트 
				attack_event.status = theSuccessComboStatus;		// 콤보 시퀀스 완성 
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

// 간접 전투계 
// if( chrAttrIndex == 밀레나 ) && if( chrClass == 아처, 소울 브레이커 ) (추후 지원)
void CActionSystem::TimingHitSystem()
{
	// 컨디션 게이지 
	return;
}

// 영창 스킬 시전할때 서버에 알려주고 받은다음 시작 (영창 모션 시작)
// 영창 콤보 포인트 히트 성공시 마다 서버에 알려준다 (모션을 바꿔주며 다른사람들에게 방송)
// 영창 스킬 끝날때 서버에 알려주고 받은다음 영창 게이지를 올려준다 (영창 성공)
// 마법계 
// if( chrAttrIndex == 레인 ) && if( chrClass == 매지션, 프로미나스, 프리스트, 호리 어벤져 ) (추후 지원)
void CActionSystem::ChantComboSystem( bool aChantCombo )
{
	// 부가효과 (콤보불가) 처리 

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


	//캐릭터가 움직이면 영창 취소
	
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

	///-- 공격속도에 따른 영창딜레이의 변화
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
		
		///-- 하드 플레이시... Send_RpCsSpelling 함수를 두번보낸다.
		
		++theSuccessChantCnt;	// 성공 시킨 카운트만 보내준다 
		g_RockClient.Send_RpCsSpelling( theSuccessChantCnt ); 
		theSuccessChantStatus = n_Chant_Complete;
		g_Pc.GetPlayer()->m_curt_event.loop = false;

		nRui->SendEvent( WID_ChantHitBar_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );

		g_Pc.GetPlayer()->m_SpellEffect.Clear();
		g_Pc.GetSkillSystem()->theUseSpellSkill = FALSE;
	}
	
	thePreTime = g_nowTime;
	
	// 영창 시스템 -> 콤보 시스템으로 변경 
	if( aChantCombo )
	{
		SetChantState( chant_level, chant_percent );
	}
		
	return;
}

void CActionSystem::ArcherComboSystem()
{
	// 부가효과 (콤보불가) 처리 	
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

	///-- 공격속도에 따른 영창딜레이의 변화
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

	if( RHand_index != -1 && LHand_index == -1 )				// 오른손 무기만 장착
	{		
		if( g_Pc_Manager.m_pItm[RHand_index]->m_type == 0 )		// 대검 
		{
			theCurtComboType = nComboType_TwoHand_Sword; 
		}
		else if( g_Pc_Manager.m_pItm[RHand_index]->m_type == 2 ) // 한손검 
		{
			theCurtComboType = nComboType_OneHand_Sword; 
		}
		else if( g_Pc_Manager.m_pItm[RHand_index]->m_type == 6 ) // 지팡이 
		{
			theCurtComboType = nComboType_Staff; 
		}
		else if( g_Pc_Manager.m_pItm[RHand_index]->m_type == 7 ) // 흑수정
		{
			theCurtComboType = nComboType_CrystalBall;	
		}
	}
	else if( RHand_index == -1 && LHand_index != -1 )			// 왼손 무기만 장착
	{
		if( g_Pc_Manager.m_pItm[LHand_index]->m_type == 1 )		// 활 
		{
			theCurtComboType = nComboType_OneHand_Sword;
			IsArcher = true;
		}
		else if( g_Pc_Manager.m_pItm[LHand_index]->m_type == 3 ) // 방패 
		{
			theCurtComboType = nComboType_OneHand_Sword;		// 방패만 있으면 평화모드 
		}
		else if( g_Pc_Manager.m_pItm[LHand_index]->m_type == 5 ) // 마법서 
		{
			theCurtComboType = nComboType_MagicBook; 
		}
	}
	else if( RHand_index != -1 && LHand_index != -1 )			// 양손에 무기 장착
	{
		if( g_Pc_Manager.m_pItm[RHand_index]->m_type == 2 && 
			g_Pc_Manager.m_pItm[LHand_index]->m_type == 3 )		// 한손검 + 방패
		{
			theCurtComboType = nComboType_Sword_Shield; 
		}
	}
	else														// 무기 없음 (맨손 공격)
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
	int weapon_skill = 0;				// 무기의 스킬 능력치에 맞아야 콤보 사용 가능 
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
		// Log에 기록되어 있는 시퀀스만 사용할 수 있다. 
		for( num = 0; num < c_Max_ComboNum; ++num )
		{
			if( theCombo_Info[theCurtComboType].combo_enable[num] )
			{
				theCombo_Info[theCurtComboType].combo_enable[num] = false;
				
				int nRegNum = g_Pc.theSkillSystem.theShortCut_SkillCode[2] - 1;
				
				///-- 현재 등록된 콤보만 사용가능하다.
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
				weapon_skill = c_ComboZKey1 + (c_MaxComboItemLevel - weapon_info->theZAttack);		// 무기의 Z 사용 능력치에 맞아야 콤보 가능 

				if( combo_key >= c_ComboZKey1 && combo_key <= weapon_skill )
//				if( combo_key >= c_ComboZKey1 && combo_key <= c_ComboZKey5 )
				{
					success_combo = true;
				}
			}
			else if( aPressKey == DIK_X )
			{
				weapon_skill = c_ComboXKey1 + (c_MaxComboItemLevel - weapon_info->theXAttack);		// 무기의 X 사용 능력치에 맞아야 콤보 가능 

				if( combo_key >= c_ComboXKey1 && combo_key <= weapon_skill )
				//if( combo_key >= c_ComboXKey1 && combo_key <= c_ComboXKey5 )
				{
					success_combo = true;
				}
			}
			else if( aPressKey == DIK_C )
			{
				weapon_skill = c_ComboCKey1 + (c_MaxComboItemLevel - weapon_info->theCAttack);		// 무기의 C 사용 능력치에 맞아야 콤보 가능 

				if( combo_key >= c_ComboCKey1 && combo_key <= weapon_skill )
				//if( combo_key >= c_ComboCKey1 && combo_key <= c_ComboCKey5 )
				{
					success_combo = true;
				}
			}

			if( success_combo )				// 무기 및 시퀀스에 따른 콤보 성공 
			{
				///-- By simwoosung
				///-- 일단 애니메이션 동작은 등록된 키에 우선순위를 더둔다.				
				if( !success_combo_key )  ///-- 아직 콤보키가 지정되어 있지 않으면
				{
					success_combo_key = combo_key;		///-- 지정					
					success_combo_num = num;
				}
				else	///-- 콤보키가 현재 지정되어있다면
				{
					///-- 현재 등록된 콤보키냐를 따져서 애니메이션을 지정콤보키를 바꿔준다.
					if( g_Pc.theSkillSystem.theShortCut_SkillCode[2] ==  (num + 1) )
					{
					   	success_combo_key = combo_key;		///-- 재지정					
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

	///-- By simwoosung - 콤보가 분기되는것이 여기에 두어야 실행가능하게됨...
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
//		if( nRui->thePcParam.Stat1.theSP >= useSP )		// 소모되는 SP 수치에 맞는 SP 가 있다면 콤보 성공!!!
		if( nowSP >= useSP )		// 소모되는 SP 수치에 맞는 SP 가 있다면 콤보 성공!!!			
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
	// 8개의 콤보 시퀀스가 완성했거나 성공한 시퀀스 다음 시퀀스가 0일 경우에 콤보 완성!!!
	if( (aSequenceNum >= c_Max_ComboSeqNum) || 
		(theCombo_Info[theCurtComboType].combo_sequence[success_combo_num][aSequenceNum] == 0) ||
		( ( aSequenceNum >= (combo_level + 3) ) &&  
		( (nRui->thePcParam.MainClass != Wanderer) && ( nRui->thePcParam.MainClass != Stranger) ) )
	  )	//by simwoosung - 콤보 가능시퀀스가 완성될때도(단 전직을 했을경우에만...왠 하드코딩 ㅠ.ㅠ) 컴플리트를 보냄
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
	int weapon_skill = 0;				// 무기의 스킬 능력치에 맞아야 콤보 사용 가능 
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
		weapon_skill = c_ComboZKey1 + (c_MaxComboItemLevel);		// 무기의 Z 사용 능력치에 맞아야 콤보 가능 

		if( combo_key >= c_ComboZKey1 && combo_key <= weapon_skill )
		{
			success_combo = true;
		}
	}
	else if( aPressKey == DIK_X )
	{
		weapon_skill = c_ComboXKey1 + (c_MaxComboItemLevel);		// 무기의 X 사용 능력치에 맞아야 콤보 가능 

		if( combo_key >= c_ComboXKey1 && combo_key <= weapon_skill )
		{
			success_combo = true;
		}
	}
	else if( aPressKey == DIK_C )
	{
		weapon_skill = c_ComboCKey1 + (c_MaxComboItemLevel);		// 무기의 C 사용 능력치에 맞아야 콤보 가능 

		if( combo_key >= c_ComboCKey1 && combo_key <= weapon_skill )
		{
			success_combo = true;
		}
	}

	if( success_combo )				// 무기 및 시퀀스에 따른 콤보 성공 
	{
		success_combo_key = combo_key;		///-- 지정					
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
		if( nowSP >= useSP )		// 소모되는 SP 수치에 맞는 SP 가 있다면 콤보 성공!!!			
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

	// 8개의 콤보 시퀀스가 완성했거나 성공한 시퀀스 다음 시퀀스가 0일 경우에 콤보 완성!!!
	if( aSequenceNum == theTotComboStep ) 
	{
		theSuccessComboStatus = n_ComboComplete; 
		theSuccessComboSeq = success_combo_num;
	}

	return ani_index;
}

bool CActionSystem::SetComboType()
{
	// 현재 무기에 따라 결정 
	theCurtComboType = nComboType_OneHand_Sword;
	SItemBaseInfo* weapon_info = NULL;

	int RHand_index = g_Pc.GetPlayer()->m_pUserInfo->ArmorInfo.index[0];
	int LHand_index = g_Pc.GetPlayer()->m_pUserInfo->ArmorInfo.index[1];

	///--무기를 근접무기를 보유하고 있을때
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

	if( RHand_index != -1 && LHand_index == -1 )				// 오른손 무기만 장착
	{		
		if( g_Pc_Manager.m_pItm[RHand_index]->m_type == 0 )		// 대검 
		{
			theCurtComboType = nComboType_TwoHand_Sword; 
		}
		else if( g_Pc_Manager.m_pItm[RHand_index]->m_type == 2 ) // 한손검 
		{
			theCurtComboType = nComboType_OneHand_Sword; 
		}
		else if( g_Pc_Manager.m_pItm[RHand_index]->m_type == 6 ) // 지팡이 
		{
			theCurtComboType = nComboType_Staff; 
		}
		else if( g_Pc_Manager.m_pItm[RHand_index]->m_type == 7 ) // 흑수정
		{
			theCurtComboType = nComboType_CrystalBall;	
		}
	}
	else if( RHand_index == -1 && LHand_index != -1 )			// 왼손 무기만 장착
	{
		if( g_Pc_Manager.m_pItm[LHand_index]->m_type == 1 )		// 활 
		{
			theCurtComboType = nComboType_OneHand_Sword; 
		}
		else if( g_Pc_Manager.m_pItm[LHand_index]->m_type == 3 ) // 방패 
		{
			theCurtComboType = nComboType_OneHand_Sword;		// 방패만 있으면 평화모드 
		}
		else if( g_Pc_Manager.m_pItm[LHand_index]->m_type == 5 ) // 마법서 
		{
			theCurtComboType = nComboType_MagicBook; 
		}
	}
	else if( RHand_index != -1 && LHand_index != -1 )			// 양손에 무기 장착
	{
		if( g_Pc_Manager.m_pItm[RHand_index]->m_type == 2 && 
			g_Pc_Manager.m_pItm[LHand_index]->m_type == 3 )		// 한손검 + 방패
		{
			theCurtComboType = nComboType_Sword_Shield; 
		}
	}
	else														// 무기 없음 (맨손 공격)
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
		useSP = theSuccessComboCnt - 2;		// 콤보 수 - 2 만큼 소모
	}

	return useSP;
}

int CActionSystem::GetNowSP()
{
	int chrAttrIndex = g_Pc.GetPlayer()->GetAttrIndex();
	int chrClassType = nRui->thePcParam.MainClass;
	int nowSP = 0;


	if(	// 밀레나 남-녀, 직업은 원더러, 소드맨, 나이트, 머시너리, 슬레이어  
		( chrAttrIndex == n_Millena_Man || chrAttrIndex == n_Millena_Woman ) &&
		( chrClassType == Wanderer || 
		  chrClassType == Swordman || chrClassType == Knight || chrClassType == Mercenary || chrClassType == Slayer ) 
	  ) 
	{
		nowSP = nRui->thePcParam.Stat1.theSP;
	}
	else if( // 레인 남-녀, 직업은 스트레인저, 소서러, 서머너 
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
	
	//영창할떄 캐릭이 이동하면 .. 취소하라 .. ㅋㅋ 
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
	// 영창 포인트에 맞게 theChantTimeBar를 확인하여 DIK_SPACE 키를 눌러서 영창게이지를 채운다 
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
//					g_Pc.GetPlayer()->AnimationPlay( g_Pc.GetPlayer()->SetChantMotion( SM_CHANT_SPELLING, theSuccessChantCnt ) );		// 영창 모션이 없어서 테스트...	
					
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

	// 여기서 아처는 그려주지 않기 ( 게이지 내에서 히트 되면 그려준다 )
	if( nRui->thePcParam.MainClass != Archer ||
		nRui->thePcParam.MainClass != Sniper   )
		g_Pc.GetPlayer()->m_ComboEffectSet.SetComboEffect( theComboLevel );

/*	// 실제 Progress Bar를 그려주기 위해서...
	int level_percent = 16.6 * level;	// (16.6 = 100 / 6)
	int gauge_percent = 0;

	if( gauge <= 15 ) gauge_percent = 50 * (gauge / 15);   // 50 퍼센트
	else if( gauge > 15 && gauge <= 35 )  gauge_percent = 50 + ( 33 * (gauge / 20) );	// 33 퍼센트 
	else if( gauge > 35 && gauge <= 50 )  gauge_percent = 83 + ( 17 * (gauge / 15) );	// 17 퍼센트 

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

// 코드값을 가지고 레벨을 얻어낸다. 
int	CActionSystem::GetSkillLevel( WORD aSkillCode )
{	
	if( aSkillCode == 0 )
		return -1;

	if( aSkillCode == c_SkillCode_BasicOndHandSwordShip || 
		aSkillCode == c_SkillCode_BasicProimaOfUnderstand )	
	{
		return 1;			// 초급 검술이나 초급 프로이마의 경우 콤보 1레벨까지 사용 가능 ( 시퀀스 3개까지 가능 )
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

//by simwoosung - 콤보스킬이 현재 가능한지 여부 판단
bool CActionSystem::IsEnableComboSkill()
{
	int chrAttrIndex = g_Pc.GetPlayer()->GetAttrIndex();
	int chrClassType = nRui->thePcParam.MainClass;		

	if( 
		(	// 밀레나 남-녀, 직업은 원더러, 소드맨, 나이트, 머시너리, 슬레이어  
			( chrAttrIndex == n_Millena_Man || chrAttrIndex == n_Millena_Woman ) &&
			( chrClassType == Wanderer || 
			  chrClassType == Swordman || chrClassType == Knight || 
			  chrClassType == Mercenary || chrClassType == Slayer ) 
		) ||

		(	// 레인 남-녀, 직업은 스트레인저, 소서러, 서머너
			// 스트레인져일 경우에는 가진 아이템이 수정구일때만 적용시킨다.
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

	int weapon_skill = 0;				// 무기의 스킬 능력치에 맞아야 콤보 사용 가능 
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

int	CActionSystem::GetEnableSkillLevel()		// 사용가능한 스킬레벨 얻기함수
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
	
	//여기선 기술로 가는게 낳을듯
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
	// type 1		점점 빠르게 설정 
	case 1: 
		{			
			chant_percent *= aTimeRate;		
		}
		break;

	// type 2		점점 빠르게 설정 
	case 2: 
		{			
			chant_percent *= aTimeRate * aTimeRate;		
		}
		break;
	
	// type 3		점점 빨라지다 반이상엔 점점 느려지게 설정 
	case 3: 
		{
			chant_percent *= aTimeRate * 2.0f;

			if( chant_percent >= 50 )
			{
				chant_percent /= aTimeRate * 2.0f;
			}
		}
		break;

	// type 4		반까지 점점 빨라지다가 다시 늦은 상황에서부터 점점 빨라지게 설정 
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
