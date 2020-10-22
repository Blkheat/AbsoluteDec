#ifndef		_____WATER_____
#define		_____WATER_____


struct		VTXWATER
{
	float	x, y, z;
	float	tu, tv;
};

#define		D3DFVF_WATER		(D3DFVF_XYZ | D3DFVF_TEX1)

class		CPlaneWater
{
protected:
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DDEVICE9		m_pDevice;
	LPDIRECT3DVERTEXBUFFER9	m_pVB;
	LPDIRECT3DINDEXBUFFER9	m_pIB;
#else
	LPDIRECT3DDEVICE8		m_pDevice;
	LPDIRECT3DVERTEXBUFFER8	m_pVB;
	LPDIRECT3DINDEXBUFFER8	m_pIB;
#endif // DIRECT_VERSION_9_MJH
	VTXWATER	*			m_pVtxList;
	WORD	*				m_pIdxList;
	DWORD					m_dwFVF;

public:
	D3DXMATRIX				m_matPos;
	D3DXMATRIX				m_matTexPos;
	LONG					m_lPreCount;
	DWORD					m_dwPos;
	float					m_fHeight;
	short					m_nDir;
	bool					m_bIsVisible;
	D3DXVECTOR3				m_vPos[4];

#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DDEVICE9		GetDevice()	{ return m_pDevice; };
#else
	LPDIRECT3DDEVICE8		GetDevice()	{ return m_pDevice; };
#endif // DIRECT_VERSION_9_MJH

public:
	short					m_texIdx;
	short					m_listIdx;

public:
#ifdef DIRECT_VERSION_9_MJH
	bool		InitDevice(LPDIRECT3DDEVICE9 pdev);
#else
	bool		InitDevice(LPDIRECT3DDEVICE8 pdev);
#endif // DIRECT_VERSION_9_MJH
	bool		Create(const D3DXVECTOR3& v0, const D3DXVECTOR3& v1, const D3DXVECTOR3& v2, const D3DXVECTOR3& v3);
	bool		Update(const D3DXVECTOR3& v0, const D3DXVECTOR3& v1, const D3DXVECTOR3& v2, const D3DXVECTOR3& v3);
	void		PlayAni();
	HRESULT		Display();
	void		FinalCleanup();
	void		SetupPosTM(float x, float y, float z);
	
	CPlaneWater();
	~CPlaneWater();
};


#endif