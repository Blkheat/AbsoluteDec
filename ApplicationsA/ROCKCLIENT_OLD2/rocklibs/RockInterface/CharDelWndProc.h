//----------------------------------------------------------------------------------------------------------------------
// 파일명 : CharDelWndProc.h
// 용  도 : 캐릭터 삭제창
//----------------------------------------------------------------------------------------------------------------------
#ifndef CHAR_DEL_WND_PROC_H
#define CHAR_DEL_WND_PROC_H


#include "WndProc.h"


class CFrameWnd;
class CButton;
class CTextBox;
class CEditBox;
class CharDelWndProc : public CWndProc  
{
public:
	
	TCHAR Name[256];
#ifdef DECO_RENEWAL_MJH
	TCHAR UserName[256];
	TCHAR PassWord[256];
#endif // DECO_RENEWAL_MJH
	int	BufLen;
	
	
	//==============================================================================
	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );
	//==============================================================================
	
	CFrameWnd*		m_pFrameWnd;			
	
	CButton*			m_OkBtn;
	CButton*			m_CancelBtn;
	
	CTextBox*			m_EffectText;
	
	CEditBox*		m_NameEdit;
#ifdef DECO_RENEWAL_MJH
	CEditBox*		m_UserNameEdit;
	CEditBox*		m_passWordEdit;
#endif // DECO_RENEWAL_MJH
	
	//==============================================================================
	void Open();
	void Close();
	
	
	int IsCheckInSpace(void *Str);
	bool CreateNameAndVerification();
	bool IsCheckSymbol(void *SrcStr , void *DstStr);
	
	
	
	
	CharDelWndProc();
	virtual ~CharDelWndProc(){}
};


extern  CharDelWndProc	g_CharDelWndProc;


#endif// #ifndef CHAR_DEL_WND_PROC_H