#ifndef __CCOLREGIONINFOWND_H__
#define __CCOLREGIONINFOWND_H__
//-----------------------------------------------------------------------------

#include "Define.h"
#include "WndProc.h"

///-- �ݷδ� ���� ����â

class CColRegionInfoWnd : public CWndProc
{
public:
	SColAreaData	*  m_pLinkColAreaData;		//����� �ݷδ� ��������

protected:
	CFrameWnd		*  m_pColRInfoFrame;
	CButton			*  m_pCloseBtn;				//Ŭ���� ��ư

	CTextBox		*  m_pColRegionName;		//��������
	CTextBox		*  m_pColOccuFronName;		//���� ����Ƽ�� ����
	CTextBox		*  m_pColFronMasterName;	//���� ������Ƽ�� ������ ����
	CTextBox		*  m_pColFronMemNumber;		//���� �ο�
	CTextBox		*  m_pColDeclWarCanDate;	//���������ɽð� or �̹� ��������� �ð�
	CTextBox		*  m_pColAttckFName;	    //�̹� ���������� ����Ƽ�� �̸� 
	
	CButton			*  m_pPrevBtn;				//�����ܰ� ��ư
	CButton			*  m_pDeclWar;				//�������� ��ư
	
	BOOL			m_IsButton_lock;			//�ι� �������� �ϴ°��� �������� ��ư

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