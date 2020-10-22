///////////////////////////////////////////////////////////////////////////////
///
///		File		: UpGauge.cpp
///		Desc		:
///
///		Author		: Park Soo-Hyun
///		Team		: Program - Client Team
///		Date		: 
///
///		Copyright(c) 2004, Rocksoft Co., Ltd. ALL RIGHTS RESERVED
///
///////////////////////////////////////////////////////////////////////////////
#include "UpGauge.h"

//-----------------------------------------------------------------------------
CUpGauge::CUpGauge( SDesktop* DT ) : CWnd( DT )
{
	Percent = 0;		//현재 %
	ProgressRate = 0;	//%에 따른 진행률
	BlankLR = 0;
	BlankTB	= 0;

	Blength = 0;			//Bar의 실제 길이
	Bimg = TID_CLIENT;			//Progress Bar
	Rbcolor[A] = 255;
	Rbcolor[R] = 255;
	Rbcolor[G] = 255;
	Rbcolor[B] = 255;
	Bcolor = D3DCOLOR_ARGB( Rbcolor[A], Rbcolor[R], Rbcolor[G], Rbcolor[B] );
}

//-----------------------------------------------------------------------------
CUpGauge::~CUpGauge()
{
}

//-----------------------------------------------------------------------------
void CUpGauge::RenderProc()
{
	UpdateHandler();
	
	Render->Draw( Cimg, Cx, Cy, Cw, Ch, Ccolor );

	//Bsize[1] = Cy + ( 100 - ProgressRate );
	//Render->DrawUpGauge( Bimg, Bsize[0], Bsize[1], Bsize[2], Bsize[3], Bcolor );
	switch( enPD )
	{
	case n_PD_DOWN:
		Bsize[3] = ProgressRate;
		Render->DrawUpGaugeDown( Bimg, Bsize[0], Bsize[1], Bsize[2], Bsize[3], Bcolor );
		break;
	case n_PD_UP:
		Bsize[1] = Cy + ( Blength - ProgressRate );
		Bsize[3] = ProgressRate;
		Render->DrawUpGaugeUp( Bimg, Bsize[0], Bsize[1], Bsize[2], Bsize[3], Bcolor, ( Blength - ProgressRate ) );
		break;
	}
	
}

//-----------------------------------------------------------------------------
IRESULT CUpGauge::ReceiveEvent( NEventMessage Event, EPARAM fParam, EPARAM sParam, EPARAM xParam, EPARAM exParam )
{
	int Iresult( 0 );
	Iresult = CWnd::ReceiveEvent( Event, fParam, sParam, xParam, exParam );

	switch( Event )
	{
	case n_emInitProgress:
		InitProgress( (int)fParam );
		break;
	case n_emSetPercent:
		SetPercent( (int)fParam );
		break;
	case n_emSetPercentAccum:
		SetPercentAccum( (int)fParam );
		break;
	case n_emGetPercentage:
		Iresult = GetPercentage();
		break;
	}

	return( Iresult );
}

SEventMessage* CUpGauge::EventProc( SEventMessage* EMsg )
{
	EMsg->FocusWnd = Iid;
	return EMsg;
}

//-----------------------------------------------------------------------------
void CUpGauge::SetAdjustPosition( int PosX, int PosY )
{
	Bsize[0]	= Cx;
    Bsize[1]	= Cy;
    Bsize[2]	= Cw; 
    Bsize[3]	= Ch;
	Blength		= Bsize[3];//진행 범위(컨트롤의 길이 c_size[2])

	CalculationRate( this->Percent );
}

//-----------------------------------------------------------------------------
void CUpGauge::SetBarImage( TEXID Bimg )
{
	this->Bimg = Bimg;
}

//-----------------------------------------------------------------------------
void CUpGauge::InitProgress( int Percent )
{
	if( Percent <= 0 ){	Percent = 0;	}
	if( Percent >= 100 ){	Percent = 100;	}
	this->Percent     = Percent;

	Bsize[0]	= Cx;
    Bsize[1]	= Cy;
    Bsize[2]	= Cw; 
    Bsize[3]	= Ch;
	Blength		= Bsize[3];//진행 범위(컨트롤의 길이 c_size[2])

    CalculationRate( this->Percent );
}

//-----------------------------------------------------------------------------
int CUpGauge::GetPercentage()
{
	return( Percent );
}

//-----------------------------------------------------------------------------
void CUpGauge::CalculationRate( int Percent )
{
	this->Percent = Percent;
	ProgressRate = ( Blength * this->Percent ) / 100;
}

//-----------------------------------------------------------------------------
void CUpGauge::SetPercent( int RateValue )
{
	if( RateValue <= 0 ){	RateValue = 0;	}
	if( RateValue >= 100 ){	RateValue = 100;	}
	Percent = RateValue;
	CalculationRate( Percent );
}

//-----------------------------------------------------------------------------
void CUpGauge::SetPercentAccum( int RateDelta )
{
	Percent += RateDelta;
	if( Percent <= 0 ){	Percent = 0;	}
	if( Percent >= 100 ){	Percent = 100;	}

	CalculationRate( Percent );
}

//-----------------------------------------------------------------------------
void CUpGauge::SetBarColor( UCHAR Red, UCHAR Green, UCHAR Blue )
{
	Rbcolor[R] = Red;
	Rbcolor[G] = Green;
	Rbcolor[B] = Blue;
	Bcolor = D3DCOLOR_ARGB( Rbcolor[A], Rbcolor[R], Rbcolor[G], Rbcolor[B] );
}

//-----------------------------------------------------------------------------
void CUpGauge::SetBarAlpha( UCHAR Alpha )
{
	Rbcolor[A] = Alpha;
	Bcolor = D3DCOLOR_ARGB( Rbcolor[A], Rbcolor[R], Rbcolor[G], Rbcolor[B] );
}

//-----------------------------------------------------------------------------
void CUpGauge::SetBarBlank( int BlankLR, int BlankTB )
{
	this->BlankLR = BlankLR;
	this->BlankTB = BlankTB;
}

//-----------------------------------------------------------------------------
void CUpGauge::SetProgressDirection( N_PROGRESS_DIRECTION	_pd )
{
	enPD = _pd;
}

//-----------------------------------------------------------------------------