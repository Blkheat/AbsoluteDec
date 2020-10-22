///////////////////////////////////////////////////////////////////////////////
///
///		File		: Button.h
///		Desc		:
///
///		Author		: Park Soo-Hyun
///		Team		: Program - Client Team
///		Date		: 
///
///		Copyright(c) 2004, Rocksoft Co., Ltd. ALL RIGHTS RESERVED
///
///////////////////////////////////////////////////////////////////////////////
#ifndef __BUTTON_H__
#define __BUTTON_H__
//-----------------------------------------------------------------------------
#include "TextBox.h"



class CButton : public CTextBox
{
public:
	CButton(){}
	CButton( SDesktop* DT );
	virtual ~CButton();

	//-------------------------------------------------------------------------
	virtual SEventMessage* EventProc( SEventMessage* EMsg );//OR
	virtual void RenderProc();//OR
	virtual IRESULT ReceiveEvent( NEventMessage Event, EPARAM fParam, EPARAM sParam, EPARAM xParam, EPARAM exParam );//OR

	virtual WNDID FocusCheck( int Mx, int My, bool IsChildCheck );
	//-------------------------------------------------------------------------
	

	void SetBtnImage( TEXID Normal, TEXID Focus, TEXID Select, TEXID Hold );
	///void SetBtnColor( UCHAR Red, UCHAR Green, UCHAR Blue );
	///void SetBtnAlpha( UCHAR Alpha );
	void SetBtnHold( bool IsHold );
	void SetBtnState( NButtonState aState );
	void SetBtnPosWnd( int addBx, int addBy );
	void SetImgBtn( bool aIsImgBtn );

	NButtonState GetBtnState() { return BtnState; }
	//by simwoosung
	void		 SetClickTxt(bool bFlag)  { IsClickTxt = bFlag; }
	virtual		 void SetEnable( bool Enable );
	virtual		 void ResetSize( SRect* size );

protected:
	
	bool			IsImgBtn;
	bool			IsHold;
	int				ReactionDelta;

	NButtonState	BtnState;
	TEXID			BtnImg[4];//�⺻, ��Ŀ��, ����, Ȧ��

	int				Bx;
	int				By;
	int				Bw;
	int				Bh;

	///D3DCOLOR		BtnColor;
	///ICOLOR4			RBtnColor;//Ref Button Color.... Max Alpha�� 255�� ����


	int				Refx;
	int				Refy;

	//by simwoosung
	bool			IsClickTxt;			//���� ������ȿ��

	virtual void SetAdjustPosition( int PosX, int PosY );//OR
	virtual void UpdateHandler();//OR

	//-------------------------------------------------------------------------
	virtual void OnMouseFocus( int Mx, int My );//OR
	virtual void OnMouseLButtonDown( int Mx, int My );//OR
	virtual void OnMouseLButtonPress( int Mx, int My );//OR
	virtual void OnMouseLButtonUp( int Mx, int My );//OR
	//-------------------------------------------------------------------------
};

///---------------------------------------------------------------------------
///-- CImageBox9
///---------------------------------------------------------------------------
class CButton9: public CButton
{
	public:
	CButton9(){}
	CButton9( SDesktop* DT );
	virtual ~CButton9();
	
	virtual void RenderProc();//OR

	void SetAdjustPosition( int PosX, int PosY );
	void SetWndTileSizeLR( const int al_left, const int al_right );		///-- Tile3 �� ����� LR Tile ũ������
	void SetWndTileSizeTB( const int al_top, const int al_bottom );		///-- Tile9 �� ����� TB Tile ũ������

protected:
	signed long int	maal_tile[9][4];				///-- Split Wnd �� �� ������
	signed long int	ml_Wnd_width[2];				///-- Tile 3 ���� Left / Right Wnd �� ũ��
	signed long int	ml_Wnd_height[2];				///-- Tile 9 ���� top / bottom Wnd �� ũ��
};

//-----------------------------------------------------------------------------
#endif  __BUTTON_H__
