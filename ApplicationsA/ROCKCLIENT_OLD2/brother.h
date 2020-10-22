//brother.h
#ifndef __BRO_H__
#define __BRO_H__
#include	<windows.h>
#include	<stdio.h>
#ifdef DIRECT_VERSION_9_MJH
#include	<D3DX9.h>
#else
#include	<D3DX8.h>
#endif // DIRECT_VERSION_9_MJH


#ifdef _DEBUG
	#define ROCKCLIENT_DEV
#endif



/*
struct Char_Item
{
	DWORD	index;		// �ε���	
	DWORD	unique;		// ����ũ
	DWORD	loc;		// ��ġ
	DWORD	life;		// ������( ���� �� )
	DWORD	upgrade;	// ���׷��̵�
	DWORD	color;		// Į��

	Char_Item() { index = 0; unique = 0; }
};
*/

typedef enum NSystemColorType
{
	n_DarkRed = 0,
	n_DarkGreen,	
	n_LightGreen,
	n_LightBlue,
	n_LightYellow,
	n_LightOrange

} NSystemColorType;

typedef enum NSysMsgType
{	
	n_SystemMsg = 0,
	n_AdminMsg,
	n_MapNameMsg

} NSysMsgType;


//=======================================================================
//                   Bresenham's Line algorithm
//=======================================================================
void putpixel( int x, int y, DWORD Color, int size );
void line_Bresenham_X_plus(int x,int y,int dx,int dy,DWORD color, int size);
void line_Bresenham_X_minus(int x,int y,int dx,int dy,DWORD color, int size);
void line_Bresenham_Y_plus(int x,int y,int dx,int dy,DWORD color, int size);
void line_Bresenham_Y_minus(int x,int y,int dx,int dy,DWORD color, int size);
void line_Bresenham( int x1,int y1,int x2,int y2,DWORD color, int size);

int  GetFileCount( char* file_pos, char* file_ext );

void CalcNaturalCubic(float get_abcd[][4],int n, float *vptr);

int  GetIndexKeyInFileName(char *pName);// ���� �̸����κ��� _���Ŀ� �ε��� ���� �д´�
int  GetIndexKeyInFileName(char *pName , char *pStartStr);
void SystemPrint( int aColor, NSysMsgType aType, void * pStr, ... );

/// sooree pack
void GetFileNameOnly( char* pFullName, char* pOutStr );
void GetFileExt( char* pFullName, char* pOutStr );// ��ο��� Ȯ���� �������� �Լ�

int RandomNum( int iMin, int iMax );
float RandomNum( float fMin, float fMax );

void CloseWnd_Error( char* aErrorStr, ... );

#endif
