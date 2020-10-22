
#ifndef _MATH_H_
#define _MATH_H_

#include	<windows.h>
#include	<stdio.h>
#include	"Define.H"


#define		CHANGE_COLOR_COUNT	10

/*
//-----------------------------------------------------------------------------
// Desc: ����
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
// Desc: �ڻ���
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
// Desc: ����
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
// Desc: ���̺�
//-----------------------------------------------------------------------------
struct	COLOR_TABLE
{
	D3DCOLORVALUE	diffuse;
	D3DCOLORVALUE	ambient;	
};


//-----------------------------------------------------------------------------
// Desc: ����ó����ɴ��
//
// ���ӿ��� ����ϴ� ����ó���κа� ��Ÿ ����� ������ �������� ���
//
// Copyright (c) 2003.12.26 By KStaR79
//-----------------------------------------------------------------------------
class EtcTable
{
public:	
	COLOR_TABLE		m_TimeColor[6];						// �ð��� ���� ����
	D3DCOLORVALUE	m_hair_color[CHANGE_COLOR_COUNT];	// �Ӹ�ī������	
	short			m_face_type[CHANGE_COLOR_COUNT];	// ���ε���
	short			m_hair_type[CHANGE_COLOR_COUNT];	// �Ӹ�ī���ε���	
	short			m_fairy_ani[PC_ANI_INDEX_COUNT];	// ���ִϸ��̼� �ε���	

public:
	EtcTable()	{ Clear(); }	

	//-----------------------------------------------------------------------------
	// �ʱ�ȭ
	//-----------------------------------------------------------------------------
	void	Clear();	

	//-----------------------------------------------------------------------------		
	// �������� ���ε����� ���� ó��
	//-----------------------------------------------------------------------------		
	bool	GetDungeonMap( int _nMapIndex );	// �������̴�?

	//-----------------------------------------------------------------------------
	// ��Ÿ���ϴ� ��ġ��
	//-----------------------------------------------------------------------------
	void	GetHorseRidePos( int angle1, int angle2, int& dest1, int& dest2, int size );

	//-----------------------------------------------------------------------------
	// �� �ִϸ��̼� �ε��� ����
	//-----------------------------------------------------------------------------
	int		GetFairyAnimationIndex( int index );

	//-----------------------------------------------------------------------------
	// �ð��� ���� ����ȭ�� ���
	//-----------------------------------------------------------------------------
#ifdef DIRECT_VERSION_9_MJH
	void	ChangeMaterial( D3DMATERIAL9* pMaterial, float value = -1.0f );
#else
	void	ChangeMaterial( D3DMATERIAL8* pMaterial, float value = -1.0f );
#endif // DIRECT_VERSION_9_MJH

	//-----------------------------------------------------------------------------
	// �Ӹ�ī��, �� �ε���(����)
	//-----------------------------------------------------------------------------		
	bool			FindFaceIndex( int index );
	int				GetFaceIndex( int index )		{ return m_face_type[index]; }
	bool			FindHairIndex( int index );
	int				GetHairIndex( int index )		{ return m_hair_type[index]; }	
	D3DCOLORVALUE	GetHairMaterial( int index )	{ return m_hair_color[index]; }

};

extern EtcTable	g_EtcTable;


//-----------------------------------------------------------------------------
// Desc: ���� x ��Ʈ���� = ����
//-----------------------------------------------------------------------------
void	VectorMatrixMultiply( D3DXVECTOR3& vDest, D3DXVECTOR3& vSrc, D3DXMATRIX& mat );
void	Float3MatrixMultiply( float& x, float& y, float& z, D3DXVECTOR3& vSrc, D3DXMATRIX& mat );

//-----------------------------------------------------------------------------
// Desc: ���н�
//-----------------------------------------------------------------------------
DWORD	FloatToDWORD( float f );

//-----------------------------------------------------------------------------
// Desc: ������
//-----------------------------------------------------------------------------
HRESULT DrawIndexedPrimitive( int nVtxCount, int nIdxCount );

#endif
