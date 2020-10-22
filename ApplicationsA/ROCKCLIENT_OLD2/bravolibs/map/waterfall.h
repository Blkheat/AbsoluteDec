
#ifndef	_WATERFALL_H_
#define	_WATERFALL_H_

#include "..\\camera.h"
#include "d3drenderobj.h"

const int	BEWATERSIZE		= 128;
const int	ETEXTURESIZE	= 64;

class	CEffectiveWater	: public CRenderObj
{
protected:
	char		m_lutDisplacement[(ETEXTURESIZE*2)];
	int			m_nBuffer[ETEXTURESIZE*ETEXTURESIZE];
	int			m_nBuffer2[ETEXTURESIZE*ETEXTURESIZE];
	int	*		m_pWaterActive;
	int	*		m_pWaterScratch;

	bool		m_bIsVisible;
	D3DXMATRIX	m_matPos;
	bool		m_bProcess;
	D3DXMATRIX	m_matTexPos;
// external	control class

public:

	BYTE		m_byRunsDir;
	bool		IsVisible()								{ return m_bIsVisible; }
	void		SetVisible(bool bVisible)				{ m_bIsVisible	= bVisible; }
	void		SetProcess(bool bProcess)				{ m_bProcess	= bProcess; }
	void		BuildTable(const float& fRefractionIndex, const float& fDepth);
	void		Process();
	HRESULT		BuildFinal();
	void		BuildDroplet(int iX, int iY, int iSize, int iSplashStrength);	
	void		SetDefaultTexture()						{ m_pFinal = m_pTexture; }
#ifdef DIRECT_VERSION_9_MJH
	void		SetTexture(LPDIRECT3DTEXTURE9 ptex)		{ m_pFinal = ptex; }
	void		SetBaseTexture(LPDIRECT3DTEXTURE9 pTex)	{ m_pTexture = (pTex)?pTex:m_pTexture; }
#else
	void		SetTexture(LPDIRECT3DTEXTURE8 ptex)		{ m_pFinal = ptex; }
	void		SetBaseTexture(LPDIRECT3DTEXTURE8 pTex)	{ m_pTexture = (pTex)?pTex:m_pTexture; }
#endif // DIRECT_VERSION_9_MJH

	void		SetPosTM(const float& x, const float& y, const float& z)	{ m_matPos._41 = x; m_matPos._42 = y; m_matPos._43 = z; }
	void		SetPosTM(const D3DXMATRIX& mat)	{ m_matPos = mat; }
	void		SetHeight(const float& height)	{ m_matPos._42	= height; }

public:
#ifdef DIRECT_VERSION_9_MJH
	HRESULT		OneTimeSceneInit(const float& x, const float& y, const float& z, LPDIRECT3DTEXTURE9 pTex);
	HRESULT		InitDeviceObjects(LPDIRECT3DDEVICE9 pd3ddsev);
#else
	HRESULT		OneTimeSceneInit(const float& x, const float& y, const float& z, LPDIRECT3DTEXTURE8 pTex);
	HRESULT		InitDeviceObjects(LPDIRECT3DDEVICE8 pd3ddsev);
#endif // DIRECT_VERSION_9_MJH
	HRESULT		RestoreDeviceObjects(float afMin = 0.0f, float afMax = BEWATERSIZE);
	HRESULT		InvalidateDeviceObjects();
	HRESULT		DeleteDeviceObjects();
	HRESULT		FinalCleanup();
	HRESULT		Render();
	HRESULT		FrameMove();

#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DTEXTURE9		m_pTexture;
	LPDIRECT3DTEXTURE9		m_pFinal;
#else
	LPDIRECT3DTEXTURE8		m_pTexture;
	LPDIRECT3DTEXTURE8		m_pFinal;
#endif // DIRECT_VERSION_9_MJH

	CEffectiveWater();
	~CEffectiveWater();
};


