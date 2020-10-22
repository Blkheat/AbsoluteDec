#ifndef __MFRIENDTABWND_H__
#define __MFRIENDTABWND_H__
//-----------------------------------------------------------------------------

#include "WndProc.h"
#include "LimitLenTextBox.h"
#include "FrameTextBox.h"

#define MAX_SFRIEND_DATA_NUM	 52

enum SORTPOLICY
{
	SORT_ASCNAME = 0,
	SORT_DESCNAME,
	SORT_ASCAREA,
	SORT_DESCAREA,
	SORT_ASCCHANEL,
	SORT_DESCCHANEL,
	SORT_ASCLEVEL,
	SORT_DESCLEVEL,
	SORT_ASCJOB,
	SORT_DESCJOB,
};


struct SFriendData
{
	TCHAR strFName[512];			//친구이름
	bool  IsSameChanel;				//같은 채널여부
	UINT  uiMapID;					//맵 아이디
	int	  nChanelNum;				//채널 넘버
	int	  iLevel;					//레벨
	UINT  uiJob;					//직업
	bool  bOpenArea;				//지역 공개여부
	bool  bOpenLevel;				//레벨 공개여부
	bool  bOpenJob;					//직업 공개여부
	UINT  uFriendState;				//친구 상태표시
	BYTE  bCitizen;
	SFriendData()
	{
		IsSameChanel = true;
		bCitizen = 0 ; 
	}
};

struct SFriendField
{
	SFriendData*			pFriendData;
	CLimitLenTextBox*		pFNameTxt;			//이름
	CLimitLenTextBox*		pFAreaTxt;			//지역
	CTextBox*				pFChanelTxt;		//채널
	CTextBox*				pFLevelTxt;			//레벨
	CImageBox*				pFJobImg;			//직업이미지
	CTextBox*				pFJobTxt;			//직업 - ?표시
	CTextBox*				pFStateTxt;			//상태표시

	SFriendField()
	{
		pFriendData = NULL;
	}
};

extern int ascendNameCmp( const void *arg1, const void *arg2 );
extern int descendNameCmp( const void *arg1, const void *arg2 );
extern int ascendAreaCmp( const void *arg1, const void *arg2 );
extern int descendAreaCmp( const void *arg1, const void *arg2 );
extern int ascendChanelCmp( const void *arg1, const void *arg2 );
extern int descendChanelCmp( const void *arg1, const void *arg2 );
extern int ascendLevelCmp( const void *arg1, const void *arg2 );
extern int descendLevelCmp( const void *arg1, const void *arg2 );
extern int ascendJobCmp( const void *arg1, const void *arg2 );
extern int descendJobCmp( const void *arg1, const void *arg2 );

class CMFriendTabWnd : public CWndProc
{
protected:
	CFrameWnd	* m_pMFieldTabWnd;
	
	CButton		* m_pNameFieldBtn;
	CButton		* m_pAreaFieldBtn;
	CButton		* m_pChanelFieldBtn;
	CButton		* m_pLevelFieldBtn;
	CButton		* m_pJobFieldBtn;

	SFriendField	FriendField[6];
	CButton*		FriendSelBtn[6];

	CFrameTextBox*	m_FocusOriginTxt;
	CFrameTextBox*	m_FocusToolTipTxt;	
	CImageBox *			m_BackImg;		//상태 타이틀

	SFriendData		* m_pFriendDataList[MAX_SFRIEND_DATA_NUM + 1];

	int				m_nNumOfFriend;
	int				m_nStartListNum;

	int				m_nSelectedBtn;				//선택버튼

	UINT			m_uSortPolicy;				//현재 소팅정책
	
	bool			m_bAscNameSort;				//이름부분 오름차순 정렬여부
	bool			m_bAscAreaSort;				//지역부분 오름차순 정렬여부
	bool			m_bAscChanelSort;			//채널부분 오름차순 정렬여부
	bool			m_bAscLevelSort;			//레벨부분 오름차순 정렬여부
	bool			m_bAscJobSort;				//직업부분 오름차순 정렬여부

public:
	CMFriendTabWnd();
	~CMFriendTabWnd();

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );

	TEXID GetClassIcon(int iClass);	
	int GetClassNation(int iClass);

	void  InsertFriendData(SFriendData * pFriendData, bool bReNewFlag);
	void  DeleteFriendData(TCHAR * m_strParam, bool bReNewFlag);

	void  SortFriendData();
	void  SetFriendField();

	void  SetListScroll( int _linenum );
	void  FieldBntSelected( int _num_btn );

	SFriendData * GetSelectData();
	SFriendData * FindFriendData(TCHAR * strName);
	UINT  GetMFieldTabWndId()	{ 
									if(m_pMFieldTabWnd)
									{
										return m_pMFieldTabWnd->GetID();
									}
									return 0;
								}	
};

//-----------------------------------------------------------------------------
#endif	__MFRIENDTABWND_H__