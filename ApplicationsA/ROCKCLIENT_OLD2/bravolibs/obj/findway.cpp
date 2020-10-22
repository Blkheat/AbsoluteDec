#include "RockPCH.h"

#include "..\\..\\RockClient.h"
#include	"FindWay.H"
#include	"..\\quadlist.h"
#include	<Map\Field.h>
#include	"..\\Map.H"


#define	IS_MOVEABLE_CNT_CHK

// extern ----------------------------------
//extern "C" _ismoveable();

// Debug vars
#ifdef IS_MOVEABLE_CNT_CHK
DWORD	IsMoveAbleCnt;
DWORD	MaxMoveableCnt = (FW_MAPSX*FW_MAPSY)/3;
#endif


//-----------------------------------------------------------------------------
// Desc: 방향값
//-----------------------------------------------------------------------------
//								0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12
static BYTE		DirTbl[]	= {	0, 3, 4, 6, 8, 0,12, 0, 1, 2, 0, 0, 9 };
static DWORD	XTbl[]		= {	0, 0, 1, 2,-1, 0, 0, 0,-1, 1, 0, 0,-2 };
static DWORD	YTbl[]		= {	0,-1, 1, 0, 1, 0, 2, 0,-1,-2, 0, 0, 0 };


class CFindWay;

void CFindWay::Clear()
{
	NearLength	= 0;
	Dir			= 0;
	Step		= 0;
	memset( PList, 0, sizeof(PXY)*FW_MAXSTEP );
	memset( LList, 0, sizeof(WORD)*FW_MAXSTEP );
	PCnt		= 0;
	Shortest    = 0;
    DiffX       = 0;
    DiffY       = 0;
    Loops		= 0;
	ShStep		= 0;
	StepCnt		= 0;	
	AW			= 0;
	Steps		= 0;
	
	NoJunc		= 0;
	memset( &LenMap, 0, sizeof(WORD)*(FW_MAPSX*FW_MAPSY) );

	FWMapSx		= FW_MAPSX;
	FWMapSy		= FW_MAPSY;	

	GoalReached = 0;;
	ChangedEndPos = FALSE;	
	DirectMove	= TRUE;	

	FWMaxStep	= 0;

	m_pRefCharacter = NULL;
}

void CFindWay::Destroy()
{
}
	
//-----------------------------------------------------------------------------
// Desc: 타일검사 제한값 저장
//-----------------------------------------------------------------------------
void CFindWay::_InitFindWay( DWORD MapSx, DWORD MapSy )
{
	FWMapSx = MapSx;
	FWMapSy = MapSy;

	// 맵의 1/3정도 검사를 해도 길을 못 찾으면 못간다고 판단하는데 사용하는 카운터 값.
	#ifdef IS_MOVEABLE_CNT_CHK
	MaxMoveableCnt = (MapSx*MapSy)/3;
	#endif
}

void  CFindWay::RotateRight()
{	
	NewPos.X += XTbl[Dir];
	NewPos.Y += YTbl[Dir];
	Dir		 =	DirTbl[Dir];
}

//-----------------------------------------------------------------------------
// Desc: 해당 타일이 갈 수 있는 곳인지 리턴해주기
//-----------------------------------------------------------------------------
BOOL  CFindWay::IsFwMoveable( DWORD px, DWORD py )
{	
	///-- By simwoosung
	///-- 이부분 수정들어가야함....
	if( g_Map.m_pAttrMap[py][px].byAttr == MATTR_OBJ ) 
	{
		return false;
	}

	bool IsMove = g_Map.m_Block[px][py].GetClear();

	if(IsMove)
	{
		///-- 현재 이 자동추적 알고리즘을 가지고 있는 주체의 현재 위치와 
		///-- 이동하려고 하는 위치의 방향을 구한다.
		if(m_pRefCharacter)
		{
			Character *pCharacter = (Character *)m_pRefCharacter;	
			D3DXVECTOR3 vNexPos = D3DXVECTOR3( (float)( (px * CLIENT_TILESIZE) + (CLIENT_TILESIZE / 2) ),
												0.0f,
											   (float)( (py * CLIENT_TILESIZE) + (CLIENT_TILESIZE / 2) ) );	
			
			bool bMove = g_Map.IsNoGiantUnit(pCharacter->m_Mov.vStPos, vNexPos, pCharacter);
			IsMove = bMove;
		}
	}

	return IsMove;
}


