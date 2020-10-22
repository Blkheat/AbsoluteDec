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
// Desc: ���Ⱚ
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
// Desc: Ÿ�ϰ˻� ���Ѱ� ����
//-----------------------------------------------------------------------------
void CFindWay::_InitFindWay( DWORD MapSx, DWORD MapSy )
{
	FWMapSx = MapSx;
	FWMapSy = MapSy;

	// ���� 1/3���� �˻縦 �ص� ���� �� ã���� �����ٰ� �Ǵ��ϴµ� ����ϴ� ī���� ��.
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
// Desc: �ش� Ÿ���� �� �� �ִ� ������ �������ֱ�
//-----------------------------------------------------------------------------
BOOL  CFindWay::IsFwMoveable( DWORD px, DWORD py )
{	
	///-- By simwoosung
	///-- �̺κ� ����������....
	if( g_Map.m_pAttrMap[py][px].byAttr == MATTR_OBJ ) 
	{
		return false;
	}

	bool IsMove = g_Map.m_Block[px][py].GetClear();

	if(IsMove)
	{
		///-- ���� �� �ڵ����� �˰����� ������ �ִ� ��ü�� ���� ��ġ�� 
		///-- �̵��Ϸ��� �ϴ� ��ġ�� ������ ���Ѵ�.
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
// Desc: �־��� ����� �����Ÿ��� �̵� �������� Ȯ��
//
// LineStartPos -> Pos���� ���.
//
// Output:
// TRUE		= �浹�� �Ͼ��.
// FALSE	= �浹���� ���� ����.
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
		if( base == 1 )		// x base (x������ �̵��ϸ鼭 �˻�)
		{
			x += add_x;
			ay += dist_y;
			if( ay >= dist_x ) 
			{ 
				y += add_y; 
				ay -= dist_x;
			}
		}
		else				// y base (y������ �̵��ϸ鼭 �˻�)
		{
			y += add_y;
			ax += dist_x;
			if( ax >= dist_y )
			{ 
				x += add_x; 
				ax -= dist_y; 
			}
		}
		
		// Ÿ�ϰ˻�
		if( IsFwMoveable( x, y ) == FALSE )
			return FALSE;

//		Steps++;
	}
	return TRUE;
}


//-----------------------------------------------------------------------------
// EndPos�������� StartPos�� ���� �ִ� ���� ����� �� ã��.
//
// EndPos -> StartPos���� ���.
//
// Output:
//		TRUE  = NewPos���� �ִ� ���� �������.
//		FALSE = ���� �ִ��� ����.
//
// Desc:
//	�ǹ��� ��� �ִ°����� ���� ������ �� ������ �����Ÿ��� �ؼ� 
//  ����� �Ÿ��� ã�� �ʰ� ���� �;� ��� ������ ���� �׸��鼭
//	Ȯ���ϸ鼭 ���� �ִ� ó������ ã�´�.
//-----------------------------------------------------------------------------
BOOL CFindWay::SearchMoveable()
{
	// ��� ���� �غ�
	BOOL FoundOne = FALSE;
	WORD FoundLen;

	// ��뺯�� �غ�.
	PXY P1;
	PXY P2;
	P1.X = EndPos.X - 1;
	P1.Y = EndPos.Y - 1;
	P2.X = EndPos.X + 1;
	P2.Y = EndPos.Y + 1;

	int x;
	int y;

	// �˻� ����.
	RE_CHECK:

	// x�� ���� -----> ���� �˻�.
	for( x = P1.X; x < (int) P2.X; ++x ) 
	{
		// �� Y
		if(IsFwMoveable(x, P1.Y))
		{
			if(FoundOne)
			{	//  ���� ã������ ������ �Ÿ� ��
				WORD NewLen=abs((long)x- (long)StartPos.X)+abs((long)P1.Y- (long)StartPos.Y);
				if(FoundLen>NewLen)
				{
					NewPos.X=x;
					NewPos.Y=P1.Y;
					FoundLen=NewLen;
				}
			}
			else
			{	//  ó�� ã�����̶� �ٷ� ����.
				NewPos.X=x;
				NewPos.Y=P1.Y;
				FoundLen=abs((long)x- (long)StartPos.X)+abs((long)P1.Y- (long)StartPos.Y);
				FoundOne=TRUE;
			}
		}

		// �Ʒ� Y
		if(IsFwMoveable(x, P2.Y))
		{
			if(FoundOne)
			{	//  ���� ã������ ������ �Ÿ� ��
				WORD NewLen=abs((long)x- (long)StartPos.X)+abs((long)P2.Y- (long)StartPos.Y);
				if(FoundLen>NewLen)
				{
					NewPos.X=x;
					NewPos.Y=P2.Y;
					FoundLen=NewLen;
				}
			}
			else
			{	//  ó�� ã�����̶� �ٷ� ����.
				NewPos.X=x;
				NewPos.Y=P2.Y;
				FoundLen=abs((long)x- (long)StartPos.X)+abs((long)P2.Y- (long)StartPos.Y);
				FoundOne=TRUE;
			}
		}
	}

	// y�� ����-----> ���� �˻�.
	for( y = P1.Y + 1; y < (int) P2.Y - 1; ++y )
	{
		// ���� X
		if(IsFwMoveable(P1.X, y))
		{
			if(FoundOne)
			{	//  ���� ã������ ������ �Ÿ� ��
				WORD NewLen=abs((long)P1.X- (long)StartPos.X)+abs((long)y- (long)StartPos.Y);
				if(FoundLen>NewLen)
				{
					NewPos.X=P1.X;
					NewPos.Y=y;
					FoundLen=NewLen;
				}
			}
			else
			{	//  ó�� ã�����̶� �ٷ� ����.
				NewPos.X=P1.X;
				NewPos.Y=y;
				FoundLen=abs((long)P1.X- (long)StartPos.X)+abs((long)y- (long)StartPos.Y);
				FoundOne=TRUE;
			}
		}

		// ������ X
		if(IsFwMoveable(P2.X, y))
		{
			if(FoundOne)
			{	//  ���� ã������ ������ �Ÿ� ��
				WORD NewLen=abs((long)P2.X- (long)StartPos.X)+abs((long)y- (long)StartPos.Y);
				if(FoundLen>NewLen)
				{
					NewPos.X=P2.X;
					NewPos.Y=y;
					FoundLen=NewLen;
				}
			}
			else
			{	//  ó�� ã�����̶� �ٷ� ����.
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
// Desc: ��ã��
//-----------------------------------------------------------------------------
void CFindWay::_FindWay()
{
	int li_max_loop;	///-- �ִ� ���� ����

	// end�� ��ü�� ���� �ִ��� Ȯ��.
	ChangedEndPos	= FALSE;
	FWMaxStep		= 0;

	if( IsFwMoveable( EndPos.X, EndPos.Y ) == FALSE )
	{
		// �־��� end���� ������ ���� �ִ� �������� ã�´�.
		if( SearchMoveable() )
		{
			// ���� �ִ����� �������� ����.
			EndPos			= NewPos;
			ChangedEndPos	= TRUE;

			if( ( EndPos.X == StartPos.X ) && ( EndPos.Y == StartPos.Y ) )
			{	
				// �ٷ� ���� ����.
				GoalReached = 0;		// EndPos���ſ䱸�� -> tarpx,tarpy�����Ǿ����.
				DirectMove	= FALSE;
				JuncPos = EndPos;				
				return;
			}
		}
		else
		{
			// ���� ����.
			GoalReached		= 0;		// EndPos���ſ䱸�� -> tarpx,tarpy�����Ǿ����.
			ChangedEndPos	= TRUE;			
			DirectMove		= FALSE;
			EndPos = JuncPos = StartPos;
			return;
		}
	}

	// �ѹ��� �浹���� �ٷ� ���� �ִ��� �˻�.
	LineStartPos = StartPos;
	Pos = EndPos;

	if( LineIntersect() == TRUE )
	{	
		// �ٷ� ���� ����.
		GoalReached = 1;
		JuncPos		= EndPos;
		NearPos		= EndPos;
		DirectMove	= TRUE;
		return;
	}
	else
		DirectMove = FALSE;

	// ���������� ��ã�� ������ �غ�.
	// Length map clear
	for( DWORD x = 0; x < FWMapSx; ++x )
		memset( LenMap[x], -1, FWMapSy*sizeof(WORD) );

	// ��뺯�� �غ�.
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
		///-- �ִ� loop
		++li_max_loop;
		if( li_max_loop >= 6000 && 1)
		{
			GoalReached		= 0;		// EndPos���ſ䱸�� -> tarpx,tarpy�����Ǿ����.
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

		// �ش��� ���� ���� ��츦 ����ؼ� ���� ������� ����.
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
				if( IsFwMoveable( NewPos.X, NewPos.Y ) )		// ���� �ִ��� Ȯ���� ���� �ɸ��Ƿ� ������ ���� Ȯ��.
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

			//	RotateRight();	// �Ʒ� ���̺� �ڵ� �����.
			NewPos.X += XTbl[Dir];
			NewPos.Y += YTbl[Dir];
			Dir		 =	DirTbl[Dir];
		}

		// ���� �ִ°� �˻縦 ���� �������� �ص� ��ã���� �����ٰ� �Ǵ�.
		// ����� ������ �ϴ� �����Ѵ�.
#ifdef IS_MOVEABLE_CNT_CHK
		if(IsMoveAbleCnt>MaxMoveableCnt) 
			break;
#endif

	} while( (!GoalReached) && (PCnt != 0) );

	// ������� �� ���޽� ���� ���� ������ ���� ����� �Ÿ� ã��.
	if( !GoalReached ) 
	{
		ChangedEndPos = TRUE;
		EndPos = NearPos;
	}

	// EndPos���� StartPos���� �߰ߵ� ���� ��� �����Ѵ�. 
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
		// ������� ����.
		NewPos = JuncBF[NoJunc] = ShPos;
		++NoJunc;
		if( NoJunc >= FW_MAXJUNC ) 
		{	
			// ���� ������ �����ٰ� �Ǵ�.
			ChangedEndPos=TRUE;
			JuncPos = EndPos = StartPos;
			NoJunc = 0;
			DirectMove = TRUE;
			return;
		}
	}

	// �����Ÿ�
	FWMaxStep = NoJunc;
	int nFindStep = 1;
	if( NoJunc == 1 )
		nFindStep = 0;

	// StartPos���� EndPos���� �ٷ� ���� �ִ� ��(���� JuncPos)��ã�´�.
	//while( NoJunc > 0 )
	while( NoJunc > nFindStep )
	{
		--NoJunc;
		LineStartPos = StartPos;
		Pos = JuncBF[NoJunc];

		// �������� �� �� �ִ��� �˻�
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
