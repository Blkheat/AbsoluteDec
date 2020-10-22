#include "RockPCH.h"

#include	<NETWORK\\Protocol.h>
#include	<Base\\D3DBase.h>
//#include	"..\\Network.h"
#include	<Base\\d3dfont.h>
#include	<Base\\d3dapp.h>
#include	"..\\quadlist.h"
#include	"..\\RockClient.h"
#include	"..\\Camera.h"
#include	"..\\Pc.H"
#include	"..\\Effect.H"
//#include	"..\\ChatList.h"
//#include	"..\\Menu_Interface.h"
//#include	"..\\Weather_Manager.H"
#include	"OBJ\\Character.h"
#include	"OBJ\\ObjMath.h"
#include	"OBJ\\Player_Manager.H"
#include	"Field.h"
#include	"..\\rocklibs\\RockInterface\\MiniMapWndProc.h"
#include	"..\\rocklibs\\RockInterface\\PetInfoWnd.h"
#include	"..\\rocklibs\\RockInterface\\UiUtil.h"

#include "..\\global.h"

#define		NPC_DIE_READY_TIME		500

extern	bool	g_bBlockDebug;

void	SetupMaxMin(D3DXVECTOR3* max, D3DXVECTOR3* min);

extern	long	g_nowTime;

bool		CField::UpdateNpc( int number )
{
	if ( m_Npc[number].m_bIsLoaded )
	{
		D3DXMATRIX		mat;
		D3DXMatrixIdentity(&mat);
		mat._41	= m_Npc[number].GetPosTM()._41;
		mat._42	= m_Npc[number].GetPosTM()._42;
		mat._43	= m_Npc[number].GetPosTM()._43;
		
		D3DXVECTOR3		min	= g_Pc_Manager.GetBoundMinPos( &m_Npc[number] );
		D3DXVECTOR3		max	= g_Pc_Manager.GetBoundMaxPos( &m_Npc[number] );

		if( g_Map.IsGiantUnit( &m_Npc[number] ) )
		{
			min	= g_Pc_Manager.GetCBoundMinPos( &m_Npc[number] );
			max	= g_Pc_Manager.GetCBoundMaxPos( &m_Npc[number] );
		}

		D3DXVec3TransformCoord(&min, &min, &mat);
		D3DXVec3TransformCoord(&max, &max, &mat);
		SetupMaxMin(&max,&min);

		float	sizex	= max.x	- min.x;
		float	sizey	= max.y - min.y;
		float	sizez	= max.z - min.z;
					
		bool bFind;		
		if( true == (bFind = g_Camera.CubeInFrustum(min.x, min.y, min.z, sizex, sizey, sizez) ))
		{						
			m_Npc[number].m_bIsVisible = true;
			dwNpcVisibleCount++;

			// test
			D3DXMATRIX		matChar	= g_Pc.GetPosTM(-1);
			const float	_x  = (matChar._41-mat._41)*(matChar._41-mat._41);
			const float	_y 	= (matChar._43-mat._43)*(matChar._43-mat._43);			
			m_Npc[number].m_fDistance = sqrt(_x+_y);

			if( m_Npc[number].m_fDistance > 500.0f )
			{
				dwNpcVisibleCount--;
				m_Npc[number].m_bIsVisible = false;
			}
		}
		else
		{
			m_Npc[number].m_bIsVisible = false;
		}
	}
	return true;
}


bool		CField::UpdateNpc()
{
	D3DXMATRIX	matChar	= g_Pc.GetPosTM(-1);
	dwNpcVisibleCount	= 0;

	for ( DWORD i = 0; i < MAX_NPC_INTABLE; i ++ )
	{
		if ( m_Npc[i].m_bIsLoaded )
		{
			D3DXMATRIX		mat;
			D3DXMatrixIdentity( &mat );			
			mat._41	= m_Npc[i].GetPosTM()._41;
			mat._42	= m_Npc[i].GetPosTM()._42;
			mat._43	= m_Npc[i].GetPosTM()._43;
			
			D3DXVECTOR3		min	= g_Pc_Manager.GetBoundMinPos( &m_Npc[i] );
			D3DXVECTOR3		max	= g_Pc_Manager.GetBoundMaxPos( &m_Npc[i] );

			if( g_Map.IsGiantUnit( &m_Npc[i] ) )
			{
				min	= g_Pc_Manager.GetCBoundMinPos( &m_Npc[i] );
				max	= g_Pc_Manager.GetCBoundMaxPos( &m_Npc[i] );
			}

			// õ���� ���� �� Ŀ�� �ø��� �ȵǳ�...��  �� �� .
			if( m_NpcTable[i].code >= c_ObjectNPCCode_Min && m_NpcTable[i].code <= c_ObjectNPCCode_Max ) 
			{
				min = min * 0.85f;
				max = max * 0.85f;
				max.y = min.y;
			}

			D3DXVec3TransformCoord(&min, &min, &mat);
			D3DXVec3TransformCoord(&max, &max, &mat);
			SetupMaxMin(&max,&min);

			float	sizex	= max.x	- min.x;
			float	sizey	= max.y - min.y;
			float	sizez	= max.z - min.z;
			
			m_Npc[i].m_bIsVisible = true;

			if ( true == (m_Npc[i].m_bIsVisible = g_Camera.CubeInFrustum(min.x, min.y, min.z, 
																		 sizex, sizey, sizez) ))
			{						
				dwNpcVisibleCount++;

				const float	_x  = (matChar._41-mat._41)*(matChar._41-mat._41);
				const float	_y 	= (matChar._43-mat._43)*(matChar._43-mat._43);			
				m_Npc[i].m_fDistance = sqrt(_x+_y);
			}
		}
	}

	return true;
}


//-----------------------------------------------------------------------------
// Desc: NPC �ִϸ��̼�ó�� & ����ó��
//-----------------------------------------------------------------------------
void		CField::AniProcessNpc()
{
	//-----------------------------------------------------------------------------
	// ĳ���� ���÷���
	//-----------------------------------------------------------------------------
	for( DWORD i = 0; i < MAX_NPC_INTABLE; i ++ )
	{
		if( m_Npc[i].m_bIsLoaded == true )		
		{			
			m_Npc[i].UpdateHPData();
			// ����Ÿó��			
			m_Npc[i].QueueProcess();
						
			// �ִϸ��̼�ó��
			int nRes = m_Npc[i].AniProcess();
//			switch( nRes )
//			{
//			case SM_NPC_DISAPPEAR:	// �þ߹���
//				SM__NPC__DISAPPEAR( m_NpcTable[i].index, m_NpcTable[i].lUnique );
//				break;
//			case SM_STATE_DIE:		// ���
//				SM__NPC__DISAPPEAR( m_NpcTable[i].index, m_NpcTable[i].lUnique );
//				break;
//			}				
		}
	}
}


//-----------------------------------------------------------------------------
// Desc: NPC ������
//-----------------------------------------------------------------------------
HRESULT		CField::DisplayNpc() 
{
	for( DWORD i = 0; i < MAX_NPC_INTABLE; i ++ )
	{					
		if( m_Npc[i].m_bIsLoaded == true && m_Npc[i].m_bIsVisible == true )
		{
			if( m_Npc[i].AniRender() == true )
				m_Npc[i].Display();
		}
	}
	return	S_OK;
}

//-----------------------------------------------------------------------------
// Desc: NPC ������
//-----------------------------------------------------------------------------
HRESULT		CField::ReSetNpcQuestMark() 
{
	for( DWORD i = 0; i < MAX_NPC_INTABLE; i ++ )
	{					
		if( m_Npc[i].m_bIsLoaded == true )
		{
			int nNpcCode	= m_NpcTable[i].code;		

			if(::IsNonPlayerCode(nNpcCode))
			{
				g_Map.m_Npc[i].m_QuestMarkType = NONE_QUEST_MARK;
				g_Map.m_Npc[i].thePreQuestCodeList.clear();
				UpdateNPCQuestMark(nNpcCode, i);
			}
		}
	}
	
	return	S_OK;
}

//-----------------------------------------------------------------------------
// Desc: NPC �׸��� 
//-----------------------------------------------------------------------------
// Desc: �׸��� ����
void	CField::ShadowMakeNpc()
{	
	// �׸��� ���÷���
	for( DWORD i = 0; i < MAX_NPC_INTABLE; i ++ )
	{
		if( m_Npc[i].m_bIsVisible == true && m_Npc[i].m_bIsLoaded == true )		
			m_Npc[i].ShadowMake();
	}
}

// Desc: �׸��� �׸���
void	CField::ShadowDisplayNpc()
{	
	// �׸��� ���÷���
	for( DWORD i = 0; i < MAX_NPC_INTABLE; i ++ )
	{		
		if( m_Npc[i].m_bIsVisible == true && m_Npc[i].m_bIsLoaded == true )
		{
			if( m_Npc[i].m_CharAlpha.GetState() == CHAR_NONE_DISPLAY )
				continue;
			
			if( !m_Npc[i].m_IsAllLTexture )
				continue;

			m_Npc[i].ShadowDisplay();
		}
	}
}


//-----------------------------------------------------------------------------
// Desc: NPC ����
//-----------------------------------------------------------------------------
void		CField::CleanupNpc()
{	
	for( DWORD i = 0; i < MAX_NPC_INTABLE; i ++ )
	{		
		if( m_Npc[i].m_bIsLoaded == true )
		{
			m_dwNpcCount--;
			m_Npc[i].Destroy();				
		}
	}		
	m_nCurNpc = -1;
	InitNpcTable();
}



HRESULT		CField::PickNpc(HWND hWnd, DWORD dwScreenWidth, DWORD dwScreenHeight)
{

	return S_OK;
}


/***********************************************************
	* NPC TABLE
***********************************************************/
void		CField::InitNpcTable()
{	
	for( int i = 0; i < MAX_NPC_INTABLE; i ++ )
	{
		// �ڱⰡ Ÿ�� �ִ� ���� �������� �ʴ´�
//		if( g_Pc.GetPlayer()->m_State.horse_index == i )
//		{
//			OutputDebugString( "\n������ �ȵ�!" );
//		}
//		else
//		{
			m_NpcTable[i].Init();
//		}
	}
}


int CField::FindNpc( long unique, int race )
{
	for( int i = 0; i < MAX_NPC_INTABLE; i ++ )
	{
		if( m_NpcTable[i].lUnique == unique && m_NpcTable[i].race == race )
		{
			if( g_Map.m_Npc[ i ].m_nTableIndex == -1 )
				return MAX_NPC_INTABLE;
			
			return i;		
		}
	}		
	
	return	MAX_NPC_INTABLE;
}


int			CField::GetEmptyTable()
{
	for( int i = 0; i < MAX_NPC_INTABLE; i ++ )
	{
		if( m_NpcTable[i].index == -1 && m_NpcTable[i].lUnique == -1 )
			return i;		
	}
	
	return	MAX_NPC_INTABLE;
}


int			CField::GetNpcIndex( DWORD pos )
{
	for( int i = 0; i < MAX_NPC_INTABLE; i ++ )
	{
		if( m_NpcTable[i].lDestBlock == pos )
			return m_NpcTable[i].index;		
	}	
	return -1;
}