//-----------------------------------------------------------------------------
// Desc: 주어진 점까기 직선거리로 이동 가능한지 확인
//
// LineStartPos -> Pos변수 사용.
//
// Output:
// TRUE		= 충돌이 일어났음.
// FALSE	= 충돌없이 갈수 있음.
//
//-----------------------------------------------------------------------------
BOOL  CFindWay::LineIntersect()
{
	DWORD 	x, y;
	DWORD	add_x, add_y;
	DWORD	dist_x, dist_y, ax, ay;

//	Steps = 0;

	dist_x	= abs((long)LineStartPos.X- (long)Pos.X);
	dist_y	= abs((long)LineStartPos.Y- (long)Pos.Y);
	x	= LineStartPos.X;
	y	= LineStartPos.Y;

	if( LineStartPos.X > Pos.X )
		add_x = -1; 
	else 
		add_x = 1;

	if( LineStartPos.Y > Pos.Y) 
		add_y = -1; 
	else 
		add_y = 1;

	if( dist_x == 0 ) 
		add_x = 0;	
	if( dist_y == 0 ) 
		add_y = 0;
	
	ax = dist_x;
	ay = dist_y;

	BYTE base;				// x base = 1, y base = 2
	if( dist_x >= dist_y ) 
		base = 1; 
	else 
		base = 2;

	while( x != Pos.X || y != Pos.Y )
	{
		if( base == 1 )		// x base (x축으로 이동하면서 검색)
		{
			x += add_x;
			ay += dist_y;
			if( ay >= dist_x ) 
			{ 
				y += add_y; 
				ay -= dist_x;
			}
		}
		else				// y base (y축으로 이동하면서 검색)
		{
			y += add_y;
			ax += dist_x;
			if( ax >= dist_y )
			{ 
				x += add_x; 
				ax -= dist_y; 
			}
		}
		
		// 타일검사
		if( IsFwMoveable( x, y ) == FALSE )
			return FALSE;

//		Steps++;
	}
	return TRUE;
}


