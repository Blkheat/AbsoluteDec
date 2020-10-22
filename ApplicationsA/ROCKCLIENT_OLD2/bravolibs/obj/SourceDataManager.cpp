
#include "SourceDataManager.h"
#include "etcstruct.h"
#include "..\\..\\VFileSystem.h"

CSourceDataManager::CSourceDataManager()
{
	ClearData();
}

CSourceDataManager::~CSourceDataManager()
{
	ClearData();
}

BOOL CSourceDataManager::LoadData(void)
{
	ClearData();

	LoadConSkillData();
	LoadEmblemItemCode();
	LoadNpcMoveInfo();	
	LoadSkillField();	
	
	return TRUE;
}

BOOL CSourceDataManager::LoadConSkillData()
{
	char FileName[ 1024 ] = "";		
	wsprintf( FileName , "%s" , "ConAniSkill.dat" );

	VFileHandle* pFH = g_VFChrRoot.OpenFile( FileName );
	VFileHandle* pTFH = NULL;

	if( pFH == NULL ) 
	{
		return FALSE;
	}

	int nfile_size = pFH->GetSize();

	///-- 사이즈 정규성 Check
	if( nfile_size % sizeof( SConSkillData ) != 0 )
	{
		g_VFChrRoot.CloseFile( pFH );
		return FALSE;
	}
	///-- Set Count
	int n_count = nfile_size / sizeof( SConSkillData );

	for( int i = 0; i < n_count ; ++i )
	{
		SConSkillData ConSkillData;	
		pFH->Read( ( void * )&ConSkillData, sizeof( SConSkillData ));

		DWORD nLinkEffect = ConSkillData.dLinkEffect;
		m_ConSkillDataList.insert(map <DWORD , SConSkillData>::value_type(nLinkEffect, ConSkillData));	
	}	

	g_VFChrRoot.CloseFile( pFH );
	
	return TRUE;
}

BOOL CSourceDataManager::LoadEmblemItemCode()
{
	char FileName[ 1024 ] = "";		
	wsprintf( FileName , "%s" , "EmbItemCode.dat" );

	VFileHandle* pFH = g_VFChrRoot.OpenFile( FileName );
	VFileHandle* pTFH = NULL;

	if( pFH == NULL ) 
	{
		return FALSE;
	}

	int nfile_size = pFH->GetSize();

	///-- 사이즈 정규성 Check
	if( nfile_size % sizeof( SEmblemItemCode ) != 0 )
	{
		g_VFChrRoot.CloseFile( pFH );
		return FALSE;
	}
	///-- Set Count
	int n_count = nfile_size / sizeof( SEmblemItemCode );

	for( int i = 0; i < n_count ; ++i )
	{
		SEmblemItemCode sEmbItemCode;	
		pFH->Read( ( void * )&sEmbItemCode, sizeof( SEmblemItemCode ));

		DWORD dItemCode = sEmbItemCode.dItemCode;
		
		if(sEmbItemCode.IsPc)
		{
			m_ChrEmblemItemCodeList.insert(map <DWORD , SEmblemItemCode>::value_type(dItemCode, sEmbItemCode));
		}
		else
		{
			m_NpcEmblemItemCodeList.insert(map <DWORD , SEmblemItemCode>::value_type(dItemCode, sEmbItemCode));
		}
	}	

	g_VFChrRoot.CloseFile( pFH );

	return TRUE;
}

BOOL CSourceDataManager::LoadNpcMoveInfo()
{
	char FileName[ 1024 ] = "";		
	wsprintf( FileName , "%s" , "NPC_MoveInfo.dat" );

	VFileHandle* pFH = g_VFNpcRoot.OpenFile( FileName );
	VFileHandle* pTFH = NULL;

	if( pFH == NULL ) 
	{
		return FALSE;
	}

	int nfile_size = pFH->GetSize();

	///-- 사이즈 정규성 Check
	if( nfile_size % sizeof( SNPCMoveInfo ) != 0 )
	{
		g_VFNpcRoot.CloseFile( pFH );
		return FALSE;
	}
	///-- Set Count
	int n_count = nfile_size / sizeof( SNPCMoveInfo );

	for( int i = 0; i < n_count ; ++i )
	{
		SNPCMoveInfo * pNpcMoveInfo = SAFE_NEW( SNPCMoveInfo );			
		pFH->Read( ( void * )pNpcMoveInfo, sizeof( SNPCMoveInfo ));

		m_NpcMoveInfoList.push_back(pNpcMoveInfo);	
	}	

	g_VFNpcRoot.CloseFile( pFH );

	return TRUE;
}

BOOL CSourceDataManager::LoadSkillField()
{
	char FileName[ 1024 ] = "";		
	wsprintf( FileName , "%s" , "SkillFieldTable.dat" );

	VFileHandle* pFH = g_VFChrRoot.OpenFile( FileName );
	VFileHandle* pTFH = NULL;

	if( pFH == NULL ) 
	{
		return FALSE;
	}

	int nfile_size = pFH->GetSize();

	///-- 사이즈 정규성 Check
	if( nfile_size % sizeof( SSkillField ) != 0 )
	{
		g_VFChrRoot.CloseFile( pFH );
		return FALSE;
	}
	///-- Set Count
	int n_count = nfile_size / sizeof( SSkillField );

	for( int i = 0; i < n_count ; ++i )
	{
		SSkillField SkillField;	
		pFH->Read( ( void * )&SkillField, sizeof( SSkillField ));

		DWORD dCorresValue = SkillField.dCorresValue;
		m_SkillFieldList.insert( SkillFieldListValue(dCorresValue, SkillField) );	
	}	

	g_VFChrRoot.CloseFile( pFH );
	
	return TRUE;
}

