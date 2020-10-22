// ----------------------------------------------------------------------------------------------------
// D3DBase.H
// Direct3D Display 폴더 내의 헤더들을 정리
// Direct3D Display 폴더 내의 전역 변수들 외부 선언
// ----------------------------------------------------------------------------------------------------
#ifndef __D3DBASE_H__
#define __D3DBASE_H__

#ifdef DIRECT_VERSION_9_MJH
#include	<d3d9.h>
#include	<d3dx9.h>
#else
#include	<d3d8.h>
#include	<d3dx8.h>
#endif // DIRECT_VERSION_9_MJH
#include	<Base\\d3dapp.h>
#include	<Base\\d3dfont.h>
#include	<Base\\d3dutil.h>
#include	<Base\\dxutil.h>

#endif