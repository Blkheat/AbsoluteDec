#ifndef __CCOLITEMPREVIEWWND_H__
#define __CCOLITEMPREVIEWWND_H__
//-----------------------------------------------------------------------------
#include "..\\RockPCH.h"
#ifdef DIRECT_VERSION_9_MJH
#include <d3dx9.h>
#else
#include <d3dx8.h>
#endif // DIRECT_VERSION_9_MJH

#include "Define.h"

#include "Fontman.h"
#include "Render.h"

enum N_COLONY_ITEMTIP_TYPE
{
	n_COLONY_POSITION,
	n_COLONY_ATTCKARMS,
	n_COLONY_BULDING,
	n_COLONY_GTOWER,
	n_COLONY_NONE,
};

///-- �ݷδ� ������ �̸�����â
class CColItemPreViewWnd
{

public:
	CColItemPreViewWnd();
	~CColItemPreViewWnd();	

protected:
	
	WNDID			m_SlotWndID;
	SPcItem*		m_PcItem;
	SDesktop*		m_pDT;

public:	
	void Initialize(SDesktop* DT);
	void Set( WNDID wndid, SRect* wc_size, SPcItem* pcitem );
	void CalcClientSize( SRect* wc_size );
	void CalcFrameSize();
	
	void InitInfo();
	void SetItemNoaml();
	void SetItemPosition();
	void SetItemAttackArams();
	void SetItemBuilding();
	void SetItemGuardTower();
	
	void RenderProc();
	void DrawNormal();
	void DrawItemPosition();
	void DrawItemAttackArams();
	void DrawItemBuilding();
	void DrawItemGuardTower();

	D3DCOLOR GetRareColor( BYTE rare );
	
	SStringInfo		m_StrInfo;

	SRect			m_ClientRect;
	
	SRect			m_NorthFrm;
	SRect			m_NorthEastFrm;
	SRect			m_EastFrm;
	SRect			m_SouthEastFrm;
	SRect			m_SouthFrm;
	SRect			m_SouthWestFrm;
	SRect			m_WestFrm;
	SRect			m_NorthWestFrm;

	int				m_FrameWidth;
	
	N_COLONY_ITEMTIP_TYPE m_ItemType;

	SRect			m_DetailImgRect;
	
	///-- ������	
	///-- ������ �̸�
	STipString		m_ItemName;
	
	TEXID			m_DetailImgID;			///-- �� �̹���

	STipString		m_CallNpcList;			///-- ��ȯ���
	
	///-- ��ȯ ���������� ����
	STipString		m_CallNPCName1;			///-- ȣ�� NPC
	STipString		m_MAXHP1;				///-- �ִ� HP

	STipString		m_AttackAndMagic1;		///--�������� and ��������
	STipString		m_DefenceAndMagicRes1;	///--������� and �������


	STipString		m_CallNPCName2;			///-- ȣ�� NPC
	STipString		m_MAXHP2;				///-- �ִ� HP

	STipString		m_AttackAndMagic2;		///--�������� and ��������
	STipString		m_DefenceAndMagicRes2;	///--������� and �������

	STipString		m_CallNPCName3;			///-- ȣ�� NPC
	STipString		m_MAXHP3;				///-- �ִ� HP

	STipString		m_AttackAndMagic3;		///--�������� and ��������
	STipString		m_DefenceAndMagicRes3;	///--������� and �������	
	
	///-- ������ ����
	STipString		m_ItemDesc[10];	
	Int				m_DescLineNum;

public:
	CFontg*			m_Fontg;   //by simwoosung ��ġ����
	D3DCOLOR		m_TColor;
};

extern CColItemPreViewWnd g_ColItemPreViewWnd;

#endif