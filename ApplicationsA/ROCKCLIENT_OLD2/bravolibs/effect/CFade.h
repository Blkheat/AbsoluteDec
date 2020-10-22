#ifndef __CFADE_H
#define __CFADE_H


#ifdef DIRECT_VERSION_9_MJH
#include <d3d9.h>
#include "d3dx9math.h"
#else
#include <d3d8.h>
#include "d3dx8math.h"
#endif // DIRECT_VERSION_9_MJH

typedef struct s_VERTEX_FADE_2D
{
	
	D3DXVECTOR4	vpos;
	D3DCOLOR   color;

	s_VERTEX_FADE_2D()
	{
		vpos  = D3DXVECTOR4( 0.0f , 0.0f , 0.0f , 1.0f );
		color = D3DCOLOR_ARGB( 0 , 0 , 0 , 0 );
	}

}VERTEX_FADE_2D;

#define D3DFVF_VERTEX_FADE_2D	(D3DFVF_XYZRHW|D3DFVF_DIFFUSE)

typedef enum
{

	FADE_STATE_NONE	        ,		// alpha 0
	FADE_STATE_FADE		    ,		// alpha 255 -> 0
	FADE_STATE_CONTINUE		,		// alpha ����
	FADE_STATE_LOOP					// ���� -> �߰� -> ��

}POST_FADE_STATE;


class CFade
{
public:

	 CFade();
	~CFade();

public:

	 void Init(int xsize,int ysize);																// ȭ�� ������ ����
	 void Start(POST_FADE_STATE Type,D3DXCOLOR scolor,D3DXCOLOR ecolor,DWORD Time,BOOL ISContinue); // ���̵� ���� 
     void SetColor(D3DXCOLOR color);																// ���� Į�� ����
#ifdef DIRECT_VERSION_9_MJH
	  void Process(LPDIRECT3DDEVICE9 pDevice);														// ���̵� ó�� �κ�
#else
	  void Process(LPDIRECT3DDEVICE8 pDevice);														// ���̵� ó�� �κ�
#endif // DIRECT_VERSION_9_MJH
	 void Render();
	 void ResetFade(void);																			// ���̵� ó�� ����
	 
	 D3DCOLOR GetColor(void)	{ return m_Color; }

	 void Start(POST_FADE_STATE Type,D3DXCOLOR scolor,D3DXCOLOR mcolor,D3DXCOLOR ecolor,DWORD Time,DWORD Time2,BOOL ISContinue);

	 POST_FADE_STATE GetState(void)	{ return m_State;}
private:

	 void  SetEndColor(D3DXCOLOR color);						    							 // �� �� ����
	 float GetRange(void);
	 void  SetVertexColor(D3DXCOLOR color);

private:

	CRITICAL_SECTION	csFadeFlag;	
	
	D3DXCOLOR	m_Color;
	D3DXCOLOR	m_EndColor;
	D3DXCOLOR	m_MiddleColor;
	DWORD		m_MiddleTime;
	
	POST_FADE_STATE  m_State;

	DWORD		m_OldTime;
	DWORD		m_RangeTime;

	int         m_XSize;
	int         m_YSize;
	
	BOOL		m_ISContinue;

	VERTEX_FADE_2D	m_VertexList[ 4 ];
};


#endif