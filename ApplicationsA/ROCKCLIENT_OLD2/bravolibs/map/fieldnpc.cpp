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

			// 천년의 나무 넘 커서 컬링이 안되네...ㅜ  ㅜ 흑 .
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
// Desc: NPC 애니메이션처리 & 버퍼처리
//-----------------------------------------------------------------------------
void		CField::AniProcessNpc()
{
	//-----------------------------------------------------------------------------
	// 캐릭터 디스플레이
	//-----------------------------------------------------------------------------
	for( DWORD i = 0; i < MAX_NPC_INTABLE; i ++ )
	{
		if( m_Npc[i].m_bIsLoaded == true )		
		{			
			m_Npc[i].UpdateHPData();
			// 데이타처리			
			m_Npc[i].QueueProcess();
						
			// 애니메이션처리
			int nRes = m_Npc[i].AniProcess();
//			switch( nRes )
//			{
//			case SM_NPC_DISAPPEAR:	// 시야범위
//				SM__NPC__DISAPPEAR( m_NpcTable[i].index, m_NpcTable[i].lUnique );
//				break;
//			case SM_STATE_DIE:		// 사망
//				SM__NPC__DISAPPEAR( m_NpcTable[i].index, m_NpcTable[i].lUnique );
//				break;
//			}				
		}
	}
}


//-----------------------------------------------------------------------------
// Desc: NPC 렌더링
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
// Desc: NPC 렌더링
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
// Desc: NPC 그림자 
//-----------------------------------------------------------------------------
// Desc: 그림자 생성
void	CField::ShadowMakeNpc()
{	
	// 그림자 디스플레이
	for( DWORD i = 0; i < MAX_NPC_INTABLE; i ++ )
	{
		if( m_Npc[i].m_bIsVisible == true && m_Npc[i].m_bIsLoaded == true )		
			m_Npc[i].ShadowMake();
	}
}

