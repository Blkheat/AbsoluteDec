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
	int GetPercentage();		//������� ���� % ������

protected:
	int			Percent;		//���� %
	int			ProgressRate;	//%�� ���� �����
	int			BlankLR;		//Client�� Bar�� ũ�Ⱑ Ʋ����....
	int			BlankTB;		

	int			Blength;		//Bar�� ���� ����
	ISIZE		Bsize;			//
	TEXID		Bimg;			//Progress Bar Texture
	D3DCOLOR	Bcolor;			//Bar Color
	ICOLOR4		Rbcolor;		//Ref Bar Color.... Max Alpha�� 255�� ����

	N_PROGRESS_DIRECTION	enPD;

	void CalculationRate( int Percent );	//%�� ���� ����� ���
};

//-----------------------------------------------------------------------------
#endif  __UPGAUGE_H__