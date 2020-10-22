//............................................................................................................
//
// CSetEnv.h											 
//
//............................................................................................................
#ifndef __CSETENV_H
#define __CSETENV_H

#ifdef DIRECT_VERSION_9_MJH
#include <d3dx9math.h>
#include <d3d9.h>
#else
#include <d3dx8math.h>
#include <d3d8.h>
#endif // DIRECT_VERSION_9_MJH
#include <vector>

using namespace std;

#define ENVSET_FILENAME				"Env\\setenv.dat"


typedef enum
{

	ENV_DAYLIGHT = 0 ,				// �� 
	ENV_DUSK	  	 ,				// Ȳȥ
	ENV_NIGH		 ,				// ��
	ENV_DAWN		 ,				// ����
	ENV_CYCLE_MAX

}ENV_CYCLE;

typedef struct s_EnvLight
{
	D3DCOLORVALUE Ambient;			
	D3DCOLORVALUE Diffuse;
	D3DCOLORVALUE Specular;

}ENV_LIGHT_DATA,*LPENV_LIGHT_DATA;

typedef struct s_EnvMaterial
{

	D3DCOLORVALUE Ambient;
	D3DCOLORVALUE Diffuse;
	D3DCOLORVALUE Emissive;
	D3DCOLORVALUE Specular;
	float		  Power;

}ENV_MATERIAL_DATA,*LPENV_MATERIAL_DATA;


typedef struct s_EnvData
{
	int   MapIndex;						// �� �ε���
	int  IsInsect0;					// ���� ����
	int  IsInsect1;					// ��   ����
	
	int   CloudIndex;					// ���� �ε���
	float CloudSize;					// ���� ������
	float CloudHeight;					// ���� ����
	int	  SkyIndex;						// ��ī�� �ڽ� �ε���

	float FogR;							// ���� R
	float FogG;							// ���� G
	float FogB;							// ���� B
	
	float FogStart;						// ���� ����
	float FogEnd;						// ���� ��
	float FogDensity;					// ���� �е�

	int	  BGM;							// ��� ����
	
	ENV_LIGHT_DATA	LandLight;			// ���� ����Ʈ
	ENV_LIGHT_DATA  ObjectLight;		// ������Ʈ ����Ʈ

	ENV_MATERIAL_DATA LandMaterial;		// ���� ���͸���
	ENV_MATERIAL_DATA ObjectMaterial;	// ������Ʈ ���͸���

	s_EnvData()
	{
		memset( this , 0 , sizeof( s_EnvData ) );
	}

	bool operator ==(const int &Index) const
	{
		return ( MapIndex == Index );  
	}


}ENV_DATA,*LPENV_DATA;


class CSetEnv
{
public:
	 
	 CSetEnv();
	~CSetEnv();

	 
public:
#ifdef DIRECT_VERSION_9_MJH
	inline D3DMATERIAL9	GetLandMaterial(void)		{ return m_LandMaterial;}
	inline D3DLIGHT9	GetLandLight(void)			{ return m_LandLight;}

	inline D3DMATERIAL9	GetObjectMaterial(void)		{ return m_ObjectMaterial;}
	inline D3DLIGHT9	GetObjectLight(void)		{ return m_ObjectLight;}
#else
	inline D3DMATERIAL8	GetLandMaterial(void)		{ return m_LandMaterial;}
	inline D3DLIGHT8	GetLandLight(void)			{ return m_LandLight;}

	inline D3DMATERIAL8	GetObjectMaterial(void)		{ return m_ObjectMaterial;}
	inline D3DLIGHT8	GetObjectLight(void)		{ return m_ObjectLight;}
#endif // DIRECT_VERSION_9_MJH

	int	   Load(char *pPath);
	
	int	   Set(int MapIndex,ENV_CYCLE Cycle);

	LPENV_DATA			GetSet(void)				{ return m_pSelectData; }

private:
	
	vector <ENV_DATA>	m_SetList;
#ifdef DIRECT_VERSION_9_MJH
	D3DLIGHT9			m_LandLight;
	D3DMATERIAL9		m_LandMaterial;

	D3DLIGHT9			m_ObjectLight;
	D3DMATERIAL9		m_ObjectMaterial;
#else
	D3DLIGHT8			m_LandLight;
	D3DMATERIAL8		m_LandMaterial;

	D3DLIGHT8			m_ObjectLight;
	D3DMATERIAL8		m_ObjectMaterial;
#endif // DIRECT_VERSION_9_MJH

	int					m_MapIndex;
	ENV_CYCLE			m_CycleState;

	LPENV_DATA          m_pSelectData;
};

#endif