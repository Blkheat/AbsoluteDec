   //.................................................................................................................
//
//  CDamageEffect.cpp																					By wxywxy
//
//  데미지 처리시 이펙트 / 사운드 처리
//.................................................................................................................

#include "RockPCH.h"
#include "VFileSystem.h"
#include	<NETWORK\Protocol.h>
#include	"..\\quadlist.h"
#include	<BASE\\D3DBASE.H>
#include	"..\\RockClient.h"
#include	"..\\PC.h"
#include	"..\\Camera.h"
#include	"..\\Map.h"
#include	"ObjBase.H"
#include	"..\\bravolibs\\obj\\Character_Item.H"
#include	"..\\bravolibs\\obj\\Player_Manager.H"
#include	 "..\\bravolibs\\obj\\Line_Vertex.H"
#include	"CDamageEffect.h"
#include	"..\\Effect.h"
#include	<stdio.h>
#include	<assert.H>
#include	<SOUND\Sound.H>
#include	"CTextCommand.h"

CDamageEffect::CDamageEffect()
{
}

CDamageEffect::~CDamageEffect()
{ 
}

//.................................................................................................................
// 데미지 효과 세트 로딩
//.................................................................................................................
int CDamageEffect::Load(char *Path)
{
	return TRUE;

	CTextCommand TextCmd;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined( LOAD_VF ) /////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	char	FullName[ 1024 ] = "";

	wsprintf( FullName , "%s\\%s" , Path , DAMAGE_EFFECT_FILENAME );

	if( TextCmd.Open( FullName ) == NULL ) 
	{
		DebugPrint( "Error Loading DamageEffect.Dat\n" );
		
		return FALSE;
	}

///////////////////////////////////////////////////////////////////////////////////////////////////	
#else  ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

	if( TextCmd.OpenVF( DAMAGE_EFFECT_FILENAME ) == NULL ) 
	{
		DebugPrint( "Error Loading DamageEffect.Dat\n" );
		
		return FALSE;
	}

///////////////////////////////////////////////////////////////////////////////////////////////////	
#endif ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

	NPC_DEMAGE_SET	TempSet;

	TempSet.Clear();

	while( TextCmd.GetLine() != NULL )
	{
		if( TextCmd.ISArgEmpty() ) continue;

		//..........................................................................................................
		// 전사(남)
		//..........................................................................................................
		if( TextCmd.EqualArgToString( 0 , "[PLAYER1]" ) )
		{
			TextCmd.GetArgValue( 1 , m_UserSet[ 0 ].EffectNum[ 0 ] );
			TextCmd.GetArgValue( 2 , m_UserSet[ 0 ].LifeTime [ 0 ] );

			TextCmd.GetArgValue( 3 , m_UserSet[ 0 ].EffectNum[ 1 ] );
			TextCmd.GetArgValue( 4 , m_UserSet[ 0 ].LifeTime [ 1 ] );

			TextCmd.GetArgValue( 5 , m_UserSet[ 0 ].EffectNum[ 2 ] );
			TextCmd.GetArgValue( 6 , m_UserSet[ 0 ].LifeTime [ 2 ] );

			TextCmd.GetArgValue( 7 , m_UserSet[ 0 ].SoundNum [ 0 ] );
			TextCmd.GetArgValue( 8 , m_UserSet[ 0 ].SoundNum [ 1 ] );
			TextCmd.GetArgValue( 9 , m_UserSet[ 0 ].SoundNum [ 2 ] );
		}
		else
		//..........................................................................................................
		// 전사(여)
		//..........................................................................................................
		if( TextCmd.EqualArgToString( 0 , "[PLAYER2]" ) )
		{
			TextCmd.GetArgValue( 1 , m_UserSet[ 1 ].EffectNum[ 0 ] );
			TextCmd.GetArgValue( 2 , m_UserSet[ 1 ].LifeTime [ 0 ] );

			TextCmd.GetArgValue( 3 , m_UserSet[ 1 ].EffectNum[ 1 ] );
			TextCmd.GetArgValue( 4 , m_UserSet[ 1 ].LifeTime [ 1 ] );

			TextCmd.GetArgValue( 5 , m_UserSet[ 1 ].EffectNum[ 2 ] );
			TextCmd.GetArgValue( 6 , m_UserSet[ 1 ].LifeTime [ 2 ] );

			TextCmd.GetArgValue( 7 , m_UserSet[ 1 ].SoundNum [ 0 ] );
			TextCmd.GetArgValue( 8 , m_UserSet[ 1 ].SoundNum [ 1 ] );
			TextCmd.GetArgValue( 9 , m_UserSet[ 1 ].SoundNum [ 2 ] );
		}
		else
		//..........................................................................................................
		// 법사(남)
		//..........................................................................................................
		if( TextCmd.EqualArgToString( 0 , "[PLAYER3]" ) )
		{
			TextCmd.GetArgValue( 1 , m_UserSet[ 2 ].EffectNum[ 0 ] );
			TextCmd.GetArgValue( 2 , m_UserSet[ 2 ].LifeTime [ 0 ] );

			TextCmd.GetArgValue( 3 , m_UserSet[ 2 ].EffectNum[ 1 ] );
			TextCmd.GetArgValue( 4 , m_UserSet[ 2 ].LifeTime [ 1 ] );

			TextCmd.GetArgValue( 5 , m_UserSet[ 2 ].EffectNum[ 2 ] );
			TextCmd.GetArgValue( 6 , m_UserSet[ 2 ].LifeTime [ 2 ] );

			TextCmd.GetArgValue( 7 , m_UserSet[ 2 ].SoundNum [ 0 ] );
			TextCmd.GetArgValue( 8 , m_UserSet[ 2 ].SoundNum [ 1 ] );
			TextCmd.GetArgValue( 9 , m_UserSet[ 2 ].SoundNum [ 2 ] );
		}
		else
		//..........................................................................................................
		// 법사(여)
		//..........................................................................................................
		if( TextCmd.EqualArgToString( 0 , "[PLAYER4]" ) )
		{
			TextCmd.GetArgValue( 1 , m_UserSet[ 3 ].EffectNum[ 0 ] );
			TextCmd.GetArgValue( 2 , m_UserSet[ 3 ].LifeTime [ 0 ] );

			TextCmd.GetArgValue( 3 , m_UserSet[ 3 ].EffectNum[ 1 ] );
			TextCmd.GetArgValue( 4 , m_UserSet[ 3 ].LifeTime [ 1 ] );

			TextCmd.GetArgValue( 5 , m_UserSet[ 3 ].EffectNum[ 2 ] );
			TextCmd.GetArgValue( 6 , m_UserSet[ 3 ].LifeTime [ 2 ] );

			TextCmd.GetArgValue( 7 , m_UserSet[ 3 ].SoundNum [ 0 ] );
			TextCmd.GetArgValue( 8 , m_UserSet[ 3 ].SoundNum [ 1 ] );
			TextCmd.GetArgValue( 9 , m_UserSet[ 3 ].SoundNum [ 2 ] );
		}
		else
		//..........................................................................................................
		// TYPE
		//..........................................................................................................
		if( TextCmd.EqualArgToString( 0 , "[TYPE]" ) )
		{
			TextCmd.GetArgValue( 1 , TempSet.Type );
		}
		else
		//..........................................................................................................
		// [HIT1]
		//..........................................................................................................
		if( TextCmd.EqualArgToString( 0 , "[HIT1]" ) )
		{
			TextCmd.GetArgValue( 1 , TempSet.EffectNum[ 0 ] );
			TextCmd.GetArgValue( 2 , TempSet.LifeTime [ 0 ] );
			TextCmd.GetArgValue( 3 , TempSet.SoundNum [ 0 ] );
		}
		else
		//..........................................................................................................
		// [HIT2]
		//..........................................................................................................
		if( TextCmd.EqualArgToString( 0 , "[HIT2]" ) )
		{
			TextCmd.GetArgValue( 1 , TempSet.EffectNum[ 1 ] );
			TextCmd.GetArgValue( 2 , TempSet.LifeTime [ 1 ] );
			TextCmd.GetArgValue( 3 , TempSet.SoundNum [ 1 ] );
		}
		else
		//..........................................................................................................
		// [HIT3]
		//..........................................................................................................
		if( TextCmd.EqualArgToString( 0 , "[HIT3]" ) )
		{
			TextCmd.GetArgValue( 1 , TempSet.EffectNum[ 2 ] );
			TextCmd.GetArgValue( 2 , TempSet.LifeTime [ 2 ] );
			TextCmd.GetArgValue( 3 , TempSet.SoundNum [ 2 ] );
		}
		else
		//..........................................................................................................
		// [END]
		//..........................................................................................................
		if( TextCmd.EqualArgToString( 0 , "[END]" ) )
		{
			m_NpcSet.push_back( TempSet );

			TempSet.Clear();
		}
	}

	TextCmd.Close();

	return TRUE;
}