class	CBravoEffectiveWater
{
protected:
	CEffectiveWater	*	m_pWater;
	int					m_nSplit;
	CBravoCamera	*	m_pCamera;
	DWORD				m_dwPreCount;
	int					m_nCX, m_nCY;
	D3DXMATRIX			m_matPos;
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DDEVICE9	m_pd3dDevice;
#else
	LPDIRECT3DDEVICE8	m_pd3dDevice;
#endif // DIRECT_VERSION_9_MJH

public:

	BYTE				m_byRunsDir;

#ifdef DIRECT_VERSION_9_MJH
	HRESULT		Build(const int& mapsize, LPDIRECT3DDEVICE9 pd3ddev, CBravoCamera * pCamera, LPDIRECT3DTEXTURE9 pTex);
#else
	HRESULT		Build(const int& mapsize, LPDIRECT3DDEVICE8 pd3ddev, CBravoCamera * pCamera, LPDIRECT3DTEXTURE8 pTex);
#endif // DIRECT_VERSION_9_MJH
	HRESULT		FinalCleanup();
	HRESULT		Render();
	HRESULT		FrameMove();
	HRESULT		PlayWave(const float& x, const float& y);
	HRESULT		PlayWave(const D3DXMATRIX& matPos);
	HRESULT		UpdateCenter(const D3DXMATRIX& matPos);
	HRESULT		UpdateCenter(const float& x, const float& y);

	void		UpdateHeight(const float& height)	{ m_matPos._42	= height; }
#ifdef DIRECT_VERSION_9_MJH
	HRESULT		ChangeTexture(LPDIRECT3DTEXTURE9 pTex);
#else
	HRESULT		ChangeTexture(LPDIRECT3DTEXTURE8 pTex);
#endif // DIRECT_VERSION_9_MJH
	void		SetRunsDir(BYTE runsdir);

protected:
#ifdef DIRECT_VERSION_9_MJH
	HRESULT		OneTimeSceneInit(LPDIRECT3DTEXTURE9 pTex);
	HRESULT		InitDeviceObjects(LPDIRECT3DDEVICE9 pd3ddev);
#else
	HRESULT		OneTimeSceneInit(LPDIRECT3DTEXTURE8 pTex);
	HRESULT		InitDeviceObjects(LPDIRECT3DDEVICE8 pd3ddev);
#endif // DIRECT_VERSION_9_MJH
	HRESULT		RestoreDeviceObjects();
	HRESULT		InvalidateDeviceObjects();
	HRESULT		DeleteDeviceObjects();

public:
	~CBravoEffectiveWater();
};


class	CBravoWaterfall	: public CRenderObj
{
protected:
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DTEXTURE9	m_pTexture;
	LPDIRECT3DTEXTURE9	*	m_pFallTexture;
#else
	LPDIRECT3DTEXTURE8	m_pTexture;
	LPDIRECT3DTEXTURE8	*	m_pFallTexture;
#endif // DIRECT_VERSION_9_MJH
	D3DXMATRIX			m_matTexPos;
//	CEffectiveWater		m_Water;

	bool				m_bIsVisible;
	bool				m_bEnable;
	DWORD				m_dwPreCount;
	unsigned short		m_nTexIndex;
	unsigned char		m_byCurrentFall;

protected:
#ifdef DIRECT_VERSION_9_MJH
	HRESULT	OneTimeSceneInit(LPDIRECT3DTEXTURE9 pTex, LPDIRECT3DTEXTURE9* pFallTex);
	HRESULT	InitDeviceObjects(LPDIRECT3DDEVICE9 pd3ddev);
#else
	HRESULT	OneTimeSceneInit(LPDIRECT3DTEXTURE8 pTex, LPDIRECT3DTEXTURE8* pFallTex);
	HRESULT	InitDeviceObjects(LPDIRECT3DDEVICE8 pd3ddev);
#endif // DIRECT_VERSION_9_MJH
	HRESULT	RestoreDeviceObjects();
	HRESULT	InvalidateDeviceObjects();
	HRESULT	DeleteDeviceObjects();

public:
#ifdef DIRECT_VERSION_9_MJH
	HRESULT	Build(LPDIRECT3DDEVICE9 pd3ddev, LPDIRECT3DTEXTURE9 pTex, LPDIRECT3DTEXTURE9* pFallTex, const unsigned short& idx);
#else
	HRESULT	Build(LPDIRECT3DDEVICE8 pd3ddev, LPDIRECT3DTEXTURE8 pTex, LPDIRECT3DTEXTURE8* pFallTex, const unsigned short& idx);
#endif // DIRECT_VERSION_9_MJH
	HRESULT	Render();
	HRESULT	FrameMove();
	HRESULT	FinalCleanup();

