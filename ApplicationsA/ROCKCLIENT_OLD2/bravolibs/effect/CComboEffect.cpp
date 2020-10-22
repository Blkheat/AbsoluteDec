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
#include	<stdio.h>
#include	<assert.H>
#include	<SOUND\Sound.H>
#include	"CTextCommand.h"
#include    "CComboEffect.h"

extern	CBravoPC		g_Pc;

CComboEffect::CComboEffect()
{
  	m_OldLevel = 0;
	
	m_pList    = NULL;
	m_pList    = SAFE_NEW_ARRAY( ComboEffectSet , 6 );		
	
	//............................................................................................................
	// ������ ���� ����Ʈ ��ȣ �ֱ�
	//............................................................................................................
	m_pList[ 0 ].EffectNum[ 0 ] = -1;	
	m_pList[ 0 ].EffectNum[ 1 ] = -1;	
	m_pList[ 0 ].EffectNum[ 2 ] = -1;	

	m_pList[ 1 ].EffectNum[ 0 ] = EF_MILLENA_COMBO2;	
	m_pList[ 1 ].EffectNum[ 1 ] = EF_RAIN_COMBO2;	
	m_pList[ 1 ].EffectNum[ 2 ] = -1;	

	m_pList[ 2 ].EffectNum[ 0 ] = EF_MILLENA_COMBO3;	
	m_pList[ 2 ].EffectNum[ 1 ] = EF_RAIN_COMBO3;	
	m_pList[ 2 ].EffectNum[ 2 ] = -1;	

	m_pList[ 3 ].EffectNum[ 0 ] = EF_MILLENA_COMBO4;	
	m_pList[ 3 ].EffectNum[ 1 ] = EF_RAIN_COMBO4;	
	m_pList[ 3 ].EffectNum[ 2 ] = -1;	

	m_pList[ 4 ].EffectNum[ 0 ] = EF_MILLENA_COMBO5;	
	m_pList[ 4 ].EffectNum[ 1 ] = EF_RAIN_COMBO5;	
	m_pList[ 4 ].EffectNum[ 2 ] = -1;	

	m_pList[ 5 ].EffectNum[ 0 ] = EF_MILLENA_COMBOEX;	
	m_pList[ 5 ].EffectNum[ 1 ] = EF_RAIN_COMBOEX;	
	m_pList[ 5 ].EffectNum[ 2 ] = -1;	
}

CComboEffect::~CComboEffect()
{
	SAFE_DELETE_ARRAY( m_pList );
}

//................................................................................................................
// �޺� ����Ʈ ��ȭ üũ�ؼ� �߰��ϱ�
//................................................................................................................
void CComboEffect::SetComboEffect(int level)
{
	if( m_OldLevel == level ) return;

	// ���� ����Ʈ �����
	int Type = GetClass();
	Clear( Type );		
	
	m_OldLevel = level;

	if( m_pList[ m_OldLevel ].EffectNum[ Type ]	== -1 ) 
		return;

	//g_Particle.SetEffSetCon( 0.0f , m_pList[ m_OldLevel ].EffectNum[ Type ] , 0 , 1000 , g_Pc.GetMainPCUniqe() , -1 );
}

//................................................................................................................
// ���� ����Ʈ �����
//................................................................................................................
void CComboEffect::Clear(int Type)
{
	if( NULL == m_pList )
		return;

	if( m_pList[ m_OldLevel ].EffectNum[ Type ]	== -1 ) 
		return;
	
	g_Particle.DelEffSetCon( m_pList[ m_OldLevel ].EffectNum[ Type ] , 0 , g_Pc.GetMainPCUniqe() , -1 );
}

int CComboEffect::GetClass(void)
{
	int chrAttrIndex = g_Pc.GetPlayer()->GetAttrIndex();
	int chrClassType = nRui->thePcParam.MainClass;	

	switch( chrAttrIndex )
	{
		case n_Millena_Man:
		case n_Millena_Woman:

			return 0;

			break;

		case n_Rain_Man:
		case n_Rain_Woman:
			
			return 1;
			
			break;
	}

	/*

	if(	(	// �з��� ��-��, ������ ������, �ҵ��, ����Ʈ, �ӽóʸ�, �����̾�  
		( chrAttrIndex == n_Millena_Man || chrAttrIndex == n_Millena_Woman ) &&
		( chrClassType == Wanderer || chrClassType == Swordman || chrClassType == Knight || chrClassType == Mercenary || chrClassType == Slayer ) 
		) ||
		(	// ���� ��-��, ������ ��Ʈ������, �Ҽ���, ���ӳ� 
			( chrAttrIndex == n_Rain_Man || chrAttrIndex == n_Rain_Woman ) &&
			( chrClassType == Stranger || chrClassType == Sorcerer || chrClassType == Psyche ) 
		)
	  )
	{
		return 0;
	}
	// ���� ������ 
	else if(	// �з��� ��-��, ������ ������, ��ó, �ҿ� �극��Ŀ  
		( chrAttrIndex == n_Millena_Man || chrAttrIndex == n_Millena_Woman ) &&
		( chrClassType == Wanderer || chrClassType == Archer || chrClassType == Sniper ) 
		) 			
	{
		return 1;
	}
	
	// ������ 
	else if(	// ���� ��-��, ������ ��Ʈ������, �Ҽ���, ���ӳ� 
		( chrAttrIndex == n_Rain_Man || chrAttrIndex == n_Rain_Woman ) &&
		( chrClassType == Stranger || chrClassType == Magician || chrClassType == Prominas || chrClassType == Priest || chrClassType == Holy_Avenger ) 		 
		)
	{
		return 1;
	}
*/	

	return -1;
}



