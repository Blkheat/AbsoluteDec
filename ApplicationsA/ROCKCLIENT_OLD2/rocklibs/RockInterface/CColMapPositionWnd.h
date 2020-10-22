#ifndef __CCOLMAPPOSIOTIONWND_H__
#define __CCOLMAPPOSIOTIONWND_H__
//-----------------------------------------------------------------------------

#include "WndProc.h"
#include "CheckButton.h"
#include "CArrowLineList.h"

///-- 콜로니 공성/수성병기 맵배치창
class CColMapPositionWnd : public CWndProc
{
public:
	CFrameWnd * m_pOutFrameWnd;						///-- 배치창 외곽 프레임
	CButton *  m_pCloseBtn;							///-- 닫기 버튼
	CTextBox * m_pMapName;							///-- 맵이름 표시				

	
	CFrameWnd * m_pMapViewWnd;						///-- 미니맵 외곽 윈도우
	CCheckButton  *	m_pBatchCButton[3];				///-- 배치 포인트
	CArrowLineList * m_pArrowLineList[3];			///-- 공성병기 이동 경로 라인
	SNPCBaseInfo * m_pBachNPCInfo[3];				///-- 배치된 NPC 이름

	int			 m_nNowMapId;						///-- 현재 맵 아이디	
	TEXID		 m_NowMapTexId;						///-- 미니맵 이미지

	int			m_nSelectPosition;					///-- 선택된 배치포인트	
	
	SPcItem	 m_LinkColItem;							///-- 링크 콜로니 아이템
	UINT	 m_ColCallType;							///-- 콜로니 아이템 타입

	CButton *  m_pOkBtn;							///-- 확인 버튼
	CButton *  m_pCancleBtn;						///-- 취소 버튼


	//콜로니 아이템 툴팁 렌더링 요소
	UINT		m_WIDToolTipDestWnd;
	TCHAR		m_strToolTipItemName[256];

	int			m_tilesize;
	
	BOOL		m_IsButton_lock;					//두번 아이템 사용 막기위한 버튼

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