///////////////////////////////////////////////////////////////////////////////
///
///		File		: ProgressBar.h
///		Desc		:
///
///		Author		: Park Soo-Hyun
///		Team		: Program - Client Team
///		Date		: 
///
///		Copyright(c) 2004, Rocksoft Co., Ltd. ALL RIGHTS RESERVED
///
///////////////////////////////////////////////////////////////////////////////
#ifndef __PROGRESSBAR_H__
#define __PROGRESSBAR_H__
//-----------------------------------------------------------------------------
#include "Wnd.h"

class CProgressBar : public CWnd
{
public:
	CProgressBar(){}
	CProgressBar( SDesktop* DT );
	virtual ~CProgressBar();

	//-------------------------------------------------------------------------
	virtual void RenderProc();//OR
	virtual IRESULT ReceiveEvent( NEventMessage Event, EPARAM fParam, EPARAM sParam, EPARAM xParam, EPARAM exParam );//OR
	
	virtual void SetAdjustPosition( int PosX, int PosY );
	//-------------------------------------------------------------------------

	void InitProgress( int Percent );
	
	void SetBarImage( TEXID Bimg );
	void SetBarColor( UCHAR Red, UCHAR Green, UCHAR Blue );
	void SetBarAlpha( UCHAR Alpha );
	void SetBarBlank( int BlankLR, int BlankTB );

	void SetPercent( int RateValue );
	void SetPercentAccum( int RateDelta );
	int GetPercentage();		//진행률에 따른 % 얻어오기
	
	void SetDirection(int Dir);




protected:
	int			Percent;		//현재 %
	int			ProgressRate;	//%에 따른 진행률
	int			ProgressRateInv;
	int			ProgressRateH;
	int			ProgressRateHInv;

	int			BlankLR;		//Client와 Bar의 크기가 틀리때....
	int			BlankTB;		

	int			Blength;		//Bar의 실제 길이
	int			BlengthH;		//Bar의 실제 길이
	ISIZE		Bsize;			//
	TEXID		Bimg;			//Progress Bar Texture
	D3DCOLOR	Bcolor;			//Bar Color
	ICOLOR4		Rbcolor;		//Ref Bar Color.... Max Alpha는 255로 고정

	int				Direction;	// 0 : defult 1 : Left diagonal 

	
	void CalculationRate( int Percent );	//%에 따라 진행률 계산
};

//-----------------------------------------------------------------------------
#endif  __PROGRESSBAR_H__