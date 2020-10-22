#ifndef __CONTAINER_H__
#define __CONTAINER_H__
//-----------------------------------------------------------------------------
#include "..\\RockPCH.h"

#include "Define.h"
#include "Wnd.h"



///----------------------------------------------------------------------------
///-- CContainer
///----------------------------------------------------------------------------
///-- WID �� ������ ��� Wnd �� �����Ѵ�.
///-- ������ ����.

///-- 2005.03.23 / Lyul ����
///-- vector �� ���� 

///-- !!!! WID �� Sorting �� �Ǹ� �迭ó�� Random Access ����
///-- !!!! reserve() �� �Ҵ��� �ʿ� ���� -> count �� ���� �ľǵ�  reserve
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