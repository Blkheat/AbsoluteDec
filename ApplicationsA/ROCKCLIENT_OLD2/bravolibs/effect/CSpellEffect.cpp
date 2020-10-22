#include "RockPCH.h"
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
#include <stdio.h>
#include <assert.H>
#include <SOUND\Sound.H>
#include "CTextCommand.h"
#include "CSpellEffect.h"

extern	CBravoPC		g_Pc;

//...............................................................................................................
// 영창 이펙트 리스트
//...............................................................................................................
CSpellEffectBaseData	g_SpellEffectBaseData;
//...............................................................................................................
CSpellEffectBaseData::CSpellEffectBaseData()
{
}

CSpellEffectBaseData::~CSpellEffectBaseData()
{
}

int CSpellEffectBaseData::Load(char *Path)
{
	CTextCommand TextCmd;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined( LOAD_VF ) /////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	char	FullName[ 1024 ] = "";

	wsprintf( FullName , "%s\\Etc\\spelltable.dat" , Path );

	if( TextCmd.Open( FullName ) == NULL ) 
	{
		DebugPrint( "Error Loading SpellTable.Dat\n" );
		
		return FALSE;
	}

///////////////////////////////////////////////////////////////////////////////////////////////////	
#else  ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

	if( TextCmd.OpenVF( "spelltable.dat" ) == NULL ) 
	{
		DebugPrint( "Error Loading SpellTable.Dat\n" );
		
		return FALSE;
	}

///////////////////////////////////////////////////////////////////////////////////////////////////	
#endif ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

	SPELL_EFFECT_DATA	TempData;
	int					Code;
	int				    Count;

	while( TextCmd.GetLine() != NULL )
	{
		if( TextCmd.ISArgEmpty() ) continue;

		if( TextCmd.GetArgSize() == 19 )
		{
			TextCmd.GetArgValue( 0 , Code );								// 스킬 코드

			Count = 1;

			for( int i = 0 ; i < 6 ; ++i )
			{
				TextCmd.GetArgValue( Count++ , TempData.SpotEffect[ i ] );
				TextCmd.GetArgValue( Count++ , TempData.SpotTime[ i ] );
			}

			for(int i = 0 ; i < 6 ; ++i )
			{
				TextCmd.GetArgValue( Count++ , TempData.LinkEffect[ i ] );
			}

			m_EffectList.Insert( ( int ) Code , TempData );
		}
	}

	TextCmd.Close();

	return TRUE;
	
}

//...............................................................................................................
// 무기종류 , 레벨로 영창 이펙트 번호 찾기
//...............................................................................................................
int	CSpellEffectBaseData::GetSpellEffect(DWORD SkillCode,int Level,int *pSpotEffectNum,float *pSpotTime,int *pLinkEffectNum)
{
	SPELL_EFFECT_DATA *pData = NULL;
	
	if( NULL != ( pData = m_EffectList.GetKeyDataPointer( SkillCode ) ) )
	{
		*pLinkEffectNum = pData->LinkEffect[ Level ];
		*pSpotEffectNum = pData->SpotEffect[ Level ];
		*pSpotTime      = pData->SpotTime[ Level ];
		return TRUE;
	}

	return FALSE;
}
//...............................................................................................................



CSpellEffect::CSpellEffect()
{
	m_OldSpotNum = -1;
	m_OldLinkNum = -1;
}

CSpellEffect::~CSpellEffect()
{
}

//...............................................................................................................
// 영창 이펙트 발동 ( 스킬 코드 / 영창 레벨 )
//...............................................................................................................
void CSpellEffect::SetSpellEffect(DWORD SkillCode,int Level,long lUnique,int nIndex)
{
	m_lUnique = lUnique;
	m_Index   = nIndex;

	Clear();

	g_Particle.m_Combo2DEffect.SetCombo( 1 , Level );

	if( Level == -1 || SkillCode == - 1 )
	{
		return;
	}
	
	int   LinkEffectNum = -1;
	int   SpotEffectNum = -1;
	float SpotTime      = 0.0f;

	if( g_SpellEffectBaseData.GetSpellEffect( SkillCode , Level , &SpotEffectNum , &SpotTime , &LinkEffectNum ) )
	{
		if( -1 != SpotEffectNum )
			g_Particle.SetEffSetCon( 0.0f , SpotEffectNum , 0 , SpotTime , m_lUnique , m_Index );

		if( -1 != LinkEffectNum )
			g_Particle.SetEffSetCon( 0.0f , LinkEffectNum , 0 , EFFECT_SET_LOOP_TIME , m_lUnique , m_Index );

		m_OldSpotNum = SpotEffectNum;
		m_OldLinkNum = LinkEffectNum;

#ifdef __DEBUG
	
		DebugPrint( "Skill Code : %d , Spell : Level ( %d ) , Spot( %d ) , Link( %d )\n " , SkillCode , Level , SpotEffectNum , LinkEffectNum );
		
#endif		
	
	}
	
}

//...............................................................................................................
// 영창 이펙트 끄기
//...............................................................................................................
void CSpellEffect::Clear(void)
{
	if( -1 != m_OldSpotNum )
	{	
		g_Particle.DelEffSetCon( m_OldSpotNum , 0 , m_lUnique , m_Index );
	}

	if( -1 != m_OldLinkNum )
	{	
		g_Particle.DelEffSetCon( m_OldLinkNum , 0 , m_lUnique , m_Index );
	}
	
}

