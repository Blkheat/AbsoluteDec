//----------------------------------------------------------------------------------------------------------------------
// 파일명 : LoadingWndProc.h
// 용  도 : 로딩창
//----------------------------------------------------------------------------------------------------------------------
#ifndef __LOADINGBARWND_H__
#define __LOADINGBARWND_H__


#include "WndProc.h"


typedef struct s_VERTEX_LOAD_2D
{
    D3DXVECTOR4 pos;
    D3DCOLOR    color;
    FLOAT       tu0, tv0;

	s_VERTEX_LOAD_2D()
	{
		pos.z = 0.0f;
		pos.w = 1.0f;

		color = D3DCOLOR_ARGB( 255 , 255 , 255 , 255 );
	}

}VERTEX_LOAD_2D;

#define D3DFVF_VERTEXT_LOAD_2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1 )


class CTextBox;
class CProgressBar;
class CLoadingBarWnd : public CWndProc
{
public:

	bool					IsProgress;
	bool					IsChange;
	int						Delta;
	int						Percent;
	
	
	bool					m_IsMakeImg;
	
	SRpScAnswerWarp*		m_pMsgScAnswerWarp;
	SRpScFightRoomList*		m_pMsgScFightRoomList;
	
	bool					IsFirstLoading;			// 첫 로딩시 이미지 고정용

public:

	 CLoadingBarWnd()
	 {
		 Init();
	 }
	~CLoadingBarWnd(){}

	virtual void Init();
	virtual void Composition();
	virtual void Update();


	HRESULT InitDeviceObjects();// 페이드 아웃용 텍스처 생성(CGlow 참고)
	HRESULT InvalidateDeviceObjects();
	HRESULT RestoreDeviceObjects();
	
	void Open();
	void Close();

	HRESULT MakeScreenAfterImage();///-- 페이드 시키기전에 한번 배경이미지를 배경텍스쳐에 렌더링시킨다.
	void DrawScreenAfterImage();// 페이드 이미지 그린다	

	int Get_warpmap_id() const { return ml_warpmap_ID;  };
	void Set_warpmap_id( const int al_map_id ){ ml_warpmap_ID = al_map_id ; };
	void SetPercent(int nPercent);

	CTextBox * GetHelpTex()	{ return m_pHelpTex; }

private:

	CProgressBar*			LoadingBar;
	CTextBox *				m_pHelpTex;	
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DSURFACE9		m_pBackBuffer;// 백버퍼
	LPDIRECT3DSURFACE9		m_pBackSurface;// 백버퍼 복사용 표면
	LPDIRECT3DTEXTURE9		m_pBackTexture;// 백버퍼 복사용 텍스처

	LPD3DXRENDERTOSURFACE	m_pRenderToSurface;// 복사 렌더링 표면
	LPDIRECT3DTEXTURE9		m_pTex;// 복사 텍스처
	LPDIRECT3DSURFACE9		m_pSurface;// 복사 표면

	LPDIRECT3DVERTEXBUFFER9	m_pVB;// 복사 버텍스
	LPDIRECT3DINDEXBUFFER9	m_pIB;

	LPDIRECT3DVERTEXBUFFER9	m_pBackVB;// 백버퍼 버텍스
#else
	LPDIRECT3DSURFACE8		m_pBackBuffer;// 백버퍼
	LPDIRECT3DSURFACE8		m_pBackSurface;// 백버퍼 복사용 표면
	LPDIRECT3DTEXTURE8		m_pBackTexture;// 백버퍼 복사용 텍스처

	LPD3DXRENDERTOSURFACE	m_pRenderToSurface;// 복사 렌더링 표면
	LPDIRECT3DTEXTURE8		m_pTex;// 복사 텍스처
	LPDIRECT3DSURFACE8		m_pSurface;// 복사 표면

	LPDIRECT3DVERTEXBUFFER8	m_pVB;// 복사 버텍스
	LPDIRECT3DINDEXBUFFER8	m_pIB;

	LPDIRECT3DVERTEXBUFFER8	m_pBackVB;// 백버퍼 버텍스
#endif // DIRECT_VERSION_9_MJH
	int ml_warpmap_ID;

private:
	
	void DrawTexture( float x, float y, float w, float h, 
					  float tx, float ty, float tw, float th,
#ifdef DIRECT_VERSION_9_MJH
					  D3DCOLOR color , LPDIRECT3DDEVICE9 pDevice );
#else
					  D3DCOLOR color , LPDIRECT3DDEVICE8 pDevice );
#endif // DIRECT_VERSION_9_MJH

};

extern	CLoadingBarWnd	LoadingBarWnd;

#endif	__LOADINGBARWND_H__