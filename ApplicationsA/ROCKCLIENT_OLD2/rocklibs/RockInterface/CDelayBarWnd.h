// DelayBarWnd.h: interface for the CDelayBarWnd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DELAYBARWND_H__D3B1AFAA_212A_4E4B_AFB8_E438DCFF4357__INCLUDED_)
#define AFX_DELAYBARWND_H__D3B1AFAA_212A_4E4B_AFB8_E438DCFF4357__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WndProc.h"

enum NDELAY_TYPE
{
   DELAY_NONE = 0,	   
   DELAY_LOG_OFF,
   DELAY_GAME_OUT,
   DELAY_INVEN_RETURN,
   DELAY_MAIN_RETURN,
   DELAY_RIDE_ON,
   DELAY_RIDE_OFF,
   DELAY_PVP_ROOM,
};

class CDelayBarWnd : public CWndProc  
{
protected:
	BOOL           m_BStart;                       //���� ����
	BOOL           m_BActivate;                    //Ȱ��ȭ ����
	BOOL		   m_BIsDie;					   //�����̰� ���۵Ǳ����� �׾��ִ����� ����		

	NDELAY_TYPE    m_TDelay_Type;                  //������ Ÿ��
	CProgressBar  *m_pProgressBar;                 //����� ������
	CTextBox      *m_pTextBox;                     //�ؽ�Ʈ �ڽ� ������
   
    float          m_fDelay_Time;
	float          m_fProgress_Time;

	long           m_lPrevTime;

	DWORD		   m_DwTempItemUID;
	TCHAR          m_tstrNofify[512];
	
    D3DXVECTOR3    m_vPlayerPos;
	D3DXVECTOR3    m_vOldPlayerPos;

public:
	CDelayBarWnd();
	virtual ~CDelayBarWnd();

	virtual void Init();
    virtual void Composition();
    virtual void Update();
    virtual void Proc( SEventMessage* EMsg );

public:
	BOOL IsStart();                    
	BOOL IsActive()                   {   return  m_BActivate ;  }
    void SetActive(BOOL bFalg)        {   m_BActivate =  bFalg;  }

	void ResetDelay(NDELAY_TYPE DType, DWORD DwTempId, BOOL IsDie);
	void ProcessDelayAfter();
	void ProcessDelayLogOff();
	void ProcessGameOut();
	void ProcessDelayInven_Return();
    void ProcessDelayMain_Return();
	void ProcessRideOn();
	void ProcessRideOff();
	void ProcessPvPRoom();

	NDELAY_TYPE GetDelay_Type()		{ return m_TDelay_Type; }
	void Clear();
	void Close();

};

extern CDelayBarWnd g_DelayBarWnd;

#endif // !defined(AFX_DELAYBARWND_H__D3B1AFAA_212A_4E4B_AFB8_E438DCFF4357__INCLUDED_)
