#include "RockPCH.h"

#include	"..\brother.h"

#include	"Animation.H"
#include	<stdio.H>
#include	<assert.H>

extern	long				g_nowTime;

//-----------------------------------------------------------------------------
// Desc: 본클래스 생성자 & 파괴자
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
// Desc: 본파일 읽기
//-----------------------------------------------------------------------------
bool Bone::Read( char* file_path )
{
	// 파일포인터 열기
	FILE*	file;
	file = fopen( file_path, "rb" );
	if( file == NULL )
	{
		CloseWnd_Error( "!%s 파일을 찾을 수 없습니다.", file_path );			
		return false;
	}

	// 애니 프레임 & 애니 타임 & 노드 갯수저장
	fread( &m_ani_frame,	sizeof(m_ani_frame),	1,	file );
	fread( &m_num_node,		sizeof(m_num_node),		1,	file );

	// 노드 구조체 메모리할당
	m_pNode = SAFE_NEW_ARRAY( Bone_Node , m_num_node );	
	for( unsigned int bone = 0; bone < m_num_node; ++bone )
	{
		// 아이디 로컬값
		fread( &m_pNode[bone].m_self_id,	sizeof(m_pNode[bone].m_self_id),	1, file );
		fread( &m_pNode[bone].m_matLocal,	sizeof(m_pNode[bone].m_matLocal),	1, file );

		// 애니메이션 메모리할당
		m_pNode[bone].m_pAnimation = SAFE_NEW_ARRAY( Bone_Animation , m_ani_frame );			
		fread( m_pNode[bone].m_pAnimation, sizeof(Bone_Animation), m_ani_frame, file );		
	}	

	// 파일 포인터 닫기
	fclose( file );

	return true;
}

//-----------------------------------------------------------------------------
// Desc: 본파일 읽기
//-----------------------------------------------------------------------------
bool Bone::ReadVF( CVFileSystem* pVF, char* file_path )	/// sooree pack
{
	// 파일포인터 열기
	VFileHandle* pFH = pVF->OpenFile( file_path );
	if( pFH == NULL )
	{
		CloseWnd_Error( "!%s 파일을 찾을 수 없습니다.", file_path );			
		return false;
	}
		
	// 애니 프레임 & 애니 타임 & 노드 갯수저장
	pFH->Read( &m_ani_frame,	sizeof(m_ani_frame) );
	pFH->Read( &m_num_node,		sizeof(m_num_node) );

	// 노드 구조체 메모리할당
	m_pNode = SAFE_NEW_ARRAY( Bone_Node , m_num_node );		
	for( unsigned int bone = 0; bone < m_num_node; ++bone )
	{
		// 아이디 로컬값
		pFH->Read( &m_pNode[bone].m_self_id,	sizeof(m_pNode[bone].m_self_id) );
		pFH->Read( &m_pNode[bone].m_matLocal,	sizeof(m_pNode[bone].m_matLocal) );

		// 애니메이션 메모리할당
		m_pNode[bone].m_pAnimation = SAFE_NEW_ARRAY( Bone_Animation , m_ani_frame );			
		pFH->Read( m_pNode[bone].m_pAnimation, sizeof(Bone_Animation) * m_ani_frame );		
	}	

	// 파일 포인터 닫기
	pVF->CloseFile( pFH );

	return true;
}
