#ifndef __MBLOCKTABWND_H__
#define __MBLOCKTABWND_H__
//-----------------------------------------------------------------------------

#include "WndProc.h"
#include "LimitLenTextBox.h"
#include "FrameTextBox.h"

#define MAX_SBLOCK_DATA_NUM		32
#define MAX_BLOCKFIELD_LINE		20

struct SBlockData
{
	TCHAR strBDestName[512];			//블록 대상이름
	SBlockData()
	{
		memset(strBDestName , NULL , sizeof(strBDestName));
	}
};

struct SBlockField
{
	SBlockData			*  pBlockData;
	CLimitLenTextBox	*  pBDestName;			//대상 이름
	
	SBlockField()
	{
		pBlockData = NULL;
	}
};

class CMBlockTabWnd : public CWndProc
{
protected:
	CFrameWnd	* m_pMBlockTabWnd;
	CTextBox	* m_pBlockTitleField;

	SBlockField	  BlockField[MAX_BLOCKFIELD_LINE];
	CButton*	  BlockSelBtn[MAX_BLOCKFIELD_LINE];
	CImageBox *			m_BackImg;		//상태 Image

	CFrameTextBox*	m_FocusOriginTxt;

	SBlockData	* m_pBlockDataList[MAX_SBLOCK_DATA_NUM + 1];
	int			m_nNumOfBlock;
	int			m_nStartListNum;

	int			m_nSelectedBtn;				//선택버튼

public:
	CMBlockTabWnd();
	~CMBlockTabWnd();

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );

public:
	void InsertBlockData(SBlockData * pBlockData, bool bReNewFlag);
	void SetBlockField();
	void FieldBntSelected( int _num_btn );
	void SetListScroll( int _linenum );
	SBlockData * GetSelectData();
	void DeleteBlockData(TCHAR * strName, bool bReNewFlag);
	bool ExistBlockName(TCHAR *strName);
};

//-----------------------------------------------------------------------------
#endif	__MBLOCKTABWND_H__