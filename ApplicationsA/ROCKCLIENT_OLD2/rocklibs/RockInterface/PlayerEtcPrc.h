// PlayerEtcPrc.h: interface for the PlayerEtcPrc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLAYERETCPRC_H__931737E6_1DF4_4520_B7A4_2C3FA8A2956A__INCLUDED_)
#define AFX_PLAYERETCPRC_H__931737E6_1DF4_4520_B7A4_2C3FA8A2956A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//기타 플레이어의 정보를 처리한다.
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
	//플레이어 위치값을 랜덤으로 찍어준다. 
	void PlayerPosRandom();

};


extern PlayerEtcPrc g_PlayerEtcPrc;

#endif // !defined(AFX_PLAYERETCPRC_H__931737E6_1DF4_4520_B7A4_2C3FA8A2956A__INCLUDED_)
