#include "Container.h"

///-- !!!! Naming 변경 
CContainer*	Container;

///----------------------------------------------------------------------------
///-- Destructor
///----------------------------------------------------------------------------
CContainer::~CContainer()
{
	std::vector <CWnd*> ::iterator i = theWnd.begin();
	for( ; i != theWnd.end() ; ++i )
    {
		CWnd * pWnd = (*i);
		SAFE_DELETE(pWnd);	
		(*i) = NULL;
    }
};
///----------------------------------------------------------------------------
///-- Add Wnd
///----------------------------------------------------------------------------
///-- Debug 에서는 로딩되는 Wnd 의 갯수를 보여준다.
void CContainer::Add( CWnd* aWnd )
{
#ifdef GM_CLIENT 
	RLG2("Chain Add - ID : %d    Num : %d", aWnd->GetID(), theWnd.size()+1);


	///-- Check WID Duplication
	
	std::vector <CWnd*> ::iterator i = theWnd.begin();

	for( ; i != theWnd.end() ; ++i )
    {
		if( (*i)->GetID() == aWID)
		{
			assert(0);
		}
    }

#endif

	theWnd.push_back(aWnd);
	
};
///----------------------------------------------------------------------------
///-- Get
///----------------------------------------------------------------------------
///-- 해당 Wid 를 가지는 Wnd 를 반환한다.
///-- 추후 WID 의 정렬이 가능해지면 Random Access 가능
CWnd* CContainer::Get( WNDID aWID )
{
	std::vector <CWnd*> ::iterator i = theWnd.begin();

	for( ; i != theWnd.end() ; ++i )
    {
		if( (*i)->GetID() == aWID)
		{
			return (*i);
		}
    }
	return NULL;
}