// Desc: 그림자 그리기
void	CField::ShadowDisplayNpc()
{	
	// 그림자 디스플레이
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
// Desc: NPC 삭제
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
		// 자기가 타고 있는 말은 삭제하지 않는다
//		if( g_Pc.GetPlayer()->m_State.horse_index == i )
//		{
//			OutputDebugString( "\n내말은 안되!" );
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
//-- Monster Instance 의 Pointer 를 얻는다.
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
			
			// 살아있다면 정보저장			
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
	// 죽어있다면?
	//-----------------------------------------------------------------------------
	if( m_Npc[index].m_curt_event.type == SM_STATE_DIE )
		return false;

	//-----------------------------------------------------------------------------
	// 공격가능한 NPC?
	//-----------------------------------------------------------------------------
	// 라이브 아이템
	bool bFind = false;
//	if( m_NpcTable[index].race == LIVEITEM_RACE )
//		bFind = true;
	// 일반 NPC
//	else if( g_Pc_Manager.m_NpcAttr[m_NpcTable[index].index].attack == 1 )
		bFind = true;

	// 공격가능해
	if( bFind == true )
	{
		// 지금 선택한 NPC를 다시 선택했다면 false
		if( g_Pc.GetPlayer()->m_curt_event.type		== SM_ATTACK_NPC &&
			g_Pc.GetPlayer()->m_curt_event.index	== index && 
			g_Pc.GetPlayer()->m_curt_event.unique	== m_NpcTable[index].lUnique )
		{
			return false;
		}
		return true;
	}	
	// 이벤트 있는 NPC라면(상인들)
//	else if( g_Pc_Manager.m_NpcAttr[m_NpcTable[index].index].attack == 3 || 
//		 	 g_Pc_Manager.m_NpcAttr[m_NpcTable[index].index].attack == 4 || 
//			 g_Pc_Manager.m_NpcAttr[m_NpcTable[index].index].attack == 5 || 
//			 g_Pc_Manager.m_NpcAttr[m_NpcTable[index].index].attack == 6 )
//	{
		// 상인인터페이스 출력
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
// Desc: NPC 생성
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
			RLG2( "!현재맵에 Code:%d NPC가 npc_%d.set 파일에 없음!!!", index, g_Map.m_NowMapID );		
#endif 
			return -1;
		}
	}

	//-----------------------------------------------------------------------------
	// NPC테이블 비어있는지 찾기
	//-----------------------------------------------------------------------------
	int who = FindNpc( unique );
		
	//-----------------------------------------------------------------------------
	// 없는 NPC이기때문에 로딩해야함
	//-----------------------------------------------------------------------------
	if( who == MAX_NPC_INTABLE )
	{		
		who = GetEmptyTable();

		if( who == MAX_NPC_INTABLE )
		{
			DebugPrint( "SM__NPC__APPEAR 초과!!!\n" );
			return -1;
		}

		SNPCBaseInfo* npc_Info;
		npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( index ); 
		
		//-----------------------------------------------------------------------------
		// 테이블에 저장 
		//-----------------------------------------------------------------------------
		m_NpcTable[who].code		= index;					
		m_NpcTable[who].index		= npc_Info->theFileCode;	// m_pNpcFileInfo[index] 만들어져 있는 몹의 정보를 리턴 (ChrIndex)
		m_NpcTable[who].theLevel	= npc_Info->theLevel;		// 몬스터 레벨 표시 
		m_NpcTable[who].lUnique		= unique;
		m_NpcTable[who].lBlock		= pos;			
		m_NpcTable[who].lDestBlock	= pos;		
		m_NpcTable[who].nStatus		= status;					// 몹 상태에 따라 적용 ( 아직 사용 X )
		m_NpcTable[who].race		= c_NpcRace_Mob;			// npc table 에 따라서 어떤 타입인지 저장...
//		m_NpcTable[who].type		= type;						// 말 색깔에 따라 사용 (백마, 흑마, 적마 등...) 사용 X 
//		m_NpcTable[who].nCurrentFloor = 0;
		Rsprintf( RWCHAR(m_NpcTable[who].szName), _RT("%s"), RWCHAR(npc_Info->theName)  );
		m_NpcTable[who].theHP		= hp;
		m_NpcTable[who].theSMP		= sp;
		m_NpcTable[who].theMaxHP	= npc_Info->theMaxHP;
		m_NpcTable[who].theRealHP	= hp;
		m_NpcTable[who].theNpcSize	= npc_Info->theSize;

		//-----------------------------------------------------------------------------
		// NPC 말이라면 race를 말로 저장
		// 말꼬시는 것때문에.. 말은 따로 알고 있어야 한다
		//-----------------------------------------------------------------------------
//		if( FindHorse( who ) == true || STID_INDEX == index )		
//			m_NpcTable[who].race = HORSE_RACE;
				
		//-----------------------------------------------------------------------------
		// 캐릭터 생성
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
		// NPC 각도 ( 서버에서 주는 값으로 처리 By wxywxy )
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
// 퀘스트 관련
//.............................................................................................................
		//by simwoosung
		if(::IsNonPlayerCode(index))
		{
			UpdateNPCQuestMark(index, who);
		}
//.............................................................................................................
		
		//-----------------------------------------------------------------------------
		// NPC보이니? 텍스쳐검사	
		//-----------------------------------------------------------------------------
		UpdateNpc( who );
		
		//-----------------------------------------------------------------------------
		// 게임중일때만 하는 처리
		//-----------------------------------------------------------------------------
		if( g_RockClient.GetGameMode() == GMODE_NORMAL )
		{
			// 블락체크
			g_Map.SetMapData( NPC_BLOCK, who, pos );
			
			// 파티클 있는 놈이면 파티클 생성			
			// g_Particle.Make_Npc_Effect( index, unique );
		}
	}
	//-----------------------------------------------------------------------------
	// 있는 넘인데 다시 받았다면 위치값만 바꾸기
	//-----------------------------------------------------------------------------
	else
	{			
		DebugPrint( "!\r\n SM__NPC__APPEAR:NPC 또 받음" );
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
	// NPC테이블 비어있는지 찾기
	//-----------------------------------------------------------------------------
	int who = FindNpc( unique );
		
	//-----------------------------------------------------------------------------
	// 없는 NPC이기때문에 로딩해야함
	//-----------------------------------------------------------------------------
	if( who == MAX_NPC_INTABLE )
	{		
		who = GetEmptyTable();

		if( who == MAX_NPC_INTABLE )
		{
			DebugPrint( "SM__PLAYER_NPC__APPEAR 초과!!!\n" );
			return -1;
		}

		SNPCBaseInfo* npc_Info;
		npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( index );
		
		//-----------------------------------------------------------------------------
		// 테이블에 저장 
		//-----------------------------------------------------------------------------
		m_NpcTable[who].code		= index;		
		if( PetLevel )
		{
			m_NpcTable[who].theLevel	= PetLevel;
		}
		else
		{
			m_NpcTable[who].theLevel	= npc_Info->theLevel;		// 몬스터 레벨 표시 
		}
		m_NpcTable[who].lUnique		= unique;
		m_NpcTable[who].lBlock		= pos;			
		m_NpcTable[who].lDestBlock	= pos;		
		m_NpcTable[who].nStatus		= status;					// 몹 상태에 따라 적용 ( 아직 사용 X )
		m_NpcTable[who].race		= c_NpcRace_Mob;			// npc table 에 따라서 어떤 타입인지 저장...
		Rsprintf( RWCHAR(m_NpcTable[who].szName) , _RT("%s"), RWCHAR(npc_Info->theName) );
		m_NpcTable[who].theHP		= hp;
		m_NpcTable[who].theSMP		= sp;
		m_NpcTable[who].theMaxHP	= npc_Info->theMaxHP;
		m_NpcTable[who].theRealHP	= hp;
		m_NpcTable[who].theNpcSize	= npc_Info->theSize;

		///-- 이쪽에서 외부 테이블을 불러오고 해당 데이터로 각필드를 채운다.		
		///-- 인덱스 값은 국가와 성별로 다시정의해 받게한다.		
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
		// 캐릭터 생성
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
		// NPC 각도 ( 서버에서 주는 값으로 처리 By wxywxy )
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
		// NPC보이니? 텍스쳐검사	
		//-----------------------------------------------------------------------------
		UpdateNpc( who );
		
		if( g_RockClient.GetGameMode() == GMODE_NORMAL )
		{
			// 블락체크
			g_Map.SetMapData( NPC_BLOCK, who, pos );
		}
		
	}
	//-----------------------------------------------------------------------------
	// 있는 넘인데 다시 받았다면 위치값만 바꾸기
	//-----------------------------------------------------------------------------
	else
	{			
		DebugPrint( "!\r\n SM__PLAYER_NPC__APPEAR:NPC 또 받음" );
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
			RLG2( "!현재맵에 Code:%d NPC가 npc_%d.set 파일에 없음!!!", index, g_Map.m_NowMapID );			
#endif 
			return -1;
		}
	}

	//-----------------------------------------------------------------------------
	// NPC테이블 비어있는지 찾기
	//-----------------------------------------------------------------------------
	int who = FindNpc( unique, c_NpcRace_Transpotation );
		
	//-----------------------------------------------------------------------------
	// 없는 NPC이기때문에 로딩해야함
	//-----------------------------------------------------------------------------
	if( who == MAX_NPC_INTABLE )
	{		
		who = GetEmptyTable();

		if( who == MAX_NPC_INTABLE )
		{
			DebugPrint( "SM__RIDE__APPEAR 초과!!!\n" );
			return -1;
		}

		SNPCBaseInfo* npc_Info;
		npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( index ); 
		
		//-----------------------------------------------------------------------------
		// 테이블에 저장 
		//-----------------------------------------------------------------------------
		m_NpcTable[who].code		= index;					
		m_NpcTable[who].index		= npc_Info->theFileCode;	// m_pNpcFileInfo[index] 만들어져 있는 몹의 정보를 리턴 (ChrIndex)
		m_NpcTable[who].theLevel	= npc_Info->theLevel;		// 몬스터 레벨 표시 
		m_NpcTable[who].lUnique		= unique;
		m_NpcTable[who].lBlock		= pos;			
		m_NpcTable[who].lDestBlock	= pos;		
		m_NpcTable[who].nStatus		= status;					// 몹 상태에 따라 적용 ( 아직 사용 X )
		m_NpcTable[who].race		= c_NpcRace_Transpotation;	// npc table 에 따라서 어떤 타입인지 저장...
//		m_NpcTable[who].type		= type;						// 말 색깔에 따라 사용 (백마, 흑마, 적마 등...) 사용 X 
//		m_NpcTable[who].nCurrentFloor = 0;
		Rsprintf( RWCHAR(m_NpcTable[who].szName) , _RT("%s"), RWCHAR(npc_Info->theName) );
		m_NpcTable[who].theHP		= hp;
		m_NpcTable[who].theSMP		= sp;
		m_NpcTable[who].theMaxHP	= npc_Info->theMaxHP;
		m_NpcTable[who].theRealHP	= hp;
		m_NpcTable[who].theNpcSize	= npc_Info->theSize;

		//-----------------------------------------------------------------------------
		// NPC 말이라면 race를 말로 저장
		// 말꼬시는 것때문에.. 말은 따로 알고 있어야 한다
		//-----------------------------------------------------------------------------
//		if( FindHorse( who ) == true || STID_INDEX == index )		
//			m_NpcTable[who].race = HORSE_RACE;
				
		//-----------------------------------------------------------------------------
		// 캐릭터 생성
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
		// NPC 각도 ( 서버에서 주는 값으로 처리 By wxywxy )
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
		// 임시 테스트
		g_Particle.SetEffSetCon( 0.0f , 3987			, 0 , 999.0f , unique , m_Npc[who].GetIndex() );
		g_Particle.SetEffSetCon( 0.0f , EF_RAIN_COMBOEX , 0 , 999.0f , unique , m_Npc[who].GetIndex() );
		//
*/
		//-----------------------------------------------------------------------------
		// NPC보이니? 텍스쳐검사	
		//-----------------------------------------------------------------------------
		UpdateNpc( who );		
		
	}
	//-----------------------------------------------------------------------------
	// 있는 넘인데 다시 받았다면 위치값만 바꾸기
	//-----------------------------------------------------------------------------
	else
	{			
		DebugPrint( "!\r\n SM__RIDE__APPEAR:NPC 또 받음" );
	}
	return who;
}