// By simwoosung
NPCTABLE *CField::GetNpcTable( NonPlayer * pNonPlayer  )
{
	for( int i = 0; i < MAX_NPC_INTABLE; i ++ )
	{
		if( ( &m_Npc[i] == pNonPlayer ) && ( m_Npc[i].m_bPC == pNonPlayer->m_bPC ) )
			return &m_NpcTable[ i ];
	}	
	
	return NULL;
}

// By wxywxy
NPCTABLE *CField::GetNpcTable( int unique  )
{
	for( int i = 0; i < MAX_NPC_INTABLE; i ++ )
	{
		if( m_NpcTable[i].lUnique == unique && m_NpcTable[i].race == c_NpcRace_Mob )
			return &m_NpcTable[ i ];
	}	
	
	return NULL;
}

// By wxywxy
//-----------------------------------------------------------------------------
//-- Monster Instance �� Pointer �� ��´�.
//-----------------------------------------------------------------------------
NonPlayer *CField::GetMonster(int Unique )
{
	for( int i = 0; i < MAX_NPC_INTABLE; i ++ )
	{
		if( m_NpcTable[i].lUnique == Unique && m_NpcTable[i].race == c_NpcRace_Mob )
		{
			return &m_Npc[i];
		}		
	}		

	return	NULL;
}



long		CField::GetNpcPos( int index, int unique )
{	
	int i = 0;
	for( i = 0; i < MAX_NPC_INTABLE; i ++ )
	{
		if( m_NpcTable[i].index == index && m_NpcTable[i].lUnique == unique && m_NpcTable[i].race == c_NpcRace_Mob )			
			break;	
	}
	return m_NpcTable[i].lDestBlock;
}

bool		CField::GetNpcAlive( EVENT_DATA_INFO& data )
{
	for( int i = 0; i < MAX_NPC_INTABLE; i ++ )
	{
		if( m_NpcTable[i].index == data.index && m_NpcTable[i].lUnique == data.unique )
		{
			if( m_Npc[i].m_curt_event.type == SM_STATE_DIE )			
				return false;
			
			// ����ִٸ� ��������			
			data.x	= m_NpcTable[i].lDestBlock%(m_cSize*2);
			data.y	= m_NpcTable[i].lDestBlock/(m_cSize*2);
			data.vPos.x = m_Npc[i].GetPosTM()._41;
			data.vPos.y = m_Npc[i].GetPosTM()._42;
			data.vPos.z = m_Npc[i].GetPosTM()._43;			
			return true;
		}		
	}		
	return	false;
}


bool		CField::GetNpcAlive( int index )
{
	if( m_Npc[index].m_curt_event.type == SM_STATE_DIE )			
		return false;
	return true;	
}



bool	CField::GetNpcAttackEnable( int index )
{
	//-----------------------------------------------------------------------------
	// �׾��ִٸ�?
	//-----------------------------------------------------------------------------
	if( m_Npc[index].m_curt_event.type == SM_STATE_DIE )
		return false;

	//-----------------------------------------------------------------------------
	// ���ݰ����� NPC?
	//-----------------------------------------------------------------------------
	// ���̺� ������
	bool bFind = false;
//	if( m_NpcTable[index].race == LIVEITEM_RACE )
//		bFind = true;
	// �Ϲ� NPC
//	else if( g_Pc_Manager.m_NpcAttr[m_NpcTable[index].index].attack == 1 )
		bFind = true;

	// ���ݰ�����
	if( bFind == true )
	{
		// ���� ������ NPC�� �ٽ� �����ߴٸ� false
		if( g_Pc.GetPlayer()->m_curt_event.type		== SM_ATTACK_NPC &&
			g_Pc.GetPlayer()->m_curt_event.index	== index && 
			g_Pc.GetPlayer()->m_curt_event.unique	== m_NpcTable[index].lUnique )
		{
			return false;
		}
		return true;
	}	
	// �̺�Ʈ �ִ� NPC���(���ε�)
//	else if( g_Pc_Manager.m_NpcAttr[m_NpcTable[index].index].attack == 3 || 
//		 	 g_Pc_Manager.m_NpcAttr[m_NpcTable[index].index].attack == 4 || 
//			 g_Pc_Manager.m_NpcAttr[m_NpcTable[index].index].attack == 5 || 
//			 g_Pc_Manager.m_NpcAttr[m_NpcTable[index].index].attack == 6 )
//	{
		// �����������̽� ���
//	}
	return false;
}


D3DXVECTOR3	CField::GetNpcBoundMax( int index, long unique )
{
	int i = 0;
	for(  i = 0; i < MAX_NPC_INTABLE; i ++ )
	{
		if( //m_NpcTable[i].index == index && 
			m_NpcTable[i].lUnique == unique && m_NpcTable[i].race == c_NpcRace_Mob )		
			break;				
	}	
	
	return ( m_Npc[i].GetBoundMaxVec() - m_Npc[i].GetBoundMinVec() );
}

float	CField::GetNpcBoundSize( int index, long unique )
{
	D3DXVECTOR3 v;
	int i = 0;
	for(  i = 0; i < MAX_NPC_INTABLE; i ++ )
	{
		if( //m_NpcTable[i].index == index && 
			m_NpcTable[i].lUnique == unique && m_NpcTable[i].race == c_NpcRace_Mob )		
			break;				
	}	
	
	v = ( m_Npc[i].GetBoundMaxPos() - m_Npc[i].GetBoundMinPos() )/2.0f;
	return (float)sqrt((double)(v.x*v.x + v.y*v.y + v.z*v.z));
}


D3DXMATRIX CField::GetNpcMatrix( int index, long unique )
{
	int i = 0;
	for( i = 0; i < MAX_NPC_INTABLE; i ++ )
	{
		if( m_NpcTable[i].index == index && m_NpcTable[i].lUnique == unique && m_NpcTable[i].race == c_NpcRace_Mob )		
		{
			return m_Npc[i].GetPosTM();
		}

	}		
	
	return m_Npc[i].GetPosTM();
}

D3DXMATRIX CField::GetNpcCenterBoneMatrix( int index, long unique )
{
	int i = 0;
	for( i = 0; i < MAX_NPC_INTABLE; i ++ )
	{
		if( m_NpcTable[i].index == index && m_NpcTable[i].lUnique == unique && m_NpcTable[i].race == c_NpcRace_Mob )		
		{
			return m_Npc[i].GetPosCenterBoneTM();
		}

	}		
	
	return m_Npc[i].GetPosCenterBoneTM();
}


//-----------------------------------------------------------------------------
// Desc: NPC ����
//-----------------------------------------------------------------------------
int CField::SM__NPC__APPEAR( int index, DWORD unique, DWORD pos, DWORD hp, DWORD sp, BYTE status , float fAngle )
{	
	
	RLG1( "-------------------- SM_NPC_APPEAR : %d ---------------------------------------" , index );
	
	bool bFindNpc = false;

	bFindNpc = g_Pc_Manager.IsFindFileData( (DWORD)index );
	
	if( !bFindNpc )
	{
		if( !g_Pc_Manager.AddNpcCode( index ) )
		{
#ifdef ROCKCLIENT_DEV		
			RLG2( "!����ʿ� Code:%d NPC�� npc_%d.set ���Ͽ� ����!!!", index, g_Map.m_NowMapID );		
#endif 
			return -1;
		}
	}

	//-----------------------------------------------------------------------------
	// NPC���̺� ����ִ��� ã��
	//-----------------------------------------------------------------------------
	int who = FindNpc( unique );
		
	//-----------------------------------------------------------------------------
	// ���� NPC�̱⶧���� �ε��ؾ���
	//-----------------------------------------------------------------------------
	if( who == MAX_NPC_INTABLE )
	{		
		who = GetEmptyTable();

		if( who == MAX_NPC_INTABLE )
		{
			DebugPrint( "SM__NPC__APPEAR �ʰ�!!!\n" );
			return -1;
		}

		SNPCBaseInfo* npc_Info;
		npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( index ); 
		
		//-----------------------------------------------------------------------------
		// ���̺� ���� 
		//-----------------------------------------------------------------------------
		m_NpcTable[who].code		= index;					
		m_NpcTable[who].index		= npc_Info->theFileCode;	// m_pNpcFileInfo[index] ������� �ִ� ���� ������ ���� (ChrIndex)
		m_NpcTable[who].theLevel	= npc_Info->theLevel;		// ���� ���� ǥ�� 
		m_NpcTable[who].lUnique		= unique;
		m_NpcTable[who].lBlock		= pos;			
		m_NpcTable[who].lDestBlock	= pos;		
		m_NpcTable[who].nStatus		= status;					// �� ���¿� ���� ���� ( ���� ��� X )
		m_NpcTable[who].race		= c_NpcRace_Mob;			// npc table �� ���� � Ÿ������ ����...
//		m_NpcTable[who].type		= type;						// �� ���� ���� ��� (�鸶, �渶, ���� ��...) ��� X 
//		m_NpcTable[who].nCurrentFloor = 0;
		Rsprintf( RWCHAR(m_NpcTable[who].szName), _RT("%s"), RWCHAR(npc_Info->theName)  );
		m_NpcTable[who].theHP		= hp;
		m_NpcTable[who].theSMP		= sp;
		m_NpcTable[who].theMaxHP	= npc_Info->theMaxHP;
		m_NpcTable[who].theRealHP	= hp;
		m_NpcTable[who].theNpcSize	= npc_Info->theSize;

		//-----------------------------------------------------------------------------
		// NPC ���̶�� race�� ���� ����
		// �����ô� �Ͷ�����.. ���� ���� �˰� �־�� �Ѵ�
		//-----------------------------------------------------------------------------
//		if( FindHorse( who ) == true || STID_INDEX == index )		
//			m_NpcTable[who].race = HORSE_RACE;
				
		//-----------------------------------------------------------------------------
		// ĳ���� ����
		//-----------------------------------------------------------------------------
		m_Npc[who].m_bPC			= false;		
		m_Npc[who].m_nTableIndex	= who;
		m_Npc[who].m_State.bWalk	= true;
		
		if( npc_Info->theRace == NPC_RACE_BIGMONSTER )
		{
			m_Npc[who].m_State.nCharacterType = nCharacterTypeBigMoster;
			m_GiantUnitList.push_back((Character *)(&m_Npc[who]));	
		}

		if( npc_Info->theRace == NPC_RACE_BUILDING )
		{
			m_Npc[who].m_State.nCharacterType = nCharacterTypeBulding;
			m_GiantUnitList.push_back((Character *)(&m_Npc[who]));
			m_GiantBilbList.push_back((Character *)(&m_Npc[who]));
		}


		if( m_Npc[who].CreateNonPlayer() == false )
		{
			m_NpcTable[who].Init();
			return who;
		}
		//.....................................................................................................
		// NPC ���� ( �������� �ִ� ������ ó�� By wxywxy )
		//.....................................................................................................
		if( !m_Npc[who].m_bPC )
		{
			if( ( m_NpcTable[m_Npc[who].m_nTableIndex].code >= c_NonplayerCode_Min  
				&& m_NpcTable[m_Npc[who].m_nTableIndex].code <  c_NonplayerCode_Max  ) ||
				( m_NpcTable[m_Npc[who].m_nTableIndex].code >= c_NotMoveNPCCode_Min 
				&& m_NpcTable[m_Npc[who].m_nTableIndex].code <= c_NotMoveNPCCode_Max ) ||
				m_Npc[who].m_State.nCharacterType == nCharacterTypeBulding )
			{
				m_Npc[who].SetDirection( D3DXToRadian( fAngle ) );
				DebugPrint( "------ NPC : %d SetDirection : %f ------\n" , 
					m_NpcTable[m_Npc[who].m_nTableIndex].code ,	 fAngle );				
			}
		}
		//.....................................................................................................
		
		m_Npc[who].SetTilePos( (int)(pos%g_Pc.m_cSize), (int)(pos/g_Pc.m_cSize) );	
		m_dwNpcCount++;		

//.............................................................................................................
// ����Ʈ ����
//.............................................................................................................
		//by simwoosung
		if(::IsNonPlayerCode(index))
		{
			UpdateNPCQuestMark(index, who);
		}
//.............................................................................................................
		
		//-----------------------------------------------------------------------------
		// NPC���̴�? �ؽ��İ˻�	
		//-----------------------------------------------------------------------------
		UpdateNpc( who );
		
		//-----------------------------------------------------------------------------
		// �������϶��� �ϴ� ó��
		//-----------------------------------------------------------------------------
		if( g_RockClient.GetGameMode() == GMODE_NORMAL )
		{
			// ���üũ
			g_Map.SetMapData( NPC_BLOCK, who, pos );
			
			// ��ƼŬ �ִ� ���̸� ��ƼŬ ����			
			// g_Particle.Make_Npc_Effect( index, unique );
		}
	}
	//-----------------------------------------------------------------------------
	// �ִ� ���ε� �ٽ� �޾Ҵٸ� ��ġ���� �ٲٱ�
	//-----------------------------------------------------------------------------
	else
	{			
		DebugPrint( "!\r\n SM__NPC__APPEAR:NPC �� ����" );
	}
	return who;
}

