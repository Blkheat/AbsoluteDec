#if !defined( __RUI_PARTYMEMBER_WND_H__ )
#define __RUI_PARTYMEMBER_WND_H__
//-----------------------------------------------------------------------------

#include "WndProc.h"
#include "LimitLenTextBox.h"
#include "FlashFrameWnd.h"
#include "CharImgButton.h"
#include "PartyBaseInfo.h"

class CPartyMemeberWnd : public CWndProc
{
public:
	CFlashFrameWnd*			m_pPartyMemWnd;						//��Ƽ ���
	CImageBox *				m_pPartyMemSelectImg;				//��Ƽ�� �̹���

protected:
	CCharImgButton*			m_pPartyMemCharImgBtn;				//��Ƽ �̹��� ��ư(����߰�)
	CImageBox *				m_pPartyMemMasterImg;				//��Ƽ�� �̹���
	CImageBox *				m_pPartyMemFKeyTypeImg;				//��Ƽ��� F1Ÿ�� �̹���
	CTextBox *				m_pPartyMemLevelTxt;				//��Ƽ��� ���� �ؽ�Ʈ
	CImageBox *				m_pPartyMemComboImg[6];				//��Ƽ ��� �޺��̹���
	CLimitLenTextBox*		m_pPartyMemNameTxt;					//��Ƽ ��� �̸��ؽ�Ʈ
	CProgressBar *          m_pPartyMemHPGauge;					//��Ƽ ��� HP������
	CProgressBar *			m_pPartyMemSPMPGauge;				//��Ƽ ��� SPMP������

	CTextBox*		m_HPText;									//��Ƽ ��� HP�ѹ�
	CTextBox*		m_SMPText;									//��Ƽ ��� SMP�ѹ�
	
	

protected:	  
	int m_nIndex;	 //��� �ε�����ȣ
	
	SPARTYMEMBER_INFO * m_pMemberInfo;							//��Ƽ ��� ����
	TCHAR			 m_tmpStr[128];
	
protected:	
	void SetComboGauge(int iGage, BOOL BFlag);

public:
	CPartyMemeberWnd();
	~CPartyMemeberWnd();

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );

	void UpdateMemberUnitInfo();

public:
	void Composition(int index, int iXPos, int iYPos);
	void SetPartyMemberInfo( SPARTYMEMBER_INFO * pMemberInfo );
	SPARTYMEMBER_INFO * GetPartyMemberInfo();
};


//-----------------------------------------------------------------------------
#endif  __RUI_PARTYMEMBER_WND_H__