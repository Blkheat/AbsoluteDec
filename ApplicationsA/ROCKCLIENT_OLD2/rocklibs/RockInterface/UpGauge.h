///////////////////////////////////////////////////////////////////////////////
///
///		File		: UpGauge.h
///		Desc		:
///
///		Author		: Park Soo-Hyun
///		Team		: Program - Client Team
///		Date		: 
///
///		Copyright(c) 2004, Rocksoft Co., Ltd. ALL RIGHTS RESERVED
///
///////////////////////////////////////////////////////////////////////////////
#ifndef __UPGAUGE_H__
#define __UPGAUGE_H__
//-----------------------------------------------------------------------------
#include "Wnd.h"

class CUpGauge : public CWnd
{
public:
	CUpGauge(){}
	CUpGauge( SDesktop* DT );
	virtual ~CUpGauge();

	//-------------------------------------------------------------------------
	virtual void RenderProc();//OR
	virtual IRESULT ReceiveEvent( NEventMessage Event, EPARAM fParam, EPARAM sParam, EPARAM xParam, EPARAM exParam );//OR
	
	virtual void SetAdjustPosition( int PosX, int PosY );
	virtual SEventMessage* EventProc( SEventMessage* EMsg );
	//-------------------------------------------------------------------------

	void InitProgress( int Percent );
	
	void SetBarImage( TEXID Bimg );
	void SetBarColor( UCHAR Red, UCHAR Green, UCHAR Blue );
	void SetBarAlpha( UCHAR Alpha );
	void SetBarBlank( int BlankLR, int BlankTB );
	void SetProgressDirection( N_PROGRESS_DIRECTION	_en_pd );

	void SetPercent( int RateValue );
	void SetPercentAccum( int RateDelta );
	int GetPercentage();		//진행률에 따른 % 얻어오기

protected:
	int			Percent;		//현재 %
	int			ProgressRate;	//%에 따른 진행률
	int			BlankLR;		//Client와 Bar의 크기가 틀리때....
	int			BlankTB;		

	int			Blength;		//Bar의 실제 길이
	ISIZE		Bsize;			//
	TEXID		Bimg;			//Progress Bar Texture
	D3DCOLOR	Bcolor;			//Bar Color
	ICOLOR4		Rbcolor;		//Ref Bar Color.... Max Alpha는 255로 고정

	N_PROGRESS_DIRECTION	enPD;

	void CalculationRate( int Percent );	//%에 따라 진행률 계산
};

//-----------------------------------------------------------------------------
#endif  __UPGAUGE_H__