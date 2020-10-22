#ifndef __CCOLMAPPOSIOTIONWND_H__
#define __CCOLMAPPOSIOTIONWND_H__
//-----------------------------------------------------------------------------

#include "WndProc.h"
#include "CheckButton.h"
#include "CArrowLineList.h"

///-- �ݷδ� ����/�������� �ʹ�ġâ
class CColMapPositionWnd : public CWndProc
{
public:
	CFrameWnd * m_pOutFrameWnd;						///-- ��ġâ �ܰ� ������
	CButton *  m_pCloseBtn;							///-- �ݱ� ��ư
	CTextBox * m_pMapName;							///-- ���̸� ǥ��				

	
	CFrameWnd * m_pMapViewWnd;						///-- �̴ϸ� �ܰ� ������
	CCheckButton  *	m_pBatchCButton[3];				///-- ��ġ ����Ʈ
	CArrowLineList * m_pArrowLineList[3];			///-- �������� �̵� ��� ����
	SNPCBaseInfo * m_pBachNPCInfo[3];				///-- ��ġ�� NPC �̸�

	int			 m_nNowMapId;						///-- ���� �� ���̵�	
	TEXID		 m_NowMapTexId;						///-- �̴ϸ� �̹���

	int			m_nSelectPosition;					///-- ���õ� ��ġ����Ʈ	
	
	SPcItem	 m_LinkColItem;							///-- ��ũ �ݷδ� ������
	UINT	 m_ColCallType;							///-- �ݷδ� ������ Ÿ��

	CButton *  m_pOkBtn;							///-- Ȯ�� ��ư
	CButton *  m_pCancleBtn;						///-- ��� ��ư


	//�ݷδ� ������ ���� ������ ���
	UINT		m_WIDToolTipDestWnd;
	TCHAR		m_strToolTipItemName[256];

	int			m_tilesize;
	
	BOOL		m_IsButton_lock;					//�ι� ������ ��� �������� ��ư

public:
	CColMapPositionWnd();
	~CColMapPositionWnd();
	
public:	
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );

	void SetColItem(SPcItem * pItem, UINT nColCallType);
	void Open();
	void Close();	
	void LogOff();

	void Initialize();
	void BatchColItemType();
	void BatchCoPositionGui();
	void BatchCoAttackAramsGui();
	void BatchCoBuildingGui();
	void BatchCoGuardTowerGui();

	void ConvertCellTo2DPoint(DWORD dCell, POINT & sPoint);

	void SetFocusItemBtn( UINT WIDToolTipDestWnd, void *strToolTipItemName );
	void RenderToolItemName();
	void Check_BatchItem();
};

extern CColMapPositionWnd  * g_ColMapPositionWnd;

#endif