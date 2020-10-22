#if 0
#include "RockPCH.h"
#include "RockChksum.h"


#pragma warning ( disable : 4244 )


#define	IMAGE_DOS_SIGNATURE		0x5A4D 

#define	BINARY_CHKSUM_SIZE		50


CRockChksum::CRockChksum()
{
	Cleanup();
}

CRockChksum::~CRockChksum()
{

}

void	CRockChksum::Cleanup()
{
	ZeroMemory( m_szModuleName, sizeof( m_szModuleName ) );
	m_dwModuleSize = 0;
	m_dwCompiledDate = 0;
	m_dwAddressOfEntryPoint = 0;
	m_dwAddressOfImportTable = 0;
}

void	CRockChksum::CreateChksumData(  char* pszFullPath )
{
	Cleanup();

	char* p = pszFullPath;
	p += lstrlen( pszFullPath );
	while( p != pszFullPath )
	{
		if( *p == '\\' ) break;
		--p;
	}
	if( p == pszFullPath ) return;
	++p;
	
	lstrcpy( m_szModuleName, p );

	HANDLE hFile = CreateFile(  pszFullPath,   
								GENERIC_READ,              // open for reading 
								FILE_SHARE_READ,           // share for reading 
								NULL,                      // no security 
								OPEN_EXISTING,             // existing file only 
								FILE_ATTRIBUTE_NORMAL,     // normal file 
								NULL );
	
	if( hFile == ( HANDLE )0xFFFFFFFF )	return;

	m_dwModuleSize = GetFileSize( hFile, NULL );

	BYTE* pBaseAddr = new BYTE[m_dwModuleSize];
	ULONG dwRet;
	ReadFile( hFile, ( LPVOID )pBaseAddr, m_dwModuleSize, &dwRet, NULL );
	CloseHandle( hFile );

	PIMAGE_DOS_HEADER pDosHdr = ( PIMAGE_DOS_HEADER )pBaseAddr;
	if( pDosHdr->e_magic != IMAGE_DOS_SIGNATURE ) return;

	PIMAGE_NT_HEADERS pNtHdr = ( PIMAGE_NT_HEADERS )( pBaseAddr + pDosHdr->e_lfanew );

	m_dwCompiledDate = pNtHdr->FileHeader.TimeDateStamp;
	m_dwAddressOfEntryPoint = pNtHdr->OptionalHeader.AddressOfEntryPoint;
	m_dwAddressOfImportTable = pNtHdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;

	delete [] pBaseAddr;
}

