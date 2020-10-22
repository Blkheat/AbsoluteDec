#ifndef __CEFTripleFire_H
#define __CEFTripleFire_H

#include "CEffectBase.h"

struct sFireData 
{

	DWORD m_OldTime;
	DWORD m_RangeTime;
	int m_AddRange; //변위 
	int m_Angle;	// 각 
	int m_Speed;
	
	D3DXVECTOR3 m_vStartPos; //시작위치
	D3DXMATRIX m_matWorld; //위치
	bool bStartFlag;

	sFireData()
	{
		m_OldTime = 0 ; 
		m_RangeTime  = 0 ; 
		m_vStartPos =D3DXVECTOR3(0.0f, 0.0f, 0.0f ) ; 
		
		m_AddRange = 0 ; 
		m_Angle= 0 ;
		m_Speed = 0 ;
		bStartFlag =0  ; 

		D3DXMatrixIdentity( &m_matWorld );
	}

};

class CEFTripleFire : public CEffectBase
{
public:
	
	         CEFTripleFire();
	virtual ~CEFTripleFire();

public:

	virtual int		 CheckFrame(void);
	virtual int		 Render(float dtime);							
	virtual int      Update(float dtime);
	virtual int      SetAfterEffect(void);

	static  int		 InitDevice(void);
	static  int		 DeleteDevice(void);
	
	virtual void	 Init(void);
	void InitFireBall();

	void TripleFire(float dtime);
	void TestTripleFire(float dtime);
	void SpellEnd();
	
private:

	CParticleSystem	 m_FireParticle[3];
	sFireData	FireData[3];
	float		m_fHeight[3];
	


	DWORD m_OldTime; 
	float m_DamagePercent[3];
	int m_MaxDamage;
	int m_EffectCnt;



};

#endif


