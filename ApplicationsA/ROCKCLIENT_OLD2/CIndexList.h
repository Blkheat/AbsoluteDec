//----------------------------------------------------------------------------------------------------------------------
// 파  일   명 : CIndexList.h
// 용       도 : stl map을 직접 구현. 사용은 effect.h/CSpellEffect.h에 사용하니 map으로 바꾸길 권장
//----------------------------------------------------------------------------------------------------------------------
#ifndef __CINDEXLIST_H
#define __CINDEXLIST_H

#include <vector>
#include <functional>
using namespace std;

//................................................................................................................
// 인덱스 키 
//................................................................................................................
typedef struct s_IndexKey 
{

	unsigned int IndexKey;								// 인덱스 키
	unsigned int IndexData;								// 데이터 인덱스

	bool operator ==(const unsigned int &key) const
	{
		return ( IndexKey == key );  
	}

}INDEX_KEY,*LPINDEX_KEY;


//................................................................................................................
// 인덱스키로 작은것 부터 정렬
//................................................................................................................
struct IndexKeyListLessSort: public binary_function<INDEX_KEY, INDEX_KEY, bool> 
{ 
       bool operator()(const INDEX_KEY &Key0,const INDEX_KEY &Key1) const 
       { 
		   return ( Key0.IndexKey < Key1.IndexKey );
       } 
}; 

//................................................................................................................
// class CIndexList
//................................................................................................................
template <class INDEX_DATA>
class CIndexList
{
public:
	 
	 CIndexList() {};
	 ~CIndexList() {Clear();}

private:

	vector <INDEX_KEY>	   m_IndexKeyList;									// 인덱스 리스트
	vector <INDEX_DATA>    m_DataList;										// 데이터 리스트

public:

	unsigned int Size(void)													// 데이터 사이즈
	{
		return m_IndexKeyList.size();
	}

	void		 IndexKeyLessSort(void)										// 인덱스키 작은순 정렬
	{
		sort( m_IndexKeyList.begin() , m_IndexKeyList.end() , IndexKeyListLessSort() );
	}

	void		 Clear(void)												// 데이터 지우기
	{
		m_IndexKeyList.clear();
		m_DataList.clear();
	}

	void		 ClearPointer(void)											// 포인터 데이터 지우기 ( 데이터가 포인트 일때 )
	{
		m_IndexKeyList.clear();
		
		vector <INDEX_DATA> ::iterator i = m_DataList.begin();
		
		for( ; i != m_DataList.end() ; ++i )
		{
			INDEX_DATA pData = (*i);
			SAFE_DELETE( pData );
			(*i) = NULL;
		}

		m_DataList.clear();
	}

	INDEX_DATA  *GetKeyDataPointer(unsigned int key)								// 키로 데이터 찾기
	{
		vector <INDEX_KEY> ::iterator fi = find( m_IndexKeyList.begin () , m_IndexKeyList.end (), key );

		if( fi == m_IndexKeyList.end() )
			return NULL;
		
		return &m_DataList[ fi->IndexData ];
	}

	INDEX_DATA  *GetKeyNum(unsigned int num)								// 키순서로 데이터 찾기
	{
		return &m_DataList[ m_IndexKeyList[ num ].IndexData ];
	}

	INDEX_DATA  *GetDataNum(unsigned int num)								// 테이터 순서데로 데이터 찾기
	{
		return &m_DataList[ num ];
	}

	INDEX_DATA *Insert(unsigned int index_key,INDEX_DATA &data)			// 데이터 넣기
	{
		m_DataList.push_back( data );
		
		INDEX_KEY	KeyData;
		
		KeyData.IndexKey  = index_key;
		KeyData.IndexData = m_DataList.size() - 1;
		
		m_IndexKeyList.push_back( KeyData );
		
		return &m_DataList[ KeyData.IndexData ];
	}

};

#endif