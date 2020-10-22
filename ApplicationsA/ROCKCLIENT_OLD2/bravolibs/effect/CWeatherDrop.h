#ifndef __CWEATHERDROP_H
#define __CWEATHERDROP_H

#include <iostream>
#include <vector>
#ifdef DIRECT_VERSION_9_MJH
#include <d3dx9math.h>
#include <d3d9.h>
#else
#include <d3dx8math.h>
#include <d3d8.h>
#endif // DIRECT_VERSION_9_MJH
#include "CBillboardLine.h"
#include "CFade.h"

typedef struct 
{

    D3DXVECTOR3 pos;
	float		u,v;			

}WEATHER_DROP_VERTEX;

typedef struct 
{
	D3DXVECTOR3 pos;
	DWORD	    color;

}WEATHER_DROP_LINE;

class CWeatherDropData
{
public:
	 
	 CWeatherDropData();
	~CWeatherDropData() {}

	int Update(float Time);
	
public:

	D3DXVECTOR3		m_vLocation;

	D3DXVECTOR3		m_vGravity;
	D3DXVECTOR3		m_vGravityDelta;

	D3DXCOLOR		m_Color;
	D3DXCOLOR		m_ColorDelta;

	float		    m_LifeTime;				
	float		    m_NowTime;				
	float			m_DelayTime;

	D3DXVECTOR3		m_vScale;
	D3DXVECTOR3     m_vPos;

	DWORD			m_AniTimeRange;				// ���ϸ��̼� �Ǵ� �ð� ����
	DWORD			m_OldAniTime;				// ���� ���ϸ��̼� �ð�
	DWORD			m_AniTexNum;				// ���ϸ��̼� �ؽ�ó ��ȣ
	
	int				m_TexType;
};

typedef enum WeatherDropType
{
	WEATHER_TYPE_NONE = -1 ,
	WEATHER_TYPE_RAIN	   ,
	WEATHER_TYPE_SNOW	   ,
	WEATHER_TYPE_LEAF	   ,	
	WEATHER_TYPE_PAPER	   ,
	WEATHER_TYPE_NIGHT	   ,	
};

enum
{
	n_WNone = 0,	  //�ƹ��ܰ� �ƴ�							
	n_WDaytoNight,	  //������ ������...	
	n_WNighttoDay,	  //�㿡�� ������...	
};


#define D3DFVF_WEATHER_DROP_VERTEX			(D3DFVF_XYZ|D3DFVF_TEX1)
#define D3DFVF_WEATHER_DROP_LINE			(D3DFVF_XYZ|D3DFVF_DIFFUSE)

#define WEATHER_DROP_TICK					100							// �������� ����

#define WEATHER_DROP_MIN_X					-160.0f
#define WEATHER_DROP_MAX_X					160.0f
		
#define WEATHER_DROP_MIN_Y				 	0.0f
#define WEATHER_DROP_MAX_Y					100.0f

#define WEATHER_DROP_MIN_Z					-160.0f
#define WEATHER_DROP_MAX_Z					160.0f

#define WEATHER_DROP_MIN_VELOCITY			1.0f
#define WEATHER_DROP_MAX_VELOCITY			5.0f

#define WEATHER_RAIN_MIN_HEIGHT				2.0f
#define WEATHER_RAIN_MAX_HEIGHT				7.0f

#define WEATHER_SNOW_WIDTH					1.0f
#define WEATHER_SNOW_HEIGHT					1.0f

#define WEATHER_NIGHT_FADE_TIME				3000

class CWeatherDrop
{
public:
	
	 CWeatherDrop();
	~CWeatherDrop();

public:

	int  Update(float Time,D3DXVECTOR3 Pos);
	void UpdateDNType();
	int  LoadTexture(void);
	int  CreateAll(int Num);
#ifdef DIRECT_VERSION_9_MJH
	int  Render(LPDIRECT3DDEVICE9 pDevice,D3DXMATRIX matView,D3DXVECTOR3 Pos);
	int  RenderRain(LPDIRECT3DDEVICE9 pDevice,D3DXMATRIX matView,D3DXVECTOR3 Pos);
	int  RenderSnow(LPDIRECT3DDEVICE9 pDevice,D3DXMATRIX matView,D3DXVECTOR3 Pos);
	int  RenderLeaf(LPDIRECT3DDEVICE9 pDevice,D3DXMATRIX matView,D3DXVECTOR3 Pos);
	int  RenderPapaer(LPDIRECT3DDEVICE9 pDevice,D3DXMATRIX matView,D3DXVECTOR3 Pos);
	int  InitDevice(LPDIRECT3DDEVICE9 pDevice);
#else
	int  Render(LPDIRECT3DDEVICE8 pDevice,D3DXMATRIX matView,D3DXVECTOR3 Pos);
	int  RenderRain(LPDIRECT3DDEVICE8 pDevice,D3DXMATRIX matView,D3DXVECTOR3 Pos);
	int  RenderSnow(LPDIRECT3DDEVICE8 pDevice,D3DXMATRIX matView,D3DXVECTOR3 Pos);
	int  RenderLeaf(LPDIRECT3DDEVICE8 pDevice,D3DXMATRIX matView,D3DXVECTOR3 Pos);
	int  RenderPapaer(LPDIRECT3DDEVICE8 pDevice,D3DXMATRIX matView,D3DXVECTOR3 Pos);
	int  InitDevice(LPDIRECT3DDEVICE8 pDevice);
#endif // DIRECT_VERSION_9_MJH
	void Init();
	void SetDropType(WeatherDropType Type);
	
	WeatherDropType GetDropType(void)	    { return m_DropType;  }

	void SetDNType(UINT dType);
	UINT GetDNType()						{ return m_udType;	}

	
	CFade	m_Fade;

	void Reset(void);
#ifdef DIRECT_VERSION_9_MJH
	D3DLIGHT9				m_Character_Light;
	D3DLIGHT9				m_Object_Light;
	D3DLIGHT9				m_Terrain_Light;
	D3DLIGHT9				m_Sky_Light;
#else
	D3DLIGHT8				m_Character_Light;
	D3DLIGHT8				m_Object_Light;
	D3DLIGHT8				m_Terrain_Light;
	D3DLIGHT8				m_Sky_Light;
#endif // DIRECT_VERSION_9_MJH	

	D3DVECTOR				m_Fog_Color;
	BOOL					m_WasBeforeNight;
	
private:
	
	int Create(int i,BOOL IsOnetime,D3DXVECTOR3 Pos);
	int m_Count;

	UINT m_udType;	//�㳷 ����Ÿ��
	long m_lPrevtime;
	bool m_Onetime;
	bool m_isNight;

private:

	WeatherDropType							m_DropType;		

#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DTEXTURE9						m_pTexture[2];
	LPDIRECT3DVERTEXBUFFER9					m_pVB;
	LPDIRECT3DINDEXBUFFER9					m_pIB;
	LPDIRECT3DTEXTURE9						m_pLeafTexture[3];
	LPDIRECT3DTEXTURE9						m_pPaperTexture[6][3];
#else
	LPDIRECT3DTEXTURE8						m_pTexture[2];
	LPDIRECT3DVERTEXBUFFER8					m_pVB;
	LPDIRECT3DINDEXBUFFER8					m_pIB;
	LPDIRECT3DTEXTURE8						m_pLeafTexture[3];
	LPDIRECT3DTEXTURE8						m_pPaperTexture[6][3];
#endif // DIRECT_VERSION_9_MJH

	std::vector		<CWeatherDropData>		m_List;

};

#endif