int	CField::SM__PLAYER_NPC__APPEAR(int index, DWORD unique, DWORD pos, DWORD hp, DWORD sp, BYTE status , float fAngle, BYTE PetLevel )
{
	RLG1( "-------------------- SM_PLAYER_NPC_APPEAR : %d ---------------------------------------" , index );

	SPlayerNPCData PNPCData = g_Pc_Manager.m_PlayerNPCDataManager.FindPlayerNPCData(index);
	if( index != PNPCData.NPC_Code )
	{
		return -1;
	}	
	
	//-----------------------------------------------------------------------------
	// NPC���̺� ����ִ��� ã��
	//-----------------------------------------------------------------------------
	int who = FindNpc( unique );
		
	//-----------------------------------------------------------------------------
	// ���� NPC�̱⶧���� �ε��ؾ���
	//-----------------------------------------------------------------------------
	if( who == MAX_NPC_INTABLE )
	{		
		who = GetEmptyTable();

		if( who == MAX_NPC_INTABLE )
		{
			DebugPrint( "SM__PLAYER_NPC__APPEAR �ʰ�!!!\n" );
			return -1;
		}

		SNPCBaseInfo* npc_Info;
		npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( index );
		
		//-----------------------------------------------------------------------------
		// ���̺� ���� 
		//-----------------------------------------------------------------------------
		m_NpcTable[who].code		= index;		
		if( PetLevel )
		{
			m_NpcTable[who].theLevel	= PetLevel;
		}
		else
		{
			m_NpcTable[who].theLevel	= npc_Info->theLevel;		// ���� ���� ǥ�� 
		}
		m_NpcTable[who].lUnique		= unique;
		m_NpcTable[who].lBlock		= pos;			
		m_NpcTable[who].lDestBlock	= pos;		
		m_NpcTable[who].nStatus		= status;					// �� ���¿� ���� ���� ( ���� ��� X )
		m_NpcTable[who].race		= c_NpcRace_Mob;			// npc table �� ���� � Ÿ������ ����...
		Rsprintf( RWCHAR(m_NpcTable[who].szName) , _RT("%s"), RWCHAR(npc_Info->theName) );
		m_NpcTable[who].theHP		= hp;
		m_NpcTable[who].theSMP		= sp;
		m_NpcTable[who].theMaxHP	= npc_Info->theMaxHP;
		m_NpcTable[who].theRealHP	= hp;
		m_NpcTable[who].theNpcSize	= npc_Info->theSize;

		///-- ���ʿ��� �ܺ� ���̺��� �ҷ����� �ش� �����ͷ� ���ʵ带 ä���.		
		///-- �ε��� ���� ������ ������ �ٽ������� �ް��Ѵ�.		
		m_NpcTable[who].theExPcInfo.theCitizen = PNPCData.Citizen;
		m_NpcTable[who].theExPcInfo.theGender = PNPCData.Gender;
		m_NpcTable[who].theExPcInfo.theMainClass = PNPCData.MainClass;
		m_NpcTable[who].theExPcInfo.theFace = PNPCData.Face_Code;
		m_NpcTable[who].theExPcInfo.theHair = PNPCData.Hair_Code;
		m_NpcTable[who].theExPcInfo.theWearItem[n_WearSlot_CoatArmor] = PNPCData.CoatArmor_Code;
		m_NpcTable[who].theExPcInfo.theWearItem[n_WearSlot_PantsArmor] = PNPCData.PantsArmor_Code;
		m_NpcTable[who].theExPcInfo.theWearItem[n_WearSlot_Gloves] = PNPCData.Gloves_Code;
		m_NpcTable[who].theExPcInfo.theWearItem[n_WearSlot_Shoes] = PNPCData.Shoes_Code;
		m_NpcTable[who].theExPcInfo.theWearItem[n_WearSlot_Helmet] = PNPCData.Helmet_Code;
		m_NpcTable[who].theExPcInfo.theWearItem[n_WearSlot_Serve] = PNPCData.Serve_Code;
		m_NpcTable[who].theExPcInfo.theWearItem[n_WearSlot_Shield] = PNPCData.LeftArm_Code;
		m_NpcTable[who].theExPcInfo.theWearItem[n_WearSlot_Weapon] = PNPCData.RightArm_Code;		
		
		m_NpcTable[who].index = g_Pc.GetAttrIndex( m_NpcTable[who].theExPcInfo.theCitizen, 
												   m_NpcTable[who].theExPcInfo.theGender );		
		//-----------------------------------------------------------------------------
		// ĳ���� ����
		//-----------------------------------------------------------------------------	
		m_Npc[who].m_bPC			= false;		
		m_Npc[who].m_nTableIndex	= who;
		m_Npc[who].m_State.bWalk	= true;

		m_Npc[who].m_State.nCharacterType = nCharacterTypePlayerNPC;

		bool IsPet = false;

		if( PetLevel )
		{
			IsPet =	true;
		}
		else
		{
			IsPet =	false;
		}
		
		if( m_Npc[who].CreateNonPlayer(IsPet) == false )
		{
			m_NpcTable[who].Init();
			return who;
		}

		//.....................................................................................................
		// NPC ���� ( �������� �ִ� ������ ó�� By wxywxy )
		//.....................................................................................................
		if( !m_Npc[who].m_bPC )
		{
			if( ( m_NpcTable[m_Npc[who].m_nTableIndex].code >= c_NonplayerCode_Min  
					&& m_NpcTable[m_Npc[who].m_nTableIndex].code <  c_NonplayerCode_Max  ) ||
				( m_NpcTable[m_Npc[who].m_nTableIndex].code >= c_NotMoveNPCCode_Min 
					&& m_NpcTable[m_Npc[who].m_nTableIndex].code <= c_NotMoveNPCCode_Max )   )
			{
				m_Npc[who].SetDirection( D3DXToRadian( fAngle ) );
				DebugPrint( "------ NPC : %d SetDirection : %f ------\n" , m_NpcTable[m_Npc[who].m_nTableIndex].code ,
															 fAngle );			
			}
		}
		//.....................................................................................................
		
		m_Npc[who].SetTilePos( (int)(pos%g_Pc.m_cSize), (int)(pos/g_Pc.m_cSize) );	
		m_dwNpcCount++;			

		//-----------------------------------------------------------------------------
		// NPC���̴�? �ؽ��İ˻�	
		//-----------------------------------------------------------------------------
		UpdateNpc( who );
		
		if( g_RockClient.GetGameMode() == GMODE_NORMAL )
		{
			// ���üũ
			g_Map.SetMapData( NPC_BLOCK, who, pos );
		}
		
	}
	//-----------------------------------------------------------------------------
	// �ִ� ���ε� �ٽ� �޾Ҵٸ� ��ġ���� �ٲٱ�
	//-----------------------------------------------------------------------------
	else
	{			
		DebugPrint( "!\r\n SM__PLAYER_NPC__APPEAR:NPC �� ����" );
	}
	
	return who;	
}

