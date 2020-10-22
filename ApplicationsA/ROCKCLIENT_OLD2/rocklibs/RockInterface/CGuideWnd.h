#ifndef __CGUIDEWND_H__
#define __CGUIDEWND_H__
//-----------------------------------------------------------------------------

#include "WndProc.h"
#include "FrameWnd.h"
#include "ScrollBar.h"
#include <vector>

using namespace std;

#pragma pack(push,1)
struct SGuideData
{
	DWORD	dIndex;
	DWORD	dClassNum;
	DWORD   dLevel;
	char	strDesc[300];

	SGuideData()
	{
		memset(this, 0, sizeof(SGuideData));
	}
};
#pragma pack(pop)

typedef vector < SGuideData * > GuideDataList;
typedef vector < SGuideData * > ::iterator GuideDataListPos;

class CGuideWnd : public CWndProc
{
public:
	CGuideWnd();
	virtual ~CGuideWnd();

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );
	
	void SetWheelValue( short _wheel );

	///-- 메인 클래스가 변화할때 해당 함수를 호출해서 가이드 데이타를 재갱신한다.
	void RefreshGuideDataList();
	///-- 레벨업시 - 현재 레벨에 맞는 가이드 문을 시스템 메시지로 출력한다.
	void PrintNowLevelGuide();
	///-- 램덤하게 현재 클래스에 맞는 가이드문 데이터를 가져온다.
	char * GetRandomHelpGuide();
	
	void SetListScroll( int _linenum );
	void AllClearData();
	
	void Open();
	void Close();

	void InsertStrLine();
	void SetGuideField();

	CFrameWnd * m_pGuideFrameWnd;		///-- 외곽 윈도우	

protected:
	GuideDataList m_GuideDataList;	

	int			m_nStartListNum;

	CTextBox * m_GuideField[17];		///-- 모험백서 출력 텍스트 박스
	CButton	*  m_pCloseBtn;				///-- 클로즈 버튼
	CScrollBar * m_pScrollBar;			///-- 스크롤바
	
//	char	   m_strLine[120][76];		///-- 문자열라인	
	char	   m_strLine[200][100];		///-- 문자열라인		
};

extern CGuideWnd * g_pGuideWnd;

#endif