void CField::SM__RIDE__DISAPPEAR( int index, DWORD unique )
{
	int who = FindNpc( unique, c_NpcRace_Transpotation );
	if( who < MAX_NPC_INTABLE )
	{			
		// 해당 타겟의 이펙트삭제
		g_Particle.StopDisplay( unique, index );
		g_Billboard.DelManager( unique, index );
//		g_ChatList.DelUserBalloon( unique, index );

		// 이펙트 리스트 지우기 By wxywxy
		g_Particle.m_EffectManager.DeleteNow( ( Character * ) &m_Npc[who] );
		g_Particle.DelEffSetCon( m_Npc[who].GetUnique() , m_Npc[who].GetIndex() );
		
//		std::vector<int>::iterator ir = g_Map.m_Npc[who].thePreQuestCodeList.begin();
//		for( ; ir != g_Map.m_Npc[who].thePreQuestCodeList.end() ; ++ir )
//		{
			g_Map.m_Npc[who].thePreQuestCodeList.clear();
			g_Map.m_Npc[who].m_QuestMarkType = NONE_QUEST_MARK;
//		}				

		// 맵블럭삭제(타겟이 죽어있는 상태라면 삭제할 필요없다)
//		if( m_Npc[who].m_curt_event.type != SM_STATE_DIE )
			//g_Map.SetMapData( CLEAR_BLOCK, who, m_NpcTable[who].lDestBlock, false );

		// NPC테이블삭제
		m_NpcTable[who].Init();
		m_Npc[who].Destroy();
		
		m_Npc[who].m_bIsLoaded = false;
		m_Npc[who].m_bIsVisible = false;		
		m_dwNpcCount--;
	}
	else
	{
		DebugPrint( "!\r\n SM__RIDE__DISAPPEAR:NPC 못찼았음" );
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
			RLG2( "!현재맵에 Code:%d NPC가 npc_%d.set 파일에 없음!!!", index, g_Map.m_NowMapID );	
#endif 
			return -1;
		}
	}

	//-----------------------------------------------------------------------------
	// NPC테이블 비어있는지 찾기
	//-----------------------------------------------------------------------------
	int who = FindNpc( unique );
		
	//-----------------------------------------------------------------------------
	// 없는 NPC이기때문에 로딩해야함
	//-----------------------------------------------------------------------------
	if( who == MAX_NPC_INTABLE )
	{		
		who = GetEmptyTable();

		if( who == MAX_NPC_INTABLE )
		{
			DebugPrint( "SM__PET__APPEAR 초과!!!\n" );
			return -1;
		}

		SNPCBaseInfo* npc_Info;
		npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( index ); 
		
		//-----------------------------------------------------------------------------
		// 테이블에 저장 
		//-----------------------------------------------------------------------------
		m_NpcTable[who].code		= index;					
		m_NpcTable[who].index		= npc_Info->theFileCode;	// m_pNpcFileInfo[index] 만들어져 있는 몹의 정보를 리턴 (ChrIndex)
		m_NpcTable[who].theLevel	= PetLevel;		// 몬스터 레벨 표시 
		m_NpcTable[who].lUnique		= unique;
		m_NpcTable[who].lBlock		= pos;			
		m_NpcTable[who].lDestBlock	= pos;		
		m_NpcTable[who].nStatus		= status;					// 몹 상태에 따라 적용 ( 아직 사용 X )
		m_NpcTable[who].race		= c_NpcRace_Mob;			// npc table 에 따라서 어떤 타입인지 저장...
//		m_NpcTable[who].type		= type;						// 말 색깔에 따라 사용 (백마, 흑마, 적마 등...) 사용 X 
//		m_NpcTable[who].nCurrentFloor = 0;
		Rsprintf( RWCHAR(m_NpcTable[who].szName), _RT("%s") , RWCHAR(npc_Info->theName) );
		m_NpcTable[who].theHP		= hp;
		m_NpcTable[who].theSMP		= sp;
		m_NpcTable[who].theMaxHP	= npc_Info->theMaxHP;
		m_NpcTable[who].theRealHP	= hp;
		m_NpcTable[who].theNpcSize	= npc_Info->theSize;
		
		//-----------------------------------------------------------------------------
		// NPC 말이라면 race를 말로 저장
		// 말꼬시는 것때문에.. 말은 따로 알고 있어야 한다
		//-----------------------------------------------------------------------------
//		if( FindHorse( who ) == true || STID_INDEX == index )		
//			m_NpcTable[who].race = HORSE_RACE;
				
		//-----------------------------------------------------------------------------
		// 캐릭터 생성
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
		// NPC 각도 ( 서버에서 주는 값으로 처리 By wxywxy )
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
		// 임시 테스트
		g_Particle.SetEffSetCon( 0.0f , 3987			, 0 , 999.0f , unique , m_Npc[who].GetIndex() );
		g_Particle.SetEffSetCon( 0.0f , EF_RAIN_COMBOEX , 0 , 999.0f , unique , m_Npc[who].GetIndex() );
		//
*/
		//-----------------------------------------------------------------------------
		// NPC보이니? 텍스쳐검사	
		//-----------------------------------------------------------------------------
		UpdateNpc( who );
		
		if( g_RockClient.GetGameMode() == GMODE_NORMAL )
		{
			// 블락체크
			g_Map.SetMapData( NPC_BLOCK, who, pos );
			
			// 파티클 있는 놈이면 파티클 생성			
			// g_Particle.Make_Npc_Effect( index, unique );
		}
		
	}
	//-----------------------------------------------------------------------------
	// 있는 넘인데 다시 받았다면 위치값만 바꾸기
	//-----------------------------------------------------------------------------
	else
	{			
		DebugPrint( "!\r\n SM__RIDE__APPEAR:NPC 또 받음" );
	}
	return who;
}


