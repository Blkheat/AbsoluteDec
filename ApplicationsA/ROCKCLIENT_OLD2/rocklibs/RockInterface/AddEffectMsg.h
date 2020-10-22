// AddEffectMsg.h: interface for the AddEffectMsg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ADDEFFECTMSG_H__A6F29631_A848_493E_AFB1_AF97EBEF6421__INCLUDED_)
#define AFX_ADDEFFECTMSG_H__A6F29631_A848_493E_AFB1_AF97EBEF6421__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

const DWORD ADDEFFECT_MAXCOUNT = 6;


class AddEffectMsg : public CWndProc

{
private:
	BYTE m_Effect_State[MAX_ADDEFFECT_NUM];	//효과 상태 
	
public:
	AddEffectMsg();
	virtual ~AddEffectMsg();

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );
	CFrameWnd *m_pFrameWnd;
	CNoImageBox * m_BgImgBox;
	CTextBox * m_TextEffectMsg[ADDEFFECT_MAXCOUNT];		

	void UpdateEffectMessage(BYTE *Effect_State);
};

extern AddEffectMsg * g_pAddEffectMsg;


#endif // !defined(AFX_ADDEFFECTMSG_H__A6F29631_A848_493E_AFB1_AF97EBEF6421__INCLUDED_)