int	CField::SM__RIDE__APPEAR(int index, DWORD unique, DWORD pos, DWORD hp, DWORD sp, BYTE status , float fAngle)
{
	RLG1( "-------------------- SM_RIDE_APPEAR : %d ---------------------------------------" , index );
	
	bool bFindNpc = false; 

	bFindNpc = g_Pc_Manager.IsFindFileData( (DWORD)index );

	if( !bFindNpc )
	{
		if( !g_Pc_Manager.AddNpcCode( index ) )
		{
#ifdef ROCKCLIENT_DEV		
			RLG2( "!����ʿ� Code:%d NPC�� npc_%d.set ���Ͽ� ����!!!", index, g_Map.m_NowMapID );			
#endif 
			return -1;
		}
	}

	//-----------------------------------------------------------------------------
	// NPC���̺� ����ִ��� ã��
	//-----------------------------------------------------------------------------
	int who = FindNpc( unique, c_NpcRace_Transpotation );
		
	//-----------------------------------------------------------------------------
	// ���� NPC�̱⶧���� �ε��ؾ���
	//-----------------------------------------------------------------------------
	if( who == MAX_NPC_INTABLE )
	{		
		who = GetEmptyTable();

		if( who == MAX_NPC_INTABLE )
		{
			DebugPrint( "SM__RIDE__APPEAR �ʰ�!!!\n" );
			return -1;
		}

		SNPCBaseInfo* npc_Info;
		npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( index ); 
		
		//-----------------------------------------------------------------------------
		// ���̺� ���� 
		//-----------------------------------------------------------------------------
		m_NpcTable[who].code		= index;					
		m_NpcTable[who].index		= npc_Info->theFileCode;	// m_pNpcFileInfo[index] ������� �ִ� ���� ������ ���� (ChrIndex)
		m_NpcTable[who].theLevel	= npc_Info->theLevel;		// ���� ���� ǥ�� 
		m_NpcTable[who].lUnique		= unique;
		m_NpcTable[who].lBlock		= pos;			
		m_NpcTable[who].lDestBlock	= pos;		
		m_NpcTable[who].nStatus		= status;					// �� ���¿� ���� ���� ( ���� ��� X )
		m_NpcTable[who].race		= c_NpcRace_Transpotation;	// npc table �� ���� � Ÿ������ ����...
//		m_NpcTable[who].type		= type;						// �� ���� ���� ��� (�鸶, �渶, ���� ��...) ��� X 
//		m_NpcTable[who].nCurrentFloor = 0;
		Rsprintf( RWCHAR(m_NpcTable[who].szName) , _RT("%s"), RWCHAR(npc_Info->theName) );
		m_NpcTable[who].theHP		= hp;
		m_NpcTable[who].theSMP		= sp;
		m_NpcTable[who].theMaxHP	= npc_Info->theMaxHP;
		m_NpcTable[who].theRealHP	= hp;
		m_NpcTable[who].theNpcSize	= npc_Info->theSize;

		//-----------------------------------------------------------------------------
		// NPC ���̶�� race�� ���� ����
		// �����ô� �Ͷ�����.. ���� ���� �˰� �־�� �Ѵ�
		//-----------------------------------------------------------------------------
//		if( FindHorse( who ) == true || STID_INDEX == index )		
//			m_NpcTable[who].race = HORSE_RACE;
				
		//-----------------------------------------------------------------------------
		// ĳ���� ����
		//-----------------------------------------------------------------------------	
		m_Npc[who].m_bPC			= false;		
		m_Npc[who].m_nTableIndex	= who;
		m_Npc[who].m_State.bWalk	= true;
		if( m_Npc[who].CreateNonPlayer() == false )
		{
			m_NpcTable[who].Init();
			return who;
		}

		//.....................................................................................................
		// NPC ���� ( �������� �ִ� ������ ó�� By wxywxy )
		//.....................................................................................................
		if( !m_Npc[who].m_bPC )
		{
			if( ( m_NpcTable[m_Npc[who].m_nTableIndex].code >= c_NonplayerCode_Min  && m_NpcTable[m_Npc[who].m_nTableIndex].code <  c_NonplayerCode_Max  ) ||
				( m_NpcTable[m_Npc[who].m_nTableIndex].code >= c_NotMoveNPCCode_Min && m_NpcTable[m_Npc[who].m_nTableIndex].code <= c_NotMoveNPCCode_Max )   )
			{
				m_Npc[who].SetDirection( D3DXToRadian( fAngle ) );
				DebugPrint( "------ NPC : %d SetDirection : %f ------\n" , m_NpcTable[m_Npc[who].m_nTableIndex].code ,
															 fAngle );

				//m_Npc[who].SetDirection( fAngle );
			}
		}
		//.....................................................................................................
		
		m_Npc[who].SetTilePos( (int)(pos%g_Pc.m_cSize), (int)(pos/g_Pc.m_cSize) );	
		m_dwNpcCount++;	

		
/*
		// �ӽ� �׽�Ʈ
		g_Particle.SetEffSetCon( 0.0f , 3987			, 0 , 999.0f , unique , m_Npc[who].GetIndex() );
		g_Particle.SetEffSetCon( 0.0f , EF_RAIN_COMBOEX , 0 , 999.0f , unique , m_Npc[who].GetIndex() );
		//
*/
		//-----------------------------------------------------------------------------
		// NPC���̴�? �ؽ��İ˻�	
		//-----------------------------------------------------------------------------
		UpdateNpc( who );		
		
	}
	//-----------------------------------------------------------------------------
	// �ִ� ���ε� �ٽ� �޾Ҵٸ� ��ġ���� �ٲٱ�
	//-----------------------------------------------------------------------------
	else
	{			
		DebugPrint( "!\r\n SM__RIDE__APPEAR:NPC �� ����" );
	}
	return who;
}

void CField::SM__RIDE__DISAPPEAR( int index, DWORD unique )
{
	int who = FindNpc( unique, c_NpcRace_Transpotation );
	if( who < MAX_NPC_INTABLE )
	{			
		// �ش� Ÿ���� ����Ʈ����
		g_Particle.StopDisplay( unique, index );
		g_Billboard.DelManager( unique, index );
//		g_ChatList.DelUserBalloon( unique, index );

		// ����Ʈ ����Ʈ ����� By wxywxy
		g_Particle.m_EffectManager.DeleteNow( ( Character * ) &m_Npc[who] );
		g_Particle.DelEffSetCon( m_Npc[who].GetUnique() , m_Npc[who].GetIndex() );
		
//		std::vector<int>::iterator ir = g_Map.m_Npc[who].thePreQuestCodeList.begin();
//		for( ; ir != g_Map.m_Npc[who].thePreQuestCodeList.end() ; ++ir )
//		{
			g_Map.m_Npc[who].thePreQuestCodeList.clear();
			g_Map.m_Npc[who].m_QuestMarkType = NONE_QUEST_MARK;
//		}				

		// �ʺ�����(Ÿ���� �׾��ִ� ���¶�� ������ �ʿ����)
//		if( m_Npc[who].m_curt_event.type != SM_STATE_DIE )
			//g_Map.SetMapData( CLEAR_BLOCK, who, m_NpcTable[who].lDestBlock, false );

		// NPC���̺����
		m_NpcTable[who].Init();
		m_Npc[who].Destroy();
		
		m_Npc[who].m_bIsLoaded = false;
		m_Npc[who].m_bIsVisible = false;		
		m_dwNpcCount--;
	}
	else
	{
		DebugPrint( "!\r\n SM__RIDE__DISAPPEAR:NPC ��á����" );
	}
}

int	CField::SM__PET__APPEAR(int index, DWORD unique, DWORD pos, DWORD hp, DWORD sp, BYTE status , BYTE PetLevel ,float fAngle)
{
	RLG1( "-------------------- SM_RIDE_APPEAR : %d ---------------------------------------" , index );
	
	bool bFindNpc = false; 

	bFindNpc = g_Pc_Manager.IsFindFileData( (DWORD)index );

	if( !bFindNpc )
	{
		if( !g_Pc_Manager.AddNpcCode( index ) )
		{
#ifdef ROCKCLIENT_DEV		
			RLG2( "!����ʿ� Code:%d NPC�� npc_%d.set ���Ͽ� ����!!!", index, g_Map.m_NowMapID );	
#endif 
			return -1;
		}
	}

	//-----------------------------------------------------------------------------
	// NPC���̺� ����ִ��� ã��
	//-----------------------------------------------------------------------------
	int who = FindNpc( unique );
		
	//-----------------------------------------------------------------------------
	// ���� NPC�̱⶧���� �ε��ؾ���
	//-----------------------------------------------------------------------------
	if( who == MAX_NPC_INTABLE )
	{		
		who = GetEmptyTable();

		if( who == MAX_NPC_INTABLE )
		{
			DebugPrint( "SM__PET__APPEAR �ʰ�!!!\n" );
			return -1;
		}

		SNPCBaseInfo* npc_Info;
		npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( index ); 
		
		//-----------------------------------------------------------------------------
		// ���̺� ���� 
		//-----------------------------------------------------------------------------
		m_NpcTable[who].code		= index;					
		m_NpcTable[who].index		= npc_Info->theFileCode;	// m_pNpcFileInfo[index] ������� �ִ� ���� ������ ���� (ChrIndex)
		m_NpcTable[who].theLevel	= PetLevel;		// ���� ���� ǥ�� 
		m_NpcTable[who].lUnique		= unique;
		m_NpcTable[who].lBlock		= pos;			
		m_NpcTable[who].lDestBlock	= pos;		
		m_NpcTable[who].nStatus		= status;					// �� ���¿� ���� ���� ( ���� ��� X )
		m_NpcTable[who].race		= c_NpcRace_Mob;			// npc table �� ���� � Ÿ������ ����...
//		m_NpcTable[who].type		= type;						// �� ���� ���� ��� (�鸶, �渶, ���� ��...) ��� X 
//		m_NpcTable[who].nCurrentFloor = 0;
		Rsprintf( RWCHAR(m_NpcTable[who].szName), _RT("%s") , RWCHAR(npc_Info->theName) );
		m_NpcTable[who].theHP		= hp;
		m_NpcTable[who].theSMP		= sp;
		m_NpcTable[who].theMaxHP	= npc_Info->theMaxHP;
		m_NpcTable[who].theRealHP	= hp;
		m_NpcTable[who].theNpcSize	= npc_Info->theSize;
		
		//-----------------------------------------------------------------------------
		// NPC ���̶�� race�� ���� ����
		// �����ô� �Ͷ�����.. ���� ���� �˰� �־�� �Ѵ�
		//-----------------------------------------------------------------------------
//		if( FindHorse( who ) == true || STID_INDEX == index )		
//			m_NpcTable[who].race = HORSE_RACE;
				
		//-----------------------------------------------------------------------------
		// ĳ���� ����
		//-----------------------------------------------------------------------------	
		m_Npc[who].m_bPC			= false;		
		m_Npc[who].m_nTableIndex	= who;
		m_Npc[who].m_State.bWalk	= true;
		if( m_Npc[who].CreateNonPlayer(true) == false )
		{
			m_NpcTable[who].Init();
			return who;
		}

		//.....................................................................................................
		// NPC ���� ( �������� �ִ� ������ ó�� By wxywxy )
		//.....................................................................................................
		if( !m_Npc[who].m_bPC )
		{
			if( ( m_NpcTable[m_Npc[who].m_nTableIndex].code >= c_NonplayerCode_Min  && m_NpcTable[m_Npc[who].m_nTableIndex].code <  c_NonplayerCode_Max  ) ||
				( m_NpcTable[m_Npc[who].m_nTableIndex].code >= c_NotMoveNPCCode_Min && m_NpcTable[m_Npc[who].m_nTableIndex].code <= c_NotMoveNPCCode_Max )   )
			{
				m_Npc[who].SetDirection( D3DXToRadian( fAngle ) );
				DebugPrint( "------ NPC : %d SetDirection : %f ------\n" , m_NpcTable[m_Npc[who].m_nTableIndex].code ,
															 fAngle );

				//m_Npc[who].SetDirection( fAngle );
			}
		}
		//.....................................................................................................
		
		m_Npc[who].SetTilePos( (int)(pos%g_Pc.m_cSize), (int)(pos/g_Pc.m_cSize) );	
		m_dwNpcCount++;	

		
/*
		// �ӽ� �׽�Ʈ
		g_Particle.SetEffSetCon( 0.0f , 3987			, 0 , 999.0f , unique , m_Npc[who].GetIndex() );
		g_Particle.SetEffSetCon( 0.0f , EF_RAIN_COMBOEX , 0 , 999.0f , unique , m_Npc[who].GetIndex() );
		//
*/
		//-----------------------------------------------------------------------------
		// NPC���̴�? �ؽ��İ˻�	
		//-----------------------------------------------------------------------------
		UpdateNpc( who );
		
		if( g_RockClient.GetGameMode() == GMODE_NORMAL )
		{
			// ���üũ
			g_Map.SetMapData( NPC_BLOCK, who, pos );
			
			// ��ƼŬ �ִ� ���̸� ��ƼŬ ����			
			// g_Particle.Make_Npc_Effect( index, unique );
		}
		
	}
	//-----------------------------------------------------------------------------
	// �ִ� ���ε� �ٽ� �޾Ҵٸ� ��ġ���� �ٲٱ�
	//-----------------------------------------------------------------------------
	else
	{			
		DebugPrint( "!\r\n SM__RIDE__APPEAR:NPC �� ����" );
	}
	return who;
}


