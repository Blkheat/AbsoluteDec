///////////////////////////////////////////////////////////////////////////////
///
///		File		: FrameWnd.h
///		Desc		:
///
///		Author		: Park Soo-Hyun
///		Team		: Program - Client Team
///		Date		: 
///
///		Copyright(c) 2004, Rocksoft Co., Ltd. ALL RIGHTS RESERVED
///
///////////////////////////////////////////////////////////////////////////////
#ifndef __FRAMEWND_H__
#define __FRAMEWND_H__
//-----------------------------------------------------------------------------
#include "Chain.h"
#include "Container.h"
#include "WndProc.h"

#include "Wnd.h"
#include "TextBox.h"
#include "Button.h"
#include "ImageBox.h"

enum enFrameWndFocusPos
{
	fwfNoneFocus,
	fwfClientFocus,
	fwfTitleBar,
	fwfCloseButton,
	fwfNorthFrm,
	fwfNorthEastFrm,
	fwfEastFrm,
	fwfSouthEastFrm,
	fwfSouthFrm,
	fwfSouthWestFrm,
	fwfWestFrm,
	fwfNorthWestFrm,
	fwfChildWndFocus
};
enum enFrameWndTextureStyle
{
	fwfTex_Style_Normal,
	fwfTex_Style_Tile3,
	fwfTex_Style_Tile9
};
enum enFrameWndAlphaOrder
{
	fwfWnd_Alpha_None = 0,
	fwfWnd_Alpha_DO_Normal,
	fwfWnd_Alpha_DO_Alpha
};

///----------------------------------------------------------------------------
///-- enFrameWndTextureStyle
///----------------------------------------------------------------------------
///-- ������� �������� �ʴ� Texture �� ��쿡 Tiling ����̴�.
///-- ������ Texture �� ����� ���� Size �� Control Image �� ����� ����.

/*������ Texture �� ������ �����̴�.

	-- fwfTex_Style_Tile3

    ��������������������������
    ��      ��      ��      ��
	��      ��      ��      ��
    ��   1  ��   2  ��   3  ��     2 �� : ���� Size
	��      ��      ��      ��
    ��      ��      ��      �� 
    ��������������������������




	-- fwfTex_Style_Tile9

    ��������������������������
    ��      ��      ��      �� 
    ��   1  ��   2  ��   3  ��   
    ��      ��      ��      �� 
    ��������������������������
    ��      ��      ��      �� 
    ��   4  ��   5  ��   6  ��
    ��      ��      ��      ��
    �������������������������� 
    ��      ��      ��      �� 
    ��   7  ��   8  ��   9  �� 
    ��      ��      ��      �� 
    ��������������������������

  

*/
class CFrameWnd : public CWnd
{
public:
	CFrameWnd();
	CFrameWnd( SDesktop* pDT );
	virtual ~CFrameWnd();

	//-------------------------------------------------------------------------
	virtual void CreateWnd( NWindowType WType, WNDID Iid, WNDID Pid, float PosX, float PosY, int Width, int Height, bool isRatio );
		
	virtual SEventMessage* EventProc( SEventMessage* EMsg );
	virtual void RenderProc();

	virtual IRESULT ReceiveEvent( NEventMessage Event, EPARAM fParam, EPARAM sParam, EPARAM xParam, EPARAM exParam );
	virtual WNDID FocusCheck( int Mx, int My, bool IsChildCheck );

	virtual void ResetSize( SRect* size );
	//-------------------------------------------------------------------------
	
	virtual void RegisterChild( WNDID Cid );
	virtual void SetVisible( bool Visible );

	void Initialize();
	void SetCaption( void* Caption );
	//UNICODE_ROCK 
	
	void SetTitle( bool bTitle , const int al_margin_y = 2, const int al_margin_x = 40 );	
	void SetResize( bool bResize );
	void SetMove( bool bMove );	
	void SetWndProc( void (*WndProc)( SEventMessage* EMsg ) );
	void SetWndProc( CWndProc* Wnd );
	void SetWndTexStyle( enFrameWndTextureStyle ae_wndtex_style );		///-- Wnd Texture Style �� ����
	void SetWndTileSizeLR( const int al_left, const int al_right );		///-- Tile3 �� ����� LR Tile ũ������
	void SetWndTileSizeTB( const int al_top, const int al_bottom );		///-- Tile9 �� ����� TB Tile ũ������
	void SetTitleBack( const bool ab_titleback );						///-- Title Text �� ���
	void SetIconTID( const TEXID ae_tid );