void	CRockChksum::GenerateChksum( char* pszChksumOut, int sizeOfOutBuffer )
{
	ZeroMemory( pszChksumOut, sizeOfOutBuffer );
	if( sizeOfOutBuffer <= BINARY_CHKSUM_SIZE * 3 ) return;

	char szModuleName[20];
	ZeroMemory( szModuleName, sizeof( szModuleName ) ); 
	strncpy( szModuleName, ( LPCTSTR )m_szModuleName, sizeof( szModuleName ) ); /// 실행 파일명은 20 Byte까지만 인정 

	/// 랜덤 2 Byte + ImportTable 하위 2 Byte + 랜덤 1 Byte + 실행파일명 전반부 10 Byte + 랜덤 1 Byte + 
	///	EntryPoint 상위 2 Byte + 랜덤 1 Byte + ModuleSize 4 Byte + 랜덤 1 Byte + CompiledDate 하위 2 Byte + 랜덤 2 Byte + 
	///	실행파일명 후반부 10 Byte + 랜덤 1 Byte + CompiledDate 상위 2 Byte + 랜덤 2 Byte + 
	///	ImportTable 상위 2 Byte + 랜덤 2 Byte + EntryPoint 하위 2 Byte + 랜덤 1 Byte
	/// = 50 Bytes 
	BYTE binChksum[BINARY_CHKSUM_SIZE];
	int pos = 0;

	WORD wVar;
	BYTE byVar;

	wVar = rand();	/// 랜덤 2 Byte
	memcpy( &(binChksum[pos]), &wVar, sizeof( wVar ) );				pos += sizeof( wVar );
	wVar = m_dwAddressOfImportTable & 0xFFFF;	/// ImportTable 하위 2 Byte
	memcpy( &(binChksum[pos]), &wVar, sizeof( wVar ) );				pos += sizeof( wVar );
	byVar = rand() & 0xFF;	/// 랜덤 1 Byte
	memcpy( &(binChksum[pos]), &byVar, sizeof( byVar ) );			pos += sizeof( byVar );
	/// 실행파일명 전반부 10 Byte
	memcpy( &(binChksum[pos]), szModuleName, sizeof( char ) * 10 );	pos += ( sizeof( char ) * 10 );
	byVar = rand() & 0xFF;	/// 랜덤 1 Byte
	memcpy( &(binChksum[pos]), &byVar, sizeof( byVar ) );			pos += sizeof( byVar );
	wVar = ( m_dwAddressOfEntryPoint >> 16 ) & 0xFFFF;	/// EntryPoint 상위 2 Byte
	memcpy( &(binChksum[pos]), &wVar, sizeof( wVar ) );				pos += sizeof( wVar );
	byVar = rand() & 0xFF;	/// 랜덤 1 Byte
	memcpy( &(binChksum[pos]), &byVar, sizeof( byVar ) );			pos += sizeof( byVar );
	/// ModuleSize 4 Byte
	memcpy( &(binChksum[pos]), &m_dwModuleSize, sizeof( m_dwModuleSize ) );		pos += sizeof( m_dwModuleSize );
	byVar = rand() & 0xFF;	/// 랜덤 1 Byte
	memcpy( &(binChksum[pos]), &byVar, sizeof( byVar ) );			pos += sizeof( byVar );
	wVar = m_dwCompiledDate & 0xFFFF;	/// CompiledDate 하위 2 Byte
	memcpy( &(binChksum[pos]), &wVar, sizeof( wVar ) );				pos += sizeof( wVar );
	wVar = rand();	/// 랜덤 2 Byte
	memcpy( &(binChksum[pos]), &wVar, sizeof( wVar ) );				pos += sizeof( wVar );
	/// 실행파일명 후반부 10 Byte
	memcpy( &(binChksum[pos]), &( szModuleName[10] ), sizeof( char ) * 10 );	pos += ( sizeof( char ) * 10 );
	byVar = rand() & 0xFF;	/// 랜덤 1 Byte
	memcpy( &(binChksum[pos]), &byVar, sizeof( byVar ) );			pos += sizeof( byVar );
	wVar = ( m_dwCompiledDate >> 16 ) & 0xFFFF;	/// CompiledDate 상위 2 Byte
	memcpy( &(binChksum[pos]), &wVar, sizeof( wVar ) );				pos += sizeof( wVar );
	wVar = rand();	/// 랜덤 2 Byte
	memcpy( &(binChksum[pos]), &wVar, sizeof( wVar ) );				pos += sizeof( wVar );
	wVar = ( m_dwAddressOfImportTable >> 16 ) & 0xFFFF;	/// ImportTable 상위 2 Byte
	memcpy( &(binChksum[pos]), &wVar, sizeof( wVar ) );				pos += sizeof( wVar );
	wVar = rand();	/// 랜덤 2 Byte
	memcpy( &(binChksum[pos]), &wVar, sizeof( wVar ) );				pos += sizeof( wVar );
	wVar = m_dwAddressOfEntryPoint & 0xFFFF;	/// EntryPoint 하위 2 Byte
	memcpy( &(binChksum[pos]), &wVar, sizeof( wVar ) );				pos += sizeof( wVar );
	byVar = rand() & 0xFF;	/// 랜덤 1 Byte
	memcpy( &(binChksum[pos]), &byVar, sizeof( byVar ) );			pos += sizeof( byVar );

	char temp[4];
	int i = 0;
	for( ; i < BINARY_CHKSUM_SIZE; ++i )
	{
		wsprintf( temp, "%03u", binChksum[i] );
		lstrcat( pszChksumOut, temp );
	}
}


#endif