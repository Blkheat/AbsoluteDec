// PlayerEtcPrc.h: interface for the PlayerEtcPrc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLAYERETCPRC_H__931737E6_1DF4_4520_B7A4_2C3FA8A2956A__INCLUDED_)
#define AFX_PLAYERETCPRC_H__931737E6_1DF4_4520_B7A4_2C3FA8A2956A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//��Ÿ �÷��̾��� ������ ó���Ѵ�.
#include "RockPch.h"

class PlayerEtcPrc  
{

	bool m_bStartCheck;
	DWORD m_PrevTime;

public:
	PlayerEtcPrc();
	virtual ~PlayerEtcPrc();

	void Update();
	void Init();
	//�÷��̾� ��ġ���� �������� ����ش�. 
	void PlayerPosRandom();

};


extern PlayerEtcPrc g_PlayerEtcPrc;

#endif // !defined(AFX_PLAYERETCPRC_H__931737E6_1DF4_4520_B7A4_2C3FA8A2956A__INCLUDED_)
