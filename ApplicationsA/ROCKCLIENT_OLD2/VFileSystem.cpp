#include "RockPCH.h"	/// RockBase를 쓰기 위한 PCH include

#include "VFileSystem.h"
#include "ZLib/zlib.h"	/// RockBase에 포함되어 있는 zlib 쓴다 

RB_USING
CRockCrypto		g_Crypto;

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

CVFileHandle::CVFileHandle()
{
	ZeroMemory(&m_FileContext, sizeof(FILECONTEXT));
	m_lSrtPos = 0;

	m_pData = NULL;
	m_lSize = 0;
	m_lCurPos = 0;
} 

CVFileHandle::~CVFileHandle()
{
	SAFE_DELETE_ARRAY( m_pData );	
}


int CVFileHandle::Getws(wchar_t* pBuff, long lSize)
{
	if(!m_pData) return 0;
	
	int i = -1;
	//BYTE *p = ;

	WCHAR *pData = (WCHAR*)&m_pData[m_lCurPos] ;

	while( ++i < lSize - 1 )
	{
		if( m_lSize <= m_lCurPos + i ) break;
		pBuff[i] = pData[i] ;
		if( pBuff[i] == '\n' ) break;
	}
	
	pBuff[i] = '\0';
	
	if( i  > 0 )
	{
		m_lCurPos += ( (i*2) + 2 );
		if( pBuff[i - 1] == '\r' ) pBuff[i - 1] = '\0';	/// \r\n 인 경우,  \r을 떼어내자 
	}
	
	return i;
}


int CVFileHandle::Gets(char* pBuff, long lSize)
{
	if(!m_pData) return 0;

	int i = -1;
	while( ++i < lSize - 1 )
	{
		if( m_lSize <= m_lCurPos + i ) break;
		pBuff[i] = ( char )( m_pData[m_lCurPos + i] );
		if( pBuff[i] == '\n' ) break;
	}

	pBuff[i] = '\0';
	if( i > 0 )
	{
		m_lCurPos += ( i + 1 );
		if( pBuff[i - 1] == '\r' ) pBuff[i - 1] = '\0';	/// \r\n 인 경우,  \r을 떼어내자 
	}

	return i;
}

int CVFileHandle::Read(void* pBuff, long lSize)
{
	if( !m_pData ) return 0;
	if( !pBuff ) return 0;

	if(m_lSize < m_lCurPos) return 0;
	if(m_lSize < m_lCurPos+lSize) lSize = m_lSize-m_lCurPos;

	memcpy(pBuff, m_pData+m_lCurPos, lSize);
	m_lCurPos += lSize;

	return lSize;
}

