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
	CFlashFrameWnd*			m_pPartyMemWnd;						//파티 멤버
	CImageBox *				m_pPartyMemSelectImg;				//파티장 이미지

protected:
	CCharImgButton*			m_pPartyMemCharImgBtn;				//파티 이미지 버튼(멤버추가)
	CImageBox *				m_pPartyMemMasterImg;				//파티장 이미지
	CImageBox *				m_pPartyMemFKeyTypeImg;				//파티멤버 F1타입 이미지
	CTextBox *				m_pPartyMemLevelTxt;				//파티멤버 레벨 텍스트
	CImageBox *				m_pPartyMemComboImg[6];				//파티 멤버 콤보이미지
	CLimitLenTextBox*		m_pPartyMemNameTxt;					//파티 멤버 이름텍스트
	CProgressBar *          m_pPartyMemHPGauge;					//파티 멤버 HP게이지
	CProgressBar *			m_pPartyMemSPMPGauge;				//파티 멤버 SPMP게이지

	CTextBox*		m_HPText;									//파티 멤버 HP넘버
	CTextBox*		m_SMPText;									//파티 멤버 SMP넘버
	
	

protected:	  
	int m_nIndex;	 //멤버 인덱스번호
	
	SPARTYMEMBER_INFO * m_pMemberInfo;							//파티 멤버 정보
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