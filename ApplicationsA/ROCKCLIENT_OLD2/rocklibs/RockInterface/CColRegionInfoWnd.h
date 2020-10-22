#ifndef __CCOLREGIONINFOWND_H__
#define __CCOLREGIONINFOWND_H__
//-----------------------------------------------------------------------------

#include "Define.h"
#include "WndProc.h"

///-- 콜로니 지역 정보창

class CColRegionInfoWnd : public CWndProc
{
public:
	SColAreaData	*  m_pLinkColAreaData;		//연결된 콜로니 지역정보

protected:
	CFrameWnd		*  m_pColRInfoFrame;
	CButton			*  m_pCloseBtn;				//클로즈 버튼

	CTextBox		*  m_pColRegionName;		//지역정보
	CTextBox		*  m_pColOccuFronName;		//점령 프론티어 네임
	CTextBox		*  m_pColFronMasterName;	//점령 주프론티어 마스터 네임
	CTextBox		*  m_pColFronMemNumber;		//전투 인원
	CTextBox		*  m_pColDeclWarCanDate;	//선전포고가능시간 or 이미 선전포고된 시간
	CTextBox		*  m_pColAttckFName;	    //이미 선전포고한 프론티어 이름 
	
	CButton			*  m_pPrevBtn;				//이전단계 버튼
	CButton			*  m_pDeclWar;				//선전포고 버튼
	
	BOOL			m_IsButton_lock;			//두번 선전포고 하는것을 막기위한 버튼

public:
	CColRegionInfoWnd();
	~CColRegionInfoWnd();

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );

public:
	void Open();
	void Close();	
	void LogOff();

	void OpenColListWnd();

	void SetLinkColAreaData(SColAreaData * pLinkColAreaData);
	void RefreshColRegionInfo();
};

extern CColRegionInfoWnd g_ColRegionInfoWnd;

#endif