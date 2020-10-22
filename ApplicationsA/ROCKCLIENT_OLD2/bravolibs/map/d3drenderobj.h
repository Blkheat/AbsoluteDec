#ifndef	D3D8_RENDERER
#define	D3D8_RENDERER

class	CRenderObj
{
protected:
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DDEVICE9		m_pd3dDevice;
	LPDIRECT3DINDEXBUFFER9	m_pIB;
	LPDIRECT3DVERTEXBUFFER9	m_pVB;
#else
	LPDIRECT3DDEVICE8		m_pd3dDevice;
	LPDIRECT3DINDEXBUFFER8	m_pIB;
	LPDIRECT3DVERTEXBUFFER8	m_pVB;
#endif // DIRECT_VERSION_9_MJH
	D3DXMATRIX				m_matPos;
	DWORD					m_dwFVF;

	short					m_nStartX, m_nStartY, m_nEndX, m_nEndY;

public:
	unsigned int			m_nVtxCount;
	unsigned int			m_nIdxCount;

public:
#ifdef DIRECT_VERSION_9_MJH
	virtual	HRESULT	InitDeviceObjects(LPDIRECT3DDEVICE9 pd3ddev);
#else
	virtual	HRESULT	InitDeviceObjects(LPDIRECT3DDEVICE8 pd3ddev);
#endif // DIRECT_VERSION_9_MJH
	virtual	HRESULT	RestoreDeviceObjects();
	virtual	HRESULT	InvalidateDeviceObjects();
	virtual	HRESULT	DeleteDeviceObjects();
	virtual	HRESULT	Error(const char* strError);

	short				GetStartX()	{ return m_nStartX; }
	short				GetStartY()	{ return m_nStartY; }
	short				GetEndX()	{ return m_nEndX; }
	short				GetEndY()	{ return m_nEndY; }

public:
	CRenderObj();
};

class	CRenderBound	: public CRenderObj
{
public:
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DVERTEXBUFFER9	m_pDebugVB;
#else
	LPDIRECT3DVERTEXBUFFER8	m_pDebugVB;
#endif // DIRECT_VERSION_9_MJH

public:
	bool	m_bIsReady;
	int		m_nVtxCount, m_nIdxCount;
	
	D3DXVECTOR3 m_vMin;
	D3DXVECTOR3 m_vMax;
	
public:
	HRESULT	RestoreDeviceObjects();
	HRESULT	Render(const D3DXMATRIX& matPos);	
	HRESULT	Update(D3DXVECTOR3 min, D3DXVECTOR3 max);
	HRESULT	InvalidateDeviceObjects();
	HRESULT	DeleteDeviceObjects();
#ifdef DIRECT_VERSION_9_MJH
	HRESULT	InitDeviceObjects(LPDIRECT3DDEVICE9 pd3dDev);
#else
	HRESULT	InitDeviceObjects(LPDIRECT3DDEVICE8 pd3dDev);
#endif // DIRECT_VERSION_9_MJH
	HRESULT	FinalCleanup();

	//by simwoosung
	HRESULT RenderBox(void * pCharacter);

	CRenderBound();	
	~CRenderBound();	
};

#endif