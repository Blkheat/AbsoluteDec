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
	GMODE_INITLOAD,		/// 최초 로딩 : 회사 로고 디스플레이 
	GMODE_LOGINPAGE,	/// 로그인 페이지 - 로그인창, 서버 선택 창 : 3D 배경, 마우스 포인터 
	GMODE_SELECT,		/// 캐릭터 선택 페이지 : 3D 배경, 마우스 포인터 
	GMODE_WORLDINIT,	/// 게임 월드 진입을 위한 최종 준비 
	GMODE_NORMAL,		/// 게임 진행 
	GMODE_MAPLOAD,		/// 워프시 맵 로딩 
};

	

extern	WNDID	g_FocusWnd;
//-----------------------------------------------------------------------------
#endif	__GLOBAL_H__