//.................................................................................................................
// 타입 찾기
//.................................................................................................................
int CDamageEffect::FindWeaponType(int WeaponType)
{
	std::vector	<NPC_DEMAGE_SET>::iterator i = m_NpcSet.begin();
	
	int Count = 0;

	for( ; i != m_NpcSet.end() ; ++i )
	{
		if( (*i).Type == WeaponType )
		{
			return Count;
		}

		++Count;
	}


	return -1;
}


//.................................................................................................................
// NPC 공격
//.................................................................................................................
int CDamageEffect::AttackedNPC_Effect(long unique,DWORD index,long Target_unique,DWORD Target_index,DWORD Damage,BYTE IsCritical,D3DXVECTOR3 vPos,BYTE IsSuccess)
{
	if( !IsSuccess )
	{
		// 미스시 이펙트
//		g_Particle.DisplayUpdate( PT_MISS , Target_unique , Target_index , -1 );

		return FALSE;
	}

//	g_Particle.DisplayUpdate( 2 , Target_unique , Target_index , -1 );
//	g_Particle.DisplayUpdate( 1 , Target_unique , Target_index , -1 );

	int DemageLevel = DamageLevel( Target_unique , Target_index , Damage , IsCritical );

	//..............................................................................................................
	// 무기 종류 체크
	//..............................................................................................................
	int WeaponType = -2;

	int RHand_index = 0;
	int LHand_index = 0;

	if( unique != - 1 && index != -1 )
	{
		RHand_index = g_Pc.m_Pc[ index ].m_pUserInfo->ArmorInfo.index[0];
		LHand_index = g_Pc.m_Pc[ index ].m_pUserInfo->ArmorInfo.index[1];
	}
	else
	{
		RHand_index = g_Pc.GetPlayer()->m_pUserInfo->ArmorInfo.index[0];
		LHand_index = g_Pc.GetPlayer()->m_pUserInfo->ArmorInfo.index[1];
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
	}
	else if( RHand_index != -1 && LHand_index != -1 )			// 양손에 무기 장착
	{
		if( g_Pc_Manager.m_pItm[RHand_index]->m_type == 2 && 
			g_Pc_Manager.m_pItm[LHand_index]->m_type == 3 )		// 한손검 + 방패
		{
			WeaponType = 2;
		}
	}
	else														// 무기 없음 (맨손 공격)
	{
		WeaponType = -1;
	}

	//..............................................................................................................

	int TableIndex = FindWeaponType( WeaponType );

	if( TableIndex != -1 )
	{
		int SoundNum = m_NpcSet[ TableIndex ].SoundNum[ DemageLevel ];
		
		g_Sound.Play( SoundNum );
		
		// 임시 NPC 속성 바뀌면 수정
		//g_Sound.Play( 8 ,  vPos.x , vPos.y , vPos.z );

		g_Particle.SetEffSetCon( 0.0f , m_NpcSet[ TableIndex ].EffectNum[ DemageLevel ] , 0 , m_NpcSet[ TableIndex ].LifeTime[ DemageLevel ] , Target_unique , Target_index );
		
		D3DXVECTOR3 vHeight = g_Pc.GetPcInnerPos( Target_index , Target_unique , 1.2f );
		
		g_Particle.m_DamageNum.Add( vHeight  , Damage , DemageLevel, NULL );
	}

	return TRUE;
}