//-----------------------------------------------------------------------------
// Desc: NPC ����
//-----------------------------------------------------------------------------
void	CField::SM__NPC__DISAPPEAR( int index, DWORD unique )
{
	int who = FindNpc( unique );
	if( who < MAX_NPC_INTABLE )
	{			
		
		if( g_Map.IsGiantUnit( &m_Npc[who] ) )
		{
			list < Character * >::iterator iPos = m_GiantUnitList.begin();		
			Character *pDisChr = (Character *)(&m_Npc[who]);
			for( ; iPos != m_GiantUnitList.end() ; iPos++)
			{
				Character * pChr = (Character *)(*iPos);
				if(pDisChr == pChr)
				{
					m_GiantUnitList.erase(iPos);
					break;
				}
			}
		}

		if( m_Npc[who].m_State.nCharacterType == nCharacterTypeBulding )
		{
			list < Character * >::iterator iPos = m_GiantBilbList.begin();		
			Character *pDisChr = (Character *)(&m_Npc[who]);
			for( ; iPos != m_GiantBilbList.end() ; iPos++)
			{
				Character * pChr = (Character *)(*iPos);
				if(pDisChr == pChr)
				{
					m_GiantBilbList.erase(iPos);
					break;
				}
			}
		}
		
		///-- ť�� �����ִ� ���������ƾ�� �ϰ�ó���Ѵ�.
		m_Npc[who].AllActQueue();		
		
		// �ش� Ÿ���� ����Ʈ����
		g_Particle.StopDisplay( unique, index );
		g_Billboard.DelManager( unique, index );
//		g_ChatList.DelUserBalloon( unique, index );

		// ����Ʈ ����Ʈ ����� By wxywxy
		g_Particle.m_EffectManager.DeleteNow( ( Character * ) &m_Npc[who] );
		g_Particle.DelEffSetCon( m_Npc[who].GetUnique() , m_Npc[who].GetIndex() );
		
//		std::vector<int>::iterator ir = g_Map.m_Npc[who].thePreQuestCodeList.begin();
//		for( ; ir != g_Map.m_Npc[who].thePreQuestCodeList.end() ; ++ir )
//		{
			g_Map.m_Npc[who].thePreQuestCodeList.clear();
			g_Map.m_Npc[who].m_QuestMarkType = NONE_QUEST_MARK;
//		}				

		// �ʺ�����(Ÿ���� �׾��ִ� ���¶�� ������ �ʿ����)
//		if( m_Npc[who].m_curt_event.type != SM_STATE_DIE )
			g_Map.SetMapData( CLEAR_BLOCK, who, m_NpcTable[who].lDestBlock, false );

		// NPC���̺����
		m_NpcTable[who].Init();
		m_Npc[who].Destroy();
		
		m_Npc[who].m_bIsLoaded = false;
		m_Npc[who].m_bIsVisible = false;		
		m_dwNpcCount--;
	}
	else
	{
		DebugPrint( "!\r\n SM__NPC__DISAPPEAR:NPC ��á����" );
	}
}


//-----------------------------------------------------------------------------
// Desc: NPC ����
//-----------------------------------------------------------------------------
int CField::SM__ITEM__APPEAR( DWORD aUnique, DWORD aCell, WORD aItemCode )
{		
	//-----------------------------------------------------------------------------
	// NPC���̺� ����ִ��� ã��
	//-----------------------------------------------------------------------------
	int item_table = FindNpc( aUnique, c_NpcRace_Item );
		
	//-----------------------------------------------------------------------------
	// ���� NPC�̱⶧���� �ε��ؾ���
	//-----------------------------------------------------------------------------
	if( item_table == MAX_NPC_INTABLE )
	{		
		item_table = GetEmptyTable();
		
		if( item_table == MAX_NPC_INTABLE )
		{
			DebugPrint( "SM__ITEM__APPEAR �ʰ�!!!\n" );
			return -1;
		}

		SItemBaseInfo* item_Info = NULL;
		item_Info = g_Pc_Manager.GetItemBaseInfoPtr( aItemCode ); 
		if( item_Info == NULL )
			return -1;

		int NpcChr_Index = g_Pc_Manager.the_sNpc_SetData.total_npc_set_num - c_MaxDropItemIndex;

		//-----------------------------------------------------------------------------
		// ���̺� ���� 
		//-----------------------------------------------------------------------------
		m_NpcTable[item_table].code			= 1; // aItemCode;	// �������� NPC �ڵ忡 �߰��Ҷ����� �ӽ÷� ��...�Ѥ�;		
//		m_NpcTable[item_table].index		= npc_Info->theFileCode;	

		if( item_Info->theClass == n_Class_Weapon || 
			item_Info->theClass == n_Class_CoatArmor || 
			item_Info->theClass == n_Class_PantsArmor ||
			item_Info->theClass == n_Class_Gloves ||
			item_Info->theClass == n_Class_Shoes ||
			item_Info->theClass == n_Class_Serve ||
			item_Info->theClass == n_Class_Helmet ||
			item_Info->theClass == n_Class_Accessory || 
			item_Info->theClass == n_Class_Shield )
		{
			if( item_Info->theRare >= 0 && item_Info->theRare <= 30 )
			{
				m_NpcTable[item_table].index = NpcChr_Index + n_NormalBox;
			}
			else if( item_Info->theRare >= 31 && item_Info->theRare <= 70 )
			{
				m_NpcTable[item_table].index = NpcChr_Index + n_GoodBox;
			}
			else
			{
				m_NpcTable[item_table].index = NpcChr_Index + n_QuestBox;
			}			
		}
		else if( item_Info->theClass == n_Class_Use || 
			item_Info->theClass == n_Class_Useless || 
			item_Info->theClass == n_Class_Special )
		{
			// ���� ���� ó�� 
			if( item_Info->theClass == n_Class_Special && item_Info->theType == n_Type_Eld )
			{
				m_NpcTable[item_table].index = NpcChr_Index + n_Eld;
			}
			else
			{
				if( item_Info->theRare >= 0 && item_Info->theRare <= 70 )
				{
					m_NpcTable[item_table].index = NpcChr_Index + n_Pack;
				}
				else
				{
					m_NpcTable[item_table].index = NpcChr_Index + n_QuestBox;;
				}
			}

		}
		else if( item_Info->theClass == n_Class_Quest )
		{
			m_NpcTable[item_table].index = NpcChr_Index + n_QuestBox;
		}
		else
		{
			m_NpcTable[item_table].index = NpcChr_Index;	// �� ������ ������ ������ �ε��� ���� 
		}
		
		m_NpcTable[item_table].lUnique		= aUnique;
		m_NpcTable[item_table].lBlock		= aCell;			
		m_NpcTable[item_table].lDestBlock	= aCell;		
		m_NpcTable[item_table].race			= c_NpcRace_Item;
		
		Rsprintf( RWCHAR(m_NpcTable[item_table].szName) , _RT("%s"), RWCHAR(item_Info->theName) );
		
		//-----------------------------------------------------------------------------
		// ĳ���� ����
		//-----------------------------------------------------------------------------		
		m_Npc[item_table].m_bPC			= false;		
		m_Npc[item_table].m_nTableIndex	= item_table;
		m_Npc[item_table].m_State.bWalk	= true;

		if( m_Npc[item_table].CreateNonPlayer() == false )
		{
			m_NpcTable[item_table].Init();
			return item_table;
		}
		m_Npc[item_table].SetTilePos( (int)(aCell%g_Pc.m_cSize), (int)(aCell/g_Pc.m_cSize) );	
		m_dwNpcCount++;	
		
		//-----------------------------------------------------------------------------
		// NPC���̴�? �ؽ��İ˻�	
		//-----------------------------------------------------------------------------
		UpdateNpc( item_table );
		
		//-----------------------------------------------------------------------------
		// �������϶��� �ϴ� ó��
		//-----------------------------------------------------------------------------
//		if( g_RockClient.GetGameMode() == GMODE_NORMAL )
//		{
			// ��üũ
//			g_Map.SetMapData( NPC_BLOCK, item_table, aCell );
//		}
		
		// ���ٸ� �� ���� �ִϸ��̼� ó���� ���ָ� �ȴ� 
		EVENT_DATA_INFO drop_event;
		drop_event.type = SM_ITEM_DROPING;
		m_Npc[item_table].PushQueue( drop_event, TRUE );
	}
	//-----------------------------------------------------------------------------
	// �ִ� ���ε� �ٽ� �޾Ҵٸ� ��ġ���� �ٲٱ�
	//-----------------------------------------------------------------------------
	else
	{			
		DebugPrint( "!SM__ITEM__APPEAR : �� �����϶��??!!!\n" );
	}

	return item_table;
}


//-----------------------------------------------------------------------------
// Desc: NPC ����
//-----------------------------------------------------------------------------
void CField::SM__ITEM__DISAPPEAR( DWORD aUnique )
{
	int item_table = FindNpc( aUnique, c_NpcRace_Item );

	if( item_table < MAX_NPC_INTABLE )
	{			
		// �ش� Ÿ���� ����Ʈ����
		g_Particle.StopDisplay( aUnique, c_NpcIndex_Item );
		g_Billboard.DelManager( aUnique, c_NpcIndex_Item );
//		g_ChatList.DelUserBalloon( unique, index );

		// ����Ʈ ����Ʈ ����� By wxywxy
		g_Particle.m_EffectManager.Delete( ( Character * ) &m_Npc[item_table] );
		
		// �ʺ�����(Ÿ���� �׾��ִ� ���¶�� ������ �ʿ����)
//		if( m_Npc[item_table].m_curt_event.type != SM_STATE_DIE )
//			g_Map.SetMapData( CLEAR_BLOCK, item_table, m_NpcTable[item_table].lDestBlock );

		// NPC���̺����
		m_NpcTable[item_table].Init();
		m_Npc[item_table].Destroy();
		m_dwNpcCount--;
	}
	else
	{
		DebugPrint( "!\r\n SM__NPC__DISAPPEAR:NPC ��á����" );
	}
}


//-----------------------------------------------------------------------------
// Desc: NPC �з�����
//-----------------------------------------------------------------------------
void	CField::SM__SLIDE__NPC( int index, DWORD unique, DWORD pos )
{
	int who = FindNpc( unique );
	if( who < MAX_NPC_INTABLE )
	{		
		// �з����� �Ÿ�
		int cur_x	= m_NpcTable[who].lDestBlock%(m_cSize*2);
		int cur_y	= m_NpcTable[who].lDestBlock/(m_cSize*2);
		int des_x	= pos%(m_cSize*2);
		int des_y	= pos/(m_cSize*2);

		// �޼�������
		EVENT_DATA_INFO	data;
		data.type	= SM_SLIDE_NPC;
		data.x		= des_x;
		data.y		= des_y;
		data.vPos.x	= (des_x-cur_x)*CLIENT_TILESIZE;
		data.vPos.y	= g_Pc.GetMapHeight( des_x, des_y )-m_Npc[who].m_Mov.curt.height;
		data.vPos.z	= (des_y-cur_y)*CLIENT_TILESIZE;
		m_Npc[who].PushQueue( data, TRUE );
		m_Npc[who].QueueProcess();

		// ��ó��
		g_Map.SetMapData( NPC_BLOCK, who, pos, false );
		g_Map.SetMapData( CLEAR_BLOCK, who, m_NpcTable[who].lDestBlock, false );		

		// ��ġ�� ���̺� ����
		m_NpcTable[who].lDestBlock	= pos;
		m_NpcTable[who].lBlock		= pos;

		// Ŭ���̾�Ʈ ��ġ�� ����
		m_Npc[who].m_Mov.Init( pos%(m_cSize*2), pos/(m_cSize*2) );
	}
}