//-----------------------------------------------------------------------------
// Desc: NPC 삭제
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
		
		///-- 큐에 남아있던 공격적용루틴을 일괄처리한다.
		m_Npc[who].AllActQueue();		
		
		// 해당 타겟의 이펙트삭제
		g_Particle.StopDisplay( unique, index );
		g_Billboard.DelManager( unique, index );
//		g_ChatList.DelUserBalloon( unique, index );

		// 이펙트 리스트 지우기 By wxywxy
		g_Particle.m_EffectManager.DeleteNow( ( Character * ) &m_Npc[who] );
		g_Particle.DelEffSetCon( m_Npc[who].GetUnique() , m_Npc[who].GetIndex() );
		
//		std::vector<int>::iterator ir = g_Map.m_Npc[who].thePreQuestCodeList.begin();
//		for( ; ir != g_Map.m_Npc[who].thePreQuestCodeList.end() ; ++ir )
//		{
			g_Map.m_Npc[who].thePreQuestCodeList.clear();
			g_Map.m_Npc[who].m_QuestMarkType = NONE_QUEST_MARK;
//		}				

		// 맵블럭삭제(타겟이 죽어있는 상태라면 삭제할 필요없다)
//		if( m_Npc[who].m_curt_event.type != SM_STATE_DIE )
			g_Map.SetMapData( CLEAR_BLOCK, who, m_NpcTable[who].lDestBlock, false );

		// NPC테이블삭제
		m_NpcTable[who].Init();
		m_Npc[who].Destroy();
		
		m_Npc[who].m_bIsLoaded = false;
		m_Npc[who].m_bIsVisible = false;		
		m_dwNpcCount--;
	}
	else
	{
		DebugPrint( "!\r\n SM__NPC__DISAPPEAR:NPC 못찼았음" );
	}
}


