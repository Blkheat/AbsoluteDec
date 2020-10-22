//----------------------------------------------------------------------------------------------------------------------
// 파일명 : CharCreateWndProc.h
// 용  도 : 캐릭터 생성창
//----------------------------------------------------------------------------------------------------------------------
#ifndef CHAR_CREATE_WND_PROC_H
#define CHAR_CREATE_WND_PROC_H


#include "WndProc.h"


class CFrameWnd;
class CButton;
class CTextBox;
class CharCreateWndProc : public CWndProc
{
public:
	CharCreateWndProc();
	~CharCreateWndProc();
	
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );
	
	void Open();
	void Close();
	bool CheckFocusWnd( UINT  FocusWnd );
	

protected:
	CFrameWnd * m_pFrameWnd;
	
	CButton *   m_Create_Millena_Btn;
	CButton *	m_Create_Rain_Btn;
	CTextBox *	m_nationtext_millena;
	CTextBox *  m_nationtext_rain;
};



#ifdef DECO_RENEWAL_MJH

class CharCreateHairProc : public CWndProc
{
public:
	CharCreateHairProc();
	~CharCreateHairProc();
	
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );
	
	void Open();
	void Close();
	bool CheckFocusWnd( UINT  FocusWnd );
	

protected:
	CFrameWnd * m_pFrameWnd;
	
	CButton *   m_Create_Hair1;
	CButton *	m_Create_Hair2;
	CButton *   m_Create_Hair3;
	CButton *	m_Create_Hair4;
	CButton *   m_Create_Hair5;
	CButton *	m_Create_Hair6;
};
extern CharCreateHairProc	g_CharCreateHairProc;

#endif // DECO_RENEWAL_MJH
extern CharCreateWndProc	g_CharCreateWndProc;

#endif// #ifndef CHAR_CREATE_WND_PROC_H