void	CField::UpdateNPCQuestMark(int nNpcCode, int nNpcIndex)
{	
	static int nGroupId[RP_MAX_QUEST_FLAG];
	int i = 0;
	for( i = 0; i < RP_MAX_QUEST_FLAG; ++i )
	{
		if(nRui->thePcInven.QuestFlag[i])
		{
			for( int j = 0; j < c_Max_PreQuestInfo_Count ; ++j )
			{
				if( nRui->thePcInven.QuestFlag[i]->theQuestCode == 
					g_Pc_Manager.thePreQuestInfoTable[j].theQuestCode )
				{
					nGroupId[i] = g_Pc_Manager.thePreQuestInfoTable[j].theQuestGCode;
					break;
				}
			}
		}
		else
		{
			nGroupId[i] = 0;
		}
	}	
	
	for( i = 0; i < c_Max_PreQuestInfo_Count ; ++i )
	{
		int quest_code  = g_Pc_Manager.thePreQuestInfoTable[i].theQuestCode;
		int npc_code    = g_Pc_Manager.thePreQuestInfoTable[i].theNPCCode;
		int npc_code2   = g_Pc_Manager.thePreQuestInfoTable[i].theAfterNPCCode;
		int need_level  = g_Pc_Manager.thePreQuestInfoTable[i].theNeedLevel;
		int FindNPCType = -1;	
		int ISDisplay   = g_Pc_Manager.thePreQuestInfoTable[i].theDisplayNPCEffect;
		int QuestGCode  = g_Pc_Manager.thePreQuestInfoTable[i].theQuestGCode;

		if( quest_code == 0 )
		{
			break;			
		}
		
		if( npc_code == nNpcCode )					// �ϴ� �ش� NPC����??!!...
		{
			FindNPCType = 0;
		}

		if( npc_code2 == nNpcCode )					// ���� NPC �̳�?
		{
			if(0 == FindNPCType)					// ���� NPC�� ���� NPC�� ������?
			{
				FindNPCType = 2;
			}
			else
			{
				FindNPCType = 1;
			}				
		}	

		if( FindNPCType == -1 )
		{
			continue;
		}

		if( need_level > nRui->thePcParam.Level )		// �׸��� ���� �� �� �ִ°ų�??!!..
		{
			continue;
		}
		
		bool		IsInsertQuest = true;
		QUESTDATA	QuestData;
		
		for( int j = 0; j < RP_MAX_QUEST_FLAG; ++j )
		{
			if( nGroupId[j] == 0 )	//�÷��� ���� ������� - ���۵� ���Ѱ���ε�...
			{
				// ���� NPC ���� ���� ���ڰ� �������ϱ� ����
				if( FindNPCType == 1 || ISDisplay == FALSE  )
				{
					IsInsertQuest = false;
					break;
				}

				RLGS("!���� NPC �� ���� : APPEAR");

				if( g_Map.m_Npc[nNpcIndex].m_QuestMarkType != REPORT_QUEST_MARK )
				{
					g_Map.m_Npc[nNpcIndex].m_QuestMarkType = START_QUEST_MARK;
					
					//by simwoosung
					SMinimapNPCInfo * pNPCInfo = NULL;
					pNPCInfo = MiniMapWnd.FindQuestNPC(quest_code, nNpcCode);
					if( pNPCInfo && (REPORT_QUEST_MARK != pNPCInfo->nQuestMarkType) )
					{
						pNPCInfo->nQuestMarkType = START_QUEST_MARK;
					}
				}

				QuestData.Code = quest_code;
				QuestData.Type = START_QUEST_MARK;
				break;
			}
			// �÷��� ���� �ִ� ���϶�
			else if( nGroupId[j] == QuestGCode )
			{
				//���� �׷������� ����Ʈ �ڵ尡 Ʋ����...
				if(nRui->thePcInven.QuestFlag[j]->theQuestCode != quest_code )
				{
					IsInsertQuest = false;
					break;
				}				
				
				///-- ����Ʈ ����	
				if( nRui->thePcInven.QuestFlag[j]->theFlag == 60 )
				{
					IsInsertQuest = false;
					break;
				}
				// �������϶�
				else
				if( nRui->thePcInven.QuestFlag[j]->theFlag != 60 && FindNPCType != 0 )
				{
					RLGS("!���� NPC �� ���� : APPEAR");

					g_Map.m_Npc[nNpcIndex].m_QuestMarkType = REPORT_QUEST_MARK;
					
					QuestData.Code = quest_code;
					QuestData.Type = REPORT_QUEST_MARK;
					
					break;
				}
				// �׿ܿ��� ����
				else
				{
					IsInsertQuest = false;
					break;
				}			
			}
		}
		
		if( IsInsertQuest )
		{
			// �÷��̾ ���� �ؾ� �� ����Ʈ�� �˷��ֱ� ���ؼ�...^ ^;;
			g_Map.m_Npc[nNpcIndex].thePreQuestCodeList.push_back( QuestData );
		}		
	}	
}

//................................................................................................................
// �ż� ����
//................................................................................................................
int CField::SM__HOLY__APPEAR( int index, DWORD unique, DWORD pos, DWORD hp, DWORD sp, BYTE status , float fAngle , BYTE Level)
{	
	
	RLG1( "-------------------- SM_HOLY_APPEAR : %d ---------------------------------------" , index );
	
	bool bFindNpc = false;
	int  BaseCode = index;
	
	index = g_Pc_Manager.m_HolyManager.GetHolyCode( BaseCode , Level );

	if( index == -1 )
		return -1;

	// NPC �� Ÿ�Կ� ���� ���� �����϶�� �ϸ� �ý��� �ٿ� 
	bFindNpc = g_Pc_Manager.IsFindFileData( (DWORD)index );
	
	if( !bFindNpc )
	{
		if( !g_Pc_Manager.AddNpcCode( index ) )
		{
#ifdef ROCKCLIENT_DEV		
		RLG2( "!����ʿ� Code:%d NPC�� npc_%d.set ���Ͽ� ����!!!", index, g_Map.m_NowMapID );	
#endif 		
			return -1;
		}		
	}

	//-----------------------------------------------------------------------------
	// NPC���̺� ����ִ��� ã��
	//-----------------------------------------------------------------------------
	int who = FindNpc( unique );
		
	//-----------------------------------------------------------------------------
	// ���� NPC�̱⶧���� �ε��ؾ���
	//-----------------------------------------------------------------------------
	if( who == MAX_NPC_INTABLE )
	{		
		who = GetEmptyTable();

		if( who == MAX_NPC_INTABLE )
		{
			DebugPrint( "SM__HOLY__APPEAR �ʰ�!!!\n" );
			return -1;
		}

		SNPCBaseInfo* npc_Info;
		npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( index ); 
		
		//-----------------------------------------------------------------------------
		// ���̺� ���� 
		//-----------------------------------------------------------------------------
		m_NpcTable[who].code		= index;					
		m_NpcTable[who].index		= npc_Info->theFileCode;	// m_pNpcFileInfo[index] ������� �ִ� ���� ������ ���� (ChrIndex)
		m_NpcTable[who].theLevel	= Level;					// �ż� ���� ǥ�� 
		m_NpcTable[who].lUnique		= unique;
		m_NpcTable[who].lBlock		= pos;			
		m_NpcTable[who].lDestBlock	= pos;		
		m_NpcTable[who].nStatus		= status;					// �� ���¿� ���� ���� ( ���� ��� X )
		m_NpcTable[who].race		= c_NpcRace_Mob;			// npc table �� ���� � Ÿ������ ����...
//		m_NpcTable[who].type		= type;						// �� ���� ���� ��� (�鸶, �渶, ���� ��...) ��� X 
//		m_NpcTable[who].nCurrentFloor = 0;
		Rsprintf( RWCHAR(m_NpcTable[who].szName), _RT("%s"), RWCHAR(npc_Info->theName ));
		m_NpcTable[who].theHP		= hp;
		m_NpcTable[who].theSMP		= sp;
		m_NpcTable[who].theMaxHP	= npc_Info->theMaxHP;
		m_NpcTable[who].theRealHP	= hp;
		m_NpcTable[who].theNpcSize	= npc_Info->theSize;
		
		m_Npc[who].m_State.BaseCode = BaseCode;
		//-----------------------------------------------------------------------------
		// NPC ���̶�� race�� ���� ����
		// �����ô� �Ͷ�����.. ���� ���� �˰� �־�� �Ѵ�
		//-----------------------------------------------------------------------------
//		if( FindHorse( who ) == true || STID_INDEX == index )		
//			m_NpcTable[who].race = HORSE_RACE;
				
		//-----------------------------------------------------------------------------
		// ĳ���� ����
		//-----------------------------------------------------------------------------
		m_Npc[who].m_bPC			= false;		
		m_Npc[who].m_nTableIndex	= who;
		m_Npc[who].m_State.bWalk	= true;
		
		if( m_Npc[who].CreateNonPlayer() == false )
		{
			m_NpcTable[who].Init();
			return who;
		}

		m_Npc[who].SetTilePos( (int)(pos%g_Pc.m_cSize), (int)(pos/g_Pc.m_cSize) );	
		m_dwNpcCount++;	

		//-----------------------------------------------------------------------------
		// NPC���̴�? �ؽ��İ˻�	
		//-----------------------------------------------------------------------------
		UpdateNpc( who );
		
		//-----------------------------------------------------------------------------
		// �������϶��� �ϴ� ó��
		//-----------------------------------------------------------------------------
		if( g_RockClient.GetGameMode() == GMODE_NORMAL )
		{
			// ���üũ - �ż��ϰ�쿡�� ���üũ�� �����ʴ´�.
			//g_Map.SetMapData( NPC_BLOCK, who, pos );
			
			// ��ƼŬ �ִ� ���̸� ��ƼŬ ����			
			// g_Particle.Make_Npc_Effect( index, unique );
		}
	}
	//-----------------------------------------------------------------------------
	// �ִ� ���ε� �ٽ� �޾Ҵٸ� ��ġ���� �ٲٱ�
	//-----------------------------------------------------------------------------
	else
	{			
		DebugPrint( "!\r\n SM__HOLY__APPEAR:NPC �� ����" );
	}
	return who;
}


