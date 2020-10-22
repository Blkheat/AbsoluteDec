#include "CFloatSequence.h"

CFloatSequence::CFloatSequence()
{
}

CFloatSequence::~CFloatSequence()
{
}

void CFloatSequence::SetMaxTime(DWORD Time)
{
	m_MaxTime = Time;
}

int  CFloatSequence::Insert(DWORD Time,float Value)
{
	if( Time > m_MaxTime )
	{
		return FALSE;
	}

	FLOAT_SEQUENCE_TABLE TempData;
	
	TempData.Time  = Time;
	TempData.Value = Value;

	m_List.push_back( TempData );

	// 시간순으로 소트
	std::sort( m_List.begin() , m_List.end() , FloatSequenceListLessSort() );

	return TRUE;
}

float  CFloatSequence::GetValue(DWORD Time,float &Value)
{
	// 범위가 두개이상되야징...
	if( m_List.size() < 2 )
	{
		Value = 0.0f;

		return FALSE;
	}
	
	if( Time >= m_MaxTime )
	{
		Value = m_List[ m_List.size() - 1 ].Value;

		return FALSE;
	}
	
	for( unsigned int i = 0 ; i < ( m_List.size() - 1 ); ++i )
	{
		if( m_List[ i ].Time == Time )
		{
			Value = m_List[ i ].Value;

			return TRUE;
		}
		else if( m_List[ i ].Time <= Time && Time < m_List[ i + 1 ].Time )
		{
			float Range = ( float )( Time - m_List[ i ].Time ) / ( float )( m_List[ i + 1 ].Time - m_List[ i ].Time );

			Value = m_List[ i ].Value + ( m_List[ i + 1 ].Value - m_List[ i ].Value ) * Range;

			return TRUE;		
		}
	}

	Value = m_List[ m_List.size() - 1 ].Value;
	
	return FALSE;
}