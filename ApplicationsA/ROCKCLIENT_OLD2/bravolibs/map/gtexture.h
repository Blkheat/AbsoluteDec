#ifndef		_____GRASS_TEXTURE_H_____
#define		_____GRASS_TEXTURE_H_____


class		CEnvTexture
{
protected:
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DTEXTURE9	*	m_pTexList;
#else
	LPDIRECT3DTEXTURE8	*	m_pTexList;
#endif // DIRECT_VERSION_9_MJH
	short					m_nCount;

public:
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DTEXTURE9		GetTexture(const short& i)	
#else
	LPDIRECT3DTEXTURE8		GetTexture(const short& i)	
#endif // DIRECT_VERSION_9_MJH
	{ 
		if( i < 0 || i >= m_nCount ) return NULL;
		return m_pTexList[i]; 
	}

	short					GetCount()					{ return m_nCount; }
	short					CountTexture(char* type);
#ifdef DIRECT_VERSION_9_MJH
	bool					Create(LPDIRECT3DDEVICE9 pDevice, char* type);
#else
	bool					Create(LPDIRECT3DDEVICE8 pDevice, char* type);
#endif // DIRECT_VERSION_9_MJH
	void					FinalCleanup();

	CEnvTexture();
	~CEnvTexture();
};

class		CGTexture
{
protected:
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DTEXTURE9	*	m_pTexList;
#else
	LPDIRECT3DTEXTURE8	*	m_pTexList;
#endif // DIRECT_VERSION_9_MJH
	DWORD					m_cCount;

public:
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DTEXTURE9		GetTexture(DWORD i)
#else
	LPDIRECT3DTEXTURE8		GetTexture(DWORD i)
#endif // DIRECT_VERSION_9_MJH
	{ 
		if( i < 0 || i >= m_cCount ) return NULL;
		return m_pTexList[i]; 
	}

	DWORD					CountTexture();
#ifdef DIRECT_VERSION_9_MJH
	bool					Create(LPDIRECT3DDEVICE9 pDevice);
#else
	bool					Create(LPDIRECT3DDEVICE8 pDevice);
#endif // DIRECT_VERSION_9_MJH
	void					FinalCleanup();

	CGTexture();
	~CGTexture();

};

extern	CGTexture			g_GTexture;

class		CBravoSkyTexture
{
protected:
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DTEXTURE9	*	m_pTexList;
	LPDIRECT3DTEXTURE9	*	m_pEveTexList;
	LPDIRECT3DTEXTURE9	*	m_pNightTexList;
#else
	LPDIRECT3DTEXTURE8	*	m_pTexList;
	LPDIRECT3DTEXTURE8	*	m_pEveTexList;
	LPDIRECT3DTEXTURE8	*	m_pNightTexList;
#endif // DIRECT_VERSION_9_MJH

	short					m_nCount;
	char			**		m_szNameList;

public:
	/// by sooree
	short					CountTexture();
#ifdef DIRECT_VERSION_9_MJH
	bool					Create(LPDIRECT3DDEVICE9 pDevice);
	LPDIRECT3DTEXTURE9		GetTexture(const short& i)	{ if( m_pTexList == NULL) return NULL; return m_pTexList[i]; }
	LPDIRECT3DTEXTURE9		GetEveTexture(const short& i )	{ if( m_pEveTexList == NULL) return NULL; return m_pEveTexList[i]; }
	LPDIRECT3DTEXTURE9		GetNightTexture(const short& i)	{ if( m_pNightTexList == NULL) return NULL; return m_pNightTexList[i]; }
#else
	bool					Create(LPDIRECT3DDEVICE8 pDevice);
	LPDIRECT3DTEXTURE8		GetTexture(const short& i)	{ if( m_pTexList == NULL) return NULL; return m_pTexList[i]; }
	LPDIRECT3DTEXTURE8		GetEveTexture(const short& i )	{ if( m_pEveTexList == NULL) return NULL; return m_pEveTexList[i]; }
	LPDIRECT3DTEXTURE8		GetNightTexture(const short& i)	{ if( m_pNightTexList == NULL) return NULL; return m_pNightTexList[i]; }
#endif // DIRECT_VERSION_9_MJH
	void					FinalCleanup();
	short					GetCount()	{ return m_nCount; }
	short					GetIndexByName(char* name);
	char *					GetName(const short& i)		{ return m_szNameList[i]; }

