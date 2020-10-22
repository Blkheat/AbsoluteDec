//----------------------------------------------------------------------------------------------------------------------
// ���ϸ� : LoadingWndProc.h
// ��  �� : �ε�â
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
	
	bool					IsFirstLoading;			// ù �ε��� �̹��� ������

public:

	 CLoadingBarWnd()
	 {
		 Init();
	 }
	~CLoadingBarWnd(){}

	virtual void Init();
	virtual void Composition();
	virtual void Update();


	HRESULT InitDeviceObjects();// ���̵� �ƿ��� �ؽ�ó ����(CGlow ����)
	HRESULT InvalidateDeviceObjects();
	HRESULT RestoreDeviceObjects();
	
	void Open();
	void Close();

	HRESULT MakeScreenAfterImage();///-- ���̵� ��Ű������ �ѹ� ����̹����� ����ؽ��Ŀ� ��������Ų��.
	void DrawScreenAfterImage();// ���̵� �̹��� �׸���	

	int Get_warpmap_id() const { return ml_warpmap_ID;  };
	void Set_warpmap_id( const int al_map_id ){ ml_warpmap_ID = al_map_id ; };
	void SetPercent(int nPercent);

	CTextBox * GetHelpTex()	{ return m_pHelpTex; }

private:

	CProgressBar*			LoadingBar;
	CTextBox *				m_pHelpTex;	
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DSURFACE9		m_pBackBuffer;// �����
	LPDIRECT3DSURFACE9		m_pBackSurface;// ����� ����� ǥ��
	LPDIRECT3DTEXTURE9		m_pBackTexture;// ����� ����� �ؽ�ó

	LPD3DXRENDERTOSURFACE	m_pRenderToSurface;// ���� ������ ǥ��
	LPDIRECT3DTEXTURE9		m_pTex;// ���� �ؽ�ó
	LPDIRECT3DSURFACE9		m_pSurface;// ���� ǥ��

	LPDIRECT3DVERTEXBUFFER9	m_pVB;// ���� ���ؽ�
	LPDIRECT3DINDEXBUFFER9	m_pIB;

	LPDIRECT3DVERTEXBUFFER9	m_pBackVB;// ����� ���ؽ�
#else
	LPDIRECT3DSURFACE8		m_pBackBuffer;// �����
	LPDIRECT3DSURFACE8		m_pBackSurface;// ����� ����� ǥ��
	LPDIRECT3DTEXTURE8		m_pBackTexture;// ����� ����� �ؽ�ó

	LPD3DXRENDERTOSURFACE	m_pRenderToSurface;// ���� ������ ǥ��
	LPDIRECT3DTEXTURE8		m_pTex;// ���� �ؽ�ó
	LPDIRECT3DSURFACE8		m_pSurface;// ���� ǥ��

	LPDIRECT3DVERTEXBUFFER8	m_pVB;// ���� ���ؽ�
	LPDIRECT3DINDEXBUFFER8	m_pIB;

	LPDIRECT3DVERTEXBUFFER8	m_pBackVB;// ����� ���ؽ�
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