	//void	SetPosTM(const float& x, const float& y, const float& z)	{ m_matPos._41 = x; m_matPos._42 = y; m_matPos._43 = z; m_Water.SetPosTM(x,y,z); }
	void	SetPosTM(const float& x, const float& y, const float& z)	{ m_matPos._41 = x; m_matPos._42 = y; m_matPos._43 = z; }
	//void	SetPosTM(const D3DXMATRIX& mat)	{ m_matPos = mat; m_Water.SetPosTM(mat); }
	void	SetPosTM(const D3DXMATRIX& mat)	{ m_matPos = mat; }
	D3DXMATRIX	GetPosTM()	{ return m_matPos; }
	bool	Enable()	{ return m_bEnable; }
	bool	IsVisible()	{ return m_bIsVisible; }
	unsigned short	GetTexIndex()	{ return m_nTexIndex; }
	void	SetVisible(bool bVisible)		{ m_bIsVisible = bVisible; }
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DTEXTURE9	GetTexture()		{ return	m_pTexture; }
	LPDIRECT3DTEXTURE9*	GetFallTexture()	{ return	m_pFallTexture; }
#else
	LPDIRECT3DTEXTURE8	GetTexture()		{ return	m_pTexture; }
	LPDIRECT3DTEXTURE8*	GetFallTexture()	{ return	m_pFallTexture; }
#endif // DIRECT_VERSION_9_MJH

	void				SetupScalingX(float val);
	void				SetupScalingY(float val);
	void				SetupScalingZ(float val);
	void				SetupRotationX(float angle);
	void				SetupRotationY(float angle);
	void				SetupRotationZ(float angle);
	void				SetupTranslationX(float x);
	void				SetupTranslationY(float y);
	void				SetupTranslationZ(float z);


	CBravoWaterfall();
	~CBravoWaterfall();
};


class	CWaterWave	: public CRenderObj
{
protected:
	DWORD			m_dwPreCount;
#ifdef DIRECT_VERSION_9_MJH
	D3DMATERIAL9	m_Material;
#else
	D3DMATERIAL8	m_Material;
#endif // DIRECT_VERSION_9_MJH
	float			m_fScale;
	bool			m_bIsPlaying;
	float			m_fX, m_fY, m_fZ;

public:
#ifdef DIRECT_VERSION_9_MJH
	HRESULT		Build(IDirect3DDevice9* pDev);
#else
	HRESULT		Build(IDirect3DDevice8* pDev);
#endif // DIRECT_VERSION_9_MJH
	HRESULT		Render();
	HRESULT		FrameMove();
	bool		IsPlaying()	{ return m_bIsPlaying; }
	void		SetPlayEnable();
	void		SetPosTM(const float& x, const float& y, const float& z);

public:
	HRESULT		OneTimeSceneInit();
#ifdef DIRECT_VERSION_9_MJH
	HRESULT		InitDeviceObjects(LPDIRECT3DDEVICE9 pd3ddev);
#else
	HRESULT		InitDeviceObjects(LPDIRECT3DDEVICE8 pd3ddev);
#endif // DIRECT_VERSION_9_MJH
	HRESULT		RestoreDeviceObjects();
	HRESULT		InvalidateDeviceObjects();
	HRESULT		DeleteDeviceObjects();
	HRESULT		FinalCleanup();

	CWaterWave();
};

