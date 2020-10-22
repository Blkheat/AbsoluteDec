#include "Container.h"

///-- !!!! Naming ���� 
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
///-- Debug ������ �ε��Ǵ� Wnd �� ������ �����ش�.
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
///-- �ش� Wid �� ������ Wnd �� ��ȯ�Ѵ�.
///-- ���� WID �� ������ ���������� Random Access ����
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