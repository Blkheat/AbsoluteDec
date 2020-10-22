//----------------------------------------------------------------------------------------------------------------------
// ��  ��   �� : CIndexList.h
// ��       �� : stl map�� ���� ����. ����� effect.h/CSpellEffect.h�� ����ϴ� map���� �ٲٱ� ����
//----------------------------------------------------------------------------------------------------------------------
#ifndef __CINDEXLIST_H
#define __CINDEXLIST_H

#include <vector>
#include <functional>
using namespace std;

//................................................................................................................
// �ε��� Ű 
//................................................................................................................
typedef struct s_IndexKey 
{

	unsigned int IndexKey;								// �ε��� Ű
	unsigned int IndexData;								// ������ �ε���

	bool operator ==(const unsigned int &key) const
	{
		return ( IndexKey == key );  
	}

}INDEX_KEY,*LPINDEX_KEY;


//................................................................................................................
// �ε���Ű�� ������ ���� ����
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

	vector <INDEX_KEY>	   m_IndexKeyList;									// �ε��� ����Ʈ
	vector <INDEX_DATA>    m_DataList;										// ������ ����Ʈ

public:

	unsigned int Size(void)													// ������ ������
	{
		return m_IndexKeyList.size();
	}

	void		 IndexKeyLessSort(void)										// �ε���Ű ������ ����
	{
		sort( m_IndexKeyList.begin() , m_IndexKeyList.end() , IndexKeyListLessSort() );
	}

	void		 Clear(void)												// ������ �����
	{
		m_IndexKeyList.clear();
		m_DataList.clear();
	}

	void		 ClearPointer(void)											// ������ ������ ����� ( �����Ͱ� ����Ʈ �϶� )
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

	INDEX_DATA  *GetKeyDataPointer(unsigned int key)								// Ű�� ������ ã��
	{
		vector <INDEX_KEY> ::iterator fi = find( m_IndexKeyList.begin () , m_IndexKeyList.end (), key );

		if( fi == m_IndexKeyList.end() )
			return NULL;
		
		return &m_DataList[ fi->IndexData ];
	}

	INDEX_DATA  *GetKeyNum(unsigned int num)								// Ű������ ������ ã��
	{
		return &m_DataList[ m_IndexKeyList[ num ].IndexData ];
	}

	INDEX_DATA  *GetDataNum(unsigned int num)								// ������ �������� ������ ã��
	{
		return &m_DataList[ num ];
	}

	INDEX_DATA *Insert(unsigned int index_key,INDEX_DATA &data)			// ������ �ֱ�
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