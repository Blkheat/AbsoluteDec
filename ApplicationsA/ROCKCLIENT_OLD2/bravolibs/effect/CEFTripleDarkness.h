#ifndef __CEFTripleDarkness_H
#define __CEFTripleDarkness_H

#include "CEffectBase.h"


class CEFTripleDarkness : public CEffectBase
{

enum E_OBJECT{ 
	FIRST ,	
	SECOND ,
	THIRD ,
};

struct sDarkData 
{

	DWORD m_OldTime;
	DWORD m_RangeTime;
	int m_AddRange; //변위 
	float m_Angle;	// 각 
	int m_Speed;
	float OldRange;
	int	m_Theta;	//초기시작각 
	D3DXVECTOR3 m_vStartPos; //시작위치
	D3DXMATRIX matLocal; //위치
	D3DXMATRIX matX;
	D3DXMATRIX matY;
	D3DXMATRIX matZ;

	
	bool bStartFlag;

	sDarkData()
	{
	
		Clear();

	}
	void Clear()
	{
		m_OldTime = 0 ; 
		m_RangeTime  = 0 ; 
		m_vStartPos =D3DXVECTOR3(0.0f, 0.0f, 0.0f ) ; 
			
		m_Theta = 0 ; 

		m_AddRange = 0 ; 
		m_Angle= 0 ;
		m_Speed = 0 ;
		bStartFlag =0  ; 
		OldRange = 0 ; 


		D3DXMatrixIdentity( &matLocal );
		D3DXMatrixIdentity( &matX );
		D3DXMatrixIdentity( &matY );
		D3DXMatrixIdentity( &matZ );

	}

};


struct TDark
{
	sDarkData Ball[3];
	CParticleSystem	 m_DarkParticle[3];
	bool	bLumpflag;
	
	TDark()
	{
		Clear();
	}
	void Clear()
	{
		for(int i=0 ; i < 3 ; ++i)
		{
			Ball[i].Clear();
		}
		
		bLumpflag = 0;

	}
	
	
};


public:
	
	         CEFTripleDarkness();
	virtual ~CEFTripleDarkness();

public:

	virtual int		 CheckFrame(void);
	virtual int		 Render(float dtime);							
	virtual int      Update(float dtime);
	virtual int      SetAfterEffect(void);

	static  int		 InitDevice(void);
	static  int		 DeleteDevice(void);
	
	virtual void	 Init(void);


	void SpellEnd();
	
private:

	
	TDark		DarkData[3];
	float		m_fHeight[3];
	
	void DarkBall(int LumpNum , int Num , int Theta , float dtime );


	DWORD m_OldTime; 
	float m_DamagePercent[3];
	int m_MaxDamage;
	int m_EffectCnt;



};

#endif

