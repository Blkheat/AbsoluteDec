#ifndef __CHARIDRENDERER_H__
#define __CHARIDRENDERER_H__
//-----------------------------------------------------------------------------

#include "Fontg.h"
#include "Define.h"

class CCharIdRenderer
{
protected:
	SPcInfo*			m_pPcInfo;
	CFontg*				m_pFontg;	
	
	TCHAR				m_strTemp[256];
	TCHAR				m_strCharName[256];
	TCHAR				m_strPartyName[256];
	TCHAR				m_strFrontierName[256];
	TCHAR			    m_strCallName[256];
	DWORD				m_uFrontierId;
	BYTE				m_Status;
	
	D3DCOLOR			m_Name_Color;
	D3DCOLOR			m_Party_Color;
	D3DCOLOR			m_Frontier_Color;
	D3DCOLOR			m_CallName_Color;

	int					m_nIndex;
	
	D3DXVECTOR3			m_vPos;				//그려져야 할 맨 아래쪽 라인의 중심위치
	
	BOOL				m_IsRenderEmblen;			
	BOOL				m_IsRenderCallName;		
	BOOL				m_IsRenderCharName;		
	BOOL				m_IsRenderFtierName;	
	BOOL				m_IsRenderPartyName;
	
	SRect				m_FrameRect;		//검은색 배경의 사각영역
	SRect				m_EmblenRect;		//엠블렌이 그려져야 할 영역
	SRect				m_CallNameRect;		//호칭이 그려져야 할 영역
	SRect				m_CharNameRect;		//이름이 그려져야 할 영역
	SRect				m_FtierNameRect;	//프론티어 이름이 그려져야 할 영역
	SRect				m_PartyNameRect;	//파티 이름이 그려져야 할 영역 
	
	///-- 중국 관련 PK 포인트 - 카오 표시 작업
	BOOL				m_IsRenderCaoMark;	//카오마크가 그려져야 하는지 여부
	SRect				m_CaoMarkRect;		//카오마크가 그려져야 할 영역
	
	void Init();

public:
	CCharIdRenderer();
	virtual ~CCharIdRenderer();

	void Init(SPcInfo* pcinfo, CFontg* pFontg);

	void SetPcInfo( SPcInfo* pcinfo );
	BOOL SetIndex( int nIndex );

	void SetFrontierId(DWORD uId);
	void SetCharName(TCHAR *tStr);
	void SetPartyName(TCHAR *tStr);	
	void SetFrontierName(TCHAR *tStr);
	void SetCallName(TCHAR *tStr);
	void SetFrontierStatus(BYTE Status);

	void SetNameColor(D3DCOLOR Color);
	void SetPartyColor(D3DCOLOR Color);
	void SetFrontierColor(D3DCOLOR Color);
	void SetCallNameColor(D3DCOLOR Color);	

	void RecallLayout();
	void RenderProc();

	void SetCaoMark(BOOL bFlag)	{ m_IsRenderCaoMark = bFlag; }
};


//-----------------------------------------------------------------------------
#endif	__CHARIDRENDERER_H__