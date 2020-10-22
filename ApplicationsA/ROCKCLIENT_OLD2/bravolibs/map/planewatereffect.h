#ifndef		_____PLANE_WATER_EFFECT_____
#define		_____PLANE_WATER_EFFECT_____

struct		VTXWATER;

enum		WE_DIR
{
	WED_TOP = 0,
	WED_TRIGHT,
	WED_RIGHT,
	WED_BRIGHT,
	WED_BOTTOM,
	WED_BLEFT,
	WED_LEFT,
	WED_TLEFT,
};

class		CPlaneWaterEffect
{
public:
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DDEVICE9		m_pDevice;
	LPDIRECT3DVERTEXBUFFER9	m_pVB;
	LPDIRECT3DINDEXBUFFER9	m_pIB;
	D3DMATERIAL9			m_Material;
#else
	LPDIRECT3DDEVICE8		m_pDevice;
	LPDIRECT3DVERTEXBUFFER8	m_pVB;
	LPDIRECT3DINDEXBUFFER8	m_pIB;
	D3DMATERIAL8			m_Material;
#endif // DIRECT_VERSION_9_MJH

	VTXWATER	*			m_pVtxList;
	WORD	*				m_pIdxList;
	D3DXMATRIX				m_matPos;
	float					m_fAngle;
	D3DXMATRIX				m_matTemp;

	LONG					m_lPreCount;
	short					m_nFps;
	float					m_fPosStep;

public:

	bool		m_bIsVisible;

#ifdef DIRECT_VERSION_9_MJH
	bool		InitDevice(LPDIRECT3DDEVICE9 pdev);
#else
	bool		InitDevice(LPDIRECT3DDEVICE8 pdev);
#endif // DIRECT_VERSION_9_MJH
	bool		Create(BYTE dir, float x, float y, float z);
	void		FinalCleanup();
	void		Display();
	void		SetupPosTM(float x, float y, float z);
	void		SetupRotTM(float angle);
	void		PlayAni();

	CPlaneWaterEffect();
	~CPlaneWaterEffect();
};

#endif