	void Reposition( int x, int y );

	int				NumEdit;
	int				CurrentEditNum;
	WNDID			EditList[10];//���� ����
	
	void (*WndProc)( SEventMessage* EMsg ); //WndProc -> CallBack Function
	CWndProc*		theWnd;


	CTextBox*	GetTitleBar() { return TitleText; }
	bool		GetIsTitle() { return isTitle; }


	void		SetFocusWndColor( bool aIsFocus );

protected:
	CChain		Chain;
	
	CTextBox*	TitleText;	
	CImageBox*	TitleBack;	
	TEXID		IconTID;
	
	WNDID		FocusChildWid;
	CWnd*		FocusChildWnd;

	enFrameWndFocusPos				FocusPos;	
	enFrameWndAlphaOrder			me_do_alpha;	///-- Window �� child ���� Alpha ��ħ
	enFrameWndTextureStyle			me_WndStytle;	///-- Window �� Texture style
	signed long int	ml_Wnd_width[2];				///-- Tile 3 ���� Left / Right Wnd �� ũ��
	signed long int	ml_Wnd_height[2];				///-- Tile 9 ���� top / bottom Wnd �� ũ��
	signed long int	ml_margin_x;					///-- Title Text �� Margin
	signed long int	ml_margin_y;		
	bool			mb_title_back;					///-- Title Text �� ���

	ISIZE			Wsize;				///-- ������ ������, ĸ�ǹٿ� ������ ����....
	signed long int	maal_tile[9][4];	///-- Split Wnd �� �� ������
	


	TCHAR			theCaptionStr[64];
	int				DeltaX;//���콺 �̵��� ��� ��ǥ
	int				DeltaY;

	bool			isTitle;//Ÿ��Ʋ TEXT ��� ����
	bool			isCloseBtn;	
	bool			isVisibleEditFocus;
	
	
	bool			isResize;//������ ���� ��� ����
	//bool			isDrag;
	bool			isMove;
	
	//by simwoosung
	CFrameWnd *		pMoveLinkFrame;
	
	//by dongs
	CFrameWnd *		pMoveLinkFrame2;


	virtual void NotifyEvent();
	//virtual WNDID RegionFocusCheck( int Lx, int Ly );
	virtual WNDID ChildFocusCheck( int Mx, int My );
	virtual void RenderWnd();

	void VisibleFrameWnd();
	void CloseFrameWnd();
	void EditSpaceAction( WNDID Eid );
	void EditReturnAction( WNDID Eid );
	void EditTabAction( WNDID Eid );
	void EditImmConvStatus( WNDID Eid, bool isAlphaNumeric );


	//-------------------------------------------------------------------------
	virtual void SetAdjustPosition( int PosX, int PosY );//OR	
	virtual void UpdateHandler();
			
	virtual void OnMouseFocus( int Mx, int My );
	
	virtual void OnMouseLButtonDown( int Mx, int My );
	virtual void OnMouseLButtonPress( int Mx, int My );
	virtual void OnMouseLButtonUp( int Mx, int My );
	virtual void OnMouseLDrag( int Mx, int My );
	virtual void OnMouseLDrop( int Mx, int My );
	//-------------------------------------------------------------------------
	
public:
	//by simwoosung
	void SetAbsolutePosition( int PosX, int PosY );
	void SetMoveLinkFrameWnd(CFrameWnd *pFrameWnd);
	CFrameWnd * GetMoveLinkFrameWnd();
	
	//by dongs ���߿� ������ 3������ ���̸� .. �׶����� ����� ���� ;; ����� 2���ۿ� .. �����ȵ� 
	void SetMoveLinkFrameWnd2(CFrameWnd *pFrameWnd);
	CFrameWnd * GetMoveLinkFrameWnd2();
	

	CChain* GetChain() { return &Chain; }
	bool GetIsMove()  { return isMove; }
	void InDirectOnMouseLDrag( int Mx, int My );
	void SetFrameWndFocusPos(enFrameWndFocusPos	tFocusPos);
};

//-----------------------------------------------------------------------------
#endif  __FRAMEWND_H__