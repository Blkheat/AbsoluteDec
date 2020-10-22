#ifndef __MLINEEDITBOX_H__
#define __MLINEEDITBOX_H__
//-----------------------------------------------------------------------------

#include "EditBox.h"

class CMLineEditBox : public CEditBox
{
protected:
	bool	IsReturnEnterKey;		 //앤터키가 개행문자의 역활을 하는지의 여부
	
	//자동으로 최대라인과 한라인당 글자수를 구하는 루틴을 구하기전까지 임시로 사용

	int		m_nMaxLineNum;			 //최대라인 개수

	int		m_nMaxLastLineLen;		 //마지막 라인 최대길이   

	int		m_nLineNum;				 //라인수

	int		m_nLastStartIndex;		 //라스트라인 시작인덱스	
	TCHAR	m_strLastLine[256];		 //마지막 라인글자	

protected:
	void CreateWindowMLEditBox();

public:
	CMLineEditBox() {}; 
	CMLineEditBox( SDesktop* DT );
	virtual ~CMLineEditBox();

	//-------------------------------------------------------------------------
	virtual SEventMessage* EventProc( SEventMessage* EMsg );//OR
	virtual void RenderProc();//OR
	virtual IRESULT ReceiveEvent( NEventMessage Event, EPARAM fParam, EPARAM sParam, EPARAM xParam, EPARAM exParam );//OR
	//-------------------------------------------------------------------------

	void InitMLEditBox();

	virtual void DrawText();
	virtual void DrawCaret();

	virtual LRESULT CALLBACK EditSubProc( HWND hEdit, UINT iMsg, WPARAM wParam, LPARAM lParam );
	virtual LRESULT CALLBACK EditSubProc_JP( HWND hEdit, UINT iMsg, WPARAM wParam, LPARAM lParam );
	virtual LRESULT CALLBACK EditSubProc_IME( HWND hEdit, UINT iMsg, WPARAM wParam, LPARAM lParam );	

public:
	void SetReturnEnterKey(bool bFlag)	{  IsReturnEnterKey = bFlag;  }
	void SetMaxLineNum(int nNum)			{  m_nMaxLineNum = nNum;		    }
	void SetMaxLastLineLen(int nLineLen)	{  m_nMaxLastLineLen = nLineLen;	}
};

//-----------------------------------------------------------------------------
#endif	__MLINEEDITBOX_H__
