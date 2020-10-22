//.................................................................................................................
//
// CArrowWeapon.cpp ( 화살 )													By wxywxy
//
//.................................................................................................................
#include "CArrowWeapon.h"

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
#ifdef DIRECT_VERSION_9_MJH
#include "D3dx9math.h"
#else
#include "D3dx8math.h"
#endif // DIRECT_VERSION_9_MJH

extern		long	g_nowTime;
#ifdef DIRECT_VERSION_9_MJH
extern		LPDIRECT3DDEVICE9	g_pd3dDevice;
#else
extern		LPDIRECT3DDEVICE8	g_pd3dDevice;
#endif // DIRECT_VERSION_9_MJH

CArrowWeapon::CArrowWeapon()
{

}



CArrowWeapon::~CArrowWeapon()
{

}

//.................................................................................................................
// 이동 처리
//.................................................................................................................
int CArrowWeapon::Update(void)
{
	//.............................................................................................................
	// 타겟위치
	//.............................................................................................................
	D3DXVECTOR3	vPos;
		
	if( GetTargetPos( vPos ) == FALSE )			
		return FALSE;		
				

	vPos.y =+ 4.0f;
	//.............................................................................................................
	// 화살 상태 
	//.............................................................................................................
	switch( m_State )
	{
		//.........................................................................................................
		// 화살이 날라가는중
		//.........................................................................................................
		case FLYWEAPON_STATE_MOVE:	
		{				
			//.....................................................................................................
			// 화살이 목적지에 도착
			//.....................................................................................................
			if( SAFE_TIME_COMPARE( m_DestTime , <= , g_nowTime ) )
			{
				SelfDestroy();

				// 현재위치값 저장			
				m_matWorld._41 = vPos.x;
				m_matWorld._42 = vPos.y;
				m_matWorld._43 = vPos.z;

				return TRUE;

			}				
			//.....................................................................................................
			// 화살이 이동중
			//.....................................................................................................
			else
			{
				float Range = 1.0f - ( ( SAFE_TIME_SUB( m_DestTime , g_nowTime ) ) / ( float ) m_TimeRange );
				
				vPos.x = m_vCurtPos.x + ( ( m_vDestPos.x - m_vCurtPos.x ) * Range );
				vPos.y = m_vCurtPos.y + ( ( m_vDestPos.y - m_vCurtPos.y ) * Range );
				vPos.z = m_vCurtPos.z + ( ( m_vDestPos.z - m_vCurtPos.z ) * Range );
				
				m_matWorld._41 = vPos.x;
				m_matWorld._42 = vPos.y;
				m_matWorld._43 = vPos.z;
			}
		}
		break;

		//.........................................................................................................
		// 화살이 타겟에 박혀있다
		//.........................................................................................................

		case FLYWEAPON_STATE_ATTACH:	
		{
			//.....................................................................................................
			// 타겟이 죽었다면 화살지운다
			//.....................................................................................................
			if( m_bPC == true )
			{
				if( m_Index == MAX_USER_INTABLE )
				{
					if( g_Pc.GetPlayer()->m_curt_event.type == SM_STATE_DIE )
					{
						SelfDestroy();
					
						return FALSE;
					}
				}
				else 
				{
					if( g_Pc.m_Pc[ m_Index ].m_bIsLoaded == false )
					{
						SelfDestroy();

						return FALSE;
					}
					else 
					if( g_Pc.m_Pc[ m_Index ].m_curt_event.type == SM_STATE_DIE )
					{
						SelfDestroy();

						return FALSE;
					}
				}
			}
			else
			{
				if( g_Map.m_Npc[ m_Index ].m_bIsLoaded == false )
				{
					SelfDestroy();
					
					return FALSE;
				}
				else if( g_Map.m_Npc[ m_Index ].m_curt_event.type == SM_STATE_DIE )
				{
					SelfDestroy();
					
					return FALSE;
				}
			}
			
			//.................................................................................................
			// 화살이 박혀있는 시간 지났다면 삭제
			//.................................................................................................
			if( SAFE_TIME_COMPARE( m_DestTime , <= , g_nowTime ) )
			{
				SelfDestroy();
				
				return FALSE;
			}
			
			// 현재위치값 저장			
			m_matWorld._41 = vPos.x;
			m_matWorld._42 = vPos.y;
			m_matWorld._43 = vPos.z;
			
			//.................................................................................................
			// 타겟 대상의 회전값이 변했다면 회전시키기
			//.................................................................................................
			if( m_fTargetRot != GetTargetRot() )
			{				
				D3DXMATRIX	matRot;	
				
				D3DXMatrixRotationY( &matRot, GetTargetRot() - m_fTargetRot );
				D3DXMatrixMultiply( &m_matWorld , &m_matWorld , &matRot );
				
				m_fTargetRot = GetTargetRot();
			}	
			
			//.................................................................................................
			// 링크 노드의 애니메이션값을 더해준다
			//.................................................................................................
			float time;
			int num, local, type, frame;
			if( m_bPC == true )
			{
				if( m_Index == MAX_USER_INTABLE )
				{
					num		= g_Pc.GetPlayer()->m_ani_number;
					frame	= g_Pc.GetPlayer()->m_ani_frame;					
					type	= g_Pc.GetPlayer()->m_curt_event.type;
					//local	= g_Pc.GetPlayer()->m_pAniInfo[ATTACK0].ani_number;
					local	= g_Pc.GetPlayer()->m_ani_number;
					time	= g_Pc.GetPlayer()->m_middle_time;
				}
				else
				{
					num		= g_Pc.m_Pc[m_Index].m_ani_number;
					frame	= g_Pc.m_Pc[m_Index].m_ani_frame;					
					type	= g_Pc.m_Pc[m_Index].m_curt_event.type;
					//local	= g_Pc.m_Pc[m_Index].m_pAniInfo[ATTACK0].ani_number;
					local	= g_Pc.m_Pc[m_Index].m_ani_number;
					time	= g_Pc.m_Pc[m_Index].m_middle_time;
				}
			}
			else
			{
				num		= g_Map.m_Npc[m_Index].m_ani_number;
				frame	= g_Map.m_Npc[m_Index].m_ani_frame;
				type	= g_Map.m_Npc[m_Index].m_curt_event.type;
				//local	= g_Map.m_Npc[m_Index].m_pAniInfo[NPC_STANDARD0].ani_number;
				local	= g_Map.m_Npc[m_Index].m_ani_number;
				time	= g_Map.m_Npc[m_Index].m_middle_time;
				
			}
			
			// 현재 상태에 따라서 처리
			switch( type )
			{
			case SM_MOVE_BROADCAST:	// 이동중
				{
					float ani_x		= g_Pc_Manager.m_pAni[num]->m_pNode[ m_btLink ].m_pAnimation[frame].m_vPos.x;
					float ani_y		= g_Pc_Manager.m_pAni[num]->m_pNode[ m_btLink ].m_pAnimation[frame].m_vPos.y;
					float ani_z		= g_Pc_Manager.m_pAni[num]->m_pNode[ m_btLink ].m_pAnimation[frame].m_vPos.z;
					
					m_matWorld._41	= vPos.x+ani_x;
					m_matWorld._42	= vPos.y+ani_y;
					m_matWorld._43	= vPos.z+ani_z;
					
				}
				break;
			case SM_ATTACK_NPC:	// 일반공격
				{
					SelfDestroy();
					
					return FALSE;
				}
				break;
			case SM_USE_SKILL:	// 스킬사용
				{
					
					SelfDestroy();
					
					return FALSE;
				}
				break;
				
			default:
				{
					D3DXMATRIX	matTemp = g_Pc_Manager.NodeAnimaion( num, m_btLink, local, frame, time );
					
//					D3DXMatrixMultiply( &m_matWorld , &matTemp , &m_matWorld );
			
				}
				break;
			}
		}
		break;
			
		default:
		{
			SelfDestroy();
				
			return FALSE;
		}
		break;
	}

	return TRUE;
}

