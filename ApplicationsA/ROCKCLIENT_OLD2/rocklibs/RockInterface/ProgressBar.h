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
	int GetPercentage();		//������� ���� % ������
	
	void SetDirection(int Dir);




protected:
	int			Percent;		//���� %
	int			ProgressRate;	//%�� ���� �����
	int			ProgressRateInv;
	int			ProgressRateH;
	int			ProgressRateHInv;

	int			BlankLR;		//Client�� Bar�� ũ�Ⱑ Ʋ����....
	int			BlankTB;		

	int			Blength;		//Bar�� ���� ����
	int			BlengthH;		//Bar�� ���� ����
	ISIZE		Bsize;			//
	TEXID		Bimg;			//Progress Bar Texture
	D3DCOLOR	Bcolor;			//Bar Color
	ICOLOR4		Rbcolor;		//Ref Bar Color.... Max Alpha�� 255�� ����

	int				Direction;	// 0 : defult 1 : Left diagonal 

	
	void CalculationRate( int Percent );	//%�� ���� ����� ���
};

//-----------------------------------------------------------------------------
#endif  __PROGRESSBAR_H__