//................................................................................................................
// �ż� ����
//................................................................................................................
void	CField::SM__HOLY__DISAPPEAR( int index, DWORD unique )
{
	int who = FindNpc( unique );
	if( who < MAX_NPC_INTABLE )
	{			
		// �ش� Ÿ���� ����Ʈ����
		g_Particle.StopDisplay( unique, index );
		g_Billboard.DelManager( unique, index );
//		g_ChatList.DelUserBalloon( unique, index );

		// ����Ʈ ����Ʈ ����� By wxywxy
		g_Particle.m_EffectManager.DeleteNow( ( Character * ) &m_Npc[who] );
		g_Particle.DelEffSetCon( m_Npc[who].GetUnique() , m_Npc[who].GetIndex() );
		
//		std::vector<int>::iterator ir = g_Map.m_Npc[who].thePreQuestCodeList.begin();
//		for( ; ir != g_Map.m_Npc[who].thePreQuestCodeList.end() ; ++ir )
//		{
			g_Map.m_Npc[who].thePreQuestCodeList.clear();
			g_Map.m_Npc[who].m_QuestMarkType = NONE_QUEST_MARK;
//		}				

		// �ʺ�����(Ÿ���� �׾��ִ� ���¶�� ������ �ʿ����)
//		if( m_Npc[who].m_curt_event.type != SM_STATE_DIE )
			g_Map.SetMapData( CLEAR_BLOCK, who, m_NpcTable[who].lDestBlock, false );

		// NPC���̺����
		m_NpcTable[who].Init();
		m_Npc[who].Destroy();
		
		m_Npc[who].m_bIsLoaded = false;
		m_Npc[who].m_bIsVisible = false;		
		m_dwNpcCount--;
	}
	else
	{
		DebugPrint( "!\r\n SM__NPC__DISAPPEAR:NPC ��á����" );
	}
}

//................................................................................................................
// �ż� ������
//................................................................................................................
void	CField::SM__HOLY__LEVELUP( DWORD Unique, WORD Level )
{
	int Index = g_Map.FindNpc( Unique );

	if( Index == MAX_NPC_INTABLE )
		return;

	if( g_Map.m_Npc[ Index ].m_State.nHolyState != nHolyState_Holy )
		return;

	//...........................................................................................................
	// ���� ���ҽ� ������ �����	
	//...........................................................................................................
	g_Particle.StopDisplay( Unique , Index );
	g_Billboard.DelManager( Unique , Index );

	// ����Ʈ ����Ʈ ����� By wxywxy
	g_Particle.m_EffectManager.DeleteNow( ( Character * ) &m_Npc[ Index ] );
	g_Particle.DelEffSetCon( m_Npc[ Index ].GetUnique() , m_Npc[ Index ].GetIndex() );
		
	int NextHolyCode = g_Pc_Manager.m_HolyManager.GetHolyCode( g_Map.m_Npc[ Index ].m_State.BaseCode , Level );

	if( NextHolyCode == -1 )
		return;	
	
	SNPCBaseInfo* npc_Info;
	npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( NextHolyCode ); 

	if( NULL == npc_Info )
	{
		RLGS( "Not Found Info In Npc Table [ SM__HOLY__LEVELUP ]");
		return;
	}

	bool bFindNpc = false;
	
	// NPC �� Ÿ�Կ� ���� ���� �����϶�� �ϸ� �ý��� �ٿ� 
	bFindNpc = g_Pc_Manager.IsFindFileData( (DWORD)NextHolyCode );

	if( !bFindNpc )
	{
		if( !g_Pc_Manager.AddNpcCode( NextHolyCode ) )
		{
			return;
		}
	}

	g_Map.m_Npc[ Index ].m_CharAlpha.Clear();
	
	g_Map.m_NpcTable[ Index ].code				= NextHolyCode;					
	g_Map.m_NpcTable[ Index ].index				= npc_Info->theFileCode;
	g_Map.m_NpcTable[ Index ].theLevel		    = Level;
	g_Map.m_Npc[ Index ].m_ani_number		= -1;
	g_Map.m_Npc[ Index ].m_pre_ani_number	= -1;
	g_Map.m_Npc[ Index ].m_ani_time			= 0.3f;
	g_Map.m_Npc[ Index ].m_end_frame		= 0;
	g_Map.m_Npc[ Index ].m_pre_end_frame	= 0;
	g_Map.m_Npc[ Index ].m_ani_frame		= 0;
	g_Map.m_Npc[ Index ].m_ani_index		= 0;

	int	nItmCount;
	int	chrAttrIndex = g_Map.m_Npc[ Index ].GetAttrIndex();
	
	nItmCount = g_Pc_Manager.GetItmCount( &g_Map.m_Npc[ Index ] );
	
	for( int i = 0; i < nItmCount; ++i )
	{
		if( n_Wear_Serve == i )
		{
			g_Map.m_Npc[ Index ].m_pItm_index[i]	= -1;			
		}
		else
		{		
			// ������, ���͸���
			g_Map.m_Npc[ Index ].m_pItm_index[i]	= g_Pc_Manager.GetInitItmIndex( &g_Map.m_Npc[ Index ] , i );
		}
		
		g_Map.m_Npc[ Index ].m_pMaterial[i]	= g_Pc_Manager.GetItmMaterial( &g_Map.m_Npc[ Index ] , i );
		
		g_Map.m_Npc[ Index ].m_pVtxBuffer[i].Destroy();

		g_Map.m_Npc[ Index ].m_pVtxBuffer[i].CreateBuffer( g_Pc_Manager.GetVtxCount( &g_Map.m_Npc[ Index ] , i) );
		
	}

	// ������
	g_Map.m_Npc[ Index ].m_vScale = g_Pc_Manager.GetChrScale( &g_Map.m_Npc[ Index ] );
	
	g_Map.m_Npc[ Index ].AnimationPlay( n_NPC_Stand0 );	
	
	// ����ð�����
	g_Map.m_Npc[ Index ].m_PreTime	= g_nowTime;	
	g_Map.m_Npc[ Index ].m_SimTime	= g_nowTime;
	
	g_Particle.SetEffSetCon( 0.0f , EF_HOLY_LEVEL_UP , 0 , 5.1f , g_Map.m_Npc[ Index ].GetUnique() , g_Map.m_Npc[ Index ].GetIndex() );

	//......................................................................................................
	// NPC ������ ����Ʈ �ް� ������ By wxywxy
	//......................................................................................................
	npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( NextHolyCode );
			
	if( NULL != npc_Info )
	{
		if( npc_Info->theRightEff != -1 )
		{
			g_Particle.m_EffectManager.CreateEffect( ( EFFECT_ID ) npc_Info->theRightEff , ( Character * ) &g_Map.m_Npc[ Index ] , NULL , NULL );
		}
	}
}

//................................................................................................................
// �� ������
//................................................................................................................
void	CField::SM__PET__LEVELUP( DWORD Unique, WORD nNewPetCode, BYTE aLevel )
{
	int Index = g_Map.FindNpc( Unique );

	if( Index == MAX_NPC_INTABLE )
		return;

	if( g_Map.m_Npc[ Index ].m_State.nPetState != nPetState_Pet )
		return;

	SNPCBaseInfo* npc_Info = NULL;

	//...........................................................................................................
	// ���� ���ҽ� ������ �����	
	//...........................................................................................................
	g_Particle.StopDisplay( Unique , Index );
	g_Billboard.DelManager( Unique , Index );

	// ����Ʈ ����Ʈ ����� By wxywxy
	g_Particle.m_EffectManager.DeleteNow( ( Character * ) &m_Npc[ Index ] );
	g_Particle.DelEffSetCon( m_Npc[ Index ].GetUnique() , m_Npc[ Index ].GetIndex() );
		
	//���� Pet�ڵ�� �Ѿ�� �ڵ尪�� ��	
	int nNowPetCode = g_Map.m_NpcTable[Index].code;

	if( nNowPetCode == nNewPetCode )
	{
		g_Map.m_NpcTable[ Index ].theLevel = aLevel;		

		g_Particle.SetEffSetCon( 0.0f , EF_PET_LEVELUP , 0 , 5.1f , g_Map.m_Npc[ Index ].GetUnique() , 
							 g_Map.m_Npc[ Index ].GetIndex() );
		//......................................................................................................
		// NPC ������ ����Ʈ �ް� ������ By wxywxy
		//......................................................................................................
		npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( nNewPetCode ); 
				
		if( NULL != npc_Info )
		{
			if( npc_Info->theRightEff != -1 )
			{
				g_Particle.m_EffectManager.CreateEffect( ( EFFECT_ID ) npc_Info->theRightEff , ( Character * ) &g_Map.m_Npc[ Index ] , NULL , NULL );
			}
		}		
		return;
	}
	
	bool bFindNpc = false;
	
	// NPC �� Ÿ�Կ� ���� ���� �����϶�� �ϸ� �ý��� �ٿ� 
	bFindNpc = g_Pc_Manager.IsFindFileData( (DWORD)nNewPetCode );

	if( !bFindNpc )
	{
		if( !g_Pc_Manager.AddNpcCode( nNewPetCode ) )
		{
			return;
		}
	}		

	npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( nNewPetCode ); 

	if( NULL == npc_Info )
	{
		RLGS( "Not Found Info In Npc Table [ SM__PET__LEVELUP ]");
		return;
	}
	
	///-- ���� �������� �ص� ���� ���� ���´� �״�� �����Ѵ�.
	///--g_Map.m_Npc[ Index ].m_CharAlpha.Clear();	
	
	g_Map.m_NpcTable[ Index ].code				= nNewPetCode;					
	g_Map.m_NpcTable[ Index ].index				= npc_Info->theFileCode;
	g_Map.m_NpcTable[ Index ].theLevel		    = aLevel;

	g_Map.m_Npc[ Index ].m_ani_number		= -1;
	g_Map.m_Npc[ Index ].m_pre_ani_number	= -1;
	g_Map.m_Npc[ Index ].m_ani_time			= 0.3f;
	g_Map.m_Npc[ Index ].m_end_frame		= 0;
	g_Map.m_Npc[ Index ].m_pre_end_frame	= 0;
	g_Map.m_Npc[ Index ].m_ani_frame		= 0;
	g_Map.m_Npc[ Index ].m_ani_index		= 0;

	int	nItmCount;
	int	chrAttrIndex = g_Map.m_Npc[ Index ].GetAttrIndex();
	
	nItmCount = g_Pc_Manager.GetItmCount( &g_Map.m_Npc[ Index ] );
	
	for( int  i = 0; i < nItmCount; ++i )
	{
		if( n_Wear_Serve == i )
		{
			g_Map.m_Npc[ Index ].m_pItm_index[i]	= -1;			
		}
		else
		{		
			// ������, ���͸���
			g_Map.m_Npc[ Index ].m_pItm_index[i]	= g_Pc_Manager.GetInitItmIndex( &g_Map.m_Npc[ Index ] , i );
		}
		
		g_Map.m_Npc[ Index ].m_pMaterial[i]	= g_Pc_Manager.GetItmMaterial( &g_Map.m_Npc[ Index ] , i );
		
		g_Map.m_Npc[ Index ].m_pVtxBuffer[i].Destroy();

		g_Map.m_Npc[ Index ].m_pVtxBuffer[i].CreateBuffer( g_Pc_Manager.GetVtxCount( &g_Map.m_Npc[ Index ] , i) );
		
	}

	// ������
	g_Map.m_Npc[ Index ].m_vScale = g_Pc_Manager.GetChrScale( &g_Map.m_Npc[ Index ] );
	
	g_Map.m_Npc[ Index ].AnimationPlay( n_NPC_Stand0 );	
	
	// ����ð�����
	g_Map.m_Npc[ Index ].m_PreTime	= g_nowTime;	
	g_Map.m_Npc[ Index ].m_SimTime	= g_nowTime;
	
	int nCode =  g_Pc_Manager.m_HolyManager.GetPetLevelUpEffect( nNewPetCode , aLevel );

	g_Particle.SetEffSetCon( 0.0f , nCode , 0 , 5.1f , g_Map.m_Npc[ Index ].GetUnique() , 
							 g_Map.m_Npc[ Index ].GetIndex() );	

	//......................................................................................................
	// NPC ������ ����Ʈ �ް� ������ By wxywxy
	//......................................................................................................
	npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( nNewPetCode ); 
			
	if( NULL != npc_Info )
	{
		if( npc_Info->theRightEff != -1 )
		{
			g_Particle.m_EffectManager.CreateEffect( ( EFFECT_ID ) npc_Info->theRightEff , ( Character * ) &g_Map.m_Npc[ Index ] , NULL , NULL );
		}
	}
}

