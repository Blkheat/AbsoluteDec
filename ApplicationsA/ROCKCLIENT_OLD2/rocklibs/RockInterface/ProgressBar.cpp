///////////////////////////////////////////////////////////////////////////////
///
///		File		: ProgressBar.cpp
///		Desc		:
///
///		Author		: Park Soo-Hyun
///		Team		: Program - Client Team
///		Date		: 
///
///		Copyright(c) 2004, Rocksoft Co., Ltd. ALL RIGHTS RESERVED
///
///////////////////////////////////////////////////////////////////////////////
#include "ProgressBar.h"

//-----------------------------------------------------------------------------
CProgressBar::CProgressBar( SDesktop* DT ) : CWnd( DT )
{
	Percent = 0;		//현재 %
	ProgressRate = 0;	//%에 따른 진행률
	ProgressRateInv = 0 ;
	ProgressRateH = 0 ; 
	ProgressRateHInv= 0 ; 


	BlankLR = 0;
	BlankTB	= 0;

	Direction = 0 ;		// 1은 왼쪽 대각선으로 택스쳐가 이동 한다 .. 

	Blength = 0;			//Bar의 실제 길이
	BlengthH = 0;

	Bimg = TID_CLIENT;			//Progress Bar
	Rbcolor[A] = 255;
	Rbcolor[R] = 255;
	Rbcolor[G] = 255;
	Rbcolor[B] = 255;
	Bcolor = D3DCOLOR_ARGB( Rbcolor[A], Rbcolor[R], Rbcolor[G], Rbcolor[B] );
}

//-----------------------------------------------------------------------------
CProgressBar::~CProgressBar()
{
}

//-----------------------------------------------------------------------------
void CProgressBar::RenderProc()
{
	UpdateHandler();
	

	if( isFitImg )
	{
		Render->DrawFitImg( Cimg, Cx, Cy, Cw, Ch, Ccolor );
	}
	else
	{
		Render->Draw( Cimg, Cx, Cy, Cw, Ch, Ccolor );
	}
	//프로그래스 바의 방향이다 디폴트는 왼쪽에서 걍 오른쪽 
	// 1번은 왼쪽 대각선 방향으로 올라가는것이다 .. 

	if(Direction == 1)
	{

		Bsize[0] = Cx + ProgressRateInv;
		Bsize[2] = ProgressRate;
		
		Bsize[1] = Cy + ProgressRateHInv;
		Bsize[3] = ProgressRateH;
		

		Render->DrawAllDir( Bimg, Bsize[0], Bsize[1], Bsize[2], Bsize[3], Bcolor );
	}
	else if(Direction == 2)
	{

		Bsize[0] = Cx + ProgressRateInv;
		Bsize[2] = ProgressRate;
		
		Bsize[1] = Cy + ProgressRateHInv;
		Bsize[3] = ProgressRateH;
		
		Render->Draw( Bimg, Bsize[0], Bsize[1], Bsize[2], Bsize[3], Bcolor );
	}
	else 
	{
		Bsize[2] = ProgressRate;
		Render->Draw( Bimg, Bsize[0], Bsize[1], Bsize[2], Bsize[3], Bcolor );

	}


	return;
}

//-----------------------------------------------------------------------------
IRESULT CProgressBar::ReceiveEvent( NEventMessage Event, EPARAM fParam, EPARAM sParam, EPARAM xParam, EPARAM exParam )
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

//-----------------------------------------------------------------------------
void CProgressBar::SetAdjustPosition( int PosX, int PosY )
{
	Bsize[0]	= Cx + BlankLR;
    Bsize[1]	= Cy + BlankTB;
    Bsize[2]	= Cw - ( BlankLR * 2 ); 
    Bsize[3]	= Ch - ( BlankTB * 2 );
	Blength		= Bsize[2];//진행 범위(컨트롤의 길이 c_size[2])
	BlengthH		= Bsize[3];

	CalculationRate( this->Percent );
}

//-----------------------------------------------------------------------------
void CProgressBar::SetBarImage( TEXID Bimg )
{
	this->Bimg = Bimg;
}

//-----------------------------------------------------------------------------
void CProgressBar::InitProgress( int Percent )
{
	if( Percent <= 0 ){	Percent = 0;	}
	if( Percent >= 100 ){	Percent = 100;	}
	this->Percent     = Percent;

	Bsize[0]	= Cx + BlankLR;
    Bsize[1]	= Cy + BlankTB;
    Bsize[2]	= Cw - ( BlankLR * 2 ); 
    Bsize[3]	= Ch - ( BlankTB * 2 );
	Blength		= Bsize[2];//진행 범위(컨트롤의 길이 c_size[2])
	BlengthH		= Bsize[3];

    CalculationRate( this->Percent );
}

//-----------------------------------------------------------------------------
int CProgressBar::GetPercentage()
{
	return( Percent );
}

//-----------------------------------------------------------------------------
void CProgressBar::CalculationRate( int Percent )
{
	this->Percent = Percent;
	ProgressRate = ( Blength * this->Percent ) / 100;
	ProgressRateInv = (Blength - ProgressRate);

	ProgressRateH	= (BlengthH * this->Percent) /100;
	ProgressRateHInv = (BlengthH - ProgressRateH);


}

//-----------------------------------------------------------------------------
void CProgressBar::SetPercent( int RateValue )
{
	if( RateValue <= 0 ){	RateValue = 0;	}
	if( RateValue >= 100 ){	RateValue = 100;	}
	Percent = RateValue;
	CalculationRate( Percent );
}

//-----------------------------------------------------------------------------
void CProgressBar::SetPercentAccum( int RateDelta )
{
	Percent += RateDelta;
	if( Percent <= 0 ){	Percent = 0;	}
	if( Percent >= 100 ){	Percent = 100;	}

	CalculationRate( Percent );
}

//-----------------------------------------------------------------------------
void CProgressBar::SetBarColor( UCHAR Red, UCHAR Green, UCHAR Blue )
{
	Rbcolor[R] = Red;
	Rbcolor[G] = Green;
	Rbcolor[B] = Blue;
	Bcolor = D3DCOLOR_ARGB( Rbcolor[A], Rbcolor[R], Rbcolor[G], Rbcolor[B] );
}

//-----------------------------------------------------------------------------
void CProgressBar::SetBarAlpha( UCHAR Alpha )
{
	Rbcolor[A] = Alpha;
	Bcolor = D3DCOLOR_ARGB( Rbcolor[A], Rbcolor[R], Rbcolor[G], Rbcolor[B] );
}

//-----------------------------------------------------------------------------
void CProgressBar::SetBarBlank( int BlankLR, int BlankTB )
{
	this->BlankLR = BlankLR;
	this->BlankTB = BlankTB;
}

void CProgressBar::SetDirection(int Dir)
{
	Direction = Dir;
}


//-----------------------------------------------------------------------------