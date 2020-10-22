#if defined(__ROCKCHKSUM_H_INCLUDED__)
#define __ROCKCHKSUM_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CRockChksum  
{
public:
	CRockChksum();
	virtual ~CRockChksum();

	void	Cleanup();

	void	CreateChksumData(  char* pszFullPath );
	void	GenerateChksum( char* pszChksumOut, int sizeOfOutBuffer );

public:
	char	m_szModuleName[MAX_PATH];
	DWORD	m_dwModuleSize;
	DWORD	m_dwCompiledDate;
	DWORD	m_dwAddressOfEntryPoint;
	DWORD	m_dwAddressOfImportTable;
};

#endif