//-----------------------------------------------------------------------------
// Desc: NPC 생성
//-----------------------------------------------------------------------------
int CField::SM__ITEM__APPEAR( DWORD aUnique, DWORD aCell, WORD aItemCode )
{		
	//-----------------------------------------------------------------------------
	// NPC테이블 비어있는지 찾기
	//-----------------------------------------------------------------------------
	int item_table = FindNpc( aUnique, c_NpcRace_Item );
		
	//-----------------------------------------------------------------------------
	// 없는 NPC이기때문에 로딩해야함
	//-----------------------------------------------------------------------------
	if( item_table == MAX_NPC_INTABLE )
	{		
		item_table = GetEmptyTable();
		
		if( item_table == MAX_NPC_INTABLE )
		{
			DebugPrint( "SM__ITEM__APPEAR 초과!!!\n" );
			return -1;
		}

		SItemBaseInfo* item_Info = NULL;
		item_Info = g_Pc_Manager.GetItemBaseInfoPtr( aItemCode ); 
		if( item_Info == NULL )
			return -1;

		int NpcChr_Index = g_Pc_Manager.the_sNpc_SetData.total_npc_set_num - c_MaxDropItemIndex;

		//-----------------------------------------------------------------------------
		// 테이블에 저장 
		//-----------------------------------------------------------------------------
		m_NpcTable[item_table].code			= 1; // aItemCode;	// 아이템을 NPC 코드에 추가할때까지 임시로 씀...ㅡㅡ;		
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
			// 엘드 따로 처리 
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
			m_NpcTable[item_table].index = NpcChr_Index;	// 맨 마지막 보따리 아이템 인덱스 저장 
		}
		
		m_NpcTable[item_table].lUnique		= aUnique;
		m_NpcTable[item_table].lBlock		= aCell;			
		m_NpcTable[item_table].lDestBlock	= aCell;		
		m_NpcTable[item_table].race			= c_NpcRace_Item;
		
		Rsprintf( RWCHAR(m_NpcTable[item_table].szName) , _RT("%s"), RWCHAR(item_Info->theName) );
		
		//-----------------------------------------------------------------------------
		// 캐릭터 생성
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
		// NPC보이니? 텍스쳐검사	
		//-----------------------------------------------------------------------------
		UpdateNpc( item_table );
		
		//-----------------------------------------------------------------------------
		// 게임중일때만 하는 처리
		//-----------------------------------------------------------------------------
//		if( g_RockClient.GetGameMode() == GMODE_NORMAL )
//		{
			// 블럭체크
//			g_Map.SetMapData( NPC_BLOCK, item_table, aCell );
//		}
		
		// 별다른 것 없이 애니메이션 처리만 해주면 된다 
		EVENT_DATA_INFO drop_event;
		drop_event.type = SM_ITEM_DROPING;
		m_Npc[item_table].PushQueue( drop_event, TRUE );
	}
	//-----------------------------------------------------------------------------
	// 있는 넘인데 다시 받았다면 위치값만 바꾸기
	//-----------------------------------------------------------------------------
	else
	{			
		DebugPrint( "!SM__ITEM__APPEAR : 또 생성하라고??!!!\n" );
	}

	return item_table;
}


//-----------------------------------------------------------------------------
// Desc: NPC 삭제
//-----------------------------------------------------------------------------
void CField::SM__ITEM__DISAPPEAR( DWORD aUnique )
{
	int item_table = FindNpc( aUnique, c_NpcRace_Item );

	if( item_table < MAX_NPC_INTABLE )
	{			
		// 해당 타겟의 이펙트삭제
		g_Particle.StopDisplay( aUnique, c_NpcIndex_Item );
		g_Billboard.DelManager( aUnique, c_NpcIndex_Item );
//		g_ChatList.DelUserBalloon( unique, index );

		// 이펙트 리스트 지우기 By wxywxy
		g_Particle.m_EffectManager.Delete( ( Character * ) &m_Npc[item_table] );
		
		// 맵블럭삭제(타겟이 죽어있는 상태라면 삭제할 필요없다)
//		if( m_Npc[item_table].m_curt_event.type != SM_STATE_DIE )
//			g_Map.SetMapData( CLEAR_BLOCK, item_table, m_NpcTable[item_table].lDestBlock );

		// NPC테이블삭제
		m_NpcTable[item_table].Init();
		m_Npc[item_table].Destroy();
		m_dwNpcCount--;
	}
	else
	{
		DebugPrint( "!\r\n SM__NPC__DISAPPEAR:NPC 못찼았음" );
	}
}