	CBravoSkyTexture();
	~CBravoSkyTexture();
};

// extern	CBravoSkyTexture	g_SkyTexture;

class		CBravoCloudTexture
{
protected:
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DTEXTURE9	*	m_pTexList;
#else
	LPDIRECT3DTEXTURE8	*	m_pTexList;
#endif // DIRECT_VERSION_9_MJH
	short					m_nCount;

public:
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DTEXTURE9		GetTexture(const short& i)	{ return m_pTexList[i]; }
#else
	LPDIRECT3DTEXTURE8		GetTexture(const short& i)	{ return m_pTexList[i]; }
#endif // DIRECT_VERSION_9_MJH
	short					CountTexture();
#ifdef DIRECT_VERSION_9_MJH
	bool					Create(LPDIRECT3DDEVICE9 pDevice);
#else
	bool					Create(LPDIRECT3DDEVICE8 pDevice);
#endif // DIRECT_VERSION_9_MJH
	void					FinalCleanup();
	short					GetCount()		{ return m_nCount; }

	CBravoCloudTexture();
	~CBravoCloudTexture();
};

extern	CBravoCloudTexture	g_CloudTexture;

class		CBravoWaterTexture
{
protected:
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DTEXTURE9	*	m_pTexList;
#else
	LPDIRECT3DTEXTURE8	*	m_pTexList;
#endif // DIRECT_VERSION_9_MJH
	short					m_nCount;

public:
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DTEXTURE9		GetTexture(const short& i)
#else
	LPDIRECT3DTEXTURE8		GetTexture(const short& i)
#endif // DIRECT_VERSION_9_MJH
	{ 
		if( i < 0 || i >= m_nCount ) return NULL;
		return m_pTexList[i]; 
	}

	short					CountTexture();
#ifdef DIRECT_VERSION_9_MJH
	bool					Create(LPDIRECT3DDEVICE9 pDevice);
#else
	bool					Create(LPDIRECT3DDEVICE8 pDevice);
#endif // DIRECT_VERSION_9_MJH
	void					FinalCleanup();
	short					GetCount()		{ return m_nCount; }

	CBravoWaterTexture();
	~CBravoWaterTexture();
};

extern	CBravoWaterTexture	g_WaterTexture;


class	CBravoWaterfallTexture
{
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DTEXTURE9	*	m_pTexList;
	LPDIRECT3DTEXTURE9	**	m_pFallTexture;
	LPDIRECT3DTEXTURE9		m_pTextureWave;
#else
	LPDIRECT3DTEXTURE8	*	m_pTexList;
	LPDIRECT3DTEXTURE8	**	m_pFallTexture;
	LPDIRECT3DTEXTURE8		m_pTextureWave;
#endif // DIRECT_VERSION_9_MJH
	short					m_nCount;

public:
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DTEXTURE9		GetTexture(const short& i)	
#else
	LPDIRECT3DTEXTURE8		GetTexture(const short& i)	
#endif // DIRECT_VERSION_9_MJH
	{ 
		if( i < 0 || i >= m_nCount ) return NULL;
		return m_pTexList[i]; 
	}

	short					CountTexture();
#ifdef DIRECT_VERSION_9_MJH
	bool					Create(LPDIRECT3DDEVICE9 pDevice);
	LPDIRECT3DTEXTURE9	*	GetFallTexture(const short& i)	{ return m_pFallTexture[i]; }
	LPDIRECT3DTEXTURE9		GetWaveTexture()				{ return m_pTextureWave; }
#else
	bool					Create(LPDIRECT3DDEVICE8 pDevice);
	LPDIRECT3DTEXTURE8	*	GetFallTexture(const short& i)	{ return m_pFallTexture[i]; }
	LPDIRECT3DTEXTURE8		GetWaveTexture()				{ return m_pTextureWave; }
#endif // DIRECT_VERSION_9_MJH
	void					FinalCleanup();
	short					GetCount()		{ return m_nCount; }

	CBravoWaterfallTexture();
	~CBravoWaterfallTexture();
};
extern	CBravoWaterfallTexture	g_WaterfallTexture;

#endif