int CVFileHandle::Seek(long lOffset, int iOrigin)
{
	if(!m_pData) return -1;

	long lPos;
	switch(iOrigin)
	{
	case SEEK_SET: lPos = lOffset;			 break;
	case SEEK_CUR: lPos = lOffset+m_lCurPos; break;
	case SEEK_END: lPos = m_lSize+lOffset;	 break;
	default:								 return -1;
	}
	if(lPos < 0 || lPos > m_lSize) return -1;

	m_lCurPos = lPos;

	return 0;
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

CVFileSystem::CVFileSystem()
{
/*	/// 생성자에서 SetKey()하니 안먹네요 -_-;
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	0 : CStrmCrypto - 12 Byte Key	: Stream Bit Scramble Crypto
	BYTE key0[] = { 0x6a,  0x2c,  0x67,  0xe1,  0x42,  0x34,  0x93,  0xbc,  0x0a,  0x76,  0xc1,  0x0b };
	g_Crypto.SetKey( (const char *)key0,  NULL,  NULL,  NULL );
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/
///	srand( timeGetTime() );

	m_pFile = NULL;

	InitializeCriticalSection( &m_FSysFlag );

	CloseFileSystem();
}

CVFileSystem::~CVFileSystem()
{	
	CloseFileSystem();

	DeleteCriticalSection( &m_FSysFlag );
}

int CVFileSystem::FreadCrypto( void* buffer,  int size,  int count,  FILE* fp )
{
	int n = fread( buffer,  size,  count,  fp ); 
	g_Crypto.Decrypt( buffer,  size * n );
	return n;
}

int CVFileSystem::FwriteCrypto( void* buffer,  int size,  int count,  FILE* fp )
{
	BYTE temp[1024*32];	/// [주의] 암호화 버퍼. 헤더만 암호화한다는 전제. 최대 32K까지만 한번에 쓴다. 
	memcpy( temp,  buffer,  size * count );
	g_Crypto.Encrypt( temp,  size * count );
	return fwrite( temp,  size,  count,  fp );
}

void CVFileSystem::Encrypt( void* buffer,  int size )
{
	g_Crypto.Encrypt( buffer,  size );
}

void CVFileSystem::Decrypt( void* buffer,  int size )
{
	g_Crypto.Decrypt( buffer,  size );
}

void CVFileSystem::SetCurDir(const char* pTok)
{
	if(!strcmp(pTok, ".")) return;

	if(!strcmp(pTok, ".."))
	{
		if(strlen(m_strCurDir) == 1 && m_strCurDir[0] == '/') return;

		char strCurDir[_MAX_PATH];
		strcpy(strCurDir, m_strCurDir);

		int iLen = strlen(strCurDir);

		if(strCurDir[iLen-1] == '/') strCurDir[iLen-1] = '\0';

		for(int i=0;i<_MAX_PATH;i++)
		{
			char* cp = strCurDir+(_MAX_PATH-1-i);
			if(*cp == '/') break;
			*cp = '\0';
		}

		strcpy(m_strCurDir, strCurDir);
		return;
	}
	strcat(m_strCurDir, pTok );
	strcat(m_strCurDir, "/" );
}

void CVFileSystem::GetSafeName(char* strDst, const char* strName)
{
	ZeroMemory(strDst, _MAX_FNAME);

	int iLen = strlen(strName);
	if(iLen >= _MAX_FNAME) iLen = _MAX_FNAME-1;

	strncpy(strDst, strName, iLen);
}

BOOL CVFileSystem::SearchDir(const char* strName, CVFileHandle* pFH)
{
	for( ; ; )
	{
		fread( &(pFH->m_FileContext),  sizeof(FILECONTEXT),  1,  m_pFile );
		Decrypt( &(pFH->m_FileContext),  sizeof(FILECONTEXT) - sizeof(long) );	/// 암호화 : long lNext; 제외 

		if(!stricmp(strName, pFH->m_FileContext.strName)) return TRUE;

		pFH->m_FileContext.strName[0] = '\0';

		if(!pFH->m_FileContext.lNext) return FALSE;

		fseek(m_pFile, pFH->m_FileContext.lNext, SEEK_SET);
	}
}

void CVFileSystem::CloseFileSystem()
{
	if(m_pFile)
	{
		fclose(m_pFile);
		m_pFile = NULL;
	}

	for(VFileHandleList_i it=m_FileHandleList.begin();it!=m_FileHandleList.end();++it) 
	{
		CVFileHandle * pData = (*it);
		SAFE_DELETE( pData );
		(*it) = NULL;
	}
	
	m_FileHandleList.clear();

	for(int i=0;i<FS_MAX_OPEN;i++)
	{
		CloseFile(&m_HandleList[i]);
	}

	m_strCurDir[0] = '\0';
}

BOOL CVFileSystem::OpenFileSystem(const char* strPath)
{
	CloseFileSystem();

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	0 : CStrmCrypto - 12 Byte Key	: Stream Bit Scramble Crypto
	BYTE key0[] = { 0x6a,  0x2c,  0x67,  0xe1,  0x42,  0x34,  0x93,  0xbc,  0x0a,  0x76,  0xc1,  0x0b };
	g_Crypto.SetKey( (const char *)key0,  NULL,  NULL,  NULL );
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	m_pFile = fopen(strPath, "r+b");
	if(!m_pFile) return FALSE;

	char strHeader[FS_MAX_HEADER];
//	fread(strHeader, strlen(FS_HEADER_STR)+1, 1, m_pFile);
	FreadCrypto(strHeader, strlen(FS_HEADER_STR)+1, 1, m_pFile);	/// 암호화

	if(strcmp(strHeader, FS_HEADER_STR))
	{
		fclose(m_pFile);
		m_pFile = NULL;
		return FALSE;
	}

	ChangeDir("/");

	return TRUE;
}

BOOL CVFileSystem::NewFileSystem(const char* strPath)
{
	FILE* fp = fopen(strPath, "w+b");
	if(!fp) return FALSE;

	FILECONTEXT FileContext;
	ZeroMemory(&FileContext, sizeof(FILECONTEXT));
	FileContext.iType = FT_DIR;
	strcpy(FileContext.strName, ".");
	{
		for( int k = strlen(FileContext.strName) + 1; k < sizeof( FileContext.strName ); ++k )
			FileContext.strName[k] = rand() % 255;	/// 해킹 방지 
	}

	FileContext.lCompressSize = 0;
	FileContext.lUncompressSize = 0;
	FileContext.lSub = strlen(FS_HEADER_STR)+1; 
	FileContext.lNext = 0;

//	fwrite(FS_HEADER_STR, strlen(FS_HEADER_STR)+1, 1, fp);	
	FwriteCrypto(FS_HEADER_STR, strlen(FS_HEADER_STR)+1, 1, fp);	/// 암호화

	Encrypt( &FileContext,  sizeof(FILECONTEXT) - sizeof(long) );	/// 암호화 : long lNext; 제외 
	fwrite(&FileContext, sizeof(FILECONTEXT), 1, fp);


	fclose(fp);

	return TRUE;
}

void CVFileSystem::CloseFile(CVFileHandle* pFH)
{
	EnterCriticalSection( &m_FSysFlag );
	
	if(!m_pFile) 
	{
		LeaveCriticalSection( &m_FSysFlag );
		return;
	}

	ZeroMemory(&pFH->m_FileContext, sizeof(FILECONTEXT));

	SAFE_DELETE_ARRAY( pFH->m_pData );

	pFH->m_lSrtPos = 0;

	pFH->m_pData = NULL;
	pFH->m_lSize = 0;
	pFH->m_lCurPos = 0;

	LeaveCriticalSection( &m_FSysFlag );
}

CVFileHandle* CVFileSystem::OpenFile(const char* strPath)
{
	CVFileHandle * pVFileHandle = NULL; 
	
	EnterCriticalSection( &m_FSysFlag );
	
	if(!m_pFile)
	{
		LeaveCriticalSection( &m_FSysFlag );
		return NULL;
	}

	char strOldPath[_MAX_PATH];
	strcpy(strOldPath, m_strCurDir);

	char strCurPath[_MAX_PATH];
	GetPathOnly(strCurPath, strPath);

	if(strCurPath[0]) ChangeDir(strCurPath);

	char strName[_MAX_FNAME];
	GetNameOnly(strName, strPath);

	VFileHandleList_i it;
	for(it=m_FileHandleList.begin();it!=m_FileHandleList.end();++it)
	{
		if((*it)->m_FileContext.iType == FT_FILE)
		{
			if(!stricmp(strName, (*it)->m_FileContext.strName)) break;
		}
	}
	
	if(it == m_FileHandleList.end()) 
	{
		LeaveCriticalSection( &m_FSysFlag );
		return NULL;
	}

	int iHandle = -1;
	for(int i=0;i<FS_MAX_OPEN;i++)
	{
		if(!m_HandleList[i].m_FileContext.strName[0])
		{
			iHandle = i;
			break;
		}
	}

	if(iHandle < 0) 
	{
		LeaveCriticalSection( &m_FSysFlag );
		return NULL;
	}

	fseek(m_pFile, (*it)->m_lSrtPos+sizeof(FILECONTEXT), SEEK_SET);

	BYTE* pSrcData = NULL;
	BYTE* pDstData = NULL;

	long lSrcSize = (*it)->m_FileContext.lCompressSize;
	pSrcData = SAFE_NEW_ARRAY( BYTE , lSrcSize );
	
	long lDstSize = (*it)->m_FileContext.lUncompressSize;
	pDstData = SAFE_NEW_ARRAY( BYTE , lDstSize );

	if( pSrcData == NULL || pDstData == NULL )
	{
		LeaveCriticalSection( &m_FSysFlag );
		return NULL;
	}

	fread(pSrcData, lSrcSize, 1, m_pFile);
	
	if(uncompress(pDstData, (uLongf*)&lDstSize, pSrcData, lSrcSize) != Z_OK)
	{
		SAFE_DELETE_ARRAY( pSrcData );
		SAFE_DELETE_ARRAY( pDstData );

		LeaveCriticalSection( &m_FSysFlag );
		return NULL;
	}
	
	SAFE_DELETE_ARRAY( pSrcData );

	m_HandleList[iHandle].m_FileContext.iType = (*it)->m_FileContext.iType;
	strcpy(m_HandleList[iHandle].m_FileContext.strName, (*it)->m_FileContext.strName);
	m_HandleList[iHandle].m_FileContext.lCompressSize = lSrcSize;
	m_HandleList[iHandle].m_FileContext.lUncompressSize = lDstSize;
	m_HandleList[iHandle].m_FileContext.lSub = 0;
	m_HandleList[iHandle].m_FileContext.lNext = 0;
	m_HandleList[iHandle].m_pData = pDstData;
	m_HandleList[iHandle].m_lSize = lDstSize;
	m_HandleList[iHandle].m_lCurPos = 0;

	if(strCurPath[0]) ChangeDir(strOldPath);

	pVFileHandle = &m_HandleList[iHandle];

	LeaveCriticalSection( &m_FSysFlag );
	
	return pVFileHandle;
}

BOOL CVFileSystem::AddDir(const char* strName)
{
	if(!m_pFile) return FALSE;

	FILECONTEXT FileContext,  FileContextCryptBuf;
	ZeroMemory(&FileContext, sizeof(FILECONTEXT));

	FileContext.iType = FT_DIR;
	GetSafeName(FileContext.strName, strName);
	if(CheckNameExist(FileContext.strName)) return FALSE;
	{
		for( int k = strlen(FileContext.strName) + 1; k < sizeof( FileContext.strName ); ++k )
			FileContext.strName[k] = rand() % 255;	/// 해킹 방지 
	}
	fseek(m_pFile, 0, SEEK_END);
	long lNext = ftell(m_pFile);

	FileContext.lSub = ftell(m_pFile)+sizeof(FILECONTEXT);
	memcpy( &FileContextCryptBuf,  &FileContext,  sizeof(FILECONTEXT) );	/// 암호화 버퍼 
	Encrypt( &FileContextCryptBuf,  sizeof(FILECONTEXT) - sizeof(long) );	/// 암호화 : long lNext; 제외 
	fwrite(&FileContextCryptBuf, sizeof(FILECONTEXT), 1, m_pFile);

	ZeroMemory(FileContext.strName, _MAX_FNAME);
	strcpy(FileContext.strName, ".");
	{
		for( int k = strlen(FileContext.strName) + 1; k < sizeof( FileContext.strName ); ++k )
			FileContext.strName[k] = rand() % 255;	/// 해킹 방지 
	}
	FileContext.lNext = ftell(m_pFile)+sizeof(FILECONTEXT);
	memcpy( &FileContextCryptBuf,  &FileContext,  sizeof(FILECONTEXT) );	/// 암호화 버퍼 
	Encrypt( &FileContextCryptBuf,  sizeof(FILECONTEXT) - sizeof(long) );	/// 암호화 : long lNext; 제외 
	fwrite(&FileContextCryptBuf, sizeof(FILECONTEXT), 1, m_pFile);


	ZeroMemory(FileContext.strName, _MAX_FNAME);
	strcpy(FileContext.strName, ".." );
	{
		for( int k = strlen(FileContext.strName) + 1; k < sizeof( FileContext.strName ); ++k )
			FileContext.strName[k] = rand() % 255;	/// 해킹 방지 
	}
	CVFileHandle* pFFH = m_FileHandleList.front();
	FileContext.lSub = pFFH->m_lSrtPos;
	FileContext.lNext = 0;
	memcpy( &FileContextCryptBuf,  &FileContext,  sizeof(FILECONTEXT) );	/// 암호화 버퍼 
	Encrypt( &FileContextCryptBuf,  sizeof(FILECONTEXT) - sizeof(long) );	/// 암호화 : long lNext; 제외 
	fwrite(&FileContextCryptBuf, sizeof(FILECONTEXT), 1, m_pFile);

	CVFileHandle* pBFH = m_FileHandleList.back();
	fseek(m_pFile, pBFH->m_lSrtPos, SEEK_SET);
	fseek(m_pFile, FC_OFFSET_NEXT, SEEK_CUR);
	fwrite(&lNext, sizeof(long), 1, m_pFile);	/// 요 구조 때문에 lNext는 암호화하지 않는다 

	if(!ChangeDir(".")) return FALSE;

	return TRUE;
}

BOOL CVFileSystem::AddFile(const char* strPath)
{
	if(!m_pFile) return FALSE;

	FILE* fp = fopen(strPath, "rb");
	if(!fp) return FALSE;

	fseek(fp, 0, SEEK_END);
	long lSize = ftell(fp);
	if(!lSize)
	{
		fclose(fp);
		return TRUE;
	}

	fseek(fp, 0, SEEK_SET);
	BYTE* pData = NULL;
	pData = SAFE_NEW_ARRAY( BYTE , lSize );	

	if( pData == NULL )
	{
		return FALSE;
	}

	fread(pData, lSize, 1, fp);

	fclose(fp);

	char strName[_MAX_FNAME];
	GetNameOnly(strName, strPath);
	if(!AddFile(strName, pData, lSize))
	{
		SAFE_DELETE_ARRAY( pData );		
		return FALSE;
	}

	SAFE_DELETE_ARRAY( pData );		

	return TRUE;
}

BOOL CVFileSystem::AddFile(const char* strName, BYTE* pData, long lSize)
{
	if(!m_pFile) return FALSE;

	if(lSize <= 0) return TRUE;

	long lDstSize = lSize+12+max(1, (lSize+12)/1000);
	BYTE* pDstData = NULL;
	pDstData = SAFE_NEW_ARRAY( BYTE , lDstSize );	

	if( pDstData == NULL )
	{
		return FALSE;
	}

	if(compress(pDstData, (uLongf*)&lDstSize, pData, lSize) != Z_OK)
	{
		SAFE_DELETE_ARRAY( pDstData );		
		return FALSE;
	}

	FILECONTEXT FileContext,  FileContextCryptBuf;
	ZeroMemory(&FileContext, sizeof(FILECONTEXT));
	FileContext.iType = FT_FILE;
	GetNameOnly(FileContext.strName, strName);
	if(CheckNameExist(FileContext.strName))
	{
		SAFE_DELETE_ARRAY( pDstData );		
		return FALSE;
	}
	{
		for( int k = strlen(FileContext.strName) + 1; k < sizeof( FileContext.strName ); ++k )
			FileContext.strName[k] = rand() % 255;	/// 해킹 방지 
	}

	FileContext.lCompressSize = lDstSize;
	FileContext.lUncompressSize = lSize;

	fseek(m_pFile, 0, SEEK_END);
	long lNext = ftell(m_pFile);

	memcpy( &FileContextCryptBuf,  &FileContext,  sizeof(FILECONTEXT) );	/// 암호화 버퍼 
	Encrypt( &FileContextCryptBuf,  sizeof(FILECONTEXT) - sizeof(long) );	/// 암호화 : long lNext; 제외 
	fwrite(&FileContextCryptBuf, sizeof(FILECONTEXT), 1, m_pFile);
	fwrite(pDstData, lDstSize, 1, m_pFile);

	SAFE_DELETE_ARRAY( pDstData );	

	CVFileHandle* pBFH = m_FileHandleList.back();
	pBFH->m_FileContext.lNext = lNext;
	fseek(m_pFile, pBFH->m_lSrtPos, SEEK_SET);
	fseek(m_pFile, FC_OFFSET_NEXT, SEEK_CUR);
	fwrite(&lNext, sizeof(lNext), 1, m_pFile);

	CVFileHandle* pFH = NULL;
	pFH = SAFE_NEW( CVFileHandle );	
	
	if( pFH == NULL )
	{
		return FALSE;
	}

	*pFH->GetFileContext() = FileContext;
	pFH->m_lSrtPos = lNext;
	pFH->m_lCurPos = 0;
	m_FileHandleList.push_back(pFH);

	return TRUE;
}

BOOL CVFileSystem::Remove(const char* strName)
{
	if(!m_pFile) return FALSE;

	if(!strcmp(strName, ".") || !strcmp(strName, "..")) return FALSE;

	for(VFileHandleList_i it=m_FileHandleList.begin();it!=m_FileHandleList.end();++it)
	{
		if(!stricmp(strName, (*it)->m_FileContext.strName))
		{
			long lNext = (*it)->m_FileContext.lNext;

			--it;
			(*it)->m_FileContext.lNext = lNext;
			fseek(m_pFile, (*it)->m_lSrtPos, SEEK_SET);
			fseek(m_pFile, FC_OFFSET_NEXT, SEEK_CUR);
			fwrite(&lNext, sizeof(long), 1, m_pFile);

			++it;
			
			CVFileHandle * pData = (*it); 
			SAFE_DELETE( pData );
			(*it) = NULL;
			
			m_FileHandleList.erase(it);
			break;
		}
	}

	return TRUE;
}

BOOL CVFileSystem::Rename(const char* strName, const char* strRename)
{
	if(!m_pFile) return FALSE;

	if(CheckNameExist(strRename)) return FALSE;
	if(!stricmp(strName, strRename)) return TRUE;
	if(!strcmp(strName, ".") || !strcmp(strName, "..")) return FALSE;

	for(VFileHandleList_i it=m_FileHandleList.begin();it!=m_FileHandleList.end();++it)
	{
        if(!stricmp(strName, (*it)->m_FileContext.strName))
		{
			GetSafeName((*it)->m_FileContext.strName, strRename);

			long lPos = ftell(m_pFile);
//			fseek(m_pFile, (*it)->m_lSrtPos+FC_OFFSET_NAME, SEEK_SET);
//			fwrite((*it)->m_FileContext.strName, _MAX_FNAME, 1, m_pFile);
			/// 암호화 블록 단위는 동일해야 한다. : m_FileContext.strName만 따로 암호화해서 저장하면 안됨 
			fseek(m_pFile,  (*it)->m_lSrtPos,  SEEK_SET);
			FILECONTEXT FileContextCryptBuf;
			memcpy( &FileContextCryptBuf,  (*it)->GetFileContext(),  sizeof( FILECONTEXT ) ); 
			Encrypt( &FileContextCryptBuf,  sizeof(FILECONTEXT) - sizeof(long) );		/// 암호화 : long lNext; 제외 
			fwrite( &FileContextCryptBuf,  sizeof(FILECONTEXT),  1,  m_pFile );

			fseek(m_pFile, lPos, SEEK_SET);

			return TRUE;
		}
	}
	
	return FALSE;
}

BOOL CVFileSystem::ChangeDir(const char* strPath)
{
	if(!m_pFile) return FALSE;

	if(!strPath || !strlen(strPath)) return TRUE;

	char strLastPath[_MAX_PATH];	
	int n;
	if( ( n = strlen( m_strCurDir ) ) > 0 )
	{
		strcpy( strLastPath,  m_strCurDir);
		if( strLastPath[ n - 1  ] == '/' )
		{
			strLastPath[ n - 1  ] = '\0';
			if( strcmp( strPath,  strLastPath ) == 0 ) return TRUE;
		}
	}
	if( strcmp( strPath,  m_strCurDir ) == 0 ) return TRUE;

	long lPos = ftell(m_pFile);

	if(m_FileHandleList.size())
	{
		CVFileHandle* pFFH = m_FileHandleList.front();
		fseek(m_pFile, pFFH->m_lSrtPos, SEEK_SET);
	}

	char strTemp[_MAX_PATH];
	strcpy(strTemp, strPath );

	if(strTemp[0] == '/')
	{
		fseek(m_pFile, strlen(FS_HEADER_STR)+1, SEEK_SET);
		strcpy(m_strCurDir, "/");
	}

	char* pTok;
	CVFileHandle FH;
	for(pTok=strtok(strTemp, "\\/\x0\n\r\t");pTok;pTok=strtok(NULL, "\\/\x0\n\r\t"))
	{
		if(!SearchDir(pTok, &FH))
		{
			fseek(m_pFile, lPos, SEEK_SET);
			return FALSE;
		}

		fseek(m_pFile, FH.m_FileContext.lSub, SEEK_SET);
		SetCurDir(pTok);
	}

	for(VFileHandleList_i it=m_FileHandleList.begin();it!=m_FileHandleList.end();++it) 
	{
		CVFileHandle * pData = (*it);
		SAFE_DELETE( pData );
		(*it) = NULL;
	}

	m_FileHandleList.clear();

	lPos = ftell(m_pFile);

	CVFileHandle* pFH = NULL;
	for(long lSrtPos=lPos;lSrtPos;lSrtPos=pFH->m_FileContext.lNext)
	{
		pFH = SAFE_NEW( CVFileHandle );		

		fread( &(pFH->m_FileContext),  sizeof(FILECONTEXT),  1,  m_pFile );
		Decrypt( &(pFH->m_FileContext),  sizeof(FILECONTEXT) - sizeof(long) );	/// 암호화 : long lNext; 제외 
		fseek(m_pFile, pFH->m_FileContext.lNext, SEEK_SET);
		pFH->m_lSrtPos = lSrtPos;
		pFH->m_lCurPos = 0;
		m_FileHandleList.push_back(pFH);
	}

	fseek(m_pFile, lPos, SEEK_SET);

	return TRUE;
}

BOOL CVFileSystem::CheckNameExist(const char* strName)
{
	for(VFileHandleList_i it=m_FileHandleList.begin();it!=m_FileHandleList.end();++it)
	{
		if(!stricmp(strName, (*it)->m_FileContext.strName)) return TRUE;
	}

	return FALSE;
}

void CVFileSystem::GetNameOnly(char* strDst, const char* strPath)
{
	char strTemp[_MAX_PATH];
	strcpy(strTemp, strPath);
	strrev(strTemp);

	char* cp = strtok(strTemp, "\\/\x0\n\r\t");
	strcpy(strTemp, cp);
	strrev(strTemp);
	GetSafeName(strDst, strTemp);
}

void CVFileSystem::GetPathOnly(char* strDst, const char* strPath)
{
	char strName[_MAX_FNAME];
	GetNameOnly(strName, strPath);

	int iLen = strlen(strPath)-strlen(strName);

	if(!iLen)
	{
		strDst[0] = '\0';
	}
	else
	{
		strncpy(strDst, strPath, iLen);
		strDst[iLen] = '\0';
	}
}

////////////////////////////////////////////////////////////////////////////

/// Virtual File 에서 file을 Remove하면 header만 지우고 body data는 그대로 있다. 그걸 추려내기 위해 optimize
BOOL CVFileSystem::OptimizeFileSystem(const char* strPath)
{
	if(!m_pFile) return FALSE;

	CVFileSystem FileSystem;
	if(!FileSystem.NewFileSystem(strPath) || !FileSystem.OpenFileSystem(strPath)) return FALSE;

	char strOldPath[_MAX_PATH];
	strcpy(strOldPath, m_strCurDir);

	ChangeDir("/");

	VFileHandleList_i it;
	for(unsigned int i=0;i<m_FileHandleList.size();i++)
	{
		it = m_FileHandleList.begin();
		for(unsigned int j=0;j<i;j++) ++it;

		if((*it)->m_FileContext.iType == FT_DIR)
		{
			if(strcmp((*it)->m_FileContext.strName, ".") && strcmp((*it)->m_FileContext.strName, ".."))
			{
				char strTemp[_MAX_FNAME];
				strcpy(strTemp, (*it)->m_FileContext.strName);
				if(!OptimizeDir(&FileSystem, strTemp)) return FALSE; // 미결 - 분석요망
			}
		}
		else if((*it)->m_FileContext.iType == FT_FILE)
		{
			CVFileHandle* pFH = OpenFileForOptimize((*it)->m_FileContext.strName);
			if(!pFH->m_lSize) continue;

			BYTE* pData = NULL;
			pData = SAFE_NEW_ARRAY( BYTE , pFH->m_lSize );		

			if( pData == NULL )
			{
				return FALSE;
			}

			if(!pFH->Read(pData, pFH->m_lSize))
			{
				SAFE_DELETE_ARRAY( pData );		
				CloseFile(pFH);
				return FALSE;
			}

			CloseFile(pFH);

			if(!FileSystem.AddFileForOptimize((*it)->m_FileContext.strName, pData, (*it)->m_FileContext.lCompressSize, (*it)->m_FileContext.lUncompressSize))
			{
				SAFE_DELETE_ARRAY( pData );				
				return FALSE;
			}

			SAFE_DELETE_ARRAY( pData );			
		}
	}

	FileSystem.CloseFileSystem();
	ChangeDir(strOldPath);
	
	return TRUE;
}

BOOL CVFileSystem::OptimizeDir(CVFileSystem* pFS, const char* strName)
{
	if(!ChangeDir(strName) || !pFS->AddDir(strName) || !pFS->ChangeDir(strName)) return FALSE;

	VFileHandleList_i it;
	for(unsigned int i=0;i<m_FileHandleList.size();i++)
	{
		it = m_FileHandleList.begin();
		for(unsigned int j=0;j<i;j++) ++it;

		if((*it)->m_FileContext.iType == FT_FILE)
		{
			CVFileHandle* pFH = OpenFileForOptimize((*it)->m_FileContext.strName);
			if(!pFH->m_lSize) continue;

			BYTE* pData = NULL;
			pData = SAFE_NEW_ARRAY( BYTE, pFH->m_lSize );			

			if( pData == NULL )
			{
				return FALSE;
			}

			if(!pFH->Read(pData, pFH->m_lSize))
			{
				SAFE_DELETE_ARRAY( pData );				
				CloseFile(pFH);
				return FALSE;
			}

			CloseFile(pFH);

			if(!pFS->AddFileForOptimize((*it)->m_FileContext.strName, pData, (*it)->m_FileContext.lCompressSize, (*it)->m_FileContext.lUncompressSize))
			{
				SAFE_DELETE_ARRAY( pData );			
				return FALSE;
			}

			SAFE_DELETE_ARRAY( pData );			
		}
		else if((*it)->m_FileContext.iType == FT_DIR)
		{
			if(strcmp((*it)->m_FileContext.strName, ".") && strcmp((*it)->m_FileContext.strName, ".."))
			{
				char strTemp[_MAX_FNAME];
				strcpy(strTemp, (*it)->m_FileContext.strName);
				if(!OptimizeDir(pFS, strTemp)) return FALSE;
			}
		}
	}

	ChangeDir("..");
	pFS->ChangeDir("..");

	return TRUE;
}

CVFileHandle*  CVFileSystem::OpenFileForOptimize(const char* strPath)
{
	if(!m_pFile) return NULL;

	char strOldPath[_MAX_PATH];
	strcpy(strOldPath, m_strCurDir);

	char strCurPath[_MAX_PATH];
	GetPathOnly(strCurPath, strPath);

	if(strCurPath[0]) ChangeDir(strCurPath);

	char strName[_MAX_FNAME];
	GetNameOnly(strName, strPath);

	VFileHandleList_i it;
	for(it=m_FileHandleList.begin();it!=m_FileHandleList.end();++it)
	{
		if((*it)->m_FileContext.iType == FT_FILE)
		{
			if(!stricmp(strName, (*it)->m_FileContext.strName)) break;
		}
	}
	if(it == m_FileHandleList.end()) return NULL;

	int iHandle = -1;
	for(int i=0;i<FS_MAX_OPEN;i++)
	{
		if(!m_HandleList[i].m_FileContext.strName[0])
		{
			iHandle = i;
			break;
		}
	}
	if(iHandle < 0) return NULL;

	fseek(m_pFile, (*it)->m_lSrtPos+sizeof(FILECONTEXT), SEEK_SET);

	long lSize = (*it)->m_FileContext.lCompressSize;
	BYTE* pData = NULL;
	pData = SAFE_NEW_ARRAY( BYTE, lSize );

	if( pData == NULL )
	{
		return NULL;
	}

	fread(pData, lSize, 1, m_pFile);

	m_HandleList[iHandle].m_FileContext.iType = (*it)->m_FileContext.iType;
	strcpy(m_HandleList[iHandle].m_FileContext.strName, (*it)->m_FileContext.strName);
	m_HandleList[iHandle].m_FileContext.lCompressSize = lSize;
	m_HandleList[iHandle].m_FileContext.lUncompressSize = (*it)->m_FileContext.lUncompressSize;
	m_HandleList[iHandle].m_FileContext.lSub = 0;
	m_HandleList[iHandle].m_FileContext.lNext = 0;
	m_HandleList[iHandle].m_pData = pData;
	m_HandleList[iHandle].m_lSize = lSize;
	m_HandleList[iHandle].m_lCurPos = 0;

	if(strCurPath[0]) ChangeDir(strOldPath);

	return &m_HandleList[iHandle];
}

BOOL  CVFileSystem::AddFileForOptimize(const char* strName, BYTE* pData, long lCompressSize, long lUncompressSize)
{
	if(!m_pFile) return FALSE;

	if(lCompressSize <= 0) return TRUE;

	FILECONTEXT FileContext, FileContextCryptBuf;
	ZeroMemory(&FileContext, sizeof(FILECONTEXT));

	FileContext.iType = FT_FILE;
	GetNameOnly(FileContext.strName, strName);
	if(CheckNameExist(FileContext.strName)) return FALSE;
	{
		for( int k = strlen(FileContext.strName) + 1; k < sizeof( FileContext.strName ); ++k )
			FileContext.strName[k] = rand() % 255;	/// 해킹 방지 
	}

	FileContext.lCompressSize = lCompressSize;
	FileContext.lUncompressSize = lUncompressSize;

	fseek(m_pFile, 0, SEEK_END);
	long lNext = ftell(m_pFile);

	memcpy( &FileContextCryptBuf,  &FileContext,  sizeof(FILECONTEXT) );	/// 암호화 버퍼 
	Encrypt( &FileContextCryptBuf,  sizeof(FILECONTEXT) - sizeof(long) );	/// 암호화 : long lNext; 제외 
	fwrite(&FileContextCryptBuf, sizeof(FILECONTEXT), 1, m_pFile);
	fwrite(pData, lCompressSize, 1, m_pFile);

	CVFileHandle* pBFH = m_FileHandleList.back();
	pBFH->m_FileContext.lNext = lNext;
	fseek(m_pFile, pBFH->m_lSrtPos, SEEK_SET);
	fseek(m_pFile, FC_OFFSET_NEXT, SEEK_CUR);
	fwrite(&lNext, sizeof(lNext), 1, m_pFile);

	CVFileHandle* pFH = NULL;
	pFH = SAFE_NEW( CVFileHandle );		

	if( pFH == NULL )
	{
		return FALSE;
	}

	*pFH->GetFileContext() = FileContext;
	pFH->m_lSrtPos = lNext;
	pFH->m_lCurPos = 0;
	m_FileHandleList.push_back(pFH);

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////
