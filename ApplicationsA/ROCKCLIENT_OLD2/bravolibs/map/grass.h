#ifndef		______GRASS______
#define		______GRASS______

class		CGrass
{
public:
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DDEVICE9		m_pDevice;
#else
	LPDIRECT3DDEVICE8		m_pDevice;
#endif // DIRECT_VERSION_9_MJH
	DWORD					m_cCount;
	char **					m_szNameList;

public:
#ifdef DIRECT_VERSION_9_MJH
	HRESULT		Create(LPDIRECT3DDEVICE9 pDevice);
#else
	HRESULT		Create(LPDIRECT3DDEVICE8 pDevice);
#endif // DIRECT_VERSION_9_MJH
	bool		Load(DWORD idx);
	void		FinalCleanup();
	DWORD		GetCount()		{ return m_cCount; };
	char*		GetName(DWORD i )	{ return m_szNameList[i]; };
	DWORD		GetIndexByName(char* Name);

	HRESULT		Update();

	CGrass();
	~CGrass();
};

extern	CGrass		g_Grass;

#endif