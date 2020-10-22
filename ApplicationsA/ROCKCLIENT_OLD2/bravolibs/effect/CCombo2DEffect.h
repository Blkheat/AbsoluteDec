#ifndef __CCOMBO2DEFFECT_H
#define __CCOMBO2DEFFECT_H

#ifdef DIRECT_VERSION_9_MJH
#include <d3dx9math.h>
#include <d3d9.h>
#else
#include <d3dx8math.h>
#include <d3d8.h>
#endif // DIRECT_VERSION_9_MJH

typedef enum
{
	COMBO_NONE	= -1 ,				// ÄÞº¸ ¾ÈÂï¾îÁÜ
	SLIDE_LEFT	=  0 ,				// ÄÞº¸ ¿ÞÂÊÀ¸·Î ÀÌµ¿
	SLIDE_RIGHT      ,				// ÄÞº¸ ¿À¸¥ÂÊÀ¸·Î ÀÌµ¿
	SLIDE_CENTER     ,				// ÄÞº¸ Á¦ÀÚ¸® Ã£±â
	COMBO_TWINKLE	 ,				// ÄÞº¸ ¹ß±¤
	COMBO_NORMAL     ,				// ÄÞº¸ º¸¿©ÁÖ±â

}COMBO_SQ_STATE;

typedef enum
{
	COMPLE_COMBO_NONE	= -1 ,
	COMPLE_SLIDE_LEFT	=  0 ,				
	COMPLE_SLIDE_RIGHT      ,				
	COMPLE_SLIDE_CENTER     ,			
	COMPLE_COMBO_TWINKLE	 ,			
	COMPLE_COMBO_NORMAL     ,

}COMPLE_COMBO_SQ_STATE;

typedef enum
{

	COMBO_NORMAL_IMAGE = 0 ,
	COMBO_SLIDE_IMAGE	   ,
	COMBO_SPOT_IMAGE	   ,

}COMBOIMAGETYPE;

typedef struct s_COMBO2DVERTEX
{
    D3DXVECTOR4 pos;
    D3DCOLOR    color;
    FLOAT       tu0, tv0;

	s_COMBO2DVERTEX()
	{
		pos.z = pos.w = 1.0f;
	}

}COMBO2DVERTEX;

#define D3DFVF_COMBO2DVERTEXT ( D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1 )


#define C2DCOMBO_LEFT_SLIDE_TIME	100
#define C2DCOMBO_RIGHT_SLIDE_TIME	50
#define C2DCOMBO_CENTER_SLIDE_TIME	50
#define C2DCOMBO_TWINKLE_SLIDE_TIME	100

#define C2DCOMBO_SPOT_TIME			10


typedef struct s_ComboTexture
{
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DTEXTURE9	Texture[ 3 ];
#else
	LPDIRECT3DTEXTURE8	Texture[ 3 ];
#endif // DIRECT_VERSION_9_MJH

}COMBOTEXTURE;


class CCombo2DEffect
{
public:
	 
	 CCombo2DEffect();
	~CCombo2DEffect();

public:
	
	int  Clear(void);
	int  Update(void);
	int  SetCombo(int race,int level, BOOL IsComplete = FALSE );
#ifdef DIRECT_VERSION_9_MJH
	int  Draw(LPDIRECT3DDEVICE9 pDevice);
	int	 LoadTexture(LPDIRECT3DDEVICE9 pDevice);
#else
	int  Draw(LPDIRECT3DDEVICE8 pDevice);
	int	 LoadTexture(LPDIRECT3DDEVICE8 pDevice);
#endif // DIRECT_VERSION_9_MJH
	void DeleteAllMemory(void);

	void DrawTexture( float x, float y, float w, float h, 
					  float tx, float ty, float tw, float th,
#ifdef DIRECT_VERSION_9_MJH
					  D3DCOLOR color , LPDIRECT3DDEVICE9 pDevice );
#else
					  D3DCOLOR color , LPDIRECT3DDEVICE8 pDevice );
#endif // DIRECT_VERSION_9_MJH
	

public:

	int				m_Race;
	int				m_ComboNum;
	BOOL			m_IsComplelete;
	
private:
	
	COMBO_SQ_STATE	m_State;
	
	COMBOIMAGETYPE	m_ImageNum;

	D3DXVECTOR2		m_Start;
	D3DXVECTOR2		m_LeftSide;
	D3DXVECTOR2		m_RightSide;
	D3DXVECTOR2		m_Center;

	D3DXVECTOR2		m_Pos;

	D3DXVECTOR2		m_StartPos;
	D3DXVECTOR2		m_EndPos;

	DWORD			m_RangeTime;
	DWORD			m_OldTimer;	

	BYTE			m_Alpha;

	COMPLE_COMBO_SQ_STATE m_Comple_State;

	COMBOIMAGETYPE	m_ComImageNum;

	D3DXVECTOR2		m_ComStart;
	D3DXVECTOR2		m_ComLeftSide;
	D3DXVECTOR2		m_ComRightSide;
	D3DXVECTOR2		m_ComCenter;

	D3DXVECTOR2		m_ComPos;

	D3DXVECTOR2		m_ComStartPos;
	D3DXVECTOR2		m_ComEndPos;

	DWORD			m_ComRangeTime;
	DWORD			m_ComOldTimer;
	
	BYTE			m_ComAlpha;
	
	COMBOTEXTURE	m_Texture[3][9];	

	DWORD			m_SpotTimer;
	int				m_SpotNum;
	BOOL			m_ISSpot;
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DTEXTURE9 m_SpotTexture[3][3];
	LPDIRECT3DTEXTURE9 m_CompleTexture[3];
#else
	LPDIRECT3DTEXTURE8 m_SpotTexture[3][3];
	LPDIRECT3DTEXTURE8 m_CompleTexture[3];
#endif // DIRECT_VERSION_9_MJH
};

#endif