//-----------------------------------------------------------------------------
// EndPos주위에서 StartPos로 갈수 있는 가장 가까운 점 찾기.
//
// EndPos -> StartPos변수 사용.
//
// Output:
//		TRUE  = NewPos갈수 있는 가장 가까운점.
//		FALSE = 갈수 있는점 없음.
//
// Desc:
//	건물이 길게 있는곳에서 끝을 찍으면 못 감으로 직선거리로 해서 
//  가까운 거리를 찾지 않고 가고 싶어 사는 주위로 원을 그리면서
//	확산하면서 갈수 있는 처음점을 찾는다.
//-----------------------------------------------------------------------------
BOOL CFindWay::SearchMoveable()
{
	// 결과 변수 준비
	BOOL FoundOne = FALSE;
	WORD FoundLen;

	// 사용변수 준비.
	PXY P1;
	PXY P2;
	P1.X = EndPos.X - 1;
	P1.Y = EndPos.Y - 1;
	P2.X = EndPos.X + 1;
	P2.Y = EndPos.Y + 1;

	int x;
	int y;

	// 검사 시작.
	RE_CHECK:

	// x점 증가 -----> 으로 검사.
	for( x = P1.X; x < (int) P2.X; ++x ) 
	{
		// 위 Y
		if(IsFwMoveable(x, P1.Y))
		{
			if(FoundOne)
			{	//  현재 찾은점이 있으면 거리 비교
				WORD NewLen=abs((long)x- (long)StartPos.X)+abs((long)P1.Y- (long)StartPos.Y);
				if(FoundLen>NewLen)
				{
					NewPos.X=x;
					NewPos.Y=P1.Y;
					FoundLen=NewLen;
				}
			}
			else
			{	//  처음 찾은점이라 바로 세팅.
				NewPos.X=x;
				NewPos.Y=P1.Y;
				FoundLen=abs((long)x- (long)StartPos.X)+abs((long)P1.Y- (long)StartPos.Y);
				FoundOne=TRUE;
			}
		}

		// 아래 Y
		if(IsFwMoveable(x, P2.Y))
		{
			if(FoundOne)
			{	//  현재 찾은점이 있으면 거리 비교
				WORD NewLen=abs((long)x- (long)StartPos.X)+abs((long)P2.Y- (long)StartPos.Y);
				if(FoundLen>NewLen)
				{
					NewPos.X=x;
					NewPos.Y=P2.Y;
					FoundLen=NewLen;
				}
			}
			else
			{	//  처음 찾은점이라 바로 세팅.
				NewPos.X=x;
				NewPos.Y=P2.Y;
				FoundLen=abs((long)x- (long)StartPos.X)+abs((long)P2.Y- (long)StartPos.Y);
				FoundOne=TRUE;
			}
		}
	}

	// y점 증가-----> 으로 검사.
	for( y = P1.Y + 1; y < (int) P2.Y - 1; ++y )
	{
		// 왼쪽 X
		if(IsFwMoveable(P1.X, y))
		{
			if(FoundOne)
			{	//  현재 찾은점이 있으면 거리 비교
				WORD NewLen=abs((long)P1.X- (long)StartPos.X)+abs((long)y- (long)StartPos.Y);
				if(FoundLen>NewLen)
				{
					NewPos.X=P1.X;
					NewPos.Y=y;
					FoundLen=NewLen;
				}
			}
			else
			{	//  처음 찾은점이라 바로 세팅.
				NewPos.X=P1.X;
				NewPos.Y=y;
				FoundLen=abs((long)P1.X- (long)StartPos.X)+abs((long)y- (long)StartPos.Y);
				FoundOne=TRUE;
			}
		}

		// 오른쪽 X
		if(IsFwMoveable(P2.X, y))
		{
			if(FoundOne)
			{	//  현재 찾은점이 있으면 거리 비교
				WORD NewLen=abs((long)P2.X- (long)StartPos.X)+abs((long)y- (long)StartPos.Y);
				if(FoundLen>NewLen)
				{
					NewPos.X=P2.X;
					NewPos.Y=y;
					FoundLen=NewLen;
				}
			}
			else
			{	//  처음 찾은점이라 바로 세팅.
				NewPos.X=P2.X;
				NewPos.Y=y;
				FoundLen=abs((long)P2.X- (long)StartPos.X)+abs((long)y- (long)StartPos.Y);
				FoundOne=TRUE;
			}
		}
	}

	if(FoundOne) return TRUE;

	if( (StartPos.X >= P1.X) && (StartPos.X <= P2.X) &&
		(StartPos.Y >= P1.Y) && (StartPos.Y <= P2.Y) )
		return FALSE;

	if( P1.X > 0 ) --P1.X;
	if( P1.Y > 0 ) --P1.Y;
	if( P2.X < FWMapSx ) ++P2.X;
	if( P2.Y < FWMapSy ) ++P2.Y;
	if( ((P1.X+P1.Y)==0) && ((P2.X+P2.Y)==(FWMapSx+FWMapSy-2)) )	// check last
	{
		return FALSE;
	}
	goto RE_CHECK;
}

