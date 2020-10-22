#include "RockPCH.h"

#include	"..\brother.h"

#include	"Animation.H"
#include	<stdio.H>
#include	<assert.H>

extern	long				g_nowTime;

//-----------------------------------------------------------------------------
// Desc: ��Ŭ���� ������ & �ı���
//-----------------------------------------------------------------------------
void Bone::Destory()
{	
	if( m_pNode != NULL )
	{
		for( unsigned int i = 0; i < m_num_node; ++i )
		{
			SAFE_DELETE_ARRAY(m_pNode[i].m_pAnimation);
			m_pNode[i].Clear();
		}	
		
		SAFE_DELETE_ARRAY(m_pNode);

		Clear();		
	}
}

void Bone::Clear() 
{ 
	m_pNode = NULL; 
	m_Index = 0; 
	m_race = 0;
	m_lPrevTime = g_nowTime;
}

//-----------------------------------------------------------------------------
// Desc: ������ �б�
//-----------------------------------------------------------------------------
bool Bone::Read( char* file_path )
{
	// ���������� ����
	FILE*	file;
	file = fopen( file_path, "rb" );
	if( file == NULL )
	{
		CloseWnd_Error( "!%s ������ ã�� �� �����ϴ�.", file_path );			
		return false;
	}

	// �ִ� ������ & �ִ� Ÿ�� & ��� ��������
	fread( &m_ani_frame,	sizeof(m_ani_frame),	1,	file );
	fread( &m_num_node,		sizeof(m_num_node),		1,	file );

	// ��� ����ü �޸��Ҵ�
	m_pNode = SAFE_NEW_ARRAY( Bone_Node , m_num_node );	
	for( unsigned int bone = 0; bone < m_num_node; ++bone )
	{
		// ���̵� ���ð�
		fread( &m_pNode[bone].m_self_id,	sizeof(m_pNode[bone].m_self_id),	1, file );
		fread( &m_pNode[bone].m_matLocal,	sizeof(m_pNode[bone].m_matLocal),	1, file );

		// �ִϸ��̼� �޸��Ҵ�
		m_pNode[bone].m_pAnimation = SAFE_NEW_ARRAY( Bone_Animation , m_ani_frame );			
		fread( m_pNode[bone].m_pAnimation, sizeof(Bone_Animation), m_ani_frame, file );		
	}	

	// ���� ������ �ݱ�
	fclose( file );

	return true;
}

//-----------------------------------------------------------------------------
// Desc: ������ �б�
//-----------------------------------------------------------------------------
bool Bone::ReadVF( CVFileSystem* pVF, char* file_path )	/// sooree pack
{
	// ���������� ����
	VFileHandle* pFH = pVF->OpenFile( file_path );
	if( pFH == NULL )
	{
		CloseWnd_Error( "!%s ������ ã�� �� �����ϴ�.", file_path );			
		return false;
	}
		
	// �ִ� ������ & �ִ� Ÿ�� & ��� ��������
	pFH->Read( &m_ani_frame,	sizeof(m_ani_frame) );
	pFH->Read( &m_num_node,		sizeof(m_num_node) );

	// ��� ����ü �޸��Ҵ�
	m_pNode = SAFE_NEW_ARRAY( Bone_Node , m_num_node );		
	for( unsigned int bone = 0; bone < m_num_node; ++bone )
	{
		// ���̵� ���ð�
		pFH->Read( &m_pNode[bone].m_self_id,	sizeof(m_pNode[bone].m_self_id) );
		pFH->Read( &m_pNode[bone].m_matLocal,	sizeof(m_pNode[bone].m_matLocal) );

		// �ִϸ��̼� �޸��Ҵ�
		m_pNode[bone].m_pAnimation = SAFE_NEW_ARRAY( Bone_Animation , m_ani_frame );			
		pFH->Read( m_pNode[bone].m_pAnimation, sizeof(Bone_Animation) * m_ani_frame );		
	}	

	// ���� ������ �ݱ�
	pVF->CloseFile( pFH );

	return true;
}
