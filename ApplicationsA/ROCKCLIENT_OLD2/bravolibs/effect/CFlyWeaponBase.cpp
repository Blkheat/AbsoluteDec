//.................................................................................................................
//
// CFlyWeaponBase.cpp ( 날아가는 무기 베이스 클래스 )													By wxywxy
//
//.................................................................................................................
#include "CflyWeaponBase.h"

#include <EFFECT\\EffectSystem.h>
#include <NETWORK\Protocol.h>
#include "..\\quadlist.h"
#include <BASE\\D3DBASE.H>
#include "..\\RockClient.h"
#include "..\\PC.h"
#include "..\\Camera.h"
#include "..\\Map.h"
#include "ObjBase.H"
#include "..\\bravolibs\\obj\\Character_Item.H"
#include "..\\bravolibs\\obj\\Player_Manager.H"
#include "..\\bravolibs\\obj\\Line_Vertex.H"
#include <stdio.h>
#include <assert.H>


CFlyWeaponBase::CFlyWeaponBase()
{
	m_fTargetRot = 0.0f;
	m_DestTime   = 0;
	m_btLink	 = 0;
	m_State      = FLYWEAPON_STATE_MOVE;
	m_bPC		 = FALSE;
	m_Index		 = -1;
	
	D3DXMatrixIdentity( &m_matWorld );
	
	m_vDestPos = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
	m_vCurtPos = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
}

CFlyWeaponBase::~CFlyWeaponBase()
{
}

//.................................................................................................................
// 타켓 위치 구하기
//.................................................................................................................
D3DXVECTOR3	CFlyWeaponBase::GetTargetPos(void)
{
	D3DXVECTOR3	vPos( 0.0f , 0.0f , 0.0f );
#ifdef DIRECT_VERSION_9_MJH
#else
	if( !GetTargetPos( vPos ) )
	  return false;
#endif // DIRECT_VERSION_9_MJH	

	return vPos;
}

//.................................................................................................................
// 타켓 위치 구하기
//.................................................................................................................
BOOL CFlyWeaponBase::GetTargetPos(D3DXVECTOR3& vPos)
{	
	// 유저	
	if( m_bPC == true )
	{
		if( m_Index == MAX_USER_INTABLE )
		{					
			vPos.x = g_Pc.GetPlayer()->GetPosTM()._41;
			vPos.y = g_Pc.GetPlayer()->GetPosTM()._42;
			vPos.z = g_Pc.GetPlayer()->GetPosTM()._43;
		}
		else
		{
			if( g_Pc.m_Pc[ m_Index ].m_bIsLoaded == FALSE )
			{
				m_State = FLYWEAPON_STATE_DESTROY;

				return false;
			}
			vPos.x = g_Pc.m_Pc[ m_Index ].GetPosTM()._41;
			vPos.y = g_Pc.m_Pc[ m_Index ].GetPosTM()._42;
			vPos.z = g_Pc.m_Pc[ m_Index ].GetPosTM()._43;
		}
	}
	// NPC
	else
	{
		if( g_Map.m_Npc[ m_Index ].m_bIsLoaded == false )
		{
			m_State = FLYWEAPON_STATE_DESTROY;

			return false;
		}
		
		vPos.x = g_Map.m_Npc[ m_Index ].GetPosTM()._41;
		vPos.y = g_Map.m_Npc[ m_Index ].GetPosTM()._42;
		vPos.z = g_Map.m_Npc[ m_Index ].GetPosTM()._43;
	}
	
	return true;
}

//.................................................................................................................
// 타켓 회전값
//.................................................................................................................
float CFlyWeaponBase::GetTargetRot(void)
{
	// 유저	
	float rot;

	if( m_bPC == true )
	{
		if( m_Index == MAX_USER_INTABLE )		
			rot = g_Pc.GetPlayer()->m_Mov.now_Yrad;		
		else
		{
			if( g_Pc.m_Pc[ m_Index ].m_bIsLoaded == false )
			{
				m_State = FLYWEAPON_STATE_DESTROY;
	
				return -1.0f;
			}
			rot = g_Pc.m_Pc[ m_Index ].m_Mov.now_Yrad;
		}
	}
	// NPC
	else
	{
		if( g_Map.m_Npc[ m_Index ].m_bIsLoaded == false )
		{
			m_State = FLYWEAPON_STATE_DESTROY;

			return -1.0f;
		}
		
		rot = g_Map.m_Npc[ m_Index ].m_Mov.now_Yrad;
	}
	return rot;
}

//.................................................................................................................
// 타켓 상태 구하기
//.................................................................................................................
int	CFlyWeaponBase::GetTargetEvent(void)
{
	int event;
	
	if( m_bPC == true )
	{
		if( m_Index == MAX_USER_INTABLE )		
			event = g_Pc.GetPlayer()->m_curt_event.type;
		else
			event = g_Pc.GetUser((short) m_Index )->m_curt_event.type;
	}
	// NPC
	else
		event = g_Map.m_Npc[ m_Index ].m_curt_event.type;

	return event;
}
