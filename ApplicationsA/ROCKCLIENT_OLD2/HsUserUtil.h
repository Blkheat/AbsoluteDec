/*********************************************************************************
		Administartors그룹 권한을 가진 계정생성 헤더파일 - HsUserUtil.h
					HsUserUtil V1.0 for Windows 2000, XP, 2003
							(주) 안철수 연구소

	작업자 : 남성일
	날  짜 : 2005년 2월 2일
	버  전 :
	주  석 : Administartors그룹 권한을 가진 계정생성 및 폴더권한 설정
==================================================================================
==================================================================================
	날  짜 : 2005년 2월 1일 

	1) 최초등록
==================================================================================
	날  짜 : 2005년 2월 3일

	1) 	_AhnHsUserUtil_CreateUser ()						함수정의.
	2)  _AhnHsUserUtil_SetFolderPermission(LPTSTR szPath)	함수정의.
	 
*********************************************************************************/

#ifndef _HSUSERUTIL_H
#define _HSUSERUTIL_H

//ERROR CODE 
#define	HSUSERUTIL_ERR_OK						0x00000000		//성공
#define	HSUSERUTIL_ERR_UNKNOWN					0x0005A001		//알수없는 오류
#define	HSUSERUTIL_ERR_NOT_ADMIN				0x0005A002		//Administrator 권한이 아님
#define	HSUSERUTIL_ERR_NOT_NT					0x0005A003		//NT계열이 아님

#define	HSUSERUTIL_ERR_DELSHADOWACNT_FAIL		0x0005A005		//쉐도우계정 패턴삭제 실패
#define	HSUSERUTIL_ERR_DELHIDEIDINFO_FAIL		0x0005A006		//로그온시 유저계정 감추기기 위한 정보삭제 실패
#define	HSUSERUTIL_ERR_DELSHADOWACNTINFO_FAIL	0x0005A007		//쉐도우계정정보 삭제실패
#define	HSUSERUTIL_ERR_ADDSHADOWACNT_FAIL		0x0005A008		//쉐도우계정 생성실패

#define	HSUSERUTIL_ERR_SETFLDRPERMISSION_FAIL	0x0005A10A		//폴더권한주기 실패
#define	HSUSERUTIL_ERR_GETGROUPSID_FAIL			0x0005A10B		//그룹 SID 얻기 실패
#define	HSUSERUTIL_ERR_GETSECINFO_FAIL			0x0005A10C		//SD와 DACL정보 얻기실패
#define	HSUSERUTIL_ERR_ADDNEWACE_FAIL			0x0005A10D		//새 ACE 생성 실패
#define	HSUSERUTIL_ERR_ADDNEWDACL_FAIL			0x0005A10E		//새 DACL 생성 실패
#define	HSUSERUTIL_ERR_COPYOLDDACL_FAIL			0x0005A10F		//새 DACL에 기존DACL 복사 실패
#define	HSUSERUTIL_ERR_ADDNEWACETONEWDACL_FAIL	0x0005A110		//새 DACL에 새 ACE 추가 실패
#define	HSUSERUTIL_ERR_GETVOLUMEINFO_FAIL		0x0005A111		//드라이브 VOLUME INFO얻기 실패.


DWORD   __stdcall _AhnHsUserUtil_CreateUser ( );
DWORD	__stdcall _AhnHsUserUtil_SetFolderPermission(LPTSTR szPath);
DWORD	__stdcall _AhnHsUserUtil_DeleteUser ( );
DWORD	__stdcall _AhnHsUserUtil_IsEnableHSAdminRights ( );
#endif //_HSUSERUTIL_H
