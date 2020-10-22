#ifndef __CSLANGFILTER_H
#define __CSLANGFILTER_H

#include <vector>
#include <string>
#include <iostream>

#include "bravolibs\\obj\\EtcStruct.H"

typedef struct s_SlangData
{
#ifdef UNICODE_ROCK
	WCHAR *StrCompare;           // ���� ����
    WCHAR *StrReplace;           // ��ü�� ����
#else
	char *StrCompare;           // ���� ����
    char *StrReplace;           // ��ü�� ����
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
	
    RTCHAR *StrCompare;          // ���� ����
	
	s_NameList()
	{
		StrCompare = NULL;
	}
	
	~s_NameList()
	{
		SAFE_DELETE_ARRAY( StrCompare );
	}
	
	
}NAME_DATA,*LPNAME_DATA;


#define MAX_SLANG_SPACE         100     // �ִ� ���鹮�� ����
#define MAX_SLANG_STRING        1024    // �ִ� ���ڿ� ��Ʈ�� ����

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
	
	//UNICODE ������ �ΰ��� �и��Ѵ� ;; 
	int	Filter(void *Str);
	int	FilterA(char *Str);
	int	FilterW(WCHAR *Str);

	int  ISFindFilterName(void *Str);
	int  ISFindFilterNameA(char *Str);
	int  ISFindFilterNameW(WCHAR *Str);


	bool IsValidString(void * buffer); // ĳ���� �̸� ������
	bool IsValidStringA(char * buffer); // ĳ���� �̸� ������
	bool IsValidStringW(WCHAR * buffer); // ĳ���� �̸� ������
	
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