void CSourceDataManager::ClearData()
{
	m_ConSkillDataList.clear();
	m_ChrEmblemItemCodeList.clear();
	m_NpcEmblemItemCodeList.clear();
	
	NPCMoveInfoListPos iPos = m_NpcMoveInfoList.begin();

	for( ; iPos != m_NpcMoveInfoList.end() ; iPos++ )
	{
		SNPCMoveInfo * pNpcMoveInfo = (SNPCMoveInfo *)(*iPos);
		SAFE_DELETE( pNpcMoveInfo );	
	}	
	
	m_NpcMoveInfoList.clear();
	///--m_ItemAttrList.clear();

	m_SkillFieldList.clear();
}

SConSkillData CSourceDataManager::FindConSkillData(DWORD dLinkEffect)
{
	SConSkillData ConSkillData;
	
	ConSkillDataListPos iPos = m_ConSkillDataList.find(dLinkEffect);
	
	if(iPos == m_ConSkillDataList.end())
	{
		return ConSkillData;	
	}
	
	ConSkillData = (*iPos).second;	
	
	return ConSkillData;
}

SConSkillData * CSourceDataManager::FindConSkillDataPtr(DWORD dLinkEffect)
{
	ConSkillDataListPos iPos = m_ConSkillDataList.find(dLinkEffect);
	
	if(iPos == m_ConSkillDataList.end())
	{
		return NULL;	
	}
	
	return (SConSkillData *)(&((*iPos).second));
}

SEmblemItemCode CSourceDataManager::FindChrEmblemItemCode(DWORD dItemCode)
{
	SEmblemItemCode sEmbItemCode;

	EmblemItemCodeListPos iPos = m_ChrEmblemItemCodeList.find(dItemCode);
	
	if(iPos == m_ChrEmblemItemCodeList.end())
	{
		return sEmbItemCode;	
	}
	
	sEmbItemCode = (*iPos).second;

	return sEmbItemCode;
}

SEmblemItemCode CSourceDataManager::FindNpcEmblemItemCode(DWORD dItemCode)
{
	SEmblemItemCode sEmbItemCode;

	EmblemItemCodeListPos iPos = m_NpcEmblemItemCodeList.find(dItemCode);
	
	if(iPos == m_NpcEmblemItemCodeList.end())
	{
		return sEmbItemCode;	
	}
	
	sEmbItemCode = (*iPos).second;

	return sEmbItemCode;
}

SEmblemItemCode * CSourceDataManager::FindChrEmblemItemCodePtr(DWORD dItemCode)
{
	EmblemItemCodeListPos iPos = m_ChrEmblemItemCodeList.find(dItemCode);
	
	if(iPos == m_ChrEmblemItemCodeList.end())
	{
		return NULL;	
	}
	
	return (SEmblemItemCode *)(&((*iPos).second));
}

SEmblemItemCode * CSourceDataManager::FindNpcEmblemItemCodePtr(DWORD dItemCode)
{
	EmblemItemCodeListPos iPos = m_NpcEmblemItemCodeList.find(dItemCode);
	
	if(iPos == m_NpcEmblemItemCodeList.end())
	{
		return NULL;	
	}
	
	return (SEmblemItemCode *)(&((*iPos).second));
}

SNPCMoveInfo * CSourceDataManager::FindNpcMoveInfoPtr(WORD NPCCode, WORD Mapid )
{
	NPCMoveInfoListPos iPos = m_NpcMoveInfoList.begin();

	for( ; iPos != m_NpcMoveInfoList.end() ; iPos++ )
	{
		SNPCMoveInfo * pNpcMoveInfo = (SNPCMoveInfo *)(*iPos);
		if( pNpcMoveInfo )
		{
			if( pNpcMoveInfo->theNPCCode == NPCCode && pNpcMoveInfo->theMap == Mapid )
			{
				return pNpcMoveInfo;
			}
		}		
	}

	return NULL;
}

SItemAttr CSourceDataManager::FindNpcMoveInfoPtr( int nNumber )
{
	SItemAttr sItemAttr;

	ItemAttrListPos iPos = m_ItemAttrList.find(nNumber);
	
	if(iPos == m_ItemAttrList.end())
	{
		return sItemAttr;	
	}
	
	sItemAttr = (*iPos).second;	

	return sItemAttr;	
}

SSkillField * CSourceDataManager::FindSkillFieldPtr( DWORD dCorresValue )
{
	SkillFieldListPos iPos = m_SkillFieldList.find(dCorresValue);
	
	if(iPos == m_SkillFieldList.end())
	{
		return NULL;	
	}
	
	return (SSkillField *)(&((*iPos).second));
}