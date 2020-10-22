///////////////////////////////////////////////////////////////////////////////
///
///		File		: global.h
///		Desc		:
///
///		Author		: Park Soo-Hyun
///		Team		: Program - Client Team
///		Date		: 2004-05-20
///
///		Copyright(c) 2004, Rocksoft Co., Ltd. ALL RIGHTS RESERVED
///
///////////////////////////////////////////////////////////////////////////////
#ifndef __GLOBAL_H__
#define __GLOBAL_H__
//-----------------------------------------------------------------------------

enum		GAMEMODE
{
	GMODE_INITLOAD,		/// ���� �ε� : ȸ�� �ΰ� ���÷��� 
	GMODE_LOGINPAGE,	/// �α��� ������ - �α���â, ���� ���� â : 3D ���, ���콺 ������ 
	GMODE_SELECT,		/// ĳ���� ���� ������ : 3D ���, ���콺 ������ 
	GMODE_WORLDINIT,	/// ���� ���� ������ ���� ���� �غ� 
	GMODE_NORMAL,		/// ���� ���� 
	GMODE_MAPLOAD,		/// ������ �� �ε� 
};

	

extern	WNDID	g_FocusWnd;
//-----------------------------------------------------------------------------
#endif	__GLOBAL_H__