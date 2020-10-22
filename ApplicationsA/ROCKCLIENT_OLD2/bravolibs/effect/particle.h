#pragma once

#ifdef DIRECT_VERSION_9_MJH
#include	<D3DX9.H>
#else
#include	<D3DX8.H>
#endif // DIRECT_VERSION_9_MJH
//#include	"EffectBase.h"

class CParticleSystem;

#define		EFFECT_REFLECT 0.4f


class	CParticle
{
public:

	 CParticle();  // {m_pNext = NULL; m_pPrev = NULL;}
	~CParticle();

public:

	FLOAT			m_fAge;
	FLOAT			m_fLifetime;

	FLOAT			m_fSize;
	FLOAT			m_fSizeDelta;

	FLOAT			m_fGravity;
	FLOAT			m_fGravityDelta;

	D3DXVECTOR3		m_vGravity;
	D3DXVECTOR3		m_vGravityDelta;
	
	D3DXVECTOR3		m_vDirection;

	FLOAT			m_fAlpha;
	FLOAT			m_fAlphaDelta;

	D3DXCOLOR		m_Color;
	D3DXCOLOR		m_ColorDelta;

	D3DXVECTOR3		m_vLocation;
	D3DXVECTOR3		m_vPrevLocation;
	D3DXVECTOR3		m_vVelocity;

	CParticleSystem *m_pParent;

	D3DXMATRIX		matLocal;
	
	BOOL			m_IsInitUpdate;
	
	long			m_nPrevUpdateTime;		// 업데이트 이전타임
	long			m_nDelUpdateTime;		// 업데이트 간격타임
	BOOL			m_IsUpdate;				// 업데이트 여부
	FLOAT			m_fDelCumtime;			// 누적타임

public:

	void			Delete();
	BOOL			Update(float fTimeDelta);

	VOID			SetParent(CParticleSystem* Parent)	{ m_pParent = Parent; }

	//CParticle*	m_pNext;
	//CParticle*	m_pPrev;


public:

	void operator=(const CParticle &Src);

};