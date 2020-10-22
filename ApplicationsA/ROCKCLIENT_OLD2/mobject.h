#ifndef		_____MAP_OBJ_H_____
#define		_____MAP_OBJ_H_____

class		CBravoObject
{
protected:
	DWORD		m_dwObjCount;
	char**		m_szObjNameList;

public:
	bool		Create( char* ObjPath );
	void		FinalCleanup();

	DWORD		GetCount()					{ return m_dwObjCount; };
	char*		GetName(DWORD i)			
	{ 
		if( m_dwObjCount <= i )
		{
			MessageBox( NULL , "Arrary Over Flow CBravoObject::GetName()" , "Error" , MB_OK );
		
			return NULL;
		}

		return m_szObjNameList[i]; 
	};

	DWORD		GetIndexByName(char* name);

	CBravoObject();
	~CBravoObject();
};

extern	CBravoObject	g_Object;

#endif
