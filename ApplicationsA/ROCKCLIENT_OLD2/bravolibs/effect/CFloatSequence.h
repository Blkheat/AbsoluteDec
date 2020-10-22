#ifndef __CFLOATSEQUENCE_H
#define __CFLOATSEQUENCE_H

#include <vector>
#include <algorithm>
#include <iostream>
#include "d3d9.h"
#include "d3dx9math.h"
#include <map>

using namespace std;

typedef struct s_FloatSequence_Table
{
	
	DWORD	Time;
	float	Value;

}FLOAT_SEQUENCE_TABLE;


class CFloatSequence
{
public:
	 
	 CFloatSequence();
	~CFloatSequence();

	 void   SetMaxTime(DWORD Time);
	 int    Insert(DWORD Time,float Value);
	 float	GetValue(DWORD Time,float &Value);


private:
	
	std::vector <FLOAT_SEQUENCE_TABLE>	m_List;

	DWORD	m_MaxTime;

};

//................................................................................................................
// 인덱스키로 작은것 부터 정렬
//................................................................................................................
struct FloatSequenceListLessSort: public binary_function<FLOAT_SEQUENCE_TABLE, FLOAT_SEQUENCE_TABLE, bool> 
{ 
       bool operator()(const FLOAT_SEQUENCE_TABLE &Key0,const FLOAT_SEQUENCE_TABLE &Key1) const 
       { 
		   return ( Key0.Time < Key1.Time );
       } 
}; 

#endif