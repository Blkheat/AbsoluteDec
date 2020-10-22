
#ifndef __FIND_WAY__
#define __FIND_WAY__

#include	<windows.h>
#include    "..\\map\\fileinfo.h"

//-----------------------------------------------------------------------------
// 1. StartPos, EndPos에 이동할 값을 넣는다
//
// 2. _FindWay() 함수를 실행한다
//
// 3. ChangedEndPos가 TRUE이면 도착타일값이 변경되었다.
//
// 4. DirectMove가 TRUE이면 JuncPos, FALSE이면 NearPos로 이동한다
//-----------------------------------------------------------------------------

#ifdef APPLYWIDE_MAP

 #define FW_MAPSX		1024
 #define FW_MAPSY		1024	
 #define FW_MAXSTEP		1024

#else
 
 #define FW_MAPSX		512	
 #define FW_MAPSY		512	
 #define FW_MAXSTEP		512

#endif

#define	FW_MAXJUNC		256

struct PXY
{
	PXY()
	{
		X = 0;
		Y = 0;
	}

	DWORD	X;
	DWORD	Y;
};

class CFindWay
{
protected:	
	// find way var ----------------------------
	DWORD	NearLength;
	
	PXY		Pos;				//: MapCoord;
	PXY		NewPos;				//: MapCoord;
	BYTE	Dir;				//: Byte;
	WORD	Step;				//: Byte;
	PXY		ShPos;				//: MapCoord;
	PXY		PList[FW_MAXSTEP];	//: ARRAY[0..500] of MapCoord; { List to remember all free paths }
	WORD	LList[FW_MAXSTEP];	//: ARRAY[0..500] of Word; { List to remember lenght of free paths}
	WORD	PCnt;				//: Word;
	DWORD	Shortest;			//: Word;
	int		DiffX,DiffY;		//: Integer;
	WORD	Loops;				//: Word;
	WORD	ShStep;				//: Word;
	WORD	StepCnt;			//: Word;
	WORD	AW;					//: Word;
	PXY		LineStartPos;		//: MapCoord;
	WORD	Steps;				//: Word;
	
	int		NoJunc;
	WORD	LenMap[FW_MAPSX][FW_MAPSY];	// length buffer

public:
	// input var -------------------------------
	DWORD	FWMapSx;			// 길찾기 사용하는 버퍼 클리어 위한 Map size x,y
	DWORD	FWMapSy;	
	PXY		StartPos;			// Tile좌표 사용.
	PXY		EndPos;				

	// output datas
	DWORD	GoalReached;		// 1=ok, 0=no goal, but move to near and change endpos to nearpos
	BOOL	ChangedEndPos;		// Target점 조정 되었음.
	BOOL	DirectMove;			// 충돌없이 바로가는 가까운 거리인 경우 TRUE

	DWORD	FWMaxStep;			// max move step(tile size)
	PXY		JuncPos;			// 끝점 또는 가까운점에 가기위해 처음에 가야될 점.
	PXY		NearPos;			// 결과점 미 도달시 가장 가까운점.(입력변수인 EndPos에도 같이 세팅된다).
	
	PXY		JuncBF[FW_MAXJUNC];	// 이동 junction list

	void *  m_pRefCharacter;	//참조 캐릭터 포인터	
	
public:
	CFindWay() { Clear(); }
	~CFindWay() { Destroy(); }

	void	Clear();
	void	Destroy();

	void	_InitFindWay( DWORD MapSx, DWORD MapSy );
	void	_FindWay();

	void	RotateRight();
	BOOL	IsFwMoveable( DWORD px, DWORD py );
	BOOL	LineIntersect();
	BOOL	SearchMoveable();
	
	int		GetNoJunc() { return NoJunc; }

};


#endif

