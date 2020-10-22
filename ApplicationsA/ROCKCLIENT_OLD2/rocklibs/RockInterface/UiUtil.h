#if !defined( __UIUTIL_H__ )
#define __UIUTIL_H__

#define SMALL 1.0e-4f
#define BIG 1.0e+10f
#define ISZERO(a) ((a) > -SMALL && (a) < SMALL )

//#define MINIGAMETEST



//by simwoosung
extern void ConvertTemKeyRestKey(RTCHAR *pString);
extern int  GetNSkillLevel(int nGageValue);
extern void RandomNameMake(TCHAR *pStr, int size);
extern void GetTCharStartIndexString(TCHAR *In, int sIndex, TCHAR * Out);
extern TCHAR * GetStringReturnInString(TCHAR ch, bool bInch, int nSize, TCHAR *strIn, TCHAR *strOut, BOOL IsMarkUp = FALSE );
extern RTCHAR * GetStringReturnInStringW(RTCHAR ch, bool bInch, int nSize, void *strIn, void *strOut, BOOL IsMarkUp = FALSE );
extern int  DetectionDBCS( int* Count, char* Str, char *Db );
extern void ConvertStrInLimitLength(RTCHAR *strTemp, int size);
extern void ConvertOtherConLanguage(RTCHAR *strTemp);
extern void ConvertTimeToString(time_t	tTime,  TCHAR *pStr);

extern bool IsNonPlayerCode( WORD code );
extern bool IsObjectNPCCode( WORD code );
extern BOOL IsSameNationUnit( void * pPtr );

extern void ChatMessagePrint(TCHAR * tstrMessage, D3DCOLOR tcolor, BOOL bMessWndFlag );

extern void GetClassText(int iClass, TCHAR * strTemp);
extern void GetAreaStrName(TCHAR * strTemp, UINT uiMapID);

extern int	GetPKStepFromPKPoint(DWORD dPKPoint);
extern int	GetDecreaseEXPRatioFromPKPoint(DWORD dPKPoint);
extern float GetBuyShopRatioFromPKPoint(DWORD dPKPoint);
extern float GetSellShopRatioFromPKPoint(DWORD dPKPoint);
extern void GetRemainTimeStr( TCHAR * tmpStr , DWORD dReaminTime );

template< class T >
class CDataQueue 
{
protected:
	int m_iMaxSize;                  //ť�� �ִ��� �����ִ� �޽����� ũ�� 
	int m_iCurSize;                  //���� ť�� ����ִ� �޽����� �Ѱ���
	
	T *	m_pDataList;                 //�Ҵ�� �޽��� ����Ʈ
	
	int m_iFrontIndex;               //ť�� �Է� �ε���
	int m_iBackIndex;                //ť�� ��� �ε���	
	
public:
	CDataQueue();
	CDataQueue(int MaxSize);
	virtual ~CDataQueue();
	
	int GetCurQueueSize()     { return m_iCurSize; } 
	bool PushData(T & data);   //ť�� ����Ÿ�� �߰��Ѵ�.  
	bool PopData(T & data);    //ť���� ����Ÿ�� �����´�.
	bool CheckData( T & data); //ť���� �����͸� üũ�Ѵ�.
	void Clear();			   //ť�� ��ü �����Ѵ�.
	
	void OverFlow();           //�����÷ο� ó���Լ�
	void UnderFlow();          //����÷ο� ó���Լ�
};

template< class T >
CDataQueue< T >::CDataQueue()
{
    m_pDataList = NULL;
    m_iMaxSize = 100;        //�⺻ �迭�� ũ�⸦ 100���� ��´�.
    m_iCurSize = 0;
	
    m_pDataList = SAFE_NEW_ARRAY( T , m_iMaxSize );	
	
    m_iFrontIndex = 0;
    m_iBackIndex = 0;
}

template< class T >
CDataQueue< T >::CDataQueue(int MaxSize)
{
    m_pDataList = NULL;
    m_iMaxSize = MaxSize;
    m_iCurSize = 0;
	
    m_pDataList = SAFE_NEW_ARRAY( T , m_iMaxSize );
	
    m_iFrontIndex = 0;
    m_iBackIndex = 0;
}

template< class T >
CDataQueue< T >::~CDataQueue()
{
	SAFE_DELETE_ARRAY(m_pDataList);
}

template< class T >
void CDataQueue< T >::Clear()
{
	m_iCurSize = 0;
	m_iFrontIndex = 0;
    m_iBackIndex = 0;
}

template< class T >
bool CDataQueue< T >::PushData(T & data)
{
    m_iFrontIndex = (m_iFrontIndex + 1) % m_iMaxSize;
	
	if(m_iFrontIndex != m_iBackIndex)
		m_pDataList[m_iFrontIndex] = data;
    else
	{
		OverFlow();
		return false;
	}  
    m_iCurSize++;
	return true;
}

template< class T >
bool CDataQueue< T >::PopData(T & data)
{
	if(m_iFrontIndex == m_iBackIndex)
	{
		UnderFlow();
		return false; 
	}
	
	m_iBackIndex = (m_iBackIndex + 1) % m_iMaxSize;
	data = m_pDataList[m_iBackIndex];
	
	m_iCurSize--;
	return true; 
}

template< class T >
bool CDataQueue< T >::CheckData( T & data)
{
   data = m_pDataList[m_iBackIndex];		
   return true;
}

template< class T >
void CDataQueue< T >::OverFlow()
{
	
}

template< class T >
void CDataQueue< T >::UnderFlow()
{
	
}

#endif  __UIUTIL_H__