//...............................................................................................................
// 
//	CEFBaragosEff.h														 �ٶ�� ���� ����Ʈ
//
//...............................................................................................................
#ifndef __CEFBaragosEff_H
#define __CEFBaragosEff_H

#include "CEffectBase.h"
#include "CDropEffect.h"

#include <list>
using namespace	std;

struct BRESS_POS
{
	DWORD		dInitTime;
	D3DXVECTOR3 vStartPos;
	D3DXVECTOR3 vEndPos;

	BRESS_POS()
	{
		memset(this, 0, sizeof(BRESS_POS));
	}
	
	void operator=(BRESS_POS &temp)
	{
		dInitTime = temp.dInitTime;
		vStartPos = temp.vStartPos;
		vEndPos = temp.vEndPos;
	}	
};

class CEFBaragosEff : public CEffectBase
{
public:
	CEFBaragosEff();
	virtual ~CEFBaragosEff();


public:
	enum BESS_STATE
	{
		BRESS_STATE_NONE = 0,
		BRESS_STATE_SHOT,
		BRESS_STATE_MOVE,
		BRESS_STATE_CLOSE,
	};
	
	enum THUNDER_STATE
	{
		THUNDER_STATE_NONE = 0,
		THUNDER_STATE_SHOT,
		THUNDER_STATE_MOVE,
		THUNDER_STATE_CLOSE,
	};


public:
	virtual void	 Init(void);
	virtual int		 CheckFrame(void);
	virtual int      Update(float dtime);
	virtual int		 Render(float dtime);


public:
	void	InitHowlOfDevil();
	void	InitThunderBreak();// ���� �극��ũ(�����)
	void	InitBerSerk();// ����ȭ �ʱ�ȭ	
	
	// ������ ��ȣ�� �ٸ� ����Ʈ�� ������.
	int		CheckDragonBress();
	int		CheckHowlOfDevil();
	int		CheckThunderBreak();
	int		CheckBerserk();// ����ȭ ����Ʈ ������ üũ

	int		UpdateDragonBress(float dtime);
	int		UpdateHowlOfDevil(float dtime);
	int		UpdateThunderBreak(float dtime);

	int		RenderDragonBress(float dtime);
	int		RenderHowlOfDevil(float dtime);
	int		RenderBerserk(float dtime);// ����ȭ �׸���

	DWORD			 m_InitTime;
	DWORD			 m_RangeTime;
	DWORD			 m_PrevTime;
	DWORD			 m_GapTime;

	DWORD			 m_ParticleTime;
	float			 m_fRadius;

	D3DXVECTOR3		 m_vPreStartPos;
	D3DXVECTOR3		 m_vMoveDir;
	D3DXVECTOR3		 m_vTempPos;

	BOOL			 m_ISShot;
	BOOL			 m_ISDamageProcess;

	BESS_STATE		 m_nBressState;
	THUNDER_STATE	 m_nThunderState;

	list < BRESS_POS > m_BressParPosInfoList;	

	CParticleSystem	 m_ParticleLink[20];
	CParticleSystem	 m_AddParticleLink[2];


private:
	enum
	{
#ifdef HHM_SEALED_TEMPLE_BOSSMOB_EFFECT_LIMIT_20091207
		
		ME_BERSERK_COUNT = 4,

#else
		
		ME_BERSERK_COUNT = 12,

#endif// #ifdef HHM_SEALED_TEMPLE_BOSSMOB_EFFECT_LIMIT_20091207
	};


private:
	bool	m_bDragonBressMouseFire;// �� ������ ȭ�� �� ū ȭ���� �¿���
	int		m_iDragonBress1132Count;	

	bool	m_bHowlOfDevilBlur;

	int		m_iThunderBreak1129Count;

	float	m_fBerserkFireEffSize;// ����Ʈ ������
	int		m_aiBerserkFireBone[ME_BERSERK_COUNT];// ����Ʈ�� ���� ���� �ε���
};

#endif