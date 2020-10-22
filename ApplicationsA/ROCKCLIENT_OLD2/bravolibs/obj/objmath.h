
#ifndef _MATH_H_
#define _MATH_H_

#include	<windows.h>
#include	<stdio.h>
#include	"Define.H"


#define		CHANGE_COLOR_COUNT	10

/*
//-----------------------------------------------------------------------------
// Desc: 라디안
//-----------------------------------------------------------------------------
class Radian
{
public:
	float	m_radian[360];

public:
	Radian()	{ Clear(); }
	~Radian()	{ Destory(); }

	void	Clear();
	void	Destory();
};

extern Radian g_Radian;


//-----------------------------------------------------------------------------
// Desc: 코사인
//-----------------------------------------------------------------------------
class Cos
{
public:
	float	m_cos[ 360 ];

public:
	Cos()	{ Clear(); }
	~Cos()	{ Destory(); }

	void	Clear();
	void	Destory();
};

extern	Cos	g_Cos;



//-----------------------------------------------------------------------------
// Desc: 사인
//-----------------------------------------------------------------------------
class Sin
{
public:
	float	m_sin[ 360 ];


public:
	Sin()	{ Clear(); }
	~Sin()	{ Destory(); }

	void	Clear();
	void	Destory();
};

extern	Sin	g_Sin;
*/


//-----------------------------------------------------------------------------
// Desc: 테이블
//-----------------------------------------------------------------------------
struct	COLOR_TABLE
{
	D3DCOLORVALUE	diffuse;
	D3DCOLORVALUE	ambient;	
};


//-----------------------------------------------------------------------------
// Desc: 예외처리기능담당
//
// 게임에서 사용하는 예외처리부분과 기타 잡다한 변수를 저장으로 사용
//
// Copyright (c) 2003.12.26 By KStaR79
//-----------------------------------------------------------------------------
class EtcTable
{
public:	
	COLOR_TABLE		m_TimeColor[6];						// 시간에 따른 색깔
	D3DCOLORVALUE	m_hair_color[CHANGE_COLOR_COUNT];	// 머리카락색깔	
	short			m_face_type[CHANGE_COLOR_COUNT];	// 얼굴인덱스
	short			m_hair_type[CHANGE_COLOR_COUNT];	// 머리카락인덱스	
	short			m_fairy_ani[PC_ANI_INDEX_COUNT];	// 페어리애니메이션 인덱스	

public:
	EtcTable()	{ Clear(); }	

	//-----------------------------------------------------------------------------
	// 초기화
	//-----------------------------------------------------------------------------
	void	Clear();	

	//-----------------------------------------------------------------------------		
	// 예외적인 맵인덱스에 따른 처리
	//-----------------------------------------------------------------------------		
	bool	GetDungeonMap( int _nMapIndex );	// 던전맵이니?

	//-----------------------------------------------------------------------------
	// 말타야하는 위치값
	//-----------------------------------------------------------------------------
	void	GetHorseRidePos( int angle1, int angle2, int& dest1, int& dest2, int size );

	//-----------------------------------------------------------------------------
	// 페어리 애니메이션 인덱스 리턴
	//-----------------------------------------------------------------------------
	int		GetFairyAnimationIndex( int index );

	//-----------------------------------------------------------------------------
	// 시간에 따른 색깔변화에 사용
	//-----------------------------------------------------------------------------
#ifdef DIRECT_VERSION_9_MJH
	void	ChangeMaterial( D3DMATERIAL9* pMaterial, float value = -1.0f );
#else
	void	ChangeMaterial( D3DMATERIAL8* pMaterial, float value = -1.0f );
#endif // DIRECT_VERSION_9_MJH

	//-----------------------------------------------------------------------------
	// 머리카락, 얼굴 인덱스(색깔)
	//-----------------------------------------------------------------------------		
	bool			FindFaceIndex( int index );
	int				GetFaceIndex( int index )		{ return m_face_type[index]; }
	bool			FindHairIndex( int index );
	int				GetHairIndex( int index )		{ return m_hair_type[index]; }	
	D3DCOLORVALUE	GetHairMaterial( int index )	{ return m_hair_color[index]; }

};

extern EtcTable	g_EtcTable;


//-----------------------------------------------------------------------------
// Desc: 벡터 x 메트릭스 = 벡터
//-----------------------------------------------------------------------------
void	VectorMatrixMultiply( D3DXVECTOR3& vDest, D3DXVECTOR3& vSrc, D3DXMATRIX& mat );
void	Float3MatrixMultiply( float& x, float& y, float& z, D3DXVECTOR3& vSrc, D3DXMATRIX& mat );

//-----------------------------------------------------------------------------
// Desc: 수학식
//-----------------------------------------------------------------------------
DWORD	FloatToDWORD( float f );

//-----------------------------------------------------------------------------
// Desc: 렌더링
//-----------------------------------------------------------------------------
HRESULT DrawIndexedPrimitive( int nVtxCount, int nIdxCount );

#endif