int CArrowWeapon::Render(void)
{
	g_RenderManager.SetTransform( D3DTS_WORLD, &m_matWorld );

	DWORD   Index     = g_Pc_Manager.SetItemID(9,0,1,0,2,0 );
	int     IndexItem = g_Pc_Manager.GetItemID( Index );
	int		mod_index = g_Pc_Manager.m_pItm[ IndexItem ]->m_mod_index;
	int		tex_index = g_Pc_Manager.m_pItm[ IndexItem ]->m_tex_index;		
	
	g_RenderManager.SetTexture( 0, g_Pc_Manager.m_pTex[tex_index]->m_pD3DTex );	
	g_RenderManager.SetIndices( g_Pc_Manager.m_pMod[mod_index]->m_pIB, 0 );
	g_RenderManager.SetStreamSource( 0, g_Pc_Manager.m_pItm[IndexItem]->m_pNormalVB, sizeof(OBJECTVERTEX) );
	
	DrawIndexedPrimitive( g_Pc_Manager.m_pMod[mod_index]->m_header.m_num_vertex,
		g_Pc_Manager.m_pMod[mod_index]->m_header.m_num_face );

	
	return TRUE;
}

int CArrowWeapon::Create(BOOL ISPC,int table_index,BYTE link,D3DXVECTOR3 Curt,D3DXVECTOR3 Dest) 
{ 
	//-----------------------------------------------------------------------------
	// 타겟정보저장
	//-----------------------------------------------------------------------------
	m_bPC		 = ISPC;
	m_Index		 = table_index;
	m_btLink	 = link;
	m_vCurtPos	 = Curt;
	m_vDestPos	 = Dest;
	m_fTargetRot = 0.0f;		

/*
	//-----------------------------------------------------------------------------
	// 거리에 따라서 날라가는 시간계산
	//-----------------------------------------------------------------------------
	float x = m_vCurtPos.x - m_vDestPos.x;
	float y = m_vCurtPos.y - m_vDestPos.y;
	float z = m_vCurtPos.z - m_vDestPos.z;
	int range;
	
	if( abs(x) > abs(y) )
	{
		if( abs(x) > abs(z) )
			range = (int)(x/2);
		else
			range = (int)(z/2);
	}
	else
	{
		if( abs(y) > abs(z) )
			range = (int)(y/2);
		else
			range = (int)(z/2);
	}	

	if( range >= 2 )
		m_DestTime += ARROW_MOVING_TIME;
	else if( range <= 1 )
		m_DestTime += (ARROW_MOVING_TIME/2);
	else
		m_DestTime += (ARROW_MOVING_TIME-(ARROW_MOVING_TIME/range));

//	m_DestTime += ARROW_MOVING_TIME; // 임시

	//-----------------------------------------------------------------------------
	// 화살이 날라가는 회전값 저장 ( Y방향 회전 )
	//-----------------------------------------------------------------------------
	float dest_x, dest_y, fYRot, fXRot;	

	dest_x	= GetTargetPos().x - m_vCurtPos.x;
	dest_y	= GetTargetPos().z - m_vCurtPos.z;	
	fYRot	= -atan2( dest_y, dest_x )-1.57f;	
		
	// X방향회전
	dest_y	= m_vCurtPos.y-(GetTargetPos().y+m_vDestPos.y);
	fXRot	= -dest_y*ANGLE;

	// 회전값 저장하기
	D3DXMatrixRotationYawPitchRoll( &m_matWorld, fYRot, fXRot, 0.0f );	
*/
	D3DXVECTOR3		vTarget     = GetTargetPos() - m_vCurtPos;
	float			fProduct    = 1.0f;
	D3DXVECTOR3     vDir( 0.0f , 0.0f , -1.0f );
	D3DXVECTOR3     vNormal;
	
	float			fDistance   = D3DXVec3Length( &vTarget );
	
	D3DXVec3Normalize( &vTarget , &vTarget );

	D3DXVec3Cross( &vNormal , &vDir , &vTarget );

	if( vNormal.y < 0 )
		fProduct = -1.0f;

	float Dot   = D3DXVec3Dot( &vDir , &vTarget );

	float fRotY = acosf( Dot ) * fProduct;

	m_TimeRange = fDistance * ARROW_MOVING_TIME;
	m_DestTime	 = SAFE_TIME_ADD( g_nowTime , m_TimeRange );

	D3DXMatrixRotationY( &m_matWorld , fRotY );
	
	return TRUE; 
}

