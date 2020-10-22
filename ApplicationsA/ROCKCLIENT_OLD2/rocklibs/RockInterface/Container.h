#ifndef __CONTAINER_H__
#define __CONTAINER_H__
//-----------------------------------------------------------------------------
#include "..\\RockPCH.h"

#include "Define.h"
#include "Wnd.h"



///----------------------------------------------------------------------------
///-- CContainer
///----------------------------------------------------------------------------
///-- WID 를 가지는 모든 Wnd 를 관리한다.
///-- 삭제는 없다.

///-- 2005.03.23 / Lyul 수정
///-- vector 로 변경 

///-- !!!! WID 가 Sorting 이 되면 배열처럼 Random Access 가능
///-- !!!! reserve() 로 할당할 필요 있음 -> count 로 개수 파악뒤  reserve
class CContainer
{
public:

	~CContainer();						///-- Destructor	
	void Add( CWnd* aWnd );				///-- Add Wnd	
	CWnd* Get( WNDID aWid );			///-- Get Wnd
	
private:

	std::vector<CWnd*> theWnd;			///-- CWnd instance


};


extern CContainer* Container;
//-----------------------------------------------------------------------------
#endif  __CONTAINER_H__