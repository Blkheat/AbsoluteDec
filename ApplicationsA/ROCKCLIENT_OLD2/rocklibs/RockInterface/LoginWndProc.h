#if !defined( __RUI_LOGIN_WNDPROC_H__ )
#define __RUI_LOGIN_WNDPROC_H__
//�α���-----------------------------------------------------------------------------
#include "WndProc.h"


class CLoginWnd : public CWndProc
{
public:
	CLoginWnd(){}
	~CLoginWnd(){}
	
	virtual void Init();
	virtual void Composition();
	virtual void Proc( SEventMessage* EMsg );
#ifdef DECO_RENEWAL_MJH
	TCHAR* GetUserName();
	TCHAR* GetPassWord();
#endif // DECO_RENEWAL_MJH	
private:
	int IsCheckInSpace(char *Str);// �����̽� �ֳ� Ȯ��
	bool IsCheckSymbol(char *SrcStr , char *DstStr);
	bool TestDecoIDCheck(char *Str);

};

 
extern	CLoginWnd	LoginWnd;
//-----------------------------------------------------------------------------
#endif  __RUI_LOGIN_WNDPROC_H__