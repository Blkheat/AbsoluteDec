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

	///-- ���� Ŭ������ ��ȭ�Ҷ� �ش� �Լ��� ȣ���ؼ� ���̵� ����Ÿ�� �簻���Ѵ�.
	void RefreshGuideDataList();
	///-- �������� - ���� ������ �´� ���̵� ���� �ý��� �޽����� ����Ѵ�.
	void PrintNowLevelGuide();
	///-- �����ϰ� ���� Ŭ������ �´� ���̵幮 �����͸� �����´�.
	char * GetRandomHelpGuide();
	
	void SetListScroll( int _linenum );
	void AllClearData();
	
	void Open();
	void Close();

	void InsertStrLine();
	void SetGuideField();

	CFrameWnd * m_pGuideFrameWnd;		///-- �ܰ� ������	

protected:
	GuideDataList m_GuideDataList;	

	int			m_nStartListNum;

	CTextBox * m_GuideField[17];		///-- ����鼭 ��� �ؽ�Ʈ �ڽ�
	CButton	*  m_pCloseBtn;				///-- Ŭ���� ��ư
	CScrollBar * m_pScrollBar;			///-- ��ũ�ѹ�
	
//	char	   m_strLine[120][76];		///-- ���ڿ�����	
	char	   m_strLine[200][100];		///-- ���ڿ�����		
};

extern CGuideWnd * g_pGuideWnd;

#endif