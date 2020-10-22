#ifndef __CSLANGFILTER_H
#define __CSLANGFILTER_H

#include <vector>
#include <string>
#include <iostream>

#include "bravolibs\\obj\\EtcStruct.H"

typedef struct s_SlangData
{
#ifdef UNICODE_ROCK
	WCHAR *StrCompare;           // 비교할 문자
    WCHAR *StrReplace;           // 대체할 문자
#else
	char *StrCompare;           // 비교할 문자
    char *StrReplace;           // 대체할 문자
#endif
    
    s_SlangData()
    {
        StrCompare = NULL;
        StrReplace = NULL;
    }
	
    ~s_SlangData()
    {
        SAFE_DELETE_ARRAY( StrCompare );
		SAFE_DELETE_ARRAY( StrReplace );
    }
	
}SLANG_DATA,*LPSLANG_DATA;

typedef struct s_NameList
{
	
    RTCHAR *StrCompare;          // 비교할 문자
	
	s_NameList()
	{
		StrCompare = NULL;
	}
	
	~s_NameList()
	{
		SAFE_DELETE_ARRAY( StrCompare );
	}
	
	
}NAME_DATA,*LPNAME_DATA;


#define MAX_SLANG_SPACE         100     // 최대 공백문자 개수
#define MAX_SLANG_STRING        1024    // 최대 문자열 컨트롤 개수

class CSlangFilter
{
public:
	
	CSlangFilter();
    ~CSlangFilter();
	
public:
	
	void DeleteAllMemory(void);
	void DeleteSlangList(void);
	void DeleteNameList(void);
	void DeleteAdminNameList(void);


	int	LoadList(char *FileName);
	int	LoadNameList(char *FileName);
	int	LoadAdminNameList(char *FileName);
	
	//UNICODE 때문에 두개로 분리한다 ;; 
	int	Filter(void *Str);
	int	FilterA(char *Str);
	int	FilterW(WCHAR *Str);

	int  ISFindFilterName(void *Str);
	int  ISFindFilterNameA(char *Str);
	int  ISFindFilterNameW(WCHAR *Str);


	bool IsValidString(void * buffer); // 캐릭터 이름 생성시
	bool IsValidStringA(char * buffer); // 캐릭터 이름 생성시
	bool IsValidStringW(WCHAR * buffer); // 캐릭터 이름 생성시
	
	bool IsFilterGmName(void *Str);
	bool IsFilterGmNameA(char *Str);
	bool IsFilterGmNameW(WCHAR *Str);
	
	bool IsCheckSymbol(void *SrcStr , void *DstStr);
	
	bool ConvertToUpper(char * buffer);
	
	char *GetString(void)              { return m_TempStr; }
	
	bool ISSlangFound(void)			{ return m_ISSlangFound; }
	
private:
	
	bool						m_ISSlangFound;
	
    std::vector <LPSLANG_DATA>  m_pSlangList;
	std::vector <LPNAME_DATA>   m_pNameList;
	std::vector <LPNAME_DATA>	m_pAdminNameList;

	

    int  m_SpacePosition[ MAX_SLANG_SPACE ];
    int  m_SpaceCount;
	
    char m_TempStr[ MAX_SLANG_STRING ];
};

#endif