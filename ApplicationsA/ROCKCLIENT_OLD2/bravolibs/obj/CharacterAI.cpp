#include "RockPCH.h"

#include	<NETWORK\Protocol.h>
//#include	"..\Network.H"
#include	"..\\quadlist.h"
#include	"Character.H"
#include	"..\\PC.h"
#include	"..\\Effect.h"
#include	"..\\Camera.H"
//#include	"..\\Menu_Interface.h"
//#include	"..\\Weather_Manager.H"
#include	"TextureManager.H"
#include	<SOUND\\Sound.h>
//#include	<INTERFACE\\Item.H>
#include    "..\\Effect\\CEffectBase.h"

#include	<assert.H>


#include "..\\RockClient.h"
#include "..\\global.h"

#include    "..\\\rocklibs\RockInterface\\PartyInforWnd.h"
#include    "..\\\rocklibs\RockInterface\\EmblemTexManager.h"

#include "..\\..\\CRenderManager.h"
#include "CharacterAI.h"

CNormalState * CNormalState::Instance()
{
	static CNormalState instance;

	return &instance;
}

void CNormalState::Enter(Character *pCharacter)
{

}

void CNormalState::Execute(Character *pCharacter)
{
	//내 유닛과 펫 유닛의 거리차를 비교	
}

void CNormalState::Exit(Character *pCharacter)
{

}

CTransState * CTransState::Instance()
{
	static CTransState instance;

	return &instance;
}
	
void CTransState::Enter(Character *pCharacter)
{
	
}

void CTransState::Execute(Character *pCharacter)
{		
	//정해진 포지션으로 Pet을 이동
	if( pCharacter->dDestCell )
	{
		D3DXVECTOR3 dest(pCharacter->GetPosTM()._41, pCharacter->GetPosTM()._42, pCharacter->GetPosTM()._43);
		D3DXVECTOR3 src(g_Pc.GetPlayer()->GetPosTM()._41, g_Pc.GetPlayer()->GetPosTM()._42, g_Pc.GetPlayer()->GetPosTM()._43);
		
		DWORD PosX = pCharacter->dDestCell % g_Pc.m_cSize;
		DWORD PosY = pCharacter->dDestCell / g_Pc.m_cSize;		

		float fHeight = g_Map.Get__MapHeight( PosX , PosY );

		float x = ( PosX * CLIENT_TILESIZE ) + ( CLIENT_TILESIZE / 2 );
		float z = ( PosY * CLIENT_TILESIZE ) + ( CLIENT_TILESIZE / 2 );

		D3DXVECTOR3 vPos(x, fHeight, z);
		dest = dest - vPos;       //펫의 위치에서 목적지 뺌
		src  = src - vPos;		  //플레이어의 위치에서 목적지 뺌
		
		if( ( 35.0f < ( D3DXVec3Length( &dest ) - D3DXVec3Length( &src ) ) ) ) // || ( fDot < fDestD )  )
		{
			vPos -= (25.0f * g_Pc.GetPlayer()->m_vDir);

			D3DXVECTOR3 vRight = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			D3DXVec3Cross( &vRight , &(g_Pc.GetPlayer()->m_vDir) , &D3DXVECTOR3(0.0f, 1.0f, 0.0f) );
			D3DXVec3Normalize(&vRight, &vRight);

			vPos += (20.0f * vRight);

			PosX = (DWORD)(vPos.x / 8.0f);
			PosY = (DWORD)(vPos.z / 8.0f);				
			
			bOnetime = true;
			DWORD aDestCell = PosX + ( PosY * g_Pc.m_cSize );
	
			for( int i = 0; i < 25; ++i )
			{
				PosX += RandomNum( -1, 1 );
				PosY += RandomNum( -1, 1 );

				if( ( g_Map.m_pAttrMap[PosY][PosX].byAttr != MATTR_OBJ ) && ( g_Map.m_Block[PosX][PosY].GetClear() ) )
				{		
					aDestCell = PosX + ( PosY * g_Pc.m_cSize );
					break;
				}
			}

			int npc_tbl_index = g_Pc.GetPlayer()->m_State.PetIndex;
			DWORD aPetID = (DWORD)g_Map.m_NpcTable[npc_tbl_index].lUnique;
			g_RockClient.Send_CsPetStartMove(aPetID, aDestCell);			
			
			pCharacter->ChangeState(CNormalState::Instance());
			
			pCharacter->dDestCell = 0;			
		}		
	}

	if( pCharacter->m_IsDefenceBack )
	{
		DWORD PosX = 0;
		DWORD PosY = 0;
		
		D3DXVECTOR3 vPosPet(pCharacter->GetPosTM()._41, pCharacter->GetPosTM()._42, pCharacter->GetPosTM()._43);
		D3DXVECTOR3 vPosPlayer(g_Pc.GetPlayer()->GetPosTM()._41, g_Pc.GetPlayer()->GetPosTM()._42, g_Pc.GetPlayer()->GetPosTM()._43);
		
		D3DXVECTOR3 vDir1 = pCharacter->vDestPos - vPosPlayer;
		D3DXVECTOR3 vDir2 = vPosPet - vPosPlayer;
		
		D3DXVec3Normalize(&vDir1, &vDir1);
		D3DXVec3Normalize(&vDir2, &vDir2);

		float fDot = D3DXVec3Dot( &vDir1, &vDir2 );
		float fDest = (float)cos( D3DXToRadian(145) );

		float fLen = D3DXVec3Length( &(vPosPet - vPosPlayer) );
	
		if( (fDest < fDot) || (fLen > 35.0f))
		{
			D3DXVECTOR3 vPos = vPosPlayer - ( 20.0f * vDir1 );	

			PosX = (DWORD)(vPos.x / 8.0f);
			PosY = (DWORD)(vPos.z / 8.0f);

			DWORD aDestCell = PosX + ( PosY * g_Pc.m_cSize );
		
			for( int i = 0; i < 25; ++i )
			{
				PosX += RandomNum( -1, 1 );
				PosY += RandomNum( -1, 1 );

				if( ( g_Map.m_pAttrMap[PosY][PosX].byAttr != MATTR_OBJ ) && ( g_Map.m_Block[PosX][PosY].GetClear() ) )
				{		
					aDestCell = PosX + ( PosY * g_Pc.m_cSize );
					break;
				}
			}

			int npc_tbl_index = g_Pc.GetPlayer()->m_State.PetIndex;
			DWORD aPetID = (DWORD)g_Map.m_NpcTable[npc_tbl_index].lUnique;

			g_RockClient.Send_CsPetStartMove(aPetID, aDestCell);	

		}
		
		pCharacter->ChangeState(CNormalState::Instance());

		pCharacter->m_IsDefenceBack = false;		
	}

	if(pCharacter->m_IsOwnerBack)
	{
		DWORD PosX = 0;
		DWORD PosY = 0;
		
		D3DXVECTOR3 vPosPlayer(g_Pc.GetPlayer()->GetPosTM()._41, g_Pc.GetPlayer()->GetPosTM()._42, 
							   g_Pc.GetPlayer()->GetPosTM()._43);

		D3DXVECTOR3 vPos = vPosPlayer - ( 25.0f * g_Pc.GetPlayer()->m_vDir );
		
		PosX = (DWORD)(vPos.x / 8.0f);
		PosY = (DWORD)(vPos.z / 8.0f);

		DWORD aDestCell = PosX + ( PosY * g_Pc.m_cSize );
		
		for( int i = 0; i < 25; ++i )
		{
			PosX += RandomNum( -1, 1 );
			PosY += RandomNum( -1, 1 );

			if( ( g_Map.m_pAttrMap[PosY][PosX].byAttr != MATTR_OBJ ) && ( g_Map.m_Block[PosX][PosY].GetClear() ) )
			{		
				aDestCell = PosX + ( PosY * g_Pc.m_cSize );
				break;
			}
		}

		int npc_tbl_index = g_Pc.GetPlayer()->m_State.PetIndex;
		DWORD aPetID = (DWORD)g_Map.m_NpcTable[npc_tbl_index].lUnique;

		g_RockClient.Send_CsPetStartMove(aPetID, aDestCell);
		
		pCharacter->m_IsOwnerBack = false;
	}	

	
	if( !pCharacter->dDestCell )
	{
		pCharacter->ChangeState(CNormalState::Instance());
	}
}

