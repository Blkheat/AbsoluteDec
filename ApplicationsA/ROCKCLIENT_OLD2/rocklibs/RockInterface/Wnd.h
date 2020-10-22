///////////////////////////////////////////////////////////////////////////////
///
///		File		: Wnd.h
///		Desc		:
///
///		Author		: Park Soo-Hyun
///		Team		: Program - Client Team
///		Date		: 
///
///		Copyright(c) 2004, Rocksoft Co., Ltd. ALL RIGHTS RESERVED
///
///////////////////////////////////////////////////////////////////////////////
#ifndef __WND_H__
#define __WND_H__
//-----------------------------------------------------------------------------
#include "..\\RockPCH.h"
#ifdef DIRECT_VERSION_9_MJH
#include <d3dx9.h>
#else
#include <d3dx8.h>
#endif // DIRECT_VERSION_9_MJH

#include "Define.h"

#include "Fontman.h"
#include "Render.h"



enum enClientStyle
{
	csNone, csPlane, csImage
};

enum enClientBorder
{
	cbNone, cbLine
};


class CWnd
{
public:
	CWnd() {}
	CWnd( SDesktop* DT );
	virtual ~CWnd();

	//-------------------------------------------------------------------------
	virtual void CreateWnd( NWindowType WType, WNDID Iid, WNDID Pid, float PosX, float PosY, int Width, int Height, bool isRatio );

	virtual SEventMessage* EventProc( SEventMessage* EMsg );	
	virtual void RenderProc();

	virtual IRESULT SendEvent( WNDID Wid, NEventMessage Event, EPARAM fParam, EPARAM sParam, EPARAM xParam, EPARAM exParam );
	virtual IRESULT ReceiveEvent( NEventMessage Event, EPARAM fParam, EPARAM sParam, EPARAM xParam, EPARAM exParam );
	virtual WNDID FocusCheck( int Mx, int My, bool IsChildCheck );
	//-------------------------------------------------------------------------


	//-------------------------------------------------------------------------
	virtual NWindowType GetWindowType();
	virtual void SetID( WNDID Iid );
	virtual WNDID GetID();
	virtual WNDID GetParentID();
	virtual CWnd* GetParentHandle();

	virtual void SetHID( HLPID Hid );
	virtual HLPID GetHID();
		
	virtual void GetClientSize( SRect* Csize );
	virtual void SetClientSize( int Width, int Height );
	virtual void ResetSize( SRect* size );

	virtual TEXID GetClientImage();
	virtual void SetClientImage( TEXID Cimg );

	virtual enClientStyle GetClientStyle();
	virtual enClientBorder GetClientBorder();
	virtual void GetClientStyle( enClientStyle Cstyle );
	virtual void GetClientBorder( enClientBorder Cborder );
	
	//by simwoosung
	virtual void SetWndRepositionR( int PosX, int PosY );
	virtual void SetClientBgImage( TEXID Cimg );
	virtual void SetClientExtraImage( TEXID Cimg );
	
	virtual void Reposition( int x, int y );
	virtual void SetWndPositionR( int PosX, int PosY );
	virtual void SetWndPositionA( int PosX, int PosY );
	virtual void SetAdjustPosition( int PosX, int PosY ); //Regulation or Adjustment
	
	virtual void GetWndPosition( IPOS2 Pos );
	virtual void GetScenePos( IPOS2 Pos );
	
	virtual bool IsVisible();
	virtual bool IsFocusCheck();
	virtual bool IsRegion();
	virtual void SetVisible( bool Visible );
	virtual void SetFocusCheck( bool FocusCheck );
	virtual void SetRegion( bool Region );	
	virtual void SetFitImg( bool FitImg );	
	
	
		
	//by simwoosung
	virtual void SetEnable( bool Enable );
	virtual bool GetEnable();

	virtual NDepthOrder GetZorder();
	virtual void SetZorder( NDepthOrder Zorder );

	virtual void InitMaxAlpha( UCHAR Alpha );
	virtual void SetClientAlpah( UCHAR Alpha );
	virtual UCHAR GetClientAlpah();
	virtual void SetClientColor( UCHAR Red, UCHAR Green, UCHAR Blue );
	//-------------------------------------------------------------------------

	IPOS2			Apos; //계층구조에 따라 계산한 절대 위치(absolute)
							//ComboBox에서 access protected member declared in class 'RUI_Wnd' 에러메세지....
							//일단 여기서 처리....-.-;

#ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/22)
	
	// 기준 윈도우 사이즈 1024 * 768에서 확장된 사이즈
	float			m_fExtentW;
	float			m_fExtentH;
	unsigned char	m_ucCoordStd;// 좌표 위치 기준(0 : 전체화면, 1 : 화면중앙, 2 : LeftTop)
	
	void SetExtentWH(float fW, float fH);
	
#endif // #ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/22)

protected:
	SDesktop*		pDT;
	
	NWindowType		WType;//윈도우 타입
	WNDID			Iid;//자신 ID
	WNDID			Pid;//부모 ID 
	CWnd*			Pwnd;

	IPOS2			Rpos; //부모윈도우에 대한 상대 위치(relative)
	
	ISIZE			Csize; //클라이언트 사이즈
	

	NDepthOrder		Zorder;
	
	D3DCOLOR		Ccolor;//Client Color
	ICOLOR4			Rcolor;//Ref Color
	UCHAR			MaxAlpha;

	TEXID			Cimg;  //이미지
	enClientStyle	Cstyle;
	enClientBorder	Cborder;

	HLPID			Hid;

	int				Cx;
	int				Cy;
	int				Cw;
	int				Ch;	
	
	bool			isVisible;//가시 여부
	bool			isFocusCheck;//포커스 체크 여부
	bool			isRegion;//모양 여부( Hit Test용 )
	bool			isInit;
	bool			isFitImg;
	
	//by simwoosung
	long			NowTime;
	IEVENT			PrevNotify;
	bool			isEnable;
	TEXID			CExtraimg;  //추가이미지
	TEXID			CBgImage;	//백그라운드 이미지
	
	

	WNDID GetWndID();

	virtual WNDID RegionFocusCheck( int Lx, int Ly );

	virtual void SetGroupWnd( int Count, WNDID* WndList );
	virtual void SetSelectedWnd();
	virtual void SetRatio( POINT* ratio, float x, float y );
	
	virtual void UpdateHandler();
		
	//-------------------------------------------------------------------------
	virtual void OnMouseFocus( int Mx, int My );
	
	virtual void OnMouseLButtonDown( int Mx, int My );
	virtual void OnMouseLButtonPress( int Mx, int My );
	virtual void OnMouseLButtonUp( int Mx, int My );
	
	virtual void OnMouseRButtonDown( int Mx, int My );
	virtual void OnMouseRButtonPress( int Mx, int My );
	virtual void OnMouseRButtonUp( int Mx, int My );

	virtual void OnMouseLDrag( int Mx, int My );
	virtual void OnMouseLDrop( int Mx, int My );

	virtual void OnMouseRDrag( int Mx, int My );
	virtual void OnMouseRDrop( int Mx, int My );
	
	virtual	void OnMouseLDblClick( int Mx, int My );
	virtual	void OnMouseRDblClick( int Mx, int My );
	//-------------------------------------------------------------------------
};

//-----------------------------------------------------------------------------
#endif  __WND_H__
