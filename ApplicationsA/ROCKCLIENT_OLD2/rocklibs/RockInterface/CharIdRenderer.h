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
	
	D3DXVECTOR3			m_vPos;				//�׷����� �� �� �Ʒ��� ������ �߽���ġ
	
	BOOL				m_IsRenderEmblen;			
	BOOL				m_IsRenderCallName;		
	BOOL				m_IsRenderCharName;		
	BOOL				m_IsRenderFtierName;	
	BOOL				m_IsRenderPartyName;
	
	SRect				m_FrameRect;		//������ ����� �簢����
	SRect				m_EmblenRect;		//������ �׷����� �� ����
	SRect				m_CallNameRect;		//ȣĪ�� �׷����� �� ����
	SRect				m_CharNameRect;		//�̸��� �׷����� �� ����
	SRect				m_FtierNameRect;	//����Ƽ�� �̸��� �׷����� �� ����
	SRect				m_PartyNameRect;	//��Ƽ �̸��� �׷����� �� ���� 
	
	///-- �߱� ���� PK ����Ʈ - ī�� ǥ�� �۾�
	BOOL				m_IsRenderCaoMark;	//ī����ũ�� �׷����� �ϴ��� ����
	SRect				m_CaoMarkRect;		//ī����ũ�� �׷����� �� ����
	
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