//.................................................................................................................
// 유저 공격 
//.................................................................................................................
int CDamageEffect::AttackedUser_Effect(long Target_unique,DWORD Target_index,DWORD Damage,BYTE IsCritical,D3DXVECTOR3 vPos,BYTE IsSuccess)
{
	if( !IsSuccess )
	{
//		g_Particle.DisplayUpdate( PT_MISS , Target_unique , -1 , -1 );

		return FALSE;
	}
	
	int DemageLevel = DamageLevel( Target_unique , Target_index , Damage , IsCritical );

	// 임시 데미지 조정
//	Damage = rand() % 100;
	
	if( Damage >= 50 && Target_unique == g_Pc.GetMainPC().lUnique )
		g_Particle.RockCamera( ( rand() % 200 ) + 400 );

	
	int RaceType = 0;									// 일단 종족 0으로 수정 
		
	int SoundNum = m_UserSet[ RaceType ].SoundNum[ DemageLevel ];
	
	if( SoundNum != -1 )
	{
		g_Sound.Play( SoundNum , NULL );

	}

	g_Particle.SetEffSetCon( 0.0f , m_UserSet[ RaceType ].EffectNum[ DemageLevel ] , 0 , m_UserSet[ RaceType ].LifeTime[ DemageLevel ] , Target_unique , -1 );

	D3DXVECTOR3 vHeight = g_Pc.GetPcInnerPos( Target_index , Target_unique , 1.2f );
	
	// 임시
	g_Particle.m_DamageNum.Add( vHeight  , Damage , DemageLevel, NULL );
	
	
	return TRUE;
}

//.................................................................................................................
// 데미지 레벨 ( 0 : 작은거 , 1 : 중간 , 2 : 크리티컬 )
//.................................................................................................................
DamageState CDamageEffect::DamageLevel(long Target_unique,DWORD Target_index,DWORD Damage,BYTE IsCritical)
{
	if( IsCritical ) 
	{
		return DS_CRITICAL;
	}

	DWORD theHP = 100.0f;							// 임시 나중에 정보 있을때 수정 By wxywxy

	float DamageLevel = (float) Damage / ( float ) theHP;

	if( DamageLevel > 0.3f )
	{
		return DS_STRONG;
	}

	return DS_WEAK;
}