class	CSimpleWave
{
protected:
	CWaterWave		m_Wave[3];
	DWORD			m_dwPreCount;
	BYTE			m_byPlayStep;
public:
	CWaterWave		GetLastWave()	{ return m_Wave[2]; }
	HRESULT			PlayWave();
#ifdef DIRECT_VERSION_9_MJH
	HRESULT			Build(LPDIRECT3DDEVICE9 pd3dDev);
#else
	HRESULT			Build(LPDIRECT3DDEVICE8 pd3dDev);
#endif // DIRECT_VERSION_9_MJH
	HRESULT			FinalCleanup();
	void			SetPlayEnable();
	void			SetPosition(const float& x, const float& y, const float& z);
};

#define	MAX_WATERWAVE	128

class	CWaveList
{
protected:
	CSimpleWave		m_List[MAX_WATERWAVE];

public:
#ifdef DIRECT_VERSION_9_MJH
	HRESULT			BuildAllUnits(LPDIRECT3DDEVICE9 pd3dDev);
#else
	HRESULT			BuildAllUnits(LPDIRECT3DDEVICE8 pd3dDev);
#endif // DIRECT_VERSION_9_MJH
	HRESULT			AddWave(const D3DXMATRIX& matPos);
	HRESULT			AddWave(const float& x, const float& y, const float& z);
	HRESULT			PlayAll();
	HRESULT			FinalCleanup();

	~CWaveList();
};

class	CCubeSky	: public CRenderObj
{
protected:
	float		m_fRadius;
	float		m_fLow;

	HRESULT		OneTimeSceneInit();
	HRESULT		RestoreDeviceObjects();
#ifdef DIRECT_VERSION_9_MJH
	HRESULT		InitDeviceObjects(LPDIRECT3DDEVICE9 pd3dDev);
#else
	HRESULT		InitDeviceObjects(LPDIRECT3DDEVICE8 pd3dDev);
#endif // DIRECT_VERSION_9_MJH
	HRESULT		InvalidateDeviceObjects();
	HRESULT		DeleteDeviceObjects();

public:
	HRESULT		FinalCleanup();
	HRESULT		FrameMove();
#ifdef DIRECT_VERSION_9_MJH
	HRESULT		Build(LPDIRECT3DDEVICE9 pd3dDev);
	HRESULT		Render(LPDIRECT3DTEXTURE9 pd3dTex);
#else
	HRESULT		Build(LPDIRECT3DDEVICE8 pd3dDev);
	HRESULT		Render(LPDIRECT3DTEXTURE8 pd3dTex);
#endif // DIRECT_VERSION_9_MJH
};


#endif


/*
#ifndef		_____WATERFALL_____
#define		_____WATERFALL_____

#include	"d3dfvf.h"

class		CWaterfall
{
	LPDIRECT3DDEVICE8	m_pDevice;
	DWORD				m_dwFVF;

	LPDIRECT3DVERTEXBUFFER8	m_pVB;
	LPDIRECT3DINDEXBUFFER8	m_pIB;
	LPDIRECT3DTEXTURE8		m_pTexture;
	D3DXMATRIX				m_matPos;
	D3DXMATRIX				m_matTexPos;

	FVF_PT	*			m_pVtxList;
	WORD	*			m_pIdxList;

	LONG				m_lPreCount;
	short				m_nFps;
	float				m_fDistance;

public:
	bool				m_bIsVisible;
	bool				InitDevice(LPDIRECT3DDEVICE8 pdev);
	LPDIRECT3DDEVICE8	GetDevice()	{ return m_pDevice; };
	bool				Build();
	bool				Update();
	bool				Display();

	void				FinalCleanup();
	
	
	void				SetupScalingX(float val);
	void				SetupScalingY(float val);
	void				SetupScalingZ(float val);
	void				SetupRotationX(float angle);
	void				SetupRotationY(float angle);
	void				SetupRotationZ(float angle);
	void				SetupTranslationX(float x);
	void				SetupTranslationY(float y);
	void				SetupTranslationZ(float z);
	void				SetupTransTM(float x, float y, float z);
	void				SetupPosTM(D3DXMATRIX mat)	{ m_matPos	= mat; };
	D3DXMATRIX			GetPosTM()			{ return m_matPos; };

	CWaterfall();
	~CWaterfall();
};


#endif
  */