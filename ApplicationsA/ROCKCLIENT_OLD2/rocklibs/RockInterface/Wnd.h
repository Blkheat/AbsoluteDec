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

	IPOS2			Apos; //���������� ���� ����� ���� ��ġ(absolute)
							//ComboBox���� access protected member declared in class 'RUI_Wnd' �����޼���....
							//�ϴ� ���⼭ ó��....-.-;

#ifdef HHM_USER_RESOLUTION_SELECT// �ػ� ����(2009/04/22)
	
	// ���� ������ ������ 1024 * 768���� Ȯ��� ������
	float			m_fExtentW;
	float			m_fExtentH;
	unsigned char	m_ucCoordStd;// ��ǥ ��ġ ����(0 : ��üȭ��, 1 : ȭ���߾�, 2 : LeftTop)
	
	void SetExtentWH(float fW, float fH);
	
#endif // #ifdef HHM_USER_RESOLUTION_SELECT// �ػ� ����(2009/04/22)

protected:
	SDesktop*		pDT;
	
	NWindowType		WType;//������ Ÿ��
	WNDID			Iid;//�ڽ� ID
	WNDID			Pid;//�θ� ID 
	CWnd*			Pwnd;

	IPOS2			Rpos; //�θ������쿡 ���� ��� ��ġ(relative)
	
	ISIZE			Csize; //Ŭ���̾�Ʈ ������
	

	NDepthOrder		Zorder;
	
	D3DCOLOR		Ccolor;//Client Color
	ICOLOR4			Rcolor;//Ref Color
	UCHAR			MaxAlpha;

	TEXID			Cimg;  //�̹���
	enClientStyle	Cstyle;
	enClientBorder	Cborder;

	HLPID			Hid;

	int				Cx;
	int				Cy;
	int				Cw;
	int				Ch;	
	
	bool			isVisible;//���� ����
	bool			isFocusCheck;//��Ŀ�� üũ ����
	bool			isRegion;//��� ����( Hit Test�� )
	bool			isInit;
	bool			isFitImg;
	
	//by simwoosung
	long			NowTime;
	IEVENT			PrevNotify;
	bool			isEnable;
	TEXID			CExtraimg;  //�߰��̹���
	TEXID			CBgImage;	//��׶��� �̹���
	
	

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
