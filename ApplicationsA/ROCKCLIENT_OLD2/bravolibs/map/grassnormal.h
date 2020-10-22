#ifndef		_____GRASS_NORMAL_____
#define		_____GRASS_NORMAL_____

/*
		\|/
		- -	
		/|\
*/


#define		D3DFVF_GRSNORMAL		(D3DFVF_XYZ|D3DFVF_TEX1)
#define		HEIGHT_GRSNORMAL		10.0f

struct		VTXGRSNORMAL
{
	float	x, y, z;
	float	tu, tv;
};

struct		KEYGRSNORMAL
{
	float	fMaxAngle;
	float	fMinAngle;
	float	fCurAngle;
	LONG	lFps;
	LONG	lPreCount;
	bool	bDirInc;

	bool	UpdateFrame();
};


/*************************************************************
	* GRASS DATA
*************************************************************/
class	CGrassData
{
public:
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DDEVICE9		m_pDevice;
	LPDIRECT3DVERTEXBUFFER9	m_pVB;
	LPDIRECT3DINDEXBUFFER9	m_pIB;
#else
	LPDIRECT3DDEVICE8		m_pDevice;
	LPDIRECT3DVERTEXBUFFER8	m_pVB;
	LPDIRECT3DINDEXBUFFER8	m_pIB;
#endif // DIRECT_VERSION_9_MJH

	VTXGRSNORMAL *		m_pVtxList;
	WORD *				m_pIdxList;
	KEYGRSNORMAL		m_Key[4];

	float	m_fHeight;
	short	m_nTexIdx;

#ifdef DIRECT_VERSION_9_MJH
	void	InitDevice(LPDIRECT3DDEVICE9 pdev)	{ m_pDevice = (pdev) ? pdev : NULL; };
#else
	void	InitDevice(LPDIRECT3DDEVICE8 pdev)	{ m_pDevice = (pdev) ? pdev : NULL; };
#endif // DIRECT_VERSION_9_MJH
	void	Update();
	bool	Load( short nTexIndex, float fHeight );
};

/*************************************************************
	* GRASS Ani File
*************************************************************/
class	CGrassAni
{
public:
	KEYGRSNORMAL *			m_Key;
};

class	CGrassList
{
public:
	CGrassData *			m_pData;
	CGrassAni *				m_pAni;
	D3DXMATRIX				m_matPos;
	bool					m_bIsVisible;
	short					m_nGrassIdx;
	CGrassList	*			m_pNext;

	void					Display();

	CGrassList()
	{
		m_pData	= NULL;
		m_pAni	= NULL;
		D3DXMatrixIdentity(&m_matPos);
		m_bIsVisible	= false;
	}
};

#endif