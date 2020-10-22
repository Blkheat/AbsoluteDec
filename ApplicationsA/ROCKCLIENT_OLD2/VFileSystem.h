/*///////////////////////////////////////////////////////////////////////////
	Virtual File System

	Thanks to 신용우
		(http://www.gamesync.com.ne.kr/fpsedit.html#가상파일시스템)
	Thanks to shiva
		( http://www.3dstudy.net/cgi-bin/ez2000/ezboard.cgi?db=column&action=read&dbf=21&page=0&depth=1 )
	Modified by sooree
		Encrypt & Decrypt when read or write FILECONTEXT ... by RockCrypto Library

	VFileSystem을 사용하기 위해선 아래와 같은 파일들이 필요 
		- VFileSystem.h, VFileSystem.cpp
		- zlib 관련 : RockBase에 있는 Lib를 쓴다. (원래는 zlib.h, zconf.h, zdll.lib zlib1.dll )
///////////////////////////////////////////////////////////////////////////*/

#pragma once

#ifdef _DEBUG
#	pragma comment(lib, "zlibDM.lib")
#else
#	pragma comment(lib, "zlibM.lib")
#endif
 
#pragma warning(disable:4702)
#pragma warning(disable:4786)

#define _STLP_USE_NEWALLOC
#include <list>
#include"stdio.h"
using namespace std;

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

#define FS_MAX_OPEN		8
#define FS_MAX_HEADER	24
#define FS_HEADER_STR	"@Rock:VirtualFile$2004"

#define FC_OFFSET_TYPE			 (0)
#define FC_OFFSET_NAME			 (sizeof(int))
#define FC_OFFSET_COMPRESSSIZE	 (sizeof(int)+_MAX_FNAME)
#define FC_OFFSET_UNCOMPRESSSIZE (sizeof(int)+_MAX_FNAME+sizeof(long))
#define FC_OFFSET_SUB			 (sizeof(int)+_MAX_FNAME+sizeof(long)*2)
#define FC_OFFSET_NEXT			 (sizeof(int)+_MAX_FNAME+sizeof(long)*3)

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

enum FILETYPE {FT_DIR,  FT_FILE};

typedef struct _tagFILECONTEXT
{
	int		iType;
	char	strName[_MAX_FNAME];
	long	lCompressSize;
	long	lUncompressSize;
	long	lSub;
	long	lNext;
}FILECONTEXT;

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

class CVFileHandle
{
private:
	FILECONTEXT	m_FileContext;
	long		m_lSrtPos;

private:
	BYTE*		m_pData;
	long		m_lSize;
	long		m_lCurPos;

public:
	CVFileHandle();
	virtual ~CVFileHandle();

public:
	inline FILECONTEXT* GetFileContext() { return &m_FileContext; }
	inline void*	GetData()	{	return m_pData;	}
	inline int		GetSize()	{	return ( GetFileContext()->lUncompressSize );	}

public:
	int	Gets(char* pBuff, long lSize);
	int	Getws(wchar_t* pBuff, long lSize);
	int Read(void* pBuff, long lSize);
	int Seek(long lOffset, int iOrigin);
	
	long GetFsize()				{   return m_lSize; }
	long GetFCurPos()			{   return m_lCurPos; }

	friend class CVFileSystem;
};

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
typedef	CVFileHandle					VFileHandle;
typedef list<CVFileHandle*>				VFileHandleList;
typedef list<CVFileHandle*>::iterator	VFileHandleList_i;

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

class CVFileSystem
{
private:
	FILE*			m_pFile;
	VFileHandleList	m_FileHandleList;
	CVFileHandle	m_HandleList[FS_MAX_OPEN];
	char			m_strCurDir[_MAX_PATH];
	
	CRITICAL_SECTION	m_FSysFlag;

public:
	CVFileSystem();
	virtual ~CVFileSystem();

public:
	inline VFileHandleList* GetFileHandleList() { return &m_FileHandleList; }
	inline const char*		GetCurDir()			{ return m_strCurDir; }

private:
	void SetCurDir(const char* pTok);
	void GetSafeName(char* strDst, const char* strName);
	BOOL SearchDir(const char* name, CVFileHandle* fh);

	int FreadCrypto( void* buffer, int size, int count, FILE* fp );
	int FwriteCrypto( void* buffer, int size, int count, FILE* fp );
	void Encrypt( void* buffer, int size );
	void Decrypt( void* buffer, int size );

public:
	void CloseFileSystem();
	BOOL OpenFileSystem(const char* strPath );
	BOOL NewFileSystem(const char* strPath);

public:
	void CloseFile(CVFileHandle* pFH);
	CVFileHandle* OpenFile(const char* strPath);
	BOOL AddDir(const char* strName);
	BOOL AddFile(const char* strPath);
	BOOL AddFile(const char* strName, BYTE* pData, long lSize);
	BOOL Remove(const char* strName);
	BOOL Rename(const char* strName, const char* strRename);

public:
	BOOL ChangeDir(const char* strPath);
	BOOL CheckNameExist(const char* strName);
	void GetNameOnly(char* strDst, const char* strPath);
	void GetPathOnly(char* strDst, const char* strPath);

public:
	/// Virtual File 에서 file을 Remove하면 header만 지우고 body data는 그대로 있다. 그걸 추려내기 위해 optimize
	BOOL OptimizeFileSystem(const char* strPath);
	BOOL OptimizeDir(CVFileSystem* pFS,const char* strName);
	CVFileHandle* OpenFileForOptimize(const char* strPath);
	BOOL AddFileForOptimize(const char* strName,BYTE* pData,long lCompressSize,long lUncompressSize);
};

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

extern	CVFileSystem		g_VFChrRoot; /// sooree pack 
extern	CVFileSystem		g_VFChrItm;	/// 빈번한 ChangeDir()은 오버헤드가 심하다. 
extern	CVFileSystem		g_VFChrMsh; /// 아예 자주 쓰는 디렉토리별로 파일 시스템을 별도로 Open 하자. 
extern	CVFileSystem		g_VFChrTex;
extern	CVFileSystem		g_VFChrAni;

extern	CVFileSystem		g_VFNpcRoot; /// sooree pack 
extern	CVFileSystem		g_VFNpcItm;	/// 빈번한 ChangeDir()은 오버헤드가 심하다. 
extern	CVFileSystem		g_VFNpcMsh; /// 아예 자주 쓰는 디렉토리별로 파일 시스템을 별도로 Open 하자. 
extern	CVFileSystem		g_VFNpcTex;
extern	CVFileSystem		g_VFNpcAni;

extern	CVFileSystem		g_VFEffectRoot;
extern	CVFileSystem		g_VFEffectData;
extern	CVFileSystem		g_VFEffectStatic;
extern	CVFileSystem		g_VFEffectTex;
extern	CVFileSystem		g_VFEffectTexAni;

extern	CVFileSystem		g_VFEnv;
extern	CVFileSystem		g_VFEtc;
extern	CVFileSystem		g_VFInterface;
extern	CVFileSystem		g_VFSound;
extern	CVFileSystem		g_VFMap;
