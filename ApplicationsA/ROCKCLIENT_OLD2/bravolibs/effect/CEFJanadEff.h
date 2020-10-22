//...............................................................................................................
// 
//	CEFJanadEff.h														 �𳪵� ���� ����Ʈ
//
//...............................................................................................................
#ifndef __CEFJanadEff_H
#define __CEFJanadEff_H

#include "CEffectBase.h"
#include "CDropEffect.h"


class CEFJanadEff : public CEffectBase
{
public:
	CEFJanadEff();

	
public:
	virtual void	 Init(void);
	virtual int		 CheckFrame(void);
	virtual int      Update(float dtime);
	virtual int		 Render(float dtime);


public:
	void	InitDashTripleAttack()	{}//-- �� ��ų�� �ϴ� Ȧ��;;;	
	void	InitCallDevilSoul();// ���ɼ�ȯ
	void	InitDarkSwordStorm();// �����ǳ
	void	InitDispel();// ����
	void	InitMargmaBust();// �������

	int		CheckDashTripleAttack();
	int		CheckCallDevilSoul();
	int		CheckDarkSwordStorm();
	int		CheckDispel();
	int		CheckMargmaBust();
	int		CheckSwordFlashOfDevil();// ��Ȳ�� ����

	int		UpdateDarkSwordStorm(float dtime);
	int		UpdateMargmaBust(float dtime);
	int		UpdateSwordFlashOfDevil(float dtime);

	int		RenderCallDevilSoul(float dtime);
	int		RenderDarkSwordStorm(float dtime);
	int		RenderDispel(float dtime);
	int		RenderMargmaBust(float dtime);
	int		RednerSwordFlashOfDevil(float dtime);

	DWORD			 m_dDestCell;		///-- �뽬������ �̵���ġ	
	
	DWORD			 m_InitTime;
	DWORD			 m_RangeTime;
	DWORD			 m_PrevTime;
	DWORD			 m_GapTime;
	DWORD			 m_RunningTime;

	float			 m_fRadius;

	D3DXVECTOR3		 m_vTempPos;
	D3DXVECTOR3		 m_vCenterPos;

	BOOL			 m_ISShot;
	BOOL			 m_ISDamageProcess;

	D3DXVECTOR3		 m_vStartPos[36];
	D3DXVECTOR3		 m_vEndPos[36];

	CParticleSystem	 m_ParticleLink[20];
	CParticleSystem	 m_ParticleLink2[20];


private:
	int		m_iSwordFlashOfDevilCount;
	float	m_fSwordFlashOfDevilRadius;

	int		m_iMargmaBustCount;
	bool	m_bMargmaBust;

	bool	m_bDispel;

	bool	m_bDarkSwordStorm;
	int		m_iDarkSwordStormCount;
};


#endif