void	CField::SM__NPC__LEVELUP( DWORD aNPCID, DWORD aNPCCode )
{
	int Index = g_Map.FindNpc( aNPCID );

	if( Index == MAX_NPC_INTABLE )
		return;	

	SNPCBaseInfo* npc_Info = NULL;

	//...........................................................................................................
	// ���� ���ҽ� ������ �����	
	//...........................................................................................................
	g_Particle.StopDisplay( aNPCID , Index );
	g_Billboard.DelManager( aNPCID , Index );

	// ����Ʈ ����Ʈ ����� By wxywxy
	g_Particle.m_EffectManager.DeleteNow( ( Character * ) &m_Npc[ Index ] );
	g_Particle.DelEffSetCon( m_Npc[ Index ].GetUnique() , m_Npc[ Index ].GetIndex() );
		
	//���� NPC�ڵ�� �Ѿ�� �ڵ尪�� ��	
	int nNowNpcCode = g_Map.m_NpcTable[Index].code;

	if( nNowNpcCode == aNPCCode )
	{
		return;
	}
	
	bool bFindNpc = false;
	
	// NPC �� Ÿ�Կ� ���� ���� �����϶�� �ϸ� �ý��� �ٿ� 
	bFindNpc = g_Pc_Manager.IsFindFileData( (DWORD)aNPCCode );

	if( !bFindNpc )
	{
		if( !g_Pc_Manager.AddNpcCode( aNPCCode ) )
		{
			return;
		}
	}		

	npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( aNPCCode );
	if( NULL == npc_Info )
	{
		RLGS( "Not Found Info In Npc Table [ SM__NPC__LEVELUP ]");
		return;
	}

	g_Map.m_Npc[ Index ].m_CharAlpha.Clear();
	
	g_Map.m_NpcTable[ Index ].code				= aNPCCode;					
	g_Map.m_NpcTable[ Index ].index				= npc_Info->theFileCode;
	g_Map.m_NpcTable[ Index ].theLevel		    = npc_Info->theLevel;
	Rsprintf( RWCHAR(m_NpcTable[ Index ].szName) , _RT("%s") , RWCHAR( npc_Info->theName) );
	g_Map.m_NpcTable[ Index ].theMaxHP			= npc_Info->theMaxHP;
	g_Map.m_NpcTable[ Index ].theNpcSize		= npc_Info->theSize;

	g_Map.m_Npc[ Index ].m_ani_number		= -1;
	g_Map.m_Npc[ Index ].m_pre_ani_number	= -1;
	g_Map.m_Npc[ Index ].m_ani_time			= 0.3f;
	g_Map.m_Npc[ Index ].m_end_frame		= 0;
	g_Map.m_Npc[ Index ].m_pre_end_frame	= 0;
	g_Map.m_Npc[ Index ].m_ani_frame		= 0;
	g_Map.m_Npc[ Index ].m_ani_index		= 0;

	int	nItmCount;
	int	chrAttrIndex = g_Map.m_Npc[ Index ].GetAttrIndex();
	
	nItmCount = g_Pc_Manager.GetItmCount( &g_Map.m_Npc[ Index ] );
	
	for( int  i = 0; i < nItmCount; ++i )
	{
		if( n_Wear_Serve == i )
		{
			g_Map.m_Npc[ Index ].m_pItm_index[i]	= -1;			
		}
		else
		{		
			// ������, ���͸���
			g_Map.m_Npc[ Index ].m_pItm_index[i]	= g_Pc_Manager.GetInitItmIndex( &g_Map.m_Npc[ Index ] , i );
		}
		
		g_Map.m_Npc[ Index ].m_pMaterial[i]	= g_Pc_Manager.GetItmMaterial( &g_Map.m_Npc[ Index ] , i );
		
		g_Map.m_Npc[ Index ].m_pVtxBuffer[i].Destroy();

		g_Map.m_Npc[ Index ].m_pVtxBuffer[i].CreateBuffer( g_Pc_Manager.GetVtxCount( &g_Map.m_Npc[ Index ] , i) );
		
	}

	// ������
	g_Map.m_Npc[ Index ].m_vScale = g_Pc_Manager.GetChrScale( &g_Map.m_Npc[ Index ] );
	
	g_Map.m_Npc[ Index ].AnimationPlay( n_NPC_Stand0 );	
	
	// ����ð�����
	g_Map.m_Npc[ Index ].m_PreTime	= g_nowTime;	
	g_Map.m_Npc[ Index ].m_SimTime	= g_nowTime;

	g_Particle.SetEffSetCon( 0.0f , EF_PET_LEVELUP , 0 , 5.1f , g_Map.m_Npc[ Index ].GetUnique() , 
							 g_Map.m_Npc[ Index ].GetIndex() );	

	//......................................................................................................
	// NPC ������ ����Ʈ �ް� ������ By wxywxy
	//......................................................................................................
	npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( aNPCCode ); 
			
	if( NULL != npc_Info )
	{
		if( npc_Info->theRightEff != -1 )
		{
			g_Particle.m_EffectManager.CreateEffect( ( EFFECT_ID ) npc_Info->theRightEff , ( Character * ) &g_Map.m_Npc[ Index ] , NULL , NULL );
		}
	}

}

void	CField::SM__NPC__CHANGE( DWORD aNPCID, DWORD aNPCCode )
{
	int Index = g_Map.FindNpc( aNPCID );

	if( Index == MAX_NPC_INTABLE )
		return;	

	SNPCBaseInfo* npc_Info = NULL;

	//...........................................................................................................
	// ���� ���ҽ� ������ �����	
	//...........................................................................................................
	g_Particle.StopDisplay( aNPCID , Index );
	g_Billboard.DelManager( aNPCID , Index );

	// ����Ʈ ����Ʈ ����� By wxywxy
	g_Particle.m_EffectManager.DeleteNow( ( Character * ) &m_Npc[ Index ] );
	g_Particle.DelEffSetCon( m_Npc[ Index ].GetUnique() , m_Npc[ Index ].GetIndex() );
		
	//���� NPC�ڵ�� �Ѿ�� �ڵ尪�� ��	
	int nNowNpcCode = g_Map.m_NpcTable[Index].code;

	if( nNowNpcCode == aNPCCode )
	{
		return;
	}
	
	bool bFindNpc = false;
	
	// NPC �� Ÿ�Կ� ���� ���� �����϶�� �ϸ� �ý��� �ٿ� 
	bFindNpc = g_Pc_Manager.IsFindFileData( (DWORD)aNPCCode );

	if( !bFindNpc )
	{
		if( !g_Pc_Manager.AddNpcCode( aNPCCode ) )
		{
			return;
		}
	}		

	npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( aNPCCode );
	if( NULL == npc_Info )
	{
		RLGS( "Not Found Info In Npc Table [ SM__NPC__LEVELUP ]");
		return;
	}

	g_Map.m_Npc[ Index ].m_CharAlpha.Clear();
	
	g_Map.m_NpcTable[ Index ].code				= aNPCCode;					
	g_Map.m_NpcTable[ Index ].index				= npc_Info->theFileCode;
	g_Map.m_NpcTable[ Index ].theLevel		    = npc_Info->theLevel;
	Rsprintf( RWCHAR(m_NpcTable[ Index ].szName) , _RT("%s") , RWCHAR( npc_Info->theName) );
	g_Map.m_NpcTable[ Index ].theMaxHP			= npc_Info->theMaxHP;
	g_Map.m_NpcTable[ Index ].theNpcSize		= npc_Info->theSize;

	g_Map.m_Npc[ Index ].m_ani_number		= -1;
	g_Map.m_Npc[ Index ].m_pre_ani_number	= -1;
	g_Map.m_Npc[ Index ].m_ani_time			= 0.3f;
	g_Map.m_Npc[ Index ].m_end_frame		= 0;
	g_Map.m_Npc[ Index ].m_pre_end_frame	= 0;
	g_Map.m_Npc[ Index ].m_ani_frame		= 0;
	g_Map.m_Npc[ Index ].m_ani_index		= 0;

	int	nItmCount;
	int	chrAttrIndex = g_Map.m_Npc[ Index ].GetAttrIndex();
	
	nItmCount = g_Pc_Manager.GetItmCount( &g_Map.m_Npc[ Index ] );
	
	for( int  i = 0; i < nItmCount; ++i )
	{
		if( n_Wear_Serve == i )
		{
			g_Map.m_Npc[ Index ].m_pItm_index[i]	= -1;			
		}
		else
		{		
			// ������, ���͸���
			g_Map.m_Npc[ Index ].m_pItm_index[i]	= g_Pc_Manager.GetInitItmIndex( &g_Map.m_Npc[ Index ] , i );
		}
		
		g_Map.m_Npc[ Index ].m_pMaterial[i]	= g_Pc_Manager.GetItmMaterial( &g_Map.m_Npc[ Index ] , i );
		
		g_Map.m_Npc[ Index ].m_pVtxBuffer[i].Destroy();

		g_Map.m_Npc[ Index ].m_pVtxBuffer[i].CreateBuffer( g_Pc_Manager.GetVtxCount( &g_Map.m_Npc[ Index ] , i) );
		
	}

	// ������
	g_Map.m_Npc[ Index ].m_vScale = g_Pc_Manager.GetChrScale( &g_Map.m_Npc[ Index ] );
	
	g_Map.m_Npc[ Index ].AnimationPlay( n_NPC_Stand0 );	
	
	// ����ð�����
	g_Map.m_Npc[ Index ].m_PreTime	= g_nowTime;	
	g_Map.m_Npc[ Index ].m_SimTime	= g_nowTime;

//	g_Particle.SetEffSetCon( 0.0f , EF_PET_LEVELUP , 0 , 5.1f , g_Map.m_Npc[ Index ].GetUnique() , 
//							 g_Map.m_Npc[ Index ].GetIndex() );	

	//......................................................................................................
	// NPC ������ ����Ʈ �ް� ������ By wxywxy
	//......................................................................................................
	npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( aNPCCode ); 
			
	if( NULL != npc_Info )
	{
		if( npc_Info->theRightEff != -1 )
		{
			g_Particle.m_EffectManager.CreateEffect( ( EFFECT_ID ) npc_Info->theRightEff , ( Character * ) &g_Map.m_Npc[ Index ] , NULL , NULL );
		}
	}

}

