#include "CNPCImpactTable.h"

#define NPC_IMPACT_TABLE_FILENAME	"NpcImpact.dat"

CNPCImpactTable::CNPCImpactTable()
{
}

CNPCImpactTable::~CNPCImpactTable()
{

}

int CNPCImpactTable::Load(void)
{
    CTextCommand TextCmd;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined( LOAD_VF ) /////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	char Path[ 1024 ] = "";

	wsprintf( Path , "Etc\\%s" , NPC_IMPACT_TABLE_FILENAME );

    if( TextCmd.Open( Path ) == NULL )
    {
        return false;
    }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#else

    if( TextCmd.OpenVF( NPC_IMPACT_TABLE_FILENAME ) == NULL )
    {
        return false;
    }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif

    IMPACTTABLE_DATA TempData;
	int				 index;
	int				 nKey;
	
	while( TextCmd.GetLine() != NULL )
    {
		if( TextCmd.ISArgEmpty() )
        {
            continue;
        }

		TextCmd.GetArgValue( 0 , index );

		for( int i = 0 ; i < 10 ; ++i )
		{
			TextCmd.GetArgValue( i + 1 , TempData.frame[ i ] );
		}

		TextCmd.GetArgValue( 11 , nKey );

		std::map<int,IMPACTTABLE_DATA>::iterator pos;
		pos = m_NpcImpactMap.find( nKey );
		if( pos != m_NpcImpactMap.end() )
		{
			continue;
		}
		
		m_NpcImpactMap.insert( 
			std::map<int,IMPACTTABLE_DATA>::value_type( nKey , TempData ) 
			);
    }

    TextCmd.Close();
    
    return true;
}

int CNPCImpactTable::GetFrame(int nKey,int frame)
{
	std::map<int,IMPACTTABLE_DATA>::iterator pos;

	pos = m_NpcImpactMap.find( nKey );

	if( pos != m_NpcImpactMap.end() )
	{
		IMPACTTABLE_DATA data = pos->second;
		return data.frame[ frame ];
	}	
	
	return -1;
}