//-----------------------------------------------------------------------------
// Desc: 길찾기
//-----------------------------------------------------------------------------
void CFindWay::_FindWay()
{
	int li_max_loop;	///-- 최대 루프 제한

	// end점 자체가 갈수 있는지 확인.
	ChangedEndPos	= FALSE;
	FWMaxStep		= 0;

	if( IsFwMoveable( EndPos.X, EndPos.Y ) == FALSE )
	{
		// 주어진 end점이 못가면 갈수 있는 최초점을 찾는다.
		if( SearchMoveable() )
		{
			// 갈수 있는점을 끝점으로 수정.
			EndPos			= NewPos;
			ChangedEndPos	= TRUE;

			if( ( EndPos.X == StartPos.X ) && ( EndPos.Y == StartPos.Y ) )
			{	
				// 바로 갈수 있음.
				GoalReached = 0;		// EndPos갱신요구됨 -> tarpx,tarpy수정되어야함.
				DirectMove	= FALSE;
				JuncPos = EndPos;				
				return;
			}
		}
		else
		{
			// 갈수 없음.
			GoalReached		= 0;		// EndPos갱신요구됨 -> tarpx,tarpy수정되어야함.
			ChangedEndPos	= TRUE;			
			DirectMove		= FALSE;
			EndPos = JuncPos = StartPos;
			return;
		}
	}

	// 한번도 충돌없이 바로 갈수 있는지 검사.
	LineStartPos = StartPos;
	Pos = EndPos;

	if( LineIntersect() == TRUE )
	{	
		// 바로 갈수 있음.
		GoalReached = 1;
		JuncPos		= EndPos;
		NearPos		= EndPos;
		DirectMove	= TRUE;
		return;
	}
	else
		DirectMove = FALSE;

	// 본격적으로 길찾기 시작할 준비.
	// Length map clear
	for( DWORD x = 0; x < FWMapSx; ++x )
		memset( LenMap[x], -1, FWMapSy*sizeof(WORD) );

	// 사용변수 준비.
	PCnt = 1;
	PList[0] = StartPos;
	DiffX = (StartPos.X - EndPos.X);
	DiffY = (StartPos.Y - EndPos.Y);
	LList[0] = abs(DiffX)+abs(DiffY);
	LenMap[StartPos.X][StartPos.Y] = 1;
	
	NewPos = Pos = StartPos;
	Step = 1;
	GoalReached = 0;

#ifdef IS_MOVEABLE_CNT_CHK
	IsMoveAbleCnt = 0;
#endif

	NearLength = -1;

	li_max_loop = 0;
	do 
	{
		///-------------------------------------------------------------------
		///-- 최대 loop
		++li_max_loop;
		if( li_max_loop >= 6000 && 1)
		{
			GoalReached		= 0;		// EndPos갱신요구됨 -> tarpx,tarpy수정되어야함.
			ChangedEndPos	= TRUE;			
			DirectMove		= FALSE;
			EndPos = JuncPos = StartPos;
			
			if( m_pRefCharacter == (void *)(g_Pc.GetPlayer()) )
			{
				///-- SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( IDS_SYS_TOOLONGDISFORAOTOCURCUIT ) ); 
			}
			return;
		}
		
		
		// Find the one with the shortest distance in the list
		Shortest = 0;

		for( DWORD CI = 0; CI < PCnt; ++CI )
		{
			if( LList[CI] < LList[Shortest] )
				Shortest = CI;
		}

	    Pos = PList[Shortest];				// Get the one with the shortest distance

		// 해당점 까지 못갈 경우를 대비해서 가장 가까운점 보관.
		if( LList[Shortest] < NearLength ) 
		{
			NearPos = Pos;
			NearLength = LList[Shortest];
		}

	    PList[Shortest] = PList[PCnt-1];	// Replace this one with the last one in the list
	    LList[Shortest] = LList[PCnt-1];  
	    --PCnt;								// Make the list one point smaller
	
	    NewPos = Pos;
	    Dir = 2; 
		--NewPos.Y;							// Start with direction upwards 

	    for( DWORD BI = 0; BI < 8; ++BI )	// Scan all 8 directions to find free ways to go 
		{
			// Check to see if there is a free square here
			if( LenMap[NewPos.X][NewPos.Y] == (WORD) - 1 )
			{
				if( IsFwMoveable( NewPos.X, NewPos.Y ) )		// 갈수 있는지 확인이 오래 걸리므로 가본곳 부터 확인.
				{
					// Record stepvalue to get into this square
					StepCnt = LenMap[Pos.X][Pos.Y] + 1;
					LenMap[NewPos.X][NewPos.Y] = StepCnt;
					
					PList[PCnt] = NewPos;

					// Add the distance to list also
					DiffX = (NewPos.X - EndPos.X);
					DiffY = (NewPos.Y - EndPos.Y);
					LList[PCnt] = abs(DiffX) + abs(DiffY);		//+StepCnt;
					++PCnt;
					if( PCnt >= FW_MAXSTEP ) break;				// overbuffer
				}
			}

			// End the checking if we have reached the endpoint
			if ( (NewPos.X == EndPos.X) && (NewPos.Y==EndPos.Y) )
			{
				GoalReached = 1; 
				BI = 7; 
			}

			//	RotateRight();	// 아래 테이블 코드 사용함.
			NewPos.X += XTbl[Dir];
			NewPos.Y += YTbl[Dir];
			Dir		 =	DirTbl[Dir];
		}

		// 갈수 있는곳 검사를 맵의 절반정도 해도 못찾으면 못간다고 판단.
		// 가까운 곳으로 일단 가게한다.
#ifdef IS_MOVEABLE_CNT_CHK
		if(IsMoveAbleCnt>MaxMoveableCnt) 
			break;
#endif

	} while( (!GoalReached) && (PCnt != 0) );

	// 결과점에 미 도달시 가장 까까운 점까지 가는 가까운 거리 찾음.
	if( !GoalReached ) 
	{
		ChangedEndPos = TRUE;
		EndPos = NearPos;
	}

	// EndPos에서 StartPos까지 발견됨 점을 모두 저장한다. 
	NoJunc = 0;
	NewPos = EndPos;
	ShStep = -1;
	while( NewPos.X != StartPos.X || NewPos.Y != StartPos.Y )
	{
		Dir = 2; 
		--NewPos.Y;						// Start with direction upwards

		for( DWORD AI = 0; AI < 8; ++AI )
		{
			if( (LenMap[NewPos.X][NewPos.Y] < ShStep) &&
			    (NewPos.X < FWMapSx && NewPos.Y < FWMapSx) )
			{
				ShStep = LenMap[NewPos.X][NewPos.Y];
				ShPos = NewPos;
			}		
			NewPos.X += XTbl[Dir];
			NewPos.Y += YTbl[Dir];
			Dir		 =	DirTbl[Dir];
		}
		// 가까운점 보관.
		NewPos = JuncBF[NoJunc] = ShPos;
		++NoJunc;
		if( NoJunc >= FW_MAXJUNC ) 
		{	
			// 버퍼 부족시 못간다고 판단.
			ChangedEndPos=TRUE;
			JuncPos = EndPos = StartPos;
			NoJunc = 0;
			DirectMove = TRUE;
			return;
		}
	}

	// 최종거리
	FWMaxStep = NoJunc;
	int nFindStep = 1;
	if( NoJunc == 1 )
		nFindStep = 0;

	// StartPos에서 EndPos까지 바로 갈수 있는 점(최종 JuncPos)을찾는다.
	//while( NoJunc > 0 )
	while( NoJunc > nFindStep )
	{
		--NoJunc;
		LineStartPos = StartPos;
		Pos = JuncBF[NoJunc];

		// 직선으로 갈 수 있는지 검사
		if( LineIntersect() == FALSE )
		{
			if( JuncBF[NoJunc+2].X == StartPos.X && JuncBF[NoJunc+2].Y == StartPos.Y )
				break;
			JuncPos = JuncBF[NoJunc+2];
			break;
		}
		else
		{
			JuncPos = JuncBF[NoJunc];
			if( NoJunc == 1 && JuncBF[NoJunc].X == StartPos.X && JuncBF[NoJunc].Y == StartPos.Y )
				nFindStep = 0;
		}
	}
}

//-----------------------------------------------------------------------------
// end of files
//-----------------------------------------------------------------------------
