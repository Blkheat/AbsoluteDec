
#ifndef __FIND_WAY__
#define __FIND_WAY__

#include	<windows.h>
#include    "..\\map\\fileinfo.h"

//-----------------------------------------------------------------------------
// 1. StartPos, EndPos�� �̵��� ���� �ִ´�
//
// 2. _FindWay() �Լ��� �����Ѵ�
//
// 3. ChangedEndPos�� TRUE�̸� ����Ÿ�ϰ��� ����Ǿ���.
//
// 4. DirectMove�� TRUE�̸� JuncPos, FALSE�̸� NearPos�� �̵��Ѵ�
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
	DWORD	FWMapSx;			// ��ã�� ����ϴ� ���� Ŭ���� ���� Map size x,y
	DWORD	FWMapSy;	
	PXY		StartPos;			// Tile��ǥ ���.
	PXY		EndPos;				

	// output datas
	DWORD	GoalReached;		// 1=ok, 0=no goal, but move to near and change endpos to nearpos
	BOOL	ChangedEndPos;		// Target�� ���� �Ǿ���.
	BOOL	DirectMove;			// �浹���� �ٷΰ��� ����� �Ÿ��� ��� TRUE

	DWORD	FWMaxStep;			// max move step(tile size)
	PXY		JuncPos;			// ���� �Ǵ� ��������� �������� ó���� ���ߵ� ��.
	PXY		NearPos;			// ����� �� ���޽� ���� �������.(�Էº����� EndPos���� ���� ���õȴ�).
	
	PXY		JuncBF[FW_MAXJUNC];	// �̵� junction list

	void *  m_pRefCharacter;	//���� ĳ���� ������	
	
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