//-----------------------------------------------------------------------------
// Desc: NPC 밀려나기
//-----------------------------------------------------------------------------
void	CField::SM__SLIDE__NPC( int index, DWORD unique, DWORD pos )
{
	int who = FindNpc( unique );
	if( who < MAX_NPC_INTABLE )
	{		
		// 밀려가는 거리
		int cur_x	= m_NpcTable[who].lDestBlock%(m_cSize*2);
		int cur_y	= m_NpcTable[who].lDestBlock/(m_cSize*2);
		int des_x	= pos%(m_cSize*2);
		int des_y	= pos/(m_cSize*2);

		// 메세지저장
		EVENT_DATA_INFO	data;
		data.type	= SM_SLIDE_NPC;
		data.x		= des_x;
		data.y		= des_y;
		data.vPos.x	= (des_x-cur_x)*CLIENT_TILESIZE;
		data.vPos.y	= g_Pc.GetMapHeight( des_x, des_y )-m_Npc[who].m_Mov.curt.height;
		data.vPos.z	= (des_y-cur_y)*CLIENT_TILESIZE;
		m_Npc[who].PushQueue( data, TRUE );
		m_Npc[who].QueueProcess();

		// 블럭처리
		g_Map.SetMapData( NPC_BLOCK, who, pos, false );
		g_Map.SetMapData( CLEAR_BLOCK, who, m_NpcTable[who].lDestBlock, false );		

		// 위치값 테이블에 저장
		m_NpcTable[who].lDestBlock	= pos;
		m_NpcTable[who].lBlock		= pos;

		// 클라이언트 위치값 수정
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
		
		if( npc_code == nNpcCode )					// 일단 해당 NPC꺼냐??!!...
		{
			FindNPCType = 0;
		}

		if( npc_code2 == nNpcCode )					// 보고 NPC 이냐?
		{
			if(0 == FindNPCType)					// 시작 NPC와 보고 NPC가 같으냐?
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

		if( need_level > nRui->thePcParam.Level )		// 그리고 내가 할 수 있는거냐??!!..
		{
			continue;
		}
		
		bool		IsInsertQuest = true;
		QUESTDATA	QuestData;
		
		for( int j = 0; j < RP_MAX_QUEST_FLAG; ++j )
		{
			if( nGroupId[j] == 0 )	//플래그 값이 없을경우 - 시작도 안한경우인데...
			{
				// 보고 NPC 꺼면 아직 시자고 안했으니까 제외
				if( FindNPCType == 1 || ISDisplay == FALSE  )
				{
					IsInsertQuest = false;
					break;
				}

				RLGS("!시작 NPC 로 설정 : APPEAR");

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
			// 플래그 값이 있는 것일때
			else if( nGroupId[j] == QuestGCode )
			{
				//같은 그룹이지만 퀘스트 코드가 틀리면...
				if(nRui->thePcInven.QuestFlag[j]->theQuestCode != quest_code )
				{
					IsInsertQuest = false;
					break;
				}				
				
				///-- 퀘스트 종료	
				if( nRui->thePcInven.QuestFlag[j]->theFlag == 60 )
				{
					IsInsertQuest = false;
					break;
				}
				// 진행중일때
				else
				if( nRui->thePcInven.QuestFlag[j]->theFlag != 60 && FindNPCType != 0 )
				{
					RLGS("!보고 NPC 로 설정 : APPEAR");

					g_Map.m_Npc[nNpcIndex].m_QuestMarkType = REPORT_QUEST_MARK;
					
					QuestData.Code = quest_code;
					QuestData.Type = REPORT_QUEST_MARK;
					
					break;
				}
				// 그외에는 제외
				else
				{
					IsInsertQuest = false;
					break;
				}			
			}
		}
		
		if( IsInsertQuest )
		{
			// 플레이어가 수행 해야 할 퀘스트를 알려주기 위해서...^ ^;;
			g_Map.m_Npc[nNpcIndex].thePreQuestCodeList.push_back( QuestData );
		}		
	}	
}

//................................................................................................................
// 신수 생성
//................................................................................................................
int CField::SM__HOLY__APPEAR( int index, DWORD unique, DWORD pos, DWORD hp, DWORD sp, BYTE status , float fAngle , BYTE Level)
{	
	
	RLG1( "-------------------- SM_HOLY_APPEAR : %d ---------------------------------------" , index );
	
	bool bFindNpc = false;
	int  BaseCode = index;
	
	index = g_Pc_Manager.m_HolyManager.GetHolyCode( BaseCode , Level );

	if( index == -1 )
		return -1;

	// NPC 에 타입에 없는 것을 생성하라고 하면 시스템 다운 
	bFindNpc = g_Pc_Manager.IsFindFileData( (DWORD)index );
	
	if( !bFindNpc )
	{
		if( !g_Pc_Manager.AddNpcCode( index ) )
		{
#ifdef ROCKCLIENT_DEV		
		RLG2( "!현재맵에 Code:%d NPC가 npc_%d.set 파일에 없음!!!", index, g_Map.m_NowMapID );	
#endif 		
			return -1;
		}		
	}

	//-----------------------------------------------------------------------------
	// NPC테이블 비어있는지 찾기
	//-----------------------------------------------------------------------------
	int who = FindNpc( unique );
		
	//-----------------------------------------------------------------------------
	// 없는 NPC이기때문에 로딩해야함
	//-----------------------------------------------------------------------------
	if( who == MAX_NPC_INTABLE )
	{		
		who = GetEmptyTable();

		if( who == MAX_NPC_INTABLE )
		{
			DebugPrint( "SM__HOLY__APPEAR 초과!!!\n" );
			return -1;
		}

		SNPCBaseInfo* npc_Info;
		npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( index ); 
		
		//-----------------------------------------------------------------------------
		// 테이블에 저장 
		//-----------------------------------------------------------------------------
		m_NpcTable[who].code		= index;					
		m_NpcTable[who].index		= npc_Info->theFileCode;	// m_pNpcFileInfo[index] 만들어져 있는 몹의 정보를 리턴 (ChrIndex)
		m_NpcTable[who].theLevel	= Level;					// 신수 레벨 표시 
		m_NpcTable[who].lUnique		= unique;
		m_NpcTable[who].lBlock		= pos;			
		m_NpcTable[who].lDestBlock	= pos;		
		m_NpcTable[who].nStatus		= status;					// 몹 상태에 따라 적용 ( 아직 사용 X )
		m_NpcTable[who].race		= c_NpcRace_Mob;			// npc table 에 따라서 어떤 타입인지 저장...
//		m_NpcTable[who].type		= type;						// 말 색깔에 따라 사용 (백마, 흑마, 적마 등...) 사용 X 
//		m_NpcTable[who].nCurrentFloor = 0;
		Rsprintf( RWCHAR(m_NpcTable[who].szName), _RT("%s"), RWCHAR(npc_Info->theName ));
		m_NpcTable[who].theHP		= hp;
		m_NpcTable[who].theSMP		= sp;
		m_NpcTable[who].theMaxHP	= npc_Info->theMaxHP;
		m_NpcTable[who].theRealHP	= hp;
		m_NpcTable[who].theNpcSize	= npc_Info->theSize;
		
		m_Npc[who].m_State.BaseCode = BaseCode;
		//-----------------------------------------------------------------------------
		// NPC 말이라면 race를 말로 저장
		// 말꼬시는 것때문에.. 말은 따로 알고 있어야 한다
		//-----------------------------------------------------------------------------
//		if( FindHorse( who ) == true || STID_INDEX == index )		
//			m_NpcTable[who].race = HORSE_RACE;
				
		//-----------------------------------------------------------------------------
		// 캐릭터 생성
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
		// NPC보이니? 텍스쳐검사	
		//-----------------------------------------------------------------------------
		UpdateNpc( who );
		
		//-----------------------------------------------------------------------------
		// 게임중일때만 하는 처리
		//-----------------------------------------------------------------------------
		if( g_RockClient.GetGameMode() == GMODE_NORMAL )
		{
			// 블락체크 - 신수일경우에는 블록체크를 하지않는다.
			//g_Map.SetMapData( NPC_BLOCK, who, pos );
			
			// 파티클 있는 놈이면 파티클 생성			
			// g_Particle.Make_Npc_Effect( index, unique );
		}
	}
	//-----------------------------------------------------------------------------
	// 있는 넘인데 다시 받았다면 위치값만 바꾸기
	//-----------------------------------------------------------------------------
	else
	{			
		DebugPrint( "!\r\n SM__HOLY__APPEAR:NPC 또 받음" );
	}
	return who;
}


//................................................................................................................
// 신수 삭제
//................................................................................................................
void	CField::SM__HOLY__DISAPPEAR( int index, DWORD unique )
{
	int who = FindNpc( unique );
	if( who < MAX_NPC_INTABLE )
	{			
		// 해당 타겟의 이펙트삭제
		g_Particle.StopDisplay( unique, index );
		g_Billboard.DelManager( unique, index );
//		g_ChatList.DelUserBalloon( unique, index );

		// 이펙트 리스트 지우기 By wxywxy
		g_Particle.m_EffectManager.DeleteNow( ( Character * ) &m_Npc[who] );
		g_Particle.DelEffSetCon( m_Npc[who].GetUnique() , m_Npc[who].GetIndex() );
		
//		std::vector<int>::iterator ir = g_Map.m_Npc[who].thePreQuestCodeList.begin();
//		for( ; ir != g_Map.m_Npc[who].thePreQuestCodeList.end() ; ++ir )
//		{
			g_Map.m_Npc[who].thePreQuestCodeList.clear();
			g_Map.m_Npc[who].m_QuestMarkType = NONE_QUEST_MARK;
//		}				

		// 맵블럭삭제(타겟이 죽어있는 상태라면 삭제할 필요없다)
//		if( m_Npc[who].m_curt_event.type != SM_STATE_DIE )
			g_Map.SetMapData( CLEAR_BLOCK, who, m_NpcTable[who].lDestBlock, false );

		// NPC테이블삭제
		m_NpcTable[who].Init();
		m_Npc[who].Destroy();
		
		m_Npc[who].m_bIsLoaded = false;
		m_Npc[who].m_bIsVisible = false;		
		m_dwNpcCount--;
	}
	else
	{
		DebugPrint( "!\r\n SM__NPC__DISAPPEAR:NPC 못찼았음" );
	}
}

//................................................................................................................
// 신수 레벨업
//................................................................................................................
void	CField::SM__HOLY__LEVELUP( DWORD Unique, WORD Level )
{
	int Index = g_Map.FindNpc( Unique );

	if( Index == MAX_NPC_INTABLE )
		return;

	if( g_Map.m_Npc[ Index ].m_State.nHolyState != nHolyState_Holy )
		return;

	//...........................................................................................................
	// 이전 리소스 데이터 지우기	
	//...........................................................................................................
	g_Particle.StopDisplay( Unique , Index );
	g_Billboard.DelManager( Unique , Index );

	// 이펙트 리스트 지우기 By wxywxy
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
	
	// NPC 에 타입에 없는 것을 생성하라고 하면 시스템 다운 
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
			// 아이템, 메터리얼
			g_Map.m_Npc[ Index ].m_pItm_index[i]	= g_Pc_Manager.GetInitItmIndex( &g_Map.m_Npc[ Index ] , i );
		}
		
		g_Map.m_Npc[ Index ].m_pMaterial[i]	= g_Pc_Manager.GetItmMaterial( &g_Map.m_Npc[ Index ] , i );
		
		g_Map.m_Npc[ Index ].m_pVtxBuffer[i].Destroy();

		g_Map.m_Npc[ Index ].m_pVtxBuffer[i].CreateBuffer( g_Pc_Manager.GetVtxCount( &g_Map.m_Npc[ Index ] , i) );
		
	}

	// 스케일
	g_Map.m_Npc[ Index ].m_vScale = g_Pc_Manager.GetChrScale( &g_Map.m_Npc[ Index ] );
	
	g_Map.m_Npc[ Index ].AnimationPlay( n_NPC_Stand0 );	
	
	// 현재시간저장
	g_Map.m_Npc[ Index ].m_PreTime	= g_nowTime;	
	g_Map.m_Npc[ Index ].m_SimTime	= g_nowTime;
	
	g_Particle.SetEffSetCon( 0.0f , EF_HOLY_LEVEL_UP , 0 , 5.1f , g_Map.m_Npc[ Index ].GetUnique() , g_Map.m_Npc[ Index ].GetIndex() );

	//......................................................................................................
	// NPC 생성시 이펙트 달고 나오기 By wxywxy
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
// 펫 레벨업
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
	// 이전 리소스 데이터 지우기	
	//...........................................................................................................
	g_Particle.StopDisplay( Unique , Index );
	g_Billboard.DelManager( Unique , Index );

	// 이펙트 리스트 지우기 By wxywxy
	g_Particle.m_EffectManager.DeleteNow( ( Character * ) &m_Npc[ Index ] );
	g_Particle.DelEffSetCon( m_Npc[ Index ].GetUnique() , m_Npc[ Index ].GetIndex() );
		
	//현재 Pet코드와 넘어온 코드값을 비교	
	int nNowPetCode = g_Map.m_NpcTable[Index].code;

	if( nNowPetCode == nNewPetCode )
	{
		g_Map.m_NpcTable[ Index ].theLevel = aLevel;		

		g_Particle.SetEffSetCon( 0.0f , EF_PET_LEVELUP , 0 , 5.1f , g_Map.m_Npc[ Index ].GetUnique() , 
							 g_Map.m_Npc[ Index ].GetIndex() );
		//......................................................................................................
		// NPC 생성시 이펙트 달고 나오기 By wxywxy
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
	
	// NPC 에 타입에 없는 것을 생성하라고 하면 시스템 다운 
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
	
	///-- 펫은 레벨업을 해도 현재 투명 상태는 그대로 존재한다.
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
			// 아이템, 메터리얼
			g_Map.m_Npc[ Index ].m_pItm_index[i]	= g_Pc_Manager.GetInitItmIndex( &g_Map.m_Npc[ Index ] , i );
		}
		
		g_Map.m_Npc[ Index ].m_pMaterial[i]	= g_Pc_Manager.GetItmMaterial( &g_Map.m_Npc[ Index ] , i );
		
		g_Map.m_Npc[ Index ].m_pVtxBuffer[i].Destroy();

		g_Map.m_Npc[ Index ].m_pVtxBuffer[i].CreateBuffer( g_Pc_Manager.GetVtxCount( &g_Map.m_Npc[ Index ] , i) );
		
	}

	// 스케일
	g_Map.m_Npc[ Index ].m_vScale = g_Pc_Manager.GetChrScale( &g_Map.m_Npc[ Index ] );
	
	g_Map.m_Npc[ Index ].AnimationPlay( n_NPC_Stand0 );	
	
	// 현재시간저장
	g_Map.m_Npc[ Index ].m_PreTime	= g_nowTime;	
	g_Map.m_Npc[ Index ].m_SimTime	= g_nowTime;
	
	int nCode =  g_Pc_Manager.m_HolyManager.GetPetLevelUpEffect( nNewPetCode , aLevel );

	g_Particle.SetEffSetCon( 0.0f , nCode , 0 , 5.1f , g_Map.m_Npc[ Index ].GetUnique() , 
							 g_Map.m_Npc[ Index ].GetIndex() );	

	//......................................................................................................
	// NPC 생성시 이펙트 달고 나오기 By wxywxy
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
	// 이전 리소스 데이터 지우기	
	//...........................................................................................................
	g_Particle.StopDisplay( aNPCID , Index );
	g_Billboard.DelManager( aNPCID , Index );

	// 이펙트 리스트 지우기 By wxywxy
	g_Particle.m_EffectManager.DeleteNow( ( Character * ) &m_Npc[ Index ] );
	g_Particle.DelEffSetCon( m_Npc[ Index ].GetUnique() , m_Npc[ Index ].GetIndex() );
		
	//현재 NPC코드와 넘어온 코드값을 비교	
	int nNowNpcCode = g_Map.m_NpcTable[Index].code;

	if( nNowNpcCode == aNPCCode )
	{
		return;
	}
	
	bool bFindNpc = false;
	
	// NPC 에 타입에 없는 것을 생성하라고 하면 시스템 다운 
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
			// 아이템, 메터리얼
			g_Map.m_Npc[ Index ].m_pItm_index[i]	= g_Pc_Manager.GetInitItmIndex( &g_Map.m_Npc[ Index ] , i );
		}
		
		g_Map.m_Npc[ Index ].m_pMaterial[i]	= g_Pc_Manager.GetItmMaterial( &g_Map.m_Npc[ Index ] , i );
		
		g_Map.m_Npc[ Index ].m_pVtxBuffer[i].Destroy();

		g_Map.m_Npc[ Index ].m_pVtxBuffer[i].CreateBuffer( g_Pc_Manager.GetVtxCount( &g_Map.m_Npc[ Index ] , i) );
		
	}

	// 스케일
	g_Map.m_Npc[ Index ].m_vScale = g_Pc_Manager.GetChrScale( &g_Map.m_Npc[ Index ] );
	
	g_Map.m_Npc[ Index ].AnimationPlay( n_NPC_Stand0 );	
	
	// 현재시간저장
	g_Map.m_Npc[ Index ].m_PreTime	= g_nowTime;	
	g_Map.m_Npc[ Index ].m_SimTime	= g_nowTime;

	g_Particle.SetEffSetCon( 0.0f , EF_PET_LEVELUP , 0 , 5.1f , g_Map.m_Npc[ Index ].GetUnique() , 
							 g_Map.m_Npc[ Index ].GetIndex() );	

	//......................................................................................................
	// NPC 생성시 이펙트 달고 나오기 By wxywxy
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
	// 이전 리소스 데이터 지우기	
	//...........................................................................................................
	g_Particle.StopDisplay( aNPCID , Index );
	g_Billboard.DelManager( aNPCID , Index );

	// 이펙트 리스트 지우기 By wxywxy
	g_Particle.m_EffectManager.DeleteNow( ( Character * ) &m_Npc[ Index ] );
	g_Particle.DelEffSetCon( m_Npc[ Index ].GetUnique() , m_Npc[ Index ].GetIndex() );
		
	//현재 NPC코드와 넘어온 코드값을 비교	
	int nNowNpcCode = g_Map.m_NpcTable[Index].code;

	if( nNowNpcCode == aNPCCode )
	{
		return;
	}
	
	bool bFindNpc = false;
	
	// NPC 에 타입에 없는 것을 생성하라고 하면 시스템 다운 
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
			// 아이템, 메터리얼
			g_Map.m_Npc[ Index ].m_pItm_index[i]	= g_Pc_Manager.GetInitItmIndex( &g_Map.m_Npc[ Index ] , i );
		}
		
		g_Map.m_Npc[ Index ].m_pMaterial[i]	= g_Pc_Manager.GetItmMaterial( &g_Map.m_Npc[ Index ] , i );
		
		g_Map.m_Npc[ Index ].m_pVtxBuffer[i].Destroy();

		g_Map.m_Npc[ Index ].m_pVtxBuffer[i].CreateBuffer( g_Pc_Manager.GetVtxCount( &g_Map.m_Npc[ Index ] , i) );
		
	}

	// 스케일
	g_Map.m_Npc[ Index ].m_vScale = g_Pc_Manager.GetChrScale( &g_Map.m_Npc[ Index ] );
	
	g_Map.m_Npc[ Index ].AnimationPlay( n_NPC_Stand0 );	
	
	// 현재시간저장
	g_Map.m_Npc[ Index ].m_PreTime	= g_nowTime;	
	g_Map.m_Npc[ Index ].m_SimTime	= g_nowTime;

//	g_Particle.SetEffSetCon( 0.0f , EF_PET_LEVELUP , 0 , 5.1f , g_Map.m_Npc[ Index ].GetUnique() , 
//							 g_Map.m_Npc[ Index ].GetIndex() );	

	//......................................................................................................
	// NPC 생성시 이펙트 달고 나오기 By wxywxy
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