void CTransState::Exit(Character *pCharacter)
{

}

CAttackState * CAttackState::Instance()
{
	static CAttackState instance;

	return &instance;
}

void CAttackState::Enter(Character *pCharacter)
{
	bOnetime = false;
}

void CAttackState::Execute(Character *pCharacter)
{
	
	// Pet에게 목표 NPC에게 다가가기
	if(pCharacter->dAttackDestCell)
	{
		//대상 공격셀이 유저 위치와 일정거리 차일시엔 다시 유저에게 돌어온다
		
		DWORD PosX = pCharacter->dAttackDestCell % g_Pc.m_cSize;
		DWORD PosY = pCharacter->dAttackDestCell / g_Pc.m_cSize;		

		float fHeight = g_Map.Get__MapHeight( PosX , PosY );

		float x = ( PosX * CLIENT_TILESIZE ) + ( CLIENT_TILESIZE / 2 );
		float z = ( PosY * CLIENT_TILESIZE ) + ( CLIENT_TILESIZE / 2 );

		D3DXVECTOR3 vPos(x, fHeight, z);
		D3DXVECTOR3 vPosPlayer(g_Pc.GetPlayer()->GetPosTM()._41, g_Pc.GetPlayer()->GetPosTM()._42, g_Pc.GetPlayer()->GetPosTM()._43);	
		
		float fLength = D3DXVec3Length( &(vPos - vPosPlayer) );
			
		if( fLength < 250.0f )
		{
			DWORD aDestCell = pCharacter->dAttackDestCell;
		
			int npc_tbl_index = g_Pc.GetPlayer()->m_State.PetIndex;
			DWORD aPetID = (DWORD)g_Map.m_NpcTable[npc_tbl_index].lUnique;

			g_RockClient.Send_CsPetStartMove(aPetID, aDestCell);		
		
			pCharacter->dAttackDestCell = 0;
			pCharacter->ChangeState(CNormalState::Instance());
		}
		else
		{
			((NonPlayer *)pCharacter)->theAttack_Event.Clear();
			g_Pc.GetPlayer()->Pet_CsOwnerBackMove();
			pCharacter->ChangeState(CNormalState::Instance());
		}
	}

	// Pet에게 공격 명령
	if(pCharacter->theAttackedID)
	{
		pCharacter->ChangeState(CNormalState::Instance());
		pCharacter->theAttackedID = 0;
	}	

	m_nAttackIndex = -1;
}

void CAttackState::Exit(Character *pCharacter)
{

}

