#ifndef __MLINEEDITBOX_H__
#define __MLINEEDITBOX_H__
//-----------------------------------------------------------------------------

#include "EditBox.h"

class CMLineEditBox : public CEditBox
{
protected:
	bool	IsReturnEnterKey;		 //����Ű�� ���๮���� ��Ȱ�� �ϴ����� ����
	
	//�ڵ����� �ִ���ΰ� �Ѷ��δ� ���ڼ��� ���ϴ� ��ƾ�� ���ϱ������� �ӽ÷� ���

	int		m_nMaxLineNum;			 //�ִ���� ����

	int		m_nMaxLastLineLen;		 //������ ���� �ִ����   

	int		m_nLineNum;				 //���μ�

	int		m_nLastStartIndex;		 //��Ʈ���� �����ε���	
	TCHAR	m_strLastLine[256];		 //������ ���α���	

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
