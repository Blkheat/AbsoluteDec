// AliveWnd.h: interface for the CAliveWnd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ALIVEWND_H__7DC8B540_F7EF_4A88_A38B_D565E6A43C58__INCLUDED_)
#define AFX_ALIVEWND_H__7DC8B540_F7EF_4A88_A38B_D565E6A43C58__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



const WORD c_Max_AliveAnswerBtn_Num( 5 );


typedef enum NAliveAnswerWndType
{
	n_AnswerWndType_Alive = 0, 
	n_AnswerWndType_Observer_RoomOut, 

} NAliveAnswerWndType;


typedef enum NAliveAnswerType
{
	n_AnswerType_None = 0,
	n_AnswerType_Alive,
	n_AnswerType_UseChargeitem,
	n_AnswerType_Observer, 
	n_AnswerType_RoomOut,

} NAliveAnswerType;


typedef struct SAliveAnswerBtn
{
	NAliveAnswerType		answer_type;
	CButton*				select_btn;
	
} SAliveAnswerBtn;


class CAliveWnd : public CWndProc
{
public:
	NAliveAnswerType		m_AliveType;
	
	///--JAPAN_BUG_MODIFY
	CFrameWnd			*	m_pMainFrame; 

public:
	CAliveWnd();
	virtual ~CAliveWnd();

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );

	void Open( NAliveAnswerWndType aAliveAnswerWndType );
	void ClickAnswerBtn( NAliveAnswerType aAliveAnswerType );

private:
	SAliveAnswerBtn			m_AliveAnwserBtn[ c_Max_AliveAnswerBtn_Num ];
	
};

class CAliveChargeWnd : public CWndProc
{
public:
	CAliveChargeWnd();
	virtual ~CAliveChargeWnd();

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );

protected:	
	CFrameWnd*		m_pFramwWnd;

	SPcItem			m_Chargeslot_item[3];
	CSlot*			m_pChargeitem_slot[3];
	CButton*		m_pClose_btn;

public:
	bool			OpenWnd();
	void			CloseWnd();
};

extern	CAliveWnd	g_AliveWnd;
extern	CAliveChargeWnd g_AliveChargeWnd;


#endif // !defined(AFX_ALIVEWND_H__7DC8B540_F7EF_4A88_A38B_D565E6A43C58__INCLUDED_)



