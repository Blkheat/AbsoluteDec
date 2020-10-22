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
	TCHAR strFName[512];			//ģ���̸�
	bool  IsSameChanel;				//���� ä�ο���
	UINT  uiMapID;					//�� ���̵�
	int	  nChanelNum;				//ä�� �ѹ�
	int	  iLevel;					//����
	UINT  uiJob;					//����
	bool  bOpenArea;				//���� ��������
	bool  bOpenLevel;				//���� ��������
	bool  bOpenJob;					//���� ��������
	UINT  uFriendState;				//ģ�� ����ǥ��
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
	CLimitLenTextBox*		pFNameTxt;			//�̸�
	CLimitLenTextBox*		pFAreaTxt;			//����
	CTextBox*				pFChanelTxt;		//ä��
	CTextBox*				pFLevelTxt;			//����
	CImageBox*				pFJobImg;			//�����̹���
	CTextBox*				pFJobTxt;			//���� - ?ǥ��
	CTextBox*				pFStateTxt;			//����ǥ��

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
	CImageBox *			m_BackImg;		//���� Ÿ��Ʋ

	SFriendData		* m_pFriendDataList[MAX_SFRIEND_DATA_NUM + 1];

	int				m_nNumOfFriend;
	int				m_nStartListNum;

	int				m_nSelectedBtn;				//���ù�ư

	UINT			m_uSortPolicy;				//���� ������å
	
	bool			m_bAscNameSort;				//�̸��κ� �������� ���Ŀ���
	bool			m_bAscAreaSort;				//�����κ� �������� ���Ŀ���
	bool			m_bAscChanelSort;			//ä�κκ� �������� ���Ŀ���
	bool			m_bAscLevelSort;			//�����κ� �������� ���Ŀ���
	bool			m_bAscJobSort;				//�����κ� �������� ���Ŀ���

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