///////////////////////////////////////////////////////////////////////////////
///
///		File		: ImageBox.h
///		Desc		:
///
///		Author		: Park Soo-Hyun
///		Team		: Program - Client Team
///		Date		: 
///
///		Copyright(c) 2004, Rocksoft Co., Ltd. ALL RIGHTS RESERVED
///
///////////////////////////////////////////////////////////////////////////////
#ifndef __IMAGEBOX_H__
#define __IMAGEBOX_H__
//-----------------------------------------------------------------------------
#include "TextBox.h"

///---------------------------------------------------------------------------
///-- CImageBox
///---------------------------------------------------------------------------
class CImageBox : public CTextBox
{
public:
	CImageBox(){}
	CImageBox( SDesktop* DT );
	virtual ~CImageBox();
	
	virtual void RenderProc();//OR

	//by simwoosung
	void SetGlow(BOOL flag)    { m_bGlow = flag; }
	void SetClip(BOOL Clip)		{ m_bClip = Clip;} 

protected:
	
	BOOL        m_bGlow;
	BOOL		m_bClip; 

};
///---------------------------------------------------------------------------
///-- CImageBox9
///---------------------------------------------------------------------------
class CImageBox9: public CTextBox
{
	public:
	CImageBox9(){}
	CImageBox9( SDesktop* DT );
	virtual ~CImageBox9();
	
	virtual void RenderProc();//OR

	
	void SetGlow(BOOL flag)    { m_bGlow = flag; }

	void SetAdjustPosition( int PosX, int PosY );
	void SetWndTileSizeLR( const int al_left, const int al_right );		///-- Tile3 일 경우의 LR Tile 크기지정
	void SetWndTileSizeTB( const int al_top, const int al_bottom );		///-- Tile9 일 경우의 TB Tile 크기지정

protected:

	signed long int	maal_tile[9][4];				///-- Split Wnd 의 각 조각들
	signed long int	ml_Wnd_width[2];				///-- Tile 3 에서 Left / Right Wnd 의 크기
	signed long int	ml_Wnd_height[2];				///-- Tile 9 에서 top / bottom Wnd 의 크기
	BOOL        m_bGlow;
};

class CEmblemImageBox: public CTextBox
{
public:	
	CEmblemImageBox(){}
	CEmblemImageBox( SDesktop* DT );
	virtual ~CEmblemImageBox();
	
	virtual void RenderProc();//OR	
};

///---------------------------------------------------------------------------
///-- CImageBox
///---------------------------------------------------------------------------
class CNoImageBox : public CTextBox
{
public:
	CNoImageBox(){}
	CNoImageBox( SDesktop* DT );
	virtual ~CNoImageBox();
	
	virtual void RenderProc();//OR
	

public:
	//void SetBoxBgColor();
	void SetShowCount(int Count);
	void SetMaxCount(int Count );
	
protected:
	int m_nLHeight;
	int m_LineCount;
	int m_LimitCount;



};


//-----------------------------------------------------------------------------
#endif  __IMAGEBOX_H__